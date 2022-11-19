#ifndef MCCSRCH_H
#define MCCSRCH_H
/*===========================================================================

                M C C S R C H   H E A D E R    F I L E

DESCRIPTION
  This file contains all of the definitions necessary to interface between
  the Main Control CDMA subtask and the Searcher task.

Copyright (c) 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header:   O:/src/asw/COMMON/vcs/mccsrch.h_v   1.5   02 May 2001 19:48:52   fchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/02/01   fc      Removed SRCH_T_ADD_ABORT_R.
04/23/01   fc      Added SRCH_T_ADD_ABORT_R to support T_ADD_ABORT.
02/26/01   mai     Changed FEATURE_GPSONE to FEATURE_GPS_MODE to move code needed
                   only for GPS part of position location to a new #define.
02/16/01   fc      Added support for system determination 2.0. All changes 
                   are featurized under FEATURE_SD20.
09/08/00   jq      Added QPCH support in sleep state.
07/06/00   yll     Added GPS support
06/20/00   fc      Added SRCH_SCRM_MEAS_R and mccsrch_scrm_meas_rpt_type to
                   support SCRM pilot strength measurement.
05/26/00   jq      Added cur_slot_mask into mccsrch_wakeup_rpt_type
03/31/00   ry      Featurized mccsrch_get_aset() under FEATURE_IS2000
03/31/00   ry      Removed FEATURE_SRCH_DYNAMIC_BAND_CLASS featurization since
                   the feature will always be on
03/29/00   ry      Featurized the nbr_band parameter of mcc_srch_nbr_checking()
                   under FEATURE_SRCH_DYNAMIC_BAND_CLASS
           ry      Added the prototype mccsrch_get_aset() - it was deleted in
                   the previous merge
03/13/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           ks      Removed the feature ENHANCED_STANDBY_II. Also added 
                   mcc_srch_min_bs_p_rev function for ENHANCED_STANDBY_III.
           ck      Added the band class information to some SRCH reports and
                   mcc_srch_nbr_checking function.
02/22/00   jcw     Merged in the following change:
           cah     (ajn)Added a vaild time field for Wakeup Report.
11/15/99   va      IS2000 Changes:
                   Support for "SRCH_AT_R" to send a report when HO action time
                   has arrived. 
08/24/99   fc      Added num_pilots to mccsrch_pilots_meas_rpt_type to allow
                   SRCH providing the count of pilots in Pilot Strength 
                   Measurement report.
08/09/99   ry      Combined FEATURE_IS95B_HARD_HO and FEATURE_IS95B_CFS into
                   FEATURE_IS95B_MAHHO
           ry      Added struct definitions for SRCH_CFS_READY_R and
                   SRCH_CF_MEAS_DONE_R
           ry      Added CFS sequence number to mccsrch_cfnset_meas_rpt_type
07/09/99   ry      Added the SRCH_RIF_HHO_FAILURE_R, SRCH_RIF_HHO_ABORT_R, and
                   SRCH_CF_NSET_R reports
07/08/99   ych     Added support for processing Periodic Pilot Measurment
                   Message
05/14/99   kmp     Merged in the changes from the IS-95B Odie baseline
           kmp     Merged the changes listed below from the TGP IS-95B baseline.
           fc      Renamed all typedef names defined for pilot reporting to 
                   end with _type. Renamed all typedef names defined for
                   pilot reporting which started with ACC_HO or ACCESS_HO to 
                   AHO. 
           kmp     Merged the changes listed below from the TGP IS-95B baseline.
           fc      Put in changes based on code review input.
           fc      Added ACCESS_HO_ALLOWED and ACCESS_ATTEMPTED in
                   MCCSRCH_PILOTS_MEAS_RPT_TYPE to support pilot reporting
                   performance improvement.
           kmp     merged in IS-95B changes, ported IS-95B changes to ARM,
           fc      Changed the size of the Pilot Strength Measurement report.
           fc      Updated comments in mccsrch_report_code_type and 
                   mccsrch_pilots_meas_rpt_type for pilot reporting. 
           ks      Updated mccsrch_pilots_meas_rpt_type for pilot reporting.
           lh      FEATURE_JSTD008 replaced with run-time P_REV and Band 
                   Class checking.
           fc      Added SRCH_AHO_PLIST_R report type and 
                   MCCSRCH_PILOTS_MEAS_RPT_TYPE structure for reporting
                   the Nset pilot strength to MC.
           ks      Moved the SRCH_THRESH_R before the SRCH_MAX_R report.
           ks      Added SRCH_THRESH_R report for reporting to MC.
02/26/99   ejv     Added ajn's timestamp SRCH_TRYSLEEP_R message.  Also merged
                   FEATURE_ANTIREGISTRATION_BIAS function prototypes.
09/14/98   ck      Added new report code type SRCH_TRYSLEEP_R  
09/04/98   pms     Neared the variables to save ROM space.
06/27/97   rdh     Added SRCH_FING_DONE_R status reporting to MC.
05/15/96   dna     Added freq and band class fields for PCS
06/28/95   rdh     Added rude awakening status field to wakeup report.
04/28/95   rdh     Added slot_cycle_index field to WAKEUP_R command field.
03/30/95   rdh     Eliminated slam bug catch fields. Fixed in Search task.
03/23/95   rdh     Added SRCH_WAKEUP_R for deep sleep.
11/01/94   rdh     Added PC_ASSIGN command structure gotten from Don.
10/24/94   rdh     Added status field to slew command for 9600 baud paging
                   channel MSM bug workaround.
02/19/94   jah     Deleted unused commands awake SRCH_WAKEUP_R & SRCH_AWAKE_R,
                   and the associated union members.  Changed RAHO pilot to
                   an int2 to match others.
07/16/92   ptw     Created file.

===========================================================================*/

