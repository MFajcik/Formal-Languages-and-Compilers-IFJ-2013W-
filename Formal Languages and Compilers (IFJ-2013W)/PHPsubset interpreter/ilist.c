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

#include "ilist.h"

void listInit(tListOfInstr *L)
{
    L->first = NULL;
    L->last = NULL;
    L->active = NULL;
}

void listFree(tListOfInstr *L)
{
    printf("IL: "); // DEBUG
    while (L->first != NULL)
    {
        printf("%d ",L->first->Instruction.instType); // DEBUG
        L->first = L->first->nextItem;
        // element is freed at the end of program
    }
}

void listInsertLast(tListOfInstr *L, tInstr I)
{
    tListItem *newItem;
    newItem = sMalloc(sizeof (tListItem));
    newItem->Instruction = I;
    newItem->nextItem = NULL;
    if (L->first == NULL)
        L->first = newItem;
    else
        L->last->nextItem=newItem;
    L->last=newItem;
}

void listFirst(tListOfInstr *L)
{
    L->active = L->first;
}

void listNext(tListOfInstr *L)
{
    if (L->active != NULL)
        L->active = L->active->nextItem;
}

void listGoto(tListOfInstr *L, void *gotoInstr)
{
    L->active = (tListItem*) gotoInstr;
}

void *listGetPointerLast(tListOfInstr *L)
{
    return (void*) L->last;
}

tInstr *listGetData(tListOfInstr *L)
{
    if (!L->active)
        return NULL;

    return &(L->active->Instruction);
}
void Generate_instruction(tListOfInstr* IL,int instType,void* addr1,void* addr2,void* addr3)
{
    tInstr instruction;
    instruction.instType = instType;
    instruction.addr1 = addr1;
    instruction.addr2 = addr2;
    instruction.addr3 = addr3;
    listInsertLast(IL,instruction);
}
