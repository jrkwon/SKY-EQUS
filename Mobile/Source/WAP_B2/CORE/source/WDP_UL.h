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

#ifndef XX_WDP_UpperLayer_H
#define XX_WDP_UpperLayer_H

/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

/*****
* NEWTYPE TDUnitdataReqType  (TYPE section)
* <<PACKAGE WDP_UpperLayer>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WDP_UpperLayer.sun(1),173(55,25),3)
******/
typedef struct TDUnitdataReqType_s {
    AddressType  SourceAddress;
    AddressType  DestinationAddress;
    pdubuf_ptr  UserData;
} TDUnitdataReqType;

/*****
* NEWTYPE TDUnitDataIndType  (TYPE section)
* <<PACKAGE WDP_UpperLayer>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WDP_UpperLayer.sun(1),164(20,60),3)
******/
typedef struct TDUnitdataIndType_s {
    AddressType  SourceAddress;
    AddressType  DestinationAddress;
    pdubuf_ptr  UserData;
} TDUnitdataIndType;

/*****
* NEWTYPE TDUnitdataReqType  (HEADING section)
* <<PACKAGE WDP_UpperLayer>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WDP_UpperLayer.sun(1),173(55,25),3)
******/

#define yAssF_TDUnitdataReqType(V,E,A)  V = E
extern void yDef_TDUnitdataReqType XPP(( TDUnitdataReqType * ));
extern SDL_Boolean yEq_TDUnitdataReqType XPP(( TDUnitdataReqType,
  TDUnitdataReqType ));
#define yEqF_TDUnitdataReqType(Expr1,Expr2)  yEq_TDUnitdataReqType(Expr1,Expr2)
#define yNEqF_TDUnitdataReqType(Expr1,Expr2)  ( ! yEq_TDUnitdataReqType(Expr1,Expr2) )
extern TDUnitdataReqType yMake_TDUnitdataReqType XPP(( AddressType,
  AddressType, pdubuf_ptr ));
#ifndef ANY_TDUnitdataReqType
#define ANY_TDUnitdataReqType  yMake_TDUnitdataReqType(ANY_AddressType, \
  ANY_AddressType, ANY_pdubuf_ptr)
#endif
#define z_WDP_UpperLayer_33_copy(P)  P

/*****
* NEWTYPE TDUnitDataIndType  (HEADING section)
* <<PACKAGE WDP_UpperLayer>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WDP_UpperLayer.sun(1),164(20,60),3)
******/

#define yAssF_TDUnitdataIndType(V,E,A)  V = E
extern void yDef_TDUnitdataIndType XPP(( TDUnitdataIndType * ));
extern SDL_Boolean yEq_TDUnitdataIndType XPP(( TDUnitdataIndType,
  TDUnitdataIndType ));
#define yEqF_TDUnitdataIndType(Expr1,Expr2)  yEq_TDUnitdataIndType(Expr1,Expr2)
#define yNEqF_TDUnitdataIndType(Expr1,Expr2)  ( ! yEq_TDUnitdataIndType(Expr1,Expr2) )
extern TDUnitdataIndType yMake_TDUnitdataIndType XPP(( AddressType,
  AddressType, pdubuf_ptr ));
#ifndef ANY_TDUnitdataIndType
#define ANY_TDUnitdataIndType  yMake_TDUnitdataIndType(ANY_AddressType, \
  ANY_AddressType, ANY_pdubuf_ptr)
#endif
#define z_WDP_UpperLayer_43_copy(P)  P

/*****
* SIGNAL TDUnitdataReq
* <<PACKAGE WDP_UpperLayer>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WDP_UpperLayer.sun(1),176(20,110),3)
******/
#define z_WDP_UpperLayer_0_TDUnitdataReq 164
typedef struct {
    SIGNAL_VARS
    TDUnitdataReqType  Param1;
    SDL_PId  Param2;
    SDL_Natural  Param3;
} yPDef_z_WDP_UpperLayer_0_TDUnitdataReq;

/*****
* SIGNAL TDUnitDataInd
* <<PACKAGE WDP_UpperLayer>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WDP_UpperLayer.sun(1),167(20,130),3)
******/
#define z_WDP_UpperLayer_1_TDUnitDataInd 165
typedef struct {
    SIGNAL_VARS
    TDUnitdataIndType  Param1;
    SDL_Natural  Param2;
} yPDef_z_WDP_UpperLayer_1_TDUnitDataInd;

/*****
* SIGNAL AbortRequestWDP
* <<PACKAGE WDP_UpperLayer>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\WDP_UpperLayer.sun(1),185(20,195),4)
******/
#define z_WDP_UpperLayer_2_AbortRequestWDP 166
typedef struct {
    SIGNAL_VARS
    SDL_Natural  Param1;
} yPDef_z_WDP_UpperLayer_2_AbortRequestWDP;

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif
