#ifndef _HM_PAIR_H
#define _HM_PAIR_H

#include <stdint.h>
#include <stdlib.h>

typedef uint64_t timestamp_t;

typedef struct{
	void *		next;		// system dependent

	timestamp_t	created;	// 8
	uint32_t	expires;	// 4, 136 years, not that bad.

	uint32_t	keylen;		// 4
	uint32_t	vallen;		// 4

	char		buffer[];	// dynamic
}hm_pair_t;

hm_pair_t *hm_pair_create(const char*key, const char*val);
inline hm_pair_t *hm_pair_createx(const char*key, const char*val, uint32_t expires);

/* inline int hm_pair_free(hm_pair_t *pair); */

inline const char *hm_pair_getkey(const hm_pair_t *pair);
inline const char *hm_pair_getval(const hm_pair_t *pair);

inline int hm_pair_equalkey(const hm_pair_t *pair, const char *key);
inline int hm_pair_equalpair(const hm_pair_t *pair1, const hm_pair_t *pair2);

inline int hm_pair_valid(const hm_pair_t *pair);

#endif
