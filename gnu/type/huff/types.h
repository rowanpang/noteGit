#ifndef __TYPES_H__
#define __TYPES_H__

typedef _Bool			bool;
typedef signed char __s8;
typedef unsigned char __u8;

typedef signed short __s16;
typedef unsigned short __u16;

typedef signed int __s32;
typedef unsigned int __u32;

typedef signed long long __s64;
typedef unsigned long long __u64;

typedef unsigned char           unchar;
typedef unsigned short          ushort;
typedef unsigned int            uint;
typedef unsigned long           ulong;


typedef         __u8            uint8;
typedef         __u16           uint16;
typedef         __u32           uint32;

typedef         __s8            int8;
typedef         __s16           int16;
typedef         __s32           int32;

struct list_head {
    struct list_head *next, *prev;
};

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
};

#endif
