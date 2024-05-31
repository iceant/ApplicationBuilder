#include <wui_keymous.t.h>
#include <wui_window.h>
#include <stdio.h>
#include <wui_macros.h>
////////////////////////////////////////////////////////////////////////////////
////

static wui_window_t MainFrame;
static int XVal = 10;
static int YVal = 30;
static HBRUSH BackgroundColor=0;
////////////////////////////////////////////////////////////////////////////////
////

static LRESULT MainFrame_OnChar(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    wui_window_on_char_t OnCharMsg = WUI_WM_CHAR_MSG(hwnd, message, wParam, lParam);
    
    char S[100];
    
    HDC DC = GetDC(hwnd);
    int size = snprintf(S, WUI_ARRAY_SIZE(S), "WM_CHAR == > Ch = %c, cRepeat= %d   ", OnCharMsg.ch, OnCharMsg.cRepeat);
    TextOut(DC, XVal, YVal+20, S, size);
    
    ReleaseDC(hwnd, DC);
    
    return WUI_WINDOW_SUCCESS;
}

static LRESULT MainFrame_OnKey(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    wui_window_on_key_t OnKeyMsg = WUI_WM_KEY_MSG(hwnd, message, wParam, lParam);
    
    char S[100];
    HDC DC = GetDC(hwnd);
    int size = 0;
    
    if(OnKeyMsg.fDown){
        size = snprintf(S, WUI_ARRAY_SIZE(S) , "WM_KEYDOWN == > vk = %d fDown=%s, cRepeat = %d flags = %d      "
                        , OnKeyMsg.vk
                        , OnKeyMsg.fDown?"T":"F", OnKeyMsg.cRepeat, OnKeyMsg.flags);
    }else{
        size = snprintf(S, WUI_ARRAY_SIZE(S) , "WM_KEYUP == > vk = %d fDown=%s, cRepeat = %d flags = %d      "
                , OnKeyMsg.vk
                , OnKeyMsg.fDown?"T":"F", OnKeyMsg.cRepeat, OnKeyMsg.flags);
    }
    SetBkColor(DC, GetSysColor(COLOR_WINDOW));
    TextOut(DC, XVal, YVal + 40, S, size);
    
    ReleaseDC(hwnd, DC);
    return WUI_WINDOW_SUCCESS;
}

static LRESULT MainFrame_OnLButton(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char S[100];
    HDC DC = GetDC(hwnd);
    int size = 0;
    
    if(message==WM_LBUTTONDOWN || message==WM_LBUTTONDBLCLK){
        wui_window_on_lbuttondown_t Msg = WUI_WM_LBUTTONDOWN_MSG(hwnd, message, wParam, lParam);
        
        
        if(Msg.fDoubleClick){
            size = snprintf(S, WUI_ARRAY_SIZE(S) , "WM_LBUTTONDBLCLK == > Db = %s x = %d  y = %d  Flags = %d        "
                    , Msg.fDoubleClick?"T":"F"
                    , Msg.x
                    , Msg.y
                    , Msg.keyFlags
            );
        }else{
            size = snprintf(S, WUI_ARRAY_SIZE(S) , "WM_LBUTTONDOWN == > Db = %s x = %d  y = %d  Flags = %d        "
                    , Msg.fDoubleClick?"T":"F"
                    , Msg.x
                    , Msg.y
                    , Msg.keyFlags
            );
        }
        
        SetBkColor(DC, GetSysColor(COLOR_WINDOW));
        TextOut(DC, XVal, YVal + 100, S, size);
        
    }else if(message == WM_LBUTTONUP){
        wui_window_on_lbuttonup_t Msg = WUI_WM_LBUTTONUP_MSG(hwnd, message, wParam, lParam);
        size = snprintf(S, WUI_ARRAY_SIZE(S) , "WM_LBUTTONUP == > x = %d  y = %d  Flags = %d                       "
                , Msg.x
                , Msg.y
                , Msg.keyFlags
        );
        
        SetBkColor(DC, GetSysColor(COLOR_WINDOW));
        TextOut(DC, XVal, YVal + 120, S, size);
    }
    

    
    ReleaseDC(hwnd, DC);
    return WUI_WINDOW_SUCCESS;
}


static LRESULT MainFrame_OnMouseMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char S[100];
    HDC DC = GetDC(hwnd);
    int size = 0;
    
    wui_window_on_mousemove_t Msg = WUI_WM_MOUSEMOVE_MSG(hwnd, message, wParam, lParam);
    
    size = snprintf(S, WUI_ARRAY_SIZE(S) , "WM_MOUSEMOVE == > x = %d  y = %d  Flags = %d        "
            , Msg.x
            , Msg.y
            , Msg.keyFlags
    );
    if((Msg.keyFlags & MK_CONTROL)==MK_CONTROL){
        SetTextColor(DC, RGB(0, 0, 255));
    }
    if((Msg.keyFlags & MK_LBUTTON)==MK_LBUTTON){
        SetTextColor(DC, RGB(0, 255, 0));
    }
    if((Msg.keyFlags & MK_RBUTTON)==MK_RBUTTON){
        SetTextColor(DC, RGB(255, 0, 0));
    }
    if((Msg.keyFlags & MK_SHIFT)==MK_SHIFT){
        SetTextColor(DC, RGB(255, 0, 255));
    }
    
    SetBkColor(DC, GetSysColor(COLOR_WINDOW));
    TextOut(DC, XVal, YVal + 80, S, size);
    
    ReleaseDC(hwnd, DC);
    
    return WUI_WINDOW_SUCCESS;
}

