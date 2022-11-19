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
$Id: sctpred.c, this is based on v 1.33 of sctpred.h 1997/10/09 fbi Exp $ 
*/


/* This file contains implementation of operators and the read and
   write functions for the predefined data types except PId.
*/
/*
FILE INDEX
01   IdNodes for package Predefined (not relevant for Cmicro)
02   Operator implementations
03   Read and Write functions (not relevant for Cmicro)
*/

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#include "scttypes.h"


/********  SDT VALIDATOR ********/
#ifdef XVALIDATOR_LIB
#ifndef XSYSD
#define XSYSD
#endif
SDL_Time xMaxTime = {2147483647,0};

#endif /* ... XVALIDATOR_LIB */

#else /* then XSCT_CMICRO */

#include "ml_typw.h"

#ifndef XSYSD
#define XSYSD
#endif

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */


/****+***************************************************************
01   IdNodes for package Predefined
********************************************************************/

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#ifndef XOPTSORT

/*---+---------------------------------------------------------------
     Package Predefined
-------------------------------------------------------------------*/

struct xPackageIdStruct yPacR_Predefined = {xPackageEC
  xSymbTLink((xIdNode)0, (xIdNode)0), xSymbolTableRoot xIdNames("Predefined")
  XCOMMON_EXTRAS xIdNames(0) XPAC_EXTRAS};

/*---+---------------------------------------------------------------
     Boolean
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Boolean = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("boolean")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(0) xRaWF(0) xRaWF(0) xTestF(0),
  (xptrint)sizeof(SDL_Boolean), xPredef, (xSortIdNode)0, (xSortIdNode)0, 0,
  0 XSRT_EXTRAS};
#ifndef XOPTLIT

#error "warning_static_in_use_DLH"
static XCONST struct xLiteralIdStruct yFalseR = {xLiteralEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&xSrtR_SDL_Boolean xIdNames("false")
  XCOMMON_EXTRAS XLIT_EXTRAS};

#error "warning_static_in_use_DLH"
static XCONST struct xLiteralIdStruct yTrueR = {xLiteralEC  xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&xSrtR_SDL_Boolean xIdNames("true")
  XCOMMON_EXTRAS XLIT_EXTRAS};
#endif

/*---+---------------------------------------------------------------
     Bit
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Bit = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("bit")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(0) xRaWF(xRead_SDL_Bit)
  xRaWF(xWri_SDL_Bit) xTestF(0), (xptrint)sizeof(SDL_Bit),
  xPredef, (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     Bit_String
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING
XCONST struct xSortIdStruct xSrtR_SDL_Bit_String = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("bit_string")
  XCOMMON_EXTRAS xFreF(xFree_SDL_Bit_String) xAssF(xAss_SDL_Bit_String)
  xEqF(xEq_SDL_Bit_String) xRaWF(xRead_SDL_Bit_String)
  xRaWF(xWri_SDL_Bit_String) xTestF(0), (xptrint)sizeof(SDL_Bit_String),
  xPredef, (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};
#endif
       /* XNOUSEOFOCTETBITSTRING */

/*---+---------------------------------------------------------------
     Octet
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING
XCONST struct xSortIdStruct xSrtR_SDL_Octet = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("octet")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(0) xRaWF(xRead_SDL_Octet)
  xRaWF(xWri_SDL_Octet) xTestF(0), (xptrint)sizeof(SDL_Octet),
  xPredef, (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};
#endif
       /* XNOUSEOFOCTETBITSTRING */

/*---+---------------------------------------------------------------
     Octet_String
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING
XCONST struct xSortIdStruct xSrtR_SDL_Octet_String = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("octet_string")
  XCOMMON_EXTRAS xFreF(xFree_SDL_Bit_String) xAssF(xAss_SDL_Bit_String)
  xEqF(xEq_SDL_Bit_String) xRaWF(xRead_SDL_Octet_String)
  xRaWF(xWri_SDL_Octet_String) xTestF(0), (xptrint)sizeof(SDL_Octet_String),
  xPredef, (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};
#endif
       /* XNOUSEOFOCTETBITSTRING */

/*---+---------------------------------------------------------------
     Character
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Character = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("character")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(0) xRaWF(xRead_SDL_Character)
  xRaWF(xWri_SDL_Character) xTestF(0), (xptrint)sizeof(SDL_Character),
  xPredef, (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     Charstring
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Charstring = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("charstring")
  XCOMMON_EXTRAS xFreF(xFree_SDL_Charstring) xAssF(xAss_SDL_Charstring)
  xEqF(xEq_SDL_Charstring) xRaWF(xRead_SDL_Charstring)
  xRaWF(xWri_SDL_Charstring) xTestF(0), (xptrint)sizeof(SDL_Charstring),
  xPredef, (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     IA5String
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_IA5String = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("IA5String")
  XCOMMON_EXTRAS xFreF(xFree_SDL_Charstring) xAssF(xAss_SDL_Charstring)
  xEqF(xEq_SDL_Charstring) xRaWF(xRead_SDL_Charstring)
  xRaWF(xWri_SDL_Charstring) xTestF(0), (xptrint)sizeof(SDL_Charstring),
  xSyntype, &xSrtR_SDL_Charstring, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     NumericString
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_NumericString = {xSortEC
  xSymbTLink((xIdNode)0, (xIdNode)0), (xIdNode)&yPacR_Predefined
  xIdNames("NumericString") XCOMMON_EXTRAS xFreF(xFree_SDL_Charstring)
  xAssF(xAss_SDL_Charstring) xEqF(xEq_SDL_Charstring)
  xRaWF(xRead_SDL_NumericString)
  xRaWF(xWri_SDL_NumericString) xTestF(xTest_SDL_NumericString),
  (xptrint)sizeof(SDL_Charstring), xSyntype, &xSrtR_SDL_Charstring,
  (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     PrintableString
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_PrintableString = {xSortEC
  xSymbTLink((xIdNode)0, (xIdNode)0), (xIdNode)&yPacR_Predefined
  xIdNames("PrintableString") XCOMMON_EXTRAS xFreF(xFree_SDL_Charstring)
  xAssF(xAss_SDL_Charstring) xEqF(xEq_SDL_Charstring)
  xRaWF(xRead_SDL_PrintableString)
  xRaWF(xWri_SDL_PrintableString) xTestF(xTest_SDL_PrintableString),
  (xptrint)sizeof(SDL_Charstring), xSyntype, &xSrtR_SDL_Charstring,
  (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     VisibleString
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_VisibleString = {xSortEC
  xSymbTLink((xIdNode)0, (xIdNode)0), (xIdNode)&yPacR_Predefined
  xIdNames("VisibleString") XCOMMON_EXTRAS xFreF(xFree_SDL_Charstring)
  xAssF(xAss_SDL_Charstring) xEqF(xEq_SDL_Charstring)
  xRaWF(xRead_SDL_VisibleString)
  xRaWF(xWri_SDL_VisibleString) xTestF(xTest_SDL_PrintableString),
  (xptrint)sizeof(SDL_Charstring), xSyntype, &xSrtR_SDL_Charstring,
  (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     Duration
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Duration = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("duration")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(xEq_SDL_Duration)
  xRaWF(xRead_SDL_Duration) xRaWF(xWri_SDL_Duration) xTestF(0),
  (xptrint)sizeof(SDL_Duration), xPredef, (xSortIdNode)0, (xSortIdNode)0,
  0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     Integer
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Integer = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("integer")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(0) xRaWF(xRead_SDL_Integer)
  xRaWF(xWri_SDL_Integer) xTestF(0), (xptrint)sizeof(SDL_Integer),
  xPredef, (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     Natural
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Natural = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("natural")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(0) xRaWF(xRead_SDL_Integer)
  xRaWF(xWri_SDL_Integer) xTestF(xTest_SDL_Natural),
  (xptrint)sizeof(SDL_Integer), xSyntype, &xSrtR_SDL_Integer,
  (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     Null
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Null = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("null")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(0) xRaWF(xRead_SDL_Null)
  xRaWF(xWri_SDL_Null) xTestF(0), (xptrint)sizeof(SDL_Null),
  xPredef, (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     Object_Identifier
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOBJECTIDENTIFIER
XCONST struct xSortIdStruct xSrtR_SDL_Object_Identifier = {xSortEC
  xSymbTLink((xIdNode)0, (xIdNode)0), (xIdNode)&yPacR_Predefined
  xIdNames("object_identifier") XCOMMON_EXTRAS xFreF(yFree_SDL_Object_Identifier)
  xAssF(yAss_SDL_Object_Identifier) xEqF(yEq_SDL_Object_Identifier)
  xRaWF(yRead_SDL_Object_Identifier) xRaWF(yWri_SDL_Object_Identifier)
  xTestF(0), (xptrint)sizeof(SDL_Object_Identifier), xString, &xSrtR_SDL_Natural,
  (xSortIdNode)0, 0, 0 XSRT_EXTRAS};
#endif
       /* XNOUSEOFOBJECTIDENTIFIER */

/*---+---------------------------------------------------------------
     PId
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_PId = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("pid")
  XCOMMON_EXTRAS xFreF(0) xAssF(0) xEqF(yEq_SDL_PId) xRaWF(xRead_SDL_PId)
  xRaWF(xWri_SDL_PId) xTestF(0), (xptrint)sizeof(SDL_PId), xPredef,
  (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};

/*---+---------------------------------------------------------------
     Real
-------------------------------------------------------------------*/

#ifndef XNOUSEOFREAL
XCONST struct xSortIdStruct xSrtR_SDL_Real = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("real") XCOMMON_EXTRAS
  xFreF(0) xAssF(0) xEqF(0) xRaWF(xRead_SDL_Real) xRaWF(xWri_SDL_Real)
  xTestF(0), (xptrint)sizeof(SDL_Real), xPredef, (xSortIdNode)0,
  (xSortIdNode)0, 0, 0 XSRT_EXTRAS};
#endif

/*---+---------------------------------------------------------------
     Time
-------------------------------------------------------------------*/

XCONST struct xSortIdStruct xSrtR_SDL_Time = {xSortEC xSymbTLink((xIdNode)0,
  (xIdNode)0), (xIdNode)&yPacR_Predefined xIdNames("time") XCOMMON_EXTRAS
  xFreF(0) xAssF(0) xEqF(xEq_SDL_Duration) xRaWF(xRead_SDL_Time)
  xRaWF(xWri_SDL_Time) xTestF(0), (xptrint)sizeof(SDL_Time), xPredef,
  (xSortIdNode)0, (xSortIdNode)0, 0, 0 XSRT_EXTRAS};
#endif


/*---+---------------------------------------------------------------
     yInit_Predefined
-------------------------------------------------------------------*/

#ifndef XOPTSORT
void yInit_Predefined XPP((void))
{
  xInsertIdNode((xIdNode)&yPacR_Predefined);
  xInsertIdNode((xIdNode)&xSrtR_SDL_Boolean);
#ifndef XOPTLIT
  xInsertIdNode((xIdNode)&yFalseR);
  xInsertIdNode((xIdNode)&yTrueR);
#endif
  xInsertIdNode((xIdNode)&xSrtR_SDL_Bit);
#ifndef XNOUSEOFOCTETBITSTRING
  xInsertIdNode((xIdNode)&xSrtR_SDL_Bit_String);
#endif
  xInsertIdNode((xIdNode)&xSrtR_SDL_Character);
  xInsertIdNode((xIdNode)&xSrtR_SDL_Charstring);
  xInsertIdNode((xIdNode)&xSrtR_SDL_Duration);
  xInsertIdNode((xIdNode)&xSrtR_SDL_IA5String);
  xInsertIdNode((xIdNode)&xSrtR_SDL_Integer);
  xInsertIdNode((xIdNode)&xSrtR_SDL_Natural);
  xInsertIdNode((xIdNode)&xSrtR_SDL_Null);
  xInsertIdNode((xIdNode)&xSrtR_SDL_NumericString);
#ifndef XNOUSEOFOBJECTIDENTIFIER
  xInsertIdNode((xIdNode)&xSrtR_SDL_Object_Identifier);
#endif
#ifndef XNOUSEOFOCTETBITSTRING
  xInsertIdNode((xIdNode)&xSrtR_SDL_Octet);
  xInsertIdNode((xIdNode)&xSrtR_SDL_Octet_String);
#endif
  xInsertIdNode((xIdNode)&xSrtR_SDL_PId);
  xInsertIdNode((xIdNode)&xSrtR_SDL_PrintableString);
#ifndef XNOUSEOFREAL
  xInsertIdNode((xIdNode)&xSrtR_SDL_Real);
#endif
  xInsertIdNode((xIdNode)&xSrtR_SDL_Time);
  xInsertIdNode((xIdNode)&xSrtR_SDL_VisibleString);
}
#endif

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */


/****+***************************************************************
02   Operator implementations
********************************************************************/

/*** Bit_String ***************************************************/

#ifndef XNOUSEOFOCTETBITSTRING

/*---+---------------------------------------------------------------
     xAss_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
void xAss_SDL_Bit_String (
  SDL_Bit_String *BVar,
  SDL_Bit_String  BExpr,
  int             AssType )
#else
void xAss_SDL_Bit_String (BVar, BExpr, AssType)
  SDL_Bit_String *BVar;
  SDL_Bit_String  BExpr;
  int             AssType;
#endif
{
  int IsXASSPARA = XMK_FALSE ;
  /* Needed because *BVar equals BExpr in XASSCOPY case : */
  SDL_Bit_String tmpv; /* only needed for XASSCOPY case */
  PRINTF((" XASSPARA = %d", AssType));
  if (AssType == XASSPARA)
  {
      PRINTF((" XASSPARA = %d", AssType));
    /*
    ** Default setting of octet_string variable - used in the
    ** case of output only (corrected by fbi)
    */
    (*BVar).Bits = 0;
    (*BVar).Length = 0;
    (*BVar).IsAssigned = (xbool)1;
    IsXASSPARA = XMK_TRUE ;
    AssType = XASS ;
  }
  else if (AssType == XASSCOPY)
  {
    /* 
    ** When a PRD with FPAR IN is called, the incoming variable must
    ** be duplicated. This concerns the dynamic memory as well.
    ** In the case of Bit_String / Octet_String, a C struct value
    ** is passed into the PRD C function, which is already a copy
    ** of the incoming variable. So, only the dynamic memory must be
    ** allocated newly (if there was some allocated in the incoming variable).
    */
    if ( (*BVar).Bits != (SDL_Bit *)0 )
    {
      if ( ! ( (*BVar).IsAssigned ) && ( (*BVar).Length != 0) )
      {
        (*BVar).IsAssigned = (xbool)1;
      }
      else
      {
        tmpv.IsUsedInSignal = (xbool)0;
        /* Incoming Variable FPAR IN contains a value */
        tmpv.Bits = xAlloc_SDL_Bit_String((xptrint)(BExpr.Length));
        (void)memcpy((void *)(tmpv).Bits, (void *)BExpr.Bits, BExpr.Length);
        (*BVar).Bits = tmpv.Bits;
        (*BVar).IsAssigned = (xbool)1; /* FBI */
      }
    }
    return ;
  }

  if ( (*BVar).Bits != (SDL_Bit *)0 && (*BVar).Bits == BExpr.Bits )
    return;
  if ( (*BVar).Bits != (SDL_Bit *)0 && AssType == XASS )
    xFree_SDL_Bit_String((void **)BVar);
  (*BVar).Length = BExpr.Length;
  (*BVar).IsAssigned = (xbool)1;
  if (!BExpr.IsAssigned && (AssType != XASS2MAKE))
    (*BVar).Bits = BExpr.Bits;
  else if (BExpr.Bits == (SDL_Bit *)0)
    (*BVar).Bits = (SDL_Bit *)0;
  else {
    (*BVar).Bits = xAlloc_SDL_Bit_String((xptrint)(BExpr.Length));
    (void)memcpy((void *)(*BVar).Bits, (void *)BExpr.Bits, BExpr.Length);
  }
  /* Used in output : */
  if (AssType == XASSMAKE)
  {
    /* 
    ** Tag destination variable as not assigned (occurs in
    ** PRD returns x <sortname> and Make!), so that 
    ** the memory later can be reused when it is assigned 
    ** to the SDL variable.
    */
    (*BVar).IsAssigned = (xbool)0;
  }
  if (IsXASSPARA)
  {
    (*BVar).IsUsedInSignal = (xbool)XMK_TRUE;
  }
  /* Used in input : */
  if (BExpr.IsUsedInSignal)
  {
    xFree_SDL_Bit_String((void **)&BExpr); 
  }

}


