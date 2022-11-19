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


#ifndef WAE_CMMN_C
#define WAE_CMMN_C

#include "WAEDef.h"
#include "WAE_Cmmn.h"
#include "WMLDef.h"
#include "aapiclnt.h"
#include "aapimmi.h"
#include "config.h"
#include "URL.h"

/* ----------------------------------------------
 *               LISTHANDLING STRUCTS
 * ----------------------------------------------
 */

/* Each element in the list: */
typedef struct tLISTELEMENT
{
  int iIdentifier;                 /* Arbitrary identifier - optional */
  int iPrio;                       /* Priority info */
  void* pTheElement;               /* Generic data */
  struct tLISTELEMENT *pNext;      /* Doubly linked, more efficient. */
  struct tLISTELEMENT *pPrev;
} LISTELEMENT, *pLISTELEMENT;

/* The list header */
typedef struct
{
  int iMax;                        /* 0 = no upper limit */
  int iTotal;                      /* Number of elements currently in list */
  LISTELEMENT head;                /* A list element used as header node. */
} LISTHEADER, *pLISTHEADER;


typedef struct
{
   UINT8 iViewID;
   BOOL iActive;
} UAREQSTATUS,*pUAREQSTATUS;

/* ---------------------------------------------------------------------------------------------------------

                                            PID mgmt

 -------------------------------------------------------------------------------------------------------- */

/*
 * Convert a SDL PId to an integer.
 */
UINT32
PId2Int (SDL_PId pid)
{
#ifdef XSCT_CMICRO
  return (UINT32)pid;
#else
  return (UINT32)(pid.LocalPId);
#endif
}

/*
 * Convert an integer to a SDL PId.
 */
SDL_PId
Int2PId (UINT32 n)
{
#ifdef XSCT_CMICRO
  return (SDL_PId)n;
#else
  SDL_PId pid;

  pid.LocalPId = (xLocalPIdNode)n;
  pid.GlobalNodeNr = xGlobalNodeNumber ();

  return pid;
#endif
}

/* add a new pid to a list , NOTE: the pid must be converted first */
int UAAddNewPid( void* pList, int iId, unsigned long int uiPid , void* pTheUA)
{
	UAPIDSTRUCT* pPidStruct = NULL;
	if( pList != NULL ){
		pPidStruct = NEWSTRUCT(UAPIDSTRUCT);

		if( pPidStruct != NULL ){
			pPidStruct->uiPid = uiPid;
			pPidStruct->pData = pTheUA;
			return SDL_AddListElement( pList, iId, 0, (void*)pPidStruct );

		}
	}
	return 0;
	
} /* end of pidList_AddPid */

void* GetUAInList( void* pList, unsigned long int uiPid)
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	UAPIDSTRUCT*	pPidStruct = NULL;

	if( pList == NULL )
        return NULL;

	pvListElement = SDL_GetNextListElement( pList, NULL );
	while( pvListElement != NULL ){
		pvElementContent = SDL_GetListElementContent( pvListElement );
		if( pvElementContent != NULL ){
			pPidStruct = (UAPIDSTRUCT*)pvElementContent;
			if( pPidStruct && pPidStruct->uiPid == uiPid )
			{
				return pPidStruct->pData;
			}
		}
		pvListElement = SDL_GetNextListElement( pList, pvListElement );
	}

	return NULL;
} /*  */



unsigned long int UAGetPid( void* pList, int iId, int iRemove )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	unsigned long int	uiPID = 0;
	UAPIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = SDL_SearchListElement( pList, iId, NULL );
		if( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (UAPIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL ){
					if( iRemove ){
						uiPID = pPidStruct->uiPid;
						DEALLOC( &pPidStruct );
						SDL_DeleteListElement( pList, pvListElement );
						return uiPID;
					}
					else
						return pPidStruct->uiPid;
				}
			}
		}
	}
	return 0;
} /* end of pidList_GetPid */



int UARemovePid( void* pList, unsigned long int uiPid )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	UAPIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (UAPIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL && pPidStruct->uiPid == uiPid ){
					DEALLOC( &pPidStruct );
					SDL_DeleteListElement( pList, pvListElement );
					return 1;
				}
			}
			pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
} /* end of pidList_RemovePid */


/*--------------------------------------------------------------------------------------------------------

										Variable handling (setting etc)

--------------------------------------------------------------------------------------------------------*/

