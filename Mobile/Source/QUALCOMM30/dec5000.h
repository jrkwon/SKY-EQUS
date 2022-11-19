#ifndef DEC5000_H
#define DEC5000_H

#include "comdef.h"
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

$Header:   L:/src/asw/MSM5000/VCS/dec5000.h_v   1.12   06 Oct 2000 17:16:18   bcalder  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/06/00   bgc      Removed special dec_read_sch_data() for FTM (not needed)
09/06/00   rm       Added new function prototype of dec_read_sch_data
                    for Factory Test Mode.
07/12/00   rm       Added suuport for for 5ms frames and MULTIFRAME
                    INTERLEAVING. 
05/18/00   et	    added prototype for dec_dtx_thresh_init
03/24/00   bgc      Changes energy's to signed values.
02/16/00   eh       Changed DEC_MAX_SCC_NUM to 0x07  
01/28/00   rm       Added support for 1X SCH.
01/26/00   rm       Modified the dec_read_sch_data fumction to read
                    each mux pdu into the buffer pointed by the dsm_item.
11/09/99   rm       Added in support for Turbo Codes.
10/20/99   rm       Added in support for IS95B SCCH.
10/01/99   rm       Externalized dec_sch_ob_params parameter.
09/24/99   rm       Added in support for SCH (RC3 & RC4).
07/31/99   rm       Created file. Interfaces to support Serial Viterbi Decoder

============================================================================*/
#include "dsm.h"

#define DEC_FCH_TASK   0x03    /* Decoder Task ID for fundamenntal Channel */
#define DEC_SCCH_SCH_TASK   0x10  /* Decoder Task ID for SCH/SCCH */

/************************************************************************
                              COMMON CONSTANTS
************************************************************************/

#define VD_INTIMODE_SATURATE_STATE_V 0x01 << 1
#define VD_CONTINOUS_V 0x00
/* Decoder in Continous Mode fpr SYNC and Paging Channel */
#define VD_PACKET_V 0x01
/* Decoder in Packet Mode for Traffic Channel */


#define DEC_192_BITS      192     /* Bits per  9600 bps rate 1   frame */
#define DEC_96_BITS        96     /* Bits per  9600 bps rate 1/2 frame */
#define DEC_48_BITS        48     /* Bits per  9600 bps rate 1/4 frame */
#define DEC_24_BITS        24     /* Bits per  9600 bps rate 1/8 frame */

#define DEC_54_BITS       54     /* For RC3 and RC4 9600 bps rate 1/4 & 1/8 
                                     have 6 bits CRC */
#define DEC_30_BITS       30  

#define DEC_288_BITS      288     /* Bits per 14400 bps rate 1   frame */
#define DEC_144_BITS      144     /* Bits per 14400 bps rate 1/2 frame */
#define DEC_72_BITS        72     /* Bits per 14400 bps rate 1/4 frame */
#define DEC_36_BITS        36     /* Bits per 14400 bps rate 1/8 frame */


/* CRC + Quality Bit Indicator */
#define DEC_CRC8_M        0x01    /* 1 if CRC ok for  1/8 rate frame   */
#define DEC_CRC4_M        0x01    /* 1 if CRC ok for  1/4 rate frame   */
#define DEC_CRC2_M        0x01    /* 1 if CRC ok for  1/2 rate frame   */
#define DEC_CRC1_M        0x01    /* 1 if CRC ok for full rate frame   */

#define DEC_GOOD8_M       0x02    /* ACS Quality bit  1/8 rate frame   */
#define DEC_GOOD4_M       0x02    /* ACS Quality bit  1/4 rate frame   */
#define DEC_GOOD2_M       0x02    /* ACS Quality bit  1/2 rate frame   */
#define DEC_GOOD1_M       0x02    /* ACS Quality bit full rate frame   */   


/*****************************************************************************
                              SYNC CHANNEL 
*****************************************************************************/

/* SYNC Channel Data Buffer size */
#define DEC_SC_BUF_SIZ    (DEC_96_BITS / 8)    

/* SYNC Channel Data Buffer Type */
typedef byte dec_sc_type[ DEC_SC_BUF_SIZ ];

