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
#ifndef __si_float_h__
#define __si_float_h__

#include "cmmnrsrc.h"

#ifdef HAS_FLOAT

BOOL String2Float (WCHAR *aStr, UINT8 *charsUsed, FLOAT32 *pf,
                   BOOL *isOverflow);
VOID Float2String (FLOAT32 aFloat, WCHAR *str);

#endif /*HAS_FLOAT*/

#endif
