#ifndef INCLUDED_SDK_EXCEPTION_H
#define INCLUDED_SDK_EXCEPTION_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

#ifndef INCLUDED_SETJMP_H
#define INCLUDED_SETJMP_H
#include <setjmp.h>
#endif /*INCLUDED_SETJMP_H*/

#ifdef WIN32
    #ifndef INCLUDED_WINDOWS_H
    #define INCLUDED_WINDOWS_H
    #include <windows.h>
    #endif /*INCLUDED_WINDOWS_H*/
#endif

////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_exception_frame_s sdk_exception_frame_t;

typedef struct sdk_exception_s{
    const char* reason;
}sdk_exception_t;

struct sdk_exception_frame_s{
    sdk_exception_frame_t * prev;
    jmp_buf env;
    const char* file;
    int line;
    const sdk_exception_t * exception;
};

typedef enum sdk_exception_state_enum{
    kSDK_Exception_Entered = 0,
    kSDK_Exception_Raised,
    kSDK_Exception_Handled,
    kSDK_Exception_Finalized,
}sdk_exception_state_t;

////////////////////////////////////////////////////////////////////////////////
////

extern sdk_exception_frame_t * sdk_exception_g_stack;

////////////////////////////////////////////////////////////////////////////////
////

void sdk_exception_raise(const sdk_exception_t* exception, const char* file, int line);

////////////////////////////////////////////////////////////////////////////////
////
#ifdef WIN32
    extern sdk_size_t sdk_exception_g_index;
    void sdk_exception_init(void);
    void sdk_exception_push(sdk_exception_frame_t * fp);
    void sdk_exception_pop(void);
#endif


#ifdef WIN32
#define SDK_RAISE(e) sdk_exception_raise(&(e), __FILE__, __LINE__)

#define SDK_RERAISE sdk_exception_raise(sdk_exception_frame.exception, \
	sdk_exception_frame.file, sdk_exception_frame.line)

#define SDK_RETURN switch (sdk_exception_pop(),0) default: return

#define SDK_TRY do { \
	volatile int sdk_exception_flag; \
	sdk_exception_frame_t sdk_exception_frame; \
	if (sdk_exception_g_index == -1) \
		sdk_exception_init(); \
	sdk_exception_push(&sdk_exception_frame);  \
	sdk_exception_flag = setjmp(sdk_exception_frame.env); \
	if (sdk_exception_flag == kSDK_Exception_Entered) {

#define SDK_CATCH(e) \
		if (sdk_exception_flag == kSDK_Exception_Entered) sdk_exception_pop(); \
	} else if (sdk_exception_frame.exception == &(e)) { \
		sdk_exception_flag = kSDK_Exception_Handled;

#define SDK_ELSE \
		if (sdk_exception_flag == kSDK_Exception_Entered) sdk_exception_pop(); \
	} else { \
		sdk_exception_flag = kSDK_Exception_Handled;

#define SDK_FINALLY \
		if (sdk_exception_flag == kSDK_Exception_Entered) sdk_exception_pop(); \
	} { \
		if (sdk_exception_flag == kSDK_Exception_Entered) \
			sdk_exception_flag = kSDK_Exception_Finalized;

#define SDK_END_TRY \
		if (sdk_exception_flag == kSDK_Exception_Entered) sdk_exception_pop(); \
		} if (sdk_exception_flag == kSDK_Exception_Raised) SDK_RERAISE; \
} while (0);

#else
#define SDK_RAISE(e) sdk_exception_raise(&(e), __FILE__, __LINE__)
#define SDK_RERAISE sdk_exception_raise(sdk_exception_frame.exception, \
	sdk_exception_frame.file, sdk_exception_frame.line)
#define SDK_RETURN switch (sdk_exception_stack = sdk_exception_stack->prev,0) default: return
#define SDK_TRY do { \
	volatile int sdk_exception_flag; \
	sdk_exception_Frame sdk_exception_frame; \
	sdk_exception_frame.prev = sdk_exception_stack; \
	sdk_exception_stack = &sdk_exception_frame;  \
	sdk_exception_flag = setjmp(sdk_exception_frame.env); \
	if (sdk_exception_flag == kSDK_Exception_Entered) {
#define SDK_CATCH(e) \
		if (sdk_exception_flag == kSDK_Exception_Entered) sdk_exception_stack = sdk_exception_stack->prev; \
	} else if (sdk_exception_frame.exception == &(e)) { \
		sdk_exception_flag = kSDK_Exception_Handled;
#define SDK_ELSE \
		if (sdk_exception_flag == kSDK_Exception_Entered) sdk_exception_stack = sdk_exception_stack->prev; \
	} else { \
		sdk_exception_flag = kSDK_Exception_Handled;
#define SDK_FINALLY \
		if (sdk_exception_flag == kSDK_Exception_Entered) sdk_exception_stack = sdk_exception_stack->prev; \
	} { \
		if (sdk_exception_flag == kSDK_Exception_Entered) \
			sdk_exception_flag = kSDK_Exception_Finalized;
#define SDK_END_TRY \
		if (sdk_exception_flag == kSDK_Exception_Entered) sdk_exception_stack = sdk_exception_stack->prev; \
		} if (sdk_exception_flag == kSDK_Exception_Raised) SDK_RERAISE; \
} while (0)
#endif

#endif /* INCLUDED_SDK_EXCEPTION_H */
