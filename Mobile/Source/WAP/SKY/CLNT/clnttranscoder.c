/*===========================================================================

							CLIENT TRANSCODER SOURCE FILE

DESCRIPTION

 Copyright (c) 1999 by SK teletech. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/03/99   swjung  Define character set conversion functions 
===========================================================================*/

#include "clnt.h"
#include "tapicmmn.h"
#include "HCodeCnvt.h"

BOOL Ksc2Unicode_canConvert(INT16 charset) {

	if (charset == KS_C_5601_1987)
		return TRUE;
	else 
		return FALSE;
}

INT32 Ksc2Unicode_calcLen(BYTE *str, INT16 charset, BOOL isNullTerminated, UINT32 readBytes, UINT32 *strByteLen) {

	if (charset == KS_C_5601_1987) {
		return KSC2Unicode_calcLen(str, isNullTerminated, readBytes, strByteLen);
	}
	else 
		return FALSE;
}

BOOL Ksc2Unicode_convert(BYTE *str, INT16 charset, UINT32 strByteLen, WCHAR *resultBuffer, UINT32 resultBufferLen) {

	if ((charset == KS_C_5601_1987) && KSC2UniString((unsigned char *)str, resultBuffer)) 
		return TRUE;
	else 
		return FALSE;
}

UINT8 Ksc2Unicode_getNullTermByteLen(INT16 charset) {

	if (charset == KS_C_5601_1987) {
		return KS_C_5601_1987_NULL_LENGTH;
	}
	else 
		return UNKNOWN_CHAR_SET;
}
