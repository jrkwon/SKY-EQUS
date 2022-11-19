/*
# clearcase: CmicroPatchRel2.3.3
*/

/*
+------------------------------------------------------------------------------+
|                                                                              |
|  Copyright by Telelogic AB 1993 - 1998                                       | 
|  Copyright by S&P Media GmbH Germany 1993 - 1998                             | 
|                                                                              |
|  This Program is owned by Telelogic and is protected by national             | 
|  copyright laws and international copyright treaties. Telelogic              | 
|  grants you the right to use this Program on one computer or in              | 
|  one local computer network at any one time.                                 | 
|  Under this License you may only modify the source code for the purpose      | 
|  of adapting it to your environment. You must reproduce and include          | 
|  any copyright and trademark notices on all copies of the source code.       | 
|  You may not use, copy, merge, modify or transfer the Program except as      | 
|  provided in this License.                                                   |
|  Telelogic does not warrant that the Program will meet your                  | 
|  requirements or that the operation of the Program will be                   | 
|  uninterrupted and error free. You are solely responsible that the           | 
|  selection of the Program and the modification of the source code            | 
|  will achieve your intended results and that the results are actually        | 
|  obtained.                                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/

/*
# clearcase: CmicroPatchRel2.3.3
*/

#ifndef X_SCTPREDG_H
#define X_SCTPREDG_H

/* This file contains implementation of the predefined data types
   except PId. The functions defined here can be found in the file
   sctpred.c.
*/
/*
FILE INDEX
04   Generator String
05   Generator Array  (array that cannot become array in C)
06   Generator Powerset  (limited and discrete component type)
07   Generator Powerset  (general component type)
08   Generator Bag
*/

#ifndef XNO_LONG_MACROS

/****+***************************************************************
04   Generator String
********************************************************************/

/* typedef for String type ----------------------------------------*/

#define String_Type(SORT, COMPONENTSORT) \
   typedef struct XCAT(SORT,_ystruct)  * XCAT(SORT,_yptr); \
   typedef struct XCAT(SORT,_ystruct) { \
     XCAT(SORT,_yptr)   Suc; \
     COMPONENTSORT      Data; \
   } XCAT(SORT,_yrec); \
   typedef struct { \
     XCAT(SORT,_yptr) First; \
     XCAT(SORT,_yptr) Last; \
     int              Length; \
     xbool            IsAssigned; \
   } SORT;


/* String operator prototypes -------------------------------------*/

#define String_prototypes(SORT, COMPONENTSORT) \
  extern SORT XCAT(yEmptystring_,SORT) XPP((void)); \
  extern SORT XCAT(yMkString_,SORT) XPP((COMPONENTSORT)); \
  extern SDL_Integer XCAT(yLength_,SORT) XPP((SORT)); \
  extern COMPONENTSORT XCAT(yFirst_,SORT) XPP((SORT)); \
  extern COMPONENTSORT XCAT(yLast_,SORT) XPP((SORT)); \
  extern SORT XCAT(yConcat_,SORT) XPP((SORT, SORT)); \
  extern SORT XCAT(ySubString_,SORT) \
     XPP((SORT, SDL_Integer, SDL_Integer)); \
  extern COMPONENTSORT XCAT(yExtr_,SORT) XPP((SORT, SDL_Integer)); \
  extern COMPONENTSORT *XCAT(yAddr_,SORT) XPP((SORT *, SDL_Integer)); \
  \
  extern XCAT(SORT,_yptr)  XCAT(yGetAvail_,SORT) XPP((void)); \
  extern void XCAT(yAppend_,SORT) XPP((SORT *result, SORT S)); \
  \
  extern char * XCAT(yWri_,SORT) XPP((void * S)); \
  extern int XCAT(yRead_,SORT) XPP((void * Result));



/* String operator implementations --------------------------------*/

/*-- yEmptystring_SORT (String)------------------------------------*/
#define String_Emptystring(SORT, COMPONENTSORT) \
  SORT XCAT(yEmptystring_,SORT) XPP((void)) \
  { \
    SORT Result; \
    Result.First = (XCAT(SORT,_yptr))0; \
    Result.Last = (XCAT(SORT,_yptr))0; \
    Result.Length = 0; \
    Result.IsAssigned = (xbool)0; \
    return Result; \
  }


/*-- yMkString_SORT (String)---------------------------------------*/
#define String_MkString(SORT, COMPONENTSORT) \
  XCAT(SORT,_yptr) XCAT(yGetAvail_,SORT) XPP((void)) \
  { \
    XCAT(SORT,_yptr) Item; \
    if (XCAT(yAvail_,SORT) != (XCAT(SORT,_yptr))0) { \
      Item = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = XCAT(yAvail_,SORT)->Suc; \
      Item->Suc = (XCAT(SORT,_yptr))0; \
    } else { \
      Item = (XCAT(SORT,_yptr))xAlloc((unsigned)sizeof(XCAT(SORT,_yrec))); \
    } \
    XCAT(yDef_,COMPONENTSORT)(&Item->Data); \
    return Item; \
  } \
  \
  SORT XCAT(yMkString_,SORT) \
    XPP_PROTO( (COMPONENTSORT Component) ) \
    XPP_NOPROTO( (Component) COMPONENTSORT Component; ) \
  { \
    SORT Result; \
    Result.First = XCAT(yGetAvail_,SORT)(); \
    Result.Last = Result.First; \
    Result.Length = 1; \
    Result.IsAssigned = (xbool)0; \
    XCAT(yAssF_,COMPONENTSORT)(Result.First->Data, Component, XASS); \
    return Result; \
  }


/*-- yLength_SORT (String)-----------------------------------------*/
#define String_Length(SORT, COMPONENTSORT) \
  SDL_Integer XCAT(yLength_,SORT) \
    XPP_PROTO( (SORT SIn) ) \
    XPP_NOPROTO( (SIn) SORT SIn; ) \
  { \
    if (! SIn.IsAssigned && SIn.First != (XCAT(SORT,_yptr))0) { \
      XCAT(yFree_,SORT)((void **)&SIn); \
    } \
    return SIn.Length; \
  }

/*-- yFirst_SORT (String)------------------------------------------*/
#define String_First(SORT, COMPONENTSORT) \
  COMPONENTSORT XCAT(yFirst_,SORT) \
    XPP_PROTO( (SORT SIn) ) \
    XPP_NOPROTO( (SIn) SORT SIn; ) \
  { \
    COMPONENTSORT Result; \
    if (SIn.Length > 0) { \
      Result = SIn.First->Data; \
      if (! SIn.IsAssigned) { \
        XCAT(yFree_,SORT)((void **)&SIn); \
      } \
    } else { \
      xReportStringError("First in String sort", "Length is zero") \
      (void)memset((void *)(&Result), 0, sizeof(COMPONENTSORT)); \
      XCAT(yDef_,COMPONENTSORT)(&Result); \
    } \
    return Result; \
  }


/*-- yLast_SORT (String)-------------------------------------------*/
#define String_Last(SORT, COMPONENTSORT) \
  COMPONENTSORT XCAT(yLast_,SORT) \
    XPP_PROTO( (SORT SIn) ) \
    XPP_NOPROTO( (SIn) SORT SIn; ) \
  { \
    COMPONENTSORT Result; \
    if (SIn.Length > 0) { \
      Result = SIn.Last->Data; \
      if (! SIn.IsAssigned) { \
        XCAT(yFree_,SORT)((void **)&SIn); \
      } \
    } else { \
      xReportStringError("Last in String sort", "Length is zero") \
      (void)memset((void *)(&Result), 0, sizeof(COMPONENTSORT)); \
      XCAT(yDef_,COMPONENTSORT)(&Result); \
    } \
    return Result; \
  }


/*-- yConcat_SORT (String)-----------------------------------------*/
#define String_Concat(SORT, COMPONENTSORT) \
  void XCAT(yAppend_,SORT) \
    XPP_PROTO( (SORT *result, SORT SIn) ) \
    XPP_NOPROTO( (result, SIn) SORT *result; SORT SIn; ) \
  { \
    XCAT(SORT,_yptr)  TempSVar, TempExpr; \
    \
    (*result).Length = (*result).Length + SIn.Length; \
    if (SIn.IsAssigned) { \
      TempExpr = SIn.First; \
      TempSVar = XCAT(yGetAvail_,SORT)(); \
      if ((*result).First == (XCAT(SORT,_yptr))0) \
        (*result).First = TempSVar; \
      else \
        (*result).Last->Suc = TempSVar; \
      XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
      \
      while (TempExpr->Suc != (XCAT(SORT,_yptr))0) { \
        TempExpr = TempExpr->Suc; \
        TempSVar->Suc = XCAT(yGetAvail_,SORT)(); \
        TempSVar = TempSVar->Suc; \
        XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
      } \
      (*result).Last = TempSVar; \
    } else { \
      if ((*result).First == (XCAT(SORT,_yptr))0) \
        (*result).First = SIn.First; \
      else \
        (*result).Last->Suc = SIn.First; \
      (*result).Last = SIn.Last; \
    } \
  } \
  \
  \
  SORT XCAT(yConcat_,SORT) \
    XPP_PROTO( (SORT S1, SORT S2) ) \
    XPP_NOPROTO( (S1, S2) SORT S1; SORT S2; ) \
  { \
    SORT result; \
    \
    result.First = (XCAT(SORT,_yptr))0; \
    result.Last = (XCAT(SORT,_yptr))0; \
    result.Length = 0; \
    result.IsAssigned = (xbool)0; \
    if (S1.Length == 0) { \
      if (S2.Length == 0) \
        return result; \
      else if (! S2.IsAssigned) \
        return S2; \
      else { \
        XCAT(yAppend_,SORT)(&result, S2); \
      } \
    } else if (S2.Length == 0) { \
      if (! S1.IsAssigned) \
        return S1; \
      else { \
        XCAT(yAppend_,SORT)(&result, S1);  \
      } \
    } else { \
      XCAT(yAppend_,SORT)(&result, S1); \
      XCAT(yAppend_,SORT)(&result, S2); \
    } \
    \
    return result; \
  }


/*-- ySubstring_SORT (String)--------------------------------------*/
#define String_Substring(SORT, COMPONENTSORT) \
  SORT XCAT(ySubString_,SORT) \
    XPP_PROTO( (SORT SIn, SDL_Integer Start, SDL_Integer SubLength) ) \
    XPP_NOPROTO( (SIn, Start, SubLength) SORT SIn; SDL_Integer Start; SDL_Integer SubLength; ) \
  { \
    SORT result; \
    XCAT(SORT,_yptr)  TempSVar, TempExpr; \
    int I; \
    \
    result.First = (XCAT(SORT,_yptr))0; \
    result.Last = (XCAT(SORT,_yptr))0; \
    result.Length = 0; \
    result.IsAssigned = (xbool)0; \
    if ( SIn.Length <= 0 ) { \
      xReportStringError("SubString in string sort", "Length is zero" ) \
    } else if ( Start <= 0 ) { \
      xReportStringError("SubString in string sort", \
                         "Start is less than or equal to zero" ) \
    } else if ( SubLength < 0 ) { \
      xReportStringError("SubString in string sort", \
                         "SubLength is less than zero" ) \
    } else if ( SubLength == 0 ) { \
      /* No error if length is 0 */ \
    } else if ( Start+SubLength-1 > SIn.Length ) { \
      xReportStringError("SubString in string sort", \
        "Start+Substring-1 length is greater than string length" ) \
    } else { \
      TempExpr = SIn.First; \
      for (I=1; I<Start; I++) TempExpr = TempExpr->Suc; \
      TempSVar = XCAT(yGetAvail_,SORT)(); \
      result.First = TempSVar; \
      XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
      \
      for (I=2; I<=SubLength; I++) { \
        TempExpr = TempExpr->Suc; \
        TempSVar->Suc = XCAT(yGetAvail_,SORT)(); \
        TempSVar = TempSVar->Suc; \
        XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
      } \
      result.Last = TempSVar; \
      result.Length = SubLength; \
    } \
    if (! SIn.IsAssigned && SIn.First != (XCAT(SORT,_yptr))0) { \
      XCAT(yFree_,SORT)((void **)&SIn); \
    } \
    return result; \
  }



/*-- yModify_SORT (String)-----------------------------------------*/
#define String_Modify(SORT, COMPONENTSORT) \
  COMPONENTSORT *XCAT(yAddr_,SORT) \
    XPP_PROTO( (SORT *SIn, SDL_Integer Index) ) \
    XPP_NOPROTO( (SIn, Index) SORT *SIn; SDL_Integer Index; ) \
  { \
    COMPONENTSORT *result; \
    XCAT(SORT,_yptr) Temp; \
    if ((*SIn).Length == 0) { \
      xReportStringError("Modify of component of string sort", \
                         "String is empty. Returns default value" ) \
      result = (COMPONENTSORT *)xAlloc((xptrint)sizeof(COMPONENTSORT)); \
      XCAT(yDef_,COMPONENTSORT)(result); \
      return result; \
    } else { \
      if (Index > (*SIn).Length) { \
        xReportStringError("Modify of component of string sort", \
                           "Index > string length. Index set to 1" ) \
        Index = 1; \
      } \
      if (Index <= (SDL_Integer)0) { \
        xReportStringError("Modify of component of string sort", \
                           "Index <= 0. Index set to 1" ) \
        Index = 1; \
      } \
      Temp = (*SIn).First; \
      for (; Index>1; Index--) Temp = Temp->Suc; \
      if (! (*SIn).IsAssigned && (*SIn).First != (XCAT(SORT,_yptr))0) { \
        XCAT(yFree_,SORT)((void **)SIn); \
      } \
      return &Temp->Data; \
    } \
  }


