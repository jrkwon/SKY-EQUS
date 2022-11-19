#ifndef LOG_DMSS_H
#define LOG_DMSS_H

/*===========================================================================

                      Log Services Header File

General Description
  This file contains declarations for packets to be logged.

Copyright (c) 1991-2000 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                             Edit History

$Header:   O:/src/asw/COMMON/vcs/log_dmss.h_v   1.8   08 Mar 2001 11:09:12   ldefauw  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/07/01   jq      Added definition for ASET_INFO log.
01/28/01   abh     Added version id and Trk Lo to GPS Multipeak structure.
12/08/00   aaj     Added session_tag to GPS SRCH MULTIPEAK structure. Changed
                   couple of temporary variables with real names
12/05/00   lad     Created file from DMSS log.h.  Logging content only.
===========================================================================*/

#include "cai.h"
#include "log.h"
#include "nv.h"
#include "target.h"
#include "comdef.h"
#include "srch.h"

#ifdef FEATURE_PLT
#error code not present
#endif

/* -------------------------------------------------------------------------
** Data Declarations
** ------------------------------------------------------------------------- */                           

/* -------------------------------------------------------------------------
** Log Codes
**   These codes identify the kind of information contained in a log entry. 
**   They are used in conjunction with the 'code' field of the log entry 
**   header.  The data types associated with each code are defined below.
** ------------------------------------------------------------------------- */
typedef enum
{
  /* The upper 4 bits of the 16 bit log entry code specify which type
  ** of equipment created the log entry. 
  */
  LOG_TA_C = 0,                                               
                     /* Mobile Station temporal analyzer entry.              */

  LOG_AGC_PCTL_C,    /* AGC values and closed loop power control entry.      */

  LOG_F_MUX1_C,      /* Forward link frame rates and types entry.            */
  LOG_R_MUX1_C,      /* Reverse link frame rates and types entry.            */

  LOG_AC_MSG_C,      /* Access channel message entry.                        */
  LOG_R_TC_MSG_C,    /* Reverse link traffic channel message entry.          */

  LOG_SC_MSG_C,      /* Sync channel message entry.                          */
  LOG_PC_MSG_C,      /* Paging channel message entry.                        */
  LOG_F_TC_MSG_C,    /* Forward link traffic channel message entry.          */

  LOG_VOC_FOR_C,     /* Forward link vocoder packet entry                    */
  LOG_VOC_REV_C,     /* Reverse link vocoder packet entry                    */

  LOG_FING_C,        /* Temporal analyzer finger info only.                  */
  LOG_SRCH_C,        /* Searcher pathlog info (Reused old SRCH logtype)      */

  LOG_ETAK_C,        /* Position and speed information read from ETAK.       */

  LOG_MAR_C,         /* Markov frame statistics.                             */

  LOG_SRCH2_C,       /* New and improved temporal analyzer searcher info.    */

  LOG_HANDSET_C,     /*  The Fujitsu handset information                     */
                     
  LOG_ERRMASK_C,     /*  Vocoder bit error rate mask                         */

  LOG_ANALOG_INFO_C, /* Analog voice channel information                     */

  LOG_ACC_INFO_C,    /* Access probe information                             */

  LOG_GPS_C,         /* Position & speed info read from GPS receiver         */

  LOG_TEST_CMD_C,    /* Test Command information                             */

  LOG_S_AGC_PCTL_C,  /* Sparse (20ms) AGC / closed loop power control entry. */

  LOG_BAND_CHANGE_C, /* Notification of a band class change.                 */

  LOG_DBG_MSG_C,     /* DM debug messages, if being logged via log services  */

  LOG_GENRL_TA_C,    /* General temporal analyzer entry                      */

  LOG_GENRL_TA_SUP_CH_C, 
                     /* General temporal analyzer w/supplemental channels    */

#ifdef FEATURE_PLT
#error code not present
#else
  LOG_EFS_INFO_C,    /* EFS Usage Info - No implementation as yet            */
#endif /* FEATURE_PLT */
  
  LOG_ANALOG_FORW_C, /* Analog Forward Channel                               */
  
  LOG_ANALOG_REVS_C, /* Analog Reverse Channel                               */
                       
  LOG_ANALOG_HANDOFF_C, 
                     /* Analog Handoff Entry                                 */
  
  LOG_ANALOG_FMSLOT_C,  
                     /* FM Slot Statistis entry                              */

  LOG_ANALOG_WS_COUNT_C,
                     /* FOCC Word Sync Count entry                           */
  
  LOG_RLP_PACKET_C,
  LOG_ASYNC_TCP_SEG_C,
  LOG_PACKET_DATA_IP_PACKETS_C,
  LOG_FNBDT_MESSAGE_LOG_C,

/* IS-2000 LOG features */
  LOG_RLP_RX_FRAMES_C,
                     /* RLP RX Frames logging                                */ 
  LOG_RLP_TX_FRAMES_C,
                     /* RLP TX Frames logging                                */
  LOG_RLP_RSVD1_C,   /* Reserved for additions to RLP frames.                */
  LOG_RLP_RSVD2_C,   /* Reserved for additions to RLP frames.                */
  
  LOG_FWD_FRAME_TYPES_C, 
                     /* Forward Link Frame Types logging                     */
  LOG_REV_FRAME_TYPES_C, 
                     /* Reverse Link Frame Types logging                     */
  
  LOG_FFWD_PCTRL_C,  /* Fast Forward Power Control Parameters logging        */
  LOG_REV_PCTRL_C,   /* Reverse Power Control Parameters logging             */

  LOG_SRCH_FING_INFO_C,
                     /* Searcher and Finger Information logging              */
  LOG_SVC_CONFIG_C,  /* Service Configuration logging                        */
  LOG_ASET_CONFIG_C, /* Active Set Configuration logging                     */

  LOG_QPCH_C,        /* Quick Paging Channel logging                         */

  LOG_RLP_STAT_C,    /* RLP Statistics logging                               */  

  LOG_STDSO_C,       /* Simple Test Data Service Option logging              */

  LOG_SRCH_PPM_RES_C,
                     /* Pilot Phase Measurement resutlts logging             */
  
  LOG_SRCH_PPM_DB_C, /* Pilot Phase Measurement Data Base logging            */
  
  LOG_SRCH_PPM_C,    /* Pilot Phase Measurement search results logging       */
  
  LOG_GPS_FWD_MSG_C, /* IS-801 forward link message                          */
  
  LOG_GPS_REV_MSG_C, /* IS-801 reverse link message                          */

  LOG_GPS_STATS_MSG_C, 
                     /* GPS search session statistics                        */
  
  LOG_GPS_SRCH_PEAKS_MSG_C,
                     /* GPS search results                                   */
  
  LOG_FTM_C,         /* Factory Testmode logging                             */

  LOG_SRCH_GPS_MULTI_PEAKS_INFO_C,  /* Multiple Peak Logging                 */
  
  LOG_SRCH_GPS_POST_PROC_C,         /* Post processed search results logs    */

  LOG_FTDSO_C,       /* Simple Test Data Service Option logging              */

  LOG_BT_BASE_C = 62,/* Bluetooth logging                                    */

  LOG_BT_LAST_C = LOG_BT_BASE_C+30, /* BT needs 10 codes */

  /* HDR log code space */
  LOG_HDR_BASE_C = 93,
  LOG_HDR_LAST_C = LOG_HDR_BASE_C+50,

  /* IS2000 DCCH channels */
  LOG_DCCH_FWD_C = 144,
  LOG_DCCH_REV_C = 145,

  
  LOG_ZREX_C = 146, 

  /* 147 Active set info logging, similar to ASET_CONFIG, but simpler.
  */
  LOG_ASET_INFO_C = 147, 
  LOG_LAST_C = LOG_ASET_INFO_C,
  LOG_MAX_C          /* Provides count of number of log entry codes. MUST
                        REMAIN LAST ENUM ITEM.                               */
}log_code_type;

/* This is the array of "nice" values for message levels.  These values are 
** used in a similar fashion as the UNIX "nice" command.  The higher the 
** number, the less priority the item has.
*/
extern int16 log_nice[];

/* Log mask (in bits) containing info on which logs are valid for a particular 
** build.
*/
extern const uint8 log_valid_mask[];

/*---------------------------------------------------------------------------
              SPARSELY-SAMPLED AGC AND POWER CONTROL ENTRY TYPE
---------------------------------------------------------------------------*/
#define LOG_AGC_PCTL_CNT  100

LOG_RECORD_DEFINE(LOG_S_AGC_PCTL_C)

  byte          adc_therm;
    /* Calibrated thermistor ADC sample value. */

  byte          batt_volt;
    /* Calibrated battery voltage */

  byte          tx_pwr_limit;
    /* CDMA TX power limit (hi 8-bits) */

  byte          agc_vals[ LOG_AGC_PCTL_CNT ];                
    /* CAGC values sampled at 20 ms intevals. */

  byte          pctl_vals[ LOG_AGC_PCTL_CNT ];
    /* Power control values sampled at 20 ms intevals. */

  byte          tx_pwr_vals[ LOG_AGC_PCTL_CNT ];                       
    /* CAGC TX power estimate values (hi 8-bits) sampled at 20 ms intervals. */

LOG_RECORD_END


/*---------------------------------------------------------------------------
                  REVERSE LINK FRAME STATS ENTRY TYPE                        
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_R_MUX1_C)

#define               LOG_R_MUX1_CNT  100
  byte          vals[ LOG_R_MUX1_CNT ];
    /* Mixed mode bits for sequential reverse link traffic frames. */

LOG_RECORD_END


/*---------------------------------------------------------------------------
                    ACCESS CHANNEL MESSAGE ENTRY TYPE                        
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_AC_MSG_C)

  byte          msg[ CAI_AC_MAX_MSG_BYTES ];
    /* Actual access channel message bits. */

LOG_RECORD_END


/*---------------------------------------------------------------------------
                    REVERSE TRAFFIC CHANNEL ENTRY TYPE                        
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_R_TC_MSG_C)

  byte          msg[ CAI_REV_TC_MAX_MSG_BYTES ];
    /* Actual reverse link traffic channel message bits. */

LOG_RECORD_END

/*---------------------------------------------------------------------------
                     SYNC CHANNEL MESSAGE ENTRY TYPE                        
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_SC_MSG_C)

  byte          msg[ CAI_SC_MAX_MSG_BYTES ];
    /* Actual sync channel message bits. */

LOG_RECORD_END

/*---------------------------------------------------------------------------
                    PAGING CHANNEL MESSAGE ENTRY TYPE                        
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_PC_MSG_C)

  byte          msg[ CAI_PC_MAX_MSG_BYTES ];
    /* Actual paging channel message bits. */

LOG_RECORD_END

/* FORWARD TRAFFIC CHANNEL MESSAGE ENTRY TYPE                        
*/
LOG_RECORD_DEFINE(LOG_F_TC_MSG_C)
                     
  byte          msg[ CAI_FWD_TC_MAX_MSG_BYTES ];
    /* Actual forward link traffic channel message bits. */

LOG_RECORD_END

/* Vocoder Data Entry Type
*/
LOG_RECORD_DEFINE(LOG_VOC_FOR_C)

  byte  data[36];                 /* vocoder data */
  byte  rate;                     /* rate */
  boolean revlink;                /* is reverse link? */

LOG_RECORD_END

typedef LOG_VOC_FOR_C_type LOG_VOC_REV_C_type;

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */


/*---------------------------------------------------------------------------
                           FINGER ENTRY TYPE                        
 Note: similar info to that contained in the Temporal Analyzer log entry,
       but not including searcher data.
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_FING_C)

  word          fing_pilot[ 3 ];
    /* For each finger, indicates which pilot offset is in use. */

  word          fing_x[ 3 ];
    /* For each finger, gives low order 16 bits of absolute offset in 
       1/8 chip units. */

  byte          fing_y[ 3 ];
    /* For each finger, indicates the current RSSI value. If zero,
       indicates finger is not locked. */

LOG_RECORD_END


/*---------------------------------------------------------------------------
                        SEARCHER PATH PACKET TYPE ( TYPE D )                       

 Note: An abstraction of the Temporal Analyzer log entry, but logs only 
       three largest paths of each search to log every search.

 Basic usage: 
       This is designed to be used running Async 115.2 Kbps, or HDLC 57K.
       It is designed to fit within the payload data of the log_srch2_type.
       Mobile packages up each search and sends this packet to the DM.
       This is designed to be logged when in TA mode and file logging enabled.
       The mobile MUST take care to carefully meter other packets or
       some of these will get lost.
       
       Example for max data rate calc from Mobile: 
         
         Search rate:   Fastest  - 150 searches/sec (throttled value)
                        Moderate - 100 searches/sec (40-chip windows)
                        Slow     - 25 searches/sec (160-chip windows)

         Bytes/pkt:     B = (16 srchs/pkt) * sizeof(log_path_type) + [ovhd]
                          = 832 bytes + [24+?] = appx 860 (for example)

         Max rate:      R = 860 * 8 * 9.37 pks/sec = 64 kbits/sec (fastest)
                        R = 860 * 8 * 6.24 pks/sec = 43 kbits/sec (40-chip win)
                        R = 860 * 8 * 1.56 pks/sec = 11 kbits/sec (160-chip win)
                        Note: Anything above around 50 kbits/sec misses searches 
---------------------------------------------------------------------------*/
#define MAX_PATHLOG_PATHS  3
#define MAX_FINGS          3
#define MAX_PATHLOG_SRCHS 16   /* Data MUST fit log_srch2_type payload */
                               /* NOTE: 17 actually fit, but we have ovhd */

typedef PACKED struct
{
  int2          pilot;                  /* Pilot offset of search */
  byte          sect;                   /* Index of set membership */
  byte          set;                    /* Set membership */
  int2          win_siz;                /* Search window size in chips */
  int4          win_pos;                /* Absolute window position (chipx8) */
  word          tdiff;                  /* Time since pack start (1.25ms) */
  byte          frame_erase;            /* Count of frames/erasures */
  byte          filt_len;               /* Length of energy filter */
  word          filt_eng;               /* Filtered total energy of this PN */
  word          inx[MAX_PATHLOG_PATHS]; /* Position of path relative to window */
  word          eng[MAX_PATHLOG_PATHS]; /* Instantaneous energy of path */
  /* 14 words total */

  int2          f_pilot[MAX_FINGS];  /* Finger's pilot assignment */
  byte          f_sect[MAX_FINGS];   /* Finger's sector in Aset assignment */
  byte          f_rssi[MAX_FINGS];   /* Finger RSSI */
  int4          f_pos[MAX_FINGS];    /* Absolute finger positions */
  /* 12 words total */

  /* Note that we also pack the RX/TX/ADJ values into the top bytes
     of the finger positions, and the counts of frames and frame errors
     in the top byte of the searcher window positions. */
} 
log_path_type;
  
typedef PACKED struct
{
  byte   pkt_seq_num;        /* Pkt seq num to keep track of skips */
  word   srch_seq_num;       /* Can reveal lost seaches if full pkt */
  byte   srch_cnt;           /* Number of searches in search array */
  qword  start_time;         /* Time started packing message */
  /* 6 words */

  /* Search-related parms */
  byte   state;
  byte   aset_cnt;
  byte   cset_cnt;
  byte   nset_cnt;
  byte   freq_est;
  byte   search_rate;
  /* 3 words */

  byte   adc_therm;    /* Calibrated thermistor ADC sample value. */
  byte   batt_volt;    /* Calibrated battery voltage */
  byte   tx_pwr_limit; /* CDMA TX power limit (hi 8-bits) */
  byte   rx_agc;       /* CAGC value */
  byte   tx_gain_adj;  /* Power control value */
  byte   tx_agc;       /* CAGC TX power estimate values (hi 8-bits) */
  /* 3 words */
  
  log_path_type srch[MAX_PATHLOG_SRCHS]; /* Paths for each search */
} 
log_srch_type;


/*---------------------------------------------------------------------------
                        SEARCHER PATH PACKET TYPE ( TYPE A1 )                       

 Note: An abstraction of the Temporal Analyzer log entry, but logs only 
       four largest paths of each search to log every search. This packet is 
       based on the Type D packet and used to log the fourth finger in MSM2.3.

 Basic usage: 
       This is designed to be used running Async 115.2 Kbps, or HDLC 57K.
       It is designed to fit within the payload data of the log_srch2_type.
       Mobile packages up each search and sends this packet to the DM.
       This is designed to be logged when in TA mode and file logging enabled.
       The mobile MUST take care to carefully meter other packets or
       some of these will get lost.
       
---------------------------------------------------------------------------*/
#define  MAX_FINGS_MSM22          3
  /* Max number of fingers for MSM2.2 */

#define  MAX_FINGS_MSM23          4
  /* Max number of fingers for MSM2.3 */
  
#define  MAX_PATHS_MSM23          4
  /* Max number of paths for MSM2.3. MSM2.2 has three */

#define  LOG_PATH_MAX_DATA_CNT    14   
  /* Max number of searches logged per packet. Data MUST fit log_srch2_type 
     payload */ 

typedef PACKED struct
{
  int2   pilot;                       /* Pilot offset of search */
  byte   sect;                        /* Index of set membership */
  byte   set;                         /* Set membership */
  int2   win_siz;                     /* Search window size in chips */
  int4   win_pos;                     /* Absolute window position (chipx8) */
  word   tdiff;                       /* Time since pack start (1.25ms) */
  byte   frame_erase;                 /* Count of frames/erasures */
  word   filt_eng;                    /* Filtered total energy of this PN */
  word   inx[MAX_PATHS_MSM23];        /* Position of path relative to window */
  word   eng[MAX_PATHS_MSM23];        /* Instantaneous energy of path */
  /* 31 bytes total */

  int2   f_pilot[MAX_FINGS_MSM23];    /* Finger's pilot assignment */
  byte   f_sect[MAX_FINGS_MSM23];     /* Finger's sector in Aset assignment */
  byte   f_rssi[MAX_FINGS_MSM23];     /* Finger RSSI */
  int4   f_pos[MAX_FINGS_MSM23];      /* Absolute finger positions */
  /* 32 bytes total */

  /* Note that we also pack the RX/TX/ADJ values and the length of the
     energy filter into the top bytes of the finger positions, and 
     the LNA state into the top byte of the searcher window position. */
} 
log_path_data_type;
  
typedef PACKED struct
{
  byte   pkt_seq_num;        /* Pkt seq num to keep track of skips */
  word   srch_seq_num;       /* Can reveal lost seaches if full pkt */
  byte   srch_cnt;           /* Number of searches in search array */
  qword  start_time;         /* Time started packing message */
  /* 6 words */

  /* Search-related parms */
  byte   state;
  byte   aset_cnt;
  byte   cset_cnt;
  byte   nset_cnt;
  byte   freq_est;
  byte   search_rate;
  /* 3 words */

  byte   adc_therm;    /* Calibrated thermistor ADC sample value. */
  byte   batt_volt;    /* Calibrated battery voltage */
  byte   tx_pwr_limit; /* CDMA TX power limit (hi 8-bits) */
  byte   rx_agc;       /* CAGC value */
  byte   tx_gain_adj;  /* Power control value */
  byte   tx_agc;       /* CAGC TX power estimate values (hi 8-bits) */
  /* 3 words */
  
  log_path_data_type srch[ LOG_PATH_MAX_DATA_CNT ]; /* Paths for each search */
} 
log_path_srch2_A1_type;


/*---------------------------------------------------------------------------
                   GENERAL TEMPORAL ANALYZER ENTRY TYPE                         
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_GENRL_TA_C)

  byte            pregain;
    /* Pregain setting used for search: 0 = 2, 1 = 4, 2 = 8, 3 = 32. */

  byte            integ;
    /* Integration period used for search in 4 chip units. */

  byte            non_coh;
    /* Number of non-coherent integrations accumulated. Value is
       one less than actual value, e.g., 0 implies 1. */

  /* Following masks are OR'd into the pilot_off field. */

#define  LOG_ASET_M  0x8000  /* pilot belongs to active set. */
#define  LOG_CSET_M  0x4000  /* pilot belongs to candidate set. */
#define  LOG_NSET_M  0x2000  /* pilot belongs to neighbor set. */
#define  LOG_RSET_M  0x1000  /* pilot belongs to remaining set. */
#define  LOG_PCSET_M 0x0800  /* pilot belongs to the pre-candidate set. */
#define  LOG_NOCEN_M 0x0400  /* Suppress DM TA window recentering */

  unsigned short  pilot_off; 
    /* Pilot offset used during search. */

  int4            win_pos;
    /* Absolute window ending position in 1/8 chip units. */

  unsigned short  win_cen;
    /* Specifies center position of searcher window space. */

  int2            tcxo_drift;
    /* Specifies tcxo error in 1/(8*256) chip units. Note number is
       signed. */

  unsigned short  path_pos[ MAX_PATHS_MSM23 ];
    /* Position of path relative to window */

  unsigned short  path_eng[ MAX_PATHS_MSM23 ];
    /* Instantaneous energy of path */

  unsigned short  f_pilot[ MAX_FINGS_MSM23 ];
    /* Finger's pilot assignment */
  
  byte            f_sect[ MAX_FINGS_MSM23 ];
    /* Finger's sector in Aset assignment */
  
  byte            f_rssi[ MAX_FINGS_MSM23 ];
    /* For each finger, indicates the current RSSI value. If zero,
       indicates finger is not locked. */

  int4            f_pos[ MAX_FINGS_MSM23 ];
    /* Absolute finger positions */
  
  byte            rx_agc;
    /* CAGC value */

  byte            tx_agc;
    /* CAGC TX power estimate values (hi 8 bits) */

  byte            tx_gain_adj;
    /* Power control value */

  byte            tx_pwr_limit;
    /* CDMA TX power limit (hi 8 bits) */

  byte            srch_state;
    /* Searcher state */

LOG_RECORD_END


