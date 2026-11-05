#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>

#include <structures.hpp>
#include <fe.hpp>
#include <ge.hpp>
#include <sc.hpp>
#include <sha512.hpp>
#include <eddsa.hpp>
#include <data.hpp>

static int g_fail = 0;

static std::string to_hex(const uint8_t *p, size_t n) {
    static const char *hex = "0123456789abcdef";
    std::string s(n * 2, '0');
    for (size_t i = 0; i < n; i++) {
        s[2*i]   = hex[p[i] >> 4];
        s[2*i+1] = hex[p[i] & 0xF];
    }
    return s;
}

static std::vector<uint8_t> from_hex(const std::string &h) {
    std::vector<uint8_t> out(h.size() / 2);
    for (size_t i = 0; i < out.size(); i++) {
        auto nib = [](char c) -> uint8_t {
            if (c >= '0' && c <= '9') return (uint8_t)(c - '0');
            if (c >= 'a' && c <= 'f') return (uint8_t)(c - 'a' + 10);
            if (c >= 'A' && c <= 'F') return (uint8_t)(c - 'A' + 10);
            return 0;
        };
        out[i] = (uint8_t)((nib(h[2*i]) << 4) | nib(h[2*i+1]));
    }
    return out;
}

int main() {
    printf("Ed25519 test suite\n");
    test_fe();
    test_ge();
    test_sc();
    test_sha512();
    test_eddsa();
    printf("\n=== Итого ===\n");
    if (g_fail == 0) { printf("ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n"); return 0; }
    printf("ПРОВАЛЕНО: %d\n", g_fail);
    return 1;
}