#include "field.hpp"
#include <bit>
#include <bitset>

void fe_cswap(BigInt& a, BigInt& b, Bit bit) {
  Bit mask = 0 - bit;
  int dummy = mask & (a ^ b);
  a = a ^ dummy;
  b = b ^ dummy;
}
bool fe_iszero(BigInt a) {

}
bool fe_equal(BigInt a, BigInt b) {

}

void fe_frombytes(BigInt& r, const uint8_t s[32]) {

}
void fe_tobytes(uint8_t s[32], const BigInt& a) {

}

BigInt fp_add(BigInt a, BigInt b, BigInt p) {
  BigInt res = a + b;
  if(res > p) res = res - p;

  return res;
}

BigInt fp_sub(BigInt a, BigInt b, BigInt p) {
  BigInt res = a - b;
  if (res < 0) res = res + p;

  return res;
}

BigInt fp_mul(BigInt a, BigInt b, BigInt p) {
  BigInt res = a * b;
  if (res > p) res = res & p;

  return res;
}


BigInt fp_pow(BigInt a, BigInt p) {
  BigInt e = p - 2;
  BigInt result = 1;
  const auto count = std::bit_width(a); 
  std::bitset<count> bits(a);
  for(std::size_t i = 0; i < count; i++) {
    result = fp_mul(result, result, p);
    if(bits[i]) result = fp_mul(result, a, p);
  }

  return result;
}
BigInt fp_inv(BigInt a, BigInt p) {
  return fp_pow(a, p);
}