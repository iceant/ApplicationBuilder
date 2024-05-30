#ifndef INCLUDED_WUI_WINDOW_H
#define INCLUDED_WUI_WINDOW_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_WUI_TYPES_H
#include <wui_types.h>
#endif /*INCLUDED_WUI_TYPES_H*/

#ifndef INCLUDED_WUI_ERRORS_H
#include <wui_errors.h>
#endif /*INCLUDED_WUI_ERRORS_H*/

#ifndef INCLUDED_WUI_HASH_H
#include <wui_hash.h>
#endif /*INCLUDED_WUI_HASH_H*/


////////////////////////////////////////////////////////////////////////////////
////
typedef LRESULT (*wui_window_message_handler_t)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef struct wui_window_s{
    HINSTANCE hInstance;
    WNDPROC wndproc;
    HWND hwnd;
    char className[WUI_NAME_SIZE];
    wui_window_message_handler_t messageHandlerTable[UINT16_MAX];
}wui_window_t;

////////////////////////////////////////////////////////////////////////////////
////
#define WUI_WINDOW_SUCCESS (0)
#define WUI_WINDOW_FAILED  (-1)



////////////////////////////////////////////////////////////////////////////////
////

wui_err_t wui_window_init(wui_window_t* window, const char* name, HINSTANCE hInstance, WNDPROC WndProc);

wui_err_t wui_window_create(wui_window_t * window);

wui_err_t wui_window_destroy(wui_window_t * window);

wui_err_t wui_window_show(wui_window_t * window, int nCmdShow);

wui_err_t wui_window_register_message_handler(wui_window_t * window, uint16_t code, wui_window_message_handler_t handler
                                              , wui_window_message_handler_t* old_handler);

#endif /*INCLUDED_WUI_WINDOW_H*/