#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
/*---------------------------------------------------------------------------
        GENERAL TEMPORAL ANALYZER ENTRY WITH SUPPLEMENTAL CHANNELS TYPE
        NOTE: Same as log_gen_ta_type with sup channels added.                         
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_GENRL_TA_SUP_CH_C)

  byte            pregain;
    /* Pregain setting used for search: 0 = 2, 1 = 4, 2 = 8, 3 = 32. */
 
  byte            integ;
    /* Integration period used for search in 4 chip units. */
 
  byte            non_coh;
    /* Number of non-coherent integrations accumulated. Value is
       one less than actual value, e.g., 0 implies 1. */
 
  unsigned short  pilot_off; 
    /* Pilot offset used during search. */
 
  int4            win_pos;
    /* Absolute window ending position in 1/8 chip units. */
 
  unsigned short  win_cen;
    /* Specifies center position of searcher window space. */
 
  int2            tcxo_drift;
    /* Specifies tcxo error in 1/(8*256) chip units. Note number is
       signed. */
 
  unsigned short  path_pos[ MAX_PATHS_MSM23 ];
    /* Position of path relative to window */
 
  unsigned short  path_eng[ MAX_PATHS_MSM23 ];
    /* Instantaneous energy of path */
 
  unsigned short  f_pilot[ MAX_FINGS_MSM23 ];
    /* Finger's pilot assignment */
   
  byte            f_sect[ MAX_FINGS_MSM23 ];
    /* Finger's sector in Aset assignment */
   
  byte            f_rssi[ MAX_FINGS_MSM23 ];
    /* For each finger, indicates the current RSSI value. If zero,
       indicates finger is not locked. */
 
  int4            f_pos[ MAX_FINGS_MSM23 ];
    /* Absolute finger positions */
   
  byte            rx_agc;
    /* CAGC value */
 
  byte            tx_agc;
    /* CAGC TX power estimate values (hi 8 bits) */
 
  byte            tx_gain_adj;
    /* Power control value */

  byte            tx_pwr_limit;
    /* CDMA TX power limit (hi 8 bits) */
 
  byte            srch_state;
    /* Searcher state */
 
  byte              num_sup_chan;
    /* Number of supplemental channels */

  word walsh[ SRCH_SUP_CHANNEL_MAX + 1 ];
     /* Walsh Codes */

LOG_RECORD_END

#endif /* defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) */


/*---------------------------------------------------------------------------
                          ENVIRONMENT ENTRY TYPE
---------------------------------------------------------------------------*/
#define  LOG_ENVRN_MAX_DATA_CNT  37

/* Define a structure for environment data */
typedef PACKED struct
{
  unsigned short  pilot_off; 
    /* Pilot offset used during search. */
 
  int4            win_pos;
    /* Absolute window position */
   
  unsigned short  path_pos[ MAX_PATHS_MSM23 ];
    /* Position of path relative to window */
 
  unsigned short  path_eng[ MAX_PATHS_MSM23 ];
    /* Instantaneous energy of path */
 
  unsigned short  tdiff;
    /* Time since packing starts (in 1.25ms) */

  /* Note: we load LNA state into the MSBit of the tdiff field and RX AGC value
           into the upper byte of the win_pos field. */
}
log_envrn_data_type;

typedef PACKED struct
{
  byte                 srch_cnt; 
    /* Number of searches done for the packet */

  qword                start_time;
    /* Time we started packing */

  byte                 pregain;
    /* Pregain setting used for search: 0 = 2, 1 = 4, 2 = 8, 3 = 32. */

  byte                 integ;
    /* Integration period used for search in 4 chip units. */

  byte                 non_coh;
    /* Number of non-coherent integrations accumulated. Value is
       one less than actual value, e.g., 0 implies 1. */
  
  log_envrn_data_type  data[ LOG_ENVRN_MAX_DATA_CNT ];
    /* Data portion of the packet */
}
log_envrn_srch2_A0_type;

#if defined(FEATURE_GPSONE) && defined(FEATURE_SRCH_BATCH_LOG)
#error code not present
#endif /* #if defined(FEATURE_GPSONE) && defined(FEATURE_SRCH_BATCH_LOG) */


/*---------------------------------------------------------------------------
                         MARKOV FRAME STATISTICS
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_MAR_C)

  byte          count;
    /* number of entries in entry array */

#define                   LOG_MAR_CNT 100
  PACKED struct
  {
    byte          mar_inx;
    /* Decision indices for sequential frames. Each index value is computed
       according to the following formula:

         mar_inx = (EXPECTED_MUX1 * 16) + ACTUAL_MUX1

       where EXPECTED_MUX1(i) is the MUX1 index (1 to 14, a la CAI) predicted
       by the Markov process for frame i, and ACTUAL_MUX1 is the MUX1 index
       actually decided upon by the mobile's rate determination algorithm for
       frame i. */

    byte          bit_errs;
      /* Bit error counts for sequential frames. */
  }entry[ LOG_MAR_CNT];
  
LOG_RECORD_END

/*---------------------------------------------------------------------------
                        SEARCHER 2 ENTRY TYPE                        
 Note: Supersedes previous Temporal Analyzer log entry type. Allows for
       variable window size.
---------------------------------------------------------------------------*/

LOG_RECORD_DEFINE(LOG_SRCH2_C)

  byte          pregain;
    /* Pregain setting used for search: 0 = 2, 1 = 4, 2 = 8, 3 = 32. */

  byte          integ;
    /* Integration period used for search in 4 chip units. */

  byte          non_coh;
    /* Number of non-coherent integrations accumulated. Value is
       one less than actual value, e.g., 0 implies 1. */

  /* Following masks are OR'd into the pilot_off field. */

  word          pilot_off;
    /* Pilot offset used during search. */

  word          win_pos;
    /* Low order 16 bits of absolute window ending position in 1/8 chip
       units. */

  word          win_cen;
    /* Specifies center position of searcher window space. */

  int2          tcxo_drift;
    /* Specifies tcxo error in 1/(8*256) chip units. Note number is
       signed. */

  word          num_entries;
    /* Number of entries in energies array. */

#define                   LOG_TA2_CNT  (228*2)
  word          energies[ LOG_TA2_CNT ];
    /* Energies versus half chip time offset. */

LOG_RECORD_END

/*---------------------------------------------------------------------------
                     GENERAL ANALOG INFORMATION ENTRY TYPE
---------------------------------------------------------------------------*/
LOG_RECORD_DEFINE(LOG_ANALOG_INFO_C)
  word            sid;        /* SID            */
  word            channel;    /* Analog Channel */
  byte            scc;        /* SCC            */
  byte            rssi;       /* RSSI           */
  byte            power;      /* Tx Power       */                  
LOG_RECORD_END


/*---------------------------------------------------------------------------
                      ANALOG FOCC Word Sync Count ENTRY TYPE
---------------------------------------------------------------------------*/
LOG_RECORD_DEFINE(LOG_ANALOG_WS_COUNT_C)
   
  dword ws_count;            /* Word Sync Count */

LOG_RECORD_END


/*---------------------------------------------------------------------------
                     ANALOG HANDOFF ENTRY TYPE
---------------------------------------------------------------------------*/
typedef enum           /* Handoff Type */
{
  CDMA_TO_ANALOG_HANDOFF_LOG = 0,
  ANALOG_HANDOFF_LOG
}
analog_handoff_type;

typedef enum        /* Indicates reason for Handoff    */
{
  HANDOFF_MASK_NA = 0x0,   /* Not Applicable(CDMA-ANALOG HO ) */
  VCHAN_CHANGE_MASK = 0x1, /* Change in Voice Channel         */
  SCC_CHANGE_MASK = 0x2,   /* Change in SCC                   */
  VMAC_CHANGE_MASK = 0x4   /* Change in VMAC                  */
}
analog_handoff_mask_type;


LOG_RECORD_DEFINE(LOG_ANALOG_HANDOFF_C)
  byte               handoff_type;     /* 0 - Cdma to Analog  
                                          1 - Analog Handoff         */
  byte               change_mask;      /* Reason for Handoff         */
  word               amps_vc;          /* AMPS Voice Channel Number  */
  byte               scc;              /* SCC                        */
  byte               pwr_level;        /* Power Level                */
  word               system_id;        /* System Identification Code */
LOG_RECORD_END

/*---------------------------------------------------------------------------
                      ANALOG CHANNEL(FORW/REVS) ENTRY TYPE
---------------------------------------------------------------------------*/
/* Max Channel Log Buffer Size */
#define        CHANNEL_LOG_MAX         200
typedef enum  /* analog channel type */
{
   ANALOG_FOCC = 0,
   ANALOG_FVC,
   ANALOG_RECC,
   ANALOG_RVC
}
analog_channel_type; 


LOG_RECORD_DEFINE(LOG_ANALOG_FORW_C)
   
  byte msg[CHANNEL_LOG_MAX];  /* Buffer to hold channel data */

LOG_RECORD_END
 
typedef LOG_ANALOG_FORW_C_type LOG_ANALOG_REVS_C_type;


/* ANALOG FM_SLOT Statistics ENTRY TYPE
*/
LOG_RECORD_DEFINE(LOG_ANALOG_FMSLOT_C)
  dword invalid_words;           /* invalid words   */
  dword good_words;              /* good words      */
  dword single_errs;             /* single errors   */
  dword multiple_errs;           /* multiple errors */

  dword total_rx_isr;            /* total recvd frames                   */
  dword no_free_buffs;           /* count of *out of free buf* condition */
  dword skip_words;              /* #words skipped after a DFM reset     */

  dword slot_count_1;            /* #words decoded after 1 slot  */
  dword slot_count_2;            /* #words decoded after 2 slot  */
  dword slot_count_3;            /* #words decoded after 3 slot  */
  dword slot_count_4;            /* #words decoded after 4 slot  */
  dword slot_count_5;            /* #words decoded after 5 slot  */

  dword error_slot_count_1;      /* #errors in 1 slot decoded words */
  dword error_slot_count_2;      /* #errors in 2 slot decoded words */
  dword error_slot_count_3;      /* #errors in 3 slot decoded words */
  dword error_slot_count_4;      /* #errors in 4 slot decoded words */
  dword error_slot_count_5;      /* #errors in 5 slot decoded words */

  byte  new_sync_erase_value;    /* new status of sync erasure  */
  byte  old_sync_erase_value;    /* old status of sync erasure  */
  dword sync_erase_count;        /* #sync erasures              */
  dword sync_erase_error_count;  /* #errors during sync erasure */
  dword out_of_sync_frames;      /* #out of sync frames         */
  dword out_of_sync_count;       /* #out of sync                */

  dword new_time_value;          /* current time                */
  dword old_time_value;          /* old time                    */
LOG_RECORD_END

/* Band Class Change Type
** Note: This is not in use for QCT builds as of 10/26/00
*/
LOG_RECORD_DEFINE(LOG_BAND_CHANGE_C)

  byte         band_class;
  word         channel;

LOG_RECORD_END

/* EFS Information Type
*/
LOG_RECORD_DEFINE(LOG_EFS_INFO_C)
   
  byte efs_info[CHANNEL_LOG_MAX];       /* Buffer to EFS Log data */

LOG_RECORD_END

/* Access Probe Information Type
*/
LOG_RECORD_DEFINE(LOG_ACC_INFO_C)

  byte seq_num;
    /* Access probe sequence number */
  byte probe_num;
    /* Access probe number */
  byte rx_agc;
    /* Receive AGC */
  byte tx_adj;
    /* TX gain adjust */
  byte psist;
    /* number of persistence tests performed */
  byte ra;
    /* access channel number */
  byte rn;
    /* PN randomization delay */
  byte rs;
    /* Sequence backoff */
  byte rt;
    /* probe backoff */

LOG_RECORD_END


#ifdef TMOBILE
#error code not present
#endif /* TMOBILE */


#if defined(FEATURE_RLP_LOGGING)
/*---------------------------------------------------------------------------
                       RLP PARTIAL FRAMES LOG TYPE
---------------------------------------------------------------------------*/
/* This is a high volume accumulated log.  Many fields are bit stuffed. */

#define LOG_MAX_RLP_PKT 300

