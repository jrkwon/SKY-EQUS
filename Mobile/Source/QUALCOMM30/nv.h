#include "target.h"

#if defined(MMI_TREX)
#include <nv.h>
#elif (MMI_TYPE == MMI_A10)
#include <nvA10.h>
#else
#ifndef NV_H
#define NV_H
/*==========================================================================

              N V    T A S K    H E A D E R    F I L E

DESCRIPTION

  This header file contains all the definitions necessary for any task
  to interface with the Non Volatile Memory task, in order to access
  the NV EEPROM to read, write, and start.

Copyright (c) 1990,1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001
 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/nv.h_v   1.16   20 Feb 2001 17:02:56   rprenove  $
   
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
01/12/00   pg      Added support for testing in TREX.
                   Deleted support for testing in COMET.
                   Added support for NV_DATA_TRTL_ENABLED_I associated with
                   FEATURE_IS2000_R_SCH, and NV_AMPS_BACKSTOP_ENABLED_I
                   associated with FEATURE_SD20.
                   Added support for 4K roaming lists.
12/14/00   pg      Increased minor rev number to generate NV rebuild.
12/14/00   pg      Added pads to some enum types. 
12/13/00   pg      Removed #ifdef FEATURE_NV_ENUMS_ARE_WORDS on
                   nv_gps1_transport_type.
12/13/00   pg      In nv_gps1_transport_type, instead of changing 
                   NV_DBM_TRANSPORT to 0x10000000, added 
                   NV_GPS1_TRANSPORT_RANGE = 0x10000000.
                   Increased minor rev number to generate NV rebuild.
12/13/00   pg      Made nv_gps1_transport_type to occupy 4 bytes instead of 
                   1 byte.
12/06/00   pg      Enabled single NAM configs for Cellular build.
11/29/00   pg      Replaced all internal defined features: FEATURE_XX_ENABLED
                   as NV_FEATURE_XX_ENABLED.
11/28/00   pg      Cleaned out obsolete item NV_AUTO_POWER_I.
11/20/00   pg      Turned on items NV_PCS_ENC_BTF_I and NV_CDMA_ENC_BTF_I for
                   all MSM3300 builds.
11/15/00   pg      Recovered item NV_CDMA_TX_LIM_VS_VOLT_I as it is needed in
                   FFA5000 build.
11/01/00   pg      Reverted the previous change.
                   Added supported for FEATURE_TRIMODE_ITEMS. If this feature
                   is turned on by customers, NV_FEATURE_TRIMODE_ITEMS is 
                   defined.
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
08/29/00   rp      Fixed comment on line 6511 to prevent compiler warnings. 
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
07/25/00   rp      Defined NV_FS_FILENAME_MAX_LENGTH to remove dependency on
                   FS_FILENAME_MAX_LENGTH.
07/18/00   pg      Changed NV_UBROWSER_TABLE_SIZE from 32 to 30.
07/06/00   pg      Added support for NV_MM_STORAGE_DEVICE_I, NV_UBROWSER_I.
07/05/00   pg      Added support for NV_MM_RINGER_FILE_I.
06/23/00   rp      Merged in support for all MSM3300 GPS One items under
                   FEATURE_GPSONE.
06/23/00   rp      Added NVI_VERNO_EXT_BYTE_2 and NVI_VERNO_EXT_BYTE_3
                   used in nvi_verno_ext_type to track new features.
06/14/00   kmh     Added NV Items for SKTT Calibration
                   : NV_REF_TEMPSENSE_I, NV_CDMA_FREQ_VS_PWR_I, NV_PA_PWR_I
06/07/00   rp      Incremented minor revs due to NVIM file changes.
05/30/00   pg      Added support for NV_PRIMARY_DNS_I, NV_SECONDARY_DNS_I.
                   Added support for NV_DIAG_PORT_SELECT_I associated with
                   FEATURE_DIAG_PORT_SELECT.
05/22/00   pg      Fixed typo in comment so that tools would be able to 
                   generate nv list automaticly.
05/08/00   jah     Added signal for R-UIM access
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
03/29/00   rp      Incremented minor revs due to array update for
03/18/00   rp      Fixed typo in NV_RUIM_CHV_SIZE. 
03/16/00   rp      Added support for R-UIM items
                   sms_bc_service_table.
03/13/00   rp      Added conditional code for Broadcast SMS items support to
                   limit the table size with EEPROM. 
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
02/23/00   rp      Changed type for NV_CDMA_LNA_OFFSET_VS_FREQ_I and 
                   NV_CDMA_LNA_12_OFFSET_VS_FREQ_I from word to int1.  
02/22/00   rp      Added support for new RFR3100 items for Trimode.
02/22/00   rp      Changed type for NV_LNA_RANGE_12_OFFSET_I from word to 
                   int2.  
02/22/00   rp      Added support for new RFR3100 item NV_RX_AGC_MIN_11_I.
02/22/00   rp      Made NAM_LOCK a per-NAM item and added it to OTASP_COMMIT.
02/16/00   rp      Added support for new RFR3100 items.
01/31/00   rp      Added Broadcast SMS items support.
12/06/99   rp      Added Trimode items support.
                   If Trimode (FEATURE_CDMA_1900 and FEATURE_CDMA_800) then
                   #define NV_FEATURE_TRIMODE_ITEMS, FEATURE_NV_ANALOG,
                   NV_FEATURE_IMSI_T_ITEMS and NV_FEATURE_SSPR_ENHANCEMENTS. 
                   Updated nv_mode_enum_type to support Trimode. 
11/24/99   rp      Incremented minor revs due to NVIM file changes.
10/14/99   rp      Added #pragma pack(1) for Comet testing.
10/04/99   rp      Removed definitions for FEATURE_NV_OTASP_800.  Added 
                   a single defintion for FEATURE_NV_OTASP based on
                   FEATURE_NV_OTASP.
                   Added featurized support for NV_ROAM_MSG_I, NV_OTKSL_FLAG_I,
                   NV_SMS_REMINDER_TONE_I, NV_FULL_PREF_MODE_I,
                   NV_UP_BROWSER_WARN_I, NV_USER_ZONE_ENABLED_I, NV_USER_ZONE_I,
                   NV_AUTO_VOLUME_ENABLED_I, and NV_WILDCARD_SID_I for Digby.
                   Addded support to target T_O for Digby.
                   Increased nv_up_parms_type from 6 entries to 10 for Digby.
09/15/99   ymc     Undefined NV_FEATURE_PDM_TABLES as PRF3000 doesn't support it.
08/16/99   rp      Removed support for NV_PAP_DATA_I. Added support for
                   NV_PAP_USER_ID_I and NV_PAP_PASSWORD_I.
08/13/99   rp      Added support for NV_PAP_DATA_I.
07/28/99   kmp     Removed #else from #if (TG==T_O) for PDM tables. Added
                   PDM tables back for all PCS builds.
07/26/99   ks      Added support for target T_O (Odie) and enabled the PDM
                   Tables.
06/30/99   kmp     Removed references to FEATURE_NV_JSTD008 and replace them
                   with FEATURE_NV_TMSI. Replaced FEATURE_JSTD008 with
                   FEATURE_CDMA_1900 and removed PACKED from the enums for the
                   ARM 2.50 compiler. Added support for PCS NV True IMSI items.
04/18/99   snn     Enabled NV_FEATURE_PDM_TABLES in the JSTD008 path because PDM1 
                   PDM2 are used in the PCS build.
04/18/99   abh     Modified nv_otasp_commit_type otasp_commit to 
                   nv_otasp_commit_type * otasp_commit                   
04/09/99   abh     Made changes to use HOME_SID_NID item in case of OTASP commit
                   for SSPR enabled targets
04/02/99   abh     Added definitions for FEATURE_NV_OTASP_800.
03/31/99   snn     Deleted the references to 80186 pragma packs
03/31/99   snn     Added new feature definitions under T_G. 
03/26/99   snn     Modified the NVI_KLT_REV to 0x81 and NVI_VERNO_MIN;
03/24/99   snn     Added definitions for the SSPR Enhancements.
03/11/99   snn     ck made some changes to earlier files and I added them here.
                   Ifdefed the Service Negotiation item PREF_VOICE_SO_I under
                   FEATURE_EVRC_SO_MGMT
03/11/99   snn     udp made some changes to earlier files and I added them here.
                   Added T_Q #ifdef for rental items
03/11/99   snn     ny made some changes to earlier files and I added them here.
                   Cleaned up the compilation to remove warnings/error messges.
                   Applied merge guidelines 3.0 to clean up the code
03/19/99   ls      Made NV_OTKSL_I universal.
03/11/99   ls      Added NV_DATA_AUTO_PACKET_DETECTION_I item.
03/05/99   ls      Made UP link access key a separate item.
03/02/99   ls      Changed "int" to "word" in UP items.
02/26/99   ls      Added MDR items and IS-95B support.
02/11/99   ls      Merged OTASP 3.0.
02/10/99   ls      Added new Odie configuration.
02/05/99   ls      Changed featurization of NV_PREF_MODE_I.
02/03/99   ls      Introduced enums-as-words option.
02/02/99   ls      Added more PACKED and pads, added stack checker items.
01/28/99   ls      Added a new TGP configuration.
01/14/99   ls      Added PDM tables support for ISS2 targets.
01/12/99   ls      Added PACKED keyword to more types.
12/03/98   ls      Added additional support for FEATURE_NV_CARRIER_ITEMS.
11/29/98   ls      Added support for FEATURE_NV_CARRIER_ITEMS for T_MD.
11/21/98   ls      Added nv_dp_capacity function and NV_OUTPUT_UI_KEYS_I.
11/19/98   dlb     Padded command enum's to 16 bits.
11/09/98   ls      Added SSPR_ENHANCEMENTS to new Peabody configuration.
11/04/98   ls      Added another Peabody configuration.
10/30/98   ls      Linked SMS auto-reservations with external UI feature.
10/28/98   dlb     Added support for ARM.
10/26/98   ls      Added NV_SRDA_ENABLED_I.
10/19/98   ls      Merged remainder of SSPR Enhancements and IMSI-T.
10/09/98   ls      Added SSPR_ENHANCEMENTS support to Odie, Peabody, Q.
10/07/98   ls      Added large roaming list configuration for Odie.
10/01/98   ls      Changed EVRC item subtypes and added NV_SMS_AUTO_DELETE_I.
09/29/98   ls      Added EVRC, VRHFK, and new RF Cal items.
09/21/98   ls      Increased MAX_SPEED_DIALS to 109 for Q and Odie targets.
09/10/98   ls      Revived AUTO_ANSWER_SHADOW, added NV_SIO_PWRDWN_I.
09/08/98   ht      Added nv_ram_roaming_list_type for FASTPRL.
08/27/98   ls      Added NV_AFAX_I, plus other configuration changes.
08/25/98   ls      Added CARRIER_INFO for module.
08/19/98   ls      Added BORSCHT items.
08/11/98   ls      Added NV_FULL_SYSTEM_PREF_I.
08/04/98   ls      Added rental timer enabled item support.
08/02/98   ls      Added rental counter item support.
07/27/98   ls      Added rental timer items.
07/14/98   ls      Merged remainder of SSPR-800 support under internal 
                   feature control.
07/06/98   ls      Partial merge of SSPR-800: added IS-683A PRL support.
06/22/98   ls      Removed Unwired Planet cache.
06/12/98   ls      Added Unwired Planet alert items, fixed ISS2 roaming
                   list sizing problem.
06/11/98   ls      Added web items support for all targets except T_I2.
06/04/98   ls      Implemented dynamic pool reservation system.
05/14/98   ls      Added NV_PDM1_VS_TEMP_I and NV_PDM2_VS_TEMP_I.
05/08/98   ls      Added next set of SSPR-800 changes.
04/30/98   ls      Added SSPR-800 new/changed items.
04/17/98   grl     Changed MAX_SPEED_DIALS to 109 for T_DM and T_SP targets.
03/19/98   ls      Added PDM1, BELL_MODEM, and ALERTS_LVL_SHADOW.
01/20/98   ls      Added DTACO items, new RF Cal items, version field in
                   roaming list, and Odie target.
12/22/97   ls      Added AUTOMATIC and E911 values for PREF_MODE item.
12/16/97   ls      Added RF Cal pointer, elapsed_time item, and disconnect
                   timer.
11/26/97   ls      Removed several obsolete items.
11/05/97   ls      Added Unwired Planet support, version 2.
10/31/97   ls      Made OTKSL available to 2-NAM builds.
10/22/97   ls      ISS2 builds always have a 1K roaming list.
10/21/97   ls      Added data services items.
08/29/97   ls      Rebuild due to nvi_dir_number_type change.
08/22/97   ls      Replaced calculate_sms_memavail() with sms_sizeof().
08/03/97   ls      Added nv_calculate_sms_memavail() function plus support.
07/23/97   ls      Added initial support for Unwired Planet.
07/22/97   ls      Changed roaming list max size to 1024 when autosize
                   feature is enabled.
07/21/97   ls      Changed roaming list max size to 1000 when autosize
                   feature is enabled.
07/16/97   ls      Made redial item available to T_Q target.
07/15/97   ls      Added 2-NAM with 4 SID/NID pairs configuration for TGP.
07/01/97   ls      Added 8K NV support for ISS2, removed 4-NAM TGP target.
06/17/97   dhh     Added target T_Q for Q phone.
06/06/97   ls      Added voice_privacy_alert item for TGP.
06/05/97   ls      Added another phone book entry for ISS2.
06/03/97   ls      Implemented auto-detection of EEPROM configuration.
05/15/97   ls      Added support for OTASP-800.
04/30/97   ls      Added ISS2 dialing plan state table item.
04/22/97   ls      Made redial item available to all TGP variants.
04/02/97   ls      Added Clam-Shell Call Answer item, removed old MRU
                   table format.
03/14/97   jjn     Defined new target called T_MD for the Module.
02/25/97   ls      Re-enabled T_DM and T_SP configurations.
02/13/97   ls      Added vibrator item, defined both MRU table formats.
01/29/97   ls      Re-enabled ISS2 non-JSTD008 configuration.
01/23/97   ls      Added support for TGP without Roaming Lists or MRU Table.
01/17/97   ls      Changed MRU Table format.
01/08/97   ls      Increment major, not minor verno until PST updated.
01/03/97   ls      Featurized NV.
11/11/96   ls      Added valid flag to roaming_list_type.
10/03/96   ls      Added NV_TIMED_PREF_MODE_I
08/22/96   ls      Make FEATURE_JSTD008 available to ISS2 target
08/14/96   ls      Save RAM by using a single, global roaming list buffer
08/06/96   ls      Added FEATURE_NV_ONE_NAM_RL_LARGE
07/18/96   ls      Fixed embedded comment for lint
07/03/96   ls      Added NV_REDIAL_I item
06/26/96   ls      Added Roaming List and MRU Table support
06/17/96   ls      Added support for OTASP "commit" operation
06/06/96   ls      Increased to 31 speed dials for ISS2
05/30/96   ls      Added initial subset of J-STD-008 NAM parameters and 
                   extensions
05/10/96   ls      Added PCS registration updates
02/28/96   ls      Changed types of RF Cal items NV_LNA_RANGE_RISE_I and
                   NV_LNA_RANGE_FALL_I to int1
02/27/96   ls      Added N1M nv item
02/26/96   ls      Increased NV_SMS_DATA_SIZ to 388
02/20/96   ls      Added RF Calibration items
01/30/96   ras     Pre-code review cleanup
12/12/95   ras     added NV_CHKPNT_ENA_F and NV_CHKPNT_DIS_F
11/29/95   ras     changed NV_SYS_SID_ONLY to NV_SYS_HOME_PREF
11/22/95   ras     added NV_HW_CONFIG. Fixed sizing of WDC.
10/31/95   ras     added #if's for initial CSS build from ref. "spooky"
10/02/95   ras     added fm_agc,fsc2,fsc2_chksum,wdc, moved lcd to fixed item.
09/18/95   ras     changed pwr_up_cnt to pwr_dwn_cnt
09/11/95   ras     added fm_lna,life_timer_g,call_timer_g, pwr_up_tmr
09/08/95   ras     added support for ALARMS, STANDING_ALARMS, DIALING_PLAN,
                   ISD_STD_PASSWD, ISD_STD_RESTRICT
09/05/95   ras     all ISS2 speed dials are 21. Added NV_FSC_I.
08/31/95   dak     Changed NV_MAX_SPEED_DIALS to be 21 in ISS2/INDIA and
                   20 in ISS2/US.
08/23/95   ras     added DIR_NAME
08/17/95   jmk     Took out the T_SP conditionally excluded stuff.  Kept in
                   the T_SP added at the same place as T_DM.
08/11/95   jmk     Added T_SP conditional compilation (exclude rex, protos, etc)
08/08/95   ras     lint cleanup, IMSI, bad pwr down structures
07/17/95   ras     added support for sms
06/11/95   ras     added support for 20 new rf & ui items
05/22/95   ras     Gemini back to 102 stored numbers
05/12/95   ras     rf item additions + NV_LCD_I - NV_BRIGHT_I
05/09/95   ras     Initial next generation nv checkin
12/17/93   jjw     Added DM address item
11/08/93   jjw     uncommented the External Port mode values
11/04/93   jjw     Fixed a boo-boo (sony_atten_2)
08/22/93   jjw     Added data services items and several other items
05/14/93   jjw     Added compile-time directive for Beta II target
05/10/93   jjw     Changed FM_RSSI to a min_max type
04/06/93   jjw     Major upgrade: Added numerous new items, changed numerous 
                   items, removed ESN from poke, disallowed changing ESN after
                   it becomes non-zero
02/05/93   jjw     Added new items: 1) FM TX power levels (8) and 2) V Battery
                   minimum and maximum values.
10/29/92    ip     Modified items: err_log, sid_acq, auto_answer,
                   auto_redial, page_set, stack_idx.  Added items:
                   unit_id, name_nam, roam_timer, freq_adj.
10/07/92    ip     Removed boot_err item.
07/18/92    ip     Release after code review and initial unit test.
07/03/92    ip     Released per CAI 2.0 plus several items changes.
04/10/92    ip     Changes after code review and per CAI Rev 1.14.
02/24/92    ip     First porting of file from brassboard to DMSS.

===========================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

#include "target.h"
#include "comdef.h"
#include "rex.h"
#include "queue.h"
#include "qw.h"


/*===========================================================================
 
                           DATA DECLARATIONS

===========================================================================*/
/* reo 00-07-29 --> @why, what */
//#define FEATURE_EQUS_A10B1
/* <-- */

/*  Signals declarations.                                                  */

#define  NV_RPT_TIMER_SIG     0x0001      /* Watchdog report signal */
#define  NV_WRITE_TIMER_SIG   0x0002      /* Write timer signal */
#define  NV_CMD_Q_SIG         0x0004      /* Command queue signal */
#define  NV_ERR_LOG_SIG       0x0008      /* error log buffer siganl */

#ifdef FEATURE_NV_RUIM
#define  NV_RUIM_ACCESS_SIG   0x0010      /* R-UIM Access Synchronization  */
#endif

/*  Reserved signals.                                                      */

#define  NV_RSVD_0010_SIG     0x0010
#define  NV_RSVD_0020_SIG     0x0020
#define  NV_RSVD_0040_SIG     0x0040
#define  NV_RSVD_0080_SIG     0x0080
#define  NV_RSVD_0100_SIG     0x0100
#define  NV_RSVD_0200_SIG     0x0200
#define  NV_RSVD_0400_SIG     0x0400
#define  NV_RSVD_0800_SIG     0x0800

/*  Signals 0x1000 thru 0x8000 are system wide and defined in task.h.      */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_TREX
#error code not present
#endif

#ifdef FEATURE_TREX
#error code not present
#endif

/*  Command codes when command is issued to the NV task.                   */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
  typedef enum {
    NV_READ_F,          /* Read item */
    NV_WRITE_F,         /* Write item */
    NV_PEEK_F,          /* Peek at a location */
    NV_POKE_F,          /* Poke into a location */
    NV_FREE_F,          /* Free an nv item's memory allocation */
    NV_CHKPNT_DIS_F,    /* Disable cache checkpointing for glitch recovery */
    NV_CHKPNT_ENA_F,    /* Enable cache checkpointing for glitch recovery */
    NV_OTASP_COMMIT_F,  /* Commit (write) OTASP parameters to nv */
    NV_REPLACE_F,       /* Replace (overwrite) a dynamic pool item */
    NV_INCREMENT_F,     /* Increment the rental timer item */
    NV_FUNC_ENUM_PAD = 0x7FFF     /* Pad to 16 bits on ARM */
  } nv_func_enum_type;
#endif


/*  Returned status codes for requested operation.                         */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
  typedef enum {
    NV_DONE_S,          /* Request completed okay */
    NV_BUSY_S,          /* Request is queued */
    NV_BADCMD_S,        /* Unrecognizable command field */
    NV_FULL_S,          /* The NVM is full */
    NV_FAIL_S,          /* Command failed, reason other than NVM was full */
    NV_NOTACTIVE_S,     /* Variable was not active */
    NV_BADPARM_S,       /* Bad parameter in command block */
    NV_READONLY_S,      /* Parameter is write-protected and thus read only */
    NV_BADTG_S,         /* Item not valid for Target */
    NV_NOMEM_S,         /* free memory exhausted */
    NV_NOTALLOC_S,      /* address is not a valid allocation */
    NV_STAT_ENUM_PAD = 0x7FFF     /* Pad to 16 bits on ARM */
  } nv_stat_enum_type;
#endif


/*  IO operation type.  */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
  typedef enum {
    NV_WRITE_IO,                  /* io write operation */
    NV_READ_IO,                   /* io read operation */
    NV_IO_ENUM_PAD = 0x7FFF       /* Pad to 16 bits on ARM */
  } nv_io_enum_type;
#endif

/*  Dynamic Pool Reservation return type.  */
typedef struct {
  word    amount_reserved;      /* number of bytes actually reserved */
  dword   confirmation_code;    /* confirmation code for the reservation */
} nv_reservation_type;

#define  NV_INVALID_CONF_CODE   0xFFFFFFFF
#define  NV_MAX_RSVD_AMOUNT     0x01C0       /* largest valid request  */

#define NV_MAX_FL_FILE_NUM  	 21          /* 녹음/재생을 위한 정의  */
#define NV_VD_MAX_FL_FILE_NUM	 20          /* 녹음/재생을 위한 정의  */
#define NV_NAME_LTRS             16


#define RD_MODE		0 // read open mode
#define WR_MODE		1 // write open mode
#define APND_MODE	2 // append open mode
#define CLOSE_MODE  3 // closed mode


#ifdef FLASH_1000_CELL /////////////////////////////
	#define NV_FL_EOC					181
#else //////////////////////////////////////////////
	#define NV_FL_EOC					240
#endif // FLASH_1000_CELL //////////////////////////



/*=========================================================================*/

/* Enumerators of items to access.  Each command to the NV task specifies  */
/* the command type with an emumerator from the list below.                */
/* Items for all targets must be specified here, to create a consistant    */
/* numbering of items for external tools such as the PST and DM            */
/*                                                                         */
/* Type "nv_items_enum_type" must be 16 bits in length to conform to       */
/* diagnostic messaging formats.  For compilers that treat enum types      */
/* as non-16-bit entities, FEATURE_NV_ENUMS_ARE_WORDS allows forcing       */
/* this type to the proper size.                                           */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else

