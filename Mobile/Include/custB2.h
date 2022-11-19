#ifndef CUSTB2_H
#define CUSTB2_H
/*===========================================================================

            " M S M 5 0 0 0 -  S U R F "   H E A D E R   F I L E

DESCRIPTION
  Configuration for SURF Target using MSM5000 ARM Processor.
  This is a Cellular build with IS95B, NV Item Manager and 
  Embedded File System.

  Copyright (c) 1999, 2000 by QUALCOMM Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/cust4sce.h_v   1.18   11 Apr 2000 21:57:48   knordin  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/07/00   kmh     RF_HAS_TEMP_COMP definition 삭제함.
                   RF 관련 Definition을 rficap.h에서 옮겨옴
				   FEATURE_RF_PDM2_VNEGA 추가

04/11/00   kjn	   Moved all the FEATURE defines which are common to all builds to
				   custsurf.h.
04/05/00   kjn	   Undefined FEATURE_DETECT_SUP_MUTING.
04/05/00   kjn	   Added & defined FEATURE_OTASP_HOLD_CMD_IN_COMMIT,
				   FEATURE_UI_8K_SUPPORT, FEATURE_UI_8K_VOICE_SUPPORT.
04/03/00   va      Defined FEATURE_SRCH_DYNAMIC_BAND_CLASS feature
03/27/00   va      defined the following:
                   FEATURE_DETECT_SUP_NUTING
                   FEATURE_UI_DEBUG_SLOTTED_MODE
                   FEATURE_SEARCH2 (16x search)
                   FEATURE_SEARCH2_ACQ (16x search)
                   FEATURE_SEARCH2_IDLE (16x search)
                   FEATURE_UART_TCXO_CLK_FREQ 
03/07/00   kjn     Added FEATURE_DIAG_DEFAULT_BITRATE_38400. 
02/29/00   kjn     Added FEATURE_DIAG_V7_LOG_IFACE, FEATURE_DIAG_NON_STREAMING, 
					FEATURE_DIAG_NO_EVENTS. 
02/23/00    eh     Added T_MSM5000A_SUPPORT
02/07/00   va      Added FEATURE_VOC_SAT_CONTROL, RXC_ACTION_TIME_PROCESSING.
                   Removed FEATURE_INT_25_FIQ
01/19/00   lcc     Removed FEATURE_SPECIAL_MDR which is not supported in this
                   archive.  Removed TEMP_HACKS_FOR_FOR_TIGER.
                   Also undefined features not for this release.
11/03/99   lcc     Deleted FEATURE_VIRTUAL_COMBINER which is mainlined.
                   Added and defined FEATURE_SERIAL_DEVICE_MAPPER.
11/02/99   lcc     Added and defined FEATURE_IS95B_ACC_PRB_BKOFF_DLY.
10/20/99   lcc     Added block for IS2000 features.  Enabled MDR.
10/07/99   lcc     Initial release based on cust4sc.h from Panther archive.

===========================================================================*/


#include "custsurf.h"


/* 
** jrkwon 2001.07.06 --> definition for RELEASE 
**
** If SKTT_RELEASE is defined,
** SW_RESET_ENABLE is enable
** and when some errors are occurred,
** just return from the function instead of ASSERT.
*/
//#define SKTT_RELEASE

#ifdef FEATURE_ADS_COMPILER
/* Enable IRAM
*/
#define FEATURE_IRAM
#endif

/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
                            General Interest
---------------------------------------------------------------------------*/

/* Customer's Over the Air Model Number.  Note that these must be carefully
** assigned and that QualComm only has a few assigned to it.
*/
#define  CUST_MOB_MODEL 29 //48

/* Put 8 bits and 16 bits shadow registers in one array.
*/
#define FEATURE_16BIT_SHADOW_REG

#ifdef SKTT_RELEASE
#define SW_RESET_ENABLE
#endif

/*---------------------------------------------------------------------------
                            Call Processing
---------------------------------------------------------------------------*/

/* Analog Call Processing
*/
//kjh #define FEATURE_ACP
#undef FEATURE_ACP

