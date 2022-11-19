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

	WAP WTA Implementation Project

==========================================================================

	File: RepLL.c

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
 	Date    Sign	Comment
 	990803  CEF		First version
	991001	JPR		Functions updated
	991006	JPR		tMODIFYROOT struct and functions added
	991008	JPR		Function REP_UpdateResource added
	991012	JPR		REP_ScanRepository implemented. Functions updated.
	991013	JPR		WTA_DEBUG included
	991019	JPR		Updates in functions
	991102	JPR		Corrections
	991105	JPR		Merge with file with xTraceAlg and some corrections.
	991110	JPR		Correction in calls of Free_RepContent
	000207	JPR		Functions REP_GetNextChToRemove and 
								REP_RemoveAllStaleChannels added
	000207	JPR		Updated for new adapters
	000215	JPR		Function REP_PutChannelInSDLList updated.
	000216	JPR		Function REP_ScanRepository updated (installing 
								channels are removed during scan)
	000223	JPR		Correction in REP_GetStoredResource (memory leak)
	000206	JPR		Error handling added to REP_StoreChannel
	000324	JPR		Correction in functions REP_GetNextChToRemove and 
								REP_PutChannelInSDLList
	000406	JPR		Correction in function REP_PutChannelInSDLList
	000530	JPR		Correction in function REP_MergeUpdateRootBlock
	000630	JPR		Updated for WAP 1.2.1
	001011	ASPN	Lower-case filenames
    010125  JPR     Space for User Accessible attribute added in function
                                REP_StoreChannel   
    010221  ASPN    Correction in REP_GetNextChToRemove, due to the added
                                attribute User Accessible
    010303  ASPN    User Accessible and Expiry Date attributes switched 
                                place in memory structure
    010330  ASPN    iReserved removed from struct tREPCONTEXT.
                    Functions REP_AllocateMemory and REP_FreeMemory removed.
    010410  ASPN    ServiceAccessible added
                    Function REP_SetServiceAccessible added

=========================================================================*/

/* Private include statements */

#include "repll.h"
#include "aapimem.h"

/* Debug in SDL set WTA_DEBUG to 1 */

/*========================================================================
==========================================================================
	INTERNAL FUNCTIONS
==========================================================================
=========================================================================*/


/*========================================================================
	HELP FUNCTIONS FOR HANDLING THE ROOT BLOCK
=========================================================================*/

/*========================================================================
	REP_AddResourceToContext
==========================================================================
	The function adds the resource id along with its hash value to the
	context.

	The function is called by REP_StoreResource

	Input: pContext (MUST be pREPCONTEXT), ContentId, and hash value
	Output: -
========================================================================*/
void REP_AddResourceToContext (void* pContext, UINT32 iContentId, 
							   UINT32 iHashValue)
{
	UINT32 *piTemp=NULL;
	UINT16 iNbrRes=0;
	UINT16 iCount=0;
	BOOL fError=FALSE;

	/* Get number of resources */
	iNbrRes=((REPCONTEXT*)pContext)->iNumberOfResources;

	/* Create new content id list */
	piTemp=NEWARRAY(UINT32,iNbrRes+1);

	/* Copy old list to new, also check if content id already in list */
	while (iCount<iNbrRes)
	{
		if (((REPCONTEXT*)pContext)->piResourceList[iCount]!=iContentId)
		{
			piTemp[iCount]=((REPCONTEXT*)pContext)->piResourceList[iCount];
		}
		else
		{
			fError=TRUE;
		}

		iCount++;
	}

	if (!fError)
	{
		/* Dealloc old list and store new list */
		DEALLOC(&((REPCONTEXT*)pContext)->piResourceList);
		((REPCONTEXT*)pContext)->piResourceList=piTemp;

		/* Create new hash list */
		piTemp=NEWARRAY(UINT32,iNbrRes+1);

		iCount=0;

		/* Copy hash values */
		while (iCount<iNbrRes)
		{
			piTemp[iCount]=((REPCONTEXT*)pContext)->piHashList[iCount];
			iCount++;
		}

		/* Dealloc old list and store new list */
		DEALLOC(&((REPCONTEXT*)pContext)->piHashList);
		((REPCONTEXT*)pContext)->piHashList=piTemp;

		/* Store new content id and hash value */
		((REPCONTEXT*)pContext)->piResourceList[iNbrRes]=iContentId;
		((REPCONTEXT*)pContext)->piHashList[iNbrRes]=iHashValue;

		/* Increase resource counter in context */
		((REPCONTEXT*)pContext)->iNumberOfResources++;
	}
	else
	{
		/* Error - delete new list */
		DEALLOC(&piTemp);
	}
}


/*========================================================================
	REP_RemoveModifyList
==========================================================================
	The function removes the modify list in the context.

	The function is called by REP_UpdateRootBlock

	Input: pContext (MUST be pREPCONTEXT). 
	Output: -
========================================================================*/
void REP_RemoveModifyList (void* pContext)
{
	pMODIFYROOT pTemp=NULL;
	pMODIFYROOT pRemove=NULL;

	pTemp=((REPCONTEXT*)pContext)->pModRoot;

	while (pTemp!=NULL)
	{
		pRemove=pTemp;
		pTemp=pTemp->pNext;
		DEALLOC(&pRemove);
	}

	((REPCONTEXT*)pContext)->pModRoot=NULL;
}


/*========================================================================
	REP_AddToModifyRoot
==========================================================================
	The function creates a new tMODIFYROOT-element and sets the fields
	to the inputted values. If the element is not in the pModResources 
	list in the context it is added. If it already exists, the element in
	the list is updated according to the repository.

	The function is called by REP_DeleteChannel, REP_GetResourseWithUrl, 
	REP_StoreResource, REP_ChangeResourceStatus, and REP_ActivateChannel.

	Input: pContext (MUST be pREPCONTEXT), ContentId, Action, and 
		   Hash Value (only if action RESOURCE_ADD)
	Output: -
========================================================================*/
void REP_AddToModifyRoot (void* pContext, UINT32 iContentId,
							 UINT8 iAction, UINT32 iHashValue)
{
	BYTE bData;
	BOOL fAdd=TRUE;
	pMODIFYROOT pNew=NULL;
	pMODIFYROOT pTemp=NULL;

	/* Check if list not null */
	if (((REPCONTEXT*)pContext)->pModRoot!=NULL)
	{
		pTemp=((REPCONTEXT*)pContext)->pModRoot;

		/* Check if already in list */
		while ((pTemp!=NULL)&&(fAdd))
		{
			if (pTemp->iContentId==iContentId)
			{
				/* Found, do not add new */
				fAdd=FALSE;
			}
			else
			{
				/* Get next element in list */
				pTemp=pTemp->pNext;
			}
		}
	}

	if (fAdd)
	{
		/* Create new element */
		pNew=NEWSTRUCT(MODIFYROOT);
		pNew->iAction=iAction;
		pNew->iContentId=iContentId;
		pNew->iHashValue=iHashValue;
		pNew->pNext=((REPCONTEXT*)pContext)->pModRoot;

		/* Add to list */
		((REPCONTEXT*)pContext)->pModRoot=pNew;
	}
	else
	{
		/* Change element in list */
		if (pTemp->iAction!=iAction)
		{
			/* Check if content exists */
			if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,
											iContentId,0,1,&bData))
			{
				if (bData==CONTENTTYPE_RESOURCE)
				{
					pTemp->iAction=RESOURCE_ADD;
				}
				else if (bData==CONTENTTYPE_CHANNEL)
				{
					pTemp->iAction=CHANNEL_ADD;
				}
			}
			else
			{
				if ((iAction==RESOURCE_REMOVE)||(iAction==RESOURCE_ADD))
				{
					pTemp->iAction=RESOURCE_REMOVE;
				}
				else if ((iAction==CHANNEL_REMOVE)||(iAction==CHANNEL_ADD))
				{
					pTemp->iAction=CHANNEL_REMOVE;
				}
			}
		}
	}
}


/*========================================================================
	REP_CountResourcesDiff
==========================================================================
	The function returns the maximal sum of the resources in the 
	tMODIFYROOT-element list. Action *RESOURCE_ADD* increases the sum 
	with one (*RESOURCE_REMOVE* has no effect).
	
	The function is called by REP_UpdateRootBlock

	Input: pContext (MUST be pREPCONTEXT). 
	Output: UINT32
========================================================================*/
UINT32 REP_CountResourcesDiff (void* pContext)
{
	INT32 iNbr=0;
	pMODIFYROOT pTemp=NULL;

	pTemp=((REPCONTEXT*)pContext)->pModRoot;

	while (pTemp!=NULL)
	{
		/* Check action */
		if (pTemp->iAction==RESOURCE_ADD)
		{
			iNbr++;
		}

		pTemp=pTemp->pNext;
	}

	return iNbr;
}


/*========================================================================
	REP_CountChannelDiff
==========================================================================
	The function returns the maximal sum of the channels in the 
	tMODIFYROOT-element list. Action *CHANNEL_ADD* increases the sum with 
	one (*CHANNEL_REMOVE* has no effect).

	The function is called by REP_UpdateRootBlock

	Input: pContext (MUST be pREPCONTEXT). 
	Output: UINT32
========================================================================*/
UINT32 REP_CountChannelDiff (void* pContext)
{
	INT32 iNbr=0;
	pMODIFYROOT pTemp=NULL;

	pTemp=((REPCONTEXT*)pContext)->pModRoot;

	while (pTemp!=NULL)
	{
		/* Check action */
		if (pTemp->iAction==CHANNEL_ADD)
		{
			iNbr++;
		}

		pTemp=pTemp->pNext;
	}

	return iNbr;
}


/*========================================================================
	REP_UpdateRootBlockChannels
==========================================================================
	The function updates merges the updated root blocks (created by 
	REP_UpdateRootBlockChannels and REP_UpdateRootBlockResources) and 
	returns a new root block. The length (piNewSize is also updated 
	accordingly).

	The function is called by REP_UpdateRootBlock

	Input: RootBlks, pointer to new size, and ResourceDiff
	Output: New root block, size is stored in piNewSize
========================================================================*/
BYTE* REP_MergeUpdateRootBlock (BYTE* pbChRoot, BYTE* pbResRoot, 
								UINT32* piNewSize)
{
	BYTE* pbRootBlk=NULL;
	UINT32 iSize=0;
	UINT32 iTempId=0;
	UINT32 iTempHash=0;
	UINT16 iNbrCh=0;
	UINT16 iNbrRes=0;
	UINT16 iCount=0;
	UINT16 iWrite=0;

	/* Calculate new total size */
	B_COPYSTRINGN(&iNbrCh,pbChRoot+1,2);
	B_COPYSTRINGN(&iNbrRes,pbResRoot+3,2);

	iSize=5+(iNbrCh*4)+(iNbrRes*8);

	/* Create new root block */
	pbRootBlk=NEWARRAY(BYTE,iSize);

	/* Copy channel parts into new block */
	while (iCount<iNbrCh)
	{
		/* Get next channel id */
		B_COPYSTRINGN(&iTempId,pbChRoot+5+(iCount*4),4);

		if (iTempId!=0)
		{
			/* Store channel id */
			B_COPYSTRINGN(pbRootBlk+5+(iWrite*4),&iTempId,4);
			iWrite++;
		}
		iCount++;
	}

	/* Store nbr of channels */
	B_COPYSTRINGN(pbRootBlk+1,&iNbrCh,2);

	/* Find the start position of the resources in the pbResRoot string */
	B_COPYSTRINGN(&iNbrCh,pbResRoot+1,2);
	iCount=0;
	
	/* Copy resource parts into new block */
	while (iCount<iNbrRes)
	{
		/* Get next resource id */ 
		B_COPYSTRINGN(&iTempId,pbResRoot+5+((iCount+iNbrCh)*4),4);

		if (iTempId!=0)
		{
			/* Get hash */
			B_COPYSTRINGN(&iTempHash,pbResRoot+5+(iNbrRes*4)+((iCount+iNbrCh)*4),4);

			/* Store resource id and hash value */
			B_COPYSTRINGN(pbRootBlk+5+(iWrite*4),&iTempId,4);
			B_COPYSTRINGN(pbRootBlk+5+((iNbrRes+iWrite)*4),&iTempHash,4);
			iWrite++;
		}
		iCount++;
	}

	/* Store nbr of resources */
	B_COPYSTRINGN(pbRootBlk+3,&iNbrRes,2);

	/* Store status "Processing" */
	pbRootBlk[0]=REPROOT_STATUS_PROCESSING;

	/* Update size and return */
	*piNewSize=iSize;
	return pbRootBlk;
}


