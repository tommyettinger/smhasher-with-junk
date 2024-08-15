/*
 * whitewaterhash - Very fast, high quality, platform independent hashing algorithm.
 * Based on 'rapidhash', which is:
 * Copyright (C) 2024 Nicolas De Carli
 *
 * Based on 'wyhash', by Wang Yi <godspeed_china@yeah.net>
 *
 * BSD 2-Clause License (https://www.opensource.org/licenses/bsd-license.php)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following disclaimer
 *      in the documentation and/or other materials provided with the
 *      distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You can contact the author at:
 *   - rapidhash source repository: https://github.com/Nicoshev/rapidhash
 */

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4458  1224   602   315   133    76    37    24     7     5     1     1     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: whitewaterhash
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 427.493952 seconds
*/
 /*
  *  Includes.
  */
#include "Platform.h"
#include "Hashlib.h"

  /*
   *  Read functions.
   */
template <bool bswap>
static inline uint64_t ww_read64(const uint8_t* p) {
    return GET_U64<bswap>(p, 0);
}

template <bool bswap>
static inline uint64_t ww_read32(const uint8_t* p) {
    return GET_U32<bswap>(p, 0);
}

static inline uint64_t ww_readSmall(const uint8_t* p, size_t k) {
    return (((uint64_t)p[0]) << 56) | (((uint64_t)p[k >> 1]) << 32) | p[k - 1];
}

/*
 *  Part of a mixing function that pseudorandomly moves bits between the given 64-bit A and B.
 *
 *  @param A  Address of a 64-bit number.
 *  @param B  Address of a 64-bit number.
 *
 *  Stores A's and B's initial values into a and b.
 *
 *  Overwrites A's contents with A ^ a * ROTL64(b, 31) + 0xD1B54A32D192ED03.
 *  Overwrites B's contents with B ^ b * ROTL64(a, 33) + 0x9E3779B97F4A7C16.
 * 
 * The bits of A and of B will not be uniformly distributed after this; in particular,
 * the LSB of A will be '1' 75% of the time given uniformly distributed A and B inputs,
 * and the LSB of B will be '0' 75% of the time given the same distribution.
 */
template <bool isProtected>
static inline void ww_mum(uint64_t* A, uint64_t* B) {
    uint64_t a = *A, b = *B;
    *B = a * ROTL64(b, 31) + b;
    *A = b * ROTL64(a, 33) + a;
    //*A ^= a * ROTL64(b, 31) + UINT64_C(0xD1B54A32D192ED03);
    //*B ^= b * ROTL64(a, 33) + UINT64_C(0x9E3779B97F4A7C16);
}

/*
 *  Mixes the given A and B thoroughly and evenly, given uniformly distributed inputs.
 *  The distribution this produces should still be somewhat random even given very non-random input.
 *  Runs ww_mum on A and B, add A and B, xor-rotate(21)-xor-rotate(44) and return.
 */
template <bool isProtected>
static inline uint64_t ww_mix(uint64_t A, uint64_t B) {
    ww_mum<isProtected>(&A, &B);
    uint64_t r = A ^ B;
    return r ^ ROTL64(r, 21) ^ ROTL64(r, 44);
}

/*
 *  whitewaterhash main function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *  @param secret  Triplet of 64-bit secrets used to alter hash result predictably.
 *
 *  Returns a 64-bit hash.
 */
