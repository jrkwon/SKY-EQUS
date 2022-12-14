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

#ifndef XX_UDCP_USSD_H
#define XX_UDCP_USSD_H

/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

/*****
* SIGNAL ProcUSSDReqReq
* <<PACKAGE UDCP_USSD>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\UDCP_USSD.sun(1),173(30,45),4)
******/
#define z_UDCP_USSD_0_ProcUSSDReqReq 176
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Octet_String  Param2;
    SDL_Octet  Param3;
} yPDef_z_UDCP_USSD_0_ProcUSSDReqReq;

/*****
* SIGNAL USSDReqRes
* <<PACKAGE UDCP_USSD>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\UDCP_USSD.sun(1),173(30,45),10)
******/
#define z_UDCP_USSD_1_USSDReqRes 177
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Octet_String  Param2;
    SDL_Octet  Param3;
} yPDef_z_UDCP_USSD_1_USSDReqRes;

/*****
* SIGNAL USSDAbortReq
* <<PACKAGE UDCP_USSD>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\UDCP_USSD.sun(1),173(30,45),16)
******/
#define z_UDCP_USSD_2_USSDAbortReq 178

/*****
* SIGNAL ProcUSSDReqCnf
* <<PACKAGE UDCP_USSD>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\UDCP_USSD.sun(1),176(30,125),4)
******/
#define z_UDCP_USSD_3_ProcUSSDReqCnf 179
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Octet_String  Param2;
    SDL_Octet  Param3;
} yPDef_z_UDCP_USSD_3_ProcUSSDReqCnf;

/*****
* SIGNAL USSDReqInd
* <<PACKAGE UDCP_USSD>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\UDCP_USSD.sun(1),176(30,125),11)
******/
#define z_UDCP_USSD_4_USSDReqInd 180
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Octet_String  Param2;
    SDL_Octet  Param3;
} yPDef_z_UDCP_USSD_4_USSDReqInd;

/*****
* SIGNAL USSDErrorInd
* <<PACKAGE UDCP_USSD>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\UDCP_USSD.sun(1),176(30,125),18)
******/
#define z_UDCP_USSD_5_USSDErrorInd 181
typedef struct {
    SIGNAL_VARS
    SDL_Octet  Param1;
    SDL_Boolean  Param2;
} yPDef_z_UDCP_USSD_5_USSDErrorInd;

/*****
* SIGNAL USSDReleaseInd
* <<PACKAGE UDCP_USSD>>
* #SDTREF(SDL,D:\WIP\Maintenance\WAP_1.1\System\base_co\sdl\packages\UDCP_USSD.sun(1),176(30,125),21)
******/
#define z_UDCP_USSD_6_USSDReleaseInd 182

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif
