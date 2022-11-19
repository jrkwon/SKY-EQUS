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
	FILE: WMLParse.c
  
	Source for WML-specific parse

	Rev history:
		980706	JPR		Created
		981223	JPR		Event concept changed	
		990125	JPR		EndParser updated					
		990211	JPR		Corrections						
		990212	JPR		Attribute handling corrected		
		990312	JPR		Major changes. Handles the WML-specific parts 
						of the parsing.
		990316	JPR		Function WML_ModifyParse added.
		990317	JPR		Corrections
		990329	JPR		WAP 1.1
		990330	JPR		Memory leak removed
		990414	JPR		Corrections
		990422	JPR		Support for LITERALS
		990512	JPR		Corrections and extended error handling
		990526	JPR		Paragraph updates
		990602	JPR		LITERAL Correction
		991011	JPR		Correction in function WML_IsPEmptyInternal
		991028	JPR		WTA specific parts added

   ================================================================== */

#include "WMLParse.h"
#include "WMLToken.h"
#include "WBXMLDec.h"

/* WTA specific */
#ifdef CONFIG_WTA
#include "tapimmi.h"
#endif

BOOL WML_CheckPublicIDInt (pWBXMLDECODESTR pDecStr, UINT8 iPId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	/* Public ID (wml 1.1) is 0x04 */

	/* Check content-type and level (0x14 is applictaion/vnd.wap.wmlc)*/
	if (pDecStr->bContentType=='\x14')
	{
		/* Check level (0 - not specified or '\x11' - 1.1 ) */
		if ((pDecStr->iContentLevel==0)||(pDecStr->iContentLevel=='\x11'))
		{
			/* OK */
			return TRUE;
		}
	}

	if (iPId==4)
	{
		/* Public ID is wml 1.1 - ignore content-type decl. at WSP level */
		return TRUE;
	}

	/* Check if wml 1.0 or unknown */
	if ((iPId==1)||(iPId==2))
	{
		/* Accept, but send a warning to the application. */
		CLNTa_error (pDecStr->iViewID,ERR_WAE_WML_CONTENT_PUBLIC_ID_WARNING,
						ERRTYPE_INFORMATION);
		return TRUE;
	}

	/* Not accepted. Send warning to the application. */
	CLNTa_error (pDecStr->iViewID,ERR_WAE_WML_CONTENT_PUBLIC_ID_ERROR,
					ERRTYPE_INFORMATION);
	return FALSE;  
}

BOOL WML_CheckPublicIDWChar (pWBXMLDECODESTR pDecStr, WCHAR *pchId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	UINT32 iLength=0;

	WCHAR pchPublic_ID_text[]={'-','/','/','W','A','P','F','O','R','U','M',
						  '/','/','D','T','D',' ','w','m','l',' ','1',
						  '.','1','/','/','E','N',0};

	WCHAR pchPublic_ID_url[]={'h', 't', 't', 'p', ':', '/', '/', 'w', 'w', 
									'w', '.', 'w', 'a', 'p', 'f', 'o', 'r', 'u', 
									'm', '.', 'o', 'r', 'g', '/', 'D', 'T', 'D', 
									'/', 'w', 'm', 'l', '_', '1', '.', '1', '.',
									'x', 'm', 'l', 0};

	/* Check content-type and level (0x14 is applictaion/vnd.wap.wmlc)*/
	if (pDecStr->bContentType=='\x14')
	{
		/* Check level (0 - not specified or '\x11' - 1.1 ) */
		if ((pDecStr->iContentLevel==0)||(pDecStr->iContentLevel=='\x11'))
		{
			/* OK */
			return TRUE;
		}
	}

	/* Public ID (WML 1.1) -> '-//WAPFORUM//DTD WML 1.1//EN'  */
	if (pchId!=NULL) 
	{
		/* Get length of one string */
		iLength=(UINT32) (w_strlen(pchId));

		if (COMPARESTRING(pchId,pchPublic_ID_text)==0) 
		{
			return TRUE;
		}
		else if (CompareWideStringNoCase (pchId,pchPublic_ID_url,iLength))
		{
			return TRUE;
		}
	}

	/* Accept, but send a warning to the application. */
	CLNTa_error (pDecStr->iViewID,ERR_WAE_WML_CONTENT_PUBLIC_ID_WARNING,
					ERRTYPE_INFORMATION);
	return TRUE;  
}

