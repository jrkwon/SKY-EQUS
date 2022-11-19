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

#ifndef XX_WTAPackage_H
#define XX_WTAPackage_H

/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

extern void yInit_WTAPackage XPP((void));

/*****
* SIGNAL TerminateWTAService
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),512(30,40),9)
******/
#define z_WTAPackage_0_TerminateWTAService 120

/*****
* SIGNAL WTAExecuteService
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),512(30,40),10)
******/
#define z_WTAPackage_1_WTAExecuteService 121
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
} yPDef_z_WTAPackage_1_WTAExecuteService;

/*****
* SIGNAL WTAEvent
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),512(30,40),12)
******/
#define z_WTAPackage_2_WTAEvent 122
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
    z_ctypes_4_VoidStar  Param2;
} yPDef_z_WTAPackage_2_WTAEvent;

/*****
* SIGNAL WTAcConfirmInstallation
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),509(30,120),4)
******/
#define z_WTAPackage_3_WTAcConfirmInstallation 123
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_WTAPackage_3_WTAcConfirmInstallation;

/*****
* SIGNAL WTAcRetryGetInstallationResult
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),509(30,120),6)
******/
#define z_WTAPackage_4_WTAcRetryGetInstallationResult 124
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_WTAPackage_4_WTAcRetryGetInstallationResult;

/*****
* SIGNAL WTAcShowInstallationResult
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),509(30,120),8)
******/
#define z_WTAPackage_5_WTAcShowInstallationResult 125
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    SDL_Integer  Param2;
} yPDef_z_WTAPackage_5_WTAcShowInstallationResult;

/*****
* SIGNAL WTAcAbortInstallation
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),509(30,120),10)
******/
#define z_WTAPackage_6_WTAcAbortInstallation 126
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
} yPDef_z_WTAPackage_6_WTAcAbortInstallation;

/*****
* SIGNAL WTAcGetServices
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),509(30,120),12)
******/
#define z_WTAPackage_7_WTAcGetServices 127

/*****
* SIGNAL WTAcDeleteService
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),509(30,120),13)
******/
#define z_WTAPackage_8_WTAcDeleteService 128
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
} yPDef_z_WTAPackage_8_WTAcDeleteService;

/*****
* SIGNAL WTAcClearServices
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(EnvironmentSignals),509(30,120),15)
******/
#define z_WTAPackage_9_WTAcClearServices 129

/*****
* SIGNAL TerminateRepHandlerDone
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(InitSignals),266(30,40),7)
******/
#define z_WTAPackage_A_TerminateRepHandlerDone 130

/*****
* SIGNAL StartRepHandler
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(InitSignals),266(30,40),16)
******/
#define z_WTAPackage_B_StartRepHandler 131
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
} yPDef_z_WTAPackage_B_StartRepHandler;

/*****
* SIGNAL TerminateRepHandler
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(InitSignals),266(30,40),18)
******/
#define z_WTAPackage_C_TerminateRepHandler 132

/*****
* SIGNAL RepHResult
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(WSPIFSignals),434(25,45),8)
******/
#define z_WTAPackage_D_RepHResult 133
typedef struct {
    SIGNAL_VARS
    z_WAEPackage_2Y_ResponseType  Param1;
    SDL_PId  Param2;
    z_WAEPackage_2X_RequestType  Param3;
} yPDef_z_WTAPackage_D_RepHResult;

/*****
* SIGNAL RepHRequest
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(WSPIFSignals),434(25,45),19)
******/
#define z_WTAPackage_E_RepHRequest 134
typedef struct {
    SIGNAL_VARS
    z_WAEPackage_2X_RequestType  Param1;
    SDL_PId  Param2;
} yPDef_z_WTAPackage_E_RepHRequest;

/*****
* SIGNAL ReqInstallation
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(RepHSignals),392(30,45),8)
******/
#define z_WTAPackage_F_ReqInstallation 135
typedef struct {
    SIGNAL_VARS
    z_WAEPackage_2Y_ResponseType  Param1;
} yPDef_z_WTAPackage_F_ReqInstallation;

/*****
* SIGNAL ControlOfGlobalBinding
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(RepHSignals),392(30,45),9)
******/
#define z_WTAPackage_G_ControlOfGlobalBinding 136
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
} yPDef_z_WTAPackage_G_ControlOfGlobalBinding;

/*****
* SIGNAL InstallationResult
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(RepHSignals),392(30,45),19)
******/
#define z_WTAPackage_H_InstallationResult 137
typedef struct {
    SIGNAL_VARS
    z_WAEPackage_2Y_ResponseType  Param1;
} yPDef_z_WTAPackage_H_InstallationResult;

/*****
* SIGNAL GlobalBindingResult
* <<PACKAGE WTAPackage>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtapackage.sun(RepHSignals),392(30,45),21)
******/
#define z_WTAPackage_I_GlobalBindingResult 138
typedef struct {
    SIGNAL_VARS
    SDL_Integer  Param1;
    z_WAEPackage_2Y_ResponseType  Param2;
} yPDef_z_WTAPackage_I_GlobalBindingResult;

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif