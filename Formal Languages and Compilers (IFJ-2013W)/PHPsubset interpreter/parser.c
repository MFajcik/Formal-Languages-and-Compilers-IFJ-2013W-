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

#include "parser.h"

tToken token;
tTstack t_stack;
unsigned exprc = 0;
unsigned keygenc;
const tPriorityindex Ptable [INDEXCOUNT][INDEXCOUNT] =
{
    //	          +  -  *  /  . === !== >  < >= <=  (  ) ID  $
    /*1  +  */  { G, G, L, L, G, G,  G, G, G, G, G, L, G, L, G },
    /*2  -  */  { G, G, L, L, G, G,  G, G, G, G, G, L, G, L, G },
    /*3  *  */  { G, G, G, G, G, G,  G, G, G, G, G, L, G, L, G },
    /*4  /  */  { G, G, G, G, G, G,  G, G, G, G, G, L, G, L, G },
    /*5  .  */  { G, G, L, L, G, G,  G, G, G, G, G, L, G, L, G },
    /*6 === */  { L, L, L, L, L, G,  G, L, L, L, L, L, G, L, G },
    /*7 !== */  { L, L, L, L, L, G,  G, L, L, L, L, L, G, L, G },
    /*8  >  */  { L, L, L, L, L, G,  G, G, G, G, G, L, G, L, G },
    /*9  <  */  { L, L, L, L, L, G,  G, G, G, G, G, L, G, L, G },
    /*10 >= */  { L, L, L, L, L, G,  G, G, G, G, G, L, G, L, G },
    /*11 <= */  { L, L, L, L, L, G,  G, G, G, G, G, L, G, L, G },
    /*12 (  */  { L, L, L, L, L, L,  L, L, L, L, L, L, E, L, U },
    /*13 )  */  { G, G, G, G, G, G,  G, G, G, G, G, U, G, U, G },
    /*14 ID */  { G, G, G, G, G, G,  G, G, G, G, G, U, G, U, G },
    /*15 $  */  { L, L, L, L, L, L,  L, L, L, L, L, L, U, L, U }
};

tKey keygen(unsigned* counter)
{
    tKey key = sMalloc(7*sizeof(char)); //+5  max_unsigned length, +1  @ character and +1 \0
    if ((*counter)==UINT_MAX)
        exit(99);
    sprintf(key,"@%u",*counter);
    (*counter)++;
    return key;
}
int getType(tToken* token)
{
    switch (token->state)
    {
        case sInt:
            return TYPE_INT;
        case sDbl:
            return TYPE_DBL;
        case sStr:
            return TYPE_STR;
        case sCnull:
            return TYPE_NULL;
        case sCtrue:
        case sCfalse:
            return TYPE_BOOL;
        default:
            ClearAndExit(INTERNAL_ERROR);
            return INTERNAL_ERROR;
    }
}

int parse ()
{
    return prog();
}

int prog()
{
    //Rule 1:
    NextToken(&token,file);
    if (token.state != sStart)
        return SYNTAX_ERROR;
    if (body() == SYNTAX_ERROR)
        return SYNTAX_ERROR;
    NextToken(&token,file);
    return (token.state == sEof) ? EXIT_SUCCESS:SYNTAX_ERROR;
}

int body()
{
    NextToken(&token, file);
    if (token.state == sEof)
    {
        Ungettoken(token);
        return EXIT_SUCCESS;
    }
    if (stat()== SYNTAX_ERROR && dekfun() == SYNTAX_ERROR
        && stat() == SYNTAX_ERROR &&token.state !=sEof)
        return SYNTAX_ERROR;
    return body();
}

int dekfun()
{
    NextToken(&token,file);
    //Rule 2:
    if (token.state != sKfun)
    {
        Ungettoken(token);
        return SYNTAX_ERROR;
    }
    //Rule 3:
    NextToken(&token,file);
    if (token.state != sIdent)
        return SYNTAX_ERROR;
    tKey funkey = (tKey)token.data;
    tFTableItem* is_redefinition;
    is_redefinition = fTableSearch(&FT,funkey);
    if (is_redefinition && is_redefinition->data->argc != -1)   //redefinition?
        ClearAndExit(SEM_ERROR_3_2);
    tfData fundata;
    fundata.argc = 0;
    fundata.argv = sMalloc(sizeof(char));
    fundata.argv[0]='\0';

    NextToken(&token,file);
    if (token.state != sLpar)
        return SYNTAX_ERROR;
    if (idlist(&fundata)== SYNTAX_ERROR)
        return SYNTAX_ERROR;
    // no next token, token was alrea  dy loaded in idn() function
    if (token.state != sRpar)
        return SYNTAX_ERROR;
    fTableInsert(&FT,funkey,&fundata);
    NextToken(&token,file);
    if (token.state != sLbr)
        return SYNTAX_ERROR;
    Generate_instruction(&instrList,I_GOTO,NULL,NULL,NULL); //addr1 will be filled later
    tListItem* tmplast = instrList.last;
    Generate_instruction(&instrList,I_LABEL,NULL,NULL,NULL);
    tFTableItem* tmp = fTableSearch(&FT,funkey);
    if (!tmp)
        ClearAndExit(INTERNAL_ERROR);
    tmp->label = instrList.last;
    NextToken(&token, file);
    if (stat()== SYNTAX_ERROR && token.state!=sRbr)
    {
        if (token.state == sEof) //trap for eof inside if/while/declaration
            token.state = sErr; //in outer stat, eof is valid terminal
        return SYNTAX_ERROR;
    }
    // Generating implicit return
    int* type = sMalloc(sizeof(int));
    int* val = sMalloc(sizeof(int));
    (*val) = 0;
    (*type) = TYPE_NULL;
    Generate_instruction(&instrList,I_SETVAL,(void*)val,(void*)type,(void*)"#");
    Generate_instruction(&instrList,I_RETURN,(void*)"#",NULL,NULL);
    // label used for jump behind declaration
    Generate_instruction(&instrList,I_LABEL,NULL,NULL,NULL);
    tmplast->Instruction.addr1 = instrList.last; //addr1 filled
    NextToken(&token, file);
    return EXIT_SUCCESS;
}

int idlist(tfData* fundata)
{
    //Rule 6:
    NextToken(&token,file);
    if (token.state != sVar)
        return EXIT_SUCCESS;
    add_funarg(fundata,&token);
    //Rule 7:
    return idn(fundata);
}

int idn(tfData* fundata)
{
    //Rule 8:
    NextToken(&token,file);
    if (token.state != sComm)
        return EXIT_SUCCESS;
    //Rule 9:
    NextToken(&token,file);
    if (token.state != sVar)
        return SYNTAX_ERROR;
    add_funarg(fundata,&token);
    return idn(fundata);
}
int stat()
{
    int save_expr = 0;
    switch (token.state)
    {   //Rule: 10
        case sVar:
        {
            tKey destkey = (tKey)token.data;
            NextToken(&token,file);
            if (token.state != sAssign)
                return SYNTAX_ERROR;
            if (exprf(destkey,&save_expr) == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (save_expr)//expressions do not save internally their value into destkey, it might be redundant
                Generate_instruction(&instrList,I_SETVAL,(void*)"#",NULL,(void*)destkey);
            NextToken(&token,file);
            if (token.state == sScln)
            {
                NextToken(&token, file);
                return stat();
            }
            return SYNTAX_ERROR;
        }
        //Rule: 12
        case sKif:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (expr(NULL) == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            NextToken(&token, file);
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (token.state != sLbr)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_IFGOTO,(void*)"#",NULL,NULL);
            tListItem* tmp = instrList.last;
            Generate_instruction(&instrList,I_LABEL,NULL,NULL,NULL);
            tmp->Instruction.addr2 = instrList.last;
            NextToken(&token, file);
            if (stat() == SYNTAX_ERROR && token.state!=sRbr)
            {
                if (token.state == sEof) //trap for eof inside if/while/declaration
                token.state = sErr; //in outer stat, eof is valid terminal
                return SYNTAX_ERROR;
            }
            NextToken(&token, file);
            Generate_instruction(&instrList,I_GOTO,NULL,NULL,NULL);
            tListItem* tmpgoto = instrList.last;
            Generate_instruction(&instrList,I_LABEL,NULL,NULL,NULL);
            tmp->Instruction.addr3 = instrList.last;
            if (_else() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_LABEL,NULL,NULL,NULL);
            tmpgoto->Instruction.addr1 = instrList.last;
            NextToken(&token, file);
            return stat();
        }
        //Rule: 15
        case sKret:
        {
            NextToken(&token, file);
            //result in # variable
            if (expr(NULL)==SYNTAX_ERROR)
                return SYNTAX_ERROR;

            Generate_instruction(&instrList,I_RETURN,(void*)"#",NULL,NULL);
            NextToken(&token, file);
            if (token.state != sScln)
                return SYNTAX_ERROR;
            NextToken(&token, file);
            return stat();
        }
        //Rule: 16
        case sKwhile:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            tListItem* tmp  = instrList.last; //last list item before expression
            NextToken(&token,file);
            if (expr(NULL) == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            NextToken(&token, file);
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (token.state != sLbr)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_IFGOTO,(void*)"#",NULL,NULL);
            tListItem* iftmp = instrList.last;
            Generate_instruction(&instrList,I_LABEL,NULL,NULL,NULL);
            iftmp->Instruction.addr2=instrList.last;
            NextToken(&token, file);
            if (stat() == SYNTAX_ERROR && token.state!=sRbr)
            {
                if (token.state == sEof) //trap for eof inside if/while/declaration
                token.state = sErr; //in outer stat, eof is valid terminal
                return SYNTAX_ERROR;
            }
            NextToken(&token, file);
            Generate_instruction(&instrList,I_GOTO,(void*)tmp,NULL,NULL);
            Generate_instruction(&instrList,I_LABEL,NULL,NULL,NULL);
            iftmp->Instruction.addr3=instrList.last;
            NextToken(&token, file);
            return stat();
        }
        //Rule: 10
        default:
        {
            Ungettoken(token);
            return SYNTAX_ERROR;
        }
    }
}

