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
static const uint64_t Q = UINT64_C(0xD);
static const uint64_t R = UINT64_C(0x5);
static const uint64_t S = UINT64_C(0x7);
static const uint64_t T = UINT64_C(0xB);

static inline uint64_t mix(uint64_t x) {
    constexpr uint64_t O = 7UL;
    constexpr int R = 27;
    x ^= x * x | O;
    x = ROTR64(x, R);
    x ^= x * x | O;
    x ^= x >> R;
    return x;
}

static inline uint64_t mix_stream(uint64_t h, uint64_t x) {
    constexpr uint64_t O = 7UL;
    constexpr int R = 27;
    x ^= x * x | O;
    x = ROTR64(x, R);
    h += x ^ (x * x | O);
    h ^= h * h | O;
    return h;
}

static inline uint64_t mix_stream_bulk(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    //constexpr int R2 = 29;
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
    uint64_t h = ((len ^ ROTL64(len, 3) ^ ROTL64(len, 47)) ^ (seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56)));
    
    while (len >= 64) {
        h = ROTL64(h, R1);
        len -= 64;
        h = mix_stream_bulk(h, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8),
            GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        h = mix_stream_bulk(h, GET_U64<bswap>(buf, 32), GET_U64<bswap>(buf, 40),
            GET_U64<bswap>(buf, 48), GET_U64<bswap>(buf, 56));
        buf += 64;
    }

    while (len >= 8) {
        len -= 8;
        h = mix_stream(h, GET_U64<bswap>(buf, 0));
        buf += 8;
    }

    const uint8_t* const tail8 = buf;
    switch (len) {
    case 0: return mix(h);
    case 1: return mix(mix_stream(h, tail8[0]));
    case 2: return mix(mix_stream(h, GET_U16<bswap>(tail8, 0)));
    case 3: return mix(mix_stream(h, GET_U16<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[2]) << 16));
    case 4: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0)));
    case 5: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[4]) << 32));
    case 6: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32));
    case 7: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32 | static_cast<uint64_t>(tail8[6]) << 48));
    default:;
    }

    return mix(h);
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
    $.verification_LE = 0xA1801A0F,
    $.verification_BE = 0x648BB3D6,
    $.hashfn_native = xquor<false>,
    $.hashfn_bswap = xquor<true>
);
