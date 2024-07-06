#ifndef INCLUDED_SDK_TIME_H
#define INCLUDED_SDK_TIME_H

////////////////////////////////////////////////////////////////////////////////
////

typedef unsigned long long sdk_time_t;

typedef struct sdk_datetime_s{
    int year;		//年
    int mon;        //月[1-12]
    int mday;		//日[1-31]
    int hour;		//时[0-23]
    int min;		//分[0-59]
    int sec;		//秒[0-59]
    int msec;       //毫秒[0-999]
    int wday;		//星期[0-6]，0代表星期天
    int yday;		//从每年1月1日开始的天数[0-365]，0代表1月1日
}sdk_datetime_t;
////////////////////////////////////////////////////////////////////////////////
////

sdk_time_t sdk_time_get_current_ms(void);
sdk_time_t sdk_time_get_current_us(void);

int sdk_datetime_get(sdk_datetime_t* datetime, const char* format, char* str, int str_size);

#endif /* INCLUDED_SDK_TIME_H */
