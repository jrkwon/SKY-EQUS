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
/* ==================================================================
	FILE: hdrutil.h
  
	Utilities for Header Encoding/Decoding for WAP.
	Header file.

	Rev history:
		980923	JPR		Created
		990406	JPR		Corrections
		990408	JPR		Function create header Accept-language added
		990409	JPR		URL-functions removed
		990412	JPR		Uses Url.h instead of Url_Lib.h
						Macros used for character classification.	
		990416	JPR		Authentication functions updated (TR 64)
		990510	JPR		WSP_GetContentTypeByte updated
		990526	JPR		Corrections
		990815	JPR		Additions to support channel decoding
  	990825	JPR		Changes in CreateHeaderField
		990826	JPR		WSP_GetContentTypeString added
		991112	JPR		ADDRESS functions added
		000110	IPN		Moved Authentication functions to authoriz.h
		000111	JPR		Multipart functions updated
		000114	JPR		WSP Parameter handling changed.
		000117	JPR		Multipart functions added
		000121	JPR		WSP_GetEtag added, WSP_CreateHeaderField
						function updated
		000126	JPR		Comments added to WSP_CreateHeaderField
						Function WSP_GetProfileWarning added
						WSP_DeleteMultipartListWithContent added
		000211	JPR		WSP_CreateHeaderField updated to support 
						Accept Application and Bearer Indication.
						Function WSP_GetEtagWCHAR removed.
		000301	JPR		Function WAE_AddGenericFormDataPart implemented
		001106	JPR		Function WSP_GetPushHeaderAppId implemented
		001114  HEAD    Function WSP_BuildMultipart has a new parameter
						iTotalSize.

 ================================================================== */
   
#ifndef CWAPHDRUTIL_H
#define CWAPHDRUTIL_H

#include "headdef.h"
#include "cmmnrsrc.h"
#include "wmldef.h"
#include "url.h"
#include "waectype.h"

/* ---------------------------------------------
	Descr : Creates a HEADERDEF element with
			the specified attributes.
	Input : Pointer to BYTE, Length of stream.
	Output:	Pointer to an Element, NULL if error
   --------------------------------------------- */
pHEADERDEF CreateHEADERDEF (BYTE *pbInstream, UINT32 iLength);

/* ---------------------------------------------
	Descr : Deletes a HEADERDEF element 
	Input : Pointer to HEADERDEF
	Output:	-
   --------------------------------------------- */
void DeleteHEADERDEF (pHEADERDEF pDef);
					
/* ---------------------------------------------
	Descr : Copies a byte stream with the 
			specified length. If the iLen is 
			negative the stream is terminated 
			with 0 and the length is determined 
			in the function.
	Input : String and length
	Output:	Copy of string, NULL if error
   --------------------------------------------- */
BYTE *B_CopyByteString (BYTE *pbString, INT32 iLen);


	
/* ========= HEADER ELEMENT FUNCTIONS ==========
	Functions for handling the header elements.
   ============================================= */
/* ---------------------------------------------
	Descr : Creates an element with the 
			specified values.
	Input : Header Field Name, pointer to BYTE
	Output:	Pointer to an Element, NULL if error
   --------------------------------------------- */
pHEADERELEMENT CreateHeaderElement (BYTE bCodePage, BYTE bFieldName, 
									BYTE *pbFieldName, BYTE *pbStart, BYTE *pbEnd);

/* ---------------------------------------------
	Descr : Add the element to the list.
	Input : Pointer to an Element, pDef
	Output:	TRUE if OK, FALSE otherwise
   --------------------------------------------- */
void AddHeaderElement (pHEADERDEF pDef, pHEADERELEMENT pHeadElm);

/* ============================================= */




/* =========== PARSER HELP FUNCTIONS =========== 
	  Help functions used during the parsing.
   ============================================= */
BOOL HdrNextByte (BYTE **pbStart, BYTE *pbEnd);

