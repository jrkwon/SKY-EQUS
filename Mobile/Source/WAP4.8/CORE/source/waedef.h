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

/* --- ONLY USED INTERNALLY --- */
#define CONTENT_UA  100
#define REP_UA      101
#define PUSH_UA     102
/* --- ONLY USED INTERNALLY --- */

/* Cache directives */
#define NoCache 0x80
#define NoStore 0x81
#define Private 0x87

/* Request options*/
#define RequestOption_Reload				1
#define RequestOption_WmlBackNav			2
#define RequestOption_AllowSegmentedResult	4
#define RequestOption_MoreData			 	8

/*
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
    UINT16	iTimeOut;
} NETWORKSTRUCT, *pNETWORKSTRUCT;

*/

typedef struct 
{
	/* wae config variables */
	UINT32	iDefaultExpireTime;
	UINT32	liCacheSize;
	UINT8	iCacheMode;
	int		iNrOfFreeRequestProc;
	int		iNrOfFreeSessionProc;
	BYTE*	pbUSERAGENT;
	BYTE*	pbPROFILE;
	BYTE*	pbPROFILE_DIFF;
	UINT16	iPROFILE_DIFF_LEN;
	BYTE*   pTID_Table;			/* Table of TIDs. */
    UINT8   lastTID;			/* Last TID allocated in the table. */
	INT16* piObjectID;          /* Table of ObjectId:s */
	UINT8*  piChannelID;         /* Table of ChannelId:s, the channelId at position x is 
								   connected to the objectId at position x in the objectId table */
	UINT8   iMaxNoObjects;        
#ifdef CONFIG_PUSH
	UINT8	iSIInterrupt;
	UINT8	iSLInterrupt;
	UINT8	iSecLevel;
#endif
#ifdef CONFIG_SKT_SEC
	BYTE*	pbLastEncrypedURL;
#endif

} WAEMAINOBJECT,*pWAEMAINOBJECT;

typedef struct
{
	BYTE* 	pbURL;
	BYTE* 	pbReferer;
	BYTE* 	pbAccept;
	BYTE* 	pbBody;
	UINT32 	iBodyLength;
	UINT8 	iMethod;
	UINT8	iContentType;
	BYTE*	pbContentType;
	INT16	iContentCharSet;
	UINT32	iLastModified;
	BYTE*	pbEtag;
	UINT32	iTotalPostSize;

} URLREQSTRUCT, *pURLREQSTRUCT;

/*

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

*/

URLREQSTRUCT* New_URLREQSTRUCT(void);
void Delete_URLREQSTRUCT(URLREQSTRUCT** ppTheObject);

void* SDL_CreateWAEManagementObject(UINT8 iSize);
void SDL_DeleteWAEManagementObject(void** ppTheObject);

#endif
