#ifndef VOCSUP_H
#define VOCSUP_H
/*===========================================================================

            V O C O D E R    S U P P O R T     H E A D E R    F I L E

DESCRIPTION
  This file contains declarations associated with the vocoder support
  functions.                                                     

REFERENCES
  MSM2.2  User's Manual - 93-13433-1 X1
  MSM2300 User's Manual - 93-22436-1 X1
  MSM3000 User's Manual - 93-24207-1 X1

EXTERNALIZED FUNCTIONS
  vocsup_enc_format_to_rate  
    Use this routine to translate the encoder format byte to the equivalent 
    voc_rate_type.
  vocsup_dec_format_to_rate  
    Use this routine to translate the decoder format byte to the equivalent 
    voc_rate_type.
  vocsup_rate_to_size    
    Use this routine to determine the length of the Decoder or Encoder packet
    based on the rate of the packet.
  vocsup_compute_freq
    This function returns the vocoder tone value for a given frequency
    to be played by the vocoder.
  vocsup_pb_q_size       
    This function returns number of entries in the specified voice play back 
    queue.
  vocsup_pb_get_hdr
    This function returns the pointer to the current entry of the specified 
    vocoder play back queue.
  vocsup_pb_get_frame    
    This function is called to get the current frame to output in a voice
    play back session.
  vocsup_pb_add_entry    
    This function adds one entry to the specified voice play back queue.
  vocsup_pb_clear_q      
    This function clears the specified voice play back queue.
  voc_bytes_to_frames
    Given the specified vocoder and vocoder frame format, determine how many 
    COMPLETE vocoder frames in the given format will fit into the number of bytes
    specified.  In the case of variable size formats, out_min_frames and 
    out_max_frames may be set to different values.
  voc_count_frames
    Given the specified vocoder and vocoder frame format, examine the buffer of 
    vocoder frames passed in and determine how many COMPLETE vocoder frames are 
    present, up to the number of frames specified in inout_frame_count.  Return 
    the number of COMPLETE vocoder frames and how many bytes those frames 
    occupy.  Always assume that the first byte of the input buffer begins a new 
    vocoder frame. 
  vocsup_rec_init_queue
    This function intializes the voice recording queue.
  vocsup_rec_get_buf
    This function gets a record buffer from the voice record free queue.
  voc_rec_free_buf
    This function returns a record buffer to the voice record free queue.
  vocsup_get_frame
    This function records one voice frame.  It's called from vocm2_rec_frame in 
    order to record in or out of a call.  This function copies the voice frame 
    to the record buffer without disturbing the frame itself.
  vocsup_rec_start
    This function starts the recording process at the frame data level.
  vocsup_rec_stop
    This function stops voice recording.  This function will return the pointer
    of the last buffer in the recording session.
  vocsup_rec_change_config
    This function is called when the vocoder driver detects that the
    configuration has changed while an in-call recording session is in 
    progress.
    This function updates the configuration specified in the header of the 
    recorded buffers from this point on.
  voc_get_shared_mem_ptr
    This function returns a pointer to the shared RAM area between the vocoder
    driver and the VR engine.  The vocoder shared RAM area is limited to two
    buffers used in voice recording.  The purpose of sharing RAM is to eliminate
    RAM usage for the recording buffers.  By sharing RAM, voice recording (for
    voice memo only at this point) and voice recognition can not be running 
    concurrently.
  vocsup_validate_frame_control_byte
    Given the input vocoder capability, check if the input frame control byte is
    valid.


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 1998, 1999, 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR FILE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/vocsup.h_v   1.1   07 Aug 2000 20:04:34   ropalsky  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/07/00    ro     Remove size parameter from vocsup_rec_frame().
08/19/99    vl     Added VSP_PBQ_MAX (used to be in vocsup.c).
07/21/99    vl     Implemented voice answering machine feature (playback on
                   reverse link and both links, record forward link).  Created
                   vocsup_enc_format_to_rate () and vocsup_dec_format_to_rate ()
                   so playback can accomodate erasure frames, blank frames and 
                   full rate error frames.  Added frame format byte defines.
04/15/99    vl     Added VSP_MAX_REC_INTERVAL_MS.
04/07/99    vl     Removed externs for 3 vocsup functions due to name change,
                   added #include customer.h.
03/04/99    st     Added vocsup_compute_freq() function prototype; added 
                   defines for vocoder tone values, also added 
                   vocsup_freq_value_type typedef, these are used for 
                   calculating the vocoder value of a tone from its frequency 
                   value.
03/04/99    vl     Implemented Voice Record to support Voice Memo (see 
                   FEATURE_VOICE_RECORD) plus misc clean up.
12/04/98    vl     Added one more parameter to vocsup_pb_get_frame ().
                   Created function vocsup_pb_get_hdr ().
08/31/98    vl     Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "customer.h"

/* <EJECT> */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
/*---------------------------------------------------------------------------

  Vocoder tones

---------------------------------------------------------------------------*/
/* Structure containing the frequency value along with its vocoder value
 */
