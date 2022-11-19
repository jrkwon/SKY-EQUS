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
	FILE: HeadDef.h
  
	Definition file for the WSP-header decoding functions in the
	WAE-layer of the generic WAP client.

	Rev history:
		980923	JPR		Created
		990208	JPR		Corrections						
		990409	JPR		URL-definitions removed
		990526	JPR		HEADERDEF struct extended
		991111	JPR		ADDRESSTYPESTRUCT removed and ADDRESS added.
		991207	MRP		Push Header Field names added.
		991221	IPN		UAProf Header Field names added.
		000113	JPR		WSPPARAMETERS struct added

   ================================================================== */

#ifndef HEADDEF_H
#define HEADDEF_H

#include "cmmnrsrc.h"

/*	Header element.
	During the preparse, a linked list
	of header elements is created. The
	element contains six fields:
	
	bCodePage		: The actual code page. If the
					  codepage is 0, the pbFieldName
					  is used otherwise the bFieldName
					  is used.
	bFieldName		: Header field name (BYTE)
	pbFieldName		: Pointer to header field name 
					  (string)
	pbFieldValStart	: Pointer to the byte where
					  the value of the field starts
					  (not parsed)
	pbFieldValEnd	: Pointer to the byte directly 
					  after the value of the field 
					  ends.
	pNext			: Pointer to the next Element 
					  in the list.				  */
typedef struct tHEADELM
{
	BYTE bCodePage;
	BYTE bFieldName;
	BYTE *pbFieldName;
	BYTE *pbFieldValStart;
	BYTE *pbFieldValEnd;
	struct tHEADELM *pNext;
} HEADERELEMENT, *pHEADERELEMENT;

/*	Header definition element.
	Used during the header parse. The element
	contains three fields:

	pbData				: Pointer to the data.
	pbEnd				: Pointer to to the first byte after 
						  the data
	pHeadList			: List of found header fields in
						  the instream.				*/
typedef struct 
{
	BYTE *pbData;
	BYTE *pbEnd;
	pHEADERELEMENT pHeadList;
} HEADERDEF, *pHEADERDEF;


/* WSP Parameter struct */
typedef struct tWSPPARAMETERS
{
	INT16 iCharset;
	UINT8 iLevel;
	BYTE  bMPRType;			/* Used with Multipart/related */
	BYTE* pbMPRStart;		/* Used with Multipart/related */
	BYTE* pbMPRStartInfo;	/* Used with Multipart/related */
} WSPPARAMETERS, *pWSPPARAMETERS;



/* Addresstype struct */
typedef struct tADDRESS
{
	INT16	iBearerType;
	INT32	iPortNumber;
	BYTE*	pbAddress;
	UINT8	uiAddressLength;
	struct tADDRESS *pNext;
} ADDRESS, *pADDRESS;


/*	Header build help struct 
	This struct is used when headers are giong to
	be sent. For each header field a struct is 
	created. When finished, the structs are 
	assembled into one  character string, and the
	structs are discarded. The string is then sent. 

	The struct contains the following fields:

	BYTE *pchHeader				: The data
	UINT32 iLength				: Length of the data (nbr of BYTES)
	struct HEADBUILDSTR *pNext	: Pointer to next struct.	*/

typedef struct tHEADBUILDSTR
{
	BYTE *pbHeader;
	UINT32 iLength;
	struct tHEADBUILDSTR *pNext;
} HEADBUILDSTR, *pHEADBUILDSTR;


/*	Multipart help struct.
	This struct is used when a multipart is 
	received. When the function ParseMultipart
	is called the body of that indata is splitted
	into a few elements and a list of the Multi-
	part help struct is returned.
	
	The struct contains the following fields:

	BYTE *pbHdrStart			: Pointer to the start of the header
	BYTE *pbBodyStart			: Pointer to the start of the body
	UINT32 iHdrLen				: Length of the header (in BYTEs)
	UINT32 iBodyLen				: Length of the body (in BYTEs)
	struct MULTIPARTSTR *pNext	: Pointer to next struct		*/

typedef struct tMULTIPARTSTR
{
	BYTE *pbHdrStart;
	BYTE *pbBodyStart;
	UINT32 iHdrLen;
	UINT32 iBodyLen;
	struct tMULTIPARTSTR *pNext;
} MULTIPARTSTR, *pMULTIPARTSTR;

/*	Define Header Field Names for 
	Header Code Page #1	(default)			*/
#define Field_Accept				0x00
#define Field_Accept_Charset		0x01
#define Field_Accept_Encoding		0x02
#define Field_Accept_Language		0x03
#define Field_Accept_Ranges			0x04
#define Field_Age					0x05
#define Field_Allow					0x06
#define Field_Authorization			0x07
#define Field_Cache_Control			0x08
#define Field_Connection			0x09
#define Field_Content_Base			0x0A
#define Field_Content_Encoding		0x0B
#define Field_Content_Language		0x0C
#define Field_Content_Length		0x0D
#define Field_Content_Location		0x0E
#define Field_Content_MD5			0x0F
#define Field_Content_Range			0x10
#define Field_Content_Type			0x11
#define Field_Date					0x12
#define Field_Etag					0x13
#define Field_Expires				0x14
#define Field_From					0x15
#define Field_Host					0x16
#define Field_If_Modified_Since 	0x17
#define Field_If_Match				0x18
#define Field_If_None_Match			0x19
#define Field_If_Range				0x1A
#define Field_If_Unmodified_Since	0x1B
#define Field_Location 				0x1C
#define Field_Last_Modified			0x1D
#define Field_Max_Forwards			0x1E
#define Field_Pragma				0x1F
#define Field_Proxy_Authenticate	0x20
#define Field_Proxy_Authorization	0x21
#define Field_Public				0x22
#define Field_Range					0x23
#define Field_Referer				0x24
#define Field_Retry_After			0x25
#define Field_Server				0x26
#define Field_Transfer_Encoding		0x27
#define Field_Upgrade				0x28
#define Field_User_Agent			0x29
#define Field_Vary					0x2A
#define Field_Via					0x2B
#define Field_Warning				0x2C
#define Field_WWW_Authenticate		0x2D
#define Field_Content_Disposition	0x2E
#define Field_X_Wap_Application_Id  0x2F
#define Field_X_Wap_Content_URI		0x30
#define Field_X_Wap_Initiator_URI	0x31
#define Field_Accept_Application	0x32
#define Field_Bearer_Indication		0x33
#define Field_Push_Flag				0x34
#define Field_Profile				0x35
#define Field_Profile_Diff			0x36
#define Field_Profile_Warning		0x37

#endif