/*========================================================================
	REP_UpdateRootBlockChannels
==========================================================================
	The function updates the channels in the root block according to the 
	tMODIFYROOT-elements in the context. The input data (pbRootBlk) 
	contains the old root block, but is large enough to include all
	changes.

	The function is called by REP_UpdateRootBlock

	Input: pContext (MUST be pREPCONTEXT), RootBlk
	Output: -
========================================================================*/
void REP_UpdateRootBlockChannels (void* pContext, BYTE* pbRootBlk)
{
	UINT16 iNbrCh=0;
	UINT32 iCount=0;
	UINT32 iTempId=0;
	UINT32 iZero=0;
	BOOL fFound=FALSE;
	pMODIFYROOT pTemp=NULL;

	pTemp=((REPCONTEXT*)pContext)->pModRoot;
	B_COPYSTRINGN(&iNbrCh,pbRootBlk+1,2);

	/* Step through the channels in the Modify List */
	while (pTemp!=NULL)
	{
		if (pTemp->iAction==CHANNEL_ADD)
		{
			/* Add channel id to root block */
			iCount=0;
			fFound=FALSE;

			/* Find id 0 */
			while ((iCount<iNbrCh)&&(!fFound))
			{
				/* Get id */
				B_COPYSTRINGN(&iTempId,pbRootBlk+5+(iCount*4),4);

				if (iTempId==0)
				{
					/* Space found - use this index */
					fFound=TRUE;
				}
				else
				{
					iCount++;
				}
			}

			/* Store id */
			B_COPYSTRINGN(pbRootBlk+5+(iCount*4),&(pTemp->iContentId),4);
			
			if (!fFound)
			{
				iNbrCh++;
			}
		}
		else if (pTemp->iAction==CHANNEL_REMOVE)
		{
			iCount=0;

			/* Remove channel id from root block (replace with 0) */
			while (iCount<iNbrCh)
			{
				/* Get id */
				B_COPYSTRINGN(&iTempId,pbRootBlk+5+(iCount*4),4);

				if (iTempId==pTemp->iContentId)
				{
					/* Found - remove */
					B_COPYSTRINGN(pbRootBlk+5+(iCount*4),&iZero,4);
					iNbrCh--;

					/* Exit loop */
					iCount=iNbrCh;
				}

				iCount++;
			}
		}

		pTemp=pTemp->pNext;
	}

	/* Store nbr of channels */
	B_COPYSTRINGN(pbRootBlk+1,&iNbrCh,2);
}


/*========================================================================
	REP_UpdateRootBlockResources
==========================================================================
	The function updates the resources in the root block according to the 
	tMODIFYROOT-elements in the context. The input data (pbRootBlk) 
	contains the old root block, but is large enough to include all
	changes. 

	The resources are stored in the root block on the resource location
	based on the OLD number of channels.

	The iResourceDiff is needed to keep track of where the hash values 
	are stored in the root block with space added for all changes in the 
	resources. The function starts by moving the Hash values iResourceDiff*4 
	BYTES to the end of the root block.

	The function is called by REP_UpdateRootBlock

	Input: pContext (MUST be pREPCONTEXT), new root block, and ResourceDiff
	Output: -
========================================================================*/
void REP_UpdateRootBlockResources (void* pContext, BYTE* pbRootBlk, 
								   UINT32 iResourceDiff)
{
	UINT16 iNbrRes=0;
	UINT16 iNbrCh=0;
	UINT32 iCount=0;
	UINT32 iTempId=0;
	UINT32 iZero=0;
	UINT32 iHashOffset=0;
	BOOL fFound=FALSE;
	pMODIFYROOT pTemp=NULL;

	pTemp=((REPCONTEXT*)pContext)->pModRoot;
	B_COPYSTRINGN(&iNbrCh,pbRootBlk+1,2);
	B_COPYSTRINGN(&iNbrRes,pbRootBlk+3,2);

	/* Hash offset - the hash values start position in the root block 
	   with space added for all resource changes */
	iHashOffset=5+(4*iNbrCh)+(4*iNbrRes)+(4*iResourceDiff);
		
	/* Move the hash values to the end of the root block */
	memmove(pbRootBlk+iHashOffset,pbRootBlk+5+(4*iNbrCh)+
								(4*iNbrRes),(4*iNbrRes)*sizeof(BYTE));

	/* Step through the resources in the Modify List */
	while (pTemp!=NULL)
	{
		if (pTemp->iAction==RESOURCE_ADD)
		{
			/* Add resource id and hash value to root block */
			iCount=0;
			fFound=FALSE;

			/* Find id 0 */
			while ((iCount<iNbrRes)&&(!fFound))
			{
				/* Get id */
				B_COPYSTRINGN(&iTempId,pbRootBlk+5+(iNbrCh*4)+(iCount*4),4);

				if (iTempId==0)
				{
					/* Space found - use this index */
					fFound=TRUE;
				}
				else
				{
					iCount++;
				}
			}

			/* Store id and hash */
			B_COPYSTRINGN(pbRootBlk+5+(iNbrCh*4)+(iCount*4),
											&(pTemp->iContentId),4);
			B_COPYSTRINGN(pbRootBlk+iHashOffset+(iCount*4),
											&(pTemp->iHashValue),4);
			
			if (!fFound)
			{
				iNbrRes++;
			}
		}
		else if (pTemp->iAction==RESOURCE_REMOVE)
		{
			iCount=0;

			/* Remove channel id from root block (replace with 0) */
			while (iCount<iNbrRes)
			{
				/* Get id */
				B_COPYSTRINGN(&iTempId,pbRootBlk+5+(iNbrCh*4)+(iCount*4),4);

				if (iTempId==pTemp->iContentId)
				{
					/* Found - remove */
					B_COPYSTRINGN(pbRootBlk+iHashOffset+(iCount*4),&iZero,4);
					iNbrRes--;

					/* Exit loop */
					iCount=iNbrRes;
				}
				iCount++;
			}
		}

		pTemp=pTemp->pNext;
	}

	/* Store nbr of channels */
	B_COPYSTRINGN(pbRootBlk+3,&iNbrRes,2);
}


/*========================================================================
	REP_UpdateRootBlock
==========================================================================
	The function updates the root block according to the tMODIFYROOT-
	elements in the context. If the function fails, the root block is
	deleted (or set to *corrupt*), otherwise it is stored with status 
	*OK*.

	Note! The tMODIFYROOT-element list is deleted from memory.

	The function is called by REP_RootBlockSafe.

	Input: pContext (MUST be pREPCONTEXT). 
	Output: TRUE/FALSE
========================================================================*/
BOOL REP_UpdateRootBlock (void* pContext)
{
	UINT32 iRootId=REP_ROOTBLK_ID;
	UINT16 iNbrRes=0;
	UINT16 iNbrCh=0;
	UINT32 iSize=0;
	UINT32 iTempSize=0;
	UINT32 iNewSize;
	INT32 iResourceDiff=0;
	BYTE* pbRoot=NULL;
	BYTE* pbChRoot=NULL;
	BYTE* pbResRoot=NULL;

	/* Get size */
	iSize=Storage_GetBlockSize(&((REPCONTEXT*)pContext)->Storage,
														REP_ROOTBLK_ID);

	if (iSize>0)
	{
		/* Allocate memory */
		pbRoot=NEWARRAY(BYTE,iSize);

		#ifdef WTA_DEBUG
			xTraceAlg("Root block found");
		#endif

		/* Get root block data */
		if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,
										REP_ROOTBLK_ID,0,iSize,pbRoot))
		{
			#ifdef WTA_DEBUG
				xTraceAlg("Root block data successfully retrieved");
			#endif

			#ifdef WTA_DEBUG
				if (pbRoot==NULL)
				{
					xTraceAlg("Root block NULL");
				}
				else
				{
					xTraceAlg("Root block not NULL");
				}
			#endif


			/* Found. Calculate new size. */
			B_COPYSTRINGN(&iNbrCh,pbRoot+1,2);
			B_COPYSTRINGN(&iNbrRes,pbRoot+3,2);

			#ifdef WTA_DEBUG
				xTraceAlg("Root block data copied");
			#endif

			iResourceDiff=REP_CountResourcesDiff(pContext);

			#ifdef WTA_DEBUG
				xTraceAlg("Root Block resources Diff calculated");
			#endif

			iNewSize=5+(iNbrCh*4)+(iNbrRes*8)+(8*iResourceDiff)+
										(4*REP_CountChannelDiff(pContext));

			if (iNewSize>0)
			{

				/* Create new data strings */
				pbChRoot=NEWARRAY(BYTE,iNewSize);
				pbResRoot=NEWARRAY(BYTE,iNewSize);

				/* Copy old string. Since the iSize is the size of the 
				   root block rounded up to the nearest 8th byte, the 
				   iNewSize is used if iSize is bigger than iNewSize. */
				if (iSize>iNewSize)
				{
					iTempSize=iNewSize;
				}
				else
				{
					iTempSize=iSize;
				}

				B_COPYSTRINGN(pbChRoot,pbRoot,iTempSize);
				B_COPYSTRINGN(pbResRoot,pbRoot,iTempSize);

				DEALLOC(&pbRoot);

				#ifdef WTA_DEBUG
					xTraceAlg("Root block: Updating to be started");
				#endif

				/* Update channels according to Modify List */
				REP_UpdateRootBlockChannels (pContext,pbChRoot);

				#ifdef WTA_DEBUG
					xTraceAlg("Root block: channels added");
				#endif

				/* Update resources according to Modify List */
				REP_UpdateRootBlockResources(pContext,pbResRoot,iResourceDiff);

				#ifdef WTA_DEBUG
					xTraceAlg("Root block: resources added");
				#endif

				/* Merge lists AND CHANGE iNewSize */
				pbRoot=REP_MergeUpdateRootBlock (pbChRoot,pbResRoot,&iNewSize);

				#ifdef WTA_DEBUG
					xTraceAlg("Root block: Lists merged");
				#endif

				/* Remove modify list */
				REP_RemoveModifyList (pContext);

				#ifdef WTA_DEBUG
					xTraceAlg("Root block: Modifylist removed");
				#endif

				/* Remove temporary headers */
				DEALLOC(&pbChRoot);
				DEALLOC(&pbResRoot);

				if (iNewSize>iSize)
				{
					#ifdef WTA_DEBUG
						xTraceAlg("Root block: Store on new location");
					#endif

					/* Remove old root block */
					Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,
																REP_ROOTBLK_ID);

					#ifdef WTA_DEBUG
						xTraceAlg("Root block: Old block removed");
					#endif

					/* Store new root block */
					if (Storage_AllocateBlock(&((REPCONTEXT*)pContext)
													->Storage,iNewSize,&iRootId))
					{
						if (Storage_Put(&((REPCONTEXT*)pContext)->Storage,
											REP_ROOTBLK_ID,0,iNewSize,pbRoot))
						{
							/* Root block stored successfully */
							DEALLOC(&pbRoot);
							return TRUE;												
						}
					}
				}
				else
				{
					#ifdef WTA_DEBUG
						xTraceAlg("Root block: Store on same location");
					#endif

					/* Store on same location */
					if (Storage_Put(&((REPCONTEXT*)pContext)->Storage,
										REP_ROOTBLK_ID,0,iNewSize,pbRoot))
					{
						DEALLOC(&pbRoot);
						return TRUE;
					}
				}
			}
		}
	}

	/* Error in updating. Try to remove root block instead. */
	Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,REP_ROOTBLK_ID);

	/* Remove modify list */
	REP_RemoveModifyList (pContext);

	/* Remove root block */
	DEALLOC(&pbRoot);

	return FALSE;
}


/*========================================================================
	REP_ChangeRootBlockStatus
==========================================================================
	The function changes the status of the root block according to the
	inputted value. The status is not changed if the root block is 
	corrupt (REPROOT_STATUS_CORRUPT). If the status could not be changed
	the function tries to remove the root block from the repository.

	The function is called by REP_RootBlockSafe and REP_GetAllChannels
	  
	Input: pContext (MUST be pREPCONTEXT), new status
	Output: -
==========================================================================*/
void REP_ChangeRootBlockStatus (void* pContext, UINT8 iStatus)
{
	BYTE abRoot[1];

	/* Get root block */
	if (Storage_Get(&((REPCONTEXT*)pContext)->
									Storage,REP_ROOTBLK_ID,0,1,abRoot))
	{
		/* Root block found. Change status if necessary. */
		if ((abRoot[0]!=iStatus)&&(abRoot[0]!=REPROOT_STATUS_CORRUPT))
		{
			abRoot[0]=iStatus;

			/* Store status */
			Storage_Put(&((REPCONTEXT*)pContext)->
									Storage,REP_ROOTBLK_ID,0,1,abRoot);
			return;
		}
	}

	/* Try to remove root block. */
	Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,REP_ROOTBLK_ID);
}


/*========================================================================
	REP_StoreRootBlock
==========================================================================
	The function creates a root block based on the data in the RepContext
	The old root block is thereafter removed and replaced with the new 
	one. The status is set to *OK*. If anything goes wrong the root block 
	is deleted (or status set to *corrupt*).

	Note! The tMODIFYROOT-element list is NOT included.

  	The function is called by REP_ScanRepository

	Input: pContext (MUST be pREPCONTEXT)
	Output: TRUE if OK, FALSE if error
========================================================================*/
BOOL REP_StoreRootBlock (void* pContext)
{
	BYTE* pbData=NULL;
	UINT16 iNbrCh=0;
	UINT16 iNbrRes=0;
	UINT32 iSize=0;
	UINT32 iRootBlkId=REP_ROOTBLK_ID;

	/* Calculate size of root block */
	iNbrCh=(((REPCONTEXT*)pContext)->iNumberOfElements);
	iNbrRes=(((REPCONTEXT*)pContext)->iNumberOfResources);
	iSize=5+(4*iNbrCh)+(8*iNbrRes);

	/* Create data */
	pbData=NEWARRAY(BYTE,iSize);

	/* Status */
	pbData[0]=REPROOT_STATUS_OK;
	
	/* Number of channels */
	B_COPYSTRINGN(pbData+1,&iNbrCh,2);

	/* Number of resources */
	B_COPYSTRINGN(pbData+3,&iNbrRes,2);

	/* Store channel id:s */
	B_COPYSTRINGN(pbData+5,((REPCONTEXT*)pContext)->
										piElementList,(iNbrCh*4));

	/* Store resource id:s */
	B_COPYSTRINGN(pbData+5+(iNbrCh*4),((REPCONTEXT*)pContext)->
											piResourceList,(iNbrRes*4));

	/* Store resource hash values */
	B_COPYSTRINGN(pbData+5+(iNbrCh*4)+(iNbrRes*4),
				((REPCONTEXT*)pContext)->piHashList,(iNbrRes*4));

	/* Delete old root block */
	Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,REP_ROOTBLK_ID);

	/* Store new root block */
	if (Storage_AllocateBlock(&((REPCONTEXT*)pContext)->Storage,
														iSize,&iRootBlkId))
	{
		/* Store data block */
		if (!Storage_Put(&((REPCONTEXT*)pContext)->Storage,
											REP_ROOTBLK_ID,0,iSize,pbData))
		{
			/* Error - remove block */
			Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,
															REP_ROOTBLK_ID);
		}

		/* Dealloc data */
		DEALLOC(&pbData);

		return TRUE;
	}

	/* Dealloc data */
	DEALLOC(&pbData);

	return FALSE;
}






/*========================================================================
	HELP FUNCTIONS FOR RESOURCES AND CHANNELS
=========================================================================*/

/*========================================================================
	REP_GetResourceUrl
==========================================================================
	The function returns the url corresponding to the the resource id.
	If error NULL is returned. It is the caller's responsibility to 
	deallocate the memory.

	The function is called by REP_CompareUrl and REP_CalculateHashValues

	Input: pContext (MUST be pREPCONTEXT) and Resource Id
	Output: Url, NULL if error.
========================================================================*/
BYTE* REP_GetResourceUrl (void* pContext, UINT32 iResourceId)
{
	BYTE* pbUrl=NULL;
	BYTE abData[6];
	UINT16 iLen=0;

	/* Get resource header */
	if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,iResourceId,0,6,abData))
	{
		/* OK - get length of url */
		B_COPYSTRINGN(&(iLen),abData+4,2);

		if (iLen!=0)
		{
			/* Allocate memory for url and termination char */
			pbUrl=NEWARRAY(BYTE,iLen+1);

			/* Get url */
			if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,
													iResourceId,6,iLen,pbUrl))
			{
				/* Set termination char */
				pbUrl[iLen]=0;
			}
			else
			{
				/* Error */
				DEALLOC(&pbUrl);
			}
		}
	}

	return pbUrl;
}


/*========================================================================
	REP_CompareUrl
==========================================================================
	The function compares the inputted url with the url of the resource
	stored at the inputted resource id. If the urls are equivalent, TRUE
	is returned, otherwise FALSE is returned.

	The function is called by REP_GetResourceWithUrl

	Input: pContext (MUST be pREPCONTEXT) url, and ResourceId
	Output: TRUE/FALSE
==========================================================================*/
BOOL REP_CompareUrl (void* pContext, BYTE* pbUrl, UINT32 iResourceId)
{
	BOOL fResult=FALSE;
	BYTE* pbTemp=NULL;

	/* Get stored resource url */
	pbTemp=REP_GetResourceUrl(pContext,iResourceId);

	if (pbTemp!=NULL)
	{
		/* Compare url:s */
		fResult=b_EqualURL(pbUrl,pbTemp,ALL_COMP);

		/* Dealloc temporary memory */
		DEALLOC(&pbTemp);
	}

	return fResult;
}


/*========================================================================
	REP_GetStoredChannel
==========================================================================
	The function extracts data from the instream and returns the channel 
	part of a REPCONTENTSTRUCT. The instream must be correct. It is the
	caller's responsibility to deallocate the returned data

	The function is called by REP_GetContentWithId

	Input: Data
	Output: Pointer to CHCONTENTSTRUCT
==========================================================================*/
pCHCONTENTSTRUCT REP_GetStoredChannel (BYTE* pbData)
{
	pCHCONTENTSTRUCT pChannel=NULL;
	UINT16 iLen=0;
	UINT8 iLen8=0;
	UINT32 iOffset=0;

	#ifdef WTA_DEBUG
		xTraceAlg("Inside REP_GetStoredChannel");
	#endif

	/* Create struct */
	pChannel=NEWSTRUCT(CHCONTENTSTRUCT);
	pChannel->iStatus=0;
	pChannel->iExpiryDate=0;
	pChannel->pwchTitle=NULL;
	pChannel->pwchAbstract=NULL;
	pChannel->pwchEventId=NULL;
	pChannel->piAllResInChList=NULL;
	pChannel->iResCounter=0;

	/* Store in struct */
	pChannel->iStatus=pbData[1];

	/* Copy User Accessible */
	if (pbData[6]==0x00)
	{
		pChannel->useraccess=FALSE;
	}
	else
	{
		pChannel->useraccess=TRUE;
	}

	/* Copy ServiceAccessible */
	if (pbData[7]==0x00)
	{
		pChannel->bServiceAccessible=FALSE;
	}
	else
	{
		pChannel->bServiceAccessible=TRUE;
	}
	
	/* Copy expiry date */
	B_COPYSTRINGN(&(pChannel->iExpiryDate),pbData+2,4);
	iOffset+=8;
	
	/* Get length of title */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);
	iOffset+=2;
	
	/* Copy title */
	if (iLen!=0)
	{
		pChannel->pwchTitle=NEWARRAY(WCHAR,iLen+1);
		B_COPYSTRINGN(pChannel->pwchTitle,pbData+iOffset,iLen*2);
		iOffset+=iLen*2;

		/* Set termination char */
		pChannel->pwchTitle[iLen]=0;
	}
	else
	{
		pChannel->pwchTitle=NULL;
	}

	/* Get length of abstract */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);
	iOffset+=2;

	if (iLen!=0)
	{
		/* Copy abstract */
		pChannel->pwchAbstract=NEWARRAY(WCHAR,iLen+1);
		B_COPYSTRINGN(pChannel->pwchAbstract,pbData+iOffset,iLen*2);
		iOffset+=iLen*2;
		
		/* Set termination char */
		pChannel->pwchAbstract[iLen]=0;
	}
	else
	{
		pChannel->pwchAbstract=NULL;
	}

	#ifdef WTA_DEBUG
		xTraceAlg("REP_GetStoredChannel - after abstract");
	#endif
		
	/* Get length of eventid */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);
	iOffset+=2;

	#ifdef WTA_DEBUG
	{
		char stTemp[10];

		URL_DEBUG_PRINT("Event id len : ");
		_itoa( iLen, stTemp, 10 );
		URL_DEBUG_PRINT(stTemp);
	}
	#endif

	if (iLen!=0)
	{
		/* Copy eventid */
		pChannel->pwchEventId=NEWARRAY(WCHAR,iLen+1);
		B_COPYSTRINGN(pChannel->pwchEventId,pbData+iOffset,iLen*2);
		iOffset+=iLen*2;
		
		/* Set termination char */
		pChannel->pwchEventId[iLen]=0;
	}
	else
	{
		pChannel->pwchEventId=NULL;
	}

	/* Get length of channelid */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);
	iOffset+=2;

	if (iLen!=0)
	{
		/* Copy channelid */
		pChannel->pwchchannelid=NEWARRAY(WCHAR,iLen+1);
		B_COPYSTRINGN(pChannel->pwchchannelid,pbData+iOffset,iLen*2);
		iOffset+=iLen*2;
		
		/* Set termination char */
		pChannel->pwchchannelid[iLen]=0;
	}
	else
	{
		pChannel->pwchchannelid=NULL;
	}

	#ifdef WTA_DEBUG
		xTraceAlg("REP_GetStoredChannel - Getting Res List Len");
	#endif

	/* Get length of ResList */
	B_COPYSTRINGN(&(iLen8),pbData+iOffset,1);
	pChannel->iResCounter=iLen8;
	iOffset+=1;

	#ifdef WTA_DEBUG
		xTraceAlg("REP_GetStoredChannel - Getting Res List");
	#endif

	/* Copy ResList */
	pChannel->piAllResInChList=NEWARRAY(UINT32,(iLen8*4));
	B_COPYSTRINGN(pChannel->piAllResInChList,pbData+iOffset,(iLen8*4));

	#ifdef WTA_DEBUG
		xTraceAlg("REP_GetStoredChannel - Exiting");
	#endif

	return pChannel;
}


/*========================================================================
	REP_GetStoredResource
==========================================================================
	The function extracts data from the instream and returns the resource
	part of a REPCONTENTSTRUCT. The instream must be correct. It is the
	caller's responsibility to deallocate the returned data. The function
	returns NULL if the WSP_PreParseHeaders returns NULL or sets the 
	error flag.

	The function is called by REP_GetContentWithId.

	Input: Data
	Output: Pointer to RESCONTENTSTRUCT
==========================================================================*/
pRESCONTENTSTRUCT REP_GetStoredResource (BYTE* pbData)
{
	pRESCONTENTSTRUCT pResource=NULL;
	UINT32 iOffset=0;
	UINT16 iLen=0;
	UINT32 iLen32=0;
	BOOL fError=FALSE;

	pResource=NEWSTRUCT(RESCONTENTSTRUCT);

	pResource->iStatus=(UINT8)(pbData[1]);
	pResource->iRefCounter=(UINT8)(pbData[2]);
	pResource->iInstallCounter=(UINT8)(pbData[3]);

	iOffset=4;

	/* Get length of url and allocate memory */
	B_COPYSTRINGN(&(iLen),pbData+iOffset,2);
	pResource->pbUrl=NEWARRAY(BYTE,iLen+1);
	iOffset+=2;

	/* Copy url */
	B_COPYSTRINGN(pResource->pbUrl,pbData+iOffset,iLen);
	iOffset+=iLen;

	/* Set termination char */
	pResource->pbUrl[iLen]=0;

	/* Get length of header and allocate memory */
	B_COPYSTRINGN(&(iLen32),pbData+iOffset,4);
	iOffset+=4;

	/* Check scheme. If not FILE, do */

	/* Preparse header and store in resource */
	pResource->pHeaderHandle=
				WSP_PreParseHeaders(pbData+iOffset,iLen32,&fError);

	if ((fError)||(pResource->pHeaderHandle==NULL))
	{
		/* Error in pre-parse - remove all data and return NULL */
		WSP_EmptyHeaderList(pResource->pHeaderHandle);
		pResource->pHeaderHandle=NULL;

		DEALLOC(&(pResource->pbUrl));
		DEALLOC(&pResource);

		/* Fatal error. All channels using this resource MUST be 
		   deleted - or the resource MUST be updated. */

		/* ... */

		return NULL;
	}

	/* If FILE scheme, do */

	/* ... */

	/* Step past header */
	iOffset+=iLen32;
	
	/* Get length of data and allocate memory */
	B_COPYSTRINGN(&(iLen32),pbData+iOffset,4);
	pResource->iBodyLength=iLen32;
	pResource->pbBodyData=NEWARRAY(BYTE,iLen32);
	iOffset+=4;

	/* Copy data */
	B_COPYSTRINGN(pResource->pbBodyData,pbData+iOffset,iLen32);

	return pResource;
}


