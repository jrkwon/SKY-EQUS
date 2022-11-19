#ifndef DIAGT_H
#define DIAGT_H
/*===========================================================================

          D I A G N O S T I C   D I R E C T E D   T E S T   
  
                        H E A D E R   F I L E


DESCRIPTION

  This header file defines the constants and structures used by the
  diag unit and the diagnostic monitor to exchange commands and information
  regarding directed tests.  A directed test is one which is commanded by
  the diagnostic monitor.

Copyright (c) 1992-2000 by QUALCOMM, Incorporated. All Rights Reserved.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/diagt.h_v   1.0   21 Sep 2000 19:00:52   ldefauw  $

when       who   what, where, why
--------   ---   ------------------------------------------------------------
06/13/00   kmh   Added support for SKTT Calibration
--------   ---   ------------------------------------------------------------
02/02/00   lad   Cleanup and scanner definitions.
08/19/99   sk    ARM 2.50 changes.
06/17/99   lad   Fixes for TMOBILE ARM.
03/06/98   rdh   Added support for Pilot Scanner feature testing.
11/04/98   lad   Made necessary changes for ARM port.
11/01/94   gb    Sector Lock test functionality added.
05/28/94   jmk   Added field & struct to tmob for running numbered test from DM.
11/03/92   twp   Vocoder PCM & PKT loopback,  analog IS-55 tests
09/04/92   twp   Initial version.

===========================================================================*/


/*===========================================================================

                            INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "target.h"
#include "qw.h"


/*===========================================================================

                             DECLARATIONS

===========================================================================*/

/* Directed test commands */

typedef enum {
  DT_CARRIER_ON_F,            /* Carrier on command                       */
  DT_CARRIER_OFF_F,           /* Carrier off command                      */
  DT_LOAD_SYNTH_F,            /* Load Synthesizer command                 */
  DT_SET_ATTN_F,              /* Set Attenuation command                  */
  DT_RXMUTE_F,                /* Mute the receiver audio signal           */
  DT_RXUNMUTE_F,              /* Unmute the receier audio signal          */
  DT_TXMUTE_F,                /* Mute the transmit audio signal           */
  DT_TXUNMUTE_F,              /* Unmute the transmit audio signal         */
  DT_ST_ON_F,                 /* Transmit continuous Signalling Tone      */
  DT_ST_OFF_F,                /* Stop transmission of Signalling Tone     */
  DT_SEND_NAM_F,              /* Send the specified NAM                   */
  DT_SWITCH_NAM_F,            /* Change to the specified NAM              */
  DT_SEND_MIN_F,              /* Send the specified MIN                   */
  DT_VERSION_F,               /* Send software version                    */
  DT_SEND_ESN_F,              /* Send the esn                             */
  DT_RCVS1_F,                 /* Count msgs on Analog Control Channel     */
  DT_WSTS_F,                  /* Count word syncs on Analog Control 
                                 Channel                                  */
  DT_SAT_ON_F,                /* Enable transmission of SAT               */
  DT_SAT_OFF_F,               /* Disable transmission of SAT              */
  DT_TERMINATE_F,             /* Stop RCVS1 or WSTS and report counts     */
  DT_CDATA_F,                 /* Transmit continuous 5-word Reverse 
                                 Control Channel messages                 */
  DT_DTMF_ON_F,               /* Activate the DTMF generator with 
                                 specified tones                          */
  DT_DTMF_OFF_F,              /* Stop the DTMF                            */
  DT_COMPANDER_ON_F,          /* Enable the compander                     */
  DT_COMPANDER_OFF_F,         /* Disable the compander                    */
  DT_RSSI_F,                  /* Return the current RSSI measurement      */

//*****************************************************************************
#ifdef FEATURE_SKTT_CAL		// kmh insert for calibration 00.06.13
//*****************************************************************************

  DT_CTESTSTART_F = 0x50,     /* CDMA Test Start                          */
  DT_RXAGC_INIT_F,            /* 0x51 CDMA Rx AGC Calibration Init Data Load   */
  DT_RXAGC_CAL_F,             /* 0x52 CDMA Rx AGC Calibration                  */
  DT_RXAGC_LOAD_F,            /* 0x53 Save to NV and Load to Rx AGC RAS RAM    */

  DT_RXLNA_CAL_F,             /* 0x54 LNA On, Off Measurement                  */
  DT_RXLNA_LOAD_F,            /* 0x55 Load Calibrated LNA parameter            */

  DT_TXAGC_INIT_F,            /* 0x56 CDMA Tx AGC Calibration Init Data Load   */
  DT_TXAGC_CAL_F,             /* 0x57 CDMA Tx AGC Calibration Start and Next.. */
  DT_TXAGC_LOAD_F,            /* 0x58 Save to NV and Load to Tx AGC RAS RAM    */

  DT_TXLIM_INIT_F,            /* 0x59 */
  DT_TXLIM_CAL_F,             /* 0x5A CDMA Tx Limit vs. Freq                   */
  DT_TXLIM_SAVE_F,            /* 0x5B CDMA Tx Limit vs. Freq Save to NVM       */
  DT_TXLIM_DATA_F,            /* 0x5C */

  DT_RFLOADNV_F,              /* 0x5D Load NVM Data to Rf Driver               */
  DT_READY_F,                 /* 0x5E Are you, mobile ready?                   */

  DT_READVALUE_F,             /* 0x5F Read various Values of MS's current state*/
  DT_RXLNA_RISE_F,            /* 0x60 00.06.14 not use */
  DT_RXLNA_OFFSET_F,          /* 0x61 00/06.14 not use */
  DT_RXLNA_FALL_F,            /* 0x62 00/06.14 not use */
  
  DT_TXDRV_ONOFF_F,           /* 0x63 */
  DT_TXPDM_CAL_F,             /* 0x64 */
  DT_TXPDM_SAVE_F,            /* 0x65 */
  DT_RXLNA_INIT_F,            /* 0x66 */

  DT_RXLNA_OFF0_CAL_F,        /* 0x67 */
  DT_RXLNA_OFF0_SAVE_F,       /* 0x68 */
  DT_TXOFF_F,                 /* 0x69 */
  DT_LNASTATE_F,              /* 0x6A */

  DT_TXFREQ_INIT_F,           /* 0x6B 00/06.14 not use */
  DT_TXFREQ_CAL_F,            /* 0x6C 00/06.14 not use */
  DT_TXFREQ_LOAD_F,           /* 0x6D 00/06.14 not use */
  DT_TXAGC_EXCITER_F,         /* 0x6E 00/06.14 not use */
  DT_TXAGC_EXCITER_LOAD_F,    /* 0x6F 00/06.14 not use */

  DT_PAPDM_CAL_F,             /* 0x70 */
  DT_PAPDM_SAVE_F,            /* 0x71 */

  DT_THERM_CAL_INIT_F,        /* 0x72 00/06.14 not use */
  DT_THERM_CAL_F,             /* 0x73 00/06.14 not use */
  DT_THERM_CAL_LOAD_F,        /* 0x74 00/06.14 not use */

  DT_LOW_TXLIM_INIT_F,        /* 0x75 */
  DT_LOW_TXLIM_CAL_F,         /* 0x76 CDMA Tx Limit vs. Freq                   */
  DT_LOW_TXLIM_SAVE_F,        /* 0x77 CDMA Tx Limit vs. Freq Save to NVM       */
  DT_LOW_TXLIM_DATA_F,        /* 0x78 */

  DT_HIGH_TXLIM_INIT_F,       /* 0x79 */
  DT_HIGH_TXLIM_CAL_F,        /* 0x7A CDMA Tx Limit vs. Freq                   */
  DT_HIGH_TXLIM_SAVE_F,       /* 0x7B CDMA Tx Limit vs. Freq Save to NVM       */
  DT_HIGH_TXLIM_DATA_F,       /* 0x7C */

  DT_SAVE_TEMP_SENSE_F,       /* 0x7D */
  DT_SAVE_TXLIM_PWR_F,        /* 0x7E */
  DT_SAVE_PAGAIN_TEMP_F,      /* 0x7F */

  DT_PABIAS_VS_TEMP_LOAD_F,   /* 0x80 */
  DT_TXLIM_VS_TEMP_LOAD_F,    /* 0x81 */ 
  DT_TX_PAPWR_COMP_LOAD_F,    /* 0x82 */

  DT_TX_DRIVER_CAL_F,		  /* 0x83 */ 
  DT_TX_DRIVER_SAVE_F,        /* 0x84 */
  DT_TXLIM_VS_TEMP_INITLOAD_F,/* 0x85 */  

  DT_RXAGC_FRQINIT_F,         /* 0x86 */
  DT_RXAGC_FRQTUNE_F,         /* 0x87 */
  DT_RXAGC_FRQCAL_F,          /* 0x88 */
  DT_RXAGC_FRQLOAD_F,         /* 0x89 */
  DT_NORMAL_LNA_F,            /* 0x8A */

  // kmh insert for B0 SAR Test start 01.04.17
  DT_TXLIMVSFREQ_DATA_LOAD_F,  /* x8B */
  // kmh insert for B0 SAR Test end 01.04.17

  // tasik 01.05.28
  // 재부팅을 하지않고 update된 cal값들을 RAS RAM과 register에 
  // 올리기 위함.
  DT_SELECTIVE_CAL_LOAD_F,	   /* 0x8C*/

 
  DT_LNA_GAIN = 0xA0,		  /* 0xA0 tasik 00.08.02*/
  DT_RX_AGC,				  /* 0xA1 tasik 00.08.02*/
  DT_TX_AGC,				  /* 0xA2 tasik 00.08.02*/
  DT_VCA_CNT,				  /* 0xA3 tasik 00.11.02*/
  DT_LCD_TEST_F=0xA5,		  /* 0xA5 tasik 00.06.29*/ 
  DT_KEYPAD_SCAN_TEST_F,	  /* 0xA6 tasik 00.07.14*/
// AUDIO 시헝 1
  DT_AUDIO_LOOPBACK,		  /* 0xA7 tasik 00.07.24 */
  DT_AUDIO_BEEP,			  /* 0xA8 tasik 00.07.26 */
  DT_AUDIO_RING,
  DT_AUDIO_MIC_GAIN_SET=0xAA, /* 0xAA tasik 00.07.28 */
  DT_IRDA_LOOPBACK = 0xAF,
  DT_AUDIO_RECORD_TEST=0xB2,  /* 0xB2 tasik 00.07.25 */
  DT_MOTOR_F=0xB4,			  /* 0xB4 tasik 00.07.12*/
  DT_LCD_INIT_F,			  /* 0xB5 happykid 01.04.25 */	
  // AUDIO 시험 2 
  DT_AUDIO_INIT	= 0xC0,			/* 0xC0 tasik 00.07.24 */
  DT_CHANGE_EARMIC = 0xC2,		/* 0xC2 tasik 00.07.24 */
  DT_READ_GPIO=0xC3,			/* 0xC3 tasik 00.07.24 */		
 
  DT_AUDIO_VOLUME_SET=0xD0,		/* 0xD0 tasik 00.07.24 */
  DT_AUDIO_CODEC_SET,			/* 0xD1 tasik 00.07.24 */
  DT_AUDIO_VOCODER_SET,			/* 0xD2 tasik 00.07.24 */
  
	DT_KEYPAD_TEST_F = 0x200,	/* 0x0200 */
	DT_TCXO_ON_F,				/* 0x0201 */
	DT_AUDIOPWR_ON_F,			/* 0x0202 */
	DT_SLEEP_ENA_F,				/* 0x0203 */
	DT_RX_PLL_ONOFF_F,			/* 0x0204 */
	DT_IDLE_ENA_F,				/* 0x0205 */
	DT_BACKLIGHT_F,				/* 0x0206 */

	DT_READBATTB_VAL_F,			/* 0x0207 */
	DT_BATTB_ADJ_F,				/* 0x0208 */
	DT_READRSSIB_VAL_F,			/* 0x0209 */
	DT_RSSIB_ADJ_F,				/* 0x020A .. now be saved to NV_FM_TX_PWR_I */
	DT_KEYPAD_SCANTIME_F,		/* 0x020B */
	DT_READ_ECIO_F,				/* 0x020C */

	DT_CHARGER_CNTL,			/* 0x020D */	 // kmh insert for CHARGER_CNTL ON/OFF Control 00.03.23

	DT_TX_ENABLE_F,				/* 0x020E tasik 00.11.22*/ // tx 살리기 			

#ifdef SKTT_XSCRIPT // jnlee 01.11.13
// kmh test code start for Kick Project 01.04.09
	DT_ADC_READ_F = 0x0210,	  /* 0x0210 */
	DT_TX_PWR_CNTL_F,		  /* 0x0211 */
	DT_TX_MANUAL_MODE_F,	  /* 0x0212 --> 향후 삭제 예정임 by kmh */
	DT_TX_AGC_VALUE_F,		  /* 0x0213 --> 향후 삭제 예정임 by kmh */
// kmh test code end for Kick project 01.04.09
 // wonjin for KiCK 01.05.07
    DT_OUTPM_F,				  /* 0x0214 */	
    DT_OUTPWM_F,			  /* 0x0215 */	
  // end 01.05.07
// kka start for Kick 01.05.17
	DT_AUDIO_TX_VOLUME_SET,	  /* 0x0216 */
	DT_AUDIO_RX_VOLUME_SET,	  /* 0x0217 */
// kka end for Kick 01.05.17
// kmj test code start for Kick Project 01.06.07
    DT_LOAD_RAS_RAM_F,		  /* 0x0218 */
	DT_RF_FLAG_SET_F,		  /* 0x0219 */ 
	DT_TX_MAX_PWR_CNTL_F,	  /* 0x021a */
	DT_TUNE_TO_CH_F,		  /* 0x021b */
// kmj test code end for Kick Project 01.06.07
#endif // SKTT_XSCRIPT


#if MMI_TYPE == SKY_UI			/* by jnlee 01.07.25 */
	DT_UISET_F = 0xFFD,			/* 0xffd for ui setting by jnlee 01.08.09*/
	DT_UNLOCK_F = 0xFFE,		/* 0xffe unlock autolock, lock and make password 0000 for EQUS MMI */
#endif

	DT_SYSINIT_F = 0xFFF,		/* 0x0FFF ... temporary for reading tx lim power .vs. temp */

//*****************************************************************************
#endif	// #ifdef FEATURE_SKTT_CAL		// kmh insert for calibration 00.06.13
//*****************************************************************************
  DT_MAX_F                    /* Last and invalid directed test choice    */

} diag_test_enum_type;


