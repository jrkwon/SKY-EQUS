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
	FILE: hdrutil.c
  
	Utilities for Header Encoding/Decoding for WAP.
	Source file.

	Rev history:
		980923	JPR		Created
		990406	JPR		Corrections
		990408	JPR		Function create header Accept-language added
		990409	JPR		URL-functions removed
		990412	JPR		Macros used for character classification
		990416	JPR		Authentication functions updated (TR 64)
		990510	JPR		WSP_GetContentTypeByte updated
		990526	JPR		Corrections
		990611	JPR		Removal of unreferenced variables
		990623	JPR		Correction of Authentication functions
		990826	JPR		Changes in CreateHeaderField
						WSP_GetContentTypeString added
		990908	JPR		Correction in WSP_CreateHeaderField
		990921	JPR		Correction in WSP_CreateHeaderField (Accept
						Language and Accept Charset with long integer)
		991006	JPR		Correction in StepFieldLength
		991108	JPR		Correction in WSP_GetProxyAuthenticateRealm 
						and isWapCRLF.
		991112	JPR		ADDRESS functions added
		991118	JPR		Corrections in ADDRESS functions
		000110	IPN		Authentication functions moved to authoriz.c
		000111	JPR		Multipart functions updated
		000114	JPR		WSP Parameter handling changed
		000121	JPR		WSP_GetEtag added, WSP_CreateHeaderField
						function updated
		000125	JPR		WAE_AddTextFormDataPart added, corrections in
						multipart handling, correction in function
						WSP_CreateHeaderField (Authorization)
		000126	JPR		Correction in function WSP_CreateHeaderField 
						(Authorization and encoding of long integer)
						and Field_Profile_Diff header added. Function
						WSP_GetProfileWarning added.
						WSP_DeleteMultipartListWithContent added.
		000211	JPR		WSP_CreateHeaderField updated to support 
						Accept Application and Bearer Indication
						Function WSP_GetEtagWCHAR removed.
		000301	JPR		Function WAE_AddGenericFormDataPart implemented
		001106	JPR		Function WSP_GetPushHeaderAppId implemented
		001114  HEAD    Function WSP_BuildMultipart has a new parameter
						iTotalSize that is used in Large Data Transfer.
		001206	KEOL	Changed the fault handling in WSP_GetPushHeaderAppId 
				

   ================================================================== */

#include "hdrutil.h"
#include "headdef.h"
#include "confvars.h"
#include "cmmnrsrc.h"

pHEADERDEF CreateHEADERDEF (BYTE *pbInstream, UINT32 iLength)
{
	pHEADERDEF pDef;

	pDef=NEWSTRUCT(HEADERDEF);
	if (pDef!=NULL) 
	{
		pDef->pbData=pbInstream;
		pDef->pbEnd=pbInstream+iLength;
		pDef->pHeadList=NULL;
		return pDef;
	}
	return NULL;
}

void DeleteHEADERDEF (pHEADERDEF pDef)
{
	if (pDef!=NULL)
	{
		WSP_EmptyHeaderList(pDef);

		/* Remove copy of WSP header stream */
		DEALLOC(&pDef->pbData);

		DEALLOC(&pDef);
	}
}

/* Copies a byte stream with the specified length. 
   If the iLen is negative the stream is terminated with 
   0 and the length is determined in the function. */
BYTE *B_CopyByteString (BYTE *pbString, INT32 iLen)
{
	BYTE *pbTempString=NULL;

	if (iLen<0)
	{
		iLen=B_STRINGLENGTH(pbString)+1;
	}

	pbTempString=NEWARRAY(BYTE,iLen);
	if (pbTempString!=NULL) 
	{
		B_COPYSTRINGN(pbTempString,pbString,iLen);

		return pbTempString;
	}
	return NULL;
}

pHEADERELEMENT CreateHeaderElement (BYTE bCodePage, BYTE bFieldName, 
									BYTE *pbFieldName, BYTE *pbStart, BYTE *pbEnd)
{
	pHEADERELEMENT pTemp;

	pTemp=NEWSTRUCT(HEADERELEMENT);
	if (pTemp!=NULL)
	{
		pTemp->bCodePage=bCodePage;
		pTemp->bFieldName=bFieldName;
		pTemp->pbFieldName=pbFieldName;
		pTemp->pbFieldValStart=pbStart;
		pTemp->pbFieldValEnd=pbEnd;
		pTemp->pNext=NULL;
	}
	return pTemp;
}

void AddHeaderElement (pHEADERDEF pDef, pHEADERELEMENT pHeadElm)
{
	pHEADERELEMENT pTemp;

	if (pDef->pHeadList==NULL)
	{
		pDef->pHeadList=pHeadElm;
	}
	else 
	{
		pTemp=pDef->pHeadList;

		while (pTemp->pNext!=NULL)
		{
			pTemp=pTemp->pNext;
		}
		pTemp->pNext=pHeadElm;
	}
}

BOOL HdrNextByte (BYTE **ppbStart, BYTE *pbEnd) 
{
	BYTE *pbTemp=*ppbStart;

	pbTemp++;

	/* Check if within bounds */
	if (pbTemp<pbEnd) 
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	return FALSE;
}

BOOL StepFieldLength (BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;
	UINT32 iResult=0;
	UINT32 iPos=0;

	if (pbTemp<pbEnd)
	{
		/* Only this byte as data */
		if (*pbTemp>=128)
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;
		}

		/*	This byte is followed by the indicated 
			number of bytes (0-30) */
		else if (*pbTemp<=30)
		{		
			if ((*pbTemp)<(pbEnd-pbTemp))
			{
				pbTemp+=*pbTemp+1;
				*ppbStart=pbTemp;
				return TRUE;
			}
		} 

		/*	This byte is followed by an uintvar which
			indicates the number of following bytes. */
		else if (*pbTemp==31)
		{
			pbTemp++;

			if (ReadMBUInt32(&iResult,pbTemp,(UINT32)(pbEnd-pbTemp-1),&iPos))
			{
				/* Increase pTemp with the number of bytes indicated 
				   by iResult and the number of bytes for the MBUInt32. */
				pbTemp+=iPos+iResult;
				*ppbStart=pbTemp;
				return TRUE;
			}	
			return FALSE;
		} 

		/* Text string teminated with End-Of-Text */
		else if ((*pbTemp>=32)&&(*pbTemp<=127))
		{
			while (pbTemp<pbEnd) 
			/* Find NULL */
			{
				if (*pbTemp==0)
				{
					pbTemp++;
					*ppbStart=pbTemp;
					return TRUE;
				}
				pbTemp++;
			}
		} 

	}
	
	/* Error - return FALSE */
	return FALSE;
}

/* <any CHAR except CTLs or tspecials> */
BOOL isWapTokenCh(BYTE pbChar)
{
	return ( isWapCharCh(pbChar) && !isWapCtlCh(pbChar) && 
		!wae_istspecial(pbChar) );
}