/*-- yExtr_SORT (String)--------------------------------------------*/
#define String_Extract(SORT, COMPONENTSORT) \
  COMPONENTSORT XCAT(yExtr_,SORT) \
    XPP_PROTO( (SORT SIn, SDL_Integer Index) ) \
    XPP_NOPROTO( (SIn, Index) SORT SIn; SDL_Integer Index; ) \
  { \
    COMPONENTSORT result; \
    XCAT(SORT,_yptr) Temp; \
    if (SIn.Length == 0) { \
      xReportStringError("Extract in string sort", \
                         "String is empty. Returns default value" ) \
      (void)memset((void *)(&result), 0, sizeof(COMPONENTSORT)); \
      XCAT(yDef_,COMPONENTSORT)(&result); \
      return result; \
    } else { \
      if (Index > SIn.Length) { \
        xReportStringError("Extract in string sort", \
                           "Index > string length. Index set to 1" ) \
        Index = 1; \
      } \
      if (Index <= (SDL_Integer)0) { \
        xReportStringError("Extract in string sort", \
                           "Index <= 0. Index set to 1" ) \
        Index = 1; \
      } \
      Temp = SIn.First; \
      for (; Index>1; Index--) Temp = Temp->Suc; \
      if (! SIn.IsAssigned && SIn.First != (XCAT(SORT,_yptr))0) { \
        XCAT(yFree_,SORT)((void **)&SIn); \
      } \
      return Temp->Data; \
    } \
  }


/*-- yAss_SORT (String)--------------------------------------------*/
#define String_Assign(SORT, COMPONENTSORT) \
  static XCAT(SORT,_yptr)  XCAT(yAvail_,SORT) = (XCAT(SORT,_yptr))0; \
  void XCAT(yAss_,SORT) \
    XPP_PROTO( (SORT * SVar, SORT SExpr, int AssType) ) \
    XPP_NOPROTO( (SVar, SExpr, AssType)  SORT * SVar; SORT SExpr; int AssType; ) \
  { \
    XCAT(SORT,_yptr)  TempSVar, TempExpr; \
    \
    if ((*SVar).First != (XCAT(SORT,_yptr))0 && (*SVar).First == SExpr.First) \
      return; \
    if (AssType == XASS && (*SVar).First != (XCAT(SORT,_yptr))0) { \
      XCAT(yFree_,SORT)((void **)SVar); \
    } \
    if ( (SExpr.IsAssigned || AssType == XASS2MAKE) && \
         SExpr.First != (XCAT(SORT,_yptr))0) { \
      TempExpr = SExpr.First; \
      TempSVar = XCAT(yGetAvail_,SORT)(); \
      (*SVar).First = TempSVar; \
      XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
      \
      TempExpr = TempExpr->Suc; \
      while (TempExpr != (XCAT(SORT,_yptr))0) { \
        TempSVar->Suc = XCAT(yGetAvail_,SORT)(); \
        TempSVar = TempSVar->Suc; \
        XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
        TempExpr = TempExpr->Suc; \
      } \
      (*SVar).Last = TempSVar; \
    } else { \
      (*SVar).First = SExpr.First; \
      (*SVar).Last = SExpr.Last; \
    } \
    (*SVar).Length = SExpr.Length; \
    (*SVar).IsAssigned = (xbool)1; \
  }


/*-- yEq_SORT (String)---------------------------------------------*/
#define String_Equal(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yEq_,SORT) \
    XPP_PROTO( (SORT Expr1, SORT Expr2) ) \
    XPP_NOPROTO( (Expr1, Expr2) SORT Expr1; SORT Expr2; ) \
  { \
    XCAT(SORT,_yptr)  Temp1, Temp2; \
    SDL_Boolean  Result; \
    \
    Result = SDL_True; \
    if (Expr1.Length != Expr2.Length) \
      Result = SDL_False; \
    Temp1 = Expr1.First; \
    Temp2 = Expr2.First; \
    while (Result && Temp1 != (XCAT(SORT,_yptr))0) { \
      if (XCAT(yNEqF_,COMPONENTSORT)(Temp1->Data, Temp2->Data) ) \
        Result = SDL_False; \
      Temp1 = Temp1->Suc; \
      Temp2 = Temp2->Suc; \
    } \
    if (! Expr1.IsAssigned && Expr1.First != (XCAT(SORT,_yptr))0) { \
      XCAT(yFree_,SORT)((void **)&Expr1); \
    } \
    if (! Expr2.IsAssigned && Expr2.First != (XCAT(SORT,_yptr))0) { \
      XCAT(yFree_,SORT)((void **)&Expr2); \
    } \
    return Result; \
  }

/*-- yDef_SORT (String)--------------------------------------------*/
#define String_Default(SORT, COMPONENTSORT) \
  void XCAT(yDef_,SORT) \
    XPP_PROTO( (SORT *SIn) ) \
    XPP_NOPROTO( (SIn) SORT *SIn; ) \
  { \
    (*SIn).First = (XCAT(SORT,_yptr))0; \
    (*SIn).Last = (XCAT(SORT,_yptr))0; \
    (*SIn).Length = 0; \
    (*SIn).IsAssigned = (xbool)1; \
  }


/*-- yFree_SORT (String)-------------------------------------------*/
#define String_Free(SORT, COMPONENTSORT, FREEFUNC) \
  void XCAT(yFree_,SORT) \
    XPP_PROTO( (void ** C) ) \
    XPP_NOPROTO( (C) void ** C; ) \
  { \
    XCAT(SORT,_yptr)  Temp, TempSuc; \
    if (FREEFUNC != XXFREE) { \
      for (Temp =  (*(SORT *)C).First; \
           Temp != (XCAT(SORT,_yptr))0; \
           Temp =  TempSuc) { \
        FREEFUNC((void **)&Temp->Data); \
        TempSuc =  Temp->Suc; \
        XFREE((void **)&Temp, sizeof(XCAT(SORT,_yrec))); \
      } \
    } \
    else if ((*(SORT *)C).First != (XCAT(SORT,_yptr))0) { \
      (*(SORT *)C).Last->Suc = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = (*(SORT *)C).First; \
    } \
    (*(SORT *)C).First = (XCAT(SORT,_yptr))0; \
    (*(SORT *)C).Last = (XCAT(SORT,_yptr))0; \
    (*(SORT *)C).Length = 0; \
    (*(SORT *)C).IsAssigned = (xbool)0; \
    for (Temp = XCAT(yAvail_,SORT); \
         Temp; \
         Temp = XCAT(yAvail_,SORT)) { \
      XCAT(yAvail_,SORT)  = Temp->Suc; \
      if (FREEFUNC != XXFREE)  FREEFUNC((void **)&Temp->Data); \
      XFREE((void **)&Temp, sizeof(XCAT(SORT,_yrec))); \
    } \
  }

/*-- yWri_SORT (String)-------------------------------------------*/
#ifdef XITEXCOMM
#define String_Write_Start \
  if (XSYSD xUse_Itex_Syntax) \
    (void)strcpy(CTemp, "(: "); \
  else
#define String_Write_End \
  if (XSYSD xUse_Itex_Syntax) \
    xSafeStrcat(CTemp, " :)"); \
  else
#else
#define String_Write_Start
#define String_Write_End
#endif

#define String_Write(SORT, COMPONENTSORT) \
  char * XCAT(yWri_,SORT) \
    XPP_PROTO( (void * SIn) ) \
    XPP_NOPROTO( (SIn) void * SIn; ) \
  { \
    XCAT(SORT,_yptr) Temp, TempLast; \
  static  char       CTemp[MAX_WRI_LENGTH]; \
    \
    String_Write_Start \
    if (XASN1_SYNTAX) \
      (void)strcpy(CTemp, "{ "); \
    else \
      (void)strcpy(CTemp, "(. "); \
    TempLast = (*(SORT *)SIn).Last; \
    for (Temp = (*(SORT *)SIn).First; \
         Temp != (XCAT(SORT,_yptr))0; \
         Temp = Temp->Suc) { \
      xSafeStrcat(CTemp, \
             xWriteSort((void *)(&Temp->Data), XCAT(ySrtN_,COMPONENTSORT))); \
      if (strlen(CTemp) == MAX_WRI_LENGTH - 1) \
        return CTemp; \
      if (Temp != TempLast) \
        xSafeStrcat(CTemp, ", "); \
    } \
    String_Write_End \
    if (XASN1_SYNTAX) \
      xSafeStrcat(CTemp, " }"); \
    else \
      xSafeStrcat(CTemp, " .)"); \
    return CTemp; \
  }


/*-- yRead_SORT (String)------------------------------------------*/
#ifdef XITEXCOMM
#define String_Read_StartToken (Token != xxLParColon) &&
#define String_Read_EndToken   (Token != xxRParColon) &&
#define String_Read_EndTokenOr (Token == xxRParColon) ||
#else
#define String_Read_StartToken
#define String_Read_EndToken
#define String_Read_EndTokenOr
#endif

#define String_Read(SORT, COMPONENTSORT) \
  int XCAT(yRead_,SORT) \
    XPP_PROTO( (void * Result) ) \
    XPP_NOPROTO( (Result) void * Result; ) \
  { \
    COMPONENTSORT Item; \
    xxToken       Token; \
    char          strVar[256]; \
    int           Res; \
    \
    (*(SORT *)Result).First = (XCAT(SORT,_yptr))0; \
    (*(SORT *)Result).Last = (XCAT(SORT,_yptr))0; \
    (*(SORT *)Result).Length = 0; \
    (*(SORT *)Result).IsAssigned = (int)0; \
    Token = xScanToken(strVar); \
    if (XVALUE_SYNTAX && \
        String_Read_StartToken \
        Token != xxLParDot && Token != xxLCurlyBracket && \
        Token != xxQuestionMark && Token != xxEoln) { \
      xPrintString("String value should start with (. or {\n"); \
      return 0; \
    } \
    if (String_Read_StartToken \
      Token != xxLParDot && Token != xxLCurlyBracket) \
      xUngetToken(Token, strVar); \
    \
    while (String_Read_EndToken \
           Token != xxRParDot && Token != xxRCurlyBracket) { \
      (void)sprintf(strVar, " (%s) : ", XCAT(ySrtN_,COMPONENTSORT)->Name); \
      (void)memset((void *)(&Item), 0, sizeof(COMPONENTSORT)); \
      Res = xReadOneParameter(XCAT(ySrtN_,COMPONENTSORT), strVar, (void *)&Item); \
      if (Res == 0) return 0; \
      if (Res == 2) { \
        Token = xScanToken(strVar); \
        if (String_Read_EndTokenOr \
            Token == xxRParDot || Token == xxRCurlyBracket) break; \
        return 0; \
      } \
      XCAT(yAppend_,SORT)((SORT *)Result, XCAT(yMkString_,SORT)(Item)); \
      Token = xScanToken(strVar); \
      if (String_Read_EndToken \
          Token != xxRParDot && Token != xxRCurlyBracket) \
        xUngetToken(Token, strVar); \
    } \
    (*(SORT *)Result).IsAssigned = (int)1; \
    return 1; \
  }

/****+***************************************************************
05   Generator Array  (array that cannot become array in C)
********************************************************************/

/* typedef for Array type -----------------------------------------*/

#ifdef ALPHACC
#define Array_Type(SORT, INDEXSORT, COMPONENTSORT) \
   typedef struct XCAT(SORT,_ystruct)  * XCAT(SORT,_yptr); \
   typedef struct XCAT(SORT,_ystruct) { \
     XCAT(SORT,_yptr)   Suc; \
     INDEXSORT          Index; \
     COMPONENTSORT      Data; \
   } XCAT(SORT,_yrec); \
   typedef struct { \
     XCAT(SORT,_yptr) First; \
     XCAT(SORT,_yptr) Last; \
     long             IsAssigned; \
     COMPONENTSORT    Data; \
   } SORT;
#else
#define Array_Type(SORT, INDEXSORT, COMPONENTSORT) \
   typedef struct XCAT(SORT,_ystruct)  * XCAT(SORT,_yptr); \
   typedef struct XCAT(SORT,_ystruct) { \
     XCAT(SORT,_yptr)   Suc; \
     INDEXSORT          Index; \
     COMPONENTSORT      Data; \
   } XCAT(SORT,_yrec); \
   typedef struct { \
     XCAT(SORT,_yptr) First; \
     XCAT(SORT,_yptr) Last; \
     xbool            IsAssigned; \
     COMPONENTSORT    Data; \
   } SORT;
#endif


/* Array operator prototypes --------------------------------------*/

#define Array_prototypes(SORT, INDEXSORT, COMPONENTSORT) \
  extern SORT XCAT(yMake_,SORT) XPP((COMPONENTSORT)); \
  extern COMPONENTSORT XCAT(yExtr_,SORT) XPP((SORT, INDEXSORT)); \
  extern COMPONENTSORT *XCAT(yAddr_,SORT) XPP((SORT *, INDEXSORT)); \
  \
  extern XCAT(SORT,_yptr) XCAT(yGetAvail_,SORT) XPP((void)); \
  extern COMPONENTSORT XCAT(yExtrInner_,SORT) \
         XPP((SORT S, INDEXSORT Index)); \
  \
  extern char * XCAT(yWri_,SORT) XPP((void * S)); \
  extern int XCAT(yRead_,SORT) XPP((void * Result));


/* Array operator implementations ---------------------------------*/

/*-- yMake_SORT (Array)--------------------------------------------*/
#define Array_Make(SORT, INDEXSORT, COMPONENTSORT) \
  SORT XCAT(yMake_,SORT) \
    XPP_PROTO( (COMPONENTSORT Component) ) \
    XPP_NOPROTO( (Component) COMPONENTSORT Component; ) \
  { \
    SORT Result; \
    (void)memset((void *)&Result, 0, sizeof(SORT)); \
    Result.First = (XCAT(SORT,_yptr))0; \
    Result.Last = (XCAT(SORT,_yptr))0; \
    Result.IsAssigned = (xbool)0; \
    XCAT(yAssF_,COMPONENTSORT)(Result.Data, Component, XASS); \
    return Result; \
  }


/*-- yModify_SORT (Array)------------------------------------------*/
#define Array_Modify(SORT, INDEXSORT, COMPONENTSORT) \
  XCAT(SORT,_yptr) XCAT(yGetAvail_,SORT) XPP((void)) \
  { \
    XCAT(SORT,_yptr) Item; \
    if (XCAT(yAvail_,SORT) != (XCAT(SORT,_yptr))0) { \
      Item = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = XCAT(yAvail_,SORT)->Suc; \
      Item->Suc = (XCAT(SORT,_yptr))0; \
    } else { \
      Item = (XCAT(SORT,_yptr))xAlloc((unsigned)sizeof(XCAT(SORT,_yrec))); \
    } \
    return Item; \
  } \
  \
  COMPONENTSORT *XCAT(yAddr_,SORT) \
    XPP_PROTO( (SORT *S, INDEXSORT Index) ) \
    XPP_NOPROTO( (S, Index) SORT *S; INDEXSORT Index; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    Temp = (*S).First; \
    while ( Temp != (XCAT(SORT,_yptr))0  && \
            XCAT(yNEqF_,INDEXSORT)(Temp->Index, Index) ) \
      Temp = Temp->Suc; \
    if (Temp != (XCAT(SORT,_yptr))0) \
      return &Temp->Data; \
    if ((*S).First == (XCAT(SORT,_yptr))0) { \
      (*S).First = XCAT(yGetAvail_,SORT)(); \
      (*S).Last = (*S).First; \
    } else { \
      (*S).Last->Suc = XCAT(yGetAvail_,SORT)(); \
      (*S).Last = (*S).Last->Suc; \
    } \
    XCAT(yAssF_,INDEXSORT)((*S).Last->Index, Index, XASS); \
    XCAT(yAssF_,COMPONENTSORT)((*S).Last->Data, (*S).Data, XASS); \
    return &(*S).Last->Data; \
  }


/*-- yExtr_SORT (Array)---------------------------------------------*/
#define Array_Extract(SORT, INDEXSORT, COMPONENTSORT) \
  \
  COMPONENTSORT XCAT(yExtrInner_,SORT) \
    XPP_PROTO( (SORT S, INDEXSORT Index) ) \
    XPP_NOPROTO( (S, Index) SORT S; INDEXSORT Index; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    Temp = S.First; \
    while ( Temp != (XCAT(SORT,_yptr))0  && \
            XCAT(yNEqF_,INDEXSORT)(Temp->Index, Index) ) \
      Temp = Temp->Suc; \
    if (Temp != (XCAT(SORT,_yptr))0) \
      return Temp->Data; \
    else \
      return S.Data; \
  } \
  \
  COMPONENTSORT XCAT(yExtr_,SORT) \
    XPP_PROTO( (SORT S, INDEXSORT Index) ) \
    XPP_NOPROTO( (S, Index) SORT S; INDEXSORT Index; ) \
  { \
    COMPONENTSORT result; \
    result = XCAT(yExtrInner_,SORT)(S, Index); \
    if (! S.IsAssigned && S.First != (XCAT(SORT,_yptr))0) \
      XCAT(yFree_,SORT)((void **)&S); \
    return result; \
  }


/*-- yAss_SORT (Array)---------------------------------------------*/
#ifdef NO_GLOBAL_VARS
#define Array_Assign(SORT, INDEXSORT, COMPONENTSORT) \
  void XCAT(yAss_,SORT) \
    XPP_PROTO( (SORT * SVar, SORT SExpr, int AssType) ) \
    XPP_NOPROTO( (SVar, SExpr, AssType)  SORT * SVar; SORT SExpr; int AssType; ) \
  { \
    XCAT(SORT,_yptr)  TempSVar, TempExpr, PrevExpr; \
    \
    if ((*SVar).First != SExpr.First) {\
      if (AssType == XASS && (*SVar).First != (XCAT(SORT,_yptr))0) \
        XCAT(yFree_,SORT)((void **)SVar); \
      if ( (SExpr.IsAssigned || AssType == XASS2MAKE) && \
           SExpr.First != (XCAT(SORT,_yptr))0) { \
        TempExpr = SExpr.First; \
        TempSVar = XCAT(yGetAvail_,SORT)(); \
        (*SVar).First = TempSVar; \
        XCAT(yAssF_,INDEXSORT)(TempSVar->Index, TempExpr->Index, XASS); \
        XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
        \
        TempExpr = TempExpr->Suc; \
        while (TempExpr != (XCAT(SORT,_yptr))0) { \
          TempSVar->Suc = XCAT(yGetAvail_,SORT)(); \
          TempSVar = TempSVar->Suc; \
          XCAT(yAssF_,INDEXSORT)(TempSVar->Index, TempExpr->Index, XASS); \
          XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
          TempExpr = TempExpr->Suc; \
        } \
        (*SVar).Last = TempSVar; \
      } else { \
        (*SVar).First = SExpr.First; \
        (*SVar).Last = SExpr.Last; \
      } \
    } \
    XCAT(yAssF_,COMPONENTSORT)((*SVar).Data, SExpr.Data, XASS); \
    (*SVar).IsAssigned = (xbool)1; \
    \
    PrevExpr = (*SVar).First; \
    while ( PrevExpr != (XCAT(SORT,_yptr))0 && \
            XCAT(yEqF_,COMPONENTSORT)(PrevExpr->Data, (*SVar).Data) ) { \
      (*SVar).First = PrevExpr->Suc; \
      PrevExpr->Suc = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = PrevExpr; \
      PrevExpr = (*SVar).First; \
    } \
    if ( PrevExpr != (XCAT(SORT,_yptr))0 ) { \
      while ( PrevExpr->Suc != (XCAT(SORT,_yptr))0 ) { \
        TempExpr = PrevExpr->Suc; \
        if ( XCAT(yEqF_,COMPONENTSORT)(TempExpr->Data, (*SVar).Data) ) { \
          PrevExpr->Suc = TempExpr->Suc; \
          TempExpr->Suc = XCAT(yAvail_,SORT); \
          XCAT(yAvail_,SORT) = TempExpr; \
        } else { \
          PrevExpr = TempExpr; \
        } \
      } \
    } \
    (*SVar).Last = PrevExpr; \
  }
#else
#define Array_Assign(SORT, INDEXSORT, COMPONENTSORT) \
  static XCAT(SORT,_yptr)  XCAT(yAvail_,SORT) = (XCAT(SORT,_yptr))0; \
  void XCAT(yAss_,SORT) \
    XPP_PROTO( (SORT * SVar, SORT SExpr, int AssType) ) \
    XPP_NOPROTO( (SVar, SExpr, AssType)  SORT * SVar; SORT SExpr; int AssType; ) \
  { \
    XCAT(SORT,_yptr)  TempSVar, TempExpr, PrevExpr; \
    \
    if ((*SVar).First != SExpr.First) {\
      if (AssType == XASS && (*SVar).First != (XCAT(SORT,_yptr))0) \
        XCAT(yFree_,SORT)((void **)SVar); \
      if ( (SExpr.IsAssigned || AssType == XASS2MAKE) && \
           SExpr.First != (XCAT(SORT,_yptr))0) { \
        TempExpr = SExpr.First; \
        TempSVar = XCAT(yGetAvail_,SORT)(); \
        (*SVar).First = TempSVar; \
        XCAT(yAssF_,INDEXSORT)(TempSVar->Index, TempExpr->Index, XASS); \
        XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
        \
        TempExpr = TempExpr->Suc; \
        while (TempExpr != (XCAT(SORT,_yptr))0) { \
          TempSVar->Suc = XCAT(yGetAvail_,SORT)(); \
          TempSVar = TempSVar->Suc; \
          XCAT(yAssF_,INDEXSORT)(TempSVar->Index, TempExpr->Index, XASS); \
          XCAT(yAssF_,COMPONENTSORT)(TempSVar->Data, TempExpr->Data, XASS); \
          TempExpr = TempExpr->Suc; \
        } \
        (*SVar).Last = TempSVar; \
      } else { \
        (*SVar).First = SExpr.First; \
        (*SVar).Last = SExpr.Last; \
      } \
    } \
    XCAT(yAssF_,COMPONENTSORT)((*SVar).Data, SExpr.Data, XASS); \
    (*SVar).IsAssigned = (xbool)1; \
    \
    PrevExpr = (*SVar).First; \
    while ( PrevExpr != (XCAT(SORT,_yptr))0 && \
            XCAT(yEqF_,COMPONENTSORT)(PrevExpr->Data, (*SVar).Data) ) { \
      (*SVar).First = PrevExpr->Suc; \
      PrevExpr->Suc = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = PrevExpr; \
      PrevExpr = (*SVar).First; \
    } \
    if ( PrevExpr != (XCAT(SORT,_yptr))0 ) { \
      while ( PrevExpr->Suc != (XCAT(SORT,_yptr))0 ) { \
        TempExpr = PrevExpr->Suc; \
        if ( XCAT(yEqF_,COMPONENTSORT)(TempExpr->Data, (*SVar).Data) ) { \
          PrevExpr->Suc = TempExpr->Suc; \
          TempExpr->Suc = XCAT(yAvail_,SORT); \
          XCAT(yAvail_,SORT) = TempExpr; \
        } else { \
          PrevExpr = TempExpr; \
        } \
      } \
    } \
    (*SVar).Last = PrevExpr; \
  }
#endif

/*-- yEq_SORT (Array)----------------------------------------------*/
#define Array_Equal(SORT, INDEXSORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yEq_,SORT) \
    XPP_PROTO( (SORT Expr1, SORT Expr2) ) \
    XPP_NOPROTO( (Expr1, Expr2) SORT Expr1; SORT Expr2; ) \
  { \
    XCAT(SORT,_yptr)  Temp; \
    SDL_Boolean  Result; \
    \
    Result = XCAT(yEqF_,COMPONENTSORT)(Expr1.Data, Expr2.Data); \
    if (Result) { \
      Temp = Expr1.First; \
      while (Result && Temp != (XCAT(SORT,_yptr))0) { \
        if (XCAT(yNEqF_,COMPONENTSORT) \
             (Temp->Data, XCAT(yExtrInner_,SORT)(Expr2, Temp->Index)) ) \
          Result = SDL_False; \
        Temp = Temp->Suc; \
      } \
      Temp = Expr2.First; \
      while (Result && Temp != (XCAT(SORT,_yptr))0) { \
        if (XCAT(yNEqF_,COMPONENTSORT) \
             (Temp->Data, XCAT(yExtrInner_,SORT)(Expr1, Temp->Index)) ) \
          Result = SDL_False; \
        Temp = Temp->Suc; \
      } \
    } \
    \
    if (! Expr1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&Expr1); \
    if (! Expr2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&Expr2); \
    return Result; \
  }

/*-- yDef_SORT (Array)---------------------------------------------*/
#define Array_Default(SORT, INDEXSORT, COMPONENTSORT) \
  void XCAT(yDef_,SORT) \
    XPP_PROTO( (SORT *S) ) \
    XPP_NOPROTO( (S) SORT *S; ) \
  { \
    (*S).First = (XCAT(SORT,_yptr))0; \
    (*S).Last = (XCAT(SORT,_yptr))0; \
    XCAT(yDef_,COMPONENTSORT)(&(*S).Data); \
    (*S).IsAssigned = (xbool)1; \
  } 


