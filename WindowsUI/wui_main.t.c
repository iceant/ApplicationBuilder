#include <wui_main.t.h>
#include <wui_window.h>

////////////////////////////////////////////////////////////////////////////////
////

static wui_window_t MainFrame;
char Directions[100];


static LRESULT wui_main__on_create(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)(lParam);
    strcpy(Directions, "Try resizing this window");
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);


    return WUI_WINDOW_SUCCESS;
}

static LRESULT wui_main__on_destroy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    PostQuitMessage(0);
    return WUI_WINDOW_SUCCESS;
}

static LRESULT wui_main__on_paint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    PAINTSTRUCT PaintStruct;
    RECT Rect;

    HDC PaintDC = BeginPaint(hwnd, &PaintStruct);

    SetBkMode(PaintDC, TRANSPARENT);

    TextOut(PaintDC, 20, 20, Directions, lstrlen(Directions));

    GetClientRect(hwnd, &Rect);

    DrawText(PaintDC, "The middle of the road", -1, &Rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

    EndPaint(hwnd, &PaintStruct);

    return WUI_WINDOW_SUCCESS;
}

static BOOL OnCrate(HWND hwnd, CREATESTRUCT FAR * lpCreateStruct){
    strcpy(Directions, "Try resizing this window.");
    return TRUE;
}

static void OnDestroy(HWND hwnd){
    PostQuitMessage(0);
}


static LRESULT OnPaint(HWND hwnd){
    PAINTSTRUCT PaintStruct;
    RECT Rect;

    HDC PaintDC = BeginPaint(hwnd, &PaintStruct);

    SetBkMode(PaintDC, TRANSPARENT);

    TextOut(PaintDC, 20, 20, Directions, lstrlen(Directions));

    GetClientRect(hwnd, &Rect);

    DrawText(PaintDC, "The middle of the road", -1, &Rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);



    EndPaint(hwnd, &PaintStruct);

    return WUI_WINDOW_SUCCESS;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message) {
        HANDLE_MSG(hwnd, WM_CREATE, OnCrate);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        default:
            DefWindowProc(hwnd, message, wParam, lParam);
    }
}
////////////////////////////////////////////////////////////////////////////////
////


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){

    MSG Msg;

    if(!hPrevInstance){
        if(wui_window_init(&MainFrame, "Window1", hInstance, 0)!=WUI_EOK){
            return FALSE;
        }
    }

    wui_window_register_message_handler(&MainFrame, WM_PAINT, wui_main__on_paint, 0);
    wui_window_register_message_handler(&MainFrame, WM_DESTROY, wui_main__on_destroy, 0);
    wui_window_register_message_handler(&MainFrame, WM_SHOWWINDOW, wui_main__on_create, 0);

    wui_window_create(&MainFrame);

    if(wui_window_show(&MainFrame, nCmdShow)!=WUI_EOK){
        return FALSE;
    }

    while(GetMessage(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    
    wui_window_destroy(&MainFrame);
    
    return Msg.wParam;
}

