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

#ifndef XX_WTLS_H
#define XX_WTLS_H

/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

/*****
* SYNONYM MAX_WTLS_CONNECTIONS
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(5),194(50,60),2)
******/
#define z_WTLS_13_MAX_WTLS_CONNECTIONS SDL_INTEGER_LIT(15)

/*****
* SYNONYM SECURE_PUSH_PORT
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(5),197(50,80),2)
******/
#define z_WTLS_14_SECURE_PUSH_PORT SDL_INTEGER_LIT(2949)

/*****
* SIGNAL StartMain
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(1),146(50,60),2)
******/
#define z_WTLS_00_StartMain 210

/*****
* SIGNAL TerminateMain
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(1),146(50,60),3)
******/
#define z_WTLS_01_TerminateMain 211

/*****
* SIGNAL MainReady
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(1),146(50,60),4)
******/
#define z_WTLS_02_MainReady 212

/*****
* SIGNAL VerifyCertificateChainResponse
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),209(30,45),2)
******/
#define VerifyCertificateChainResponse 213
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_Integer  Param2;
} yPDef_VerifyCertificateChainResponse;

/*****
* SIGNAL KeyExchangeResponse
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),209(30,45),3)
******/
#define KeyExchangeResponse 214
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_Integer  Param2;
    SDL_Octet  Param3;
    z_ctypes_4_VoidStar  Param4;
    SDL_Natural  Param5;
} yPDef_KeyExchangeResponse;

/*****
* SIGNAL PRFResponse
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),209(30,45),5)
******/
#define PRFResponse 215
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_Integer  Param2;
    z_ctypes_4_VoidStar  Param3;
    SDL_Natural  Param4;
} yPDef_PRFResponse;

/*****
* SIGNAL GetMethodsResponse
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),209(30,45),6)
******/
#define GetMethodsResponse 216
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_Integer  Param2;
    z_ctypes_4_VoidStar  Param3;
    SDL_Natural  Param4;
    z_ctypes_4_VoidStar  Param5;
    SDL_Natural  Param6;
    z_ctypes_4_VoidStar  Param7;
    SDL_Natural  Param8;
} yPDef_GetMethodsResponse;

/*****
* SIGNAL GetClientCertificateResponse
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),209(30,45),8)
******/
#define GetClientCertificateResponse 217
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_Integer  Param2;
    z_ctypes_4_VoidStar  Param3;
    SDL_Natural  Param4;
    z_ctypes_4_VoidStar  Param5;
    SDL_Natural  Param6;
} yPDef_GetClientCertificateResponse;

/*****
* SIGNAL ComputeSignatureResponse
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),209(30,45),10)
******/
#define ComputeSignatureResponse 218
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_Integer  Param2;
    z_ctypes_4_VoidStar  Param3;
    SDL_Natural  Param4;
} yPDef_ComputeSignatureResponse;

/*****
* SIGNAL PeerLookupResponse
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),209(30,45),12)
******/
#define PeerLookupResponse 219
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_Integer  Param2;
    SDL_Octet  Param3;
} yPDef_PeerLookupResponse;

/*****
* SIGNAL SessionFetchResponse
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),209(30,45),13)
******/
#define SessionFetchResponse 220
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_Integer  Param2;
    SDL_Octet  Param3;
    z_ctypes_4_VoidStar  Param4;
    SDL_Octet  Param5;
    SDL_Octet  Param6;
    SDL_Octet  Param7;
    SDL_Octet  Param8;
    z_ctypes_4_VoidStar  Param9;
    SDL_Natural  Param10;
} yPDef_SessionFetchResponse;

/*****
* SIGNAL StartCM
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),173(30,140),2)
******/
#define z_WTLS_0B_StartCM 221
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    AddressType  Param2;
    AddressType  Param3;
    SDL_Integer  Param4;
} yPDef_z_WTLS_0B_StartCM;

