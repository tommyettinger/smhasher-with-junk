#include "Hashes.h"
#include "Random.h"

#include <stdlib.h>
//#include <stdint.h>
#include <assert.h>
//#include <emmintrin.h>
#include <x86intrin.h>

// ----------------------------------------------------------------------------
//fake / bad hashes

void
BadHash_with_state(const void *key, int len, const void *state, void *out)
{
  uint32_t	  h = *((uint32_t *)state);
  const uint8_t  *data = (const uint8_t *)key;

  for (int i = 0; i < len; i++) {
    h ^= h >> 3;
    h ^= h << 5;
    h ^= data[i];
  }

  *(uint32_t *) out = h;
}

uint32_t last0= 0xc6efa8b9;
uint32_t last= last0;
void
BadHashSiren_with_state(const void *key, int len, const void *state, void *out)
{
    *(uint32_t *) out = last;
    last = ((last << 1) | (last >> 31));
}

void
BadHashSiren_seed_state(int, const void *, void *)
{
    last= last0;
}

void
DoNothingHash_with_state(const void *, int, const void *, void *)
{
}

void
DoNothingHash_seed_state(int, const void *, void *)
{
}

void
NoopOAATReadHash_with_state(const void *key, int len, const void *state, void *out)
{
    volatile uint8_t c;
    const uint8_t *ptr = (uint8_t *)key;

    for(int i=0; i < len; i++)
    {
        c= ptr[i];
    }
}

//-----------------------------------------------------------------------------
//One - byte - at - a - time hash based on Murmur 's mix

uint32_t MurmurOAAT(const void *key, int len, uint32_t seed)
{
  const uint8_t  *data = (const uint8_t *)key;
  uint32_t	  h = seed;

  for (int i = 0; i < len; i++) {
    h ^= data[i];
    h *= 0x5bd1e995;
    h ^= h >> 15;
  }

  return h;
}

void
MurmurOAAT_with_state_test(const void *key, int len, const void *state, void *out)
{
  *(uint32_t *) out = MurmurOAAT(key, len, *((uint32_t*)state));
}
//----------------------------------------------------------------------------

void
FNV32a_with_state_test(const void *key, int len, const void *state, void *out)
{
  uint32_t  h = *((uint32_t *)state);
  const uint8_t  *data = (const uint8_t *)key;

  h ^= BIG_CONSTANT(2166136261);

  for (int i = 0; i < len; i++) {
    h ^= data[i];
    h *= 16777619;
  }

  *(uint32_t *) out = h;
}


void
FNV32a_YoshimitsuTRIAD_with_state_test(const void *key, int len, const void *state, void *out)
{
  const uint8_t  *p = (const uint8_t *)key;
  const uint32_t  PRIME = 709607;
  uint32_t	  hash32A = *((uint32_t*)state) ^ BIG_CONSTANT(2166136261);
  uint32_t	  hash32B = BIG_CONSTANT(2166136261) + len;
  uint32_t	  hash32C = BIG_CONSTANT(2166136261);

  for (; len >= 3 * 2 * sizeof(uint32_t); len -= 3 * 2 * sizeof(uint32_t), p += 3 * 2 * sizeof(uint32_t)) {
    hash32A = (hash32A ^ (ROTL32(*(uint32_t *) (p + 0), 5)  ^ *(uint32_t *) (p + 4)))  * PRIME;
    hash32B = (hash32B ^ (ROTL32(*(uint32_t *) (p + 8), 5)  ^ *(uint32_t *) (p + 12))) * PRIME;
    hash32C = (hash32C ^ (ROTL32(*(uint32_t *) (p + 16), 5) ^ *(uint32_t *) (p + 20))) * PRIME;
  }
  if (p != key) {
    hash32A = (hash32A ^ ROTL32(hash32C, 5)) * PRIME;
  }
  //Cases 0. .31
  if (len & 4 * sizeof(uint32_t)) {
    hash32A = (hash32A ^ (ROTL32(*(uint32_t *) (p + 0), 5) ^ *(uint32_t *) (p + 4))) * PRIME;
    hash32B = (hash32B ^ (ROTL32(*(uint32_t *) (p + 8), 5) ^ *(uint32_t *) (p + 12))) * PRIME;
    p += 8 * sizeof(uint16_t);
  }
  //Cases 0. .15
  if (len & 2 * sizeof(uint32_t)) {
    hash32A = (hash32A ^ *(uint32_t *) (p + 0)) * PRIME;
    hash32B = (hash32B ^ *(uint32_t *) (p + 4)) * PRIME;
    p += 4 * sizeof(uint16_t);
  }
  //Cases:0. .7
  if (len & sizeof(uint32_t)) {
    hash32A = (hash32A ^ *(uint16_t *) (p + 0)) * PRIME;
    hash32B = (hash32B ^ *(uint16_t *) (p + 2)) * PRIME;
    p += 2 * sizeof(uint16_t);
  }
  //Cases:0. .3
  if (len & sizeof(uint16_t)) {
    hash32A = (hash32A ^ *(uint16_t *) p) * PRIME;
    p += sizeof(uint16_t);
  }
  if (len & 1)
    hash32A = (hash32A ^ *p) * PRIME;

  hash32A = (hash32A ^ ROTL32(hash32B, 5)) * PRIME;
  *(uint32_t *) out = hash32A ^ (hash32A >> 16);
}

void
FNV64a_with_state_test(const void *key, int len, const void *state, void *out)
{
  uint64_t	  h = *((uint64_t*) state);
  const uint8_t  *data = (const uint8_t *)key;

  h ^= BIG_CONSTANT(0xcbf29ce484222325);

  for (int i = 0; i < len; i++) {
    h ^= data[i];
    h *= 0x100000001b3ULL;
  }

  *(uint64_t *) out = h;
}