/*========================================================================
	REP_ChangeResourceStatus
==========================================================================
	The function changes the RefCounter and the InstallCounter of the
	specified resource. The counter are modified with the values in the
	iInstMod and iRefMod parameters. If the change of counters affects the
	status of the resource, it is changes accordingly. If both the Ref-
	Counter and the InstallCounter reaches 0, the resource is removed.

	The function also creates a tMODIFYROOT-element if the change of
	the resource also affects the root block (i.e., if the resource is
	removed from the repository, it is also removed from the root block.)

	The function is called by REP_PutChannelInSDLList, REP_GetResourse-
	WithUrl, REP_DecreaseInstallCounter, REP_InstallChannel and 
	REP_ActivateChannel

	Input: pContext (MUST be pREPCONTEXT) ContentId, iInstMod, iRefMod
	Output: TRUE if OK, FALSE if error
==========================================================================*/
BOOL REP_ChangeResourceStatus (void* pContext, UINT32 iContentId, 
							  INT8 iInstMod, INT8 iRefMod)
{
	BYTE abData[4];
	BOOL fRemove=FALSE;

	/* Find resource */
	if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,iContentId,0,4,abData))
	{
		if (abData[0]==CONTENTTYPE_RESOURCE)
		{
			/* Change counters */
			abData[2]=(UINT8)(abData[2]+iRefMod);
			abData[3]=(UINT8)(abData[3]+iInstMod);

			/* Change status */
			if ((abData[3]==0)&&(abData[2]!=0))
			{
				/* Activate resource */
				abData[1]=CONTENT_ACTIVE;
			}
			else if (abData[3]!=0)
			{
				/* Status installing */
				abData[1]=CONTENT_INSTALLING;
			}
			else if ((abData[3]==0)&&(abData[2]==0))
			{
				/* Remove resource */
				fRemove=TRUE;
			}

			if (fRemove)
			{
				/* Remove from repository */
				Storage_DeleteBlock (&((REPCONTEXT*)pContext)->Storage, 
																iContentId);

				/* Add resource to tMODIFYROOT list in context with
				   action RESOURCE_REMOVE */
				REP_AddToModifyRoot (pContext,iContentId,RESOURCE_REMOVE,0);

				return TRUE;
			}
			else
			{
				/* Update resource */
				if (Storage_Put(&((REPCONTEXT*)pContext)->Storage,
												iContentId,0,4,abData))
				{
					/* OK */
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


/*========================================================================
	REP_CreateResourceData
==========================================================================
	The function created the data for a resource to be stored in the
	repository.

	Called by REP_StoreResource and REP_UpdateResource

	Input: pContext (MUST be pREPCONTEXT) ContentId, iInstMod, iRefMod
	Output: TRUE if OK, FALSE if error
==========================================================================*/
BYTE* REP_CreateResourceData (pREPCONTENTSTRUCT pResource, UINT32 iDataSize,
							  UINT32 iHeaderLen, UINT32 iUrlLen, UINT32 iDataLen)
{
	BYTE* pbData=NULL;

	/* Create new data array */
	pbData=NEWARRAY(BYTE,iDataSize);

	pbData[0]=CONTENTTYPE_RESOURCE;
	pbData[1]=CONTENT_INSTALLING;
	(pResource->content).pResContentStruct->iStatus=CONTENT_INSTALLING;

	/* Store no of referring channels and install counter */
	pbData[2]=(pResource->content).pResContentStruct->iRefCounter;
	pbData[3]=(pResource->content).pResContentStruct->iInstallCounter;

	/* Store length of url */
	B_COPYSTRINGN(pbData+4,&iUrlLen,2);
	
	/* Store url */
	B_COPYSTRINGN(pbData+6,
		(pResource->content).pResContentStruct->pbUrl,iUrlLen);

	/* Store header length */
	B_COPYSTRINGN(pbData+6+iUrlLen,&iHeaderLen,4);

	/* Store WSP headers */
	B_COPYSTRINGN(pbData+10+iUrlLen,(pResource->content).
		pResContentStruct->pHeaderHandle->pbData,iHeaderLen);

	/* Store data length */
	B_COPYSTRINGN(pbData+10+iUrlLen+iHeaderLen,&iDataLen,4);

	/* Store data */
	B_COPYSTRINGN(pbData+14+iUrlLen+iHeaderLen,
		(pResource->content).pResContentStruct->pbBodyData,iDataLen);

	return pbData;
}






/*========================================================================
	HELP FUNCTIONS FOR REP_ScanRepository
=========================================================================*/

/*========================================================================
	REP_RemoveAllUnrefResources
==========================================================================
	The function removes all non-'id 0' in the piResourceList from the 
	hash list and from the repository. The function should be called 
	until it returns FALSE. The function uses the iCurrentElement to keep 
	track of the next element to evaluate.

	The function is called by REP_ScanRepository

	Input: pContext (MUST be pREPCONTEXT)
	Output: TRUE if the function should be called again, FALSE otherwise.
==========================================================================*/
BOOL REP_RemoveAllUnrefResources (void* pContext)
{
	UINT16 iTemp=0;
	UINT32 iId=0;

	/* Find next element to remove */
	while (((REPCONTEXT*)pContext)->iCurrentElement <
							((REPCONTEXT*)pContext)->iNumberOfElements)
	{
		iTemp=((REPCONTEXT*)pContext)->iCurrentElement;

		if (((REPCONTEXT*)pContext)->piResourceList[iTemp]!=0)
		{
			/* Remove from repository */
			iId=((REPCONTEXT*)pContext)->piResourceList[iTemp];

			Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,iId);

			/* Remove from hash list */
			((REPCONTEXT*)pContext)->piHashList[iTemp]=0;
			
			((REPCONTEXT*)pContext)->iCurrentElement=(UINT16)(iTemp+1);
			
			/* Continue */
			return TRUE;
		}

		/* Check next */
		((REPCONTEXT*)pContext)->iCurrentElement=(UINT16)(iTemp+1);
	}

	/* End */
	return FALSE;
}


/*========================================================================
	REP_TrimChannelList
==========================================================================
	The function removes all 'id 0' from the piElementList and updates
	the iNumberOfElements accordingly.

	The function is called by REP_ScanRepository

	Input: pContext (MUST be pREPCONTEXT)
	Output: -
==========================================================================*/
void REP_TrimChannelList (void* pContext)
{
	UINT16 iNbrCh=0;
	UINT16 iCount=0;
	UINT16 iStoreIndex=0;
	UINT32 *piNewList=NULL;

	/* Count channels (not 0) */
	while (iCount<((REPCONTEXT*)pContext)->iNumberOfElements)
	{
		if (((REPCONTEXT*)pContext)->piElementList[iCount]!=0)
		{
			iNbrCh++;
		}
		iCount++;
	}

	/* Create new resource list */
	piNewList=NEWARRAY(UINT32,iNbrCh);
	iCount=0;

	/* Copy all 'not 0'-id:s  to new list */
	while (iCount<((REPCONTEXT*)pContext)->iNumberOfElements)
	{
		if (((REPCONTEXT*)pContext)->piElementList[iCount]!=0)
		{
			/* Store channel id */
			piNewList[iStoreIndex]=((REPCONTEXT*)pContext)
												->piElementList[iCount];
			iStoreIndex++;
		}
		iCount++;
	}

	/* Dealloc old list */
	DEALLOC(&((REPCONTEXT*)pContext)->piElementList);

	/* Store new list and element counter */
	((REPCONTEXT*)pContext)->piElementList=piNewList;
	((REPCONTEXT*)pContext)->iNumberOfElements=iNbrCh;
}


/*========================================================================
	REP_TrimResourceList
==========================================================================
	The function removes all 'id 0' from the piResourceList and updates
	the piResourceCounter accordingly.

	The function is called by REP_ScanRepository

	Input: pContext (MUST be pREPCONTEXT)
	Output: -
==========================================================================*/
void REP_TrimResourceList (void* pContext)
{
	UINT16 iNbrRes=0;
	UINT16 iCount=0;
	UINT16 iStoreIndex=0;
	UINT32 *piNewList=NULL;

	/* Count resources (not 0) */
	while (iCount<((REPCONTEXT*)pContext)->iNumberOfElements)
	{
		if (((REPCONTEXT*)pContext)->piResourceList[iCount]!=0)
		{
			iNbrRes++;
		}
		iCount++;
	}

	/* Create new resource list */
	piNewList=NEWARRAY(UINT32,iNbrRes);
	iCount=0;

	/* Copy all 'not 0'-id:s  to new list */
	while (iCount<((REPCONTEXT*)pContext)->iNumberOfElements)
	{
		if (((REPCONTEXT*)pContext)->piResourceList[iCount]!=0)
		{
			/* Store resource id */
			piNewList[iStoreIndex]=((REPCONTEXT*)pContext)
												->piResourceList[iCount];
			iStoreIndex++;
		}
		iCount++;
	}

	/* Dealloc old list */
	DEALLOC(&((REPCONTEXT*)pContext)->piResourceList);

	/* Store new list and resource counter */
	((REPCONTEXT*)pContext)->piResourceList=piNewList;
	((REPCONTEXT*)pContext)->iNumberOfResources=iNbrRes;
}



/*========================================================================
	REP_CalculateHashValues
==========================================================================
	The function calculates hash values for all resources in the 
	piResourceList. The values are stored in the piHashList. The function
	should be called repeatedly until it returns FALSE.

	Note! The piHashList is created in this function. The function uses
	the iCurrentElement to keep track of the next resource to calculate
	hash value for.

	The function is called by REP_ScanRepository

	Input: pContext (MUST be pREPCONTEXT)
	Output: TRUE if the function should be called again, FALSE otherwise.
==========================================================================*/
BOOL REP_CalculateHashValues (void* pContext)
{
	UINT32 iHashValue=0;
	UINT32 *piHashList=NULL;
	BYTE* pbUrl=NULL;

	/* Check if any resources */
	if (((REPCONTEXT*)pContext)->iNumberOfResources!=0)
	{	
		/* Check if first time */
		if (((REPCONTEXT*)pContext)->iCurrentElement==0)
		{
			/* Create hash list */
			piHashList=NEWARRAY(UINT32,((REPCONTEXT*)pContext)
													->iNumberOfResources);

			/* Store in context */
			((REPCONTEXT*)pContext)->piHashList=piHashList;
		}

		/* Calculate hash value and store */
		if (((REPCONTEXT*)pContext)->iCurrentElement<
								((REPCONTEXT*)pContext)->iNumberOfResources)
		{
			/* Get url */
			pbUrl=REP_GetResourceUrl (pContext,((REPCONTEXT*)pContext)->
						piResourceList[((REPCONTEXT*)pContext)->iCurrentElement]);

			if (pbUrl!=NULL)
			{
				/* Calculate Hash value */
				if (b_HashURL (pbUrl, &iHashValue))
				{
					/* Store hash value */
					((REPCONTEXT*)pContext)->piHashList
						[((REPCONTEXT*)pContext)->iCurrentElement]=iHashValue;
				}
				else
				{
					/* Error - Store 0 as hash value */
					((REPCONTEXT*)pContext)->piHashList
						[((REPCONTEXT*)pContext)->iCurrentElement]=0;
				}

				DEALLOC(&pbUrl);
			}
			else
			{
				/* Error - store 0 as hash value */
				((REPCONTEXT*)pContext)->piHashList
							[((REPCONTEXT*)pContext)->iCurrentElement]=0;
			}

			/* Get next resource */
			((REPCONTEXT*)pContext)->iCurrentElement++;

			return TRUE;
		}
	}
		
	/* End */
	return FALSE;
}


/*========================================================================
	REP_RemoveResFromList
==========================================================================
	The function removes (replaces with 0) the repository id from the 
	piResourceList during a repository scan.

	The function is called by REP_PutChannelInSDLList

	Input: pContext (MUST be pREPCONTEXT) and Resource Id
	Output: -
==========================================================================*/
void REP_RemoveResFromList (void* pContext, UINT32 iResourceId)
{
	UINT16 iCount=0;

	while (iCount<((REPCONTEXT*)pContext)->iNumberOfElements)
	{
		if (((REPCONTEXT*)pContext)->piResourceList[iCount]==iResourceId)
		{
			((REPCONTEXT*)pContext)->piResourceList[iCount]=0;
			return;
		}
		iCount++;
	}
}


/*========================================================================
	REP_PutChannelInSDLList
==========================================================================
	NOTE! The current implementation does NOT return channels that are
	*installing*. These channels are instead removed along with their 
	resources. This migth be subject to change.

	The function reads a channel from the repository and stores the data
	in a SDL-list. If the channel could not be found or if it is corrupt,
	FALSE is returned. Otherwise TRUE is returned. The function also does
	a status check (expiry date and status). If the channel is too old or
	if its status is NOT *active*, it is NOT added to the list (TRUE is 
	however still returned in this case - not an error). 

	The function is called by REP_ScanRepository and REP_GetAllChannels.
	If the function is called from REP_ScanRepository, the fScan parameter
	should be set to TRUE. If it is set, the function will increase the
	referring counter (install counter if status on channel is installing)
	by one on every associated resource. Also, all resources that are 
	requested are removed from the piResourceList (replaced with 0).

	The function is called by REP_ScanRepository and REP_GetAllChannels

	Input: pContext (MUST be pREPCONTEXT), Channel id, and pointer to
		   ContentList, fScan (TRUE if called from REP_ScanRepository,
		   FALSE otherwise)
	Output: TRUE/FALSE
==========================================================================*/
BOOL REP_PutChannelInSDLList (void* pContext, UINT32 iChannelID, 
							  void* pContentList, BOOL fScan, UINT32 iTime)
{
	pREPCHSTRUCT pChElm=NULL;
	pREPCONTENTSTRUCT pChannel=NULL;
	UINT8 iCount=0;

	/* Get channel with id */
	pChannel=REP_GetContentWithID(pContext,iChannelID,CONTENTTYPE_CHANNEL);

	if (pChannel!=NULL)
	{
		/* Check status and expiry date */

		if ( ((pChannel->content.pChContentStruct->iExpiryDate !=0 ) &&  
			(pChannel->content.pChContentStruct->iExpiryDate < iTime)) ||
			(pChannel->content.pChContentStruct->iStatus == CONTENT_STALE))
		{
			/* Channel to old, remove */
			if (fScan)
			{
				/* No root block yet - remove channel but do not 
				   change resource counters. */
				Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage, 
																iChannelID);
			}
			else
			{
				/* Remove channel and update root block accordingly */
				if (!REP_DeleteChannel (pContext, iChannelID))
				{
					/* Something is very wrong. */
					REP_ChangeRootBlockStatus (pContext,REPROOT_STATUS_CORRUPT);
				}
			}
		}
		else
		{
			/* Create new REPCHSTRUCT */
			pChElm=NEWSTRUCT(REPCHSTRUCT);

			/* Move fields from pREPCONTENTSTRUCT */
			pChElm->iStatus=pChannel->content.pChContentStruct->iStatus;
			pChElm->pchEventId=pChannel->content.pChContentStruct->pwchEventId;
			pChannel->content.pChContentStruct->pwchEventId=NULL;
			pChElm->pchchannelid=pChannel->content.pChContentStruct->pwchchannelid;
			pChannel->content.pChContentStruct->pwchchannelid=NULL;
			pChElm->iLLRepHId=pChannel->iId;
            pChElm->bServiceAccessible=pChannel->content.pChContentStruct->bServiceAccessible;

			if (pChannel->content.pChContentStruct->piAllResInChList!=NULL)
			{
				pChElm->iLLRepHIdToFirstRes=
						*(pChannel->content.pChContentStruct->piAllResInChList);
			}
			else
			{
				pChElm->iLLRepHIdToFirstRes=0;
			}
		
			if (fScan)
			{
				/* Change resource status on all associated resources. */
				while (iCount<pChannel->content.pChContentStruct->iResCounter)
				{
					if (pChannel->content.pChContentStruct->iStatus==CONTENT_ACTIVE)
					{
						/* Active */
						REP_ChangeResourceStatus(pContext,pChannel->content.
									pChContentStruct->piAllResInChList[iCount],0,1);

						/* Remove resource from piResourceList - all resources that are
						   NOT removed from this list is removed from the repository in
						   PHASE 4 in the REP_ScanRepository function. */
						REP_RemoveResFromList (pContext,pChannel->content.
											pChContentStruct->piAllResInChList[iCount]);
					
					}

					/*	else
					{

					...	THIS PART IS USED IF INSTALLATION SHOULD BE ABLE TO CONTINUE
						EVEN IF THE GWC HAS BEEN RESTARTED.
			
					*/
						/* Installing 
						REP_ChangeResourceStatus(pContext,pChannel->content.
									pChContentStruct->piAllResInChList[iCount],1,0);
						
					}
					*/


					/* Next resource */
					iCount++;
				}
			}
		}

		/* Add channel to list IF IT IS ACTIVE */
		if (pChannel->content.pChContentStruct->iStatus==CONTENT_ACTIVE)
		{
			SDL_AddListElement(pContentList,0,0,pChElm);
		}

		/* Delete pREPCONTENTSTRUCT */
		Free_RepContent(pChannel);

		return TRUE;
	}

	return FALSE;
}


/*========================================================================
==========================================================================
	EXTERNAL FUNCTIONS
==========================================================================
=========================================================================*/

/*========================================================================
	REP_GetResourceWithUrl
=========================================================================*/
pREPCONTENTSTRUCT REP_GetResourceWithUrl (void* pContext, BYTE *pbUrl, 
										  BOOL fInstall)
{
	BOOL fFound=FALSE;
	UINT32 iTempID=0;
	UINT32 iHashValue=0;
	UINT16 iCurRes=0;
	pREPCONTENTSTRUCT pResource=NULL;

	if (pbUrl!=NULL)
	{
		/* Calculate Hash value */
		if (b_HashURL (pbUrl, &iHashValue))
		{
			/* For all resources, do */
			while ((iCurRes<((pREPCONTEXT)(pContext))
									->iNumberOfResources)&&(!fFound))
			{
				iTempID=((pREPCONTEXT)(pContext))->piResourceList[iCurRes];

				/* Check if hash values are equal */
				if (iHashValue==((pREPCONTEXT)(pContext))->piHashList[iCurRes])
				{
					/* Hash values equal, compare url:s */
					fFound=REP_CompareUrl(pContext,pbUrl,iTempID);
				}

				/* Next in list */
				iCurRes++;
			}
		}
		else
		{			
			/* Error in hashing - find in traditional way */
			while ((iCurRes<((pREPCONTEXT)(pContext))
									->iNumberOfResources)&&(!fFound))
			{
				iTempID=((pREPCONTEXT)(pContext))->piResourceList[iCurRes];

				/* Compare url:s */
				fFound=REP_CompareUrl(pContext,pbUrl,iTempID);

				/* Next in list */
				iCurRes++;
			}
		}

		if (fFound)
		{
			/* Get the content */
			pResource=REP_GetContentWithID(pContext,iTempID,
												CONTENTTYPE_RESOURCE);

			if ((pResource!=NULL)&&(fInstall))
			{
				/* Change the install counter (+1) on the resource and change 
				   the state of the accordingly.  */
				if (REP_ChangeResourceStatus(pContext,iTempID,1,0))
				{
					/* Add one to install counter in the copy */
					pResource->content.pResContentStruct->iInstallCounter++;
				}
				else
				{
					/* Could not change status add resource to tMODIFYROOT list 
					   in context with action RESOURCE_REMOVE */
					REP_AddToModifyRoot (pContext,iTempID,
													RESOURCE_REMOVE,iHashValue);
				}				
			}
		}
	}

	return pResource;
}


/*========================================================================
	REP_StoreResourceHelp
==========================================================================*/
int REP_StoreResourceHelp (void* pContext, pREPCONTENTSTRUCT pResource, 
						UINT32* piContentID, UINT32* piSize)
{
	BYTE *pbData=NULL;
	UINT32 iDataSize=0;
	UINT16 iUrlLen=0;
	UINT32 iHeaderLen=0;
	UINT32 iDataLen=0;
	UINT32 iHashValue=0;

	/* Resource must be defined */
	if (pResource!=NULL)
	{
		/* Get size of url */
		iUrlLen=B_STRINGLENGTH((pResource->content).pResContentStruct->pbUrl);

		/* Get size of WSP header */
		iHeaderLen=(((pResource->content).pResContentStruct->pHeaderHandle)->pbEnd)-
			(((pResource->content).pResContentStruct->pHeaderHandle)->pbData);

		/* Get size of Data */
		iDataLen=(pResource->content).pResContentStruct->iBodyLength;

		/* Calculate total size of resource */
		iDataSize=(14+iUrlLen+iHeaderLen+iDataLen);

		/* Check if enough free memory */
		if (Storage_AllocateBlock(&((REPCONTEXT*)pContext)->Storage, 
													iDataSize, piContentID))
		{
			/* OK. Copy data to repository. */
			pbData=REP_CreateResourceData (pResource,iDataSize,
												iHeaderLen,iUrlLen,iDataLen);

			if (pbData==NULL)
			{
				return REPLL_ERROR;
			}

			/* Store the resource under the ContentID */
			if (Storage_Put (&((REPCONTEXT*)pContext)->Storage, 
										*piContentID, 0, iDataSize, pbData))
			{
				DEALLOC(&pbData);

				/* Store the ContentID in the resource */
				pResource->iId=*piContentID;

				/* Calculate hash value */
				b_HashURL((pResource->content).
						pResContentStruct->pbUrl,&iHashValue);

				/* Add resource to tMODIFYROOT list in context with
				   action RESOURCE_REMOVE */
				REP_AddToModifyRoot (pContext,*piContentID,
												RESOURCE_ADD,iHashValue);

				/* Add resource id to context along with its hash value */
				REP_AddResourceToContext (pContext,*piContentID,iHashValue);

				/* Return Content Id and size */
				*piSize=iDataSize;

				return REPLL_OK;
			}
			DEALLOC(&pbData);
		}
        else
          return REPLL_OUT_OF_MEM;
	}
	return REPLL_ERROR;
}


/*========================================================================
	REP_StoreResource
==========================================================================*/
int REP_StoreResource (void* pContext, pREPCONTENTSTRUCT pResource, 
						UINT32* piContentID, UINT32* piSize)
{
	UINT32 iResourceId=0;
	int iResult;

	iResult=REP_StoreResourceHelp(pContext,pResource,&iResourceId,piSize);

	if (iResult==REPLL_OK)
	{
		*piContentID=iResourceId;
	}
	return iResult;
}


/*========================================================================
	REP_UpdateResource
==========================================================================*/
int REP_UpdateResource (void* pContext, pREPCONTENTSTRUCT pResource, 
						 UINT32 iContentID, INT32* piSizeChange)
{
	UINT32 iOldSize=0;
	UINT16 iUrlLen=0;
	UINT32 iHeaderLen=0;
	UINT32 iDataLen=0;
	UINT32 iResourceSize=0;
	UINT32 iNewSize=0;
	int iResult=REPLL_ERROR;

	#ifdef WTA_DEBUG
		xTraceAlg("Update resource");
	#endif

	/* Resource must be defined */
	if (pResource!=NULL)
	{
		/* Get old size */
		iOldSize=Storage_GetBlockSize(&((REPCONTEXT*)pContext)->Storage,iContentID);

		if (iOldSize!=0)
		{
			/* Get size of url */
			iUrlLen=B_STRINGLENGTH((pResource->content).pResContentStruct->pbUrl);

			/* Get size of WSP header */
			iHeaderLen=(((pResource->content).pResContentStruct->pHeaderHandle)->pbEnd)-
				(((pResource->content).pResContentStruct->pHeaderHandle)->pbData);

			/* Get size of Data */
			iDataLen=(pResource->content).pResContentStruct->iBodyLength;

			/* Calculate total size of resource */
			iResourceSize=(14+iUrlLen+iHeaderLen+iDataLen);

			*piSizeChange=iResourceSize-iOldSize;

			#ifdef WTA_DEBUG
				xTraceAlg("Store resource");
			#endif

			if (*piSizeChange>0)
			{
				#ifdef WTA_DEBUG
					xTraceAlg("Store resource new on same location");
				#endif
				
				/* New resource bigger than old. Check if it will fit. */
				if (Storage_NumBytesFree(&((REPCONTEXT*)pContext)->Storage)<
					(UINT32)(*piSizeChange))
				{
					return REPLL_ERROR;
				}
			}
			
			/* Remove old resource */
			Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,iContentID);

			/* Store new resource */
			iResult=REP_StoreResourceHelp(pContext,pResource,&iContentID,&iNewSize);
			
			*piSizeChange=iNewSize-iOldSize;
		}
	}

	return iResult;
}


/*========================================================================
	REP_GetContentWithID
==========================================================================*/
pREPCONTENTSTRUCT REP_GetContentWithID (void* pContext, UINT32 iContentID,
										UINT8 iType)
{
	UINT32 iContentSize=0;
	BYTE* pbData=NULL;
	pREPCONTENTSTRUCT pContent=NULL;


	#ifdef WTA_DEBUG
		xTraceAlg("REP_GetContentWithId - inside");
	#endif

	/* If content is REP_ROOTBLK_ID - return NULL */
	if (iContentID==REP_ROOTBLK_ID)
	{
		return NULL;
	}

	/* Get size of Content */
	iContentSize=Storage_GetBlockSize (&((REPCONTEXT*)pContext)->Storage, 
																iContentID);
	if (iContentSize!=0)
	{
		/* Allocate memory */
		pbData=NEWARRAY(BYTE,iContentSize);

		/* Get Content */
		if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,
										iContentID,0,iContentSize,pbData))
		{
			pContent=NEWSTRUCT(REPCONTENTSTRUCT);
			pContent->Type=pbData[0];
			pContent->iId=iContentID;

			#ifdef WTA_DEBUG
			{
				char chHex[4];
				BYTE bTemp=0;
				int i=0;

				chHex[2]='.';
				chHex[3]=0;

				xTraceAlg("REP_GetContentWithId - content found");

				for (i=0; i<iContentSize; i++)
				{
					bTemp=pbData[i];

					/* First digit */
					if ((bTemp/16)<10)
					{
						chHex[0]=(bTemp/16)+48;					
					}
					else
					{
						chHex[0]=(bTemp/16)+55;
					}

					/* Second digit */
					if ((bTemp % 16)<10)
					{
						chHex[1]=(bTemp % 16)+48;					
					}
					else
					{
						chHex[1]=(bTemp % 16)+55;
					}
					URL_DEBUG_PRINT(chHex);
				}
			}
			#endif

			/* Check type - channel or resource */
			if ((pbData[0]==CONTENTTYPE_CHANNEL)&&(iType==CONTENTTYPE_CHANNEL))
			{
				#ifdef WTA_DEBUG
					xTraceAlg("REP_GetContentWithId - Get channel");
				#endif

				pContent->content.pChContentStruct=REP_GetStoredChannel (pbData);
			}
			else if ((pbData[0]==CONTENTTYPE_RESOURCE)&&(iType==CONTENTTYPE_RESOURCE))
			{
				#ifdef WTA_DEBUG
					xTraceAlg("REP_GetContentWithId - Get resource");
				#endif

				pContent->content.pResContentStruct=REP_GetStoredResource (pbData);

				/* Check if error in content retrieval */
				if (pContent->content.pResContentStruct==NULL)
				{
					DEALLOC(&pContent);

					/* Remove content id */
					Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage, 
																iContentID);

					/* Add resource to tMODIFYROOT list in context with
					   action RESOURCE_REMOVE */
					REP_AddToModifyRoot (pContext,iContentID,RESOURCE_REMOVE,0);
				}
			}
			else
			{
				/* Error */
				DEALLOC(&pContent);
			}
		}

		/* Deallocate memory */
		DEALLOC(&pbData);

		#ifdef WTA_DEBUG
			xTraceAlg("REP_GetContentWithId - Exiting");
		#endif
	}

	return pContent;
}


/*========================================================================
	REP_RootBlockSafe
==========================================================================*/
void REP_RootBlockSafe (void* pContext, BOOL fSafe)
{
	if (pContext!=NULL)
	{
		/* Change status in root block */
		if (fSafe)
		{
			/* Update root block */
			if (REP_UpdateRootBlock (pContext))
			{
				/* OK - Set status to OK */
				REP_ChangeRootBlockStatus(pContext,REPROOT_STATUS_OK);
			}
			else
			{
				/* Error - remove root block */
				REP_ChangeRootBlockStatus(pContext,REPROOT_STATUS_CORRUPT);
			}
		}
		else
		{
			REP_ChangeRootBlockStatus(pContext,REPROOT_STATUS_PROCESSING);
		}
	}
}


/*========================================================================
	REP_DecreaseInstallCounter
==========================================================================*/
BOOL REP_DecreaseInstallCounter (void* pContext, UINT32 iContentID)
{
	if (REP_ChangeResourceStatus(pContext,iContentID,-1,0))
	{
		return TRUE;
	}

	return FALSE;
}


/*========================================================================
	REP_StoreChannel
==========================================================================*/
int REP_StoreChannel (void* pContext, pREPCONTENTSTRUCT pChannel, 
					   UINT32* piContentID, UINT32* piSize)
{
	UINT16 iTitleLen=0;
	UINT16 iAbstractLen=0;
	UINT16 iEventIDLen=0;
	UINT16 iChannelIDLen=0;
	UINT8 iNbrRes=0;
	UINT32 iChLen=0;
	UINT32 iOffset=0;
	UINT32 iContentID=0;
	BYTE* pbData=NULL;
	BOOL fStoreFailed=FALSE;
    int iReturn=REPLL_ERROR;

	if (pChannel!=NULL)
	{
		/* Get size of title */
		if ((pChannel->content).pChContentStruct->pwchTitle!=NULL)
		{
			iTitleLen=STRINGLENGTH((pChannel->content).pChContentStruct->pwchTitle);
		}

		/* Get size of abstract */
		if ((pChannel->content).pChContentStruct->pwchAbstract!=NULL)
		{
			iAbstractLen=STRINGLENGTH((pChannel->content).pChContentStruct->pwchAbstract);
		}

		/* Get size of EventID */
		if ((pChannel->content).pChContentStruct->pwchEventId!=NULL)
		{
			iEventIDLen=STRINGLENGTH((pChannel->content).pChContentStruct->pwchEventId);
		}

		/* Get size of ChannelID */
		if ((pChannel->content).pChContentStruct->pwchchannelid!=NULL)
		{
			iChannelIDLen=STRINGLENGTH((pChannel->content).pChContentStruct->pwchchannelid);
		}

		#ifdef WTA_DEBUG
		{
			char stTemp[10];

			URL_DEBUG_PRINT("Event id len (Storing) : ");
			_itoa( iEventIDLen, stTemp, 10 );
			URL_DEBUG_PRINT(stTemp);
		}
		#endif

		/* Get number of resources */
 		iNbrRes=(UINT8)(pChannel->content).pChContentStruct->iResCounter;		

		/* Calculate total size of channel */
		iChLen=(17+iTitleLen*2+iAbstractLen*2+iEventIDLen*2+iChannelIDLen*2+iNbrRes*4);

		/* Check if enough free memory */
		if (Storage_AllocateBlock(&((REPCONTEXT*)pContext)
									->Storage, iChLen, &iContentID))
		{
			/* OK. Copy data to repository. */
			pbData=NEWARRAY(BYTE,iChLen);

			pbData[0]=CONTENTTYPE_CHANNEL;
			pbData[1]=CONTENT_INSTALLING;
			(pChannel->content).pChContentStruct->iStatus=CONTENT_INSTALLING;


			iOffset=2;

			/* Expiry date */
			B_COPYSTRINGN(pbData+iOffset,&((pChannel->content).
									pChContentStruct->iExpiryDate),4);

			/* Store User Accessible */
			if ((pChannel->content).pChContentStruct->useraccess)
			{
				pbData[6]=0x01;
			}
			else
			{
				pbData[6]=0x00;
			}

			/* Store ServiceAccessible */
			if ((pChannel->content).pChContentStruct->bServiceAccessible)
			{
				pbData[7]=0x01;
			}
			else
			{
				pbData[7]=0x00;
			}

			iOffset+=6;

			/* Store length of title */
			B_COPYSTRINGN(pbData+iOffset,&iTitleLen,2);
			iOffset+=2;

			/* Store title */
			if ((pChannel->content).pChContentStruct->pwchTitle!=NULL)
			{
				B_COPYSTRINGN(pbData+iOffset,((pChannel->content).
																	pChContentStruct->pwchTitle),2*iTitleLen);
				iOffset+=(2*iTitleLen);
			}

			/* Store length of abstract */
			B_COPYSTRINGN(pbData+iOffset,&iAbstractLen,2);
			iOffset+=2;

			/* Store abstract */
			if ((pChannel->content).pChContentStruct->pwchAbstract!=NULL)
			{
				B_COPYSTRINGN(pbData+iOffset,((pChannel->content).
								pChContentStruct->pwchAbstract),(2*iAbstractLen));
				iOffset+=(2*iAbstractLen);
			}

			/* Store length of eventid */
			B_COPYSTRINGN(pbData+iOffset,&iEventIDLen,2);
			iOffset+=2;

			/* Store eventid */
			if ((pChannel->content).pChContentStruct->pwchEventId!=NULL)
			{
				B_COPYSTRINGN(pbData+iOffset,((pChannel->content).
								pChContentStruct->pwchEventId),(2*iEventIDLen));
				iOffset+=(2*iEventIDLen);
			}

			/* Store length of channelid */
			B_COPYSTRINGN(pbData+iOffset,&iChannelIDLen,2);
			iOffset+=2;

			/* Store channelid */
			if ((pChannel->content).pChContentStruct->pwchchannelid!=NULL)
			{
				B_COPYSTRINGN(pbData+iOffset,((pChannel->content).
								pChContentStruct->pwchchannelid),(2*iChannelIDLen));
				iOffset+=(2*iChannelIDLen);
			}

			/* Store nbr of resources */
			B_COPYSTRINGN(pbData+iOffset,&iNbrRes,1);
			iOffset+=1;

			/* Store resource list */
			if (iNbrRes!=0)
			{
				B_COPYSTRINGN(pbData+iOffset,((pChannel->content).
							pChContentStruct->piAllResInChList),(4*iNbrRes));
			}

			#ifdef WTA_DEBUG
			{
				char chHex[4];
				BYTE bTemp=0;
				int i=0;
				char stTemp[10];

				chHex[2]='.';
				chHex[3]=0;

				xTraceAlg("REP_StoreChannel - content to be stored");

				URL_DEBUG_PRINT("Channel data len (Storing) : ");
				_itoa( iChLen, stTemp, 10 );
				URL_DEBUG_PRINT(stTemp);


				for (i=0; i<iChLen; i++)
				{
					bTemp=pbData[i];

					/* First digit */
					if ((bTemp/16)<10)
					{
						chHex[0]=(bTemp/16)+48;					
					}
					else
					{
						chHex[0]=(bTemp/16)+55;
					}

					/* Second digit */
					if ((bTemp % 16)<10)
					{
						chHex[1]=(bTemp % 16)+48;					
					}
					else
					{
						chHex[1]=(bTemp % 16)+55;
					}
					URL_DEBUG_PRINT(chHex);
				}
			}
			#endif


			/* Store the channel under the ContentID */
			if (Storage_Put (&((REPCONTEXT*)pContext)->Storage, 
										iContentID, 0, iChLen, pbData))
			{
				/* Store the ContentID in the resource */
				pChannel->iId=iContentID;
				*piContentID=iContentID;
				*piSize=iChLen;
				
				DEALLOC(&pbData);

				return REPLL_OK;
			}
			else
			{
				fStoreFailed=TRUE;
			}

			DEALLOC(&pbData);
		}
		else /* Memory allocation failed */
		{
			fStoreFailed=TRUE;
            iReturn = REPLL_OUT_OF_MEM;
		}

		if (fStoreFailed)
		{
			UINT32 iIdToRemove;
			UINT8 iCount=0;
		
			/* Use this variable again */
			fStoreFailed=FALSE;

			/* The channel could not be stored. Deduct the installing 
			   counter in the resources used by the channel with one. */

			/* For each resource in the channel, do */
			while ((iCount<pChannel->content.
								pChContentStruct->iResCounter)&&(!fStoreFailed))
			{
				/* Get next resource to remove */
				iIdToRemove=pChannel->content.
							pChContentStruct->piAllResInChList[iCount];

				/* Change the install counter (-1) on the resource and change 
				   the state of the resource accordingly.  */
				if (!REP_ChangeResourceStatus(pContext,iIdToRemove,-1,0))
				{
					fStoreFailed=TRUE;
				}

				/* Next resource */
				iCount++;
			}

			if (fStoreFailed)
			{
				/* Fatal error - remove root block. */
				REP_ChangeRootBlockStatus(pContext,REPROOT_STATUS_CORRUPT);
			}
   		}	
	}
	return iReturn;
}


/*========================================================================
	REP_DeleteChannel
==========================================================================*/
BOOL REP_DeleteChannel (void* pContext, UINT32 iChannelID)
{
	UINT8 iCount=0;
	UINT32 iIdToRemove=0;
	BOOL fOK=FALSE;
	pREPCONTENTSTRUCT pChannel=NULL;

	/* Find the channel */
	pChannel=REP_GetContentWithID(pContext,iChannelID,CONTENTTYPE_CHANNEL);

	/* Remove the channel from repository */
	fOK=Storage_DeleteBlock(&((REPCONTEXT*)pContext)
												->Storage,iChannelID);

	if (pChannel!=NULL)
	{
		/* For each resource in the channel, do */
		while ((iCount<pChannel->content.
							pChContentStruct->iResCounter)&&(fOK))
		{
			/* Get next resource to remove */
			iIdToRemove=pChannel->content.
						pChContentStruct->piAllResInChList[iCount];

			if (pChannel->content.pChContentStruct
										->iStatus==CONTENT_ACTIVE)
			{
				/* Change the ref counter (-1) on the resource and change 
				   the state of the accordingly.  */
				fOK=REP_ChangeResourceStatus(pContext,iIdToRemove,0,-1);
			}
			else if (pChannel->content.pChContentStruct
										->iStatus==CONTENT_INSTALLING)
			{
				/* Change the install counter (-1) on the resource and change 
				   the state of the accordingly.  */
				fOK=REP_ChangeResourceStatus(pContext,iIdToRemove,-1,0);
			}
			else
			{
				fOK=FALSE;
			}

			/* Next resource */
			iCount++;
		}

		/* Add a tMODIFYROOT-element to the context. Action: CHANNEL_REMOVE  */
		REP_AddToModifyRoot(pContext,iChannelID,CHANNEL_REMOVE,0);

		/* Deallocate channel */
		Free_RepContent (pChannel);

		return fOK;
	}
	return FALSE;
}


/*========================================================================
	REP_ActivateChannel
==========================================================================*/
BOOL REP_ActivateChannel (void* pContext, UINT32 iChannelID)
{
	pREPCONTENTSTRUCT pChannel=NULL;
	UINT32 iIdToActivate=0;
	UINT8 iCount=0;
	UINT8 iErrorCount=0;
	BOOL fError=FALSE;
	BYTE bActive=CONTENT_ACTIVE;

	/* Find the channel */
	pChannel=REP_GetContentWithID(pContext, iChannelID, CONTENTTYPE_CHANNEL);

	#ifdef WTA_DEBUG
		xTraceAlg("REP_ActivateChannel - after REP_GetContentWithID");
	#endif

	if (pChannel!=NULL)
	{
		#ifdef WTA_DEBUG
			xTraceAlg("REP_ActivateChannel - pChannel NOT NULL");
		#endif

		/* For each resource in the channel, do */
		while ((iCount<pChannel->content.
							pChContentStruct->iResCounter)&&(!fError))
		{
			/* Get next resource to activate */
			iIdToActivate=pChannel->content.
						pChContentStruct->piAllResInChList[iCount];

			/* Change the install counter (-1) and the ref counter (+1) 
			   on the resource and change the state of the accordingly.  */
			if (REP_ChangeResourceStatus(pContext,iIdToActivate,-1,1))
			{
				/* Next resource */
				iCount++;
			}
			else
			{
				fError=TRUE;
			}
		}

		#ifdef WTA_DEBUG
			xTraceAlg("REP_ActivateChannel - All resources activated");
		#endif

		/* All resources activated? */
		if (!fError)
		{
			#ifdef WTA_DEBUG
				xTraceAlg("REP_ActivateChannel - No errors");
			#endif

			/* Activate the channel */
			if (Storage_Put(&((REPCONTEXT*)pContext)
									->Storage,iChannelID,1,1,&bActive))
			{

				/* Add a tMODIFYROOT-element to the context. 
				   Action: CHANNEL_ADD  */
				REP_AddToModifyRoot(pContext,iChannelID,CHANNEL_ADD,0);

				/* Deallocate channel */
				Free_RepContent(pChannel);

				#ifdef WTA_DEBUG
					xTraceAlg("REP_ActivateChannel - Added to root block");
				#endif

				/* Return TRUE */
				return TRUE;
			}
		}		

		/* Could not activate all resources or the channel. 
		   Decrease RefCounter with one on all changed resources
		   and remove channel from repository. */
		while (iErrorCount<=iCount)
		{
			/* Change the ref counter (-1) on the resource and 
			   change the state accordingly.  */
			REP_ChangeResourceStatus(pContext,iIdToActivate,0,-1);

			/* Next resource */
			iErrorCount++;
		}
		
		/* Remove channel from repository */
		Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,iChannelID);

		/* Deallocate channel */
		Free_RepContent(pChannel);
	}

	return FALSE;
}


/*========================================================================
	REP_GetRootBlock
==========================================================================*/
BOOL REP_GetRootBlock (void* pContext)
{
	UINT32 iRootBlkSize=0;
	BYTE* pbRootBlk=NULL;
	BOOL fResult=FALSE;

	/* ASSERT: pContext!=NULL
	*/

	#ifdef WTA_DEBUG
		xTraceAlg("Inside REP_GetRootBlock");
	#endif

	/* Get size of Root Block */
	iRootBlkSize=Storage_GetBlockSize (&((REPCONTEXT*)pContext)->Storage, 
																REP_ROOTBLK_ID);

	#ifdef WTA_DEBUG
		xTraceAlg("Root block size retrieved");
	#endif

	if (iRootBlkSize!=0)
	{
		/* Allocate memory */
		pbRootBlk=NEWARRAY(BYTE,iRootBlkSize);

		/* Get Root Block */
		if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,
										REP_ROOTBLK_ID,0,iRootBlkSize,pbRootBlk))
		{
			#ifdef WTA_DEBUG
				xTraceAlg("Root block retrieved");
			#endif

			/* Check if status ok */
			if (pbRootBlk[0]==REPROOT_STATUS_OK)
			{
				#ifdef WTA_DEBUG
					xTraceAlg("Root block status OK");
				#endif

				/* Get nbr of Channels */
				B_COPYSTRINGN (&(((pREPCONTEXT)pContext)->iNumberOfElements),pbRootBlk+1,2);

				/* Get nbr of Resources */
				B_COPYSTRINGN (&(((pREPCONTEXT)pContext)->iNumberOfResources),pbRootBlk+3,2);

				/* Create Channel List */
				((pREPCONTEXT)pContext)->piElementList=
					NEWARRAY(UINT32,((pREPCONTEXT)pContext)->iNumberOfElements);

				/* Create Resource List */
				((pREPCONTEXT)pContext)->piResourceList=
					NEWARRAY(UINT32,((pREPCONTEXT)pContext)->iNumberOfResources);

				/* Create Hash List */
				((pREPCONTEXT)pContext)->piHashList=
					NEWARRAY(UINT32,((pREPCONTEXT)pContext)->iNumberOfResources);

				/* Get Channels */
				B_COPYSTRINGN (((pREPCONTEXT)pContext)->piElementList,
					pbRootBlk+5, 4*((pREPCONTEXT)pContext)->iNumberOfElements);

				/* Get Resources */
				B_COPYSTRINGN (((pREPCONTEXT)pContext)->piResourceList,
						pbRootBlk+5+(4*((pREPCONTEXT)pContext)->iNumberOfElements),
										4*((pREPCONTEXT)pContext)->iNumberOfResources);

				/* Get Hash values */
				B_COPYSTRINGN (((pREPCONTEXT)pContext)->piHashList,
					pbRootBlk+5+(4*(((pREPCONTEXT)pContext)->iNumberOfElements))+
									(4*(((pREPCONTEXT)pContext)->iNumberOfResources)),
										4*((pREPCONTEXT)pContext)->iNumberOfResources);

				/* OK! */
				fResult=TRUE;

			}
			else
			{
				/* Delete Root Block */
				Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage, 
																REP_ROOTBLK_ID);
			}
		}

		/* Deallocate memory */
		DEALLOC(&pbRootBlk);
	}

	return fResult;
}


/*========================================================================
	REP_ScanRepository
==========================================================================*/
BOOL REP_ScanRepository (void* pContext, void** ppContentList, UINT32 iTime)
{
	UINT16 iNbrOfElm=0;
	UINT32* piContentList=NULL;
	UINT32 iCurId=0;
	UINT16 iCur=0;
	BYTE abData[4];

	/* Check ScanPhase. Five phases. */
	switch (((pREPCONTEXT)(pContext))->iScanPhase)
	{
	case 1:
		#ifdef WTA_DEBUG
			xTraceAlg("REP_ScanRepository (PHASE 1)");
		#endif

		/* Start up - Delete root block */
		Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,REP_ROOTBLK_ID);

		if (*ppContentList==NULL)
		{
			/* Create new list (Max = 0 -> unlimited elements) */
			*ppContentList=SDL_NewList (0);

			/* First time. Get all contentids */
			if (Storage_GetAllBlockIds(&((REPCONTEXT*)pContext)->Storage,
													&piContentList,&iNbrOfElm))
			{
				if (iNbrOfElm!=0)
				{
					/* Store data in context */
					((pREPCONTEXT)(pContext))->piElementList=piContentList;
					((pREPCONTEXT)(pContext))->iNumberOfElements=iNbrOfElm;
					((pREPCONTEXT)(pContext))->iCurrentElement=0; 

					/* Make copy of list and store in resource list */
					((pREPCONTEXT)(pContext))->piResourceList=
													NEWARRAY(UINT32,(iNbrOfElm));
					B_COPYSTRINGN(((pREPCONTEXT)(pContext))->piResourceList,
						((pREPCONTEXT)(pContext))->piElementList,(4*iNbrOfElm));

					/* Go to Phase 2. */
					((pREPCONTEXT)(pContext))->iScanPhase=2; 
					return TRUE;
				}
			}

			/* Memory is empty. Create empty root block */

			REP_StoreRootBlock(pContext);

			return FALSE;
		}
		/* Error */
		break;

	case 2:
	/*		
	   (PHASE 2) Step through all elements. Keep channel id:s in 
	   piElementList, replace resource ids with 0. Keep resource id:s in 
	   piResourceList, replace channel ids with 0. For all found resources, 
	   set the installation counter and referring counter to zero temporarily. 
	   DO NOT REMOVE THE RESOURCES FROM STORAGE. 

	   When the list is scanned copy the resource list and store it in the
	   hash list temporarily. Set Scan phase to 3.
	*/

		#ifdef WTA_DEBUG
			xTraceAlg("REP_ScanRepository (PHASE 2)");
		#endif

		/* Check if more id:s to scan. */
		if (((pREPCONTEXT)(pContext))->iCurrentElement<
					((pREPCONTEXT)(pContext))->iNumberOfElements)
		{
			/* Get id */
			iCurId=((pREPCONTEXT)(pContext))->
				piElementList[((pREPCONTEXT)(pContext))->iCurrentElement];

			/* Get first four bytes for id */
			if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,iCurId,0,4,abData))
			{
				/* Check type and status of content */

				/*  ...	IF INSTALLATION SHOULD BE ABLE TO CONTINUE EVEN IF THE GWC 
						HAS BEEN RESTARTED THE *active*-CONTROL SHOULD BE REMOVED */

				if ((abData[0]==CONTENTTYPE_CHANNEL)&&(abData[1]==CONTENT_ACTIVE))
				{
					/* Delete from resource list */
					((pREPCONTEXT)(pContext))->piResourceList
						[((pREPCONTEXT)(pContext))->iCurrentElement]=0;
				}
				else if (abData[0]==CONTENTTYPE_RESOURCE)
				{
					/* Delete from channel list */
					((pREPCONTEXT)(pContext))->piElementList
						[((pREPCONTEXT)(pContext))->iCurrentElement]=0;

					/* Set resource counter and install counter to 0 */
					abData[2]=0;
					abData[3]=0;

					if (!Storage_Put(&((REPCONTEXT*)pContext)->Storage,
															iCurId,0,4,abData))
					{
						/* Error. Could not store new counters, delete */
						Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,iCurId);
					}
				}
				else
				{
					/* Delete (not resource or channel (?) )*/
					Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,iCurId);

					/* Delete from resource list and channel list */
					((pREPCONTEXT)(pContext))->piResourceList
							[((pREPCONTEXT)(pContext))->iCurrentElement]=0;
					((pREPCONTEXT)(pContext))->piElementList
							[((pREPCONTEXT)(pContext))->iCurrentElement]=0;
				}
			}

			/* Next element in list */
			((pREPCONTEXT)(pContext))->iCurrentElement++;
		}
		else
		{	
			/* Create hash list */
			((pREPCONTEXT)(pContext))->piHashList=
				NEWARRAY(UINT32,(((pREPCONTEXT)(pContext))->iNumberOfElements));

			/* Copy resource list and store in hash list */
			B_COPYSTRINGN(((pREPCONTEXT)(pContext))->piHashList,((pREPCONTEXT)
				(pContext))->piResourceList,4*((pREPCONTEXT)(pContext))->iNumberOfElements);

			/* Reset counter and go to PHASE 3 */
			((pREPCONTEXT)(pContext))->iScanPhase=3; 
			((pREPCONTEXT)(pContext))->iCurrentElement=0;
		}
		return TRUE;

	case 3:
	/*		
	   (PHASE 3)  Add all channels to ppContentList and update all 
	   associated resources accordingly. Replace all changed resource id:s 
	   in piResourceList with 0. 
	*/

		#ifdef WTA_DEBUG
			xTraceAlg("REP_ScanRepository (PHASE 3)");
		#endif

		/* Get next channel */
		iCur=((pREPCONTEXT)(pContext))->iCurrentElement;
		iCurId=0;

		/* Find first remaining channelid */
		while ((iCur<((pREPCONTEXT)(pContext))->iNumberOfElements)&&(iCurId==0))
		{
			iCurId=((pREPCONTEXT)(pContext))->piElementList[iCur];
				
			if (iCurId==0)
			{
				/* Get next id in piElementList */
				iCur++;
			}
			else
			{
				/* Channel found update iCurrentElement */
				((pREPCONTEXT)(pContext))->iCurrentElement=(UINT16)(iCur+1);
			}
		}

		if (iCurId!=0)
		{
			/* Channel id found. Store channel in ppContentList and update all
			   its associated resources. */
			if (!REP_PutChannelInSDLList(pContext,iCurId,*ppContentList,TRUE,iTime))
			{
				/* Error. Remove id */
				Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,iCurId);
			}

			return TRUE;
		}

		((pREPCONTEXT)(pContext))->iCurrentElement=0;

		/* Go to PHASE 4 */
		((pREPCONTEXT)(pContext))->iScanPhase=4;
		return TRUE;	

	case 4:
	/*			   
	   (PHASE 4) Remove all unreferenced resources (elements in the 
	   piResourceList) from the repository. ALSO, REPLACE THEIR ID'S 
	   IN THE HASH LIST WITH 0.

	   Copy the hash list to the resource list, remove the hash list,
	   trim the resource list, set the resource counter, and go to PHASE 5.
	*/
		#ifdef WTA_DEBUG
			xTraceAlg("REP_ScanRepository (PHASE 5)");
		#endif

		if (REP_RemoveAllUnrefResources (pContext))
		{
			/* Continue removal */
			return TRUE;
		}
		else
		{
			/* Copy hash list to resource list */
			B_COPYSTRINGN(((pREPCONTEXT)(pContext))->piResourceList,((pREPCONTEXT)
				(pContext))->piHashList,4*((pREPCONTEXT)(pContext))->iNumberOfElements);

			/* Remove Hash list */
			DEALLOC(&((pREPCONTEXT)(pContext))->piHashList);

			/* Trim Resource list and set resource counter */
			REP_TrimResourceList (pContext);

			/* Go to PHASE 5 */
			((pREPCONTEXT)(pContext))->iCurrentElement=0;
			((pREPCONTEXT)(pContext))->iScanPhase=5;
			return TRUE;
		}

	case 5:
	/*			   
	   (PHASE 5) Calculates hash values for all resources in the piResourceList
	   and store the values the piHashList.
	*/
		#ifdef WTA_DEBUG
			xTraceAlg("REP_ScanRepository (PHASE 5)");
		#endif

		if (REP_CalculateHashValues (pContext))
		{
			/* Continue hash calculations */
			return TRUE;
		}
		else
		{
			/* Go to PHASE 6 */
			((pREPCONTEXT)(pContext))->iScanPhase=6;
			return TRUE;
		}

	case 6:
	/*			   
	   (PHASE 6) Trim piElementList to include all channels. Store root block 
	   and set to REPROOT_STATUS_OK. Clean up and return FALSE.
	*/
	
		#ifdef WTA_DEBUG
			xTraceAlg("REP_ScanRepository (PHASE 6)");
		#endif

		/* Trim Channel list */
		REP_TrimChannelList (pContext);
			
		/* Store root block and set status to REPROOT_STATUS_OK */
		REP_StoreRootBlock(pContext);
		
		/* Clean up */
		((pREPCONTEXT)(pContext))->iScanPhase=0;
		((pREPCONTEXT)(pContext))->iCurrentElement=0;
		((pREPCONTEXT)(pContext))->iNumberOfElements=0;
		DEALLOC(&(((pREPCONTEXT)(pContext))->piElementList));

		return FALSE;
	}

	return FALSE;
}


/*========================================================================
	REP_GetAllChannels
==========================================================================*/
BOOL REP_GetAllChannels (void* pContext, void** ppContentList, UINT32 iTime)
{
	UINT32 iChannelID=0;

	/* ASSERT: pContext ok
	*/

	if (*ppContentList==NULL)
	{
		/* Create new list (Max = 0 -> unlimited elements) */
		*ppContentList=SDL_NewList (0);
	}
	
	/* Get next Channel ID and increase list counter. */
	if (((pREPCONTEXT)(pContext))->iCurrentElement<
			((pREPCONTEXT)(pContext))->iNumberOfElements)
	{
		/* Get channel id */
		iChannelID=((pREPCONTEXT)(pContext))->
			piElementList[((pREPCONTEXT)(pContext))->iCurrentElement];

		((pREPCONTEXT)(pContext))->iCurrentElement++;


		if (!REP_PutChannelInSDLList(pContext,iChannelID,*ppContentList,FALSE,iTime))
		{
			/* Could not find channel. Something is wrong. Maybe a
			   difference between the root block and the repository
			   memory. Set root block to REPROOT_STATUS_CORRUPT.  */
			REP_ChangeRootBlockStatus(pContext,REPROOT_STATUS_CORRUPT);
		}

		/* More channels? */
		if (((pREPCONTEXT)(pContext))->iCurrentElement<
				((pREPCONTEXT)(pContext))->iNumberOfElements)
		{
			return TRUE;
		}
	}

	/* No more channels - delete Channel List */
	DEALLOC(&((pREPCONTEXT)(pContext))->piElementList);
	((pREPCONTEXT)(pContext))->iNumberOfElements=0;
	((pREPCONTEXT)(pContext))->iCurrentElement=0; 

	return FALSE;
}


/*========================================================================
	REP_GetSizeOfContent
==========================================================================*/
UINT32 REP_GetSizeOfContent (void* pContext, UINT32 iContentID)
{
	return (Storage_GetBlockSize(&((REPCONTEXT*)pContext)->Storage,iContentID));	
}


/*========================================================================
	REP_CompactRepository
==========================================================================*/
BOOL REP_CompactRepository (void* pContext)
{
	return (Storage_Compact(&((REPCONTEXT*)pContext)->Storage));
}


/*========================================================================
	REP_ResetRepMem
==========================================================================*/
void REP_ResetRepMem (void* pContext)
{
	UINT32* piIds=NULL;
	UINT16 iLen=0;
	UINT16 iCount=0;

	/* Get all id:s */
	Storage_GetAllBlockIds(&((REPCONTEXT*)pContext)->Storage,&piIds,&iLen);

	while (iCount<iLen)
	{
		/* Delete block */
		Storage_DeleteBlock(&((REPCONTEXT*)pContext)->Storage,piIds[iCount]);
		iCount++;
	}

	DEALLOC(&piIds);

	/* Delete in context */
	((REPCONTEXT*)pContext)->iCurrentElement=0;
	((REPCONTEXT*)pContext)->iNumberOfElements=0;
	((REPCONTEXT*)pContext)->iNumberOfResources=0;
	((REPCONTEXT*)pContext)->iScanPhase=1;
	DEALLOC(&((REPCONTEXT*)pContext)->piElementList);
	DEALLOC(&((REPCONTEXT*)pContext)->piHashList);
	DEALLOC(&((REPCONTEXT*)pContext)->piResourceList);
	REP_RemoveModifyList(pContext);
}


/*========================================================================
	REP_InitRepMem
==========================================================================*/
void* REP_InitRepMem (UINT32 iRepSize)
{
	pREPCONTEXT pContext=NULL;

	#ifdef WTA_DEBUG
		xTraceAlg("Inside REP_InitRepMem");
	#endif

	/* Create Context struct */
	pContext=NEWSTRUCT(REPCONTEXT);

	/* Init fields */
	pContext->iNumberOfElements=0;
	pContext->iCurrentElement=0;
	pContext->piElementList=NULL;
	pContext->iScanPhase=1;

	pContext->piHashList=NULL;
	pContext->piResourceList=NULL;
	pContext->iNumberOfResources=0;
	pContext->pModRoot=NULL;

	/* Init Storage Object */
	if (Storage_Init (&pContext->Storage,iRepSize,(ReadFunction*)
			MEMa_readServiceRepository,	(WriteFunction*)MEMa_writeServiceRepository))
	{
		#ifdef WTA_DEBUG
			xTraceAlg("Storage initiated");
		#endif

		return pContext;
	}
	else
	{
		#ifdef WTA_DEBUG
			xTraceAlg("Storage initiation failed");
		#endif

		/* Error -> dealloc pContext */
		DEALLOC (&pContext);
	}

	return NULL;
}


/*========================================================================
	REP_TerminateRepMem
==========================================================================*/
void REP_TerminateRepMem (void* pContext)
{
	pREPCONTEXT pTemp=(pREPCONTEXT)pContext;

	/* Delete in context */
	DEALLOC(&((REPCONTEXT*)pContext)->piElementList);
	DEALLOC(&((REPCONTEXT*)pContext)->piHashList);
	DEALLOC(&((REPCONTEXT*)pContext)->piResourceList);

	REP_RemoveModifyList(pContext);

	/* Delete repository memory struct */
	Storage_Finalize(&(((REPCONTEXT*)pContext)->Storage));
	
	/* Delete context */
	DEALLOC(& pTemp);
}


/*========================================================================
	REP_GetNextChToRemove
==========================================================================*/
UINT32 REP_GetNextChToRemove (void* pContext, unsigned long int* pliExpiryDate )
{
	UINT32 iChannelToRemove=0;
	UINT32 iExpires=0;
	UINT32 iExpiresTemp=0;
	UINT16 iNbrOfElm=0;
	UINT16 iCount=0;
	UINT32* piContentList=NULL;
	UINT32 iContentId;
	BYTE pbData[6];
	BOOL fFirst=TRUE;

	#ifdef WTA_DEBUG
		xTraceAlg("REP_GetNextChToRemove entered.");
	#endif

	/* Get all contentids */
	if (Storage_GetAllBlockIds(&((REPCONTEXT*)pContext)->Storage,
											&piContentList,&iNbrOfElm))
	{
		/* Check all id:s */
		for (iCount=0; iCount<iNbrOfElm; iCount++)
		{
			iContentId=piContentList[iCount];

			/* Do not check if content id is 1 (root block) - not a channel */
			if (iContentId != REP_ROOTBLK_ID)
			{
				/* Get first 6 bytes of content */
				if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,
												iContentId,0,6,pbData))
				{
					if (pbData[0]==CONTENTTYPE_CHANNEL)
					{
						/* Channel found, get expiry date */
						B_COPYSTRINGN(&iExpiresTemp,pbData+2,4);

						if ((iExpiresTemp!=0)&&(iExpiresTemp<iExpires))
						{
							/* Check expiry value */
							if (iExpiresTemp<iExpires)
							{
								iChannelToRemove=iContentId;
								iExpires=iExpiresTemp;
							}
						}
						else if ((iExpiresTemp!=0)&&(fFirst))
						{
							/* This is the first channel with expires value set */
							iChannelToRemove=iContentId;
							iExpires=iExpiresTemp;
							fFirst=FALSE;
						}
					}
				}
			}
		}

		/* Remove list */
		DEALLOC (&piContentList);
	}

	/* Return found values */
	*pliExpiryDate=iExpires;
	return iChannelToRemove;
}


/*========================================================================
	REP_SetServiceAccessible
==========================================================================*/
BOOL REP_SetServiceAccessible (void* pContext, UINT32 iContentId, 
							  BOOL status)
{
  BYTE pbData[8];
  BOOL fRemove=FALSE;

  /* Find resource */
  if (Storage_Get(&((REPCONTEXT*)pContext)->Storage,iContentId,0,8,pbData))
  {
	if (pbData[0]==CONTENTTYPE_CHANNEL)
	{
      if (status)
        pbData[7]=1;
      else
        pbData[7]=0;

      /* Update resource */
	  if (Storage_Put(&((REPCONTEXT*)pContext)->Storage,iContentId,0,8,pbData))
      {
        /* OK */
        return TRUE;
      }	   
	}
  }
  return FALSE;
}