/*---+---------------------------------------------------------------
     xEq_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Boolean xEq_SDL_Bit_String (
  SDL_Bit_String B1,
  SDL_Bit_String B2 )
#else
SDL_Boolean xEq_SDL_Bit_String (B1, B2)
  SDL_Bit_String B1;
  SDL_Bit_String B2;
#endif
{
  SDL_Bit * p1;
  SDL_Bit * p2;
  SDL_Boolean Result;
  int i;

  Result = SDL_True;
  if (B1.Length != B2.Length) Result = SDL_False;
  p1 = B1.Bits; p2 = B2.Bits;
  for (i=0; i<B1.Length && Result; i++)
    if ( *(p1++) != *(p2++) ) Result = SDL_False;
  if (! B1.IsAssigned) xFree_SDL_Bit_String((void **)&B1);
  if (! B2.IsAssigned) xFree_SDL_Bit_String((void **)&B2);
  return Result;
}


/*---+---------------------------------------------------------------
     yAddr_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit * yAddr_SDL_Bit_String(
  SDL_Bit_String *B,
  SDL_Integer     Index)
#else
SDL_Bit * yAddr_SDL_Bit_String( B, Index )
  SDL_Bit_String *B;
  SDL_Integer     Index;
#endif
{
  if ( (Index < 0) || (Index >= (*B).Length) ) {
#ifdef XECSOP
    xSDLOpError("Modify! in sort Bit_String",
                "Index out of bounds." );
#endif
    if ((*B).Bits != 0)
      return &((*B).Bits[0]);
    else
      return (SDL_Bit *)xAlloc((xptrint)1);  /* ??? */
  } else
    return &((*B).Bits[Index]);
}


/*---+---------------------------------------------------------------
     xExtr_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit xExtr_SDL_Bit_String(
  SDL_Bit_String  B,
  SDL_Integer     Index )
#else
SDL_Bit xExtr_SDL_Bit_String( B, Index )
  SDL_Bit_String  B;
  SDL_Integer     Index;
#endif
{
  SDL_Bit Result;
  if ( (Index < 0) || (Index >= B.Length) ) {
#ifdef XECSOP
    xSDLOpError("Extract! in sort Bit_String",
                "Index out of bounds." );
#endif
    Result = 0;
  } else
    Result = B.Bits[Index];
  if (! B.IsAssigned) xFree_SDL_Bit_String((void **)&B);
  return Result;
}


/*---+---------------------------------------------------------------
     xNot_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xNot_SDL_Bit_String (
  SDL_Bit_String B )
#else
SDL_Bit_String xNot_SDL_Bit_String (B)
  SDL_Bit_String B;
#endif
{
  SDL_Bit_String Result;
  int i;
  Result.Length = B.Length;
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)(B.Length));
  for (i=0; i<B.Length; i++) {
    Result.Bits[i] = (unsigned char)( ! B.Bits[i]);
  }
  if (! B.IsAssigned) xFree_SDL_Bit_String((void **)&B);
  return Result;
}


/*---+---------------------------------------------------------------
     xAnd_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xAnd_SDL_Bit_String (
  SDL_Bit_String B1,
  SDL_Bit_String B2 )
#else
SDL_Bit_String xAnd_SDL_Bit_String (B1, B2)
  SDL_Bit_String B1;
  SDL_Bit_String B2;
#endif
{
  SDL_Bit_String Result;
  int i, Min_Length;

  if (B1.Length >= B2.Length) {
    Result.Length = B1.Length;
    Min_Length = B2.Length;
  } else {
    Result.Length = B2.Length;
    Min_Length = B1.Length;
  }
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)(Result.Length));
  for (i=0; i<Min_Length; i++)
    Result.Bits[i] = B1.Bits[i] & B2.Bits[i];
  for (i=Min_Length; i<Result.Length; i++)
    Result.Bits[i] = 0;
  if (! B1.IsAssigned) xFree_SDL_Bit_String((void **)&B1);
  if (! B2.IsAssigned) xFree_SDL_Bit_String((void **)&B2);
  return Result;
}


/*---+---------------------------------------------------------------
     xOr_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xOr_SDL_Bit_String (
  SDL_Bit_String B1,
  SDL_Bit_String B2 )
#else
SDL_Bit_String xOr_SDL_Bit_String (B1, B2)
  SDL_Bit_String B1;
  SDL_Bit_String B2;
#endif
{
  SDL_Bit_String Result;
  int i, Min_Length;

  if (B1.Length >= B2.Length) {
    Result.Length = B1.Length;
    Min_Length = B2.Length;
  } else {
    Result.Length = B2.Length;
    Min_Length = B1.Length;
  }
  Result.IsAssigned = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)(Result.Length));
  for (i=0; i<Min_Length; i++)
    Result.Bits[i] = B1.Bits[i] | B2.Bits[i];
  for (i=Min_Length; i<Result.Length; i++)
    Result.Bits[i] = 1;
  if (! B1.IsAssigned) xFree_SDL_Bit_String((void **)&B1);
  if (! B2.IsAssigned) xFree_SDL_Bit_String((void **)&B2);
  return Result;
}


/*---+---------------------------------------------------------------
     xXor_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xXor_SDL_Bit_String (
  SDL_Bit_String B1,
  SDL_Bit_String B2 )
#else
SDL_Bit_String xXor_SDL_Bit_String (B1, B2)
  SDL_Bit_String B1;
  SDL_Bit_String B2;
#endif
{
  SDL_Bit_String Result;
  int i, Min_Length;

  if (B1.Length >= B2.Length) {
    Result.Length = B1.Length;
    Min_Length = B2.Length;
  } else {
    Result.Length = B2.Length;
    Min_Length = B1.Length;
  }
  Result.IsAssigned = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)(Result.Length));
  for (i=0; i<Min_Length; i++)
    Result.Bits[i] = B1.Bits[i] ^ B2.Bits[i];
  for (i=Min_Length; i<Result.Length; i++)
    Result.Bits[i] = 1;
  if (! B1.IsAssigned) xFree_SDL_Bit_String((void **)&B1);
  if (! B2.IsAssigned) xFree_SDL_Bit_String((void **)&B2);
  return Result;
}


/*---+---------------------------------------------------------------
     xImpl_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xImpl_SDL_Bit_String (
  SDL_Bit_String B1,
  SDL_Bit_String B2 )
#else
SDL_Bit_String xImpl_SDL_Bit_String (B1, B2)
  SDL_Bit_String B1;
  SDL_Bit_String B2;
#endif
{
  SDL_Bit_String Result;
  int i, Min_Length;

  if (B1.Length >= B2.Length) {
    Result.Length = B1.Length;
    Min_Length = B2.Length;
  } else {
    Result.Length = B2.Length;
    Min_Length = B1.Length;
  }
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)(Result.Length));
  for (i=0; i<Min_Length; i++)
    Result.Bits[i] = (unsigned char)(B1.Bits[i] <= B2.Bits[i]);
  for (i=Min_Length; i<Result.Length; i++)
    Result.Bits[i] = 1;
  if (! B1.IsAssigned) xFree_SDL_Bit_String((void **)&B1);
  if (! B2.IsAssigned) xFree_SDL_Bit_String((void **)&B2);
  return Result;
}

/*---+---------------------------------------------------------------
     xMkString_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xMkString_SDL_Bit_String (
  SDL_Bit B )
#else
SDL_Bit_String xMkString_SDL_Bit_String ( B )
  SDL_Bit B;
#endif
{
  SDL_Bit_String Result;
  Result.Length = 1;
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)1);
  Result.Bits[0] = B;
  return Result;
}

/*---+---------------------------------------------------------------
     xLength_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Integer xLength_SDL_Bit_String (
  SDL_Bit_String B )
#else
SDL_Integer xLength_SDL_Bit_String ( B )
  SDL_Bit_String B;
#endif
{
  return B.Length;
}

/*---+---------------------------------------------------------------
     xFirst_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit xFirst_SDL_Bit_String (
  SDL_Bit_String B )
#else
SDL_Bit xFirst_SDL_Bit_String ( B )
  SDL_Bit_String B;
#endif
{
  SDL_Bit Result;
  if (B.Length == 0) {
#ifdef XECSOP
    xSDLOpError("First in sort Bit_String",
                "Bit_String length is zero." );
#endif
    Result = (SDL_Bit)0;
  } else {
    Result =  B.Bits[0];
  }
  if (! B.IsAssigned) xFree_SDL_Bit_String((void **)&B);
  return Result;
}

/*---+---------------------------------------------------------------
     xLast_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit xLast_SDL_Bit_String (
  SDL_Bit_String B )
#else
SDL_Bit xLast_SDL_Bit_String ( B )
  SDL_Bit_String B;
#endif
{
  SDL_Bit Result;
  if (B.Length == 0) {
#ifdef XECSOP
    xSDLOpError("Last in sort Bit_String",
                "Bit_String length is zero." );
#endif
    Result = (SDL_Bit)0;
  } else {
    Result = B.Bits[B.Length-1];
  }
  if (! B.IsAssigned) xFree_SDL_Bit_String((void **)&B);
  return Result;
}

/*---+---------------------------------------------------------------
     xConcat_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xConcat_SDL_Bit_String (
  SDL_Bit_String B1,
  SDL_Bit_String B2 )
#else
SDL_Bit_String xConcat_SDL_Bit_String ( B1, B2 )
  SDL_Bit_String B1;
  SDL_Bit_String B2;
#endif
{
  SDL_Bit_String Result;

  Result.Length = B1.Length+B2.Length;
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)(Result.Length));
  if (B1.Length>0)
    (void)memcpy((void *)Result.Bits, (void *)B1.Bits, B1.Length);
  if (B2.Length>0)
    (void)memcpy((void *)&(Result.Bits[B1.Length]), (void *)B2.Bits, B2.Length);
  if (! B1.IsAssigned) xFree_SDL_Bit_String((void **)&B1);
  if (! B2.IsAssigned) xFree_SDL_Bit_String((void **)&B2);
  return Result;
}

/*---+---------------------------------------------------------------
     xSubString_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xSubString_SDL_Bit_String (
  SDL_Bit_String B,
  SDL_Integer    Start,
  SDL_Integer    SubLength )
#else
SDL_Bit_String xSubString_SDL_Bit_String (B, Start, SubLength)
  SDL_Bit_String B;
  SDL_Integer    Start;
  SDL_Integer    SubLength;
#endif
{
  SDL_Bit_String Result;

  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0;
  Result.Bits = 0;
  if ( B.Length == 0 ) {
#ifdef XECSOP
    xSDLOpError("SubString in sort Bit_String",
                "Bit_String length is zero." );
#endif
    Result.Length = 0;
  } else if ( Start < 0 ) {
#ifdef XECSOP
    xSDLOpError("SubString in sort Bit_String",
                "Start is less than zero." );
#endif
    Result.Length = 0;
  } else if ( SubLength <= 0 ) {
#ifdef XECSOP
    xSDLOpError("SubString in sort Bit_String",
                "SubLength is less than or equal to zero." );
#endif
    Result.Length = 0;
  } else if ( Start+SubLength > B.Length ) { 
#ifdef XECSOP
    xSDLOpError("SubString in sort Bit_String",
                "Start + Substring length is greater than string length." );
#endif
    Result.Length = 0;
  } else {
/*  Modified by GBU,000103  added cast since "SDL_Integer" and "int" may differ in size */
/*
    Result.Length = SubLength;
*/
    Result.Length = (int)SubLength;
    Result.Bits = xAlloc_SDL_Bit_String((xptrint)(Result.Length));
    (void)memcpy((void *)Result.Bits, (void *)&(B.Bits[Start]), Result.Length);
  }
  if (! B.IsAssigned) xFree_SDL_Bit_String((void **)&B);
  return Result;
}

/*---+---------------------------------------------------------------
     xBitStr_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xBitStr_SDL_Bit_String (
  SDL_Charstring C )
#else
SDL_Bit_String xBitStr_SDL_Bit_String ( C )
  SDL_Charstring C;
#endif
{
  SDL_Bit_String Result;
  int i;
  Result.Length = xLength_SDL_Charstring(C);
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)(Result.Length));
  for (i=0; i<Result.Length; i++) {
    if (C[i+1] == '0') {
      Result.Bits[i] = (SDL_Bit)0;
    } else if (C[i+1] == '1') {
      Result.Bits[i] = (SDL_Bit)1;
    } else {
#ifdef XECSOP
      xSDLOpError("BitStr in sort Bit_String",
                  "Illegal character in Charstring (not 0 or 1)." );
#endif
      Result.Bits[i] = (SDL_Bit)1;
    }
  }
  return Result;
}

/*---+---------------------------------------------------------------
     xHexStr_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xHexStr_SDL_Bit_String (
  SDL_Charstring C )
#else
SDL_Bit_String xHexStr_SDL_Bit_String ( C )
  SDL_Charstring C;
#endif
{
  SDL_Bit_String Result;
  int C_length, C_index, value;

  C_length = xLength_SDL_Charstring(C);
  Result.Length = C_length*4;
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0;
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)Result.Length);
  for (C_index=1; C_index<=C_length; C_index++) {
    value = (int)(C[C_index]) - 'a' + 10;
    if (value < 10) value = (int)(C[C_index]) - 'A' + 10;
    if (value < 10) value = (int)(C[C_index]) - '0';

    if (value < 0 || value > 15) {
#ifdef XECSOP
      xSDLOpError("HexStr in sort Bit_String",
                  "Illegal character in Charstring (not digit or a-f)." );
#endif
      Result.Bits[(C_index-1)*4] = 0;
      Result.Bits[(C_index-1)*4+1] = 0;
      Result.Bits[(C_index-1)*4+2] = 0;
      Result.Bits[(C_index-1)*4+3] = 0;
    } else {
      Result.Bits[(C_index-1)*4] = (unsigned char)(value/8);
      Result.Bits[(C_index-1)*4+1] = (unsigned char)((value/4)%2);
      Result.Bits[(C_index-1)*4+2] = (unsigned char)((value/2)%2);
      Result.Bits[(C_index-1)*4+3] = (unsigned char)(value%2);
    }
  }
  return Result;
}

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*** Octet *********************************************************/

#ifndef XNOUSEOFOCTETBITSTRING

/*---+---------------------------------------------------------------
     yAddr_SDL_Octet
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit * yAddr_SDL_Octet(
  SDL_Octet   *B,
  SDL_Integer  Index)
#else
SDL_Bit * yAddr_SDL_Octet( B, Index )
  SDL_Octet   *B;
  SDL_Integer  Index;
#endif
{
  if ( (Index < 0) || (Index >= 8 )) {
#ifdef XECSOP
    xSDLOpError("Modify! in sort Octet",
                "Index out of bounds." );
#endif
    return B;
  }
  return B;   /* &((*B).Bits[Index]);   ????? */
}


/*---+---------------------------------------------------------------
     xExtr_SDL_Octet
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit xExtr_SDL_Octet(
  SDL_Octet   B,
  SDL_Integer Index )
#else
SDL_Bit xExtr_SDL_Octet( B, Index )
  SDL_Octet   B;
  SDL_Integer Index;
#endif
{
  if ( (Index < 0) || (Index >= 8) ) {
#ifdef XECSOP
    xSDLOpError("Extract! in sort Octet",
                "Index out of bounds." );
#endif
    return 0;
  }
  return (B & ((unsigned char)1 << Index)) ? 1 : 0;
}


#ifdef XEINTDIV
/*---+---------------------------------------------------------------
     xDiv_SDL_Octet
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet xDiv_SDL_Octet(
  SDL_Octet  i,
  SDL_Octet  k )
#else
SDL_Octet xDiv_SDL_Octet( i, k )
  SDL_Octet  i;
  SDL_Octet  k;
#endif
{
  if ( k == 0 ) {
    xSDLOpError("Division in sort Octet", "Octet division with 0" );
    return 0;
  } else
    return (SDL_Octet)(i/k);
}


/*---+---------------------------------------------------------------
     xMod_SDL_Octet
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet xMod_SDL_Octet(
  SDL_Octet  i,
  SDL_Octet  k )
#else
SDL_Octet xMod_SDL_Octet( i, k )
  SDL_Octet  i;
  SDL_Octet  k;
#endif
{
  if ( k == 0 ) {
    xSDLOpError("Mod operator in sort Octet", "Right operand is 0" );
    return 0;
  } else
    return (SDL_Octet)(i%k);
}


/*---+---------------------------------------------------------------
     xRem_SDL_Octet
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet xRem_SDL_Octet(
  SDL_Octet  i,
  SDL_Octet  k )
#else
SDL_Octet xRem_SDL_Octet( i, k )
  SDL_Octet  i;
  SDL_Octet  k;
#endif
{
  if ( k == 0 ) {
    xSDLOpError("Rem operator in sort Octet", "Right operand is 0" );
    return 0;
  } else
    return (SDL_Octet)(i%k);
}
#endif
       /* XEINTDIV */


/*---+---------------------------------------------------------------
     xBitStr_SDL_Octet
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet xBitStr_SDL_Octet (
  SDL_Charstring C )
#else
SDL_Octet xBitStr_SDL_Octet ( C )
  SDL_Charstring C;
#endif
{
  SDL_Octet Result;
  int len, i;

  len = xLength_SDL_Charstring(C);
  if (len > 8) {
#ifdef XECSOP
    xSDLOpError("BitStr in sort Octet",
                "An Octet should consist of not more than 8 characters 0 or 1." );
#endif
    return (SDL_Octet)0;
  }
  Result = 0;
  for (i=1; i<=len; i++) {
    Result <<= 1;
    if (C[i] == '1') {
      Result++;
    } else if (C[i] != '0') {
#ifdef XECSOP
      xSDLOpError("BitStr in sort Octet",
                  "An Octet should consist only of characters 0 or 1." );
#endif
      return (SDL_Octet)0;
    }
  }
  for ( ; i<=8; i++) {
    Result <<= 1;
  }
  return Result;
}


/*---+---------------------------------------------------------------
     xHexStr_SDL_Octet
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet xHexStr_SDL_Octet (
  SDL_Charstring C )
#else
SDL_Octet xHexStr_SDL_Octet ( C )
  SDL_Charstring C;
#endif
{
  SDL_Octet Result;

  if (xLength_SDL_Charstring(C) != 2) {
#ifdef XECSOP
    xSDLOpError("HexStr in sort Octet",
                "An Octet should consist of 2 HEX values." );
#endif
    return (SDL_Octet)0;
  }
  if ( '0' <= C[1] && C[1] <= '9') {
    Result = (SDL_Octet)(16 * ((int)(C[1]) - '0'));
  } else if ( 'A' <= C[1] && C[1] <= 'F') {
    Result = (SDL_Octet)(16 * ((int)(C[1]) - 'A' + 10));
  } else if ( 'a' <= C[1] && C[1] <= 'f') {
    Result = (SDL_Octet)(16 * ((int)(C[1]) - 'a' + 10));
  } else {
#ifdef XECSOP
    xSDLOpError("HexStr in sort Octet",
                "Illegal character in Charstring (not digit or a-f)." );
#endif
    return (SDL_Octet)0;
  }

  if ( '0' <= C[2] && C[2] <= '9') {
    Result += (SDL_Octet)(((int)(C[2]) - '0'));
  } else if ( 'A' <= C[2] && C[2] <= 'F') {
    Result += (SDL_Octet)(((int)(C[2]) - 'A' + 10));
  } else if ( 'a' <= C[2] && C[2] <= 'f') {
    Result += (SDL_Octet)(((int)(C[2]) - 'a' + 10));
  } else {
#ifdef XECSOP
    xSDLOpError("HexStr in sort Octet",
                "Illegal character in Charstring (not digit or a-f)." );
#endif
    return (SDL_Octet)0;
  }
  return Result;
}

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*** Octet_String **************************************************/

#ifndef XNOUSEOFOCTETBITSTRING

/*---+---------------------------------------------------------------
     yAddr_SDL_Octet_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet * yAddr_SDL_Octet_String(
  SDL_Octet_String *B,
  SDL_Integer       Index)
#else
SDL_Octet * yAddr_SDL_Octet_String( B, Index )
  SDL_Octet_String *B;
  SDL_Integer       Index;
#endif
{
  if ( (Index < 0) || (Index >= (*B).Length) ) {
#ifdef XECSOP
    xSDLOpError("Modify! in sort Octet_String",
                "Index out of bounds." );
#endif
    if ((*B).Bits != 0)
      return &((*B).Bits[0]);
    else
      return (SDL_Octet *)xAlloc((xptrint)1);  /* ??? */
  } else
    return &((*B).Bits[Index]);
}


/*---+---------------------------------------------------------------
     xExtr_SDL_Octet_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet xExtr_SDL_Octet_String(
  SDL_Octet_String  B,
  SDL_Integer       Index )
#else
SDL_Octet xExtr_SDL_Octet_String( B, Index )
  SDL_Octet_String  B;
  SDL_Integer     Index;
#endif
{
  SDL_Octet Result;
  if ( (Index < 0) || (Index >= B.Length) ) {
#ifdef XECSOP
    xSDLOpError("Extract! in sort Octet_String",
                "Index out of bounds." );
#endif
    Result = 0;
  } else
    Result = B.Bits[Index];
  if (! B.IsAssigned) xFree_SDL_Bit_String((void **)&B);
  return Result;
}


/*---+---------------------------------------------------------------
     xBitStr_SDL_Octet_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet_String xBitStr_SDL_Octet_String (
  SDL_Charstring C )
#else
SDL_Octet_String xBitStr_SDL_Octet_String ( C )
  SDL_Charstring C;
#endif
{
  SDL_Octet_String Result;
  int i, len;

  len = xLength_SDL_Charstring(C);
  Result.Length = len/8 + (len%8>0 ? 1 : 0);
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0; /* FBI July-1998 */
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)Result.Length);

  for (i=0; i<len; i++) {
    Result.Bits[i/8] <<= 1;
    if (C[i+1] == '1') {
      Result.Bits[i/8] += 1;
#ifdef XECSOP
    } else if (C[i+1] != '0') {
      xSDLOpError("BitStr in sort Octet_String",
                  "An Octet_String should consist of 0 and 1." );
#endif
    }
  }
  for ( ; i<8*Result.Length; i++)
    Result.Bits[i/8] <<= 1;

  return Result;
}


/*---+---------------------------------------------------------------
     xHexStr_SDL_Octet_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet_String xHexStr_SDL_Octet_String (
  SDL_Charstring C )
#else
SDL_Octet_String xHexStr_SDL_Octet_String ( C )
  SDL_Charstring C;
#endif
{
  SDL_Octet_String Result;
  int C_length, C_index, res;

  C_length = xLength_SDL_Charstring(C);
  Result.Length = C_length/2 + (C_length%2 == 1 ? 1 : 0);
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0; /* FBI July-1998 */
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)Result.Length);
  for (C_index=1; C_index<=C_length; C_index=C_index+2) {
    if ( '0' <= C[C_index] && C[C_index] <= '9') {
      res = 16 * ((int)(C[C_index]) - '0');
    } else if ( 'A' <= C[C_index] && C[C_index] <= 'F') {
      res = 16 * ((int)(C[C_index]) - 'A' + 10);
    } else if ( 'a' <= C[C_index] && C[C_index] <= 'f') {
      res = 16 * ((int)(C[C_index]) - 'a' + 10);
    } else {
#ifdef XECSOP
      xSDLOpError("HexStr in sort Octet_String",
                  "Illegal character in Charstring (not digit or a-f)." );
#endif
      res = 0;
    }

    if (C_index<C_length) {
      if ( '0' <= C[C_index+1] && C[C_index+1] <= '9') {
        res = res + ((int)(C[C_index+1]) - '0');
      } else if ( 'A' <= C[C_index+1] && C[C_index+1] <= 'F') {
        res = res + ((int)(C[C_index+1]) - 'A' + 10);
      } else if ( 'a' <= C[C_index+1] && C[C_index+1] <= 'f') {
        res = res + ((int)(C[C_index+1]) - 'a' + 10);
      } else {
#ifdef XECSOP
        xSDLOpError("HexStr in sort Octet_String",
                    "Illegal character in Charstring (not digit or a-f)." );
#endif
      }
    }
    Result.Bits[(C_index-1)/2] = (unsigned char)res;
  }
  return Result;
}


/*---+---------------------------------------------------------------
     xBit_String_SDL_Octet_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Bit_String xBit_String_SDL_Octet_String (
  SDL_Octet_String S )
#else
SDL_Bit_String xBit_String_SDL_Octet_String ( S )
  SDL_Octet_String S;
#endif
{
  SDL_Bit_String Result;
  int i;

  Result.Length = S.Length*8;
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0; 
  if (Result.Length == 0) {
    Result.Bits = 0;
    return Result;
  }
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)Result.Length);

  for (i=0; i<Result.Length; i=i+8) {
    Result.Bits[i]   = (unsigned char)( S.Bits[i/8]/128);
    Result.Bits[i+1] = (unsigned char)((S.Bits[i/8]/64)%2);
    Result.Bits[i+2] = (unsigned char)((S.Bits[i/8]/32)%2);
    Result.Bits[i+3] = (unsigned char)((S.Bits[i/8]/16)%2);
    Result.Bits[i+4] = (unsigned char)((S.Bits[i/8]/8)%2);
    Result.Bits[i+5] = (unsigned char)((S.Bits[i/8]/4)%2);
    Result.Bits[i+6] = (unsigned char)((S.Bits[i/8]/2)%2);
    Result.Bits[i+7] = (unsigned char)( S.Bits[i/8]%2);
  }
  return Result;
}

/*---+---------------------------------------------------------------
     xOctet_String_SDL_Octet_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Octet_String xOctet_String_SDL_Octet_String (
  SDL_Bit_String S )
#else
SDL_Octet_String xOctet_String_SDL_Octet_String ( S )
  SDL_Bit_String S;
#endif
{
  SDL_Octet_String Result;
  int i;

  Result.Length = S.Length/8 + (S.Length%8>0 ? 1 : 0);
  Result.IsAssigned = (xbool)0;
  Result.IsUsedInSignal = (xbool)0; 
  if (Result.Length == 0) {
    Result.Bits = 0;
    return Result;
  }
  Result.Bits = xAlloc_SDL_Bit_String((xptrint)Result.Length);

  for (i=0; i<S.Length; i++) {
    Result.Bits[i/8] <<= 1;
    Result.Bits[i/8] += S.Bits[i];
  }
  for ( ; i%8 != 0; i++) {
    Result.Bits[i/8] <<= 1;    /* Extending with 0, not 1 as Z105 */
  }
  return Result;
}

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*** Charstring ****************************************************/

#ifndef XNOUSEOFCHARSTRING

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

/*---+---------------------------------------------------------------
     xAss_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
void xAss_SDL_Charstring(
  SDL_Charstring *CVar,
  SDL_Charstring  CExpr,
  int             AssType )
#else
void xAss_SDL_Charstring( CVar, CExpr, AssType )
  SDL_Charstring *CVar;
  SDL_Charstring  CExpr;
  int             AssType;
#endif
{
  if (AssType == XASS) {
    if (*CVar != CExpr) {
      if (*CVar != (SDL_Charstring)0)
        xFree_SDL_Charstring((void **)CVar);
      if (CExpr == (SDL_Charstring)0)
        *CVar = (SDL_Charstring)0;
      else if (CExpr[0] == 'T') {
        CExpr[0] = 'V';
        *CVar = CExpr;
      } else {
       *CVar = xAlloc_SDL_Charstring((xptrint)(strlen(CExpr)+1));
        (void)strcpy(*CVar, CExpr);
        (*CVar)[0] = 'V';
      }
    }
  } else {
    if (CExpr == (SDL_Charstring)0)
      *CVar = (SDL_Charstring)0;
    else if (CExpr[0] == 'T' && AssType == XASSMAKE)
      *CVar = CExpr;
    else {
     *CVar = xAlloc_SDL_Charstring((xptrint)(strlen(CExpr)+1));
      (void)strcpy(*CVar, CExpr);
      (*CVar)[0] = 'T';
    }
  }
}

#else /* then XSCT_CMICRO */

