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
#ifndef __si_int_h__
#define __si_int_h__

#include "cmmnrsrc.h"


BOOL String2Int(const WCHAR *aStr, UINT8 *charsUsed, INT32 *aInt, BOOL *isOverflow);
VOID Int2String(const INT32 aInt, WCHAR *str);


#endif
