#include <sm_httpclient.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
////

int sm_httpclient_init(sm_httpclient_t* httpclient)
{
    assert(httpclient);
    
    /* Init SSL */
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    
    
    
    
    return 0;
}