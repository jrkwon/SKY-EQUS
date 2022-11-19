#if (MODEL_ID == MODEL_A10) // jwpark 00.09.30
#include <nvi.h>
#else // (MODEL_ID == MODEL_A10)

#ifndef NVI_H
#define NVI_H
/*==========================================================================

         N V    T A S K    I N T E R N A L S    H E A D E R    F I L E

DESCRIPTION
  This header file contains all the internal definitions of NVM items.
  Items are passed to the NV task using an external interface definition
  provided in NV.H.  This file specifies how NVM items are stored internally
  within the NV EEPROM.  

Copyright (c) 1992,1993,1994,1995,1996,1997,1998,1999,2000,20001 by QUALCOMM 
Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/nvi.h_v   1.9   20 Feb 2001 17:05:44   rprenove  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/01   rp      Added support for NV_DS_DEFAULT_BAUDRATE_I under
                   FEATURE_DS_DEFAULT_BITRATE_NV.
                   Added support for NV_DIAG_DEFAULT_BAUDRATE_I under
                   FEATURE_DIAG_DEFAULT_BITRATE_NV.
                   Added support for NV_JCDMA_DS_F92_I under FEATURE_JCDMA_DS.
                   Removed support for obsolete items NV_DS_DEFAULT_BAUD_I
                   and NV_DIAG_DEFAULT_BAUD_I.
02/14/01   rp      Added support for NV_GPS1_LOCK_I under FEATURE_GPSONE.
                   Added support for NV_FTM_MODE_I under 
                   FEATURE_FACTORY_TESTMODE.
12/12/00   pg      Added support for testing in TREX.
                   Deleted support for testing in COMET.
                   Added support for NV_DATA_TRTL_ENABLED_I associated with
                   FEATURE_IS2000_R_SCH, and NV_AMPS_BACKSTOP_ENABLED_I
                   associated with FEATURE_SD20.
11/28/00   pg      Cleaned out obsolete item NV_AUTO_POWER_I.
11/20/00   pg      Turned on items NV_PCS_ENC_BTF_I and NV_CDMA_ENC_BTF_I for
                   all MSM3300 builds.
11/15/00   pg      Recovered item NV_CDMA_TX_LIM_VS_VOLT_I as it is needed in
                   FFA5000 build.
11/01/00   pg      Reverted the previous change.
10/31/00   pg      Replaced NV_FEATURE_TRIMODE_ITEMS with FEATURE_TRIMODE_ITEMS.
                   And made it a feature to be turned on by customers.
10/19/00   pg      Added support for NV_GPS1_GPS_RF_LOSS_I associated with
                   FEATURE_GPSONE.
10/05/00   pg      Merged TigerII and PP/PPG archive.
                   Added support for NV_SUBPCG_PA_WARMUP_DELAY_I associated
                   with FEATURE_SUBPCG_PA_WARMUP_DELAY.
09/25/00   pg      Added NV_FEATURE_TRIMODE_ITEMS to associated with 
                   NV_PCS_ENC_BTF_I to make sure that it can only accessed
                   in Trimode builds.
09/20/00   pg      Added support for invariant item NV_BD_ADDR_I associated
                   with FEATURE_BT.
09/18/00   pg      Added support for NV_PCS_ENC_BTF_I and NV_CDMA_ENC_BTF_I 
                   associated with FEATURE_ENC_BTF_IN_NV.
09/07/00   pg      Added support for NV_GPS1_GPS_RF_DELAY_I and 
                   NV_GPS1_CDMA_RF_DELAY_I associated with FEATURE_GPSONE.
09/01/00   pg      Changed FEATURE_RUIM_SMS to FEATURE_RUIM_SMSS.
08/25/00   pg      Added support for 
                   NV_RUIM_SMS_STATUS_I 
                   NV_PCS_TX_LIN_MASTER0_I          
                   NV_PCS_TX_LIN_MASTER1_I          
                   NV_PCS_TX_LIN_MASTER2_I 
                   NV_PCS_TX_LIN_MASTER3_I   
                   NV_PCS_PA_RANGE_OFFSETS_I
                   NV_PCS_TX_COMP0_I
                   NV_PCS_TX_COMP1_I
                   NV_PCS_TX_COMP2_I
                   NV_PCS_TX_COMP3_I
                   NV_DIAG_RESTART_CONFIG_I
07/27/00   pg      Fixed error feature name for MM items in nvi_items_enum_type.
07/25/00   rp      Changed FS_FILENAME_MAX_LENGTH to NV_FS_FILENAME_MAX_LENGTH
                   to remove dependency on FS_FILENAME_MAX_LENGTH.
07/06/00   pg      Added support for NV_MM_STORAGE_DEVICE_I, NV_UBROWSER_I.
07/05/00   pg      Added support for NV_MM_RINGER_FILE_I.
06/23/00   rp      Merged in support for all MSM3300 GPS One items under
                   FEATURE_GPSONE.
06/23/00   rp      Updated NV_FEATURE_CONFIG_LENGTH used in nvi_verno_ext_type
                   to track new features from 2 bytes to 4 bytes. 
06/15/00   kmh     Added SKTT RF Calibraion Item type definitions and constants
05/30/00   pg      Added support for NV_PRIMARY_DNS_I, NV_SECONDARY_DNS_I.
                   Added support for NV_DIAG_PORT_SELECT_I associated with
                   FEATURE_DIAG_PORT_SELECT.
05/05/00   pg      Cleaned out obsolete items:
                   NV_PPCNCH_I
                   NV_SPCNCH_I
                   NV_CDMA_TX_LIM_VS_VOLT_I
                   NV_WILDCARD_SID_I
                   NV_ROAM_MSG_I
                   NV_FULL_PREF_MODE_I
                   NV_CDMA_TX_LIMIT_I
                   NV_CDMA_RIPPLE_I
                   NV_CDMA_RX_OFFSET_I
                   NV_CDMA_RX_POWER_I
                   NV_CDMA_RX_ERROR_I
                   NV_CDMA_TX_SLOPE_1_I
                   NV_CDMA_TX_SLOPE_2_I
                   NV_CDMA_TX_ERROR_I
                   NV_VOC_GAIN_I
                   NV_POWER_CYCLES_I
                   NV_MESSAGE_ALERT_I
                   NV_AUTO_REDIAL_SHADOW_I
                   NV_IMSI_I
                   NV_RESTRICT_GLOBAL_I
                   NV_CALL_TIMER_I
                   NV_LIFE_TIMER_I
                   NV_SONY_ATTEN_1_I
                   NV_SONY_ATTEN_2_I
                   NV_SIO_PWRDWN_I
05/02/00   pg      Changed NV_TX_COMP_I from indexed item to individual items:
                   NV_TX_COMP0_I, NV_TX_COMP1_I, NV_TX_COMP2_I, NV_TX_COMP3_I.
04/25/00   pg      Added support for NV_PA_RANGE_OFFSETS_I, NV_TX_COMP_I 
                   associated with FEATURE_PA_RANGE_TEMP_FREQ_COMP.
                   Added support for SDAC items associated with 
                   FEATURE_STEREO_DAC.
                   Added support for multimedia items associated with 
                   FEATURE_MULTIMEDIA.
04/09/00   pg      Added support for NV_PREF_FOR_RC_I, NV_DS_DEFAULT_BAUD_I,
                   NV_DIAG_DEFAULT_BAUD_I, NV_SIO_DEV_MAP_MENU_ITEM_I,
                   NV_TTY_I.
03/18/00   rp      Fixed typo in NV_RUIM_CHV_SIZE. 
03/16/00   rp      Added support for R-UIM items based on 
                   FEATURE_RUIM_CHV_FROM_NV.
03/13/00   rp      Added Broadcast SMS items support for EEPROM based on 
                   FEATURE_BROADCAST_SMS. 
03/09/00   rp      Changed #ifdef FEATURE_DS_PAP to 
                   #if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
03/09/00   rp      Added support for RF calibration items associated with
                   FEATURE_PA_RANGE_DIG_COMP and FEATURE_4PAGE_TX_LINEARIZER.
03/09/00   rp      Updated type for NVI_IM_LEVEL1_I and NVI_IM_LEVEL2_I from
                   byte to int1.  
03/01/00   rp      Added limited support for nvi_verno_ext_type to track new 
                   feature definitions so that a rebuild will occur when
                   feature selection changes, this code will be obsolete
                   when the new NV meta compiler is available in the future.
02/22/00   rp      Added support for new RFR3100 items for Trimode.
02/22/00   rp      Changed type for NV_LNA_RANGE_12_OFFSET_I from word to 
                   int2.  
02/22/00   rp      Added support for new RFR3100 item NV_RX_AGC_MIN_11_I.
02/22/00   rp      Changed NAM_LOCK to be a per-NAM item.
                   Made NAM_LOCK and OTAPA_ENABLED support conditional
                   based on FEATURE_OTASP_OTAPA. 
02/16/00   rp      Added support for new RFR3100 items based on 
                   FEATURE_RFR3100.
01/31/00   rp      Added Broadcast SMS items support for NVIM based on 
                   FEATURE_BROADCAST_SMS. 
12/06/99   rp      Added Trimode items support based on
                   NV_FEATURE_TRIMODE_ITEMS.
10/08/99   pg      Added featurized support for nvi_rental_timer_type and 
                   nvi_rental_cnt_type.
10/04/99   rp      Added featurized support for NV_ROAM_MSG_I, NV_OTKSL_FLAG_I,
                   NV_SMS_REMINDER_TONE_I, NV_FULL_PREF_MODE_I,
                   NV_UP_BROWSER_WARN_I, NV_USER_ZONE_ENABLED_I, NV_USER_ZONE_I,
                   NV_AUTO_VOLUME_ENABLED_I, and NV_WILDCARD_SID_I for Digby.
08/16/99   rp      Removed support for NV_PAP_DATA_I. Added support for
                   NV_PAP_USER_ID_I and NV_PAP_PASSWORD_I.
08/13/99   rp      Added support for NV_PAP_DATA_I.
06/30/99   kmp     Changed FEATURE_NV_JSTD008 to FEATURE_NV_TMSI and removed
                   PACKED from the enums for the ARM 2.50 compiler.
03/31/99   snn     Deleted the references to Pragma Packs.
03/31/99   snn     ls made the following changes. I merged them here.  
                   Made NV_OTKSL_I universal.
                   Added NV_DATA_AUTO_PACKET_DETECTION_I item.
                   Made UP link access key a separate item.
03/11/99   snn     ck did the following changes in earlier version. I included
                   them here: 
                   Ifdefed PREF_VOICE_SO_I under FEATURE_EVRC_SO_MGMT
03/11/99   snn     ny did the following changes in earlier version. I included
                   them here:
                   Cleaned up the compilation to remove warnings/error messges.
                   Applied merge guidelines 3.0 to clean up the code
03/02/99   ls      Changed "int" to "word" in UP items.
02/26/99   ls      Added MDR items and IS-95B support.
02/11/99   ls      Merged OTASP 3.0.
02/05/99   ls      Changed featurization of NV_PREF_MODE_I.
02/03/99   ls      Introduced enums-as-words option.
02/02/99   ls      Added stack checker items.
01/14/99   ls      Added PDM tables support for ISS2 targets.
12/01/98   dlb     Added handover block support.        
11/29/98   ls      Removed obsolete page msg items and types.
11/21/98   ls      Added NV_OUTPUT_UI_KEYS_I.
10/28/98   dlb     Added support for ARM.
10/26/98   ls      Added NV_SRDA_ENABLED_I.
10/19/98   ls      Merged SSPR Enhancements.
10/01/98   ls      Changed EVRC item subtypes and added NV_SMS_AUTO_DELETE_I.
09/29/98   ls      Added EVRC, VRHFK, and new RF Cal items.
09/10/98   ls      Revived AUTO_ANSWER_SHADOW, added NV_SIO_PWRDWN_I.
08/27/98   ls      Added NV_AFAX_I, plus other configuration changes.
08/25/98   ls      Added CARRIER_INFO for module.
08/19/98   ls      Added BORSCHT items.
08/11/98   ls      Added NV_FULL_SYSTEM_PREF_I.
08/04/98   ls      Added rental timer enabled item support.
08/02/98   ls      Added rental counter item support.
07/27/98   ls      Added rental timer items.
07/14/98   ls      Merged remainder of SSPR-800  support under internal 
                   feature control.
07/06/98   ls      Partial merge of SSPR-800: added IS-683A PRL support.
06/22/98   ls      Added numAddr field to UP_LINK_INFO item.
06/12/98   ls      Added Unwired Planet alert items.
05/14/98   ls      Added NV_PDM1_VS_TEMP_I and NV_PDM2_VS_TEMP_I.
03/19/98   ls      Added PDM1, BELL_MODEM, and ALERTS_LVL_SHADOW.
01/20/98   ls      Added DTACO items, new RF Cal items, and version field in
                   roaming list.
12/16/97   ls      Added RF Cal pointer, elapsed_time item, and disconnect
                   timer.
11/26/97   ls      Removed several obsolete items.
11/05/97   ls      Added Unwired Planet support, version 2.
10/31/97   ls      Made OTKSL available to 2-NAM builds.
10/21/97   ls      Added data services items.
09/09/97   ls      Returned RSVD_MEM_SIZE to 0 after fixing nva_memavail().
08/29/97   ls      Removed extraneous nam field in dir_number_type.
08/03/97   ls      Made RSVD_MEM_SIZ one short block so nvdiv move works.
07/23/97   ls      Added initial support for Unwired Planet.
06/06/97   ls      Added voice_privacy_alert item for TGP.
06/03/97   ls      Implemented auto-detection of EEPROM configuration.
05/15/97   ls      Added support for OTASP-800.
04/30/97   ls      Added ISS2 dialing plan state table item.
04/22/97   ls      Featurized redial item.
04/02/97   ls      Added Clam-Shell Call Answer item, removed old MRU
                   table format.
02/13/97   ls      Added vibrator item, defined both MRU table formats.
01/17/97   ls      Changed MRU Table format.
01/03/97   ls      Featurized NV.
11/12/96   ls      Bumped all KLT_REVs due to addition of invariant area.
11/11/96   ls      Added valid flag to roaming_list_type.
10/11/96   ls      Remove unneeded nvi_sizeof_structure to save RAM.
10/08/96   ls      Shrunk NVI_RSVD_MEM_SIZ (no longer any double-buffered 
                   items in the dynamic pool).
10/03/96   ls      Added NV_TIMED_PREF_MODE_I.
09/17/96   ls      Change dial_type to contain all 32 digits.
09/11/96   ls      Remove unused items from TPS, save about 60 bytes.
08/29/96   ls      Mass migration of items to the fixed pool.
08/22/96   ls      Make FEATURE_JSTD008 available to ISS2 target.
08/06/96   ls      Added FEATURE_NV_ONE_NAM_RL_LARGE.
08/01/96   ls      Bumped all KLT_REVs due to CURR_NAM correction.
07/03/96   ls      Added NV_REDIAL_I item.
06/26/96   ls      Added Roaming List and MRU Table support.
06/17/96   ls      Added support for OTASP "commit" operation.
06/06/96   ls      Added target-specific KLT_REV.
05/30/96   ls      Added initial subset of J-STD-008 NAM parameters and 
                   extensions.
05/10/96   ls      Added PCS registration updates.
02/28/96   ls      Changed types of LNA_RANGE_RISE and LNA_RANGE_FALL to
                   nvi_int1_type.
02/27/96   ls      Added N1m nv item.
02/15/96   ls      Added RF Calibration items.
12/22/95   ras     pre-code review cleanup.
12/21/95   ras     split klt into klt as substructure in fixed pool
                   moved sec_code,lock_code,err_log to fixed pool. 
                   bumper KLT REV.
12/06/95   ras     moved fm_rssi and vbatt to the fixed pool, bumped klt rev.
11/30/95   ras     bumped KLT version for structure changes do to pack() fix.
11/22/95   ras     added NV_HW_CONFIG. Cleanup "pack()" usage
10/02/95   ras     added fm_agc,fsc2,fsc2_chksum,wdc, moved lcd to fixed item.
09/28/95   ras     bumped NV_VER_MAJ for err log fix
09/24/95   ras     changed nvi_err_log_type to match nv_err_log_type, bumped
                   dynamic pool rev.
09/11/95   ras     added fm_lna,life_timer_g,call_timer_g, pwr_up_tmr
                   made a_key a fixed item
09/08/95   ras     added support for ALARMS, STANDING_ALARMS, DIALING_PLAN,
                   ISD_STD_PASSWD, ISD_STD_RESTRICT
09/05/95   ras     NVI_SIZEOF, NVI_FSC_I, bumped klt rev. Tied klt and verno
                   together
08/29/95   ras     Bumped rev for sms bug fix
08/23/95   ras     added fixed items into enum, introduced klt version + bumped
08/08/95   ras     lint cleanup, IMSI, bad pwr down structures
07/20/95   ras     fixed iss2 build
07/17/95   ras     added sms support, Bumped rev. (0x0d)
06/26/95   ras     Bumped rev. for stack and dial. (0x0c)
06/15/95   ras     Bumped rev. for nva bug fix. (0x0b)
06/11/95   ras     added support for 20 new rf & ui items
06/06/95   ras     dynamic long stored  bug fix
05/29/95   ras     bumped rev to 0x09 since 0x08 revs are corrupted by ERR bug
05/22/95   ras     support for combined items and dynamic long stored #'s
                   went to rev 0x08
05/12/95   ras     rf item additions + NV_LCD_I - NV_BRIGHT_I
05/09/95   ras     Initial next generation nv checkin
01/18/94   jjw     Fixed error introduced earlier: corrupted fixed structure
                   Incremented Major version number
01/13/94   jjw     Fixed a buf where items added messed up the data in the
                   1st 18 or so bytes of actual NV data. Made the sizing of
                   the fixed structure compile-time configurable
01/06/94   jah     Changed watchdog iface to use dog_report().
12/17/93   jjw     Added DM address item
08/22/93   jjw     Added data services items
05/14/93   jjw     Added compile-time directive for Beta II target
05/10/93   jjw     Changed FM_RSSI to a min_max type
04/06/93   jjw     Major upgrade: Added numerous new items, changed numerous 
                   items, rolled version number to 2
02/05/93   jjw     Added new items for FM TX power levels & V battery min/max
10/29/92    ip     Modified items: err_log, sid_acq, auto_answer,
                   auto_redial, page_set, stack_idx.  Added items:
                   unit_id, name_nam, roam_timer, freq_adj (externally
                   visible); ESN_valid (externally invisible).
10/07/92    ip     Removed boot_err item.
08/06/92    ip     Fixed cast in NVI_SEG macro.
07/18/92    ip     Release after code review and initial unit test.
07/03/92    ip     Released for code review.
05/07/92    ip     Created file.

===========================================================================*/


/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "hw.h"
#include "nv.h"

#ifdef FEATURE_NV_HANDOVER_BLOCK
# include "nvi_hand.h"
#endif


/*===========================================================================
 
                           DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                              DATA                                       */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

extern rex_timer_type nv_write_timer;              /* NVM write timer. */
extern rex_timer_type nv_rpt_timer;                /* NV watchdog report. */
extern boolean        nvi_build_dynamic_pool_only; /* nvb flag for rebuild */


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                    NVM storage type notes.                              */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/*
   Each variable in NVM is either fixed, active, or double buffered.

   A fixed variable is not relocated in NVM.  Fixed variables which are
   accessible from external tasks have a pointer, while fixed variables
   which are used internally do not have a pointer to them.

   An active variable is of the form:
   1 byte:  Active flag:
            0 if not active (no valid information stored in the variable),
            NOT 0 if active.
   n bytes: The variable
   An active variable is pointer based and can be relocated in NVM.

   A double buffered variable is of the form:
   1 byte:  Active flag:
            0 if not active (no valid information stored in the variable),
            NOT 0 if active.
   n bytes: The variable
   A double buffered item is pointer based and can be relocated in NVM.
*/


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                      Constants definitions.                             */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* The KLT Revision number (NVI_KLT_REV) is set in the "feature defines"   */
/* section of "nv.h," and uniquely identifies a specific configuration     */
/* of NV.  It MUST be changed every time there are additions, deletions,   */
/* or changes to the size or ordering of existing items.  Note that, when  */
/* incrementing KLT_REV, a never-previously-used value must be chosen (to  */
/* insure the uniqueness of that specific NV configuration).               */

#define NVI_VERNO_MAJ (0x14 + NVI_KLT_REV)  /* increment to rebuild NV */

/* Number of bytes per NVM page.                                           */

#define NVI_BYTES_PER_PAGE 32

/* Number of bytes allowed per NVM write.  This number is best set to be   */
/* the same as page size above, since this yields the most efficient NVM   */
/* write.                                                                  */

#define NVI_BYTES_PER_WRITE 32

/* Number of attempts to make at writing to a location in NVM.             */
/* 1st generation targets                                                  */

#define NVI_WRITE_TRIES 2

/* Number of milliseconds between testing to see if a write to NVM         */
/* is successful.                                                          */
/* 1st generation targets                                                  */

#define NVI_CHECK_TIME 6

/* Number of times to check for a successful NVM write.                    */
/* 1st generation targets                                                  */

#define NVI_TMO_CNT 3

/* Number of times to retry writing an item with an active flag.           */
/* 1st generation targets                                                  */

#define NVI_ACT_RETRY 3

/* Number of times to retry writing a item which is double buffered.       */
/* 1st generation targets                                                  */

#define NVI_DOUBLE_RETRY 3

/* Number of times to retry writing an item during initialization.         */
/* 1st generation targets                                                  */

#define NVI_INIT_RETRY 3

/* The NVM physical address translation factor.  As an example, if NVM is  */
/* addressed on every other byte then this value is two (2).               */
/* 1st generation targets, address every other byte                        */

#define NVI_ADDRESS_FACTOR 1

/* Maximum number of times to try to write to NVM in the fatal error       */
/* condition.                                                              */
/* 1st generation targets                                                  */

#define NVI_FATAL_RETRY 5

#define NVI_WRITE_RETRY 1

/* The following define the addresses and data which are used in order to  */
/* unlock the write protect of the NVM before it can be written to.  There */
/* are three logical addresses (which are NOT mapped to physical address)  */
/* and a unique data byte to be written to each.                           */
/* 1st generation targets                                                  */

#define NV_PROT_ADDR_1 0x1555
#define NV_PROT_ADDR_2 0x0AAA
#define NV_PROT_ADDR_3 0x1555

#define NV_PROT_DATA_1 0xAA
#define NV_PROT_DATA_2 0x55
#define NV_PROT_DATA_3 0xA0

/* Possible NVRAM capacities */
#define NV_8K_VALUE   0x2000
#define NV_16K_VALUE  0x4000

/* An always-invalid NV address */
#define NV_INVALID_ADDRESS  0x7FFF

/* Initial size of the short allocation pool within the dynamic pool */
/* (Note that this is only a starting point, it can move dynamically */
/* if needed)                                                        */
#define NVI_SHORT_POOL_INITIAL_SIZE  32

/* 32 digits (16 bytes) worth of dial storage reserved */
#define NVI_MAX_DIAL_DIGITS_STORAGE 16
#define NVI_EXT_DIGIT_SIZ 8

/* NV_REDIAL_I is a special case: this is actually stored as the       */
/* last "hidden" element of the speed dial array (item NV_DIAL_I).     */
/* This constant is added to the internal size of the speed dial       */
/* array to provide for the REDIAL element when FEATURE_NV_REDIAL_ITEM */
/* is defined.                                                         */
#ifdef FEATURE_NV_REDIAL_ITEM
#define NVI_REDIAL_EXTENSIONS   1
#else
#error code not present
#endif

/* This constant specifies the number of bytes in the "adder array"     */
/* used by the rental timer item.                                       */
#define NVI_ADDER_ARRAY_SIZE  50

/* Maximum number of SMS entries. This number is based on three things. */
/*  1) Minimum allocation size is 32 bytes                              */
/*  2) SMS was supposed to be allocated no more than 700 bytes          */
/*  3) Product management asked for a few more entries in the table.    */

#define NVI_MAX_SMS_ADDR 44 // 24->44 huni Max Size 40

