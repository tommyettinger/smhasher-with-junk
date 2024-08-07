#pragma once

#include "Types.h"

#include "jody_hash32.h"
#include "jody_hash64.h"

#include "MurmurHash1.h"
#include "MurmurHash2.h"
#include "MurmurHash3.h"

#if defined(__x86_64__)
#define XXH_INLINE_ALL
#include "xxhash.h"
#include "metrohash.h"
#include "cmetrohash.h"
#include "opt_cmetrohash.h"
#endif

#include "fasthash.h"
#include <stdlib.h>
#include <string.h>

//----------
// These are _not_ hash functions (even though people tend to use crc32 as one...)

void BadHash_with_state    ( const void * key, int len, const void *state, void * out );
void BadHashSiren_with_state    ( const void * key, int len, const void *state, void * out );
void BadHashSiren_seed_state( int seedbits, const void *seed, void *state);

void DoNothingHash_with_state( const void * key, int len, const void *state, void * out );
void DoNothingHash_seed_state( int seedbits, const void *seed, void *state);
void NoopOAATReadHash_with_state( const void * key, int len, const void *state, void * out );
void crc32_with_state_test ( const void * key, int len, const void *state, void * out );

//----------
// Cryptographic hashes

void md5_seed_state        ( int seedbits, const void *seed, void *state);
void md5_32_with_state     ( const void * key, int len, const void *state, void * out );
void md5_with_state        ( const void * key, int len, const void *state, void * out );
void sha1_32a_with_state   ( const void * key, int len, const void *state, void * out );
void sha1_64a_with_state   ( const void * key, int len, const void *state, void * out );
void sha1_32b_with_state   ( const void * key, int len, const void *state, void * out );

//----------
// General purpose hashes

#ifdef __SSE2__
void hasshe2_test          ( const void * key, int len, const void *state, void * out );
#endif
#if defined(__SSE4_2__) && defined(__x86_64__)
void crc32c_hw_with_state_test( const void * key, int len, const void *state, void * out );
void crc32c_hw1_with_state_test( const void * key, int len, const void *state, void * out );
void crc64c_hw_with_state_test( const void * key, int len, const void *state, void * out );
void falkhash_with_state_test_cxx(const void *key, int len, const void *seed, void *out);
#endif
void FNV32a_with_state_test( const void * key, int len, const void *state, void * out );
void FNV32a_YoshimitsuTRIAD_with_state_test( const void * key, int len, const void *state, void * out );
void FNV32a_NH_with_state_test( const void * key, int len, const void *state, void * out );
void FNV64a_with_state_test( const void * key, int len, const void *state, void * out );
void Bernstein_with_state  ( const void * key, int len, const void *state, void * out );
void sdbm                  ( const void * key, int len, const void *state, void * out );
void x17_test              ( const void * key, int len, const void *seed, void * out );
void JenkinsOAATH_with_state( const void * key, int len, const void *seed, void * out );
void JenkinsOAAT_with_state( const void * key, int len, const void *seed, void * out );
void GoodOAAT              ( const void * key, int len, const void *state, void * out );
void MicroOAAT             ( const void * key, int len, const void *state, void * out );
void SuperFastHash         ( const void * key, int len, const void *state, void * out );
void lookup3_with_state_test( const void * key, int len, const void* state, void * out );
void MurmurOAAT_with_state_test       ( const void * key, int len, const void *state, void * out );
void Crap8_test            ( const void * key, int len, const void *state, void * out );

void CityHash32_with_state_test  ( const void * key, int len, const void * seed, void * out );
void CityHash64_with_state_test  ( const void * key, int len, const void * seed, void * out );
void CityHash128_with_state_test  ( const void * key, int len, const void * seed, void * out );
void CityHashCrc128_with_state_test  ( const void * key, int len, const void * seed, void * out );
void FarmHash32_with_state_test       ( const void * key, int len, const void *state, void * out );
void FarmHash64_with_state_test       ( const void * key, int len, const void *state, void * out );
void FarmHash128_with_state_test      ( const void * key, int len, const void *state, void * out );

