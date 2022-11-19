#ifndef TDSO_H
#define TDSO_H
/*===========================================================================


         T E S T   D A T A   S E R V I C E   O P T I O N

                  H E A D E R    F I L E


DESCRIPTION
  This header file contains functions that are intended to be used internally
  within the simple/Full Test Data Service Options for configuring, managing
  and updating statistics the S-TDSO service option.

        Copyright (c) 1998,2000 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

            EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/tdso.h_v   1.7   23 Mar 2001 18:48:26   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/21/01   lcc     Added function prototype for tdso_end_call and replaced
                   tdso_send_handoff_complete with tdso_notify_hho_success.
03/12/01   fc      Removed "#include msg.h" and "#include err.h".
01/20/01   ks      Fixed PN initialization, ON/OFF period and counter updates.
11/13/00   ks      Added PN and RAND info to the cicular_buffer_info structure.
11/05/00   ks      Added Support for FULL TDSO Service Option.
09/20/00   fc      Added support for TDSO control directives and counter
                   retrieval directives.
08/11/00   va      Changed names of  process_rev_fch_frame and process_rev_sch_frame
                   to process_rx_fch_frame and process_rx_sch_frame respectively.
07/06/00   ks      Created file.

===========================================================================*/

/* <EJECT> */
/*==========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include <memory.h>
#include "snm.h"
#include "cai.h"
#include "mccsch.h"


/*=========================================================================

                 DEFINITIONS AND DECLARATIONS FOR THE MODULE

===========================================================================*/

/* This signal is set when an item is put on the snd_cmd_q */
#define TDSO_CMD_Q_SIG           0x01000

/* This signal is set when the dog_rpt_timer expires */
#define TDSO_RPT_TIMER_SIG       0x0001

/* ---------------------------------------------
** Control / Counter directive field definitions
** --------------------------------------------- */
#define TDSO_CTL_TYPE     0  /* Byte index for control record type field */
#define TDSO_CTL_CODE     1  /* Byte index for control code field        */
#define TDSO_CHN_DIR      2  /* Byte index for channel direction field   */
#define TDSO_VECT_CNT_ID  1  /* Byte index for vector counter id         */

#define MAX_PDU_LEN      45  /* Maximum PDU length, double of Rateset 1  */

/* ------------------
** CTL_REC_TYPE codes
** ------------------ */
typedef enum
{
  TDSO_CTL_ALL = 0,          /* Control directive for all Physical Channels */
  TDSO_CTL_FCH,              /* Control directive for FCH */
  TDSO_CTL_DCCH,             /* Control directive for DCCH */
  TDSO_CTL_SCH0,             /* Control directive for SCH0 */
  TDSO_CTL_SCH1,             /* Control directive for SCH1 */
  TDSO_CNT_FCH,              /* Counter retrieval directive for FCH */
  TDSO_CNT_DCCH,             /* Counter retrieval directive for DCCH */
  TDSO_CNT_SCH0,             /* Counter retrieval directive for SCH0 */
  TDSO_CNT_SCH1              /* Counter retrieval directive for SCH1 */
} tdso_ctl_rec_type;

/* ----------------------------
** FCH / DCCH Vector counter ID
** ---------------------------- */
typedef enum
{
  TDSO_FDCH_FER_CNT = 0,     /* FER counters */
  TDSO_FDCH_RXEX_CNT,        /* Receive expected counter response */
  TDSO_FDCH_TX_CNT,          /* Transmitted counters */
  TDSO_FDCH_5M_TX_CNT,       /* 5 ms frame transmitted counters */
  TDSO_FDCH_5M_RX_CNT        /* 5 ms frame received counters */
} tdso_fdch_vect_cnt_id_type;

/* ---------------------
** SCH Vector counter ID
** --------------------- */
typedef enum
{
  TDSO_SCH_FER_CNT = 0,      /* FER counters response */
  TDSO_SCH_PER_CNT,          /* PER counters response */
  TDSO_SCH_TX_CNT            /* Transmitted counters response */
} tdso_sch_vect_cnt_id;

/* -----------------
** Channel direction
** ----------------- */
typedef enum
{
  FWD_AND_REV_LNK = 0,      /* Both forward and reverse link direction */
  FWD_LNK_ONLY    = 1,      /* Forward link direction only */
  REV_LNK_ONLY    = 2       /* Reverse link direction only */
} tdso_ch_dir_type;

/* ------------
** Frame Source
** ------------ */
typedef enum
{
  TWO_FULL_RATE_FRAME = 0,  /* 2 full-rate frames */
  N_FULL_RATE_FRAME   = 1,  /* N full-rate frames */
  NEW_FRAME           = 2   /* New frame every frame period */
} tdso_fr_src_type;

/* -------------------------------------------------
** Enumeration for FCH / DCCH TDSO frame types
** ------------------------------------------------- */
typedef enum
{
  TDSO_RATE_2,             /* Rate 2 frame type */
  TDSO_RATE_1,             /* Rate 1 Frame type */
  TDSO_RATE_HALF,          /* Half Rate Frame type */
  TDSO_RATE_QUARTER,       /* Quarter Rate Frame type */
  TDSO_RATE_EIGHTH,        /* One Eight Rate Framte type */
  TDSO_BLANK,              /* Blank frame */
  TDSO_ERASURE,            /* Erasures */
  TDSO_DIM_BURST,          /* Dim-and-burst frame type */
  TDSO_BIT_ERROR,          /* Bit errors identified */
  TDSO_INSUFFICIENT_FRAME, /* Insufficient physical frame */
  TDSO_BLANK_BURST,        /* Blank-and-burst frame type */
  TDSO_NULL,               /* Null */
  TDSO_OTHER,              /* Other */
  TDSO_FRAME_TYPES         /* index for tables */
} tdso_rate_type;

/* ------------------------------------------
** Enumeration for SCH receive frame counters
** ------------------------------------------ */
typedef enum
{
  TDSO_SCH_RATE_1X,
  TDSO_SCH_RATE_2X,
  TDSO_SCH_RATE_4X,
  TDSO_SCH_RATE_8X,
  TDSO_SCH_RATE_16X,
  TDSO_SCH_RATE_BLANK,
  TDSO_SCH_RATE_ERASURE,
  TDSO_SCH_BIT_ERROR,
  TDSO_SCH_RATE_OTHER,
  TDSO_SCH_RATE_NONE
} tdso_sch_rate_type;

/* ---------------------------------------------------
** Channel Type, indices for FCH / DCCH / SCH counters
** --------------------------------------------------- */
typedef enum
{
  TDSO_FCH,                 /* Fundamental Channel */
  TDSO_DCCH,                /* Dedicated Common Control Channel */
  TDSO_SCH0,                /* Supplemental Channel 0 */
  TDSO_SCH1,                /* Supplemental Channel 1 */
  TDSO_UNKNOWN              /* Default, not assigned yet */
} tdso_channel_type;

/* --------------------------------------------------------------
** Multiplex Option Type, Only Rate Set 1 is supported in S-TDSO
** ------------------------------------------------------------ */
typedef enum
{
  TDSO_RATE_SET_1,          /* RS1 or Odd-number Multiplex Option */
  TDSO_RATE_SET_2,          /* RS2 or Even-numbered Multiplex Option */
  TDSO_MAX_RATE_SET         /* Maximum rate set, used for indexing */
} tdso_mux_type;

typedef enum
{
  FWD_LINK,
  REV_LINK
} tdso_direction_type;

/* ------------
** Service type
** ------------ */
typedef enum
{
  TDSO_PRIMARY_SRVC = 0,
  TDSO_SECONDARY_SRVC
} tdso_srvc_type;

/* ------------
** MuxPDU types
** ------------ */
typedef enum
{
  TDSO_MUX_PDU_1 = 0, /* Rateset1 for FCH/DCCH/SCCH */
  TDSO_MUX_PDU_2,     /* Rateset2 for FCH/DCCH/SCCH */
  TDSO_MUX_PDU_3,     /* For SCH */
  TDSO_MUX_PDU_MAX
} tdso_mux_pdu_type;

/* -------------------------
** TDSO Function return type
** ------------------------- */
typedef enum
{
  TDSO_VALID = 1,           /* Return value for Valid request */
  TDSO_INVALID_CHAN,        /* Invalid Channel type */
  TDSO_INVALID_SRVC,        /* Invalid Service type */
  TDSO_OUTOF_SYNC,          /* IF TDSO is not synchronized */
  TDSO_NOT_SUPPORTED        /* returns Unsopported frame type */
} tdso_status_type;

/*---------------------------------------------------------------------------
                        TDSO STATISTICS TYPE

The following structure is used to display TDSO statistics.  Currently not
used for S-TDSO implementation.
---------------------------------------------------------------------------*/

