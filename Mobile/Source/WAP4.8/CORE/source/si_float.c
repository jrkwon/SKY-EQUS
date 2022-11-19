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
/*********
History
-------
000802 (KHN) Changed to a safer sprintf calling style.
001006 (NKE) Made rounding of float 2 string default. 
             Can still get old style comversion by
             defining WMLS_CORRECT_FLOAT2STRING. (KHN) 
**********/
#include "si_float.h"

#include "si_int.h"
#include "si_misc.h"

#include "ansilibs.h"

#ifdef HAS_FLOAT

#define FLT_MAX         3.402823466e+38         /* Max value */
#define FLT_MIN         1.175494351e-38         /* Min positive value */

BOOL
String2Float (WCHAR *aStr, UINT8 *charsUsed, FLOAT32 *pf, BOOL *isOverflow)
{
  UINT16 len = STRINGLENGTH (aStr);
  BYTE   *buf = NEWARRAY (BYTE, len + 1);
  INT16  tmpi, i, j = 0;
  double d;

  *isOverflow = FALSE;
  if (buf == NULL)
    return FALSE;

  /* While we translate the WCHAR string to a byte string, we also perform
   * a syntax check. */

  /* Skip blanks */
  for (i = 0; i < len; i++) {
    if (!IsWhitespaceChar (aStr[i]))
      break;
  }

  /* Check for '+' or '-' */
  if ((i < len) && ((aStr[i] == WC('+')) || (aStr[i] == WC('-'))))
    buf[j++] = (BYTE)aStr[i++];

  /* Read integer part */
  tmpi = i;
  while ((i < len) && (aStr[i] >= WC('0')) && (aStr[i] <= WC('9'))) {
    buf[j++] = (BYTE)aStr[i++];
  }    

  /* Check for decimal point */
  if ((i < len) && (aStr[i] == WC('.'))) {
    buf[j++] = (BYTE)aStr[i++];
    tmpi = i;
    while ((i < len) && (aStr[i] >= WC('0')) && (aStr[i] <= WC('9'))) {
      buf[j++] = (BYTE)aStr[i++];
    }
    if (tmpi == i) {
      /* Decimal point was not followed by a number */
      DEALLOC (&buf);
      return FALSE;
    }
  }
  else if (tmpi == i) {
    /* No integer part, and no decimal point */
    DEALLOC (&buf);
    return FALSE;
  }

  /* Check for exponent part */
  if ((i < len) && ((aStr[i] == WC('e')) || (aStr[i] == WC('E')))) {
    buf[j++] = (BYTE)aStr[i++];
    if ((i < len) && ((aStr[i] == WC('+')) || (aStr[i] == WC('-'))))
      buf[j++] = (BYTE)aStr[i++];
    tmpi = i;
    while ((i < len) && (aStr[i] >= WC('0')) && (aStr[i] <= WC('9'))) {
      buf[j++] = (BYTE)aStr[i++];
    }
    if (tmpi == i) {
      /* Exponent part did not have any digits */
      DEALLOC (&buf);
      return FALSE;
    }
  }
  buf[j] = '\0';

  /* "buf" now contains a null-terminated string of characters
   * representing a floating-point literal, with correct syntax.
   * The number of characters consumed from the original string
   * is held in "i".
   */
  *charsUsed = (UINT8)i;

  /* We use strtod to do the actual conversion to a float value,
   * but we have to be careful to detect overflow correctly, both
   * on platforms that really have a type "double" and on those
   * where "double" is really the same as "float".
   */
  errno = 0;
#ifdef FEATURE_WAP
  d = strtod ((char *)buf, NULL);
#else
  d = strtod (buf, NULL);
#endif //#ifdef FEATURE_WAP
  DEALLOC (&buf);

  if ((d > FLT_MAX) || (d < -FLT_MAX) ||
      ((d == HUGE_VAL) && (errno == ERANGE)) ||
      ((d == -HUGE_VAL) && (errno == ERANGE))) {
    *isOverflow = TRUE;
    return FALSE;
  }
  if ((d < FLT_MIN) && (d > -FLT_MIN)) {
    *pf = (FLOAT32)0;
  }
  else
    *pf = (FLOAT32)d;

  return TRUE;
}

VOID
Float2String (FLOAT32 aFloat, WCHAR *str)
{
  BYTE  tmp[50];
  INT16 i, len;

#ifdef WMLS_CORRECT_FLOAT2STRING
  len = sprintf (tmp, "%.9g", (double)(aFloat) );
#else
#ifdef FEATURE_WAP
  len = sprintf ((char *)tmp, "%g", (double)(aFloat) );
#else
  len = sprintf (tmp, "%g", (double)(aFloat) );
#endif //#ifdef FEATURE_WAP
#endif
  for (i = 0; i < len; i++) {
    str[i] = (WCHAR)tmp[i];
  }
  str[len] = 0;
}

/*HAS_FLOAT*/
#endif
