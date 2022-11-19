#ifndef SNDI_H
#define SNDI_H
/*===========================================================================

    S O U N D   S E R V I C E S   I N T E R N A L   H E A D E R   F I L E

DESCRIPTION
  All the declarations and definitions necessary for the Sound driver to
  interact with the Vocoder interface and the CXD1270/1271, not including
  the specifics defining the Vocoder interface.

REFERENCES
  MSM2.2 User's Manual.

Copyright (c) 1992, 1993, 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1995, 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999, 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/sndi.h_v   1.4   18 Sep 2000 11:10:04   nbouchar  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.08.14   jwpark   YMU757B new API
--------   ---     ----------------------------------------------------------
09/18/00   nxb     Removed references to SND_ROM.
05/04/00   nxb     Added T_O to SND_ROM conditional.
08/13/99    vl     Moved #include "msg.h" inside #ifdef SND_DEBUG
                   to eliminate msg_file warnings in SND files.
06/30/99   RPW     Integrated AGC feature with VR/VM features.
03/22/99    st     Modified snd_status_type; modified SND_MAX_PERIOD macro to
                   remove snd_analog_mode dependency.
03/04/99    st     Incorporated changes to support ha's vocoder frequency 
                   calculation routines: modifed snd_dtmf_type, added 
                   snd_mn_counter_type structure for the ringer, moved 
                   #defines for vocoder tone values in voc_core.h.
                   Also made modifications to support tone generation from
                   specified frequencies: modified tone_status_type. 
01/13/99    st     Added support for voice prompting; added VOL_MEMORY and 
                   VOL_CONST definitions from sndihw.h so that they can be used
                   in snd_vol_ctl_type.
12/04/98    sm     Support selectable ROM keyword for backward compatibility.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.
10/08/98    st     HFK-VR mainlining: Added snd_hfk_vr_tone_state_type, 
                   snd_extcom_command_type, snd_ext_com_state_type, 
                   snd_hfk_aec_set_type, and snd_hfk_phs_set_type
10/08/98    st     Added SND_VERIFY_DFILT_IS_DISABLED macro and DATA_FILTERS
                   bitmask defines for volume override.
08/28/98    st     Added SND_TONE_VOC_TIMEOUT define. CR6033.
06/22/98    st     Added SND_GEN_ADSP in snd_gen_type for ADSP tone generation 
                   on SSS.
04/23/98    st     Added BIO_SW10 defines and macro for DOTG volume override
                   verification and ctl_flags field in snd_vol_ctl_type 
                   structure, fixes CR4888.
02/13/98    ha     Added scaling field to snd_pstate_type for HFK volume 
                   scaling support.
01/21/98    ha     Added scaling field to snd_status_type and 
                   tone_status_type to support volume scaling.
                   Added additional piano notes.
                   Updated copyright.
01/16/98    ro     Added SND_CODEC_VOL_M to support external communications
                   Added snd_device_type. Added device to snd_pstate_type.
                   Added device to tone_status_type.
                   Added snd_audio_process_type.
12/08/97    ha     Added click_state field to snd_status_type and to
                   tone_status_type
11/21/97    ha     Added DS6 tone entry (2489.1Hz)
10/23/97    ro     Change codec volume mask for non-HFK+, fixes CR 4190.
08/10/97    ro     Change SND_HFK_DEFAULT value to unmute car stereo.
07/23/97    ro     Moved ringer definitions to sndring.h
                   Added new tones to snd_dtmf_index_type.
                   Added new vocoder frequency constants.
07/01/97    ro     Remove #if (TG==??) in MSM register definitions.
06/17/97    ro     Added 425Hz vocoder tone.
06/06/97    ro     Get rid of hanging comma in snd_gen_type.
05/23/97    ro     Add phs_override to tone_status_type.
04/29/97    ro     Added control values for HFK PLUS.
04/25/97    ro     Change #if SND_DEBUG to #ifdef SND_DEBUG
03/28/97    ro     Support for TGP.  Add label_index and label_cnt to
                   snd_status_type to support custom alerts.
                   Changed snd_dtmf_type dtmf_ptr to snd_tone_type tone
                   in tone_status_type.
                   Add DEF_DTMF macro for SND_DEBUG builds.
                   Defined additional ringer and vocoder frequencies.
                   Remove all T_C1, no longer supported.
03/12/97   jjn     Added (TG==T_MD) for Module in conditional compile statements
02/12/97    ro     Added support for aux generators - SND_GEN_AUX.
01/07/97    ro     Added SND_FIRST_HFK_TONE_TIME.
                   Added SND_VOLUME_M, removed SND_SPKR_VOL_M, and
                   SND_PHONE_VOL_M.
                   Added snd_vol_ctl_type, snd_pstate_type, tone_status_type
                   Added sclass field to snd_status_type.
                   Changed vol from byte to snd_vol_type in snd_status_type.
                   Added VOC_xxxx_HZ definitions for tones that may now be
                   played through the car kit in addition to the ringer.
12/12/96    ro     Added (TG==T_T) cases to all #if's that have (TG==T_G)
                   for initial support of TGP build.
12/05/96    ro     Change SND_VOC_DTMF_UNITY_GAIN for T_I2 from 0x7fff
                   to 0x2000, by removing T_I2 from #if
09/19/96   fkm     Added SND_VOC_1P5DB_HOT/COLD Definitions
05/29/96   fkm     Added Defintions for SND_CODEC_3DB
04/18/96   fkm     Added SND_CODEC_15DB Definition
03/19/96   fkm     Updates Per Code Review
02/22/96   fkm     Changed index_a/index_b to index_hi/index_lo to help
                   note that High/Low tones belong in certain sides of the
                   DTMF generators on some targets.
02/16/96   fkm     Tweeks to comments and documentation
02/06/96   fkm     Put ()s around Macros to Make Lint Happy
01/17/96   fkm     Added T_C1 Stuff
01/03/96   fkm     Lint Nits
12/21/95   fkm     Removed (TG<T_G)s and added SND_DEBUG_MSG()
12/13/95   fkm     ISS2/India Vocoder and Ringer Values
11/30/95   fkm     Added New Gemini Ringer Values
11/10/95   fkm     Removed RNG_M_START bit from Ringer Table Values
11/02/95   fkm     Added Gemini Vocoder and Codec Gain Definitions
10/11/95   fkm     Added SND_ALLOW_SLEEPING.
07/21/95   fkm     Minor Documentation Updates
07/06/95   fkm     Factory Patch is no longer Automagically Enabled for Gemini
06/19/95   fkm     Factory Patch: Force Car Kit Enable from DM "poke"
06/14/95   fkm     SND_MAX_PERIOD now 25 msec in FM mode (covers for MSM bug).
06/08/95   fkm     Added TIM2 Definitions for Gemini Ringer Support
05/16/95   fkm     Removed "Twist" from Gemini for now and some changes in
                   preparation for ISS2.
04/24/95   fkm     Changes needed for Gemini
08/15/94   jah     Set Vocoder DTMF Unity gain for ISS-1
03/13/94   jah     Changed #if T_SLEEP!=0 to #ifdef T_SLEEP
12/09/93   jah     Made sleep macros conditional on T_SLEEP
04/26/93   jah     Added changes for Beta II mobile.
04/14/93   jah     Changed aud_image[] to snd_audi[]
04/09/93   jah     Added SND_SLEEP_ALLOW/FORBID() macros for CDMA sleep.
02/09/93   jah     Added added DTMF path change to the sound status structure.
01/08/93   jah     Additions for no-qwacking switch between voice and tones.
01/05/93   jah     Changed SND_MAX_PERIOD to account for SND_VOC_GRAIN, which
                   allows Vocoder tones to continue seemlessly.
12/22/92   jah     Added Portable speaker sense/select.
12/09/92   jah     Removed REAL_CLOCK pre-Stretch-1 support.
10/16/92   jah     Added extern's for regional data/routines.  Added the tone
                   generator type for the ringer.
09/15/92   jah     Added SONY_1271_GPIO and SONY_1271_GPIO_BUF.
                   Added stub horn alert for non-mobile environments.
09/03/92   jah     Moved horn alert to proper gpio, fixed SND_NEW_TONE.
09/02/92   jah     Added horn alert macros.
09/01/92   jah     Added VOX_MODE() macro.
08/25/92   jah     Ordered tones to simplify snd_change.
08/20/92   jah     Added SND_VOC_POP_WAIT for waiting for vocoder pop.
08/14/92   jah     Added Vocoder values for keypad DTMFs
07/30/92   jah     A few BIO symbol name corrections for the stretch.
07/27/92   jah     Converted to BIO interface, added changes from code review
04/20/92   jah     Created

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc. */
#include "target.h"     /* Target specific definitions      */
#include "clk.h"        /* Definitions for CLK_MS_PER_TICK  */
#include "nv.h"         /* NV Defintions                    */
#ifdef SND_DEBUG
#include "msg.h"        /* Make sure MSG_LEVEL is defined   */
#endif

