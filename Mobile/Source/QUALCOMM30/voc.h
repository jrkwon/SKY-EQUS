#include "target.h"
#ifdef MMI_TREX
#include <voc.h>
#else // #ifdef MMI_TREX

#ifndef VOC_H
#define VOC_H
/*===========================================================================

         V O C O D E R    S E R V I C E S    H E A D E R    F I L E

DESCRIPTION
  This file contains declarations associated with the Vocoder Services.

REFERENCES
  MSM2.2  User's Manual - 93-13433-1 X1
  MSM2300 User's Manual - 93-22436-1 X1
  MSM3000 User's Manual - 93-24207-1 X1

Copyright (c) 1990,1991,1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999,2000      by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/voc.h_v   1.3   07 Aug 2000 20:03:16   ropalsky  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/07/00    ro     Support for FEATURE_VM_MEM_TO_QCP.
            st     Added VOC_ABORT_PB and VOC_END_ABORT_PB status enums, to be
                   used when voice playback is aborted by CP acquire/config.
01/31/00    rm     Merged the following changes from Pantehr Archive -
            sm     Implemented VOC_SAT_CONTROL. Voc_config() supports 2nd parm.
            vl     Changed function prototype to move call to voc_config() 
                   inside voc_capture_start().
            st     Added SAT control functions and definitions, if 
                   FEATURE_VOC_SAT_CONTROL, is defined then ACP will not write
                   any vocoder registers directly. 
08/13/99    vl     Moved #include "msg.h" inside #if defined (VOC_DEBUG) || 
                   defined (VOC_VS_DEBUG) to eliminate msg_file warnings for
                   files which include voc.h.
07/30/99    vl     Removed VOC_ROM definition and voc_rec_init_queue () extern.
07/19/99    vl     Replaced voc_get_config () with voc_get_service_option ().
                   Added direction of playback, added voc_pb_notify_type,
                   added another status code to voc_rec_status_type. Added
                   voc_rx_vp_mute_ctl() function prototype to control muting 
                   of voice prompts.
06/30/99   RPW     Integrated AGC feature with VR/VM features.
05/19/99    vl     Changed voc_count_frames () to return voc_status_type (used
                   to return void).
05/10/99    vl     Added VOC_REC_STAT_INCOMING_CALL to voc_rec_status_type.
04/07/99    vl     Added voc_get_shared_mem_ptr (), fixed comments, 
                   added VOC_REC_STAT_LAST_BUFFER to voc_rec_status_type, 
                   added #include "customer.h".
03/22/99    sm     Support CDMA vocoder in FM mode.
03/04/99    vl     Implemented Voice Record to support Voice Memo (see 
                   FEATURE_VOICE_RECORD) plus misc clean up.
02/09/99    vl     Function voc_config_reacquire and struct voc_rate_set_type
                   are now visible if FEATURE_VOICE_RECORD or 
                   FEATURE_VOICE_PLAYBACK is defined.
                   Function voc_get_config should be visible if 
                   FEATURE_PHONE_VR is defined, not FEATURE_VOICE_PLAYBACK.
12/06/98    vl     Implemented Voice Play back and Voice Capture.
                   See FEATURE_VOICE_PLAYBACK, FEATURE_PHONE_VR, and
                   FEATURE_VS_TASK
12/04/98    sm     Support selectable ROM keyword for backward compatibility.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.
09/09/98    ro     Removed obsolete definitions.
08/22/98    ro     Added VOC_CAP_IS127 and VOC_CAP_IS733 to
                   voc_capability type.
                   Changed VOC_CAP_EXTERNAL to VOC_CAP_MAX.
                   Added voc_capability_supported function.
                   Updated Copyright.
08/18/98    ss     Added support for EVRC to voc_capability_type.
06/06/97    ro     Revert to previous voc_change and voc_inform interfaces.
05/23/97    ro     Add parameter to voc_change_function_ptr_type and new
                   voc_inform interface for FEATURE_HFK_PLUS.
01/28/97    ro     Added VOC_ACQ_ACP.
01/23/96   rah     Enabled modified voc_init_analog parameter list.
01/06/97    ro     Added msm_mode_type and voc_set_msm_mode().
                   Added VOC_ACQ_MSM_CHANGE to voc_change_status_type.
12/23/96   rah     Added voc_a_init_type. Modified voc_init_analog parameter 
                   list.  (still needs to be enabled).
01/15/96   dak     Added VOC_ACQ_UI flag.
11/28/95   rah     Added new capabilities. Added 13kbps packet sizes.
10/16/95   rah     Moved voc_codec_pad to vocmux.h.
09/21/95   rah     Added voc_init_digital reference.
08/07/95   rah     Changed sense of MSM2P conditional compile.
08/04/95   rah     Changed ISS2 and GEMINI conditional compiles to MSM2P.
07/20/95   rah     Undef'ed VOCDEBUG. changed Gemini compile conditionals.
                   Moved voc_config reference to common area. added voc_config
                   reference to GEMINI/ISS2 region.
07/12/95   rah     Added ISS2 conditional compile.
07/13/95   rah     Changed conditional compiles. Added codec types.
07/06/95   rah     Added conditional compiles to support VOCMUX module.
05/17/95   jah     Added prototype for voc_codec_pad()
05/10/95   jah     Added MSM 2p support, corrected MtoM Rx Gains, and
                   vocoder window-error status.
03/11/95   jah     Added define for VOC_VOL_SILENCE
12/01/94   jah     Corrected VOC_CFG_IS96A to have 1/4 rate frames
11/09/94   jah     Added prototypes for voc_acquire() and voc_release().
06/28/94   jah     Added voc_version(), voc_capability(), and voc_config().
                   Added missing function headers.  Changed voc_test_cmd()
                   to return the command status instead of the version.
                   Removed function prototypes for internal functions.
05/12/94   gb      Added Vocoder PCM & PKT loopback command interfaces.
04/27/94   gb      Fixed IS96a configuration command.
04/05/94   gb      Added IS96a configuration command.
07/10/92   gb      First DMSS release.
03/04/91   rdb     Updated prior to start of integration with vocoder.
10/23/90   rdb     Created first cut of file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#if defined (VOC_DEBUG) || defined (VOC_VS_DEBUG) 
#include "msg.h"
#endif
#include "customer.h"
#include "target.h"
#include "queue.h"      /* Definitions for queue services                  */

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#undef VOCDEBUG


