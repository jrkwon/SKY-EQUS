/*===========================================================================

                 M A 1   M E L O D Y   D A T A B A S E

DESCRIPTION
  This file contains ma2 melody data

  Copyright (c) 1999 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.08.02    tasik   create
===========================================================================*/

/*
	아래의 정의 혹은 함수는 모두 수정되어야 한다. 특히 *.mad 파일은 모두 ma1 
	디렉토리에서 공유되어 있는데 이것들은 모두 ma2라는 디렉토리를 만들어서
	새로이 정의되어야 하는 것들이다.

    현재는 컴파일 에러를 피하기 위해서 잠정적으로 ma1의 정의를 그대로 사용하기
	로 한다. 
*/




#include "..\Qualcomm30\comdef.h"
#include "..\Qualcomm30\snd.h"



#ifdef FEATURE_YMU759_MELODY 
#include "..\Audio\YMU759.h"
#include "..\Audio\MusicDB_B1.h"


///////////////////////////////////
///////1차 멜로디 작업리스트
///////////////////////////////////



const byte a8400[] = {
    #include "ma2\08400.mad"
};

const byte a8961[] = {
    #include "ma2\08961.mad"
};

const byte a130[] = {
    #include "ma2\00130.mad"
};

const byte a316[] = {
    #include "ma2\00316.mad"
};

const byte a697[] = {
    #include "ma2\00697.mad"
};

const byte a4593[] = {
    #include "ma2\04593.mad"
};

const byte a2490[] = {
    #include "ma2\02490.mad"
};

const byte a4585[] = {
    #include "ma2\04585.mad"
};

const byte a267[] = {
    #include "ma2\00267.mad"
};

const byte a3195[] = {
    #include "ma2\03195.mad"
};
 
const byte c70017[] = {
    #include "ma2\c-70017.mad"
};

const byte c70029[] = {
    #include "ma2\c-70029.mad"
};

/*
const byte c70029[] = {
    #include "ma2\c-70029.mad"
};
*/

const byte c70001[] = {
    #include "ma2\c-70001.mad"
};

const byte c70002[] = {
    #include "ma2\c-70002.mad"
};

const byte c70003[] = {
    #include "ma2\c-70003.mad"
};

const byte c70004[] = {
    #include "ma2\c-70004.mad"
};

const byte c70023[] = {
    #include "ma2\c-70023.mad"
};

const byte c70026[] = {
    #include "ma2\c-70026.mad"
};

const byte c70000[] = {
    #include "ma2\c-70000.mad"
};

/*
const byte c70026[] = {
    #include "ma2\c-70026.mad"
};
*/

const byte c70007[] = {
    #include "ma2\c-70007.mad"
};

const byte c70008[] = {
    #include "ma2\c-70008.mad"
};

const byte c70009[] = {
    #include "ma2\c-70009.mad"
};

/*
const byte c70009[] = {
    #include "ma2\c-70009.mad"
};
*/

const byte c70025[] = {
    #include "ma2\c-70025.mad"
};

const byte c70018[] = {
    #include "ma2\c-70018.mad"
};

const byte c70011[] = {
    #include "ma2\c-70011.mad"
};

const byte c70010[] = {
    #include "ma2\c-70010.mad"
};

const byte c70013[] = {
    #include "ma2\c-70013.mad"
};

const byte c70027[] = {
    #include "ma2\c-70027.mad"
};

const byte c70032[] = {
    #include "ma2\c-70032.mad"
};

const byte c70016[] = {
    #include "ma2\c-70016.mad"
};

const byte c70024[] = {
    #include "ma2\c-70024.mad"
};

const byte c70031[] = {
    #include "ma2\c-70031.mad"
};

const byte c70028[] = {
    #include "ma2\c-70028.mad"
};

const byte a2481[] = {
    #include "ma2\02481.mad"
};

const byte a2516[] = {
    #include "ma2\02516.mad"
};

///////////////////////////////////////////
//////// 2차 멜로디 작업 분 이길수 
///////////////////////////////////////////

const byte a2660[] = {
    #include "ma2\2660.mad"
};

const byte a2662[] = {
    #include "ma2\2662.mad"
};

const byte a2663[] = {
    #include "ma2\2663.mad"
};

const byte a3068[] = {
    #include "ma2\3068.mad"
};

const byte w_80000[] = {
    #include "ma2\w-80000.mad"
};