#ifdef T_SLEEP
#include "sleep.h"      /* Sleep signal definitions         */
#include "task.h"       /* Sleep TCB declaration            */
#endif

#if (TG!=T_REX)
#include "bio.h"        /* Bit I/O macros                   */
#endif


#ifdef FEATURE_YMU757B_MELODY           // jwpark 00.08.04
#include "YMU757B.h"
#endif // #ifdef FEATURE_YMU757B_MELODY // jwpark 00.08.04 end


#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif

/* <EJECT> */
/*===========================================================================

                            DATA DECLARATIONS

===========================================================================*/
/* To build a version for use in setting volume levels, define this symbol.
** When defined, the volume tables will be placed in RAM where values can
** be modified using the DM.
*/
#ifdef SND_TUNE_VOLUME
#define VOL_MEMORY
#define VOL_CONST
#else
#define VOL_MEMORY
#define VOL_CONST       const   /* const qualifier for ARM builds */
#endif

/* The maximum number of milliseconds between tone updates.
** This limit is imposed by the max interval for a rex timer (16 bits).
**
** The Vocoder's tone generation granularity (units of tone-time) are
** factored in to allow for overlap of continuing tones, instead of
** terminate-restart.
*/
#define SND_VOC_GRAIN   10
#define SND_MAX_PERIOD  (0xFFFF - SND_VOC_GRAIN)

