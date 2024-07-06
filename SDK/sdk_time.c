#include <sdk_time.h>
#include <time.h>
#include <sdk_gettimeofday.h>
#include <stdlib.h>
#if defined(WIN32)
    #include <windows.h>
#include <stdio.h>

#endif
////////////////////////////////////////////////////////////////////////////////
////

sdk_time_t sdk_time_get_current_us(void)
{
    struct timeval tv;
    sdk_gettimeofday(&tv, NULL);
    return (1000000*tv.tv_sec + tv.tv_usec);
}

sdk_time_t sdk_time_get_current_ms(void)
{
    struct timeb t;
    ftime(&t);
    return (1000*t.time + t.millitm);
}

int sdk_datetime_get(sdk_datetime_t* datetime, const char* format, char* str, int str_size)
{
    unsigned short msec;
    char buf[24] = {0};
    char *p = NULL;
    struct tm *tm_now = NULL;
#if defined(WIN32) //Linux
    time_t t;
    SYSTEMTIME currentTime;
    time(&t);
    GetSystemTime(&currentTime); //获取当前的UTC时间
    msec = currentTime.wMilliseconds;
    tm_now = localtime(&t);
#else //Windows
    struct timeb t;
    ftime(&t);
    msec = t.millitm;
    tm_now = localtime(&t.time);
#endif
    if(!tm_now) {
        return -1;
    }
    if(datetime) {
        datetime->year = tm_now->tm_year+1900;
        datetime->mon = tm_now->tm_mon+1;
        datetime->mday = tm_now->tm_mday;
        datetime->hour = tm_now->tm_hour;
        datetime->min = tm_now->tm_min;
        datetime->sec = tm_now->tm_sec;
        datetime->msec = msec;
        datetime->wday = tm_now->tm_wday;
        datetime->yday = tm_now->tm_yday;
    }
    if(str) {
        if(!format) {
            //默认格式：yyyyMMddHHmmsszzz
            snprintf(str, str_size, "%d%02d%02d%02d%02d%02d%03d", tm_now->tm_year+1900, tm_now->tm_mon+1,
                    tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, msec);
        }
        else {
            snprintf(buf, sizeof(buf), "%d%02d%02d%02d%02d%02d%03d", tm_now->tm_year+1900, tm_now->tm_mon+1,
                    tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, msec);
            strcpy(str, format);
            if(p = strstr(str, "yyyy")) { strncpy(p, buf, 4); }
            if(p = strstr(str, "MM")) { strncpy(p, buf + 4, 2); }
            if(p = strstr(str, "dd")) { strncpy(p, buf + 6, 2); }
            if(p = strstr(str, "HH")) { strncpy(p, buf + 8, 2); }
            if(p = strstr(str, "hh")) { strncpy(p, buf + 8, 2); }
            if(p = strstr(str, "mm")) { strncpy(p, buf + 10, 2); }
            if(p = strstr(str, "ss")) { strncpy(p, buf + 12, 2); }
            if(p = strstr(str, "zzz")) { strncpy(p, buf + 14, 3); }
            if(p = strstr(str, "SSS")) { strncpy(p, buf + 14, 3); }
        }
    }
    return 0;
}