#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
/*--------------------------------------------------------------------------
                  RF CALIBRATION COMMAND
--------------------------------------------------------------------------*/

/* for Rx, Tx, RAS RAM index */
typedef PACKED struct {
  byte  rasindex;
  word  rasdata;
} ras_parm_type;

/* for rf item enum type to load */
typedef enum {
	LNAOFF,
	LNAON,
	OFFSET,
	ASKAGCVAL,	/* not rational parameter, but it's very trivial */
	LNAVAR,
	NOSTATE
} lna_enum_type;

/* for Adjustment of PA Bias PDM Value dependent on Temperature */
enum {
	TEMPHI,
	TEMPLO,
	REFRES
};

/* for Value of current phone status */
enum {
	PWRLIMIT,
    FREQINX,
	TEMPINX,
	ADCTHERM,
	ADCBATT,
	AGCVAL,
	TXAGC,
	PH_STATE,
	CHANNEL,
	SYSID,
	ISLOCK,
	TXAGC7,
	ECIO
};
#endif // #ifdef FEATURE_SKTT_CAL

/*-------------------------------------------------------------------------

                    Directed Test parameter types

--------------------------------------------------------------------------*/

/* Parameters for the set_attn directed test */

typedef PACKED struct {
  byte  attn;
} set_attn_parm_type;

/* Parameters for the switch_nam directed test */

typedef PACKED struct {
  byte  nam;
} nam_type;

/* Parameters for the send_min directed test */

typedef PACKED struct {
  byte  nam;
  byte  min;
} send_min_parm_type; 

/* Parameters for the sat directed test */

typedef PACKED struct {
  byte  sat;
} sat_parm_type;

/* Parameters for the CDATA directed test */

typedef PACKED struct {
  byte  cdata[6];
} cdata_parm_type;

/* Parameters for the dtmf directed test */

typedef PACKED struct {
  byte  dtmf;
} dtmf_parm_type;

// tasik 00.07.26

/* cdata_parm_type 가 가장 큰 패킷이다. 따라서 6바이트가 넘지 않도록 해야한다.
왜냐하면 아래의 union은 가장 큰 것을 기준으로 메모리를 할당하기 때문이다.*/

/* Parameters for sample sound test */
typedef PACKED struct {
   boolean tone_on;
   word  tone_duration;	    /* time */  
   byte  tone_gain;			/* volumn */
} audio_beep_type;

// tasik 00.07.25
/* Parameters for volumn set test*/
typedef PACKED struct {
	byte Vol;
} audio_vol_set_type;

// tasik 00.07.28
/* Parameters for volumn set test*/
#if (MODEL_ID == MODEL_D1)
typedef PACKED struct {
   byte	 have_voice_or_not; 
   word  input_path;
   word  output_path;
} audio_codec_set_type;
#elif (MODEL_ID == MODEL_A10) // jwpark 00.11.15
typedef PACKED struct {
  byte  Mic_On;		// SND_MUTE_UNMUTED, SND_MUTE_MUTED
  byte  Spk_On;		// SND_MUTE_UNMUTED, SND_MUTE_MUTED
  byte  Path_set;
} audio_codec_set_type;
#elif (MODEL_ID == MODEL_B1) // jnlee 00.11.18
typedef PACKED struct {
  byte  Mic_On;		// SND_MUTE_UNMUTED, SND_MUTE_MUTED
  byte  Spk_On;		// SND_MUTE_UNMUTED, SND_MUTE_MUTED
  byte  Path_set;
} audio_codec_set_type;
#else 
typedef PACKED struct {
  byte  Mic_On;		// SND_MUTE_UNMUTED, SND_MUTE_MUTED
  byte  Spk_On;		// SND_MUTE_UNMUTED, SND_MUTE_MUTED
  byte  Path_set;
} audio_codec_set_type;
#endif
// tasik 00.07.30
/* parameters for audio mic gain set test */
typedef PACKED struct {
	boolean  state;
    byte	 mic_vol;
} audio_mic_gain_set_type;

// tasik 00.08.28
// ring_tone test를 위한 파라미터 
typedef PACKED struct {
   boolean  ring_start_en;
   word  freq;
   word  duty;
} audio_ring_type;


// jwpark 00.11.15
typedef PACKED struct {
  boolean  music_start_en;
  word     music_id;
  byte     music_vol;
} audio_music_type;


#ifdef SKTT_XSCRIPT // jnlee 01.11.13
typedef PACKED struct {
	byte device;
	byte data;
} adc_read_type;

typedef PACKED struct {
	byte onoff;
} tx_cntl_type;

typedef PACKED struct {
	word addr;
	byte mask;
	byte data;
} outpm_byte_type;

typedef PACKED struct {
	word addr;
	word mask;
	word data;
} outpm_word_type;
#endif	//#ifdef SKTT_XSCRIPT // jnlee 01.11.13



/* Union of the directed test parameter types */
typedef PACKED union {
  word                    chan;       /* Channel for load-synth cmd  */
  set_attn_parm_type      sa;         /* Attn level for set-attn cmd */
  nam_type                nam;
  send_min_parm_type      sm;
  sat_parm_type           sat;
  cdata_parm_type         cdata;
  dtmf_parm_type          dtmf;

  audio_beep_type		  audio_beep;
  audio_ring_type		  audio_ring; /* 00.08. 28 tasik*/
  audio_vol_set_type	  audio_vol; /* 00.07.25 tasik */
  audio_codec_set_type	  audio_codec; /* 00.07.28 tasik */
  audio_mic_gain_set_type audio_mic; /* 00.07.28 tasik */
  audio_music_type        audio_music; // jwpark 00.11.15

#ifdef FEATURE_SKTT_CAL		// kmh insert for calibration 00.06.13
  ras_parm_type           ras;        /* for Rx, Tx RAS RAM index      */
  lna_enum_type           lna;        /* for LNA On/Off/Ask            */
  word					  rfitem;     /* for rf item enum type to load */
  byte                    askvalue;   /* Ask Phone his current status value s*/
#endif	// #ifdef FEATURE_SKTT_CAL

#ifdef SKTT_XSCRIPT // jnlee 01.11.13
  adc_read_type				adc_read;
  tx_cntl_type				tx;
  outpm_byte_type			outpm;
  outpm_word_type			outpwm;
#endif

} diag_test_parm_type;


/*--------------------------------------------------------------------------

                     Directed Test Results Types

--------------------------------------------------------------------------*/

/* RCVS1 response (requested with a terminate command) */

typedef PACKED struct {
  byte  num_uncorr;
  byte  num_corr;
} rcvs1_type;

/* WSTS response (requested with a terminate command ) */

typedef PACKED struct {
  word  num_syncs;
} wsts_type;

/* RSSI response */

/* Union of directed test response types */

