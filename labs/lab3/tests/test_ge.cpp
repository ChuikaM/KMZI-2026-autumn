#include "base.hpp"
#include <structures.hpp>
#include <ge.hpp>

static void test_ge() {
    printf("\n=== ge_* ===\n");

    ge O;
    ge_identity(O);
    CHECK(ge_is_identity(O) == true, "ge_is_identity(O)");

    ge P = BASE_POINT;
    ge OP = ge_add(O, P, GLOBAL_PARAMS);
    CHECK(ge_equal(OP, P, P_ED) == true, "O + P == P");

    ge PO = ge_add(P, O, GLOBAL_PARAMS);
    CHECK(ge_equal(PO, P, P_ED) == true, "P + O == P");

    CHECK(ge_on_curve(BASE_POINT, GLOBAL_PARAMS) == true, "B on curve");

    ge B2_add = ge_add(P, P, GLOBAL_PARAMS);
    ge B2_dbl = ge_double(P, GLOBAL_PARAMS);
    CHECK(ge_equal(B2_add, B2_dbl, P_ED) == true, "B+B == 2B");

    uint8_t enc[32];
    ge_tobytes(enc, &P);
    ge P2;
    int ok = ge_frombytes(&P2, enc);
    CHECK(ok == 1, "ge_frombytes(ge_tobytes(B)) ok");
    CHECK(ge_equal(P, P2, P_ED) == true, "round-trip B");

    uint8_t bad[32];
    memset(bad, 0xFF, 32);
    bad[31] &= 0x7F;
    ge badp;
    int bad_ok = ge_frombytes(&badp, bad);
    CHECK(bad_ok == 0 || ge_on_curve(badp, GLOBAL_PARAMS),
          "ge_frombytes(garbage) -> 0 или точка на кривой");

    scalar one = 1;
    ge oneB;
    ge_scalarmult(&oneB, &one, &P);
    CHECK(ge_equal(oneB, P, P_ED) == true, "1·B == B");

    scalar two = 2;
    ge twoB;
    ge_scalarmult(&twoB, &two, &P);
    CHECK(ge_equal(twoB, B2_dbl, P_ED) == true, "2·B == 2B (double)");

    ge baseB;
    ge_scalarmult_base(&baseB, &one);
    CHECK(ge_equal(baseB, P, P_ED) == true, "scalarmult_base(1) == B");
    
    scalar sa = 12345, sb = 67890;
    ge aB, bB, abB, sumB;
    ge_scalarmult(&aB, &sa, &P);
    ge_scalarmult(&bB, &sb, &P);
    scalar sab = sa + sb;
    ge_scalarmult(&abB, &sab, &P);
    sumB = ge_add(aB, bB, GLOBAL_PARAMS);
    CHECK(ge_equal(abB, sumB, P_ED) == true, "(a+b)·B == a·B + b·B");
}