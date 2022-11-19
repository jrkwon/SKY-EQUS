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
#ifndef _RepLL_H
#define _RepLL_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: RepLL.h

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  Date		Sign	Comment
  990803  JPR		First version
	990831	JPR		Added functionality
	990929	JPR		Storage.h included, functions updated.
	990930	JPR		Init functions updated.
	991001	JPR		Function REP_Installing added
	991004	JPR		tMODIFYRESOURCES struct and functions added
	991008	JPR		Function REP_UpdateResource added
	991026	JPR		Updates in functions
	000207	JPR		Function REP_GetNextChToRemove added
	000214	JPR		Comments changed in REP_GetNextChToRemove
	000215	JPR		REP_GetAllChannels and REP_ScanRepository updated
	000630	JPR		Updated for WAP 1.2.1
	001011	ASPN	Lower-case filenames.
    010330  ASPN    iReserved removed from struct tREPCONTEXT.
                    Functions REP_Allocate and MemoryREP_FreeMemory removed.
    010410  ASPN    ServiceAccessible added
                    Function REP_SetServiceAccessible added

=========================================================================*/

#include "cmmnrsrc.h"
#include "rephdr.h"
#include "wae_cmmn.h"
#include "storage.h"

/*========================================================================
	DEFINITIONS
=========================================================================*/

typedef struct tREPCHSTRUCT
{
	WCHAR		*pchEventId;
	WCHAR		*pchchannelid;
	UINT32	iLLRepHId;
	UINT32	iLLRepHIdToFirstRes;
	UINT8		iStatus;
    BOOL        bServiceAccessible;
} REPCHSTRUCT, *pREPCHSTRUCT;

/* This struct is used to update the root block when resources or 
   channels are added to or removed from the repository. All content 
   in the list are either added to or removed from the root block list 
   of resources or channels. The elements in the list must always 
   reflect the changes needed on the root block to be consistent with
   the repository. */
typedef struct tMODIFYROOT
{
	UINT32	iContentId;
	UINT8		iAction; /* X_ADD/X_REMOVE */
	UINT32	iHashValue;
	struct	tMODIFYROOT* pNext;
} MODIFYROOT, *pMODIFYROOT;

typedef struct tREPCONTEXT
{
	/* Used to init the repository. Used for channels in function
	   REP_GetAllChannels and for all content in REP_ScanRepository */
	UINT16		iNumberOfElements;
	UINT16		iCurrentElement;
	UINT32*		piElementList;
	UINT8			iScanPhase;

	/* Used during scan repository and at run-time */
	UINT32*		piResourceList;
	UINT32*		piHashList; /* One number for each resource */
	UINT16		iNumberOfResources;

	/* Storage object */
	StorageObject Storage;

	/* Pointer to content to be modified in the root block */
	pMODIFYROOT pModRoot;

} REPCONTEXT, *pREPCONTEXT;


/* The Root Block is always stored at ContentID 1 */
#define REP_ROOTBLK_ID 1

#define RESOURCE_ADD 1
#define RESOURCE_REMOVE 2
#define CHANNEL_ADD 3
#define CHANNEL_REMOVE 4

#define REPLL_ERROR       0
#define REPLL_OK          1
#define REPLL_OUT_OF_MEM  2


/*========================================================================
	FUNCTION PROTOTYPES
=========================================================================*/

/*========================================================================
	REP_GetResourceWithUrl
==========================================================================
	The function retrieves a resource from the repository, specified with 
	the url. The url MUST be absolute. If the resource is found, a pointer
	to a REPCONTENTTYPE struct is returned, otherwise NULL is returned. 
	It is the caller's responsibility to free the memory of the url and 
	the REPCONTENTTYPE struct. If the Install flag is set to TRUE, the 
	Installing Counter in the resource is increased with one.
	
	The function also creates a tMODIFYRESOURCE-element.

	The function does a WSP_PreParseHeaders on the header data. If this
	operation fails, NULL is returned.
  
	Input: void* (MUST be pREPCONTEXT), url, and BOOL
	Output: pREPCONTENTSTRUCT or NULL if error
==========================================================================*/
pREPCONTENTSTRUCT REP_GetResourceWithUrl (void* pContext, BYTE *pbUrl, 
										  BOOL fInstall);


/*========================================================================
	REP_StoreResource
==========================================================================
	The function stores a resource in the repository with the installation 
	counter set to one and the referring counter set to zero. If the 
	operation was successful, TRUE is returned and the input parameter 
	piContentID is set accordingly. Also, the pResource is updated with 
	the same value.	FALSE is returned if the operation fails. It is the 
	caller's responsibility to delete the memory after the function has 
	returned. 
	
	The function creates a tMODIFYRESOURCE-element for each change needed
	in the root block. The resource's ContentID is also added to the 
	context along with its hash value.

	If the function returns TRUE, the size of the stored resource (in
	BYTES) is stored in the piSize attribute. The amount is also decucted 
	from the reserved memory. 

	Input: void* (MUST be pREPCONTEXT), Data, and pointer to ContentID
	Output: BOOL
==========================================================================*/
#ifdef FEATURE_WAP
int REP_StoreResource (void* pContext, pREPCONTENTSTRUCT pResource, 
						UINT32* piContentID, UINT32* piSize);
#else
BOOL REP_StoreResource (void* pContext, pREPCONTENTSTRUCT pResource, 
						UINT32* piContentID, UINT32* piSize);
#endif //#ifdef FEATURE_WAP



/*========================================================================
	REP_UpdateResource
==========================================================================
	The function replaces a resource in the repository, i.e., stores the 
	new data under the same ContentId. If the operation was successful, 
	TRUE is returned. Otherwise FALSE is returned. It is the caller's 
	responsibility to delete the memory after the function has returned.

	Note! The function sets the installation counter and the referring 
	counter to the values of the old resource. The installation counter
	is thereafter increased by one.

	If the function returns TRUE, the difference between the old size and
	the new size of the resource (in BYTES) is stored in the piSizeChanged 
	attribute (new size - old size).

	Input: void* (MUST be pREPCONTEXT), Data, and ContentID
	Output: BOOL
==========================================================================*/
#ifdef FEATURE_WAP
int REP_UpdateResource (void* pContext, pREPCONTENTSTRUCT pResource, 
						 UINT32 iContentID, INT32* piSizeChange);
#else
BOOL REP_UpdateResource (void* pContext, pREPCONTENTSTRUCT pResource, 
						 UINT32 iContentID, INT32* piSizeChange);
#endif //#ifdef FEATURE_WAP


						
/*========================================================================
	REP_GetContentWithID
==========================================================================
	The function retrieves the data from the repository, specified with 
	the ContentID. If the content is found and it is of the specified 
	type a pointer to a REPCONTENTTYPE struct is returned, otherwise NULL 
	is returned. It is the caller's responsibility to free the memory of 
	the struct.

	The function does a WSP_PreParseHeaders on the header data if the 
	content is a resource. If this operation fails, NULL is returned.
	THE RESOURCE IS ALSO REMOVED FROM THE MEMORY IF THIS HAPPENS.

	Input: void* (MUST be pREPCONTEXT), ContentID, Type
	Output: pREPCONTENTSTRUCT or NULL if error
==========================================================================*/
pREPCONTENTSTRUCT REP_GetContentWithID (void* pContext, UINT32 iContentID,
										UINT8 iType);


/*========================================================================
	REP_RootBlockSafe
==========================================================================
	This function must be called to inform the low level parts when 
	changes on the repository are being done. Before any changes can be 
	done (installation of channel or deletion of channel for example) the 
	function MUST be called with the parameter fSafe set to FALSE. When
	ALL changes are made, the function SHOULD be called again with the 
	fSafe parameter set to TRUE. When this happens, the root block in the
	repository is updated and set to status REPROOT_STATUS_OK if the 
	operation was successful. Otherwise the root block is removed.
  
	Input: void* (MUST be pREPCONTEXT), BOOL
	Output: -
==========================================================================*/
void REP_RootBlockSafe (void* pContext, BOOL fSafe);


/*========================================================================
	REP_DecreaseInstallCounter
==========================================================================
	The function decreases the install counter of the specified resource 
	with one and changes the status if necessary (e.g. if ref counter and 
	install counter both are 0, the	resource is removed). If the operation 
	is successful, TRUE is returned, otherwise FALSE is returned.
	
	Input: void* (MUST be pREPCONTEXT), ContentID
	Output: BOOL
==========================================================================*/
BOOL REP_DecreaseInstallCounter (void* pContext, UINT32 iContentID);


