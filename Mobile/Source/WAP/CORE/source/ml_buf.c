/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Modulname    : ML_BUF.C                                                     |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|    This Module contains functions to handle different types of buffers,      |
|    namely :                                                                  |
|                                                                              |
|       * Ringbuffer                                                           |
|       * FIFO                                                                 |
|                                                                              |
|    3 functions are implemented representing the low level functions. Each    |
|    entry has to be of the same size, because the handling is in principle    |
|    similar to array. The type of each entry is not relevant. In addition,    |
|    the caller of the functions contained in this module defines the          |
|    memory for the tracebuffer as well as a control structure representing    |
|    all the internal data and variables necessary to administer the .         |
|    tracebuffer.                                                              |
|                                                                              |
|    Users are free to have more than 1 Instance of a buffertype.              |
|                                                                              |
|  M1 Errormessages, which are routed to the ErrorHandler                      |
|  --------------------------------------------------------                    |
|  - none -                                                                    |
|                                                                              |
|  M2 Exported functions of this module :                                      |
|  --------------------------------------------------------                    |
|    ...        xmk_RingInit  ( ... )                                          |
|    ...        xmk_RingRead  ( ... )                                          |
|    ...        xmk_RingWrite ( ... )                                          |
|    ...        xmk_RingQuery ( ... )                                          |
|                                                                              |
|  M3 Static functions of this module :                                        |
|  --------------------------------------------------------                    |
|    - none -                                                                  |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-MHDR*/

/*
+------------------------------------------------------------------------------+
|                                                                              |
|  Copyright by Telelogic AB 1993 - 1998                                       |
|  Copyright by S&P Media GmbH Germany 1993 - 1998                             |
|                                                                              |
|  This Program is owned by Telelogic and is protected by national             |
|  copyright laws and international copyright treaties. Telelogic              |
|  grants you the right to use this Program on one computer or in              |
|  one local computer network at any one time.                                 |
|  Under this License you may only modify the source code for the purpose      |
|  of adapting it to your environment. You must reproduce and include          |
|  any copyright and trademark notices on all copies of the source code.       |
|  You may not use, copy, merge, modify or transfer the Program except as      |
|  provided in this License.                                                   |
|  Telelogic does not warrant that the Program will meet your                  |
|  requirements or that the operation of the Program will be                   |
|  uninterrupted and error free. You are solely responsible that the           |
|  selection of the Program and the modification of the source code            |
|  will achieve your intended results and that the results are actually        |
|  obtained.                                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/


#ifndef __ML_BUF_C_
#define __ML_BUF_C_

/*+IMPORT*/
/*====================  I M P O R T  =========================================*/
#include "ml_typw.h"

/*============================================================================*/
/*-IMPORT*/


/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/
#ifdef  BUFFERSIZE
#undef  BUFFERSIZE
#endif
#define BUFFERSIZE      p_controlblock->buffersize

#ifdef  ENTRYSIZE
#undef  ENTRYSIZE
#endif
#define ENTRYSIZE       p_controlblock->entrysize

#ifdef  MAXIMUM
#undef  MAXIMUM
#endif
#define MAXIMUM         p_controlblock->maximumentries

#ifdef  COUNTER
#undef  COUNTER
#endif
#define COUNTER         p_controlblock->counter

#ifdef  BUFFERADR
#undef  BUFFERADR
#endif
#define BUFFERADR       p_controlblock->p_buffer

#ifdef  WRITEPTR
#undef  WRITEPTR
#endif
#define WRITEPTR        p_controlblock->p_write

#ifdef  READPTR
#undef  READPTR
#endif
#define READPTR         p_controlblock->p_read

#ifdef  XMK_CTRL
#undef  XMK_CTRL
#endif
#define XMK_CTRL        p_controlblock->ctrl

#ifdef  COPY
#undef  COPY
#endif
#define COPY(p1,p2,len) \
                        if (len <= ENTRYSIZE) \
                          (void)memcpy ((void xmk_RAM_ptr) p1, (void xmk_RAM_ptr) p2, len); \
                        else \
                          (void)memcpy ((void xmk_RAM_ptr) p1, (void xmk_RAM_ptr) p2, ENTRYSIZE);

#ifdef  INCR_WRITEPTR
#undef  INCR_WRITEPTR
#endif
#define INCR_WRITEPTR   WRITEPTR = WRITEPTR + ENTRYSIZE ;

#ifdef  INCR_READPTR
#undef  INCR_READPTR
#endif
#define INCR_READPTR    READPTR  = READPTR  + ENTRYSIZE ;

/*--------------------  Constants,  Macros  ----------------------------------*/

/*--------------------  Typedefinitions     ----------------------------------*/

/*--------------------  Functions    -----------------------------------------*/

/*--------------------  Variables    -----------------------------------------*/

/*============================================================================*/
/*-MGG*/


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_RingInit                                                 |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|  This function initializes a tracebuffer. The buffer is declared by the      |
|  caller of this function, and this module has no global data.                |
|  The buffer is to be declared as type xmk_T_RING by the caller.              |
|  The structure contains all the data necessary to manage the tracebuffer.    |
|  If there is enough memory, then multiple tracebuffers are possible to use.  |
|  The last parameter selects  the mechanism which is to be used, either       |
|  ring buffer or fifo.                                                        |
|                                                                              |
|  Parameter    : xmk_T_RING    *p_controlblock. Pointer to the memory declared|
|                                             as xmk_T_RING by the caller.     |
|                                                                              |
|                 char          *p_buffer        pointer to ringbuffer         |
|                 int           buffersize      sizeof (buffer)                |
|                 unsigned char SizeOfOneEntry Size of one entry in the        |
|                                              buffer.                         |
|                 unsigned char ctrl           either XMK_FIFO or XMK_RING     |
|                                                                              |
|  Return       : void                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
   /* ANSI - Style */
   void xmk_RingInit (xmk_T_RING    xmk_RAM_ptr p_controlblock,
                      char          xmk_RAM_ptr p_buffer,
                      int           buffersize,
                      int           SizeOfOneEntry,
                      unsigned char ctrl )
#else
   /* Kernighan-Ritchie-Style */
   void xmk_RingInit (p_controlblock,
                      p_buffer,
                      buffersize,
                      SizeOfOneEntry,
                      ctrl)
   xmk_T_RING   xmk_RAM_ptr p_controlblock;
   char         xmk_RAM_ptr p_buffer;
   int          buffersize;
   int          SizeOfOneEntry;
   unsigned     char ctrl ;
#endif

/*-FDEF E*/
{
   XMK_BEGIN_CRITICAL_PATH;

   BUFFERSIZE = buffersize                  ;
   MAXIMUM    = buffersize / SizeOfOneEntry ;
   ENTRYSIZE  = SizeOfOneEntry              ;
   COUNTER    = 0                           ;
   BUFFERADR  = p_buffer                    ;
   READPTR    = p_buffer                    ;
   WRITEPTR   = p_buffer                    ;

   if ((ctrl == XMK_FIFO) || (ctrl==XMK_RING))
   {
     XMK_CTRL     = ctrl                        ;
   }
   else
   {
     XMK_CTRL     = XMK_FIFO                    ;
   }

   XMK_END_CRITICAL_PATH;

} /* END OF FUNCTION */


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_RingWrite                                                |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|  This functions puts the data into the ringbuffer. The caller has to give    |
|  a pointer to the tracebuffer as the first parameter.                        |
|                                                                              |
|  Parameter    : xmk_T_RING      * Pointer to tracebuffercontrolblock.        |
|                 char            * Pointer to data, which is to be put in     |
|                 int  len          length of data to copy ....                |
|                                                                              |
|  Return       : XMK_OVERFLOW     Ringbuffer full, the new entry has over-    |
|                                  written the last one.                       |
|                 XMK_ERROR        Error, i.e.illegal length                   |
|                 XMK_OKAY         No overflow occurred.                       |
|                                                                              |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
   /* ANSI - Style */
   int  xmk_RingWrite (xmk_T_RING xmk_RAM_ptr p_controlblock, char xmk_RAM_ptr p_entry, int xlen)

#else
   /* Kernighan-Ritchie-Style */
   int              xmk_RingWrite (p_controlblock, p_entry, xlen)
   xmk_T_RING        xmk_RAM_ptr p_controlblock;
   char              xmk_RAM_ptr p_entry;
   int               xlen;
#endif

/*-FDEF E*/
{
  int result = XMK_OKAY;
  int len;


  #ifdef XMK_USE_MAX_ERR_CHECK
    if (xlen > ENTRYSIZE)
    {
       return (XMK_ERROR);
    }
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  if (COUNTER >= MAXIMUM)
  {
    /*
    ** no place to write, use the last one ...
    */
    COUNTER = MAXIMUM      ;
    result  = XMK_OVERFLOW ;
    if (XMK_CTRL == XMK_FIFO)
    {
      XMK_END_CRITICAL_PATH;

      /*
      ** no overwrite if fifo control is used ....
      */
      return (XMK_OVERFLOW);
    }
  }
  else
  {
    /*
    ** there is another place ...
    */
    COUNTER ++;
  }

  /*
  ** if writepointer points behind the ringbuffer ...
  */
  if (WRITEPTR >= (BUFFERADR + BUFFERSIZE))
  {
    WRITEPTR = BUFFERADR ;
  }

  /*
  ** 8051x compiler needs this temporary variable at this location
  */    
  len = xlen;


  /*
  ** in any case write destructive ..
  */
  COPY (WRITEPTR, p_entry, len);

  /*
  ** increment writepointer ...
  */
  INCR_WRITEPTR ;

  XMK_END_CRITICAL_PATH;

  /*
  ** return with the result ...
  */
  return (result);

} /* END OF FUNCTION */


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_RingRead                                                 |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|  This functions reads data from the tracebuffer. The caller receives a       |
|  pointer to the entry or a NULL pointer, if no entry was found.              |
|  XMK_OKAY      : OK, 1 entry has been read and was copied into p_dest.t      |
|  XMK_EMPTY     : OK, no entry found (buffer empty)                           |
|  XMK_ERROR     : Error, the entry, which is to be read from the buffer, is   |
|                  greater than the dimension of the destination buffer.       |
|                                                                              |
|  Parameter    : xmk_T_RING      * Pointer to tracebuffercontrolblock.        |
|                 char            * p_dest Pointer to destination, where       |
|                                   the entry is to be copied into.            |
|                                   the entry is to be copied into.            |
|                 int               destlen buffersize of destination for      |
|                                   errorcheck.                                |
|  Return       : int               XMK_OKAY |XMK_EMPTY | XMK_ERROR            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
   /* ANSI - Style */
   int            xmk_RingRead ( xmk_T_RING xmk_RAM_ptr p_controlblock, 
                                 char xmk_RAM_ptr p_dest,
                                 int    destlen )
#else
   /* Kernighan-Ritchie-Style */
   int            xmk_RingRead ( p_controlblock, p_dest, destlen )
   xmk_T_RING     xmk_RAM_ptr p_controlblock;
   char           xmk_RAM_ptr p_dest;
   int            destlen;
#endif

/*-FDEF E*/
{
  int len;

  if (! COUNTER)
  {
    /*
    ** nothing to read ...
    */
    return (XMK_EMPTY);
  }

  XMK_BEGIN_CRITICAL_PATH;

  COUNTER --;

  /*
  ** check if destination buffer is large enough ....
  */
  if (ENTRYSIZE > destlen)
  {
    XMK_END_CRITICAL_PATH;
    return (XMK_ERROR);
  }

  /*
  ** prepare return value ...
  ** 8051x compiler needs this temporary variable at this location
  */    
  len = destlen;

  /*
  ** in any case write destructive ..
  */
  COPY (p_dest, READPTR, len);

  /*
  ** increment readpointer  ...
  */
  INCR_READPTR ;

  /*
  ** if readpointer points behind the ringbuffer ...
  */
  if (READPTR >= (BUFFERADR + BUFFERSIZE))
  {
    READPTR = BUFFERADR ;
  }

  XMK_END_CRITICAL_PATH;

  /*
  ** return with the result ...
  */
  return (XMK_OKAY);

} /* END OF FUNCTION */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_RingQuery                                                |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|  This functions returns the amount of free entries in the ringbuffer to the  |
|  caller.                                                                     |
|                                                                              |
|  Parameter    : xmk_T_RING      * Pointer to ringbuffer controlblock.        |
|                                                                              |
|  Return       : int               Amount of free entries or 0, if no entry   |
|                                   left free.                                 |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
   /* ANSI - Style */
   int               xmk_RingQuery ( xmk_T_RING  xmk_RAM_ptr p_controlblock )

#else
   /* Kernighan-Ritchie-Style */
   int               xmk_RingQuery ( p_controlblock )
   xmk_T_RING        xmk_RAM_ptr p_controlblock;
#endif

/*-FDEF E*/
{
    /*
    ** Check if buffer initialized correctly
    */
  #ifdef XMK_USE_MAX_ERR_CHECK
    /*
    ** Check if buffer initialization is correct, and if counter is consistent
    */
    if ((MAXIMUM == 0) || (COUNTER > MAXIMUM)) return (0);
  #endif

  return ( MAXIMUM - COUNTER );

} /* END OF FUNCTION */

#endif /* __ML_BUF_C_ */
