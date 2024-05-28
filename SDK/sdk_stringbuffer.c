#include <sdk_stringbuffer.h>
#include <string.h>
#include <errno.h>

int sdk_stringbuffer_init(sdk_stringbuffer_t* sbuf, unsigned char* buffer, sdk_size_t buffer_size){
    sbuf->buffer = buffer;
    sbuf->buffer_size = buffer_size;
    sbuf->write_idx = 0;
    sbuf->read_idx = 0;
    return SDK_STRINGBUFFER_OK;
}

int sdk_stringbuffer_put(sdk_stringbuffer_t* sbuf, char item)
{
    sdk_size_t next_write_idx = sbuf->write_idx + 1;
    if(next_write_idx>=sbuf->buffer_size){
        next_write_idx = 0;
    }
    if(next_write_idx == sbuf->read_idx){
        return SDK_STRINGBUFFER_FULL;
    }
    
    sbuf->buffer[sbuf->write_idx] = item;
    sbuf->write_idx= next_write_idx;
    
    return SDK_STRINGBUFFER_OK;
}

int sdk_stringbuffer_pop(sdk_stringbuffer_t* sbuf, char* item)
{
    if(sbuf->read_idx == sbuf->write_idx){
        return SDK_STRINGBUFFER_EMPTY;
    }
    
    sdk_size_t next_read_idx = sbuf->read_idx + 1;
    if(next_read_idx>=sbuf->buffer_size){
        next_read_idx = 0;
    }
    
    *item = (char)sbuf->buffer[sbuf->read_idx];
    
    sbuf->read_idx = next_read_idx;
    
    return SDK_STRINGBUFFER_OK;
}

int sdk_stringbuffer_peek(sdk_stringbuffer_t* sbuf, char* item)
{
    if(sbuf->read_idx == sbuf->write_idx){
        return SDK_STRINGBUFFER_EMPTY;
    }

    *item = (char)sbuf->buffer[sbuf->read_idx];
    
    return SDK_STRINGBUFFER_OK;
}

char sdk_stringbuffer_get(sdk_stringbuffer_t* sbuf, sdk_size_t idx)
{

    if(sbuf->read_idx == sbuf->write_idx){
        return SDK_STRINGBUFFER_INVALID_VALUE;
    }

    if(idx>=sbuf->buffer_size){
        return SDK_STRINGBUFFER_INVALID_VALUE;
    }

    sdk_size_t size = SDK_STRINGBUFFER_SIZE(sbuf);
    if( idx >= size){
        return SDK_STRINGBUFFER_INVALID_VALUE;
    }

    sdk_size_t next_read_idx = sbuf->read_idx + idx;
    if(next_read_idx>=sbuf->buffer_size){
        next_read_idx =  next_read_idx - sbuf->buffer_size;
    }
    
    return (char)sbuf->buffer[next_read_idx];
}


sdk_size_t sdk_stringbuffer_size(sdk_stringbuffer_t * sbuf){
    return SDK_STRINGBUFFER_SIZE(sbuf);
}


/*
 * buffer_size = 6
 * idx = 4
 * [0]   [1]   [2]   [3]   [4]   [5]
 *              |-w   |-r                   size=6-3+2 = 5; next_read_idx = 3+4 = 7; next_read_idx = 7-6=1
 */
int sdk_stringbuffer_advance_read_idx(sdk_stringbuffer_t * sbuf, sdk_size_t idx)
{
    if(sbuf->read_idx == sbuf->write_idx){
        return SDK_STRINGBUFFER_EMPTY;
    }

    if(idx>=sbuf->buffer_size){
        return SDK_STRINGBUFFER_EINVAL;
    }

    sdk_size_t size = SDK_STRINGBUFFER_SIZE(sbuf);
    if( idx >= size){
        return SDK_STRINGBUFFER_OUTOFBOUNDARY;
    }

    sdk_size_t next_read_idx = sbuf->read_idx + idx;
    if(next_read_idx>=sbuf->buffer_size){
        next_read_idx =  next_read_idx - sbuf->buffer_size;
    }
    sbuf->read_idx = next_read_idx;

    return SDK_STRINGBUFFER_OK;
}

