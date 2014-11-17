#ifndef _HM_PAIR_H
#define _HM_PAIR_H

#include <stdint.h>
#include <stdlib.h>

typedef uint64_t timestamp_t;

typedef struct HMPair{
	void *		next;		// system dependent

	uint64_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.

	uint32_t	keylen;		// 4
	uint32_t	vallen;		// 4

	char		buffer[];	// dynamic
}HMPair;

HMPair *hm_pair_create(const char*key, const char*val);
inline HMPair *hm_pair_createx(const char*key, const char*val, uint32_t expires);

/* inline int hm_pair_free(HMPair *pair); */

inline const char *hm_pair_getkey(const HMPair *pair);
inline const char *hm_pair_getval(const HMPair *pair);
inline int hm_pair_equalkey(const HMPair *pair, const char *key);
inline int hm_pair_equalpair(const HMPair *pair1, const HMPair *pair2);

inline int hm_pair_valid(const HMPair *pair);

#endif
