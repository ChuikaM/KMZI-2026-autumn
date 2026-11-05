#pragma once
#include <structures.hpp>
#include <cstdint>

void ge_identity(ge& r);
bool ge_is_identity(const ge& P);
bool ge_equal(const ge& P, const ge& Q, fe p);

void ge_tobytes(uint8_t s[32], const ge *P);
int ge_frombytes(ge *r, const uint8_t s[32]);
bool ge_on_curve(ge G, Params params);

ge ge_add(ge P, ge Q, Params params);
ge ge_doble(ge P, Params params);

void ge_cswap(ge *P, ge *Q, uint64_t bit);
void ge_scalarmult(ge *r, const scalar *k, const ge *P);
void ge_scalarmult_base(ge *r, const scalar *k);    