/*---------------------------------------------------------------------------
                        MSM OPERATING MODE
Used to inform the vocoder driver about the operating mode of the MSM
---------------------------------------------------------------------------*/
/* MSM Operating Mode */
typedef enum {
  VOC_MSM_CHANGE = 0,      /* The opearting mode of the MSM is changing    */
  VOC_MSM_CDMA,            /* The MSM is in CDMA mode                      */
  VOC_MSM_FM               /* The MSM is in FM mode                        */
} voc_msm_mode_type;

/* RF's MSM Mode must be available to the vocoder hardware driver */
extern voc_msm_mode_type voc_msm_mode;

/* <EJECT> */
/*===========================================================================

MACRO VOC_DEBUG_MSG

DESCRIPTION
  Allows leaving certain Debug Messages in Vocoder that can be turned on/off
  Regionally.  This macro is a NOP unless /DVOC_DEBUG is added to the
  compile line for the routines.

PARAMETERS
  fmt Format string for message (printf style).  Note, this parameter
      must be a string literal (e.g. "Tuned to %lx"), not a variable.
  p1  1st parameter for format string
  p2  2nd parameter for format string
  p3  3rd parameter for format string

DEPENDENCIES
  Turned on/off by /DVOC_DEBUG at compile time.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef VOC_DEBUG
/* VOC_DEBUG is meaningless unless at we are compiling with at least
** MSG_LVL_FATAL.  Force either no VOC_DEBUG or else MSG_LVL_FATAL.
*/
#if MSG_LEVEL > MSG_LVL_FATAL
#error /DVOC_DEBUG Requires at least MSG_LVL_FATAL
#endif
#define VOC_DEBUG_MSG(fmt,p1,p2,p3)     MSG_FATAL(fmt,p1,p2,p3)
#else
#define VOC_DEBUG_MSG(fmt,p1,p2,p3)
#endif


