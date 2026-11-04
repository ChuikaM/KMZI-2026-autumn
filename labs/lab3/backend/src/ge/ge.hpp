#pragma once
#include <structures.hpp>

void ge_identity(Point& r);
bool ge_is_identity(const Point& P);
bool ge_equal(const Point *P, const Point *Q);

void ge_tobytes(uint8_t s[32], const Point *P);
int ge_frombytes(Point *r, const uint8_t s[32]);
bool ge_on_curve(Point G, Params params);

Point ge_add(Point P, Point Q, Params params);
Point ge_doble(Point P, Params params);

void ge_scalarmult(ge *r, const scalar *k, const ge *P);
void ge_scalarmult_base(ge *r, const scalar *k);
void scalar_cswap(scalar *a, scalar *b, uint64_t bit);
void ge_cswap(ge *P, ge *Q, uint64_t bit);