#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>

typedef char* structElemT;

enum ListStatus
{
    LIST_NOT_CONSTRUCTED = 0,
    LIST_CONSTRUCTED     = 1,
    LIST_DESTRUCTED      = 2
};

enum ListErrorCode
{
    LIST_NO_ERROR                      = 0 ,
    LIST_CONSTRUCTED_ERROR             = 1 ,
    LIST_DESTRUCTED_ERROR              = 2 ,
    LIST_CONVERT_LOG_TO_PHYS_NUM_ERROR = 3 ,
    LIST_DUMP_OPEN_LOG_FILE_ERROR      = 4 ,
    LIST_DATA_CALLOC_ERROR             = 5 ,
    LIST_USE_NOT_CONSTRUCTED           = 6 ,
    LIST_RESIZE_UP_ERROR               = 7 ,
    LIST_IS_EMPTY                      = 8 ,
    LIST_HAVE_CYCLE                    = 9 ,
    LIST_INSERT_UNCORRECT_PLACE        = 10,
    LIST_INSERT_BEFORE_UNCORRECT_USE   = 11,
    LIST_REMOVE_UNCORRECT_PLACE        = 12
};

struct ListNode_t
{
    structElemT elem;
    int next;
    int prev;
    size_t n_elems;
};

struct List_t
{
    ListStatus status;
    int isSorted;
    size_t capacity;
    size_t size;
    ListNode_t *data;
    int head;
    int tail;
    int freePlace;
};

ListErrorCode GetListError(const List_t *list);

ListErrorCode ListCtor(List_t *list, const size_t capacity);

ListErrorCode ListDtor(List_t *list);

ListErrorCode ListDump(const List_t *list);

ListErrorCode ListLinearization(List_t *list);

int ListConvertLogicalForPhysNum(const List_t *list, const int place, ListErrorCode *listError);

ListErrorCode ListInsertAfter(List_t *list, int *physNum, const structElemT elem, const int place);

#define LIST_INSERT_AT_END_(list, physNum, elem) \
    ListInsertAfter(list, physNum, elem, (*list).tail)

ListErrorCode ListInsertBefore(List_t *list, int *physNum, const structElemT elem, const int place);

#define LIST_INSERT_AT_HEAD_(list, physNum, elem) \
    ListInsertBefore(list, physNum, elem, (*list).head)

ListErrorCode ListRemove(List_t *list, structElemT *elem, const int place);

#define LIST_REMOVE_AT_END_(list, elem) \
    ListRemove(list, elem, (*list).tail)

#define LIST_REMOVE_AT_HEAD_(list, elem) \
    ListRemove(list, elem, (*list).head)

#endif // LIST_H_ 
