#include "ge.hpp"
#include <fe.hpp>
#include <data.hpp>

void ge_identity(ge& r) {
  r.x = 0;
  r.y = 1;
  r.z = 1;
}
bool ge_is_identity(const ge& P) {
  return fe_iszero(P.x) && fe_equal(P.y, P.z);
}
bool ge_equal(const ge& P, const ge& Q, fe p) {
  fe t1 = fe_mul(P.x, Q.z, p);
  fe t2 = fe_mul(Q.x, P.z, p);
  fe t3 = fe_mul(P.y, Q.z, p);
  fe t4 = fe_mul(Q.y, P.z, p);
  return fe_equal(t1, t2) && fe_equal(t3, t4);
}

void ge_tobytes(uint8_t s[32], const ge *P) {
    fe_tobytes(s, P->y);                    // 32 байта y (LE)
    int sign = fe_get_lsb(P->x);            // 0 или 1, constant-time
    s[31] |= (uint8_t)(sign << 7);          // знак x в старший бит
}
int ge_frombytes(ge *r, const uint8_t s[32]) {
    // 1. Знак x — старший бит последнего байта
    int sign = (s[31] >> 7) & 1;

    // 2. y из 32 байт (LE), старший бит обнулить
    uint8_t tmp[32];
    for (int i = 0; i < 32; i++) tmp[i] = s[i];
    tmp[31] &= 0x7F;                    // маска, не if
    fe y;
    fe_frombytes(y, tmp);

    // 3. Проверка y < p (каноничность)
    if (fe_cmp_ge(y, P_ED)) return 0;   // публичные данные — if допустим
                                        // (если хочешь constant-time — через маску,
                                        //  но здесь это не секрет)

    // 4. y2 = y^2
    fe y2 = fe_mul(y, y, P_ED);

    // 5. num = y2 - 1
    fe num = fe_sub(y2, ONE, P_ED);

    // 6. den = d*y2 + 1
    fe dy2 = fe_mul(D_ED, y2, P_ED);
    fe den = fe_add(dy2, ONE, P_ED);

    // 7. x2 = num / den = num * den^{-1}
    fe den_inv = fe_inv(den, P_ED);
    fe x2 = fe_mul(num, den_inv, P_ED);

    // 8. x = x2^{(p+3)/8}
    fe x = fe_pow(x2, get_exp_p38(), P_ED);

    // 9. Проверка: x^2 == x2?
    fe x_sq = fe_mul(x, x, P_ED);
    int ok = fe_equal(x_sq, x2);

    // Если не ok — умножить на sqrt(-1) и проверить снова
    fe x_alt = fe_mul(x, SQRT_M1, P_ED);
    fe_cmov(x, x_alt, !ok);             // constant-time выбор
    x_sq = fe_mul(x, x, P_ED);
    int ok2 = fe_equal(x_sq, x2);
    if (!ok2) return 0;                 // точка не на кривой

    // 10. Выбор знака x по sign
    int lsb = fe_get_lsb(x);
    fe x_neg = fe_sub(P_ED, x, P_ED);   // p - x
    fe_cmov(x, x_neg, lsb ^ sign);      // если lsb != sign → x = p - x

    // 11. Отказ: x == 0 && sign == 1 (стандарт Ed25519)
    if (fe_iszero(x) && sign) return 0;

    // 12. Заполнить
    r->x = x;
    r->y = y;
    return 1;
}
bool ge_on_curve(ge G, Params params) {
  fe x = G.x; fe y = G.y; 
  fe a = params.a; fe d = params.d; fe p = params.p;
  
  fe sqx = fe_mul(x, x, p);
  fe asqx = fe_mul(a, sqx, p);
  fe sqy = fe_mul(y, y, p);
  fe lhs = fe_add(asqx, sqy, p);
  
  fe dsqx = fe_mul(d, sqx, p);
  fe dsqxsqy = fe_mul(dsqx, sqy, p);
  fe rhs = fe_add(1, dsqxsqy, p);
  return lhs == rhs;
}

ge ge_add(ge P, ge Q, Params params) {
  fe x1 = P.x; fe y1 = P.y;
  fe x2 = Q.x; fe y2 = Q.y;
  fe a = params.a; fe d = params.d; fe p = params.p;

  fe x3;
  {
    fe x1y2 = fe_mul(x1, y2, p);
    fe y1x2 = fe_mul(y1, x2, p);
    fe numerator = fe_add(x1y2, y1x2, p);
    
    fe dx1 = fe_mul(d, x1, p);
    fe dx1x2 = fe_mul(dx1, x2, p);
    fe dx1x2y1 = fe_mul(dx1x2, y1, p);
    fe dx1x2y1y2 = fe_mul(dx1x2y1, y2, p);
    fe sum_denominator = fe_add(1, dx1x2y1y2, p);
    fe denominator = fe_inv(sum_denominator, p);
    
    x3 = fe_mul(numerator, denominator, p);
  }

  fe y3;
  {
    fe y1y2 = fe_mul(y1, y2, p);
    fe x1x2 = fe_mul(x1, x2, p);
    fe ax1x2 = fe_mul(a, x1x2, p);
    fe numerator = fe_sub(y1y2, ax1x2, p);
    
    fe dx1 = fe_mul(d, x1, p);
    fe dx1x2 = fe_mul(dx1, x2, p);
    fe dx1x2y1 = fe_mul(dx1x2, y1, p);
    fe dx1x2y1y2 = fe_mul(dx1x2y1, y2, p);
    fe sum_denominator = fe_sub(1, dx1x2y1y2, p);
    fe denominator = fe_inv(sum_denominator, p);
    
    y3 = fe_mul(numerator, denominator, p);
  }

  return { x3, y3 };
}

ge ge_double(ge P, Params params) {
  return ge_add(P, P, params);
}


void ge_cswap(ge *P, ge *Q, uint64_t bit) {
    fe_cswap(P->x, Q->x, bit);
    fe_cswap(P->y, Q->y, bit);
    // z не трогаем — в аффинных формулах он всегда 1
}

void ge_scalarmult(ge *r, const scalar *k, const ge *P) {
    ge R0, R1;
    ge_identity(R0);            // R0 = O
    R1 = *P;                    // R1 = P

    for (int i = 255; i >= 0; i--) {
        uint64_t bit = (uint64_t)((*k >> i) & 1);
        ge_cswap(&R0, &R1, bit);
        R1 = ge_add(R0, R1, GLOBAL_PARAMS);
        R0 = ge_double(R0, GLOBAL_PARAMS);
        ge_cswap(&R0, &R1, bit);
    }
    *r = R0;
}

void ge_scalarmult_base(ge *r, const scalar *k) {
    ge_scalarmult(r, k, &BASE_POINT);
}