typedef enum {

/*-------------------------------------------------------------------------*/

/* Electronic Serial Number.                                               */

  NV_ESN_I = 0,            /* Electronic Serial Number                     */
  NV_ESN_CHKSUM_I,         /* Electronic Serial Number checksum            */

/*-------------------------------------------------------------------------*/

/* NV version numbers.                                                     */

  NV_VERNO_MAJ_I,          /* NV Major version number                      */
  NV_VERNO_MIN_I,          /* NV Minor version number                      */

/*-------------------------------------------------------------------------*/

/* Permanent physical station configuration parameters.                    */

  NV_SCM_I,                /* SCMp                                         */
  NV_SLOT_CYCLE_INDEX_I,   /* Slot cycle index                             */
  NV_MOB_CAI_REV_I,        /* Mobile CAI revision number                   */
  NV_MOB_FIRM_REV_I,       /* Mobile firmware revision number              */
  NV_MOB_MODEL_I,          /* Mobile model                                 */
  NV_CONFIG_CHKSUM_I,      /* Checksum of physical configuration parameters*/

/*-------------------------------------------------------------------------*/

/* Permanent general NAM items.  Each of these is associated with a        */
/* particular NAM (there are up to four NAMs per unit).  The NAM id is     */
/* specified in the request.  Certain analog parameters are truly          */
/* associated with the MIN, but since there is only one MIN per NAM        */
/* in analog mode they are defined as a NAM item.                          */

  NV_PREF_MODE_I,          /* Digital/Analog mode preference        */ /*10*/
  NV_CDMA_PREF_SERV_I,     /* CDMA preferred serving system (A/B)          */
  NV_ANALOG_PREF_SERV_I,   /* Analog preferred serving system (A/B)        */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* The NAM parameters in the block below are protected by the NAM          */
/* checksum and can only be programmed via service programming.            */
/* The NAM parameters in the block above are not protected by checksum     */
/* and may be changed by the user 'on the fly'.                            */

  NV_CDMA_SID_LOCK_I,      /* CDMA SID(s) to lockout                       */
  NV_CDMA_SID_ACQ_I,       /* CDMA SID to acquire                          */
  NV_ANALOG_SID_LOCK_I,    /* ANALOG SID(s) to lockout                     */
  NV_ANALOG_SID_ACQ_I,     /* ANALOG SID to acquire                        */
  NV_ANALOG_FIRSTCHP_I,    /* Analog FIRSTCHPp                             */
  NV_ANALOG_HOME_SID_I,    /* Analog HOME_SIDp                             */
  NV_ANALOG_REG_I,         /* Analog registration setting                  */
  NV_PCDMACH_I,            /* Primary CDMA channel   */                /*20*/
  NV_SCDMACH_I,            /* Secondary CDMA channel                       */
  NV_PPCNCH_I,             /* Primary PCN channel                          */
  NV_SPCNCH_I,             /* Secondary PCN channel                        */
  NV_NAM_CHKSUM_I,         /* NAM checksum                                 */

/*-------------------------------------------------------------------------*/

/* Authentication NAM items.  Each of these is associated with a NAM       */
/* authentication (there are up to four NAMs per unit).  The NAM id is     */
/* specified in the request.                                               */

  NV_A_KEY_I,              /* Authentication A key                         */
  NV_A_KEY_CHKSUM_I,       /* Authentication A key checksum                */
  NV_SSD_A_I,              /* SSD_Asp                                      */
  NV_SSD_A_CHKSUM_I,       /* SSD_Asp checksum                             */
  NV_SSD_B_I,              /* SSD_Bsp                                      */
  NV_SSD_B_CHKSUM_I,       /* SSD_Bsp checksum        */               /*30*/
  NV_COUNT_I,              /* COUNTsp                                      */

/*-------------------------------------------------------------------------*/

/* MIN items.  Each of these is associated with a particular MIN within    */
/* the NAM (there are up to 2 MINs per NAM).  The MIN id itself is         */
/* specified in the request.  When operating in analog mode the first MIN  */
/* is the one and only meaningfull one.                                    */

  NV_MIN1_I,               /* MIN1p                                        */
  NV_MIN2_I,               /* MIN2p                                        */
  NV_MOB_TERM_HOME_I,      /* CDMA MOB_TERM_HOMEp registration flag        */
  NV_MOB_TERM_FOR_SID_I,   /* CDMA MOB_TERM_FOR_SIDp registration flag     */
  NV_MOB_TERM_FOR_NID_I,   /* CDMA MOB_TERM_FOR_NIDp registration flag     */
  NV_ACCOLC_I,             /* ACCOLCp                                      */
  NV_SID_NID_I,            /* CDMA SID/NID pairs                           */
  NV_MIN_CHKSUM_I,         /* MIN checksum                                 */

/*-------------------------------------------------------------------------*/

/* Operational NAM settings.                                               */

  NV_CURR_NAM_I,           /* Current NAM     */                       /*40*/
  NV_ORIG_MIN_I,           /* Call origination MIN within NAM              */
  NV_AUTO_NAM_I,           /* Select NAM automatically on roaming          */
  NV_NAME_NAM_I,           /* A user selectable name for each NAM          */

/*-------------------------------------------------------------------------*/

/* Semi-permanent analog registration parameters.                          */

  NV_NXTREG_I,             /* NXTREGsp                                     */
  NV_LSTSID_I,             /* SIDsp (last SID registered)                  */
  NV_LOCAID_I,             /* LOCAIDsp                                     */
  NV_PUREG_I,              /* PUREGsp                                      */

/*-------------------------------------------------------------------------*/

/* Semi-permanent CDMA registration and channel parameters.                */

  NV_ZONE_LIST_I,          /* ZONE_LISTsp                                  */
  NV_SID_NID_LIST_I,       /* SID_NID_LISTsp                               */
  NV_DIST_REG_I,           /* Distance registration variables   */     /*50*/
  NV_LAST_CDMACH_I,        /* Last CDMA channel acquired                   */

/*-------------------------------------------------------------------------*/

/* Timers, each associated with a specific NAM.                            */

  NV_CALL_TIMER_I,      /* Last call time                                  */
  NV_AIR_TIMER_I,       /* Air time (resettable cummulative call timer)    */
  NV_ROAM_TIMER_I,      /* Roam time (resettable cummulative roam timer)   */
  NV_LIFE_TIMER_I,      /* Life time (non-resettable cumm. call timer)     */

/*-------------------------------------------------------------------------*/

/* Run timer, independent of NAM.                                          */

  NV_RUN_TIMER_I,       /* Run timer (time hardware has been running)      */

/*-------------------------------------------------------------------------*/

/* Memory (speed) dial numbers.                                            */

  NV_DIAL_I,            /* Speed dial number                               */
  NV_STACK_I,           /* Call stack number                               */
  NV_STACK_IDX_I,       /* Call stack last number index                    */

/*-------------------------------------------------------------------------*/

/* Telephone pages (obsolete).                                             */

  NV_PAGE_SET_I,        /* OBSOLETE Page setting     */                /*60*/
  NV_PAGE_MSG_I,        /* OBSOLETE Page message and time                  */

/*-------------------------------------------------------------------------*/

/* Volumes.                                                                */

  NV_EAR_LVL_I,         /* Handset ear piece volume level                  */
  NV_SPEAKER_LVL_I,     /* Handsfree speaker volume level                  */
  NV_RINGER_LVL_I,      /* Ringer volume level                             */
  NV_BEEP_LVL_I,        /* Key beep volume level                           */

/*-------------------------------------------------------------------------*/

/* Tones.                                                                  */

  NV_CALL_BEEP_I,       /* One minute call beeper select                   */
  NV_CONT_KEY_DTMF_I,   /* Continuous keypad DTMF tones select             */
  NV_CONT_STR_DTMF_I,   /* Continuous string (memory) DTMF tones select    */
  NV_SVC_AREA_ALERT_I,  /* Service area enter/exit alert select            */
  NV_CALL_FADE_ALERT_I, /* Call fade alert select     */               /*70*/

/*-------------------------------------------------------------------------*/

/* Various phone settings.                                                 */

  NV_BANNER_I,          /* Idle banner to display                          */
  NV_LCD_I,             /* Display brightness setting                      */
  NV_AUTO_POWER_I,      /* Auto power settings (power savings)             */
  NV_AUTO_ANSWER_I,     /* Auto answer setting                             */
  NV_AUTO_REDIAL_I,     /* Auto redial setting                             */
  NV_AUTO_HYPHEN_I,     /* Auto hyphen setting                             */
  NV_BACK_LIGHT_I,      /* Backlighting manual/auto mode                   */
  NV_AUTO_MUTE_I,       /* Auto radio mute setting                         */

/*-------------------------------------------------------------------------*/

/* Locks and restrictions values.                                          */

  NV_MAINTRSN_I,    /* Base station maintance required reason              */
  NV_LCKRSN_P_I,    /* Base station lock reason, until power cycled */ /*80*/
  NV_LOCK_I,        /* Indicator of whether user locked the phone          */
  NV_LOCK_CODE_I,   /* Lock code string                                    */
  NV_AUTO_LOCK_I,   /* Auto lock setting                                   */
  NV_CALL_RSTRC_I,  /* Call restrictions                                   */
  NV_SEC_CODE_I,    /* Security code                                       */
  NV_HORN_ALERT_I,  /* Horn alert setting                                  */

/*-------------------------------------------------------------------------*/

/* Error log.                                                              */

  NV_ERR_LOG_I,         /* Error log                                       */

/*-------------------------------------------------------------------------*/

/* Miscellaneous items.                                                    */

  NV_UNIT_ID_I,         /* Unit hardware id  (obsolete)                    */
  NV_FREQ_ADJ_I,        /* Frequency adjust values  (obsolete)             */

/*-------------------------------------------------------------------------*/

/* V Battery Min/Max.   (Portable Only Item)                               */

  NV_VBATT_I,           /* V battery regulator array for min/max */    /*90*/

/*-------------------------------------------------------------------------*/

/* Analog (FM) Transmit power levels                                       */

  NV_FM_TX_PWR_I,       /* Analog TX power level array                     */

/*-------------------------------------------------------------------------*/

/* Frequency/temperature offset table item                                 */

  NV_FR_TEMP_OFFSET_I,   

/*-------------------------------------------------------------------------*/

/* DM Port Mode (Mobiles only) NOTE: Use to be NV_EXT_PORT_MODE_I          */

  NV_DM_IO_MODE_I,      /* External port (I/O) mode for DM service         */

/*-------------------------------------------------------------------------*/

/* Portable Turnaround Compensation Tables  (Portable Only)                */

  NV_CDMA_TX_LIMIT_I,   /* To limit TX_GAIN_ADJ when output exceeded       */
  NV_FM_RSSI_I,         /* Analog RSSI adjustment                          */
  NV_CDMA_RIPPLE_I,     /* CDMA UHF Ripple Compensation                    */
  NV_CDMA_RX_OFFSET_I,  /* CDMA RX Offset compensation                     */
  NV_CDMA_RX_POWER_I,   /* CDMA True RX Power Table                        */
  NV_CDMA_RX_ERROR_I,   /* CDMA RX Linear and Non-linear error table       */
  NV_CDMA_TX_SLOPE_1_I, /* CDMA TX Gain Comp Slope Compensation tbl*/ /*100*/
  NV_CDMA_TX_SLOPE_2_I, /* CDMA TX Gain Adjust Slope Compensation table    */
  NV_CDMA_TX_ERROR_I,   /* CDMA TX Non-linear Error Compensation table     */
  NV_PA_CURRENT_CTL_I,  /* PA Current Control table                        */
/*-------------------------------------------------------------------------*/

/* Audio Adjustment values                                                 */

  NV_SONY_ATTEN_1_I,
  NV_SONY_ATTEN_2_I,
  NV_VOC_GAIN_I,

/*-------------------------------------------------------------------------*/

/* Spare items (2) for developer                                           */

  NV_SPARE_1_I,
  NV_SPARE_2_I,

/*-------------------------------------------------------------------------*/

/* Data Services items                                                     */

  NV_DATA_SRVC_STATE_I,   /* Data Service(Task) Enabled/Disabled           */
  NV_DATA_IO_MODE_I,      /* External port (I/O) mode for Data svc */ /*110*/
  NV_IDLE_DATA_TIMEOUT_I, /* Data service idle time in seconds             */

/*-------------------------------------------------------------------------*/

/* Maximum TX adjustment                                                   */

  NV_MAX_TX_ADJ_I,

/*-------------------------------------------------------------------------*/

/* Initial Muting Modes                                                    */

  NV_INI_MUTE_I,

/*-------------------------------------------------------------------------*/

/* Factory free format test buffer                                         */

  NV_FACTORY_INFO_I,

/*-------------------------------------------------------------------------*/

/* Additional Sony Attenuation values                                      */

  NV_SONY_ATTEN_3_I,
  NV_SONY_ATTEN_4_I,
  NV_SONY_ATTEN_5_I,

/*-------------------------------------------------------------------------*/

/* DM address item (for multi-drop HDLC)                                   */

  NV_DM_ADDR_I,
  NV_CDMA_PN_MASK_I,
  NV_SEND_TIMEOUT_I,                                                  /*120*/

/*-------------------------------------------------------------------------*/
/* MSM2P and beyond NV items. */

  NV_FM_AGC_SET_VS_PWR_I,        /* FM TX_AGC_ADJ setting vs power         */
  NV_FM_AGC_SET_VS_FREQ_I,       /* FM TX_AGC_ADJ setting vs frequency     */
  NV_FM_AGC_SET_VS_TEMP_I,       /* FM TX_AGC_ADJ setting vs temperature   */
                                 
  NV_FM_EXP_HDET_VS_PWR_I,       /* FM expected HDET reading vs power      */
  NV_FM_ERR_SLP_VS_PWR_I,        /* FM HDET error slope vs power           */
  NV_FM_FREQ_SENSE_GAIN_I,       /* deviation adj. trim                    */

  NV_CDMA_RX_LIN_OFF_0_I,        /* CDMA Rx linearizer offset              */
  NV_CDMA_RX_LIN_SLP_I,          /* CDMA Rx linearizer slope               */

  NV_CDMA_RX_COMP_VS_FREQ_I,     /* CDMA Rx gain comp vs frequency         */
  NV_CDMA_TX_COMP_VS_FREQ_I,     /* CDMA Tx gain comp vs frequency */ /*130*/
  NV_CDMA_TX_COMP_VS_VOLT_I,     /* CDMA Tx gain comp vs voltage           */

  NV_CDMA_TX_LIN_MASTER_OFF_0_I, /* CDMA Tx linearizer master offset       */
  NV_CDMA_TX_LIN_MASTER_SLP_I,   /* CDMA Tx linearizer master slope        */
  NV_CDMA_TX_LIN_VS_TEMP_I,      /* CDMA Tx linearizer trim vs temp        */
  NV_CDMA_TX_LIN_VS_VOLT_I,      /* CDMA Tx linearizer trim vs voltage     */                                          

  NV_CDMA_TX_LIM_VS_TEMP_I,      /* CDMA Tx power limit vs temperature     */
  NV_CDMA_TX_LIM_VS_VOLT_I,      /* CDMA Tx power limit vs voltage         */
  NV_CDMA_TX_LIM_VS_FREQ_I,      /* CDMA Tx power limit vs frequency       */
  NV_CDMA_EXP_HDET_VS_AGC_I,     /* CDMA expected HDET reading vs AGC PDM  */
  NV_CDMA_ERR_SLP_VS_HDET_I,  /*CDMA HDET err slope vs HDET reading*/ /*140*/

  NV_THERM_I,                    /* RF & LCD temp. based compensation      */
  NV_VBATT_PA_I,                 /* RF comp. based on voltage              */
  NV_HDET_OFF_I,                 /* ADC HDET reading offset                */
  NV_HDET_SPN_I,                 /* ADC HDET reading span                  */
  NV_ONETOUCH_DIAL_I,            /* ena/dis UI one touch dialing           */

  NV_FM_AGC_ADJ_VS_FREQ_I,
  NV_FM_AGC_ADJ_VS_TEMP_I,
  NV_RF_CONFIG_I,
  NV_R1_RISE_I,
  NV_R1_FALL_I,                                                       /*150*/
  NV_R2_RISE_I,                                                
  NV_R2_FALL_I,
  NV_R3_RISE_I,
  NV_R3_FALL_I,

  NV_PA_RANGE_STEP_CAL_I,
  NV_LNA_RANGE_POL_I,
  NV_LNA_RANGE_RISE_I,
  NV_LNA_RANGE_FALL_I,
  NV_LNA_RANGE_OFFSET_I,

  NV_POWER_CYCLES_I,                                                  /*160*/
  NV_ALERTS_LVL_I,
  NV_ALERTS_LVL_SHADOW_I,
  NV_RINGER_LVL_SHADOW_I,
  NV_BEEP_LVL_SHADOW_I,
  NV_EAR_LVL_SHADOW_I,
  NV_TIME_SHOW_I,
  NV_MESSAGE_ALERT_I,
  NV_AIR_CNT_I,
  NV_ROAM_CNT_I,
  NV_LIFE_CNT_I,                                                      /*170*/
  NV_SEND_PIN_I,
  NV_AUTO_ANSWER_SHADOW_I,
  NV_AUTO_REDIAL_SHADOW_I,
  NV_SMS_I,
  NV_SMS_DM_I,
  NV_IMSI_MCC_I,
  NV_IMSI_11_12_I,
  NV_DIR_NUMBER_I,
  NV_VOICE_PRIV_I,
  NV_SPARE_B1_I,                                                      /*180*/
  NV_SPARE_B2_I, 
  NV_SPARE_W1_I,                         
  NV_SPARE_W2_I,
  NV_FSC_I,
  NV_ALARMS_I,
  NV_STANDING_ALARM_I,
  NV_ISD_STD_PASSWD_I,
  NV_ISD_STD_RESTRICT_I,
  NV_DIALING_PLAN_I,
  NV_FM_LNA_CTL_I,                                                    /*190*/
  NV_LIFE_TIMER_G_I,
  NV_CALL_TIMER_G_I,                    
  NV_PWR_DWN_CNT_I,
  NV_FM_AGC_I,
  NV_FSC2_I,
  NV_FSC2_CHKSUM_I,
  NV_WDC_I,
  NV_HW_CONFIG_I,

/*-------------------------------------------------------------------------*/
/* MSM2P and beyond NV items. (continued)  */

  NV_CDMA_RX_LIN_VS_TEMP_I,            /* CDMA Rx linearizer vs temperature*/
  NV_CDMA_ADJ_FACTOR_I,                /* CDMA adjustment factor */   /*200*/
  NV_CDMA_TX_LIM_BOOSTER_OFF_I,
  NV_CDMA_RX_SLP_VS_TEMP_I,            
  NV_CDMA_TX_SLP_VS_TEMP_I,
  NV_PA_RANGE_VS_TEMP_I,
  NV_LNA_SWITCH_VS_TEMP_I,
  NV_FM_EXP_HDET_VS_TEMP_I,
  NV_N1M_I,  

/*-------------------------------------------------------------------------*/
/* TMSI NAM parameters and extensions.                                     */

  NV_IMSI_I,                        /* OBSOLETE InternationaMobileStationID*/
  NV_IMSI_ADDR_NUM_I,                  /* Length of IMSI                   */
  NV_ASSIGNING_TMSI_ZONE_LEN_I,        /* TMSI_ZONE_LENs-p  */        /*210*/
  NV_ASSIGNING_TMSI_ZONE_I,            /* TMSI_ZONEs-p                     */
  NV_TMSI_CODE_I,                      /* TMSI_CODEs-p                     */
  NV_TMSI_EXP_I,                       /* TMSI expiration time             */
  NV_HOME_PCS_FREQ_BLOCK_I,            /* Subscriber's home block          */
  NV_DIR_NUMBER_PCS_I,                 /* Directory number, PCS format     */

/*-------------------------------------------------------------------------*/
/* Roaming List and MRU Table.                                             */

  NV_ROAMING_LIST_I,                   /* The roaming list                 */
  NV_MRU_TABLE_I,                      /* Most recently used channels      */
                                       /*  NOTE: This item is obsolete     */

/*-------------------------------------------------------------------------*/

  NV_REDIAL_I,                 /* Last number redial                       */
  NV_OTKSL_I,                  /* One-time keypad subsidy lock             */
  NV_TIMED_PREF_MODE_I,        /* To allow analog-only operation */   /*220*/

/*-------------------------------------------------------------------------*/

  NV_RINGER_TYPE_I,            /* Ringer type setting                      */
  NV_ANY_KEY_ANSWER_I,         /* Answer call with any key                 */
  NV_BACK_LIGHT_HFK_I,         /* Hands Free Kit backlight setting         */
  NV_RESTRICT_GLOBAL_I,        /* Global phone book restriction            */
  NV_KEY_SOUND_I,              /* Type of keypress sound emitted           */
  NV_DIALS_SORTING_METHOD_I,   /* Phone book sorting method                */
  NV_LANGUAGE_SELECTION_I,     /* Language for user interface              */
  NV_MENU_FORMAT_I,            /* Type of user interface menus             */

/*-------------------------------------------------------------------------*/

  NV_RINGER_SPKR_LVL_I,        /* External speaker ringer volume           */
  NV_BEEP_SPKR_LVL_I,          /* External speaker key beep volume */ /*230*/

/*-------------------------------------------------------------------------*/

  NV_MRU2_TABLE_I,             /* New format, Most Recently Used channels  */
  NV_VIBRATOR_I,               /* Use vibrator instead of ringer           */
  NV_FLIP_ANSWERS_I,           /* Opening clamshell answers call           */

/*-------------------------------------------------------------------------*/

  NV_DIAL_RESTRICT_LVLS_I,     /* ISS2 dialing restriction levels          */
  NV_DIAL_STATE_TABLE_LEN_I,   /* Number elements in state table           */
  NV_DIAL_STATE_TABLE_I,       /* ISS2 dialing plan state table            */

/*-------------------------------------------------------------------------*/

  NV_VOICE_PRIV_ALERT_I,       /* Voice privacy alert for TGP              */

/*-------------------------------------------------------------------------*/

  NV_IP_ADDRESS_I,             /* IP/Port address  (obsolete)              */
  NV_CURR_GATEWAY_I,           /* Last active IP address (obsolete)        */

/*-------------------------------------------------------------------------*/

  NV_DATA_QNC_ENABLED_I,       /* QNC enabled flag                 */ /*240*/
  NV_DATA_SO_SET_I,            /* Which Service Option set is in effect    */

/*-------------------------------------------------------------------------*/

  NV_UP_LINK_INFO_I,           /* IP addresses and key data                */
  NV_UP_PARMS_I,               /* Current gateway, alert state, etc.       */
  NV_UP_CACHE_I,               /* The Unwired Planet cache                 */

/*-------------------------------------------------------------------------*/

  NV_ELAPSED_TIME_I,           /* Timer for formal test use                */

/*-------------------------------------------------------------------------*/

  NV_PDM2_I,                   /* For RF Cal use                           */
  NV_RX_AGC_MINMAX_I,          /* Receiver AGC min-max                     */
  NV_VBATT_AUX_I,              /* Auxillary VBATT min-max                  */

  NV_DTACO_CONTROL_I,            /* DTACO enabled/disabled control         */
  NV_DTACO_INTERDIGIT_TIMEOUT_I, /* DTACO interdigit timeout       */ /*250*/

  NV_PDM1_I,                   /* For RF Cal use                           */
  NV_BELL_MODEM_I,             /* Flag for ISS2                            */

  NV_PDM1_VS_TEMP_I,           /* For RF Cal use                           */
  NV_PDM2_VS_TEMP_I,           /* For RF Cal use                           */

/*-------------------------------------------------------------------------*/

  NV_SID_NID_LOCK_I,           /* CDMA SID(s) and NID(s) to lockout        */
  NV_PRL_ENABLED_I,            /* If the roaming list is enabled or not    */
  NV_ROAMING_LIST_683_I,       /* IS-683-A format roaming list             */  
  NV_SYSTEM_PREF_I,            /* System Preference, per NAM               */
  NV_HOME_SID_NID_I,           /* "Home" SID/NID list                      */
  NV_OTAPA_ENABLED_I,          /* Whether OTAPA is enabled, per NAM*/ /*260*/
  NV_NAM_LOCK_I,               /* SPASM protection per NAM                 */
  NV_IMSI_T_S1_I,              /* True IMSI - MIN1                         */
  NV_IMSI_T_S2_I,              /* True IMSI - MIN2                         */
  NV_IMSI_T_MCC_I,             /* True IMSI - MCC                          */
  NV_IMSI_T_11_12_I,           /* True IMSI - 11th & 12th digits           */
  NV_IMSI_T_ADDR_NUM_I,        /* True IMSI - Addr num                     */

/*-------------------------------------------------------------------------*/

  NV_UP_ALERTS_I,              /* Unwired Planet alert records             */
  NV_UP_IDLE_TIMER_I,          /* Idle time timer                          */

  NV_SMS_UTC_I,                /* Specifies format of SMS time display     */
  NV_ROAM_RINGER_I,            /* Specifies roam ringer on or off  */ /*270*/
  NV_RENTAL_TIMER_I,           /* The rental timer item                    */
  NV_RENTAL_TIMER_INC_I,       /* The rental timer increment value         */
  NV_RENTAL_CNT_I,             /* The rental counter                       */
  NV_RENTAL_TIMER_ENABLED_I,   /* Enables/disables the rental timer        */
  NV_FULL_SYSTEM_PREF_I,       /* Controls system pref display options     */

/*-------------------------------------------------------------------------*/

  NV_BORSCHT_RINGER_FREQ_I,    /* Ringer freq used for BORSCHT port (RJ-11)*/
  NV_PAYPHONE_ENABLE_I,        /* Enable/disable payphone support          */
  NV_DSP_ANSWER_DET_ENABLE_I,  /* Enable/disable dsp answer detection      */
  NV_EVRC_PRI_I,               /* EVRC/13K priority: EVRC first, 13K first */
  NV_AFAX_CLASS_20_I,          /* Obsolete item                    */ /*280*/
  NV_V52_CONTROL_I,            /* V52 control option                       */
  NV_CARRIER_INFO_I,           /* Bitmap and ASCII name info for carrier   */
  NV_AFAX_I,                   /* Analog FAX type: end to end or class 2.0 */
  NV_SIO_PWRDWN_I,             /* Distinguishes old/new UART hardware      */

/*-------------------------------------------------------------------------*/

#ifdef FEATURE_EVRC_SO_MGMT
  NV_PREF_VOICE_SO_I,          /* EVRC voice service options               */
#else
  NV_RSVD_ITEM_1_I,            /* Reserved item                            */
#endif
  NV_VRHFK_ENABLED_I,          /* Voice recognition hands free kit enabled */
  NV_VRHFK_VOICE_ANSWER_I,     /* Voice recognition voice answer           */
  NV_PDM1_VS_FREQ_I,           /* For RF Cal use                           */
  NV_PDM2_VS_FREQ_I,           /* For RF Cal use                           */
  NV_SMS_AUTO_DELETE_I,        /* SMS auto-deletion enabled status */ /*290*/
  NV_SRDA_ENABLED_I,           /* Silent redial enabled status             */
  NV_OUTPUT_UI_KEYS_I,         /* Enable-disable sending UI keystrokes     */
  NV_POL_REV_TIMEOUT_I,        /* Timeout for polarity reversal            */

/*-------------------------------------------------------------------------*/
 
  NV_SI_TEST_DATA_1_I,         /* First stack-checker diagnostic buffer    */
  NV_SI_TEST_DATA_2_I,         /* Second stack-checker diagnostic buffer   */
  NV_SPC_CHANGE_ENABLED_I,     /* Enable-disable OTASP SPC change          */

  NV_DATA_MDR_MODE_I,          /* Select the MDR mode                      */
  NV_DATA_PKT_ORIG_STR_I,      /* Dial string for originating packet calls */

  NV_UP_KEY_I,                 /* Unwired Planet access key item           */
  NV_DATA_AUTO_PACKET_DETECTION_I, /* Packet data configuration item       */
  NV_AUTO_VOLUME_ENABLED_I,    /* Enable-disable auto volume               */
  NV_WILDCARD_SID_I,           /* Allow wildcard SID                       */
  NV_ROAM_MSG_I,               /* Downloadable roaming messages            */
  NV_OTKSL_FLAG_I,             /* OTKSL Flag                               */
  NV_BROWSER_TYPE_I,           /* Browser identifier                       */ 
  NV_SMS_REMINDER_TONE_I,      /* Reminder tone is on or off               */
   
/*-------------------------------------------------------------------------*/
  
  NV_UBROWSER_I,               /* Micro browser data                       */
  NV_BTF_ADJUST_I,             /* BTF adjustment value                     */
  NV_FULL_PREF_MODE_I,         /* Controls pref display options            */
  NV_UP_BROWSER_WARN_I,        /* Confirmation screen option       */ /*310*/
  NV_FM_HDET_ADC_RANGE_I,      /* ADC range for HDET sampling (analog)     */
  NV_CDMA_HDET_ADC_RANGE_I,    /* ADC range for HDET sampling (CDMA)       */
  NV_PN_ID_I,                  /* PN code selection                        */
  NV_USER_ZONE_ENABLED_I,      /* Enable-disable User Zone table           */
  NV_USER_ZONE_I,              /* User Zone table                          */
  NV_PAP_DATA_I,               /* Password Authentication Protocol data    */
  NV_DATA_DEFAULT_PROFILE_I,   /* Default user AT command profile          */
  NV_PAP_USER_ID_I,            /* User_ID for Password Auth. Protocol      */
  NV_PAP_PASSWORD_I,           /* Actual password for PAP                  */
  NV_STA_TBYE_I,               /* Num wakeup samples below thrshld */ /*320*/
  NV_STA_MIN_THR_I,            /* Threshold for Rx+Ec/Io (RSSI) trigger    */
  NV_STA_MIN_RX_I,             /* Threshold for Rx trigger                 */
  NV_STA_MIN_ECIO_I,           /* Threshold for Rx-Only idle trigger       */
  NV_STA_PRI_I,                /* Switch to AMPS PRI setting               */

/*-------------------------------------------------------------------------*/

  NV_PCS_RX_LIN_OFF_0_I,       /* PCS Rx linearizer offset                 */
  NV_PCS_RX_LIN_SLP_I,         /* PCS Rx linearizer slope                  */
  NV_PCS_RX_COMP_VS_FREQ_I,    /* PCS Rx gain comp vs frequency            */
  NV_PCS_TX_COMP_VS_FREQ_I,    /* PCS Tx gain comp vs frequency            */
  NV_PCS_TX_LIN_MASTER_OFF_0_I,/* PCS Tx linearizer master offset          */
  NV_PCS_TX_LIN_MASTER_SLP_I,  /* PCS Tx linearizer master slope   */ /*330*/
  NV_PCS_TX_LIN_VS_TEMP_I,     /* PCS Tx linearizer trim vs temp           */
  NV_PCS_TX_LIM_VS_TEMP_I,     /* PCS Tx power limit vs temperature        */
  NV_PCS_TX_LIM_VS_FREQ_I,     /* PCS Tx power limit vs frequency          */
  NV_PCS_EXP_HDET_VS_AGC_I,    /* PCS expected HDET reading vs AGC PDM     */
  NV_PCS_HDET_OFF_I,           /* ADC HDET reading offset                  */
  NV_PCS_HDET_SPN_I,           /* ADC HDET reading span                    */
  NV_PCS_R1_RISE_I,            /* TX pwr lvl at which PA is stepped up     */
  NV_PCS_R1_FALL_I,            /* TX pwr lvl at which PA is stepped down   */
  NV_PCS_R2_RISE_I,            /* TX pwr lvl at which PA is stepped up     */
  NV_PCS_R2_FALL_I,            /* TX pwr lvl at which PA stpd down */ /*340*/
  NV_PCS_R3_RISE_I,            /* TX pwr lvl at which PA is stepped up     */
  NV_PCS_R3_FALL_I,            /* TX pwr lvl at which PA is stepped down   */
  NV_PCS_PA_RANGE_STEP_CAL_I,  /* Calibrate PA Range gain step             */
  NV_PCS_PDM1_VS_FREQ_I,       /* PDM1 vs frequency compensation table     */
  NV_PCS_PDM2_VS_FREQ_I,       /* PDM2 vs frequency compensation table     */
  NV_PCS_LNA_RANGE_POL_I,      /* Polarity of LNA range control signal     */
  NV_PCS_LNA_RANGE_RISE_I,     /* Rx pwr lvl for: LNA bypass               */
  NV_PCS_LNA_RANGE_FALL_I,     /* Rx pwr lvl for: LNA turned on            */
  NV_PCS_LNA_RANGE_OFFSET_I,   /* Rx pwr lvl offset when LNA is bypassed   */
  NV_PCS_RX_LIN_VS_TEMP_I,     /* PCS Rx linearizer vs temperature */ /*350*/
  NV_PCS_ADJ_FACTOR_I,         /* PCS adjustment factor                    */
  NV_PCS_PA_RANGE_VS_TEMP_I,   /* Changes in PA_RANGE_STEP over temp       */
  NV_PCS_PDM1_VS_TEMP_I,       /* TX temperature compensation using PDM1   */
  NV_PCS_PDM2_VS_TEMP_I,       /* TX temperature compensation using PDM2   */
  NV_PCS_RX_SLP_VS_TEMP_I,     /* Slope of RX linearizer over temperature  */
  NV_PCS_TX_SLP_VS_TEMP_I,     /* Slope of TX linearizer over temperature  */
  NV_PCS_RX_AGC_MINMAX_I,      /* Receiver AGC min-max                     */

/*-------------------------------------------------------------------------*/

  NV_PA_OFFSETS_I,
  NV_CDMA_TX_LIN_MASTER_I,     /* Obsolete tx linearizer item              */

  NV_VEXT_I,                                                          /*360*/
  NV_VLCD_ADC_CNT_I,
  NV_VLCD_DRVR_CNT_I,
  NV_VREF_ADJ_PDM_CNT_I,
  NV_IBAT_PER_LSB_I,
  NV_IEXT_I,
  NV_IEXT_THR_I,

  NV_CDMA_TX_LIN_MASTER0_I,
  NV_CDMA_TX_LIN_MASTER1_I,
  NV_CDMA_TX_LIN_MASTER2_I,
  NV_CDMA_TX_LIN_MASTER3_I,                                           /*370*/
  NV_TIME_FMT_SELECTION_I,

/*-------------------------------------------------------------------------*/

  NV_SMS_BC_SERVICE_TABLE_SIZE_I,
  NV_SMS_BC_SERVICE_TABLE_I,
  NV_SMS_BC_CONFIG_I,
  NV_SMS_BC_USER_PREF_I,

/*-------------------------------------------------------------------------*/

/* RFR3100 items */
  NV_LNA_RANGE_2_RISE_I,
  NV_LNA_RANGE_2_FALL_I,
  NV_LNA_RANGE_12_OFFSET_I,
  NV_NONBYPASS_TIMER_I,
  NV_BYPASS_TIMER_I,                                                  /*380*/
  NV_IM_LEVEL1_I,
  NV_IM_LEVEL2_I,
  NV_CDMA_LNA_OFFSET_VS_FREQ_I,
  NV_CDMA_LNA_12_OFFSET_VS_FREQ_I,
  NV_AGC_PHASE_OFFSET_I,
  NV_RX_AGC_MIN_11_I,

/*-------------------------------------------------------------------------*/

/* Trimode - RFR3100 items */
  NV_PCS_LNA_RANGE_2_RISE_I,
  NV_PCS_LNA_RANGE_2_FALL_I,
  NV_PCS_LNA_RANGE_12_OFFSET_I,
  NV_PCS_NONBYPASS_TIMER_I,                                           /*390*/
  NV_PCS_BYPASS_TIMER_I,                                              
  NV_PCS_IM_LEVEL1_I,
  NV_PCS_IM_LEVEL2_I,
  NV_PCS_CDMA_LNA_OFFSET_VS_FREQ_I,
  NV_PCS_CDMA_LNA_12_OFFSET_VS_FREQ_I,
  NV_PCS_AGC_PHASE_OFFSET_I,
  NV_PCS_RX_AGC_MIN_11_I,
  
  NV_RUIM_CHV_1_I,                 /* Card holder verification 1 for R-UIM */
  NV_RUIM_CHV_2_I,                 /* Card holder verification 2 for R-UIM */
  
  NV_GPS1_CAPABILITIES_I,                          /* GPS One Capabilities */
  NV_GPS1_PDE_ADDRESS_I,                        /* GPS One PDE TCP Address */ 
  NV_GPS1_ALLOWED_I,   /* GPS One Position Determination Services Lock-out */
  NV_GPS1_PDE_TRANSPORT_I,        /* GPS One Preferred transport mechanism */
  NV_GPS1_MOBILE_CALC_I,
                      /* GPS One Mobile vs PDE based Position Calculations */

  NV_PREF_FOR_RC_I,        /* IS2000 CAI radio configuration RC preference */
  NV_DS_DEFAULT_BAUD_I,             /* DATA SERVICES default SIO baud rate */  
  NV_DIAG_DEFAULT_BAUD_I,                    /* DIAG default SIO baud rate */                
  NV_SIO_DEV_MAP_MENU_ITEM_I,
               /* Serial Device Mapper configuration menu item information */

  NV_TTY_I,                /* Specifies whether TTY is enabled or disabled */
  NV_PA_RANGE_OFFSETS_I,                                                
      /* Digitally compensate for PA gain steps in each of the 4 PA states */
  NV_TX_COMP0_I,  /* For temp. and freq. compensation of the Tx linearizer */  
  
  NV_MM_SDAC_LVL_I,                  /* Stereo DAC Multimedia volume level */
  NV_BEEP_SDAC_LVL_I,                  /* Stereo DAC key beep volume level */
  NV_SDAC_LVL_I,                                /* Stereo DAC volume level */
  NV_MM_LVL_I,                          /* Handset Multimedia volume level */  
  NV_MM_LVL_SHADOW_I,                   /* Headset Multimedia volume level */
  NV_MM_SPEAKER_LVL_I,                      /* HFK Multimedia volume level */
  NV_MM_PLAY_MODE_I,                               /* Multimedia play mode */
  NV_MM_REPEAT_MODE_I,                           /* Multimedia repeat mode */
  NV_TX_COMP1_I,  /* For temp. and freq. compensation of the Tx linearizer */  
  NV_TX_COMP2_I,  /* For temp. and freq. compensation of the Tx linearizer */  
  NV_TX_COMP3_I,  /* For temp. and freq. compensation of the Tx linearizer */  
  NV_PRIMARY_DNS_I,           /* Contains the IP Address of the DNS Server */  
  NV_SECONDARY_DNS_I,
                    /* Contains the IP Address of the Secondary DNS Server */
  NV_DIAG_PORT_SELECT_I,              /* Info for DIAG boot port selection */
  NV_GPS1_PDE_PORT_I,        /* Listening port associated with PDE address */
  NV_MM_RINGER_FILE_I,                       /* Multimedia ringer filename */
  NV_MM_STORAGE_DEVICE_I,                      /* Multimedia file location */
  
  NV_DATA_SCRM_ENABLED_I, /* Enables/disables the mobile's ability to SCRM */
  NV_RUIM_SMS_STATUS_I,                                              /*    */
         /* For storing information required by R-UIM SMSS Elementary File */
  NV_PCS_TX_LIN_MASTER0_I,        /* PCS Tx linearizer with internal PA=00 */
  NV_PCS_TX_LIN_MASTER1_I,        /* PCS Tx linearizer with internal PA=01 */
  NV_PCS_TX_LIN_MASTER2_I,        /* PCS Tx linearizer with internal PA=10 */
  NV_PCS_TX_LIN_MASTER3_I,        /* PCS Tx linearizer with internal PA=11 */
                                 
  NV_PCS_PA_RANGE_OFFSETS_I,                       /* PCS PA range offsets */
  NV_PCS_TX_COMP0_I,        /* PCS transmit frequency compensation table 0 */  
  NV_PCS_TX_COMP1_I,        /* PCS transmit frequency compensation table 1 */  
  NV_PCS_TX_COMP2_I,        /* PCS transmit frequency compensation table 2 */  
  NV_PCS_TX_COMP3_I,        /* PCS transmit frequency compensation table 3 */
  
  NV_DIAG_RESTART_CONFIG_I, 
                       /* One-time startup configuration for DIAG services */
  
  NV_BAND_PREF_I,   /* Stores the band-class preference on a per NAM basis */
  NV_ROAM_PREF_I,      /* Stores the roaming preference on a per NAM basis */
  NV_GPS1_GPS_RF_DELAY_I,                           /* GPS RF Signal Delay */
  /* The len. of time taken for a GPS signal to pass through the RF chain  */
   
  NV_GPS1_CDMA_RF_DELAY_I,                         /* CDMA RF Signal Delay */
  /* The len. of time taken for a CDMA signal to pass through the RF chain */
  
  NV_PCS_ENC_BTF_I,
   /* CHIPX8 delay for SYNC80M via 1900mhz PCS path in RF card, biased +25 */
  NV_CDMA_ENC_BTF_I,
   /* CHIPX8 delay for SYNC80M via 800mhz CDMA path in RF card, biased +25 */
  NV_BD_ADDR_I,               /* Holds the Bluetooth address of the mobile */
  NV_SUBPCG_PA_WARMUP_DELAY_I,
                      /* Provides the less-than-one-PCG warm-up for the PA */
  NV_GPS1_GPS_RF_LOSS_I,        /* RF Loss in GPR RF Chain in 0.1 DB Units */
  NV_DATA_TRTL_ENABLED_I, 
                        /* In IS2000, if mobile should self-throttle R-SCH */
  NV_AMPS_BACKSTOP_ENABLED_I,       /* AMPS backstop system enabled status */
  
  NV_GPS1_LOCK_I,                                   /* GPS One lock status */ 
  NV_FTM_MODE_I,    /* Determines boot up mode of a factory testmode phone */
  /* DATA SERVICES default SIO baud rate (obsoletes NV_DS_DEFAULT_BAUD_I)  */
  NV_DS_DEFAULT_BAUDRATE_I, 
  /* DIAG default SIO baud rate     (obsoletes NV_DIAG_DEFAULT_BAUD_I)     */
  NV_DIAG_DEFAULT_BAUDRATE_I,      
  
//  NV_JCDMA_DS_F92_I,                  /* Stores JCDMA F92 option selected  */
 
#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
  NV_REF_TEMPSENSE_I,
  NV_CDMA_FREQ_VS_PWR_I,
  NV_PA_PWR_I,
#endif		// #ifdef FEATURE_SKTT_CAL
  NV_FN_SEQUENCE_I,
  NV_VOICE_INFO_I,
  NV_DATA_IPR_MODE_I,          /* white 2001.03.20 */
  NV_DATA_REV_MODE_I,          /* white 2001.03.20 */
  NV_FLIP_COUNT_I,             /* woosdtok 01.5.3  */
  NV_MULTI_SID_I,              /* hjcho 01.09.17 for IRM2.0 */

  /* reo 00-07-27 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)//def FEATURE_EQUS_A10B1
#if 0  
    NV_SMSRCV_I,
    NV_SMSRCV_NUM_I,
#endif
    NV_SMSSEND_I, 
    NV_SMSSEND_NUM_I,
    NV_SMSSENT_I,
    NV_SMSSENT_NUM_I,
    NV_VMSRCV_I,
    NV_VMS_NUM_I,
    NV_MYBELL_I,
    NV_MYBELL_NUM_I,
    NV_LIKEPHRASE_I,
    NV_LIKEPHRASE_NUM_I,
    NV_SMSOPTION_I,
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */  

  /* <-- */
  NV_MAX_I,                    /* Size of this enum, MUST be the last item */
                               /* (except for padding)                     */
/*-------------------------------------------------------------------------*/

  NV_PHONETAG_I = 0x7000,      /* 공장용 */

  NV_ITEMS_ENUM_PAD = 0x7FFF   /* Pad to 16 bits on ARM                    */

} nv_items_enum_type; 