void SDL_DeleteParameters(void** pppInParameters)
{
   variableType** ppTempParameters; 
   ppTempParameters = (variableType**)*pppInParameters;
   if (ppTempParameters)
   {
	   while (*ppTempParameters)
	   {
		  DEALLOC(& (*ppTempParameters)->value );
		  DEALLOC(& (*ppTempParameters)->name );
		  DEALLOC( ppTempParameters ); 
		  ppTempParameters++;
	   }
	   DEALLOC(pppInParameters);
   }
}

void SDL_StoreParameters(void* pUser, void** pppInParameters)
{
   variableType** ppTempParameters; 

   ppTempParameters = (variableType**)*pppInParameters;
   if (ppTempParameters)
   {
	   while (*ppTempParameters)
	   {
          WML_SetVariable ((UA*)pUser, (*ppTempParameters)->name, (*ppTempParameters)->value );
		  DEALLOC(& (*ppTempParameters)->value );
		  DEALLOC(& (*ppTempParameters)->name );
		  DEALLOC( ppTempParameters ); 
		  ppTempParameters++;
	   }
	   DEALLOC(pppInParameters);
   }
}

void SDL_CallFunction( int iTID, void** pRequestStruct ) 
{
	URLREQSTRUCT*	pUrlReq = NULL;
	BYTE*	pbQuery = NULL;
	BYTE*	pbHost = NULL;
	BYTE*	pbPath = NULL;

	if( *pRequestStruct != NULL )
	{
		pUrlReq = (URLREQSTRUCT*)(*pRequestStruct);
		if( pUrlReq != NULL && pUrlReq->pbURL != NULL )
		{
            if (pUrlReq->pbURL && b_GetQuery(pUrlReq->pbURL, & pbQuery) && b_GetPath(pUrlReq->pbURL, & pbPath) && b_GetHost(pUrlReq->pbURL, & pbHost) )
			  CLNTa_callFunction( (UINT8)iTID, (CHAR*)pbHost, (CHAR*)pbPath+1, (CHAR*)pbQuery );
			else
              SDL_OutputError(NULL, ERR_WAE_UA_URL_INVALID, ERRTYPE_INFORMATION );
			  	 
			DEALLOC(&pbPath);
			DEALLOC(&pbQuery);
			DEALLOC(&pbHost);
		}
		Delete_URLREQSTRUCT( &pUrlReq );
	}
}


/* ---------------------------------------------------------------------------------------------------------

                                            Initialising of WAE

 -------------------------------------------------------------------------------------------------------- */

void SDL_InitializeGlobalVars(void* WAEManagementObject)
{
    WAEMAINOBJECT   *pWAEObject = (WAEMAINOBJECT*)WAEManagementObject;
    BOOL    fMoreConfigParams;
    UINT8   iGetViewId;
    UINT8   ConfigKind;
    ConfigSize  iSize;
    ConfigValue Value;

    fMoreConfigParams = GetFirstConfig(& iGetViewId, & ConfigKind, & Value, & iSize);

    while (fMoreConfigParams)
    {
        switch (ConfigKind)
        {
            case configQ_OTA:
                pWAEObject->iOTAIsAllowed = (int)(!Value.u);
                break;
            case configCACHE_AGE:
                pWAEObject->iDefaultExpireTime = Value.u;
                break;
            case configCACHE_MODE:
                pWAEObject->iCacheMode = (int)Value.u;
                break;
        }

        fMoreConfigParams = GetNextConfig(& iGetViewId, & ConfigKind, & Value, & iSize);
    }
}

void SDL_DeleteAllConfig(void)
{
  DeleteAllConfig();
}

NETWORKSTRUCT* InstantCreate(NETWORKSTRUCT* pInStruct)
{
  if (pInStruct)
      return pInStruct;
  pInStruct = New_NETWORKSTRUCT();
  return pInStruct;
}

