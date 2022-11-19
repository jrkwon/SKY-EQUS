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

/* Program generated by SDT Cmicro, version 1.33 */

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif

#define C_MICRO_1_3
#include "ml_typw.h"
#include "ctypes.h"
#include "buffmgt.h"


/*************************************************************************
**                  #CODE directives, #BODY sections                    **
*************************************************************************/

/*************************************************************************
**                  SECTION Variables and Functions                     **
*************************************************************************/

/*****
* NEWTYPE PDU_Buffer  (BODY section)
* <<PACKAGE BuffMgt>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\buffmgt.sun(Declarations2),152(35,45),1)
******/
                /* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\buffmgt.sun(Declarations2),152(35,45),37) */

#ifndef XMBM_COPIED
#define XMBM_COPIED  2  COMMENT(needed because of cbasic)
#endif

#ifdef XREADANDWRITEF
extern char *yWri_pdubuf_ptr( void  * C)
{
  pdubuf_ptr bh_p;
  char * c_p;
  unsigned int len;
  unsigned int i;
  unsigned char * dat_p;
  bh_p = *(pdubuf_ptr*)C;

  if ( bh_p == NULL )
  {
    sprintf(yCTemp_Data, "\nPDU_Buffer[ ptr=0x0h ]");
  }
  else
  {
      sprintf(yCTemp_Data, "\nPDU_Buffer[ ptr=0x%Xh len=%u"
              ,bh_p, (len=pdubuf_getLength(bh_p)) );
      c_p = yCTemp_Data + strlen(yCTemp_Data);
      sprintf(c_p, "\n dat = " );
      c_p = yCTemp_Data + strlen(yCTemp_Data);
      dat_p = pdubuf_getStart(bh_p);
      for ( i=0; i<len; i++ )
      {
        if ((i%16==0)&&(i!=0))
        {
          sprintf(c_p, "\n0x%02Xh: ",(i/16) );
          c_p = yCTemp_Data + strlen(yCTemp_Data);
        }
        if (strlen(yCTemp_Data)>4000)
        {
          sprintf(c_p, " ..complete buffer is not displayed.. ", (int)*dat_p );
          break;
        }
    sprintf(c_p, "%02X ", (int)*dat_p );
        dat_p++;
        c_p = yCTemp_Data + strlen(yCTemp_Data);
      }
      c_p = yCTemp_Data + strlen(yCTemp_Data);
      sprintf(c_p, "] " );
  }
  return yCTemp_Data;
}
#endif


void
yDef_pdubuf_ptr (pdubuf_ptr *yVar)
{
  *yVar = 0;
}

COMMENT(( \
  Returns a PDU buffer with a data \
  buffer of ypar1 bytes. \
  Returns NULL if out of memory. \
))
pdubuf *PDU_get(SDL_Natural ypar1)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - get\n");
#  endif

  return pdubuf_new((UINT16) ypar1);
}


COMMENT(( \
  Returns a PDU buffer with a pre-allocated \
  data buffer of ypar2 bytes. \
  Returns NULL if out of memory. \
))
pdubuf *PDU_getFromData(void *ypar1, SDL_Natural ypar2)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - getFromData\n");
#  endif

  return pdubuf_newFromData((UINT16) ypar2, (BYTE*)ypar1);
}


COMMENT(( \
  Releases a PDU buffer including \
  the data it holds. Returns true \
  if succesful, otherwise false. \
))
SDL_Boolean PDU_rel(pdubuf **ypar1)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - rel\n");
#  endif

  if ( *ypar1 == NULL )
    return SDL_False;
  pdubuf_release(*ypar1);
  *ypar1 = NULL;
  return SDL_True;
}


COMMENT(( \
Checks if a PDU buffer holds \
allocated data. Returns false \
if it does, otherwise true. \
))
SDL_Boolean PDU_isNull(pdubuf *ypar1)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - isNull\n");
#  endif

  if ( ypar1 == NULL )
    return SDL_True;
  if (pdubuf_isNull(ypar1))
    return SDL_True;
  else
    return SDL_False;
}


COMMENT(( \
  Returns the used number of bytes (i.e. \
  the right side of the buffer) of the \
  passed PDU buffer. Returns zero \
  if the PDU buffer is NULL. \
))
SDL_Natural PDU_getLen(pdubuf *ypar1)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - getLen\n");
#  endif

  if ( ypar1 == NULL )
  {
    return (SDL_Natural)0;
  }
  return (SDL_Natural)pdubuf_getLength(ypar1);
}


COMMENT(( \
  Returns the total number of bytes (i.e. \
  both the right and left side of the buffer) \
  of the passed PDU buffer. \
  Returns zero if the PDU buffer is NULL. \
))
SDL_Natural PDU_maxSiz(pdubuf *ypar1)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - maxSiz\n");
#  endif

  if ( ypar1 == NULL )
  {
    return (SDL_Natural)0;
  }
  return (SDL_Natural)pdubuf_getMaxSize(ypar1);
}


COMMENT(( \
  Returns a copy of a buffer or NULL if \
  ypar1 is NULL. \
))
pdubuf *PDU_copy(pdubuf *ypar1)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - copy\n");
#  endif

  if ( ypar1 == NULL )
  {
    return NULL;
  }

  return pdubuf_copyBuffer(ypar1);
}

COMMENT(( \
  Increments the active size of the buffer. \
      Returns False if the requested increment is too large for the buffer. \
))
SDL_Boolean PDU_inc (pdubuf *ypar1, SDL_Natural ypar2)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - inc\n");
#  endif

  if ( ypar1 == NULL )
  {
    return SDL_False;
  }

  return (pdubuf_changeLength (ypar1, ypar2) != NULL);
}

COMMENT(( \
  Decrements the active size of the buffer. \
      Returns False if the requested decrement is too large for the buffer. \
))
SDL_Boolean PDU_dec (pdubuf *ypar1, SDL_Natural ypar2)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - dec\n");
#  endif

  if ( ypar1 == NULL )
  {
    return SDL_False;
  }

  return (pdubuf_changeLength (ypar1, -ypar2) != NULL);
}


COMMENT(( \
  Takes a PDU buffer and returns an \
  Octet string sharing the memory \
  of the used part (i.e. the right \
  side of the buffer) of the buffer. \
))
SDL_Boolean PDU_getDat(pdubuf *ypar1, SDL_Octet_String* ypar2)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - getDat\n");
#  endif

  if ( ypar1 == NULL )
  {
    return SDL_False;
  }

  if ( (ypar2->Bits != (SDL_Bit *)0) && (ypar2->IsAssigned != XMBM_COPIED) )
    xFree_SDL_Bit_String((void **)ypar2);

  ypar2->Bits = (SDL_Bit *)pdubuf_getStart(ypar1);
  ypar2->Length = (int)pdubuf_getLength(ypar1);
  ypar2->IsAssigned = XMBM_COPIED;

  if ( ypar2->Bits == NULL )
    return SDL_False;

  return SDL_True;
}


COMMENT(( \
  Takes a PDU buffer and enlarges \
  (increases the right side of the \
  buffer) it with ypar2 bytes. \
  Returns an Octet string sharing the \
  memory of the new usable part (i.e. \
  the right side of the buffer) of \
  the buffer. \
))
SDL_Boolean PDU_insrt(pdubuf *ypar1, SDL_Natural ypar2, SDL_Octet_String* ypar3)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - insrt\n");
#  endif

  if ( ypar1 == NULL )
  {
    return SDL_False;
  }

  if ( (ypar3->Bits != (SDL_Bit *)0) && (ypar3->IsAssigned != XMBM_COPIED) )
    xFree_SDL_Bit_String((void **)ypar3);

  ypar3->Bits = (SDL_Bit *)pdubuf_changeLength(ypar1, (INT16)ypar2);
  ypar3->Length = (int)pdubuf_getLength(ypar1);
  ypar3->IsAssigned = XMBM_COPIED;

  if ( ypar3->Bits == NULL )
    return SDL_False;

  return SDL_True;
}


COMMENT(( \
  Takes a PDU buffer and shortens \
  (decreases the right side of the \
  buffer) it with ypar2 bytes. \
  Returns an Octet string sharing \
  the memory of the new usable part \
  (i.e. the right side of the buffer) \
  of the buffer. \
))
SDL_Boolean PDU_strip(pdubuf *ypar1, SDL_Natural ypar2, SDL_Octet_String* ypar3)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - strip\n");
#  endif

  if ( ypar1 == NULL )
  {
    return SDL_False;
  }

  if ( (ypar3->Bits != (SDL_Bit *)0) && (ypar3->IsAssigned != XMBM_COPIED) )
    xFree_SDL_Bit_String((void **)ypar3);

  ypar3->Bits = (SDL_Bit *)pdubuf_changeLength(ypar1, -(INT16)ypar2);
  ypar3->Length = (int)pdubuf_getLength(ypar1);
  ypar3->IsAssigned = XMBM_COPIED;

  if ( ypar3->Bits == NULL )
    return SDL_False;

  return SDL_True;
}


COMMENT(( \
  Takes a PDU buffer and enlarges \
  (increases the right side of the \
  buffer) it with ypar2 bytes. \
  Then it moves the formerly used \
  bytes of the buffer ypar2 bytes \
  to the left. \
  Returns an Octet string sharing \
  the memory of the new usable part \
  (i.e. the right side of the buffer) \
  of the buffer. \
))
SDL_Boolean PDU_appnd(pdubuf *ypar1, SDL_Natural ypar2, SDL_Octet_String* ypar3)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - appnd\n");
#  endif

  if ( ypar1 == NULL )
  {
    return SDL_False;
  }

  if ( (ypar3->Bits != (SDL_Bit *)0) && (ypar3->IsAssigned != XMBM_COPIED) )
    xFree_SDL_Bit_String((void **)ypar3);

  ypar3->Bits = (SDL_Bit *)pdubuf_shiftData(ypar1, -(INT16)ypar2);
  ypar3->Length = (int)pdubuf_getLength(ypar1);
  ypar3->IsAssigned = XMBM_COPIED;

  if ( ypar3->Bits == NULL )
    return SDL_False;

  return SDL_True;
}


COMMENT(( \
  Create a new PDU buffer and fill it with a \
  segment from "ypar1", namely the \
  part that starts at byte position "ypar2" and \
  has length "ypar3". Returns NULL in case of \
  error. \
))
pdubuf *PDU_segm2pdu(pdubuf *ypar1, SDL_Natural ypar2, SDL_Natural ypar3)
{
#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - segm2pdu\n");
#  endif

  return pdubuf_segmentToNewBuf (ypar1, (UINT16)ypar2, (UINT16)ypar3);
}


COMMENT(( \
  Remove a segment from a PDU buffer "ypar1",
  namely the segment that starts at byte position
  "ypar2" and has length "ypar3". Returns FALSE
  in case of error, TRUE otherwise.
))
SDL_Boolean PDU_cutSegm(pdubuf *ypar1, SDL_Natural ypar2, SDL_Natural ypar3)
{
  SDL_Natural result = pdubuf_cutSegment (ypar1, (UINT16)ypar2, (UINT16)ypar3);

#  if defined(XSCT_CBASIC)
  xPrintString("PDU_Buffer - cutSegm\n");
#  endif

  return (result == 0);
}


