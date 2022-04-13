#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>
#include <fstream>
#include <assert.h>
#include <string.h>

/// Описание строки текста.
struct Line {
    char *str;  ///< Строка текста.
    int sizeStr;///< Размер строки.
};

int getFileSize(FILE *finput);
void *readFile(FILE *finput, char *str, int numberBytesSize);
int countNumberLines(char *str, int numberBytesFile);
void splitToLines(Line *lines, int linesCount, char *str);
void *fillStructLine(const char* nameFile, int *linesCount, char **ptrStr, Line **ptrLines);
void writeFile(Line *lines, int linesCount, FILE *foutput);

#endif // FILE_OPERATIONS_H_
