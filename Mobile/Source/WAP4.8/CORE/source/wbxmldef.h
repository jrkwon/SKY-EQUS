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
    FILE: wbxmldef.h
    
    General WBXML-decoder. Definition file.

    Rev history:
        990302  JPR     Created
        990319  JPR     Modified
        990326  JPR     IANA-codes moved to CmmnRsrc.h
        990414  JPR     Corrections
        990511  JPR     Additions in tWBXMLDECODESTR
        990915  JPR     Updated to support channel decoding
        991112  JPR     Updated to support PUSH content types

        991126  JPR     Redesign of Element Handling, Text Handling,
                        and Error Handling (Consult separate document).
                        Updates for WAP 1.2 are also included. Application 
                        specific parts removed
        991207  JPR     Small changes
        991208  JPR     ViewId removed
        991222  JPR     iDTD in tWBXMLDECODESTR changed to iVersion
        000104  JPR     xml:space def. added, DTD_CreateElementFunc mod.
        000113  JPR     OPAQUE changed to XMLOPAQUE
        000131  JPR     Generic Content Element added (tCONTENTELEMENT)
        000925  JPR     WBXML version 1.3 added
        010119  NKE     Replaced DTD_AddContentFunc with DTD_ValidContentFunc
                        for the new decoder in wbxmldec.c.
        010125  NKE     Changed definition of DTD_AppSpecParseModFunc.

=========================================================================*/

#ifndef CWAPWBXMLDEF_H
#define CWAPWBXMLDEF_H

#include "cmmnrsrc.h"

/*========================================================================

	Definitions

=========================================================================*/

#define WBXML_VERSION_NUMBER_1_1	((BYTE) 0x01)	/* WBXML 1.1 */
#define WBXML_VERSION_NUMBER_1_2	((BYTE) 0x02)	/* WBXML 1.2 */
#define WBXML_VERSION_NUMBER_1_3	((BYTE) 0x03)	/* WBXML 1.3 */

#define Type_Literal			4	/* Literal tag */

#define WBXML_TIMESLOT 300 /* Length of each decode session */

/* xml:space definitions; only 'preserve' or 'default' allowed */
#define XML_SPACE_PRESERVE		0
#define XML_SPACE_DEFAULT		1

#define isXMLWhiteSpace(c)	((c==(WCHAR)'\x20')||(c==(WCHAR)'\x9')||(c==(WCHAR)'\xD')||(c==(WCHAR)'\xA'))
#define B_isXMLWhiteSpace(c)	((c=='\x20')||(c=='\x9')||(c=='\xD')||(c=='\xA'))

typedef struct tWBXMLDECODESTR *pDECSTR;

/*========================================================================

	Global Element definitions

=========================================================================*/

/* Global Element types */
#define Type_ContentElement	(UINT16) 0x01	/* Super element + content	*/
#define Type_Entity			(UINT16) 0x02	/* The Element is an Entity	*/
#define Type_Inline_Text	(UINT16) 0x03	/* Inline string			*/
#define Type_Token_Value	(UINT16) 0x51	/* Token Value, used only with connectivity doc, used in provisioning and E2E sec */ 
#define Type_Ext_0			(UINT16) 0x40	/* The Element is a EXT_0	*/
#define Type_Ext_1			(UINT16) 0x41	/* The Element is a EXT_1	*/
#define Type_Ext_2			(UINT16) 0x42	/* The Element is a EXT_2	*/
#define Type_Table_Text		(UINT16) 0x83	/* String table reference	*/
#define Type_Opaque			(UINT16) 0xC3	/* OPAQUE data				*/


/* Super Element 

   The type attribute determines the type of the element in the actual 
   DTD. Elements with a global type have the same meaning in all DTD:s.
   The values are set to the same values as the global tokens in the
   WBXML specification to avoid collision with application specific types
   (defined in e.g. the WML specification).

   All elements MUST have iType (UINT16) as its first attribute and 
   pNextElement (struct tELEMENTTYPE*) as its second. 
*/
typedef struct tELEMENTTYPE
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
} ELEMENTTYPE, *pELEMENTTYPE;