BOOL    UA_ConfigStr( UAPARAM* pUAParam, int ConfigKind, ConfigValue ConfigCharValue, ConfigSize iLen)
{
    BOOL    bOk = TRUE;

    switch (ConfigKind)
    {
        /* network */
        case configSTACKMODE:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))         //white 00.11.20 because of compile warning.
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
                pUAParam->pNetwork->iStackMode = (int)ConfigCharValue.u;
            break;

        case configBEARER:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
                pUAParam->pNetwork->iActiveBearer = (int)ConfigCharValue.u;
            break;
        case configUSSD_SC_TYPE:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
                pUAParam->pNetwork->iUSSD_SC_TYPE = (int)ConfigCharValue.u;
            break;
        case configTIMEOUT:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
                pUAParam->pNetwork->iTimeOut = (UINT16)ConfigCharValue.u;
            break;

        case configUDP_IP_SRC:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
            {
                OSConnectorFree(pUAParam->pNetwork->pbUDP_CLIENT_IP);
                pUAParam->pNetwork->pbUDP_CLIENT_IP = (BYTE*)ConfigCharValue.p;
                pUAParam->pNetwork->pbUDP_CLIENT_IP_LEN = iLen;
            }
            break;
        case configUDP_IP_GW:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
            {
                OSConnectorFree(pUAParam->pNetwork->pbUDP_GW_IP);
                pUAParam->pNetwork->pbUDP_GW_IP = (BYTE*)ConfigCharValue.p;
                pUAParam->pNetwork->pbUDP_GW_IP_LEN = iLen;
            }
            break;
        case configSMS_C:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
            {
                OSConnectorFree(pUAParam->pNetwork->pbSMS_CENTER);
                pUAParam->pNetwork->pbSMS_CENTER = (BYTE*)ConfigCharValue.p;
                pUAParam->pNetwork->pbSMS_CENTER_LEN = iLen;
            }
            break;
        case configSMS_GW:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
            {
                OSConnectorFree(pUAParam->pNetwork->pbSMS_GW);
                pUAParam->pNetwork->pbSMS_GW = (BYTE*)ConfigCharValue.p;
                pUAParam->pNetwork->pbSMS_GW_LEN = iLen;
            }
            break;
        case configUSSD_C:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
            {
                OSConnectorFree(pUAParam->pNetwork->pbUSSD_CENTER);
                pUAParam->pNetwork->pbUSSD_CENTER = (BYTE*)ConfigCharValue.p;
                pUAParam->pNetwork->pbUSSD_CENTER_LEN = iLen;
            }
            break;
        case configUSSD_SC:
            //if (pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork))
            pUAParam->pNetwork=InstantCreate(pUAParam->pNetwork);
            if (pUAParam->pNetwork)
            {
                OSConnectorFree(pUAParam->pNetwork->pbUSSD_SC);
                pUAParam->pNetwork->pbUSSD_SC = (BYTE*)ConfigCharValue.p;
                pUAParam->pNetwork->pbUSSD_SC_LEN = iLen;
            }
            break;

        /* settings */
        case configAUTH_PASS_GW:
            DEALLOC(&pUAParam->pbProxyPassWord);
            if (ConfigCharValue.p)
            {
                //if (pUAParam->pbProxyPassWord = NEWARRAY(BYTE,iLen+1))
                pUAParam->pbProxyPassWord = NEWARRAY(BYTE,iLen+1);
                if (pUAParam->pbProxyPassWord)
                {
                    memcpy(pUAParam->pbProxyPassWord,ConfigCharValue.p,iLen);
                    pUAParam->pbProxyPassWord[iLen]=0;
                }
                OSConnectorFree(ConfigCharValue.p);
            }
            break;
        case configAUTH_ID_GW:
            DEALLOC(&pUAParam->pbProxyUserName);
            if (ConfigCharValue.p)
            {
                //if (pUAParam->pbProxyUserName = NEWARRAY(BYTE,iLen+1))
                pUAParam->pbProxyUserName = NEWARRAY(BYTE,iLen+1);
                if (pUAParam->pbProxyUserName)
                {
                    memcpy(pUAParam->pbProxyUserName,ConfigCharValue.p,iLen);
                    pUAParam->pbProxyUserName[iLen]=0;
                }
                OSConnectorFree(ConfigCharValue.p);
            }
            break;
        case configWSP_Language:
            if (iLen)
            {
                DEALLOC(&pUAParam->pbLanguage);
                if (ConfigCharValue.p)
                {
                    //if (pUAParam->pbLanguage = NEWARRAY(BYTE,iLen))
                    pUAParam->pbLanguage = NEWARRAY(BYTE,iLen);
                    if (pUAParam->pbLanguage)
                    {
                        memcpy(pUAParam->pbLanguage,ConfigCharValue.p,iLen);
                        pUAParam->iLanguage_Length = iLen;
                    }
                    OSConnectorFree(ConfigCharValue.p);
                }
            }
            break;

        /* other  */
        case configDISPLAY_IMAGES:
            pUAParam->iDisplayImages = (BOOL)ConfigCharValue.u;
            break;
        case configUPDATE_IMAGES:
            pUAParam->iDynamicImageUpdate = (BOOL)ConfigCharValue.u;
            break;
        case configHISTORY_SIZE:
            pUAParam->iHistorySize = (UINT16)ConfigCharValue.u;
            break;

        /* false */
        default:
            /* DEALLOC(&ConfigCharValue); */
            /*  GBU,000412, This may lead to memory leakage! */
            bOk = FALSE;
            break;
    }

    return bOk;
}


/* initializes a specific agent - returns TRUE if any int was done */
BOOL SDL_InitializeUAVars(int ViewID, void* pInUserAgent, void* pUAParameters)
{
    BOOL    fRetval = FALSE;
    UINT8   iGetViewId;
    UINT8   ConfigKind;
    ConfigSize  iSize;
    ConfigValue Value;
    BOOL    fMoreConfigParams;
    UA  *pUserAgent = (UA*)pInUserAgent;

    if ( (pUserAgent == NULL && pUAParameters == NULL) || ViewID < 0)
        return FALSE;

    fMoreConfigParams = GetFirstConfig(& iGetViewId, & ConfigKind, & Value, & iSize);

    while (fMoreConfigParams)
    {
        if (iGetViewId==0)  /* check also for viewid 0 i.e. global config */
        {
            if (iSize && Value.p)   /* stringconfig */
            {
                ConfigValue cnfgValue;
                cnfgValue.p = NEWARRAY(BYTE,iSize);
                if (cnfgValue.p)
                    B_COPYSTRINGN(cnfgValue.p, Value.p, iSize);

                if (pUserAgent)
                {
                    if (UA_ConfigStr(pUserAgent->pUAParam, ConfigKind, cnfgValue, iSize))
                        fRetval=TRUE;
                }
                else
                {
                    if (UA_ConfigStr((UAPARAM*)pUAParameters, ConfigKind, cnfgValue, iSize))
                        fRetval=TRUE;
                }
            }
            else /* int config */
            {
                if (pUserAgent)
                {
                    if (UA_ConfigStr(pUserAgent->pUAParam, ConfigKind, Value, iSize))
                        fRetval=TRUE;
                }
                else
                {
                    if (UA_ConfigStr((UAPARAM*)pUAParameters, ConfigKind, Value, iSize))
                        fRetval=TRUE;
                }
            }
        }
        else if (iGetViewId==ViewID)       /* config for this ua */
        {
            if (pUserAgent)
            {
                if (UA_ConfigStr(pUserAgent->pUAParam, ConfigKind, Value, iSize))
                {
                    fRetval=TRUE;
                    DeleteCurrentConfig();
                }
            }
            else
            {
                if (UA_ConfigStr((UAPARAM*)pUAParameters, ConfigKind, Value, iSize))
                {
                    fRetval=TRUE;
                    DeleteCurrentConfig();
                }
            }
        }

        fMoreConfigParams = GetNextConfig(& iGetViewId, & ConfigKind, & Value, & iSize);
    }

    return fRetval;
}

void* SDL_CreateNewUASettings(void* pUserAgent, void* pUAParameters)
{
  UASETTINGS* pUASettings=NULL;
  UAPARAM* pUAParam =NULL;

  if (pUserAgent==NULL && pUAParameters==NULL)
	  return NULL;

  if (pUAParameters)
    pUAParam = (UAPARAM*)pUAParameters;
  else
    pUAParam = ((UA*)pUserAgent)->pUAParam;

  pUASettings = New_UASETTINGS();

  if (pUASettings)
  {
	  if (pUserAgent)
	     pUASettings->pbAccept=(BYTE*)GenCopyString(1, ACCEPT_DEFAULT);  
 	  else
	     pUASettings->pbAccept=(BYTE*)GenCopyString(1, ACCEPT_ALL);    
	  
	  pUASettings->iProxyCredentialsLength = CopyCredentialParameters (pUAParam->pbProxyUserName, pUAParam->pbProxyPassWord, (void**)& pUASettings->pbProxyCredentials);
	  if (pUAParam->iLanguage_Length)
	  {
		pUASettings->pbLanguage = NEWARRAY(BYTE, pUAParam->iLanguage_Length);
	    if (pUASettings->pbLanguage)
		{
            B_COPYSTRINGN(pUASettings->pbLanguage, pUAParam->pbLanguage,pUAParam->iLanguage_Length);
		   pUASettings->iLanguage_Length = pUAParam->iLanguage_Length;
		}
	  }
  }
  
  return pUASettings;
}


void* SDL_CreateNewNetworkParameters(void* pUserAgent)
{
   void* pNetwork=NULL;
   if (pUserAgent==NULL)
	  return NULL;
   pNetwork = ((UA*)pUserAgent)->pUAParam->pNetwork;
   ((UA*)pUserAgent)->pUAParam->pNetwork=NULL;
   return pNetwork;
}


/*---------------------------------------------------------------------------------
When using proxy/server authenication - copy username and password into one
string ("XXX\0XXX\0")
------------------------------------------------------------------------------------*/

int CopyCredentialParameters (void* pUserName, void* pPassWord, void** ppCredentials)
{
  int iLength = 0;
  int iNameLength = 0;
  int iPassLength = 0;
  BYTE *p;

  DEALLOC (ppCredentials);

  if (pUserName==NULL && pPassWord==NULL)
	  return 0;

  iNameLength = pUserName ? B_STRINGLENGTH ((CHAR*)pUserName) : 0;
  iPassLength = pPassWord ? B_STRINGLENGTH ((CHAR*)pPassWord) : 0;
  iLength = iNameLength + iPassLength + 2;

  *ppCredentials = NEWARRAY (BYTE, iLength);
  if (*ppCredentials == NULL) {
    return 0;
  }
  p = (BYTE *)*ppCredentials;

  if (pUserName != NULL) {
    B_COPYSTRING ((char *)p, (const char *)pUserName);
    #ifdef WAE_DEBUG
      PrintChar ("Username:");
      PrintChar (p);
    #endif
    p += iNameLength;
  }
  *p++ = '\0';  /*if username is null */

  if (pPassWord != NULL) {
    B_COPYSTRING ((char *)p, (const char *)pPassWord);
    #ifdef WAE_DEBUG
      PrintChar ("Password:");
      PrintChar (p);
    #endif
    p += iPassLength;
  }
  *p = '\0';  /*if password is null */

  return iLength;
}

/*--------------------------------------------------------------------------------------------------------------------------------

                                                    TID MANAGEMENT

--------------------------------------------------------------------------------------------------------------------------------*/

#define EMPTY     0x0
#define INACTIVE  0x1
#define ACTIVE    0x2

#define MAX_TID   250

/* ---------------------------------------------
 * Returns a new TID from the TID pool. Return
 * value 0 indicates an error and is NOT a valid TID.
 * ----------------------------------------------*/
int SDL_CreateNewGlobalTID(void* pWAEInObj, int iViewID)
{
  int i;
  BYTE *tbl;
  WAEMAINOBJECT *pObj = (WAEMAINOBJECT *)pWAEInObj;

  iViewID = iViewID;  /* To get rid of a warning. */

  if (pObj == NULL)
    return 0;

  tbl = pObj->pTID_Table;

  if (tbl == NULL)
    return 0;

  /* To maximize the time between recycling a TID, we start searching
   * for an empty slot at the point where we left off last time. */
  for (i = (pObj->lastTID == MAX_TID) ? 1 : pObj->lastTID + 1;
       i != pObj->lastTID; (i == MAX_TID) ? i = 1 : i++) {
    int idx = (i >> 2);
    int n = (i & 0x3) << 1;

    if (((tbl[idx] >> n) & 0x3) == EMPTY) {
      tbl[idx] |= (ACTIVE << n);
      pObj->lastTID = (UINT8)i;
      return i;
    }
  }

  return 0;
}


/* ---------------------------------------------
 * Return the next active TID strictly larger
 * than iPrevTID. If iPrevTID is 0, the smallest
 * active TID is returned.
 * If there is no active TID larger than iPrevTID,
 * this routine returns 0.
 * ----------------------------------------------*/
int SDL_GetNextActiveTID (void* pWAEInObj, int iPrevTID)
{
  int i;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return 0;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;

  if (tbl == NULL)
    return 0;
  
  for (i = (iPrevTID == 0 ? 1 : iPrevTID + 1); i <= MAX_TID; i++) {
    int idx = (i >> 2);
    int n = (i & 0x3) << 1;

    if (((tbl[idx] >> n) & 0x3) == ACTIVE) {
      return i;
    }
  }

  return 0;
}

/* ---------------------------------------------
 * Deactivate a TID in the TID pool - i.e., the 
 * corresponding request is no longer active.
 * ----------------------------------------------*/
void SDL_DeactivateGlobalTID(void* pWAEInObj, int iTID)
{
  int idx = (iTID >> 2);
  int n = (iTID & 0x3) << 1;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;

  if (tbl == NULL)
    return;

  tbl[idx] &= ~(0x3 << n);
  tbl[idx] |= (INACTIVE << n);
} 

/* ---------------------------------------------
 * Check a TID in the TID pool - is the 
 * corresponding request active. Returns 0 (FALSE) if not active.
 * ----------------------------------------------*/