int _else()
{
    NextToken(&token,file);
    //Rule: 13,14
    if (token.state != sKelse)
        return EXIT_SUCCESS;
    NextToken(&token,file);
    if (token.state != sLbr)
        return SYNTAX_ERROR;
    NextToken(&token, file);
    if (stat() == SYNTAX_ERROR && token.state!=sRbr)
    {
        if (token.state == sEof) //trap for eof inside if/while/declaration
        token.state = sErr; //in outer stat, eof is valid terminal
        return SYNTAX_ERROR;
    }
    NextToken(&token, file);
    return EXIT_SUCCESS;
}

int fun(tKey destkey)
{
    //Rule 17. First (= fid) of this non-terminal was checked out in exprf()
    if (token.state != sIdent)
        return SYNTAX_ERROR;
    // add void record into FT
    tKey funkey = (tKey)token.data;
    if (!fTableSearch(&FT,funkey))   //redefinition?
    {
        tfData fundata;
        fundata.argc = -1; //function not declared yet
        fundata.argv = sMalloc(sizeof(char));
        fundata.argv[0]='\0';
        fTableInsert(&FT,funkey,&fundata);
    }
    NextToken(&token,file);
    if (token.state != sLpar)
        return SYNTAX_ERROR;
    if (termlist(funkey) == SYNTAX_ERROR)
        return SYNTAX_ERROR;
    //token already taken , no next token
    if (token.state != sRpar)
        return SYNTAX_ERROR;
    Generate_instruction(&instrList,I_F_JUMP,(void*)funkey,NULL,(void*)destkey);
    return EXIT_SUCCESS;
}
int termlist(tKey funkey)
{
   NextToken(&token,file);
    //Rule: 18
   if (token.state==sRpar)
        return EXIT_SUCCESS;
    //Rule: 19
   if (term()==SYNTAX_ERROR)
        return SYNTAX_ERROR;
   if (token.state == sVar)
       Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,funkey));
   else
   {
       int* type = sMalloc(sizeof(int));
       (*type) = getType(&token);
       if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
       {
           int* value = sMalloc(sizeof(int));
           (*value) = (token.state == sCtrue) ? 1 : 0;
           Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
       }
       else
           Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
       Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,funkey));
   }
   return termn(funkey);
}