#endif


/***************************************************************************/
/*                                                                         */
/* NV Features                                                             */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* FEATURE_NV_ONE_NAM_RL_LARGE   Specifies a single NAM with a large       */
/*                                 roaming list.                           */
/* FEATURE_NV_TWO_NAMS_RL_MEDIUM Specifies two NAMs with medium-sized      */
/*                                 roaming lists.                          */
/* FEATURE_NV_TWO_NAMS_RL_SMALL  Specifies two NAMs with small roaming     */
/*                                 lists.                                  */
/* FEATURE_NV_JSTD008            Enables J-STD-008 parameters(obsolete).   */
/* FEATURE_NV_TMSI               Enables TMSI parameters.                  */
/* NV_FEATURE_SSPR_ENHANCEMENTS  Enables SSPR-800 items and modifications. */
/* FEATURE_IS683A_PRL            Selects IS-683 format roaming list.       */
/* NV_FEATURE_ROAMING_LIST_683   Enables IS-683 format roaming list.       */
/* FEATURE_NV_1K_RL              Specifies a 1K roaming list regardless of */
/*                                 autosize results.                       */
/* NV_FEATURE_200_1K_RL          Specifies 200 byte roaming list(s) for 8K */
/*                                 phones, 1K roaming list(s) for 16K.     */
/* NV_FEATURE_PRL_ITEMS          Enables roaming list and related items.   */
/* NV_FEATURE_IMSI_T_ITEMS       Enables True IMSI item support.           */
/* NV_FEATURE_ANS_SHADOW_ITEMS   Enables AUTO_ANSWER_SHADOW item support.  */
/* NV_FEATURE_SIO_PWRDWN_ITEMS   Enables NV_SIO_PWRDWN item support.       */
/* FEATURE_NV_ANALOG             Enables analog calling parameters (number */
/*                                 of NAMs depends on target).             */
/* FEATURE_NV_OTASP              Enables OTASP parameters.                 */
/* FEATURE_NV_RF_CAL             Enables RF calibration parameters.        */
/* FEATURE_NV_DIAL_PLAN          Enables ISS2 dialing plan parameters.     */
/* FEATURE_NV_ALARMS             Enables ISS2 alarm parameters.            */
/* FEATURE_NV_FULL_DIAL_TYPE     Enables status and/or time stamp fields   */
/*                                 in dial type and stack dial type items. */
/* FEATURE_NV_EXTENDED_UI        Enables TGP-unique items and the duration */
/*                                 field in stack dial type items.         */
/* FEATURE_NV_REDIAL_ITEM        Enables "last number redial" item.        */
/* NV_FEATURE_RENTAL_ITEMS       Enables rental timer items.               */
/* NV_FEATURE_BORSCHT_ITEMS      Enables Borscht/payphone/afax, etc items. */
/* NV_FEATURE_TPR_ITEMS          Enables Timeout-for-Polarity-Reversal     */
/*                                 item for ISS2.                          */
/* FEATURE_NV_CARRIER_ITEMS      Enables Carrier Info items.               */
/* FEATURE_NV_ATTENUATION        Enables audio attenuation items.          */
/* FEATURE_NV_SI_DATA            Enables stack-checker diagnostic items    */
/* FEATURE_NV_FACTORY_INFO       Enables the factory info item (note: not  */
/*                                 the same as the free-form factory info  */
/*                                 data area).                             */
/* FEATURE_NV_DYNAMIC_ITEMS      Enables dynamic pool operations for       */
/*                                 certain non-SMS items.                  */
/* NV_FEATURE_WEB_ITEMS          Enables web browser items.                */
/* NV_FEATURE_BELL_MODEM         Enables the BELL_MODEM item for ISS2.     */
/* NV_FEATURE_PDM_TABLES         Enables the PDM1/2_VS_TEMP tables and     */
/*                                 disables PA_RANGE_VS_TEMP.              */
/* NV_FEATURE_I2C                Specifies use of I2C library routines for */
/*                                 accessing the EEPROM.                   */
/* FEATURE_NV_24LC32             Specifies model 24LC32 NV part in use.    */
/* FEATURE_NV_24LC65             Specifies model 24LC65 NV part in use.    */
/* FEATURE_NV_CHECKPOINT         Enables cache checkpointing for glitch    */
/*                                 recovery.                               */
/* FEATURE_NV_DEADCELL_RECOVERY  Enables deadcell recovery.                */
/* FEATURE_NV_UNIT_TEST          Enables unit test code.                   */
/*                                                                         */
/***************************************************************************/

#ifdef FEATURE_NV_TMSI
#error Cannot externally #define FEATURE_NV_TMSI
#endif
#ifdef NV_FEATURE_SSPR_ENHANCEMENTS
#error Cannot externally #define NV_FEATURE_SSPR_ENHANCEMENTS
#endif
#ifdef NV_FEATURE_ROAMING_LIST_683
#error Cannot externally #define NV_FEATURE_ROAMING_LIST_683
#endif
#ifdef FEATURE_NV_1K_RL
#error code not present
#endif
#ifdef NV_FEATURE_200_1K_RL 
#error Cannot externally #define NV_FEATURE_200_1K_RL 
#endif
#ifdef NV_FEATURE_PRL_ITEMS
#error Cannot externally #define NV_FEATURE_PRL_ITEMS
#endif
#ifdef NV_FEATURE_IMSI_T_ITEMS
#error Cannot externally #define NV_FEATURE_IMSI_T_ITEMS
#endif
#ifdef NV_FEATURE_ANS_SHADOW_ITEMS
#error code not present
#endif
#ifdef NV_FEATURE_SIO_PWRDWN_ITEMS
#error code not present
#endif
#ifdef FEATURE_NV_ANALOG
#error Cannot externally #define FEATURE_NV_ANALOG
#endif
#ifdef FEATURE_NV_OTASP
#error Cannot externally #define FEATURE_NV_OTASP
#endif
#ifdef FEATURE_NV_RF_CAL
#error Cannot externally #define FEATURE_NV_RF_CAL
#endif
#ifdef FEATURE_NV_DIAL_PLAN
#error code not present
#endif
#ifdef FEATURE_NV_ALARMS
#error code not present
#endif
#ifdef FEATURE_NV_FULL_DIAL_TYPE
#error Cannot externally #define FEATURE_NV_FULL_DIAL_TYPE
#endif
#ifdef FEATURE_NV_EXTENDED_UI
#error Cannot externally #define FEATURE_NV_EXTENDED_UI
#endif
#ifdef FEATURE_NV_REDIAL_ITEM
#error Cannot externally #define FEATURE_NV_REDIAL_ITEM
#endif
#ifdef NV_FEATURE_RENTAL_ITEMS
#error code not present
#endif
#ifdef NV_FEATURE_BORSCHT_ITEMS
#error code not present
#endif
#ifdef NV_FEATURE_TPR_ITEMS
#error code not present
#endif
#ifdef NV_FEATURE_CARRIER_ITEMS
#error code not present
#endif
#ifdef FEATURE_NV_ATTENUATION
#error Cannot externally #define FEATURE_NV_ATTENUATION
#endif
#ifdef NV_FEATURE_SI_DATA  
#error code not present
#endif
#ifdef FEATURE_NV_FACTORY_INFO
#error Cannot externally #define FEATURE_NV_FACTORY_INFO
#endif
#ifdef FEATURE_NV_DYNAMIC_ITEMS
#error Cannot externally #define FEATURE_NV_DYNAMIC_ITEMS
#endif
#ifdef NV_FEATURE_WEB_ITEMS
#error code not present
#endif
#ifdef NV_FEATURE_BELL_MODEM
#error code not present
#endif
#ifdef NV_FEATURE_PDM_TABLES
#error Cannot externally #define NV_FEATURE_PDM_TABLES
#endif
#ifdef NV_FEATURE_I2C
#error code not present
#endif
#ifdef FEATURE_NV_24LC32
#error code not present
#endif
#ifdef FEATURE_NV_24LC65
#error Cannot externally #define FEATURE_NV_24LC65
#endif
#ifdef NV_FEATURE_DYNAMIC_NVDIV
#error code not present
#endif
#ifdef FEATURE_NV_CHECKPOINT
#error Cannot externally #define FEATURE_NV_CHECKPOINT
#endif
#ifdef FEATURE_NV_DEADCELL_RECOVERY
#error Cannot externally #define FEATURE_NV_DEADCELL_RECOVERY
#endif
#ifdef FEATURE_NV_UNIT_TEST
#error code not present
#endif

/* Set internal NV feature defines based on inputs:  */ 
/*   TG (the specified target),                      */
/*   FEATURE_NV_OTASP,                               */
/*   FEATURE_NV_ONE_NAM_RL_LARGE,                    */
/*   FEATURE_NV_TWO_NAMS_RL_MEDIUM,                  */
/*   FEATURE_NV_TWO_NAMS_RL_SMALL, and               */ 
/*   FEATURE_NV_WEB_ITEMS                            */ 

/*=========================================================================*/
/* Common defines for all Targets                                          */
/*=========================================================================*/

#ifdef FEATURE_NV_I2C
#error code not present
#endif  

/* Until clients are ready to use the dynamic pool reservation system, */
/* this define will automatically handle reservations for the caller.  */
/* Once the clients are ready to use reservations, remove this define  */
/* and the associated code in file nvw.c.                              */
#ifndef FEATURE_UI_SMS_NV_RESERVATION
  #define NV_TEMP_AUTO_RESERVATIONS
#endif

/*=========================================================================*/
/* Target T_Q, T_G                                                         */
/*=========================================================================*/
#if ((TG==T_Q) || (TG==T_G) || (TG==T_O))

  #if (TG==T_O)
#error code not present
  #endif

#ifdef FEATURE_OTASP
    #define FEATURE_NV_OTASP
#endif
    
#ifdef FEATURE_TRIMODE_ITEMS
#error code not present
#endif
    
#ifdef FEATURE_CDMA_1900
    #ifdef T_MSM5000
      #define NV_FEATURE_PDM_TABLES
    #else
      #undef NV_FEATURE_PDM_TABLES
    #endif
    #define NV_FEATURE_PRL_ITEMS        /* Enable roaming list support */
    #define FEATURE_NV_TMSI             /* Enable TMSI parameters */
    
    #ifdef FEATURE_NV_ONE_NAM_RL_LARGE
      #define  NV_MAX_NAMS     1        /* One NAM supported */

      /* One-NAM configuration uses a 4K PRL by default */
      #define FEATURE_NV_4K_RL
 
      #ifdef FEATURE_IS683A_PRL
        #define NV_FEATURE_ROAMING_LIST_683   /* Use IS-683A format PRL */
      
        #ifdef FEATURE_NV_WEB_ITEMS  /* Digby */
#error code not present
        #else  
          #ifdef FEATURE_CDMA_800  /* Trimode */
            #define NV_FEATURE_TRIMODE_ITEMS
            #define FEATURE_NV_ANALOG 
            #define NV_FEATURE_IMSI_T_ITEMS
            #define NV_FEATURE_SSPR_ENHANCEMENTS 
            /* Set NV version numbers for this feature configuration */
            #define NVI_KLT_REV      0x81
            #define NVI_VERNO_MIN    0xE4
          #else
            #define NVI_KLT_REV      0x81
            #define NVI_VERNO_MIN    0xE5
          #endif  
        #endif
        
      #else
          #error This VU does not support this target/feature combination
      #endif
    #else
    
      #define FEATURE_NV_TWO_NAMS_RL_SMALL    /* Default */
      #define  NV_MAX_NAMS     2        /* Up to 2 NAMs allowed */

      #ifdef FEATURE_IS683A_PRL
        #define NV_FEATURE_ROAMING_LIST_683   /* Use IS-683A format PRL */
        #define NV_FEATURE_IMSI_T_ITEMS       /* IS683A/IS95B imply IMSI_T */
        /* The above was added for IS-95B True IMSI support. However, this
        ** also gives True IMSI OTASP download support for OTASP enabled
        ** PCS targets.
        */
      
        #ifdef FEATURE_ACP
          #define FEATURE_NV_ANALOG /* Define analog parameters for Dualband */
          /* Set NV version numbers for this feature configuration [??] */
          
          #ifdef FEATURE_NV_WEB_ITEMS  /* Digby */
#error code not present
          #else /* FEATURE_NV_WEB_ITEMS */ 
            #define NVI_KLT_REV      0x81
            #define NVI_VERNO_MIN    0xE7
          #endif
          
        #else /* FEATURE_ACP */ 
          #ifdef FEATURE_NV_WEB_ITEMS
#error code not present
          #endif
          
          #define NVI_KLT_REV      0x81
          #define NVI_VERNO_MIN    0xE8
        #endif /* FEATURE_ACP */
        
      #else /* FEATURE_IS683A_PRL */
      
        #ifdef FEATURE_NV_WEB_ITEMS
#error code not present
        #endif
          
        #ifdef FEATURE_SSPR_ENHANCEMENTS
          #error This VU does not support this target/feature combination
        #else
          #ifdef FEATURE_ACP
            #define FEATURE_NV_ANALOG
            /* Set NV version numbers for this feature configuration [??] */
            #define NVI_KLT_REV      0x81
            #define NVI_VERNO_MIN    0xE9
          #else /* FEATURE_ACP */
            /* Set NV version numbers for this feature configuration [??] */
            #define NVI_KLT_REV      0x81
            #define NVI_VERNO_MIN    0xEA
          #endif /* FEATURE_ACP */
            
        #endif /* FEATURE_SSPR_ENHANCEMENTS */
      
      #endif /* FEATURE_IS683A_PRL */
    #endif /* FEATURE_NV_ONE_NAM_RL_LARGE */

    #define  NV_MAX_SID_NID    1        /* For 1 SID+NID pair */

#else /* FEATURE_CDMA_1900 */

    #define FEATURE_NV_ANALOG       /* Enable analog parameters */
    #define FEATURE_NV_TMSI         /* Enable TMSI parameters hack */
    /* Note the above definition is for all P_REVs in the cellular
    ** band. This is not required, but doesn't hurt anything for now.
    ** Just ensure an NV rebuild, by changing to a unique NVI_VERNO_MIN
    ** below.
    */

    #ifdef FEATURE_SSPR_ENHANCEMENTS
    
      #ifdef FEATURE_NV_ONE_NAM_RL_LARGE
      
        #define  NV_MAX_NAMS     1
        #define  NV_MAX_SID_NID  1
        #define  NV_FEATURE_PRL_ITEMS     /* Enable roaming list support */
        
        #ifdef FEATURE_IS683A_PRL
          #ifdef FEATURE_NV_WEB_ITEMS
#error code not present
          #endif

          #define NV_FEATURE_ROAMING_LIST_683   /* Use IS-683A format PRL */
          #define NV_FEATURE_IMSI_T_ITEMS       /* IS683A implies IMSI_T */
          #define NV_FEATURE_SSPR_ENHANCEMENTS
          
          #define NVI_KLT_REV    0x81
          #define NVI_VERNO_MIN  0xEB
          
        #else /* FEATURE_IS683A_PRL */
          #error This VU does not support this target/feature combination
        #endif /* FEATURE_IS683A_PRL */
        
      #elif defined(FEATURE_NV_TWO_NAMS_RL_SMALL)
    
#ifdef FEATURE_NAM_5  /* hjcho - 01.04.19 */
        #define  NV_MAX_NAMS     5        /* Up to 5 NAMs allowed */
#else
        #define  NV_MAX_NAMS     4        /* Up to 4 NAMs allowed */
#endif
        #define  NV_MAX_SID_NID  1        /* For 1 SID+NID pair */
        #define  NV_FEATURE_PRL_ITEMS     /* Enable roaming list support */

        #ifdef FEATURE_IS683A_PRL
          #ifdef FEATURE_NV_WEB_ITEMS
#error code not present
          #endif
          
          #define NV_FEATURE_ROAMING_LIST_683   /* Use IS-683A format PRL */
          #define NV_FEATURE_IMSI_T_ITEMS       /* IS683A implies IMSI_T */
          #define NV_FEATURE_SSPR_ENHANCEMENTS
          
          #define NVI_KLT_REV    0x81
          #define NVI_VERNO_MIN  0xEB
          
        #else /* FEATURE_IS683A_PRL */
          #error This VU does not support this target/feature combination
        #endif /* FEATURE_IS683A_PRL */
        
      #elif defined(FEATURE_NV_TWO_NAMS_RL_MEDIUM)