/*-- yFree_SORT (Array)--------------------------------------------*/
#define Array_Free(SORT, INDEXSORT, FREEINDEX, COMPONENTSORT, FREECOMP) \
  void XCAT(yFree_,SORT) \
    XPP_PROTO( (void ** C) ) \
    XPP_NOPROTO( (C) void ** C; ) \
  { \
    XCAT(SORT,_yptr)  Temp, TempSuc; \
    if (FREECOMP != XXFREE || FREEINDEX != XXFREE) { \
      for (Temp =  (*(SORT *)C).First; \
           Temp != (XCAT(SORT,_yptr))0; \
           Temp =  TempSuc) { \
        if (FREECOMP != XXFREE)  FREECOMP((void **)&Temp->Data); \
        if (FREEINDEX != XXFREE) FREEINDEX((void **)&Temp->Index); \
        TempSuc =  Temp->Suc; \
        XFREE((void **)&Temp, sizeof(XCAT(SORT,_yrec))); \
      } \
      if (FREECOMP != XXFREE) \
        FREECOMP((void **)&(*(SORT *)C).Data); \
    } \
    else if ((*(SORT *)C).First != (XCAT(SORT,_yptr))0) { \
      (*(SORT *)C).Last->Suc = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = (*(SORT *)C).First; \
    } \
    (*(SORT *)C).First = (XCAT(SORT,_yptr))0; \
    (*(SORT *)C).Last = (XCAT(SORT,_yptr))0; \
    for (Temp = XCAT(yAvail_,SORT); \
         Temp; \
         Temp = XCAT(yAvail_,SORT)) { \
      XCAT(yAvail_,SORT)  = Temp->Suc; \
      if (FREECOMP != XXFREE)  FREECOMP((void **)&Temp->Data); \
      if (FREEINDEX != XXFREE) FREEINDEX((void **)&Temp->Index); \
      XFREE((void **)&Temp, sizeof(XCAT(SORT,_yrec))); \
    } \
  }


/*-- yWri_SORT (Array)---------------------------------------------*/
#define Array_Write(SORT, INDEXSORT, COMPONENTSORT) \
  char * XCAT(yWri_,SORT) \
    XPP_PROTO( (void * S) ) \
    XPP_NOPROTO( (S) void * S; ) \
  { \
    XCAT(SORT,_yptr) ArrPtr; \
  static  char       CTemp[MAX_WRI_LENGTH]; \
    \
    (void)strcpy(CTemp, "(: (others:"); \
    xSafeStrcat(CTemp, \
       xWriteSort((void *)(&(*(SORT *)S).Data), XCAT(ySrtN_,COMPONENTSORT))); \
    xSafeStrcat(CTemp, ")"); \
    for (ArrPtr =  (*(SORT *)S).First; \
         ArrPtr != (XCAT(SORT,_yptr))0; \
         ArrPtr =  ArrPtr->Suc) { \
      xSafeStrcat(CTemp, ", ("); \
      xSafeStrcat(CTemp, \
               xWriteSort((void *)(&ArrPtr->Index), XCAT(ySrtN_,INDEXSORT))); \
      xSafeStrcat(CTemp, ":"); \
      xSafeStrcat(CTemp, \
            xWriteSort((void *)(&ArrPtr->Data), XCAT(ySrtN_,COMPONENTSORT))); \
      xSafeStrcat(CTemp, ")"); \
    } \
    xSafeStrcat(CTemp, " :)"); \
    return CTemp; \
  }


/*-- yRead_SORT (Array)--------------------------------------------*/
#define Array_Read(SORT, INDEXSORT, COMPONENTSORT) \
  int XCAT(yRead_,SORT) \
    XPP_PROTO( (void * Result) ) \
    XPP_NOPROTO( (Result) void * Result; ) \
  { \
    INDEXSORT     Index; \
    COMPONENTSORT Item; \
    xxToken       Token; \
    char          strVar[256]; \
    xbool         IsOthers; \
    int           Res; \
    \
    XCAT(yDef_,SORT)((SORT *)Result); \
    Token = xScanToken(strVar); \
    if (XVALUE_SYNTAX && \
        Token != xxLParColon && Token != xxQuestionMark && Token != xxEoln) { \
      xPrintString("Array value should start with (:\n"); \
      return 0; \
    } \
    if (Token != xxLParColon) xUngetToken(Token, strVar); \
    \
    IsOthers = (xbool)1; \
    while (Token != xxRParColon) { \
      Token = xScanToken(strVar); \
      if (Token == xxLPar) Token = xScanToken(strVar); \
      if (! IsOthers) { \
        if (Token == xxEoln || Token == xxQuestionMark) { \
          (void)sprintf(strVar, " Index (%s) : ", XCAT(ySrtN_,INDEXSORT)->Name); \
          xPrintString(strVar); \
          Token = xScanToken(strVar); \
        } \
        if (Token == xxRParColon) break; \
        xUngetToken(Token, strVar); \
        (void)memset((void *)(&Index), 0, sizeof(INDEXSORT)); \
        if ( ! xReadSort((void *)&Index, XCAT(ySrtN_,INDEXSORT))) { \
          xPrintString("Illegal index value\n"); \
          return 0; \
        } \
        Token = xScanToken(strVar); \
      } else { \
        if (Token == xxEoln || Token == xxQuestionMark) { \
          xPrintString(" others"); \
        } else if (Token == xxId) { \
          if (xfEqualIdString(strVar, "others") != 2) { \
            xPrintString("others expected\n"); \
            return 0; \
          } \
          Token = xScanToken(strVar); \
        } else { \
          xPrintString("others expected\n"); \
          return 0; \
        } \
      } \
      if (Token != xxColon) xUngetToken(Token, strVar); \
      if (IsOthers) \
        (void)sprintf(strVar, " Others component (%s) : ", XCAT(ySrtN_,COMPONENTSORT)->Name); \
      else \
        (void)sprintf(strVar, " Component (%s) : ", XCAT(ySrtN_,COMPONENTSORT)->Name); \
      (void)memset((void *)(&Item), 0, sizeof(COMPONENTSORT)); \
      Res = xReadOneParameter(XCAT(ySrtN_,COMPONENTSORT), strVar, (void *)&Item); \
      if (Res == 0) { \
        xPrintString("Illegal component value\n"); \
        return 0; \
      } \
      if (IsOthers) \
        XCAT(yAss_,SORT)((SORT *)Result, XCAT(yMake_,SORT)(Item), XASS); \
      else \
        XCAT(yAssF_,COMPONENTSORT)( \
            (* XCAT(yAddr_,SORT)((SORT *)Result, Index)) , Item, XASS); \
      \
      Token = xScanToken(strVar); \
      if (Token == xxRPar) Token = xScanToken(strVar); \
      if (Token == xxComma) Token = xScanToken(strVar); \
      if (Token != xxRParColon) xUngetToken(Token, strVar); \
      IsOthers = (xbool)0; \
    } \
    (*(SORT *)Result).IsAssigned = (int)1; \
    return 1; \
  }


/****+***************************************************************
06   Generator Powerset  (limited and discrete component type)
********************************************************************/

/* typedef for Powerset type --------------------------------------*/

#define Powerset_Type(SORT, COMPONENTSORT, LOW, HIGH) \
  typedef struct { \
    unsigned long A[(HIGH-LOW)/32+1]; \
  } SORT;

  /* unsigned long is assumed to be 32 bits */


#define Powerset_prototypes(SORT, COMPONENTSORT, LOW, HIGH) \
  extern SORT XCAT(yEmpty_,SORT) XPP((void)); \
  extern SDL_Boolean XCAT(yIn_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SORT XCAT(yIncl_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SORT XCAT(yDel_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SDL_Boolean XCAT(yLT_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yGT_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yLE_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yGE_,SORT) XPP((SORT, SORT)); \
  extern SORT XCAT(yAnd_,SORT) XPP((SORT, SORT)); \
  extern SORT XCAT(yOr_,SORT) XPP((SORT, SORT)); \
  extern SDL_Integer XCAT(yLength_,SORT) XPP((SORT)); \
  extern COMPONENTSORT XCAT(yTake_,SORT) XPP((SORT)); \
  extern COMPONENTSORT XCAT(yTake2_,SORT) XPP((SORT, SDL_Integer));


/*-- yEmpty_SORT (Powerset)----------------------------------------*/
#define Powerset_Empty(SORT, COMPONENTSORT, LOW, HIGH) \
  SORT XCAT(yEmpty_,SORT) XPP((void)) \
  { \
    SORT PS; \
    int Item; \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) \
      PS.A[Item] = 0; \
    return PS; \
  }  


/*-- yIn_SORT (Powerset)-------------------------------------------*/
#define Powerset_In(SORT, COMPONENTSORT, LOW, HIGH) \
  SDL_Boolean XCAT(yIn_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
/*  Modified by GBU,000103  in WIP only unsigned [0..HIGH] is used */ \
/*    if (Item < LOW || Item > HIGH) return SDL_False;*/ \
/*    Item = Item - LOW;*/ \
    if (Item > HIGH) return SDL_False; \
    if ( ((unsigned long)1 << Item%32) & PS.A[Item/32] ) \
      return SDL_True; \
    return SDL_False; \
  }  


/*-- yIncl_SORT (Powerset)-----------------------------------------*/
#define Powerset_Incl(SORT, COMPONENTSORT, LOW, HIGH) \
  SORT XCAT(yIncl_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
/*  Modified by GBU,000103  in WIP only unsigned [0..HIGH] is used */ \
/*    if (Item < LOW || Item > HIGH) return PS;*/ \
/*    Item = Item - LOW;*/ \
    if (Item > HIGH) return PS; \
    PS.A[Item/32] = ((unsigned long)1 << Item%32) | PS.A[Item/32]; \
    return PS; \
  }  


/*-- yDel_SORT (Powerset)------------------------------------------*/
#define Powerset_Del(SORT, COMPONENTSORT, LOW, HIGH) \
  SORT XCAT(yDel_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
/*  Modified by GBU,000103  in WIP only unsigned [0..HIGH] is used */ \
/*    if (Item < LOW || Item > HIGH) return PS;*/ \
/*    Item = Item - LOW;*/ \
    if (Item > HIGH) return PS; \
    PS.A[Item/32] = ~((unsigned long)1 << Item%32) & PS.A[Item/32]; \
    return PS; \
  }  


/*-- yAnd_SORT (Powerset)------------------------------------------*/
#define Powerset_And(SORT, COMPONENTSORT, LOW, HIGH) \
  SORT XCAT(yAnd_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    SORT PS; \
    int Item; \
    \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) \
      PS.A[Item] = PS1.A[Item] & PS2.A[Item]; \
    return PS; \
  }  


/*-- yOr_SORT (Powerset)-------------------------------------------*/
#define Powerset_Or(SORT, COMPONENTSORT, LOW, HIGH) \
  SORT XCAT(yOr_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    SORT PS; \
    int Item; \
    \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) \
      PS.A[Item] = PS1.A[Item] | PS2.A[Item]; \
    return PS; \
  }  


/*-- yLE_SORT (Powerset)-------------------------------------------*/
#define Powerset_LE(SORT, COMPONENTSORT, LOW, HIGH) \
  SDL_Boolean XCAT(yLE_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    int Item; \
    \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) { \
      if ( (PS2.A[Item] | PS1.A[Item]) ^ PS2.A[Item] ) \
        return SDL_False; \
    } \
    return SDL_True; \
  }


/*-- yLT_SORT (Powerset)-------------------------------------------*/
#define Powerset_LT(SORT, COMPONENTSORT, LOW, HIGH) \
  SDL_Boolean XCAT(yLT_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    int Item; \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) { \
      if ( (PS2.A[Item] | PS1.A[Item]) ^ PS2.A[Item] ) \
        return SDL_False; \
    } \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) { \
      if ( PS2.A[Item] ^ PS1.A[Item] ) \
        return SDL_True; \
    } \
    return SDL_False; \
  }


/*-- yGE_SORT (Powerset)-------------------------------------------*/
#define Powerset_GE(SORT, COMPONENTSORT, LOW, HIGH) \
  SDL_Boolean XCAT(yGE_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    int Item; \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) { \
      if ( (PS1.A[Item] | PS2.A[Item]) ^ PS1.A[Item] ) \
        return SDL_False; \
    } \
    return SDL_True; \
  }


/*-- yGT_SORT (Powerset)-------------------------------------------*/
#define Powerset_GT(SORT, COMPONENTSORT, LOW, HIGH) \
  SDL_Boolean XCAT(yGT_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    int Item; \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) { \
      if ( (PS1.A[Item] | PS2.A[Item]) ^ PS1.A[Item] ) \
        return SDL_False; \
    } \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) { \
      if ( PS1.A[Item] ^ PS2.A[Item] ) \
        return SDL_True; \
    } \
    return SDL_False; \
  }


/*-- yLength_SORT (Powerset)---------------------------------------*/
#define Powerset_Length(SORT, COMPONENTSORT, LOW, HIGH) \
  SDL_Integer XCAT(yLength_,SORT) \
    XPP_PROTO( (SORT PS) ) \
    XPP_NOPROTO( (PS)  SORT PS; ) \
  { \
    int W, B; \
    int NoOfComp; \
    NoOfComp = 0; \
    for (W=0; W<=(HIGH-LOW)/32; W++) { \
      for (B=0; B<32; B++) { \
        if (((unsigned long)1 << B) & PS.A[W]) \
          NoOfComp++; \
      } \
    } \
    return NoOfComp; \
  }