/* <EJECT> */
/*===========================================================================

MACRO VOC_VS_DEBUG_MSG

DESCRIPTION
  Allows leaving certain Voice Services Debug Messages in Vocoder that can 
  be turned on/off Regionally.  This macro is a NOP unless /DVOC_VS_DEBUG is 
  added to the compile line for the routines.

PARAMETERS
  fmt Format string for message (printf style).  Note, this parameter
      must be a string literal (e.g. "Tuned to %lx"), not a variable.
  p1  1st parameter for format string
  p2  2nd parameter for format string
  p3  3rd parameter for format string

DEPENDENCIES
  Turned on/off by /DVOC_VS_DEBUG at compile time.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef VOC_VS_DEBUG
/* VOC_VS_DEBUG is meaningless unless at we are compiling with at least
** MSG_LVL_FATAL.  Force either no VOC_VS_DEBUG or else MSG_LVL_FATAL.
*/
#if MSG_LEVEL > MSG_LVL_FATAL
#error /DVOC_VS_DEBUG Requires at least MSG_LVL_FATAL
#endif
#define VOC_VS_DEBUG_MSG(fmt,p1,p2,p3)     MSG_FATAL(fmt,p1,p2,p3)
#else
#define VOC_VS_DEBUG_MSG(fmt,p1,p2,p3)
#endif


/*===========================================================================

FUNCTION VOC_MSM_MODE

DESCRIPTION
  This function informs the vocoder about the operating mode of the MSM

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The vocoder may be reset

===========================================================================*/
extern void voc_set_msm_mode( voc_msm_mode_type msm_mode );

/*---------------------------------------------------------------------------
                        VOCODER DATA RATE CONSTANTS

The following constants are used with the 'rate' fields of vocoder transmit
and receive command and response packets.
---------------------------------------------------------------------------*/

typedef enum  
{
  VOC_0_RATE = 0,      /* Indicates vocoder data was blanked. */

  VOC_8_RATE,          /* Indicates rate 1/8 vocoder data. */
  VOC_4_RATE,          /* Indicates rate 1/4 vocoder data. */
  VOC_2_RATE,          /* Indicates rate 1/2 vocoder data. */
  VOC_1_RATE,          /* Indicates rate 1   vocoder data. */
#ifndef T_MSM2P
  VOC_ERASURE = 14,    /* Indicates frame erasure          */
  VOC_ERR_RATE = 15    /* Indicates invalid vocoder data.  */
#else
  VOC_ERASURE,         /* Indicates frame erasure          */
  VOC_ERR_RATE         /* Indicates invalid vocoder data.  */
#endif
} voc_rate_type;


/*---------------------------------------------------------------------------
                        VOCODER DATA BIT COUNTS

The following constants specify the number of bits used per vocoder frame
at the various data rates.
---------------------------------------------------------------------------*/

#define  VOC_8_BITS  16    /* Data rate 1/8:  16 bits or  2 bytes. */
#define  VOC_4_BITS  40    /* Data rate 1/4:  40 bits or  5 bytes. */
#define  VOC_2_BITS  80    /* Data rate 1/2:  80 bits or 10 bytes. */
#define  VOC_1_BITS  171   /* Data rate 1  : 171 bits or 22 bytes. */

#define  VOC_BITS_SIZ  ((VOC_1_BITS + 5) / 8)

#define  VOC13K_8_BITS  20    /* Data rate 1/8:  20 bits or  3 bytes. */
#define  VOC13K_4_BITS  54    /* Data rate 1/4:  40 bits or  6 bytes. */
#define  VOC13K_2_BITS  124   /* Data rate 1/2: 124 bits or 15 bytes. */
#define  VOC13K_1_BITS  266   /* Data rate 1  : 266 bits or 34 bytes. */

#define  VOC13K_BITS_SIZ  ((VOC13K_1_BITS + 6) / 8)
  /* Specifies the number of bytes required to hold the vocoder bits at
     the highest data rates. */


/* Vocoder Volume constants */
#define  VOC_UNITY_GAIN  0x0100     /* Unity gain output */
#define  VOC_VOL_SILENCE 0x0000     /* volume for silence */

/*---------------------------------------------------------------------------
                   RETURN DIAGNOSTIC TEST RESULT COMMAND

This command instructs the vocoder to return the result of the most recently
commanded test.
---------------------------------------------------------------------------*/

typedef enum {
  VOC_RESULT_PASS         = 0x87, /* All tests passed         */
  VOC_RESULT_RAM_ALU_PASS = 0x86, /* RAM and ALU tests passed */
  VOC_RESULT_ROM_ALU_PASS = 0x85, /* ROM and ALU tests passed */
  VOC_RESULT_ALU_PASS     = 0x84, /* ALU test passed          */
  VOC_RESULT_ROM_RAM_PASS = 0x83, /* ROM and RAM tests passed */
  VOC_RESULT_RAM_PASS     = 0x82, /* RAM test passed          */
  VOC_RESULT_ROM_PASS     = 0x81, /* ROM test passed          */
  VOC_RESULT_FAIL         = 0x80  /* All tests failed         */
} voc_result_type;


/*---------------------------------------------------------------------------
                        VOCODER SERVICES RETURN VALUES

The following values can be returned by the Vocoder Services.
---------------------------------------------------------------------------*/

typedef enum
{
  VOC_DONE_S = 0,
    /* Indicates that command/response exchange was performed successfully. */

  VOC_BADFMT_S,
    /* Indicates that a format error was detected during the command/response
       exchange with the vocoder. For example, the response packet was not
       of the correct length. */

  VOC_BADCHK_S,
    /* Indicates that a packet checksum error was detected during the 
       command/response exchange with the vocoder. */

  VOC_TIMEOUT_S,
    /* Indicates that a timeout occurred while waiting for the vocoder
       response. */

  VOC_WINDOW_ERR_S,
    /* Indicates that a command was attempted outside of its allowed timing
       window. */

  VOC_CMD_FAILED_S,
    /* Indicates that the Vocoder Services was not able to execute the 
       command. */

  VOC_MAX_S
    /* For determining number of status codes. This item MUST appear
       last in enum list. */

} voc_status_type;

/*---------------------------------------------------------------------------
                        VOCODER CAPABILITY VALUES

The following are the capabilities of the Vocoder for a particular target
They are returned as a mask from voc_capability() and pass as single values
to voc_config() to configure a particular mode.  An enum is used so that
type checking can be performed.
---------------------------------------------------------------------------*/
typedef enum {
  VOC_CAP_NONE   = 0,              /* Initialization val */
  VOC_CAP_IS96   = (1 << 0),       /* IS-96  - QCELP-8K  */
  VOC_CAP_IS96A  = (1 << 1),       /* IS-96A - QCELP-8K  */
  VOC_CAP_IS733  = (1 << 2),       /* IS-733 - QCELP-13K */
  VOC_CAP_ISXXXX = (1 << 2),       /* Old name for 13k   */
  VOC_CAP_IS127  = (1 << 3),       /* IS-127 - EVRC-8K   */
  VOC_CAP_EVRC   = (1 << 3),       /* Old name for EVRC  */
  VOC_CAP_ANALOG = (1 << 4),       /* ANALOG */
  VOC_CAP_MAX                      /* External and undefined */
} voc_capability_type;
/*---------------------------------------------------------------------------
                        VOCODER ACQUISITION VALUES

The following are the users of the Vocoder.  To use the vocoder, it is
acquired, used, and then later released.
---------------------------------------------------------------------------*/

typedef enum {
  VOC_ACQ_CDMA = 0,
  VOC_ACQ_ACP,
  VOC_ACQ_SND,
  VOC_ACQ_VOC,
  VOC_ACQ_MUX,
  VOC_ACQ_UI,
#ifdef OKI_CODEC // jwpark 00.11.09
  VOC_ACQ_CODEC,
#endif
#ifdef FEATURE_VOICEPLAY // jwpark 00.11.06
  VOC_ACQ_VS,
#endif


#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */

  VOC_MAX_ACQUIRE_TYPE  /* When >16 change the type of voc_acquire_mask */
} voc_acquire_type;

/*---------------------------------------------------------------------------
                        VOCODER CHANGE VALUES

Voc_acquire is passed a pointer to a function to call when the vocoder goes
through a change of status.  Below are the statuses and a type definition
for the function pointer.
---------------------------------------------------------------------------*/