void WML_SetParserCharset (pWBXMLDECODESTR pDecStr, INT16 iCharset)
{
	/* Set charset - 0 indicates unknown charset. 
	   Follow the */
	if (pDecStr->iCharset==IANA_CHARSET_INVALID)
	{
		/* Set charset */
		if (iCharset!=0)
		{
			pDecStr->iCharset=iCharset;
		}
		else
		{
			/* Default charset is UTF-8 */
			pDecStr->iCharset=IANA_CHARSET_UTF8;
		}

	}
	else
	{
		/* Keep old charset */
	}

}

WCHAR *WML_ParseAttributeValue (pWBXMLDECODESTR pDecStr)
{
	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		if (pDecStr->iAttrCodePage==0)
		{
			switch (*pDecStr->pbCurByte)
			{
			case ATTRVAL_com:
				return WBXML_CopyCharToWChar(pDecStr,".com/");
			case ATTRVAL_edu:
				return WBXML_CopyCharToWChar(pDecStr,".edu/");
			case ATTRVAL_net:
				return WBXML_CopyCharToWChar(pDecStr,".net/");
			case ATTRVAL_org:
				return WBXML_CopyCharToWChar(pDecStr,".org/");
			case ATTRVAL_accept:
				return WBXML_CopyCharToWChar(pDecStr,"accept");
			case ATTRVAL_bottom:
				return WBXML_CopyCharToWChar(pDecStr,"bottom");
			case ATTRVAL_clear:
				return WBXML_CopyCharToWChar(pDecStr,"clear");
			case ATTRVAL_delete:
				return WBXML_CopyCharToWChar(pDecStr,"delete");
			case ATTRVAL_help:
				return WBXML_CopyCharToWChar(pDecStr,"help");
			case ATTRVAL_http:
				return WBXML_CopyCharToWChar(pDecStr,"http://\0");
			case ATTRVAL_http_www:
				return WBXML_CopyCharToWChar(pDecStr,"http://www.");
			case ATTRVAL_https:
				return WBXML_CopyCharToWChar(pDecStr,"https://\0");
			case ATTRVAL_https_www:
				return WBXML_CopyCharToWChar(pDecStr,"https://www.");
			case ATTRVAL_middle:
				return WBXML_CopyCharToWChar(pDecStr,"middle");
			case ATTRVAL_nowrap:
				return WBXML_CopyCharToWChar(pDecStr,"nowrap");
			case ATTRVAL_onpick:
				return WBXML_CopyCharToWChar(pDecStr,"onpick");
			case ATTRVAL_onenterbackward:
				return WBXML_CopyCharToWChar(pDecStr,"onenterbackward");
			case ATTRVAL_onenterforward:
				return WBXML_CopyCharToWChar(pDecStr,"onenterforward");
			case ATTRVAL_ontimer:
				return WBXML_CopyCharToWChar(pDecStr,"ontimer");
			case ATTRVAL_options:
				return WBXML_CopyCharToWChar(pDecStr,"options");
			case ATTRVAL_password:
				return WBXML_CopyCharToWChar(pDecStr,"password");
			case ATTRVAL_reset:
				return WBXML_CopyCharToWChar(pDecStr,"reset");
			case ATTRVAL_text:
				return WBXML_CopyCharToWChar(pDecStr,"text");
			case ATTRVAL_top:
				return WBXML_CopyCharToWChar(pDecStr,"top");
			case ATTRVAL_unknown:
				return WBXML_CopyCharToWChar(pDecStr,"unknown");
			case ATTRVAL_wrap:
				return WBXML_CopyCharToWChar(pDecStr,"wrap");
			case ATTRVAL_www:
				return WBXML_CopyCharToWChar(pDecStr,"www.");
			}
		}
	}
	return NULL;
}