/* The minimum time to play the first keybeep after turning on the caar kit
*/
#define SND_FIRST_HFK_TONE_TIME  150

/* Masks of things which get changed when we process the queue.  Masks are
** used so that we can clear out the queue before we go about any lengthy
** hardware manipulations.  This provides timely processing of the most
** recent commands at the expense of a static sequence of processing changes.
*/
#define SND_NO_CHANGE_M 0
#define SND_AUD_IN_M    0x0001
#define SND_AUD_OUT_M   (SND_AUD_IN_M << 1)
#define SND_AUD_DTMF_M  (SND_AUD_IN_M << 2)
#define SND_VOLUME_M    (SND_AUD_IN_M << 3)
#define SND_RING_VOL_M  (SND_AUD_IN_M << 5)
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#else
#define SND_SOUND_M     (SND_AUD_IN_M << 6)
#define SND_MULTI_M     (SND_AUD_IN_M << 7)
#define SND_TONE_M      (SND_AUD_IN_M << 8)
#endif /* FEATURE_VOICE_PLAYBACK */
/* <EJECT> */
#ifdef SND_HFK_PLUS
#error code not present
#endif /* SND_HFK_PLUS */


#ifdef FEATURE_ENHANCED_RINGS
#error code not present
#endif
/* <EJECT> */
/*===========================================================================

MACRO SND_NEW_TONE

DESCRIPTION
  See if the noise indexed by a snd_array_idx_type is in a change mask
  SND_TONE_M..SND_PLAYBACK_M or SND_TONE_M..SND_SOUND_M must be sequential.

PARAMETERS
  idx   Index into the change mask defined above.
  mask  Mask of current changes.

DEPENDENCIES
  idx must be in the range of masks specified above.

RETURN VALUE
  True  New tone
  False Not a new tone

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#else
#define SND_NEW_TONE(idx,mask)  (((mask) & (SND_SOUND_M << (idx))) != 0)
#endif /* FEATURE_VOICE_PLAYBACK */
/* <EJECT> */

