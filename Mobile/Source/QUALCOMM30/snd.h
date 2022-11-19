#ifndef SND_H
#define SND_H
/*===========================================================================

                S O U N D   T A S K    H E A D E R    F I L E

DESCRIPTION
  This header file contains all the definitions necessary for other
  tasks to interface with the sound task.

Copyright (c) 1992, 1993, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999, 2000       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.
   
  $Header:   L:/src/asw/MSM5000/VCS/snd.h_v   1.4   07 Aug 2000 20:02:28   ropalsky  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.08.16   jwpark  YMU757B new API for music on EFS
00.08.14   jwpark  YMU757B new API
--------   ---     ----------------------------------------------------------
08/07/00    ro     Merged SND_HFK_TONE1 and 2.
            st     Added SND_PB_ABORT status to inform voice playback client
                   of a VP abort. 
06/21/00   nxb     T_O uses SND_OLD_VOLUME_UNITS.
09/29/99    st     Added FEATURE_PHONE_VR to VOICE_MEMO_ACTION_COMMAND so
                   that voice services can enable the codec for capture.
07/19/99   st/vl   Added snd_pb_status_type to snd_vp_free_buffer_type.
                   Removed 'far '. Added snd_get_voice_path_out() prototype.
06/30/99  sm/RPW   Integrated AGC feature with VR/VM features; added Q2 support
                   for SND_HFK and AVBAP defines (st).
04/20/99    st     Added vibrator support for Digby (FEATURE_VIBRATOR) target.
03/22/99    st     Added SND_SND_MUTE (mask that indicates that sound task is 
                   requesting mute); modified parametric_group_type.
03/04/99    st     Made modifications to support playing tones specified by
                   their frequency values in hertz: modified snd_tone_type,
                   snd_tone_packet_type, snd_multi_packet_type.
                   Removed reference to ringer enable line (moved to ring.h);
                   added support for voice memo.
02/26/99    st     Added SND_VR_HFK_CALL_RECEIVED to snd_predef_type.
01/13/99    st     Added support for voice prompting.
12/17/98    st     Added SND_HFK_PROTOCOL_TGP_VR to snd_hfk_protocol_type.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.
10/05/98    st     HFK-VR mainlining: Added SND_EXT_PROTOCOL command; added 
                   SND_IDS_SIG signal; added extproto to snd_packets_type; 
                   added snd_hfk_protocol_type and snd_ext_proto_packet_type.
10/01/98    st     Added SND_TWO_CODECS for TGP targets.
08/17/98    st     Added SND_MAX_SOUND_TONE. Also added SND_AVBAP_SUPPORTED
                   for TGP and Odie builds.
08/10/98    st     Replaced usage of the SinBaD interface with the BORSCHT
                   interface by request from the WLL group.
06/22/98    st     Added SND_ADSP_TONEGEN feature definition, added 
                   SND_CLASS_ADSP in snd_class_type for ADSP tone generation.
05/29/98    ha     Added service transition sounds and external power 
                   on/off predefined sounds to snd_predef_type.
04/02/98    ha     Added Support for T_O.
03/18/98    ha     Added SND_CALL_IN_PROGRESS_WLL and SND_PHONE_OFF_HOOK_WLL to
                   snd_predef_type for use with FEATURE_STD_WLL.
02/13/98    ha     Added SND_OLD_VOLUME_UNITS internal feature.
01/30/98    ha     Added support for SND_DTACO_ROAM_TONE. 
01/22/98    ha     Added SND_ROAM_RING to snd_predef_type.
01/21/98    ha     Added SND_VOL_SCALE control tone (for enhanced rings) 
                   under FEATURE_ENHANCED_RINGS. 
                   Updated piano notes (octaves 4 and 5) in snd_tone_type. 
                   Updated Copyright.
01/16/98    ro     Added support for external communications and device id.
                   Added SND_EXT_CONNECT and SND_EXT_ID commands.
12/23/97    ha     Added SND_CLASS_RING to snd_class_type.
                   Removed SND_APATH_PRIVACY from snd_apath_type.
12/08/97    ha     Added click_enable field to snd_tone_packets_type.
                   Removed SND_USE_RINGER_INT for T_G cond. compile.
08/15/97   dak     Added stutter dial tone to be used if messages are waiting.
07/23/97    ro     Add Lint comment for SND_APATH_NONE.
06/17/97    ro     Support for T_Q.
06/17/97   dak     Added SND_DT_TONE_BRAZIL, SND_DIAL_TONE_BRAZIL,
                   SND_BUSY_BRAZIL and SND_RING_BRAZIL under 
                   FEATURE_BRAZIL_WLL and SND_REORDER_WLL under
                   FEATURE_STD_WLL.
06/06/97    ro     Add new pre-defined sound - SND_ALTERNATE_REORDER.
                   Get rid of some hanging comma's.
05/23/97    ro     Remove SND_NV_CMD_SIG, and replace it with
                   SND_OVERRIDE_TIMER_SIG.
                   Use FEATURE_HFK_PLUS to define SND_HFK_PLUS.
04/29/97    ro     Add #define SND_HFK_PLUS for T_T.
04/28/97    ro     Add T_MD for SND_HFK.
04/11/97    ro     Remove T_MD in conditional compiles, module does not have
                   a ringer or a carkit option. Remove unused SND_SINGLE_TONE.
03/28/97    ro     Support for TGP. Removed support for T_P. Re-ordered
                   snd_tone_type and added SND_FIRST_CONTROL_TONE to provide
                   a structured way of detecting control tones.
                   Added additional loop-back control tones.
                   Added SND_RING_C for user selectable rings.
                   Removed prototypes for snd_st_mode, snd_lpf_mode,
                   snd_wbd_mode, snd_voc_on. Removed snd_lpf_type.
                   Added SND_RING_1 through 9 for user-selectable ring sounds.
03/12/97   jjn     Added (TG==T_MD) for Module in conditional compile statements
02/12/97    ro     Added support for aux generators - SND_CLASS_AUX.
                   Added sound features: SND_USE_RINGER_ENABLE,
                   and SND_USE_RINGER_INT.
01/20/97    ro     Added SND_RPT1 in support of key delay function.
01/07/97    ro     Support for Car-kit and Earpiece DTMF's using the vocoder.
                   In snd_apath_type, paths are masks.
                   Added SND_ALL_PATHS.
                   Added SND_HFK, SND_SINGLE_TONE.
                   Added snd_path_packet_type, and SND_SET_PATH command
                   Removed snd_aud_packet_type
                   Changed snd_vol_packet_type to use snd_vol_type.
                   Added SND_VOLUME command
                   Added SND_PATH_TIMER_SIG
                   Added Maximum volume settings.
                   Added snd_vol_type, replaces byte in command packets.
                   Added snd_mute_control_type. Added
                   SND_APATH_PRIVACY to snd_apath_type.
                   Added rpt_sigs to snd_multi_packet_type.
                   Added snd_class_type. Added sclass field to cmd packets.
                   Added SND_SIGNAL_SILENCE, and SND_RPT_NOSIGNAL
                   to snd_tone_type.
10/07/96   dak     Replaced T_INDIA_WLL with FEATURE_INDIA_WLL
07/22/96   fkm     Minor Documentation Tweeks
03/19/96   fkm     Updates Per Code Review
02/16/96   fkm     Tweeks to comments and documentation
02/12/96   fkm     Lint and Documentation Updates and some Pre-Apex Defines.
12/13/95   fkm     India Updates and Menu-7-4 (mute) fix
11/30/95   fkm     Added Stuff for PIP and India Tones
11/02/95   fkm     Minor Name Change to Vocoder Gain Adjust Stuff and
                   added snd_set_voc_gain_adjust() to Interface Definitions.
11/02/95   fkm     Added snd_get_packet() defintion, Vocoder Gain Support,
                   slight change to Stereo Mute Packet.
10/10/95   fkm     Added SND_PHONE_AWAKE_SIG.
08/22/95   fkm     Added SND_MUTE_CAR_STEREO and SND_UNMUTE_CAR_STEREO
08/15/95   fkm     SND_ABRV_ALERT moved to SND_BUSY_ALERT and SND_ABVR_ALERT
                   reused per IS-99.
07/21/95   fkm     Message Waiting Sound and Tone Added to enums
06/19/95   fkm     Minor Documentation Update
06/14/95   fkm     Fix Documentation to SND.C matches SND.H
06/08/95   fkm     Defined SND_RINGER_OFF_SIG for Gemini
05/16/95   fkm     Added SND_ABRV_REORDER and SND_ABRV_INTERCEPT.
11/08/93   jah     Added SND_NV_CMD_SIG for NV command exchanges.
07/30/93   jah     Added snd_fm_pwr_type and prototype for snd_fm_tx_pwr().
06/04/93   jah     Added SND_VOC_CHNG_SIG.
05/03/93   jah     Added prototype for snd_wbd_mode().
04/12/93   jah     Added mute mask SND_MUTE_SNDK for muting during audio path
                   changes and other neat things that go 'klack'.
03/02/93   jah     Added snd_voc_on() prototype for the Portable.
02/19/93   jah     Added vox change signal for resetting vocoder background
                   noise estimate after a vox and path change.
02/09/93   jah     Added no-qwack changes, added DTMF path change to no-qwack.
01/08/93   jah     #ifdef'd tone/path/vol improvement while it stablizes.
                   Added SND_MUTE_SNDV for, voice during tone.
10/23/92   jah     Updated comments.
10/21/92   jah     Added masks for Diag mute override.
10/16/92   jah     Moved snd_apath_mute_ctl() to sndim.h (removed external
                   interface).  Added comments for mobile/portable.  Fixed
                   confusing naming between ring-back sound and ring-back tone.
09/15/92   jah     Changed SND_MUTE_ACPRX to SND_MUTE_ACPRXTX
09/10/92   jah     Added some new tones for ringing.
09/03/92   jah     Changed SND_MAX_MULTI_TONE to 66 for 1+32+32+1 tones for UI.
09/02/92   jah     Added SND_HORN_ON (horn alert).  Added snd_comp_ctl().
08/27/92   jah     Added snd_rxfm_mute_ctl()
08/25/92   jah     Added 1 KHz test tone
08/24/92   jah     Changed SND_MAX_MULTI_TONE to 65 for 32+32+1 tones for UI.
08/20/92   jah     Added SND_LOOP_BACK2 and repeat signal, added mute mask
                   for CDMA.
08/14/92   jah     Added "answer tone", "horn alert", "fade", and "Svc change"
07/27/92   jah     Converted to new BIO macros, added code review changes.
02/27/92   jah     created file

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "rex.h"        /* definition of REX data types         */
#include "target.h"     /* Target Specific Defintions           */
#include "queue.h"      /* Definitions for queue services       */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
#include "sdbid_b1.h"
#endif

