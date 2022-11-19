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
 * pdubuf.c
 *
 * Created by Anders Edenbrandt, Wed Sep 08 17:26:51 1999.
 *
 * Revision history:
 *
 */
/*  Removed by GBU,000329,  already included by pdubuf.h->cmnnrsrc.h->ansilibs.h->=||=
#include <string.h>
*/

#include "pdubuf.h"

BYTE
pdubuf_extract (pdubuf *buf, UINT16 idx)
{
  if ((buf == NULL) || (buf->data == NULL))
    return 0;

  if (buf->currpos + idx >= buf->size)
    return 0;

  return buf->data[buf->currpos + idx];
}

BYTE *
pdubuf_addr (pdubuf **pbuf, UINT16 idx)
{
  if ((pbuf == NULL) || (*pbuf == NULL) || ((*pbuf)->data == NULL))
    return NULL;

  if ((*pbuf)->currpos + idx >= (*pbuf)->size)
    return NULL;

  return &((*pbuf)->data[(*pbuf)->currpos + idx]);
}

/*
 * Return a pointer to a new pdubuf structure, having a data buffer
 * of length "maxSize".
 * The current length is set to 0.
 * Returns NULL in case of error, e.g., if memory allocation fails.
 */
pdubuf *
pdubuf_new (UINT16 maxSize)
{
  pdubuf *buf = NEWARRAY (pdubuf, 1);

  if (buf == NULL)
    return NULL;

  buf->data = NEWARRAY (BYTE, maxSize);
  if (buf->data == NULL) {
    DEALLOC (&buf);
    return NULL;
  }

  buf->size = maxSize;
  buf->currpos = maxSize;

  return buf;
}

/*
 * Return a pointer to a new pdubuf structure, but do not allocate
 * any new data buffer. Instead use the buffer passed in "data"
 * as the new data buffer. The "data" buffer must have a size
 * of at least "maxSize" bytes.
 * The current length is set to "maxSize".
 * Returns NULL in case of error.
 */
pdubuf *
pdubuf_newFromData (UINT16 maxSize, BYTE *data)
{
  pdubuf *buf;

  if (data == NULL)
    return NULL;

  buf = NEWARRAY (pdubuf, 1);
  if (buf == NULL)
    return NULL;

  buf->data = data;
  buf->size = maxSize;
  buf->currpos = 0;

  return buf;
}

/*
 * Release the pdubuf structure pointed to and also its data buffer.
 */
void
pdubuf_release (pdubuf *buf)
{
  if (buf == NULL)
    return;

  if (buf->data != NULL)
    DEALLOC (&(buf->data));
  DEALLOC (&buf);
}


/*
 * Check if a PDU buffer holds valid data.
 * Returns a non-zero value if "buf" is NULL or if "buf->data" is NULL.
 */
UINT16
pdubuf_isNull (pdubuf *buf)
{
  return (buf == NULL) || (buf->data == NULL);
}


/*
 * Return the current length of the buffer.
 * Returns 0 in case of error.
 */
UINT16
pdubuf_getLength (pdubuf *buf)
{
  if ((buf == NULL) || (buf->data == NULL))
    return 0;

  return buf->size - buf->currpos;
}


/*
 * Return the maximum size of the buffer.
 * Returns 0 in case of error.
 */
UINT16
pdubuf_getMaxSize (pdubuf *buf)
{
  if ((buf == NULL) || (buf->data == NULL))
    return 0;

  return buf->size;
}

/*
 * Return a copy of the given buffer. All data in "buf" is copied
 * to the new buffer.
 * Returns NULL in case of error.
 */
pdubuf *
pdubuf_copyBuffer (pdubuf *buf)
{
  pdubuf *newbuf;

  if ((buf == NULL) || (buf->data == NULL))
    return NULL;

  newbuf = NEWARRAY (pdubuf, 1);
  if (newbuf == NULL)
    return NULL;

  newbuf->data = NEWARRAY (BYTE, buf->size);
  if (newbuf->data == NULL) {
    DEALLOC (&newbuf);
    return NULL;
  }

  newbuf->size = buf->size;
  newbuf->currpos = buf->currpos;
  B_COPYSTRINGN (newbuf->data + newbuf->currpos,
                 buf->data + buf->currpos, buf->size - buf->currpos);

  return newbuf;
}

/*
 * Return a pointer to the first position in the buffer that is in use.
 * Returns NULL in case of error.
 */
BYTE *
pdubuf_getStart (pdubuf *buf)
{
  if ((buf == NULL) || (buf->data == NULL))
    return NULL;

  return buf->data + buf->currpos;
}

/*
 * Change the current length by "delta" bytes, and return a pointer
 * to the new start location.
 * Returns NULL in case of error.
 */
BYTE *
pdubuf_changeLength (pdubuf *buf, INT16 delta)
{
  INT16 newpos;

  if ((buf == NULL) || (buf->data == NULL))
    return NULL;

  newpos = buf->currpos - delta;
  if ((newpos < 0) || (newpos > buf->size))
    return NULL;

  buf->currpos = newpos;

  return buf->data + buf->currpos;
}

/*
 * Set the current length to "len", and return a pointer
 * to the new start location.
 * Returns NULL in case of error.
 */
BYTE *
pdubuf_setLength (pdubuf *buf, UINT16 len)
{
  if ((buf == NULL) || (buf->data == NULL))
    return NULL;

  if (len > buf->size)
    return NULL;

  buf->currpos = buf->size - len;

  return buf->data + buf->currpos;
}

/*
 * Shift the contents of the buffer "delta" positions, and
 * return a pointer to the new start location.
 * A positive "delta" means move to the right, and a negative
 * means move to the left.
 * Returns NULL in case of error.
 */
BYTE *
pdubuf_shiftData (pdubuf *buf, INT16 delta)
{
  INT16  newpos;
  UINT16 n;

  if ((buf == NULL) || (buf->data == NULL))
    return NULL;

  newpos = buf->currpos + delta;
  if ((newpos < 0) || (newpos > buf->size))
    return NULL;

  /* Number of bytes to move: */
  if (newpos < buf->currpos)
    n = buf->size - buf->currpos;
  else
    n = buf->size - newpos;

  memmove (buf->data + newpos, buf->data + buf->currpos, n);
  buf->currpos = newpos;

  return buf->data + buf->currpos;
}


/*
 * Create a new PDU buffer and fill it with a segment from a given buffer,
 * namely the part that starts at byte position "offset" and has length "len".
 * Returns NULL in case of error.
 */
pdubuf *
pdubuf_segmentToNewBuf (pdubuf *buf, UINT16 offset, UINT16 len)
{
  pdubuf *newBuf;

  if ((buf == NULL) || (buf->data == NULL)
      || (buf->currpos + offset + len > buf->size)) {
    return NULL;
  }
  newBuf = pdubuf_new (len);
  if (newBuf == NULL) {
    return NULL;
  }
  newBuf->currpos = 0;
  B_COPYSTRINGN (newBuf->data, buf->data + buf->currpos + offset, len);

  return newBuf;
}

/*
 * Remove a segment from a PDU buffer, namely the segment that starts
 * at byte position "offset" and has length "len".
 * Returns -1 in case of error, 0 otherwise.
 */
INT16
pdubuf_cutSegment (pdubuf *buf, UINT16 offset, UINT16 len)
{
  if ((buf == NULL) || (buf->data == NULL)
      || (buf->currpos + offset + len > buf->size)) {
    return -1;
  }
  memmove (buf->data + buf->currpos + len, buf->data + buf->currpos, offset);
  buf->currpos += len;

  return 0;
}
