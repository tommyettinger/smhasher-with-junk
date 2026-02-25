/*
 * jvmstring hash
 * This hash follows the algorithm described in the documentation
 * for java.lang.String.hashCode() in any recent version of the JVM.
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include "Platform.h"
#include "Hashlib.h"

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
          298    81    47    30    16    10     7    10     3     4     0     0     4

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0  2265

----------------------------------------------------------------------------------------------
Summary for: jvmstring
Overall result: FAIL            ( 6 / 186 passed)
Failures:
    Sanity              : [Prepend zeroes]
    Avalanche           : [3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
    BIC                 : [3, 8, 11, 15]
    Zeroes              : []
    Cyclic              : [4 cycles of 3 bytes, 4 cycles of 4 bytes, 4 cycles of 5 bytes, 4 cycles of 8 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 5 bytes, 8 cycles of 8 bytes, 12 cycles of 3 bytes, 12 cycles of 4 bytes, 12 cycles of 5 bytes, 12 cycles of 8 bytes, 16 cycles of 3 bytes, 16 cycles of 4 bytes, 16 cycles of 5 bytes, 16 cycles of 8 bytes]
    Sparse              : [6/2, 4/3, 4/4, 4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [dictionary]
    TextNum             : [without commas, with commas]
    Text                : [FXXXXB, FBXXXX, XXXXFB, FooXXXXBar, FooBarXXXX, XXXXFooBar, FooooXXXXBaaar, FooooBaaarXXXX, XXXXFooooBaaar, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, XXXXFooooooBaaaaar, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, XXXXFooooooooBaaaaaaar, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, XXXXFooooooooooBaaaaaaaaar, Words alnum 1-4, Words alnum 5-8, Words alnum 1-16, Words alnum 1-32, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [8, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]
    Seed                : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
    SeedAvalanche       : [4, 8, 16, 24, 32, 64, 128]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 432.538959 seconds

// Using ROTL32(h, 25):
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
          843   193    99    57    23    11    16     4     4     5     2     3     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     1     0     0     0     0     1     2     0     0     2     1  1507

----------------------------------------------------------------------------------------------
Summary for: jvmstring
Overall result: FAIL            ( 64 / 186 passed)
Failures:
    Avalanche           : [3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
    BIC                 : [3, 8, 11, 15]
    Sparse              : [6/2, 4/3, 4/4, 4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [dictionary]
    TextNum             : [without commas, with commas]
    Text                : [FXXXXB, FBXXXX, XXXXFB, FooXXXXBar, FooBarXXXX, XXXXFooBar, FooooXXXXBaaar, FooooBaaarXXXX, XXXXFooooBaaar, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooBaaaaaaaaarXXXX, Words alnum 1-4, Words alnum 5-8, Words alnum 1-16, Words alnum 1-32, Long alnum last 1968-2128, Long alnum last 4016-4176, Long alnum last 8112-8272]
    TwoBytes            : [8, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6, 15, 18]
    SeedBlockLen        : [8, 12, 16, 20, 24, 28]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]
    Seed                : [2, 3, 6, 15, 18]
    SeedAvalanche       : [4, 8]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 589.491525 seconds

// Using 0x9E3779B9u * ROTL(h, 7) and running through TheIronBorn's mixer:
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1645   343   194    93    48    44    32    14    15     8    11     8    12

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            6     0     5     2     9     2     2     3     4     3     4     3   265

----------------------------------------------------------------------------------------------
Summary for: jvmstring
Overall result: FAIL            ( 153 / 186 passed)
Failures:
    BIC                 : [3, 8, 11, 15]
    Sparse              : [10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/96]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    TextNum             : [with commas]
    TwoBytes            : [8]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedBIC             : [3]
    SeedBitflip         : [3]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 566.933656 seconds

*/