int SDL_CheckGlobalTIDStatus(void* pWAEInObj, int iTID)
{
  int idx = (iTID >> 2);
  int n = (iTID & 0x3) << 1;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return 0;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;

  if (tbl == NULL)
    return 0;

  return ((tbl[idx] >> n) & 0x3) == ACTIVE;
}

/* ---------------------------------------------
 * Releases a TID from the TID pool.
 * ----------------------------------------------*/
void SDL_ReleaseGlobalTID(void* pWAEInObj, int iTID)
{
  int idx = (iTID >> 2);
  int n = (iTID & 0x3) << 1;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;

  if (tbl == NULL)
    return;

  tbl[idx] &= ~(0x3 << n);
  tbl[idx] |= (EMPTY << n);
}

/* ---------------------------------------------
 * Deactivate ALL TIDs in the TID pool - i.e., the 
 * corresponding requests are no longer active.
 * ----------------------------------------------*/
void SDL_DeactivateAllGlobalTIDs(void* pWAEInObj)
{
  int i;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;
  
  if (tbl == NULL)
    return;

  for (i = 1; i <= MAX_TID; i++) {
    int idx = (i >> 2);
    int n = (i & 0x3) << 1;

    if (((tbl[idx] >> n) & 0x3) == ACTIVE) {
      tbl[idx] &= ~(0x3 << n);
      tbl[idx] |= (INACTIVE << n);
    }
  }
}

/* -----------------------------------------------------------------------------------------------
 *
 *                                            LISTHANDLING
 *
 * -----------------------------------------------------------------------------------------------
 */

/* --------------------------------------------------
 * Create a new list that will hold a maximum of
 * iMax elements. If iMax = 0, no upper limit is
 * enforced.
 * -------------------------------------------------- */
void* SDL_NewList (int iMax)
{
   LISTHEADER* pTheList;

   if ((pTheList = NEWSTRUCT (LISTHEADER)) == NULL)
     return NULL;

   pTheList->iMax = iMax;
   pTheList->iTotal = 0;
   pTheList->head.pNext = pTheList->head.pPrev = &(pTheList->head);

   return pTheList;
}


/* --------------------------------------------------
 * Remove all elements in the list, but NOT the data
 * that each element points to; it must be deallocated
 * separately. Total number of elements is set to 0.
 * The listheader is saved intact, and will represent
 * an empty list.
 * -------------------------------------------------- */
void SDL_ClearList (void* pList)
{
  LISTELEMENT* pHead;

  if (pList == NULL)
    return;

  pHead = &(((LISTHEADER *)pList)->head);
  while (pHead->pNext != pHead) {
    SDL_DeleteListElement (pList, pHead->pNext);
  }
}

/* --------------------------------------------------
 * Delete all listelements and then delete the list
 * header; this does NOT delete the data that each
 * element points to, it must be deallocated separately.
 * To be used when a COMPLETE removal of the list is wanted.
 * -------------------------------------------------- */
void SDL_DeleteList (void** ppList)
{
  /* Pass through all elements and deallocate them. */
  SDL_ClearList (*ppList);
  /* Remove the list header. */
  DEALLOC ((LISTHEADER**)ppList);
}

/* --------------------------------------------------
 * Add a new element to the list.
 * Puts pElement in the list and returns 1 (TRUE) if OK.
 * If iPrio != 0 then the list is a priority list, where
 * a iPrio controls the point of insertion; otherwise,
 * the element is inserted at the end of the list.
 * -------------------------------------------------- */
int
SDL_AddListElement (void* pList, int iId, int iPrio ,void* pElement)
{
  LISTHEADER* pTheList = (LISTHEADER*)pList;
  LISTELEMENT *pHead;
  LISTELEMENT *pBefore, *pNew;

  if ((pTheList == NULL) ||
      (((pTheList->iMax) != 0) && (pTheList->iTotal >= pTheList->iMax)))
    return 0;

  pHead = &(pTheList->head);
  if (iPrio != 0) {
    for (pBefore = pHead->pNext; pBefore != pHead; pBefore = pBefore->pNext) {
      if (pBefore->iPrio < iPrio)
        break;
    }
  }
  else
    pBefore = pHead;

  if ((pNew = NEWSTRUCT (LISTELEMENT)) == NULL)
    return 0;

  pTheList->iTotal++;
  pNew->iIdentifier = iId;
  pNew->pTheElement = pElement;
  pNew->iPrio = iPrio;

  pNew->pNext = pBefore;
  pNew->pPrev = pBefore->pPrev;
  pNew->pNext->pPrev = pNew;
  pNew->pPrev->pNext = pNew;

  return 1;
}


