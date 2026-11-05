#include "base.hpp"
#include <structures.hpp>
#include <sc.hpp>

static void test_sc() {
    printf("\n=== sc_* ===\n");

    uint8_t buf[32];
    for (int i = 0; i < 32; i++) buf[i] = (uint8_t)(i * 3 + 5);
    scalar v;
    sc_frombytes(v, buf);
    uint8_t buf2[32];
    sc_tobytes(buf2, v);
    CHECK(memcmp(buf, buf2, 32) == 0, "sc_tobytes(sc_frombytes(s)) == s");

    scalar zero = 0;
    scalar one = 1;
    CHECK(sc_iszero(&zero) == 1, "sc_iszero(0) == 1");
    CHECK(sc_iszero(&one) == 0, "sc_iszero(1) == 0");

    uint8_t q_bytes[64] = {0};
    sc_tobytes(q_bytes, Q);
    scalar q_red = sc_reduce(q_bytes);
    CHECK(q_red == 0, "sc_reduce(q) == 0");

    uint8_t qp1_bytes[64] = {0};
    sc_tobytes(qp1_bytes, Q + 1);
    scalar qp1_red = sc_reduce(qp1_bytes);
    CHECK(qp1_red == 1, "sc_reduce(q+1) == 1");

    scalar a = 1, b = 1, c = 0;
    CHECK(sc_muladd(&a, &b, &c) == 1, "sc_muladd(1,1,0) == 1");

    scalar qm1 = Q - 1;
    scalar zero2 = 0;
    CHECK(sc_muladd(&qm1, &qm1, &zero2) == 1, "sc_muladd(q-1, q-1, 0) == 1");

    scalar one2 = 1;
    CHECK(sc_muladd(&qm1, &one2, &one2) == 0, "sc_muladd(q-1, 1, 1) == 0");
}