#include <stdio.h>
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

int main()
{
    printf( "It is hash table!\n" );

    return 0;
}