/* --------------------------------------------------
 * Add a new element last in the list.
 * Returns 1 (TRUE) if OK, 0 otherwise.
 * -------------------------------------------------- */
int
SDL_AddElementLast (void* pList, int iId, void* pElement)
{
  if (pList == NULL)
    return 0;

  return SDL_AddElementAfter (pList, iId,  pElement,
                              ((LISTHEADER *)pList)->head.pPrev);
}

/* --------------------------------------------------
 * Add a new element first in the list.
 * Returns 1 (TRUE) if OK, 0 otherwise.
 * -------------------------------------------------- */
int
SDL_AddElementFirst (void* pList, int iId, void* pElement)
{
  if (pList == NULL)
    return 0;

  return SDL_AddElementAfter (pList, iId,  pElement,
                              &(((LISTHEADER *)pList)->head));
}

/*---------------------------------------------------
 * Add a new element after the one specified by pAfter.
 * If pAfter is NULL then add the new element first in the list.
 * Returns 1 (TRUE) if OK, 0 otherwise.
 * -------------------------------------------------- */
int
SDL_AddElementAfter (void* pList, int iId, void* pElement, void* pAfter)
{
  LISTELEMENT   *pNew = NULL;
  LISTHEADER    *pTheList = (LISTHEADER *)pList;

  if ((pTheList == NULL) ||
      (((pTheList->iMax) != 0) && (pTheList->iTotal >= pTheList->iMax)))
    return 0;

  if ((pNew = NEWSTRUCT (LISTELEMENT)) == NULL)
    return 0;

  pTheList->iTotal++;
  pNew->iIdentifier = iId;
  pNew->pTheElement = pElement;
  pNew->iPrio = 0;

  if (pAfter == NULL)
    pAfter = &(pTheList->head);

  pNew->pNext = ((LISTELEMENT*)pAfter)->pNext;
  pNew->pPrev = (LISTELEMENT*)pAfter;
  ((LISTELEMENT*)pAfter)->pNext = pNew;
  pNew->pNext->pPrev = pNew;

  return 1;
}

/* --------------------------------------------------
 * Get the element with id iID. Start searching from the
 * pSearchFrom listelement; if pSearchFrom is NULL then 
 * the search is performed from the beginning.  
 * If a matching element is found this is returned,
 * otherwise NULL.
 * -------------------------------------------------- */
void *
SDL_SearchListElement (void* pList, int iId, void* pSearchFrom)
{
  LISTELEMENT *pReturnElement = NULL;
  LISTELEMENT *pHead;

  if (pList == NULL)
    return NULL;

  pHead = &(((LISTHEADER *)pList)->head);

  if (pSearchFrom == NULL)
    pSearchFrom = pHead->pNext;

  for (pReturnElement = (LISTELEMENT*)pSearchFrom; pReturnElement != pHead;
       pReturnElement = pReturnElement->pNext) {
    if (pReturnElement->iIdentifier == iId)
      break;
  }

  return (pReturnElement == pHead) ? NULL : pReturnElement;
}


/*-------------------------------------------------
 * Get the next listelement. If pTheReqElm is NULL,
 * then the first element is returned.
 * -------------------------------------------------- */
void *
SDL_GetNextListElement (void* pList, void* pTheReqElm)
{
  LISTELEMENT *pHead;
  LISTELEMENT *pElement = (LISTELEMENT *)pTheReqElm;

  if (pList == NULL)
    return NULL;

  pHead = &(((LISTHEADER *)pList)->head);

  if (pElement == NULL)
    pElement = pHead;

  return (pElement->pNext == pHead) ? NULL : pElement->pNext;
}

/* -------------------------------------------------
 * Gets the content of a listelement, i.e., the generic
 * data pointed to.
 * --------------------------------------------------*/
void *
SDL_GetListElementContent (void* pTheReqElm)
{
  if (pTheReqElm == NULL)
    return NULL;

  return ((LISTELEMENT*)pTheReqElm)->pTheElement;
}

/* --------------------------------------------------
 * Change the content of a LISTElement, i.e., the generic
 * data pointed to.
 * -------------------------------------------------- */
void
SDL_ChangeListElementContent (void* pTheReqElm, void* pNewContentElement)
{
  if (pTheReqElm != NULL)
    ((LISTELEMENT*)pTheReqElm)->pTheElement = pNewContentElement;
}

/* --------------------------------------------------
 * Get the id of a listelement.
 * -------------------------------------------------- */
int
SDL_GetListElementID (void* pTheReqElm)
{
  if (pTheReqElm == NULL)
    return 0;

  return ((LISTELEMENT*)pTheReqElm)->iIdentifier;
}

