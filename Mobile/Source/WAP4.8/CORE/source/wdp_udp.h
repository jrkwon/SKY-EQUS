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


#ifndef XX_WDP_UDP_H
#define XX_WDP_UDP_H

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif
/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

extern void yInit_WDP_UDP XPP((void));

/*****
* SIGNAL RecUDPDatagram
* <<PACKAGE WDP_UDP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wdp_udp.sun(1),125(25,85),3)
******/
#define z_WDP_UDP_0_RecUDPDatagram 209
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
    SDL_Natural  Param2;
    DeviceAddress  Param3;
    DeviceAddress  Param4;
    SDL_Natural  Param5;
    SDL_Natural  Param6;
    SDL_Octet  Param7;
} yPDef_z_WDP_UDP_0_RecUDPDatagram;

/*****
* SIGNAL UDPErrorInd
* <<PACKAGE WDP_UDP>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wdp_udp.sun(1),125(25,85),8)
******/
#define z_WDP_UDP_1_UDPErrorInd 210
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Natural  Param2;
} yPDef_z_WDP_UDP_1_UDPErrorInd;

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif
