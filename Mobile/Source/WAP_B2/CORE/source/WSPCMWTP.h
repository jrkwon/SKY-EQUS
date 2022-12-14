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

#ifndef XX_WSPConnectionMode_WTP_H
#define XX_WSPConnectionMode_WTP_H

/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

/*****
* NEWTYPE TRInvokeReqType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),4)
******/
typedef struct z_WSPConnectionMode_WTP_B_TRInvokeReqType_s {
    AddressType  SourceAddress;
    AddressType  DestinationAddress;
    SDL_Integer  AckType;
    pdubuf_ptr  UserData;
    SDL_Integer  ClassType;
    SDL_Integer  Handle;
} z_WSPConnectionMode_WTP_B_TRInvokeReqType;

/*****
* NEWTYPE TRInvokeIndType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),13)
******/
typedef struct z_WSPConnectionMode_WTP_C_TRInvokeIndType_s {
    AddressType  SourceAddress;
    AddressType  DestinationAddress;
    SDL_Integer  AckType;
    pdubuf_ptr  UserData;
    SDL_Integer  ClassType;
    SDL_Integer  Handle;
} z_WSPConnectionMode_WTP_C_TRInvokeIndType;

/*****
* NEWTYPE TRInvokeResType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),22)
******/
typedef struct z_WSPConnectionMode_WTP_D_TRInvokeResType_s {
    pdubuf_ptr  ExitInfo;
    SDL_Integer  Handle;
} z_WSPConnectionMode_WTP_D_TRInvokeResType;

/*****
* NEWTYPE TRInvokeCnfType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),27)
******/
typedef struct z_WSPConnectionMode_WTP_E_TRInvokeCnfType_s {
    pdubuf_ptr  ExitInfo;
    SDL_Integer  Handle;
} z_WSPConnectionMode_WTP_E_TRInvokeCnfType;

/*****
* NEWTYPE TRResultIndType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),167(105,50),9)
******/
typedef struct z_WSPConnectionMode_WTP_F_TRResultIndType_s {
    pdubuf_ptr  UserData;
    SDL_Integer  Handle;
} z_WSPConnectionMode_WTP_F_TRResultIndType;

/*****
* NEWTYPE TRResultResType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),167(105,50),14)
******/
typedef struct z_WSPConnectionMode_WTP_G_TRResultResType_s {
    pdubuf_ptr  ExitInfo;
    SDL_Integer  Handle;
} z_WSPConnectionMode_WTP_G_TRResultResType;

/*****
* NEWTYPE TRAbortReqType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(2),170(40,50),4)
******/
typedef struct z_WSPConnectionMode_WTP_H_TRAbortReqType_s {
    SDL_Natural  AbortCode;
    SDL_Integer  Handle;
} z_WSPConnectionMode_WTP_H_TRAbortReqType;

/*****
* NEWTYPE TRAbortIndType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(2),170(40,50),9)
******/
typedef struct z_WSPConnectionMode_WTP_I_TRAbortIndType_s {
    SDL_Natural  AbortCode;
    SDL_Integer  Handle;
} z_WSPConnectionMode_WTP_I_TRAbortIndType;

/*****
* NEWTYPE TRInvokeReqType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),4)
******/

#define yAssF_z_WSPConnectionMode_WTP_B_TRInvokeReqType(V,E,A)  V = E
extern void yDef_z_WSPConnectionMode_WTP_B_TRInvokeReqType
  XPP(( z_WSPConnectionMode_WTP_B_TRInvokeReqType * ));
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_B_TRInvokeReqType
  XPP(( z_WSPConnectionMode_WTP_B_TRInvokeReqType,
  z_WSPConnectionMode_WTP_B_TRInvokeReqType ));
#define yEqF_z_WSPConnectionMode_WTP_B_TRInvokeReqType(Expr1,Expr2)  yEq_z_WSPConnectionMode_WTP_B_TRInvokeReqType(Expr1,Expr2)
#define yNEqF_z_WSPConnectionMode_WTP_B_TRInvokeReqType(Expr1,Expr2)  ( ! yEq_z_WSPConnectionMode_WTP_B_TRInvokeReqType(Expr1,Expr2) )
extern z_WSPConnectionMode_WTP_B_TRInvokeReqType yMake_z_WSPConnectionMode_WTP_B_TRInvokeReqType
  XPP(( AddressType, AddressType, SDL_Integer, pdubuf_ptr, SDL_Integer,
  SDL_Integer ));
#ifndef ANY_z_WSPConnectionMode_WTP_B_TRInvokeReqType
#define ANY_z_WSPConnectionMode_WTP_B_TRInvokeReqType  yMake_z_WSPConnectionMode_WTP_B_TRInvokeReqType(ANY_AddressType, \
  ANY_AddressType, ANY_SDL_Integer, ANY_pdubuf_ptr, ANY_SDL_Integer, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_B3_copy(P)  P

/*****
* NEWTYPE TRInvokeIndType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),13)
******/

#define yAssF_z_WSPConnectionMode_WTP_C_TRInvokeIndType(V,E,A)  V = E
extern void yDef_z_WSPConnectionMode_WTP_C_TRInvokeIndType
  XPP(( z_WSPConnectionMode_WTP_C_TRInvokeIndType * ));
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_C_TRInvokeIndType
  XPP(( z_WSPConnectionMode_WTP_C_TRInvokeIndType,
  z_WSPConnectionMode_WTP_C_TRInvokeIndType ));
#define yEqF_z_WSPConnectionMode_WTP_C_TRInvokeIndType(Expr1,Expr2)  yEq_z_WSPConnectionMode_WTP_C_TRInvokeIndType(Expr1,Expr2)
#define yNEqF_z_WSPConnectionMode_WTP_C_TRInvokeIndType(Expr1,Expr2)  ( ! yEq_z_WSPConnectionMode_WTP_C_TRInvokeIndType(Expr1,Expr2) )
extern z_WSPConnectionMode_WTP_C_TRInvokeIndType yMake_z_WSPConnectionMode_WTP_C_TRInvokeIndType
  XPP(( AddressType, AddressType, SDL_Integer, pdubuf_ptr, SDL_Integer,
  SDL_Integer ));
#ifndef ANY_z_WSPConnectionMode_WTP_C_TRInvokeIndType
#define ANY_z_WSPConnectionMode_WTP_C_TRInvokeIndType  yMake_z_WSPConnectionMode_WTP_C_TRInvokeIndType(ANY_AddressType, \
  ANY_AddressType, ANY_SDL_Integer, ANY_pdubuf_ptr, ANY_SDL_Integer, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_C3_copy(P)  P

/*****
* NEWTYPE TRInvokeResType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),22)
******/

#define yAssF_z_WSPConnectionMode_WTP_D_TRInvokeResType(V,E,A)  V = E
extern void yDef_z_WSPConnectionMode_WTP_D_TRInvokeResType
  XPP(( z_WSPConnectionMode_WTP_D_TRInvokeResType * ));
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_D_TRInvokeResType
  XPP(( z_WSPConnectionMode_WTP_D_TRInvokeResType,
  z_WSPConnectionMode_WTP_D_TRInvokeResType ));
#define yEqF_z_WSPConnectionMode_WTP_D_TRInvokeResType(Expr1,Expr2)  yEq_z_WSPConnectionMode_WTP_D_TRInvokeResType(Expr1,Expr2)
#define yNEqF_z_WSPConnectionMode_WTP_D_TRInvokeResType(Expr1,Expr2)  ( ! yEq_z_WSPConnectionMode_WTP_D_TRInvokeResType(Expr1,Expr2) )
extern z_WSPConnectionMode_WTP_D_TRInvokeResType yMake_z_WSPConnectionMode_WTP_D_TRInvokeResType
  XPP(( pdubuf_ptr, SDL_Integer ));
#ifndef ANY_z_WSPConnectionMode_WTP_D_TRInvokeResType
#define ANY_z_WSPConnectionMode_WTP_D_TRInvokeResType  yMake_z_WSPConnectionMode_WTP_D_TRInvokeResType(ANY_pdubuf_ptr, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_D3_copy(P)  P

/*****
* NEWTYPE TRInvokeCnfType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),164(25,50),27)
******/

#define yAssF_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(V,E,A)  V = E
extern void yDef_z_WSPConnectionMode_WTP_E_TRInvokeCnfType
  XPP(( z_WSPConnectionMode_WTP_E_TRInvokeCnfType * ));
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_E_TRInvokeCnfType
  XPP(( z_WSPConnectionMode_WTP_E_TRInvokeCnfType,
  z_WSPConnectionMode_WTP_E_TRInvokeCnfType ));
#define yEqF_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(Expr1,Expr2)  yEq_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(Expr1,Expr2)
#define yNEqF_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(Expr1,Expr2)  ( ! yEq_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(Expr1,Expr2) )
extern z_WSPConnectionMode_WTP_E_TRInvokeCnfType yMake_z_WSPConnectionMode_WTP_E_TRInvokeCnfType
  XPP(( pdubuf_ptr, SDL_Integer ));