/*-- yTake_SORT (Powerset)------------------------------------*/
#define Powerset_Take(SORT, COMPONENTSORT, LOW, HIGH) \
  COMPONENTSORT XCAT(yTake_,SORT) \
    XPP_PROTO( (SORT PS) ) \
    XPP_NOPROTO( (PS)  SORT PS; ) \
  { \
    COMPONENTSORT Result; \
    int W, B; \
    for (W=0; W<=(HIGH-LOW)/32; W++) { \
      for (B=0; B<32; B++) { \
        if (((unsigned long)1 << B) & PS.A[W]) \
          return (COMPONENTSORT)(LOW+W*32+B); \
      } \
    } \
    xReportStringError("Take in Powerset sort", "Empty powerset") \
    (void)memset((void *)(&Result), 0, sizeof(COMPONENTSORT)); \
    XCAT(yDef_,COMPONENTSORT)(&Result); \
    return Result; \
  }


/*-- yTake2_SORT (Powerset)------------------------------------*/
#define Powerset_Take2(SORT, COMPONENTSORT, LOW, HIGH) \
  COMPONENTSORT XCAT(yTake2_,SORT) \
    XPP_PROTO( (SORT PS, SDL_Integer Index) ) \
    XPP_NOPROTO( (PS, Index)  SORT PS; SDL_Integer Index; ) \
  { \
    COMPONENTSORT Result; \
    int W, B; \
    int NoOfComp; \
    NoOfComp = 0; \
    for (W=0; W<=(HIGH-LOW)/32; W++) { \
      for (B=0; B<32; B++) { \
        if (((unsigned long)1 << B) & PS.A[W]) { \
          if (++NoOfComp == Index) \
            return (COMPONENTSORT)(LOW+W*32+B); \
        } \
      } \
    } \
    xReportStringError("Take in Powerset sort", "Index out of range") \
    (void)memset((void *)(&Result), 0, sizeof(COMPONENTSORT)); \
    XCAT(yDef_,COMPONENTSORT)(&Result); \
    return Result; \
  }


/*-- yEq_SORT (Powerset)-------------------------------------------*/
#define Powerset_Equal(SORT, COMPONENTSORT, LOW, HIGH) \
  SDL_Boolean XCAT(yEq_,SORT) \
    XPP_PROTO( (SORT Expr1, SORT Expr2) ) \
    XPP_NOPROTO( (Expr1, Expr2) SORT Expr1; SORT Expr2; ) \
  { \
    int Item; \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) { \
      if ( Expr1.A[Item] ^ Expr2.A[Item] ) \
        return SDL_False; \
    } \
    return SDL_True; \
  }


/*-- yDef_SORT (Powerset)------------------------------------------*/
#define Powerset_Default(SORT, COMPONENTSORT, LOW, HIGH) \
  void XCAT(yDef_,SORT) \
    XPP_PROTO( (SORT *S) ) \
    XPP_NOPROTO( (S) SORT *S; ) \
  { \
    int Item; \
    for (Item=0; Item<=(HIGH-LOW)/32; Item++) \
      (*S).A[Item] = (unsigned long)0; \
  }



/****+***************************************************************
07   Generator Powerset  (general component type)
********************************************************************/

/* typedef for GPowerset type -------------------------------------*/

#define GPowerset_Type(SORT, COMPONENTSORT) \
   typedef struct XCAT(SORT,_ystruct)  * XCAT(SORT,_yptr); \
   typedef struct XCAT(SORT,_ystruct) { \
     XCAT(SORT,_yptr)   Suc; \
     COMPONENTSORT      Data; \
   } XCAT(SORT,_yrec); \
   typedef struct { \
     XCAT(SORT,_yptr) First; \
     XCAT(SORT,_yptr) Last; \
     int              Length; \
     xbool            IsAssigned; \
   } SORT;


#define GPowerset_prototypes(SORT, COMPONENTSORT) \
  extern SORT XCAT(yEmpty_,SORT) XPP((void)); \
  extern SDL_Boolean XCAT(yIn_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SORT XCAT(yIncl_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SORT XCAT(yDel_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SDL_Boolean XCAT(yLT_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yGT_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yLE_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yGE_,SORT) XPP((SORT, SORT)); \
  extern SORT XCAT(yAnd_,SORT) XPP((SORT, SORT)); \
  extern SORT XCAT(yOr_,SORT) XPP((SORT, SORT)); \
  extern SDL_Integer XCAT(yLength_,SORT) XPP((SORT)); \
  extern COMPONENTSORT XCAT(yTake_,SORT) XPP((SORT)); \
  extern COMPONENTSORT XCAT(yTake2_,SORT) XPP((SORT, SDL_Integer)); \
  \
  extern SORT XCAT(yCopyList_,SORT) XPP((SORT PS)); \
  extern SDL_Boolean XCAT(yInInner_,SORT) \
            XPP((COMPONENTSORT Item, SORT PS)); \
  extern XCAT(SORT,_yptr) XCAT(yGetAvail_,SORT) XPP((void)); \
  \
  extern char * XCAT(yWri_,SORT) XPP((void * PS)); \
  extern int XCAT(yRead_,SORT) XPP((void * Result));


/*-- yEmpty_SORT (GPowerset)---------------------------------------*/
#define GPowerset_Empty(SORT, COMPONENTSORT) \
  XCAT(SORT,_yptr) XCAT(yGetAvail_,SORT) XPP((void)) \
  { \
    XCAT(SORT,_yptr) Item; \
    if (XCAT(yAvail_,SORT) != (XCAT(SORT,_yptr))0) { \
      Item = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = XCAT(yAvail_,SORT)->Suc; \
      Item->Suc = (XCAT(SORT,_yptr))0; \
    } else { \
      Item = (XCAT(SORT,_yptr))xAlloc((unsigned)sizeof(XCAT(SORT,_yrec))); \
    } \
    return Item; \
  } \
  \
  SORT XCAT(yEmpty_,SORT) XPP((void)) \
  { \
    SORT Result; \
    Result.First = (XCAT(SORT,_yptr))0; \
    Result.Last = (XCAT(SORT,_yptr))0; \
    Result.Length = 0; \
    Result.IsAssigned = (xbool)0; \
    return Result; \
  }


/*-- yIn_SORT (GPowerset)------------------------------------------*/
#define GPowerset_In(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yInInner_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    SDL_Boolean Result; \
    Result = SDL_False; \
    for (Temp=PS.First; \
         Temp!=(XCAT(SORT,_yptr))0 && !Result; \
         Temp=Temp->Suc) \
      if ( XCAT(yEqF_,COMPONENTSORT)(Temp->Data, Item) ) \
        Result = SDL_True; \
    return Result; \
  } \
  \
  SDL_Boolean XCAT(yIn_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
    SDL_Boolean Result; \
    Result = XCAT(yInInner_,SORT)(Item, PS); \
    if (! PS.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS); \
    return Result; \
  }


/*-- yIncl_SORT (GPowerset)----------------------------------------*/
#define GPowerset_Incl(SORT, COMPONENTSORT) \
  SORT XCAT(yCopyList_,SORT) \
    XPP_PROTO( (SORT PS) ) \
    XPP_NOPROTO( (PS)  SORT PS; ) \
  { \
    XCAT(SORT,_yptr) TempPS, TempRes; \
    SORT Result; \
    Result = PS; \
    Result.IsAssigned = (xbool)0; \
    if (PS.First != (XCAT(SORT,_yptr))0 ) { \
      TempPS = PS.First; \
      TempRes = XCAT(yGetAvail_,SORT)(); \
      XCAT(yAssF_,COMPONENTSORT)(TempRes->Data, TempPS->Data, XASS); \
      Result.First = TempRes; \
      while (TempPS->Suc != (XCAT(SORT,_yptr))0) { \
        TempPS = TempPS->Suc; \
        TempRes->Suc = XCAT(yGetAvail_,SORT)(); \
        TempRes = TempRes->Suc; \
        XCAT(yAssF_,COMPONENTSORT)(TempRes->Data, TempPS->Data, XASS); \
      } \
      Result.Last = TempRes; \
    } \
    return Result; \
  } \
  \
  SORT XCAT(yIncl_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
    SORT Result; \
    if (PS.IsAssigned) \
      Result = XCAT(yCopyList_,SORT)(PS); \
    else \
      Result = PS; \
    if (! XCAT(yInInner_,SORT)(Item, Result) ) { \
      if (Result.Last == (XCAT(SORT,_yptr))0) { \
        Result.Last = XCAT(yGetAvail_,SORT)(); \
        Result.First = Result.Last; \
      } else { \
        Result.Last->Suc = XCAT(yGetAvail_,SORT)(); \
        Result.Last = Result.Last->Suc; \
      } \
      XCAT(yAssF_,COMPONENTSORT)(Result.Last->Data, Item, XASS); \
      Result.Length++; \
    } \
    return Result; \
  }


/*-- yDel_SORT (GPowerset)-----------------------------------------*/
#define GPowerset_Del(SORT, COMPONENTSORT) \
  SORT XCAT(yDel_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
    SORT Result; \
    XCAT(SORT,_yptr) Temp, Prev; \
    if (PS.IsAssigned) \
      Result = XCAT(yCopyList_,SORT)(PS); \
    else \
      Result = PS; \
    if ( Result.First != (XCAT(SORT,_yptr))0 ) { \
      Prev = Result.First; \
      if ( XCAT(yEqF_,COMPONENTSORT)(Prev->Data, Item) ) { \
        Result.First = Prev->Suc; \
        if (Result.Last == Prev) Result.Last = (XCAT(SORT,_yptr))0; \
        Prev->Suc = XCAT(yAvail_,SORT); \
        XCAT(yAvail_,SORT) = Prev; \
        Result.Length--; \
        return Result; \
      } \
      for (Temp=Prev->Suc; \
           Temp!=(XCAT(SORT,_yptr))0; \
           Temp=Temp->Suc) { \
        if ( XCAT(yEqF_,COMPONENTSORT)(Temp->Data, Item) ) {\
          Prev->Suc = Temp->Suc; \
          if (Result.Last == Temp) Result.Last = Prev; \
          Temp->Suc = XCAT(yAvail_,SORT); \
          XCAT(yAvail_,SORT) = Temp; \
          Result.Length--; \
          return Result; \
        } \
        Prev = Temp; \
      } \
    } \
    return Result; \
  }


/*-- yAnd_SORT (GPowerset)-----------------------------------------*/
#define GPowerset_And(SORT, COMPONENTSORT) \
  SORT XCAT(yAnd_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    SORT Result; \
    XCAT(SORT,_yptr) Temp; \
    Result = XCAT(yEmpty_,SORT)(); \
    for (Temp=PS1.First; Temp!=(XCAT(SORT,_yptr))0; Temp=Temp->Suc) \
      if ( XCAT(yInInner_,SORT)(Temp->Data, PS2) ) { \
        if (Result.First == (XCAT(SORT,_yptr))0) { \
          Result.First = XCAT(yGetAvail_,SORT)(); \
          Result.Last = Result.First; \
        } else { \
          Result.Last->Suc = XCAT(yGetAvail_,SORT)(); \
          Result.Last = Result.Last->Suc; \
        } \
        XCAT(yAssF_,COMPONENTSORT)(Result.Last->Data, Temp->Data, XASS); \
        Result.Length++; \
      } \
    if (! PS1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS1); \
    if (! PS2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS2); \
    return Result; \
  }


/*-- yOr_SORT (GPowerset)------------------------------------------*/
#define GPowerset_Or(SORT, COMPONENTSORT) \
  SORT XCAT(yOr_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    SORT Result; \
    XCAT(SORT,_yptr) Temp; \
    if (PS2.IsAssigned) \
      Result = XCAT(yCopyList_,SORT)(PS2); \
    else \
      Result = PS2; \
    for (Temp=PS1.First; Temp!=(XCAT(SORT,_yptr))0; Temp=Temp->Suc) \
      if ( ! XCAT(yInInner_,SORT)(Temp->Data, Result) ) { \
        if (Result.First == (XCAT(SORT,_yptr))0) { \
          Result.First = XCAT(yGetAvail_,SORT)(); \
          Result.Last = Result.First; \
        } else { \
          Result.Last->Suc = XCAT(yGetAvail_,SORT)(); \
          Result.Last = Result.Last->Suc; \
        } \
        XCAT(yAssF_,COMPONENTSORT)(Result.Last->Data, Temp->Data, XASS); \
        Result.Length++; \
      } \
    if (! PS1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS1); \
    return Result; \
  }


/*-- yLE_SORT (GPowerset)------------------------------------------*/
#define GPowerset_LE(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yLE_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    SDL_Boolean Result; \
    Result = PS1.Length <= PS2.Length; \
    for (Temp=PS1.First; \
         Temp!=(XCAT(SORT,_yptr))0 && Result; \
         Temp=Temp->Suc) \
      if ( ! XCAT(yInInner_,SORT)(Temp->Data, PS2) ) \
        Result = SDL_False; \
    if (! PS1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS1); \
    if (! PS2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS2); \
    return Result; \
  }


/*-- yLT_SORT (GPowerset)------------------------------------------*/
#define GPowerset_LT(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yLT_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    SDL_Boolean Result; \
    Result = PS1.Length < PS2.Length; \
    for (Temp=PS1.First; \
         Temp!=(XCAT(SORT,_yptr))0 && Result; \
         Temp=Temp->Suc) \
      if ( ! XCAT(yInInner_,SORT)(Temp->Data, PS2) ) \
        Result = SDL_False; \
    if (! PS1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS1); \
    if (! PS2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS2); \
    return Result; \
  }


