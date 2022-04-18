#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "../include/List.h"
#include "../include/FileOperations.h"

const size_t HASH_TABLE_SIZE = 700;

enum HashTableStatus
{
    HASH_TABLE_NOT_CONSTRUCTED = 0,
    HASH_TABLE_CONSTRUCTED     = 1,
    HASH_TABLE_DESTRUCTED      = 2
};

enum HashTableErrorCode
{
    HASH_TABLE_NO_ERROR            = 0,
    HASH_TABLE_CTOR_ERROR          = 1,
    HASH_TABLE_LIST_CTOR_ERROR     = 2,
    HASH_TABLE_DTOR_ERROR          = 3,
    HASH_TABLE_LIST_DTOR_ERROR     = 4,
    HASH_TABLE_LIST_INSERT_ERROR   = 5,
    HASH_TABLE_FILL_LINE_ERROR     = 6,
    HASH_TABLE_USE_NOT_CONSTRUCTED = 7
};

struct HashTable_t
{
    HashTableStatus hashTableStatus;
    struct List_t *lists;
    unsigned int (*hashFunction)( const void *, size_t );
};

HashTableErrorCode HashTableDump( struct HashTable_t *hashTable );
HashTableErrorCode HashTableCtor( struct HashTable_t *hashTable, unsigned int (*hashFunction)( const void *, size_t ) );
HashTableErrorCode HashTableDtor( struct HashTable_t *hashTable );
int                HashTableFind  ( struct HashTable_t *hashTable, const char *elem );
HashTableErrorCode HashTableInsert( struct HashTable_t *hashTable, const char *elem );
HashTableErrorCode FillHashTable( struct HashTable_t *hashTable, const char *nameFile, char **ptrStr, Line **ptrLines );

#endif // HASH_TABLE_H_
