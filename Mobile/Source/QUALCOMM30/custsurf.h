#ifndef CUSTSURF_H
#define CUSTSURF_H
/*===========================================================================

            " M S M 5 0 0 0 -  S U R F "   H E A D E R   F I L E

DESCRIPTION
  Configuration for SURF Target using MSM5000 ARM Processor.
  This is a base file with FEATUREs used in all builds.

  Copyright (c) 1999, 2000, 2001 by QUALCOMM Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/custsurf.h_v   1.49.1.0   03 May 2001 12:22:04   nbouchar  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/02/01   nxb     Removed FEATURE_TC_8X_SEARCHER.
03/29/01   nxb     Make FEATURE_NEW_SMS conditional to FEATURE_UASMS.
03/27/01   kjn     Defined FEATURE_DS_ENABLE_RM_DMNCY_IF_CTA_0.
03/13/01   kjn     Added and defined FEATURE_TX_ADJ_WINDDOWN and 
				   FEATURE_MAX_PWR_PROTECTION.
03/07/01   nxb     SMS feature cleanup.
03/06/01   nxb     Added/define FEATURE_FACTORY_TESTMODE
03/05/01   dwp     Added define for MSMHW_UART1_512BYTE_FIFOS in SIO section.
                   It replaces T_MSM5000 in siors232.c and must not be undef'd.
02/28/01   nxb     Fixed defines within SCH.
                   undef FEATURE_DIAG_V7_LOG_COMPATIBILITY
02/22/01   nxb     FEATURE_IS2000_QPCH_CCI
02/06/01   nxb     Undef FEATURE_NEWCM and FEATURE_NEWUASMS
02/06/01   nxb     Added/defined FEATURE_NEWCM.
                   Removed FEATURE_DS_PCM,
02/05/01   nxb     Removed FEATURE_DS_TEST_MEM_POOL, FEATURE_ENHANCED_STANDBY
                   Removed FEATURE_DS_FADE_DORMANCY
                   Mainlined FEATURE_DS_CTA_DCD_ASSERT
                   Added/defined FEATURE_NEWUASMS 
                   Added FEATURE_DS_ENABLE_RM_DMNCY_IF_CTA_0
01/08/00   ak      Defined FEATURE_IS2000_LTU_CRCS, which allows LTU CRC
                   checking on conv. coded SCH's.
01/03/01   nxb	   Undef FEATURE_QWORD_MISALIGNED_METHODS.
12/18/00   nxb	   Defined FEATURE_QWORD_MISALIGNED_METHODS.
                   Defined FEATURE_COMPOSE_MO_SMS
12/12/00   nxb	   Undefined FEATURE_QWORD_MISALIGNED_METHODS.
11/20/00   kjn	   Added and defined FEATURE_QWORD_MISALIGNED_METHODS.
11/20/00   kjn	   Added and defined FEATURE_NEW_DSM.
11/13/00   nxb     Define FEATURE_DSM_DIAG_ITEMS
                   Define FEATURE_DIAG_V7_LOG_COMPATIBILITY
                   Defined FEATURE_16BIT_SHADOW_REG to put 8 bits and 16 bits
                   shadow registers in one array.
                   Removed FEATURE_UI_QZ_TO_79 (obsolete)
                   Undef FEATURE_DIAG_NO_EVENTS to turn ON events.
11/13/00   bgc     Defined FEATURE_EVRC_RDA for improved EVRC rate
                   determination algorithm.
11/13/00   bgc     Defined MSMHW_TURBO_CODING for MSM5000.
11/04/00   nxb     Undefine FEATURE_DSM_DIAG_ITEMS until issues resolved.
11/02/00   lcc     Defined FEATURE_COMPLETE_SNM.
10/29/00   nxb     Define FEATURE_DSM_DIAG_ITEMS.
10/26/00   nxb     Defined R_SCH for 3.0 builds.
10/16/00   nxb     Defined FEATURE_ENHANCED_STANDBY_III.
09/20/00   nxb     Added FEATURE_DATA_TIMELINE_ADVANCE to be in sync with r3.0
09/13/00   nxb     Defined FEATURE_DIAG_RESTART_CONFIG 
                   Defined FEATURE_SCH_TRIAGE
09/05/00   nxb     Removed FEATURE_CS_ACTION_TIME_TRANS code main-lined.
                   Changed FEATURE_DS_MEM_CHK to FEATURE_DSM_MEM_CHK
                   Defined FEATURE_TC_8X_SEARCHER.
08/21/00   nxb     Remove FEATURE_SRCH_DYNAMIC_BAND_CLASS code main-lined.
08/01/00   nxb     Undefined FEATURE_IS2000_R_SCH (see 3.0 releases).
07/10/00   kjn     Defined FEATURE_IS2000_R_SCH. Added FEATURE_DS_PPP_TERM_REQ_DROP. 
                   Added and defined FEATURE_IS2000_TDSO.
06/26/00   kjn     Added and defined FEATURE_RELATIVE_DMOD_POS, FEATURE_RDA_CORRECTION.
06/16/00   jcw     Added flags for 27MHz wait state control and debug targets
06/13/00   HQ      Added #define FEATURE_CS_ACTION_TIME_TRANS.
06/07/00   kjn     Defined FEATURE_IS2000_QPCH.
05/31/00   kjn     Added and defined FEATURE_IS2000_SCH_STATS, FEATURE_DIAG_QCT_EXT.
05/31/00   kjn     Added and defined FEATURE_DS_CHAP, FEATURE_BROADCAST_SMS,
				   FEATURE_WCARD_SID_MGMT, FEATURE_OTASP_OTAPA.
05/23/00   kjn     Removed features that use the improved TX AGC for MSM5000.  They are
				   only supported in the PCS hw.
05/16/00   kjn     Added and defined FEATURE_NONSLOT_OFREQ_NSRCH. Moved data support
				   to the main branch.
04/25/00   lcc     Properly defined features for data release.
04/25/00   va      Removed FEATURE_DEFAULT_BIT_RATE_38400 and
                   FEATURE_JTAG_DEBUG
04/21/00   tim     Defined FEATURE_REX_PREEMPT_INTS
04/17/00   va      Defined  FEATURE_DIAG_V7_LOG_COMPATIBILITY
                   UnDefined  FEATURE_DIAG_V7_LOG_IFACE
04/17/00   va      Defined FEATURE_KEYPRESS_OPTIMIZATION
04/17/00   jwh     Defined FEATURE_DS_UI_BAUD, FEATURE_DS_DEFAULT_BITRATE_NV.
                   Changed conditional of FEATURE_SDEVMAP_SHARE_HFK.
04/12/00   va      New SIO features for changing baudrate.
                   FEATURE_SDEVMAP_UI_MENU
                   FEATURE_SDEVMAP_MENU_ITEM_NV
                   FEATURE_SDEVMAP_SHARE_HFK
                   FEATURE_DIAG_UI_BAUD
                   FEATURE_DIAG_DEFAULT_BITRATE_NV
04/11/00   kjn     This file now contains all common feature definitions
04/05/00   kjn     Undefined FEATURE_DETECT_SUP_MUTING.
04/05/00   kjn     Added & defined FEATURE_OTASP_HOLD_CMD_IN_COMMIT,
                                   FEATURE_UI_8K_SUPPORT, FEATURE_UI_8K_VOICE_SUPPORT.
04/03/00   va      Defined FEATURE_SRCH_DYNAMIC_BAND_CLASS feature
03/27/00   va      Undefed SCH related features. defined the following:
                   FEATURE_DETECT_SUP_MUTING
                   FEATURE_UI_DEBUG_SLOTTED_MODE
                   FEATURE_SEARCH2 (16x search)
                   FEATURE_SEARCH2_ACQ (16x search)
                   FEATURE_SEARCH2_IDLE (16x search)
                   FEATURE_UART_TCXO_CLK_FREQ
03/06/00   lcc     Added feature definitions for SCH related stuff.
03/07/00   kjn     Added FEATURE_DIAG_DEFAULT_BITRATE_38400.
02/29/00   kjn     Added FEATURE_DIAG_V7_LOG_IFACE, FEATURE_DIAG_NON_STREAMING,
                                        FEATURE_DIAG_NO_EVENTS.
02/23/00    eh     Added T_MSM5000A_SUPPORT
02/12/00   hrk     Added FEATURE_RLGC
02/07/00   va      Added FEATURE_VOC_SAT_CONTROL, RXC_ACTION_TIME_PROCESSING.
                   Removed FEATURE_INT_25_FIQ
01/19/00   lcc     Removed FEATURE_SPECIAL_MDR which is not supported in this
                   archive.  Removed TEMP_HACKS_FOR_FOR_TIGER.
                   Also undefined features not for this release.
11/03/99   lcc     Deleted FEATURE_VIRTUAL_COMBINER which is mainlined.
                   Added and defined FEATURE_SERIAL_DEVICE_MAPPER.
11/02/99   lcc     Added and defined FEATURE_IS95B_ACC_PRB_BKOFF_DLY.
10/27/99   ajn     FEATURE_IS2000_QPCH
10/20/99   lcc     Added block for IS2000 features and enabled MDR.
10/11/99   lcc     Corrected edit history to show first release for MSM5000.
10/08/99   lcc     Initial revision supporting MMS5000.  Based on same file
                   from Panther archive.

===========================================================================*/


/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
                            General Interest
---------------------------------------------------------------------------*/
#define FEATURE_KOREA_BAND

/* Define features of interest in field test and validation
*/
#undef  TMOBILE

/* Define Power Class III SCM presentation
*/
#define FEATURE_POWER_CLASS_III

/* Perform AGC/power control logging
*/
#define FEATURE_AGC_LOGGING

/* Tell MC to use clk_uptime() for some of its timers.
*/
#define FEATURE_MC_TIMER_FIX

/* Dynamic searcher throttle
*/
#define FEATURE_DYNAMIC_THROTTLE

/* Pre-emptive interrupts 
*/
#define FEATURE_REX_PREEMPT_INTS

/* Allows other frequency neighbor searches to occur during non-slotted mode.
*/
#define FEATURE_NONSLOT_OFREQ_NSRCH

/* Rate determination adjustments for SCH DTX detection
*/ 
#define FEATURE_RDA_CORRECTION

/* Rate determination improvments for EVRC
*/
#define FEATURE_EVRC_RDA 

/* Put 8 bits and 16 bits shadow registers in one array.
*/
#define FEATURE_16BIT_SHADOW_REG
  
/* Define if NOT using new optimized quad word function
*/   
#undef FEATURE_QWORD_MISALIGNED_METHODS