typedef struct {
  word frequency;
  word value;
} vocsup_freq_value_type;

/*
**      Vocoder tone = 32768.0 * cos( 2.0 * Pi * frequency / 8000.0 ) + 0.5
**
**      2*Pi radians is 1 cycle, 8000 is the sample rate of the Vocoder,
**      32768 is for scaling the number for fixed point arithmatic, the
**      0.5 is for rounding.  All values wrap to 0 at 65536.
*/
#define VOC_100_HZ      32667
#define VOC_150_HZ      32540
#define VOC_200_HZ      32365
#define VOC_250_HZ      32138
#define VOC_300_HZ      31863
#define VOC_350_HZ      31538
#define VOC_400_HZ      31164
#define VOC_450_HZ      30743
#define VOC_500_HZ      30274
#define VOC_550_HZ      29758
#define VOC_600_HZ      29197
#define VOC_650_HZ      28590
#define VOC_700_HZ      27939
#define VOC_750_HZ      27246
#define VOC_800_HZ      26510
#define VOC_850_HZ      25733
#define VOC_900_HZ      24917
#define VOC_950_HZ      24062
#define VOC_1000_HZ     23170
#define VOC_1050_HZ     22243
#define VOC_1100_HZ     21281
#define VOC_1150_HZ     20286
#define VOC_1200_HZ     19261
#define VOC_1250_HZ     18205
#define VOC_1300_HZ     17121
#define VOC_1350_HZ     16011
#define VOC_1400_HZ     14876
#define VOC_1450_HZ     13719
#define VOC_1500_HZ     12540
#define VOC_1550_HZ     11342
#define VOC_1600_HZ     10126
#define VOC_1650_HZ     8895
#define VOC_1700_HZ     7650
#define VOC_1750_HZ     6393
#define VOC_1800_HZ     5126
#define VOC_1850_HZ     3851
#define VOC_1900_HZ     2571
#define VOC_1950_HZ     1286
#define VOC_2000_HZ     1
#define VOC_2050_HZ     64249
#define VOC_2100_HZ     62964
#define VOC_2150_HZ     61684
#define VOC_2200_HZ     60409
#define VOC_2250_HZ     59144
#define VOC_2300_HZ     57885
#define VOC_2350_HZ     56640
#define VOC_2400_HZ     55410
#define VOC_2450_HZ     54195
#define VOC_2500_HZ     52996
#define VOC_2550_HZ     51817
#define VOC_2600_HZ     50661
#define VOC_2650_HZ     49524
#define VOC_2700_HZ     48414
#define VOC_2750_HZ     47331
#define VOC_2800_HZ     46274
#define VOC_2850_HZ     45249
#define VOC_2900_HZ     44254
#define VOC_2950_HZ     43292
#define VOC_3000_HZ     42365
#define VOC_3050_HZ     41473
#define VOC_3100_HZ     40618
#define VOC_3150_HZ     39802
#define VOC_3200_HZ     39025
#define VOC_3250_HZ     38289
#define VOC_3300_HZ     37596
#define VOC_3350_HZ     36945
#define VOC_3400_HZ     36339
#define VOC_3450_HZ     35777
#define VOC_3500_HZ     35261
#define VOC_3550_HZ     34792
#define VOC_3600_HZ     34371
#define VOC_3650_HZ     33997
#define VOC_3700_HZ     33673
#define VOC_3750_HZ     33397
#define VOC_3800_HZ     33170
#define VOC_3850_HZ     32994
#define VOC_3900_HZ     32868
#define VOC_3950_HZ     32792
#define VOC_4000_HZ     32767
#define VOC_UNUSED      0      /* Invalid/Unused Frequency for Vocoder */