/*========================================================================
	REP_StoreChannel
==========================================================================
	The function stores a channel in the repository. If the operation was 
	successful, TRUE is returned and the input parameter piContentID is 
	set accordingly. Otherwise FALSE is returned. It is the caller's 
	responsibility to delete the memory after the function has returned.
	
	If the function returns TRUE, the size of the stored channel (in
	BYTES, NOT including the resources) is stored in the piSize attribute.
	The amount is also decucted from the reserved memory. 

	Input: void* (MUST be pREPCONTEXT), Channel data, and pointer to 
				 ContentID
	Output: BOOL
==========================================================================*/
#ifdef FEATURE_WAP
int REP_StoreChannel (void* pContext, pREPCONTENTSTRUCT pChannel, 
					   UINT32* piContentID, UINT32* piSize);
#else
BOOL REP_StoreChannel (void* pContext, pREPCONTENTSTRUCT pChannel, 
					   UINT32* piContentID, UINT32* piSize);
#endif //#ifdef FEATURE_WAP


/*========================================================================
	REP_DeleteChannel
==========================================================================
	The function removes the channel with the specified ID from the 
	repository. If the operation was successful, TRUE is returned, 
	otherwise FALSE is returned. 
	
	The function decreases the ref counter of all associated resources 
	with one if the channel state is *active*. If the state is 
	*installing*, the installing counter is decreased. The state of the
	resource is changed if necessary (e.g. if ref counter and install 
	counter both are 0, the resource is removed). 

	If any resources are removed the function creates a tMODIFYRESOURCE-
	element for each change needed in the root block. In that case, the 
	resource's ContentID, along with its hash value, is also removed from
	the context.
	
	Input: void* (MUST be pREPCONTEXT), ChannelID
	Output: BOOL
==========================================================================*/
BOOL REP_DeleteChannel (void* pContext, UINT32 iChannelID);


/*========================================================================
	REP_ActivateChannel
==========================================================================
	The function activates a channel in the repository with the specified 
	ID. If the operation was successful, TRUE is returned, otherwise FALSE 
	is returned. The function increases the ref counter of all associated 
	resources with one, decreases the installing counter with one, and 
	changes the state of the resources accordingly. 
	
	The function creates a tMODIFYRESOURCE-element for the channel. 

	Input: void* (MUST be pREPCONTEXT), ChannelID
	Output: BOOL
==========================================================================*/
BOOL REP_ActivateChannel (void* pContext, UINT32 iChannelID);


/*========================================================================
	REP_GetRootBlock
==========================================================================
	The function reads the Root Block from the repository and creates the
	temporary ChannelList in the Context. 
	
	If the function returns TRUE, the function REP_GetAllChannels should 
	be called to create a list of all channels. If the function returns 
	FALSE, the function REP_ScanRepository should be called to create the 
	list.

	Input: void* (MUST be pREPCONTEXT)
	Output: TRUE if the Root Block exists and if it is OK, FALSE otherwise
==========================================================================*/
BOOL REP_GetRootBlock (void* pContext);


/*========================================================================
	REP_ScanRepository
==========================================================================
	NOTE! The current implementation does NOT return channels that are
	*installing*. These channels are instead removed along with their 
	resources. This migth be subject to change.
  
	The function scans the memory for content and creates a list of all 
	channels in the repository with status *active* and *installing*. 
	(Channels that have expired are removed from the repository). The 
	list is of the type LISTHEADER and the elements in the list are of 
	the type REPCHSTRUCT. It is the caller's responsibility to deallocate 
	the memory. The function should be called repeatedly until it returns 
	FALSE. 
	
	During the scan of the memory, the temporary ChannelList in the 
	Context is updated. Also, the resources in the repository are updated 
	according to the *active* and *installing* channels. Before the 
	function returns the last time, the Root Block in the persistent 
	memory is updated. (The function also deletes the temporary 
	ChannelList before returning.) 

	NOTE! When the function is called the first time, ppContentList 
	should be NULL.

	Input: void* (MUST be pREPCONTEXT), void** (MUST be LISTHEADER**),
			   and current time
	Output: TRUE if the function should be called again, FALSE otherwise
==========================================================================*/
BOOL REP_ScanRepository (void* pContext, void** ppContentList, UINT32 iTime);


/*========================================================================
	REP_GetAllChannels
==========================================================================
	NOTE! The current implementation does NOT return channels that are
	*installing*. These channels are instead removed along with their 
	resources. This migth be subject to change.

	The function reads from the repository and creates a list of all 
	channels in the repository with status *active* or *installing*. 
	(based on the temporary ChannelList in the Context). Channels that 
	have expired are removed from the repository. The created list is of 
	the type LISTHEADER and the elements in the list are of the type 
	tREPCHSTRUCT. It is the caller's responsibility to deallocate the 
	memory. The function should be called repeatedly until it returns 
	FALSE. Before the function returns the last time, the temporary 
	ChannelList in the Context is deleted.

	NOTE! When the function is called the first time, ppContentList 
	should be NULL.

	Input: void* (MUST be pREPCONTEXT), void** (MUST be LISTHEADER**),
			   and current time
	Output: TRUE if the function should be called again, FALSE otherwise
==========================================================================*/
BOOL REP_GetAllChannels (void* pContext, void** ppContentList, UINT32 iTime);


/*========================================================================
	REP_GetSizeOfContent
==========================================================================
	Returns the size of the specified content.
  
	Input: void* (MUST be pREPCONTEXT), ContentID
	Output: UINT32, 0 if error
==========================================================================*/
UINT32 REP_GetSizeOfContent (void* pContext, UINT32 iContentID);


/*========================================================================
	REP_CompactRepository
==========================================================================
	This function organizes the memory with no (minimal) fragmentation. 
	The function is called when a compactation of the repository is 
	needed. The function is called, for example, when content is to be 
	stored and the available memory in the repository is sufficient, but 
	fragmented in a way so that the largest free block is smaller than 
	the content. To allow scheduling, the compact process is not neces-
	sarily completed with one function call. As long as the function 
	returns TRUE it is possible to compact the repository more.

	Input: void* (MUST be pREPCONTEXT)
	Output: TRUE if possible to compact more, FALSE otherwise
==========================================================================*/
BOOL REP_CompactRepository (void* pContext);


/*========================================================================
	REP_ResetRepMem
==========================================================================
	The function removes all content in the repository.

	Input: void* (MUST be pREPCONTEXT)
	Output: -
==========================================================================*/
void REP_ResetRepMem (void* pContext);


/*========================================================================
	REP_InitRepMem
==========================================================================
	The function creates a REPCONTEXT struct and initiates the fields in 
	the struct. NULL is returned if error.
	
	Input: Size of repository.
	Output: void* (type is pREPCONTEXT), NULL if error
==========================================================================*/
void* REP_InitRepMem (UINT32 iRepSize);


/*========================================================================
	REP_TerminateRepMem
==========================================================================
	The function removes the REPCONTEXT struct.
	
	Input: void* (MUST be pREPCONTEXT)
	Output: -
==========================================================================*/
void REP_TerminateRepMem (void* pContext);


/*========================================================================
	REP_GetNextChToRemove
==========================================================================
	The function returns the id of the channel that is about to be removed
	next (with lowest value of expiry date). If there is no channel with
	the expiry date attribute set, 0 is returned.
	
	If a channel is found, the pliExpiryDate parameter is set with the 
	expiry date.

	Input: void* (MUST be pREPCONTEXT), set pliExpiryDate to zero if no 
		   channel to remove.
	Output: LLIdToRemove, set to zero if no ch to remove.
==========================================================================*/
UINT32 REP_GetNextChToRemove (void* pContext, unsigned long int* pliExpiryDate );


/*========================================================================
	REP_SetServiceAccessible
==========================================================================
	The function changes the ServiceAccessible flag.

	Input: void* (MUST be pREPCONTEXT), ContentID, status
	Output: TRUE if successful
==========================================================================*/
BOOL REP_SetServiceAccessible (void* pContext, UINT32 iContentId, 
							  BOOL status);
#endif