//------------------------------------------------------------
//static uint32_t jvmstring_impl( const uint8_t * data, size_t len, uint32_t h ) {
//    for (size_t i = 0; i < len; ++i) {
//        h = 0x9E3779B9u * ROTL32(h, 7) + data[i];
//    }
//
//    //[16 21f0aaad 15 735a2d97 15]
//    h ^= h >> 16;
//    h *= 0x21F0AAADu;
//    h ^= h >> 15;
//    h *= 0x735A2D97u;
//    h ^= h >> 15;
//    return h;
//}



//********* FAIL*********
//
//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1301   281   146    75    33    15    10    11     7     3     2     0     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1     1     1     2     1     3     3     2     0     0     0     1   844
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(105 / 187 passed)
//Failures :
//    Avalanche : [3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
//    BIC : [3, 8, 11, 15]
//    Sparse : [6 / 2, 4 / 3, 4 / 4, 4 / 5, 3 / 6, 3 / 7, 3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 10 / 2, 20 / 3, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TextNum : [without commas, with commas]
//    Text : [FBXXXX, XXXXFB, FooBarXXXX, FooooBaaarXXXX, FooooooBaaaaarXXXX, FooooooooBaaaaaaarXXXX, FooooooooooBaaaaaaaaarXXXX, Long alnum last 1968 - 2128, Long alnum last 4016 - 4176, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32]
//    PerlinNoise : [2]
//    Bitflip : [3, 4, 8]
//    SeedZeroes : [1280]
//    SeedSparse : [2, 3]
//    Seed : [2, 3]
//    SeedBitflip : [3, 4]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 1221.716216 seconds
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    uint32_t mul = h | 1u;
//    for (size_t i = 0; i < len; ++i) {
//        h = (ROTL32(h, 25) + data[i]) * (mul += 0x9E3779BAu);
//    }
//    h ^= ROTL32(h, 23) ^ ROTL32(h, 11);
//    h ^= ROTL32(h, 21) ^ ROTL32(h, 9);
//    return h;
//}

//********* FAIL*********
//
//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1021   190   109    55    41    21    25     8    10     5     7     3     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//3     3     3     2     5     2     1     2     7     1     0     1  1218
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(54 / 187 passed)
//Failures :
//    Avalanche : [4, 5, 6, 7, 8, 9, 12, 16, 20, 64, 128]
//    BIC : [3, 8, 11, 15]
//    Cyclic : [4 cycles of 4 bytes, 4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 5 bytes, 8 cycles of 8 bytes, 12 cycles of 4 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 5 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [FXXXXB, FooooooooXXXXBaaaaaaar, XXXXFooooooooBaaaaaaar, Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32, 1024, 2048, 4096]
//    PerlinNoise : [2]
//    Bitflip : [3, 4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedSparse : [2, 15, 18, 31, 80]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//    Seed : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
//    SeedAvalanche : [4, 16, 24, 32, 64, 128]
//    SeedBIC : [3, 8, 11, 15]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 302.630791 seconds
//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 3;
//    for (; i < len; i += 4, data += 4) {
//        h = (h + GET_U32<bswap>(data, 0)) * 0x9E3779B9u;
//    }
//    i -= 3;
//    for (; i < len; i++, data++) {
//        h = (h + data[0]) * 0x9E3779B9u;// (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 16;
//    h *= 0x21F0AAADu;
//    h ^= h >> 15;
//    h *= 0x735A2D97u;
//    h ^= h >> 15;
//    return h;
//}

//********* FAIL*********
//
//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1420   291   141    87    47    28    16    13    11     2     5     5     3
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//0     3     5     2     1     1     2     2     3     2     2     2   653
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(103 / 187 passed)
//Failures :
//    Avalanche : [4, 8, 12, 16, 20, 64, 128]
//    BIC : [3, 8, 11, 15]
//    Sparse : [3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TextNum : [with commas]
//    Text : [FBXXXX]
//    TwoBytes : [20, 32]
//    PerlinNoise : [2]
//    Bitflip : [3, 4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//    SeedBitflip : [4]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 398.427469 seconds
//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 3;
//    for (; i < len; i += 4, data += 4) {
//        h = (h ^ h >> 16 ^ GET_U32<bswap>(data, 0)) * 0x9E3779B9u;
//    }
//    i -= 3;
//    for (; i < len; i++, data++) {
//        h = (h ^ h >> 16 ^ data[0]) * 0x9E3779B9u;// (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 15;
//    h *= 0xD168AAADu;
//    h ^= h >> 15;
//    h *= 0xAF723597u;
//    h ^= h >> 15;
//    return h;
//}