#include "srch.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                      SEARCHER TASK REPORT TYPES

--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                           SEARCH REPORT CODES

The following codes are for use with the 'rpt' field of the search report
header type defined below.
--------------------------------------------------------------------------*/

typedef enum
{
  SRCH_ACQ_R,      /* Aquisition report. */
  SRCH_SLEW_R,     /* Slew complete report. */
  SRCH_UNSLEW_R,   /* Unslew complete report. */
  SRCH_PC_MEAS_R,  /* Pilot report for paging channel. */
  SRCH_TC_MEAS_R,  /* Pilot Strength measurement report. */
  SRCH_REACQ_R,    /* Fully reacquired after sleep report */
  SRCH_RAHO_R,     /* Reacquired, but idle HandOff required report */
  SRCH_RAFAIL_R,   /* Reacquisition after sleep failed */
  SRCH_ACQFAIL_R,  /* Acquisition failed */
  SRCH_PC_ASSIGN_R,/* Report strongest pilot in paging channel assign msg */
  SRCH_WAKEUP_R,   /* Report wakeup from sleep */
  SRCH_FING_DONE_R,/* Report finger assignment completed after handoff */
  SRCH_TRYSLEEP_R, /* Report we might be able to try to sleep again. */

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
  SRCH_THRESH_R,   /* EC Threshold Report from SRCH to MC */    
#if defined (FEATURE_IS95B_PILOT_RPT) || defined (FEATURE_SPECIAL_MDR)
  SRCH_AHO_PLIST_R,/* Nset Pilot Strength Measurement report */   
  SRCH_NO_SECTOR_R,/* No reference sector */
#endif /* FEATURE_IS95B_PILOT_RPT || FEATURE_SPECIAL_MDR */

  SRCH_FAST_SLEEP_R, /* SRCH rxed a fast-sleep command and is sleeping */
  SRCH_FAST_WAKE_R,  /* SRCH rxed a fast-sleep command and is waking up */

#ifdef FEATURE_IS95B_PPSMM
  SRCH_PPM_R,        /* Periodic Pilot Measurement report */
#endif /* FEATURE_IS95B_PPSMM */
#ifdef FEATURE_IS95B_MAHHO
  SRCH_RIF_HHO_FAILURE_R,/* Hard handoff failure report */
  SRCH_RIF_HHO_ABORT_R,  /* Hard handoff abort report */
  SRCH_CF_NSET_R,    /* Candidate Frequency Search Report */
  SRCH_CFS_READY_R,  /* Ready to do CFS */
  SRCH_CF_MEAS_DONE_R, /* CF meas done */
#endif /* FEATURE_IS95B_MAHHO */
#ifdef FEATURE_GPS_MODE
#error code not present
#endif /* FEATURE_GPS_MODE */
#ifdef FEATURE_IS2000
  SRCH_AT_R,  /* Sent at AT of HO/ESCAM etc. sent in SRCH context in MC file */
#endif /* FEATURE_IS2000 */
#ifdef FEATURE_IS2000_R_SCH
  SRCH_SCRM_MEAS_R,   /* SCRM pilot strength measurements report */
#endif /* FEATURE_IS2000_R_SCH */
#ifdef FEATURE_IS2000_QPCH
  SRCH_QPCH_REACQ_R,  /* SRCH have doen QPCH REACQ and possibly gone to sleep */
#endif /* FEATURE_IS2000_QPCH */
#ifdef FEATURE_IS2000_R_SCH
  SRCH_T_ADD_ABORT_R, /* T_ADD_ABORT report */
#endif /* FEATURE_IS2000_R_SCH */
  SRCH_MAX_R         /* Gives number of report codes. MUST be last enum item. */
} mccsrch_report_code_type;


