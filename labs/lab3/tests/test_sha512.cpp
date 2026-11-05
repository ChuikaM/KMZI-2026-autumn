#include "base.hpp"
#include <structures.hpp>
#include <sha512.hpp>

static void test_sha512() {
    printf("\n=== sha512 ===\n");

    uint8_t out[64];
    sha512(out, (const uint8_t*)"", 0);
    std::string h = to_hex(out, 64);
    CHECK(h == "cf83e1357eefb8bdf1542850d66d8007"
              "d620e4050b5715dc83f4a921d36ce9ce"
              "47d0d13c5d85f2b0ff8318d2877eec2f"
              "63b931bd47417a81a538327af927da3e",
          "SHA-512(\"\")");

    sha512(out, (const uint8_t*)"abc", 3);
    h = to_hex(out, 64);
    CHECK(h == "ddaf35a193617abacc417349ae204131"
              "12e6fa4e89a97ea20a9eeee64b55d39a"
              "2192992a274fc1a836ba3c23a3feebbd"
              "454d4423643ce80e2a9ac94fa54ca49f",
          "SHA-512(\"abc\")");

    uint8_t out2[64];
    sha512_2(out2, (const uint8_t*)"ab", 2, (const uint8_t*)"c", 1);
    CHECK(memcmp(out, out2, 64) == 0, "sha512_2(ab, c) == sha512(abc)");

    uint8_t out3[64];
    sha512_3(out3, (const uint8_t*)"a", 1, (const uint8_t*)"b", 1,
             (const uint8_t*)"c", 1);
    CHECK(memcmp(out, out3, 64) == 0, "sha512_3(a,b,c) == sha512(abc)");
}