typedef enum {
  LOG_RLP_FCH    = 0,  /* Fundamental Channel          */
  LOG_RLP_DCCH   = 1,  /* Dedicated Control Channel    */
  LOG_RLP_SCH_0  = 2,  /* Supplemental Channel 0       */
  LOG_RLP_SCH_1  = 3,  /* Supplemental Channel 1       */
  LOG_RLP_SCCH_0 = 4,  /* IS95B Supplemental Channel 0 */
  LOG_RLP_SCCH_1 = 5,  /* IS95B Supplemental Channel 1 */
  LOG_RLP_SCCH_2 = 6,  /* IS95B Supplemental Channel 2 */
  LOG_RLP_SCCH_3 = 7,  /* IS95B Supplemental Channel 3 */
  LOG_RLP_SCCH_4 = 8,  /* IS95B Supplemental Channel 4 */
  LOG_RLP_SCCH_5 = 9,  /* IS95B Supplemental Channel 5 */
  LOG_RLP_SCCH_6 = 10, /* IS95B Supplemental Channel 6 */
  LOG_RLP_SCCH_7 = 11, /* IS95B Supplemental Channel 7 */

  LOG_RLP_MAX    = 15  /* Values 12-15 reserved        */
} log_rlp_phy_channel_enum_type;

/* Channel frame entry for partial RLP frame.
*/
/* Plans exist to change this format slightly! */
typedef PACKED struct {
  PACKED struct {
    byte phy_chan : 4;       /* log_rlp_phy_channel_enum_type  */
    byte mux_pdu  : 3;       /* Use mux_pdu_type 0-7           */
                             /* 0 == "mux_pdu1"                */
                             /* 1 == "mux_pdu2"                */     
    byte rsvd     : 1;
  } mux_chan;

  PACKED struct {
    word phy_chan_rate : 14; /* Physical Channel Rate / 100    */
    word time_delta    : 1;  /* 0 if same frame, 
                                1 if next frame                */
    word rsvd          : 1;
  } time_rate;

  word frame_len;            /* Length, in bits, of this frame. */ 
                             /* 0 for blank, 0xFFFF for erasure.*/

  PACKED struct {
    byte tail :2;            /* Number of tail payload bytes    */
    byte head :6;            /* Number of head payload bytes    */

  } payload_len;

  byte payload[1];           /* Head and tail payload bytes, 
                                length given above.             */
} log_rlp_pkt_type;

/* Partial RLP frames log record.
*/
LOG_RECORD_DEFINE(LOG_RLP_RX_FRAMES_C)
  
  byte         service_ID; /* Service ID                      */
  byte         count;      /* Count of partial RLP packets    */

  PACKED union {
    
    log_rlp_pkt_type pkt[1];
    byte             buf_ref[LOG_MAX_RLP_PKT]; 
                           /* Accumulated partial RLP packets
                              defined in log_rlp_pkt_type     */
  } var_len_buf;

LOG_RECORD_END

typedef LOG_RLP_RX_FRAMES_C_type LOG_RLP_TX_FRAMES_C_type;

#define LOG_MAX_RLP_PAYLOAD_LEN 34

#define LOG_MAX_RLP_FRM_SIZE \
  (FPOS(log_rlp_pkt_type, payload) + LOG_MAX_RLP_PAYLOAD_LEN)
  
/* Threshold to determine when the buf is full and the log should be sent. */
#define LOG_RLP_FRM_BUF_THRESHOLD \
  (sizeof(LOG_RLP_RX_FRAMES_C_type) - \
    (FPOS(log_rlp_pkt_type, payload) + LOG_MAX_RLP_PAYLOAD_LEN))

/*---------------------------------------------------------------------------
                         RLP STATISTICS LOG TYPE
---------------------------------------------------------------------------*/
#define LOG_RLP_MAX_NAK_ROUNDS  3

/*---------------------------------------------------------------------------
  RLP statistics information definition. This structure contains all of the 
  RLP statistical information for the currently active RLP session.
---------------------------------------------------------------------------*/
LOG_RECORD_DEFINE(LOG_RLP_STAT_C)
  
  uint8        service_ID; /* Service ID                          */
  uint64       reset_time; /* Timestamp for last statistic reset. */

  uint32 naks_received ;           /* total # of nak frames received        */
  uint32 re_xmits_not_found;       /* # of Rexmits not found                */
  uint16 largest_contig_erasure;   /* largest block of consecutive erasures */
  uint8  resets;                   /* # of resets                           */
  uint8  aborts;                   /* # of aborts    (nak aborts)           */
  uint32 last_rtt;                 /* round trip time                       */   
  uint8  rlp_blob_used;            /* 0-no BLOB; 1-NAK; 2-RTT; 3-NAK+RTT    */
  
  uint32 rx_rexmit_fr_cnt;         /* # of received re-transmitted frames   */
  uint32 rx_idle_fr_cnt;           /* # of received idle frames             */
  uint32 rx_fill_fr_cnt;           /* # of received fill frames             */
  uint32 rx_blank_fr_cnt;          /* # of blank frames received            */
  uint32 rx_null_fr_cnt;           /* # of null frames received             */
  uint32 rx_new_data_fr_cnt;       /* total # of received new data frames   */ 
  uint32 rx_20ms_fr_cnt;           /* # of received Fundamental data frames */
  uint32 rx_total_bytes;           /* total # of bytes received             */
  uint32 rx_rlp_erasures;          /* # of rlp erasures received            */
  uint32 rx_mux_erasures;          /* # of mux erasures received            */
  
  uint32 tx_rexmit_fr_cnt;         /* # of transmitted re-transmitted frames*/
  uint32 tx_idle_fr_cnt;           /* # of transmitted idle frames          */
  uint32 tx_new_data_fr_cnt;       /* total # of new transmitted data frames*/ 
  uint32 tx_20ms_fr_cnt;           /* # of txed Fundamental data frames     */
  uint32 tx_total_bytes;           /* total # of bytes transmitted          */
  uint8  tx_naks_cnt;              /* Number of tx_naks[] entries to follow */
  uint32 tx_naks[LOG_RLP_MAX_NAK_ROUNDS];
                                   /* single, double etc NAKs sent array    */

LOG_RECORD_END
#endif /* FEATURE_RLP_LOGGING */


#ifdef FEATURE_IS2000
/*---------------------------------------------------------------------------
                     FORWARD FRAME TYPES LOG TYPE
---------------------------------------------------------------------------*/
/* Dedicated forward channel set mask.
*/
typedef PACKED union {

  PACKED struct {
    byte f_fch      : 1; /* F-FCH  active      */
    byte f_sch0     : 1; /* F-SCH1 active      */
    byte f_sch1     : 1; /* F-SCH0 active      */
    byte f_dcch     : 1; /* F-DCCH active      */
    byte f_scch_num : 3; /* # of F-SCCH active */
    byte reserved   : 1; /* unused/reserved    */

  } chan_set;
  
  byte mask; /* Used to init the entire field */
  
} log_ded_fwd_chan_set_type;

/* Dedicated reverse channel set mask.
*/
typedef PACKED union {

  PACKED struct {
    byte r_fch     : 1; /* R-FCH  active     */
    byte r_sch0    : 1; /* R-SCH1 active     */
    byte r_sch1    : 1; /* R-SCH0 active     */
    byte r_dcch    : 1; /* R-DCCH active     */
    byte reserved  : 4; /* Unused/reserved   */
    
  } chan_set;
  
  byte mask; /* Used to init the entire field */
  
} log_ded_rev_chan_set_type;

typedef PACKED struct {
  uint16 f_fch_rc  :4; /* Forward Fundamental Channel Radio Config       */
  uint16 f_sch0_rc :4; /* Forward Supplemental Channel 0 Radio Config    */
  uint16 f_sch1_rc :4; /* Forward Supplemental Channel 1 Radio Config    */
  uint16 f_dcch_rc :4; /* Forward Dedicated Control Channel Radio Config */

} log_fwd_ch_rc_type;

typedef PACKED struct {
  uint16 r_fch_rc  :4; /* Reverse Fundamental Channel Radio Config       */
  uint16 r_sch0_rc :4; /* Reverse Supplemental Channel 0 Radio Config    */
  uint16 r_sch1_rc :4; /* Reverse Supplemental Channel 1 Radio Config    */
  uint16 r_dcch_rc :4; /* Reverse Dedicated Control Channel Radio Config */

} log_rev_ch_rc_type;

/* Enum for FCH frame type */
typedef enum {
  LOG_FCH_TYPE_PAYLOAD_ONLY = 0,
  LOG_FCH_TYPE_DIM_BURST    = 1,
  LOG_FCH_TYPE_BLANK_BURST  = 2
  
} log_fch_type_enum_type;

/* Enum for F-FCH rates */
typedef enum {
  LOG_FFCH_ERASURE_RATE = 0, /* Indicates erasure       */
  LOG_FFCH_EIGHTH_RATE  = 1, /* Indicates rate 1/8 data */
  LOG_FFCH_QUARTER_RATE = 2, /* Indicates rate 1/4 data */
  LOG_FFCH_HALF_RATE    = 3, /* Indicates rate 1/2 data */
  LOG_FFCH_FULL_RATE    = 4  /* Indicates rate 1   data */

} log_f_fch_rate_enum_type;

/* Enum for R-FCH rates */
typedef enum {
  LOG_RFCH_FULL_RATE    = 0, /* Indicates rate 1   data */
  LOG_RFCH_HALF_RATE    = 1, /* Indicates rate 1/2 data */
  LOG_RFCH_QUARTER_RATE = 2, /* Indicates rate 1/4 data */
  LOG_RFCH_EIGHTH_RATE  = 3  /* Indicates rate 1/8 data */

} log_r_fch_rate_enum_type;

/* Enum for SCH and SCCH channels */
typedef enum {
  LOG_SCH_FULL_RATE   = 0,
  LOG_SCH_BLANK_FRAME = 1,
  LOG_SCH_ERASUE      = 2,
  LOG_SCH_NOT_ACTIVE  = 3  /* Valid for SCH only */

} log_sch_rate_enum_type; /* All SCH and SCCH channels */


/* Slowest case: 25bytes/1byte(per 20ms frame) --> 0.5 sec per log
** Fastest case: 25bytes/2bytes(per 20ms frame) --> 0.25 sec per log
*/
#define LOG_MAX_FRAME_TYPES_SIZE (25 + 4) 

/* Forward Frame Types frame record.  This structure defines each frame entry
** in the sub-record.
*/
typedef PACKED union {
  
  PACKED struct {
    byte rate : 4; /* Use log_f_fch_rate_enum_type */
    byte type : 4; /* Use log_fch_type_enum_type   */
  } f_fch;
  
  byte f_sch0; /* Use log_sch_rate_enum_type */
  byte f_sch1; /* Use log_sch_rate_enum_type */
  
  byte f_dcch; /* TBD */
  
  PACKED struct {
    byte f_scch1 : 2; /* Use log_sch_rate_enum_type */
    byte f_scch2 : 2; /* Use log_sch_rate_enum_type */
    byte f_scch3 : 2; /* Use log_sch_rate_enum_type */
    byte f_scch4 : 2; /* Use log_sch_rate_enum_type */
  
  } f_scch1_4; /* F-SCCH 1-4 */
  
  PACKED struct {
    byte f_scch5  : 2; /* Use log_sch_rate_enum_type */
    byte f_scch6  : 2; /* Use log_sch_rate_enum_type */
    byte f_scch7  : 2; /* Use log_sch_rate_enum_type */
    byte reserved : 2;
  
  } f_scch5_7; /* F-SCCH 5-7 */
} log_fwd_frame_types_frm_type;