#ifndef XRESTUSEOFCHARSTRING

#ifndef XNOPROTO
extern void xAss_SDL_Charstring(
  SDL_Charstring *CVar,
  SDL_Charstring  CExpr,
  int             AssType )
#else
extern void xAss_SDL_Charstring( CVar, CExpr, AssType )
  SDL_Charstring *CVar;
  SDL_Charstring  CExpr;
  int             AssType;
#endif
{
  SDL_Boolean Is_XASSPARA = SDL_False;

  if (AssType == XASSPARA)
  {
    /*
    ** Default setting of charstring variable - used in the
    ** case of output only (corrected by fbi at april-1997)
    */
    *CVar = (SDL_Charstring)0; /* set default for this variable */
                               /* needed for Cadvanced in tight */
                               /* integration and Cmicro in     */
                               /* general                       */
    Is_XASSPARA = SDL_True; /* to remember on that later */
    AssType = XASS ;
  }
 
  if (AssType == XASS) {
    if (*CVar != CExpr) {
      if (*CVar != (SDL_Charstring)0)
        xFree_SDL_Charstring((void **)CVar);
      if (CExpr == (SDL_Charstring)0)
        *CVar = (SDL_Charstring)0;
      else
        if ((CExpr[0] == 'T') || (CExpr[0] == 'P')) {
        /* assign the source pointer to dest.variable */
        /* and mark charstring variable with "V"      */ 
        CExpr[0] = 'V';
        *CVar = CExpr;
      } else {
       *CVar = xAlloc_SDL_Charstring((xptrint)(strlen(CExpr)+1));
        (void)strcpy(*CVar, CExpr);
        (*CVar)[0] = 'V';
        if (Is_XASSPARA)
          (*CVar)[0] = 'P'; /* if charstring as signalparameter in output, free this after reception */ 
      }
    }
  } else {
    if (AssType == XASSCOPY)
    {   
      /*
      ** Used for FPAR IN in a PRD when a local copy of IN par has to be created
      */
      if (CExpr == (SDL_Charstring)0)
      {
        /* if a null pointer value is sent down to procedure */
        *CVar = xAlloc_SDL_Charstring((xptrint)(2));
      }
      else
      {
        *CVar = xAlloc_SDL_Charstring((xptrint)(strlen(CExpr)+1));
        (void)strcpy(*CVar, CExpr);
      }
      (*CVar)[0] = 'V';
    }
    else
    {
      if (CExpr == (SDL_Charstring)0)
        *CVar = (SDL_Charstring)0;
      else if (CExpr[0] == 'T' && AssType == XASSMAKE)
      *CVar = CExpr;
      else {
       *CVar = xAlloc_SDL_Charstring((xptrint)(strlen(CExpr)+1));
        (void)strcpy(*CVar, CExpr);
        (*CVar)[0] = 'T';
        if (Is_XASSPARA)
          (*CVar)[0] = 'P'; /* if charstring as signalparameter in output, free this after reception */
      }
    }
  }
}

#endif /* ... XRESTUSEOFCHARSTRING */
 
#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) || (defined(XSCT_CMICRO) && ! defined(XRESTUSEOFCHARSTRING))

/*---+---------------------------------------------------------------
     xEq_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Boolean xEq_SDL_Charstring(
  SDL_Charstring C1,
  SDL_Charstring C2 )
#else
SDL_Boolean xEq_SDL_Charstring( C1, C2 )
  SDL_Charstring C1;
  SDL_Charstring C2;
#endif
{
  SDL_Boolean Result;

  if ( ((C1 == (SDL_Charstring)0) || (strlen(C1) <= (unsigned)1)) &&
       ((C2 == (SDL_Charstring)0) || (strlen(C2) <= (unsigned)1)) )
    Result = SDL_true;
  else if ( (C1 == (SDL_Charstring)0) || (C2 == (SDL_Charstring)0) )
    Result = SDL_false;
  else if (strcmp(C1+1, C2+1) == 0)
    Result = SDL_true;
  else
    Result = SDL_false;

  if (C1 != (SDL_Charstring)0 && C1[0] == 'T')
    xFree_SDL_Charstring((void **)&C1);
  if (C2 != (SDL_Charstring)0 && C2[0] == 'T')
    xFree_SDL_Charstring((void **)&C2);
  return Result;
}


/*---+---------------------------------------------------------------
     xMkString_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Charstring xMkString_SDL_Charstring(
  SDL_Character C )
#else
SDL_Charstring xMkString_SDL_Charstring( C )
  SDL_Character C;
#endif
{
  SDL_Charstring Result;

  if ( C == SDL_NUL ) {
#ifdef XECSOP
    xSDLOpError( "MkString in sort Charstring",
                 "Character NUL not allowed." );
#endif
    return (SDL_Charstring)0;
  }
  Result = xAlloc_SDL_Charstring((xptrint)(sizeof(C)+2));
  Result[0] = 'T';
  Result[1] = C;
  return Result;
}


/*---+---------------------------------------------------------------
     xMod_SDL_Charstring
-------------------------------------------------------------------*/

/* No longer used by generated code */

#ifndef XNOPROTO
SDL_Charstring xMod_SDL_Charstring(
  SDL_Charstring  C,
  SDL_Integer     Index,
  SDL_Character   Value )
#else
SDL_Charstring xMod_SDL_Charstring( C, Index, Value )
  SDL_Charstring  C;
  SDL_Integer     Index;
  SDL_Character   Value;
#endif
{
  if ( (C == (SDL_Charstring)0) || (Index<=0) || ((unsigned)Index>=strlen(C)) ) {
#ifdef XECSOP
    xSDLOpError("Modify! in sort Charstring", "Index out of bounds." );
#endif
    return C;
  }
  if ( Value == SDL_NUL ) {
#ifdef XECSOP
    xSDLOpError("Modify! in sort Charstring",
                "Character NUL not allowed." );
#endif
    return C;
  }
  C[Index] = Value;
  return C;
}


/*---+---------------------------------------------------------------
     yAddr_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Character * yAddr_SDL_Charstring(
  SDL_Charstring *C,
  SDL_Integer     Index)
#else
SDL_Character * yAddr_SDL_Charstring( C, Index )
  SDL_Charstring *C;
  SDL_Integer     Index;
#endif
{
  if ( (*C == (SDL_Charstring)0) || (Index<=0) ||
       ((unsigned)Index>=strlen(*C)) ) {
#ifdef XECSOP
    xSDLOpError("Modify! in sort Charstring",
                "Index out of bounds." );
#endif
    if (*C != (SDL_Charstring)0)
      return &((*C)[1]);
    else
      return (SDL_Character *)xAlloc((xptrint)2);  /* ??? */
  } else
    return &((*C)[Index]);
}


/*---+---------------------------------------------------------------
     xExtr_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Character xExtr_SDL_Charstring(
  SDL_Charstring  C,
  SDL_Integer     Index )
#else
SDL_Character xExtr_SDL_Charstring( C, Index )
  SDL_Charstring  C;
  SDL_Integer     Index;
#endif
{
  char Result;
  if ( (C == (SDL_Charstring)0) || (Index<=0) || ((unsigned)Index>=strlen(C)) ) {
#ifdef XECSOP
    xSDLOpError("Extract! in sort Charstring",
                "Index out of bounds." );
#endif
    Result = SDL_NUL;
  } else
    Result = C[Index];
  if (C != (SDL_Charstring)0 && C[0] == 'T')
    xFree_SDL_Charstring((void **)&C);
  return Result;
}


/*---+---------------------------------------------------------------
     xConcat_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Charstring xConcat_SDL_Charstring(
  SDL_Charstring  C1,
  SDL_Charstring  C2 )
#else
SDL_Charstring xConcat_SDL_Charstring( C1, C2 )
  SDL_Charstring  C1;
  SDL_Charstring  C2;
#endif
{
  SDL_Charstring Result;

  if (C1 == (SDL_Charstring)0) {
    if (C2 == (SDL_Charstring)0)
      return (SDL_Charstring)0;
    else if (C2[0] == 'T')
      return C2;
    else {
      Result = xAlloc_SDL_Charstring((xptrint)(strlen(C2)+1));
      (void)strcpy(Result, C2);
    }
  } else if (C2 == (SDL_Charstring)0) {
    if (C1[0] == 'T')
      return C1;
    else {
      Result = xAlloc_SDL_Charstring((xptrint)(strlen(C1)+1));
      (void)strcpy(Result, C1);
    }
  } else {
    Result = xAlloc_SDL_Charstring((xptrint)(strlen(C1)+strlen(C2)));
    (void)strcpy(Result, C1);
    (void)strcat(Result, C2+1);
    if (C1[0] == 'T')
      xFree_SDL_Charstring((void **)&C1);
    if (C2[0] == 'T')
      xFree_SDL_Charstring((void **)&C2);
  }

  Result[0] = 'T';
  return Result;
}


/*---+---------------------------------------------------------------
     xFirst_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Character xFirst_SDL_Charstring(
  SDL_Charstring  C )
#else
SDL_Character xFirst_SDL_Charstring( C )
  SDL_Charstring  C;
#endif
{
  char Result;
  if ( (C == (SDL_Charstring)0) || (strlen(C) <= (unsigned)1) ) {
#ifdef XECSOP
    xSDLOpError("First in sort Charstring",
                "Charstring length is zero." );
#endif
    Result = SDL_NUL;
  } else
    Result = C[1];
  if (C != (SDL_Charstring)0 && C[0] == 'T')
    xFree_SDL_Charstring((void **)&C);
  return Result;
}


/*---+---------------------------------------------------------------
     xLast_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Character xLast_SDL_Charstring(
  SDL_Charstring  C )
#else
SDL_Character xLast_SDL_Charstring( C )
  SDL_Charstring  C;
#endif
{
  char Result;
  if ( (C == (SDL_Charstring)0) || (strlen(C) <= (unsigned)1) ) {
#ifdef XECSOP
    xSDLOpError("Last in sort Charstring",
                "Charstring length is zero." );
#endif
    Result = SDL_NUL;
  } else
    Result = C[strlen(C) - 1];
  if (C != (SDL_Charstring)0 && C[0] == 'T')
    xFree_SDL_Charstring((void **)&C);
  return Result;
}


/*---+---------------------------------------------------------------
     xLength_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Integer xLength_SDL_Charstring(
  SDL_Charstring  C )
#else
SDL_Integer xLength_SDL_Charstring( C )
  SDL_Charstring  C;
#endif
{
  int Length;
  if (C == (SDL_Charstring)0)
    Length = 0;
  else {
    Length = strlen(C)-1;
    if (C[0] == 'T')
      xFree_SDL_Charstring((void **)&C);
  }
  return Length;
}


/*---+---------------------------------------------------------------
     xSubString_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Charstring xSubString_SDL_Charstring(
  SDL_Charstring  C,
  SDL_Integer     Start,
  SDL_Integer     SubLength )
#else
SDL_Charstring xSubString_SDL_Charstring( C, Start, SubLength )
  SDL_Charstring  C;
  SDL_Integer     Start;
  SDL_Integer     SubLength;
#endif
{
  SDL_Charstring Result;

  if ( (C == (SDL_Charstring)0) || (strlen(C) <= (unsigned)1) ) {
#ifdef XECSOP
    xSDLOpError("SubString in sort Charstring",
                "Charstring length is zero." );
#endif
    Result = (SDL_Charstring)0;
  } else if ( Start <= 0 ) {
#ifdef XECSOP
    xSDLOpError("SubString in sort Charstring",
                "Start is less than or equal to zero." );
#endif
    Result = (SDL_Charstring)0;
  } else if ( SubLength < 0 ) {
#ifdef XECSOP
    xSDLOpError("SubString in sort Charstring",
                "SubLength is less than zero." );
#endif
    Result = (SDL_Charstring)0;
  } else if ( SubLength == 0 ) {    /* No error if length is 0 */
    Result = (SDL_Charstring)0;
  } else if ( (unsigned)(Start+SubLength) > strlen(C)) { 
#ifdef XECSOP
    xSDLOpError("SubString in sort Charstring",
                "Start + Substring length is greater than string length." );
#endif
    Result = (SDL_Charstring)0;
  } else {
    Result = xAlloc_SDL_Charstring((xptrint)(SubLength + 2));
/*  Modified by GBU,000103  added cast since "SDL_Integer" and "size_t" may differ in size */
/*
    (void)strncpy(Result+1, (SDL_Charstring)((xptrint)C + Start), SubLength);
*/
    (void)strncpy(Result+1, (SDL_Charstring)((xptrint)C + Start), (size_t)SubLength);
    Result[0] = 'T';
    Result[SubLength+1] = '\0';
  }
  if (C != (SDL_Charstring)0 && C[0] == 'T')
    xFree_SDL_Charstring((void **)&C);
  return Result;
}

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) && (defined(XSCT_CMICRO) && ! defined(XRESTUSEOFCHARSTRING)) */

#endif 
   /* XNOUSEOFCHARSTRING */

/*** Duration ******************************************************/

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) || defined(XMK_USE_STANDARD_DURATION)

/*---+---------------------------------------------------------------
     xPlus_SDL_Duration
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Duration xPlus_SDL_Duration(
  SDL_Duration D1,
  SDL_Duration D2 )
#else
SDL_Duration xPlus_SDL_Duration( D1, D2 )
  SDL_Duration D1;
  SDL_Duration D2;
#endif
{
  D1.s = D1.s + D2.s;
  D1.ns = D1.ns + D2.ns;
#ifdef X_XINT32_INT
  if ( ((D1.s >= 0) && (D1.ns >= 1000000000)) ||
       ((D1.s < 0) && (D1.ns > 0)) ) {
    D1.ns = D1.ns - 1000000000;
    D1.s = D1.s + 1;
  } else if
     ( ((D1.s > 0) && (D1.ns < 0)) ||
       ((D1.s <= 0) && (D1.ns <= -1000000000)) ) {
    D1.ns = D1.ns + 1000000000;
    D1.s = D1.s - 1;
  }
#else
  if ( ((D1.s >= 0L) && (D1.ns >= 1000000000L)) ||
       ((D1.s < 0L) && (D1.ns > 0L)) ) {
    D1.ns = D1.ns - 1000000000L;
    D1.s = D1.s + 1L;
  } else if
     ( ((D1.s > 0L) && (D1.ns < 0L)) ||
       ((D1.s <= 0L) && (D1.ns <= -1000000000L)) ) {
    D1.ns = D1.ns + 1000000000L;
    D1.s = D1.s - 1L;
  }
#endif
  return D1;
}


/*---+---------------------------------------------------------------
     xMonMinus_SDL_Duration
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Duration xMonMinus_SDL_Duration(
  SDL_Duration D )
#else
SDL_Duration xMonMinus_SDL_Duration( D )
  SDL_Duration D;
#endif
{
  D.s = -D.s;
  D.ns = -D.ns;
  return D;
}


#ifndef XNOUSEOFREAL
/*---+---------------------------------------------------------------
     xMult_SDL_Duration
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Duration xMult_SDL_Duration(
  SDL_Duration D,
  SDL_Real     R )
#else
SDL_Duration xMult_SDL_Duration( D, R )
  SDL_Duration D;
  SDL_Real     R;
#endif
{
  SDL_Real DTemp;
  DTemp = ( (SDL_Real)D.s + (SDL_Real)D.ns*(SDL_Real)1E-9 ) * R;
  if ( DTemp >= 2.147483647E9 ) {
    /* Overflow */
    D.s = 0;
    D.ns = 0;
  } else {
    D.s = (xint32)DTemp;
    D.ns = (xint32)((DTemp-(SDL_Real)D.s)*1E9);
  }
  return D;
}


/*---+---------------------------------------------------------------
     xDiv_SDL_Duration
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Duration xDiv_SDL_Duration(
  SDL_Duration D,
  SDL_Real     R )
#else
SDL_Duration xDiv_SDL_Duration( D, R )
  SDL_Duration D;
  SDL_Real     R;
#endif
{
  SDL_Real DTemp;
  DTemp = ( (SDL_Real)D.s + (SDL_Real)D.ns*(SDL_Real)1E-9 ) / R;
  if ( DTemp >= 2.147483647E9 ) {
    /* Overflow */
    D.s = 0;
    D.ns = 0;
  } else {
    D.s = (xint32)DTemp;
    D.ns = (xint32)((DTemp-(SDL_Real)D.s)*1E9);
  }
  return D;
}

#endif
       /* ! XNOUSEOFREAL */


/*---+---------------------------------------------------------------
     xEq_SDL_Duration
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Boolean xEq_SDL_Duration(
  SDL_Duration D1,
  SDL_Duration D2 )
#else
SDL_Boolean xEq_SDL_Duration( D1, D2 )
  SDL_Duration D1;
  SDL_Duration D2;
#endif
{
  if ( (D1.s == D2.s) && (D1.ns == D2.ns) ) return SDL_True;
  return SDL_False;
}


/*---+---------------------------------------------------------------
     SDL_Duration_Lit
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Duration SDL_Duration_Lit(
  xint32 s,
  xint32 ns )
#else
SDL_Duration SDL_Duration_Lit( s, ns )
  xint32 s;
  xint32 ns;
#endif
{
  SDL_Duration D;
  D.s = s;
  D.ns = ns;
  return D;
}

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) || defined(XMK_USE_STANDARD_DURATION) */

/*** Integer *******************************************************/

#if !defined(XNOUSEOFREAL) && defined(XEFIXOF)
/*---+---------------------------------------------------------------
     xFix_SDL_Integer
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Integer xFix_SDL_Integer(
  SDL_Real     Re )
#else
SDL_Integer xFix_SDL_Integer( Re )
  SDL_Real     Re;
#endif
{
  if ( Re >= (SDL_Real)(XSYSD xMax_SDL_Integer)) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xSDLOpError("Fix in sort Integer", "Integer overflow.");
#else
    ErrorHandler (ERR_N_SDL_RANGE);
#endif
    return 0;
  } else
    return (SDL_Integer)Re;
}
#else
/* See sctpred.h #define xFix_SDL_Integer() */
#endif
       /* !XNOUSEOFREAL && XEFIXOF */


/*---+---------------------------------------------------------------
     xDiv_SDL_Integer
-------------------------------------------------------------------*/
#ifdef XEINTDIV
#ifndef XNOPROTO
SDL_Integer xDiv_SDL_Integer(
  SDL_Integer  i,
  SDL_Integer  k )
#else
SDL_Integer xDiv_SDL_Integer( i, k )
  SDL_Integer  i;
  SDL_Integer  k;
#endif
{
  if ( k == 0 ) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xSDLOpError("Division in sort Integer", "Integer division with 0" );
#else
    ErrorHandler (ERR_N_DIVIDE_BY_ZERO);
#endif
    return 0;
  } else
    return i/k;
}
/* #else
   See scttypes.h #define xDiv_SDL_Integer ... */
#endif
       /* XEINTDIV */


/*---+---------------------------------------------------------------
     xRem_SDL_Integer
-------------------------------------------------------------------*/
#ifdef XEINTDIV
#ifndef XNOPROTO
SDL_Integer xRem_SDL_Integer(
  SDL_Integer  i,
  SDL_Integer  k )
#else
SDL_Integer xRem_SDL_Integer( i, k )
  SDL_Integer  i;
  SDL_Integer  k;
#endif
{
  if ( k == 0 ) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xSDLOpError("Rem in sort Integer", "Second operand is 0" );
#else
    ErrorHandler (ERR_N_DIVIDE_BY_ZERO);
#endif
    return 0;
  } else
    return i%k;
}
/* #else
   See scttypes.h #define xRem_SDL_Integer ... */
#endif
       /* XEINTDIV */


/*---+---------------------------------------------------------------
     xMod_SDL_Integer
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Integer xMod_SDL_Integer(
  SDL_Integer  i,
  SDL_Integer  k )
#else
SDL_Integer xMod_SDL_Integer( i, k )
  SDL_Integer  i;
  SDL_Integer  k;
#endif
{
  SDL_Integer result;
  if ( k == 0 ) {
#if defined(XEINTDIV) || defined(XMK_USE_MAX_ERR_CHECK)
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xSDLOpError("Mod in sort Integer", "Second operand is 0" );
#else
    ErrorHandler (ERR_N_DIVIDE_BY_ZERO);
#endif
#endif /* defined(XEINTDIV) || defined(XMK_USE_MAX_ERR_CHECK) */
    return 0;
  } else {
/*  Modified by GBU,000103  added cast since "SDL_Integer" and "int" may differ in size */
/*
    k = abs(k);
*/
    k = abs((int)k);
    result = i%k;
    if (result < 0) result = result+k;
    return result;
  }
}


/*** Natural *******************************************************/

#ifdef XTESTF
/*---+---------------------------------------------------------------
     xTest_SDL_Natural
-------------------------------------------------------------------*/
#ifndef XNOPROTO
xbool xTest_SDL_Natural(
  void *N )
#else
xbool xTest_SDL_Natural( N )
  void *N;
#endif
{
/*  Removed by GBU,991011   unsigned is always >= 0
  if ( *(SDL_Natural *)N < 0 )
    return (xbool)0;
*/
  /*    Added by GBU,991011 to avoid warnings for unused variable */
  N = N;

  return (xbool)1;
}
#endif


#ifdef XERANGE
/*---+---------------------------------------------------------------
     xTstA_SDL_Natural
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Natural xTstA_SDL_Natural(
  SDL_Natural  N )
#else
SDL_Natural xTstA_SDL_Natural( N )
  SDL_Natural  N;
#endif
{
/*  Removed by GBU,991011   unsigned is always >= 0
  if ( N < 0 ) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xErrorSubrange(xSrtN_SDL_Natural, xWri_SDL_Integer(&N));
#else
    ErrorHandler (ERR_N_SDL_RANGE);
#endif
  }
*/
  return N;
}
#endif


#ifdef XEINDEX
/*---+---------------------------------------------------------------
     xTstI_SDL_Natural
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Natural xTstI_SDL_Natural(

  SDL_Natural  N )
#else
SDL_Natural xTstI_SDL_Natural( N )
  SDL_Natural  N;
#endif
{
/*  Removed by GBU,991011   unsigned is always >= 0
  if ( N < 0 ) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xErrorIndex(xSrtN_SDL_Natural, xWri_SDL_Integer(&N));
#else
    ErrorHandler (ERR_N_SDL_RANGE);
#endif
  }
*/
  return N;
}
#endif


/*** NumericString *************************************************/

#ifndef XNOUSEOFOCTETBITSTRING

#ifdef XTESTF
/*---+---------------------------------------------------------------
     xTest_SDL_NumericString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
xbool xTest_SDL_NumericString(
  void *N )
#else
xbool xTest_SDL_NumericString( N )
  void *N;
#endif
{
  int i;
  if (N != 0) {
    for (i=1; (*(SDL_NumericString *)N)[i] != '\0'; i++)
      if ( ( (*(SDL_NumericString *)N)[i] != ' ' ) &&
           ( ( (*(SDL_NumericString *)N)[i] < '0' ) ||
             ( (*(SDL_NumericString *)N)[i] > '9' )
           )
         )
        return (xbool)0;
  }
  return (xbool)1;
}
#endif


#ifdef XERANGE
/*---+---------------------------------------------------------------
     xTstA_SDL_NumericString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_NumericString xTstA_SDL_NumericString(
  SDL_NumericString  N )
#else
SDL_NumericString xTstA_SDL_NumericString( N )
  SDL_NumericString  N;
#endif
{
  if ( ! xTest_SDL_NumericString((void *)&N) ) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xErrorSubrange(xSrtN_SDL_NumericString, xWri_SDL_Charstring(&N));
#else
    ErrorHandler (ERR_N_SDL_RANGE);
#endif
  }
  return N;
}
#endif


#ifdef XEINDEX
/*---+---------------------------------------------------------------
     xTstI_SDL_NumericString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_NumericString xTstI_SDL_NumericString(
  SDL_NumericString  N )
#else
SDL_NumericString xTstI_SDL_NumericString( N )
  SDL_NumericString  N;
#endif
{
  if ( ! xTest_SDL_NumericString((void *)&N) ) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xErrorIndex(xSrtN_SDL_NumericString, xWri_SDL_Charstring(&N));
#else
    ErrorHandler (ERR_N_SDL_RANGE);
#endif
  }
  return N;
}
#endif


/*** PrintableString ***********************************************/

#ifdef XTESTF
/*---+---------------------------------------------------------------
     xTest_SDL_PrintableString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
xbool xTest_SDL_PrintableString(
  void *N )
#else
xbool xTest_SDL_PrintableString( N )
  void *N;
#endif
{
  int i;
  SDL_PrintableString C;
  if (N != 0) {
    C = *(SDL_PrintableString *)N;
    for (i=1; C[i] != '\0'; i++)
      if ( ( ( C[i] < 'A' ) || ( C[i] > 'Z' ) ) &&
           ( ( C[i] < 'a' ) || ( C[i] > 'z' ) ) &&
           ( ( C[i] < '0' ) || ( C[i] > '9' ) ) &&
           ( C[i] != '\'' ) && ( C[i] != '(' ) && ( C[i] != ')' ) &&
           ( C[i] != '+' ) && ( C[i] != ',' ) && ( C[i] != '-' ) && 
           ( C[i] != '.' ) && ( C[i] != '/' ) && ( C[i] != ':' ) && 
           ( C[i] != '=' ) && ( C[i] != '?' )
         )
        return (xbool)0;
  }
  return (xbool)1;
}
#endif


#ifdef XERANGE
/*---+---------------------------------------------------------------
     xTstA_SDL_PrintableString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_PrintableString xTstA_SDL_PrintableString(
  SDL_PrintableString  N )
#else
SDL_PrintableString xTstA_SDL_PrintableString( N )
  SDL_PrintableString  N;
#endif
{
  if ( ! xTest_SDL_PrintableString((void *)&N) ) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xErrorSubrange(xSrtN_SDL_PrintableString, xWri_SDL_Charstring(&N));
#else
    ErrorHandler (ERR_N_SDL_RANGE);
#endif
  }
  return N;
}
#endif


#ifdef XEINDEX
/*---+---------------------------------------------------------------
     xTstI_SDL_PrintableString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_PrintableString xTstI_SDL_PrintableString(
  SDL_PrintableString  N )
#else
SDL_PrintableString xTstI_SDL_PrintableString( N )
  SDL_PrintableString  N;
#endif
{
  if ( ! xTest_SDL_PrintableString((void *)&N) ) {
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
    xErrorIndex(xSrtN_SDL_PrintableString, xWri_SDL_Charstring(&N));
#else
    ErrorHandler (ERR_N_SDL_RANGE);
#endif
  }
  return N;
}
#endif

#endif /* ... XNOUSEOFOCTETBITSTRING */


/*** Object_Identifier *********************************************/


#ifndef NO_GLOBAL_VARS

#ifndef XNOUSEOFOBJECTIDENTIFIER
String_Assign(SDL_Object_Identifier,SDL_Natural)
String_Default(SDL_Object_Identifier,SDL_Natural)
String_Equal(SDL_Object_Identifier,SDL_Natural)
#ifdef XREADANDWRITEF
/*String_Write(SDL_Object_Identifier,SDL_Natural)*/

#ifndef XNOPROTO
char * yWri_SDL_Object_Identifier(void * S)
#else
char * yWri_SDL_Object_Identifier(S)
  void * S;
#endif
{
  SDL_Object_Identifier_yptr Temp, TempLast;
  #error "warning_static_in_use_DLH"
  static char CTemp[MAX_WRI_LENGTH];

  if (XASN1_SYNTAX)
    (void)strcpy(CTemp, "{ ");
  else
    (void)strcpy(CTemp, "(. ");
  TempLast = (*(SDL_Object_Identifier *)S).Last;
  for (Temp = (*(SDL_Object_Identifier *)S).First;
       Temp != (SDL_Object_Identifier_yptr)0;
       Temp = Temp->Suc) {
    xSafeStrcat(CTemp,
           xWriteSort((void *)(&Temp->Data), ySrtN_SDL_Natural));
    if (strlen(CTemp) == MAX_WRI_LENGTH - 1)
      return CTemp;
    if (Temp != TempLast) {
      if (! XASN1_SYNTAX) xSafeStrcat(CTemp, ",");
      xSafeStrcat(CTemp, " ");
    }
  }
  if (XASN1_SYNTAX)
    xSafeStrcat(CTemp, " }");
  else
    xSafeStrcat(CTemp, " .)");
  return CTemp;
}




String_Read(SDL_Object_Identifier,SDL_Natural)
#endif
String_Free(SDL_Object_Identifier,SDL_Natural,XXFREE)

String_Emptystring(SDL_Object_Identifier,SDL_Natural)
String_MkString(SDL_Object_Identifier,SDL_Natural)
String_Length(SDL_Object_Identifier,SDL_Natural)
String_First(SDL_Object_Identifier,SDL_Natural)
String_Last(SDL_Object_Identifier,SDL_Natural)
String_Concat(SDL_Object_Identifier,SDL_Natural)
String_Substring(SDL_Object_Identifier,SDL_Natural)
String_Modify(SDL_Object_Identifier,SDL_Natural)
String_Extract(SDL_Object_Identifier,SDL_Natural)
#endif       /* XNOUSEOFOBJECTIDENTIFIER */
#endif  /*NO_GLOBAL_VARS*/
/*** Real **********************************************************/