/* Types of tone generators
*/
typedef enum {
  SND_GEN_NONE = 0,     /* No sound generator (silence)                    */
  SND_GEN_VOC,          /* Vocoder generated tones                         */
  SND_GEN_RING,         /* Ringer generated tone (1st parameter only)      */
  SND_GEN_RING_VOCTX,   /* Both Ringer and Vocoder - The Vocoder is for    */
                        /*   TX only and is never routed locally.          */

  SND_GEN_CODEC,     /* Codec generated tones                         */

#ifdef SND_ADSP_TONEGEN
#error code not present
#endif
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif
  SND_GEN_AUX           /* Auxiliary on/off generator                      */
} snd_gen_type;

/* Frequencies and generators associated with DTMF's and single tones.
** This table is indexed by the enumeration containing all of the DTMF's
** and single tones.  Note: for some targets there is a "high" side and
** a "low" side to the DTMF generator.
*/
typedef struct {
#ifdef SND_DEBUG
  snd_tone_type       index_tone;/* Check Table in sndhw_dtmf_values    */
#endif
  word hi;  /* High Freq associated with DTMF      */
  word lo;  /* Low Freq associated with DTMF       */
} snd_dtmf_type;

#ifdef SND_DEBUG
#define DEF_DTMF(i,h,l)   (i), (h), (l)
#else
#define DEF_DTMF(i,h,l)   (h), (l)
#endif

/* Enumerated type for indexing into snd_array.  As an example,
** snd_array[SND_TONE] contains all information associated with the Tone
** currently being generated (if any).
*/
typedef enum {

#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#else

  SND_SOUND = 0,      /* Reference to a series of dtmfs or tones which are
                      ** known to the sound task and which can be referred
                      ** to by a name from the command block.
                      */
  SND_MULTI = 1,      /* Multiple tones or dtmfs supplied in command block
                      */
  SND_TONE = 2,       /* Single tone or dtmf
                      */
#endif /* FEATURE_VOICE_PLAYBACK */
  SND_ARRAY_SIZE      /* Each variable in this enumeration is used to index
                      ** into the snd_array so this constant can be used to
                      ** size the snd_array.
                      */
} snd_array_idx_type;

#if (TG==T_T)
#error code not present
#else
#define SND_EXTERNAL_DEVICE_DEFAULT    SND_DEVICE_HFK_CORD
#endif

/* Enumerated type for deciding which kind of tone to play. Tone may be:
** (a) regular tone (for a specific freq & duration) or (b) click which 
** happens only once (with no freq)
*/
typedef enum {
  SND_TONE_ENABLE = 0,            /*  Regular tones are to be generated    */
  SND_CLICK_ENABLE,               /*  Click is to be generated             */
  SND_CLICK_DONE                  /*  Click has been done                  */
} snd_ring_type;

