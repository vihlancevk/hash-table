

static int HashRot13( const void *elem, size_t size )
{
    assert(elem != nullptr);

    int hash = 0;

    char *buffer = (char*)elem;

    for(size_t i = 0; i < size; i++)
    {
        hash += (int)buffer[i];
        hash -= (hash << 13) | (hash >> 19);
    }

    return hash % ( HASH_TABLE_SIZE );
}
