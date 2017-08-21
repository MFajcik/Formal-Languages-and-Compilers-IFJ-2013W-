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

#ifndef _LIST_H_
#define _LIST_H_

#include <malloc.h>
#include <stdio.h>
#include "collector.h"

// instruction types
typedef enum
{
/*1*/  I_READ = 1,
/*2*/  I_WRITE,
/*3*/  I_ADD,      // is used for int and double - type need to be verified
/*4*/  I_SUB,      // is used for int and double - type need to be verified
/*5*/  I_MUL,      // is used for int and double - type need to be verified
/*6*/  I_DIV,      // is used for int and double - type need to be verified
/*7*/  I_MORE,     // is used for int and double - type need to be verified
/*8*/  I_LESS,     // is used for int and double - type need to be verified
/*9*/  I_MOREEQ,   // is used for int and double - type need to be verified
/*10*/ I_LESSEQ,   // is used for int and double - type need to be verified
/*11*/ I_EQUAL,    // is used for int and double - type need to be verified
/*12*/ I_NOTEQUAL, // is used for int and double - type need to be verified
/*13*/ I_LABEL,    // placeholder for labels in cycles (marks start and end of the body cycle) and functions, does nothing
/*14*/ I_PUSH,     // used for pushing arguments into functions
/*15*/ I_F_JUMP,   // for use on jumping into functions
/*16*/ I_RETURN,   // for use to get out of function
/*17*/ I_GOTO,     // for use on jumping through code(skipping function and cycle parts of interpret)
/*18*/ I_IFGOTO,   // for use on if constructions or in while cycle
/*19*/ I_SETVAL,   // used for storing values in symbol tables
       // all inline functions - to be implemented in ial.c
/*20*/ I_BOOLVAL,
/*21*/ I_DOUBLEVAL,
/*22*/ I_INTVAL,
/*23*/ I_STRVAL,
/*24*/ I_STRLEN,
/*25*/ I_SUBSTR,
/*26*/ I_FINDSTR,
/*27*/ I_SORTSTR,
/*28*/ I_CONCAT,
} instrCode;

// 3-AC instrution
typedef struct
{
  int instType;  // instruction type
  void *addr1; // adress 1
  void *addr2; // adress 2
  void *addr3; // adress 3
} tInstr;

// instruction list element
typedef struct listItem
{
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;

// instruction list
typedef struct
{
  struct listItem *first;
  struct listItem *last;
  struct listItem *active;
} tListOfInstr;

// interface for instruction list
void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
void listInsertLast(tListOfInstr *L, tInstr I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
void listGoto(tListOfInstr *L, void *gotoInstr);
void *listGetPointerLast(tListOfInstr *L);
tInstr *listGetData(tListOfInstr *L);
void Generate_instruction(tListOfInstr* IL,int instType,void* addr1,void* addr2,void* addr3);

#endif