/*****
* SIGNAL CMReady
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),173(30,140),3)
******/
#define z_WTLS_0C_CMReady 222

/*****
* SIGNAL StartHandshake
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),173(30,140),4)
******/
#define z_WTLS_0D_StartHandshake 223

/*****
* SIGNAL OutboundMsgs
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),173(30,140),5)
******/
#define z_WTLS_0E_OutboundMsgs 224

/*****
* SIGNAL TerminateInd
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),173(30,140),6)
******/
#define z_WTLS_0F_TerminateInd 225

/*****
* SIGNAL TerminateReq
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(2),173(30,140),7)
******/
#define z_WTLS_0G_TerminateReq 226

/*****
* SIGNAL StartRec
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(3),245(50,55),2)
******/
#define z_WTLS_0H_StartRec 227
typedef struct {
    SIGNAL_VARS
    SDL_PId  Param1;
    SDL_PId  Param2;
    SDL_Natural  Param3;
    z_ctypes_4_VoidStar  Param4;
} yPDef_z_WTLS_0H_StartRec;

/*****
* SIGNAL RecReady
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(3),245(50,55),3)
******/
#define z_WTLS_0I_RecReady 228

/*****
* SIGNAL IncomingMsgs
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(3),245(50,55),4)
******/
#define z_WTLS_0J_IncomingMsgs 229

/*****
* SIGNAL AlertIndication
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),2)
******/
#define z_WTLS_0K_AlertIndication 230
typedef struct {
    SIGNAL_VARS
    SDL_Natural  Param1;
    SDL_Natural  Param2;
} yPDef_z_WTLS_0K_AlertIndication;

/*****
* SIGNAL AppendHandshakeMessage
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),3)
******/
#define z_WTLS_0L_AppendHandshakeMessage 231
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
    SDL_Natural  Param2;
    SDL_Boolean  Param3;
} yPDef_z_WTLS_0L_AppendHandshakeMessage;

/*****
* SIGNAL AppendCCS
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),4)
******/
#define z_WTLS_0M_AppendCCS 232

/*****
* SIGNAL FlushBuffer
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),5)
******/
#define z_WTLS_0N_FlushBuffer 233

/*****
* SIGNAL ClearBuffer
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),6)
******/
#define z_WTLS_0O_ClearBuffer 234

/*****
* SIGNAL SendAlert
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),7)
******/
#define z_WTLS_0P_SendAlert 235
typedef struct {
    SIGNAL_VARS
    SDL_Natural  Param1;
    SDL_Natural  Param2;
} yPDef_z_WTLS_0P_SendAlert;

/*****
* SIGNAL SendData
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),8)
******/
#define z_WTLS_0Q_SendData 236
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
} yPDef_z_WTLS_0Q_SendData;

/*****
* SIGNAL SendDataWithBuffer
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),9)
******/
#define z_WTLS_0R_SendDataWithBuffer 237
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
} yPDef_z_WTLS_0R_SendDataWithBuffer;

/*****
* SIGNAL MessageIndication
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),10)
******/
#define z_WTLS_0S_MessageIndication 238
typedef struct {
    SIGNAL_VARS
    SDL_Natural  Param1;
    z_ctypes_4_VoidStar  Param2;
    SDL_Natural  Param3;
} yPDef_z_WTLS_0S_MessageIndication;

/*****
* SIGNAL MessageRequest
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),11)
******/
#define z_WTLS_0T_MessageRequest 239
typedef struct {
    SIGNAL_VARS
    SDL_Boolean  Param1;
} yPDef_z_WTLS_0T_MessageRequest;

/*****
* SIGNAL ErrorIndication
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),12)
******/
#define z_WTLS_0U_ErrorIndication 240

/*****
* SIGNAL TimeRequired
* <<PACKAGE WTLS>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtls.sun(4),122(35,60),13)
******/
#define z_WTLS_0V_TimeRequired 241

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
#endif
