#ifndef VOCMUX_H
#define VOCMUX_H
/*===========================================================================

  G L O B A L  V O C O D E R    S E R V I C E S    H E A D E R    F I L E

DESCRIPTION
  This file contains declarations associated with the external common
  Vocoder Services.

REFERENCES
  MSM2.2  User's Manual - 93-13433-1 X1
  MSM2300 User's Manual - 93-22436-1 X1
  MSM3000 User's Manual - 93-24207-1 X1

Copyright (c) 1995, 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/vocmux.h_v   1.2   31 Jan 2000 14:32:08   rmalhotr  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/31/00    rm     Merged the following changes from Panther archive -
            sm     Integrated MSM3100's FEATURE_VOC_SAT_CONTROL.
                   Voc_config() supports 2nd parm.
            vl     Changed voc_pb_resume_ptr to return boolean value.  Added
                   vmux_pb_end ().
07/19/99    vl     Added vmux_pb_notify (), moved voc_pb_register to vmux level
                   (part of voice answering machine feature implementation).
06/30/99  sm/RPW   Integrated AGC feature with VR/VM features.  MSM3000 only;
                   removed VOC_EXT_COM_ENA; added Q2 support (st).
04/06/99    sm     Added MSM3000-B support.
04/21/99    vl     Changed voc_rec_pause & voc_rec_resume to vmux_rec_pause &
                   vmux_rec_resume.  Added parameter to vmux_rec_resume().
04/07/99    vl     Added #include customer.h, removed #include vocsup.h, 
                   removed vocsup defines.
03/08/99    sm     Remove pre MSM2.2B0 support.
03/04/99    vl     Implemented Voice Record to support Voice Memo (see 
                   FEATURE_VOICE_RECORD) plus misc clean up.
12/19/98    vl     Implemented Voice Playback and Voice Capture.
                   See FEATURE_VOICE_PLAYBACK and FEATURE_PHONE_VR.
12/17/98    sm     Fixes to support disabling of EVRC by undefing FEATURE_EVRC
11/04/98    sm     Merged ARM/MSM3000 support to mainline.
10/01/98    st     Added code to support use of different filters depending on
                   the codec being used.
09/07/98    ss     Changed global variables to be in 'near' in attempt to save
                   code space.
08/18/98    ss     Added support for MSM2310, dynamic download, and EVRC.
04/02/98    ha     Added T_O Support.
03/25/98    ro     Added voc_get_status().
01/16/98    ro     Added VOC_EXT_COM_ENA to voc_aux_dout_src_type to support
                   communications to devices connected to the AUX PCM port.
                   Update copyright.
11/10/97   udp     Added MSM2.3 support for A4
10/01/97   udp     Added MSM2.3 support for voc_msm_version_type
09/18/97    ro     If FEATURE_HFK_PLUS, then no VOC_EXT13K.
06/01/97    ro     Configure for Q Phone (T_Q)
05/23/97    ro     Add parameter to voc_release.
05/08/97    ro     Added voc_put_pcmpad and voc_get_pcmpad, conditionally
                   compiled with FEATURE_HFK_PLUS.
                   Define VOC_EXT13K to make support of external 13k
                   vocoder conditional.
03/14/97   jjn     Added (TG==T_MD) where ever appropriate for the Module
01/20/97   jah     Configured for TGP (T_T)
12/23/96   rah     Removed voc_osc_ctl call reference.
09/25/96   rah     Bracketed MSM version flags for LINT.
08/14/96   rah     Changed MSM version enumeration to be independant of MSM
                   version coding. Changed internal/external 8k/13k detection
                   bitmaps.
07/17/96   rah     Added hands free kit types to voc_coeffs_type. Added 
                   structure typedefs for MSM 2 workaround call.
05/20/96   rah     Added voc_osc_ctl call.
05/02/96   rah     Added a new element to msm_major_version_type. Enabled
                   u-law CODEC support for MSM 2.2 B0.
04/16/96   rah     Removed VOC_T_G conditionals from voc_dout_src_type defs.
04/02/96   rah     Added B0ULAW conditional compile for MSM 2.2 B0 u-law
                   CODEC evaluation. Removed voc_codec_select from global
                   call pointers and added a direct reference to call.
                   Removed voc_aux_dout_src reference.
03/27/96   rah     Changed T_VOC_ to VOC_T in target macros. Added MSM target
                   macros.
03/01/96   rah     Added #defines for standardized target support.
02/08/96   jca     Added -esyms to keep Lint from complaining too loudly.
11/28/95   rah     Changed to reflect VOCMUX.C changes to use call pointer
                   control of MSM2 and DSP1617 vocoders.
11/03/95   rah     Removed extra voc_codec_pad reference.
10/16/95   rah     Added variable pre/deemphasis filter support. Added
                   voc_codec_pad reference.
10/09/95   rah     Changed voc8k_init_snd_parms parameter list.
09/21/95   rah     Changed voc_test_cmd reference.
08/17/95   rah     Added voc8k_init_snd_parms reference.
08/04/95   rah     Added voc8k_codec_select reference.
07/20/95   rah     Removed voc_codec_type definition. Removed voc_config
                   reference. Changed conditional compile to GEMINI target
                   only for voc_aux_dout_src function reference.
07/12/95   rah     Added voc_aux_dout_src function.
04/08/95   rah     Created version of file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"
#include "rex.h"
#include "target.h"
#include "voc.h"

/* <EJECT> */
/*===========================================================================

                     DEFINE VALUES FOR UNIT

===========================================================================*/

/**** EVRC / MSM Vocoder Download Handling Flags ****/

#if ( defined(FEATURE_VOC_LYNX) || defined (FEATURE_VOC_PANTHER) )
#define VOC_DOWNLOADABLE_QDSP
#ifdef FEATURE_EVRC
#define VOC_INTERNAL_EVRC
#endif /* FEATURE_EVRC */
#endif

/**** MSM 2.2 B0 u-law workaround ****/

//we don`t use it. (happykid)
//#define B0ULAW

/**** Timing debugging ****/

#undef VOC_LOADTEST

/* Define targets which support two different codecs */
#if (TG == T_T)
#error code not present
#endif

/**** Board level targets ****/

/* positive logic */
#define VOC_T_G      ((TG==T_MD) || (TG == T_G) || (TG==T_O) || (TG==T_Q2))

/* negative logic */
#define VOC_T_NG     ((TG!=T_MD) && (TG!=T_G) && (TG!=T_O) && (TG!=T_Q2))

/* <EJECT> */
/*===========================================================================

                     EXTERNAL COMPATABILITY DEFINES

===========================================================================*/

/* Set -esyms so Lint will not complain about #define re-casts */
/*lint -esym(652,voc_init) */
/*lint -esym(652,voc_reinit) */
/*lint -esym(652,voc_vol_cmd) */
/*lint -esym(652,voc_tx_cmd) */
/*lint -esym(652,voc_rx_cmd) */
/*lint -esym(652,voc_frame_cmd) */
/*lint -esym(652,voc_dtmf_cmd) */
/*lint -esym(652,voc_vox_cmd) */
/*lint -esym(652,voc_dpeek_cmd) */
/*lint -esym(652,voc_dpoke_cmd) */
/*lint -esym(652,voc_rx_mute_cmd) */
/*lint -esym(652,voc_tx_mute_cmd) */
/*lint -esym(652,voc_tx_rate_limit) */
/*lint -esym(652,voc_pcm_loop_cmd) */
/*lint -esym(652,voc_pkt_loop_cmd) */
/*lint -esym(652,voc_put_pcmpad) */
/*lint -esym(652,voc_get_pcmpad) */
/*lint -esym(652,voc_agc_cmd) */
/*lint -esym(652,voc_pb_abort) */
/*lint -esym(652,voc_pb_pause) */
/*lint -esym(652,voc_pb_register) */
/*lint -esym(652,voc_snd_active_status) */