#if (defined(FEATURE_IDS))
#error code not present
#endif
#if (defined(FEATURE_VOICE_PLAYBACK) || defined(FEATURE_VOICE_MEMO))
#error code not present
#endif

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* Sound task feature control
*/

/* Target has a HFK (car-kit) option.
*/
#if ((TG==T_MD) || (TG==T_G) || (TG==T_T) || \
     (TG==T_Q)  || (TG==T_O) || (TG==T_Q2))
#define SND_HFK
#endif

/* The carkit for this target uses "NGP Plus" protocol
*/
#ifdef FEATURE_HFK_PLUS
#error code not present
#endif

/* Does this target have an auxilliary alert generator */
#if (defined(FEATURE_VIBRATOR) || defined(FEATURE_BORSCHT))
#error code not present
#endif

/* Does this target support intelligent accessories */
#if (defined(FEATURE_IDS))
#error code not present
#endif

/* Target uses Gemini-like UI volume levels. 
*/
#if ((TG==T_O) || (TG==T_G) || (TG==T_I2) || (TG==T_Q))
#define SND_OLD_VOLUME_UNITS
#endif

/* Target (SSS) uses ADSP for tone generation during data call
*/
#ifdef FEATURE_ECFM
#error code not present
#endif

/* Target requires AVBAP  */
#if ((TG==T_O) || (TG==T_T) || (TG==T_Q2))
#error code not present
#endif
#if ((TG==T_O) || (TG==T_Q2)) 
#error code not present
#endif

#if ((TG==T_T) && defined(SND_AVBAP_SUPPORTED))
#error code not present
#endif

/* Parametric alerts */
#ifdef FEATURE_PARAMETRIC_ALERTS
#define SND_PARA_SUPPORT
#endif

/* <EJECT> */
/* Sound task REX signal definitions
*/

#ifdef FEATURE_VOICEPLAY // jwpark 00.11.09
	#define SND_VOICEPLAY_TIMER_SIG 0x00010000
#endif


#if defined(FEATURE_YMU757B_MELODY) || defined(FEATURE_YMU759_MELODY)
	// general purpose music timer
	#define SND_MUSIC_TIMER_SIG		0x1000 // jwpark 00.07.27

	// music data load to buffer from EFS file
	#define SND_FILE_READ_SIG       0x0800 // jwpark 00.08.16

	// Yamaha chip의  Vref를 끄기 위한 타이머
	// Vref켜는데 시간이 많이(200[msec]) 걸리므로
	// 끌때 어느정도 뒤(3000[msec])에 끄도록 한다.
	#define SND_VREF_TIMER_SIG		0x0400
