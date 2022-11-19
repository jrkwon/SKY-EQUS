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
/*========================================================================

	WAP Push Implementation Project

==========================================================================

	File: PushLL.C

	Description:

	Author: Martin Ranstorp, Kent Olsson AU-System AB

	Revision history:
  	Date    Rev  Sign  Comment
  	990930  PA1  MRP   First version
	991206  PA2  MRP   Updated. Working in WAE test environment
	000124  PA3  MRP   Updated after inspection.
	000214	PA4  MRP   Copyright added.
	000316	PA5	 MRP   Push API changed.
	000511  PA6  MRP   Startup scanning changed.
	001114	PA7	 KEOL  Uppdated after change request regarding handling of GetSI and GetSL (reply with initiators URI as well) 	
	001212  PA8  HEAD  The repository is now initiated and closed from the application.
	002020  PA9  HEAD  Instead of storage can now a file-api be used to store push messages.

------------------------------------------------------------------------*/

/* Private include statements */

#include "cmmnrsrc.h"
#include "pushll.h"
#include "push.h"
#include "storage.h"
#include "aapimem.h"
#include "aapipush.h"

#ifdef FILE_PUSH_REPOSITORY
#include "aapifile.h"
#define PUSH_CATEGORY (char) 'P'
#endif

/* Private strucs */
static struct{
	UINT8 isInitialized;
#ifdef FILE_PUSH_REPOSITORY
	UINT32 storageSize;
	UINT32 maxStorageSize;
#else
	StorageObject *store;
#endif
} pushMem = {0}; 

/*========================================================================
	INTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	EXTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	PUSH_Store
==========================================================================
Purpose: Store an Push in permanent push memory
Params:  vsPushContent (MUST be of pPUSHCONTENTSTRUCT type)
Return:  0 =OK, -1 = Error, -2 = Message larger than repository.
=========================================================================*/
INT8 PUSH_Store( void* pContext, void* pvPushContent, 
					UINT32* piContentID )
{
	PUSHCONTENTSTRUCT* pPushContent=NULL;
	BYTE *pbData=NULL;
	UINT32 iDataSize=0;
	UINT16 iUrlLen=0;
	UINT16 iSIidLen=0;
	UINT16 iMessLen=0;
	UINT16 iInitUriLen=0;
	BOOL myCheck;
#ifdef FILE_PUSH_REPOSITORY
	INT32 iBytesWritten;
	INT16 iFile;
#endif

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End PUSH_Store");
#endif

	if (!pushMem.isInitialized) {
		return -2;
	}

	pPushContent = (PUSHCONTENTSTRUCT*)pvPushContent;
	/* Push content to store must be defined */
	if (pPushContent!=NULL)
	{
		
		if (pPushContent->iType == SI)     /* content type is SI */
		{
			/* Get size of Url */
			if ((pPushContent->content).pSIContentStruct->pbUrl != NULL)
			{
				iUrlLen=B_STRINGLENGTH((char*)(pPushContent->content).pSIContentStruct->pbUrl);
			}
			else
				iUrlLen = 1;	

			/* Get size of SI-id */
			if ((pPushContent->content).pSIContentStruct->pwchSIid != NULL)
			{
				iSIidLen=STRINGLENGTH((pPushContent->content).pSIContentStruct->pwchSIid);
			}
			else
				iSIidLen = 1;

			/* Get size of message text */
			if ((pPushContent->content).pSIContentStruct->pwchMessage != NULL)
			{
				iMessLen=STRINGLENGTH((pPushContent->content).pSIContentStruct->pwchMessage);
			}
			else
				iMessLen = 1;

			/* Get size of InitUri */
			if ((pPushContent->content).pSIContentStruct->pbInitUri != NULL)
			{
				iInitUriLen=B_STRINGLENGTH((char *)(pPushContent->content).pSIContentStruct->pbInitUri); 
			}
			else
				iInitUriLen = 1;									
			
			/* Calculate total size of resource */
            /* the last 2 is for the Id towards the WAP application */
			iDataSize=(22+iUrlLen+(2*iSIidLen)+(2*iMessLen)+ iInitUriLen + 2);

			/* Check if enough free memory */
#ifdef FILE_PUSH_REPOSITORY
			pContext = pContext;
			if (iDataSize>pushMem.maxStorageSize)
				return -2;
			if ((pushMem.storageSize+iDataSize)<=pushMem.maxStorageSize) {
				iFile = FILEa_create (PUSH_CATEGORY, piContentID);
				myCheck= (iFile==-1) ? FALSE : TRUE;
			}
			else
				myCheck = FALSE;
#else
			if (iDataSize>(((PUSHCONTEXT*)pContext)->Storage).size)
				return -2;
			myCheck = Storage_AllocateBlock(&((PUSHCONTEXT*)pContext)->Storage, iDataSize, piContentID);
#endif
			if (myCheck)
			{
				/* OK. Copy data to push memory. */
				pbData=NEWARRAY(BYTE,iDataSize);
				if (pbData==NULL)
				{
					return -1;
				}

				pbData[0]=SI;
				pbData[1]=PUSH_STORED;	
				(pPushContent->content).pSIContentStruct->iStatus=PUSH_STORED;
			
				/* Store Created date */
				B_COPYSTRINGN(pbData+2,&((pPushContent->content).pSIContentStruct->iCreated),4);
			
				/* Store Expires date */
				B_COPYSTRINGN(pbData+6,&((pPushContent->content).pSIContentStruct->iExpires),4);
			
				/* Store Action */
				pbData[10]=(pPushContent->content).pSIContentStruct->iAction;

				/* Store PortType */
				pbData[11]=pPushContent->iPortType;

				/* Store iNewChannelId */
				pbData[12]=pPushContent->iNewChannelId;

				/* Store SIid length */
				B_COPYSTRINGN(pbData+13,&iSIidLen,2);

				/* Store SIid */
				if ((pPushContent->content).pSIContentStruct->pwchSIid != NULL)
				{
					/* Store SIid */
					B_COPYSTRINGN(pbData+15,
					(void*)(pPushContent->content).pSIContentStruct->pwchSIid,2*iSIidLen);
				}
				else
					pbData[15] = 0;

                 /* store the push Id towards the WAP application */
                B_COPYSTRINGN(pbData+15+2*iSIidLen, &(pPushContent->iIdNumber),2);

				/* Store Url length */
				B_COPYSTRINGN(pbData+17+2*iSIidLen,&iUrlLen,2);

				/* Store url */
				if ((pPushContent->content).pSIContentStruct->pbUrl != NULL)
				{
					/* Store url */
					B_COPYSTRINGN(pbData+19+2*iSIidLen,
					(void*)(pPushContent->content).pSIContentStruct->pbUrl,iUrlLen);
				}
				else
					pbData[19+2*iSIidLen] = 0;

				/* Store message text length */
				B_COPYSTRINGN(pbData+19+2*iSIidLen+iUrlLen,&iMessLen,2);

				/* Store message */
				if ((pPushContent->content).pSIContentStruct->pwchMessage != NULL)
				{
					/* Store message  */
					B_COPYSTRINGN(pbData+21+2*iSIidLen+iUrlLen,
					(void*)(pPushContent->content).pSIContentStruct->pwchMessage,2*iMessLen);
				}
				else
					pbData[21+2*iSIidLen+iUrlLen] = 0;

				/* Store InitUri length */
				B_COPYSTRINGN(pbData+21+2*iSIidLen+iUrlLen+2*iMessLen,&iInitUriLen,2);    
				
				/* Store inituri */
				if ((pPushContent->content).pSIContentStruct->pbInitUri != NULL)
				{
					/* Store inituri */
					B_COPYSTRINGN(pbData+23+2*iSIidLen+iUrlLen+2*iMessLen,					
					(void*)((pPushContent->content).pSIContentStruct->pbInitUri),iInitUriLen);
				}
				else
					pbData[23+2*iSIidLen+iUrlLen+2*iMessLen] = 0;                
				
					/* Store the SI under the ContentID */
#ifdef FILE_PUSH_REPOSITORY
				iBytesWritten = FILEa_write(PUSH_CATEGORY, *piContentID, pbData, 0, iDataSize);
				myCheck = (iBytesWritten == -1 || iDataSize!=(UINT32) iBytesWritten) ? FALSE : TRUE;
				if (!myCheck)
					FILEa_delete(PUSH_CATEGORY, *piContentID);
				else {
					pushMem.storageSize+=iBytesWritten;
					FILEa_flush (PUSH_CATEGORY, *piContentID);
				}
#else
				myCheck = Storage_Put (&((PUSHCONTEXT*)pContext)->Storage,*piContentID, 0, iDataSize, pbData);
#endif
				if (myCheck)
				{	
					/* Store the ContentID in the resource */
					pPushContent->iId=*piContentID;

					DEALLOC(&pbData);
					return 0;
				}
				DEALLOC(&pbData);
			}
		}
		else if (pPushContent->iType == SL)    /* content type is SL */
		{
			/* Get size of Url */
			iUrlLen=B_STRINGLENGTH((char*)(pPushContent->content).pSLContentStruct->pbUrl);

			/* Get size of InitUri */
			if ((pPushContent->content).pSLContentStruct->pbInitUri != NULL)
			{
				iInitUriLen=B_STRINGLENGTH((char *)(pPushContent->content).pSLContentStruct->pbInitUri);  
			}
			else
				iInitUriLen = 1;
			
			/* Calculate total size of resource */
            /* the last 2 is for the Id towards the WAP application */
			iDataSize=(9+4+iUrlLen+iInitUriLen + 2);

			/* Check if enough free memory */
#ifdef FILE_PUSH_REPOSITORY
			if (iDataSize>pushMem.maxStorageSize)
				return -2;
			if ((pushMem.storageSize+iDataSize)<=pushMem.maxStorageSize) {
				iFile = FILEa_create (PUSH_CATEGORY, piContentID);
				myCheck= (iFile==-1) ? FALSE : TRUE;
			}
			else
				myCheck = FALSE;
#else
			if (iDataSize>(((PUSHCONTEXT*)pContext)->Storage).size)
				return -2;
			myCheck = Storage_AllocateBlock(&((PUSHCONTEXT*)pContext)->Storage, iDataSize, piContentID);
#endif
			if (myCheck)
			{
				/* OK. Copy data to push memory. */
				pbData=NEWARRAY(BYTE,iDataSize);
				if (pbData==NULL)
				{
					return -1;
				}


				pbData[0]=SL;
				pbData[1]=PUSH_STORED;	
				(pPushContent->content).pSLContentStruct->iStatus=PUSH_STORED;
			
				/* Store Action */
				pbData[2]=(pPushContent->content).pSLContentStruct->iAction;

				/* Store PortType */
				pbData[3]=pPushContent->iPortType;

				/* Store iNewChannelId */
				pbData[4]=pPushContent->iNewChannelId;

				/* Store Url length */
				B_COPYSTRINGN(pbData+5,&iUrlLen,2);

				/* Store url */
				B_COPYSTRINGN(pbData+7,
				(void*)(pPushContent->content).pSLContentStruct->pbUrl,iUrlLen);

				/* store receive date */
				B_COPYSTRINGN(pbData+7+iUrlLen,&((pPushContent->content).pSLContentStruct->iReceived),4);

                /* store the push Id towards the WAP application */
                B_COPYSTRINGN(pbData+11+iUrlLen, &(pPushContent->iIdNumber),2);

				/* Store InitUri length */
				B_COPYSTRINGN(pbData+13+iUrlLen,&iInitUriLen,2);

				if ((pPushContent->content).pSLContentStruct->pbInitUri != NULL)
				{
					/* Store inituri */
					B_COPYSTRINGN(pbData+15+iUrlLen,								
				(void*)((pPushContent->content).pSLContentStruct->pbInitUri),iInitUriLen);

				}
				else
					pbData[15+iUrlLen] = 0;

                

				/* Store the SL under the ContentID */
#ifdef FILE_PUSH_REPOSITORY
				iBytesWritten = FILEa_write(PUSH_CATEGORY, *piContentID, pbData, 0, iDataSize);
				myCheck = (iBytesWritten == -1 || iDataSize!=(UINT32) iBytesWritten) ? FALSE : TRUE;
				if (!myCheck)
					FILEa_delete(PUSH_CATEGORY, *piContentID);
				else {
					pushMem.storageSize+=iBytesWritten;
					FILEa_flush (PUSH_CATEGORY, *piContentID);
				}
#else
				myCheck = Storage_Put (&((PUSHCONTEXT*)pContext)->Storage,*piContentID, 0, iDataSize, pbData);
#endif
				if (myCheck)
				{	
					/* Store the ContentID in the resource */
					pPushContent->iId=*piContentID;
					DEALLOC(&pbData);
					return 0;
				}
				DEALLOC(&pbData);
			}
		}
	}
	return -2;

} /* end of PUSH_Store */


/*========================================================================
	PUSH_StorePush
==========================================================================
Purpose: Store the Push in memory
Params:  pContext, piContentID
Return:  0 =OK, -1 = Error, -2 = Message larger than repository.
=========================================================================*/
INT8 PUSH_StorePush( void* pContext, void* pvContent,
					UINT32* iContentID )
{

	UINT32 iContId=0;
	INT8 bRes;

	if (!pushMem.isInitialized) {
		return FALSE;
	}

	bRes=PUSH_Store(pContext,pvContent,&iContId);

	if (bRes==0)
	{
		*iContentID=iContId;
	}
	return bRes;

} /* end of PUSH_StorePush */

/*========================================================================
	PUSH_DeletePush
==========================================================================
Purpose: Delete the Push content
Params:  pContext, iContentID
Return:  BOOL
=========================================================================*/
BOOL PUSH_DeletePush( void* pContext, UINT32 iContentID, UINT8 iType )
{
	BOOL fOK=FALSE;
	PUSHCONTENTSTRUCT* pPushContent=NULL;
#ifdef FILE_PUSH_REPOSITORY
	INT32 liFileSize;
#endif

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End PUSH_DeletePush");
#endif

	if (!pushMem.isInitialized) {
		return FALSE;
	}

	/* Find the push content to delete */
	pPushContent = (PUSHCONTENTSTRUCT *)PUSH_GetContentWithID(((PUSHCONTEXT*)pContext),iContentID,iType);

    /* Remove the push from push memory */
#ifdef FILE_PUSH_REPOSITORY
	liFileSize=FILEa_getSize(PUSH_CATEGORY, iContentID);
	pushMem.storageSize -= (liFileSize==-1) ? 0 : liFileSize; 
	FILEa_delete(PUSH_CATEGORY, iContentID);
	fOK = TRUE;
#else
	fOK=Storage_DeleteBlock(&((PUSHCONTEXT*)pContext)->Storage,iContentID);
#endif

	if (pPushContent!=NULL)
	{
		ME_Free_PushContentStruct(pPushContent);
		return fOK;
	}
	return FALSE;

} /* end of PUSH_DeletePush */

/*========================================================================
	PUSH_GetStoredSI
==========================================================================
	The function extracts data from the instream and returns the SI
	part of a PUSHCONTENTSTRUCT. The instream must be correct. It is the
	caller's responsibility to deallocate the returned data.

	The function is called by PUSH_GetContentWithId

	Input: Data
	Output: Pointer to SICONTENTSTRUCT
==========================================================================*/
pSICONTENTSTRUCT PUSH_GetStoredSI (BYTE* pbData)
{
	pSICONTENTSTRUCT pContent=NULL;
	UINT32 iOffset=0;
	UINT16 iLen=0;

	if (!pushMem.isInitialized) {
		return NULL;
	}

	pContent=NEWSTRUCT(SICONTENTSTRUCT);
	if (pContent==NULL)
	{
		return NULL;
	}

	
	pContent->iStatus=(UINT8)(pbData[1]);
	pContent->iAction=(UINT8)(pbData[10]);

	/* Copy iCreated */
	B_COPYSTRINGN(&(pContent->iCreated),pbData+2,4);
	/* Copy iExpires */
	B_COPYSTRINGN(&(pContent->iExpires),pbData+6,4);

	/* Get length of SIid and allocate memory */
	B_COPYSTRINGN(&(iLen),pbData+13,2);
	if (pbData[15] == 0 && iLen == 1)																		
		pContent->pwchSIid = NULL;
	else
	{
		pContent->pwchSIid=NEWARRAY(WCHAR,iLen+1);
		if (pContent->pwchSIid==NULL)
		{
			DEALLOC(&(pContent));
			return NULL;
		}
		/* Copy SIid */
		B_COPYSTRINGN(pContent->pwchSIid,pbData+15,iLen*2);
		/* Set termination char */
		pContent->pwchSIid[iLen]=0;
	}
	iOffset=17+iLen*2;

/* Get length of url and allocate memory */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);
	if (pbData[iOffset + 2] == 0 && iLen == 1)
	{
		pContent->pbUrl = NULL;
		iOffset+=2;
	}
	else
	{
		pContent->pbUrl=NEWARRAY(BYTE,iLen+1);
		if (pContent->pbUrl==NULL)
		{
			DEALLOC(&(pContent->pwchSIid));
			DEALLOC(&(pContent));
			return NULL;
		}
		iOffset+=2;
		/* Copy url */
		B_COPYSTRINGN(pContent->pbUrl,pbData+iOffset,iLen);  
		/* Set termination char */
		pContent->pbUrl[iLen]=0;	
	}
	iOffset+=iLen;

/* Get length of text message and allocate memory */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);
	if (pbData[iOffset + 2] == 0 && iLen == 1)
	{
		pContent->pwchMessage = NULL;
		iOffset+=2;
	}
	else
	{
		pContent->pwchMessage=NEWARRAY(WCHAR,iLen+1);
		if (pContent->pwchMessage==NULL)
		{
			DEALLOC(&(pContent->pwchSIid));
			DEALLOC(&(pContent->pbUrl));
			DEALLOC(&(pContent));
			return NULL;
		}
		iOffset+=2;
		/* Copy message text */
		B_COPYSTRINGN(pContent->pwchMessage,pbData+iOffset,iLen*2);
		/* Set termination char */
		pContent->pwchMessage[iLen]=0;
	}
	
	iOffset = iOffset + 2*iLen; 

	/* Get length of InitUri and allocate memory */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);	
	iOffset+=2;	
	if (pbData[iOffset] == 0 && iLen == 1)
		pContent->pbInitUri = NULL;
	else
	{
		pContent->pbInitUri=NEWARRAY(BYTE,iLen+1);
		if (pContent->pbInitUri==NULL)
		{		
			DEALLOC(&(pContent->pwchMessage));
			DEALLOC(&(pContent->pwchSIid));
			DEALLOC(&(pContent->pbUrl));
			DEALLOC(&(pContent));
			return NULL;
		}
		/* Copy message text */
		B_COPYSTRINGN(pContent->pbInitUri,pbData+iOffset,iLen);	
		/* Set termination char */
		pContent->pbInitUri[iLen]=0;
	}

	return pContent;
} /* End of PUSH_GetStoredSI */

/*========================================================================
	PUSH_GetStoredSL
==========================================================================
	The function extracts data from the instream and returns the SL
	part of a PUSHCONTENTSTRUCT. The instream must be correct. It is the
	caller's responsibility to deallocate the returned data.

	The function is called by PUSH_GetContentWithId

	Input: Data
	Output: Pointer to SLCONTENTSTRUCT
==========================================================================*/
pSLCONTENTSTRUCT PUSH_GetStoredSL (BYTE* pbData)
{
	pSLCONTENTSTRUCT pContent=NULL;
	UINT32 iOffset=0;
	UINT16 iLen=0;

	if (!pushMem.isInitialized) {
		return NULL;
	}

	pContent=NEWSTRUCT(SLCONTENTSTRUCT);
	if (pContent==NULL)
	{
		return NULL;
	}
	pContent->iStatus=(UINT8)(pbData[1]);
	pContent->iAction=(UINT8)(pbData[2]);

	/* Get length of url and allocate memory */
	B_COPYSTRINGN(&(iLen),pbData+5,2);
	pContent->pbUrl=NEWARRAY(BYTE,iLen+1);
	if (pContent->pbUrl==NULL)
	{
		DEALLOC(&(pContent));
		return NULL;
	}
	/* Copy url */
	B_COPYSTRINGN(pContent->pbUrl,pbData+7,iLen);  
	/* Set termination char */
	pContent->pbUrl[iLen]=0;
	iOffset = 7 + iLen;

	/* copy receive date */
	B_COPYSTRINGN(&(pContent->iReceived),pbData+iOffset,4);
		
	iOffset += 2;
	
	/* Get length of InitUri and allocate memory */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);
	iOffset+=2;
	/* Copy message text */
	if (pbData[iOffset] == 0 && iLen == 1)
		pContent->pbInitUri = NULL;
	else
	{
		pContent->pbInitUri=NEWARRAY(BYTE,iLen+1);
		if (pContent->pbInitUri==NULL)
		{		
			DEALLOC(&(pContent->pbUrl));
			DEALLOC(&(pContent));
			return NULL;
		}
		B_COPYSTRINGN(pContent->pbInitUri,pbData+iOffset,iLen);	
		/* Set termination char */
		pContent->pbInitUri[iLen]=0;
	}
	
	return pContent;

} /* End of PUSH_GetStoredSL */

/*========================================================================
	PUSH_ScanForContent
==========================================================================
	The function scans the memory for content and creates a list of all 
	pushes in the push buffer (pushes that have expired are removed from
	the push buffer). The list is of the type LISTHEADER and the elements 
	in the list are of the type PUSHLISTSTRUCT. It is the caller's 
	responsibility to deallocate the memory. The function should be 
	called repeatedly until it returns FALSE. 

	NOTE! When the function is called the first time, ppContentList 
	should be NULL.

	Input: void* (MUST be pPUSHCONTEXT), void** (MUST be LISTHEADER**)
	Output: TRUE if the function should be called again, FALSE otherwise
==========================================================================*/
BOOL PUSH_ScanForContent( void* pContentList, void* pContext )
{
	UINT16	iNbrOfElm=0;
	UINT32* piContentList=NULL;
	UINT32	iCurId=0;
	BYTE*	pbData=NULL;
	UINT8	iRes;
	pPUSHLISTSTRUCT pPushListStruct=NULL;
	UINT16	iLen=0;
	UINT32	iLen32=0;
	INT32	myLen=0;
	BYTE*	pbTemp = NULL;
	BOOL    myCheck = FALSE;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End PUSH_ScanForContent");
#endif

	if (!pushMem.isInitialized) {     
		return FALSE;
	}

	/* Check ScanPhase */
	switch (((pPUSHCONTEXT)(pContext))->iScanPhase)
	{
	case 1:

		if (pContentList!=NULL)   
		{
			/* First time. Get all contentids */
#ifdef FILE_PUSH_REPOSITORY
			iNbrOfElm=FILEa_getFileIds(PUSH_CATEGORY, NULL, 65535);
			if (iNbrOfElm!=0) {
				piContentList = NEWARRAY (UINT32, iNbrOfElm);
				if (piContentList==NULL)
					return FALSE;
				iNbrOfElm=FILEa_getFileIds(PUSH_CATEGORY, piContentList, iNbrOfElm);
				if (iNbrOfElm==-1) {
					DEALLOC(&piContentList);
					return FALSE;
				}
			}
#else
			if (!Storage_GetAllBlockIds(&((PUSHCONTEXT*)pContext)->Storage, &piContentList,&iNbrOfElm))
				return FALSE;
#endif
			if (iNbrOfElm!=0)
			{
				/* Store data in context */
				((PUSHCONTEXT*)(pContext))->piElementList=piContentList;
				((PUSHCONTEXT*)(pContext))->iNumberOfElements=iNbrOfElm;
				((PUSHCONTEXT*)(pContext))->iCurrentElement=0; 
				/* Go to Phase 2. */
				((pPUSHCONTEXT)(pContext))->iScanPhase=2; 
				return TRUE;
			}
			return FALSE;

		}
		/* Error */
		break;
		
	case 2:

		/* Check if more id:s to scan. */
		if ( ((pPUSHCONTEXT)(pContext))->iCurrentElement <
					((PUSHCONTEXT*)(pContext))->iNumberOfElements )
		{
			/* Get id */
			iCurId=((pPUSHCONTEXT)(pContext))->
				piElementList[((PUSHCONTEXT*)(pContext))->iCurrentElement];

			/* Get size of content */
#ifdef FILE_PUSH_REPOSITORY
			myLen = FILEa_getSize(PUSH_CATEGORY, iCurId);
			iLen32 = myLen!=-1 ? (UINT32) myLen : 0;
			if (iLen32 != 0 && (pushMem.storageSize+iLen32)>pushMem.maxStorageSize) {
				iLen32=0;
				FILEa_delete(PUSH_CATEGORY, iCurId);
			}
			pushMem.storageSize+=iLen32;
#else
			iLen32 = Storage_GetBlockSize(&((PUSHCONTEXT*)pContext)->Storage,iCurId);
#endif
			if (iLen32 != 0)
			{
				pbData=NEWARRAY(BYTE,iLen32);
				/* Get content */
#ifdef FILE_PUSH_REPOSITORY
				myLen = FILEa_read(PUSH_CATEGORY, iCurId, (void *) pbData, 0, iLen32);
				myCheck = myLen!=-1 ? TRUE : FALSE;
#else
				myCheck = Storage_Get(&((PUSHCONTEXT*)pContext)->Storage,iCurId,0,iLen32,pbData);
#endif

				if (myCheck)
				{
					pPushListStruct=NEWSTRUCT(PUSHLISTSTRUCT);
					/* Check type */
					if (pbData[0] == SI)
					{  
						/* Get Expires date */										
						B_COPYSTRINGN(&(pPushListStruct->iExpires),pbData+6,4);		
						/* Get Created date */
						B_COPYSTRINGN(&(pPushListStruct->iCreated),pbData+2,4);
						pPushListStruct->iPushType=(UINT8)pbData[0];
						pPushListStruct->iStatus=(UINT8)pbData[1];
						pPushListStruct->iAction=(UINT8)pbData[10];
						pPushListStruct->iLLPushId=iCurId;
						/* Get length of PushId */
						B_COPYSTRINGN(&iLen,pbData+13,2);
						pPushListStruct->pwchPushId=NEWARRAY(WCHAR,iLen+1);
						/* Get pwchPushId */
						B_COPYSTRINGN(pPushListStruct->pwchPushId,pbData+15,iLen*2);
						/* Set termination char */
						pPushListStruct->pwchPushId[iLen]=0;
                        B_COPYSTRINGN(&(pPushListStruct->iIdNumber),pbData+15+iLen*2,2);
			            iRes = SDL_AddElementLast( pContentList,pPushListStruct->iIdNumber, (void*)pPushListStruct );
					}   
					else  /* Type is SL  */ 
					{
						pPushListStruct->iPushType=(UINT8)pbData[0];
						pPushListStruct->iStatus=(UINT8)pbData[1];
						pPushListStruct->iAction=(UINT8)pbData[2];
						pPushListStruct->iLLPushId=iCurId;
						/* Get length of PushId */				
						B_COPYSTRINGN(&iLen,pbData+5,2);
						pbTemp = NEWARRAY(BYTE,iLen+1);						
						/* Get pwchPushId */
						B_COPYSTRINGN(pbTemp,pbData+7,iLen);					/*Need pbTemp to convert from BYTE to WCHAR*/
						/*pPushListStruct->pwchPushId=NEWARRAY(WCHAR,iLen+1);*/
						pPushListStruct->pwchPushId = wip_byte2wchar(pbTemp);
						DEALLOC(&pbTemp);
						/* Set termination char */
						pPushListStruct->pwchPushId[iLen]=0;
						B_COPYSTRINGN(&(pPushListStruct->iCreated),pbData+7+iLen,4);
                        B_COPYSTRINGN(&(pPushListStruct->iIdNumber),pbData+11+iLen,2);
						iRes = SDL_AddElementLast( pContentList,pPushListStruct->iIdNumber, (void*)pPushListStruct );	
					}
				}
				DEALLOC(&pbData);
			}
			/* Next element in list */
			((PUSHCONTEXT*)(pContext))->iCurrentElement++;
			return TRUE;
		}
		else /* No more in Push memory */
		{
			/* Clean up */
			((PUSHCONTEXT*)(pContext))->iScanPhase=0;
			((PUSHCONTEXT*)(pContext))->iCurrentElement=0;
			((PUSHCONTEXT*)(pContext))->iNumberOfElements=0;
			DEALLOC(&(((PUSHCONTEXT*)(pContext))->piElementList));
			return FALSE;
		}
	} /* end switch */
	return FALSE;

} /* end of PUSH_ScanForContent */

/*========================================================================
	PUSH_GetContentWithID
==========================================================================
Purpose: Fetch PushContent with ContentID
Params:  iContentID
Return:  pPushContentStruct
=========================================================================*/
void* PUSH_GetContentWithID( void* pContext, UINT32 iContentID,
										 UINT8 iType)
{
	BYTE* pbData=NULL;
	UINT32 iContentSize=0;

#ifdef FILE_PUSH_REPOSITORY
	INT32 iSize;
#endif

	BOOL myCheck;
	PUSHCONTENTSTRUCT* pContent=NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End PUSH_GetContentWithID");
#endif

	if (!pushMem.isInitialized) {     
		return FALSE;
	}

	/* Get size of content */
#ifdef FILE_PUSH_REPOSITORY
	pContext = pContext;
	iSize = FILEa_getSize( PUSH_CATEGORY, iContentID);
	iContentSize = iSize!=-1 ? (UINT32) iSize : 0;
#else
	iContentSize=Storage_GetBlockSize (&((PUSHCONTEXT*)pContext)->Storage,iContentID);
#endif
	if (iContentSize!=0)
	{
		pbData=NEWARRAY(BYTE,iContentSize);
		if (pbData!=NULL)
		{
#ifdef FILE_PUSH_REPOSITORY
		iSize = FILEa_read( PUSH_CATEGORY, iContentID, pbData, 0, iContentSize);
		myCheck = iSize!=-1 ? TRUE : FALSE;
		iContentSize = (UINT32) iSize;
#else
		myCheck = Storage_Get (&((PUSHCONTEXT*)pContext)->Storage, iContentID,0,iContentSize,pbData);
#endif
			if(myCheck)
			{
				pContent=NEWSTRUCT(PUSHCONTENTSTRUCT);
				if (pContent!=NULL)
				{
					pContent->iType=pbData[0];
					pContent->iId=iContentID;

					if((pbData[0]==SI)&&(iType==SI))
					{
						pContent->iPortType=pbData[11];
						pContent->iNewChannelId = pbData[12];
						pContent->content.pSIContentStruct=PUSH_GetStoredSI(pbData);
					}
					else if ((pbData[0]==SL)&&(iType==SL))
					{
						pContent->iPortType=pbData[3];
						pContent->iNewChannelId = pbData[4];
						pContent->content.pSLContentStruct=PUSH_GetStoredSL(pbData);
					}
					else
					{
						/* Error */
						DEALLOC(&pContent);
					}
				}
			}
			DEALLOC(&pbData);
		}
	}
	return (void*)pContent;

} /* end of PUSH_GetContentWithID */

/*========================================================================
	PUSH_ChangePushStatus
==========================================================================
Purpose: Change status on PushContent with ContentID
Params:  iContentID
Return:  TRUE
=========================================================================*/
BOOL PUSH_ChangePushStatus (void* pContext, UINT32 iContentID, UINT8 iStatus)
{

	BYTE pbData[2];
#ifdef FILE_PUSH_REPOSITORY
	INT32 iLen;
#endif

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End PUSH_ChangePushStatus");
#endif

	if (!pushMem.isInitialized) {     
		return FALSE;
	}

	/* Find push */
#ifdef FILE_PUSH_REPOSITORY
	pContext = pContext;
	iLen = FILEa_read (PUSH_CATEGORY, iContentID, (void *)pbData, 0, 2);
	if (iLen !=2)
		return FALSE;
#else
	if (!Storage_Get(&((PUSHCONTEXT*)pContext)->Storage,iContentID,0,2,pbData))
		return FALSE;
#endif
	/* Change status on push */	
	pbData[1]=iStatus;

#ifdef FILE_PUSH_REPOSITORY
	iLen = FILEa_write (PUSH_CATEGORY, iContentID, (void *)pbData, 0, 2);
	if (iLen !=2)
		return FALSE;
	FILEa_flush (PUSH_CATEGORY, iContentID);
#else
	if (!Storage_Put(&((PUSHCONTEXT*)pContext)->Storage,iContentID,0,2,pbData))
		return FALSE;
#endif

	return TRUE;

} /* end of PUSH_ChangePushStatus */

/*========================================================================
	PUSH_CompactPushBuffer
==========================================================================
	This function organizes the memory with no (minimal) fragmentation. 
	The function is called when a compactation of the push buffer is 
	needed. The function is called, for example, when content is to be 
	stored and the available memory in the push buffer is sufficient, but 
	fragmented in a way so that the largest free block is smaller than 
	the content. To allow scheduling, the compact process is not neces-
	sarily completed with one function call. As long as the function 
	returns TRUE it is possible to compact the push buffer more.

	Input: void* (MUST be pPUSHCONTEXT)
	Output: TRUE if possible to compact more, FALSE otherwise
==========================================================================*/
BOOL PUSH_CompactPushBuffer( void* pContext )
{

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End PUSH_CompactPushBuffer");
#endif

	if (!pushMem.isInitialized) {     
		return FALSE;
	}

	if (pContext != NULL)
	{
#ifdef FILE_PUSH_REPOSITORY
		return FALSE;
#else
		return (Storage_Compact(&((PUSHCONTEXT*)pContext)->Storage));
#endif
	}
	return FALSE;

} /* end of PUSH_CompactPushBuffer */


/*========================================================================
	PUSH_InitPushMem
==========================================================================*/
void* PUSH_InitPushMem (UINT32 iPushSize)
{
	pPUSHCONTEXT pContext=NULL;

	/* Create Context struct */
	pContext=NEWSTRUCT(PUSHCONTEXT);
	if (pContext==NULL)
	{
		return NULL;
	}

	/* Init fields */
	pContext->iNumberOfElements=0;
	pContext->iCurrentElement=0;
	pContext->piElementList=NULL;
	pContext->iScanPhase=1;

#ifdef FILE_PUSH_REPOSITORY
	pushMem.isInitialized = 1;
	pushMem.storageSize=0;
	pushMem.maxStorageSize=iPushSize;
	return pContext;
#else
	/* Init Storage Object */
	if (Storage_Init (&pContext->Storage,iPushSize,(ReadFunction*)MEMa_readPushRepository,
		(WriteFunction*)MEMa_writePushRepository))
	{
		pushMem.isInitialized = 1;
		pushMem.store=&(pContext->Storage);
		return pContext;
	}
	else
	{
		/* Error -> dealloc pContext */
		DEALLOC (&pContext);
		return NULL;
	}
#endif

}

/*========================================================================
	PUSH_ResetPushMem
==========================================================================
	The function removes all content in the push memory.

	Input: void* (MUST be pPUSHCONTEXT)
	Output: -
==========================================================================*/
void PUSH_ResetPushMem (void* pContext)
{
#ifdef FILE_PUSH_REPOSITORY
	INT16 iNbr;
#endif
	UINT32* piIds=NULL;
	UINT16 iLen=0;
	UINT16 iCount=0;

	/* Get all id:s */
#ifdef FILE_PUSH_REPOSITORY
	iNbr = FILEa_getFileIds (PUSH_CATEGORY, NULL, (UINT32) cfg_wae_push_in_buffer_size);
	if (iNbr==-1)
		return;
	piIds = NEWARRAY(UINT32, iNbr);
	if (piIds==NULL)
		return;
	iNbr = FILEa_getFileIds (PUSH_CATEGORY, piIds, (UINT16) iNbr);
	if (iNbr==-1)
		return;
	iLen=iNbr;
	pushMem.storageSize=0;
#else
	Storage_GetAllBlockIds(&((PUSHCONTEXT*)pContext)->Storage,&piIds,&iLen);
#endif

	while (iCount<iLen)
	{
		/* Delete block */
#ifdef FILE_PUSH_REPOSITORY
		FILEa_delete (PUSH_CATEGORY, piIds[iCount]);
#else
		Storage_DeleteBlock(&((PUSHCONTEXT*)pContext)->Storage,piIds[iCount]);
#endif
		iCount++;
	}

	DEALLOC(&piIds);

	/* Delete in context */
	((PUSHCONTEXT*)pContext)->iCurrentElement=0;
	((PUSHCONTEXT*)pContext)->iNumberOfElements=0;
	((PUSHCONTEXT*)pContext)->iScanPhase=1;
	DEALLOC(&((PUSHCONTEXT*)pContext)->piElementList);
}