typedef struct
{
  cmd_hdr_type            rpt_hdr;
    /* Includes the queue links,  pointer to requesting task TCB,
       signal to set on completion,  and done queue. */
  
  mccsrch_report_code_type   rpt;
    /* Specifies which type of report this is. See enumerated report
       codes above. */
}
mccsrch_rpt_hdr_type;


/*--------------------------------------------------------------------------
                         ACQUISITION REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_acq_rpt_type;


/*--------------------------------------------------------------------------
                     SLEW TO SYSTEM TIMING COMPLETE REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_slew_rpt_type;


/*--------------------------------------------------------------------------
                    UNSLEW TO SYNC TIMING COMPLETE REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_unslew_rpt_type;


/*--------------------------------------------------------------------------
            PAGING CHANNEL PILOT STRENGTH MEASUREMENT REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  int2               pilot;
/* Removed FEATRUE_JSTD008 */
  byte               band_class;
  int2               freq;
} 
mccsrch_pc_meas_rpt_type;


/*--------------------------------------------------------------------------
            TRAFFIC CHANNEL PILOT STRENGTH MEASUREMENT REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  int2               ref_pilot;
  byte               ecio;
  byte               keep;

  word               meas_cnt;  
    /* Number of pilot measurements in list. */

  struct
  {
    word  pn_phase;
    byte  ecio;
    byte  keep;
  }                  meas[ SRCH_NSET_MAX + SRCH_ASET_MAX - 1 ];

} 
mccsrch_tc_meas_rpt_type;

/* FEATURE_IS95B_PILOT_RPT */
/*--------------------------------------------------------------------------
            Active Pilot and Nset Pilot Strength Measurement Report
--------------------------------------------------------------------------*/

typedef struct
{
  byte    pilot_strength;
  int2    pilot_pn;
  int2    pilot_pn_phase;
  boolean access_ho_allowed;
  boolean access_attempted;
  byte    band_class;
  int2    freq;
} pilot_rpt_list_type;

typedef struct
{
  mccsrch_rpt_hdr_type hdr;
  word num_pilots;
    /* Number of pilots reported by SRCH excluding Active pilot. This value
       can be different from the size of MC neighbor record in case of
       duplicate pilots in Neigbor List Message */
  pilot_rpt_list_type  pilot_list[ CAI_N8M + 1 ];
}
mccsrch_pilots_meas_rpt_type;

#ifdef FEATURE_IS95B_MAHHO
typedef struct
{
 mccsrch_rpt_hdr_type hdr;

 byte   total_rx_pwr;   /* Total received power on the TF */
 word   pilot_cnt;              /* Number of pilots included in this report */

 struct
 {
   word pilot;
   byte ecio;
 } pilot_rec[ SRCH_ASET_MAX ];
}
mccsrch_hho_fail_rpt_type;

typedef enum
{
  DIFF_RX_PWR_TOO_LOW,

  TOTAL_PILOT_EC_IO_TOO_LOW,

  FAIL_REASON_MAX
}
hho_fail_enum_type;

typedef struct
{
  mccsrch_rpt_hdr_type hdr;

  hho_fail_enum_type    hho_fail_reason;

}
mccsrch_hho_abort_rpt_type;