void
FNV32a_NH_with_state_test(const void *key, int len, const void *state, void *out)
{
//  const uint8_t  *p = (const uint8_t *)key;
//  //const uint32_t  PRIME = 0xfb0a14adUL;
//
//  const uint32_t  c0 = 0xecac1273UL, c1 = 0x80d953bdUL, c2 = 0xac7d0731UL,
//                  c3 = 0xc546137bUL, c4 = 0x947076b5UL, c5 = 0xf403781fUL;
//
//  //const uint64_t c0 = 0xa0761d6478bd642full, c1 = 0xe7037ed1a0b428dbull, c2 = 0x8ebc6af09c88c6e3ull;
//  //const uint64_t c3 = 0x589965cc75374cc3ull, c4 = 0x1d8e4e27c47d124full, c5 = 0xeb44accab455d165ull;
//
//  uint32_t    seed = *((uint32_t*)state);
//  seed ^= ROTL32(seed, 11) ^  ROTL32(seed, 21);
//  uint32_t	  hash32A = seed * ((uint32_t)len - c3);
//  seed ^= ROTL32(seed, 12) ^  ROTL32(seed, 20);
//  uint32_t	  hash32B = (c5 + (uint32_t)len) * seed;
//  seed ^= ROTL32(seed, 13) ^  ROTL32(seed, 19);
//  uint32_t	  hash32C = (c1 - (uint32_t)len) * seed;
//
//  for (; len >= 3 * 2 * sizeof(uint32_t); len -= 3 * 2 * sizeof(uint32_t), p += 3 * 2 * sizeof(uint32_t)) {
//    hash32A += (hash32A ^ c0 ^ *(uint32_t *) (p + 0 )) * (hash32A ^ c1 ^ *(uint32_t *) (p + 4 ));
//    hash32B += (hash32B ^ c2 ^ *(uint32_t *) (p + 8 )) * (hash32B ^ c3 ^ *(uint32_t *) (p + 12));
//    hash32C += (hash32C ^ c4 ^ *(uint32_t *) (p + 16)) * (hash32C ^ c5 ^ *(uint32_t *) (p + 20));
//  }
//  if (p != key) {
//    //hash32B ^= c4 * (hash32A += hash32C ^ ROTL64(hash32C, 47) ^ ROTL64(hash32C, 19));
//    hash32A += hash32C;
//  }
//  //Cases 0. .31
//  if (len & 4 * sizeof(uint32_t)) {
//    hash32A += ((c2 ^ *(uint32_t *) (p + 0 )) * (c3 ^ *(uint32_t *) (p + 4 )));
//    hash32B += ((c0 ^ *(uint32_t *) (p + 8 )) * (c1 ^ *(uint32_t *) (p + 12)));
//    p += 8 * sizeof(uint16_t);
//  }
//  //Cases 0. .15
//  if (len & 2 * sizeof(uint32_t)) {
//    hash32A += (c5 ^ *(uint32_t *) (p + 0)) * (c4 ^ *(uint32_t *) (p + 4));
//    //hash32A += (c2 ^ *(uint32_t *) (p + 0)) * (hash32A ^ c3);
//    //hash32A = ROTL32(hash32A, 11);
//    //hash32B += (c4 ^ *(uint32_t *) (p + 4)) * (hash32B ^ c5);
//    //hash32B = ROTL32(hash32B, 11);
//    p += 4 * sizeof(uint16_t);
//  }
//  //Cases:0. .7
//  if (len & sizeof(uint32_t)) {
//    hash32A += (c3 ^ *(uint16_t *) (p + 0)) * (c1 ^ *(uint16_t *) (p + 2));
//    //hash32A = ROTL32(hash32A, 11);
//    //hash32B += (c1 ^ *(uint16_t *) (p + 2)) * (hash32B ^ c0);
//    //hash32B = ROTL32(hash32B, 11);
//    p += 2 * sizeof(uint16_t);
//  }
//  //Cases:0. .3
//  if (len & sizeof(uint16_t)) {
//    hash32A += (c4 ^ *(uint16_t *) (p + 0)) * (c5);
//    //hash32A = ROTL32(hash32A, 11);
//    p += sizeof(uint16_t);
//  }
//  if (len & 1) {
//    hash32A += (c1 ^ *(p + 0)) * (c0);
//    //hash32A = ROTL32(hash32A, 11);
//  }
//  hash32A += hash32B;//c5 * (hash32B ^ ROTL64(hash32B, 41) ^ ROTL64(hash32B, 23));
//  hash32A = (hash32A ^ ROTL32(hash32A, 13) ^ ROTL32(hash32A, 37)) * c5;
//  *(uint32_t *) out = (hash32A ^ (hash32A >> 21) ^ (hash32A >> 13) ^ (hash32A >> 17));
  const uint8_t  *p = (const uint8_t *)key;
  const uint32_t  PRIME = 0x947076b5UL;//709607;
  uint32_t    seed    = *((uint32_t*)state);
  uint32_t	  hash32A = seed ^ 0xecac1273UL;
  uint32_t	  hash32B = 0x80d953bdUL * len + seed;
  uint32_t	  hash32C = 0xac7d0731UL * seed + len;

  for (; len >= 3 * 2 * sizeof(uint32_t); len -= 3 * 2 * sizeof(uint32_t), p += 3 * 2 * sizeof(uint32_t)) {
    hash32A = (hash32A ^ (ROTL32(*(uint32_t *) (p + 0), 5)  ^ *(uint32_t *) (p + 4)))  * PRIME;
    hash32B = (hash32B ^ (ROTL32(*(uint32_t *) (p + 8), 5)  ^ *(uint32_t *) (p + 12))) * PRIME;
    hash32C = (hash32C ^ (ROTL32(*(uint32_t *) (p + 16), 5) ^ *(uint32_t *) (p + 20))) * PRIME;
  }
  if (p != key) {
    hash32A = (hash32A ^ ROTL32(hash32C, 5)) * PRIME;
  }
  //Cases 0. .31
  if (len & 4 * sizeof(uint32_t)) {
    hash32A = (hash32A ^ (ROTL32(*(uint32_t *) (p + 0), 5) ^ *(uint32_t *) (p + 4))) * PRIME;
    hash32B = (hash32B ^ (ROTL32(*(uint32_t *) (p + 8), 5) ^ *(uint32_t *) (p + 12))) * PRIME;
    p += 8 * sizeof(uint16_t);
  }
  //Cases 0. .15
  if (len & 2 * sizeof(uint32_t)) {
    hash32A = (hash32A ^ *(uint32_t *) (p + 0)) * PRIME;
    hash32B = (hash32B ^ *(uint32_t *) (p + 4)) * PRIME;
    p += 4 * sizeof(uint16_t);
  }
  //Cases:0. .7
  if (len & sizeof(uint32_t)) {
    hash32A = (hash32A ^ *(uint16_t *) (p + 0)) * PRIME;
    hash32B = (hash32B ^ *(uint16_t *) (p + 2)) * PRIME;
    p += 2 * sizeof(uint16_t);
  }
  //Cases:0. .3
  if (len & sizeof(uint16_t)) {
    hash32A = (hash32A ^ *(uint16_t *) p) * PRIME;
    p += sizeof(uint16_t);
  }
  if (len & 1)
    hash32A = (hash32A ^ *p) * PRIME;

  hash32A = (hash32A ^ ROTL32(hash32B, 5)) * PRIME;
  *(uint32_t *) out = hash32A ^ (hash32A >> 16);

}


