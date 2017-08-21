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

#ifndef _INTERPRET_H_
#define _INTERPRET_H_

#include <stdio.h>
#include <stdbool.h>
#include "collector.h"
#include "ial.h"
#include "ilist.h"
#include "structs.h"

int inter(tST* mainST);

#endif
