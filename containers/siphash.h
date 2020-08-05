#ifndef siphash_h_
#define siphash_h_

#include <sys/types.h>
typedef u_int64_t uint64;

#define SIP_ROT(x, b) x = (x << b) | (x >> (64 - b))

#define SIP_ROUND				\
	v0 += v1;				\
	v2 += v3;				\
	SIP_ROT(v1,13);				\
	SIP_ROT(v3,16);				\
	v1 ^= v0;				\
	v3 ^= v2;				\
	SIP_ROT(v0,32);				\
	v2 += v1;				\
	v0 += v3;				\
	SIP_ROT(v1,17);				\
	SIP_ROT(v3,21);				\
	v1 ^= v2;				\
	v3 ^= v0;				\
	SIP_ROT(v2,32);

uint64 siphash(const unsigned char *in, unsigned long long inlen, const unsigned char *k);

#endif