/* Bytes to reserve to insure that double buffered items can be allocated. */
/* This 'extra' space is required because the new item is allocated before */
/* the old item is free'd.  Currently, there are no double-buffered items  */
/* in the dynamic pool, so the value should be zero.                       */
#define NVI_RSVD_MEM_SIZ  0


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                              MACROS                                     */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Macro to kick the watchdog timer and set a timer to wake it again.      */

#define KICK_WATCHDOG() \
        dog_report(DOG_NV_RPT); \
        (void) rex_set_timer(&nv_rpt_timer, DOG_NV_RPT_TIME);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Macro to calculate position of structure member within nvi_fixed_type.  */
#define NV_FIXED_POS(field)  FPOS(nvi_fixed_type, field)

/* Macro to calculate position of structure member within nvi_fixed_upper8_*/
/* type (which defines the layout of fixed items in the upper 8K space)    */
#define NV_FIXED_UPPER8_POS(field)  FPOS(nvi_fixed_upper8_type, field)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Macro to calculate position of structure member within nvi_tps_type.    */
#define NV_TPS_POS(field)    FPOS(nvi_tps_type, field)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Macro to calculate position of structure member within nvi_msl_type.    */
#define NV_MSL_POS(field)    FPOS(nvi_msl_type, field)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Macro to calculate size of double buffered item given the size of the   */
/* variable included in the item.                                          */

#define NV_DOUBLE_SIZE(size)  ((size * 2) + 1)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Macro to word align location of variable.  All pointers of pointer      */
/* based NVM items are word aligned, which ensures that both pointer       */
/* bytes are contained on the same NVM page.                               */
/* This is only used for first generation targets.                         */

#define NV_WORD_ALIGN(loc) ((loc) + ((loc) % 2))

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Macro to page align location of variable.  This ensures that a given    */
/* item resides within one physical NVM page.  The macro checks that if    */
/* the value is 0 it is not changed (to avoid negative number arithmetic   */
/* on unsigned value).  The macro also ensures that a number which is      */
/* already on a page boundary is not changed.                              */
/* This is only used for first generation targets.                         */

#define NV_PAGE_ALIGN(loc) \
        (((loc) == 0) ? \
         (loc) : \
         ((loc) - 1) + NVI_BYTES_PER_PAGE - (((loc) - 1) % NVI_BYTES_PER_PAGE))

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Macro to turn NVM address to physical address.   This macro multiplies  */
/* its argument by the logical-to-physical translation factor.  As an      */
/* example, if physical NVM is addressed on every other byte then the      */
/* logical address is always multiplied by the address factor.             */
/* This is only used for first generation targets.                         */

#define NV_PHYSICAL(addr) ((addr) * (NVI_ADDRESS_FACTOR))


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Following are all the internal NV storage types.  The definitions       */
/* start with the pack(1) pragma, to force byte packing, and end with      */
/* the pack() pragma to return to default packing.  Types are defined      */
/* in the order of the external item enum type (NV_..._I from NV.H).       */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#ifdef FEATURE_TREX
#error code not present
#endif


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Item type definitions and constants.                                    */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* NV Extended Version Designator                                          */

#define  NV_FEATURE_CONFIG_LENGTH 4   /* 4 bytes (16 bits) of feature mask */

typedef PACKED struct {
  /* Number of bytes to follow */
  byte      length;
  /* Version Designator general revision number */
  word      general_rev;
  /* Variable-length feature mask bits */
  byte      feature_config[NV_FEATURE_CONFIG_LENGTH];
} nvi_verno_ext_type;

/* This is the value of the "length" field above */
#define  NVI_VERNO_EXT_LENGTH   (sizeof(nvi_verno_ext_type) - sizeof(byte))

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* CDMA channel numbers type.                                              */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      channel_a;           /* A carrier channel number */
  word      channel_b;           /* B carrier channel number */
} nvi_cdmach_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* CDMA SID_NID pair type.                                                 */

typedef PACKED struct {
  word      sid;
  word      nid;
} _nvi_sid_nid_type;

/* For NV_SID_NID_I item.                                                  */
typedef PACKED struct {
  boolean           active;      /* has this item ever been initialized? */
  _nvi_sid_nid_type value[NV_MAX_MINS][NV_MAX_SID_NID];
} nvi_sid_nid_type;

/* For NV_HOME_SID_NID_I item.                                             */
typedef PACKED struct {
  boolean           active;        /* has this item ever been initialized? */
  _nvi_sid_nid_type value[NV_MAX_HOME_SID_NID];
} nvi_home_sid_nid_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* CDMA SID_NID_LIST type.                                                 */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      sid;
  word      nid;
  word      block_or_sys;
  byte      band_class;
} nvi_sid_nid_list_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean  active;               /* has this item ever been initialized? */
  word     sys;
} nvi_sys_pref_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean  active;               /* has this item ever been initialized? */
  _nvi_sid_nid_type pair[NV_MAX_SID_NID_LOCK];
} nvi_sid_nid_lock_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean  active;               /* has this item ever been initialized? */
  boolean  enabled;
} nvi_enabled_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* CDMA ZONE_LIST type.                                                    */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      zone;
  word      sid;
  word      nid;   
  word      block_or_sys;
  byte      band_class;
} nvi_zone_list_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* CDMA distance based registration paremeters.                            */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  dword     base_lat_reg;
  dword     base_long_reg;
  word      reg_dist_reg;
} nvi_dist_reg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Speed dial numbers.  Dialed digits are stored internally packed as two  */
/* digits per byte, hence the size computation below.                      */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
#ifdef FEATURE_NV_FULL_DIAL_TYPE
  byte      status;              /* flag bits */
#endif
  byte      num_digits;
  byte      digits[NVI_MAX_DIAL_DIGITS_STORAGE];
  byte      letters[NV_MAX_LTRS];
} nvi_dial_type;


/* stack dial numbers.  Dialed digits are stored internally packed as two  */
/* digits per byte, hence the size computation below.                      */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
#ifdef FEATURE_NV_FULL_DIAL_TYPE
  byte      status;              /* flag bits */
  dword     time_stamp;          /* years to seconds */
#endif
  byte      num_digits;
  byte      digits[(NV_MAX_DIAL_DIGITS+1)/2];
  byte      letters[NV_MAX_LTRS];
#ifdef FEATURE_NV_EXTENDED_UI
  dword     duration;
#endif
} nvi_stdial_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold call stack index.  The array of bytes allows sorting the   */
/* call stack by order of dialing and resorting as it is being used.  The  */
/* secret field indicates if a secret number is in that stack location.    */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      index[NV_MAX_STACK_DIALS];   /* Index array */
  boolean   secret[NV_MAX_STACK_DIALS];  /* TRUE if a secret number */
} nvi_stack_idx_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for holding the dial string used for originating packet data calls */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      num_digits;                     /* Number of digits */
  byte      digits[NV_MAX_PKT_ORIG_DIGITS]; /* The digit array */
} nvi_data_pkt_orig_str_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify auto answer rings and enable/disable.                   */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  boolean   enable;
  byte      rings;
} nvi_auto_answer_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify auto redial rings and enable/disable.                   */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  boolean   enable;
  byte      rings;
} nvi_auto_redial_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify page answer rings and enable/disable.                   */

typedef PACKED struct {
  boolean   active;            /* has this item ever been initialized? */
  byte      enable;            /* TRUE if paging enabled */
  byte      rings;             /* Number of rings when to accept page */
} nvi_page_set_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* MAINTRSNsp.                                                             */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  boolean   order;
  byte      reason;
} nvi_maintrsn_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* LCKRSN_Psp.                                                             */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      order;
  byte      reason;
} nvi_lckrsn_p_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Error log.                                                              */

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      err_count;
  byte      file_name[NV_ERR_FILE_NAME_LEN];
  word      line_num;
  boolean   fatal;
} nvi_err_log_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  dword     value;
} nvi_dword_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  qword     value;
} nvi_qword_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value;
} nvi_byte_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  int1      value;
} nvi_int1_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  int2      value;
} nvi_int2_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  int16     value;
} nvi_int16_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  uint32    value;
} nvi_int32_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  uint32    value;
} nvi_uint32_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      value;
} nvi_word_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Enumerator items stored in NVM are stored as word, so that they have    */
/* the same format as external enum types, and can be copied directly      */
/* from internal to external format.                                       */

typedef  nvi_word_type  nvi_enum_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  boolean   value;
} nvi_boolean_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      value[NV_MAX_SID_LOCK];
} nvi_sid_lock_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      value[NV_MAX_SID_ACQ];
} nvi_sid_acq_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  dword     value[NV_MAX_MINS];
} nvi_min1_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      value[NV_MAX_MINS];
} nvi_min2_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte     value;
} nvi_imsi_11_12_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      value;
} nvi_imsi_mcc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      dir_number[NV_DIR_NUMB_SIZ]; 
} nvi_dir_number_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      n_digits;            /* Number of digits in dir_number */
  byte      dir_number[NV_DIR_NUMB_PCS_SIZ]; 
} nvi_dir_number_pcs_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  boolean   value[NV_MAX_MINS];
} nvi_min_bool_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  boolean   value[NV_MAX_MINS];
} nvi_mob_term_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_MAX_MINS];
} nvi_min_byte_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      value[NV_MAX_MINS];
} nvi_min_word_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_MAX_LTRS];
} nvi_name_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      wdc[NV_WDC_LTRS];
  byte      extra[3];            /* in case the wdc def. changes */
} nvi_wdc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      fsc[NV_FSC_SIZ];
} nvi_fsc_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  dword     fsc2;
} nvi_fsc2_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  dword     fsc2_chksum;
} nvi_fsc2_chksum_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean    active;              /* has this item ever been initialized? */
  dword      alarms;              /* DOT alarms */
} nvi_alarms_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the standing alarms for India DOT                          */

typedef PACKED struct {
  boolean    active;              /* has this item ever been initialized? */
  byte       isd_std_passwd[NV_ISD_STD_PASSWD_SIZ];     
} nvi_isd_std_passwd_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  qword     value; 
} nvi_ssd_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  qword     value;  
} nvi_a_key_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_LOCK_CODE_SIZE];
} nvi_lock_code_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_SEC_CODE_SIZE];
} nvi_sec_code_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean     active;              /* has this item ever been initialized? */
  byte        level_0;             /* power level 0 (max)*/
  byte        level_1;             /* power level 1 */
  byte        level_2;             /* power level 2 */
  byte        level_3;             /* power level 3 */
  byte        level_4;             /* power level 4 */
  byte        level_5;             /* power level 5 */
  byte        level_6;             /* power level 6 */
  byte        level_7;             /* power level 7 */
} nvi_fm_tx_pwr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_FREQ_ADJ_TABLE_SIZ];
} nvi_freq_adj_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_FR_TEMP_OFFSET_TABLE_SIZ];
} nvi_temp_offset_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      min;                      
  byte      max;
} nvi_minmax_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_HDET_TABLE_SIZ];
} nvi_pa_cur_ctl_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_FACTORY_INFO_SIZ];
} nvi_fact_info_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_PWR_TABLE_SIZ];
} nvi_pwr_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_FREQ_TABLE_SIZ];
} nvi_freq_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  int1      value[NV_FREQ_TABLE_SIZ];
} nvi_freq_int1_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_TEMP_TABLE_SIZ];
} nvi_temp_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  int1      value[NV_TEMP_TABLE_SIZ];
} nvi_temp_int1_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_CDMA_RX_LIN_SIZ];
} nvi_cdma_rx_lin_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef NV_FEATURE_TRIMODE_ITEMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_VOLT_TABLE_SIZ];
} nvi_volt_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_CDMA_TX_LIN_MASTER_SIZ];
} nvi_tx_lin_master_slp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef NV_FEATURE_TRIMODE_ITEMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_CDMA_EXP_HDET_VS_AGC_SIZ];
} nvi_exp_hdet_vs_agc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef NV_FEATURE_TRIMODE_ITEMS
#error code not present
#endif
               
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_CDMA_ERR_SLP_VS_HDET_SIZ];
} nvi_err_slp_vs_hdet_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_ADJ_VS_FREQ_SIZ];
} nvi_adj_vs_freq_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  byte      value[NV_ADJ_VS_TEMP_SIZ];
} nvi_adj_vs_temp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_ITEM_MGR
typedef PACKED struct {
  boolean   active;             /* has this item ever been initialized? */
  dword     ticks;              /* number of ticks                      */
} nvi_rental_timer_type;

#else
/* Special structure for the rental timer item                             */
typedef PACKED struct {
  boolean   active1;    /* active flag for "ticks1"                        */
  dword     ticks1;     /* number of ticks (1st of 2 double-buffered)      */
  boolean   active2;    /* active flag for "ticks2"                        */
  dword     ticks2;     /* number of ticks (2nd of 2 double-buffered)      */
  byte      adder[NVI_ADDER_ARRAY_SIZE];  /* "adder" byte array            */
  boolean   active_p;   /* active flag for "porch"                         */
  dword     porch;      /* for complete write, holds new value temporarily */
} nvi_rental_timer_type;

 #endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_ITEM_MGR
typedef PACKED struct {
  boolean   active;             /* has this item even been initialized */
  dword     cnt;                /* count value                         */
} nvi_rental_cnt_type;

#else
/* Special structure for the rental counter item (double-buffered)         */
typedef PACKED struct {
  boolean   active1;    /* active flag for "cnt1"                          */
  dword     cnt1;       /* count value (1st of 2 double-buffered)          */
  boolean   active2;    /* active flag for "cnt2"                          */
  dword     cnt2;       /* count value (2nd of 2 double-buffered)          */
} nvi_rental_cnt_type;

#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  byte        address;               /* sms entry [0..23] */
  byte        status;                /* status mask */
  byte        protect;
  byte        user_data_length;
  byte        call_back_length; 
  qword       param_mask;
  word        length;                /* bytes in 'data', [8..325] */
} nvi_sms_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      prl_version;    
  word      size;
  boolean   valid;
  byte      roaming_list[NV_ROAMING_LIST_MAX_SIZE];
} nvi_roaming_list_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* This type is only a "spacer" for the roaming list in the fixed pool */
typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  word      prl_version;    
  word      size;
  boolean   valid;
  byte      roaming_list[NV_ROAMING_LIST_MAX_8K_SIZE];
} nvi_roaming_list_fp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* An MRU2 Table entry */
typedef PACKED struct {
  byte      mode;
  byte      band_class;
  word      cs;
} nvi_mru2_table_entry_type;

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  nvi_mru2_table_entry_type  entry[NV_MRU2_TABLE_SIZE];
} nvi_mru2_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean  active;              /* has this item ever been initialized? */
  byte     restriction_level[NV_DIAL_RESTRICT_LVLS_SIZE];
} nvi_dial_restrict_lvls_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean  active;                 /* has this item ever been initialized? */
  byte     next_state[NV_DIAL_STATE_TABLE_NEXT_STATES];
  byte     timeout;
  byte     restriction_level;
  byte     remaining_digits;
} nvi_dial_state_table_elem_type;

typedef PACKED struct {       
  boolean  active;                 /* has this item ever been initialized? */
  word     length;       /* Number of "elem_type" entries currently stored */
} nvi_dial_state_table_len_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  byte      ip[NV_IP_ADDRESSES_PER_ENTRY];
  word      port;
} nvi_ip_address_type;

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  word      numAddr;                      /* Number of valid ip address    */
  nvi_ip_address_type  addr[NV_IP_ADDRESS_TABLE_SIZE];
  byte      key_length;                   /* Length of key field           */
} nvi_up_link_info_type;

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      key[NV_UP_MAX_KEY_SIZE];      /* Access key                    */
} nvi_up_key_type;

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      curr_gateway;                 /* Last active IP address        */
  byte      alert_state;                  /* Last alert state              */
  byte      disconnect_timer;             /* Disconnect time-out value     */
  byte      perm_storage[NV_UP_MAX_PERM_STORE_SIZE];  /* permanent storage */
} nvi_up_parms_type;

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      alert_name[NV_UP_ALERT_NAME_SIZE];
  byte      alert_status;
  word      alert_id;
} nvi_up_alert_record_type;

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      cache[NV_UP_MAX_CACHE_SIZE];  /* The UP cache                  */
} nvi_up_cache_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      data[NV_CARRIER_INFO_ELEMENT_SIZE];
} nvi_carrier_info_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_EVRC_SO_MGMT
typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  boolean   evrc_capability_enabled;
  word      home_page_voice_so;
  word      home_orig_voice_so;
  word      roam_orig_voice_so;
} nvi_pref_voice_so_type;
#else
typedef PACKED struct {
  byte      items[8];                         
} nvi_rsvd_item_1_type;
#endif

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      data[NV_SI_TEST_DATA_SIZE];
} nvi_si_test_data_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_UBROWSER
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      data[NV_PN_ID_TABLE_SIZE];
} nvi_pn_id_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_USER_ZONE
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      user_id_len;                           /* Length of user_id */
  byte      user_id[NV_MAX_PAP_USER_ID_LENGTH];    /* The user_id */
} nvi_pap_user_id_type;
#endif

typedef PACKED struct {
  boolean   active;                /* has this item ever been initialized? */
  byte      password_len;                          /* Length of password */
  byte      password[NV_MAX_PAP_PASSWORD_LENGTH];  /* The password */
} nvi_pap_password_type;

#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
typedef PACKED struct {
  boolean	active;
  byte		tempsense[3];
} nvi_ref_tempsense_type;

typedef PACKED struct {
//  byte		ref_txslp;
  word		ref_txslp;
  word		power[3];
} _freq_pwr_type;

typedef PACKED struct {
  boolean	active;
  _freq_pwr_type  freq_pwr[NV_FREQ_TABLE_SIZ];
} nvi_freq_pwr_table_type;

typedef PACKED struct {
	boolean active;
	byte ref_txslp;
	byte min;
	byte max;
} nvi_pminmax_type;
#endif	// #ifdef FEATURE_SKTT_CAL

/*  NV_VOICE_INFO_I   00.12.19 hjcho  */
typedef PACKED struct {
    boolean   active;
	dword	  remained_byte;
	word	  user_voice_l; 
	word	  remained_time;
	byte	  total_rec_num;
	byte	  new_ans_num;
	boolean   is_user_voice;
	byte	  user_voice_fn;
} nvi_voice_info_type;


/*   NV_FN_SEQUENCE_I 00.12.19 hjcho */
typedef PACKED struct {
    boolean     active;
	word		Time;			// time (sec)
	byte		Fn;				// file number
	byte		Type;			// file type
} nvi_fn_sequence_type;


/*   NV_MULTI_SID_I  01.09.17 hjcho */
typedef PACKED struct {
    boolean     active;
    byte        nSIDNum;
    byte        nSelIndex; 
    word        awChList[NV_MAX_CH_LIST_SIZE];
    word        awSIDList[NV_MAX_CH_LIST_SIZE];
} nvi_multi_sid_type;


/* reo 00-07-27 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)  || (MODEL_ID == MODEL_B2)
/*----------------받은 문자 메시지 NV 저장 format------------------------*/
#if 0
typedef PACKED struct {
    boolean             active;

    byte                unUsed;                         //Ox01:Used/UnUsed
    word                nID;                            //Message ID : USED IN CM
    boolean             bCheckTime;                      //Timefield가 있는지 조사 
    char                Time[4];                        //Receive time                        
    byte                nAttr;                          /*
                                                            0x01:Read/UnRead
                                                            0x02:Protect/UnProtect
                                                            0x04:Duplicate/Single
                                                            initial:UnRead/UnProtect/Single                                                            
                                                        */
    byte                unDuplicateNum;
    byte                unCallBackLen;
    byte                szCallBack[MAX_DIAL_DIGIT+1];
    byte                unDataLen;
    byte                szData[MAX_SMS_MSG_LEN+1];
}nvi_smsrcv_type;
#endif
/*---------보낼 메시지/ 보낸 메시지 NV 저장 format-------------------------*/
typedef PACKED struct {
    boolean             active;

    byte                unUsed;                             //0x01: Used/UnUsed                                                                                                               
    char                Time[4];                            //SMS Made time
    byte                unMultiSendNum;                     //Number of Multi-Send
/* below field later needed modified */
/* initial value -1 :   resend message phone numbers */
    char                CallDigit[MAX_NUM_MULTI_SEND][MAX_DIAL_DIGIT];

    byte                szData[MAX_SMS_MSG_LEN+1];           //SMS Message
}nvi_smssend_type;

/*-----------------받은 음성 메시지 NV 저장 format-------------------------*/
typedef PACKED struct {
    boolean             active;

    byte                unUsed;
    word                nID;
    char                Time[4];
    byte                unAttr;                         //0x01: Read/UnRead
    byte                unDialLen;
    byte                szDial[MAX_DIAL_DIGIT+1];
}nvi_vmsrcv_type; 

/*------------------------Mybell NV 저장 format----------------------------*/
typedef PACKED struct {
    boolean             active;

    byte                unUsed;
    char                Time[4];
    byte                unDataLen;
    byte                szData[MAX_MYBELL_DATA];
}nvi_mybell_type;

/*------------------------상용구 NV 저장 format----------------------------*/
typedef PACKED struct {
    boolean             active;

    byte                unUsed;
    byte                szLikePhrase[MAX_LIKEPHRASE_LEN+1];
}nvi_likephrase_type;
/*------------------------SMS Option NV 저장 format------------------------*/
typedef PACKED struct {
    byte                unRcvListOrder;
    byte                unRingType;
    byte                unSMSBellType;
}nvi_smsoption_type;

#endif /* (EQUS_MODEL_ID == EQUS_B1) || (EQUS_MODEL_ID == EQUS_B0) */
/* <-- */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if (defined FEATURE_PA_RANGE_DIG_COMP || defined FEATURE_PA_RANGE_TEMP_FREQ_COMP)
typedef PACKED struct
{
  boolean   active;                /* has this item ever been initialized? */
  nv_pa_offset_type pa_offsets[NV_NUM_PA_RANGES];
} nvi_pa_offset_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_4PAGE_TX_LINEARIZER 
typedef PACKED struct
{
  boolean   active;                /* has this item ever been initialized? */
  int2      offset;
  byte      slope[NV_CDMA_TX_LIN_MASTER_SIZ];
} nvi_tx_linearizer_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_BROADCAST_SMS
/* Definitions for Broadcast SMS items.                                    */
typedef PACKED struct
{
  boolean   active;                /* has this item ever been initialized? */
  boolean   active_service;        /* for delete operation */
  uint2     service;
  uint1     language;
  boolean   selected;
  uint1     priority;
  char      label[NV_SMS_BC_SRV_LABEL_SIZE]; /* a UI text label for each service/language pair */
} nvi_sms_bc_service_table_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_RUIM_CHV_FROM_NV

typedef PACKED struct
{
  boolean  active;                /* has this item ever been initialized? */
  byte     digits[NV_RUIM_CHV_SIZE]; 
} nvi_ruim_chv_type;

#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP

typedef PACKED struct {
  boolean   active;              /* has this item ever been initialized? */
  int2      value[NV_NUM_PA_RANGES];
} nvi_pa_range_offsets_type;

typedef PACKED struct
{
  boolean  active;                /* has this item ever been initialized? */
  int1     nv_cdma_tx_lin_vs_temp[NV_TEMP_TABLE_SIZ];
  int1     nv_cdma_tx_slp_vs_temp[NV_TEMP_TABLE_SIZ];
  int1     nv_cdma_tx_comp_vs_freq[NV_FREQ_TABLE_SIZ];
} nvi_tx_comp_type;

#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_GPSONE
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_RUIM_SMSS
typedef PACKED struct
{
  boolean active;
  nv_ruim_sms_status_type value;
} nvi_ruim_sms_status_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_DIAG_RESTART_CONFIG
typedef PACKED struct
{
  boolean active;
  nv_diag_restart_config_type value;
} nvi_diag_restart_config_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct
{
  boolean active;
  nv_bd_addr_type value;
  #ifndef FEATURE_NV_ITEM_MGR
  dword chksum;
  #endif
} nvi_bd_addr_type;


// 
typedef	PACKED struct {
  boolean	active;
  dword phoneTAG;
  char  processTAG[2];
  dword SN;
  dword OEM_SN;
  char  LineCode[10][2];
  char  ModelCode[3];
  char  ColorCode[2];
  char  FirstDate[12];
  char  LastDate[12];
} nvi_phoneTAG_type;


/*=========================================================================*/
/*                                                                         */
/* Enumerators of items to access.  Each command to the NV task specifies  */
/* the command type with an emumerator from the list below.                */
/* This enum is used to identify items as they are internally stored.      */
/* nvb.c, nvr.c, nvw.c, and nvmio.c convert from the external enum to the  */
/* internal enum. The internal enum buys us several memory savings within  */
/* the nv device. First, we can have target specific builds, only          */
/* containing pointers in the tps relevant to the target being build,      */
/* without interfering with the diagnostic monitor's target independent use*/
/* of the enum. Second, the internal enum allows for grouping items        */
/* together into more memory efficent arrangements, without external tasks */
/* needing to be modified.                                                 */

typedef enum {

/*-------------------------------------------------------------------------*/

/* Electronic Serial Number.                                               */

  NVI_ESN_I = 0,         /* Electronic Serial Number */
  NVI_ESN_CHKSUM_I,      /* Electronic Serial Number checksum */

/*-------------------------------------------------------------------------*/

/* NV version numbers.                                                     */

  NVI_VERNO_MAJ_I,         /* Major version number */
  NVI_VERNO_MIN_I,         /* Minor version number */

/*-------------------------------------------------------------------------*/

/* Permanent physical station configuration parameters.                    */

  NVI_SLOT_CYCLE_INDEX_I,  /* Slot cycle index */
  NVI_MOB_CAI_REV_I,       /* Mobile CAI revision number */

/*-------------------------------------------------------------------------*/

/* Permanent general NAM items.  Each of these is associated with a        */
/* particular NAM (there are up to four NAMs per unit).  The NAM id is     */
/* specified in the request.  Certain analog parameters are truly          */
/* associated with the MIN, but since there is only one MIN per NAM        */
/* in analog mode they are defined as a NAM item.                          */

  NVI_PREF_MODE_I,          /* Digital/Analog mode preference */

#if (defined(NV_FEATURE_SSPR_ENHANCEMENTS) || defined(FEATURE_NV_ANALOG))
  NVI_PCDMACH_I,            /* Primary CDMA channel */
  NVI_SCDMACH_I,            /* Secondary CDMA channel */
#endif


#ifdef FEATURE_NV_ANALOG
  NVI_CDMA_PREF_SERV_I,     /* CDMA preferred serving system (A/B) */
  NVI_ANALOG_PREF_SERV_I,   /* Analog preferred serving system (A/B) */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* The NAM parameters in the block below are protected by the NAM          */
/* checksum and can only be programmed via service programming.            */
/* The NAM parameters in the block above are not protected by checksum     */
/* and may be changed by the user 'on the fly'.                            */

  NVI_CDMA_SID_LOCK_I,      /* CDMA SID(s) to lockout */
  NVI_CDMA_SID_ACQ_I,       /* CDMA SID to acquire */
  NVI_ANALOG_SID_LOCK_I,    /* ANALOG SID(s) to lockout */
  NVI_ANALOG_SID_ACQ_I,     /* ANALOG SID to acquire */
  NVI_ANALOG_FIRSTCHP_I,    /* Analog FIRSTCHPp */
  NVI_ANALOG_HOME_SID_I,    /* Analog HOME_SIDp */
  NVI_ANALOG_REG_I,         /* Analog registration setting */ 
#endif  /* FEATURE_NV_ANALOG */

  NVI_NAM_CHKSUM_I,         /* NAM checksum */

/*-------------------------------------------------------------------------*/

/* Authentication NAM items.  Each of these is associated with a NAM       */
/* authentication (there are up to four NAMs per unit).  The NAM id is     */
/* specified in the request.                                               */

  NVI_A_KEY_I,              /* Authentication A key */
  NVI_SSD_A_I,              /* SSD_Asp */
  NVI_SSD_B_I,              /* SSD_Bsp */ 
  NVI_COUNT_I,              /* COUNTsp */

/*-------------------------------------------------------------------------*/

/* MIN items.  Each of these is associated with a particular MIN within    */
/* the NAM (there are now up to 2 MINs per NAM).  The MIN id itself is     */
/* specified in the request.  When operating in analog mode the first MIN  */
/* is the one and only meaningfull one.                                    */

  NVI_MIN1_I,               /* MIN1p */
  NVI_MIN2_I,               /* MIN2p */

#ifdef NV_FEATURE_IMSI_T_ITEMS
  NVI_IMSI_T_S1_I,          /* True IMSI - MIN1                            */
  NVI_IMSI_T_S2_I,          /* True IMSI - MIN2                            */
  NVI_IMSI_T_MCC_I,         /* True IMSI - MCC                             */
  NVI_IMSI_T_11_12_I,       /* True IMSI - 11th & 12th digits              */
  NVI_IMSI_T_ADDR_NUM_I,    /* True IMSI - Addr num                        */
#endif

  NVI_MOB_TERM_HOME_I,      /* CDMA MOB_TERM_HOMEp registration flag       */
  NVI_MOB_TERM_FOR_SID_I,   /* CDMA MOB_TERM_FOR_SIDp registration flag    */
  NVI_MOB_TERM_FOR_NID_I,   /* CDMA MOB_TERM_FOR_NIDp registration flag    */
  NVI_ACCOLC_I,             /* ACCOLCp                                     */

#ifdef NV_FEATURE_SSPR_ENHANCEMENTS
  NVI_HOME_SID_NID_I,       /* CDMA home SID/NID pairs                     */
#else
  NVI_SID_NID_I,            /* CDMA SID/NID pairs                          */
#endif
  NVI_MIN_CHKSUM_I,         /* MIN checksum                                */

/*-------------------------------------------------------------------------*/

/* Operational NAM settings.                                               */

  NVI_NAME_NAM_I,           /* A user selectable name for each NAM */

/*-------------------------------------------------------------------------*/

/* Semi-permanent CDMA registration and channel parameters.                */

  NVI_SID_NID_LIST_I,       /* SID_NID_LISTsp */

/*-------------------------------------------------------------------------*/

/* Timers, each associated with a specific NAM.                            */

  NVI_AIR_TIMER_I,       /* Air time (resettable cummulative call timer) */
  NVI_ROAM_TIMER_I,      /* Roam time (resettable cummulative roam timer) */

/*-------------------------------------------------------------------------*/

/* Run timer, independent of NAM.                                          */

  NVI_RUN_TIMER_I,       /* Run timer (time hardware has been running) */

/*-------------------------------------------------------------------------*/

/* Memory (speed) dial numbers.                                            */

  NVI_DIAL_I,            /* Speed dial number */
  NVI_STACK_I,           /* Call stack number */
  NVI_STACK_IDX_I,       /* Call stack last number index */

/*-------------------------------------------------------------------------*/

/* Telephone pages.                                                        */

  NVI_PAGE_SET_I,        /* Page setting */

/*-------------------------------------------------------------------------*/

/* Various phone settings.                                                 */

  NVI_BANNER_I,          /* Idle banner to display */

/*-------------------------------------------------------------------------*/

/* Locks and restrictions values.                                          */

  NVI_LOCK_CODE_I,       /* Lock code string */
  NVI_SEC_CODE_I,        /* Security code */

/*-------------------------------------------------------------------------*/

/* Error log.                                                              */

  NVI_ERR_LOG_I,         /* Error log */

/*-------------------------------------------------------------------------*/

/* Miscellaneous items.                                                    */

/* V Battery Min/Max.   (Portable Only Item)                               */

  NVI_VBATT_I,           /* V battery regulator array for min/max */

/*-------------------------------------------------------------------------*/

/* Analog (FM) Transmit power levels                                       */

  NVI_FM_TX_PWR_I,       /* Analog TX power level array */

/*-------------------------------------------------------------------------*/

/* Frequency/temperature offset table item                                 */

  NVI_FR_TEMP_OFFSET_I,   

/*-------------------------------------------------------------------------*/

/* DM Port Mode (Mobiles only) NOTE: Use to be NVI_EXT_PORT_MODE_I          */

  NVI_DM_IO_MODE_I,       /* External port (I/O) mode for DM service   */

/*-------------------------------------------------------------------------*/

  NVI_FM_RSSI_I,               /* Analog RSSI adjustment                    */

/*-------------------------------------------------------------------------*/

/* Spare items (2) for developer                                           */

  NVI_SPARE_1_I,
  NVI_SPARE_2_I,

/*-------------------------------------------------------------------------*/

/* Data Services items                                                     */

  NVI_DATA_IO_MODE_I,          /* External port (I/O) mode for Data service */

/*-------------------------------------------------------------------------*/

/* Factory free format test buffer                                         */
#ifdef FEATURE_NV_FACTORY_INFO
  NVI_FACTORY_INFO_I,
#endif

/*-------------------------------------------------------------------------*/

/* Additional Sony Attenuation values                                      */

#ifdef FEATURE_NV_ATTENUATION
  NVI_SONY_ATTEN_3_I,
  NVI_SONY_ATTEN_4_I,
  NVI_SONY_ATTEN_5_I,
#endif


/*-------------------------------------------------------------------------*/

/* DM address item (for multi-drop HDLC                                    */

  NVI_DM_ADDR_I,

#ifdef FEATURE_NV_RF_CAL
  NVI_FM_AGC_SET_VS_PWR_I, 
  NVI_FM_AGC_SET_VS_FREQ_I,
  NVI_FM_AGC_SET_VS_TEMP_I,

  NVI_FM_EXP_HDET_VS_PWR_I, 
  NVI_FM_ERR_SLP_VS_PWR_I, 
  NVI_FM_FREQ_SENSE_GAIN_I, 
  NVI_FM_AGC_ADJ_VS_FREQ_I,
  NVI_FM_LNA_CTL_I,
#endif

  NVI_CDMA_RX_LIN_OFF_0_I, 
  NVI_CDMA_RX_LIN_SLP_I,   

  NVI_CDMA_RX_COMP_VS_FREQ_I,   
  NVI_CDMA_TX_COMP_VS_FREQ_I,  

  NVI_CDMA_TX_LIN_MASTER_OFF_0_I,  
  NVI_CDMA_TX_LIN_MASTER_SLP_I,   
  NVI_CDMA_TX_LIN_VS_TEMP_I,     
  NVI_CDMA_TX_LIN_VS_VOLT_I,    

  NVI_CDMA_TX_LIM_VS_TEMP_I,   
  NVI_CDMA_TX_LIM_VS_FREQ_I, 
  NVI_CDMA_EXP_HDET_VS_AGC_I,
  NVI_CDMA_ERR_SLP_VS_HDET_I,    

  NVI_CDMA_RX_LIN_VS_TEMP_I,
  NVI_CDMA_ADJ_FACTOR_I,
  NVI_CDMA_RX_SLP_VS_TEMP_I,
  NVI_CDMA_TX_SLP_VS_TEMP_I,

#ifdef NV_FEATURE_PDM_TABLES
  NVI_PDM1_VS_TEMP_I,
  NVI_PDM2_VS_TEMP_I,
  NVI_PDM1_VS_FREQ_I,
  NVI_PDM2_VS_FREQ_I,
#else
  NVI_PA_RANGE_VS_TEMP_I,
#endif

  NVI_FM_EXP_HDET_VS_TEMP_I,

  NVI_THERM_I,                  
  NVI_VBATT_PA_I,              
  NVI_HDET_OFF_I,             
  NVI_HDET_SPN_I,            
  NVI_LCD_I,

  NVI_RF_CONFIG_I,
  NVI_R1_RISE_I,
  NVI_R1_FALL_I,
  NVI_R2_RISE_I,
  NVI_R2_FALL_I,
  NVI_R3_RISE_I,
  NVI_R3_FALL_I,

  NVI_PA_RANGE_STEP_CAL_I,
  NVI_LNA_RANGE_POL_I,
  NVI_LNA_RANGE_RISE_I,
  NVI_LNA_RANGE_FALL_I,
  NVI_LNA_RANGE_OFFSET_I,

  NVI_SMS_00_I,
  NVI_SMS_01_I,
  NVI_SMS_02_I,
  NVI_SMS_03_I,
  NVI_SMS_04_I,
  NVI_SMS_05_I,
  NVI_SMS_06_I,
  NVI_SMS_07_I,
  NVI_SMS_08_I,
  NVI_SMS_09_I,
  NVI_SMS_10_I,
  NVI_SMS_11_I,
  NVI_SMS_12_I,
  NVI_SMS_13_I,
  NVI_SMS_14_I,
  NVI_SMS_15_I,
  NVI_SMS_16_I,
  NVI_SMS_17_I,
  NVI_SMS_18_I,
  NVI_SMS_19_I,
  NVI_SMS_20_I,
  NVI_SMS_21_I,
  NVI_SMS_22_I,
  NVI_SMS_23_I, 
  NVI_SMS_24_I, // 24~43 add huni Max Size 40 begin
  NVI_SMS_25_I,
  NVI_SMS_26_I,
  NVI_SMS_27_I,
  NVI_SMS_28_I,
  NVI_SMS_29_I,
  NVI_SMS_30_I,
  NVI_SMS_31_I,
  NVI_SMS_32_I,
  NVI_SMS_33_I,
  NVI_SMS_34_I,
  NVI_SMS_35_I,
  NVI_SMS_36_I,
  NVI_SMS_37_I,
  NVI_SMS_38_I,
  NVI_SMS_39_I,
  NVI_SMS_40_I,
  NVI_SMS_41_I,
  NVI_SMS_42_I,
  NVI_SMS_43_I, // 24~43 add huni Max Size 40 end

  NVI_IMSI_MCC_I,
  NVI_IMSI_11_12_I,
  NVI_DIR_NUMBER_I,
  NVI_FSC_I,
  NVI_N1M_I,
  NVI_ROAM_CNT_I,
  NVI_AIR_CNT_I,

#ifdef FEATURE_NV_ALARMS
#error code not present
#endif

  NVI_FM_AGC_I,
  NVI_FSC2_I,
  NVI_FSC2_CHKSUM_I,
  NVI_WDC_I,
  NVI_HW_CONFIG_I,

#ifdef FEATURE_NV_TMSI
  NVI_ASSIGNING_TMSI_ZONE_LEN_I,
  NVI_ASSIGNING_TMSI_ZONE_I,
  NVI_TMSI_CODE_I,
  NVI_TMSI_EXP_I,
#endif /* FEATURE_NV_TMSI */
  
  NVI_OTKSL_I,  
  NVI_IMSI_ADDR_NUM_I,
  NVI_DIR_NUMBER_PCS_I,

#ifdef NV_FEATURE_PRL_ITEMS
  NVI_MRU2_TABLE_I,
  NVI_PRL_ENABLED_I,
  #ifdef NV_FEATURE_ROAMING_LIST_683
    NVI_ROAMING_LIST_683_I,
  #else  
    NVI_ROAMING_LIST_I,
  #endif
#endif

  NVI_SCM_I,
  NVI_MOB_FIRM_REV_I,
  NVI_MOB_MODEL_I,
  NVI_CONFIG_CHKSUM_I,
  NVI_CURR_NAM_I,
  NVI_AUTO_NAM_I,
  NVI_NXTREG_I,
  NVI_LSTSID_I,
  NVI_LOCAID_I,
  NVI_PUREG_I,
  NVI_ZONE_LIST_I,
  NVI_DIST_REG_I,
  NVI_EAR_LVL_I,
  NVI_SPEAKER_LVL_I,
  NVI_RINGER_LVL_I,
  NVI_BEEP_LVL_I,
  NVI_CALL_BEEP_I,
  NVI_CONT_KEY_DTMF_I,
  NVI_CONT_STR_DTMF_I,
  NVI_SVC_AREA_ALERT_I,
  NVI_CALL_FADE_ALERT_I,
  NVI_AUTO_ANSWER_I,
  NVI_AUTO_REDIAL_I,
  NVI_AUTO_HYPHEN_I,
  NVI_BACK_LIGHT_I,
  NVI_AUTO_MUTE_I,
  NVI_MAINTRSN_I,
  NVI_LCKRSN_P_I,
  NVI_LOCK_I,
  NVI_AUTO_LOCK_I,
  NVI_CALL_RSTRC_I,
  NVI_HORN_ALERT_I,
  NVI_DATA_SRVC_STATE_I,
  NVI_IDLE_DATA_TIMEOUT_I,
  NVI_MAX_TX_ADJ_I,
  NVI_INI_MUTE_I,
  NVI_CDMA_PN_MASK_I,
  NVI_SEND_TIMEOUT_I,
  NVI_LIFE_TIMER_G_I,
  NVI_CALL_TIMER_G_I,
  NVI_LAST_CDMACH_I,
  NVI_ONETOUCH_DIAL_I,
  NVI_ALERTS_LVL_I,
  NVI_ALERTS_LVL_SHADOW_I,
  NVI_RINGER_LVL_SHADOW_I,
  NVI_BEEP_LVL_SHADOW_I,
  NVI_EAR_LVL_SHADOW_I,
  NVI_TIME_SHOW_I,
  NVI_SEND_PIN_I,
  NVI_PWR_DWN_CNT_I,
  NVI_LIFE_CNT_I,
  NVI_VOICE_PRIV_I,

#ifdef NV_FEATURE_ANS_SHADOW_ITEMS
#error code not present
#endif

#ifdef FEATURE_NV_ANALOG
  NVI_TIMED_PREF_MODE_I,
#endif
 
#ifdef FEATURE_NV_EXTENDED_UI
  NVI_ANY_KEY_ANSWER_I,
  NVI_BACK_LIGHT_HFK_I,
  NVI_KEY_SOUND_I,
  NVI_DIALS_SORTING_METHOD_I,
  NVI_LANGUAGE_SELECTION_I,
  NVI_MENU_FORMAT_I,
  NVI_VOICE_PRIV_ALERT_I,
#endif

  NVI_RINGER_TYPE_I,
  NVI_RINGER_SPKR_LVL_I,
  NVI_BEEP_SPKR_LVL_I,

  NVI_VIBRATOR_I,
  NVI_FLIP_ANSWERS_I,

  NVI_ELAPSED_TIME_I,

#ifdef FEATURE_NV_DIAL_PLAN
#error code not present
#endif

#ifdef NV_FEATURE_BELL_MODEM
#error code not present
#endif

#ifdef NV_FEATURE_RENTAL_ITEMS
#error code not present
#endif

  NVI_DATA_QNC_ENABLED_I,
  NVI_DATA_SO_SET_I,

#ifdef NV_FEATURE_WEB_ITEMS
#error code not present
#endif

  NVI_SYSTEM_PREF_I,
#ifdef NV_FEATURE_SSPR_ENHANCEMENTS
  NVI_SID_NID_LOCK_I,
#endif

#ifdef FEATURE_OTASP_OTAPA
  NVI_NAM_LOCK_I,
  NVI_OTAPA_ENABLED_I,
#endif

  NVI_PDM1_I,
  NVI_PDM2_I,
  NVI_RX_AGC_MINMAX_I,
  NVI_VBATT_AUX_I,
  NVI_DTACO_CONTROL_I,
  NVI_DTACO_INTERDIGIT_TIMEOUT_I,
  NVI_SMS_UTC_I,
  NVI_ROAM_RINGER_I,
  NVI_FULL_SYSTEM_PREF_I,

#ifdef FEATURE_EVRC_SO_MGMT
  NVI_PREF_VOICE_SO_I,
#endif
  
  NVI_VRHFK_ENABLED_I,
  NVI_VRHFK_VOICE_ANSWER_I,
  NVI_SMS_AUTO_DELETE_I,
  NVI_SRDA_ENABLED_I,
  NVI_OUTPUT_UI_KEYS_I,
  NVI_SPC_CHANGE_ENABLED_I,
  NVI_DATA_AUTO_PACKET_DETECT_I,
  NVI_DATA_MDR_MODE_I,
  NVI_DATA_PKT_ORIG_STR_I,

#ifdef NV_FEATURE_BORSCHT_ITEMS
#error code not present
#endif

#ifdef NV_FEATURE_CARRIER_ITEMS
#error code not present
#endif

#ifdef NV_FEATURE_TPR_ITEMS
#error code not present
#endif

#ifdef NV_FEATURE_SI_DATA
#error code not present
#endif

#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
  NVI_PAP_USER_ID_I,
  NVI_PAP_PASSWORD_I,
#endif

#if (defined FEATURE_PA_RANGE_DIG_COMP || defined FEATURE_PA_RANGE_TEMP_FREQ_COMP)
  NVI_PA_OFFSETS_I,
#endif

#ifdef FEATURE_4PAGE_TX_LINEARIZER
  NVI_CDMA_TX_LIN_MASTER0_I,
  NVI_CDMA_TX_LIN_MASTER1_I,
  NVI_CDMA_TX_LIN_MASTER2_I,
  NVI_CDMA_TX_LIN_MASTER3_I,
#endif

#ifdef NV_FEATURE_TRIMODE_ITEMS
#error code not present
#endif

#ifdef FEATURE_BROADCAST_SMS
  NVI_SMS_BC_SERVICE_TABLE_SIZE_I,
  NVI_SMS_BC_SERVICE_TABLE_I,
  NVI_SMS_BC_CONFIG_I,
  NVI_SMS_BC_USER_PREF_I,
#endif

#ifdef FEATURE_RFR3100
#error code not present
#endif

#if (defined(NV_FEATURE_TRIMODE_ITEMS) && defined(FEATURE_RFR3100))
#error code not present
#endif
  
#ifdef FEATURE_RUIM_CHV_FROM_NV
  NVI_RUIM_CHV_1_I,                 
  NVI_RUIM_CHV_2_I,                 
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif

#ifdef FEATURE_IS2000
  NVI_PREF_FOR_RC_I,
#endif

#ifdef FEATURE_DS_DEFAULT_BITRATE_NV
  NVI_DS_DEFAULT_BAUD_I,
#endif

#ifdef FEATURE_DIAG_DEFAULT_BITRATE_NV
  NVI_DIAG_DEFAULT_BAUD_I,
#endif

#ifdef FEATURE_SDEVMAP_MENU_ITEM_NV
  NVI_SIO_DEV_MAP_MENU_ITEM_I,
#endif

#ifdef FEATURE_TTY
#error code not present
#endif

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
  NVI_PA_RANGE_OFFSETS_I,
  NVI_TX_COMP0_I,
#endif

#ifdef FEATURE_STEREO_DAC
#error code not present
#endif

#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
  NVI_TX_COMP1_I,    
  NVI_TX_COMP2_I,    
  NVI_TX_COMP3_I, 
#endif   
  
  NVI_PRIMARY_DNS_I,
  NVI_SECONDARY_DNS_I,
  
#ifdef FEATURE_DIAG_PORT_SELECT
  NVI_DIAG_PORT_SELECT_I,
#endif

#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif
 
#ifdef FEATURE_UBROWSER
#error code not present
#endif
  
#ifdef FEATURE_IS2000_R_SCH
  NVI_DATA_SCRM_ENABLED_I,
#endif

#ifdef FEATURE_RUIM_SMSS
  NVI_RUIM_SMS_STATUS_I,
#endif

#ifdef NV_FEATURE_TRIMODE_ITEMS
#error code not present
#endif

#ifdef FEATURE_DIAG_RESTART_CONFIG
  NVI_DIAG_RESTART_CONFIG_I,
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif

#ifdef FEATURE_SD20
#error code not present
#endif

#if (defined (FEATURE_ENC_BTF_IN_NV) || defined (T_MSM33))
#error code not present
#endif

#ifdef FEATURE_BT
#error code not present
#endif

#ifdef FEATURE_SUBPCG_PA_WARMUP_DELAY
#error code not present
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif

#ifdef FEATURE_IS2000_R_SCH
  NVI_DATA_TRTL_ENABLED_I,
#endif

#ifdef FEATURE_SD20
#error code not present
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif

#ifdef FEATURE_FACTORY_TESTMODE                   
  NVI_FTM_MODE_I,    
#endif

#ifdef FEATURE_DS_DEFAULT_BITRATE_NV
  NVI_DS_DEFAULT_BAUDRATE_I,
#endif

#ifdef FEATURE_DIAG_DEFAULT_BITRATE_NV
  NVI_DIAG_DEFAULT_BAUDRATE_I,
#endif

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif

  NVI_EXTRA_2_I,
  /* reo 2000-09-27 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)  || (MODEL_ID == MODEL_B2)
#if 0
    NVI_SMSRCV_I,
    NVI_SMSRCV_NUM_I,
#endif
    NVI_SMSSEND_I, 
    NVI_SMSSEND_NUM_I,
    NVI_SMSSENT_I,
    NVI_SMSSENT_NUM_I,
    NVI_VMSRCV_I,
    NVI_VMS_NUM_I,
    NVI_MYBELL_I,
    NVI_MYBELL_NUM_I,
    NVI_LIKEPHRASE_I,
    NVI_LIKEPHRASE_NUM_I,
    NVI_SMSOPTION_I,
  
#endif /* (EQUS_MODEL_ID == EQUS_B1) || (EQUS_MODEL_ID == EQUS_B0)*/
  /* <-- */

  
NVI_MAX_I,             /* Size of this enum, MUST be the last item */
                       /* (except for padding)                     */
  NVI_ITEMS_ENUM_PAD = 0x7FFF  /* Pad to 16 bits on ARM */

} nvi_items_enum_type; 


/*=========================================================================*/
/* Enumerators of items whose pointers are stored in the tps.              */

#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NVI_TPS_SMS_00_I = 0,
  NVI_TPS_SMS_01_I,
  NVI_TPS_SMS_02_I,
  NVI_TPS_SMS_03_I,
  NVI_TPS_SMS_04_I,
  NVI_TPS_SMS_05_I,
  NVI_TPS_SMS_06_I,
  NVI_TPS_SMS_07_I,
  NVI_TPS_SMS_08_I,
  NVI_TPS_SMS_09_I,
  NVI_TPS_SMS_10_I,
  NVI_TPS_SMS_11_I,
  NVI_TPS_SMS_12_I,
  NVI_TPS_SMS_13_I,
  NVI_TPS_SMS_14_I,
  NVI_TPS_SMS_15_I,
  NVI_TPS_SMS_16_I,
  NVI_TPS_SMS_17_I,
  NVI_TPS_SMS_18_I,
  NVI_TPS_SMS_19_I,
  NVI_TPS_SMS_20_I,
  NVI_TPS_SMS_21_I,
  NVI_TPS_SMS_22_I,
  NVI_TPS_SMS_23_I,
  NVI_TPS_SMS_24_I, // 20->40 huni Max Size 40 begin
  NVI_TPS_SMS_25_I,
  NVI_TPS_SMS_26_I,
  NVI_TPS_SMS_27_I,
  NVI_TPS_SMS_28_I,
  NVI_TPS_SMS_29_I,
  NVI_TPS_SMS_30_I,
  NVI_TPS_SMS_31_I,
  NVI_TPS_SMS_32_I,
  NVI_TPS_SMS_33_I,
  NVI_TPS_SMS_34_I,
  NVI_TPS_SMS_35_I,
  NVI_TPS_SMS_36_I,
  NVI_TPS_SMS_37_I,
  NVI_TPS_SMS_38_I,
  NVI_TPS_SMS_39_I,
  NVI_TPS_SMS_40_I,
  NVI_TPS_SMS_41_I,
  NVI_TPS_SMS_42_I,
  NVI_TPS_SMS_43_I, // 20->40 huni Max Size 40 end
  NVI_TPS_MAX_I,       /* Size of this enum, MUST be the last item */
                       /* (except for padding)                     */
  NVI_TPS_ITEMS_PAD = 0x7FFF /* Pad to 16 bits on ARM */

} nvi_tps_items_type;
#endif


