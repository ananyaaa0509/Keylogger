#include <stdio.h>
#include <windows.h>    
#include "input_windows.h"

KeyEvent windows_get_event(void) {
    HANDLE input=GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD record;
    DWORD events_read;
    
    while(1){
        if (!ReadConsoleInput(input, &record, 1, &events_read)) {
            printf("Could not read console input.\n");
            continue;
        }
        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown){

            WORD vk = record.Event.KeyEvent.wVirtualKeyCode;
            char key = record.Event.KeyEvent.uChar.AsciiChar;
            DWORD state = record.Event.KeyEvent.dwControlKeyState;

            int ctrl = (state & LEFT_CTRL_PRESSED) || (state & RIGHT_CTRL_PRESSED);
            int shift = (state & SHIFT_PRESSED) != 0;
            int alt = (state & LEFT_ALT_PRESSED) || (state & RIGHT_ALT_PRESSED);

            if (vk == VK_SPACE) {
                return create_event(KEY_SPACE, 0, ctrl, shift, alt);
            }

            if (vk == VK_RETURN) {
                return create_event(KEY_ENTER, 0, ctrl, shift, alt);
            }

            if(vk==VK_BACK){
                return create_event(KEY_BACKSPACE, 0, ctrl, shift, alt);
            }

            if(vk==VK_SHIFT || vk==VK_RSHIFT){
                return create_event(KEY_SHIFT, 0, ctrl, shift, alt);
            }

            if (vk==VK_ESCAPE)
            {
                return create_event(KEY_ESCAPE, 0, ctrl, shift, alt);
            }
            

            if (key != 0) {
                return create_event(KEY_NORMAL, key, ctrl, shift, alt);
            }
        }
    }

}