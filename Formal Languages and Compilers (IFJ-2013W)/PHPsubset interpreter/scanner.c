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

#include "scanner.h"
#include "structs.h"
#define STRSTANDARD 31
tTstack tstack;

const char* const keys[] =
{
    [K_ELSE] = "else",
    [K_FUNCTION] = "function",
    [K_IF] = "if",
    [K_RETURN] = "return",
    [K_WHILE] = "while"
};

const char* const constants[] =
{
    [C_NULL] = "null",
    [C_TRUE] = "true",
    [C_FALSE] = "false"
};

void Ungettoken(tToken token)
{
    Tpush(&tstack,token);
}

void NextToken(tToken* token,FILE* file)
{
    if (!Tstackempty(&tstack))   // for ungettoken
    {
        (*token)=Ttoppop(&tstack);
        return;
    }
    Tstackinit(&tstack);
    char character;
    char* string;
    char istart=0;
    int i=0;
    while (((character=fgetc(file))!=EOF))
    {
        if (character <= STRSTANDARD) // ignore some bullshit characters(debuger -64 "<<" etc..),probably windows added crap
        {
            istart=1;
            continue;
        }
        if (!isspace(character))
        {
            if (isalpha(character) || character == '_')    //temp_ident state
            {
                add_to_str(&character,&string,&i); //write down the character (in the end string pointer gonna be send into table)
                character=fgetc(file); //taking next character
                while(isalnum(character)|| character == '_') //now we will cycle and buffer characters until identifier ends
                {
                    add_to_str(&character,&string,&i);
                    character=fgetc(file);
                }
                ungetc(character,file);    //next character was invalid, returning to previous position
                add_to_str(NULL,&string,&i);//pushing NULL indicates function to free remaining redundant space by realloc
                // Constant?
                if (!strcmp(string,constants[C_NULL]))
                    token->state=sCnull;
                else if (!strcmp(string,constants[C_TRUE]))
                    token->state=sCtrue;
                else if (!strcmp(string,constants[C_FALSE]))
                    token->state=sCfalse;
                // Keyword?
                else if (!strcmp(string,keys[K_ELSE]))
                    token->state=sKelse;
                else if (!strcmp(string,keys[K_FUNCTION]))
                    token->state=sKfun;
                else if (!strcmp(string,keys[K_IF]))
                    token->state=sKif;
                else if (!strcmp(string,keys[K_RETURN]))
                    token->state=sKret;
                else if (!strcmp(string,keys[K_WHILE]))
                    token->state=sKwhile;
                //Inbuilt functions
                else if (!strcmp(string,inbuilt[IN_BOOLVAL]))
                    token->state=sINboolval;
                else if (!strcmp(string,inbuilt[IN_DOUBLEVAL]))
                    token->state=sINdoubleval;
                else if (!strcmp(string,inbuilt[IN_INTVAL]))
                    token->state=sINintval;
                else if (!strcmp(string,inbuilt[IN_STRVAL]))
                    token->state=sINstrval;
                else if (!strcmp(string,inbuilt[IN_GETSTRING]))
                    token->state=sINgetstring;
                else if (!strcmp(string,inbuilt[IN_STRLEN]))
                    token->state=sINstrlen;
                else if (!strcmp(string,inbuilt[IN_GETSUBSTRING]))
                    token->state=sINgetsubstring;
                else if (!strcmp(string,inbuilt[IN_FINDSTRING]))
                    token->state=sINfindstring;
                else if (!strcmp(string,inbuilt[IN_SORTSTRING]))
                    token->state=sINsortstring;
                else if (!strcmp(string,inbuilt[IN_PUTSTRING]))
                    token->state=sINputstring;
                else
                    token->state=sIdent;    //setting up ident status

                token->data=(void*)string; //assigning data into void* pointer
                return;
            }

            if (isdigit(character))//temp_num state
            {
                unsigned int intnumber=0;//our language support only noumbers>=0
                add_to_int(character,&intnumber);
                character=fgetc(file);
                while(isdigit(character))   //buffering integer
                {
                    add_to_int(character,&intnumber);
                    character=fgetc(file);
                }
                switch(character)
                {
                    case '.':
                    {
                        double dblnumber=intnumber;//we will continue working with double
                        character=fgetc(file);
                        if (isdigit(character))//number is in correct form xx.x
                        {
                            int decmarkdistance=0; //distance from decimal mark
                            add_to_frac(character,&dblnumber,decmarkdistance);
                            decmarkdistance++;
                            character=fgetc(file);
                            while(isdigit(character))   //buffering fractional part
                            {
                                add_to_frac(character,&dblnumber,decmarkdistance);
                                decmarkdistance++;
                                character=fgetc(file);
                            }
                            if (character=='e' || character=='E')
                            {
                                character=fgetc(file);
                                if (character=='-') // exponent <0
                                {
                                    unsigned int exponent=0;
                                    character=fgetc(file);
                                    if (isdigit(character))
                                    {
                                        while(isdigit(character))   //buffering exponent
                                        {
                                            add_to_int(character,&exponent);
                                            character=fgetc(file);
                                        }
                                        ungetc(character,file);
                                        calculate_minusexp_to_DBL(&dblnumber,exponent);// we will calculate number with e to standard double format xxx.xxxx
                                        token->state=sDbl;
                                        double* tmp = sMalloc(sizeof(double));            //assigning data into void* pointer
                                        *tmp = dblnumber;
                                        token->data=(void*)tmp;
                                        return;
                                    }
                                    else// our LA recieved smt like 11.2e- or 11.2e-A
                                    {
                                        token->state=sErr;
                                        ungetc(character,file);
                                        ClearAndExit(LEX_ERROR);
                                        return;
                                    }

                                }
                                else if (character=='+' || isdigit(character)) //exponent >=0
                                {
                                    unsigned int exponent=0;
                                    if (character=='+')
                                        character=fgetc(file);
                                    if (isdigit(character)) //we left it here for +smt exponent
                                    {
                                        while(isdigit(character))   //buffering exponent
                                        {
                                            add_to_int(character,&exponent);
                                            character=fgetc(file);
                                        }
                                        ungetc(character,file);
                                        calculate_exp_to_DBL(&dblnumber,exponent);// we will calculate number with e to standard double format xxx.xxxx
                                        token->state=sDbl;
                                        double* tmp = sMalloc(sizeof(double));            //assigning data into void* pointer
                                        *tmp = dblnumber;
                                        token->data=(void*)tmp;
                                        return;
                                    }
                                    else// our LA recieved smt like 11.2e+ or 11.2e+A
                                    {
                                        token->state=sErr;
                                        ungetc(character,file);
                                        ClearAndExit(LEX_ERROR);
                                        return;
                                    }
                                }
                                else //in case of xxxxx.xe
                                {
                                    token->state=sErr;
                                    ungetc(character,file);
                                    ClearAndExit(LEX_ERROR);
                                    return;
                                }
                            }//endif (character=='e' || character=='E')
                            else    //double was corretly buffered (no exp expression found)
                            {
                                ungetc(character,file);
                                token->state=sDbl;

                                double* tmp=sMalloc(sizeof(double));            //assigning data into void* pointer
                                *tmp = dblnumber;
                                token->data=(void*)tmp;
                                return;
                            }
                        }
                        else //incorrect number form xx.<nothing>
                        {
                          token->state=sErr;
                          ungetc(character,file);
                          ClearAndExit(LEX_ERROR);
                          return;
                        }
                    }
                    case 'e':
                    case 'E':
                    {
                        double dblnumber=intnumber;
                        character=fgetc(file);
                        if (character=='-') // exponent <0
                        {
                            unsigned int exponent=0;
                            character=fgetc(file);
                            if (isdigit(character))
                            {
                                while(isdigit(character))   //buffering exponent
                                {
                                    add_to_int(character,&exponent);
                                    character=fgetc(file);
                                }
                                ungetc(character,file);
                                calculate_minusexp_to_DBL(&dblnumber,exponent);// we will calculate number with e to standard double format xxx.xxxx
                                token->state=sDbl;
                                double* tmp=sMalloc(sizeof(double));            //assigning data into void* pointer
                                *tmp = dblnumber;
                                token->data=(void*)tmp;
                                return;
                            }
                            else// our LA recieved smt like 11.2e- or 11.2e-A
                            {
                                token->state=sErr;
                                ungetc(character,file);
                                ClearAndExit(LEX_ERROR);
                                return;
                            }
                        }
                        else if (character=='+' || isdigit(character)) //exponent >=0
                        {
                            unsigned int exponent=0;
                            if (character=='+')
                                character=fgetc(file);
                            if (isdigit(character)) //we left it here for +smt exponent
                            {
                                while(isdigit(character))   //buffering exponent
                                {
                                    add_to_int(character,&exponent);
                                    character=fgetc(file);
                                }
                                ungetc(character,file);
                                calculate_exp_to_DBL(&dblnumber,exponent);// we will calculate number with e to standard double format xxx.xxxx
                                token->state=sDbl;
                                double* tmp=sMalloc(sizeof(double));            //assigning data into void* pointer
                                *tmp = dblnumber;
                                token->data=(void*)tmp;
                                return;
                            }
                            else// our LA recieved smt like 11.2e+ or 11.2e+A
                            {
                                token->state=sErr;
                                ungetc(character,file);
                                ClearAndExit(LEX_ERROR);
                                return;
                            }
                        }
                        else //in case of xxxxx.xe
                                {
                                    token->state=sErr;
                                    ungetc(character,file);
                                    ClearAndExit(LEX_ERROR);
                                    return;
                                }
                    }
                    default:
                    {
                        ungetc(character,file);
                        token->state=sInt;//just bare numbers? its integer

                        int* tmp=sMalloc(sizeof(int));           //assigning data into void* pointer
                        *tmp=intnumber;
                        token->data=(void*)tmp;
                        return;
                    }
                }
            }

            switch(character)
            {
            case '$':    //temp_var state, token.status=teVar might be used here, but its not necessary
                {
                    character=fgetc(file); //taking next character
                    if (isalnum(character)|| character == '_')    //is character after $ alnum()?
                    {
                        add_to_str(&character,&string,&i);
                        character=fgetc(file);
                        while(isalnum(character) || character == '_')    //temp_var1 state
                        {
                            add_to_str(&character,&string,&i);
                            character=fgetc(file);
                        }
                            add_to_str(NULL,&string,&i);//pushing NULL indicates function to free remaining redundant space by realloc
                            ungetc(character,file);    //next character was invalid, returning to previous position
                            token->state=sVar;    //setting up status
                            token->data=(void*)string;//assigning data into void* pointer
                    }
                    else
                    {
                            ungetc(character,file);    //next character was invalid, returning to previous position
                            token->state=sErr;    //setting up status
                            ClearAndExit(LEX_ERROR);
                    }
                    return;
                }
            case '=':    //temp_eq state
                {
                    character=fgetc(file);
                    if (character == '=')    //temp_eq1 state
                    {
                        character=fgetc(file);
                        if (character == '=')    //state eql
                            token->state=sEq;
                        else
                        {
                            token->state=sErr;    //ERR state
                            ungetc(character,file);
                            ClearAndExit(LEX_ERROR);
                        }
                    }
                    else                    //state_assign
                    {
                        ungetc(character,file);
                        token->state=sAssign;
                    }
                    return;
                }
            case '<':    //temp_less state
                {
                    character=fgetc(file);
                    if (character == '=')    //state_lessq
                        token->state=sLesseq;
                    else if (character == '?')  // start symbol <?php
                    {
                        if (fgetc(file)=='p' && fgetc(file)=='h' && fgetc(file)=='p')
                        {
                            if (!istart)//no character b4 < expected
                                token->state=sStart;
                            else
                                ClearAndExit(SYNTAX_ERROR);
                        }
                        else
                        {
                            token->state=sErr; // its Error w characters taken  are not returned into the buffer !!!
                            ClearAndExit(LEX_ERROR);
                        }
                    }
                    else
                    {
                        token->state=sLess;    //state_less
                        ungetc(character,file);
                    }
                    return;
                }
            case '>':    //temp_more state
                {
                    character=fgetc(file);
                    if (character == '=')    //state_moreq
                        token->state=sMoreq;
                    else
                    {
                        token->state=sMore;    //state_more
                        ungetc(character,file);
                    }
                    return;
                }
            case ')':    //state_lprnt
                {
                    token->state=sRpar;
                    return;
                }
            case '(':    //state_lprnt
                {
                    token->state=sLpar;
                    return;
                }
            case '}':    //state_lbr
                {
                    token->state=sRbr;
                    return;
                }
            case '{':    //state_rBr
                {
                    token->state=sLbr;
                    return;;
                }
            case ';':    //state_smcln
                {
                    token->state=sScln;
                    return;
                }
            case '.':    //state_cat
                {
                    token->state=sCat;
                    return;
                }
            case ',':   //state_comm
                {
                    token->state=sComm;
                    return;
                }
            case '/':    //state_div
                {
                    character=fgetc(file);
                    if (character == '/')//skip 1 line comments
                    {
                        do
                        {
                            character=fgetc(file);
                        }while(character != '\n' && character != EOF);
                        break;
                    }
                    else if (character=='*') //skip multi line comments
                    {
                        do
                        {
                            character=fgetc(file);
                            if (character=='*')
                            {
                                character=fgetc(file);
                                if (character=='/')
                                    break;//we dont need to ungetc, comment characters are invalid
                            }

                        }while(character != EOF);
                    }
                    else
                    {
                        ungetc(character,file);
                        token->state=sDiv;
                        return;
                    }
                    break;
                }
            case '*':    //state_mul
                {
                    token->state=sMul;
                    return;
                }
            case '-':    //state_sub
                {
                    token->state=sSub;
                    return;
                }
            case '+':    //state_add
                {
                    token->state=sAdd;
                    return;
                }
            case '!':    //temp_neg state
                {
                    character=fgetc(file);
                    if (character == '=')  //temp_exclm
                    {
                        character=fgetc(file);
                        if (character == '=') //state_exclm
                            token->state=sNoteq;
                        else                  //ERR state
                        {
                            token->state=sErr;
                            ungetc(character,file);
                            ClearAndExit(LEX_ERROR);
                        }

                    }
                    else                    //state_neg
                    {
                        token->state=sNeg;
                        ungetc(character,file);
                    }
                    return;
                }
            case '"':    //temp_string
                {
                    character=fgetc(file);
                    while( character!=EOF && character!='"' )//string buffering
                    {
                        if (character == '\\')  //temp_esc
                        {
                           character=fgetc(file);
                           switch(character)    //escape sequences
                           {
                               case '\\':
                               case '$' :
                               case '"' :
                               {
                                   add_to_str(&character,&string,&i);
                                   break;
                               }
                               case 't':
                               {
                                   character='\t';
                                   add_to_str(&character,&string,&i);
                                   break;
                               }
                               case 'n':
                               {
                                   character='\n';
                                   add_to_str(&character,&string,&i);
                                   break;
                               }
                               case 'x':    //hexadecimal expected
                               {
                                   char* tmp=sMalloc(HEXSIZE*sizeof(char));//HEXSIZE=3,2 noumbers + \0 character
                                   tmp[2]='\0';// String ending char
                                   tmp[0]=fgetc(file);//trying first character
                                   if (ishex(tmp[0]))
                                   {
                                        tmp[1]=fgetc(file);
                                        if(ishex(tmp[1]))//trying second character
                                        {
                                            character=strtol(tmp,NULL,16); //strol in this case wont return value greater then 255
                                            add_to_str(&character,&string,&i);
                                        }
                                        else
                                        {
                                            ungetc(tmp[1],file);
                                            character='\\';// we know that b4 fgetc was there must be \x to get into this f
                                                           //so we will insert it manually
                                            add_to_str(&character,&string,&i);
                                            character='x';
                                            add_to_str(&character,&string,&i);
                                            add_to_str(tmp,&string,&i); //*tmp=tmp[0]; we also need to insert this when tmp[1] is not hexa
                                        }

                                   }
                                   else
                                   {
                                        char chartmp=character;
                                        ungetc(tmp[0],file);
                                        character='\\';// we know that b4 fgetc was there must be \x to get into this f
                                                           //so we will insert it manually
                                        add_to_str(&character,&string,&i);
                                        character=chartmp;
                                        add_to_str(&character,&string,&i);
                                   }
                                   break;
                               }
                               default:
                               {
                                   ungetc(character,file);
                                   character='\\';// we know that b4 fgetc was character='\'
                                   add_to_str(&character,&string,&i);
                                   break;
                               }
                           }
                           character=fgetc(file);
                        }
                        else if (character<=STRSTANDARD || character == '$')
                            ClearAndExit(LEX_ERROR);
                        else
                        {
                            add_to_str(&character,&string,&i);
                            character=fgetc(file);
                        }
                    }
                    if (character == '"')   //string was loaded properly
                    {
                        token->state=sStr;
                        add_to_str(NULL,&string,&i);//pushing NULL indicates function to free remaining redundant space by realloc
                        token->data=(void*)string;//assigning data into void* pointer
                    }
                    else   // second " missing
                    {
                        token->state=sErr;
                        ClearAndExit(LEX_ERROR);
                    }
                    return;
                }
            }
        }
        istart=1;
    }
    token->state=sEof;
    return;
}
