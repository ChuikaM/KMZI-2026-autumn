#pragma once

using BigInt = int;
using Bit = bool;

struct Point {
  BigInt x;
  BigInt y;
  BigInt z;
};

struct Params {
  BigInt a;
  BigInt d;
  BigInt p;
};

typedef struct { uint64_t v[4]; } fe;      // элемент GF(2^255-19)
typedef struct { fe X, Y, Z; } ge;         // точка (X:Y:Z)
typedef struct { uint64_t v[4]; } scalar; 