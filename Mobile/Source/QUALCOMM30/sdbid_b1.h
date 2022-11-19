/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     sdbid_b1.h
**
** Description:
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-29 woodstok     Created.
**
** ============================================================================
*/

#ifndef _SDBID_B1_H_
#define _SDBID_B1_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tag_MUS_ID_TYPE_E {
    MUS_FIRST_SND = 1,

    // 기본전화벨
    MUS_DENWA2  = MUS_FIRST_SND,
    MUS_DENWA5,
    MUS_DENWA8,
    MUS_DENWA9,
    MUS_KURODEN,

    // 금영 멜로디
    MUS_DOREMI,
    MUS_ELEPHANTWALK,
	MUS_SILENT_NIGHT,
	MUS_RODOLF_NOSE,
	MUS_JINGLEBELL,
	MUS_WEDDING_MARCH,
	MUS_BIRTHDAY_PANG,
	MUS_CORONATION_MARCH,
	MUS_MOON_TARYUNG,
	MUS_ULYBULY,
	MUS_FLOWER_WALTZ,
	MUS_ALHAMBRA,
	MUS_LAD_MARCH,
	MUS_VIOLIN_ROMANCE,
	MUS_VOKERINI_MINUET,
	MUS_SPRING_SONG,
	MUS_4SEAS_SPRING,
	MUS_WOMEN_HEART,
	MUS_TRIUMPH_MARCH,
	MUS_NOCTURNE,
	MUS_4ELIZE,
	MUS_MAGIC_FLUTE,
	MUS_MAJESTIC_MARCH,
	MUS_WILLIAM_PRELUDE,
	MUS_KANGKANG,
	MUS_CARMEN_PRELUDE,
	MUS_MATADOR_SONG,
	MUS_CANON,
	MUS_LOVE_PLEASURE,
	MUS_ARIA_G,
	MUS_PERGUINT_MORNING,
	MUS_MULLET,
	MUS_SANTALUTIA,
	MUS_PANG,
	MUS_PANG1,
	MUS_COPTER_SOUND,
	MUS_BIRD_SOUND,
	MUS_WAVE_SOUND,
	MUS_TRAIN_SOUND,
	MUS_KNOCK_SOUND,
	MUS_MARRIAGE_MARCH, //////////////// 3월 15 일 추가
	MUS_CHESTNUT_TARYUNG,
	MUS_JOYFUL_JESUS_COME,
	MUS_FARM_ROAD,
	MUS_MILYANG_ARIRANG,
	MUS_DRUM_BOY,
	MUS_BIRD_TARYUNG,
	MUS_OLDYEAR_OUT,
	MUS_LOOKOUT_WINDOW,
	MUS_PUNGNYEONGA,
	MUS_YOUNG_BAND,
	MUS_SALUTE_NATIONALFLAG,
	MUS_WIND_SOUND,
	MUS_GUN_SOUND,
	MUS_NOEL,
	MUS_HE_WOW,
	MUS_HE_HURRY,
	MUS_HE_RING,
	MUS_SHE_WOW,
	MUS_SHE_HURRY,
	MUS_SHE_RING,
	MUS_TING_A_LING, ///////////////////// 3월 19일 추가 
	MUS_MOTHER_PIG_CHILD_PIG,
	MUS_GOOD,
	MUS_CUTY_KID,
	MUS_SUN_BLAZING,
	MUS_INVADE,
	MUS_OPENING_PANG,
	MUS_4SEAS_WINTER,
	MUS_PATHOS_SONATA,
	MUS_HUMORESQUE,
	MUS_TURKEY_MARCH,
	MUS_HAYDN_TRUMPET,
	MUS_HALLELUJAH,
////////////////////////
/// SMS Alarm Bell
///////////////////////
	MUS_SMS_1,
	MUS_SMS_2,
	MUS_SMS_3,
	MUS_SMS_4,
	MUS_SMS_5,
	MUS_SMS_7,

	MUS_STAR,////////////////////////////////////3월 20일 추가 
	MUS_DONAU_WALTZ,
	MUS_TOY_SYMPHONY,
	MUS_COMEBACK_SOTRENTO,
	MUS_CHICK_DANCE,
	MUS_FINLANDIA,
	MUS_MAIDEN_PRAYER,
	MUS_NEWLAND_SYMPHONY,
	MUS_WASHINGTON,
	MUS_SILVERY_WAVE,
	MUS_CAR_SOUND,

	MUS_PUPPY_SOUND,///////////////////////////////3월 22일 추가 
	MUS_HE_PLUS_RING,
	MUS_SHE_PLUS_MUSIC,

	MUS_SMS_6,/////////////////////////////////////4월 11일 추가 

	MUS_ERROR_1,
	MUS_ERROR_2,
	MUS_ERROR_3,
	MUS_ERROR_4,
	MUS_ERROR_5,
	MUS_ERROR_6,
	MUS_ERROR_7,
	MUS_ERROR_8,
	MUS_ERROR_9,

	MUS_DENWA_1,

	MUS_DREAMEND_10,
	MUS_DREAMEND_11,

	MUS_SHUTTER,

	MUS_ALLERT2,
	MUS_ETIQUETTE,

    MUS_DDING,
    MUS_LAST_SND
};

typedef enum tag_MUS_ID_TYPE_E MUS_ID_TYPE_E;
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
    
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SDBID_B1_H_ */
