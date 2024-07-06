#include <sdk_time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    sdk_datetime_t now;
    char time_now_str[128];
    sdk_datetime_get(&now, "yyyy-MM-dd HH:mm:ss.SSS", time_now_str, sizeof(time_now_str));
    printf("Time: %s\n", time_now_str);
    
    printf("Time us: %llu\n", sdk_time_get_current_us());
    printf("Time ms: %llu\n", sdk_time_get_current_ms());
    return 0;
}