#ifndef XNOUSEOFREAL
/*---+---------------------------------------------------------------
     xDiv_SDL_Real
-------------------------------------------------------------------*/
#ifdef XEREALDIV
#ifndef XNOPROTO
SDL_Real xDiv_SDL_Real(
  SDL_Real     i,
  SDL_Real     k )
#else
SDL_Real xDiv_SDL_Real( i, k )
  SDL_Real     i;
  SDL_Real     k;
#endif
{
  if ( k == 0 ) {
    xSDLOpError("Division in sort Real", "Real division with 0" );
    return 0;
  } else
    return i/k;
}
/* #else
   See sctpred.h #define xDiv_SDL_Real ... */
#endif
       /* XEREALDIV */
#endif
       /* XNOUSEOFREAL */


/*** Time **********************************************************/

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) || defined(XMK_USE_STANDARD_TIME)

/*---+---------------------------------------------------------------
     xEq_SDL_Time
-------------------------------------------------------------------*/
/* Using xEq_SDL_Duration, see macro in sctpred.h */


/*---+---------------------------------------------------------------
     xLT_SDL_Time
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Boolean xLT_SDL_Time(
  SDL_Time T1,
  SDL_Time T2 )
#else
SDL_Boolean xLT_SDL_Time( T1, T2 )
  SDL_Time T1;
  SDL_Time T2;
#endif
{
  if ( T1.s < T2.s ) return SDL_True;
  if ( (T1.s == T2.s) && (T1.ns < T2.ns) ) return SDL_True;
  return SDL_False;
}


/*---+---------------------------------------------------------------
     xLE_SDL_Time
-------------------------------------------------------------------*/
#ifndef XNOPROTO
SDL_Boolean xLE_SDL_Time(
  SDL_Time T1,
  SDL_Time T2 )
#else
SDL_Boolean xLE_SDL_Time( T1, T2 )
  SDL_Time T1;
  SDL_Time T2;
#endif
{
  if ( T1.s < T2.s ) return SDL_True;
  if ( (T1.s == T2.s) && (T1.ns <= T2.ns) ) return SDL_True;
  return SDL_False;
}

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) || defined(XMK_USE_STANDARD_TIME) */


/****+***************************************************************
03   Read and Write functions
********************************************************************/


/****+***************************************************************
03   Read and Write functions
********************************************************************/
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#ifdef XREADANDWRITEF

#define TIMEVALUEDECIMALS 4
        /* Number of decimals given normally when time or duration printed */
#error "warning_static_in_use_DLH"
static char * xfCharacterLiteral[] = {
    "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
    "BS",  "HT",  "LF",  "VT",  "FF",  "CR",  "SO",  "SI",
    "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
    "CAN", "EM",  "SUB", "ESC", "IS4", "IS3", "IS2", "IS1",
    "DEL"  /* 127 is coded as 32 */
};


/*---+---------------------------------------------------------------
     xReadSDL_Time
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xReadSDL_Time(
  SDL_Time *T)
#else
int xReadSDL_Time(T)
  SDL_Time *T;
#endif
{
  xxToken Token;
  char    strVar[256];
  char   *TempStr;
  int     Pos;
  xbool   GotSomething = (xbool)0;
  xbool   Plus, Minus;

  (*T).s = (xint32)0;
  (*T).ns = (xint32)0;
  Token = xScanToken(strVar);
  Plus = (xbool)0;
  Minus = (xbool)0;
  if (Token == xxPlus) {
    Plus = (xbool)1;
    Token = xScanToken(strVar);
  } else if (Token == xxMinus) {
    Minus = (xbool)1;
    Token = xScanToken(strVar);
  }
  if (Token != xxId) {
    xPrintString("Illegal syntax\n");
    return 0;
  }
  TempStr = strVar;
  while (*TempStr >= '0' && *TempStr <= '9') {
    GotSomething = (xbool)1;
    if ((*T).s > XSYSD xMaxTime.s / 10) {
      xPrintString("Integer overflow\n");
      return 0;                        /* overflow */
    }
    if ((*T).s * 10 > XSYSD xMaxTime.s - (*TempStr - '0')) {
      xPrintString("Integer overflow\n");
      return 0;                        /* overflow */
    }
    (*T).s = (*T).s * 10 + (*TempStr - '0');
    TempStr++;
  }
  if (*TempStr == '.') {
    TempStr++;
    for (Pos = 1; Pos <= 9; Pos++) {
      (*T).ns *= 10;
      if (*TempStr >= '0' && *TempStr <= '9') {
        GotSomething = (xbool)1;
        (*T).ns += (*TempStr - '0');
        TempStr++;
      }
    }
  }
  if (GotSomething) {
#if defined(XMONITOR) && ! defined(XCALENDERCLOCK) && !defined(XVALIDATOR_LIB)
    if (Minus)
      *T = xMinusD_SDL_Time(XSYSD NowInMonitor, *T);
    else if (Plus)
      *T = xPlus_SDL_Time(XSYSD NowInMonitor, *T);
#else
    if (Minus)
      *T = xMinusD_SDL_Time(SDL_Now(), *T);
    else if (Plus)
      *T = xPlus_SDL_Time(SDL_Now(), *T);
#endif
    return 1;
  } else {
    xPrintString("Illegal syntax\n");
    return 0;
  }
}


/*---+---------------------------------------------------------------
     xWriteSDL_Time
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char * xWriteSDL_Time(
  SDL_Time T,
  int      decimals)
#else
char * xWriteSDL_Time(T, decimals)
  SDL_Time T;
  int      decimals;
#endif
{
  char NS[10];
  xbool Negative = (xbool)0;

  if (T.s < 0 || T.ns < 0) {
    T.s = -T.s;
    T.ns = -T.ns;
    Negative = (xbool)1;
  }
#ifdef X_XINT32_INT
  if (decimals == 0)
    T.ns += 500000000;
  else if (decimals == 1)
    T.ns += 50000000;
  else if (decimals == 2)
    T.ns += 5000000;
  else if (decimals == 3)
    T.ns += 500000;
  else if (decimals == 4)
    T.ns += 50000;
  else if (decimals == 5)
    T.ns += 5000;
  else if (decimals == 6)
    T.ns += 500;
  else if (decimals == 7)
    T.ns += 50;
  else if (decimals == 8)
    T.ns += 5;
  else if (decimals != 9)
    decimals = 9;
  if (T.ns > 1000000000) {
    T.ns -= 1000000000;
    T.s++;
  }
  (void)sprintf(NS, "%09d", T.ns);
  NS[decimals] = '\0';
  if (Negative)
    (void)sprintf(XSYSD CTempTemp, "-%d.%s", T.s, NS);
  else
    (void)sprintf(XSYSD CTempTemp, "%d.%s", T.s, NS);
#else
  if (decimals == 0)
    T.ns += 500000000L;
  else if (decimals == 1)
    T.ns += 50000000L;
  else if (decimals == 2)
    T.ns += 5000000L;
  else if (decimals == 3)
    T.ns += 500000L;
  else if (decimals == 4)
    T.ns += 50000L;
  else if (decimals == 5)
    T.ns += 5000L;
  else if (decimals == 6)
    T.ns += 500L;
  else if (decimals == 7)
    T.ns += 50L;
  else if (decimals == 8)
    T.ns += 5L;
  else if (decimals != 9)
    decimals = 9;
  if (T.ns > 1000000000L) {
    T.ns -= 1000000000L;
    T.s++;
  }
  (void)sprintf(NS, "%09ld", T.ns);
  NS[decimals] = '\0';
  if (Negative)
    (void)sprintf(XSYSD CTempTemp, "-%ld.%s", T.s, NS);
  else
    (void)sprintf(XSYSD CTempTemp, "%ld.%s", T.s, NS);
#endif
  return XSYSD CTempTemp;
}


/*---+---------------------------------------------------------------
     xRead_SDL_Bit
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_Bit(
  void  * Result)
#else
int xRead_SDL_Bit( Result )
  void  * Result;
#endif
{
  xxToken Token;
  char    strVar[256];
  char    strVar2[256];
  int     i;
  
  Token = xScanToken(strVar);
  if (Token==xxString) {
    /* ASN.1 syntax */
    /* Check 'B' */
    Token = xScanToken(strVar2);
    if ( Token != xxId || strlen(strVar2) != 1 || 
        ( strVar2[0] != 'b' && strVar2[0] != 'B' )
        ) {
      xPrintString("Bit value should end with 'B'\n");
      return 0;
    }
    /* Assign result */
    if (strlen(strVar)==3) {
      if (strVar[1] == '0') {
        *(SDL_Bit *)Result = 0;
        return 1;
      } else if (strVar[1] == '1') {
        *(SDL_Bit *)Result = 1;
        return 1;
      } else {
        xPrintString("Bit value should should be '0' or '1'\n");
        return 0;
      }
    } else {
      xPrintString("Bit length should be 1\n");
      return 0;
    }
  } else {
    /* SDL syntax */
    xUngetToken( Token, strVar );
    Token = xPromptQuestionMark(" (bit) : ",
                                " (bit) : ", strVar);
    if ( (Token == xxId) && (sscanf(strVar, "%d", &i) >= 1) &&
        (i==0 || i==1) ) {
      *(SDL_Bit *)Result = (SDL_Bit)i;
      return 1;
    }
    xPrintString("Bit value should be 0 or 1\n");
    return 0;
  }
}


/*---+---------------------------------------------------------------
     xWri_SDL_Bit
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Bit(
  void  * C)
#else
char *xWri_SDL_Bit( C )
  void  * C;
#endif
{
#ifdef XITEXCOMM
  if ( XSYSD xUse_Itex_Syntax ) {
    if ( *(SDL_Bit *)C ) return "'1'B";
    return "'0'B";
  }
  else
#endif
  if (XASN1_SYNTAX)  {
    if ( *(SDL_Bit *)C ) return "'1'B";
    return "'0'B";
  } else {
    if ( *(SDL_Bit *)C ) return "1";
    return "0";
  }
}


/*---+---------------------------------------------------------------
     xRead_SDL_Bit_String
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

#ifndef XNOPROTO
int xRead_SDL_Bit_String(
  void  * Result)
#else
int xRead_SDL_Bit_String( Result )
  void  * Result;
#endif
{
  xxToken Token;
  char    strVar[MAX_READ_LENGTH];
  char    strVar2[256];
  int     i, len, value;
  
  Token = xPromptQuestionMark(" (bit_string) : ",
     " (bit_string) : ", strVar);
  if (Token != xxString) {
    xPrintString("Bit_String value should enclosed in '\n");
    return 0;
  }
  Token = xScanToken(strVar2);
  if ( Token != xxId || strlen(strVar2) != 1 || 
       ( strVar2[0] != 'b' && strVar2[0] != 'B' &&
         strVar2[0] != 'h' && strVar2[0] != 'H'
       )
     ) {
    xPrintString("Bit_String value should end with B or H\n");
    return 0;
  }
  if (strVar2[0] == 'b' || strVar2[0] == 'B') {
    len = strlen(strVar);
    (*(SDL_Bit_String *)Result).Length = len-2;
    (*(SDL_Bit_String *)Result).IsAssigned = (xbool)1;
    (*(SDL_Bit_String *)Result).Bits = xAlloc_SDL_Bit_String((xptrint)(len-2));
    for (i=1; i<len-1; i++) {
      if (strVar[i] == '0')
        (*(SDL_Bit_String *)Result).Bits[i-1] = 0;
      else if (strVar[i] == '1')
        (*(SDL_Bit_String *)Result).Bits[i-1] = 1;
      else {
        xPrintString("Only 0 and 1 allowed in Bit_String value with ''B format\n");
        return 0;
      }
    }
  } else {
    len = strlen(strVar)-2;
    (*(SDL_Bit_String *)Result).Length = 4*len;
    (*(SDL_Bit_String *)Result).IsAssigned = (xbool)1;
    (*(SDL_Bit_String *)Result).Bits = xAlloc_SDL_Bit_String((xptrint)(4*len));
    for (i=1; i<=len; i++) {
      value = (int)(strVar[i]) - 'a' + 10;
      if (value < 10) value = (int)(strVar[i]) - 'A' + 10;
      if (value < 10) value = (int)(strVar[i]) - '0';
      if (value < 0 || value > 15) {
        xPrintString("Only 0-9, A-F allowed in Bit_String value with ''H format\n");
        return 0;
      }
      (*(SDL_Bit_String *)Result).Bits[(i-1)*4] = (unsigned char)(value/8);
      (*(SDL_Bit_String *)Result).Bits[(i-1)*4+1] =(unsigned char)((value/4)%2);
      (*(SDL_Bit_String *)Result).Bits[(i-1)*4+2] =(unsigned char)((value/2)%2);
      (*(SDL_Bit_String *)Result).Bits[(i-1)*4+3] = (unsigned char)(value%2);
    }
  }
  return 1;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Bit_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Bit_String(
  void  * C)
#else
char *xWri_SDL_Bit_String( C )
  void  * C;
#endif
{
  int i;
  XSYSD CTempTemp[0] = '\'';
  for (i=0; i<(*(SDL_Bit_String *)C).Length; i++)
    XSYSD CTempTemp[i+1] = (*(SDL_Bit_String *)C).Bits[i] ? '1' : '0';
  XSYSD CTempTemp[i+1] = '\'';
  XSYSD CTempTemp[i+2] = 'B';
  XSYSD CTempTemp[i+3] = '\0';
  return XSYSD CTempTemp;
}

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*---+---------------------------------------------------------------
     xRead_SDL_Octet
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

#ifndef XNOPROTO
int xRead_SDL_Octet(
  void  * Result)
#else
int xRead_SDL_Octet( Result )
  void  * Result;
#endif
{
  xxToken   Token;
  char      strVar[256];
  char      strVar2[256];
  unsigned  temp;
  SDL_Octet res;
  
  Token = xScanToken(strVar);
  if (Token==xxString) {
    /* ASN.1 syntax */
    /* Check 'H' */
    Token = xScanToken(strVar2);
    if ( Token != xxId || strlen(strVar2) != 1 || 
        ( strVar2[0] != 'h' && strVar2[0] != 'H' &&
          strVar2[0] != 'o' && strVar2[0] != 'O' )
        ) {
      xPrintString("Octet value should end with 'H'\n");
      return 0;
    }
    /* Assign result */
    if (strlen(strVar)==4) {
      if ( '0' <= strVar[1] && strVar[1] <= '9') {
        res = (SDL_Octet)(16 * ((int)(strVar[1]) - '0'));
      } else if ( 'A' <= strVar[1] && strVar[1] <= 'F') {
        res = (SDL_Octet)(16 * ((int)(strVar[1]) - 'A' + 10));
      } else if ( 'a' <= strVar[1] && strVar[1] <= 'f') {
        res = (SDL_Octet)(16 * ((int)(strVar[1]) - 'a' + 10));
      } else {
        xPrintString("Octet value should consist of HEX values: 0-9, A-F\n");
        return 0;
      }
      if ( '0' <= strVar[2] && strVar[2] <= '9') {
        res = (SDL_Octet)(res + (int)(strVar[2]) - '0');
      } else if ( 'A' <= strVar[2] && strVar[2] <= 'F') {
        res = (SDL_Octet)(res + (int)(strVar[2]) - 'A' + 10);
      } else if ( 'a' <= strVar[2] && strVar[2] <= 'f') {
        res = (SDL_Octet)(res + (int)(strVar[2]) - 'a' + 10);
      } else {
        xPrintString("Octet value should consist of HEX values: 0-9, A-F\n");
        return 0;
      }
      *(SDL_Bit *)Result = res;
      return 1;
    } else {
      xPrintString("Octet length should be 2\n");
      return 0;
    }
  } else {
    /* SDL syntax */
    xUngetToken( Token, strVar );
    Token = xPromptQuestionMark(" (octet) : ",
                                " (octet) : ", strVar);

    if (Token == xxId && strlen(strVar) <= 2 &&
        sscanf(strVar, "%x", &temp) >= 1) {
      *(SDL_Octet *)Result = (SDL_Octet)temp;
      return 1;
    }
    xPrintString("Illegal Octet value. It should be two HEX values.\n");
    return 0;
  }
}


