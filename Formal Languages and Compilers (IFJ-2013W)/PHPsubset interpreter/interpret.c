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

#include "interpret.h"

int inter(tST* mainST) {
    tInstr *I; // Data aktualni instrukce
    int counter = 0; // Counter poctu parametru
    tST tmpST; // Pomocna tabulka symbolu
    tableInit(&tmpST); // Jeji inicializace
    bool returnState = 0; // Priznak pro ukonceni interpetu
    while (returnState == 0) {
        I = listGetData(&instrList);
        if (!I) //list is empty
            return EXIT_SUCCESS; //File with sStart and sEof token
        switch (I->instType) {
            case I_READ:
            {

                if ( I->addr3 == NULL)
                {
                    break;
                }

                tData* data=sMalloc(sizeof(tData));
                data->varType = TYPE_STR;
                data->varValue=(void*)get_string();
                tableInsert(mainST, (tKey)I->addr3, data);
                }
                break;

            case I_ADD:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    //------------------------------INT + INT = BUDE TO INT-------------------------------
                    if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        int *soucet = sMalloc(sizeof(int));
                        *soucet = (*data1)+(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_INT;
                        vysledek->varValue = (void*)soucet;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------INT + DBL = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_DBL)) {
                        int* data1 = (int*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        double *soucet = sMalloc(sizeof(double));
                        *soucet = (*data1)+(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)soucet;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL + INT = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_INT)) {
                        double* data1 = (double*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        double *soucet = sMalloc(sizeof(double));
                        *soucet = (*data1)+(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)soucet;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL + DBL = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        double *soucet = sMalloc(sizeof(double));
                        *soucet = (*data1)+(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)soucet;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else  ClearAndExit(SEM_ERROR_12);
                }
                break;
            case I_SUB:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    //------------------------------INT - INT = BUDE TO INT-------------------------------
                    if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        int *rozdil = sMalloc(sizeof(int));
                        *rozdil = (*data1)-(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_INT;
                        vysledek->varValue = (void*)rozdil;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                     //------------------------------INT - DBL = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_DBL)) {
                        int* data1 = (int*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        double *rozdil = sMalloc(sizeof(double));
                        *rozdil = (*data1)-(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)rozdil;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL - INT = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_INT)) {
                        double* data1 = (double*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        double *rozdil = sMalloc(sizeof(double));
                        *rozdil = (*data1)-(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)rozdil;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL - DBL = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        double *rozdil = sMalloc(sizeof(double));
                        *rozdil = (*data1)-(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)rozdil;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else ClearAndExit(SEM_ERROR_12);
                }
                break;
            case I_MUL:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    //------------------------------INT * INT = BUDE TO INT-------------------------------
                    if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        int *soucin = sMalloc(sizeof(int));
                        *soucin = (*data1)*(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_INT;
                        vysledek->varValue = (void*)soucin;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                     //------------------------------INT * DBL = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_DBL)) {
                        int* data1 = (int*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        double *soucin = sMalloc(sizeof(double));
                        *soucin = (*data1)*(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)soucin;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL * INT = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_INT)) {
                        double* data1 = (double*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        double *soucin = sMalloc(sizeof(double));
                        *soucin = (*data1)*(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)soucin;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL * DBL = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        double *soucin = sMalloc(sizeof(double));
                        *soucin = (*data1)*(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)soucin;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else ClearAndExit(SEM_ERROR_12);
                }
                 break;
            case I_DIV:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    //------------------------------INT / INT = BUDE TO DBL-------------------------------
                    if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        double *podil = sMalloc(sizeof(double));
                        if (!data2 || (*data2) == 0) ClearAndExit(SEM_ERROR_10);
                        *podil = (*data1)/(double)(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)podil;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                     //------------------------------INT / DBL = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_DBL)) {
                        int* data1 = (int*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        double *podil = sMalloc(sizeof(double));
                        if (!data2 || (*data2) == 0.0) ClearAndExit(SEM_ERROR_10);
                        *podil = (*data1)/(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)podil;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL / INT = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_INT)) {
                        double* data1 = (double*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        double *podil = sMalloc(sizeof(double));
                        if (!data2 || (*data2) == 0) ClearAndExit(SEM_ERROR_10);
                        *podil = (*data1)/(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)podil;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL / DBL = BUDE TO DBL-------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        double *podil = sMalloc(sizeof(double));
                        if (!data2 || (*data2) == 0.0) ClearAndExit(SEM_ERROR_10);
                        *podil = (*data1)/(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_DBL;
                        vysledek->varValue = (void*)podil;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else ClearAndExit(SEM_ERROR_12);
                }
                break;
            case I_CONCAT:
                {

                    if (I->addr1 == NULL || I->addr2 == NULL || I->addr3 == NULL)
                        break;
                    /* parametr 1 == string1 */
                    tTableItem* p1=tableSearch(mainST, (tKey)(I->addr1));

                    if (!p1) ClearAndExit(SEM_ERROR_5);

                    if (p1->data->varType!=TYPE_STR)
                        ClearAndExit(SEM_ERROR_12);

                    char* string1=(char*)p1->data->varValue;

                    /* parametr 2 == string2 */
                    tTableItem* p2=tableSearch(mainST, (tKey)I->addr2);

                    if (!p2) ClearAndExit(SEM_ERROR_5);

                    char* string2;
                    if (p2->data->varType!=TYPE_STR)
                    {
                        string2=strval(p2->data);

                    }
                    else
                    {
                        string2=(char*)p2->data->varValue;
                    }
                    /* instrukce */

                    int len=(strlen(string1)+strlen(string2));

                    char* con_string=sMalloc(len+1);
                    strcpy(con_string, string1);
                    strcat(con_string, string2);
                    con_string[len]='\0';

                    tData* data=sMalloc(sizeof(tData));
                    data->varType = TYPE_STR;
                    data->varValue=(void*)con_string;
                    tableInsert(mainST, (tKey)I->addr3, data);

                    tableReInit(&tmpST);
                }
                break;

            case I_MORE:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    bool* more = sMalloc(sizeof(bool));
                    //-----------------------------NULL > NULL = BUDE TO BOOL-----------------------------
                    if((param1->data->varType==TYPE_NULL) && (param2->data->varType==TYPE_NULL)) {
                        *more = false;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)more;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------BOOL > BOOL = BUDE TO BOOL-----------------------------
                    else if((param1->data->varType==TYPE_BOOL) && (param2->data->varType==TYPE_BOOL)) {
                        bool* data1 = (bool*)param1->data->varValue;
                        bool* data2 = (bool*)param2->data->varValue;

                        if ((*data1) && !(*data2))
                            *more = true;
                        else
                            *more = false;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)more;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------INT > INT = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        *more = (*data1)>(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)more;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL > DBL = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        *more = (*data1)>(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)more;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //---------------------------STRING > STRING = BUDE TO BOOL---------------------------
                    else if((param1->data->varType==TYPE_STR) && (param2->data->varType==TYPE_STR)) {
                        char* data1 = (char*)param1->data->varValue;
                        char* data2 = (char*)param2->data->varValue;

                        *more =(strcmp(data1, data2)>0);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)more;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else
                    {
                        tData* vysledek = sMalloc(sizeof(tData));
                        bool *data = sMalloc(sizeof(bool));
                        vysledek->varType = TYPE_BOOL;
                        *data = false;
                        vysledek->varValue = (void*)data;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                    }
                }
                break;
            case I_LESS:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    bool* less = sMalloc(sizeof(bool));
                    //-----------------------------NULL < NULL = BUDE TO BOOL-----------------------------
                    if((param1->data->varType==TYPE_NULL) && (param2->data->varType==TYPE_NULL)) {
                        *less = false;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)less;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------BOOL < BOOL = BUDE TO BOOL-----------------------------
                    else if((param1->data->varType==TYPE_BOOL) && (param2->data->varType==TYPE_BOOL)) {
                        bool* data1 = (bool*)param1->data->varValue;
                        bool* data2 = (bool*)param2->data->varValue;

                        if (!(*data1) && (*data2))
                            *less = true;
                        else
                            *less = false;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)less;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------INT < INT = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        *less = (*data1)<(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)less;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL < DBL = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        *less = (*data1)<(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)less;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //---------------------------STRING < STRING = BUDE TO BOOL---------------------------
                    else if((param1->data->varType==TYPE_STR) && (param2->data->varType==TYPE_STR)) {
                        char* data1 = (char*)param1->data->varValue;
                        char* data2 = (char*)param2->data->varValue;

                        *less = (strcmp(data1, data2)<0);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)less;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else
                    {
                        tData* vysledek = sMalloc(sizeof(tData));
                        bool *data = sMalloc(sizeof(bool));
                        vysledek->varType = TYPE_BOOL;
                        *data = false;
                        vysledek->varValue = (void*)data;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                    }
                }
                break;
            case I_MOREEQ:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    bool* moreeq = sMalloc(sizeof(bool));
                    //-----------------------------NULL >= NULL = BUDE TO BOOL-----------------------------
                    if((param1->data->varType==TYPE_NULL) && (param2->data->varType==TYPE_NULL)) {
                        *moreeq = true;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)moreeq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------BOOL >= BOOL = BUDE TO BOOL-----------------------------
                    else if((param1->data->varType==TYPE_BOOL) && (param2->data->varType==TYPE_BOOL)) {
                        bool* data1 = (bool*)param1->data->varValue;
                        bool* data2 = (bool*)param2->data->varValue;

                        if ((*data1) || ((*data1) == (*data2)))
                            *moreeq = true;
                        else
                            *moreeq = false;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)moreeq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------INT >= INT = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        *moreeq = (*data1)>=(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)moreeq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL >= DBL = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        *moreeq = (*data1)>=(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)moreeq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //---------------------------STRING >= STRING = BUDE TO BOOL---------------------------
                    else if((param1->data->varType==TYPE_STR) && (param2->data->varType==TYPE_STR)) {
                        char* data1 = (char*)param1->data->varValue;
                        char* data2 = (char*)param2->data->varValue;

                        *moreeq =(strcmp(data1, data2)>=0);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)moreeq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else
                    {
                        tData* vysledek = sMalloc(sizeof(tData));
                        bool *data = sMalloc(sizeof(bool));
                        vysledek->varType = TYPE_BOOL;
                        *data = false;
                        vysledek->varValue = (void*)data;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                    }
                }
                break;
            case I_LESSEQ:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    bool* lesseq = sMalloc(sizeof(bool));
                    //-----------------------------NULL <= NULL = BUDE TO BOOL-----------------------------
                    if((param1->data->varType==TYPE_NULL) && (param2->data->varType==TYPE_NULL)) {
                        *lesseq = true;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)lesseq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------BOOL <= BOOL = BUDE TO BOOL-----------------------------
                    else if((param1->data->varType==TYPE_BOOL) && (param2->data->varType==TYPE_BOOL)) {
                        bool* data1 = (bool*)param1->data->varValue;
                        bool* data2 = (bool*)param2->data->varValue;

                        if ((*data2) || ((*data1) == (*data2)))
                            *lesseq = true;
                        else
                            *lesseq = false;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)lesseq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------INT <= INT = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        *lesseq = (*data1)<=(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)lesseq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL <= DBL = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        *lesseq = (*data1)<=(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)lesseq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //---------------------------STRING <= STRING = BUDE TO BOOL---------------------------
                    else if((param1->data->varType==TYPE_STR) && (param2->data->varType==TYPE_STR)) {
                        char* data1 = (char*)param1->data->varValue;
                        char* data2 = (char*)param2->data->varValue;

                        *lesseq = (strcmp(data1, data2)<=0);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)lesseq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else
                    {
                        tData* vysledek = sMalloc(sizeof(tData));
                        bool *data = sMalloc(sizeof(bool));
                        vysledek->varType = TYPE_BOOL;
                        *data = false;
                        vysledek->varValue = (void*)data;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                    }
                }
                break;
            case I_EQUAL:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    bool* eq = sMalloc(sizeof(bool));
                    //-----------------------------NULL == NULL = BUDE TO BOOL-----------------------------
                    if((param1->data->varType==TYPE_NULL) && (param2->data->varType==TYPE_NULL)) {
                        *eq = true;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)eq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------BOOL == BOOL = BUDE TO BOOL-----------------------------
                    else if((param1->data->varType==TYPE_BOOL) && (param2->data->varType==TYPE_BOOL)) {
                        bool* data1 = (bool*)param1->data->varValue;
                        bool* data2 = (bool*)param2->data->varValue;

                        *eq = (*data1)==(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)eq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------INT == INT = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        *eq = (*data1)==(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)eq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL == DBL = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        *eq = (*data1)==(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)eq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //---------------------------STRING == STRING = BUDE TO BOOL---------------------------
                    else if((param1->data->varType==TYPE_STR) && (param2->data->varType==TYPE_STR)) {
                        char* data1 = (char*)param1->data->varValue;
                        char* data2 = (char*)param2->data->varValue;

                        *eq = (strcmp(data1, data2)==0);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)eq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else
                    {
                        tData* vysledek = sMalloc(sizeof(tData));
                        bool *data = sMalloc(sizeof(bool));
                        vysledek->varType = TYPE_BOOL;
                        *data = false;
                        vysledek->varValue = (void*)data;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                    }
                }
                break;
            case I_NOTEQUAL:
                {
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tTableItem* param1 = tableSearch(mainST, (tKey)I->addr1);
                    tTableItem* param2 = tableSearch(mainST, (tKey)I->addr2);

                    if (!param1 || !param2) ClearAndExit(SEM_ERROR_5);
                    bool* noteq = sMalloc(sizeof(bool));
                    //-----------------------------NULL != NULL = BUDE TO BOOL-----------------------------
                    if((param1->data->varType==TYPE_NULL) && (param2->data->varType==TYPE_NULL)) {
                        *noteq = false;

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)noteq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------BOOL != BOOL = BUDE TO BOOL-----------------------------
                    else if((param1->data->varType==TYPE_BOOL) && (param2->data->varType==TYPE_BOOL)) {
                        bool* data1 = (bool*)param1->data->varValue;
                        bool* data2 = (bool*)param2->data->varValue;

                        *noteq = (*data1)!=(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)noteq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------INT != INT = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_INT) && (param2->data->varType==TYPE_INT)) {
                        int* data1 = (int*)param1->data->varValue;
                        int* data2 = (int*)param2->data->varValue;

                        *noteq = (*data1)!=(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)noteq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //------------------------------DBL != DBL = BUDE TO BOOL------------------------------
                    else if((param1->data->varType==TYPE_DBL) && (param2->data->varType==TYPE_DBL)) {
                        double* data1 = (double*)param1->data->varValue;
                        double* data2 = (double*)param2->data->varValue;

                        *noteq = (*data1)!=(*data2);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)noteq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //---------------------------STRING != STRING = BUDE TO BOOL---------------------------
                    else if((param1->data->varType==TYPE_STR) && (param2->data->varType==TYPE_STR)) {
                        char* data1 = (char*)param1->data->varValue;
                        char* data2 = (char*)param2->data->varValue;

                        *noteq = (strcmp(data1, data2)!=0);

                        tData* vysledek = sMalloc(sizeof(tData));
                        vysledek->varType = TYPE_BOOL;
                        vysledek->varValue = (void*)noteq;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                        break;
                    }
                    //-----------------------------NEKOMPATIBILNI PARAMETRY-------------------------------
                    else
                    {
                        tData* vysledek = sMalloc(sizeof(tData));
                        bool *data = sMalloc(sizeof(bool));
                        vysledek->varType = TYPE_BOOL;
                        *data = true;
                        vysledek->varValue = (void*)data;
                        tableInsert(mainST, (tKey)I->addr3, vysledek);
                    }
                }
                break;
            case I_LABEL:
                {
                } break;
            case I_GOTO: // Instrukce pro skok na jiny prvek instrukcniho listu
                {
                    tListItem *pom;
                    pom = (tListItem*)I->addr1;
                    listGoto(&instrList, pom);
                }
                break;
            case I_IFGOTO:
                {
                    tListItem *pom;
                    tTableItem *symbol;
                    tKey key;
                    key = (tKey)I->addr1;
                    symbol = tableSearch(mainST,key);
                    if (!symbol) ClearAndExit(SEM_ERROR_5);
                    if (boolval(symbol->data)) {
                        pom = (tListItem*)I->addr2;
                    } else {
                        if (I->addr3 == NULL) break;
                        pom = (tListItem*)I->addr3;
                    }
                    listGoto(&instrList, pom);
                }
                break;
            case I_PUSH:
                {
                    tKey keyFrom;
                    tKey keyTo;
                    tFTableItem *funkce = (tFTableItem*)I->addr3;
                    if (!funkce) ClearAndExit(SEM_ERROR_3_1);

                    if (funkce->data->argc <= counter)
                        break;

                    keyFrom = (tKey)I->addr1;
                    keyTo = getArgKey(&FT, funkce->key, counter);
                    tTableItem *symbol = tableSearch(mainST, keyFrom);
                    if (!symbol) ClearAndExit(SEM_ERROR_5);
                    tableInsert(&tmpST, keyTo, symbol->data);
                    counter++;
                }
                break;

            case I_RETURN:
                {
                    tTableItem *symbol;
                    tKey klic = (tKey)I->addr1;
                    symbol = tableSearch(mainST, klic);
                    if (!symbol) ClearAndExit(SEM_ERROR_5);
                    tableInsert(mainST, "return", symbol->data);
                    return 0;
                }
                break;
            case I_SETVAL:
                {
                    tKey key;
                    tKey save;
                    save = (tKey)I->addr3;
                    if (I->addr2 != NULL) {
                        tData *data = sMalloc(sizeof(tData));
                        int *type;
                        type = (int*)I->addr2;
                        data->varType = *type;
                        data->varValue = (void*)I->addr1; // (void*) cast is not neccesary - both are already (void*)
                        tableInsert(mainST, save, data);
                    } else {
                        tTableItem *symbol;
                        key = (tKey)I->addr1;
                        symbol = tableSearch(mainST,key);
                        if (!symbol) ClearAndExit(SEM_ERROR_5);
                        tableInsert(mainST, save, symbol->data);
                    }
                } break;
            case I_F_JUMP:
                {
                    tListItem *backupActual = instrList.active; // Zaloha Aktualni isntrukce
                    tFTableItem* funkce = fTableSearch(&FT, (tKey)I->addr1); // Identifikace funkce
                    if (!funkce)
                        ClearAndExit(SEM_ERROR_3_1);

                    if (counter < funkce->data->argc)
                        ClearAndExit(SEM_ERROR_4);

                    counter = 0;
                    listGoto(&instrList, (void*)funkce->label); // Skok na label funkce
                    inter(&tmpST); // Rekurzivni volani interpretu
                    listGoto(&instrList, (void*)backupActual); // Obnova aktualniho prvku

                    // Vlozeni navratove hodnoty na spravne misto
                    tTableItem* returnVal = tableSearch(&tmpST, "return");
                    if (!returnVal)
                        ClearAndExit(SEM_ERROR_5);
                    tableInsert(mainST, (tKey)I->addr3, (tData*)returnVal->data);

                    // Vyprazdneni pomocne tabulky
                    tableReInit(&tmpST);
                } break;
            case I_WRITE:
                {
                    tTableItem *symbol;
                    tKey termKey = (tKey)I->addr1;
                    symbol = tableSearch(mainST, termKey);
                    if (!symbol) ClearAndExit(SEM_ERROR_5);
                    fprintf(stdout,strval(symbol->data));
                    tData *returnVal = sMalloc(sizeof(tData));
                    returnVal->varType = TYPE_INT;
                    returnVal->varValue = (void*)((int*)I->addr2);
                    tableInsert(mainST, (tKey)I->addr3, returnVal);
                } break;
            case I_BOOLVAL:
                {
                    if (counter < 1) ClearAndExit(SEM_ERROR_4);
                    tTableItem *term;
                    tData *result = sMalloc(sizeof(tData));
                    int returnVal;
                    tKey key;

                    counter = 0; // Reset pocitadla parametru
                    key = getArgKey(&FT, "boolval", 0);
                    term = tableSearch(&tmpST, key);
                    if (!term) ClearAndExit(SEM_ERROR_5);
                    returnVal = boolval((tData*)term->data);
                    result->varType = TYPE_BOOL;
                    result->varValue = (void*)&returnVal;
                    tableInsert(mainST, (tKey)I->addr3, result);

                    // Vyprazdneni pomocne tabulky
                    tableReInit(&tmpST);
                }
                break;
            case I_DOUBLEVAL:
                {
                    if (counter < 1) ClearAndExit(SEM_ERROR_4);
                    tTableItem *term;
                    tData *result = sMalloc(sizeof(tData));
                    double returnVal;
                    tKey key;

                    counter = 0; // Reset pocitadla parametru
                    key = getArgKey(&FT, "doubleval", 0);
                    term = tableSearch(&tmpST, key);
                    if (!term) ClearAndExit(SEM_ERROR_5);
                    returnVal = doubleval((tData*)term->data);
                    result->varType = TYPE_DBL;
                    result->varValue = (void*)&returnVal;
                    tableInsert(mainST, (tKey)I->addr3, result);

                    // Vyprazdneni pomocne tabulky
                    tableReInit(&tmpST);
                }
                break;
            case I_INTVAL:
                {
                    if (counter < 1) ClearAndExit(SEM_ERROR_4);
                    tTableItem *term;
                    tData *result = sMalloc(sizeof(tData));
                    int returnVal;
                    tKey key;

                    counter = 0; // Reset pocitadla parametru
                    key = getArgKey(&FT, "intval", 0);
                    term = tableSearch(&tmpST, key);
                    if (!term) ClearAndExit(SEM_ERROR_5);
                    returnVal = intval((tData*)term->data);
                    result->varType = TYPE_INT;
                    result->varValue = (void*)&returnVal;
                    tableInsert(mainST, (tKey)I->addr3, result);

                    // Vyprazdneni pomocne tabulky
                    tableReInit(&tmpST);
                }
                break;
            case I_STRVAL:
                {
                    if (counter < 1) ClearAndExit(SEM_ERROR_4);
                    tTableItem *term;
                    tData *result = sMalloc(sizeof(tData));
                    char *returnVal;
                    tKey key;

                    counter = 0; // Reset pocitadla parametru
                    key = getArgKey(&FT, "strval", 0);
                    term = tableSearch(&tmpST, key);
                    if (!term) ClearAndExit(SEM_ERROR_5);
                    returnVal = strval((tData*)term->data);
                    result->varType = TYPE_STR;
                    result->varValue = (void*)returnVal;
                    tableInsert(mainST, (tKey)I->addr3, result);

                    // Vyprazdneni pomocne tabulky
                    tableReInit(&tmpST);
                }
                break;
            case I_FINDSTR:
                {
                    if (counter < 2)
                        ClearAndExit(SEM_ERROR_4);

                    if (I->addr3==NULL)
                        break;

                    tFTableItem* func = fTableSearch(&FT, "find_string");
                    if (!func) ClearAndExit(SEM_ERROR_3_1);

                    tKey key1  = getArgKey(&FT, func->key, 0);
                    tKey key2  = getArgKey(&FT, func->key, 1);
                    counter = 0;

                    /* parametr 1 == string1 */
                    tTableItem* p1=tableSearch(&tmpST, key1);
                    if (!p1) ClearAndExit(SEM_ERROR_5);

                    char* string1;
                    if (p1->data->varType!=TYPE_STR)
                        string1=strval(p1->data);
                    else
                        string1=(char*)p1->data->varValue;

                    /* parametr 2 == string2 */
                    tTableItem* p2=tableSearch(&tmpST, key2);
                    if (!p2) ClearAndExit(SEM_ERROR_5);

                    char* string2;
                    if (p2->data->varType!=TYPE_STR)
                        string2=strval(p2->data);
                    else
                        string2=(char*)p2->data->varValue;

                    /* instrukce */

                    int *index=sMalloc(sizeof(int));
                    *index=find_string(string1, string2);
                    tData* data=sMalloc(sizeof(tData));
                    data->varType = TYPE_INT;
                    data->varValue=(void*) index;
                    tableInsert(mainST, (tKey)I->addr3, data);

                    tableReInit(&tmpST);

                }
                break;
            case I_SORTSTR:
                {
                    if (counter < 1) ClearAndExit(SEM_ERROR_4);
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tFTableItem* func = fTableSearch(&FT, "sort_string");
                    if (!func) ClearAndExit(SEM_ERROR_3_1);
                    tKey key1 = getArgKey(&FT, func->key, 0);
                    counter = 0;

                    tTableItem* p1 = tableSearch(&tmpST, (tKey)key1);
                    if (!p1) ClearAndExit(SEM_ERROR_5);
                    tData* input = (tData*)p1->data;

                    //----------------------------------VOLANI INSTRUKCE----------------------------------
                    tData* data = sMalloc(sizeof(tData));
                    data->varType = TYPE_STR;
                    data->varValue = (void*)(sort_string(input));
                    tableInsert(mainST, (tKey)I->addr3, data);
                }
                break;
            case I_STRLEN:
                {
                    if (counter < 1) ClearAndExit(SEM_ERROR_4);
                    //---------------------------------NACTENI PARAMETRU----------------------------------
                    tFTableItem* func = fTableSearch(&FT, "strlen");
                    if (!func) ClearAndExit(SEM_ERROR_3_1);
                    tKey key = getArgKey(&FT, func->key, 0);

                    counter = 0;
                    tTableItem* p1 = tableSearch(&tmpST, key);
                    if (!p1) ClearAndExit(SEM_ERROR_5);
                    tData* input = (tData*)p1->data;

                    //----------------------------------VOLANI INSTRUKCE----------------------------------
                    tData* data = sMalloc(sizeof(tData));
                    data->varType = TYPE_INT;
                    int *result = sMalloc(sizeof(int));
                    *result = strleng(input);
                    data->varValue = (void*)(result);
                    tableInsert(mainST, (tKey)I->addr3, data);

                    tableReInit(&tmpST);
                } break;
            case I_SUBSTR:
                {
                    if (counter < 3) ClearAndExit(SEM_ERROR_4);

                    tFTableItem* func = fTableSearch(&FT, "get_substring");
                    if (!func) ClearAndExit(SEM_ERROR_3_1);
                    tKey key1  = getArgKey(&FT, func->key, 0);
                    tKey key2  = getArgKey(&FT, func->key, 1);
                    tKey key3  = getArgKey(&FT, func->key, 2);
                    counter = 0;

                    /* parametr 1 == string */
                    tTableItem* p1=tableSearch(&tmpST, (tKey)key1);
                    if (!p1) ClearAndExit(SEM_ERROR_5);
                    char* input_string;
                    if (p1->data->varType!=TYPE_STR)
                        input_string=strval(p1->data);
                    else
                        input_string=(char*)p1->data->varValue;


                    /* parametr 2 == integer1 */
                    tTableItem* p2=tableSearch(&tmpST, (tKey)key2);
                    if (!p2) ClearAndExit(SEM_ERROR_5);
                    unsigned int* input_int1 = sMalloc(sizeof(int));
                    if (p2->data->varType!=TYPE_INT)
                        *input_int1=intval(p2->data);
                    else
                        input_int1 = (unsigned int*)p2->data->varValue;

                    /* parametr 3 == integer2 */
                    tTableItem* p3=tableSearch(&tmpST, (tKey)key3);
                    if (!p3) ClearAndExit(SEM_ERROR_5);
                    unsigned int* input_int2 = sMalloc(sizeof(int));
                    if (p3->data->varType!=TYPE_INT)
                        *input_int2=intval(p3->data);
                    else
                        input_int2 = (unsigned int*)p3->data->varValue;

                    /* instrukce */
                    tData* data=sMalloc(sizeof(tData));
                    data->varType = TYPE_STR;
                    data->varValue=(void*)(get_substring(input_string, *input_int1, *input_int2));
                    tableInsert(mainST, (tKey)I->addr3, data);

                    tableReInit(&tmpST);
                }
                break;
                default: ClearAndExit(INTERNAL_ERROR);
        }

        listNext(&instrList);
    }
    return EXIT_SUCCESS;
}