/* Status information for Sync chanel */
typedef struct {             
 byte status;                
 byte ser;                   /* Symbol Error Rate */
 int2 energy;                /* Frame Energy based on SER Calculations */
} dec_sc_status_type;   /* For Sync Channel */



/*****************************************************************************
                              PAGING CHANNEL 
*****************************************************************************/


/* Paging Channel data buffer type, the rate is set dynamically.
** The buffer is sized for a 192 bit (1:1) frame.
*/
#define DEC_PC_BUF_SIZ    (DEC_192_BITS / 8)

typedef enum
{

   DEC_PRAT_9600=0,   /* Paging Channel Rate 9600 bps */
   DEC_PRAT_4800      /* paging Channel Rate 4800 bps */

} dec_pc_rate_type;

/* Paging Channel Buffer */
typedef byte dec_pc_type[ DEC_PC_BUF_SIZ ];

/* Status information for Paging Channel */
typedef struct {
 byte status;
 byte ser;                   /* Symbol Error Count */
 int2 energy;                /* Frame Energy based on SER calculations */
} dec_pc_status_type;   /*  Paging Channel */

typedef enum 
{
  DEC_20MS_FRAME=0,
  DEC_40MS_FRAME,
  DEC_80MS_FRAME
} dec_frame_type;

/*****************************************************************************
                              FUNDAMENTAL CHANNEL 
*****************************************************************************/

/* RC1 FCH Size for different rates */
#define DEC_FCH_SIZ_FULL_RC1 ((DEC_192_BITS + 7) / 8)
#define DEC_FCH_SIZ_HALF_RC1 ((DEC_96_BITS  + 7) / 8)
#define DEC_FCH_SIZ_QTR_RC1 ((DEC_48_BITS  + 7) / 8) 
#define DEC_FCH_SIZ_8TH_RC1 ((DEC_24_BITS +7) / 8) + 1

#define DEC_FCH_SIZ_ALL_RC1   \
  ( DEC_FCH_SIZ_FULL_RC1 + DEC_FCH_SIZ_HALF_RC1 \
  + DEC_FCH_SIZ_QTR_RC1  + DEC_FCH_SIZ_8TH_RC1 )


/* RC2 FCH Size for different rates */
#define DEC_FCH_SIZ_FULL_RC2 ((DEC_288_BITS + 7) / 8)
#define DEC_FCH_SIZ_HALF_RC2 ((DEC_144_BITS  + 7) / 8)
#define DEC_FCH_SIZ_QTR_RC2 ((DEC_72_BITS  + 7) / 8) + 1
#define DEC_FCH_SIZ_8TH_RC2 ((DEC_36_BITS +7) / 8) + 1

#define DEC_FCH_SIZ_ALL_RC2   \
  ( DEC_FCH_SIZ_FULL_RC2 + DEC_FCH_SIZ_HALF_RC2 \
  + DEC_FCH_SIZ_QTR_RC2  + DEC_FCH_SIZ_8TH_RC2 )


/* RC3 FCH Size for different rates */
#define DEC_FCH_SIZ_FULL_RC3 ((DEC_192_BITS + 7) / 8)
#define DEC_FCH_SIZ_HALF_RC3 ((DEC_96_BITS  + 7) / 8)
#define DEC_FCH_SIZ_QTR_RC3 ((DEC_54_BITS  + 7) / 8) + 1
#define DEC_FCH_SIZ_8TH_RC3 ((DEC_30_BITS +7) / 8) 

#define DEC_FCH_SIZ_ALL_RC3   \
  ( DEC_FCH_SIZ_FULL_RC3 + DEC_FCH_SIZ_HALF_RC3 \
  + DEC_FCH_SIZ_QTR_RC3  + DEC_FCH_SIZ_8TH_RC3 )

/* RC4 FCH Size for different rates */
#define DEC_FCH_SIZ_FULL_RC4 ((DEC_192_BITS + 7) / 8)
#define DEC_FCH_SIZ_HALF_RC4 ((DEC_96_BITS  + 7) / 8)
#define DEC_FCH_SIZ_QTR_RC4 ((DEC_54_BITS  + 7) / 8) + 1
#define DEC_FCH_SIZ_8TH_RC4 ((DEC_30_BITS +7) / 8)

#define DEC_FCH_SIZ_ALL_RC4   \
  ( DEC_FCH_SIZ_FULL_RC4 + DEC_FCH_SIZ_HALF_RC4 \
  + DEC_FCH_SIZ_QTR_RC4  + DEC_FCH_SIZ_8TH_RC4 )


/* RC5 FCH Size for different rates */
#define DEC_FCH_SIZ_FULL_RC5 ((DEC_288_BITS + 7) / 8)
#define DEC_FCH_SIZ_HALF_RC5 ((DEC_144_BITS  + 7) / 8)
#define DEC_FCH_SIZ_QTR_RC5 ((DEC_72_BITS  + 7) / 8) + 1
#define DEC_FCH_SIZ_8TH_RC5 ((DEC_36_BITS +7) / 8) + 1

#define DEC_FCH_SIZ_ALL_RC5   \
  ( DEC_FCH_SIZ_FULL_RC5 + DEC_FCH_SIZ_HALF_RC5 \
  + DEC_FCH_SIZ_QTR_RC5 + DEC_FCH_SIZ_8TH_RC5 )

/* Decoder Output Buffer offset for FCH RC1 for different rates 
 (Full, Half, Quarter & Eighth ) */
 
#define DEC_FCH_FULL_OB_ADDR_RC1   0
#define DEC_FCH_HALF_OB_ADDR_RC1 \
  (DEC_FCH_FULL_OB_ADDR_RC1 + DEC_FCH_SIZ_FULL_RC1 + 4)
#define DEC_FCH_QTR_OB_ADDR_RC1 \
  (DEC_FCH_HALF_OB_ADDR_RC1 + DEC_FCH_SIZ_HALF_RC1 + 4)
#define DEC_FCH_8TH_OB_ADDR_RC1 \
  (DEC_FCH_QTR_OB_ADDR_RC1 + DEC_FCH_SIZ_QTR_RC1 + 4)

/* Decoder Output Buffer offset for FCH RC2 for different rates 
 (Full, Half, Quarter & Eighth ) */

#define DEC_FCH_FULL_OB_ADDR_RC2   0
#define DEC_FCH_HALF_OB_ADDR_RC2 \
  (DEC_FCH_FULL_OB_ADDR_RC2 + DEC_FCH_SIZ_FULL_RC2 + 4)
#define DEC_FCH_QTR_OB_ADDR_RC2 \
  (DEC_FCH_HALF_OB_ADDR_RC2 + DEC_FCH_SIZ_HALF_RC2 + 4)
#define DEC_FCH_8TH_OB_ADDR_RC2 \
  (DEC_FCH_QTR_OB_ADDR_RC2 + DEC_FCH_SIZ_QTR_RC2 + 4)


/* Decoder Output Buffer offset for FCH RC3 for different rates 
 (Full, Half, Quarter & Eighth ) */

#define DEC_FCH_FULL_OB_ADDR_RC3   0
#define DEC_FCH_HALF_OB_ADDR_RC3 \
  (DEC_FCH_FULL_OB_ADDR_RC3 + DEC_FCH_SIZ_FULL_RC3 + 4)
#define DEC_FCH_QTR_OB_ADDR_RC3 \
  (DEC_FCH_HALF_OB_ADDR_RC3 + DEC_FCH_SIZ_HALF_RC3 + 4)
#define DEC_FCH_8TH_OB_ADDR_RC3 \
  (DEC_FCH_QTR_OB_ADDR_RC3 + DEC_FCH_SIZ_QTR_RC3 + 4)


/* Decoder Output Buffer offset for FCH RC4 for different rates 
 (Full, Half, Quarter & Eighth ) */

#define DEC_FCH_FULL_OB_ADDR_RC4   0
#define DEC_FCH_HALF_OB_ADDR_RC4 \
  (DEC_FCH_FULL_OB_ADDR_RC4 + DEC_FCH_SIZ_FULL_RC4 + 4)
#define DEC_FCH_QTR_OB_ADDR_RC4 \
  (DEC_FCH_HALF_OB_ADDR_RC4 + DEC_FCH_SIZ_HALF_RC4 + 4)
#define DEC_FCH_8TH_OB_ADDR_RC4 \
  (DEC_FCH_QTR_OB_ADDR_RC4 + DEC_FCH_SIZ_QTR_RC4 + 4)


/* Decoder Output Buffer offset for FCH RC5 for different rates 
 (Full, Half, Quarter & Eighth ) */

#define DEC_FCH_FULL_OB_ADDR_RC5   0
#define DEC_FCH_HALF_OB_ADDR_RC5 \
  (DEC_FCH_FULL_OB_ADDR_RC5 + DEC_FCH_SIZ_FULL_RC5 + 4)
#define DEC_FCH_QTR_OB_ADDR_RC5 \
  (DEC_FCH_HALF_OB_ADDR_RC5 + DEC_FCH_SIZ_HALF_RC5 + 4)
#define DEC_FCH_8TH_OB_ADDR_RC5 \
  (DEC_FCH_QTR_OB_ADDR_RC5 + DEC_FCH_SIZ_QTR_RC5 + 4)

#define DEC_FCH_MAX_SIZ_FULL  DEC_FCH_SIZ_FULL_RC2  
#define DEC_FCH_MAX_SIZ_HALF  DEC_FCH_SIZ_HALF_RC2
#define DEC_FCH_MAX_SIZ_QTR  DEC_FCH_SIZ_QTR_RC2
#define DEC_FCH_MAX_SIZ_8TH  DEC_FCH_SIZ_8TH_RC2
#define DEC_FCH_MAX_SIZ_ALL  DEC_FCH_SIZ_ALL_RC2


/******************************************************************************
                        Supplemental Channel 
******************************************************************************/

/* Buffer Size (in bytes) for IS95C  RC3 SCH 1X (9.6 kbps) */
#define DEC_SCH_SIZ_1X_RC3  (192 / 8)
/* Buffer Size (in bytes) for IS95C  RC3 SCH 2X (19.2 kbps) */
#define DEC_SCH_SIZ_2X_RC3  (384 / 8)
/* Buffer Size (in bytes) for IS95C  RC3 SCH 4X (38.4 kbps) */
#define DEC_SCH_SIZ_4X_RC3  (768 / 8)
/* Buffer Size (in bytes) for IS95C  RC3 SCH 8X (76.8 kbps) */
#define DEC_SCH_SIZ_8X_RC3  (1536 / 8)
/* Buffer Size (in bytes) for IS95C  RC3 SCH 16X (153.6 kbps) */
#define DEC_SCH_SIZ_16X_RC3  (3072 / 8)

/* Buffer Size (in bytes) for IS95C  RC4 SCH 1X (9.6 kbps) */
#define DEC_SCH_SIZ_1X_RC4  (192 / 8)
/* Buffer Size (in bytes) for IS95C  RC4 SCH 2X (19.2 kbps) */
#define DEC_SCH_SIZ_2X_RC4  (384 / 8)
/* Buffer Size (in bytes) for IS95C  RC4 SCH 4X (38.4 kbps) */
#define DEC_SCH_SIZ_4X_RC4  (768 / 8)
/* Buffer Size (in bytes) for IS95C  RC4 SCH 8X (76.8 kbps) */
#define DEC_SCH_SIZ_8X_RC4  (1536 / 8)
/* Buffer Size (in bytes) for IS95C  RC4 SCH 16X (153.6 kbps) */
#define DEC_SCH_SIZ_16X_RC4  (3072 / 8)

/* Buffer Size (in bytes) for IS95C  RC5 SCH 1X (14.4 kbps) */
#define DEC_SCH_SIZ_1X_RC5  (288 / 8)
/* Buffer Size (in bytes) for IS95C  RC5 SCH 2X (28.8 kbps) */
#define DEC_SCH_SIZ_2X_RC5  (576 / 8)
/* Buffer Size (in bytes) for IS95C  RC5 SCH 4X (57.6 kbps) */
#define DEC_SCH_SIZ_4X_RC5  (1152 / 8)
/* Buffer Size (in bytes) for IS95C  RC5 SCH 8X (115.2 kbps) */
#define DEC_SCH_SIZ_8X_RC5  (2304 / 8)
/* Buffer Size (in bytes) for IS95C  RC5 SCH 16X (230.4 kbps) */
#define DEC_SCH_SIZ_16X_RC5  (4608 / 8)