typedef struct
{
   mccsrch_rpt_hdr_type  hdr;  /* Common header */
   byte cfs_seq;
     /* The sequence number for the current search period */

}mccsrch_cfs_meas_done_rpt_type;

typedef struct
{
    mccsrch_rpt_hdr_type  hdr;  /* Common header */
    byte cfs_seq;
      /* The sequence number for the current search period */

} mccsrch_cfs_ready_rpt_type;

/*--------------------------------------------------------------------------
                  CANDIDATE FREQUENCY SEARCH COMPLETE REPORT
                  
This report is sent when we have finished a Candidate Frequency search
--------------------------------------------------------------------------*/
typedef struct
{
    mccsrch_rpt_hdr_type  hdr;  /* Common header */
    byte cfs_seq;
      /* The sequence number for the current search period */

    byte  total_rx_pwr;    /*   CF total received power */

    word  pilot_cnt;   
      /* Number of pilot shall be measured this time */
    struct
    {
        word  pilot;   /* Pilot offset */
        byte  ecio;
    } pilot_rec[SRCH_CFNSET_MAX];
} mccsrch_cfnset_meas_rpt_type;
#endif /* FEATURE_IS95B_MAHHO */

/*--------------------------------------------------------------------------
                  GPS VISIT READY REQUEST AND GPS VISIT DONE REPORT                
--------------------------------------------------------------------------*/
#ifdef FEATURE_GPS_MODE
#error code not present
#endif /* FEATURE_GPS_MODE */

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

/*--------------------------------------------------------------------------
                     SLEEP STATE REACQUIRED REPORT

This report is sent when the channel has been reacquired after a
period of sleep.  The sleep task should then be commanded to go to
the Paging Channel state to resume normal processing.
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
} 
mccsrch_reacq_rpt_type;

#ifdef FEATURE_IS2000_QPCH
typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
} 
mccsrch_qpch_reacq_rpt_type;
#endif /* FEATURE_IS2000_QPCH */

/*--------------------------------------------------------------------------
         SLEEP STATE REACQUIRED, IDLE HANDOFF REQUIRED REPORT

This report is sent when we have reacquired a neighbor set member
after a period of sleep. 
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  int2               pilot;
      /* The pilot offset of the reacquired pilot */
/* Removed FEATRUE_JSTD008 */
  byte               band_class;
  int2               freq;
      /* The frequency of the reacquired pilot */
} 
mccsrch_raho_rpt_type;

/*--------------------------------------------------------------------------
               SLEEP STATE REACQUISITION FAILED REPORT

This report is sent when we have failed to reacquire the channel after a
period of sleep. 
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
} 
mccsrch_rafail_rpt_type;

/*--------------------------------------------------------------------------
                         ACQUISITION FAILED REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_acq_fail_type;

/*--------------------------------------------------------------------------
              PAGING CHANNEL ASSIGNMENT SEARCH COMPLETE REPORT
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr; /* New report type SRCH_PC_ASSIGN_R */

  int2  pilot; /* Strongest pilot in the list given in the pc_assign_cmd */
}
mccsrch_pc_assign_rpt_type;

/*--------------------------------------------------------------------------
                     SLEEP STATE WAKEUP REPORT

This report is sent when we have fully awakened from a period of deep sleep.
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  byte  slot_cycle_index;
    /* Slot cycle index used in latest sleep period */

  boolean  rude_awakening;
    /* If TRUE, Search was rudely awakened */

  boolean  srch_slept;
    /* Set to TRUE if the wakeup report follows search actually sleeping,
       set to FALSE if we didn't actually get to sleep */

  qword    valid_time;
    /* Time stamp of when "Wakeup" officially occurs at */

  byte cur_slot_mask;
    /* the current slot mask SRCH is waking up to  */
} 
mccsrch_wakeup_rpt_type;

/*--------------------------------------------------------------------------
                  FINGER ASSIGNMENT COMPLETED REPORT 

This report is sent when we have assigned demodulation fingers after handoff
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
    /* Report header only */
} 
mccsrch_fing_done_rpt_type;

