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
#ifndef WAEDEF_C
#define WAEDEF_C
#include "WAEDef.h"
#include "WAE_Cmmn.h"


/*----------------------*/

UAPARAM* New_UAPARAM(void)
{
   UAPARAM*  pUAParam;
   pUAParam = NEWSTRUCT(UAPARAM);

   if (pUAParam)
   {
	  pUAParam->iHistorySize = cfg_wae_ua_defaultHistorySize;
	  pUAParam->iDisplayImages = cfg_wae_ua_defaultDisplayImages;
	  pUAParam->iDynamicImageUpdate = cfg_wae_ua_defaultUpdateImages;
	  pUAParam->pbProxyUserName = NULL;
	  pUAParam->pbProxyPassWord = NULL;
	  pUAParam->pbLanguage = NULL;
	  pUAParam->iLanguage_Length = 0;
  	  pUAParam->pNetwork = NULL;
   }
   return pUAParam;
}

void Delete_UAPARAM(UAPARAM** ppTheObject)
{
  if (*ppTheObject !=NULL)
  {
    DEALLOC(&((*ppTheObject)->pbProxyUserName));
    DEALLOC(&((*ppTheObject)->pbProxyPassWord));
    DEALLOC(&((*ppTheObject)->pbLanguage));
    DEALLOC(&((*ppTheObject)->pNetwork));
    DEALLOC(ppTheObject);
  }
}

/*----------------*/


UASETTINGS* New_UASETTINGS(void)
{
   UASETTINGS*  pUASettings;
   pUASettings = NEWSTRUCT(UASETTINGS);

   if (pUASettings)
   {
 	  pUASettings->pbAccept=NULL;                             
      pUASettings->pbProxyCredentials=NULL;
	  pUASettings->iProxyCredentialsLength=0;   
	  pUASettings->pbLanguage=NULL;
	  pUASettings->iLanguage_Length=0;
   }
   return pUASettings;
}

void Delete_UASETTINGS(UASETTINGS** ppTheObject)
{
  if (*ppTheObject !=NULL)
  {
    DEALLOC(&((*ppTheObject)->pbAccept));
    DEALLOC(&((*ppTheObject)->pbProxyCredentials));
    DEALLOC(&((*ppTheObject)->pbLanguage));
    DEALLOC(ppTheObject);
  }
}

/*---- "constructor" and "destructor" for  URLREQSTRUCT ------*/


URLREQSTRUCT* New_URLREQSTRUCT(void)
{
   URLREQSTRUCT*  pURLStruct;
   pURLStruct = NEWSTRUCT(URLREQSTRUCT);

   if (pURLStruct)
   {
 	  pURLStruct->pbURL=NULL;                             
      pURLStruct->pbCredentials=NULL;
	  pURLStruct->iCredentialsLength=0;   
	  pURLStruct->pbReferer=NULL;     
	  pURLStruct->pbAccept=NULL;                          
	  pURLStruct->pbBody=NULL;                   
      pURLStruct->iBodyLength=0;
	  pURLStruct->iMethod=0;
	  pURLStruct->iContentCharSet = IANA_CHARSET_INVALID;
   }
   return pURLStruct;
}

void Delete_URLREQSTRUCT(URLREQSTRUCT** ppTheObject)
{
  if (*ppTheObject !=NULL)
  {
    DEALLOC(&((*ppTheObject)->pbURL));
    DEALLOC(&((*ppTheObject)->pbCredentials));
    DEALLOC(&((*ppTheObject)->pbReferer));
	DEALLOC(&((*ppTheObject)->pbAccept));
    DEALLOC(&((*ppTheObject)->pbBody));
    DEALLOC(ppTheObject);
  }
}


/*---- "constructor" and "destructor" for  NETWORKSTRUCT ------*/