/* ---------------------------------------------
	Descr : Calculates the length of the field-
			value and steps in the stream.
	Input :	Pointer to HEADERDEF
	Output:	TRUE if OK, FALSE if error			
   --------------------------------------------- */
BOOL StepFieldLength (BYTE **ppbStart, BYTE *pbEnd);
/* ============================================= */



/* ============= PARSER FUNCTIONS ============== 
	   Functions used during the header parse.
   ============================================= */

/*	The following macros/functions with a signature 
	looking like 

		isFooCh (c) / BOOL isFooCh(BYTE bChar)

	determines if the character is of the specified
	type. If so, TRUE is returned, otherwise FALSE
	is returned. The macros/functions does NOT modify 
	anything.									
*/

/* MACRO DEFINITIONS */
#define isWapCtlCh(c)			((c<=31)||(c==127))
#define isWapEndOfStringCh(c)	(c==0)
#define isWapQuoteCh(c)			(c==34)
#define isWapCRCh(c)			(c==13)
#define isWapLFCh(c)			(c==10)
#define isWapHTCh(c)			(c==9)
#define isWapSPCh(c)			(c==32)
#define isWapShortLengthCh(c)	(c<=30)
#define isWapLengthQuoteCh(c)	(c==31)
#define isWapCharCh(c)			(c<=127)
#define isWapShortIntegerCh(c)	(c & 128)
#define isWapBasicCh(c)			(c==128)

BOOL isWapTokenCh(BYTE bChar);

/*	General remark for all functions with a 
	signature looking like 
	
	  BOOL isFoo(BYTE **pbStart, BYTE *pbEnd)

	If the method returns TRUE the pbStart in is 
	updated to point at the position directly after 
	where the item Foo that was found in the input 
	string. If the method returns FALSE, then pbStart
	in has the same value at exit as it had on entry.
	pbEnd points to the first byte AFTER the valid
	bytestream.	Similar action takes place for the
	functions looking like 							

	  BOOL isFoo(pHEADERDEF pDef)

	but the fields in pDef is used instead.		  */

/* CR LF */
BOOL isWapCRLF(BYTE **ppbStart, BYTE *pbEnd);

/* [CRLF] 1*( SP | HT ) */
BOOL isWapLWS(BYTE **ppbStart, BYTE *pbEnd);

/* <any OCTET except CTLs but including LWS> */
BOOL isWapTEXT(BYTE **ppbStart, BYTE *pbEnd);

/* 1*<any CHAR except CTLs or tspecials> */
BOOL isWapToken (BYTE **ppbStart, BYTE *pbEnd);

/* <OCTET 127> */
BOOL isWapShiftDel (BYTE **ppbStart, BYTE *pbEnd);

/* <Any OCTET 1-31> */
BOOL isWapShortCutShiftDel (BYTE **ppbStart, BYTE *pbEnd);


/* ---------------- BASIC RULES ---------------- */

/* [Quote] *TEXT End-of-string */
BOOL isWapTextString(BYTE **ppbStart, BYTE *pbEnd);     

/* Token End-of-string */
BOOL isWapTokenText(BYTE **ppbStart, BYTE *pbEnd);

/* *TEXT End-of-string */
BOOL isWapExtensionMedia(BYTE **ppbStart, BYTE *pbEnd);     

/* <Octet 34> *TEXT End-of-string */
BOOL isWapQuotedString(BYTE **ppbStart, BYTE *pbEnd);     

/* OCTET (1xxx xxxx) */
BOOL isWapShortInteger (BYTE **ppbStart, BYTE *pbEnd);

/* <ANY OCTET 0-30> 1*30 OCTET */
BOOL isWapLongInteger (BYTE **ppbStart, BYTE *pbEnd);

/* <Octet 127> */
BOOL isWapQuote(BYTE **ppbStart, BYTE *pbEnd);

/* --------------------------------------------- */



/* --------------- LENGTH RULES ---------------- */

/* Short-length | (Length-quote Length) */
BOOL isWapValueLength (BYTE **ppbStart, BYTE *pbEnd);

/* <Any Octet 0-30> */
BOOL isWapShortLength(BYTE **ppbStart, BYTE *pbEnd);

/* <Octet 31> */
BOOL isWapLengthQuote(BYTE **ppbStart, BYTE *pbEnd);

/* --------------------------------------------- */



/* ----------- PARAMETER VALUE RULES ----------- */

/* <Octet 0> */
BOOL isWapNoValue(BYTE **ppbStart, BYTE *pbEnd);

/* No-Value | Token-text | Quoted-string */
BOOL isWapTextValue(BYTE **ppbStart, BYTE *pbEnd);

/* Short-integer | Long-integer */
BOOL isWapIntegerValue (BYTE **ppbStart, BYTE *pbEnd);

/* 1*2 Tokens ---- NOT IMPLEMENTED ---- */
BOOL isWapQValue(BYTE **ppbStart, BYTE *pbEnd);

/* Short-integer | Text-string */
BOOL isWapVersionValue(BYTE **ppbStart, BYTE *pbEnd);

/* --------------------------------------------- */



/* -------------- PARAMETER RULES -------------- */

/* Compact-value | Text-value */
BOOL isWapTypedValue(BYTE **ppbStart, BYTE *pbEnd);

/* Integer-value | Date-value | Q-value | Version-value | Uri-value */
BOOL isWapCompactValue(BYTE **ppbStart, BYTE *pbEnd);

/* Integer-value | Text-value */
BOOL isWapUnTypedValue(BYTE **ppbStart, BYTE *pbEnd);

/* --------------------------------------------- */



/* ------------ AUTHORIZATION RULES ------------ */

/* Token-text */
BOOL isWapAuthenticationScheme (BYTE **ppbStart, BYTE *pbEnd);

/* <OCTET 128> */
BOOL isWapBasic (BYTE **ppbStart, BYTE *pbEnd);

/* --------------------------------------------- */



/* ====================== HEADER PARSE FUNCTIONS =========================
					Functions used to parse the headers.
   ======================================================================= */

/*========================================================================
	WSP_PreParseHeaders 
==========================================================================
	Finds the field names in the header and stores the pointers to the in 
	a linked list. The pointers are to a COPY of the inputted data. The 
	error-flag is set if error.
	
	Input :	The header as a BYTE *, number of bytes and pointer to BOOL.
	Output:	Creates a HEADERDEF and returns a pointer to it 
			(consult HeadDef.h).
==========================================================================*/
pHEADERDEF WSP_PreParseHeaders (BYTE *pbInstream, INT32 iLength, BOOL *fError);


/*========================================================================
	WSP_EmptyHeaderList 
==========================================================================
	Descr : Deletes the element list and frees the memory.

	Input : pDef
	Output:	-
==========================================================================*/
void WSP_EmptyHeaderList (pHEADERDEF pDef);


/*========================================================================
	WSP_DeleteParameters 
==========================================================================
	Removes a tWSPPARAMETERS struct.

	Input : (**) to tWSPPARAMETERS
	Output:	-
==========================================================================*/
void WSP_DeleteParameters (pWSPPARAMETERS* ppParameters);


/*========================================================================
	WSP_GetHeaderWK 
	WSP_GetHeaderTT 
==========================================================================
	The function returns a pointer to the element specified	by the 
	parameters. Some headers might exist in more than one instance, e.g. 
	Accept-language. To get all the elements, the function should be 
	called repeatedly and the pFirstElm should be changed gradually.

	Input : The desired field as a short-int (first	function) or a 
			Token-Text (second function). The pFirstElm points to the 
			first element in the list to be examined.
	Output:	Pointer to header element, NULL if not found.
==========================================================================*/

/* Well-Known header */
pHEADERELEMENT WSP_GetHeaderWK (BYTE bCodePage, BYTE bByte, 
							pHEADERELEMENT pFirstElm);
