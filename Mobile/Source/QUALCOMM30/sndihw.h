#ifndef SNDIHW_H
#define SNDIHW_H
/*===========================================================================

       S O U N D   I N T E R N A L   H A R D W A R E   R O U T I N E S

DESCRIPTION
  The file SNDIHW.H defines the "generic level hardware interface".  The
  actual routines that drive the hardware are "hidden" in the target
  specific routines SNDHWG1.C (all 1st Generation Phones), SNDHWG2.C
  (all 2nd Generation Phones) and so on.  These routines are for the
  exclusive use of the Sound Driver (SND.C).

Copyright(c) 1995, 1996, 1997 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright(c) 1998, 1999       by QUALCOMM, Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/sndihw.h_v   1.4   17 Sep 2000 18:45:56   nbouchar  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/14/00   nxb     Removed sndhw_init_from_nv().
09/12/00   nxb     Removed SND_ROM.
09/11/00   nxb     Remove references to DTMF_MEMORY and DTMF_CONST
06/30/99  sm/RPW   Integrated AGC feature with VR/VM features.
03/22/99    st     Added snd_rx_mute_ctl() prototype.
03/04/99    st     Removed ringer and vibrator function prototypes (moved to 
                   ring.h), renamed remaining ringer functions with prefix 
                   sndhw_ instead of sndrng_.
01/13/99    st     Added DTMF_CONST and VOL_CONST defines to compile volume
                   and DTMF tables into ROM for ARM targets; added function 
                   prototypes for voice prompting support; moved VOL_MEMORY
                   and VOL_CONST definitions to sndi.h.
12/04/98    sm     Support selectable ROM keyword for backward compatibility.
10/08/98    st     HFK-VR mainlining: modified sndhw_extcom_ctl() funtion 
                   prototype; added funtion prototypes sndhw_extcom_protocol(),
                   sndhw_hfk_phs(), sndhw_hfk_aec() sndhw_hfk_vr_tone().
10/08/98    st     Added sndhw_voc_change_path_set() prototype.
06/22/98    st     Added sndhw_adsp_tone() prototype.
01/16/98    ro     Added sndhw_extcom_ctl().
01/12/98    ro     Added mute parameter to sndhw_stereo_mute.
                   Update copyright.
12/08/97    ha     Added sndrng_click() prototype. 
06/17/97    ro     No longer need Q phone fix for DTMF_MEMORY
                   (Q is a target - T_Q - now)
04/29/97    ro     Added sndhw_hfk_ctl() for support of
                   TGP carkit - "NGP Plus".
04/11/97    ro     Don't need sndhw_voc_acquire to be available to others.
                   Fix definition of DTMF_MEMORY for "Q" phone.
                   Changed sndhw_rxfm_mute_ctl(). to sndhw_rx_mute_ctl().
                   Update dependencies.
03/28/97    ro     Support for TGP. Change sndhw_stop_ring changed to
                   sndrng_stop_ring. Change sndhw_ring to sndrng_ring.
                   Remove prototypes for sndhw_st_mode, sndhw_lpf_mode,
                   sndhw_wbd_mode, sndhw_1270_tone, sndhw_1270_stop_tone,
                   sndhw_splt_ctl.  Only T_G defines ringer table in RAM,
                   Remove T_T and T_MD. Remove extern definition of
                   sndhw_dtmf. Use FEATURE_VIBRATOR.
03/12/97   jjn     Added (TG==T_MD) for module in conditional compile statements
02/12/97    ro     Added sndhw_vibrator_ctl().
01/07/97    ro     Added SND_TUNE_VOLUME to support setting volume levels.
                   Add sndhw_path_timer_ctl.
                   Change interface to sndhw_voc_tone.
                   Change interface to sndhw_ring.
                   Add snd_setup_voice_path
                   Add snd_get_tone_volume
                   Change interface to sndhw_set_apath_out, and
                   sndhw_set_apath_in.
10/14/96    ro     Added (TG==T_T) cases to all #if's that have (TG==T_G)
                   for initial support of TGP build.
07/22/96   fkm     Minor Documentation Tweeks
03/19/96   fkm     Updates Per Code Review
02/16/96   fkm     Tweeks to comments and documentation
02/12/96   fkm     Lint and Documentation Updates.
12/13/95   fkm     Lint Nits
11/30/95   fkm     sndhw_dtmf[] can now move to RAM for some targets
10/11/95   fkm     Defintion for sndhw_voc_acquire() added to this level.
08/22/95   fkm     Added Defintion for sndhw_stereo_mute()
07/06/95   fkm     Documentation Updates Per Code Review
04/24/95   fkm     Changes to Support Gemini.
01/17/95   fkm     Created file.

===========================================================================*/

#include "comdef.h"
#include "snd.h"


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_change_gen

DESCRIPTION
  Turn off Tone Generators, either because they are no longer needed or
  because they have been preempted by a higher priority sound.

DEPENDENCIES
  snd_ring_busy
  snd_voc_busy
  sndhw_aux_busy

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void sndhw_change_gen
(
  snd_gen_type new_gen
    /* Type of Generator that is about to start */
);
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_comp_ctl

DESCRIPTION
  Compander control for IS-55 tests.  Turn the compressor/expander off/on.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void sndhw_comp_ctl
(
  boolean on
    /* True = Compander ON, False = Compander Off */
);
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_dtmf_tx_mute_ctl

DESCRIPTION
  Control mute/unmute of the DTMF output to the FM transmit path.

DEPENDENCIES
  snd_dtmf_tx_mute_ctl() has additional code to allow multiple source
     muting.

RETURN VALUE
  None

SIDE EFFECTS
  None

========================================================================*/
extern void sndhw_dtmf_tx_mute_ctl
(
  boolean mute_audio
    /* True = mute audio, False = un-mute. */
);
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_gen_tone

DESCRIPTION
  This routine is called by the Generic Sound Task Level to actually begin
  the generation of a tone.

DEPENDENCIES
  snd_ring_busy
  snd_voc_busy
  sndhw_aux_busy
  sndhw_adsp_busy
  sndhw_hfk_protocol
  sndhw_hfk_vr_tone_ready

RETURN VALUE
  None.

SIDE EFFECTS
  snd_ring_volume

===========================================================================*/
extern void sndhw_gen_tone
(
  tone_status_type *tone_status_ptr
);
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_init

DESCRIPTION
  Generic Low Level Hardware Initialization for Sound Functions and chips.

DEPENDENCIES
  The Timers and Queues in snd_init() must have been initialized first.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void sndhw_init( void );
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_ring

DESCRIPTION
  Start a DTMF emitting from the ringer.  (See Gemini Ringer Software Design
  Notes above.)

DEPENDENCIES
  snd_ring_busy
  snd_ring_volume
  sndrng_dtmf

RETURN VALUE
  None

SIDE EFFECTS
  Sets snd_ring_busy to indicate the state of the ringer.

===========================================================================*/
extern void sndhw_ring
(
  tone_status_type *tone_status_ptr
    /* Information required to generate tone */
);
/* <EJECT> */
/*===========================================================================

FUNCTION snd_rx_mute_ctl

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
extern void snd_rx_mute_ctl
(
  snd_mute_type mask,
    /* mask of audio mute request associated with caller */

  boolean mute_audio
    /* True = mute audio, False = un-mute.               */
);
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_rx_mute_ctl

DESCRIPTION
  Temporary mute/unmute of the audio input from the CDMA/FM receive path.
  This is the low level hardware interface that actually directs the
  hardware to mute.
  
DEPENDENCIES
  See snd_rxfm_mute_ctl().  Logic exists at that level to allow multiple
  mute requestors.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void sndhw_rx_mute_ctl
