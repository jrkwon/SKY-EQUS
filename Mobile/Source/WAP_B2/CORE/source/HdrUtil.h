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
	FILE: HdrUtil.h
  
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

 ================================================================== */
   
#ifndef CWAPHDRUTIL_H
#define CWAPHDRUTIL_H

#include "HeadDef.h"
#include "CmmnRsrc.h"
#include "WMLDef.h"
#include "Url.h"
#include "WAEctype.h"

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



/* ---------------------------------------------
	Descr : Finds the field names in the header
			and stores the pointers to the in a
			linked list. The pointers are to a
			COPY of the inputted data. The error-
			flag is set if error.
	Input :	The header as a BYTE *, The number 
			of bytes and pointer to BOOL.
	Output:	Creates a HEADERDEF and returns a
			pointer to it (consult HeadDef.h).
   --------------------------------------------- */
pHEADERDEF WSP_PreParseHeaders (BYTE *pbInstream, INT32 iLength, BOOL *fError);

/* ---------------------------------------------
	Descr : Deletes the element list and frees 
			the memory.
	Input : pDef
	Output:	-
   --------------------------------------------- */
void WSP_EmptyHeaderList (pHEADERDEF pDef);
/* ============================================= */




/* ========= HEADER PARSE FUNCTIONS ============ 
	    Functions used to parse the headers.
   ============================================= */

/* ---------------------------------------------
	Descr : Returns a pointer to the element specified
			by the parameters. Some headers might 
			exist in more than one instance, e.g. 
			Accept-language. To get the elements call
			the function repeatedly and change the 
			pFirstElm gradually.
	Input : The desired field as a short-int (first
			function) or a Token-Text (second function).
			The pFirstElm points to the first element
			int the list to be examined.
	Output:	Pointer to header element, NULL if not found.
   --------------------------------------------- */
/* Well-Known */
pHEADERELEMENT WSP_GetHeaderWK (BYTE bCodePage, BYTE bByte, 
							pHEADERELEMENT pFirstElm);
/* Token-Text */
pHEADERELEMENT WSP_GetHeaderTT (BYTE *pbTokenText, pHEADERELEMENT pFirstElm);


/* Return <0xxx xxxx> of a BYTE */
BYTE WSP_GetShortInt (BYTE bShortInt);

UINT32 WSP_GetLongInteger (BYTE **ppbStart, BYTE *pbEnd, BOOL *pfError);

/* ---------------------------------------------
	Descr : Returns the Content-type value of
			the HEADERELEMENT. If the value
			includes parameters (charset or level), 
			the	iCharset integer and iLevel integer
			are set accordingly. The iCharset is set
			with the IANA value of the charset. If 
			no charset parameter is included the 
			iCharset is set to IANA_INVALID. The
			iLevel is set in the same way with the
			default value 0.
	Input :	Pointer to HEADERELEMENT, pointer
			to error flag (set to FALSE), pointer 
			to INT16 and pointer to UINT8
	Output:	BYTE, The error flag is set if error.
   --------------------------------------------- */
BYTE WSP_GetContentTypeByte (pHEADERELEMENT pElement, BOOL *fError, 
							 INT16 *piCharset, UINT8 *piLevel);

/* ---------------------------------------------
	Descr : As above, but returns string. It is
			the caller's responsibility to de-
			allocate the memory of the string.

	Output:	BYTE*, The error flag is set if error.
   --------------------------------------------- */
BYTE *WSP_GetContentTypeString (pHEADERELEMENT pElement, BOOL *fError, 
								 INT16 *piCharset, UINT8 *piLevel);

/* ---------------------------------------------
	Descr : Returns the Content-type value of
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

/* ============================================= */



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





/* ========= AUTHENTICATION FUNCTIONS ==========
	Functions for handling authorization, i.e. 
	keep track of the visited realms, and the 
	appropriate passwords, etc.
   ============================================= */

