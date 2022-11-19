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

	WAP Implementation Project

==========================================================================

	File: prov_dec.h

	Description: External DTD functions needed for the WBXML-decoding of the Provisioning
				 Content types.

	Author: Kent Olsson, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	010315    KEOL   First version

==========================================================================*/
#ifndef _prov_dec_h
#define _prov_dec_h

#include "WBXMLDef.h"
#include "wbxmldec.h"
#include "tapiclnt.h"

/*========================================================================

	Provisioning Definitions

==========================================================================*/

/* Element type definitions */
#define Prov_Type_WapProvisioningDoc		(UINT16) 0x05
#define Prov_Type_Characteristic			(UINT16) 0x06
#define Prov_Type_Parm						(UINT16) 0x07

#define PROV_VERSION_1_0					1


/* ProvisioningDoc and Characteristic element - called PROVELM */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;			/* Characterisitic or Parm */
	UINT16 iAttribute;				/* Version if a ProvisioningDoc, or Type if a Characteristics */
/*	union						Om godkänna sträng som värde för attribut type i characteristics måste vi använda denna union istället
	{
		UINT16 iAttribute;					
		WCHAR* pwchAttribute;				
	}Attribute;*/	
} PROV_PROVELEMENT, *pPROV_PROVELEMENT;


/* Parm element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	UINT16 iName;
	WCHAR* pwchName;
	UINT16 iValue;				/* Eihter iValue or pwchValue, if not used iValue = 0, and pwchValue = NULL */
	WCHAR* pwchValue;

} PROV_PARMELEMENT, *pPROV_PARMELEMENT;


/* element used to store incomming Provisoning Push messages */
/* while calculating the MAC parameter */
typedef struct
{
	void*	vsBody;
	UINT32	iBodyLen;
	UINT16	iSEC;
	void*	vsMAC;
	INT16	iObjectID;
	INT16	iTID;
	UINT16	iExpireDate;

} PROV_MACELEMENT, *pPROV_MACELEMENT;

 

/*========================================================================

	External Provisioning DTD Functions

==========================================================================*/

/*========================================================================
	Prov_CreateElement
==========================================================================*/
pELEMENTTYPE Prov_CreateElement (pDECSTR pDecStr, UINT16 iType);

/*========================================================================
	Prov_ValidContent
==========================================================================*/
BOOL Prov_ValidContent (pDECSTR pDecStr, pELEMENTTYPE pElement);

/*========================================================================
	Prov_GetContent
==========================================================================*/
pELEMENTTYPE Prov_GetContent (pELEMENTTYPE pElement);

/*========================================================================
	Prov_AppSpecParseMod
==========================================================================*/
void Prov_AppSpecParseMod (pDECSTR pDecStr);

/*========================================================================
	Prov_LiteralAttributeToToken
==========================================================================*/
UINT16 Prov_LiteralAttributeToToken (WCHAR* pwchToken);

/*========================================================================
	Prov_LiteralTagToToken
==========================================================================*/
UINT16 Prov_LiteralTagToToken (WCHAR* pwchToken);

/*========================================================================
	Prov_DeleteElement
==========================================================================*/
void Prov_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement);

/*========================================================================
	Prov_StoreAttributeValue
==========================================================================*/
BOOL Prov_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal);

/*========================================================================
	Prov_GetAttributeValue
==========================================================================*/
WCHAR* Prov_GetAttributeValue (UINT16 iToken);

/*========================================================================
	PROV_CheckPublicID
==========================================================================*/
BOOL PROV_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId);

/*========================================================================

	External Provisioning Functions (Not DTD functions)

==========================================================================*/

/*========================================================================
	Prov_CreateTokenValueElem (Not a DTD function)
==========================================================================*/
pELEMENTTYPE Prov_CreateTokenValueElem(UINT16 iAttrVal);

#endif
