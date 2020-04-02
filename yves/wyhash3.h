/*	Author: Wang Yi <godspeed_china@yeah.net>	*/
#ifndef wyhash_version_3
#define wyhash_version_3
#include	<stdint.h>
#include	<string.h>
#if defined(_MSC_VER) && defined(_M_X64)
#include <intrin.h>
#pragma	intrinsic(_umul128)
#endif
const	uint64_t	_wy3p0=0xa0761d6478bd642full,	_wy3p1=0xe7037ed1a0b428dbull,	_wy3p2=0x8ebc6af09c88c6e3ull,	_wy3p3=0x589965cc75374cc3ull,	_wy3p4=0x1d8e4e27c47d124full;
static	inline	uint64_t	_wy3rotr(uint64_t v, unsigned k) {	return	(v>>k)|(v<<(64-k));	}
static	inline	uint64_t	_wy3mum(uint64_t	A,	uint64_t	B) {
#ifdef WOOT
    //return (A ^ _wy3rotr(B, 25)) * (B ^ _wy3rotr(A, 25));
    uint64_t r = (A ^ _wy3rotr(B, 25)) * (B ^ _wy3rotr(A, 25));
	return r ^ (r >> 32);
#elif	defined(WOOF)
    //return (A ^ A >> 32) * 0xC13FA9A902A6328FULL + (B ^ B >> 32) * 0x91E10DA5C79E7B1DULL;
	uint64_t r = (A ^ A >> 32) * 0xC13FA9A902A6328FULL + (B ^ B >> 32) * 0x91E10DA5C79E7B1DULL;
	return r ^ A ^ B;
#elif	defined(WYHASH32)
	uint64_t	hh=(A>>32)*(B>>32),	hl=(A>>32)*(unsigned)B,	lh=(unsigned)A*(B>>32),	ll=(uint64_t)(unsigned)A*(unsigned)B;
	return	_wy3rotr(hl,32)^_wy3rotr(lh,32)^hh^ll;
#elif defined(__SIZEOF_INT128__)
	__uint128_t	r=A;	r*=B;	return	(r>>64)^r;
#elif	defined(_MSC_VER) && defined(_M_X64)
	A=_umul128(A, B, &B);	return	A^B;
#else
	uint64_t	ha=A>>32,	hb=B>>32,	la=(uint32_t)A,	lb=(uint32_t)B,	hi, lo;
	uint64_t	rh=ha*hb,	rm0=ha*lb,	rm1=hb*la,	rl=la*lb,	t=rl+(rm0<<32),	c=t<rl;
	lo=t+(rm1<<32);	c+=lo<t;hi=rh+(rm0>>32)+(rm1>>32)+c;	return hi^lo;
#endif
}
static	inline	uint64_t	_wy3r8(const	uint8_t	*p)	{	uint64_t v;	memcpy(&v,  p,  8);	return  v;	}
static	inline	uint64_t	_wy3r4(const	uint8_t	*p) {	uint32_t v;	memcpy(&v,	p,	4);	return	v;	}
static	inline	uint64_t	_wy3r3(const	uint8_t	*p,	unsigned	k) {	return	(((uint64_t)p[0])<<16)|(((uint64_t)p[k>>1])<<8)|p[k-1];	}
static	inline	uint64_t	wyhash3(const void* key,	uint64_t	len,	uint64_t	seed) {
    seed = (seed ^ _wy3rotr(seed, 50) ^ _wy3rotr(seed, 25)) * _wy3p0;
    seed = (seed ^ seed >> 29) * _wy3p1;
    seed ^= seed >> 26;
#if defined(__GNUC__) || defined(__INTEL_COMPILER)
	if(__builtin_expect(!len,0))	return	seed;
#else
	if(!len)	return	seed;
#endif
	const	uint8_t	*p=(const	uint8_t*)key;
	if(len<4)	return	_wy3mum(_wy3mum(_wy3r3(p,len)^seed^_wy3p0,seed^_wy3p1),len^_wy3p4);
	else	if(len<=8)	return	_wy3mum(_wy3mum(_wy3r4(p)^seed^_wy3p0,_wy3r4(p+len-4)^seed^_wy3p1),len^_wy3p4);
	else	if(len<=16)	return	_wy3mum(_wy3mum(_wy3r8(p)^seed^_wy3p0,_wy3r8(p+len-8)^seed^_wy3p1),len^_wy3p4);
	else	if(len<=24)	return	_wy3mum(_wy3mum(_wy3r8(p)^seed^_wy3p0,_wy3r8(p+8)^seed^_wy3p1)^_wy3mum(_wy3r8(p+len-8)^seed^_wy3p2,seed^_wy3p3),len^_wy3p4);
	else	if(len<=32)	return	_wy3mum(_wy3mum(_wy3r8(p)^seed^_wy3p0,_wy3r8(p+8)^seed^_wy3p1)^_wy3mum(_wy3r8(p+16)^seed^_wy3p2,_wy3r8(p+len-8)^seed^_wy3p3),len^_wy3p4);
	uint64_t	see1=seed,	i=len;
	if(i>=256)	for(; i>=256; i-=256,p+=256) {
		seed=_wy3mum(_wy3r8(p)^seed^_wy3p0,_wy3r8(p+8)^seed^_wy3p1)^_wy3mum(_wy3r8(p+16)^seed^_wy3p2,_wy3r8(p+24)^seed^_wy3p3);
		see1=_wy3mum(_wy3r8(p+32)^see1^_wy3p1,_wy3r8(p+40)^see1^_wy3p2)^_wy3mum(_wy3r8(p+48)^see1^_wy3p3,_wy3r8(p+56)^see1^_wy3p0);
		seed=_wy3mum(_wy3r8(p+64)^seed^_wy3p0,_wy3r8(p+72)^seed^_wy3p1)^_wy3mum(_wy3r8(p+80)^seed^_wy3p2,_wy3r8(p+88)^seed^_wy3p3);
		see1=_wy3mum(_wy3r8(p+96)^see1^_wy3p1,_wy3r8(p+104)^see1^_wy3p2)^_wy3mum(_wy3r8(p+112)^see1^_wy3p3,_wy3r8(p+120)^see1^_wy3p0);
		seed=_wy3mum(_wy3r8(p+128)^seed^_wy3p0,_wy3r8(p+136)^seed^_wy3p1)^_wy3mum(_wy3r8(p+144)^seed^_wy3p2,_wy3r8(p+152)^seed^_wy3p3);
		see1=_wy3mum(_wy3r8(p+160)^see1^_wy3p1,_wy3r8(p+168)^see1^_wy3p2)^_wy3mum(_wy3r8(p+176)^see1^_wy3p3,_wy3r8(p+184)^see1^_wy3p0);
		seed=_wy3mum(_wy3r8(p+192)^seed^_wy3p0,_wy3r8(p+200)^seed^_wy3p1)^_wy3mum(_wy3r8(p+208)^seed^_wy3p2,_wy3r8(p+216)^seed^_wy3p3);
		see1=_wy3mum(_wy3r8(p+224)^see1^_wy3p1,_wy3r8(p+232)^see1^_wy3p2)^_wy3mum(_wy3r8(p+240)^see1^_wy3p3,_wy3r8(p+248)^see1^_wy3p0);
	}
	for(; i>=32; i-=32,p+=32) {	seed=_wy3mum(_wy3r8(p)^seed^_wy3p0,_wy3r8(p+8)^seed^_wy3p1);	see1=_wy3mum(_wy3r8(p+16)^see1^_wy3p2,_wy3r8(p+24)^see1^_wy3p3);	}
	if(!i) {}
	else	if(i<4)	seed=_wy3mum(_wy3r3(p,i)^seed^_wy3p0,seed^_wy3p1);
	else	if(i<=8)	seed=_wy3mum(_wy3r4(p)^seed^_wy3p0,_wy3r4(p+i-4)^seed^_wy3p1);
	else	if(i<=16)	seed=_wy3mum(_wy3r8(p)^seed^_wy3p0,_wy3r8(p+i-8)^seed^_wy3p1);
	else	if(i<=24) {	seed=_wy3mum(_wy3r8(p)^seed^_wy3p0,_wy3r8(p+8)^seed^_wy3p1);	see1=_wy3mum(_wy3r8(p+i-8)^see1^_wy3p2,see1^_wy3p3);	} 
	else {	seed=_wy3mum(_wy3r8(p)^seed^_wy3p0,_wy3r8(p+8)^seed^_wy3p1);	see1=_wy3mum(_wy3r8(p+16)^see1^_wy3p2,_wy3r8(p+i-8)^see1^_wy3p3);	}
	return	_wy3mum(seed^see1,len^_wy3p4);
}
//static	inline	uint64_t	wyhash64(uint64_t	A, uint64_t	B) {	return	_wy3mum(_wy3mum(A^_wy3p0,	B^_wy3p1),	_wy3p2);	}
//static	inline	double	wy2u01(uint64_t	r) {	const	double	_wy3norm=1.0/(1ull<<52);	return	(r>>11)*_wy3norm;	}
//static	inline	float	wy2gau(uint64_t	r) {	const	float	_wy3norm1=1.0f/(1ull<<15);	return	(((r>>16)&0xffff)+((r>>32)&0xffff)+(r>>48))*_wy3norm1-3.0f;	}
//static	inline	uint64_t	wyrand(uint64_t	*seed) {	*seed+=_wy3p0;	return	_wy3mum(*seed^_wy3p1,*seed);	}
//static	uint64_t	_wy3rand_seed=0;
//#define	WYRAND_MAX	0xffffffffffffffffull
//static	inline	void	wysrand(uint64_t	seed) {	_wy3rand_seed=seed;	}
//static	inline	uint64_t	wygrand(void) {
//	uint64_t s;
//#if defined(_OPENMP)
//	#pragma omp atomic capture
//#endif
//	{
//		_wy3rand_seed+=_wy3p0;	s=_wy3rand_seed;
//	}
//	return	_wy3mum(s^_wy3p1,s);
//}
#endif