///////////////////////////////////////////
////////// 3월 15일 멜로디 추가
///////////////////////////////////////////

const byte a00124[] = {
    #include "ma2\00124.mad"
};

const byte f00155[] = {
    #include "ma2\00155.mad"
};

const byte a00189[] = {
    #include "ma2\00189.mad"
};

const byte k00344[] = {
    #include "ma2\00344.mad"
};

const byte f00377[] = {
    #include "ma2\00377.mad"
};

const byte a00420[] = {
    #include "ma2\00420.mad"
};

const byte f00482[] = {
    #include "ma2\00482.mad"
};

const byte a00505[] = {
    #include "ma2\00505.mad"
};

const byte a00703[] = {
    #include "ma2\00703.mad"
};

const byte f00752[] = {
    #include "ma2\00752.mad"
};

const byte k01635[] = {
    #include "ma2\01635.mad"
};

const byte a02491[] = {
    #include "ma2\02491.mad"
};

const byte e02628[] = {
    #include "ma2\02628.mad"
};

const byte e02637[] = {
    #include "ma2\02637.mad"
};

const byte a03111[] = {
    #include "ma2\03111.mad"
};

//////////////////////////////////////사람 목소리
const byte w80007[] = {
    #include "ma2\w-80007.mad"
};

const byte w80008[] = {
    #include "ma2\w-80008.mad"
};

const byte w80009[] = {
    #include "ma2\w-80009.mad"
};

const byte w80010[] = {
    #include "ma2\w-80010.mad"
};

const byte w80011[] = {
    #include "ma2\w-80011.mad"
};

const byte w80012[] = {
    #include "ma2\w-80012.mad"
};


////////////////////////////////
///// 3월 19일 추가분 //////////
////////////////////////////////

const byte k04582[] = {
    #include "ma2\04582.mad"
};


const byte k04506[] = {
    #include "ma2\04506.mad"
};


const byte f04561[] = {
    #include "ma2\04561.mad"
};


const byte k02820[] = {
    #include "ma2\02820.mad"
};


const byte k04627[] = {
    #include "ma2\04627.mad"
};


const byte k05001[] = {
    #include "ma2\05001.mad"
};


const byte e04491[] = {
    #include "ma2\04491.mad"
};


const byte c70020[] = {
    #include "ma2\c-70020.mad"
};


const byte c70005[] = {
    #include "ma2\c-70005.mad"
};


const byte c70019[] = {
    #include "ma2\c-70019.mad"
};


const byte c70012[] = {
    #include "ma2\c-70012.mad"
};


const byte c70014[] = {
    #include "ma2\c-70014.mad"
};


const byte c70015[] = {
    #include "ma2\c-70015.mad"
};


////////////////////////////////
//////////// SMS 벨 ////////////
////////////////////////////////

const byte sms01[] = {
    #include "ma2\sms01.mad"
};


const byte sms02[] = {
    #include "ma2\sms02.mad"
};


const byte sms03[] = {
    #include "ma2\sms03.mad"
};


const byte sms04[] = {
    #include "ma2\sms04.mad"
};


const byte sms05[] = {
    #include "ma2\sms05.mad"
};


const byte sms07[] = {
    #include "ma2\sms07.mad"
};

//////////////////////////////////////////////
///////////  3월 19, 20 일 분 멜로디 추가
//////////////////////////////////////////////


const byte c70073[] = {
    #include "ma2\c-70073.mad"
};


const byte c70074[] = {
    #include "ma2\c-70074.mad"
};


const byte c70075[] = {
    #include "ma2\c-70075.mad"
};


const byte c70076[] = {
    #include "ma2\c-70076.mad"
};


const byte c70077[] = {
    #include "ma2\c-70077.mad"
};


const byte c70078[] = {
    #include "ma2\c-70078.mad"
};


const byte c70079[] = {
    #include "ma2\c-70079.mad"
};


const byte c70080[] = {
    #include "ma2\c-70080.mad"
};


const byte c70081[] = {
    #include "ma2\c-70081.mad"
};


const byte c70082[] = {
    #include "ma2\c-70082.mad"
};


const byte w03067[] = {
    #include "ma2\w-03067.mad"
};


////////////////////////////////
///////  3월 22일 추가분 
///////////////////////////////


const byte w80002[] = {
    #include "ma2\w-80002.mad"
};


