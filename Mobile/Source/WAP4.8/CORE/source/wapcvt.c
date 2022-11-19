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
/*
 * wapcvt.c
 *
 * Created by Anders Edenbrandt, Mon Nov 20 15:27:30 2000.
 *
 * Revision history:
 *
 */
/************************************************************
 * Functions to convert between an external and an internal
 * representation.
 *
 * All decoding and encoding functions take a pointer to a
 * "conversion object" (see type definition below) as
 * the first argument. Usually, the second argument is a pointer
 * to an element of the type to be converted.
 *
 * There are five different conversion operations:
 *   WAP_CVT_DECODE         convert from external to internal representation
 *   WAP_CVT_DECODE_STATIC  as above, but don't allocate any memory, just
 *                            set pointers into the given buffer
 *   WAP_CVT_ENCODE         convert from internal to external representation
 *   WAP_CVT_ENCODE_SIZE    calculate the buffer size required to do an
 *                            encoding operation
 *   WAP_CVT_FREE           deallocate any vectors that were allocated as
 *                            part of a previous decoding operation
 *
 * Each function returns TRUE on success and FALSE on error.
 *
 ************************************************************/
#include "confvars.h"
#include "ansilibs.h"

#include "wapcvt.h"
#include "cmmnrsrc.h"

/************************************************************
 * Macro definitions
 ************************************************************/

#define WAP_CVT_GET_BYTE(obj)    ((obj)->data[(obj)->pos++])
#define WAP_CVT_PUT_BYTE(obj, b) ((obj)->data[(obj)->pos++] = (b))

/*
 * Return the number of bytes remaining in the buffer.
 */
#define WAP_CVT_REM_LENGTH(str) ((str)->length - (str)->pos)



/************************************************************
 * Conversion routines
 ************************************************************/

/*
 * Prepare a conversion object for a specific operation,
 * using the indicated buffer of given length.
 */
void
wap_cvt_init (wap_cvt_t *obj, UINT8 operation, BYTE *buf, UINT32 length)
{
  obj->operation = operation;
  obj->data = buf;
  obj->length = length;
  obj->pos = 0;
}

/*
 * Convert a 1-byte unsigned integer.
 */
