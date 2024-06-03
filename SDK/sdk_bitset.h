#ifndef INCLUDED_SDK_BITSET_H
#define INCLUDED_SDK_BITSET_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_UP_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_UP_TYPES_H*/


////////////////////////////////////////////////////////////////////////////////
////
typedef struct sdk_bitset_s sdk_bitset_t;

////////////////////////////////////////////////////////////////////////////////
////
sdk_bitset_t*   sdk_bitset_new(int length);
int sdk_bitset_length(sdk_bitset_t* set);
int sdk_bitset_count (sdk_bitset_t* set);
void sdk_bitset_free(sdk_bitset_t* *set);
int sdk_bitset_get(sdk_bitset_t* set, int n);
int sdk_bitset_put(sdk_bitset_t* set, int n, int bit);
void sdk_bitset_clear(sdk_bitset_t* set, int lo, int hi);
void sdk_bitset_set  (sdk_bitset_t* set, int lo, int hi);
void sdk_bitset_not  (sdk_bitset_t* set, int lo, int hi);
int sdk_bitset_lt (sdk_bitset_t* s, sdk_bitset_t* t);
int sdk_bitset_eq (sdk_bitset_t* s, sdk_bitset_t* t);
int sdk_bitset_leq(sdk_bitset_t* s, sdk_bitset_t* t);
void sdk_bitset_map(sdk_bitset_t* set,
                   void apply(int n, int bit, void *cl), void *cl);
sdk_bitset_t* sdk_bitset_union(sdk_bitset_t* s, sdk_bitset_t* t);
sdk_bitset_t* sdk_bitset_inter(sdk_bitset_t* s, sdk_bitset_t* t);
sdk_bitset_t* sdk_bitset_minus(sdk_bitset_t* s, sdk_bitset_t* t);
sdk_bitset_t* sdk_bitset_diff (sdk_bitset_t* s, sdk_bitset_t* t);

#endif /*INCLUDED_SDK_BITSET_H*/
