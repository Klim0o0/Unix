#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "fcntl.h"


long getFileSize(int fp) {
    long fileSize = lseek(fp, 0L, SEEK_END);
    lseek(fp, 0L, SEEK_SET);
    return fileSize;
}

bool isZeroBlock(const char buffer[], int bufferLen) {
    for (int i = 0; i < bufferLen; i++) {
        if ((int) buffer[i] != 0x00)
            return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    int blockSize = 4096;

    int j;
    while ((j = getopt(argc, argv, "b:")) != -1) {
        if (j == 'b') {
            blockSize = atoi(optarg);
            if (blockSize <= 0) {
                printf("ERROR: Buffer size les then 0");
                return 1;
            }
        }
    }

    if (argv[optind] == NULL) {
        printf("Error: No output file");
        return 1;
    }


    char* fileA = argv[optind];
    char* fileB = argv[optind + 1];

    int fa, fb;
    if (fileB == NULL) {
        fa = STDIN_FILENO;
        fb = open(fileA, O_WRONLY | O_CREAT, 0640);
    } else {
        fa = open(fileA, O_RDONLY);
        fb = open(fileB, O_WRONLY | O_CREAT, 0640);
    }


    char buffer[blockSize];


    int readed = read(fa, buffer, blockSize);
    while (readed) {
        if (isZeroBlock(buffer, readed))
            lseek(fb, blockSize, SEEK_CUR);
        else
            write(fb, buffer, blockSize);
        readed = read(fa, buffer, blockSize);
    }

    close(fb);
    close(fa);
    return 0;
}