/* Super Element with content

   This element is used when content is to be stored. All elements with
   content MUST be conformant with this element. The content is stored 
   with a call to the function XML_AddContent. Content is retrieved with
   a call to the function XML_GetContent.
*/
typedef struct tCONTENTELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
	pELEMENTTYPE pContent;
} CONTENTELEMENT, *pCONTENTELEMENT;


/* Inline Text Element.

   Used for internal representation of inline strings. The data is 
   stored in the pwchTextData attribute.
*/
typedef struct tI_TEXTELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE *pNextElement;
	WCHAR* pwchTextData;
} I_TEXTELEMENT, *pI_TEXTELEMENT;


/* String table reference Text Element.

   Used for internal representation of strings. A reference to the 
   string table is stored in the iTable attribute.
*/
typedef struct tT_TEXTELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE *pNextElement;
	UINT32 iTableRef;
} T_TEXTELEMENT, *pT_TEXTELEMENT;

/* Token Element.

   Used for internal representation of Token values. The data is 
   stored in the iTokenData attribute.
*/
typedef struct t_TOKENELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE *pNextElement;
	UINT16 iTokenData;
} TOKENELEMENT, *pTOKENELEMENT;


/* Ext Element.

   Used for internal representation of extensions (Type_Ext_0, 
   Type_Ext_1, and Type_Ext_2). The name of the extension is stored 
   in the pwchName attribute.
*/
typedef struct tEXT_ELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE *pNextElement;
	WCHAR* pwchName;
} EXT_ELEMENT, *pEXT_ELEMENT;


/* Entity Element.

   Used for internal representation of Entities. The entity is stored 
   in the iEntity attribute.
*/
typedef struct tENTITY
{
	UINT16 iType;
	struct tELEMENTTYPE *pNextElement;
	UINT32 iEntity;
} ENTITY, *pENTITY;


/* Opaque Element.

   Used for internal representation of Opaque data. The data is stored 
   in the pbData attribute and the length is stored in the iLength 
   attribute.
*/
typedef struct tXMLOPAQUE
{
	UINT16 iType;
	struct tELEMENTTYPE *pNextElement;
	UINT32 iLength;
	BYTE* pbData;
} XMLOPAQUE, *pXMLOPAQUE;



/*========================================================================

	Definitions for the Document Type Specific function pointers that
	are needed by the WBXML-decoder for each supported DTD.

=========================================================================*/

/*========================================================================
	DTD_CreateElementFunc
==========================================================================
	A function of this type is used for creating an element.

	Creates an element of the specified type and sets the default values.
	The returned element is of the type tELEMENTTYPE, but can be casted
	to the correct type (stored in the element, consult the definitions in
	the file WBXMLDef.h) if the attributes are required.

	It is the caller's responsibility to deallocate the memory (with a 
	call to the function XML_DeleteElement).
	
	Input: pWBXMLDECODESTR (not NULL), Type of element to be created
	Output: Pointer to a struct of the type tELEMENTTYPE or NULL.
==========================================================================*/
typedef 
pELEMENTTYPE DTD_CreateElementFunc (pDECSTR pDecStr, UINT16 iType);


/*========================================================================
	DTD_DeleteElementFunc
==========================================================================
	A function of this type is used for deleting an element and all its
	content.

	Deletes an element and all its content and frees the memory. The in-
	parameter ppElement is set to point to NULL after the element has been 
	deleted.

	Input: pWBXMLDECODESTR (not NULL), ELEMENTTYPE**
	Output: -
==========================================================================*/
typedef 
void DTD_DeleteElementFunc (pDECSTR pDecStr, pELEMENTTYPE* ppElement);


