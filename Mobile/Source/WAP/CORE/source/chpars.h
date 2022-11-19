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
#ifndef _ChPars_H
#define _ChPars_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: ChPars.h

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	990803	CEF		First version
	990811	JPR		Updated with functions
	000207	JPR		Updated to support new WBXML-decoder
	000301	JPR		Correction in Ch_CheckResourceEtag
	001011	ASPN	Lower-case filenames

=========================================================================*/

/* Private include statements */

#include "cmmnrsrc.h"
#include "wbxmldef.h"
#include "hdrutil.h"

/*========================================================================
	Ch_BuildStructure
==========================================================================
Purpose:  Function to build the element structure needed to perform 
		  different operations. The returned value is the structure, or 
		  NULL if error. The pbData is left unchanged. It is the caller's 
		  responsibility to delete the structure after it is used. The 
		  parameters are as follows: 

		  pbData - pointer to the first byte in the indata.
		  pbEnd - pointer to the byte AFTER the last byte in the indata.
		  iCharset - integer containing the IANA-code of the charset.
		  iContentLevel - The level (WSP parameter) of the contenttype.
		  iViewID - The view for the user agent (used for error output)
======================================================================== */
pDECSTR Ch_BuildStructure (BYTE* pbData, BYTE* pbEnd, INT16 iCharset, 
						   UINT8 iContentLevel, UINT8 iViewID);


/*========================================================================
	Ch_DeleteStructure
==========================================================================
Purpose:  Function to delete a Channel structure. The input (**) struct 
		  must be of the type DECSTR.
======================================================================== */
void Ch_DeleteStructure (pDECSTR* ppDecStr);


/*========================================================================
	Ch_GetAbstract
==========================================================================
Purpose:  The function returns a copy of the text in the abstract in the 
		  channel. NULL is returned if no abstract is present. The string 
		  MUST be deleted after it is used.
======================================================================== */
WCHAR* Ch_GetAbstract (pDECSTR pDecStr);


/*========================================================================
	Ch_GetTitle
==========================================================================
Purpose:  The function returns a copy of the title in the channel. NULL 
		  is returned if no title is present. The string MUST be deleted 
		  after it is used.
======================================================================== */
WCHAR* Ch_GetTitle (pDECSTR pDecStr);

/*========================================================================
	Ch_Getuseraccessible
==========================================================================
Purpose:  The function returns the value of the useraccessible attribute.
======================================================================== */
BOOL Ch_Getuseraccessible (pDECSTR pDecStr);


/*========================================================================
	Ch_GetSuccessUrl
==========================================================================
Purpose:  The function returns a copy of the successurl in the channel. 
		  NULL is returned if no successurl is present. The string MUST 
		  be deleted after it is used.
======================================================================== */
BYTE* Ch_GetSuccessUrl (pDECSTR pDecStr);


/*========================================================================
	Ch_GetFailureUrl
==========================================================================
Purpose:  The function returns a copy of the failure url in the channel. 
		  NULL is returned if no failure url is present. The string MUST 
		  be deleted after it is used.
======================================================================== */
BYTE* Ch_GetFailureUrl (pDECSTR pDecStr);


/*========================================================================
	Ch_GetBaseUrl
==========================================================================
Purpose:  The function returns a copy of the baseurl in the channel. 
		  NULL is returned if no baseurl is present. The string MUST be 
		  deleted after it is used.
======================================================================== */
BYTE* Ch_GetBaseUrl (pDECSTR pDecStr);


/*========================================================================
	Ch_GetEventID
==========================================================================
Purpose:  The function returns a copy of the EventID in the channel. 
		  NULL is returned if no EventID is present. The string MUST be 
		  deleted after it is used.
======================================================================== */
WCHAR* Ch_GetEventID (pDECSTR pDecStr);

/*========================================================================
	Ch_Getchannelid
==========================================================================
Purpose:  The function returns a copy of the channelid in the channel. 
		  NULL is returned if no channelid is present. The string MUST be 
		  deleted after it is used.
======================================================================== */
WCHAR* Ch_Getchannelid (pDECSTR pDecStr);

/*========================================================================
	Ch_GetMaxSpace
==========================================================================
Purpose:  The function returns the maxspace attribute in the channel. A 
		  negative number is returned if the maxspace attribute is not 
		  specified.
======================================================================== */
INT32 Ch_GetMaxSpace (pDECSTR pDecStr);


/*========================================================================
	Ch_CheckIfChExpired
==========================================================================
Purpose:  Check if the content has expired
======================================================================== */
BOOL Ch_CheckIfChExpired (HEADERDEF* pContentHeader, UINT32 iTime);


/*========================================================================
	Ch_GetNbrOfResources
==========================================================================
Purpose:  The function returns the number of resources in the channel.
======================================================================== */
UINT32 Ch_GetNbrOfResources (pDECSTR pDecStr);


/*========================================================================
	Ch_GetFirstResource
==========================================================================
Purpose:  The first resource in the element structure is returned. NULL 
		  is returned if none is present. The element MUST NOT be modified.
======================================================================== */
pELEMENTTYPE Ch_GetFirstResource (pDECSTR pDecStr);


/*========================================================================
	Ch_GetNextResource
==========================================================================
Purpose:  The next resource in the channel is returned. In parameter is 
		  the previous resource (e.g., the result of a Ch_GetFirst-
		  Resource function call). NULL is returned if no more resources. 
		  The element MUST NOT be modified.
======================================================================== */
pELEMENTTYPE Ch_GetNextResource (pELEMENTTYPE pResource);


/*========================================================================
	Ch_GetResourcehref
==========================================================================
Purpose:  Returns a copy of the href of a specific resource. The string 
		  MUST be deleted after it is used. If the href is not specified, 
		  NULL is returned.
======================================================================== */
BYTE* Ch_GetResourcehref (pELEMENTTYPE pResource);


/*========================================================================
	Ch_GetResourceLastMod
==========================================================================
Purpose:  Returns zero if no lastMod is specified.
======================================================================== */
UINT32 Ch_GetResourceLastMod(pELEMENTTYPE pResource);


/*========================================================================
	Ch_GetResourceMd5
==========================================================================
Purpose:  Returns a copy of the md5 attribute of a specific resource. 
		  The string MUST be deleted after it is used. If the md5 
		  attribute is not specified, NULL is returned.
======================================================================== */
WCHAR* Ch_GetResourceMd5 (pELEMENTTYPE pResource);


/*========================================================================
	Ch_GetResourceEtag
==========================================================================
Purpose:  Returns a copy of the etag attribute of a specific resource. 
		  The string MUST be deleted after it is used. If the etag 
		  attribute is not specified, NULL is returned.
======================================================================== */
BYTE* Ch_GetResourceEtag (pELEMENTTYPE pResource);


/*========================================================================
	Ch_IsResourceFresh
==========================================================================
Purpose:  The function compares the lastmod attribute of the specified 
		  WSP-header with the iTime parameter. If the content is fresh 
		  (consult RFC 2068 for details), TRUE is returned. Otherwise, 
		  FALSE is returned.

		  If no Md5-header is found in the list the parameter pfCheckDone
		  is set to FALSE. Otherwise it is set to TRUE.		

NOTE!	  The parameter pHeaderList is a LIST of headerelements that
		  has to be traversed (i.e., the pHeadList pointer in the 
		  HEADERDEF struct).
======================================================================== */
BOOL Ch_IsResourceFresh (pHEADERELEMENT pHeaderList, UINT32 iTime,
						 BOOL* pfCheckDone);


/*========================================================================
	Ch_CheckResourceEtag
==========================================================================
Purpose:  The function compares the Etag attribute of the specified WSP-
		  header with the etag parameter. If they are equal, TRUE is 
		  returned, otherwise, FALSE is returned. Consult RFC 2068 for 
		  details. The pchEtag string is not modified by the function.

		  If no Etag-header is found in the list the parameter pfCheckDone
		  is set to FALSE. Otherwise it is set to TRUE.		

NOTE!	  The parameter pHeaderList is a LIST of headerelements that
		  has to be traversed (i.e., the pHeadList pointer in the 
		  HEADERDEF struct).
======================================================================== */
BOOL Ch_CheckResourceEtag (pHEADERELEMENT pHeaderList, BYTE *pbEtag, 
						   BOOL* pfCheckDone);


/*========================================================================
	Ch_CheckResourceMd5
==========================================================================
Purpose:  The function compares the md5 attribute of the specified WSP-
		  header with the md5 parameter. If they are equal, TRUE is 
		  returned, otherwise, FALSE is returned. Consult RFC 2068 for 
		  details. The pchMd5 string is not modified by the function.

		  If no Md5-header is found in the list the parameter pfCheckDone
		  is set to FALSE. Otherwise it is set to TRUE.		

NOTE!	  The parameter pHeaderList is a LIST of headerelements that
		  has to be traversed (i.e., the pHeadList pointer in the 
		  HEADERDEF struct).
======================================================================== */
BOOL Ch_CheckResourceMd5 (pHEADERELEMENT pHeaderList, WCHAR* pchMd5,
						  BOOL* pfCheckDone);


#endif
