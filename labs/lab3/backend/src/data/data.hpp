#pragma once
#include <structures.hpp>

static const fe P_ED    = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed;
static const fe D_ED    = 0x52036cee2b6ffe738cc740797779e89800700a4d4141d8ab75eb4dca135978a3;
static const fe SQRT_M1 = 0x2b8324804fc1df0b2b4d00993dfbd7a72f431806ad2fe478c4ee1b274a0ea0b0;
static const fe ONE = 1;

static const fe A_ED = P_ED - 1;

inline fe get_exp_p38() {
    return (P_ED + 3) / 8;
}
inline fe get_exp_p14() {
    return (P_ED - 1) / 4;
}

static const Params GLOBAL_PARAMS = { A_ED, D_ED, P_ED };

static const ge BASE_POINT = {
    fe("15112221349535400772501151409588531511454012693041857206046113283949847762202"),
    fe("46316835694926478169428394003475163141307993866256225615783033603165251855960"),
    fe(1)
};