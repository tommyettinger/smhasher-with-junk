/*
 * tritiumhash
 *
 * author: Tommy Ettinger, 2024-07-05, github.com/tommyettinger
 * based partly on mx3 by Jon Maiga, jonkagstrom.com, @jonkagstrom
 * license: CC0 license
 */
#include "Platform.h"
#include "Hashlib.h"

 //------------------------------------------------------------
static const uint64_t C = UINT64_C(0xbea225f9eb34556d);
static const uint64_t P = UINT64_C(0xAEF17502108EF2D9);
static const uint64_t Q = UINT64_C(13166748625691186689);
static const uint64_t R = UINT64_C(1573836600196043749);
static const uint64_t S = UINT64_C(1478582680485693857);
static const uint64_t T = UINT64_C(1584163446043636637);


static inline uint64_t mix(uint64_t x) {
    x = (x ^ x >> ((x >> 59u) + 5u)) * P + C;
    return x ^ x >> 43u;
}

static inline uint64_t mix_stream(uint64_t h, uint64_t x) {
    x = (x ^ x >> ((x >> 59u) + 5u)) * P + h;
    x ^= x >> 43u;
    x = (x ^ x >> ((x >> 59u) + 5u)) * P + C;
    return x ^ x >> 43u;

}

static inline uint64_t mix_stream_bulk(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    a = a * Q + h;
    b = b * R + h;
    c = c * S + h;
    d = d * T + h;
    a ^= a >> 37;
    b ^= b >> 41;
    c ^= c >> 35;
    d ^= d >> 43;
    h = h * R + a;
    h = h * S + b;
    h = h * T + c;
    h = h * Q + d;
    h = h * C + P;
    //h = ((a + b - c - d) ^ h) * C + P;
    //h ^= h >> 39;
    return h;
}

template <bool bswap>
static inline uint64_t tritiumhash(const uint8_t* buf, size_t len, uint64_t seed) {
    const uint8_t* const tail = buf + (len & ~7);
    uint64_t h = mix_stream(len * C, seed);

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
static void tritium(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = tritiumhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(tritiumhash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(tritium,
    $.desc = "tritium hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = tritium<false>,
    $.hashfn_bswap = tritium<true>
);