/*---+---------------------------------------------------------------
     xWri_SDL_Octet
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Octet(
  void  * C)
#else
char *xWri_SDL_Octet( C )
  void  * C;
#endif
{
#ifdef XITEXCOMM
  if ( XSYSD xUse_Itex_Syntax ) {
    (void)sprintf( XSYSD CTempTemp, "'%0.2X'O", *(SDL_Octet *)C);
  }
  else
#endif
  if (XASN1_SYNTAX)  {
    (void)sprintf( XSYSD CTempTemp, "'%0.2X'H", *(SDL_Octet *)C);
  } else {
    (void)sprintf( XSYSD CTempTemp, "%0.2X", *(SDL_Octet *)C);
  }
  return  XSYSD CTempTemp;
}

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*---+---------------------------------------------------------------
     xRead_SDL_Octet_String
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

#ifndef XNOPROTO
int xRead_SDL_Octet_String(
  void  * Result)
#else
int xRead_SDL_Octet_String( Result )
  void  * Result;
#endif
{
  xxToken Token;
  char    strVar[MAX_READ_LENGTH];
  char    strVar2[256];
  int     i, len, res;
  
  Token = xPromptQuestionMark(" (octet_string) : ",
     " (octet_string) : ", strVar);
  if (Token != xxString) {
    xPrintString("Octet_String value should enclosed in '\n");
    return 0;
  }
  Token = xScanToken(strVar2);
  if ( Token != xxId || strlen(strVar2) != 1 || 
       ( strVar2[0] != 'h' && strVar2[0] != 'H' &&
         strVar2[0] != 'o' && strVar2[0] != 'O'
       )
     ) {
    xPrintString("Octet_String value should end with H\n");
    return 0;
  }
  len = strlen(strVar)-2;
  if (len%2 == 1) {
    strVar[++len] = '0';
    strVar[len+1] = '\'';
    strVar[len+2] = '\'';
  }
  (*(SDL_Octet_String *)Result).Length = len/2;
  (*(SDL_Octet_String *)Result).IsAssigned = (xbool)1;
  (*(SDL_Octet_String *)Result).Bits = xAlloc_SDL_Bit_String((xptrint)(len/2));
  for (i=1; i<len; i=i+2) {
    if ( '0' <= strVar[i] && strVar[i] <= '9') {
      res = 16 * ((int)(strVar[i]) - '0');
    } else if ( 'A' <= strVar[i] && strVar[i] <= 'F') {
      res = 16 * ((int)(strVar[i]) - 'A' + 10);
    } else if ( 'a' <= strVar[i] && strVar[i] <= 'f') {
      res = 16 * ((int)(strVar[i]) - 'a' + 10);
    } else {
      xPrintString("Octet_String value should consist of HEX values: 0-9, A-F\n");
      return 0;
    }
    if ( '0' <= strVar[i+1] && strVar[i+1] <= '9') {
      res = res + (int)(strVar[i+1]) - '0';
    } else if ( 'A' <= strVar[i+1] && strVar[i+1] <= 'F') {
      res = res + (int)(strVar[i+1]) - 'A' + 10;
    } else if ( 'a' <= strVar[i+1] && strVar[i+1] <= 'f') {
      res = res + (int)(strVar[i+1]) - 'a' + 10;
    } else {
      xPrintString("Octet_String value should consist of HEX values: 0-9, A-F\n");
      return 0;
    }
    (*(SDL_Octet_String *)Result).Bits[(i-1)/2] = (unsigned char)res;
  }
  return 1;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Octet_String
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Octet_String(
  void  * C)
#else
char *xWri_SDL_Octet_String( C )
  void  * C;
#endif
{
  int i;
  XSYSD CTempTemp[0] = '\'';
  for (i=0; i<(*(SDL_Bit_String *)C).Length; i++) {
    (void)sprintf( (char *)&(XSYSD CTempTemp[2*i+1]), "%0.2X",
                   (*(SDL_Octet_String *)C).Bits[i]);
  }
  XSYSD CTempTemp[2*i+1] = '\'';
#ifdef XITEXCOMM
  if ( XSYSD xUse_Itex_Syntax ) {
    XSYSD CTempTemp[2*i+2] = 'O';
  }
  else
#endif
    XSYSD CTempTemp[2*i+2] = 'H';
  XSYSD CTempTemp[2*i+3] = '\0';
  return XSYSD CTempTemp;
}

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*---+---------------------------------------------------------------
     xRead_SDL_Character
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_Character(
  void  * Result)
#else
int xRead_SDL_Character( Result )
  void  * Result;
#endif
{
  xxToken Token;
  char    strVar[256];
  int     i;
  
  Token = xPromptQuestionMark(" (character) : ",
     " (character) : ", strVar);

  if (Token == xxId) {
    if (strVar[1] == '\0') {
      *(SDL_Character *)Result = strVar[0];
      return 1;
    }
    for (i = 0; i <= 32; i++) {
      if (xfEqualIdString(strVar, xfCharacterLiteral[i]) == 2) {
        if (i == 32)
          *(SDL_Character *)Result = (SDL_Character) 127;
        else
          *(SDL_Character *)Result = (SDL_Character) i;
        return 1;
      }
    }
  }

  if (Token == xxString) {
    i = strlen(strVar);
    if ( i == 3 ||
         (i == 4 && strVar[1] == '\'' && strVar[2] == '\'')
       ) {
      *(SDL_Character *)Result = strVar[1];
      return 1;
    }
  }
  if (Token == xxASN1String) {
    i = strlen(strVar);
    if ( i == 3 ||
         (i == 4 && strVar[1] == '\"' && strVar[2] == '\"')
       ) {
      *(SDL_Character *)Result = strVar[1];
      return 1;
    }
  }
  if (Token == xxSlash) {
    *(SDL_Character *)Result = '/';
    return 1;
  }
  if (Token == xxColon) {
    *(SDL_Character *)Result = ':';
    return 1;
  }
  if (Token == xxMinus) {
    *(SDL_Character *)Result = '-';
    return 1;
  }
  if (Token == xxPlus) {
    *(SDL_Character *)Result = '+';
    return 1;
  }
  if (Token == xxStar) {
    *(SDL_Character *)Result = '*';
    return 1;
  }
  if (Token == xxComma) {
    *(SDL_Character *)Result = ',';
    return 1;
  }
  if (Token == xxSemicolon) {
    *(SDL_Character *)Result = ';';
    return 1;
  }
  if (Token == xxArrowHead) {
    *(SDL_Character *)Result = '^';
    return 1;
  }
  if (Token == xxLPar) {
    *(SDL_Character *)Result = '(';
    return 1;
  }
  if (Token == xxRPar) {
    *(SDL_Character *)Result = ')';
    return 1;
  }
  if (Token == xxDot) {
    *(SDL_Character *)Result = '.';
    return 1;
  }
/* '[' used as cancel in SImUI
  if (Token == xxLBracket) {
    *(SDL_Character *)Result = '[';
    return 1;
  }
*/
  if (Token == xxRBracket) {
    *(SDL_Character *)Result = ']';
    return 1;
  }
  if (Token == xxLCurlyBracket) {
    *(SDL_Character *)Result = '{';
    return 1;
  }
  if (Token == xxRCurlyBracket) {
    *(SDL_Character *)Result = '}';
    return 1;
  }
  if (Token == xxAt) {
    *(SDL_Character *)Result = '@';
    return 1;
  }
  if (Token == xxQuestionMark) {
    *(SDL_Character *)Result = '?';
    return 1;
  }
  if (Token == xxExclMark) {
    *(SDL_Character *)Result = '!';
    return 1;
  }
  xPrintString("Illegal character value\n");
  return 0;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Character
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Character(
  void  * C)
#else
char *xWri_SDL_Character( C )
  void  * C;
#endif
{
  int index = 0;

  if (*(SDL_Character *)C == 127)
    return xfCharacterLiteral[32];
  if (*(SDL_Character *)C >= 0 && *(SDL_Character *)C <= 31)
    return xfCharacterLiteral[(int)*(SDL_Character *)C];
  if (XASN1_SYNTAX) 
    XSYSD CTempTemp[index++] = '"';
  else
    XSYSD CTempTemp[index++] = '\'';
  XSYSD CTempTemp[index++] = *(SDL_Character *)C;
  if (XASN1_SYNTAX)  {
    if (*(SDL_Character *)C == '"')
      XSYSD CTempTemp[index++] = *(SDL_Character *)C;
    XSYSD CTempTemp[index++] = '"';
  } else {
    if (*(SDL_Character *)C == '\'')
      XSYSD CTempTemp[index++] = *(SDL_Character *)C;
    XSYSD CTempTemp[index++] = '\'';
  }
  XSYSD CTempTemp[index] = '\0';
  return XSYSD CTempTemp;
}


