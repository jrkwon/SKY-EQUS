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
	FILE: WSPCapa.c
  
	Functions for WSP Capabilities Encoding/Decoding for WAP.
	Source file.

	Rev history:
		990831	JPR		Created
		990909	JPR		Updated
=========================================================================*/

#include "wspcapa.h"

pCAPABILITIESDEF WSP_PreParseCapabilities (BYTE* pbData, INT32 iLength,
										   BOOL *pfError)
{
	pCAPABILITIESDEF pCapaList=NULL;
	pCAPABILITY pEntry=NULL;
	pCAPABILITY pLastEntry=NULL;
	BYTE* pbEnd=NULL;
	BYTE* pbTemp=NULL;
	UINT32 iCapaLen=0;
	UINT32 iPos=0;


	/* Check input parameters */
	if ((pbData!=NULL)&&(iLength>0))
	{	
		/* Create List Head */
		pCapaList=NEWSTRUCT(CAPABILITIESDEF);

		/* Error */
		if (pCapaList==NULL)
		{
			*pfError=TRUE;
			return NULL;
		}

		/* Store data */
		pCapaList->iDataLength=iLength;
		pCapaList->pbData=pbData;
		pCapaList->pList=NULL;

		/* Set end pointer */
		pbEnd=pbData+iLength;

		/* Get all capabilities */
		while ((pbData<pbEnd)&&(!*pfError))
		{
			iPos=0;
			iCapaLen=0;

			/* Get length of first Capability */
			if (ReadMBUInt32(&iCapaLen,pbData,(UINT32)(pbEnd-pbData-1),&iPos))
			{
				/* Create Capability */
				pEntry=NEWSTRUCT(CAPABILITY);
				pEntry->pNext=NULL;
				pEntry->pbEnd=pbData+iCapaLen+iPos;

				/* Data is first byte after lentgh and identifier */
				pbTemp=pbData+iPos;

				if (isWapShortInteger (&pbTemp,pbEnd))
				{
					pEntry->pbData=pbTemp;
				}
				else if (isWapTokenText (&pbTemp,pbEnd))
				{
					pEntry->pbData=pbTemp;
				}

				/* Add capability byte to struct */
				if (pbData+iPos<pbEnd)
				{
					pEntry->bCapability=*(pbData+iPos);
				}
				else
				{
					/* error */
					pEntry->bCapability=0xff;
				} 

				/* Add Capability to the end of the list */
				if (pLastEntry!=NULL)
				{
					pLastEntry->pNext=pEntry;
					pLastEntry=pEntry;
				}
				else
				{
					pCapaList->pList=pEntry;
					pLastEntry=pEntry;
				}

				/* Increase pbData with the number of bytes indicated 
				   by iCapaLen and the number of bytes for the MBUInt32. */
				pbData+=iPos+iCapaLen;
			}
			else
			{
				/* Error in encoding of mbuint */
				*pfError=TRUE;
			}
		}
	}

	return pCapaList;
}


UINT32 WSP_GetCapabilityWK (BYTE bCapability, pCAPABILITY pCapa, BOOL* pfError)
{
	pCAPABILITY pTemp=NULL;
	BOOL fFound=FALSE;
	UINT32 iResult=0;
	UINT32 iPos=0;

	/* Find correct capability */
	pTemp=pCapa;

	while ((pTemp!=NULL)&&(!fFound))
	{
		/* Check current capability */
		if ((pTemp->pbData!=NULL)&&(pTemp->pbData<pTemp->pbEnd))
		{
			if ( ((pTemp->bCapability)&0x7f) == bCapability )
			{
				/* Capability found */
				fFound=TRUE;
			}		
		}

		if (!fFound)
		{
			/* Get next capability */
			pTemp=pTemp->pNext;
		}
	}

	/* Get value */
	if (fFound)
	{
		switch (bCapability)
		{
		case CLIENT_SDU_SIZE:
		case SERVER_SDU_SIZE:
			/* uintvar */

			if (ReadMBUInt32(&iResult,pTemp->pbData,
				(UINT32)((pTemp->pbEnd)-(pTemp->pbData)),&iPos))
			{
				return iResult;
			}

			break;
		case METHOD_MOR:
		case PUSH_MOR:
			/* uint8 */
			if (pTemp->pbData<pTemp->pbEnd)
			{
				return (UINT32)(*(pTemp->pbData));
			}

			break;
		}
	}

	/* Not found */
	*pfError=TRUE;
	return 0;
}


void WSP_DeleteCapabilitiesDef (pCAPABILITIESDEF* ppCapabilities)
{
	pCAPABILITY pEntry=NULL;

	if (*ppCapabilities!=NULL)
	{
		/* Remove all capabilities */
		while ((*ppCapabilities)->pList!=NULL)
		{
			pEntry=(*ppCapabilities)->pList;

			(*ppCapabilities)->pList=(*ppCapabilities)->pList->pNext;

			/* Remove pEntry */
			DEALLOC(&pEntry);
		}

		/* Remove Data */
		DEALLOC(&((*ppCapabilities)->pbData));

		/* Remove Head struct */
		DEALLOC(ppCapabilities);
	}
}