/*---------------------------------------------------------------------------
                       VOCODER FRAME FORMAT BYTE 
                       
Masks used to process Vocoder encoder/decoder format byte.
---------------------------------------------------------------------------*/

/* Mask used to validate frame format byte .  
**    Bit 7:  Set to 1 if 13K packet data, 0 if 8K packet data (EVRC)
**    Bit 6:  Should always be 0 (reserved)
*/
#define VSP_VALID_PLAYBACK_FORMAT_M  0xC0      
#define VSP_13K_V                    0x80           /* Use 13K vocoder     */
#define VSP_8K_V                     0x00           /* Use 8K vocoder      */
                                                                         
/* Mask used to get rate information from encoder/decoder frame format byte.   
*/                                                                       
#define VSP_FMT_RATE_M               0x03           /* Frame rate          */
                                                                         
/* Masks for decoder frame format byte.                                 
*/                                                                       
#define VSP_DEC_FULL_LIKELY_M   0x10    /* Flag full-rate-likely frames    */
#define VSP_DEC_FULL_LIKELY_V   0x10    /* Frame is a full-rate-likely     */
#define VSP_DEC_ERASE_M         0x08    /* Flag erased frame               */
#define VSP_DEC_ERASE_V         0x08    /* Frame is an erasure             */
#define VSP_DEC_BLANK_M         0x04    /* Flag blanked frame              */
#define VSP_DEC_BLANK_V         0x04    /* Frame is blanked                */

#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK */

#if defined (FEATURE_VOICE_RECORD) || defined (FEATURE_PHONE_VR) 
#error code not present
#endif /* FEATURE_VOICE_RECORD || FEATURE_PHONE_VR */

/* <EJECT> */
/*===========================================================================

                 FUNCTION DEFINITIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION VOCSUP_ENC_FORMAT_TO_RATE

DESCRIPTION
  Use this routine to translate the encoder format byte to the equivalent 
  voc_rate_type.

DEPENDENCIES
  None

RETURN VALUE
  voc_rate_type value.

SIDE EFFECTS
  None

===========================================================================*/
extern voc_rate_type vocsup_enc_format_to_rate (byte format);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCSUP_DEC_FORMAT_TO_RATE

DESCRIPTION
  Use this routine to translate the decoder format byte to the equivalent 
  voc_rate_type.

DEPENDENCIES
  None

RETURN VALUE
  voc_rate_type value.

SIDE EFFECTS
  None

===========================================================================*/
extern voc_rate_type vocsup_dec_format_to_rate (byte format);

/* <EJECT> */
/*===========================================================================

FUNCTION VOCSUP_RATE_TO_SIZE

DESCRIPTION
  Use this routine to determine the length of the Decoder or Encoder packet
  based on the rate of the packet.

DEPENDENCIES
  None

RETURN VALUE
  The length in bytes of the packet not including the format byte.

SIDE EFFECTS
  None

===========================================================================*/
extern word vocsup_rate_to_size(voc_rate_type rate, voc_capability_type so);

/* <EJECT> */
/*===========================================================================

FUNCTION vocsup_compute_freq

DESCRIPTION
  This function returns the vocoder tone value for a given frequency
  to be played by the vocoder.

DEPENDENCIES
  Requires vocsup_freq_values[].

RETURN VALUE
  Returns the a word containing the vocoder tone value.

SIDE EFFECTS
  None.
===========================================================================*/
extern word vocsup_compute_freq
(
  word frequency
    /* Frequency in Hz for which vocoder tone values are to be computed */
);
/* <EJECT> */
#ifdef FEATURE_VOICE_PLAYBACK
#error code not present
#endif /* FEATURE_VOICE_PLAYBACK */

/* <EJECT> */
#ifdef FEATURE_VOICE_RECORD
#error code not present
#endif /* FEATURE_VOICE_RECORD */

#endif /* VOCSUP_H */

