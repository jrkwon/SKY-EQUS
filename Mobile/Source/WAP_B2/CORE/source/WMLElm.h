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
	FILE: WMLElm.h
  
	Interface file for WML-specific element handler.

	Rev history:
		980706	JPR		Created
		981223	JPR		Event concept changed	
		990303	JPR		Corrections		
		990316	JPR		Major changes. Handles the WML-specific 
						elements. Variable handling moved to 
						WMLIF.h
		990317	JPR		Corrections
		990323	JPR		WAP 1.1
		990412	JPR		IsAlpha and IsNumber functions removed
		990512	JPR		ViewID added to WML_CreateElement function.
		990827	JPR		WML_AddContentToEnd added
		991026	JPR		Additions for WTA included.
   ================================================================== */

#ifndef _ELMHANDLERH
#define _ELMHANDLERH

#include "WMLDef.h"
#include "WBXMLElm.h"
#include "WMLEvent.h"

/*-------------------------------------------
  Returns a WCHAR between '0' and '9' as an UINT8.
  Input: WCHAR
  Output: UINT8
  -------------------------------------------*/
UINT8 WML_CharToInt (WCHAR chNumber);

/*-------------------------------------------
  Converts a WCHAR string with digits '0' to
  '9' to an integer. If the number is terminated
  with '%', the flag fIsPercent is set. If the
  string is not a valid number, 0 is returned.
  NOTE! The string is removed.
  Input: WCHAR*, pointer to BOOL
  Output: UINT8
  -------------------------------------------*/
UINT8 WML_ConvertNumber (WCHAR *pchNumber, BOOL *pfIsPercent);

/*-------------------------------------------
  Converts a WCHAR string with digits '0' to
  '9' to an integer. If the string is not a 
  valid number, 0 is returned.
  NOTE! The string is removed.
  Input: WCHAR*
  Output: UINT32
  -------------------------------------------*/
UINT32 WML_ConvertNumber32 (WCHAR *pchNumber);

/* ------------------------------------------
   Converts the input/event type to an integer. 
   If the type exists the function returns the 
   appropriate number. The inputted string is 
   Deallocated. TYPE_UNKNOWN is returned if 
   the string is not an intrinsic event or 
   TEXT / PASSWORD.

   Input: WCHAR**
   Output: INT8 according to TYPE code table.
   ------------------------------------------*/
INT8 WML_ConvertType (WCHAR **ppchType);

#ifdef CONFIG_WTA
/* ------------------------------------------
   Converts the WTA-event type to an integer. 
   If the type exists the function returns the 
   appropriate number. TYPE_UNKNOWN is returned 
   if the string is not an WTA event.

   Input: WCHAR*
   Output: INT8 according to WTA event table.
   ------------------------------------------*/
INT8 WTA_ConvertEventToInt (WCHAR *pwchType);
#endif

/* ------------------------------------------
   Converts the align to an integer. If the 
   type exists the function returns the 
   appropriate number. The inputted string is 
   Deallocated. TYPE_UNKNOWN is returned if 
   the string is not known.
   Input: WCHAR
   Output: INT8 according to TYPE code table.
   ------------------------------------------*/
INT8 WML_ConvertAlign (WCHAR **ppchAlign);

/* ------------- ELEMENT FUNCTIONS ------------- 
	   Functions for handling the WML elements.
   --------------------------------------------- */
/* ----------------------------------------------
   Creates an element of the specified type and 
   sets the default values.
   Input: Type and ViewID
   Output: Pointer to an Element
   ----------------------------------------------*/
pELEMENTTYPE WML_CreateElement (UINT8 iType, UINT8 iViewID);

/* ----------------------------------------------
   Deletes an element and frees the memory.
   Input: Pointer to an element.	
   Output: -
   ----------------------------------------------*/
void WML_DeleteElement (pELEMENTTYPE pElementObj);

/* Help function for WML_AddContent. */
void WML_AddContentToEnd (pELEMENTTYPE *ppContentPtr, 
						  pELEMENTTYPE pContent);

/* ----------------------------------------------
   Add the specified content to an element.
   Input: Element and content
   Output: -
   ----------------------------------------------*/
void WML_AddContent (pELEMENTTYPE pElementObj, pELEMENTTYPE pcontent);

/* ----------------------------------------------
   Returns the content for the specified element.
   Input: Element
   Output: Pointer to ELEMENT 
   ----------------------------------------------*/
pELEMENTTYPE WML_GetContent (pELEMENTTYPE pElementObj);

/* ----------------------------------------------
   Adds the DO element to the Template DO list on
   the card. Note! Only the pointer is stored - 
   DO NOT DEALLOCATE the DO element.
   Input: Pointer to Card and the DO element
		  (MUST be defined)
   Output: TRUE if success, FALSE otherwise
   ----------------------------------------------*/
BOOL WML_AddTemplateDO (pELEMENTTYPE pCard, pELEMENTTYPE pDO);

