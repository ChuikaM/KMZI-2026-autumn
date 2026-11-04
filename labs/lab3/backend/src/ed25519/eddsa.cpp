#include "eddsa.hpp"

void eddsa_keygen(Point *A, BigInt d, const Params *C) {
  
}

void eddsa_sign(Point *R, BigInt *S, const BigInt d,
                const char *M, size_t len, const Params *C) {

}

void eddsa_verify(const Point *R, const BigInt S,
                 const char *M, size_t len,
                 const Point *A, const Params *C) {

}