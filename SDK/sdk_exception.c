#include <sdk_exception.h>
#include <assert.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
//// GLOBAL External

sdk_exception_frame_t * sdk_exception_g_stack = 0;

////////////////////////////////////////////////////////////////////////////////
////

void sdk_exception_raise(const sdk_exception_t* e, const char* file, int line)
{
    #ifdef WIN32
    sdk_exception_frame_t * p;
    if(sdk_exception_g_index == -1){
        sdk_exception_init();
    }
    p = TlsGetValue(sdk_exception_g_index);
    #else
    sdk_exception_frame_t * p = sdk_exception_g_stack;
    #endif
    
    assert(e);
    if(p==NULL){
        fprintf(stderr, "Uncaught exception");
        if(e->reason){
            fprintf(stderr, " %s", e->reason);
        }else{
            fprintf(stderr, " at 0x%p", e);
        }
        
        if(file && line>0){
            fprintf(stderr, " raised at %s:%d\n", file, line);
        }
        fprintf(stderr, " aborting...\n");
        fflush(stderr);
        abort();
    }
    
    p->exception = 0;
    p->file = file;
    p->line= line;
    
    #ifdef WIN32
    sdk_exception_pop();
    #else
    sdk_exception_g_stack = sdk_exception_g_stack->prev;
    #endif
    longjmp(p->env, kSDK_Exception_Raised);
}

#ifdef WIN32
//    extern  _CRTIMP void __cdecl _assert(void*, void*, unsigned);
    #undef assert
    #define assert(e) ((e) || (_assert(#e, __FILE__, __LINE__), 0))
    
    sdk_size_t sdk_exception_g_index = -1;
    
    void sdk_exception_init(void) {
        BOOL cond;
        
        sdk_exception_g_index = TlsAlloc();
        assert(sdk_exception_g_index != TLS_OUT_OF_INDEXES);
        cond = TlsSetValue(sdk_exception_g_index, NULL);
        assert(cond == TRUE);
    }
    
    void sdk_exception_push(sdk_exception_frame_t *fp) {
        BOOL cond;
        
        fp->prev = TlsGetValue(sdk_exception_g_index);
        cond = TlsSetValue(sdk_exception_g_index, fp);
        assert(cond == TRUE);
    }
    
    void sdk_exception_pop(void) {
        BOOL cond;
        sdk_exception_frame_t *tos = TlsGetValue(sdk_exception_g_index);
        
        cond = TlsSetValue(sdk_exception_g_index, tos->prev);
        assert(cond == TRUE);
    }
#endif

