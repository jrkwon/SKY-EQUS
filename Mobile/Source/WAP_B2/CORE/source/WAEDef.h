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

#ifndef WAEDEF_H
#define WAEDEF_H

#include "cmmnrsrc.h"
#include "confvars.h"
#include "aapiclnt.h"
#include "wipprdct.h"

/* --- ONLY USED INTERNALLY --- */
#define CONTENT_UA  10
#define REP_UA      11
#define PUSH_UA     12
/* --- ONLY USED INTERNALLY --- */

typedef struct
{
	BYTE*	pbAccept;
	BYTE*	pbProxyCredentials;
	int		iProxyCredentialsLength;
	BYTE*	pbLanguage;
	UINT8	iLanguage_Length;
} UASETTINGS, *pUASETTINGS;

typedef struct
{
	int		iStackMode;
	int		iActiveBearer;
	BYTE*	pbUDP_CLIENT_IP;
	UINT8	pbUDP_CLIENT_IP_LEN;
	BYTE*	pbUDP_GW_IP;
	UINT8	pbUDP_GW_IP_LEN;
	BYTE*	pbSMS_CENTER;
	UINT8	pbSMS_CENTER_LEN;
	BYTE*	pbSMS_GW;
	UINT8	pbSMS_GW_LEN;
	BYTE*	pbUSSD_CENTER;
	UINT8	pbUSSD_CENTER_LEN;
	int		iUSSD_SC_TYPE;
	BYTE*	pbUSSD_SC;
	UINT8	pbUSSD_SC_LEN;
    UINT16	iTimeOut;				/*Added*/
} NETWORKSTRUCT, *pNETWORKSTRUCT;

typedef struct
{
	/* wae config variables */
	UINT16	iHistorySize;
/*	UINT16	iTimeOut;
	BOOL	iDisplayImages;
	BOOL	iDynamicImageUpdate;*/
/*	BYTE*   pbProxyUserName;
	BYTE*   pbProxyPassWord;
  BYTE*   pbLanguage;
  UINT8   iLanguage_Length;*/
/*	UINT8   iViewID;
	NETWORKSTRUCT* pNetwork;     */ 
    UINT32	iSDLPID_UA_ME;
	UINT32	iDefaultExpireTime;
	UINT32	liCacheSize;
	UINT8	iCacheMode;
	int		iOTAIsAllowed;
	int		iNrOfFreeRequestProc;
	int		iNrOfFreeSessionProc;
	BYTE*   pTID_Table;  /* Table of TIDs. */
    UINT8   lastTID;     /* Last TID allocated in the table. */
#ifdef CONFIG_SKT_SEC
	BYTE*	pbLastEncrypedURL;
#endif
} WAEMAINOBJECT,*pWAEMAINOBJECT;

typedef struct
{
	BYTE* 	pbURL;
	BYTE*	pbCredentials;
	int		iCredentialsLength;
	BYTE* 	pbReferer;
	BYTE* 	pbAccept;
	BYTE* 	pbBody;
	UINT32 	iBodyLength;
	UINT8 	iMethod;
	INT16	iContentCharSet;
} URLREQSTRUCT, *pURLREQSTRUCT;

typedef struct
{
	NETWORKSTRUCT* 	pNetwork;

	BYTE*   	pbProxyUserName;
	BYTE*   	pbProxyPassWord;
	UINT16		iHistorySize;
	BOOL		iDisplayImages;
	BOOL		iDynamicImageUpdate;
	BYTE*		pbLanguage;
	UINT8		iLanguage_Length;
} UAPARAM, *pUAPARAM;



URLREQSTRUCT* New_URLREQSTRUCT(void);
void Delete_URLREQSTRUCT(URLREQSTRUCT** ppTheObject);

NETWORKSTRUCT* New_NETWORKSTRUCT(void);
void Delete_NETWORKSTRUCT(NETWORKSTRUCT** ppTheObject);

UAPARAM* New_UAPARAM(void);
void Delete_UAPARAM(UAPARAM** ppTheObject);

UASETTINGS* New_UASETTINGS(void);
void Delete_UASETTINGS(UASETTINGS** ppTheObject);

void* SDL_CreateWAEManagementObject(void);
void SDL_DeleteWAEManagementObject(void** ppTheObject);
void* SDL_CopyNetworkParameters(void* pWaeObject);

#endif
