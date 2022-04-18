#include <assert.h>
#include "../include/HashTable.h"
#include "../include/HashFunctions.h"

unsigned int HashOne( const void *elem, size_t size )
{
    assert(elem != nullptr);

    unsigned int hash = 1;

    return hash % ( HASH_TABLE_SIZE );
}

unsigned int HashFirstSymbol( const void *elem, size_t size )
{
    assert(elem != nullptr);

    unsigned int hash = 0;
    char *buffer      = (char*)elem;

    hash = (unsigned int)buffer[0];

    return hash % ( HASH_TABLE_SIZE );
}

unsigned int HashWordLen( const void *elem, size_t size )
{
    assert(elem != nullptr);

    unsigned int hash = (unsigned int)size;

    return hash % ( HASH_TABLE_SIZE );
}

unsigned int HashSumSymbol( const void *elem, size_t size )
{
    assert(elem != nullptr);

    unsigned int hash = 0;
    char *buffer      = (char*)elem;

    for(size_t i = 0; i < size; i++)
    {
        hash += (unsigned int)buffer[i];
    }

    return hash % ( HASH_TABLE_SIZE );
}

unsigned int HashDed( const void *elem, size_t size )
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