int termn(tKey funkey)
{
    NextToken(&token,file);
    //Rule 20,21
    if (token.state != sComm)
        return EXIT_SUCCESS;

    NextToken(&token,file);
    if (term()== SYNTAX_ERROR)
        return SYNTAX_ERROR;
    if (token.state == sVar)
        Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,funkey));
    else
    {
        int* type = sMalloc(sizeof(int));
        (*type) = getType(&token);
        if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
        {
            int* value = sMalloc(sizeof(int));
            (*value) = (token.state == sCtrue) ? 1 : 0;
            Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
        }
        else
            Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
        Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,funkey));
    }
    return termn(funkey);
}
int exprf(tKey destkey,int* save_expr)
{
    NextToken(&token,file);
    //Rule: 22,23,24
    if (fun(destkey) == SYNTAX_ERROR
     && inlinefun(destkey) == SYNTAX_ERROR
     && expr(save_expr)==SYNTAX_ERROR)
        return SYNTAX_ERROR;
    return EXIT_SUCCESS;
}
int expr(int* save_expr)
{
    // rule 25
    if (save_expr)
        *save_expr = 1;
    if (term()==SYNTAX_ERROR)
    {
        if (expression()==SYNTAX_ERROR)
            return SYNTAX_ERROR;
        // expression() final value is in #
    }
    else
    {   // insert term() value into accumulator #
        if (token.state==sVar)
           Generate_instruction(&instrList,I_SETVAL,token.data,NULL,(void*)"#");
        else
        {
            int* type = sMalloc(sizeof(int));
            switch (token.state)
            {
                case sInt:
               {
                  *type = TYPE_INT;
                  Generate_instruction(&instrList,I_SETVAL,token.data,(void*)type,(void*)"#");
                  break;
               }
               case sDbl:
               {
                  *type = TYPE_DBL;
                  Generate_instruction(&instrList,I_SETVAL,token.data,(void*)type,(void*)"#");
                  break;
               }
               case sStr:
               {
                  *type = TYPE_STR;
                  Generate_instruction(&instrList,I_SETVAL,token.data,(void*)type,(void*)"#");
                  break;
               }
               case sCtrue:
               {
                  *type = TYPE_BOOL;
                  int* tmp = sMalloc(sizeof(int));
                  *tmp=1;
                  Generate_instruction(&instrList,I_SETVAL,(void*)tmp,(void*)type,(void*)"#");
                  break;
               }
               case sCfalse:
               {
                  *type = TYPE_BOOL;
                  int* tmp = sMalloc(sizeof(int));
                  *tmp=0;
                  Generate_instruction(&instrList,I_SETVAL,(void*)tmp,(void*)type,(void*)"#");
                  break;
               }
               case sCnull:
               {
                  *type = TYPE_NULL;
                  int* tmp = sMalloc(sizeof(int));
                  *tmp=0;
                  Generate_instruction(&instrList,I_SETVAL,(void*)tmp,(void*)type,(void*)"#");
                  break;
               }
               default:
                ClearAndExit(INTERNAL_ERROR);
                return INTERNAL_ERROR;
            }
        }
    }
    return EXIT_SUCCESS;
}
int litexp()
{
    switch (token.state)
    {       //TODO: generovanie zoznamu ?
        //Rule: 27,28,29
        case sInt:
        case sDbl:
        case sCtrue:
        case sCfalse:
            return EXIT_SUCCESS;
        //Rule: 30
        default:
            return strform();
    }
}
int term()
{
    //Rule: 31,32
   if (token.state !=sCnull &&litexp()==SYNTAX_ERROR)
        return SYNTAX_ERROR;
   tToken tmp = token;
   NextToken(&token,file);
   if (token.state != sScln && token.state != sRpar && token.state != sComm)
   {
       Ungettoken(token);
       token = tmp;
       return SYNTAX_ERROR;
   }
   Ungettoken(token);
   token = tmp;
   return EXIT_SUCCESS;
}
int strform()
{
    //Rule: 35,36
    if (token.state == sStr || token.state == sVar)
        return EXIT_SUCCESS;

    return SYNTAX_ERROR;
}
int strflist(int* write_count, tKey destkey)
{
    NextToken(&token,file);
    //Rule: 37
    if (token.state == sRpar)
        return EXIT_SUCCESS;
    //Rule: 38
    else if (token.state == sComm)
    {
        NextToken(&token,file);
        if (term()!=SYNTAX_ERROR)
        {
            (*write_count)++;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_WRITE, token.data, (void*)write_count, (void*)destkey);
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_WRITE, (void*)"#", (void*)write_count, (void*)destkey);
            }
            return strflist(write_count, destkey);
        }
        else
            return SYNTAX_ERROR;
    }
    return SYNTAX_ERROR;
}
int inlinefun(tKey destkey)
{
    switch (token.state)
    {
        //Rule: 39
        case sINboolval:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            if (termlist("boolval") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_BOOLVAL,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 40
        case sINdoubleval:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            if (termlist("doubleval") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_DOUBLEVAL,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 41
        case sINintval:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            if (termlist("intval") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_INTVAL,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 42
        case sINstrval:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            if (termlist("strval") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_STRVAL,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 43
        case sINgetstring:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            if (termlist("get_string") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_READ,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 44
        case sINstrlen:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (term() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,"strlen"));
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,"strlen"));
            }
            if (termn("strlen") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_STRLEN,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 47
        case sINsortstring:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (term() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,"sort_string"));
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,"sort_string"));
            }

            if (termn("sort_string") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_SORTSTR,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 45
        case sINgetsubstring:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (term() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,"get_substring"));
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,"get_substring"));
            }
            NextToken(&token,file);
            if (token.state != sComm)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (term() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,"get_substring"));
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,"get_substring"));
            }
            NextToken(&token,file);
            if (token.state != sComm)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (term() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,"get_substring"));
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,"get_substring"));
            }
            if (termn("get_substring") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_SUBSTR,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 46
        case sINfindstring:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (term() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,"find_string"));
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,"find_string"));
            }

            NextToken(&token,file);
            if (token.state != sComm)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (term() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_PUSH,token.data,NULL,(void*)fTableSearch(&FT,"find_string"));
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_PUSH,(void*)"#",NULL,(void*)fTableSearch(&FT,"find_string"));
            }

            if (termn("find_string") == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            if (token.state != sRpar)
                return SYNTAX_ERROR;
            Generate_instruction(&instrList,I_FINDSTR,NULL,NULL,(void*)destkey);
            return EXIT_SUCCESS;
        }
        //Rule: 48
        case sINputstring:
        {
            NextToken(&token,file);
            if (token.state != sLpar)
                return SYNTAX_ERROR;
            NextToken(&token,file);
            if (term() == SYNTAX_ERROR)
                return SYNTAX_ERROR;
            int* write_count=sMalloc(sizeof(int));
            *write_count = 1;
            if (token.state == sVar)
                Generate_instruction(&instrList,I_WRITE, token.data, (void*)write_count, (void*)destkey);
            else
            {
                int* type = sMalloc(sizeof(int));
                (*type) = getType(&token);
                if (token.state == sCtrue || token.state == sCfalse || token.state == sCnull)
                {
                    int* value = sMalloc(sizeof(int));
                    (*value) = (token.state == sCtrue) ? 1 : 0;
                    Generate_instruction(&instrList,I_SETVAL, (void*)value,(void*)type,(void*)"#");
                }
                else
                    Generate_instruction(&instrList,I_SETVAL, token.data,(void*)type,(void*)"#");
                Generate_instruction(&instrList,I_WRITE, (void*)"#", (void*)write_count, (void*)destkey);

            }
            return strflist(write_count, destkey);
        }
        default: return SYNTAX_ERROR;
    }
}
int expression()
{
    // PRECEDENCE ANALYSIS
    tPstack stack;
    int counter=0;
    keygenc=0;
    int already_taken=0; // we dont want to covert same token again
    tPstack tmpstack;
    Tstackinit(&t_stack);
    Pstackinit(&stack);
    Pstackinit(&tmpstack);
    Ppush(&stack,ENDMARKER); // $ is on stack top
    tIndex a = ERROR;
    tIndex b = ERROR;
    do{
        if (!already_taken)
            a = Convert_token(token,&counter);

        if (a == ERROR)
            return SYNTAX_ERROR;
        b = stack.top->element;
        while (b>ENDMARKER) //moving non terminals onto tmpstack
        {                   //accesing PT with value greater then ENDMARKER
                            //would result into segfault
            Ppush(&tmpstack,Ptoppop(&stack));
            b = stack.top->element;
        }
        if (counter == -1)
            a = ENDMARKER;
        if ( a>ENDMARKER || b>ENDMARKER)    // handling segfault threat
        {
            while (!Tstackempty(&t_stack))
                Tpop(&t_stack);
            while (!Pstackempty(&stack))
                Ppop(&stack);
            while (!Pstackempty(&tmpstack))
                Ppop(&tmpstack);
            return SYNTAX_ERROR;
        }
        switch (Ptable[b][a])
        {
            case E:
            {
                already_taken = 0;
                while (!Pstackempty(&tmpstack)) // non terminals are supposed to be under pushed symbol
                    Ppush(&stack,Ptoppop(&tmpstack));
                Ppush(&stack,a);
                NextToken(&token,file);
                break;
            }
            case L:
            {
                already_taken = 0;
                tIndex btmp = stack.top->element;
                while(btmp>ENDMARKER) // EXTRACT NON TERMINALS FROM MAIN STACK
                {
                    Ppush(&tmpstack,Ptoppop(&stack));
                    btmp=stack.top->element;
                }
                Ppush(&stack,LESS); // pushing less dummy
                while (!Pstackempty(&tmpstack))
                {
                    Ppush(&stack,Ptoppop(&tmpstack));
                }
                Ppush(&stack,a);
                NextToken(&token,file);
                break;
            }
            case G:
            {
                already_taken = 1; //we wont take token this round
                int result = reduction (&stack,&tmpstack);
                if (result == SYNTAX_ERROR)
                {
                    while (!Tstackempty(&t_stack))
                        Tpop(&t_stack);
                    while (!Pstackempty(&stack))
                        Ppop(&stack);
                    while (!Pstackempty(&tmpstack))
                        Ppop(&tmpstack);
                    return SYNTAX_ERROR;
                }
                break;
            }
            default: //Unknown operator (error)
            {
                while (!Tstackempty(&t_stack))
                    Tpop(&t_stack);
                while (!Pstackempty(&stack))
                        Ppop(&stack);
                while (!Pstackempty(&tmpstack))
                        Ppop(&tmpstack);
                return SYNTAX_ERROR;
            }
        }
        //NONTERM & LESS symbols are moved onto tmpstack
        while(stack.top->element > ENDMARKER)
            Ppush(&tmpstack,Ptoppop(&stack));

        b = stack.top->element; // reloading b ..stack top $ means finish
        while (!Pstackempty(&tmpstack))
            Ppush(&stack,Ptoppop(&tmpstack));
   }while(!(a==ENDMARKER && b == ENDMARKER));
    while (!Tstackempty(&t_stack))
        Tpop(&t_stack);
    while (!Pstackempty(&stack)) //free stacks
       Ppop(&stack);
    while (!Pstackempty(&tmpstack))
       Ppop(&tmpstack);
   Ungettoken(token);
   return EXIT_SUCCESS;
}

