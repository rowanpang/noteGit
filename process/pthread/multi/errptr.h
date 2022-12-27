/*=============================================================================
#     FileName: errptr.h
#         Desc: modify from linux/include/err.h
#       Author: Rowan Pang
#        Email: pangweizhen.2008@hotmail.com
#     HomePage:
#      Version: 0.0.1
#   LastChange: 2017-04-28 14:37:51
#      History:
=============================================================================*/
#ifndef _ROWAN_ERRPTR_H
#define _ROWAN_ERRPTR_H

#define MAX_ERRNO    4095

#ifndef __ASSEMBLY__

#define IS_ERR_VALUE(x) ( (unsigned long)(void *)(x) >= (unsigned long)-MAX_ERRNO )

static inline void * ERR_PTR(long error)
{
    return (void *) error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}

static inline int IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

static inline int IS_ERR_OR_NULL(const void *ptr)
{
    return (!ptr) || IS_ERR_VALUE((unsigned long)ptr);
}

/**
 * ERR_CAST - Explicitly cast an error-valued pointer to another pointer type
 * @ptr: The pointer to cast.
 *
 * Explicitly cast an error-valued pointer to another pointer type in such a
 * way as to make it clear that's what's going on.
 */
static inline void * ERR_CAST(const void *ptr)
{
    /* cast away the const */
    return (void *) ptr;
}

static inline int PTR_ERR_OR_ZERO(const void *ptr)
{
    if (IS_ERR(ptr))
        return PTR_ERR(ptr);
    else
        return 0;
}

#endif

#endif /* _ROWAN_ERRPTR_H */