INT16
wap_cvt_uint8 (wap_cvt_t *obj, UINT8 *p)
{
  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    if (WAP_CVT_REM_LENGTH (obj) < 1)
      return FALSE;
    *p = WAP_CVT_GET_BYTE (obj);
    break;

  case WAP_CVT_ENCODE:
    if (WAP_CVT_REM_LENGTH (obj) < 1)
      return FALSE;
    WAP_CVT_PUT_BYTE (obj, *p);
    break;

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += 1;
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert a 1-byte signed integer in two's complement format.
 */
INT16
wap_cvt_int8 (wap_cvt_t *obj, INT8 *p)
{
  BYTE a;

  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    if (WAP_CVT_REM_LENGTH (obj) < 1)
      return FALSE;
    a = WAP_CVT_GET_BYTE (obj);
    if (a & 0x80) {
      /* We have a negative number */
      a = (UINT8)(a & 0x7f);
      if (a == 0)
        return FALSE; /* Error, because this might overflow */
      a = (BYTE)((0x7f - a) + 1);
      *p = -(INT8)a;
    }
    else {
      *p = (INT8)a;
    }
    break;

  case WAP_CVT_ENCODE:
    if (WAP_CVT_REM_LENGTH (obj) < 1)
      return FALSE;
    if (*p < 0) {
      a = (BYTE)((0x7f + *p) + 1);
      a |= 0x80;
    }
    else {
      a = (UINT8)*p;
    }
    WAP_CVT_PUT_BYTE (obj, a);
    break;

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += 1;
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert a 2-byte unsigned integer in big-endian format.
 */
INT16
wap_cvt_uint16 (wap_cvt_t *obj, UINT16 *p)
{
  BYTE a, b;

  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    if (WAP_CVT_REM_LENGTH (obj) < 2)
      return FALSE;
    a = WAP_CVT_GET_BYTE (obj);
    b = WAP_CVT_GET_BYTE (obj);
    *p = (UINT16)(((UINT16)a << 8) | (UINT16)b);
    break;

  case WAP_CVT_ENCODE:
    if (WAP_CVT_REM_LENGTH (obj) < 2)
      return FALSE;
    WAP_CVT_PUT_BYTE (obj, (UINT8)((*p >> 8) & 0xff));
    WAP_CVT_PUT_BYTE (obj, (UINT8)(*p & 0xff));
    break;

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += 2;
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert a 2-byte signed integer in big-endian, two's complement format.
 */
INT16
wap_cvt_int16 (wap_cvt_t *obj, INT16 *p)
{
  BYTE   a, b;
  UINT16 n;

  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    if (WAP_CVT_REM_LENGTH (obj) < 2)
      return FALSE;
    a = WAP_CVT_GET_BYTE (obj);
    b = WAP_CVT_GET_BYTE (obj);
    n = (UINT16)(((UINT16)a << 8) | (UINT16)b);
    if (n & 0x8000) {
      /* We have a negative number */
      n = (UINT16)(n & 0x7fff);
      if (n == 0)
        return FALSE; /* Error, because this might overflow */
      n = (UINT16)((0x7fff - n) + 1);
      *p = -(INT16)n;
    }
    else {
      *p = (INT16)n;
    }
    break;

  case WAP_CVT_ENCODE:
    if (WAP_CVT_REM_LENGTH (obj) < 2)
      return FALSE;
    if (*p < 0) {
      n = (UINT16)((0x7fff + *p) + 1);
      n |= 0x8000;
    }
    else {
      n = *p;
    }
    WAP_CVT_PUT_BYTE (obj, (UINT8)((n >> 8) & 0xff));
    WAP_CVT_PUT_BYTE (obj, (UINT8)(n & 0xff));
    break;

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += 2;
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert a 4-byte unsigned integer in big-endian format.
 */
INT16
wap_cvt_uint32 (wap_cvt_t *obj, UINT32 *p)
{
  BYTE a, b, c, d;

  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    if (WAP_CVT_REM_LENGTH (obj) < 4)
      return FALSE;
    a = WAP_CVT_GET_BYTE (obj);
    b = WAP_CVT_GET_BYTE (obj);
    c = WAP_CVT_GET_BYTE (obj);
    d = WAP_CVT_GET_BYTE (obj);
    *p = ((UINT32)a << 24) | ((UINT32)b << 16) | ((UINT32)c << 8) | (UINT32)d;
    break;

  case WAP_CVT_ENCODE:
    if (WAP_CVT_REM_LENGTH (obj) < 4)
      return FALSE;
    WAP_CVT_PUT_BYTE (obj, (UINT8)((*p >> 24) & 0xff));
    WAP_CVT_PUT_BYTE (obj, (UINT8)((*p >> 16) & 0xff));
    WAP_CVT_PUT_BYTE (obj, (UINT8)((*p >> 8) & 0xff));
    WAP_CVT_PUT_BYTE (obj, (UINT8)(*p & 0xff));
    break;

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += 4;
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert a 4-byte signed integer in big-endian, two's complement format.
 */
INT16
wap_cvt_int32 (wap_cvt_t *obj, INT32 *p)
{
  BYTE   a, b, c, d;
  UINT32 n;

  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    if (WAP_CVT_REM_LENGTH (obj) < 4)
      return FALSE;
    a = WAP_CVT_GET_BYTE (obj);
    b = WAP_CVT_GET_BYTE (obj);
    c = WAP_CVT_GET_BYTE (obj);
    d = WAP_CVT_GET_BYTE (obj);
    n = ((UINT32)a << 24) | ((UINT32)b << 16) | ((UINT32)c << 8) | (UINT32)d;
    if (n & 0x8000) {
      /* We have a negative number */
      n = (UINT32)(n & 0x7fffffff);
      if (n == 0)
        return FALSE; /* Error, because this might overflow */
      n = (0x7fffffff - n) + 1;
      *p = -(INT32)n;
    }
    else {
      *p = (INT32)n;
    }
    break;

  case WAP_CVT_ENCODE:
    if (WAP_CVT_REM_LENGTH (obj) < 4)
      return FALSE;
    if (*p < 0) {
      n = (0x7fffffff + *p) + 1;
      n |= 0x80000000;
    }
    else {
      n = *p;
    }
    WAP_CVT_PUT_BYTE (obj, (UINT8)((n >> 24) & 0xff));
    WAP_CVT_PUT_BYTE (obj, (UINT8)((n >> 16) & 0xff));
    WAP_CVT_PUT_BYTE (obj, (UINT8)((n >> 8) & 0xff));
    WAP_CVT_PUT_BYTE (obj, (UINT8)(n & 0xff));
    break;

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += 4;
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Return the number of bytes required to store n as a Uintvar.
 */
UINT8
wap_cvt_uintvar_len (UINT32 n)
{
  if (n < 0x80)
    return 1;
  else if (n < 0x4000)
    return 2;
  else if (n < 0x200000)
    return 3;
  else if (n < 0x10000000)
    return 4;
  else
    return 5;
}

/*
 * Convert a Uintvar.
 */
INT16
wap_cvt_uintvar (wap_cvt_t *obj, UINT32 *p)
{
  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    {
      UINT32 n = 0;
      UINT8  k = 0;
      BYTE   b;

      do {
        if (WAP_CVT_REM_LENGTH (obj) <= 0)
          return FALSE;
        b = WAP_CVT_GET_BYTE (obj);
        n <<= 7;
        n |= (b & 0x7f);
        k++;
        if (k > 5)
          return FALSE;
      } while (b > 0x7f);
      *p = n;
    }
    break;

  case WAP_CVT_ENCODE:
    {
      UINT32 n = *p;
      UINT8  k = wap_cvt_uintvar_len (n);
      BYTE   b;

      if (WAP_CVT_REM_LENGTH (obj) < k)
        return FALSE;
      for (; k > 0; k--) {
        b = (BYTE)((n >> ((k - 1) * 7)) & 0x7f);
        if (k > 1)
          b |= 0x80;
        WAP_CVT_PUT_BYTE (obj, b);
      }
    }
    break;

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += wap_cvt_uintvar_len (*p);
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert a static bytevector, i.e., a vector whose internal
 * representation is a static vector (e.g., BYTE v[12];).
 */
INT16
wap_cvt_static_bytevector (wap_cvt_t *obj, UINT32 length, BYTE *p)
{
  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    if (WAP_CVT_REM_LENGTH (obj) < length)
      return FALSE;
    memcpy (p, obj->data + obj->pos, length);
    obj->pos += length;
    break;

  case WAP_CVT_ENCODE:
    if (WAP_CVT_REM_LENGTH (obj) < length)
      return FALSE;
    memcpy (obj->data + obj->pos, p, length);
    /* Fall through */

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += length;
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert a byte vector, i.e., a vector whose internal representation
 * is a pointer to a byte (BYTE *v;).
 */
INT16
wap_cvt_bytevector (wap_cvt_t *obj, UINT32 length, BYTE **p)
{
  switch (obj->operation) {
  case WAP_CVT_DECODE:
    if (WAP_CVT_REM_LENGTH (obj) < length) {
      *p = NULL;
      return FALSE;
    }
    if (length == 0) {
      *p = NULL;
      break;
    }
    *p = OSConnectorAlloc (length);
    memcpy (*p, obj->data + obj->pos, length);
    obj->pos += length;
    break;

  case WAP_CVT_DECODE_STATIC:
    if (WAP_CVT_REM_LENGTH (obj) < length) {
      *p = NULL;
      return FALSE;
    }
    if (length == 0) {
      *p = NULL;
      break;
    }
    *p = obj->data + obj->pos;
    obj->pos += length;
    break;

  case WAP_CVT_ENCODE:
    if (WAP_CVT_REM_LENGTH (obj) < length)
      return FALSE;
    memcpy (obj->data + obj->pos, *p, length);
    /* Fall through */

  case WAP_CVT_ENCODE_SIZE:
    obj->pos += length;
    break;

  case WAP_CVT_FREE:
    DEALLOC (p);
    break;

  default:
    return FALSE;
  }

  return TRUE;
}
