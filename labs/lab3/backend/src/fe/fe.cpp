#include "fe.hpp"

void fe_cswap(fe& a, fe& b, Bit bit) {
    fe mask = bit ? ~fe(0) : fe(0);
    fe dummy = mask & (a ^ b);
    a = a ^ dummy;
    b = b ^ dummy;
}

bool fe_iszero(fe a) { return a == 0; }
bool fe_equal(fe a, fe b) { return a == b; }

void fe_frombytes(fe& r, const uint8_t s[32]) {
    r = 0;
    for (int i = 31; i >= 0; i--)
        r = (r << 8) | fe(s[i]);
}

void fe_tobytes(uint8_t s[32], const fe& a) {
    fe t = a;
    for (int i = 0; i < 32; i++) {
        s[i] = (uint8_t)(t & 0xFF);
        t >>= 8;
    }
}

fe fe_add(fe a, fe b, fe p) {
    fe res = a + b;
    if (res >= p) res = res - p;
    return res;
}

fe fe_sub(fe a, fe b, fe p) {
    fe res = a - b;
    if (res < 0) res = res + p;
    return res;
}

fe fe_mul(fe a, fe b, fe p) {
    return (a * b) % p;
}

fe fe_sq(fe a, fe p) {
    return fe_mul(a, a, p);
}

fe fe_pow(fe a, fe p, fe e) {
    fe result = 1;
    fe base = a;
    while (e > 0) {
        if ((e & 1) == 1) result = fe_mul(result, base, p);
        base = fe_mul(base, base, p);
        e >>= 1;
    }
    return result;
}
fe fe_inv(fe a, fe p) {
    return fe_pow(a, p, p - 2);
}

void fe_neg(fe& r, const fe& a, fe p) {
    if (a == 0) r = 0;
    else r = p - a;
}
void fe_cmov(fe& r, const fe& a, int bit) {
    fe mask = fe(0) - fe(bit & 1);
    r = (a & mask) | (r & ~mask);
}
int fe_get_lsb(const fe& a) {
    return (int)(a & 1);
}
int fe_cmp_ge(const fe& a, const fe& b) {
    return (a >= b) ? 1 : 0;
}