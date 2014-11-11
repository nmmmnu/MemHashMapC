#ifndef _MHPAIR_H
#define _MHPAIR_H

struct mhpair{
	unsigned long int	created;	// 8
	unsigned long int	expires;	// 8
	unsigned int		keylen;		// 4
	unsigned int		vallen;		// 4
	char			buffer[];
};

struct mhpair *mhpair_create(const char*key, const char*val, unsigned long int expires);

inline const char *mhpair_getkey(const struct mhpair *pair, char *buffer, unsigned int len);
inline const char *mhpair_getval(const struct mhpair *pair, char *buffer, unsigned int len);
inline int mhpair_equals(const struct mhpair *pair, const char *key);

int mhpair_valid(const struct mhpair *pair);
int mhpair_expired(const struct mhpair *pair);

void mhpair_test();

#endif
