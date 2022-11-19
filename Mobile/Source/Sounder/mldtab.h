#ifndef BBSNDTAB_H
#define BBSNDTAB_H
/*===========================================================================

     B O O T   B L O C K   M E L O D Y  T A B L E (S)   H E A D E R   F I L E

DESCRIPTION
  This file contains the definitions needed for the sound driver to access
  the tables found in BBSNDTAB.C in the Boot Block.

Copyright (c) 1996, 1997, 1998, 1999 by QUALCOMM Incorporated. All Rights Reserved.
===========================================================================*/

#include "comdef.h"
#include "sndi.h"

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   O:/SRC/MSM3000/ASW/VCS/bbsndtab.h_v   1.0   11 Jan 1999 19:52:40   hkulkarn  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/11/00   moda    Declare public data of added melodies.
01/25/00   sasaki  Created file.(Look Like B5)

===========================================================================*/

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#ifdef TERMINATED_MELODY		// added by moda, 2000.02.22
extern snd_compact_tone_type mb_MyBellPreBuffer[];				// MyBell pre-buffer

extern snd_compact_tone_type mymelody1_snd_buf[];
extern snd_compact_tone_type mymelody2_snd_buf[];
extern snd_compact_tone_type mymelody3_snd_buf[];
extern const snd_compact_tone_type snd_bell1_buf[];
extern const snd_compact_tone_type snd_bell2_buf[];
extern const snd_compact_tone_type snd_bell3_buf[];
extern const snd_compact_tone_type snd_bell4_buf[];
extern const snd_compact_tone_type snd_bell5_buf[];
extern const snd_compact_tone_type incoming_buf[];
extern const snd_compact_tone_type chime_ring_buf[];
extern const snd_compact_tone_type melody1_ring_buf[];
extern const snd_compact_tone_type melody2_ring_buf[];
extern const snd_compact_tone_type melody3_ring_buf[];
extern const snd_compact_tone_type melody4_ring_buf[];
extern const snd_compact_tone_type melody5_ring_buf[];
extern const snd_compact_tone_type melody6_ring_buf[];
extern const snd_compact_tone_type melody7_ring_buf[];
extern const snd_compact_tone_type melody8_ring_buf[];
extern const snd_compact_tone_type melody9_ring_buf[];
extern const snd_compact_tone_type melody10_ring_buf[];
#ifndef MELODY_20
extern const snd_compact_tone_type melody11_ring_buf[];
extern const snd_compact_tone_type melody12_ring_buf[];
extern const snd_compact_tone_type melody13_ring_buf[];
extern const snd_compact_tone_type melody14_ring_buf[];
extern const snd_compact_tone_type melody15_ring_buf[];
extern const snd_compact_tone_type melody16_ring_buf[];
extern const snd_compact_tone_type melody17_ring_buf[];
extern const snd_compact_tone_type melody18_ring_buf[];
extern const snd_compact_tone_type melody19_ring_buf[];
extern const snd_compact_tone_type melody20_ring_buf[];
extern const snd_compact_tone_type melody21_ring_buf[];
extern const snd_compact_tone_type melody22_ring_buf[];
extern const snd_compact_tone_type melody23_ring_buf[];
extern const snd_compact_tone_type melody24_ring_buf[];
extern const snd_compact_tone_type melody25_ring_buf[];
extern const snd_compact_tone_type melody26_ring_buf[];
extern const snd_compact_tone_type melody27_ring_buf[];
extern const snd_compact_tone_type melody28_ring_buf[];
extern const snd_compact_tone_type melody29_ring_buf[];
extern const snd_compact_tone_type melody30_ring_buf[];
extern const snd_compact_tone_type melody31_ring_buf[];
extern const snd_compact_tone_type melody32_ring_buf[];
extern const snd_compact_tone_type melody33_ring_buf[];
extern const snd_compact_tone_type melody34_ring_buf[];
extern const snd_compact_tone_type melody35_ring_buf[];
extern const snd_compact_tone_type melody36_ring_buf[];
extern const snd_compact_tone_type melody37_ring_buf[];
extern const snd_compact_tone_type melody38_ring_buf[];
extern const snd_compact_tone_type melody39_ring_buf[];
extern const snd_compact_tone_type melody40_ring_buf[];
#endif	// MELODY_20

