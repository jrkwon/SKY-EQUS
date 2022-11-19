/*
 * Copyright (C) Ericsson Mobile Communications AB, 2001.
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


#ifndef XX_WSPConnectionMode_WTP_H
#define XX_WSPConnectionMode_WTP_H

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif
/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

extern void yInit_WSPConnectionMode_WTP XPP((void));

/*****
* NEWTYPE TRInvokeReqType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(1),164(25,50),3)
******/
typedef struct TRInvokeReqType_s {
    AddressType  SourceAddress;
    AddressType  DestinationAddress;
    SDL_Integer  AckType;
    pdubuf_ptr  UserData;
    SDL_Integer  ClassType;
    SDL_Integer  Handle;
    SDL_Boolean  AllowSegmentedResult;
    SDL_Boolean  MoreData;
    SDL_Natural  TotalSize;
} TRInvokeReqType;

/*****
* NEWTYPE TRInvokeIndType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(1),164(25,50),15)
******/
typedef struct TRInvokeIndType_s {
    AddressType  SourceAddress;
    AddressType  DestinationAddress;
    SDL_Integer  AckType;
    pdubuf_ptr  UserData;
    SDL_Integer  ClassType;
    SDL_Integer  Handle;
} TRInvokeIndType;

/*****
* NEWTYPE TRResultIndType  (TYPE section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(1),164(25,50),24)
******/
typedef struct TRResultIndType_s {
    pdubuf_ptr  UserData;
    SDL_Integer  Handle;
    SDL_Boolean  MoreData;
    SDL_Natural  TotalSize;
} TRResultIndType;

/*****
* NEWTYPE TRInvokeReqType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(1),164(25,50),3)
******/

#define yAssF_TRInvokeReqType(V,E,A)  V = E
extern void yDef_TRInvokeReqType XPP(( TRInvokeReqType * ));
extern SDL_Boolean yEq_TRInvokeReqType XPP(( TRInvokeReqType,
  TRInvokeReqType ));
#define yEqF_TRInvokeReqType(Expr1,Expr2)  yEq_TRInvokeReqType(Expr1,Expr2)
#define yNEqF_TRInvokeReqType(Expr1,Expr2)  ( ! yEq_TRInvokeReqType(Expr1,Expr2) )
extern TRInvokeReqType yMake_TRInvokeReqType XPP(( AddressType, AddressType,
  SDL_Integer, pdubuf_ptr, SDL_Integer, SDL_Integer, SDL_Boolean, SDL_Boolean,
  SDL_Natural ));
#ifndef ANY_TRInvokeReqType
#define ANY_TRInvokeReqType  yMake_TRInvokeReqType(ANY_AddressType, \
  ANY_AddressType, ANY_SDL_Integer, ANY_pdubuf_ptr, ANY_SDL_Integer, \
  ANY_SDL_Integer, ANY_SDL_Boolean, ANY_SDL_Boolean, ANY_SDL_Natural)
#endif
#define z_WSPConnectionMode_WTP_C3_copy(P)  P

/*****
* NEWTYPE TRInvokeIndType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(1),164(25,50),15)
******/

#define yAssF_TRInvokeIndType(V,E,A)  V = E
extern void yDef_TRInvokeIndType XPP(( TRInvokeIndType * ));
extern SDL_Boolean yEq_TRInvokeIndType XPP(( TRInvokeIndType,
  TRInvokeIndType ));
#define yEqF_TRInvokeIndType(Expr1,Expr2)  yEq_TRInvokeIndType(Expr1,Expr2)
#define yNEqF_TRInvokeIndType(Expr1,Expr2)  ( ! yEq_TRInvokeIndType(Expr1,Expr2) )
extern TRInvokeIndType yMake_TRInvokeIndType XPP(( AddressType, AddressType,
  SDL_Integer, pdubuf_ptr, SDL_Integer, SDL_Integer ));
#ifndef ANY_TRInvokeIndType
#define ANY_TRInvokeIndType  yMake_TRInvokeIndType(ANY_AddressType, \
  ANY_AddressType, ANY_SDL_Integer, ANY_pdubuf_ptr, ANY_SDL_Integer, \
  ANY_SDL_Integer)
#endif
#define z_WSPConnectionMode_WTP_D3_copy(P)  P

/*****
* NEWTYPE TRResultIndType  (HEADING section)
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(1),164(25,50),24)
******/

