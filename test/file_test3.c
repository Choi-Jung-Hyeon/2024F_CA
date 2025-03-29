#include <stdio.h>

int main() {

FILE *fp;
int n = 0x00208033;
fp = fopen("data.bin", "wb");

fwrite(&n, sizeof(n), 1, fp);

fclose(fp);

return 0;
}