const byte bell1[] = {
    #include "ma2\bell-1.mad"
};


const byte bell2[] = {
    #include "ma2\bell-2.mad"
};

////////////////////////////////
///////  4월 11일 추가분 SMS 벨 
///////////////////////////////


const byte Dreamharp10[] = {
    #include "ma2\Dreamharp10.mad"
};

const byte Game[] = {
    #include "ma2\game.mad"
};
///////////////////////////////
////// 4월 19일 추가분
////////////////////////////////

const byte error1[] = {
    #include "ma2\error1.mad"
};

const byte error2[] = {
    #include "ma2\error2.mad"
};

const byte error3[] = {
    #include "ma2\error3.mad"
};

const byte error4[] = {
    #include "ma2\error4.mad"
};

const byte error5[] = {
    #include "ma2\error5.mad"
};

const byte error6[] = {
    #include "ma2\error6.mad"
};

const byte error7[] = {
    #include "ma2\error7.mad"
};

const byte error8[] = {
    #include "ma2\error8.mad"
};

const byte error9[] = {
    #include "ma2\error9.mad"
};



///////////////////////////////////4월 23일 추가 

const byte denwa1[] = {
    #include "ma2\denwa1.mad"
};


//////////////////////////////////4월 26일 추가 

const byte dreamend10[] = {
    #include "ma2\dreamend10.mad"
};

const byte Dreamend11[] = {
    #include "ma2\Dreamend11.mad"
};



//////////////////////////////////5월 4일 추가

const byte shutter[] = {
    #include "ma2\shutter.mad"
};


//////////////////////////////////7월 2일 추가
const byte Allert2[] = {
    #include "ma2\Allert2.mad"
};
const byte Etiq[] = {
    #include "ma2\Etiq.mad"
};
const byte dding[] = {
    #include "ma2\dding.mad"
};






///////////////////////////////
// 기본전화벨
/////////////////////////
const byte denwa2[] = {
    #include "ma2\denwa2.mad"
};

const byte denwa5[] = {
    #include "ma2\denwa5.mad"
};

const byte denwa8[] = {
    #include "ma2\denwa8.mad"
};

const byte denwa9[] = {
    #include "ma2\denwa9.mad"
};

const byte kuroden[] = {
    #include "ma2\kuroden.mad"
};
//////////////////////////////////////////////
 
const MUS_DATA_TYPE_T MUSIC_ma2_db[] = {
    {denwa2, (sint15)0x1f},       // for DUMMY

    // 기본전화벨(A7)
    {denwa2, (sint15)0x1f},       // MUS_DENWA2
    {denwa5, (sint15)0x1f},       // MUS_DENWA5
    {denwa8, (sint15)0x1f},       // MUS_DENWA8
    {denwa9, (sint15)0x1f},       // MUS_DENWA9
    {kuroden, (sint15)0x1d},       // MUS_KURODEN

    // 금영 멜로디
    {a8400, (sint15)0x1A},       // MUS_DOREMI
    {a8961, (sint15)0x1A},       // MUS_ELEPHANTWALK
	{a130, (sint15)0x1C},        // MUS_SILENT_NIGHT

    
	{a316, (sint15)0x1C},        // MUS_RODOLF_NOSE
    {a697, (sint15)0x17},        // MUS_JINGLEBELL
    {a4593, (sint15)0x19},       // MUS_WEDDING_MARCH
    {a2490, (sint15)0x17},       // MUS_BIRTHDAY_PANG
    {a4585, (sint15)0x1c},       // MUS_CORONATION_MARCH
    {a267, (sint15)0x1B},        // MUS_MOON_TARYUNG
    {a3195, (sint15)0x1A},       // MUS_ULYBULY
    {c70017, (sint15)0x19},      // MUS_FLOWER_WALTZ
    {c70029, (sint15)0x1C},      // MUS_ALHAMBRA
	{c70001, (sint15)0x17},      // MUS_LAD_MARCH
    {c70002, (sint15)0x1c},      // MUS_VIOLIN_ROMANCE
    {c70003, (sint15)0x17},      // MUS_VOKERINI_MINUET
    {c70004, (sint15)0x1E},      // MUS_SPRING_SONG
    {c70023, (sint15)0x18},      // MUS_4SEAS_SPRING
    {c70026, (sint15)0x1D},      // MUS_WOMEN_HEART

    {c70000, (sint15)0x1a},      // MUS_TRIUMPH_MARCH
    {c70007, (sint15)0x1F},      // MUS_NOCTURNE
    {c70008, (sint15)0x1d},      // MUS_4ELIZE
    {c70009, (sint15)0x17},      // MUS_MAGIC_FLUTE
    {c70025, (sint15)0x15},      // MUS_MAJESTIC_MARCH
    {c70018, (sint15)0x16},      // MUS_WILLIAM_PRELUDE
	{c70011, (sint15)0x19},      // MUS_KANGKANG
    {c70010, (sint15)0x1C},      // MUS_CARMEN_PRELUDE

	{c70013, (sint15)0x1a},      // MUS_MATADOR_SONG
    {c70027, (sint15)0x1C},      // MUS_CANON
	{c70032, (sint15)0x17},      // MUS_LOVE_PLEASURE
    {c70016, (sint15)0x16},      // MUS_ARIA_G
    {c70024, (sint15)0x16},      // MUS_PERGUINT_MORNING
    {c70031, (sint15)0x17},      // MUS_MULLET
    {c70028, (sint15)0x1a},      // MUS_SANTALUTIA
	{a2481, (sint15)0x1B},       // MUS_PANG
    {a2516, (sint15)0x1C},       // MUS_PANG1

///////////////////////금영 2차 작업분
    {a2660, (sint15)0x18},       // MUS_COPTER_SOUND
	{a2662, (sint15)0x1C},       // MUS_BIRD_SOUND
	{a2663, (sint15)0x1A},       // MUS_WAVE_SOUND
	{a3068, (sint15)0x1F},	      // MUS_TRAIN_SOUND
	{w_80000, (sint15)0x19},	  // MUS_KNOCK_SOUND

////////////////////////3월 15일 추가분 

	{a00124, (sint15)0x17},      // MUS_MARRIAGE_MARCH
	{f00155, (sint15)0x1A},      // MUS_CHESTNUT_TARYUNG
	{a00189, (sint15)0x17},      // MUS_JOYFUL_JESUS_COME
	{k00344, (sint15)0x19},      // MUS_FARM_ROAD
	{f00377, (sint15)0x19},      // MUS_MILYANG_ARIRANG
	{a00420, (sint15)0x18},	  // MUS_DRUM_BOY	
	{f00482, (sint15)0x19},	  // MUS_BIRD_TARYUNG
	{a00505, (sint15)0x19},      // MUS_OLDYEAR_OUT
	{a00703, (sint15)0x18},	  // MUS_LOOKOUT_WINDOW

	{f00752, (sint15)0x19},	  // MUS_PUNGNYEONGA
	{k01635, (sint15)0x18},	  // MUS_YOUNG_BAND
	{a02491, (sint15)0x16},	  // MUS_SALUTE_NATIONALFLAG
	{e02628, (sint15)0x1F},	  // MUS_WIND_SOUND
	{e02637, (sint15)0x1D},	  // MUS_GUN_SOUND
	{a03111, (sint15)0x17},      // MUS_NOEL	

	///////////////인간의 음성//////////////////////
	{w80007, (sint15)0x1B},	  // MUS_HE_WOW
	{w80008, (sint15)0x19},	  // MUS_HE_HURRY
	{w80009, (sint15)0x1B},	  // MUS_HE_RING
	{w80010, (sint15)0x18},	  // MUS_SHE_WOW
	{w80011, (sint15)0x18},	  // MUS_SHE_HURRY
	{w80012, (sint15)0x19},      // MUS_SHE_RING

////////////////////////3월 19일 추가분 
	{k04582, (sint15)0x1a},	  // MUS_TING_A_LING
	{k04506, (sint15)0x18},	  // MUS_MOTHER_PIG_CHILD_PIG	
	{f04561, (sint15)0x18},	  // MUS_GOOD
	{k02820, (sint15)0x18},	  // MUS_CUTY_KID
	{k04627, (sint15)0x18},	  // MUS_SUN_BLAZING
	{k05001, (sint15)0x18},	  // MUS_INVADE	
	{e04491, (sint15)0x16},	  // MUS_OPENING_PANG
	{c70020, (sint15)0x18},	  // MUS_4SEAS_WINTER
	{c70005, (sint15)0x1A},	  // MUS_PATHOS_SONATA
	{c70019, (sint15)0x1A},	  // MUS_HUMORESQUE
	{c70012, (sint15)0x19},	  // MUS_TURKEY_MARCH
	{c70014, (sint15)0x1B},	  // MUS_HAYDN_TRUMPET
	{c70015, (sint15)0x18},   // MUS_HALLELUJAH
/*아직 결정되지 않았음*/	
	{sms01, (sint15)0x1c}, 	  // MUS_SMS_1 
	{sms02, (sint15)0x1c}, 	  // MUS_SMS_2
	{sms03, (sint15)0x1c}, 	  // MUS_SMS_3
	{sms04, (sint15)0x1c}, 	  // MUS_SMS_4
	{sms05, (sint15)0x1c}, 	  // MUS_SMS_5
	{sms07, (sint15)0x1c},    // MUS_SMS_7
/*아직 결정되지 않았음*/
////////////////////////////////////////////////3월 20일 추가분 
	{c70073, (sint15)0x17},  	  // MUS_STAR
	{c70074, (sint15)0x19}, 	  // MUS_DONAU_WALTZ
	{c70075, (sint15)0x1A},  	  // MUS_TOY_SYMPHONY
	{c70076, (sint15)0x1B},  	  // MUS_COMEBACK_SOTRENTO
	{c70077, (sint15)0x1A},  	  // MUS_CHICK_DANCE
	{c70078, (sint15)0x17},  	  // MUS_FINLANDIA
/*아직 결정되지 않았음*/	
	{c70079, (sint15)0x1B},  	  // MUS_MAIDEN_PRAYER
/*아직 결정되지 않았음*/
	{c70080, (sint15)0x17},  	  // MUS_NEWLAND_SYMPHONY
	{c70081, (sint15)0x18},  	  // MUS_WASHINGTON
	{c70082, (sint15)0x1A},  	  // MUS_SILVERY_WAVE
	{w03067, (sint15)0x1c},  	  // MUS_CAR_SOUND

/////////////////////////////////////////////////3월 22일 추가분 
	{w80002, (sint15)0x18},  	  // MUS_PUPPY_SOUND
	{bell1, (sint15)0x17},  	  // MUS_HE_PLUS_RING
	{bell2, (sint15)0x17},  	  // MUS_SHE_PLUS_MUSIC

///////////////////////////////////////////////////////////4월 11일 SMS멜로디 추가
	{Dreamharp10, (sint15)0x1c},  	  // MUS_SMS_6

///////////////////////////////////////////////////////////4월 19일 추가분
	{error1, (sint15)0x1c},  	  // MUS_ERROR_1
	{error2, (sint15)0x1c},  	  // MUS_ERROR_2
	{error3, (sint15)0x1c},  	  // MUS_ERROR_3
	{error4, (sint15)0x1c},  	  // MUS_ERROR_4
	{error5, (sint15)0x1c},  	  // MUS_ERROR_5
	{error6, (sint15)0x1c},  	  // MUS_ERROR_6
	{error7, (sint15)0x1c},  	  // MUS_ERROR_7
	{error8, (sint15)0x1c},   	  // MUS_ERROR_8
	{error9, (sint15)0x1c},   	  // MUS_ERROR_8

	{denwa1, (sint15)0x1c},   	  // MUS_DENWA_1

	{dreamend10, (sint15)0x1c},   // MUS_DREAMEND_10
	{Dreamend11, (sint15)0x1c},   // MUS_DREAMEND_11

/////////////////////////////////////////////////////////////5월 4일 추가분
	{shutter, (sint15)0x1c},    // MUS_SHUTTER

/////////////////////////////////////////////////////////////5월 4일 추가분
	{Allert2, (sint15)0x1c},    // MUS_ALLERT2
	{Etiq,    (sint15)0x1c},    // MUS_ETIQUETTE
    {dding,   (sint15)0x1c}     // MUS_DDING

};

const byte *MUSIC_get_music_data(MUS_ID_TYPE_E  MusicID)
{
    if(MusicID < MUS_FIRST_SND)
        return NULL;

    return MUSIC_ma2_db[MusicID].pData;
}

sint15 MUSIC_get_music_volume(MUS_ID_TYPE_E  MusicID)
{
    if(MusicID < MUS_FIRST_SND)
        return 0;

    return MUSIC_ma2_db[MusicID].vol;
}
#endif // #ifdef FEATURE_YMU759_MELODY 
