#include <stdio.h>
#include "../include/HashTable.h"

int main()
{
    printf( "It is hash table!\n" );

    struct HashTable_t hashTable = {};

    HashTableErrorCode hashTableError = HASH_TABLE_NO_ERROR;

    hashTableError = HashTableCtor( &hashTable );
    hashTableError = HashTableDtor( &hashTable );    

    return hashTableError;
}