NETWORKSTRUCT* New_NETWORKSTRUCT(void)
{
    NETWORKSTRUCT*  pNetwStruct;
    pNetwStruct = NEWSTRUCT(NETWORKSTRUCT);
	if (pNetwStruct)
    {
	  pNetwStruct->iStackMode=0;
	  pNetwStruct->iActiveBearer=-1;
	  pNetwStruct->pbUDP_CLIENT_IP=NULL;
	  pNetwStruct->pbUDP_CLIENT_IP_LEN=0;

	  pNetwStruct->pbUDP_GW_IP=NULL;
	  pNetwStruct->pbUDP_GW_IP_LEN=0;
	  
	  pNetwStruct->pbSMS_CENTER=NULL;
	  pNetwStruct->pbSMS_CENTER_LEN=0;
	  
	  pNetwStruct->pbSMS_GW=NULL;
	  pNetwStruct->pbSMS_GW_LEN=0;
	  
	  pNetwStruct->pbUSSD_CENTER=NULL;
	  pNetwStruct->pbUSSD_CENTER_LEN=0;
	  
	  pNetwStruct->iUSSD_SC_TYPE=-1;
	  pNetwStruct->pbUSSD_SC=NULL;
	  pNetwStruct->pbUSSD_SC_LEN=0;
	  pNetwStruct->iTimeOut=TIMEOUT_VALUE_INVALID;
    }
	return pNetwStruct;
}

void Delete_NETWORKSTRUCT(NETWORKSTRUCT** ppTheObject)
{
	DEALLOC(& ((*ppTheObject)->pbUDP_CLIENT_IP ));
	DEALLOC(&((*ppTheObject)->pbUDP_GW_IP));
	DEALLOC(&((*ppTheObject)->pbSMS_CENTER));
	DEALLOC(&((*ppTheObject)->pbSMS_GW));
	DEALLOC(&((*ppTheObject)->pbUSSD_CENTER));
	DEALLOC(&((*ppTheObject)->pbUSSD_SC));
    DEALLOC(ppTheObject);
}

/* void* SDL_CopyNetworkParameters(void* pWaeObject)
{
  NETWORKSTRUCT*  pNetwStruct=NULL;
  NETWORKSTRUCT*  pOriginalStruct=NULL;
  pNetwStruct = New_NETWORKSTRUCT();
  
  if (pWaeObject)
     pOriginalStruct = ((WAEMAINOBJECT*)pWaeObject)->pNetwork;

  if ((pOriginalStruct) && (pNetwStruct))
  {
	  pNetwStruct->iActiveBearer = pOriginalStruct->iActiveBearer;

	  if (pOriginalStruct->pbUDP_CLIENT_IP)
	  {
        pNetwStruct->pbUDP_CLIENT_IP=NEWARRAY(BYTE,pOriginalStruct->pbUDP_CLIENT_IP_LEN);
		if (pNetwStruct->pbUDP_CLIENT_IP)
        {
			B_COPYSTRINGN(pNetwStruct->pbUDP_CLIENT_IP, pOriginalStruct->pbUDP_CLIENT_IP, pOriginalStruct->pbUDP_CLIENT_IP_LEN);
			pNetwStruct->pbUDP_CLIENT_IP_LEN=pOriginalStruct->pbUDP_CLIENT_IP_LEN;
		}
      }


	  if (pOriginalStruct->pbUDP_GW_IP)
	  {
        pNetwStruct->pbUDP_GW_IP=NEWARRAY(BYTE,pOriginalStruct->pbUDP_GW_IP_LEN); 
		if (pNetwStruct->pbUDP_GW_IP)
        { 
			B_COPYSTRINGN(pNetwStruct->pbUDP_GW_IP, pOriginalStruct->pbUDP_GW_IP, pOriginalStruct->pbUDP_GW_IP_LEN);
			pNetwStruct->pbUDP_GW_IP_LEN=pOriginalStruct->pbUDP_GW_IP_LEN;
		}
      }


	  if (pOriginalStruct->pbSMS_CENTER)
	  {
        pNetwStruct->pbSMS_CENTER=NEWARRAY(BYTE,pOriginalStruct->pbSMS_CENTER_LEN); 
		if (pNetwStruct->pbSMS_CENTER)
        {
			B_COPYSTRINGN(pNetwStruct->pbSMS_CENTER, pOriginalStruct->pbSMS_CENTER, pOriginalStruct->pbSMS_CENTER_LEN);
			pNetwStruct->pbSMS_CENTER_LEN=pOriginalStruct->pbSMS_CENTER_LEN;
		}
      }


	  if (pOriginalStruct->pbSMS_GW)
	  {
        pNetwStruct->pbSMS_GW=NEWARRAY(BYTE,pOriginalStruct->pbSMS_GW_LEN); 
		if (pNetwStruct->pbSMS_GW)
        {
			B_COPYSTRINGN(pNetwStruct->pbSMS_GW, pOriginalStruct->pbSMS_GW, pOriginalStruct->pbSMS_GW_LEN);
			pNetwStruct->pbSMS_GW_LEN=pOriginalStruct->pbSMS_GW_LEN;
		}
      }

	  
	  if (pOriginalStruct->pbUSSD_CENTER)
	  {
        pNetwStruct->pbUSSD_CENTER=NEWARRAY(BYTE,pOriginalStruct->pbUSSD_CENTER_LEN); 
		if (pNetwStruct->pbUSSD_CENTER)
        {
			B_COPYSTRINGN(pNetwStruct->pbUSSD_CENTER, pOriginalStruct->pbUSSD_CENTER, pOriginalStruct->pbUSSD_CENTER_LEN);
			pNetwStruct->pbUSSD_CENTER_LEN=pOriginalStruct->pbUSSD_CENTER_LEN;
        }
      }


	  if (pOriginalStruct->pbUSSD_SC)
	  {
        pNetwStruct->pbUSSD_SC=NEWARRAY(BYTE,pOriginalStruct->pbUSSD_SC_LEN); 
		if (pNetwStruct->pbUSSD_SC)
        {
			B_COPYSTRINGN(pNetwStruct->pbUSSD_SC, pOriginalStruct->pbUSSD_SC, pOriginalStruct->pbUSSD_SC_LEN);
			pNetwStruct->pbUSSD_SC_LEN=pOriginalStruct->pbUSSD_SC_LEN;
		}
      }


	  pNetwStruct->iUSSD_SC_TYPE=pOriginalStruct->iUSSD_SC_TYPE;
  }
  return pNetwStruct;
} */


