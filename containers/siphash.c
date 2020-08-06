/*
 * Taken mostly from supercop:
 * https://github.com/nsacyber/simon-speck-supercop/blob/master/crypto_auth/siphash24/little2/siphash.c
 * Redis also uses siphash for its hash tables, and is a good read.
 * Modified to do siphash1-2 rather than 2-4.
 */
#include "siphash.h"

uint64 siphash(const unsigned char *in, unsigned long long inlen, const unsigned char *k)
{
	uint64 v0 = *(uint64 *) (k);
	uint64 v1 = *(uint64 *) (k + 8);
	uint64 lastblock = inlen << 56;
	uint64 v2 = v0;
	uint64 v3 = v1;
	int i;

	v0 ^= 0x736f6d6570736575;
	v1 ^= 0x646f72616e646f6d;
	v2 ^= 0x6c7967656e657261;
	v3 ^= 0x7465646279746573;

	while (inlen >= 8) { /* floor(inlen/8) normal loops */
		uint64 mi = *(uint64 *) in;
		in += 8;
		v3 ^= mi;
		SIP_ROUND;
		SIP_ROUND;
		v0 ^= mi;
		inlen -= 8;
	}
	/* now inlen <= 7; 1 extra loop; total loops: ceil((inlen+1)/8) */
	for (i = 0;i < inlen;++i) i[(char *) &lastblock] = i[in];
	v3 ^= lastblock;
	SIP_ROUND;
	v0 ^= lastblock;

	v2 ^= 0xff;
	SIP_ROUND;
	SIP_ROUND;

	return (v0 ^ v1) ^ (v2 ^ v3);
}
