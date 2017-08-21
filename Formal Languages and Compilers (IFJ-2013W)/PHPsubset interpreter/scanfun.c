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

#include "scanfun.h"

/** \brief concatenates character with string
 *
 * \param character char* :character taken from the file stream
 * \param string char**   :previous string
 * \param index int*      :index where an actual character should be written at
 * \return void
 *
 */
void add_to_str(char *character, char ** string, int* index)
{
    //we dont expect  *string to be NULL !! no condition done
    if (*index == 0)    //if str is empty we will alloc AVGLEN(32) characters space
       (*string)=sMalloc(AVGLEN*sizeof(char));

    //string length exceeds 32 characters we will allocate more space
    if (*index > (AVGLEN-1) || character == NULL )// or string buffering ended, we will free redundant space
    {
        char* tmp;
        tmp = sRealloc(*string,sizeof(char)*((*index)+2));//+1 == '\0' & +1 bcs we index from 0
        (*string) = tmp;
        (*string)[(*index)] = '\0'; // gcc(windows) compilator doesnt add terminating null!
    }
    if (character!=NULL)
        (*string)[(*index)++] = *character;
}
/** \brief converts character variable into part of integer number
 *
 * \param character char   :character taken from the file stream
 * \param number unsigned* :previous int number
 * \return void
 *
 */
void add_to_int(char character, unsigned* number)
{
    *number = (*number)*10+(character-'0');
}
/** \brief appends fraction part to integer part
 *
 * \param character char :character taken from the file stream
 * \param number double* :previous double number (may already have frac)
 * \param val unsigned int :distance from decimal mark
 * \return void
 *
 */
void add_to_frac(char character, double* number, unsigned int val)
{   //val=distance from decimal mark
    double frac = 10.0;
    double charac = character;
    while(val > 0)
    {
        frac = frac*10; //we use frac to lower number by division (as e-val)
        val--;
    }
    *number=  (*number)+(charac-'0')/frac;
}
/** \brief Calculates whole double number from pormat xxe-xx to xx.xxx
 *
 * \param dblnumber double* :previous double number (may already have frac)
 * \param exponent unsigned :exponent in absolute value
 * \return void
 *
 */
void calculate_minusexp_to_DBL(double* dblnumber, unsigned exponent)
{
    if (exponent == 0)
        return;

    while(exponent > 0)   //we multiply dblnoumber by 10^exponent
    {
        *dblnumber = (*dblnumber)/10;
        exponent--;
    }
}
/** \brief Calculates whole double number from pormat xxe+xx to xx.xxx
 *
 * \param dblnumber double* :previous double number (may already have frac)
 * \param exponent unsigned :exponent in absolute value
 * \return void
 *
 */
void calculate_exp_to_DBL(double* dblnumber, unsigned exponent)
{
    if (exponent == 0)
        return;

    while(exponent>0)   //we multiply dblnoumber by 10^exponent
    {
        *dblnumber = (*dblnumber)*10;
        exponent--;
    }
}
/** \brief If character is hexadecimal number function returns true
 *
 * \param c char :character taken from the file stream
 * \return int
 *
 */
int ishex(char c)
{
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F'))     ? 1 : 0;
}