void WML_ParseAttribute (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pElement, 
						 BYTE bAttribute)
{	
	pTEXTELEMENT pAttrVal=NULL;
	BOOL fWTA=FALSE;

	#ifdef CONFIG_WTA
	/* Set the fWTA to TRUE if the user agent is WTA enabled,
	   FALSE otherwise. This is used when the WML_SetType 
	   function is called. */

	if (((UA*)(pDecStr->pAppSpec))->iUserAgentMode==WTA_UA)
	{
		fWTA=TRUE;
	}
	#endif

	if (pDecStr->pbCurByte<pDecStr->pbEnd)
	{
		if (pDecStr->iAttrCodePage==0)
		{
			/* Parse attribute value */
			pAttrVal=WBXML_ParseAttributeValue (pDecStr);

			if (pElement!=NULL)
			{
				/* Store attribute value */
				switch (bAttribute)
				{
				case ATTRST_accept_charset:			
					WML_SetAccChar(pElement,pAttrVal);
					break;
				case ATTRST_align:
					WML_SetAlign(pElement,pAttrVal,DEF_TYPE_STRING);
					break;
				case ATTRST_align_bottom:			
					WML_SetAlign(pElement,pAttrVal,ALIGN_BOTTOM);
					break;
				case ATTRST_align_center:
					WML_SetAlign(pElement,pAttrVal,ALIGN_CENTER);
					break;
				case ATTRST_align_left:
					WML_SetAlign(pElement,pAttrVal,ALIGN_LEFT);
					break;
				case ATTRST_align_middle:
					WML_SetAlign(pElement,pAttrVal,ALIGN_MIDDLE);
					break;
				case ATTRST_align_right:
					WML_SetAlign(pElement,pAttrVal,ALIGN_RIGHT);
					break;
				case ATTRST_align_top:
					WML_SetAlign(pElement,pAttrVal,ALIGN_TOP);
					break;
				case ATTRST_alt:
					WML_SetAlt(pElement,pAttrVal);
					break; 
				case ATTRST_class:
					/* Not used - delete */
					Text_DeleteText(&pAttrVal);
					break; 
				case ATTRST_columns:
					WML_SetColumns(pElement,pAttrVal);
					break; 
				case ATTRST_content:
					WML_SetContent(pElement,pAttrVal);
					break;
				case ATTRST_content_wmlc_charset:
					Text_AddHeadString(&pAttrVal,WBXML_CopyCharToWChar(pDecStr,
										"application/vnd.wap.wmlc;charset="));
					WML_SetContent(pElement,pAttrVal);
					break;
				case ATTRST_domain:
					WML_SetDomain(pElement,pAttrVal);			
					break;
				case ATTRST_emptyok_false:
					WML_SetEmptyOK(pElement,FALSE,pAttrVal);
					break;
				case ATTRST_emptyok_true:
					WML_SetEmptyOK(pElement,TRUE,pAttrVal);
					break;
				case ATTRST_format:
					WML_SetFormat(pElement,pAttrVal);
					break;
				case ATTRST_forua_false:
					WML_SetForua(pElement,FALSE,pAttrVal);
					break;
				case ATTRST_forua_true:
					WML_SetForua(pElement,TRUE,pAttrVal);
					break;
				case ATTRST_height:
					WML_SetHeight(pElement,pAttrVal);
					break;
				case ATTRST_href:
					WML_SetHref(pElement,pAttrVal);
					break;
				case ATTRST_href_http:
					Text_AddHeadString(&pAttrVal,WBXML_CopyCharToWChar(pDecStr,"http://"));
					WML_SetHref(pElement,pAttrVal);
					break;
				case ATTRST_href_https:
					Text_AddHeadString(&pAttrVal,WBXML_CopyCharToWChar(pDecStr,"https://"));
					WML_SetHref(pElement,pAttrVal);
					break;
				case ATTRST_hspace:
					WML_SetHSpace(pElement,pAttrVal);
					break;
				case ATTRST_http_equiv:
					WML_SetHttpEq(pElement,pAttrVal);
					break;
				case ATTRST_http_equiv_Content_Type:
					Text_AddHeadString(&pAttrVal,WBXML_CopyCharToWChar(
								pDecStr,"Content-Type"));
					WML_SetHttpEq(pElement,pAttrVal);
					break;
				case ATTRST_http_equiv_Expires:
					Text_AddHeadString(&pAttrVal,WBXML_CopyCharToWChar(
								pDecStr,"Expires"));
					WML_SetHttpEq(pElement,pAttrVal);
					break;
				case ATTRST_id:
					WML_SetId(pElement,pAttrVal);
					break;
				case ATTRST_ivalue:
					WML_SetIValue(pElement,pAttrVal);
					break;
				case ATTRST_iname:
					WML_SetIName(pElement,pAttrVal);
					break;
				case ATTRST_label:
					WML_SetLabel(pElement,pAttrVal);
					break;
				case ATTRST_localsrc:
					WML_SetLocalSrc(pElement,pAttrVal);
					break;
				case ATTRST_maxlength:
					WML_SetMaxLength(pElement,pAttrVal);
					break;
				case ATTRST_method_get:
					WML_SetMethod(pElement,M_Get,pAttrVal);
					break;
				case ATTRST_method_post:
					WML_SetMethod(pElement,M_Post,pAttrVal);
					break;
				case ATTRST_mode_nowrap:
					WML_SetMode(pElement,P_NoWrap,pAttrVal);
					break;
				case ATTRST_mode_wrap:
					WML_SetMode(pElement,P_Wrap,pAttrVal);
					break;
				case ATTRST_multiple_false:
					WML_SetMultiple(pElement,FALSE,pAttrVal);
					break;
				case ATTRST_multiple_true:
					WML_SetMultiple(pElement,TRUE,pAttrVal);
					break;
				case ATTRST_name: 
					WML_SetName(pElement,pAttrVal);
					break;
				case ATTRST_newcontext_false:
					WML_SetNewContext(pElement,FALSE,pAttrVal);
					break;
				case ATTRST_newcontext_true:
					WML_SetNewContext(pElement,TRUE,pAttrVal);
					break;
				case ATTRST_onenterbackward:
					/* Create super element to be conformant with
					   onevent action other than url:s, i.e., all
					   action must be of the type ELEMENTTYPE. */
					{
						pELEMENTTYPE pTemp=NULL;

						pTemp=XML_CreateTextElement(&pAttrVal);
						WML_SetOnEntBack(pElement,&pTemp);

						if (pTemp!=NULL)
						{
							/* Add the event ONENTERBACKWARD to the eventtable */
							WML_AddToEventTable(pElement,DEF_TYPE_ONENTERBACKWARD,pTemp);
						}
					}
					break;
				case ATTRST_onenterforward:
					{
						pELEMENTTYPE pTemp=NULL;

						pTemp=XML_CreateTextElement(&pAttrVal);
						WML_SetOnEntForw(pElement,&pTemp);

						if (pTemp!=NULL)
						{
							/* Add the event ONENTERFORWARD to the eventtable */
							WML_AddToEventTable(pElement,DEF_TYPE_ONENTERFORWARD,pTemp);
						}
					}
					break;
				case ATTRST_onpick:
					{
						pELEMENTTYPE pTemp=NULL;

						pTemp=XML_CreateTextElement(&pAttrVal);
						WML_SetOnPick(pElement,&pTemp);
					}
					break;
				case ATTRST_ontimer:
					{
						pELEMENTTYPE pTemp=NULL;

						pTemp=XML_CreateTextElement(&pAttrVal);
						WML_SetOnTimer(pElement,&pTemp);

						if (pTemp!=NULL)
						{
							/* Add the event ONTIMER to the eventtable */
							WML_AddToEventTable(pElement,DEF_TYPE_ONTIMER,pTemp);
						}
					}
					break;
				case ATTRST_optional_false:
					WML_SetOptional(pElement,FALSE,pAttrVal);
					break;
				case ATTRST_optional_true:
					WML_SetOptional(pElement,TRUE,pAttrVal);
					break;
				case ATTRST_path:
					WML_SetPath(pElement,pAttrVal);			
					break;
				case ATTRST_scheme:
					WML_SetScheme(pElement,pAttrVal);			
					break;
				case ATTRST_sendreferer_false:
					WML_SetSendReferer(pElement,FALSE,pAttrVal);
					break;
				case ATTRST_sendreferer_true:
					WML_SetSendReferer(pElement,TRUE,pAttrVal);
					break;
				case ATTRST_size:
					WML_SetSize(pElement,pAttrVal);
					break;
				case ATTRST_src:
					WML_SetSrc(pElement,pAttrVal);
					break;
				case ATTRST_src_http:
					Text_AddHeadString(&pAttrVal,WBXML_CopyCharToWChar(
								pDecStr,"http://"));
					WML_SetSrc(pElement,pAttrVal);
					break;
				case ATTRST_src_https:
					Text_AddHeadString(&pAttrVal,WBXML_CopyCharToWChar(
								pDecStr,"https://"));
					WML_SetSrc(pElement,pAttrVal);
					break;
				case ATTRST_ordered_true:
					WML_SetOrdered(pElement,TRUE,pAttrVal);
					break;
				case ATTRST_ordered_false:
					WML_SetOrdered(pElement,FALSE,pAttrVal);
					break;
				case ATTRST_tabindex:
					WML_SetTabIndex(pElement,pAttrVal);
					break;
				case ATTRST_title:
					WML_SetTitle(pElement,pAttrVal);
					break;
				case ATTRST_type:
					/* ONEVENT, DO, INPUT */
					WML_SetType(pElement,DEF_TYPE_STRING,Text_ToString(&pAttrVal),fWTA);
					break;
				case ATTRST_type_accept:
					WML_SetType(pElement,DEF_TYPE_STRING,WBXML_CopyCharToWChar(pDecStr,"accept"),fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_delete:
					WML_SetType(pElement,DEF_TYPE_STRING,WBXML_CopyCharToWChar(pDecStr,"delete"),fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_help:
					WML_SetType(pElement,DEF_TYPE_STRING,WBXML_CopyCharToWChar(pDecStr,"help"),fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_password:
					WML_SetType(pElement,DEF_TYPE_PASSWORD,NULL,fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_onpick:
					WML_SetType(pElement,DEF_TYPE_ONCLICK,NULL,fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_onenterbackward:
					WML_SetType(pElement,DEF_TYPE_ONENTERBACKWARD,NULL,fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_onenterforward:
					WML_SetType(pElement,DEF_TYPE_ONENTERFORWARD,NULL,fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_ontimer:
					WML_SetType(pElement,DEF_TYPE_ONTIMER,NULL,fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_options:
					WML_SetType(pElement,DEF_TYPE_STRING,WBXML_CopyCharToWChar(pDecStr,"options"),fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_prev:
					WML_SetType(pElement,DEF_TYPE_STRING,WBXML_CopyCharToWChar(pDecStr,"prev"),fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_reset:
					WML_SetType(pElement,DEF_TYPE_STRING,WBXML_CopyCharToWChar(pDecStr,"reset"),fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_text:
					WML_SetType(pElement,DEF_TYPE_TEXT,NULL,fWTA);
					Text_DeleteText(&pAttrVal);
					break;
				case ATTRST_type_vnd:
					Text_AddHeadString(&pAttrVal,WBXML_CopyCharToWChar(pDecStr,"vnd."));
					WML_SetType(pElement,DEF_TYPE_STRING,Text_ToString(&pAttrVal),fWTA);
					break;
				case ATTRST_value:
					WML_SetValue(pElement,pAttrVal);
					break;
				case ATTRST_vspace:
					WML_SetVSpace(pElement,pAttrVal);
					break;
				case ATTRST_width:
					WML_SetWidth(pElement,pAttrVal);
					break;
				case ATTRST_xml_lang:
					WML_SetXmllang(pElement,pAttrVal);
					break;
				default:
					/* Not valid attribute start token. */
					Text_DeleteText(&pAttrVal);

					/* Send warning to the application. */
					CLNTa_error (pDecStr->iViewID,ERR_WAE_WML_UNKNOWN_TOKEN,
							ERRTYPE_INFORMATION);
					break;
				}
			}
			else
			{
				/* No element to store the attribute in */
				Text_DeleteText(&pAttrVal);
			}
		}
	}
}

BYTE WML_ConvertTokenToByte (WCHAR *pchToken, const WCHAR * const ppchTokenTab[],
							 const BYTE *pbByteTab, UINT16 iTabLen)
{
	BYTE bResult=Type_Literal;
	UINT8 iCount=2;
	BOOL fFound=FALSE;
	UINT16 iIndex=(UINT16)(iTabLen/2);
	UINT16 iMod=iIndex;
	INT8 iCheck=0;
	WCHAR *pchRead=NULL;
	WCHAR *pchWrite=NULL;

	pchWrite=pchToken;
	pchRead=pchToken;

	/* Remove all white space before and after name */
	if (pchToken!=NULL)
	{
		while (*pchRead!=0)
		{
			if (((*pchRead==(WCHAR)'\x20')||(*pchRead==(WCHAR)'\x9')||
					(*pchRead==(WCHAR)'\xD')||(*pchRead==(WCHAR)'\xA')))
			{
				/* Do not step write pos */
				pchRead++;
			}
			else
			{
				/* Not white space - write */
				*pchWrite++=*pchRead++;
			}
		}

		/* Set termination char */
		*pchWrite=0;

		/* Binary search */
		while ((!fFound) && (iIndex<iTabLen) && (iCount))
		{
			/* Check this index */
			iCheck=COMPARESTRING (pchToken,(ppchTokenTab[iIndex]));

			/* Find next index modifier */
			if (iMod%2==0)
			{
				iMod/=2;
			}
			else
			{
				iMod=(UINT16)((iMod/2)+1);
			}

			if (iCheck==0)
			{
				/* Equal */
				fFound=TRUE;

				bResult=pbByteTab[iIndex];
			}
			else if (iCheck<0)
			{
				/* Search left subtree */
				iIndex-=iMod;
			}
			else
			{
				/* Search right subtree */
				iIndex+=iMod;
			}

			/* To stop if not found */
			if (iMod==1)
			{
				iCount--;
			}
		}
	}

	return bResult;
}

BYTE WML_ConvertAttributeToByte (pWBXMLDECODESTR pDecStr, UINT32 iPos)
{
	WCHAR *pchAttribute=NULL;
	BYTE bResult=Type_Literal;

	/* ASSERT:	pDecStr!=NULL
	*/

	/* Get string from string table */
	pchAttribute=WBXML_GetStrTabVal (pDecStr, iPos);

	/* Compare string */
	if (pchAttribute!=NULL)
	{
		bResult=WML_ConvertTokenToByte (pchAttribute, ppchAttrTokens,
							 pbAttrBytes, STAT_NBROFATTR);
	}

	return bResult;
}

BYTE WML_ConvertTagToByte (pWBXMLDECODESTR pDecStr, UINT32 iPos)
{
	WCHAR *pchTag=NULL;
	BYTE bResult=Type_Literal;

	/* ASSERT:	pDecStr!=NULL
	*/

	/* Get string from string table */
	pchTag=WBXML_GetStrTabVal (pDecStr, iPos);

	/* Compare string */
	if (pchTag!=NULL)
	{
		bResult=WML_ConvertTokenToByte (pchTag, ppchTagTokens,
							 pbTagBytes, STAT_NBROFTAGS);
	}

	return bResult;
}


/* Returns TRUE if the text contains only white space
   characters or if it is NULL. FALSE otherwise. */
BOOL WML_CheckIfOnlyWhiteSpace (WCHAR *pchText)
{
	if (pchText!=NULL)
	{
		while (*pchText!=0)
		{
			/* White spaces in wml */
			if (!((*pchText==(WCHAR)'\x20')||(*pchText==(WCHAR)'\x9')||
				(*pchText==(WCHAR)'\xD')||(*pchText==(WCHAR)'\xA')))
			{
				/* Not white space */
				return FALSE;
			}
			
			/* Next character */
			pchText++;
		}
	}

	/* NULL -> return TRUE */
	return TRUE;
}

/*
 	Examines if a p element is insignificant, i.e.,
	NULL, empty, or only white space. In TEXT-
	ELEMENTS, variables are considered significant.
*/
BOOL WML_IsPEmptyInternal (pELEMENTTYPE pPElement)
{
	pELEMENTTYPE pContent=NULL;
	pTEXTELEMENT pText=NULL;
	WCHAR iEntity;

	if (pPElement==NULL)
	{
		/* NO ELEMENT - EMPTY (?) */
		return TRUE;
	}

	/* Get p content */
	pContent=WML_GetContent(pPElement);

	while (pContent!=NULL)
	{
		if (pContent->iType==Type_Text)
		{
			/* Text found, check if only white space */
			pText=(pTEXTELEMENT)(pContent->pElm);

			/* Static check of text element */
			while (pText!=NULL)
			{
				if (pText->iType==T_String)
				{
					if (!WML_CheckIfOnlyWhiteSpace (pText->pchTextData))
					{
						/* Not only white space found */
						return FALSE;
					}
				}
				else if (pText->iType==T_Entity)
				{
					/* Check if white space entity */
					iEntity=(WCHAR)(pText->iEntity);

					if (!((iEntity==(WCHAR)'\x20')||(iEntity==(WCHAR)'\x9')||
						(iEntity==(WCHAR)'\xD')||(iEntity==(WCHAR)'\xA')))
					{
						/* Not white space */
						return FALSE;
					}					
				}
				else
				{
					/* Variable - not white space */
					return FALSE;
				}

				/* Get next text part */
				pText=pText->pNextText;
			}
		}
		else
		{
			/* Other element found, return FALSE  
			   (p element not empty) */
			return FALSE;
		}

		/* Get next content */
		pContent=pContent->pNextElement;
	}

	/* p element is empty */
	return TRUE;
}

void WML_ModifyParse (pWBXMLDECODESTR pDecStr, pELEMENTTYPE *ppElement, pELEMENTTYPE pParent)
{
	if (*ppElement!=NULL)
	{
		if (pDecStr->iTagCodePage==0)
		{
			switch ((*ppElement)->iType)
			{
			case Type_P:
				/* Check if p is empty (Static check) */
				if (WML_IsPEmptyInternal(*ppElement))
				{
					/* Remove p */
					WML_DeleteElement(*ppElement);
					*ppElement=NULL;
				}
				else
				{
					/* Keep p and update current Wrap-mode */
					if (((PELEMENT *)((*ppElement)->pElm))->iMode==P_Unknown)
					{
						((PELEMENT *)((*ppElement)->pElm))->iMode=
							((UA *)(pDecStr->pAppSpec))->iCurrentWrapMode;
					}

					((UA *)(pDecStr->pAppSpec))->iCurrentWrapMode = 
						((PELEMENT *)((*ppElement)->pElm))->iMode;
				}
				break;
			case Type_ONEVENT:
				if (pParent!=NULL)
				{
					/* Only valid events are ONENTERFORWARD, ONENTERBACKWARD,
					   ONTIMER or ONCLICK. All other should be ignored. For
					   CARD and TEMPLATE the first three are valid and for
					   OPTION, the ONCLICK event is valid. */

                    /*
                    If WTA is enabled, other events are valid in the CARD
                    and TEMPLATE as well.
                    */

					/* Check if parent is OPTION */
					if (pParent->iType==Type_OPTION)
					{
						/* Check if the type is legal for OPTION. */
						if ( ((ONEVENTELEMENT *)((*ppElement)->pElm))->iType==DEF_TYPE_ONCLICK)
						{
							/* Remove conflicting task - implies non-fatal error */
							WML_DeleteElement(((OPTIONELEMENT *)(pParent->pElm))->pOnpick);

							/* Move the Action to "attribute level" */
							((OPTIONELEMENT *)(pParent->pElm))->pOnpick=
									((ONEVENTELEMENT *)((*ppElement)->pElm))->pContent;

							/* Remove the pointer to the task to allow the
							   use of DeleteElement without deleting the task. */
							((ONEVENTELEMENT *)((*ppElement)->pElm))->pContent=NULL;
							break;
						}
					}

					/* Check if parent is CARD or TEMPLATE */
					else if (((pParent->iType==Type_CARD)||(pParent->iType==Type_TEMPLATE))) 
					{
						/* Check if the type is legal for CARD or TEMPLATE. */
						if ((((ONEVENTELEMENT *)((*ppElement)->pElm))->iType==DEF_TYPE_ONTIMER)||
							(((ONEVENTELEMENT *)((*ppElement)->pElm))->iType==DEF_TYPE_ONENTERFORWARD)||
							(((ONEVENTELEMENT *)((*ppElement)->pElm))->iType==DEF_TYPE_ONENTERBACKWARD))
						{
							/* The event is legal. Add event to the parent (CARD or TEMPLATE). */
							WML_AddToEventTable(pParent, (((ONEVENTELEMENT *)((*ppElement)->pElm))->iType),
									(((ONEVENTELEMENT *)((*ppElement)->pElm))->pContent));
							break;
						}

						/* Check if WTA event (only if WTA is enabled AND the current user
						   agent is a WTA user agent) */
						#ifdef CONFIG_WTA
						if (((UA*)(pDecStr->pAppSpec))->iUserAgentMode==WTA_UA)
						{
							/* Allow WTA events as well Check if WTA event.
							   The following if-statement checks if the event is within the
							   legal boundaries for the current configuration.
							*/
							if (((((ONEVENTELEMENT *)((*ppElement)->pElm))->iType>=WTAEvent_cc_ic)&&
								(((ONEVENTELEMENT *)((*ppElement)->pElm))->iType<=WTAEvent_ms_ns))

								/* GSM Specific */
								#ifdef CONFIG_WTAI_GSM

								/* Continue if-statement */
								|| ((((ONEVENTELEMENT *)((*ppElement)->pElm))->iType>=WTAEvent_gsm_ru)&&
								(((ONEVENTELEMENT *)((*ppElement)->pElm))->iType<=WTAEvent_gsm_ca))

								#endif
								
								/* PDC Specific */
								#ifdef CONFIG_WTAI_PDC

									/* No events defined */

								#endif
								
								/* IS-136 Specific */
								#ifdef CONFIG_WTAI_IS_136

								/* Continue if-statement */
								|| ((((ONEVENTELEMENT *)((*ppElement)->pElm))->iType>=WTAEvent_is136_ia)&&
								(((ONEVENTELEMENT *)((*ppElement)->pElm))->iType<=WTAEvent_is136_if))

								#endif

								/* End if statement */
								)
							{
								/* The event is legal. Add event to the parent (CARD or TEMPLATE). */
								WML_AddToEventTable(pParent, (((ONEVENTELEMENT *)((*ppElement)->pElm))->iType),
										(((ONEVENTELEMENT *)((*ppElement)->pElm))->pContent));
								break;
							}
						}
						#endif
					}
				}

				/* ONEVENT type not legal - remove element */
				WML_DeleteElement(*ppElement);
				*ppElement=NULL;
				break;
			case Type_TEMPLATE:
				/* Move the event table from the TEMPLATE element 
				   to the event table in the User Agent. */
				((UA *)(pDecStr->pAppSpec))->pTemplateEvents =
							((TEMPLATEELEMENT *)((*ppElement)->pElm))->pEventTab;
				break;
			}
		}
	}
}
