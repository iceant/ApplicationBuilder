#ifndef INCLUDED_SDK_GETTIMEOFDAY_H
#define INCLUDED_SDK_GETTIMEOFDAY_H

#ifndef INCLUDED_TIME_H
#define INCLUDED_TIME_H
#include <time.h>
#endif /*INCLUDED_TIME_H*/

////////////////////////////////////////////////////////////////////////////////
////
#if defined(WIN32)

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#endif

//struct timezone
//{
//    int  tz_minuteswest; /* minutes W of Greenwich */
//    int  tz_dsttime;     /* type of dst correction */
//};

int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif

#define sdk_gettimeofday gettimeofday

#endif /* INCLUDED_SDK_GETTIMEOFDAY_H */
