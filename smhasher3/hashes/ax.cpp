/*
 * axhash
 *
 * original author: Jon Maiga, 2020-08-03, jonkagstrom.com, @jonkagstrom
 * additional author: Tommy Ettinger, 2024-07-05, github.com/tommyettinger
 * license: CC0 license
 */
#include "Platform.h"
#include "Hashlib.h"

 //------------------------------------------------------------
static const uint64_t C = UINT64_C(0xbea225f9eb34556d);

static inline uint64_t mix(uint64_t x) {
    constexpr uint32_t R0 = 32;
    constexpr uint32_t R1 = 29;
    constexpr uint32_t R2 = 32;
    constexpr uint32_t R3 = 29;
    x ^= x >> R0;
    x *= C;
    x ^= x >> R1;
    x *= C;
    x ^= x >> R2;
    x *= C;
    x ^= x >> R3;
    return x;
}

static inline uint64_t mix_stream(uint64_t h, uint64_t x) {
    constexpr uint32_t R1 = 39;

    x *= C;
    x ^= (x >> R1);
    h += x * C;
    h *= C;
    return h;
}

static inline uint64_t mix_stream_bulk(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    a *= C;
    b *= C;
    c *= C;
    d *= C;
    a ^= a >> 39;
    b ^= b >> 39;
    c ^= c >> 39;
    d ^= d >> 39;
    h += a * C;
    h *= C;
    h += b * C;
    h *= C;
    h += c * C;
    h *= C;
    h += d * C;
    h *= C;
    return h;
}

template <bool bswap>
static inline uint64_t axhash(const uint8_t* buf, size_t len, uint64_t seed) {
    const uint8_t* const tail = buf + (len & ~7);

    //uint64_t h = mix_stream(seed, len + 1);
    uint64_t h = mix(seed + mix(len + C));


    while (len >= 64) {
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
static void ax(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = axhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(axhash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(ax,
    $.desc = "ax",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0xF6FC34B5,
    $.verification_BE = 0x9E78CD56,
    $.hashfn_native = ax<false>,
    $.hashfn_bswap = ax<true>
);