#ifdef FEATURE_IS95B_PPSMM
typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
  int2               ref_plt;     /* Time reference PN offset */
  byte               ecio;     /* Pilot Strength */
  byte               keep;        /* Keep pilot indicator */
  byte               sf_rx_pwr;   /* received power spectral density */
  byte               meas_cnt;   /* Number of pilot measurements */  
  
  /* num_pilot occurrences of the following record */
  struct
  {
    word  pn_phase;
    byte  ecio;
    byte  keep;
  }meas[SRCH_CSET_MAX + SRCH_ASET_MAX - 1];

}mccsrch_ppm_rpt_type;
#endif /* FEATURE_IS95B_PPSMM */

/*--------------------------------------------------------------------------
                  OK TO RETRY A VETOED SLEEP REPORT 

This report is sent when conditions have changed and a previously vetoed
sleep command may be retried.
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
    /* Report header only */

  qword rpt_time;
    /* Time at which report is generated.  Allows MCC to ignore report
       lingering in queues during sleep. */
} 
mccsrch_trysleep_rpt_type;   

/* This report is used to indicate to MC that AT for HO/ESCAM has arrived
   This rpt is sent in the srch context but by the function that srch calls at
   action time to get the active set */
#ifdef FEATURE_IS2000
typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
    /* Report header only */
  byte transaction_num;
    /* trans num of the HO/ESCAM transaction */

} mccsrch_at_rpt_type; 
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_IS2000_R_SCH
/*--------------------------------------------------------------------------
       SUPPLEMENTAL CHANNEL REQUEST PILOT STRENGTH MEASUREMENT REPORT 

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
  int2 ref_pn;           /* Time reference PN sequence offset */ 
  byte ref_pn_ecio;      /* Reference pilot strength */
  byte meas_cnt;         /* Number of pilot measurements */
  /* Pilots in ASet, Nset and Cset other than Reference Pilot */
  struct  
  {
    word    pn_phase;    /* Pilot measured phase */
    byte    pn_ecio;     /* Pilot strength */
    boolean pn_in_aset;  /* Pilot in ASet or not */
  } meas[CAI_MAX_SCRM_RPT_PN];
} mccsrch_scrm_meas_rpt_type;
#endif /* FEATURE_IS2000_R_SCH */

/*--------------------------------------------------------------------------
                      UNION OF ALL SEARCH REPORTS

--------------------------------------------------------------------------*/

typedef union
{
  mccsrch_rpt_hdr_type      hdr;
  mccsrch_acq_rpt_type      acq;
  mccsrch_slew_rpt_type     slew;
  mccsrch_unslew_rpt_type   unslew;
  mccsrch_pc_meas_rpt_type  pc_meas;
  mccsrch_tc_meas_rpt_type  tc_meas;
  mccsrch_reacq_rpt_type    reacq;
  mccsrch_raho_rpt_type     raho;
  mccsrch_rafail_rpt_type   rafail;
  mccsrch_acq_fail_type      aqfail;
  mccsrch_pc_assign_rpt_type pc_assign;
  mccsrch_wakeup_rpt_type   wakeup;
  mccsrch_fing_done_rpt_type fing_done;
#ifdef FEATURE_SD20
#error code not present
#endif/* FEATURE_SD20 */
  mccsrch_trysleep_rpt_type trysleep;
#if defined (FEATURE_IS95B_PILOT_RPT) || defined (FEATURE_SPECIAL_MDR)
  mccsrch_pilots_meas_rpt_type pilots_meas;
#endif /* FEATURE_IS95B_PILOT_RPT || FEATURE_SPECIAL_MDR */
#ifdef FEATURE_IS95B_PPSMM
  mccsrch_ppm_rpt_type  ppsm;
#endif /* FEATURE_IS95B_PPSMM */ 
#ifdef FEATURE_IS95B_MAHHO
  mccsrch_hho_abort_rpt_type   hho_abort;
  mccsrch_hho_fail_rpt_type    hho_fail;
  mccsrch_cfnset_meas_rpt_type cfnset_meas_rpt; /* New added */
  mccsrch_cfs_ready_rpt_type   cfs_ready_rpt;
  mccsrch_cfs_meas_done_rpt_type cfs_meas_done_rpt;
#endif /* FEATURE_IS95B_MAHHO */
#ifdef FEATURE_GPS_MODE
#error code not present
#endif /* FEATURE_GPS_MODE */
#ifdef FEATURE_IS2000
  mccsrch_at_rpt_type at_rpt;
#endif /* FEATURE_IS2000 */
#ifdef FEATURE_IS2000_R_SCH
  mccsrch_scrm_meas_rpt_type scrm_meas_rpt;
  mccsrch_scrm_meas_rpt_type t_add_abort_rpt;
#endif /* FEATURE_IS2000_R_SCH */
} mccsrch_rpt_type;