/* Type to describe a sound's status
*/
typedef struct {

  boolean        active;           /* Whether sound is in process -or- not */
  boolean        hold;             /* Whether we are waiting for the voc.  */
  snd_compact_type
                 *tone_ptr;        /* Pointer to array of tones to play    */
  word           time_remain_ms;   /* Count of ms before tone is timed out */
  byte           tone_index;       /* Tone_ptr array index of current tone */
  rex_tcb_type   *task_ptr;        /* Task to signal when done             */
  rex_sigs_type  sigs;             /* Signal to send task when done        */
  rex_sigs_type  rpt_sigs;         /* Signals to signal @ SND_RPT          */
  snd_device_type device;            /* Temporary audio path                 */
  snd_class_type sclass;           /* Sound class (which generator)        */
  snd_vol_type   vol;              /* Tone Volume for all paths            */
  snd_dtmf_apath_type
                 dtmf_path;        /* Temporary DTMF tx/local/mute pathing */
  word           loop_cnt;         /* Loop count for SND_LOOP_BACK2        */
  byte           label_index;      /* Array index of label tone            */
  word           label_cnt;        /* Loop count for label                 */
  word           repeat_cnt;       /* Repeat count                         */
  snd_ring_type  click_state;      /* Tone in use reg tone/click/click done*/ 
#ifdef FEATURE_ENHANCED_RINGS
#error code not present
#endif
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif
} snd_status_type;

/* Defines indicating any extra audio processing the the vocoder should perform
** for this device.
*/
#define SND_PROCESS_ESEC_MASK       0x01
#define SND_PROCESS_ESEC_ENABLE     0x01
#define SND_PROCESS_ESEC_DISABLE    0x00

#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif

typedef enum {
  SND_PROCESS_NONE = 0x00,         /* No additional audio processing       */
  SND_PROCESS_ESEC = 0x01,         /* Enable Ear Seal Echo Cancellation    */
  SND_PROCESS_NOT_SET = 0x80       /* Use as first time indicator          */
} snd_audio_process_type;

/* Bitmask position indicating whether BIO_SW10 should be checked for
** overriding volume settings during DOTG calls. For use in ctl_flags
** of snd_vol_ctl_type structure. Rest of bitfield positions are 
** reserved for future use.
*/
#define SND_CHECK_BIO_SW10_MASK     0x01
#define SND_CHECK_BIO_SW10_ENABLE   0x01
#define SND_CHECK_BIO_SW10_DISABLE  0x00

/* Bitmask position indicating whether DATA_FILTERS should be checked for
** overriding volume settings. For use in ctl_flags of snd_vol_ctl_type 
** structure. Rest of bitfield positions are reserved for future use.
*/
#define SND_VERIFY_DATA_FILTER_MASK     0x02
#define SND_VERIFY_DATA_FILTER_ENABLE   0x02
#define SND_VERIFY_DATA_FILTER_DISABLE  0x00

/* Structure containing the constant information needed to activate and
** set the volume for a particular audio path, generator, and sound
** class. These structures can be defined in ROM since they should
** never change.
*/
typedef struct {
  snd_device_type gen_device;    /* The path where the generator resides.  */
                              /* This lets one path use generators on   */
                              /* another path to play certain classes   */
                              /* of sound.                              */
  snd_gen_type   def_gen;     /* Each class of sound on each path plays */
                              /* to one default generator. The actual   */
                              /* generator may be SND_GEN_NONE if the   */
                              /* tone is a control tone.                */
  byte           aud_process; /* Bitmask of any extra audio processing  */
                              /* that should be enabled for this device.*/
  VOL_CONST int  *map_ptr;    /* Pointer to the volume mapping table.   */
  VOL_CONST void *vol_table;  /* Access to volume table for a specific  */
                              /* generator. A void pointer is used      */
                              /* because each generator requires a      */
                              /* different kind of table.               */ 
  byte           max_vol;     /* Maximum volume setting.                */
  byte           *vol_ptr;    /* The regional volume setting connected  */
                              /* to this class of sound. NULL if none.  */
  byte           ctl_flags;   /* Bitmask containing flags of useful     */
                              /* information                            */
#ifdef FEATURE_ENHANCED_RINGS
#error code not present
#endif
} snd_vol_ctl_type;

