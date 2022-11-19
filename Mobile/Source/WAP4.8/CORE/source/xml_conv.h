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
	FILE: XML_Conv.h
  
	Interface file for converter functoins used by the application specific
	functions of the WBXML-decoder.

	Rev history:
		991202	JPR		Created
		000104	JPR		Function Convert_XmlSpaceToUINT8 added
		000419	JPR		Function return method changed to support overflow

==========================================================================*/

#ifndef _XMLCONVH
#define _XMLCONVH

#include "cmmnrsrc.h"

/*========================================================================
	Convert_StringToUINT8
	Convert_StringToUINT16
	Convert_StringToUINT32
==========================================================================
	The following functions converts a WCHAR-string of characters between 
	'0' and '9'	to a number. The number is returned as an UINT8, UINT16
	or an UINT32. If the value is larger than the datatype can hold, the
	maximum value for the datatype is returned (e.g., UINT16_MAX). If the
	value is negative 0 is returned.

	The string is deleted after the operation.

	Input: WCHAR**, Pointer to the result ( UINT8* | UINT16* | UINT32* )
	Output: TRUE if successful, FALSE otherwise
==========================================================================*/
BOOL Convert_StringToUINT8 (WCHAR** ppwchString, UINT8* piResult);
BOOL Convert_StringToUINT16 (WCHAR** ppwchString, UINT16* piResult);
BOOL Convert_StringToUINT32 (WCHAR** ppwchString, UINT32* piResult);


/*========================================================================
	Convert_StringToINT8
	Convert_StringToINT16
	Convert_StringToINT32
==========================================================================
	The following functions converts a WCHAR-string of characters between 
	'0' and '9'	to a number. If the string begins with the character '-',
	the value is negative. The result is returned as an INT8, INT16, or 
	an INT32. If the value is larger than the datatype can hold, the
	maximum value for the datatype is returned (e.g., INT16_MAX). If the
	value is negative the minimum value is returned (e.g., INT16_MIN)

	The string is deleted after the operation.

	Input: WCHAR**, Pointer to the result ( INT8* | INT16* | INT32* )
	Output: TRUE if successful, FALSE otherwise
==========================================================================*/
BOOL Convert_StringToINT8 (WCHAR** ppwchString, INT8* piResult);
BOOL Convert_StringToINT16 (WCHAR** ppwchString, INT16* piResult);
BOOL Convert_StringToINT32 (WCHAR** ppwchString, INT32* piResult);


/*========================================================================
	Convert_StringToUINT8percent
	Convert_StringToUINT16percent
	Convert_StringToUINT32percent
==========================================================================
	The following functions converts a WCHAR-string of characters between 
	'0' and '9'	to a number. If the string ends with the character '%',
	the value is a percentage value. In that case, the value is checked to
	be between 0 and 100, and the pfPercent-flag is set.
	
	The result is returned as an UINT8, UINT16, or an UINT32. If the value 
	is larger than the datatype can hold, the maximum value for the 
	datatype is returned (e.g., UINT16_MAX). If the	value is negative 0 is 
	returned.

	The string is deleted after the operation.

	Input: WCHAR**, Pointer to the result ( UINT8* | UINT16* | UINT32* ), and
		   pointer to percentage flag (TRUE/FALSE)
	Output: TRUE if successful, FALSE otherwise
==========================================================================*/
BOOL Convert_StringToUINT8percent (WCHAR** ppwchString, UINT8* piResult,
								   BOOL* pfPercent);
BOOL Convert_StringToUINT16percent (WCHAR** ppwchString, UINT16* piResult,
									BOOL* pfPercent);
BOOL Convert_StringToUINT32percent (WCHAR** ppwchString, UINT32* piResult,
									BOOL* pfPercent);



/*========================================================================
	Convert_StringToBool
==========================================================================
	The following functions converts a WCHAR-string to a boolean. If the
	value of the string is 'true' the fResult is set to TRUE. If the value
	of the string is 'false', the the fResult is set to TRUE. Otherwise,
	the operation is considered to have failed.

	The string is deleted after the operation.

	Input: WCHAR**, Pointer to the result ( BOOL* )
	Output: TRUE if successful, FALSE otherwise
==========================================================================*/
BOOL Convert_StringToBool (WCHAR** ppwchString, BOOL* pfResult);



/*========================================================================
	Convert_XmlSpaceToUINT8
==========================================================================
	The following functions converts a WCHAR-string to a predefined UINT8. 
	If the value of the string is 'preserve' the iResult is set to 
	XML_SPACE_PRESERVE. If the value of the string is 'default', the the 
	fResult is set to XML_SPACE_DEFAULT. Otherwise, the operation is 
	considered to have failed.

	The string is deleted after the operation.

	Input: WCHAR**, Pointer to the result ( UINT8* )
	Output: TRUE if successful, FALSE otherwise
==========================================================================*/
BOOL Convert_XmlSpaceToUINT8 (WCHAR** ppwchString, UINT8* piResult);

#endif
