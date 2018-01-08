#ifndef _ROWAN_TYPES_H
#define _ROWAN_TYPES_H

#ifndef __ASSEMBLY__


typedef _Bool			bool;

/* bsd */
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

/* sysv */
typedef unsigned char		unchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned int		uint;
typedef unsigned long		ulong;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef char		s8;
typedef short		s16;
typedef int		s32;
typedef long long	s64;

struct list_head {
	struct list_head *next, *prev;
};

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};

#endif /*  __ASSEMBLY__ */
#endif /* _ROWAN_TYPES_H */
