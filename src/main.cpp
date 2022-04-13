#include <stdio.h>
#include "../include/HashTable.h"
#include "../include/HashFunctions.h"
#include "../include/FileOperations.h"

const char *INPUT_ONEGIN_FILE      = "./res/inputOnegin.txt"    ;
const char *OUTPUT_HASH_TABLE_FILE = "./res/outputHashTable.txt";

enum MainErrorCode
{
    MAIN_NO_ERROR,
    MAIN_HASH_TABLE_CTOR_ERROR,
    MAIN_HASH_TABLE_FILL_ERROR,
    MAIN_OUTPUT_HASH_TABLE_FILE_NOT_OPEN,
    MAIN_HASH_TABLE_DTOR_ERROR
};

MainErrorCode CheckHashFunction( const char *hashFunctionName, int (*hashFunction)( const void *, size_t ) )
{
    HashTableErrorCode hashTableError = HASH_TABLE_NO_ERROR;

    struct HashTable_t hashTable = {};
    hashTableError = HashTableCtor( &hashTable, hashFunction );
    if ( hashTableError != HASH_TABLE_NO_ERROR )
        return MAIN_HASH_TABLE_CTOR_ERROR;
    
    char *ptrStr   = nullptr;
    Line *ptrLines = nullptr;
    hashTableError = FillHashTable( &hashTable, INPUT_ONEGIN_FILE, &ptrStr, &ptrLines );
    if ( hashTableError != HASH_TABLE_NO_ERROR )
    {
        free( ptrStr   );
        free( ptrLines );
        return MAIN_HASH_TABLE_FILL_ERROR;
    }

    FILE *foutput = fopen( OUTPUT_HASH_TABLE_FILE, "w" );
    if ( foutput == nullptr )
        return MAIN_OUTPUT_HASH_TABLE_FILE_NOT_OPEN;

    fprintf( foutput, "%s\n", hashFunctionName );

    size_t j = 0;
    for ( size_t i = 0; i < HASH_TABLE_SIZE; i++ )
    {
        j += hashTable.lists[i].size;
        fprintf( foutput, "%zu %zu ", i, hashTable.lists[i].size );
    }
    printf( "%zu\n", j );

    fclose( foutput );

    free( ptrStr   );
    free( ptrLines );
    hashTableError = HashTableDtor( &hashTable );
    if ( hashTableError != HASH_TABLE_NO_ERROR )
        return MAIN_HASH_TABLE_DTOR_ERROR;

    return MAIN_NO_ERROR;
}

int main()
{
    MainErrorCode mainError = MAIN_NO_ERROR;

    mainError = CheckHashFunction( "HashRot13", HashRot13 );

    return mainError;
}