extern const snd_compact_tone_type vibrator_buf[];

extern const snd_compact_tone_type snd_mpg_theme_buf[]; 
extern const snd_compact_tone_type snd_pzg_theme_buf[];
extern const snd_compact_tone_type snd_sam_theme_buf[];
extern const snd_compact_tone_type snd_ddg_theme_buf[];
extern const snd_compact_tone_type snd_ddg_shot_buf[];
extern const snd_compact_tone_type snd_ddg_born_buf[];
extern const snd_compact_tone_type snd_mpg_shot_buf[];
extern const snd_compact_tone_type snd_mpg_jump_buf[];
extern const snd_compact_tone_type snd_mpg_ufo_shot_buf[];
extern const snd_compact_tone_type snd_mpg_ufo_blaster_buf[];
extern const snd_compact_tone_type snd_mpg_stone_blaster_buf[];
extern const snd_compact_tone_type snd_mpg_car_blaster_buf[];

extern const snd_compact_tone_type VocTest_buf[];
#endif  // TERMINATED_MELODY

//#ifdef TERMINATED_MELODY
//#ifdef REFERENCE
//	/// Added by sasaki 2000.1.25
//extern const snd_compact_sound_type melody20_ring_sound;
//#endif

#ifdef TERMINATED_MELODY
extern snd_compact_tone_type mymelody1_snd_sound;
extern snd_compact_tone_type mymelody2_snd_sound;
extern snd_compact_tone_type mymelody3_snd_sound;
extern const snd_compact_tone_type snd_bell1_sound;
extern const snd_compact_tone_type snd_bell2_sound;
extern const snd_compact_tone_type snd_bell3_sound;
extern const snd_compact_tone_type snd_bell4_sound;
extern const snd_compact_tone_type snd_bell5_sound;
extern const snd_compact_tone_type incoming_sound;
extern const snd_compact_tone_type chime_ring_sound;
extern const snd_compact_sound_type melody1_ring_sound;
extern const snd_compact_sound_type melody2_ring_sound;
extern const snd_compact_sound_type melody3_ring_sound;
extern const snd_compact_sound_type melody4_ring_sound;
extern const snd_compact_sound_type melody5_ring_sound;
extern const snd_compact_sound_type melody6_ring_sound;
extern const snd_compact_sound_type melody7_ring_sound;
extern const snd_compact_sound_type melody8_ring_sound;
extern const snd_compact_sound_type melody9_ring_sound;
extern const snd_compact_sound_type melody10_ring_sound;
#ifndef MELODY_20
extern const snd_compact_sound_type melody11_ring_sound;
extern const snd_compact_sound_type melody12_ring_sound;
extern const snd_compact_sound_type melody13_ring_sound;
extern const snd_compact_sound_type melody14_ring_sound;
extern const snd_compact_sound_type melody15_ring_sound;
extern const snd_compact_sound_type melody16_ring_sound;
extern const snd_compact_sound_type melody17_ring_sound;
extern const snd_compact_sound_type melody18_ring_sound;
extern const snd_compact_sound_type melody19_ring_sound;
extern const snd_compact_sound_type melody20_ring_sound;
extern const snd_compact_sound_type melody21_ring_sound;
extern const snd_compact_sound_type melody22_ring_sound;
extern const snd_compact_sound_type melody23_ring_sound;
extern const snd_compact_sound_type melody24_ring_sound;
extern const snd_compact_sound_type melody25_ring_sound;
extern const snd_compact_sound_type melody26_ring_sound;
extern const snd_compact_sound_type melody27_ring_sound;
extern const snd_compact_sound_type melody28_ring_sound;
extern const snd_compact_sound_type melody29_ring_sound;
extern const snd_compact_sound_type melody30_ring_sound;
extern const snd_compact_sound_type melody31_ring_sound;
extern const snd_compact_sound_type melody32_ring_sound;
extern const snd_compact_sound_type melody33_ring_sound;
extern const snd_compact_sound_type melody34_ring_sound;
extern const snd_compact_sound_type melody35_ring_sound;
extern const snd_compact_sound_type melody36_ring_sound;
extern const snd_compact_sound_type melody37_ring_sound;
extern const snd_compact_sound_type melody38_ring_sound;
extern const snd_compact_sound_type melody39_ring_sound;
extern const snd_compact_sound_type melody40_ring_sound;
#endif  // MELODY_20
extern const snd_compact_sound_type snd_mpg_theme_sound; /* game sound : ( not in sdb.c ) */
extern const snd_compact_sound_type snd_pzg_theme_sound;
extern const snd_compact_sound_type snd_sam_theme_sound;
extern const snd_compact_sound_type snd_ddg_theme_sound;
extern const snd_compact_sound_type snd_ddg_shot_sound;
extern const snd_compact_sound_type snd_ddg_born_sound;
extern const snd_compact_sound_type snd_mpg_shot_sound;
extern const snd_compact_sound_type snd_mpg_jump_sound;
extern const snd_compact_sound_type snd_mpg_ufo_shot_sound;
extern const snd_compact_sound_type snd_mpg_ufo_blaster_sound;
extern const snd_compact_sound_type snd_mpg_stone_blaster_sound;
extern const snd_compact_sound_type snd_mpg_car_blaster_sound;
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////// 추가된 게임 사운드 
////// added by yschoi 00.5.19
extern const snd_compact_sound_type snd_apache_theme_sound;
extern const snd_compact_sound_type snd_apache_missile_shot_sound;
extern const snd_compact_sound_type snd_apache_sam_shot_sound;
extern const snd_compact_sound_type snd_apache_land_blast_sound;
extern const snd_compact_sound_type snd_apache_air_blast_sound;

