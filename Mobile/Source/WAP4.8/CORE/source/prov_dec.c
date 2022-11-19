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

	File: prov_elm.c

	Description:

	Functions for handling the different elements in the Provisioning speci-
	fications, i.e., WAP-PROVISIONINGDOC, CHARACTERISTIC and PARM.

	Author: Kent Olsson, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	010221    KEOL   First version

==========================================================================*/

#include "prov_dec.h"
#include "WBXMLElm.h"
#include "WBXMLDef.h"

/*========================================================================

	Private functions 

=========================================================================*/

/*========================================================================
	Set_Char_RootAttribute
==========================================================================*/
BOOL Set_Char_RootAttribute(pDECSTR pDecStr,pELEMENTTYPE pElement,UINT16 iAttribute)
{
	if ( pDecStr->parentElement->iType == Prov_Type_WapProvisioningDoc)
	{
		((PROV_PROVELEMENT *)(pElement))->iAttribute=iAttribute;
		return TRUE;
	}
	else
		return FALSE;
}

/*========================================================================
	Set_Char_PxlogicAttribute
==========================================================================*/
BOOL Set_Char_PxlogicAttribute(pDECSTR pDecStr,pELEMENTTYPE pElement,UINT16 iAttribute)
{
	pPROV_PROVELEMENT pParentElm = (PROV_PROVELEMENT *) pDecStr->parentElement;
	
	if ( pParentElm->iType == Prov_Type_Characteristic && pParentElm->iAttribute == PRVNG_PXLOGICAL)
	{
		((PROV_PROVELEMENT *)(pElement))->iAttribute=iAttribute;
		return TRUE;
	}
	else
		return FALSE;
}

/*========================================================================
	Set_Char_NapdefAttribute
==========================================================================*/
BOOL Set_Char_NapdefAttribute(pDECSTR pDecStr,pELEMENTTYPE pElement,UINT16 iAttribute)
{
	pPROV_PROVELEMENT pParentElm = (PROV_PROVELEMENT *) pDecStr->parentElement;
	
	if ( pParentElm->iType == Prov_Type_Characteristic && pParentElm->iAttribute == PRVNG_NAPDEF)
	{
		((PROV_PROVELEMENT *)(pElement))->iAttribute=iAttribute;
		return TRUE;
	}
	else
		return FALSE;
}

/*========================================================================
	Set_Char_PxAttribute
==========================================================================*/
BOOL Set_Char_PxAttribute(pDECSTR pDecStr,pELEMENTTYPE pElement,UINT16 iAttribute)
{
	pPROV_PROVELEMENT pParentElm = (PROV_PROVELEMENT *) pDecStr->parentElement;
	
	if ( pParentElm->iType == Prov_Type_Characteristic && 
		(pParentElm->iAttribute == PRVNG_PXLOGICAL || PRVNG_PXPHYSICAL))
	{
		((PROV_PROVELEMENT *)(pElement))->iAttribute=iAttribute;
		return TRUE;
	}
	else
		return FALSE;
}

/*========================================================================
	Parm_Set_Token_Value
==========================================================================*/
BOOL Parm_Set_Token_Value (pELEMENTTYPE pElement, pELEMENTTYPE *ppAttrVal, UINT16 Lower_Limit, UINT16 Upper_Limit)
{
	UINT16 iToken;
	pELEMENTTYPE pElementTemp = *ppAttrVal;

	if (pElementTemp->iType == Type_Token_Value){
		iToken = ((TOKENELEMENT *)(pElementTemp))->iTokenData;
		if (Lower_Limit <= iToken && iToken <= Upper_Limit){
				((PROV_PARMELEMENT *)(pElement))->iValue = iToken;
				DEALLOC(ppAttrVal);
				return TRUE;
		}
		else
			return FALSE;
	}	
	else
	{
		((PROV_PARMELEMENT *)(pElement))->iValue = 0;
		return FALSE;
	}
}			

/*========================================================================
	Parm_Set_String_Value
==========================================================================*/
BOOL Parm_Set_String_Value (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAttrVal)
{
	pELEMENTTYPE pElementTemp = *ppAttrVal;

	if (pElementTemp != NULL)
	{
		if (pElementTemp->iType == Type_Inline_Text)
		{
			if (((PROV_PARMELEMENT *)(pElement))->pwchValue == NULL)	
			{
				WCHAR *pchTempText=XML_ConvertToString(ppAttrVal,pDecStr);
				((PROV_PARMELEMENT *)(pElement))->pwchValue = pchTempText;
				return TRUE;
			}
		}
		else if (pElementTemp->iType==Type_Table_Text)
		{
			/* Get string from string table */
			WCHAR* pwchTempText = CreateStringCopy(WBXML_GetStrTabVal 
							(pDecStr,((pT_TEXTELEMENT)pElementTemp)->iTableRef));
			
			if (pwchTempText!=NULL)
			{
				((PROV_PARMELEMENT *)(pElement))->pwchValue = pwchTempText;
				return TRUE;
			}
			DEALLOC(&pwchTempText);
		}
	}

	return FALSE;
}			

/*========================================================================
	Check_Parm_Name_Ok
==========================================================================*/
BOOL Check_Parm_Name_Ok(pDECSTR pDecStr, pELEMENTTYPE pElement, UINT16 iAttribute, pELEMENTTYPE* ppAttrVal)
{
	BOOL fResult=TRUE;
	BOOL e2eSecFlag = FALSE;
	pELEMENTTYPE pElementTemp = *ppAttrVal;
	pPROV_PROVELEMENT pParentElm = (PROV_PROVELEMENT *) pDecStr->parentElement;

	if (pDecStr->iContentLevel == 1)
		e2eSecFlag = TRUE;
	
	if (pParentElm->iAttribute == PRVNG_PXLOGICAL)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_NAME:
			case PRVNG_Parm_PROXY_ID:
			case PRVNG_Parm_PROXY_PROVIDER_ID:
			case PRVNG_Parm_DOMAIN:
			case PRVNG_Parm_MASTER:
			case PRVNG_Parm_STARTPAGE:
			case PRVNG_Parm_BASAUTH_ID:
			case PRVNG_Parm_BASAUTH_PW:
			case PRVNG_Parm_WSP_VERSION:
			case PRVNG_Parm_PUSHENABLED:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			/* E2E security (navigation-doc) must not contain the TRUST parm */
			case PRVNG_Parm_TRUST:
				if (e2eSecFlag)
					{
						fResult = FALSE;
						break;
					}
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;;
				break;
			default:
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}
		
	else if (pParentElm->iAttribute == PRVNG_PXPHYSICAL)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_PHYSICAL_PROXY_ID:
			case PRVNG_Parm_DOMAIN:
			case PRVNG_Parm_PXADDR:
			case PRVNG_Parm_PXADDRTYPE:
			case PRVNG_Parm_WSP_VERSION:
			case PRVNG_Parm_PUSHENABLED:
			case PRVNG_Parm_TO_NAPID:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			default:
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}
		
	else if (pParentElm->iAttribute == PRVNG_PORT)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_PORTNBR:
			case PRVNG_Parm_SERVICE:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			default:
			/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}
		
	else if (pParentElm->iAttribute == PRVNG_VALIDITY)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_COUNTRY:
			case PRVNG_Parm_NETWORK:
			case PRVNG_Parm_SID:
			case PRVNG_Parm_SOC:
			case PRVNG_Parm_VALIDUNTIL:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			default:
			/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}
	
	else if (pParentElm->iAttribute == PRVNG_NAPDEF)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_NAPID:
			case PRVNG_Parm_BEARER:
			case PRVNG_Parm_NAME:
			case PRVNG_Parm_INTERNET:
			case PRVNG_Parm_NAP_ADDRESS:
			case PRVNG_Parm_NAP_ADDRTYPE:
			case PRVNG_Parm_CALLTYPE:
			case PRVNG_Parm_LOCAL_ADDR:
			case PRVNG_Parm_LOCAL_ADDRTYPE:
			case PRVNG_Parm_LINKSPEED:
			case PRVNG_Parm_DNLINKSPEED:
			case PRVNG_Parm_LINGER:
			case PRVNG_Parm_DELIVERY_ERR_SDU:
			case PRVNG_Parm_DELIVERY_ORDER:
			case PRVNG_Parm_TRAFFIC_CLASS:
			case PRVNG_Parm_MAX_SDU_SIZE:
			case PRVNG_Parm_MAX_BITRATE_UPLINK:
			case PRVNG_Parm_MAX_BITRATE_DNLINK:
			case PRVNG_Parm_RESIDUAL_BER:
			case PRVNG_Parm_SDU_ERROR_RATIO:
			case PRVNG_Parm_TRAFFIC_HANDL_PRIO:
			case PRVNG_Parm_TRANSFER_DELAY:
			case PRVNG_Parm_GUARANTEED_BITRATE_UPLINK:
			case PRVNG_Parm_GUARANTEED_BITRATE_DNLINK:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			default:
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}

	else if (pParentElm->iAttribute == PRVNG_BOOTSTRAP)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_COUNTRY:
			case PRVNG_Parm_NETWORK:
			case PRVNG_Parm_NAME:
			case PRVNG_Parm_PROVURL:
			case PRVNG_Parm_PROXY_ID:
			case PRVNG_Parm_CONTEXT_ALLOW:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			default:
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}

	else if (pParentElm->iAttribute == PRVNG_VENDOR_CONFIG)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_NAME:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			case PRVNG_Parm_LITERAL:
				if (pElementTemp->iType == Type_Inline_Text)
				{
					if (((PROV_PARMELEMENT *)(pElement))->pwchName == NULL)	
					{
						WCHAR *pchTempText=XML_ConvertToString(ppAttrVal,pDecStr);
						((PROV_PARMELEMENT *)(pElement))->pwchName = pchTempText;
						((PROV_PARMELEMENT *)(pElement))->iName = PRVNG_Parm_LITERAL;
					}
				}	
				else
					fResult = FALSE;
				break;
			default:
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}

	else if (pParentElm->iAttribute == PRVNG_CLIENTIDENTITY)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_CLIENT_ID:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			default:
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}

	else if (pParentElm->iAttribute == PRVNG_PXAUTHINFO)
	{
		switch (iAttribute)
		{
			case PRVNG_Parm_PXAUTH_TYPE:
			case PRVNG_Parm_PXAUTH_ID:
			case PRVNG_Parm_PXAUTH_PW:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			default:
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}

	else if (pParentElm->iAttribute == PRVNG_NAPAUTHINFO)
	{
		switch (iAttribute)
		{
			case PRVNG_name_AUTHTYPE:
			case PRVNG_Parm_AUTHNAME:
			case PRVNG_Parm_AUTHSECRET:
				((PROV_PARMELEMENT *)(pElement))->iName=iAttribute;
				break;
			default:
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
		}
	}

	else
		fResult = FALSE;
	return fResult;
}


