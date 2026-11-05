#pragma once
#include <boost/multiprecision/cpp_int.hpp>

using fe = boost::multiprecision::uint256_t;
using Bit = bool;

typedef struct ge {
  fe x;
  fe y;
  fe z;
};

using scalar = fe;

typedef struct Params {
  fe a;
  fe d;
  fe p;
};