typedef enum {
  VOC_ACQ_RE_INIT_FM,             /* Finished (re)initialization into FM */
  VOC_ACQ_RE_INIT_CDMA,           /* Finished (re)initialization into CDMA */
  VOC_ACQ_IN_RESET,               /* Entered reset               */
  VOC_ACQ_MSM_CHANGE              /* The operating mode of the   */
                                  /* MSM is about to change      */
#ifdef FEATURE_VOC_SAT_CONTROL
  ,VOC_ACQ_SAT_DCC                /* Vocoder received SAT DCC update   */
#endif /* FEATURE_VOC_SAT_CONTROL */
} voc_change_status_type;

/* A pointer to a function returning void, called with 1 parameter of
** type voc_change_status_type.
*/
typedef void
  (*voc_change_function_ptr_type)( voc_change_status_type new_status );

#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif /* FEATURE_AUDIO_AGC */

#ifdef FEATURE_VOC_SAT_CONTROL
/*---------------------------------------------------------------------------
                        VOCODER SAT VALUES
---------------------------------------------------------------------------*/

/* SAT control values */
typedef enum {
  VOC_SAT_ON,
  VOC_SAT_OFF
} voc_sat_ctrl_type;

/* The allowable SCC values */
typedef enum {
  VOC_SCC_5970,
  VOC_SCC_6000,
  VOC_SCC_6030,
  VOC_SCC_INVALID
} voc_scc_type;

#endif /* FEATURE_VOC_SAT_CONTROL */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/* <EJECT> */
/*===========================================================================

FUNCTION VOC_CONFIG

DESCRIPTION
  This function configures an Vocoder to a specific configuration option.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_config
(
  voc_acquire_type key,
  voc_capability_type option  /* option to configure */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_INFORM

DESCRIPTION
  Inform Vocoder users of a vocoder change of status.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void voc_inform
(
  voc_change_status_type status       /* Status change to inform users of */
);

/*===========================================================================

FUNCTION VOC_ACQUIRE

DESCRIPTION
  Acquire the vocoder for use.  This takes it out of powerdown.
  This does not acquire exclusive access, just access.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void voc_acquire
(
  voc_acquire_type key,  /* What entity wants to acquire the Vocoder */

  voc_change_function_ptr_type voc_chng_routine_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_RELEASE

DESCRIPTION
  Release the vocoder, no longer in use.  This puts it into powerdown,
  if it is no longer being used by any user.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void voc_release
(
  voc_acquire_type key   /* What entity wants to release the Vocoder */
);
/* <EJECT> */
/*===========================================================================

FUNCTION VOC_INIT

DESCRIPTION
  This function intializes the vocoder in various stages from releasing
  reset to intializing the time offsets, filter coefficients and background
  noise estimate.

DEPENDENCIES
  Depends on voc_pwr_on having been called sometime previously. This function
  should be called at regular 20 mSec intervals until it returns a TRUE. Only
  then is the vocoder initialized and ready for voice traffic.

RETURN VALUE
  TRUE when vocoder fully initialized, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean voc_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_REINIT

DESCRIPTION
  This function reinitializes the vocoder background noise measurement.

DEPENDENCIES
  Must be done a few frames after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  The vocoder is commanded to reinitialize the background noise measurement.

===========================================================================*/