//-----------------------------------------------------------------------------

uint32_t x17(const void *key, int len, uint32_t h)
{
  const uint8_t  *data = (const uint8_t *)key;

  for (int i = 0; i < len; ++i) {
    h = 17 * h + (data[i] - ' ');
  }

  return h ^ (h >> 16);
}

void
x17_test(const void *key, int len, const void *state, void *out)
{
  *(uint32_t *) out = x17(key, len, *((uint32_t*)state));
}

//-----------------------------------------------------------------------------

//also used in perl5 as djb2
void
Bernstein_with_state(const void *key, int len, const void *state, void *out)
{
  const uint8_t  *data = (const uint8_t *)key;
  uint32_t hash = *((uint32_t *)state);

  for (int i = 0; i < len; ++i) {
    //hash = ((hash << 5) + hash) + data[i];
    hash = 33 * hash + data[i];
  }

  *(uint32_t *) out = hash;
}

//as used in perl5
void
sdbm(const void *key, int len, const void *state, void *out)
{
  unsigned char  *str = (unsigned char *)key;
  const unsigned char *const end = (const unsigned char *)str + len;
  uint32_t hash= *((uint32_t*)state) + len;
  while (str < end) {
    hash = (hash << 6) + (hash << 16) - hash + *str++;
  }
  *(uint32_t *) out = hash;
}

//as used in perl5 as one_at_a_time_hard
void
JenkinsOAATH_with_state(const void *key, int len, const void *state, void *out)
{
  unsigned char  *str = (unsigned char *)key;
  const unsigned char *const end = (const unsigned char *)str + len;
  unsigned char  *seed = (unsigned char *)state;
  uint32_t       hash = *((uint32_t *)state) + len;
  //unsigned char seed[8];
  //note that perl5 adds part of the seed to the end of key, to frustrate
  //key extension attacks, and because the last byte or so of the key is
  //not well mixed into the final hash. Mixing four additional bytes
  //ensures that the real key has had an opportunity to affect every bit
  //of the output, and that the seed is well mixed before we return the
  //hash value. Older versions of this hash had a trivial seed discovery
  //attack, which the new one is robust to. Regardless, this is not a good
  //choice of hash function these days.
  while (str < end) {
    hash += (hash << 10);
    hash ^= (hash >> 6);
    hash += *str++;
  }

  hash += (hash << 10);
  hash ^= (hash >> 6);
  hash += seed[4];

  hash += (hash << 10);
  hash ^= (hash >> 6);
  hash += seed[5];

  hash += (hash << 10);
  hash ^= (hash >> 6);
  hash += seed[6];

  hash += (hash << 10);
  hash ^= (hash >> 6);
  hash += seed[7];

  hash += (hash << 10);
  hash ^= (hash >> 6);

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash = hash + (hash << 15);
  *(uint32_t *) out = hash;
}

//as used in perl5 until 5.17(one_at_a_time_old)
void JenkinsOAAT_with_state(const void *key, int len, const void *seed, void *out)
{
  unsigned char  *str = (unsigned char *)key;
  const unsigned char *const end = (const unsigned char *)str + len;
  uint32_t hash = *((uint32_t *)seed);
  while (str < end) {
    hash += *str++;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash = hash + (hash << 15);
  *(uint32_t *) out = hash;
}

void JenkinsOAAT(const void *key, int len, uint32_t hash, void *out)
{
    JenkinsOAAT_with_state(key, len, &hash, out);
}

//------------------------------------------------
// One of a smallest non-multiplicative One-At-a-Time function
// that passes whole SMHasher. (The old one anyway, it fails test
// on the new one. */
// Author: Sokolov Yura aka funny-falcon <funny.falcon@gmail.com>
void GoodOAAT(const void *key, int len, const void *state, void *out) {
#define grol(x,n) (((x)<<(n))|((x)>>(32-(n))))
#define gror(x,n) (((x)>>(n))|((x)<<(32-(n))))
  unsigned char  *str = (unsigned char *)key;
  const unsigned char *const end = (const unsigned char *)str + len;
  uint32_t seed= *((uint32_t*)state);
  uint32_t h1 = seed ^ 0x3b00;
  uint32_t h2 = grol(seed, 15);
  for (;str != end; str++) {
    h1 += str[0];
    h1 += h1 << 3; // h1 *= 9
    h2 += h1;
    // the rest could be as in MicroOAAT: h1 = grol(h1, 7)
    // but clang doesn't generate ROTL instruction then.
    h2 = grol(h2, 7);
    h2 += h2 << 2; // h2 *= 5
  }
  h1 ^= h2;
  /* now h1 passes all collision checks,
   * so it is suitable for hash-tables with prime numbers. */
  h1 += grol(h2, 14);
  h2 ^= h1; h2 += gror(h1, 6);
  h1 ^= h2; h1 += grol(h2, 5);
  h2 ^= h1; h2 += gror(h1, 8);
  *(uint32_t *) out = h2;
#undef grol
#undef gror
}

// MicroOAAT suitable for hash-tables using prime numbers.
// It passes all collision checks.
// Author: Sokolov Yura aka funny-falcon <funny.falcon@gmail.com>
void MicroOAAT(const void *key, int len, const void *state, void *out) {
#define grol(x,n) (((x)<<(n))|((x)>>(32-(n))))
#define gror(x,n) (((x)>>(n))|((x)<<(32-(n))))
  unsigned char  *str = (unsigned char *)key;
  const unsigned char *const end = (const unsigned char *)str + len;
  uint32_t seed = *((uint32_t*)state);
  uint32_t h1 = seed ^ 0x3b00;
  uint32_t h2 = grol(seed, 15);
  for (;str != end; str++) {
    h1 += str[0];
    h1 += h1 << 3; // h1 *= 9
    h2 -= h1;
    // unfortunately, clang produces bad code here,
    // cause it doesn't generate rotl instruction.
    h1 = grol(h1, 7);
  }
  *(uint32_t *) out = h1 ^ h2;
#undef grol
#undef gror
}

//-----------------------------------------------------------------------------
//Crap8 hash from http://www.team5150.com / ~andrew / noncryptohashzoo / Crap8.html

uint32_t Crap8(const uint8_t * key, uint32_t len, uint32_t seed)
{
#define c8fold( a, b, y, z ) { p = (uint32_t)(a) * (uint64_t)(b); y ^= (uint32_t)p; z ^= (uint32_t)(p >> 32); }
#define c8mix( in ) { h *= m; c8fold( in, m, k, h ); }

  const uint32_t  m = 0x83d2e73b, n = 0x97e1cc59, *key4 = (const uint32_t *)key;
  uint32_t	  h = len + seed, k = n + len;
  uint64_t	  p;

  while (len >= 8) {
    c8mix(key4[0]) c8mix(key4[1]) key4 += 2;
    len -= 8;
  }
  if (len >= 4) {
    c8mix(key4[0]) key4 += 1;
    len -= 4;
  }
  if (len) {
    c8mix(key4[0] & ((1 << (len * 8)) - 1))
  }
  c8fold(h ^ k, n, k, k)
    return k;
}

void
Crap8_test(const void *key, int len, const void *state, void *out)
{
  *(uint32_t *) out = Crap8((const uint8_t *)key, len, *((uint32_t *)state));
}

extern		"C" {
#ifdef __SSE2__
  void		  hasshe2 (const void *input, int len, const void *state, void *out);
#endif
#if defined(__SSE4_2__) && defined(__x86_64__)
  uint32_t	  crc32c_hw(const void *input, int len, uint32_t seed);
  uint32_t	  crc32c(const void *input, int len, uint32_t seed);
  uint64_t	  crc64c_hw(const void *input, int len, uint64_t seed);
#endif
}

#ifdef __SSE2__
void
hasshe2_test(const void *input, int len, const void *state, void *out)
{
  hasshe2(input, len, state, out);
}
#endif

#if defined(__SSE4_2__) && (defined(__i686__) || defined(_M_IX86) || defined(__x86_64__))
/* Compute CRC-32C using the Intel hardware instruction.
   TODO: arm8
 */
void
crc32c_hw_with_state_test(const void *input, int len, const void *state, void *out)
{
  if (!len) {
    *(uint32_t *) out = 0;
    return;
  }
  *(uint32_t *) out = crc32c_hw(input, len, *((uint32_t*)state));
}

/* Faster Adler SSE4.2 crc32 in HW */
void
crc32c_hw1_with_state_test(const void *input, int len, const void *state, void *out)
{
  if (!len) {
    *(uint32_t *) out = 0;
    return;
  }
  *(uint32_t *) out = crc32c(input, len, *((uint32_t*)state));
}

#if defined(__SSE4_2__) && defined(__x86_64__)
/* Compute CRC-64C using the Intel hardware instruction. */
void
crc64c_hw_with_state_test(const void *input, int len, const void *state, void *out)
{
  if (!len) {
    *(uint64_t *) out = 0;
    return;
  }
  *(uint64_t *) out = crc64c_hw(input, len, *((uint64_t*)state));
}

#endif
#endif

#include "siphash.h"
/* https://github.com/floodyberry/siphash */

void
siphash_seed_state_test(int seedbits, const void *seed, void *state)
{
    siphash_seed_state((unsigned char *)seed, (unsigned char *)state);
}

void
siphash_with_state_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = siphash_with_state((const unsigned char *)seed, (const unsigned char *)input, (size_t) len);
}

void
siphash13_with_state_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = siphash13_with_state((const unsigned char *)seed, (const unsigned char *)input, (size_t) len);
}

void
halfsiphash_seed_state_test(int seed_bits, const void * seed, void *state)
{
    halfsiphash_seed_state((const unsigned char *)seed,(unsigned char *)state);
}

void
halfsiphash_with_state_test(const void *key, int len, const void * state, void *out)
{
  *(uint32_t *) out = halfsiphash_with_state((const unsigned char *)state, (const unsigned char *)key, (size_t) len);
}


/* https://github.com/gamozolabs/falkhash */
#if defined(__SSE4_2__) && defined(__x86_64__)
extern "C" {
  uint64_t falkhash_test(uint8_t *data, uint64_t len, uint64_t seed, void *out);
}

void
falkhash_with_state_test_cxx(const void *input, int len, const void *seed, void *out)
{
  uint64_t hash[2] = {0ULL, 0ULL};
  falkhash_test((uint8_t *)input, (uint64_t)len, *((uint64_t *)seed), hash);
  *(uint64_t *) out = hash[0];
}
#endif

void lua_v53_string_hash_oaat (const void *key, int len, const void *seed, void *out) {
  uint8_t *str= (uint8_t *)key+len-1;
  unsigned int h = *((uint32_t*)seed) ^ ((unsigned int)len);
  for (; key <= str ; str--)
    h ^= ((h<<5) + (h>>2) + *str);
  *((uint32_t*)out)=h;
}

void lua_v53_string_hash (const void *key, int len, const void *seed, void *out) {
  uint8_t *str= (uint8_t *)key;
  unsigned int h = *((uint32_t*)seed) ^ ((unsigned int)len);
  size_t step = (len >> 5) + 1;
  for (; len >= step; len -= step)
    h ^= ((h<<5) + (h>>2) + str[len-1]);
  *((uint32_t*)out)=h;
}

#ifdef HAVE_FANOM_HASH
// Fast non-multiplicative hash function.
// https://github.com/funny-falcon/fanom_hash
#include "fanom_hash.h"
void
fanom_hash64_seed_state_test(int seed_bits, const void * seed, void *state)
{
    memcpy(state,seed,seed_bits/8);
}

void
fanom_hash64_with_state_test(const void *key, int len, const void * state, void *out)
{
    uint64_t *s64 = (uint64_t *)state;
  *(uint64_t *) out = fanom64_string_hash2(key, len, s64[0], s64[1] ^ 1);
}
#endif


void
wyhash_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = wyhash((const unsigned char *)input, (size_t) len, *((const uint64_t *)seed));
}

void
wyhash3_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = wyhash3((const unsigned char *)input, (size_t) len, *((const uint64_t *)seed));
}

void
waterhash_test(const void *input, int len, const void *seed, void *out)
{
  *(uint32_t *) out = waterhash((const unsigned char *)input, (uint32_t) len, *((const uint64_t *)seed));
}

void
wheathash_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = wheathash((const unsigned char *)input, (uint64_t) len, *((const uint64_t *)seed));
}

void
wheatoaathash_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = wheatoaathash((const unsigned char *)input, (uint64_t) len, *((const uint64_t *)seed));
}

void
woothash_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = woothash((const unsigned char *)input, (uint64_t) len, *((const uint64_t *)seed));
}

void
wickethash_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = wickethash((const unsigned char *)input, (uint64_t) len, *((const uint64_t *)seed));
}
/*
            long result = 0x9E3779B97F4A7C94L, a = 0x632BE59BD9B4E019L;
            final int len = data.length;
            for (int i = 0; i < len; i++) {
                result += (a ^= 0x8329C6EB9E6AD3E3L * data[i]);
            }
            return result * (a | 1L) ^ (result << 37 | result >>> 27);

*/


void
puller_test(const void *key, int len, const void *state, void *out)
{
	const uint8_t *data = (const uint8_t *)key;
	const int nblocks = len / 8;
	const uint64_t * blocks = (const uint64_t *)(data + nblocks * 8);
	uint64_t a = *((uint64_t *)state) ^ 0xDB4F0B9175AE2165UL, d = 0x632BE59BD9B4E019UL;// , a = 0x632BE59BD9B4E019UL * len;
	for (int i = -nblocks; i; i++) {
		//a += (read64u(blocks, i)) * 0x880355F21E6D1965UL; //0x2127599BF4325C37UL
		//result ^= a ^ a >> 47 ^ a >> 29 ^ a >> 19; //fast-hash 0x880355F21E6D1965 // also tried 0x8329C6EB9E6AD3E3UL for multiplier
		//a ^= (result = (blocks[i] ^ 0x9E3779B97F4A7C15UL) * 0x8329C6EB9E6AD3E3UL + ROTL64(result, 23));
		//result += (blocks[i] ^ 0x880355F21E6D1965UL) * (a += 0x9E3779B97F4A7C16UL);
		//result = ROTL64(result, 41);

		//result += (a ^= blocks[i] * 0x880355F21E6D1965UL);

		//a = (a * 0xAC564B05UL + blocks[i]) * 0x818102004182A025UL;
		//a = ROTL64(a, 29);
		//((state = (state << 29 | state >> > 35) * 0xAC564B05L) * 0x818102004182A025L);
    //a += blocks[i] * (d += 0x9E3779B97F4A7C16UL);
    //a = (a ^ a >> 31) * 0xDB4F0B9175AE2165UL;
    //a ^= a >> 28;
    //(d = __rolq(d, 29u) * 0xAC564B05ULL) * 0x818102004182A025ULL
    //a += (d = __rolq(d, 29u) + 0xDB4F0B9175AE2165UL) * blocks[i] + 0x8329C6EB9E6AD3E3UL;
    //a += (d = ((d ^ __rolq(d, 23u) ^ __rolq(d, 47u)) + blocks[i]) * 0xDB4F0B9175AE2165UL);
    //a = __rolq(a, 25u);
    //a += (d ^= __rolq(d, 50u) ^ __rolq(d, 25u));
//    a += (d ^= __rolq(d, 50u) ^ __rolq(d, 25u));

//    a += __rolq(a, 25) + (d += (blocks[i] + 0xDB4F0B9175AE2165UL) * 0x818102004182A025ULL);
    //a += (d += blocks[i]) * 0x818102004182A025ULL + __rolq(a, 17);// ^ (d += 0x9E3779B97F4A7C15UL));
    
    //works
    a += blocks[i] * 0x818102004182A025ULL ^ __rolq(a, 17);// ^ (d += 0x9E3779B97F4A7C15UL));
    
    //a ^= blocks[i] * (d += 0x9E3779B97F4A7C16UL) + __rolq(a, 17);
    //a += blocks[i] * (d += 0x9E3779B97F4A7C16UL) ^ __rolq(a, 17);
    //a += a >> 29;
    //a += __rolq(a, 17);
    
    //a += blocks[i] * (d += 0x9E3779B97F4A7C16UL);
    //a += a >> 13;
    //a = __rolq(a, 17);
    //a += __rolq(a, 17);
    //a = __rolq(a, 25) + blocks[i] * 0x818102004182A025UL + 0xDB4F0B9175AE2165UL;

	}
	const uint8_t * tail = (const uint8_t*)(data + nblocks * 8);
	switch (len & 7)
	{
	case 7: a += (tail[6] ^ UINT64_C(0x55076507395F3485)) * UINT64_C(0x8329C6EB9E6AD3E3) ^ a >> 27;
	case 6: a += (tail[5] ^ UINT64_C(0xE2CE02C1DBEA2845)) * UINT64_C(0xD7EF17178C46ABE3) ^ a >> 26;
	case 5: a += (tail[4] ^ UINT64_C(0x6096A6800704E7C5)) * UINT64_C(0x8329C6EB9E6AD3E3) ^ a >> 25;
	case 4: a += (tail[3] ^ UINT64_C(0xB2F97045254154A5)) * UINT64_C(0xD7EF17178C46ABE3) ^ a >> 24;
	case 3: a += (tail[2] ^ UINT64_C(0x414C6E02D8B72D05)) * UINT64_C(0x8329C6EB9E6AD3E3) ^ a >> 23;
	case 2: a += (tail[1] ^ UINT64_C(0xF6FDE799B4A0DBA5)) * UINT64_C(0xD7EF17178C46ABE3) ^ a >> 22;
	case 1: a += (tail[0] ^ UINT64_C(0x735664783B1136B5)) * UINT64_C(0x8329C6EB9E6AD3E3) ^ a >> 21;
		a ^= (a + UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xC6BC279692B5CC83);
	};
  a = (a ^ __rolq(a, 41) ^ __rolq(a, 17)) * 0x369DEA0F31A53F85UL;
  a = (a ^ a >> 31) * 0xDB4F0B9175AE2165UL;
  a ^= a >> 28;

	*(uint64_t *)out = a;
}
const uint32_t _gwoemul0 = 0xa0761d65u, _gwoemul1 = 0xe7037ed1u, _gwoemul2 = 0x8ebc6af1u, _gwoemul3 = 0x589965cdu;

void
Gwoemul_with_state(const void *key, int len, const void *state, void *out)
{
//					const uint64_t s = (stateA += 0xC6BC279692B5C323UL);
//          const uint64_t z = (s ^ s >> 31) * (stateB += 0x9E3779B97F4A7C16UL);
//					return z ^ z >> 26 ^ z >> 6;

  // 0xDB4F0B9175AE2165ULL 0xC6BC279692B5C323ULL
     //h ^= __rolq(((s += 0xDB4F0B9175AE2165ULL) + blocks[i]) * (m += 0x9479D2858AF899E6ULL), m & 63);
	const uint8_t *data = (const uint8_t *)key;
  const int nblocks = len / 4;
	const uint32_t * blocks = (const uint32_t *)(data + nblocks * 4);
  uint64_t s = *((uint64_t *)state);
  uint64_t h = len + s ^ 0xC6BC279692B5C323ULL;
  uint64_t m = ~(s << 1);
	for (int i = -nblocks; i; i++) {
     h ^= __rolq(blocks[i] * m, (m += 0x9479D2858AF899E6ULL) >> 58);
  }
	const uint8_t * tail = (const uint8_t*)(data + nblocks * 4);
  switch(len & 3)
  {
    case 3: h ^= __rolq(tail[2] * m, (m += 0x9479D2858AF899E6ULL) >> 58);
    case 2: h ^= __rolq(tail[1] * m, (m += 0x9479D2858AF899E6ULL) >> 58);
    case 1: h ^= __rolq(tail[0] * m, (m +  0x9479D2858AF899E6ULL) >> 58);
  }
  h ^= h >> 27;
  h *= 0x3C79AC492BA7B653ULL;
  h ^= h >> 33;
  h *= 0x1C69B3F74AC4AE35ULL;
  h ^= h >> 27;
  *(uint64_t *) out = h;
}


uint64_t WobbleOAAT(const void *key, int len, uint64_t seed)
{
  const uint8_t  *data = (const uint8_t *)key;
  uint64_t	  h = seed + len, m = ~(seed << 1);

  for (int i = 0; i < len; i++) {
    h += data[i] + 0xC6BC279692B5C323ULL;
    h *= (m += 0x9479D2858AF899E6ULL);
    //h ^= h >> 8;
  }

  h ^= h >> 27;
  h *= 0x3C79AC492BA7B653ULL;
  h ^= h >> 33;
  h *= 0x1C69B3F74AC4AE35ULL;
  h ^= h >> 27;
  return h;
}

void
WobbleOAAT_with_state_test(const void *key, int len, const void *state, void *out)
{
  *(uint64_t *) out = WobbleOAAT(key, len, *((uint64_t*)state));
}


void
curlup_test(const void *key, int len, const void *state, void *out)
{
	const uint8_t *data = (const uint8_t *)key;
	const int nblocks = (len / 32) * 8;
	const uint32_t * blocks = (const uint32_t *)(data + nblocks * 4);
	uint64_t a = *((uint64_t *)state);
  a = (a ^ __rolq(a, 41) ^ __rolq(a, 17)) * 0x369DEA0F31A53F85UL;
//  a = (a ^ a >> 31) * 0xDB4F0B9175AE2165UL;
//  a ^= a >> 28;

  a = (a ^ len) * 0x9E3779B97F4A7C15UL;
  a ^= a >> 28;
	for (int i = -nblocks; i; i+=8) {
    a = 0xEBEDEED9D803C815UL * a
      + 0xD96EB1A810CAAF5FUL * blocks[i]
      + 0xC862B36DAF790DD5UL * blocks[i + 1]
      + 0xB8ACD90C142FE10BUL * blocks[i + 2]
      + 0xAA324F90DED86B69UL * blocks[i + 3]
      + 0x9CDA5E693FEA10AFUL * blocks[i + 4]
      + 0x908E3D2C82567A73UL * blocks[i + 5]
      + 0x8538ECB5BD456EA3UL * blocks[i + 6]
      + 0xD1B54A32D192ED03UL * blocks[i + 7];
	}
  //a ^= a >> 37;
  const int nflank = (len / 4) - nblocks;
  for (int i = 0; i < nflank; i++) {
    a = 0xCC62FCEB9202FAADUL * (a + blocks[i]);
  }

//  a *= 0x94D049BB133111EBL;
  //a *= 0xCB9C59B3F9F87D4DUL;
	const uint8_t * tail = (const uint8_t*)(data + (nblocks + nflank) * 4);
	switch (len & 3)
	{
	case 3: a = (tail[2] + a) * 0xD1342543DE82EF95UL;
	case 2: a = (tail[1] + a) * 0xF7C2EBC08F67F2B5UL;
	case 1: a = (tail[0] + a) * 0xCB9C59B3F9F87D4DUL;
    a = (a ^ 0x9E3779B97F4A7C15UL) * 0xC6BC279692B5CC83UL;
	}
//  a = (a ^ a >> 27) * 0x3C79AC492BA7B653L;
//  a ^= a >> 25;
//  a = (a ^ a >> 33) * 0x1C69B3F74AC4AE35L;
//  a ^= a >> 27;

//  a = (a ^ __rolq(a, 41) ^ __rolq(a, 17)) * 0x369DEA0F31A53F85UL;
  a = (a ^ a >> 31) * 0xDB4F0B9175AE2165UL;
  a ^= a >> 28;
	*(uint64_t *)out = a;
}
uint64_t
roller(uint64_t a)
{
  //return a ^ __rolq(a, 23) ^ __rolq(a, 47);
  return a ^ a >> 23 ^ a >> 47;
}

