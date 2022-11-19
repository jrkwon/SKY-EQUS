#ifndef CAIX_H
#define CAIX_H
/*===========================================================================

            C D M A   T R A N S L A T I O N    S E R V I C E S    
                    D A T A    D E C L A R A T I O N S 

DESCRIPTION
  This file contains the data structures needed for use in the
  CDMA translation services.
  

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/caix.h_v   1.1   02 Oct 2000 19:24:16   louiel  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/02/00   yll     Removed the packing of MSID type. MSID packing was moved 
                   to PDSM.
06/19/00   yll     Added packing the MSID to be used by PDSM IS801 services.
12/08/99   jw      Basic IS2000 message support.
02/12/99   lcc     Added some IS95B support related to pilot reporting.
11/23/98   ram     merged in IS95B P_REV support from
                   MDR archive. Updated Copyright notice.
01/23/91   jai     Changed name of file to caix.h
10/30/90   jai     Created file.

===========================================================================*/


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Channels */
enum
{
  CAIX_SC,     /* Sync Channel */
  CAIX_PC,     /* Paging Channel */
  CAIX_AC,     /* Access Channel */
  CAIX_TC      /* Traffic Channel */
};

/* Return status */
enum
{
  CAIX_DONE_S = 0,  /* Message has been translated ok */
  CAIX_INV_LEN_S,   /* Message length was invalid */
  CAIX_INV_MSG_S,   /* Received message type was invalid */
  CAIX_INV_CHN_S    /* Received Channel was invalid */
};


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/* Procedure which translates messgaes from external to internal format */
extern word caix_ext_to_int
       (
       word len,
         /* message length in bits */
       word chn,                    
         /* channel message was received on */
       cai_rx_msg_type *rx_msg_ptr,    
         /* pointer to received message to be translated */
       caii_rx_msg_type *int_msg_ptr   
         /* pointer to place where translated message should be placed */
       );

/* Procedure which translates messgaes from internal to external format */
extern word caix_int_to_ext
       (
       word *len_ptr,
         /* pointer to where message length, in bits should be placed */
       word chn,                   
         /* channel message is to be transmitted on */
       caii_tx_msg_type *int_msg_ptr,  
         /* pointer to internal message to be tanslated */
       cai_tx_msg_type *tx_msg_ptr 
         /* pointer to place where translated message should be placed */
       );

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

#ifdef FEATURE_IS2000
  #define CAIX_P_REV_IS2000   6
   /* Minimum P_REV for IS2000 phones */
#endif

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  #define CAIX_P_REV_IS95B    4
   /* Minimum P_REV for IS95B phones */
  #define CAIX_P_REV_UNKNOWN  0
   /* Used to tell layer 3 that a message is in old format ( Pre 95B ) */

/* Procedure which translates pilot report from inernal format to external
   format. This is used in IS-95B Accessing. The external format of the
   pilot report can be appended to the external format of an access channel
   message */
#ifndef FEATURE_IS2000
extern word xlate_int_plt_rpt
       (
       word *len_ptr,
         /* Length of message in bits */
       byte *buf_ptr,
         /* Pointer to where translated message should be placed */
       caii_plt_rpt_type *int_ptr
         /* Pointer to internal format of the pilot report */
       );
#endif
#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

#ifdef FEATURE_IS2000
void caix_set_p_rev_in_use(byte p_rev_in_use);
#endif

#endif /* CAIX_H */

