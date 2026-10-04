#include<stdio.h>
#include "input.h"
#include "logger.h"
#include "platform/input_platform.h"
static int shift_active = 0;
KeyEvent create_event(KeyType type, char key, int ctrl, int shift, int alt){
    KeyEvent event;
    event.type=type;
    event.key=key;
    event.ctrl=ctrl;
    event.shift=shift;
    event.alt=alt;
    return event;
}
InputResult start_input(){
    int r=1;
    
    while(r){
        
        printf("Enter a key (esc to quit): ");

        KeyEvent event=get_event();
        
        if(is_exit_event(event)){
            r=0;
            continue;
        }
        process_event(event);
        
       
    }  
     return INPUT_EXIT_REQUESTED;
}
void process_event(KeyEvent event){
    log_key(event);
}
int is_exit_event(KeyEvent event){
    return event.type == KEY_ESCAPE;
}