extern voc_status_type voc_reinit( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_VOL_CMD

DESCRIPTION
  This function sets the internal vocoder gain.

DEPENDENCIES
  Must be done a few frames after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
The vocoder rx gain is adjusted.

===========================================================================*/

extern voc_status_type voc_vol_cmd
(
  word volume
    /* volume to set to.  VOC_UNITY_GAIN is unity gain */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_TX_CMD

DESCRIPTION
  This function gets the current vocoder frame and sets the rate for the
  next frame.

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  Data rate for current vocoder frame.

SIDE EFFECTS
  Rate of next frame is set.

===========================================================================*/

extern voc_rate_type voc_tx_cmd
(
  voc_rate_type max_rate,             /* Max rate for vocoder next frame */
  voc_rate_type min_rate,             /* Min rate for vocoder next frame */
  byte **data_ptr                     /* Pointer to pointer to data */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_RX_CMD

DESCRIPTION
  This function gives the current vocoder frame to the vocoder.

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_rx_cmd
(
  byte rate,                          /* rate of vocoder frame */
  byte *data_ptr                      /* pointer to frame data */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_FRAME_CMD

DESCRIPTION
  This function sets the rate of the next frame for the vocoder.

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_frame_cmd
(
  voc_rate_type max_rate,       /* max rate for vocoder next frame */
  voc_rate_type min_rate        /* min rate for vocoder next frame */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_RESULT_CMD

DESCRIPTION
  This function gets the results of the self-test from the vocoder.

DEPENDENCIES
  None.

RETURN VALUE
  Result of tests in vocoder.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_result_type voc_result_cmd( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_DTMF_CMD

DESCRIPTION
  This function gives the specified tones to the vocoder to generate for
  the specified duration at the specified volume.

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_dtmf_cmd
(
  word tone1,                  /* first tone to generate */
  word tone2,                  /* second tone to generate */
  word vol,                    /* volume to use */
  word duration                /* duration of tones */
);

#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif /* FEATURE_AUDIO_AGC */

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_VOX_CMD

DESCRIPTION
  This function puts the vocoder in voice operated switch mode ( hands free ).

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_vox_cmd
(
  boolean vox_on                /* TRUE to turn on vox function */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_DPEEK_CMD

DESCRIPTION
  This function gets the results of a peek from the vocoder.

DEPENDENCIES
  None.

RETURN VALUE
  Address of result of peek in vocoder or NULL for failure.

SIDE EFFECTS
  None.

===========================================================================*/

extern byte *voc_dpeek_cmd
(
  word address
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_DPOKE_CMD

DESCRIPTION
  This function pokes the specified bytes to the vocoder.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_dpoke_cmd
(
  word address,                 /* address of poke */
  word data                     /* data of poke */
);

/* <EJECT> */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif
/* <EJECT> */
/*===========================================================================

FUNCTION VOC_RX_MUTE_CMD

DESCRIPTION
  This function mutes the vocoder receive audio path.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_rx_mute_cmd
(
  boolean mute_on                   /* TRUE = mute, FALSE = unmute */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_TX_MUTE_CMD

DESCRIPTION
  This function mutes the vocoder transmit audio path.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_tx_mute_cmd
(
  boolean mute_on                   /* TRUE = mute, FALSE = unmute */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_TX_RATE_LIMIT

DESCRIPTION
  This function limits the vocoder transmit rate to ( 2s+1 ) / ( 2s+2 ).

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type voc_tx_rate_limit
(
  word rate_factor                   /* RDR limit from Sysparm msg */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_PCM_LOOP_CMD

DESCRIPTION
  This function sets the PCM loopback state of the vocoder receive audio path.
  This causes the encoder's input samples to be looped back as the decoder's
  output samples.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void voc_pcm_loop_cmd
(
  boolean loop_on
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_PKT_LOOP_CMD

DESCRIPTION
  This function sets the Packet loopback state of the vocoder receive
  audio path.  This causes the Rx packets to loop-back as Tx packets.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void voc_pkt_loop_cmd
(
  boolean loop_on 
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_CAPABILITY

DESCRIPTION
  This function determines the options this Vocoder is capabile of supporting,
  and returns that information to the caller.

DEPENDENCIES
  None.

RETURN VALUE
  Mask of options the Vocoder is capable of supporting.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_capability_type voc_capability( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_CAPABILITY_SUPPORTED

DESCRIPTION
  This function checks whether the vocoder supports the
  desired service option.

DEPENDENCIES
  The version information must be initialized by a call to
  voc_test_cmd() prior to calling this routine.

RETURN VALUE
  TRUE - if the service option is supported
  FALSE - if the service option is not supported

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean voc_capability_supported( voc_capability_type );

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_VERSION

DESCRIPTION
  This function acquires version information from the Vocoder

DEPENDENCIES
  None.

RETURN VALUE
  The major and minor vocoder version.

SIDE EFFECTS
  None.

===========================================================================*/

extern void voc_version
(
  PACKED byte *major,         /* Major vocoder version return value */
  PACKED byte *minor          /* Minor vocoder version return value */
);

/* <EJECT> */

/* Codec type initialization */
typedef enum
{
  VOC_08_MUL_08 = 0,   /*  8-bit mu law */
  VOC_08_MUL_16,       /* 16-bit mu law */
  VOC_13_LIN_16        /* 16-bit linear */
}voc_codec_type;

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_DEVICE_INIT

DESCRIPTION
  Non-mode-specific initialization of the Vocoder Core

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void voc_device_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_INIT_ANALOG

DESCRIPTION
  This function takes care of initializing the vocoder into analog.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Chipx8 will be turned on if it is off.

===========================================================================*/
/* Voice/Data Analog initialization spec. */
typedef enum
{
  VOC_NORMAL_DEV,  
  VOC_DATA_DEV
}voc_a_init_type; 

#ifdef FEATURE_VOC_SAT_CONTROL
void voc_init_analog
(
  voc_a_init_type init_type,
  voc_change_function_ptr_type sat_dcc_cb
);
#else

extern void voc_init_analog( voc_a_init_type init_type );

#endif /* FEATURE_VOC_SAT_CONTROL */

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_SHUTDOWN_ANALOG

DESCRIPTION
  This function takes care of shutting down the vocoder into analog.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void voc_shutdown_analog( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_INIT_DIGITAL

DESCRIPTION
  This function takes care of initializing the vocoder into CDMA.

DEPENDENCIES
  Chipx8 must be present in order for Vocoder initialization to happen.

RETURN VALUE
  None

SIDE EFFECTS
  The Vocoder is reset

===========================================================================*/
extern void voc_init_digital( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_GET_SERVICE_OPTION

DESCRIPTION
  This function returns the current service option.

DEPENDENCIES
  None

RETURN VALUE
  Current service option.

SIDE EFFECTS
  None

===========================================================================*/

extern voc_capability_type voc_get_service_option (void);

/* <EJECT> */
#if defined (FEATURE_VOICE_PLAYBACK) || defined (FEATURE_VOICE_RECORD) 
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK || FEATURE_VOICE_RECORD */

/* <EJECT> */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK */

/* <EJECT> */
#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */

/* <EJECT> */
#ifdef FEATURE_VM_MEM_TO_QCP
#error code not present
#endif /* FEATURE_VM_MEM_TO_QCP */

/* <EJECT> */
#if defined (FEATURE_VOICE_RECORD) || defined (FEATURE_PHONE_VR)
#error code not present
#endif /* FEATURE_PHONE_VR || FEATURE_VOICE_RECORD */

#ifdef FEATURE_VOICE_RECORD
#error code not present
#endif /* FEATURE_VOICE_RECORD */

#ifdef FEATURE_VOC_SAT_CONTROL
/* <EJECT> */
/*===========================================================================

FUNCTION VOC_DFM_SAT_CTRL 

DESCRIPTION
  Enables or disables SAT transpond.

DEPENDENCIES
  None

RETURN VALUE
  VOC_DONE_S - SAT transpond was successfully enabled or disabled.
  VOC_CMD_FAILED_S - Attempt to enabled or disable SAT transpond failed.
  VOC_BADFMT_S - voc_sat_ctrl_type parameter was invalid.

SIDE EFFECTS
  SAT transpond may be enabled or disabled.

===========================================================================*/
extern voc_status_type voc_dfm_sat_ctrl
(
  voc_sat_ctrl_type sat_mode
  /* Indicates whether SAT should be enabled or disabled */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_DFM_SAT_SET_ECC

DESCRIPTION
  Configures the SAT Expected Color Code (ECC) to be used.

DEPENDENCIES
  None

RETURN VALUE
  VOC_DONE_S - SAT ECC was successfully configured.
  VOC_CMD_FAILED_S - Attempt to configure SAT ECC failed.
  VOC_BADFMT_S - voc_scc_type parameter was invalid.

SIDE EFFECTS
  The SAT ECC may be configured.

===========================================================================*/
extern voc_status_type voc_dfm_sat_set_ecc
(
  voc_scc_type ecc
  /* SAT Expected Color Code value to be used */ 
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_DFM_SAT_GET_DCC

DESCRIPTION
  Retrieves the SAT Detected Color Code (DCC).

DEPENDENCIES
  None

RETURN VALUE
  voc_scc_type - the SAT DCC

SIDE EFFECTS
  None

===========================================================================*/
extern voc_scc_type voc_dfm_sat_get_dcc
(
  void
);
#endif /* FEATURE_VOC_SAT_CONTROL */

/* include multiplex module */
/*---------------------------------------------------------------------------
                      CODEC TYPES TO VOCODER
---------------------------------------------------------------------------*/

#include "vocmux.h"
#endif /* VOC_H */

#endif // #ifdef MMI_TREX