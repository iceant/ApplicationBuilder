#include <sdk_str.h>
#include <sdk_memory.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
////
#define SDK_STR_IDX(i, len) ((i) <= 0 ? (i) + (len) : (i) - 1)

#define SDK_STR_CONVERT(s, A, B) \
do {                             \
    int sdk_str_convert__len;    \
	assert(s);                      \
    sdk_str_convert__len = strlen(s); \
	A = SDK_STR_IDX(A, sdk_str_convert__len); \
    B = SDK_STR_IDX(B, sdk_str_convert__len); \
	if (A > B) {                    \
        int t = A;                  \
        A = B;                      \
        B = t;                      \
    }                               \
	assert(A >= 0 && B <= sdk_str_convert__len); \
} while (0)



////////////////////////////////////////////////////////////////////////////////
////
void sdk_str_free(char* s){
    SDK_FREE(s);
}

char* sdk_str_sub(const char* s, int i, int j)
{
    char *str, *p;
    SDK_STR_CONVERT(s, i, j);
    p = str = SDK_ALLOC(j - i + 1);
    while (i < j)
        *p++ = s[i++];
    *p = '\0';
    return str;
}

char *sdk_str_dup(const char *s, int i, int j, int n)
{
    int k;
    char *str, *p;
    assert(n >= 0);
    SDK_STR_CONVERT(s, i, j);
    p = str = SDK_ALLOC(n*(j - i) + 1);
    if (j - i > 0){
        while (n-- > 0){
            for (k = i; k < j; k++){
                *p++ = s[k];
            }
        }
    }
    *p = '\0';
    return str;
}

char *sdk_str_reverse(const char *s, int i, int j)
{
    char *str, *p;
    SDK_STR_CONVERT(s, i, j);
    p = str = SDK_ALLOC(j - i + 1);
    while (j > i)
        *p++ = s[--j];
    *p = '\0';
    return str;
}

char *up_str_cat(const char *s1, int i1, int j1,
                 const char *s2, int i2, int j2) {
    char *str, *p;
    SDK_STR_CONVERT(s1, i1, j1);
    SDK_STR_CONVERT(s2, i2, j2);
    p = str = SDK_ALLOC(j1 - i1 + j2 - i2 + 1);
    while (i1 < j1)
        *p++ = s1[i1++];
    while (i2 < j2)
        *p++ = s2[i2++];
    *p = '\0';
    return str;
}


char *sdkstr_catv(const char *s, ...) {
    char *str, *p;
    const char *save = s;
    int i, j, len = 0;
    va_list ap;
    va_start(ap, s);
    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        SDK_STR_CONVERT(s, i, j);
        len += j - i;
        s = va_arg(ap, const char *);
    }
    va_end(ap);
    p = str = SDK_ALLOC(len + 1);
    s = save;
    va_start(ap, s);
    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        SDK_STR_CONVERT(s, i, j);
        while (i < j)
            *p++ = s[i++];
        s = va_arg(ap, const char *);
    }
    va_end(ap);
    *p = '\0';
    return str;
}

char *sdk_str_map(const char *s, int i, int j,
                 const char *from, const char *to) {
    static char map[256] = { 0 };
    if (from && to) {
        unsigned c;
        for (c = 0; c < sizeof map; c++)
            map[c] = (char)c;
        while (*from && *to)
            map[(unsigned char)*from++] = *to++;
        assert(*from == 0 && *to == 0);
    } else {
        assert(from == NULL && to == NULL && s);
        assert(map['a']);
    }
    if (s) {
        char *str, *p;
        SDK_STR_CONVERT(s, i, j);
        p = str = SDK_ALLOC(j - i + 1);
        while (i < j)
            *p++ = map[(unsigned char)s[i++]];
        *p = '\0';
        return str;
    } else
        return NULL;
}


int sdk_str_pos(const char *s, int i) {
    int len;
    assert(s);
    len = strlen(s);
    i = SDK_STR_IDX(i, len);
    assert(i >= 0 && i <= len);
    return i + 1;
}

int sdk_str_len(const char *s, int i, int j) {
    SDK_STR_CONVERT(s, i, j);
    return j - i;
}


int sdk_str_cmp(const char *s1, int i1, int j1,
               const char *s2, int i2, int j2) {
    SDK_STR_CONVERT(s1, i1, j1);
    SDK_STR_CONVERT(s2, i2, j2);
    s1 += i1;
    s2 += i2;
    if (j1 - i1 < j2 - i2) {
        int cond = strncmp(s1, s2, j1 - i1);
        return cond == 0 ? -1 : cond;
    } else if (j1 - i1 > j2 - i2) {
        int cond = strncmp(s1, s2, j2 - i2);
        return cond == 0 ? +1 : cond;
    } else
        return strncmp(s1, s2, j1 - i1);
}

int sdk_str_chr(const char *s, int i, int j, int c) {
    SDK_STR_CONVERT(s, i, j);
    for ( ; i < j; i++)
        if (s[i] == c)
            return i + 1;
    return 0;
}

int sdk_str_rchr(const char *s, int i, int j, int c) {
    SDK_STR_CONVERT(s, i, j);
    while (j > i)
        if (s[--j] == c)
            return j + 1;
    return 0;
}

