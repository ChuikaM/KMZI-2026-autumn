#include "base.hpp"
#include <structures.hpp>
#include <fe.hpp>

static void test_fe() {
    printf("\n=== fe_* ===\n");

    fe a = P_ED - 1;
    fe b = 1;
    CHECK(fe_add(a, b, P_ED) == 0, "fe_add(p-1, 1) == 0");

    CHECK(fe_sub(0, 1, P_ED) == P_ED - 1, "fe_sub(0, 1) == p-1");

    CHECK(fe_mul(a, a, P_ED) == 1, "fe_mul(p-1, p-1) == 1");

    fe inv = fe_inv(a, P_ED);
    CHECK(fe_mul(a, inv, P_ED) == 1, "fe_mul(a, fe_inv(a)) == 1");

    CHECK(fe_sq(2, P_ED) == 4, "fe_sq(2) == 4");

    fe e = P_ED - 1;
    fe r;
    fe_pow(r, 2, e, P_ED);
    CHECK(r == 1, "fe_pow(2, p-1) == 1 (Ферма)");

    CHECK(fe_iszero(0) == true,  "fe_iszero(0)");
    CHECK(fe_iszero(1) == false, "fe_iszero(1) == false");
    CHECK(fe_equal(5, 5) == true, "fe_equal(5,5)");
    CHECK(fe_equal(5, 6) == false, "fe_equal(5,6) == false");

    fe x = 5, y = 7;
    fe_cswap(x, y, 1);
    CHECK(x == 7 && y == 5, "fe_cswap(5,7,1) -> (7,5)");
    fe_cswap(x, y, 0);
    CHECK(x == 7 && y == 5, "fe_cswap(7,5,0) -> no swap");

    fe z = 10;
    fe_cmov(z, fe(42), 1);
    CHECK(z == 42, "fe_cmov(10, 42, 1) -> 42");
    fe_cmov(z, fe(99), 0);
    CHECK(z == 42, "fe_cmov(42, 99, 0) -> 42");

    CHECK(fe_get_lsb(0) == 0, "fe_get_lsb(0) == 0");
    CHECK(fe_get_lsb(1) == 1, "fe_get_lsb(1) == 1");
    CHECK(fe_get_lsb(2) == 0, "fe_get_lsb(2) == 0");
    CHECK(fe_get_lsb(3) == 1, "fe_get_lsb(3) == 1");

    CHECK(fe_cmp_ge(5, 3) == 1, "fe_cmp_ge(5,3) == 1");
    CHECK(fe_cmp_ge(3, 5) == 0, "fe_cmp_ge(3,5) == 0");
    CHECK(fe_cmp_ge(5, 5) == 1, "fe_cmp_ge(5,5) == 1");

    uint8_t buf[32];
    for (int i = 0; i < 32; i++) buf[i] = (uint8_t)(i * 7 + 1);
    fe v;
    fe_frombytes(v, buf);
    uint8_t buf2[32];
    fe_tobytes(buf2, v);
    CHECK(memcmp(buf, buf2, 32) == 0, "fe_tobytes(fe_frombytes(s)) == s");
}