void farmhash32_c_with_state_test       ( const void * key, int len, const void *state, void * out );
void farmhash64_c_with_state_test       ( const void * key, int len, const void *state, void * out );
void farmhash128_c_with_state_test      ( const void * key, int len, const void *state, void * out );

void marvin_32_with_state_test ( const void * key, int len, const void * state, void * out );
void lua_v53_string_hash_oaat (const void *str, int len, const void *seed, void *out);
void lua_v53_string_hash (const void *str, int len, const void *seed, void *out);


void SpookyHash32_with_state_test ( const void * key, int len, const void * state, void * out );
void SpookyHash64_with_state_test ( const void * key, int len, const void * state, void * out );
void SpookyHash128_with_state_test ( const void * key, int len, const void * state, void * out );
void SpookyHash_seed_state_test( int seedbits, const void *seed, void *state);


//----------
void siphash_with_state_test( const void * key, int len, const void * seed, void * out );
void siphash13_with_state_test( const void * key, int len, const void * seed, void * out );
void siphash_seed_state_test( int seedbits, const void *seed, void *state);
void halfsiphash_with_state_test( const void * key, int len, const void * seed, void * out );
void halfsiphash_seed_state_test( int seedbits, const void *seed, void *state);
//-----------------------------------------------------------------------------
// Test harnesses for Murmur1/2

inline void MurmurHash1_with_state_test ( const void * key, int len, const void *state, void * out )
{
  *(uint32_t*)out = MurmurHash1(key,len,*((uint32_t *)state));
}

inline void MurmurHash2_with_state_test ( const void * key, int len, const void *state, void * out )
{
  *(uint32_t*)out = MurmurHash2(key,len,*((uint32_t *)state));
}

inline void MurmurHash2A_with_state_test ( const void * key, int len, const void *state, void * out )
{
  *(uint32_t*)out = MurmurHash2A(key,len,*((uint32_t *)state));
}

inline void MurmurHash64A_with_state_test ( const void * key, int len, const void *state, void * out )
{
  *(uint64_t*)out = MurmurHash64A(key,len,*((uint64_t *)state));
}

inline void MurmurHash64B_with_state_test ( const void * key, int len, const void *state, void * out )
{
  *(uint64_t*)out = MurmurHash64B(key,len,*((uint64_t *)state));
}

inline void jodyhash32_with_state_test( const void * key, int len, const void *state, void * out ) {
    void *buf;
    bool do_free= 0;
    if (0 && len % sizeof(jodyhash32_t)) {
        int words= (len + sizeof(jodyhash32_t)-1 ) / sizeof(jodyhash32_t);
        buf= malloc(words * sizeof(jodyhash32_t));
        memcpy(buf,key,len);
        do_free = 1;
    } else {
        buf= (void*)key;
    }
    *(uint32_t*)out = (uint32_t) jody_block_hash32((const jodyhash32_t *)buf, *((jodyhash32_t *)state), (size_t) len);
    if (do_free) free(buf);
}

static uint64_t last_state=0;
inline void jodyhash64_with_state_test( const void * key, int len, const void *state, void * out ) {
    void *buf;
    bool do_free= 0;
    if (0 && len % sizeof(jodyhash_t)) {
        int words= (len + sizeof(jodyhash_t)-1 ) / sizeof(jodyhash_t);
        buf= malloc(words * sizeof(jodyhash_t));
        memcpy(buf,key,len);
        do_free = 1;
    } else {
        buf= (void*)key;
    }
    *(uint64_t*)out = (uint64_t) jody_block_hash((const jodyhash_t *)buf, *((jodyhash_t*)state), (size_t) len);
    if (do_free) free(buf);
}

#if defined(__x86_64__)
inline void xxHash32_with_state_test( const void * key, int len, const void *state, void * out ) {
  *(uint32_t*)out = (uint32_t) XXH32(key, (size_t) len, *((unsigned *) state));
}

inline void xxHash64_with_state_test( const void * key, int len, const void *state, void * out ) {
  *(uint64_t*)out = (uint64_t) XXH64(key, (size_t) len, *((unsigned long long *)state));
}

