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

Results from January 13, 2025 (this version passes SMHasher 2 as well):
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4389  1286   588   306   158    94    24    20    13     4     0     0     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     1     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: ax
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 387.866363 seconds

*/

 //------------------------------------------------------------
// MX3 multiplier
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);
// Random 64-bit probable primes, as given by Java's BigInteger class.
static const uint64_t Q = UINT64_C(0xD1B92B09B92266DD);
static const uint64_t R = UINT64_C(0x9995988B72E0D285);
static const uint64_t S = UINT64_C(0x8FADF5E286E31587);
static const uint64_t T = UINT64_C(0xFCF8B405D3D0783B);

//// Moremur multipliers
//static const uint64_t A = UINT64_C(0x3C79AC492BA7B653);
//static const uint64_t B = UINT64_C(0x1C69B3F74AC4AE35);
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
    constexpr unsigned int Q2 = 28u;
    constexpr unsigned int R2 = 29u;
    constexpr unsigned int S2 = 27u;
    constexpr unsigned int T2 = 25u;
    return h
        + (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + d) * S
        + (ROTL64(d, T2) + a) * T;

    //constexpr int R2 = 29;
    //return h
    //+ (ROTL64(a, R2) - c) * Q
    //+ (ROTL64(b, R2) - d) * R
    //+ (ROTL64(c, R2) - b) * S
    //+ (ROTL64(d, R2) - a) * T;
}

template <bool bswap>
static inline uint64_t axhash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int Q1 = 29;
    constexpr int Q2 = 47;
    constexpr int R1 = 37;

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

    // This returns once at the end, and may emit smaller code...
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

    //const uint8_t* const tail8 = buf;
    //switch (len) {
    //case 1: h = mix_stream_bulk(h, h, tail8[0], Q, R);                                               break;
    //case 2: h = mix_stream_bulk(h, h, GET_U16<bswap>(tail8, 0), Q, R);                               break;
    //case 3: h = mix_stream_bulk(h, h, GET_U16<bswap>(tail8, 0), tail8[2], Q);                        break;
    //case 4: h = mix_stream_bulk(h, h, GET_U32<bswap>(tail8, 0), Q, R);                               break;
    //case 5: h = mix_stream_bulk(h, h, GET_U32<bswap>(tail8, 0), tail8[4], Q);                        break;
    //case 6: h = mix_stream_bulk(h, h, GET_U32<bswap>(tail8, 0), GET_U16<bswap>(tail8, 4), Q);        break;
    //case 7: h = mix_stream_bulk(h, h, GET_U32<bswap>(tail8, 0), GET_U16<bswap>(tail8, 4), tail8[6]); break;
    //default:;
    //}
    //return mix(h);
}

//------------------------------------------------------------
template <bool bswap>
static void ax(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = axhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}


// First try, passing! Even with a 32-bit seed given to a uint64_t parameter.

//----------------------------------------------------------------------------------------------
//-log2(p-value) summary:
//
//          0     1     2     3     4     5     6     7     8     9    10    11    12
//        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
//         1860   451   216    95    57    44    17     5     3     4     0     1     0
//
//         13    14    15    16    17    18    19    20    21    22    23    24    25+
//        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
//            0     0     0     0     0     0     0     0     0     0     0     0     0
//
//----------------------------------------------------------------------------------------------
//Summary for: ax-trunc
//Overall result: pass            ( 188 / 188 passed)
//
//----------------------------------------------------------------------------------------------
//Verification value is 0x00000001 - Testing took 215.391453 seconds
//
//Small key speed test - [1, 31] - byte keys
// 24.43 cycles/hash
//Bulk speed test - 262144-byte keys
// Average       - 12.15 bytes/cycle - 39.61 GiB/sec @ 3.5 gh

// From January 13, 2025:
//----------------------------------------------------------------------------------------------
//-log2(p-value) summary:
//
//          0     1     2     3     4     5     6     7     8     9    10    11    12
//        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
//         1837   467   213    97    62    47    17     4     3     4     1     1     0
//
//         13    14    15    16    17    18    19    20    21    22    23    24    25+
//        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
//            0     0     0     0     0     0     0     0     0     0     0     0     0
//
//----------------------------------------------------------------------------------------------
//Summary for: ax-trunc
//Overall result: pass            ( 188 / 188 passed)
//
//----------------------------------------------------------------------------------------------
//Verification value is 0x00000001 - Testing took 291.145580 seconds

