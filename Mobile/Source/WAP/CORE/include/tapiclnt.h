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
#ifndef TAPICLNT_H
#define TAPICLNT_H

#include "tapicmmn.h"


/*
    UINT8   type,
        0 = integer
        1 = float
        2 = string
        3 = boolean
        4 = invalid

    INT32   intVal, (32 bit integer)
        If the type indicates integer, the value is stored here.

    FLOAT32 floatVal, (32 bit floating point following the ANSI/IEEE Standard 754)
        If the type indicates float, the value is stored here.

    INT16   stringIANAcharset,
        If the type is string then this value is the MIBenum IANA code 
        for the character encoding used in the string (stringVal).
        For instance 1000 == UCS-2 (Unicode) and 36 == KSC5610.
        The character set must be supported by the Generic WAP Client
        (see INIT_ACCEPTCHARSET).

    UINT32  stringLengthInBytes,
        If the type is string then this is the length (in number of bytes) 
        of the string stored in stringVal. The length should not 
        count a terminating NULL if there is one.

    CHAR    *stringVal,
        If the type is string:
            then this is the string value encoded in the character encoding indicated by
            stringIANAcharset and with the length of stringLengthInBytes.
            It is not necessary to end the string with a NULL character.
        else
            the stringVal must be a NULL pointer.

    BOOL    boolVal,
        If the type indicates boolean, the value is stored here.
        0 = FALSE
        1 = TRUE
*/
typedef struct WMLSvar
{
    UINT8   type;
    INT32   intVal;
    FLOAT32 floatVal;
    INT16   stringIANAcharset;
    UINT32  stringLengthInBytes;
    CHAR    *stringVal;
    BOOL    boolVal;
} WMLSvar;


#endif