/*=========================================================================*/
/*                                                                         */
/* NVM items union type.                                                   */
/*                                                                         */
/*=========================================================================*/

typedef PACKED union {
  dword                       esn;
  dword                       esn_chksum;
  dword                       esn_valid;
  byte                        verno_maj;
  byte                        verno_min;
  nvi_byte_type               scm;
  nvi_byte_type               slot_cycle_index;
  nvi_byte_type               mob_cai_rev;
  nvi_word_type               mob_firm_rev;
  nvi_byte_type               mob_model;
  nvi_word_type               config_chksum;
  nvi_enum_type               pref_mode;
  nvi_enum_type               cdma_pref_serv;
  nvi_enum_type               analog_pref_serv;  
  nvi_sid_lock_type           cdma_sid_lock;
  nvi_sid_acq_type            cdma_sid_acq;
  nvi_sid_lock_type           analog_sid_lock;
  nvi_sid_acq_type            analog_sid_acq;
  nvi_word_type               analog_firstchp;
  nvi_word_type               analog_home_sid;
  nvi_enum_type               analog_reg;
  nvi_boolean_type            analog_ex;
  nvi_cdmach_type             pcdmach;
  nvi_cdmach_type             scdmach;
  nvi_word_type               nam_chksum;
  nvi_a_key_type              a_key;
  nvi_word_type               a_key_chksum;
  nvi_ssd_type                ssd_a;
  nvi_word_type               ssd_a_chksum;
  nvi_ssd_type                ssd_b;
  nvi_word_type               ssd_b_chksum;
  nvi_byte_type               count;
  nvi_min1_type               min1;
  nvi_min2_type               min2;
  nvi_mob_term_type           mob_term_home;
  nvi_mob_term_type           mob_term_for_sid;
  nvi_mob_term_type           mob_term_for_nid;
  nvi_min_byte_type           accolc;
  nvi_sid_nid_type            sid_nid;
  nvi_home_sid_nid_type       home_sid_nid;
  nvi_min_word_type           min_chksum;
  nvi_byte_type               curr_nam;
  nvi_boolean_type            auto_nam;
  nvi_name_type               name_nam;
  nvi_dword_type              nxtreg;
  nvi_word_type               lstsid;
  nvi_word_type               locaid;
  nvi_boolean_type            pureg;
  nvi_zone_list_type          zone_list;
  nvi_sid_nid_list_type       sid_nid_list;
  nvi_dist_reg_type           dist_reg;
  nvi_cdmach_type             last_cdmach;
  nvi_dword_type              call_timer_g;
  nvi_dword_type              air_timer;
  nvi_dword_type              roam_timer;
  nvi_dword_type              life_timer_g;
  nvi_dword_type              run_timer;
  nvi_dial_type               dial;
  nvi_stdial_type             stack;
  nvi_stack_idx_type          stack_idx;
  nvi_dial_type               redial;
  nvi_page_set_type           page_set;
  nvi_byte_type               ear_level;
  nvi_byte_type               speaker_level;
  nvi_byte_type               ringer_level;
  nvi_byte_type               beep_level;
  nvi_boolean_type            call_beep;
  nvi_boolean_type            cont_key_dtmf;
  nvi_boolean_type            cont_str_dtmf;
  nvi_boolean_type            svc_area_alert;
  nvi_boolean_type            call_fade_alert;
  nvi_name_type               banner;
  nvi_fsc_type                fsc;
  nvi_byte_type               n1m;
  nvi_auto_answer_type        auto_answer;
  nvi_auto_redial_type        auto_redial;
  nvi_boolean_type            auto_hyphen;
  nvi_byte_type               back_light;
  nvi_boolean_type            auto_mute;
  nvi_maintrsn_type           maintrsn;
  nvi_lckrsn_p_type           lckrsn_p;
  nvi_byte_type               lock;
  nvi_lock_code_type          lock_code;
  nvi_boolean_type            auto_lock;
  nvi_word_type               call_rstrc;
  nvi_sec_code_type           sec_code;
  nvi_boolean_type            horn_alert;
  nvi_err_log_type            err_log;
  nvi_minmax_type             vbatt;
  nvi_fm_tx_pwr_type          fm_tx_pwr;
  nvi_temp_offset_type        fr_temp_offset;
  nvi_minmax_type             fm_rssi;
  nvi_pa_cur_ctl_type         pa_cur_ctl;
  nvi_word_type               spare_1;
  nvi_word_type               spare_2;
  nvi_byte_type               data_io_mode;
  nvi_byte_type               dm_io_mode;
  nvi_boolean_type            data_srvc_enabled;
  nvi_word_type               idle_data_timeout;
  nvi_byte_type               max_tx_adj;
  nvi_byte_type               init_mute;
  nvi_fact_info_type          fact_info;
  nvi_byte_type               sony_atten_3;
  nvi_byte_type               sony_atten_4;
  nvi_byte_type               sony_atten_5;
  nvi_byte_type               dm_addr;
  nvi_byte_type               cdma_pn_mask;
  nvi_byte_type               send_timeout;
  nvi_adj_vs_freq_type        fm_agc_adj_vs_freq;
  nvi_pwr_table_type          fm_agc_set_vs_pwr;
  nvi_freq_table_type         fm_agc_set_vs_freq;
  nvi_temp_table_type         fm_agc_set_vs_temp;
  nvi_pwr_table_type          fm_exp_hdet_vs_pwr;
  nvi_pwr_table_type          fm_err_slp_vs_pwr;
  nvi_byte_type               fm_freq_sense_gain;
  nvi_minmax_type             fm_lna_ctl;
  nvi_byte_type               cdma_rx_lin_off_0;
  nvi_cdma_rx_lin_type        cdma_rx_lin_slp;
  nvi_freq_int1_table_type    cdma_rx_comp_vs_freq;
  nvi_freq_int1_table_type    cdma_tx_comp_vs_freq;
  nvi_word_type               cdma_tx_lin_master_off_0;
  nvi_tx_lin_master_slp_type  cdma_tx_lin_master_slp;
  nvi_temp_table_type         cdma_tx_lin_vs_temp;
  nvi_volt_table_type         cdma_tx_lin_vs_volt;
  nvi_temp_table_type         cdma_tx_lim_vs_temp;
  nvi_volt_table_type         cdma_tx_lim_vs_volt;
  nvi_freq_table_type         cdma_tx_lim_vs_freq;
  nvi_exp_hdet_vs_agc_type    cdma_exp_hdet_vs_agc;
  nvi_err_slp_vs_hdet_type    cdma_err_slp_vs_hdet;
  nvi_temp_int1_table_type    cdma_rx_lin_vs_temp;
  nvi_byte_type               cdma_adj_factor;
  nvi_temp_int1_table_type    cdma_rx_slp_vs_temp;
  nvi_temp_int1_table_type    cdma_tx_slp_vs_temp;
  nvi_temp_int1_table_type    pa_range_vs_temp;
  nvi_temp_int1_table_type    pdm1_vs_temp;
  nvi_temp_int1_table_type    pdm2_vs_temp;
  nvi_freq_table_type         pdm1_vs_freq;
  nvi_freq_table_type         pdm2_vs_freq;
  nvi_temp_table_type         fm_exp_hdet_vs_temp;
  nvi_minmax_type             therm;
  nvi_minmax_type             vbatt_pa;
  nvi_byte_type               hdet_off;
  nvi_byte_type               hdet_spn; 
  nvi_byte_type               lcd;
  nvi_byte_type               rf_config;
  nvi_byte_type               r1_rise;
  nvi_byte_type               r1_fall;
  nvi_byte_type               r2_rise;
  nvi_byte_type               r2_fall;
  nvi_byte_type               r3_rise;
  nvi_byte_type               r3_fall;
  nvi_byte_type               pa_range_step_cal;
  nvi_byte_type               lna_range_pol;
  nvi_int1_type               lna_range_rise;
  nvi_int1_type               lna_range_fall;
  nvi_word_type               lna_range_offset;

  nvi_boolean_type            onetouch_dial;
  nvi_byte_type               alerts_lvl;
  nvi_byte_type               alerts_lvl_shadow;
  nvi_byte_type               ringer_lvl_shadow;
  nvi_byte_type               beep_lvl_shadow;
  nvi_byte_type               ear_lvl_shadow;
  nvi_byte_type               auto_answer_shadow;
  nvi_boolean_type            time_show;
  nvi_dword_type              air_cnt;
  nvi_dword_type              roam_cnt;
  nvi_dword_type              life_cnt;
  nvi_dword_type              pwr_dwn_cnt;
  nvi_boolean_type            send_pin;
  nvi_sms_type                sms;
  nvi_imsi_11_12_type         imsi_11_12;
  nvi_imsi_mcc_type           imsi_mcc;
  nvi_dir_number_type         dir_number;
  nvi_dir_number_pcs_type     mob_dir_number;
  nvi_byte_type               voice_priv;
  nvi_byte_type               voice_priv_alert;
  nvi_byte_type               spare_b1;
  nvi_byte_type               spare_b2;
  nvi_word_type               spare_w1;
  nvi_word_type               spare_w2;

  nvi_alarms_type             alarms;
  nvi_isd_std_passwd_type     isd_std_passwd;
  nvi_word_type               standing_alarm;
  nvi_word_type               dialing_plan;
  nvi_byte_type               isd_std_restrict;
  nvi_minmax_type             fm_agc;
  nvi_fsc2_type               fsc2;
  nvi_fsc2_chksum_type        fsc2_chksum;
  nvi_wdc_type                wdc;
  nvi_dword_type              hw_config;

  nvi_byte_type               imsi_addr_num;
  nvi_byte_type               assigning_tmsi_zone_length;
  nvi_qword_type              assigning_tmsi_zone;
  nvi_dword_type              tmsi_code;
  nvi_dword_type              tmsi_exp_timer;
  nvi_byte_type               home_block;
  nvi_roaming_list_type       roaming_list;
  nvi_mru2_table_type         mru2_table;
  nvi_sec_code_type           otksl;
  nvi_boolean_type            timed_pref_mode;
  nvi_min1_type               imsi_t_s1;
  nvi_min2_type               imsi_t_s2;
  nvi_imsi_mcc_type           imsi_t_mcc;
  nvi_imsi_11_12_type         imsi_t_11_12;
  nvi_byte_type               imsi_t_addr_num;

  nvi_byte_type               ringer_type;
  nvi_boolean_type            any_key_answer;
  nvi_byte_type               back_light_hfk;
  nvi_enum_type               key_sound;
  nvi_enum_type               sorting_method;
  nvi_enum_type               language_selection;
  nvi_enum_type               menu_format;

  nvi_byte_type               ringer_spkr_lvl;
  nvi_byte_type               beep_spkr_lvl;

  nvi_boolean_type            vibrator;
  nvi_boolean_type            flip_answers;

  nvi_dial_restrict_lvls_type     dial_restrict_lvls;
  nvi_dial_state_table_len_type   dial_state_table_len;
  nvi_dial_state_table_elem_type  dial_state_table;

  nvi_boolean_type            data_qnc_enabled;
  nvi_byte_type               data_so_set;

  nvi_up_link_info_type       up_link_info;
  nvi_up_key_type             up_key;
  nvi_up_parms_type           up_parms;
  nvi_up_cache_type           up_cache;
  nvi_up_alert_record_type    up_alert_record;
  nvi_word_type               up_idle_timer;

  nvi_word_type               elapsed_time;

  nvi_byte_type               pdm1;
  nvi_byte_type               pdm2;
  nvi_minmax_type             rx_agc_minmax;
  nvi_minmax_type             vbatt_aux;
  
  nvi_enum_type               dtaco_control;
  nvi_byte_type               dtaco_interdigit_timeout;
  nvi_boolean_type            bell_modem;
  
  nvi_sys_pref_type           system_pref;
  nvi_sid_nid_lock_type       sid_nid_lock;
  nvi_enabled_type            prl_enabled;

#ifdef FEATURE_OTASP_OTAPA
  nvi_enabled_type            otapa_enabled;
  nvi_enabled_type            nam_lock;
#endif

  nvi_boolean_type            sms_utc;
  nvi_boolean_type            roam_ringer;
  nvi_rental_timer_type       rental_timer;
  nvi_byte_type               rental_timer_inc;
  nvi_rental_cnt_type         rental_cnt;
  nvi_boolean_type            rental_timer_enabled;
  nvi_boolean_type            full_system_pref;
  
  nvi_byte_type               borscht_ringer_freq;
  nvi_boolean_type            payphone_enable;
  nvi_boolean_type            dsp_answer_det_enable;
  nvi_enum_type               evrc_pri;
  nvi_enum_type               v52_control;
  nvi_enum_type               afax;

  nvi_carrier_info_type       carrier_info;
  #ifdef FEATURE_EVRC_SO_MGMT
  nvi_pref_voice_so_type      pref_voice_so;
  #endif
  nvi_boolean_type            vrhfk_enabled;
  nvi_boolean_type            vrhfk_voice_answer;
  nvi_boolean_type            sms_auto_delete;
  nvi_boolean_type            srda_enabled;
  nvi_boolean_type            output_ui_keys;
  
  nvi_byte_type               pol_rev_timeout;

  nvi_si_test_data_type       si_test_data_1;
  nvi_si_test_data_type       si_test_data_2;
  nvi_boolean_type            spc_change_enabled;
  nvi_byte_type               data_auto_packet_detection;

  nvi_byte_type               data_mdr_mode;
  nvi_data_pkt_orig_str_type  data_pkt_orig_str;

#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif  
#ifdef FEATURE_UI_NEW_OTKSL
#error code not present
#endif  
#ifdef FEATURE_UI_SMS
  nvi_byte_type               sms_reminder_tone;
#endif  
#ifdef FEATURE_UBROWSER
#error code not present
#endif
  
  nvi_int2_type               btf_adjust;

#ifdef FEATURE_UI_MINIBROWSER
#error code not present
#endif  
#ifdef FEATURE_USER_ZONE
#error code not present
#endif
#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
  nvi_pap_user_id_type        pap_user_id;
  nvi_pap_password_type       pap_password;
#endif

#if (defined FEATURE_PA_RANGE_DIG_COMP || defined FEATURE_PA_RANGE_TEMP_FREQ_COMP)
  nvi_pa_offset_type          pa_offsets;
#endif

#ifdef FEATURE_4PAGE_TX_LINEARIZER
  nvi_tx_linearizer_type      cdma_tx_lin_master0;
  nvi_tx_linearizer_type      cdma_tx_lin_master1;
  nvi_tx_linearizer_type      cdma_tx_lin_master2;
  nvi_tx_linearizer_type      cdma_tx_lin_master3;
#endif

#ifdef FEATURE_BROADCAST_SMS
  nvi_word_type                  sms_bc_service_table_size;   
  nvi_sms_bc_service_table_type  sms_bc_service_table;                                                              
  nvi_byte_type                  sms_bc_config;                                                              
  nvi_byte_type                  sms_bc_user_pref;
#endif

#ifdef FEATURE_RFR3100
#error code not present
#endif

#ifdef FEATURE_RUIM_CHV_FROM_NV
  nvi_ruim_chv_type              ruim_chv_1;
  nvi_ruim_chv_type              ruim_chv_2;
#endif

#ifdef FEATURE_IS2000
  nvi_enum_type                  pref_for_rc;
#endif

#ifdef FEATURE_DS_DEFAULT_BITRATE_NV
  nvi_enum_type                  ds_default_baud;
#endif

#ifdef FEATURE_DIAG_DEFAULT_BITRATE_NV
  nvi_enum_type                  diag_default_baud;
#endif

#ifdef FEATURE_SDEVMAP_MENU_ITEM_NV
  nvi_int1_type                  sio_dev_map_menu_item;
#endif

#ifdef FEATURE_TTY
#error code not present
#endif

#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
  nvi_ref_tempsense_type	  ref_tempsense;
  nvi_freq_pwr_table_type	  cdma_freq_pwr;
  nvi_pminmax_type			  pa_pwr_p1p2;
#endif	// #ifdef FEATURE_SKTT_CAL
  nvi_voice_info_type	      voice_info;      /* hjcho 00.12.19 */
  nvi_fn_sequence_type	      Fn_sequence;     /* hjcho 00.12.19 */
            
  nvi_byte_type               data_ipr_mode;   //white 2001.03.21
  nvi_byte_type               data_rev_mode;   //white 2001.03.21

  nvi_dword_type            flip_count;         // woodstok 01.5.3
  nvi_multi_sid_type        multi_sid;          /* hjcho 01.09.17 */

/* reo 00-07-27 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)  || (MODEL_ID == MODEL_B2)
#if 0    
    nvi_smsrcv_type         sms_rcv_list;
    nvi_byte_type           sms_rcv_num; 
#endif
    nvi_smssend_type        sms_send_list;
    nvi_byte_type           sms_send_num;

    nvi_smssend_type        sms_sent_list;
    nvi_byte_type           sms_sent_num;

    nvi_vmsrcv_type         vms_list;
    nvi_byte_type           vms_num;
                                       
    nvi_mybell_type         mybell_list;
    nvi_byte_type           mybell_num;

    nvi_likephrase_type     likephrase_list;
    nvi_byte_type           likephrase_num;

    nvi_smsoption_type      sms_option;
#endif /* (EQUS_MODEL_ID == EQUS_B1) || (EQUS_MODEL_ID == EQUS_B0) */
/* <-- */

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
  nvi_pa_range_offsets_type      pa_range_offsets;
  nvi_tx_comp_type               tx_comp0;
#endif  

#ifdef FEATURE_STEREO_DAC
#error code not present
#endif

#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
  nvi_tx_comp_type               tx_comp1;
  nvi_tx_comp_type               tx_comp2;
  nvi_tx_comp_type               tx_comp3;
#endif  

  nvi_dword_type                 primary_dns;
  nvi_dword_type                 secondary_dns;

#ifdef FEATURE_DIAG_PORT_SELECT
  nvi_byte_type                  diag_port_select;
#endif

#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif

#ifdef FEATURE_IS2000_R_SCH
  nvi_boolean_type               data_scrm_enabled;
#endif

#ifdef FEATURE_RUIM_SMSS
  nvi_ruim_sms_status_type       ruim_sms_status;
#endif
  
#ifdef NV_FEATURE_TRIMODE_ITEMS
#error code not present
#endif
  
#ifdef FEATURE_DIAG_RESTART_CONFIG
  nvi_diag_restart_config_type   diag_restart_config;
#endif

#ifdef FEATURE_SD20
#error code not present
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif

#if (defined (FEATURE_ENC_BTF_IN_NV) || defined (T_MSM33))    
#error code not present
#endif

#ifdef FEATURE_BT
#error code not present
#endif

#ifdef FEATURE_SUBPCG_PA_WARMUP_DELAY
#error code not present
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif

#ifdef FEATURE_IS2000_R_SCH
  nvi_boolean_type               data_trtl_enabled;
#endif

#ifdef FEATURE_SD20
#error code not present
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif

#ifdef FEATURE_FACTORY_TESTMODE                   
  nvi_boolean_type               ftm_mode;
#endif

#ifdef FEATURE_DS_DEFAULT_BITRATE_NV
  nvi_enum_type                  ds_default_baudrate;
#endif

#ifdef FEATURE_DIAG_DEFAULT_BITRATE_NV
  nvi_enum_type                  diag_default_baudrate;
#endif

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif

	nvi_phoneTAG_type	phoneTAG;

} nvi_item_type;