/*========================================================================
	Set_Parm_Value
==========================================================================*/
BOOL Set_Parm_Value (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAttrVal)
{
	BOOL fResult=TRUE;
	pPROV_PARMELEMENT pParmElm = (PROV_PARMELEMENT *) pElement;
	pELEMENTTYPE pElementTemp = *ppAttrVal;
	
	switch (pParmElm->iName)
	{
		/* The following shall have one of the predefined Values */
		case PRVNG_Parm_NAP_ADDRTYPE:
		case PRVNG_Parm_PXADDRTYPE:
		case PRVNG_Parm_LOCAL_ADDRTYPE:
			fResult = Parm_Set_Token_Value(pElement, ppAttrVal, PRVNG_IPV4, PRVNG_MAN);
			break;
		case PRVNG_Parm_CALLTYPE:
			fResult = Parm_Set_Token_Value(pElement, ppAttrVal, PRVNG_ANALOG_MODEM, PRVNG_DIRECT_ASCNCHRONOUS_DATA_SERVICE);
			break;
		case PRVNG_name_AUTHTYPE:
		case PRVNG_Parm_PXAUTH_TYPE:
			fResult = Parm_Set_Token_Value(pElement, ppAttrVal, PRVNG_PAP, PRVNG_WTLS_SS);
			break;
		case PRVNG_Parm_BEARER:
			fResult = Parm_Set_Token_Value(pElement, ppAttrVal, PRVNG_GSM_USSD, PRVNG_MOBITEX_MPAK);
			break;
		case PRVNG_Parm_LINKSPEED:
		case PRVNG_Parm_DNLINKSPEED:
			if (pElementTemp->iType == Type_Token_Value)
				fResult = Parm_Set_Token_Value(pElement, ppAttrVal, PRVNG_AUTOBAUDING, PRVNG_AUTOBAUDING);
			else if (pElementTemp->iType == Type_Inline_Text)
				fResult = Parm_Set_String_Value (pDecStr, pElement, ppAttrVal);
			break;
		case PRVNG_Parm_SERVICE:
			fResult = Parm_Set_Token_Value(pElement, ppAttrVal, PRVNG_CL_WSP, PRVNG_CO_SEC_WTA);
			break;
			/* The following shall have a string value */
		case PRVNG_Parm_LITERAL:	
		case PRVNG_Parm_NAME:	
		case PRVNG_Parm_NAP_ADDRESS:								
		case PRVNG_Parm_VALIDUNTIL:									
		case PRVNG_Parm_AUTHNAME:				
		case PRVNG_Parm_AUTHSECRET:				
		case PRVNG_Parm_LINGER:									
		case PRVNG_Parm_NAPID:					
		case PRVNG_Parm_COUNTRY:				
		case PRVNG_Parm_NETWORK:				
		case PRVNG_Parm_INTERNET:				
		case PRVNG_Parm_PROXY_ID:				
		case PRVNG_Parm_PROXY_PROVIDER_ID:			
		case PRVNG_Parm_DOMAIN:					
		case PRVNG_Parm_PROVURL:								
		case PRVNG_Parm_PXAUTH_ID:				
		case PRVNG_Parm_PXAUTH_PW:			
		case PRVNG_Parm_STARTPAGE:				
		case PRVNG_Parm_BASAUTH_ID:				
		case PRVNG_Parm_BASAUTH_PW:				
		case PRVNG_Parm_PUSHENABLED:				
		case PRVNG_Parm_PXADDR:								
		case PRVNG_Parm_TO_NAPID:				
		case PRVNG_Parm_PORTNBR:												
		case PRVNG_Parm_LOCAL_ADDR:							
		case PRVNG_Parm_CONTEXT_ALLOW:				
		case PRVNG_Parm_TRUST:					
		case PRVNG_Parm_MASTER:					
		case PRVNG_Parm_SID:					
		case PRVNG_Parm_SOC:						
		case PRVNG_Parm_WSP_VERSION:				
		case PRVNG_Parm_PHYSICAL_PROXY_ID:		
		case PRVNG_Parm_CLIENT_ID:					
		case PRVNG_Parm_DELIVERY_ERR_SDU:			
		case PRVNG_Parm_DELIVERY_ORDER:				
		case PRVNG_Parm_TRAFFIC_CLASS:				
		case PRVNG_Parm_MAX_SDU_SIZE:				
		case PRVNG_Parm_MAX_BITRATE_UPLINK:			
		case PRVNG_Parm_MAX_BITRATE_DNLINK:		
		case PRVNG_Parm_RESIDUAL_BER:			
		case PRVNG_Parm_SDU_ERROR_RATIO:			
		case PRVNG_Parm_TRAFFIC_HANDL_PRIO:			
		case PRVNG_Parm_TRANSFER_DELAY:				
		case PRVNG_Parm_GUARANTEED_BITRATE_UPLINK:
		case PRVNG_Parm_GUARANTEED_BITRATE_DNLINK:
			fResult = Parm_Set_String_Value (pDecStr, pElement, ppAttrVal);
			break;
		default:
			/* Not valid attribute start token. */
			fResult=FALSE;
			break;
		}
	return fResult;
}


