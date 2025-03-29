#include <stdio.h>

int main() {
    FILE* fp;
    /*
    fp = fopen("test2.bin", "wb");
    int arr[5] = {1,2,3,4,5};
    fwrite(arr, sizeof(int), sizeof(arr)/sizeof(int), fp);
    fclose(fp);
    */
    char arr2[8];
    fp = fopen("data.bin", "rb");
    fread(arr2, 1, 8, fp);
    fclose(fp);

    for(int i = 0; i < 8; i++) {
        printf("%c ", (unsigned char)arr2[i]);
    }
    return 0;
}