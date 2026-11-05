#pragma once
#include <structures.hpp>
#include <cstdint>

void fe_cswap(fe& a, fe& b, Bit bit);
bool fe_iszero(fe a);
bool fe_equal(fe a, fe b);

void fe_frombytes(fe& r, const uint8_t s[32]);
void fe_tobytes(uint8_t s[32], const fe& a);

fe fe_add(fe a, fe b, fe p);
fe fe_sub(fe a, fe b, fe p);
fe fe_mul(fe a, fe b, fe p);
fe fe_sq(fe a, fe p);

fe fe_pow(fe a, fe p, fe e);
fe fe_inv(fe a, fe p);

void fe_neg(fe& r, const fe& a, fe p);
void fe_cmov(fe& r, const fe& a, int bit);
int  fe_get_lsb(const fe& a);
int  fe_cmp_ge(const fe& a, const fe& b);  