/* ---------------------------------------------
	Descr :	The function stores the realm, the 
			path and the credentials under the 
			appropriate Host in the Host List
			in the User Agent. If the realm 
			already exists under the actual host,
			the least significant path is stored
			(e.g. /pub/ is stored instead of 
			/pub/ftp.) DATA IS COPIED.
	Input :	User Agent, Complete URL, Realm, 
			User-id+password, length of User-id
			+password (nbr of bytes including EOT)
	Output:	TRUE if OK, FALSE otherwise
   --------------------------------------------- */
BOOL StoreAuthParams (pUA pUser, BYTE *pbURL, BYTE *pbRealm,
						BYTE *pbCredentials, UINT8 iCredLength);


/* ---------------------------------------------
	Descr :	The function examines if the host in 
			the URL has	been visited before, and 
			if it requires authorization. If the 
			Realm field is specified and the realm
			is found in the list, the result is TRUE,
			otherwise FALSE. If the realm value is not
			specified (NULL) and the host contains
			at least one realm the URL is checked 
			against the URL in the realm. If the new
			URL is "below" the realm url, the result
			is TRUE, otherwise FALSE. If the result 
			is TRUE, the correct User-id and password 
			is stored in ppbCredentials and the length 
			of the bytestream is stored in piCredLength. 
			If the function returns FALSE, these 
			values are set to NULL and 0 respectively.
			DATA IS NOT COPIED.
	Input : User Agent, complete URL, Realm, pointer 
			to Credentials and pointer to UINT8.
	Output: TRUE if host exist, FALSE otherwise
   --------------------------------------------- */
BOOL CheckAuth (pUA pUser, BYTE *pbURL, BYTE *pbRealm, 
					BYTE **ppbCredentials, UINT8 *piCredLength);

/* ---------------------------------------------
	Descr :	Removes the Host List from the user
			user agent.
	Input :	User Agent
	Output:	-
   --------------------------------------------- */
void DeleteHostList (pUA pUser);

/* ============================================= */




/* ============= CONVERT FUNCTIONS =============
	   Functions for coverting from bytes to
	   strings and vice versa.
   ============================================= */
/* ---------------------------------------------
	Descr : Converts the value of the Content Type
			Value from a character string to a BYTE
			or vce versa. If *ppbText is NULL the byte
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




/* ============ MULTIPART FUNCTIONS ============
		Functions for parsing multipart data.
   ============================================= */
/* ---------------------------------------------
	Descr : This function parses the body of a
			multipart entity and returns a list
			of MULTIPARTSTR (consult HeadDef.h).
			The pbInstream is NOT deallocated.
	Input :	Pointer to BYTEstream and length.
	Output:	Pointer to MULTIPARTSTR, NULL if
			error.
   --------------------------------------------- */
pMULTIPARTSTR WSP_PreParseMultipart (BYTE *pbInstream, UINT32 iLength);

/* ============================================= */




/* ======= HEADER BUILD HELP FUNCTIONS ========= 
	Help functions used during the construction
	of headers.
   ============================================= */
/* ---------------------------------------------
	Descr : Creates one header with the specified
			data and adds it to the end of the
			header list. The indata pbData is
			NOT deallocated. The ppList pointer
			is updated to point to the last added 
			header.
			
			(Field_Accept, Field_Accept_Charset,
			Field_Accept_Language, Field_Authorization, 
			Field_Proxy_Authorization, 
			Field_If_Modified_Since, Field_Referer 
			and Field_User_Agent supported.)

	Input :	Field, data, length of data in bytes
			and pointer to pointer of header list.
	Output:	TRUE if OK, FALSE otherwise. If the 
			result is FALSE.
   --------------------------------------------- */
BOOL WSP_CreateHeaderField (BYTE bField, BYTE *pbData, UINT32 iDataLength,
						pHEADBUILDSTR *ppList);

/* ---------------------------------------------
	Descr : Assembles the headers in the header
			list to a byte stream. This stream 
			can then be sent as a valid header. 
			The length of the byte stream is 
			stored in the iDataLength variable. 
			The list of header build elements is 
			deleted if the 	operation is succesful.
	Input :	Pointer to Header List and pointer 
			to iDataLength.
	Output:	BYTE *, NULL if error.
   --------------------------------------------- */
BYTE *WSP_BuildHeader (pHEADBUILDSTR pList, UINT32 *iDataLength);

/* ============================================= */

#endif