/* Enable factory test mode */
// bwpark 2001.06.11
//#define FEATURE_FACTORY_TESTMODE

/* Prevent further collection of Power Control down bits when the mobile
   transmit power is at minimum
*/
#define FEATURE_TX_ADJ_WINDDOWN


/*---------------------------------------------------------------------------
                            Call Processing
---------------------------------------------------------------------------*/

/* Authentication and Voice Privacy
*/
#undef FEATURE_AUTH

/* Phone will do OTASP Protocol
** This feature includes full compliance with IS-683A except for OTAPA.
*/
#define  FEATURE_OTASP

/* Preferred Roaming
*/
#define  FEATURE_PREFERRED_ROAMING

/* These features need to be turned on to enable all the functionality needed for
** FEATURE_OTASP.
*/
#define  FEATURE_IS683A_PRL
#define  FEATURE_HOME_ONLY

/* This is a new task used by OTASP to perform the A-Key exchange.  This feature enables
** the task and the code to send commands to and receive reports from the task.
*/
#undef  FEATURE_DH
#undef  FEATURE_DH_EXP

/* This feature is required to ensure the proper functionality of the preferred roaming
** code, for both PCS and SSPR 800 builds.
*/
#define  FEATURE_MAX_ACCESS_FALLBACK

/* GSRM Service Indicator
*/
#define  FEATURE_GSRM_SVC_IND

/* EVRC SO Management
*/
#define  FEATURE_EVRC_SO_MGMT

/*  Class 1 IMSI support
*/
#define FEATURE_CLASS_1_IMSI

/*  Emergency 911
*/
//kjh #define FEATURE_E911

/*  Use faster algorithm to scan through the PRL when a system is acquired
*/
#define FEATURE_FASTPRL

/*      Hold OTASP command in commit
*/
#define FEATURE_OTASP_HOLD_CMD_IN_COMMIT

/* Support for Over the Air Authentication
*/ 
#define  FEATURE_OTASP_OTAPA

/*  Wildcard System ID Management   
*/
#define FEATURE_WCARD_SID_MGMT

/*---------------------------------------------------------------------------
                             Diagnostics
---------------------------------------------------------------------------*/

/* Diag originated test calls, with UI command
*/
#define  FEATURE_DIAG_ORIG_CALL

/* Client logging interface using log_put()
*/
#undef FEATURE_DIAG_V7_LOG_IFACE
#undef FEATURE_DIAG_V7_LOG_COMPATIBILITY

/* Poll/response serial protocol
*/
#undef FEATURE_DIAG_NON_STREAMING

/* Removes event reporting services
*/
#define FEATURE_DIAG_NO_EVENTS

/* Support for Diagnostic Extensions
*/
#define FEATURE_DIAG_QCT_EXT

/* Allows for boot sequence logging/debugging.
*/
#define FEATURE_DIAG_RESTART_CONFIG

/*---------------------------------------------------------------------------
                          Non-Volatile Memory
---------------------------------------------------------------------------*/

/* NV task will auto-detect the EEPROM size and automatically
** scale the roaming list size
*/
#undef  FEATURE_NV_AUTOSIZE

/* Phone's NV memory will support one NAM of 625 bytes, and the one-time
   subsidy lock item
*/
#undef  FEATURE_NV_ONE_NAM_RL_LARGE

/* This feature configures NV to have two small roaming lists for a two NAM phone,
** as opposed to having one large roaming list for a one NAM phone.
*/
#define  FEATURE_NV_TWO_NAMS_RL_SMALL

/*---------------------------------------------------------------------------
                            Data Services
---------------------------------------------------------------------------*/

/* Data Services
*/
#define FEATURE_DS

#define	FEATURE_SKT_DS	//	khekim 00/03/22

#ifdef FEATURE_DS

/* Enables features used for test
*/
#define FEATURE_DS_AT_TEST_ONLY

/* Enables Data on the Go AMPS data
*/
#undef FEATURE_DS_DOTG_DATA

/* Enables Original IS-99 Command Reflection Scheme
*/
#undef  FEATURE_DS_IS99_REFL

/* Enable Data Services memory checks (DSM, optimized code).
*/
#define FEATURE_DSM_MEM_CHK

/* Used to check for DSM memory leaks after a data call
   Used only in testing.
*/
/* DSM diag. items.
*/
#define FEATURE_DSM_DIAG_ITEMS

/* use DSM item scheme, with DUP pool
*/
#define FEATURE_NEW_DSM


#undef FEATURE_DS_TEST_MEM_POOL

/* Allows an OK to ATD #777, allows packet services for WIN95
*/
#ifdef	FEATURE_SKT_DS	//	khekim 00/09/24
#undef FEATURE_DS_ATD_PKT
#else	//	#ifdef	FEATURE_SKT_DS
#define FEATURE_DS_ATD_PKT
#endif	//	#ifdef	FEATURE_SKT_DS

/* Enables packet based Browser Interface
*/
#undef FEATURE_DS_BROWSER_INTERFACE

/* Enables Network Model
*/
#define FEATURE_DS_NET_MODEL

/* Enables Password Authentication Protocal
*/
#define FEATURE_DS_PAP	//	Packet : Network Model work well

/* Enable use of NV to store PAP user id and password
   Requires FEATURE_DS_PAP to be enabled.
*/
#ifdef	FEATURE_SKT_DS	//	khekim 00/09/24
 #undef FEATURE_DS_PAP_NV
#else	//	#ifdef	FEATURE_SKT_DS
#ifdef FEATURE_DS_PAP
 #define FEATURE_DS_PAP_NV
#endif
#endif	//	#ifdef	FEATURE_SKT_DS

/* Enables Challenge Authentication Protocol
*/
#ifdef	FEATURE_SKT_DS	//	khekim 00/09/24
#undef	FEATURE_DS_CHAP
#else	//	#ifdef	FEATURE_SKT_DS
#define FEATURE_DS_CHAP
#endif	//	#ifdef	FEATURE_SKT_DS

/* Enables reconnection on packet zone ID change
*/
#define FEATURE_DS_PZID_RECON

/* Enables DCD to stay high after a packet call ends
   if AT+CTA > 0, independent of AT&Cx.
*/
#define FEATURE_DS_CTA_DCD_ASSERT

/* Enables packet based sockets interface
*/
#define FEATURE_DS_SOCKETS

/* Enables Protocol statistics for Data Services
*/
#define FEATURE_DS_PSTATS

/* Enables Debug UI for Data Services
*/
#define FEATURE_DS_DEBUG_UI

/* Enable AT cmd to specify the relay model data service option number
*/
#define FEATURE_DS_RPSO

/* Enable Mobile to Mobile calls for IS-99 and IS-657
*/
#define FEATURE_DS_MTOM

/*********************************************************************
  Exactly ONE of FEATURE_DS_ORIG_XXX_SO must be defined.
*********************************************************************/
/* Originate with IS-707.0 (Pre-707) service options
   (If FEATURE_DS_CHOOSE_SO is defined, this specifies the default set.)
*/
#define FEATURE_DS_ORIG_PRE707_SO     // bwpark 2000.02.16 #undef ---> #define

/* Originate with proprietary service options
   (If FEATURE_DS_CHOOSE_SO is defined, this specifies the default set.)
*/
#undef FEATURE_DS_ORIG_PROPTRY_SO

/* Originate with IS-707 standard service options
   (If FEATURE_DS_CHOOSE_SO is defined, this specifies the default set.)
*/
#undef FEATURE_DS_ORIG_IS707_SO       // bwpark 2000.02.16 ##define ---> #undef

/* Support choosing the set of service options used for originations.
*/
#define FEATURE_DS_CHOOSE_SO

/* Support Quick Net Connect originations
*/
#define FEATURE_DS_QNC

/* Support Answer Voice As Data
*/
#define FEATURE_DS_VOICE_AS_DATA

/* Enable Autobaud detection for rates between 1200 and 19200
*/
#define FEATURE_AUTOBAUD

/* Enable Call Manager support for Data Services
*/
#define FEATURE_DS_CM

/* Avoid a call origination for a network model packet call in dormant mode when the 
** MS receives PPP TERM_REQ from the RM interface.
*/
#undef FEATURE_DS_PPP_TERM_REQ_DROP 

//#if defined(FEATURE_SKT_DS) && !defined(SKT_DM_ESMS)	//	khekim 00/02/17
//#define	FEATURE_SKT_DR_DS	//	parsing at*skt*dr command by Qualcomm S/W
//#endif	//	#if defined(FEATURE_SKT_DS) && !defined(SKT_DM_ESMS)

#endif
/* FEATURE_DS
*/

/*---------------------------------------------------------------------------
                            Call Manager
---------------------------------------------------------------------------*/
/* Temporary feature for CM API changes
*/
#undef FEATURE_NEWCM

/* Enable CM Powerup-hold-origination feature to hold origination
** right after powerup giving service a chance to appear.
*/
#define FEATURE_CM_PWRUP_HOLD_ORIG

/* Enable CM Preferred-service-hold-origination feature to hold origination after
** powerup or mode preference change giving preferred service a chance
** to appears once sufficient service is available.
*/
#define FEATURE_CM_PREF_SRV_HOLD_ORIG

/* Enable CM Silent redial feature to silently redial failed calls or to ride through
** limited periods of no service.
*/
#define FEATURE_CM_SILENT_REDIAL


/*---------------------------------------------------------------------------
                         Short Messaging Service
---------------------------------------------------------------------------*/

/* Short Message Services (SMS)
*/
#define FEATURE_UASMS

/* Support new UASMS API changes. 
*/
#undef FEATURE_NEWUASMS

#ifdef FEATURE_UASMS

/* NEW_SMS defines the new buffer system for sms messages. This label is a
** temporary until uin is the mainline ui. At that time NEW_SMS ifdef and
** else's, should be deleted. Leaving only the ifdef code.
*/
#define NEW_SMS

/*  Broadcast Short Message Services (SMS)
*/
#define FEATURE_BROADCAST_SMS 

   /* For Mobile-Originated SMS message, the user can edit the destination
   ** address message text and the callback number.
   */
   #define FEATURE_COMPOSE_MO_SMS
#endif

/*---------------------------------------------------------------------------
                             User Interface
---------------------------------------------------------------------------*/

/* UI To Indicate on Display if Auto Answer is Enabled
*/
#undef  FEATURE_UI_AUTO_ANS_INDICATE

/* UI "case" screen to use "A->a" (otherwize "X->x")
*/
#undef  FEATURE_UI_CASE_USE_A

/* UI To Support Sending PIN when requested
*/
#undef  FEATURE_UI_SEND_PIN

/* Call Manager Exists
*/
#define FEATURE_CM

/* UI To Show "Dropped Call"
*/
#define FEATURE_UI_SHOW_DROP_CALL

/* UI Provides some Alternate Behaviors for the Power Key on Power On and
** Power Off.
*/
#define  FEATURE_UI_PWR_KEY_ALT

/* In alpha edit, the second line will prompt "Enter Name: "
*/
#undef  FEATURE_UI_ENTER_NAME_PROMPT

/* Save the redial number through a power-cycle
*/
#undef  FEATURE_UI_SAVE_REDIAL_BUFFER

/* When this feature is turned on, Old Markov Call is supported.
*/
#undef  FEATURE_UI_OLD_MARKOV

/* Envelope icon is used for voice mail also, just like text messages
*/
#undef  FEATURE_UI_ENVELOPE_VM_ICON

/* Default the IMSI to all 0's is okay
*/
#undef  FEATURE_UI_IMSI_ALL_0_OK

/* Voice Privacy Alert if the Service Alert is enabled.  The first entry
** into enhanced mode per call is ignored so that is doesn't beep in every
** call.  From then on beep every time it toggles.
*/
#undef  FEATURE_UI_PRIVACY_ALERT

/*  Shows preferred Roaming List Version
*/
#define FEATURE_UI_PRL_VER

/* handle forward DTMF
*/
#define FEATURE_HANDLE_FWD_DTMF

/* UI slotted mode support
*/
#define FEATURE_UI_DEBUG_SLOTTED_MODE

/* 8K Loopback or Markov
*/
#define FEATURE_UI_8K_SUPPORT

/* 8K Voice
*/
#define FEATURE_UI_8K_VOICE_SUPPORT

/*---------------------------------------------------------------------------
                            MSM3000 Features
---------------------------------------------------------------------------*/
/* TCXO Power Up
*/
#define PRODUCTION_SURF_REV1

/* UART power-down feature.
*/
#define FEATURE_UART_POWERDOWN

/* RF Warmup reduction from 42ms.
*/
#define FEATURE_FAST_WARMUP

/* EVRC service option supported
*/
#define FEATURE_EVRC

/* Panther Downloadable vocoder supported by vocoder driver
*/
#define FEATURE_VOC_PANTHER

/* Use 19.68MHz TCXO clock as vocoder clock for EVRC mode.
*/
#define FEATURE_TCXO_CLOCK

/* Enables the SBI to communicate with IFR/IFT3000
*/
#define FEATURE_SBI

/* Adjusts the freq tracking loop during access probes
*/
#define FEATURE_TX_ACCESS_POWER_FIX

/* Enable use of Low Frequency Sleep Oscillator
*/
#define FEATURE_SLEEP_SLOW_CLOCK

#ifdef FEATURE_SLEEP_SLOW_CLOCK
/* Selection to enable
     30 KHz -   60 KHz or  (default)
   1.92 MHz - 3.84 MHz range for sleep crystal
*/
#undef FEATURE_SLEEP_SLOW_CLOCK_1P92_3P84_MHZ

/* Enable TCXO Shutdown (for SURF)
*/
#define FEATURE_TCXO_SHUTDOWN

#endif /* FEATURE_SLEEP_SLOW_CLOCK */

/*---------------------------------------------------------------------------
                            MSM5000 Features
---------------------------------------------------------------------------*/
#ifdef T_MSM5000
  /* Enable UART clock to be TCXO not TCXO/4 */
  #define FEATURE_UART_TCXO_CLK_FREQ

  /*---------------------------------------------------------------------------
   MSM5000 software is based on CDMA-2000 standard.
  ---------------------------------------------------------------------------*/
  #define FEATURE_IS2000

 
  /*---------------------------------------------------------------------------
   This MSM5000 software is supporting MSM5000A and later version.
  ---------------------------------------------------------------------------*/
  #define T_MSM5000A_SUPPORT

  /*---------------------------------------------------------------------------
   Set various CPU and Wait State - related features
  ---------------------------------------------------------------------------*/
  #define FEATURE_CPU_CLK_13_5MZ //SKTT, by KJH

  #if !defined(FEATURE_CPU_CLK_13_5MZ) && !defined(FEATURE_ICE_DEBUG)
    /* run at 27MHz */
    #define FEATURE_CPU_CLK_27MHZ
    #define FEATURE_CPU_CLK_DIV1
  #endif
  
  /* MSM5000 supports turbo codes */
  #define MSMHW_TURBO_CODING
#endif

/*---------------------------------------------------------------------------
                           Serial I/O Features
---------------------------------------------------------------------------*/
/* Enable use of new SIO code.  New SIO code is more
** efficient, and supports two UARTs.
*/
#define FEATURE_NEW_SIO

/* Enables second UART.  Must define
** FEATURE_NEW_SIO for this to be effective.
*/
#define FEATURE_SECOND_UART

/* Enable use of the new Serial Device Mapping Code.
*/
#define FEATURE_SERIAL_DEVICE_MAPPER

#ifdef FEATURE_SERIAL_DEVICE_MAPPER

  /* Enables UI selection of port mapping the port map to be stored in
  ** NV and retrieved - requires power cycle to make changes take effect.
  ** These should be defined or undefined together.
  */
  #define FEATURE_SDEVMAP_UI_MENU
  #define FEATURE_SDEVMAP_MENU_ITEM_NV

  /* Enables the HFK to operate whenever the 2nd UART isn't open.
  ** Requires FEATURE_SECOND_UART to be defined also.
  */
  #ifdef FEATURE_SECOND_UART
    #define FEATURE_SDEVMAP_SHARE_HFK
  #endif

#endif  /* FEATURE_SERIAL_DEVICE_MAPPER */

/* Enables UI selection of DS baud rate and the DS baud to be stored in
** and retrieved from NV.  These should be defined or undefined together.
*/
#define FEATURE_DS_UI_BAUD
#define FEATURE_DS_DEFAULT_BITRATE_NV

/* Enables UI selection of DIAG baud rate and the DIAG baud to be stored in
** and retrieved from NV.  These should be defined or undefined together.
*/
#define FEATURE_DIAG_UI_BAUD
#define FEATURE_DIAG_DEFAULT_BITRATE_NV

/* Enable correct UART updates based on FIFO size.
** Must not undefine for MSM5000
*/
#define MSMHW_UART1_512BYTE_FIFOS

/* Features under development */
#undef FEATURE_SIO_PNP

#undef FEATURE_SIO_DTR_HIGH_GPIO

#undef FEATURE_SIO_DTR_ACTIVE_LOW

/*---------------------------------------------------------------------------
                            IS-95B/MDR definition
---------------------------------------------------------------------------*/
#define FEATURE_IS95B

#ifdef FEATURE_IS95B

  /*-----------------------------------------------------------*/
  /* Enable the individual IS-95B Protocol Revision 4 features */
  /*-----------------------------------------------------------*/

  /* IS95B compliant version of True IMSI
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_TRUE_IMSI

  /* IS95B compliant version of Open Loop Power Correction Estimation
  ** This is included only if FEATURE_IS95B is included
  */
//kjh TEST only, Request of RF  #define FEATURE_IS95B_OLPEC

  /* IS95B compliant version of System Reselection
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_SYS_RESELEC

  /* IS95B compliant version of New Soft Handoff Algorithm
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_SOFT_HO

  /* IS95B compliant version of Calling NAme Presentation (CNAP)
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_EXT_DISP

  /* IS95B compliant version of Access Handoff
  ** This is included only if FEATURE_IS95B is included
  */
//kjh TEST only  #define FEATURE_IS95B_ACCESS_HO

  /* IS95B compliant version of Pilot Reporting
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_PILOT_RPT

  /* IS95B compliant version of Neighbor Search Enhancements
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_NGHBR_SRCH_ENH

  /* IS95B compliant version of the Release Order on Access channel
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_RELEASE_ON_AC

  /* IS95B compliant version of the Periodic Pilot Strength Measurement Msg
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_PPSMM

  /* IS95B compliant version of Network Directed System Selection (NDSS)
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_NDSS

  /* IS95B compliant version of the Incoming Call Forwarding Enhancement
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_INC_CALL_FWD_ENH

  /* IS95B compliant version of the Alert Answer Bypass Feature
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_ALERT_ANS_BYPASS

  /*-----------------------------------------------------------*/
  /* Enable the individual IS-95B Protocol Revision 5 features */
  /* These are optional for Revsion 4                          */
  /*-----------------------------------------------------------*/

  /* IS95B compliant version of Mobile Assisted Hard Handoff feature.
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_IS95B_MAHHO

  /*-----------------------------------------------------------*/
  /* Enable the optional IS-95B Protocol Revision features     */
  /*-----------------------------------------------------------*/

  /* IS95B compliant version of High Speed Packet Data feature (MDR).
  ** This is included only if FEATURE_IS95B is included.  It also
  ** requires that FEATURE_DS be defined.
  */
  #ifdef FEATURE_DS
    #define FEATURE_IS95B_MDR
  #endif

  /* IS95B compliant version of Access Entry Handoff
  ** This is included only if FEATURE_IS95B is included
  */
//kjh TEST only  #define FEATURE_IS95B_ACCESS_ENTRY_HO

  /* IS95B compliant version of Access Probe Handoff
  ** This is included only if FEATURE_IS95B is included
  */
//kjh TEST only  #define FEATURE_IS95B_ACCESS_PROBE_HO

  /* IS95B compliant version of reducing the Access Probe Backoff delay.
  ** This is included only if FEATURE_IS95B is included
  */
//kjh TEST only  #define FEATURE_IS95B_ACC_PRB_BKOFF_DLY

  /*-----------------------------------------------------------*/
  /* End of IS-95B Feature Definitions                         */
  /*-----------------------------------------------------------*/


#endif /* FEATURE_IS95B */

/* MDR specific features. These are included only
** if FEATURE_IS95B_MDR is included
*/
#if defined(FEATURE_IS95B_MDR)
  /* MDR Data Pattern Test feature. This is an internal test feature
  */
  #undef FEATURE_MDR_DPT_MARKOV

  /* Sockets feature to restrict socket calls to LSPD service options
  */
#if defined(FEATURE_DS_SOCKETS)
  #define FEATURE_DS_SOCKETS_USE_LSPD
#endif

/* Support for the IS-707-A format of the AT+CMUX command. This command
** sets the maximum forward & reverse multiplex options.
*/
#ifdef FEATURE_DS
  #define FEATURE_DS_IS707A_CMUX
#endif

/*On a supplemental channel, where no real data is being transmitted
 ** support detection and elimination of random full rate frame that
 ** pass the CRC check
*/
#undef FEATURE_DETECT_SUP_MUTING

#endif /* FEATURE_IS95B_MDR */

/* Feature to enable some test code to demonstrate how Packet Zone Id change
** handler is used.  This feature should be turned off by the customers.
*/
#define FEATURE_PZID_CHANGE_EXAMPLE

/*---------------------------------------------------------------------------
                            Standby Enhancement Features
---------------------------------------------------------------------------*/
/* decrease the number of idle hand-offs requiring registration by
** increasing the hand-off threshold.
*/
#define FEATURE_ANTIREGISTRATION_BIAS

/* increases standby time  and decreases MER.
*/
#define FEATURE_ENHANCED_STANDBY_II

/* increases standby time
*/
#define FEATURE_ENHANCED_STANDBY_III

/* Optimizes RF circutry power up on key presses
*/
#define FEATURE_KEYPRESS_OPTIMIZATION

/* improves searcher acquisition times
*/
#define FEATURE_QUICK_SCAN

/*  Messages for MER testing
*/
#define FEATURE_STANDBY_DEBUG_MESSAGES

/* must be defined if "virtual boot block" has not been implemented.
*/
#undef FEATURE_NO_VBB

/* must be defined if the tcxo shutdown "voting" scheme has not been implemented
*/
#define FEATURE_NO_TCXO_VOTE

/* only define this if extended linearizer support exists in the RF module.
*/
#undef FEATURE_EXT_LINEARIZER

/*  Enables the 5ms callback to call srch_finger_lock_handler() when in a traffic channel
**  Note needed for Tiger 2 because the problem will be fixed in hardware
*/
#undef FEATURE_FINGER_POLL

/* The following feature helps prevent multiple fingers from
** being on the same path 
*/
#define FEATURE_FING_MERGE_DETECT

/* 16X Search Features 
*/
#define FEATURE_SEARCH2
#define FEATURE_SEARCH2_IDLE
#define FEATURE_SEARCH2_ACQ

/* Allows masking of the searcher instead of slewing to the absolute position.  
*/	
#define FEATURE_RELATIVE_DMOD_POS
	
/*---------------------------------------------------------------------------
                            FEATURE MINIMIZE ISR LOADING
---------------------------------------------------------------------------*/
/* This feature changes the rxc/txc task's way of processing traffic channel
** bit streams. The changes are centered around the viterbi decoder ISR and
** convolutional encoder isr.
*/
#define FEATURE_MINIMIZE_ISR_LOADING

/*---------------------------------------------------------------------------
                            QUEUE ENHANCEMENTS
---------------------------------------------------------------------------*/

/* Removes the reference of self pointer and queue pointer in Queue elements.
** Saves 8 Bytes of RAM per queue element.
*/
#define FEATURE_Q_NO_SELF_QPTR

/*   Links queue elements as Singly-link list. Saves 4 Bytes of RAM per queue element.
*/
#define FEATURE_Q_SINGLE_LINK


/*---------------------------------------------------------------------------
                            CDMA2000
---------------------------------------------------------------------------*/
#ifdef FEATURE_IS2000

  /* Fast Forward Power Control */
  #define FEATURE_FFPC

  /* New action time processing */
  #define FEATURE_RXC_ACTION_TIME_PROCESSING

  /* Quick Paging Channel */
#define FEATURE_IS2000_QPCH

  /* CCI bits */
  #define FEATURE_IS2000_QPCH_CCI

  /* Reverse Link Gain Control */
  #define FEATURE_RLGC
  
  /* LTU CRC checking for conv. coded SCH.  Can heavily load CPU */
  #define FEATURE_IS2000_LTU_CRCS

  #ifdef FEATURE_DS
    #define FEATURE_DS_IS2000
    #define FEATURE_DS_RLP3

    #ifdef FEATURE_DS_IS2000
	
      //Supplemental Channel
      #define FEATURE_IS2000_SCH
	  
      #ifdef FEATURE_IS2000_SCH
      // Forward supplemental channel
      #define FEATURE_IS2000_F_SCH
	  
      // Reverse supplemental channel
      #define FEATURE_IS2000_R_SCH

      #ifdef FEATURE_IS2000_R_SCH
         #define FEATURE_DATA_TIMELINE_ADVANCE
      #endif

      // Supplemental Channel MUX statistics
      #define FEATURE_IS2000_SCH_STATS
    #endif

    #endif

    #define FEATURE_RLP_LOGGING
  #endif
  
  /* Test Data Service Option Support
  */
#ifdef	FEATURE_SKT_DS	//	khekim 00/02/17
  #undef FEATURE_IS2000_TDSO
#else	//		#ifdef	FEATURE_SKT_DS
  #define FEATURE_IS2000_TDSO
#endif	//		#ifdef	FEATURE_SKT_DS

  /* Bias triage towards pilots with supplemental channels
  */
  #define FEATURE_SCH_TRIAGE

  /* Max Power Protection on the SCH 
  */
  #define FEATURE_MAX_PWR_PROTECTION

#endif // FEATURE_IS2000

/* Enables management of all service options by SNM */
#define FEATURE_COMPLETE_SNM


#ifdef	FEATURE_SKT_DS	//	khekim 00/02/17
// Feature enabling dial-up pkt data service (with dormant, at command-enabled function)
#define	FEATURE_SKT_ATPKT_DS	//	khekim 00/07/26

//  Feature related to UART RESETERR. if there is no this feature, phone may be reset in sleep mode
	//#define	FEATURE_SKT_UART_RESETERR	//	khekim 00/03/23
//  Feature related to UART POWERDOWN
	//#define	FEATURE_SKT_UART_POWERDOWN	//	00/03/17
// Feature related to Rx Auto hardware flow control
	//#define	SIORS232_RX_AUTO_HRDWR_FCTL
	//#define	FEATURE_SKT_RXAUTOHRDCTL_DS
// Feature related to using DTR as GPIO 26 instead of GPIO 4
	//#define	FEATURE_USE_DTR_AS_GPIO26
// Feature related to data manager interface
	#define	FEATURE_SKT_DMIF_DS

// Feature related to Cellular Result Code
	#define	FEATURE_SKT_DS_CELLRESULT

// khekim 00/02/24 전화접속 네트워킹 프로그램 종료의 성능 향상을 위한 Feature
// Packet 호 종료를 Async&Fax 호 종료처럼 수정함
#define	FEATURE_SKT_DTRSENSE_DS		//	khekim 00/02/22
#define	FEATURE_SKT_PKTDTRSENSE_DS	// khekim 00/02/24

// Feature related to Data Service Redial
#ifndef	FEATURE_DS_CM	//	khekim 00/02/22
#define	FEATURE_SKT_DS_REDIAL
#endif	//	#ifndef	FEATURE_DS_CM	//	khekim 00/02/22

// Feature related to RLP(mdrrlp.c)
	#define	FEATURE_SKT_RLPBUG_DS	//	for resolving mismatch with system rlp(9.6K, 14.4K)
	//#define	FEATURE_SKT_VARIABLE_RATE_DS	//	support rlp variable rate
	#define FEATURE_SKT_MDR_RLP		//  for resolving mismatch with system rlp(64 K)

// Feature related to LDR
#define	FEATURE_SKT_PIWF_DS
#define	FEATURE_SKT_TCP_DS
	/* SK Telecom Service Negotiation Mode for 8K-only base stations:
	** If the mobile station receives a Channel Assignment msg
	** with ASSIGN_MODE = '7' then begin negotiation with an
	** 8K Data call instead of the default 13K Data call.
	*/
#define FEATURE_SKT_ASSIGN_MODE

	// Feature related to MDR
		/* Update BS Protocol Revision for MDR.
		*/
		//#define FEATURE_SKT_P_REV

//	Feature related to SKT MMI
	#define	FEATURE_SKT_DS_MMI
	#ifdef	FEATURE_SKT_DS_MMI	//	khekim 00/04/04
	// Feature related to processing no service softly when data orignate right after power-up animation
		#define	FEATURE_SKT_NOSVC_DS
	// Feature related to Stop animation during data origination right after power-up
		#define	FEATURE_SKT_STOPANI_DS		//	khekim 00/02/27
	// Feature realted to Data Receiving Banner Display
		#define	FEATURE_SKT_DISPBANNER_DS	//	khekim 00/02/27
	#endif	//	#ifdef	FEATURE_SKT_DS_MMI

	// FEATURE_SKT_DS_LDR
//	#define	FEATURE_SKT_DS_LDR
//	#define	FEATURE_SKT_DEBUG
//	#define	FEATURE_SKT_RLPDUMP_DS	//	Data Service: RLP DM
//	#define	FEATURE_SKT_RLP3DUMP_DS	//	Data Service: RLP3 DM
#endif	//	#ifdef	FEATURE_SKT_DS	//	khekim 00/02/17

/* jrkwon 2000-04-10 --> */
//#define FEATURE_NUI_D1
/* <-- */
//#define FEATURE_NUI_B1

/* Enables management of all service options by SNM */
#define FEATURE_COMPLETE_SNM

#endif /* CUSTSURF_H */
