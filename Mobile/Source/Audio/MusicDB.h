#ifndef MUSICDB_H
#define MUSICDB_H
/*===========================================================================

              M A 1   M E L O D Y   D A T A B A S E   H E A D E R

DESCRIPTION
  This file contains ma1 melody data

  Copyright (c) 1999 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who      what, where, why
--------   ---      ----------------------------------------------------------
00.08.14   jwpark   YMU757B new API
00.07.25   jwpark   proting to coopper from nui-ori
00.07.13   jwpark   start porting to A10 from A7
           happykid A7

===========================================================================*/




#include "comdef.h"
#include "snd.h"




#ifdef FEATURE_YMU757B_MELODY // jwpark 00.07.25




//////////////////////////////////////////////////////////////////////////////
typedef enum {
    // chojw 00.12.11 - change melody index for datamanager
//  MUS_FIRST_SND = 0,
  MUS_FIRST_SND = 0x47,

  //Music sound
  MUS_ONCEMORE = MUS_FIRST_SND,	/* 0 -  Yesterday once more                  */
//#if (MMI_TYPE != MMI_A10)
  MUS_MUSICBOX1,                /* Music box dancer 1                        */
  MUS_MUSICBOX2,                /* Music box dancer 2                        */
  MUS_RUDOLPH1,					/* Rudolph the red-nosed raindeer 1          */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_RUDOLPH2,					/* Rudolph the red-nosed raindeer 2          */
//#if (MMI_TYPE != MMI_A10)
  MUS_ALONE1,					/* We are all alone 1						 */
  MUS_ALONE2,					/* We are all alone 2						 */
  MUS_SIDEWALK_NY,              /* The sidewalks of New York                 */
  MUS_ANPAN,					/* Anpanman March                            */
  MUS_ARIA1,					/* Aria1                                     */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_ARIA2,					/* Aria2                                     */
//#if (MMI_TYPE != MMI_A10)
  MUS_CAT_TANGO,				/* Black Cat`s Tango                         */
  MUS_WHITE_XMAS,               /* White Christmas                           */
  MUS_DECKHALL1,                /* Deck the hall 1                           */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_DECKHALL2,                /* Deck the hall 2                           */
//#if (MMI_TYPE != MMI_A10)
  MUS_JINGLEBELL1,              /* Jingle bell 1                             */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_JINGLEBELL2,              /* Jingle bell 2                             */
  MUS_CARMEN,                   /* Carmen Suite no.1 "Prelude"               */
  MUS_HALLELLUJAH,              /* Messiah "Hallellujah Chorus"              */
//#if (MMI_TYPE != MMI_A10)
  MUS_TIMEGOES,                 /* Time goes By                              */
  MUS_TOMORROW,					/* Tomorrow                                  */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_CHIMCHIM1,                /* Chim Chim cheree 1                        */
//#if (MMI_TYPE != MMI_A10)
  MUS_CHIMCHIM2,				/* Chim Chim cheree 2                        */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_TURKISH,					/* Turkish rondo                             */
//#if (MMI_TYPE != MMI_A10)
  MUS_VALKYRIE,					/* Valkyrie				                     */
  MUS_DANGO,					/* Dango Brothers							 */
  MUS_BALDMOUNTAIN,				/* A night on bald mountain		             */
  MUS_WHEN_YOU_WISH_UP_ON_A_STAR,/* When you wish up on a star				 */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_SPRING,					/* The four seasons                          */
//#if (MMI_TYPE != MMI_A10)
  MUS_NEW_WORLD,				/* The New world Symphony No.9               */
  MUS_WEDDING1,					/* Wedding March1							 */
  MUS_WEDDING2,					/* Wedding March2							 */
  MUS_KITAYADO,					/* Kitano yadokara					         */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_BABY_ELEPHANT1,			/* Baby elephant walk 1						 */
//#if (MMI_TYPE != MMI_A10)
  MUS_BABY_ELEPHANT2,			/* Baby elephant walk 2						 */
  MUS_LAST_WALTZ1,				/* Last Waltz 1							     */
  MUS_LAST_WALTZ2,				/* Last Waltz 2							     */
  MUS_MELODY_FAIR,				/* Melody Fair								 */
//#endif //#if (MMI_TYPE != MMI_A10)
  MUS_MINUET,					/* L`Arlesiana Suite No.2 "Minuet"           */
//#if (MMI_TYPE != MMI_A10)
  MUS_MICHEL1,					/* Michel 1									 */
  MUS_MICHEL2,					/* Michel 2									 */
  MUS_MOVING_ON,				/* Moving on Without you                     */
  MUS_FOR_A_LONG_TIME,			/* For a long time                           */
  MUS_SCARBOLO_FAIR,			/* Scarbolo fair                             */
//#endif //#if (MMI_TYPE != MMI_A10)

  //Effect Sound
  MUS_PWR_ON,					/* Power On									 */
  MUS_JAPAN_TELEPHONE,			/* Japanese Telephone						 */
  MUS_BICYCLE_BELL,				/* Bell of bicycle				             */
  MUS_JAPAN_FESTIVAL,			/* Japanese festival						 */
  MUS_JINGLE1,					/* Jingle 1								     */
  MUS_JINGLE2,					/* Jingle 2								     */
  MUS_JINGLE3,					/* Jingle 3								     */
  MUS_JINGLE4,					/* Jingle 4								     */
  MUS_JINGLE5,					/* Jingle 5								     */
  MUS_JINGLE6,					/* Jingle 6								     */
  MUS_JINGLE7,					/* Jingle 7								     */
  MUS_PWR_OFF,					/* Power Off							     */
  MUS_JINGLE9,					/* Jingle 9								     */
  MUS_JINGLE10,					/* Jingle 10							     */
  MUS_JINGLE11,					/* Jingle 11							     */
  MUS_JINGLE12,					/* Jingle 12							     */
  MUS_JINGLE13,					/* Jingle 13							     */
  MUS_JINGLE14,					/* Jingle 14							     */
  MUS_CHINESE_NOODLES,			/* Chinese noodles stand music				 */
  MUS_W_BELL,					/* Westminster bell							 */
  MUS_TRAIN,					/* SL										 */
  MUS_CICADAS,					/* Chirring of cicadas                       */
  MUS_CRACTION,					/* Horn										 */
  MUS_RAILROAD,					/* Japanese railroad crossing sound          */
  MUS_TELEPHONE1,				/* Telephone call 1							 */
  MUS_TELEPHONE2,				/* Telephone call 2							 */
  MUS_TELEPHONE3,				/* Telephone call 3							 */
  MUS_TELEPHONE4,				/* Telephone call 4							 */
  MUS_TELEPHONE5,				/* Telephone call 5							 */
  MUS_TELEPHONE6,				/* Telephone call 6							 */
  MUS_TELEPHONE7,				/* Telephone call 7							 */
  MUS_TELEPHONE8,				/* Telephone call 8							 */
  MUS_TELEPHONE9,				/* Telephone call 9							 */
  MUS_CUCKOO,					/* Japanese cuckoo							 */
  MUS_WIND_CLOCK,				/* Wind up the Clock						 */
  MUS_WIND_CRICKET,				/* Chirring of cricket                       */
  MUS_WAVE,						/* Ocean waves								 */
  MUS_AMBULANCE,				/* Japanese ambulence						 */

	/* Old Melody Type  */
  MUS_BASIC,
  MUS_SEA_SYMPHONY ,
  MUS_CANNON,
  MUS_FRANCE_SUITE6,
  MUS_MAGIC,
  MUS_MUSETT,
  MUS_MINUET_BACH,
  MUS_BACAROL,
  MUS_JOY_LOVE,
  MUS_LOSSIGNOL,
  MUS_FRANCE_SUITE2,
  MUS_FRANCE_SUITE1,
  MUS_FRANCE_SUITE3,
  MUS_FRANCE_SUITE4,
  MUS_FRANCE_SUITE5,
  MUS_MINUET_MOZART,
  MUS_AVE_VERUM,

  //Kim`s media  ¡¶¿€∞Ó
  MUS_PEERGYNT,                 // Null Data
  MUS_SONGSPRING,
  MUS_TURANDOT,
  MUS_VIOLINROMANCE,
  MUS_PATHETIQUE,
  MUS_FALL,                   // Null Data
  MUS_WARSAW,                 // Null Data
  MUS_AIDA,
  MUS_PATHETIQUE_SYM,         // Null Data
  MUS_SOUSA1,
  MUS_SOUSA2,
  MUS_SOUSA3,
  MUS_1007,
  MUS_IGOR,
  MUS_MARCH,
  MUS_STRANGER,
  MUS_AMEPARIS,               // Null Data
  MUS_GIANT,
  MUS_GONEWIND,               // Null Data
  MUS_GUMBAM,
  MUS_TRUMPET,
  MUS_SABREDANCE,
  MUS_MASCAGNI,               // Null Data
  MUS_WEDD,                   // Null Data
  MUS_VIOLINC,                // Null Data 
  MUS_CZARDAS,                // Null Data
  MUS_NOCTURN,
  MUS_CANCAN,
  MUS_PIANOC,                 // Null Data
  MUS_ARANPEZ,                // Null Data
  MUS_ROMANCE,                // Null Data
  MUS_CYNGNE,
  MUS_GYMNOPEDIES,            // Null Data
  MUS_SIBELIUS1,              // Null Data
  MUS_SIBELIUS2,              // Null Data
  MUS_NEWTOWN, 
  MUS_GOLDEN,
  MUS_CONGRATULATION,
  MUS_CUTEBOY,
  MUS_NARANHI,
  MUS_UMBRELLA,
  MUS_YELLOWRIBBON,
  MUS_YODDLE,
  MUS_MONGGUMPO,
  MUS_MILYANG,
  MUS_HARVEST,
  MUS_GUDDY,
  MUS_SILENTNIGHT,
  MUS_JINGLEBELL,             // Null Data
  MUS_JOY,
  MUS_MERRYXMAS,
  MUS_MENUET,
  MUS_JOYLOVE,                // Null Data
  MUS_MAGICFLUTE,
  MUS_CANON,
  MUS_FANTASY,
  MUS_TROUT,
  MUS_HUMORESQUE,
  MUS_DREAM,
  MUS_FLOWER,
  MUS_BIRD,                   // Null Data
  MUS_TOREDOR,
  MUS_SPRING2,                // Null Data
  MUS_TURKEY,
  MUS_SWANLAKE,               // Null Data
  MUS_WILLIAMTELL,
  MUS_TOY,
  MUS_FUNIKULA,
  MUS_HYPOPIPO,
  MUS_WALTZ,                  // Null Data
  MUS_ARABESQUE,              // Null Data
  MUS_DOREMI,
  MUS_FORELISE,
  MUS_CHICKY,                 // Null Data
  MUS_PATHETIQUE_SONATA,
  MUS_LADETZKY,
  MUS_DONAU1,                 // Null Data
  MUS_DONAU2,
  MUS_SOUNDSPRING,

  MUS_ROSE,                   // Null Data
  MUS_ALHAMBRA,               // Null Data
  MUS_FRANCE2,                // Null Data
  MUS_BACHMENUET,             // Null Data

  // mbz for game music & sound
  MUS_DDG_THEME_SOUND, 
  MUS_DDG_BORN_SOUND,
  MUS_DDG_SHOT_SOUND,
  MUS_MPG_THEME_SOUND,
  MUS_MPG_CAR_BLASTER_SOUND,
  MUS_MPG_UFO_BLASTER_SOUND,
  MUS_MPG_UFO_SHOT_SOUND,
  MUS_MPG_STONE_BLASTER_SOUND,
  MUS_MPG_JUMP_SOUND,
  MUS_MPG_SHOT_SOUND,
  MUS_SAM_THEME_SOUND,
  MUS_PZG_THEME_SOUND,
  MUS_SMS_BELL1,
  MUS_SMS_BELL2,
  MUS_SMS_BELL3,
  MUS_SMS_BELL4,
  MUS_SMS_BELL5,
  MUS_SMS_BELL6,
  MUS_SMS_BELL7,
  MUS_LOW_BATT,

  MUS_LAST_SND = MUS_LOW_BATT
} mus_id_type;



//////////////////////////////////////////////////////////////////////////////
uint16 *MUSIC_get_music_data(mus_id_type music_id);
sint15 MUSIC_get_music_gain_offset(mus_id_type music_id);
#endif // #ifdef FEATURE_YMU757B_MELODY // jwpark 00.07.25 end



#endif // #ifndef MUSICDB_H