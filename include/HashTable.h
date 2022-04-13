#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "../include/List.h"
#include "../include/FileOperations.h"

const size_t HASH_TABLE_SIZE = 700;

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
    HASH_TABLE_FILL_LINE_ERROR,
    HASH_TABLE_USE_NOT_CONSTRUCTED
};

struct HashTable_t
{
    HashTableStatus hashTableStatus;
    struct List_t *lists;
    int (*hashFunction)( const void *, size_t );
};

HashTableErrorCode HashTableDump( struct HashTable_t *hashTable );
HashTableErrorCode HashTableCtor( struct HashTable_t *hashTable, int (*hashFunction)( const void *, size_t ) );
HashTableErrorCode HashTableDtor( struct HashTable_t *hashTable );
int                HashTableFind  ( struct HashTable_t *hashTable, const char *elem );
HashTableErrorCode HashTableInsert( struct HashTable_t *hashTable, const char *elem );
HashTableErrorCode FillHashTable( struct HashTable_t *hashTable, const char *nameFile, char **ptrStr, Line **ptrLines );

#endif // HASH_TABLE_H_
