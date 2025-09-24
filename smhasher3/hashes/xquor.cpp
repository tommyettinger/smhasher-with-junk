/*
 * xquorhash
 *
 * author: Tommy Ettinger, 2024-07-05, github.com/tommyettinger
 * based partly on mx3 by Jon Maiga, jonkagstrom.com, @jonkagstrom
 * uses moremur by Pelle Evensen, https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
 * license: CC0 license
 */
#include "Platform.h"
#include "Hashlib.h"


 //------------------------------------------------------------
// MX3 multiplier
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);
static const uint64_t Q = UINT64_C(7);
static const uint64_t R = UINT64_C(11);
static const uint64_t S = UINT64_C(13);
static const uint64_t T = UINT64_C(19);

static inline uint64_t mix(uint64_t x) {
    constexpr int R = 27;
    x ^= x * x | Q;
    x = ROTR64(x, R);
    x ^= x * x | Q;
    x ^= x >> R;
    return x;
}

static inline uint64_t mix_stream(uint64_t h, uint64_t x) {
    constexpr uint64_t O = 7UL;
    constexpr int R = 27;
    x ^= x * x | Q;
    h += ROTR64(x, R);
    h ^= h * h | Q;
    return h ^ h >> R;
}

static inline uint64_t mix_stream_bulk(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    h += a ^ (a * a | Q);
    h += b ^ (b * b | R);
    h += c ^ (c * c | S);
    h += d ^ (d * d | T);
    return h;
}

template <bool bswap>
static inline uint64_t xquorhash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int R1 = 37;

    const uint8_t* const tail = buf + (len & ~7);
    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = ((len ^ ROTL64(len, 3) ^ ROTL64(len, 47)) ^ (seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56))),
        a = ROTL64(s, 11), b = ROTL64(s, 23), c = ROTL64(s, 36), d = ROTL64(s, 50);
    
    while (len >= 64) {
        len -= 64;
        a ^= GET_U64<bswap>(buf, 0);
        b ^= GET_U64<bswap>(buf, 8);
        c ^= GET_U64<bswap>(buf, 16);
        d ^= GET_U64<bswap>(buf, 24);
        a ^= a * a | Q;
        b ^= b * b | R;
        c ^= c * c | S;
        d ^= d * d | T;
        a ^= GET_U64<bswap>(buf, 32);
        b ^= GET_U64<bswap>(buf, 40);
        c ^= GET_U64<bswap>(buf, 48);
        d ^= GET_U64<bswap>(buf, 56);
        a ^= a >> R1;
        b ^= b >> R1;
        c ^= c >> R1;
        d ^= d >> R1;
        buf += 64;
    }
    s = a * Q + b * R + c * S + d * T;

    while (len >= 8) {
        len -= 8;
        s = mix_stream(s, GET_U64<bswap>(buf, 0));
        buf += 8;
    }

    const uint8_t* const tail8 = buf;
    switch (len) {
    case 0: return mix(s);
    case 1: return (mix_stream(s, tail8[0]));
    case 2: return (mix_stream(s, GET_U16<bswap>(tail8, 0)));
    case 3: return (mix_stream(s, GET_U16<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[2]) << 16));
    case 4: return (mix_stream(s, GET_U32<bswap>(tail8, 0)));
    case 5: return (mix_stream(s, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[4]) << 32));
    case 6: return (mix_stream(s, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32));
    case 7: return (mix_stream(s, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32 | static_cast<uint64_t>(tail8[6]) << 48));
    default:;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void xquor(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = xquorhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(xquorhash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(xquor,
    $.desc = "xquor hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0xA6503033,
    $.verification_BE = 0xB4347E27,
    $.hashfn_native = xquor<false>,
    $.hashfn_bswap = xquor<true>
);