#else
	#ifdef FEATURE_VOICE_PLAYBACK
	#error code not present
	#else
	/* Reserved (unused) signals.  Note the Upper Nibble is reserved for
	** system wide "common signals".  (See task.h.)
	*/
	#define SND_RSVD_0400_SIG       0x0400
	#endif /* FEATURE_VOICE_PLAYBACK */

	/* This signal is used to signal when the HFK, in VR mode, has been told that 
	** a tone is going to be played.
	*/
	#define SND_IDS_SIG             0x0800
#endif

/* This signal is used to create a delay for turning on an audio path
*/
#define SND_PATH_TIMER_SIG       0x0200

/* This signal is used by Searcher to tell Sound the Phone is fully awake.
*/
#define SND_PHONE_AWAKE_SIG       0x0100

/* This signal is used exclusively for rex_timed_wait() calls for VERY
** short naps in sound.  Note that they MUST in all cases be short enough
** to not cause Watch Dog Problems!
*/
#define SND_TIMED_WAIT_SIG      0x0080

/* This signal is used to keep the vocoder on for a period of time
** after a sound is played.
*/
#define SND_OVERRIDE_TIMER_SIG  0x0040

/* This signal is set when the Vocoder changes on/off state
*/
#define SND_VOC_CHNG_SIG        0x0020

/* This signal is set when the VOX is changed for CDMA
*/

// jwpark 00.11.02
#ifdef OKI_CODEC
#define SND_CODEC_OFF_TIMER_SIG 0x0010
#else
#define SND_VOX_CHNG_TIMER_SIG  0x0010
#endif

/* This signal is set when it's time to stop a tone
*/
#define SND_TONE_TIMER_SIG      0x0008

/* This signal is set when the audio can be unmuted after an audio path change
*/
#define SND_UNMUTE_SIG          0x0004

/* This signal is set when an item is put on the snd_cmd_q
*/
#define SND_CMD_Q_SIG           0x0002

/* This signal is set when the diag_rpt_timer expires
*/
#define SND_RPT_TIMER_SIG       0x0001

/* <EJECT> */
/*
** Mute control is "voted", so that multiple callers can ask for the
** audio to mute and un-mute.  A counting semaphore was not appropriate
** due to the problems with the caller counting mute/unmutes properly.
** Each caller has a specifed mask which identifies it.
*/
typedef enum {
  SND_MUTE_NONE    = 0,    /* NO MUTERS - MUST BE ZERO       */
  SND_MUTE_ACPFM   = 2,    /* Analog Call Processing (FM)    */
  SND_MUTE_ACPRXTX = 4,    /* Analog Call Processing (RX/TX) */
  SND_MUTE_SND     = 8,    /* Sound Task, general            */
  SND_MUTE_SNDV    = 16,   /* Sound Task, voice during tone  */
  SND_MUTE_DIAG    = 64,   /* Diagnostic override            */
  SND_MUTE_SNDPATH = 256   /* Sound Task, Path Mute (FM)     */
} snd_mute_type;

/* Mask of all-mute masks
*/
#define SND_SND_MUTE  ( (snd_mute_type)         \
                        ( SND_MUTE_SND          \
                        | SND_MUTE_SNDV         \
                        | SND_MUTE_SNDPATH ) )

#define SND_ALL_MUTE  ( (snd_mute_type)         \
                        ( SND_MUTE_ACPFM        \
                        | SND_MUTE_ACPRXTX      \
                        | SND_MUTE_SND          \
                        | SND_MUTE_SNDV         \
                        | SND_MUTE_DIAG         \
                        | SND_MUTE_SNDPATH ) )

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Sound Commands
*/
typedef enum {          /* (parameter) Brief description                   */

  SND_ANALOG,           /* (alog) Set the audio paths to Analog or not     */
  SND_SET_PATH,         /* (path) Set the audio path, and muting           */
  SND_VOLUME,           /* (volume) Set the voice volume for all paths     */
  SND_RING_VOLUME,      /* (volume) Set the ringer volume                  */
  SND_TONE_START,       /* (tone) Play a single tone for awhile            */
  SND_TONE_STOP,        /* (no parameter) Stop playing the current tone    */
  SND_MULTI_START,      /* (*multi_ptr) Play a multi-tone                  */
  SND_MULTI_STOP,       /* (no parameter) Stop the current multi-tone      */
  SND_SOUND_START,      /* (sound) Play a predefined sound                 */
  SND_SOUND_STOP,       /* (no parameter) Stop the current sound           */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif
#if (defined(FEATURE_VOICE_MEMO) || defined (FEATURE_PHONE_VR))
#error code not present
#endif
  SND_MUTE_CAR_STEREO,  /* (stereo) Mute Car Stereo (if Car Kit)           */
  SND_VOC_GAIN_ADJUST,  /* (vgadjust) Vocoder Gain Adjustment (M2M/M2L)    */
  SND_FILTERS,          /* (filters) Set VOX etc based on filter type      */
#ifdef SND_EXTCOM
#error code not present
#endif
#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif
#ifdef FEATURE_YMU757B_MELODY	// jwpark 00.08.04
  SND_MUSIC_START,
  SND_MUSIC_STOP,
#endif
#ifdef FEATURE_YMU759_MELODY	// youandi
  SND_MUSIC_START,
//SND_MUSIC_SOUND_START,
  SND_MUSIC_STOP,
  SND_MUSIC_HW_POWER_DOWN,
#endif
#ifdef FEATURE_VOICEPLAY  // jwpark 00.11.06
  SND_VOICE_START,
  SND_VOICE_STOP,
#endif
  SND_SPEAKER_VOLUME,
  SND_INVALID           /* (Leave Last) Invalid Command - Causes Error     */

} snd_op_type;

/* Physical and logical audio in/out paths
*/
/*  Type defining the different types of audio input/output devices
*/
typedef enum {
  SND_DEVICE_EARPIECE = 0,      /* The phones earpiece               */
  SND_DEVICE_EARPHONE,      /* The phones earphone               */
  SND_DEVICE_HFK_CORD,          /* Carkit with coil-cord             */
  SND_DEVICE_HFK_NO_CORD,       /* Carkit with blind-mate connector  */
#ifdef SND_EXTERNAL_HEADSET
#error code not present
#endif  
  SND_DEVICE_MAX ,               /*  */
  SND_DEVICE_CURRENT
  /*lint -esym(788,SND_DEVICE_MAX) this is never in a switch
  **statement.
  */
} snd_device_type;

typedef enum {
  SND_APATH_NONE    = 0,      /* No path (not to be used as 'mute')   */
  SND_APATH_PHONE   = 0x001,  /* Handset path                         */
  SND_APATH_EXT     = 0x002,  /* External path (speaker/hands-free)   */
  SND_APATH_MUTE    = 0x100   /* Audio is muted                       */
  /*lint -esym(788,SND_APATH_NONE)
  **This is never in a switch statement
  */
} snd_apath_type;

/* Mask of all physical paths
*/
#if (TG==T_T) 
#error code not present
#else
#define SND_ALL_PATHS   ( SND_APATH_PHONE       \
                        | SND_APATH_EXT         \
                        )
