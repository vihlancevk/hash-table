#include <stdio.h>
#include "../include/HashTable.h"

int main()
{
    printf( "It is hash table!\n" );

    struct HashTable_t hashTable = {};

    HashTableErrorCode hashTableError = HASH_TABLE_NO_ERROR;

    hashTableError = HashTableCtor( &hashTable );
    hashTableError = HashTableInsert( &hashTable, (char*)"Kosty"  );
    hashTableError = HashTableInsert( &hashTable, (char*)"Andrey" );
    hashTableError = HashTableInsert( &hashTable, (char*)"Vany"   );
    hashTableError = HashTableInsert( &hashTable, (char*)"Lev"    );
    hashTableError = HashTableInsert( &hashTable, (char*)"Alik"   );
    hashTableError = HashTableDump ( &hashTable );
    hashTableError = HashTableDtor( &hashTable );

    return hashTableError;
}