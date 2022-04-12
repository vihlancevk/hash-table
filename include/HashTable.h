#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "../include/List.h"

const size_t HASH_TABLE_SIZE = 2 << 8;

enum HashTableErrorCode
{
    HASH_TABLE_NO_ERROR,
    HASH_TABLE_LIST_CTOR_ERROR,
    HASH_TABLE_LIST_DTOR_ERROR,
    HASH_TABLE_LIST_INSERT_ERROR,
    HASH_TABLE_LIST_INSERT_ALREDY_EXISTS
};

struct HashTable_t
{
    struct List_t *lists;
};

HashTableErrorCode HashTableCtor( struct HashTable_t *hashTable );
HashTableErrorCode HashTableDtor( struct HashTable_t *hashTable );
HashTableErrorCode HashTableInsert( struct HashTable_t *hashTable, char *elem );
HashTableErrorCode HashTableRemove( struct HashTable_t *hashTable, char *elem );
HashTableErrorCode HashTableFind  ( struct HashTable_t *hashTable, char *elem );

#endif // HASH_TABLE_H_