#endif

/* Any physical path may be muted or unmuted.
*/
typedef enum {
  SND_MUTE_UNMUTED = 0,         /* Audio path is not muted.     */
  SND_MUTE_MUTED,               /* Audio path is muted.         */
  SND_MUTE_MAX                  /* Always last in the list      */
} snd_mute_control_type;

/* Structure containing a volume setting for each physical path.
*/
typedef struct {
  byte phone;                   /* Volume level for Handset     */
  byte ext;                     /* Volume level for Speaker     */
  byte priv;                    /* Volume level for Privacy HS  */
} snd_vol_type;

/* DTMF audio output path
*/
typedef enum {
  SND_DTMF_APATH_LOCAL,         /* DTMF's on local audio        */
  SND_DTMF_APATH_TX,            /* Transmit DTMFs               */
  SND_DTMF_APATH_BOTH,          /* Tx and sound DTMF's locally  */
  SND_DTMF_APATH_MUTE           /* Mute the DTMFs               */
} snd_dtmf_apath_type;

/* Each audio path has one or more tone generators. The sound's class
** determines which generator is used to play it.
*/
typedef enum {
  SND_CLASS_DEFAULT = 0, /* Same as SND_CLASS_BEEP                         */
  SND_CLASS_BEEP,        /* Use the path's ringer, or voice generator      */
  SND_CLASS_RING,        /* Use the path's ringer or voice generator       */
  SND_CLASS_VOICE_BEEP,  /* Use the path's voice generator at tone volume  */
  SND_CLASS_VOICE,       /* Use the path's voice generator at voice volume */
#ifdef SND_ADSP_TONEGEN
#error code not present
#endif /* SND_ADSP_TONEGEN */
#ifdef SND_AUX_ALERT
#error code not present
#endif /* SND_AUX_ALERT */
#ifdef FEATURE_YMU757B_MELODY           // jwpark 00.08.03
  SND_CLASS_MUSIC,
  SND_CLASS_SAMPLE_MUSIC,
  SND_CLASS_BGM,
#endif // #ifdef FEATURE_YMU757B_MELODY // jwpark 00.08.03 end
#ifdef FEATURE_YMU759_MELODY           //youandi
  SND_CLASS_MUSIC,
  SND_CLASS_SAMPLE_MUSIC,
  SND_CLASS_BGM,
#endif // #ifdef FEATURE_YMU759_MELODY // youandi
  SND_CLASS_MAX          /* Always last in the list                        */
} snd_class_type;

/* DTMF/Tones for building single, multi-tones and Predefined Sounds.
** Note that some of these are Country Specific Tones, however suppressing
** Country Specific stuff at this level save only a few bytes at a large
** increase in complexity.
*/
typedef enum {

  SND_FIRST_TONE = -1,/* Use for range checking 1st tone                  */
  SND_0 = 0,          /* DTMF for 0 key                                   */
  SND_1,              /* DTMF for 1 key                                   */
  SND_2,              /* DTMF for 2 key                                   */
  SND_3,              /* DTMF for 3 key                                   */
  SND_4,              /* DTMF for 4 key                                   */
  SND_5,              /* DTMF for 5 key                                   */
  SND_6,              /* DTMF for 6 key                                   */
  SND_7,              /* DTMF for 7 key                                   */
  SND_8,              /* DTMF for 8 key                                   */
  SND_9,              /* DTMF for 9 key                                   */
  SND_POUND,          /* DTMF for # key                                   */
  SND_STAR,           /* DTMF for * key                                   */
  SND_CTRL,           /* Tone for a control key                           */
  SND_2ND,            /* Tone for secondary function on a key             */
  SND_WARN,           /* Warning tone (e.g. overwriting user phone# slot) */
  SND_ERR,            /* Tone to indicate an error                        */
  SND_TIME,           /* Time marker tone                                 */
  SND_RING_A,         /* 1st Ringer tone                                  */
  SND_RING_B,         /* 2nd Ringer tone                                  */
  SND_RING_C,         /* 3rd Ringer tone                                  */
  SND_RING_D,         /* 4th Ringer tone                                  */
  SND_RING_A4,        /*  440.0 Hz  -Piano Notes-                         */
  SND_RING_AS4,       /*  466.1 Hz                                        */
  SND_RING_B4,        /*  493.8 Hz                                        */
  SND_RING_C4,        /*  523.2 Hz                                        */
  SND_RING_CS4,       /*  554.3 Hz                                        */
  SND_RING_D4,        /*  587.3 Hz                                        */
  SND_RING_DS4,       /*  622.2 Hz                                        */
  SND_RING_E4,        /*  659.2 Hz                                        */
  SND_RING_F4,        /*  698.5 Hz                                        */
  SND_RING_FS4,       /*  739.9 Hz                                        */
  SND_RING_G4,        /*  784.0 Hz                                        */
  SND_RING_GS4,       /*  830.6 Hz                                        */
  SND_RING_A5,        /*  880.0 Hz                                        */
  SND_RING_AS5,       /*  932.2 Hz                                        */
  SND_RING_B5,        /*  987.7 Hz                                        */
  SND_RING_C5,        /* 1046.5 Hz                                        */
  SND_RING_CS5,       /* 1108.7 Hz                                        */
  SND_RING_D5,        /* 1174.6 Hz                                        */
  SND_RING_DS5,       /* 1244.3 Hz                                        */
  SND_RING_E5,        /* 1318.5 Hz                                        */
  SND_RING_F5,        /* 1397.0 Hz                                        */
  SND_RING_FS5,       /* 1479.9 Hz                                        */
  SND_RING_G5,        /* 1568.0 Hz                                        */
  SND_RING_GS5,       /* 1661.2 Hz                                        */
  SND_RING_A6,        /* 1760.0 Hz                                        */
  SND_RING_AS6,       /* 1864.7 Hz                                        */
  SND_RING_B6,        /* 1975.5 Hz                                        */
  SND_RING_C6,        /* 2093.1 Hz                                        */
  SND_RING_CS6,       /* 2217.4 Hz                                        */
  SND_RING_D6,        /* 2349.3 Hz                                        */
  SND_RING_DS6,       /* 2489.1 Hz                                        */
  SND_RING_E6,        /* 2637.0 Hz                                        */
  SND_RING_F6,        /* 2793.7 Hz                                        */
  SND_RING_FS6,       /* 2959.9 Hz                                        */
  SND_RING_G6,        /* 3135.9 Hz                                        */
  SND_RING_GS6,       /* 3322.4 Hz                                        */
  SND_RING_A7,        /* 3520.0 Hz                                        */
  SND_RBACK,          /* Ring back (audible ring)                         */
  SND_BUSY,           /* Busy tone                                        */
  SND_INTERCEPT_A,    /* First tone of an intercept                       */
  SND_INTERCEPT_B,    /* Second tone of an intercept                      */
  SND_REORDER_TONE,   /* Reorder                                          */
  SND_PWRUP,          /* Power-up tone                                    */
  SND_OFF_HOOK_TONE,  /* Off-hook tone, IS-95 (CAI 7.7.5.5)               */
  SND_CALL_WT_TONE,   /* Call-waiting tone                                */
  SND_DIAL_TONE_TONE, /* Dial tone                                        */
  SND_ANSWER_TONE,    /* Answer tone                                      */
  SND_HIGH_PITCH_A,   /* 1st High pitch for IS-54B alerting               */
  SND_HIGH_PITCH_B,   /* 2nd High pitch for IS-54B alerting               */
  SND_MED_PITCH_A,    /* 1st Medium pitch for IS-54B alerting             */
  SND_MED_PITCH_B,    /* 2nd Medium pitch for IS-54B alerting             */
  SND_LOW_PITCH_A,    /* 1st Low pitch for IS-54B alerting                */
  SND_LOW_PITCH_B,    /* 2nd Low pitch for IS-54B alerting                */
  SND_TEST_ON,        /* Test tone on                                     */
  SND_MSG_WAITING,    /* Message Waiting Tone                             */
  SND_PIP_TONE_TONE,  /* Used for Pip-Pip-Pip-Pip (Vocoder) Tone          */
  SND_SPC_DT_INDIA,   /* Used for India's Special Dial Tone               */
  SND_SIGNAL_INDIA,   /* Used in Various India Signalling Tones           */
  SND_DT_TONE_INDIA,  /* Used for India's Normal Dial Tone (and others)   */
  SND_DT_TONE_BRAZIL, /* Used for Brazil's Dial Tone                      */
  SND_DT_DTACO_TONE,  /* Used for DTACO's single tone (350Hz, 350Hz)      */
#ifdef OKI_CODEC // added by happykid in 2000.6.27
///OKI codec dependent value
	SND_400_TONE , 
	SND_440_TONE , 
	SND_480_TONE , 
	SND_667_TONE , 
	SND_800_TONE , 
	SND_1000_TONE , 
	SND_1200_TONE , 
	SND_1300_TONE , 
	SND_1477_TONE , 
	SND_1633_TONE , 
	SND_2000_TONE , 
	SND_2100_TONE , 
	SND_2400_TONE , 
	SND_2500_TONE , 
	SND_2700_TONE , 
	SND_3000_TONE , 
	SND_1000W_TONE,
	SND_2000W_TONE,
	SND_2730W_TONE,
///OKI codec dependent value
#endif
#if defined (FEATURE_PHONE_VR_HFK)
#error code not present
#endif
#ifdef SND_PARA_SUPPORT
  SND_FREQ,           /* Tone is defined by specified frequency values    */
#endif
                      /* Control tones are defined after the last audible */
                      /* tone.  This ordering is used by the SND task to  */
                      /* determine if a tone is a control tone or not.    */
                      /* Control tones affect the timing and sequence of  */
                      /* sounds, but are always silent. A simple silent   */
                      /* is also a control tone.                          */
                      /* Define a marker to use when checking for a       */
                      /* control tone. Set the marker tone and silence    */
                      /* equal so no un-playable tone is embedded in the  */
                      /* sequence.                                        */
  SND_FIRST_CONTROL_TONE,
  SND_SILENCE = SND_FIRST_CONTROL_TONE,
  SND_SIGNAL_SILENCE, /* Send repeat signal then Silence - no tone at all */
  SND_STOP,           /* Stop playing list (last item in a multi-tone)    */
  SND_RPT,            /* Repeat list (last item in a multi-tone)          */
  SND_RPT1,           /* Repeat the previous tone                         */
  SND_RPT_NOSIGNAL,   /* Repeat list - do not send repeat signal          */
  SND_LOOP_BACK2,     /* Loop back 2 items, use duration as a loop count  */
  SND_LABEL,          /* Label for looping                                */
  SND_BACK_TO_LABEL,  /* Back to label use duration as a loop count       */
#ifdef FEATURE_ENHANCED_RINGS
#error code not present
#endif
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif
  SND_LAST_TONE       /* Use for range checking last tone                 */

} snd_tone_type;