typedef struct
{
  tdso_mux_type tdso_rate;       /* current TDSO rate */
  dword fch_bit_errs;           /* FCH bit_errs */
  dword fch_bad_frames;         /* FCH Frame Errors */
  dword fch_total_frames;       /* total number of FCH frames */
  dword sch_bit_errs;           /* SCH bit errors */
  dword sch_bad_frames;         /* SCH Frame Errors */
  dword sch_total_frames;       /* SCH total frames */
  dword fch_tx_stat[ 4 ] [ 10 ]; /* FCH TDSO statistics */
  dword fch_rx_stat[ 4 ] [ 10 ]; /* SCH TDSO statistics */
  dword sch_tx_stat[ 4 ] [ 10 ]; /* FCH TDSO statistics */
  dword sch_rx_stat[ 4 ] [ 10 ]; /* SCH TDSO statistics */
} tdso_stat_type;

/* -----------------------------------
** Types for different array sizes
** ---------------------------------- */
typedef word  tdso_2x5_type[2][5];
typedef dword tdso_2x5_dw_type[2][5];

extern tdso_2x5_type const tdso_bytes;

/* mask last byte to compare bits */
extern tdso_2x5_type const tdso_last_byte;

/* ---------------------------------------------------------
** Table of bit data length for S-TDSO frames
** -------------------------------------------------------- */
extern word tdso_data_len[TDSO_ERASURE][TDSO_MAX_RATE_SET];

/*--------------------------------------------------------------------------
  Type for "state" of a service option. A service option is either ENABLED
  (currently in use) or DISABLED (not in use). If a service option is marked
  as DISABLED, no incoming or outgoing calls can be established using that
  service option number.
----------------------------------------------------------------------------*/
typedef enum
{
  SO_DISABLED, /* Service option not in use */
  SO_ENABLED   /* Service option currently in use */
} tdso_so_state;

/* ------------------------------------------------------------------
** Structure to store SNM parameters - default config and negotiation
** parameters, for each service option.
** ------------------------------------------------------------------ */
typedef struct
{
  const snm_config_type  *default_cfg; /* Default configuration  */
  const snm_neg_par_type *neg_par;     /* Negotiation parameters */
} tdso_snm_param;

/* ---------------------------------------
** Structure to store TDSO test parameters
** --------------------------------------- */
typedef struct
{
  uint8 ch_dir;             /* Channel direction            */
  boolean cpy_cnts;         /* Copy counters                */
  boolean clr_cnts;         /* Clear counters               */
  boolean pseudo_rand_src;  /* Pseudo-random bits as source */
  uint8 frame_src;          /* Frame source                 */
  uint8 rand_frame_act;     /* Random frame activity        */
  uint8 tst_opt;            /* Test options                 */
  uint8 num_circ_buf_frames; /* Num of full-rate frames in circular buffer */
  uint8 on_to_off_prob;     /* ON to OFF state transition probability */
  uint8 off_to_on_prob;     /* OFF to ON state transition probability */
  uint8 tx_on_period;       /* Transmission on period        */
  uint8 tx_off_period;      /* Transmission off period       */
  uint8 data_pattern;       /* Data pattern                  */
} tdso_tst_par_type;

/* ------------------------------------------------------------------------
** Structure to store TDSO operational parameters for Individual Channel.
** Both Forward and Reverse Physical channels have seperate set of parms.
** --------------------------------------------------------------------- */
typedef struct
{
  uint8* temp_circ_buffer;  /* Temporary Circular buffer */
  uint8* circ_buffer;       /* Circular buffer data byte offset */
  uint32 last_seed;         /* last Seed value during RN generation */
  uint32 temp_last_seed;    /* Temporary Last Seed */
  uint32 yn1;               /* Offset is computed by Yn1 mod NUM_RAND */
  uint32 pn;                /* pn used for setting ON/OFF random state */
  boolean pn_valid;         /* This field is used to set the PN value */
  uint16  max_frame_size;   /* Max. # of bits in frame         */
  boolean tdso_sync;        /* This field identifies Synchronization */
  uint16 pn_offset;         /* PN offset generated for every frame */
  uint16 data_offset;       /* Reference pointer for Circular buffer */
  uint16 rc;                /* Radio Configuration for the channel */
  uint16 num_rand;          /* Number of random number to generate */
  boolean tdso_state;       /* Indicates if TDSO state is ON or OFF     */
  uint8 tx_on_period;       /* Transmission on period        */
  uint8 tx_off_period;      /* Transmission off period       */
  uint16 tx_on_period_cnt;  /* Counter for reducing TDSO ON period      */
  uint16 tx_off_period_cnt; /* Counter for reducing TDSO OFF period     */
  boolean rand_frame_act;   /* Random Frame Activity */
  boolean pseudo_rand_src;  /* Pseudo-random bits as source */
  uint8 data_pattern;       /* Data pattern                  */
  uint32 tx_on_thresh;      /* ON Threshold for dynamaic transmission   */
  uint32 tx_off_thresh;     /* OFF Threshold for dynamaic transmission  */
  tdso_tst_par_type tst_par;/* Test Parms specified in Serv cntl mesg   */
}circ_buffer_info;

/* ----------------------------------------------
** Structure to store TDSO channel parameters.
** --------------------------------------------- */
typedef struct
{
  circ_buffer_info pn_info[2]; /* For both FWD and REV directions */
} tdso_chan_parms;

/*-------------------------------------------------------------------------
  Structure of TDSO frames received on the forward link - each frame is put
  into this structure by the Mux and this is passed onto TDSO.
---------------------------------------------------------------------------*/
typedef struct
{
  boolean            valid_frame;     /* TRUE-good frame; FALSE- erasure */
  tdso_srvc_type     srvc_type;       /* Primary/Sec./NA                 */
  tdso_mux_pdu_type  mux_pdu_type;    /* MUX PDU type                    */
  tdso_channel_type  fr_channel;      /* Channel on which frame received */
  tdso_rate_type     fr_rate;         /* Received frame rate             */
  tdso_sch_rate_type sch_rate;        /* SCH frame rate, 1x, 2x etc      */
  uint8              fr_item_ptr[MAX_PDU_LEN]; /* Ptr to frame payload   */
                                      /* Allocate MAX data length */
} tdso_fwd_frame_type;

/* ----------------------------------------------------------------------
** Type definition for TDSO frames to be transmitted on the reverse link.
** ---------------------------------------------------------------------- */
typedef struct
{
  boolean            valid_frame;     /* TRUE-good frame; FALSE- erasure */
  uint16             max_frame_size;  /*  Max. # of bits in frame         */
  tdso_srvc_type     srvc_type;       /*  Primary/Sec./NA                 */
  tdso_mux_pdu_type  mux_pdu_type;    /*  MuxPDU type                     */
  tdso_channel_type  fr_channel;      /*  Channel on which frame received */
  tdso_rate_type     fr_rate;         /*  Current frame rate              */
  tdso_sch_rate_type sch_rate;        /*  SCH frame rate, 1x, 2x etc      */
  uint8              fr_item_ptr[MAX_PDU_LEN]; /* Ptr to frame payload             */
} tdso_rev_frame_type;

/* -----------------------------------------------------------------------
** Type definition for all TDSO frames exchanged between TDSO & MUX layer.
** ----------------------------------------------------------------------- */
#define TDSO_MAX_PDU   9  /* Max # of TDSO frames passed by Mux to TDSO */

typedef struct
{
  uint8         num_frames; /* # of TDSO frames in frame_array; 0 means erasure */
  dword         fr_time;    /* MUX: system time - used for logging              */
  tdso_mux_type fr_mux;     /* Rate Set 1 or Rate set 2        */
  struct
  {
    tdso_fwd_frame_type fwd_arr[ TDSO_MAX_PDU];
    tdso_rev_frame_type rev_arr[ TDSO_MAX_PDU];
  } frame_type;
                            /* array of rx'ed TDSO frames  */
} tdso_frame_block_type;

/*===========================================================================

FUNCTION tdso_task

DESCRIPTION
  This procedure is the entrance procedure for the TDSO Task.  It
  contains the main processing loop for the TDSO Task.

DEPENDENCIES

RETURN VALUE
  Does not return

SIDE EFFECTS
  snd_tcb

===========================================================================*/
extern void tdso_task
(
  dword ignored
    /* Parameter received from Main Control task - ignored */
);


/*===========================================================================
FUNCTION   TDSO_BUILD_TX_FRAMES

DESCRIPTION
  This function is used by the Multiplex layer to fetch the FCH or SCH
  Channel transmission TDSO frame buffers.  For simple TDSO the buffer is
  padded with all '1's.

DEPENDENCIES
  None.

RETURN VALUE

  This function returns the FCH frame based on the TDSO frame type requested.

SIDE EFFECTS
  None
===========================================================================*/
tdso_status_type tdso_build_tx_frames
(
  tdso_direction_type direction,
  tdso_frame_block_type *rev_fr_blk     /* Ptr to frames to trasmit   */
);

/*===========================================================================
FUNCTION     TDSO_PROCESS_RX_FRAME

DESCRIPTION
  This function is used by the Multiplex layer to put the received FCH
  or SCH TDSO frame.

DEPENDENCIES
  None.

RETURN VALUE

  None

SIDE EFFECTS
  only FCH and SCH are supported in TDSO.
===========================================================================*/
tdso_status_type tdso_process_rx_frames
(
  tdso_frame_block_type *fwd_frame_block /* Ptr to Fwd TDSO frames block */
);

/*===========================================================================
FUNCTION PROCESS_RX_FCH_TDSO

DESCRIPTION
  This function processes the Received FCH TDSO frame(s).

  Input Parameters:
    tdso_fwd_frame_type *.  Pointer to the received SCH TDSO frame.

DEPENDENCIES

RETURN VALUE
  None


SIDE EFFECTS
  None
===========================================================================*/
tdso_status_type process_rx_fch_tdso
(
  tdso_frame_block_type  *tdso_fch_block
);

/*===========================================================================
FUNCTION PROCESS_RX_SCH_TDSO

DESCRIPTION
  This function processes the Received SCH TDSO frame(s).

  Input Parameters:
    tdso_fwd_frame_type *.  Pointer to the received SCH TDSO frame.

DEPENDENCIES

RETURN VALUE
  None


SIDE EFFECTS
  None
===========================================================================*/
tdso_status_type process_rx_sch_tdso
(
  tdso_frame_block_type *fwd_frame_block /* Ptr to Fwd TDSO frames block */
);

void tdso_sch_chng_cback( mccsch_sch_info_type* sch_info);

/*******************************************************************
*
* FUNCTION : TDSO_LOG_STATS
*
* DESCRIPTION:
*  This function calls either the stdso_log_stats() function or the
*  ftdso_log_stats() function based on the type of Service option.
*  Also this is called by MC task during the call tear down. This
*  provides an abstration to the type of SO in action and the logging
*  dependencies.
*
* DEPENDENCIES:
*
*
* RETURN VALUE:
*   None.
*
* SIDE EFFECTS:
*
*******************************************************************/
void tdso_log_stats( void );

/*===========================================================================

FUNCTION     TDSO_END_CALL

DESCRIPTION
  This function is called by MC in the release substate.  It does some
  cleanup needed when ending a TDSO call.

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
extern void tdso_end_call(void);


/*===========================================================================
FUNCTION TDSO_INIT_SO_CFG

DESCRIPTION
  This function performs the power-up initialization for the Simple TDSO
  service option. This includes registering the TDSO with the SNM and
  enabling the appropriate service options.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdso_init_so_cfg( void );


/*******************************************************************
*
* FUNCTION tdso_initializer
*
* DESCRIPTION:
*   This function will initialize the TDSO information
*   On a per call basis. This initializer function is registered with
*   the service negotiation manager, so that it will be called when
*   the service option has been initialized.
*
*   Note the two arguments are required to be compatible with
*   the service negotiation manager design, but are not used.
*
* DEPENDENCIES:
*   None.
*
* RETURN VALUE:
*   None.
*
* SIDE EFFECTS:
*   The internal tables will be reset during initialization
*******************************************************************/
void tdso_initializer
(
  const caii_srv_cfg_type* cfg, /* Service configuration info,
                                   Not used by this function, but
                                   it is required to be passed in
                                   by the initilizer */
  word connect_number             /* Connection number */
);

/*===========================================================================

FUNCTION TDSO_UPDATE_TEST_PARAMETER

DESCRIPTION
  This function validates the input TDSO test parameters, It updates the
  corresponding channel's TDSO test parameters if the input is valid.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if update success.
  FALSE if update fails because of invalid parameters.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdso_update_test_parameter(uint8 *tsf, uint8 tsf_len);

/*===========================================================================
FUNCTION     TDSO_NOTIFY_HHO_SUCCESS

DESCRIPTION
  This function is called by MC when a HHO has been successfully performed.
  If a full TDSO is in progress, this function sets the current TDSO state
  to FALSE so that all 1's are transmitted util the next SYNC frame.  The TDSO
  also sends the Service Option Control Message (SOCM) of the test parameters
  that were used before the Handoff.  This is a full TDSO requirement.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdso_notify_hho_success( void );

/*===========================================================================

FUNCTION TDSO_GET_VECT

DESCRIPTION
  This function updates a given table with the appropiate TDSO statistics.

DEPENDENCIES
  None

RETURN VALUE
  Length of vector (vector elements are dword here).

SIDE EFFECTS
  None
===========================================================================*/
uint8 tdso_get_vect(uint8  ctl_rec_type, uint8  vect_counter_id, uint32 *tsf);

#endif /* TDSO_H */
