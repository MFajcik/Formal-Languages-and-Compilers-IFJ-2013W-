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

#include "ial.h"

/*
 * Vrati hodnotu termu term konvertovanou na pravdivostni hodnotu
 */
bool boolval(tData *term)
{
    switch(term->varType) // int/long/double/char/string
    {
        case TYPE_INT:
            return (*((int*)term->varValue) == 0)?false:true;
        case TYPE_DBL:
            return (*((double*)term->varValue)== 0.0)?false:true;
        case TYPE_STR:
            return (strcmp((char*)term->varValue,""))?true:false;
        case TYPE_BOOL:
            return *(bool*)term->varValue;
        case TYPE_NULL:
        default: return false;
    }
}

int intval(tData *term)
{
    switch(term->varType)
    {
        case TYPE_INT:
            return *((int*)term->varValue);
        case TYPE_DBL:
            return (int)(*((double*)term->varValue));
        case TYPE_STR:
        {
            char znak;
            int i = 0;
            char *source = (char*)term->varValue;
            int result = 0;
            int test = 0;
            while((znak = source[i]) != '\0')
            {
                i++;
                if (test == 0 && (!isprint(znak) || znak == ' ')) continue;
                if (znak < '0' || znak > '9') break;
                if (test == 0) test = 1;

                result *= 10;
                result += (znak-'0');
            }
            return result;
        } break;
        case TYPE_BOOL:
            return (*((bool*)term->varValue)) ? 1 : 0 ;
        case TYPE_NULL:
        default: return 0;
    }
}

/*
 * Prevede hodnotu na desetinne cislo
 */
double doubleval(tData *term)
{
    double result = 0.0;
    switch(term->varType)
    {
        case TYPE_INT:
            result = (double)(*(int*)term->varValue);
            break;
        case TYPE_DBL:
            result = *((double*)term->varValue);
            break;
        case TYPE_STR:
            {
                char znak;
                int i = 0;
                char *source = (char*)term->varValue;
                int test = 0;
                double add ;
                int cntAfterDot = 0;
                bool afterDot = false;
                bool znakE = false;
                bool negExponent = false;
                int exponent = 0;
                while((znak = source[i]) != '\0')
                {
                    i++;
                    if (test == 0 && (!isprint(znak) || znak == ' ')) continue;
                    /*bool podminka = (((znak < '0' || znak > '9') && znak != 'e' && znak =='.' && afterDot) || (znak == 'e' && znakE));
                    bool podminka1 = ((znak < '0' || znak > '9') && znak != 'e');
                    bool podminka2 = ((znak != '.' && afterDot) || (znak == '.' && afterDot));
                    bool podminka3 = (znak != 'e' && !znakE);
                    if (podminka1 && podminka2 && podminka3) break;*/
                    //if(podminka) break;
                    if (((znak < '0' || znak > '9') && znak != 'e') && ((znak != '.' && afterDot) || (znak == '.' && afterDot)) && (znak != 'e' && !znakE)) break;
                    if (test == 0) test = 1;

                    if(znak == '.')
                    {
                        afterDot = true;
                        continue;
                    }

                    if(znak == 'e')
                    {
                        if (znakE) break;
                        if ((source[i] < '0' || source[i] > '9') && source[i] != '-')
                            ClearAndExit(SEM_ERROR_11);

                        znakE = true;
                        continue;
                    }

                    if (znak == '-' && znakE)
                    {
                        negExponent = true;
                        continue;
                    }

                    if (!znakE) {
                        add = znak-'0';
                        if (afterDot) {
                            cntAfterDot++;
                            for(int j = 0; j < cntAfterDot;j++)
                                add /= 10;
                        } else result *= 10;
                        result += add;
                    } else {
                        add = znak-'0';
                        exponent *= 10;
                        exponent += add;
                    }
                }
                if (negExponent)
                    exponent *= -1;

                if(exponent > 0)
                    for(;exponent > 0; result *= 10, --exponent);
                else if(exponent < 0)
                    for(;exponent < 0; result /= 10, ++exponent);

            } break;
        case TYPE_BOOL:
            result = (*((bool*)term->varValue) == true) ? 1.0 : 0.0;
            break;
        case TYPE_NULL:
            result = 0.0;
            break;
        default: ClearAndExit(SEM_ERROR_11);
    }

    return result;
}

/*
 * Vrati hodnotu konvertovanou na retezec.
 * Konverzi celeho ci desetineho cisla resi pomoci standartnich formatovacich znaku
 * "%d", resp. "%g"
 */
char* strval(tData *term) { // Je toto spravna navratova hodnota?
    switch(term->varType) {
        case TYPE_INT:
        {
            char buffer[50];
            int size = sprintf(buffer, "%d", *(int*)term->varValue);
            char* string = sMalloc(size+1);
            memcpy(string, buffer, size+1);
            return string;
        }
        case TYPE_DBL:
        {
            char buffer[50];
            int size = sprintf(buffer, "%g", *(double*)term->varValue);
            char* string = sMalloc(size+1);
            memcpy(string, buffer, size+1);
            return string;
        }
        case TYPE_STR:
            return (char*)term->varValue;
        case TYPE_BOOL:
            return *((bool*)term->varValue) ? "1" : "" ;
        case TYPE_NULL:
        default: return "";
    }
}