/* Return to default packing of data structures.                        */
#ifdef FEATURE_TREX
#error code not present
#endif

typedef enum {
  NVI_LONGPOOL_I,
  NVI_SHORTPOOL_I,
  NVI_POOL_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
} nvi_pool_enum_type; 


/*===========================================================================

MACRO NVI_SIZEOF

DESCRIPTION
  Size an internal nv item of the union type "nvi_item_type."

PARAMETERS
  Member of the nvi_enum_type to be sized.

DEPENDENCIES
  None.

LIMITATIONS
  None.

RETURN VALUE
  sizeof the internal item.

SIDE EFFECTS
  None.

===========================================================================*/
#define NVI_SIZEOF(A) ((word) (sizeof(((nvi_item_type *) 0)-> ##A)))

/*===========================================================================

FUNCTION NVI_MOVE_HANDOVER

DESCRIPTION
  This function checks for the presence of the RF Handover Block.  If the
  block is found, it reallocates it to a "safe" location to be checked
  again later, after the rebuild.

DEPENCENCIES
  This function should only be called by nva_init immediately after
  detecting that the handover block header is present.

RETURN VALUE
  None.

SIDE EFFECTS
  The handover block (if present) is reallocated into the SMS message
  area.

===========================================================================*/

#ifdef FEATURE_NV_HANDOVER_BLOCK
void nvi_move_handover (void);
#endif


#endif /* NVI_H */

#endif // (MODEL_ID == MODEL_A10)$H/*#i#i
