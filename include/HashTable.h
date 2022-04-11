#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "../include/List.h"

const size_t HASH_TABLE_SIZE = 2 << 8;

enum HashTableErrorCode
{
    HASH_TABLE_NO_ERROR,
    HASH_TABLE_LIST_CTOR_ERROR
};

struct HashTable_t
{
    struct List_t lists[HASH_TABLE_SIZE];
};

HashTableErrorCode HashTableCtor( struct HashTable_t *hashTable );
HashTableErrorCode HashTableDtor( struct HashTable_t *hashTable );

#endif // HASH_TABLE_H_