int reduction (tPstack* stack, tPstack* tmpstack)
{
    /** \brief PRECEDENCE ANALYSIS RULES
     *       1:E->E+E
     *       2:E->E-E
     *       3:E->E*E
     *       4:E->E/E
     *       5:E->E.E
     *       6:E->E>E
     *       7:E->E<E
     *       8:E->E>=E
     *       9:E->E<=E
     *      10:E->E===E
     *      11:E->E!==E
     *      12:E->(E)
     *      13:E->i
     */
    int correct = 0;
    char _op;
    tIndex btmp = stack->top->element;
    while (btmp!=LESS && !Pstackempty(stack))//finding topest LESS symbol
    {
        Ppush(tmpstack,Ptoppop(stack));
        btmp=stack->top->element;
    }
    //RULES
    if (btmp != LESS)
        return SYNTAX_ERROR;
    // LESS found
    tIndex tmp2 = Ptoppop(tmpstack);//element above LESS index
    if ( tmp2 == ID ) // Rule E->id
    {
        //top of token stack has info about an ID
        Ppop(stack); // < removed from the stack
        Ppush(stack, NONTERM); // replacement for E non-terminal
        return EXIT_SUCCESS;
    }
    else if (tmp2 == LPAR)
    {
        tIndex tmp3;
        if (!Pstackempty(tmpstack))
            tmp3=Ptoppop(tmpstack);
        else
            return SYNTAX_ERROR;

        //non-term E must be behind parenthesis
        if (tmp3 == NONTERM)
        {
            tIndex tmp4;
            if (!Pstackempty(tmpstack)) // right paranthesis expected
                tmp4 = Ptoppop(tmpstack);
            else
                return SYNTAX_ERROR;

            if (tmp4 == RPAR && Pstackempty(tmpstack)) // pravidlo E->(E)
            {
                Ppop(stack);// < removed from the stack
                Ppush(stack, NONTERM);// replacement for E non-terminal
                return EXIT_SUCCESS;
            }
            else
                return SYNTAX_ERROR; // redundant characters
        }
        else
            return SYNTAX_ERROR; // no non-term E found
    }
    else if (tmp2 == NONTERM)
    {
        tIndex tmp3;
        if (!Pstackempty(tmpstack)) // operator expected behind E non-term
                tmp3 = Ptoppop(tmpstack);
        else
            return SYNTAX_ERROR;
        switch (tmp3)
        {
            case PLUS:
                _op=I_ADD;
                correct=1;
                break;
            case MINUS:
                _op=I_SUB;
                correct=1;
                break;
            case MUL:
                _op=I_MUL;
                correct=1;
                break;
            case DIV:
                _op=I_DIV;
                correct=1;
                break;
            case CONCAT:
                _op=I_CONCAT;
                correct=1;
                break;
            case EQUAL:
                _op=I_EQUAL;
                correct=1;
                break;
            case NOTEQUAL:
                _op=I_NOTEQUAL;
                correct=1;
                break;
            case GREATER:
                _op=I_MORE;
                correct=1;
                break;
            case LESSER:
                _op=I_LESS;
                correct=1;
                break;
            case GREATEREQ:
                _op=I_MOREEQ;
                correct=1;
                break;
            case LESSEREQ:
                _op=I_LESSEQ;
                correct=1;
                break;
            default: return SYNTAX_ERROR;
        }
        tIndex tmp4 = ERROR;
        if (correct)
        {
            if (!Pstackempty(tmpstack))
               tmp4 = Ptoppop(tmpstack);
        }
        else return SYNTAX_ERROR;
        if (tmp4 == NONTERM && Pstackempty(tmpstack))
        {
            if (!exprc) // first time top token stack item is in #
                Tpop(&t_stack);
            exprc++;
            tToken operand2 = Ttoppop(&t_stack);
            Generate_instruction(&instrList,_op,operand2.data,(void*)"#",(void*)"#");

            //expression value in greater expression is masked as variable
            tKey mask = keygen(&keygenc);
            Generate_instruction(&instrList,I_SETVAL,(void*)"#",NULL,(void*)mask);
            exprc=0; // new expression excpected
            tToken tmptoken;
            tmptoken.state=sVar;
            tmptoken.data=(void*)mask;
            Tpush(&t_stack,tmptoken);

            Ppop(stack); // removing < from stack
            Ppush(stack,NONTERM);
            return EXIT_SUCCESS;
        }
        else
            return SYNTAX_ERROR;
    }
    else
        return SYNTAX_ERROR;
    while (Pstackempty(stack))
        Ppop(stack);
    while (Pstackempty(tmpstack))
        Ppop(tmpstack);
    return EXIT_SUCCESS;
}

