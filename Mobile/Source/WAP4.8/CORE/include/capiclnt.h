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
#include "confvars.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
	#include "tapiclnt.h"
#endif

typedef BOOL  (*fPtr_Iana2Unicode_canConvert)( INT16 );
typedef INT32 (*fPtr_Iana2Unicode_calcLen)( BYTE*, INT16, BOOL, UINT32, UINT32* );
typedef BOOL  (*fPtr_Iana2Unicode_convert)( BYTE*, INT16, UINT32, WCHAR*, UINT32 );
typedef UINT8 (*fPtr_Iana2Unicode_getNullTermByteLen)( INT16 );


typedef enum    stackmode
{
    MODE_ANY          = 0,
    MODE_CL_WSP       = 9200,
    MODE_CL_WTLS      = 9202,
    MODE_CL_WTA       =   30,	/* just dummy value, not defined in WAP */
    MODE_CL_WTLS_WTA  = 2805,
    MODE_CO_WSP       = 9201,
    MODE_CO_WTLS      = 9203,
    MODE_CO_WTA       =   40,	/* just dummy value, not defined in WAP */
    MODE_CO_WTLS_WTA  = 2923
} stackmode;


#define BEARER_ANY_UDP   0
#define BEARER_GSM_USSD  2
#define BEARER_GSM_SMS   3
#define BEARER_GSM_CSD   10
#define BEARER_GSM_GPRS  11
#define BEARER_BT        150
#define BEARER_ANY       255

typedef enum
{
    /* setIntConfig */
    configHISTORY_SIZE,
    configCACHE_AGE,
    configCACHE_MODE,
    configDISPLAY_IMAGES,
    configUPDATE_IMAGES,
    configPUSH_SECURITY_LEVEL,
    configDEFAULT_CHANNEL,
	configCOOKIES_ENABLED,
	configBA_PERSISTENT_ENABLED,

    /* setDCHIntConfig */
    configACCESS_TYPE,
    configONLINE,
    configCLIENT_LOCAL_PORT,
    configUSSD_GW_TYPE,
    configTIMEOUT,
    configSTACKMODE,
	configEXPIRES,
	configMASTER_GW,
	configALLOW_WTLS_CLASS_1

} ConfigInt;


typedef enum 
{
    /* setStrConfig */
    configWSP_Language = configALLOW_WTLS_CLASS_1 + 1,
    configPROFILE,
    configPROFILE_DIFF,
    configUSERAGENT,

    /* setDCHStrConfig */
    configUDP_IP_SRC,
    configUDP_IP_GW,
    configSMS_C,
    configSMS_GW,
    configUSSD_C,
    configUSSD_GW,
    configAUTH_PASS_GW,
    configAUTH_ID_GW,
	configSTARTPAGE,
	configPROXY_PROVIDER_ID,
    configADD_HOST,
    configDELETE_HOST

} ConfigStr;


typedef struct variableType
{
    WCHAR   *name;
    WCHAR   *value;
} variableType;

/* Send modes in CLNTc_PostContent */
#define SENDMODE_URL_ENCODED		1
#define SENDMODE_MULTIPART_FORMDATA	2
#define SENDMODE_BINARY				3

IMPORT_C VOID    CLNTc_start (VOID);

IMPORT_C VOID    CLNTc_run (VOID);

IMPORT_C BOOL    CLNTc_wantsToRun (VOID);

IMPORT_C VOID    CLNTc_terminate (VOID);

IMPORT_C VOID    CLNTc_timerExpired (VOID);

IMPORT_C VOID    CLNTc_setIntConfig (UINT8 objectId, ConfigInt kind, UINT32 value);

IMPORT_C VOID    CLNTc_setStrConfig (UINT8 objectId, ConfigStr kind, const CHAR *value, UINT16 length);

IMPORT_C VOID    CLNTc_setDCHIntConfig (UINT8 objectId, UINT8 channelId, ConfigInt kind, UINT32 value);

IMPORT_C VOID    CLNTc_setDCHStrConfig (UINT8 objectId, UINT8 channelId, ConfigStr kind, const CHAR *value, UINT8 length);

IMPORT_C VOID    CLNTc_file (UINT8 fileId, CHAR *data, UINT16 length, const CHAR *contentType);

IMPORT_C VOID    CLNTc_functionResult (UINT8 functionId, const CHAR *data, UINT16 length, const CHAR *contentType, const variableType * const *resultVariables);

IMPORT_C VOID    CLNTc_getContent (const CHAR *url, UINT8 urlID, BOOL reload, const CHAR *acceptHeader);

IMPORT_C VOID    CLNTc_postContent (const CHAR *url, UINT8 urlID, BOOL reload, const CHAR *acceptHeader, const CHAR *data, UINT16 dataLen, BOOL moreData, const CHAR *contentType, UINT8 sendMode, const CHAR *contentDisp, UINT32 totalSize);

IMPORT_C VOID    CLNTc_postMoreContent (UINT8 urlID, const CHAR *data, UINT16 dataLen, BOOL moreData);

IMPORT_C VOID    CLNTc_acknowledgeContent (UINT8 urlID);

IMPORT_C VOID    CLNTc_cancelContent (UINT8 urlID);

IMPORT_C VOID    CLNTc_setTranscoders (fPtr_Iana2Unicode_canConvert canConvert, fPtr_Iana2Unicode_calcLen calcLen, fPtr_Iana2Unicode_convert convert, fPtr_Iana2Unicode_getNullTermByteLen nullLen);

IMPORT_C VOID    CLNTc_requestConnectionDone (UINT8 objectId, BOOL success );

IMPORT_C VOID    CLNTc_setupConnectionDone (UINT8 objectId, UINT8 channelId, BOOL success );

IMPORT_C VOID    CLNTc_closeConnection (UINT8 objectId, UINT8 channelId, BOOL deleteChannel );

#ifdef USE_PROPRIETARY_WMLS_LIBS

IMPORT_C VOID    CLNTc_WMLSLibFuncResponse (UINT16 invokeId, const WMLSvar *resultVar);

#endif

IMPORT_C VOID    CLNTc_decodeProvisioningData (const CHAR *data, UINT32 dataLen);

IMPORT_C VOID    CLNTc_e2eSecurityCnfgrd (UINT8 id, BOOL success );

IMPORT_C VOID    CLNTc_provSharedSecret (UINT16 iListID, CHAR* vcSharedSecret, UINT16 iLenSharedSecret, BOOL bLastTry);


#endif