typedef PACKED union {
  rcvs1_type        rcvs1;              /* Response from RCVS1 command */
  wsts_type         wsts;               /* Response from WSTS command */
  byte              rssi;               /* Response from RSSI command */

#ifdef FEATURE_SKTT_CAL		// kmh insert for calibration 00.06.13
  word              rasresult;          /* for Rx, Tx RAS RAM result */
  word              readvalue;          /* Value Read */
#endif	// #ifdef FEATURE_SKTT_CAL

#ifdef SKTT_XSCRIPT // jnlee 01.11.13
// kmh test code start for Kick Project 01.04.09
  adc_read_type		adc_read;
  tx_cntl_type		tx;
// kmh test code end for Kick Project 01.04.09
#endif

} diag_test_results_type;


/*===========================================================================

   Test Mobile Commands

===========================================================================*/

 typedef enum {
   TMOB_TCA_F,                 /* Traffic Channel Assignment */
   TMOB_TC_MSG_F,              /* Traffic Channel Message */
   TMOB_AC_MSG_F,              /* Access Channel Message */
   TMOB_SRCH_PARM_F,
   TMOB_PC_ASET_F, 
   TMOB_PC_NSET_F, 
   TMOB_TC_ASET_F, 
   TMOB_TC_NSET_F, 
   TMOB_MARKOV_F, 
   TMOB_TEST_F,                /* factory test */
   TMOB_SECT_LOCK_F,
   TMOB_SCAN_RF_CNTL_F,        /* Scanner commands */
   TMOB_SCAN_CONFIG_F, 
   TMOB_SCAN_ENA_F, 
   TMOB_SCAN_PN_ID_F, 
   TMOB_SCAN_NLIST_F,
   TMOB_MAX_F
} tmob_cmd_code_type;

 typedef PACKED struct {
   word     tmob_cmd_id;       /* Use tmob_cmd_code_type */
   word     test_id;           /* ID of test to run */
} tmob_test_type;

 typedef PACKED struct {
   word     tmob_cmd_id;       /* Use tmob_cmd_code_type */
   byte     tca_code_chan;
   dword    tca_long_code;
   word     tca_so;
   word     tca_freq;
   byte     tca_frame_stagger;
   byte     tca_mar_rate;
} tmob_tca_type;


 typedef PACKED struct {
   word     tmob_cmd_id;    /* Use tmob_cmd_code_type */

   byte          len;

#define               TEST_R_TC_MSG_CNT  (1184/8)
   byte          msg[ TEST_R_TC_MSG_CNT ];
    /* Actual reverse link traffic channel message bits. */
} tmob_tc_msg_type;


 typedef PACKED struct {
  word     tmob_cmd_id;       /* Use tmob_cmd_code_type */

  byte      len;              /* Number of bytes in message */
  boolean   req;              /* True: access request, False: response */
  byte      seq_num;          /* Max number of access probe sequences */

  /* set the next 3 parameters all to zero to get a long code mask
  ** of ZERO which implies no scrambling.
  */
  byte          pagech;        /* Current Paging Channel */
  word          base_id;       /* Base station identification */
  word          pilot_pn;      /* Pilot PN sequence offset index */

  byte          pam_sz;        /* Access Channel preamble length */
  byte          max_cap_sz;    /* Max Access Channel msg capsule size */
  byte          bkoff;         /* Probe sequence backoff range */
  byte          probe_bkoff;   /* Probe backoff range */
  byte          acc_tmo;       /* Acknowledgement timeout */
  byte          init_pwr;      /* Initial power offset for access */
  byte          pwr_step;      /* Power increment */
  byte          num_step;      /* Number of access probes */
  byte          acc_chan;      /* Number of access channels */
  word          rn;            /* Random number of chips backoff - ignored */
  dword         p;             /* Persistence probability */

#define              TEST_AC_MSG_CNT  (1184/8)
  byte          msg [TEST_AC_MSG_CNT];
                      /* Access Channel message to send */
} tmob_ac_msg_type;


/*--------------------------------------------------------------------------
                   CHANGE PILOT SEARCH PARAMETERS COMMAND

This command provides the Searcher Task with search parameters from
base station messages.
--------------------------------------------------------------------------*/

 typedef PACKED struct
{
  word      tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  byte       win_a;
   /* Active set search window size index    */
  byte       win_n;
   /* Neighbor set search window size index  */
  byte       win_r;
   /* Remaining set search window size index */

  byte       t_add;
   /* Threshold of energy at which a pilot moves up to the Candidate Set */

  byte       t_drop;
   /* Threshold of energy at which a pilot should be dropped from the
      Active Set or Candidate Set */

  byte       t_comp;
   /* Margin by which a pilot must exceed an Active Set pilot to trigger a
      new Power Measurement Report */

  byte       t_tdrop;
   /* Index of the duration for which a pilot must be below t_drop before
      being dropped or reported */

  byte       nghbr_max_age;
   /* Maximum "age" for a pilot to remain in the Neighbor Set */

} tmob_srch_parm_type;


/*--------------------------------------------------------------------------
               PAGING CHANNEL ACTIVE SET UPDATE COMMAND

This command tells the searcher to perform an idle handoff from one
Paging Channel pilot to another.
--------------------------------------------------------------------------*/

 typedef PACKED struct
{
   word      tmob_cmd_id;
   /* Common command header.  Use tmob_cmd_code_type */

  int2       aset_pilot;
    /* The pilot PN index of the new Active Set pilot */

  word       aset_walsh;
    /* The walsh channel to be used with the new paging channel */

  qword      pn_mask;
    /* PN long code mask to be used on the new paging channel */

  word       cdma_freq;
    /* CDMA Channel number of the new paging channel */

} tmob_pc_aset_type;


/*--------------------------------------------------------------------------
                      NEIGHBOR SET UPDATE COMMAND

This command updates the Neighbor Set.

Note: this command format is used for both paging and traffic channel
variants.  
--------------------------------------------------------------------------*/

 typedef PACKED struct
{
  word      tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  byte      pilot_inc;
   /* Spacing of potential pilot offset in use in this system.  The
      remaining set consists of all pilot offsets which are multiples
      of this value (but are not in another set). */

  word      nset_cnt;  
    /* Number of neighbor set members */

  PACKED struct
  {
    int2  pilot;
  }                  nset[ 20 ];
    /* Pilot offset indices for neighbor set members */

} tmob_nset_type;


/*--------------------------------------------------------------------------
              TRAFFIC CHANNEL ACTIVE SET UPDATE COMMAND

This command replaces the Active Set on the traffic channel.
--------------------------------------------------------------------------*/

 typedef PACKED struct
{
  word      tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  word      cdma_freq;
   /* CDMA Channel number */

  byte      t_add;
   /* Threshold of energy at which a pilot moves up to the Candidate Set */

  byte      t_drop;
   /* Threshold of energy at which a pilot should be dropped from the
      Active Set or Candidate Set */

  byte      t_comp;
   /* Margin by which a pilot must exceed an Active Set pilot to trigger a
      new Power Measurement Report */

  byte      t_tdrop;
   /* Index of the duration for which a pilot must be below t_drop before
      being dropped or reported */

  word      aset_cnt;
   /* Number of Active Set members */

   /* For each member of the new Active Set: */
  PACKED struct
  {
    int2  pilot;
      /* Pilot PN index of this Active Set member */

    word  walsh;
      /* Walsh channel to be used on the traffic channel with the pilot */

    byte  pwr_comb_ind;
      /* Flag: nonzero if the power control subchannel on this pilot
               contains the same symbols as that on the previous pilot. */

  }      aset[ 6 ];
    /* Pilot offset indices and walsh channels for active set members */

} tmob_tc_aset_type;


/*===========================================================================

           SET MARKOV TABLE BINARY PROBABILITIES

Probabilities are listed as values which ran# must be less than to be
valid. Probs are listed eighth, eighth+quarter, eighth+quarter+half.
If ran# is greater or equal than last value then transition to full.

Sn = State at frame n.  Rn = Rate at frame n. 
State is table 1st dim index.     Transition is index to table 2nd dim.

Sn+1 = 4 * ( Sn mod 4 ) + ( 3 - Trans ).
Rn+1 = 4 - ( Sn+1 mod 4 ).

===========================================================================*/

typedef PACKED struct
{
  word      tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  word Markov_prob [16] [3];          /* Variable Rate Probs */

} tmob_markov_type;

/*===========================================================================

PACKET   tmob_sect_lock_type

ID       TMOB_SECT_LOCK_F

PURPOSE  Sent by DM to lock the Mobile into a specified sector

===========================================================================*/
 typedef PACKED struct
{
  word   tmob_cmd_id;   /* Common command header.  Use tmob_cmd_code_type */
  word   pn_offset;
  word   cdma_chan;
  byte   pag_chan;
  byte   acc_chan;
} tmob_sect_lock_type;

/*--------------------------------------------------------------------------
                         SCANNER RF CONTROL COMMAND

This command controls various RF parameters of the pilot scanner.
--------------------------------------------------------------------------*/
typedef PACKED struct
{
  word     tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  word     cdmach;
    /* CDMA Channel (0=Auto, >0=Force this channel) */
 
  word     reg;
    /* Registration On/Off (0=Auto, 1=OFF) */

  word     lna;
    /* LNA On/Off (0=Auto, 1=Force OFF, 2=Force ON) */

} tmob_scan_rf_cntl_type;  /* Scanner RF contol command */

/*--------------------------------------------------------------------------
                        PILOT SCANNER CONFIG COMMAND

This command provides the Search Task with parameters for
the pilot scanner.
--------------------------------------------------------------------------*/
typedef PACKED struct
{
  word tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  word scan_mode;
    /* Scan mode (0=A R R R R (all PNs), 1=A N N N (List) */

  word incr;
    /* Pilot Increment (0=Use OTA, 1,2,..=User specified) */

  word winsize;
    /* Scanner window size in chips (0=64*incr, 1=Use OTA N, 2..512=User specified) */

  word integ;
    /* Integration parameter (0=Default, 1=360, 2=512, 3=720, 4=1020) */

  word ho_typ;
    /* Handoffs allowed (0=Nlist only, 1=All, 2=None) */

  word log_fmt;
    /* Logging format (0=Dense Scanner format, 1=STD Qualcomm DM format) */

  word log_thr;
    /* Logging threshold value (0=All, 1=T_DROP, 2=T_ADD) */

} tmob_scan_config_type;/* Scanner config */

/*--------------------------------------------------------------------------
                        SCANNER ENABLE/DISABLE COMMAND

This command enables or disables the pilot scanner.
--------------------------------------------------------------------------*/
typedef PACKED struct
{
  word tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  word ena;
    /* TRUE=ON,  FALSE=OFF */

} tmob_scan_ena_type; /*  Scanner enable command */

/*--------------------------------------------------------------------------
                      SCANNER PN ID STORAGE COMMAND

This command stores a character string identification for a pilot offset.
--------------------------------------------------------------------------*/

typedef PACKED struct
{
  word     tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  int2     pilot_off;
   /* Active pilot we want to define the Neighbor list for */

  byte     name[4];
    /* Name associated with this pilot offset */
  
} tmob_scan_pn_id_type;

/*--------------------------------------------------------------------------
                  SCANNER NEIGHBOR LIST STORAGE COMMAND

This command stores a Neighbor list for a particular Active pilot.
--------------------------------------------------------------------------*/

typedef PACKED struct
{
  word     tmob_cmd_id;
  /* Common command header.  Use tmob_cmd_code_type */

  word     replace_all;
    /* TRUE = replace all Neighbors,
       FALSE = Just add to the end of the Neighbor list or
               replace last member if Neighbor list full */

  int2     active_pilot;
    /* Active pilot we want to define the Neighbor list for */

  word     nlist_cnt;  
    /* Number of neighbor list members for this Active pilot */

  int2     nlist_pilot[ 20 ];
    /* Pilot offset indices for neighbor set members */

} tmob_scan_nlist_type;

typedef PACKED union {
   tmob_cmd_code_type     tmob_cmd_id;
   tmob_test_type         test;         /* Run a special test         */
   tmob_tca_type          tca;          /* Traffic Channel Assignment */
   tmob_tc_msg_type       tc_msg;       /* Traffic Channel Message    */
   tmob_ac_msg_type       ac_msg;       /* Access Channel Message     */
   tmob_srch_parm_type    srch_parm;    /* Srch parameters            */
   tmob_pc_aset_type      pc_aset;      /* paging active set member   */
   tmob_nset_type         nset;         /* page/traffic neighbors     */
   tmob_tc_aset_type      tc_aset;      /* traffic active set         */
   tmob_markov_type       markov;       /* update Markov table        */
   tmob_sect_lock_type    sect_lock;    /* Lock to a specified Sector */
   tmob_scan_rf_cntl_type scan_rf_cntl; /* Scanner capability         */
   tmob_scan_config_type  scan_config;  /* Scanner config             */
   tmob_scan_ena_type     scan_ena;     /* Scanner on/off command     */
   tmob_scan_pn_id_type   scan_pn_id;   /* Scanner PN ID              */
   tmob_scan_nlist_type   scan_nlist;   /* Neighbor list storage      */
} tmob_cmd_type;

#endif
