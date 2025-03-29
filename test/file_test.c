#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *in;
    int ch;

    if( (in = fopen("data.bin", "rb")) == NULL) {
        fputs("file open error", stderr);
        exit(1);
    }

    while( (ch = fgetc(in)) != EOF) {
        printf("%02X", ch);
    }

    fclose(in);
    return 0;
}