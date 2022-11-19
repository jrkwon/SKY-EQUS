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
/*********************
trnscode.c
Created by KHN (AU-System Radio AB)
*********************/
/*********
History
-------
000119 (KHN) Corrected the way the length of UCS2 strings are calculated.
               Before a BYTE* was cast into WCHAR* and since most
							 calculations were made on a part of a bigger BYTE* string
							 the reading of the WCHAR* resulted in error due to misalignment.

**********/


#include "trnscode.h"

#ifdef NO_GLOBAL_VARS
#include "userdata.h"
#else
static fPtr_Iana2Unicode_canConvert         g_pCanConvert = NULL;
static fPtr_Iana2Unicode_calcLen            g_pCalcLen = NULL;
static fPtr_Iana2Unicode_convert            g_pConvert = NULL;
static fPtr_Iana2Unicode_getNullTermByteLen g_pNullLen = NULL;
#endif




void newReverseMemcpy(WCHAR* pwchResult, BYTE* pbStr, size_t iLen)
{
	/* ASSERT: iLen must be even.
	*/

	/* Divide with two */
	iLen>>=1;

	/* Copy one WCHAR at a time */
	while (iLen--)
	{
		/* a b ---> (b<<8) | a */

		*pwchResult++=(WCHAR) ( ((WCHAR)(((WCHAR)(*(pbStr+1)))<<8)) | (WCHAR) (*pbStr) );
		pbStr+=2;
	}
}


void newmemcpy(WCHAR* pwchResult, BYTE* pbStr, size_t iLen)
{
	/* ASSERT: iLen must be even.
	*/

	/* Divide with two */
	iLen>>=1;

	/* Copy one WCHAR at a time */
	while (iLen--)
	{
		/* a b ---> (a<<8) | b */

		*pwchResult++=(WCHAR) ( ((WCHAR)(((WCHAR)(*pbStr))<<8)) | (WCHAR) (*(pbStr+1)) );
		pbStr+=2;
	}
}


UINT32 ucs2len(const BYTE* str)
{
  UINT32	len = 0;

	while (*str || *(str+1))
	{
		len++;
		str += 2;
	}

  return(len);
}


/* ================================================================ */
/*
File:    ConvertUTF.C
Author: Mark E. Davis
Copyright (C) 1994 Taligent, Inc. All rights reserved.

This code is copyrighted. Under the copyright laws, this code may not
be copied, in whole or part, without prior written consent of Taligent.

Taligent grants the right to use or reprint this code as long as this
ENTIRE copyright notice is reproduced in the code or reproduction.
The code is provided AS-IS, AND TALIGENT DISCLAIMS ALL WARRANTIES,
EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  IN
NO EVENT WILL TALIGENT BE LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING,
WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS
INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY
LOSS) ARISING OUT OF THE USE OR INABILITY TO USE THIS CODE, EVEN
IF TALIGENT HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
BECAUSE SOME STATES DO NOT ALLOW THE EXCLUSION OR LIMITATION OF
LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES, THE ABOVE
LIMITATION MAY NOT APPLY TO YOU.

RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the
government is subject to restrictions as set forth in subparagraph
(c)(l)(ii) of the Rights in Technical Data and Computer Software
clause at DFARS 252.227-7013 and FAR 52.227-19.

This code may be protected by one or more U.S. and International
Patents.

TRADEMARKS: Taligent and the Taligent Design Mark are registered
trademarks of Taligent, Inc.
*/
/* ================================================================ */
typedef UINT32                  UCS4;
typedef UINT16                  UCS2;
typedef UINT16                  UTF16;
typedef UINT8                   UTF8;
/* ================================================================ */
/*	Each of these routines converts the text between *sourceStart and
sourceEnd, putting the result into the buffer between *targetStart and
targetEnd. Note: the end pointers are *after* the last item: e.g.
*(sourceEnd - 1) is the last item.

	The return result indicates whether the conversion was successful,
and if not, whether the problem was in the source or target buffers.

	After the conversion, *sourceStart and *targetStart are both
updated to point to the end of last text successfully converted in
the respective buffers.
*/