#define DEC_SCCH_RC1_PAYLOAD_SIZE 11   
/* Payload for SCCH RC1 is (192 - 12 -8 -1) = 171 bits. 12 bits for Frame CRC,
   8 tail bits and 1 bit for MM Indicator . Note Paylod is in words */

#define DEC_SCCH_RC2_PAYLOAD_SIZE 17
/* Payload for SCCH RC2 is (288 - 12 -8 -2) = 171 bits. 12 bits for Frame CRC,
   8 tail bits and 2 bits for MM  & EIB Indicator . Note Paylod is in words */


#define DEC_SCC_SIZE_9600          (DEC_192_BITS / 16)  
/* Bits is RC1 channel */
#define DEC_SCC_SIZE_14400         (DEC_288_BITS / 16)
/* Bits is RC2 channel */

#define DEC_MAX_SCC_NUM    0x07  
/* Maximum Number of SCCh supported */
       
/* Decoder Output Buffer offset for SCH and SCCH channel data. 128 points to 
   the Large RAM in Decoder Output Buffer */
#define DEC_SCH_RAM_BASE 128

/* Decoder Output Buffer offset for 5ms frames (FCH) */

#define DEC_5MS_PHASE1_RAM_OFFSET  0x0055
#define DEC_5MS_PHASE2_RAM_OFFSET  0x005A
#define DEC_5MS_PHASE3_RAM_OFFSET  0x005F
#define DEC_5MS_PHASE4_RAM_OFFSET  0x0050

/* HACK FOR SNM */
typedef enum {
  DEC_RATE_9600    = 0x00, /* Select 9600 bps rate set   */
  DEC_RATE_14400   = 0x01, /* Select 14400 bps rate set  */
  DEC_RATE_INVALID = 0x02  /* Invalid rate set           */
} dec_rate_set_type;


#define DEC_FCH_RC_BASE 0x01

typedef enum
{

   DEC_RC1=0x01,
   DEC_RC2,
   DEC_RC3,
   DEC_RC4,
   DEC_RC5
} dec_rc_type;

#define DEC_SCH_RC_BASE 0x03  /* IS95C SCH begins from RC3 */

#define DEC_FCH_RC_INDEX(rc)\
    (rc - DEC_FCH_RC_BASE)

#define DEC_SCH_RC_INDEX(rc)\
    (rc - DEC_SCH_RC_BASE)


/* Decode Rates to read
*/
typedef enum {
  DEC_RATE_ALL = 0,     /* Return all data for decode at all data rates */
  DEC_RATE_FULL,        /* Return data for "full" rate only             */
  DEC_RATE_HALF,        /* Return data for "half" rate only             */
  DEC_RATE_QTR,         /* Return data for "quarter" rate only          */
  DEC_RATE_8TH         /* Return data for "eighth" rate only             */
} dec_fch_rate_type;

/*  Decoder Supplemental Channel Rate */
typedef enum {
  DEC_SCH_RATE_1X,
  DEC_SCH_RATE_2X,
  DEC_SCH_RATE_4X,
  DEC_SCH_RATE_8X,
  DEC_SCH_RATE_16X
} dec_sch_rate_type;

/* Decoding Type : Convolutional /  TURBO */
typedef enum {
  DEC_CONVOLUTIONAL = 0,
  DEC_TURBO
} dec_coding_type;

/* Fundamental Channel buffer type */
typedef union
{
  struct 
  {
     byte bits1[DEC_FCH_MAX_SIZ_FULL];
     byte bits2[DEC_FCH_MAX_SIZ_HALF];
     byte bits4[DEC_FCH_MAX_SIZ_QTR];
     byte bits8[DEC_FCH_MAX_SIZ_8TH];
  } buf;
  byte bits[DEC_FCH_MAX_SIZ_ALL];
} dec_fch_type;

typedef dec_fch_type dec_tc_type;

/* The following structure stores the Output Buffer address for a given RC for 
   all rates */
typedef struct
{
  byte dec_ob_addr_full_rate;
  byte dec_ob_addr_half_rate;
  byte dec_ob_addr_qtr_rate;
  byte dec_ob_addr_8th_rate;
  byte dec_full_rate_siz;
  byte dec_half_rate_siz;
  byte dec_qtr_rate_siz;
  byte dec_8th_rate_siz;
} dec_fch_ob_params_type;


/* Buffer for holding Symbol Error Rates, used for rate determination.
*/
typedef struct {
  byte status[4];            /* CRC and Quality bits for all frame sizes  */
  byte ser[4];              /* SER for decode as a full rate frame       */
  int2 energy[4]; 
} dec_fch_status_type;

/* Supplemental Channel (SCH) Buffer Type. 3072 bits in a 20ms frame is largest for SCH for
   and RC5 */
typedef word dec_sch_type[4608/16];

/* Buffer for holding SCH Outut Buffer addresses */
typedef word dec_sch_ob_params_type;

#define NUM_SCH_RC 3       /* for SCH , RC3, RC4 and RC5 are supported */
#define NUM_SCH_RATES  5  /* Rates Supported are 1X, 2X, 4X, 8X and 16X */
#define NUM_TD_SCH_RATES 4 /* Rates Supported for Turbo Codes 2X,4X,8X & 16X */
extern dec_sch_ob_params_type dec_sch_ob_params[NUM_SCH_RC][NUM_SCH_RATES];



/* Buffer to hold IS95C SCH status */
typedef struct {
  byte status;                /* bit 0 = FRAME CRC, bit 1 = Quality Indicator */
  byte ser;                   /* Symbol Error Rate */
  int2 energy;
  word ltu_crc;               /* Valid only for Convolutional Decoding */
}  dec_conv_sch_status_type;   

typedef struct {
  word min_llr; /* Log Likelihood Ratio */
  int4 energy;  /* 18 bits of energy metric */
  byte num_iteration; /* Indicates the number of iterations run by the TD */
  boolean crc_pass; 
}  dec_turbo_sch_status_type;  

#ifdef FEATURE_MULTIFRAME_INTERLEAVING
#error code not present
#else
typedef struct
{
   dec_rc_type rc;       /* Supplemental Channel Radio Configuration */
   dec_sch_rate_type sch_rate;  /* SCH Rate (2x, 4X, 8X or 16X              */
   boolean mux_pdu_double;  /*  TRUE if double sized PDU */
   dec_coding_type code_type; /* CONVOLUTIONAL or TURBO */
} dec_sch_cfg_type;
#endif /*  FEATURE_MULTIFRAME_INTERLEAVING  */

typedef union {
  dec_conv_sch_status_type conv_sch_status;
  dec_turbo_sch_status_type turbo_sch_status;
} dec_sch_status_type;


extern dec_fch_ob_params_type dec_fch_ob_params[];

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*==========================================================================

FUNCTION DEC_INIT

DESCRIPTION
 Initialize the circuit to Idle. Load the polynomials for  1/2, 1/3,
 1/4 code rate.

DEPENDENCIES
  deint_init() calls dec_init()

RETURN VALUE
 None

SIDE EFFECTS
 None

============================================================================*/
extern void dec_init
(
  void
);

extern void dec_chipx8_clk_disable
(
  void 
);

extern void dec_chipx8_clk_enable
(
  void
);

/*===========================================================================

FUNCTION DEC_VERSION

DESCRIPTION
  Return the hardware version number for this circuit.

DEPENDENCIES
  None

RETURN VALUE
  Returns the version number of the circuit

SIDE EFFECTS
  None

===========================================================================*/

extern byte dec_version
( 
  void
);

/*===========================================================================

FUNCTION DEC_SC_INIT

DESCRIPTION
  Initialize the circuit for decoding the Sync Channel.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  The decoder is put in continous mode.

===========================================================================*/
extern void dec_sc_init(
  void
);

/*===========================================================================

FUNCTION DEC_READ_SC_DATA

DESCRIPTION
  Read data from the Decoder as Sync Channel data.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_read_sc_data
(
  dec_sc_type buffer
  /* Buffer Pointer for storing the sync channel data */
);

/*=========================================================================
FUNCTION DEC_READ_SC_STATUS

DESCRIPTION
  This function reads the Status (SER, Energy Metrics) for Sync Channel frames.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
extern void dec_read_sc_status
(
  dec_sc_status_type *sc_buffer
  /* Pointer to the buffer for Sync Channel Status information */
);

/*===========================================================================

FUNCTION DEC_PC_INIT

DESCRIPTION
  Initialize the circuit for decoding the Paging Channel.

DEPENDENCIES
  Dec_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  The decoder is put in continous mode.

===========================================================================*/
extern void dec_pc_init
(
  dec_pc_rate_type pc_rate
  /* pc_rate =0 indicates Paging Channel Rate of 9600bps, 
    pc_rate =1 indicates 4800 bps */
);

/*===========================================================================

FUNCTION DEC_READ_PC_DATA

DESCRIPTION
  Read data from the Decoder Output Buffer as Paging Channel data.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_read_pc_data
(
  dec_pc_type buffer
  /* pointer to the paging channel buffer, the paging channel data will be 
     stored in this buffer */
  
);

/*=========================================================================
FUNCTION DEC_READ_PC_STATUS

DESCRIPTION
  This function reads the Status (SER, Energy Metrics) for Paging Channel frame

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
extern void dec_read_pc_status
(
  dec_pc_status_type *pc_buffer
  /* pointer to the paging channel status buffer */
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

extern void dec_tc_init
(
  void
);

/*===========================================================================

FUNCTION DEC_READ_FCH_DATA

DESCRIPTION
  Read data from the decoder as Fundamental Channel data.  Note that the data
  for the specified RC is loaded from the beginning of the buffer.  Any
  space at the end of the buffer, which was not requested to be filled,
  is indeterminant.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void dec_read_fch_data
(
  dec_fch_type      *buf_ptr,    
  /* Buffer for Traffic Channel data      */
  dec_rc_type       rc,
  /* Radio Configuration */
  dec_fch_rate_type fch_rate     
  /* Rate decode buffer to read           */
);


/*=========================================================================
FUNCTION DEC_READ_FCH_STATUS

DESCRIPTION
  This function reads the Status (CRC, SER, Energy Metrics) for
  Fundamental Channel frames.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
extern void dec_read_fch_status
(
  dec_rc_type         rc, 
  /* Radio Configuration */
  dec_fch_status_type *fch_status
  /* Pointer to the FCH status buffer */
);


/*===========================================================================

FUNCTION DEC_READ_SCCH_DATA

DESCRIPTION
  Read data from the Decoder as Supplemental(Traffic) Channel data
 (IS95B SCCH).

DEPENDENCIES
  Dec_tc_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void dec_read_scch_data
(
  dec_rc_type       rc,         
  /* RADIO CONFIGURATION */
  word              *buf_ptr,    
  /* Buffer for Supplemental Code Channel data      */
  byte              *scch_ser,
  /* pointer to symbol error rate */
  int2              *scch_energy,
  byte              *scch_crc,
  byte              num_sup     
  /* Number of SCCH */
);


/*===========================================================================

FUNCTION DEC_READ_SCH_DATA

DESCRIPTION
  Read data from the Decoder as Supplemental(Traffic) Channel data
 (IS95C SCH). This function reads each mux pdu (single/double) into the 
  buffer pointed to by the dsm_item.

DEPENDENCIES
  Dec_tc_init() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_PLT
#error code not present
#else
extern void dec_read_sch_data
(
  dec_sch_cfg_type *sch_rec_ptr,  /* Pointer to rxc_sch record pointer */
  dsm_item_type **dsm_item_ptr, /* array of dsm_item pointers */
  byte *mux_pdu_count      /* return the number of MUX PDU read */
);
#endif  /* FEATURE_FACTORY_TESTMODE */
/*=========================================================================
FUNCTION DEC_READ_SCH_STATUS

DESCRIPTION
  This function reads the Status (SER, Energy Metrics) for SCH Channel frames

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
extern void dec_read_sch_status
(
  dec_rc_type rc, 
  dec_sch_rate_type sch_rate, 
  dec_sch_status_type *sch_status,
  dec_coding_type code_type   /* Indicates if Convolutional/Turbo coding */
);

#endif
/*=========================================================================
FUNCTION DEC_DTX_THRESH_INIT

DESCRIPTION
  This function initializes the dtx threshold value depending on the RC and
  the code type.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Writes into the MSM register DEFAULT_IQ_ACC_WH
=============================================================================*/
void dec_dtx_thresh_init(dec_rc_type rc, dec_coding_type code_type, dec_sch_rate_type rate);