/* ----------------------------------------------
   The following functions sets the attribute
   values in the elements. If the attribute 
   field is not a valid field for the element,
   the value is deleted. Otherwise it is stored.
   Input: Element (NOT NULL) and attribute value.						
   Output: -
   ----------------------------------------------*/
void WML_SetAccChar (pELEMENTTYPE pElementObj, pTEXTELEMENT pAccChar);
void WML_SetAlign (pELEMENTTYPE pElementObj, pTEXTELEMENT pAlign, INT8 iAlign);
void WML_SetAlt (pELEMENTTYPE pElementObj, pTEXTELEMENT pAlt);
void WML_SetColumns (pELEMENTTYPE pElementObj, pTEXTELEMENT pColumns);
void WML_SetContent (pELEMENTTYPE pElementObj, pTEXTELEMENT pContent);
void WML_SetDomain (pELEMENTTYPE pElementObj, pTEXTELEMENT pDomain);
void WML_SetEmptyOK (pELEMENTTYPE pElementObj, BOOL fEmptyOK, pTEXTELEMENT pEmptyOK);
void WML_SetFormat (pELEMENTTYPE pElementObj, pTEXTELEMENT pFormat);
void WML_SetForua (pELEMENTTYPE pElementObj, BOOL fForua, pTEXTELEMENT pForua);
void WML_SetHeight (pELEMENTTYPE pElementObj, pTEXTELEMENT pHeight);
void WML_SetHSpace (pELEMENTTYPE pElementObj, pTEXTELEMENT pHSpace);
void WML_SetHref(pELEMENTTYPE pElementObj, pTEXTELEMENT pHref);
void WML_SetHttpEq(pELEMENTTYPE pElementObj, pTEXTELEMENT pHttpEq);
void WML_SetId (pELEMENTTYPE pElementObj, pTEXTELEMENT pId);
void WML_SetIValue (pELEMENTTYPE pElementObj, pTEXTELEMENT pIValue);
void WML_SetIName (pELEMENTTYPE pElementObj, pTEXTELEMENT pIName);
void WML_SetLabel (pELEMENTTYPE pElementObj, pTEXTELEMENT pLabel);
void WML_SetLocalSrc (pELEMENTTYPE pElementObj, pTEXTELEMENT pLocalSrc);
void WML_SetMaxLength (pELEMENTTYPE pElementObj, pTEXTELEMENT pLength);
void WML_SetMethod (pELEMENTTYPE pElementObj, enum MethodType iMethod, pTEXTELEMENT pMethod);
void WML_SetMode (pELEMENTTYPE pElementObj, enum WrapMode iMode, pTEXTELEMENT pMode);
void WML_SetMultiple (pELEMENTTYPE pElementObj, BOOL fMultiple, pTEXTELEMENT pMultiple);
void WML_SetName (pELEMENTTYPE pElementObj, pTEXTELEMENT pName);
void WML_SetNewContext (pELEMENTTYPE pElementObj, BOOL fContext, pTEXTELEMENT pNewContext);

/* Note the pointers! */
void WML_SetOnEntBack (pELEMENTTYPE pElementObj, pELEMENTTYPE *ppURL);
void WML_SetOnEntForw (pELEMENTTYPE pElementObj, pELEMENTTYPE *ppURL);
void WML_SetOnPick (pELEMENTTYPE pElementObj, pELEMENTTYPE *ppURL);
void WML_SetOnTimer (pELEMENTTYPE pElementObj, pELEMENTTYPE *ppURL);

void WML_SetOptional (pELEMENTTYPE pElementObj, BOOL fOptional, pTEXTELEMENT pOptional);
void WML_SetPath (pELEMENTTYPE pElementObj, pTEXTELEMENT pPath);
void WML_SetScheme (pELEMENTTYPE pElementObj, pTEXTELEMENT pScheme);
void WML_SetSendReferer (pELEMENTTYPE pElementObj, BOOL fSendRef, pTEXTELEMENT pSendRef);
void WML_SetSize (pELEMENTTYPE pElementObj, pTEXTELEMENT pSize);
void WML_SetSrc (pELEMENTTYPE pElementObj, pTEXTELEMENT pHref);
void WML_SetOrdered (pELEMENTTYPE pElementObj, BOOL fOrdered, pTEXTELEMENT pOrdered);
void WML_SetTabIndex (pELEMENTTYPE pElementObj, pTEXTELEMENT pTabIndex);
void WML_SetTitle (pELEMENTTYPE pElementObj, pTEXTELEMENT pTitle);
void WML_SetType (pELEMENTTYPE pElementObj, INT8 iType, WCHAR *pchType, BOOL fWTA);
void WML_SetValue (pELEMENTTYPE pElementObj, pTEXTELEMENT pValue);
void WML_SetVSpace (pELEMENTTYPE pElementObj, pTEXTELEMENT pVSpace);
void WML_SetWidth (pELEMENTTYPE pElementObj, pTEXTELEMENT pWidth);
void WML_SetXmllang (pELEMENTTYPE pElementObj, pTEXTELEMENT pXmllang);

#endif
