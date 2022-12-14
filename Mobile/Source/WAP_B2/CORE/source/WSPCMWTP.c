/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */

/* Program generated by SDT Cmicro, version 1.33 */

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif

#define C_MICRO_1_3
#include "ml_typ.h"
#include "ctypes.h"
#include "BuffMgt.h"
#include "GlblDecl.h"
#include "WSPCMWTP.h"


/*************************************************************************
**                  #CODE directives, #BODY sections                    **
*************************************************************************/

/*************************************************************************
**                  SECTION Variables and Functions                     **
*************************************************************************/

/*****
* NEWTYPE TRInvokeReqType  (BODY section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),4)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_z_WSPConnectionMode_WTP_B_TRInvokeReqType(
   z_WSPConnectionMode_WTP_B_TRInvokeReqType *yVar )
#else
extern void yDef_z_WSPConnectionMode_WTP_B_TRInvokeReqType( yVar )
  z_WSPConnectionMode_WTP_B_TRInvokeReqType *yVar;
#endif
{
  yDef_AddressType(&((*yVar).SourceAddress));
  yDef_AddressType(&((*yVar).DestinationAddress));
  xDef_SDL_Integer(&((*yVar).AckType));
  yDef_pdubuf_ptr(&((*yVar).UserData));
  xDef_SDL_Integer(&((*yVar).ClassType));
  xDef_SDL_Integer(&((*yVar).Handle));
}

/*------------------EQUAL-------------------*/
#ifndef XNOPROTO
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_B_TRInvokeReqType(
  z_WSPConnectionMode_WTP_B_TRInvokeReqType yExpr1,
  z_WSPConnectionMode_WTP_B_TRInvokeReqType yExpr2 )
#else
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_B_TRInvokeReqType
  ( yExpr1, yExpr2 )
  z_WSPConnectionMode_WTP_B_TRInvokeReqType yExpr1, yExpr2;
#endif
{
  if ( yNEqF_AddressType(yExpr1.SourceAddress, yExpr2.SourceAddress) )
  return SDL_False;
  if ( yNEqF_AddressType(yExpr1.DestinationAddress,
  yExpr2.DestinationAddress) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.AckType, yExpr2.AckType) ) return SDL_False;
  if ( yNEqF_pdubuf_ptr(yExpr1.UserData, yExpr2.UserData) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.ClassType, yExpr2.ClassType) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.Handle, yExpr2.Handle) ) return SDL_False;
  return SDL_True;
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern z_WSPConnectionMode_WTP_B_TRInvokeReqType yMake_z_WSPConnectionMode_WTP_B_TRInvokeReqType(
  AddressType ySourceAddress,
  AddressType yDestinationAddress,
  SDL_Integer yAckType,
  pdubuf_ptr yUserData,
  SDL_Integer yClassType,
  SDL_Integer yHandle )
#else
extern z_WSPConnectionMode_WTP_B_TRInvokeReqType  yMake_z_WSPConnectionMode_WTP_B_TRInvokeReqType
  ( ySourceAddress, yDestinationAddress, yAckType, yUserData, yClassType,
  yHandle )
  AddressType ySourceAddress;
  AddressType yDestinationAddress;
  SDL_Integer yAckType;
  pdubuf_ptr yUserData;
  SDL_Integer yClassType;
  SDL_Integer yHandle;
#endif
{
  z_WSPConnectionMode_WTP_B_TRInvokeReqType  yVar;
  memset((void *)(&yVar), 0, sizeof(z_WSPConnectionMode_WTP_B_TRInvokeReqType));
  yAssF_AddressType(yVar.SourceAddress, ySourceAddress, XASSMAKE);
  yAssF_AddressType(yVar.DestinationAddress, yDestinationAddress, XASSMAKE);
  yAssF_SDL_Integer(yVar.AckType, yAckType, XASSMAKE);
  yAssF_pdubuf_ptr(yVar.UserData, yUserData, XASSMAKE);
  yAssF_SDL_Integer(yVar.ClassType, yClassType, XASSMAKE);
  yAssF_SDL_Integer(yVar.Handle, yHandle, XASSMAKE);
  return yVar;
}

/*****
* NEWTYPE TRInvokeIndType  (BODY section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),13)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_z_WSPConnectionMode_WTP_C_TRInvokeIndType(
   z_WSPConnectionMode_WTP_C_TRInvokeIndType *yVar )
#else
extern void yDef_z_WSPConnectionMode_WTP_C_TRInvokeIndType( yVar )
  z_WSPConnectionMode_WTP_C_TRInvokeIndType *yVar;
#endif
{
  yDef_AddressType(&((*yVar).SourceAddress));
  yDef_AddressType(&((*yVar).DestinationAddress));
  xDef_SDL_Integer(&((*yVar).AckType));
  yDef_pdubuf_ptr(&((*yVar).UserData));
  xDef_SDL_Integer(&((*yVar).ClassType));
  xDef_SDL_Integer(&((*yVar).Handle));
}

/*------------------EQUAL-------------------*/
#ifndef XNOPROTO
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_C_TRInvokeIndType(
  z_WSPConnectionMode_WTP_C_TRInvokeIndType yExpr1,
  z_WSPConnectionMode_WTP_C_TRInvokeIndType yExpr2 )
#else
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_C_TRInvokeIndType
  ( yExpr1, yExpr2 )
  z_WSPConnectionMode_WTP_C_TRInvokeIndType yExpr1, yExpr2;
#endif
{
  if ( yNEqF_AddressType(yExpr1.SourceAddress, yExpr2.SourceAddress) )
  return SDL_False;
  if ( yNEqF_AddressType(yExpr1.DestinationAddress,
  yExpr2.DestinationAddress) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.AckType, yExpr2.AckType) ) return SDL_False;
  if ( yNEqF_pdubuf_ptr(yExpr1.UserData, yExpr2.UserData) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.ClassType, yExpr2.ClassType) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.Handle, yExpr2.Handle) ) return SDL_False;
  return SDL_True;
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern z_WSPConnectionMode_WTP_C_TRInvokeIndType yMake_z_WSPConnectionMode_WTP_C_TRInvokeIndType(
  AddressType ySourceAddress,
  AddressType yDestinationAddress,
  SDL_Integer yAckType,
  pdubuf_ptr yUserData,
  SDL_Integer yClassType,
  SDL_Integer yHandle )
#else
extern z_WSPConnectionMode_WTP_C_TRInvokeIndType  yMake_z_WSPConnectionMode_WTP_C_TRInvokeIndType
  ( ySourceAddress, yDestinationAddress, yAckType, yUserData, yClassType,
  yHandle )
  AddressType ySourceAddress;
  AddressType yDestinationAddress;
  SDL_Integer yAckType;
  pdubuf_ptr yUserData;
  SDL_Integer yClassType;
  SDL_Integer yHandle;
#endif
{
  z_WSPConnectionMode_WTP_C_TRInvokeIndType  yVar;
  memset((void *)(&yVar), 0, sizeof(z_WSPConnectionMode_WTP_C_TRInvokeIndType));
  yAssF_AddressType(yVar.SourceAddress, ySourceAddress, XASSMAKE);
  yAssF_AddressType(yVar.DestinationAddress, yDestinationAddress, XASSMAKE);
  yAssF_SDL_Integer(yVar.AckType, yAckType, XASSMAKE);
  yAssF_pdubuf_ptr(yVar.UserData, yUserData, XASSMAKE);
  yAssF_SDL_Integer(yVar.ClassType, yClassType, XASSMAKE);
  yAssF_SDL_Integer(yVar.Handle, yHandle, XASSMAKE);
  return yVar;
}

/*****
* NEWTYPE TRInvokeResType  (BODY section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),22)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_z_WSPConnectionMode_WTP_D_TRInvokeResType(
   z_WSPConnectionMode_WTP_D_TRInvokeResType *yVar )
#else
extern void yDef_z_WSPConnectionMode_WTP_D_TRInvokeResType( yVar )
  z_WSPConnectionMode_WTP_D_TRInvokeResType *yVar;
#endif
{
  yDef_pdubuf_ptr(&((*yVar).ExitInfo));
  xDef_SDL_Integer(&((*yVar).Handle));
}

/*------------------EQUAL-------------------*/
#ifndef XNOPROTO
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_D_TRInvokeResType(
  z_WSPConnectionMode_WTP_D_TRInvokeResType yExpr1,
  z_WSPConnectionMode_WTP_D_TRInvokeResType yExpr2 )
#else
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_D_TRInvokeResType
  ( yExpr1, yExpr2 )
  z_WSPConnectionMode_WTP_D_TRInvokeResType yExpr1, yExpr2;
#endif
{
  if ( yNEqF_pdubuf_ptr(yExpr1.ExitInfo, yExpr2.ExitInfo) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.Handle, yExpr2.Handle) ) return SDL_False;
  return SDL_True;
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern z_WSPConnectionMode_WTP_D_TRInvokeResType yMake_z_WSPConnectionMode_WTP_D_TRInvokeResType(
  pdubuf_ptr yExitInfo,
  SDL_Integer yHandle )
#else
extern z_WSPConnectionMode_WTP_D_TRInvokeResType  yMake_z_WSPConnectionMode_WTP_D_TRInvokeResType
  ( yExitInfo, yHandle )
  pdubuf_ptr yExitInfo;
  SDL_Integer yHandle;
#endif
{
  z_WSPConnectionMode_WTP_D_TRInvokeResType  yVar;
  memset((void *)(&yVar), 0, sizeof(z_WSPConnectionMode_WTP_D_TRInvokeResType));
  yAssF_pdubuf_ptr(yVar.ExitInfo, yExitInfo, XASSMAKE);
  yAssF_SDL_Integer(yVar.Handle, yHandle, XASSMAKE);
  return yVar;
}

/*****
* NEWTYPE TRInvokeCnfType  (BODY section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),27)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(
   z_WSPConnectionMode_WTP_E_TRInvokeCnfType *yVar )
#else
extern void yDef_z_WSPConnectionMode_WTP_E_TRInvokeCnfType( yVar )
  z_WSPConnectionMode_WTP_E_TRInvokeCnfType *yVar;
#endif
{
  yDef_pdubuf_ptr(&((*yVar).ExitInfo));
  xDef_SDL_Integer(&((*yVar).Handle));
}

/*------------------EQUAL-------------------*/
#ifndef XNOPROTO
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(
  z_WSPConnectionMode_WTP_E_TRInvokeCnfType yExpr1,
  z_WSPConnectionMode_WTP_E_TRInvokeCnfType yExpr2 )
#else
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_E_TRInvokeCnfType
  ( yExpr1, yExpr2 )
  z_WSPConnectionMode_WTP_E_TRInvokeCnfType yExpr1, yExpr2;
#endif
{
  if ( yNEqF_pdubuf_ptr(yExpr1.ExitInfo, yExpr2.ExitInfo) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.Handle, yExpr2.Handle) ) return SDL_False;
  return SDL_True;
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern z_WSPConnectionMode_WTP_E_TRInvokeCnfType yMake_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(
  pdubuf_ptr yExitInfo,
  SDL_Integer yHandle )
#else
extern z_WSPConnectionMode_WTP_E_TRInvokeCnfType  yMake_z_WSPConnectionMode_WTP_E_TRInvokeCnfType
  ( yExitInfo, yHandle )
  pdubuf_ptr yExitInfo;
  SDL_Integer yHandle;
#endif
{
  z_WSPConnectionMode_WTP_E_TRInvokeCnfType  yVar;
  memset((void *)(&yVar), 0, sizeof(z_WSPConnectionMode_WTP_E_TRInvokeCnfType));
  yAssF_pdubuf_ptr(yVar.ExitInfo, yExitInfo, XASSMAKE);
  yAssF_SDL_Integer(yVar.Handle, yHandle, XASSMAKE);
  return yVar;
}

/*****
* NEWTYPE TRResultIndType  (BODY section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),167(105,50),9)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_z_WSPConnectionMode_WTP_F_TRResultIndType(
   z_WSPConnectionMode_WTP_F_TRResultIndType *yVar )
#else
extern void yDef_z_WSPConnectionMode_WTP_F_TRResultIndType( yVar )
  z_WSPConnectionMode_WTP_F_TRResultIndType *yVar;
#endif
{
  yDef_pdubuf_ptr(&((*yVar).UserData));
  xDef_SDL_Integer(&((*yVar).Handle));
}

/*------------------EQUAL-------------------*/
#ifndef XNOPROTO
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_F_TRResultIndType(
  z_WSPConnectionMode_WTP_F_TRResultIndType yExpr1,
  z_WSPConnectionMode_WTP_F_TRResultIndType yExpr2 )
#else
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_F_TRResultIndType
  ( yExpr1, yExpr2 )
  z_WSPConnectionMode_WTP_F_TRResultIndType yExpr1, yExpr2;
#endif
{
  if ( yNEqF_pdubuf_ptr(yExpr1.UserData, yExpr2.UserData) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.Handle, yExpr2.Handle) ) return SDL_False;
  return SDL_True;
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern z_WSPConnectionMode_WTP_F_TRResultIndType yMake_z_WSPConnectionMode_WTP_F_TRResultIndType(
  pdubuf_ptr yUserData,
  SDL_Integer yHandle )
#else
extern z_WSPConnectionMode_WTP_F_TRResultIndType  yMake_z_WSPConnectionMode_WTP_F_TRResultIndType
  ( yUserData, yHandle )
  pdubuf_ptr yUserData;
  SDL_Integer yHandle;
#endif
{
  z_WSPConnectionMode_WTP_F_TRResultIndType  yVar;
  memset((void *)(&yVar), 0, sizeof(z_WSPConnectionMode_WTP_F_TRResultIndType));
  yAssF_pdubuf_ptr(yVar.UserData, yUserData, XASSMAKE);
  yAssF_SDL_Integer(yVar.Handle, yHandle, XASSMAKE);
  return yVar;
}

/*****
* NEWTYPE TRResultResType  (BODY section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),167(105,50),14)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_z_WSPConnectionMode_WTP_G_TRResultResType(
   z_WSPConnectionMode_WTP_G_TRResultResType *yVar )
#else
extern void yDef_z_WSPConnectionMode_WTP_G_TRResultResType( yVar )
  z_WSPConnectionMode_WTP_G_TRResultResType *yVar;
#endif
{
  yDef_pdubuf_ptr(&((*yVar).ExitInfo));
  xDef_SDL_Integer(&((*yVar).Handle));
}

/*------------------EQUAL-------------------*/
#ifndef XNOPROTO
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_G_TRResultResType(
  z_WSPConnectionMode_WTP_G_TRResultResType yExpr1,
  z_WSPConnectionMode_WTP_G_TRResultResType yExpr2 )
#else
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_G_TRResultResType
  ( yExpr1, yExpr2 )
  z_WSPConnectionMode_WTP_G_TRResultResType yExpr1, yExpr2;
#endif
{
  if ( yNEqF_pdubuf_ptr(yExpr1.ExitInfo, yExpr2.ExitInfo) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.Handle, yExpr2.Handle) ) return SDL_False;
  return SDL_True;
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern z_WSPConnectionMode_WTP_G_TRResultResType yMake_z_WSPConnectionMode_WTP_G_TRResultResType(
  pdubuf_ptr yExitInfo,
  SDL_Integer yHandle )
#else
extern z_WSPConnectionMode_WTP_G_TRResultResType  yMake_z_WSPConnectionMode_WTP_G_TRResultResType
  ( yExitInfo, yHandle )
  pdubuf_ptr yExitInfo;
  SDL_Integer yHandle;
#endif
{
  z_WSPConnectionMode_WTP_G_TRResultResType  yVar;
  memset((void *)(&yVar), 0, sizeof(z_WSPConnectionMode_WTP_G_TRResultResType));
  yAssF_pdubuf_ptr(yVar.ExitInfo, yExitInfo, XASSMAKE);
  yAssF_SDL_Integer(yVar.Handle, yHandle, XASSMAKE);
  return yVar;
}

/*****
* NEWTYPE TRAbortReqType  (BODY section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(2),170(40,50),4)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_z_WSPConnectionMode_WTP_H_TRAbortReqType(
   z_WSPConnectionMode_WTP_H_TRAbortReqType *yVar )
#else
extern void yDef_z_WSPConnectionMode_WTP_H_TRAbortReqType( yVar )
  z_WSPConnectionMode_WTP_H_TRAbortReqType *yVar;
#endif
{
  xDef_SDL_Natural(&((*yVar).AbortCode));
  xDef_SDL_Integer(&((*yVar).Handle));
}

/*------------------EQUAL-------------------*/
#ifndef XNOPROTO
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_H_TRAbortReqType(
  z_WSPConnectionMode_WTP_H_TRAbortReqType yExpr1,
  z_WSPConnectionMode_WTP_H_TRAbortReqType yExpr2 )
#else
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_H_TRAbortReqType
  ( yExpr1, yExpr2 )
  z_WSPConnectionMode_WTP_H_TRAbortReqType yExpr1, yExpr2;
#endif
{
  if ( yNEqF_SDL_Natural(yExpr1.AbortCode, yExpr2.AbortCode) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.Handle, yExpr2.Handle) ) return SDL_False;
  return SDL_True;
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern z_WSPConnectionMode_WTP_H_TRAbortReqType yMake_z_WSPConnectionMode_WTP_H_TRAbortReqType(
  SDL_Natural yAbortCode,
  SDL_Integer yHandle )
#else
extern z_WSPConnectionMode_WTP_H_TRAbortReqType  yMake_z_WSPConnectionMode_WTP_H_TRAbortReqType
  ( yAbortCode, yHandle )
  SDL_Natural yAbortCode;
  SDL_Integer yHandle;
#endif
{
  z_WSPConnectionMode_WTP_H_TRAbortReqType  yVar;
  memset((void *)(&yVar), 0, sizeof(z_WSPConnectionMode_WTP_H_TRAbortReqType));
  yAssF_SDL_Natural(yVar.AbortCode, yAbortCode, XASSMAKE);
  yAssF_SDL_Integer(yVar.Handle, yHandle, XASSMAKE);
  return yVar;
}

/*****
* NEWTYPE TRAbortIndType  (BODY section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(2),170(40,50),9)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_z_WSPConnectionMode_WTP_I_TRAbortIndType(
   z_WSPConnectionMode_WTP_I_TRAbortIndType *yVar )
#else
extern void yDef_z_WSPConnectionMode_WTP_I_TRAbortIndType( yVar )
  z_WSPConnectionMode_WTP_I_TRAbortIndType *yVar;
#endif
{
  xDef_SDL_Natural(&((*yVar).AbortCode));
  xDef_SDL_Integer(&((*yVar).Handle));
}

/*------------------EQUAL-------------------*/
#ifndef XNOPROTO
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_I_TRAbortIndType(
  z_WSPConnectionMode_WTP_I_TRAbortIndType yExpr1,
  z_WSPConnectionMode_WTP_I_TRAbortIndType yExpr2 )
#else
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_I_TRAbortIndType
  ( yExpr1, yExpr2 )
  z_WSPConnectionMode_WTP_I_TRAbortIndType yExpr1, yExpr2;
#endif
{
  if ( yNEqF_SDL_Natural(yExpr1.AbortCode, yExpr2.AbortCode) ) return SDL_False;
  if ( yNEqF_SDL_Integer(yExpr1.Handle, yExpr2.Handle) ) return SDL_False;
  return SDL_True;
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern z_WSPConnectionMode_WTP_I_TRAbortIndType yMake_z_WSPConnectionMode_WTP_I_TRAbortIndType(
  SDL_Natural yAbortCode,
  SDL_Integer yHandle )
#else
extern z_WSPConnectionMode_WTP_I_TRAbortIndType  yMake_z_WSPConnectionMode_WTP_I_TRAbortIndType
  ( yAbortCode, yHandle )
  SDL_Natural yAbortCode;
  SDL_Integer yHandle;
#endif
{
  z_WSPConnectionMode_WTP_I_TRAbortIndType  yVar;
  memset((void *)(&yVar), 0, sizeof(z_WSPConnectionMode_WTP_I_TRAbortIndType));
  yAssF_SDL_Natural(yVar.AbortCode, yAbortCode, XASSMAKE);
  yAssF_SDL_Integer(yVar.Handle, yHandle, XASSMAKE);
  return yVar;
}
