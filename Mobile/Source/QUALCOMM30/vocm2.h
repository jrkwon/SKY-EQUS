#ifndef VOCM2_H
#define VOCM2_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               M S M 2   V O C O D E R   S E R V I C E S
                        H E A D E R  F I L E

DESCRIPTION
  This file contains references for DMSS interaction with the MSM 2 Vocoder.

REFERENCES
  MSM2.2  User's Manual - 93-13433-1 X1
  MSM2300 User's Manual - 93-22436-1 X1
  MSM3000 User's Manual - 93-24207-1 X1

Copyright (c) 1995,1996,1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/vocm2.h_v   1.2   31 Jan 2000 14:31:44   rmalhotr  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/24/00   rm      Merged the following changes from Panther Archive into MSM5000
                   archive-
           sm     Integrated MSM3100's FEATURE_VOC_SAT_CONTROL.
           vl     Changed vocm2_pb_resume () to return boolean value.  Added
                   vocm2_pb_end ().
           st     Added SAT control functions.
07/19/99    vl     Added  vocm2_pb_notify () (part of voice answering machine 
                   feature implementation).
06/30/99   RPW     Integrated AGC feature with VR/VM features.
04/21/99    vl     Added parameter to vocm2_rec_resume().
04/07/99    vl     Added #include "customer.h".
03/04/99    vl     Implemented Voice Record to support Voice Memo (see 
                   FEATURE_VOICE_RECORD) plus misc clean up.
12/03/98    vl     Implemented Voice Playback and Voice Capture to support VR.
                   See FEATURE_VOICE_PLAYBACK and FEATURE_PHONE_VR.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.
09/09/98    ro     Remove vocm2_pwr
01/16/98    ro     Added vocm2_codec_fmt. Update copyright.
10/07/97   udp     Added function vocm2_osc_ctl, used by sleep.c to complete
                   MSM2.3 update.
10/01/97   udp     Added MSM2.3 include of voc.h for searcher
05/23/97    ro     Add config parameter to vocm2_release.
                   Remove extra declaration of voc_config.
                   Add incall parameter to vocm2_init_analog.
                   Update copyright.
05/08/97    ro     Add vocm2_get_pcmpad, vocm2_put_pcmpad, include
                   conditionally with FEATURE_HFK_PLUS.
01/28/97    ro     Added vocm2_init_analog.
11/28/95   rah     Created file from vocm2.c function headers.

===========================================================================*/

#include "customer.h"
#include "comdef.h"
#include "processor.h"
#include "voc.h"

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_ACQUIRE