#ifndef ANY_z_WSPConnectionMode_WTP_E_TRInvokeCnfType
#define ANY_z_WSPConnectionMode_WTP_E_TRInvokeCnfType  yMake_z_WSPConnectionMode_WTP_E_TRInvokeCnfType(ANY_pdubuf_ptr, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_E3_copy(P)  P

/*****
* NEWTYPE TRResultIndType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),167(105,50),9)
******/

#define yAssF_z_WSPConnectionMode_WTP_F_TRResultIndType(V,E,A)  V = E
extern void yDef_z_WSPConnectionMode_WTP_F_TRResultIndType
  XPP(( z_WSPConnectionMode_WTP_F_TRResultIndType * ));
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_F_TRResultIndType
  XPP(( z_WSPConnectionMode_WTP_F_TRResultIndType,
  z_WSPConnectionMode_WTP_F_TRResultIndType ));
#define yEqF_z_WSPConnectionMode_WTP_F_TRResultIndType(Expr1,Expr2)  yEq_z_WSPConnectionMode_WTP_F_TRResultIndType(Expr1,Expr2)
#define yNEqF_z_WSPConnectionMode_WTP_F_TRResultIndType(Expr1,Expr2)  ( ! yEq_z_WSPConnectionMode_WTP_F_TRResultIndType(Expr1,Expr2) )
extern z_WSPConnectionMode_WTP_F_TRResultIndType yMake_z_WSPConnectionMode_WTP_F_TRResultIndType
  XPP(( pdubuf_ptr, SDL_Integer ));
#ifndef ANY_z_WSPConnectionMode_WTP_F_TRResultIndType
#define ANY_z_WSPConnectionMode_WTP_F_TRResultIndType  yMake_z_WSPConnectionMode_WTP_F_TRResultIndType(ANY_pdubuf_ptr, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_F3_copy(P)  P

/*****
* NEWTYPE TRResultResType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),167(105,50),14)
******/

#define yAssF_z_WSPConnectionMode_WTP_G_TRResultResType(V,E,A)  V = E
extern void yDef_z_WSPConnectionMode_WTP_G_TRResultResType
  XPP(( z_WSPConnectionMode_WTP_G_TRResultResType * ));
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_G_TRResultResType
  XPP(( z_WSPConnectionMode_WTP_G_TRResultResType,
  z_WSPConnectionMode_WTP_G_TRResultResType ));
#define yEqF_z_WSPConnectionMode_WTP_G_TRResultResType(Expr1,Expr2)  yEq_z_WSPConnectionMode_WTP_G_TRResultResType(Expr1,Expr2)
#define yNEqF_z_WSPConnectionMode_WTP_G_TRResultResType(Expr1,Expr2)  ( ! yEq_z_WSPConnectionMode_WTP_G_TRResultResType(Expr1,Expr2) )
extern z_WSPConnectionMode_WTP_G_TRResultResType yMake_z_WSPConnectionMode_WTP_G_TRResultResType
  XPP(( pdubuf_ptr, SDL_Integer ));
#ifndef ANY_z_WSPConnectionMode_WTP_G_TRResultResType
#define ANY_z_WSPConnectionMode_WTP_G_TRResultResType  yMake_z_WSPConnectionMode_WTP_G_TRResultResType(ANY_pdubuf_ptr, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_G3_copy(P)  P

/*****
* NEWTYPE TRAbortReqType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(2),170(40,50),4)
******/

#define yAssF_z_WSPConnectionMode_WTP_H_TRAbortReqType(V,E,A)  V = E
extern void yDef_z_WSPConnectionMode_WTP_H_TRAbortReqType
  XPP(( z_WSPConnectionMode_WTP_H_TRAbortReqType * ));
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_H_TRAbortReqType
  XPP(( z_WSPConnectionMode_WTP_H_TRAbortReqType,
  z_WSPConnectionMode_WTP_H_TRAbortReqType ));
#define yEqF_z_WSPConnectionMode_WTP_H_TRAbortReqType(Expr1,Expr2)  yEq_z_WSPConnectionMode_WTP_H_TRAbortReqType(Expr1,Expr2)
#define yNEqF_z_WSPConnectionMode_WTP_H_TRAbortReqType(Expr1,Expr2)  ( ! yEq_z_WSPConnectionMode_WTP_H_TRAbortReqType(Expr1,Expr2) )
extern z_WSPConnectionMode_WTP_H_TRAbortReqType yMake_z_WSPConnectionMode_WTP_H_TRAbortReqType
  XPP(( SDL_Natural, SDL_Integer ));
#ifndef ANY_z_WSPConnectionMode_WTP_H_TRAbortReqType
#define ANY_z_WSPConnectionMode_WTP_H_TRAbortReqType  yMake_z_WSPConnectionMode_WTP_H_TRAbortReqType(ANY_SDL_Natural, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_H3_copy(P)  P