inline void xxh3_with_state_test( const void * key, int len, const void *state, void * out ) {
  *(uint64_t*)out = (uint64_t) XXH3_64bits_withSeed(key, (size_t) len, *((unsigned long long *)state));
}

inline void metrohash64_1_with_state_test ( const void * key, int len, const void *state, void * out ) {
  metrohash64_1((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
inline void metrohash64_2_with_state_test ( const void * key, int len, const void *state, void * out ) {
  metrohash64_2((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
inline void metrohash128_1_with_state_test ( const void * key, int len, const void *state, void * out ) {
  metrohash128_1((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
inline void metrohash128_2_with_state_test ( const void * key, int len, const void *state, void * out ) {
  metrohash128_2((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
inline void speckle_with_state_test ( const void * key, int len, const void *state, void * out ) {
  speckleHash((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
#if defined(__SSE4_2__) && defined(__x86_64__)

inline void metrohash64crc_1_with_state_test ( const void * key, int len, const void *state, void * out ) {
  metrohash64crc_1((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
inline void metrohash64crc_2_with_state_test ( const void * key, int len, const void *state, void * out ) {
  metrohash64crc_2((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
inline void metrohash128crc_1_with_state_test ( const void * key, int len, const void *state, void * out ) {
  metrohash128crc_1((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
inline void metrohash128crc_2_with_state_test ( const void * key, int len, const void *state, void * out ) {
  metrohash128crc_2((const uint8_t *)key,(uint64_t)len,*((uint64_t*)state),(uint8_t *)out);
}
#endif
inline void cmetrohash64_1_with_state_test ( const void * key, int len, const void *state, void * out ) {
  cmetrohash64_1((const uint8_t *)key,(uint64_t)len,*((uint64_t *)state),(uint8_t *)out);
}
inline void cmetrohash64_1_optshort_with_state_test ( const void * key, int len, const void *state, void * out ) {
  cmetrohash64_1_optshort((const uint8_t *)key,(uint64_t)len,*((uint64_t *)state),(uint8_t *)out);
}
inline void cmetrohash64_2_with_state_test ( const void * key, int len, const void *state, void * out ) {
  cmetrohash64_2((const uint8_t *)key,(uint64_t)len,*((uint64_t *)state),(uint8_t *)out);
}

inline void fasthash32_test ( const void * key, int len, const void *state, void * out ) {
  *(uint32_t*)out = fasthash32(key, (size_t) len, *((uint32_t*)state));
}
inline void fasthash64_test ( const void * key, int len, const void *state, void * out ) {
  *(uint64_t*)out = fasthash64(key, (size_t) len, *((uint64_t*)state));
}
#endif

void mum_hash_with_state_test(const void * key, int len, const void *state, void * out);

//-----------------------------------------------------------------------------

#include "t1ha.h"

inline void t1ha2_atonce_test(const void * key, int len, const void *seed, void * out)
{
  *(uint64_t*)out = t1ha2_atonce(key, len, *((uint64_t*)seed));
}

inline void t1ha2_stream_test(const void * key, int len, const void *seed, void * out)
{
  t1ha_context_t ctx;
  t1ha2_init(&ctx, ((uint64_t*)seed)[0], ((uint64_t*)seed)[1]);
  t1ha2_update(&ctx, key, len);
  *(uint64_t*)out = t1ha2_final(&ctx, NULL);
}

inline void t1ha2_atonce128_test(const void * key, int len, const void *seed, void * out)
{
  *(uint64_t*)out = t1ha2_atonce128((uint64_t*)out + 1, key, len, *((uint64_t*)seed));
}

inline void t1ha2_stream128_test(const void * key, int len, const void *seed, void * out)
{
  t1ha_context_t ctx;
  t1ha2_init(&ctx, ((uint64_t*)seed)[0], ((uint64_t*)seed)[1]);
  t1ha2_update(&ctx, key, len);
  *(uint64_t*)out = t1ha2_final(&ctx, (uint64_t*)out + 1);
}

inline void t1ha1_64le_test(const void * key, int len, const void *seed, void * out)
{
  *(uint64_t*)out = t1ha1_le(key, len, *((uint64_t*)seed));
}

inline void t1ha1_64be_test(const void * key, int len, const void* seed, void * out)
{
  *(uint64_t*)out = t1ha1_be(key, len, *((uint64_t*)seed));
}

inline void t1ha0_32le_test(const void * key, int len, const void *state, void * out)
{
  *(uint64_t*)out = t1ha0_32le(key, len, *((uint64_t*)state));
}

inline void t1ha0_32be_test(const void * key, int len, const void *state, void * out)
{
  *(uint64_t*)out = t1ha0_32be(key, len, *((uint64_t*)state));
}

#ifdef AES_AVAIL
inline void t1ha0_ia32aes_noavx_test(const void * key, int len, const void *state, void * out)
{
  *(uint64_t*)out = t1ha0_ia32aes_noavx(key, len, *((uint64_t*)state));
}

#ifdef AVX_AVAIL
inline void t1ha0_ia32aes_avx_test(const void * key, int len, const void *state, void * out)
{
  *(uint64_t*)out = t1ha0_ia32aes_avx(key, len, *((uint64_t*)state));
}
inline void t1ha0_ia32aes_avx2_test(const void * key, int len, const void *state, void * out)
{
  *(uint64_t*)out = t1ha0_ia32aes_avx2(key, len, *((uint64_t*)state));
}
#endif /* AVX_AVAIL */
#endif /* AES_AVAIL */

//-----------------------------------------------------------------------------

#ifdef HAVE_FUNNY_HASH
#include "funny_hash.h"
inline void
funny_hash64_2_seed_state_test(int seed_bits, const void * seed, void *state)
{
    memcpy(state,seed,seed_bits/8);
}

inline void
funny_hash64_2_with_state_test(const void *key, int len, const void * state, void *out)
{
    uint64_t *s64 = (uint64_t *)state;
  *(uint64_t *) out = fh64_string_hash2(key, len, s64[0], s64[1]);
}
#endif

#ifdef HAVE_FANOM_HASH
void fanom_hash64_seed_state_test(int seed_bits, const void * seed, void *state);

void fanom_hash64_with_versionstate_test(const void *key, int len, const void * state, void *out);
#endif

#include "wyhash.h"
void wyhash_test( const void * key, int len, const void * seed, void * out );

//#define WOOT
#include "wyhash3.h"
void wyhash3_test( const void * key, int len, const void * seed, void * out );

#include "wyhash_final_3.h"
void wyhash3final_test( const void * key, int len, const void * seed, void * out );

#include "wildhash.h"
void wildhash_test( const void * key, int len, const void * seed, void * out );

#include "waterhash.h"
void waterhash_test( const void * key, int len, const void * seed, void * out );

#include "wheathash.h"
void wheathash_test( const void * key, int len, const void * seed, void * out );
void wheatoaathash_test( const void * key, int len, const void * seed, void * out );

#include "woothash.h"
void woothash_test( const void * key, int len, const void * seed, void * out );

#include "wickethash.h"
void wickethash_test( const void * key, int len, const void * seed, void * out );

void puller_test( const void * key, int len, const void * seed, void * out );

void Gwoemul_with_state  ( const void * key, int len, const void *state, void * out );

void WobbleOAAT_with_state_test( const void * key, int len, const void * seed, void * out );

void curlup_test  ( const void * key, int len, const void *state, void * out );

void curlup64_test  ( const void * key, int len, const void *state, void * out );

void Frost_with_state  ( const void * key, int len, const void *state, void * out );

void wisp64_with_state  ( const void * key, int len, const void *state, void * out );

void snout64_test ( const void * key, int len, const void *state, void * out );

void tern64_test ( const void * key, int len, const void *state, void * out );

void puff64_test ( const void * key, int len, const void *state, void * out );

#include "mx3.h"

void mx3_hash_test ( const void *input, int len, const void *seed, void *out );

#include "murk.h"

void murk_hash_test ( const void *input, int len, const void *seed, void *out );