/* All the information required to play a tone to a specific generator
** on a specific audio path.
*/
typedef struct {
  snd_tone_type      tone;           /* The index of the tone to play        */
  word               time_remain_ms; /* Time remaining on this tone [mSec]   */
  boolean            tone_hold;      /* Are we extending the tone because    */
                                     /* we are waiting for the vocoder?      */
  boolean            phs_override;   /* Is the tone force to play from the   */
                                     /* carkit speaker when external?        */
  snd_gen_type       generator;      /* The generator (default or none)      */
  byte               volume;         /* Volume (depends on path)             */
  snd_dtmf_apath_type 
                     dtmf_path;      /* Who will hear the tone tx/local/mute */
  const snd_vol_ctl_type 
                     *vol_ctl;       /* What path, default generator,        */
                                     /* and how to set the volume.           */
  snd_ring_type      click_state;    /* Tone in use reg tone/click/click done*/ 
#ifdef FEATURE_ENHANCED_RINGS
#error code not present
#endif
  snd_device_type    device;         /* We need to pass this back...         */
#ifdef SND_PARA_SUPPORT
  snd_freq_pair_type freq_pair;      /* freq pair for this tone, if appl.    */
#endif
} tone_status_type;

/* Path State Type
** Includes: physical path, whether the path is muted or not,
** the current volume, and how to set the volume.
*/
// jwpark 00.10.29 : insert manner mode
typedef struct {
  snd_device_type         device;       /* The audio device connected   */
  snd_mute_control_type   mute;         /* Path's mute state            */
  const snd_vol_ctl_type  *vol_ctl;     /* Physical audio path,         */
                                        /* generator,                   */
                                        /* and how to set the volume.   */
  byte                    volume;       /* The current volume.          */
  boolean                 manner;       // Is in manner mode?
#ifdef FEATURE_ENHANCED_RINGS
#error code not present
#endif
} snd_pstate_type;

#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK */

#ifdef SND_PARA_SUPPORT
/* Structure of buffer entries used to hold frequency pairs and duration of
 * a specified tone.
 */
typedef struct {
  snd_freq_pair_type freq;          /* Frequency pair of DTMF */
  word               duration_ms;   /* Duration of tone       */
} snd_freq_buff_type;
#endif

/* <EJECT> */
/*---------------------------------------------------------------------------
  MSM2Px macros, commands, definitions, and types
---------------------------------------------------------------------------*/
                                                                                        
/* Define unity-gain for the DTMF's generated by the Vocoder.  Define
** it here, in terms of a vocoder value, so that we can configure 'unity'
** to the actual unity-gain in the event that it is not the theoretical
** unity-gain.
*/
#if (TG==T_I1)
#error code not present
#endif
#if ((TG==T_MD) || (TG==T_G) || (TG==T_T))
#define SND_VOC_DTMF_UNITY_GAIN 0x7FFF
#endif
#ifndef SND_VOC_DTMF_UNITY_GAIN
#define SND_VOC_DTMF_UNITY_GAIN 0x2000  /* Was 0x4000 Until 2/23/95 - FKM */
#endif

/* Mask for CODEC Volume in Vocoder Pad
*/
#ifdef SND_HFK_PLUS
#error code not present
#else
#define SND_CODEC_VOL_M         (0xff)          /* Codec Gain Voc Pad   */
#endif

/* Mask to give complete control of pad byte to external communications
*/
#ifdef SND_EXTCOM
#error code not present
#endif

/* Certain Vocoder Gains by a Number of DB Hotter than VOC_UNITY_GAIN)
*/
#define SND_VOC_1P5DB_COLD      (53*VOC_UNITY_GAIN/63)  /* 1.5 DB Cold  */
#define SND_VOC_1P5DB_HOT       (63*VOC_UNITY_GAIN/53)  /* 1.5 DB Hot   */
#define SND_VOC_3P0DB_HOT       (89*VOC_UNITY_GAIN/63)  /* 3.0 DB Hot   */
#define SND_VOC_4P5DB_HOT       (47*VOC_UNITY_GAIN/28)  /* 4.5 DB Hot   */

