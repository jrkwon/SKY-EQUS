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


#ifndef XX_PushPackage_H
#define XX_PushPackage_H

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif
/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

extern void yInit_PushPackage XPP((void));

/*****
* SIGNAL InitPushRepository
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),4)
******/
#define z_PUSHPackage_0_InitPushRepository 144
typedef struct {
    SIGNAL_VARS
    z_ctypes_7_LongInt  Param1;
} yPDef_z_PUSHPackage_0_InitPushRepository;

/*****
* SIGNAL ClosePushRepository
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),6)
******/
#define z_PUSHPackage_1_ClosePushRepository 145
typedef struct {
    SIGNAL_VARS
    z_ctypes_7_LongInt  Param1;
} yPDef_z_PUSHPackage_1_ClosePushRepository;

/*****
* SIGNAL ChangeStatus
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),8)
******/
#define z_PUSHPackage_2_ChangeStatus 146
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_2_ChangeStatus;

/*****
* SIGNAL RemovePushMessage
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),10)
******/
#define z_PUSHPackage_3_RemovePushMessage 147
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
} yPDef_z_PUSHPackage_3_RemovePushMessage;

/*****
* SIGNAL LoadSI
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),12)
******/
#define z_PUSHPackage_4_LoadSI 148
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_4_LoadSI;

/*****
* SIGNAL LoadSL
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),14)
******/
#define z_PUSHPackage_5_LoadSL 149
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_5_LoadSL;

/*****
* SIGNAL GetSIinfo
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),16)
******/
#define z_PUSHPackage_6_GetSIinfo 150
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_6_GetSIinfo;

/*****
* SIGNAL GetSLinfo
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),18)
******/
#define z_PUSHPackage_7_GetSLinfo 151
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_7_GetSLinfo;

/*****
* SIGNAL DeleteSI
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),20)
******/
#define z_PUSHPackage_8_DeleteSI 152
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_8_DeleteSI;

/*****
* SIGNAL DeleteSL
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),22)
******/
#define z_PUSHPackage_9_DeleteSL 153
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_9_DeleteSL;

/*****
* SIGNAL PushReqConnectionDone
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),24)
******/
#define z_PUSHPackage_A_PushReqConnectionDone 154
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
    SDL_Boolean  Param3;
} yPDef_z_PUSHPackage_A_PushReqConnectionDone;

/*****
* SIGNAL CheckIfAnyUnloadedMsg
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(EnvironmentSignals),509(25,40),26)
******/
#define z_PUSHPackage_B_CheckIfAnyUnloadedMsg 155

/*****
* SIGNAL StartPush
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(InitSignals),470(30,45),8)
******/
#define z_PUSHPackage_C_StartPush 156
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
} yPDef_z_PUSHPackage_C_StartPush;

/*****
* SIGNAL TerminatePush
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(InitSignals),470(30,45),10)
******/
#define z_PUSHPackage_D_TerminatePush 157

/*****
* SIGNAL StartPushDone
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(InitSignals),470(30,45),19)
******/
#define z_PUSHPackage_E_StartPushDone 158

/*****
* SIGNAL TerminatePushDone
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(InitSignals),470(30,45),20)
******/
#define z_PUSHPackage_F_TerminatePushDone 159

/*****
* SIGNAL WSPNewSession
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(WSPIFSignals),512(25,45),10)
******/
#define z_PUSHPackage_G_WSPNewSession 160
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_G_WSPNewSession;

/*****
* SIGNAL WSPTerminateSession
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(WSPIFSignals),512(25,45),12)
******/
#define z_PUSHPackage_H_WSPTerminateSession 161
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_PUSHPackage_H_WSPTerminateSession;

/*****
* SIGNAL PushIndicationResp
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(WSPIFSignals),512(25,45),14)
******/
#define z_PUSHPackage_I_PushIndicationResp 162
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
    SDL_Integer  Param3;
    SDL_Integer  Param4;
    SDL_Integer  Param5;
} yPDef_z_PUSHPackage_I_PushIndicationResp;

/*****
* SIGNAL PushIndication
* <<PACKAGE PushPackage>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\pushpackage.sun(WSPIFSignals),512(25,45),24)
******/
#define z_PUSHPackage_J_PushIndication 163
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
    SDL_Integer  Param3;
    SDL_Boolean  Param4;
    z_ctypes_4_VoidStar  Param5;
    SDL_Integer  Param6;
    z_ctypes_4_VoidStar  Param7;
    SDL_Integer  Param8;
    SDL_Integer  Param9;
} yPDef_z_PUSHPackage_J_PushIndication;

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif
