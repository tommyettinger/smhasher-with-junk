// This is free and unencumbered software released into the public domain under The Unlicense (http://unlicense.org/)
// main repo: https://github.com/wangyi-fudan/wyhash
// author: 王一 Wang Yi <godspeed_china@yeah.net>
// contributors: Reini Urban, Dietrich Epp, Joshua Haberman, Tommy Ettinger, Daniel Lemire, Otmar Ertl, cocowalla, leo-yuriev, Diego Barrios Romero, paulie-g, dumblob, Yann Collet, ivte-ms, hyb, James Z.M. Gao, easyaspi314 (Devin), TheOneric

/* quick example:
   string s="fjsakfdsjkf";
   uint64_t hash=wyhash(s.c_str(), s.size(), 0, _wildp);
*/

#ifndef wildhash_version_1
#define wildhash_version_1

//includes
#include <stdint.h>
#include <string.h>
#if defined(_MSC_VER) && defined(_M_X64)
  #include <intrin.h>
#endif

//likely and unlikely macros
#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
  #define _likely_(x)  __builtin_expect(x,1)
  #define _unlikely_(x)  __builtin_expect(x,0)
#else
  #define _likely_(x) (x)
  #define _unlikely_(x) (x)
#endif

//128bit multiply function
static inline void _wildmum(uint64_t *A, uint64_t *B){
  uint64_t m = (*A - ROTL64(*B, 38)) * (*B - ROTL64(*A, 40));
  m^=m>>32;
  *A=*B+m; *B=*A+m;
  // uint64_t hh=(*A>>32)*(*B>>32), hl=(*A>>32)*(uint32_t)*B, lh=(uint32_t)*A*(*B>>32), ll=(uint64_t)(uint32_t)*A*(uint32_t)*B;
  // *A=ROTL64(hl,32)^hh; *B=ROTL64(lh,32)^ll;
}

//multiply and xor mix function, aka MUM
static inline uint64_t _wildmix(uint64_t A, uint64_t B){ _wildmum(&A,&B); return A^B; }

//endian macros
#ifndef WYHASH_LITTLE_ENDIAN
  #if defined(_WIN32) || defined(__LITTLE_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    #define WYHASH_LITTLE_ENDIAN 1
  #elif defined(__BIG_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define WYHASH_LITTLE_ENDIAN 0
  #else
    #warning could not determine endianness! Falling back to little endian.
    #define WYHASH_LITTLE_ENDIAN 1
  #endif
#endif

//read functions
#if (WYHASH_LITTLE_ENDIAN)
static inline uint64_t _wildr8(const uint8_t *p) { uint64_t v; memcpy(&v, p, 8); return v;}
static inline uint64_t _wildr4(const uint8_t *p) { uint32_t v; memcpy(&v, p, 4); return v;}
#elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
static inline uint64_t _wildr8(const uint8_t *p) { uint64_t v; memcpy(&v, p, 8); return __builtin_bswap64(v);}
static inline uint64_t _wildr4(const uint8_t *p) { uint32_t v; memcpy(&v, p, 4); return __builtin_bswap32(v);}
#elif defined(_MSC_VER)
static inline uint64_t _wildr8(const uint8_t *p) { uint64_t v; memcpy(&v, p, 8); return _byteswap_uint64(v);}
static inline uint64_t _wildr4(const uint8_t *p) { uint32_t v; memcpy(&v, p, 4); return _byteswap_ulong(v);}
#else
static inline uint64_t _wildr8(const uint8_t *p) {
  uint64_t v; memcpy(&v, p, 8);
  return (((v >> 56) & 0xff)| ((v >> 40) & 0xff00)| ((v >> 24) & 0xff0000)| ((v >>  8) & 0xff000000)| ((v <<  8) & 0xff00000000)| ((v << 24) & 0xff0000000000)| ((v << 40) & 0xff000000000000)| ((v << 56) & 0xff00000000000000));
}
static inline uint64_t _wildr4(const uint8_t *p) {
  uint32_t v; memcpy(&v, p, 4);
  return (((v >> 24) & 0xff)| ((v >>  8) & 0xff00)| ((v <<  8) & 0xff0000)| ((v << 24) & 0xff000000));
}
#endif
static inline uint64_t _wildr3(const uint8_t *p, size_t k) { return (((uint64_t)p[0])<<16)|(((uint64_t)p[k>>1])<<8)|p[k-1];}
//wildhash main function
static inline uint64_t wildhash(const void *key, size_t len, uint64_t seed){
  const uint8_t *p=(const uint8_t *)key;
  uint64_t a, b, x = seed+0xDB4F0B9175AE2165UL, y = ROTL64(seed, 21)+0x9E3779B97F4A7C16UL, z = ROTL64(seed, 41)+0x632BE59BD9B4E019UL;
  if(_likely_(len<=16)){
    if(_likely_(len>=4)){
      a=((_wildr4(p)<<32)|_wildr4(p+((len>>3)<<2)))^(x+=0xC6BC279692B5CC83UL);
      b=((_wildr4(p+len-4)<<32)|_wildr4(p+len-4-((len>>3)<<2)))^(y+=0xF1357AEA2E62A9C5UL);
    }
    else if(_likely_(len>0)){ a=_wildr3(p,len)^(x+=0xC6BC279692B5CC83UL); b=(y+=0xF1357AEA2E62A9C5UL);}
    else {
      a=(x+=0xC6BC279692B5CC83UL);
      b=(y+=0xF1357AEA2E62A9C5UL);
    }
  }
  else{
    size_t i=len;
    if(_unlikely_(i>48)){
      uint64_t see1=seed, see2=seed;
      do{
        seed=_wildmix(_wildr8(p)^(x+=0xC6BC279692B5CC83UL),_wildr8(p+8)^seed);
        see1=_wildmix(_wildr8(p+16)^(y+=0xF1357AEA2E62A9C5UL),_wildr8(p+24)^see1);
        see2=_wildmix(_wildr8(p+32)^(z+=0xBEA225F9EB34556DUL),_wildr8(p+40)^see2);
        p+=48; i-=48;
      }while(_likely_(i>48));
      seed^=see1^see2;
    }
    while(_unlikely_(i>16)){  seed=_wildmix(_wildr8(p)^(x+=0xC6BC279692B5CC83UL),_wildr8(p+8)^seed^(y+=0xF1357AEA2E62A9C5UL));  i-=16; p+=16;  }
    a=_wildr8(p+i-16)^(x+=0xC6BC279692B5CC83UL);  b=_wildr8(p+i-8)^(y+=0xF1357AEA2E62A9C5UL);
  }
  return _wildmix((x+=0xF1357AEA2E62A9C5UL)^len,_wildmix(a^x,b^seed));
}


#endif

/* The Unlicense
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/