template <bool bswap>
static void ax_trunc(const void* in, const size_t len, const seed_t seed, void* out) {
    uint32_t h = (uint32_t)axhash<bswap>((const uint8_t*)in, len, (uint32_t)seed);

    PUT_U32<bswap>(h, (uint8_t*)out, 0);
}



//*********FAIL*********
//
//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1532   285   174    86    41    23    17     9     7     9     6     7     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//4     7     6     7     1     4     2     3     5     4     4     2   504
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(131 / 188 passed)
//Failures :
//    Avalanche : [64, 128]
//    BIC : [15]
//    Cyclic : [4 cycles of 4 bytes, 4 cycles of 8 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [4 / 5, 9 / 4, 5 / 9, 3 / 32, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [FXXXXB, FooooXXXXBaaar, FooooooooXXXXBaaaaaaar]
//    TwoBytes : [20, 32, 48]
//    PerlinNoise : [2]
//    Bitflip : [4, 8]
//    SeedBlockLen : [22, 24, 26, 28, 30]
//    SeedBlockOffset : [4, 5]
//    Seed : [2, 3, 6, 15, 18, 31, 80]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 308.392236 seconds

// Just changed C32 and mix32 to something stronger...

//*********FAIL*********
//
//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1609   343   182   106    79    40    29    28    27     9    15    19    12
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//10    11     8     9     9     7     9     9     5     4     6     4   164
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(146 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 4 bytes, 4 cycles of 8 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [FooBarXXXX, FooooXXXXBaaar, FooooBaaarXXXX, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooBaaaaaaaaarXXXX]
//    TwoBytes : [20, 32, 48]
//    PerlinNoise : [2]
//    Bitflip : [8]
//    SeedBlockOffset : [1, 2, 4, 5]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 276.792451 seconds

// Small changes to mix_stream32:

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1728   378   190    98    50    41    35    14    14    19    13    11     7
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//11     5     5     6     4     1     3     4     3     4     5     3   101
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(161 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//    PerlinNoise : [2]
//    Bitflip : [8]
//    SeedBitflip : [3, 4]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 281.252644 seconds

// Maybe a bit different. Stronger mix_stream32().

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1714   385   194    97    59    36    25    22    18     8     8     5     9
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//4     7     7     6     3     5     3     5     4     5     4     2   118
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(159 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//    PerlinNoise : [2]
//    Bitflip : [8]
//    SeedBitflip : [3, 4]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 283.386293 seconds

// Rotating h when calling mix_stream32 n smaller keys...

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1721   383   178   113    46    37    20    24    19    12     8     7     8
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//5     8     9     4     6     6     3     4     6     2     4     3   117
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(159 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//    PerlinNoise : [2]
//    Bitflip : [8]
//    SeedBitflip : [3, 4]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 289.461743 seconds

// strengthen mix_stream32() instead.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1692   388   189   108    74    41    21    21     9    12    13     9    10
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//4     3    11     6     6     3     4     5     5     2     3     4   110
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(163 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//    PerlinNoise : [2]
//    SeedBitflip : [3, 4]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 322.173221 seconds

// Trying just using mix_stream_bulk32(), no mix_stream32(). Faster for small keys, but fails more tests.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1324   287   181   109    60    38    33    27    28    23    26    12    14
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//5    11    18    10     8     8     4     9    13    10     9     1   485
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(105 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [5 / 9, 4 / 14, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TextNum : [with commas]
//    Text : [FXXXXB, XXXXFooBar, FooooXXXXBaaar, FooooooXXXXBaaaaar, FooooooooXXXXBaaaaaaar, FooooooooooXXXXBaaaaaaaaar, XXXXFooooooooooBaaaaaaaaar]
//    TwoBytes : [20, 32, 48]
//    Bitflip : [4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedSparse : [15, 18, 31, 52, 80, 200]
//    SeedBlockLen : [9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//    Seed : [6, 31, 52, 80, 200]
//    SeedBitflip : [3, 4]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 306.982172 seconds

// Using constants instead of h, and multiplying the one usage of h by a constant:

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1357   280   158    97    69    45    29    24    22    27    12    12    14
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//20     8    10    11    13    10     8     8     7     5     6     3   498
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(128 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 4 bytes, 4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 14, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [20, 32, 48]
//    Bitflip : [4, 8]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 285.678721 seconds

// Slower due to how mix_stream_bulk32() works.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1466   241   146   109    53    45    34    21    27    14    17    14     5
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//14    18    13    12     9     5     5     7     0     4     5     5   464
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(125 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 4 bytes, 4 cycles of 8 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 3 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 12, 3 / 14, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TextNum : [without commas]
//    TwoBytes : [20, 32, 48]
//    Bitflip : [8]
//    SeedBlockLen : [12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [3, 5]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 318.657668 seconds

// Much better! Back to mix_stream32(), now based on mix32(), so slower on small hashes.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1793   452   202    91    46    35    17    10     3     4     3     2     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     0     1     0     0     1     3     1     0     1     1     0    81
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(165 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 341.999265 seconds

// This change did NOT work. Much slower, and more failures...

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1666   413   171    93    58    22    14    11     2     6     1     1     0
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     0     0     1     0     3     0     0     0     0     0     0   289
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(143 / 188 passed)
//Failures :
//    Zeroes : []
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [32, 48, 1024, 2048, 4096]
//    SeedZeroes : [1280, 8448]
//    SeedSparse : [80, 200, 1025]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//    Seed : [80, 200, 1025]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 488.574588 second

// Slower than the one two up because of an extra multiplication, but more failures.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1766   434   183    97    55    20    15    12     1     7     1     1     0
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     2     0     0     1     2     0     0     0     0     1     1   152
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(149 / 188 passed)
//Failures :
//    Cyclic : [8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [32, 48, 1024, 2048, 4096]
//    SeedZeroes : [1280, 8448]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 344.478652 seconds

// The only difference here from the more-successful variant that passes 165 tests, is different rotation amounts in mix_stream_bulk32().

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1759   437   207    90    48    33    18     7     6     6     4     3     5
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     1     0     1     1     0     3     2     1     0     0     1   118
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(155 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [32, 48, 1024, 2048, 4096]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 297.215667 seconds

// Here, mix_stream_bulk32() uses a rotation of 19 for everything.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1795   453   213    91    47    34    18    13     4     5     5     2     6
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     0     1     0     0     1     1     1     3     0     0     0    58
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(168 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 295.889347 seconds

// Tried changing Q32 through T32, but the result is slightly worse.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1803   454   206    88    47    32    20     8     3     5     5     1     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//3     0     0     0     1     0     1     2     1     0     0     0    69
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(166 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 298.799838 seconds

// Rotation by 11 instead of 19 in bulk...

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1777   462   210    88    42    33    17     8     3     5     3     1     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//3     0     0     0     1     0     3     4     4     1     0     0    84
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(163 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48, 1024, 2048, 4096]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 403.065465 seconds

// Rotation by 21 is worse than 19 or 11...

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1782   451   210    95    43    31    19     9     3     4     3     1     5
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     0     0     0     1     0     2     1     0     0     0     0    91
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(160 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [32, 48, 1024, 2048, 4096]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 297.202872 seconds

// 16 is worse; pretty much the expected outcome.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1779   445   201    88    44    33    18     7     5     4     5     1     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//3     0     0     0     0     0     2     1     3     1     2     2   105
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(156 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1280]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 297.611117 seconds

// Using no bulk method at all, this still doesn't pass, failing some permutation tests.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1857   451   222    89    47    33    21     8     3     4     3     2     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     0     0     0     0     0     1     1     0     0     0     0    27
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(176 / 186 passed)
//Failures :
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 641.135729 seconds

// Another uint32_t at a time version, very slow, still fails Permutation...

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1865   426   198   107    63    28    27     9     9     4     1     4     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     0     0     0     0     1     0     0     0     0     0     0    27
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(176 / 186 passed)
//Failures :
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 696.412430 seconds

// Incorporating length into mix_stream32() doesn't help at all...

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1854   428   221    93    72    23    22    18     5     4     1     2     1
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1     2     0     0     0     0     0     0     0     0     0     1    27
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(176 / 186 passed)
//Failures :
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 718.388838 seconds

// This first try with 4 extra state variables did not go very well.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1695   427   207    99    55    28    17     3    14    12     3     1     2
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//0     0     2     0     0     0     0     1     1     0     0     0   208
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(154 / 186 passed)
//Failures :
//    Sanity : [Basic 2]
//    Avalanche : [64, 128]
//    Cyclic : [16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [1024, 2048, 4096]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 177.348465 seconds

// Using two of the abcd variables per qrst variable, it isn't much different.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1697   436   219   100    55    29    19     5    12    16     5     2     3
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1     1     3     0     1     0     0     2     2     2     0     1   164
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(154 / 186 passed)
//Failures :
//    Sanity : [Basic 2]
//    Avalanche : [64, 128]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [1024, 2048, 4096]
//    Seed : [52]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 202.703342 seconds

// Using bulk, but incorporating one h value into each state portion -- this is quite fast, but fails a bunch.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1715   451   206   105    55    26    20    11     7     4     5     1     2
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//6     3     0     1     1     3     0     0     2     1     1     1   148
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(147 / 186 passed)
//Failures :
//    Zeroes : []
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum first 4016 - 4176, Long alnum first 8112 - 8272]
//    TwoBytes : [1024, 2048, 4096]
//    SeedZeroes : [1280, 8448]
//    SeedSparse : [52, 80, 200, 1025]
//    Seed : [80, 200, 1025]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 171.496551 seconds

// About as good as it has ever been with a "real" bulk method, OK! And it's also fast.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1873   412   198   116    68    30    20     6     3     3     2     1     0
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//0     0     2     0     0     1     0     0     2     2     0     0    36
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(172 / 186 passed)
//Failures :
//    Sparse : [3 / 32, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 164.156993 seconds

// Just changing `* C32` to `* C32 + T32` makes more tests and types of test fail, for some reason...

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1840   399   201   114    66    31    19     6     3     2     3     2     0
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//0     0     1     0     2     0     0     1     1     2     0     0    60
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(168 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 340.820970 seconds

// Back to `* C32`, but now bulk ends in a right xorshift by 15.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1847   406   193   119    61    31    15     7     3     3     4     2     0
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//3     0     1     0     0     0     0     0     1     1     0     0    56
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(170 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//
//    Permutation : [4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 321.634444 seconds

// Tried changing mix_stream32() to use mix_stream_bulk32() -- disastrous, it seems.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1202   236   140   113    63    50    34    19    24    18    14    18    22
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//11    10     9     9    12     7     4     5     4     1     2     5   721
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(98 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 3 bytes, 4 cycles of 4 bytes, 4 cycles of 5 bytes, 4 cycles of 8 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [dictionary]
//    TextNum : [with commas]
//    Text : [XXXXFB, FooXXXXBar, XXXXFooBar, FooooXXXXBaaar, XXXXFooooBaaar, FooooooXXXXBaaaaar, XXXXFooooooBaaaaar, FooooooooXXXXBaaaaaaar, XXXXFooooooooBaaaaaaar, FooooooooooXXXXBaaaaaaaaar, XXXXFooooooooooBaaaaaaaaar, Words alnum 5 - 8, Words alnum 1 - 16, Words alnum 1 - 32, Long alnum last 1968 - 2128, Long alnum last 4016 - 4176, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32, 48]
//    Bitflip : [4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 4, 5]
//    SeedBitflip : [4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 328.448342 seconds

// Just like the nice 172/188 one, but with one extra XRR... and it's worse.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1832   412   202   116    65    30    17     6     5     1     2     1     0
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//0     0     1     0     0     2     0     0     3     1     0     0    57
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(168 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32]
//    Permutation : [4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 321.043115 seconds

// Changing qrst vars doesn't actually... help...

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1834   397   198   112    64    28    17     3     3     1     3     1     2
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//2     0     1     0     1     0     0     0     1     2     0     0    83
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(165 / 188 passed)
//Failures :
//    Cyclic : [16 cycles of 4 bytes]
//    Sparse : [3 / 32, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum first 4016 - 4176, Long alnum first 8112 - 8272]
//    TwoBytes : [32, 48, 1024, 2048, 4096]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 312.285322 seconds

// Using powers of QRST32 that get greater and greater:

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1821   400   201   122    68    30    19     4     8     2     4     2     2
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1     1     1     3     1     0     2     0     1     1     0     0    59
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(170 / 188 passed)
//Failures :
//    Cyclic : [16 cycles of 4 bytes]
//    Sparse : [3 / 32, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48, 4096]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 392.000891 seconds

// Try switching mix_stream32 to one like the 64-bit ax, but otherwise the same as the 172/188 version. Nope.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1554   320   173   108    70    34    35    19    29    24    17    14    13
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//12    11     9     2     7     7     2    10     8     7     6     6   278
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(122 / 186 passed)
//Failures :
//    Sparse : [3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TextNum : [with commas]
//    Text : [Long alnum last 1968 - 2128, Long alnum last 4016 - 4176, Long alnum last 8112 - 8272]
//    TwoBytes : [8]
//    PerlinNoise : [2]
//    Bitflip : [8]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 149.621238 seconds

// A HA! The HASH_IMPL_VERY_SLOW flag reduces the number of trials performed, which made the 172/188 one actually be 167/188 !

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1850   397   192   115    66    30    17     7     3     3     3     1     1
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//0     0     2     0     0     1     0     0     3     2     0     0    60
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(167 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 32, 3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 306.843808 seconds

// Ugh, tried incorporating 4 different rotations of h in bulk, instead of just h at the end... There's also an added constant in stream.

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1745   396   221   121    47    33    18     8     9     3     3     2     2
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1     0     1     1     1     0     1     0     2     0     1     1   136
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(147 / 188 passed)
//Failures :
//    Zeroes : []
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [Long alnum first 1968 - 2128, Long alnum first 4016 - 4176, Long alnum first 8112 - 8272]
//    TwoBytes : [32, 48, 1024, 2048, 4096]
//    SeedZeroes : [1280, 8448]
//    SeedSparse : [52, 80, 200, 1025]
//    Seed : [80, 200, 1025]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 575.397853 seconds

// Maybe adding a constant in the bulk step helps a little?

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1789   465   200   104    48    31    27    10     2     4     3     1     3
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//0     0     3     0     0     0     0     0     1     1     1     0    60
//
//----------------------------------------------------------------------------------------------
//Summary for: ax32
//Overall result : FAIL(168 / 188 passed)
//Failures :
//    Cyclic : [4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 8 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 64, 3 / 96]
//    Permutation : [4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TwoBytes : [32, 48]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 300.700213 seconds



static const uint32_t C32 = UINT32_C(0xB89A8925);

// truncated 64-bit, low 32 bits
//static const uint32_t Q32 = UINT32_C(0xB92266DD);
//static const uint32_t R32 = UINT32_C(0x72E0D285);
//static const uint32_t S32 = UINT32_C(0x86E31587);
//static const uint32_t T32 = UINT32_C(0xD3D0783B);

static const uint32_t Q32 = UINT32_C(0x89A4EF89);
static const uint32_t R32 = UINT32_C(0x9196714F);
static const uint32_t S32 = UINT32_C(0xD72D0CC9);
static const uint32_t T32 = UINT32_C(0x9B05C645);

static inline uint32_t mix32(uint32_t h) {
    h = h ^ h >> 17;
    h = h * 0xED5AD4BBu;
    h = h ^ h >> 11;
    h = h * 0xAC4C1B51u;
    h = h ^ h >> 15;
    h = h * 0x31848BABu;
    h = h ^ h >> 14;
    return h;
}

//static inline uint32_t mix_stream32(uint32_t h, uint32_t x) {
//    constexpr uint32_t R1 = 7;
//    x *= C32;
//    x ^= (x >> R1);
//    h += x * C32;
//    h *= C32;
//    return h;
//}

static inline uint32_t mix_stream32(uint32_t h, uint32_t x) {
    h = h ^ h >> 17;
    h = h * 0xED5AD4BBu + x;
    h = h ^ h >> 11;
    h = h * 0xAC4C1B51u;
    h = h ^ h >> 15;
    h = h * 0x31848BABu - x;
    h = h ^ h >> 14;
    return h;
}

//, uint32_t* q, uint32_t* r, uint32_t* s, uint32_t* t
static inline uint32_t mix_stream_bulk32(uint32_t h, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    constexpr int R2 = 19;
    return (ROTL32(a, R2) - c) * Q32
        + (ROTL32(b, R2) - d) * R32
        + (ROTL32(c, R2) - b) * S32
        + (ROTL32(d, R2) - a) * T32
        + h + C32;

    //h += *q += (ROTL32(a, R2) - c) * Q32;
    //h += *r += (ROTL32(b, R2) - d) * R32;
    //h += *s += (ROTL32(c, R2) - b) * S32;
    //h += *t += (ROTL32(d, R2) - a) * T32;
    //return h;
}

template <bool bswap>
static inline uint32_t axhash32(const uint8_t* buf, size_t len, uint32_t seed) {
    constexpr int Q1 = 14;
    constexpr int Q2 = 23;
    constexpr int R1 = 17;

    uint32_t h = len ^ seed ^ ROTL32(seed, Q1) ^ ROTL32(seed, Q2);
    //    uint32_t q = ROTL32(h, 7), r = ROTL32(h, 13), s = ROTL32(h, 20), t = ROTL32(h, 26);

    while (len >= 32) {
        len -= 32;
        h = mix_stream_bulk32(h * C32, GET_U32<bswap>(buf, 0), GET_U32<bswap>(buf, 4),
            GET_U32<bswap>(buf, 8), GET_U32<bswap>(buf, 12));
        h = mix_stream_bulk32(ROTL32(h, R1), GET_U32<bswap>(buf, 16), GET_U32<bswap>(buf, 20),
            GET_U32<bswap>(buf, 24), GET_U32<bswap>(buf, 28));
        buf += 32;
    }
    //    h ^= q ^ r ^ s ^ t;

    while (len >= 4) {
        len -= 4;
        h = mix_stream32(h, GET_U32<bswap>(buf, 0));
        buf += 4;
    }

    const uint8_t* const tail8 = buf;
    switch (len) {
    case 1: h = (mix_stream32(h, tail8[0]));                                                                                                                 break;
    case 2: h = (mix_stream32(h, GET_U16<bswap>(tail8, 0)));                                                                                                 break;
    case 3: h = (mix_stream32(h, GET_U16<bswap>(tail8, 0) | static_cast<uint32_t>(tail8[2]) << 16));                                                         break;
    default:;
    }

    return mix32(h);
}

//------------------------------------------------------------
template <bool bswap>
static void ax32(const void* in, const size_t len, const seed_t seed, void* out) {
    uint32_t h = axhash32<bswap>((const uint8_t*)in, len, (uint32_t)seed);

    PUT_U32<bswap>(h, (uint8_t*)out, 0);
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
    $.verification_LE = 0x16BAEA10,//0x3DFB9ECD,//0x19498DD8,// 0xB482B1A1,// 0x288113E9,
    $.verification_BE = 0x0EF27385,//0x8D5ADC80,//0x45CC43B9,// 0xEC3B3404,// 0x78278B75,
    $.hashfn_native = ax<false>,
    $.hashfn_bswap = ax<true>
);

REGISTER_HASH(ax_trunc,
    $.desc = "ax truncated",
    $.hash_flags =
    FLAG_HASH_SMALL_SEED,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 32,
    $.verification_LE = 0x37461AF5,
    $.verification_BE = 0x3D7023B7,
    $.hashfn_native = ax_trunc<false>,
    $.hashfn_bswap = ax_trunc<true>
);

REGISTER_HASH(ax32,
    $.desc = "ax32",
    $.hash_flags =
    FLAG_HASH_SMALL_SEED,
    $.impl_flags =
    //FLAG_IMPL_VERY_SLOW |
    FLAG_IMPL_MULTIPLY |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 32,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = ax32<false>,
    $.hashfn_bswap = ax32<true>
);