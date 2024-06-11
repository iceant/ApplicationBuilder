#include <sdk_ap.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    sdk_ap_t ap1 = sdk_ap_new(11);
    sdk_ap_t ap2 = sdk_ap_new(2);
    
    sdk_ap_t z = sdk_ap_div(ap1, ap2);
    
    sdk_fmt_register('A', sdk_ap_fmt);
    sdk_fmt_print("%A\n", z);
    
    sdk_ap_free(&z);
    sdk_ap_free(&ap1);
    sdk_ap_free(&ap2);
    
    return 0;
}