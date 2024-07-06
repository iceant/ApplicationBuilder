#ifndef INCLUDED_SDK_MACROS_H
#define INCLUDED_SDK_MACROS_H

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_ARRAY_SIZE(A) (sizeof(A)/sizeof((A)[0]))

#define SDK_MIN(a, b) ((a)>(b)?(b):(a))
#define SDK_MAX(a, b) ((a)>(b)?(a):(b))

#define SDK_CONTAINER_OF(ptr, type, field) \
  ((type *) ((char *) (ptr) - ((size_t) &((type *) 0)->field)))


#define SDK_PAGES(T,U) (((T)-1)/(U) + 1)


////////////////////////////////////////////////////////////////////////////////
////

#endif /* INCLUDED_SDK_MACROS_H */