tIndex Convert_token(tToken token,int* counter)
{
  switch (token.state)
  {
    //Operator
    case sAdd: return PLUS;
    case sSub: return MINUS;
    case sMul: return MUL;
    case sDiv: return DIV;
    case sCat: return CONCAT;
    case sEq:  return EQUAL;
    case sNoteq: return NOTEQUAL;
    case sMore: return GREATER;
    case sLess: return LESSER;
    case sMoreq: return GREATEREQ;
    case sLesseq: return LESSEREQ;
    case sLpar:
    {
        (*counter)++;
        return LPAR;
    }
    case sRpar:
    {
        (*counter)--;
        return RPAR;
    }
    //ID
    case sVar:
    {
        Tpush(&t_stack,token);
        if (!exprc)
             Generate_instruction(&instrList,I_SETVAL,token.data,NULL,(void*)"#");

        return ID;
    }   //other ID cases are masked as variables
    case sInt:
    {
        tKey mask = keygen(&keygenc);
        int* type = sMalloc(sizeof(int));
        *type = TYPE_INT;
        Generate_instruction(&instrList,I_SETVAL,token.data,(void*)type,(void*)mask);
        //constant value is placed in mask variable
        Tpush(&t_stack,token);
        //redefine token.data as it is used in variable
        t_stack.top->token.data=(void*)mask;
        if (!exprc)
             Generate_instruction(&instrList,I_SETVAL,t_stack.top->token.data,NULL,(void*)"#");

        return ID;
    }
    case sDbl:
    {
        tKey mask = keygen(&keygenc);
        int* type = sMalloc(sizeof(int));
        *type = TYPE_DBL;
        Generate_instruction(&instrList,I_SETVAL,token.data,(void*)type,(void*)mask);
        //constant value is placed in mask variable
        Tpush(&t_stack,token);
        //mask token.data in stack as variable name
        t_stack.top->token.data=(void*)mask;
        if (!exprc)
             Generate_instruction(&instrList,I_SETVAL,t_stack.top->token.data,NULL,(void*)"#");

        return ID;
    }
    case sStr:
    {
        tKey mask = keygen(&keygenc);
        int* type = sMalloc(sizeof(int));
        *type = TYPE_STR;
        Generate_instruction(&instrList,I_SETVAL,token.data,(void*)type,(void*)mask);
        //constant value is placed in mask variable
        Tpush(&t_stack,token);
        //redefine token.data as it is used in variable
        t_stack.top->token.data=(void*)mask;
        if (!exprc)
             Generate_instruction(&instrList,I_SETVAL,t_stack.top->token.data,NULL,(void*)"#");

        return ID;
    }
    case sCtrue:
    {
        tKey mask = keygen(&keygenc);
        int* type = sMalloc(sizeof(int));
        *type = TYPE_BOOL;
        int* tmp = sMalloc(sizeof(int));
        *tmp = 1;
        Generate_instruction(&instrList,I_SETVAL,(void*)tmp,(void*)type,(void*)mask);
        //constant value is placed in mask variable
        Tpush(&t_stack,token);
        //redefine token.data as it is used in variable
        t_stack.top->token.data=(void*)mask;
        if (!exprc)
             Generate_instruction(&instrList,I_SETVAL,t_stack.top->token.data,NULL,(void*)"#");

        return ID;
    }
    case sCfalse:
    {
        tKey mask = keygen(&keygenc);
        int* type = sMalloc(sizeof(int));
        *type = TYPE_BOOL;
        int* tmp = sMalloc(sizeof(int));
        *tmp = 0;
        Generate_instruction(&instrList,I_SETVAL,(void*)tmp,(void*)type,(void*)mask);
        //constant value is placed in mask variable
        Tpush(&t_stack,token);
        //redefine token.data as it is used in variable
        t_stack.top->token.data=(void*)mask;
        if (!exprc)
             Generate_instruction(&instrList,I_SETVAL,t_stack.top->token.data,NULL,(void*)"#");

        return ID;
    }
    case sCnull:
    {
        tKey mask = keygen(&keygenc);
        int* type = sMalloc(sizeof(int));
        *type = TYPE_NULL;
        int* tmp = sMalloc(sizeof(int));
        *tmp = 0;
        Generate_instruction(&instrList,I_SETVAL,(void*)tmp,(void*)type,(void*)mask);
        //constant value is placed in mask variable
        Tpush(&t_stack,token);
        //redefine token.data as it is used in variable
        t_stack.top->token.data=(void*)mask;
        if (!exprc)
             Generate_instruction(&instrList,I_SETVAL,t_stack.top->token.data,NULL,(void*)"#");

        return ID;
    }
    //$
    case sScln: return ENDMARKER;
    default: return ERROR;
  }
}