int sdk_str_upto(const char *s, int i, int j,
                const char *set) {
    assert(set);
    SDK_STR_CONVERT(s, i, j);
    for ( ; i < j; i++)
        if (strchr(set, s[i]))
            return i + 1;
    return 0;
}

int sdk_str_rupto(const char *s, int i, int j,
                 const char *set) {
    assert(set);
    SDK_STR_CONVERT(s, i, j);
    while (j > i)
        if (strchr(set, s[--j]))
            return j + 1;
    return 0;
}

int sdk_str_find(const char *s, int i, int j,
                const char *str) {
    int len;
    SDK_STR_CONVERT(s, i, j);
    assert(str);
    len = strlen(str);
    if (len == 0){
        return i + 1;
    }
    else if (len == 1) {
        for ( ; i < j; i++) {
            if (s[i] == *str) {
                return i + 1;
            }
        }
    } else {
        for (; i + len <= j; i++) {
            if ((strncmp(&s[i], str, len) == 0)) {
                return i + 1;
            }
        }
    }
    return 0;
}

int sdk_str_rfind(const char *s, int i, int j,
                 const char *str) {
    int len;
    SDK_STR_CONVERT(s, i, j);
    assert(str);
    len = strlen(str);
    if (len == 0) {
        return j + 1;
    }else if (len == 1) {
        while (j > i) {
            if (s[--j] == *str) {
                return j + 1;
            }
        }
    } else {
        for (; j - len >= i; j--) {
            if (strncmp(&s[j - len], str, len) == 0) {
                return j - len + 1;
            }
        }
    }
    return 0;
}

int sdk_str_any(const char *s, int i, const char *set) {
    int len;
    assert(s);
    assert(set);
    len = strlen(s);
    i = SDK_STR_IDX(i, len);
    assert(i >= 0 && i <= len);
    if (i < len && strchr(set, s[i]))
        return i + 2;
    return 0;
}

int sdk_str_many(const char *s, int i, int j,
                const char *set) {
    assert(set);
    SDK_STR_CONVERT(s, i, j);
    if (i < j && strchr(set, s[i])) {
        do {
            i++;
        }while (i < j && strchr(set, s[i]));
        return i + 1;
    }
    return 0;
}

int sdk_str_rmany(const char *s, int i, int j,
                 const char *set) {
    assert(set);
    SDK_STR_CONVERT(s, i, j);
    if (j > i && strchr(set, s[j-1])) {
        do {
            --j;
        }while (j >= i && strchr(set, s[j]));
        return j + 2;
    }
    return 0;
}

int sdk_str_match(const char *s, int i, int j,
                 const char *str) {
    int len;
    SDK_STR_CONVERT(s, i, j);
    assert(str);
    len = strlen(str);
    if (len == 0)
        return i + 1;
    else if (len == 1) {
        if (i < j && s[i] == *str)
            return i + 2;
    } else if (i + len <= j && (strncmp(&s[i], str, len) == 0))
        return i + len + 1;
    return 0;
}

int sdk_str_rmatch(const char *s, int i, int j,
                  const char *str) {
    int len;
    SDK_STR_CONVERT(s, i, j);
    assert(str);
    len = strlen(str);
    if (len == 0)
        return j + 1;
    else if (len == 1) {
        if (j > i && s[j-1] == *str)
            return j;
    } else if (j - len >= i
               && strncmp(&s[j-len], str, len) == 0)
        return j - len + 1;
    return 0;
}

void sdk_str_fmt(int code, va_list_box *box,
                int put(int c, void *cl), void *cl,
                unsigned char flags[], int width, int precision) {
    char *s;
    int i, j;
    assert(box && flags);
    s = va_arg(box->ap, char *);
    i = va_arg(box->ap, int);
    j = va_arg(box->ap, int);
    SDK_STR_CONVERT(s, i, j);
    sdk_fmt_puts(s + i, j - i, put, cl, flags,
                width, precision);
}


char* sdk_str_replace(char* original, char* pattern, char* replacement)
{
    sdk_size_t const replen = strlen(replacement);
    sdk_size_t const patlen = strlen(pattern);
    sdk_size_t const orilen = strlen(original);

    sdk_size_t patcnt = 0;
    const char * oriptr;
    const char * patloc;

    // find how many times the pattern occurs in the original string
    for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
    {
        patcnt++;
    }

    {
        // allocate memory for the new string
        sdk_size_t const retlen = orilen + patcnt * (replen - patlen);
        char * const returned = (char *) SDK_ALLOC( sizeof(char) * (retlen + 1) );

        if (returned != NULL)
        {
            // copy the original string,
            // replacing all the instances of the pattern
            char * retptr = returned;
            for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
            {
                sdk_size_t const skplen = patloc - oriptr;
                // copy the section until the occurence of the pattern
                strncpy(retptr, oriptr, skplen);
//                memcpy(retptr, oriptr, skplen);
                retptr += skplen;
                // copy the replacement
                strncpy(retptr, replacement, replen);
//                memcpy(retptr, replacement, replen);
                retptr += replen;
            }
            // copy the rest of the string.
            strcpy(retptr, oriptr);
        }
        return returned;
    }
}


