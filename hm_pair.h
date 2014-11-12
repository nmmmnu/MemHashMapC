#ifndef _HM_PAIR_H
#define _HM_PAIR_H

#include <stdint.h>

typedef struct HMPair{
	uint64_t	created;	// 8
	uint64_t	expires;	// 8
	uint32_t	keylen;		// 4
	uint32_t	vallen;		// 4
	char		buffer[];
}HMPair;

struct HMPair *hm_pair_create(const char*key, const char*val, unsigned long int expires);

inline const char *hm_pair_getkey(const HMPair *pair, char *buffer, unsigned int len);
inline const char *hm_pair_getval(const HMPair *pair, char *buffer, unsigned int len);
inline int hm_pair_equals(const HMPair *pair, const char *key);
inline int hm_pair_equalp(const HMPair *pair1, const HMPair *pair2);

int hm_pair_valid(const HMPair *pair);
int hm_pair_expired(const HMPair *pair);

#endif
