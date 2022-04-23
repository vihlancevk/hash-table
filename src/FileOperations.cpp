// ToDo:
// 1) возвращение кода ошибки
// 2) <errno.h> - прочитать про обработку ошибок

#include <ctype.h>
#include "../include/FileOperations.h"

const char *SEPARATION_SYMBOLS = " .,!?;:-'`()<>{}[]/|&*#%$~_\"\n\t";

static void  moveToNextLine( FILE *foutput );

//================================================================================
//! @brief Функция подсчёта размера файла в байтах.
//!
//! @param [in] finput файл, размер кототого считается.
//!
//! @warning В случае ошибки при работе с файлом будет выведено
//!          соответствующее сообщение и завершена работа программы.
//!
//! @return Размер файла в байтах.
//--------------------------------------------------------------------------------

ssize_t getFileSize( FILE *finput )
{
    assert( finput != nullptr );

    if ( fseek( finput, 0, SEEK_END ) )
    {
        fprintf( stderr, "Error fseek!\n" );
        return -1;
    }

    ssize_t numberBytesFile = ftell(finput);
    if ( numberBytesFile == -1L )
    {
        fprintf( stderr, "Error ftell!\n" );
        return -1;
    }

    rewind( finput );

    return numberBytesFile;
}

//================================================================================
//! @brief Функция считывания файла в буфер.
//!
//! @param [in] finput файл, данные которого считываются.
//! @param [out] str буфер, в который происходит считывание.
//! @param [in] numberBytesFile размер файла finput в байтах.
//!
//! @warning В случае ошибки при работе с файлом будет выведено
//!          соответствующее сообщение и завершена работа программы.
//!
//! @return Указатель на буфер.
//--------------------------------------------------------------------------------

void *readFile( FILE *finput, char *str, ssize_t numberBytesFile )
{
    assert( finput != nullptr );
    assert( str    != nullptr );
    assert( numberBytesFile > 0 );

    if ( ( fread( str, sizeof(char), numberBytesFile, finput) != numberBytesFile ) && !feof( finput ) )
    {
        fprintf( stderr, "Error fread!\n" );
        return nullptr;
    }

    return str;
}

//================================================================================
//! @brief Функция подсчёта количества строк в тексте.
//!
//! @return [in] str буфер с текстом.
//!
//! @return Количество строк в тексте.
//--------------------------------------------------------------------------------

size_t countNumberLines( char *str, ssize_t numberBytesFile )
{
    assert( str != nullptr );

    size_t linesCount     = 1;
    size_t curOffsetInStr = 0;
    for ( ; curOffsetInStr < numberBytesFile - 1; curOffsetInStr++ )
    {
        if ( strchr( SEPARATION_SYMBOLS, str[curOffsetInStr] ) != nullptr )
        {
            linesCount++;
            str[curOffsetInStr] = '\0';
        }
        else
        {
            unsigned int codeSym = str[curOffsetInStr];
            if ( codeSym < 'a' )
                str[curOffsetInStr] = tolower( codeSym );
        }
    }
    str[curOffsetInStr] = '\0';

    return linesCount;
}

//================================================================================
//! @brief Функция разделения текста на строки.
//!
//! @return [out] lines массив структур Line.
//! @return [in] linesCount количество строк в тексте.
//! @return [in] str буфер с текстом.
//!
//! @note В массив структур lines происходит построчная запись текста.
//--------------------------------------------------------------------------------

void splitToLines( Line *lines, int linesCount, char *str )
{
    assert( lines != nullptr );
    assert( linesCount > 0 );
    assert( str != nullptr );

    char *ptrStr = str;

    for ( size_t i = 0; i < linesCount; i++ )
    {
        lines[i].str = ptrStr;
        __asm__ ( "next:\n\t"
                  "cmpb $0x0, (%%rsi)\n\t"
                  "je stop\n\t"
                  "incq %%rsi\n\t"
                  "jmp next\n\t"
                  "stop:\n\t"
                  "incq %%rsi\n\t"
                  :"=S"( ptrStr )
                  :"0" ( ptrStr )
                  :"%rax"
                );
        lines[i].sizeStr = (int)( ptrStr - 1 - lines[i].str );
    }
}

//================================================================================
//! @brief Функция перевода позиции указателя в файле вывода на новую строку.
//!
//! @param [in] foutput файл, в котором нужно переместить позицию указателя
//!             на новую строку.
//--------------------------------------------------------------------------------

static void moveToNextLine( FILE *foutput )
{
    assert( foutput != nullptr );

    fputs("\n", foutput);
}

//================================================================================
//! @brief Функция заполняет массив струтур Line строками из файла.
//!
//! @param [in] nameFile имя файла.
//! @param [out] linesCount указатель на  переменную, в которой будет
//!              храниться количество строк в файле.
//! @param [in] str указатель на буфер, в которой будет скопирован файл nameFile.
//!
//! @return Указатель на заполненный массив структур Line.
//--------------------------------------------------------------------------------

void *fillStructLine( const char* nameFile, size_t *linesCount, char **ptrStr, Line **ptrLines )
{
    assert( nameFile   != nullptr );
    assert( linesCount != nullptr );
    assert( ptrStr     != nullptr );
    assert( ptrLines   != nullptr );

    FILE *finput = fopen( nameFile, "r" );
    assert( finput != nullptr );

    ssize_t numberBytesFile = getFileSize( finput );
    if (numberBytesFile == -1)
    {
        fprintf( stderr, "Error getFileSize!\n" );
        return nullptr;
    }

    char *str = (char*)calloc( numberBytesFile + 1, sizeof(char) );
    assert( str != nullptr );

    str  = (char *)readFile( finput, str, numberBytesFile );
    if ( str == nullptr )
    {
        fprintf( stderr, "Error readFile!\n" );
        return 0;
    }

    *linesCount = countNumberLines( str, numberBytesFile );

    Line *lines = (Line*)calloc( *linesCount, sizeof(Line) );
    assert( lines != nullptr );

    splitToLines( lines, *linesCount, str );

    *ptrStr   = str;
    *ptrLines = lines;
    fclose( finput );

    return lines;
}

//================================================================================
//! @brief Функция записи информации о тексте в файл.
//!
//! @param [in] lines массив структур Line, в котором хранится текст.
//! @param [in] linesCount количество строк в тексте.
//! @param [in] foutput файл, в который происходит запись.
//! @param [in] nameSort название сортировки для текста.
//--------------------------------------------------------------------------------

void writeFile( Line *lines, int linesCount, FILE *foutput )
{
    assert( lines   != nullptr );
    assert( linesCount > 0);
    assert( foutput != nullptr );


    for ( size_t i = 0; i < linesCount; i++ )
    {
        if ( strcmp( lines[i].str, "\0" ) != 0 )
        {
            fputs( lines[i].str, foutput );
            moveToNextLine( foutput );
        }
    }
} 
