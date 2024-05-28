#ifndef INCLUDED_SDK_MACROS_H
#define INCLUDED_SDK_MACROS_H

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_ARRAY_SIZE(ARRAY) (sizeof(ARRAY)/sizeof((ARRAY)[0]))

#define SDK_MIN(a, b) ((a)>(b)?(b):(a))
#define SDK_MAX(a, b) ((a)>(b)?(a):(b))

#define SDK_CONTAINER_OF(ptr, type, field) \
  ((type *) ((char *) (ptr) - ((unsigned long) &((type *) 0)->field)))


#define SDK_PAGES(T,SZ) (((T)-1)/(SZ) + 1)

#endif /* INCLUDED_SDK_MACROS_H */