/* Forward Frame Types sub-record.  Used to log multiple frames
** with the same active channel set.  If the active channel set 
** changes, a new sub-record is started.
*/
typedef PACKED struct {
  log_ded_fwd_chan_set_type ch_set_mask; /* Set of active channels */

  word frame_cnt; /* # of frames represented in this sub-record */

  byte len_per_frame; /* Length, in bytes, of each frame entry */

  /* Array of frame type indicators. One per channel per frame.
  ** Note: There will be an entry for each member that is associated with an
  ** active channel in the active channel mask.  Not all members will exist
  ** in each frame entry.  Entries appear in order of active LSB to MSB in
  ** the channel set mask (FCH first).
  */
  PACKED union {
    /* Array of multiple frame entries.  
    ** Up to 4 entries (4 ch or FCH + SCCH) per frame 
    */
    log_fwd_frame_types_frm_type frame_type_ind[4];

    /* Member for referenceing offsets in buf */
    byte buf_ref[LOG_MAX_FRAME_TYPES_SIZE - 4];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

} log_fwd_frame_types_subrecord_type; /* FTSS */

/* Forward Frame Types Log Record
*/
LOG_RECORD_DEFINE(LOG_FWD_FRAME_TYPES_C)

  byte frame_offset; /* Frame offset in 1.25 ms units. */
  
  word so;           /* Service option */
  
  log_fwd_ch_rc_type fwd_ch_rc; 
                     /* Forward channel Radio Config Info */
  
  byte sr_cnt;       /* Count of sub-records */
  
  /* Series of var length log_fwd_frame_types_subrecord_type */
  PACKED union {
    /* Buffer will contain at least 1 subrecord */
    log_fwd_frame_types_subrecord_type sr[1];
    
    /* Member for referenceing offsets in buf */
    byte buf_ref[LOG_MAX_FRAME_TYPES_SIZE]; 

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

LOG_RECORD_END

/* Threshold to determine when the buf is full and the log should be sent. 
*/
#define LOG_FWD_FRAME_TYPES_BUF_THRESHOLD \
  (sizeof(LOG_FWD_FRAME_TYPES_C_type) -     \
   FPOS(log_fwd_frame_types_subrecord_type, var_len_buf.frame_type_ind[4]))


/*---------------------------------------------------------------------------
                     REVERSE FRAME TYPES LOG TYPE
---------------------------------------------------------------------------*/

/* Reverse Frame Types frame record.  This structure defines each frame entry
** in the sub-record. 
*/
typedef PACKED union {
  
  PACKED struct {
    byte rate : 4; /* Use log_r_fch_rate_enum_type */
    byte type : 4; /* Use log_fch_type_enum_type   */
  } r_fch;
  
  byte r_sch0; /* Use log_sch_rate_enum_type */
  byte r_sch1; /* Use log_sch_rate_enum_type */
  
  byte r_dcch; /* TBD */   
  
} log_rev_frame_types_frm_type;

/* Reverse Frame Types sub-record.  Used to log multiple frames with the same 
** active channel set.  If the active channel set changes, a new 
** sub-record is started.
*/
typedef PACKED struct {
  log_ded_rev_chan_set_type ch_set_mask; /* Set of active channels */

  word frame_cnt; /* # of frames represented in this sub-record */

  byte len_per_frame; /* Length, in bytes, of each frame entry */

  /* Array of frame type indicators. One per channel per frame.
  ** Note: There will be an entry for each member that is associated with an
  ** active channel in the active channel mask.  Not all members will exist
  ** in each frame entry.  Entries appear in order of active LSB to MSB in
  ** the channel set mask (FCH first).
  */
  PACKED union {
    /* Array of multiple frame entries.  
    ** Up to 4 entries (4 ch) per frame 
    */
    log_rev_frame_types_frm_type frame_type_ind[4]; /* 4 is the max per frame */

    /* Member for referenceing offsets in buf */
    byte buf_ref[LOG_MAX_FRAME_TYPES_SIZE - 4];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

} log_rev_frame_types_subrecord_type; /* FTSS */

/* Reverse Frame Types Log Record 
*/
LOG_RECORD_DEFINE(LOG_REV_FRAME_TYPES_C)

  byte frame_offset; /* Frame offset in 1.25 ms units. */
  
  word so;           /* Service option */
  
  log_rev_ch_rc_type rev_ch_rc; 
                     /* Reverse channel Radio Config Info */

  byte sr_cnt;       /* Count of sub-records */
  
  /* Series of var length log_rev_frame_types_subrecord_type */
  PACKED union {
    /* Buffer will contain at least 1 subrecord */
    log_rev_frame_types_subrecord_type sr[1];
    
    /* Member for referenceing offsets in buf */
    byte buf_ref[LOG_MAX_FRAME_TYPES_SIZE]; 

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

LOG_RECORD_END

/* Threshold to determine when the buf is full and the log should be sent. 
*/
#define LOG_REV_FRAME_TYPES_BUF_THRESHOLD \
  (sizeof(LOG_REV_FRAME_TYPES_C_type) -     \
   FPOS(log_rev_frame_types_subrecord_type, var_len_buf.frame_type_ind[4]))


#ifdef FEATURE_FFPC
/*---------------------------------------------------------------------------
                   FAST FORWARD POWER CONTROL LOG TYPE
---------------------------------------------------------------------------*/
/* Slowest case: 75bytes/3bytes(per 20ms frame) --> 0.5 sec per log
** Fastest case: 75bytes/5bytes(per 20ms frame) --> 0.3 sec per log
*/
#define LOG_MAX_FFPC_SIZE (75 + 4) /* 75 frame entries + 4 per sr */
  
/* Fast Forward Power Control frame record.  This structure defines each 
** frame entry in the sub-record.
*/
typedef PACKED struct {
  word dec_history; /* Decision History: 16 1-bit decisions per frame */
  
  /* List of ffpc setpoint indicators that are active in the channel set mask.
  ** Note: There will be an entry for each member that is associated with an
  ** active channel in the active channel mask.  Not all members will exist
  ** in each frame entry.  Entries appear in order of active LSB to MSB in
  ** the channel set mask (FCH first).
  */
  PACKED union {
    uint8 f_fch;       /* F-FCH setpoint in 0.125 bB units       */
    uint8 f_sch0;      /* F-SCH0 setpoint in 0.125 bB units      */
    uint8 f_sch1;      /* F-SCH1 setpoint in 0.125 bB units      */
    uint8 f_dcch;      /* F-DCCH setpoint in 0.125 bB units      */
    int8  f_sch_delta; /* F-SCH delta setpoint in 0.125 dB units */

  } setpts[5]; /* Up to five entries based on channel set mask. */

} log_ffwd_pctrl_frame_type;

/* Fast Forward Power Control sub-record.  Used to log multiple frames with
** the same active channel set.  If the active channel set changes, a new 
** sub-record is started.
*/
typedef PACKED struct {
  log_ded_fwd_chan_set_type ch_set_mask; /* Set of active channels */

  word frame_cnt; /* # of frames represented in this sub-record */

  byte len_per_frame; /* Length, in bytes, of each frame entry */

  /* Variable length buffer containing 'frame_cnt' 
  ** entries of log_ffwd_pctrl_frame_type 
  */
  PACKED union {
    
    /* Each frame entry is variable length based on channel set mask */
    log_ffwd_pctrl_frame_type frame_vals[1];

    /* Member for referenceing offsets in buf */
    byte buf_ref[LOG_MAX_FFPC_SIZE - 4];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

} log_ffwd_pctrl_subrecord_type;

/* Fast Forward Power Control Log Record 
*/
LOG_RECORD_DEFINE(LOG_FFWD_PCTRL_C)

  byte frame_offset; /* Frame offset in 1.25 ms units. */

  word so;               /* Service Option                 */
  byte fpc_mode;         /* FPC Mode                       */
  byte fpc_pri_chan;     /* Sub-channel Indicator          */
  byte fpc_subchan_gain; /* Sub-channel gain 0.25dB units  */

  byte sr_cnt;       /* Count of sub-records */
  
  /* Series of var length log_ffwd_pctrl_subrecord_type */
  PACKED union {
    /* Buffer will contain at least 1 subrecord */
    log_ffwd_pctrl_subrecord_type sr[1];
    
    /* Member for referenceing offsets in buf */
    byte buf_ref[LOG_MAX_FFPC_SIZE]; 

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

LOG_RECORD_END

/* Threshold to determine when the buf is full and the log should be sent. 
*/
#define LOG_FFPC_BUF_THRESHOLD                          \
  (sizeof(log_ffwd_pctrl_type) -                        \
    (FPOS(log_ffwd_pctrl_subrecord_type, var_len_buf) + \
     sizeof(log_ffwd_pctrl_frame_type)))
  
#endif /* FEATURE_FFPC */


/*---------------------------------------------------------------------------
                 REVERSE POWER CONTROL LOG TYPE
---------------------------------------------------------------------------*/
/* Slowest case: 175bytes/7bytes(per 20ms frame) --> 0.5 sec per log
** Fastest case: 175bytes/9bytes(per 20ms frame) --> 0.38 sec per log
*/
#define LOG_MAX_REV_PCTRL_SIZE 175

/* Reverse Power Control frame record.  This structure defines each 
** frame entry in the sub-record.
*/
typedef PACKED struct {
  
  uint16 dec_history; /* Power control decision history */

  uint8  rx_agc;       /* RX AGC vals            */
  uint8  tx_pwr;       /* TX CAGC vals           */
  uint8  tx_gain_adj;  /* Gain adjust pctrl vals */
  
  /* List rpctrl setpoint indicators that are active in chan_mask.
  ** Note: There will be an entry for each member that is associated with an
  ** active channel in the active channel mask.  Not all members will exist
  ** in each frame entry.  Entries appear in order of active LSB to MSB in
  ** the channel set mask (FCH first).
  */
  PACKED union {
    int16 r_fch_pich;  /* R-FCH TX power in 0.125 bB units  */
    int16 r_sch0_pich; /* R-SCH0 TX power in 0.125 bB units */
    int16 r_sch1_pich; /* R-SCH1 TX power in 0.125 bB units */
    int16 r_dcch_pich; /* R-DCCH TX power in 0.125 bB units */

  } tx_ch_pwr[4]; /* array of channel pwr vals. Up to 4 per frame. */
} log_rev_pctrl_frame_type;

/* Reverse Power Control sub-record.  Used to log multiple frames with
** the same active channel set.  If the active channel set changes, a new 
** sub-record is started.
*/
typedef PACKED struct {
  log_ded_rev_chan_set_type ch_set_mask; /* Set of active channels */

  word frame_cnt;   /* # of frames represented in this sub-record */

  byte len_per_frame; /* Length, in bytes, of each frame entry */

  /* Variable length buffer containing 'frame_cnt' 
  ** entries of log_rev_pctrl_frame_type 
  */
  PACKED union {
    
    /* Each frame entry is variable length based on channel set mask */
    log_rev_pctrl_frame_type frame_vals[1];

    /* Member for referenceing offsets in buf */
    byte buf_ref[LOG_MAX_REV_PCTRL_SIZE - 3];

  } var_len_buf; /* sizeof(var_len_buf) gives max length */
  
} log_rev_pctrl_subrecord_type;

/* Reverse Power Control Log Record 
*/
LOG_RECORD_DEFINE(LOG_REV_PCTRL_C)

  byte frame_offset; /* Frame offset in 1.25 ms units. */

  byte band_class;   /* Band Class */

  log_rev_ch_rc_type rev_ch_rc; 
                     /* Reverse channel Radio Config Info */

  byte pilot_gating_rate; /* Pilot gating rate       */
  byte step_size;         /* Power control step size */

  byte sr_cnt;       /* Count of sub-records */
  
  /* Series of var length log_rev_pctrl_subrecord_type */
  PACKED union {

    /* Buffer will contain at least 1 subrecord */
    log_rev_pctrl_subrecord_type sr[1];
    
    /* Member for referenceing offsets in buf */
    byte buf_ref[LOG_MAX_REV_PCTRL_SIZE]; 

  } var_len_buf; /* sizeof(var_len_buf) gives max length */

LOG_RECORD_END

/* Threshold to determine when the buf is full and the log should be sent. 
*/
#define LOG_REV_PCTRL_BUF_THRESHOLD                    \
  (sizeof(LOG_REV_PCTRL_C_type) -                      \
    (FPOS(log_rev_pctrl_subrecord_type, var_len_buf) + \
     sizeof(log_rev_pctrl_frame_type)))


/*---------------------------------------------------------------------------
                 SEARCHER AND FINGER INFORMATION LOG TYPE
---------------------------------------------------------------------------*/
typedef PACKED struct {
    uint2 path_pos;  /* Position of path relative to window */
    
    uint2 path_eng;  /* Instantaneous energy of path        */

} log_srch_path_info_type; /* 'num_peaks' items in array */

typedef PACKED union {
  PACKED struct {
    word enabled   : 1;
    word assgn     : 1; /* Is the finger assigned?                      */
    word comb_lock : 1; /* Is the finger locked in the symbol combiner? */
    word pc_lock   : 1; /* The rest (below) are not yet used            */
    word aux_pilot : 1;
    word otd_pilot : 1;
    word sts_pilot : 1;
    word div_chan  : 1;
    word sr3       : 1;
    word non_pilot : 1;
    word reserved  : 6;
  } status;

  word mask; /* Use to init entire mask. */

} log_srch_fing_status_type;

typedef PACKED struct {
    uint2 f_pilot; /* Finger's pilot assignment                          */
     
    byte  f_sect;  /* Finger's sector in Aset assignment                 */
     
    byte  f_rssi;  /* For each finger, indicates the current RSSI value.
                      If zero, indicates finger is not locked.           */
   
    int4  f_pos;   /* Absolute finger position                           */

  
    log_srch_fing_status_type status_mask; /* Finger status */

} log_srch_fing_info_type;

LOG_RECORD_DEFINE(LOG_SRCH_FING_INFO_C)

  PACKED struct {
    byte td_entry   : 1; /* 1 == TX diversity pilot in this record */
    byte td         : 1; /* 1 == TX diversity enabled              */
    byte pilot_type : 1; /* 0 == Common, 1 == Auxiliary            */
    byte reserved   : 5;
  } pilot_type_info;

  byte band_class; /* Band Class */                                

  byte pregain;    /* Pregain setting used for 
                      search: 0 = 2, 1 = 4, 2 = 8, 3 = 32.               */

  byte integ;      /* Integration period used for search
                      in 4 chip units.                                   */

  byte non_coh;    /* Number of non-coherent integrations accumulated. 
                      Value is one less than actual value, 
                      e.g., 0 implies 1.                                 */

  uint2 pilot_off; /* Pilot offset used during search.                   */ 

  int4  win_pos;   /* Absolute window ending position in 1/8 chip units. */
 
  uint2 win_size;  /* The sweep window size to use when searching for this sector.
                      Units are 1/8 chips to make slew math clear and simple. */


  byte rx_agc;       /* CAGC value                                */
 
  byte tx_agc;       /* CAGC TX power estimate values (hi 8 bits) */
 
  byte tx_gain_adj;  /* Power control value                       */

  byte tx_pwr_limit; /* CDMA TX power limit (hi 8 bits)           */
 

  byte srch_state;   /* Searcher state             */

  log_fwd_ch_rc_type fwd_ch_rc;
                     /* Forward channel Radio Configurations */

  log_ded_fwd_chan_set_type active_channels;
                     /* Active forward channel set */
  
  byte num_peaks;    /* Number of peaks            */
                     
  byte num_fingers;  /* Number of fingers          */
  
  PACKED union {
    PACKED struct {
      log_srch_path_info_type peaks[4];  /* Up to 4 peaks, based on 'num_peaks'.
                                            Note: this is a variable length member.  
                                            Not all entries will exist in the record. */
      log_srch_fing_info_type fings[12]; /* Up to 12 fingers based on 'num_fingers'.
                                            Note: this is a variable length member.  
                                            Not all entries will exist in the record. */
    } max_buf_content;

    byte buf_ref[1]; /* Member used to reference offsets in buffer. */
   
  } var_len_buf;

LOG_RECORD_END


/*---------------------------------------------------------------------------
  Service Configuration Log Type
---------------------------------------------------------------------------*/
typedef PACKED struct {
  PACKED struct {
    byte sch_id   : 3; /* SCH ID             */
    byte fwd_rev  : 1; /* 0 == fwd, 1 == rev */
    byte reserved : 4;

  } identifier;
  
  int16 mux_opt;     /* SCH Mulitplex option */
  byte  coding_type; /* SCH Coding type      */
  
} log_svc_config_sch_info_type;

LOG_RECORD_DEFINE(LOG_SVC_CONFIG_C)
  
  uint16 so;         /* Service Option */

  /* Radio Configurations */
  log_fwd_ch_rc_type fwd_ch_rc; 
                     /* Forward channel Radio Config Info */
  log_rev_ch_rc_type rev_ch_rc; 
                     /* Reverse channel Radio Config Info */

  byte fch_frame_size;   /* FCH frame size  */
  byte dcch_frame_size;  /* DCCH frame size */

  byte fwd_traffic_type; /* Forward link traffic type */
  byte rev_traffic_type; /* Reverse link traffic type */

  int16 fwd_mux_opt;     /* Forward Link Multiplex option */ 
  int16 rev_mux_opt;     /* Reverse Link Multiplex option */ 

  byte fwd_rate;         /* Forward transmission rate */
  byte rev_rate;         /* Reverse transmission rate */

  byte num_for_sch;      /* Number of forward supplemental channels */
  byte num_rev_sch;      /* Number of reverse supplemental channels */

  /* The following entries will be ordered in order of active channels:
  ** F-SCH0, F-SCH1, R-SCH0, R-SCH1.  Inactive channels will not be 
  ** included in the record. 
  */
  log_svc_config_sch_info_type sch_info[4];

LOG_RECORD_END

/*---------------------------------------------------------------------------
                ACTIVE SET CONFIGURATION LOG TYPE
---------------------------------------------------------------------------*/
/* Per pilot aset configuration information
*/

/* Structure describing per channel information */
typedef PACKED struct {
  uint16 code_chan;   /* Walse Code Channel                */
  byte   qof_mask_id; /* Quasi-Orthagonal Function Mask ID */

} log_aset_config_ch_type;

/* Variable length structure describing per pilot information. */
typedef PACKED struct {
  PACKED struct{
    uint16 pilot_pn;       /* Pilot PN Offset for this pilot */
    byte   pilot_rec_type; /* Pilot record type              */
    byte   reserved;       /* Reserved for future expansion  */

  } pilot_pn_info;

  log_ded_fwd_chan_set_type ch_set_mask; /* Set of active channels */
  
  byte ch_cfg_cnt; /* Number of entries in the following array. */

  /* Active Set Configuraiton info per channel.  
  ** Up to 4 for IS2000, up to 8 for IS95B.
  ** The entries appear in the order of bits in the active 
  ** channel mask, from LSB to MSB.
  */
  log_aset_config_ch_type ch_cfg[CAI_MAX_NUM_SUP+1]; 

} log_aset_config_pilot_type;

/* Rate information for the SCH channels.  This is *not* per pilot.
*/
typedef PACKED struct {
  byte rate       : 4; 
  byte r_walsh_id : 1; /* 1 if reverse SCH is covered by walsh code */
  byte reserved   : 3; 

} log_aset_config_sch_type;

LOG_RECORD_DEFINE(LOG_ASET_CONFIG_C)

  byte band_class;   /* CDMA band class */
  byte cdma_chan;    /* CDMA channel    */

  byte num_for_sch;  /* # of forward SCH channels in the active set */
  byte num_rev_sch;  /* # of reverse SCH channels in the active set */

  byte num_pilots;   /* Number of pilots in the active set */
  
  /* Variable length per pilot data for the rest of the packet.
  */
  PACKED union {

    /* Variable length based on num_sch.
    */
    PACKED struct {
      /* Variable length array of SCH rates.  'num_sch' gives # of entries.
      ** The entries appear in the following order: 
      ** F-SCH0, F-SCH1, R-SCH0, R-SCH1.
      */    
      log_aset_config_sch_type sch_rate[CAI_MAX_NUM_FOR_SCH +
                                        CAI_MAX_NUM_REV_SCH];
  
      /* Variable length based on 'num_pilots'.
      */
      log_aset_config_pilot_type pilot[6]; /* 6 is the max # of 
                                              pilots for now. */
    } sch_and_pilot_buf;

    /* Variable length based on 'num_pilots'.
    ** Note: Each pilot entry is also variable length.
    */
    log_aset_config_pilot_type pilot[6]; /* 6 is the max # of 
                                            pilots for now. */

    byte buf_ref[1]; /* Used to reference positions */
    
  } var_len_buf;

LOG_RECORD_END

typedef PACKED struct
{
  boolean enable;
  word code_chan;         /* code for current channel - cdma only */
  byte qof_mask_id;
} chan_info_type;

typedef PACKED struct
{
  word    pilot_pn;     /* Pilot PNs currently in the active set */
  byte pilot_rec_type; 
  chan_info_type fch;
  chan_info_type dcch;
  chan_info_type sch;
} dm_active_set_type;

typedef PACKED struct
{
  boolean active;
  byte rate;
  byte walsh_id;
} dm_aset_rsch_type;

LOG_RECORD_DEFINE(LOG_ASET_INFO_C)
  byte    rf_mode;                /* 0->analog,  1->cdma         */
  byte    band_class;             /* 0->Cellular, 1->PCS etc              */
  word    curr_chan;              /* center frequency channel             */
  byte    num_pilots;             /* number of pilots */

  dm_active_set_type aset[CAI_N6M];
  byte    fsch_rate;      // the rate is the same for all FSCHs, only valid when at least one F-SCH is active
  dm_aset_rsch_type rsch; // we only support one supplemental CH

LOG_RECORD_END 
/*---------------------------------------------------------------------------
                QUICK PAGING CHANNEL (QPCH) LOG TYPE
---------------------------------------------------------------------------*/
/* Array index values for QPCH indicators array.
*/
typedef enum {     
  LOG_QPCH_PI1_IND  = 0, /* PI Indicator bit #1  */
  LOG_QPCH_PI2_IND  = 1, /* PI Indicator bit #2  */
  LOG_QPCH_CCI1_IND = 2, /* CCI Indicator bit #1 */
  LOG_QPCH_CCI2_IND = 3, /* CCI Indicator bit #2 */

  LOG_QPCH_MAX_IND

} log_qpch_indicator_enum_type;

/* Status values associated with each indicator bit entry.
*/
typedef enum {
  LOG_QPCH_NOT_APPLICABLE_S = 0, /* This indicator entry is not applicable */
  
  LOG_QPCH_IND_NO_DETECT_S  = 1, /* Indicator bit not detected             */
  LOG_QPCH_IND_DETECT_S     = 2, /* Indicator bit detected                 */
  LOG_QPCH_IND_ERASURE_S    = 3,
  LOG_QPCH_IND_INVALID_S    = 4

} log_qpch_status_enum_type;

/* Status values associated with each indicator bit entry.
*/
typedef PACKED struct {
  uint8  status;         /* Use log_qpch_status_enum_type           */
  uint8  type;           /* Use log_qpch_indicator_enum_type        */
  
  uint16 position;       /* Indicator bit position (ignore for CCI) */
  
  int16  ind_I_energy;   /* QPCH indicator bit energy on I channel  */
  int16  ind_Q_energy;   /* QPCH indicator bit energy on Q channel  */
  int16  pilot_I_energy; /* Pilot energy on I channel               */
  int16  pilot_Q_energy; /* Pilot energy on Q channel               */
  
} log_qpch_indicator_type;

/* QPCH indicator log definition
*/
LOG_RECORD_DEFINE(LOG_QPCH_C)
  
  uint8  chan;       /* See mccqpch_channel_type               */
  uint8  rate;       /* See mccqpch_rate_type                  */
  
  uint16 pilot_pn;   /* Pilot PN offset.                       */
  
  int8  pi_level;    /* QPCH Power level in dB                 */
  byte  thi;         /* THI threshold for PI decision          */
  byte  thb;         /* THB threshold for PI decision          */
    
  uint8 num_entries; /* # of entries in the following array    */

  log_qpch_indicator_type ind[LOG_QPCH_MAX_IND];

LOG_RECORD_END

#ifdef FEATURE_FACTORY_TESTMODE
/* FTM Log Type
*/
LOG_RECORD_DEFINE(LOG_FTM_C)
  
  dword fch_total_frames;       /* total count FCH frames */
  dword fch_fer;                /* total FCH frame error count */
  dword sch_total_frames;       /* total count of SCH frames */
  dword sch_fer;                /* total SCH frame error count */
  dword sch_num_dtx;            /* total SCH DTX count */

LOG_RECORD_END
#endif

#ifdef FEATURE_IS2000_TDSO
/*---------------------------------------------------------------------------
  Test Data Service Option (TDSO) Log Type
	
  Refer to the following tables in the TDSO standard for more info:
	Table 13, Table 14, Table 15, Table 16, Table 17 
---------------------------------------------------------------------------*/
LOG_RECORD_DEFINE(LOG_FTDSO_C)
  uint16 for_mux_opt; /* Forward MUX option */
  uint16 rev_mux_opt; /* REverse MUX option */

  /* Transmit FCH frame counters */
  uint32 tdso_e1_t1;      /* Rate 1 with no signaling                */
  uint32 tdso_e1_td;      /* Rate 1 with dim-and-burst signaling     */
  uint32 tdso_e1_tb;      /* Rate 1 with blank-and-burst signaling   */
  uint32 tdso_eb_tb;      /* Blank 																	 */
  uint32 tdso_eb_to;      /* Frame Category other than blank 				 */
  
  /* Transmit SCH0 frame counters */
  uint32 tdso_e1x_t1x;   /* 1 x (9.6 or 14.4), depending on SCH MUX option  */
  uint32 tdso_e2x_t2x;   /* 2 x (9.6 or 14.4), depending on SCH MUX option  */
  uint32 tdso_e4x_t4x;   /* 4 x (9.6 or 14.4), depending on SCH MUX option  */
  uint32 tdso_e8x_t8x;   /* 8 x (9.6 or 14.4), depending on SCH MUX option  */
  uint32 tdso_e16x_t16x; /* 16 x (9.6 or 14.4), depending on SCH MUX option */
  uint32 tdso_e1x_tb;    /* 1 x (9.6 or 14.4) expected, blank tx'd instead  */
  uint32 tdso_e2x_tb;    /* 2 x (9.6 or 14.4) expected, blank tx'd instead  */
  uint32 tdso_e4x_tb;    /* 4 x (9.6 or 14.4) expected, blank tx'd instead  */
  uint32 tdso_e8x_tb;    /* 8 x (9.6 or 14.4) expected, blank tx'd instead  */
  uint32 tdso_e16x_tb;   /* 16 x (9.6 or 14.4) expected, blank tx'd instead */
  uint32 tdso_esb_tb;     /* Blank   */

  /* Received Frame counters for Fundicated channel FCH/DCCH              */
  uint32 tdso_e1_r1;    /* Error-free Rate 1 frame with no dim-and-burst  */
  uint32 tdso_e1_rerr;  /* Rate 1 with bit errors                         */
  uint32 tdso_e1_rd;    /* Dim-and-burst frame                            */
  uint32 tdso_e1_ro;    /* Other rate Frame                               */
  uint32 tdso_e1_rb;    /* Blank-and-burst                                */
  uint32 tdso_e1_rfl;   /* Rate 1 physical layer frame with insufficient 
                           physical layer frame quality                   */
  uint32 tdso_e1_re;    /* Insufficient frame quality (erasure)           */
  uint32 tdso_en_rn;    /* Null  */
  uint32 tdso_en_rb;    /* Blank */
  uint32 tdso_en_ro;    /* Other */

  /* Received SCH0 frame counters */
  uint32 tdso_e1x_r1x;   /* Error-free 1 x (9.6 or 14.4) frame          */
  uint32 tdso_e2x_r2x;   /* Error-free 2 x (9.6 or 14.4) frame          */
  uint32 tdso_e4x_r4x;   /* Error-free 4 x (9.6 or 14.4) frame          */
  uint32 tdso_e8x_r8x;   /* Error-free 8 x (9.6 or 14.4) frame          */
  uint32 tdso_e16x_r16x; /* Error-free 16 x (9.6 or 14.4) frame         */
  uint32 tdso_e1x_rerr;  /* 1 x (9.6 or 14.4) frame with bit errors     */
  uint32 tdso_e2x_rerr;  /* 2 x (9.6 or 14.4) frame with bit errors     */
  uint32 tdso_e4x_rerr;  /* 4 x (9.6 or 14.4) frame with bit errors     */
  uint32 tdso_e8x_rerr;  /* 8 x (9.6 or 14.4) frame with bit errors     */
  uint32 tdso_e16x_rerr; /* 16 x (9.6 or 14.4) frame with bit errors    */
  uint32 tdso_e1x_re;    /* Expected 1 x (9.6 or 14.4), erasure         */
  uint32 tdso_e2x_re;    /* Expected 2 x (9.6 or 14.4), erasure         */
  uint32 tdso_e4x_re;    /* Expected 4 x (9.6 or 14.4), erasure         */
  uint32 tdso_e8x_re;    /* Expected 8 x (9.6 or 14.4), erasure         */
  uint32 tdso_e16x_re;   /* Expected 16 x (9.6 or 14.4), erasure        */
  uint32 tdso_e1x_rb;    /* 1 x (9.6 or 14.4) expected, blank received  */
  uint32 tdso_e2x_rb;    /* 2 x (9.6 or 14.4) expected, blank received  */
  uint32 tdso_e4x_rb;    /* 4 x (9.6 or 14.4) expected, blank received  */
  uint32 tdso_e8x_rb;    /* 8 x (9.6 or 14.4) expected, blank received  */
  uint32 tdso_e16x_rb;   /* 16 x (9.6 or 14.4) expected, blank received */
  uint32 tdso_esb_rb;     /* Blank                                       */
  uint32 tdso_esb_ro;     /* Frame category other than blank             */
  
                                                                              
  /* Received PDU counters for the Supplemental Channels                */
  uint32 tdso_e3_r3;    /* Error-free Rate 3 MuxPDU                     */
  uint32 tdso_e3_rerr;  /* Rate 3 MuxPDU with Errors                    */
  uint32 tdso_e3_re;    /* Insufficient Frame Quality, Erasures         */
  uint32 tdso_e2_r2;    /* Error-free Rate 2 MuxPDU                     */
  uint32 tdso_e2_rerr;  /* Rate 2 MuxPDU with Errors                    */
  uint32 tdso_e2_re;    /* Insufficient Frame Quality, Erasures         */
  uint32 tdso_e1a_r1a;   /* Error-free Rate 1a MuxPDU                    */
  uint32 tdso_e1a_rerr; /* Rate 1a MuxPDU with Errors                   */
  uint32 tdso_e1a_re;   /* Insufficient Frame Quality, Erasures         */
  uint32 tdso_e1b_r1b;   /* Error-free Rate 1b MuxPDU                    */
  uint32 tdso_e1b_rerr; /* Rate 1a MuxPDU with Errors                   */
  uint32 tdso_e1b_re;   /* Insufficient Frame Quality, Erasures         */
  
LOG_RECORD_END 

/*---------------------------------------------------------------------------
  Simple Test Data Service Option (TDSO) Log Type
---------------------------------------------------------------------------*/
LOG_RECORD_DEFINE(LOG_STDSO_C)
  uint16 for_mux_opt; /* Forward MUX option */
  uint16 rev_mux_opt; /* REverse MUX option */

  /* Transmit FCH frame counters */
  uint32 e1_t1;      /* Rate 1 with no signaling                */
  uint32 e2_t2;      /* Rate 1/2 with no signaling              */
  uint32 e4_t4;      /* Rate 1/4 with no signaling              */
  uint32 e8_t8;      /* Rate 1/8 with no signaling              */
  uint32 e1_td;      /* Rate 1 with dim-and-burst signaling     */
  uint32 e2_td;      /* Rate 1/2 with dim-and-burst signaling   */
  uint32 e4_td;      /* Rate 1/4 with dim-and-burst signaling   */
  uint32 e8_td;      /* Rate 1/8 with dim-and-burst signaling   */
  uint32 e1_tb;      /* Rate 1 with blank-and-burst signaling   */
  uint32 e2_tb;      /* Rate 1/2 with blank-and-burst signaling */
  uint32 e4_tb;      /* Rate 1/4 with blank-and-burst signaling */
  uint32 e8_tb;      /* Rate 1/8 with blank-and-burst signaling */
  uint32 eb_tb;      /* Blank                                   */
  uint32 eb_to;      /* Frame category other than blank         */

  /* Transmit SCH0 frame counters */
  uint32 es1x_t1x;   /* 1 x (9.6 or 14.4), depending on SCH MUX option  */
  uint32 es2x_t2x;   /* 2 x (9.6 or 14.4), depending on SCH MUX option  */
  uint32 es4x_t4x;   /* 4 x (9.6 or 14.4), depending on SCH MUX option  */
  uint32 es8x_t8x;   /* 8 x (9.6 or 14.4), depending on SCH MUX option  */
  uint32 es16x_t16x; /* 16 x (9.6 or 14.4), depending on SCH MUX option */
  uint32 es1x_tb;    /* 1 x (9.6 or 14.4) expected, blank tx'd instead  */
  uint32 es2x_tb;    /* 2 x (9.6 or 14.4) expected, blank tx'd instead  */
  uint32 es4x_tb;    /* 4 x (9.6 or 14.4) expected, blank tx'd instead  */
  uint32 es8x_tb;    /* 8 x (9.6 or 14.4) expected, blank tx'd instead  */
  uint32 es16x_tb;   /* 16 x (9.6 or 14.4) expected, blank tx'd instead */
  uint32 esb_tb;     /* Blank                                           */

  /* Received FCH frame counters */
  uint32 e1_r1;      /* Error-free Rate 1 frame with no dim-and-burst         */
  uint32 e1_rerr;    /* Rate 1 with bit errors detected by the service option */
  uint32 e1_rd;      /* Dim-and-burst frame                                   */
  uint32 e1_rb;      /* Blank-and-burst                                       */
  uint32 e1_r2;      /* Rate 1/2 frame with no dim-and-burst                  */
  uint32 e1_r4;      /* Rate 1/4 frame with no dim-and-burst                  */
  uint32 e1_r8;      /* Rate 1/8 frame with no dim-and-burst                  */
  uint32 e1_rfl;     /* Rate 1 physical layer frame with insufficient 
                        physical layer frame quality                          */
  uint32 e1_re;      /* Insufficient frame quality (erasure)                  */
  uint32 e2_r2;      /* Error-free Rate 1/2 frame with no dim-and-burst       */
  uint32 e2_rerr;    /* Rate 1 with bit errors detected by the service option */
  uint32 e2_rd;      /* Dim-and-burst frame                                   */
  uint32 e2_rb;      /* Blank-and-burst                                       */
  uint32 e2_r1;      /* Rate 1 frame with no dim-and-burst                    */
  uint32 e2_r4;      /* Rate 1/4 frame with no dim-and-burst                  */
  uint32 e2_r8;      /* Rate 1/8 frame with no dim-and-burst                  */
  uint32 e2_rfl;     /* Rate 1 physical layer frame with insufficient 
                        physical layer frame quality                          */
  uint32 e2_re;      /* Insufficient frame quality (erasure)                  */
  uint32 e4_r4;      /* Error-free Rate 1/4 frame with no dim-and-burst       */
  uint32 e4_rerr;    /* Rate 1 with bit errors detected by the service option */
  uint32 e4_rd;      /* Dim-and-burst frame                                   */
  uint32 e4_rb;      /* Blank-and-burst                                       */
  uint32 e4_r1;      /* Rate 1 frame with no dim-and-burst                    */
  uint32 e4_r2;      /* Rate 1/2 frame with no dim-and-burst                  */
  uint32 e4_r8;      /* Rate 1/8 frame with no dim-and-burst                  */
  uint32 e4_rfl;     /* Rate 1 physical layer frame with insufficient 
                        physical layer frame quality                          */
  uint32 e4_re;      /* Insufficient frame quality (erasure)                  */
  uint32 e8_r8;      /* Error-free Rate 1/8 frame with no dim-and-burst       */
  uint32 e8_rerr;    /* Rate 1 with bit errors detected by the service option */
  uint32 e8_rd;      /* Dim-and-burst frame                                   */
  uint32 e8_rb;      /* Blank-and-burst                                       */
  uint32 e8_r1;      /* Rate 1 frame with no dim-and-burst                    */
  uint32 e8_r2;      /* Rate 1/2 frame with no dim-and-burst                  */
  uint32 e8_r4;      /* Rate 1/4 frame with no dim-and-burst                  */
  uint32 e8_rfl;     /* Rate 1 physical layer frame with insufficient 
                        physical layer frame quality                          */
  uint32 e8_re;      /* Insufficient frame quality (erasure)                  */
  uint32 en_rn;      /* Null  */
  uint32 en_rb;      /* Blank */
  uint32 en_ro;      /* Other */

  /* Received SCH0 frame counters */
  uint32 es1x_r1x;   /* Error-free 1 x (9.6 or 14.4) frame          */
  uint32 es2x_r2x;   /* Error-free 2 x (9.6 or 14.4) frame          */
  uint32 es4x_r4x;   /* Error-free 4 x (9.6 or 14.4) frame          */
  uint32 es8x_r8x;   /* Error-free 8 x (9.6 or 14.4) frame          */
  uint32 es16x_r16x; /* Error-free 16 x (9.6 or 14.4) frame         */
  uint32 es1x_rerr;  /* 1 x (9.6 or 14.4) frame with bit errors     */
  uint32 es2x_rerr;  /* 2 x (9.6 or 14.4) frame with bit errors     */
  uint32 es4x_rerr;  /* 4 x (9.6 or 14.4) frame with bit errors     */
  uint32 es8x_rerr;  /* 8 x (9.6 or 14.4) frame with bit errors     */
  uint32 es16x_rerr; /* 16 x (9.6 or 14.4) frame with bit errors    */
  uint32 es1x_re;    /* Expected 1 x (9.6 or 14.4), erasure         */
  uint32 es2x_re;    /* Expected 2 x (9.6 or 14.4), erasure         */
  uint32 es4x_re;    /* Expected 4 x (9.6 or 14.4), erasure         */
  uint32 es8x_re;    /* Expected 8 x (9.6 or 14.4), erasure         */
  uint32 es16x_re;   /* Expected 16 x (9.6 or 14.4), erasure        */
  uint32 es1x_rb;    /* 1 x (9.6 or 14.4) expected, blank received  */
  uint32 es2x_rb;    /* 2 x (9.6 or 14.4) expected, blank received  */
  uint32 es4x_rb;    /* 4 x (9.6 or 14.4) expected, blank received  */
  uint32 es8x_rb;    /* 8 x (9.6 or 14.4) expected, blank received  */
  uint32 es16x_rb;   /* 16 x (9.6 or 14.4) expected, blank received */
  uint32 esb_rb;     /* Blank                                       */
  uint32 esb_ro;     /* Frame category other than blank             */
LOG_RECORD_END

#endif /* FEATURE_IS2000_TDSO */

#endif /* FEATURE_IS2000 */


#ifdef FEATURE_PLT
#error code not present
#endif


#if defined( FEATURE_GPSONE )
#error code not present
#endif /* defined( FEATURE_GPSONE ) */

#if 1
//#if defined(FEATURE_LOG_NAMING_COMPATIBILITY)
/* Union of All Log Entry Types
*/
  typedef LOG_R_MUX1_C_type log_r_mux1_type;
  typedef LOG_AC_MSG_C_type log_ac_msg_type;
  typedef LOG_R_TC_MSG_C_type log_r_tc_msg_type;
  typedef LOG_SC_MSG_C_type log_sc_msg_type;
  typedef LOG_PC_MSG_C_type log_pc_msg_type;
  typedef LOG_F_TC_MSG_C_type log_f_tc_msg_type;
  typedef LOG_VOC_FOR_C_type log_voc_type;
  typedef LOG_FING_C_type log_fing_type;
  typedef LOG_SRCH2_C_type LOG_SRCH_C_type; 
  typedef LOG_MAR_C_type log_mar_type;
  typedef LOG_SRCH2_C_type log_srch2_type;
  typedef LOG_ANALOG_INFO_C_type log_analog_info_type;
  typedef LOG_ANALOG_FORW_C_type log_analog_channel_type;
  typedef LOG_ANALOG_FMSLOT_C_type log_analog_fmslot_type;
  typedef LOG_ANALOG_WS_COUNT_C_type log_analog_ws_count_type;
  typedef LOG_ANALOG_HANDOFF_C_type log_analog_handoff_type;
  typedef LOG_ACC_INFO_C_type log_acc_info_type;
  typedef LOG_S_AGC_PCTL_C_type log_s_agc_pctl_type;

  
  typedef LOG_GENRL_TA_C_type log_gen_ta_type;

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  typedef LOG_GENRL_TA_SUP_CH_C_type log_gen_ta_sup_ch_type;
#endif

#ifdef FEATURE_PLT
#error code not present
#endif

#if defined(FEATURE_RLP_LOGGING)
  typedef LOG_RLP_RX_FRAMES_C_type log_rlp_frames_type;
  typedef LOG_RLP_STAT_C_type      log_rlp_stats_type;
#endif

#ifdef FEATURE_IS2000
  typedef LOG_FWD_FRAME_TYPES_C_type log_fwd_frame_types_type;
  typedef LOG_REV_FRAME_TYPES_C_type log_rev_frame_types_type;
  typedef LOG_FFWD_PCTRL_C_type      log_ffwd_pctrl_type;
  typedef LOG_REV_PCTRL_C_type       log_rev_pctrl_type;
  typedef LOG_SRCH_FING_INFO_C_type  log_srch_fing_type;
  typedef LOG_ASET_CONFIG_C_type     log_aset_config_type;
  typedef LOG_ASET_INFO_C_type       log_aset_info_type;
  typedef LOG_SVC_CONFIG_C_type      log_svc_config_type;
  typedef LOG_QPCH_C_type            log_qpch_type;

#ifdef FEATURE_IS2000_TDSO
  typedef LOG_STDSO_C_type log_stdso_type;
  typedef LOG_FTDSO_C_type log_ftdso_type;
#endif
#endif /* FEATURE_IS2000 */

#if defined( FEATURE_GPSONE )
#error code not present
#endif /* defined( FEATURE_GPSONE ) */

#ifdef FEATURE_FACTORY_TESTMODE
  typedef LOG_FTM_C_type log_ftm_type;
#endif



typedef PACKED union
{
  log_hdr_type             hdr;       /* Generic header.                     */
  log_r_mux1_type          r_mux1;    /* Reverse link frame stats.           */
  log_ac_msg_type          ac_msg;    /* Access channel message.             */
  log_r_tc_msg_type        r_tc_msg;  /* Reverse traffic channel message.    */
  log_sc_msg_type          sc_msg;    /* Sync channel message.               */
  log_pc_msg_type          pc_msg;    /* Paging channel message.             */
  log_f_tc_msg_type        f_tc_msg;  /* Forward traffic channel message.    */
  log_voc_type             voc;       /* Vocoder package                     */
  log_fing_type            fing;      /* Temporal Analyzer finger info.      */
  log_srch_type            srch;      /* Temporal Analyzer searcher info.    */
  log_mar_type             mar;       /* Markov frame statistics info.       */
  log_srch2_type           srch2;     /* Temporal Analyzer report            */
  log_analog_info_type     analog;    /* Gen Analog information              */
  log_analog_channel_type  channel;   /* Analog Channel Information          */
  log_analog_fmslot_type   fmslot;    /* FM Slot Stats                       */
  log_analog_ws_count_type ws_count;  /* FOCC Word Sync Count                */
  log_analog_handoff_type  handoff;   /* Handoff information                 */
  log_acc_info_type        acc_info;  /* Access probe information            */
  log_s_agc_pctl_type      s_agc_pctl;/* Sparsely-sampled AGC and pctl value.*/

  log_gen_ta_type          gen_ta;    /* General Temporal Analyzer info.     */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  log_gen_ta_sup_ch_type gen_ta_sup;  /* Gen TA info w/supplemental channels */
#endif

#ifdef FEATURE_PLT
#error code not present
#endif

#if defined(FEATURE_RLP_LOGGING)
  log_rlp_frames_type      rlp_frames;/* RLP frames logging                  */
  log_rlp_stats_type       rlp_stats;  /* RLP3 statistics                    */
#endif

#ifdef FEATURE_IS2000
  log_fwd_frame_types_type fwd_frame_types;
                                       /* Forward Link Frame Types           */
  log_rev_frame_types_type rev_frame_types;
                                       /* Reverse Link Frame Types           */
  log_ffwd_pctrl_type      ffwd_pctrl; /* Fast Forward Power Control         */
  log_rev_pctrl_type       rev_pctrl;  /* Reverse Power Control              */
  log_srch_fing_type       srch_fing;  /* Searcher and Finger Info           */
  log_svc_config_type      svc_config; /* Service Configuration              */
  log_qpch_type            qpch;       /* QPCH logging                       */

#ifdef FEATURE_IS2000_TDSO
  log_stdso_type           stdso;      /* SImple Test Data Service Option    */
  log_ftdso_type           ftdso;      /* Full Test Data Service Option      */
#endif
#endif /* FEATURE_IS2000 */

#if defined( FEATURE_GPSONE )
#error code not present
#endif /* defined( FEATURE_GPSONE ) */

#ifdef FEATURE_FACTORY_TESTMODE
  log_ftm_type             ftm;  /* Factory test mode FER log */
#endif
}                
log_type;

#endif /* FEATURE_LOG_NAMING_COMPATIBILITY */
#endif /* LOG_DMSS_H */

