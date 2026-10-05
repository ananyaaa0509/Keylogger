#include <stdio.h>
#include "logger.h"
#include "input.h"
#include "cipher.h"
int main(void) {
    if(!start_session()){
        return 1;
    }

    

    InputResult result=start_input();
    if(result==INPUT_EXIT_REQUESTED){
        end_session();
        int encrypted=cipher("keystrokes.log", "enc_keystrokes.log", "DAMN");
        if(encrypted){
            printf("encryption successful");
            remove("keystrokes.log");
    }
    else{
        printf("encryption failed");
    }
    }

    
    
}