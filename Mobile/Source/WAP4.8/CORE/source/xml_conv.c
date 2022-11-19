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
	FILE: XML_Conv.c
  
	Source file for converter functions used by the application specific
	functions of the WBXML-decoder.

	Rev history:
		991202	JPR		Created
		991209	JPR		Functions implemented
		000104	JPR		Function Convert_XmlSpaceToUINT8 added
		000419	JPR		Function return method changed to support overflow

==========================================================================*/

#include "xml_conv.h"
#include "waectype.h"
#include "wbxmldef.h"

void Convert_RemoveAllWhiteSpace (WCHAR** ppwchString)
{
	/* ASSERT: String not NULL
	*/

	WCHAR* pwchRead=*ppwchString;
	WCHAR* pwchWrite=*ppwchString;
	WCHAR* pwchLastChar=*ppwchString;

	while (*pwchRead!=0)
	{
		if (isXMLWhiteSpace(*pwchRead))
		{
			/* Do not step write pos */
			pwchRead++;
		}
		else
		{
			/* Not white space - write */
			*pwchWrite++=*pwchRead++;
			pwchLastChar=pwchWrite;
		}
	}

	/* Set new termination character */
	*pwchLastChar=0;
}


BOOL Convert_ConvertNumber (WCHAR *pwchNumber, BOOL *pfIsPercent, 
							UINT32* piResult, BOOL* pfNeg) 
{
	UINT32 iCount=0;
	WCHAR *pwchTemp=pwchNumber;
	BOOL fDigitFound=FALSE;

	/* Reset results */
	*piResult=0;
	*pfIsPercent=FALSE;

	if (pwchTemp!=NULL) 
	{
		/* Trim text (remove all white spaces) */
		Convert_RemoveAllWhiteSpace (&pwchTemp);

		/* Check if first character is '-' */
		if (*pwchTemp==(WC('-')))
		{
			*pfNeg=TRUE;
			pwchTemp++;
		}

		while ((pwchTemp[iCount]!=0) && (!(*pfIsPercent)))
		{
			fDigitFound=TRUE;

			/* Check if between '0' and '9' and percent is not set */
			if ( (pwchTemp[iCount] < 0x100) && (wae_isdigit(pwchTemp[iCount])) && 
				 (*pfIsPercent==FALSE) )
			{
				UINT8 iDigit=(UINT8)(pwchTemp[iCount]-(WC('0')));

				/* Check if result will overflow */
				if ((*piResult>(UINT32)(429496729UL))|| ( (*piResult==(UINT32)(429496729UL))&&(iDigit>5)) )
				{
					/* return max */
                    *piResult = UINT32_MAX;
					return TRUE;
				}

				*piResult=(UINT32)((*piResult*10)+iDigit);
			}
			else if (pwchTemp[iCount]==(WC('%')))
			{
				/* Per cent sign found */
				*pfIsPercent=TRUE;
			}
			else 
			{
				/* Not valid character */
				return FALSE;
			}
			iCount++;
		}
	}

	return fDigitFound;
}


/*========================================================================
	Convert_StringToUINT8
==========================================================================*/
BOOL Convert_StringToUINT8 (WCHAR** ppwchString, UINT8* piResult)
{
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;
	BOOL fNeg=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (!fPercent)
			{
				if (fNeg)
				{
					*piResult=0;
				}
				else if ( (iTemp < UINT8_MAX) )
				{
					*piResult=(UINT8)(iTemp);
				}
				else
				{
					*piResult=UINT8_MAX;
				}

				fCalcResult=TRUE;
			}
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}


/*========================================================================
	Convert_StringToUINT16
==========================================================================*/
BOOL Convert_StringToUINT16 (WCHAR** ppwchString, UINT16* piResult)
{
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;
	BOOL fNeg=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (!fPercent)
			{
				if (fNeg)
				{
					*piResult=0;
				}
				else if ( (iTemp < UINT16_MAX) )
				{
					*piResult=(UINT16)(iTemp);
				}
				else
				{
					*piResult=UINT16_MAX;
				}

				fCalcResult=TRUE;
			}
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}


/*========================================================================
	Convert_StringToUINT32
==========================================================================*/
BOOL Convert_StringToUINT32 (WCHAR** ppwchString, UINT32* piResult)
{
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;
	BOOL fNeg=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (!fPercent)
			{
				if (fNeg)
				{
					*piResult=0;
				}
				else
				{
					*piResult=iTemp;
					fCalcResult=TRUE;
				}

				fCalcResult=TRUE;
			}
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}


/*========================================================================
	Convert_StringToINT8
==========================================================================*/
BOOL Convert_StringToINT8 (WCHAR** ppwchString, INT8* piResult)
{
	BOOL fNeg=FALSE;
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (!fPercent)
			{
				if ( (iTemp < INT8_MAX) )
				{
					*piResult=(INT8)(iTemp);
				}
				else
				{
					*piResult=INT8_MAX;
				}

				if (fNeg)
				{
					if (iTemp!=INT8_MAX)
					{
						*piResult = (INT8)(-(*piResult));
					}
					else
					{
						*piResult = INT8_MIN;
					}
				}

				fCalcResult=TRUE;
			}
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}