/*---+---------------------------------------------------------------
     xRead_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_Charstring(
  void  * Result)
#else
int xRead_SDL_Charstring( Result )
  void  * Result;
#endif
{
  xxToken Token;
  char    strVar[1000];
  char    UnPrint[1000];
  int     IRes, IstrVar, IUnPrint, I;
  
  Token = xPromptQuestionMark(" (charstring) : ",
    " (charstring) : ", strVar);
  if (Token != xxString && Token != xxASN1String) {
    xPrintString("Charstring value should start with ' or \"\n");
    return 0;
  }

  if (*(SDL_Charstring *)Result != (SDL_Charstring)0)
    xFree_SDL_Charstring((void **)Result);
  *(SDL_Charstring *)Result =
    xAlloc_SDL_Charstring((xptrint)(strlen(strVar)));
  (*(SDL_Charstring *)Result)[0] = 'V';

  IRes = 1;
  for (IstrVar=1; strVar[IstrVar+1]!='\0'; IstrVar++) {
    if ( ( Token == xxString &&
           strVar[IstrVar] == '\'' && strVar[IstrVar+1] != '\'' ) ||
         ( Token == xxASN1String &&
           strVar[IstrVar] == '"' && strVar[IstrVar+1] != '"' )
       ) {
      IUnPrint = 0;
      if (Token == xxString) {
        for (IstrVar++; strVar[IstrVar]!='\''; IstrVar++)
          UnPrint[IUnPrint++] = strVar[IstrVar];
      } else {
        for (IstrVar++; strVar[IstrVar]!='"'; IstrVar++)
          UnPrint[IUnPrint++] = strVar[IstrVar];
      }
      UnPrint[IUnPrint] = '\0';
      for (I=0; I<=32 && strcmp(UnPrint, xfCharacterLiteral[I])!=0; I++)
        ;
      if (I < 32)
        (*(SDL_Charstring *)Result)[IRes++] = (char)I;
      else if (I == 32)
        (*(SDL_Charstring *)Result)[IRes++] = (char)127;
      else {
        xPrintString("Illegal unprintable character in charstring\n");
        (*(SDL_Charstring *)Result)[IRes] = '\0';
        return 0;
      }
    } else {
      if ( ( Token == xxString &&
             strVar[IstrVar] == '\'' && strVar[IstrVar+1] == '\'' ) ||
           ( Token == xxASN1String &&
             strVar[IstrVar] == '"' && strVar[IstrVar+1] == '"' )
         ) {
        IstrVar++;
      }
      (*(SDL_Charstring *)Result)[IRes++] = strVar[IstrVar];
    }
  }
  (*(SDL_Charstring *)Result)[IRes] = '\0';
  return 1;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Charstring
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Charstring(
  void * CS)
#else
char *xWri_SDL_Charstring( CS )
  void * CS;
#endif
{
  char * C;
  int index = 0;
  int index2;
  char delimiter;

  if (XASN1_SYNTAX)
    delimiter = '"';
  else
    delimiter = '\'';
  if ( *(SDL_Charstring *)CS == (SDL_Charstring)0 ) {
    if (XASN1_SYNTAX)
      (void)strcpy(XSYSD CTempTemp, "\"\"");
    else
      (void)strcpy(XSYSD CTempTemp, "''");
  } else {
    C = *(SDL_Charstring *)CS + 1;
    XSYSD CTempTemp[index++] = delimiter;
    while (*C != '\0') {
      if (*C == 127) {
        XSYSD CTempTemp[index++] = delimiter;
        for (index2 = 0;
             (unsigned)index2 < strlen(xfCharacterLiteral[32]);
             index2++)
          XSYSD CTempTemp[index++] = xfCharacterLiteral[32][index2];
        XSYSD CTempTemp[index++] = delimiter;
      }
      else if (*C >= 0 && *C <= 31) {
        XSYSD CTempTemp[index++] = delimiter;
        for (index2 = 0;
             (unsigned)index2 < strlen(xfCharacterLiteral[(int)*C]);
             index2++)
          XSYSD CTempTemp[index++] = xfCharacterLiteral[(int)*C][index2];
        XSYSD CTempTemp[index++] = delimiter;
      }
      else {
        XSYSD CTempTemp[index++] = *C;
        if (XASN1_SYNTAX) {
          if (*C == '"') XSYSD CTempTemp[index++] = *C;
        } else {
          if (*C == '\'') XSYSD CTempTemp[index++] = *C;
        }
      }
      C++;
    }
    XSYSD CTempTemp[index++] = delimiter;
    XSYSD CTempTemp[index] = '\0';
  }
  return XSYSD CTempTemp;
}


/*---+---------------------------------------------------------------
     xRead_SDL_NumericString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_NumericString(
  void  * Result)
#else
int xRead_SDL_NumericString( Result )
  void  * Result;
#endif
{
  int     rv;
#ifdef XITEXCOMM
  xxToken Token;
  char    strVar[1000];
#endif

  rv = xRead_SDL_Charstring(Result);
#ifdef XITEXCOMM
  if (rv) {
    Token = xScanToken(strVar);
    if (Token != xxId || strlen(strVar) != 1 || strVar[0] != 'N')
      xUngetToken(Token, strVar);
  }
#endif
  return rv;
}


/*---+---------------------------------------------------------------
     xWri_SDL_NumericString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_NumericString(
  void * CS)
#else
char *xWri_SDL_NumericString( CS )
  void * CS;
#endif
{
  char *rv;

  rv = xWri_SDL_Charstring(CS);
#ifdef XITEXCOMM
  if ( XSYSD xUse_Itex_Syntax )
    (void)strcat(rv, "N");
#endif
  return rv;
}


/*---+---------------------------------------------------------------
     xRead_SDL_PrintableString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_PrintableString(
  void  * Result)
#else
int xRead_SDL_PrintableString( Result )
  void  * Result;
#endif
{
  int     rv;
#ifdef XITEXCOMM
  xxToken Token;
  char    strVar[1000];
#endif

  rv = xRead_SDL_Charstring(Result);
#ifdef XITEXCOMM
  if (rv) {
    Token = xScanToken(strVar);
    if (Token != xxId || strlen(strVar) != 1 || strVar[0] != 'P')
      xUngetToken(Token, strVar);
  }
#endif
  return rv;
}


/*---+---------------------------------------------------------------
     xWri_SDL_PrintableString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_PrintableString(
  void * CS)
#else
char *xWri_SDL_PrintableString( CS )
  void * CS;
#endif
{
  char *rv;

  rv = xWri_SDL_Charstring(CS);
#ifdef XITEXCOMM
  if ( XSYSD xUse_Itex_Syntax ) {
    (void)strcat(rv, "P");
  }
#endif
  return rv;
}


/*---+---------------------------------------------------------------
     xRead_SDL_VisibleString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_VisibleString(
  void  * Result)
#else
int xRead_SDL_VisibleString( Result )
  void  * Result;
#endif
{
  int     rv;
#ifdef XITEXCOMM
  xxToken Token;
  char    strVar[1000];
#endif

  rv = xRead_SDL_Charstring(Result);
#ifdef XITEXCOMM
  if (rv) {
    Token = xScanToken(strVar);
    if (Token != xxId || strlen(strVar) != 1 || strVar[0] != 'V')
      xUngetToken(Token, strVar);
  }
#endif
  return rv;
}


/*---+---------------------------------------------------------------
     xWri_SDL_VisibleString
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_VisibleString(
  void * CS)
#else
char *xWri_SDL_VisibleString( CS )
  void * CS;
#endif
{
  char *rv;

  rv = xWri_SDL_Charstring(CS);
#ifdef XITEXCOMM
  if ( XSYSD xUse_Itex_Syntax ) {
    (void)strcat(rv, "V");
  }
#endif
  return rv;
}


/*---+---------------------------------------------------------------
     xRead_SDL_Duration
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_Duration(
  void  * Result)
#else
int xRead_SDL_Duration( Result )
  void  * Result;
#endif
{
  xxToken      Token;
  char         strVar[256];
  SDL_Duration Value;
  int          Res;
  
  Token = xPromptQuestionMark(" (duration) : ", 
     " (duration) : ", strVar);
  xUngetToken(Token, strVar);

  Res = xReadSDL_Time(&Value);
  if (Res) *(SDL_Duration *)Result = Value;
  return Res;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Duration
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Duration(
  void  * D)
#else
char *xWri_SDL_Duration( D )
  void  * D;
#endif
{
  return xWriteSDL_Time(*(SDL_Duration *)D, TIMEVALUEDECIMALS);
}


/*---+---------------------------------------------------------------
     xRead_SDL_Integer
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_Integer(
  void  * Result)
#else
int xRead_SDL_Integer( Result )
  void  * Result;
#endif
{
  xxToken     Token;
  char        strVar[256];
  SDL_Integer Value;
  xbool       Minus=(xbool)0;

  Token = xPromptQuestionMark(" (integer) : ",
      " (integer) : ", strVar);
  if (Token == xxPlus) {
    Token = xScanToken(strVar);
  } else if (Token == xxMinus) {
    Token = xScanToken(strVar);
    Minus = (xbool)1;
  }
  if (Token != xxId) {
    xPrintString("Illegal syntax\n");
    return 0;
  }

#ifdef X_LONG_INT
  if (sscanf(strVar, "%ld", &Value) >= 1) {
#else
  if (sscanf(strVar, "%d", &Value) >= 1) {
#endif
    if (Minus)
      *(SDL_Integer *)Result = -Value;
    else
      *(SDL_Integer *)Result = Value;
    return 1;
  }
  xPrintString("Illegal syntax\n");
  return 0;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Integer
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Integer(
  void  * C)
#else
char *xWri_SDL_Integer( C )
  void  * C;
#endif
{
#ifdef X_LONG_INT
  (void)sprintf(XSYSD CTempTemp, "%ld", *(SDL_Integer *)C);
#else
  (void)sprintf(XSYSD CTempTemp, "%d", *(SDL_Integer *)C);
#endif
  return XSYSD CTempTemp;
}


/*---+---------------------------------------------------------------
     xRead_SDL_Null
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_Null(
  void  * Result)
#else
int xRead_SDL_Null( Result )
  void  * Result;
#endif
{
  xxToken   Token;
  char      strVar[256];
  
  Token = xPromptQuestionMark(" (null) : ",
     " (null) : ", strVar);

  if (Token == xxNull) {
    *(SDL_Null *)Result = SDL_NullValue;
    return 1;
  }
  xPrintString("Illegal Null value. Null is the only legal value.\n");
  return 0;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Null
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Null(
  void  * C)
#else
char *xWri_SDL_Null( C )
  void  * C;
#endif
{
  if (*(SDL_Null *)C == SDL_NullValue) return "Null";
  return "Illegal Null value";
}


#ifndef XNOUSEOFREAL
/*---+---------------------------------------------------------------
     xRead_SDL_Real
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_Real(
  void  * Result)
#else
int xRead_SDL_Real( Result )
  void  * Result;
#endif
{
  xxToken   Token;
  char      strVar[256];
  SDL_Real  Value;
  xbool     Minus = (xbool)0;
  xbool     MinusExp  = (xbool)0;
  unsigned int digits, base, exponent, i;

  Token = xPromptQuestionMark(" (real) : ", " (real) : ", strVar);

  /* check special case: 0 */
  if (Token == xxId && !strcmp(strVar, "0")) {
    *(SDL_Real *)Result = 0.0;
    return 1;
  }

  if (Token != xxLCurlyBracket) {
    /* Read SDL syntax */
    if (Token == xxPlus) {
      Token = xScanToken(strVar);
    } else if (Token == xxMinus) {
      Token = xScanToken(strVar);
      Minus = (xbool)1;
    }
    if (Token != xxId) {
      xPrintString("Illegal syntax\n");
      return 0;
    }

    if (sscanf(strVar, "%lf", &Value) >= 1) {
      if (Minus)
        *(SDL_Real *)Result = -Value;
      else
        *(SDL_Real *)Result = Value;
      return 1;
    }
    xPrintString("Illegal syntax\n");
    return 0;
  }

  /* Try ASN.1 syntax */
  Token = xScanToken(strVar);
  if (Token == xxId && !strcmp(strVar, "mantissa"))
    Token = xScanToken(strVar);

  /* Check for sign */
  if (Token == xxMinus) {
    Minus = (xbool)1;
    Token = xScanToken(strVar);
  }

  /* Read/check digits */
  if (Token != xxId || sscanf(strVar, "%d", &digits) < 1) {
    xPrintString("Illegal syntax, word mantissa or integer value expected.\n");
    return 0;
  }

  /* Check , and base */
  Token = xScanToken(strVar);
  if (Token == xxComma) Token = xScanToken(strVar);
  if (Token == xxId && !strcmp(strVar, "base"))
    Token = xScanToken(strVar);

  /* Read/check base */
  if ( Token != xxId || 
       ( strcmp(strVar,"2") && strcmp(strVar,"10") )
     ) {
    xPrintString("Illegal syntax, word base or 2 or 10 expected\n");
    return 0;
  }
  if (!strcmp(strVar,"2"))
    base = 2;
  else
    base = 10;

  /* Check , */
  Token = xScanToken(strVar);
  if (Token == xxComma) Token = xScanToken(strVar);
  if (Token == xxId && !strcmp(strVar, "exponent"))
    Token = xScanToken(strVar);

  /* Check for exponent sign */
  if (Token == xxMinus) {
    MinusExp = (xbool)1;
    Token = xScanToken(strVar);
  }

  /* Read exponent */
  if (Token != xxId || sscanf(strVar, "%d", &exponent) < 1) {
    xPrintString("Illegal syntax, word exponent or integer value expected\n");
    return 0;
  }

  /* Check trailing } */
  Token = xScanToken(strVar);
  if (Token != xxRCurlyBracket) xUngetToken(Token, strVar);

  /* Compute Result */
  if (exponent == 0) {
    Value = 1;
  } else if (MinusExp) {
    Value = ((SDL_Real)1) / ((SDL_Real)base);
    for (i=1; i<exponent; i++) {
      Value = Value/((SDL_Real)base);
    }
  } else {
    Value = base;
    for (i=1; i<exponent; i++) {
      Value = Value*((SDL_Real)base);
    }
  }
  if (Minus)
    *(SDL_Real *)Result = -(((SDL_Real)digits) * Value);
  else
    *(SDL_Real *)Result = ((SDL_Real)digits) * Value;
  return 1;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Real
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Real(
  void  * C)
#else
char *xWri_SDL_Real( C )
  void  * C;
#endif
{
  double orig = *(double *)C;
  int tmpexp,exponent;
  char tmpbuf[100];
  char * tmpbufp = tmpbuf;
  int negexp, onlyzeros;
  char * tmp;
  char cpcharbuf[2];
  
  if (! XASN1_SYNTAX) {
    (void)sprintf(XSYSD CTempTemp, "%.4f", *(SDL_Real *)C);
    return XSYSD CTempTemp;
  }

  /* Take care of special case: 0 */
  if (orig==0) return "0";

  /* Write the leading { and optional sign */
  XSYSD CTempTemp[0] = '\0';
  xSafeStrcat(XSYSD CTempTemp, "{mantissa ");
  if (orig<0) {
    xSafeStrcat(XSYSD CTempTemp, "-");
    orig = (-orig);
  }
 
  /* Write orig to tmpbuf */
  (void)sprintf(tmpbufp, "%e", orig);

  /* Copy the first digit */
  cpcharbuf[1] = '\0';
  cpcharbuf[0] = *tmpbufp++;
  xSafeStrcat(XSYSD CTempTemp, cpcharbuf);

  /* Treat the optional decimal part and start computing the exponent*/
  if (*tmpbufp=='.') {

    /* Skip the dot */
    tmpbufp++;

    /* Find out if all the digits are 0 */
    /* If only zeros tmp is set to the char following the zeros */
    tmp = tmpbufp;
    onlyzeros = 1;
    while (*tmp != 'e') {
      if (*tmp != '0') {
        onlyzeros = 0;
        break;
      }
      tmp++;
    }

    /* Write the rest of the digits and start computing the exponent (tmpexp) */
    tmpexp=0;
    while ((!onlyzeros) && (*tmpbufp!='e')) {
      /* Copy digit and increase tmpexp */
      cpcharbuf[0] = *tmpbufp++;
      xSafeStrcat(XSYSD CTempTemp, cpcharbuf);
      tmpexp++;
      /* Find out if all the digits are 0 */
      /* If only zeros tmp is set to the char following the zeros */
      tmp=tmpbufp;
      onlyzeros=1;
      while (*tmp!='e') {
        if (*tmp!='0') {
          onlyzeros = 0;
          break;
        }
        tmp++;
      }
    }
    /* If loop broken by onlyzeros: set tmpbufp */
    if (onlyzeros) {
      tmpbufp=tmp;
    }
  } else {
    tmpexp = 0;
  }

  /* Skip the 'e' */
  tmpbufp++;

  /* Read the exponent part and compute the total exponent */
  if (*tmpbufp++=='-') {
    negexp = 1;
  } else {
    negexp = 0;
  }
  exponent=0;
  while ((*tmpbufp>47) && (*tmpbufp<58)) {
    exponent = 10*exponent + (*tmpbufp-48);
    tmpbufp++;
  }
  if (negexp) exponent = (-exponent);
  exponent = exponent - tmpexp;

  /* Write the base and commas, the exponent, and trailing }  */
  xSafeStrcat(XSYSD CTempTemp, ", base 10, exponent ");
  (void)sprintf(tmpbufp, "%d", exponent);
  xSafeStrcat(XSYSD CTempTemp, tmpbufp);
  xSafeStrcat(XSYSD CTempTemp, "}");

  /* Return result */
  return XSYSD CTempTemp;
}
#endif
       /* ! XNOUSEOFREAL */


/*---+---------------------------------------------------------------
     xRead_SDL_Time
-------------------------------------------------------------------*/
#ifndef XNOPROTO
int xRead_SDL_Time(
  void  * Result)
#else
int xRead_SDL_Time( Result )
  void  * Result;
#endif
{
  xxToken   Token;
  char      strVar[256];
  SDL_Time  Value;
  int       Res;
  
  Token = xPromptQuestionMark(" (time) : ", 
     " (time) : ", strVar);
  xUngetToken(Token, strVar);

  Res = xReadSDL_Time(&Value);
  if (Res) *(SDL_Time *)Result = Value;
  return Res;
}


/*---+---------------------------------------------------------------
     xWri_SDL_Time
-------------------------------------------------------------------*/
#ifndef XNOPROTO
char *xWri_SDL_Time(
  void  * T)
#else
char *xWri_SDL_Time( T )
  void  * T;
#endif
{
  return xWriteSDL_Time(*(SDL_Time *)T, TIMEVALUEDECIMALS);
}


#endif
   /* XREADANDWRITEF */

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */



/*---+---------------------------------------------------------------
     xPassOwn
-------------------------------------------------------------------*/
#ifndef XNOPROTO
extern void * xPassOwn(void **P)
#else
extern void * xPassOwn(P)
  void **P;
#endif
{
  void * tmp;
  tmp = *P;
  *P = 0;
  return tmp;
}