/*========================================================================
	PUSH_TerminatePushMem
==========================================================================
	The function removes the PUSHCONTEXT struct. 
	
	Input: void* (MUST be pPUSHCONTEXT)
	Output: -
==========================================================================*/
void PUSH_TerminatePushMem (void* pContext)
{
	pPUSHCONTEXT pTemp=(PUSHCONTEXT*)pContext;

	/* Delete in context */
	DEALLOC(&((PUSHCONTEXT*)pContext)->piElementList);

	/* Delete memory struct */
#ifdef FILE_PUSH_REPOSITORY
	pushMem.storageSize=0;
#else
	Storage_Finalize(&(((PUSHCONTEXT*)pContext)->Storage));
#endif

	/* To use the pushMem. The PushMem must first be initialized*/
	pushMem.isInitialized = 0;
	
	/* Delete context */
	DEALLOC(&pTemp);

}

/*========================================================================
	PUSH_ClosePushMem
==========================================================================
	The function closes the pushMem. Run PUSH_InitPushMem to be able to use the 
	memory again. If more data is stored than liNewSize specifies then data
	will be removed.
	
	Input: pContext MUST be pPUSHCONTEXT
		   pContentList must be of type ListHeader
		   liNewSize (How much can be saved)
		   iType (if content should be removed then -1=oldest SI, -2 =oldest SL)
	Output: TRUE if closed, false if more content should be removed.
==========================================================================*/
BOOL PUSH_ClosePushMem(void* pContext, void *pContentList, UINT32 liNewSize, INT8 iType)
{
	UINT32 liDate=0;
	UINT32 liStorageSize;
	pPUSHCONTEXT pTemp=(PUSHCONTEXT*)pContext;

#ifdef FILE_PUSH_REPOSITORY
	INT32  liSize;
#endif

	void *pvListElement;
	pPUSHLISTSTRUCT pPushListStruct=NULL;

	if (!pushMem.isInitialized || pContentList==NULL) {
		MEMa_pushRepositoryClosed();
		return TRUE;
	}

	/*Remove the oldest SI or SL message if iType = -1 or -2*/
	pvListElement = ME_Push_Get_First(pContentList);
	if ((iType==-1 || iType==-2) && pvListElement!=NULL)
	{
		pPushListStruct = (pPUSHLISTSTRUCT) ME_Push_Get_PushListStruct(pvListElement);
		while (pPushListStruct!=NULL) {
			if (pPushListStruct->iPushType == SI && iType == -1) 
			{
				/*Message found and shall be deleted*/
#ifdef FILE_PUSH_REPOSITORY
				liSize=FILEa_getSize(PUSH_CATEGORY, pPushListStruct->iLLPushId);
				if (liSize==-1)
					return FALSE;
				pushMem.storageSize-=liSize;
				FILEa_delete(PUSH_CATEGORY, pPushListStruct->iLLPushId);
#else
				Storage_DeleteBlock (pushMem.store, pPushListStruct->iLLPushId);
#endif
				ME_Push_List_Delete( pContentList, pPushListStruct);
				break;
			}
			else if (pPushListStruct->iPushType == SL && iType == -2) 
			{
				/*Message found and shall be deleted*/
#ifdef FILE_PUSH_REPOSITORY
				liSize=FILEa_getSize(PUSH_CATEGORY, pPushListStruct->iLLPushId);
				if (liSize==-1)
					return FALSE;
				pushMem.storageSize-=liSize;
				FILEa_delete(PUSH_CATEGORY, pPushListStruct->iLLPushId);
#else
				Storage_DeleteBlock (pushMem.store, pPushListStruct->iLLPushId);
#endif
				ME_Push_List_Delete( pContentList, pPushListStruct);
				break;
			}
			pvListElement = ME_Push_Get_Next(pContentList, pvListElement);
			if ( pvListElement != NULL )
				pPushListStruct = (pPUSHLISTSTRUCT) ME_Push_Get_PushListStruct(pvListElement);
			else
				pPushListStruct = NULL;
		}
	}

#ifdef FILE_PUSH_REPOSITORY
	liStorageSize = pushMem.storageSize;
#else
	liStorageSize = Storage_BytesUsed(pushMem.store);
	if (liStorageSize<=4)
		liStorageSize=0;
#endif

	/*Push messages must be removed if this statement is true*/
	if (liStorageSize>liNewSize)
		return FALSE;

#ifdef FILE_PUSH_REPOSITORY
	pushMem.storageSize=0;
#else
	while (Storage_Compact (pushMem.store)) {
	}

	Storage_ChangeSize (pushMem.store, liNewSize);
	Storage_Finalize (pushMem.store);
#endif
	pushMem.isInitialized = 0;
	MEMa_pushRepositoryClosed(); 

	/* Delete in context */
	DEALLOC(&((PUSHCONTEXT*)pContext)->piElementList);

	/* Delete context */
	DEALLOC(&pTemp);
	return TRUE;

}


