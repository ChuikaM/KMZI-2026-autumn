#pragma once
#include <structures.hpp>
#include <cstdint>

extern const scalar Q;

scalar sc_reduce(const uint8_t s[64]);
scalar sc_muladd(const scalar *a, const scalar *b, const scalar *c); // a*b+c mod q
int  sc_iszero(const scalar *a);

void sc_frombytes(scalar& r, const uint8_t s[32]);
void sc_tobytes(uint8_t s[32], const scalar& a);