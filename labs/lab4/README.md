mkdir -p ~/lab4/root ~/lab4/inter ~/lab4/server ~/lab4/client && cd ~/lab4
/opt/homebrew/opt/openssl@3/bin/openssl genpkey -algorithm ED25519 -out root/root.key && chmod 600 root/root.key
/opt/homebrew/opt/openssl@3/bin/openssl req -new -x509 -key root/root.key -days 3650 -subj '/C=BY/O=Lab PKI/CN=Lab Root CA' -addext 'basicConstraints=critical,CA:TRUE,pathlen:1' -addext 'keyUsage=critical,keyCertSign,cRLSign' -addext 'subjectKeyIdentifier=hash' -out root/root.crt

cat > inter/inter.ext <<'EOF'
basicConstraints=critical,CA:TRUE,pathlen:0
keyUsage=critical,keyCertSign,cRLSign
subjectKeyIdentifier=hash
authorityKeyIdentifier=keyid:always
EOF

/opt/homebrew/opt/openssl@3/bin/openssl genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 -out inter/inter.key && chmod 600 inter/inter.key
/opt/homebrew/opt/openssl@3/bin/openssl req -new -key inter/inter.key -subj '/C=BY/O=Lab PKI/CN=Lab Intermediate CA' -out inter/inter.csr
/opt/homebrew/opt/openssl@3/bin/openssl x509 -req -in inter/inter.csr -CA root/root.crt -CAkey root/root.key -CAcreateserial -sha256 -days 1825 -extfile inter/inter.ext -out inter/inter.crt


cd ~/lab4/inter && touch index.txt && echo 01 > crlnumber && echo 1000 > serial && cd ~/lab4
cat > inter/ca.cnf <<'EOF'
[ ca ]
default_ca = CA_default
[ CA_default ]
dir              = /Users/matvey/lab4/inter
database         = $dir/index.txt
new_certs_dir    = $dir
serial           = $dir/serial
crlnumber        = $dir/crlnumber
certificate      = $dir/inter.crt
private_key      = $dir/inter.key
default_md       = sha256
default_days     = 365
default_crl_days = 30
unique_subject   = no
copy_extensions  = none
policy           = policy_any
[ policy_any ]
commonName       = supplied
EOF


cat > server/server.ext <<'EOF'
basicConstraints=critical,CA:FALSE
keyUsage=critical,digitalSignature
extendedKeyUsage=serverAuth
subjectKeyIdentifier=hash
authorityKeyIdentifier=keyid:always
subjectAltName=DNS:localhost,IP:127.0.0.1
EOF

/opt/homebrew/opt/openssl@3/bin/openssl genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 -out server/server.key && chmod 600 server/server.key
/opt/homebrew/opt/openssl@3/bin/openssl req -new -key server/server.key -subj '/C=BY/O=Lab PKI/CN=localhost' -out server/server.csr
/opt/homebrew/opt/openssl@3/bin/openssl ca -config inter/ca.cnf -batch -in server/server.csr -extfile server/server.ext -notext -out server/server.crt


/opt/homebrew/opt/openssl@3/bin/openssl verify -CAfile root/root.crt -untrusted inter/inter.crt server/server.crt
/opt/homebrew/opt/openssl@3/bin/openssl verify -CAfile root/root.crt server/server.crt
/opt/homebrew/opt/openssl@3/bin/openssl x509 -in server/server.crt -noout -ext subjectAltName,extendedKeyUsage,basicConstraints



server {
    listen 8443 ssl;
    server_name localhost;
    ssl_certificate     /home/matvey/lab4/server/fullchain.pem;
    ssl_certificate_key /home/matvey/lab4/server/server.key;
    ssl_protocols TLSv1.3;
    ssl_conf_command Ciphersuites TLS_AES_256_GCM_SHA384;
    ssl_ecdh_curve X25519:P-256;
    ssl_session_tickets on;
    location / { return 200 'tls13 ok'; }
}


/opt/homebrew/opt/curl/bin/curl -v --cacert root/root.crt https://localhost:8443/

/opt/homebrew/opt/curl/bin/curl https://localhost:8443/
/opt/homebrew/opt/curl/bin/curl --cacert root/root.crt https://127.0.0.2:8443/
/opt/homebrew/opt/openssl@3/bin/openssl genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 -out /tmp/other.key
/opt/homebrew/opt/openssl@3/bin/openssl req -new -x509 -key /tmp/other.key -subj '/CN=Other Root' -days 1 -out /tmp/other.crt
/opt/homebrew/opt/curl/bin/curl --cacert /tmp/other.crt https://localhost:8443/
( sleep 1 ) | /opt/homebrew/opt/openssl@3/bin/openssl s_client -connect localhost:8443 -tls1_2 -CAfile root/root.crt 2>&1 | grep -E 'alert|Protocol'
