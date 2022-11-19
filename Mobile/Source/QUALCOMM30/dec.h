#ifndef DEC_H
#define DEC_H

#ifdef T_MSM5000
#include "dec5000.h"      /* This file is replaced for MSM5000 */
#else
/*===========================================================================

           D E C O D E R   ( S V D )    H E A D E R    F I L E

DESCRIPTION
  All the declarations and definitions necessary to support interaction
  with the QUALCOMM CDMA Serial Viterbi Decoder chip

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/dec.h_v   1.3   17 Jan 2000 17:51:48   rmalhotr  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/17/00   rm      Cleaned up TEMP_HACKS_FOR_TIGER.
9/22/99    lcc     Merged in MSM5000 support from PLT archive:
           jcw     Added MSM5000 redirection to dec5000.h
Imaginary Buffer Line
04/15/99   aaj     Function prototype for dec_fcc_intr() function for MDR
02/22/99   aaj     Cosmetic changes in dec_read_tc_data to avoid cplr warns
02/08/99   aaj     Declared function to clear sup metric for standby time improv.
01/15/99   aaj     Merged MDR branch to Panther mainline
11/20/98   aaj     Added support for Medium (High) data rate sup channels
11/05/98   dlb     Added support for MSM3000.
03/02/95   jah     Added support for 14400 rate set and MSM2p decoder mods.
05/28/93   jah     Added prototype for dec_state_zero_ctl().  Removed use
                   of FPOS() macro in DEC_TC_BUF_* definitions, to appease
                   the compiler.
05/11/93   jah     Added dec_chipx8_clk_enable().
02/06/92   jah     Ported module from brassboard dec.h

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "rex.h"        /* definition of REX data types         */
#include "target.h"     /* Target specific definitions          */

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#if defined( T_MSM ) || defined( T_3CHIP )

/* Repeat rates for the paging channel
*/
typedef enum {
  DEC_RPT_1 = 0x00,       /* 1:1 repeat rate, 9600 bps */
  DEC_RPT_2 = 0x02,       /* 1:2 repeat rate, 4800 bps */
  DEC_RPT_4 = 0x04,       /* 1:4 repeat rate, 2400 bps */
  DEC_RPT_8 = 0x06        /* 1:8 repeat rate, 1200 bps */
} dec_repeat_rate_type;

#else

/* Repeat rates for the paging channel
*/
typedef enum {
  DEC_RPT_1 = 0x00,       /* 1:1 repeat rate, 9600 bps */
  DEC_RPT_2 = 0x08,       /* 1:2 repeat rate, 4800 bps */
  DEC_RPT_4 = 0x10,       /* 1:4 repeat rate, 2400 bps */
  DEC_RPT_8 = 0x18        /* 1:8 repeat rate, 1200 bps */
} dec_repeat_rate_type;

#endif

/* Data decoding rate sets for dec_rate_set()
*/
typedef enum {
  DEC_RATE_9600    = 0x00, /* Select 9600 bps rate set   */
  DEC_RATE_14400   = 0x01, /* Select 14400 bps rate set  */
  DEC_RATE_INVALID = 0x02  /* Invalid rate set           */
} dec_rate_set_type;

/* Buffer for holding Symbol Error Rates, used for rate determination.
*/
typedef struct {
  byte status;            /* CRC and Quality bits for all frame sizes  */
  byte ser1;              /* SER for decode as a full rate frame       */
  byte ser2;              /* SER for decode as a 1/2 rate frame        */
  byte ser4;              /* SER for decode as a 1/4 rate frame        */
  byte ser8;              /* SER for decode as a 1/8 rate frame        */
} dec_ser_type;

/* Masks for Status/Quality byte in Symbol Error Rate type
*/
#if defined( T_MSM ) || defined( T_3CHIP )
#define DEC_BYTERDY_M     0x01    /* 1 if decoder byte ready           */
#define DEC_PKTRDY_M      0x02    /* 1 if decoder frame ready          */
#else
#define DEC_CRC8_M        0x01    /* 1 if CRC ok for  1/8 rate frame   */
#define DEC_CRC4_M        0x02    /* 1 if CRC ok for  1/4 rate frame   */
#endif
#define DEC_CRC2_M        0x04    /* 1 if CRC ok for  1/2 rate frame   */
#define DEC_CRC1_M        0x08    /* 1 if CRC ok for full rate frame   */

#define DEC_GOOD8_M       0x10    /* ACS Quality bit  1/8 rate frame   */
#define DEC_GOOD4_M       0x20    /* ACS Quality bit  1/4 rate frame   */
#define DEC_GOOD2_M       0x40    /* ACS Quality bit  1/2 rate frame   */
#define DEC_GOOD1_M       0x80    /* ACS Quality bit full rate frame   */

/* Define bit constants for frame sizes.
*/
#define DEC_192_BITS      192     /* Bits per  9600 bps rate 1   frame */
#define DEC_96_BITS        96     /* Bits per  9600 bps rate 1/2 frame */
#define DEC_48_BITS        48     /* Bits per  9600 bps rate 1/4 frame */
#define DEC_24_BITS        24     /* Bits per  9600 bps rate 1/8 frame */

#define DEC_288_BITS      288     /* Bits per 14400 bps rate 1   frame */
#define DEC_144_BITS      144     /* Bits per 14400 bps rate 1/2 frame */
#define DEC_72_BITS        72     /* Bits per 14400 bps rate 1/4 frame */
#define DEC_36_BITS        36     /* Bits per 14400 bps rate 1/8 frame */

/* Sync Channel data buffer type
*/
#define DEC_SC_BUF_SIZ    (DEC_96_BITS / 8)

typedef byte dec_sc_type[ DEC_SC_BUF_SIZ ];

/* Paging Channel data buffer type, the rate is set dynamically.
** The buffer is sized for a 192 bit (1:1) frame.
*/
#define DEC_PC_BUF_SIZ    (DEC_192_BITS / 8)

typedef byte dec_pc_type[ DEC_PC_BUF_SIZ ];

/* 9600 Traffic Channel data buffer sizes
*/
#define DEC_TC_SIZ_FULL_9600  ((DEC_192_BITS + 7) / 8)   /* Rate 1    */
#define DEC_TC_SIZ_HALF_9600  ((DEC_96_BITS  + 7) / 8)   /* Rate 1/2  */
#define DEC_TC_SIZ_QTR_9600   ((DEC_48_BITS  + 7) / 8)   /* Rate 1/4  */

#ifdef T_MSM3
/* To read modulo-2 number of bytes because DEC_DATA is 16 bit regr now */
#define DEC_TC_SIZ_8TH_9600   ((DEC_24_BITS  + 7) / 8) + 1  /* Rate 1/8  */
#else
#define DEC_TC_SIZ_8TH_9600   ((DEC_24_BITS  + 7) / 8)   /* Rate 1/8  */
#endif /* T_MSM3 */

#define DEC_TC_SIZ_ALL_9600   \
  ( DEC_TC_SIZ_FULL_9600 + DEC_TC_SIZ_HALF_9600 \
  + DEC_TC_SIZ_QTR_9600  + DEC_TC_SIZ_8TH_9600 )

/* 14400 Traffic Channel data buffer sizes
*/
#define DEC_TC_SIZ_FULL_14400 ((DEC_288_BITS + 7) / 8)   /* Rate 1    */
#define DEC_TC_SIZ_HALF_14400 ((DEC_144_BITS + 7) / 8)   /* Rate 1/2  */

#ifdef T_MSM3
/* To read modulo-2 number of bytes because DEC_DATA is 16 bit regr now */
#define DEC_TC_SIZ_QTR_14400  ((DEC_72_BITS  + 7) / 8) + 1 /* Rate 1/4  */
#define DEC_TC_SIZ_8TH_14400  ((DEC_36_BITS  + 7) / 8) + 1 /* Rate 1/8  */
#else
#define DEC_TC_SIZ_QTR_14400  ((DEC_72_BITS  + 7) / 8)   /* Rate 1/4  */
#define DEC_TC_SIZ_8TH_14400  ((DEC_36_BITS  + 7) / 8)   /* Rate 1/8  */
#endif /* T_MSM3 */

#define DEC_TC_SIZ_ALL_14400  \
  ( DEC_TC_SIZ_FULL_14400 + DEC_TC_SIZ_HALF_14400 \
  + DEC_TC_SIZ_QTR_14400  + DEC_TC_SIZ_8TH_14400 )

/* Sum of all sizes for the highest bit rate (currently 14400)
*/
#define DEC_TC_ALL_SIZ        DEC_TC_SIZ_ALL_14400

/* Traffic Channel data buffer type.  This buffer is big enough
** to hold the decoded frames for all data rates.
**
** Note: for convenience of the rate determination algorithm, the
** buffer is expressed as both a group of 4 buffers, and as a sequence
** of bits.  When editing, be careful to make these match.
*/
typedef union {

  /* The buffer expressed as 1 buffer for each rate, 9600 bps rate set
  */
  struct {
    byte bits1[ DEC_TC_SIZ_FULL_9600 ];         /* Rate 1   data bits */
    byte bits2[ DEC_TC_SIZ_HALF_9600 ];         /* Rate 1/2 data bits */
    byte bits4[ DEC_TC_SIZ_QTR_9600  ];         /* Rate 1/4 data bits */
    byte bits8[ DEC_TC_SIZ_8TH_9600  ];         /* Rate 1/8 data bits */
  } buf_9600;

  /* The buffer expressed as 1 buffer for each rate, 14400 rate set
  */
  struct {
    byte bits1[ DEC_TC_SIZ_FULL_14400 ];        /* Rate 1   data bits */
    byte bits2[ DEC_TC_SIZ_HALF_14400 ];        /* Rate 1/2 data bits */
    byte bits4[ DEC_TC_SIZ_QTR_14400  ];        /* Rate 1/4 data bits */
    byte bits8[ DEC_TC_SIZ_8TH_14400  ];        /* Rate 1/8 data bits */
  } buf_14400;

  /* The buffer expressed as a linear sequence of bits
  */
  byte bits[ DEC_TC_ALL_SIZ ];

} dec_tc_type;

/* Indices into 'bits' element of dec_tc_type.
*/
#define DEC_TC_BUF_ALL_9600       0
#define DEC_TC_BUF_FULL_9600      DEC_TC_BUF_ALL_9600
#define DEC_TC_BUF_HALF_9600      (DEC_TC_BUF_FULL_9600 + (DEC_192_BITS / 8))
#define DEC_TC_BUF_QTR_9600       (DEC_TC_BUF_HALF_9600 + ( DEC_96_BITS / 8))
#define DEC_TC_BUF_8TH_9600       (DEC_TC_BUF_QTR_9600  + ( DEC_48_BITS / 8))

#define DEC_TC_BUF_ALL_14400      0
#define DEC_TC_BUF_FULL_14400     DEC_TC_BUF_ALL_14400
#define DEC_TC_BUF_HALF_14400     (DEC_TC_BUF_FULL_14400 + (DEC_288_BITS / 8))
#define DEC_TC_BUF_QTR_14400      (DEC_TC_BUF_HALF_14400 + (DEC_144_BITS / 8))
#ifdef T_MSM3
#define DEC_TC_BUF_8TH_14400      (DEC_TC_BUF_QTR_14400  + ( DEC_72_BITS / 8)+1)
#else
#define DEC_TC_BUF_8TH_14400      (DEC_TC_BUF_QTR_14400  + ( DEC_72_BITS / 8))
#endif /* T_MSM3 */


/*-------------------------------------------------------------------------------------*/
#ifdef T_MSM3
#if  defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR)

#define DEC_BUF_SCC_BASE_9600      (DEC_TC_BUF_ALL_9600   +   \
                                   (DEC_192_BITS / 16)    +   \
                                   (DEC_96_BITS  / 16)    +   \
                                   (DEC_48_BITS  / 16)    +   \
                                   ((DEC_24_BITS + 15) / 16))   

                
#define DEC_BUF_SCC_BASE_14400     (DEC_TC_BUF_ALL_14400 +      \
                                   (DEC_288_BITS / 16)   +      \
                                   (DEC_144_BITS / 16)   +      \
                                   ((DEC_72_BITS + 15) / 16) +  \
                                   ((DEC_36_BITS + 15) / 16))   

#define DEC_SCC_SIZE_9600          (DEC_192_BITS / 16)          /* RS1 Full rate data */
#define DEC_SCC_SIZE_14400         (DEC_288_BITS / 16)          /* RS2 Full rate data */

#define DEC_MAX_SCC_NUM            8                            /* 8 for both RS1 and RS2 */

#define DEC_BUF_SCC1_OFFSET_9600   DEC_BUF_SCC_BASE_9600
#define DEC_BUF_SCC2_OFFSET_9600   (DEC_BUF_SCC_BASE_9600 + 1 * DEC_SCC_SIZE_9600) 
#define DEC_BUF_SCC3_OFFSET_9600   (DEC_BUF_SCC_BASE_9600 + 2 * DEC_SCC_SIZE_9600) 
#define DEC_BUF_SCC4_OFFSET_9600   (DEC_BUF_SCC_BASE_9600 + 3 * DEC_SCC_SIZE_9600) 
#define DEC_BUF_SCC5_OFFSET_9600   (DEC_BUF_SCC_BASE_9600 + 4 * DEC_SCC_SIZE_9600) 
#define DEC_BUF_SCC6_OFFSET_9600   (DEC_BUF_SCC_BASE_9600 + 5 * DEC_SCC_SIZE_9600) 
#define DEC_BUF_SCC7_OFFSET_9600   (DEC_BUF_SCC_BASE_9600 + 6 * DEC_SCC_SIZE_9600) 

#define DEC_BUF_SCC1_OFFSET_14400  DEC_BUF_SCC_BASE_14400
#define DEC_BUF_SCC2_OFFSET_14400  (DEC_BUF_SCC_BASE_14400 + 1 * DEC_SCC_SIZE_14400) 
#define DEC_BUF_SCC3_OFFSET_14400  (DEC_BUF_SCC_BASE_14400 + 2 * DEC_SCC_SIZE_14400) 
#define DEC_BUF_SCC4_OFFSET_14400  (DEC_BUF_SCC_BASE_14400 + 3 * DEC_SCC_SIZE_14400) 
#define DEC_BUF_SCC5_OFFSET_14400  (DEC_BUF_SCC_BASE_14400 + 4 * DEC_SCC_SIZE_14400) 
#define DEC_BUF_SCC6_OFFSET_14400  (DEC_BUF_SCC_BASE_14400 + 5 * DEC_SCC_SIZE_14400) 
#define DEC_BUF_SCC7_OFFSET_14400  (DEC_BUF_SCC_BASE_14400 + 6 * DEC_SCC_SIZE_14400) 



     
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */
#endif /* T_MSM3 */

/* Decode Rates to read
*/
typedef enum {
  DEC_RATE_ALL = 0,     /* Return all data for decode at all data rates */
  DEC_RATE_FULL,        /* Return data for "full" rate only             */
  DEC_RATE_HALF,        /* Return data for "half" rate only             */
  DEC_RATE_QTR,         /* Return data for "quarter" rate only          */
  DEC_RATE_8TH          /* Return data for "eighth" rate only           */
} dec_tc_rate_type;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION DEC_INIT

DESCRIPTION
  Initialize the circuit to Idle, including loading the Symbol Metric Table
  into decoder memory.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_init( void );


/*===========================================================================

FUNCTION DEC_VERSION

DESCRIPTION
  Return the hardware version number for this circuit.

DEPENDENCIES
  None

RETURN VALUE
  Returns the 6-bit version number of the circuit

SIDE EFFECTS
  None

===========================================================================*/
extern byte dec_version( void );


/*===========================================================================

FUNCTION DEC_SC_INIT

DESCRIPTION
  Initialize the circuit for decoding the Sync Channel.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_sc_init( void );


/*===========================================================================

FUNCTION DEC_PC_INIT

DESCRIPTION
  Initialize the circuit for decoding the Paging Channel.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_pc_init
(
  dec_repeat_rate_type repeat_rate      /* Repeat rate for the paging channel */
);


/*===========================================================================

FUNCTION DEC_TC_INIT

DESCRIPTION
  Initialize the circuit for decoding the Traffic Channel.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_tc_init( void );


/*===========================================================================

FUNCTION DEC_SER

DESCRIPTION
  Read from the decoder the frame status and symbol error rates for each
  code type.  The status byte is decoded by masking it with masks defined
  in dec.h.  The error rates are scaled to the range for a 192 bit frame.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_ser
(
  dec_ser_type *ser_ptr         /* pointer to buffer for symbol error rates */
);


/*===========================================================================

FUNCTION DEC_READ_SC_DATA

DESCRIPTION
  Read data from the decoder as Sync Channel data.

DEPENDENCIES
  Dec_sc_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_read_sc_data
(
  dec_sc_type   buf             /* Buffer for Sync Channel data */
);


/*===========================================================================

FUNCTION DEC_READ_PC_DATA

DESCRIPTION
  Read data from the decoder as Paging Channel data.  Data size is taken
  from the repeat rate parameter passed to dec_pc_init().

DEPENDENCIES
  Dec_pc_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_read_pc_data
(
  dec_pc_type   buf             /* Buffer for Paging Channel data */
);


/*===========================================================================

FUNCTION DEC_READ_TC_DATA

DESCRIPTION
  Read data from the decoder as Traffic Channel data.  Note that the data
  for the specified rate is loaded from the beginning of the buffer.  Any
  space at the end of the buffer, which was not requested to be filled,
  is indeterminant.

DEPENDENCIES
  Dec_tc_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_read_tc_data
(
  dec_tc_type      *buf_ptr,    /* Buffer for Traffic Channel data      */
  dec_tc_rate_type  tc_rate     /* Rate decode buffer to read           */
);


/*===========================================================================

FUNCTION DEC_RATE_SET

DESCRIPTION
  Set the rate set to use for decoding data on the Traffic Channel.

DEPENDENCIES
  Dec_tc_init() should have already been called.  Data should not be
  fetched from the decoder while this call is in progress.

RETURN VALUE
  None

SIDE EFFECTS
  If the rate set changes, the SMT and CRC Masks are reloaded.

===========================================================================*/
extern void dec_rate_set
(
  dec_rate_set_type rate_set    /* Rate set to use for decoding data */
);


/*===========================================================================

FUNCTION DEC_CHIPX8_CLK_DISABLE

DESCRIPTION
  Disable the CHIPX8 clock on the Decoder.  Disabling the CHIPX8 clocking
  saves power while the Decoder is not in use.  The chip state is preserved
  at the time the clocks are disabled.  When the clock is re-enabled, by
  calling dec_init(), the circuit state is re-synchronized after two frame
  interrupts from the Decoder.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Shuts off the chip.

===========================================================================*/
extern void dec_chipx8_clk_disable( void );


/*===========================================================================

FUNCTION DEC_CHIPX8_CLK_ENABLE

DESCRIPTION
  Enable the CHIPX8 clock on the Decoder.  The chip state is preserved
  from when the chipx8 was disabled.  The circuit state is re-synchronized
  after two frame interrupts.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Turns on the chip.

===========================================================================*/
extern void dec_chipx8_clk_enable( void );


/*===========================================================================

FUNCTION DEC_STATE_ZERO_CTL

DESCRIPTION
  Cause the decoder to decode all input as zeroes, in order to clear-out
  path memory.

DEPENDENCIES
  Should be called 'true' >11ms before the symbol combiner slam when the
  Decoder wakes up during slotted mode, and 'false' following the symbol
  combiner slam.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_state_zero_ctl
(
  boolean set  /* TRUE = set into state 0, FALSE = normal operation */
);

#ifdef FEATURE_ENHANCED_STANDBY_II
/*===========================================================================

FUNCTION DEC_CLEAR_STATE_METRICS

DESCRIPTION
  Completely clears all the decoder state metrics, leaving the
  Viterbi decoder with no assumptions about prior frames. Note
  that this will not lead to as good a performance as if the
  decoder received a good precharge frame, but it is a good
  place to start in extreme circumstances when it is known
  that the previous frame's symbols are not good.

DEPENDENCIES
  May only be called when the decoder in not running.  For example, after a
  decoder interrupt but before the next 20ms frame boundary.
  
  After the decoder interrupt, normal operation must be selected.
  
  NOTE - This function is valid for MSM3000 only.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void dec_clear_state_metrics( void );
#endif /* FEATURE_ENHANCED_STANDBY_II */

#if  defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR)
/*===========================================================================

FUNCTION DEC_READ_SCC_DATA

DESCRIPTION
  Read data from the Decoder as Supplemental Code Channel data.  Note that the 
  data for SCC is FULL rate only and is loaded after the fundamental channel data
  (four possible rates). It reads data for n supplemental channels.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_read_scc_data
(
  word              *scc_bits,    /* Buffer for Sup Code Channel data        */
  byte              sup_num,     /* Number of active supplemental channels */
  dec_rate_set_type rate_set     /* Rate set */
);

/*===========================================================================

FUNCTION DEC_SCC_NUM_SUP

DESCRIPTION
  This function reads number of supplemental channels read from the decoder.

DEPENDENCIES
  None

RETURN VALUE
  num_sup

SIDE EFFECTS
  None

===========================================================================*/
extern byte dec_scc_num_sup( void );

/*===========================================================================

FUNCTION DEC_SCC_CRC

DESCRIPTION
  This function reads the CRC for all the 7 sup channels
  Each bits stands for each supplemental channels. It is 1 if the CRC matches.

DEPENDENCIES
  None

RETURN VALUE
  num_sup

SIDE EFFECTS
  None

===========================================================================*/
extern byte dec_scc_crc( void );

/*===========================================================================

FUNCTION DEC_SCC_SER

DESCRIPTION
  This function reads supplemental channel SER from the decoder.

DEPENDENCIES
  None

RETURN VALUE
  num_sup

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_scc_ser ( byte *ser_array );

/*===========================================================================

FUNCTION DEC_FCC_INTR

DESCRIPTION
  This function returns zero if the decoder mdr_results register was read in
  in supplemental channel interrupt and returns 0x20 if it is read in fundamental
  channel interrupt. 

  DEC_MODE:FUND_INTR bit should be set to be able to get two separate interrupts 
  for fundamental and supplemental channels.

DEPENDENCIES
  None

RETURN VALUE
  byte

SIDE EFFECTS
  None

===========================================================================*/
extern byte dec_fcc_intr( void );

#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */

#endif /* T_MSM5000 */
#endif  /* DEC_H */
