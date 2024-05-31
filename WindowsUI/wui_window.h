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

#ifndef INCLUDED_SDK_HASHTABLE_H
#include <sdk_hashtable.h>
#endif /*INCLUDED_SDK_HASHTABLE_H*/

#ifndef INCLUDED_WINUSER_H
#define INCLUDED_WINUSER_H
#include <winuser.h>
#endif /*INCLUDED_WINUSER_H*/



////////////////////////////////////////////////////////////////////////////////
////
typedef LRESULT (*wui_window_message_handler_t)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef struct wui_window_s{
    HINSTANCE hInstance;
    WNDPROC wndproc;
    HWND hwnd;
    char className[WUI_NAME_SIZE];
    sdk_hashtable_t messageHandlerTable;
    ATOM register_atom;
}wui_window_t;

////////////////////////////////////////////////////////////////////////////////
////

typedef struct wui_window_on_char_s{
    HWND hwnd;
    UINT ch;
    int cRepeat;
}wui_window_on_char_t;
#define WUI_WM_CHAR_MSG(hwnd, message, wParam, lParam) \
    {hwnd, (TCHAR)(wParam), (int)(short)LOWORD(lParam)}


typedef struct wui_window_on_key_s{
    HWND hwnd;
    UINT vk;
    BOOL fDown;
    int cRepeat;
    UINT flags;
}wui_window_on_key_t;

#define WUI_WM_KEY_MSG(hwnd, message, wParam, lParam) \
    {hwnd, (UINT)(wParam),(message==WM_KEYDOWN)?TRUE:FALSE,(int)(short)LOWORD(lParam),(UINT)HIWORD(lParam)}


typedef struct wui_window_on_lbuttondown_s{
    HWND hwnd;
    BOOL fDoubleClick;
    int x;
    int y;
    UINT keyFlags;
}wui_window_on_lbuttondown_t;

#define WUI_WM_LBUTTONDOWN_MSG(hwnd, message, wParam, lParam) \
    {(hwnd),(WM_LBUTTONDBLCLK==message),(int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam),(UINT)(wParam)}


typedef struct wui_window_on_lbuttonup_s{
    HWND hwnd;
    int x;
    int y;
    UINT keyFlags;
}wui_window_on_lbuttonup_t;

#define WUI_WM_LBUTTONUP_MSG(hwnd, message, wParam, lParam) \
    {(hwnd),(int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam),(UINT)(wParam)}


typedef struct wui_window_on_mousemove_s{
    HWND hwnd;
    int x;
    int y;
    UINT keyFlags;
}wui_window_on_mousemove_t;

#define WUI_WM_MOUSEMOVE_MSG(hwnd, message, wParam, lParam) \
    {(hwnd),(int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam),(UINT)(wParam)}


typedef struct wui_window_on_syskey_s{
    HWND hwnd;
    UINT vk;
    BOOL fDown;
    int cRepeat;
    UINT flags;
}wui_window_on_syskey_t;

#define WUI_WM_SYSKEY_MSG(hwnd, message, wParam, lParam) \
    {hwnd, (UINT)(wParam),(message==WM_SYSKEYDOWN)?TRUE:FALSE,(int)(short)LOWORD(lParam),(UINT)HIWORD(lParam)}


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


wui_err_t wui_window_background_set(wui_window_t* window, HBRUSH background);
wui_err_t wui_window_cursor_set(wui_window_t* window, HCURSOR cursor);
wui_err_t wui_window_icon_set(wui_window_t* window, HICON icon);
wui_err_t wui_window_style_set(wui_window_t* window, UINT style);

#endif /*INCLUDED_WUI_WINDOW_H*/
