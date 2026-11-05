#include "eddsa.hpp"
#include <fe.hpp>
#include <ge.hpp>
#include <sc.hpp>
#include <sha512.hpp>
#include <cstring>
#include <data.hpp>

void eddsa_keygen(uint8_t pk[32], uint8_t sk[64], const uint8_t seed[32]) {
    uint8_t h[64];
    sha512(h, seed, 32);

    memcpy(sk, seed, 32);
    memcpy(sk + 32, h + 32, 32);        // sk = seed ‖ h[32..63]

    uint8_t a[32];
    memcpy(a, h, 32);
    a[0]  &= 248;                        // clamp
    a[31] &= 127;
    a[31] |= 64;

    scalar d;
    fe_frombytes(d, a);                  // d — BigInt

    ge A;
    ge_scalarmult_base(&A, &d);          // A = d·B

    ge_tobytes(pk, &A);                  // pk = encode(A)
}

void eddsa_sign(uint8_t sig[64], const uint8_t *m, size_t mlen,
                const uint8_t pk[32], const uint8_t sk[64]) {
    // 1. d из sk[0..31] (clamp)
    uint8_t a[32];
    memcpy(a, sk, 32);
    a[0]  &= 248;
    a[31] &= 127;
    a[31] |= 64;
    scalar d;
    fe_frombytes(d, a);

    // 2. r = SHA-512(sk[32..63] ‖ M) mod q
    uint8_t r_hash[64];
    sha512_2(r_hash, sk + 32, 32, m, mlen);
    scalar r = sc_reduce(r_hash);

    // 3. R = r·B
    ge R_ge;
    ge_scalarmult_base(&R_ge, &r);
    uint8_t R_bytes[32];
    ge_tobytes(R_bytes, &R_ge);

    // 4. h = SHA-512(R ‖ A ‖ M) mod q
    uint8_t h_hash[64];
    sha512_3(h_hash, R_bytes, 32, pk, 32, m, mlen);
    scalar h = sc_reduce(h_hash);

    // 5. S = (r + h·d) mod q
    scalar S = sc_muladd(&h, &d, &r);

    // 6. sig = R ‖ S
    memcpy(sig, R_bytes, 32);
    sc_tobytes(sig + 32, S);     // сериализация S (32 байта LE)
}

int eddsa_verify(const uint8_t sig[64], const uint8_t *m, size_t mlen,
                 const uint8_t pk[32]) {
    // 1. S из sig[32..63]
    scalar S;
    sc_frombytes(S, sig + 32);

    // 2. S != 0 и S < q
    if (sc_iszero(&S)) return 0;
    if (fe_cmp_ge(S, Q)) return 0;

    // 3. R = decode(sig[0..31]), A = decode(pk)
    ge R_ge, A_ge;
    if (!ge_frombytes(&R_ge, sig)) return 0;
    if (!ge_frombytes(&A_ge, pk)) return 0;

    // 4. h = SHA-512(R ‖ A ‖ M) mod q
    uint8_t h_hash[64];
    sha512_3(h_hash, sig, 32, pk, 32, m, mlen);
    scalar h = sc_reduce(h_hash);

    // 5. SB = S·B, hA = h·A
    ge SB, hA;
    ge_scalarmult_base(&SB, &S);
    ge_scalarmult(&hA, &h, &A_ge);

    // 6. R + h·A
    ge RhA = ge_add(R_ge, hA, GLOBAL_PARAMS);

    // 7. SB == R + h·A ?
    return ge_equal(SB, RhA, P_ED) ? 1 : 0;
}