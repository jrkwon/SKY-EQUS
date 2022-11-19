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
	FILE: wmlparse.c
  
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

		991206	JPR		Changes to support new generic WBXML-decoder
		991208	JPR		Minor corrections
		991223	JPR		WML_CheckPublicID updated
		000104	JPR		Support for xml:space added
		000131	JPR		Correction in WML_LiteralAttributeToToken
		000201	JPR		Correction in WML_StoreAttributeValue and
						WML_LiteralAttributeToToken 
		000204	JPR		Correction in function WML_ModifyParse
		000225	JPR		Call of function WML_SetXmlSpace updated
		000808	JPR		Updated to support WTA-WML (WTA 1.2.1)
		000811	JPR		Addition of cache-control and new public identifiers
		010119	NKE		Updated WML_ModifyParse for the new decoder in wbxmldec.c.
		010126	NKE		Changed definition of WML_ModifyParse. Added new
						functionality to WML_ModifyParse to be able to handle
						complete wml documents and not as before only cards.
						This functionality has ben moved from 
						wmlif.c/WML_ParseNextElement().
        010305  JPR     WTA-WML supported in function WML_CheckPublicID

==========================================================================*/

#include "wmlparse.h"
#include "wmltoken.h"
#include "wbxmldec.h"
#include "wbxmlelm.h"
#include "wmlevent.h"
#include "wmlelm.h"

/* WTA specific */
#ifdef CONFIG_WTA
#include "tapimmi.h"
#endif

/*
 	Examines if a p element is insignificant, i.e.,
	NULL, empty, or only white space. Variables 
	(extensions) are considered significant.
*/
BOOL WML_IsPEmptyInternal (pDECSTR pDecStr, pELEMENTTYPE pPElement)
{
	pELEMENTTYPE pContent=NULL;
	WCHAR* pwchText=NULL;
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
		if (pContent->iType==Type_Inline_Text)
		{
			/* Inline text found, check if only white space */
			pwchText=((pI_TEXTELEMENT)(pContent))->pwchTextData;

			if (!WML_CheckIfOnlyWhiteSpace (pwchText))
			{
				/* Not only white space found */
				return FALSE;
			}
		}
		else if (pContent->iType==Type_Table_Text)
		{
			/* Table text found, check if only white space */
			pwchText=WBXML_GetStrTabVal(pDecStr,
							((pT_TEXTELEMENT)(pContent))->iTableRef);

			if (!WML_CheckIfOnlyWhiteSpace (pwchText))
			{
				/* Not only white space found */
				return FALSE;
			}
		}
		else if (pContent->iType==Type_Entity)
		{
			/* Check if white space entity */
			iEntity=(WCHAR) (((pENTITY)(pContent))->iEntity);

			if (!((iEntity==(WCHAR)'\x20')||(iEntity==(WCHAR)'\x9')||
				(iEntity==(WCHAR)'\xD')||(iEntity==(WCHAR)'\xA')))
			{
				/* Not white space */
				return FALSE;
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



/*========================================================================
	WML_ModifyParse
==========================================================================

NOTE: use WBXML_DeleteCurrentElement instead of XML_DeleteElement!

*/
void WML_ModifyParse (pDECSTR decodeStruct)
{
	pELEMENTTYPE current = decodeStruct->currentElement;
	pELEMENTTYPE parent  = decodeStruct->parentElement;

	if (current == NULL) 
		return;

	switch (current->iType) {
	
	case Type_P:
		/* Check if p is empty (Static check) */
		if (WML_IsPEmptyInternal(decodeStruct,current))
			/* Remove p */
			WBXML_DeleteCurrentElement(decodeStruct);
		else {
			/* Keep p and update current Wrap-mode */
			if (((pPELEMENT) current)->iMode == P_Unknown)
				((pPELEMENT) current)->iMode = ((UA*) decodeStruct->pAppSpec)->iCurrentWrapMode;

			((UA*) decodeStruct->pAppSpec)->iCurrentWrapMode = ((pPELEMENT) current)->iMode;
		}
		break;

	case Type_ONEVENT:
		if (parent!=NULL) {
			/* Only valid events are ONENTERFORWARD, ONENTERBACKWARD,
				 ONTIMER or ONCLICK. All other should be ignored. For
				 CARD and TEMPLATE the first three are valid and for
				 OPTION, the ONCLICK event is valid. */

			/* If WTA is enabled, other events are valid in the CARD
				 and TEMPLATE as well. */

			if (parent->iType == Type_OPTION) {
				/* Check if the type is legal for OPTION. */
				if ( ((pONEVENTELEMENT) current)->iEventType == DEF_TYPE_ONCLICK) {
					/* Remove conflicting task - implies non-fatal error */
					XML_DeleteElement(&(((pOPTIONELEMENT) parent)->pOnpick) ,decodeStruct);

					/* Move the Action to "attribute level" */
					((pOPTIONELEMENT) parent)->pOnpick = ((pONEVENTELEMENT) current)->pContent;

					/* Remove the pointer to the task to allow the
						 use of DeleteElement without deleting the task. */
					((pONEVENTELEMENT) current)->pContent = NULL;
					break;
				}
			}

			else if (((parent->iType == Type_CARD) || (parent->iType == Type_TEMPLATE))) 
			{
				/* Check if the type is legal for CARD or TEMPLATE. */
				if ((((pONEVENTELEMENT) current)->iEventType == DEF_TYPE_ONTIMER) ||
				   (((pONEVENTELEMENT) current)->iEventType == DEF_TYPE_ONENTERFORWARD) ||
				   (((pONEVENTELEMENT) current)->iEventType == DEF_TYPE_ONENTERBACKWARD)) {
					/* The event is legal. Add event to the parent (CARD or TEMPLATE). */
					WML_AddToEventTable(parent, (((pONEVENTELEMENT) current)->iEventType),
							(((pONEVENTELEMENT)(current))->pContent));
					break;
				}

				/* Check if WTA event (only if WTA is enabled AND the current user
					 agent is a WTA user agent) */
				#ifdef CONFIG_WTA
				if (((UA*) decodeStruct->pAppSpec)->iUserAgentMode == User_Agent_WTA) {
					/* Allow WTA events as well Check if WTA event.
						 The following if-statement checks if the event is within the
						 legal boundaries for the current configuration.
					*/
					if (((((pONEVENTELEMENT) current)->iEventType >= WTAEvent_cc_ic) &&
					     (((pONEVENTELEMENT) current)->iEventType <= WTAEvent_ms_ns))

						/* GSM Specific */
						#ifdef CONFIG_WTAI_GSM
						|| ((((pONEVENTELEMENT) current)->iEventType >= WTAEvent_gsm_ru) &&
						    (((pONEVENTELEMENT) current)->iEventType <= WTAEvent_gsm_ca))
						#endif
						
						/* PDC Specific */
						#ifdef CONFIG_WTAI_PDC
							/* No events defined */
						#endif
						
						/* IS-136 Specific */
						#ifdef CONFIG_WTAI_IS_136
						|| ((((pONEVENTELEMENT) current)->iEventType >= WTAEvent_is136_ia) &&
						    (((pONEVENTELEMENT) current)->iEventType <= WTAEvent_is136_if))
						#endif

						) {
						/* The event is legal. Add event to the parent (CARD or TEMPLATE). */
						WML_AddToEventTable(parent, (((pONEVENTELEMENT) current)->iEventType),
								(((pONEVENTELEMENT) current)->pContent));
						break;
					}
				}
				#endif
			}
		}

		/* ONEVENT type not legal - remove element */
		WBXML_DeleteCurrentElement(decodeStruct);
		break;

	case Type_TEMPLATE:
		/* Move the event table from the TEMPLATE element 
			 to the event table in the User Agent. */
		((UA*) decodeStruct->pAppSpec)->pTemplateEvents = ((pTEMPLATEELEMENT) current)->pEventTab;
		break;

	case Type_CARD:
		((UA*) decodeStruct->pAppSpec)->iCurrentWrapMode = P_Wrap;
		break;

	case Type_UNKNOWN:
		parent = ((pUNKNOWNELEMENT) current)->pContent;		
		((pUNKNOWNELEMENT) current)->pContent = NULL;
		WBXML_DeleteCurrentElement(decodeStruct);
		WBXML_AddElement(decodeStruct, parent);
		break;

	case Type_WML:
		{
		pELEMENTTYPE saveParent = parent;
		pELEMENTTYPE saveCurrent = current;

		decodeStruct->parentElement = current;
		current = ((pWMLELEMENT) current)->pContent;
		while (current != NULL) {
			if (current->iType != Type_CARD && 
				current->iType != Type_HEAD &&
				current->iType != Type_TEMPLATE) {
				
				decodeStruct->currentElement = current;
				current = current->pNextElement;
				WBXML_DeleteCurrentElement(decodeStruct);
				decodeStruct->iDecodeResult |= WBXML_Warning_IllegalElement;
			} else
				current = current->pNextElement;
		}
		decodeStruct->currentElement = saveCurrent;
		decodeStruct->parentElement  = saveParent;
		}
		break;

	}
}



/*========================================================================
	WML_StoreAttributeValue
==========================================================================*/
BOOL WML_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							  UINT16 iAttribute, pELEMENTTYPE* ppAttrVal)
{	
	BOOL fWTA=FALSE;
	BOOL fResult=TRUE;

	#ifdef CONFIG_WTA
	/* Set the fWTA to TRUE if the user agent is WTA enabled,
	   FALSE otherwise. This is used when the WML_SetType 
	   function is called. */

	if (((UA*)(pDecStr->pAppSpec))->iUserAgentMode==User_Agent_WTA)
	{
		fWTA=TRUE;
	}
	#endif

	if (pElement!=NULL)
	{
		/* Store attribute value */
		switch (iAttribute)
		{
		case ATTRST_accept_charset:
			fResult=WML_SetAccChar(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_accesskey:
			fResult=WML_SetAccesskey(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_align:
			fResult=WML_SetAlign(pDecStr,pElement,ppAttrVal,DEF_TYPE_STRING);
			break;
		case ATTRST_align_bottom:			
			fResult=WML_SetAlign(pDecStr,pElement,ppAttrVal,ALIGN_BOTTOM);
			break;
		case ATTRST_align_center:
			fResult=WML_SetAlign(pDecStr,pElement,ppAttrVal,ALIGN_CENTER);
			break;
		case ATTRST_align_left:
			fResult=WML_SetAlign(pDecStr,pElement,ppAttrVal,ALIGN_LEFT);
			break;
		case ATTRST_align_middle:
			fResult=WML_SetAlign(pDecStr,pElement,ppAttrVal,ALIGN_MIDDLE);
			break;
		case ATTRST_align_right:
			fResult=WML_SetAlign(pDecStr,pElement,ppAttrVal,ALIGN_RIGHT);
			break;
		case ATTRST_align_top:
			fResult=WML_SetAlign(pDecStr,pElement,ppAttrVal,ALIGN_TOP);
			break;
		case ATTRST_alt:
			fResult=WML_SetAlt(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_cache_control:
			fResult=WML_SetCacheControl(pDecStr,pElement,ppAttrVal,Cache_nocache);
			break;
		case ATTRST_class:
			/* Not used - delete */
			XML_DeleteElementList(ppAttrVal,pDecStr);
			break; 
		case ATTRST_columns:
			fResult=WML_SetColumns(pDecStr,pElement,ppAttrVal);
			break; 
		case ATTRST_content:
			fResult=WML_SetContent(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_content_wmlc_charset:
			XML_AddHeadString(ppAttrVal,"application/vnd.wap.wmlc;charset=",pDecStr);
			fResult=WML_SetContent(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_domain:
			fResult=WML_SetDomain(pDecStr,pElement,ppAttrVal);			
			break;
		case ATTRST_emptyok_false:
			fResult=WML_SetEmptyOK(pDecStr,pElement,ppAttrVal,FALSE);
			break;
		case ATTRST_emptyok_true:
			fResult=WML_SetEmptyOK(pDecStr,pElement,ppAttrVal,TRUE);
			break;
		case ATTRST_enctype:
			fResult=WML_SetEnctype(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_application:
			XML_AddHeadString(ppAttrVal,"application/x-www-form-urlencoded",pDecStr);
			fResult=WML_SetEnctype(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_multipart:
			XML_AddHeadString(ppAttrVal,"multipart/form-data",pDecStr);
			fResult=WML_SetEnctype(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_format:
			fResult=WML_SetFormat(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_forua_false:
			fResult=WML_SetForua(pDecStr,pElement,ppAttrVal,FALSE);
			break;
		case ATTRST_forua_true:
			fResult=WML_SetForua(pDecStr,pElement,ppAttrVal,TRUE);
			break;
		case ATTRST_height:
			fResult=WML_SetHeight(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_href:
			fResult=WML_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_href_http:
			XML_AddHeadString(ppAttrVal,"http://",pDecStr);
			fResult=WML_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_href_https:
			XML_AddHeadString(ppAttrVal,"https://",pDecStr);
			fResult=WML_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_hspace:
			fResult=WML_SetHSpace(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_http_equiv:
			fResult=WML_SetHttpEq(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_http_equiv_Content_Type:
			XML_AddHeadString(ppAttrVal,"Content-Type",pDecStr);
			fResult=WML_SetHttpEq(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_http_equiv_Expires:
			XML_AddHeadString(ppAttrVal,"Expires",pDecStr);
			fResult=WML_SetHttpEq(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_id:
			fResult=WML_SetId(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_ivalue:
			fResult=WML_SetIValue(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_iname:
			fResult=WML_SetIName(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_label:
			fResult=WML_SetLabel(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_localsrc:
			fResult=WML_SetLocalSrc(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_maxlength:
			fResult=WML_SetMaxLength(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_method_get:
			fResult=WML_SetMethod(pDecStr,pElement,ppAttrVal,M_Get);
			break;
		case ATTRST_method_post:
			fResult=WML_SetMethod(pDecStr,pElement,ppAttrVal,M_Post);
			break;
		case ATTRST_mode_nowrap:
			fResult=WML_SetMode(pDecStr,pElement,ppAttrVal,P_NoWrap);
			break;
		case ATTRST_mode_wrap:
			fResult=WML_SetMode(pDecStr,pElement,ppAttrVal,P_Wrap);
			break;
		case ATTRST_multiple_false:
			fResult=WML_SetMultiple(pDecStr,pElement,ppAttrVal,FALSE);
			break;
		case ATTRST_multiple_true:
			fResult=WML_SetMultiple(pDecStr,pElement,ppAttrVal,TRUE);
			break;
		case ATTRST_name: 
			fResult=WML_SetName(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_newcontext_false:
			fResult=WML_SetNewContext(pDecStr,pElement,ppAttrVal,FALSE);
			break;
		case ATTRST_newcontext_true:
			fResult=WML_SetNewContext(pDecStr,pElement,ppAttrVal,TRUE);
			break;
		case ATTRST_onenterbackward:
			fResult=WML_SetOnEntBack(pDecStr,pElement,ppAttrVal);

			if (fResult)
			{
				/* Add the event ONENTERBACKWARD to the eventtable */
				WML_AddToEventTable(pElement,DEF_TYPE_ONENTERBACKWARD,*ppAttrVal);
				*ppAttrVal=NULL;
			}
			break;
		case ATTRST_onenterforward:
			fResult=WML_SetOnEntForw(pDecStr,pElement,ppAttrVal);

			if (fResult)
			{
				/* Add the event ONENTERFORWARD to the eventtable */
				WML_AddToEventTable(pElement,DEF_TYPE_ONENTERFORWARD,*ppAttrVal);
				*ppAttrVal=NULL;
			}
			break;
		case ATTRST_onpick:
			fResult=WML_SetOnPick(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_ontimer:
			fResult=WML_SetOnTimer(pDecStr,pElement,ppAttrVal);

			if (fResult)
			{
				/* Add the event ONTIMER to the eventtable */
				WML_AddToEventTable(pElement,DEF_TYPE_ONTIMER,*ppAttrVal);
				*ppAttrVal=NULL;
			}
			break;
		case ATTRST_optional_false:
			fResult=WML_SetOptional(pDecStr,pElement,ppAttrVal,FALSE);
			break;
		case ATTRST_optional_true:
			fResult=WML_SetOptional(pDecStr,pElement,ppAttrVal,TRUE);
			break;
		case ATTRST_path:
			fResult=WML_SetPath(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_scheme:
			fResult=WML_SetScheme(pDecStr,pElement,ppAttrVal);			
			break;
		case ATTRST_sendreferer_false:
			fResult=WML_SetSendReferer(pDecStr,pElement,ppAttrVal,FALSE);
			break;
		case ATTRST_sendreferer_true:
			fResult=WML_SetSendReferer(pDecStr,pElement,ppAttrVal,TRUE);
			break;
		case ATTRST_size:
			fResult=WML_SetSize(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_src:
			fResult=WML_SetSrc(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_src_http:
			XML_AddHeadString(ppAttrVal,"http://",pDecStr);
			fResult=WML_SetSrc(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_src_https:
			XML_AddHeadString(ppAttrVal,"https://",pDecStr);
			fResult=WML_SetSrc(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_ordered_true:
			fResult=WML_SetOrdered(pDecStr,pElement,ppAttrVal,TRUE);
			break;
		case ATTRST_ordered_false:
			fResult=WML_SetOrdered(pDecStr,pElement,ppAttrVal,FALSE);
			break;
		case ATTRST_tabindex:
			fResult=WML_SetTabIndex(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_title:
			fResult=WML_SetTitle(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_type:
			/* ONEVENT, DO, INPUT */
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_accept:
			XML_AddHeadString(ppAttrVal,"accept",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_delete:
			XML_AddHeadString(ppAttrVal,"delete",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_help:
			XML_AddHeadString(ppAttrVal,"help",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_password:
			XML_AddHeadString(ppAttrVal,"password",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_onpick:
			XML_AddHeadString(ppAttrVal,"onpick",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_onenterbackward:
			XML_AddHeadString(ppAttrVal,"onenterbackward",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_onenterforward:
			XML_AddHeadString(ppAttrVal,"onenterforward",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_ontimer:
			XML_AddHeadString(ppAttrVal,"ontimer",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_options:
			XML_AddHeadString(ppAttrVal,"options",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_prev:
			XML_AddHeadString(ppAttrVal,"prev",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_reset:
			XML_AddHeadString(ppAttrVal,"reset",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_text:
			XML_AddHeadString(ppAttrVal,"text",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_type_vnd:
			XML_AddHeadString(ppAttrVal,"vnd.",pDecStr);
			fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
			break;
		case ATTRST_value:
			fResult=WML_SetValue(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_vspace:
			fResult=WML_SetVSpace(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_width:
			fResult=WML_SetWidth(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_xml_lang:
			fResult=WML_SetXmllang(pDecStr,pElement,ppAttrVal);
			break;
		case ATTRST_xml_space_preserve:
			fResult=WML_SetXmlSpace(pDecStr,pElement,ppAttrVal,XML_SPACE_PRESERVE);
			break;
		case ATTRST_xml_space_default:
			fResult=WML_SetXmlSpace(pDecStr,pElement,ppAttrVal,XML_SPACE_DEFAULT);
			break;
		default:

			/* Codepage 1 for WTA-WML */
			#ifdef CONFIG_WTA

			if (fWTA)
			{
				switch (iAttribute)
				{
				case WTAATTRST_href_wtai:
					XML_AddHeadString(ppAttrVal,"wtai://",pDecStr);
					fResult=WML_SetHref(pDecStr,pElement,ppAttrVal);
					break;
				case WTAATTRST_href_wtai_wp_mc:
					XML_AddHeadString(ppAttrVal,"wtai://wp/mc;",pDecStr);
					fResult=WML_SetHref(pDecStr,pElement,ppAttrVal);
					break;
				case WTAATTRST_href_wtai_wp_sd:
					XML_AddHeadString(ppAttrVal,"wtai://wp/sd;",pDecStr);
					fResult=WML_SetHref(pDecStr,pElement,ppAttrVal);
					break;
				case WTAATTRST_href_wtai_wp_ap:
					XML_AddHeadString(ppAttrVal,"wtai://wp/ap;",pDecStr);
					fResult=WML_SetHref(pDecStr,pElement,ppAttrVal);
					break;
				case WTAATTRST_href_wtai_ms_ec:	
					XML_AddHeadString(ppAttrVal,"wtai://ms/ec",pDecStr);
					fResult=WML_SetHref(pDecStr,pElement,ppAttrVal);
					break;
				case WTAATTRST_type_wtaev:
					XML_AddHeadString(ppAttrVal,"wtaev-",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_cc:			
					XML_AddHeadString(ppAttrVal,"wtaev-cc/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_cc_ic:	
					XML_AddHeadString(ppAttrVal,"wtaev-cc/ic",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_cc_cl:
					XML_AddHeadString(ppAttrVal,"wtaev-cc/cl",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_cc_co:
					XML_AddHeadString(ppAttrVal,"wtaev-cc/co",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_cc_oc:
					XML_AddHeadString(ppAttrVal,"wtaev-cc/oc",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_cc_cc:
					XML_AddHeadString(ppAttrVal,"wtaev-cc/cc",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_cc_dtmf:
					XML_AddHeadString(ppAttrVal,"wtaev-cc/dtmf",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_nt:
					XML_AddHeadString(ppAttrVal,"wtaev-nt/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_nt_it:	
					XML_AddHeadString(ppAttrVal,"wtaev-nt/it",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_nt_st:
					XML_AddHeadString(ppAttrVal,"wtaev-nt/st",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_pb:
					XML_AddHeadString(ppAttrVal,"wtaev-pb/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_lg:		
					XML_AddHeadString(ppAttrVal,"wtaev-lg/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_ms:		
					XML_AddHeadString(ppAttrVal,"wtaev-ms/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_ms_ns:	
					XML_AddHeadString(ppAttrVal,"wtaev-ms/ns",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_gsm:	
					XML_AddHeadString(ppAttrVal,"wtaev-gsm/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_gsm_ru:	
					XML_AddHeadString(ppAttrVal,"wtaev-gsm/ru",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_gsm_ch:
					XML_AddHeadString(ppAttrVal,"wtaev-gsm/ch",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_gsm_ca:
					XML_AddHeadString(ppAttrVal,"wtaev-gsm/ca",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_pdc:
					XML_AddHeadString(ppAttrVal,"wtaev-pdc/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_is136:
					XML_AddHeadString(ppAttrVal,"wtaev-is136/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_is136_ia:
					XML_AddHeadString(ppAttrVal,"wtaev-is136/ia",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_is136_if:
					XML_AddHeadString(ppAttrVal,"wtaev-is136/if",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				case WTAATTRST_type_wtaev_cdma:
					XML_AddHeadString(ppAttrVal,"wtaev-cdma/",pDecStr);
					fResult=WML_SetType(pDecStr,pElement,ppAttrVal,fWTA);
					break;
				default:
					/* Not valid attribute start token. */
					fResult=FALSE;
					break;
				}
			}
			else

			/* CONFIG_WTA */
			#endif

			{			
				/* Not valid attribute start token. */
				fResult=FALSE;
				break;
			}
		}
	}

	/* Check if all elements in the attribute value were used in the 
	   store function. If not all elements were user, return FALSE. 
	   This will result in a warning. Also, delete the unused element
	   to avoid memory leak. */
	if (*ppAttrVal!=NULL)
	{
		fResult=FALSE;
		XML_DeleteElementList(ppAttrVal,pDecStr);
	}	

	return fResult;
}


/*========================================================================
	WML_GetAttributeValue
==========================================================================*/
WCHAR* WML_GetAttributeValue (UINT16 iToken)
{
	CHAR* pchString=NULL;

	switch (iToken)
	{
	case ATTRVAL_com:
		pchString=".com/"; 
		break;
	case ATTRVAL_edu:
		pchString=".edu/"; 
		break;
	case ATTRVAL_net:
		pchString=".net/"; 
		break;
	case ATTRVAL_org:
		pchString=".org/"; 
		break;
	case ATTRVAL_accept:
		pchString="accept"; 
		break;
	case ATTRVAL_bottom:
		pchString="bottom"; 
		break;
	case ATTRVAL_clear:
		pchString="clear"; 
		break;
	case ATTRVAL_delete:
		pchString="delete"; 
		break;
	case ATTRVAL_help:
		pchString="help"; 
		break;
	case ATTRVAL_http:
		pchString="http://\0"; 
		break;
	case ATTRVAL_http_www:
		pchString="http://www."; 
		break;
	case ATTRVAL_https:
		pchString="https://\0"; 
		break;
	case ATTRVAL_https_www:
		pchString="https://www."; 
		break;
	case ATTRVAL_middle:
		pchString="middle"; 
		break;
	case ATTRVAL_nowrap:
		pchString="nowrap"; 
		break;
	case ATTRVAL_onpick:
		pchString="onpick"; 
		break;
	case ATTRVAL_onenterbackward:
		pchString="onenterbackward"; 
		break;
	case ATTRVAL_onenterforward:
		pchString="onenterforward"; 
		break;
	case ATTRVAL_ontimer:
		pchString="ontimer"; 
		break;
	case ATTRVAL_options:
		pchString="options"; 
		break;
	case ATTRVAL_password:
		pchString="password"; 
		break;
	case ATTRVAL_reset:
		pchString="reset"; 
		break;
	case ATTRVAL_text:
		pchString="text"; 
		break;
	case ATTRVAL_top:
		pchString="top"; 
		break;
	case ATTRVAL_unknown:
		pchString="unknown"; 
		break;
	case ATTRVAL_wrap:
		pchString="wrap"; 
		break;
	case ATTRVAL_www:
		pchString="www."; 
		break;
	}

	if (pchString!=NULL)
	{
		/* Copy string and return */
		return (w_cstr2wstr(pchString));
	}

	return NULL;
}


/*========================================================================
	WML_LiteralAttributeToToken
==========================================================================*/
UINT16 WML_LiteralAttributeToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchAttrTokens,STAT_NBROFATTR);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piAttrBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	WML_LiteralTagToToken
==========================================================================*/
UINT16 WML_LiteralTagToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchTagTokens,STAT_NBROFTAGS);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piTagBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	WML_CheckPublicID
==========================================================================*/
BOOL WML_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	/* Use string */
	if (pwchPId!=NULL)
	{
		UINT32 iLength=0;

		/* WML 1.0 */
		const WCHAR pwchPublic_ID_1_0[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','W','M','L',' ','1',
								   '.','0','/','/','E','N',0};

		/* WML 1.1 */
		const WCHAR pwchPublic_ID_1_1[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','W','M','L',' ','1',
								   '.','1','/','/','E','N',0};

		/* WML 1.2 */
		const WCHAR pwchPublic_ID_1_2[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','W','M','L',' ','1',
								   '.','2','/','/','E','N',0};

		/* WML 1.3 */
		const WCHAR pwchPublic_ID_1_3[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','W','M','L',' ','1',
								   '.','3','/','/','E','N',0};

    #ifdef CONFIG_WTA
		/* wta-wml */
		const WCHAR pwchPublic_ID_WTA_WML_1_2[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','W','T','A','-','W',
								   'M','L',' ','1','.','2','/','/','E','N',0};
    #endif /* CONFIG_WTA */


		/* Get length of pwchPId string */
		iLength=(UINT32) (w_strlen(pwchPId));

		if (CompareWideStringNoCase (pwchPId,(WCHAR*)pwchPublic_ID_1_3,iLength))
		{
			/* WML 1.3 - OK */
			*piPId=WML_VERSION_1_3;
			return TRUE;
		}

    /* wta-wml - Only used if CONFIG_WTA enabled */
    #ifdef CONFIG_WTA
		else if (CompareWideStringNoCase (pwchPId,(WCHAR*)pwchPublic_ID_WTA_WML_1_2,iLength))
    {
      /* Use wml version 1.3 */
			*piPId=WML_VERSION_1_3;
			return TRUE;
    }
    #endif /* CONFIG_WTA */

		else if (CompareWideStringNoCase (pwchPId,(WCHAR*)pwchPublic_ID_1_2,iLength))
		{
			/* WML 1.2 - OK */
			*piPId=WML_VERSION_1_2;
			return TRUE;
		}
		else if (CompareWideStringNoCase (pwchPId,(WCHAR*)pwchPublic_ID_1_1,iLength))
		{
			/* WML 1.1 - OK */
			*piPId=WML_VERSION_1_1;
			return TRUE;
		}
		else if (CompareWideStringNoCase (pwchPId,(WCHAR*)pwchPublic_ID_1_0,iLength))
		{
			/* WML 1.0 - NOT OK */
			return FALSE;
		}

		/* Check content-type and level (0x14 is applictaion/vnd.wap.wmlc) */
		if (pDecStr->bContentType=='\x14')
		{
			/* Check level */
			if (pDecStr->iContentLevel=='\x12')
			{
				/* WML 1.2 - OK */
				*piPId=WML_VERSION_1_2;
				return TRUE;
			}
			else if (pDecStr->iContentLevel=='\x11')
			{
				/* WML 1.1 - OK */
				*piPId=WML_VERSION_1_1;
				return TRUE;
			}
			else if (pDecStr->iContentLevel=='\x10')
			{
				/* WML 1.0 - NOT OK */
				return FALSE;
			}
			else
			{
				/* WML 1.3 (or other) - OK */
				*piPId=WML_VERSION_1_3;
				return TRUE;
			}
		}	
	}

	/* Use integer */
	else
	{
		/* Public ID (wml 1.0) is 0x02 - SHOULD result in error. */
		/* Public ID (wml 1.1) is 0x04 */
		/* Public ID (wml 1.2) is 0x09 */
		/* Public ID (wml 1.3) is 0x0A */

		switch (*piPId)
		{

		/* Unknown */
		case '\x01':
			/* Check content-type and level (0x14 is applictation/vnd.wap.wmlc)*/
			if (pDecStr->bContentType=='\x14')
			{
				/* Check level */
				if (pDecStr->iContentLevel=='\x12')
				{
					/* WML 1.2 - OK */
					*piPId=WML_VERSION_1_2;
					return TRUE;
				}
				else if (pDecStr->iContentLevel=='\x11')
				{
					/* WML 1.1 - OK */
					*piPId=WML_VERSION_1_1;
					return TRUE;
				}
				else if (pDecStr->iContentLevel=='\x10')
				{
					/* WML 1.0 - NOT OK */
					return FALSE;
				}
				else
				{
					/* WML 1.3 (or other) - OK */
					*piPId=WML_VERSION_1_3;
					return TRUE;
				}
			}
			return FALSE;

		/* WML 1.0 - not supported */
		case '\x02':
			return FALSE;

		/* WML 1.1 */
		case '\x04':
			*piPId=WML_VERSION_1_1;
			return TRUE;

		/* WML 1.2 */
		case '\x09':
			*piPId=WML_VERSION_1_2;
			return TRUE;

		/* WML 1.3 */
		case '\x0A':
			*piPId=WML_VERSION_1_3;
			return TRUE;

    /* wta-wml - Only used if CONFIG_WTA enabled */
    #ifdef CONFIG_WTA
		case '\x0C':
      /* Use wml version 1.3 */
			*piPId=WML_VERSION_1_3;
			return TRUE;
    #endif /* CONFIG_WTA */
		}
	}

	/* Other - not supported */
	return FALSE;
}




/*========================================================================

	Other functions

=========================================================================*/

/*========================================================================
	WML_CheckIfOnlyWhiteSpace
==========================================================================*/
BOOL WML_CheckIfOnlyWhiteSpace (WCHAR* pwchText)
{
	WCHAR* pwchTemp=pwchText;

	if (pwchTemp!=NULL)
	{
		while (*pwchTemp!=0)
		{
			/* White spaces in wml */
			if (!((*pwchTemp==(WCHAR)'\x20')||(*pwchTemp==(WCHAR)'\x9')||
				(*pwchTemp==(WCHAR)'\xD')||(*pwchTemp==(WCHAR)'\xA')))
			{
				/* Not white space */
				return FALSE;
			}
			
			/* Next character */
			pwchTemp++;
		}
	}

	/* NULL -> return TRUE */
	return TRUE;
}

