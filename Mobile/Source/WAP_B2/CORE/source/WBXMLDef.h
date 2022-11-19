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
	FILE: WBXMLDef.h
	
	General WBXML-decoder. Definition file.

	Rev history:
		990302	JPR		Created
		990319	JPR		Modified
		990326	JPR		IANA-codes moved to CmmnRsrc.h
		990414	JPR		Corrections
		990511	JPR		Additions in tWBXMLDECODESTR
		990915	JPR		Updated to support channel decoding
		991112	JPR		Updated to support PUSH content types

   ================================================================== */

#ifndef CWAPWBXMLDEF_H
#define CWAPWBXMLDEF_H

#include "CmmnRsrc.h"

#define WBXML_VERSION_NUMBER		((BYTE) 0x01)

/* Special tokens. Filters etc.
	ATTR - Token has attribute (bit 7)
	CONT - Token has content (bit 6)
	BOTH - Token has ATTR and CONT		
	MASK - The first 6 bits set to 1	*/
#define ATTR		(unsigned char) '\x80'
#define CONT		(unsigned char) '\x40'
#define BOTH		(unsigned char) '\xC0'
#define MASK		(unsigned char) '\x3F'

/* Global tokens			*/
#define GLOBAL_SWITCH_PAGE	(unsigned char) '\x00'
#define	GLOBAL_END			(unsigned char) '\x01'
#define	GLOBAL_ENTITY		(unsigned char) '\x02'
#define	GLOBAL_STR_I		(unsigned char) '\x03'
#define	GLOBAL_LITERAL		(unsigned char) '\x04'
#define	GLOBAL_EXT_I_0		(unsigned char) '\x40'
#define	GLOBAL_EXT_I_1		(unsigned char) '\x41'
#define	GLOBAL_EXT_I_2		(unsigned char) '\x42'
#define	GLOBAL_PI			(unsigned char) '\x43'
#define	GLOBAL_LITERAL_C	(unsigned char) '\x44'
#define	GLOBAL_EXT_T_0		(unsigned char) '\x80'
#define	GLOBAL_EXT_T_1		(unsigned char) '\x81'
#define	GLOBAL_EXT_T_2		(unsigned char) '\x82'
#define GLOBAL_STR_T		(unsigned char)	'\x83'
#define	GLOBAL_LITERAL_A	(unsigned char) '\x84'
#define	GLOBAL_EXT_0		(unsigned char) '\xC0'
#define	GLOBAL_EXT_1		(unsigned char) '\xC1'
#define	GLOBAL_EXT_2		(unsigned char) '\xC2'
#define	GLOBAL_OPAQUE		(unsigned char) '\xC3'
#define	GLOBAL_LITERAL_AC	(unsigned char) '\xC4'

/* DTD Definitions */
#define DTD_WML					1

/* WTA specific */
#ifdef CONFIG_WTA
	#define DTD_CHANNEL			2
#endif

/* PUSH specific */
#ifdef CONFIG_PUSH
	#define DTD_PUSH_SI			3
	#define DTD_PUSH_SL			4
	#define DTD_PUSH_CO			5
#endif


/* Error codes */
#define ERROR_Out_Of_Memory				 1
#define ERROR_String_Table_Reference	 2
#define ERROR_Text_Encoding				 4
#define ERROR_Unknown_Attribute_Value	 8
#define ERROR_Unknown_Token				16
#define ERROR_WML_Error					32

#define W_EndOfText	(WCHAR) 0

/* Super element 

   The type attribyte determines the type of the element
   in the actual DTD. Elements with a global type have
   the same meaning in all DTD:s.
*/

#define Type_Text				1	/* Change to Type_Text */
#define Type_Literal			2	/* Change to Type_Literal */

typedef struct tELEMENTTYPE
{
	UINT8 iType;
	void *pElm;
	struct tELEMENTTYPE *pNextElement;
} ELEMENTTYPE, *pELEMENTTYPE;

/* ------------------------------------------
   String Table
   ------------------------------------------ */
typedef struct tSTRINGTABSTR
{
	WCHAR *pchString;
	UINT32 iOffset;
	struct tSTRINGTABSTR *pNext;
} STRINGTABSTR, *pSTRINGTABSTR;

/* Text element. 

   WBXML-types  ( string | extension | entity )

   Used for internal representation of texts, If 
   the element is a extension, pchTextData is the 
   name of the extension. If the element is a 
   string pchTextData is the Text. The iEntity field
   is used only if the element is an entity.
*/
enum TextType {T_String, T_Ext_0, T_Ext_1, T_Ext_2, T_Entity};

typedef struct tTEXTELEMENT 
{
	enum TextType iType;
	UINT32 iEntity;
	WCHAR *pchTextData;
	struct tTEXTELEMENT *pNextText;
} TEXTELEMENT, *pTEXTELEMENT;

typedef struct tWBXMLDECODESTR
{
	BYTE *pbCurByte;
	BYTE *pbEnd;
	INT16 iCharset;
	BYTE bContentType;
	UINT8 iContentLevel;
	UINT8 iDTD;
	UINT8 iAttrCodePage;
	UINT8 iTagCodePage;
	UINT8 iError;
	BOOL fAbort;	/* Fatal error */
	UINT8 iViewID;
	pSTRINGTABSTR pStrTab;
	void *pAppSpec;	/* Pointer to application specific data */
} WBXMLDECODESTR, *pWBXMLDECODESTR;

#endif
