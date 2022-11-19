#ifndef DIAGPARM_H 
#define DIAGPARM_H
/*==========================================================================

   P A R A M E T E R   R E T R E I V A L   D A T A   D E F I N I T I O N S

DESCRIPTION
  Structure definitions to support the retreivable parameter function
  of the diag subsystem.

Copyright (c) 1992,1993,1994,1995 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 1996,1997,1998,1999 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/diagparm.h_v   1.1   10 Sep 1999 17:39:20   lchan  $

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/10/92   twp     First release

===========================================================================*/


/*===========================================================================

                        STRUCTURE DEFINITIONS

===========================================================================*/

/*
   The set of parameters that can be downloaded or retreived. 

   This enum follows the values assigned in CAI Ver. 2.0.
      Appendix E CDMA RETRIEVABLE AND SETTABLE PARAMETERS

*/

typedef enum {

  /* Accumulated Reverse Traffic Channel Stats -- 
     Defined in CAI Ver. 2.0 Section 6.4.5.2, Table 6.4.5.2-1 */

  MUX1_REV_1 = 1,    /* 9600 bps frame, primary traffic only */
  MUX1_REV_2,        /* 9600 bps frame, dim&burst w/ R=1/2 prim & sig */
  MUX1_REV_3,        /* 9600 bps frame, dim&burst w/ R=1/4 prim & sig */
  MUX1_REV_4,        /* 9600 bps frame, dim&burst w/ R=1/8 prim & sig */
  MUX1_REV_5,        /* 9600 bps frame, blank&burst, sig only */
  MUX1_REV_6,        /* 4800 bps frame, primary traffic only */
  MUX1_REV_7,        /* 2400 bps frame, primary traffic only */
  MUX1_REV_8,        /* 1200 bps frame, prim only or null Traffic only */
  MUX1_REV_9,        /* Reserved. */
  MUX1_REV_10,       /* Reserved. */
  MUX1_REV_11,       /* 9600 bps frame, dim&burst w/ R=1/2 prim & sec */
  MUX1_REV_12,       /* 9600 bps frame, dim&burst w/ R=1/4 prim & sec */
  MUX1_REV_13,       /* 9600 bps frame, dim&burst w/ R=1/8 prim & sec */
  MUX1_REV_14,       /* 9600 bps frame, blank&burst w/ secondary only */

  /* Accumulated Forward Traffic Channel Statistics -- 
     Defined in CAI Ver. 2.0 Section 6.4.5.4, Table 6.4.5.4-1 */

  MUX1_FOR_1,        /* 9600 bps frame, primary traffic only */
  MUX1_FOR_2,        /* 9600 bps frame, dim&burst w/ R=1/2, prim & sig */
  MUX1_FOR_3,        /* 9600 bps frame, dim&burst w/ R=1/4, prim & sig */
  MUX1_FOR_4,        /* 9600 bps frame, dim&burst w/ R=1/8, prim & sig */
  MUX1_FOR_5,        /* 9600 bps frame, blank&burst w/ signalling only */
  MUX1_FOR_6,        /* 4800 bps frame, primary traffic only */
  MUX1_FOR_7,        /* 2400 bps frame, primary traffic only */
  MUX1_FOR_8,        /* 1200 bps frame, prim or null Traffic Channel only */
  MUX1_FOR_9,        /* 9600 bps frame, prim only, probable bit errors */
  MUX1_FOR_10,       /* Frame quality insufficient to decide upon rate */
  MUX1_FOR_11,       /* 9600 bps frame, dim&burst, w/ R=1/2 prim & sec */
  MUX1_FOR_12,       /* 9600 bps frame, dim&burst, w/ R=1/4 prim & sec */
  MUX1_FOR_13,       /* 9600 bps frame, dim&burst, w/ R=1/8 prim & sec */
  MUX1_FOR_14,       /* 9600 bps frame, blank&burst, w/ secondary only */

  /* Accumulated Paging Channel Statistics -- 
     Defined in CAI Ver. 2.0 Section 6.4.5.3, Table 6.4.5.3-1 */

  PAG_1,             /* # Paging Channel msgs mobile tried to rx */
  PAG_2,             /* # Paging Channel msgs rx'd w/ good CRC */
  PAG_3,             /* # Paging Channel msgs or recs rx'd that
                          were addressed to this mobile */
  PAG_4,             /* # times mobile had to resync to Paging Channel */
  PAG_5,             /* # times mobile declared loss of Paging Channel */
  PAG_6,             /* # mobile station idle handoffs */

  /* Accumulated Access Channel Statistics -- 
     Defined in CAI Ver. 2.0 Section 6.4.5.1, Table 6.4.5.1-1 */

  ACC_1,            /* # Access Channel req msgs generated by layer 3 */
  ACC_2,            /* # Access Channel rsp msgs generated by layer 3 */
  ACC_3,            /* # times access probe tx'd at least 2 times */
  ACC_4,            /* # times access probe tx'd at least 3 times */
  ACC_5,            /* # times access probe tx'd at least 4 times */
  ACC_6,            /* # times access probe tx'd at least 5 times */
  ACC_7,            /* # times access probe tx'd at least 6 times */
  ACC_8,            /* Reserved */
  ACC_9,            /* Reserved */
  ACC_10,           /* # Unsuccessful access attempts */

  /* Accumulated Layer 2 Statistics --
     Defined in CAI Ver. 2.0 Section 6.4.5.5, Table 6.4.5.5-1 */

  LAYER2_RTC1,      /* # msgs needing ack rx'd at least once on RTC */
  LAYER2_RTC2,      /* # msgs needing ack tx'd at least twice on RTC */
  LAYER2_RTC3,      /* # msgs needing ack tx'd at least 3 times */
  LAYER2_RTC4,      /* Reserved. */
  LAYER2_RTC5,      /* # times call aborted for timeout */
  LAYER2_RTC6,      /* # msgs not needing act tx'd on RTC */

  /* Other Monitored Quantities and Statistics -- 
     Defined in CAI Ver. 2.0 Section 6.4.5.6, Table 6.4.5.6-1 */

  OTHER_SYS_TIME,   /* SYS_TIME field from the first Sync
                         Channel Message received after power on */

  /* Call history parameter --
     Defined in CAI Ver. 2.0 Section 2.3.12.1.3 */

  COUNT,            /* Call History parameter */

  PARM_ID_MAX_I     /* list terminator. */

} parm_id_enum_type;

#endif  /* DIAGPARM_H */
