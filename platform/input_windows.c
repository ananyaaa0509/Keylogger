#include <stdio.h>
#include <windows.h>    
#include "input_windows.h"
HHOOK hKeyboardHook = NULL;


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            int ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
            int shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
            int alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
            
            WORD vk = pKeyStruct->vkCode;
            KeyEvent ev;
            int valid_event=1;

            if (vk == VK_SPACE) {
                ev= create_event(KEY_SPACE, 0, ctrl, shift, alt);
            }

            else if (vk == VK_RETURN) {
                ev= create_event(KEY_ENTER, 0, ctrl, shift, alt);
            }

            else if(vk==VK_BACK){
                ev= create_event(KEY_BACKSPACE, 0, ctrl, shift, alt);
            }

            else if(vk==VK_SHIFT || vk==VK_RSHIFT){
                ev= create_event(KEY_SHIFT, 0, ctrl, shift, alt);
            }

            else if (vk==VK_ESCAPE)
            {
                ev= create_event(KEY_ESCAPE, 0, ctrl, shift, alt);
                process_event(ev);
                PostQuitMessage(0); 
            }
            

            else{
                BYTE keyboardState[256];
                GetKeyboardState(keyboardState);
                WORD asciiVal = 0;
                int conversionResult = ToAscii(vk, pKeyStruct->scanCode, keyboardState, &asciiVal, 0);
                
                if (conversionResult == 1) { // Single character translated successfully
                    char key = (char)asciiVal;
                    ev = create_event(KEY_NORMAL, key, ctrl, shift, alt); 
                }else{
                    valid_event=0;
                }
            }

            if (valid_event) {
                process_event(ev); // Connecting to your existing processing layer
            }

        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

InputResult windows_start_platform_input(void){
    HINSTANCE hInstance = GetModuleHandle(NULL);
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);

    if (hKeyboardHook == NULL) {
    return INPUT_ERROR;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hKeyboardHook);
    hKeyboardHook = NULL;

    return INPUT_EXIT_REQUESTED;
}