/*========================================================================
	Prov_CreatePROVELM
==========================================================================*/
pPROV_PROVELEMENT Prov_CreatePROVELM (UINT16 iType) 
{
	pPROV_PROVELEMENT pElement=NEWSTRUCT(PROV_PROVELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=iType;
		pElement->pNextElement=NULL;

		pElement->pContent=NULL;
		pElement->iAttribute = 0;
	}
	return pElement;
}

/*========================================================================
	Prov_CreatePARM
==========================================================================*/
pPROV_PARMELEMENT Prov_CreatePARM (void) 
{
	pPROV_PARMELEMENT pElement=NEWSTRUCT(PROV_PARMELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Prov_Type_Parm;
		pElement->pNextElement=NULL;

		pElement->iName = 0;
		pElement->pwchName = NULL;
		pElement->iValue = 0;      
		pElement->pwchValue = NULL;

	}
	return pElement;
}


/*========================================================================

	DTD functions and other external functions needed for handling 
	the WAP-PROVISIONINGDOC, CHARACTERISTIC and PARM elements.

=========================================================================*/

/*========================================================================
	Prov_CreateElement
==========================================================================*/
pELEMENTTYPE Prov_CreateElement (pDECSTR pDecStr, UINT16 iType)
{
	void *pvNewElm=NULL;

	pDecStr=pDecStr;

	/* Create specific element */
	switch (iType)
	{
	case Prov_Type_WapProvisioningDoc:
		pvNewElm=Prov_CreatePROVELM(iType);
		break;
	case Prov_Type_Characteristic:
		pvNewElm=Prov_CreatePROVELM(iType);
		break;
	case Prov_Type_Parm:
		pvNewElm=Prov_CreatePARM();
		break;
	}

	return (ELEMENTTYPE*)pvNewElm;
}