/* Token-Text header */
pHEADERELEMENT WSP_GetHeaderTT (BYTE *pbTokenText, pHEADERELEMENT pFirstElm);


/* Various functions */
BYTE WSP_GetShortInt (BYTE bShortInt);
UINT32 WSP_GetLongInteger (BYTE **ppbStart, BYTE *pbEnd, BOOL *pfError);
UINT32 WSP_GetInteger (BYTE **ppbStart, BYTE *pbEnd, BOOL *pfError);
BYTE* WSP_GetTextString (BYTE **ppbStart, BYTE *pbEnd, BOOL *pfError);


/*========================================================================
	WSP_GetContentTypeByte 
==========================================================================
	The function returns the Content-type value of the HEADERELEMENT. If 
	the value includes parameters (e.g. charset or level), a struct of the 
	type tWSPPARAMETERS is created and the values in the struct are set 
	accordingly. The iCharset is set with the IANA value of the charset. 
	If no charset parameter is included the iCharset is set to 
	IANA_INVALID. The iLevel is set in the same way with the default value 
	0.

	NOTE! If no parameters are specified, the pParameters is set to NULL.
	If one or more parameters are found, these are stored in the struct.
	Parameters that are not specifed are given their defualt values. The
	pParameters struct MUST be deleted with a call to the function
	WSP_DeleteParameters.
	
	Input: Pointer to HEADERELEMENT, pointer to error flag (set to FALSE),
		   and a (**) to (WSPPARAMETERS) - set to NULL if no parameters
	Output: BYTE, The error flag is set if error.
==========================================================================*/
BYTE WSP_GetContentTypeByte (pHEADERELEMENT pElement, BOOL *fError, 
							 pWSPPARAMETERS* ppParameters);

/*========================================================================
	WSP_GetContentTypeString
==========================================================================
	As above, but returns string. It is the caller's responsibility to de-
	allocate the memory of the string.	

	Input: Pointer to HEADERELEMENT, pointer to error flag (set to FALSE),
		   and a (**) to (WSPPARAMETERS) - set to NULL if no parameters
	Output: BYTE*, The error flag is set if error.
==========================================================================*/
BYTE *WSP_GetContentTypeString (pHEADERELEMENT pElement, BOOL *fError, 
								pWSPPARAMETERS* ppParameters);


/* ---------------------------------------------
	Descr : Returns the Cache-control value of
			the HEADERELEMENT. If Delta-seconds
			are specified, they are stored in the
			piDeltaSec parameter.
	Input :	Pointer to HEADERELEMENT, pointer
			to error flag, pointer to UINT32.
	Output:	BYTE, The error flag is set if error
			(e.g. if the value is not a BYTE)
   --------------------------------------------- */
BYTE WSP_GetCacheControlByte (pHEADERELEMENT pElement, UINT32 *piDeltaSec,
									BOOL *fError);

/* ---------------------------------------------
	Descr : Returns a Basic Realm-value.
	Input :	Pointer to HEADERELEMENT
	Output:	Pointer to text. (Not copied)
	        NULL if error.
   --------------------------------------------- */
BYTE *WSP_GetProxyAuthenticateRealm (pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns a Basic Realm-value.
	Input :	Pointer to HEADERELEMENT
	Output:	Pointer to text. (Not copied)
	        NULL if error.
   --------------------------------------------- */
BYTE *WSP_GetWWWAuthenticateRealm (pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns the value of the Age field
			as an UINT32. If error the Error flag 
			is set.
	Input :	Pointer to HEADERELEMENT, pointer to
			flag (set to FALSE).
	Output:	UINT32	        
   --------------------------------------------- */
UINT32 WSP_GetAge (pHEADERELEMENT pElement, BOOL *pfError);

/* ---------------------------------------------
	Descr : Returns the value of the Date field
			as an UINT32. If error the Error flag 
			is set.
	Input :	Pointer to HEADERELEMENT, pointer to
			flag (set to FALSE).
	Output:	UINT32	        
   --------------------------------------------- */
UINT32 WSP_GetDate (pHEADERELEMENT pElement, BOOL *pfError);

/* ---------------------------------------------
	Descr : Returns an Etag-value.
	Input :	Pointer to HEADERELEMENT
	Output:	Pointer to text. (Not copied)
	        NULL if error.
   --------------------------------------------- */
BYTE *WSP_GetEtag (pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns the value of the Expires field
			as an UINT32. If error the Error flag 
			is set.
	Input :	Pointer to HEADERELEMENT, pointer to
			flag (set to FALSE).
	Output:	UINT32	        
   --------------------------------------------- */
UINT32 WSP_GetExpires (pHEADERELEMENT pElement, BOOL *pfError);

/* ---------------------------------------------
	Descr : Returns the value of the Last Modified
			field as an UINT32. If error the Error 
			flag is set.
	Input :	Pointer to HEADERELEMENT, pointer to
			flag (set to FALSE).
	Output:	UINT32	        
   --------------------------------------------- */
UINT32 WSP_GetLastModified (pHEADERELEMENT pElement, BOOL *pfError);

/* ---------------------------------------------
	Descr : Returns a Location-value.
	Input :	Pointer to HEADERELEMENT
	Output:	Pointer to text. (Not copied)
	        NULL if error.
   --------------------------------------------- */
BYTE *WSP_GetLocation (pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns a Content-location-value.
	Input :	Pointer to HEADERELEMENT
	Output:	Pointer to text. (Not copied)
	        NULL if error.
   --------------------------------------------- */
BYTE *WSP_GetContentLocation (pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns a Content-base-value.
	Input :	Pointer to HEADERELEMENT
	Output:	Pointer to text. (Not copied)
	        NULL if error.
   --------------------------------------------- */
BYTE *WSP_GetContentBase (pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns the value of the Push Flag
			field as an UINT8. 
	Input :	Pointer to HEADERELEMENT, pointer to
			flag (set to FALSE).
	Output:	UINT8	        
   --------------------------------------------- */
UINT8 WSP_GetPushFlag (pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns a X-Wap-Initiator-URI.
	Input :	Pointer to HEADERELEMENT
	Output:	Pointer to text. (Not copied)
	        NULL if error.
   --------------------------------------------- */
BYTE *WSP_GetXInitiatorURI(pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns a X-Wap-Content-URI.
	Input :	Pointer to HEADERELEMENT
	Output:	Pointer to text. (Not copied)
	        NULL if error.
   --------------------------------------------- */
BYTE *WSP_GetXContentURI (pHEADERELEMENT pElement);

/* ---------------------------------------------
	Descr : Returns a Profile-warning code
			according to WSP (8.4.2.62)
	Input :	Pointer to HEADERELEMENT
	Output:	BYTE, 0 if error
   --------------------------------------------- */
BYTE WSP_GetProfileWarning (pHEADERELEMENT pElement);


/*========================================================================
	WSP_GetPushHeaderAppId
==========================================================================
	Descr : Returns TRUE if the decoding of the X-WAP-Application-Id is 
					successful, FALSE if not. 
					
					Sets URIHeaderAppId = X-WAP-Application-Id if it is an URI, OR
					CodeHeaderAppId = X-WAP-Application-Id and URIHeaderAppId = NULL

	Input :	Pointer to HEADERELEMENT, adress to URIHeaderAppld and
					CodeHeaderAppId
	Output:	TRUE/FALSE, URIHeaderAppId or CodeHeaderAppId is set if TRUE
					is returned (NULL and 0 otherwise)
==========================================================================*/
BOOL WSP_GetPushHeaderAppId (pHEADERELEMENT pElement, BYTE** ppbURIHeaderAppId, int* piCodeHeaderAppId);


/*========================================================================
	WSP_GetMd5
==========================================================================
	The function returns a stream of 16 bytes containing the md5-data of
	the header. The	stream MUST NOT be deallocated.
	
	If error, NULL is returned and the error flag is set.
	
	Input :	Pointer to HEADERELEMENT, pointer to flag.
	Output:	BYTE* or NULL
==========================================================================*/
BYTE* WSP_GetMd5 (pHEADERELEMENT pElement, BOOL *pfError);


/*========================================================================
	WSP_ConvertBase64ToByte
==========================================================================
	The function converts a WCHAR-string containing characters from the
	'Base64 Alphabeth' (see RFC 1521, section 5.2) into an bytestream. The
	length of the created stream is stored in the parameter piNbrBytes.
	If error, NULL is returned and the parameter piNbrBytes is set to -1.

	NOTE! It is the callers responsibility to deallocate the memory.
	
	Input :	Pointer to WCHAR, pointer to INT16.
	Output:	BYTE* or NULL
==========================================================================*/
BYTE* WSP_ConvertBase64ToByte (WCHAR* pwchMd5, INT16* piNbrBytes);




/* ============ ADDRESS FUNCTIONS ==============
	Functions for handling address parsing.
   ============================================= */

/* ---------------------------------------------
	Descr : The function parses an instream for
			address data according to the WSP
			specification section 8.2.2.3. If the
			function fails for any reason, FALSE
			is returned and the *ppAddress parameter
			is set to NULL. Otherwise the ppAddress
			includes all addresses in the instream.

			If the Bearer Type or the Port Number
			is not specified, these fields are set
			to -1 in the ADDRESS struct.
		
			NOTE! It is the caller's responsibility
			to delete all data. The ADDRESS-structs
			will be useless if the pbData is deleted.

	Input :	Data (IN), length of data (IN) and 
			ADDRESS** (OUT)
	Output:	TRUE/FALSE, *ppAdress is set if 
			TRUE is returned (NULL otherwise)
   --------------------------------------------- */
BOOL WSP_ParseAddress( BYTE* pbData, INT32 iLength, pADDRESS* ppAddress );


/* ---------------------------------------------
	Descr : The function deletes a list of ADDRESS-
			structs, starting with the specified one.

	Input :	ADDRESS** (The first element in the list
			to be deleted)
	Output:	-
   --------------------------------------------- */
void WSP_DeleteAddress( pADDRESS* ppAddress );


/* ============================================= */




/* ============= CONVERT FUNCTIONS =============
	   Functions for coverting from bytes to
	   strings and vice versa.
   ============================================= */
/* ---------------------------------------------
	Descr : Converts the value of the Content Type
			Value from a character string to a BYTE
			or vice versa. If *ppbText is NULL the byte
			is converted to a string. If the string 
			is not NULL, it is converted to a BYTE.
			The string is NOT deallocated. If the 
			operation was succesful, the result is
			stored in the appropriate field (pbValue
			or ppbText)
	Input :	Pointer to BYTE and pointer to BYTE *
	Output:	TRUE if OK, FALSE otherwise.
   --------------------------------------------- */
BOOL WSP_ConvertContentStringByte(BYTE *pbValue, BYTE **ppbText);

/* ============================================= */




/* ======================== MULTIPART FUNCTIONS ==========================
			Functions for parsing and creating multipart data.
========================================================================== */

/*========================================================================
	WSP_PreParseMultipart
==========================================================================
	This function parses the body of a multipart entity and returns a list
	of MULTIPARTSTR (consult HeadDef.h). The pbInstream must NOT be 
	deallocated until the list is deleted.
	
	Input: Pointer to BYTEstream and length.
	Output: Pointer to MULTIPARTSTR, NULL if error.
==========================================================================*/
pMULTIPARTSTR WSP_PreParseMultipart (BYTE *pbInstream, UINT32 iLength);


/*========================================================================
	WSP_DeleteMultipartList
==========================================================================
	This function deletes a list of MULTIPARTSTR (consult HeadDef.h).

	NOTE! The data in the stucts is NOT deallocated!
	
	Input: pointer to pMULTIPARTSTR
	Output: -
==========================================================================*/
void WSP_DeleteMultipartList (pMULTIPARTSTR* ppList);


/*========================================================================
	WSP_DeleteMultipartListWithContent
==========================================================================
	This function deletes a list of MULTIPARTSTR (consult HeadDef.h).

	NOTE! The data in the stucts IS deallocated!
	
	Input: pointer to pMULTIPARTSTR
	Output: -
==========================================================================*/
void WSP_DeleteMultipartListWithContent (pMULTIPARTSTR* ppList);


/*========================================================================
	WSP_MultipartAddPart
==========================================================================
	This function creates an instance of a MULTIPARTSTR, and stores the
	appropriate values in the struct. The created struct is then stored 
	last in the list. The list is transformed into multipart content with 
	a call to the function WSP_BuildMulipart.

	NOTE! It is the caller's responsibility to deallocate the created 
	list.
	
	NOTE! The allocated memory in the parameters ppbHeader and ppbData 
	is taken over by the function, i.e., the parameters are set to point 
	to NULL before the function returns.

	NOTE! ppList, ppbHeader, and ppbData MUST not be NULL.
	
	Input: Pointer to list of (MULTIPARTSTR), pointer to header data and 
		   content data (BYTE**), and length of header data and content 
		   data (UINT32). 
	Output: -
==========================================================================*/
void WSP_MultipartAddPart (pMULTIPARTSTR* ppList, BYTE** ppbHeader, 
						   BYTE** ppbData, UINT32 iHeaderLen, UINT32 iDataLen);


/*========================================================================
	WSP_BuildMultipart
==========================================================================
	This function creates multipart content from a list of MULTIPARTSTR
	structs. If the transformation is successful, TRUE is returned and the
	list is removed. Otherwise FALSE is returned (and the list is left
	unchanged). 
	
	The created data is stored in the ppbContentData, and the length of 
	the data is stored in the piDataLen parameter. It is the caller's
	responsibility to deallocate the memory.
		
	Input: Pointer to list of (MULTIPARTSTR), pointer to created multipart 
		   data (BYTE**) and length of the data (UINT32). When using Large 
		   Data transfer and multipart the total data length (iTotalSize) 
		   is used. Otherwise iTotalSize should be set to 0.
	Output: TRUE if the transformation was ok, FALSE otherwise.
==========================================================================*/
BOOL WSP_BuildMultipart (pMULTIPARTSTR* ppList, BYTE** ppbContentData,
						 UINT32* piDataLen, UINT32 iTotalSize);


/*========================================================================
	The following functions use the functions above to create valid WSP
	multipart content parts. When all parts are created, they must be 
	assembled by a call to the WSP_BuildMultipart function. Before the 
	content is sent, a header for the multipart content must also be 
	created, using the "HEADER BUILD HELP FUNCTIONS" below. 
==========================================================================*/


/*========================================================================
	WAE_AddTextFormDataPart
==========================================================================
	This function creates one part of a multipart/form-data and stores 
	the result last in the list ppMultiPartList (MULTIPARTSTR). A WSP
	header is created and added for each part. The list is transformed 
	into multipart content with a call to the function WSP_BuildMulipart.
	The following WSP headers are added to each part:
	
	Content-type : text/plain with charset as indicated by the parameter 
		(iCharEnc). "Charset is required when the content contains 
		characters not in the US-ASCII characer set" (WML 9.5.1)
	
	Content-Disposition : form-data with parameter name as indicated by 
		the ppbNameData.

	NOTE! It is the caller's responsibility to deallocate the created 
	list.
	
	NOTE! The allocated memory in the parameter ppbValueData is taken 
	over by the function (the parameter is set to point to NULL before 
	the function returns).

  	NOTE! ppMultipartList, ppbNameData, and ppbValueData MUST NOT be NULL.

	Input: Pointer to list (MULTIPARTSTR**), Name (BYTE*), Value (BYTE**),
		   Value length (UINT32) and character encoding IANA nbr (INT16) 
	Output: -
==========================================================================*/
/*  Modified by GBU,000510, use type
void WAE_AddTextFormDataPart (void** ppMultiPartList, BYTE* pbNameData,  
							  BYTE** ppbValueData, UINT32 iValueLen, INT16 iCharEnc);
*/
void WAE_AddTextFormDataPart (pMULTIPARTSTR* ppMultiPartList, BYTE* pbNameData,  
                              BYTE** ppbValueData, UINT32 iValueLen, INT16 iCharEnc);


/*========================================================================
	WAE_AddGenericFormDataPart
==========================================================================
	This function creates one part of a multipart/form-data and stores 
	the result last in the list ppMultiPartList (MULTIPARTSTR). A WSP
	header is created and added for each part. The list is transformed 
	into multipart content with a call to the function WSP_BuildMulipart.
	The following WSP headers are added to each part:
	
	Content-type : As indicated by the string pbContentType.
	
	Content-Disposition : attachment. If the pbContentDispositionFileName
		parameter is specified, this is added as the filename parameter.

	NOTE! It is the caller's responsibility to deallocate the created 
	list.
	
	NOTE! The allocated memory in the parameter ppbData is taken over by 
	the function (the parameter is set to point to NULL before the function 
	returns).

	NOTE! ppMultipartHandle MUST NOT be NULL.

	Input: Pointer to list (MULTIPARTSTR**), Data (BYTE*), DataLen (UINT16),
		   ContentType (BYTE*), File name (BYTE*)
	Output: -
==========================================================================*/
/*  Modified by GBU,000510, use type
void WAE_AddGenericFormDataPart (void** ppMultiPartHandle, BYTE** ppbData, UINT16 iDataLen, 
								 BYTE* pbContentType, BYTE* pbContentDispositionFileName);
*/
void WAE_AddGenericFormDataPart (pMULTIPARTSTR* ppMultiPartHandle, BYTE** ppbData, UINT16 iDataLen,
                                 BYTE* pbContentType, BYTE* pbContentDispositionFileName);



/* ==================== HEADER BUILD HELP FUNCTIONS ======================
			Help functions used during the construction of headers.
   ======================================================================= */

/*========================================================================
	WSP_CreateHeaderField
==========================================================================
	Creates one header with the specified data and adds it to the end of 
	the header list. The indata pbData is NOT deallocated. The ppList 
	pointer	is updated to point to the last added header.

	NOTE! The Data MUST be defined, i.e., NOT null and at least one byte
	in length.

	Supported headers are:
	
	Field_Accept, Field_Accept_Charset, Field_Accept_Language, 
	Field_Authorization, Field_Proxy_Authorization, Field_Referer,
	Field_If_Modified_Since, Field_User_Agent, Field_If_None_Match,
	Field_Profile, Field_Accept_Application, and Field_Bearer_Indication
	
	Input: Field, data (NOT NULL), length of data in bytes (NOT 0) and 
		   pointer to pointer of header list.
	Output: TRUE if OK, FALSE otherwise.
==========================================================================*/
BOOL WSP_CreateHeaderField (BYTE bField, BYTE *pbData, UINT32 iDataLength,
						pHEADBUILDSTR *ppList);

/*========================================================================
	WSP_BuildHeader
==========================================================================
	Assembles the headers in the header	list to a byte stream. This stream 
	can then be sent as a valid header. The length of the byte stream is 
	stored in the iDataLength variable. The list of header build elements 
	is deleted if the operation is succesful.
	
	Input :	Pointer to Header List and pointer to iDataLength.
	Output:	BYTE *, NULL if error.
==========================================================================*/
BYTE *WSP_BuildHeader (pHEADBUILDSTR pList, UINT32 *iDataLength);


#endif
