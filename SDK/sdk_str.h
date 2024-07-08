#ifndef INCLUDED_SDK_STR_H
#define INCLUDED_SDK_STR_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif /*INCLUDED_STRING_H*/

#ifndef INCLUDED_STDARG_H
#define INCLUDED_STDARG_H
#include <stdarg.h>
#endif /*INCLUDED_STDARG_H*/

#ifndef INCLUDED_SDK_FMT_H
#include <sdk_fmt.h>
#endif /*INCLUDED_SDK_FMT_H*/

////////////////////////////////////////////////////////////////////////////////
////

/*
 1  2  3  4  5  6  7  8  9  10
 ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓
  I  n  t  e  r  f  a  c  e
 ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑   ↑  ↑
 -9 -8 -7 -6 -5 -4 -3 -2 -1  0
 */

////////////////////////////////////////////////////////////////////////////////
//// Interface  ALLOCATE MEMORY

void sdk_str_free(char* s);

char* sdk_str_sub(const char* s, int i, int j);

char *sdk_str_dup(const char *s, int i, int j, int n);

char *sdk_str_reverse(const char *s, int i, int j);

char *sdk_str_cat(const char *s1, int i1, int j1,
                 const char *s2, int i2, int j2);

char *sdkstr_catv(const char *s, ...);

char *sdk_str_map(const char *s, int i, int j,
                  const char *from, const char *to);


char* sdk_str_replace(char* original, char* pattern, char* replacement);

////////////////////////////////////////////////////////////////////////////////
//// No Memory Allocation

char* sdk_str_sub_to(char* dst, int dst_size, const char* s, int i, int j);

char *sdk_str_dup_to(char* dst, int dst_size, const char *s, int i, int j, int n);

char *sdk_str_reverse_to(char* dst, int dst_size, const char *s, int i, int j);

char *sdk_str_cat_to(char* dst, int dst_size
        , const char *s1, int i1, int j1
        , const char *s2, int i2, int j2);

char *sdkstr_catv_to(char* dst, int dst_size, const char *s, ...);

char *sdk_str_map_to(char* dst, int dst_size
                     , const char *s, int i, int j
                     , const char *from, const char *to);

int sdk_str_pos(const char *s, int i);

int sdk_str_len(const char *s, int i, int j);

int sdk_str_cmp(const char *s1, int i1, int j1,
                const char *s2, int i2, int j2);

int sdk_str_chr(const char *s, int i, int j, int c);

int sdk_str_rchr(const char *s, int i, int j, int c);

int sdk_str_upto(const char *s, int i, int j,
                 const char *set);

int sdk_str_rupto(const char *s, int i, int j,
                  const char *set);

int sdk_str_find(const char *s, int i, int j,
                 const char *str);

int sdk_str_rfind(const char *s, int i, int j,
                  const char *str);

int sdk_str_any(const char *s, int i, const char *set);

int sdk_str_many(const char *s, int i, int j,
                 const char *set);

int sdk_str_rmany(const char *s, int i, int j,
                  const char *set);


int sdk_str_match(const char *s, int i, int j,
                  const char *str);

int sdk_str_rmatch(const char *s, int i, int j,
                   const char *str);

void sdk_str_fmt(int code, va_list_box *box,
                 int put(int c, void *cl), void *cl,
                 unsigned char flags[], int width, int precision);

char* sdk_str_ltrim(char* str);
char* sdk_str_rtrim(char* str);
char* sdk_str_trim(char* str);

#endif /*INCLUDED_SDK_STR_H*/
