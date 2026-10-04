#include <stdio.h>
#include <time.h>
#include "logger.h"
static FILE *log_file = NULL;
static unsigned long total_keystrokes=0;
static unsigned long total_characters=0;
static unsigned long key_frequency[256]={0};
static unsigned long space_count = 0;
static unsigned long enter_count = 0;
static unsigned long backspace_count = 0;
static unsigned long shift_count = 0;
static time_t session_start_time;

int start_session(void) {
    log_file = fopen("keystrokes.log", "a");

    if (log_file == NULL) {
        printf("Could not open log file\n");
        return 0;
    }
    time_t now = time(NULL);

    session_start_time = now;
    total_keystrokes = 0;
    for(int i=0;i<256;i++){
        key_frequency[i]=0;
    }
    space_count=0;
    enter_count=0;
    backspace_count=0;
    shift_count=0;

    
    struct tm *local_time = localtime(&now);

    fprintf(
        log_file,
        "\n--- Session started: %02d-%02d-%02d: %02d/%02d/%04d ---\n",
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec,
        local_time->tm_mday,
        local_time->tm_mon + 1,
        local_time->tm_year + 1900
        
    );
    return 1;
}
void log_key(KeyEvent event){
    if(log_file==NULL){
        printf("could not open log file\n");
        return;
    }

    time_t now=time(NULL);
    struct tm *local_time=localtime(&now);

    switch(event.type){
        case KEY_NORMAL:
            total_keystrokes++;
            total_characters++;
            key_frequency[(unsigned char)event.key]++;
            fprintf(log_file, "[%02d-%02d-%02d: %02d/%02d/%04d] %c\n",
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec,
                local_time->tm_mday,
                local_time->tm_mon + 1,
                local_time->tm_year + 1900,
                event.key);
            break;
        case KEY_SPACE:
            total_keystrokes++;
            total_characters++;
            space_count++;
            fprintf(log_file, "[%02d-%02d-%02d: %02d/%02d/%04d] [SPACE]\n",
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec,
                local_time->tm_mday,
                local_time->tm_mon + 1,
                local_time->tm_year + 1900
                );
            break;
        case KEY_BACKSPACE:
            total_keystrokes++;
            backspace_count++;
            fprintf(log_file, "[%02d-%02d-%02d: %02d/%02d/%04d] [BACKSPACE]\n",
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec,
                local_time->tm_mday,
                local_time->tm_mon + 1,
                local_time->tm_year + 1900
                );
            break;
        case KEY_ENTER:
            total_keystrokes++;
            total_characters++;
            enter_count++;
            fprintf(log_file, "[%02d-%02d-%02d: %02d/%02d/%04d] [ENTER]\n",
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec,
                local_time->tm_mday,
                local_time->tm_mon + 1,
                local_time->tm_year + 1900
                );
            break;
        case KEY_SHIFT:
            total_keystrokes++;
            shift_count++;
            fprintf(log_file, "[%02d-%02d-%02d: %02d/%02d/%04d] [SHIFT]\n",
                local_time->tm_hour,
                local_time->tm_min,
                local_time->tm_sec,
                local_time->tm_mday,
                local_time->tm_mon + 1,
                local_time->tm_year + 1900
                );
            break;
    }
    

}
void flush_log(void){
    if (log_file!=NULL){
        fflush(log_file);
    }
    
}

void show_top_keys(void){
    int used[260]={0};
    unsigned long special_count[4]={
        space_count,
        enter_count,
        shift_count,
        backspace_count
    };
    const char *special_names[4] = {
        "[SPACE]",
        "[ENTER]",
        "[BACKSPACE]",
        "[SHIFT]"
    };
    unsigned long counts[260] = {0};
    const char *names[260] = {0};
    for(int i=0;i<256;i++){
        counts[i]=key_frequency[i];
    }
    for(int i=0;i<4;i++){
        counts[256+i]=special_count[i];
        names[256+i]=special_names[i];
    }
    for(int i=0;i<5;i++){
        int best_key=-1;
        unsigned long int best_count=0;
        for(int j=0;j<260;j++){
            if (!used[j] && key_frequency[j] > best_count){
                best_count=key_frequency[j];
                best_key=j;
            }
        }
        if (best_key == -1 || best_count==0) {
        break;
        }
        if(best_key<256){
            printf("%d.'%c'-%lu\n", i+1, (char)best_key, best_count);
        }
        else{
            printf("%d.%s-%lu\n", i+1, names[best_key], best_count);
        }
        used[best_key]=1;
    }   
}

void end_session(void) {
    time_t end_time = time(NULL);
    double runtime = difftime(end_time, session_start_time);
    double WPM=total_characters/5.0*(60.0/runtime);
    double error_rate=backspace_count*100/total_characters;
    
     if (log_file != NULL) {
        fprintf(
            log_file,
            "--- Session ended: %lu keystrokes, %.0f seconds, %.2f WPM ---\n\n",
            total_keystrokes,
            runtime,
            WPM
        );
        show_top_keys();
        flush_log();
        fclose(log_file);
        log_file=NULL;
        
     }

    printf("\nSession ended.\n");
    printf("Total keystrokes: %lu\n", total_keystrokes);
    printf("Active runtime: %.0f seconds\n", runtime);
    printf("error rate: %.2f %\n", error_rate);
    printf("WPM: %.2f", WPM);
}

