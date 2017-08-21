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

#ifndef _LEXICAL_ANALYZER_H_
#define _LEXICAL_ANALYZER_H_

#define HEXSIZE 3 //HEXSIZE=3,2 numbers + \0 character

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanfun.h"

// token states
typedef enum
{
    // state constants
//S    prefix legend: s=state(final), te=state(temporary)
/*0*/     sErr,    //  Error state
/*1*/     sIdent,  //  a*isAlNum() found
/*2*/     sCnull,  //  Constant null found
/*3*/     sVar,    //  a*isAlNum() found after $
/*4*/     sKelse,  //  Keyword else found
/*5*/     sNoteq,  //  == found after !
/*6*/     sNeg,    //  ONLY ! found
/*7*/     sAdd,    //  + found
/*8*/     sSub,    //  - found
/*9*/     sMul,    //  * found
/*10*/    sDiv,    //  / found
/*11*/    sLpar,   //  ( found     ///CHYBA koment,?,||,
/*12*/    sRpar,   //  ) found
/*13*/    sLbr,    //  { found
/*14*/    sRbr,    //  } found
/*15*/    sCat,    //  . found
/*16*/    sComm,   //  , found
/*17*/    sScln,   //  ; found
/*18*/    sMore,   //  ONLY > found
/*19*/    sMoreq,  //  = found after >
/*20*/    sLess,   //  ONLY < found
/*21*/    sLesseq, //  = found after <
/*22*/    sAssign, //  ONLY = found
/*23*/    sEq,     //  = found after == (Note: Logical Equation in IFJ13 is "===" string (without quotes))
/*24*/    sStart,  //  <?php found
/*25*/    sStr,    //  "string form" found
/*26*/    sEof,    //  EOF found
/*27*/    sInt,    //  ONLY a*IsDigit() found
/*28*/    sDbl,    //  a*IsDigit() found after a*IsDigit().
/*29*/    sCtrue,  //  Constant true found
/*30*/    sCfalse, //  Constant false found
/*31*/    sKfun,   //  Keyword function found
/*32*/    sKif,    //  Keyword if found
/*33*/    sKret,   //  Keyword return found
/*34*/    sKwhile, //  Keyword while found
/*35*/    sINboolval,   // inbuilt functions found
/*36*/    sINdoubleval,
/*37*/    sINintval,
/*38*/    sINstrval,
/*39*/    sINgetstring,
/*40*/    sINstrlen,
/*41*/    sINgetsubstring,
/*42*/    sINfindstring,
/*43*/    sINsortstring,
/*44*/    sINputstring
}tState;

// token struct
typedef struct
{
    tState state;
    void* data;
}tToken;

/*
 * konstanty a ich retazcove reperentacie
 */
enum consts
{
    C_NULL,
    C_TRUE,
    C_FALSE,
};

extern const char* const constants[];

/*
 * klucove slova a ich retazcove reperentacie
 */
enum keyid
{
    K_ELSE,
    K_FUNCTION,
    K_IF,
    K_RETURN,
    K_WHILE,
};

extern const char* const keys[];

/*
 * vstavane funkcie a ich retazcove reperentacie
 */
enum inbuilt
{
    IN_BOOLVAL,
    IN_DOUBLEVAL,
    IN_INTVAL,
    IN_STRVAL,
    IN_GETSTRING,
    IN_STRLEN,
    IN_GETSUBSTRING,
    IN_FINDSTRING,
    IN_SORTSTRING,
    IN_PUTSTRING,
};

extern const char* const inbuilt[];

void Ungettoken(tToken token);
void NextToken(tToken* token, FILE* file);

#endif
