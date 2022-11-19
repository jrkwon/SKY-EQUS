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
trnscode.h
Created by KHN (AU-System Radio AB)
*********************/

#ifndef _TRNSCODE_H
#define _TRNSCODE_H


#include "cmmnrsrc.h"
#include "capiclnt.h"


void newmemcpy(WCHAR* pwchResult, BYTE* pbStr, size_t iLen);


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
BOOL Iana2Unicode_canConvert( INT16 iIANAcharset );


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
		UINT32 iIanaStrByteLen, UINT32 *piIanaStrResultBytelLen );


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
		WCHAR *pwchResultBuffer, UINT32 iResultBufferLen );




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
UINT8 Iana2Unicode_getNullTermByteLen( INT16 iIANAcharset );


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
															 fPtr_Iana2Unicode_getNullTermByteLen pNullLen );



/******************************************************************************/

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
UINT32 Unicode2UTF8_calcLen( WCHAR* pwchStr, UINT32 nbrOfWChars );


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
		BYTE* pbDst, UINT32 iDstLen, BYTE** ppbResultDstEnd );


#endif
/* _TRNSCODE_H */