////////////////////////////////////////////////////////////////////////////////
////

sdk_size_t sdk_stringbuffer_find(sdk_stringbuffer_t * sbuf
        , const char* needle, sdk_size_t needle_size)
{
    sdk_size_t i;
    sdk_size_t j;

    sdk_size_t size = SDK_STRINGBUFFER_SIZE(sbuf);

    if (needle_size > size) {
        return SDK_STRINGBUFFER_INVALID_INDEX;
    }

    for(i=0; i< size; i++){
        if(sdk_stringbuffer_get(sbuf, i)==needle[0]){
            for(j=1; j<needle_size; j++){
                if(sdk_stringbuffer_get(sbuf, i+j)!=needle[j]){
                    break;
                }
            }
            if(j==needle_size){
                break;
            }
        }
    }

    if(i==size){
        return SDK_STRINGBUFFER_INVALID_INDEX;
    }
    return i;
}

////////////////////////////////////////////////////////////////////////////////
////

#ifndef ULONG_MAX
#define	ULONG_MAX	((unsigned long)(~0L))		/* 0xFFFFFFFF */
#endif

/*
 * isspace --
 *
 * PUBLIC: #ifndef HAVE_ISSPACE
 * PUBLIC: int isspace __P((int));
 * PUBLIC: #endif
 */
int
ISSPACE(int c)
{
    return (c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r' || c == ' ' ? 1 : 0);
}

/*
 * isdigit --
 *
 * PUBLIC: #ifndef HAVE_ISDIGIT
 * PUBLIC: int isdigit __P((int));
 * PUBLIC: #endif
 */
int
ISDIGIT(int c)
{
    /*
     * Depends on ASCII-like character ordering.
     */
    return (c >= '0' && c <= '9' ? 1 : 0);
}

int
ISUPPER(int c)
{
    /*
     * Depends on ASCII-like character ordering.
     */
    return (c >= 'A' && c <= 'Z' ? 1 : 0);
}

/*
 * isalpha --
 *
 * PUBLIC: #ifndef HAVE_ISALPHA
 * PUBLIC: int isalpha __P((int));
 * PUBLIC: #endif
 */
int
ISALPHA(int c)
{
    /*
     * Depends on ASCII-like character ordering.
     */
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? 1 : 0);
}

/*
 * Convert a string to an unsigned long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
unsigned long
sdk_ringbuffer_strtoul(sdk_stringbuffer_t * sbuf, sdk_size_t * endptr, register int base)
{
    register sdk_size_t s = 0;
    register unsigned long acc;
    register int c;
    register unsigned long cutoff;
    register int neg = 0, any, cutlim;

    /*
     * See strtol for comments as to the logic used.
     */
    do {
        c = (int)sdk_stringbuffer_get(sbuf, s++);
    } while (ISSPACE(c));
    if (c == '-') {
        neg = 1;
        c = (int)sdk_stringbuffer_get(sbuf, s++);
    } else if (c == '+')
        c = (int)sdk_stringbuffer_get(sbuf, s++);
    if ((base == 0 || base == 16) &&
        c == '0' && (sdk_stringbuffer_get(sbuf, s) == 'x' || sdk_stringbuffer_get(sbuf, s) == 'X')) {
        c = (int)sdk_stringbuffer_get(sbuf, 1);
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
    cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
    for (acc = 0, any = 0;; c = sdk_stringbuffer_get(sbuf, s++)) {
        if (ISDIGIT(c))
            c -= '0';
        else if (ISALPHA(c))
            c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = ULONG_MAX;
        errno = ERANGE;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = (sdk_size_t) (any ? s - 1 : sbuf->read_idx);
    return (acc);
}