/*========================================================================
	Prov_ValidContent
==========================================================================*/
BOOL Prov_ValidContent (pDECSTR pDecStr, pELEMENTTYPE pElement)
{
	pDecStr=pDecStr;

	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Prov_Type_WapProvisioningDoc:
		case Prov_Type_Characteristic:
		case Prov_Type_Parm:

			return TRUE;
		}
	}

	return FALSE;
}

/*========================================================================
	Prov_GetContent
==========================================================================*/
pELEMENTTYPE Prov_GetContent (pELEMENTTYPE pElement)
{
	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Prov_Type_WapProvisioningDoc:
		case Prov_Type_Characteristic:

			/* Return content using generic function */
			return (XML_GetContent(pElement));
		}
	}
	return NULL;
}

/*========================================================================
	Prov_AppSpecParseMod
==========================================================================*/
void Prov_AppSpecParseMod (pDECSTR pDecStr)
{
	/* This function is not used in Provisioning */
	pDecStr=pDecStr;
}

/*========================================================================
	Prov_LiteralAttributeToToken
==========================================================================*/
UINT16 Prov_LiteralAttributeToToken (WCHAR* pwchToken)
{
	/* Dummy */
	UINT16 iToken=Type_Literal;
	pwchToken = NULL;
	return iToken;
}

/*========================================================================
	Prov_LiteralTagToToken
==========================================================================*/
UINT16 Prov_LiteralTagToToken (WCHAR* pwchToken)
{
		/* Dummy */
	UINT16 iToken=Type_Literal;
	pwchToken = NULL;
	return iToken;
}