extern const snd_compact_sound_type snd_ffighter_theme_sound;
extern const snd_compact_sound_type snd_ffighter_shot_sound;
extern const snd_compact_sound_type snd_ffighter_special_sound;
extern const snd_compact_sound_type snd_ffighter_destroynew_sound;
extern const snd_compact_sound_type snd_ffighter_keyfree_sound;
extern const snd_compact_sound_type snd_ffighter_ffufoshot_sound;

extern const snd_compact_sound_type snd_polar_theme_sound;
extern const snd_compact_sound_type snd_polar_eat_sound;
extern const snd_compact_sound_type snd_polar_eat_bad_sound;
extern const snd_compact_sound_type snd_polar_jump_sound;
extern const snd_compact_sound_type snd_polar_trap_sound;

extern const snd_compact_sound_type snd_rally_theme_sound;
extern const snd_compact_sound_type snd_rally_rallyacc_sound;
extern const snd_compact_sound_type snd_rally_ralchul_sound;
extern const snd_compact_sound_type snd_rally_beeplow_sound;
extern const snd_compact_sound_type snd_rally_beeplow_sound;
// end of yschoi

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
////////// SMS용 사운드 For A8, by yschoi 00.5.31
extern const snd_compact_sound_type snd_sms_bell1_sound;
extern const snd_compact_sound_type snd_sms_bell2_sound;
extern const snd_compact_sound_type snd_sms_bell3_sound;
extern const snd_compact_sound_type snd_sms_bell4_sound;
extern const snd_compact_sound_type snd_sms_bell5_sound;
extern const snd_compact_sound_type snd_sms_bell6_sound;
extern const snd_compact_sound_type snd_sms_bell7_sound;
// end of yschoi

#endif  // TERMINATED_MELODY
#endif /* BBSNDTAB_H */
