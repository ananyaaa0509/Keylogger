#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "input_linux.h"
KeyEvent linux_get_event(void){
    struct termios old_settings;
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings=old_settings;
    new_settings.c_lflag&= ~(ICANON |ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    char key;
    read(STDIN_FILENO, &key, 1);
    KeyType type=KEY_NORMAL;

    if(key==' '){
        type=KEY_SPACE;
    }

    if(key=='\n'){
        type=KEY_ENTER;
    }

    if(key==127){
        type=KEY_BACKSPACE;
    }
    else if (key == 27) {
        type = KEY_ESCAPE;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    return create_event(type, key, 0, 0, 0);
}