#error code not present
      #else
        #error No RL Type defined!!
      #endif
        
    #else /* FEATURE_SSPR_ENHANCEMENTS */
      /* NV Definitions to support Old Style System Determination with Cellular Targets */
    
      #define  NV_MAX_NAMS     4        /* Up to 4 NAMs allowed */
      #define  NV_MAX_SID_NID  4        /* Up to 4 SID+NID pairs */
    
      /* Set NV version numbers for this feature configuration [34] */
      #define NVI_KLT_REV      0x81
      #define NVI_VERNO_MIN    0xED

    #endif /* FEATURE_SSPR_ENHANCEMENTS */

  #endif /* !FEATURE_CDMA_1900 */


  /* Standard features for target O/Q/Gemini */
  #define FEATURE_NV_RF_CAL
  #define FEATURE_NV_FULL_DIAL_TYPE
  #define FEATURE_NV_REDIAL_ITEM
#if (TG == T_Q)
#error code not present
#endif
#if (TG == T_O)
#error code not present
#endif
  #define FEATURE_NV_EXTENDED_UI
  #define FEATURE_NV_24LC65
  #define FEATURE_NV_CHECKPOINT
  #define FEATURE_NV_DEADCELL_RECOVERY

  /* Target-specific constants for O/Q/Gemini */
  #define  NV_MAX_SPEED_DIALS  109      /* Maximum of 109 speed dials */
  #define  NV_MAX_LTRS         12       /* With up to 12-letter names */
  //#define  NV_MAX_STACK_DIALS  10       /* Maximum 10 call stack numbers */
#ifdef FEATURE_CALL_STACK_SEPERATED
  #define  NV_MAX_STACK_DIALS  30//20
#else
  #define  NV_MAX_STACK_DIALS  40
#endif//  #define  NV_MAX_STACK_DIALS  40



/*=========================================================================*/
/* Target T_PC                                                             */
/*=========================================================================*/
#elif (TG==T_PC) 
#error code not present
#elif ((TG==T_DM) || (TG==T_SP))
#error code not present
#else
#error code not present
#endif

/* Generate values for nvi_verno_ext_type to track new feature definitions 
   so that a rebuild will occur when feature selection changes */
/* This code will be obsoleted with the new NV meta compiler */

/* Length of variable sized part of extended version designator */
#define NVI_VERNO_LENGTH       NVI_VERNO_EXT_LENGTH
/* General revision number for feature bits assignment */
#define NVI_VERNO_GENERAL_REV  0x0002

/* Version designator feature bits - byte 0 */
#ifdef FEATURE_BROADCAST_SMS
  #define NV_FEATURE_BROADCAST_SMS_ENABLED 0x80 
#else
  #define NV_FEATURE_BROADCAST_SMS_ENABLED 0
#endif
#ifdef FEATURE_RFR3100
#error code not present
#else
  #define NV_FEATURE_RFR3100_ENABLED 0
#endif
#ifdef FEATURE_OTASP_OTAPA
  #define NV_FEATURE_OTASP_OTAPA_ENABLED 0x20 
#else
  #define NV_FEATURE_OTASP_OTAPA_ENABLED 0
#endif
#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
  #define NV_FEATURE_DS_PAP_ENABLED 0x10 
#else
  #define NV_FEATURE_DS_PAP_ENABLED 0
#endif
#ifdef FEATURE_PA_RANGE_DIG_COMP
  #define NV_FEATURE_PA_RANGE_DIG_COMP_ENABLED 0x08
#else
  #define NV_FEATURE_PA_RANGE_DIG_COMP_ENABLED 0
#endif
#ifdef FEATURE_4PAGE_TX_LINEARIZER
  #define NV_FEATURE_4PAGE_TX_LINEARIZER_ENABLED 0x04
#else
  #define NV_FEATURE_4PAGE_TX_LINEARIZER_ENABLED 0
#endif
#ifdef FEATURE_RUIM_CHV_FROM_NV
  #define NV_FEATURE_RUIM_CHV_FROM_NV_ENABLED 0x02
#else
  #define NV_FEATURE_RUIM_CHV_FROM_NV_ENABLED 0
#endif
#ifdef FEATURE_IS2000
  #define NV_FEATURE_IS2000_ENABLED 0x01
#else
  #define NV_FEATURE_IS2000_ENABLED 0
#endif

#define  NVI_VERNO_EXT_BYTE_0                                    \
     (NV_FEATURE_BROADCAST_SMS_ENABLED | NV_FEATURE_RFR3100_ENABLED |  \
      NV_FEATURE_OTASP_OTAPA_ENABLED | NV_FEATURE_DS_PAP_ENABLED    |  \
      NV_FEATURE_PA_RANGE_DIG_COMP_ENABLED                       |  \
      NV_FEATURE_4PAGE_TX_LINEARIZER_ENABLED                     |  \
      NV_FEATURE_RUIM_CHV_FROM_NV_ENABLED                        |  \
      NV_FEATURE_IS2000_ENABLED)                                 
      

/* Version designator feature bits - byte 1 */
#ifdef FEATURE_DS_DEFAULT_BITRATE_NV
  #define NV_FEATURE_DS_DEFAULT_BITRATE_NV_ENABLED 0x80
#else
  #define NV_FEATURE_DS_DEFAULT_BITRATE_NV_ENABLED 0
#endif
#ifdef FEATURE_DIAG_DEFAULT_BITRATE_NV
  #define NV_FEATURE_DIAG_DEFAULT_BITRATE_NV_ENABLED 0x40
#else
  #define NV_FEATURE_DIAG_DEFAULT_BITRATE_NV_ENABLED 0
#endif
#ifdef FEATURE_SDEVMAP_MENU_ITEM_NV
  #define NV_FEATURE_SDEVMAP_MENU_ITEM_NV_ENABLED 0x20
#else
  #define NV_FEATURE_SDEVMAP_MENU_ITEM_NV_ENABLED 0
#endif
#ifdef FEATURE_TTY
#error code not present
#else
  #define NV_FEATURE_TTY_ENABLED 0
#endif
#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
  #define NV_FEATURE_PA_RANGE_TEMP_FREQ_COMP_ENABLED 0x08
#else 
  #define NV_FEATURE_PA_RANGE_TEMP_FREQ_COMP_ENABLED 0
#endif
#ifdef FEATURE_STEREO_DAC
#error code not present
#else
  #define NV_FEATURE_STEREO_DAC_ENABLED 0
#endif
#ifdef FEATURE_MULTIMEDIA
#error code not present
#else
  #define NV_FEATURE_MULTIMEDIA_ENABLED 0
#endif
#ifdef FEATURE_DIAG_PORT_SELECT
  #define NV_FEATURE_DIAG_PORT_SELECT_ENABLED 0X01
#else
  #define NV_FEATURE_DIAG_PORT_SELECT_ENABLED 0
#endif

#define  NVI_VERNO_EXT_BYTE_1                                    \
     (NV_FEATURE_DS_DEFAULT_BITRATE_NV_ENABLED                   |  \
      NV_FEATURE_DIAG_DEFAULT_BITRATE_NV_ENABLED                 |  \
      NV_FEATURE_SDEVMAP_MENU_ITEM_NV_ENABLED                    |  \
      NV_FEATURE_TTY_ENABLED                                     |  \
      NV_FEATURE_PA_RANGE_TEMP_FREQ_COMP_ENABLED                 |  \
      NV_FEATURE_STEREO_DAC_ENABLED                              |  \
      NV_FEATURE_MULTIMEDIA_ENABLED                              |  \
      NV_FEATURE_DIAG_PORT_SELECT_ENABLED)

/* Version designator feature bits - byte 2 */
#ifdef FEATURE_GPSONE
#error code not present
#else
  #define NV_FEATURE_GPSONE_ENABLED 0
#endif
#ifdef FEATURE_MMC
#error code not present
#else
  #define NV_FEATURE_MMC_ENABLED 0
#endif
#ifdef FEATURE_UBROWSER
#error code not present
#else
  #define NV_FEATURE_UBROWSER_ENABLED 0
#endif
#ifdef FEATURE_RUIM_SMSS
  #define NV_FEATURE_RUIM_SMSS_ENABLED 0x10
#else
  #define NV_FEATURE_RUIM_SMSS_ENABLED 0
#endif
#ifdef FEATURE_DIAG_RESTART_CONFIG
  #define NV_FEATURE_DIAG_RESTART_CONFIG_ENABLED 0x08
#else
  #define NV_FEATURE_DIAG_RESTART_CONFIG_ENABLED 0
#endif
#ifdef FEATURE_ENC_BTF_IN_NV
#error code not present
#else
  #define NV_FEATURE_ENC_BTF_IN_NV_ENABLED 0
#endif
#ifdef FEATURE_IS2000_R_SCH
  #define NV_FEATURE_IS2000_R_SCH_ENABLED 0x02
#else
  #define NV_FEATURE_IS2000_R_SCH_ENABLED 0
#endif
#ifdef FEATURE_SD20
#error code not present
#else
  #define NV_FEATURE_SD20_ENABLED 0
#endif

#define NVI_VERNO_EXT_BYTE_2                                     \
     (NV_FEATURE_GPSONE_ENABLED                                  |  \
      NV_FEATURE_MMC_ENABLED                                     |  \
      NV_FEATURE_UBROWSER_ENABLED                                |  \
      NV_FEATURE_RUIM_SMSS_ENABLED                               |  \
      NV_FEATURE_DIAG_RESTART_CONFIG_ENABLED                     |  \
      NV_FEATURE_ENC_BTF_IN_NV_ENABLED                           |  \
      NV_FEATURE_IS2000_R_SCH_ENABLED                            |  \
      NV_FEATURE_SD20_ENABLED)
      
#ifdef FEATURE_SUBPCG_PA_WARMUP_DELAY
#error code not present
#else
  #define NV_FEATURE_SUBPCG_PA_WARMUP_DELAY_ENABLED 0
#endif
#ifdef FEATURE_TRIMODE_ITEMS
#error code not present
#else
  #define NV_FEATURE_TRIMODE_ITEMS_ENABLED 0
#endif
#ifdef FEATURE_NV_4K_RL
  #define NV_FEATURE_4K_RL_ENABLED 0x20
#else
  #define NV_FEATURE_4K_RL_ENABLED 0
#endif
#ifdef FEATURE_FACTORY_TESTMODE
  #define NV_FEATURE_FACTORY_TESTMODE_ENABLED 0x10
#else
  #define NV_FEATURE_FACTORY_TESTMODE_ENABLED 0
#endif
#ifdef FEATURE_JCDMA_DS
#error code not present
#else
  #define NV_FEATURE_JCDMA_DS_ENABLED 0
#endif

/* Version designator feature bits - byte 3 */
#define NVI_VERNO_EXT_BYTE_3                                     \
     (NV_FEATURE_SUBPCG_PA_WARMUP_DELAY_ENABLED                  |  \
      NV_FEATURE_TRIMODE_ITEMS_ENABLED                           |  \
      NV_FEATURE_4K_RL_ENABLED                                   |  \
      NV_FEATURE_FACTORY_TESTMODE_ENABLED                        |  \
      NV_FEATURE_JCDMA_DS_ENABLED)

/*=========================================================================*/
/* End of NV Feature Defines Section                                       */
/*=========================================================================*/


/*=========================================================================*/

/* Following are all the types to be used in message.  Unless otherwise    */
/* specified all fields are right justified.  All nv.h structures are      */
/* packed(1).  This is done so that external and internal NV storage       */
/* representation is as identical as possible and it minimizes storage     */
/* requirements.                                                           */

/*-------------------------------------------------------------------------*/

/* Various definitions to be used in subsequent types.                     */

#define     NV_DIR_NUMB_SIZ     10        /* num digits in dir_number */
#define     NV_DIR_NUMB_PCS_SIZ 15        /* num digits in dir_number_pcs */
#define     NV_WDC_LTRS          5        /* With up to 5 letters wdc */
#define     NV_FSC_SIZ           6        /*number digits in FSC */
#define     NV_MAX_PAGE_MSGS    20        /* Maximum 20 page messages */
#define     NV_MAX_DIAL_DIGITS  32        /* Maximum 32 digit number */    
#define     NV_SMS_DATA_SIZ    388        /* Max bytes in SMS data buffer */
/* DM uses a reduced data size to avoid *huge* packets for serial interface */
#define     NV_SMS_DM_DATA_SIZ 100        /* Max bytes in SMS DM data buffer */

#define     NV_MAX_MINS          2        /* Up to 2 MINs per NAM allowed */
#define     NV_MAX_HOME_SID_NID 20        /* Up to 20 "home" SID/NID pairs */
#define     NV_MAX_SID_LOCK      6        /* Up to 6 SIDs to lock out */
#define     NV_MAX_SID_NID_LOCK 10        /* Up to 10 SID/NIDs to lock out */
#define     NV_MAX_SID_ACQ       6        /* Up to 6 SIDs to acquire */
#define     NV_MAX_ERR_LOG      20        /* Maximum 20 logs in error log */
#define     NV_ERR_FILE_NAME_LEN 8        /* 8 characters file name length */
#define     NV_CDMA_MIN_INDEX    1        /* CDMA MIN index */
#define     NV_ANALOG_MIN_INDEX  0        /* Analog MIN index */
#define     NV_MAX_ALARMS        6        /* One time alarms for DOT */
#define     NV_ISD_STD_PASSWD_SIZ 4       /* Bytes in passwd */

/* reo 00-07-27 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)  || (MODEL_ID == MODEL_B2)//def FEATURE_EQUS_A10B1 

#define     MAX_SMS_SEND_MSG    10 //발신 실패 메시지 
#define     MAX_SMS_SENT_MSG        10 //발신 성공 메시지 
#define     MAX_SMS_MSG_LEN         80 //SMS Message length
#define     MAX_NUM_MULTI_SEND      10 //한번에 전송할 수 있는 사람 최대 수 
#define     MAX_DIAL_DIGIT          32 // NUMEDIT LIMIT
#define     MAX_SMS_RCV_MSG         50 

#define     MAX_SMS_RCV_VMS         20 // huni vms 20 (10 ->20)
#define     MAX_SMS_RCV_MYBELL      10
#define     MAX_MYBELL_DATA         80
#define     MAX_LIKEPHRASE_NUM      20
#define     MAX_LIKEPHRASE_LEN          MAX_SMS_MSG_LEN

#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
/* <-- */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* All items marked by a checksum, except the ESN take a simple checksum.  */
/* If set as defined below it denotes that the item has been programmed    */
/* to NV and is valid.  Any other value denotes that the item is invalid.  */
/* The ESN checksum is hidden from the user, and the flag returned, as     */
/* defined below, indicates if the ESN checksum is valid.                  */

#define     NV_VALID_GEN_CHKSUM     0xFFFF
#define     NV_VALID_ESN_CHKSUM     0xFFFFFFFFL

/*-------------------------------------------------------------------------*/

/* Type to hold ESN.  The ESN and its checksum are write-once protected    */
/* and the NV task will not write to NV an ESN or its checksum if the      */
/* stored ESN or its checksum are non-zero.                                */

typedef PACKED struct {
  dword     esn;
} nv_esn_type;

typedef PACKED struct {
  dword     fsc2;
} nv_fsc2_type;

/*-------------------------------------------------------------------------*/

/* Type to hold ESN checksum.  The checksum is 30 bit CRC with generator   */
/* polynomial as used for CDMA sync channel signaling, per CAI section     */
/* 7.7.1.2.2.  The ESN and its checksum are write-once protected and the   */
/* NV task will not write to NV an ESN or its checksum if the stored ESN,  */
/* or its checksum, or the internal valid flag are not valid.  This item   */
/* can be written only internally, by the NV task.  The status of this     */
/* item can be read externally by other tasks to verify that the ESN has   */
/* been correctly programmed to NV.  An NV_VALID_ESN_CHKSUM value returned */
/* indicates valid ESN checksum.                                           */    

typedef PACKED struct {
  dword     chksum;
} nv_esn_chksum_type;

typedef PACKED struct {
  dword     fsc2_chksum;
} nv_fsc2_chksum_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold configuration checksum.  The checksum is a simple 0xFFFF   */
/* marker which indicates that configuration parameters have been          */
/* programmed and are valid.                                               */

typedef PACKED struct {
  word      chksum;
} nv_config_chksum_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SID and associated NAM.  The SID is 15 bits, per CAI       */
/* section 2.3.8.                                                          */

typedef PACKED struct {
  byte      nam;            /* NAM id 0-N */
  word      sid;            /* SID */
} nv_sid_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold analog/digital preferred mode, and the associated NAM.     */
/* The type is per CAI section 2.3.10.2.                                   */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_MODE_DIGITAL_PREF,     /* CDMA then Analog */
  NV_MODE_DIGITAL_ONLY,     /* CDMA only */
  NV_MODE_ANALOG_PREF,      /* Analog then CDMA */
  NV_MODE_ANALOG_ONLY,      /* Analog only */
  NV_MODE_AUTOMATIC,        /* Determine mode automatically */
  NV_MODE_E911,             /* Emergency mode */
  NV_MODE_HOME_ONLY,        /* Restrict to home only */
  NV_MODE_PCS_CDMA_ONLY,
  NV_MODE_CELL_CDMA_ONLY,
  NV_MODE_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
} nv_mode_enum_type; 
#endif

typedef PACKED struct {
  byte               nam;        /* NAM id 0-N */
  nv_mode_enum_type  mode;       /* Preferred mode per enum above */
} nv_pref_mode_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold preferred serving system and the associated NAM.           */
/* Type is per CAI section 2.3.10.1.                                       */

#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_SYS_A_ONLY=0,          /* A only */
  NV_SYS_A_PREF,            /* A then B */
  NV_SYS_B_ONLY,            /* B only */
  NV_SYS_B_PREF,            /* B then A */
  NV_SYS_HOME_ONLY,         /* Home SID only */
  NV_SYS_HOME_PREF,         /* Home SID preferred */
  NV_SYS_ENUM_PAD = 0x7FFF  /* Pad to 16 bits on ARM */
} nv_sys_enum_type;
#endif

typedef PACKED struct {
  byte               nam;   /* NAM id 0-N */
  nv_sys_enum_type   sys;   /* Preferred system per above enum */
} nv_pref_serv_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the system preference and the associated NAM. This item    */
/* replaces NV_CDMA_PREF_SERV_I and NV_ANALOG_PREF_SERV_I for SSPR 800.    */

#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_SP_A_ONLY=0,          /* A only */
  NV_SP_B_ONLY,            /* B only */
  NV_SP_HOME_ONLY,         /* Home only */
  NV_SP_STANDARD,          /* Home preferred */
  NV_SYS_PREF_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
} nv_sys_pref_enum_type;
#endif

typedef PACKED struct {
  byte                    nam;   /* NAM id 0-N */
  nv_sys_pref_enum_type   sys;   /* Preferred system per above enum */
} nv_sys_pref_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SID(s) for lockout.  The phone will reject acquisition if  */
/* any of these SID(s) is acquired.                                        */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  word      sid[NV_MAX_SID_LOCK];   /* SID */
} nv_sid_lock_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SID(s) for acquisition.  The phone will allow acquisition  */
/* only on these SID(s).                                                   */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  word      sid[NV_MAX_SID_ACQ];    /* SID */
} nv_sid_acq_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold FIRSTCHPp and associated NAM.  Value is 11 bits, per       */
/* CAI section 2.3.7.                                                      */

typedef PACKED struct {
  byte      nam;            /* NAM id 0-N */
  word      channel;        /* FIRSTCHPp channel */
} nv_firstchp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold analog registration type, along with associated NAM id.    */
/* Type includes enumeration for possible registration values.             */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_DISABLED,           /* Autonomous registration disabled               */
  NV_WHEREABOUTS_KNOWN,  /* Autonomuos registration enabled, tell location */
  NV_WHEREABOUTS_UNKNOWN,/* Autonomous registration enabled, hide location */
  NV_ANALOG_REG_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
} nv_analog_reg_enum_type;
#endif

typedef PACKED struct {
  byte                      nam;    /* NAM id 0-N */
  nv_analog_reg_enum_type   kind;   /* Registration kind per enum above */
} nv_analog_reg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold CDMA channel and associated NAM.  Value is 11 bits for     */
/* Primary and Secondary channels, per CAI section 6.1.1.                  */

typedef PACKED struct {
  byte      nam;                 /* NAM id 0-N */
  word      channel_a;           /* A carrier channel number */
  word      channel_b;           /* B carrier channel number */
} nv_cdmach_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold NAM data checksum.  The checksum is a simple 0xFFFF marker */
/* which is set to indicate that NAM parameters have been programmed and   */
/* are valid.                                                              */

typedef PACKED struct {
  byte      nam;                 /* NAM id 0-N */
  word      chksum;              /* The checksum */
} nv_nam_chksum_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold authentication A key and the associated NAM id.  The       */
/* number is 64 bits, per IS-54 appendix A (CAI Appendix X).               */

typedef PACKED struct {
  byte      nam;                 /* NAM id 0-N */
  qword     key;                 /* A key array */
} nv_a_key_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold authentication A key 16 bit CRC checksum with generator    */
/* polynomial as used for CDMA reverse and forward traffic signaling       */
/* messages, per CAI sections 6.7.2.2.2 and 7.7.3.2.  Exact security       */
/* and protection of the A-Key is for now TBD.                             */

typedef PACKED struct {
  byte      nam;                 /* NAM id 0-N */
  word      chksum;              /* Checksum */
} nv_a_key_chksum_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SSD (A or B) variable and the associated NAM id.  Each     */
/* SSD is 64 bits, per CAI sections 2.3.12.1.1. and 6.3.12.1.1.            */

typedef PACKED struct {
  byte      nam;                 /* NAM id 0-N */
  qword     ssd;                 /* SSD array */
} nv_ssd_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SSD (A or B) 16 bit CRC checksum with generator polynomial */
/* as used for CDMA reverse and forward traffic signaling messages, per    */
/* CAI sections 6.7.2.2.2 and 7.7.3.2.                                     */

typedef PACKED struct {
  byte      nam;                 /* NAM id 0-N */
  word      chksum;              /* Checksum */
} nv_ssd_chksum_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the call history COUNT parameter and the associated NAM    */
/* id, per CAI section 2.3.12.1.3.                                         */

typedef PACKED struct {
  byte      nam;                 /* NAM id 0-N */
  byte      count;               /* COUNTsp */
} nv_count_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold MIN1p for 2 MINs, along with the associated NAM id.        */
/* The number is 24 bits, per CAI section 2.3.1.                           */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  dword     min1[NV_MAX_MINS];      /* MIN1 */
} nv_min1_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold MIN2p for 2 MINs, along with the associated NAM id.        */
/* The number is 10 bits, per CAI section 2.3.1.                           */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  word      min2[NV_MAX_MINS];      /* MIN2 */
} nv_min2_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold IMSI MCC , along with the associated NAM id.               */
/* The number is 24 bits                                                   */

typedef PACKED struct {
  byte     nam;                    /* NAM id 0-N */
  word     imsi_mcc;                          
} nv_imsi_mcc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold DIR_NUMBER  with the associated NAM id.                    */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  byte      dir_number[NV_DIR_NUMB_SIZ];                          
} nv_dir_number_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold IMSI_11_12 for 4 MINs, along with the associated NAM id.   */
/* The number is 8 bits.                                                   */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  byte      imsi_11_12;                          
} nv_imsi_11_12_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold CDMA mobile termination type, along with associated NAM,   */
/* to be used for the MOB_TERM_... variables for registration.             */

typedef PACKED struct {
  byte               nam;                    /* NAM id 0-N */
  boolean            enabled[NV_MAX_MINS];   /* Registration enabled */
} nv_mob_term_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold ACCOLCp, along with associated NAM id.  Value is 4 bits,   */
/* per CAI section 2.3.5.                                                  */

typedef PACKED struct {
  byte      nam;                             /* NAM id 0-N */
  #ifdef __cplusplus                         
/* jrkwon 2000-08-19 --> for simulator */
//#error code not present
    byte      class_[NV_MAX_MINS];            /* ACCOLCp class */
/* <-- */
  #else
    byte      class[NV_MAX_MINS];            /* ACCOLCp class */
  #endif
} nv_accolc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SID+NID pairs.  The SID is 15 bits, per CAI 2.3.8, and the */
/* NID is 16 bits, per CAI section 2.3.10.3.                               */

typedef PACKED struct {
  word      sid;
  word      nid;
} nv_sid_nid_pair_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SID+NID pairs for CDMA acquisition, along with NAM id      */
/* NID is 16 bits, per CAI section 2.3.10.3.  There are up to 4 SID+NID    */
/* pairs, in descending preferrence (0=first, 3=last).                     */

typedef PACKED struct {
  byte                 nam;                               /* NAM id 0-N */
  nv_sid_nid_pair_type pair[NV_MAX_MINS][NV_MAX_SID_NID]; /* SID+NID Pair */
} nv_sid_nid_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold "home" SID+NID pairs for CDMA acquisition, along with NAM  */
/* id.  Note that this item is NOT "per-MIN."                              */

typedef PACKED struct {
  byte                 nam;                               /* NAM id 0-N */
  nv_sid_nid_pair_type pair[NV_MAX_HOME_SID_NID];         /* SID+NID Pair */
} nv_home_sid_nid_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SID+NID pairs to be locked out from CDMA acquisition.      */

typedef PACKED struct {
  byte                 nam;                               /* NAM id 0-N */
  nv_sid_nid_pair_type pair[NV_MAX_SID_NID_LOCK];         /* SID+NID Pair */
} nv_sid_nid_lock_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Generic "enabled/disabled per NAM" type used to hold both the roaming   */
/* list enabled indicator and the OTAPA enabled indicator.                 */

typedef PACKED struct {
  byte                 nam;                               /* NAM id 0-N */
  boolean              enabled;
} nv_enabled_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold MIN data checksum.  The checksum is a simple 0xFFFF marker */
/* which specifies that MIN parameters have been programmed and are valid. */

typedef PACKED struct {
  byte      nam;                             /* NAM id 0-N */
  word      chksum[NV_MAX_MINS];             /* Checksum */
} nv_min_chksum_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold ORIG_MIN id, along with the associated NAM id.             */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  byte      id;                     /* MIN id 0-N */
} nv_orig_min_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the name of each NAM, along with the associated NAM id.    */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  byte      name[NV_MAX_LTRS];      /* NAM name string */
} nv_name_nam_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_PCS_BLOCK_A,
  NV_PCS_BLOCK_B,
  NV_PCS_BLOCK_C,
  NV_PCS_BLOCK_D,
  NV_PCS_BLOCK_E,
  NV_PCS_BLOCK_F,
  NV_CELLULAR_SYS_A,
  NV_CELLULAR_SYS_B,
  NV_BLOCK_OR_SYS_INVALID,
  NV_BLOCK_OR_SYS_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
} nv_block_or_sys_enum_type;
#endif

/* Type to hold SID_NID_LIST, per IS-95A section 6.3.4.                    */

typedef PACKED struct {
  word                       sid;
  word                       nid;
  nv_block_or_sys_enum_type  block_or_sys;
  byte                       band_class;
} nv_sid_nid_list_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold ZONE_LIST, per CAI section 6.3.4.                          */

typedef PACKED struct {
  word                       reg_zone;            /* REG_ZONEsp */
  word                       sid;                 /* SIDsp */
  word                       nid;                 /* NIDsp */
  nv_block_or_sys_enum_type  block_or_sys;
  byte                       band_class;
} nv_zone_list_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold distance based registration paremeters, per CAI            */
/* section 6.3.4.                                                          */

typedef PACKED struct {
  dword     base_lat_reg;        /* BASE_LAT_REGsp */
  dword     base_long_reg;       /* BASE_LONG_REGsp */
  word      reg_dist_reg;        /* BASE_DIST_REGsp */
} nv_dist_reg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold call timers and associated NAM id.  Each NAM has its own   */
/* set of these timers, with units as shown.   The range of these timers   */
/* is more then 100 years.                                                 */
/*                                                                         */
/*    Timer             units                                              */
/*    -----             -----                                              */
/*    Call Time         seconds                                            */
/*    Air Time          minutes                                            */
/*    Roam Time         minutes                                            */
/*    Life Time         minutes                                            */