//********* FAIL*********
//
//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1227   213   133    54    36    23    14    10     5     5     5     2     5
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//3     2     1     2     3     1     1     1     0     0     1     2   998
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(58 / 187 passed)
//Failures :
//    Avalanche : [4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
//    BIC : [3, 8, 11, 15]
//    Zeroes : []
//    Cyclic : [4 cycles of 4 bytes, 4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 5 bytes, 8 cycles of 8 bytes, 12 cycles of 4 bytes, 12 cycles of 8 bytes, 16 cycles of 4 bytes, 16 cycles of 5 bytes, 16 cycles of 8 bytes]
//    Sparse : [4 / 5, 3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [XXXXFooooooooBaaaaaaar, Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32, 1024, 2048, 4096]
//    Bitflip : [3, 4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//    Seed : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
//    SeedAvalanche : [4, 8, 16, 24, 32, 64, 128]
//    SeedBIC : [3, 8, 11, 15]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 278.199765 second

//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 3;
//    uint32_t mul = ((len ^ h) * 0x86E31586u + 0xD3D0783Bu);
//    for (; i < len; i += 4, data += 4) {
//        h = (h + GET_U32<bswap>(data, 0)) * (mul += 0x9E3779BAu);;
//    }
//    i -= 3;
//    for (; i < len; i++, data++) {
//        h = (h + data[0]) * (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 15;
//    h *= 0xD168AAADu;
//    h ^= h >> 15;
//    h *= 0xAF723597u;
//    h ^= h >> 15;
//    return h;
//}

//********* FAIL*********
//
//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1152   243   144    82    54    31    18     7     6     9     8    12     6
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//7     4     5     5     5     3     2     0     3     6     4     2   929
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(48 / 187 passed)
//Failures :
//    Avalanche : [4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
//    BIC : [3, 8, 11, 15]
//    Zeroes : []
//    Cyclic : [4 cycles of 4 bytes, 4 cycles of 8 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 5 bytes, 8 cycles of 8 bytes, 12 cycles of 4 bytes, 12 cycles of 8 bytes, 16 cycles of 3 bytes, 16 cycles of 4 bytes, 16 cycles of 5 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 20 / 3, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TextNum : [with commas]
//    Text : [FBXXXX, FooooBaaarXXXX, XXXXFooooooBaaaaar, FooooooooXXXXBaaaaaaar, Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32, 1024, 2048, 4096]
//    Bitflip : [3, 4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedSparse : [52, 1025]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//    Seed : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
//    SeedAvalanche : [4, 8, 16, 24, 32, 64, 128]
//    SeedBIC : [3, 8, 11, 15]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 184.644482 second

//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 3;
//    uint32_t mul = ((len + h) * 0x86E31586u + 0xD3D0783Bu);
//    for (; i < len; i += 4, data += 4) {
//        h ^= GET_U32<bswap>(data, 0) * (mul += 0x9E3779BAu);
//    }
//    i -= 3;
//    for (; i < len; i++, data++) {
//        h ^= data[0] * (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 15;
//    h *= 0xD168AAADu;
//    h ^= h >> 15;
//    h *= 0xAF723597u;
//    h ^= h >> 15;
//    return h;
//}

//********* FAIL*********
//
//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1262   305   158   101    49    30    29    16    11    14     8    11     8
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//5     4     5     4     4     4     3     0     2     7     5     4   698
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(59 / 187 passed)
//Failures :
//    Avalanche : [16, 20, 64, 128]
//    BIC : [3, 8, 11, 15]
//    Zeroes : []
//    Cyclic : [4 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 4 bytes, 16 cycles of 4 bytes, 16 cycles of 8 bytes]
//    Sparse : [3 / 8, 3 / 10, 3 / 12, 3 / 14, 20 / 3, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    TextNum : [without commas]
//    Text : [XXXXFB, FooXXXXBar, XXXXFooBar, FooooBaaarXXXX, XXXXFooooBaaar, XXXXFooooooBaaaaar, XXXXFooooooooBaaaaaaar, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, XXXXFooooooooooBaaaaaaaaar, Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32, 1024, 2048, 4096]
//    Bitflip : [3, 4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedSparse : [6, 52, 80]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//    Seed : [2, 3, 6, 15, 18, 31, 52, 200, 1025]
//    SeedAvalanche : [4, 8, 16, 24, 64, 128]
//    SeedBIC : [3, 8, 11, 15]
//    SeedBitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 179.673414 seconds

//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 15;
//    uint32_t mul = ((len + h) * 0x86E31586u + 0xD3D0783Bu);
//    for (; i < len; i += 16, data += 16) {
//        h ^=
//            GET_U32<bswap>(data, 0) * (mul + 0xD1B92B0Au) +
//            GET_U32<bswap>(data, 4) * (mul + 0xFCF8B406u) +
//            GET_U32<bswap>(data, 8) * (mul + 0x8FADF5E2u) +
//            GET_U32<bswap>(data, 12) * (mul += 0x9E3779BAu);
//        h ^= h >> 15;
//    }
//    i -= 15;
//    for (; i < len; i++, data++) {
//        h ^= data[0] * (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 15;
//    h *= 0xD168AAADu;
//    h ^= h >> 15;
//    h *= 0xAF723597u;
//    h ^= h >> 15;
//    return h;
//}

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1400   343   195    87    60    23    26    16     8    12     4    10    11
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//4     4     6     3     2     2     5     8     3     5     1     4   505
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(94 / 187 passed)
//Failures :
//    BIC : [3, 8, 11, 15]
//    Cyclic : [4 cycles of 3 bytes, 8 cycles of 4 bytes, 12 cycles of 4 bytes, 16 cycles of 4 bytes]
//    Sparse : [3 / 6, 3 / 7, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 20 / 3, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [FXXXXB, FBXXXX, XXXXFB, FooBarXXXX, FooooXXXXBaaar, Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32, 1024, 2048, 4096]
//    Bitflip : [3, 4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 191.481190 seconds

//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 15;
//    h ^= ROTL32(h, 10) ^ ROTL32(h, 23);
//    uint32_t mul = ((len + h) * 0x86E31586u + 0xD3D0783Bu);
//    for (; i < len; i += 16, data += 16) {
//        h +=
//            GET_U32<bswap>(data, 0) * (mul + 0xD1B92B0Au) +
//            GET_U32<bswap>(data, 4) * (mul + 0xFCF8B406u) +
//            GET_U32<bswap>(data, 8) * (mul + 0x8FADF5E2u) +
//            GET_U32<bswap>(data, 12) * (mul += 0x9E3779BAu);
//        h ^= ROTL32(h, 10) ^ ROTL32(h, 23);
//    }
//    i -= 15;
//    for (; i < len; i++, data++) {
//        h ^= data[0] * (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 15;
//    h *= 0xD168AAADu;
//    h ^= h >> 15;
//    h *= 0xAF723597u;
//    h ^= h >> 15;
//    return h;
//}

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//756   196    70    40    24    19     6     5     0     0     4     0     0
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//0     0     0     0     0     0     0     0     1     0     0     0  1626
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(72 / 187 passed)
//Failures :
//    Avalanche : [16, 20, 64, 128]
//    BIC : [3, 8, 11, 15]
//    Zeroes : []
//    Cyclic : [4 cycles of 3 bytes, 8 cycles of 4 bytes, 12 cycles of 4 bytes, 16 cycles of 4 bytes]
//    Sparse : [4 / 3, 4 / 4, 4 / 5, 3 / 6, 3 / 7, 3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 20 / 3, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [dictionary]
//    TextNum : [without commas, with commas]
//    Text : [FXXXXB, FBXXXX, XXXXFB, FooXXXXBar, FooBarXXXX, XXXXFooBar, FooooXXXXBaaar, FooooBaaarXXXX, XXXXFooooBaaar, FooooooooBaaaaaaarXXXX, FooooooooooBaaaaaaaaarXXXX, Words alnum 1 - 4, Words alnum 5 - 8, Words alnum 1 - 16, Words alnum 1 - 32, Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32, 1024, 2048, 4096]
//    Bitflip : [3, 4, 8]
//    SeedZeroes : [1280, 8448]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 198.491466 second

//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 15;
//    h ^= ROTL32(h, 10) ^ ROTL32(h, 23);
//    uint32_t mul = ((len + h) * 0x86E31586u + 0xD3D0783Bu);
//    for (; i < len; i += 16, data += 16) {
//        h +=
//            GET_U32<bswap>(data, 0) * (mul + 0xD1B92B0Au) +
//            GET_U32<bswap>(data, 4) * (mul + 0xFCF8B406u) +
//            GET_U32<bswap>(data, 8) * (mul + 0x8FADF5E2u) +
//            GET_U32<bswap>(data, 12) * (mul += 0x9E3779BAu);
//        h ^= h >> 15;
//    }
//    i -= 15;
//    for (; i < len; i++, data++) {
//        h += data[0] * (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 15;
//    h *= 0xD168AAADu;
//    h ^= h >> 15;
//    h *= 0xAF723597u;
//    h ^= h >> 15;
//    return h;
//}

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1789   392   202    75    50    28     9     9    11     5     0     2     1
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//3     0     1     0     0     0     2     2     3     0     1     1   161
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(156 / 187 passed)
//Failures :
//    BIC : [3, 8, 11, 15]
//    Sparse : [20 / 3, 9 / 4, 5 / 9, 3 / 48]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [XXXXFooooBaaar, FooooooXXXXBaaaaar, XXXXFooooooooBaaaaaaar, FooooooooooXXXXBaaaaaaaaar]
//    TwoBytes : [20, 32]
//    Bitflip : [3, 4, 8]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 1068.895088 seconds

//template <bool bswap>
//static uint32_t jvmstring_impl( const uint8_t * data, size_t len, uint32_t h ) {
//    h ^= ROTL32(h, 11) ^ ROTL32(h, 20);
//    uint32_t mul = ((len + h) * 0x86E31586u + 0xD3D0783Bu);
//
//    for (size_t i = 0; i < len; ++i) {
//        h = (h ^ ROTL32(h, 10) ^ ROTL32(h, 23) ^ data[i]) * (mul += 0x9E3779BAu);
//    }
//
//    //[16 21f0aaad 15 735a2d97 15]
//    h ^= h >> 16;
//    h *= 0x21F0AAADu;
//    h ^= h >> 15;
//    h *= 0x735A2D97u;
//    h ^= h >> 15;
//    return h;
//}

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1496   357   157    82    52    34    18     7     7     3     3     3     4
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1     1     3     1     6     0     4     2     1     3     1     2   499
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(113 / 187 passed)
//Failures :
//    Avalanche : [4, 8, 12, 16, 20, 64, 128]
//    BIC : [3, 8, 11, 15]
//    Sparse : [4 / 4, 3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 9 / 4, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [FBXXXX]
//    TwoBytes : [20, 32]
//    Bitflip : [3, 4, 8]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 372.901064 seconds

