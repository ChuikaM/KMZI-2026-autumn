#pragma once
#include <structures.hpp>

void fe_cswap(BigInt& a, BigInt& b, Bit bit);
bool fe_iszero(BigInt a);
bool fe_equal(BigInt a, BigInt b);

void fe_frombytes(BigInt& r, const uint8_t s[32]);
void fe_tobytes(uint8_t s[32], const BigInt& a);

BigInt fp_add(BigInt a, BigInt b, BigInt p);
BigInt fp_sub(BigInt a, BigInt b, BigInt p);
BigInt fp_mul(BigInt a, BigInt b, BigInt p);

BigInt fp_pow(BigInt a, BigInt p, BigInt e);
BigInt fp_inv(BigInt a, BigInt p);