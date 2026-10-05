#include<stdio.h>
#include "input.h"
#include "logger.h"
#include "platform/input_platform.h"

KeyEvent create_event(KeyType type, char key, int ctrl, int shift, int alt){
    KeyEvent event;
    event.type=type;
    event.key=key;
    event.ctrl=ctrl;
    event.shift=shift;
    event.alt=alt;
    return event;
}
InputResult start_input(void){
    return start_platform_input();
}
void process_event(KeyEvent event){
    log_key(event);
}
int is_exit_event(KeyEvent event){
    return event.type == KEY_ESCAPE;
}
