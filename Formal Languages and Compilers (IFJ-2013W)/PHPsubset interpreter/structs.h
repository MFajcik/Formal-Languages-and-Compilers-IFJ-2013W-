/**
 * Predmet:   IFJ / IAL
 * Projekt:   Implementace interpretu jazyka IFJ13
 * Varianta:  a/1/I
 * Soubor:    structs.h
 * Popis:     Soubor s definicemi struktur celeho programu
 * Datum:     9.10.2013
 * Autori:     Martin Kalabek            <xkalab06@stud.fit.vutbr.cz>
 *             Martin Facik              <xfajci00@stud.fit.vutbr.cz>
 */

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "collector.h"
#include "ial.h"
#include "ilist.h"
#include "scanfun.h"

#define INDEXCOUNT 15

// Precedence table indexes
typedef enum
{
  PLUS,
  MINUS,
  MUL,
  DIV,
  CONCAT,
  EQUAL,
  NOTEQUAL,
  GREATER,
  LESSER,
  GREATEREQ,
  LESSEREQ,
  LPAR,
  RPAR,
  ID,
  ENDMARKER,
  LESS, //dummy index for < in stack
  NONTERM,
  ERROR,
}tIndex;


// token stack structs
typedef struct _stack
{
    tToken token;
    struct _stack* next;
}_tTelement;

typedef struct{
    _tTelement* top;
}tTstack;

// precedence analysis index stack structs
typedef struct _pstack
{
    tIndex element;
    struct _pstack* next;
}_tPelement;

typedef struct{
    _tPelement* top;
}tPstack;

// Token and precedence stack interface
void Tstackinit(tTstack* tstack);
void Pstackinit(tPstack* pstack);
int Tstackempty(tTstack* tstack);
int Pstackempty(tPstack* pstack);
void Tpush(tTstack* tstack, tToken token);
void Ppush(tPstack* pstack, tIndex index);
void Tpop(tTstack* tstack);
void Ppop(tPstack* pstack);
tToken Ttoppop(tTstack* tstack);
tIndex Ptoppop(tPstack* pstack);

/** \brief Global variables
 *  tST - Symbol Table
 *  tFT - Function Table
 *  tListOfInstr - Instruction list
 *  FILE* - pointer onto source file
 */
extern tST ST;
extern tFT FT;
extern tListOfInstr instrList;
extern FILE* file;

/** \brief Table of precedence
 *  G - Greater precedence
 *  L - Lesser precedence
 *  E - Equal precedence
 *  U - Unknown precedence (error)
 */
typedef enum
{
    G,
    L,
    E,
    U
}tPriorityindex;

extern const tPriorityindex Ptable [INDEXCOUNT][INDEXCOUNT];

#endif
