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
 * pdubuf.h
 *
 * Created by Anders Edenbrandt, Wed Sep 08 17:25:44 1999.
 *
 * Revision history:
 *
 */
#ifndef _pdubuf_h
#define _pdubuf_h

/*
 * This file contains functions to handle a data type called "pdubuf".
 * This is a data buffer with attributes "max length", which is the
 * capacity of the buffer, and "current length", which is the length
 * of the part of the buffer that is currently in use.
 * Whenever the current length is less than the maximum length, it is
 * assumed that the part of the buffer that is being used is the last
 * "current length" bytes of the buffer.
 */

#include "cmmnrsrc.h"

typedef struct s_pdubuf {
  BYTE   *data;
  UINT16 currpos;
  UINT16 size;
} pdubuf;

BYTE
pdubuf_extract (pdubuf *pb, UINT16 idx);

BYTE *
pdubuf_addr (pdubuf **pb, UINT16 idx);

/*
 * Return a pointer to a new pdubuf structure, having a data buffer
 * of length "maxSize".
 * The current length is set to 0.
 * Returns NULL in case of error, e.g., if memory allocation fails.
 */
pdubuf *
pdubuf_new (UINT16 maxSize);

/*
 * Return a pointer to a new pdubuf structure, but do not allocate
 * any new data buffer. Instead use the buffer passed in "data"
 * as the new data buffer. The "data" buffer must have a size
 * of at least "maxSize" bytes.
 * The current length is set to "maxSize".
 * Returns NULL in case of error.
 */
pdubuf *
pdubuf_newFromData (UINT16 maxSize, BYTE *data);

/*
 * Release the pdubuf structure pointed to and also its data buffer.
 */
void
pdubuf_release (pdubuf *buf);

/*
 * Check if a PDU buffer holds valid data.
 * Returns a non-zero value if "buf" is NULL or if "buf->data" is NULL.
 */
UINT16
pdubuf_isNull (pdubuf *buf);

/*
 * Return the current length of the buffer.
 * Returns 0 in case of error.
 */
UINT16
pdubuf_getLength (pdubuf *buf);

/*
 * Return the maximum size of the buffer.
 * Returns 0 in case of error.
 */
UINT16
pdubuf_getMaxSize (pdubuf *buf);

/*
 * Return a copy of the given buffer. All data in "buf" is copied
 * to the new buffer.
 * Returns NULL in case of error.
 */
pdubuf *
pdubuf_copyBuffer (pdubuf *buf);

/*
 * Return a pointer to the first position in the buffer that is in use.
 * Returns NULL in case of error.
 */
BYTE *
pdubuf_getStart (pdubuf *buf);

/*
 * Change the current length by "delta" bytes, and return a pointer
 * to the new start location.
 * Returns NULL in case of error.
 */
BYTE *
pdubuf_changeLength (pdubuf *buf, INT16 delta);

/*
 * Set the current length to "len", and return a pointer
 * to the new start location.
 * Returns NULL in case of error.
 */
BYTE *
pdubuf_setLength (pdubuf *buf, UINT16 len);

/*
 * Shift the contents of the buffer "delta" positions, and
 * return a pointer to the new start location.
 * A positive "delta" means move to the right, and a negative
 * means move to the left.
 * Returns NULL in case of error.
 */
BYTE *
pdubuf_shiftData (pdubuf *buf, INT16 delta);

/*
 * Create a new PDU buffer and fill it with a segment from a given buffer,
 * namely the part that starts at byte position "offset" and has length "len".
 * Returns NULL in case of error.
 */
pdubuf *
pdubuf_segmentToNewBuf (pdubuf *buf, UINT16 offset, UINT16 len);

/*
 * Remove a segment from a PDU buffer, namely the segment that starts
 * at byte position "offset" and has length "len".
 * Returns -1 in case of error, 0 otherwise.
 */
INT16
pdubuf_cutSegment (pdubuf *buf, UINT16 offset, UINT16 len);

#endif
