#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <typeinfo>
#include "../include/HashTable.h"

const size_t LIST_SIZE = 4 ;

static u_int64_t HashRot13(const void *elem, size_t size)
{
    assert(elem != nullptr);

    u_int64_t hash = 0;

    char *buffer = (char*)elem;

    for(size_t i = 0; i < size; i++)
    {
        hash += (u_int64_t)buffer[i];
        hash -= (hash << 13) | (hash >> 19);
    }

    return hash % ( 2 << 8 );
}

HashTableErrorCode HashTableCtor( struct HashTable_t *hashTable )
{
    assert( hashTable != nullptr );

    hashTable->lists = (struct List_t*)calloc( HASH_TABLE_SIZE, sizeof( struct List_t ) );

    return HASH_TABLE_NO_ERROR;
}

HashTableErrorCode HashTableDtor( struct HashTable_t *hashTable )
{
    assert( hashTable != nullptr );

    ListErrorCode listError = LIST_NO_ERROR;

    for ( int i = 0; i < HASH_TABLE_SIZE; i++ )
    {
        if ( hashTable->lists[i].status == LIST_CONSTRUCTED )
        {
            listError = ListDtor( &hashTable->lists[i] );
        }
    }

    free( hashTable->lists );
    return HASH_TABLE_NO_ERROR;
}

HashTableErrorCode HashTableInsert( struct HashTable_t *hashTable, char *elem )
{
    assert( hashTable != nullptr );
    assert( elem      != nullptr );

    u_int64_t hashTableIndex = HashRot13( elem, strlen( elem ) );
    if ( hashTable->lists[hashTableIndex].status == LIST_NOT_CONSTRUCTED )
    {
        if ( ListCtor( &hashTable->lists[hashTableIndex], LIST_SIZE ) != LIST_NO_ERROR )
            return HASH_TABLE_LIST_CTOR_ERROR;
    }

    for ( int i = 0, j = hashTable->lists[hashTableIndex].head; i < hashTable->lists[hashTableIndex].size; i++ )
    {
        if ( strcmp( hashTable->lists[hashTableIndex].data[j].elem, elem ) == 0 )
            return HASH_TABLE_LIST_INSERT_ALREDY_EXISTS;
        j = hashTable->lists[hashTableIndex].data[j].next;
    }

    int physNum = 0;
    if ( LIST_INSERT_AT_END_( &hashTable->lists[hashTableIndex], &physNum, elem ) != LIST_NO_ERROR )
        return HASH_TABLE_LIST_INSERT_ERROR;

    return HASH_TABLE_NO_ERROR;
}
