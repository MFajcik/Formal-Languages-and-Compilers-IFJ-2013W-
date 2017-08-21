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

#include "error.h"
#include "ilist.h"
#include "interpret.h"
#include "parser.h"
#include "scanner.h"
#include "signal.h"
#include "structs.h"

tST ST;
tFT FT;
tListOfInstr instrList;
FILE* file;

const char* const err[] =
{
    [ARGC] =         "ERROR: Invalid argument recieved. \n",
    [MALLOC] =       "ERROR: Malloc()'s dynamic memory alocation failed. \n",
    [REALLOC] =      "ERROR: Realloc()'s dynamic memory relocation failed. \n",
    [FOPEN] =        "ERROR: Fopen(), failed to open file. \n",
    [LEX_ERROR] =    "Error occured during lexical analysis. \n",
    [SYNTAX_ERROR] = "Error occured during syntactic analysis. \n",
    [SEM_ERROR_3_1] = "Error occured during semantic analysis. Undefined function is called. \n",
    [SEM_ERROR_3_2] = "Error occured during semantic analysis. Attempt to redefine function detected. \n",
    [SEM_ERROR_4]   = "Error occured during semantic analysis. Missing parameter at calling function. \n",
    [SEM_ERROR_5]   = "Error occured during semantic analysis. Undeclared variable found. \n",
    [SEM_ERROR_10]  = "Error occured during semantic analysis. Attempt to divide by zero detected. \n",
    [SEM_ERROR_11]  = "Error occured during semantic analysis. Cast failed. \n",
    [SEM_ERROR_12]  = "Error occured during semantic analysis. Operator type compatility error detected. \n",
    [SEM_ERROR_13]  = "Error occured during semantic analysis. Other semantic/runtime error. \n",
    [SIGINT_TRAP]   = "Signal SIGINT trapped, clearing resources and exiting ...\n",
};

int main(int argc,char** argv)
{
     signal(SIGINT, signalHandler);
     if (argc == 1)
     {
         fprintf(stderr, err[ARGC]);
         return INTERNAL_ERROR;
     }
     file = fopen(argv[1],"r");
     if (file == NULL)
     {
         fprintf(stderr, err[FOPEN]);
         return INTERNAL_ERROR;
     }

     tableInit(&ST);  // initialiaze symbol tamble
     fTableInit(&FT); // function table initialization
     listInit(&instrList);// initialiaze instruction list

     // start syntactic analysis
     int result;
     result = parse(&ST,&FT,&instrList,file);
     if (result == SYNTAX_ERROR)
     {
        // error occured during analysis
        ClearAndExit(result);
     }

     // interpret instructions
     listFirst(&instrList);
     inter(&ST);

     ClearAndExit(OK);

     return OK;
}
