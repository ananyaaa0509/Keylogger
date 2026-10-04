#ifndef LOGGER_H
#define LOGGER_H

typedef enum{
    KEY_NORMAL,
    KEY_SPACE,
    KEY_ENTER,
    KEY_BACKSPACE,
    KEY_SHIFT,
    KEY_ESCAPE

} KeyType;
typedef struct{
    KeyType type;
    char key;
    int ctrl;
    int shift;
    int alt;
} KeyEvent;

void log_key(KeyEvent event);
int start_session(void);
void end_session(void);
void flush_log(void);
#endif