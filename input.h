#ifndef INPUT_H
#define INPUT_H
#include "logger.h"
typedef enum{
    INPUT_RUNNING,
    INPUT_EXIT_REQUESTED
} InputResult;
KeyEvent create_event(KeyType type, char key, int ctrl, int shift, int alt);
void process_event(KeyEvent event);
InputResult start_input(void);
int is_exit_event(KeyEvent event);
#endif