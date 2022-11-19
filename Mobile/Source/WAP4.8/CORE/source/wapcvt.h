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
 * wapcvt.h
 *
 * Created by Anders Edenbrandt, Mon Nov 20 16:52:41 2000.
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
#ifndef _wapcvt_h
#define _wapcvt_h

#include "tapicmmn.h"

#define WAP_CVT_DECODE             1
#define WAP_CVT_DECODE_STATIC      2
#define WAP_CVT_ENCODE             3
#define WAP_CVT_ENCODE_SIZE        4
#define WAP_CVT_FREE               5

/*
 * The object used for converting between internal
 * and external data representation.
 */
typedef struct {
  UINT32  pos;
  UINT32  length;
  UINT8   operation;
  BYTE   *data;
} wap_cvt_t;

void
wap_cvt_init (wap_cvt_t *obj, UINT8 operation, BYTE *buf, UINT32 length);

INT16
wap_cvt_uint8 (wap_cvt_t *obj, UINT8 *p);

INT16
wap_cvt_int8 (wap_cvt_t *obj, INT8 *p);

INT16
wap_cvt_uint16 (wap_cvt_t *obj, UINT16 *p);

INT16
wap_cvt_int16 (wap_cvt_t *obj, INT16 *p);

INT16
wap_cvt_uint32 (wap_cvt_t *obj, UINT32 *p);

INT16
wap_cvt_int32 (wap_cvt_t *obj, INT32 *p);

UINT8
wap_cvt_uintvar_len (UINT32 n);

INT16
wap_cvt_uintvar (wap_cvt_t *obj, UINT32 *p);

INT16
wap_cvt_static_bytevector (wap_cvt_t *obj, UINT32 length, BYTE *p);

INT16
wap_cvt_bytevector (wap_cvt_t *obj, UINT32 length, BYTE **p);

#endif
