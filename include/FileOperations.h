#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include <stdio.h>
#include <fstream>
#include <assert.h>
#include <string.h>

/// Описание строки текста.
struct Line {
    const char *str; ///< Строка текста.
    int sizeStr;     ///< Размер строки.
};

ssize_t getFileSize( FILE *finput );
void *readFile( FILE *finput, char *str, ssize_t numberBytesFile );
size_t countNumberLines( char *str, ssize_t numberBytesFile );
void splitToLines( Line *lines, int linesCount, char *str );
void *fillStructLine( const char* nameFile, size_t *linesCount, char **ptrStr, Line **ptrLines );
void writeFile( Line *lines, int linesCount, FILE *foutput );

#endif // FILE_OPERATIONS_H_