/*========================================================================
	DTD_ValidContentFunc
==========================================================================
	A function of this type is used for checking if an element may take
	content.

	The function returns TRUE if the content is legal in the element,
	FALSE otherwise. The function SHOULD consider the Version 
	attribute in the Decode Struct when the decision is made. If the 
	function returns FALSE, the calling function will set the Illegal 
	Content Warning in the Decode Struct. 
	
	Input: pWBXMLDECODESTR (not NULL) and an element.
	Output: TRUE/FALSE
==========================================================================*/
typedef
BOOL DTD_ValidContentFunc (pDECSTR pDecStr, pELEMENTTYPE pElement);


/*========================================================================
	DTD_GetContentFunc
==========================================================================
	A function of this type is used to retrieve the first content from an
	element.

	If no content is present or if the pElement is NULL, NULL is returned.
	
	NOTE! The returned data is not a copy and therefore MUST NOT be 
	deallocated. The data is deallocated when the structure is deleted.

	Input: Element
	Output: First Content in the Element or NULL
==========================================================================*/
typedef 
pELEMENTTYPE DTD_GetContentFunc (pELEMENTTYPE pElement);



/*========================================================================
	DTD_AppSpecParseModFunc
==========================================================================
	A function of this type is used to do application specific modifi-
	cations to the element. 

	NOTE! The Element may be removed!

	Input: pWBXMLDECODESTR (not NULL)
	Output: -
==========================================================================*/
typedef 
void DTD_AppSpecParseModFunc (pDECSTR pDecStr);


/*========================================================================
	DTD_StoreAttributeValueFunc
==========================================================================
	The function stores the specified attribute value (ppAttrVal) in the 
	element under the specified attribute (iAttribute). The ppAttrVal is
	a list of elements ( Type_Text, Type_Literal, or Type_Ext ) that 
	should be stored in the element. The iAttribute is an UINT16 identi-
	fying the attribute including code page changes.

	The function returns TRUE if the attribute and the attribute value
	is legal in the element, FALSE otherwise. The function SHOULD con-
	sider the version attribute in the Decode Struct when the decision is 
	made. If the function returns FALSE, the calling function will set 
	the	Illegal Attribute Warning in the Decode Struct.
	
	NOTE! If the value is stored, the ppAttrVal pointer MUST be set to 
	NULL.

	Input: pWBXMLDECODESTR (not NULL), Element, Attribute, and attribute
		   value (list of ELEMENTTYPE*)
	Output: TRUE/FALSE
==========================================================================*/
typedef 
BOOL DTD_StoreAttributeValueFunc (pDECSTR pDecStr, pELEMENTTYPE pElement,
                                  UINT16 iAttribute, pELEMENTTYPE* ppAttrVal);


/*========================================================================
	DTD_GetAttributeValueFunc
==========================================================================
	Returns the corresponding string to the specified attribute value. If 
	a matching value is not found, NULL is returned.
	
	Input: Attribute value (UINT16)
	Output: Null-terminated string (WCHAR*)
==========================================================================*/
typedef 
WCHAR* DTD_GetAttributeValueFunc (UINT16 iToken);


/*========================================================================
	DTD_LiteralAttributeToTokenFunc
==========================================================================
	Returns the corresponding attribute token to the specified string. If 
	a matching value is not found, Type_Literal is returned.

	NOTE! The string MUST be left unchanged.
	
	Input: Null terminated WCHAR*.
	Output: UINT16
==========================================================================*/
typedef 
UINT16 DTD_LiteralAttributeToTokenFunc (WCHAR* pwchToken);


/*========================================================================
    DTD_LiteralTagToTokenFunc
==========================================================================
    Returns the corresponding tag token to the specified string. If a 
    matching value is not found, Type_Literal is returned.

    NOTE! The string MUST be left unchanged.

    Input: Null terminated WCHAR*.
    Output: UINT16
==========================================================================*/
typedef 
UINT16 DTD_LiteralTagToTokenFunc (WCHAR* pwchToken);


