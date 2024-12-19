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

static const uint32_t C32 = UINT32_C(0xB89A8925);

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

static inline uint32_t mix_stream32(uint32_t h, uint32_t x) {
    h = h ^ h >> 17 ^ x;
    h = h * 0xED5AD4BBu;
    h = h ^ h >> 11;
    h = h * 0xAC4C1B51u;
    h = h ^ h >> 15;
    h = h * 0x31848BABu;
    h = h ^ h >> 14 ^ x;
    return h;
}

static inline uint32_t mix_stream_bulk32(uint32_t h, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    constexpr int R2 = 15;
    h += (ROTL32(a, R2) - c) * Q32;
    h += (ROTL32(b, R2) - d) * R32;
    h += (ROTL32(c, R2) - b) * S32;
    h += (ROTL32(d, R2) - a) * T32;
    return h;
}

template <bool bswap>
static inline uint32_t axhash32(const uint8_t* buf, size_t len, uint32_t seed) {
    constexpr int Q1 = 14;
    constexpr int Q2 = 23;
    constexpr int R1 = 17;

    const uint8_t* const tail = buf + (len & ~3);
    uint32_t h = len ^ seed ^ ROTL32(seed, Q1) ^ ROTL32(seed, Q2);

    while (len >= 32) {
        len -= 32;
        h = mix_stream_bulk32(h * C32, GET_U32<bswap>(buf, 0), GET_U32<bswap>(buf, 4),
            GET_U32<bswap>(buf, 8), GET_U32<bswap>(buf, 12));
        h = mix_stream_bulk32(ROTL32(h, R1), GET_U32<bswap>(buf, 16), GET_U32<bswap>(buf, 20),
            GET_U32<bswap>(buf, 24), GET_U32<bswap>(buf, 28));
        buf += 32;
    }

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
    $.verification_LE = 0x3DFB9ECD,//0x19498DD8,// 0xB482B1A1,// 0x288113E9,
    $.verification_BE = 0x8D5ADC80,//0x45CC43B9,// 0xEC3B3404,// 0x78278B75,
    $.hashfn_native = ax<false>,
    $.hashfn_bswap = ax<true>
);

REGISTER_HASH(ax32,
    $.desc = "ax32",
    $.hash_flags =
    FLAG_HASH_SMALL_SEED,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 32,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = ax32<false>,
    $.hashfn_bswap = ax32<true>
);
