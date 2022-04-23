// ToDo:
// 1) strcmp через интринсики

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/HashTable.h"

static const size_t  LIST_SIZE                  = 10;
static const ssize_t IS_ELEM_IN_HASH_TABLE      = -1;
static const ssize_t IS_NOT_ELEM_IN_HASH_TABLE  =  1;
static const char    *HASH_TABLE_GRAPH_VIZ      = "./res/hashTableGraphviz.gv" ;

#define ASSERT_OK_( hashTable, elem )                       \
    do                                                      \
    {                                                       \
        assert( hashTable != nullptr );                     \
        assert( elem      != nullptr );                     \
        hashTableError = HashTableVerificator( hashTable ); \
        if ( hashTableError != HASH_TABLE_NO_ERROR )        \
        {                                                   \
            return hashTableError;                          \
        }                                                   \
    } while( 0 )

HashTableErrorCode HashTableVerificator( struct HashTable_t *hashTable )
{
    assert( hashTable != nullptr );

    if ( hashTable->hashTableStatus != HASH_TABLE_CONSTRUCTED )
    {
        return HASH_TABLE_USE_NOT_CONSTRUCTED;
    }

    return HASH_TABLE_NO_ERROR;
}

struct HashTableDumpNodeDescription
{
    char color[10];
};

HashTableErrorCode HashTableDump( struct HashTable_t *hashTable )
{
    assert( hashTable != nullptr );

    FILE *graphViz = fopen( HASH_TABLE_GRAPH_VIZ, "w" );

    fprintf( graphViz, "digraph HashTable{\n\n" );
    fprintf( graphViz, "\trankdir=LR;\n\n" );
    fprintf( graphViz, "\tnode[color=\"red\",fontsize=14];\n\n" );

    for ( size_t i = HASH_TABLE_SIZE; i > 0; i-- )
    {
        fprintf( graphViz, "\t\"%zuht\"[shape=record, style=\"filled\", fillcolor=\"pink\", label=\"%zuht\"];\n", i - 1, i - 1 );
    }
    fprintf( graphViz, "\n" );

    size_t currentOffset = 0;
    for ( size_t i = 0; i < HASH_TABLE_SIZE; i++ )
    {
        size_t j = 0;

        if ( hashTable->lists[i].status != LIST_CONSTRUCTED )
        {
            fprintf( graphViz, "\t\"%zuht\"->%zu;\n", i, 1 + currentOffset );
            for ( j = 1; j < LIST_SIZE + 1; j++ )
            {
                fprintf( graphViz, "\t%lu[shape=record, style=\"filled\", fillcolor=%s, label=\"<%lu> address : %lu | <%s> elem : %s | <%d> next : %d | <%d> prev : %d\"];\n",
                         j + currentOffset, "white", 0l, 0l,
                         "(null)", "(null)",
                         0, 0,
                         0, 0 );
            }
            fprintf( graphViz, "\n" );

            fprintf( graphViz, "\t{\n" );
            fprintf( graphViz, "\t\tedge[color=black]\n" );
            for ( j = 1; j < LIST_SIZE; j++ )
            {
                fprintf( graphViz, "\t\t%zu ", j + currentOffset );
                fprintf( graphViz, "-> %zu\n", j + currentOffset + 1 );
            }
            fprintf( graphViz, "\t}\n" );
            fprintf( graphViz, "\n" );
            currentOffset += j;
            continue;
        }

        if ( hashTable->lists[i].size != 0 )
        {
            fprintf( graphViz, "\t\"%zuht\"->%zu;\n", i, 1 + currentOffset );
            HashTableDumpNodeDescription hashTableNode = {};
            int head = hashTable->lists[i].head;
            for ( j = 1; j < hashTable->lists[i].capacity; j++ )
            {
                if ( hashTable->lists[i].data[j].prev == -1 )
                {
                    strcpy( hashTableNode.color, "green" );
                }
                else
                {
                    strcpy( hashTableNode.color, "yellow" );
                }

                fprintf( graphViz, "\t%lu[shape=record, style=\"filled\", fillcolor=%s, label=\"<%lu> address : %lu | <%s> elem : %s | <%d> next : %d | <%d> prev : %d\"];\n",
                         j + currentOffset, hashTableNode.color, j, j,
                         hashTable->lists[i].data[j].elem, hashTable->lists[i].data[j].elem,
                         hashTable->lists[i].data[j].next, hashTable->lists[i].data[j].next,
                         hashTable->lists[i].data[j].prev, hashTable->lists[i].data[j].prev );

                head = hashTable->lists[i].data[head].next;
            }
            fprintf( graphViz, "\n" );

            fprintf( graphViz, "\t{\n" );
            fprintf( graphViz, "\t\tedge[color=black]\n" );
            for ( j = 1; j < hashTable->lists[i].capacity - 1; j++ )
            {
                fprintf( graphViz, "\t\t%zu ", j + currentOffset );
                fprintf( graphViz, "-> %zu\n", j + currentOffset + 1 );
            }
            fprintf( graphViz, "\t}\n" );
            fprintf( graphViz, "\n" );
        }
        currentOffset += j;
    }
    fprintf( graphViz, "}\n\n" );

    fclose( graphViz );

    system( "dot -Tpng ./res/hashTableGraphviz.gv -o ./res/hashTableGraphviz.png" );

    return HASH_TABLE_NO_ERROR;
}