#define voc_init           voc_fn_ptr->voc_init_ptr
#define voc_reinit         voc_fn_ptr->voc_reinit_ptr
#define voc_vol_cmd        voc_fn_ptr->voc_vol_cmd_ptr
#define voc_codec_pad      voc_fn_ptr->voc_codec_pad_ptr
#define voc_tx_cmd         voc_fn_ptr->voc_tx_cmd_ptr
#define voc_rx_cmd         voc_fn_ptr->voc_rx_cmd_ptr
#define voc_frame_cmd      voc_fn_ptr->voc_frame_cmd_ptr
#define voc_dtmf_cmd       voc_fn_ptr->voc_dtmf_cmd_ptr
#define voc_vox_cmd        voc_fn_ptr->voc_vox_cmd_ptr
#define voc_dpeek_cmd      voc_fn_ptr->voc_dpeek_cmd_ptr
#define voc_dpoke_cmd      voc_fn_ptr->voc_dpoke_cmd_ptr
#define voc_rx_mute_cmd    voc_fn_ptr->voc_rx_mute_cmd_ptr
#define voc_tx_mute_cmd    voc_fn_ptr->voc_tx_mute_cmd_ptr
#define voc_tx_rate_limit  voc_fn_ptr->voc_tx_rate_limit_ptr
#define voc_pcm_loop_cmd   voc_fn_ptr->voc_pcm_loop_cmd_ptr
#define voc_pkt_loop_cmd   voc_fn_ptr->voc_pkt_loop_cmd_ptr
#ifdef FEATURE_HFK_PLUS
#error code not present
#endif
#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif /* FEATURE_AUDIO_AGC */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK */
#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */
#ifdef FEATURE_VOC_SAT_CONTROL
#define voc_dfm_sat_ctrl      voc_fn_ptr->voc_dfm_sat_ctrl_ptr
#define voc_dfm_sat_set_ecc   voc_fn_ptr->voc_dfm_sat_set_ecc_ptr
#define voc_dfm_sat_get_dcc   voc_fn_ptr->voc_dfm_sat_get_dcc_ptr
#endif /* FEATURE_VOC_SAT_CONTROL */

/*===========================================================================

                     INTERNAL COMPATABILITY DEFINES

===========================================================================*/

#define vmux_acquire       voc_fn_ptr->voc_acquire_ptr
#define vmux_release       voc_fn_ptr->voc_release_ptr
#define vmux_test_cmd      voc_fn_ptr->voc_test_cmd_ptr
#define vmux_result_cmd    voc_fn_ptr->voc_result_cmd_ptr
#define vmux_version       voc_fn_ptr->voc_version_ptr
#define vmux_config        voc_fn_ptr->voc_config_ptr
#define vmux_init          voc_def_fn_ptr->voc_init_ptr
#define vmux_codec_select  voc_fn_ptr->voc_codec_select_ptr
#if defined (FEATURE_VOICE_PLAYBACK) || defined (FEATURE_VOICE_RECORD)
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK || FEATURE_VOICE_RECORD */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK */
#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */
#ifdef FEATURE_VOICE_RECORD
#error code not present
#endif /* FEATURE_VOICE_RECORD */

/*===========================================================================

                     VOCODER CONFIGURATION FLAGS

===========================================================================*/

#define INTERNAL_08   0x04
#define INTERNAL_13   0x08
#define EXTERNAL_08   0x01
#define EXTERNAL_13   0x02
#define INTERNAL_EVRC 0x20
#define EXTERNAL_EVRC 0x10

#define VOCODER_ERR   0x00

#define MSM_EVRC      (INTERNAL_08 | INTERNAL_13 | INTERNAL_EVRC)
#define MSM22_13K     (INTERNAL_08 | INTERNAL_13)
#define MSM2_8K       INTERNAL_08

/* Default voc_msm_version value before hw_version() is called. */
#define VOC_MSM_NO_VERSION 0xFFFF

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------

                        MUX STATE VARIABLE TYPES

---------------------------------------------------------------------------*/

/* vocoder data rate multiplex state flag */
typedef enum
{
  VOC_08K_S = 0,
  VOC_13K_S
}voc_rate_mux_type;

/* Codec DOUT source */
typedef enum
{
  VOC_ON_BOARD = 0,     /* DOUT_SRC */
  VOC_OFF_BOARD         /* AUX_DOUT_SRC */
}voc_aux_dout_src_type;

/* vocoder pre/deemphasis filter type */
typedef enum
{
  VOC_0DB_HS = 0,
  VOC_3DB_HS,
  VOC_6DB_HS,
  VOC_0DB_HF,
  VOC_6DB_HF,
#ifdef VOC_TWO_CODECS
#error code not present
#endif
  VOC_MAX_COEFF
}voc_coeff_type;

/* MSM 2.2 B0 PCM sample workaround state typedef */
typedef enum
{
  VC_PCM_SAT_POKE_S = 0,
  VC_PCM_98M_POKE_S,
  VC_PCM_13K_POKE_S,
  VC_PCM_98M_NULL_S
}vc_pcm_wa_state_type;

/* MSM 2x peek/poke workaround return type */
typedef struct vc_ver_wa_typ
{
  boolean active;
  byte peek_poke_control;
  byte peek_poke_addr_lsb;
  byte peek_poke_data_msb;
  byte peek_poke_data_lsb;
}vc_msm_ver_w_a_ret_type;

/*---------------------------------------------------------------------------

                        GENERAL CALL TYPEDEF

---------------------------------------------------------------------------*/

typedef struct voc_fn_tdef
{
   boolean (*voc_init_ptr)(void);
   voc_status_type (*voc_reinit_ptr)(void);
   voc_status_type (*voc_vol_cmd_ptr)(word);
   void (*voc_codec_pad_ptr)(byte, byte, voc_coeff_type);
   voc_rate_type (*voc_tx_cmd_ptr)(voc_rate_type, voc_rate_type, byte **);
   voc_status_type (*voc_rx_cmd_ptr)(byte, byte *);
   voc_status_type (*voc_frame_cmd_ptr)(voc_rate_type, voc_rate_type);
   voc_status_type (*voc_dtmf_cmd_ptr)(word, word, word, word);
   voc_status_type (*voc_vox_cmd_ptr)(boolean);
   byte *(*voc_dpeek_cmd_ptr)(word);
   voc_status_type (*voc_dpoke_cmd_ptr)(word, word);
   voc_status_type (*voc_rx_mute_cmd_ptr)(boolean);
   voc_status_type (*voc_tx_mute_cmd_ptr)(boolean);
   voc_status_type (*voc_tx_rate_limit_ptr)(word);
   void (*voc_pcm_loop_cmd_ptr)(boolean);
   void (*voc_pkt_loop_cmd_ptr)(boolean);
   void (*voc_codec_select_ptr)(voc_codec_type);
   void (*voc_acquire_ptr)(void);
   void (*voc_release_ptr)(boolean);
   voc_status_type (*voc_test_cmd_ptr)(void);
   voc_result_type (*voc_result_cmd_ptr)(void);
   void (*voc_version_ptr)(byte *, byte *);
   voc_status_type (*voc_config_ptr)(voc_capability_type);
#ifdef FEATURE_HFK_PLUS
#error code not present
#endif
#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif /* FEATURE_AUDIO_AGC */
#if defined (FEATURE_VOICE_PLAYBACK) || defined (FEATURE_VOICE_RECORD)
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK || FEATURE_VOICE_RECORD */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK */
#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */
#ifdef FEATURE_VOICE_RECORD
#error code not present
#endif /* FEATURE_VOICE_RECORD */
#ifdef FEATURE_VOC_SAT_CONTROL
   voc_status_type (*voc_dfm_sat_ctrl_ptr) (voc_sat_ctrl_type);
   voc_status_type (*voc_dfm_sat_set_ecc_ptr) (voc_scc_type);
   voc_scc_type (*voc_dfm_sat_get_dcc_ptr) (void);
#endif /* FEATURE_VOC_SAT_CONTROL */

}voc_fn_ptr_type;

/*---------------------------------------------------------------------------

                        CALL POINTER REFERENCES

---------------------------------------------------------------------------*/

extern const voc_fn_ptr_type * voc_fn_ptr;
extern const voc_fn_ptr_type * voc_def_fn_ptr;


/* <EJECT> */
/*===========================================================================

FUNCTION VOC_TEST_CMD

DESCRIPTION
  This function commands the vocoder to perform its self-tests.

DEPENDENCIES
  None.

RETURN VALUE
  VOC_DONE_S - Initialization performed successfully or else vocxxk_test_cmd
  return.

SIDE EFFECTS
  Resets the Vocoder, makes it unusable for general use until reset.

===========================================================================*/

extern voc_status_type voc_test_cmd
(
  rex_sigs_type signal,
  rex_timer_type  *timer_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION VOC_CODEC_SELECT

DESCRIPTION
  This function sets the desired vocoder to use a specific codec.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void voc_codec_select
(
  voc_aux_dout_src_type path
);
/* <EJECT> */
/*===========================================================================

FUNCTION VOC_GET_STATUS

DESCRIPTION
  This function returns the current vocoder state.

DEPENDENCIES
  None.

RETURN VALUE
  Curent vocoder state.

SIDE EFFECTS
  None.

===========================================================================*/
voc_change_status_type voc_get_status( void );

#endif /* VOCMUX_H */

