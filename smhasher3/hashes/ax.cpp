/*
 * axhash
 *
 * original author: Jon Maiga, 2020-08-03, jonkagstrom.com, @jonkagstrom
 * additional author: Tommy Ettinger, 2024-07-05, github.com/tommyettinger
 * license: CC0 license
 */
#include "Platform.h"
#include "Hashlib.h"

/*
Results from August 31, 2024:
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4433  1269   591   297   151    75    29    22    13     1     1     1     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: ax
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 419.459933 seconds

Results from September 7, 2024:
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4450  1237   618   268   146    85    44    16     6     7     4     0     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: ax
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 418.447033 seconds
*/

 //------------------------------------------------------------
// Moremur multipliers
static const uint64_t A = UINT64_C(0x3C79AC492BA7B653);
static const uint64_t B = UINT64_C(0x1C69B3F74AC4AE35);
// MX3 multiplier
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);
// Random 64-bit probable primes, as given by Java's BigInteger class.
static const uint64_t Q = UINT64_C(0xD1B92B09B92266DD);
static const uint64_t R = UINT64_C(0x9995988B72E0D285);
static const uint64_t S = UINT64_C(0x8FADF5E286E31587);
static const uint64_t T = UINT64_C(0xFCF8B405D3D0783B);

//// Moremur unary hash, by Pelle Evensen
//// https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
//static inline uint64_t mix(uint64_t x) {
//    constexpr int R0 = 27;
//    constexpr int R1 = 33;
//    constexpr int R2 = 27;
//    x ^= x >> R0;
//    x *= A;
//    x ^= x >> R1;
//    x *= B;
//    x ^= x >> R2;
//    return x;
//}

static inline uint64_t mix(uint64_t x) {
    constexpr int R0 = 23;
    constexpr int R1 = 43;
    constexpr int R2 = 11;
    constexpr int R3 = 50;
    x = (x ^ ROTL64(x, R0) ^ ROTL64(x, R1));
    x *= C;
    x = (x ^ ROTL64(x, R2) ^ ROTL64(x, R3));
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
    constexpr int R2 = 29;
    h += (ROTL64(a, R2) - c) * Q;
    h += (ROTL64(b, R2) - d) * R;
    h += (ROTL64(c, R2) - b) * S;
    h += (ROTL64(d, R2) - a) * T;
    return h;
}

template <bool bswap>
static inline uint64_t axhash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int Q1 = 29;
    constexpr int Q2 = 47;
    constexpr int R1 = 37;

    const uint8_t* const tail = buf + (len & ~7);
    uint64_t h = len ^ seed ^ ROTL64(seed, Q1) ^ ROTL64(seed, Q2);
    //uint64_t h = ((len ^ ROTL64(len, 3) ^ ROTL64(len, 47)) + (seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56)));

    while (len >= 64) {
        len -= 64;
        h = mix_stream_bulk(h * C, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8),
            GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        h = mix_stream_bulk(ROTL64(h, R1), GET_U64<bswap>(buf, 32), GET_U64<bswap>(buf, 40),
            GET_U64<bswap>(buf, 48), GET_U64<bswap>(buf, 56));
        buf += 64;
    }

    while (len >= 8) {
        len -= 8;
        h = mix_stream(h, GET_U64<bswap>(buf, 0));
        buf += 8;
    }
    
    // This format is behaviorally equivalent to the uncommented one below it.
    // I don't know if having multiple return statements might have a negative performance impact.
    // The code for this commented block is probably slightly larger.
    //const uint8_t* const tail8 = buf;
    //switch (len) {
    //case 0: return mix(h);
    //case 1: return mix(mix_stream(h, tail8[0]));
    //case 2: return mix(mix_stream(h, GET_U16<bswap>(tail8, 0)));
    //case 3: return mix(mix_stream(h, GET_U16<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[2]) << 16));
    //case 4: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0)));
    //case 5: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[4]) << 32));
    //case 6: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32));
    //case 7: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32 | static_cast<uint64_t>(tail8[6]) << 48));
    //default:;
    //}

    //return mix(h);

    const uint8_t* const tail8 = buf;
    switch (len) {
    case 1: h = (mix_stream(h, tail8[0]));                                                                                                                 break;
    case 2: h = (mix_stream(h, GET_U16<bswap>(tail8, 0)));                                                                                                 break;
    case 3: h = (mix_stream(h, GET_U16<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[2]) << 16));                                                         break;
    case 4: h = (mix_stream(h, GET_U32<bswap>(tail8, 0)));                                                                                                 break;
    case 5: h = (mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[4]) << 32));                                                         break;
    case 6: h = (mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32));                                         break;
    case 7: h = (mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32 | static_cast<uint64_t>(tail8[6]) << 48)); break;
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
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0x3DFB9ECD,//0x19498DD8,// 0xB482B1A1,// 0x288113E9,
    $.verification_BE = 0x8D5ADC80,//0x45CC43B9,// 0xEC3B3404,// 0x78278B75,
    $.hashfn_native = ax<false>,
    $.hashfn_bswap = ax<true>
);