/* --------------------------------------------------
 * Change the id of a LISTElement.
 * -------------------------------------------------- */
void
SDL_ChangeListElementID (void* pTheReqElm, int iId )
{
  if (pTheReqElm != NULL)
    ((LISTELEMENT*)pTheReqElm)->iIdentifier = iId;
}

/* -------------------------------------------------
 * Delete the listelement pRElement. Does NOT delete
 * the generic data pointed to by the element.
 * --------------------------------------------------*/
void
SDL_DeleteListElement (void* pList, void* pRElement)
{
  LISTHEADER    *pTheList = (LISTHEADER *)pList;
  LISTELEMENT   *pRemoveElement = (LISTELEMENT *)pRElement;

  if ((pTheList == NULL) || (pRElement == NULL) || (pTheList->iTotal <= 0))
    return;

  pTheList->iTotal--;
  pRemoveElement->pPrev->pNext = pRemoveElement->pNext;
  pRemoveElement->pNext->pPrev = pRemoveElement->pPrev;
  DEALLOC (&pRemoveElement);
}


/* --------------------------------------------------
 * Get the number of elements in a list.
 * -------------------------------------------------- */
int
SDL_GetCardinal (void* pList)
{
  if (pList == NULL)
    return 0;

  return ((LISTHEADER*)pList)->iTotal;
}

/*---------------------------------------------------------------------------------------------------------
                
				                         Stackfunctions

-----------------------------------------------------------------------------------------------------------*/

void* SDL_NewLIFOStack(void)
{
  return SDL_NewList(0);
}

void SDL_ClearLIFOStack( void** ppStack )
{
   SDL_DeleteList(ppStack);
}

void* SDL_POPLIFOStack(void* pStack)
{
   void* pListElm=NULL;
   void* pReturn;
   /* get the first element */
   pListElm = SDL_GetNextListElement(pStack, NULL);
   pReturn = SDL_GetListElementContent(pListElm);
   SDL_DeleteListElement(pStack, pListElm);
   return pReturn;
}

int SDL_PUSHLIFOStack( void* pStack, void* pElement )
{
   return SDL_AddElementFirst(pStack, 0, pElement);
}

void* SDL_CheckLIFOStack( void *pStack )
{
   void* pListElm=NULL;
   pListElm = SDL_GetNextListElement(pStack, NULL);
   return SDL_GetListElementContent(pListElm);
}


/*------------------------------------------------------------
                 General
--------------------------------------------------------------*/


/*---------------------------------------------
Enables SDL to dynamically free structures in C
e.g. strings
Input: Datapointer
Output: -
----------------------------------------------*/
void SDL_Free(void** pTheData)
{
    DEALLOC(pTheData);
}

/*---------------------------------------------
Enables SDL to output errormessages to the application
Input: errNo , defined in the porting guide.
Output:
----------------------------------------------*/

void SDL_OutputError(void* pUser, int iErrorcode, int iErrType)
{
   if (pUser==NULL)
     CLNTa_error(0, (INT16)iErrorcode, (UINT8)iErrType);
   else
     CLNTa_error(((UA*)pUser)->iViewID, (INT16)iErrorcode, (UINT8)iErrType);
}

void SDL_OutputStatus(void* pUser, int iStatus, void* pURL )
{
   MMIa_status( ((UA*)pUser)->iViewID, (UINT8)iStatus, (CHAR*)pURL );
}

void SDL_LOG(void* pUser, int logno, void* pString)
{
    /*  To get rid of warnings for unused variables */
    pUser = pUser;
    logno = logno;
    pString = pString;

 #ifdef LOG_INTERNAL 
   if (pUser==NULL)
     CLNTa_log( 0, (INT16)logno, pString );
   else
     CLNTa_log( ((UA*)pUser)->iViewID, (INT16)logno, pString );
 #endif
}


void* GenCopyString(INT8 iSize, const void* pStr)
{
   void* pRes=NULL;
   if (pStr !=NULL)
   {
     if (iSize == 1)
     {
       pRes = NEWARRAY(BYTE,B_STRINGLENGTH((CHAR*)pStr)+1);
       if (pRes)
         B_COPYSTRING((char *)pRes,(const char *)pStr);
     }
     else
     {
       pRes = NEWARRAY(WCHAR,STRINGLENGTH((WCHAR*)pStr)+1);
       if (pRes)
         COPYSTRING((WCHAR*)pRes,(WCHAR*)pStr);
     }
   }
   return pRes;
}

#endif
