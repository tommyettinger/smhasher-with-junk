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

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1707   383   193   114    41    22    13     6     7     7     1     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            1     0     1     0     0     0     1     1     0     0     1     1   245

----------------------------------------------------------------------------------------------
Summary for: jvmstring3
Overall result: FAIL            ( 154 / 187 passed)
Failures:
    BIC                 : [8]
    Sparse              : [20/3, 5/9]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [numbers with commas, FooooooooBaaaaaaarXXXX]
    TwoBytes            : [20, 32]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedZeroes          : [1280, 8448]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 187.796886 seconds
 */
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

/*
Initial results: not good.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1584   335   160    61    52    31     9     7     6     1     4     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            4     1     1     3     1     0     3     1     0     1     2     2   476

----------------------------------------------------------------------------------------------
Summary for: jvmstring4
Overall result: FAIL            ( 112 / 187 passed)
Failures:
    BIC                 : [8]
    Sparse              : [3/8, 3/9, 3/10, 3/12, 3/14, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [numbers without commas, numbers with commas, FooooooBaaaaarXXXX, FooooooooooBaaaaaaaaarXXXX]
    TwoBytes            : [20, 32]
    Bitflip             : [3, 4, 8]
    SeedZeroes          : [1280, 8448]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 277.442588 seconds

With the change to have s run independently, it's worse!
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1201   295   129    59    52    11    16     8     3     2     1     5     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            1     6     2     1     1     3     2     2     0     1     1     1   942

----------------------------------------------------------------------------------------------
Summary for: jvmstring4
Overall result: FAIL            ( 82 / 187 passed)
Failures:
    Avalanche           : [3]
    BIC                 : [3, 8, 11, 15]
    Cyclic              : [4 cycles of 3 bytes, 4 cycles of 4 bytes, 4 cycles of 8 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 8 bytes, 12 cycles of 4 bytes, 16 cycles of 3 bytes, 16 cycles of 4 bytes]
    Sparse              : [6/2, 4/3, 4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 10/2, 20/3, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [dictionary, numbers without commas, numbers with commas, FBXXXX, FooBarXXXX, FooooBaaarXXXX, FooooooBaaaaarXXXX, FooooooooBaaaaaaarXXXX, FooooooooooBaaaaaaaaarXXXX, Words alnum 1-4, Words alnum 5-8, Words alnum 1-16, Words alnum 1-32, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [20, 32, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [3, 8]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [2, 3]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 210.802545 seconds


Not really much better... It looks like QOA is not a great tool here.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1558   335   175    96    54    35    17     7     7     3     2     4     6

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            3     3     1     0     0     1     0     1     2     1     0     0   436

----------------------------------------------------------------------------------------------
Summary for: jvmstring4
Overall result: FAIL            ( 116 / 187 passed)
Failures:
    Sparse              : [4/5, 3/8, 3/9, 3/10, 3/12, 3/14, 20/3, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [numbers with commas]
    TwoBytes            : [20, 32]
    PerlinNoise         : [2]
    Bitflip             : [3, 8]
    SeedZeroes          : [1280, 8448]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 286.772139 seconds

Multiplying h and the absorbed value... with a random-ish rotation... doesn't work well either.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1454   288   144    81    44    30    16     7     4     5     5     8     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     3     3     3     7     1     0     5     0     3     2     4   628

----------------------------------------------------------------------------------------------
Summary for: jvmstring4
Overall result: FAIL            ( 97 / 187 passed)
Failures:
    BIC                 : [8]
    Sparse              : [4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [numbers with commas, FXXXXB, FBXXXX, FooXXXXBar, FooBarXXXX, FooooXXXXBaaar, FooooBaaarXXXX, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooXXXXBaaaaaaaaar, Words alnum 5-8, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [20, 32, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [4, 8]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [1, 2, 3, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 204.218807 seconds

Trying as simple as we can in iterations, but it's not very good...
This uses a 64-bit seed.

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1243   324   160    79    50    25    16    16    10     2     4     7     8

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            3     3     4     5     3     2     1     3     4     3     5     4  1515

----------------------------------------------------------------------------------------------
Summary for: jvmstring4
Overall result: FAIL            ( 54 / 187 passed)
Failures:
    Cyclic              : [4 cycles of 4 bytes, 4 cycles of 5 bytes, 4 cycles of 8 bytes, 8 cycles of 4 bytes, 8 cycles of 5 bytes, 8 cycles of 8 bytes, 12 cycles of 3 bytes, 12 cycles of 4 bytes, 12 cycles of 5 bytes, 12 cycles of 8 bytes, 16 cycles of 3 bytes, 16 cycles of 4 bytes, 16 cycles of 5 bytes, 16 cycles of 8 bytes]
    Sparse              : [4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; BE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [dictionary, numbers without commas, numbers with commas, FXXXXB, FBXXXX, FooXXXXBar, FooBarXXXX, FooooXXXXBaaar, FooooBaaarXXXX, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, Words alnum 1-4, Words alnum 5-8, Words alnum 1-16, Words alnum 1-32, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [20, 32, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [8]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]
    Seed                : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 258.224990 seconds

It passes all tests!
This also uses a 64-bit seed.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2367   578   276   148    58    35    20    13     2     2     0     0     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: jvmstring4
Overall result: pass            ( 187 / 187 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 283.981081 seconds
*/
template <bool bswap>
static void jvmstring4( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint64_t h = (uint64_t)(seed + len);
    const uint8_t* data = (const uint8_t*)in;
    size_t i = 3;
    h ^= 1111111111111111111UL;
    h *= 5555555555555555555UL;
    h ^= ROTL64(h, 11) ^ ROTL64(h, 51);

    for (; i < len; data += 4) {
        h = (ROTL64(h, 41) + GET_U32<bswap>(data, 0)) * 5555555555555555555UL + (i += 4);
    }
    i -= 3;
    for (; i < len; data++) {
        h = (ROTL64(h, 41) + data[0]) * 5555555555555555555UL + ++i;
    }
    h ^= h >> 31;
    h += h * h | 0x65535UL;
    h ^= h >> 29;
    h += h * h | 0x65535UL;
    h ^= h >> 27;

    PUT_U32<bswap>((uint32_t)h, (uint8_t *)out, 0);
}
/*
Getting rid of big multipliers per-word doesn't help at all.
Well, it is faster.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2048   504   229   107    74    34    20    13     5     4     0     0     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     1     0     0     0     0     0     0   459

----------------------------------------------------------------------------------------------
Summary for: jvmstring5
Overall result: FAIL            ( 105 / 187 passed)
Failures:
    Sparse              : [4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE]]
    Text                : [dictionary, numbers without commas, numbers with commas, FXXXXB, FBXXXX, FooXXXXBar, FooBarXXXX, FooooXXXXBaaar, FooooBaaarXXXX, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, Words alnum 5-8, Words alnum 1-16, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [20, 32, 1024, 2048, 4096]
    SeedZeroes          : [1280, 8448]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 200.492680 seconds

Using just addition instead of mixing XOR with addition is faster, but still fails.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2057   481   264   105    69    33    12    10     8     3     2     0     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     3     0     0     0     1     0     0     0     0     2   449

----------------------------------------------------------------------------------------------
Summary for: jvmstring5
Overall result: FAIL            ( 105 / 187 passed)
Failures:
    Cyclic              : [16 cycles of 8 bytes]
    Sparse              : [4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE]]
    Text                : [dictionary, numbers without commas, numbers with commas, FXXXXB, FBXXXX, FooXXXXBar, FooBarXXXX, FooooXXXXBaaar, FooooBaaarXXXX, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, Words alnum 5-8, Words alnum 1-16, Words alnum 1-32, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [20, 32, 1024, 2048, 4096]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 171.868624 seconds

Reading in 64 bits at a time and multiplying helps some... Still fails lots of tests.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2214   554   262   123    57    38    21    13     7     0     2     2     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     1     1     1     1     0     1     0     0     0     0     0   197

----------------------------------------------------------------------------------------------
Summary for: jvmstring5
Overall result: FAIL            ( 146 / 187 passed)
Failures:
    Sparse              : [3/12, 3/14, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Text                : [Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [20, 32, 1024, 2048, 4096]
    SeedBlockLen        : [16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [3]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 199.816799 seconds

Back to reading 32 bits at a time, and this works again!
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2373   599   256   133    63    34    19    14     4     2     1     1     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: jvmstring5
Overall result: pass            ( 187 / 187 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 282.699604 seconds

*/
template <bool bswap>
static void jvmstring5( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint64_t h = (uint64_t)(seed + len);
    const uint8_t* data = (const uint8_t*)in;
    size_t i = 3;
    h ^= 1111111111111111111UL;
    h *= 5555555555555555555UL;
    h ^= ROTL64(h, 11) ^ ROTL64(h, 51);

    for (; i < len; data += 4) {
        h = (ROTL64(h, 41) + GET_U32<bswap>(data, 0)) * 5555555555555555555UL + (i += 4);
    }
    switch (len & 3) {
        case 1: h = (ROTL64(h, 41) + data[0]) * 5555555555555555555UL + 1; break;
        case 2: h = (ROTL64(h, 41) + GET_U16<bswap>(data, 0)) * 5555555555555555555UL + 2; break;
        case 3: h = (ROTL64(h, 41) + GET_U16<bswap>(data, 0) + ((uint64_t)data[2] << 16)) * 5555555555555555555UL + 3; break;
        // case 4: h = (ROTL64(h, 41) + GET_U32<bswap>(data, 0)) * 5555555555555555555UL + 4; break;
        // case 5: h = (ROTL64(h, 41) + GET_U32<bswap>(data, 0) + ((uint64_t)data[4] << 32)) * 5555555555555555555UL + 5; break;
        // case 6: h = (ROTL64(h, 41) + GET_U32<bswap>(data, 0) + ((uint64_t)GET_U16<bswap>(data, 4) << 32)) * 5555555555555555555UL + 6; break;
        // case 7: h = (ROTL64(h, 41) + GET_U32<bswap>(data, 0) + ((uint64_t)GET_U16<bswap>(data, 4) << 32) + ((uint64_t)data[6] << 48)) * 5555555555555555555UL + 7; break;
    }

    h ^= h >> 31;
    h += h * h | 0x65535UL;
    h ^= h >> 29;
    h += h * h | 0x65535UL;
    h ^= h >> 27;

    PUT_U32<bswap>((uint32_t)h, (uint8_t *)out, 0);
}

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4260  1214   649   313   160    74    37    22    11     5     4     1     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     0     2     0     0     0     1     0     0     0     0     0   112

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 172 / 187 passed)
Failures:
    BIC                 : [3, 11, 15]
    Sparse              : [4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    TwoBytes            : [20, 32]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 279.743789 seconds

Trying to rearrange... worse! And slightly slower.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4220  1188   560   269   162    79    40    14    12     8     0     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            1     1     2     3     0     2     1     0     0     2     1     2   300

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 145 / 187 passed)
Failures:
    Sparse              : [4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    TwoBytes            : [20, 32]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 294.198065 seconds

Still struggling...
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4241  1247   583   263   137    72    33    20    12     6    10     9     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     2     2     1     1     2     1     2     0     2     5     0   215

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 149 / 187 passed)
Failures:
    Sparse              : [4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    TwoBytes            : [20, 32]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [3, 4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 295.372662 seconds

Out of the frying pan...
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4104  1257   612   261   146    71    42    23    11    10     7     2     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            3     3     3     0     2     4     1     2     2     1     4     1   296

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 143 / 187 passed)
Failures:
    Sparse              : [4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    TwoBytes            : [20, 32]
    SeedZeroes          : [1280, 8448]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 294.142857 seconds

No multiplication in the bulk loop makes this faster in bulk, but...
Yeah, it still fails quite a few tests.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4149  1224   572   314   137    65    33    19     8    11     3     1     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            4     0     2     0     0     0     0     0     1     0     1     0   324

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 142 / 187 passed)
Failures:
    BIC                 : [3, 8, 11, 15]
    Sparse              : [3/10, 3/12, 3/14, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    TwoBytes            : [20, 32]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 267.513107 seconds

Still not very good... Passes BIC now.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4090  1295   578   321   143    95    24    27    11     8     3     3     3

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            1     1     1     1     0     1     1     0     2     2     5     2   251

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 146 / 187 passed)
Failures:
    Sparse              : [3/12, 3/14, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 high+low bits; BE]]
    TwoBytes            : [20, 32]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 266.310456 seconds

Still about the same at quality, but MX3 is faster (for small keys) as a finalizer.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4168  1263   559   292   125    85    44    20    15     9     2     2    10

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            3     1     3     2     2     2     0     0     4     3     2     1   252

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 145 / 187 passed)
Failures:
    Sparse              : [3/12, 3/14, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 high+low bits; BE]]
    TwoBytes            : [20, 32]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [3, 4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 265.942385 seconds

Without incorporating i or len per-iteration, it's faster but no better on quality.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4124  1242   574   283   136    66    35    18    13     7     7     6     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            3     1     1     0     0     0     0     1     1     0     0     0   351

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 143 / 187 passed)
Failures:
    Sparse              : [3/12, 3/14, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 high+low bits; BE]]
    TwoBytes            : [20, 32, 1024, 2048, 4096]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 244.042758 seconds

Using a multiply helps, a little...
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4302  1274   567   266   142    83    43    10    17     5     7     1     3

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     3     1     3     2     0     1     1     0     1     2   135

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 160 / 187 passed)
Failures:
    Sparse              : [4/16]
    TwoBytes            : [20]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 280.182771 seconds

This one only fails seed-related tests!
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4246  1295   599   300   147    80    37    11     8     7     4     6     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     1     1     1     0     1     0     1     1     0     1     0   122

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: FAIL            ( 162 / 187 passed)
Failures:
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [4]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 295.653378 seconds

Passing!
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4446  1249   588   276   143    92    39    12    12     4     4     2     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: jvmstring6
Overall result: pass            ( 187 / 187 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 296.345587 seconds
 */
template <bool bswap>
static void jvmstring6( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint64_t h = seed ^ 1234567890987654321UL;
    const uint8_t* data = (const uint8_t*)in;
    size_t i = 7;

    h ^= ROTL64(h, 11) ^ ROTL64(h, 51);
    h = h * 5555555555555555555UL + len;

    for (; i < len; data += 8, i += 8) {
        h += GET_U64<bswap>(data, 0);
        h = (h ^ ROTL64(h, 23) ^ ROTL64(h, 50)) * 5555555555555555555UL + i;
    }
    switch (len & 7) {
        case 1: h += (data[0]); break;
        case 2: h += (GET_U16<bswap>(data, 0)); break;
        case 3: h += (GET_U16<bswap>(data, 0) + ((uint64_t)data[2] << 16)); break;
        case 4: h += (GET_U32<bswap>(data, 0)); break;
        case 5: h += (GET_U32<bswap>(data, 0) + ((uint64_t)data[4] << 32)); break;
        case 6: h += (GET_U32<bswap>(data, 0) + ((uint64_t)GET_U16<bswap>(data, 4) << 32)); break;
        case 7: h += (GET_U32<bswap>(data, 0) + ((uint64_t)GET_U16<bswap>(data, 4) << 32) + ((uint64_t)data[6] << 48)); break;
    }
    h = (h ^ ROTL64(h, 23) ^ ROTL64(h, 50)) * 5555555555555555555UL + len;

    h ^= h >> 32;
    h *= 0xBEA225F9EB34556DUL;
    h ^= h >> 29;
    h *= 0xBEA225F9EB34556DUL;
    h ^= h >> 32;
    h *= 0xBEA225F9EB34556DUL;
    h ^= h >> 29;

    PUT_U64<bswap>(h, (uint8_t *)out, 0);
}

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4303  1339   574   324   143    80    44    19     5     7     3     4     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0    24

----------------------------------------------------------------------------------------------
Summary for: jvmstring7
Overall result: FAIL            ( 185 / 187 passed)
Failures:
    Permutation         : [4-bytes [0, high bit; LE], 8-bytes [0, high bit; LE]]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 299.963300 seconds
*/
// But, changing h to ROTL64(h, 11) in the bulk loop...
/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4362  1288   592   291   176    78    34    25    11     8     1     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            1     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: jvmstring7
Overall result: pass            ( 187 / 187 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 329.118076 seconds

Using a loop for the last few bytes:

[[[ Speed Tests ]]]

Small key speed test - [1, 31]-byte keys
   1-byte keys -    14.05 cycles/hash
   2-byte keys -    17.36 cycles/hash
   3-byte keys -    20.87 cycles/hash
   4-byte keys -     8.60 cycles/hash
   5-byte keys -    11.69 cycles/hash
   6-byte keys -    11.90 cycles/hash
   7-byte keys -    11.85 cycles/hash
   8-byte keys -     7.85 cycles/hash
   9-byte keys -    10.13 cycles/hash
  10-byte keys -    12.44 cycles/hash
  11-byte keys -    14.85 cycles/hash
  12-byte keys -    10.79 cycles/hash
  13-byte keys -    13.05 cycles/hash
  14-byte keys -    15.47 cycles/hash
  15-byte keys -    17.67 cycles/hash
  16-byte keys -    13.61 cycles/hash
  17-byte keys -    16.11 cycles/hash
  18-byte keys -    18.27 cycles/hash
  19-byte keys -    20.59 cycles/hash
  20-byte keys -    16.62 cycles/hash
  21-byte keys -    19.19 cycles/hash
  22-byte keys -    21.28 cycles/hash
  23-byte keys -    23.51 cycles/hash
  24-byte keys -    19.43 cycles/hash
  25-byte keys -    21.76 cycles/hash
  26-byte keys -    24.12 cycles/hash
  27-byte keys -    26.41 cycles/hash
  28-byte keys -    22.33 cycles/hash
  29-byte keys -    24.69 cycles/hash
  30-byte keys -    27.04 cycles/hash
  31-byte keys -    29.29 cycles/hash
Average        -    17.51 cycles/hash
 rnd-byte keys -    16.62 cycles/hash

Bulk speed test - 262144-byte keys
Alignment   7 -  1.38 bytes/cycle -  4.50 GiB/sec @ 3.5 ghz
Alignment   6 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   5 -  1.38 bytes/cycle -  4.50 GiB/sec @ 3.5 ghz
Alignment   4 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   3 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   2 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   1 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   0 -  1.38 bytes/cycle -  4.50 GiB/sec @ 3.5 ghz
Average       -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment rnd -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz

Bulk speed test - [262017, 262144]-byte keys
Alignment   7 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   6 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   5 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   4 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   3 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   2 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   1 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   0 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Average       -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment rnd -  1.37 bytes/cycle -  4.47 GiB/sec @ 3.5 ghz

Verification value is 0x00000001 - Testing took 24.530428 seconds

With switch at the end, which is faster for short keys:
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4295  1301   625   306   152    88    45    31    11     8     3     3     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            1     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: jvmstring7
Overall result: pass            ( 187 / 187 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 317.625343 seconds

With switch:
[[[ Speed Tests ]]]

Small key speed test - [1, 31]-byte keys
   1-byte keys -    14.20 cycles/hash
   2-byte keys -    14.22 cycles/hash
   3-byte keys -    15.34 cycles/hash
   4-byte keys -     8.69 cycles/hash
   5-byte keys -    12.56 cycles/hash
   6-byte keys -    12.72 cycles/hash
   7-byte keys -    13.51 cycles/hash
   8-byte keys -    12.66 cycles/hash
   9-byte keys -    12.70 cycles/hash
  10-byte keys -    10.67 cycles/hash
  11-byte keys -    11.20 cycles/hash
  12-byte keys -    10.70 cycles/hash
  13-byte keys -    13.62 cycles/hash
  14-byte keys -    13.60 cycles/hash
  15-byte keys -    14.06 cycles/hash
  16-byte keys -    13.52 cycles/hash
  17-byte keys -    16.68 cycles/hash
  18-byte keys -    16.58 cycles/hash
  19-byte keys -    17.02 cycles/hash
  20-byte keys -    16.39 cycles/hash
  21-byte keys -    19.34 cycles/hash
  22-byte keys -    19.34 cycles/hash
  23-byte keys -    20.03 cycles/hash
  24-byte keys -    19.63 cycles/hash
  25-byte keys -    22.23 cycles/hash
  26-byte keys -    22.23 cycles/hash
  27-byte keys -    22.86 cycles/hash
  28-byte keys -    22.20 cycles/hash
  29-byte keys -    25.12 cycles/hash
  30-byte keys -    25.20 cycles/hash
  31-byte keys -    25.75 cycles/hash
Average        -    16.60 cycles/hash
 rnd-byte keys -    15.26 cycles/hash

Bulk speed test - 262144-byte keys
Alignment   7 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   6 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   5 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   4 -  1.39 bytes/cycle -  4.52 GiB/sec @ 3.5 ghz
Alignment   3 -  1.39 bytes/cycle -  4.52 GiB/sec @ 3.5 ghz
Alignment   2 -  1.39 bytes/cycle -  4.52 GiB/sec @ 3.5 ghz
Alignment   1 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Alignment   0 -  1.38 bytes/cycle -  4.49 GiB/sec @ 3.5 ghz
Average       -  1.38 bytes/cycle -  4.50 GiB/sec @ 3.5 ghz
Alignment rnd -  1.38 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz

Bulk speed test - [262017, 262144]-byte keys
Alignment   7 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   6 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   5 -  1.39 bytes/cycle -  4.52 GiB/sec @ 3.5 ghz
Alignment   4 -  1.38 bytes/cycle -  4.51 GiB/sec @ 3.5 ghz
Alignment   3 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   2 -  1.37 bytes/cycle -  4.48 GiB/sec @ 3.5 ghz
Alignment   1 -  1.39 bytes/cycle -  4.52 GiB/sec @ 3.5 ghz
Alignment   0 -  1.38 bytes/cycle -  4.51 GiB/sec @ 3.5 ghz
Average       -  1.38 bytes/cycle -  4.50 GiB/sec @ 3.5 ghz
Alignment rnd -  1.37 bytes/cycle -  4.47 GiB/sec @ 3.5 ghz

Verification value is 0x00000001 - Testing took 24.578312 seconds

Commenting out the two marked xor-rotate-xor-rotate lines makes this fail a lot of tests.
It's only a tiny bit faster on short keys...
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2368   682   322   161    89    47    19    17     9     8     2    12    14

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            5     2     2     1     4     2     2     1     1     0     3     1  3095

----------------------------------------------------------------------------------------------
Summary for: jvmstring7
Overall result: FAIL            ( 76 / 187 passed)
Failures:
    Avalanche           : [3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
    BIC                 : [3, 8, 11, 15]
    Sparse              : [6/2, 4/3, 4/4, 4/5, 3/7, 3/8, 3/10, 3/12, 3/14, 10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [numbers without commas, numbers with commas, FXXXXB, FBXXXX, XXXXFB, FooXXXXBar, FooBarXXXX, XXXXFooBar, FooooXXXXBaaar, FooooBaaarXXXX, XXXXFooooBaaar, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, XXXXFooooooBaaaaar, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, XXXXFooooooooooBaaaaaaaaar, Words alnum 1-4, Long alnum last 1968-2128, Long alnum last 4016-4176, Long alnum last 8112-8272]
    TwoBytes            : [20, 32]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedSparse          : [2, 3, 6, 15, 18, 31]
    SeedBlockOffset     : [0, 1]
    Seed                : [2, 3, 6, 15, 18, 31, 52]
    SeedAvalanche       : [4, 8, 16, 24, 32]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 319.629891 seconds
*/
template <bool bswap>
static void jvmstring7( const void * in, const size_t len, const seed_t seed, void * out ) {
    const uint8_t* data = (const uint8_t*)in;
    uint64_t h = seed;
    h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    h = h * 0xBEA225F9EB34556DUL + len;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44); // don't remove this XRXR!

    size_t i = 3;
    for (; i < len; i += 4, data += 4) {
        h = (GET_U32<bswap>(data, 0) - ROTL64(h, 11)) * 0x2127599BF4325C37UL;
    }
    // i -= 3;
    // for (; i < len; i++, data++) {
        // h = (data[0] - h) * 0x2127599BF4325C37UL;
    // }
    switch (len & 3) {
    case 1: h = (data[0] - ROTL64(h, 11)) * 0x2127599BF4325C37UL; break;
    case 2: h = (GET_U16<bswap>(data, 0) - ROTL64(h, 11)) * 0x2127599BF4325C37UL; break;
    case 3: h = (GET_U16<bswap>(data, 0) + ((uint64_t)data[2] << 16) - ROTL64(h, 11)) * 0x2127599BF4325C37UL; break;
    }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47); // don't remove this XRXR!
    h *= 0xBEA225F9EB34556DUL;
    h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    PUT_U64<bswap>(h, (uint8_t *)out, 0);
}

// Loading 64 bits at a time doesn't work here.
/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4032  1137   569   286   142    71    30    16    12     7     2     5     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            1     4     1     0     1     1     1     2     0     0     1     1   547

----------------------------------------------------------------------------------------------
Summary for: jvmstring8
Overall result: FAIL            ( 129 / 187 passed)
Failures:
    Avalanche           : [8, 9, 10, 12, 16, 20, 64, 128]
    BIC                 : [8, 11, 15]
    Sparse              : [3/10, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE]]
    Text                : [FooooooXXXXBaaaaar, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [20, 32, 1024, 2048, 4096]
    Bitflip             : [8]
    SeedBlockLen        : [16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 260.369522 seconds
 */
template <bool bswap>
static void jvmstring8( const void * in, const size_t len, const seed_t seed, void * out ) {
    const uint8_t* data = (const uint8_t*)in;
    uint64_t h = seed;
    h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    h = h * 0xBEA225F9EB34556DUL + len;
    h ^= ROTL64(h, 21) ^ ROTL64(h, 44);

    size_t i = 7;
    for (; i < len; i += 8, data += 8) {
        h = (GET_U64<bswap>(data, 0) - ROTL64(h, 11)) * 0x2127599BF4325C37UL;
    }
    i -= 7;
    for (; i < len; i++, data++) {
        h = (data[0] - h) * 0x2127599BF4325C37UL;
    }
    h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    h *= 0xBEA225F9EB34556DUL;
    h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    PUT_U64<bswap>(h, (uint8_t *)out, 0);
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

REGISTER_HASH(jvmstring4,
   $.desc       = "jvmstring4",
   $.hash_flags =
         0,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_SLOW,
   $.bits = 32,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring4<false>,
   $.hashfn_bswap    = jvmstring4<true>
 );

REGISTER_HASH(jvmstring5,
   $.desc       = "jvmstring5",
   $.hash_flags =
         0,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_SLOW,
   $.bits = 32,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring5<false>,
   $.hashfn_bswap    = jvmstring5<true>
 );

REGISTER_HASH(jvmstring6,
   $.desc       = "jvmstring6",
   $.hash_flags =
         0,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_SLOW,
   $.bits = 64,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring6<false>,
   $.hashfn_bswap    = jvmstring6<true>
 );

REGISTER_HASH(jvmstring7,
   $.desc       = "jvmstring7",
   $.hash_flags =
         0,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_SLOW,
   $.bits = 64,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring7<false>,
   $.hashfn_bswap    = jvmstring7<true>
 );

REGISTER_HASH(jvmstring8,
   $.desc       = "jvmstring8",
   $.hash_flags =
         0,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_SLOW,
   $.bits = 64,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring8<false>,
   $.hashfn_bswap    = jvmstring8<true>
 );
