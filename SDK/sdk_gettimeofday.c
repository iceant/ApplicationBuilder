#include <sdk_gettimeofday.h>

#include <time.h>
#ifdef WIN32
#include <windows.h>
#include <stdint.h>

#else
#include <sys/time.h>
#endif
#ifdef WIN32

#if 0
int gettimeofday(struct timeval *tp, struct timezone *tz)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year   = wtm.wYear  - 1900;
    tm.tm_mon    = wtm.wMonth - 1;
    tm.tm_mday   = wtm.wDay;
    tm.tm_hour   = wtm.wHour;
    tm.tm_min    = wtm.wMinute;
    tm.tm_sec    = wtm.wSecond;
    tm.tm_isdst  = -1;
    clock = mktime(&tm);
    tp->tv_sec   = clock;
    tp->tv_usec  = wtm.wMilliseconds * 1000;
    return (0);
}
#endif

#if 0
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;
    
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);
    
    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;
    
    tv->tv_sec = (long)((time - EPOCH) / 10000000L);
    tv->tv_usec = (long)(system_time.wMilliseconds * 1000);
    return 0;
}
#endif

#if 1
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    FILETIME ft;
    uint64_t tmpres = 0;
    static int tzflag = 0;
    
    if (tv) {
        #ifdef _WIN32_WCE
        SYSTEMTIME st;
        GetSystemTime(&st);
        SystemTimeToFileTime(&st, &ft);
        #else
        GetSystemTimeAsFileTime(&ft);
        #endif
        
        
        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;
        
        
        /*converting file time to unix epoch*/
        tmpres /= 10;  /*convert into microseconds*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS;
        tv->tv_sec = (long) (tmpres / 1000000UL);
        tv->tv_usec = (long) (tmpres % 1000000UL);
    }
    
    
    if (tz) {
        if (!tzflag) {
            #if !TSK_UNDER_WINDOWS_RT
            _tzset();
            #endif
            tzflag++;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }
    
    return 0;
}
#endif

#endif