//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 3;
//    h ^= ROTL32(h, 10) ^ ROTL32(h, 23);
//    uint32_t mul = ((len + h) * 0x86E31586u + 0xD3D0783Bu);
//    for (; i < len; i += 4, data += 4) {
//        h = (h ^ h >> 15 ^ GET_U32<bswap>(data, 0)) * (mul += 0x9E3779BAu);
//    }
//    i -= 3;
//    for (; i < len; i++, data++) {
//        h = (h ^ h >> 15 ^ data[0]) * (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 15;
//    h *= 0xD168AAADu;
//    h ^= h >> 15;
//    h *= 0xAF723597u;
//    h ^= h >> 15;
//    return h;
//}

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1449   324   145    79    39    34    17    12    13     8     2     5     6
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//5     3     6     4     2     3     4     2     2     1     3     3   576
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(100 / 187 passed)
//Failures :
//    Sparse : [4 / 5, 3 / 6, 3 / 7, 3 / 8, 3 / 9, 3 / 10, 3 / 12, 3 / 14, 5 / 9, 4 / 14, 4 / 16, 3 / 32, 3 / 48, 3 / 64, 3 / 96, 2 / 128, 2 / 256, 2 / 512, 2 / 1024, 2 / 1280]
//    Permutation : [4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [dictionary]
//    TextNum : [without commas, with commas]
//    Text : [FXXXXB, FBXXXX, FooXXXXBar, FooBarXXXX, FooooXXXXBaaar, FooooBaaarXXXX, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, Words alnum 5 - 8, Words alnum 1 - 16, Long alnum first 1968 - 2128, Long alnum last 1968 - 2128, Long alnum first 4016 - 4176, Long alnum last 4016 - 4176, Long alnum first 8112 - 8272, Long alnum last 8112 - 8272]
//    TwoBytes : [20, 32, 1024, 2048, 4096]
//    Bitflip : [8]
//    SeedBlockLen : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
//    SeedBlockOffset : [1, 2, 3, 5]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 285.070837 seconds

//template <bool bswap>
//static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
//    size_t i = 3;
//    h ^= ROTL32(h, 10) ^ ROTL32(h, 23);
//    uint32_t mul = ((len + h) * 0x86E31586u + 0xD3D0783Bu);
//    for (; i < len; i += 4, data += 4) {
//        h = (ROTL32(h, 7) + GET_U32<bswap>(data, 0)) * (mul += 0x9E3779BAu);
//    }
//    i -= 3;
//    for (; i < len; i++, data++) {
//        h = (ROTL32(h, 7) + data[0]) * (mul += 0x9E3779BAu);
//    }
//    h ^= h >> 17;
//    h *= 0xed5ad4bbu;
//    h ^= h >> 11;
//    h *= 0xac4c1b51u;
//    h ^= h >> 15;
//    h *= 0x31848babu;
//    h ^= h >> 14;
//    return h;
//}

//----------------------------------------------------------------------------------------------
//- log2(p - value) summary:
//
//0     1     2     3     4     5     6     7     8     9    10    11    12
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//1685   359   196    83    52    39    29    12    14     7     3     3     3
//
//13    14    15    16    17    18    19    20    21    22    23    24    25 +
//---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- - ---- -
//3     0     3     0     1     2     3     1     0     3     0     0   246
//
//----------------------------------------------------------------------------------------------
//Summary for: jvmstring
//Overall result : FAIL(154 / 187 passed)
//Failures :
//    Sparse : [20 / 3, 5 / 9]
//    Permutation : [4 - bytes[3 low bits; LE], 4 - bytes[3 low bits; BE], 4 - bytes[3 high bits; LE], 4 - bytes[3 high bits; BE], 4 - bytes[3 high + low bits; LE], 4 - bytes[3 high + low bits; BE], 4 - bytes[0, low bit; LE], 4 - bytes[0, low bit; BE], 4 - bytes[0, high bit; LE], 4 - bytes[0, high bit; BE], 8 - bytes[0, low bit; LE], 8 - bytes[0, low bit; BE], 8 - bytes[0, high bit; LE], 8 - bytes[0, high bit; BE]]
//    Text : [FXXXXB, FooooXXXXBaaar, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX]
//    TwoBytes : [20, 32]
//    Bitflip : [3, 8]
//    SeedZeroes : [1280, 8448]
//    SeedBlockOffset : [0, 1, 2, 3, 4, 5]
//
//    ----------------------------------------------------------------------------------------------
//    Verification value is 0x00000001 - Testing took 482.930569 seconds