void
curlup64_test(const void *key, int len, const void *state, void *out)
{
	const uint8_t *data = (const uint8_t *)key;
	const int nblocks = (len / 64) * 8;
	const uint64_t * blocks = (const uint64_t *)(data + nblocks * 8);
	uint64_t a = *((uint64_t *)state) ^ len, m = 0xDB4F0B9175AE2165UL;
//  a = (a ^ __rolq(a, 41) ^ __rolq(a, 17)) * 0x369DEA0F31A53F85UL;

//  a = (a ^ a >> 31) * 0xDB4F0B9175AE2165UL;
//  a ^= a >> 28;
	for (int i = -nblocks; i; i+=8) {
    a = 0xEBEDEED9D803C815UL * roller(a)
      + 0xD96EB1A810CAAF5FUL * roller(blocks[i])
      + 0xC862B36DAF790DD5UL * roller(blocks[i + 1])
      + 0xB8ACD90C142FE10BUL * roller(blocks[i + 2])
      + 0xAA324F90DED86B69UL * roller(blocks[i + 3])
      + 0x9CDA5E693FEA10AFUL * roller(blocks[i + 4])
      + 0x908E3D2C82567A73UL * roller(blocks[i + 5])
      + 0x8538ECB5BD456EA3UL * roller(blocks[i + 6])
      + 0xD1B54A32D192ED03UL * roller(blocks[i + 7])
      ;
    //a ^= __rolq(a, 23) ^ __rolq(a, 47);    
	}
  //a ^= a >> 37;
  const int nflank = (len / 8) - nblocks;
  for (int i = 0; i < nflank; i++) {
    a += roller(blocks[i]) * (m += 0x95B534A1ACCD52DAUL);
  }

//  a *= 0x94D049BB133111EBL;
  //a *= 0xCB9C59B3F9F87D4DUL;
	const uint8_t * tail = (const uint8_t*)(data + (nblocks + nflank) * 8);
	switch (len & 7)
	{
	case 7: a += roller(tail[6]) * 0xC862B36DAF790DD5UL;
	case 6: a += roller(tail[5]) * 0xB8ACD90C142FE10BUL;
	case 5: a += roller(tail[4]) * 0xAA324F90DED86B69UL;
	case 4: a += roller(tail[3]) * 0x9CDA5E693FEA10AFUL;
	case 3: a += roller(tail[2]) * 0x908E3D2C82567A73UL;
	case 2: a += roller(tail[1]) * 0x8538ECB5BD456EA3UL;
	case 1: a += roller(tail[0]) * 0xD1B54A32D192ED03UL;
    a = (a ^ a >> 26 ^ 0x9E3779B97F4A7C15UL) * 0xC6BC279692B5CC83UL;
	}
//  a = (a ^ a >> 27) * 0x3C79AC492BA7B653L;
//  a ^= a >> 25;
//  a = (a ^ a >> 33) * 0x1C69B3F74AC4AE35L;
//  a ^= a >> 27;

//  a = (a ^ __rolq(a, 41) ^ __rolq(a, 17)) * 0x369DEA0F31A53F85UL;

//  a = (a ^ a >> 31) * 0xDB4F0B9175AE2165UL;

//            a *= 0x94D049BB133111EBL;
            a ^= __rolq(a, 41) ^ __rolq(a, 17);
            a *= 0x369DEA0F31A53F85L;
            a ^= a >> 31;
            a *= 0xDB4F0B9175AE2165L;
  a ^= a >> 28;
	*(uint64_t *)out = a;
}
/*
0xEBEDEED9D803C815UL 3 fail
0xD96EB1A810CAAF5FUL 2 fail
0xC862B36DAF790DD5UL
0xB8ACD90C142FE10BUL
0xAA324F90DED86B69UL
0x9CDA5E693FEA10AFUL
0x908E3D2C82567A73UL
0x8538ECB5BD456EA3UL
0xD1B54A32D192ED03UL
*/
void
Frost_with_state(const void *key, int len, const void *state, void *out)
{
//					const uint64_t s = (stateA += 0xC6BC279692B5C323UL);
//          const uint64_t z = (s ^ s >> 31) * (stateB += 0x9E3779B97F4A7C16UL);
//					return z ^ z >> 26 ^ z >> 6;

  // 0xDB4F0B9175AE2165ULL 0xC6BC279692B5C323ULL
     //h ^= __rolq(((s += 0xDB4F0B9175AE2165ULL) + blocks[i]) * (m += 0x9479D2858AF899E6ULL), m & 63);
	const uint8_t *data = (const uint8_t *)key;
  const int nblocks = len / 2;
	const uint16_t * blocks = (const uint16_t *)(data + nblocks * 2);
  uint64_t h = (uint64_t)len + *((uint64_t *)state);
  uint64_t m = 0xC862B36DAF790DD5UL;//0xF7C2EBC08F67F2B5UL;//0x9E3779B97F4A7C15UL;
	for (int i = -nblocks; i; i++) {
    h += (blocks[i] ^ 0xC6BC279692B5C323UL) * (m += 0x95B534A1ACCD52DAUL);
    h = __rolq(h, 27);
  }
  if(len & 1)
  {
    h += (((const uint8_t*)(data + nblocks * 2))[0] ^ 0xC6BC279692B5C323UL) * (m + 0x95B534A1ACCD52DAUL);
    h = __rolq(h, 27);
  }

//  h = (h ^ __rolq(h, 41) ^ __rolq(h, 17)) * 0x369DEA0F31A53F85UL;
//  h = (h ^ h >> 31) * 0xDB4F0B9175AE2165UL;
//  h ^= h >> 28;

  h ^= h >> 27;
  h *= 0x3C79AC492BA7B653UL;
  h ^= h >> 33;
  h *= 0x1C69B3F74AC4AE35UL;
  h ^= h >> 27;
  *(uint64_t *) out = h;
}
void
Frost64_with_state(const void *key, int len, const void *state, void *out)
{
	const uint8_t *data = (const uint8_t *)key;
  const int nblocks = len / 8;
	const uint64_t * blocks = (const uint64_t *)(data + nblocks * 8);
  uint64_t h = *((uint64_t *)state) + (uint64_t)len;// ^ 0x94D049BB133111EBUL;
  uint64_t m = 0xDB4F0B9175AE2165UL; //(m += 0x95B534A1ACCD52DAUL)
	for (int i = -nblocks; i; i++) {
    uint64_t t = blocks[i];
    h += __rolq((t ^ t >> 27) * m, m >> 58) ^ (m += 0x95B534A1ACCD52DAUL);
  }
  const uint8_t * tail = (const uint8_t*)(data + nblocks * 8);

  for(int ln = (len & 7); ln;)
  {
    h += __rolq((0xC6BC279692B5C323UL ^ tail[--ln]) * m, (m += 0x95B534A1ACCD52DAUL) >> 58);
  }

//// Pelican unary hash, in case avalanche needs to be better. Slightly slower.
//  h = (h ^ __rolq(h, 41) ^ __rolq(h, 17)) * 0x369DEA0F31A53F85UL;
//  h = (h ^ h >> 31) * 0xDB4F0B9175AE2165UL;
//  h ^= h >> 28;

  h ^= h >> 27;
  h *= 0x3C79AC492BA7B653UL;
  h ^= h >> 33;
  h *= 0x1C69B3F74AC4AE35UL;
  h ^= h >> 27;
  *(uint64_t *) out = h;
}

