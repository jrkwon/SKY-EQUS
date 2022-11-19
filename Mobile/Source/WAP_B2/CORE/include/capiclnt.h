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
#ifndef CAPICLNT_H
#define CAPICLNT_H


#include "tapicmmn.h"
#include "wipprdct.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
#include "tapiclnt.h"
#endif

typedef BOOL  (*fPtr_Iana2Unicode_canConvert)( INT16 );
typedef INT32 (*fPtr_Iana2Unicode_calcLen)( BYTE*, INT16, BOOL, UINT32, UINT32* );
typedef BOOL  (*fPtr_Iana2Unicode_convert)( BYTE*, INT16, UINT32, WCHAR*, UINT32 );
typedef UINT8 (*fPtr_Iana2Unicode_getNullTermByteLen)( INT16 );


typedef enum    stackmode
{
    MODE_CL_WSP       = 9200,
    MODE_CL_WTLS      = 9202,
    MODE_CL_WTA       =   30,	/* just dummy value, not defined in WAP */
    MODE_CL_WTLS_WTA  = 2805,
    MODE_CO_WSP       = 9201,
    MODE_CO_WTLS      = 9203,
    MODE_CO_WTA       =   40,	/* just dummy value, not defined in WAP */
    MODE_CO_WTLS_WTA  = 2923 	
} stackmode;


typedef INT8    BearerType;
#define BEARER_UDP  (BearerType)0
#define BEARER_USSD (BearerType)2
#define BEARER_SMS  (BearerType)3
#define BEARER_WD   (BearerType)255

typedef enum    ConfigInt
{
    configSTACKMODE,
    configBEARER,
    configUSSD_SC_TYPE,
    configHISTORY_SIZE,
    configTIMEOUT,
    configCACHE_AGE,
    configCACHE_MODE,
    configDISPLAY_IMAGES,
    configUPDATE_IMAGES,
    configQ_OTA
} ConfigInt;


typedef enum    ConfigStr
{
    configUDP_IP_SRC = configQ_OTA + 1,
    configUDP_IP_GW,
    configSMS_C,
    configSMS_GW,
    configUSSD_C,
    configUSSD_SC,
    configAUTH_PASS_GW,
    configAUTH_ID_GW,
    configWSP_Language,
    configPROFILE,
	configPROFILE_DIFF
} ConfigStr;


typedef struct variableType
{
    WCHAR   *name;
    WCHAR   *value;
} variableType;


IMPORT_C VOID    CLNTc_start (VOID);

IMPORT_C VOID    CLNTc_initialised (VOID);

IMPORT_C VOID    CLNTc_run (VOID);

IMPORT_C BOOL    CLNTc_wantsToRun (VOID);

IMPORT_C VOID    CLNTc_terminate (VOID);

IMPORT_C VOID    CLNTc_timerExpired (VOID);

IMPORT_C VOID    CLNTc_setIntConfig (UINT8 viewId, ConfigInt kind, UINT32 value);

IMPORT_C VOID    CLNTc_setStrConfig (UINT8 viewId, ConfigStr kind, const CHAR *value, UINT8 length);

IMPORT_C VOID    CLNTc_usePort (UINT8 viewId, UINT8 id, UINT16 port, BOOL success);

IMPORT_C VOID    CLNTc_closePort (UINT16 port);

IMPORT_C VOID    CLNTc_file (UINT8 fileId, CHAR *data, UINT16 length, const CHAR *contentType);

IMPORT_C VOID    CLNTc_functionResult (UINT8 functionId, const CHAR *data, UINT16 length, const CHAR *contentType, const variableType * const *resultVariables);

IMPORT_C VOID    CLNTc_getContent (const CHAR *url, UINT8 urlID, BOOL isOTAallowed);

IMPORT_C VOID    CLNTc_confirmDownload (INT16 confirmId, BOOL isOTAallowed);

IMPORT_C VOID    CLNTc_setTranscoders (fPtr_Iana2Unicode_canConvert canConvert, fPtr_Iana2Unicode_calcLen calcLen, fPtr_Iana2Unicode_convert convert, fPtr_Iana2Unicode_getNullTermByteLen nullLen);

#ifdef USE_PROPRIETARY_WMLS_LIBS

IMPORT_C VOID    CLNTc_WMLSLibFuncResponse (UINT16 invokeId, const WMLSvar *resultVar);

#endif


#endif
