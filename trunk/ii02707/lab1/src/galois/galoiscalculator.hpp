#pragma once
#include <string>
#include <algorithm>

typedef uint8_t GALOIS_INT;
typedef std::string GALOIS_FIELD;

namespace galois {

// x^8+x^5+x^2+x+1 (0x12B)
GALOIS_INT galois_field_to_int(GALOIS_FIELD field) {
    
}

// p(x) = x^128+x^7+x^2+x+1

class GaloisCalculator {
public:
    explicit GaloisCalculator() = default;
    GALOIS_INT add(GALOIS_INT a, GALOIS_INT b);
    GALOIS_INT multiply(GALOIS_INT a, GALOIS_INT b, GALOIS_INT p_x);
    GALOIS_INT inverse(GALOIS_INT a, GALOIS_INT p_x);

private:
    static constexpr int FIELD_POWER { 8 };

};

}