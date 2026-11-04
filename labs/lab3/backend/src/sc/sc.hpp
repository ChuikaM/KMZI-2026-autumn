#pragma once
#include <structures.hpp>

void sc_reduce(scalar *r, const uint8_t s[64]);
void sc_muladd(scalar *r, const scalar *a, const scalar *b, const scalar *c); // a*b+c mod q
int  sc_iszero(const scalar *a);