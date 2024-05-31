#include <wui_window.h>
#include <wui_macros.h>
#include <assert.h>
#include <sdk_hashtable.h>
#include <strsafe.h>

////////////////////////////////////////////////////////////////////////////////
////

static sdk_hashtable_t wui_window__instances;


SDK_C_CONSTRUCTOR(wui_window__module_init){
    sdk_hashtable_init(&wui_window__instances, WUI_WINDOW_INSTANCE_HASH_TABLE_CAPACITY
            , wui_hash_uint_key_ops, wui_hash_uintptr_val_ops);
}

static LRESULT CALLBACK wui_window__wndproc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    wui_window_t* window = sdk_hashtable_get(&wui_window__instances, hwnd);
    if(window){
        wui_window_message_handler_t handler = sdk_hashtable_get(&window->messageHandlerTable, (const void*)message);
        if(handler){
            return handler(hwnd, message, wParam, lParam);
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

static BOOL wui_window__register(HINSTANCE hInstance, const char* name, WNDPROC WndProc)
{
    WNDCLASSEX WndClass;
    WndClass.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = name;
    WndClass.cbSize = sizeof(WndClass);
    WndClass.hIconSm = LoadImage(hInstance, // small class icon
                                 MAKEINTRESOURCE(5),
                                 IMAGE_ICON,
                                 GetSystemMetrics(SM_CXSMICON),
                                 GetSystemMetrics(SM_CYSMICON),
                                 LR_DEFAULTCOLOR);

    return RegisterClassEx(&WndClass)!=0;
}


static LRESULT wui_window__destroy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 0;
}

static void wui_window__error_exit(LPCTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code
    
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();
    
    FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );
    
    // Display the error message and exit the process
    
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                                      (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
                    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                    TEXT("%s failed with error %d: %s"),
                    lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
    
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

////////////////////////////////////////////////////////////////////////////////
////

wui_err_t wui_window_background_set(wui_window_t* window, HBRUSH background)
{
    if(window->hwnd){
        SetClassLongPtr(window->hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)background);
    }
    return WUI_EOK;
}

wui_err_t wui_window_cursor_set(wui_window_t* window, HCURSOR cursor)
{
    if(window->hwnd){
        SetClassLongPtr(window->hwnd, GCLP_HCURSOR, (LONG_PTR)cursor);
    }
    return WUI_EOK;
}

wui_err_t wui_window_icon_set(wui_window_t* window, HICON icon)
{
    if(window->hwnd){
        SetClassLongPtr(window->hwnd, GCLP_HICON, (LONG_PTR)icon);
    }
    return WUI_EOK;
}

wui_err_t wui_window_style_set(wui_window_t* window, UINT style)
{
    WNDCLASS WndClass;
    GetClassInfo(window->hInstance, window->className, &WndClass);
    WndClass.style = WndClass.style|style;
    return (RegisterClass(&WndClass)!=0)?WUI_EOK:WUI_ERROR;
}


wui_err_t wui_window_init(wui_window_t* window, const char* name, HINSTANCE hInstance, WNDPROC WndProc)
{
    window->hInstance = hInstance;
    window->wndproc = (WndProc==0)?wui_window__wndproc:WndProc;
    window->hwnd = 0;
    
    sdk_hashtable_init(&window->messageHandlerTable, 10, wui_hash_uint_key_ops, wui_hash_uintptr_val_ops);
    
    wui_window_register_message_handler(window, WM_DESTROY, wui_window__destroy, 0);

    if(name){
        size_t name_size = strlen(name);
        name_size = WUI_MIN(name_size, WUI_ARRAY_SIZE(window->className)-1);
        memcpy(window->className, name, name_size);
        window->className[name_size]='\0';
    }
    
    wui_window__register(hInstance, window->className, window->wndproc);
    return WUI_EOK;
}

wui_err_t wui_window_destroy(wui_window_t * window){
    WINBOOL ret = CloseWindow(window->hwnd);
    return (ret==TRUE)?WUI_EOK:WUI_ERROR;
}

wui_err_t wui_window_create(wui_window_t * window, int x, int y, int width, int height){
    if(!window->hwnd){

        HWND hwnd = CreateWindow(window->className, window->className,
                                 WS_OVERLAPPEDWINDOW, // top-level window
                                 x /*CW_USEDEFAULT*/,       // default horizontal position
                                 y /*CW_USEDEFAULT*/,       // default vertical position
                                 width /*CW_USEDEFAULT*/,       // default width
                                 height /*CW_USEDEFAULT*/,       // default height
                                 (HWND) NULL,         // no owner window
                                 (HMENU) NULL,        // use class menu
                                 window->hInstance,           // handle to application instance
                                 (LPVOID) NULL);      // no window-creation data

        if(hwnd==NULL){
            return WUI_ERROR;
        }

        window->hwnd = hwnd;

        sdk_hashtable_put(&wui_window__instances, hwnd, window);
    }
    return WUI_EOK;
}

wui_err_t wui_window_show(wui_window_t * window, int nCmdShow){
    ShowWindow(window->hwnd, nCmdShow);
    UpdateWindow(window->hwnd);
    return WUI_EOK;
}

wui_err_t wui_window_register_message_handler(wui_window_t * window, uint16_t code, wui_window_message_handler_t handler
        , wui_window_message_handler_t* old_handler)
{
    assert(window);
    
    wui_window_message_handler_t old_msg_handler = sdk_hashtable_get(&window->messageHandlerTable, (const void*)code);
    
    sdk_hashtable_put(&window->messageHandlerTable, (const void*)code, handler);

    if(old_handler){
        *old_handler = old_msg_handler;
    }
    
    return WUI_EOK;
}


