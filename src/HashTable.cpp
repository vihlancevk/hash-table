#include <stdlib.h>
#include "../include/HashTable.h"

const size_t LIST_SIZE = 4 ;

HashTableErrorCode HashTableCtor( struct HashTable_t *hashTable )
{
    ListErrorCode listError = LIST_NO_ERROR;

    for ( int i = 0; i < HASH_TABLE_SIZE; i++ )
    {
        listError = ListCtor( &hashTable->lists[i], LIST_SIZE );
        if ( listError != LIST_NO_ERROR )
        {
            return HASH_TABLE_LIST_CTOR_ERROR;
        }
    }

    return HASH_TABLE_NO_ERROR;
}

HashTableErrorCode HashTableDtor( struct HashTable_t *hashTable )
{
    for ( int i = 0; i < HASH_TABLE_SIZE; i++ )
    {
        ListDtor( &hashTable->lists[i] );
    }

    return HASH_TABLE_NO_ERROR;
}
