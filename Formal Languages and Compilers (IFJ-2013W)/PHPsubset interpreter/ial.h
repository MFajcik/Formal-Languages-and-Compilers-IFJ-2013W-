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

#ifndef _IAL_H_
#define _IAL_H_

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "collector.h"
#include "error.h"
#include "scanfun.h"
#include "scanner.h"

#define HTSIZE 101 // adequate HT size

/*
 * Seznam datovych typu
 */
enum varTypes
{
    TYPE_VAR = 0,
    TYPE_INT,
    TYPE_DBL,
    TYPE_BOOL,
    TYPE_NULL,
    TYPE_STR,
};

// definice klice do hashovaci tabulky
typedef char* tKey;

// structs for table of symbols
typedef struct
{
    int varType;
    void* varValue;
} tData;

typedef struct tableItem
{
    tKey key;
    tData* data;
    struct tableItem* nextItem;
} tTableItem;

typedef tTableItem* tST[HTSIZE];

// structs for table of functions
typedef struct
{
    int argc;   // argument count
    char* argv; // argument values (example: "a#B#C\0") delimiter #
} tfData;

typedef struct fTableItem
{
    tKey key;
    tfData* data;
    struct listItem* label;
    struct fTableItem* nextItem;
} tFTableItem;
typedef tFTableItem* tFT[HTSIZE];

int hashCode (tKey key);

// symbol table interface
void tableInit(tST* ST);
tTableItem* tableSearch(tST* ST, tKey key);
int tableInsert(tST* ST, tKey key, tData* data);
void tableFree(tST* ST);
void tableReInit(tST* ST);

// function table interface
void fTableInit(tFT* T);
int fTableInsert(tFT* T, tKey key, tfData* data);
tFTableItem* fTableSearch(tFT* T, tKey key);
tKey getArgKey (tFT* T, tKey key, int argn);
tKey getArgKeyFromData (tfData* fundata, int argn);
void fTableFree(tFT* T);

// adds argument name and increments argument number in tfData structure
void add_funarg(tfData* fundata, tToken* token);

bool boolval(tData* term);
double doubleval(tData* term);
int intval(tData* term);
char* strval(tData* term); // Je toto spravna navratova hodnota?
char* get_string();
//int put_string(char* string1, char* string2, ...);
int strleng(tData* term);
char* get_substring(char* string, unsigned int integer1 , unsigned int integer2);
int find_string(char* string, char* string2);
char* sort_string(tData* term);

#endif