/* Certain Codec Gains (for voc_codec_pad() Calls).  There are 8 Steps
** of 3DB each for the Handset and 8 steps of 4.25 DB each for the Car Kit.
*/
#define SND_CODEC_21DB    (byte)(~(0x7<<5))     /* 21 DB Above Minimum  */
#define SND_CODEC_18DB    (byte)(~(0x6<<5))     /* 18 DB Above Minimum  */
#define SND_CODEC_15DB    (byte)(~(0x5<<5))     /* 18 DB Above Minimum  */
#define SND_CODEC_12DB    (byte)(~(0x4<<5))     /* 12 DB Above Minimum  */
#define SND_CODEC_9DB     (byte)(~(0x3<<5))     /* 9 DB Above Minimum   */
#define SND_CODEC_3DB     (byte)(~(0x1<<5))     /* 3 DB Above Minimum   */
#define SND_CODEC_0DB     (byte)(~(0x0<<5))     /* 0 DB Above Minimum   */
#define SND_CODEC_MIN_VOL       SND_CODEC_0DB   /* Minimum Codec Volume */

/* Macro to lower Vocoder Gain Value by 6DB.
*/
#define SND_VOC_MINUS_6DB(gain) (gain/2)

/* How long after voc_init() to keep the audio muted to avoid a pop.
** The current measurement is at 1.75 milliseconds.  We want to wait
** at least 2 milliseconds, and not less due to tick frequency.
*/
#define SND_VOC_POP_WAIT        (2 + CLK_MS_PER_TICK)

/* Amount of time in ms to wait for the sound task to wait for the vocoder 
** to become available to play a tone. If, at the end of this time, the 
** vocoder is not available then SND_TONE will be expired.
*/
#define SND_TONE_VOC_TIMEOUT 500

/* <EJECT> */
/*===========================================================================

MACRO VOX_MODE

DESCRIPTION
  Determine whether we are in VOX mode or not.

PARAMETERS
  None

DEPENDENCIES
  snd_apath_out, snd_apath_in

RETURN VALUE
  True  Phone should be in VOX mode
  False Phone should be in regular mode

SIDE EFFECTS
  None

===========================================================================*/

#define VOX_MODE() \
  ( snd_apath_out == SND_APATH_EXT && snd_apath_in == SND_APATH_EXT )
/* <EJECT> */
/*===========================================================================

MACRO SND_SLEEP_ALLOW

DESCRIPTION
  Tell the sleep task that we are in a state where we can sleep.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The sleep task may take us up on our offer.

===========================================================================*/
#ifdef T_SLEEP
#define SND_SLEEP_ALLOW() \
  (void) rex_set_sigs( &sleep_tcb, SLEEP_SND_OKTS_SIG )
#else
#define SND_SLEEP_ALLOW()
#endif
/* <EJECT> */
/*===========================================================================

MACRO SND_SLEEP_FORBID

DESCRIPTION
  Tell the sleep task that we are NOT in a state where we can sleep.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef T_SLEEP
#define SND_SLEEP_FORBID() \
  (void) rex_clr_sigs( &sleep_tcb, SLEEP_SND_OKTS_SIG )
#else
#define SND_SLEEP_FORBID()
#endif
/* <EJECT> */
/*===========================================================================

MACRO SND_ALLOW_SLEEPING

DESCRIPTION
  The decision to allow sleeping involves several parts.  This macros
  knows what all the parts are and decides if sleeping should be allowed
  at this time.

PARAMETERS
  None

DEPENDENCIES
  snd_need_voc_acquire
  snd_tone_awake
  snd_voc_acquired

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if defined (FEATURE_YMU757B_MELODY)	// jwpark 00.11.27
#define YMU757B_SLEEPING	MUSIC_sleep_enable
#elif defined (FEATURE_YMU759_MELODY)
#define YMU759_SLEEPING	MUSIC_sleep_enable
#else
#define YMU757B_SLEEPING    1
#endif

#ifdef OKI_CODEC				// youandi  01.04.10
extern boolean vocoder_sleep_enable;
#define OKI_CODEC_SLEEPING	(Codec_sleep_enable && vocoder_sleep_enable) 
#else
#define OKI_CODEC_SLEEPING  1
#endif


#if defined FEATURE_YMU757B_MELODY	// jwpark 00.11.27
#define SND_ALLOW_SLEEPING (!snd_tone_awake       && \
                            !snd_voc_acquired     && \
                            !snd_need_voc_acquire && \
							YMU757B_SLEEPING      && \
							OKI_CODEC_SLEEPING)
#elif defined FEATURE_YMU759_MELODY
#define SND_ALLOW_SLEEPING (!snd_tone_awake       && \
                            !snd_voc_acquired     && \
                            !snd_need_voc_acquire && \
							YMU759_SLEEPING      && \
							OKI_CODEC_SLEEPING)
#endif

/* <EJECT> */
/*===========================================================================

MACRO SND_DO_PROCESS_ESEC

DESCRIPTION
  This macros decides if ESEC should be enabled.

PARAMETERS
  bitmask

DEPENDENCIES
  none

RETURN VALUE
  TRUE if ESEC should be enabled
  FALSE if ESEC should not be enabled

SIDE EFFECTS
  None

===========================================================================*/
#define SND_DO_PROCESS_ESEC(v) ((v & SND_PROCESS_ESEC_MASK) == \
                                     SND_PROCESS_ESEC_ENABLE)