/*  FM sleep improvements
*/
//kjh #define FEATURE_FM_SLOTTED
#undef FEATURE_FM_SLOTTED

/* FM slotted debug information
*/
#undef FM_SLOTTED_DEBUG

/* Supports CDMA at 800MHz
*/
#define FEATURE_CDMA_800

/* This feature allows the use of a roaming list as defined in IS-683A.
** SSPR_800 can operate without a roaming list, in which case it behaves similar
** to the old algorithm, but not exactly the same.
*/
#define FEATURE_SSPR_800

/* Feature to prevent roam side available systems from being acquired
*/
#define FEATURE_AVAIL_HOME_SIDE_PREF


/* needs to be turned on to enable all the functionality needed for FEATURE_SSPR_800.
*/
#define  FEATURE_SSPR_ENHANCEMENTS

/* These features need to be turned on to enable all the functionality needed for
** FEATURE_OTASP.
*/
#define  FEATURE_PRL_FORCE_MODE




/*---------------------------------------------------------------------------
                            Standby Enhancement Features
---------------------------------------------------------------------------*/

//by jnlee 01.04.02
#undef FEATURE_ENHANCED_STANDBY_III	// QA요청에 의함 (sleep시 폴더 처리 느림 방지)


/*---------------------------------------------------------------------------
								SK Teletech Features (Protocol)
---------------------------------------------------------------------------*/
/* Improves call processing.
*/
#define FEATURE_SKT_CAI

#define FEATURE_SKT_NDSS

#define FEATURE_SKT_NDSS_REDIAL

#define FEATURE_SKT_MRU_SPEC

/* SK Telecom's Default PRL Data Load
*/
#define FEATURE_SKT_PRL

#ifndef FEATURE_SKT_DEBUG
#define FEATURE_SKT_DEBUG //debug screen, SKTT DM
#endif

/* Needs to process CNAP service */
#define FEATURE_IS95B_EXT_DISP


/*---------------------------------------------------------------------------
                             User Interface
---------------------------------------------------------------------------*/

/* UI To Support "Force Call" in its.
*/
//kjh #define  FEATURE_UI_FORCE_CALL
//kjh #define  FEATURE_UI_FORCE_MODE_SEL
#undef  FEATURE_UI_FORCE_CALL
#undef  FEATURE_UI_FORCE_MODE_SEL


/*---------------------------------------------------------------------------
                          Non-Volatile Memory
---------------------------------------------------------------------------*/

/* Use NV Item Manager with Embedded file system.
*/
#define FEATURE_NV_ITEM_MGR

/* Embedded file system.
*/
#define FEATURE_EFS
#define FEATURE_EFS_FLASH

/* Use flash on SURF for the embedded file system.
*/
#define FEATURE_SURF_FLASH

/* Use 5 NAMs SKTT
*/
#define FEATURE_NAM_5   

/*---------------------------------------------------------------------------
                            MSM5000 Features
---------------------------------------------------------------------------*/

/* Enable all SAT writes under vocoder control */
#define FEATURE_VOC_SAT_CONTROL
#define SND_PARA_SUPPORT

/*---------------------------------------------------------------------------
                            P_REV definition
---------------------------------------------------------------------------*/

#ifdef FEATURE_IS2000
  /* MOB_P_REV definition needed for run-time P_REV checking
  ** It is defined as 6 for IS-95B compliant mobiles
  */

#ifdef FEATURE_SKTT95
#define MOB_P_REV 4
#else
#define MOB_P_REV 6
#endif
#elif defined( FEATURE_IS95B)
  /* MOB_P_REV definition needed for run-time P_REV checking
  ** It is defined as 4 for IS-95B compliant mobiles
  */
#define MOB_P_REV 4
#else
  /* MOB_P_REV definition needed for run-time P_REV checking
  ** It is defined as 3 to assist the P_REV1+ solution for MDR
  */
#define MOB_P_REV 3
#endif

/*---------------------------------------------------------------------------
                            IS-95B definition
---------------------------------------------------------------------------*/
#ifdef FEATURE_IS95B

  /* TMSI support is required for IS-95B Band Class 0
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_TMSI

  #else

  /* Disable TMSI since it is not valid for Band Class 0 P_REV = 3 mobiles.
  */
  #undef FEATURE_TMSI

#endif /* FEATURE_IS95B */

// kmh insert for Calibration 00.06.13 start
/*---------------------------------------------------------------------------
                       SKTT RF & Calibration definition
---------------------------------------------------------------------------*/
#define FEATURE_SKTT_CAL		// SKTT Calibration scheme
#define RF_HAS_LNA_GAIN			// LNA Gain Control

#define CAGC_LNA_GN_POL_V CAGC_LNA_GN_POL_INV_V
#undef FEATURE_RF_PDM2_VNEGA		// PDM2 -> VNegative 전원 조정
#undef FEATURE_RF_PDM1_VCACNT		// PDM1 -> Vcacnt 조절 
#define RF_HAS_PA_RN				// PA_RANGE 사용 00.08.29 by jnlee
// kmh insert for Calibration 00.06.13 end

// by jnlee 00.08.28	TX range 적용 (PA3100사용)
#define FEATURE_4PAGE_TX_LINEARIZER
#define FEATURE_PA_RANGE_TEMP_FREQ_COMP
#define FEATURE_PA_STATE_SYNC	// jnlee 00.11.21
// end. by jnlee

//#define FEATURE_YMU757B_MELODY	// jwpark 00.07.25
#define FEATURE_YMU759_MELODY		// jnlee  00.08.07
//#define FEATURE_SMART_BATTERY		// youandi00.12.29 

/*---------------------------------------------------------------------------
  Set Vocoder Capability to EVRC                             8k 13k 
---------------------------------------------------------------------------*/

#ifndef WIN32 /* FEATURE_SKTT_PM defined in Project Settings in Win32 */
#define FEATURE_SKT_EIF    // redstar   2001.02.05
#endif

/*---------------------------------------------------------------------------
                             Photo Mail
---------------------------------------------------------------------------*/
#ifndef WIN32 /* FEATURE_SKTT_PM defined in Project Settings in Win32 */
#define FEATURE_SKTT_PM
#endif

/* rosa 01-02-02 --> @why, what */
#define FEATURE_GVM
//#define FEATURE_IP_AIRDATA
//#ifndef	FEATURE_SKT_AIRDATA
//	#define	FEATURE_SKT_AIRDATA
//#endif	//	#ifndef	FEATURE_SKT_AIRDATA

#ifndef WIN32 /* FEATURE_SKTT_PM defined in Project Settings in Win32 */
#define FEATURE_SKVM
#endif
#ifdef FEATURE_SKVM
	#define COLOR_8BIT                   // by tortol color 값 보정. 만일 B2에서는 color_16bit으로 수정
    #define SKVM_IN_UI_TASK              
	#define SUPPORT_ANNUNCIATORS_IN_SKVM
	#define SUPPORT_DORMANT_IN_SKVM
	//#define SUPPORT_REX_TIMER_IN_SKVM
	//#define SKVM_USE_SLEEP_TIMER_IN_NETWORK
	//#define SUPPORT_SET_SKVM_IN_BG

#endif 

#if defined(FEATURE_SKT_AIRDATA) || defined(FEATURE_GVM) || defined(FEATURE_AIRP2P)
#define	FEATURE_SKT_PTPAPI
#endif	//	#if defined(FEATURE_SKT_AIRDATA) || defined(FEATURE_GVM) || defined(FEATURE_AIRP2P)

#if defined(FEATURE_IP_AIRDATA) || defined(FEATURE_GVM) || defined(FEATURE_WAVELET)
#define FEATURE_SKT_SOCKAPI
#endif	// #if defined(FEATURE_IP_AIRDATA) || defined(FEATURE_GVM) || defined(FEATURE_WAVELET)

#if defined(FEATURE_GVM) || defined(FEATURE_AIRP2P)
#define	FEATURE_SKT_PIWFPTPAPI	//	khekim 00/08/29
#endif	//	#if defined(FEATURE_GVM) || defined(FEATURE_AIRP2P)

#if defined(FEATURE_GVM)
//	Feature enabling Serial port in Browser, socket, AirData Service
#define FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
// WAP=>GVM online으로 이동
// #define FEATURE_SKT_GVM_ONLINE  //  khekim 01/04/13 <- closed by undersea 01/09/xx
#endif	//	#if defined(FEATURE_GVM)
/* <-- */



/*---------------------------------------------------------------------------
                             Call Stack     added by  hjcho 01.05.29
---------------------------------------------------------------------------*/

/* 착/발신 이력 분리를 위해 추가                                           */
/* 외부 키 매핑 변경                                                       */
/* 기존 :                                                                  */
/*  Up-Key: 착/발신이력 Down-Key:카메라 Left-Key:스케줄러 Right-Key:전화부 */
/* 변경안:                                                                 */
/* Up-Key:발신이력   Down-Key:착신이력  Left-Key:메뉴  Right-Key:전화부    */
#define FEATURE_CALL_STACK_SEPERATED



/*---------------------------------------------------------------------------
                             Voice Memo
---------------------------------------------------------------------------*/
#define TREX_VOICE_APP
#define TREX_VOICE_MENU
#define FEATURE_HOLDON
#define FEATURE_HALF_DUPLEX_RECORD
#define FEATURE_VOICEPLAY
/*---------------------------------------------------------------------------
                            SMS             added by reo 01
---------------------------------------------------------------------------*/
#define FEATURE_UASMS_SAVE_MEM
#define FEATURE_SKTT_EQUS_SMS
#define FEATURE_SKTT_EQUS_LOCATION
/*---------------------------------------------------------------------------
                These features are added by hjcho 00.12.19
---------------------------------------------------------------------------*/

/* swjung 2001-07-20 --> @why, what */
#define FEATURE_MCOMMERCE
/* <-- */


// WAP관련 FEATURE
/* Don't Use WAPPER FEATURE
#ifdef WAPPER
	//#define EIF_WAP		// zeus(yjchoi) 00.12.05 for EIF WAP (External Interface WAP)
	#define WAP_NOWRAP	//yjahn96 00.10.30 for Wap MMI(scroll, no wrap)
	#define FEATURE_DS_BROWSER_INTERFACE
	#define newsrc
	#ifdef FEATURE_DS_BROWSER_INTERFACE
		#define jjkyg
		#undef  dns
		#define jjkygtest
		#define DNSIP
		#undef  CLPORT
		#define USE_DNS
		#define FOR_MS
		#define FEATURE_DS_PAP
	#endif//define FEATURE_DS_BROWSER_INTERFACE
	#define FEATURE_MUSIC_BELL	//yjahn96 ----- musicbell
    //#define FEATURE_CHARACBELL  // bgjung (character bell)
    #define FEATURE_CHARACTER_BELL  // bgjung (캐릭벨)
	#define WAP_SETURL
	#define FEATURE_WAPMAIL 
	#define INCOMING_CALL_IN_WAP
    #define FEATURE_PHONEBOOK_VER12
	#define INCOMING_SMS_IN_WAP
	#define WAP_USE_GOBACK_ERROR
	#define WAP95C_TEST
    #define WAP_INIT_ORIG
    //#define FEATURE_MLZ // bgjung (나만의 그림 MLZ Codec)
    //#define WAP_USE_INIT
	//#define WAP_HOMEPAGE
	//#define WAP_ERROR_TEST
#endif //#ifdef WAPPER */

/* AU WAP4.3 FEATURE */

#ifdef FEATURE_WAP
	#define DNSIP
	#define WAP_SETURL
	#define FEATURE_WAPMAIL
    #define FEATURE_MUSIC_BELL   
    //#define FEATURE_CHARACBELL  // bgjung (character bell)
    #define FEATURE_CHARACTER_BELL  // bgjung (캐릭벨)
    #define WAP_NOWRAP	//yjahn96 00.10.30 for Wap MMI(scroll, no wrap)    
    #define FEATURE_WAP408    
    #define FEATURE_WAP_WAVELET
    #define INCOMING_CALL_IN_WAP
    #define FEATURE_WAP_MUXOPTION
    #define FEATURE_PHONEBOOK_VER12
    #define FEATURE_MLZ // bgjung (나만의 그림 MLZ Codec)
#endif //FEATURE_WAP

//#define FEATURE_STI_DS

#ifdef	USE_IRDA
	#define	FEATURE_USE_IRDA
	#ifdef	FEATURE_USE_IRDA
		#define FEATURE_IRDA_UPGRADE
        #define FEATURE_IRDA_HIGH_RATE
        #define FEATURE_IRDA_SIGNAL // woodstok 01.5.2
	#endif	//#ifdef	FEATURE_USE_IRDA
#endif	//#ifdef	USE_IRDA

/*---------------------------------------------------------------------------
								SK Teletech Features (Data Service)
---------------------------------------------------------------------------*/
/* jrkwon 2000-08-25 --> for building B1 */
#undef FEATURE_BROADCAST_SMS
//#define DS_BROWSER_PKT_SRVC_MODE    1
/* <-- */

#ifdef	FEATURE_SKT_DS	//	khekim 00/11/23
// Display Dormant State of RM Interface on LCD of UI task
#define FEATURE_UI_DORMANT
// Enhance Dormant Function of RM Interface
#define	FEATURE_RM_DORMANT

//#ifdef	FEATURE_DS_IS2000
// Set the default value of AT+CMUX to 1 instead of 2 in IS-95C
//#define	FEATURE_SKT_95C_DS
//#endif	//	#ifdef	FEATURE_DS_IS2000

// Use DNS API of Socket Library released by Qualcomm
#define	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
// Process something(Dial String, SO, Auth Uesrname and so on) regarding Packet Data Service of E-mail, Netgame and IP AirData in IS-95A or IS-95C
#define	FEATURE_SKT_SOCK_SUP_95AC
	// Reset IP Address to 0.0.0.0 when PPP renego. after PZID is changed in E-mail, Netgame, IP Airdata
//	#define	FEATURE_SKT_SIMPLEIP
// If Any Data from Serial Interface is entered before connecting, Call is dropped.
//#define	FEATURE_SKT_CANCEL_CALL
	
	#undef	FEATURE_SKT_PKTDTRSENSE_DS	//	khekim 00/11/08
//	Process system-initiated dormant state irrevalent to the value of AT+CTA 
#define	FEATURE_SKT_ATCTA_DS	// khekim 00/12/16

// Set Alternative Service Option used for Pkt Data Service Negotiation and Support AT+CRM=150(SO=33) in IS-95A/B
#define FEATURE_SKT_ALT_SO

// Add In-Traffic Muliplex Option Nego in IS-95C
#ifdef	FEATURE_DS_IS2000
#define	FEATURE_SKT_INTRAFFIC_MONEGO
#endif	//	#ifdef	FEATURE_DS_IS2000

// NDSS에 의해서 호 설정 및 PPP username/password 수정
#ifdef	FEATURE_DS_IS2000
#define FEATURE_SKT_NDSS_DS
#endif  //  #ifdef	FEATURE_DS_IS2000
#endif	//	#ifdef	FEATURE_SKT_DS

#define FEATURE_COLOR_ID_LCD        //white 01.02.14
#define FEATURE_DSM_DIAG_ITEMS

//Added by happykid in 01.03.05
#define	FEATURE_SKTT_NEW_CAL
//khekim 01.08.01::Socket서비스직후 다음 Packet서비스시 Dormant 진입안되는 버그 수정
#define FEATURE_SKT_IDLETMR_DS 
#define FEATURE_IP_AIRDATA          //white 2001.03.21    
#define FEATURE_SKT_AIRDATA         //white 2001.03.21    

#undef FEATURE_COMPLETE_SNM

#define FEATURE_PARTIAL_DISPLAY
#define FEATURE_64KCOLOR_LCD

#define FEATURE_LCD_RESET			//asdf 2001.10.05

//#define	FEATURE_GP_TIMER_AUTO_KICK

#define SKTT_XSCRIPT			// by jnlee 01.11.14

#endif /* CUSTB2_H */

