#include <stdio.h>

int write_file(char* filename, char* buf, int size) {
    FILE* fp;
    int write_len = 0;

    if((fp = fopen(filename, "wb")) == NULL) {
        printf("file open error '%s'\n", filename);
        return -1;
    }

    if((write_len = fwrite(buf, sizeof(char), size, fp)) < size) {
        fclose(fp);
        printf("write error '%s' '%d(%d)'\n", filename, size, write_len);
    }
    
    fclose(fp);
    return 0;
}

int read_file(char* filename, char *buf, int size) {
    FILE* fp;
    int read_len = 0;

    if((fp = fopen(filename, "rb")) == NULL) {
        printf("file open error '%s'\n", filename);
        return -1;
    }

    read_len = fread(buf, sizeof(char), size, fp);
    fclose(fp);
    return 0;
}

int main() {
    char filename[256] = "test2.bin";
    char write_data[20] = {0xFF, 0xFD, 0xFC, 0xFB, 0xFA, };
    char read_data[20] = {0x00, };

    if(write_file(filename, write_data, 5) == 0) {
        read_file(filename, read_data, 5);
        for(int i = 0; i < 5; i++) {
            printf("0x%X ", (unsigned char) read_data[i]);
        }
    } 
    return 0;
}