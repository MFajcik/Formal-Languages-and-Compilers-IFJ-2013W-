/**
 * Predmet : IFJ/IAL
 * Projekt : Implementace interpretu imperativniho jazyka IFJ13
 * Varianta: a/3/II
 * Datum   : 15.12.2013
 * Autori  : Pavel Beran    <xberan33>
 *           Martin Kalabek <xkalab06>
 *           Martin Fajcik  <xfajci00>
 *           Tomas Vojtech  <xvojte02>
 *           Ondrej Soudek  <xsoude01>
 */

#include "collector.h"
#include "structs.h"

MemoryElem* first = NULL;
MemoryElem* last = NULL;

void* sMalloc(size_t size)
{
    void* result = malloc(size);
    if (result == NULL)
    {
        fprintf(stderr, "%s", err[MALLOC]);
        ClearAndExit(INTERNAL_ERROR);
    }

    sMemInsert(result);

    return result;
}

void* sRealloc(void* ptr, size_t size)
{
    void* result = NULL;
    if (ptr == NULL)
    {
        result = sMalloc(size);
        return result;
    }
    else
    {
        result = realloc(ptr, size);
        if (result == NULL)
        {
            fprintf(stderr, "%s", err[REALLOC]);
            ClearAndExit(INTERNAL_ERROR);
        }

        if (result != ptr)
            sMemRepair(result, ptr);

        return result;
    }
}

void sMemInsert(void* result)
{
    // insert at the end of the list
    MemoryElem* pom = malloc(sizeof(MemoryElem));
    if (!pom)
    {
        fprintf(stderr, "%s", err[MALLOC]);
        ClearAndExit(INTERNAL_ERROR);
    }

    pom->data = result;
    pom->nextPtr = NULL;

    if (!first)
        first = pom;

    if (last)
        last->nextPtr = pom;

    last = pom;
}

void sMemDelete(void* ptr)
{
    MemoryElem* me = first;
    while (me != NULL) // until list is empty
    {
        if (me->data == ptr) // if data found
            me->data = NULL; // NULL it

        me = me->nextPtr; // move to another element
    }
}

void sMemRepair(void* result, void* ptr)
{
    // find the old
    sMemDelete(ptr);
    // insert new
    sMemInsert(result);
}

void ClearAndExit(int exitCode)
{
    if (exitCode != OK)
        fprintf(stderr,err[exitCode]);
    MemoryElem* me = first;
    MemoryElem* pom = NULL;
    while (me != NULL) // until list is empty
    {
        if (me->data != NULL) // if data exists - should always happen
            free(me->data); // free them

        pom = me;
        me = me->nextPtr; // move to another element
        free(pom); // free element
    }

    fclose(file);
    switch (exitCode)
    {
        case OK:
        case LEX_ERROR:
        case SYNTAX_ERROR:
            exit(exitCode);
            break;
        case SEM_ERROR_3_1:
        case SEM_ERROR_3_2:
            exit(3);
            break;
        case SEM_ERROR_4:
            exit(4);
            break;
        case SEM_ERROR_5:
            exit(5);
            break;
        case SEM_ERROR_10:
            exit(10);
            break;
        case SEM_ERROR_11:
            exit(11);
            break;
        case SEM_ERROR_12:
            exit(12);
            break;
        case SEM_ERROR_13:
            exit(13);
            break;
        case SIGINT_TRAP:
        case INTERNAL_ERROR:
            exit(INTERNAL_ERROR);
        default:
            exit(exitCode);
    }
}

void signalHandler()
{
    ClearAndExit(SIGINT_TRAP);
}