template <bool bswap>
static uint32_t jvmstring_impl(const uint8_t* data, size_t len, uint32_t h) {
    size_t i = 3;
    for (; i < len; i += 4, data += 4) {
        h = h * 31 + GET_U32<bswap>(data, 0);
    }
    i -= 3;
    for (; i < len; i++, data++) {
        h = h * 31 + data[0];
    }
    return h;
}

template <bool bswap>
static uint32_t jvmstring2_impl(const uint8_t* data, size_t len, uint32_t h) {
    size_t i = 3;
    h ^= h >> 17;
    h *= 0xed5ad4bbu;
    h ^= h >> 11;
    h *= 0xac4c1b51u;
    h ^= h >> 15;
    h *= 0x31848babu;
    h ^= h >> 14;
    for (; i < len; i += 4, data += 4) {
        h = ((h ^ ROTL32(h, 19) ^ ROTL32(h, 13)) + GET_U32<bswap>(data, 0)) * 0xD168AAADu;
        h ^= ROTL32(h, 21) ^ ROTL32(h, 11);
    }
    i -= 3;
    for (; i < len; i++, data++) {
        h = ((h ^ ROTL32(h, 19) ^ ROTL32(h, 13)) + data[0]) * 0xD168AAADu;
        h ^= ROTL32(h, 21) ^ ROTL32(h, 11);
    }
    h ^= h >> 17;
    h *= 0xed5ad4bbu;
    h ^= h >> 11;
    h *= 0xac4c1b51u;
    h ^= h >> 15;
    h *= 0x31848babu;
    h ^= h >> 14;
    return h;
}

template <bool bswap>
static void jvmstring( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint32_t h = jvmstring_impl<bswap>((const uint8_t *)in, len, (uint32_t)(seed + len));

    PUT_U32<bswap>(h, (uint8_t *)out, 0);
}

template <bool bswap>
static void jvmstring2( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint32_t h = jvmstring2_impl<bswap>((const uint8_t *)in, len, (uint32_t)(seed + len));

    PUT_U32<bswap>(h, (uint8_t *)out, 0);
}

template <bool bswap>
static void jvmstring3( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint32_t h = (uint32_t)(seed + len);
    const uint8_t* data = (const uint8_t*)in;
    size_t i = 3;
    h = (h ^ 7) * 555555555;
    h += h * h | 25;
    h = ROTR32(h, 12);
    h += h * h | 25;
    h ^= h >> 13;

    for (; i < len; i += 4, data += 4) {
        h += (GET_U32<bswap>(data, 0) ^ 7) * 555555555;
        //h ^= h * h | 7;
        h ^= ROTR32(h, 12) ^ ROTR32(h, 21);
    }
    i -= 3;
    for (; i < len; i++, data++) {
        h += (data[0] ^ 7) * 555555555;
        //h ^= h * h | 7;
        h ^= ROTR32(h, 12) ^ ROTR32(h, 21);
    }
    h += h * h | 7;
    h = ROTR32(h, 14);
    h += h * h | 7;
    h ^= h >> 15;

    PUT_U32<bswap>(h, (uint8_t *)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(jvmstring,
   $.src_url    = "https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp",
   $.src_status = HashFamilyInfo::SRC_ACTIVE
 );

REGISTER_HASH(jvmstring,
   $.desc       = "jvmstring",
   $.hash_flags =
         FLAG_HASH_SMALL_SEED,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_SLOW,
   $.bits = 32,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring<false>,
   $.hashfn_bswap    = jvmstring<true>
 );

REGISTER_HASH(jvmstring2,
   $.desc       = "jvmstring2",
   $.hash_flags =
         FLAG_HASH_SMALL_SEED,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_SLOW,
   $.bits = 32,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring2<false>,
   $.hashfn_bswap    = jvmstring2<true>
 );

REGISTER_HASH(jvmstring3,
   $.desc       = "jvmstring3",
   $.hash_flags =
         FLAG_HASH_SMALL_SEED,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_SLOW,
   $.bits = 32,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring3<false>,
   $.hashfn_bswap    = jvmstring3<true>
 );
