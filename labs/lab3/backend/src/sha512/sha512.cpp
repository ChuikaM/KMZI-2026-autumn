#include "sha512.hpp"
#include <cstring>

#define ROTR(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
#define SHR(x, n)  ((x) >> (n))

// Функции SHA-512 (FIPS 180-4, §4.1.3)
#define CH(x, y, z)  (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define BSIG0(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define BSIG1(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define SSIG0(x) (ROTR(x,  1) ^ ROTR(x,  8) ^ SHR(x, 7))
#define SSIG1(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ SHR(x, 6))

static void sha512_compress(sha512_ctx *ctx, const uint8_t block[128]) {
    uint64_t w[80];
    uint64_t a, b, c, d, e, f, g, h;
    int i;

    // 1. Разбор блока в 16 слов big-endian
    for (i = 0; i < 16; i++) {
        w[i] = ((uint64_t)block[i*8    ] << 56)
             | ((uint64_t)block[i*8 + 1] << 48)
             | ((uint64_t)block[i*8 + 2] << 40)
             | ((uint64_t)block[i*8 + 3] << 32)
             | ((uint64_t)block[i*8 + 4] << 24)
             | ((uint64_t)block[i*8 + 5] << 16)
             | ((uint64_t)block[i*8 + 6] <<  8)
             | ((uint64_t)block[i*8 + 7]);
    }

    // 2. Расширение до 80 слов
    for (i = 16; i < 80; i++) {
        w[i] = SSIG1(w[i-2]) + w[i-7] + SSIG0(w[i-15]) + w[i-16];
    }

    // 3. Инициализация рабочих переменных
    a = ctx->h[0]; b = ctx->h[1]; c = ctx->h[2]; d = ctx->h[3];
    e = ctx->h[4]; f = ctx->h[5]; g = ctx->h[6]; h = ctx->h[7];

    // 4. 80 раундов
    for (i = 0; i < 80; i++) {
        uint64_t t1 = h + BSIG1(e) + CH(e, f, g) + K[i] + w[i];
        uint64_t t2 = BSIG0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // 5. Прибавление к состоянию
    ctx->h[0] += a; ctx->h[1] += b; ctx->h[2] += c; ctx->h[3] += d;
    ctx->h[4] += e; ctx->h[5] += f; ctx->h[6] += g; ctx->h[7] += h;
}

void sha512_init(sha512_ctx *ctx) {
    for (int i = 0; i < 8; i++) ctx->h[i] = H0[i];
    ctx->total_len = 0;
    ctx->buf_len = 0;
}

void sha512_update(sha512_ctx *ctx, const uint8_t *data, size_t len) {
    ctx->total_len += len;

    // 1. Дополняем буфер, если он непустой
    if (ctx->buf_len > 0) {
        size_t need = 128 - ctx->buf_len;
        size_t take = (len < need) ? len : need;
        memcpy(ctx->buf + ctx->buf_len, data, take);
        ctx->buf_len += take;
        data += take;
        len  -= take;
        if (ctx->buf_len == 128) {
            sha512_compress(ctx, ctx->buf);
            ctx->buf_len = 0;
        }
    }

    // 2. Обрабатываем целые блоки напрямую из data
    while (len >= 128) {
        sha512_compress(ctx, data);
        data += 128;
        len  -= 128;
    }

    // 3. Остаток — в буфер
    if (len > 0) {
        memcpy(ctx->buf, data, len);
        ctx->buf_len = len;
    }
}

void sha512_final(sha512_ctx *ctx, uint8_t out[64]) {
    uint64_t bit_len = ctx->total_len * 8;   // младшие 64 бита длины в битах
    // старшие 64 бита длины — 0 (для сообщений < 2^61 байт)

    // 1. Дополнение: 0x80, затем нули до 112 mod 128
    uint8_t pad[128];
    size_t pad_len;

    size_t rem = ctx->buf_len;
    if (rem < 112) {
        pad_len = 112 - rem;          // влезает в текущий блок
    } else {
        pad_len = 128 + 112 - rem;    // нужен ещё один блок
    }

    memset(pad, 0, sizeof(pad));
    pad[0] = 0x80;

    // 2. Дописываем 0x80 + нули, затем 16 байт длины (big-endian)
    //    Удобно: сначала update с pad, потом отдельно length — но
    //    проще собрать всё вручную, чтобы не рекурсировать в update.

    // Собираем хвост: buf[0..rem-1] + 0x80 + нули + [16 байт длины]
    uint8_t tail[256];
    size_t tail_len = 0;
    memcpy(tail, ctx->buf, rem);
    tail_len = rem;
    tail[tail_len++] = 0x80;
    while (tail_len % 128 != 112) tail[tail_len++] = 0x00;
    // 16 байт длины (старшие 8 — нули, младшие 8 — bit_len BE)
    for (int i = 0; i < 8; i++) tail[tail_len++] = 0x00;
    for (int i = 7; i >= 0; i--) tail[tail_len++] = (uint8_t)(bit_len >> (i*8));

    // 3. Прогоняем tail блоками по 128
    for (size_t off = 0; off < tail_len; off += 128) {
        sha512_compress(ctx, tail + off);
    }

    // 4. Вывод H0..H7 в big-endian
    for (int i = 0; i < 8; i++) {
        out[i*8    ] = (uint8_t)(ctx->h[i] >> 56);
        out[i*8 + 1] = (uint8_t)(ctx->h[i] >> 48);
        out[i*8 + 2] = (uint8_t)(ctx->h[i] >> 40);
        out[i*8 + 3] = (uint8_t)(ctx->h[i] >> 32);
        out[i*8 + 4] = (uint8_t)(ctx->h[i] >> 24);
        out[i*8 + 5] = (uint8_t)(ctx->h[i] >> 16);
        out[i*8 + 6] = (uint8_t)(ctx->h[i] >>  8);
        out[i*8 + 7] = (uint8_t)(ctx->h[i]);
    }
}

void sha512(uint8_t out[64], const uint8_t *in, size_t len) {
    sha512_ctx ctx;
    sha512_init(&ctx);
    sha512_update(&ctx, in, len);
    sha512_final(&ctx, out);
}

void sha512_2(uint8_t out[64],
              const uint8_t *a, size_t alen,
              const uint8_t *b, size_t blen) {
    sha512_ctx ctx;
    sha512_init(&ctx);
    sha512_update(&ctx, a, alen);
    sha512_update(&ctx, b, blen);
    sha512_final(&ctx, out);
}

void sha512_3(uint8_t out[64],
              const uint8_t *a, size_t alen,
              const uint8_t *b, size_t blen,
              const uint8_t *c, size_t clen) {
    sha512_ctx ctx;
    sha512_init(&ctx);
    sha512_update(&ctx, a, alen);
    sha512_update(&ctx, b, blen);
    sha512_update(&ctx, c, clen);
    sha512_final(&ctx, out);
}