/* Define a single tone/DTMF with a duration
*/
typedef struct {
  snd_tone_type tone;           /* Tone/DTMF to generate        */
  word          duration_ms;    /* Duration in milliseconds     */
} snd_compact_type;

#ifdef SND_PARA_SUPPORT
/* Frequency pair which defines a tone/DTMF 
*/
typedef struct {
  word           freq_hi;        /* High frequency value in hz */
  word           freq_lo;        /* Low frequency value in hz  */
} snd_freq_pair_type;
#endif

/* Define a single tone with duration and possibly the actual frequencies
 * of the DTMF.
 */
typedef struct {
  snd_tone_type      tone;           /* Tone/DTMF to generate        */
  word               duration_ms;    /* Duration in milliseconds     */
#ifdef SND_PARA_SUPPORT
  snd_freq_pair_type freq;           /* Freq pair of tone, used only */
                                     /* if tone is SND_FREQ          */
#endif
} snd_type;

/* Predefined sounds (complex tone sequences like ringing)
*/
typedef enum {          /* Description of sound                 */

  SND_FIRST_SND = -1,   /* Use for range checking last sound    */

  SND_ALERT = 0,                /* 0 -  Ringing for incoming call            */
  SND_WAKEUP,                   /* Wake-up/Power-up sound                    */
  SND_DIAL_TONE,                /* Dial tone                                 */
  SND_DTACO_ROAM_TONE,          /* DTACO roaming dial tone                   */
  SND_RING_BACK,                /* Ring-back sound                           */
  SND_INTERCEPT,                /* Send request intercepted locally          */
  SND_REORDER,                  /* System busy                               */
  SND_BUSY_ALERT,               /* Busy Signal                               */
  SND_CONFIRMATION,             /* Confirmation Tone                         */
  SND_CALL_WAITING,             /* Call Waiting                              */
  SND_ANSWER,                   /* 10 - Answer Tone                          */
  SND_OFF_HOOK,                 /* Off-Hook Warning                          */
  SND_NORMAL_ALERT,             /* "Normal" Alerting                         */
  SND_INTR_GROUP_ALERT,         /* Intergroup Alerting                       */
  SND_SPCL_ALERT,               /* Special/Priority Alerting                 */
  SND_PING_RING,                /* "Ping ring"                               */
  SND_IS54B_LONG_H_ALERT,       /* IS-54B High Long                          */
  SND_IS54B_SS_H_ALERT,         /* IS-54B High Short-short                   */
  SND_IS54B_SSL_H_ALERT,        /* IS-54B High Short-short-long              */
  SND_IS54B_SS2_H_ALERT,        /* IS-54B High Short-short-2                 */
  SND_IS54B_SLS_H_ALERT,        /* 20 - IS-54B High Short-long-short         */
  SND_IS54B_SSSS_H_ALERT,       /* IS-54B High Short-short-short-short       */
  SND_IS54B_PBX_LONG_H_ALERT,   /* IS-54B High PBX Long                      */
  SND_IS54B_PBX_SS_H_ALERT,     /* IS-54B High PBX Short-short               */
  SND_IS54B_PBX_SSL_H_ALERT,    /* IS-54B High PBX Short-short-long          */
  SND_IS54B_PBX_SLS_H_ALERT,    /* IS-54B High PBX Short-long-short          */
  SND_IS54B_PBX_SSSS_H_ALERT,   /* IS-54B High PBX Short-short-short-short   */
  SND_IS53A_PPPP_H_ALERT,       /* IS-53A High Pip-Pip-Pip-Pip Alert         */
  SND_IS54B_LONG_M_ALERT,       /* IS-54B Medium Long                        */
  SND_IS54B_SS_M_ALERT,         /* IS-54B Medium Short-short                 */
  SND_IS54B_SSL_M_ALERT,        /* 30 - IS-54B Medium Short-short-long       */
  SND_IS54B_SS2_M_ALERT,        /* IS-54B Medium Short-short-2               */
  SND_IS54B_SLS_M_ALERT,        /* IS-54B Medium Short-long-short            */
  SND_IS54B_SSSS_M_ALERT,       /* IS-54B Medium Short-short-short-short     */
  SND_IS54B_PBX_LONG_M_ALERT,   /* IS-54B Medium PBX Long                    */
  SND_IS54B_PBX_SS_M_ALERT,     /* IS-54B Medium PBX Short-short             */
  SND_IS54B_PBX_SSL_M_ALERT,    /* IS-54B Medium PBX Short-short-long        */
  SND_IS54B_PBX_SLS_M_ALERT,    /* IS-54B Medium PBX Short-long-short        */
  SND_IS54B_PBX_SSSS_M_ALERT,   /* IS-54B Medium PBX Short-short-short-short */
  SND_IS53A_PPPP_M_ALERT,       /* IS-53A Medium Pip-Pip-Pip-Pip Alert       */
  SND_IS54B_LONG_L_ALERT,       /* 40 - IS-54B Low Long                      */
  SND_IS54B_SS_L_ALERT,         /* IS-54B Low Short-short                    */
  SND_IS54B_SSL_L_ALERT,        /* IS-54B Low Short-short-long               */
  SND_IS54B_SS2_L_ALERT,        /* IS-54B Low Short-short-2                  */
  SND_IS54B_SLS_L_ALERT,        /* IS-54B Low Short-long-short               */
  SND_IS54B_SSSS_L_ALERT,       /* IS-54B Low Short-short-short-short        */
  SND_IS54B_PBX_LONG_L_ALERT,   /* IS-54B Low PBX Long                       */
  SND_IS54B_PBX_SS_L_ALERT,     /* IS-54B Low PBX Short-short                */
  SND_IS54B_PBX_SSL_L_ALERT,    /* IS-54B Low PBX Short-short-long           */
  SND_IS54B_PBX_SLS_L_ALERT,    /* IS-54B Low PBX Short-long-short           */
  SND_IS54B_PBX_SSSS_L_ALERT,   /* 50 - IS-54B Low PBX Short-short-short-shrt*/
  SND_IS53A_PPPP_L_ALERT,       /* IS-53A Low Pip-Pip-Pip-Pip Alert          */
  SND_FADE_TONE,                /* Tone to inform user of a fade             */
  SND_SVC_CHANGE,               /* Inform user of a service area change      */
  SND_HORN_ALERT,               /* Horn alert                                */
  SND_ABRV_REORDER,             /* Abbreviated System busy                   */
  SND_ABRV_INTERCEPT,           /* Abbrev'd Send request intercepted locally */
  SND_ALTERNATE_REORDER,        /* Alternate reorder                         */
  SND_MESSAGE_ALERT,            /* Message Waiting Signal                    */
  SND_ABRV_ALERT,               /* Abbreviated alert                         */
  SND_PIP_TONE,                 /* 60 - Pip Tone (Voice Mail Waiting)        */
#if ((TG==T_I2) && defined(FEATURE_INDIA_WLL))
#error code not present
#endif /* ((TG==T_I2) && defined(FEATURE_INDIA_WLL)) */
#if ((TG==T_I2) && defined(FEATURE_BRAZIL_WLL))
#error code not present
#endif /* ((TG==T_I2) && defined(FEATURE_BRAZIL_WLL)) */
#ifdef FEATURE_STD_WLL
#error code not present
#endif
  SND_ROAM_RING,                /* Ringing option while roaming              */
  SND_SVC_ACQ,                  /* Service acquired sound                    */
  SND_SVC_LOST,                 /* Service lost sound                        */
  SND_SVC_CHNG_MORE_PREF,       /* Change to a more preferred service sound  */
  SND_SVC_CHNG_LESS_PREF,       /* Change to a less preferred service sound  */
  SND_EXT_PWR_ON,               /* External power on sound                   */
  SND_EXT_PWR_OFF,              /* External power off sound                  */
#if (defined(FEATURE_SND_RING_SET_1) || defined(FEATURE_SND_RING_SET_2))
#error code not present
#endif
#if defined(FEATURE_SND_RING_SET_2)
#error code not present
#endif
#if (defined (SND_EXTCOM) || defined (FEATURE_PHONE_VR_HFK))
#error code not present
#endif

#if ((MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI))
  SND_AUDIO_APP_START,
  SND_AUDIO_APP_END,
#endif //#if (MMI_TYPE == MMI_A10)

  SND_LAST_SND                  /* Use for range checking last sound         */

} snd_predef_type;

