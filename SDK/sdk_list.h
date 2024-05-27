#ifndef INCLUDED_SDK_LIST_H
#define INCLUDED_SDK_LIST_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

////////////////////////////////////////////////////////////////////////////////
////
typedef struct sdk_list_node_s sdk_list_node_t;
typedef sdk_list_node_t sdk_list_t;

struct sdk_list_node_s {
    sdk_list_node_t * prev;
    sdk_list_node_t * next;
};
////////////////////////////////////////////////////////////////////////////////
////
#define SDK_LIST_NEXT(N) (N)->next
#define SDK_LIST_PREV(N) (N)->prev
#define SDK_LIST_PREV_NEXT(N) SDK_LIST_NEXT(SDK_LIST_PREV(N))
#define SDK_LIST_NEXT_PREV(N) SDK_LIST_PREV(SDK_LIST_NEXT(N))

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_LIST_INIT(L) (SDK_LIST_NEXT(L) = SDK_LIST_PREV(L) = (L))


#define SDK_LIST_IS_EMPTY(L) (SDK_LIST_NEXT(L)==(L))

/*
 Np <-- N <-- Nn
 */
#define SDK_LIST_REMOVE(N) \
do{                        \
    SDK_LIST_PREV_NEXT(N) = SDK_LIST_NEXT(N); \
    SDK_LIST_NEXT_PREV(N) = SDK_LIST_PREV(N); \
    SDK_LIST_INIT(N);      \
}while(0)


/*
  Np <-- L <-- Nn
  Np <-- N <-- L <-- Nn
*/
#define SDK_LIST_INSERT_BEFORE(L, N) \
do{                                  \
    SDK_LIST_PREV_NEXT(L) = (N);     \
    SDK_LIST_PREV(N) = SDK_LIST_PREV(L); \
    SDK_LIST_NEXT(N) = (L);          \
    SDK_LIST_PREV(L) = (N);          \
}while(0)



/*
  Np <-- L <-- Nn
  Np <-- L <-- N <-- Nn
*/
#define SDK_LIST_INSERT_AFTER(L, N) \
do{                                  \
    SDK_LIST_NEXT_PREV(L) = (N);     \
    SDK_LIST_NEXT(N) = SDK_LIST_NEXT(L); \
    SDK_LIST_PREV(N) = (L);          \
    SDK_LIST_NEXT(L) = (N);          \
}while(0)



/*
  Lp <-- L <-- Ln <-- Lp
  Np <-- N <-- Nn <-- Np
  
  |--------------------------|
  |                          ↓
  L <-- Ln <-- Lp <-- Nn <-- Np
  ↑                          |
  |<-------------------------|
*/
#define SDK_LIST_ADD(L, N)                                                    \
  do {                                                                        \
    SDK_LIST_PREV_NEXT(L) = SDK_LIST_NEXT(N);   /* Lp --> Nn */               \
    SDK_LIST_NEXT_PREV(N) = SDK_LIST_PREV(L);   /* Lp <-- Nn */               \
    SDK_LIST_PREV(L) = SDK_LIST_PREV(N);        /* Np <-- L  */               \
    SDK_LIST_PREV_NEXT(L) = (L);                /* Np --> L  */               \
    SDK_LIST_INIT(N);                                                         \
  }                                                                           \
  while (0)


/*
    Lp <-- L <-- Ln <-- Lp
    Np <-- N <-- Nn <-- Np
    Qp <-- Q <-- Qn <-- Qp
    
    Ln <-- Lp <-- N <-- Q <-- Qn <-- Qp <-- L
    |---------------------------------------^
*/
#define SDK_LIST_SPLIT(L, Q, N) \
  do {                          \
    SDK_LIST_PREV(N) = SDK_LIST_PREV(L); /* Lp <-- N */  \
    SDK_LIST_PREV_NEXT(N) = (N);         /* Lp --> N */   \
    SDK_LIST_NEXT(N) = (Q);              /* N  --> Q */   \
    SDK_LIST_PREV(L) = SDK_LIST_PREV(Q); /* Qp <-- L */   \
    SDK_LIST_PREV_NEXT(L) = (L);         /* Qp --> L */   \
    SDK_LIST_PREV(Q) = (N);              /* N <-- Q */ \
  }                             \
  while (0)


/*

*/
#define SDK_LIST_MOVE(h, n) \
  do {                      \
    if (SDK_LIST_EMPTY(h))  \
      SDK_LIST_INIT(n);     \
    else {                  \
      SDK_LIST* q = SDK_LIST_HEAD(h); \
      SDK_LIST_SPLIT(h, q, n);        \
    }                       \
  }                         \
  while (0)

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_LIST_HEAD(L) (SDK_LIST_NEXT(L))

#define SDK_LIST_DATA(ptr, type, field) \
  ((type *) ((char *) (ptr) - ((unsigned long) &((type *) 0)->field)))


#endif /* INCLUDED_SDK_LIST_H */