/*
 * Nacte 1 radek z stdin (po znak \n) nebo v pripade EOF praci ""
 */
char* get_string()
{
    int index = 0;
    char* text;
    char znak;
    while((znak=getchar()) != '\n' && znak != '\r' && znak != EOF)
        add_to_str(&znak, &text, &index);
    add_to_str(NULL, &text, &index);
    return text;
}

/*
 * Vraci delku stringu
 */
int strleng(tData* term)
{
    return strlen(strval(term));
}

/*
 * Vrati substring ze stringu mezi indexy int1 a int2
 */
char* get_substring(char* string, unsigned int integer1 , unsigned int integer2)
{
    /*
    integer1 == index 1. kopirovaneho znaku
    integer2 == index za poslednim kopirovany znakem
    */
    unsigned int len = strlen(string);
    if (integer1 > integer2 || integer1 >= len || integer2 > len)
    {
        fprintf(stderr, err[SEM_ERROR_13]);
        exit(SEM_ERROR_13);
        return NULL;  //irrelevant
    }

    int length = integer2 - integer1;
    char* substring = sMalloc(sizeof(char)*(length+1));
    strncpy(substring, string+integer1, length);
    substring[length]='\0';

    return substring;
}

/*
 * Vrati index na kterem zacina string2 ve stringu1. Vyuziva Knuth–Morris–Pratt algoritmus
 * V pripade nenalezeni vraci -1
 */
int find_string(char* string, char* string2)
{
    char* text = string;
    char* pattern = string2;

    /*  next tabulka
        n == delka vzorku
        m == delka textu
    */
    int n = strlen(pattern);
    if (n == 0)
        return 0;
    int m = strlen(text);
    int j = -1;
    int i = 1;
    int next[n+1];
    next[0] = j;

    for(i = 1; i < n; i++)
    {
        while((j > -1) && (pattern[j+1] != pattern[i]))
            j = next[j];
        if (pattern[i] == pattern[j+1])
            j++;
        next[i] = j;
    }

    /* kmp */
    i = 0;
    j = -1;

    for(i = 0; i < m; i++)
    {
        while((j > -1) && (pattern[j+1] != text[i]))
            j = next[j];
        if (text[i] == pattern[j+1])
            j++;
        if (j == n-1)
            return i-j;
    }
    return -1;
}

/*
 * Seradi string podle ordinalnich hodnot.
 * Vraci serazeny string.
 */
char* sort_string(tData* term)
{
    int size = strleng(term);
    char* text = sMalloc(size);
    text = strval(term);
    int gap = size/2;

    while(gap > 0)
    {
        for(int i = 0; i < size-gap; i++)
        {
            int j = i+gap;
            int tmp = text[j];
            while(j >= gap && tmp < text[j-gap])
            {
                text[j] = text[j-gap];
                j -= gap;
            }
            text[j] = tmp;
        }
        if (gap == 2)
          gap = 1;
        else
          gap /= 2;
    }
    return text;
}


// hash function
int hashCode (tKey key)
{
    int retval = 1;
    int keylen = strlen(key);
    for (int i = 0; i < keylen; i++)
        retval += key[i];
    return ( retval % HTSIZE );
}

// function initializes table of symbols
void tableInit(tST* sourceST)
{
    if (!sourceST)
        return;

    memset(*sourceST, 0, sizeof(tTableItem*) * HTSIZE);
    tableInsert(sourceST, "#", NULL); // inserts custom symbol used for expression oprerand #1
    tableInsert(sourceST, "##", NULL); // inserts custom symbol used for expression oprerand #2
}

// function for searching in table
// if key has been found function returns table item else null
tTableItem* tableSearch(tST* sourceST, tKey key)
{
    if (!sourceST || !(*sourceST))
        return NULL;

    tTableItem* pom = (*sourceST)[hashCode(key)];
    while (pom)
    {
        if (!strcmp(pom->key, key))
            return pom;

        pom = pom->nextItem;
    }

    return NULL;
}

// function inserts new item into table or rewrites his data if already exists
// data can be NULL
int tableInsert(tST* sourceST, tKey key, tData* data)
{
    if (!sourceST)
        return EXIT_FAILURE;

    tTableItem* pom = tableSearch(sourceST, key);
    if (pom)
    {
        if (data)
        {
            pom->data->varType = data->varType;
            pom->data->varValue = data->varValue;
        }
    }
    else
    {
        pom = (*sourceST)[hashCode(key)];
        tTableItem* item = sMalloc(sizeof(tTableItem));
        item->key = sMalloc((strlen(key)+1) * sizeof(char));
        item->data = sMalloc(sizeof(tData));
        memcpy(item->key, key, strlen(key)+1);
        if (data)
        {
            item->data->varType = data->varType;
            item->data->varValue = data->varValue;
        }
        item->nextItem = pom;
        (*sourceST)[hashCode(key)] = item;
    }

    return EXIT_SUCCESS;
}

