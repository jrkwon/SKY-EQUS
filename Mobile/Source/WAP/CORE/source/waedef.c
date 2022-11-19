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
#include "waedef.h"
#include "wae_cmmn.h"

/*---- "constructor" and "destructor" for  URLREQSTRUCT ------*/

URLREQSTRUCT* New_URLREQSTRUCT(void)
{
   URLREQSTRUCT*  pURLStruct;
   pURLStruct = NEWSTRUCT(URLREQSTRUCT);

   if (pURLStruct)
   {
 	  pURLStruct->pbURL=NULL;                             
	  pURLStruct->pbReferer=NULL;     
	  pURLStruct->pbAccept=NULL;                          
	  pURLStruct->pbBody=NULL;                   
      pURLStruct->iBodyLength=0;
	  pURLStruct->iMethod=0;
	  pURLStruct->iLastModified=0;
	  pURLStruct->pbEtag=NULL;
	  pURLStruct->iContentType=0;
	  pURLStruct->pbContentType=NULL;
	  pURLStruct->iContentCharSet = IANA_CHARSET_INVALID;
	  pURLStruct->iTotalPostSize = 0;
   }
   return pURLStruct;
}

void Delete_URLREQSTRUCT(URLREQSTRUCT** ppTheObject)
{
  if (*ppTheObject !=NULL)
  {
    DEALLOC(&((*ppTheObject)->pbURL));
    DEALLOC(&((*ppTheObject)->pbReferer));
	DEALLOC(&((*ppTheObject)->pbAccept));
    DEALLOC(&((*ppTheObject)->pbBody));
	DEALLOC(&((*ppTheObject)->pbEtag));
	DEALLOC(&((*ppTheObject)->pbContentType));
    DEALLOC(ppTheObject);
  }
}


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
	   pTheObj->iDefaultExpireTime= 86400;
	   pTheObj->iCacheMode = CACHE_MODE_NEVER_CHECK;
	   pTheObj->liCacheSize = 0;
	   pTheObj->iNrOfFreeRequestProc = 0;
	   pTheObj->pbUSERAGENT = NULL;
	   pTheObj->pbPROFILE = NULL;
	   pTheObj->pbPROFILE_DIFF = NULL;
	   pTheObj->iPROFILE_DIFF_LEN = 0;

	   /* The maximum number of parallell requests is 250, and we use
      * 2 bits to store information about each. Hence, we need
      * at least 63 bytes here. However, 64 is a nice round number. */
	   pTheObj->pTID_Table = NEWARRAY (BYTE, 64);
	   
	   if (pTheObj->pTID_Table)
	      memset (pTheObj->pTID_Table, 0, 64);
	   else
	      CLNTa_error (0, ERR_WAE_OUT_OF_MEMORY, ERRTYPE_FATAL);

     pTheObj->lastTID = 0; /* The last TID allocated. */

#ifdef CONFIG_PUSH
	 pTheObj->iSIInterrupt =0;
	 pTheObj->iSLInterrupt =0;
	 pTheObj->iSecLevel    =0;
#endif
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
	 DEALLOC (&((*ppTheObj)->pbUSERAGENT));
     DEALLOC (&((*ppTheObj)->pbPROFILE));
     DEALLOC (&((*ppTheObj)->pbPROFILE_DIFF));
     DEALLOC (&((*ppTheObj)->pTID_Table));
#ifdef CONFIG_SKT_SEC
	 DEALLOC (&((*ppTheObj)->pbLastEncrypedURL));
#endif
     DEALLOC(ppTheObj);
   }
}

#endif