/*****
* NEWTYPE TRAbortIndType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(2),170(40,50),9)
******/

#define yAssF_z_WSPConnectionMode_WTP_I_TRAbortIndType(V,E,A)  V = E
extern void yDef_z_WSPConnectionMode_WTP_I_TRAbortIndType
  XPP(( z_WSPConnectionMode_WTP_I_TRAbortIndType * ));
extern SDL_Boolean yEq_z_WSPConnectionMode_WTP_I_TRAbortIndType
  XPP(( z_WSPConnectionMode_WTP_I_TRAbortIndType,
  z_WSPConnectionMode_WTP_I_TRAbortIndType ));
#define yEqF_z_WSPConnectionMode_WTP_I_TRAbortIndType(Expr1,Expr2)  yEq_z_WSPConnectionMode_WTP_I_TRAbortIndType(Expr1,Expr2)
#define yNEqF_z_WSPConnectionMode_WTP_I_TRAbortIndType(Expr1,Expr2)  ( ! yEq_z_WSPConnectionMode_WTP_I_TRAbortIndType(Expr1,Expr2) )
extern z_WSPConnectionMode_WTP_I_TRAbortIndType yMake_z_WSPConnectionMode_WTP_I_TRAbortIndType
  XPP(( SDL_Natural, SDL_Integer ));
#ifndef ANY_z_WSPConnectionMode_WTP_I_TRAbortIndType
#define ANY_z_WSPConnectionMode_WTP_I_TRAbortIndType  yMake_z_WSPConnectionMode_WTP_I_TRAbortIndType(ANY_SDL_Natural, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_I3_copy(P)  P

/*****
* SIGNAL ParaTimer
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(1),176(25,165),1)
******/
#define z_WSPConnectionMode_WTP_0_ParaTimer 167
typedef struct {
    SIGNAL_VARS
    SDL_Natural  Param1;
} yPDef_z_WSPConnectionMode_WTP_0_ParaTimer;

/*****
* SIGNAL TRInvokeReq
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(3),158(30,55),6)
******/
#define z_WSPConnectionMode_WTP_1_TRInvokeReq 168
typedef struct {
    SIGNAL_VARS
    z_WSPConnectionMode_WTP_B_TRInvokeReqType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_1_TRInvokeReq;

/*****
* SIGNAL TRResultRes
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(3),158(30,55),7)
******/
#define z_WSPConnectionMode_WTP_2_TRResultRes 169
typedef struct {
    SIGNAL_VARS
    z_WSPConnectionMode_WTP_G_TRResultResType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_2_TRResultRes;

/*****
* SIGNAL TRAbortReq
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(3),158(30,55),8)
******/
#define z_WSPConnectionMode_WTP_3_TRAbortReq 170
typedef struct {
    SIGNAL_VARS
    z_WSPConnectionMode_WTP_H_TRAbortReqType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_3_TRAbortReq;

/*****
* SIGNAL TRInvokeRes
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(3),158(30,55),9)
******/
#define z_WSPConnectionMode_WTP_4_TRInvokeRes 171
typedef struct {
    SIGNAL_VARS
    z_WSPConnectionMode_WTP_D_TRInvokeResType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_4_TRInvokeRes;

/*****
* SIGNAL TRInvokeCnf
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(3),161(30,145),5)
******/
#define z_WSPConnectionMode_WTP_5_TRInvokeCnf 172
typedef struct {
    SIGNAL_VARS
    z_WSPConnectionMode_WTP_E_TRInvokeCnfType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_5_TRInvokeCnf;

/*****
* SIGNAL TRResultInd
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(3),161(30,145),6)
******/
#define z_WSPConnectionMode_WTP_6_TRResultInd 173
typedef struct {
    SIGNAL_VARS
    z_WSPConnectionMode_WTP_F_TRResultIndType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_6_TRResultInd;

/*****
* SIGNAL TRAbortInd
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(3),161(30,145),7)
******/
#define z_WSPConnectionMode_WTP_7_TRAbortInd 174
typedef struct {
    SIGNAL_VARS
    z_WSPConnectionMode_WTP_I_TRAbortIndType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_7_TRAbortInd;

/*****
* SIGNAL TRInvokeInd
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WSPConnectionMode_WTP.sun(3),161(30,145),8)
******/
#define z_WSPConnectionMode_WTP_8_TRInvokeInd 175
typedef struct {
    SIGNAL_VARS
    z_WSPConnectionMode_WTP_C_TRInvokeIndType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_8_TRInvokeInd;

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif
