#pragma once

#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("[FAIL] %s\n", msg); g_fail++; } \
    else         { printf("[ OK ] %s\n", msg); } \
} while (0)