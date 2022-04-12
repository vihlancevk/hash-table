#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "../include/List.h"

const size_t HASH_TABLE_SIZE = 2 << 8;

enum HashTableStatus
{
    HASH_TABLE_NOT_CONSTRUCTED,
    HASH_TABLE_CONSTRUCTED,
    HASH_TABLE_DESTRUCTED,
};

enum HashTableErrorCode
{
    HASH_TABLE_NO_ERROR,
    HASH_TABLE_CTOR_ERROR,
    HASH_TABLE_LIST_CTOR_ERROR,
    HASH_TABLE_DTOR_ERROR,
    HASH_TABLE_LIST_DTOR_ERROR,
    HASH_TABLE_LIST_INSERT_ERROR,
    HASH_TABLE_LIST_INSERT_ALREDY_EXISTS,
    HASH_TABLE_USE_NOT_CONSTRUCTED
};

struct HashTable_t
{
    HashTableStatus hashTableStatus;
    struct List_t *lists;
    int (*hashFunction)( const void *elem, size_t size );
};

HashTableErrorCode HashTableCtor( struct HashTable_t *hashTable );
HashTableErrorCode HashTableDtor( struct HashTable_t *hashTable );
int                HashTableFind  ( struct HashTable_t *hashTable, const char *elem );
HashTableErrorCode HashTableInsert( struct HashTable_t *hashTable, const char *elem );

#endif // HASH_TABLE_H_
