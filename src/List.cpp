#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/List.h"

#define DEBUG
#undef  DEBUG

ListErrorCode listStatus = LIST_NO_ERROR;

#define ASSERT_OK_(list)                 \
    do                                   \
    {                                    \
        assert(list != nullptr);         \
        listStatus = GetListError(list); \
        if (listStatus != LIST_NO_ERROR) \
        {                                \
            return listStatus;           \
        }                                \
    } while(0)

const int LIST_PLACE_FREE = -1;
const int LIST_RESIZE_UP_COEFFICIENT = 2;
const int LIST_CMD_MAX_SIZE = 1000;
const int POISON = -(1000 - 7);
const void *const ERR_PTR = (void*)666;
const char *LIST_LOG_FILE  = "./res/listLog.html";
const char *LIST_GRAPH_VIZ = "./res/graphviz.gv" ;

static int IsListCycle(const List_t *list)
{
    assert(list != nullptr);

    int slow = list->head;
    int fast = list->head;

    while (fast != 0 && list->data[fast].next != 0)
    {
        slow = list->data[slow].next;
        fast = list->data[fast].next;
        fast = list->data[fast].next;

        if (slow == fast)
        {
            return 1;
        }
    }

    return 0;
}

ListErrorCode GetListError(const List_t *list)
{
    assert(list != nullptr);

    if (list->status != LIST_CONSTRUCTED)
    {
        return LIST_USE_NOT_CONSTRUCTED;
    }

    if (IsListCycle(list))
    {
        return LIST_HAVE_CYCLE;
    }

    return LIST_NO_ERROR;
}

void ClearListLogFile()
{
    FILE *logFile = fopen(LIST_LOG_FILE, "w");
    fclose(logFile);
}

ListErrorCode ListCtor(List_t *list, const size_t capacity)
{
    assert(list != nullptr);

    if (list->status != LIST_NOT_CONSTRUCTED)
    {
        return LIST_CONSTRUCTED_ERROR;
    }
    
    ClearListLogFile();

    list->status = LIST_CONSTRUCTED;
    list->isSorted = 1;
    list->capacity = capacity + 1;
    list->size = 0;

    list->data = (ListNode_t*)calloc(list->capacity, sizeof(ListNode_t));
    if (list->data == nullptr)
    {
        return LIST_DATA_CALLOC_ERROR;
    }

    size_t  i = 0;
    for (i = 1; i < list->capacity - 1; i++)
    {
        list->data[i].next = i + 1;
    }

    for (i = 1; i < list->capacity; i++)
    {
        list->data[i].prev = LIST_PLACE_FREE;
    }

    list->head = 0;
    list->tail = 0;
    list->freePlace = 1;

    #ifdef DEBUG
        ListDump(list);
    #endif // DEBUG

    return LIST_NO_ERROR;
}

ListErrorCode ListDtor(List_t *list)
{
    assert(list != nullptr);

    if (list->status != LIST_CONSTRUCTED)
    {
        return LIST_DESTRUCTED_ERROR;
    }

    list->status = LIST_DESTRUCTED;
    list->isSorted = POISON;
    list->capacity = POISON;
    list->size = POISON;
    free(list->data);
    list->data = (ListNode_t*)ERR_PTR;
    list->head = POISON;
    list->tail = POISON;

    return LIST_NO_ERROR;
}

struct ListDumpNodeDescription
{
    char color[10];
};

