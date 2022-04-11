#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "../include/List.h"

enum HashTableError
{
    HASH_TABLE_NO_ERROR
};

struct HashTable
{
    List_t *list;
    HashTableError hashTableError;
};

#endif // HASH_TABLE_H_
