/*===========================================================================

                 M A 1   M E L O D Y   D A T A B A S E

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
00.07.25   jwpark   porting to cooper from nui-ori
00.07.13   jwpark   start porting to A10 from A7
           happykid A7

===========================================================================*/



#include "comdef.h"
#include "snd.h"



#ifdef FEATURE_YMU757B_MELODY // jwpark 00.07.25
#include "YMU757B.h"
#include "MusicDB.h"



////////////////////////////////////////////////////////////////////////////
#if (MMI_TYPE != MMI_A10)
const uint16 aior1[] = {
	#include "ma1\aior1.mad"
};
const uint16 aior2[] = {		
	#include "ma1\aior2.mad"
};
const uint16 akahan1[] = {	
	#include "ma1\akahan1.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)

const uint16 akahan2[] = {
	#include "ma1\akahan2.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 alone1[] = {		
	#include "ma1\alone1.mad"
};
const uint16 alone2[] = {		
	#include "ma1\alone2.mad"
};
const uint16 ame[] = {		
	#include "ma1\ame.mad"
};
const uint16 anpan[] = {
	#include "ma1\anpan.mad"
};
const uint16 aria1[] = {
	#include "ma1\aria1.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)

const uint16 aria2[] = {
	#include "ma1\aria2.mad"
};

#if (MMI_TYPE != MMI_A10)
const uint16 B_tango[] ={
	#include "ma1\B_tango.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)

const uint16 Bicycle[] ={
	#include "ma1\Bicycle.mad"
};
const uint16 bonodori[] = {
	#include "ma1\bonodori.mad"
};
const uint16 carmen[] = {
	#include "ma1\carmen.mad"
};
const uint16 catch1[] = {
	#include "ma1\catch1.mad"
};
const uint16 catch2[] = {
	#include "ma1\catch2.mad"
};
const uint16 catch3[] = {
	#include "ma1\catch3.mad"
};
const uint16 catch4[] = {
	#include "ma1\catch4.mad"
};
const uint16 catch5[] = {
	#include "ma1\catch5.mad"
};
const uint16 catch6[] = {	
	#include "ma1\catch6.mad"
};
const uint16 catch7[] = {
	#include "ma1\catch7.mad"
};
const uint16 catch8[] = {
	#include "ma1\catch8.mad"
};
const uint16 catch9[] = {
	#include "ma1\catch9.mad"
};
const uint16 catch10[] = {
	#include "ma1\catch10.mad"
};
const uint16 catch11[] = {
	#include "ma1\catch11.mad"
};
const uint16 catch12[] = {
	#include "ma1\catch12.mad"
};
const uint16 catch13[] = {
	#include "ma1\catch13.mad"
};
const uint16 catch14[] = {
	#include "ma1\catch14.mad"
};
const uint16 charumer[] = {
	#include "ma1\charumer.mad"
};
const uint16 chime[] = {
	#include "ma1\chime.mad"
};
const uint16 cimu1[] = {
	#include "ma1\cimu1.mad"
};

#if (MMI_TYPE != MMI_A10)
const uint16 cimu2[] = {
	#include "ma1\cimu2.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)

const uint16 Craction[] = {
	#include "ma1\Craction.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Dango3[] = {
	#include "ma1\Dango3.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Denwa1[] = {
	#include "ma1\Denwa1.mad"
};
const uint16 Denwa2[] = {
	#include "ma1\Denwa2.mad"
};
const uint16 Denwa3[] = {
	#include "ma1\Denwa3.mad"
};
const uint16 Denwa4[] = {
	#include "ma1\Denwa4.mad"
};
const uint16 Denwa5[] = {
	#include "ma1\Denwa5.mad"
};
const uint16 Denwa6[] = {
	#include "ma1\Denwa6.mad"
};
const uint16 Denwa7[] = {
	#include "ma1\Denwa7.mad"
};
const uint16 Denwa8[] = {
	#include "ma1\Denwa8.mad"
};
const uint16 Denwa9[] = {
	#include "ma1\Denwa9.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Hageyama[] = {
	#include "ma1\Hageyama.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Hallelus[] = {
	#include "ma1\Hallelus.mad"
};

#if (MMI_TYPE != MMI_A10)
const uint16 Hiirag1[] = {
	#include "ma1\Hiirag1.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)

const uint16 Hiirag2[] = {
	#include "ma1\Hiirag2.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Hoshi[] = {
	#include "ma1\Hoshi.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Humikiri[] = {
	#include "ma1\Humikiri.mad"
};

#if (MMI_TYPE != MMI_A10)
const uint16 Jing1[] = {
	#include "ma1\Jing1.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)

const uint16 Jing2[] = {
	#include "ma1\Jing2.mad"
};
const uint16 Kakko[] = {
	#include "ma1\Kakko.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Kekkon1[] = {
	#include "ma1\Kekkon1.mad"
};
const uint16 Kekkon2[] = {
	#include "ma1\Kekkon2.mad"
};
const uint16 Kitayado[] = {
	#include "ma1\Kitayado.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Koorogi[] = {
	#include "ma1\Koorogi.mad"
};
const uint16 Kozo1[] = {
	#include "ma1\Kozo1.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Kozo2[] = {
	#include "ma1\Kozo2.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Kuroden[] = {
	#include "ma1\Kuroden.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Lastw1[] = {
	#include "ma1\Lastw1.mad"
};
const uint16 Lastw2[] = {
	#include "ma1\Lastw2.mad"
};
const uint16 Melody[] = {		
	#include "ma1\Melody.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Menuet[] = {
	#include "ma1\Menuet.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Miche1[] = {
	#include "ma1\Miche1.mad"
};
const uint16 Miche2[] = {
	#include "ma1\Miche2.mad"
};
const uint16 Movin_o2[] = {
	#include "ma1\Movin_o2.mad"
};
const uint16 Nagai[] = {
	#include "ma1\Nagai.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Oncemo[] = {
	#include "ma1\Oncemo.mad"
};
const uint16 Osirase[] = {	
	#include "ma1\Osirase.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Scab[] = {
	#include "ma1\Scab.mad"
};
const uint16 Sinsekai[] = {
	#include "ma1\Sinsekai.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Sl[] = {
	#include "ma1\Sl.mad"
};
const uint16 Spring[] = {
	#include "ma1\Spring.mad"
};

#if (MMI_TYPE != MMI_A10)
const uint16 Timegoes[] = {
	#include "ma1\Timegoes.mad"
};
const uint16 Tomor[] = {
	#include "ma1\Tomor.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)

const uint16 Tukutuku[] = {	
	#include "ma1\Tukutuku.mad"
};
const uint16 Turkish[] = {
	#include "ma1\Turkish.mad"
};
#if (MMI_TYPE != MMI_A10)
const uint16 Valkyrie[] = {
	#include "ma1\Valkyrie.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)
const uint16 Wave[] = {
	#include "ma1\Wave.mad"
};

#if (MMI_TYPE != MMI_A10)
const uint16 White[] = {
	#include "ma1\White.mad"
};
#endif //#if (MMI_TYPE != MMI_A10)

const uint16 Zenmai[] = {
	#include "ma1\Zenmai.mad"
};
const uint16 Amblc[] = {
	#include "ma1\Amblc.mad"
};


// mbz for game music &,  sound
const uint16 ddg_theme[] = {	
	#include "game_mad\dtheme.mad" 
}; 
const uint16 ddg_born[] = {	
	#include "game_mad\ddgborn.mad" 
}; 
const uint16 ddg_shot[] = {
	#include "game_mad\dshot.mad" 
}; 
const uint16 mpg_theme[] = {	
	#include "game_mad\mtheme.mad"
}; 
const uint16 mpg_car_blaster[] = {	
	#include "game_mad\mcarb.mad" 
}; 
const uint16 mpg_ufo_blaster[] = {	
	#include "game_mad\mufob.mad" 
}; 
const uint16 mpg_ufo_shot[] = {	
	#include "game_mad\mufos.mad" 
}; 
const uint16 mpg_stone_blaster[] = {	
	#include "game_mad\mstoneb.mad" 
};
const uint16 mpg_jump[] = {	
	#include "game_mad\mjump.mad" 
}; 
const uint16 mpg_shot[] = {	
	#include "game_mad\mshot.mad" 
}; 
const uint16 sam_theme[] = {	
	#include "game_mad\stheme.mad" 
}; 
const uint16 pzg_theme[] = {	
	#include "game_mad\ptheme.mad" 
}; 


const uint16 sms_bell1[] = {	
	#include "ma1\sms1.mad" 
}; 
const uint16 sms_bell2[] = {	
	#include "ma1\sms2.mad" 
}; 
const uint16 sms_bell3[] = {	
	#include "ma1\sms3.mad" 
}; 
const uint16 sms_bell4[] = {	
	#include "ma1\sms4.mad" 
}; 
const uint16 sms_bell5[] = {	
	#include "ma1\sms5.mad" 
}; 
const uint16 sms_bell6[] = {	
	#include "ma1\sms6.mad" 
}; 
const uint16 sms_bell7[] = {	
	#include "ma1\sms7.mad" 
}; 
const uint16 low_vbatt[] = {	
	#include "ma1\batt.mad" 
}; 

// Kim's media
/*
const uint16 PeerGyntMorning[] = {		
	#include "kims\grieg-~1.mad"
};
*/
const uint16 SongofSpring[] = {		
	#include "kims\mendel~2.mad"
};
/*
const uint16 Turando[] = {		
	#include "kims\puccin~1.mad"
};
*/
const uint16 ViolinRomance[] = {		
	#include "kims\romanc.mad"
};
const uint16 Pathetique[] = {		
	#include "kims\sonata.mad"
};
/*
const uint16 fall[] = {		
	#include "kims\vivald~1.mad"
};
const uint16 warsaw[] = {		
	#include "kims\warsaw~1.mad"
};
*/
const uint16 aida[] = {		
	#include "kims\verdi-~1.mad"
};
/*
const uint16 Tchaikov[] = {		
	#include "kims\Tchaik~1.mad"
};
*/
const uint16 star[] = {		
	#include "kims\sousa-~1.mad"
};
const uint16 livertybell[] = {		
	#include "kims\sousa-~2.mad"
};
const uint16 washington[] = {		
	#include "kims\sousa-~3.mad"
};
const uint16 cello[] = {		
	#include "kims\1007su~1.mad"
};
const uint16 igor[] = {		
	#include "kims\borodi~1.mad"
};
const uint16 pumpmarch[] = {		
	#include "kims\elgar-~1.mad"
};
const uint16 gabriel[] = {		
	#include "kims\gabrie~1.mad"
};
/*
const uint16 AmericanParis[] = {		
	#include "kims\gershw~1.mad"
};
*/
const uint16 giant[] = {		
	#include "kims\giant.mad"
};
/*
const uint16 gonewind[] = {		
	#include "kims\gonewind.mad"
};
*/
const uint16 gumbam[] = {		
	#include "kims\gumbam.mad"
};
const uint16 trumpetconcert[] = {		
	#include "kims\hiden-~1.mad"
};
const uint16 sabredance[] = {		
	#include "kims\khacha~1.mad"
};
/*
const uint16 mascagni[] = {		
	#include "kims\mascag~1.mad"
};
*/
const uint16 wed[] = {
	#include "kims\mendel~1.mad"
};
const uint16 violin[] = {
	#include "kims\mendel~3.mad"
};
/*
const uint16 czardas[] = {
	#include "kims\monti-~1.mad"
};
*/
const uint16 nocturn2[] = {
	#include "kims\noctur~1.mad"
};
const uint16 cancan[] = {
	#include "kims\offenb~1.mad"
};
/*
const uint16 pianoc[] = {
	#include "kims\pianoc~1.mad"
};
const uint16 aranpez[] = {
	#include "kims\rodrig~1.mad"
};
const uint16 romance[] = {
	#include "kims\romanc~1.mad"
};
*/
const uint16 cygnus[] = {
	#include "kims\saints~1.mad"
};
/*
const uint16 gymnopedie[] = {
	#include "kims\satie-~1.mad"
};
const uint16 finlandia[] = {
	#include "kims\sibeli~1.mad"
};
const uint16 kuolema[] = {
	#include "kims\sibeli~2.mad"
};
*/
const uint16 newtown[] = {
	#include "kims\003.mad"
};
const uint16 goldwed[] = {
	#include "kims\006.mad"
};
const uint16 congratulation[] = {
	#include "kims\008.mad"
};
const uint16 cuteboy[] = {
	#include "kims\011.mad"
};
const uint16 serial[] = {
	#include "kims\013.mad"
};
const uint16 umbrella[] = {
	#include "kims\034.mad"
};
const uint16 yellowribbon[] = {
	#include "kims\053.mad"
};
const uint16 yoddle[] = {
	#include "kims\059.mad"
};
const uint16 monggumpo[] = {
	#include "kims\068.mad"
};
const uint16 milyang[] = {
	#include "kims\069.mad"
};
const uint16 harvest[] = {
	#include "kims\072.mad"
};
const uint16 guddy[] = {
	#include "kims\075.mad"
};
const uint16 silentnight[] = {
	#include "kims\088.mad"
};
/*
const uint16 jinglebell[] = {
	#include "kims\090.mad"
};
*/
const uint16 joytotheworld[] = {
	#include "kims\093.mad"
};
const uint16 merryxmas[] = {
	#include "kims\095.mad"
};
/*
const uint16 bachminuet[] = {
	#include "kims\104.mad"
};
*/
const uint16 minuet[] = {
	#include "kims\105.mad"
};
/*
const uint16 joylove[] = {
	#include "kims\106.mad"
};
*/
const uint16 magicflute[] = {
	#include "kims\107.mad"
};
const uint16 canon[] = {
	#include "kims\110.mad"
};
const uint16 fantasy[] = {
	#include "kims\115.mad"
};
const uint16 trout[] = {
	#include "kims\116.mad"
};
const uint16 humoresque[] = {
	#include "kims\118.mad"
};
const uint16 dream[] = {
	#include "kims\121.mad"
};
const uint16 flower[] = {
	#include "kims\122.mad"
};
/*
const uint16 bird[] = {
	#include "kims\123.mad"
};
*/
const uint16 toredor[] = {
	#include "kims\127.mad"
};
/*
const uint16 spring[] = {
	#include "kims\132.mad"
};
*/
const uint16 turkey[] = {
	#include "kims\135.mad"
};
/*
const uint16 swanlake[] = {
	#include "kims\137.mad"
};
*/
const uint16 williamtell[] = {
	#include "kims\138.mad"
};
const uint16 toy[] = {
	#include "kims\139.mad"
};
const uint16 funi[] = {
	#include "kims\141.mad"
};
const uint16 hypopipo[] = {
	#include "kims\143.mad"
};
/*
const uint16 waltz[] = {
	#include "kims\chopin~1.mad"
};
const uint16 arabesque[] = {
	#include "kims\debuss~1.mad"
};
*/
const uint16 doremi[] = {
	#include "kims\doremi~1.mad"
};
const uint16 forellise[] = {
	#include "kims\for~el~1.mad"
};
/*
const uint16 chicky[] = {
	#include "kims\mussor~1.mad"
};
*/
const uint16 pathetique1[] = {
	#include "kims\sonata2.mad"
};
const uint16 ladetzky[] = {
	#include "kims\straus~1.mad"
};
/*
const uint16 donau[] = {
	#include "kims\straus~2.mad"
};
*/
const uint16 donau2[] = {
	#include "kims\straus~3.mad"
};
const uint16 soundspring[] = {
	#include "kims\straus~5.mad"
};
/*
const uint16 rose[] = {
	#include "kims\straus~4.mad"
};
const uint16 alhambra[] = {
	#include "kims\alhambra.mad"
};
const uint16 france[] = {
	#include "kims\098.mad"
};
*/




//////////////////////////////////////////////////////////////////////////////
typedef struct {
	const uint16	*data;
	sint15			offset;
} mus_ma1_type;




#if (MMI_TYPE != MMI_A10)
////////////////////////////////////////////////////////////////////////////
const mus_ma1_type MUSIC_ma1_db[] = {
	//Music sound
	{Oncemo,	 0},			/* MUS_ONCEMORE,				         */
#if (MMI_TYPE != MMI_A10)
	{aior1,		-6},			/* MUS_MUSICBOX1,					     */
	{aior2,		-6},			/* MUS_MUSICBOX2,						 */
	{akahan1,	-6},			/* MUS_RUDOLPH1,				         */
#endif //#if (MMI_TYPE != MMI_A10)
	{akahan2,	-6},			/* MUS_RUDOLPH2,				         */
#if (MMI_TYPE != MMI_A10)
	{alone1,	-6},			/* MUS_ALONE1,							 */
	{alone2,	-3},			/* MUS_ALONE2,							 */
	{ame,		+5},			/* MUS_SIDEWALK_NY,			             */
	{anpan,		-5},			/* MUS_ANPAN,						     */
	{aria1,		-4},			/* MUS_ARIA1,						     */
#endif //#if (MMI_TYPE != MMI_A10)
	{aria2,		-2},			/* MUS_ARIA2,							 */
#if (MMI_TYPE != MMI_A10)
	{B_tango,	-5},			/* MUS_CAT_TANGO,						 */
	{White,		-12},			/* MUS_WHITE_XMAS,		                 */
	{Hiirag1,	-7},			/* MUS_DECKHALL1,			             */
#endif //#if (MMI_TYPE != MMI_A10)
	{Hiirag2,	-8},			/* MUS_DECKHALL2,				         */
#if (MMI_TYPE != MMI_A10)
	{Jing1,		-3},			/* MUS_JINGLEBELL1,					     */
#endif //#if (MMI_TYPE != MMI_A10)
	{Jing2,		-3},			/* MUS_JINGLEBELL2,						 */
	{carmen,	-4},			/* MUS_CARMEN,					         */	
	{Hallelus,	-8},			/* MUS_HALLELLUJAH,					     */
#if (MMI_TYPE != MMI_A10)
	{Timegoes,	-3},			/* MUS_TIMEGOES,						 */
	{Tomor,		-5},			/* MUS_TOMORROW,				         */
#endif //#if (MMI_TYPE != MMI_A10)
	{cimu1,		-4},//30		/* MUS_CHIMCHIM1,						 */
#if (MMI_TYPE != MMI_A10)
	{cimu2,		-2},			/* MUS_CHIMCHIM2,						 */
#endif //#if (MMI_TYPE != MMI_A10)
	{Turkish,	-4},			/* MUS_TURKISH,							 */
#if (MMI_TYPE != MMI_A10)
	{Valkyrie,	-2},			/* MUS_VALKYRIE,						 */
	{Dango3,	-4},			/* MUS_DANGO,							 */
	{Hageyama,	-5},			/* MUS_BALDMOUNTAIN,			         */
	{Hoshi,		-2},			/* MUS_WHEN_YOU_WISH_UP_ON_A_STAR,		 */
#endif //#if (MMI_TYPE != MMI_A10)
	{Spring,	-1},//70		/* MUS_SPRING,					         */
#if (MMI_TYPE != MMI_A10)
	{Sinsekai,	-1}, //5		/* MUS_NEW_WORLD,				         */
	{Kekkon1,	-3},			/* MUS_WEDDING1,						 */
	{Kekkon2,	-2},			/* MUS_WEDDING2,						 */
	{Kitayado,	-3},			/* MUS_KITAYADO,				         */
#endif //#if (MMI_TYPE != MMI_A10)
	{Kozo1,		-2},			/* MUS_BABY_ELEPHANT1,					 */
#if (MMI_TYPE != MMI_A10)
	{Kozo2,		-1},			/* MUS_BABY_ELEPHANT2,					 */
	{Lastw1,	-5},			/* MUS_LAST_WALTZ1,					     */
	{Lastw2,	-4}, //60		/* MUS_LAST_WALTZ2,					     */
	{Melody,	-4},			/* MUS_MELODY_FAIR,						 */
#endif //#if (MMI_TYPE != MMI_A10)
	{Menuet,	-3},			/* MUS_MINUET,					         */
#if (MMI_TYPE != MMI_A10)
	{Miche1,	-5},			/* MUS_MICHEL1,							 */
	{Miche2,	-4},			/* MUS_MICHEL2,							 */
	{Movin_o2,	-2},			/* MUS_MOVING_ON,				         */
	{Nagai,		-3},			/* MUS_FOR_A_LONG_TIME			         */
	{Scab,		 0},			/* MUS_SCARBOLO FAIR,			         */
#endif //#if (MMI_TYPE != MMI_A10)

	//Effect Sound
	{Osirase,	+2},			/* MUS_PWR_ON,							 */
	{Kuroden,	-2},			/* MUS_JAPAN_TELEPHONE,					 */
	{Bicycle,	 0},			/* MUS_BICYCLE_BELL,			         */
	{bonodori,	 5},			/* MUS_JAPAN_FESTIVAL,					 */
	{catch1,	-1},			/* MUS_JINGLE1,						     */
	{catch2,	-4},			/* MUS_JINGLE2,						     */
	{catch3,	-2},			/* MUS_JINGLE3,						     */
	{catch4,	-4},			/* MUS_JINGLE4,						     */
	{catch5,	-1},			/* MUS_JINGLE5,						     */
	{catch6,	-1},			/* MUS_JINGLE6,						     */
	{catch7,	-2}, //20		/* MUS_JINGLE7,						     */
	{catch8,	-4},			/* MUS_PWR_OFF,						     */
	{catch9,	-3},			/* MUS_JINGLE9,						     */
	{catch10,	-8},			/* MUS_JINGLE10,						 */
	{catch11,	 0}, //20		/* MUS_JINGLE11,						 */
	{catch12,	-3},			/* MUS_JINGLE12,						 */
	{catch13,	-2},			/* MUS_JINGLE13,						 */
	{catch14,	-4},			/* MUS_JINGLE14,						 */
	{charumer,	-5},			/* MUS_CHINESE_NOODLES,					 */
	{chime,		-1},			/* MUS_W_BELL,							 */
	{Sl,		-4},			/* MUS_TRAIN,							 */
	{Tukutuku,	 5},			/* MUS_CICADAS,					         */
	{Craction,	-2},			/* MUS_CRACTION,						 */
	{Humikiri,	-3},			/* MUS_RAILROAD,				         */
	{Denwa1,	 2},			/* MUS_TELEPHONE1,						 */
	{Denwa2,	 1},			/* MUS_TELEPHONE2,						 */
	{Denwa3,	 3},//40		/* MUS_TELEPHONE3,						 */
	{Denwa4,	 4},			/* MUS_TELEPHONE4,						 */
	{Denwa5,	 0},			/* MUS_TELEPHONE5,						 */
	{Denwa6,	 0},			/* MUS_TELEPHONE6,						 */
	{Denwa7,	 0},//40		/* MUS_TELEPHONE7,						 */
	{Denwa8,	 4},			/* MUS_TELEPHONE8,						 */
	{Denwa9,	-1},			/* MUS_TELEPHONE9,						 */
	{Kakko,		 0},			/* MUS_CUCKOO,							 */
	{Zenmai,	 6},			/* MUS_WIND_CLOCK,						 */
	{Koorogi,	 4},			/* MUS_WIND_CRICKET,			         */
	{Wave,		-1},			/* MUS_WAVE,							 */
	{Amblc,		-1},			/* MUS_AMBULANCE,						 */

	//Kim`s media  제작곡
//	{PeerGyntMorning,	-4},	// 그리그 - 페르귄트 -아침 MUS_PEERGYNT,
	{SongofSpring,		-3},	// 멘델스존 -봄의 노래 MUS_SONGSPRING,
	{ViolinRomance,		-6},	// 베토벤-바이올린 로망스 MUS_VIOLINROMANCE,
	{Pathetique,	   -10},	// 베토벤-비창 소나타 MUS_PATHETIQUE,
//	{fall,				-5},	// MUS_FALL,
//	{warsaw,			-6},	// MUS_WARSAW,
	{aida,				-4},	// MUS_AIDA,
//	{Tchaikov,			-2},	// MUS_PATHETIQUE_SYM,
	{star,				-8},	// MUS_SOUSA1,
	{livertybell,		-5},	// MUS_SOUSA2,
	{washington,		-5},	// MUS_SOUSA3,
	{cello,				-4},	// MUS_1007,
	{igor,				-6},	// MUS_IGOR,
	{pumpmarch,			-5},	// MUS_MARCH,
	{gabriel,			-6},	// MUS_STRANGER,
//	{AmericanParis,		-6},	// MUS_AMEPARIS,
	{giant,				-5},	// MUS_GIANT,
//	{gonewind,			-6},	// MUS_GONEWIND,
	{gumbam,			-8},	// MUS_GUMBAM,
	{trumpetconcert,	-4},	// MUS_TRUMPET,
	{sabredance,		-4},	// MUS_SABREDANCE,
//	{mascagni,			-4},	// MUS_MASCAGNI,
//	{czardas,			-4},	// MUS_CZARDAS,
	{nocturn2,			-5},	// MUS_NOCTURN,
	{cancan,			-3},	// MUS_CANCAN,
//	{pianoc,			-3},	// MUS_PIANOC,
//	{aranpez,			-5},	// MUS_ARANPEZ,
//	{romance,			-5},	// MUS_ROMANCE,
	{cygnus,			-6}, 	// MUS_CYNGNE,
//	{gymnopedie,		-5},	// MUS_GYMNOPEDIES,
//	{finlandia,			-5},	// MUS_SIBELIUS1,
//	{kuolema,			-6},	// MUS_SIBELIUS2,
	{newtown,			-5},	// MUS_NEWTOWN, 
	{goldwed,			-7},	// MUS_GOLDEN,
	{congratulation,	-5},	// MUS_CONGRATULATION,
	{cuteboy,			-7},	// MUS_CUTEBOY,
	{serial,			-8},	// MUS_NARANHI,
	{umbrella,			-6},	// MUS_UMBRELLA,
	{yellowribbon,		-5},	// MUS_YELLOWRIBBON,
	{yoddle,			-6},	// MUS_YODDLE,
	{monggumpo,			-4},	// MUS_MONGGUMPO,
	{milyang,			 0},	// MUS_MILYANG,
	{harvest,			-7},	// MUS_HARVEST,
	{guddy,				-5},	// MUS_GUDDY,
	{silentnight,		-7},	// MUS_SILENTNIGHT,
//	{jinglebell,		-4},	// MUS_JINGLEBELL,
	{joytotheworld,		-9},	// MUS_JOY,
	{merryxmas,			-3},	// MUS_MERRYXMAS,
	{minuet,			-8},	// MUS_MENUET,
//	{joylove,			-5},	// MUS_JOYLOVE,
	{magicflute,		-1},	// MUS_MAGICFLUTE,
	{canon,				-7},	// MUS_CANON,
	{fantasy,			-9},	// MUS_FANTASY,
	{trout,				-8},	// MUS_TROUT,
	{humoresque,		-7},	// MUS_HUMORESQUE,
	{dream,				-5},	// MUS_DREAM,
	{flower,			-4},	// MUS_FLOWER,
//	{bird,				-2},	// MUS_BIRD,
	{toredor,			-7},	// MUS_TOREDOR,
//	{spring,			-4},	// MUS_SPRING2,
	{turkey,			-5},	// MUS_TURKEY,
//	{swanlake,			-5},	// MUS_SWANLAKE,
	{williamtell,		-5},    // MUS_WILLIAMTELL,
	{toy,				-9},	// MUS_TOY,
	{funi,				-9},	// MUS_FUNIKULA,
	{hypopipo,			-9},	// MUS_HYPOPIPO,
//	{waltz,				-5},	// MUS_WALTZ,
//	{arabesque,			-6},	// MUS_ARABESQUE,
	{doremi,			-7},	// MUS_DOREMI,
	{forellise,			-7},	// MUS_FORELISE,
//	{chicky,			-5},	// MUS_CHICKY,
	{pathetique1,		-6},	// MUS_PATHETIQUE_SONATA,
	{ladetzky,			-5},	// MUS_LADETZKY,
//	{donau,				-6},	// MUS_DONAU1,
	{donau2,			-4},	// MUS_DONAU2,
	{soundspring,		-3},	// MUS_SOUNDSPRING,
    {violin,            -2},    // MUS_VIOLINC
    {wed,               -3},    // MUS_WEDD

//	{rose,				-4},	// MUS_ROSE,
//	{alhambra,			-5},	// MUS_ALHAMBRA,
//	{france,			-5},	// MUS_FRANCE2,
//	{bachminuet,		-5},	// MUS_BACHMENUET,

	// mbz for game music & sound
	{ddg_theme,			-1},
	{ddg_born,			 1},
	{ddg_shot,			-1},
	{mpg_theme,			-1},
	{mpg_car_blaster,	-1},
	{mpg_ufo_blaster,	-1},
	{mpg_ufo_shot,		-1},
	{mpg_stone_blaster,	-1},
	{mpg_jump,			-1},
	{mpg_shot,			-1},
	{sam_theme,			-1},
	{pzg_theme,			-1},
	{sms_bell1,			-1},
	{sms_bell2,			-1},
	{sms_bell3,			-2},
	{sms_bell4,			-1},
	{sms_bell5,			-1},
	{sms_bell6,			-1},
	{sms_bell7,			-1},
	{low_vbatt,			-1}
}; 
#else // (MMI_TYPE != MMI_A10)
////////////////////////////////////////////////////////////////////////////
// chojw for A10 : to sync with DataManager
const mus_ma1_type MUSIC_ma1_db[] = {
	//Music sound
	{Oncemo,	 0},			/* MUS_ONCEMORE,				         */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,		-6},			/* MUS_MUSICBOX1,					     */
	{Oncemo,		-6},			/* MUS_MUSICBOX2,						 */
	{Oncemo,	-6},			/* MUS_RUDOLPH1,				         */
//#endif //#if (MMI_TYPE != MMI_A10)
	{akahan2,	-6},			/* MUS_RUDOLPH2,				         */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,	-6},			/* MUS_ALONE1,							 */
	{Oncemo,	-3},			/* MUS_ALONE2,							 */
	{Oncemo,		+5},			/* MUS_SIDEWALK_NY,			             */
	{Oncemo,		-5},			/* MUS_ANPAN,						     */
	{Oncemo,		-4},			/* MUS_ARIA1,						     */
//#endif //#if (MMI_TYPE != MMI_A10)
	{aria2,		-2},			/* MUS_ARIA2,							 */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,	-5},			/* MUS_CAT_TANGO,						 */
	{Oncemo,		-12},			/* MUS_WHITE_XMAS,		                 */
	{Oncemo,	-7},			/* MUS_DECKHALL1,			             */
//#endif //#if (MMI_TYPE != MMI_A10)
	{Hiirag2,	-8},			/* MUS_DECKHALL2,				         */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,		-3},			/* MUS_JINGLEBELL1,					     */
//#endif //#if (MMI_TYPE != MMI_A10)
	{Jing2,		-3},			/* MUS_JINGLEBELL2,						 */
	{carmen,	-4},			/* MUS_CARMEN,					         */	
	{Hallelus,	-8},			/* MUS_HALLELLUJAH,					     */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,	-3},			/* MUS_TIMEGOES,						 */
	{Oncemo,		-5},			/* MUS_TOMORROW,				         */
//#endif //#if (MMI_TYPE != MMI_A10)
	{cimu1,		-4},//30		/* MUS_CHIMCHIM1,						 */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,		-2},			/* MUS_CHIMCHIM2,						 */
//#endif //#if (MMI_TYPE != MMI_A10)
	{Turkish,	-4},			/* MUS_TURKISH,							 */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,	-2},			/* MUS_VALKYRIE,						 */
	{Oncemo,	-4},			/* MUS_DANGO,							 */
	{Oncemo,	-5},			/* MUS_BALDMOUNTAIN,			         */
	{Oncemo,		-2},			/* MUS_WHEN_YOU_WISH_UP_ON_A_STAR,		 */
//#endif //#if (MMI_TYPE != MMI_A10)
	{Spring,	-1},//70		/* MUS_SPRING,					         */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,	-1}, //5		/* MUS_NEW_WORLD,				         */
	{Oncemo,	-3},			/* MUS_WEDDING1,						 */
	{Oncemo,	-2},			/* MUS_WEDDING2,						 */
	{Oncemo,	-3},			/* MUS_KITAYADO,				         */
//#endif //#if (MMI_TYPE != MMI_A10)
	{Kozo1,		-2},			/* MUS_BABY_ELEPHANT1,					 */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,		-1},			/* MUS_BABY_ELEPHANT2,					 */
	{Oncemo,	-5},			/* MUS_LAST_WALTZ1,					     */
	{Oncemo,	-4}, //60		/* MUS_LAST_WALTZ2,					     */
	{Oncemo,	-4},			/* MUS_MELODY_FAIR,						 */
//#endif //#if (MMI_TYPE != MMI_A10)
	{Menuet,	-3},			/* MUS_MINUET,					         */
//#if (MMI_TYPE != MMI_A10)
	{Oncemo,	-5},			/* MUS_MICHEL1,							 */
	{Oncemo,	-4},			/* MUS_MICHEL2,							 */
	{Oncemo,	-2},			/* MUS_MOVING_ON,				         */
	{Oncemo,		-3},			/* MUS_FOR_A_LONG_TIME			         */
	{Oncemo,		 0},			/* MUS_SCARBOLO FAIR,			         */
//#endif //#if (MMI_TYPE != MMI_A10)

	//Effect Sound
	{Osirase,	+2},			/* MUS_PWR_ON,							 */
	{Kuroden,	-2},			/* MUS_JAPAN_TELEPHONE,					 */
	{Bicycle,	 0},			/* MUS_BICYCLE_BELL,			         */
	{bonodori,	 5},			/* MUS_JAPAN_FESTIVAL,					 */
	{catch1,	-1},			/* MUS_JINGLE1,						     */
	{catch2,	-4},			/* MUS_JINGLE2,						     */
	{catch3,	-2},			/* MUS_JINGLE3,						     */
	{catch4,	-4},			/* MUS_JINGLE4,						     */
	{catch5,	-1},			/* MUS_JINGLE5,						     */
	{catch6,	-1},			/* MUS_JINGLE6,						     */
	{catch7,	-2}, //20		/* MUS_JINGLE7,						     */
	{catch8,	-4},			/* MUS_PWR_OFF,						     */
	{catch9,	-3},			/* MUS_JINGLE9,						     */
	{catch10,	-8},			/* MUS_JINGLE10,						 */
	{catch11,	 0}, //20		/* MUS_JINGLE11,						 */
	{catch12,	-3},			/* MUS_JINGLE12,						 */
	{catch13,	-2},			/* MUS_JINGLE13,						 */
	{catch14,	-4},			/* MUS_JINGLE14,						 */
	{charumer,	-5},			/* MUS_CHINESE_NOODLES,					 */
	{chime,		-1},			/* MUS_W_BELL,							 */
	{Sl,		-4},			/* MUS_TRAIN,							 */
	{Tukutuku,	 5},			/* MUS_CICADAS,					         */
	{Craction,	-2},			/* MUS_CRACTION,						 */
	{Humikiri,	-3},			/* MUS_RAILROAD,				         */
	{Denwa1,	 2},			/* MUS_TELEPHONE1,						 */
	{Denwa2,	 1},			/* MUS_TELEPHONE2,						 */
	{Denwa3,	 3},//40		/* MUS_TELEPHONE3,						 */
	{Denwa4,	 4},			/* MUS_TELEPHONE4,						 */
	{Denwa5,	 0},			/* MUS_TELEPHONE5,						 */
	{Denwa6,	 0},			/* MUS_TELEPHONE6,						 */
	{Denwa7,	 0},//40		/* MUS_TELEPHONE7,						 */
	{Denwa8,	 4},			/* MUS_TELEPHONE8,						 */
	{Denwa9,	-1},			/* MUS_TELEPHONE9,						 */
	{Kakko,		 0},			/* MUS_CUCKOO,							 */
	{Zenmai,	 6},			/* MUS_WIND_CLOCK,						 */
	{Koorogi,	 4},			/* MUS_WIND_CRICKET,			         */
	{Wave,		-1},			/* MUS_WAVE,							 */
	{Amblc,		-1},			/* MUS_AMBULANCE,						 */

	/* Old Melody Type  */
	{Oncemo,	-4},	        //  MUS_BASIC,
	{Oncemo,	-4},	        //  MUS_SEA_SYMPHONY ,
	{Oncemo,	-4},	        //  MUS_CANNON,
	{Oncemo,	-4},	        //  MUS_FRANCE_SUITE6,
	{Oncemo,	-4},	        //  MUS_MAGIC,
	{Oncemo,	-4},	        //  MUS_MUSETT,
	{Oncemo,	-4},	        //  MUS_MINUET_BACH,
	{Oncemo,	-4},	        //  MUS_BACAROL,
	{Oncemo,	-4},	        //  MUS_JOY_LOVE,
	{Oncemo,	-4},	        //  MUS_LOSSIGNOL,
	{Oncemo,	-4},	        //  MUS_FRANCE_SUITE2,
	{Oncemo,	-4},	        //  MUS_FRANCE_SUITE1,
	{Oncemo,	-4},	        //  MUS_FRANCE_SUITE3,
	{Oncemo,	-4},	        //  MUS_FRANCE_SUITE4,
	{Oncemo,	-4},	        //  MUS_FRANCE_SUITE5,
	{Oncemo,	-4},	        //  MUS_MINUET_MOZART,
	{Oncemo,	-4},	        //  MUS_AVE_VERUM,

	//Kim`s media  제작곡
	{Oncemo,	-4},	// 그리그 - 페르귄트 -아침 MUS_PEERGYNT,
	{SongofSpring,		-3},	// 멘델스존 -봄의 노래 MUS_SONGSPRING,
	{Oncemo,	-4},	        //  MUS_TURANDOT,
	{ViolinRomance,		-6},	// 베토벤-바이올린 로망스 MUS_VIOLINROMANCE,
	{Pathetique,	   -10},	// 베토벤-비창 소나타 MUS_PATHETIQUE,
	{Oncemo,				-5},	// MUS_FALL,
	{Oncemo,			-6},	// MUS_WARSAW,
	{aida,				-4},	// MUS_AIDA,
	{Oncemo,			-2},	// MUS_PATHETIQUE_SYM,
	{star,				-8},	// MUS_SOUSA1,
	{livertybell,		-5},	// MUS_SOUSA2,
	{washington,		-5},	// MUS_SOUSA3,
	{cello,				-4},	// MUS_1007,
	{igor,				-6},	// MUS_IGOR,
	{pumpmarch,			-5},	// MUS_MARCH,
	{gabriel,			-6},	// MUS_STRANGER,
	{Oncemo,		-6},	// MUS_AMEPARIS,
	{giant,				-5},	// MUS_GIANT,
	{Oncemo,			-6},	// MUS_GONEWIND,
	{gumbam,			-8},	// MUS_GUMBAM,
	{trumpetconcert,	-4},	// MUS_TRUMPET,
	{sabredance,		-4},	// MUS_SABREDANCE,
	{Oncemo,			-4},	// MUS_MASCAGNI,
    {wed,               -3},    // MUS_WEDD
    {violin,            -2},    // MUS_VIOLINC
	{Oncemo,			-4},	// MUS_CZARDAS,
	{nocturn2,			-5},	// MUS_NOCTURN,
	{cancan,			-3},	// MUS_CANCAN,
	{Oncemo,			-3},	// MUS_PIANOC,
	{Oncemo,			-5},	// MUS_ARANPEZ,
	{Oncemo,			-5},	// MUS_ROMANCE,
	{cygnus,			-6}, 	// MUS_CYNGNE,
	{Oncemo,		-5},	// MUS_GYMNOPEDIES,
	{Oncemo,			-5},	// MUS_SIBELIUS1,
	{Oncemo,			-6},	// MUS_SIBELIUS2,
	{newtown,			-5},	// MUS_NEWTOWN, 
	{goldwed,			-7},	// MUS_GOLDEN,
	{congratulation,	-5},	// MUS_CONGRATULATION,
	{cuteboy,			-7},	// MUS_CUTEBOY,
	{serial,			-8},	// MUS_NARANHI,
	{umbrella,			-6},	// MUS_UMBRELLA,
	{yellowribbon,		-5},	// MUS_YELLOWRIBBON,
	{yoddle,			-6},	// MUS_YODDLE,
	{monggumpo,			-4},	// MUS_MONGGUMPO,
	{milyang,			 0},	// MUS_MILYANG,
	{harvest,			-7},	// MUS_HARVEST,
	{guddy,				-5},	// MUS_GUDDY,
	{silentnight,		-7},	// MUS_SILENTNIGHT,
	{Oncemo,		-4},	// MUS_JINGLEBELL,
	{joytotheworld,		-9},	// MUS_JOY,
	{merryxmas,			-3},	// MUS_MERRYXMAS,
	{minuet,			-8},	// MUS_MENUET,
	{Oncemo,			-5},	// MUS_JOYLOVE,
	{magicflute,		-1},	// MUS_MAGICFLUTE,
	{canon,				-7},	// MUS_CANON,
	{fantasy,			-9},	// MUS_FANTASY,
	{trout,				-8},	// MUS_TROUT,
	{humoresque,		-7},	// MUS_HUMORESQUE,
	{dream,				-5},	// MUS_DREAM,
	{flower,			-4},	// MUS_FLOWER,
	{Oncemo,				-2},	// MUS_BIRD,
	{toredor,			-7},	// MUS_TOREDOR,
	{Oncemo,			-4},	// MUS_SPRING2,
	{turkey,			-5},	// MUS_TURKEY,
	{Oncemo,			-5},	// MUS_SWANLAKE,
	{williamtell,		-5},    // MUS_WILLIAMTELL,
	{toy,				-9},	// MUS_TOY,
	{funi,				-9},	// MUS_FUNIKULA,
	{hypopipo,			-9},	// MUS_HYPOPIPO,
	{Oncemo,				-5},	// MUS_WALTZ,
	{Oncemo,			-6},	// MUS_ARABESQUE,
	{doremi,			-7},	// MUS_DOREMI,
	{forellise,			-7},	// MUS_FORELISE,
	{Oncemo,			-5},	// MUS_CHICKY,
	{pathetique1,		-6},	// MUS_PATHETIQUE_SONATA,
	{ladetzky,			-5},	// MUS_LADETZKY,
	{Oncemo,				-6},	// MUS_DONAU1,
	{donau2,			-4},	// MUS_DONAU2,
	{soundspring,		-3},	// MUS_SOUNDSPRING,

	{Oncemo,				-4},	// MUS_ROSE,
	{Oncemo,			-5},	// MUS_ALHAMBRA,
	{Oncemo,			-5},	// MUS_FRANCE2,
	{Oncemo,		-5},	// MUS_BACHMENUET,

	// mbz for game music & sound
	{ddg_theme,			-1},
	{ddg_born,			 1},
	{ddg_shot,			-1},
	{mpg_theme,			-1},
	{mpg_car_blaster,	-1},
	{mpg_ufo_blaster,	-1},
	{mpg_ufo_shot,		-1},
	{mpg_stone_blaster,	-1},
	{mpg_jump,			-1},
	{mpg_shot,			-1},
	{sam_theme,			-1},
	{pzg_theme,			-1},
	{sms_bell1,			-1},
	{sms_bell2,			-1},
	{sms_bell3,			-2},
	{sms_bell4,			-1},
	{sms_bell5,			-1},
	{sms_bell6,			-1},
	{sms_bell7,			-1},
	{low_vbatt,			-1}
}; 
#endif // (MMI_TYPE != MMI_A10)


////////////////////////////////////////////////////////////////////////////
uint16 *MUSIC_get_music_data(mus_id_type music_id)
{
  if((music_id >= MUS_FIRST_SND) && (music_id <= MUS_LAST_SND))
    return (uint16 *)MUSIC_ma1_db[music_id - MUS_FIRST_SND].data;

  return NULL;
} // MUSIC_get_music_data()



////////////////////////////////////////////////////////////////////////////
sint15 MUSIC_get_music_gain_offset(mus_id_type music_id)
{
  if((music_id >= MUS_FIRST_SND) && (music_id <= MUS_LAST_SND))
    return MUSIC_ma1_db[music_id - MUS_FIRST_SND].offset;

  return 0;
} // MUSIC_get_music_gain_offset()
#endif // #ifdef FEATURE_YMU757B_MELODY // jwpark 00.07.25 end