/* The maximum number of tones in a multi-tone sequence, including stop/repeat.
** Current max is based on 1 silence + 32 DTMF digits + 32 inter-digit
** silences + STOP
*/
#define SND_MAX_MULTI_TONE      66

#ifdef FEATURE_AUXROM
/* The maximum number of tones in a predefined sound. This maximum is based
** on the largest predefined sound that is currently defined, this number has
** to be modified if a sound with more than 54 tones is to be defined.
*/
#define SND_MAX_SOUND_TONE      54
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Header for commands
**      If done_q_ptr is non-null, the command will be put on that queue
**      when the command has finished.  If task_ptr is non-null, the
**      associated task will be signalled when the command has finished.
*/
typedef struct {
  q_link_type         link;           /* Command queue link           */
  q_type              *done_q_ptr;    /* Queue to put on when done    */
  rex_tcb_type        *task_ptr;      /* Task to signal when done     */
  rex_sigs_type       sigs;           /* Signals to signal when done  */
  snd_op_type         cmd;            /* Sound command to perform     */
} snd_hdr_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Analog audio or not-analog audio
*/
typedef struct {
  snd_hdr_type  hdr;                    /* Command Header      */
  boolean       analog_mode;            /* Analog audio on/off */
} snd_alog_packet_type;

/* DTMF Audio path control packet (Obsolete)
*/
typedef struct {
  snd_hdr_type        hdr;              /* Command Header   */
  snd_dtmf_apath_type dtmf_path;        /* DTMF audio path  */
} snd_daud_packet_type;