// function dealocates table
void tableFree(tST* sourceST)
{
    if (!sourceST)
        return;

    // table is freed at the end of program
    // just reset table elements
    memset(*sourceST, 0, sizeof(tTableItem*) * HTSIZE);
}

// Macro function for reinicialize Symbol Table
void tableReInit(tST* sourceST)
{
    if (!sourceST)
        return;

    tableFree(sourceST);
    tableInit(sourceST);
}

// function initializes table of symbols
void fTableInit(tFT* T)
{
    if (!T)
        return;

    memset(*T, 0, sizeof(tFTableItem*) * HTSIZE );
    tfData data;
    data.argv = sMalloc(6*sizeof(char));
    data.argc = 0;
    data.argv = "";
    fTableInsert(T, "get_string",&data);
    data.argc = 1;
    data.argv = "a";
    fTableInsert(T, "boolval", &data);
    fTableInsert(T, "doubleval", &data);
    fTableInsert(T, "intval", &data);
    fTableInsert(T, "strval", &data);
    fTableInsert(T, "put_string", &data);
    fTableInsert(T, "strlen", &data);
    fTableInsert(T, "sort_string", &data);
    data.argc = 2;
    data.argv = "a#b";
    fTableInsert(T, "find_string", &data);
    data.argc = 3;
    data.argv = "a#b#c";
    fTableInsert(T, "get_substring", &data);
}

// function inserts new item into table or rewrites his data if already exists
// data can be NULL
int fTableInsert(tFT* T, tKey key, tfData* data)
{
    if (!T)
        return EXIT_FAILURE;

    tFTableItem* tmp= fTableSearch(T, key);
    if (tmp)
    {
        if (data)
        {
            tmp->data->argc = data->argc;
            strncpy(tmp->data->argv, data->argv, strlen(data->argv)+1);
        }
    }
    else
    {
        tmp = (*T)[hashCode(key)];
        tFTableItem* newItem = sMalloc(sizeof(tFTableItem));
        int keysize = strlen(key)+1;
        newItem->key = sMalloc(keysize * sizeof(char));
        newItem->data = sMalloc(sizeof(tfData));
        memcpy(newItem->key, key, keysize);
        if (data)
        {
            newItem->data->argc = data->argc;
            newItem->data->argv = sMalloc((strlen(data->argv)+1) * sizeof(char));
            strncpy(newItem->data->argv, data->argv, strlen(data->argv)+1);
        }
        newItem->label = NULL;
        newItem->nextItem = tmp;
        (*T)[hashCode(key)] = newItem;
    }
    return EXIT_SUCCESS;
}

// function for searching in table
// if key has been found function returns table item else null
tFTableItem* fTableSearch(tFT* T, tKey key)
{
    if (!T || !(*T))
        return NULL;

    tFTableItem* tmp = (*T)[hashCode(key)];
    while (tmp)
    {
        if (!strcmp(tmp->key, key))
            return tmp;

        tmp = tmp->nextItem;
    }

    return NULL;
}

// function dealocates table
void fTableFree(tFT* T)
{
    if (!T)
        return;

    // table is freed at the end of program
    // just reset table elements
    memset(*T, 0, sizeof(tFTableItem*) * HTSIZE);
}

// function searches for function in table and tries to find argument given by argn
tKey getArgKey(tFT* T, tKey key, int argn)
{
    if (!T)
        return NULL;

    tFTableItem* tmp = fTableSearch(T, key);
    if (tmp)
        return getArgKeyFromData(tmp->data, argn);
    else
        return NULL;
}
tKey getArgKeyFromData(tfData* fundata, int argn)
{
    if (!fundata)
        return NULL;

    int i, charnumber;
    char* a;
    int tagnumber = 0;
    i = charnumber = 0;

    while(fundata->argv[i] != '\0')
    {
        if (fundata->argv[i] == '#')
               tagnumber++;
        else
        {
            if (tagnumber == argn)
                add_to_str(&(fundata->argv[i]), &a, &charnumber);
        }
        i++;
    }
    add_to_str(NULL, &a, &charnumber);
    return (tKey)a;
}

/** \brief adds argument name and increments argument number in tfData structure
 *
 * \param fundata tfData* data to be modified
 * \param token tToken* token ,where data are taken from
 * \return void
 *
 */
void add_funarg(tfData* fundata, tToken* token)
{

    char* tmp = (char*)token->data;

    for (int i = 0; i < fundata->argc; i++)
    {
        tKey arg = getArgKeyFromData(fundata, i);

        if (strcmp(tmp, arg) == 0)
            ClearAndExit(SEM_ERROR_13);
    }
    fundata->argc++;
    char* dummy = sRealloc(fundata->argv,(strlen(tmp)+strlen(fundata->argv)+2)*sizeof(char));
    if (!dummy)
        return;
    fundata->argv = dummy;
    strcat(fundata->argv, tmp);
    strcat(fundata->argv, "#");
}

