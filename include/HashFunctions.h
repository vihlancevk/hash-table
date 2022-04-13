#ifndef HASH_FUNCTIONS_H_
#define HASH_FUNCTIONS_H_

#include <stddef.h>

int HashOne        ( const void *elem, size_t size );
int HashFirstSymbol( const void *elem, size_t size );
int HashWordLen    ( const void *elem, size_t size );
int HashSumSymbol  ( const void *elem, size_t size );
int HashDed        ( const void *elem, size_t size );
int HashRot13      ( const void *elem, size_t size );

#endif // HASH_FUNCTIONS_H_
