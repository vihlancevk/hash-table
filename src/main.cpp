#include <stdio.h>
#include "../include/HashTable.h"
#include "../include/FileOperations.h"

const char *INPUT_ONEGIN_FILE  = "./res/inputOnegin.txt" ;

int main()
{
    printf( "It is hash table!\n" );

    struct HashTable_t hashTable = {};

    HashTableErrorCode hashTableError = HASH_TABLE_NO_ERROR;

    hashTableError = HashTableCtor( &hashTable );
    
    char *ptrStr   = nullptr;
    Line *ptrLines = nullptr;
    hashTableError = FillHashTable( &hashTable, INPUT_ONEGIN_FILE, ptrStr, ptrLines );

    if ( ptrStr   != nullptr ) { free( ptrStr   ) ; }
    if ( ptrLines != nullptr ) { free( ptrLines ) ; }
    hashTableError = HashTableDtor( &hashTable );

    return hashTableError;
}