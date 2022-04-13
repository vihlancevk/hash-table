#include <assert.h>
#include "../include/HashTable.h"
#include "../include/HashFunctions.h"

int HashOne( const void *elem, size_t size )
{
    assert(elem != nullptr);

    int hash = 1;

    return hash % ( HASH_TABLE_SIZE );
}

int HashFirstSymbol( const void *elem, size_t size )
{
    assert(elem != nullptr);

    int hash     = 0;
    char *buffer = (char*)elem;

    hash = (int)buffer[0];

    return hash % ( HASH_TABLE_SIZE );
}

int HashWordLen    ( const void *elem, size_t size )
{
    assert(elem != nullptr);

    int hash = (int)size;

    return hash % ( HASH_TABLE_SIZE );
}

int HashSumSymbol( const void *elem, size_t size )
{
    assert(elem != nullptr);

    int hash     = 0;
    char *buffer = (char*)elem;

    for(size_t i = 0; i < size; i++)
    {
        hash += (int)buffer[i];
    }

    return hash % ( HASH_TABLE_SIZE );
}

int HashDed( const void *elem, size_t size )
{
    assert(elem != nullptr);

    char *buffer      = (char*)elem;
    unsigned int hash = buffer[0];

    for ( size_t i = 0; i < size - 1; i++ )
    {
        hash = ( hash << 1 | hash >> 31 ) ^ buffer[i + 1];
    }

    return hash % ( HASH_TABLE_SIZE );
}

int HashRot13( const void *elem, size_t size )
{
    assert(elem != nullptr);

    int hash     = 0;
    char *buffer = (char*)elem;

    for(size_t i = 0; i < size; i++)
    {
        hash += (int)buffer[i];
        hash -= (hash << 13) | (hash >> 19);
    }

    return hash % ( HASH_TABLE_SIZE );
}