extern q_type          mcc_srch_q;
extern q_type          mcc_srch_free_q;


/*===========================================================================

FUNCTION MCCSRCH_INIT

DESCRIPTION
  This function initializes the queues between the Searcher task and the CDMA
  Main Control task.  It also places buffers on the free queue.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mccsrch_init( void );


/*===========================================================================

FUNCTION MCCSRCH_FAST_RAHO

DESCRIPTION
  This function is called in the context of the Searcher task during idle 
  sleep to expedite MC's response to a fast RAHO.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcc_srch_fast_raho(word new_pilot);


/*===========================================================================

FUNCTION MCC_SRCH_NBRCFG1_PC_WALSH

DESCRIPTION
  This function returns page channel walsh code of a neighbor BS with 
  NBR_CFG=1. The function is called in the context of the Searcher task 
  during a fast RAHO so there is enough time for the new PN long code mask
  to settle down.

DEPENDENCIES
  None.

RETURN VALUE
  The page channel walsh code of the neighbor BS.

SIDE EFFECTS
  None.

===========================================================================*/

extern byte mcc_srch_nbrcfg1_pc_walsh(word pilot);


#if defined( FEATURE_ANTIREGISTRATION_BIAS ) || defined ( FEATURE_IS2000_QPCH )

/*===========================================================================

FUNCTION MCC_SRCH_NBR_CHECKING

DESCRIPTION
  This function checks neighbor BS info before SRCH make Idle Hand Off 
  decision.

DEPENDENCIES
  None.

RETURN VALUE
  A value of mcc_srch_nbr_info_type.

SIDE EFFECTS
  None.

===========================================================================*/

/* return type definition of function mcc_srch_nbr_checking() */
typedef enum
{
  NBR_BS_IMMEDIATE_REG,
    /* if IHO ocurrs, the MS needs an immediate zone or parameter 
     * registration on the neighbor BS.
     */
  NBR_BS_OVHD_NOT_OK,
    /* Neighbor BS does NOT have an entry in th BS_INFO array, 
     * or it does not have all overhead messages
     */
  NBR_BS_OVHD_OK
    /* Neighbor BS' OVHD MSG are current at the moment the funtion 
     * is called 
     */
} mcc_srch_nbr_info_type;

/* Removed FEATURE_JSTD008 */
extern mcc_srch_nbr_info_type mcc_srch_nbr_checking(word nbr_pilot,
                                                    byte nbr_band,
                                                    word nbr_freq);


#endif /* FEATURE_ANTIREGISTRATION_BIAS */

#ifdef FEATURE_ENHANCED_STANDBY_III
/*===========================================================================

FUNCTION MCC_SRCH_MIN_BS_P_REV

DESCRIPTION
  This function returns the minimum P_REV among the known base station on the
  CDMA channel of the ASET. The known base stations are those with current
  configuration parameters.

DEPENDENCIES
  None.

RETURN VALUE
  The minimum P_REV among the known base station.

SIDE EFFECTS
  None.

===========================================================================*/

extern byte mcc_srch_min_bs_p_rev(byte band, word freq);

#endif /* FEATURE_ENHANCED_STANDBY_III */

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION mccsrch_get_aset

DESCRIPTION
  This function updates code channel list table and returns aset to
  SRCH. This is called from SRCH in its context. 

DEPENDENCIES
  This function is called by SRCH only in the case of a SHO, For HHO, MC knows when
  to give this to SRCH. 
  THIS IS CALLED IN SRCH CONTEXT, MUST NOT HAVE ANY UNNECESSARY STUFF

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void mccsrch_get_aset
(
  byte transaction_num, /* Transaction num of the HO command */
  srch_tc_aset_info_type * tc_aset /*Pointer in which the aset will be returned */
);
#endif /* FEATURE_IS2000 */

#endif /* MCCSRCH_H */