static LRESULT MainFrame_OnPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT PaintStruct;
    RECT Rect;
    static char* Message[]={
            "WM_CHAR",
            "WM_KEY",
            "WM_SYSKEY",
            "WM_MOUSEMOVE",
            "WM_MOUSEDOWN",
            "WM_MOUSEUP",
    };
    
    HDC PaintDC = BeginPaint(hwnd, &PaintStruct);
    
    SetBkColor(PaintDC, GetSysColor(COLOR_WINDOW));
    HFONT OldFont = SelectFont(PaintDC, GetStockObject(OEM_FIXED_FONT));
    GetClientRect(hwnd, &Rect);
    Rect.top = 5;
    DrawText(PaintDC, "MOUSE AND KEYBOARD DEMONSTRATION", -1, &Rect, DT_CENTER);
    Rect.top = 20;
    Rect.bottom = 40;
    DrawText(PaintDC, "(Try experimenting with the mouse and keyboard)", -1, &Rect, DT_CENTER);
    SelectFont(PaintDC, OldFont);
    for(int i=0; i<6; i++){
        TextOut(PaintDC, XVal, YVal + (20 * (i+1)), Message[i], strlen(Message[i]));
    }
    
    EndPaint(hwnd, &PaintStruct);
    
    return WUI_WINDOW_SUCCESS;
}



static LRESULT MainFrame_OnSysKey(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT PaintStruct;
    RECT Rect;
    char S[100];
    int size;
    HDC DC = GetDC(hwnd);
    
    wui_window_on_syskey_t Msg = WUI_WM_SYSKEY_MSG(hwnd, message, wParam, lParam);
    if(Msg.fDown){
        size = snprintf(S, WUI_ARRAY_SIZE(S), "WM_SYSKEYDOWN == > vk = %d  fDown=%s  cRepeat = %d  flags = %d        "
            , Msg.vk
            , Msg.fDown?"T":"F"
            , Msg.cRepeat
            , Msg.flags
        );
        
        TextOut(DC, XVal, YVal + 60 , S, size);
        FORWARD_WM_SYSKEYDOWN(hwnd, Msg.vk, Msg.cRepeat, Msg.flags, DefWindowProc);
    }else{
        size = snprintf(S, WUI_ARRAY_SIZE(S), "WM_SYSKEYUP == > vk = %d  fDown=%s  cRepeat = %d  flags = %d        "
                , Msg.vk
                , Msg.fDown?"T":"F"
                , Msg.cRepeat
                , Msg.flags
        );
        
        TextOut(DC, XVal, YVal + 60 , S, size);
        FORWARD_WM_SYSKEYDOWN(hwnd, Msg.vk, Msg.cRepeat, Msg.flags, DefWindowProc);
    }
    
    ReleaseDC(hwnd, DC);
    
    return WUI_WINDOW_SUCCESS;
}

static LRESULT MainFrame_OnShow(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(!BackgroundColor){
        BackgroundColor = CreateSolidBrush(RGB(60, 60, 60));
    }
    return WUI_WINDOW_SUCCESS;
}

static LRESULT MainFrame_OnDestroy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(BackgroundColor){
        DeleteObject(BackgroundColor);
    }
    PostQuitMessage(0);
    return WUI_WINDOW_SUCCESS;
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
    wui_window_style_set(&MainFrame, CS_DBLCLKS);
    
    if(!BackgroundColor){
        BackgroundColor = CreateSolidBrush(RGB(125, 125, 125));
    }
    
    wui_window_register_message_handler(&MainFrame, WM_CHAR, MainFrame_OnChar, 0);
    wui_window_register_message_handler(&MainFrame, WM_KEYDOWN, MainFrame_OnKey, 0);
    wui_window_register_message_handler(&MainFrame, WM_KEYUP, MainFrame_OnKey, 0);
    wui_window_register_message_handler(&MainFrame, WM_LBUTTONDOWN, MainFrame_OnLButton, 0);
    wui_window_register_message_handler(&MainFrame, WM_LBUTTONDBLCLK, MainFrame_OnLButton, 0);
    wui_window_register_message_handler(&MainFrame, WM_LBUTTONUP, MainFrame_OnLButton, 0);
    wui_window_register_message_handler(&MainFrame, WM_MOUSEMOVE, MainFrame_OnMouseMove, 0);
    wui_window_register_message_handler(&MainFrame, WM_PAINT, MainFrame_OnPaint, 0);
    wui_window_register_message_handler(&MainFrame, WM_SYSKEYUP, MainFrame_OnSysKey, 0);
    wui_window_register_message_handler(&MainFrame, WM_SYSKEYDOWN, MainFrame_OnSysKey, 0);
    wui_window_register_message_handler(&MainFrame, WM_SHOWWINDOW, MainFrame_OnShow, 0);
    wui_window_register_message_handler(&MainFrame, WM_DESTROY, MainFrame_OnDestroy, 0);
    
    wui_window_create(&MainFrame, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400);
    
    wui_window_icon_set(&MainFrame, LoadIcon(NULL, IDI_QUESTION));
    wui_window_cursor_set(&MainFrame, LoadCursor(NULL, IDC_HAND));

    wui_window_background_set(&MainFrame, BackgroundColor);
  
    
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