/* <EJECT> */
#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif
/* <EJECT> */
/*===========================================================================

MACRO SND_BIO_SW10_IS_ENABLED

DESCRIPTION
  This macro will check the proper bitmask of the byte to see if 
  BIO_SW10 should be checked for volume override during a DOTG call.

PARAMETERS
  bitmask

DEPENDENCIES
  None

RETURN VALUE
  TRUE  if BIO_SW10 should be checked.
  FALSE if BIO_SW10 should not be checked.

SIDE EFFECTS
  None

===========================================================================*/
#define SND_BIO_SW10_IS_ENABLED(v) ((v & SND_CHECK_BIO_SW10_MASK) == \
                                    SND_CHECK_BIO_SW10_ENABLE)
/* <EJECT> */
/*===========================================================================

MACRO SND_VERIFY_DFILT_IS_DISABLED

DESCRIPTION
  This macro will check the proper bitmask of the byte to see if
  DATA FILTER should override the volume settings.

PARAMETERS
  bitmask

DEPENDENCIES
  None

RETURN VALUE
  TRUE  if DATA FILTER should not override volume settings.
  FALSE if DATA FILTER should override volume settings.

SIDE EFFECTS
  None

===========================================================================*/
#define SND_VERIFY_DFILT_IS_DISABLED(v) ((v & SND_VERIFY_DATA_FILTER_MASK) == \
                                         SND_VERIFY_DATA_FILTER_DISABLE)
/* <EJECT> */
/*===========================================================================

MACRO SND_DEBUG_MSG

DESCRIPTION
  Allows leaving certain Debug Messages in Sound that can be turned on/off
  Regionally.  This macro is a NOP unless /DSND_DEBUG is added to the
  compile line for the routines.

PARAMETERS
  fmt Format string for message (printf style).  Note, this parameter
      must be a string literal (e.g. "Tuned to %lx"), not a variable.
  p1  1st parameter for format string
  p2  2nd parameter for format string
  p3  3rd parameter for format string

DEPENDENCIES
  Turned on/off by /DSND_DEBUG at compile time.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef SND_DEBUG
/* SND_DEBUG is meaningless unless at we are compiling with at least
** MSG_LVL_FATAL.  Force either no SND_DEBUG or else MSG_LVL_FATAL.
*/
#if MSG_LEVEL > MSG_LVL_FATAL
#error /DSND_DEBUG Requires at least MSG_LVL_FATAL
#endif
#define SND_DEBUG_MSG(fmt,p1,p2,p3)     MSG_FATAL(fmt,p1,p2,p3)
#else
#define SND_DEBUG_MSG(fmt,p1,p2,p3)
#endif
/* <EJECT> */
/*===========================================================================

                      REGIONAL FUNCTION DECLARATIONS

===========================================================================*/
#endif /* SNDI_H */