/*========================================================================
    DTD_CheckPublicIDFunc
==========================================================================
    Examines the PublicID and verifies it. If the function returns TRUE
    the public id is accepted. The value in the piPId is stored in the 
    iVersion attribute in the Decode Struct. If the function returns 
    FALSE, the decoding is terminated.

    NOTE! If the pwchPId parameter is specified, this will be used, other-
    wise the piPId will be used for the check. 

    If the function returns TRUE, the correct integer MUST be stored in 
    the piPId parameter. This value will then be stored in the attribute 
    iVersion in the pWBXMLDECODESTR.

    Input: pWBXMLDECODESTR (not NULL), WCHAR*, and UINT8
    Output: TRUE if valid ID, FALSE otherwise.
==========================================================================*/
typedef 
BOOL DTD_CheckPublicIDFunc (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId);


/*========================================================================

    Decode Structure

=========================================================================*/

/*  String Table Element

    When the prolog in the WBXML document is decoded, a list of all
    strings in the WBXML string table is created. The list consists of
    elements of the following type and is stored in the pStrTab attribute
    in the tWBXMLDECODESTR. The string table is used during the decoding 
    of the WBXML document and AS LONG AS THE WBXML STRUCTURE IS USED. 
    The string table is discarded in the WBXML_DeleteWBXMLDecStr function.
*/
typedef struct tSTRINGTABSTR
{
    WCHAR *pwchString;              /* The string                   */
    UINT32 iOffset;                 /* Offset from the first byte   */
    struct tSTRINGTABSTR *pNext;    /* Next string in the list      */
} STRINGTABSTR, *pSTRINGTABSTR;

/* 
    The Decode Struct

    This structure is created with a call to the WBXML_DeleteWBXMLDecStr 
    function. After this, the attributes in the struct MUST be set to
    correct values BEFORE the decoding is started. 

    NOTE! All of the function pointers must have a value other than NULL.

    DO NOT DELETE UNTIL THE USE OF THE WBXML STRUCTURE IS FINISHED!
*/
typedef struct tWBXMLDECODESTR
{
    BYTE*         pbCurByte;        /* Pointer to data */
    BYTE*         pbEnd;            /* Pointer to first byte AFTER the data */
    pSTRINGTABSTR pStrTab;
    void*         pAppSpec;         /* Pointer to application specific data */
    UINT32        iDecodeResult;    /* Errors and Warnings set during the decode */
    INT16         iCharset;         /* IANA code */
    BYTE          bContentType;     /* WSP well-known content type */
    UINT8         iContentLevel;
    UINT8         iVersion;         /* The version; set in DTD_CheckPublicID */
    UINT8         iAttrCodePage;    /* Starts at 0 */
    UINT8         iTagCodePage;     /* Starts at 0 */

    /* Used while building the structure */
    pELEMENTTYPE  currentElement;
    pELEMENTTYPE  parentElement;
    pELEMENTTYPE  topElement;
    pELEMENTTYPE  deletePtr;
    UINT16        timer;
    BYTE          state;
	BOOL          scheduled;


    /* Application specific functions */
    DTD_CreateElementFunc           *DTD_CreateElement;
    DTD_DeleteElementFunc           *DTD_DeleteElement;
    DTD_ValidContentFunc            *DTD_ValidContent;
    DTD_GetContentFunc              *DTD_GetContent;
    DTD_AppSpecParseModFunc         *DTD_AppSpecParseMod;
    DTD_StoreAttributeValueFunc     *DTD_StoreAttributeValue;
    DTD_GetAttributeValueFunc       *DTD_GetAttributeValue;
    DTD_LiteralAttributeToTokenFunc *DTD_LiteralAttributeToToken;
    DTD_LiteralTagToTokenFunc       *DTD_LiteralTagToToken;
    DTD_CheckPublicIDFunc           *DTD_CheckPublicID;

} WBXMLDECODESTR, *pWBXMLDECODESTR;



