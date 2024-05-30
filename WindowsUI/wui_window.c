#include <wui_window.h>
#include <wui_macros.h>
#include <assert.h>
#include <sdk_hashtable.h>

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
        wui_window_message_handler_t handler = window->messageHandlerTable[message];
        if(handler){
            return handler(hwnd, message, wParam, lParam);
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

static BOOL wui_window__register(HINSTANCE hInstance, const char* name, WNDPROC WndProc)
{
    WNDCLASS WndClass;
    WndClass.style = CS_HREDRAW|CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = name;

    return RegisterClass(&WndClass)!=0;
}


static LRESULT wui_window__destroy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////


wui_err_t wui_window_init(wui_window_t* window, const char* name, HINSTANCE hInstance, WNDPROC WndProc)
{
    window->hInstance = hInstance;
    window->wndproc = (WndProc==0)?wui_window__wndproc:WndProc;
    window->hwnd = 0;

    for(uint16_t i=0; i< WUI_ARRAY_SIZE(window->messageHandlerTable); i++){
        window->messageHandlerTable[i] = 0;
    }
    wui_window_register_message_handler(window, WM_DESTROY, wui_window__destroy, 0);

    if(name){
        size_t name_size = strlen(name);
        name_size = WUI_MIN(name_size, WUI_ARRAY_SIZE(window->className)-1);
        memcpy(window->className, name, name_size);
        window->className[name_size]='\0';
    }

    if(wui_window__register(hInstance, window->className, window->wndproc)!=TRUE){
        return WUI_ERROR;
    }

    return WUI_EOK;
}

wui_err_t wui_window_destroy(wui_window_t * window){
    WINBOOL ret = CloseWindow(window->hwnd);
    return (ret==TRUE)?WUI_EOK:WUI_ERROR;
}

wui_err_t wui_window_create(wui_window_t * window){
    if(!window->hwnd){

        HWND hwnd = CreateWindow(window->className, window->className,
                                 WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT, CW_USEDEFAULT,
                                 CW_USEDEFAULT, CW_USEDEFAULT,
                                 NULL, NULL, window->hInstance, NULL);

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

    if(old_handler){
        *old_handler = window->messageHandlerTable[code];
    }
    window->messageHandlerTable[code] = handler;

    return WUI_EOK;
}