(
  boolean mute_audio
    /* True = mute audio, False = un-mute.                              */
);
/* <EJECT> */
/*===========================================================================

REGIONAL FUNCTION sndhw_set_apath_in

DESCRIPTION
  Change input paths

DEPENDENCIES
  The Sound Task must have been started in order to initialize the hardware.
  snd_analog_mode
  snd_tx_muted

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void sndhw_set_apath_in
(
  snd_mute_control_type   mute
    /* audio input path mute state */
);
/* <EJECT> */
/*===========================================================================

REGIONAL FUNCTION sndhw_set_apath_out

DESCRIPTION
  Change output paths

DEPENDENCIES
  The Sound Task must have been started in order to initialize the hardware.
  Cannot be called from interrupt context, cannot be called from any task
  other than the sound task.

RETURN VALUE
  None

SIDE EFFECTS
  snd_crnt_path_out
  snd_voc_acquired

===========================================================================*/
extern void sndhw_set_apath_out
(
    snd_pstate_type *apath_out
    /* audio output path to use */
);
/* <EJECT> */
/*===========================================================================

REGIONAL FUNCTION sndhw_set_volume

DESCRIPTION
  Set the output volume according to the current output path.  This includes
  muting if the output is muted.

DEPENDENCIES
  snd_crnt_apath_out

RETURN VALUE
  None

SIDE EFFECTS
  snd_voc_dtmf_volume

===========================================================================*/
extern void sndhw_set_volume( void );
/* <EJECT> */
#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif
/* <EJECT> */
/*===========================================================================

REGIONAL FUNCTION sndhw_aud_process_ctl

DESCRIPTION
  Set the VOX mode appropriately for the current input/output
  path combination and Analog/non-Analog mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If aud-in or aud-out is muted, the associated switches are set to mute.
  Sets snd_vox_mode.

===========================================================================*/
extern void sndhw_aud_process_ctl( void );
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_compute_adsp_freq

DESCRIPTION
  This function returns the ADSP tone value for a given frequency
  to be played by the ADSP.

DEPENDENCIES

RETURN VALUE
  Returns the a word containing the ADSP tone value.

SIDE EFFECTS
  None.
===========================================================================*/
word sndhw_compute_adsp_freq
(
  int frequency
    /* Frequency for which ADSP tone values are to be computed*/
);
/* <EJECT> */
#ifdef SND_TEST_CODE
/*===========================================================================

FUNCTION sndhw_test_code

DESCRIPTION
  This function provides hardware test code when compiled into a non-production
  version of the system.  This function is hardware dependent.

DEPENDENCIES
  If SND_TEST_CODE is defined, then this routine watches for value poked into
  memory from the incircuit emulator and actives the hardware as indicated
  by the flags.

RETURN VALUE
  None

SIDE EFFECTS
  Values in sndhw_test_block are clear after they are executed.

===========================================================================*/
extern void sndhw_test_code
(
  word *change_mask
    /* Pointer to "main loop" Change Flags */
);
#endif /* SND_TEST_CODE */
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_txfm_mute_ctl

DESCRIPTION
  Temporary mute/unmute of the audio output to the FM transmit path.
  
  Note:  this does not mute DTMF output to the FM transmit path, use
  snd_dtmf_tx_mute_ctl() to mute DTMF output.

DEPENDENCIES
  Multiple Mute Sources is supported at snd_txfm_mute_ctl() level.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void sndhw_txfm_mute_ctl
(
  boolean mute_audio
    /* True = mute audio, False = un-mute.                              */
);
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_path_timer_ctl

DESCRIPTION
  Mark paths ready after a fixed delay period.

DEPENDENCIES
  SND_CAR_KIT_ON_TIME
  SND_EARPIECE_ON_TIME
  snd_path_timer

RETURN VALUE
  None

SIDE EFFECTS
  snd_path_is_ready

===========================================================================*/
extern void sndhw_path_timer_ctl( void );
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_voc_tone

DESCRIPTION
  Send a command to the vocoder to have it start generating a tone.

DEPENDENCIES
  snd_analog_mode
  snd_voc_dtmf_volume

RETURN VALUE
  None

SIDE EFFECTS
  Sets snd_voc_busy to indicate the state of the Vocoder.

===========================================================================*/
extern void sndhw_voc_tone
(
  tone_status_type *tone_status_ptr
);
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_codec_tone