/* Audio path setting packet
*/
// jwpark 00.10.29 : insert manner mode
typedef struct {
  snd_hdr_type           hdr;           /* Command Header        */
  snd_device_type         device;         /* Audio path            */
  snd_mute_control_type  out_mute_s;    /* Is output path muted? */
  snd_mute_control_type  in_mute_s;     /* Is input path muted?  */
  boolean                manner;        // Is in manner mode?
} snd_path_packet_type;

/* Volume level control packet
*/
typedef struct {
  snd_hdr_type        hdr;              /* Command Header   */
  snd_vol_type        volume;           /* Volume level     */
} snd_vol_packet_type;
 
/* Single tone packet
*/
typedef struct {
  snd_hdr_type        hdr;              /* Command Header              */
  snd_type            tone;             /* Single tone                 */
  snd_device_type      device;            /* Audio path                  */
  snd_class_type      sclass;           /* Which generator to use      */
  snd_vol_type        vol;              /* Tone Volume for all paths   */
  snd_dtmf_apath_type dtmf_path;        /* DTMF tx/local/mute pathing  */
  boolean             click_enable;     /* Flag for clicks vs tones    */ 
} snd_tone_packet_type;

/* Multi-tone packet
*/
typedef struct {
  snd_hdr_type        hdr;              /* Command Header              */
  rex_sigs_type       rpt_sigs;         /* Signals to signal @ SND_RPT */
  snd_type            *multi_ptr;       /* Multi-tone                  */
  snd_device_type      device;            /* Audio path                  */
  snd_class_type      sclass;           /* Which generator to use      */
  snd_vol_type        vol;              /* Tone Volume for all paths   */
  snd_dtmf_apath_type dtmf_path;        /* DTMF tx/local/mute pathing  */
  word                repeat_cnt;       /* Repeat counter              */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif
} snd_multi_packet_type;

/* Pre-defined sound packet
*/
typedef struct {
  snd_hdr_type        hdr;              /* Command Header              */
  rex_sigs_type       rpt_sigs;         /* Signals to signal @ SND_RPT */
  snd_predef_type     sound;            /* Predefined sound            */
  snd_device_type      device;            /* Audio path                  */
  snd_class_type      sclass;           /* Which generator to use      */
  snd_vol_type        vol;              /* Tone Volume for all paths   */
  snd_dtmf_apath_type dtmf_path;        /* DTMF tx/local/mute pathing  */
  word                repeat_cnt;       /* Repeat counter              */
                      /* This can be used to sample a predefined sound */
} snd_predef_packet_type;

#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif

#if (defined(FEATURE_VOICE_MEMO) || defined (FEATURE_PHONE_VR))
#error code not present
#endif

/* Vocoder Gain Type Sound Packet - Used to declare either Mobile to Mobile,
**   or Mobile to Land Gain Adjustments.
*/
typedef enum {
  SND_VGADJUST_M2M,                     /* Mobile to Mobile Call       */
  SND_VGADJUST_M2L                      /* Mobile to Land (e.g. Telco) */
} snd_vgadjust_type;

typedef struct {
  snd_hdr_type    hdr;                  /* Command Header              */
  snd_vgadjust_type voc_gain_adjust;    /* Type of Vocoder Gain Needed */
} snd_vgadjust_packet_type;

/* Car Stereo Mute Control Packet
*/
typedef struct {
  snd_hdr_type   hdr;                   /* Command Header              */
  boolean        if_muted;              /* True=Mute Stereo            */
} snd_stereo_packet_type;

/* Used to declare what kind of filtering Sound is to provide.  Mostly,
** this is used on the External PCM interface, but there is nothing to
** say this must be so.
*/

typedef enum {
  SND_FILTERS_VOICE,    /* Set Filters for Normal Voice Call            */
  SND_FILTERS_DATA      /* Set Filters for Data Call (no filtering)     */
} snd_filters_type;

typedef struct {
  snd_hdr_type     hdr;                  /* Command Header              */
  snd_filters_type filters;              /* Type of Filtering Wanted    */
} snd_filters_packet_type;

#ifdef SND_EXTCOM
#error code not present
#endif /* SND_EXTCOM */

#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif


#ifdef FEATURE_YMU757B_MELODY	// jwpark 00.07.27
typedef struct {
  snd_hdr_type    hdr;
  snd_device_type device;
  snd_class_type  sclass;
  char            *file_name;
  uint16		  *music_buf;
  uint16		  music_id;
  uint16          volume;
  uint16          repeat_cnt;
} snd_music_packet_type;
#endif // FEATURE_YMU757B_MELODY	// jwpark 00.07.27 end
#ifdef FEATURE_YMU759_MELODY	// youandi
typedef struct {
  snd_hdr_type    hdr;
  snd_device_type device;
  snd_class_type  sclass;
  char            *file_name;
  uint8 		  *music_buf;
  uint16		  music_id;
  uint16          volume;
  uint16          repeat_cnt;
} snd_music_packet_type;
#endif // FEATURE_YMU759_MELODY	// youandi end

#ifdef FEATURE_VOICEPLAY // jwpark 00.11.06
typedef struct {
  snd_hdr_type    hdr;
  snd_device_type device;
  snd_class_type  sclass;
  word            volume_lvl;
  byte            *voice_buf;
  word            voice_len;
  word            repeat_cnt;
#ifdef FEATURE_WAVELET
  boolean		  Isfromwaveletbuffer;
#endif // FEATURE_WAVELET
} snd_voice_packet_type;
#endif
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Sound commands
*/
typedef union {
  snd_hdr_type             hdr;     /* Plain header                      */
  snd_alog_packet_type     alog;    /* Analog call audio                 */
  snd_path_packet_type     path;    /* Audio path packet                 */
  snd_daud_packet_type     daud;    /* DTMF audio path packet--Obsolete  */
  snd_vol_packet_type      vol;     /* Volume level packet               */
  snd_tone_packet_type     tone;    /* Single tone packet                */
  snd_multi_packet_type    multi;   /* Multi-tone packet                 */
  snd_predef_packet_type   snd;     /* Pre-defined sound packet          */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif
#if (defined(FEATURE_VOICE_MEMO) || defined (FEATURE_PHONE_VR))
#error code not present
#endif
  snd_vgadjust_packet_type vgadjust;/* Vocoder Gain Adjust (M2M/M2L)     */
  snd_stereo_packet_type   stereo;  /* Car Stereo Mute Packet            */
  snd_filters_packet_type  filters; /* Type of Filtering Wanted          */
#ifdef SND_EXTCOM
#error code not present
#endif
#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif
#ifdef FEATURE_YMU757B_MELODY	    // jwpark 00.07.27
  snd_music_packet_type  music;     // play a music
#endif
#ifdef FEATURE_YMU759_MELODY	    //youandi
  snd_music_packet_type  music;     // play a music
#endif
#ifdef FEATURE_VOICEPLAY // jwpark 00.11.06
  snd_voice_packet_type  voice;
#endif
} snd_packets_type;