/*---------------------------------------------------
Creates a global WAE object that contains e.g.
config data, used from SDL at WAP client startup
-----------------------------------------------------*/
void* SDL_CreateWAEManagementObject(void)
{
   WAEMAINOBJECT *pTheObj;
   pTheObj = NEWSTRUCT(WAEMAINOBJECT);
   
   if (pTheObj)
   {
	   pTheObj->iDefaultExpireTime= cfg_wae_ua_defaultCacheExpireTime;
	   pTheObj->iCacheMode = CACHE_MODE_NEVER_CHECK;
	   pTheObj->liCacheSize = 0;
	   pTheObj->iOTAIsAllowed = 0;
	   pTheObj->iNrOfFreeRequestProc = 0;

	   /* The maximum number of parallell requests is 250, and we use
      * 2 bits to store information about each. Hence, we need
      * at least 63 bytes here. However, 64 is a nice round number. */
	   pTheObj->pTID_Table = NEWARRAY (BYTE, 64);
	   
	   if (pTheObj->pTID_Table)
	      memset (pTheObj->pTID_Table, 0, 64);
	   else
	      CLNTa_error (0, ERR_WAE_OUT_OF_MEMORY, ERRTYPE_FATAL);

     pTheObj->lastTID = 0; /* The last TID allocated. */
#ifdef CONFIG_SKT_SEC
		pTheObj->pbLastEncrypedURL = NULL;
#endif
   }
   else
	 CLNTa_error (0, ERR_WAE_OUT_OF_MEMORY, ERRTYPE_FATAL);
   return (void*)pTheObj;
}



/*---------------------------------------------------
Deletes WAE object that contains e.g.
config data, used at WAP client shutdown
-----------------------------------------------------*/
void SDL_DeleteWAEManagementObject(void** ppTheObject)
{
   WAEMAINOBJECT** ppTheObj = NULL;
   ppTheObj = (WAEMAINOBJECT**)ppTheObject;

   if ((*ppTheObject)!=NULL)
   {
     DEALLOC (&((*ppTheObj)->pTID_Table));
#ifdef CONFIG_SKT_SEC
	 DEALLOC (&((*ppTheObj)->pbLastEncrypedURL));
#endif
     DEALLOC(ppTheObj);
   }
}



#endif
