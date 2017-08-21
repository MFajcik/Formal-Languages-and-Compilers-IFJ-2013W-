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

#ifndef _COLLECTOR_H_
#define _COLLECTOR_H_

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "signal.h"

void* sMalloc(size_t size);
void* sRealloc(void* ptr, size_t size);
void sMemInsert(void* result);
void sMemDelete(void* ptr);
void sMemRepair(void* result, void* ptr);
void ClearAndExit(int exitCode);
void signalHandler();

typedef struct memoryPtr
{
    void* data;
    struct memoryPtr* nextPtr;
} MemoryElem;

#endif