/*-- yGE_SORT (GPowerset)------------------------------------------*/
#define GPowerset_GE(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yGE_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    return XCAT(yLE_,SORT)(PS2, PS1); \
  }


/*-- yGT_SORT (GPowerset)------------------------------------------*/
#define GPowerset_GT(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yGT_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    return XCAT(yLT_,SORT)(PS2, PS1); \
  }


/*-- yAss_SORT (GPowerset)-----------------------------------------*/
#define GPowerset_Assign(SORT, COMPONENTSORT) \
  static XCAT(SORT,_yptr)  XCAT(yAvail_,SORT) = (XCAT(SORT,_yptr))0; \
  void XCAT(yAss_,SORT) \
    XPP_PROTO( (SORT * SVar, SORT SExpr, int AssType) ) \
    XPP_NOPROTO( (SVar, SExpr, AssType)  SORT * SVar; SORT SExpr; int AssType; ) \
  { \
    if ((*SVar).First != (XCAT(SORT,_yptr))0 && (*SVar).First == SExpr.First) { \
      (*SVar).IsAssigned = (xbool)1; \
      return; \
    } \
    if (AssType == XASS && (*SVar).First != (XCAT(SORT,_yptr))0) { \
      XCAT(yFree_,SORT)((void **)SVar); \
    } \
    if ( (SExpr.IsAssigned || AssType == XASS2MAKE) && \
         SExpr.First != (XCAT(SORT,_yptr))0) { \
      *SVar = XCAT(yCopyList_,SORT)(SExpr); \
    } else { \
      (*SVar).First = SExpr.First; \
      (*SVar).Last = SExpr.Last; \
      (*SVar).Length = SExpr.Length; \
    } \
    (*SVar).IsAssigned = (xbool)1; \
  }


/*-- yEq_SORT (GPowerset)------------------------------------------*/
#define GPowerset_Equal(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yEq_,SORT) \
    XPP_PROTO( (SORT Expr1, SORT Expr2) ) \
    XPP_NOPROTO( (Expr1, Expr2) SORT Expr1; SORT Expr2; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    SDL_Boolean Result; \
    Result = Expr1.Length == Expr2.Length; \
    for (Temp=Expr1.First; \
         Temp!=(XCAT(SORT,_yptr))0 && Result; \
         Temp=Temp->Suc) \
      if ( ! XCAT(yInInner_,SORT)(Temp->Data, Expr2) ) \
        Result = SDL_False; \
    if (! Expr1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&Expr1); \
    if (! Expr2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&Expr2); \
    return Result; \
  }


/*-- yDef_SORT (GPowerset)-----------------------------------------*/
#define GPowerset_Default(SORT, COMPONENTSORT) \
  void XCAT(yDef_,SORT) \
    XPP_PROTO( (SORT *S) ) \
    XPP_NOPROTO( (S) SORT *S; ) \
  { \
    (*S).First = (XCAT(SORT,_yptr))0; \
    (*S).Last = (XCAT(SORT,_yptr))0; \
    (*S).Length = 0; \
    (*S).IsAssigned = (xbool)1; \
  }


/*-- yFree_SORT (GPowerset)---------------------------------------*/
#define GPowerset_Free(SORT, COMPONENTSORT, FREEFUNC) \
  void XCAT(yFree_,SORT) \
    XPP_PROTO( (void ** C) ) \
    XPP_NOPROTO( (C) void ** C; ) \
  { \
    XCAT(SORT,_yptr)  Temp, TempSuc; \
    if (FREEFUNC != XXFREE) { \
      for (Temp =  (*(SORT *)C).First; \
           Temp != (XCAT(SORT,_yptr))0; \
           Temp =  TempSuc) { \
        FREEFUNC((void **)&Temp->Data); \
        TempSuc =  Temp->Suc; \
        XFREE((void **)&Temp, sizeof(XCAT(SORT,_yrec))); \
      } \
    } \
    else if ((*(SORT *)C).First != (XCAT(SORT,_yptr))0) { \
      (*(SORT *)C).Last->Suc = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = (*(SORT *)C).First; \
    } \
    (*(SORT *)C).First = (XCAT(SORT,_yptr))0; \
    (*(SORT *)C).Last = (XCAT(SORT,_yptr))0; \
    (*(SORT *)C).Length = 0; \
    (*(SORT *)C).IsAssigned = (xbool)0; \
    for (Temp = XCAT(yAvail_,SORT); \
         Temp; \
         Temp = XCAT(yAvail_,SORT)) { \
      XCAT(yAvail_,SORT)  = Temp->Suc; \
      if (FREEFUNC != XXFREE)  FREEFUNC((void **)&Temp->Data); \
      XFREE((void **)&Temp, sizeof(XCAT(SORT,_yrec))); \
    } \
  }


/*-- yLength_SORT (GPowerset)---------------------------------------*/
#define GPowerset_Length(SORT, COMPONENTSORT) \
  SDL_Integer XCAT(yLength_,SORT) \
    XPP_PROTO( (SORT PS) ) \
    XPP_NOPROTO( (PS)  SORT PS; ) \
  { \
    return PS.Length; \
  }


/*-- yTake_SORT (GPowerset)------------------------------------*/
#define GPowerset_Take(SORT, COMPONENTSORT) \
  COMPONENTSORT XCAT(yTake_,SORT) \
    XPP_PROTO( (SORT PS) ) \
    XPP_NOPROTO( (PS)  SORT PS; ) \
  { \
    COMPONENTSORT Result; \
    if (PS.First != (XCAT(SORT,_yptr))0) \
      return PS.First->Data; \
    xReportStringError("Take in Powerset sort", "Empty powerset") \
    (void)memset((void *)(&Result), 0, sizeof(COMPONENTSORT)); \
    XCAT(yDef_,COMPONENTSORT)(&Result); \
    return Result; \
  }


/*-- yTake2_SORT (GPowerset)------------------------------------*/
#define GPowerset_Take2(SORT, COMPONENTSORT) \
  COMPONENTSORT XCAT(yTake2_,SORT) \
    XPP_PROTO( (SORT PS, SDL_Integer Index) ) \
    XPP_NOPROTO( (PS, Index)  SORT PS; SDL_Integer Index; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    int Number = 0; \
    COMPONENTSORT Result; \
    for (Temp=PS.First; \
         Temp!=(XCAT(SORT,_yptr))0 && ++Number<Index; \
         Temp=Temp->Suc) ; \
    if (Number == Index && Temp != (XCAT(SORT,_yptr))0) \
      return Temp->Data; \
    xReportStringError("Take in Powerset sort", "Index out of range") \
    (void)memset((void *)(&Result), 0, sizeof(COMPONENTSORT)); \
    XCAT(yDef_,COMPONENTSORT)(&Result); \
    return Result; \
  }


/*-- yWri_SORT (GPowerset)----------------------------------------*/
#define GPowerset_Write(SORT, COMPONENTSORT) \
  char * XCAT(yWri_,SORT) \
    XPP_PROTO( (void * PS) ) \
    XPP_NOPROTO( (PS) void * PS; ) \
  { \
    XCAT(SORT,_yptr) Temp, TempLast; \
    static  char     CTemp[MAX_WRI_LENGTH]; \
    \
    (void)strcpy(CTemp, "[ "); \
    TempLast = (*(SORT *)PS).Last; \
    for (Temp = (*(SORT *)PS).First; \
         Temp != (XCAT(SORT,_yptr))0; \
         Temp = Temp->Suc) { \
      xSafeStrcat(CTemp, \
             xWriteSort((void *)(&Temp->Data), XCAT(ySrtN_,COMPONENTSORT))); \
      if (strlen(CTemp) == MAX_WRI_LENGTH - 1) \
        return CTemp; \
      if (Temp != TempLast) \
        xSafeStrcat(CTemp, ", "); \
    } \
    xSafeStrcat(CTemp, " ]"); \
    return CTemp; \
  }


/*-- yRead_SORT (GPowerset)---------------------------------------*/
#define GPowerset_Read(SORT, COMPONENTSORT) \
  int XCAT(yRead_,SORT) \
    XPP_PROTO( (void * Result) ) \
    XPP_NOPROTO( (Result) void * Result; ) \
  { \
    COMPONENTSORT Item; \
    xxToken       Token; \
    char          strVar[256]; \
    int           Res; \
    \
    (*(SORT *)Result).First = (XCAT(SORT,_yptr))0; \
    (*(SORT *)Result).Last = (XCAT(SORT,_yptr))0; \
    (*(SORT *)Result).Length = 0; \
    (*(SORT *)Result).IsAssigned = (int)0; \
    Token = xScanToken(strVar); \
    if (XVALUE_SYNTAX && \
        Token != xxLBracket && Token != xxQuestionMark && Token != xxEoln) { \
      xPrintString("Powerset value should start with [\n"); \
      return 0; \
    } \
    if (Token != xxLBracket) xUngetToken(Token, strVar); \
    \
    while (Token != xxRBracket) { \
      (void)sprintf(strVar, " (%s) : ", XCAT(ySrtN_,COMPONENTSORT)->Name); \
      (void)memset((void *)(&Item), 0, sizeof(COMPONENTSORT)); \
      Res = xReadOneParameter(XCAT(ySrtN_,COMPONENTSORT), strVar, (void *)&Item); \
      if (Res == 0) return 0; \
      if (Res == 2) { \
        Token = xScanToken(strVar); \
        if (Token == xxRBracket) break; \
        return 0; \
      } \
      *(SORT *)Result = XCAT(yIncl_,SORT)(Item, *(SORT *)Result); \
      Token = xScanToken(strVar); \
      if (Token != xxRBracket) xUngetToken(Token, strVar); \
    } \
    (*(SORT *)Result).IsAssigned = (int)1; \
    return 1; \
  }


/****+***************************************************************
08   Generator Bag
********************************************************************/

/* typedef for Bag type -------------------------------------------*/

#if defined(ALPHACC) && defined(XVALIDATOR_LIB)
#define Bag_Type(SORT, COMPONENTSORT) \
   typedef struct XCAT(SORT,_ystruct)  * XCAT(SORT,_yptr); \
   typedef struct XCAT(SORT,_ystruct) { \
     XCAT(SORT,_yptr)   Suc; \
     long               NoOfItems; \
     COMPONENTSORT      Data; \
   } XCAT(SORT,_yrec); \
   typedef struct { \
     XCAT(SORT,_yptr) First; \
     XCAT(SORT,_yptr) Last; \
     int              Length; \
     xbool            IsAssigned; \
   } SORT;
#else
#define Bag_Type(SORT, COMPONENTSORT) \
   typedef struct XCAT(SORT,_ystruct)  * XCAT(SORT,_yptr); \
   typedef struct XCAT(SORT,_ystruct) { \
     XCAT(SORT,_yptr)   Suc; \
     int                NoOfItems; \
     COMPONENTSORT      Data; \
   } XCAT(SORT,_yrec); \
   typedef struct { \
     XCAT(SORT,_yptr) First; \
     XCAT(SORT,_yptr) Last; \
     int              Length; \
     xbool            IsAssigned; \
   } SORT;
#endif


