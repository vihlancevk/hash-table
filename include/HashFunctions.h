#ifndef HASH_FUNCTIONS_H_
#define HASH_FUNCTIONS_H_

#include <stddef.h>

unsigned int HashOne        ( const void *elem, size_t size );
unsigned int HashFirstSymbol( const void *elem, size_t size );
unsigned int HashWordLen    ( const void *elem, size_t size );
unsigned int HashSumSymbol  ( const void *elem, size_t size );
unsigned int HashDed        ( const void *elem, size_t size );

// unsigned int HashRot13      ( const void *elem, size_t size );

extern "C" unsigned int HashRot13( const void *elem, size_t size );

#endif // HASH_FUNCTIONS_H_
