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

/*
*********FAIL*********

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         3554  1056   468   240   135    71    27    18    16     6     3     4     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2    10     1     0     2     2     3     0     2     1     1     2  1258

----------------------------------------------------------------------------------------------
Summary for: xquor
Overall result: FAIL            ( 83 / 188 passed)
Failures:
    Avalanche           : [3, 4, 7, 9, 10]
    BIC                 : [3, 11, 15]
    Sparse              : [6/2, 4/3, 4/4, 4/5, 3/6, 3/7, 3/9, 3/10, 3/12, 3/14, 10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE]]
    TextNum             : [with commas]
    Text                : [FooBarXXXX, FooooooBaaaaarXXXX, FooooooooooBaaaaaaaaarXXXX, Words alnum 1-4, Words alnum 1-16, Words alnum 1-32, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [20, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [3, 4]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]
    Seed                : [2, 3, 6]
    SeedAvalanche       : [4]
    SeedBIC             : [3]
    SeedBitflip         : [3, 4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 367.481270 second
*/

//------------------------------------------------------------
// MX3 multiplier
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);
static const uint64_t O = UINT64_C(7);

static const uint64_t Q = UINT64_C(0x9E3779B97F4A7C55);
static const uint64_t R = UINT64_C(0xC13FA9A902A63293);
static const uint64_t S = UINT64_C(0xD1B54A32D192ED2D);
static const uint64_t T = UINT64_C(0xDB4F0B9175AE2169);
static const uint64_t U = UINT64_C(0xE19B01AA9D42C66D);
static const uint64_t V = UINT64_C(0xE60E2B722B53AEF3);
static const uint64_t W = UINT64_C(0xE95E1DD17D35802B);
static const uint64_t X = UINT64_C(0xEBEDEED9D803C871);

static inline uint64_t mix(uint64_t x) {
    
    constexpr int R = 27;
    x ^= x * x | O;
    x = ROTR64(x, R);
    x ^= x * x | O;
    x ^= x >> R;
    return x;
}

static inline uint64_t mix_stream(uint64_t h, uint64_t x) {
    constexpr int R = 27;
    x ^= x * x | O;
    h += ROTR64(x, R);
    h ^= h * h | O;
    return h ^ h >> R;
}

template <bool bswap>
static inline uint64_t xquorhash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int R1 = 37;
    constexpr int R2 = 25;

    const uint8_t* const tail = buf + (len & ~7);
    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = ((len ^ ROTL64(len, 3) ^ ROTL64(len, 47)) ^ (seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56))),
        a = ROTL64(s, 11), b = ROTL64(s, 23), c = ROTL64(s, 36), d = ROTL64(s, 50);
    
    while (len >= 64) {
        len -= 64;
        s += 
            Q * GET_U64<bswap>(buf, 0) + 
            R * GET_U64<bswap>(buf, 8) + 
            S * GET_U64<bswap>(buf, 16) + 
            T * GET_U64<bswap>(buf, 24) + 
            U * GET_U64<bswap>(buf, 32) + 
            V * GET_U64<bswap>(buf, 40) + 
            W * GET_U64<bswap>(buf, 48) + 
            X * GET_U64<bswap>(buf, 56);
        s ^= s >> R2;
        s ^= s * s | O;
        buf += 64;
    }

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
    $.verification_LE = 0x23B3E373,
    $.verification_BE = 0x3A8E6591,
    $.hashfn_native = xquor<false>,
    $.hashfn_bswap = xquor<true>
);
