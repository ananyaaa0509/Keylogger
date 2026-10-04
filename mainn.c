#include <stdio.h>
#include "logger.h"
#include "input.h"
int main(void) {
    if(!start_session()){
        return 1;
    }
    InputResult result=start_input();
    if(result==INPUT_EXIT_REQUESTED){
        end_session();
    }
    
    
}