#ifdef SND_PARA_SUPPORT
#define MAX_PARAMETRIC_GROUPS    16

typedef struct {
  unsigned char           amplitude;
  unsigned short int      freq_1;
  unsigned short int      freq_2;
  unsigned char           on_time;
  unsigned char           off_time;
  unsigned char           repeat;
  unsigned char           delay;
} parametric_group_type;

typedef struct {
  unsigned char           cadence_cnt;
  unsigned char           num_groups;
  parametric_group_type   groups[MAX_PARAMETRIC_GROUPS];
} parametric_struct_type;
#endif

/* <EJECT> */
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/* <EJECT> */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK */
/* <EJECT> */
/*===========================================================================

FUNCTION snd_task

DESCRIPTION
  This procedure is the entrance procedure for the Sound Task.  It
  contains the main processing loop for the Sound Task which controls
  queue processing and sound generation.

DEPENDENCIES
  snd_array
  snd_path_q

RETURN VALUE
  Does not return

SIDE EFFECTS
  snd_tcb

===========================================================================*/
extern void snd_task
(
  dword ignored
    /* Parameter received from Main Control task - ignored */
    /*lint -esym(715,ignored)
    ** Have lint not complain about the ignored parameter 'ignored' which is
    ** specified to make this routine match the template for rex_def_task().
    */
);
/* <EJECT> */
/*===========================================================================

FUNCTION snd_comp_ctl

DESCRIPTION
  Compander control for IS-55 tests.  Turn the compressor/expander off/on.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void snd_comp_ctl
(
  boolean comp_enable
    /* TRUE=Compandor On, FALSE=Compandor Off */
);
/* <EJECT> */
/*===========================================================================

FUNCTION snd_get_voice_path_out

DESCRIPTION
  Function returns the mute state of the current voice path.

DEPENDENCIES
  None

RETURN VALUE
  Mute state of current voice path.

SIDE EFFECTS
  None

===========================================================================*/
extern snd_mute_control_type snd_get_voice_path_out(void);
/* <EJECT> */
/*===========================================================================

FUNCTION snd_rxfm_mute_ctl

DESCRIPTION
  Temporary mute/unmute of the audio input from the FM receive path.
  The parameter allows multiple requesters to request muting and for the
  audio path to unmute only when all requesters have un-muted.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void snd_rxfm_mute_ctl
(
  snd_mute_type mask,
    /* mask of audio mute request associated with caller */

  boolean mute_audio
    /* True = mute audio, False = un-mute.               */
);
/* <EJECT> */
/*===========================================================================

FUNCTION snd_txfm_mute_ctl

DESCRIPTION
  Temporary mute/unmute of the audio output to the FM transmit path.
  The parameter allows multiple requesters to request muting and for the
  audio path to unmute only when all requesters have un-muted.
  
  Note:  this does not mute DTMF output to the FM transmit path, use
  snd_dtmf_tx_mute_ctl() to mute DTMF output.

DEPENDENCIES
  snd_txfm_mute_override

RETURN VALUE
  None

SIDE EFFECTS
  snd_txfm_mute_override

===========================================================================*/
extern void snd_txfm_mute_ctl
(
  snd_mute_type mask,
    /* mask of audio mute request associated with caller */

  boolean mute_audio
    /* True = mute audio, False = un-mute. */
);
/* <EJECT> */
/*===========================================================================

FUNCTION snd_dtmf_tx_mute_ctl

DESCRIPTION
  Temporary mute/unmute of the DTMF output to the FM transmit path.
  The parameter allows multiple requesters to request muting and for the
  audio path to unmute only when all requesters have un-muted.

DEPENDENCIES
  snd_dtmf_tx_mute_override

RETURN VALUE
  None

SIDE EFFECTS
  snd_dtmf_tx_mute_override

===========================================================================*/
extern void snd_dtmf_tx_mute_ctl
(
  snd_mute_type mask,
    /* mask of audio mute request associated with caller */

  boolean mute_dtmf
    /* True = mute DTMF, False = un-mute DTMF. */
);
/* <EJECT> */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif
/* <EJECT> */
/*===========================================================================

FUNCTION snd_set_voc_gain_adjust

DESCRIPTION
  Certain Gain Adjustments are required depending if the call is
  Mobile-to-Mobile (M2M) or Mobile-to-Land (M2L).  This routine allows
  the caller to specify which type of call we currently have.

DEPENDENCIES
  snd_voc_gain_adjust

RETURN VALUE
  None

SIDE EFFECTS
  Command Packet queued to Sound Task.

===========================================================================*/
extern void snd_set_voc_gain_adjust
(
  snd_vgadjust_type new_voc_gain_adjust
    /* The New Vocoder Gain Type the caller wants */
);
/* <EJECT> */
/*===========================================================================

FUNCTION snd_get_packet

DESCRIPTION
  Get a Sound Command Packet.  Use by callers of snd_cmd().
  
DEPENDENCIES
  snd_init() needs to have initialized the queues.

RETURN VALUE
  A pointer to an available snd_packets_type or NULL if no packets are
  available.  (Note if NULL is returned, ERR() will have ALREADY been
  called to log the problem.)

SIDE EFFECTS
  snd_free_q (and the packet fetched off snd_free_q)

===========================================================================*/
extern snd_packets_type *snd_get_packet( void );
/* <EJECT> */
/*===========================================================================

FUNCTION snd_cmd

DESCRIPTION
  Queue a command for processing by the Sound Task.

DEPENDENCIES
  This routine must not be called until the Sound Task has been started.

RETURN VALUE
  None

SIDE EFFECTS
  snd_cmd_q
  snd_tcb

===========================================================================*/
extern void snd_cmd
(
  snd_packets_type *snd_cmd_ptr
    /* pointer to sound command */
);

#ifdef FEATURE_WAVELET
extern byte* MM_PlayAudio(void);
#endif // FEATURE_WAVELET
#ifdef SND_PARA_SUPPORT
/*===========================================================================

FUNCTION parse_parametric

DESCRIPTION
  Take the parametric alerting structure and create an equivalent multi-tone
  for sound to play.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to a multi-tone.

SIDE EFFECTS
  None.

===========================================================================*/
extern snd_type *parse_parametric (
  parametric_struct_type *para
);
#endif
#endif /* SND_H */

