#pragma once
#include <structures.hpp>
#include <cstdlib>

void eddsa_keygen(uint8_t pk[32], uint8_t sk[64], const uint8_t seed[32]);
void eddsa_sign(uint8_t sig[64], const uint8_t *m, size_t mlen,
                const uint8_t pk[32], const uint8_t sk[64]);
int eddsa_verify(const uint8_t sig[64], const uint8_t *m, size_t mlen,
                 const uint8_t pk[32]);