typedef enum {
    ok,                 /* conversion successful */
    sourceExhausted,    /* partial character in source, but hit end */
    targetExhausted     /* insuff. room in target for conversion */
} ConversionResult;

/* ================================================================ */
const UCS4 kReplacementCharacter =  0x0000FFFDUL;
const UCS4 kMaximumUCS2 =           0x0000FFFFUL;
const UCS4 kMaximumUTF16 =          0x0010FFFFUL;
const UCS4 kMaximumUCS4 =           0x7FFFFFFFUL;

const INT32 halfShift           = 10;
const UCS4 halfBase             = 0x0010000UL;
const UCS4 halfMask             = 0x3FFUL;
const UCS4 kSurrogateHighStart  = 0xD800UL;
const UCS4 kSurrogateHighEnd    = 0xDBFFUL;
const UCS4 kSurrogateLowStart   = 0xDC00UL;
const UCS4 kSurrogateLowEnd     = 0xDFFFUL;
/* ================================================================ */

const UCS4 offsetsFromUTF8[6] =    {0x00000000UL, 0x00003080UL, 0x000E2080UL,
                                    0x03C82080UL, 0xFA082080UL, 0x82082080UL};
const UINT8 bytesFromUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5};

const UTF8 firstByteMark[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

/* ================================================================ */
/*    This code is similar in effect to making successive calls on the
mbtowc and wctomb routines in FSS-UTF. However, it is considerably
different in code:
* it is adapted to be consistent with UTF16,
* the interface converts a whole buffer to avoid function-call overhead
* constants have been gathered.
* loops & conditionals have been removed as much as possible for
efficiency, in favor of drop-through switch statements.
*/
/* ================================================================ */

ConversionResult    ConvertUTF8toUTF16 (
        UTF8** sourceStart, UTF8* sourceEnd,
        UTF16** targetStart, const UTF16* targetEnd)
{
    ConversionResult result = ok;
    UINT8 tmpCh;
    register UTF8* source = *sourceStart;
    register UTF16* target = *targetStart;

    while (source < sourceEnd) {
        /*register*/ UCS4 ch = 0;
        register unsigned short extraBytesToWrite = bytesFromUTF8[*source];
        if (source + extraBytesToWrite > sourceEnd) {
            result = sourceExhausted; break;
        };

        switch(extraBytesToWrite) {    /* note: code falls through cases! */
            case 5:
                tmpCh = *source++;
                ch += tmpCh;
                ch <<= 6;
            case 4:
                tmpCh = *source++;
                ch += tmpCh;
                ch <<= 6;
            case 3:
                tmpCh = *source++;
                ch += tmpCh;
                ch <<= 6;
            case 2:
                tmpCh = *source++;
                ch += tmpCh;
                ch <<= 6;
            case 1:
                tmpCh = *source++;
                ch += tmpCh;
                ch <<= 6;
            case 0:
                tmpCh = *source++;
                ch += tmpCh;
        };
        ch -= offsetsFromUTF8[extraBytesToWrite];

        if (target >= targetEnd) {
            result = targetExhausted; break;
        };
        if (ch <= kMaximumUCS2) {
            *target++ = (UTF16)ch;
        } else if (ch > kMaximumUTF16) {
            *target++ = (UTF16)kReplacementCharacter;
        } else {
            if (target + 1 >= targetEnd) {
                result = targetExhausted; break;
            };
            ch -= halfBase;
            *target++ = (UTF16)((ch >> halfShift) + kSurrogateHighStart);
            *target++ = (UTF16)((ch & halfMask) + kSurrogateLowStart);
        };
    };
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/************************************************
GetUT8CharLen
-------------
Return the number of bytes in the UTF8 char.
The only exception is for the 0 byte value (NULL)
which results in 0 as return value.
**************************************************/
UINT8 GetUT8CharLen(BYTE firstByte)
{
    BYTE        checkBit1 = 0x80;
    UINT8   nbrOfBytes = 0;

    if (firstByte == 0) {
        return 0; /* NULL found */
    }
    while ((firstByte & checkBit1) != 0) {
        nbrOfBytes++;
        firstByte = (BYTE)(firstByte << 1);
    }

    if (nbrOfBytes != 0) {
        return nbrOfBytes;
    }
    else {
        return 1; /* special case when only one byte (first bit is 0) */
    }
}


INT32 UTF8Len( BYTE* utf8Str, BOOL nullTerminated, UINT32 *byteLen )
/* if nullTerminated and byteLen != 0  then this value will eb used as a upper boundary */
/* returns nbr of characters, NOT bytes! */
/* the return value is -1 if the UTF8 string is corrupt (not properly ended) */
/* if nullTerminated then byteLen will be returned with the exact length in bytes.
        If not nullTerminated, the byteLen will not be changed */
{
    BYTE        checkUTF8byte = 0xC0;    /* to get the 2 most significant bits of the byte */
    BYTE        UTF8byte =      0x80; /* 10XXXXXX */
    UINT32    	strPos =        0;
    UINT8       i;
    UINT8       utf8CharLen;
    UINT32    	result = 0;
    BOOL        correctStr = TRUE;
    BOOL        foundEnd = FALSE;

    if (nullTerminated) {
        /* the string is nullterminated */
        while (correctStr && (!foundEnd))
        {
            utf8CharLen = GetUT8CharLen( *utf8Str );
            if (utf8CharLen == 0)
            {
                foundEnd = TRUE;
            }
            else
            {
                result++;

                strPos++;
                i = 2;
                while ((correctStr) && (i <= utf8CharLen))
                {
										if ((*byteLen != 0) && (strPos >= *byteLen))
										/* check so that we are not passing the upper limit (if any) */
                    {
                    	/* passed the upper limit in the middle of a utf8 char
                      => invalid string */
											correctStr = FALSE;
                    }
                    else
                    {
											/* make sure that there is not an imbedded NULL
	                    or other illegal character */
                      if ( (correctStr = ((BOOL)(((*(utf8Str + (i-1))) & checkUTF8byte) == UTF8byte))) )
                      {
		                    strPos++;
		                    i++;
                      }
                    }
                }
								if (correctStr)
                {
	                utf8Str += utf8CharLen;
                }
            }

            if ((*byteLen != 0) && (strPos >= *byteLen))
            {
                /* there is an upper bounds strLen that must be checked */
                correctStr = FALSE;
            }
        }
				/* *byteLen will contain the correct length
        or if something went wrong: the number of bytes read excluding
        the byte where the error was detected*/
        *byteLen = strPos;
        if (correctStr)
        {
            return result;
        }
        else
        {
            return -1;
        }
    }
    else {
        /* known length string */
        while (strPos < *byteLen)
        {
            utf8CharLen = GetUT8CharLen( *utf8Str );
            if (utf8CharLen == 0)
            {
                /* NULL found and in this mode it is accepted as a char */
                utf8CharLen = 1;
            }
            strPos += utf8CharLen;
            utf8Str += utf8CharLen;
	          result++;
        }
        if (strPos == *byteLen)
        {
            /* correct ending of utf8 string */
            return result;
        }
        else
        {
            /* length of characters not matching with length of string */
            return -1;
        }
    }
}

/* ================================================================ */

ConversionResult	ConvertUTF16toUTF8 (
		UTF16** sourceStart, const UTF16* sourceEnd,
		UTF8** targetStart, const UTF8* targetEnd)
{
	ConversionResult result = ok;
	register UTF16* source = *sourceStart;
	register UTF8* target = *targetStart;
	register UCS4 ch;
	register unsigned short bytesToWrite = 0;
	register const UCS4 byteMask = 0xBF;
	register const UCS4 byteMark = 0x80;
	register UCS4 ch2 = *source;

	while (source < sourceEnd)
  {
		ch = *source++;
		if (ch >= kSurrogateHighStart && ch <= kSurrogateHighEnd
				&& source < sourceEnd)
    {
			ch2 = *source;
			if (ch2 >= kSurrogateLowStart && ch2 <= kSurrogateLowEnd)
      {
				ch = ((ch - kSurrogateHighStart) << halfShift)
					+ (ch2 - kSurrogateLowStart) + halfBase;
				++source;
			};
		};
		if (ch < 0x80) {				bytesToWrite = 1;
		} else if (ch < 0x800) {		bytesToWrite = 2;
		} else if (ch < 0x10000) {		bytesToWrite = 3;
		} else if (ch < 0x200000) {		bytesToWrite = 4;
		} else if (ch < 0x4000000) {	bytesToWrite = 5;
		} else if (ch <= kMaximumUCS4){	bytesToWrite = 6;
		} else {						bytesToWrite = 2;
										ch = kReplacementCharacter;
		}; /* I wish there were a smart way to avoid this conditional */

		target += bytesToWrite;
		if (target > targetEnd)
    {
			target -= bytesToWrite; result = targetExhausted; break;
		};
		switch (bytesToWrite)
    {	/* note: code falls through cases! */
			case 6:	*--target = (UTF8) ((ch | byteMark) & byteMask); ch >>= 6;
			case 5:	*--target = (UTF8) ((ch | byteMark) & byteMask); ch >>= 6;
			case 4:	*--target = (UTF8) ((ch | byteMark) & byteMask); ch >>= 6;
			case 3:	*--target = (UTF8) ((ch | byteMark) & byteMask); ch >>= 6;
			case 2:	*--target = (UTF8) ((ch | byteMark) & byteMask); ch >>= 6;
			case 1:	*--target = (UTF8) (ch | firstByteMark[bytesToWrite]);
		};
		target += bytesToWrite;
	};
	*sourceStart = source;
	*targetStart = target;
	return result;
}

/* ================================================================ */













/*===========================================
Iana2Unicode_canConvert
---------------------------------------------
Returns TRUE if transcoding of the charset
is available, otherwise FALSE.

Parameters:
-----------
iIANAcharset: The MIBenum IANA code that corresponds
              to a specific character encoding.

=============================================
Returns: TRUE if transcoding is supported for the charset,
         otherwise FALSE.
============================================*/
BOOL Iana2Unicode_canConvert( INT16 iIANAcharset )
{
	switch (iIANAcharset)
	{
		case IANA_CHARSET_INVALID:
			return FALSE;

		case IANA_CHARSET_USASCII:
		case IANA_CHARSET_LATIN1:
		case IANA_CHARSET_UTF8:
		case IANA_CHARSET_UCS2:
			return TRUE;

		default:
			if ((g_pCanConvert != NULL) && ((g_pCanConvert)(iIANAcharset)) )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
	}
}


/*===========================================
Iana2Unicode_calcLen
---------------------------------------------
Returns the number of characters pbIanaStr contains.
The length of pbIanaStr is returned in the piIanaStrResultBytelLen
parameter.

Parameters:
-----------
pbIanaStr: the string to be transcoded.
iIANAcharset: The MIBenum IANA code that corresponds
              to a specific character encoding.
fIsNullTerminated: If TRUE, pbIanaStr's size is not known.
                   To avoid searching infinitely after a termination,
									 set iIanaStrByteLen to a nonzero value. This will
									 then be the upper limit of bytes which will be read
									 in search of the termination.
iIanaStrByteLen: If fIsNullTerminated == TRUE this is the upper size limit
                 of pbIanaStr.
								 If fIsNullTerminated == FALSE this is actual size
								 (number of bytes) of pbIanaStr.
piIanaStrResultBytelLen: This parameter will after call to the function
                         contain the size of pbIanaStr (in number of bytes).
												 This parameter will thus be the same as iIanaStrByteLen
												 if fIsNullTerminated == FALSE. This parameter should
												 contain a correct result even if the calcLen function
												 failed due to incorrect characters in pbIanaStr.
												 If the byte length could not be decided, 0 should be
												 returned.

=============================================
Returns:
>= 0: the number of characters in pbIanaStr
-1:   something went wrong (the piIanaStrResultBytelLen must still be valid)
============================================*/
INT32 Iana2Unicode_calcLen( BYTE *pbIanaStr, INT16 iIANAcharset, BOOL fIsNullTerminated,
		UINT32 iIanaStrByteLen, UINT32 *piIanaStrResultBytelLen )
{
	UINT32	len = 0;
	BOOL    fCompensateForBOM = 0;
	UINT32	iIanaStrWchLen = 0;

	if (iIANAcharset < 0)
	{
		return -1;
	}

	if (pbIanaStr == NULL)
	{
		*piIanaStrResultBytelLen = 0;
		return 0;
	}

	switch (iIANAcharset)
	{
		case IANA_CHARSET_INVALID:
			return -1;

		/************************/

		case IANA_CHARSET_USASCII:
		case IANA_CHARSET_LATIN1:
			if (! fIsNullTerminated)
			{
				*piIanaStrResultBytelLen = iIanaStrByteLen;
				return iIanaStrByteLen;
			}
			else
			{
				if (iIanaStrByteLen == 0)
				{
					/* no upper limit => strlen */
#ifdef FEATURE_WAP
					*piIanaStrResultBytelLen = strlen((char *) pbIanaStr );
#else
                    *piIanaStrResultBytelLen = strlen( pbIanaStr );
#endif //#ifdef FEATURE_WAP

					return *piIanaStrResultBytelLen;
				}
				else
				{
					while ((len < iIanaStrByteLen) && (*pbIanaStr != 0))
					{
						len++;
						pbIanaStr++;
					}
					*piIanaStrResultBytelLen = len;
					if (len <= iIanaStrByteLen)
					{
						return len;
					}
					else
					{
						return -1;
					}
				}
			}


		case IANA_CHARSET_UCS2:

			fCompensateForBOM = ((pbIanaStr[0] == 0xFE) && (pbIanaStr[1] == 0xFF)) || /* Normal BOM */
      										((pbIanaStr[0] == 0xFF) && (pbIanaStr[1] == 0xFE));		/* Reverse byte order BOM */
					/* The Byte Order Mark (BOM) character is a signal character 
						only and is not a part of the actual string. 
						Thus we must subtract one from the length to compensate for the BOM. */
					
			if (! fIsNullTerminated)
			{
				*piIanaStrResultBytelLen = iIanaStrByteLen;
				if ((iIanaStrByteLen % 2) == 0)
				{
					if (fCompensateForBOM)
					{	
						return (iIanaStrByteLen / 2) -1;	/* 2 == sizeof(WCHAR) */
					}
					else
					{
						return (iIanaStrByteLen / 2);	/* 2 == sizeof(WCHAR) */
					}
				}
				else
				{
					return -1;
				}
			}
			else
			{
				if (iIanaStrByteLen == 0)
				{
					/* no upper limit => strlen */
					*piIanaStrResultBytelLen = ucs2len( pbIanaStr );

					if (fCompensateForBOM)
					{	
						return *piIanaStrResultBytelLen -1;
					}
					else
					{
						return *piIanaStrResultBytelLen;
					}
				}
				else
				{
					iIanaStrWchLen = iIanaStrByteLen / 2;
					while ((len < iIanaStrWchLen) && (*pbIanaStr || *(pbIanaStr+1)))
					{
						len++;
						pbIanaStr += 2;
					}
					*piIanaStrResultBytelLen = len*2;
					if (len <= iIanaStrWchLen)
					{
						if (fCompensateForBOM)
						{	
							return len - 1;
						}
						else
						{
							return len;
						}
					}
					else
					{
						return -1;
					}

				}
			}

		case IANA_CHARSET_UTF8:
			*piIanaStrResultBytelLen = iIanaStrByteLen;
			return UTF8Len( pbIanaStr, fIsNullTerminated, piIanaStrResultBytelLen );

		/************************/

		default:
			if (g_pCalcLen)
			{
				return (g_pCalcLen)( pbIanaStr, iIANAcharset, fIsNullTerminated, iIanaStrByteLen, piIanaStrResultBytelLen );
			}
			else
			{
				return -1;
			}

		/************************/
	}
}


/*===========================================
Iana2Unicode_convert
---------------------------------------------
Converts pbIanaStr into a unicode string.

Parameters:
-----------
pbIanaStr: the string to be transcoded.
iIANAcharset: The MIBenum IANA code that corresponds
              to a specific character encoding.
iIanaStrByteLen: The actual size (number of bytes) of pbIanaStr.
pwchResultBuffer: A pointer to the buffer where the result will be placed.
iResultBufferLen: The length (in number of characters) of pwchResultBuffer. 

=============================================
Returns: 
TRUE: The conversion went ok.
FALSE: Something went wrong.
============================================*/
BOOL Iana2Unicode_convert( BYTE *pbIanaStr, INT16 iIANAcharset, UINT32 iIanaStrByteLen, 
		WCHAR *pwchResultBuffer, UINT32 iResultBufferLen )
{
	UINT32	i = 0;
	BYTE		*pbSrc = NULL;
	WCHAR		*pwchDst = NULL;
	BOOL    fCompensateForBOM = 0;
	BOOL    fReverseByteOrder = 0;


	if (pbIanaStr && iIanaStrByteLen)
	{
		if (iIANAcharset < 0)
		{
			return FALSE;
		}

		switch (iIANAcharset)
		{
			case IANA_CHARSET_INVALID:
				return FALSE;


			case IANA_CHARSET_USASCII:
			case IANA_CHARSET_LATIN1:
				if (iResultBufferLen >= iIanaStrByteLen)
				{
					for (i=1; i<=iIanaStrByteLen; i++ )
					{
						*pwchResultBuffer++ = (WCHAR) *pbIanaStr++;
					}
					return TRUE;
				}
				else
				{
					return FALSE;
				}


			case IANA_CHARSET_UCS2:
				fCompensateForBOM = ((pbIanaStr[0] == 0xFE) && (pbIanaStr[1] == 0xFF)) || /* Normal BOM */
    	  										((pbIanaStr[0] == 0xFF) && (pbIanaStr[1] == 0xFE));		/* Reverse byte order BOM */
					/* The Byte Order Mark (BOM) character is a signal character
						only and is not a part of the actual string. 
						Thus we must subtract one from the length to compensate for the BOM. */
				fReverseByteOrder = ( fCompensateForBOM && ((pbIanaStr[0] == 0xFF) && (pbIanaStr[1] == 0xFE)) );
				

				if ( iResultBufferLen >= (fCompensateForBOM?((iIanaStrByteLen/2) -1):(iIanaStrByteLen/2)) )
				{
					if (fCompensateForBOM)
					{	/* BOM character as first char in UCS2 */
						if (fReverseByteOrder)
						{	/* little endian UCS2 */
							newReverseMemcpy( pwchResultBuffer, pbIanaStr+2, iIanaStrByteLen-2 );
						}
						else
						{
							newmemcpy( pwchResultBuffer, pbIanaStr+2, iIanaStrByteLen-2 );
						}
					}
					else
					{ /* normal big endian UCS2 */
						newmemcpy( pwchResultBuffer, pbIanaStr, iIanaStrByteLen );
					}

					return TRUE;
				}
				else
				{
					return FALSE;
				}


			case IANA_CHARSET_UTF8:
				pbSrc = pbIanaStr;
				pwchDst = pwchResultBuffer;
				return (ok == ConvertUTF8toUTF16( (UTF8**)(&pbSrc), (UTF8*)(pbSrc + iIanaStrByteLen),
						(UTF16**)(&pwchDst), (UTF16*)(pwchDst + iResultBufferLen) ) );


			default:
				if (g_pConvert != NULL)
				{
					return (g_pConvert)(pbIanaStr, iIANAcharset, iIanaStrByteLen, pwchResultBuffer, iResultBufferLen);
				}
				else
				{
					return FALSE;
				}
		}
	}
	else
	{
		return TRUE;
	}
}


/*===========================================
Iana2Unicode_getNullTermByteLen
---------------------------------------------
Returns the length (in bytes) that a string terminating character
occupies in a string encoded with iIANAcharset.

Parameters:
-----------
iIANAcharset: The MIBenum IANA code that corresponds
              to a specific character encoding.

=============================================
Returns: 
0: Uknown IANA code.
>0: The number of bytes that a string terminating character occupies.
============================================*/
UINT8 Iana2Unicode_getNullTermByteLen( INT16 iIANAcharset )
{
	switch (iIANAcharset)
	{
		case IANA_CHARSET_INVALID:
			return 0;

		case IANA_CHARSET_USASCII:
		case IANA_CHARSET_LATIN1:
		case IANA_CHARSET_UTF8:
			return 1;
		case IANA_CHARSET_UCS2:
			return 2;

		default:
			if (g_pNullLen != NULL)
			{
				return (g_pNullLen)(iIANAcharset);
			}
			else
			{
				return 0;
			}
	}
}




/*===========================================
Iana2Unicode_setFuncPtrs
---------------------------------------------
Provides function pointers to external transcoding functions.
These external functions will be used if the internal
functions can not perform the transcoding.

Parameters:
-----------
pCanConvert: a pointer to a external version of Iana2Unicode_canConvert. 
pCalcLen: a pointer to a external version of Iana2Unicode_calcLen. 
pConvert: a pointer to a external version of Iana2Unicode_convert. 
pNullLen: a pointer to a external version of Iana2Unicode_getNullTermByteLen. 

=============================================
Returns: 
-
============================================*/
VOID Iana2Unicode_setFuncPtrs( fPtr_Iana2Unicode_canConvert pCanConvert,
															 fPtr_Iana2Unicode_calcLen pCalcLen,
															 fPtr_Iana2Unicode_convert pConvert,
															 fPtr_Iana2Unicode_getNullTermByteLen pNullLen )
{
	if (pCanConvert && pCalcLen && pConvert && pNullLen )
	{
		g_pCanConvert = pCanConvert;
		g_pCalcLen = pCalcLen;
		g_pConvert = pConvert;
		g_pNullLen = pNullLen;
	}
}


/*===========================================
Unicode2UTF8_calcLen
---------------------------------------------
Returns the number of bytes it takes to store pwchStr in UTF8 format.

Parameters:
-----------
pwchStr: The unicode string.
nbrOfWChars: The number of unicode characters that should be included
             in the calculation.

=============================================
Returns:
The number of bytes it takes to store pwchStr in UTF8 format.
============================================*/
UINT32 Unicode2UTF8_calcLen( WCHAR* pwchStr, UINT32 nbrOfWChars )
{
	UINT32 resultByteLen = 0;
  UINT32 iwchPos = 0;

	if ((pwchStr == NULL) || (nbrOfWChars == 0))
  {
  	return 0;
  }
	while ( iwchPos < nbrOfWChars )
  {
		if (pwchStr[ iwchPos ]< 0x80)
    {
    	resultByteLen += 1;
		}
    else if (pwchStr[iwchPos]< 0x800)
    {
    	resultByteLen += 2;
		}
    else
    {
    	resultByteLen += 3;
    }

    iwchPos++;
  }
  return resultByteLen;
}


/*===========================================
Unicode2UTF8_convert
---------------------------------------------
Encodes pwchSrc (unicode) in a UTF8 string format.

Parameters:
-----------
pwchSrc: the unciode string to be encoded.
iSrcLen: the length (in nbr of characters) of pwchSrc.
pbDst: the destination buffer.
iDstLen: the length (in bytes) of pbDst.
ppbResultDstEnd: if the function succeds, this parameter will hold a pointer
                 to the first byte after the UTF8 result string in the
                 destination buffer.

=============================================
Returns:
TRUE: The encoding went ok.
FALSE: Something went wrong.
============================================*/
BOOL Unicode2UTF8_convert( WCHAR* pwchSrc, UINT32 iSrcLen,
		BYTE* pbDst, UINT32 iDstLen, BYTE** ppbResultDstEnd )
{
	if ( ((iSrcLen != 0) && (pwchSrc == NULL)) ||
				((iDstLen != 0) && (pbDst == NULL)) )
  {
  	return FALSE;
  }
  if (ok == ConvertUTF16toUTF8( &pwchSrc, pwchSrc + iSrcLen,
                                 &pbDst, pbDst + iDstLen))
  {
  	*ppbResultDstEnd = pbDst;
    return TRUE;	
  }
  else
  {
  	return FALSE;
  }
}
