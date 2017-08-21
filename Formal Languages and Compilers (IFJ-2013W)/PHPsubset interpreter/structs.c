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

#include "structs.h"

const char* const inbuilt[] =
{
    [IN_BOOLVAL] = "boolval",
    [IN_DOUBLEVAL] = "doubleval",
    [IN_INTVAL] = "intval",
    [IN_STRVAL] = "strval",
    [IN_GETSTRING] = "get_string",
    [IN_STRLEN] = "strlen",
    [IN_GETSUBSTRING] = "get_substring",
    [IN_FINDSTRING] = "find_string",
    [IN_SORTSTRING] = "sort_string",
    [IN_PUTSTRING] = "put_string"
};

void Tstackinit(tTstack* tstack)
{
    tstack->top = NULL;
}

void Pstackinit(tPstack* pstack)
{
    pstack->top = NULL;
}

int Tstackempty(tTstack* tstack)
{
    return tstack->top == NULL;
}

int Pstackempty(tPstack* pstack)
{
    return pstack->top == NULL;
}

void Tpush(tTstack* tstack, tToken token)
{
    _tTelement* tmp = sMalloc(sizeof(struct _stack));
    tmp->token = token;
    tmp->next = tstack->top;
    tstack->top = tmp;
}

void Ppush(tPstack* pstack, tIndex index)
{
    _tPelement* tmp = sMalloc(sizeof(struct _pstack));
    tmp->element = index;
    tmp->next=pstack->top;
    pstack->top = tmp;
}

tToken Ttoppop(tTstack* tstack)
{
    tToken tmp = tstack->top->token;
    tstack->top = tstack->top->next;
    // element is freed at the end of program
    return tmp;
}

tIndex Ptoppop(tPstack* pstack)
{
    tIndex tmp = pstack->top->element;
    pstack->top = pstack->top->next;
    // element is freed at the end of program
    return tmp;
}

void Tpop(tTstack* tstack)
{
    tstack->top = tstack->top->next;
   // element is freed at the end of program
}

void Ppop(tPstack* pstack)
{
    pstack->top = pstack->top->next;
    // element is freed at the end of program
}