ListErrorCode ListDump(const List_t *list)
{
    assert(list != nullptr);

    FILE *logFile = fopen(LIST_LOG_FILE, "a");
    if (logFile == nullptr)
    {
        return LIST_DUMP_OPEN_LOG_FILE_ERROR;
    }

    fprintf(logFile, "--------------------------------------------------------------------------------<br><br>");

    fprintf(logFile, "[LIST_DUMP]<br><br>");

    fprintf(logFile, "isSorted : %d<br>", list->isSorted);
    fprintf(logFile, "size : %lu<br><br>", list->size);

    size_t i = 0;
    fprintf(logFile, "%4s : ", "i");
    for (i = 0; i < list->capacity; i++)
    {
        fprintf(logFile, "%3lu ", i);
    }
    fprintf(logFile, "<br><br>");

    #define FPRINTF_LIST_DATA_(specifier, arrayName)                          \
        do{                                                                   \
            fprintf(logFile, "%4s : ", #arrayName);                           \
            for (i = 0; i < list->capacity; i++)                              \
            {                                                                 \
                fprintf(logFile, "%3"#specifier" ", list->data[i].arrayName); \
            }                                                                 \
            fprintf(logFile, "<br><br>");                                     \
        } while(0)

    FPRINTF_LIST_DATA_(s, elem);
    FPRINTF_LIST_DATA_(d, next);
    FPRINTF_LIST_DATA_(d, prev);

    fprintf(logFile, "head : %d<br>", list->head);
    fprintf(logFile, "tail : %d<br>", list->tail);
    fprintf(logFile, "freePlace : %d<br><br>", list->freePlace);

    FILE *graphViz = fopen(LIST_GRAPH_VIZ, "w");

    fprintf(graphViz, "digraph List{\n\n");
    fprintf(graphViz,"\trankdir=LR;\n\n");
    fprintf(graphViz, "\tnode[color=\"red\",fontsize=14];\n\n");

    if (list->size != 0)
    {
        ListDumpNodeDescription listNode = {};
        int head = list->head;
        for (i = 1; i < list->capacity; i++)
        {
            if (list->data[i].prev == -1)
            {
                strcpy(listNode.color, "green");
            }
            else
            {
                strcpy(listNode.color, "yellow");
            }
            fprintf(graphViz, "\t%lu[shape=record, style=\"filled\", fillcolor=%s, label=\"<%lu> address : %lu | <%s> elem : %s | <%d> next : %d | <%d> prev : %d\"];\n",
                    i, listNode.color, i, i,
                    list->data[i].elem, list->data[i].elem,
                    list->data[i].next, list->data[i].next,
                    list->data[i].prev, list->data[i].prev);

            head = list->data[head].next;
        }
        fprintf(graphViz, "\n");

        if (list->size > 1)
        {
            fprintf(graphViz, "\t{\n");
            fprintf(graphViz, "\t\tedge[color=white]\n");
            for (i = 1; i < list->capacity - 1; i++)
            {
                fprintf(graphViz, "\t\t%zu ", i);
                fprintf(graphViz, "-> %zu\n", i + 1);
            }
            fprintf(graphViz, "\t}\n");
            fprintf(graphViz, "\n");

            for (i = 1; i < list->capacity; i++)
            {
                if (list->data[i].next != 0 && list->data[i].prev != -1)
                {
                    fprintf(graphViz, "\t%zu : %d ", i, list->data[i].next);
                    fprintf(graphViz, "-> %d : %d[color=\"green\", label=\"next\", fontsize=12, constraint=false]\n", list->data[i].next, list->data[list->data[i].next].next);
                }
                if (list->data[i].next != 0 && list->data[i].prev == -1)
                {
                    fprintf(graphViz, "\t%zu : %d ", i, list->data[i].next);
                    fprintf(graphViz, "-> %d : %d[color=\"green\", label=\"free next\", fontsize=12, constraint=false]\n", list->data[i].next, list->data[list->data[i].next].next);
                }
            }
            fprintf(graphViz, "\n");

            for (i = 1; i < list->capacity; i++)
            {
                if (list->data[i].prev != -1 && list->data[i].prev != 0)
                {
                    fprintf(graphViz, "\t%zu : %d ", i, list->data[i].prev);
                    fprintf(graphViz, "-> %d : %d[color=\"blue\", label=\"prev\", fontsize=12, constraint=false]\n", list->data[i].prev, list->data[list->data[i].prev].prev);
                }
            }
            fprintf(graphViz, "\n");
        }
    }

    fprintf(graphViz, "}\n\n");

    fclose(graphViz);

    static int cntImg = 1;
    char *str = (char*)calloc(LIST_CMD_MAX_SIZE, sizeof(char));
    sprintf(str, "%s%d%s", "dot -Tpng ./res/graphviz.gv -o ./res/graphviz/graphviz", cntImg, ".png");
    system(str);
    sprintf(str, "%s%d%s", "<img src =\"./res/graphviz/graphviz", cntImg, ".png\"><br>");
    fprintf(logFile, "%s", str);
    cntImg = cntImg + 1;
    free(str);

    fprintf(logFile, "--------------------------------------------------------------------------------<br><br>");

    fclose(logFile);

    return LIST_NO_ERROR;
}

ListErrorCode ListLinearization(List_t *list)
{
    assert(list != nullptr);

    ListNode_t *data = (ListNode_t*)calloc(list->capacity, sizeof(ListNode_t));
    if (data == nullptr)
    {
        return LIST_CONVERT_LOG_TO_PHYS_NUM_ERROR;
    }

    size_t i = 0;
    int currentLogNum = list->head;
    for (i = 1; i < list->size + 1; i++)
    {
        data[i].elem = list->data[currentLogNum].elem;
        currentLogNum = list->data[currentLogNum].next;
    }

    for (i = 1; i < list->size; i++)
    {
        data[i].next = i + 1;
    }

    for (i = 2; i < list->size + 1; i++)
    {
        data[i].prev = i - 1;
    }

    for (i = list->size + 1; i < list->capacity; i++)
    {
        data[i].elem = 0;
        data[i].prev = LIST_PLACE_FREE;
    }

    for (i = list->size + 1; i < list->capacity - 1; i++)
    {
        data[i].next = i + 1;
    }

    free(list->data);
    list->data = data;
    if (list->size == list->capacity)
    {
        list->freePlace = 0;
    }
    else
    {
        list->freePlace = list->size + 1;
    }
    list->isSorted = 1;

    #ifdef DEBUG
        ListDump(list);
    #endif // DEBUG

    return LIST_NO_ERROR;
}

int ListConvertLogicalForPhysNum(const List_t *list, const int place, ListErrorCode *listError)
{
    if (place > list->size)
    {
        return -1;
    }

    if (list->isSorted == 1)
    {
        if (list->data[place].prev != -1)
            return place;
        else
            return list->head;
    }

    int curPlace = list->head;
    for (int i = 1; i < place; i++)
    {
        curPlace = list->data[curPlace].next;
    }

    return curPlace;
}

static ListErrorCode ListResizeUp(List_t *list)
{
    assert(list != nullptr);

    list->capacity = list->capacity * LIST_RESIZE_UP_COEFFICIENT;
    ListNode_t *data = (ListNode_t*)realloc(list->data, list->capacity * sizeof(ListNode_t));
    if (data == nullptr)
    {
        return LIST_RESIZE_UP_ERROR;
    }
    list->data = data;
    list->freePlace = list->size + 1;

    size_t i = 0;
    for (i = list->freePlace; i < list->capacity; i++)
    {
        list->data[i].elem = 0;
        list->data[i].prev = LIST_PLACE_FREE;
    }

    for (i = list->freePlace; i < list->capacity - 1; i++)
    {
        list->data[i].next = i + 1;
    }

    return LIST_NO_ERROR;
}

ListErrorCode ListInsertAfter(List_t *list, int *physNum, const structElemT elem, const int place)
{
    ASSERT_OK_(list);

    if (list->freePlace == 0)
    {
        ListErrorCode listError = ListResizeUp(list);
        if (listError != LIST_NO_ERROR)
            return listError;
    }

    if (list->data[place].prev == LIST_PLACE_FREE)
    {
        return LIST_INSERT_UNCORRECT_PLACE;
    }

    if (physNum != nullptr)
    {
        *physNum = list->freePlace;
    }
    int freePlace = list->freePlace;
    list->freePlace = list->data[freePlace].next;

    list->data[freePlace].next = list->data[place].next;
    list->data[freePlace].prev = place;
    if (list->size == 0)
    {
        list->head = freePlace;
    }
    else
    {
        list->data[place].next = freePlace;
    }

    if (list->tail == place)
    {
        list->tail = freePlace;
    }
    else
    {
        list->data[list->data[freePlace].next].prev = freePlace;
        list->isSorted = 0;
    }
    list->data[freePlace].elem = elem;
    list->size = list->size + 1;

    #ifdef DEBUG
        ListDump(list);
    #endif // DEBUG

    ASSERT_OK_(list);

    return LIST_NO_ERROR;
}

ListErrorCode ListInsertBefore(List_t *list, int *physNum, const structElemT elem, const int place)
{
    ASSERT_OK_(list);

    if (list->size == 0)
    {
        return LIST_INSERT_BEFORE_UNCORRECT_USE;
    }

    if (list->freePlace == 0)
    {
        ListErrorCode listError = ListResizeUp(list);
        if (listError != LIST_NO_ERROR)
            return listError;
    }

    if (list->data[place].prev == LIST_PLACE_FREE)
    {
        return LIST_INSERT_UNCORRECT_PLACE;
    }

    if (physNum != nullptr)
    {
        *physNum = list->freePlace;
    }
    int freePlace = list->freePlace;
    list->freePlace = list->data[freePlace].next;

    list->data[freePlace].prev = list->data[place].prev;
    list->data[place].prev = freePlace;
    list->data[freePlace].next = place;
    if (list->head == place)
    {
        list->head = freePlace;
    }
    else
    {
        list->data[list->data[freePlace].prev].next = freePlace;
    }
    list->data[freePlace].elem = elem;
    list->size = list->size + 1;
    list->isSorted = 0;

    #ifdef DEBUG
        ListDump(list);
    #endif // DEBUG

    ASSERT_OK_(list);

    return LIST_NO_ERROR;
}

ListErrorCode ListRemove(List_t *list, structElemT *elem, const int place)
{
    ASSERT_OK_(list);

    assert(elem != nullptr);

    if (list->size <= 0)
    {
        return LIST_IS_EMPTY;
    }

    if (list->data[place].prev == LIST_PLACE_FREE)
    {
        return LIST_REMOVE_UNCORRECT_PLACE;
    }

    int prev = list->data[place].prev;

    *elem = list->data[place].elem;
    list->data[place].elem = 0;
    if (prev == 0)
    {
        list->size = list->size - 1;
        list->data[list->data[place].next].prev = list->data[place].prev;
        if (list->head == list->tail)
        {
            list->head = 0;
            list->tail = 0;
        }
        else
        {
            list->head = list->data[place].next;
        }
    }
    else
    {
        list->size = list->size - 1;
        list->data[prev].next = list->data[place].next;
        if (list->tail == place)
        {
            list->tail = list->tail - 1;
        }
        else
        {
            list->data[list->data[place].next].prev = list->data[place].prev;
            list->isSorted = 0;
        }
    }

    list->data[place].next = list->freePlace;
    list->data[place].prev = LIST_PLACE_FREE;
    list->freePlace = place;

    #ifdef DEBUG
        ListDump(list);
    #endif // DEBUG

    ASSERT_OK_(list);

    return LIST_NO_ERROR;
}