/*========================================================================
	Prov_DeleteElement
==========================================================================*/
void Prov_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement)
{
	if (ppElement==NULL)
	{
		return;
	}

	if (*ppElement!=NULL) 
	{
		switch ((*ppElement)->iType) 
		{
		case Prov_Type_WapProvisioningDoc:
			XML_DeleteElementList(&(((PROV_PROVELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Prov_Type_Characteristic:
			XML_DeleteElementList(&(((PROV_PROVELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Prov_Type_Parm:
			if (((PROV_PARMELEMENT*)(*ppElement))->pwchValue != NULL)
				DEALLOC(&(((PROV_PARMELEMENT*)(*ppElement))->pwchValue));
			break;
		}

		/* Delete element */
		DEALLOC(ppElement);		
	}
}


/*========================================================================
	Prov_StoreAttributeValue
==========================================================================*/
BOOL Prov_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal)
{
	BOOL fResult=TRUE;
	BOOL e2eSecFlag = FALSE;

	if (pDecStr->iContentLevel == 1)
		e2eSecFlag = TRUE;
	
	if (pElement!=NULL)
	{
	
		if (pElement->iType == Prov_Type_WapProvisioningDoc)
		{
			if (pDecStr->parentElement == NULL)
			{
				if (iAttribute == ATTRST_version_1_0)
					((PROV_PROVELEMENT *)(pElement))->iAttribute=iAttribute;
			}
			else
				return FALSE;
		}

		else if (pElement->iType == Prov_Type_Characteristic)
		{
			switch (iAttribute)
			{	
				/* These five may only occur at the root of a connectivity document*/
				case PRVNG_PXLOGICAL:
				case PRVNG_NAPDEF:
				case PRVNG_VENDOR_CONFIG:
					fResult = Set_Char_RootAttribute(pDecStr,pElement,iAttribute);
					break;
				/* E2E security (navigation-doc) must not contain the following two characteristics */
				case PRVNG_BOOTSTRAP:
				case PRVNG_CLIENTIDENTITY:
					if (e2eSecFlag)
					{
						fResult = FALSE;
						break;
					}
					fResult = Set_Char_RootAttribute(pDecStr,pElement,iAttribute);
					break;
				/* These two may only occur inside a PXLOGICAL characteristic */
				case PRVNG_PXAUTHINFO:
				case PRVNG_PXPHYSICAL:
					fResult = Set_Char_PxlogicAttribute(pDecStr,pElement,iAttribute);
					break;
				/* These two may only occur at the root of a NAPDEF characteristic */
				case PRVNG_NAPAUTHINFO:
				case PRVNG_VALIDITY:
					fResult = Set_Char_NapdefAttribute(pDecStr,pElement,iAttribute);
					break;
				/* May only occur in PXLOGICAL and PXPHYSICAL */
				case PRVNG_PORT:
					fResult = Set_Char_PxAttribute(pDecStr,pElement,iAttribute);
					break;
/*				case PRVNG_type_LITERAL:
					fResult = Set_Char_StringName(......)  om vi tillåter string som type
					break; */
				default:
					/* Not valid attribute start token. */
					fResult=FALSE;
					break;
			}
		}
		
		else if (pElement->iType == Prov_Type_Parm)
		{
			if (iAttribute == PRVNG_Parm_VALUE)  
				fResult = Set_Parm_Value(pDecStr, pElement, ppAttrVal);	
			else
				fResult = Check_Parm_Name_Ok(pDecStr, pElement, iAttribute, ppAttrVal);
		}
	}

	/* Check if all elements in the attribute value were used in the 
	   store function. If not all elements used, return FALSE. 
	   This will result in a warning. Also, delete the unused element
	   to avoid memory leak. */
/*	if (*ppAttrVal!=NULL)
	{
		fResult=FALSE;
		XML_DeleteElementList(ppAttrVal,pDecStr);
	}*/	
	return fResult;
}

/*========================================================================
	Prov_GetAttributeValue
==========================================================================*/
WCHAR* Prov_GetAttributeValue (UINT16 iToken)
{
	/* Dummy Function: Never used for connectivity doc, since Token values are ok */
	iToken  = 0;
	return NULL;
}


/*========================================================================
	PROV_CheckPublicID
==========================================================================*/
BOOL PROV_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	/* Use string */
	if (pwchPId!=NULL)
	{
		UINT32 iLength=0;

		/* PROV 1.0 */
		WCHAR pwchPublic_ID_1_0[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','P','R','O','V',' ','1',
								   '.','0','/','/','E','N',0};

		/* Get length of pwchPId string */
		iLength=(UINT32) (w_strlen(pwchPId));

		if (CompareWideStringNoCase (pwchPId,pwchPublic_ID_1_0,iLength))
		{
			/* Prov 1.0 - OK */
			*piPId=PROV_VERSION_1_0;
			return TRUE;
		}
		else
			return FALSE;

		/* Check content-type and level (0x36 is application/vnd.wap.connectivity-wbxml) */
	/*	if (pDecStr->bContentType=='\x36')
		{

			*piPId=PROV_VERSION_1_0;
			return TRUE;	
		}*/
	}

	/* Use integer */
	else
	{
		/* Public ID (Prov 1.0) is 0x0B */

		switch (*piPId)
		{

		/* Unknown */
		case '\x01':
			/* Check content-type and level (0x36 is application/vnd.wap.connectivity-wbxml)*/
			if (pDecStr->bContentType=='\x36')
			{
				*piPId=PROV_VERSION_1_0;
				return TRUE;
			}
			return FALSE;

		/* Prov 1.0 */
		case '\x0B':
			*piPId=PROV_VERSION_1_0;
			return TRUE;
		}
	}

	/* Other - not supported */
	return FALSE;
}

/*========================================================================
	Prov_CreateTokenValueElem (Not a DTD function)
==========================================================================*/
pELEMENTTYPE Prov_CreateTokenValueElem(UINT16 iAttrVal)
{

	pTOKENELEMENT pElement=NEWSTRUCT(TOKENELEMENT);

	/* Store attributes */
	pElement->iType=Type_Token_Value;
	pElement->pNextElement=NULL;
	pElement->iTokenData = iAttrVal;

	/* Cast to super type and return */
	return (pELEMENTTYPE) pElement;
}
