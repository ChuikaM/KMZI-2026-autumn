#include "sc.hpp"

static scalar make_Q() {
    scalar q = 0;
    q += scalar(1) << 252;                    // 2^252
    q += 27742317777372353535851937790883648493; // остаток
    return q;
}
const scalar Q = make_Q();

// ---- constant-time сравнение a >= b ----
static scalar ct_mask_ge(const scalar &a, const scalar &b) {
    scalar diff = a - b;
    int neg = (int)((diff >> 255) & 1);        // 1 если a < b
    scalar t = scalar(0) - scalar(neg);        // neg=1 → all1, neg=0 → 0
    return ~t;                                  // neg=1 → 0, neg=0 → all1
}

// ---- sc_reduce ----
scalar sc_reduce(const uint8_t s[64]) {
    scalar x = 0;
    for (int i = 63; i >= 0; i--) x = (x << 8) | scalar(s[i]);
    // вход — публичный хэш, timing допустим
    return x % Q;
}

// ---- sc_muladd ----
scalar sc_muladd(const scalar *a, const scalar *b, const scalar *c) {
    scalar t = (*a) * (*b) + (*c);     // < 2^505
    scalar r = 0;
    for (int i = 504; i >= 0; i--) {
        r <<= 1;
        r |= (t >> i) & 1;
        scalar d = r - Q;
        scalar mask = ct_mask_ge(r, Q);
        r = (d & mask) | (r & ~mask);
    }
    return r;
}

// ---- sc_iszero ----
int sc_iszero(const scalar *a) {
    scalar neg = -(*a);
    scalar acc = (*a) | neg;
    int nonzero = (int)((acc >> 255) & 1);   // старший бит
    return 1 - nonzero;
}

void sc_frombytes(scalar& r, const uint8_t s[32]) {
    r = 0;
    for (int i = 31; i >= 0; i--) {
        r = (r << 8) | scalar(s[i]);
    }
}

void sc_tobytes(uint8_t s[32], const scalar& a) {
    scalar t = a;
    for (int i = 0; i < 32; i++) {
        s[i] = (uint8_t)(t & 0xFF);
        t >>= 8;
    }
}