/* CR LF */
BOOL isWapCRLF(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;
	
	/* Check CR */
	if (pbTemp<pbEnd)
	{
		if (isWapCRCh(*pbTemp))
		{
			if (HdrNextByte(&pbTemp,pbEnd))
			{
				/* Check LF */
				if (isWapLFCh(*pbTemp))
				{
					pbTemp++;
					*ppbStart=pbTemp;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/* [CRLF] 1*( SP | HT ) */
BOOL isWapLWS(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;	

	isWapCRLF(&pbTemp,pbEnd);

	if (pbTemp<pbEnd)
	{
		/* Check at least one SP or HT */
		if ((isWapSPCh(*pbTemp))||(isWapHTCh(*pbTemp)))
		{
			while (HdrNextByte(&pbTemp,pbEnd))
			{
				if (!((isWapSPCh(*pbTemp))||(isWapHTCh(*pbTemp))))
				{
					/* Finished - return TRUE */
					*ppbStart=pbTemp;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/* <any OCTET except CTLs but including LWS> */
BOOL isWapTEXT(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	/* At least one TEXT-byte must exist */
	if (pbTemp<pbEnd)
	{
		if (isWapLWS(&pbTemp,pbEnd))
		{
			/* OK - do nothing */
		}
		else
		{
			if (isWapCtlCh(*pbTemp))
			{
				return FALSE;
			}
		}

		while (pbTemp<pbEnd)
		{
			if (isWapLWS(&pbTemp,pbEnd))
			{
				/* OK - do nothing */
			}
			else
			{
				if (isWapCtlCh(*pbTemp))
				{
					*ppbStart=pbTemp;
					return TRUE;
				}
				pbTemp++;
			}
		}
	}
	return FALSE;		
}

/* 1*<any CHAR except CTLs or tspecials> */
BOOL isWapToken(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		/* at least one Token */
		if (isWapTokenCh(*pbTemp))  
		{
			pbTemp++;

			/* Step through token */
			while (pbTemp<pbEnd)
			{
				if (!(isWapTokenCh(*pbTemp)))
				{
					/* Token ended */
					*ppbStart=pbTemp;
					return TRUE;
				}
				pbTemp++; 
			}
		}
	}
	return FALSE;
}

/* <Octet 127> */
BOOL isWapQuote(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if (*pbTemp==127)
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;
		}
	}
	return FALSE;
}

/* <Octet 0> */
BOOL isWapNoValue(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if (*pbTemp==0)
		{
			pbTemp++;			
			*ppbStart=pbTemp;
			return TRUE;
		}
	}
	return FALSE;
}

/* <Any Octet 0-30> */
BOOL isWapShortLength(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if (isWapShortLengthCh(*pbTemp))
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;
		}
	}
	return FALSE;
}

/* <Octet 31> */
BOOL isWapLengthQuote(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if (isWapLengthQuoteCh(*pbTemp))
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;
		}
	}
	return FALSE;
}

/* *TEXT End-of-string */
BOOL isWapExtensionMedia(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	/* Check if TEXT */
	if (isWapTEXT(&pbTemp,pbEnd))
	{
		/* Check if End-of-text */
		if (pbTemp<pbEnd)
		{
			if (isWapEndOfStringCh(*pbTemp))
			{
				pbTemp++;
				*ppbStart=pbTemp;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/* <Octet 34> *TEXT End-of-string */
BOOL isWapQuotedString(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if (isWapQuoteCh(*pbTemp))
		{
			pbTemp++;

			/* Check if TEXT */
			if (isWapExtensionMedia(&pbTemp,pbEnd))
			{
				*ppbStart=pbTemp;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/* [quote] *TEXT End-of-string */
BOOL isWapTextString(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		/* Step past quote. */
		isWapQuote(&pbTemp,pbEnd);

		/* Check if Extension-media */
		if (isWapExtensionMedia(&pbTemp,pbEnd))
		{
			*ppbStart=pbTemp;
			return TRUE;
		}
	}
	return FALSE;
}

/* Token End-of-string */
BOOL isWapTokenText(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (isWapToken(&pbTemp,pbEnd))
	{
		/* Step past End-of-string */
		if (pbTemp<pbEnd)
		{
			if (isWapEndOfStringCh(*pbTemp))
			{
				pbTemp++;
				*ppbStart=pbTemp;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/* No-Value | Token-text | Quoted-string */
BOOL isWapTextValue(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (isWapTokenText(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	else if (isWapQuotedString(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	else if (isWapNoValue(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	return FALSE;
}

/* 1*2 OCTETS */
BOOL isWapQValue(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if (*pbTemp>='\x80')
		{
			/* Two bytes */
			if (HdrNextByte(&pbTemp,pbEnd))
			{
				pbTemp++;
				*ppbStart=pbTemp;
				return TRUE;
			}
		}
		else
		{
			/* One byte */
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;
		}
	}
	return FALSE;
}

/* Short-integer | Text-string */
BOOL isWapVersionValue(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (isWapShortInteger(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	else if (isWapTextString(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	return FALSE;
}

/* Integer-value | Date-value | Q-value | Version-value | Uri-value */
BOOL isWapCompactValue(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (isWapIntegerValue(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}

	/* Date-value = Long-integer */
	
	else if (isWapQValue(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	else if (isWapVersionValue(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	/* Uri-value = Text-string */
	else if (isWapTextString(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	return FALSE;
}

/* Compact-value | Text-value */
BOOL isWapTypedValue(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (isWapCompactValue(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	else if (isWapTextValue(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	return FALSE;
}

/* Integer-value | Text-value */
BOOL isWapUnTypedValue(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (isWapIntegerValue(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	else if (isWapTextValue(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	return FALSE;
}

/* OCTET (1xxx xxxx) */
BOOL isWapShortInteger(BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;	

	if (pbTemp<pbEnd)
	{
		if (isWapShortIntegerCh(*pbTemp))
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;
		}
	}
	return FALSE;
}

/* <ANY OCTET 0-30> 1*30 OCTET */
BOOL isWapLongInteger (BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;
	BYTE bLength=0;
	UINT32 iCount=0;
	
	if (pbTemp<pbEnd)
	{
		bLength=*pbTemp;
		
		if (isWapShortLength(&pbTemp,pbEnd))
		{
			for (iCount=0; iCount<bLength; iCount++)
			{
				if (!(pbTemp++<pbEnd))
				{
					/* Error - return FALSE */
					return FALSE;
				}
			}

			/* OK! */
			*ppbStart=pbTemp;
			return TRUE;
		}
	}
	return FALSE;
}

/* Short-integer | Long-integer */
BOOL isWapIntegerValue (BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;	

	if (isWapShortInteger(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	else if (isWapLongInteger(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	return FALSE;
}

/* Short-length | (Length-quote Length) */
BOOL isWapValueLength (BYTE **ppbStart, BYTE *pbEnd)
{
	UINT32 iPos=0;
	UINT32 iResult=0;
	BYTE *pbTemp=*ppbStart;
	
	if (pbTemp<pbEnd)
	{
		if (isWapShortLengthCh(*pbTemp))
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;
		}
		else if (isWapLengthQuoteCh(*pbTemp))
		{
			pbTemp++;

			if (ReadMBUInt32(&iResult,pbTemp,(UINT32)(pbEnd-pbTemp),&iPos))
			{
				pbTemp+=iPos;

				*ppbStart=pbTemp;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/* <OCTET 127> */
BOOL isWapShiftDel (BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if (*pbTemp==0x7f)
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;	
		}
	}
	return FALSE;
}

/* <Any OCTET 1-31> */
BOOL isWapShortCutShiftDel (BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if ((*pbTemp>=1)&&(*pbTemp<=31))
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;	
		}
	}
	return FALSE;
}

/* ------------ AUTHORIZATION RULES ------------ */

/* Token-text */
BOOL isWapAuthenticationScheme (BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;	

	if (isWapTokenText(&pbTemp,pbEnd))
	{
		*ppbStart=pbTemp;
		return TRUE;
	}
	return FALSE;
}

/* <OCTET 128> */
BOOL isWapBasic (BYTE **ppbStart, BYTE *pbEnd)
{
	BYTE *pbTemp=*ppbStart;

	if (pbTemp<pbEnd)
	{
		if (isWapBasicCh(*pbTemp))
		{
			pbTemp++;
			*ppbStart=pbTemp;
			return TRUE;	
		}
	}
	return FALSE;
}

/* --------------------------------------------- */

void WSP_EmptyHeaderList (pHEADERDEF pDef)
{
	pHEADERELEMENT pTemp;

	if (pDef!=NULL)
	{
		if (pDef->pHeadList!=NULL) 
		{
			/* First element */
			pTemp=pDef->pHeadList;

			while (pDef->pHeadList!=NULL)
			{
				pDef->pHeadList=pDef->pHeadList->pNext;

				/* Check if in original instream or if the 
				   header is added in the client, i.e., if
				   the pTemp->end < start or pTemp->Start 
				   >= end. */

				/* pTemp never NULL */
				if ((pTemp->pbFieldValStart>pDef->pbEnd) ||
					(pTemp->pbFieldValEnd<pDef->pbData))
				{
					/* This block is added in the client! - DEALLOC */
					DEALLOC(&pTemp->pbFieldValStart);
				}

				DEALLOC(&pTemp);
				pTemp=pDef->pHeadList;
			}
		}
	}
}


/*========================================================================
	WSP_DeleteParameters 
==========================================================================*/
void WSP_DeleteParameters (pWSPPARAMETERS* ppParameters)
{
	if ((ppParameters!=NULL)&&(*ppParameters!=NULL))
	{
		/* Delete struct */
		DEALLOC(ppParameters);
	}
}


pHEADERDEF WSP_PreParseHeaders (BYTE *pbInstream, INT32 iLength, BOOL *fError) 
{
	pHEADERDEF pDef;
	pHEADERELEMENT pHead;
	BYTE *pbToken;
	BYTE *pbInstreamCopy=NULL;

	BYTE *pbTemp=NULL;
	BYTE *pbEnd=NULL;
	BYTE bCurrentCodePage=0;
	
	/* ASSERT:	pbInstream!=NULL
	*/

	/* Copy byte data */
	pbInstreamCopy=B_CopyByteString(pbInstream, iLength);
		
	if (pbInstreamCopy!=NULL)
	{	
		/* Init variables */
		pDef=CreateHEADERDEF(pbInstreamCopy, iLength);
		pbTemp=pbInstreamCopy;
		pbEnd=pbInstreamCopy+iLength;
		bCurrentCodePage=1;

		if (pDef!=NULL)
		{
			if (pbTemp<pbEnd)
			{
				/* Content-Type always the first header */
				pHead=CreateHeaderElement (bCurrentCodePage, Field_Content_Type | 0x80, 
												NULL, pbTemp, pbEnd);
				if (pHead!=NULL)
				{
					AddHeaderElement(pDef,pHead);

					if (StepFieldLength(&pbTemp,pbEnd))
					{
						/* Set FieldValEnd in pHead */
						pHead->pbFieldValEnd=pbTemp;
					}
					else 
					{
						pHead->pbFieldValEnd=pbEnd;
						*fError=TRUE;
					}
				}
				else
				{
					/* Out of memory */
					*fError=TRUE;
				}	
			}

			/* Parse all header fields. */
			while ((pbTemp<pbEnd)&&!(*fError)) 
			{
				/* Pointer to current byte */
				pbToken=pbTemp;
	
				/* Check if Shift-delimeter */
				if (isWapShiftDel(&pbTemp, pbEnd)) 
				{
					if (pbTemp<pbEnd)
					{
						/* Set the new code page */
						bCurrentCodePage=*pbTemp;
					}
					else
					{
						*fError=TRUE;
					}
				}
					
				/* Check if Short-cut-shift-delimeter */
				else if (isWapShortCutShiftDel(&pbTemp, pbEnd))
				{
					/* Set the new code page */
					bCurrentCodePage=*pbToken;
				}
					
				/* Check if Well-known-field-name */
				else if (isWapShortInteger(&pbTemp, pbEnd))
				{
					pHead=CreateHeaderElement (bCurrentCodePage, *pbToken,
												NULL, pbTemp, pbEnd);
					if (pHead!=NULL)
					{
						AddHeaderElement(pDef,pHead);

						if (StepFieldLength(&pbTemp,pbEnd))
						{
							/* Set FieldValEnd in pHead */
							pHead->pbFieldValEnd=pbTemp;
						}
						else 
						{
							*fError=TRUE;
						}
					}
					else
					{
						/* Out of memory */
						*fError=TRUE;
					}
				}
					
				/* Check if Token-Text */
				else if (isWapTokenText(&pbTemp, pbEnd))
				{
					pHead=CreateHeaderElement (0, 0, pbToken, pbTemp, pbEnd);
					if (pHead!=NULL)
					{
						AddHeaderElement(pDef,pHead);
						if (StepFieldLength(&pbTemp,pbEnd))
						{
							/* Set FieldValEnd in pHead */
							pHead->pbFieldValEnd=pbTemp;
						}
						else 
						{
							*fError=TRUE;
						}
					}
				}
				else 
				{
					*fError=TRUE;
				}				
			}
			return pDef;
		}
	}

	/* Could not create HEADERDEF */
	*fError=TRUE;
	return NULL;
}


pHEADERELEMENT WSP_GetHeaderWK (BYTE bCodePage, BYTE bByte, 
							pHEADERELEMENT pFirstElm)
{
	while (pFirstElm!=NULL)
	{
		if ((bCodePage==pFirstElm->bCodePage)&&
				(bByte==((pFirstElm->bFieldName)&(127))))
		{
			return pFirstElm;
		}
		else
		{
			pFirstElm=pFirstElm->pNext;
		}
	}
	return NULL;
}

pHEADERELEMENT WSP_GetHeaderTT (BYTE *pbTokenText, pHEADERELEMENT pFirstElm)
{
	BYTE *pbTemp;
	BYTE *pbTestText;
	BOOL fTest=TRUE;

	while (pFirstElm!=NULL)
	{
		if (pFirstElm->pbFieldName!=NULL)
		{
			pbTemp=pFirstElm->pbFieldName;
			pbTestText=pbTokenText;

			/* Compare the strings. */
			while ((*pbTestText!='\x0')&&(fTest))
			{
				fTest=FALSE;
				if (*pbTemp!='\x0')
				{
					if (*pbTemp==*pbTestText)
					{
						fTest=TRUE;
				
						/* Increase pbTemp */
						if (pbTemp<pFirstElm->pbFieldValEnd)
						{
							pbTemp++;
						}
						else 
						{
							fTest=FALSE;
						}
					}
				}
				/* Increase pbTestText */
				pbTestText++;
			}
			if (fTest)
			{
				/* Check if both strings terminated */
				if ((*pbTestText==0)&&(*pbTemp==0))
				{
					return pFirstElm;
				}
			}
		}
		pFirstElm=pFirstElm->pNext;
	}
	return NULL;
}

BYTE WSP_GetShortInt (BYTE bShortInt)
{
	return (BYTE)((bShortInt)&('\x7F'));
}

UINT32 WSP_GetLongInteger (BYTE **ppbStart, BYTE *pbEnd, BOOL *pfError)
{
	UINT32 iResult=0;
	BYTE bLength=0;
	BYTE *pbValue=NULL;
	BYTE *pbTemp=NULL;
	INT8 i = 0;
	UINT32 t;

	if (*ppbStart<pbEnd)
	{
		pbValue=*ppbStart;
		pbTemp=*ppbStart;
		bLength=*pbValue;

		/* Check if Long-integer */
		if (isWapLongInteger(&pbTemp,pbEnd))
		{
			pbValue++;

			/* Get Long-integer length (maximum 4 bytes) Big-endian. */
			if (bLength>4)
			{
				bLength=4;
			}

			for (i = 0; i < bLength; i++)
			{
				t = pbValue[i];
				t = t << ((bLength -1 - i) * 8);
				iResult |= t;
			}

			/* Step past integer data */
			*ppbStart=pbTemp;
		}
	}
	else 
	{
		*pfError=TRUE;
	}
	return iResult;
}


UINT32 WSP_GetInteger (BYTE **ppbStart, BYTE *pbEnd, BOOL *pfError)
{
	BYTE *pbTemp=NULL;
	UINT32 iInteger=0;

	if (*ppbStart<pbEnd)
	{
		pbTemp=*ppbStart;

		/* Get typed value */
		if (isWapShortInteger(&pbTemp,pbEnd))
		{
			iInteger=WSP_GetShortInt(**ppbStart);
		}
		else if (isWapLongInteger(&pbTemp,pbEnd))
		{
			/* Long-integer-value */
			iInteger=WSP_GetLongInteger (ppbStart,pbEnd,pfError);
		}
		else
		{
			/* Error */
			*pfError=TRUE;
		}
	}
	else
	{
		/* Error */
		*pfError=TRUE;
	}

	return iInteger;
}


BYTE* WSP_GetTextString (BYTE **ppbStart, BYTE *pbEnd, BOOL *pfError)
{
	BYTE* pbValue=*ppbStart;
	
	/* Step past Quote if found. */
	isWapQuote(&pbValue,pbEnd);
	
	/* Check if valid Text-string */
	if (isWapTextString(ppbStart,pbEnd))
	{
		/* OK - Return text */
		*pfError=FALSE;
		return pbValue;
	}
	
	*pfError=TRUE;
	return NULL;
}


/* --------------------------------------------------------------------
	Descr : The function creates a WSPPARAMETERS struct and stores the 
			found parameters in the struct. Only Well-Known parameters
			are considered (table 38 is WSP)

			Supported parameters are:

			Level, Charset, Type (Multipart/Related), Start, and
			Start-info
   -------------------------------------------------------------------- */
void WSP_GetParameters (BYTE *pbStart, BYTE *pbEnd, BOOL *fError, 
								   pWSPPARAMETERS* ppParameters)
{
	BYTE bParam=0;
	BYTE *pbValue=pbStart;
	BYTE *pbTemp=NULL;
	pWSPPARAMETERS pParams=NULL;

	/* Create new parameter struct */
	pParams=NEWSTRUCT(WSPPARAMETERS);
	
	if (pParams==NULL)
	{
		/* Error */
		*fError=TRUE;
		return;
	}

	/* Get all parameters (only if general form - fParameter TRUE)*/
	while ((pbValue<pbEnd)&&(!*fError))
	{		
		bParam=*pbValue;

		if (isWapShortInteger(&pbValue,pbEnd))
		{
			/* Well known parameter (Short-integer) */
			bParam=WSP_GetShortInt(bParam);
			
			/* If Well-known-charset */
			if (bParam==1)
			{
				/* Get charset value (Integer-value) */
				pParams->iCharset=(INT16)(WSP_GetInteger (&pbValue,pbEnd,fError));
			}

			/* Well-known-level*/
			else if (bParam==2)
			{
				pbTemp=pbValue;

				/* Get level value (only integer is used) */
				if (isWapIntegerValue(&pbTemp,pbEnd))
				{
					pParams->iLevel=(BYTE)(WSP_GetInteger (&pbValue,pbEnd,fError));
				}
				else if (isWapTextString(&pbValue,pbEnd))
				{
					/* Not used */
				}
			}

			/* Type (Multipart/Related) */
			else if (bParam==9)
			{
				pbTemp=pbValue;

				/* Check if Constrained-media */
				if (isWapShortInteger(&pbValue,pbEnd))
				{
					pParams->bMPRType=WSP_GetShortInt(*pbTemp);
				}
				else if (isWapExtensionMedia(&pbValue,pbEnd))
				{
					/* Convert the value to byte */
					if (!(WSP_ConvertContentStringByte(&(pParams->bMPRType),&pbTemp)))
					{
						/* Not supported type */
						pParams->bMPRType=0;
					}
				}
			}

			/* Start */
			else if (bParam==0xa)
			{
				/* Text-string */
				pParams->pbMPRStart=WSP_GetTextString(&pbValue,pbEnd,fError);
			}

			/* Start-info */
			else if (bParam==0xb)
			{
				/* Text-string */
				pParams->pbMPRStartInfo=WSP_GetTextString(&pbValue,pbEnd,fError);
			}
		}
		else if (isWapLongInteger(&pbValue,pbEnd))
		{
			if (isWapTypedValue(&pbValue,pbEnd))
			{
				/* Not supported here */
			}
			else
			{
				/* Error */
				*fError=TRUE;
			}
		}
		else if (isWapTokenText(&pbValue,pbEnd))
		{
			/* Untyped-parameter */
			if (isWapUnTypedValue(&pbValue,pbEnd))
			{
				/* Not supported here */
			}
			else
			{
				/* Error */
				*fError=TRUE;
			}
		}
		else
		{
			/* Error */
			*fError=TRUE;
		}
	}

	*ppParameters=pParams;
}


BYTE WSP_GetContentTypeByte (pHEADERELEMENT pElement, BOOL *fError, 
							 pWSPPARAMETERS* ppParameters)
{
	BYTE *pbValue=NULL;
	BYTE *pbTemp=NULL;
	BOOL fParameter=FALSE;
	BYTE bValue=0;

	*fError=TRUE;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;
			pbTemp=pbValue;

			/* Check if Constrained-media or Content-general-form */
			if (isWapShortInteger(&pbValue,pElement->pbFieldValEnd))
			{
				bValue=WSP_GetShortInt(*pbTemp);
				*fError=FALSE;
			}
			else if (isWapExtensionMedia(&pbValue,pElement->pbFieldValEnd))
			{
				if (WSP_ConvertContentStringByte(&bValue,&pbTemp))
				{
					*fError=FALSE;

					if (bValue==0x37)
					{
						/* WTA-WML (text) internally --> Not supported */
						*fError=TRUE;
					}
					else if (bValue==0x38)
					{
						/* WTA-WML internally ---> 0x75 */
						bValue=0x75;
					}
				}
			}
			/* Content-general-form ( Value-length Media-type ) */
			else if (isWapValueLength(&pbValue,pElement->pbFieldValEnd))
			{
				/* Parameter */
				fParameter=TRUE;

				pbTemp=pbValue;

				/* Check if well-known value */
				if (isWapShortInteger(&pbValue,pElement->pbFieldValEnd))
				{
					bValue=WSP_GetShortInt(*pbTemp);
					*fError=FALSE;
				}
				else if (isWapExtensionMedia(&pbValue,pElement->pbFieldValEnd))
				{
					if (WSP_ConvertContentStringByte(&bValue,&pbTemp))
					{
						*fError=FALSE;
					}
				}
			}
			
			/* Get all parameters (only if general form - fParameter TRUE)*/
			if (fParameter)
			{
				WSP_GetParameters (pbValue, pElement->pbFieldValEnd, 
											  fError, ppParameters);
			}
			else
			{
				/* Store NULL in ppParameters */
				*ppParameters=NULL;
			}
		}
	}

	return bValue;
}

BYTE *WSP_GetContentTypeString (pHEADERELEMENT pElement, BOOL *fError, 
								pWSPPARAMETERS* ppParameters)
{
	BYTE *pbValue=NULL;
	BYTE *pbNewString=NULL;
	BYTE *pbTemp=NULL;
	BOOL fParameter=FALSE;
	BYTE bValue=0;

	*fError=TRUE;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;
			pbTemp=pbValue;

			/* Check if Constrained-media or Content-general-form */
			if (isWapShortInteger(&pbValue,pElement->pbFieldValEnd))
			{
				bValue=WSP_GetShortInt(*pbTemp);

				/* Convert byte to string */
				if (WSP_ConvertContentStringByte(&bValue,&pbNewString))
				{
					*fError=FALSE;
				}
			}
			else if (isWapExtensionMedia(&pbValue,pElement->pbFieldValEnd))
			{
				*fError=FALSE;

				/* Copy string (-1 indicates null-terminated) */
				pbNewString=B_CopyByteString(pbTemp, -1);

			}
			/* Content-general-form ( Value-length Media-type ) */
			else if (isWapValueLength(&pbValue,pElement->pbFieldValEnd))
			{
				/* Parameter */
				fParameter=TRUE;

				pbTemp=pbValue;

				/* Check if well-known value */
				if (isWapShortInteger(&pbValue,pElement->pbFieldValEnd))
				{
					bValue=WSP_GetShortInt(*pbTemp);

					/* Convert byte to string */
					if (WSP_ConvertContentStringByte(&bValue,&pbNewString))
					{
						*fError=FALSE;
					}

				}
				else if (isWapExtensionMedia(&pbValue,pElement->pbFieldValEnd))
				{
					*fError=FALSE;

					/* Copy string (-1 indicates null-terminated) */
					pbNewString=B_CopyByteString(pbTemp, -1);
				}
			}
			
			/* Get all parameters (only if general form - fParameter TRUE)*/
			if (fParameter)
			{
				WSP_GetParameters (pbValue, pElement->pbFieldValEnd, 
											  fError, ppParameters);
			}
			else
			{
				/* Store NULL in ppParameters */
				*ppParameters=NULL;
			}
		}
	}

	return pbNewString;
}

BYTE WSP_GetCacheControlByte (pHEADERELEMENT pElement, UINT32 *piDeltaSec,
									BOOL *fError)
{
	BYTE bValue=0;
	BYTE *pbValue=NULL;
	BYTE *pbTemp=NULL;

	*fError=TRUE;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;

			if (pbValue<pElement->pbFieldValEnd)
			{
				bValue=*pbValue;
	
				/* Check if well known cache control-value */
				if ((bValue>=128)&&(bValue<=138)&&
						(bValue!=130)&&(bValue!=132))
				{
					*fError=FALSE;
				}
				else if (isWapTokenText(&pbValue,pElement->pbFieldValEnd))
				{
					/* Cache extension */
				}
				else if (isWapValueLength(&pbValue,pElement->pbFieldValEnd))
				{
					if (pbValue<pElement->pbFieldValEnd)
					{
						bValue=*pbValue;

						/* Check if no-cache */
						if (bValue==128)
						{
							*fError=FALSE;
						}
						
						/* Check if Max-age, Max-stale or Min-Fresh */
						else if ((bValue==130)||(bValue==130)||(bValue==130))
						{
							if (HdrNextByte(&pbValue,pElement->pbFieldValEnd))
							{
								pbTemp=pbValue;
								
								if (isWapShortInteger(&pbTemp,pElement->pbFieldValEnd))
								{
									*piDeltaSec=(UINT32)WSP_GetShortInt(*pbValue);
									*fError=FALSE;
								}
								else if (isWapLongInteger(&pbTemp,pElement->pbFieldValEnd))
								{
									*fError=FALSE;
									*piDeltaSec=WSP_GetLongInteger(&pbValue,pElement->pbFieldValEnd,fError);
								}
							}

						}

						/* Check if Private */
						else if ((bValue==135))
						{
							*fError=FALSE;
						}

						else if (isWapTokenText(&pbValue,pElement->pbFieldValEnd))
						{
							/* Cache extension */
						}
					}
				}
			}
		}
	}
	return bValue;
}


/* Value-length Challenge */
BYTE *WSP_GetProxyAuthenticateRealm (pHEADERELEMENT pElement)
{
	BYTE *pbValue=NULL;
	BYTE *pbResult=NULL;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;

			/* Check length */
			if (!isWapValueLength(&pbValue,pElement->pbFieldValEnd))
			{
				/* Error in length - return NULL */
			}

			/* Get Challenge */
			if (isWapBasic(&pbValue,pElement->pbFieldValEnd))
			{
				/* Basic. Get Realm-value */
				pbResult=pbValue;

				/* Check if text-string */
				if (!isWapTextString(&pbValue,pElement->pbFieldValEnd))
				{
					return NULL;
				}

			}
			else if (isWapAuthenticationScheme(&pbValue,pElement->pbFieldValEnd))
			{
				/* Authentication-scheme. Get Realm-value */
				pbResult=pbValue;

				/* Check if text-string */
				if (!isWapTextString(&pbValue,pElement->pbFieldValEnd))
				{
					return NULL;
				}
			}
		}
	}
	return pbResult;
}

BYTE *WSP_GetWWWAuthenticateRealm (pHEADERELEMENT pElement)
{
	return (WSP_GetProxyAuthenticateRealm(pElement));
}

/* Delta-seconds-value */
UINT32 WSP_GetAge (pHEADERELEMENT pElement, BOOL *pfError)
{
	BYTE *pbTemp=NULL;
	BYTE *pbValue=NULL;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;
			pbTemp=pbValue;
				
			if (isWapShortInteger(&pbTemp,pElement->pbFieldValEnd))
			{
				/* Short integer - return BYTE */
				return (UINT32) (WSP_GetShortInt(*pbValue));
			}
			else if (isWapLongInteger(&pbTemp,pElement->pbFieldValEnd))
			{
				/* Get long integer */
				return WSP_GetLongInteger(&pbValue,pElement->pbFieldValEnd,pfError);
			}
		}
	}
	return 0;
}

/* Date-value */
UINT32 WSP_GetDate (pHEADERELEMENT pElement, BOOL *pfError)
{
	BYTE *pbValue=NULL;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;

			/* Read date */
			return WSP_GetLongInteger(&pbValue,pElement->pbFieldValEnd,pfError);
		}
	}	

	*pfError=TRUE;
	return 0;
}

/* Text-string */
BYTE *WSP_GetEtag (pHEADERELEMENT pElement)
{
	return WSP_GetLocation(pElement);
}

/* Date-value */
UINT32 WSP_GetExpires (pHEADERELEMENT pElement, BOOL *pfError)
{
	return WSP_GetDate(pElement,pfError);
}

/* Date-value */
UINT32 WSP_GetLastModified (pHEADERELEMENT pElement, BOOL *pfError)
{
	return WSP_GetDate(pElement,pfError);
}

/* Uri-value */
BYTE *WSP_GetLocation (pHEADERELEMENT pElement)
{
	BYTE *pbTemp=NULL;
	BOOL fError=FALSE;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbTemp=pElement->pbFieldValStart;

			return WSP_GetTextString(&pbTemp,pElement->pbFieldValEnd,&fError);
		}
	}
	/* Error */
	return NULL;
}

/* Uri-value */
BYTE *WSP_GetContentLocation (pHEADERELEMENT pElement)
{
	return WSP_GetLocation(pElement);
}

/* Uri-value */
BYTE *WSP_GetContentBase (pHEADERELEMENT pElement)
{
	return WSP_GetLocation(pElement);
}

UINT8 WSP_GetPushFlag (pHEADERELEMENT pElement)
{
	BYTE *pbTemp=NULL;
	BYTE *pbValue=NULL;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;
			pbTemp=pbValue;
				
			if (isWapShortInteger(&pbTemp,pElement->pbFieldValEnd))
			{
				/* Short integer - return BYTE */
				return (UINT8) (WSP_GetShortInt(*pbValue));
			}
		}
	}
	return 0;
}


/* Uri-value */
BYTE *WSP_GetXInitiatorURI(pHEADERELEMENT pElement)
{
	return WSP_GetLocation(pElement);
}

/* Uri-value */
BYTE *WSP_GetXContentURI (pHEADERELEMENT pElement)
{
	return WSP_GetLocation(pElement);
}

/*  Profile-warning-value = Warn-code | (Value-length Warn-code Warn-target *Warn-date)

	Warn-code = Short-integer
	Warn-target = Uri-value
	Warn-date = Date-value
*/
BYTE WSP_GetProfileWarning (pHEADERELEMENT pElement)
{
	BYTE* pbTemp=NULL;
	BYTE* pbValue=NULL;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;
			pbTemp=pbValue;

			/* Check if Short-integer */
			if (isWapShortInteger(&pbValue,pElement->pbFieldValEnd))
			{
				return WSP_GetShortInt(*pbTemp);
			}

			/* Value-length */
			else if (isWapValueLength(&pbValue,pElement->pbFieldValEnd))
			{
				pbTemp=pbValue;

				/* Check if Short-integer */
				if (isWapShortInteger(&pbValue,pElement->pbFieldValEnd))
				{
					return WSP_GetShortInt(*pbTemp);
				}
			}
		}
	}

	return 0;
}


/*========================================================================
	WSP_GetPushHeaderAppId
==========================================================================*/
BOOL WSP_GetPushHeaderAppId (pHEADERELEMENT pElement, BYTE** ppbURIHeaderAppId, int* piCodeHeaderAppId)
{
	BYTE* pbTemp=NULL;
	BYTE* pbValue=NULL;
	BOOL fError = FALSE;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;
			pbTemp=pbValue;

			/* Check if integer value */
			if (isWapIntegerValue(&pbTemp,pElement->pbFieldValEnd))
			{
				*piCodeHeaderAppId = WSP_GetInteger (&pbValue,pElement->pbFieldValEnd,&fError);
			}
			else if (isWapTextString(&pbTemp,pElement->pbFieldValEnd))
			{
				*ppbURIHeaderAppId = WSP_GetTextString(&pbValue,pElement->pbFieldValEnd,&fError);
			}
			else
			{
				/* Error */
				fError = TRUE;
			}
		}
	}

	/* Return result, returns !fError so TRUE will be returned if ok  */
	return (!fError);
}



/*========================================================================
	WSP_GetMd5
==========================================================================*/
BYTE* WSP_GetMd5 (pHEADERELEMENT pElement, BOOL *pfError)
{
	BYTE *pbValue=NULL;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;

			/* Check value size - MUST be 17 bytes */
			if ((pElement->pbFieldValEnd - pElement->pbFieldValStart) == 17 )

			/* Get length - only short length of 16 can be correct */
			if ( WSP_GetShortInt (*pbValue) == 16 )
			{
				pbValue++;

				*pfError=FALSE;
				return pbValue;
			}
		}
	}

	*pfError=TRUE;
	return NULL;
}


/* Returns the correct value in the Base64 alphabet */
BYTE WSP_GetBase64Value (WCHAR pwchChar)
{
	if ((pwchChar > 0x40) && (pwchChar < 0x5B))
	{
		/* A - Z */
		return (BYTE)(pwchChar - 0x41);
	}
	else if ((pwchChar > 0x60) && (pwchChar < 0x7B))
	{
		/* a - z */
		return (BYTE)(pwchChar - 0x47);
	}
	else if ((pwchChar > 0x2F) && (pwchChar < 0x3A))
	{
		/* 0 - 9 */
		return (BYTE)(pwchChar + 0x04);
	}
	else if (pwchChar == 0x2B)
	{
		/* + */
		return (BYTE)(0x3E);
	}
	else if (pwchChar == 0x2F)
	{
		/* / */
		return (BYTE)(0x3F);
	}
	else if (pwchChar == 0x3D)
	{
		/* = */
		return (BYTE)(0x40);
	}

	/* Not in alphabet - return FF */
	return (BYTE)(0xFF);
}


/*========================================================================
	WSP_ConvertBase64ToByte
==========================================================================*/
BYTE* WSP_ConvertBase64ToByte (WCHAR* pwchMd5, INT16* piNbrBytes)
{
	/* ASSERT: piNbrBytes!=NULL
	*/

	BYTE* pbResult=NULL;
	BYTE* pbTemp=NULL;
	INT16 iLength=-1;
	UINT8 iBitsFound=0;
	BYTE bThis=0;
	BYTE bPrev=0;
	BOOL fQuit=FALSE;

	if (pwchMd5!=NULL)
	{
		/* Calculate length */
		iLength=STRINGLENGTH(pwchMd5);
		iLength= (INT16) ((iLength*3)/4 + (iLength*3)%4);

		/* Allocate memory */
		pbResult=NEWARRAY(BYTE,iLength);
		pbTemp=pbResult;

		iLength=0;

		/* Convert */
		while ((*pwchMd5!=0) && (!fQuit))
		{
			/* Convert character to value */
			bThis=WSP_GetBase64Value (*pwchMd5);

			if (bThis < 0x40)
			{
				iBitsFound+=6;

				if (iBitsFound>=8)
				{
					iBitsFound-=8;

					/* Store one byte */
					*pbTemp++ = (BYTE)( (bPrev<<(6-iBitsFound)) | (bThis>>(iBitsFound)) );

					/* Add one to nbr of bytes */
					iLength++;
				}
			}
			else if (bThis == 0x40)
			{
				/* Padding found - quit */
				fQuit=TRUE;
			}

			/* Next char */
			bPrev=bThis;
			pwchMd5++;
		}
	}

	/* Return */
	*piNbrBytes=iLength;
	return pbResult;
}





/* ============ ADDRESS FUNCTIONS ==============
	Functions for handling address parsing.
   ============================================= */


BOOL WSP_ParseAddress( BYTE* pbData, INT32 iLength, pADDRESS* ppAddress )
{
	BYTE* pbTemp=pbData;
	BYTE* pbEnd=pbData+iLength;
	BYTE bFirst=0;
	BOOL fStreamOk=TRUE;
	pADDRESS pTempAddress=NULL;
	pADDRESS pLast=NULL;
	
	if (ppAddress==NULL)
	{
		return FALSE;
	}

	/* Delete old list (if any) */
	WSP_DeleteAddress( ppAddress );
	pLast=*ppAddress;

	/* Step through the byte stream - one address type at a time. */
	while ((pbTemp<pbEnd)&&(fStreamOk))
	{
		bFirst=*pbTemp;

		/* Create ADDRESS struct */
		pTempAddress=NEWSTRUCT(ADDRESS);

		if (pTempAddress!=NULL)
		{
			/* Set default values */
			pTempAddress->iBearerType=-1;
			pTempAddress->iPortNumber=-1;
			pTempAddress->pbAddress=NULL;
			pTempAddress->pNext=NULL;
			pTempAddress->uiAddressLength=0;
		}
		else
		{
			/* Out of memory */
			return FALSE;
		}

		/* Get address length (the six least significant bits) */
		pTempAddress->uiAddressLength=(BYTE) ((bFirst) & (0x3f));

		/* Get Bearer Type (if the most significant bit is set) */
		if ((bFirst)&(0x80))
		{
			/* Next byte */
			if (HdrNextByte(&pbTemp,pbEnd))
			{
				pTempAddress->iBearerType=*pbTemp;
			}
			else
			{
				fStreamOk=FALSE;
			}
		}

		/* Get Port Number (if the second most significant bit is set) */
		if ((bFirst)&(0x40))
		{
			if (HdrNextByte(&pbTemp,pbEnd))
			{
				UINT16 iA=*pbTemp;

				if (HdrNextByte(&pbTemp,pbEnd))
				{
					pTempAddress->iPortNumber=(iA<<8)|(*pbTemp);
				}
				else
				{
					fStreamOk=FALSE;
				}
			}
			else
			{
				fStreamOk=FALSE;
			}
		}
		
		if (pTempAddress->uiAddressLength>0)
		{
		
			if ( (pbTemp+(pTempAddress->uiAddressLength))<pbEnd ) 
			{
				/* Get the address */
				pTempAddress->pbAddress=pbTemp+1;

				/* Step past address */
				pbTemp+=(pTempAddress->uiAddressLength);
			}
			else
			{
				fStreamOk=FALSE;
			}
		}

		pbTemp++;

		/* Add address struct to list */
		if (pLast!=NULL)
		{
			pLast->pNext=pTempAddress;
		}
		else
		{
			/* Add to beginning of list */
			*ppAddress=pTempAddress;
		}

		pLast=pTempAddress;
	}

	if (!fStreamOk)
	{
		WSP_DeleteAddress( ppAddress );
		return FALSE;
	}

	return TRUE;
}


void WSP_DeleteAddress( pADDRESS* ppAddress )
{
	pADDRESS pTemp=NULL;

	if (ppAddress!=NULL)
	{

		while (*ppAddress!=NULL)
		{		
			pTemp=*ppAddress;
			*ppAddress=(*ppAddress)->pNext;
			DEALLOC(&pTemp);
		}		
	}
}


/* ============================================= */

#include "contypes.h"

/* Compare two strings and return the length of the common
   prefix of the two. */
static UINT32
CommonPrefix (BYTE *s, BYTE *t)
{
  UINT32 k = 0;

  while (*s && (*s++ == *t++))
    k++;

  return k;
}

/* Search a prefix tree pn for the string t.
   On successful search, return the index associated with the string,
   otherwise return -1. */
static INT8
SearchPrefixTree (const node_t *pn, BYTE *t)
{
  INT8 p = 0;
  node_t *q;

  while (p >= 0) {
    q = (node_t *)pn + p;
    if (*(q->s) != *t) {
      p = q->sibling;
    }
    else {
#ifdef FEATURE_WAP
      if (CommonPrefix ((BYTE *)q->s, t) < q->len) { break; }
#else
      if (CommonPrefix (q->s, t) < q->len) { break; }
#endif //FEATURE_WAP
      t += q->len;
      if (*t) {
				p = q->child;
      }
      else
				return q->idx;
    }
  }
  /* Not found */
  return -1;
}

/* Return a copy of the string with index idx.
   Return NULL if index is not in the table. */
static BYTE *
ReverseLookup (const node_t *pn, INT8 idx)
{
  INT32 i;

  if (idx < 0 || idx >= NUM_NODES)
    return NULL;

  for (i = idx; i < NUM_NODES; i++) {
    if (pn[i].idx == idx) {
      /* Compute length of string */
      INT32 j, l;
      BYTE *s, *t;

      for (l = 0, j = i; j >= 0; j = pn[j].parent) {
				l += pn[j].len;
      }
      s = NEWARRAY (BYTE, l + 1);
      t = s + l;
      *t = '\0';
      for (j = i; j >= 0; j = pn[j].parent) {
				t -= pn[j].len;
				B_COPYSTRINGN (t, pn[j].s, pn[j].len);
      }
      return s;
    }
  }
  return NULL;
}

BOOL WSP_ConvertContentStringByte (BYTE *pbValue, BYTE **ppbText)
{
	if (*ppbText != NULL)	{
		BYTE buf[128];
		INT8 k;

		/* Convert string to lowercase */
		DowncaseString (buf, *ppbText);
		
		/* Convert from text to byte */
		k = SearchPrefixTree (treeNodes, buf);

		if (k >= 0) {
			*pbValue = k;
			return TRUE;
		}
		return FALSE;
	}
	else {
		/* Convert from byte to text */
		*ppbText = ReverseLookup (treeNodes, *pbValue);
		return *ppbText != NULL;
	}
}

/*========================================================================
	WSP_PreParseMultipart
==========================================================================*/
pMULTIPARTSTR WSP_PreParseMultipart (BYTE *pbInstream, UINT32 iLength)
{
	pMULTIPARTSTR pMultiList=NULL;
	pMULTIPARTSTR pMultiTemp=NULL;
	pMULTIPARTSTR pMultiLast=NULL;
	BYTE* pbEnd=pbInstream+iLength;
	
	BOOL fError=FALSE;
	UINT32 iCount=0;
	UINT32 iNbrOfEntries=0;
	UINT32 iPos=0;

	UINT32 iLen=0;

	/* Check if within bounds */
	if (pbInstream<pbEnd)
	{
		/* Get number of entries */
		if (ReadMBUInt32(&iNbrOfEntries,pbInstream,(UINT32)(pbEnd-pbInstream-1),&iPos))
		{
			/* Increase pbInstream with the number of bytes indicated 
			   by the number of bytes for the MBUInt32. */
			pbInstream+=iPos;

			/* Split the multipart data */
			while ((iCount<iNbrOfEntries)&&(!fError))
			{
				/* Create MULTIPARTSTR */
				pMultiTemp=NEWSTRUCT(MULTIPARTSTR);
			
				if (pMultiTemp!=NULL)
				{
					pMultiTemp->pNext=NULL;
				
					/* Check if within bounds */
					if (pbInstream<pbEnd)
					{
						iPos=0;
	
						/* Get header length */
						if (ReadMBUInt32(&iLen,pbInstream,(UINT32)(pbEnd-pbInstream-1),&iPos))
						{
							pbInstream+=iPos;

							/* Store headerlen */
							pMultiTemp->iHdrLen=iLen;
						}
						else 
						{
							fError=TRUE;
						}
					}
					else 
					{
						fError=TRUE;
					}
	
					/* Check if within bounds */
					if ((pbInstream<pbEnd)&&(!fError))
					{
						iPos=0;
	
						/* Get data length */
						if (ReadMBUInt32(&iLen,pbInstream,(UINT32)(pbEnd-pbInstream-1),&iPos))
						{
							pbInstream+=iPos;

							/* Store Data len */
							pMultiTemp->iBodyLen=iLen;

							/* Store pointer to header */
							pMultiTemp->pbHdrStart=pbInstream;
							pbInstream+=pMultiTemp->iHdrLen;

							/* Store pointer to data */
							pMultiTemp->pbBodyStart=pbInstream;
							pbInstream+=iLen;

							/* Check boundaries */
							if (!( ((pMultiTemp->pbHdrStart+pMultiTemp->iHdrLen-1) < pMultiTemp->pbBodyStart) &&
								 ((pMultiTemp->pbBodyStart+pMultiTemp->iBodyLen-1) < pbEnd ) ))
							{
								fError=TRUE;
							}
						}
						else 
						{
							fError=TRUE;
						}
					}
					else 
					{
						fError=TRUE;
					}
		
					if (!fError)
					{
						/* Add the multipart element to list */
						if (pMultiList==NULL)
						{
							pMultiList=pMultiTemp;
							pMultiLast=pMultiList;
						}
						else 
						{
							/* Last in the list */
							pMultiLast->pNext=pMultiTemp;
							pMultiLast=pMultiTemp;
						}
					}
					else 
					{
						DEALLOC (&pMultiTemp);	

						/* Empty list and return NULL */
						while (pMultiList!=NULL)
						{
							pMultiTemp=pMultiList;
							pMultiList=pMultiList->pNext;
							DEALLOC (&pMultiTemp);	
						}
						return NULL;
					}
				}
				else 
				{
					fError=TRUE;
				}

				iCount++;
			}
		}
	}
	return pMultiList;
}



/*========================================================================
	WSP_DeleteMultipartList
==========================================================================*/
void WSP_DeleteMultipartList (pMULTIPARTSTR* ppList)
{
	pMULTIPARTSTR pTemp=NULL;

	if (ppList!=NULL)
	{
		while (*ppList!=NULL)
		{
			pTemp=*ppList;
			*ppList=(*ppList)->pNext;
			DEALLOC(&pTemp);			
		}
	}
}


/*========================================================================
	WSP_DeleteMultipartListWithContent
==========================================================================*/
void WSP_DeleteMultipartListWithContent (pMULTIPARTSTR* ppList)
{
	pMULTIPARTSTR pTemp=NULL;

	if (ppList!=NULL)
	{
		while (*ppList!=NULL)
		{
			pTemp=*ppList;
			*ppList=(*ppList)->pNext;

			/* Delete memory */
			DEALLOC(&pTemp->pbHdrStart);
			DEALLOC(&pTemp->pbBodyStart);
			DEALLOC(&pTemp);			
		}
	}
}


/*========================================================================
	WSP_MultipartAddPart
==========================================================================*/
void WSP_MultipartAddPart (pMULTIPARTSTR* ppList, BYTE** ppbHeader, 
						   BYTE** ppbData, UINT32 iHeaderLen, UINT32 iDataLen)
{
	pMULTIPARTSTR pNew=NEWSTRUCT(MULTIPARTSTR);
	pMULTIPARTSTR pTemp;

	/* ASSERT: ppList!=NULL
			   ppbHeader!=NULL
			   ppbData!=NULL
	*/

	if (pNew!=NULL)
	{
		/* Store values in multipart struct */
		pNew->pbHdrStart=*ppbHeader;
		pNew->iHdrLen=iHeaderLen;
		pNew->pbBodyStart=*ppbData;
		pNew->iBodyLen=iDataLen;
		pNew->pNext=NULL;
		
		/* Store struct last in list */
		pTemp=*ppList;

		if (pTemp==NULL)
		{
			/* First element in list */
			*ppList=pNew;
		}
		else
		{
			/* Find last element */
			while (pTemp->pNext!=NULL)
			{
				pTemp=pTemp->pNext;
			}

			pTemp->pNext=pNew;
		}

		/* Set inparameters to point to NULL */
		*ppbHeader=NULL;
		*ppbData=NULL;
	}
}


/*========================================================================
	WSP_BuildMultipart
==========================================================================*/
BOOL WSP_BuildMultipart (pMULTIPARTSTR* ppList, BYTE** ppbContentData,
						 UINT32* piDataLen, UINT32 iTotalSize)
{
	/* Check inparameters */
	if ((ppList!=NULL)&&(ppbContentData!=NULL)&&(piDataLen!=NULL))
	{
		pMULTIPARTSTR pTemp=*ppList;
		UINT32 iSize=0;
		UINT32 iNbrOfParts=0;
		BYTE* pbTemp;

		/* Calculate size for multipart content */
		while (pTemp!=NULL)
		{
			iNbrOfParts++;

			/* Add space for lengths */
			iSize += UintvarLen(pTemp->iHdrLen);
			if (iTotalSize>(pTemp->iBodyLen))
				iSize += UintvarLen(iTotalSize);
			else
				iSize += UintvarLen(pTemp->iBodyLen);

			/* Add size of part */
			iSize += pTemp->iHdrLen;
			iSize += pTemp->iBodyLen;			

			/* Next part */
			pTemp=pTemp->pNext;	
		}

		/* Add space for number of entries (uintvar) */
		iSize+=UintvarLen (iNbrOfParts);

		if (iSize>0)
		{
			/* Allocate memory */
			*ppbContentData=NEWARRAY(BYTE,iSize);
			pbTemp=*ppbContentData;

			if (*ppbContentData==NULL)
			{
				/* Could not allocate memory */
				return FALSE;
			}

			/* Store size */
			*piDataLen=iSize;

			/* Store number of entries in multipart data */
			pbTemp+=IntToUintvar (iNbrOfParts, pbTemp);

			pTemp=*ppList;

			/* Store the parts */
			while (pTemp!=NULL)
			{
				/* Store header size */
				pbTemp+=IntToUintvar (pTemp->iHdrLen, pbTemp);

				/* Store data size */
				if (iTotalSize>(pTemp->iBodyLen))
					pbTemp+=IntToUintvar (iTotalSize, pbTemp);
				else
					pbTemp+=IntToUintvar (pTemp->iBodyLen, pbTemp);

				/* Copy header data */
				B_COPYSTRINGN (pbTemp,pTemp->pbHdrStart,pTemp->iHdrLen);
				pbTemp+=pTemp->iHdrLen;

				/* Copy body data */
				B_COPYSTRINGN (pbTemp,pTemp->pbBodyStart,pTemp->iBodyLen);
				pbTemp+=pTemp->iBodyLen;

				/* Get next part */
				pTemp=pTemp->pNext;
			}

			/* OK */
			return TRUE;
		}
	}

	return FALSE;
}




BYTE *WSP_CreateStructure (BYTE bField, UINT32 iLength, BYTE **ppbTemp)
{
	BYTE *pbHeaderString=NULL;

	pbHeaderString=NEWARRAY(BYTE,iLength);

	if (pbHeaderString!=NULL)
	{
		*ppbTemp=pbHeaderString;

		/* Add Field Name */
		*(*ppbTemp)++=(BYTE)(bField|128);

	}		
	
	return pbHeaderString;
}

void WSP_EndCreateHeader (pHEADBUILDSTR *ppList, pHEADBUILDSTR pHead, UINT32 iLength)
{
	pHead->iLength=iLength;
	pHead->pNext=NULL;

	/* Addto the end of the list */
	if (*ppList!=NULL)
	{
		(*ppList)->pNext=pHead;
	}

	/* Update ppList */
	*ppList=pHead;
}


/*	
	THE pbContentType PARAMETER IS USED IF IT IS NOT NULL. OTHERWISE
	THE bContentType PARAMETER IS USED. IF THE fUseParams is TRUE, 
	A PARAMETER IS ADDED (only charset is used at the moment).

	Content-type-value = Constrained-media | Content-general-form

	Content-general-form = Value-length Media-type
	Media-type = (Well-known-media | Extension-media) *(Parameter)

	Well-known-media = Integer-value
	Extension-media = *TEXT End-of-string

    Charset parameter = 0x01 (Well-known-charset)
	Well-known-charset = Integer-value
*/
pHEADBUILDSTR WSP_CreateContentTypeHeader (BYTE* pbContentType, BYTE bContentType, 
										   WSPPARAMETERS sParams, BOOL fUseParams )
{
	pHEADBUILDSTR pHead=NEWSTRUCT(HEADBUILDSTR);
	BYTE* pbHeader=NULL;
	BYTE* pbTemp=NULL;
	UINT16 iLength=0;
	UINT16 iStringLength=0;
	UINT8 iCharEncLen=0;
	INT16 iCharset=sParams.iCharset;
	BYTE tempArray[2];

	if (pHead!=NULL)
	{
		if (fUseParams)
		{
			/* Calculate length of CharEnc parameter */
			if ( iCharset >= 128 )
			{
				/* If the byte is greater or equal to x80 - encode as long 
				   integer. Add space for Short-length */
				iCharEncLen=1;

				/* Calculate number of bytes in Multi-octet-integer, one or two. */
				if( iCharset < 256 )
				{
					tempArray[0] = (BYTE) (iCharset & 0xff);
					iCharEncLen++;
				}
				else
				{
					tempArray[0] = (BYTE) ((iCharset >> 8) & 0xff);
					tempArray[1] = (BYTE) (iCharset & 0xff);
					iCharEncLen+=2;
				}
			}
			else if (iCharset > -1 )
			{
				/* One byte */
				iCharEncLen=1;
			}
			else
			{
				/* One byte */
				iCharEncLen=1;

				/* Set charset to US-ASCII */
				iCharset=IANA_CHARSET_USASCII;
			}

			/* Add space for Value-length, parameter Charset,
			   and charset integer */
			iLength+=(2+iCharEncLen);
		}

		/* Add length for content-type-value (byte or string) */
		if (pbContentType!=NULL)
		{
			/* Add room for string */
			iStringLength=B_STRINGLENGTH (pbContentType);

			iLength+=iStringLength+1;
		}
		else
		{
			iLength++;
		}

		/* Create new array */
		pbHeader=NEWARRAY(BYTE,iLength);
		pbTemp=pbHeader;

		if (pbHeader==NULL)
		{
			/* Error */
			return NULL;
		}

		if (fUseParams)
		{
			/* Add Value-length */
			*pbTemp++=(BYTE)(iLength-1);
		}

		/* Add Content-type */
		if (pbContentType!=NULL)
		{
			/* Copy string */
			B_COPYSTRING (pbTemp,pbContentType);
			pbTemp+=iStringLength+1;
		}
		else
		{
			*pbTemp++=((BYTE)( bContentType | 0x80 ));
		}

		if (fUseParams)
		{
			/* Add parameter Charset */
			*pbTemp++=(0x81);

			/* Add parameter value */
			if (iCharEncLen==1)
			{
				/* Short-integer */
				*pbTemp++=(BYTE)( ((BYTE)(iCharset)) | 0x80 );
			}
			else
			{
				/* Long-integer, add Short-length */
				*pbTemp++=(BYTE)(iCharEncLen-1);

				/* Add integer */
				B_COPYSTRINGN (pbTemp,tempArray,iCharEncLen-1);
			}
		}

		/* Store data in struct */
		pHead->pbHeader=pbHeader;
		pHead->iLength=iLength;
		pHead->pNext=NULL;
	}

	return pHead;
}


/* Create WSP header Content-Disposition : form-data; Name = pbNameData 
							OR
					 Content-Disposition : attachment [; Filename = pbNameData]

   Content-disposition-value = Value-length Disposition *(Parameter)
   Disposition = Form-data | Attachment

   Form-data  = <octet 128>
   Attachment = <octet 129>

   Name parameter		= 0x05 (Text-string)
   FileName parameter	= 0x06 (Text-string)

   e.g., AE 07 80 85 'N' 'A' 'M' 'E' 00
*/
pHEADBUILDSTR WSP_CreateContentDispositionHeader 
					(BYTE* pbNameData, BYTE bDisposition, BYTE bParameter)
{
	pHEADBUILDSTR pHead=NEWSTRUCT(HEADBUILDSTR);
	BYTE* pbHeader=NULL;
	BYTE* pbTemp=NULL;
	UINT32 iLength=0;
	UINT32 iDataLength=0;
	UINT32 iNameLength=0;
	UINT8 iUintVarLen=0;

	if (pHead!=NULL)
	{
		if (pbNameData!=NULL)
		{
			/* Calculate name data length including termination char */
			iNameLength=B_STRINGLENGTH(pbNameData)+1;

			iDataLength=iNameLength;

			/* Check if quote needed */
			if (pbNameData[0]>127)
			{
				iDataLength++;
			}

			/* Add space for Disposition and parameter */
			iDataLength+=2;

			/* Calculate total length (include Content-disposition) */
			iLength=iDataLength+1;

			if (iDataLength > 30)
			{
				/* Get length of uintvar. */
				iUintVarLen = (UINT8) UintvarLen (iDataLength);

				/* Add length for bytes and Value-length (octet 31) */
				iLength += (iUintVarLen+1);
			}
			else
			{
				/* Add length for Value-length */
				iLength++;
			}
		}
		else
		{
			/* No parameter */
			iLength=2;
		}

		/* Create new array */
		pbHeader=WSP_CreateStructure(Field_Content_Disposition,iLength,&pbTemp);

		if (pbHeader==NULL)
		{
			/* Error */
			return NULL;
		}

		/* If iLength == 2  --> No parameter */
		if (iLength != 2) 
		{
			/* Check if DATA length > 30 */
			if (iDataLength > 30)
			{
				/* Add Length-quote */
				*pbTemp++=31;

				/* Convert to uintvar. */
				(void) IntToUintvar (iDataLength, pbTemp);

				pbTemp+=iUintVarLen;
			}
			else
			{
				*pbTemp++=(BYTE)(iDataLength);
			}
		}

		/* Store Disposition */
		*pbTemp++=bDisposition;

		/* Store parameter (if any) */
		if ((pbNameData!=NULL)&&(iLength!=2))
		{
			*pbTemp++ = (BYTE)( bParameter | 0x80 );

			/* Copy data */
			B_COPYSTRINGN(pbTemp,pbNameData,iNameLength);
		}

		/* Store data in struct */
		pHead->pbHeader=pbHeader;
		pHead->iLength=iLength;
		pHead->pNext=NULL;
	}

	return pHead;
}


/*========================================================================
	WAE_AddTextFormDataPart
==========================================================================*/
/*  Modified by GBU,000510, use type
void WAE_AddTextFormDataPart (void** ppMultiPartList, BYTE* pbNameData,  
							  BYTE** ppbValueData, UINT32 iValueLen, INT16 iCharEnc)
*/
void WAE_AddTextFormDataPart (pMULTIPARTSTR* ppMultiPartList, BYTE* pbNameData,  
                              BYTE** ppbValueData, UINT32 iValueLen, INT16 iCharEnc)
{
	/* ASSERT: ppMultipartList!=NULL
			   ppbNameData!=NULL
			   ppbValueData!=NULL
	*/

	pHEADBUILDSTR pList=NULL;
	WSPPARAMETERS sParams;	
	BYTE* pbHeader=NULL;
	UINT32 iHeaderLen=0;

	/* Create parameter struct for WSP Content-type header */
	sParams.iCharset=iCharEnc;
	sParams.iLevel=0;
	sParams.bMPRType=0;
	sParams.pbMPRStart=NULL;
	sParams.pbMPRStartInfo=NULL;

	/* Content-type : text/plain with charset as indicated by the parameter 
	  (iCharEnc). "Charset is required when the content contains 
	  characters not in the US-ASCII characer set" (WML 9.5.1) */

	/* Create WSP header Content-type : text/plain; charset = iCharEnc */
	pList=WSP_CreateContentTypeHeader (NULL,0x03,sParams,TRUE);

	if (pList!=NULL)
	{
		/* Create WSP header Content-Disposition : form-data; name = pbNameData */
		pList->pNext=WSP_CreateContentDispositionHeader (pbNameData,128,5);

		/* Build header */
		pbHeader=WSP_BuildHeader(pList,&iHeaderLen);

		/* Add multipart record */
/*  Modified by GBU.000510, removed cast
		WSP_MultipartAddPart ((pMULTIPARTSTR*)ppMultiPartList,&pbHeader,
							   ppbValueData,iHeaderLen,iValueLen);
*/
        WSP_MultipartAddPart (ppMultiPartList,&pbHeader,
                              ppbValueData,iHeaderLen,iValueLen);

	}

	/* Deallocate ppbValueData (if not already done) */
	DEALLOC (ppbValueData);
}


/*  Modified by GBU,000510, use type
void WAE_AddGenericFormDataPart (void** ppMultiPartHandle, BYTE** ppbData, UINT16 iDataLen, 
								 BYTE* pbContentType, BYTE* pbContentDispositionFileName)
*/
void WAE_AddGenericFormDataPart (pMULTIPARTSTR* ppMultiPartHandle, BYTE** ppbData, UINT16 iDataLen,
                                 BYTE* pbContentType, BYTE* pbContentDispositionFileName)
{
	/* ASSERT: ppMultipartHandle!=NULL
	*/

	pHEADBUILDSTR pList=NULL;
	WSPPARAMETERS sParams;	
	BYTE* pbHeader=NULL;
	UINT32 iHeaderLen=0;
	BYTE bContentType=0;
	BYTE* pbContentCopy=NULL;
	UINT16 iLength=0;

	/* Create parameter struct for WSP Content-type header */
	sParams.iCharset=-1;
	sParams.iLevel=0;
	sParams.bMPRType=0;
	sParams.pbMPRStart=NULL;
	sParams.pbMPRStartInfo=NULL;

	/* Copy content type string */
	if (pbContentType!=NULL)
	{
		iLength= (UINT16)(B_STRINGLENGTH (pbContentType)+1);
		pbContentCopy = NEWARRAY (BYTE,iLength);

		if (pbContentCopy!=NULL)
		{
			B_COPYSTRING (pbContentCopy,pbContentType);	

			/* Convert Content-type to byte (if possible) */
			if (WSP_ConvertContentStringByte (&bContentType,&pbContentCopy))
			{
				pList=WSP_CreateContentTypeHeader (NULL,bContentType,sParams,FALSE);
			}
			else
			{
				/* Content-type could not be converted to byte, store as string. */
				pList=WSP_CreateContentTypeHeader (pbContentCopy,0,sParams,FALSE);
			}
			DEALLOC(&pbContentCopy);

			if (pList!=NULL)
			{
				/* Create WSP header Content-Disposition : attachment (octet 129)
										[; filename (octet 6) = pbContentDispositionFileName] */
				pList->pNext=WSP_CreateContentDispositionHeader (pbContentDispositionFileName,129,6);

				/* Build header */
				pbHeader=WSP_BuildHeader(pList,&iHeaderLen);

				/* Add multipart record */
/*  Modified by GBU.000510, removed cast
				WSP_MultipartAddPart ((pMULTIPARTSTR*)ppMultiPartHandle,&pbHeader,
									   ppbData,iHeaderLen,iDataLen);
*/
                WSP_MultipartAddPart (ppMultiPartHandle,&pbHeader,
                                      ppbData,iHeaderLen,iDataLen);

			}
		}
	}

	/* Deallocate ppbData (if not already done) */
	DEALLOC (ppbData);
}


BOOL WSP_CreateHeaderField (BYTE bField, BYTE *pbData, UINT32 iDataLength,
						pHEADBUILDSTR *ppList)
{
	pHEADBUILDSTR pHead=NULL;
	BYTE *pbHeaderString=NULL;
	BYTE *pbTemp=NULL;
	BYTE *pbDataTemp=pbData;
	UINT32 iLength=iDataLength;
	UINT8 iUintVarLen=0;
	UINT8 iCount=0;
	UINT8 iShortLength=0;
	BOOL fQuote=FALSE;
	BOOL fConvert=FALSE;
	BYTE bContent=0;

	if (pbData!=NULL)
	{
		/* Create new header element. */
		pHead=NEWSTRUCT(HEADBUILDSTR);

		if (pHead!=NULL)
		{
			/* Add length for Field Name - length is total length
			   including field name and all data. */
			iLength++;

			switch (bField)
			{
			case Field_Accept:
				fConvert=WSP_ConvertContentStringByte(&bContent,&pbData);

				if (fConvert)
				{
					/* Converted to byte. Set length to 2. */
					iLength=2;
				}

				pbHeaderString=WSP_CreateStructure(bField,iLength,&pbTemp);

				if (pbHeaderString!=NULL)
				{
					if (fConvert)
					{
						/* Add content as byte. */
						*pbTemp=(BYTE)(bContent|128);
					}
					else
					{
						/* Add content as string. */

						/* Copy the string. */
						B_COPYSTRINGN(pbTemp,pbDataTemp,iDataLength);
					}
						
					pHead->pbHeader=pbHeaderString;

					WSP_EndCreateHeader(ppList,pHead,iLength);
					return TRUE;
				}
				
				break;
			case Field_Accept_Charset:
			case Field_Accept_Language:
			case Field_Bearer_Indication:
			case Field_Accept_Application:

				bContent=*pbDataTemp;

				/* Examine indata size */
				if (iDataLength>1)
				{
					/* Encode as long integer */
					iShortLength=2;

					/* Add space for Short-length and header length */
					iLength+=2;
				}

				/* Examine if the byte is greater or equal to x80. If
				   so - encode as long integer. */
				else if (bContent>=128)
				{
					/* Encode as long integer */
					iShortLength=1;

					/* Add space for Short-length and header length */
					iLength+=2;
				}

				/* Create new array */
				pbHeaderString=WSP_CreateStructure(bField,iLength,&pbTemp);

				if (pbHeaderString!=NULL)
				{
					if (iShortLength!=0)
					{
						/* Integer is long, add header length */
						*pbTemp++=(BYTE)(iShortLength+1);

						/* Add short-length */
						*pbTemp++=iShortLength;

						/* Copy data */
						for (iCount=0; iCount<iDataLength; iCount++)
						{
							*pbTemp++=*pbDataTemp++;
						}
					}
					else
					{
						/* Short integer */

						/* Add content as byte. */
						*pbTemp=(BYTE)(bContent|128);
					}

					pHead->pbHeader=pbHeaderString;

					WSP_EndCreateHeader(ppList,pHead,iLength);
					return TRUE;
				}
				break;
			case Field_Authorization:
			case Field_Proxy_Authorization:
				{
					BYTE* pbPwdStart;

					/* If first characte in user-id or password is in range 
					   128-255, a quote character must precede it */
					if (*pbData>=128)
					{
						iLength++;
						iDataLength++;
					}

					/* Check password */
					pbPwdStart=pbData;
					while (*pbPwdStart++!=0);

					if (*pbPwdStart>=128)
					{
						iLength++;
						iDataLength++;
					}
					
					/* Add length for Value-Length and Basic */
					if (iDataLength>=30)
					{
						/* Add length for Value-length */
						iLength++;

						/* Get length of uintvar. */
						iUintVarLen = (UINT8) UintvarLen (iDataLength + 1);

						/* Add space for bytes */
						iLength+=iUintVarLen;
					}
					else
					{
						/* Add length for Value-length */
						iLength++;
					}

					/* Add length for Basic */
					iLength++;

					pbHeaderString=WSP_CreateStructure(bField,iLength,&pbTemp);

					if (pbHeaderString!=NULL)
					{
						/* Add Value-length */
						if (iDataLength>=30)
						{
							/* Add Length-quote */
							*pbTemp++=31;

							/* Convert to uintvar. */
							(void) IntToUintvar (iDataLength + 1, pbTemp);

							pbTemp+=iUintVarLen;
						}
						else
						{
							*pbTemp++=(BYTE) (iDataLength+1);
						}

						/* Add Basic <Octet 128> */
						*pbTemp++=128;

						/* Add User-id */
						if (*pbDataTemp>=128)
						{
							/* Add Quote */
							*pbTemp++=127;
						}
						B_COPYSTRING(pbTemp,pbDataTemp);

						/* Step past length of username */
						pbTemp+=B_STRINGLENGTH(pbDataTemp);

						/* Set termination character */
						*pbTemp++=0;

						/* Add Password */
						if (*pbPwdStart>=128)
						{
							/* Add Quote */
							*pbTemp++=127;
						}
						B_COPYSTRING(pbTemp,pbPwdStart);
						
						/* Step past length of password */
						pbTemp+=B_STRINGLENGTH(pbPwdStart);

						/* Set termination character */
						*pbTemp++=0;

						pHead->pbHeader=pbHeaderString;

						WSP_EndCreateHeader(ppList,pHead,iLength);
						return TRUE;
					}
				}
				break;
			case Field_If_Modified_Since:
				/* Add space for short length */
				iLength++;

				pbHeaderString=WSP_CreateStructure(bField,iLength,&pbTemp);

				if (pbHeaderString!=NULL)
				{
					/* Add short length */
					*pbTemp++=(BYTE)iDataLength;

					/* Add the date */
					B_COPYSTRINGN(pbTemp,pbDataTemp,iDataLength);

					pHead->pbHeader=pbHeaderString;

					WSP_EndCreateHeader(ppList,pHead,iLength);
					return TRUE;
				}
				break;

			/* Text-String, Uri-Value */
			case Field_Referer:
			case Field_User_Agent:
			case Field_If_None_Match:
			case Field_Profile:

				/* If in range 128-255 quote char must precede */
				if (*pbData>127)
				{
					fQuote=TRUE;
					iLength++;
				}

				/* Create string */
				pbHeaderString=WSP_CreateStructure(bField,iLength,&pbTemp);

				if (pbHeaderString!=NULL)
				{
					/* Add quote (if any) */
					if (fQuote)
					{
						*pbTemp++=127;
					}
	
					/* Copy the string. */
					B_COPYSTRINGN(pbTemp,pbDataTemp,iDataLength);

					pHead->pbHeader=pbHeaderString;

					WSP_EndCreateHeader(ppList,pHead,iLength);

					return TRUE;
				}
				break;

			/* CCPP-Profile */
			case Field_Profile_Diff:

				if (pbData!=NULL)
				{
					/* Add length for Value-Length and Basic */
					if (iDataLength>=30)
					{
						/* Add length for Value-length */
						iLength++;

						/* Get length of uintvar. */
						iUintVarLen = (UINT8) UintvarLen (iDataLength);

						/* Add space for bytes */
						iLength+=iUintVarLen;
					}
					else
					{
						/* Add length for Value-length */
						iLength++;
					}

					/* Create string */
					pbHeaderString=WSP_CreateStructure(bField,iLength,&pbTemp);

					if (pbHeaderString!=NULL)
					{
						/* Add Value-length */
						if (iDataLength>=30)
						{
							/* Add Length-quote */
							*pbTemp++=31;

							/* Convert to uintvar. */
							(void) IntToUintvar (iDataLength,pbTemp);

							pbTemp+=iUintVarLen;
						}
						else
						{
							*pbTemp++=(BYTE) (iDataLength);
						}
		
						/* Copy the data. */
						B_COPYSTRINGN(pbTemp,pbDataTemp,iDataLength);

						pHead->pbHeader=pbHeaderString;

						WSP_EndCreateHeader(ppList,pHead,iLength);

						return TRUE;
					}
				}

				break;
			}

			/* Remove pHead */
			DEALLOC(&pHead);
		}
	}
	return FALSE;
}

BYTE *WSP_BuildHeader (pHEADBUILDSTR pList, UINT32 *piDataLength)
{
	pHEADBUILDSTR pTemp=NULL;
	pHEADBUILDSTR pDelete=NULL;
	BYTE *pbTarget=NULL;
	BYTE *pbTargetTemp=NULL;
	BYTE *pbSource=NULL;

	if ((pList!=NULL)&&(piDataLength!=NULL))
	{
		pTemp=pList;
		*piDataLength=0;

		/* Calculate length of result string. */
		while (pTemp!=NULL)
		{
			*piDataLength+=pTemp->iLength;
			pTemp=pTemp->pNext;
		}

		/* Create target string. */
		pbTarget=NEWARRAY(BYTE,(*piDataLength));

		if (pbTarget!=NULL)
		{
			pTemp=pList;
			pbTargetTemp=pbTarget;

			/* Assemble target string. */
			while (pTemp!=NULL)
			{
				pbSource=pTemp->pbHeader;

				/* Copy BYTE by BYTE */
				B_COPYSTRINGN(pbTargetTemp,pbSource,pTemp->iLength);

				pbTargetTemp+=pTemp->iLength;

				pDelete=pTemp;
				pTemp=pTemp->pNext;

				/* Delete struct */
				DEALLOC(&pDelete->pbHeader);
				DEALLOC(&pDelete);
			}
		}
	}
	return pbTarget;
}