DESCRIPTION
  Send a command to the codec to have it start generating a tone.

DEPENDENCIES
  snd_voc_dtmf_volume

RETURN VALUE
  None

SIDE EFFECTS
  Sets snd_voc_busy to indicate the state of the Vocoder.

===========================================================================*/
extern void sndhw_codec_tone
(
  tone_status_type *tone_status_ptr
);
/* <EJECT> */

#ifdef SND_ADSP_TONEGEN
#error code not present
#endif
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_stereo_mute

DESCRIPTION
  Attempt to mute/unmute the Car Stereo as requested by Regional Flags.
  This assumes of course that we are connected to a Car Kit that supports
  stereo mute.

DEPENDENCIES
  sndhw_ext_com_state
  sndhw_hfk_protocol

RETURN VALUE
  None

SIDE EFFECTS
  sndhw_stereo_mute_s
  extcom_stereo_mute_s

===========================================================================*/
extern void sndhw_stereo_mute( boolean mute );
/* <EJECT> */
#ifdef SND_HFK_PLUS
#error code not present
#endif /* SND_HFK_PLUS */
/* <EJECT> */
#ifdef SND_EXTCOM
#error code not present
#endif /* SND_EXTCOM */
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_voc_change_path_set

DESCRIPTION
  This routine does a path set and possibly signals the IDS task after the
  vocoder state has changed to VOC_ACQ_RE_INIT_S.

DEPENDENCIES
  extcom_task_ptr
  extcom_sigs
  sndhw_ext_com_state

RETURN VALUE
  None

SIDE EFFECTS
  sndhw_ext_com_state

===========================================================================*/
extern void sndhw_voc_change_path_set( void );
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_filters_ctl

DESCRIPTION
  Some targets allow different types of "Filtering" to be done by Sound.
  For instance, most data calls need VOXing turned off.

DEPENDENCIES
  snd_current_filters

RETURN VALUE
  None

SIDE EFFECTS
  snd_current_filters

===========================================================================*/
extern void sndhw_filters_ctl
(
  snd_filters_type filters
    /* Type of Filtering Current Desired */
);
/* <EJECT> */
/*===========================================================================
FUNCTION snd_setup_voice_path

DESCRIPTION
  Setup the state of an audio output path. This is for voice path only.

DEPENDENCIES
  snd_voice_volume
  snd_ctl_phone_voice
  snd_ctl_ext_voice
  snd_ctl_priv_voice

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
// jwpark 00.10.29 : insert manner mode
void snd_setup_voice_path
( 
  snd_device_type       device,       /* Setting for physical path */
  snd_mute_control_type mute,         /* Mute setting              */
  boolean               manner,       // Is in manner mode?
  snd_pstate_type       *path_state   /* Output: path state        */
);
/* <EJECT> */
/*===========================================================================
FUNCTION snd_get_tone_volume

DESCRIPTION
  This routine sets up the volume and volume control information in 
  the tone status.

DEPENDENCIES
  snd_voice_volume
  snd_ctl_phone_beep
  snd_ctl_phone_voice
  snd_ctl_phone_voice_beep
  snd_ctl_ext_beep
  snd_ctl_ext_voice
  snd_ctl_ext_voice_beep
  snd_ctl_priv_beep
  snd_ctl_priv_voice
  snd_ctl_priv_voice_beep

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void snd_get_tone_volume
( 
  snd_device_type   device,
  snd_status_type  *snd_ptr,
  tone_status_type *tone_status_ptr
);
/* <EJECT> */
/*===========================================================================

FUNCTION sndhw_which_codec

DESCRIPTION
  Setup the Phone based on the output path.  Hardware will use the Internal
  or External Codec as required.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Volume is muted, and needs to be reset. The volume is not set in this
  routine because the vocoder may be resetting.

===========================================================================*/
//LOCAL void sndhw_which_codec
//(
//  snd_device_type device_out
//    /* Which Audio Path to Setup */
//);
/* <EJECT> */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PROMPT */
#endif /* SNDIHW_H */

