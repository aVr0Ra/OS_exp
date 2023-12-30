#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *src, *dst;
    char buffer[1024];
    size_t bytes;

    if (argc != 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    src = fopen(argv[1], "rb");
    if (src == NULL) {
        perror("Error opening source file");
        return 1;
    }

    dst = fopen(argv[2], "wb");
    if (dst == NULL) {
        perror("Error opening destination file");
        fclose(src);
        return 1;
    }

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }

    fclose(src);
    fclose(dst);

    printf("File copied successfully.\n");
    return 0;
}