typedef PACKED struct {
  byte      nam;            /* NAM id 0-N */
  dword     time;           /* Time */
} nv_call_time_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold run timer, with units as shown.  The range is a zillion    */
/* years.                                                                  */
/*                                                                         */
/*    Timer             units                                              */
/*    -----             -----                                              */
/*    Run Time          minutes                                            */

typedef PACKED struct
{
  dword     time;       /* Time */
} nv_run_time_type;

typedef PACKED struct {
  byte      nam;            /* NAM id 0-N */
  dword     cnt;            /* count */
} nv_call_cnt_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for rental timer.                                                  */

typedef PACKED struct {
  dword     ticks;            /* total number of "ticks" of the clock */
} nv_rental_timer_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold speed dial number.  'num_digits' and 'num_letters'         */
/* indicate the actual number of digits and letters, or zero if none.      */
/* The storage address is in the range 0-99 for speed dial numbers         */

typedef PACKED struct {
  byte      address;                      /* Storage address */
#ifdef FEATURE_NV_FULL_DIAL_TYPE
  byte      status;                       /* flag bits */
#endif
  byte      num_digits;                   /* Number of digits */
  byte      digits[NV_MAX_DIAL_DIGITS];   /* Array of digits */
  byte      letters[NV_MAX_LTRS];         /* Name associated with number */
} nv_dial_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold stack dial number.  'num_digits' and 'num_letters'         */
/* indicate the actual number of digits and letters, or zero if none.      */
/* The storage address is in the range 0-9 for a stack number              */

typedef PACKED struct {
  byte      address;                      /* Storage address */
#ifdef FEATURE_NV_FULL_DIAL_TYPE
  byte      status;                       /* flag bits */
  dword     time_stamp;                   /* years to seconds */
#endif
  byte      num_digits;                   /* Number of digits */
  byte      digits[NV_MAX_DIAL_DIGITS];   /* Array of digits */
  byte      letters[NV_MAX_LTRS];         /* Name associated with number */
#ifdef FEATURE_NV_EXTENDED_UI
  dword     duration;                     /* Duration of call in seconds */
#endif
} nv_stdial_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold call stack index.  The array of bytes allows sorting the   */
/* call stack by order of dialing and resorting as it is being used.  The  */
/* secret field indicates if a secret number is in that stack location.    */

typedef PACKED struct {
  byte      index[NV_MAX_STACK_DIALS];    /* Index array */
  boolean   secret[NV_MAX_STACK_DIALS];   /* TRUE if a secret number */
} nv_stack_idx_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for holding the dial string used for originating packet data calls */
#define     NV_MAX_PKT_ORIG_DIGITS  16    /* Maximum 16 digit number */    

typedef PACKED struct {
  byte      num_digits;                     /* Number of digits */
  byte      digits[NV_MAX_PKT_ORIG_DIGITS]; /* The digit array */
} nv_data_pkt_orig_str_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify page answer rings and answer mode.                      */

#define NV_PAGE_DISABLE 0
#define NV_PAGE_SILENT  1
#define NV_PAGE_AUDIBLE 2

typedef PACKED struct {
  byte   enable;            /* paging mode */
  byte   rings;             /* Number of rings when to accept page */
} nv_page_set_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold page message.  The page characters are stored as ASCII     */
/* string.                                                                 */

#define  NV_MAX_PAGE_CHARS 20            /* Up to 20 characters in a page */

typedef PACKED struct {
  byte               address;                      /* Storage address 0-19 */
  dword              time_stamp;                   /* Time stamp */
#ifdef __cplusplus                         
/* jrkwon 2000-08-19 --> for simulator */
//#error code not present
  boolean            new_;                          /* New page status */
/* <-- */
#else
  boolean            new;                          /* New page status */
#endif
  byte               num_chars;                    /* Number of characters */
  byte               chars[NV_MAX_PAGE_CHARS];     /* Array of characters */
} nv_page_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold display banner.  The banner is fixed length and is stored  */
/* as ASCII string.                                                        */

typedef PACKED struct {
  byte      letters[NV_MAX_LTRS];            /* Banner */
} nv_banner_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef PACKED struct {
  byte      wdc[NV_WDC_LTRS];            /* Warranty date code */
  byte      extra[3];                    /* in case the wdc def. changes */
} nv_wdc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold Field Service Code is fixed length and is stored           */
/* as ASCII string.                                                        */

typedef PACKED struct {
  byte      fsc[NV_FSC_SIZ];            /* Field Service Code */
} nv_fsc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the standing alarms for India DOT                          */

typedef PACKED struct {
  byte       alarm_id;                  /* which standing alarm */
  dword      alarms;                    /* DOT alarms */
} nv_alarms_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the standing alarms for India DOT                          */

typedef PACKED struct {
  byte      isd_std_passwd[NV_ISD_STD_PASSWD_SIZ];     
} nv_isd_std_passwd_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify auto answer rings and enable/disable.                   */

typedef PACKED struct {
  boolean   enable;            /* TRUE if auto answer enabled */
  byte      rings;             /* Number of rings when to answer call */
} nv_auto_answer_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify auto redial rings and enable/disable.                   */

typedef PACKED struct {
  boolean   enable;      /* TRUE if auto redial enabled */
  byte      rings;       /* Number of 10 seconds system busy before redial */
} nv_auto_redial_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold maintenance order and reason code.  Maintenance reason is  */
/* per IS-95 section 7.7.4.                                                */

typedef PACKED struct {
  boolean maint_order;   /* Set when maintenance order received            */
  byte    maint_reason;  /* Maintenance reason received in the order       */
} nv_maintrsn_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold lock order and reason code.  Lock reason is per CAI        */
/* section 7.7.4.                                                          */

typedef PACKED struct {
  boolean lock_order;   /* Set when lock order received and until unlocked */
  byte    lock_reason;  /* Lock reason received in the order */
} nv_lckrsn_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold user lock code.  The lock code is fixed length and is      */
/* stored as ASCII string.                                                 */

#define  NV_LOCK_CODE_SIZE 4              /* 4 digit lock code */

typedef PACKED struct {
  byte      digits[NV_LOCK_CODE_SIZE];    /* Lock code array */
} nv_lock_code_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold security code.  The security code is fixed length and is   */
/* stored as ASCII string.                                                 */

#define  NV_SEC_CODE_SIZE 6               /* 6 digit security code */

typedef PACKED struct {
  byte      digits[NV_SEC_CODE_SIZE];     /* Security code array */
} nv_sec_code_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold error log.  Up to NV_MAX_ERR_LOG error codes are stored,   */
/* each with number of times detected, file name, and line number.         */

typedef PACKED struct {
  byte      address;                         /* Storage address 0 to       */
                                             /*  NV_MAX_ERR_LOG-1          */
  byte      err_count;                       /* Number of occurances       */
                                             /* (0=empty,FF=full)          */
  byte      file_name[NV_ERR_FILE_NAME_LEN]; /* File name string */
  word      line_num;                        /* Line number in file */  
  boolean   fatal;                           /* TRUE if fatal error */
} nv_err_log_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for peek.  Can peek at up to 32 bytes in one request.              */

#define  NV_MAX_PEEK_SIZE  32 

typedef PACKED struct {
  word      address;                   /* Where to start peek from */
  byte      length;                    /* How many bytes to peek */
  byte      mem[NV_MAX_PEEK_SIZE];     /* Returned peek data */
} nv_peek_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for poke.  Can poke up to 32 bytes in one request.                 */

#define  NV_MAX_POKE_SIZE  32 

typedef PACKED struct {
  word      address;                   /* Where to start poke */
  byte      length;                    /* How many bytes to poke */
  byte      mem[NV_MAX_POKE_SIZE];     /* Poke data */
} nv_poke_type;


/*  NV_VOICE_INFO_I   00.12.19 hjcho  */
typedef PACKED struct {
	dword	remained_byte;
	word	user_voice_l; 
	word	remained_time;
	byte	total_rec_num;
	byte	new_ans_num;
	boolean is_user_voice;
	byte	user_voice_fn;
} nv_voice_info_type;


/*   NV_FN_SEQUENCE_I 00.12.19 hjcho */
typedef PACKED struct {
	byte		address;		//Storage address
	word		Time;			// time (sec)
	byte		Fn;				//file number
	byte		Type;			// file type
} nv_fn_sequence_type;


/*  01.09.17 for NV_MULTI_SID_I */
#define  NV_MAX_CH_LIST_SIZE  4 

typedef PACKED struct {
    byte        nSIDNum;
    byte        nSelIndex;       //only from UI
    word        awChList[NV_MAX_CH_LIST_SIZE];
    word        awSIDList[NV_MAX_CH_LIST_SIZE];
} nv_multi_sid_type;


typedef	PACKED struct {
  dword phoneTAG;
  char  processTAG[2];
  dword SN;
  dword OEM_SN;
  char  LineCode[10][2];
  char  ModelCode[3];
  char  ColorCode[2];
  char  FirstDate[12];
  char  LastDate[12];
} nv_phoneTAG_type;


/* reo 00-07-27 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)  || (MODEL_ID == MODEL_B2)//def  FEATURE_EQUS_A10B1
/*----------------받은 문자 메시지 NV 저장 format------------------------*/
#if 0
typedef PACKED struct {
    byte                unAddr;                         //Storage address
    byte                unUsed;                         //Ox01:Used/UnUsed
    word                nID;                            //Message ID : USED IN CM
    boolean             bCheckTime;                     //Timefield가 있는지 조사 
    char                Time[4];                        //Receive time                        
    byte                unAttr;                          /*
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
}nv_smsrcv_type;
#endif
/*---------보낼 메시지/ 보낸 메시지 NV 저장 format-------------------------*/
typedef PACKED struct {
    byte                unAddr;                             //Storage address
    byte                unUsed;                             //0x01: Used/UnUsed                                                                                                               
    char                Time[4];                            //SMS Made time
    byte                unMultiSendNum;                     //Number of Multi-Send
/* below field later needed modified */
/* initial value -1 :   resend message phone numbers */
    char                CallDigit[MAX_NUM_MULTI_SEND][MAX_DIAL_DIGIT];

    byte                szData[MAX_SMS_MSG_LEN+1];           //SMS Message
}nv_smssend_type;

/*-----------------받은 음성 메시지 NV 저장 format-------------------------*/
typedef PACKED struct {
    byte                unAddr;
    byte                unUsed;
    word                nID;
    char                Time[4];
    byte                unAttr;                         //0x01: Read/UnRead
    byte                unDialLen;
    byte                szDial[MAX_DIAL_DIGIT+1];
}nv_vmsrcv_type;

/*------------------------Mybell NV 저장 format----------------------------*/
typedef PACKED struct {
    byte                unAddr;
    byte                unUsed;
    char                Time[4];
    byte                unDataLen;
    byte                szData[MAX_MYBELL_DATA];
}nv_mybell_type;

/*------------------------상용구 NV 저장 format----------------------------*/
typedef PACKED struct {
    byte                unAddr;
    byte                unUsed;
    byte                szLikePhrase[MAX_LIKEPHRASE_LEN+1];
}nv_likephrase_type;

/*------------------------SMS Option NV 저장 format------------------------*/
typedef PACKED struct {
    byte                unRcvListOrder;
    byte                unRingType;
    byte                unSMSBellType;
}nv_smsoption_type;

/*-----------------------will be removed at other model--------------------*/
typedef PACKED struct {
  dword       confirmation_code;       /* reservation confirmation code */
  dword       deletion_list;           /* sms msgs to delete prior to write */
  byte        address;                 /* sms entry [0..23] */
  byte        status;                  /* status mask */
  byte        protect;
  byte        user_data_length;
  byte        call_back_length; 
  qword       param_mask;
  word        length;                  /* bytes in 'data', [8..388] */
  byte        data[NV_SMS_DATA_SIZ];   /* generic sms data, variable size  */
} nv_sms_type;

#else /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for real sms item                                                  */

typedef PACKED struct {
  dword       confirmation_code;       /* reservation confirmation code */
  dword       deletion_list;           /* sms msgs to delete prior to write */
  byte        address;                 /* sms entry [0..23] */
  byte        status;                  /* status mask */
  byte        protect;
  byte        user_data_length;
  byte        call_back_length; 
  qword       param_mask;
  word        length;                  /* bytes in 'data', [8..388] */
  byte        data[NV_SMS_DATA_SIZ];   /* generic sms data, variable size  */
} nv_sms_type;
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
/* <-- */

/* Type for DM (diagnostic monitor) sms item                               */

typedef PACKED struct {
  dword       confirmation_code;
  dword       deletion_list;
  byte        address;                 /* sms entry [0..23] */
  byte        status;                  /* status mask */
  byte        protect;
  byte        user_data_length;
  byte        call_back_length; 
  qword       param_mask;
  word        length;                  /* bytes in 'data', [8..325] */
  byte        data[NV_SMS_DM_DATA_SIZ]; /* generic sms data, variable size */
} nv_sms_dm_type;

/* Total size of header fields within nvi_sms_type (i.e., excluding "data")*/
#define  NV_SMS_HEADER_SIZE  14

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for generic minimum/maximum type                                   */

typedef PACKED struct {
  byte        min;                      
  byte        max;
} nv_minmax_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for Analog (FM) TX power levels                                    */

typedef PACKED struct {
  byte        level_0;                               /* power level 0 (max)*/
  byte        level_1;                                    /* power level 1 */
  byte        level_2;                                    /* power level 2 */
  byte        level_3;                                    /* power level 3 */
  byte        level_4;                                    /* power level 4 */
  byte        level_5;                                    /* power level 5 */
  byte        level_6;                                    /* power level 6 */
  byte        level_7;                                    /* power level 7 */
} nv_fm_tx_pwr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for Frequency/Temperature Offset Table                             */

#define NV_FR_TEMP_OFFSET_TABLE_SIZ 64

typedef PACKED struct {
  byte   item[NV_FR_TEMP_OFFSET_TABLE_SIZ];
} nv_fr_temp_offset_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for Frequency Adjustment Table                                     */

#define NV_FREQ_ADJ_TABLE_SIZ 32

typedef PACKED struct {
  byte item[ NV_FREQ_ADJ_TABLE_SIZ];
} nv_freq_adj_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold IMSI, along with the associated NAM id.                    */
/* The number is 51 bits, per J-STD-008 section 2.3.1.                     */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  qword     imsi;                   /* International Mobile Station ID */
} nv_imsi_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold IMSI length, along with the associated NAM id.             */
/* The number is 3 bits, per J-STD-008 section 2.3.1.                      */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  byte      num;                    /* Length of the IMSI for this NAM */
} nv_imsi_addr_num_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold TMSI zone length, along with the associated NAM id.               */
/* The number is 4 bits, per J-STD-008 section 2.3.15.                    */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  byte      length;                 /* The TMSI zone length */
} nv_tmsi_zone_length_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold TMSI zone, along with the associated NAM id.               */
/* The number is 64 bits, per J-STD-008 section 2.3.15.                    */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  qword     zone;                   /* The TMSI zone */
} nv_tmsi_zone_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold TMSI code, along with the associated NAM id.               */
/* The number is 32 bits, per J-STD-008 section 2.3.15.                    */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  dword     code;                   /* The TMSI code */
} nv_tmsi_code_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold TMSI expiration timer, along with the associated NAM id.   */
/* The number is 24 bits, per J-STD-008 section 2.3.15.                    */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  dword     time;                   /* The TMSI expiration timer */
} nv_tmsi_exp_time_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the subscriber's home block (3 bits), along with the       */
/* associated NAM id.                                                      */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  byte      block;                  /* Home block */
} nv_home_block_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the roaming list, along with the associated NAM id.        */
/* Roaming list is bit-packed.                                             */

#ifdef FEATURE_NV_4K_RL
  #define NV_ROAMING_LIST_MAX_16K_SIZE   4096   /* 4K PRL */
#else
  #define NV_ROAMING_LIST_MAX_16K_SIZE   3072   /* 3K PRL */
#endif
#define NV_ROAMING_LIST_1K_PRL_SIZE    1024   /* 1K PRL */

#ifdef FEATURE_NV_ONE_NAM_RL_LARGE
  #define  NV_ROAMING_LIST_MAX_8K_SIZE   625
#else
  #define  NV_ROAMING_LIST_MAX_8K_SIZE   200
#endif

#ifdef FEATURE_NV_1K_RL
  #define  NV_ROAMING_LIST_MAX_SIZE  NV_ROAMING_LIST_1K_PRL_SIZE
#else
  #ifdef FEATURE_NV_AUTOSIZE
#error code not present
  #else
    #define  NV_ROAMING_LIST_MAX_SIZE  NV_ROAMING_LIST_MAX_16K_SIZE 
  #endif
#endif

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  word      prl_version;    
  word      size;   
  boolean   valid;             
  byte      roaming_list[NV_ROAMING_LIST_MAX_SIZE]; 
} nv_roaming_list_type;

/* Same structure as roaming_list_type but adding some buffer bytes 
 * at the end of roaming_list for FASTPRL.
 * This structure is used in RAM
 */
#define NV_RAM_PRL_BUF_BYTES  6     /* buffer bytes to account for uneven
                                     * bit in PRL system table, ni_pr area,
                                     * ni2bit area, and word alignment in
                                     * geo_sid area.
                                     */                             

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  word      prl_version;    
  word      size;   
  boolean   valid;             
  byte      roaming_list[NV_ROAMING_LIST_MAX_SIZE + NV_RAM_PRL_BUF_BYTES];
} nv_ram_roaming_list_type;

/* Same structure as roaming_list_type but for use in union nv_item_type */
typedef PACKED struct {
  byte      nam;
  word      prl_version;    
  word      size;    
  boolean   valid;             
  byte      roaming_list[1]; 
} nv_roaming_list_union_type;

/* Only the roaming_list_type header */
typedef PACKED struct {
  byte      nam;
  word      prl_version;    
  word      size;    
  boolean   valid;             
} nv_roaming_list_header_type;

/* Total size of header fields within nv_roaming_list_type */
#define NV_ROAMING_LIST_HEADER_SIZE  (sizeof(nv_roaming_list_header_type))

/* An always-invalid prl version number */
#define NV_PRL_VERSION_INVALID   0xFFFF  

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef NV_FEATURE_PRL_ITEMS
/* Type to hold the downloadable roaming messages */

#define NV_ROAM_MSG_MAX_SIZE    12  /* Up to 12 characters of text allowed */
#define NV_ROAM_MSG_MAX_ENTRIES  6  /* Up to 6 entries in the table */

typedef PACKED struct {
  byte      index;                       /* Which table entry to reference */
  byte      text[NV_ROAM_MSG_MAX_SIZE];  /* The roaming message text */
} nv_roam_msg_type;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the MRU table (old format), along with the associated NAM. */
/* This type is now obsolete -- use MRU2 format below instead.             */

#define  NV_MRU_TABLE_SIZE 12       /* Table consists of 12 words */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  word      table[NV_MRU_TABLE_SIZE]; 
} nv_mru_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold the MRU table (new format), along with the associated NAM. */

#define  NV_MRU2_TABLE_SIZE 12       /* Table consists of 12 words */

/* Specifies the mode: AMPS or CDMA */
#define  NV_MODE_INACTIVE  0
#define  NV_MODE_ANALOG    1
#define  NV_MODE_CDMA      2

/* Specifies the cellular system type */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_SYSTEM_A,
  NV_SYSTEM_B,
  NV_SYSTEM_PAD = 0x7FFF             /* Pad to 16 bits on ARM */
} nv_system_type;
#endif

/* Specifies either a channel or cellular system */
typedef PACKED union {
  word            cdma_channel;     /* The CDMA channel to acquire */
  nv_system_type  cellular_system;  /* The cellular system: A or B */
} nv_cs_union_type;

/* An MRU2 Table entry */
typedef PACKED struct {
  byte              mode;           /* The mode: AMPS or CDMA */  
  byte              band_class;     /* The band class: cellular or PCS */
  nv_cs_union_type  cs;             /* The CDMA channel or cellular system */
} nv_mru2_table_entry_type;

/* The MRU2 Table */
typedef PACKED struct {
  byte                      nam;     /* NAM id 0-N */
  nv_mru2_table_entry_type  entry[NV_MRU2_TABLE_SIZE]; 
} nv_mru2_table_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* For OTASP "commit" operation -- type to hold the mobile directory       */
/* number (per IS-683 section 3.5.2.2).                                    */

typedef PACKED struct {
  byte      nam;                          /* NAM id 0-N       */
  byte      n_digits;                     /* Number of digits */
  byte      digitn[NV_DIR_NUMB_PCS_SIZ];  /* The digits       */
} nv_mob_dir_data_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* For OTASP "commit" operation -- type to hold the CDMA NAM information   */
/* for 800 or 1900 MHz.  Includes information from the Cellular and/or     */
/* PCS parameter blocks (IS-683 sections 4.5.2.1 and  4.5.2.3).            */

typedef PACKED struct {
  byte     imsi_addr_num;     /* Number of IMSI address digits             */
  word     mcc;               /* Mobile country code                       */
  byte     imsi_11_12;        /* 11th and 12th digits of IMSI              */
  dword    imsi_s1;           /* Corresponds to old min1                   */
  word     imsi_s2;           /* Corresponds to old min2                   */
  byte     accolc;            /* Access overload class                     */
  boolean  mob_term_home;     /* Termination indicator for the home system */
  boolean  mob_term_for_sid;  /* Termination indicator for SID roaming     */
  boolean  mob_term_for_nid;  /* Termination indicator for NID roaming     */
  struct {
    word   sid;               /* System identification                     */
    word   nid;               /* Network identification                    */
#ifdef NV_FEATURE_SSPR_ENHANCEMENTS
  } pair[NV_MAX_HOME_SID_NID];
#else
  } pair[NV_MAX_SID_NID];
#endif
} nv_cdma_data_type;  

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* For OTASP "commit" operation -- type to hold the AMPS NAM information   */
/* for 800 MHz cellular.  Includes information from the Cellular parameter */
/* block (IS-683 section 4.5.2.1).                                         */

typedef PACKED struct {
  word     firstchp;          /* First paging channel for the home system  */
  word     home_sid;          /* Analog home system identification         */
} nv_amps_data_type;  

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* For OTASP "commit" operation -- type to hold the IMSI-T parameters.     */

typedef PACKED struct {
  dword    s1;
  word     s2;
  byte     imsi_t_11_12;
  word     mcc;
  byte     addr_num;
} nv_imsi_t_data_type;  

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* For OTASP "commit" operation -- type to hold the consolidated data to   */
/* be committed, along with control flags.                                 */

typedef PACKED struct {
  byte nam;                      /* Which NAM the data is for              */
  boolean commit_mob_dir;        /* Commit mobile directory number if set  */
  boolean commit_ftc_cdma;       /* Commit CDMA NAM information if set     */
  boolean commit_ftc_amps;       /* Commit AMPS NAM information if set     */
  boolean commit_roam_list;      /* Commit roaming list if set             */
  boolean commit_spc;            /* Commit SPC if set                      */
  boolean commit_imsi_t;         /* Commit IMSI_T parms if set             */
  boolean commit_nam_lock;       /* Commit NAM_LOCK parms if set           */
  nv_mob_dir_data_type mob_dir;          /* The data to commit to nv       */
  nv_cdma_data_type ftc_cdma;            /* The data to commit to nv       */
  nv_amps_data_type ftc_amps;            /* The data to commit to nv       */
  nv_roaming_list_type *roam_list_ptr;   /* The data to commit to nv       */
  nv_sec_code_type spc;                  /* The data to commit to nv       */
  nv_imsi_t_data_type imsi_t;            /* The data to commit to nv       */
  nv_enabled_type nam_lock;              /* The data to commit to nv       */
} nv_otasp_commit_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify which type of keypress sound should be emitted.         */

#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_KEY_SOUND_CLICK,          /* Emit clicks for key presses */
  NV_KEY_SOUND_TONES,          /* Emit tones for key presses  */
  NV_KEY_SOUND_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
 } nv_key_sound_enum_type; 
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify which phone book sorting method is to be used.          */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_SORT_ALPHABETICAL,        /* Sort entries alphabetically */
  NV_SORT_BY_LOCATION,         /* Sort entries by location    */
  NV_DIALS_SORT_ENUM_PAD = 0x7FFF    /* Pad to 16 bits on ARM */
} nv_dials_sort_enum_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify which language to use for the user interface.           */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_LANGUAGE_ENGLISH,         /* Use English language    */
  NV_LANGUAGE_SPANISH,         /* Use Spanish language    */
  NV_LANGUAGE_FRENCH,          /* Use French language     */   
  NV_LANGUAGE_PORTUGUESE,      /* Use Portuguese language */
  NV_LANGUAGE_ENUM_PAD = 0x7FFF  /* Pad to 16 bits on ARM */
} nv_language_enum_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to specify the user interface menu format.                         */

#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_MENU_FULL,                /* Display full menus   */
  NV_MENU_SHORT,               /* Display short menus  */
  NV_MONU_FORMAT_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
} nv_menu_format_enum_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for ISS2 dialing plan restriction levels.                          */

#define NV_DIAL_RESTRICT_LVLS_SIZE  10         /* Ten entries in the table */

typedef PACKED struct {
  byte  restriction_level[NV_DIAL_RESTRICT_LVLS_SIZE];
} nv_dial_restrict_lvls_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Types for ISS2 dialing plan state table.                                */

#define  NV_DIAL_STATE_TABLE_NEXT_STATES 12    /* 12 bytes per table entry */
#define  NV_DIAL_STATE_TABLE_MAX_SIZE   125    /* Number entries in table */

/* Type of the elements within the table.                                  */
typedef PACKED struct {
  byte  address;                                          /* Storage index */
  char  next_state[NV_DIAL_STATE_TABLE_NEXT_STATES];
  byte  timeout;
  byte  restriction_level;
  byte  remaining_digits;
} nv_dial_state_table_elem_type;

/* Type for the number of entries in the table.                            */
typedef PACKED struct {
  word  length;          /* Number of "elem_type" entries currently stored */
} nv_dial_state_table_len_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type definitions for Unwired Planet                                     */

#define  NV_UP_LINK_INFO_TABLE_SIZE    2  /* For 2 link info table records */
#define  NV_IP_ADDRESS_TABLE_SIZE      2  /* For 2 IP address records      */
#define  NV_IP_ADDRESSES_PER_ENTRY     4  /* For 4 IP addresses per record */
#define  NV_UP_MAX_KEY_SIZE           16  /* Max size of key field         */
#define  NV_UP_MAX_PERM_STORE_SIZE    10  /* Max size of perm_store field  */
#define  NV_UP_ALERT_NAME_SIZE        11  /* Max size of alert_name field  */
#define  NV_UP_MAX_ALERT_RECORDS       9  /* Number of alert records       */
#define  NV_UP_MAX_CACHE_SIZE          1  /* Max size of the UP cache      */

typedef PACKED struct {
  byte   ip[NV_IP_ADDRESSES_PER_ENTRY];   /* IP address table              */
  word   port;                            /* Port address                  */
} nv_ip_address_type;

/* Type for NV_UP_LINK_INFO_I                                              */
typedef PACKED struct {
  byte   index;                           /* UP link info table index      */
  word   numAddr;                         /* Number of valid ip address    */
  nv_ip_address_type  addr[NV_IP_ADDRESS_TABLE_SIZE];
  byte   key_length;                      /* Length of key field           */
  byte   obsolete[NV_UP_MAX_KEY_SIZE];    /* Obsolete key (actual key is   */
} nv_up_link_info_type;                   /* stored in NV_UP_KEY_I)        */

/* Type for NV_UP_KEY_I                                                    */
typedef PACKED struct {
  byte   index;                           /* UP link info table index      */
  byte   key[NV_UP_MAX_KEY_SIZE];         /* Access key                    */
} nv_up_key_type;

/* Type for NV_UP_PARMS_I                                                  */
typedef PACKED struct {
  byte   curr_gateway;                    /* Last active IP address        */
  byte   alert_state;                     /* Last alert state              */
  byte   disconnect_timer;                /* Disconnect time-out value     */
  byte   perm_storage[NV_UP_MAX_PERM_STORE_SIZE];  /* "permanent storage"  */
} nv_up_parms_type;

/* Type for NV_UP_ALERTS_I                                                 */
typedef PACKED struct {
  byte   index;             /* To specify which of the 9 records to access */
  byte   alert_name[NV_UP_ALERT_NAME_SIZE];
  byte   alert_status;
  word   alert_id;
} nv_up_alert_record_type;

/* Type for NV_UP_CACHE_I                                                  */
typedef PACKED struct {
  byte   cache[NV_UP_MAX_CACHE_SIZE];     /* The UP cache                  */
} nv_up_cache_type;

/* Same structure as nv_up_cache_type but for use in union nv_item_type    */
typedef PACKED struct {
  byte   cache[1];
} nv_up_cache_union_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type definitions for DTACO                                              */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_DTACO_DISABLED,                      /* DTACO disabled                */
  NV_DTACO_ENABLED,                       /* DTACO enabled                 */
  NV_DTACO_ENABLED_ON_FLIP,               /* DTACO enabled on flip         */
  NV_DTACO_ENUM_PAD = 0x7FFF              /* Pad to 16 bits on ARM */
} nv_dtaco_enum_type;
#endif

typedef PACKED struct {
  nv_dtaco_enum_type  dtaco_control;
} nv_dtaco_control_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type definitions for BORSCHT items.                                     */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_EVRC_1ST,                            /* Priority: EVRC first          */
  NV_EVRC_13K_1ST,                        /* Priority: 13K first           */
  NV_EVRC_PRI_ENUM_PAD = 0x7FFF           /* Pad to 16 bits on ARM */
} nv_evrc_pri_enum_type;
#endif


#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_V52_DISABLED = 0,    /* V5.2 support disabled, functionality bypassed */
  NV_V52_ENABLED,         /* V5.2 support enabled, with no local buffering */
  NV_V52_LOCAL_BUFFERING, /* V5.2 support enabled, with local buffering    */
  NV_V52_FUTURE_SUPPORT,  /* V5.2 support reserved field                   */
  NV_V52_CONTROL_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
} nv_v52_control_enum_type;
#endif


#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_AFAX_ETE,                            /* Analog Fax end-to-end         */
  NV_AFAX_CLASS2,                         /* Analog Fax Class 2.0 Fax      */
  NV_AFAX_ENUM_PAD = 0x7FFF               /* Pad to 16 bits on ARM */
} nv_afax_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type definitions for Carrier Info items.                                */

#define  NV_CARRIER_INFO_ELEMENT_SIZE  100  /* 100 bytes per entry         */
#define  NV_CARRIER_INFO_MAX_ELEMENTS    3  /* Max of 3 entries allowed    */

typedef PACKED struct {
  byte   element;                           /* Element ID: 0, 1, or 2      */
  byte   data[NV_CARRIER_INFO_ELEMENT_SIZE];
} nv_carrier_info_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


#ifdef FEATURE_EVRC_SO_MGMT
/* Type for EVRC preferred voice service options.                          */

typedef PACKED struct {
  byte      nam;                    /* NAM id 0-N */
  boolean   evrc_capability_enabled;
  word      home_page_voice_so;
  word      home_orig_voice_so;
  word      roam_orig_voice_so;
} nv_pref_voice_so_type;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Definitions for Micro Browser data table item.                          */
              
#define NV_UBROWSER_ELEMENT_SIZE  64 
#define NV_UBROWSER_TABLE_SIZE    30 

typedef PACKED struct { 
  byte            index;                            /* Which array element */
  byte            data[NV_UBROWSER_ELEMENT_SIZE];   /* The data */ 
} nv_ubrowser_type; 

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Definitions for PN ID table.                                            */

#define NV_PN_ID_TABLE_SIZE       2048

typedef PACKED struct {
  byte            data[NV_PN_ID_TABLE_SIZE];
} nv_pn_id_type;

/* Same structure as nv_pn_id_type but for use in union nv_item_type       */
typedef PACKED struct {
  byte            data[1];
} nv_pn_id_union_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_USER_ZONE
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Definitions for Password Authentication Protocol (PAP) items.           */

#define NV_MAX_PAP_USER_ID_LENGTH  64    /* Max of 64 chars in User ID */
#define NV_MAX_PAP_PASSWORD_LENGTH 14    /* Max of 14 chars in Password */

#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
typedef PACKED struct {
  byte      user_id_len;                           /* Length of user_id */
  byte      user_id[NV_MAX_PAP_USER_ID_LENGTH];    /* The user_id */
} nv_pap_user_id_type;

typedef PACKED struct {
  byte      password_len;                          /* Length of password */
  byte      password[NV_MAX_PAP_PASSWORD_LENGTH];  /* The password */
} nv_pap_password_type;
#endif

#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
typedef PACKED struct {
  byte		tempsense[3];
} nv_ref_tempsense_type;

typedef PACKED struct {
  word		ref_txslp;
  word		power[3];
} nv_freq_pwr_type;

typedef PACKED struct {
  byte		  ref_txslp;
  byte        min;                      
  byte        max;
} nv_pminmax_type;
#endif	// #ifdef FEATURE_SKTT_CAL

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Size of all tables based on frequency   */
#define NV_FREQ_TABLE_SIZ             16
/* Size of all tables based on temperature */
#define NV_TEMP_TABLE_SIZ              8

#define NV_NUM_PA_RANGES 4
  
#if (defined FEATURE_PA_RANGE_DIG_COMP || defined FEATURE_PA_RANGE_TEMP_FREQ_COMP)
typedef PACKED struct {
  int2      offset;
  int1      temp_adj[NV_TEMP_TABLE_SIZ];
  int1      freq_adj[NV_FREQ_TABLE_SIZ];
} nv_pa_offset_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* CDMA Tx linearizer master offset size */
#define NV_CDMA_TX_LIN_MASTER_SIZ     36

#ifdef FEATURE_4PAGE_TX_LINEARIZER
#define NV_NUM_TX_LINEARIZERS 4

typedef PACKED struct
{
  int2      offset;
  byte      slope[NV_CDMA_TX_LIN_MASTER_SIZ];
} nv_tx_linearizer_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_BROADCAST_SMS
/* Definitions for Broadcast SMS items.                                    */
#define NV_SMS_MAX_SERVICES  32   
/* Limit the table size with EEPROM */  
  #ifdef FEATURE_NV_ITEM_MGR
     #define NV_SMS_MAX_LANGUAGES  4 
  #else
     #define NV_SMS_MAX_LANGUAGES  1      
  #endif
#define NV_SMS_BC_TABLE_SIZE  (NV_SMS_MAX_SERVICES*NV_SMS_MAX_LANGUAGES)
#define NV_SMS_BC_SRV_LABEL_SIZE 10

typedef PACKED struct
{
  byte     index;
  boolean  active_service; /* for delete operation */
  word     service;
  byte     language;
  boolean  selected;
  byte     priority;
  char     label[NV_SMS_BC_SRV_LABEL_SIZE]; /* a UI text label for each service/language pair */
} nv_sms_bc_service_table_type;

#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_RUIM_CHV_FROM_NV

#define NV_RUIM_CHV_SIZE 8                /* Number of bytes in an R-UIM CHV */
typedef PACKED struct
{
  byte     digits[NV_RUIM_CHV_SIZE]; 
} nv_ruim_chv_type;

#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_GPSONE
#error code not present
#endif

/*-------------------------------------------------------------------------*/
/* Type of IS2000 CAI radio configuration RC preference                    */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS                          
#error code not present
#else
typedef enum {
  NV_CAI_RC_1 = 1,   
  NV_CAI_RC_2,       
  NV_CAI_RC_3,             
  NV_CAI_RC_4,      
  NV_CAI_RC_5,
  NV_RC_PREF_ENUM_PAD = 0x7FFF               /* Pad to 16 bits on ARM */
} nv_rc_pref_type;
#endif

/*-------------------------------------------------------------------------*/
/* Type of DATA SERVICES default SIO baud rate                             */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_SIO_BITRATE_AUTO = 0,   
  NV_SIO_BITRATE_45,       
  NV_SIO_BITRATE_50,             
  NV_SIO_BITRATE_75,      
  NV_SIO_BITRATE_110,
  NV_SIO_BITRATE_150, 
  NV_SIO_BITRATE_300, 
  NV_SIO_BITRATE_600, 
  NV_SIO_BITRATE_1200, 
  NV_SIO_BITRATE_2400, 
  NV_SIO_BITRATE_4800, 
  NV_SIO_BITRATE_9600, 
  NV_SIO_BITRATE_19200, 
  NV_SIO_BITRATE_38400, 
  NV_SIO_BITRATE_57600, 
  NV_SIO_BITRATE_115200, 
  NV_SIO_BITRATE_230400, 
  NV_SIO_BITRATE_BEST, 
  NV_SIO_BITRATE_MAX,
  NV_SIO_BAUD_ENUM_PAD = 0x7FFF               /* Pad to 16 bits on ARM */
} nv_sio_baud_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type of NV_TX_COMP0_I, NV_TX_COMP1_I, NV_TX_COMP2_I, NV_TX_COMP3_I      */

typedef PACKED struct {
  int1      nv_cdma_tx_lin_vs_temp[NV_TEMP_TABLE_SIZ];
  int1      nv_cdma_tx_slp_vs_temp[NV_TEMP_TABLE_SIZ];
  int1      nv_cdma_tx_comp_vs_freq[NV_FREQ_TABLE_SIZ];
} nv_tx_comp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type of NV_RUIM_SMS_STATUS_I                                            */

typedef PACKED struct {
  word msg_id;
  word wap_msg_id;
  boolean mem_avail;
} nv_ruim_sms_status_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type of NV_DIAG_RESTART_CONFIG_I                                        */

#define  NV_DIAG_RESTART_CONFIG_LOGMASK_SIZE  16
typedef PACKED struct {
  uint8 event_config;
  uint8 msg_level;
  uint8 logmask[NV_DIAG_RESTART_CONFIG_LOGMASK_SIZE];
  uint8 reserved[4];
} nv_diag_restart_config_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Enumeration of system selection band and sub-band preference            */
#ifdef FEATURE_SD20
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type of NV_BD_ADDR_I                                                    */

#define  NV_BD_ADDR_SIZE  6
typedef PACKED struct {
  uint8 bd_addr[NV_BD_ADDR_SIZE];
} nv_bd_addr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type of DATA SERVICES default SIO baud rate                             */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_SIO_BAUDRATE_AUTO = 0,   
  NV_SIO_BAUDRATE_45,       
  NV_SIO_BAUDRATE_50,             
  NV_SIO_BAUDRATE_75,      
  NV_SIO_BAUDRATE_110,
  NV_SIO_BAUDRATE_150, 
  NV_SIO_BAUDRATE_300, 
  NV_SIO_BAUDRATE_600, 
  NV_SIO_BAUDRATE_1200, 
  NV_SIO_BAUDRATE_2400, 
  NV_SIO_BAUDRATE_4800, 
  NV_SIO_BAUDRATE_9600,
  NV_SIO_BAUDRATE_14400,
  NV_SIO_BAUDRATE_19200, 
  NV_SIO_BAUDRATE_38400, 
  NV_SIO_BAUDRATE_57600,
  NV_SIO_BAUDRATE_76800,
  NV_SIO_BAUDRATE_115200, 
  NV_SIO_BAUDRATE_230400, 
  NV_SIO_BAUDRATE_BEST=0x7FFE, 
  NV_SIO_BAUDRATE_MAX=0x7FFF
} nv_sio_baudrate_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type of NV_JCDMA_DS_F92_I                                               */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NV_JCDMA_DS_F92_ASYNC_FAX_96 = 0,       /* Async/Fax 9600bps */
  NV_JCDMA_DS_F92_ASYNC_FAX_144,          /* Async/Fax 14400bps */
  NV_JCDMA_DS_F92_PACKET_96,              /* Packet 9600bps */
  NV_JCDMA_DS_F92_PACKET_144,             /* Packet 14400bps */
  NV_JCDMA_DS_F92_PACKET_HIGH,            /* Packet 64kbps */
  NV_JCDMA_DS_F92_ENUM_PAD = 0x7fff       /* Pad to 16 bits on ARM */
} nv_jcdma_ds_f92_enum_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Values for NV_MAX_TX_ADJ_I                                              */

#define NV_MUTE_NORM 0
#define NV_MUTE_MUTE 1
#define NV_MUTE_SPKR 2

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Stack-checker diagnostic item sizes                                     */

#define NV_SI_TEST_DATA_SIZE  100  /* 100 bytes for each of two items */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Array size for NV_FACTORY_INFO_I                                        */

#define NV_FACTORY_INFO_SIZ  100

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Definitions for Port mode                                               */

#define NV_PORT_SYNC_MODE     0      /* Sync (HDLC) mode for port */
#define NV_PORT_BAUD_96_MODE  1      /* 9600 Baud mode */
#define NV_PORT_BAUD_192_MODE 2      /* 19200 Baud mode */
#define NV_PORT_BAUD_384_MODE 3      /* 38400 Baud mode */

/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/

/* Portable RF Turnaround Compensation Table Sizes                         */

#define NV_RIPPLE_COMP_TABLE_SIZ      32
#define NV_RX_OFFSET_TABLE_SIZ        32
#define NV_RX_POWER_TABLE_SIZ        128
#define NV_RX_ERROR_TABLE_SIZ        128
#define NV_TX_SLOPE_1_TABLE_SIZ      128
#define NV_TX_SLOPE_2_TABLE_SIZ      128
#define NV_TX_NON_LIN_ERR_TABLE_SIZ  128
#define NV_HDET_TABLE_SIZ             64

#define NV_PWR_TABLE_SIZ   16  /* size of all tables based on power level */
#define NV_VOLT_TABLE_SIZ   8  /* size of all tables based on battery voltage */

/* CDMA Rx linearizer offset size */
#define NV_CDMA_RX_LIN_SIZ            16
/* PCS Rx linearizer offset size  */
#define NV_PCS_RX_LIN_SIZ             16

/* CDMA Tx linearizer master offset size */
#define NV_CDMA_TX_LIN_MASTER_SIZ     36
/* PCS Tx linearizer master offset size  */
#define NV_PCS_TX_LIN_MASTER_SIZ      36

/* CDMA expected HDET reading vs AGC PDM size */
#define NV_CDMA_EXP_HDET_VS_AGC_SIZ   16
/* PCS expected HDET reading vs AGC PDM size */
#define NV_PCS_EXP_HDET_VS_AGC_SIZ    16
#define NV_CDMA_ERR_SLP_VS_HDET_SIZ  8 /* CDMA HDET error slope vs HDET reading size */
#define NV_ADJ_VS_FREQ_SIZ           16
#define NV_ADJ_VS_TEMP_SIZ            8

#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
#define NV_CDMA_LNA_LIN_SIZ          8 /* CDMA LNA linearizer offset size */
#endif	// #ifdef FEATURE_SKTT_CAL



/*=========================================================================*/

/* NVM items union type.  The union is ordered according to the            */
/* NV_xxx_I enum items ordering.  Note that when the caller defines        */
/* or allocates a buffer of this union type, the largest member of         */
/* this union defines the size, and caution should be then exercised.      */
/* The largest member is nv_sms_type                                       */

/*-------------------------------------------------------------------------*/

typedef PACKED union {

/*-------------------------------------------------------------------------*/

/* Physical NAM parameters.                                                */

/*-------------------------------------------------------------------------*/

/* Type for NV_ESN_I item.                                                 */
/* -----------------------                                                 */

  nv_esn_type        esn;

/* Type for NV_ESN_CHKSUM_I item.                                          */
/* ------------------------------                                          */

  nv_esn_chksum_type esn_chksum;

/* Type for NV_VERNO_MAJ_I item.                                           */
/* -----------------------------                                           */
/* This is the NV major version number.  Changing the major (or minor)     */
/* version number forces an NV rebuild.                                    */

  byte         verno_maj;

/* Type for NV_VERNO_MIN_I item.                                           */
/* -----------------------------                                           */
/* This is the NV minor version number.  Changing the minor (or major)     */
/* version number forces an NV rebuild.                                    */

  byte         verno_min;

/* Type for NV_SCM_I item.                                                 */
/* -----------------------                                                 */
/* The SCM is 8 bits, per CAI section 6.3.3.                               */

  byte         scm;

/* Type for NV_SLOT_CYCLE_INDEX_I item.                                    */
/* ------------------------------------                                    */
/* The slot cycle index is 3 bits, per CAI section 1.1.2.2.                */

  byte         slot_cycle_index;

/* Type for NV_MOB_CAI_REV_I item.                                         */
/* -------------------------------                                         */
/* The Mobile CAI revision is 8 bits, per CAI section 1.1.2.2. and as      */
/* specified for each CAI revision in the title section.                   */

  byte         mob_cai_rev;

/* Type for NV_MOB_FIRM_REV_I item.                                        */
/* --------------------------------                                        */
/* Software version number.  16 bits per CAI section 1.1.2.2.              */

  word         mob_firm_rev;

/* Type for NV_MOB_MODEL_I item.                                           */
/* -----------------------------                                           */
/* Mobile model.  8 bits per CAI section 1.1.2.2.                          */

  byte         mob_model;

/* Type for NV_CONFIG_CHKSUM_I item.                                       */
/* ---------------------------------                                       */

  nv_config_chksum_type       config_chksum;

/*-------------------------------------------------------------------------*/

/* NAM items.                                                              */

/* Type for NV_PREF_MODE_I item.                                           */
/* -----------------------------                                           */

  nv_pref_mode_type  pref_mode;

/* Type for NV_CDMA_PREF_SERV_I item.                                      */
/* -----------------------------                                           */

  nv_pref_serv_type   cdma_pref_serv;

/* Type for NV_ANALOG_PREF_SERV_I item.                                    */
/* ------------------------------------                                    */

  nv_pref_serv_type   analog_pref_serv;  

/* Type for NV_CDMA_SID_LOCK_I item.                                       */
/* ---------------------------------                                       */

  nv_sid_lock_type      cdma_sid_lock;

/* Type for NV_CDMA_SID_ACQ_I item.                                        */
/* --------------------------------                                        */

  nv_sid_acq_type       cdma_sid_acq;

/* Type for NV_ANALOG_SID_LOCK_I item.                                     */
/* -----------------------------------                                     */

  nv_sid_lock_type      analog_sid_lock;

/* Type for NV_ANALOG_SID_ACQ_I item.                                      */
/* ----------------------------------                                      */

  nv_sid_acq_type       analog_sid_acq;

/* Type for NV_ANALOG_FIRSTCHP_I item.                                     */
/* -----------------------------------                                     */

  nv_firstchp_type      analog_firstchp;

/* Type for NV_ANALOG_HOME_SID_I item.                                     */
/* -----------------------------------                                     */

  nv_sid_type           analog_home_sid;

/* Type for NV_ANALOG_REG_I item.                                          */
/* ------------------------------                                          */

  nv_analog_reg_type       analog_reg;

/* Type for NV_PCDMACH_I item.                                             */
/* ---------------------------                                             */

  nv_cdmach_type           pcdmach;

/* Type for NV_SCDMACH_I item.                                             */
/* ---------------------------                                             */

  nv_cdmach_type           scdmach;

/* Type for NV_NAM_CHKSUM_I item.                                          */
/* ------------------------------                                          */

  nv_nam_chksum_type       nam_chksum;

/* Type for NV_A_KEY_I item.                                               */
/* -------------------------                                               */

  nv_a_key_type   a_key;

/* Type for NV_A_KEY_CHKSUM_I item.                                        */
/* --------------------------------                                        */

  nv_a_key_chksum_type   a_key_chksum;

/* Type for NV_SSD_A_I item.                                               */
/* -------------------------                                               */

  nv_ssd_type     ssd_a;

/* Type for NV_SSD_A_CHKSUM_I item.                                        */
/* --------------------------------                                        */

  nv_ssd_chksum_type     ssd_a_chksum;

/* Type for NV_SSD_B_I item.                                               */
/* -------------------------                                               */

  nv_ssd_type     ssd_b;

/* Type for NV_SSD_B_CHKSUM_I item.                                        */
/* --------------------------------                                        */

  nv_ssd_chksum_type     ssd_b_chksum;

/* Type for NV_COUNT_I item.                                               */
/* -------------------------                                               */

  nv_count_type   count;

/*-------------------------------------------------------------------------*/

/* MIN items.                                                              */

/*-------------------------------------------------------------------------*/

/* Type for NV_MIN1_I item.                                                */
/* ------------------------                                                */

  nv_min1_type       min1;

/* Type for NV_MIN2_I item.                                                */
/* ------------------------                                                */

  nv_min2_type       min2;

/* Type for NV_IMSI_T_S1_I item.                                           */
/* -----------------------------                                           */

  nv_min1_type       imsi_t_s1;

/* Type for NV_IMSI_T_S2_I item.                                           */
/* -----------------------------                                           */

  nv_min2_type       imsi_t_s2;


/* Type for NV_MOB_TERM_HOME_I item.                                       */
/* ---------------------------------                                       */

  nv_mob_term_type    mob_term_home;

/* Type for NV_MOB_TERM_FOR_SID_I item.                                    */
/* ------------------------------------                                    */

  nv_mob_term_type       mob_term_for_sid;

/* Type for NV_MOB_TERM_FOR_NID_I item.                                    */
/* ------------------------------------                                    */

  nv_mob_term_type       mob_term_for_nid;

/* Type for NV_ACCOLC_I item.                                              */
/* --------------------------                                              */

  nv_accolc_type     accolc;

/* Type for NV_SID_NID_I item.                                             */
/* ---------------------------                                             */

  nv_sid_nid_type sid_nid;

/* Type for NV_MIN_CHKSUM_I item.                                          */
/* ------------------------------                                          */

  nv_min_chksum_type    min_chksum;

/*-------------------------------------------------------------------------*/

/* Operational NAM settings.                                               */

/*-------------------------------------------------------------------------*/

/* Type for NV_CURR_NAM_I item.                                            */
/* ----------------------------                                            */
/* This type holds the id of the current NAM (0-3) and is then used to     */
/* identify or index into any parameter which is NAM specific.             */

  byte          curr_nam;

/* Type for NV_ORIG_MIN_I item.                                            */
/* -----------------------------                                           */
/* This type holds the id of the MIN (0-1) within the current NAM (0-3)    */
/* which specifies the MIN used for call origination.  This id may be      */
/* changed by the user and is retained during powerdown.                   */

  nv_orig_min_type  orig_min;

/* Type for NV_AUTO_NAM_I item.                                            */
/* ----------------------------                                            */
/* If auto nam is set then the phone will automatically switch to the      */
/* the new NAM if it is defined as one of the four NAMs and if the phone   */
/* has matched the SID of this new NAM as it roamed into a new SID.        */

  boolean      auto_nam;

/* Type for NV_NAME_NAM_I item.                                            */
/* -----------------------------                                           */
/* This type holds the name of each NAM.                                   */

  nv_name_nam_type  name_nam;

/*-------------------------------------------------------------------------*/

/* Semi-permanent analog registration parameters.                          */

/*-------------------------------------------------------------------------*/

/* Type for NV_NXTREG_I item.                                              */
/* ----------------------------                                            */
/* Holds the registration count per CAI section 2.3.4.1.                   */

  dword        nxtreg;

/* Type for NV_LSTSID_I item.                                              */
/* --------------------------                                              */
/* Holds the last SID registered to, per CAI section 2.3.4.1.              */

  word         lstsid;

/* Type for NV_LOCAID_I item.                                              */
/* --------------------------                                              */
/* Holds the location area ID, per CAI section 2.3.4.2.                    */

  word         locaid;

/* Type for NV_PUREG_I item.                                               */
/* -------------------------                                               */
/* Holds the powerup registration identifier, per CAI section 2.3.4.2.     */

  boolean      pureg;

/*-------------------------------------------------------------------------*/

/* Semi-permanent CDMA registration parameters.                            */

/*-------------------------------------------------------------------------*/

/* Type for NV_ZONE_LIST_I item.                                           */
/* -----------------------------                                           */

  nv_zone_list_type        zone_list;

/* Type for NV_SID_NID_LIST_I item.                                        */
/* --------------------------------                                        */
/* Holds the SID+NID list, per CAI section 6.3.4.                          */

  nv_sid_nid_list_type     sid_nid_list;

/* Type for NV_DIST_REG_I item.                                            */
/* ----------------------------                                            */

  nv_dist_reg_type         dist_reg;

/* Type for NV_LAST_CDMACH_I item.                                         */
/* -------------------------------                                         */

  nv_cdmach_type           last_cdmach;

/*-------------------------------------------------------------------------*/

/* Timers.                                                                 */
            
/*-------------------------------------------------------------------------*/

/* Type for NV_CALL_TIMER_G_I item.                                          */
/* ------------------------------                                          */

  nv_run_time_type   call_timer_g;

/* Type for NV_AIR_TIMER_I item.                                           */
/* ------------------------------                                          */

  nv_call_time_type   air_timer;

/* Type for NV_ROAM_TIMER_I item.                                          */
/* ------------------------------                                          */

  nv_call_time_type   roam_timer;

/* Type for NV_LIFE_TIMER_G_I item.                                          */
/* ------------------------------                                          */

  nv_run_time_type   life_timer_g;

/* Type for NV_RUN_TIMER_I item.                                           */
/* ------------------------------                                          */

  nv_run_time_type    run_timer;

/*-------------------------------------------------------------------------*/

/* Speed dial numbers.                                                     */

/*-------------------------------------------------------------------------*/

/* Type for NV_DIAL_I item.                                                */
/* ------------------------                                                */

  nv_dial_type       dial;

/* Type for NV_STACK_I item.                                               */
/* -------------------------                                               */

  nv_stdial_type       stack;

/* Type for NV_STACK_IDX_I item.                                           */
/* -----------------------------                                           */

  nv_stack_idx_type  stack_idx;


/* Type for NV_REDIAL_I item.                                              */
/* -------------------------                                               */

  nv_dial_type       redial;

/*-------------------------------------------------------------------------*/

/* Type for NV_PAGE_SET_I item.                                            */
/* ----------------------------                                            */

  nv_page_set_type   page_set;

/*-------------------------------------------------------------------------*/

/* Type for NV_PAGE_MSG_I item.                                            */
/* ----------------------------                                            */

  nv_page_msg_type   page_msg;

/*-------------------------------------------------------------------------*/

/* Volumes.                                                                */

/*-------------------------------------------------------------------------*/

/* Type for NV_EAR_LEVEL_I item.                                           */
/* -----------------------------                                           */
/* Volume is from 1 (min) to 255 (max) and 0 means OFF (or SILENCE).       */

  byte         ear_level;

/* Type for NV_SPEAKER_LEVEL_I item.                                       */
/* ---------------------------------                                       */
/* Volume is from 1 (min) to 255 (max) and 0 means OFF (or SILENCE).       */

  byte         speaker_level;

/* Type for NV_RINGER_LEVEL_I item.                                        */
/* --------------------------------                                        */
/* Volume is from 1 (min) to 255 (max) and 0 means OFF (or SILENCE).       */

  byte         ringer_level;

/* Type for NV_BEEP_LEVEL_I item.                                          */
/* -----------------------------                                           */
/* Volume is from 1 (min) to 255 (max) and 0 means OFF (or SILENCE).       */

  byte         beep_level;

/*-------------------------------------------------------------------------*/

/* Tones.                                                                  */

/*-------------------------------------------------------------------------*/

/* Type for NV_CALL_BEEP_I item.                                           */
/* -----------------------------                                           */
/* When set the user is beeped during a call once a minute on the          */
/* 50 seconds mark.                                                        */

  boolean      call_beep;

/* Type for NV_CONT_KEY_DTMF_I item.                                       */
/* ---------------------------------                                       */
/* When set this means that manual DTMF tones should be sent as continuous */
/* instead of as burst tones.                                              */

  boolean      cont_key_dtmf;

/* Type for NV_CONT_STR_DTMF_I item.                                       */
/* ---------------------------------                                       */
/* When set this means that string DTMF tones should be sent as continuous */
/* instead of as burst tones.                                              */

  boolean      cont_str_dtmf;

/* Type for NV_SVC_AREA_ALERT_I item.                                      */
/* ----------------------------------                                      */
/* When set this means that the phone beeps whenever it leaves a service   */
/* area and whenever it enters a service area.                             */

  boolean      svc_area_alert;

/* Type for NV_CALL_FADE_ALERT_I item.                                     */
/* -----------------------------------                                     */
/* When set this means that the phone beeps whenever a call fades.         */

  boolean      call_fade_alert;

/*-------------------------------------------------------------------------*/

/* Various phone settings.                                                 */

/*-------------------------------------------------------------------------*/

/* Type for NV_BANNER_I item.                                              */
/* --------------------------                                              */

  nv_banner_type  banner;

/* Type for NV_LCD_I item.                                                 */
/* --------------------------                                              */
/* Specifies handset brightness, with 1 lowest, 255 highest, and 0 means   */
/* blank (lights out).                                                     */

  byte         lcd;

/* Type for NV_AUTO_POWER_I item.                                          */
/* ------------------------------                                          */
/* This values specifies, in units of ten minutes, how long the phone      */
/* should stay in power on when ignition is turned off:                    */
/*                                                                         */
/* 0        - power off when ignition is turned off                        */
/* 1-254    - stay in power on, range is from 10 minutes to 42 hours       */
/* 255      - do not ever power down (good for testing)                    */

  byte         auto_power;

/* Type for NV_AUTO_ANSWER_I item.                                         */
/* -------------------------------                                         */

  nv_auto_answer_type   auto_answer;

/* Type for NV_AUTO_REDIAL_I item.                                         */
/* -------------------------------                                         */

  nv_auto_redial_type   auto_redial;

/* Type for NV_AUTO_HYPHEN_I item.                                         */
/* -------------------------------                                         */
/* When set the phone inserts hyphens automatically after 4, 7, 10, and 11 */
/* digits (i.e. 1-619-587-1121).  When cleared no hyphens are entered.     */

  boolean         auto_hyphen;

/* Type for NV_BACK_LIGHT_I item.                                          */
/* ------------------------------                                          */
/* This values specifies how many seconds should pass before the           */
/* backlighting is turned off, starting from the last moment when any      */
/* key was touched:                                                        */
/*                                                                         */
/* 0        - backlight never on                                           */
/* 1-10     - number of seconds before turning off                         */
/* 11-254   - illegal value                                                */
/* 255      - backlight never off                                          */

  byte      back_light;

/* Type for NV_AUTO_MUTE_I item.                                           */
/* -----------------------------                                           */
/* When set this causes the phone to mute the car stereo when a call is    */
/* started and unmute the car stereo when the call is ended.               */

  boolean      auto_mute;

/*-------------------------------------------------------------------------*/

/* Locks and restrictions.                                                 */

/*-------------------------------------------------------------------------*/

/* Type for NV_MAINTRSN_I item.                                            */
/* ---------------------------                                             */

  nv_maintrsn_type     maintrsn;

/* Type for NV_LCKRSN_P_I item.                                            */
/* -----------------------------                                           */

  nv_lckrsn_type     lckrsn_p;

/* Type for NV_LOCK_I item.                                                */
/* ------------------------                                                */
/* Set when the phone is locked (from the keypad), cleared when unlocked   */
/* (from the keypad).                                                      */

  byte       lock;

/* Type for NV_LOCK_CODE_I item.                                           */
/* -----------------------------                                           */

  nv_lock_code_type lock_code;

/* Type for NV_AUTO_LOCK_I item.                                           */
/* ---------------------------------                                       */
/* When set this means that everytime the phone is powered down it is      */
/* electronically locked and must be unlocked via the lock code.           */

  boolean      auto_lock;

/* Type for NV_CALL_RSTRC_I item.                                          */
/* ------------------------------                                          */
/* Word holding call restrictions bit map.  If the word is cleared         */
/* (all zeros) then no call restrictions are in place.                     */

  word         call_rstrc;

/* Type for NV_SEC_CODE_I item.                                            */
/* ----------------------------                                            */

  nv_sec_code_type   sec_code;

/* Type for NV_HORN_ALERT_I item.                                          */
/* ------------------------------                                          */
/* When set the horn sounds if the phone rings while ignition is off.      */

  boolean      horn_alert;

/*-------------------------------------------------------------------------*/

/* Error log.                                                              */

/*-------------------------------------------------------------------------*/

/* Type for NV_ERR_LOG_I item.                                             */
/* ---------------------------                                             */

  nv_err_log_type    err_log;

/*-------------------------------------------------------------------------*/


/* Miscellaneous items.                                                    */

/* Type for NV_UNIT_ID_I item.                                             */
/* ---------------------------                                             */

  dword             unit_id;

/* Type for NV_FREQ_ADJ_I item (table).                                    */
/* ------------------------------------                                    */

  nv_freq_adj_type  freq_adj;


/*-------------------------------------------------------------------------*/

/* Type for peek operation.                                                */
/* ------------------------                                                */

  nv_peek_type     peek;


/*-------------------------------------------------------------------------*/

/* Type for poke operation.                                                */
/* ------------------------                                                */

  nv_poke_type     poke;


/*-------------------------------------------------------------------------*/

/* Type for NV_VBATT_I item.                                               */
/* -----------------------------                                           */

   nv_minmax_type     vbatt;


/*-------------------------------------------------------------------------*/

/* Type for NV_FM_TX_PWR_I item.                                           */
/* -------------------------------                                         */

  nv_fm_tx_pwr_type   fm_tx_pwr;


/*-------------------------------------------------------------------------*/

/* Type for NV_FR_TEMP_OFFSET_I item.                                      */
/* -------------------------------                                         */

  nv_fr_temp_offset_type  fr_temp_offset;  


/*-------------------------------------------------------------------------*/

/* Type for   NV_FM_RSSI_I  item.                                          */
/* ---------------------------------                                       */

  nv_minmax_type    fm_rssi;


/*-------------------------------------------------------------------------*/

/* Type for   NV_PA_CURENT_CTL_I item.                                     */
/* ---------------------------------                                       */

  byte    pa_cur_ctl[ NV_HDET_TABLE_SIZ];


/*-------------------------------------------------------------------------*/

/* Type for   NV_SPARE_1_I                                                 */
/* ---------------------------------                                       */

  word    spare_1;

/*-------------------------------------------------------------------------*/

/* Type for   NV_SPARE_2_I                                                 */
/* ---------------------------------                                       */

  word    spare_2;

/*-------------------------------------------------------------------------*/

/* Type for NV_DATA_IO_MODE_I item.                                        */
/* -------------------------------                                         */

  byte    data_io_mode;


/*-------------------------------------------------------------------------*/

/* Type for NV_DATA_MODE_ENABLED_I item.                                   */
/* -------------------------------                                         */

  byte    dm_io_mode;


/*-------------------------------------------------------------------------*/

/* Type for NV_DATA_MODE_ENABLED_I item.                                   */
/* -------------------------------                                         */

  boolean    data_srvc_enabled;

/*-------------------------------------------------------------------------*/

/* Type for NV_IDLE_DATA_TIMEOUT_I item.                                   */
/* -------------------------------                                         */

  word      idle_data_timeout;

/*-------------------------------------------------------------------------*/

/* Type for NV_MAX_TX_ADJ_I item.                                          */
/* -------------------------------                                         */

  byte      max_tx_adj;

/*-------------------------------------------------------------------------*/

/* Type for NV_INI_MUTE_I item.                                            */
/* -------------------------------                                         */

  byte      init_mute;

/*-------------------------------------------------------------------------*/

/* Type for NV_FACTORY_INFO_I item.                                        */
/* -------------------------------                                         */

  byte      fact_info[ NV_FACTORY_INFO_SIZ];

/*-------------------------------------------------------------------------*/

/* Type for   NV_SONY_ATTEN_3_I                                            */
/* ---------------------------------                                       */

  byte    sony_atten_3;

/*-------------------------------------------------------------------------*/

/* Type for   NV_SONY_ATTEN_4_I                                            */
/* ---------------------------------                                       */

  byte    sony_atten_4;

/*-------------------------------------------------------------------------*/

/* Type for   NV_SONY_ATTEN_5_I                                            */
/* ---------------------------------                                       */

  byte    sony_atten_5;

/* Type for  NV_DM_ADDR_I                                                  */
/* ---------------------------------                                       */

  byte    dm_addr;
/* Type for  NV_CDMA_PN_MASK_I                                             */
/* ---------------------------------                                       */

  byte    cdma_pn_mask;

/* Type for  NV_SEND_TIMEOUT_I                                             */
/* ---------------------------------                                       */

  byte    send_timeout;
/*-------------------------------------------------------------------------*/

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  FM mode TX_AGC_ADJ initial setting vs power, frequency, and temperature
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  byte fm_agc_set_vs_pwr[NV_PWR_TABLE_SIZ];
  int1 fm_agc_set_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 fm_agc_set_vs_temp[NV_TEMP_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  FM mode TX_AGC_ADJ adjustments vs frequency, temperature, and power
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  byte fm_exp_hdet_vs_pwr[NV_PWR_TABLE_SIZ];
  byte fm_err_slp_vs_pwr[NV_PWR_TABLE_SIZ];

  byte fm_freq_sense_gain;
  nv_minmax_type     fm_lna_ctl;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  PCS/CDMA mode Rx AGC linearization table (to be loaded into MSM2P)
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  byte cdma_rx_lin_off_0;
  byte pcs_rx_lin_off_0;
  byte cdma_rx_lin_slp[NV_CDMA_RX_LIN_SIZ];
  byte pcs_rx_lin_slp[NV_PCS_RX_LIN_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  PCS/CDMA mode RX/TX_GAIN_COMP values vs frequency and battery voltage
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  int1 cdma_rx_comp_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 pcs_rx_comp_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 cdma_tx_comp_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 pcs_tx_comp_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 cdma_tx_comp_vs_volt[NV_VOLT_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  PCS/CDMA mode Tx AGC linearization (MSM2P data is derived from these tables)
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  word cdma_tx_lin_master_off_0;
  word pcs_tx_lin_master_off_0;
  byte cdma_tx_lin_master_slp[NV_CDMA_TX_LIN_MASTER_SIZ];
  byte pcs_tx_lin_master_slp[NV_PCS_TX_LIN_MASTER_SIZ];
  byte cdma_tx_lin_vs_temp[NV_TEMP_TABLE_SIZ];
  byte pcs_tx_lin_vs_temp[NV_TEMP_TABLE_SIZ];
  byte cdma_tx_lin_vs_volt[NV_VOLT_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  PCS/CDMA mode Tx power limit data vs temperature, battery voltage, 
  frequency, TX_AGC_ADJ PDM setting, and HDET reading
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  byte cdma_tx_lim_vs_temp[NV_TEMP_TABLE_SIZ];
  byte pcs_tx_lim_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 cdma_tx_lim_vs_volt[NV_VOLT_TABLE_SIZ];
  int1 cdma_tx_lim_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 pcs_tx_lim_vs_freq[NV_FREQ_TABLE_SIZ];
  byte cdma_exp_hdet_vs_agc[NV_CDMA_EXP_HDET_VS_AGC_SIZ];
  byte pcs_exp_hdet_vs_agc[NV_PCS_EXP_HDET_VS_AGC_SIZ];
  byte cdma_err_slp_vs_hdet[NV_CDMA_ERR_SLP_VS_HDET_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  PCS/CDMA RF Calibration items
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  int1 cdma_rx_lin_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pcs_rx_lin_vs_temp[NV_TEMP_TABLE_SIZ];
  byte cdma_adj_factor;
  byte pcs_adj_factor;
  byte cdma_tx_lim_booster_off;
  int1 cdma_rx_slp_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pcs_rx_slp_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 cdma_tx_slp_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pcs_tx_slp_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pa_range_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pcs_pa_range_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pdm1_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pcs_pdm1_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pdm2_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pcs_pdm2_vs_temp[NV_TEMP_TABLE_SIZ];
  int1 pdm1_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 pcs_pdm1_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 pdm2_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 pcs_pdm2_vs_freq[NV_FREQ_TABLE_SIZ];
  int1 lna_switch_vs_temp[NV_TEMP_TABLE_SIZ];
  byte fm_exp_hdet_vs_temp[NV_TEMP_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  ADC offset and gain values for calculating indecies to the above tables
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Type for NV_THERM_I item.                                               */
/* -----------------------------                                           */

  nv_minmax_type     therm; /* RF & LCD calibration */
  nv_minmax_type     vbatt_pa; /*  */

  byte hdet_off;     /* ADC HDET reading offset */
  byte pcs_hdet_off; /* ADC HDET reading offset */
  byte hdet_spn;     /* ADC HDET reading span */
  byte pcs_hdet_spn; /* ADC HDET reading span */
  boolean onetouch_dial;  /* ena/dis UI one touch dialing */
  byte fm_agc_adj_vs_freq[NV_ADJ_VS_FREQ_SIZ];
  byte fm_agc_adj_vs_temp[NV_ADJ_VS_TEMP_SIZ];
  byte rf_config;
  byte r1_rise;
  byte pcs_r1_rise;
  byte r1_fall;
  byte pcs_r1_fall;
  byte r2_rise;
  byte pcs_r2_rise;
  byte r2_fall;
  byte pcs_r2_fall;
  byte r3_rise;
  byte pcs_r3_rise;
  byte r3_fall;
  byte pcs_r3_fall;

/* Type for  NV_PA_RANGE_STEP_CAL_I                                      */
/* -----------------------------                                         */
  byte   pa_range_step_cal;

/* Type for  NV_PCS_PA_RANGE_STEP_CAL_I                                  */
/* -----------------------------                                         */
  byte   pcs_pa_range_step_cal;

/* Type for  NV_LNA_RANGE_POL_I                                          */
/* -----------------------------                                         */
  byte   lna_range_pol;

/* Type for  NV_PCS_LNA_RANGE_POL_I                                      */
/* --------------------------------                                      */
  byte   pcs_lna_range_pol;

/* Type for  NV_LNA_RANGE_RISE_I                                         */
/* -----------------------------                                         */
  int1   lna_range_rise;

/* Type for  NV_PCS_LNA_RANGE_RISE_I                                     */
/* ---------------------------------                                     */
  int1   pcs_lna_range_rise;

/* Type for  NV_LNA_RANGE_FALL_I                                         */
/* -----------------------------                                         */
  int1   lna_range_fall;

/* Type for  NV_PCS_LNA_RANGE_FALL_I                                     */
/* ---------------------------------                                     */
  int1   pcs_lna_range_fall;

/* Type for  NV_LNA_RANGE_OFFSET_I                                       */
/* -------------------------------                                       */
  word   lna_range_offset;

/* Type for  NV_PCS_LNA_RANGE_OFFSET_I                                   */
/* -----------------------------------                                   */
  word   pcs_lna_range_offset;
      
/* Type for  NV_ALERTS_LVL_I                                             */
/* -----------------------------                                         */
  byte   alerts_lvl;

/* Type for  NV_ALERTS_LVL_SHADOW_I                                      */
/* -----------------------------                                         */
  byte   alerts_lvl_shadow;

/* Type for  NV_RINGER_LVL_SHADOW_I                                      */
/* -----------------------------                                         */
  byte   ringer_lvl_shadow;

/* Type for  NV_BEEP_LVL_SHADOW_I                                        */
/* -----------------------------                                         */
  byte   beep_lvl_shadow;

/* Type for  NV_EAR_LVL_SHADOW_I                                         */
/* -----------------------------                                         */
  byte   ear_lvl_shadow;

/* Type for  NV_TIME_SHOW_I                                              */
/* -----------------------------                                         */
  boolean time_show;

/* Type for  NV_MESSAGE_ALERT_I                                          */
/* -----------------------------                                         */
  byte    message_alert;

/* Type for  NV_AIR_CNT_I                                                */
/* -----------------------------                                         */
  nv_call_cnt_type   air_cnt;

/* Type for  NV_ROAM_CNT_I                                               */
/* -----------------------------                                         */
  nv_call_cnt_type   roam_cnt;

/* Type for  NV_LIFE_CNT_I                                               */
/* -----------------------------                                         */
  dword   life_cnt;

/* Type for  NV_DWNR_UP_CNT_I                                             */
/* -----------------------------                                         */
  dword   pwr_dwn_cnt;

/* Type for  NV_SEND_PIN_I                                               */
/* -----------------------------                                         */
  boolean   send_pin;

/* Type for  NV_AUTO_ANSWER_SHADOW_I                                     */
/* -----------------------------                                         */
  byte      auto_answer_shadow;
 
/* Type for  NV_SMS_I                                                    */
/* -----------------------------                                         */
  nv_sms_type   sms;

/* Type for  NV_IMSI_11_12_I                                             */
/* -------------------------                                             */
  nv_imsi_11_12_type  imsi_11_12;

/* Type for  NV_IMSI_T_11_12_I                                           */
/* -----------------------------                                         */
  nv_imsi_11_12_type  imsi_t_11_12;

/* Type for  NV_IMSI_MCC_I                                               */
/* -----------------------                                               */
  nv_imsi_mcc_type    imsi_mcc;

/* Type for  NV_IMSI_T_MCC_I                                             */
/* -----------------------------                                         */
  nv_imsi_mcc_type    imsi_t_mcc;

/* Type for  NV_DIR_NUMBER_I                                             */
/* -------------------------                                             */
  nv_dir_number_type    dir_number;

/* Type for  NV_DIR_NUMBER_PCS_I                                         */
/* -----------------------------                                         */
  nv_mob_dir_data_type  mob_dir_number;

/* Type for  NV_VOICE_PRIV_I                                             */
/* -----------------------------                                         */
  byte    voice_priv;

/* Type for NV_FSC_I item.                                               */
/* --------------------------                                            */
  nv_fsc_type  fsc;

/* Type for  NV_N1M_I                                                    */
/* -----------------------------                                         */
  byte    n1m;

/**********The following items are country specific for ISS2 ***************/
/* Type for NV_ALARMS_I item.                                              */
/* --------------------------                                              */
  nv_alarms_type  alarms;

/* Type for NV_STANDING_ALARM_I item.                                      */
/* --------------------------                                              */
  word  standing_alarm;

/* Type for NV_DIALING_PLAN_I item.                                        */
/* --------------------------                                              */
  word  dialing_plan;

/* Type for NV_ISD_STD_PASSWD_I item.                                      */
/* --------------------------                                              */
  nv_isd_std_passwd_type  isd_std_passwd;

/* Type for NV_ISD_STD_RESTRICT_I item.                                    */
/* --------------------------                                              */
  byte  isd_std_restrict;

/* Type for NV_FM_AGC_I item.                                              */
/* -----------------------                                                 */
  nv_minmax_type     fm_agc; 

/* Type for NV_FSC2_I item.                                                */
/* -----------------------                                                 */
  nv_fsc2_type        fsc2;

/* Type for NV_FSC2_CHKSUM_I item.                                         */
/* ------------------------------                                          */
  nv_fsc2_chksum_type fsc2_chksum;

/* Type for NV_WDC_I item.                                                 */
/* -----------------------                                                 */
  nv_wdc_type        wdc;

/* Type for NV_HW_CONFIG_I item.                                           */
/* -----------------------                                                 */
  dword              hw_config;

/* Type for NV_IMSI_ADDR_NUM_I item.                                       */
/* -----------------------                                                 */
  nv_imsi_addr_num_type imsi_addr_num;

/* Type for NV_IMSI_T_ADDR_NUM_I item.                                     */
/* -----------------------                                                 */
  nv_imsi_addr_num_type imsi_t_addr_num;

/* Type for NV_ASSIGNING_TMSI_ZONE_LEN_I item.                             */
/* -----------------------                                                 */
  nv_tmsi_zone_length_type  assigning_tmsi_zone_length;

/* Type for NV_ASSIGNING_TMSI_ZONE_I item.                                 */
/* -----------------------                                                 */
  nv_tmsi_zone_type  assigning_tmsi_zone;

/* Type for NV_TMSI_CODE_I item.                                           */
/* -----------------------                                                 */
  nv_tmsi_code_type  tmsi_code;

/* Type for NV_TMSI_EXP_I item.                                            */
/* -----------------------                                                 */
  nv_tmsi_exp_time_type tmsi_exp_timer;

/* Type for NV_HOME_PCS_FREQ_BLOCK_I item.                                 */
/* -----------------------                                                 */
  nv_home_block_type home_block;

/* Type for NV_ROAMING_LIST_(683)_I item.                                  */
/* --------------------------------------                                  */
  nv_roaming_list_union_type roaming_list;

/* Type for (now obsolete) NV_MRU_TABLE_I item.                            */
/* -----------------------                                                 */
  nv_mru_table_type mru_table;

/* Type for NV_MRU2_TABLE_I item.                                          */
/* -----------------------                                                 */
  nv_mru2_table_type mru2_table;

/* Type for OTASP "commit" operation (writes several nv items).            */
/* -----------------------                                                 */
  nv_otasp_commit_type * otasp_commit;    

/* Type for NV_OTKSL_I item.                                               */
/* -----------------------                                                 */
  nv_sec_code_type   otksl;

/* Type for NV_TIMED_PREF_MODE_I item.                                     */
/* -----------------------                                                 */
  boolean      timed_pref_mode;

/* Type for NV_RINGER_TYPE_I item.                                         */
/* -----------------------                                                 */
  byte    ringer_type;

/* Type for NV_ANY_KEY_ANSWER_I item.                                      */
/* -----------------------                                                 */
  boolean   any_key_answer;

/* Type for NV_BACK_LIGHT_HFK_I item.                                      */
/* ------------------------------                                          */
/* This values specifies how many seconds should pass before the           */
/* backlighting is turned off, starting from the last moment when any      */
/* key was touched:                                                        */
/*                                                                         */
/* 0        - backlight never on                                           */
/* 1-15     - number of seconds before turning off                         */
/* 16-254   - illegal value                                                */
/* 255      - backlight never off                                          */

  byte      back_light_hfk;

/* Type for NV_KEY_SOUND_I item.                                           */
/* -----------------------------                                           */
  nv_key_sound_enum_type     key_sound;

/* Type for NV_DIALS_SORTING_METHOD_I item.                                */
/* ----------------------------------------                                */
  nv_dials_sort_enum_type   sorting_method;

/* Type for NV_LANGUAGE_SELECTION_I item.                                  */
/* --------------------------------------                                  */
  nv_language_enum_type  language_selection;

/* Type for NV_MENU_FORMAT_I item.                                         */
/* -------------------------------                                         */
  nv_menu_format_enum_type  menu_format;

/* Type for NV_RINGER_SPKR_LVL_I item.                                     */
/* -----------------------------------                                     */
  byte    ringer_spkr_lvl;

/* Type for NV_BEEP_SPKR_LVL_I item.                                       */
/* ---------------------------------                                       */
  byte    beep_spkr_lvl;

/* Type for NV_VIBRATOR_I item.                                            */
/* ----------------------------                                            */
  boolean vibrator;

/* Type for NV_FLIP_ANSWERS_I item.                                        */
/* --------------------------------                                        */
  boolean flip_answers;

/* Type for NV_DIAL_RESTRICT_LVLS_I item.                                  */
/* --------------------------------------                                  */
  nv_dial_restrict_lvls_type dial_restrict_lvls;

/* Type for NV_DIAL_STATE_TABLE_LEN_I item.                                */
/* ----------------------------------------                                */
  nv_dial_state_table_len_type dial_state_table_len;

/* Type for NV_DIAL_STATE_TABLE_I item.                                    */
/* ------------------------------------                                    */
  nv_dial_state_table_elem_type dial_state_table;

/* Type for NV_VOICE_PRIV_ALERT_I item.                                    */
/* ------------------------------------                                    */
  byte    voice_priv_alert;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for NV_DATA_QNC_ENABLED_I item.                                    */
/* ------------------------------------                                    */
  boolean             data_qnc_enabled;

/* Type for NV_DATA_SO_SET_I item.                                         */
/* -------------------------------                                         */
  byte                data_so_set;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for NV_UP_LINK_INFO_I item.                                        */
/* --------------------------------                                        */
  nv_up_link_info_type    up_link_info; 

/* Type for NV_UP_KEY_I item.                                              */
  nv_up_key_type          up_key;

/* Type for NV_UP_PARMS_I item.                                            */
/* ----------------------------                                            */
  nv_up_parms_type        up_parms;

/* Type for NV_UP_CACHE_I item.                                            */
/* ----------------------------                                            */
  nv_up_cache_union_type  up_cache;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for NV_ELAPSED_TIME_I item.                                        */
/* --------------------------------                                        */
  word    elapsed_time;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for NV_PDM2_I item.                                                */
/* ------------------------                                                */
  byte    pdm2;

/* Type for NV_RX_AGC_MINMAX_I item.                                       */
/* ---------------------------------                                       */
  nv_minmax_type    rx_agc_minmax;

/* Type for NV_PCS_RX_AGC_MINMAX_I item.                                   */
/* -------------------------------------                                   */
  nv_minmax_type    pcs_rx_agc_minmax;

/* Type for NV_VBATT_AUX_I item.                                           */
/* -----------------------------                                           */
  nv_minmax_type    vbatt_aux;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for NV_DTACO_CONTROL_I item.                                       */
/* ---------------------------------                                       */
  nv_dtaco_control_type    dtaco_control;

/* Type for NV_DTACO_INTERDIGIT_TIMEOUT_I item.                            */
/* --------------------------------------------                            */
  byte    dtaco_interdigit_timeout;

/* Type for NV_PDM1_I item.                                                */
/* ------------------------                                                */
  byte    pdm1;

/* Type for NV_BELL_MODEM_I item.                                          */
/* ------------------------------                                          */
  boolean bell_modem;

/* Type for NV_SID_NID_LOCK_I item.                                        */
/* --------------------------------                                        */
  nv_sid_nid_lock_type     sid_nid_lock;

/* Type for NV_PRL_ENABLED_I item.                                         */
/* -------------------------------                                         */
  nv_enabled_type          prl_enabled;
 
/* Type for NV_SYSTEM_PREF_I item.                                         */
/* -------------------------------                                         */
  nv_sys_pref_type         system_pref;

/* Type for NV_HOME_SID_NID_I item.                                        */
/* --------------------------------                                        */
  nv_home_sid_nid_type     home_sid_nid;

/* Type for NV_OTAPA_ENABLED_I item.                                       */
/* ---------------------------------                                       */
#ifdef FEATURE_OTASP_OTAPA
  nv_enabled_type          otapa_enabled;
#endif

/* Type for NV_NAM_LOCK_I item.                                            */
/* ----------------------------                                            */
#ifdef FEATURE_OTASP_OTAPA
  nv_enabled_type          nam_lock;
#endif

/* Type for NV_UP_ALERTS_I item.                                           */
/* -----------------------------                                           */
  nv_up_alert_record_type  up_alert_record;

/* Type for NV_UP_IDLE_TIMER_I item.                                       */
/* ---------------------------------                                       */
  word                     up_idle_timer;

/* Type for NV_SMS_UTC_I item.                                             */
/* ---------------------------                                             */
  boolean                  sms_utc;

/* Type for NV_ROAM_RINGER_I item.                                         */
/* -------------------------------                                         */
  boolean                  roam_ringer;

/* Type for NV_RENTAL_TIMER_I item.                                        */
/* --------------------------------                                        */
  nv_rental_timer_type     rental_timer;

/* Type for NV_RENTAL_TIMER_INC_I item.                                    */
/* ------------------------------------                                    */
  byte                     rental_timer_inc;

/* Type for NV_RENTAL_CNT_I item.                                          */
/* ------------------------------                                          */
  dword                    rental_cnt;

/* Type for NV_RENTAL_TIMER_ENABLED_I item.                                */
/* ----------------------------------------                                */
  boolean                  rental_timer_enabled;

/* Type for NV_FULL_SYSTEM_PREF_I item.                                    */
/* ------------------------------------                                    */
  boolean                  full_system_pref;

/* Type for NV_BORSCHT_RINGER_FREQ_I item.                                 */
/* ---------------------------------------                                 */
  byte                     borscht_ringer_freq;

/* Type for NV_PAYPHONE_ENABLE_I item.                                     */
/* -----------------------------------                                     */
  boolean                  payphone_enable;

/* Type for NV_DSP_ANSWER_DET_ENABLE_I item.                               */
/* -----------------------------------------                               */
  boolean                  dsp_answer_det_enable;

/* Type for NV_EVRC_PRI_I item.                                            */
/* ----------------------------                                            */
  nv_evrc_pri_enum_type    evrc_pri;
                                                                                          
/* Type for NV_AFAX_CLASS_20_I item.                                       */
/* ---------------------------------                                       */
  boolean                  afax_class_20;

/* Type for NV_V52_CONTROL_I item.                                         */
/* -------------------------------                                         */
  nv_v52_control_enum_type v52_control;

/* Type for NV_CARRIER_INFO_I item.                                        */
/* --------------------------------                                        */
  nv_carrier_info_type     carrier_info;

/* Type for NV_AFAX_I item.                                                */
/* ------------------------                                                */
  nv_afax_type             afax;

#ifdef FEATURE_EVRC_SO_MGMT
/* Type for NV_PREF_VOICE_SO_I item.                                       */
/* ---------------------------------                                       */
  nv_pref_voice_so_type    pref_voice_so; 
#endif

/* Type for NV_VRHFK_ENABLED_I item.                                       */
/* ---------------------------------                                       */
  boolean                  vrhfk_enabled;

/* Type for NV_VRHRK_VOICE_ANSWER_I item.                                  */
/* --------------------------------------                                  */
  boolean                  vrhfk_voice_answer;

/* Type for NV_SMS_AUTO_DELETE_I item.                                     */
/* -----------------------------------                                     */
  boolean                  sms_auto_delete;

/* Type for NV_SRDA_ENABLED_I item.                                        */
/* --------------------------------                                        */
  boolean                  srda_enabled;

/* Type for NV_OUTPUT_UI_KEYS_I item.                                      */
/* ----------------------------------                                      */
  boolean                  output_ui_keys;

/* Type for NV_POL_REV_TIMEOUT_I item.                                     */
/* -----------------------------------                                     */
  byte                     pol_rev_timeout;
  
/* Type for NV_SI_TEST_DATA_1_I item.                                      */
/* ----------------------------------                                      */
  byte                     si_test_data_1[NV_SI_TEST_DATA_SIZE];

/* Type for NV_SI_TEST_DATA_2_I item.                                      */
/* ----------------------------------                                      */
  byte                     si_test_data_2[NV_SI_TEST_DATA_SIZE];

/* Type for NV_SPC_CHANGE_ENABLED_I item.                                  */
/* --------------------------------------                                  */
  boolean                  spc_change_enabled;

/* Type for NV_DATA_MDR_MODE_I item.                                       */
/* ---------------------------------                                       */
  byte                     data_mdr_mode;

/* Type for NV_DATA_PKT_ORIG_STR_I item.                                   */
/* -------------------------------------                                   */
  nv_data_pkt_orig_str_type data_pkt_orig_str;

/* Type for NV_DATA_AUTO_PACKET_DETECTION_I item.                          */
/* -------------------------------------                                   */
  byte                     data_auto_packet_detection; 

#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif

#ifdef FEATURE_UI_NEW_OTKSL
#error code not present
#endif


#ifdef FEATURE_UI_SMS
/* Type for NV_SMS_REMINDER_TONE_I item.                                   */
/* -------------------------------------                                   */
  byte                     sms_reminder_tone;
#endif


/* Type for NV_BTF_ADJUST_I item.                                          */
/* ------------------------------                                          */
  int2                     btf_adjust;

#ifdef FEATURE_UBROWSER
#error code not present
#endif

#ifdef FEATURE_UI_MINIBROWSER
#error code not present
#endif

#ifdef FEATURE_USER_ZONE
#error code not present
#endif

#ifdef FEATURE_USER_ZONE
#error code not present
#endif

#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
/* Type for NV_PAP_USER_ID_I item.                                         */
/* -------------------------------                                         */
  nv_pap_user_id_type      pap_user_id;

/* Type for NV_PAP_PASSWORD_I item.                                        */
/* --------------------------------                                        */
  nv_pap_password_type     pap_password;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if (defined FEATURE_PA_RANGE_DIG_COMP || defined FEATURE_PA_RANGE_TEMP_FREQ_COMP)
/* Type for  NV_PA_OFFSETS_I item.                                         */
/* -------------------------------                                         */
  nv_pa_offset_type        pa_offsets[NV_NUM_PA_RANGES];
#endif

#ifdef FEATURE_4PAGE_TX_LINEARIZER
/* Type for  NV_CDMA_TX_LIN_MASTER0_I item.                                */
/* ---------------------------------------                                 */
  nv_tx_linearizer_type    cdma_tx_lin_master0;

/* Type for  NV_CDMA_TX_LIN_MASTER1_I item.                                */
/* ---------------------------------------                                 */
  nv_tx_linearizer_type    cdma_tx_lin_master1;

/* Type for  NV_CDMA_TX_LIN_MASTER2_I item.                                */
/* ---------------------------------------                                 */
  nv_tx_linearizer_type    cdma_tx_lin_master2;

/* Type for  NV_CDMA_TX_LIN_MASTER3_I item.                                */
/* ---------------------------------------                                 */
  nv_tx_linearizer_type    cdma_tx_lin_master3;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_BROADCAST_SMS
/* Type for NV_SMS_BC_SERVICE_TABLE_SIZE_I item.                                          */
/* ------------------------------------                                    */
  word                     sms_bc_service_table_size;   

/* Type for NV_SMS_BC_SERVICE_TABLE_I item.                                          */
/* ------------------------------------                                    */
  nv_sms_bc_service_table_type sms_bc_service_table;                                                              

/* Type for NV_SMS_BC_CONFIG_I item.                                       */
/* ------------------------------------                                    */
  byte                     sms_bc_config;                                                              

/* Type for NV_SMS_BC_USER_PREF_I item.                                    */
/* ------------------------------------                                    */
  byte                     sms_bc_user_pref;                                                              
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_RFR3100
#error code not present
#endif

#if (defined(NV_FEATURE_TRIMODE_ITEMS) && defined(FEATURE_RFR3100))
#error code not present
#endif

#ifdef FEATURE_RUIM_CHV_FROM_NV
/* Type for NV_RUIM_CHV_1_I item.                                         */
/* ------------------------------------                                   */
  nv_ruim_chv_type         ruim_chv_1;

/* Type for NV_RUIM_CHV_2_I item.                                         */
/* ------------------------------------                                   */
  nv_ruim_chv_type         ruim_chv_2;
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif


#ifdef FEATURE_IS2000
/* Type for  NV_PREF_FOR_RC_I item.                                        */
/* -------------------------------                                         */
  nv_rc_pref_type          pref_for_rc;
#endif

#ifdef FEATURE_DS_DEFAULT_BITRATE_NV
/* Type for  NV_DS_DEFAULT_BAUD_I item.                                    */
/* -------------------------------                                         */
  nv_sio_baud_type         ds_default_baud;
#endif

#ifdef FEATURE_DIAG_DEFAULT_BITRATE_NV
/* Type for  NV_DIAG_DEFAULT_BAUD_I item.                                  */
/* -------------------------------                                         */
  nv_sio_baud_type         diag_default_baud;
#endif

#ifdef FEATURE_SDEVMAP_MENU_ITEM_NV
/* Type for  NV_SIO_DEV_MAP_MENU_ITEM_I item.                              */
/* -------------------------------                                         */
  int1                     sio_dev_map_menu_item;
#endif

#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
  nv_ref_tempsense_type	ref_tempsense;
  nv_freq_pwr_type		freq_pwr[NV_FREQ_TABLE_SIZ];
  nv_pminmax_type		pa_pwr_p1p2;
#endif	// #ifdef FEATURE_SKTT_CAL

/* reo 00-07-27 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)  || (MODEL_ID == MODEL_B2)//def FEATURE_EQUS_A10B1 
#if 0
/* Type for  NV_SMSRCV_I item.                                  */
    nv_smsrcv_type          sms_rcv_list;
/* Type for  NV_SMSRCV_NUM_I item.                                  */
    byte                    sms_rcv_num; 
#endif
/* Type for  NV_SMSSEND_I item.                                 */
    nv_smssend_type         sms_send_list;
/* Type for  NV_SMSSEND_NUM_I item.                                 */
    byte                    sms_send_num;

/* Type for  NV_SMSSENT_I item.                                 */
    nv_smssend_type         sms_sent_list;
/* Type for  NV_SMSSENT_NUM_I item.                                 */
    byte                    sms_sent_num;

/* Type for  NV_VMSRCV_I item.                                     */
    nv_vmsrcv_type             vms_list;
/* Type for  NV_VMS_NUM_I item.                                 */
    byte                    vms_num;

/* Type for  NV_MYBELL_I item.                                     */    
    nv_mybell_type          mybell_list;
/* Type for  NV_MYBELL_NUM_I item.                                     */    
    byte                    mybell_num;

/* Type for  NV_LIKEPHREASE_I item.                                     */    
    nv_likephrase_type      likephrase_list;
/* Type for  NV_LIKEPHREASE_NUM_I item.                                     */    
    byte                    likephrase_num; 
    
/* Type for  NV_SMSOPTION_I item.                                     */    
    nv_smsoption_type       sms_option;
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
/* <-- */

#ifdef FEATURE_TTY
#error code not present
#endif

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
/* Type for NV_PA_RANGE_OFFSETS_I item.                                    */
/* -------------------------------                                         */
  int2                     pa_range_offsets[NV_NUM_PA_RANGES];

/* Type for NV_TX_COMP0_I item.                                            */
/* -------------------------------                                         */
  nv_tx_comp_type          tx_comp0;
#endif

#ifdef FEATURE_STEREO_DAC
#error code not present
#endif

#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
/* Type for NV_TX_COMP1_I item.                                            */
/* -------------------------------                                         */
  nv_tx_comp_type          tx_comp1;
  
/* Type for NV_TX_COMP2_I item.                                            */
/* -------------------------------                                         */
  nv_tx_comp_type          tx_comp2;
  
/* Type for NV_TX_COMP3_I item.                                            */
/* -------------------------------                                         */
  nv_tx_comp_type          tx_comp3;
#endif

/* Type for NV_PRIMARY_DNS_I item.                                         */
/* -------------------------------                                         */
  dword                    primary_dns;
  
/* Type for NV_SECONDARY_DNS_I item.                                       */
/* -------------------------------                                         */
  dword                    secondary_dns;

#ifdef FEATURE_DIAG_PORT_SELECT
/* Type for NV_DIAG_PORT_SELECT_I item.                                    */
/* -------------------------------                                         */
  byte                     diag_port_select; 
#endif              

#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif
    
#ifdef FEATURE_IS2000_R_SCH
/* Type for NV_DATA_SCRM_ENABLED_I item.                                   */
/* -------------------------------                                         */
  boolean                  data_scrm_enabled;
#endif

#ifdef FEATURE_RUIM_SMSS
/* Type for NV_RUIM_SMS_STATUS_I item.                                     */
/* -------------------------------                                         */
  nv_ruim_sms_status_type  ruim_sms_status;
#endif

#ifdef NV_FEATURE_TRIMODE_ITEMS
#error code not present
#endif
  
#ifdef FEATURE_DIAG_RESTART_CONFIG
/* Type for NV_DIAG_RESTART_CONFIG_I item.                                 */
/* -------------------------------                                         */
  nv_diag_restart_config_type diag_restart_config;
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
/* Type for NV_DATA_TRTL_ENABLED_I item.                                   */
/* -------------------------------                                         */
  boolean                  data_trtl_enabled;
#endif

#ifdef FEATURE_SD20
#error code not present
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif

#ifdef FEATURE_FACTORY_TESTMODE                   
/* Type for NV_FTM_MODE_I item.                                            */
/* -------------------------------                                         */
  boolean                  ftm_mode;
#endif

#ifdef FEATURE_DS_DEFAULT_BITRATE_NV
/* Type for  NV_DS_DEFAULT_BAUDRATE_I item.                                */
/* -------------------------------                                         */
  nv_sio_baudrate_type     ds_default_baudrate;
#endif

#ifdef FEATURE_DIAG_DEFAULT_BITRATE_NV
/* Type for  NV_DIAG_DEFAULT_BAUDRATE_I item.                              */
/* -------------------------------                                         */
  nv_sio_baudrate_type     diag_default_baudrate;
#endif

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif

  nv_voice_info_type	voice_info;               /* hjcho 00.12.19 */

  nv_fn_sequence_type	Fn_sequence;              /* hjcho 00.12.19 */

  byte                  data_ipr_mode;            /* white 2001.03.20 */
  byte			        data_rev_mode;            /* white 2001.03.20 */

  dword                 flip_count;     // woodstok 01.5.3
  nv_multi_sid_type     multi_sid;      // hjcho 01.09.17 for MULTI_SID

#ifdef FEATURE_NV_UNIT_TEST
#error code not present
#endif


/* Type for NV_PHONETAG_I item.                                        */
/* --------------------------------                                        */
  nv_phoneTAG_type      phoneTAG;   /* 공장용 */

} nv_item_type;


/*-------------------------------------------------------------------------*/

/* The command on the NVM queue */

typedef struct {
  q_link_type           link;           /* Queue field */
  nv_items_enum_type    item;           /* Item to access */
  nv_func_enum_type     cmd;            /* READ, WRITE, PEEK, POKE, etc. */
  nv_item_type          *data_ptr;      /* Pointer to read or write data */
  rex_tcb_type          *tcb_ptr;       /* Task to signal */
  rex_sigs_type         sigs;           /* Rex signals for task */
  q_type                *done_q_ptr;    /* Where to place done buffer */
  nv_stat_enum_type     status;         /* Status of request */
} nv_cmd_type;

/*-------------------------------------------------------------------------*/

/* Return to default packing of data structures.                        */
#ifdef FEATURE_TREX
#error code not present
#endif

/*===========================================================================
                                                                         
                              MACROS                                     
                                                                         
===========================================================================*/


/* Macro to set specific bits of the SMS deletion_list (which specifies    */
/* which SMS messages are to be deleted prior to writing a new one).       */
/* Each call sets exactly one bit, but leaves all other bits unchanged.    */

#define NV_SET_SMS_DELETE_LIST( sms_index, delete_list )    \
          ( (dword)(0x80000000>>(sms_index))|(delete_list) )

/* Macro to allow reading specific bits of the SMS deletion_list by        */
/* simple indexing.  Output is a Boolean, TRUE if the corresponding        */
/* deletion bit is set, FALSE otherwise.                                   */

#define NV_READ_SMS_DELETE_LIST( sms_index, delete_list )   \
          ( (( (0x80000000>>(sms_index))&(delete_list) ) ? TRUE : FALSE) )


#ifdef SW_RESET_ENABLE
#if (MODEL_ID == MODEL_B1)  || (MODEL_ID == MODEL_B2)
#define	NV_SW_RESET_CHECK_POS	0x0127FFF8 
#define NV_SW_RESET_CHECK_VAL	0xabcd
#endif
#endif

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/



/*===========================================================================

FUNCTION NV_TASK

DESCRIPTION
  The main control task calls this function with rex_def_task to start up
  the nv task.  This function then initializes NV timers and command queue.
  If the major revision number has changed then this function rebuilds the
  NV itself.  Once initialization is complete the function sets MC_ACK_SIG
  and it waits for NV_START_SIG.  When the start signal is received the
  function reads and processes any request it finds on its queue until
  there are no more requests.  Once the request queue is empty the task
  then starts normal operation by waiting for and processing any new commands
  when they arrrive on its command queue.  Throughout its operation the
  NV task wakes up periodically to kick the watchdog task.

DEPENDENCIES
  This function must be called with rex_def_task _once_.  It requires the
  NV_START_SIG before proceeding past the initialization stage.

RETURN VALUE
  Never returns.

SIDE EFFECTS
  Modifies NVM.

===========================================================================*/

extern void nv_task
(
  dword param      /* parameter from REX, unused */
);


/*===========================================================================

FUNCTION NV_CMD

DESCRIPTION
  This is the outside world's interface to the non volatile memory task.
  This function takes an already filled out nv_cmd_type which it places
  on the nv command queue.  The function then returns to the caller.
  Return to the caller does not mean that the command has been executed
  yet, only that it is queued for execution.  When nv_cmd is done processing
  the request it returns the buffer to the specified queue if a return
  queue adress is provided.  Otherwise it does not move the buffer.

  An example NV READ request for MIN1 is as follows:
                              
  static   nv_cmd_type       cmd_buf;        - Define command buffer
  static   nv_item_type      data_buf;       - Define data buffer

  data_buf.min1.nam  = current_nam;          - Specify which NAM          
  cmd_buf.cmd        = NV_READ_F;            - Specify read operation     
  cmd_buf.tcb_ptr    = rex_self();           - Own TCB                    
  cmd_buf.sigs       = XX_NV_CMD_SIG;        - Signal to set when done    
  cmd_buf.done_q_ptr = NULL;                 - No buffer return required  
  cmd_buf.item       = NV_MIN1_I;            - Specify the item to read   
  cmd_buf.data_ptr   = &data_buf;            - Where to place read data   

  rex_clr_sigs (rex_self(), XX_NV_CMD_SIG);  - Clear the signal        
  nv_cmd (&cmd_buf);                         - And issue the command   

DEPENDENCIES
  The NV task must have been started up already.  All of the items in
  the nv_cmd_type must be already set.

RETURN VALUE
  None directly.  The requested data is returned in the buffer pointed
  to inside the nv_cmd_type.  The status variable of the nv_cmd_type
  will be updated to reflect the current status of this command as it
  is processed.  In the above nv_cmd buffer nv_cmd.status is set to:

  NV_DONE_S,      -  Request completed okay 
  NV_BUSY_S,      -  Request is queued 
  NV_BADCMD_S,    -  Unrecognizable command field 
  NV_FULL_S,      -  The NVM is full 
  NV_FAIL_S,      -  Command failed, reason other than NVM was full 
  NV_NOTACTIVE_S, -  Variable was not active 
  NV_BADPARM_S,   -  Bad parameter in command block 
  NV_READONLY_S,  -  Parameter is write-protected and thus read only 
  NV_BADTG_S,     -  Item not valid for Target 
  NV_NOMEM_S,     -  free memory exhausted 
  NV_NOTALLOC_S   -  address is not a valid allocation 

SIDE EFFECTS
  The nv_cmd_type is placed on the NV command queue.  It must not be
  modified until the command has been processed!

===========================================================================*/

extern void nv_cmd
(
  nv_cmd_type *cmd_ptr  /* pointer to the NV command block */
);



/*===========================================================================

FUNCTION NV_SMS_SIZEOF

DESCRIPTION
  When passed the number of SMS message bytes, this function returns
  the number of "raw" bytes of NVRAM that will be required to store
  the SMS message.
  
DEPENDENCIES
  None.

RETURN VALUE
  The number of "raw" bytes of NVRAM required to store the specified
  SMS message length.

SIDE EFFECTS
  None.

===========================================================================*/

word nv_sms_sizeof
(
  word sms_message_size       /* Size of sms message data in bytes */
);



/*===========================================================================

FUNCTION NV_MEMAVAIL

DESCRIPTION
  Return the number of bytes available in the long pool for allocation
  The number is calculated at each write or free operation, and stored
  globally. This function provides an interface to that global data.

DEPENDENCIES
  nva_memavail must have been called

RETURN VALUE
  The number of bytes available in the long pool for allocation

SIDE EFFECTS
  None.

===========================================================================*/

extern word nv_memavail (void);

/*===========================================================================

FUNCTION NV_READ_ERR_LOG

DESCRIPTION
  This function reads an error log from NVM.

DEPENDENCIES
  The function MUST NOT USE ANY TASK SERVICES, so that it can be
  called when tasks are not initialized.  The function MUST NOT
  USE ERROR SERVICES.  It returns status to the caller if there
  is a failure.

RETURN VALUE
  NV_DONE_S         if it worked
  NV_NOT_ACTIVE_S   if item was not active
  NV_FAIL_S         if item could not be read

SIDE EFFECTS
  None.

===========================================================================*/

nv_stat_enum_type nv_read_err_log
(
  nv_err_log_type  *nv_err_log_ptr    /* Pointer where to return read data */
);

/*===========================================================================

FUNCTION NV_WRITE_ERR_LOG

DESCRIPTION
  This function writes the boot error log to NVM.

DEPENDENCIES
  The function MUST NOT USE ANY TASK SERVICES, so that it can be
  called when tasks are not initialized.  The function MUST NOT
  USE ERROR SERVICES.  It returns status to the caller if there
  is a failure.

RETURN VALUE
  NV_DONE_S - if it worked
  NV_FAIL_S - if item could not be written

SIDE EFFECTS
  None.

===========================================================================*/

nv_stat_enum_type nv_write_err_log
(
  nv_err_log_type  *nv_err_log_ptr    /* Pointer where to get write data */
);


/*===========================================================================

FUNCTION NV_MAX_SIZE_OF_ROAMING_LIST

DESCRIPTION
  This function returns the maximum number of bytes available for
  storage of a roaming list (per NAM).  The value returned depends 
  on the total amount of NVRAM installed in the phone and the total 
  number of NAMs defined.  

DEPENDENCIES
  The function cannot be called until NV is initialized.  A
  premature call will raise err_fatal.

RETURN VALUE
  The maximum number of bytes available for storage of a roaming list
  (per NAM).

SIDE EFFECTS
  None.

===========================================================================*/

word  nv_max_size_of_roaming_list (void);


/*===========================================================================

FUNCTION NV_MAX_SIZE_OF_WEB_CACHE

DESCRIPTION
  This function returns the maximum number of bytes currently available 
  for storage of the Unwired Planet cache.  Currently, this item is not
  supported, so this function will always return zero.
  
DEPENDENCIES
  The function cannot be called until NV is initialized.  A
  premature call will raise err_fatal.

RETURN VALUE
  The maximum number of bytes currently available for storage of the
  Unwired Planet cache.

SIDE EFFECTS
  None.

===========================================================================*/

word  nv_max_size_of_web_cache (void);


/*===========================================================================

FUNCTION NV_DP_CAPACITY

DESCRIPTION
  This function returns the maximum capcity of the dynamic pool.
  
DEPENDENCIES
  The function cannot be called until NV is initialized.

RETURN VALUE
  The maximum capacity (in bytes) of the dynamic pool.

SIDE EFFECTS
  None.

===========================================================================*/

word  nv_dp_capacity( void );


/*===========================================================================

FUNCTION NV_RESERVE_MEM

DESCRIPTION
  This function reserves a specified quantity of dynamic pool memory
  for the caller.  All users of the dynamic pool must call this function
  prior to attempting a "write" operation.  (Note: currently, only SMS
  messages are stored in the dynamic pool).  
  
  If there is not enough available memory to fulfill the request, the
  input "accept_partial_reservation" flag is checked.  If TRUE, then all
  remaining free memory will be reserved for the caller (even though it 
  will be less than the amount requested); if FALSE, then no memory will 
  be reserved.
  
  If a reservation is made, a confirmation code is returned to the caller.
  This confirmation code must be passed back to the NV task with the 
  subsequent "write" command.
  
DEPENDENCIES
  None.

RETURN VALUE
  An "nv_reservation_type" object is returned which contains two items:
  1) the actual amount of dynamic pool memory reserved; and
  2) a confirmation code.

SIDE EFFECTS
  The "memory available" state data items may be changed.

===========================================================================*/

nv_reservation_type  nv_reserve_mem
( 
  word     amount_requested,
  boolean  accept_partial_reservation
);


/*===========================================================================

FUNCTION NV_CANCEL_RESERVATION

DESCRIPTION
  This function cancels a previously-made reservation for a specified 
  quantity of dynamic pool memory (see function "nv_reserve_mem").  
  
  Note that this function only needs to be called if a reservation
  is made that will NOT be followed by a "write" command; that is,
  if the caller decides he will not be needing the reserved memory
  after all.  Normally, a reservation disappears automatically when 
  the corresponding "write" command is processed by the NV task.
  
  The confirmation code received by the previous call to nv_reserve_mem
  must be passed in to allow identification of which reservation is
  being cancelled and how many bytes to return to "available" status.
  
DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S     - if all worked properly.
  NV_BADPARM_S  - if an invalid confirmation code was provided.

SIDE EFFECTS
  The "memory available" state data items may be changed.

===========================================================================*/

nv_stat_enum_type  nv_cancel_reservation
( 
  dword    confirmation_code
);


/*===========================================================================

FUNCTION NV_PRL_IS_VALID

DESCRIPTION
  This function returns the "valid" indicator from the specified NAM's
  roaming list.  

DEPENDENCIES
  None.

RETURN VALUE
  TRUE         if the roaming list "valid" field is TRUE 
  FALSE        if the roaming list "valid" field is FALSE or if the
                 specified NAM is invalid or if the roaming list
                 is currently NOT_ACTIVE

SIDE EFFECTS
  None.

===========================================================================*/

boolean  nv_prl_is_valid
(
  byte  nam
);


/*===========================================================================

FUNCTION NV_PRL_VERSION

DESCRIPTION
  This function returns the "prl_version" indicator from the specified NAM's
  roaming list.  

DEPENDENCIES
  None.

RETURN VALUE
  prl_version             if the roaming list for the specified NAM is
                            currently ACTIVE 
  NV_PRL_VERSION_INVALID  if the specified NAM is invalid or if the 
                            roaming list for the specified NAM is 
                            currently NOT_ACTIVE

SIDE EFFECTS
  None.

===========================================================================*/

word  nv_prl_version
(
  byte  nam
);


/*===========================================================================

FUNCTION NV_BUILT

DESCRIPTION
  This function checks the major version number of the the NV task
  as stored at edit time, against the major version number of the
  NV EEPROM as read from NVM.  If the numbers are different then the
  function returns FALSE, which indicates that the NV is not built
  and it may not be accessed from any external task.  If the numbers
  are the same then the function returns TRUE, which indicates that
  NV is built and it may be accessed.  This function lets other units
  issue direct NV read and write commands if the status returned is
  TRUE, thus allowing NV read and write before the NV task has been
  started.

DEPENDENCIES
  This is a special use function, normally called by error services
  to allow early access to NV, and before the NV task has been started.
  If NV_BUILT returns TRUE the NV may be accessed BUT ONLY USING DIRECT
  READ AND WRITES OF THE ERROR LOG (NV_READ_ERR_LOG and NV_WRITE_ERR_LOG).
  The NV task itself can not be counted upon to be started yet and normal
  NV requests must not be used.  As with all direct read and write
  operation, the data item (here it is the major version number) must
  be contained on a signle physical NV page.

RETURN VALUE
  TRUE      - The NV has been built and direct read/write is allowed
  FALSE     - The NV has not been built and access is not allowed

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean nv_built (void);


/*===========================================================================

FUNCTION NV_DETERMINE_ESN_VALIDITY

DESCRIPTION
  This function is determines if the current ESN has been programmed with the
  status of this check returned.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the current ESN is non-zero with proper checksums, else FALSE

SIDE EFFECTS

===========================================================================*/

boolean nv_determine_esn_validity( void);

#endif /* NV_H */
#endif
