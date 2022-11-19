#ifndef _DSRLP3_H
#define _DSRLP3_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                R A D I O   L I N K   P R O T O C O L  I I I

                             H E A D E R  F I L E 

GENERAL DESCRIPTION
  This file contains the interface definitiions for the third revision of 
  the Radio Link Protocol (RLP Type 3), as defined in IS-707-A.  This file 
  contains the  version specific interfaces for frame processing for both 
  receive and transmit of RLP frames.  All functions in this file are 
  internal to the RLP framework, and are called as function pointers from 
  the generic RLP frame processing in dsrlpi.c.

 Copyright (c) 1999, 2000 by QUALCOMM Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/dsrlp3.h_v   1.0   18 Oct 2000 18:07:00   jeffd  $

when       who        what, where, why
--------   ---        -------------------------------------------------------
04/12/00   na         Removed unnecessary DSRLP3_MAX_NAK_ROUNDS_FWD & REV
03/14/00   hcg        Fixed Lint errors.
08/05/99 na/hcg/ak    Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"

#include "dsrlpi.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
extern  dsrlpi_ver_const_type dsrlp3_constants;

#define DSRLP3_MAX_CONS_ERASURES    128

#define RLP3_NAK_LAST_S_SEQ_MAX     4095        /* for seg naks, max val  */
                                                /* for last s_seq         */
/*==========================================================================
                R L P 3  D A T A   F R A M E   D E F I N E S 
===========================================================================*/
/*---------------------------------------------------------------------------
  Unsegmented Data Frames.  The CTL field is a single bit and shall be set
  to '0'.  These frames also contain a retransmit bit, indicating whether or
  not the frame is a new data frame, or a retransmitted data frame.
---------------------------------------------------------------------------*/
#define RLP3_UNSEG_DATA_MASK                 0x80
#define RLP3_UNSEG_DATA_MASK_RSLT            0x00
#define RLP3_UNSEG_DATA_REXMIT_MASK          0x40

/*---------------------------------------------------------------------------
  These define the size of the unsegmented data frame's header, as well
  as the maximum size of the data payload in an unsegmented data frame.
  Both values are in bytes.
---------------------------------------------------------------------------*/
#define RLP3_DATA_UNSEG_HDR_SIZE                2
#define RLP3_DATA_UNSEG_DATA_MAX_LEN           32

/*---------------------------------------------------------------------------
  Segmented Data Frames.  The CTL field is four bits wide and shall be set
  to '1000'.  The SQI bit is used to determine if extended header sequence 
  number information is available.  If set, the frame contains a 4-bit SEQ_HI
  field.  These frames also contain a retransmit bit, indicating whether 
  or not the frame is a new segmented data frame, or a retransmitted 
  segmented data frame.
---------------------------------------------------------------------------*/
#define RLP3_SEG_DATA_FLD_CTL               0x80
#define RLP3_SEG_DATA_MASK                  0xF0
#define RLP3_SEG_DATA_SQI_MASK              0x08
#define RLP3_SEG_DATA_REXMIT_MASK           0x02
#define RLP3_SEG_DATA_LST_MASK              0x04

/*---------------------------------------------------------------------------
  Size of the segmented data frame's header in bytes, assuming all optional
  fields are included.
---------------------------------------------------------------------------*/
#define RLP3_DATA_SEG_HDR_SIZE                 5

/*---------------------------------------------------------------------------
  Bit lengths of the LEN, SEQ_HI, and S_SEQ fields in a segmented data
  frame.
---------------------------------------------------------------------------*/
#define RLP3_DATA_SEG_LEN_LEN                  5
#define RLP3_DATA_SEG_SEQ_HI_LEN               4
#define RLP3_DATA_SEG_S_SEQ_LEN               12
                                    
/*---------------------------------------------------------------------------
  Bit positions of the LEN, SEQ_HI, and S_SEQ fields in a segmented data
  frame, assuming all optional fields are included.
---------------------------------------------------------------------------*/
#define RLP3_DATA_SEG_LEN_POS                 15
                                      
#define RLP3_DATA_SEG_SEQ_HI_POS              (RLP3_DATA_SEG_LEN_POS +     \
                                              RLP3_DATA_SEG_LEN_LEN)

#define RLP3_DATA_SEG_S_SEQ_POS               (RLP3_DATA_SEG_SEQ_HI_POS +  \
                                              RLP3_DATA_SEG_SEQ_HI_LEN)

/*---------------------------------------------------------------------------
  Mask definitions for format D data frames CTL field bit positions used for 
  frame determination.  These masks are specific to the bit positions within 
  the ctl_byte that is extracted as part of the format D frame parsing.
---------------------------------------------------------------------------*/
#define RLP3_FMT_D_DATA_SSP_MASK            0x80
#define RLP3_FMT_D_DATA_SQI_MASK            0x40
#define RLP3_FMT_D_DATA_LST_MASK            0x20
#define RLP3_FMT_D_DATA_RTX_MASK            0x10

/*---------------------------------------------------------------------------
  Size of the FMT D frame's header in bytes.  All optional fields are
  included.  This value also includes the 1 byte for the type bits and
  MUX header.
---------------------------------------------------------------------------*/
#define RLP3_FMT_D_HDR_SIZE             6

/*---------------------------------------------------------------------------
  Bit lengths of the LEN, SEQ_HI, and S_SEQ fields in a FMT D frame.
---------------------------------------------------------------------------*/
#define RLP3_FMT_D_LEN_LEN              8
#define RLP3_FMT_D_SEQ_HI_LEN           4
#define RLP3_FMT_D_S_SEQ_LEN           12

/*---------------------------------------------------------------------------
  Bit positions of the LEN, SEQ_HI, and S_SEQ fields in a FMT D frame,
  assuming all optional fields are included.
---------------------------------------------------------------------------*/
#define RLP3_FMT_D_LEN_POS            20
                                      
#define RLP3_FMT_D_SEQ_HI_POS         (RLP3_FMT_D_LEN_POS +                 \
                                       RLP3_FMT_D_LEN_LEN)

#define RLP3_FMT_D_S_SEQ_POS          (RLP3_FMT_D_SEQ_HI_POS +              \
                                       RLP3_FMT_D_SEQ_HI_LEN)

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#endif /* _DSRLP3_H */
