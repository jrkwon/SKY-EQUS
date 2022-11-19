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

#include "charset.h"
#include "capiclnt.h"
#include "HCodeCnvt.h" /* KSC5601 converter */


#define IANA_CHARSET_KSC5601 36


BOOL CHARSET_canConvert(INT16 charset)
{
	if (charset == IANA_CHARSET_KSC5601)
		return TRUE;
	else
		return FALSE;
}


INT32 CHARSET_calcLen(BYTE *str, INT16 charset, BOOL isNullTerminated, UINT32 readBytes, UINT32 *strByteLen)
{
	if (charset == IANA_CHARSET_KSC5601)
	{
		return KSC2Unicode_calcLen(str, isNullTerminated, readBytes, strByteLen);
	}
	else
	{
		*strByteLen = 0;
		return 0;
	}
}


BOOL CHARSET_convert(BYTE *str, INT16 charset, UINT32 strByteLen, WCHAR *resultBuffer, UINT32 resultBufferLen)
{
//	UINT32 x1 = resultBufferLen; /* dummy, to consume a compiler warning */
//	UINT32 x2 = strByteLen; /* dummy, to consume a compiler warning */

	if (charset == IANA_CHARSET_KSC5601)
	{
		KSC2UniString((unsigned char *)str, resultBuffer);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


UINT8 CHARSET_nullLen(INT16 charset)
{
	if (charset == IANA_CHARSET_KSC5601)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


VOID initialiseCharsetPlugin(VOID)
{
	CLNTc_setTranscoders (CHARSET_canConvert, CHARSET_calcLen, CHARSET_convert, CHARSET_nullLen);
}