/*========================================================================

    Global WBXML Token Definitions 

=========================================================================*/

/* Special tokens. Filters etc.
    ATTR - Token has attribute (bit 7)
    CONT - Token has content (bit 6)
    BOTH - Token has ATTR and CONT
    MASK - The first 6 bits set to 1
*/
#define ATTR        (unsigned char) '\x80'
#define CONT        (unsigned char) '\x40'
#define BOTH        (unsigned char) '\xC0'
#define MASK        (unsigned char) '\x3F'

/* Global tokens */
#define GLOBAL_SWITCH_PAGE  (unsigned char) '\x00'
#define GLOBAL_END          (unsigned char) '\x01'
#define GLOBAL_ENTITY       (unsigned char) '\x02'
#define GLOBAL_STR_I        (unsigned char) '\x03'
#define GLOBAL_LITERAL      (unsigned char) '\x04'
#define GLOBAL_EXT_I_0      (unsigned char) '\x40'
#define GLOBAL_EXT_I_1      (unsigned char) '\x41'
#define GLOBAL_EXT_I_2      (unsigned char) '\x42'
#define GLOBAL_PI           (unsigned char) '\x43'
#define GLOBAL_LITERAL_C    (unsigned char) '\x44'
#define GLOBAL_EXT_T_0      (unsigned char) '\x80'
#define GLOBAL_EXT_T_1      (unsigned char) '\x81'
#define GLOBAL_EXT_T_2      (unsigned char) '\x82'
#define GLOBAL_STR_T        (unsigned char) '\x83'
#define GLOBAL_LITERAL_A    (unsigned char) '\x84'
#define GLOBAL_EXT_0        (unsigned char) '\xC0'
#define GLOBAL_EXT_1        (unsigned char) '\xC1'
#define GLOBAL_EXT_2        (unsigned char) '\xC2'
#define GLOBAL_OPAQUE       (unsigned char) '\xC3'
#define GLOBAL_LITERAL_AC   (unsigned char) '\xC4'


/*========================================================================

    WBXML Decoder Errors and Warnings

=========================================================================*/

/* 
 Warnings. 
 
    If any of the following should occur during the decoding of the data,
    the corresponding bit in the DecodeResult-variable in the decode
    struct is set. Warnings are not fatal, i.e., the decoding will 
    continue.

    (Warnings are defined to be the 16 least significant bits in the 
    DecodeResult-variable)
*/
#define WBXML_Warning_UnknownToken          (UINT32)    0x1
#define WBXML_Warning_UnknownAttribute      (UINT32)    0x2
#define WBXML_Warning_UnknownAttributeValue (UINT32)    0x4
#define WBXML_Warning_IllegalAttribute      (UINT32)    0x8
#define WBXML_Warning_IllegalElement        (UINT32)    0x10
#define WBXML_Warning_Version               (UINT32)    0x20
#define WBXML_Warning_UnknownCharset        (UINT32)    0x40

/* 
    Errors. 

    If any of the following should occur during the decoding of the data,
    the corresponding bit in the DecodeResult-variable in the decode
    struct is set. Errors are fatal, i.e., the decoding will terminate.

    (Errors are defined to be the 16 most significant bits in the 
     DecodeResult-variable)
*/
#define WBXML_Error_OutOfMemory             (UINT32)    0x10000
#define WBXML_Error_DataStream              (UINT32)    0x20000
#define WBXML_Error_StringTable             (UINT32)    0x40000
#define WBXML_Error_MbuintEncoding          (UINT32)    0x80000
#define WBXML_Error_CharsetEncoding         (UINT32)    0x100000
#define WBXML_Error_Encoding                (UINT32)    0x200000
#define WBXML_Error_PublicID                (UINT32)    0x400000
#define WBXML_Error_Unknown                 (UINT32)    0x800000

/* If the DecodeResult is greater than 0xFFFF, the decoding is terminated */
#define WBXML_FatalError                    (UINT32)    0xFFFF


#endif
