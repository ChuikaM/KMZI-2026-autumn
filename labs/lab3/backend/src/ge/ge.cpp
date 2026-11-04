#include "ge.hpp"
#include <fe.hpp>


void ge_identity(Point& r) {

}
bool ge_is_identity(const Point& P) {

}
bool ge_equal(const Point *P, const Point *Q) {

}

void ge_tobytes(uint8_t s[32], const Point *P) {

}
int ge_frombytes(Point *r, const uint8_t s[32]) {

}
bool ge_on_curve(Point G, Params params) {
  BigInt x = G.x; BigInt y = G.y; 
  BigInt a = params.a; BigInt d = params.d; BigInt p = params.p;
  
  BigInt sqx = fp_mul(x, x, p);
  BigInt asqx = fp_mul(a, sqx, p);
  BigInt sqy = fp_mul(y, y, p);
  BigInt lhs = fp_add(asqx, sqy, p);
  
  BigInt dsqx = fp_mul(d, sqx, p);
  BigInt dsqxsqy = fp_mul(dsqx, sqy, p);
  BigInt rhs = fp_add(1, dsqxsqy, p);
  return lhs == rhs;
}

Point ge_add(Point P, Point Q, Params params) {
  BigInt x1 = P.x; BigInt y1 = P.y;
  BigInt x2 = Q.x; BigInt y2 = Q.y;
  BigInt a = params.a; BigInt d = params.d; BigInt p = params.p;

  BigInt x3;
  {
    BigInt x1y2 = fp_mul(x1, y2, p);
    BigInt y1x2 = fp_mul(y1, x2, p);
    BigInt numerator = fp_add(x1y2, y1x2, p);
    
    BigInt dx1 = fp_mul(d, x1, p);
    BigInt dx1x2 = fp_mul(dx1, x2, p);
    BigInt dx1x2y1 = fp_mul(dx1x2, y1, p);
    BigInt dx1x2y1y2 = fp_mul(dx1x2y1, y2, p);
    BigInt sum_denominator = fp_add(1, dx1x2y1y2, p);
    BigInt denominator = fp_inv(sum_denominator, p);
    
    x3 = fp_mul(numerator, denominator, p);
  }

  BigInt y3;
  {
    BigInt y1y2 = fp_mul(y1, y2, p);
    BigInt x1x2 = fp_mul(x1, x2, p);
    BigInt ax1x2 = fp_mul(a, x1x2, p);
    BigInt numerator = fp_sub(y1y2, ax1x2, p);
    
    BigInt dx1 = fp_mul(d, x1, p);
    BigInt dx1x2 = fp_mul(dx1, x2, p);
    BigInt dx1x2y1 = fp_mul(dx1x2, y1, p);
    BigInt dx1x2y1y2 = fp_mul(dx1x2y1, y2, p);
    BigInt sum_denominator = fp_sub(1, dx1x2y1y2, p);
    BigInt denominator = fp_inv(sum_denominator, p);
    
    y3 = fp_mul(numerator, denominator, p);
  }

  return { x3, y3 };
}

Point ge_double(Point P, Params params) {
  return ge_add(P, P, params);
}