DESCRIPTION
  Acquire the MSM2P vocoder for use.  This takes it out of powerdown.
  This does not acquire exclusive access, just access.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void vocm2_acquire ( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_RELEASE

DESCRIPTION
  Release the MSM2P vocoder, no longer in use.  This puts it into powerdown,
  if it is no longer being used by any user.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void vocm2_release ( boolean );

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_INIT

DESCRIPTION
  This function intializes the MSM2P vocoder in various stages from releasing
  reset to intializing the time offsets, filter coefficients and background
  noise estimate.

DEPENDENCIES
  ?

RETURN VALUE
  TRUE when vocoder fully initialized, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean vocm2_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_REINIT

DESCRIPTION
  This function reinitializes the MSM2P vocoder background noise measurement.

DEPENDENCIES
  Must be done a few frames after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  The vocoder is commanded to reinitialize the background noise measurement.

===========================================================================*/

extern voc_status_type vocm2_reinit( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_VOL_CMD

DESCRIPTION
  This function sets the MSM2P internal vocoder gain.

DEPENDENCIES
  Must be done a few frames after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
The vocoder rx gain is adjusted.

===========================================================================*/

extern voc_status_type vocm2_vol_cmd
(
  word volume
    /* volume to set to.  VOC_UNITY_GAIN is unity gain */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_TX_CMD

DESCRIPTION
  This function gets the current MSM2P vocoder frame and sets the rate for
  the next frame.

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  Data rate for current vocoder frame.

SIDE EFFECTS
  Rate of next frame is set.

===========================================================================*/

extern voc_rate_type vocm2_tx_cmd
(
  voc_rate_type max_rate,             /* Max rate for vocoder next frame */
  voc_rate_type min_rate,             /* Min rate for vocoder next frame */
  byte **data_ptr                     /* Pointer to pointer to data */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_RX_CMD

DESCRIPTION
  This function gives the current frame to the MSM2P vocoder.

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_rx_cmd
(
  byte rate,                          /* rate of vocoder frame */
  byte *data_ptr                      /* pointer to frame data */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_FRAME_CMD

DESCRIPTION
  This function sets the rate of the next frame for the MSM2P vocoder.

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_frame_cmd
(
  voc_rate_type max_rate,       /* max rate for vocoder next frame */
  voc_rate_type min_rate        /* min rate for vocoder next frame */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_TEST_CMD

DESCRIPTION
  This function commands the vocoder to perform its self-tests.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  Resets the Vocoder, makes it unusable for general use until reset.

===========================================================================*/

extern voc_status_type vocm2_test_cmd( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_RESULT_CMD

DESCRIPTION
  This function gets the results of the self-test from the vocoder.

DEPENDENCIES
  None.

RETURN VALUE
  Result of tests in vocoder.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_result_type vocm2_result_cmd( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_DTMF_CMD

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

extern voc_status_type vocm2_dtmf_cmd
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

FUNCTION VOCM2_VOX_CMD

DESCRIPTION
  This function puts the vocoder in voice operated switch mode ( hands free ).

DEPENDENCIES
  Must be done after initializing the vocoder.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_vox_cmd
(
  boolean vox_on                /* TRUE to turn on vox function */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_DPEEK_CMD

DESCRIPTION
  This function gets the results of a peek from the vocoder.

DEPENDENCIES
  None.

RETURN VALUE
  Address of result of peek in vocoder or NULL for failure.

SIDE EFFECTS
  None.

===========================================================================*/

extern byte *vocm2_dpeek_cmd
(
  word address
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_DPOKE_CMD

DESCRIPTION
  This function pokes the specified bytes to the vocoder.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_dpoke_cmd
(
  word address,                 /* address of poke */
  word data                     /* data of poke */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_RX_MUTE_CMD

DESCRIPTION
  This function mutes the vocoder receive audio path.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_rx_mute_cmd
(
  boolean mute_on                   /* TRUE = mute, FALSE = unmute */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_TX_MUTE_CMD

DESCRIPTION
  This function mutes the vocoder transmit audio path.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_tx_mute_cmd
(
  boolean mute_on                   /* TRUE = mute, FALSE = unmute */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_TX_RATE_LIMIT

DESCRIPTION
  This function limits the vocoder transmit rate to ( 2s+1 ) / ( 2s+2 ).

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_tx_rate_limit
(
  word rate_factor                   /* RDR limit from Sysparm msg */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_PCM_LOOP_CMD

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

extern void vocm2_pcm_loop_cmd
(
  boolean loop_on
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_PKT_LOOP_CMD

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

extern void vocm2_pkt_loop_cmd
(
  boolean loop_on
);
/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_REF_STRB_CMD

DESCRIPTION
  This function forces the alignment of the vocoder reference strobe to the
  time it receives this command.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else voc_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_ref_strb_cmd( void );

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_CONFIG

DESCRIPTION
  This function configures an Vocoder to a specific configuration option.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_config
(
  voc_capability_type option  /* option to configure */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_VERSION

DESCRIPTION
  This function acquires version information from the Vocoder

DEPENDENCIES
  None.

RETURN VALUE
  The major and minor vocoder version.

SIDE EFFECTS
  None.

===========================================================================*/

extern void vocm2_version
(
  byte *major,                /* Major vocoder version return value */
  byte *minor                 /* Minor vocoder version return value */
);
/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_COD_VOL_CMD

DESCRIPTION
  This function sets the codec volume level.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern voc_status_type vocm2_codec_vol_cmd( byte volume );

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_CODEC_PAD

DESCRIPTION
  This function sets the codec volume level.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocm2_cmd return.

SIDE EFFECTS
  None.

===========================================================================*/

extern void vocm2_codec_pad
(
  byte value,                 /* Value for the pad bits */
  byte mask,                  /* Mask of bits to change */
  voc_coeff_type filter       /* pre/deemphasis filter coeffs */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_CONFIG_RATE

DESCRIPTION
  This gateway function configures an Vocoder to a specific rate option.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocxxk_config
  return.

SIDE EFFECTS
  None.

===========================================================================*/
extern void vocm2_config_rate
(
  voc_rate_mux_type option           /* option to configure */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_CODEC_SELECT

DESCRIPTION
  This function selects the CODEC data format used by the vocoder.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void vocm2_codec_select
(
  voc_codec_type codec
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_CODEC_FMT

DESCRIPTION
  This function sets the CODEC data format used by the vocoder.

DEPENDENCIES
  Writes vocoder registers, so vocoder clocks must be enabled.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void vocm2_codec_fmt
(
  voc_codec_type codec
);
/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_INIT_SND_PARMS

DESCRIPTION
  This function sets initial vocoder parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void vocm2_init_snd_parms (void);
/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_INIT_ANALOG

DESCRIPTION
  This function takes care of initializing the vocoder into analog.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Chipx8 will be turned on if it is off.

===========================================================================*/
extern void vocm2_init_analog( voc_a_init_type init_type, boolean incall );
/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_OSC_CTL

DESCRIPTION
  This function enables/disables micro processor to enable/disable XTAL IN/OUT

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  XTAL IN can be disabled by micro processor.

===========================================================================*/
extern void vocm2_osc_ctl( byte value );
/* <EJECT> */
#ifdef FEATURE_HFK_PLUS
#error code not present
#endif /* FEATURE_HFK_PLUS */

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
#ifdef FEATURE_VOICE_RECORD
#error code not present
#endif /* FEATURE_VOICE_RECORD */

#ifdef FEATURE_VOC_SAT_CONTROL
/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_DFM_SAT_CTRL 

DESCRIPTION
  Enables or disables SAT transpond.  SAT DCC update notification is 
  enabled by default.

DEPENDENCIES
  None

RETURN VALUE
  VOC_DONE_S - SAT transpond was successfully enabled or disabled.
  VOC_BADFMT_S - voc_sat_ctrl_type parameter was invalid.

SIDE EFFECTS
  SAT transpond may be enabled or disabled.

===========================================================================*/
voc_status_type vocm2_dfm_sat_ctrl
(
   voc_sat_ctrl_type sat_mode
   /* Indicates whether SAT should be enabled or disabled */
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_DFM_SAT_SET_ECC

DESCRIPTION
  Configures the SAT Expected Color Code (ECC) to be used.

DEPENDENCIES
  None

RETURN VALUE
  VOC_DONE_S - SAT ECC was successfully configured.
  VOC_BADFMT_S - voc_scc_type parameter was invalid.

SIDE EFFECTS
  The SAT ECC may be configured.

===========================================================================*/
voc_status_type vocm2_dfm_sat_set_ecc
(
  voc_scc_type ecc
  /* SAT Expected Color Code value to be used */ 
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCM2_DFM_SAT_GET_DCC

DESCRIPTION
  Retrieves the SAT Detected Color Code (DCC).

DEPENDENCIES
  None

RETURN VALUE
  voc_scc_type - the SAT DCC

SIDE EFFECTS
  None

===========================================================================*/
voc_scc_type vocm2_dfm_sat_get_dcc( void );

#endif /* FEATURE_VOC_SAT_CONTROL */
#endif /* VOCM2_H */

