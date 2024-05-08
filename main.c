#include <stdio.h>
#include <stdlib.h>

#define CHAR_PER_LINE 16
#define MAX_BLOC_PER_LINE (CHAR_PER_LINE / 2)

void printSeparator() {
    putchar(' ');
}

void printLineNumber(unsigned int lineNumber) {
    printf("%08x:", lineNumber * 16);
}

void printHexDump(unsigned char line[16], unsigned int lineLength) {
    unsigned int parity = 0; 

    for (int i = 0; i < CHAR_PER_LINE; i++) {
        if (i < lineLength) {
            printf("%02x", line[i]);
        } else {
            putchar(' ');
            putchar(' ');
        }

        if (parity) {
            putchar(' ');
        }

        parity = (parity + 1) & 1;
    }
}

void printAsciiDump(unsigned char line[16], unsigned int lineLength) {
    for (int i = 0; i < lineLength && i < CHAR_PER_LINE; i++) {
        if (0x20 <= line[i] && line[i] <= 0x7E) {
            putchar(line[i]);
        } else {
            putchar('.');
        }
    }
}

void printLine(unsigned int lineNumber, unsigned char line[16], unsigned int lineLength) {
    printLineNumber(lineNumber);
    printSeparator();
    printHexDump(line, lineLength);
    printSeparator();
    printAsciiDump(line, lineLength);
    printf("\n");
}

int dumpLine(FILE *file, unsigned char line[16]) {
    unsigned int lineLength = 0;
    int c;

    while (lineLength < CHAR_PER_LINE && !feof(file)) {
        c = fgetc(file);

        if (c == EOF) {
            break;
        }

        line[lineLength] = (unsigned char) c;
        lineLength++;

    }

    return lineLength;
}

void dumpFile(FILE *file) {
    unsigned int lineNumber = 0;
    unsigned int lineLength;

    while (!feof(file)) {
        unsigned char line[CHAR_PER_LINE] = {0};
        lineLength = dumpLine(file, line);

        if (!lineLength) {
            continue;
        }

        printLine(lineNumber, line, lineLength);
        lineNumber++;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        puts("usage : ./xxd <filename>");
        return -1;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "rb");

    if (!file) {
        puts("Error opening file");
        return -1;
    }

    dumpFile(file);

    fclose(file);
    file = NULL;

    return 0;
}