#define yAssF_TRResultIndType(V,E,A)  V = E
extern void yDef_TRResultIndType XPP(( TRResultIndType * ));
extern SDL_Boolean yEq_TRResultIndType XPP(( TRResultIndType,
  TRResultIndType ));
#define yEqF_TRResultIndType(Expr1,Expr2)  yEq_TRResultIndType(Expr1,Expr2)
#define yNEqF_TRResultIndType(Expr1,Expr2)  ( ! yEq_TRResultIndType(Expr1,Expr2) )
extern TRResultIndType yMake_TRResultIndType XPP(( pdubuf_ptr, SDL_Integer,
  SDL_Boolean, SDL_Natural ));
#ifndef ANY_TRResultIndType
#define ANY_TRResultIndType  yMake_TRResultIndType(ANY_pdubuf_ptr, \
  ANY_SDL_Integer, ANY_SDL_Boolean, ANY_SDL_Natural)
#endif
#define z_WSPConnectionMode_WTP_E3_copy(P)  P

/*****
* SIGNAL TRInvokeReq
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),158(30,55),3)
******/
#define z_WSPConnectionMode_WTP_0_TRInvokeReq 211
typedef struct {
    SIGNAL_VARS
    TRInvokeReqType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_0_TRInvokeReq;

/*****
* SIGNAL TRResultRes
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),158(30,55),4)
******/
#define z_WSPConnectionMode_WTP_1_TRResultRes 212
typedef struct {
    SIGNAL_VARS
    pdubuf_ptr  Param1;
    SDL_Integer  Param2;
    SDL_Natural  Param3;
} yPDef_z_WSPConnectionMode_WTP_1_TRResultRes;

/*****
* SIGNAL TRAbortReq
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),158(30,55),5)
******/
#define z_WSPConnectionMode_WTP_2_TRAbortReq 213
typedef struct {
    SIGNAL_VARS
    SDL_Natural  Param1;
    SDL_Integer  Param2;
    SDL_Natural  Param3;
} yPDef_z_WSPConnectionMode_WTP_2_TRAbortReq;

/*****
* SIGNAL TRInvokeRes
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),158(30,55),6)
******/
#define z_WSPConnectionMode_WTP_3_TRInvokeRes 214
typedef struct {
    SIGNAL_VARS
    pdubuf_ptr  Param1;
    SDL_Integer  Param2;
    SDL_Natural  Param3;
} yPDef_z_WSPConnectionMode_WTP_3_TRInvokeRes;

/*****
* SIGNAL TRAckReq
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),158(30,55),7)
******/
#define z_WSPConnectionMode_WTP_4_TRAckReq 215
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
} yPDef_z_WSPConnectionMode_WTP_4_TRAckReq;

/*****
* SIGNAL TRInvokeCnf
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),161(30,125),3)
******/
#define z_WSPConnectionMode_WTP_5_TRInvokeCnf 216
typedef struct {
    SIGNAL_VARS
    pdubuf_ptr  Param1;
    SDL_Integer  Param2;
    SDL_Natural  Param3;
} yPDef_z_WSPConnectionMode_WTP_5_TRInvokeCnf;

/*****
* SIGNAL TRResultInd
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),161(30,125),4)
******/
#define z_WSPConnectionMode_WTP_6_TRResultInd 217
typedef struct {
    SIGNAL_VARS
    TRResultIndType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_6_TRResultInd;

/*****
* SIGNAL TRAbortInd
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),161(30,125),5)
******/
#define z_WSPConnectionMode_WTP_7_TRAbortInd 218
typedef struct {
    SIGNAL_VARS
    SDL_Natural  Param1;
    SDL_Integer  Param2;
    SDL_Natural  Param3;
} yPDef_z_WSPConnectionMode_WTP_7_TRAbortInd;

/*****
* SIGNAL TRInvokeInd
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),161(30,125),6)
******/
#define z_WSPConnectionMode_WTP_8_TRInvokeInd 219
typedef struct {
    SIGNAL_VARS
    TRInvokeIndType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WSPConnectionMode_WTP_8_TRInvokeInd;

/*****
* SIGNAL TRAckInd
* <<PACKAGE WSPConnectionMode_WTP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wspconnectionmode_wtp.sun(2),161(30,125),7)
******/
#define z_WSPConnectionMode_WTP_9_TRAckInd 220
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
} yPDef_z_WSPConnectionMode_WTP_9_TRAckInd;

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif