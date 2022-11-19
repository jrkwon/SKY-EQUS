#ifndef CAI_H
#define CAI_H
/*===========================================================================

       TIA/EIA-95-B   P R O T O C O L   D A T A    D E C L A R A T I O N S

DESCRIPTION

  This file contains the data structures for messages received and
  transmitted to the base station while the mobile station is operating
  in CDMA mode and other CDMA-related constants and indicators as
  defined in IS-95A.

Copyright (c) 1990,1991,1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/cai.h_v   1.11.1.1   02 May 2001 17:19:42   nbouchar  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/02/01   nxb(va) Added cai_pilot_rec_type enum.
04/19/01   lcc     Added CAI_DEF_CONFIG_USE_ASSGN_RC.
03/06/01   fc      Added enumeration cai_tx_channel_id_type, 
                   cai_rx_channel_id_type, cai_timer_id_type and 
                   cai_counter_id_type.
03/01/01   lad     Changed CAI_LAST_PARM_ID to reflect IS2000 values.
01/24/01   ych     Merged T53 and JCDMA features.
11/29/00   ak      Added new XTRA parms for LTU CRC statistics.
10/30/00   ks      Fixed the CAI_SO_FULL_TDSO from 0x032 to 0x20.
10/19/00   ks      Added Service Option for CAI_SO_FULL_TDSO. 
10/02/00   yll     Added CAI_POSITION_DET burst type. Removed msid_hdr_type.
09/19/00   lcc     Corrected number of bits in rev_sch_id and for_sch_id in
                   reverse traffic SCR to 2 for BRV.
08/31/00   lcc     Added reserved bits in nn-SCR for BRV version of CDMA 2000.
07/26/00   ak      Added #defines for IS2000 RS1 Mux options.
07/21/00   fc      Changed FPC mode definitions to use enumerated type.
                   Changed power control step size to use enumerated type.
                   Added SCH rate definition.
07/21/00   va      Added cai_sch_rate_type enum.
07/17/00   va      Added CAI_DEFAULT_REV_PWR_CNTL_DELAY
07/17/00   jq      Added CAI_REL_SRV_INACT definition
07/10/00   ks      Added CAI_SO_TDSO service option for TDSO support.
07/06/00   yll     Added support for GPS.
07/06/00   jq      Added definititions for ECCLM and EGSRM. Modified CFS_RSP
                   for addendum compliance.
06/21/00   fc      Renamed MAX_SCRM_BLOB_SIZ to CAI_MAX_SCRM_BLOB_SIZ,
                   MAX_SCRM_ACT_PN to CAI_MAX_ACT_PN and MAX_SCRM_NGHBR_PN to
                   CAI_MAX_SCRM_NGHBR_PN.
                   Added CAI_MAX_SCRM_RPT_PN and retry type definitions.
06/20/00   cah     Remerged in changes that were lost in last checkin.
           jrw     Corrected a problem with power control message where the
                   init_setpt shouldn't have been included with the SCH set points.
06/19/00   lad     Updated IS2000 support for use in PARM database.
06/12/00   ks      Added support for align_timing fields in CFSREQ message.
06/09/00   ak      Added statistics for IS2000 MUX SCH0.
06/02/00   lcc/jrw Changes to support IS2000 Rev 0 published addendum.  Also
                   added some constant definitions.
05/25/00   ks      Added support for Neighbor SRCH window offset.
05/04/00   va      Changed N1M value to 13 for IS2000 targets.
04/24/00   lcc/jrw Added ESCAM, outer loop report message, and changes to PMRM.
04/17/00   va      Added constant definition of L2 length field.
04/06/00   va      Merged the following from MSM3100_CP.03.00.17
           ry      Added support for OTAPA (IS-683A SO 18 and 19)
03/10/00   ry      Added IS2000 featurization
           ry      Replaced FEATURE_SILK with FEATURE_MSG_PACK_UNIT_TEST
           ry      Merged the following from PP (MSM3100_CP.03.00.10):
           fc      Added support for IS-733 service option 17.
03/06/00   lcc     Added definitions for maximum rates for SCH.
02/29/00   jrw     Fixed CAI_UNIV_HO_DIR_MSG (0x24 -> 0x22)
02/11/00   jrw     Added support for IS2000 Addendum
12/17/99   jrw     Added support for IS2000 Phase 1.
11/30/99   na      Added 1x Pkt Service option - 33.
10/20/99   lcc     Added max. mux options for MSM5000.  Corrected max number of
                   supplementals from 8 to 7.
09/16/99   lcc     Merged in radio configurations definitions from PLT archive.
09/02/99   nmn     Included mux values 3-16 always to enable cleaner DS SO CFG
                   code, for non-MDR builds.
08/17/99   kk      Modified the service redirection message for TC.
08/13/99   jq      Added support for incoming call forwarding enhancement.
                   FEATURE_IS95B_INC_CALL_FWD_ENH
08/13/99   doj     Unincluded new reject reasons for FEATURE_SPECIAL_MDR to
                   be consistent with original FEATURE_SPECIAL_MDR.
08/06/99   doj     Added masks for sign extension of add_intercept and
                   drop interecept for handoff soft slope equation.
07/09/99   doj     Included a few more reject reasons under FEATURE_SPECIAL_MDR
                   and added service negotiation types (Merge from HHO_DEV)
06/17/99   kmp     Added timer t72 to the cai_tmo_type structure. Changed
                   values of CAI_DEF_CONFIG_FOR1_REV2 and CAI_DEF_CONFIG_FOR2_REV1.
06/02/99   kmp     Merge IS-95B changes listed below from SPSW code base.
           lh      Support release order on access channel, name change
                   from reserve to auth_mode.
           kk      NDSS literals defined.
           lh      Updated for Published version of IS-95B.
           fc      Renamed CAI_MAX_PILOTS to CAI_N13M.
           fc      Put in changes based on input from code review.
           jq      Modified CAI_GRANT_MUX1 to CAI_GRANT_USE_DC
           fc      Fixed lint error.
           kmp     Merged in IS-95B additions listed below from TGP baseline
                   and ported IS-95B changes to ARM
                   Following are the IS-95B additions:
           jq      Added ECAM/CAM default_config definitions.
                   CAI_DEF_CONFIG_xxxxxx
           lh      Fixed msg format error in FTC service redir msg.
           fc      Changed CAI_MAX_ADD_PILOTS to 5. Added CAI_MAX_PILOTS.
                   Changed FEATURE_IS95B_AHO to FEATURE_IS95B_ACCESS_HO.
           ks      Added a new timer T72 for IS95B AHO & APHO.
           lh      FEATURE_JSTD008 replaced with run-time P_REV and Band
                   Class checking.
           lh      Modified ECAM. Now parser will parse the ECAM and pass the
                   caller a packed internal format.
           lh      Added comments.
           lh      Added comments.
           jcw     Added defines for 95B Extended Display support
           lh      Change soft_slop to soft_slope in ESPM processing.
           jcw     Parsing of extended display record has been updated to
                   not treat blank & skip subrecords specially.
           lh      Define max length for pilot report external format.
           lh      IS-95B support.
05/25/99   lcc     Corrected pref_msid_type field in extended system parameters
                   message so that it is 2 bits for no-TMSI (IS-95-A) builds
                   instead of 3.
05/19/99   doj     Consolidated structures for HDM, EHDM and GHDM into one
                   generic handoff structure.
03/30/99   ram     Verified and check-ed in changes made for MDR
                   DM support by aaj.
01/27/99   yll     Fixed CR7630. Added CAI_RS1_RLP to comply with IS707.4.
02/16/99   doj     Added PACKED qualifier to cai_redirect_rec1_type.
02/13/99   doj     Added support for General Handoff Direction Message (GHDM).
                   Also includes several misc. updates by many people to
                   support FEATURE_SPECIAL_MDR.
02/13/99   doj     Added PACKED qualifier to cai_rtc_rej_type and
                   cai_orig_c_var_type
02/10/99   nmn     Added FEATURE_SPECIAL_MDR support.  Also changed below year
                   from 00 to 99.
01/27/99   ck      Added FEATURE_EVRC_SO_MGMT around CAI_SO_WILDCARD.
01/27/99   nmn     Added MDR PARAMER_IDs and #defined MAX_MUX for RS1 and RS2
01/16/99   ram     Merged in the changes from IS95B MDR Branch
12/11/98   lcc     Corrected CAI_MAX_SUP_PILOTS to be defined as CAI_N6M (6)
                   instead of 7.
12/01/98   ram     Added MDR DPT Service Options
11/27/98   lcc     Added definitions for MDR related service options and mux.
                   options.
10/20/98   ldg     Added SO numbers for analog end to end fax (IS-707A).
10/09/98   ck      Added CAI_SO_VOICE_WILDCARD for EVRC Service Negotiation
09/01/98   lcc     Merged in ARM support.
08/25/98   ck      Added CAI_SO_RS2_SMS Service Option.
08/12/98   lh      IS95B WLL support: Line Control, Parametric Alerting and
                   Meter Pulses.
06/15/98   ck      Changed CAI_SO_EVRC to CAI_SO_VOICE_EVRC
04/10/98   ldg     Fixed IS-707 RS1 SO numbers.  Specified as hex in SPP,
                   these were actually decimal per PN-3676.1.
02/02/98   day     Consolidating feature #define's for Korean PCS support.
                   Changed FEATURE_CP_KPCS to FEATURE_KPCS.
01/16/98   lh      "customer.h" included.
01/15/98   lh      Added IS-95B Line Control info record.
01/14/98   dna     Add support for Korean PCS in Band Class Status Response
11/03/97   ldg     Added (and renamed some) data service option numbers to
                   support IS-707.0, Qualcomm proprietary, and IS-707,
                   without a compile-time switch.
07/24/97   na      Added FEATURE_DS_PROPTRY_SO which includes the Qualcomm
                   proprietary service option numbers for rs2 data services.
07/24/97   jca     CAI_PRIV_PLAN changed to correct value as per IS-95A.
07/09/97   na      Changed rateset2 data service option numbers to Qualcomm
                   proprietary numbers used by CBS.
06/10/97   ck      Added CAI_SO_EVRC service Option
02/06/97   jca     Changed T50m from 200 msecs to 1 second.
12/13/96   jca     Added new fields to cai_chnasn_am2_type.
11/14/96   jjw     Added Rate Set 2 data service SO definitions
10/30/96   rdh     Changed N1m default from 3 to 9.
06/12/96   jca     Added #defines for Data Burst Msg Burst Type Assignments.
06/05/96   jpf     Reduced minimum size of several messages in JSTD008 because
                   the smallest MSID went from an ESN to a 2-byte TMSI.
05/28/96   dna     Class 1 IMSI support
04/15/96   jca     Added #define for CAI_STOP_CONT_DTMF order qualifier.
04/09/96   day     Added TMSI addressing modifications.  Using T_TMSI ifdef.
03/13/96   day     Added J-STD-008 PCS message modifications.
                   Using FEATURE_JSTD008 ifdef.
02/06/96   fkm     Put ()s Around Macros to Make Lint Happy
12/12/95   jca     Added CAI_SO_WLL_OFF_HOOK service option.
09/22/95   jca     Restored N1m to IS95A specified value of 3.
08/22/95   dna     New sync channel message field
08/18/95   dna     Added support for assign mode '101'.
08/16/95   gb      Changed CAI_MAX_FRAME_SIZE to support rate set 2 frames.
08/14/95   gb      Added MUX2 parameter support.
08/10/95   rdb     Added defs for new information record types, Status Request
                   Message, Status Response Message per the IS-95-A TSB.
07/27/95   gb      Changed field in Extended Handoff Direction Message.
07/20/95   gb      Added Extended Handoff Direction Message & new Markov SOs.
07/20/95   jca     Added Extended Sys Parameters and Service Redirection
                   messages.  Modified other messages as per IS95A.
07/13/95   rdb     Added defs for channel assignment type 4 record,
                   along with defs for service negotiation messages and
                   time constants and mux option and rate sets and loopback13.
06/25/95   dy      Added define for SMS service option.
06/07/95   dna     Added SSD Update ORDQ's and T64m timeout
05/30/95   gb      Added 13k markov and voice service options.
03/01/95   jjw     Added IS-99 and packet Service Option numbers.
01/10/95   dna     Added reject type CAI_WLL_OFF_HOOK
08/09/94   jca     Added new defines for IS-96A/IS-96 voice service options.
05/12/94   gb      Changed N1m to 13 to support high FER testing.
04/25/94   gb      Parameterised all IS95 timeouts in cai_tmo.
04/20/94   jca     Added mux1 enums and constants.  Added define for
                   Loopback service option.
12/22/93   jca     Changed message formats to IS-95 compatible.
10/29/92   jai     Added new System Parameter msg, In-Traffic System Parameter
                   message and renamed signal types for UI task.
07/23/92   jai     Modified file for lint and linking.
07/08/92   jai     Created file

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#else

#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "qw.h"
#include "nv.h"

#endif

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
** Multiplex Option 1 Data Rates
---------------------------------------------------------------------------*/

typedef enum
{
  CAI_BLANK_RATE,     /* Indicates data was blanked */
  CAI_EIGHTH_RATE,    /* Indicates rate 1/8 data    */
  CAI_QUARTER_RATE,   /* Indicates rate 1/4 data    */
  CAI_HALF_RATE,      /* Indicates rate 1/2 data    */
  CAI_FULL_RATE       /* Indicates rate 1   data    */
} cai_data_rate_type;

/*---------------------------------------------------------------------------
** Traffic types.
** Values for CAI_NULL_TRAF, CAI_PRI_TRAF and CAI_SEC_TRAF are defined in the
** Standard.  Do not change these values.  CAI_PRI_OR_SEC_TRAF is used to
** indicate if a particular service option allows either type of traffic.
** Internal use only.
---------------------------------------------------------------------------*/

typedef enum
{
  CAI_NUL_TRAF = 0,
  CAI_PRI_TRAF = 1,
  CAI_SEC_TRAF = 2,
  CAI_PRI_OR_SEC_TRAF = 3
} cai_traffic_type_type;

/*---------------------------------------------------------------------------
 Radio configuration types.
---------------------------------------------------------------------------*/

#ifdef FEATURE_IS2000
#define CAI_AC_L2_LENGTH_BITS 5
#define CAI_MAX_RCS 5

typedef enum
{
   CAI_RC_1 = 1,
   CAI_RC_2 = 2,
   CAI_RC_3 = 3,
   CAI_RC_4 = 4,
   CAI_RC_5 = 5
} cai_radio_config_type;

/* The following values are defined in the standard, do not change these */
typedef enum
{
  CAI_CHIND_RESERVED_1 =0,
  CAI_CHIND_FCH_ONLY = 1,
  CAI_CHIND_DCCH_ONLY = 2,
  CAI_CHIND_FCH_AND_DCCH = 3,
  CAI_CHIND_RESERVED_2 =4,
  CAI_CHIND_FCH_CONT_REV_PILOT =5,
  CAI_CHIND_DCCH_CONT_REV_PILOT =6,
  CAI_CHIND_FCH_AND_DCCH_CONT_REV_PILOT =7,
  CAI_CHIND_ENUM_END
} cai_chind_type;

/* The following values are defined in the standard, do not change these */
typedef enum
{
  CAI_CHAN_ADJ_GAIN =0,
  CAI_BASIC_RATE_ADJ_GAIN=1,
  CAI_HIGH_RATE_ADJ_GAIN=2
} cai_rpc_adj_rec_type;

/* The following values are defined in the standard, do not change these*/
typedef enum
{
  CAI_CONVOLUTIONAL_CODING = 0,
  CAI_TURBO_CODING = 1
} cai_sch_coding_type;

#define CAI_DEFAULT_REV_PWR_CNTL_DELAY 1
#endif /* FEATURE_IS2000 */

/*---------------------------------------------------------------------------
** Types for CDMA Multiplex Option 1 Frames
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
** The following constants specify the number of bits used per frame at the
** various data rates.
---------------------------------------------------------------------------*/

#define  CAI_EIGHTH_RATE_BITS   16  /* Data rate 1/8:  16 bits or  2 bytes */
#define  CAI_QUARTER_RATE_BITS  40  /* Data rate 1/4:  40 bits or  5 bytes */
#define  CAI_HALF_RATE_BITS     80  /* Data rate 1/2:  80 bits or 10 bytes */
#define  CAI_FULL_RATE_BITS    171  /* Data rate 1  : 171 bits or 22 bytes */

#define  CAI_MAX_FRAME_SIZE  (36)
  /* Specifies the number of bytes required to hold the frame info bits at
     the highest data rate */

typedef byte cai_frame_type[CAI_MAX_FRAME_SIZE];

/*----------------------------------------------------------------------
             Rate Set and Traffic Type Constants
----------------------------------------------------------------------*/

/* Transmission rate codes for rate set 1, forward and reverse link */
#define CAI_RS1_9600 0x80  /* 9.6 kbps */
#define CAI_RS1_4800 0x40  /* 4.8 kbps */
#define CAI_RS1_2400 0x20  /* 2.4 kbps */
#define CAI_RS1_1200 0x10  /* 1.2 kbps */
#define CAI_RS1_ALL  0xF0  /* All 4 rates */
#define CAI_RS1_RLP  0xD0  /* All rates without Quarter Rate */

/* Transmission rate codes for rate set 2, forward and reverse link */
#define CAI_RS2_14400 0x80  /* 14.4 kbps */
#define CAI_RS2_7200  0x40  /*  7.2 kbps */
#define CAI_RS2_3600  0x20  /*  3.6 kbps */
#define CAI_RS2_1800  0x10  /*  1.8 kbps */
#define CAI_RS2_ALL   0xF0  /* All 4 rates */

/*-------------------------------------------------------------------------*/
/*  Sync Channel Constants                                                 */
/*-------------------------------------------------------------------------*/

#define CAI_SC_MAX_MSG_BYTES 148
  /* Max Sync Channel message length (see IS-95A Section 7.7.1.2.1) */
#define CAI_SC_MAX_SIZE (CAI_SC_MAX_MSG_BYTES * 8)
  /* Maximum number of bits in a Sync Channel Message */
#define CAI_SC_CRC_SIZE  30
  /* Number of bits in a Sync Channel CRC */
#define CAI_SC_LGTH_SIZE 8
  /* Number of bits in the length field for a Sync Channel message */
#define CAI_SC_BODY_SIZE (CAI_SC_MAX_SIZE - CAI_SC_CRC_SIZE - \
                             CAI_SC_LGTH_SIZE)
  /* Maximum number of bits in the body of a Sync Channel message */

#define CAI_SC_EXT_SIZE  (CAI_SC_MAX_MSG_BYTES - 1)
  /* Number of bytes required for a Sync Channel Message body and CRC
     but not the length byte  */

/*-------------------------------------------------------------------------*/
/* Paging Channel constants                                                */
/*-------------------------------------------------------------------------*/

#define CAI_PC_MAX_MSG_BYTES 255
  /* Maximum Paging Channel message length (see IS-95A Section 7.7.2.2.1) */
#define CAI_PC_MAX_SIZE (CAI_PC_MAX_MSG_BYTES * 8)
  /* Maximum number of bits in a Paging Channel message */
#define CAI_PC_CRC_SIZE 30
  /* Number of bits in a Paging Channel CRC */
#define CAI_PC_LGTH_SIZE 8
  /* Number of bits in the length field for a Paging Channel message */
#define CAI_PC_BODY_SIZE  (CAI_PC_MAX_SIZE - CAI_PC_CRC_SIZE -         \
                             CAI_PC_LGTH_SIZE)
  /* Maximum number of bits in the body of a Paging Channel message */

#define CAI_PC_EXT_SIZE (CAI_PC_MAX_MSG_BYTES - 1)
  /* Number of bytes required for the Paging Channel message body and CRC
     but not the length byte */

#define PAGECHN_MAX( x ) \
        ((CAI_PC_BODY_SIZE - sizeof( x##_fix_type )) / sizeof( x##_var_type ))
  /* Macro to calculate maximum number of variable types that can occur
     in a Paging Channel message */

/*-------------------------------------------------------------------------*/
/* Traffic Channel constants                                               */
/*-------------------------------------------------------------------------*/

#define CAI_FWD_TC_MAX_MSG_BYTES 255
  /* Max Forward Traffic Channel msg length (see IS-95A Section 7.7.3.2.1) */

#define CAI_REV_TC_MAX_MSG_BYTES 255
  /* Max Reverse Traffic Channel msg length (see IS-95A Section 6.7.2.2) */

#define CAI_FWD_TC_MSG_SIZE (CAI_FWD_TC_MAX_MSG_BYTES * 8)
   /* Maximum number of bits in a Forward Traffic Channel Message */

#define CAI_REV_TC_MSG_SIZE (CAI_REV_TC_MAX_MSG_BYTES * 8)
   /* Maximum number of bits in a Reverse Traffic Channel Message */

#define CAI_TC_CRC_SIZE 16
   /* Number of bits in a Forward or Reverse Traffic Channel Message CRC */

#define CAI_TC_LGTH_SIZE 8
   /* Number of bits in a Forward or Reverse Traffic Channel Message
      length field */

#define CAI_FWD_TC_MSG_BODY_SIZE (CAI_FWD_TC_MSG_SIZE -               \
                                    CAI_TC_CRC_SIZE -                 \
                                      CAI_TC_LGTH_SIZE)
  /* Max number of bits in the body of a Foward Traffic Channel message */

#define CAI_REV_TC_MSG_BODY_SIZE (CAI_REV_TC_MSG_SIZE -                \
                                    CAI_TC_CRC_SIZE -                  \
                                      CAI_TC_LGTH_SIZE)
  /* Max number of bits in the body of a Reverse Traffic Channel message */

#define CAI_FWD_TC_EXT_SIZE (CAI_FWD_TC_MAX_MSG_BYTES - 1)
  /* Number of bytes required for the Forward Traffic Channel message body
     and CRC but not the length byte */

#define CAI_REV_TC_EXT_SIZE (CAI_REV_TC_MAX_MSG_BYTES - 1)
  /* Number of bytes required for the Reverse Traffic Channel message body
     and CRC but not the length byte */

#define TC_FWD_MAX( x )  ((CAI_FWD_TC_MSG_BODY_SIZE -  \
                            sizeof( x##_fix_type )) /  \
                              sizeof( x##_var_type ))
  /* Macro to caluculate the maximum number of variable types that can
     occur in a Forward Traffic Channel Message */

#define TC_REV_MAX( x )  ((CAI_REV_TC_MSG_BODY_SIZE -  \
                            sizeof( x##_fix_type )) /  \
                              sizeof( x##_var_type ))
  /* Macro to caluculate the maximum number of variable types that can
     occur in a Reverse Traffic Channel Message */

/*-------------------------------------------------------------------------*/
/* Access Channel constants                                                */
/*-------------------------------------------------------------------------*/

#define CAI_AC_MAX_MSG_BYTES 110
  /* Maximum Access Channel message length (see IS-95A Section 6.7.1.2.1) */
#define CAI_AC_MAX_SIZE (CAI_AC_MAX_MSG_BYTES * 8)
  /* Maximum number of bits in an Access Channel message */
#define CAI_AC_CRC_SIZE 30
  /* Number of bits in the Access Channel message CRC */
#define CAI_AC_LGTH_SIZE 8
  /* Number of bits in the Access Channel length field */

#define CAI_AC_BODY_SIZE (CAI_AC_MAX_SIZE - CAI_AC_CRC_SIZE -  \
                            CAI_AC_LGTH_SIZE)
  /* Maximum number of bits in the body of an Access Channel message */

#define CAI_AC_EXT_SIZE  (CAI_AC_MAX_MSG_BYTES - 1)
  /* Number of bytes required for the Access Channel message body
     and CRC but not the length byte */

#define AC_MAX( x )   \
  ((CAI_AC_BODY_SIZE - sizeof( x##_fix_type )) / sizeof( x##_var_type ))
  /* Macro to calculate the maximum number of variable types that can occur
     in an Access Channel message */

#ifdef FEATURE_IS2000
/*-------------------------------------------------------------------------*/
/* SCH constants                                                */
/*-------------------------------------------------------------------------*/
/* SCH rate enum, As defined in the std, do not change these */
typedef enum
{
  CAI_SCH_RATE_RS1_1X_RS2_1X = 0,
  CAI_SCH_RATE_RS1_2X_RS2_2X = 1,
  CAI_SCH_RATE_RS1_4X_RS2_4X = 2,
  CAI_SCH_RATE_RS1_8X_RS2_8X = 3,
  CAI_SCH_RATE_RS1_16X_RS2_16X = 4,
  CAI_SCH_RATE_RS1_32X_RS2_18X = 5,
  CAI_SCH_RATE_RS1_64X_RS2_32X = 6,
  CAI_SCH_RATE_RS1_INVALID_RS2_36X = 7,
  CAI_SCH_RATE_RS1_INVALID_RS2_72X = 8,
  CAI_SCH_RATE_NULL=0xff
}cai_sch_rate_type;

#define CAI_MAX_SCH_RATE_RS1  CAI_SCH_RATE_RS1_64X_RS2_32X
#define CAI_MAX_SCH_RATE_RS2  CAI_SCH_RATE_RS1_INVALID_RS2_72X

/*-------------------------------------------------------------------------*/
/* FPC_MODE definitions                                                    */
/*-------------------------------------------------------------------------*/
typedef enum
{
  CAI_FPC_MODE_800_PRI         = 0,
  CAI_FPC_MODE_400_PRI_400_SEC = 1,
  CAI_FPC_MODE_200_PRI_600_SEC = 2,
  CAI_FPC_MODE_50_PRI          = 3
} cai_fpc_mode_type;

#endif /* FEATURE_IS2000 */

/*-------------------------------------------------------------------------*/
/* Power control step size definitions                                     */
/*-------------------------------------------------------------------------*/
typedef enum
{
  CAI_PWR_CNTL_STEP_1_DB    = 0,
  CAI_PWR_CNTL_STEP_0_5_DB  = 1,
  CAI_PWR_CNTL_STEP_0_25_DB = 2
} cai_pwr_cntl_step_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  CAI MESSAGE TYPES                                     */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif

/* Sync Channel message */
#define CAI_SYNC_MSG        0x0001

/* Paging Channel messages */

#define CAI_SYS_PARM_MSG      0x01
  /* System Parameters Message */
#define CAI_ACC_PARM_MSG      0x02
  /* Access Parameters Message */

#define CAI_NL_MSG            0x03
  /* Neighbor List Message */

#define CAI_CL_MSG            0x04
  /* CDMA Channel List Message */
#define CAI_SLT_PAGE_MSG      0x05
  /* Slotted Page Message */
#define CAI_PAGE_MSG          0x06
  /* Page Message */
#define CAI_PC_ORD_MSG        0x07
  /* Order Message */
#define CAI_CHN_ASN_MSG       0x08
  /* Channel Assigment Message */
#define CAI_PC_BURST_MSG      0x09
  /* Data Burst Message */
#define CAI_PC_AUTH_MSG       0x0A
  /* Authentication Challenge Message */
#define CAI_PC_SSD_UP_MSG     0x0B
  /* SSD Update Message */
#define CAI_FEATURE_MSG       0x0C
  /* Feature Notification Message */
#define CAI_EXT_SYS_PARM_MSG  0x0D
  /* Extended System Parameters Message */

#define CAI_EXT_NL_MSG        0x0E
  /* Extended Neighbor List Message */

#define CAI_PC_STREQ_MSG      0x0F
  /* Status Request Message */
#define CAI_SERVICE_REDIR_MSG 0x10
  /* Service Redirection Message */
#define CAI_GENERAL_PAGE_MSG  0x11
  /* General Page Message */
#define CAI_GLOBAL_REDIR_MSG  0x12
  /* Global Service Redirection Message */

#ifdef FEATURE_TMSI
#define CAI_PC_TMSI_ASGN_MSG  0x13
  /* TMSI Assignment Message */
#endif /* FEATURE_TMSI */

#ifdef FEATURE_IS95B
#define CAI_PACA_MSG          0x14
  /* PACA Message */
#define CAI_EXT_CHN_ASN_MSG   0x15
  /* Extended Channel Assignment Message */
#define CAI_GEN_NL_MSG        0x16
  /* General Neighbor List Message */
#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
#define CAI_EXT_GLB_REDIR_MSG 0x1A
  /* Extended Global Service Redirection Message */
#define CAI_EXT_CDMA_CHN_LIST_MSG 0x1B
  /* Extended CDMA channel list message */
#endif /* FEATURE_IS2000 */

/* Access Channel Messages  */

#define CAI_REG_MSG           0x01
  /* Registration Message */
#define CAI_AC_ORD_MSG        0x02
  /* Order Message */
#define CAI_AC_BURST_MSG      0x03
  /* Data Burst message */
#define CAI_ORIG_MSG          0x04
  /* Origination Message */
#define CAI_PAGE_RESP_MSG     0x05
  /* Page Response Message */
#define CAI_AC_AUTH_RESP_MSG  0x06
  /* Authentication Challenge Response Message */
#define CAI_AC_STRSP_MSG      0x07
  /* Status Response Message */
#ifdef FEATURE_TMSI
#define CAI_AC_TMSI_CMP_MSG   0x08
  /* TMSI Assignment Completion Message */
#endif /* FEATURE_TMSI */
#ifdef FEATURE_IS95B
#define CAI_AC_PACA_CAN_MSG   0x09
  /* PACA Cancel Message */
#define CAI_AC_EXT_STRSP_MSG  0x0A
  /* Extended Status Response Message */
#endif /* FEATURE_IS95B */



/* Forward Traffic Channel Messages (see IS-95A Table 7.7.3.3-1) */

#define CAI_TC_FWD_ORD_MSG    0x01
  /* Order Message */
#define CAI_TC_AUTH_MSG       0x02
  /* Authentication Challenge Message */
#define CAI_ALERT_MSG         0x03
  /* Alert with Information Message */
#define CAI_TC_FWD_BURST_MSG  0x04
  /* Data Burst Message */
#define CAI_HO_DIR_MSG        0x05
  /* Handoff Direction Message */
#define CAI_FM_HO_MSG         0x06
  /* Analog Handoff Direction Message */
#define CAI_TC_SYS_PARM_MSG   0x07
  /* In-Traffic System Parameter Message */
#define CAI_TC_NLU_MSG        0x08
  /* Neighbor List Update Message */
#define CAI_SEND_BURST_MSG    0x09
  /* Send Burst DTMF Message */
#define CAI_PWR_CTL_MSG       0x0A
  /* Power Control Parameters Message */
#define CAI_RET_PARMS_MSG     0x0B
  /* Retrieve Parameters Message */
#define CAI_SET_PARMS_MSG     0x0C
  /* Set Parameters Message */
#define CAI_TC_SSD_UP_MSG     0x0D
  /* SSD Update Message */
#define CAI_FWD_FLASH_MSG     0x0E
  /* Flash with Information Message */
#define CAI_MOB_REG_MSG       0x0F
  /* Mobile Station Registered Message */
#define CAI_TC_STREQ_MSG      0x10
  /* Status Request Message */
#define CAI_EXT_HO_DIR_MSG    0x11
  /* Extended Handoff Direction Message */
#define CAI_FTC_SRV_REQ_MSG   0x12
  /* Service Request Message */
#define CAI_FTC_SRV_RSP_MSG   0x13
  /* Service Response Message */
#define CAI_SRV_CON_MSG       0x14
  /* Service Connect Message */
#define CAI_FTC_SRV_CTL_MSG   0x15
  /* Service Option Control Message */

#ifdef FEATURE_TMSI
#define CAI_FTC_TMSI_ASGN_MSG 0x16
  /* TMSI Assignment Message */
#endif /* FEATURE_TMSI */

#ifdef FEATURE_IS95B
#define CAI_FTC_SRV_REDIR_MSG 0x17
  /* Service Redirection Message */
#define CAI_SUP_CHN_ASN_MSG   0x18
  /* Supplemental CHannel Assignment Message */
#define CAI_FTC_PWR_CTRL_MSG  0x19
  /* Power Control Message */
#define CAI_EXT_NLU_MSG       0x1A
  /* Extended Neighbor List Update Message */
#define CAI_CF_SRCH_REQ_MSG   0x1B
  /* Candidate Frequency Search Request Message */
#define CAI_CFS_CTRL_MSG      0x1C
  /* Candidate Frequency Search Control Message */
#define CAI_PUF_MSG           0x1D
  /* Power Up Function Message */
#define CAI_PUF_CMP_MSG       0x1E
  /* Power Up Function Completion Message */
#endif /* FEATURE_IS95B */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
#define CAI_GEN_HO_DIR_MSG    0x1F
  /* General Handoff Direction Message */
#endif

#ifdef FEATURE_IS2000
#define CAI_UNIV_HO_DIR_MSG   0X22
  /* Universal Handoff Direction Message */
#define CAI_EXT_SUP_CHN_ASN_MSG   0x23
  /* Extended Supplemental Channel Assignment Message */
#endif

/* Reverse Traffic Channel Messages (see IS-95A Table 6.7.2.3-1) */
#define CAI_TC_REV_ORD_MSG    0x01
  /* Order Message */
#define CAI_TC_AUTH_RESP_MSG  0x02
  /* Authentication Challenge Response Message */
#define CAI_REV_FLASH_MSG     0x03
  /* Flash with Information Message */
#define CAI_TC_REV_BURST_MSG  0x04
  /* Data Burst Message */
#define CAI_PIL_STR_MSG       0x05
  /* Pilot Strength Measurement Message */
#define CAI_PWR_MSR_MSG       0x06
  /* Power Measurement Report Message */
#define CAI_SEND_DTMF_MSG     0x07
  /* Send Burst DTMF Message */
#define CAI_STATUS_MSG        0x08
  /* Status Message */
#define CAI_ORIG_C_MSG        0x09
  /* Origination Continuation Message */
#define CAI_HO_COMP_MSG       0x0A
  /* Handoff Completion Message */
#define CAI_PARM_RSP_MSG      0x0B
  /* Parameters Response message */
#define CAI_RTC_SRV_REQ_MSG   0x0C
  /* Service Request Message */
#define CAI_RTC_SRV_RSP_MSG   0x0D
  /* Service Response Message */
#define CAI_SRV_CMP_MSG       0x0E
  /* Service Connect Completion Message */
#define CAI_RTC_SRV_CTL_MSG   0x0F
  /* Service Option Control Message */
#define CAI_TC_STRSP_MSG      0x10
  /* Status Response Message */
#ifdef FEATURE_TMSI
#define CAI_RTC_TMSI_CMP_MSG  0x11
  /* TMSI Assignment Completion Message */
#endif /* FEATURE_TMSI */
/* Following are for IS-95B */
#ifdef FEATURE_IS95B
#define CAI_SUP_CHAN_REQ_MSG  0x12
  /* Supplemental Channel Request Message */
#define CAI_CFS_RSP_MSG       0x13
  /* Candidate Frequency Search Response Message */
#define CAI_CFS_RPT_MSG       0x14
  /* Candidate Frequency Search Report Message */
#define CAI_PER_PLT_STR_MSG   0x15
  /* Periodic Pilot Strength Measurement Message */
#ifdef FEATURE_IS2000
#define CAI_OUTER_RPT_MSG     0x16
  /* Outer Loop Report Message */
#endif
#endif /*  FEATURE_IS95B */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                     ORDER  CODES                                        */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Tables 6.7.3-1 Order and Order Qualification Codes Used on
   the Reverse Traffic Channel and the Access Channel and 7.7.4-1. Order
   and Order Qualification Codes Used on the Paging Channel and the Forward
   Traffic Channel                                                         */

/* Forward Channel Specific Order codes */

#define CAI_ABBR_ALERT_ORD    0x01
  /* Abbreviated Alert Order */
#define CAI_BS_CHAL_CONF_ORD  0x02
  /* Base Station Challenge Confirmation Order */
#define CAI_ENCRYPT_MODE_ORD  0x03
  /* Message Encryption Mode Order */
#define CAI_REORDER_ORD       0x04
  /* Reorder Order */
#define CAI_PARAM_UPDATE_ORD  0x05
  /* Parameter Update Order */
#define CAI_AUDIT_ORD         0x06
  /* Audit Order */
#define CAI_INTERCEPT_ORD     0x09
  /* Intercept Order */
#define CAI_MAINT_ORD         0x0A
  /* Maintenance Order */
#define CAI_BS_ACK_ORD        0x10
  /* Base Station Acknowledgement Order */
#define CAI_PILOT_MEASURE_ORD 0x11
  /* Pilot Measurement Request Order */
#define CAI_LOCK_OR_MAINT_ORD 0x12
  /* Lock Until Power Cycled, Maintenance Required or Unlock Orders */
#define CAI_STATUS_ORD        0x1A
  /* Status Request Order */
#define CAI_REG_ORD           0x1B
  /* Registration Order */
#define CAI_LOCAL_CTL_ORD     0x1E
  /* Local Control Order */
#ifdef FEATURE_IS2000
#define CAI_SLOTTED_MODE_ORD  0x1F
  /* Slotted Mode Order */
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
#define CAI_RETRY_ORD         0x20
  /* Retry Order */
#endif
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#ifdef FEATURE_IS2000
/* -----------------
** Retry Order Types
** ----------------- */
typedef enum
{
  CAI_RETRY_CLR_ALL = 0, /* Clear all retry delay */
  CAI_RETRY_ORIG    = 1, /* Origination retry delay */
  CAI_RETRY_RESA    = 2, /* Resource request retry delay */
  CAI_RETRY_SCRM    = 3  /* Supplemental Channel request retry delay */
} cai_retry_order_type;
#endif /* FEATURE_IS2000 */

/* Reverse Channel Specific Order Codes */

#define CAI_BS_CHAL_ORD           0x02
  /* Base Station Challenge Order */
#define CAI_SSD_UPDATE_ORD        0x03
  /* SSD Update Confirmation/Rejection Order */
#define CAI_PARAM_UPDATE_CONF_ORD 0x05
  /* Parameter Update Confirmation Order */
#define CAI_FM_REQUEST_ORD        0x0B
  /* Request Analog Service Order */
#define CAI_MS_ACK_ORD            0x10
  /* Mobile Station Acknowledgement Order */
#define CAI_MS_CONNECT_ORD        0x18
  /* Connect Order */
#define CAI_LOCAL_CTL_RESP_ORD    0x1E
  /* Local Control Response order */
#define CAI_MS_REJECT_ORD         0x1F
  /* Mobile Station Reject Order */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Order Codes that are used on both the Reverse and Forward Traffic
   Channels */

#define CAI_SO_REQ_ORD            0x13
  /* Service Option Request Order */
#define CAI_SO_RESP_ORD           0x14
  /* Service Option Response Order */
#define CAI_RELEASE_ORD           0x15
  /* Release Order */

#ifdef FEATURE_IS2000
#define CAI_OUTER_LOOP_RPT_ORD    0x16
  /* Outer Loop Report Order */
#endif

#define CAI_PLC_ORD               0x17
  /* Long Code Transition Request/Response Order */
#define CAI_DTMF_ORD              0x19
  /* Continuous DTMF Tone Order */
#define CAI_SO_CTL_ORD            0x1D
  /* Service Option Control Order */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                          ORDER QUALIFIERS                               */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Tables 6.7.3-1 Order and Order Qualification Codes Used on
   the Reverse Traffic Channel and the Access Channel and 7.7.4-1. Order
   and Order Qualification Codes Used on the Paging Channel and the Forward
   Traffic Channel                                                         */

#define CAI_LUPC_MASK      0x10
  /* Mask to determine if an order is a Lock Until Power Cycle Order */
#define CAI_UNLOCK_MASK    0xFF
  /* Mask to determine if an order is an Unlock Order */
#define CAI_MAINT_REQ_MASK 0x20
  /* Mask to determine if an order is a Maintenance Required Order */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_STOP_CONT_DTMF 0xFF
  /* Value of ORDQ field for a Continuous DTMF Tone Order
     (Stop continuous DTMF tone) */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_SSD_UPDATE_CONF        0x00
  /* Value of Order qualifier which indicates that the order is an
     SSD Update Confirmation Order */
#define CAI_SSD_UPDATE_REJ         0x01
  /* Value of Order qualifier which indicates that the order is an
     SSD Update Confirmation Order */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_REG_ACK 0x00
  /* Value of Order Qualifier if Order is a Registration Accepted Order */
#define CAI_REG_REQ 0x01
  /* Value of Order Qualifier if Order is a Registration Request Order */
#define CAI_REG_REJ 0x02
  /* Value of Order Qualifier if Order is a Registration Rejected Order */
#ifdef FEATURE_TMSI
#define CAI_REG_REJ_DEL_TMSI 0x04
  /* Value of Order Qualifier if Order is a Registration Rejected Order */
  /* (delete TMSI)                                                      */
#endif /* FEATURE_TMSI */
#ifdef FEATURE_IS95B
#define CAI_REG_ACC 0x05
  /* Value of Order Qualifier if Order is a Registration Accepted Order
     ( ROAM INDI included ) */
#endif /* FEATURE_IS95B */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_PLC_REQ_PUB 0x00
  /* Value of Order qualifier which indicates that the order is a
     Long Code Transition Request Order (request public) */
#define CAI_PLC_REQ_PRI 0x01
  /* Value of Order qualifier which indicates that the order is a
     Long Code Transition Request Order (request private) */
#define CAI_PLC_RSP_PUB 0x02
  /* Value of Order qualifier which indicates that the order is a
     Long Code Transition Response Order (use public) */
#define CAI_PLC_RSP_PRI 0x03
  /* Value of Order qualifier which indicates that the order is a
     Long Code Transition Response Order (use private) */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_REL_NORMAL 0x00
  /* Value of ORDQ to indicate a normal release */
#define CAI_REL_PWDN   0x01
  /* Value of Order qualifier to indicate a release with a powerdown
     indication */
#define CAI_REL_SO     0x02
  /* Value of Order qualification code to indicate that call is released
     because the requested service option is rejected */

#define CAI_REL_SRV_INACT  0x02
  /* Value of ORDQ to indicate a release with service inactive indication */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_REJ_UNSP   0x01
  /* Value of Order qualifier which indicates that the order was rejected
     for an unspecified reason */

#define CAI_REJ_STATE  0x02
  /* Value of Order qualifer which indicates that the message was rejected
     due to the mobiles current state */

#define CAI_REJ_STRUCT 0x03
  /* Value of Order qualifer which indicates that the message was rejected
     because the message structure was unacceptable */

#define CAI_REJ_FIELD  0x04
  /* Value of Order qualifer which indicates that the message was rejected
     because a message field was not in the valid range */

#define CAI_REJ_CODE   0x05
  /* Value of Order qualifier which indicates that the message was rejected
     because the message type or order code was not understood. */

#define CAI_REJ_CAP    0x06
  /* Value of Order qualifier which indicates that the message was rejected
     because it requires a capability that is not supported by the mobile
     station */

#define CAI_REJ_CFG    0x07
  /* Value of Order qualifer which indicates that the message was rejected
     because the message cannot be handled by the current mobile station
     configuration. */

#ifdef FEATURE_IS95B
#define CAI_REJ_LEN    0x08
  /* Value of Order qualifer which indicates that the message was rejected
     because the response message would exceed allowable length */

#define CAI_REJ_MOD    0x09
  /* Value of Order qualifer which indicates that the message was rejected
     because the info record is not supported by the specified band class
     and operating mode */

#define CAI_REJ_SRCH   0x0A
  /* Value of Order qualifer which indicates that the message was rejected
     because the search set not specified  */

#define CAI_REJ_ISRCH  0x0B
  /* Value of Order qualifer which indicates that the message was rejected
     because invalid search request */

#define CAI_REJ_IFREQ  0x0C
  /* Value of Order qualifer which indicates that the message was rejected
     because invalid frequency assignment */

#define CAI_REJ_TSHT   0x0D
  /* Value of Order qualifer which indicates that the message was rejected
     because the search period is too short */

#define CAI_VAL_RPT_PER 0x0A
  /* Value of Order qualifier which indicates a valid report period for
     the periodic pilot measurement request order. */

#endif /* FEATURE_IS95B || FEATURE_SPECIAL_MDR */

#ifdef FEATURE_IS2000
#define CAI_REJ_RC_MISMATCH 0x0E
  /* RC does not match with the value in the field DEFAULT_CONFIG */
#define CAI_REJ_RESOURCE_CTL_OOS 0x10
  /* Resource Control out-of-sync */
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* (See IS-95A Table 7.7.4.4-1. Status Request ORDQ Values)                 */

#define CAI_CALL_MODE_REQ  0x07
  /* Value of order qualifier for a Status Request order requesting
     call mode information */

#define CAI_TERM_INFO_REQ  0x08
  /* Value of order qualifer for a Status Request order requesting
     terminal information */

#define CAI_ROAM_INFO_REQ  0x09
  /* Value of order qualifer for a Status Request order requesting
     Roaming information */

#define CAI_SEC_STATUS_REQ 0x0A
  /* Value of order qualifer for a Status Request order requesting
     security status information */

#define CAI_IMSI_REQ       0x0C
  /* Value of order qualifer for a Status Request order requesting
     IMSI information */

#define CAI_ESN_REQ        0x0D
  /* Value of order qualifer for a Status Request order requesting
     ESN information */

/* The following request codes are for use in the Status Request Message */

#define CAI_BAND_REQ       0x0E
  /* Request code for band class information */
#define CAI_PWR_REQ        0x0F
  /* Request code for power class information */
#define CAI_OP_MODE_REQ    0x10
  /* Request code for operating mode information */
#define CAI_SO_REQ         0x11
  /* Request code for service option information */
#define CAI_MUX_REQ        0x12
  /* Request code for multiplex option information */
#define CAI_SRV_CFG_REQ    0x13
  /* Reqeust code for service configuration information */
#ifdef FEATURE_IS95B
#define CAI_PWR_CTRL_REQ   0x17
  /* Request code for Power Control Information */
#define CAI_IMSI_M_REQ     0x18
  /* Reqeust code for MIN based IMSI */
#define CAI_IMSI_T_REQ     0x19
  /* Reqeust code for True IMSI */
#define CAI_CAP_REQ        0x1A
  /* Request code for Capability Information */
#endif /* FEATURE_IS95B */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                 PAGING CHANNEL ADDRESS TYPES                            */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.2.3.1-1 Address Types                              */

#define CAI_MIN_ADDR_TYPE   0x0
  /* MIN (MIN1 and MIN2) address field type */
#define CAI_ESN_ADDR_TYPE   0x1
  /* ESN address field type */
#define CAI_IMSI_ADDR_TYPE  0x2
  /* IMSI address field type */
#define CAI_TMSI_ADDR_TYPE  0x3
  /* TMSI address field type */
#define CAI_BCAST_ADDR_TYPE 0x5
  /* BROADCAST address field type */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                 ACCESS CHANNEL ADDRESS TYPES                            */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.1.3.1.1-1 Address Types                            */

/* ----------------------------------------------------------
** NOTE - IMSI and ESN address types share the same #define's
**         as those defined above for the Paging Channel
** ---------------------------------------------------------- */
#define CAI_MIN_AND_ESN_ADDR_TYPE   0x0
  /* MIN (MIN1 and MIN2) and ESN address field type */
#define CAI_IMSI_AND_ESN_ADDR_TYPE  0x3
  /* IMSI and ESN address field type */
#define CAI_TMSI_AC_MSID_ADDR_TYPE  0x5
  /* TMSI address MSID field type */
#define CAI_TMSI_IMSI_PREF_MSID_TYPE  0x6
  /* TMSI address MSID field type */
#define CAI_TMSI_IMSI_ESN_MSID_TYPE  0x7
  /* TMSI address MSID field type */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*     PAGING AND FORWARD TRAFFIC CHANNEL INFORMATION RECORDS              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.5-1 Information Record Types                       */

#define CAI_DISPLAY_REC     0x01
  /* Display record type */
#define CAI_CALLED_REC      0x02
  /* Called Party Number record type */
#define CAI_CALLING_REC     0x03
  /* Calling Party Number record type */
#define CAI_CONNECT_REC     0x04
  /* Connected Number record type */
#define CAI_SIGNAL_REC      0x05
  /* Signal record type */
#define CAI_MSG_WAITING_REC 0x06
  /* Message Waiting record type */
#define CAI_FTC_SRV_CFG_REC 0x07
  /* Service confguration record type */
#ifdef FEATURE_IS95B
/* Following are defined for IS-95B */
#define CAI_CALLED_SUB_REC  0x08
  /* IS95B Called Party Subaddress record type */
#define CAI_CALLING_SUB_REC 0x09
  /* IS95B Calling Party Subaddress record type */
#define CAI_CONNECT_SUB_REC 0x0A
  /* IS95B Connected Subaddress record type */
#define CAI_REDIR_NUM_REC   0x0B
  /* IS95B Redirecting Number record type */
#define CAI_REDIR_SUB_REC   0x0C
  /* IS95B Redirecting Subaddress record type */
#endif /* FEATURE_IS95B */
#if defined( FEATURE_IS95B ) || defined( FEATURE_IS95B_WLL_ONLY )
#define CAI_METER_PULSES_REC  0x0D
  /* IS95B Meter pulses record type */
#define CAI_PARA_ALERT_REC  0x0E
  /* IS95B Parametric alerting record type */
#define CAI_LINE_CTRL_REC   0x0F
  /* IS95B Line control record type */
#endif
#ifdef FEATURE_IS95B
#define CAI_EXT_DISPLAY_REC 0x10
  /* IS95B Extended Display record type */
#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
#define CAI_USER_ZONE_UPDATE_REC 0x11
  /* IS2000 User Zone Update record type */
#define CAI_USER_ZONE_REJECT_REC 0x12
  /* IS2000 User Zone Reject record type */
#define CAI_FTC_NON_NEG_SRV_CFG_REC 0x13
  /* IS2000 Non-Negotiable service configuration */
#endif

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*      ACCESS CHANNEL & REVERSE TRAFFIC CHANNEL INFORMATION RECORDS       */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.4-1 Information Record Types                       */

#define CAI_FEAT_REC        0x02
  /* Feature Indicator */
#define CAI_KEYPAD_REC      0x03
  /* Keypad Facility */
#define CAI_REV_CALLED_REC  0x04
  /* Called Party Number */
#define CAI_REV_CALLING_REC 0x05
  /* Calling Party Number */
#define CAI_CALL_MODE_REC   0x07
  /* Call Mode */
#define CAI_TERM_INFO_REC   0x08
  /* Terminal Information */
#define CAI_ROAM_INFO_REC   0x09
  /* MIN Information */
#define CAI_SEC_REC         0x0A
  /* Security Status */
#define CAI_REV_CONN_REC    0x0B
  /* Connected Number */
#define CAI_IMSI_REC        0x0C
  /* IMSI information */
#define CAI_ESN_REC         0x0D
  /* ESN information */
#define CAI_BAND_REC        0x0E
  /* Band class information */
#define CAI_PWR_REC         0x0F
  /* Power class information */
#define CAI_OP_MODE_REC     0x10
  /* Operating mode information */
#define CAI_SO_REC          0x11
  /* Service option information */
#define CAI_MUX_REC         0x12
  /* Multiplex option information */
#define CAI_RTC_SRV_CFG_REC 0x13
  /* Service configuration record type */
#ifdef FEATURE_IS95B
/* Following are defined for IS-95B */
#define CAI_REV_CALLED_SUB_REC  0x14
  /* Called Party Subaddress */
#define CAI_REV_CALLING_SUB_REC 0x15
  /* Called Party Subaddress */
#define CAI_REV_CONN_SUB_REC    0x16
  /* Connected Subaddress */
#define CAI_PWR_CTRL_REC        0x17
  /* Power Control Information */
#define CAI_IMSI_M_REC          0x18
  /* Reqeust code for MIN based IMSI */
#define CAI_IMSI_T_REC          0x19
  /* Reqeust code for True IMSI */
#define CAI_CAPA_INFO_REC       0x1A
  /* Capability Information */
#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
#define CAI_CHAN_CFG_CAPA_INFO_REC    0x1B
  /* Channel Configuration Capability Information */
#define CAI_EXT_MULT_OPT_INFO_REC     0x1C
  /* Extended Multiplex Option Information */
#define CAI_USER_ZONE_UPDATE_REQ_REC  0x1D
  /* User Zone Update Request */
#define CAI_GEO_LOC_INFO_REC          0x1E
  /* Geo-location information */
#define CAI_BAND_SUBCLASS_INFO_REC    0x1F
  /* Band subclass information */
#endif

#ifdef FEATURE_IS95B_INC_CALL_FWD_ENH
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*               REVERSE TRAFFIC CHANNEL FEATURE INDICATOR                 */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95B Table 6.7.4.1-1 Feauture Identifiers                         */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Feature indicator record supplementary service types */
#define CAI_INCOMING_CALL_FWD   0x00
#endif /* FEATURE_IS95B_INC_CALL_FWD_ENH */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                      REQUEST_MODE CODES                                 */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.1.3.2.4-1. REQUEST_MODE Codes                      */

#define CAI_CDMA_ONLY      0x1
  /* CDMA only */
#define CAI_ANALOG_ONLY    0x2
  /* Analog only */
#define CAI_CDMA_OR_ANALOG 0x3
  /* Either CDMA or analog */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                        NUMBER TYPES                                     */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.1.3.2.4-2. Number Types                            */

#define CAI_UNKNOWN_NUM  0x00
  /* Unknown number type */
#define CAI_INT_NUM      0x01
  /* International number */
#define CAI_NAT_NUM      0x02
  /* National number */
#define CAI_NET_SPEC_NUM 0x03
  /* Network-specific number */
#define CAI_SUB_NUM      0x04
  /* Suscriber number */
#define CAI_ABB_NUM      0x06
  /* Abbreviated number */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                        NUMBER PLANS                                     */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.1.3.2.4-3. Numbering Plan Identification           */

#define CAI_UNKNOWN_PLAN 0x00
  /* Unknown number plan */
#define CAI_ISDN_PLAN    0x01
  /* ISDN/Telephony numbering plan (CCITT E.164 and CCITT E.163) */
#define CAI_DATA_PLAN    0x03
  /* Data numbering plan (CCITT X.121) */
#define CAI_TELEX_PLAN   0x04
  /* Telex numbering plan (CCITT F.69) */
#define CAI_PRIV_PLAN    0x09
  /* Private numbering plan */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                        PRESENTATION INDICATORS                          */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.4.4-1. Presentation Indicators                     */

#define CAI_ALLOW_PI     0x00
  /* Presentation allowed */
#define CAI_RES_PI       0x01
  /* Presentation restricted */
#define CAI_NOT_AVAIL_PI 0x02
  /* Number not available */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                       SCREENING INDICATORS                              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.4.4-2. Screening Indicators                        */

#define CAI_NOT_SI  0x00
  /* User-provided, not screened */
#define CAI_PASS_SI 0x01
  /* User-provided, verified and passed */
#define CAI_FAIL_SI 0x02
  /* User-provided, verified and failed */
#define CAI_NET_SI  0x03
  /* Network-provided */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                        SIGNAL TYPES                                     */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.5.5-1. Signal Type                                 */

#define CAI_SIG_TONE  0x00
  /* Tone signal */
#define CAI_SIG_ISDN  0x01
  /* ISDN Alerting */
#define CAI_SIG_IS54B 0x02
  /* IS-54B Alerting */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                      ALERT PITCHES                                      */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.5.5-2. Alert Pitch                                 */

#define CAI_PITCH_MED  0x00
  /* Medium pitch (standard alert) */
#define CAI_PITCH_HIGH 0x01
  /* High pitch */
#define CAI_PITCH_LOW  0x02
  /* Low pitch */
#define CAI_PITCH_RSV  0x03
  /* Reserved pitch */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                          TONE SIGNALS                                   */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.5.5-3. Tone Signals (SIGNAL_TYPE = '00')           */

#define CAI_TONE_DIAL           0x00
  /* Dial tone on */
#define CAI_TONE_RING           0x01
  /* Ring back tone on */
#define CAI_TONE_INTERCEPT      0x02
  /* Intercept tone on */
#define CAI_TONE_ABBR_INTERCEPT 0x03
  /* Abbreviated intercept: alternating tones repeated for four seconds */
#define CAI_TONE_REORDER        0x04
  /* Network congestion (reorder) tone on */
#define CAI_TONE_ABBR_REORDER   0x05
  /* Abbreviated network congestion (reorder) */
#define CAI_TONE_BUSY           0x06
  /* Busy tone on */
#define CAI_TONE_CONFIRM        0x07
  /* Confirm tone on */
#define CAI_TONE_ANSWER         0x08
  /* Answer tone on */
#define CAI_TONE_CALLWAIT       0x09
/* Call waiting tone on */
#ifdef FEATURE_IS95B
#define CAI_TONE_PIP            0x0A
  /* Pip tone on */
#endif /* FEATURE_IS95B */
#define CAI_TONE_OFF            0x3F
  /* Tones off */

/* See IS-95A Table 7.7.5.5-4. ISDN Alerting (SIGNAL_TYPE = '01')         */

#define CAI_ISDN_NORMAL     0x00
  /* Normal Alerting */
#define CAI_ISDN_INTERGROUP 0x01
  /* Intergroup Alerting */
#define CAI_ISDN_SPECIAL    0x02
  /* Special/Priority Alerting */
#define CAI_ISDN_RESERVED3  0x03
  /* Reserved (ISDN Alerting pattern 3) */
#define CAI_ISDN_PING       0x04
  /* "Ping ring": single burst of 500 ms */
#define CAI_ISDN_RESERVED5  0x05
  /* Reserved (ISDN Alerting pattern 5) */
#define CAI_ISDN_RESERVED6  0x06
  /* Reserved (ISDN Alerting pattern 6) */
#define CAI_ISDN_RESERVED7  0x07
  /* Reserved (ISDN Alerting pattern 7) */
#define CAI_ISDN_OFF        0x0F
  /* Alerting off */

/* See IS-95A Table 7.7.5.5-5. IS-54B Alerting (SIGNAL_TYPE = '10')        */

#define CAI_IS54B_OFF      0x00
  /* No Tone: off */
#define CAI_IS54B_L        0x01
  /* Long: (standard alert) */
#define CAI_IS54B_SS       0x02
  /* Short-short */
#define CAI_IS54B_SSL      0x03
  /* Short-short-long */
#define CAI_IS54B_SS2      0x04
  /* Short-short-2 */
#define CAI_IS54B_SLS      0x05
  /* Short-long-short */
#define CAI_IS54B_SSSS     0x06
  /* Short-short-short-short */
#define CAI_IS54B_PBX_L    0x07
  /* PBX Long */
#define CAI_IS54B_PBX_SS   0x08
  /* PBX Short-short */
#define CAI_IS54B_PBX_SSL  0x09
  /* PBX Short-short-long */
#define CAI_IS54B_PBX_SLS  0x0A
  /* PBX Short-long-short */
#define CAI_IS54B_PBX_SSSS 0x0B
  /* PBX Short-short-short-short */
#ifdef FEATURE_IS95B
#define CAI_IS54B_PPPP     0x0C
  /* Pip-Pip-Pip-Pip */
#endif /* FEATURE_IS95B */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                    DTMF_ON_LENGTHS                                      */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.2.3.2.7-1. Recommended DTMF Pulse Width            */

#define CAI_DTMF_ON_95  0x00
  /* 95 ms recommended pulse width */
#define CAI_DTMF_ON_150 0x01
  /* 150 ms recommended pulse width */
#define CAI_DTMF_ON_200 0x02
  /* 200 ms recommended pulse width */
#define CAI_DTMF_ON_250 0x03
  /* 250 ms recommended pulse width */
#define CAI_DTMF_ON_300 0x04
  /* 300 ms recommended pulse width */
#define CAI_DTMF_ON_350 0x05
  /* 350 ms recommended pulse width */
#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                    DTMF_OFF_LENGTHS                                      */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.2.3.2.7-2 Recommended Minimum Interdigit Interval  */

#define CAI_DTMF_OFF_60  0x00
  /* 60 ms recommended minimum interdigit interval */
#define CAI_DTMF_OFF_100 0x01
  /* 100 ms recommended minimum interdigit interval */
#define CAI_DTMF_OFF_150 0x02
  /* 150 ms recommended minimum interdigit interval */
#define CAI_DTMF_OFF_200 0x03
  /* 200 ms recommended minimum interdigit interval */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  PAGING CHANNEL DATA RATES                              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.1.3-1. Paging Channel Data Rate                    */

#define CAI_PC_1_RATE 0x0    /* 9600 bps */
#define CAI_PC_2_RATE 0x1    /* 4800 bps */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  TRAFFIC CHANNEL RATE SETS                              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_MUX1 0x1    /* 9600 bps */
#define CAI_MUX2 0x2    /* 14400 bps */
#define CAI_MUX3  0x3    /* 9600  bps, 1 supplemental */
#define CAI_MUX4  0x4    /* 14400 bps, 1 supplemental */
#define CAI_MUX5  0x5    /* 9600  bps, 2 supplemental */
#define CAI_MUX6  0x6    /* 14400 bps, 2 supplemental */
#define CAI_MUX7  0x7    /* 9600  bps, 3 supplemental */
#define CAI_MUX8  0x8    /* 14400 bps, 3 supplemental */
#define CAI_MUX9  0x9    /* 9600  bps, 4 supplemental */
#define CAI_MUX10 0xa    /* 14400 bps, 4 supplemental */
#define CAI_MUX11 0xb    /* 9600  bps, 5 supplemental */
#define CAI_MUX12 0xc    /* 14400 bps, 5 supplemental */
#define CAI_MUX13 0xd    /* 9600  bps, 6 supplemental */
#define CAI_MUX14 0xe    /* 14400 bps, 6 supplemental */
#define CAI_MUX15 0xf    /* 9600  bps, 7 supplemental */
#define CAI_MUX16 0x10   /* 14400 bps, 7 supplemental */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
#define CAI_MAX_NUM_SUP_RS1 7   /* Absolute Max num of sup in RS1 */
#define CAI_MAX_NUM_SUP_RS2 7   /* Absolute Max num of sup in RS2 */

#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  #ifdef T_MSM5000  /* MSM5000 supports only 3 supps */
    #define CAI_MAX_USABLE_MUX_RS1 CAI_MUX7
    #define CAI_MAX_USABLE_MUX_RS2 CAI_MUX8
  #else
    #define CAI_MAX_USABLE_MUX_RS1 CAI_MUX15
    #define CAI_MAX_USABLE_MUX_RS2 CAI_MUX12
  #endif
  #define CAI_MAX_MUX_RS1 CAI_MUX15
  #define CAI_MAX_MUX_RS2 CAI_MUX16
#else
  #define CAI_MAX_USABLE_MUX_RS1 CAI_MUX1
  #define CAI_MAX_USABLE_MUX_RS2 CAI_MUX2
  #define CAI_MAX_MUX_RS1 CAI_MUX1
  #define CAI_MAX_MUX_RS2 CAI_MUX2
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */

#ifdef FEATURE_IS2000
#define CAI_MUX_1X_RS1                   0x03   /* 1X, MUX PDU 1           */

#define CAI_MUX_2X_RS1_SINGLE            0x809  /* 2X, single size pdu     */
#define CAI_MUX_2X_RS1_DOUBLE            0x905  /* 2X, double size pdu     */

#define CAI_MUX_4X_RS1_SINGLE            0x811  /* 4X, single size pdu     */
#define CAI_MUX_4X_RS1_DOUBLE            0x909  /* 4X, double size pdu     */

#define CAI_MUX_8X_RS1_SINGLE            0x821  /* 8X, single size pdu     */
#define CAI_MUX_8X_RS1_DOUBLE            0x911  /* 8x, double size pdu     */

#define CAI_MUX_16X_RS1_DOUBLE           0x921  /* 16x, double size  only  */

#endif /* FEATURE_IS2000 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*             SERVICE NEGOTIATION TYPES                                   */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_SERV_OPT_NEG 0
  /* Indicates service option negotiation. */

#define CAI_SERV_NEG     1
  /* Indicates service negotiation. */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  SERVICE OPTIONS                                        */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_SO_REJ          0x0000
  /* Service Option number to indicate service option rejection */

#ifdef FEATURE_EVRC_SO_MGMT
#define CAI_SO_VOICE_WILDCARD  0x0000
 /* Service Option to indicate a wildcard voice service option that
    is internal to UI and Call Processing. UI sends this Service
    Option in the origination command to CP if the call is not orig by DM
    or if it is not a voice call */
#endif

#define CAI_SO_VOICE_IS96A  0x0001
 /* Basic Variable Rate Voice Service (IS-96A) */

#define CAI_SO_LOOPBACK     0x0002
  /* Mobile Station Loopback (IS-126) */

#define CAI_SO_VOICE_EVRC   0x0003
  /* Enhanced Variable Rate Voice Service */

#define CAI_SO_ASYNC_DATA_PRE707     0x0004 /* IS-99 s.o. number      */
  /* Asynchronous Data Service (IS-99) */

#define CAI_SO_G3_FAX_PRE707         0x0005 /* IS-99 s.o. number      */
  /* Group 3 FAX Service (IS-99) */

#define CAI_SO_SMS          0x0006
  /* Short Message Services (IS-637) */

#define CAI_SO_PPP_PKT_DATA_PRE707  0x0007 /* IS-657 s.o. number     */
  /* Internet Standard PPP Packet Data Service (IS-657) */

#define CAI_SO_LOOPBACK_13K 0x0009
  /* Mobile Station Loopback (IS-126) */

#define CAI_SO_ASYNC_DATA_13K_PRE707     0x000C /* IS-99 s.o. number  */
  /* Asynchronous Data Service (IS-99) over Rate set 2 */

#define CAI_SO_G3_FAX_13K_PRE707         0x000D /* IS-99 s.o. number  */
  /* Group 3 FAX Service (IS-99) over Rate set 2 */

#define CAI_SO_RS2_SMS                   0x000E
  /* Short Message Services using Mux Option 2 (TSB-79) */

#define CAI_SO_PPP_PKT_DATA_13K_PRE707  0x000F /* IS-657 s.o. number */
  /* Internet Standard PPP Packet Data Service (IS-657) over Rate set 2 */

#define CAI_SO_VOICE_13K_IS733          0X0011 /* IS-733 s.o. number */
  /* High Rate Voice Service (13 kbps) IS-733 */

#define CAI_SO_RS1_OTAPA                0x0012 /* IS-683A s.o. number */
  /* Over The Air Parameter Administration over Rate set 1 */

#define CAI_SO_RS2_OTAPA                0x0013 /* IS-683A s.o. number */
  /* Over The Air Parameter Administration over Rate set 2 */

#define CAI_SO_MDR_PKT_DATA_FRS1_RRS1   0x0016 /* IS-707A s.o. number = 22 */
  /* Medium Data Rate (externally known as High Speed Data) PPP Packet Data Service
   (IS-707A) with rate set 1 forward and reverse.  Default Mux = 9 forward 1 reverse */

#define CAI_SO_MDR_PKT_DATA_FRS1_RRS2   0x0017 /* IS-707A s.o. number = 23 */
  /* Medium Data Rate (externally known as High Speed Data) PPP Packet Data Service
   (IS-707A) with rate set 1 forward and rate set 2 reverse.
   Default Mux = 9 forward 2 reverse.  This SO is not support by MSM3000. */

#define CAI_SO_MDR_PKT_DATA_FRS2_RRS1   0x0018 /* IS-707A s.o. number = 24 */
  /* Medium Data Rate (externally known as High Speed Data) PPP Packet Data Service
   (IS-707A) with rate set 2 forward and rate set 1 reverse.
   Default Mux = 10 forward 1 reverse.  This SO is not support by MSM3000. */

#define CAI_SO_MDR_PKT_DATA_FRS2_RRS2   0x0019 /* IS-707A s.o. number = 25 */
  /* Medium Data Rate (externally known as High Speed Data) PPP Packet Data Service
   (IS-707A) with rate set 2 forward and reverse.  Default Mux = 10 forward 2 reverse */

#define CAI_SO_RS1_PD                   0x0023
  /* IS-801 Position Determination Service Option 35 for rate set 1  */
#define CAI_SO_RS2_PD                   0x0024
  /* IS-801 Position Determination Service Option 36 for rate set 2  */

#define CAI_SO_MDR_DPT_FRS1_RRS1        0x8024 /* MDR Data Pattern Test: Proprietary
                                               Service Option rate set 1 fwd and rev */

#define CAI_SO_MDR_DPT_FRS2_RRS2        0x8025 /* MDR Data Pattern Test: Proprietary
                                               Service Option rate set 2 fwd and rev */

#define CAI_SO_ANALOG_FAX_RS1    0x0014 /* IS-707A */
  /* Analog End to End Fax */

#define CAI_SO_ANALOG_FAX_RS2  0x0015 /* IS-707A */
  /* Analog End to End Fax */

#define CAI_SO_ASYNC_DATA_IS707         0x1004 /* decimal 4100 */
  /* Asynchronous Data Service (IS-707) */

#define CAI_SO_G3_FAX_IS707             0x1005 /* decimal 4101 */
  /* Group 3 FAX Service (IS-707) */

#define CAI_SO_PPP_PKT_DATA_IS707       0x1007 /* decimal 4103 */
  /* Internet Standard PPP Packet Data Service (IS-707) */

#define CAI_SO_VOICE_13K    0x8000
 /* Speech Service Option (13K) */

#define CAI_SO_VOICE_IS96   0x8001
 /* Speech Service Option (IS-96) */

#define CAI_SO_MARKOV       0x8002
  /* Markov Service Option */

#define CAI_SO_DATA         0x8003
  /* Data Services Service Option, HACK Packet method */

#define CAI_SO_MARKOV_13K   0x801C
  /* Markov Service Option (13k) */

#define CAI_SO_RS1_MARKOV   0x801E
  /* Markov Service Option (8k) */

#define CAI_SO_RS2_MARKOV   0x801F
  /* Markov Service Option (13k) */

#define CAI_SO_WLL_OFF_HOOK 0x801D
  /* WLL phone off hook Service Option */

#ifdef FEATURE_IS2000
#define CAI_SO_PPP_PKT_DATA_3G 0x0021
  /* CDMA2000 pkt service option */

#ifdef FEATURE_IS2000_TDSO
/* Service option for Simple TDSO */
#define CAI_SO_TDSO         0x8008
/* Service option for Full TDSO */
#define CAI_SO_FULL_TDSO    0x0020 
#endif
#endif /* FEATURE_IS2000 */

#define CAI_SO_PPP_PKT_DATA_13K_PROPTRY 0x8020 /* QC proprietary number */
  /* Internet Standard PPP Packet Data Service (QC Prptry) over Rate set 2 */

#define CAI_SO_ASYNC_DATA_13K_PROPTRY   0x8021 /* QC proprietary number */
  /* Asynchronous Data Service (QC Proptry) over Rate set 2 */

#define CAI_SO_G3_FAX_13K_PROPTRY       0x8022 /* QC proprietary number */
  /* Group 3 FAX Service (QC Proptry) over Rate set 2 */

#define CAI_SO_NULL         0xFFFF
  /* NULL value for service option negotiation */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                   AUTHENTICATON MODES                                   */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_NO_AUTH 0x00
#define CAI_AUTH    0x01

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                 MESSAGE ENCRYPTION MODES                                */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.2.3.2.8-2. Message Encryption Modes                */

#define CAI_ENCRYPTION_DISABLED   0x00
  /* Encryption disabled */
#define CAI_ENCRYPT_CALL_CTL_MSGS 0x01
  /* Encrypt call control messages */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                    ASSIGNMENT MODES                                     */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.2.3.2.8-1. Assignment Mode                         */

#define CAI_TRAFFIC_CHAN_ASSIGN 0x00
  /* Traffic Channel Assignment */
#define CAI_PAGING_CHAN_ASSIGN  0x01
  /* Paging Channel Assignment */
#define CAI_ACQ_ANALOG_SYSTEM   0x02
  /* Acquire Analog System */
#define CAI_ANALOG_VCHAN_ASSIGN 0x03
  /* Analog Voice Channel Assignment */
#define CAI_EXT_TRAF_CHAN_ASSIGN 0x04
  /* Extended Traffic Channel Assignment */
#define CAI_EXT_PAGING_CHAN_ASSIGN 0x05
  /* Extended Paging Channel Assignment */

#ifdef FEATURE_SKT_ASSIGN_MODE
#define CAI_SO1213_TRAFFIC_CHAN_ASSIGN 0x07
  /* SO 12 or 13 Traffic Channel Assignment */
#endif /* FEATURE_SKT_ASSIGN_MODE */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                       GRANTED MODES                                     */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95-A TSB                                                         */

#define CAI_GRANT_USE_DC  0x0  /* Granted mode 0. In IS-95A it means to use
                                * mux1 and rate set 1, and perform service
                                * negotiation on tc. In IS-95B it means to
                                * use the default config value (see below).*/
#define CAI_GRANT_DEF   0x1  /* Granted mode 1. Use default mux and rate
                                set for requested service option, and
                                perform service negotiation on tc. */
#define CAI_GRANT_STR   0x2  /* Granted mode 2 (streamlined). Use default
                                mux and rate set for requested service
                                option, but do NOT perform service
                                negotiation on tc. */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   */
/*                       DEFAULT CONFIG                                   */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   */
/* See ANSI/TIA/EIA-95-B / IS-95-B                                        */

#define CAI_DEF_CONFIG_FOR1_REV1  0x0  /* Multiplex Option 1 and Rate Set 1 for
                             both the forward and reverse traffic channel */
#define CAI_DEF_CONFIG_FOR2_REV2  0x1  /* Multiplex Option 2 and Rate Set 2 for
                             both the forward and reverse traffic channel */
#define CAI_DEF_CONFIG_FOR1_REV2  0x2  /* Multiplex Option 1 and Rate Set 1 for
                                 the forward traffic channel and Multiplex
                                 Option 2 and Rate Set 2 for reverse traffic
                                 channel */
#define CAI_DEF_CONFIG_FOR2_REV1  0x3  /* Multiplex Option 2 and Rate Set 2 for
                                 the forward traffic channel and Multiplex
                                 Option 1 and Rate Set 1 for reverse traffic
                                 channel */

#define CAI_DEF_CONFIG_USE_ASSGN_RC  0x4  /* Use RC from the E/CAM msg */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                         BAND CLASSES                                    */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95-A TSB                                                         */

#ifdef FEATURE_T53
#error code not present
#else
#define CAI_BAND_CELL  0  /* 800 MHz cellular band */
#endif /* FEATURE_T53 */

#ifdef FEATURE_KPCS
#error code not present
#else
#define CAI_BAND_PCS   1  /* 1.8 to 2.0 GHz band for PCS */
#endif /* FEATURE_KPCS */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                       OPERATING MODES                                   */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95-A TSB                                                         */
#define CAI_J_STD_008_OP_MODE 0x00 /* ANSI J-STD-008 mode */
#define CAI_IS95_CDMA_BC1_OP_MODE 0x00 /* IS-95B Band Class 1 */
#define CAI_IS95_CDMA_BC0_OP_MODE 0x01 /* IS-95B Band Class 0 */
#define CAI_IS95_CDMA_OP_MODE 0x01 /* IS-95 CDMA mode */
#define CAI_IS95_ANAL_OP_MODE 0x02 /* IS-95 analog mode */
#define CAI_IS91_WIDE_OP_MODE 0x03 /* IS-91 wide analog mode */
#define CAI_IS91_NARR_OP_MODE 0x04 /* IS-91 narrow analog mode */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                 QUALIFICATION INFORMATION TYPES                         */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95-A TSB                                                         */

#define CAI_NO_QUAL           0x00 /* No qualification */
#define CAI_BAND_QUAL         0x01 /* Band class qualification */
#define CAI_BAND_OP_MODE_QUAL 0x02 /* Band class and operating mode
                                      qualification */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                   REGISTRATION TYPES                                    */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.1.3.2.1-1. Registration Type (REG_TYPE) Codes      */

#define CAI_TIMER_REG     0x00
  /* Timer-based registration */
#define CAI_POWERUP_REG   0x01
  /* Power-up registration */
#define CAI_ZONE_REG      0x02
  /* Zone-based registration */
#define CAI_POWERDOWN_REG 0x03
  /* Power-down registration */
#define CAI_PARAMETER_REG 0x04
  /* Parameter-change registration */
#define CAI_ORDERED_REG   0x05
  /* Ordered registration */
#define CAI_DISTANCE_REG  0x06
  /* Distance-based registration */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  RETURN CAUSE CODES                                     */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95B Table 6.7.1.3.2.1-2. RETURN CAUSE codes                      */
#define CAI_NORMAL_ACCESS_RETURN       0x0
  /* Normal access */
#define CAI_SYSTEM_NOT_ACQ_RETURN      0x1
  /* System not acquired */
#define CAI_PROTOCOL_MISMATCH_RETURN   0x2
  /* Protocol mismatch encountered */
#define CAI_REG_REJ_RETURN             0x3
  /* Registration rejection return */
#define CAI_WRONG_SID_RETURN           0x4
  /* Wrong SID return */
#define CAI_WRONG_NID_RETURN           0x5
  /* Wrong NID return */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  REDIRECTION        TYPES                               */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.2.3.2.16-1. Redirection        Types               */
#define CAI_NORMAL_REDIRECTION       0x0
  /* Standard Redirection indicator */
#define CAI_NDSS_REDIRECTION         0x1
  /* IS95-B NDSS Redirection indicator */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  REDIRECTION RECORD TYPES                               */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.2.3.2.16-1. Redirection Record Types               */
#define CAI_NDSS_OFF_INDICATION    0x00
  /* IS-95B addition, no type specific fields included */
#define CAI_ANALOG_REDIRECTION_REC 0x01
  /* Redirection to an analog system */
#define CAI_CDMA_REDIRECTION_REC   0x02
  /* Redirection to a CDMA system */
#define CAI_TACS_REDIRECTION_REC   0x03
  /* Redirection to a TACS analog system */
#define CAI_JTACS_REDIRECTION_REC  0x04
  /* Redirection to a JTACS analog system */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  ANALOG CHANNEL TYPES                                   */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 7.7.3.3.2.6-1. Analog Channel Types                    */

#define CAI_ANALOG_WIDE_CHAN        0x0
  /* Wide channel on ANALOG_CHAN */
#define CAI_ANALOG_NARROW_BELOW     0x1
  /* Narrow channel 10 kHz below ANALOG_CHAN */
#define CAI_ANALOG_NARROW_ABOVE     0x2
  /* Narrow channel 10 kHz above ANALOG_CHAN */
#define CAI_ANALOG_NARROW_CENTERED  0x3
  /* Narrow channel centered on ANALOG_CHAN */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*          SETTABLE AND RETRIEVABLE PARAMETERS                            */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* The following parameters are from IS-95A Table E-1.                     */

#define CAI_MUX1_REV_1_ID 1
  /* Number of Reverse Traffic Channel 9600 bps frames with primary traffic
     only */
#define CAI_MUX1_REV_1_LEN 24
  /* Length of CAI_MUX1_REV_1 parameter */

#define CAI_MUX1_REV_2_ID 2
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/2 primary and signaling traffic */
#define CAI_MUX1_REV_2_LEN 24
  /* Length of CAI_MUX1_REV_2 parameter */

#define CAI_MUX1_REV_3_ID 3
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and_burst with
     rate 1/4 primary and signaling traffic */
#define CAI_MUX1_REV_3_LEN 24
  /* Length of CAI_MUX1_REV_3 parameter */

#define CAI_MUX1_REV_4_ID 4
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and_burst with
     rate 1/8 primary and signaling traffic */
#define CAI_MUX1_REV_4_LEN 24
  /* Length of CAI_MUX1_REV_4 parameter */

#define CAI_MUX1_REV_5_ID 5
  /* Number of Reverse Traffic Channel 9600 bps frame, blank-and-burst with
     signaling traffic only */
#define CAI_MUX1_REV_5_LEN 24
  /* Length of CAI_MUX1_REV_5 parameter */

#define CAI_MUX1_REV_6_ID 6
  /* Number of Reverse Traffic Channel 4800 bps frames with primary traffic
     only */
#define CAI_MUX1_REV_6_LEN 24
  /* Length of CAI_MUX1_REV_6 parameter */

#define CAI_MUX1_REV_7_ID 7
  /* Number of Reverse Traffic Channel 2400 bps frames with primary traffic
     only */
#define CAI_MUX1_REV_7_LEN 24
  /* Length of CAI_MUX1_REV_7 parameter */

#define CAI_MUX1_REV_8_ID 8
  /* Number of Reverse Traffic Channel 1200 bps frmaes primary traffic or
     null Traffic Channel data only */
#define CAI_MUX1_REV_8_LEN 24
  /* Length of CAI_MUX1_REV_8 parameter */

#define CAI_MUX1_REV_9_ID 9
  /* Reserved */
#define CAI_MUX1_REV_10_ID 10
  /* Reserved */

#define CAI_MUX1_REV_11_ID 11
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/2 primary and secondary traffic */
#define CAI_MUX1_REV_11_LEN 24
  /* Length of CAI_MUX1_REV_11 parameter */

#define CAI_MUX1_REV_12_ID 12
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and_burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX1_REV_12_LEN 24
  /* Length of CAI_MUX1_REV_12 parameter */

#define CAI_MUX1_REV_13_ID 13
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and_burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX1_REV_13_LEN 24
  /* Length of CAI_MUX1_REV_13 parameter */

#define CAI_MUX1_REV_14_ID 14
  /* Number of Reverse Traffic Channel 9600 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX1_REV_14_LEN 24
  /* Length of CAI_MUX1_REV_14 parameter */

#define CAI_MUX1_FOR_1_ID 15
  /* Number of Forward Traffic Channel 9600 bps primary traffic only frames */
#define CAI_MUX1_FOR_1_LEN 24
  /* Length of CAI_MUX1_FOR_1 parameter */

#define CAI_MUX1_FOR_2_ID 16
  /* Number of Forward Traffic Channel 9600 bps frames dim-and-burst with
     rate 1/2 primary and signaling traffic */
#define CAI_MUX1_FOR_2_LEN 24
  /* Length of CAI_MUX1_FOR_2 parameter */

#define CAI_MUX1_FOR_3_ID 17
  /* Number of Forward Traffic Channel 9600 bps frames dim-and-burst with
     rate 1/4 primary and signaling traffic */
#define CAI_MUX1_FOR_3_LEN 24
  /* Length of CAI_MUX1_FOR_3 parameter */

#define CAI_MUX1_FOR_4_ID 18
  /* Number of Forward Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/8 primary and signaling traffic */
#define CAI_MUX1_FOR_4_LEN 24
  /* Length of CAI_MUX1_FOR_4 parameter */

#define CAI_MUX1_FOR_5_ID 19
  /* Number of Forward Traffic Channel 9600 bps frames, blank-and-burst with
     signaling traffic only */
#define CAI_MUX1_FOR_5_LEN 24
  /* Length of CAI_MUX1_FOR_5 parameter */

#define CAI_MUX1_FOR_6_ID 20
  /* Number of Forward Traffic Channel 4800 bps frames, primary traffic only */
#define CAI_MUX1_FOR_6_LEN 24
  /* Length of CAI_MUX1_FOR_6 parameter */

#define CAI_MUX1_FOR_7_ID 21
  /* Number of Forward Traffic Channel 2400 bps frames, primary traffic only */
#define CAI_MUX1_FOR_7_LEN 24
  /* Length of CAI_MUX1_FOR_7 parameter */

#define CAI_MUX1_FOR_8_ID 22
  /* Number of Forward Traffic Channel 1200 bps frames, primary traffic or
     null Traffic Channel data only */
#define CAI_MUX1_FOR_8_LEN 24
  /* Length of CAI_MUX1_FOR_8 parameter */

#define CAI_MUX1_FOR_9_ID 23
  /* Number of Forward Traffic Channel 9600 bps frames primary traffic only,
     probable bit errors */
#define CAI_MUX1_FOR_9_LEN 24
  /* Length of CAI_MUX1_FOR_9 parameter */

#define CAI_MUX1_FOR_10_ID 24
  /* Number of Forward Traffic Channel frames with frame quality insufficient
     to decide upon rate */
#define CAI_MUX1_FOR_10_LEN 24
  /* Length of CAI_MUX1_FOR_10 parameter */


#define CAI_MUX1_FOR_11_ID 25
  /* Number of Forward Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/2 primary and secondary traffic */
#define CAI_MUX1_FOR_11_LEN 24
  /* Length of CAI_MUX1_FOR_11 parameter */

#define CAI_MUX1_FOR_12_ID 26
  /* Number of Forward Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX1_FOR_12_LEN 24
  /* Length of CAI_MUX1_FOR_12 parameter */

#define CAI_MUX1_FOR_13_ID 27
  /* Number of Forward Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX1_FOR_13_LEN 24
  /* Length of CAI_MUX1_FOR_13 parameter */

#define CAI_MUX1_FOR_14_ID 28
  /* Number of Forward Traffic Channel 9600 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX1_FOR_14_LEN 24
  /* Length of CAI_MUX1_FOR_14 parameter */

#define CAI_PAG_1_ID 29
  /* Number of Paging Channel messages attempted to be received */
#define CAI_PAG_1_LEN 24
  /* Length of CAI_PAG_1_ID parameter */

#define CAI_PAG_2_ID 30
  /* Number of Paging Channel msgs received with a CRC that does not check */
#define CAI_PAG_2_LEN 24
  /* Length of CAI_PAG_2_ID parameter */

#define CAI_PAG_3_ID 31
  /* Number of Paging Channel messages received, addressed to mobile */
#define CAI_PAG_3_LEN 16
  /* Length of CAI_PAG_3 parameter */

#define CAI_PAG_4_ID 32
  /* Number of Paging Channel half frames received by the mobile */
#define CAI_PAG_4_LEN 24
  /* Length of CAI_PAG_4 parameter */

#define CAI_PAG_5_ID 33
  /* Number of Paging Channel half frames that contain any part of a message
     with a CRC that checks */
#define CAI_PAG_5_LEN 24
  /* Length of CAI_PAG_5 parameter */

#define CAI_PAG_6_ID 34
  /* Number of times that the mobile station declared a loss of the Paging
     Channel */
#define CAI_PAG_6_LEN 16
  /* Length of CAI_PAG_6 parameter */

#define CAI_PAG_7_ID 35
  /* Number of mobile station idle handoffs */
#define CAI_PAG_7_LEN 16
  /* Length of CAI_PAG_7 parameter */

#define CAI_ACC_1_ID 36
  /* Number of Access Channel request messages generated by layer 3 */
#define CAI_ACC_1_LEN 16
  /* Length of CAI_ACC_1 parameter */

#define CAI_ACC_2_ID 37
  /* Number of Access Channel response messages generated by layer 3 */
#define CAI_ACC_2_LEN 16
  /* Length of CAI_ACC_2 parameter */

#define CAI_ACC_3_ID 38
  /* Number of times access probe transmitted at least twice */
#define CAI_ACC_3_LEN 16
  /* Length of CAI_ACC_3 parameter */

#define CAI_ACC_4_ID 39
  /* Number of times access probe transmitted at least 3 times */
#define CAI_ACC_4_LEN 16
  /* Length of CAI_ACC_4 parameter */

#define CAI_ACC_5_ID 40
  /* Number of times access probe transmitted at least 4 times */
#define CAI_ACC_5_LEN 16
  /* Length of CAI_ACC_5 parameter */

#define CAI_ACC_6_ID 41
  /* Number of times access probe transmitted at least 5 times */
#define CAI_ACC_6_LEN 16
  /* Length of CAI_ACC_6 parameter */

#define CAI_ACC_7_ID 42
  /* Number of times access probe transmitted at least 6 times */
#define CAI_ACC_7_LEN 16
  /* Length of CAI_ACC_7 parameter */

#define CAI_ACC_8_ID 43
  /* Number of unsuccessful access attempts */
#define CAI_ACC_8_LEN 16
  /* Length of CAI_ACC_8 parameter */

#define CAI_LAYER2_RTC1_ID 44
  /* Number of messages requring acknowledgement which were transmitted at
     least once on the Reverse Traffic Channel */
#define CAI_LAYER2_RTC1_LEN 16
  /* Length of CAI_LAYER2_RTC1 parameter */

#define CAI_LAYER2_RTC2_ID 45
  /* Number of messages requring acknowledgement which were transmitted at
     least twice on the Reverse Traffic Channel */
#define CAI_LAYER2_RTC2_LEN 16
  /* Length of CAI_LAYER2_RTC2 parameter */

#define CAI_LAYER2_RTC3_ID 46
  /* Number of messages requring acknowledgement which were transmitted at
     least 3 times on the Reverse Traffic Channel */
#define CAI_LAYER2_RTC3_LEN 16
  /* Length of CAI_LAYER2_RTC3 parameter */

#define CAI_LAYER2_RTC4_ID 47
  /* Number of times mobile aborted call because of timeout expiring after
     the N1m transmission of a message requring an acknowledgement.  */
#define CAI_LAYER2_RTC4_LEN 16
  /* Length of CAI_LAYER2_RTC4 parameter */

#define CAI_LAYER2_RTC5_ID 48
  /* Number of times a message not requiring an acknowledgement was sent on
     the Reverse Traffic Channel */
#define CAI_LAYER2_RTC5_LEN 16
  /* Length of CAI_LAYER2_RTC5 parameter */

#define CAI_OTH_SYS_TIME_ID 49
  /* The SYS_TIME field from the most recently received Sync Channel Message */
#define CAI_OTH_SYS_TIME_LEN 36
  /* Length of CAI_OTH_SYS_TIME parameter */

#define CAI_MUX2_REV_1_ID 50
  /* Number of Reverse Traffic Channel 14400 bps frames with primary traffic
     or Null Traffic only */
#define CAI_MUX2_REV_1_LEN 24
  /* Length of CAI_MUX2_REV_1 parameter */

#define CAI_MUX2_REV_2_ID 51
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/2 primary and signaling traffic */
#define CAI_MUX2_REV_2_LEN 24
  /* Length of CAI_MUX2_REV_2 parameter */

#define CAI_MUX2_REV_3_ID 52
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and_burst with
     rate 1/4 primary and signaling traffic */
#define CAI_MUX2_REV_3_LEN 24
  /* Length of CAI_MUX2_REV_3 parameter */

#define CAI_MUX2_REV_4_ID 53
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and_burst with
     rate 1/8 primary and signaling traffic */
#define CAI_MUX2_REV_4_LEN 24
  /* Length of CAI_MUX2_REV_4 parameter */

#define CAI_MUX2_REV_5_ID 54
  /* Number of Reverse Traffic Channel 14400 bps frame, blank-and-burst with
     signaling traffic only */
#define CAI_MUX2_REV_5_LEN 24
  /* Length of CAI_MUX2_REV_5 parameter */

#define CAI_MUX2_REV_6_ID 55
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/2 primary and secondary traffic */
#define CAI_MUX2_REV_6_LEN 24
  /* Length of CAI_MUX2_REV_6 parameter */

#define CAI_MUX2_REV_7_ID 56
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX2_REV_7_LEN 24
  /* Length of CAI_MUX2_REV_7 parameter */

#define CAI_MUX2_REV_8_ID 57
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_REV_8_LEN 24
  /* Length of CAI_MUX2_REV_8 parameter */

#define CAI_MUX2_REV_9_ID 58
  /* Number of Reverse Traffic Channel 14400 bps frame, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_REV_9_LEN 24
  /* Length of CAI_MUX2_REV_9 parameter */

#define CAI_MUX2_REV_10_ID 59
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/8 primary, secondary and signalling traffic */
#define CAI_MUX2_REV_10_LEN 24
  /* Length of CAI_MUX2_REV_10 parameter */

#define CAI_MUX2_REV_11_ID 60
  /* Number of Reverse Traffic Channel 7200 bps frames with primary traffic
     or Null Traffic only */
#define CAI_MUX2_REV_11_LEN 24
  /* Length of CAI_MUX2_REV_11 parameter */

#define CAI_MUX2_REV_12_ID 61
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/4 primary and signalling traffic */
#define CAI_MUX2_REV_12_LEN 24
  /* Length of CAI_MUX2_REV_12 parameter */

#define CAI_MUX2_REV_13_ID 62
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/8 primary and signalling traffic */
#define CAI_MUX2_REV_13_LEN 24
  /* Length of CAI_MUX2_REV_13 parameter */

#define CAI_MUX2_REV_14_ID 63
  /* Number of Reverse Traffic Channel 7200 bps frames, blank-and-burst with
     signalling traffic only */
#define CAI_MUX2_REV_14_LEN 24
  /* Length of CAI_MUX2_REV_14 parameter */

#define CAI_MUX2_REV_15_ID 64
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX2_REV_15_LEN 24
  /* Length of CAI_MUX2_REV_15 parameter */

#define CAI_MUX2_REV_16_ID 65
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_REV_16_LEN 24
  /* Length of CAI_MUX2_REV_16 parameter */

#define CAI_MUX2_REV_17_ID 66
  /* Number of Reverse Traffic Channel 7200 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_REV_17_LEN 24
  /* Length of CAI_MUX2_REV_17 parameter */

#define CAI_MUX2_REV_18_ID 67
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/8 primary, secondary and signalling traffic */
#define CAI_MUX2_REV_18_LEN 24
  /* Length of CAI_MUX2_REV_18 parameter */

#define CAI_MUX2_REV_19_ID 68
  /* Number of Reverse Traffic Channel 3600 bps frames with primary traffic
     or Null Traffic only */
#define CAI_MUX2_REV_19_LEN 24
  /* Length of CAI_MUX2_REV_19 parameter */

#define CAI_MUX2_REV_20_ID 69
  /* Number of Reverse Traffic Channel 3600 bps frames, dim-and-burst with
     rate 1/8 primary and signalling traffic */
#define CAI_MUX2_REV_20_LEN 24
  /* Length of CAI_MUX2_REV_20 parameter */

#define CAI_MUX2_REV_21_ID 70
  /* Number of Reverse Traffic Channel 14400 bps frames, blank-and-burst with
     signalling traffic only */
#define CAI_MUX2_REV_21_LEN 24
  /* Length of CAI_MUX2_REV_21 parameter */

#define CAI_MUX2_REV_22_ID 71
  /* Number of Reverse Traffic Channel 3600 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_REV_22_LEN 24
  /* Length of CAI_MUX2_REV_22 parameter */

#define CAI_MUX2_REV_23_ID 72
  /* Number of Reverse Traffic Channel 3600 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_REV_23_LEN 24
  /* Length of CAI_MUX2_REV_23 parameter */

#define CAI_MUX2_REV_24_ID 73
  /* Number of Reverse Traffic Channel 1800 bps frames with primary traffic
     or Null Traffic only */
#define CAI_MUX2_REV_24_LEN 24
  /* Length of CAI_MUX2_REV_24 parameter */

#define CAI_MUX2_REV_25_ID 74
  /* Number of Reverse Traffic Channel 1800 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_REV_25_LEN 24
  /* Length of CAI_MUX2_REV_25 parameter */

#define CAI_MUX2_REV_26_ID 75
  /* Reserved */


#define CAI_MUX2_FOR_1_ID  76
  /* Number of Forward Traffic Channel 14400 bps primary traffic only frames */
#define CAI_MUX2_FOR_1_LEN 24
  /* Length of CAI_MUX2_FOR_1 parameter */

#define CAI_MUX2_FOR_2_ID 77
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/2 primary and signaling traffic */
#define CAI_MUX2_FOR_2_LEN 24
  /* Length of CAI_MUX2_FOR_2 parameter */

#define CAI_MUX2_FOR_3_ID 78
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/4 primary and signaling traffic */
#define CAI_MUX2_FOR_3_LEN 24
  /* Length of CAI_MUX2_FOR_3 parameter */

#define CAI_MUX2_FOR_4_ID 79
  /* Number of Forward Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/8 primary and signaling traffic */
#define CAI_MUX2_FOR_4_LEN 24
  /* Length of CAI_MUX2_FOR_4 parameter */

#define CAI_MUX2_FOR_5_ID 80
  /* Number of Forward Traffic Channel 14400 bps frames, blank-and-burst with
     signaling traffic only */
#define CAI_MUX2_FOR_5_LEN 24
  /* Length of CAI_MUX2_FOR_5 parameter */

#define CAI_MUX2_FOR_6_ID 81
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/2 primary and secondary traffic */
#define CAI_MUX2_FOR_6_LEN 24
  /* Length of CAI_MUX2_FOR_6 parameter */

#define CAI_MUX2_FOR_7_ID 82
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX2_FOR_7_LEN 24
  /* Length of CAI_MUX2_FOR_7 parameter */

#define CAI_MUX2_FOR_8_ID 83
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_FOR_8_LEN 24
  /* Length of CAI_MUX2_FOR_8 parameter */

#define CAI_MUX2_FOR_9_ID 84
  /* Number of Forward Traffic Channel 14400 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_FOR_9_LEN 24
  /* Length of CAI_MUX2_FOR_9 parameter */

#define CAI_MUX2_FOR_10_ID 85
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/8 primary, secondary and signalling traffic */
#define CAI_MUX2_FOR_10_LEN 24
  /* Length of CAI_MUX2_FOR_10 parameter */

#define CAI_MUX2_FOR_11_ID 86
  /* Number of Forward Traffic Channel 7200 bps frames, primary traffic only */
#define CAI_MUX2_FOR_11_LEN 24
  /* Length of CAI_MUX2_FOR_11 parameter */

#define CAI_MUX2_FOR_12_ID 87
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/4 primary and signalling traffic */
#define CAI_MUX2_FOR_12_LEN 24
  /* Length of CAI_MUX2_FOR_12 parameter */

#define CAI_MUX2_FOR_13_ID 88
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/8 primary and signalling traffic */
#define CAI_MUX2_FOR_13_LEN 24
  /* Length of CAI_MUX2_FOR_13 parameter */

#define CAI_MUX2_FOR_14_ID 89
  /* Number of Forward Traffic Channel 7200 bps frames, blank-and-burst with
     signalling traffic only */
#define CAI_MUX2_FOR_14_LEN 24
  /* Length of CAI_MUX2_FOR_14 parameter */

#define CAI_MUX2_FOR_15_ID 90
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX2_FOR_15_LEN 24
  /* Length of CAI_MUX2_FOR_15 parameter */

#define CAI_MUX2_FOR_16_ID 91
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_FOR_16_LEN 24
  /* Length of CAI_MUX2_FOR_16 parameter */

#define CAI_MUX2_FOR_17_ID 92
  /* Number of Forward Traffic Channel 7200 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_FOR_17_LEN 24
  /* Length of CAI_MUX2_FOR_17 parameter */

#define CAI_MUX2_FOR_18_ID 93
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/8 primary, secondary and signalling traffic */
#define CAI_MUX2_FOR_18_LEN 24
  /* Length of CAI_MUX2_FOR_18 parameter */

#define CAI_MUX2_FOR_19_ID 94
  /* Number of Forward Traffic Channel 3600 bps frames, primary traffic only */
#define CAI_MUX2_FOR_19_LEN 24
  /* Length of CAI_MUX2_FOR_19 parameter */

#define CAI_MUX2_FOR_20_ID 95
  /* Number of Forward Traffic Channel 3600 bps frames, dim-and-burst with
     rate 1/8 primary and signalling traffic */
#define CAI_MUX2_FOR_20_LEN 24
  /* Length of CAI_MUX2_FOR_20 parameter */

#define CAI_MUX2_FOR_21_ID 96
  /* Number of Forward Traffic Channel 3600 bps frames, blank-and-burst with
     signalling traffic only */
#define CAI_MUX2_FOR_21_LEN 24
  /* Length of CAI_MUX2_FOR_21 parameter */

#define CAI_MUX2_FOR_22_ID 97
  /* Number of Forward Traffic Channel 3600 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_FOR_22_LEN 24
  /* Length of CAI_MUX2_FOR_22 parameter */

#define CAI_MUX2_FOR_23_ID 98
  /* Number of Forward Traffic Channel 3600 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_FOR_23_LEN 24
  /* Length of CAI_MUX2_FOR_23 parameter */

#define CAI_MUX2_FOR_24_ID 99
  /* Number of Forward Traffic Channel 1800 bps frames, primary traffic or
     null Traffic Channel data only */
#define CAI_MUX2_FOR_24_LEN 24
  /* Length of CAI_MUX2_FOR_24 parameter */

#define CAI_MUX2_FOR_25_ID 100
  /* Number of Forward Traffic Channel 1800 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_FOR_25_LEN 24
  /* Length of CAI_MUX2_FOR_25 parameter */

#define CAI_MUX2_FOR_26_ID 101
  /* Number of Forward Traffic Channel frames with frame quality insufficient
     to decide upon rate */
#define CAI_MUX2_FOR_26_LEN 24
  /* Length of CAI_MUX2_FOR_26 parameter */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
#define CAI_SUPP1_REV_S_ID 102
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP1_REV_S_LEN 24
  /* Length of CAI_SUPP1_REV_S parameter */
#define CAI_SUPP1_REV_P_ID 103
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP1_REV_P_LEN 24
  /* Length of CAI_SUPP1_REV_P parameter */
#define CAI_SUPP2_REV_S_ID 104
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP2_REV_S_LEN 24
  /* Length of CAI_SUPP2_REV_S parameter */
#define CAI_SUPP2_REV_P_ID 105
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP2_REV_P_LEN 24
  /* Length of CAI_SUPP2_REV_P parameter */
#define CAI_SUPP3_REV_S_ID 106
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP3_REV_S_LEN 24
  /* Length of CAI_SUPP3_REV_S parameter */
#define CAI_SUPP3_REV_P_ID 107
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP3_REV_P_LEN 24
  /* Length of CAI_SUPP3_REV_P parameter */
#define CAI_SUPP4_REV_S_ID 108
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP4_REV_S_LEN 24
  /* Length of CAI_SUPP4_REV_S parameter */
#define CAI_SUPP4_REV_P_ID 109
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP4_REV_P_LEN 24
  /* Length of CAI_SUPP4_REV_P parameter */
#define CAI_SUPP5_REV_S_ID 110
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP5_REV_S_LEN 24
  /* Length of CAI_SUPP5_REV_S parameter */
#define CAI_SUPP5_REV_P_ID 111
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP5_REV_P_LEN 24
  /* Length of CAI_SUPP5_REV_P parameter */
#define CAI_SUPP6_REV_S_ID 112
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP6_REV_S_LEN 24
  /* Length of CAI_SUPP6_REV_S parameter */
#define CAI_SUPP6_REV_P_ID 113
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP6_REV_P_LEN 24
  /* Length of CAI_SUPP6_REV_P parameter */
#define CAI_SUPP7_REV_S_ID 114
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP7_REV_S_LEN 24
  /* Length of CAI_SUPP7_REV_S parameter */
#define CAI_SUPP7_REV_P_ID 115
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP7_REV_P_LEN 24
  /* Length of CAI_SUPP7_REV_P parameter */

#define CAI_SUPP1_FWD_S_ID 116
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP1_FWD_S_LEN 24
  /* Length of CAI_SUPP1_FWD_S parameter */
#define CAI_SUPP1_FWD_P_ID 117
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP1_FWD_P_LEN 24
  /* Length of CAI_SUPP1_FWD_P parameter */
#define CAI_SUPP2_FWD_S_ID 118
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP2_FWD_S_LEN 24
  /* Length of CAI_SUPP2_FWD_S parameter */
#define CAI_SUPP2_FWD_P_ID 119
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP2_FWD_P_LEN 24
  /* Length of CAI_SUPP2_FWD_P parameter */
#define CAI_SUPP3_FWD_S_ID 120
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP3_FWD_S_LEN 24
  /* Length of CAI_SUPP3_FWD_S parameter */
#define CAI_SUPP3_FWD_P_ID 121
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP3_FWD_P_LEN 24
  /* Length of CAI_SUPP3_FWD_P parameter */
#define CAI_SUPP4_FWD_S_ID 122
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP4_FWD_S_LEN 24
  /* Length of CAI_SUPP4_FWD_S parameter */
#define CAI_SUPP4_FWD_P_ID 123
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP4_FWD_P_LEN 24
  /* Length of CAI_SUPP4_FWD_P parameter */
#define CAI_SUPP5_FWD_S_ID 124
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP5_FWD_S_LEN 24
  /* Length of CAI_SUPP5_FWD_S parameter */
#define CAI_SUPP5_FWD_P_ID 125
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP5_FWD_P_LEN 24
  /* Length of CAI_SUPP5_FWD_P parameter */
#define CAI_SUPP6_FWD_S_ID 126
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP6_FWD_S_LEN 24
  /* Length of CAI_SUPP6_FWD_S parameter */
#define CAI_SUPP6_FWD_P_ID 127
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP6_FWD_P_LEN 24
  /* Length of CAI_SUPP6_FWD_P parameter */
#define CAI_SUPP7_FWD_S_ID 128
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP7_FWD_S_LEN 24
  /* Length of CAI_SUPP7_FWD_S parameter */
#define CAI_SUPP7_FWD_P_ID 129
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP7_FWD_P_LEN 24
  /* Length of CAI_SUPP7_FWD_P parameter */
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */

#ifdef FEATURE_IS2000
/*---------------------------------------------------------------------------
  Gap for unsupported features.
---------------------------------------------------------------------------*/
#define CAI_SCH0_REV_1X_ID     231  /* Good 1x SDU frames sent on SCH 1    */
#define CAI_SCH0_REV_1X_LEN    24   /* Bit length of CAI_SCH0_REV_1X_P_ID  */

#define CAI_SCH0_REV_2X_ID     232  /* Good 2x SDU frames sent on SCH 1    */
#define CAI_SCH0_REV_2X_LEN    24   /* Bit length of CAI_SCH0_REV_2X_P_ID  */

#define CAI_SCH0_REV_4X_ID     233  /* Good 4x SDU frames sent on SCH 1    */
#define CAI_SCH0_REV_4X_LEN    24   /* Bit length of CAI_SCH0_REV_4X_P_ID  */

#define CAI_SCH0_REV_8X_ID     234  /* Good 8x SDU frames sent on SCH 1    */
#define CAI_SCH0_REV_8X_LEN    24   /* Bit length of CAI_SCH0_REV_8X_P_ID  */

#define CAI_SCH0_REV_16X_ID    235  /* Good 16x SDU framessentd on SCH 1   */
#define CAI_SCH0_REV_16X_LEN   24   /* Bit length of CAI_SCH0_REV_16X_P_ID */

#define CAI_SCH0_REV_LTU_ID    236  /* # ltu's sent on SCH 1               */
#define CAI_SCH0_REV_LTU_LEN   24   /* Bit length of CAI_SCH0_REV_LTU      */

#define CAI_SCH0_REV_LTUOK_ID  237  /* not used                            */
#define CAI_SCH0_REV_LTUOK_LEN 24   /* Bit length of CAI_SCH0_REV_LTUOK    */
/*---------------------------------------------------------------------------
  Blank gap for unsupported SCH 1
---------------------------------------------------------------------------*/
#define CAI_SCH0_FWD_1X_ID     245  /* Good 1x SDU frames rx'ed on SCH 1   */
#define CAI_SCH0_FWD_1X_LEN    24   /* Bit length of CAI_SCH0_FWD_1X_P_ID  */

#define CAI_SCH0_FWD_2X_ID     246  /* Good 2x SDU frames rx'ed on SCH 1   */
#define CAI_SCH0_FWD_2X_LEN    24   /* Bit length of CAI_SCH0_FWD_2X_P_ID  */

#define CAI_SCH0_FWD_4X_ID     247  /* Good 4x SDU frames rx'ed on SCH 1   */
#define CAI_SCH0_FWD_4X_LEN    24   /* Bit length of CAI_SCH0_FWD_4X_P_ID  */

#define CAI_SCH0_FWD_8X_ID     248  /* Good 8x SDU frames rx'ed on SCH 1   */
#define CAI_SCH0_FWD_8X_LEN    24   /* Bit length of CAI_SCH0_FWD_8X_P_ID  */

#define CAI_SCH0_FWD_16X_ID    249  /* Good 16x SDU frames rx'ed on SCH 1  */
#define CAI_SCH0_FWD_16X_LEN   24   /* Bit length of CAI_SCH0_FWD_16X_P_ID */

#define CAI_SCH0_FWD_LTU_ID    250  /* # ltu's received on SCH 1           */
#define CAI_SCH0_FWD_LTU_LEN   24   /* Bit length of CAI_SCH0_FWD_LTU      */

#define CAI_SCH0_FWD_LTUOK_ID  251  /* # good ltu's received on SCH 1      */
#define CAI_SCH0_FWD_LTUOK_LEN 24   /* Bit length of CAI_SCH0_FWD_LTUOK    */
#endif /* FEATURE_IS2000 */

#if defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR)
/*----------------------- Additional non-standard parameters ----------------*/
/* IS95B does not define CAI paramters for maintaining sup channel
   FER statistics. But we need FER on sup channels for debugging and
   testing purposes. Hence we have defined new CAI paramters here. Usage
   of XTRA is intentional and it is supposed to point out that these are
   not std CAI paramters */

#define CAI_XTRA_SUPP1_FWD_ERR_ID 0
  /* Number of Supplemental 0 Traffic channel frames in error */
#define CAI_XTRA_SUPP1_FWD_ERR_LEN 24
  /* Length of CAI_XTRA_SUPP1_FWD_ERR parameter */

#define CAI_XTRA_SUPP2_FWD_ERR_ID 1
  /* Number of Supplemental 1 Traffic channel frames in error */
#define CAI_XTRA_SUPP2_FWD_ERR_LEN 24
  /* Length of CAI_XTRA_SUPP1_FWD_ERR parameter */

#define CAI_XTRA_SUPP3_FWD_ERR_ID 2
  /* Number of Supplemental 2 Traffic channel frames in error */
#define CAI_XTRA_SUPP3_FWD_ERR_LEN 24
  /* Length of CAI_XTRA_SUPP1_FWD_ERR parameter */

#define CAI_XTRA_SUPP4_FWD_ERR_ID 3
  /* Number of Supplemental 3 Traffic channel frames in error */
#define CAI_XTRA_SUPP4_FWD_ERR_LEN 24
  /* Length of CAI_XTRA_SUPP1_FWD_ERR parameter */

#define CAI_XTRA_SUPP5_FWD_ERR_ID 4
  /* Number of Supplemental 4 Traffic channel frames in error */
#define CAI_XTRA_SUPP5_FWD_ERR_LEN 24
  /* Length of CAI_XTRA_SUPP1_FWD_ERR parameter */

#define CAI_XTRA_SUPP6_FWD_ERR_ID 5
  /* Number of Supplemental 5 Traffic channel frames in error */
#define CAI_XTRA_SUPP6_FWD_ERR_LEN 24
  /* Length of CAI_XTRA_SUPP1_FWD_ERR parameter */

#define CAI_XTRA_SUPP7_FWD_ERR_ID 6
  /* Number of Supplemental 6 Traffic channel frames in error */
#define CAI_XTRA_SUPP7_FWD_ERR_LEN 24
  /* Length of CAI_XTRA_SUPP1_FWD_ERR parameter */
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */

#ifdef FEATURE_IS2000
/* IS95C does not include all statistics that may be needed.  The folllowing
   are added to get better statistics from the mobile.
*/
#define CAI_XTRA_SCH0_FWD_SDU_ERR_ID  7   /* sdu erasures on SCH 1         */
#define CAI_XTRA_SCH0_FWD_SDU_ERR_LEN 24  /* length of sdu erasure field   */

#define CAI_XTRA_SCH0_FWD_SDU_DTX_ID  8   /* detected dtx on sch 1         */
#define CAI_XTRA_SCH0_FWD_SDU_DTX_LEN 24  /* length of detected dtx field  */

#define CAI_XTRA_SCH0_FWD_BLANK_PDU_ID  9  /* # blank PDU's received       */
#define CAI_XTRA_SCH0_FWD_BLANK_PDU_LEN 24 /* length of blank pdu field    */
/*-------------------------------------------------------------------------*/
#define CAI_XTRA_SCH0_REV_SDU_ERR_ID  10  /* sdu erasures on SCH 1-not used*/
#define CAI_XTRA_SCH0_REV_SDU_ERR_LEN 24  /* length of sdu erasure field   */

#define CAI_XTRA_SCH0_REV_SDU_DTX_ID  11  /* detected dtx on sch 1         */
#define CAI_XTRA_SCH0_REV_SDU_DTX_LEN 24  /* length of detected dtx field  */

#define CAI_XTRA_SCH0_REV_BLANK_PDU_ID  12 /* # blank PDU's received       */
#define CAI_XTRA_SCH0_REV_BLANK_PDU_LEN 24 /* length of blank pdu field    */
/*-------------------------------------------------------------------------*/
#define CAI_XTRA_SCH0_FWD_BAD_LTUS_ID  13 /* # failed LTU CRCs when when   */
                                          /* SDU CRC passes                */
#define CAI_XTRA_SCH0_FWD_BAD_LTUS_LEN 24 /* length of bad ltus field      */

#define CAI_XTRA_SCH0_FWD_SAVED_LTUS_ID  14 /* #ltu's recoverd when SDU CRC*/
                                            /* fails                       */
#define CAI_XTRA_SCH0_FWD_SAVED_LTUS_LEN 24 /* len of recoverd ltu fld     */
#endif /* FEATURE_IS2000 */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) || defined(FEATURE_IS2000)
/* The following two defines set limits of XTRA parms */
  #ifdef FEATURE_IS2000
    #define CAI_XTRA_FIRST_PARM_ID CAI_XTRA_SUPP1_FWD_ERR_ID
    #define CAI_XTRA_LAST_PARM_ID CAI_XTRA_SCH0_FWD_SAVED_LTUS_ID
  #else
    #define CAI_XTRA_FIRST_PARM_ID CAI_XTRA_SUPP1_FWD_ERR_ID
    #define CAI_XTRA_LAST_PARM_ID CAI_XTRA_SUPP7_FWD_ERR_ID
  #endif
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR || FEATURE_IS2000 */
/*---------------------------------------------------------------*/

#define CAI_FIRST_PARM_ID 1
  /* This must be set to the lowest valid Parameter ID */

#if defined (FEATURE_IS2000)
#define CAI_LAST_PARM_ID CAI_SCH0_FWD_LTUOK_ID
#else
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  #define CAI_LAST_PARM_ID CAI_SUPP7_FWD_P_ID
#else
  #define CAI_LAST_PARM_ID CAI_MUX2_FOR_26_ID
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */
#endif /* FEATURE_IS2000 */

  /* This must be set to the highest valid Parameter ID */
#define CAI_PARM_MIN_LEN 16  /* Minimum length for parameter */
#define CAI_PARM_MAX_LEN 36  /* Maximum length for a parameter */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  TIME-OUTS AND TIME LIMITS                              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table D-1. Time Limits                                       */

#define CAI_T1M 400
  /* Number of ms before mobile station times out its wait for an
     aknowledgement on the Forward Traffic Channel */
#define CAI_T2M 200
  /* Number of ms allowed for mobile to send an acknowlegement of a
     signaling message received on the Forward Traffic Channel */
#define CAI_T3M 320
  /* Number of ms during which the mobile will consider a mesage received
     on the Forward Traffic Channel not requiring an acknowledgement having
     the same sequence number as a previously received message to be a
     duplicate */
#define CAI_T4M 2200
  /* Number of ms during which the mobile will consider a mesage received
     on the Paging Channel from the same base station which has the same
     sequence number as a previously received message to be a duplicate
     the formula for the calculation is 4 * 2**slot_cycle_index + .2 sec */
#ifdef FEATURE_SKT_CAI
#define CAI_T5M 10000
#else
#define CAI_T5M 5000
#endif
  /* Number of ms for the Forward Traffic Channel Fade timer */
#define CAI_T20M 15000
  /* Number of ms for the Pilot Channel Acquisition substate timer */
#define CAI_T21M 1000
  /* Number of ms for the Receipt of Sync Channel message timeout */
#define CAI_T30M 3000
  /* Number of ms before timeout for a non-slotted mobile station to receive
     a Paging Channel message with a good CRC */
#define CAI_T31M 600000L
  /* Time for a mobile station to consider overhead messages valid */
#define CAI_T32M 5000
  /* Maximum time to enter the Update Overhead Information Substate of the
     System Access State to respond to an SSD Update Message, BS Challenge
     Confirmation Order, and Authentication Challenge Message */
#define CAI_T33M 300
  /* Maximum time to enter the Update Overhead Information Substate of the
     System Access State to respond to messages received while in the
     Mobile Station Idle State (except authentication messages) */
#define CAI_T34M 3000
  /* Maximum time to enter the Update Overhead Information Substate or the
     Mobile Station Idle State after receiving a Channel Assignment Message
     with ASSIGN_MODEr equal to '001' or '101' */

#ifdef FEATURE_IS95B
#define CAI_T40M 3000
  /* Maximum time to receive a valid Paging Channel message before aborting
     an access attempt */
#else
#define CAI_T40M 1000
  /* Maximum time to receive a valid Paging Channel message before aborting
     an access attempt */
#endif /* FEATURE_IS95B */

#define CAI_T41M 4000
  /* Maximum time to obtain updated overhead messages arriving
     on the Paging Channel */
#define CAI_T42M 12000
  /* System Access State timer while waiting for a delayed response, in ms */
#define CAI_T50M 1000
  /* First valid frame timeout, in ms */
#define CAI_T51M 2000
  /* First good message timeout */
#define CAI_T52M 5000
  /* Number of ms to be used for the Waiting For Order Substate timer */
#define CAI_T53M 65000U
  /* Number of ms to be used for the Waiting For Mobile Station Answer
     Substate timer */
#define CAI_T54M 200
  /* Maximum time for the mobile station to send an Origination Continuation
     Message upon entering the Conversation Substate of the Mobile Station
     Control on the Traffic Channel State */
#define CAI_T55M 2000
  /* Number of ms to be used for the Release Substate timer */
#define CAI_T56M 200
  /* Default maximum time to respond to a received message or order on the
     Forward Traffic Channel */
#define CAI_T57M 20000
  /* Limit of powerup registration timer */
#define CAI_T58M 5000
  /* Maximum time for the mobile station to respond to a service option
     request */
#define CAI_T59M 5000
  /* Maximum time for the mobile station to respond to a service request or
     service response message. */
#define CAI_T60M 60
  /* Maximum time to execute a hard handoff involving a new frequency
     assignment using the same base station */
#define CAI_T61M 80
  /* Maximum time to execute a hard handoff involving a new frequency
     assignment using a different base station */
#define CAI_T62M 20
  /* Maximum time to execute a hard handoff involving the same frequency
     assignment */
#define CAI_T63M 100
  /* Maximum time to execute a CDMA to Analog handoff */
#define CAI_T64M 10000
  /* Maximum time to receive BS Challenge Confimation during SSD Update */
#define CAI_T65M 5000
  /* Maximum time for the moible station to wait for a service connect
     message. */
#define CAI_T68M 5000
  /* Maximum time for the mobile station to wait for a service request
     or service response message. */
#define CAI_T69M 24000
  /* Fixed portion of the full-TMSI timer */

#ifdef FEATURE_IS95B_MAHHO
#define CAI_T71M 40
  /* Maximum time for the mobile station to send a Candidate Frequency
     Report Message after completing a search */
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS95B
#define CAI_T72M 1000
  /* Maximum time to receive a valid Paging Channel message before aborting
     an access attempt, when there exists at least one access handoff
     candidate pilot for the access attempt (see also T40M) */
#endif

#define CAI_T1B 1280
  /* Maximum period between subsequent transmissions of an overhead
     message on the Paging Channel by the base station */
#define CAI_T2B 800
  /* Maximum time for the base station to send a Release Order after
     receiving a Release Order */
#define CAI_T3B 300
  /* Maximum time the base station continues to transmit on a code channel
     after sending or receiving a Release Order */
#define CAI_T4B 5000
  /* Maximum time for the base station to respond to a service option
     request */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table D-2. Other Constants                                   */

#ifdef FEATURE_IS2000
#define CAI_N1M 13
  /* Maximum number of times that a mobile station transmits a message
   * requiring an acknowledgement on the Reverse Traffic Channel. */
  /* Note this value was increased in IS2000 and hence the feature define
  here, so that we do not affect targets that do not have to be IS2000
  compliant */
#else
#define CAI_N1M 9
  /* Maximum number of times that a mobile station transmits a message
   * requiring an acknowledgement on the Reverse Traffic Channel. */
#endif /* FEATURE_IS2000 */
#ifdef FEATURE_SKT_CAI
#define CAI_N2M 500
#else
#define CAI_N2M 12
#endif
  /* Number of consecutive Forward Traffic Channel frames placed into
     category 10 before a mobile station must disable its transmitter */
#define CAI_N3M 2
  /* Number of consecutive Forward Traffic Channel frames that must be
     placed into other than category 10 before a  mobile station is
     allowed to re-enable its transmitter after disabling its transmitter */
#define CAI_N5M 2
  /* Number of received consecutive good Forward Traffic Channel
     frames before a mobile enables its transmitter in TCI */
#define CAI_N6M 6     /* Supported Traffic Channel Active Set size */

#ifdef FEATURE_IS95B
#define CAI_N7M 10    /* Supported Traffic Channel Candidate Set size */
#define CAI_N8M 40    /* Minimum supported neighbor set size */
#else
#define CAI_N7M 5     /* Supported Traffic Channel Candidate Set size */
#define CAI_N8M 20    /* Minimum supported neighbor set size */
#endif /* FEATURE_IS95B */

#define CAI_N9M 7     /* Minimum supported zone-list size */
#define CAI_N10M 4    /* Minimum supported sid-nid list size */
#define CAI_N11M 1
  /* Number of received consecutive good Forward Traffic Channel frames
     before a mobile station reenables its transmitter during a CDMA to
     CDMA Hard Handoff */
#if defined (FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
#define CAI_N13M           6 /* N13m */
  /* Maximum number of pilots that can be reported in an AC Msg */

#define CAI_MAX_ADD_PILOTS ( CAI_N13M - 1 )
  /* Maximum number of additional pilots that can be reported in AC Msg */
  /* in addition to the active pilot */
#endif /* FEATURE_IS95B || FEATURE_SPECIAL_MDR */

/* Maximum number of pilots in the traffic channel active set size,
   i.e. N6M */
#define CAI_EXT_CHNASN_AM0_MAX     (CAI_N6M)

/* Minimum supported Neighbor set size, i.e. N8M */
#define CAI_EXT_CHNASN_AM1_MAX     (CAI_N8M)

#ifdef FEATURE_IS2000
/* Maximum number of pilots in the traffic channel active set size,
   i.e. N6M */
#define CAI_EXT_CHNASN_AM4_MAX     (CAI_N6M)
#endif

/* For sign extending 6-bit signed handoff soft slope intercept fields */
#define CAI_HO_INTERCEPT_SIGN_BIT (0x20)
#define CAI_HO_INTERCEPT_SIGN_EXT (0xC0)

typedef PACKED struct
{
  word t1m;   /* Maximum time the mobile waits for an acknowledgement */
  word t3m;   /* Period in which 2 Forward Traffic Channel messages
                 with the same sequence number are considered duplicates */
  word t4m;   /* Period in which 2 Paging Channel messages
                 with the same sequence number are considered duplicates */
  word t5m;   /* Limit of Forward Traffic Channel fade timer */
  word t20m;  /* Maximum time to remain in the Pilot Channel Acquisition
                 substate */
  word t21m;  /* Maximum time to receive a valid Sync Channel message */
  word t30m;  /* Maximum time to receive a valid Paging Channel message */
  dword t31m; /* Maximum time Paging Channel message considred valid */
  word t40m;  /* Maximum time to receive a valid Paging Channel message
                 before aborting an access attempt */
  word t41m;  /* Maximum time to obtain updated overhead messages arriving
                 on the Paging Channel */
  word t42m;  /* Maximum time to receive a delayed layer 3 response
                 following the receipt of an ack for an access probe */
  word t50m;  /* Maximum time to obtain N5m consecutive good frames while
                 in the Traffic Channel Initialization state */
  word t51m;  /* Maximu time to receive a Base Station ack when in the
                 Traffic Channel Initialization state */
  word t52m;  /* Maximum time to receive a message in the Waiting for
                 Order substate which transits the mobile to a new state */
  word t53m;  /* Maximum time to receive a message in the Waiting for
                 Answer substate which transits the mobile to a new state */
  word t55m;  /* Maximum time to receive a message in the Release substate
                 which transits the mobile to a new state */
  dword t57m; /* Limit of powerup registration timer */
  word t69m;  /* Full TMSI timer fixed portion */
#ifdef FEATURE_IS95B
  word t72m;  /* Maximum time to receive a valid Paging Channel message before
                 aborting an access attempt, when there exists at least one
                 access handoff candidate pilot for the access attempt */
#endif

  word n1m;   /* Maximum number of time a mobile transmits a message
                 requiring an ack on the Reverse Traffic Channel */
  word n2m;   /* Number of received consecutive bad Forward Traffic Channel
                 frames before a mobile disables its transmitter */
  word n3m;   /* Number of received consecutive good Forward Traffic Channel
                 frames before a mobile re-enables its transmitter */
  word n5m;   /* Number of received consecutive good Forward Traffic Channel
                 frames before a mobile enables its transmitter in TCI */
  word n6m;   /* Minimum supported Active Set size */
  word n7m;   /* Minimum supported Candidate Set size */
  word n8m;   /* Minimum supported Neighbor Set size */
  word n9m;   /* Minimum supported zone list size */
  word n10m;  /* Minimum supported SID/NID list size */
  word n11m;  /* Number of received consecutive good Forward Traffic Channel
                 frames before a mobile station reenables its transmitter
                 during a CDMA to CDMA Hard Handoff */
#if defined (FEATURE_IS95B) || defined (FEATURE_SPECIAL_MDR)
  word n13m;  /* number of pilots reported in an Access Channel Message */
#endif /* FEATURE_IS95B || FEATURE_SPECIAL_MDR */
} cai_tmo_type;

extern cai_tmo_type cai_tmo;  /* time-out structure */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                      REGISTRATION CONSTANTS                             */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define CAI_HOME_NID 65535
  /* Special value of NID used in a SID, NID pair to indicate that the mobile
     station considers all NIDs with a SID to be home (see IS-95A Section
     6.6.5.2 Systems and Networks) */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*              DATA BURST MESSAGE BURST TYPE ASSIGNMENTS                  */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See TSB58 Table 4.1-1 Standard Data Burst Msg Burst Type Assignments    */

#define CAI_UNKNOWN_BURST_TYPE  0x00
  /* Unknown burst type */
#define CAI_ASYNC_DATA_SERVICES 0x01
  /* Asynchronous Data Services */
#define CAI_GROUP_3_FACSIMILE   0x02
  /* Group-3 Facsimile */
#define CAI_SHORT_MSG_SERVICES  0x03
  /* Short Message Services */
#define CAI_OTA_ACTIVATION      0x04
  /* Over-the-Air Activation */
#define CAI_POSITION_DET        0x05
  /* Position Determination Services */
#define CAI_EXTENDED_BURST_TYPE_INTL 0x3E
  /* Extended Burst Type - International */
#define CAI_EXTENDED_BURST_TYPE 0x3F
  /* Extended Burst Type */

/* FEATURE_IS95B_EXT_DISP */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*              EXTENDED DISPLAY RECORD TYPES                              */
/*               See IS-95B Section 7.7.5.16                               */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Extended display information record Display Types                       */
/* Normal display type */
#define CAI_EXT_DISP_TYPE_NORMAL 0

/* Extended display information record control tags                        */
/* See table 7.7.5.16-2 */
#define CAI_EXT_DISP_BLANK              0x80
#define CAI_EXT_DISP_SKIP               0x81
#define CAI_EXT_DISP_CONTINUATION       0x82
#define CAI_EXT_DISP_CALLED_ADDR        0x83
#define CAI_EXT_DISP_CAUSE              0x84
#define CAI_EXT_DISP_PROGRESS_IND       0x85
#define CAI_EXT_DISP_NOTIF_IND          0x86
#define CAI_EXT_DISP_PROMPT             0x87
#define CAI_EXT_DISP_ACCUM_DIGITS       0x88
#define CAI_EXT_DISP_STATUS             0x89
#define CAI_EXT_DISP_INBAND             0x8A
#define CAI_EXT_DISP_CALLING_ADDR       0x8B
#define CAI_EXT_DISP_REASON             0x8C
#define CAI_EXT_DISP_CALLING_NAME       0x8D
#define CAI_EXT_DISP_CALLED_NAME        0x8E
#define CAI_EXT_DISP_ORIGINAL_NAME      0x8F
#define CAI_EXT_DISP_REDIRECTING_NAME   0x90
#define CAI_EXT_DISP_CONNECTED_NAME     0x91
#define CAI_EXT_DISP_ORIG_RESTRICT      0x92
#define CAI_EXT_DISP_DATE_AND_TIME      0x93
#define CAI_EXT_DISP_CALL_APPEARNC      0x94
#define CAI_EXT_DISP_FEATURE_ADDR       0x95
#define CAI_EXT_DISP_REDIRECTION_NAM    0x96
#define CAI_EXT_DISP_REDIRECTION_NUM    0x97
#define CAI_EXT_DISP_REDIRECTING_NUM    0x98
#define CAI_EXT_DISP_ORIG_CALLED_NUM    0x99
#define CAI_EXT_DISP_CONNECTED_NUM      0x9A
#define CAI_EXT_DISP_TEXT               0x9E

/* end of FEATURE_IS95B_EXT_DISP */


/* -------------------------------------------------------------------------
** The following data declarations are representations of the CDMA messages
** found in IS-95A (see sections 6.7 and 7.7).  Each bit in the message is
** represented by a byte in the data structure.
** ------------------------------------------------------------------------ */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                       GENERIC MESSAGES                                  */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Type to isolate msg_type */
typedef PACKED struct
{
  byte msg_type[8];    /* Message type */
} cai_gen_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Generic Paging Channel Message Header */
typedef PACKED struct
{
  byte msg_type[8];   /* Message type */
  byte ack_seq[3];    /* Acknowledgement sequence Number */
  byte msg_seq[3];    /* Message sequence number */
  byte ack_req[1];    /* Acknowledgement required indicator */
  byte valid_ack[1];  /* Valid acknowledgement indicator */
  byte addr_type[3];  /* Address type */
  byte addr_len[4];   /* Address field length */
} cai_pc_hdr_type;

/* Paging Channel Address Fields */
typedef PACKED struct
{
  byte min1[24];     /* First part of the mobile identification number */
  byte min2[10];     /* Second part of the mobile identification number */
  byte reserved[6];  /* Reserved bits */
} cai_addr0_type;

typedef PACKED struct
{
  byte esn[8];      /* Mobile station's electronic serial number */
} cai_addr1_type;

typedef PACKED struct
{
  byte imsi_class[1];  /* Identifies IMSI address class */
} cai_addr2_type;

typedef PACKED struct
{
  byte tmsi_zone[64];   /* TMSI zone         */
  byte tmsi_code[32];   /* TMSI code address */
} cai_tmsi_type;

#define CAI_BC_ADDR_MIN 2
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif

typedef PACKED struct
{
  byte bc_addr[8];     /* Broadcast address */
} cai_addr5_type;

#ifdef FEATURE_TMSI
#define CAI_MIN_MS_ADDR_WIDTH_BITS 16
  /* Minimum address size is a 2-octet TMSI address */
#else
#define CAI_MIN_MS_ADDR_WIDTH_BITS 32
  /* Minimum address size is a 4-octet ESN address */
#endif /* FEATURE_TMSI */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Generic Access Channel Message Header */

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte prot_disc[2];  /* protocol discriminator */
  byte msg_type[6];   /* message type */
} cai_ac_msg_type_type;
#endif

typedef PACKED struct
{
  byte msg_type[8];   /* Message type */
  byte ack_seq[3];    /* Acknowledgement sequence number */
  byte msg_seq[3];    /* Message sequence number */
  byte ack_req[1];    /* Acknowledgement required indicator */
  byte valid_ack[1];  /* Valid acknowledgement indicator */
  byte ack_type[3];   /* Acknowledgement address type */
  byte msid_type[3];  /* Mobile station identifier field type */
  byte msid_len[4];   /* Mobile station identifier field length */
} cai_ac_hdr_type;

/* Access Channel Mobile Station Identifier Fields (MSID_TYPE = '000') */
typedef PACKED struct
{
  byte min1[24];     /* First part of the mobile identification number */
  byte min2[10];     /* Second part of the mobile identification number */
  byte esn[32];      /* Mobile station's electronic serial number */
  byte reserved[6];  /* Reserved bits */
} cai_msid0_type;

/* Access Channel Mobile Station Identifier Fields (MSID_TYPE = '001')  */
/* (NOTE: For now the mobile station's ESN field is limited to 32 bits) */
typedef PACKED struct
{
  byte esn[32];       /* Mobile station's electronic serial number */
} cai_msid1_type;

/* Access Channel Mobile Station Identifier Fields (MSID_TYPE = '010')      */
typedef PACKED struct
{
  byte imsi_class[1];         /* IMSI Class */
} cai_msid2_type;

/* Access Channel Mobile Station Identifier Fields (MSID_TYPE = '011')      */
typedef PACKED struct
{
  byte esn[32];               /* Mobile station's electronic serial number */
  byte imsi_class[1];         /* IMSI Class */
} cai_msid3_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* IMSI Address types, used in both Paging and Access channel headers      */

typedef PACKED struct
{
  byte imsi_class_0_type[2];  /* IMSI class 0 type */
} cai_imsi_class0_type;

typedef PACKED struct
{
  byte imsi_class_1_type[1];  /* IMSI class 1 type */
} cai_imsi_class1_type;

/* IMSI Class 0 Type specific subfields (Class Type = 0) */
typedef PACKED struct
{
  byte reserved[3];       /* Reserved bits */
  byte imsi_s[34];        /* Last ten digits of the IMSI */
} cai_imsi_class0_type0_type;

/* IMSI Class 0 Type specific subfields (Class Type = 1) */
typedef PACKED struct
{
  byte reserved[4];       /* Reserved bits */
  byte imsi_11_12[7];     /* The 11th and 12th digits of IMSI */
  byte imsi_s[34];        /* Last ten digits of the IMSI */
} cai_imsi_class0_type1_type;

/* IMSI Class 0 Type specific subfields (Class Type = 2) */
typedef PACKED struct
{
  byte reserved[1];       /* Reserved bits */
  byte mcc[10];           /* Mobile country code */
  byte imsi_s[34];        /* Last ten digits of the IMSI */
} cai_imsi_class0_type2_type;

/* IMSI Class 0 Type specific subfields (Class Type = 3) */
typedef PACKED struct
{
  byte reserved[2];       /* Reserved bits */
  byte mcc[10];           /* Mobile country code */
  byte imsi_11_12[7];     /* The 11th and 12th digits of IMSI */
  byte imsi_s[34];        /* Last ten digits of the IMSI */
} cai_imsi_class0_type3_type;

/* IMSI Class 1 Type specific subfields (Class Type = 0) */
typedef PACKED struct
{
  byte reserved[2];       /* Reserved bits */
  byte imsi_addr_num[3];  /* Number of IMSI address digits */
  byte imsi_11_12[7];     /* The 11th and 12th digits of IMSI */
  byte imsi_s[34];        /* Last ten digits of the IMSI */
} cai_imsi_class1_type0_type;

/* IMSI Class 1 Type specific subfields (Class Type = 1) */
typedef PACKED struct
{
  byte imsi_addr_num[3];  /* Number of IMSI address digits */
  byte mcc[10];           /* Mobile country code */
  byte imsi_11_12[7];     /* The 11th and 12th digits of IMSI */
  byte imsi_s[34];        /* Last ten digits of the IMSI */
} cai_imsi_class1_type1_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Traffic Channel Layer 2 Message Header */

typedef PACKED struct
{
  byte ack_seq[3];          /* Acknowledgement Sequence number */
  byte msg_seq[3];          /* Message Sequence number */
  byte ack_req[1];          /* Acknowledgement Required indicator */
  byte encryption[2];       /* Message encryption indicator */
} cai_tc_hdr_type;

/* Generic Forward and Reverse Traffic Channel message */
typedef PACKED struct
{
  byte msg_type[8];    /* Message type */
  cai_tc_hdr_type hdr; /* Layer 2 message header */
} cai_gen_tc_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*      PAGING AND FORWARD TRAFFIC CHANNEL INFORMATION RECORD TYPES        */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Note - On the Paging Channel, information records may be included in
   the Feature Notification Message.  On the Forward Traffic Channel,
   information records may be included in the Alert With Information
   and the Flash with Information messages.                                */

/* Information Record header */
typedef PACKED struct
{
  byte record_type[8]; /* Information record type */
  byte record_len[8];  /* Information record length */
} cai_rec_hdr_type;

/* Display record (see IS-95A Section 7.7.5.1 Display) */
typedef PACKED struct
{
  byte chari[8];       /* Character */
} cai_display_rec_var_type;

/* Called Party Number record (see IS-95A Section 7.7.5.2 Called Party
   Number) */

/* Fixed part of Called Party Number fixed record */
typedef PACKED struct
{
  byte number_type[3];  /* Type of number */
  byte number_plan[4];  /* Numbering plan */
} cai_called_fix_type;

/* Variable part of Called Party Number record */
typedef PACKED struct
{
  byte chari[8];        /* Character */
} cai_called_var_type;

/* Calling Party Number record (see IS-95A Section 7.7.5.3 Calling Party
   Number) */

/* Fixed part of Calling Party Number record */
typedef PACKED struct
{
  byte number_type[3];   /* Type of number */
  byte number_plan[4];   /* Numbering plan */
  byte pi[2];            /* Presentation indicator */
  byte si[2];            /* Screening indicator */
} cai_calling_fix_type;

/* Variable part of Calling Party Number record */
typedef PACKED struct
{
  byte chari[8];         /* Character */
} cai_calling_var_type;

/* Use Calling Party Number record types for Connnected Number (see
   IS-95A Section 7.7.5.4 Connected Number) */

/* Signal record (see IS-95A Section 7.7.5.5 Signal) */
typedef PACKED struct
{
  byte signal_type[2];    /* Signal type */
  byte alert_pitch[2];    /* Pitch of the alerting signal */
  byte signal[6];         /* Signal code */
  byte reserved[6];       /* Reserved bits */
} cai_signal_rec_type;

/* Message Waiting record (see IS-95A Section 7.7.5.6 Message Waiting) */
typedef PACKED struct
{
  byte msg_count[8];      /* Number of waiting messages */
} cai_msg_waiting_rec_type;

/* Service configuration record. Used on both forward and reverse
   traffic channels. (see IS-95-A TSB) */

typedef PACKED struct  /* Mux and rate set info */
{
  byte fwd_mux[16];      /* Forward link multiplex option */
  byte rev_mux[16];      /* Reverse link multiplex option */
  byte fwd_rates[8];     /* Forward traffic channel rates */
  byte rev_rates[8];     /* Reverse traffic channel rates */
  byte num_con[8];       /* Number of service option connection records */
} cai_srv_cfg_hdr_type;

typedef PACKED struct  /* Service option connection info */
{
  byte rec_len[8];   /* Record length */
  byte con_ref[8];   /* Connection reference */
  byte so[16];       /* Service option number */
  byte fwd_traf[4];  /* Forward traffic type */
  byte rev_traf[4];  /* Reverse traffic type */
#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  byte ui_encrypt_mode[3];
  byte sr_id[3];
  byte rlp_info_incl[1];
#endif
#endif
} cai_srv_cfg_con_type;

#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
typedef PACKED struct
{
  byte rlp_blob_len[4];
  byte rlp_blob[8];
} cai_srv_cfg_con_var_type;
#endif
#endif

#ifdef FEATURE_IS2000
#define CAI_MAX_MULT_OPT_FCH_DCCH     2
#define CAI_MAX_MULT_OPT_SCH          15
#define CAI_MAX_NUM_FOR_SCH           2
#define CAI_MAX_NUM_REV_SCH           2
#define CAI_MAX_RLP_BLOB_SIZE         7
#define CAI_MAX_NUM_LPM_ENTRIES       15
#define CAI_MAX_NUM_RPC_RECS          3
#define CAI_MAX_NUM_FOR_SCH_CFG       16
#define CAI_MAX_NUM_REV_SCH_CFG       16
#define CAI_MAX_NUM_SCH_CHAN          16
#define CAI_DED_CTL                   2
#define CAI_FUND_CONT_REV_PLT         5
#define CAI_DED_CTL_CONT_REV_PLT      6
#define CAI_FUND_DED_CTL_CONT_REV_PLT 7
#define CAI_MAX_RLP_INFO_LEN          7
#define CAI_MAX_NUM_PER_SCH_CFG       16
#define CAI_ESCAM_MAX_REV_CFG         32
#define CAI_ESCAM_MAX_REV_SCH         2
#ifdef FEATURE_IS2000_2_1_ADDENDUM
  #define CAI_ESCAM_MAX_FOR_CFG       2
#else
  #define CAI_ESCAM_MAX_FOR_CFG       32
#endif
#define CAI_ESCAM_MAX_RECS            16
#define CAI_ESCAM_MAX_SUP_SHO         CAI_N6M
#define CAI_ESCAM_MAX_FOR_SCH         2
#define CAI_ESCAM_MAX_SUP             2
#define CAI_OUTER_RPT_MAX_SUP         2
#endif

#ifdef FEATURE_IS2000
typedef enum
{
  CAI_FPC_PRI_CHAN_FCH = 0,
  CAI_FPC_PRI_CHAN_DCCH = 1
} cai_fpc_pri_chan_type;
#endif

#ifdef FEATURE_IS2000
typedef PACKED struct
{
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  byte sch_rec_len[4];
#endif
  byte sch_rc[5];
  byte coding[1];
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte frame_40_used[1];
  byte frame_40_max_rate[4];
  byte frame_80_used[1];
  byte frame_80_max_rate[4];
#endif
} cai_sch_chn_cfg_type;

typedef PACKED struct
{
  byte for_sch_id[2];
  byte for_sch_mux[16];
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte for_multi_frame_offset[2];
#endif
} cai_for_sch_type;

typedef PACKED struct
{
#ifdef FEATURE_IS2000_2_1_ADDENDUM
  byte for_sch_id[1];
#else
  byte for_sch_id[2];
#endif
  byte for_sch_mux[16];
} cai_rtc_for_sch_type;

typedef PACKED struct
{
  byte rev_sch_id[2];
  byte rev_sch_mux[16];
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte rev_multi_frame_offset[2];
#endif
} cai_rev_sch_type;

typedef PACKED struct
{
#ifdef FEATURE_IS2000_2_1_ADDENDUM
  byte rev_sch_id[1];
#else
  byte rev_sch_id[2];
#endif
  byte rev_sch_mux[16];
} cai_rtc_rev_sch_type;

typedef PACKED struct
{
  byte fch_cc_incl[1];
  byte fch_frame_size[1];
  byte for_fch_rc[5];
  byte rev_fch_rc[5];
  byte dcch_cc_incl[1];
  byte dcch_frame_size[2];
  byte for_dcch_rc[5];
  byte rev_dcch_rc[5];
  byte for_sch_cc_incl[1];
  byte num_for_sch[2];
  byte rev_sch_cc_incl[1];
  byte num_rev_sch[2];
  byte rlp_info_len[3];
  byte rlp_blob[8];
} cai_srv_cfg_is2000_type;
#endif

#ifdef FEATURE_IS95B
/* Fixed part Called Party Subaddress info record IS-95B 7.7.5.8  */
/* Fixed part Calling Party Subaddress info record IS-95B 7.7.5.9 */
/* Fixed part Connected Subaddress info record IS-95B 7.7.5.10    */
/* Fixed part Redirecting Subaddress info record IS-95B 7.7.5.12  */
typedef PACKED struct
{
  byte extension_bit[1];  /* The extension bit */
  byte subadd_type[3];    /* Type of subaddress */
  byte odd_even_ind[1];   /* The indicator of even/odd bits */
  byte reserved[3];       /* Reserved bits */
} cai_subaddr_fix_type;

/* Variable part of Subaddress record */
typedef PACKED struct
{
  byte chari[8];        /* Character */
} cai_subaddr_var_type;

/* Fixed part Redirecting Number info record IS-95B 7.7.5.11 */
typedef PACKED struct
{
  byte extension_bit1[1]; /* The extension bit */
  byte number_type[3];    /* Type of number */
  byte number_plan[4];    /* Numbering plan */
  byte extension_bit2[1]; /* The extension bit */
  byte pi[2];             /* Presentation indicator */
  byte reserved1[3];      /* Reserved bits */
  byte si[2];             /* Screening indicator */
  byte extension_bit3[1]; /* The extension bit */
  byte reserved2[3];      /* Reserved bits */
  byte redir_reason[4];   /* The reason for redirection */
} cai_redir_fix_type;

typedef PACKED struct
{
  byte chari[8];          /* Character */
} cai_redir_var_type;

#endif /* FEATURE_IS95B */

#if defined( FEATURE_IS95B ) || defined( FEATURE_IS95B_WLL_ONLY )
typedef PACKED struct  /* Meter pulses info record IS-95B 7.7.5.13 */
{
  byte pulse_freq[11];    /* Pulse frequency */
  byte on_time[8];        /* Pulse on time */
  byte off_time[8];       /* Pulse off time */
  byte pulse_cnt[4];      /* Pulse count */
  byte reserved[1];       /* Reserved */
} cai_meter_pulses_rec_type;

/* Fixed Part of Parametric alerting record IS-95B 7.7.5.14 */
typedef PACKED struct
{
  byte cadence_cnt[8];    /* Cadence count */
  byte num_groups[4];     /* Num groups */
} cai_para_alert_fix_type;

typedef PACKED struct  /* Variable Part of Parametric alerting record */
{
  byte amplitude[8];     /* Amplitude */
  byte freq_1[10];       /* Freq 1 */
  byte freq_2[10];       /* Freq 2 */
  byte on_time[8];       /* On time */
  byte off_time[8];      /* Off time */
  byte repeat[4];        /* Repeat */
  byte delay[8];         /* Delay */
} cai_para_alert_var_type;

typedef PACKED struct  /* Line Control IS-95B 7.7.5.15 */
{
  byte pol_inc[1];   /* Polarity included */
  byte tgl_mod[1];   /* Toggle mode */
  byte rev_pol[1];   /* Reverse polarity */
  byte pow_dtm[8];   /* Power denial time */
} cai_line_ctrl_rec_type;
#endif /* FEATURE_IS95B || FEATURE_IS95B_WLL_ONLY */

#ifdef FEATURE_IS95B
typedef PACKED struct  /* Fixed Part of Extended Display record IS-95B 7.7.5.16 */
{
  byte ext_display_ind[1];  /* The indicator if Extended Display record */
  byte display_type[7];
} cai_ext_disp_fix_type;

typedef PACKED struct  /* Variable Part of Extended Display record */
{
  byte display_tag[8];  /* Indicator of the display information */
  byte display_len[8];  /* The display length */
} cai_ext_disp_var1_type;

typedef PACKED struct
{
  byte chari[8];        /* Character */
} cai_ext_disp_var2_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte uzid[16];
} cai_user_zone_update_type;

typedef PACKED struct
{
  byte reject_uzid[16];
  byte reject_action_indi[3];
  byte uzid_assign_incl[1];
  byte assign_uzid[16];
} cai_user_zone_reject_type;
#endif

#endif /* FEATURE_IS95B */

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*            REVERSE TRAFFIC CHANNEL INFORMATION RECORD TYPES             */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Note - these records are used in the Flash With Information and the
   Status messages (see IS-95A Sections 6.7.2.3.2.3 Flash With Information
   Message and 6.7.2.3.2.8 Status Message)                                 */

/* Note that the Reverse Traffic Channel information records use the
   same record header type as the Forward Traffic Channel information
   records */

/* Feature Indicator record (see IS-95A Section 6.7.4.1 Feature Indicator) */
typedef PACKED struct
{
  byte feature[4];   /* Feature indentifier */
  byte reserved[4];  /* Reserved bits */
} cai_feat_type;

/* Keypad facility record (see IS-95A Section 6.7.4.2 Keypad Facility)     */
typedef PACKED struct
{
  byte chari[8];     /* Character */
} cai_keypad_type;

/* Use Called Party Number and Calling Party Number record types from
   Forward Traffic Channel Information records (see IS-95A Sections
   6.7.4.3 Called Party Number and 6.7.4.4 Calling Party Number) */

/* Call Mode record (see IS-95A Section 6.7.4.6 Call Mode) */
typedef PACKED struct
{
  byte orig_mode[1];     /* Origination mode indicator */
  byte pri_service[16];  /* Primary service option */
  byte sec_service[16];  /* Secondray service option */
  byte reserved[7];      /* Reserved bits */
} cai_call_mode_type;

/* Terminal Information record (see IS-95A Section 6.7.4.7 Terminal
   Information) */

/* Fixed part of Terminal Information record */
typedef PACKED struct
{
  byte mob_p_rev[8];         /* Protocol revision of the mobile station */
  byte mob_mfg_code[8];      /* Manufacturer code */
  byte mob_model[8];         /* Model number */
  byte mob_firm_rev[16];     /* Firmware revision number */
  byte scm[8];               /* Station class mark */
  byte local_ctrl[1];        /* Local control indicator */
  byte slot_cycle_index[3];  /* Slot cycle index */
} cai_term_info_fix_type;

/* Variable part of Terminal Information record */
typedef PACKED struct
{
  byte service_option[16];   /* Supported service option */
} cai_term_info_var_type;

/* Roaming info record (see IS-95A Section 6.7.4.8 Roaming Information) */

/* Fixed part of Roaming Information record */
typedef PACKED struct
{
  byte accolc[4];            /* Overload class */
  byte mob_term_home[1];     /* Home registration enable indicator */
  byte mob_term_for_sid[1];  /* Foreign SID roaming registration enable
                                indicator */
  byte mob_term_for_nid[1];  /* Foreign NID roaming registration enable
                                indicator */
} cai_roam_info_fix_type;

/* Variable part of Roaming Information record */
typedef PACKED struct
{
  byte sid[15];              /* System identification */
  byte nid[16];              /* Network identification */
} cai_roam_info_var_type;

/* Security Status record (see IS-95A Section 6.7.4.9 Security Status) */
typedef PACKED struct
{
  byte auth_mode[2];       /* Authentication mode */
  byte encrypt_mode[2];    /* Message encryption mode */
  byte private_lcm[1];     /* Private long code mask indicator */
  byte reserved[3];        /* Reserved bits */
} cai_sec_stat_type;

/* Use Forward Traffic Channel Connected Number information record type for
   Connected Number type */

/* IMSI info record (see IS-95A Section 6.7.4.11 IMSI) */
typedef PACKED struct
{
  byte imsi_class[1];      /* Set to '0' if assigned a class 0 IMSI */
  byte imsi_addr_num[3];   /* Number of IMSI address digits */
  byte mcc[10];            /* Mobile country code */
  byte imsi_11_12[7];      /* The 11th and 12th digits of IMSI */
  byte imsi_s[34];         /* Last ten digits of the IMSI */
  byte reserved[1];        /* Reserved bits */
} cai_imsi_info_type;

/* ESN info record (see IS-95A Section 6.7.4.12 ESN) */
typedef PACKED struct
{
  byte esn[32];            /* Mobile station electronic serial number */
} cai_esn_info_type;

/* Band class info record (see IS-95-A TSB) */
typedef PACKED struct
{
  byte band0[1];       /* Band class 0 - 800 MHz Cellular */
  byte band1[1];       /* Band class 1 - 1.8-2.0 GHz PCS */
  byte reserved1[2];    /* Reserved bits */
  byte band4[1];       /* Band class 4 Korean PCS */
#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  byte band5[1];
  byte band6[1];
  byte reserved2[1];
#else
  byte reserved2[3];
#endif
#else
  byte reserved2[3];    /* Reserved bits */
#endif
} cai_band_info_type;

/* Power class info record (see IS-95-A TSB) */
typedef PACKED struct
{
  byte max_eirp[8];  /* Max EIRP (Effective Isotropic Radiated Power */
} cai_pwr_info_type;

/* Operating mode info rec (see IS-95-A TSB) */
typedef PACKED struct
{
  byte op_mode0[1];  /* ANSI J-STD-008 */
  byte op_mode1[1];  /* TIA/EIA/IS-95-A CDMA mode */
  byte op_mode2[1];  /* TIA/EIA/IS-95-A analog mode */
  byte op_mode3[1];  /* TIA/EIA/IS-91 wide analog mode */
  byte op_mode4[1];  /* TIA/EIA/IS-91 narrow analog mode */
  byte reserved[3];  /* Reserved bits */
} cai_op_mode_info_type;

/* Service option info record (see IS-95-A TSB) */
typedef PACKED struct
{
  byte reserved[6];  /* Reserved bits */
  byte fwd[1];       /* Forward support indicator */
  byte rev[1];       /* Reverse support indicator */
  byte so[16];       /* Service option number */
} cai_so_info_var_type;

/* Multiplex option info record (see IS-95-A TSB) */
typedef PACKED struct
{
  byte mux[16];       /* Multiplex option number */
  byte fwd_rates[8];  /* Vector of supported forward link rates */
  byte rev_rates[8];  /* Vector of supported reverse link rates */
} cai_mux_info_type;

#ifdef FEATURE_IS95B
/* Called Party Subaddress, Calling Party Subaddress, and Connected
   Subaddress (see IS-95B 6.7.4.19-21 ) use subaddress type defined
   for Forward Link */

/* Power Control Information (see IS-95B Section 6.7.4.22) */
typedef PACKED struct
{
  byte min_pwr_cntl_step[3];   /* Minimum power control step */
  byte reserved[5];            /* Reserved bits */
} cai_pwr_ctrl_info_type;

/* IMSI_M info record (see IS-95B Section 6.7.4.23) */
/* IMSI_T info record (see IS-95B Section 6.7.4.24) */
/* Use IMSI info record type defined before */

/* Capability info record (see IS-95B Section 6.7.4.25) */
typedef PACKED struct
{
  byte acc_entry_ho[1];    /* Access entry handoff supported */
  byte acc_probe_ho[1];    /* Access probe handoff supported */
  byte analog_search[1];   /* Analog search supported */
  byte hop_beacon[1];      /* Hopping Beacon */
  byte mahho[1];           /* Mobile Assisted HHO */
  byte puf[1];             /* PUF */
  byte analog_553[1];      /* TIA/EIA553 supported */
  byte reserved[1];        /* Reserved bits */
} cai_capa_info_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte qpch[1];
  byte slotted_timer[1];
  byte mac_cf_supported[1];
  byte chs_supported[1];
  byte gating_rate_set[2];
  byte suspended_supported[1];
  byte ext_cap_included[1];
  byte mabo[1];
  byte sdb[1];
  byte rlp_cap_blob_len[3];
  byte rlp_cap_blob[8];
} cai_capa_info_is2000_type;
#endif

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte otd_supported[1];
  byte fch_supported[1];
  byte dcch_supported[1];
  byte for_sch_supported[1];
  byte rev_sch_supported[1];
  byte nonoctet_aligned_data[1];
  byte octet_aligned_data[1];
} cai_chan_cfg_capa_info_type;

typedef PACKED struct
{
  byte num_mo_for_fch[4];
  byte mo_for_fch[16];
  byte for_rates_fch[8];
  byte num_mo_rev_fch[4];
  byte mo_rev_fch[16];
  byte rev_rates_fch[8];
  byte num_mo_for_dcch[4];
  byte mo_for_dcch[16];
  byte num_mo_rev_dcch[4];
  byte mo_rev_dcch[16];
  byte num_mo_for_sch[4];
  byte for_sch_id[1];
  byte mo_for_sch[16];
  byte num_mo_rev_sch[4];
  byte rev_sch_id[1];
  byte mo_rev_sch[16];
} cai_ext_mult_opt_info_type;

typedef PACKED struct
{
  byte uzid[16];
} cai_user_zone_update_req_type;

#ifndef FEATURE_IS2000_2_1_ADDENDUM
/* Geo-location information */
typedef PACKED struct
{
  byte geo_loc[3]; /* geo location */
  byte reserved[5];
} cai_geo_loc_info_type;

/* Geo location codes */
#define CAI_NO_MS_ASSIST_GEO_CAP 0x0
/* no mobile station assisted geo location capabilities */

#define CAI_IS801_FWD_TRIG_CAP 0x1
/* IS-801 capable, advanced forward link triangulation */

#define CAI_IS801_FWD_TRIG_GPS_CAP 0x2
/* IS-801 capable, advanced forward link triangulation and GPS */

#define CAI_GPS_CAP 0x3
/* GPS only */
#endif

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SYNC CHANNEL MESSAGE (see IS-95A Section 7.7.1.3)                       */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

typedef PACKED struct
{
  byte msg_type[8];    /* Message type */
  byte p_rev[8];       /* Protocol revision level */
  byte min_p_rev[8];   /* Minimum protocol revision level */
  byte sid[15];        /* System identification */
  byte nid[16];        /* Network identification */
  byte pilot_pn[9];    /* Pilot PN sequence offset index */
  byte lc_state[42];   /* Long code state */
  byte sys_time[36];   /* System time */
  byte lp_sec[8];      /* Number of leap seconds that have occurred since
                          the start of System Time */
  byte ltm_off[6];     /* Offset of local time from System Time */
  byte daylt[1];       /* Daylight savings time indicator */
  byte prat[2];        /* Paging Channel data rate */
  byte cdma_freq[11];  /* Frequency assignment */
#ifdef FEATURE_IS2000
  byte ext_cdma_freq[11];
#endif
} cai_sync_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                    PAGING CHANNEL MESSAGES                              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SYSTEM PARAMETERS MESSAGE (see IS-95A Section 7.2.3.2.11)               */

typedef PACKED struct
{
  byte msg_type[8];             /* Message type */
  byte pilot_pn[9];             /* Pilot PN sequence offset index */
  byte config_msg_seq[6];       /* Configuration message sequence number */
  byte sid[15];                 /* System identification */
  byte nid[16];                 /* Network identification */
  byte reg_zone[12];            /* Registration zone */
  byte total_zones[3];          /* Number of registration zones to be
                                   retained */
  byte zone_timer[3];           /* Zone timer length */
  byte mult_sids[1];            /* Multiple SID storage indicator */
  byte mult_nids[1];            /* Multiple NID storage indicator */
  byte base_id[16];             /* Base station identification */
  byte base_class[4];           /* Base station class */
  byte page_chan[3];            /* Number of Paging Channels */
  byte max_slot_cycle_index[3]; /* Maximum slot cycle index */
  byte home_reg[1];             /* Home registration indicator */
  byte for_sid_reg[1];          /* SID roamer registration indicator */
  byte for_nid_reg[1];          /* NID roamer registration indicator */
  byte power_up_reg[1];         /* Power-up registration indicator */
  byte power_down_reg[1];       /* Power-down registration indicator */
  byte parameter_reg[1];        /* Parameter-change registration indicator */
  byte reg_prd[7];              /* Registration period */
  byte base_lat[22];            /* Base station latitude */
  byte base_long[23];           /* Base station longitude */
  byte reg_dist[11];            /* Registration distance */
  byte srch_win_a[4];           /* Search window size for the Active Set and
                                   Candidate Set */
  byte srch_win_n[4];           /* Search window size for the Neighbor Set */
  byte srch_win_r[4];           /* Search window size for the Remaining Set */
  byte nghbr_max_age[4];        /* Neighbor Set maximum AGE */
  byte pwr_rep_thresh[5];       /* Power control reporting threshold */
  byte pwr_rep_frames[4];       /* Power control reporting frame count */
  byte pwr_thresh_enable[1];    /* Threshold report mode indicator */
  byte pwr_period_enable[1];    /* Threshold report mode indicator */
  byte pwr_rep_delay[5];        /* Power report delay */
  byte rescan[1];               /* Rescan indicator */
  byte t_add[6];                /* Pilot detection threshold */
  byte t_drop[6];               /* Pilot drop threshold */
  byte t_comp[4];               /* Active Set versus Candidate Set
                                   comparison threshold */
  byte t_tdrop[4];              /* Drop timer value */
  byte ext_sys_parameter[1];    /* Extended System Parameters Message
                                   indicator */
  byte ext_nghbr_list[1];       /* Extended Neighbor List Message indicator */
/* FEATURE_IS95B */
  byte gen_nghbr_list[1];       /* General Neighbor List Message indicator */
/* end of FEATURE_IS95B */
  byte global_redirect[1];      /* Global Service Redirection Message
                                   indicator */
} cai_sysparm_msg_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte pri_nghbr_list[1];       /* Private Neighbor List Message indicator */
  byte user_zone_id[1];         /* User Zone Identification Message
                                   indicator */
  byte ext_global_redirect[1];  /* Extended Global Service Redirection
                                   Message indicator */
  byte ext_chan_list[1];        /* Extended CDMA Channel List Message
                                   indicator */
} cai_sysparm_msg_is2000_type;

typedef PACKED struct
{
  byte is2000_incl[1];          /* is2000 fields included indicator */
} cai_sysparm_msg_inferred_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ACCESS PARAMETERS MESSAGE (see IS-95A Section 7.7.2.3.2.2)              */

typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  byte pilot_pn[9];      /* Pilot PN sequence offset index */
  byte acc_msg_seq[6];   /* Access parameters message sequence number */
  byte acc_chan[5];      /* Number of Access Channels */
  byte nom_pwr[4];       /* Nominal transmit power offset */
  byte init_pwr[5];      /* Initial power offset for access */
  byte pwr_step[3];      /* Power increment */
  byte num_step[4];      /* Number of access probes */
  byte max_cap_sz[3];    /* Maximum Access Channel message capsule size */
  byte pam_sz[4];        /* Access Channel preamble length */
  byte psist_0_9[6];     /* Persistence value for access overload classes
                            0 through 9 */
  byte psist_10[3];      /* Persistence value for Access Overload Class 10 */
  byte psist_11[3];      /* Persistence value for Access Overload Class 11 */
  byte psist_12[3];      /* Persistence value for Access Overload Class 12 */
  byte psist_13[3];      /* Persistence value for Access Overload Class 13 */
  byte psist_14[3];      /* Persistence value for Access Overload Class 14 */
  byte psist_15[3];      /* Persistence value for Access Overload Class 15 */
  byte msg_psist[3];     /* Persistence modifier for Access Channel attempts
                            for message transmissions */
  byte reg_psist[3];     /* Persistence modifier for Access Channel attempts
                            for registrations */
  byte probe_pn_ran[4];  /* Time randomization for Access Channel probes */
  byte acc_tmo[4];       /* Acknowledgement timeout */
  byte probe_bkoff[4];   /* Access Channel probe backoff range */
  byte bkoff[4];         /* Access Channel probe sequence backoff range */
  byte max_req_seq[4];   /* Maximum number of acces probe sequences for an
                            Access Channel request */
  byte max_rsp_seq[4];   /* Maximum number of access probe sequences for an
                            Access Channel response */
  byte auth[2];          /* Authentication mode */
  byte rand[32];         /* Random challenge value - only present if auth
                            field is set to '01' */
  byte nom_pwr_ext[1];   /* Extended Nominal transmit power */
  byte reserved[6];      /* Reserved bits */
} cai_accparm_msg_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* EXTENDED NEIGHBOR LIST MESSAGE (see J-STD-008 Section 3.7.2.3.2.14)     */

/* Fixed Part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte pilot_pn[9];        /* Pilot PN sequence offset index */
  byte config_msg_seq[6];  /* Configuration message sequence number */
  byte pilot_inc[4];       /* Pilot PN sequence offset index increment */
} cai_ext_nghbr_fix_type;

/* Variable part of message */
typedef PACKED struct
{
  byte nghbr_config[3];     /* Neighbor configuration */
  byte nghbr_pn[9];         /* Neighbor pilot PN sequence offset index */
  byte search_pri[2];       /* Pilot Channel search priority */
  byte freq_incl[1];        /* Frequency included indicator */
  byte nghbr_band[5];       /* Neighbor band class */
  byte nghbr_freq[11];      /* Neighbor frequency assignment */
} cai_ext_nghbr_var_type;

/* Maximum number of variable parts that will fit in message */
#define CAI_EXT_NGHBR_MAX  PAGECHN_MAX( cai_ext_nghbr )

typedef PACKED struct
{
  cai_ext_nghbr_fix_type fix;
  cai_ext_nghbr_var_type var[CAI_EXT_NGHBR_MAX] ;
} cai_ext_nghbr_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* NEIGHBOR LIST MESSAGE (see IS-95A Section 7.7.2.3.2.3)                  */

/* Fixed Part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte pilot_pn[9];        /* Pilot PN sequence offset index */
  byte config_msg_seq[6];  /* Configuration message sequence number */
  byte pilot_inc[4];       /* Pilot PN sequence offset index increment */
} cai_nghbr_fix_type;

/* Variable part of message */
typedef PACKED struct
{
  byte nghbr_config[3];     /* Neighbor configuration */
  byte nghbr_pn[9];         /* Neighbor pilot PN sequence offset index */
} cai_nghbr_var_type;

/* Maximum number of variable parts that will fit in message */
#define CAI_NGHBR_MAX  PAGECHN_MAX( cai_nghbr )

typedef PACKED struct
{
  cai_nghbr_fix_type fix;
  cai_nghbr_var_type var[CAI_NGHBR_MAX] ;
} cai_nghbr_type;


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* CDMA CHANNEL LIST MESSAGE (see IS-95A Section 7.7.2.3.2.4)              */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte pilot_pn[9];        /* Pilot PN sequence offset index */
  byte config_msg_seq[6];  /* Configuration message sequence number */
} cai_chnlist_fix_type;

/* Variable part of message */
typedef byte cai_chnlist_var_type[11]; /* CDMA Channel freq assignment */

/* Maximum number of variable message parts that will fit into message */
#define CAI_CHNLIST_MAX  PAGECHN_MAX( cai_chnlist )

typedef PACKED struct
{
  cai_chnlist_fix_type fix;
  cai_chnlist_var_type var[CAI_CHNLIST_MAX];
} cai_chnlist_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SLOTTED PAGE MESSAGE (see IS-95A Section 7.7.2.3.2.5)                   */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte config_msg_seq[6];  /* Configuration message sequence number */
  byte acc_msg_seq[6];     /* Access parameters message sequence number */
  byte more_pages[1];      /* More slotted pages to follow indicator */
} cai_slt_page_fix_type;

/* Variable part of message */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte ext_addr[1];         /* Extra address indicator */
  byte min1[24];            /* First part of mobile identification number */
  byte min2[10];            /* Second part of mobile identification number
                               (only present if ext_addr field is set to 1) */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_slt_page_var_type;

/* Minumum size of variable part of message */
#define CAI_SLT_PAGE_MIN \
   (sizeof( cai_slt_page_var_type ) -      \
      FSIZ( cai_slt_page_var_type, min2 ) - \
        FSIZ( cai_slt_page_var_type, service_option ))

/* Maximum number of variable parts of message  */
#define CAI_SLT_PAGE_MAX \
  ((CAI_PC_BODY_SIZE - sizeof( cai_slt_page_fix_type )) / CAI_SLT_PAGE_MIN)

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* PAGE MESSAGE (see IS-95A Section 7.7.2.3.2.6)                           */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte config_msg_seq[6];  /* Configuration message sequence number */
  byte acc_msg_seq[6];     /* Access parameters message sequence number */
} cai_page_fix_type;

/* Variable part of message */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message Sequence number */
  byte ext_addr[1];         /* Extended addressing indicator */
  byte min1[24];            /* First part of mobile identification number */
  byte min2[10];            /* Second part of mobile identification number
                               (only present if ext_addr field is set to 1) */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_page_var_type;

/* Minimum size of variable length page */
#define CAI_PAGE_MIN  \
   (sizeof( cai_page_var_type ) - \
      FSIZ( cai_page_var_type, min2 ) - \
        FSIZ( cai_page_var_type, service_option ))

/* Maximum number of pages in message */
#define CAI_PAGE_MAX  \
  ((CAI_PC_BODY_SIZE - sizeof( cai_page_fix_type )) / CAI_PAGE_MIN)

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* PAGING CHANNEL ORDER MESSAGE (IS-95A Sections 7.7.2.3.2.7 and 7.7.4)    */

/* NOTE: The following structures define the contents of the Paging
         Channel Order Message following the ADDRESS field. */

/* Orders without an order qualifier */
typedef PACKED struct
{
  byte order[6];           /* Order field */
  byte add_record_len[3];  /* Additional record length */
} cai_pc_gen_ord_type;

/* Orders with an order qualifier */
typedef PACKED struct
{
  byte order[6];           /* Order field */
  byte add_record_len[3];  /* Additional record length */
  byte ordq[8];            /* Order qualification code */
} cai_pc_ordq_type;

/* Base Station Challenge Confirmation Order */
typedef PACKED struct
{
  byte order[6];           /* Order field */
  byte add_record_len[3];  /* Additional record length */
  byte ordq[8];            /* Order qualification code */
  byte authbs[18];         /* Challenge response */
  byte reserved[6];        /* Reserved bits */
} cai_pc_bs_chal_type;

#ifdef FEATURE_IS95B
/* Registration Accepted Order */
typedef PACKED struct
{
  byte order[6];            /* Order field */
  byte add_record_len[3];   /* Additional record length */
  byte ordq[8];             /* Order qualification code */
  byte roam_indi[8];        /* Roaming Display Indication */
} cai_pc_reg_acc_type;

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
/* PC Retry Order */
typedef PACKED struct
{
  byte order[6];            /* Order field */
  byte add_record_len[3];   /* Additional record length */
  byte ordq[8];             /* Order qualification code */
  byte retry_type[3];
  byte retry_delay[8];
  byte reserved[5];
} cai_pc_retry_type;
#endif
#endif

/* Minimum size for an order record in a Paging Channel Order Message */
#define CAI_PC_ORDER_MIN  (sizeof( cai_pc_hdr_type ) -           \
                             FSIZ( cai_pc_hdr_type, msg_type ) + \
                               CAI_MIN_MS_ADDR_WIDTH_BITS +      \
                                 sizeof( cai_pc_gen_ord_type ))

/* Maximum number of orders in a Paging Channel Order Message */
#define CAI_PC_ORDER_MAX \
  ((CAI_PC_BODY_SIZE - sizeof( cai_gen_type )) / \
      (CAI_PC_ORDER_MIN - sizeof( cai_gen_type)))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* CHANNEL ASSIGNMENT MESSAGE (see IS-95A Section 7.7.2.3.2.8)             */

/* NOTE: The following structures define the contents of the
         Channel Assignment Message following the ADDRESS field. */

/* Base of Channel Assignment Message */
typedef PACKED struct
{
  byte assign_mode[3];     /* Assignment mode */
  byte add_record_len[3];  /* Additional record length */
} cai_chnasn_base_type;

/* Types for Channel Assigment Message with Assign mode = 000 */
typedef PACKED struct
{
  byte assign_mode[3];     /* Assignment mode */
  byte add_record_len[3];  /* Additional record length */
  byte freq_incl[1];       /* Frequency included indicator */
  byte code_chan[8];       /* Code channel */
  byte cdma_freq[11];      /* Frequency assignment (only present if
                              freq_incl field is set to 1) */
  byte frame_offset[4];    /* Frame offset */
  byte encrypt_mode[2];    /* Message encryption mode */
} cai_chnasn_am0_type;

/* Channel Assignment Message with Assign mode = 001 */
typedef PACKED struct
{
  byte assign_mode[3];     /* Assignment mode */
  byte add_record_len[3];  /* Additional record length */
  byte respond[1];         /* Respond on new Access Channel indicator */
  byte freq_incl[1];       /* Frequency included indicator */
  byte cdma_freq[11];      /* Frequency assignment */
} cai_chnasn_am1_fix_type;

typedef PACKED struct
{
  byte pilot_pn[9];  /* Pilot PN sequence offset index */
} cai_chnasn_am1_var_type;

/* Maximum number of variable message parts that will fit into message:
             56 bits (max add_record_len value is 7 * 8 bits)
         -    2 bits (respond and freq_incl fields --> NO cdma_freq field)
         -----------
             54 bits  (remaining for pilot_pn records)
 divided by   9 bits  (size of pilot_pn field)
         -----------
              6 (max possible pilot_pn records per msg) */

#define CAI_CHNASN_AM1_MAX 6

/* Channel Assignment Message with Assign mode = 010 */
typedef PACKED struct
{
  byte assign_mode[3];     /* Assignment mode */
  byte add_record_len[3];  /* Additional record length */
  byte respond[1];         /* Respond on analog control channel indicator */
  byte analog_sys[1];      /* System indicator */
  byte use_analog_sys[1];  /* Use analog system indicator */
  byte band_class[5];      /* Band Class */
} cai_chnasn_am2_type;

/* Channel Assignment Message with Assign mode = 011 */
typedef PACKED struct
{
  byte assign_mode[3];     /* Assignment mode */
  byte add_record_len[3];  /* Additional record length */
  byte sid[15];            /* System identification of the analog system */
  byte vmac[3];            /* Voice mobile station attenuation code */
  byte analog_chan[11];    /* Voice channel number */
  byte scc[2];             /* SAT color code */
  byte mem[1];             /* Message encryption mode indicator */
  byte an_chan_type[2];    /* Analog voice channel type */
  byte dscc_msb[1];        /* DSCC most significant bit */
  byte band_class[5];      /* Band class */
} cai_chnasn_am3_type;

/* Types for Channel Assigment Message with Assign mode = 100 */
typedef PACKED struct
{
  byte assign_mode[3];     /* Assignment mode */
  byte add_record_len[3];  /* Additional record length */
  byte freq_incl[1];       /* Frequency included indicator */
  byte reserved[3];        /* Reserved bits */
  byte bypass_alert_answer[1];  /* Bypass Indicator */
  byte default_config[3];  /* Default configuration */
  byte grant_mode[2];      /* Granted mode */
  byte code_chan[8];       /* Code channel */
  byte frame_offset[4];    /* Frame offset */
  byte encrypt_mode[2];    /* Message encryption mode */
  byte band[5];            /* Band class (only present if freq_incl
                              is set to 1) */
  byte cdma_freq[11];      /* Frequency assignment (only present if
                              freq_incl field is set to 1) */
} cai_chnasn_am4_type;

/* Channel Assignment Message with Assign mode = 101 */
typedef PACKED struct
{
  byte assign_mode[3];     /* Assignment mode */
  byte add_record_len[3];  /* Additional record length */
  byte respond[1];         /* Respond on new Access Channel indicator */
  byte freq_incl[1];       /* Frequency included indicator */
  byte band[5];            /* Band class (only present if freq_incl
                              is set to 1) */
  byte cdma_freq[11];      /* Frequency assignment */
} cai_chnasn_am5_fix_type;

typedef PACKED struct
{
  byte pilot_pn[9];  /* Pilot PN sequence offset index */
} cai_chnasn_am5_var_type;

/* Maximum number of variable length parts.  See assign mode '001' */
#define CAI_CHNASN_AM5_MAX 6

/* Minimum record size for a valid Channel Assignment Message */
#define CAI_CHN_ASN_REC_MIN  (sizeof( cai_pc_hdr_type ) -            \
                                sizeof( cai_gen_type ) +             \
                                  CAI_MIN_MS_ADDR_WIDTH_BITS +       \
                                     sizeof( cai_chnasn_am2_type ))

/* Maximum number of Channel Assignment orders that can fit in a
   Channel Assignment Message */
#define CAI_CHN_ASN_MAX  \
  ((CAI_PC_BODY_SIZE - sizeof( cai_gen_type )) / CAI_CHN_ASN_REC_MIN)

/* Minimum size for a valid Channel Assignment Message */
#define CAI_CHN_ASN_MIN  (sizeof( cai_pc_hdr_type ) +         \
                            CAI_MIN_MS_ADDR_WIDTH_BITS +      \
                              sizeof( cai_chnasn_am2_type ))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* TMSI ASSIGNMENT MESSAGE (see J-STD-008 Section 3.7.2.3.2.19)            */

typedef PACKED struct  /* TMSI Assignment Message - Paging Channel */
{
  byte reserved1[5];        /* Reserved             */
  byte tmsi_zone_len[4];    /* TMSI zone length     */
  byte tmsi_zone[64];       /* TMSI zone            */
  byte tmsi_code[32];       /* TMSI code address    */
  byte tmsi_exp_time[24];   /* TMSI expiration time */
  byte reserved2[2];        /* Reserved             */
} cai_pc_tmsi_asgn_type;

/* Minimum TMSI Assignment Message size includes */
/* the smallest TMSI zone that can be set.       */
#define CAI_PC_TMSI_MIN  \
   (sizeof( cai_pc_hdr_type ) + CAI_MIN_MS_ADDR_WIDTH_BITS + \
      sizeof( cai_pc_tmsi_asgn_type ) - 56)


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* DATA BURST MESSAGE (see IS-95A Section 7.7.2.3.2.9)                     */

/* NOTE: The following structures define the contents of the
         Data Burst Message following the ADDRESS field. */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_number[8];    /* Message number */
  byte burst_type[6];    /* Data burst type */
  byte num_msgs[8];      /* Number of messages in the data burst stream */
  byte num_fields[8];    /* Number of characters in this message */
} cai_burst_pc_fix_type;

/*  Variable part of message */
typedef PACKED struct
{
  byte chari[8];         /* Character */
} cai_burst_pc_var_type;

/* Maximum number of variable length message parts:
           2002 bits (maximum paging channel message body size)
         -   23 bits (msg_type + common ack fields + addr_type + addr_len)
         -   32 bits (esn --> addr_type == '001')
         -   30 bits (other Data Burst Message fields)
         -----------
           1917 bits (remaining for data burst characters)
 divided by   8 bits (size of data burst character)
         -----------
            239 (max characters per msg) */

#define CAI_PC_BURST_MAX  239

/* Minimum Data Burst Message size includes 5 for reserved bits */
#define CAI_PC_BURST_MIN  \
   (sizeof( cai_pc_hdr_type ) + CAI_MIN_MS_ADDR_WIDTH_BITS + \
      sizeof( cai_burst_pc_fix_type ) + 5)

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* AUTHENTICATION CHALLENGE MESSAGE (see IS-95A Section 7.7.2.3.2.10)      */

/* NOTE: The following structure defines the contents of the Authentication
         Challenge Message following the ADDRESS field. */

typedef PACKED struct
{
  byte randu[24];        /* Random challenge data */
  byte reserved[3];      /* Reserved bits */
} cai_pc_auth_ch_type;

/* Minimum size of Authentication Challenge Message */
#define CAI_PC_AUTH_MIN (sizeof( cai_pc_hdr_type ) +         \
                           CAI_MIN_MS_ADDR_WIDTH_BITS +      \
                             sizeof( cai_pc_auth_ch_type ))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SSD UPDATE MESAGE (see IS-95A Section 7.7.2.3.2.11)                     */

/* NOTE: The following structure defines the contents of the SSD Update
         Message following the ADDRESS field. */

typedef PACKED struct
{
  byte randssd[56];  /* Random data for the computation of SSD */
  byte reserved[3];  /* Reserved bits */
} cai_pc_ssd_type;

/* Minimum size of SSD Update Message */
#define CAI_PC_SSD_MIN (sizeof( cai_pc_hdr_type ) +         \
                          CAI_MIN_MS_ADDR_WIDTH_BITS +      \
                            sizeof( cai_pc_ssd_type ))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* FEATURE NOTIFICATION MESSAGE (see IS-95A Section 7.7.2.3.2.12)          */

/* NOTE: The following structure defines the contents of the Feature
         Notification Message following the ADDRESS field. */

typedef PACKED struct
{
  byte release[1];      /* Origination completion indicator */
} cai_pc_feature_type;

/* Minimum length of Feature Notification Message */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#else
#define CAI_FEATURE_MIN (sizeof( cai_pc_hdr_type ) +          \
                           CAI_MIN_MS_ADDR_WIDTH_BITS +       \
                             sizeof( cai_pc_feature_type ) +  \
                               sizeof( cai_rec_hdr_type ) +   \
                                 sizeof( cai_display_rec_var_type ) + 2)
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* EXTENDED SYSTEM PARAMETERS MESSAGE (see IS-95A Section 7.7.2.3.2.13)    */

typedef PACKED struct
{
  byte msg_type[8];             /* Message type */
  byte pilot_pn[9];             /* Pilot PN sequence offset index */
  byte config_msg_seq[6];       /* Configuration message sequence number */
  byte delete_for_tmsi[1];      /* New field for IS-95B */
#ifdef FEATURE_TMSI
  byte pref_msid_type[3];       /* This is the same as JSTD-008 definition
                                   First bit is USE_TMSI bit */
#else
  byte reserved[1];
  byte pref_msid_type[2];       /* USE_TMSI bit not used for non-TMSI */
#endif /* FEATURE_TMSI */
  byte mcc[10];                 /* Mobile country code */
  byte imsi_11_12[7];           /* 11th and 12th digits of the IMSI */
  byte tmsi_zone_len[4];        /* TMSI zone length */
} cai_ext_sysparm_fix1_type;

typedef PACKED struct
{
  byte tmsi_zone[64];           /* TMSI zone */
} cai_ext_sysparm_var_type;

typedef PACKED struct
{
  byte bcast_index[3];            /* Broadcast slot cycle index */
#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte imsi_t_supported[1];
  byte p_rev[8];                  /* P_REV supported by BS */
  byte min_p_rev[8];              /* Minimum P_REV required for access */
  byte soft_slope[6];             /* Soft slop for soft handoff */
  byte add_intercept[6];          /* For IS-95B soft handoff */
  byte drop_intercept[6];         /* For IS-95B soft handoff */
  byte packet_zone_id[8];         /* Packet Data service zone identifier */
  byte max_num_alt_so[3];         /* Max. num. of alternative service options */
  byte reselect_included[1];      /* System Reselection parameters included */
  byte ec_thresh[5];              /* Pilot Power Threshold */
  byte ec_io_thresh[5];           /* Pilot Ec/Io threshold */
  byte pilot_report[1];           /* Pilot reporting indicator */
  byte nghbr_set_entry_info[1];   /* Access entry handoff info included */
  byte acc_ent_ho_order[1];       /* Entry handoff permitted indicator */
  byte nghbr_set_access_info[1];  /* Neighbor set access handoff included */
  byte access_ho[1];              /* Access handoff permitted indicator */
  byte access_ho_msg_rsp[1];      /* AHO permitted for msg response */
  byte access_probe_ho[1];        /* APHO permitted indicator */
  byte acc_ho_list_upd[1];        /* APHO list update permitted indicator*/
  byte acc_probe_ho_other_msg[1]; /* APHO for msg other than ORIG/PR */
  byte max_num_probe_ho[3];       /* Times limit for access probe handoff */
  byte nghbr_set_size[6];         /* Size of neighbor set */
#else
  byte reserved[7];               /* Reserved bits */
#endif   /* FEATURE_IS95B || FEATURE_SPECIAL_MDR */
} cai_ext_sysparm_fix2_type;

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
/* This can be changed after other IS-95B mods */
typedef PACKED struct
{
  byte access_entry_ho[1];        /* Entry handoff permitted */
} cai_ext_sysparm_var1_type;

typedef PACKED struct
{
  byte access_ho_allowed[1];      /* AHO and APHO permitted */
} cai_ext_sysparm_var2_type;

#define MAX_NGHBR_SET_SIZE 40     /* Max num neighbors, IS-95B */

#endif /* FEATURE_IS95B */

/* Minimum size of Extended System Parameters Message */
/* Note that a IS-95B phone can still receive old ESPM, therefore, the
   minimum length should be equal to the min length of the old format */
#define CAI_EXT_SYSPARM_MIN (sizeof( cai_ext_sysparm_fix1_type ) + 10 )

#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte broadcast_gps_asst[1];     /* Broadcast GPS Assist indicator */
  byte qpch_supported[1];         /* Quick Paging Channel Supported indicator */
  byte sdb_supported[1];          /* Short Data Burst supported indicator */
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte mac_cf_supported[1];       /* MAC control function supported indicator */
#endif
  byte rlgain_traffic_pilot[6];   /* Gain Adjustment of the Reverse Traffic
                                     Channel relative to the Reverse Pilot
                                     Channel for Radio Configurations greater
                                     than 2 */
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte rev_pwr_cntl_delay_incl[1];
  byte rev_pwr_cntl_delay[2];
#endif
} cai_ext_sysparm_is2000_fix_type;

typedef PACKED struct
{
  byte num_qpch[2];               /* Number of Quick Paging Channels */
  byte qpch_rate[1];              /* Quick Paging Channel Indicator rate */
  byte qpch_power_level_page[3];  /* Quick Paging Channel paging indicator
                                     transmit power level */
  byte qpch_cci_supported[1];     /* Quick Paging Channel configuration change
                                     indicator supported */
  byte qpch_power_level_config[3]; /* Quick Paging Channel configuration change
                                      indicator transmit power level */
} cai_ext_sysparm_is2000_var_type;

typedef PACKED struct
{
  byte is95b_incl[1];             /* is95b fields included indicator */
  byte is2000_incl[1];            /* is2000 fields included indicator */
} cai_ext_sysparm_inferred_type;
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* STATUS REQUEST MESSAGE (see IS-95-A TSB)                                */

/* NOTE: The following structures define the contents of the
         Status Request Message following the ADDRESS field. */

typedef PACKED struct
{
  byte reserved[4];      /* Reserved bits */
  byte qual_typ[8];      /* Qualification information type */
  byte qual_len[3];      /* Length of qualification information */
/*byte qual_info[?];*/   /* Qualification info (see below) */
  byte num_fields[4];    /* Number of info rec types specified in message */
/*byte rec_typs[8*?];*/  /* List of requested info rec types (see below) */
} cai_pc_streq_fix_type;

typedef PACKED struct
{
  byte band[5];      /* Band class */
  byte reserved[3];  /* Reserved bits */
} cai_qual_info1_type;

typedef PACKED struct
{
  byte band[5];      /* Band class */
  byte op_mode[8];   /* Operating mode */
  byte reserved[3];  /* Reserved bits */
} cai_qual_info2_type;

typedef PACKED struct
{
  byte rec_typ[8];  /* Information record type for requested info */
} cai_streq_var_type;

/* Minimum Status Request Message size includes */
#define CAI_PC_STREQ_MIN  \
   (sizeof( cai_pc_hdr_type ) + CAI_MIN_MS_ADDR_WIDTH_BITS + \
      sizeof( cai_pc_streq_fix_type ))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE REDIRECTION MESSAGE (see IS-95A Section 7.7.2.3.2.16)           */

/* NOTE: The following structures define the contents of the
         Service Redirection Message following the ADDRESS field. */

typedef PACKED struct
{
  byte return_if_fail[1];       /* Return if fail indicator */
  byte delete_tmsi[1];          /* Delete TMSI indicator */
  /* FEATURE_IS95B */
  byte redirect_type[1];        /* Redirect indicator for IS-95B */
  /* end of FEATURE_IS95B */
  byte record_type[8];          /* Redirection record type */
  byte record_len[8];           /* Redirection record length */
} cai_serv_redirect_fix_type;

/* Service Redirection Record Type = '00000001' */
/* Historically, we share the following structures in our code. However,
   in IS-95B, cai_redirect_rec1_type for Service Redirection and Global
   Service Redirection is slightly different. In SRM, 5 bits are reserved.
   In GSRM, the previously reserved 5 bits are defined as max_redirect_delay.
   To minimize impact to the Call Processing code, we will still share these
   structures, but the parser will take the responsibility of clearing
   the max_redirect_delay field for the SRM. */
typedef PACKED struct
{
  byte expected_sid[15];   /* Expected SID */
  byte ignore_cdma[1];     /* Ignore CDMA Available indicator */
  byte sys_ordering[3];    /* System ordering */
  /* FEATURE_IS95B */
  byte max_redirect_delay[5];    /* Max delay for redirection */
  /* end of FEATURE_IS95B */
} cai_redirect_rec1_type;

/* Service Redirection Record Type = '00000002' */
typedef PACKED struct
{
  byte band_class[5];      /* Band class */
  byte expected_sid[15];   /* Expected SID */
  byte expected_nid[16];   /* Expected NID */
  byte reserved[4];        /* Reserved bits */
  byte num_chans[4];       /* Number of CDMA channels */
} cai_redirect_rec2_fix_type;

typedef PACKED struct
{
  byte cdma_chan[11];      /* CDMA Channel number */
} cai_redirect_rec2_var_type;

/* Service Redirection Record Type = 4, redirection to a JTACS system, as
defined in ARIB's RCR STD-36 */
typedef PACKED struct
 {
  byte expected_aid[15];    /* Expected AID */
  byte ignore_cdma[1];      /* Ignore CDMA Available Indicator */
 } cai_redirect_rec4_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* GENERAL PAGE MESSAGE (see IS-95A Section 7.7.2.3.2.17)                  */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte config_msg_seq[6];  /* Configuration message sequence number */
  byte acc_msg_seq[6];     /* Access parameters message sequence number */
  byte class_0_done[1];    /* Class 0 pages are done */
  byte class_1_done[1];    /* Class 1 pages are done */
  byte tmsi_done[1];       /* TMSI pages are done */
  byte ordered_tmsis[1];   /* TMSIs sent in numerical order */
  byte broadcast_done[1];  /* Broadcast pages are done */
  byte reserved2[4];       /* Reserved bits */
  byte add_length[3];      /* Number of octets in the page message
                              specific fields */
} cai_gen_page_fix_type;

/* Variable parts of message */
typedef PACKED struct
{
  byte add_pfield[8];      /* Additional page message specific fields */
} cai_gen_page_pfield_type;

typedef PACKED struct
{
  byte page_class[2];      /* Page class */
  byte page_subclass[2];   /* Page subclass */
} cai_gen_page_class_type;

/* PAGE_CLASS = '00' and PAGE_SUBCLASS = '00' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte imsi_s[34];          /* Last ten digits of the IMSI */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_0_sc_0_type;

/* PAGE_CLASS = '00' and PAGE_SUBCLASS = '01' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte imsi_11_12[7];       /* The 11th and 12th digits of the IMSI */
  byte imsi_s[34];          /* Last ten digits of the IMSI */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_0_sc_1_type;

/* PAGE_CLASS = '00' and PAGE_SUBCLASS = '10' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte mcc[10];             /* Mobile country code */
  byte imsi_s[34];          /* Last ten digits of the IMSI */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_0_sc_2_type;

/* PAGE_CLASS = '00' and PAGE_SUBCLASS = '11' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte mcc[10];             /* Mobile country code */
  byte imsi_11_12[7];       /* The 11th and 12th digits of the IMSI */
  byte imsi_s[34];          /* Last ten digits of the IMSI */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_0_sc_3_type;

/* PAGE_CLASS = '01' and PAGE_SUBCLASS = '00' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte imsi_addr_num[3];    /* Number of IMSI address digits */
  byte imsi_11_12[7];       /* The 11th and 12th digits of the IMSI */
  byte imsi_s[34];          /* Last ten digits of the IMSI */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_1_sc_0_type;

/* PAGE_CLASS = '01' and PAGE_SUBCLASS = '01' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte imsi_addr_num[3];    /* Number of IMSI address digits */
  byte mcc[10];             /* Mobile country code */
  byte imsi_11_12[7];       /* The 11th and 12th digits of the IMSI */
  byte imsi_s[34];          /* Last ten digits of the IMSI */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_1_sc_1_type;

/* PAGE_CLASS = '10' and PAGE_SUBCLASS = '00' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte tmsi_code_addr[32];  /* TMSI code */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_2_sc_0_type;

/* PAGE_CLASS = '10' and PAGE_SUBCLASS = '01' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte tmsi_code_addr[24];  /* TMSI code */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_2_sc_1_type;

/* PAGE_CLASS = '10' and PAGE_SUBCLASS = '10' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte tmsi_code_addr[16];  /* TMSI code */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_2_sc_2_type;

/* PAGE_CLASS = '10' and PAGE_SUBCLASS = '11' */
typedef PACKED struct
{
  byte msg_seq[3];          /* Message sequence number */
  byte tmsi_zone_len[4];    /* TMSI zone length */
  byte tmsi_zone[64];       /* TMSI zone */
  byte tmsi_code_addr[32];  /* TMSI code */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Service option (only present if
                               special_service field is set to 1) */
} cai_pc_2_sc_3_type;

/* PAGE_CLASS = '11' and PAGE_SUBCLASS = '01' */
typedef PACKED struct
{
  byte burst_type[6];       /* Data burst type */
  byte addr_len[4];         /* Address field length */
} cai_pc_3_sc_0_fix_type;

typedef PACKED struct
{
  byte bc_addr[8];          /* Broadcast address */
} cai_pc_3_sc_0_var_type;

/* Minumum size of variable part of message */
#define CAI_GEN_PAGE_MIN \
   (sizeof( cai_gen_page_class_type ) +              \
      sizeof( cai_pc_3_sc_0_fix_type ) + (8*CAI_BC_ADDR_MIN))

/* Maximum number of variable parts of message  */
#define CAI_GEN_PAGE_MAX \
  ((CAI_PC_BODY_SIZE - sizeof( cai_gen_page_fix_type )) / CAI_GEN_PAGE_MIN)

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* GLOBAL SERVICE REDIRECTION MESSAGE (see IS-95A Section 7.7.2.3.2.18)    */

typedef PACKED struct
{
  byte msg_type[8];             /* Message type */
  byte pilot_pn[9];             /* Pilot PN sequence offset index */
  byte config_msg_seq[6];       /* Configuration message sequence number */
  byte redirect_accolc[16];     /* Redirected access overload classes */
  byte return_if_fail[1];       /* Return if fail indicator */
  byte delete_tmsi[1];          /* Delete TMSI indicator */
  byte reserved[1];             /* Reserved bit */
  byte record_type[8];          /* Redirection record type */
  byte record_len[8];           /* Redirection record length */
} cai_global_redirect_type;

/* Use record definitions from Service Redirection Message for Service
   Redirection variable record definitions */

#ifdef FEATURE_IS95B
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* PACA MESSAGE (see IS-95B Section 7.7.2.3.2.20)    */

typedef PACKED struct
{
  byte reserved[7];             /* Reserved bits */
  byte purpose[4];              /* See Table 7.7.2.3.2.20-1 */
  byte q_pos[8];                /* PACA Q position */
  byte paca_timeout[3];         /* PACA state timer duration */
  byte reserved1[5];            /* More reserved bits */
} cai_paca_type;

/* Minimum size of PACA Message */
#define CAI_PACA_MIN (sizeof( cai_pc_hdr_type ) +         \
                          CAI_MIN_MS_ADDR_WIDTH_BITS +      \
                            sizeof( cai_paca_type ))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* EXTENDED CHANNEL ASSIGNMENT MESSAGE (see IS-95B Section 7.7.2.3.2.21)   */

/* NOTE: The following structures define the contents of the
         Channel Assignment Message following the ADDRESS field. */

/* Base of Extended Channel Assignment Message */
typedef PACKED struct
{
  byte reserved1[1];       /* Reserved_1*/
  byte add_record_len[8];  /* Additional record length */
  byte assign_mode[3];     /* Assignment mode */
  byte reserved2[5];       /* Reserved_2 */
} cai_ext_chnasn_base_type;

/* Types for Extended Channel Assigment Message with Assign mode = 000 */
typedef PACKED struct
{
  byte freq_incl[1];       /* Frequency included indicator */
  byte default_config[3];  /* Default configuration */
  byte bypass_alert_answer[1]; /* Bypass indicator */
  byte reserved[1];        /* Reserved */
  byte num_pilots[3];      /* Number of Pilots */
  byte granted_mode[2];    /* Granted mode */
  byte frame_offset[4];    /* Frame offset */
  byte encrypt_mode[2];    /* Message encryption mode */
  byte band_class[5];      /* Band class  (only present if
                              freq_incl field is set to 1) */
  byte cdma_freq[11];      /* Frequency assignment (only present if
                              freq_incl field is set to 1) */
} cai_ext_chnasn_am0_fix_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte for_fch_rc[5];          /* Forward Fundamental Channel Radio Config */
  byte rev_fch_rc[5];          /* Reverse Fundamental Channel Radio Config */
  byte fpc_fch_init_setpt[8];  /* Initial Fundamental Channel Outer Loop
                                  Eb/Nt setpoint */
  byte fpc_subchan_gain[5];    /* Forward Power Control Subchannel Rel Gain */
  byte rl_gain_adj[4];         /* Reverse Traffic Chan Power Relative to
                                  access power */
  byte fpc_fch_fer[5];         /* Fundamental Channel Target Frame Err Rate */
  byte fpc_fch_min_setpt[8];   /* Min Fund Chan Outer Loop Eb/Nt setpoint */
  byte fpc_fch_max_setpt[8];   /* Max Fund Chan Outer Loop Eb/Nt setpoint */
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte rev_fch_gating_mode[1];
  byte rev_pwr_cntl_delay_incl[1];
  byte rev_pwr_cntl_delay[2];
#endif
} cai_ext_chnasn_am0_is2000_type;

typedef PACKED struct
{
  byte is2000_incl[1];      /* is2000 fields included indicator */
} cai_ext_chnasn_am0_inferred_type;
#endif

typedef PACKED struct
{
  byte pilot_pn[9];         /* Pilot PN */
  byte pwr_comb_ind[1];     /* Power control symbol combining indicator */
  byte code_chan[8];        /* Code channel index */
} cai_ext_chnasn_am0_var_type;

/* Extended Channel Assignment Message with Assign mode = 001 */
typedef PACKED struct
{
  byte respond[1];         /* Respond on new Access Channel indicator */
  byte freq_incl[1];       /* Frequency included indicator */
  byte band_class[5];      /* Band class */
  byte cdma_freq[11];      /* Frequency assignment */
  byte num_pilots[6];      /* Number of pilots */
} cai_ext_chnasn_am1_fix_type;

typedef PACKED struct
{
  byte pilot_pn[9];  /* Pilot PN sequence offset index */
} cai_ext_chnasn_am1_var_type;

/* Extended Channel Assignment Message with Assign mode = 010 */
typedef PACKED struct
{
  byte respond[1];         /* Respond on analog control channel indicator */
  byte analog_sys[1];      /* System indicator */
  byte use_analog_sys[1];  /* Use analog system indicator */
  byte band_class[5];      /* Band class */
} cai_ext_chnasn_am2_type;

/* Extended Channel Assignment Message with Assign mode = 011 */
typedef PACKED struct
{
  byte sid[15];            /* System identification of the analog system */
  byte vmac[3];            /* Voice mobile station attenuation code */
  byte analog_chan[11];    /* Voice channel number */
  byte scc[2];             /* SAT color code */
  byte mem[1];             /* Message encryption mode indicator */
  byte an_chan_type[2];    /* Analog voice channel type */
  byte dscc_msb[1];        /* DSCC most significant bit */
  byte band_class[5];      /* Band Class */
} cai_ext_chnasn_am3_type;

#ifdef FEATURE_IS2000
#define FUND_CHAN_ONLY 1
#define DED_CTRL_CHAN_ONLY 2
#define FUND_CHAN_AND_DED_CTRL_CHAN 3

#ifdef FEATURE_IS2000
typedef enum
{

  CAI_PILOT_REC_1X_COM_TD =    0x00,
  CAI_PILOT_1X_COMMON =        0xff
  /* Not part of the std, Added for internal use*/
} cai_pilot_rec_type;
#endif

typedef PACKED struct
{
  byte otd_power_level[2];
  byte reserved[6];
} cai_add_pilot_rec0_type;

typedef PACKED struct
{
  byte pilot_pn[9];
  byte add_pilot_rec_incl[1];
  byte pilot_rec_type[3];
  byte record_len[3];
  byte pwr_comb_ind[1];
  byte code_chan_fch[11];
  byte qof_mask_id_fch[2];
} cai_ext_ch_ind1_pilot_rec_type;

typedef PACKED struct
{
  byte fpc_fch_init_setpt[8];
  byte fpc_fch_fer[5];
  byte fpc_fch_min_setpt[8];
  byte fpc_fch_max_setpt[8];
} cai_ext_ch_ind1_type;

typedef PACKED struct
{
  byte pilot_pn[9];
  byte add_pilot_rec_incl[1];
  byte pilot_rec_type[3];
  byte record_len[3];
  byte pwr_comb_ind[1];
  byte code_chan_dcch[11];
  byte qof_mask_id_dcch[2];
} cai_ext_ch_ind2_pilot_rec_type;

typedef PACKED struct
{
  byte fpc_dcch_init_setpt[8];
  byte fpc_dcch_fer[5];
  byte fpc_dcch_min_setpt[8];
  byte fpc_dcch_max_setpt[8];
} cai_ext_ch_ind2_type;

typedef PACKED struct
{
  byte pilot_pn[9];
  byte add_pilot_rec_incl[1];
  byte pilot_rec_type[3];
  byte record_len[3];
  byte pwr_comb_ind[1];
  byte code_chan_fch[11];
  byte qof_mask_id_fch[2];
  byte code_chan_dcch[11];
  byte qof_mask_id_dcch[2];
} cai_ext_ch_ind3_pilot_rec_type;

typedef PACKED struct
{
  byte fpc_fch_init_setpt[8];
  byte fpc_dcch_init_setpt[8];
  byte fpc_pri_chan[1];
  byte fpc_fch_fer[5];
  byte fpc_fch_min_setpt[8];
  byte fpc_fch_max_setpt[8];
  byte fpc_dcch_fer[5];
  byte fpc_dcch_min_setpt[8];
  byte fpc_dcch_max_setpt[8];
} cai_ext_ch_ind3_type;

/* Extended Channel Assignment Message with Assign mode = 100 */
typedef PACKED struct
{
  byte freq_incl[1];
  byte band_class[5];
  byte cdma_freq[11];
  byte bypass_alert_answer[1];
  byte granted_mode[2];
  byte default_config[3];
  byte for_rc[5];
  byte rev_rc[5];
  byte frame_offset[4];
  byte encrypt_mode[2];
  byte fpc_subchan_gain[5];
  byte rl_gain_adj[4];
  byte num_pilots[3];
  byte ch_ind[2];
  byte ch_record_len[5];
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte rev_fch_gating_mode[1];
  byte rev_pwr_cntl_delay_incl[1];
  byte rev_pwr_cntl_delay[2];
#endif
} cai_ext_chnasn_am4_type;
#endif

/* Minimum record size for a valid Extended Channel Assignment Message */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#else
#define CAI_EXT_CHN_ASN_REC_MIN  (sizeof( cai_pc_hdr_type ) -            \
                                   sizeof( cai_gen_type ) +             \
                                     CAI_MIN_MS_ADDR_WIDTH_BITS +       \
                                       sizeof(cai_ext_chnasn_base_type)+ \
                                         sizeof( cai_ext_chnasn_am2_type ))
#endif

/* Maximum number of Extended Channel Assignment orders that can fit in an
   Extended Channel Assignment Message */
#define CAI_EXT_CHN_ASN_MAX  \
  ((CAI_PC_BODY_SIZE - sizeof( cai_gen_type )) / CAI_EXT_CHN_ASN_REC_MIN)

/* Minimum size for a valid Extended Channel Assignment Message */
#define CAI_EXT_CHN_ASN_MIN  (sizeof( cai_pc_hdr_type ) +         \
                               CAI_MIN_MS_ADDR_WIDTH_BITS +      \
                                 sizeof(cai_ext_chnasn_base_type)+ \
                                   sizeof( cai_ext_chnasn_am2_type ))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* GENERAL NEIGHBOR LIST MESSAGE (see IS-95B Section 7.7.2.3.2.22)         */

/* Fixed Part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte pilot_pn[9];        /* Pilot PN sequence offset index */
  byte config_msg_seq[6];  /* Configuration message sequence number */
  byte pilot_inc[4];       /* Pilot PN sequence offset index increment */
  byte nghbr_srch_mode[2]; /* Neighbor search mode */
  byte nghbr_config_pn_incl[1]; /* Neighbor configuration and PN offset
                                 included. */
  byte freq_fields_incl[1];   /* Frequency fields included */
  byte use_timing[1];         /* Use timing indicator */
  byte global_timing_incl[1]; /* Global timing included.  (Included if
                              use_timing is set to 1) */
  byte global_tx_duration[4];   /* Global neighbor tranmit time duration
                               (Included if global_timing_incl is included
                                and set to 1). */
  byte global_tx_period[7];     /* Global neighbor transmit time period.
                               (Included if global_timing_incl is included
                                and set to 1). */
  byte num_nghbr[6];       /* Number of neighbors */
  byte num_analog_nghbr[3];   /* Number of analog neighbors */
} cai_gen_nghbr_fix_type;

/* Variable part of message */
typedef PACKED struct
{
  byte nghbr_config[3];     /* Neighbor configuration */
  byte nghbr_pn[9];         /* Neighbor pilot PN sequence offset index */
  byte search_pri[2];       /* Pilot Channel search priority */
  byte srch_win_nghbr[4];   /* Neighbor pilot channel search window size */
  byte freq_incl[1];        /* Frequency included indicator */
  byte nghbr_band[5];       /* Neighbor band class */
  byte nghbr_freq[11];      /* Neighbor frequency assignment */
  byte timing_incl[1];      /* Timing included indicator (Included if
                                 use_timing is set to 1). */
  byte nghbr_tx_offset[7];      /* Neighbor transmit time offset. (Included
                                 if timing_incl is included and set to 1).*/
  byte nghbr_tx_duration[4];    /* Neighbor transit time duration.  (Included
                                 if timing_incl is included and set to 1,
                                 and global_timing_incl is set to 0) */
  byte nghbr_tx_period[7];      /* Neighbor transit time period.  (Included
                                 if timing_incl is included and set to 1,
                                 and global_timing_incl is set to 0). */

} cai_gen_nghbr_var_type;

/* Variable number of analog neighbor systems */
typedef PACKED struct
{
 byte band_class[5];       /* Band Class */
 byte sys_a_b[2];          /* System A/B */
} cai_gen_nghbr_analog_var_type;

/* Maximum number of variable parts that will fit in message */
#define CAI_GEN_NGHBR_MAX  40     /* Defined in IS-95B */
#define CAI_GEN_ANALOG_NGHBR_MAX  7     /* Defined in IS-95B */

typedef PACKED struct
{
  cai_gen_nghbr_fix_type fix;
  cai_gen_nghbr_var_type var[CAI_GEN_NGHBR_MAX] ;
  cai_gen_nghbr_analog_var_type var2[CAI_GEN_ANALOG_NGHBR_MAX];
} cai_gen_nghbr_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte srch_offset_incl[1];
} cai_gen_nghbr_is2000_type;

typedef PACKED struct
{
  byte add_pilot_rec_incl[1];
} cai_gen_nghbr_is2000_rec_fix_type;

typedef PACKED struct
{
  byte nghbr_pilot_rec_type[3];
  byte record_len[3];
  byte srch_offset_nghbr[3];
} cai_gen_nghbr_is2000_rec_var_type;

typedef PACKED struct
{
  byte is2000_incl[1];          /* is2000 fields included indicator */
} cai_gen_nghbr_inferred_type;
#endif

#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif

/* Search mode definitions */
#define SRCH_NO_PRI_OR_WIN    0
#define SRCH_PRI_ONLY         1
#define SRCH_WIN_ONLY         2
#define SRCH_PRI_AND_WIN      3

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
/* Extended Global Service Redirection Message (EGSRM), IS2000, 3.7.2.3.2.27 */
/* Fixed Part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte pilot_pn[9];        /* Pilot PN sequence offset index */
  byte config_msg_seq[6];  /* Configuration message sequence number */
  byte redirect_accolc[16]; /* redirected access overload classes */
  byte return_if_fail[1];  /* return if fail indicator */
  byte delete_tmsi[1];     /* delete TMSI indicator */
  byte redirect_p_rev_incl[1]; /* redirection mobile protocol revision included */
  byte record_type[8];     /* redirection record type */
  byte record_len[8];      /* redirection record length */
} cai_ext_glb_redir_fix_type;

/* Variable Part of message */
typedef PACKED struct
{
  byte excl_p_rev_ind[1];  /* excluding mobile protocol revision indicator */
  byte redirect_p_min[8];  /* minimum redirection protocol revision */
  byte redirect_p_max[8];       /* maximum direction protocol revision */
} cai_ext_glb_redir_var_type;

/* for record_type = 01 and 10 use service redirect message definitions */

/* Extended CDMA Channel List Message (ECCLM), IS2000, 3.7.2.3.2.28  */
/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  byte pilot_pn[9];        /* Pilot PN sequence offset index */
  byte config_msg_seq[6];  /* Configuration message sequence number */
  byte num_freq[4];        /* Number of CDMA frequencies */
  byte rc_qpch_sel_incl[1]; /* RC and QPCH selection included indicator */
} cai_ext_chnlist_fix_type;

/* Variable part of message */
typedef PACKED struct
{
  byte cdma_freq[11]; /* CDMA Channel freq assignment */
  byte rc_qpch_hash_ind[1];/* RC_QPCH channel hashing indicator */
} cai_ext_chnlist_var_type;

/* Maximum number of variable message parts that will fit into message */
/* The variable part of the message must have at least cdma_freq */
#define CAI_EXT_CHNLIST_MAX \
        ((CAI_PC_BODY_SIZE - sizeof( cai_ext_chnlist_fix_type )) / FSIZ(cai_ext_chnlist_var_type, cdma_freq))

#endif /* FEATURE_IS2000 */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                   ACCESS CHANNEL MESSAGES                               */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* NOTE: The Access Channel Message structures define the contents of the
         Access Channel Messages following the MSID field. */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* REGISTRATION MESSAGE (see IS-95A Section 6.7.1.3.2.1)                   */

typedef PACKED struct
{
  byte auth_mode[2];        /* Authentication mode */
  byte authr[18];           /* Authentication data
                               (only present if auth_mode is set to '01' */
  byte randc[8];            /* Random challenge value
                               (only present if auth_mode is set to '01' */
  byte count[6];            /* Call history parameter
                               (only present if auth_mode is set to '01' */
  byte reg_type[4];         /* Registration type */
  byte slot_cycle_index[3]; /* Slot cycle index */
  byte mob_p_rev[8];        /* Protocol revision of the mobile station */
  byte scm[8];              /* Station class mark */
  byte mob_term[1];         /* Mobile terminated calls accepted indicator */
  /* FEATURE_IS95B */
  byte return_cause[4];     /* Return cause for IS-95B */
  /* end of FEATURE_IS95B */
} cai_reg_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte qpch_supported[1];
  byte enhanced_rc[1];
  byte uzid_incl[1];
  byte uzid[16];
} cai_reg_is2000_type;
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* ACCESS CHANNEL ORDER MESSSAGE (see IS-95A Section 6.7.1.3.2.2)          */

/* Generic Order Message */
typedef PACKED struct
{
  byte reserved[2];       /* Reserved bits */
  byte order[6];          /* Order code */
  byte add_record_len[3]; /* Additional record length */
} cai_ac_gen_ord_type;

/* Order Message with Order Qualifier */
typedef PACKED struct
{
  byte reserved[2];       /* Reserved bits */
  byte order[6];          /* Order code */
  byte add_record_len[3]; /* Additional record length */
  byte ordq[8];           /* Order qualification code */
} cai_ac_ordq_type;

/* Base Station Challenge Order */
typedef PACKED struct
{
  byte reserved[2];       /* Reserved bits */
  byte order[6];          /* Order code */
  byte add_record_len[3]; /* Additional record length */
  byte ordq[8];           /* Order qualification code */
  byte randbs[32];        /* Random challenge data */
} cai_ac_bs_chal_type;

/* Mobile Station Reject Orders */
typedef PACKED struct
{
  byte reserved[2];       /* Reserved bits */
  byte order[6];          /* Order code */
  byte add_record_len[3]; /* Additional record length */
  byte ordq[8];           /* Order qualification code */
  byte rej_msg_type[8];   /* Mesage type of rejected message */
} cai_ac_rej_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte rejected_pdu_type[2];
  byte reserved[6];
} cai_ac_rej_is2000_type;
#endif

typedef PACKED struct
{
  byte reserved[2];       /* Reserved bits */
  byte order[6];          /* Order code */
  byte add_record_len[3]; /* Additional record length */
  byte ordq[8];           /* Order qualification code */
  byte rej_msg_type[8];   /* Mesage type of rejected message */
  byte rej_order[8];      /* Order type of rejected message */
  byte rej_ordq[8];       /* Order qualification code of rejected message */
} cai_ac_rej_ord_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte rejected_pdu_type[2];
  byte reserved[6];
} cai_ac_rej_ord_is2000_type;
#endif

typedef PACKED struct
{
  byte reserved[2];       /* Reserved bits */
  byte order[6];          /* Order code */
  byte add_record_len[3]; /* Additional record length */
  byte ordq[8];           /* Order qualification code */
  byte rej_msg_type[8];   /* Mesage type of rejected message */
  byte rej_record[8];     /* Record type of the rejected info record */
} cai_ac_rej_rec_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte rejected_pdu_type[2];
  byte reserved[6];
} cai_ac_rej_rec_is2000_type;
#endif

/* Access Channel Order Message */
typedef union
{
  cai_ac_gen_ord_type gen;
  cai_ac_ordq_type    ordq;
  cai_ac_bs_chal_type chal;
  cai_ac_rej_type     rej;
  cai_ac_rej_ord_type rej_ord;
  cai_ac_rej_rec_type rej_rec;
} cai_ac_ord_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* TMSI ASSIGNMENT C0MPLETION MESSAGE (see J-STD-008 Section 2.7.1.3.2.8)  */

/* TMSI Assignment Completion Message - Access Channel */
typedef PACKED struct
{
  byte auth_mode[2];     /* Authentication mode */
  byte authr[18];        /* Authentication data
                            (only present if auth_mode is set to '01' */
  byte randc[8];         /* Random challenge value
                            (only present if auth_mode is set to '01' */
  byte count[6];         /* Call history parameter
                            (only present if auth_mode is set to '01' */
  byte reserved[6];      /* Reserved             */
} cai_ac_tmsi_cmp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DATA BURST MESSAGE (see IS-95A Section 6.7.1.3.2.3)                     */

/* Fixed part of message */
typedef PACKED struct
{
  byte auth_mode[2];     /* Authentication mode */
  byte authr[18];        /* Authentication data
                            (only present if auth_mode is set to '01' */
  byte randc[8];         /* Random challenge value
                            (only present if auth_mode is set to '01' */
  byte count[6];         /* Call history parameter
                            (only present if auth_mode is set to '01' */
  byte msg_number[8];    /* Message number within the data burst stream */
  byte burst_type[6];    /* Data burst type */
  byte num_msgs[8];      /* Number of messages in the data burst stream */
  byte num_fields[8];    /* Number of characters in this message */
} cai_burst_ac_fix_type;

/*  Variable part of message */
typedef byte cai_burst_ac_var_type[8];  /* Characters */

/* Maximum number of variable length message parts */
#define CAI_AC_BURST_MAX  \
   ((CAI_AC_BODY_SIZE - sizeof( cai_ac_hdr_type ) - \
       sizeof( cai_msid1_type ) - sizeof( cai_burst_ac_fix_type ) + \
         FSIZ( cai_burst_ac_fix_type, authr ) +     \
         FSIZ( cai_burst_ac_fix_type, randc ) +     \
         FSIZ( cai_burst_ac_fix_type, count )) /    \
           sizeof( cai_burst_ac_var_type ))

typedef PACKED struct
{
  cai_burst_ac_fix_type fix;
  cai_burst_ac_var_type var[CAI_AC_BURST_MAX];
} cai_burst_ac_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* ORIGINATION MESSAGE (see IS-95A Section 6.7.1.3.2.4)                    */

/* Fixed type for Origination message */
typedef PACKED struct
{
  byte auth_mode[2];        /* Authentication mode */
  byte authr[18];           /* Authentication data
                               (only present if auth_mode is set to '01' */
  byte randc[8];            /* Random challenge value
                               (only present if auth_mode is set to '01' */
  byte count[6];            /* Call history parameter
                               (only present if auth_mode is set to '01' */
  byte mob_term[1];         /* Mobile terminated calls accepted indicator */
  byte slot_cycle_index[3]; /* Slot cycle index */
  byte mob_p_rev[8];        /* Protocol revision of the mobile station */
  byte scm[8];              /* Station class mark */
  byte request_mode[3];     /* Requested mode code */
  byte special_service[1];  /* Special service option indicator */
  byte service_option[16];  /* Requested service option for this origination
                               (only present if special_service field is
                                set to 1) */
  byte pm[1];               /* Privacy mode indicator */
  byte digit_mode[1];       /* Digit mode indicator */
  byte number_type[3];      /* Type of number (only present if digit_mode
                               field is set to 1) */
  byte number_plan[4];      /* Numbering plan (only present if digit_mode
                               field is set to 1) */
  byte more_fields[1];      /* More dialed digits indicator */
  byte num_fields[8];       /* Number of dialed digits in this message */
} cai_orig_fix1_type;

typedef union
{
  byte char4[4];  /* 4 bit representation of digits */
  byte char8[8];  /* 8 bit representation of digits */
} cai_orig_var_type;

typedef PACKED struct
{
  byte nar_an_cap[1];     /* Narrow analog capability */
  /* FEATURE_IS95B */
  byte paca_reorig[1];    /* PACA reorigination for IS-95B */
  byte return_cause[4];   /* Return cause for IS-95B */
  byte more_records[1];   /* More records indicator */
  byte encry_sup[4];      /* Encryption Supported */
  byte paca_sup[1];       /* PACA Supported */
  byte num_alt_so[3];     /* Number of alternative SO */
  /* end of FEATURE_IS95B */
} cai_orig_fix2_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte drs[1];
  byte uzid_incl[1];
  byte ch_ind[2];
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte blob_incl[1];
  byte new_so_ind[1];
#else
  byte sr_id[3];
#endif
  byte otd_supported[1];
  byte qpch_supported[1];
  byte enhanced_rc[1];
  byte for_rc_pref[5];
  byte rev_rc_pref[5];
  byte fch_supported[1];
  byte dcch_supported[1];
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte reserved[1];
  byte rev_fch_gating_req[1];
#endif
} cai_orig_fix_is2000_type;

typedef PACKED struct
{
  byte uzid[16];
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte blob[7];
#endif
} cai_orig_var_is2000_type;

typedef PACKED struct
{
  byte fch_frame_size[1];
  byte for_fch_len[3];
  byte for_fch_rc_map[3];
  byte rev_fch_len[3];
  byte rev_fch_rc_map[3];
} cai_fch_spec_fields_type;

typedef PACKED struct
{
  byte dcch_frame_size[2];
  byte for_dcch_len[3];
  byte for_dcch_rc_map[3];
  byte rev_dcch_len[3];
  byte rev_dcch_rc_map[3];
} cai_dcch_spec_fields_type;

typedef PACKED struct
{
  byte for_sch_len[3];
  byte for_sch_rc_map[3];
  byte for_sch_num[2];
  byte for_turbo_supported[1];
  byte for_max_turbo_block_size[4];
  byte for_conv_supported[1];
  byte for_max_conv_block_size[4];
  byte for_frame_40_supported[1];
  byte for_frame_40_rate[4];
  byte for_frame_80_supported[1];
  byte for_frame_80_rate[4];
} cai_for_sch_spec_fields_type;

typedef PACKED struct
{
  byte rev_sch_len[3];
  byte rev_sch_rc_map[3];
  byte rev_sch_num[2];
  byte rev_turbo_supported[1];
  byte rev_max_turbo_block_size[4];
  byte rev_conv_supported[1];
  byte rev_max_conv_block_size[4];
  byte rev_frame_40_supported[1];
  byte rev_frame_40_rate[4];
  byte rev_frame_80_supported[1];
  byte rev_frame_80_rate[4];
} cai_rev_sch_spec_fields_type;
#endif

/* FEATURE_IS95B */
typedef PACKED struct
{
  byte alt_so[16];        /* Alternative service option */
} cai_alt_so_type;
/* end of FEATURE_IS95B */

/* Maximum number of variable length message parts */
#define CAI_AC_ORIG_MAX NV_MAX_DIAL_DIGITS

/* Maximum number of alternative service options */
#define CAI_AC_ALT_SO_MAX   7

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PAGE RESPONSE MESSAGE  (see IS-95A Section 6.7.1.3.2.5)                 */

typedef PACKED struct
{
  byte auth_mode[2];        /* Authentication mode */
  byte authr[18];           /* Authentication data
                               (only present if auth_mode is set to '01' */
  byte randc[8];            /* Random challenge value
                               (only present if auth_mode is set to '01' */
  byte count[6];            /* Call history parameter
                               (only present if auth_mode is set to '01' */
  byte mob_term[1];         /* Mobile terminated calls accepted indicator */
  byte slot_cycle_index[3]; /* Slot cycle index */
  byte mob_p_rev[8];        /* Protocol revision of the mobile station */
  byte scm[8];              /* Station class mark */
  byte request_mode[3];     /* Requested mode code */
  byte service_option[16];  /* Service option */
  byte pm[1];               /* Privacy mode indicator */
  byte nar_an_cap[1];       /* Narrow analog capability */
  byte encry_sup[4];        /* Encryption Supported for IS-95B */
  byte num_alt_so[3];       /* Number of alternative SO */
} cai_page_resp_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte uzid_incl[1];
  byte ch_ind[2];
  byte otd_supported[1];
  byte qpch_supported[1];
  byte enhanced_rc[1];
  byte for_rc_pref[5];
  byte rev_rc_pref[5];
  byte fch_supported[1];
  byte dcch_supported[1];
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte rev_fch_gating_req[1];
#endif
} cai_page_resp_fix_is2000_type;

typedef PACKED struct
{
  byte uzid[16];
} cai_page_resp_var_is2000_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* AUTHENTICATION CHALLENGE RESPONSE MSG (see IS-95A Section 6.7.1.3.2.6)  */

typedef PACKED struct
{
  byte auth_mode[2];     /* Authentication mode */
  byte authu[18];        /* Authentication challenge response */
} cai_auth_ch_resp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* STATUS RESPONSE MESSAGE (see IS-95-A TSB)                               */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_ac_hdr_type hdr;  /* Access Channel common header fields */
  cai_msid0_type msid;  /* Mobile station identifier fields */
  byte auth_mode[2];    /* Authentication mode */
  byte qual_typ[8];     /* Qualification info type (defined with pc version
                           of Status Request Message) */
  byte qual_len[3];     /* Qualification info length */
/*byte qual_info[?];*/  /* Qualification info (defined with pc version of
                           Status Request Message */
/*byte info_recs[?];*/  /* Information records (arbitrary number) */
  byte reserved2[3];    /* More reserved bits */
} cai_ac_strsp_fix_type;

#ifdef FEATURE_IS95B
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PACA CANCEL MESSAGE (see IS-95B Section 6.7.1.3.2.9)                    */

typedef PACKED struct
{
  byte auth_mode[2];        /* Authentication mode */
  byte authr[18];           /* Authentication data
                               (only present if auth_mode is set to '01' */
  byte randc[8];            /* Random challenge value
                               (only present if auth_mode is set to '01' */
  byte count[6];            /* Call history parameter
                               (only present if auth_mode is set to '01' */
} cai_ac_paca_can_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* EXTENDED STATUS RESPONSE MESSAGE (see IS-95B Section 6.7.1.3.2.10)      */

typedef PACKED struct
{
  byte auth_mode[2];        /* Authentication mode */
  byte qual_info_type[8];   /* Qualification info (defined with pc version of
                              Status Request Message */
  byte qual_info_len[3];    /* Qualification info length */
  byte num_info_rec[4];     /* More reserved bits */
} cai_ac_ext_strsp_fix_type;
#endif /* FEATURE_IS95B */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PILOT REPORT ( see IS-95B Section 6.7.1.3.2 )                           */

typedef PACKED struct
{
  byte act_plt_str[6];       /* Active Pilot strength */
  byte first_is_act[1];      /* Active Pilot is the first pilot on which an
                                access probe was sent */
  byte first_is_pta[1];      /* The first pilo tis the previous to the active
                                pilot on which an access probe was sent */
  byte num_add_plt[3];       /* Number of additional reported pilots */
} cai_plt_rpt_fix_type;

typedef PACKED struct
{
  byte plt_pn_phase[15];     /* Pilot measured phase */
  byte plt_strength[6];      /* Pilot strength */
  byte acc_ho_en[1];         /* Access Handoff enable */
  byte acc_attempted[1];     /* Access attempted flag */
} cai_plt_rpt_var_type;

#define CAI_MAX_PLT_RPT_SIZE  (sizeof(cai_plt_rpt_fix_type)+ \
                                CAI_MAX_ADD_PILOTS* \
                                  sizeof( cai_plt_rpt_var_type ))

#endif /* defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                         TRAFFIC CHANNEL MESSAGES                        */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                FORWARD TRAFFIC CHANNEL ORDER MESSAGE                    */

/* Generic Order Message (see IS-95A Section 7.7.3.3.2.1 Order Message)    */
typedef PACKED struct
{
  byte msg_type[8];       /* Message type */
  cai_tc_hdr_type hdr;    /* Layer 2 header */
  byte use_time[1];       /* Use action time indicator */
  byte action_time[6];    /* Action time */
  byte order[6];          /* Order field */
  byte add_record_len[3]; /* Additional Record Length */
} cai_ftc_gen_ord_type;

/* Order Message with Order Qualifier (see IS-95A Section 7.7.4 Orders)    */
typedef PACKED struct
{
  byte msg_type[8];       /* Message type */
  cai_tc_hdr_type hdr;    /* Layer 2 header */
  byte use_time[1];       /* Use action time indicator */
  byte action_time[6];    /* Action time */
  byte order[6];          /* Order field */
  byte add_record_len[3]; /* Additional Record Length */
  byte ordq[8];           /* Order qualification code */
} cai_ftc_ordq_type;

/* Base Station Challenge Confirmation Order (see IS-95A Section 7.7.4.1) */
typedef PACKED struct
{
  byte msg_type[8];       /* Message type */
  cai_tc_hdr_type hdr;    /* Layer 2 header */
  byte use_time[1];       /* Use action time indicator */
  byte action_time[6];    /* Action time */
  byte order[6];          /* Order field */
  byte add_record_len[3]; /* Additional Record Length */
  byte ordq[8];           /* Order qualification code */
  byte authbs[18];        /* Challenge response */
  byte reserved[6];       /* Reserved bits */
} cai_ftc_chal_type;

/* Service Option Request Order (see IS-95A Section 7.7.4.2) */
typedef PACKED struct
{
  byte msg_type[8];         /* Message type */
  cai_tc_hdr_type hdr;      /* Layer 2 header */
  byte use_time[1];         /* Use action time indicator */
  byte action_time[6];      /* Action time */
  byte order[6];            /* Order field */
  byte add_record_len[3];   /* Additional Record Length */
  byte ordq[8];             /* Order qualification code */
  byte service_option[16];  /* Service option */
} cai_ftc_so_req_type;

/* Service Option Response Order (see IS-95A Section 7.7.4.3) */
typedef PACKED struct
{
  byte msg_type[8];         /* Message type */
  cai_tc_hdr_type hdr;      /* Layer 2 header */
  byte use_time[1];         /* Use action time indicator */
  byte action_time[6];      /* Action time */
  byte order[6];            /* Order field */
  byte add_record_len[3];   /* Additional Record Length */
  byte ordq[8];             /* Order qualification code */
  byte service_option[16];  /* Service option */
} cai_ftc_so_res_type;

/* Status Request Order (see IS-95A Section 7.7.4.4) */
typedef PACKED struct
{
  byte msg_type[8];         /* Message type */
  cai_tc_hdr_type hdr;      /* Layer 2 header */
  byte use_time[1];         /* Use action time indicator */
  byte action_time[6];      /* Action time */
  byte order[6];            /* Order field */
  byte add_record_len[3];   /* Additional Record Length */
  byte ordq[8];             /* Order qualification code */
  byte sid[15];             /* System identification */
  byte nid[16];             /* Network identification */
  byte reserved[1];         /* Reserved bits */
} cai_ftc_stat_req_type;

#ifdef FEATURE_IS95B
/* Periodic Pilot Measurement Order (see IS-95B Section 7.7.4.6) */
typedef PACKED struct
{
  byte msg_type[8];         /* Message type */
  cai_tc_hdr_type hdr;      /* Layer 2 header */
  byte use_time[1];         /* Use action time indicator */
  byte action_time[6];      /* Action time */
  byte order[6];            /* Order field */
  byte add_record_len[3];   /* Additional Record Length */
  byte ordq[8];             /* Order qualification code */
  byte min_pilot_pwr_thresh[5];   /* Threshold of Ec of pilots
                                     in Active set */
  byte min_pilot_ec_io_thresh[5]; /* Pilot Strength Threshold of
                                     Serving Frequency */
  byte reserved[6];         /* Reserved Bits */
} cai_ftc_ppm_type;

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
typedef PACKED struct
{
  byte msg_type[8];         /* Message type */
  cai_tc_hdr_type hdr;      /* Layer 2 header */
  byte use_time[1];         /* Use action time indicator */
  byte action_time[6];      /* Action time */
  byte order[6];            /* Order field */
  byte add_record_len[3];   /* Additional Record Length */
  byte ordq[8];             /* Order qualification code */
  byte retry_type[3];
  byte retry_delay[8];
  byte reserved[5];         /* Reserved Bits */
} cai_ftc_retry_type;
#endif
#endif

/* Forward Traffic Channel Order Message */
typedef union
{
  cai_ftc_gen_ord_type gen;
  cai_ftc_ordq_type ordq;
  cai_ftc_chal_type chal;
  cai_ftc_so_req_type so_req;
  cai_ftc_so_res_type so_res;
  cai_ftc_stat_req_type stat_req;
  #ifdef FEATURE_IS95B
  cai_ftc_ppm_type       ppm;
  #endif /* FEATURE_IS95B */
#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  cai_ftc_retry_type     retry;
#endif
#endif

} cai_ftc_ord_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* AUTHENTICATION CHALLENGE MESSAGE (see IS-95A Section 7.7.3.3.2.2)       */
typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte randu[24];        /* Random challenge data */
  byte reserved[7];      /* Reserved bits */
} cai_auth_ch_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* ALERT WITH INFORMATION MESSAGE (see IS-95A Section 7.7.3.3.2.3)         */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
} cai_alert_fix_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* TMSI ASSIGNMENT MESSAGE (see J-STD-008 Section 3.7.3.3.2.22)            */

typedef PACKED struct  /* TMSI Assignment Message - Forward Traffic Channel */
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte tmsi_zone_len[4];    /* TMSI zone length     */
  byte tmsi_zone[64];       /* TMSI zone            */
  byte tmsi_code[32];       /* TMSI code address    */
  byte tmsi_exp_time[24];   /* TMSI expiration time */
  byte reserved[3];         /* Reserved             */
} cai_ftc_tmsi_asgn_type;

/* Minimum TMSI Assignment Message size includes */
/* the smallest TMSI zone that can be set.       */
#define CAI_FTC_TMSI_MIN  \
   (sizeof( cai_ftc_tmsi_asgn_type ) - 56)

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* DATA BURST MESSAGE (see IS-95A Section 7.7.3.3.2.4)                     */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte msg_number[8];    /* Number of message in the data stream burst */
  byte burst_type[6];    /* Type of data burst */
  byte num_msgs[8];      /* Number of messages in the data stream burst */
  byte num_fields[8];    /* Number of characters in field */
} cai_burst_tc_fix_type;

/* Variable part of message */
typedef byte cai_burst_tc_var_type[8];
  /* Characters for one way data burst */

/* Maximum number of variable parts of message */
#define CAI_FWD_TC_BURST_MAX  TC_FWD_MAX( cai_burst_tc )

typedef PACKED struct
{
  cai_burst_tc_fix_type fix;
  cai_burst_tc_var_type var[CAI_FWD_TC_BURST_MAX];
} cai_fwd_tc_burst_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* ANALOG HANDOFF DIRECTION MESSAGE (see IS-95A Section 7.7.3.3.2.6)       */

typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte use_time[1];      /* Indicator of whether to use the ACTION_TIME */
  byte action_time[6];   /* Action time */
  byte sid[15];          /* System ID of analog system */
  byte vmac[3];          /* Voice mobile station attenuation code */
  byte analog_chan[11];  /* Analog voice channel number */
  byte scc[2];           /* SAT color code */
  byte mem[1];           /* Message encryption mode indicator */
  byte an_chan_type[2];  /* Analog voice channel type */
  byte dscc_msb[1];      /* DSCC most significant bit */
  byte band_class[5];    /* Changed for IS-95B */
#ifdef FEATURE_T53
#error code not present
#endif
} cai_fm_ho_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* IN-TRAFFIC SYSTEM PARAMETER MESSAGE (see IS-95A Section 7.7.3.3.2.7)    */

typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte sid[15];          /* System identification */
  byte nid[16];          /* Network identification */
  byte srch_win_a[4];    /* Characteristic field for Active Pilot Set */
  byte srch_win_n[4];    /* Characteristic field for Neighbor Pilot Set */
  byte srch_win_r[4];    /* Characteristic field for the Remaining set */
  byte t_add[6];         /* Pilot detection threshold */
  byte t_drop[6];        /* Pilot drop threshold */
  byte t_comp[4];        /* Active Set vs Candidate set comparsion
                            threshold */
  byte t_tdrop[4];       /* Drop timer value */
  byte nghbr_max_age[4]; /* Neighbor set member maximum age for retention */
  byte p_rev[8];         /* Protocol Revision Level */
  byte soft_slope[6];    /* Slope in criterion to add/drop active set pilots*/
  byte add_intercept[6]; /* Intercept in criterion to add to active set */
  byte drop_intercept[6];/* Intercept in criterion for dropping pilot */
  byte packet_zone_id[8];/* Packet data services zone identifier */
  byte extension[1];     /* Indicator that extension fields are present */
  byte t_mulchan[3];     /* Supplemental Channel Request Message pilot
                          * strength reporting offset. */
  byte begin_preamble[3];/* Number of preamble frames at beginning */
  byte resume_preamble[3];/* Number of preamble frames at resumption */
} cai_tc_sysparm_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte t_slotted_incl[1];
  byte t_slotted[8];
} cai_tc_sysparm_is2000_type;
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* NEIGBHOR LIST UPDATE MESSAGE (see IS-95A Section 7.7.3.3.2.8)           */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte pilot_inc[4];     /* Pilot PN sequence offset index increment */
} cai_nlu_fix_type;

/* Variable part of message */
typedef PACKED struct
{
  byte nghbr_pn[9];      /* Neighbor pilot PN sequence offset index */
} cai_nlu_var_type;

/* Maximum number of neighbors in message */
#define CAI_NLU_MAX  TC_FWD_MAX( cai_nlu )

typedef PACKED struct
{
  cai_nlu_fix_type fix;
  cai_nlu_var_type var[CAI_NLU_MAX];
} cai_nlu_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                SEND BURST DTMF MESAGE                                   */
/* (See IS-95A Section 7.7.3.3.2.9 Send Burst DTMF Message)                */

typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  cai_tc_hdr_type hdr;     /* Layer 2 header */
  byte num_digits[8];      /* Number of DTMF digits in message */
  byte dtmf_on_length[3];  /* DTMF pulse width code */
  byte dtmf_off_length[3]; /* DTMF interdigit interval code */
} cai_sbdtmf_fix_type;

typedef byte cai_sbdtmf_var_type[4];

#define CAI_SBDTMF_MAX 255
  /* Max number of digits is constrained by size of the num_digits field */

typedef PACKED struct
{
  cai_sbdtmf_fix_type fix;
  cai_sbdtmf_var_type var[CAI_SBDTMF_MAX];
} cai_sbdtmf_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* POWER CONTROL PARAMETERS MESSAGE (see IS-95A Section 7.7.3.3.2.10)      */

typedef PACKED struct
{
  byte msg_type[8];          /* Message type */
  cai_tc_hdr_type hdr;       /* Layer 2 header */
  byte pwr_rep_thresh[5];    /* Power control reporting threshold */
  byte pwr_rep_frames[4];    /* Power control reporting frame count */
  byte pwr_thresh_enable[1]; /* Threshold report mode indicator */
  byte pwr_period_enable[1]; /* Threshold report mode indicator */
  byte pwr_rep_delay[5];     /* Power Measurement report delay */
  byte reserved[7];          /* Reserved bits */
} cai_pwr_ctl_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* RETRIEVE PARAMETERS MESSAGE (see IS-95A Section 7.7.3.3.2.11)           */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
} cai_ret_parms_fix_type;

typedef PACKED struct
{
  byte parameter_id[16];  /* Parameter identification */
} cai_ret_parms_var_type;

#define CAI_RET_PARMS_MAX  TC_FWD_MAX( cai_ret_parms )
  /* Maximum number of variable type that will fit in message */

typedef PACKED struct
{
  cai_ret_parms_fix_type fix;
  cai_ret_parms_var_type var[CAI_RET_PARMS_MAX];
} cai_ret_parms_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SET PARAMETERS MESSAGE (see IS-95A Section 7.7.3.3.2.12)                */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
} cai_set_parm_fix_type;

typedef PACKED struct
{
  byte parameter_id[16];   /* Identification for parameter */
  byte parameter_len[10];  /* length in bits of returned parameter */
  byte parameter[1];       /* First bit of parameter */
} cai_set_parm_var_type;

/* Maximum number of variable types in message */
#define CAI_SET_PARM_MAX ((CAI_FWD_TC_MSG_BODY_SIZE -                      \
                          sizeof( cai_set_parm_fix_type )) /               \
                          ( FSIZ( cai_set_parm_var_type, parameter_id ) +  \
                            FSIZ( cai_set_parm_var_type, parameter_len ) + \
                            CAI_PARM_MIN_LEN))

typedef PACKED struct
{
  cai_set_parm_fix_type fix;
  cai_set_parm_var_type var;
} cai_set_parm_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SSD UPDATE MESSAGE (see IS-95A Section 7.7.3.3.2.13)                    */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
  byte randssd[56];     /* Random data */
  byte reserved[7];     /* Reserved bits */
} cai_ssd_up_type;


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* FLASH WITH INFORMATION MESSAGE (see IS-95A Section 7.7.3.3.2.14)        */

/* Use cai_alert_fix_type from Alert With Information record type
   definition above */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* MOBILE STATION REGISTERED MESSAGE (see IS-95A Section 7.7.3.3.2.15)     */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
  byte sid[15];         /* System identification */
  byte nid[16];         /* Network identification */
  byte reg_zone[12];    /* Registration zone */
  byte total_zones[3];  /* Number of registration zones to be retained */
  byte zone_timer[3];   /* Zone timer length */
  byte mult_sids[1];    /* Multiple SID storage indicator */
  byte mult_nids[1];    /* Multiple NID storage indicator */
  byte base_lat[22];    /* Base station latitude */
  byte base_long[23];   /* Base station longitude */
  byte reg_dist[11];    /* Registration distance */
  byte reserved[4];     /* Reserved bits */
} cai_registered_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* STATUS REQUEST MESSAGE (see IS-95-A TSB)                                */

typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte qual_typ[8];      /* Qualification info type (see pc version of this
                            message for definitions). */
  byte qual_len[3];      /* Qualfication info length. */
/*byte qual_info[?];*/   /* Qualification info (see pc version of this
                            message for definitions */
  byte num_fields[4];    /* Number of info rec types specified in message */
/*byte rec_typs[8*?];*/  /* List of requested info rec types */
} cai_tc_streq_fix_type;

/* Note: cai_streq_var type is defined above with pc version of the
   Status Request Message. */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE REQUEST MESSAGE (see IS-95-A TSB)                               */

/* Purpose codes for use in service request and response messages. */
#define CAI_SRV_ACC_PURP  0  /* Accept code (only for rtc messages) */
#define CAI_SRV_REJ_PURP  1  /* Reject code */
#define CAI_SRV_PRO_PURP  2  /* Propose code */

/* Fixed portion of Service Request Message body */
typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
  byte req_seq[3];      /* Request sequence number */
  byte purp[4];         /* Request purpose */
} cai_ftc_srv_req_fix_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE RESPONSE MESSAGE (see IS-95-A TSB)                              */

/* Fixed portion of Service Response Message body */
typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
  byte req_seq[3];      /* Request sequence number */
  byte purp[4];         /* Response purpose */
} cai_ftc_srv_rsp_fix_type;


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE CONNECT MESSAGE (see IS-95-A TSB)                               */

/* Fixed portion of Service Connect Message body */
typedef PACKED struct
{
  byte msg_type[8];       /* Message type */
  cai_tc_hdr_type hdr;    /* Layer 2 header */
  byte use_time[1];       /* Use action time indicator */
  byte action_time[6];    /* Action time */
  byte con_seq[3];        /* Connect sequence number */
  byte reserved[5];       /* Reserved bits */
} cai_srv_con_fix_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte record_type2[8];
  byte record_len2[8];
  byte rc_sync_id_incl[1];
  byte rc_sync_id[4];
  byte blob_incl[1];
  byte blob[7];
} cai_srv_con_is2000_type;

typedef PACKED struct
{
  byte sr_id[3];
  byte logical_resource[4];
  byte physical_resource[4];
  byte forward_flag[1];
  byte reverse_flag[1];
  byte priority[4];
} cai_lpm_entry_type;

typedef PACKED struct
{
  byte fpc_incl[1];
  byte fpc_pri_chan[1];
  byte fpc_mode[3];
  byte fpc_olpc_fch_incl[1];
  byte fpc_fch_fer[5];
  byte fpc_fch_min_setpt[8];
  byte fpc_fch_max_setpt[8];
  byte fpc_olpc_dcch_incl[1];
  byte fpc_dcch_fer[5];
  byte fpc_dcch_min_setpt[8];
  byte fpc_dcch_max_setpt[8];
  byte gating_rate_incl[1];
  byte pilot_gate_rate[2];
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte lpm_ind[1];
#else
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte reserved[2];
#endif
  byte lpm_ind[2];
#endif
  byte num_lpm_entries[4];
} cai_non_neg_srv_cfg_type;
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE OPTION CONTROL MESSAGE (see IS-95-A TSB)                        */

/* Fixed portion of Service Option Control Message body */
typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte use_time[1];      /* Use action time indicator */
  byte action_time[6];   /* Action time */
  byte con_ref[8];       /* Request sequence number */
  byte so[16];           /* Service option */
  byte tsf_len[8];       /* Control record length */
} cai_ftc_srv_ctl_fix_type;

/* Variable portion of Service Option Control Message body */
typedef PACKED struct
{
  byte tsf[ 8 ];  /* Type specific field */
} cai_ftc_srv_ctl_var_type;

#define  CAI_FTC_SRV_CTL_MAX  TC_FWD_MAX ( cai_ftc_srv_ctl )
  /* Maximumn number of type specific octets in message */

/* Complete Service Option Control Message body */
typedef PACKED struct
{
  cai_ftc_srv_ctl_fix_type  fix;                        /* Fixed part */
  cai_ftc_srv_ctl_var_type  var[ CAI_FTC_SRV_CTL_MAX ]; /* Variable part */
} cai_ftc_srv_ctl_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This section defines a generic handoff direction message.               */
/* HANDOFF DIRECTION MESSAGE (see IS-95A Section 7.7.3.3.2.5)              */
/* EXTENDED HANDOFF DIRECTION MESSAGE (see IS-95B Section 7.7.3.3.2.17)    */
/* GENERAL HANDOFF DIRECTION MESSAGE (see IS-95B Section 7.7.3.3.2.31)     */

typedef PACKED struct
{
  byte msg_type[8];           /* Message type */
  cai_tc_hdr_type hdr;        /* Layer 2 header */
  byte use_time[1];           /* Use action_time indicator */
  byte hdm_seq[2];            /* Handoff Direction Message sequence number */
  byte srch_inc[1];           /* Search Parameters included */
  byte extra_parms[1];        /* Hard Handoff plus other parms included */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte sup_chan_parms_inc[1]; /* Supplemental Channel Parms included */
  byte use_pwr_cntl_step[1];  /* Use Power Control Step indicator */
#endif /* defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */

  byte num_pilots[3];         /* Number of pilot records included */
} cai_ho_msg_fix_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte parms_incl[1];
  byte fpc_subchan_gain[5];
  byte use_pc_time[1];
  byte pc_action_time[6];
  byte rlgain_traffic_pilot[6];
  byte default_rlag[1];
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  byte nnscr_included[1];
  byte clear_retry_delay[1];
  byte srch_offset_incl[1];
#endif
  byte record_type[8];
  byte record_len[8];
  byte rc_sync_id_incl[1];
  byte rc_sync_id[4];
  byte blob_incl[1];
  byte blob[7];
  byte sch_incl[1];
  byte num_for_assign[2];
  byte for_sch_id[1];
  byte for_sch_duration[4];
  byte for_sch_start_time_incl[1];
  byte for_sch_start_time[5];
  byte sccl_index[4];
  byte num_rev_assign[2];
  byte rev_sch_id[1];
  byte rev_sch_duration[4];
  byte rev_sch_start_time_incl[1];
  byte rev_sch_start_time[5];
  byte rev_sch_rate[4];
  byte ch_ind[3];
  byte active_set_rec_len[8];
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte rev_fch_gating_mode[1];
  byte rev_pwr_cntl_delay_incl[1];
  byte rev_pwr_cntl_delay[2];
#endif
} cai_ho_msg_is2000_type;

typedef PACKED struct
{
  byte num_for_sch[5];
  byte for_sch_id[1];
  byte sccl_index[4];
  byte for_sch_rate[4];
  byte num_rev_sch[5];
  byte rev_sch_id[1];
  byte rev_walsh_id[1];
  byte rev_sch_rate[4];
} cai_sch_cfg_type;
#endif

typedef PACKED struct
{
  byte action_time[6];         /* Message action time */
} cai_ho_action_time_type;     /* Subordinate to use_time */

typedef PACKED struct
{
  byte srch_win_a[4];     /* Active and Candidate Set search window size */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte srch_win_n[4];     /* Search window size for the Neighbor Set */
  byte srch_win_r[4];     /* Search window size for the Remaining Set */
#endif /* defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */

  byte t_add[6];          /* Pilot detection threshold */
  byte t_drop[6];         /* Pilot drop threshold */
  byte t_comp[4];         /* Active vs Candidate set comparison threshold */
  byte t_tdrop[4];        /* Drop timer value */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte soft_slope[6];     /* Soft Slope for soft HO inequality */
  byte add_intercept[6];  /* Add Intercept for soft HO inequality */
  byte drop_intercept[6]; /* Drop Intercept for soft HO inequality */
#endif /* defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */

} cai_ho_srch_type;       /* Subordinate to srch_inc */

typedef PACKED struct
{
#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte p_rev[8];             /* Protocol Revision for new base station */
  byte packet_zone_id[8];    /* Packet Data Zone ID for new base station */
#endif /* defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */

  byte frame_offset[4];      /* Frame Offset */
  byte private_lcm[1];       /* Private long code mask indicator */
  byte reset_l2[1];          /* Reset acknowledgement procedures command */
  byte reset_fpc[1];         /* Reset forward power control command */
  byte serv_neg_type[1];     /* Service Negotiation type */
  byte encrypt_mode[2];      /* Message Encryption Mode */
  byte nom_pwr_ext[1];       /* Extended nominal transmit power indicator */
  byte nom_pwr[4];           /* Nominal transmit power */
  byte num_preamble[3];      /* Number of preamble frames to transmit */
  byte band_class[5];        /* Which band the CDMA channel freq is in */
  byte cdma_freq[11];        /* Frequency assignment of CDMA channel */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte return_if_ho_fail[1]; /* Return to old config if handoff fails */
  byte periodic_srch[1];     /* Periodic search of candidate freq flag */
  byte scr_included[1];  /* Service configuration included flag */
#endif /* defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */
} cai_ho_extra_type;        /* Subordinate to extra_parms */

typedef PACKED struct
{
  byte freq_incl[1];     /* Frequency included indicator for HDM only */
} cai_ho_freq_inc_type;

typedef PACKED struct
{
  byte add_length[3];    /* additional length indicator for EHDM only */
  byte ext_ho_p_rev[8];  /* P_REV field only for EHDM */
} cai_ho_add_length_type;

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

typedef PACKED struct
{
  byte complete_srch[1];     /* Complete candidate frequency search flag */
} cai_ho_comp_srch_type;     /* Subordinate to return_if_ho_fail */

typedef PACKED struct
{
  byte serv_con_seq[3];    /* Connect sequence number */
  /* Use existing cai_rec_hdr_type for type and length fields */
  /* Use existing cai_srv_cfg_hdr_type and
                  cai_srv_cfg_con_type for type-specific fields */
} cai_ho_svc_inc_type;     /* Subordinate to service_included */


typedef PACKED struct
{
  byte for_included[1];  /* Forward supplemental assignment included flag */
  byte rev_included[1];  /* Reverse supplemental assignment included flag */
  byte rev_parms_inc[1]; /* Reverse supplemental parms included flag */
} cai_ho_sup_chan_parms_type; /* Subordinate to sup_chan_parms_inc */

typedef PACKED struct
{
  byte for_sup_config[2];  /* Supplemental code channel config indicator */
} cai_ho_sup_for_type;     /* Subordinate to for_included */

typedef PACKED struct
{
  byte num_for_sup[3];      /* Number of fwd supplemental code channels */
  byte use_for_duration[1]; /* Use forward duration indicator */
  /* for_sup_included is part of the num_pilots stuff below */
} cai_ho_for_sup_cfg_type;  /* Subordinate to for_sup_config */

typedef PACKED struct
{
  byte for_duration[8];    /* Duration of fwd supp code channel assignment */
} cai_ho_sup_for_dur_type; /* Subordinate to use_for_duration */

typedef PACKED struct
{
  byte rev_dtx_duration[4];  /* Rev supp chan discontinuous trans duration */
  byte clr_retry_dly[1];     /* Clear retry delay for rev supp chan req */
  byte use_rev_duration[1];  /* Use reverse duration indicator */
  byte num_rev_codes[3];     /* Number of reverse supp code channels */
  byte use_t_add_abort[1];   /* Reverse use T_ADD abort indicator */
} cai_ho_sup_rev_type;       /* Subordinate to rev_included */

typedef PACKED struct
{
  byte rev_duration[8];    /* Duration of rev supp code channel assignment */
} cai_ho_sup_rev_dur_type; /* Subordinate to use_rev_duration */

typedef PACKED struct
{
  byte t_mulchan[3];        /* Supp Chan Req Msg pilot reporting offset */
  byte begin_preamble[3];   /* Num rev supp preamble frames on xmit start */
  byte resume_preamble[3];  /* Num rev supp preamble frames on xmit resume */
} cai_ho_sup_rev_parm_type; /* Subordinate to rev_parms_inc */

typedef PACKED struct
{
  byte pwr_cntl_step[3];    /* Power Control Step */
} cai_ho_pwr_cntl_type;     /* Subordinate to use_pwr_cntl_step */

#endif /* defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */

typedef PACKED struct
{
  byte pilot_pn[9];           /* Pilot PN sequence offset index */
  byte pwr_comb_ind[1];       /* Power control symbol combining indicator */
  byte code_chan[8];          /* Code channel index */
} cai_ho_msg_pilot_type;      /* Subordinate to num_pilots */

#ifdef FEATURE_IS2000
typedef PACKED struct
{
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  byte srch_offset[3];
#endif
  byte add_pilot_rec_incl[1];
  byte pilot_rec_type[3];
  byte record_len[3];
  byte code_chan_fch[11];
  byte qof_mask_id_fch[2];
  byte code_chan_dcch[11];
  byte qof_mask_id_dcch[2];
  byte num_sch[5];
} cai_ho_msg_pilot_is2000_type;

typedef PACKED struct
{
  byte for_sch_id[1];
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte for_sch_id_111[2];
#else
  byte for_sch_id_111[1];
#endif
  byte sccl_index[4];
  byte pilot_incl[1];
  byte code_chan_sch[11];
  byte qof_mask_id_sch[2];
} cai_sch_chan_rec_type;
#endif

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

typedef PACKED struct
{
  byte for_sup_inc[1];        /* Forward supp code chan included indicator */
} cai_ho_for_sup_cfg_sup_type;  /* Subordinate to for_sup_config above,
                                       but part of pilot PN record */

typedef PACKED struct
{
  byte expl_code_chan[1];       /* Explicit code channel indicator */
} cai_ho_for_sup_ch_rec_type;   /* Subordinate to for_sup_included */

typedef PACKED struct
{
  byte base_code_chan[8];       /* Base code channel index */
} cai_ho_for_sup_base_type;     /* Subordinate to expl_code_chan */

typedef PACKED struct
{
  byte sup_code_chan[8];        /* Forward supplemental code channels */
} cai_ho_for_sup_chan_type;     /* Subordinate to expl_code_chan
                                   and NUM_FOR_SUP */
#endif /* defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */

typedef PACKED struct
{
  byte reserved[7];           /* Max number of reserved bits */
} cai_ho_msg_reserved_type;

/* Minimum message must have the minimum fixed fields plus at least one
   HDM/EHDM style pilot record.  The structure cai_ho_msg_fix_type is the
   smallest fixed type between HDM, EHDM and GHDM */

#define CAI_HO_MSG_MIN_LENGTH              \
          (sizeof (cai_ho_msg_fix_type) +  \
           sizeof (cai_ho_msg_pilot_type))

#define CAI_HO_MSG_MAX_PILOTS  CAI_N6M

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* STATUS RESPONSE MESSAGE (see IS-95-A TSB)                               */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
  byte qual_typ[8];     /* Qualification info type (defined with pc version
                           of Status Request Message) */
  byte qual_len[3];     /* Qualification info length */
/*byte qual_info[?];*/  /* Qualification info (defined with pc version of
                           Status Request Message */
/*byte info_recs[?];*/  /* Information records (arbitrary number) */
  byte reserved[4];     /* More reserved bits */
} cai_tc_strsp_fix_type;

/* Supplemental assignment related definitions that are also needed for
   special MDR.  It's harmless to just define them in all cases. */
#define CAI_MAX_SUP_PILOTS  CAI_N6M /* Maximum # of pilots carrying supplementals */
#define CAI_MAX_NUM_SUP     7       /* Maximum # of supplemental channels */

/* Definitions for field FOR_SUP_CONFIG */
#define CAI_FOR_SUP_STOP       0x00
#define CAI_FOR_SUP_START      0x01
#define CAI_FOR_SUP_SPEC_STOP  0x02
#define CAI_FOR_SUP_SPEC_START 0x03

#ifdef FEATURE_IS95B
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*  SERVICE REDIRECTION MESSAGE (see IS-95B Section 7.7.3.3.2.23)          */
typedef PACKED struct
{
  byte msg_type[8];            /* Message type */
  cai_tc_hdr_type hdr;         /* Layer 2 header */
  byte return_if_fail[1];      /* Return if fail indicator */
  byte delete_tmsi[1];          /* Delete TMSI indicator */
  byte redirect_type[1];        /* Redirect indicator for IS-95B */
  byte record_type[8];          /* Redirection record type */
  byte record_len[8];           /* Redirection record length */
} cai_ftc_srv_redir_fix_type;

/* Share the type specific record type with the Paging Channel Service
   Redirection Message */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SUPPLEMENTAL CHANNEL ASSIGNMENT MESSAGE(see IS-95B Section 7.7.3.3.2.24)*/
typedef PACKED struct
{
  byte msg_type[8];            /* Message type */
  cai_tc_hdr_type hdr;         /* Layer 2 header */
  byte use_retry_delay[1];     /* Assign or retry indicator */
  byte retry_delay[8];         /* SCRM retry delay */
  byte rev_included[1];        /* Reverse Supplemental Code Channel config */
} cai_scam_fix1_type;

typedef PACKED struct
{
  byte rev_dtx_duration[4];    /* Reverse Discontinuous Transmission Duration*/
  byte expl_rev_start_tm[1];   /* Explicit reverse supp code chan assign
                                  start time */
  byte rev_start_tm[6];        /* Explicit start time */
  byte use_rev_dur[1];         /* Use reverse duration indicator */
  byte rev_dur[8];             /* Reverse duration */
  byte use_rev_hdm_seq[1];     /* Use reverse GHDM sequence # indicator */
  byte rev_hdm_seq[2];         /* Sequence # of the reverse linked GHDM */
  byte num_rev_codes[3];       /* Number of reverse sup code channels */
  byte use_t_add_abt[1];       /* Reverse use T_ADD abort indicator */
  byte use_scrm_seq[1];        /* Use SCRM sequence # indicator */
  byte scrm_seq_num[4];        /* SCRM sequence number */
  byte rev_parms_inc[1];       /* Reverse additional parameter included */
  byte t_mulchan[3];           /* SCRM pilot strength reporting offset */
  byte begin_pre[3];           /* Number of preamble frames */
  byte resume_pre[3];          /* Number of preamble at resumption of xmit */
} cai_scam_var1_type;

typedef PACKED struct
{
  byte for_inc[1];             /* FWD supp channel config included indicator */
} cai_scam_fix2_type;

typedef PACKED struct
{
  byte for_sup_config[2];      /* FWD supp channel config indicator */
  byte expl_for_start_tm[1];   /* Explicit forward start time indicator */
  byte for_start_tm[6];        /* Forward start time */
  byte use_for_dur[1];         /* Use forward duration indicator */
  byte for_dur[8];             /* Forward duration */
  byte use_for_hdm_seq[1];     /* User forward GHDM sequence # indicator */
  byte for_hdm_seq[2];         /* Sequence # of GHDM */
} cai_scam_var2_type;

typedef PACKED struct
{
  byte num_sup_plt[3];         /* Number of pilots in the active set which
                                  xmitting the sup chan */
  byte num_sup[3];             /* Number of FWD supp code channel */
} cai_scam_var3_type;

typedef PACKED struct
{
  byte plt_pn[9];              /* Pilot PN */
  byte expl_code_chan[1];      /* Explicit Code Channel indicator */
} cai_scam_var4_type;

typedef PACKED struct
{
  byte sup_code_chan[8];        /* Supplemental conde channel index */
} cai_scam_var5_type;

typedef PACKED struct
{
  byte base_code_chan[8];       /* Base code channel index */
} cai_scam_var6_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*  POWER CONTROL MESSAGE (see IS-95A Section 7.7.3.3.2.25)                */
typedef PACKED struct
{
  byte msg_type[8];            /* Message type */
  cai_tc_hdr_type hdr;         /* Layer 2 header */
  byte pwr_cntl_step[3];       /* Assign or retry indicator */
  byte reserved[4];
} cai_pwr_ctrl_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte use_time[1];
  byte action_time[6];
  byte fpc_incl[1];
  byte fpc_mode[3];
  byte fpc_pri_chan[1];
  byte fpc_olpc_fch_incl[1];
  byte fpc_fch_fer[5];
  byte fpc_fch_min_setpt[8];
  byte fpc_fch_max_setpt[8];
  byte fpc_olpc_dcch_incl[1];
  byte fpc_dcch_fer[5];
  byte fpc_dcch_min_setpt[8];
  byte fpc_dcch_max_setpt[8];
#ifdef FEATURE_IS2000_2_1_ADDENDUM
  byte fpc_olpc_sch_m_incl[1];
#endif
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte fpc_sec_chan[2];
#else
  byte fpc_sec_chan[1];
#endif
#ifdef FEATURE_IS2000_2_1_ADDENDUM
  byte fpc_sch_m_fer[5];
  byte fpc_min_sch_m_setpt[8];
  byte fpc_max_sch_m_setpt[8];
#endif
  byte num_sup[2];
  byte fpc_thresh_incl[1];
  byte fpc_setpt_thresh[8];
  byte fpc_thresh_sch_incl[1];
  byte fpc_setpt_thresh_sch[8];
  byte rpc_incl[1];
  byte rpc_num_rec[2];
} cai_pwr_ctrl_is2000_type;

typedef PACKED struct
{
  byte sch_id[1];
  byte fpc_sch_fer[5];
  /* byte fpc_sch_init_setpt[8]; */
  byte fpc_sch_min_setpt[8];
  byte fpc_sch_max_setpt[8];
} cai_fpc_sup_chn_type;

typedef PACKED struct
{
  byte rpc_adj_rec_type[4];
  byte rpc_adj_rec_len[5];
} cai_rpc_type;

typedef PACKED struct
{
  byte fch_incl[1];
  byte fch_chan_adj_gain[8];
  byte dcch_incl[1];
  byte dcch_chan_adj_gain[8];
  byte sch0_incl[1];
  byte sch0_chan_adj_gain[8];
  byte sch1_incl[1];
  byte sch1_chan_adj_gain[8];
} cai_rpc_adj0_type;

typedef PACKED struct
{
  byte rl_att_adj_gain_type[1];
  byte rs1_20ms_incl[1];
  byte rl_att_adj_gain_1500[8];
  byte rl_att_adj_gain_2700[8];
  byte rl_att_adj_gain_4800[8];
  byte rl_att_adj_gain_9600[8];
  byte rs2_20ms_incl[1];
  byte rl_att_adj_gain_1800[8];
  byte rl_att_adj_gain_3600[8];
  byte rl_att_adj_gain_7200[8];
  byte rl_att_adj_gain_14400[8];
  byte five_ms_incl[1];
  byte norm_att_gain_9600_5ms[8];
} cai_rpc_adj1_type;

typedef PACKED struct
{
  byte code_type[1];
  byte rl_att_adj_gain_type[1];
  byte rs1_20ms_incl[1];
  byte rl_att_adj_gain_19200[8];
  byte rl_att_adj_gain_38400[8];
  byte rl_att_adj_gain_76800[8];
  byte rl_att_adj_gain_153600[8];
  byte rl_att_adj_gain_307200[8];
  byte rl_att_adj_gain_614400[8];
  byte rs2_20ms_incl[1];
  byte rl_att_adj_gain_28800[8];
  byte rl_att_adj_gain_57600[8];
  byte rl_att_adj_gain_115200[8];
  byte rl_att_adj_gain_230400[8];
  byte rl_att_adj_gain_460800[8];
  byte rl_att_adj_gain_1036800[8];
} cai_rpc_adj2_type;
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* NEIGBHOR LIST UPDATE MESSAGE (see IS-95A Section 7.7.3.3.2.26)           */

/* Fixed part of message */
typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte pilot_inc[4];     /* Pilot PN sequence offset index increment */
  byte nghbr_srch_mode[2];  /* Neighbor Search Mode */
  byte srch_win_n[4];    /* Default window size for neighbor set */
  byte use_timing[1];    /* Use timing indicator */
  byte global_timing_incl[1]; /* Global Timing Included */
  byte global_tx_duration[4]; /* Global neighbor transmit time duration */
  byte global_tx_period[7]; /* Global neighbor transmit time period */
  byte num_nghbr[6];     /* Number of neighbor */
} cai_ext_nlu_fix_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte srch_offset_incl[1];
} cai_ext_nlu_fix_is2000_type;
#endif

/* Variable part of message */
typedef PACKED struct
{
  byte nghbr_pn[9];      /* Neighbor pilot PN sequence offset index */
  byte search_pri[2];      /* Search Piority */
  byte srch_win_nghbr[4];/* Per neighbor search window */
  byte timing_incl[1];   /* Timing Inlcuded Indicator */
  byte nghbr_tx_offset[7];  /* Neighbor transmit time offset */
  byte nghbr_tx_duration[4];  /* Global neighbor transmit time duration */
  byte nghbr_tx_period[7];  /* Global neighbor transmit time period */
} cai_ext_nlu_var_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte add_pilot_rec_incl[1];
  byte nghbr_pilot_rec_type[3];
  byte record_len[3];
  byte srch_offset_nghbr[3];
} cai_ext_nlu_var_is2000_type;
#endif

/* Maximum number of neighbors in message */
#define CAI_EXT_NLU_MAX  40  /* Neighbor set maximum */

typedef PACKED struct
{
  cai_ext_nlu_fix_type fix;
  cai_ext_nlu_var_type var[CAI_EXT_NLU_MAX];
} cai_ext_nlu_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*  CANDIDATE FREQUENCY SEARCH REQUEST MESSAGE                             */
/*      (see IS-95B Section 7.7.3.3.2.)                                    */
#define CAI_CFSREQ_REC_MAX   40
  /* Maximum Number of pilots included in Candidate Frequency Search
   * Request Message --- Max Value of num_pilot  */

#define CAI_CF_SRCH_TYPE_CDMA   0
#define CAI_CF_SRCH_TYPE_ANALOG 1

typedef PACKED struct
{
  byte msg_type[8];            /* Message type */
  cai_tc_hdr_type hdr;         /* Layer 2 header */
  byte use_time[1];            /* Use action time indicator */
  byte action_time[6];         /* Action time */
  byte reserved[4];
  byte cfsrm_seq[2];           /* CFSR message sequence number */
  byte srch_type[2];           /* Search Type Code */
  byte srch_period[4];         /* Search Period    */
  byte srch_mode[4];           /* Search Mode Type */
  byte mod_sp_len[8];          /* Mode specific lenghth */
} cai_cfs_req_fix_type;

typedef PACKED struct
{
  byte band_class[5];           /* Band Class     */
  byte cdma_freq[11];           /* CDMA Frequence Assignment */
  byte sf_total_ec_thresh[5];   /* Serving Frequency total pilot Ec threshold */
  byte sf_total_ec_io_thresh[5];/* Serving Frequency total pilot Ec/Io thresh.*/
  byte diff_rx_pwr_thresh[5];   /* Minimum Difference in received power */
  byte min_total_pilot_ecio[5]; /* Minimum Total Pilot Ec/Io */
  byte cf_t_add[6];             /* Pilot Detection Threshold for the CDMA
                                 * Candidate Frequency                      */
  byte tf_wait_time[4];         /* CDMA Candidate Frequence Total Wait Time */
  byte cf_pilot_inc[4];         /* Pilot PN sequence offset index increment
                                 * to be used on the CDMA Candidate Frequency
                                 * after handoff                             */
  byte cf_srch_win_n[4];        /* Default Search Window Size for the
                                 * Candidate Frequency Search Set            */
  byte cf_srch_win_r[4];        /* Search Window Size for the Remaining Set  */
  byte reserved[5];
  byte pilot_update[1];         /* Pilot Search Parameter update Indicator   */
  byte num_pilot[6];            /* Number of Pilots included in the Message  */
  byte cf_nghbr_srch_mode[2];   /* Search mode for candidate frequency search
                                 * set                                       */
} cai_cfs_mod0_fix_type;

typedef PACKED struct
{
  byte nghbr_pn[9];              /* Offset of neighbor pilot pn */
  byte search_set[1];            /* Pilot Search Indicator      */
  byte search_priority[2];       /* Pilot Channel Search priority */
  byte srch_win_nghbr[4];        /* Beighbor pilot channel search window size */
} cai_cfs_mod0_var_type;

typedef PACKED struct
{
  byte band_class[5];            /* Band Class     */
  byte sf_total_ec_thresh[5];    /* Serving Frequency total pilot Ec threshold */
  byte sf_total_ec_io_thresh[5]; /* Serving Frequency total pilot Ec/Io thresh.*/
  byte reserved[6];
  byte num_analog_freqs[3];      /* Number of analog freq */
} cai_cfs_mod1_fix_type;

#define CAI_CFS_ANALOG_MAX  7

typedef PACKED struct
{
  byte analog_freq[11];         /* Analog frequency */
} cai_cfs_mod1_var_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte cf_srch_offset_incl[1];
} cai_cfs_req_is2000_type;

typedef PACKED struct
{
  byte add_pilot_rec_incl[1];
} cai_cfs_req_is2000_rec_fix_type;

typedef PACKED struct
{
  byte nghbr_pilot_rec_type[3];
  byte record_len[3];
  byte srch_offset_nghbr[3];
} cai_cfs_req_is2000_rec_var_type;

typedef PACKED struct
{
  byte align_timing[1];      /* Align Timing */
  byte search_offset[6];     /* search offset time based on the align timing */
} cai_cfs_req_type;

#endif


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* CANDIDATE FREQUENCY SEARCH CONTROL MESSAGE(see IS-95B Sec 7.7.3.3.2.28) */
typedef PACKED struct
{
  byte msg_type[8];            /* Message type */
  cai_tc_hdr_type hdr;         /* Layer 2 header */
  byte use_time[1];            /* Use action time indicator */
  byte action_time[6];         /* Action time */
  byte cfscm_seq[2];           /* CFSR message sequence number */
  byte srch_type[2];           /* Search Type Code */
  byte reserved[4];
} cai_cfs_ctrl_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* POWER UP FUNCTION MESSAGE (see IS-95B Section 7.7.3.3.2.29)     */
typedef PACKED struct
{
  byte msg_type[8];            /* Message type */
  cai_tc_hdr_type hdr;         /* Layer 2 header */
  byte use_time[1];            /* Use action time indicator */
  byte action_time[6];         /* Action time */
  byte act_tm_frm[2];          /* Action time frame */
  byte puf_setup_sz[6];        /* Puf setup size */
  byte puf_pulse_sz[7];        /* Puf pulse size */
  byte puf_interval[10];       /* Puf interval */
  byte puf_init_pwr[6];        /* Power increase of initial pulse */
  byte puf_pwr_step[5];        /* PUF power step */
  byte tot_puf_prob[4];        /* Total number of PUF probes */
  byte max_pwr_puf[4];         /* Max # of probe at full power */
  byte puf_freq_incl[1];       /* Frequency included indicator */
  byte band_class[5];          /* PUF band class */
  byte cdma_freq[11];          /* PUF CDMA frequency */
  byte reserved[3];
} cai_puf_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* POWER UP FUNCTION COMPLETION MESSAGE (see IS-95B Section 7.7.3.3.2.30)  */
typedef PACKED struct
{
  byte msg_type[8];            /* Message type */
  cai_tc_hdr_type hdr;         /* Layer 2 header */
  byte reserved[6];
  byte loc_ind[1];             /* Location indicator */
  byte reserved1[3];
  byte ms_lat[22];             /* Mobile station latitude */
  byte ms_long[23];            /* Mobile station longitude */
  byte tm_stamp[24];           /* Time stamp */
} cai_puf_comp_type;

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte start_time_unit[3];
  byte rev_sch_dtx_duration[4];
  byte use_t_add_abort[1];
  byte use_scrm_seq_num[1];
  byte scrm_seq_num[4];
  byte add_info_incl[1];
  byte fpc_pri_chan[1];
  byte rev_cfg_included[1];
  byte num_rev_cfg_recs[5];
  byte rev_sch_id[1];
  byte rev_walsh_id[1];
  byte rev_sch_rate[4];
  byte num_rev_sch[2];
  byte rev_sch_id_2[1];
  byte rev_sch_duration[4];
  byte rev_sch_start_time_incl[1];
  byte rev_sch_start_time[5];
  byte rev_sch_rate_2[4];
  byte num_for_sch_cfg[2];
  byte for_sch_id[1];
  byte num_rec[5];
  byte sccl_index[4];
  byte for_sch_rate[4];
  byte num_sup_sho[3];
  byte pilot_pn[9];
  byte add_pilot_rec_incl[1];
  byte active_pilot_rec_type[3];
  byte record_len[3];
  byte for_sch_cc_index[11];
  byte qof_mask_id_sch[2];
  byte num_for_sch[2];
  byte for_sch_id_2[1];
  byte for_sch_duration[4];
  byte for_sch_start_time_incl[1];
  byte for_sch_start_time[5];
  byte for_cfg_included[1];
  byte for_sch_fer_rep[1];
  byte num_for_cfg_recs[5];
  byte sccl_index_2[4];
  byte fpc_incl[1];
  byte fpc_mode_sch[3];
  byte fpc_sch_init_setpt_op[1];
  byte fpc_olpc_sch_m_incl[1];
  byte fpc_sec_chan[1];
  byte fpc_sch_m_fer[5];
  byte fpc_sch_init_m_setpt[8];
  byte fpc_sch_min_m_setpt[8];
  byte fpc_sch_max_m_setpt[8];
  byte num_sup[2];
  byte sch_id[1];
  byte fpc_sch_fer[5];
  byte fpc_sch_init_setpt[8];
  byte fpc_sch_min_setpt[8];
  byte fpc_sch_max_setpt[8];
  byte fpc_thresh_sch_incl[1];
  byte fpc_setpt_thresh_sch[8];
  byte rpc_incl[1];
  byte rlgain_sch_pilot[6];
  byte rpc_num_sup[1];
} cai_escam_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* REVERSE TRAFFIC CHANNEL ORDER MESSAGE (see IS-95A Section 6.7.2.3.2.1)  */

/* Generic Order message */
typedef PACKED struct
{
  byte msg_type[8];       /* Message type */
  cai_tc_hdr_type hdr;    /* Layer 2 header */
  byte order[6];          /* Order field */
  byte add_record_len[3]; /* Additional Record Length */
} cai_rtc_gen_ord_type;

/* Order with order qualifier */
typedef PACKED struct
{
  byte msg_type[8];       /* Message type */
  cai_tc_hdr_type hdr;    /* Layer 2 header */
  byte order[6];          /* Order field */
  byte add_record_len[3]; /* Additional Record Length */
  byte ordq[8];           /* Order qualifier */
} cai_rtc_ordq_type;

/* Base Station Challenge Order (see IS-95A Section 6.7.3.1) */
typedef PACKED struct
{
  byte msg_type[8];       /* Message type */
  cai_tc_hdr_type hdr;    /* Layer 2 header */
  byte order[6];          /* Order field */
  byte add_record_len[3]; /* Additional Record Length */
  byte ordq[8];           /* Order qualifier */
  byte randbs[32];        /* Random Challenge data */
} cai_rtc_bs_chal_type;

/* Service Option Request Order and Service Option Response Order
   (see IS-95A Sections 6.7.3.2 and 6.7.3.3) */
typedef PACKED struct
{
  byte msg_type[8];         /* Message type */
  cai_tc_hdr_type hdr;      /* Layer 2 header */
  byte order[6];            /* Order field */
  byte add_record_len[3];   /* Additional Record Length */
  byte ordq[8];             /* Order qualifier */
  byte service_option[16];  /* Service option */
} cai_rtc_so_type;

/* Mobile Station Reject Order (see IS-95A Section 6.7.3.4) */

/* Fixed part of Mobile Station Reject Order */
typedef PACKED struct
{
  byte msg_type[8];       /* Message type */
  cai_tc_hdr_type hdr;    /* Layer 2 header */
  byte order[6];          /* Order field */
  byte add_record_len[3]; /* Additional Record Length */
  byte ordq[8];           /* Order qualifier */
  byte rej_msg_type[8];   /* Message type of rejected message */
} cai_rtc_rej_fixed_type;

/* Only present if the rejected message was an Order Message */
typedef PACKED struct
{
  byte rej_order[8];  /* Order type of rejected message */
  byte rej_ordq[8];   /* Order qualification code of rejected message */
} cai_rtc_rej_order_type;

/* Only present if the rejected message was a Retrieve Parameters Message
   or a Set Parameters Message */
typedef PACKED struct
{
  byte rej_param_id[16];  /* Parameter id of the rejected parameter */
} cai_rtc_rej_param_type;

/* Only present if the rejected message was an  Alert with Information
   Message or a Flash with Information Message */
typedef PACKED struct
{
  byte rej_record[8];  /* Record type of the rejected information record */
} cai_rtc_rej_record_type;

typedef PACKED struct
{
  cai_rtc_rej_fixed_type fix;
  PACKED union {
    cai_rtc_rej_order_type  order;
    cai_rtc_rej_param_type  param;
    cai_rtc_rej_record_type rec;
  } var;
} cai_rtc_rej_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte rejected_pdu_type[2];
  byte reserved[6];
} cai_rtc_rej_ord_is2000_type;
#endif

/* Reverse Traffic Channel Order Message */
typedef union
{
  cai_rtc_gen_ord_type  gen;
  cai_rtc_ordq_type     ordq;
  cai_rtc_bs_chal_type  chal;
  cai_rtc_so_type       so;
  cai_rtc_rej_type      rej;
} cai_rtc_ord_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* AUTHENTICATION CHALLENGE RESPONSE MSG (see IS-95A Section 6.7.2.3.2.2)  */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
  byte authu[18];       /* Authentication challenge response */
} cai_auth_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* FLASH WITH INFORMATION MESSAGE (see IS-95A Section 6.7.2.3.2.3)         */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
} cai_flash_fix_type;


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* TMSI ASSIGNMENT C0MPLETION MESSAGE (see J-STD-008 Section 2.7.2.3.2.17) */

/* TMSI Assignment Completion Message - Reverse Traffic Chn */
typedef PACKED struct
{
  byte msg_type[8];        /* Message type */
  cai_tc_hdr_type hdr;     /* Layer 2 header */
  byte reserved[7];        /* Reserved             */
} cai_rtc_tmsi_cmp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DATA BURST MESSAGE (see IS-95A Section 6.7.2.3.2.4)                     */

/* Use cai_tc_burst_fix_type and cai_burst_tc_var_type from Forward
   Traffic Channel Message recordtype definitions above */

/* Maximum number of variable parts of message */
#define CAI_REV_TC_BURST_MAX  TC_REV_MAX( cai_burst_tc )

typedef PACKED struct
{
  cai_burst_tc_fix_type fix;
  cai_burst_tc_var_type var[CAI_REV_TC_BURST_MAX];
} cai_rev_tc_burst_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PILOT STRENGTH MEASUREMENT MESSAGE (see IS-95A Section 6.7.2.3.2.5)     */

typedef PACKED struct
{
  byte msg_type[8];           /* Message type */
  cai_tc_hdr_type hdr;        /* Layer 2 header */
  byte ref_pn[9];             /* Time reference PN sequence offset */
  byte pilot_strength[6];     /* Pilot strength */
  byte keep[1];               /* Keep pilot indicator */
} cai_pil_str_fix_type;

typedef PACKED struct
{
  byte pilot_pn_phase[15]; /* Pilot measured phase */
  byte pilot_strength[6];  /* Pilot strength */
  byte keep[1];            /* Keep pilot indicator */
} cai_pil_str_var_type;

/* Maximum number of variable parts of message */
#define CAI_PIL_STR_MAX TC_REV_MAX( cai_pil_str )

typedef PACKED struct
{
  cai_pil_str_fix_type fix;
  cai_pil_str_var_type var[CAI_PIL_STR_MAX];
} cai_pil_str_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*               POWER MEASUREMENT REPORT                                  */
/* (See IS-95A Section 6.7.2.3.2.6 Power Measurement Report Message)       */

typedef PACKED struct
{
  byte msg_type[8];          /* Message type */
  cai_tc_hdr_type hdr;       /* Layer 2 header */
  byte errors_detected[5];   /* Number of frame errors detected */
  byte pwr_meas_frames[10];  /* Number of Forward Traffic Channel frames
                                in the measurement period */
  byte last_hdm_seq[2];      /* Handoff Direction Message sequence number */
  byte num_pilots[4];        /* number of pilots */
} cai_pwr_rpt_fix_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte dcch_pwr_meas_incl[1];
  byte dcch_pwr_meas_frames[10];
  byte dcch_errors_detected[5];
  byte sch_pwr_meas_incl[1];
  byte sch_id[1];
  byte sch_pwr_meas_frames[16];
  byte sch_errors_detected[10];
} cai_pwr_rpt_2000_type;
#endif

typedef PACKED struct
{
  byte pilot_strength[6];         /* pilot strength */
} cai_pwr_rpt_var_type;

#define CAI_PWR_MAX 15
  /* Max number of pilot_strength records is constrained by size
     of the num_pilots field */

typedef PACKED struct
{
  cai_pwr_rpt_fix_type fix;
  cai_pwr_rpt_var_type var[CAI_PWR_MAX];
} cai_pwr_rpt_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SEND BURST DTMF MESSAGE (see IS-95A Section 6.7.2.3.2.7)                */

/* Use cai_sbdtmf_type from Forward Traffic Channel Message record type
   definitions above */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* STATUS MESSAGE (see IS-95A Section 6.7.2.3.2.8)                         */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
  byte record_type[8];  /* Information record type */
  byte record_len[8];   /* Information record length */
} cai_status_fix_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ORIGINATION CONTINUATION MESSAGE (see IS-95A Section 6.7.2.3.2.9)       */

typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte digit_mode[1];    /* Digit mode indicator */
  byte num_fields[8];    /* Number of dialed digits in this message */
} cai_orig_c_fix_type;

typedef PACKED union
{
  byte digit4[4];  /* 4 bit representation of the digit */
  byte digit8[8];  /* ASCII representation of the digit */
} cai_orig_c_var_type;

#define CAI_ORIG_C_MAX  NV_MAX_DIAL_DIGITS

typedef PACKED struct
{
  cai_orig_c_fix_type fix;
  cai_orig_c_var_type var;
} cai_orig_c_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* HANDOFF COMPLETION MESSAGE (see IS-95A Section 6.7.2.3.2.10)            */

typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte last_hdm_seq[2];  /* Handoff Direction Message Sequence Number */
} cai_comp_fix_type;

typedef PACKED struct
{
  byte pilot_pn[9];      /* Pilot PN sequence offset */
} cai_comp_var_type;

#define CAI_MAX_COMP TC_REV_MAX( cai_comp )

typedef PACKED struct
{
  cai_comp_fix_type fix;
  cai_comp_var_type var[CAI_MAX_COMP];
} cai_comp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PARAMETERS RESPONSE MESSAGE (see IS-95A Section 6.7.2.3.2.11)           */

typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
} cai_parm_rsp_fix_type;

typedef PACKED struct
{
  byte parameter_id[16];   /* Identification for parameter */
  byte parameter_len[10];  /* length in bits of returned parameter */
  byte parameter[1];       /* First bit of parameter */
} cai_parm_rsp_var_type;

/* Maximum number of variable length message parts */
#define CAI_PARM_RSP_MAX ((CAI_REV_TC_MSG_BODY_SIZE -                       \
                          sizeof( cai_parm_rsp_fix_type )) /                \
                            (FSIZ( cai_parm_rsp_var_type, parameter_id ) +  \
                             FSIZ( cai_parm_rsp_var_type, parameter_len ) + \
                             CAI_PARM_MIN_LEN))

typedef PACKED struct
{
  cai_parm_rsp_fix_type fix;
  cai_parm_rsp_var_type var;
} cai_parm_rsp_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE REQUEST MESSAGE (see IS-95-A TSB)                               */

/* Fixed portion of Service Request Message body */
typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte req_seq[3];       /* Request sequence number */
  byte purp[4];          /* Request purpose */
} cai_rtc_srv_req_fix_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte blob_incl[1];
  byte blob[7];
} cai_rtc_srv_req_is2000_type;
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE RESPONSE MESSAGE (see IS-95-A TSB)                              */

/* Fixed portion of Service Response Message body */
typedef PACKED struct
{
  byte msg_type[8];     /* Message type */
  cai_tc_hdr_type hdr;  /* Layer 2 header */
  byte req_seq[3];      /* Request sequence number */
  byte purp[4];         /* Response purpose */
} cai_rtc_srv_rsp_fix_type;


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE CONNECT COMPLETION MESSAGE (see IS-95-A TSB)                    */

typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte reserved1[1];     /* Reserved bits */
  byte con_seq[3];       /* Connection sequence number */
  byte reserved[3];      /* Reserved bits */
} cai_srv_cmp_type;


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE OPTION CONTROL MESSAGE (see IS-95-A TSB)                        */

/* Fixed portion of Service Option Control Message body */
typedef PACKED struct
{
  byte msg_type[8];      /* Message type */
  cai_tc_hdr_type hdr;   /* Layer 2 header */
  byte con_ref[8];       /* Request sequence number */
  byte so[16];           /* Service option */
  byte reserved[7];      /* Reserved bits */
  byte tsf_len[8];       /* Control record length */
} cai_rtc_srv_ctl_fix_type;

/* Variable portion of Service Option Control Message body */
typedef PACKED struct
{
  byte  tsf[8];  /* Type specific field */
} cai_rtc_srv_ctl_var_type;

#define  CAI_RTC_SRV_CTL_MAX  TC_REV_MAX ( cai_rtc_srv_ctl )
  /* Maximum number of octets for type specific field */

/* Complete Service Option Control Message body */
typedef PACKED struct
{
  cai_rtc_srv_ctl_fix_type  fix;                        /* Fixed part */
  cai_rtc_srv_ctl_var_type  var[ CAI_RTC_SRV_CTL_MAX ]; /* Variable part */
} cai_rtc_srv_ctl_type;

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

#ifdef FEATURE_IS95B
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SUPPLEMENTAL CHANNEL REQUEST MESSAGE (see IS-95B Section 6.7.2.3.2.18)  */

#define CAI_MAX_SCRM_BLOB_SIZE  15 /* 4 bit value */
#define CAI_MAX_SCRM_ACT_PN      5 /* 3 bit value */
#define CAI_MAX_SCRM_NGHBR_PN    7 /* 3 bit value */
#define CAI_MAX_SCRM_RPT_PN      7 /* 3 bit value */

typedef PACKED struct
{
  byte msg_type[8];           /* Message type */
  cai_tc_hdr_type hdr;        /* Layer 2 header */
  byte sz_req_blob[4];        /* Size of the request info block of bytes */
  byte use_scrm_seq_num[1];   /* Use SCRM sequence number indicator */
  byte scrm_seq_num[4];       /* SCRM sequence number */
  byte ref_pn[9];             /* Time reference PN offset */
  byte pilot_str[6];          /* Pilot strength */
  byte num_act_pn[3];         /* Number of pn in the active set */
  byte num_nghbr_pn[3];       /* Number of neigbor set pn */

} cai_scrm_fix_type;

typedef PACKED struct
{
  byte req_blob_data[8];      /* REQ_BLOB */
} cai_scrm_var1_type;

typedef PACKED struct
{
  byte act_pn_phase[15];      /* PN phase */
  byte act_pn_str[6];         /* Strength of the PN */
} cai_scrm_var2_type;

typedef PACKED struct
{
  byte nghbr_pn_phase[15];      /* PN phase */
  byte nghbr_pn_str[6];         /* Strength of the PN */
} cai_scrm_var3_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* CANDIDATE FREQUENCY SEARCH RESPONSE MESSAGE(see IS-95B Sec 6.7.2.3.2.19)*/
typedef PACKED struct
{
  byte msg_type[8];              /* Message type */
  cai_tc_hdr_type hdr;           /* Layer 2 header */
  byte last_cfsrm_seq[2];        /* CFSRM sequence number */
  byte tot_off_tm_fwd[6];        /* Total time the mobile is off the FTC */
  byte max_off_tm_fwd[6];        /* Max time the mobile is off the FTC */
  byte tot_off_tm_rev[6];        /* Total time the mobile is off the RTC */
  byte max_off_tm_rev[6];        /* Max time the mobile is off the RTC */
  byte reserved[5];
} cai_cfs_rsp_type;

#ifdef FEATURE_IS2000
typedef PACKED struct
{
  byte pcg_off_times[1]; /* indicator if off times are expressed */
  byte align_timing_used[1];  /*alignment timing used indicator */
  byte num_visits[5];  /* number of visits per search period */
  byte inter_visit_time[6];  /* inter-visit time */
} cai_cfs_rsp_is2000_type;
#endif /* FEATURE_IS2000 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* CANDIDATE FREQUENCY SEARCH REPORT MESSAGE (see IS-95B Sec 6.7.2.3.2.20) */
typedef PACKED struct
{
  byte msg_type[8];              /* Message type */
  cai_tc_hdr_type hdr;           /* Layer 2 header */
  byte last_srch_msg[1];         /* Type of msg started the search */
  byte last_srch_seq[2];         /* Seq # in the msg started the search */
  byte srch_mode[4];             /* Search mode */
  byte mode_sp_len[8];           /* Mode specific fields length */
} cai_cfs_rpt_fix_type;

typedef PACKED struct
{
  byte band_class[5];            /* Band class */
  byte cdma_freq[11];            /* CDMA frequency */
  byte sf_tot_rx_pwr[5];         /* Total received pwr on the serving freq */
  byte cf_tot_rx_pwr[5];         /* Total received pwr on the candidate freq*/
  byte num_pilots[6];            /* Number of pilots */
} cai_cfs_rpt_mod0_fix_type;

#define CAI_MAX_CFS_RPT_PLTS   31  /* Max number of pilots */

typedef PACKED struct
{
  byte plt_pn_phase[15];         /* Pilot PN phase */
  byte plt_str[6];               /* Pilot strength */
  byte reserved[3];
} cai_cfs_rpt_mod0_var_type;

typedef PACKED struct
{
  byte band_class[5];            /* Band Class */
  byte sf_tot_rx_pwr[5];         /* Total receiving power on the serving freq */
  byte num_analog_freqs[3];      /* Number of analog frequencies */
  byte reserved[5];
} cai_cfs_rpt_mod1_fix_type;

#define CAI_MAX_ANALOG_FREQS  7    /* Max number of analog frequencies */

typedef PACKED struct
{
  byte analog_freq[11];          /* Analog frequency */
  byte sig_str[6];               /* Signal strength */
} cai_cfs_rpt_mod1_var_type;


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* PERIODIC PILOT STRENGTH MEASUREMENT MESSAGE(see IS-95B Sec 6.7.2.3.2.21)*/
typedef PACKED struct
{
  byte msg_type[8];              /* Message type */
  cai_tc_hdr_type hdr;           /* Layer 2 header */
  byte ref_pn[9];                /* Time reference PN seq offset */
  byte pilot_str[6];             /* Pilot strength */
  byte keep[1];                  /* Keep pilot indicator */
  byte sf_rx_pwr[5];             /* Serving Freq received power density */
  byte num_pilot[4];             /* Number of pilots */
} cai_plt_str_meas_fix_type;

#define  CAI_PLT_STR_MEAS_MAX  15  /* Max number of pilots in this msg */

typedef PACKED struct
{
  byte plt_pn_phase[15];         /* Pilot PN phase */
  byte plt_str[6];               /* Pilot strength */
  byte keep[1];                  /* keep pilot indicator */
} cai_plt_str_meas_var_type;

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* OUTER LOOP REPORT MESSAGE (see IS-2000 Sec 2.7.2.3.2.22)                */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
typedef struct
{
  byte msg_type[8];              /* Message type */
  cai_tc_hdr_type hdr;           /* Layer 2 header */
  byte fch_incl[1];
  byte fpc_fch_curr_setpt[8];
  byte dcch_incl[1];
  byte fpc_dcch_curr_setpt[8];
  byte num_sup[2];
  byte sch_id[1];
  byte fpc_sch_curr_setpt[8];
} cai_outer_rpt_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* The following data types are used by the Transmit and Receive tasks for
   message storage */

/* Sync Channel Message type */
typedef struct
{
  byte sframe_num;             /* Subframe for message */
  byte length;                 /* Length byte */
  byte body[CAI_SC_EXT_SIZE];  /* Message body -- including CRC */
} cai_sc_ext_type;

/* Paging Channel Message type */
typedef struct
{
  qword frame_num;            /* Frame number */
  byte length;                /* Message length */
  byte body[CAI_PC_EXT_SIZE]; /* Message body including CRC */
} cai_pc_ext_type;

/* Access Channel Message type */
typedef struct
{
  qword frame_num;              /* Frame number */
  byte length;                  /* Message Length */
  byte body[CAI_AC_EXT_SIZE];   /* Message body including CRC */
} cai_ac_ext_type;

/* Forward Traffic Channel Message type */
typedef struct
{
  qword frame_num;                   /* Frame number */
  byte length;                       /* Message length */
  byte body[CAI_FWD_TC_EXT_SIZE];    /* Message body including CRC */
} cai_fwd_tc_ext_type;

/* Reverse Traffic Channel Message type */
typedef struct
{
  qword frame_num;                   /* Frame number */
  byte length;                       /* Message length */
  byte body[CAI_REV_TC_EXT_SIZE];    /* Message body including CRC */
} cai_rev_tc_ext_type;

/* Union of Receive types */
typedef union
{
  cai_sc_ext_type     sc;
  cai_pc_ext_type     pc;
  cai_fwd_tc_ext_type tc;
} cai_rx_msg_type;

/* Union of transmit types */

typedef union
{
  cai_ac_ext_type     ac;
  cai_rev_tc_ext_type tc;
} cai_tx_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* The following data types are used by event report                       */

/* Channel ID in which message is received */

typedef enum
{
  CAI_SYNCH,
  CAI_PCH,
  CAI_BCCH,
  CAI_F_CCCH,
  CAI_F_CACH,
  CAI_F_FCH,
  CAI_F_DCCH
} cai_rx_channel_id_type;

/* Channel ID in which message is transmitted */
typedef enum
{
  CAI_ACH,
  CAI_EACH,
  CAI_R_CCCH,
  CAI_R_FCH,
  CAI_R_DCCH
} cai_tx_channel_id_type;

/* Timers ID */
typedef enum
{
  CAI_TIMER_T1M,
  CAI_TIMER_T75M,
  CAI_TIMER_T5M,
  CAI_TIMER_T20M,
  CAI_TIMER_T21M,
  CAI_TIMER_T30M,
  CAI_TIMER_T72M,
  CAI_TIMER_T40M,
  CAI_TIMER_T41M,
  CAI_TIMER_T42M,
  CAI_TIMER_T50M,
  CAI_TIMER_T51M,
  CAI_TIMER_T52M,
  CAI_TIMER_T53M,
  CAI_TIMER_T55M,
  CAI_TIMER_T57M,
  CAI_TIMER_T64M,
  CAI_TIMER_T65M,
  CAI_TIMER_T68M
} cai_timer_id_type;

/* Counters ID */
typedef enum
{
  CAI_COUNTER_N1M,
  CAI_COUNTER_N14M,
  CAI_COUNTER_N15M,
  CAI_COUNTER_N2M,
  CAI_COUNTER_N3M,
  CAI_COUNTER_N5M,
  CAI_COUNTER_N11M
} cai_counter_id_type;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

#endif /* CAI_H */

