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
		000216	JPR		Corrections according to TR 206

   ================================================================== */

#include "HdrUtil.h"
#include "HeadDef.h"
#include "confvars.h"
#include "CmmnRsrc.h"

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
		iLen=B_STRINGLENGTH((char *)pbString)+1;
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


/* -------------------------------------------------------------------
	Descr : The function stores the parameters Charset and Level in
			the appropriate variables.
   ------------------------------------------------------------------- */
void WSP_GetContentTypeParameters (BYTE *pbStart, BYTE *pbEnd, BOOL *fError, 
									 INT16 *piCharset, UINT8 *piLevel)
{
	BYTE bParam=0;
	BYTE *pbValue=pbStart;
	BYTE *pbParamValue=NULL;
	INT16 iValue=0;
	BYTE iParameter=0;

	/* Get all parameters (only if general form - fParameter TRUE)*/
	while ((pbValue<pbEnd)&&(!*fError))
	{		
		bParam=*pbValue;

		if (isWapShortInteger(&pbValue,pbEnd))
		{
			/* Well known parameter (Short-integer) */
			bParam=WSP_GetShortInt(bParam);

			pbParamValue=pbValue;

			/* Get typed value */

			if (isWapShortInteger(&pbValue,pbEnd))
			{
				if (bParam==1)
				{
					/* Get value */
					iValue=(INT16)(WSP_GetShortInt(*pbParamValue));
				}
				else
				{
					iParameter=WSP_GetShortInt(*pbParamValue);
				}
			}
			else if (isWapLongInteger(&pbValue,pbEnd))
			{
				/* Long-integer-value */
				iValue=(INT16)WSP_GetLongInteger (&pbParamValue,
							pbEnd,fError);

			}
			else
			{
				/* Error */
				*fError=TRUE;
			}
			
			/* If Well-known-charset */
			if (bParam==1)
			{
				/* Set charset variable */
				*piCharset=iValue;
			}

			/* Well-known-level*/
			else if (bParam==2)
			{
				/* Set level variable */
				*piLevel=iParameter;
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
}


BYTE WSP_GetContentTypeByte (pHEADERELEMENT pElement, BOOL *fError, 
									 INT16 *piCharset, UINT8 *piLevel)
{
	BYTE *pbValue=NULL;
	BYTE *pbTemp=NULL;
	BOOL fParameter=FALSE;
	BYTE bValue=0;

	*fError=TRUE;
	*piCharset=IANA_CHARSET_INVALID;

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
				WSP_GetContentTypeParameters (pbValue, pElement->pbFieldValEnd, fError, 
									  piCharset, piLevel);
			}
		}
	}

	return bValue;
}