/*========================================================================
	Convert_StringToINT16
==========================================================================*/
BOOL Convert_StringToINT16 (WCHAR** ppwchString, INT16* piResult)
{
	BOOL fNeg=FALSE;
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (!fPercent)
			{
				if ( (iTemp < INT16_MAX) )
				{
					*piResult=(INT16)(iTemp);
				}
				else
				{
					*piResult=INT16_MAX;
				}

				if (fNeg)
				{
					if (iTemp!=INT16_MAX)
					{
						*piResult = (INT16)(-(*piResult));
					}
					else
					{
						*piResult = INT16_MIN;
					}
				}

				fCalcResult=TRUE;
			}
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}


/*========================================================================
	Convert_StringToINT32
==========================================================================*/
BOOL Convert_StringToINT32 (WCHAR** ppwchString, INT32* piResult)
{
	BOOL fNeg=FALSE;
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (!fPercent)
			{
				if ( (iTemp < INT32_MAX) )
				{
					*piResult=(INT32)(iTemp);
				}
				else
				{
					*piResult=INT32_MAX;
				}

				if (fNeg)
				{
					if (iTemp!=INT32_MAX)
					{
						*piResult = (INT32)(-(*piResult));
					}
					else
					{
						*piResult = INT32_MIN;
					}
				}

				fCalcResult=TRUE;
			}
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}



/*========================================================================
	Convert_StringToUINT8percent
==========================================================================*/
BOOL Convert_StringToUINT8percent (WCHAR** ppwchString, UINT8* piResult,
								   BOOL* pfPercent)
{
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;
	BOOL fNeg=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (fNeg)
			{
				*piResult=0;
			}
			else if ( (iTemp < UINT8_MAX) )
			{
				*piResult=(UINT8)(iTemp);
			}
			else
			{
				*piResult=UINT8_MAX;
			}

			*pfPercent=fPercent;
			fCalcResult=TRUE;
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}


/*========================================================================
	Convert_StringToUINT16percent
==========================================================================*/
BOOL Convert_StringToUINT16percent (WCHAR** ppwchString, UINT16* piResult,
									BOOL* pfPercent)
{
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;
	BOOL fNeg=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (fNeg)
			{
				*piResult=0;
			}
			else if ( (iTemp < UINT16_MAX) )
			{
				*piResult=(UINT16)(iTemp);
			}
			else
			{
				*piResult=UINT16_MAX;
			}

			*pfPercent=fPercent;
			fCalcResult=TRUE;
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}


/*========================================================================
	Convert_StringToUINT32percent
==========================================================================*/
BOOL Convert_StringToUINT32percent (WCHAR** ppwchString, UINT32* piResult,
									BOOL* pfPercent)
{
	UINT32 iTemp=0;
	BOOL fPercent=FALSE;
	BOOL fCalcResult=FALSE;
	BOOL fNeg=FALSE;

	if ((ppwchString!=NULL)&&(*ppwchString!=NULL))
	{
		/* Calculate number */
		if (Convert_ConvertNumber (*ppwchString,&fPercent,&iTemp,&fNeg))
		{
			if (fNeg)
			{
				*piResult=0;
			}

			*piResult=(UINT32)(iTemp);
			*pfPercent=fPercent;
			fCalcResult=TRUE;
		}

		DEALLOC(ppwchString);
	}

	return fCalcResult;
}


/*========================================================================
	Convert_StringToBool
==========================================================================*/
BOOL Convert_StringToBool (WCHAR** ppwchString, BOOL* pfResult)
{
	const WCHAR pwchConst_true[]={'t','r','u','e',0};
	const WCHAR pwchConst_false[]={'f','a','l','s','e',0};
	BOOL fConvRes=FALSE;
	
	if (*ppwchString!=NULL) 
	{
		if (COMPARESTRING(*ppwchString,pwchConst_true)==0) 
		{
			*pfResult=TRUE;
			fConvRes=TRUE;
		}
		else if (COMPARESTRING(*ppwchString,pwchConst_false)==0) 
		{
			*pfResult=FALSE;
			fConvRes=TRUE;
		}
		DEALLOC(ppwchString);
	}
	
	return fConvRes;
}


/*========================================================================
	Convert_XmlSpaceToUINT8
==========================================================================*/
BOOL Convert_XmlSpaceToUINT8 (WCHAR** ppwchString, UINT8* piResult)
{
	const WCHAR pwchConst_default[]={'d','e','f','a','u','l','t',0};
	const WCHAR pwchConst_preserve[]={'p','r','e','s','e','r','v','e',0};
	BOOL fConvRes=FALSE;
	
	if (*ppwchString!=NULL) 
	{
		if (COMPARESTRING(*ppwchString,pwchConst_default)==0) 
		{
			*piResult=XML_SPACE_DEFAULT;
			fConvRes=TRUE;
		}
		else if (COMPARESTRING(*ppwchString,pwchConst_preserve)==0) 
		{
			*piResult=XML_SPACE_PRESERVE;
			fConvRes=TRUE;
		}
		DEALLOC(ppwchString);
	}
	
	return fConvRes;
}