#define Bag_prototypes(SORT, COMPONENTSORT) \
  extern SORT XCAT(yEmpty_,SORT) XPP((void)); \
  extern SORT XCAT(yMakebag_,SORT) XPP((COMPONENTSORT)); \
  extern SDL_Boolean XCAT(yIn_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SORT XCAT(yIncl_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SORT XCAT(yDel_,SORT) XPP((COMPONENTSORT, SORT)); \
  extern SDL_Boolean XCAT(yLT_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yGT_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yLE_,SORT) XPP((SORT, SORT)); \
  extern SDL_Boolean XCAT(yGE_,SORT) XPP((SORT, SORT)); \
  extern SORT XCAT(yAnd_,SORT) XPP((SORT, SORT)); \
  extern SORT XCAT(yOr_,SORT) XPP((SORT, SORT)); \
  extern SDL_Integer XCAT(yLength_,SORT) XPP((SORT)); \
  extern COMPONENTSORT XCAT(yTake_,SORT) XPP((SORT)); \
  extern COMPONENTSORT XCAT(yTake2_,SORT) XPP((SORT, SDL_Integer)); \
  \
  extern SORT XCAT(yCopyList_,SORT) XPP((SORT PS)); \
  extern XCAT(SORT,_yptr) XCAT(yInInner_,SORT) \
            XPP((COMPONENTSORT Item, SORT PS)); \
  extern XCAT(SORT,_yptr) XCAT(yGetAvail_,SORT) XPP((void)); \
  \
  extern char * XCAT(yWri_,SORT) XPP((void * PS)); \
  extern int XCAT(yRead_,SORT) XPP((void * Result));


/*-- yEmpty_SORT (Bag)---------------------------------------------*/
#define Bag_Empty(SORT, COMPONENTSORT) \
  XCAT(SORT,_yptr) XCAT(yGetAvail_,SORT) XPP((void)) \
  { \
    XCAT(SORT,_yptr) Item; \
    if (XCAT(yAvail_,SORT) != (XCAT(SORT,_yptr))0) { \
      Item = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = XCAT(yAvail_,SORT)->Suc; \
      Item->Suc = (XCAT(SORT,_yptr))0; \
    } else { \
      Item = (XCAT(SORT,_yptr))xAlloc((unsigned)sizeof(XCAT(SORT,_yrec))); \
    } \
    Item->NoOfItems = 1; \
    return Item; \
  } \
  \
  SORT XCAT(yEmpty_,SORT) XPP((void)) \
  { \
    SORT Result; \
    Result.First = (XCAT(SORT,_yptr))0; \
    Result.Last = (XCAT(SORT,_yptr))0; \
    Result.Length = 0; \
    Result.IsAssigned = (xbool)0; \
    return Result; \
  }


/*-- yIn_SORT (Bag)------------------------------------------------*/
#define Bag_In(SORT, COMPONENTSORT) \
  XCAT(SORT,_yptr) XCAT(yInInner_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    for (Temp=PS.First; \
         Temp!=(XCAT(SORT,_yptr))0 && \
                XCAT(yNEqF_,COMPONENTSORT)(Temp->Data, Item); \
         Temp=Temp->Suc) ; \
    return Temp; \
  } \
  \
  SDL_Boolean XCAT(yIn_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
    SDL_Boolean Result; \
    Result = XCAT(yInInner_,SORT)(Item, PS) != 0; \
    if (! PS.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS); \
    return Result; \
  }


/*-- yMakebag_SORT (Bag)---------------------------------------------*/
#define Bag_Makebag(SORT, COMPONENTSORT) \
  SORT XCAT(yMakebag_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item) ) \
    XPP_NOPROTO( (Item)  COMPONENTSORT Item; ) \
  { \
    SORT Result; \
    Result.First = XCAT(yGetAvail_,SORT)(); \
    Result.Last = Result.First; \
    XCAT(yAssF_,COMPONENTSORT)(Result.First->Data, Item, XASS); \
    Result.Length = 1; \
    Result.IsAssigned = (xbool)0; \
    return Result; \
  }


/*-- yIncl_SORT (Bag)----------------------------------------------*/
#define Bag_Incl(SORT, COMPONENTSORT) \
  SORT XCAT(yCopyList_,SORT) \
    XPP_PROTO( (SORT PS) ) \
    XPP_NOPROTO( (PS)  SORT PS; ) \
  { \
    XCAT(SORT,_yptr) TempPS, TempRes; \
    SORT Result; \
    Result = PS; \
    Result.IsAssigned = (xbool)0; \
    if (PS.First != (XCAT(SORT,_yptr))0 ) { \
      TempPS = PS.First; \
      TempRes = XCAT(yGetAvail_,SORT)(); \
      XCAT(yAssF_,COMPONENTSORT)(TempRes->Data, TempPS->Data, XASS); \
      TempRes->NoOfItems = TempPS->NoOfItems; \
      Result.First = TempRes; \
      while (TempPS->Suc != (XCAT(SORT,_yptr))0) { \
        TempPS = TempPS->Suc; \
        TempRes->Suc = XCAT(yGetAvail_,SORT)(); \
        TempRes = TempRes->Suc; \
        XCAT(yAssF_,COMPONENTSORT)(TempRes->Data, TempPS->Data, XASS); \
        TempRes->NoOfItems = TempPS->NoOfItems; \
      } \
      Result.Last = TempRes; \
    } \
    return Result; \
  } \
  \
  SORT XCAT(yIncl_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
    SORT Result; \
    XCAT(SORT,_yptr) P; \
    if (PS.IsAssigned) \
      Result = XCAT(yCopyList_,SORT)(PS); \
    else \
      Result = PS; \
    P = XCAT(yInInner_,SORT)(Item, Result); \
    if (P == 0) { \
      if (Result.Last == (XCAT(SORT,_yptr))0) { \
        Result.Last = XCAT(yGetAvail_,SORT)(); \
        Result.First = Result.Last; \
      } else { \
        Result.Last->Suc = XCAT(yGetAvail_,SORT)(); \
        Result.Last = Result.Last->Suc; \
      } \
      XCAT(yAssF_,COMPONENTSORT)(Result.Last->Data, Item, XASS); \
    } else { \
      P->NoOfItems++; \
    } \
    Result.Length++; \
    return Result; \
  }


/*-- yDel_SORT (Bag)-----------------------------------------------*/
#define Bag_Del(SORT, COMPONENTSORT) \
  SORT XCAT(yDel_,SORT) \
    XPP_PROTO( (COMPONENTSORT Item, SORT PS) ) \
    XPP_NOPROTO( (Item, PS)  COMPONENTSORT Item; SORT PS; ) \
  { \
    SORT Result; \
    XCAT(SORT,_yptr) Temp, Prev; \
    if (PS.IsAssigned) \
      Result = XCAT(yCopyList_,SORT)(PS); \
    else \
      Result = PS; \
    if ( Result.First != (XCAT(SORT,_yptr))0 ) { \
      Prev = Result.First; \
      if ( XCAT(yEqF_,COMPONENTSORT)(Prev->Data, Item) ) { \
        Prev->NoOfItems--; \
        if (Prev->NoOfItems < 1) { \
          Result.First = Prev->Suc; \
          if (Result.Last == Prev) Result.Last = (XCAT(SORT,_yptr))0; \
          Prev->Suc = XCAT(yAvail_,SORT); \
          XCAT(yAvail_,SORT) = Prev; \
	} \
        Result.Length--; \
        return Result; \
      } \
      for (Temp=Prev->Suc; \
           Temp!=(XCAT(SORT,_yptr))0; \
           Temp=Temp->Suc) { \
        if ( XCAT(yEqF_,COMPONENTSORT)(Temp->Data, Item) ) { \
          Temp->NoOfItems--; \
          if (Temp->NoOfItems < 1) { \
            Prev->Suc = Temp->Suc; \
            if (Result.Last == Temp) Result.Last = Prev; \
            Temp->Suc = XCAT(yAvail_,SORT); \
            XCAT(yAvail_,SORT) = Temp; \
	  } \
          Result.Length--; \
          return Result; \
        } \
        Prev = Temp; \
      } \
    } \
    return Result; \
  }


/*-- yAnd_SORT (Bag)-----------------------------------------------*/
#define Bag_And(SORT, COMPONENTSORT) \
  SORT XCAT(yAnd_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    SORT Result; \
    XCAT(SORT,_yptr) Temp, Temp2; \
    int NoOfItems; \
    Result = XCAT(yEmpty_,SORT)(); \
    for (Temp=PS1.First; Temp!=(XCAT(SORT,_yptr))0; Temp=Temp->Suc) { \
      Temp2 = XCAT(yInInner_,SORT)(Temp->Data, PS2); \
      if (Temp2 != 0 ) { \
        if (Result.First == (XCAT(SORT,_yptr))0) { \
          Result.First = XCAT(yGetAvail_,SORT)(); \
          Result.Last = Result.First; \
        } else { \
          Result.Last->Suc = XCAT(yGetAvail_,SORT)(); \
          Result.Last = Result.Last->Suc; \
        } \
        XCAT(yAssF_,COMPONENTSORT)(Result.Last->Data, Temp->Data, XASS); \
        NoOfItems = Temp->NoOfItems; \
        if (Temp->NoOfItems > Temp2->NoOfItems) NoOfItems = Temp2->NoOfItems; \
        Result.Last->NoOfItems = NoOfItems; \
        Result.Length = Result.Length+NoOfItems; \
      } \
    } \
    if (! PS1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS1); \
    if (! PS2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS2); \
    return Result; \
  }


/*-- yOr_SORT (Bag)------------------------------------------------*/
#define Bag_Or(SORT, COMPONENTSORT) \
  SORT XCAT(yOr_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    SORT Result; \
    XCAT(SORT,_yptr) Temp, Temp2; \
    if (PS2.IsAssigned) \
      Result = XCAT(yCopyList_,SORT)(PS2); \
    else \
      Result = PS2; \
    for (Temp=PS1.First; Temp!=(XCAT(SORT,_yptr))0; Temp=Temp->Suc) { \
      Temp2 = XCAT(yInInner_,SORT)(Temp->Data, Result); \
      if (Temp2 == 0) { \
        if (Result.First == (XCAT(SORT,_yptr))0) { \
          Result.First = XCAT(yGetAvail_,SORT)(); \
          Result.Last = Result.First; \
        } else { \
          Result.Last->Suc = XCAT(yGetAvail_,SORT)(); \
          Result.Last = Result.Last->Suc; \
        } \
        XCAT(yAssF_,COMPONENTSORT)(Result.Last->Data, Temp->Data, XASS); \
        Result.Last->NoOfItems = Temp->NoOfItems; \
        Result.Length = Result.Length+Temp->NoOfItems; \
      } else { \
        Result.Length = Result.Length+Temp->NoOfItems; \
        Temp2->NoOfItems = Temp2->NoOfItems+Temp->NoOfItems; \
      } \
    } \
    if (! PS1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS1); \
    return Result; \
  }


/*-- yLE_SORT (Bag)------------------------------------------------*/
#define Bag_LE(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yLE_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    XCAT(SORT,_yptr) Temp, Temp2; \
    SDL_Boolean Result; \
    Result = PS1.Length <= PS2.Length; \
    for (Temp=PS1.First; \
         Temp!=(XCAT(SORT,_yptr))0 && Result; \
         Temp=Temp->Suc) { \
      Temp2 = XCAT(yInInner_,SORT)(Temp->Data, PS2); \
      if (Temp2 == 0 || Temp->NoOfItems > Temp2->NoOfItems) \
        Result = SDL_False; \
    } \
    if (! PS1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS1); \
    if (! PS2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS2); \
    return Result; \
  }


/*-- yLT_SORT (Bag)------------------------------------------------*/
#define Bag_LT(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yLT_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    XCAT(SORT,_yptr) Temp, Temp2; \
    SDL_Boolean Result; \
    Result = PS1.Length < PS2.Length; \
    for (Temp=PS1.First; \
         Temp!=(XCAT(SORT,_yptr))0 && Result; \
         Temp=Temp->Suc) { \
      Temp2 = XCAT(yInInner_,SORT)(Temp->Data, PS2); \
      if (Temp2 == 0 || Temp->NoOfItems > Temp2->NoOfItems) \
        Result = SDL_False; \
    } \
    if (! PS1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS1); \
    if (! PS2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&PS2); \
    return Result; \
  }


/*-- yGE_SORT (Bag)------------------------------------------------*/
#define Bag_GE(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yGE_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    return XCAT(yLE_,SORT)(PS2, PS1); \
  }


/*-- yGT_SORT (Bag)------------------------------------------------*/
#define Bag_GT(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yGT_,SORT) \
    XPP_PROTO( (SORT PS1, SORT PS2) ) \
    XPP_NOPROTO( (PS1, PS2)  SORT PS1; SORT PS2; ) \
  { \
    return XCAT(yLT_,SORT)(PS2, PS1); \
  }


/*-- yAss_SORT (Bag)-----------------------------------------------*/
#define Bag_Assign(SORT, COMPONENTSORT) \
  static XCAT(SORT,_yptr)  XCAT(yAvail_,SORT) = (XCAT(SORT,_yptr))0; \
  void XCAT(yAss_,SORT) \
    XPP_PROTO( (SORT * SVar, SORT SExpr, int AssType) ) \
    XPP_NOPROTO( (SVar, SExpr, AssType)  SORT * SVar; SORT SExpr; int AssType; ) \
  { \
    if ((*SVar).First != (XCAT(SORT,_yptr))0 && (*SVar).First == SExpr.First) { \
      (*SVar).IsAssigned = (xbool)1; \
      return; \
    } \
    if (AssType == XASS && (*SVar).First != (XCAT(SORT,_yptr))0) { \
      XCAT(yFree_,SORT)((void **)SVar); \
    } \
    if ( (SExpr.IsAssigned || AssType == XASS2MAKE) && \
         SExpr.First != (XCAT(SORT,_yptr))0) { \
      *SVar = XCAT(yCopyList_,SORT)(SExpr); \
    } else { \
      (*SVar).First = SExpr.First; \
      (*SVar).Last = SExpr.Last; \
      (*SVar).Length = SExpr.Length; \
    } \
    (*SVar).IsAssigned = (xbool)1; \
  }


/*-- yEq_SORT (Bag)------------------------------------------------*/
#define Bag_Equal(SORT, COMPONENTSORT) \
  SDL_Boolean XCAT(yEq_,SORT) \
    XPP_PROTO( (SORT Expr1, SORT Expr2) ) \
    XPP_NOPROTO( (Expr1, Expr2) SORT Expr1; SORT Expr2; ) \
  { \
    XCAT(SORT,_yptr) Temp, Temp2; \
    SDL_Boolean Result; \
    Result = Expr1.Length == Expr2.Length; \
    for (Temp=Expr1.First; \
         Temp!=(XCAT(SORT,_yptr))0 && Result; \
         Temp=Temp->Suc) { \
      Temp2 = XCAT(yInInner_,SORT)(Temp->Data, Expr2); \
      if (Temp2 == 0 || Temp->NoOfItems != Temp2->NoOfItems) \
        Result = SDL_False; \
    } \
    if (! Expr1.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&Expr1); \
    if (! Expr2.IsAssigned) \
      XCAT(yFree_,SORT)((void **)&Expr2); \
    return Result; \
  }


/*-- yDef_SORT (Bag)-----------------------------------------------*/
#define Bag_Default(SORT, COMPONENTSORT) \
  void XCAT(yDef_,SORT) \
    XPP_PROTO( (SORT *S) ) \
    XPP_NOPROTO( (S) SORT *S; ) \
  { \
    (*S).First = (XCAT(SORT,_yptr))0; \
    (*S).Last = (XCAT(SORT,_yptr))0; \
    (*S).Length = 0; \
    (*S).IsAssigned = (xbool)1; \
  }


/*-- yFree_SORT (Bag)---------------------------------------------*/
#define Bag_Free(SORT, COMPONENTSORT, FREEFUNC) \
  void XCAT(yFree_,SORT) \
    XPP_PROTO( (void ** C) ) \
    XPP_NOPROTO( (C) void ** C; ) \
  { \
    XCAT(SORT,_yptr)  Temp, TempSuc; \
    if (FREEFUNC != XXFREE) { \
      for (Temp =  (*(SORT *)C).First; \
           Temp != (XCAT(SORT,_yptr))0; \
           Temp =  TempSuc) { \
        FREEFUNC((void **)&Temp->Data); \
        TempSuc =  Temp->Suc; \
        XFREE((void **)&Temp, sizeof(XCAT(SORT,_yrec))); \
      } \
    } \
    else if ((*(SORT *)C).First != (XCAT(SORT,_yptr))0) { \
      (*(SORT *)C).Last->Suc = XCAT(yAvail_,SORT); \
      XCAT(yAvail_,SORT) = (*(SORT *)C).First; \
    } \
    (*(SORT *)C).First = (XCAT(SORT,_yptr))0; \
    (*(SORT *)C).Last = (XCAT(SORT,_yptr))0; \
    (*(SORT *)C).Length = 0; \
    (*(SORT *)C).IsAssigned = (xbool)0; \
    for (Temp = XCAT(yAvail_,SORT); \
         Temp; \
         Temp = XCAT(yAvail_,SORT)) { \
      XCAT(yAvail_,SORT)  = Temp->Suc; \
      if (FREEFUNC != XXFREE)  FREEFUNC((void **)&Temp->Data); \
      XFREE((void **)&Temp, sizeof(XCAT(SORT,_yrec))); \
    } \
  }


/*-- yLength_SORT (Bag)---------------------------------------------*/
#define Bag_Length(SORT, COMPONENTSORT) \
  SDL_Integer XCAT(yLength_,SORT) \
    XPP_PROTO( (SORT PS) ) \
    XPP_NOPROTO( (PS)  SORT PS; ) \
  { \
    return PS.Length; \
  }


/*-- yTake_SORT (Bag)----------------------------------------------*/
#define Bag_Take(SORT, COMPONENTSORT) \
  COMPONENTSORT XCAT(yTake_,SORT) \
    XPP_PROTO( (SORT PS) ) \
    XPP_NOPROTO( (PS)  SORT PS; ) \
  { \
    COMPONENTSORT Result; \
    if (PS.First != (XCAT(SORT,_yptr))0) \
      return PS.First->Data; \
    xReportStringError("Take in Bag sort", "Empty bag") \
    (void)memset((void *)(&Result), 0, sizeof(COMPONENTSORT)); \
    XCAT(yDef_,COMPONENTSORT)(&Result); \
    return Result; \
  }


/*-- yTake2_SORT (Bag)---------------------------------------------*/
#define Bag_Take2(SORT, COMPONENTSORT) \
  COMPONENTSORT XCAT(yTake2_,SORT) \
    XPP_PROTO( (SORT PS, SDL_Integer Index) ) \
    XPP_NOPROTO( (PS, Index)  SORT PS; SDL_Integer Index; ) \
  { \
    XCAT(SORT,_yptr) Temp; \
    int Number = 0; \
    COMPONENTSORT Result; \
    for (Temp=PS.First; \
         Temp!=(XCAT(SORT,_yptr))0; \
         Temp=Temp->Suc) { \
      Number = Number + Temp->NoOfItems; \
      if (Number >= Index) break; \
    } \
    if (Temp != (XCAT(SORT,_yptr))0 && Index > 0) \
      return Temp->Data; \
    xReportStringError("Take in Bag sort", "Index out of range") \
    (void)memset((void *)(&Result), 0, sizeof(COMPONENTSORT)); \
    XCAT(yDef_,COMPONENTSORT)(&Result); \
    return Result; \
  }


/*-- yWri_SORT (Bag)----------------------------------------------*/
#if defined(ALPHACC) && defined(XVALIDATOR_LIB)
#define Bag_Write(SORT, COMPONENTSORT) \
  char * XCAT(yWri_,SORT) \
    XPP_PROTO( (void * PS) ) \
    XPP_NOPROTO( (PS) void * PS; ) \
  { \
    XCAT(SORT,_yptr) Temp, TempLast; \
    static  char     CTemp[MAX_WRI_LENGTH]; \
    static  char     NoOfItems[20]; \
    int              i; \
    \
    (void)strcpy(CTemp, "{ "); \
    TempLast = (*(SORT *)PS).Last; \
    for (Temp = (*(SORT *)PS).First; \
         Temp != (XCAT(SORT,_yptr))0; \
         Temp = Temp->Suc) { \
      for (i=1; i<=Temp->NoOfItems; i++) { \
        xSafeStrcat(CTemp, \
             xWriteSort((void *)(&Temp->Data), XCAT(ySrtN_,COMPONENTSORT))); \
        if (strlen(CTemp) == MAX_WRI_LENGTH - 1) \
          return CTemp; \
        if (! XASN1_SYNTAX) { \
          if (Temp->NoOfItems > 1) { \
            (void)sprintf(NoOfItems, ":%ld", Temp->NoOfItems); \
            xSafeStrcat(CTemp, NoOfItems); \
          } \
          break; \
        } \
        if (i < Temp->NoOfItems) \
          xSafeStrcat(CTemp, ", "); \
      } \
      if (Temp != TempLast) \
        xSafeStrcat(CTemp, ", "); \
    } \
    xSafeStrcat(CTemp, " }"); \
    return CTemp; \
  }
#else
#ifdef XITEXCOMM
#define Bag_Write_Start \
  if (XSYSD xUse_Itex_Syntax) \
    (void)strcpy(CTemp, "[ "); \
  else
#define Bag_Write_End \
  if (XSYSD xUse_Itex_Syntax) \
    xSafeStrcat(CTemp, " ]"); \
  else
#define AND_NOT_ITEX_SYNTAX && ! XSYSD xUse_Itex_Syntax
#else
#define Bag_Write_Start
#define Bag_Write_End
#define AND_NOT_ITEX_SYNTAX
#endif

#define Bag_Write(SORT, COMPONENTSORT) \
  char * XCAT(yWri_,SORT) \
    XPP_PROTO( (void * PS) ) \
    XPP_NOPROTO( (PS) void * PS; ) \
  { \
    XCAT(SORT,_yptr) Temp, TempLast; \
    static  char     CTemp[MAX_WRI_LENGTH]; \
    static  char     NoOfItems[20]; \
    int              i; \
    \
    Bag_Write_Start \
    (void)strcpy(CTemp, "{ "); \
    TempLast = (*(SORT *)PS).Last; \
    for (Temp = (*(SORT *)PS).First; \
         Temp != (XCAT(SORT,_yptr))0; \
         Temp = Temp->Suc) { \
      for (i=1; i<=Temp->NoOfItems; i++) { \
        xSafeStrcat(CTemp, \
             xWriteSort((void *)(&Temp->Data), XCAT(ySrtN_,COMPONENTSORT))); \
        if (strlen(CTemp) == MAX_WRI_LENGTH - 1) \
          return CTemp; \
        if (! XASN1_SYNTAX AND_NOT_ITEX_SYNTAX) { \
          if (Temp->NoOfItems > 1) { \
            (void)sprintf(NoOfItems, ":%d", Temp->NoOfItems); \
            xSafeStrcat(CTemp, NoOfItems); \
          } \
          break; \
        } \
        if (i < Temp->NoOfItems) \
          xSafeStrcat(CTemp, ", "); \
      } \
      if (Temp != TempLast) \
        xSafeStrcat(CTemp, ", "); \
    } \
    Bag_Write_End \
    xSafeStrcat(CTemp, " }"); \
    return CTemp; \
  }
#endif


/*-- yRead_SORT (Bag)---------------------------------------------*/
#ifdef XITEXCOMM
#define Bag_Read_StartToken (Token != xxLBracket) &&
#define Bag_Read_EndToken   (Token != xxRBracket) &&
#define Bag_Read_EndTokenOr (Token == xxRBracket) ||
#else
#define Bag_Read_StartToken
#define Bag_Read_EndToken
#define Bag_Read_EndTokenOr
#endif

#define Bag_Read(SORT, COMPONENTSORT) \
  int XCAT(yRead_,SORT) \
    XPP_PROTO( (void * Result) ) \
    XPP_NOPROTO( (Result) void * Result; ) \
  { \
    COMPONENTSORT Item; \
    xxToken       Token; \
    char          strVar[256]; \
    int           Res, intVar; \
    \
    (*(SORT *)Result).First = (XCAT(SORT,_yptr))0; \
    (*(SORT *)Result).Last = (XCAT(SORT,_yptr))0; \
    (*(SORT *)Result).Length = 0; \
    (*(SORT *)Result).IsAssigned = (int)0; \
    Token = xScanToken(strVar); \
    if (XVALUE_SYNTAX && \
        Bag_Read_StartToken \
        Token != xxLCurlyBracket && Token != xxQuestionMark && \
        Token != xxEoln) { \
      xPrintString("Bag value should start with {\n"); \
      return 0; \
    } \
    if (Bag_Read_StartToken \
        Token != xxLCurlyBracket) xUngetToken(Token, strVar); \
    \
    while (Bag_Read_EndToken \
           Token != xxRCurlyBracket) { \
      (void)sprintf(strVar, " (%s) : ", XCAT(ySrtN_,COMPONENTSORT)->Name); \
      (void)memset((void *)(&Item), 0, sizeof(COMPONENTSORT)); \
      Res = xReadOneParameter(XCAT(ySrtN_,COMPONENTSORT), strVar, (void *)&Item); \
      if (Res == 0) return 0; \
      if (Res == 2) { \
        Token = xScanToken(strVar); \
        if (Bag_Read_EndTokenOr \
            Token == xxRCurlyBracket) break; \
        return 0; \
      } \
      *(SORT *)Result = XCAT(yIncl_,SORT)(Item, *(SORT *)Result); \
      Token = xScanToken(strVar); \
      if (Token == xxColon) { \
        Token = xScanToken(strVar); \
        if (Token == xxId && sscanf(strVar, "%d", &intVar) && intVar >= 1) { \
          for (Res=2; Res<=intVar; Res++) \
            *(SORT *)Result = XCAT(yIncl_,SORT)(Item, *(SORT *)Result); \
          Token = xScanToken(strVar); \
	} else { \
          xPrintString("Illegal syntax for number of items"); \
          return 0; \
        } \
      } \
      if (Bag_Read_EndToken \
          Token != xxRCurlyBracket) xUngetToken(Token, strVar); \
    } \
    (*(SORT *)Result).IsAssigned = (int)1; \
    return 1; \
  }
#endif
   /* XNO_LONG_MACROS */

#ifndef XNOUSEOFOBJECTIDENTIFIER
/*---+---------------------------------------------------------------
     Object_Identifier
-------------------------------------------------------------------*/

String_Type(SDL_Object_Identifier, SDL_Natural)
String_prototypes(SDL_Object_Identifier, SDL_Natural)

#define xDef_SDL_Object_Identifier(V)       yDef_SDL_Object_Identifier(V)
extern void yDef_SDL_Object_Identifier XPP((SDL_Object_Identifier *));

#define yAssF_SDL_Object_Identifier(V,E,A)  yAss_SDL_Object_Identifier(&(V),E,A)
extern void yAss_SDL_Object_Identifier
    XPP((SDL_Object_Identifier *, SDL_Object_Identifier, int));

#define yEqF_SDL_Object_Identifier(E1,E2)   yEq_SDL_Object_Identifier(E1,E2)
#define yNEqF_SDL_Object_Identifier(E1,E2)  (! yEq_SDL_Object_Identifier(E1,E2))
extern SDL_Boolean yEq_SDL_Object_Identifier
    XPP((SDL_Object_Identifier, SDL_Object_Identifier));

extern void yFree_SDL_Object_Identifier XPP((void **));

#define ySrtN_SDL_Object_Identifier         xSrtN_SDL_Object_Identifier

#define ANY_SDL_Object_Identifier  \
   yConcat_SDL_Object_Identifier(yMkString_SDL_Object_Identifier(ANY_SDL_Natural), \
     yMkString_SDL_Object_Identifier(ANY_SDL_Natural))
#endif
       /* XNOUSEOFOBJECTIDENTIFIER */


extern void * xPassOwn XPP((void **P));

#endif
   /* X_SCTPREDG_H */