BYTE *WSP_GetContentTypeString (pHEADERELEMENT pElement, BOOL *fError, 
									 INT16 *piCharset, UINT8 *piLevel)
{
	BYTE *pbValue=NULL;
	BYTE *pbNewString=NULL;
	BYTE *pbTemp=NULL;
	BOOL fParameter=FALSE;
	BYTE bValue=0;

	*fError=TRUE;
	*piCharset=IANA_CHARSET_INVALID;

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
				WSP_GetContentTypeParameters (pbValue, pElement->pbFieldValEnd, fError, 
									  piCharset, piLevel);
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
	BYTE *pbValue=NULL;
	BYTE *pbTemp=NULL;

	if (pElement!=NULL)
	{
		/* Check code page. */

		/* Just code page 1 */
		if (pElement->bCodePage==1)
		{ 
			pbValue=pElement->pbFieldValStart;
			pbTemp=pbValue;

			/* Step past Quote if found. */
			isWapQuote(&pbValue,pElement->pbFieldValEnd);
			
			/* Check if valid Text-string */
			if (isWapTextString(&pbTemp,pElement->pbFieldValEnd))
			{
				return pbValue;
			}
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







/* ========= AUTHENTICATION FUNCTIONS ==========
	Functions for handling authorization, i.e. 
	keep track of the visited realms, and the 
	appropriate passwords, etc.
   ============================================= */

/* Returns the struct for the specified Host. NULL
   if not found. */
pHOSTSTR FindAuthHost (pUA pUser, BYTE *pbURL)
{
	pHOSTSTR pTempHost=NULL;
	BYTE *pbHostPart=NULL;

	/* Check if at least one host */
	if (pUser->pVisitedHosts!=NULL)
	{
		pTempHost=pUser->pVisitedHosts;

		/* Get Host Part of the URL */
		if (b_GetHost(pbURL,&pbHostPart))
		{
			/* Find the Host */
			while (pTempHost!=NULL)
			{
				if (pTempHost->pbURL!=NULL)
				{						
					/* Case INsensitive comparision. */
					if (ByteStringNCmpNoCase(pbHostPart,pTempHost->pbURL,INT32_MAX)==0)
					{
						/* Host Found */
						DEALLOC(&pbHostPart);
						return pTempHost;
					}
				}
				pTempHost=pTempHost->pNext;
			}
			DEALLOC(&pbHostPart);
		}
	}
	return NULL;
}

/* Returns the struct for the specified Realm in the
   specified Host. NULL if not found. */
pREALMSTR FindAuthRealm (pHOSTSTR pHost, BYTE *pbRealm)
{
	pREALMSTR pTempRealm=NULL;

	/* Get first Realm */
	pTempRealm=pHost->pRealmList;

	while (pTempRealm!=NULL)
	{
		if (pTempRealm->pbRealm!=NULL)
		{
			/* Case sensitive comparision. */
/*  Modified by GBU,000114 */
/*
			if (B_COMPARESTRING(pbRealm,pTempRealm->pbRealm)==0)
*/
            if (strcmp((const char *)pbRealm, (const char *)pTempRealm->pbRealm)==0)
			{
				/* Realm Found */
				return pTempRealm;
			}
		}
		pTempRealm=pTempRealm->pNext;
	}
	return NULL;
}

/* Removes everything after the last slash in the path 
   e.g. /path/wml/test.wml --> /path/wml/  
   (The termination character is moved ) 
   Returns TRUE if OK, FALSE otherwise				*/
BOOL RemoveAfterSlash (BYTE *pbPath)
{
	/* ASSERT:	Valid, complete path (e.g. /path/test.wml)
	*/

	BYTE *pbLast=NULL;
	BYTE *pbTemp=NULL;

	if (pbPath!=NULL)
	{
		pbTemp=pbPath;

		/* Find last slash '/' */
		while (*pbTemp)
		{
			if (*pbTemp=='/')
			{
				pbLast=pbTemp;
			}
			pbTemp++;
		}

		if (pbLast!=NULL)
		{
			pbLast++;

			/* Move termination character */
			*pbLast=0;
			return TRUE;
		}
	}
	return FALSE;
}

/* Updates the Credentials in the Realm struct.  */
BOOL UpdateRealm (pREALMSTR pRealm, BYTE *pbCredentials, UINT8 iCredLength)
{
	BYTE *pbCredentialsCopy=NULL;

	/* ASSERT:	pRealm!=NULL; 
	*/

	/* Update Credentials */
	if (pbCredentials!=NULL)
	{
		pbCredentialsCopy=B_CopyByteString(pbCredentials,(INT32)iCredLength);
	}

	if (pbCredentialsCopy!=NULL)
	{
		DEALLOC(&pRealm->pbCredentials);
		pRealm->pbCredentials=pbCredentialsCopy;
		pRealm->iCredLength=iCredLength;
		return TRUE;
	}

	return FALSE;
}


pREALMSTR CreateNewRealm (BYTE *pbRealm, BYTE *pbCredentials,
							UINT8 iCredLength, BYTE *pbURL)
{
	BYTE *pbRealmCopy=NULL;
	BYTE *pbCredentialsCopy=NULL;
	pREALMSTR pNewRealm=NULL;
	BYTE *pbPath=NULL;

	pNewRealm=NEWSTRUCT(REALMSTR);

	if (pNewRealm!=NULL)
	{
		/* Copy parameters */

		if (pbRealm!=NULL)
		{
			pbRealmCopy=B_CopyByteString(pbRealm,-1);
		}

		if (pbCredentials!=NULL)
		{
			pbCredentialsCopy=B_CopyByteString(pbCredentials,(INT32)iCredLength);
		}

		b_GetPath(pbURL,&pbPath);

		/* Remove everything after last slash '/' */
		if (pbPath!=NULL)
		{
			RemoveAfterSlash(pbPath);
		}

		if ((pbRealmCopy!=NULL)&&(pbCredentialsCopy!=NULL)
					&&(pbPath!=NULL))
		{
			/* Set parameters */
			pNewRealm->pbRealm=pbRealmCopy;
			pNewRealm->pbRelativeURL=pbPath;
			pNewRealm->pbCredentials=pbCredentialsCopy;
			pNewRealm->iCredLength=iCredLength;

			return pNewRealm;
		}
		else
		{
			/* Out of memory */
			DEALLOC(&pbRealmCopy);
			DEALLOC(&pbCredentialsCopy);
			DEALLOC(&pbPath);
			DEALLOC(&pNewRealm);
		}
	}
	return NULL;
}


BOOL StoreAuthParams (pUA pUser, BYTE *pbURL, BYTE *pbRealm,
						BYTE *pbCredentials, UINT8 iCredLength)
{
	pHOSTSTR pHost=NULL;
	pREALMSTR pRealm=NULL;
	pREALMSTR pNewRealm=NULL;
	BYTE *pbHost=NULL;

	/* ASSERT:	pUser!=NULL; 
				pbURL!=NULL;
				No relative URL:s
	*/

	/* Realm required */
	if ((pbRealm!=NULL)&&(pUser!=NULL)&&(pbURL!=NULL))
	{
		/* Get the Host */
		pHost=FindAuthHost(pUser, pbURL);

		if (pHost!=NULL)
		{
			/* Host Found */

			/* Find Realm */
			pRealm=FindAuthRealm (pHost, pbRealm);

			if (pRealm!=NULL)
			{
				/* Realm Found, indicates that credentials should be
				   updated, but NOT url. */
				if (UpdateRealm (pRealm, pbCredentials, iCredLength))
				{
					return TRUE;
				}
			}
			else
			{
				/* Create New Realm Entry */
				pNewRealm=CreateNewRealm(pbRealm,pbCredentials,iCredLength,pbURL);

				if (pNewRealm!=NULL)
				{
					/* Add to Realm List */
					pNewRealm->pNext=pHost->pRealmList;
					pHost->pRealmList=pNewRealm;

					return TRUE;
				}
			}
		}
		else
		{
			/* Create New Host entry */
			pHost=NEWSTRUCT(HOSTSTR);

			if (pHost!=NULL)
			{
				/* Set parameters */
				if (b_GetHost(pbURL,&pbHost))
				{
					pHost->pbURL=pbHost;

					/* Create New Realm Entry */
					pNewRealm=CreateNewRealm(pbRealm,pbCredentials,iCredLength,pbURL);

					if (pNewRealm!=NULL)
					{
						/* Add to Realm List */
						pNewRealm->pNext=NULL;
						pHost->pRealmList=pNewRealm;

						/* Add to Host List */
						pHost->pNext=pUser->pVisitedHosts;
						pUser->pVisitedHosts=pHost;

						return TRUE;
					}
					DEALLOC(&pbHost);
				}
				/* Out of memory */
				DEALLOC(&pHost);
			}
		}
	}
	return FALSE;
}


BOOL CheckAuth (pUA pUser, BYTE *pbURL, BYTE *pbRealm, 
					BYTE **ppbCredentials, UINT8 *piCredLength)
{
	pHOSTSTR pHost=NULL;
	BYTE *pbTempPath=NULL;
	BYTE *pbTempUrl=NULL;
	BYTE *pbInPath=NULL;
	pREALMSTR pRealm=NULL;

	/* ASSERT:	pUser!=NULL
				No relative URL:s
	*/

	if ((pUser!=NULL)&&(pbURL!=NULL))
	{
		/* Get the Host */
		pHost=FindAuthHost(pUser, pbURL);
			
		if (pHost!=NULL)
		{
			/* Host found */

			/* Is realm specified? */
			if (pbRealm!=NULL)
			{
				pRealm=FindAuthRealm (pHost,pbRealm);

				if (pRealm!=NULL)
				{
					/* Get path of in-url */
					if(b_GetPath(pbURL,&pbInPath))
					{
						/* Find the longest common path-prefix of the
						   in-url and the realm-url. */
						pbTempUrl=b_LongestCommonPrefix (pRealm->pbRelativeURL,pbInPath);
	
						DEALLOC(&pbInPath);

						if (pbTempUrl==NULL)
						{
							/* Set the url to "/" */
							pbTempUrl=NEWARRAY(BYTE,2);

							if (pbTempUrl!=NULL)
							{
								pbTempUrl[0]='/';
								pbTempUrl[1]='\0';
							}
						}

						DEALLOC(&pRealm->pbRelativeURL);
						pRealm->pbRelativeURL=pbTempUrl;
					}

					/* Realm found */
					*ppbCredentials=pRealm->pbCredentials;
					*piCredLength=pRealm->iCredLength;
					return TRUE;
				}
			}
			else 
			{
				/* Realm NOT specified */
				pRealm=pHost->pRealmList;

				while (pRealm!=NULL)
				{
					/* Check URL */
					if(b_GetPath(pbURL,&pbTempPath))
					{
						RemoveAfterSlash(pbTempPath);

						if (pRealm->pbRelativeURL!=NULL)
						{
							/* Check if 'below' in pRealm->pchPath */
/*  Modified by GBU,0001014 */
/*
                            if (B_STRSTRING(pbTempPath,pRealm->pbRelativeURL))
*/
							if (strstr((const char *)pbTempPath,(const char *)pRealm->pbRelativeURL))
							{
								/* The string pchRealtiveURL is a subset of the
								   specified URL pchTempPath */
								*ppbCredentials=pRealm->pbCredentials;
								*piCredLength=pRealm->iCredLength;
			
								DEALLOC(&pbTempPath);
								return TRUE;
							}
						}
						DEALLOC(&pbTempPath);
					}
					pRealm=pRealm->pNext;
				}
			}
		}
	}

	/* Not found */
	*ppbCredentials=NULL;
	*piCredLength=0;
	return FALSE;
}


void DeleteHostList (pUA pUser)
{
	pREALMSTR pRealm=NULL;
	pREALMSTR pRealmTemp=NULL;
	pHOSTSTR pHostTemp=NULL;

	if (pUser!=NULL)
	{
		/* Delete all hosts */
		while (pUser->pVisitedHosts!=NULL)
		{
			pHostTemp=pUser->pVisitedHosts;

			if (pUser->pVisitedHosts->pRealmList!=NULL)
			{
				pRealm=pUser->pVisitedHosts->pRealmList;

				/* Delete all realms under this host */
				while (pRealm!=NULL)
				{
					pRealmTemp=pRealm;
					DEALLOC(&pRealm->pbRealm);
					DEALLOC(&pRealm->pbRelativeURL);
					DEALLOC(&pRealm->pbCredentials);
					pRealm=pRealm->pNext;
					DEALLOC(&pRealmTemp);
				}

			}
			pUser->pVisitedHosts=pUser->pVisitedHosts->pNext;

			DEALLOC(&pHostTemp->pbURL);
			DEALLOC(&pHostTemp);
		}
		pUser->pVisitedHosts=NULL;
	}
}

/* ============================================= */

#include "ConTypes.h"

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
      if (CommonPrefix ((BYTE *)q->s, (BYTE *)t) < q->len) { break; }
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

  if (idx < 0 || idx >= 59)
    return NULL;

  for (i = idx; i < 59; i++) {
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

pMULTIPARTSTR WSP_PreParseMultipart (BYTE *pbInstream, UINT32 iLength)
{
	pHEADERDEF pTemp=NULL;
	pMULTIPARTSTR pMultiTemp=NULL;
	pMULTIPARTSTR pMultiList=NULL;
	pMULTIPARTSTR pMultiLast=NULL;
	BYTE *pbHeaderStart=NULL;
	
	BOOL fError=FALSE;
	UINT32 iCount=0;
	UINT32 iBoundCheck=0;
	UINT32 iNbrOfEntries=0;
	UINT32 iPos=0;

	UINT32 iHeaderLen=0;
	UINT32 iDataLen=0;

	/* Check if within bounds */
	if (iBoundCheck<iLength)
	{
		/* Get number of entries */
		if (ReadMBUInt32 (&iNbrOfEntries,pbInstream,iLength-1,&iPos))
		{
			pbInstream+=iPos;
			iBoundCheck+=iPos;

			/* Split the multipart data */
			for (iCount=0; iCount<iNbrOfEntries; iCount++)
			{
				/* Create MULTIPARTSTR */
				pMultiTemp=NEWSTRUCT(MULTIPARTSTR);
			
				if (pMultiTemp!=NULL)
				{
					pMultiTemp->pNext=NULL;
				
					/* Check if within bounds */
					if (iBoundCheck<iLength)
					{
						iPos=0;
	
						/* Get header length */
						if (ReadMBUInt32 (&iHeaderLen,pbInstream,iLength-1,&iPos))
						{
							pbInstream+=iPos;
							iBoundCheck+=iPos;
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
					if (iBoundCheck<iLength)
					{
						iPos=0;
	
						/* Get data length */
						if (ReadMBUInt32 (&iDataLen,pbInstream,iLength-1,&iPos))
						{
							pMultiTemp->iBodyLen=iDataLen;
							pbInstream+=iPos;

							/* Header start pointer INCLUDING content-type value */
							pbHeaderStart=pbInstream;
							iBoundCheck+=iPos;
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
					if (iBoundCheck<iLength)
					{
						/* Create temporary HEADERDEF */
						pTemp=NEWSTRUCT(HEADERDEF);
	
						if (pTemp!=NULL)
						{
							pTemp->pbData=pbInstream;
							pTemp->pHeadList=NULL;
	
							/* Step past content-type */
/*							if (StepFieldLength (&pTemp->pbCurByte, pTemp->pbEnd))
							{*/
								/* Past content-type 
								iBoundCheck+=(pTemp->pbCurByte-pbInstream);
								pbInstream=pTemp->pbCurByte;
							}
							else 
							{
								fError=TRUE;
							}*/
	
							DEALLOC (&pTemp);
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
					if (iBoundCheck<iLength)
					{
						/* Step past headers and data */
						if ((iHeaderLen+iDataLen+iBoundCheck-1)<iLength)
						{
							/*	Header start INCLUDING the content-type value */
							pMultiTemp->pbHdrStart=pbHeaderStart;
							pMultiTemp->pbBodyStart=pbInstream+iHeaderLen;

							/* Header length INCLUDING content-type value */
							pMultiTemp->iHdrLen=iHeaderLen+(pbInstream-pbHeaderStart);

							pbInstream+=(iHeaderLen+iDataLen);
							iBoundCheck+=(iHeaderLen+iDataLen);
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
			}
		}
	}
	return pMultiList;
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
						*pbTemp++=(iShortLength+1);

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
						B_COPYSTRING((char *)pbTemp,(const char *)pbDataTemp);

						/* Step past length of username */
						pbTemp+=B_STRINGLENGTH((char *)pbDataTemp);

						/* Set termination character */
						*pbTemp++=0;

						/* Add Password */
						if (*pbPwdStart>=128)
						{
							/* Add Quote */
							*pbTemp++=127;
						}
#ifdef WAPCORE_FOR_WARNING
						B_COPYSTRING((char *)pbTemp,(const char *)pbPwdStart);
#else
						B_COPYSTRING((void *)pbTemp,(const void *)pbPwdStart);
#endif //WAPCORE_FOR_WARNING
						
						/* Step past length of password */
						pbTemp+=B_STRINGLENGTH((char *)pbPwdStart);

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
			case Field_Referer:
			case Field_User_Agent:
				if (pbData!=NULL)
				{
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
	
			pList=NULL;
		}
	}
	return pbTarget;
}
