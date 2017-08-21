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

#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

 /**************************************Error Codes*******************************************************
  * 1 - chyba v programu v ramci lexikalni analyzy (chybna struktura aktualniho lexemu).                 *
  * 2 - chyba v programu v ramci syntakticke analyzy (chybna syntaxe struktury programu).                *
  * 3 - semanticka chyba v programu - nedefinovana funkce, pokus o redefinici funkce.                    *
  * 4 - semanticka/behova chyba v programu - chybejici parametr pri volani funkce.                       *
  * 5 - semanticka/behova chyba v programu - nedeklarovana promenna.                                     *
  * 10 - semanticka/behova chyba deleni nulou.                                                           *
  * 11 - semanticka/behova chyba pri pretypovani na cislo (funkce doubleval).                            *
  * 12 - semanticka/behova chyba typove kompatibility v aritmetickych a relacnich vyrazech.              *
  * 13 - ostatni semanticke/behove chyby
  * 99 - interni chyba interpretu                                                                        *
  ********************************************************************************************************/

/*
 * Errors array ...
 */
enum errcodes
{
    OK,
    LEX_ERROR,
    SYNTAX_ERROR,
    SEM_ERROR_3_1,      // undefined function
    SEM_ERROR_3_2,      // attemp to redefine function
    SEM_ERROR_4,        // missing parameter in
    SEM_ERROR_5,        // undeclared variable
    SEM_ERROR_10 = 10,  // dividing by zero
    SEM_ERROR_11,       // cast failed (doubleval error)
    SEM_ERROR_12,       // type compatibility failed - examle: "hello" + 1
    SEM_ERROR_13 = 13,
    SIGINT_TRAP,
    ARGC,
    MALLOC,
    REALLOC,
    FOPEN,
    INTERNAL_ERROR = 99,
};

extern const char* const err[];

#endif