template <bool bswap, bool isProtected, bool unrolled>
static inline uint64_t whitewaterhash(const void* key, size_t len, uint64_t seed, const uint64_t* secrets) {
    const uint8_t* p = (const uint8_t*)key;
    uint64_t        a, b;

    seed ^= ww_mix<isProtected>(seed + secrets[2], UINT64_C(0xD1B54A32D192ED03) ^ secrets[0]) ^ len;

    if (likely(len <= 16)) {
        if (likely(len >= 4)) {
            const uint8_t* plast = p + len - 4;
            a = (ww_read32<bswap>(p) << 32) | ww_read32<bswap>(plast);
            const uint64_t delta = ((len & 24) >> (len >> 3));
            b = (ww_read32<bswap>(p + delta) << 32) | ww_read32<bswap>(plast - delta);
        }
        else if (likely(len > 0)) {
            a = ww_readSmall(p, len);
            b = 0;
        }
        else {
            a = b = 0;
        }
    }
    else {
        size_t i = len;
        if (unlikely(i > 48)) {
            uint64_t see1 = seed, see2 = seed;
            if (unrolled) {
                while (likely(i >= 96)) {
                    seed = ww_mix<isProtected>(ww_read64<bswap>(p) ^ secrets[0],
                        ww_read64<bswap>(p + 8) ^ seed);
                    see1 = ww_mix<isProtected>(ww_read64<bswap>(p + 16) ^ secrets[1],
                        ww_read64<bswap>(p + 24) ^ see1);
                    see2 = ww_mix<isProtected>(ww_read64<bswap>(p + 32) ^ secrets[2],
                        ww_read64<bswap>(p + 40) ^ see2);
                    seed = ww_mix<isProtected>(ww_read64<bswap>(p + 48) ^ secrets[0],
                        ww_read64<bswap>(p + 56) ^ seed);
                    see1 = ww_mix<isProtected>(ww_read64<bswap>(p + 64) ^ secrets[1],
                        ww_read64<bswap>(p + 72) ^ see1);
                    see2 = ww_mix<isProtected>(ww_read64<bswap>(p + 80) ^ secrets[2],
                        ww_read64<bswap>(p + 88) ^ see2);
                    p += 96; i -= 96;
                }
                if (unlikely(i >= 48)) {
                    seed = ww_mix<isProtected>(ww_read64<bswap>(p) ^ secrets[0],
                        ww_read64<bswap>(p + 8) ^ seed);
                    see1 = ww_mix<isProtected>(ww_read64<bswap>(p + 16) ^ secrets[1],
                        ww_read64<bswap>(p + 24) ^ see1);
                    see2 = ww_mix<isProtected>(ww_read64<bswap>(p + 32) ^ secrets[2],
                        ww_read64<bswap>(p + 40) ^ see2);
                    p += 48; i -= 48;
                }
            }
            else {
                do {
                    seed = ww_mix<isProtected>(ww_read64<bswap>(p) ^ secrets[0],
                        ww_read64<bswap>(p + 8) ^ seed);
                    see1 = ww_mix<isProtected>(ww_read64<bswap>(p + 16) ^ secrets[1],
                        ww_read64<bswap>(p + 24) ^ see1);
                    see2 = ww_mix<isProtected>(ww_read64<bswap>(p + 32) ^ secrets[2],
                        ww_read64<bswap>(p + 40) ^ see2);
                    p += 48; i -= 48;
                } while (likely(i >= 48));
            }
            seed ^= see1 ^ see2;
        }
        if (i > 16) {
            seed = ww_mix<isProtected>(ww_read64<bswap>(p) ^ secrets[2],
                ww_read64<bswap>(p + 8) ^ seed ^ secrets[1]);
            if (i > 32) {
                seed = ww_mix<isProtected>(ww_read64<bswap>(p + 16) ^ secrets[2],
                    ww_read64<bswap>(p + 24) ^ seed);
            }
        }
        a = ww_read64<bswap>(p + i - 16);
        b = ww_read64<bswap>(p + i - 8);
    }
    a ^= secrets[2];
    b ^= seed;
    ww_mum<isProtected>(&a, &b);
    return ww_mix<isProtected>(a ^ secrets[0] ^ len, b ^ secrets[1]);
}

/*
 *  Default secret parameters.
 */
static const uint64_t ww_secret[3] = {
    UINT64_C(0x2d358dccaa6c78a5), UINT64_C(0x8bb84b93962eacc9), UINT64_C(0x4b33a62ed433d4a3)
};

//-----------------------------------------------------------------------------
template <bool bswap, bool isProtected, bool unrolled>
static void WhiteWaterHash64(const void* in, const size_t len, const seed_t seed, void* out) {
    if (isLE()) {
        PUT_U64<bswap>(whitewaterhash<false, isProtected, unrolled>(in, len,
            (uint64_t)seed, ww_secret), (uint8_t*)out, 0);
    }
    else {
        PUT_U64<bswap>(whitewaterhash<true, isProtected, unrolled>(in, len,
            (uint64_t)seed, ww_secret), (uint8_t*)out, 0);
    }
}

//-----------------------------------------------------------------------------
static bool whitewaterhash64_selftest(void) {
    struct {
        const uint64_t  hash;
        const char* key;
    } selftests[] = {
        { UINT64_C(0xd7ba21ce5b60c875), "" }                          ,
        { UINT64_C(0x173490eee96c67d3), "a" }                         ,
        { UINT64_C(0xefeb3a9ca431f32f), "abc" }                       ,
        { UINT64_C(0x43e58e89c0ab73c9), "message digest" }            ,
        { UINT64_C(0xd168b4692b7a72bf), "abcdefghijklmnopqrstuvwxyz" },
        { UINT64_C(0xd6afc4e006eddac6), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" },
        { UINT64_C(0x9a5d091b74c3df3b), "123456789012345678901234567890123456789012345678901234567890" \
                                        "12345678901234567890" },
    };

    for (size_t i = 0; i < sizeof(selftests) / sizeof(selftests[0]); i++) {
        uint64_t h;
        if (isLE()) {
            WhiteWaterHash64<false, false, false>(selftests[i].key, strlen(selftests[i].key), i, &h);
        }
        else {
            WhiteWaterHash64<true, false, false>(selftests[i].key, strlen(selftests[i].key), i, &h);
            // h is in little-endian format
            h = COND_BSWAP(h, true);
        }
        if (h != selftests[i].hash) {
            printf("Hash %016" PRIx64 " != expected %016" PRIx64 " for string \"%s\"\n",
                h, selftests[i].hash, selftests[i].key);
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
REGISTER_FAMILY(whitewaterhash,
    $.src_url = "https://github.com/Nicoshev/rapidhash",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(whitewaterhash,
    $.desc = "whitewaterhash, 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_LICENSE_BSD,
    $.bits = 64,
    $.verification_LE = 0x316F5300,
    $.verification_BE = 0,
    $.hashfn_native = WhiteWaterHash64<false, false, true>,
    $.hashfn_bswap = WhiteWaterHash64<true, false, true>,
    $.initfn = whitewaterhash64_selftest
);
