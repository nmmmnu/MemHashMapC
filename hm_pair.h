#ifndef _HM_PAIR_H
#define _HM_PAIR_H

#include <stdint.h>
#include <stdlib.h>

typedef uint64_t timestamp_t;

typedef struct HMPair{
	void *		next;		// system dependent

	uint32_t	keylen;		// 4
	uint32_t	vallen;		// 4
	char		buffer[];	// dynamic
}HMPair;

HMPair *hm_pair_create(const char*key, const char*val);
inline int hm_pair_free(HMPair *pair);

inline const char *hm_pair_getkey(const HMPair *pair);
inline const char *hm_pair_getval(const HMPair *pair);
inline int hm_pair_equalkey(const HMPair *pair, const char *key);
inline int hm_pair_equalpair(const HMPair *pair1, const HMPair *pair2);

#endif