HashTableErrorCode HashTableCtor( struct HashTable_t *hashTable, unsigned int (*hashFunction)( const void *, size_t ) )
{
    assert( hashTable != nullptr );

    if ( hashTable->hashTableStatus != HASH_TABLE_NOT_CONSTRUCTED )
    {
        return HASH_TABLE_CTOR_ERROR;
    }

    hashTable->lists           = (struct List_t*)calloc( HASH_TABLE_SIZE, sizeof( struct List_t ) );
    hashTable->hashFunction    = hashFunction;
    hashTable->hashTableStatus = HASH_TABLE_CONSTRUCTED;

    return HASH_TABLE_NO_ERROR;
}

HashTableErrorCode HashTableDtor( struct HashTable_t *hashTable )
{
    assert( hashTable != nullptr );

    if ( hashTable->hashTableStatus != HASH_TABLE_CONSTRUCTED )
    {
        return HASH_TABLE_DTOR_ERROR;
    }

    ListErrorCode listError = LIST_NO_ERROR;

    for ( int i = 0; i < HASH_TABLE_SIZE; i++ )
    {
        if ( hashTable->lists[i].status == LIST_CONSTRUCTED )
        {
            listError = ListDtor( &hashTable->lists[i] );
            if ( listError != LIST_NO_ERROR )
            {
                free( hashTable->lists );
                hashTable->hashFunction    = nullptr;
                hashTable->hashTableStatus = HASH_TABLE_DESTRUCTED;
                return HASH_TABLE_LIST_DTOR_ERROR;           
            }
        }
    }

    free( hashTable->lists );
    hashTable->hashFunction    = nullptr;
    hashTable->hashTableStatus = HASH_TABLE_DESTRUCTED;
    return HASH_TABLE_NO_ERROR;
}

int HashTableFind( struct HashTable_t *hashTable, struct Line *lines, ssize_t *isElemInHashTable )
{
    HashTableErrorCode hashTableError = HASH_TABLE_NO_ERROR;
    ASSERT_OK_( hashTable, lines->str );
    assert( isElemInHashTable != nullptr );

    int hashTableIndex = hashTable->hashFunction( lines->str, lines->sizeStr );
    if ( hashTable->lists[hashTableIndex].status == LIST_NOT_CONSTRUCTED )
    {
        *isElemInHashTable = IS_NOT_ELEM_IN_HASH_TABLE;
    }

    return hashTableIndex;
}

HashTableErrorCode HashTableInsert( struct HashTable_t *hashTable, struct Line *lines )
{
    HashTableErrorCode hashTableError = HASH_TABLE_NO_ERROR;
    ASSERT_OK_( hashTable, lines->str );

    ssize_t isElemInHashTable = IS_ELEM_IN_HASH_TABLE;
    size_t  hashTableIndex    = HashTableFind( hashTable, lines, &isElemInHashTable );
    if ( isElemInHashTable == IS_NOT_ELEM_IN_HASH_TABLE )
    {
        if ( ListCtor( &hashTable->lists[hashTableIndex], LIST_SIZE ) != LIST_NO_ERROR )
            return HASH_TABLE_LIST_CTOR_ERROR;
    }

    for ( size_t i = 0, j = hashTable->lists[hashTableIndex].head; i < hashTable->lists[hashTableIndex].size; i++ )
    {
        if ( strcmp( hashTable->lists[hashTableIndex].data[j].elem, lines->str ) == 0 )
        {
            ++( hashTable->lists[hashTableIndex].data[j].n_elems );
            return HASH_TABLE_NO_ERROR;
        }
        j = hashTable->lists[hashTableIndex].data[j].next;
    }

    int physNum = 0;
    if ( LIST_INSERT_AT_END_( &hashTable->lists[hashTableIndex], &physNum, (const structElemT)lines->str ) != LIST_NO_ERROR )
        return HASH_TABLE_LIST_INSERT_ERROR;

    return HASH_TABLE_NO_ERROR;
}

HashTableErrorCode FillHashTable( struct HashTable_t *hashTable, const char *nameFile, char **ptrStr, Line **ptrLines )
{
    assert( hashTable != nullptr );
    assert( nameFile  != nullptr );
    assert( ptrStr    != nullptr );
    assert( ptrLines  != nullptr );

    size_t linesCount  = 0;
    struct Line *lines = (Line*)fillStructLine( nameFile, &linesCount, ptrStr, ptrLines );
    if ( lines == nullptr )
        return HASH_TABLE_FILL_LINE_ERROR;

    for ( size_t i = 0; i < linesCount; i++ )
    {
        if ( strcmp( lines[i].str, "\0" ) != 0 )
        {
            HashTableErrorCode hashTableError = HashTableInsert( hashTable, &lines[i] );
            if ( hashTableError != HASH_TABLE_NO_ERROR  )
                return hashTableError;
        }
    }

    return HASH_TABLE_NO_ERROR;
}
