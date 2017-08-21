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

#ifndef _LA_FUNCTIONS_H_
#define _LA_FUNCTIONS_H_

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "collector.h"
#include "error.h"

// average string length (to prevent using realloc too much)
#define AVGLEN 32

char* hexatodec(char* string, int* index);
int ishex(char c);
void add_to_str(char *character, char ** string, int* index);
void add_to_int(char character, unsigned *number);
void add_to_frac(char character, double* number, unsigned int val);
void calculate_minusexp_to_DBL(double* dblnumber, unsigned exponent);
void calculate_exp_to_DBL(double* dblnumber, unsigned exponent);

#endif
