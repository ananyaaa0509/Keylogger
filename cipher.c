#include <stdio.h>
#include <string.h>
#include "cipher.h"
int cipher(const char *input_file, const char *output_file,const char *key ){
    FILE *og=fopen(input_file, "rb");
    if (og == NULL) {
        printf("Could not open input file.\n");
        return 0;
    }

    FILE *enc=fopen(output_file, "wb");
    if (enc == NULL) {
        printf("Could not open output file.\n");
        fclose(og);
        return 0;
    }
    int len=strlen(key);
    int  key_index=0;
    int a;

    while((a=fgetc(og))!=EOF){
        
        int c=a^key[key_index];

        fputc(c, enc);
        key_index=(key_index+1)%len;
        
    }
    return 1;
    
}