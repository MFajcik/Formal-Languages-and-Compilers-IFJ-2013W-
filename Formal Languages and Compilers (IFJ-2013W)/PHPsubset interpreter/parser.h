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

#ifndef _SYNTAX_ANALYZER_H_
#define _SYNTAX_ANALYZER_H_

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"
#include "structs.h"

tKey generate_tkey(unsigned* counter);
int parse();
int prog();
int body();
int dekfun();
int idlist(tfData* fundata);
int idn(tfData* fundata);
int stat();
int _else();
int expr(int* save_expr);
int exprf(tKey destkey,int* save_expr);
int fun(tKey destkey);
int termlist(tKey funkey);
int termn(tKey funkey);
int litexp();
int term();
int strform();
int strflist(int* write_count, tKey destkey);
int inlinefun(tKey destkey);
int expression();
int reduction (tPstack* stack, tPstack* tmpstack);
tIndex Convert_token(tToken token, int* counter);

#endif