void
snout64_test(const void *key, int len, const void *state, void *out)
{
	const uint8_t *data = (const uint8_t *)key;
	const int nblocks = (len / 64) * 4;
	const uint64_t * blocks = (const uint64_t *)(data + nblocks * 8);
  uint64_t m = 0xDB4F0B9175AE2165UL;
	uint64_t a = *((uint64_t *)state) ^ len, b = ~a, c = a ^ m, d = b ^ m;

	for (int i = -nblocks; i; i+=4) {
      uint64_t fa = a ^ blocks[i];
      uint64_t fb = b - blocks[i + 1];
      uint64_t fc = c ^ blocks[i + 2];
      uint64_t fd = d + blocks[i + 3];
      a = (fb ^ __rolq(fd, 25)) * 0xF1357AEA2E62A9C5UL;
	    b = __rolq(fa, 44) * 0xF1357AEA2E62A9C5UL + fc;
	    c = (__rolq(fb, 37) + fa) * 0x9E3779B97F4A7C15UL;
	    d = fa ^ __rolq(fc, 50) * 0x9E3779B97F4A7C15UL;
	}
  d ^= c += b ^= a -= m;
  const int nflank = (len / 8) - nblocks;
  for (int i = 0; i < nflank; i++) {
      uint64_t blk = blocks[i];
      uint64_t fa = a + blk;
      uint64_t fb = b ^ blk;
      uint64_t fc = c - blk;
      uint64_t fd = d ^ blk;
      a = (fb ^ __rolq(fd, 25)) * 0xF1357AEA2E62A9C5UL;
	    b = __rolq(fa, 44) * 0xF1357AEA2E62A9C5UL + fc;
	    c = (__rolq(fb, 37) + fa) * 0x9E3779B97F4A7C15UL;
	    d = fa ^ __rolq(fc, 50) * 0x9E3779B97F4A7C15UL;
  }
  d ^= c + (b ^ a - m);
//  a *= 0x94D049BB133111EBL;
  //a *= 0xCB9C59B3F9F87D4DUL;
	const uint8_t * tail = (const uint8_t*)(data + (nblocks + nflank) * 8);
	switch (len & 7)
	{
	case 7: d += (tail[6] ^ UINT64_C(0x55076507395F3485)) * UINT64_C(0x8329C6EB9E6AD3E3) ^ __rolq(d, 27);
	case 6: d += (tail[5] ^ UINT64_C(0xE2CE02C1DBEA2845)) * UINT64_C(0xD7EF17178C46ABE3) ^ __rolq(d, 26);
	case 5: d += (tail[4] ^ UINT64_C(0x6096A6800704E7C5)) * UINT64_C(0x8329C6EB9E6AD3E3) ^ __rolq(d, 25);
	case 4: d += (tail[3] ^ UINT64_C(0xB2F97045254154A5)) * UINT64_C(0xD7EF17178C46ABE3) ^ __rolq(d, 24);
	case 3: d += (tail[2] ^ UINT64_C(0x414C6E02D8B72D05)) * UINT64_C(0x8329C6EB9E6AD3E3) ^ __rolq(d, 23);
	case 2: d += (tail[1] ^ UINT64_C(0xF6FDE799B4A0DBA5)) * UINT64_C(0xD7EF17178C46ABE3) ^ __rolq(d, 22);
	case 1: d += (tail[0] ^ UINT64_C(0x735664783B1136B5)) * UINT64_C(0x8329C6EB9E6AD3E3) ^ __rolq(d, 21);
		d ^= (__rolq(d, 35) + UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xC6BC279692B5CC83);
	};
  d ^= d >> 32;
  d *= 0xbea225f9eb34556dULL;
  d ^= d >> 32;
  d *= 0xbea225f9eb34556dULL;
  d ^= d >> 32;
  d *= 0xbea225f9eb34556dULL;
  d ^= d >> 32;
	*(uint64_t *)out = d;
}


void
mx3_hash_test(const void *input, int len, const void *seed, void *out)
{
  *(uint64_t *) out = mx3::mx3_hash((const unsigned char *)input, (uint64_t) len, *((const uint64_t *)seed));
}

