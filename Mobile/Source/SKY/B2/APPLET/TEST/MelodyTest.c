/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     
**
** Description: 
**     <@Desciption about this file here>
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00.07.25   jwpark     MelodyList(), MelodyOption()
** 00-07-21   woodstok   Created.
**
** ===========================================================================
*/

/* dddddd
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"

#if ( MODEL_ID == MODEL_B1 )

#include "biogB1.h"
#include "hsig.h"
#include "MusicDB_B1.h"

#elif(MODEL_ID == MODEL_B2)
#include "biogB2.h"
#include "MusicDB_B1.h"
#endif

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

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

#if ( MODEL_ID == MODEL_B1 ) || ( MODEL_ID == MODEL_B2 )

LOCAL ROM CHAR* ROM LIGHTSENSOR[] ={
	"ON",	"OFF"
};
LOCAL ROM CHAR* ROM LIGHTONOFF[] ={
	"ON",	"OFF"
};
LOCAL ROM CHAR* ROM EQVolmueList[] = {
	"-13db",	"-14db",	"-15db",	"-16db",	"-17db",	"-18db",	"-19db",	"-20db",	"0db",	"-1db",	"-2db",
	"-3db",	"-4db",	"-5db",	"-6db",	"-7db",	"-8db",	"-9db",	"-10db",	"-11db",	"-12db"
};
LOCAL ROM CHAR* ROM IDLCDContrastLevel[] = {
	"45",	"42",	"39",	"36",	"33",	"30",	"27",	"24",	"21",	"18",	"15",	"12",	"09",	"06",
	"03",	"63",	"60",	"57",	"54",	"51",	"48",
};
/*
LOCAL ROM CHAR* ROM MICVolumrList[] = {
	"0x70",	"0x60",	"0x50",	"0x40",	"0x30",	"0x20",	"0x10",	"0x00"
};
*/
LOCAL ROM CHAR* ROM VolmueList[] = {
	"0x1F",	"0x1c",	"0x19",	"0x16",	"0x13",	"----",	"----",	"R ON",	"G ON",	"B ON",	"light ON",	"light OFF",
	"MOTOR ON",	"MOTOR OFF",	"Stop"
};
/*
LOCAL ROM CHAR* ROM MelodyList[] = {
	"pkmn_ad5",	"네박자",	"날 닮은 너",	"Winter-Largo",	"Allerg",	"Autumn",	"Summer",	"atra_k022",	"boun8_k13"
};
*/                                
CONST CHAR* CONST EFSMelodyList[] = {SF_BLACK_NUM1"OFF",
                         SF_BLACK_NUM2"ON"
};

CONST CHAR* CONST MelodyOption[] = {SF_BLACK_NUM1"OFF",
					    SF_BLACK_NUM2"ON"
};

extern CONST BELL_NAME_TYPE_T aBell_List[NUM_BELL_LIST];

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

//static UINT8 volume = 0x1f;
static uint16 eqvolume;
LOCAL BYTE *List[NUM_BELL_LIST];
extern void Ma2w_sp_vol(UINT8 vol);
extern void Ma2w_motor_control(UINT8 ctrl,UINT8 syncch);
extern void Ma2w_led_control(UINT8 ctrl,UINT8 syncch);
extern void	Ma2w_eq_vol(UINT8 vol);
//extern void smb_init_850mA(void);  
//extern void SKY_SetSubLCDContrast(word contrast);
//extern void hs_lcd_contrast(word idlcdcontrast);
//extern void msm7732_set_tx_gain_modifing(byte plevel);
//extern void	ReInitialidLCD (byte IRR );
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

void SKY_API SKA_MainMICVolumeTest(SKY_EVENT_T *pInEvent)
{
	STATIC UINT8 nIndex;
	int i;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
		for(i =0; i<NUM_BELL_LIST; i++)
		{
			List[i] = (BYTE*)SKY_GET_TOKEN(aBell_List[i].BellName);
		}
        EQC_ScrollList(0, 0, 16, 6, List, NUM_BELL_LIST, NUM_BELL_LIST, 0);
        EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;

	case SE_APP_END:
		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:

		break;

    case SE_IO_SCROLL_LIST_SELECT:
		nIndex = (pInEvent->dwParam);
		SKY_ReInitSubLCD((byte)(7-nIndex));//ReInitialidLCD((byte)(7-nIndex));
//		msm7732_set_tx_gain_modifing((byte)(7-nIndex));
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}

void SKY_API SKA_MainTestMelodyList(SKY_EVENT_T* pInEvent)
{
	STATIC UINT8 nIndex;
	int i;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
 		for(i =0; i<NUM_BELL_LIST; i++)
		{
			List[i] = (BYTE*)SKY_GET_TOKEN(aBell_List[i].BellName);
		}
        EQC_ScrollList(0, 0, 16, 6, List, NUM_BELL_LIST, NUM_BELL_LIST, 0);
        EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;

	case SE_APP_END:
		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:

		break;

    case SE_TM_TIMER1:
		SKY_PlayMusicEx(aBell_List[nIndex].wBellId,5,0);
		
		break;
    case SE_IO_SCROLL_LIST_SELECT:

		EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
		SKY_StopMusic();
		nIndex = (pInEvent->dwParam);
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}
void SKY_API SKA_MainMelodyVolumeTest(SKY_EVENT_T* pInEvent)
{
	int vi;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQC_ScrollList(0, 0, 16, 6, (BYTE **)VolmueList, 15, 15, 0);
        EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;

	case SE_APP_END:
		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:
		break;

    case SE_IO_SCROLL_LIST_SELECT:
		vi=(pInEvent->dwParam);
		SKY_StopMusic();
		SKY_StopMusic();
		SKY_StopMusic();
		SKY_StopMusic();
		SKY_StopMusic();
		SKY_StopMusic();
		SKY_StopMusic();
		SKY_StopMusic();
		if(vi<5)
		{
		SKY_PlayMusicEx(0,5-vi,0);
		}
#if SKY_MODEL_ID == SKY_MODEL_B1
#ifndef WIN32
		switch (vi)
		{
            case 7:
				BIO_OUT( ENC_GPIO_1, BIO_LED_RED_M,   BIO_LED_RED_HI_V);
				break;
			case 8:
				BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M,  BIO_LED_GREEN_HI_V);
				break;
			case 9:
				BIO_OUT( ENC_GPIO_1, BIO_LED_BLUE_M,  BIO_LED_BLUE_HI_V);
				break;
			case 10:
				BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_HI_V); 
				BIO_OUT( ENC_GPIO_1, BIO_LED_BLUE_M,  BIO_LED_BLUE_HI_V); \
				BIO_OUT( ENC_GPIO_1, BIO_LED_RED_M,   BIO_LED_RED_HI_V); \
				BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_HI_V); \
				Ma2w_led_control(0x80,0x00);
				Ma2w_led_control(0x80,0x00);
				break;
			case 11:
				BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_LO_V); 
				BIO_OUT( ENC_GPIO_1, BIO_LED_BLUE_M,  BIO_LED_BLUE_LO_V); \
				BIO_OUT( ENC_GPIO_1, BIO_LED_RED_M,   BIO_LED_RED_LO_V); \
				BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V); \
				Ma2w_led_control(0x00,0x00);
				Ma2w_led_control(0x00,0x00);
				break;
			case 12:
				Ma2w_motor_control(0x80,0x00);
				break;
			case 13:
				Ma2w_motor_control(0x00,0x00);		
				break;
			case 14:
				SKY_StopMusic();
				break;
			default :	
				break;
		}
#endif /* ifndef WIN32 */
#endif //SKY_MODEL_ID == SKY_MODEL_B1
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;
	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}
void SKY_API SKA_MainBackLightTest(SKY_EVENT_T* pInEvent)
{
	int vi;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQC_ScrollList(0, 0, 16, 6, (BYTE **)LIGHTONOFF, 2, 2, 0);
        EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;

	case SE_APP_END:

		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:

		break;

    case SE_IO_SCROLL_LIST_SELECT:
		vi=(int)(pInEvent->dwParam);

		switch (vi)
		{
			case 0:
				BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_HI_V); 
				Ma2w_led_control(0x80,0x00);
				Ma2w_led_control(0x80,0x00);
				break;
			case 1:
				BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_LO_V); 
				Ma2w_led_control(0x00,0x00);
				Ma2w_led_control(0x00,0x00);
				break;

			default :	
				break;
		}
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}
void SKY_API SKA_MainLightSensorTest(SKY_EVENT_T* pInEvent)
{
	int vi,i, chk=0,chk2=0;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQC_ScrollList(0, 0, 16, 6, (BYTE **)LIGHTSENSOR, 2, 2, 0);
		EQS_StartTimer(EQS_TIMER_1,100,TRUE);
        EQS_SetIOCActive(EQS_GetNthIOC(0));
		break;
	case SE_APP_END:
		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:

		break;

    case SE_IO_SCROLL_LIST_SELECT:
		vi=(int)(pInEvent->dwParam);
		chk=0;
		switch (vi)
		{
			case 0:
#if MODEL_ID != MODEL_B2
				for(i=0; i<100; ++i){
					if(  BIO_INM( DEC_GPIO_0, BIO_LIGHT_SEN_M ) == BIO_LIGHT_SEN_LO_V )
						++chk;
				}
				if(chk != 0)
#endif
					Ma2w_led_control(0x80,0x00);

				break;
			case 1:
				Ma2w_led_control(0x00,0x00);
				break;
			default :	
				break;
		}
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

	case SE_TM_TIMER1:
		chk=0;
		chk2=0;
#if MODEL_ID != MODEL_B2
		for(i=0; i<10; ++i){
			if(  BIO_INM( DEC_GPIO_0, BIO_LIGHT_SEN_M ) == BIO_LIGHT_SEN_LO_V )
				++chk;
			if(  BIO_INM( DEC_GPIO_0, BIO_LIGHT_SEN_M ) == BIO_LIGHT_SEN_HI_V )
				++chk2;
		}
#endif
		if(chk != 0 && chk2 == 0)
		 	Ma2w_led_control(0x80,0x00);
		else
		 	Ma2w_led_control(0x00,0x00);

		break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}
void SKY_API SKA_MainLightSensorTest2(SKY_EVENT_T* pInEvent)
{
	int vi,i, chk=0,chk2=0;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQC_ScrollList(0, 0, 16, 6, (BYTE **)LIGHTSENSOR, 2, 2, 0);
		EQS_StartTimer(EQS_TIMER_1,100,TRUE);
        EQS_SetIOCActive(EQS_GetNthIOC(0));

		break;

	case SE_APP_END:

		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:

		break;

    case SE_IO_SCROLL_LIST_SELECT:
		vi=(int)(pInEvent->dwParam);
		chk=0;
		switch (vi)
		{
			case 0:
#if MODEL_ID != MODEL_B2
				for(i=0; i<100; ++i){
					if(  BIO_INM( DEC_GPIO_0, BIO_LIGHT_SEN_M ) == BIO_LIGHT_SEN_LO_V )
						++chk;
				}
#endif
				if(chk != 0)
					Ma2w_led_control(0x80,0x00);

				break;
			case 1:
				Ma2w_led_control(0x00,0x00);
				break;
			default :	
				break;
		}
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

	case SE_TM_TIMER1:
		chk=0;
		chk2=0;
#if MODEL_ID != MODEL_B2
		for(i=0; i<20; ++i){
			if(  BIO_INM( DEC_GPIO_0, BIO_LIGHT_SEN_M ) == BIO_LIGHT_SEN_LO_V )
				++chk;
			else
				++chk2;
		}
#endif
		if(chk == 20 && chk2 == 0 )
		 	Ma2w_led_control(0x80,0x00);
		else
		 	Ma2w_led_control(0x00,0x00);

		break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}
void SKY_API SKA_MainIDLCDContrastTest(SKY_EVENT_T* pInEvent)
{
	int vi;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQC_ScrollList(0, 0, 16, 6, (BYTE **)IDLCDContrastLevel, 21, 21, 0);
        EQS_SetIOCActive(EQS_GetNthIOC(0));

		break;

	case SE_APP_END:

		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:

		break;

    case SE_IO_SCROLL_LIST_SELECT:
		vi=(int)(pInEvent->dwParam);

		switch (vi)
		{
			case 0:	SKY_SetSubLCDContrast(45, 0 );      //white 2001.05.10 For Compile.
				break;
			case 1:	SKY_SetSubLCDContrast(42, 0 );
				break;
			case 2:	SKY_SetSubLCDContrast(39, 0 );
				break;
			case 3:	SKY_SetSubLCDContrast(36, 0 );
				break;
			case 4:	SKY_SetSubLCDContrast(33, 0 );
				break;
			case 5: SKY_SetSubLCDContrast(30, 0 );
				break;
			case 6:	SKY_SetSubLCDContrast(27, 0 );
				break;
			case 7:	SKY_SetSubLCDContrast(24, 0 );
				break;

			case 8:	SKY_SetSubLCDContrast(21, 0 );
				break;
			case 9:	SKY_SetSubLCDContrast(18, 0 );
				break;
			case 10: SKY_SetSubLCDContrast(15, 0 );
				break;
			case 11: SKY_SetSubLCDContrast(12, 0 );
				break;
			case 12: SKY_SetSubLCDContrast(9, 0 );
				break;
			case 13: SKY_SetSubLCDContrast(6, 0 );
				break;
			case 14: SKY_SetSubLCDContrast(3, 0 );
				break;
			case 15: SKY_SetSubLCDContrast(63, 0 );
				break;

			case 16: SKY_SetSubLCDContrast(60, 0 );
				break;
			case 17: SKY_SetSubLCDContrast(57, 0 );
				break;
			case 18: SKY_SetSubLCDContrast(54, 0 );
				break;
			case 19: SKY_SetSubLCDContrast(51, 0 );
				break;
			case 20: SKY_SetSubLCDContrast(48, 0 );
				break;
		}
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}
void SKY_API SKA_MainColorLCDContrastTest(SKY_EVENT_T* pInEvent)
{
	int vi;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQC_ScrollList(0, 0, 16, 6, (BYTE **)IDLCDContrastLevel, 21, 21, 0);
        EQS_SetIOCActive(EQS_GetNthIOC(0));

		break;

	case SE_APP_END:

		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:

		break;

    case SE_IO_SCROLL_LIST_SELECT:
		vi=(int)(pInEvent->dwParam);

		switch (vi)
		{
			case 0:
				break;
			case 1:
				break;
			default :	
				break;
		}
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}

void SKY_API SKA_MainEQVolumeTest(SKY_EVENT_T* pInEvent)
{
	int vi;
    switch(pInEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQC_ScrollList(0, 0, 16, 6, (BYTE **)EQVolmueList, 21, 21, 0);
        EQS_SetIOCActive(EQS_GetNthIOC(0));

		break;

	case SE_APP_END:

		EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_CHG:

		break;

    case SE_IO_SCROLL_LIST_SELECT:
		vi=(int)(pInEvent->dwParam);

		switch (vi)
		{
			case 0:	
				eqvolume=0x12;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x12;	Ma2w_eq_vol(eqvolume);
				break;
			case 1:	
				eqvolume=0x11;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x11;	Ma2w_eq_vol(eqvolume);
				break;
			case 2:	
				eqvolume=0x10;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x10;	Ma2w_eq_vol(eqvolume);
				break;
			case 3:	
				eqvolume=0x0f;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x0f;	Ma2w_eq_vol(eqvolume);
				break;
			case 4:	
				eqvolume=0x0e;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x0e;	Ma2w_eq_vol(eqvolume);
				break;
			case 5: 
				eqvolume=0x0d;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x0d;	Ma2w_eq_vol(eqvolume);
				break;
			case 6:	
				eqvolume=0x0c;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x0c;	Ma2w_eq_vol(eqvolume);
				break;
			case 7:	
				eqvolume=0x0b;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x0b;	Ma2w_eq_vol(eqvolume);
				break;
			case 8:	
				eqvolume=0x1f;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x1f;	Ma2w_eq_vol(eqvolume);
				break;
			case 9:	
				eqvolume=0x1e;	Ma2w_eq_vol(eqvolume);
				eqvolume=0x1e;	Ma2w_eq_vol(eqvolume);
				break;
			case 10: 
				eqvolume=0x1d;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x1d;			Ma2w_eq_vol(eqvolume);
				break;
			case 11: 
				eqvolume=0x1c;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x1c;			Ma2w_eq_vol(eqvolume);
				break;
			case 12: 
				eqvolume=0x1b;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x1b;			Ma2w_eq_vol(eqvolume);
				break;
			case 13: 
				eqvolume=0x1a;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x1a;			Ma2w_eq_vol(eqvolume);
				break;
			case 14: 
				eqvolume=0x19;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x19;			Ma2w_eq_vol(eqvolume);
				break;
			case 15: 
				eqvolume=0x18;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x18;			Ma2w_eq_vol(eqvolume);
				break;

			case 16: 
				eqvolume=0x17;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x17;			Ma2w_eq_vol(eqvolume);
				break;
			case 17: 
				eqvolume=0x16;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x16;			Ma2w_eq_vol(eqvolume);
				break;
			case 18: 
				eqvolume=0x15;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x15;			Ma2w_eq_vol(eqvolume);
				break;
			case 19: 
				eqvolume=0x14;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x14;			Ma2w_eq_vol(eqvolume);
				break;
			case 20: 
				eqvolume=0x13;			Ma2w_eq_vol(eqvolume);
				eqvolume=0x13;			Ma2w_eq_vol(eqvolume);
				break;

			default :	
				break;
		}
		EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pInEvent);
		break;
	}
}
#endif //#if defined (MODEL_ID == MODEL_B1) // youandi 00.12.21

#if( MODEL_ID == MODEL_B0 )  
void SKY_API SKA_MainTestMelodyList(SKY_EVENT_T* pInEvent)
{

}
void SKY_API SKA_MainMelodyVolumeTest(SKY_EVENT_T* pInEvent)
{

}
void SKY_API SKA_MainBackLightTest(SKY_EVENT_T* pInEvent)
{

}
void SKY_API SKA_MainLightSensorTest(SKY_EVENT_T* pInEvent)
{

}

#endif
/*
void SKY_API SKA_MainLcdContrastTest(SKY_EVENT_T* pInEvent)
{
	static HCONTROL hHnd;
    void EQS_API (*pfnDraw[11])(void) = {
        sDraw256ImageRoses,
        sDraw256ImageRock,
        sDraw4GImage,
        sDraw256ImageT,
        sDraw256ImageT1,
        sDraw256ImageT2,
		redred,
		greengreen,
		blueblue,
		whitewhite,
		c4color

    };
    STATIC INT8 i = 0;
	UINT8	iKey;
	int idcont[10] = {0,7,14,21,28,35,42,49,56,63};
	

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
		csel = 8;
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0xff);			// id lcd select
		SKY_SetSubLCDContrast(idcont[csel]);
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0);
        EQS_GetDC(TRUE);
		//hHnd = EQC_StaticText("LCD TEST", 0, 0, FT_MED, 0);
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_UP:
	case EQE_KEY_1:


		hs_lcd_contrast (0x21);

		if(csel==9)
			;
		else
			csel++;
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0xff);			// id lcd select
		SKY_SetSubLCDContrast(idcont[csel]);
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0);
		break;

	case EQE_KEY_DOWN:
	case EQE_KEY_2:


		hs_lcd_contrast (1);

		if(csel==0)
			;
		else
			csel--;
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0xff);			// id lcd select
		SKY_SetSubLCDContrast(idcont[csel]);
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0);
		break;

	case EQE_KEY_4:
		HS_BACKLIGHT_ON();
		break;
	case EQE_KEY_5:
		HS_BACKLIGHT_OFF();
		break;

	case EQE_KEY_6:
		BIO_OUT( ENC_GPIO_1, BIO_LED_BLUE_M,  BIO_LED_BLUE_LO_V); 
		BIO_OUT( ENC_GPIO_1, BIO_LED_RED_M,   BIO_LED_RED_LO_V); 
		BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V); 
		break;

	case EQE_KEY_7:
		BIO_OUT( ENC_GPIO_1, BIO_LED_BLUE_M,  BIO_LED_BLUE_HI_V); 
		break;

	case EQE_KEY_8:
		BIO_OUT( ENC_GPIO_1, BIO_LED_RED_M,   BIO_LED_RED_HI_V); 
		break;

	case EQE_KEY_9:
		BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_HI_V); 
		break;

	case EQE_KEY_F1:
        if(i > 0)
            i--;
		else
			i = 10;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	case EQE_KEY_F2:
        if(i < 10)
            i++;
		else
			i = 0;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}

}
*/

#if (0)
VOID EQA_EFSMelodyList(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	BYTE	iKey = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		//hHnd = ScrollList((char *)EFSMelodyList, 2, 2, 0, 0, 13, 2, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
#ifdef FEATURE_YMU757B_MELODY
		uisnd_music_stop();
#endif
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:
	case EQE_KEY_2:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_SOFTKEY_SELECT:
		switch(EQC_GetScrollIndex(hHnd)){
		case 0:
#ifdef FEATURE_YMU757B_MELODY
			uisnd_music_stop();
#endif
			EQC_MesgBox("OFF", 3, FT_MED,MB_BEEP);
			break;
			break;

		case 1:		
#ifdef FEATURE_YMU757B_MELODY
			uisnd_sample_music(UI_MUSIC_FILE_SND, "test", NULL, (mus_id_type)0, 5);
#endif
			EQC_MesgBox("ON", 3, FT_MED,MB_BEEP);
			break;
		default:
			break;
		}
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

VOID EQA_MelodyOption(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	BYTE	iKey = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		hHnd = EQC_ScrollList(MelodyOption, 2, 2, 0, 0, 13, 2, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:
	case EQE_KEY_2:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_SOFTKEY_SELECT:
		switch(EQC_GetScrollIndex(hHnd)){
		case 0:
//			BIO_TRISTATE(ENC_GPIO_0, BIO_MOTOR_M, BIO_OUTPUT_ENA_V);
//			BIO_OUT(ENC_GPIO_0, BIO_MOTOR_M, BIO_MOTOR_OFF_V);
			uisnd_sample_snd(NULL, NULL, NULL);
			EQC_MesgBox("OFF.", 3, FT_MED,MB_BEEP);
			break;

		case 1:		
//			BIO_TRISTATE(ENC_GPIO_0, BIO_MOTOR_M, BIO_OUTPUT_ENA_V);
//			BIO_OUT(ENC_GPIO_0, BIO_MOTOR_M, BIO_MOTOR_ON_V);
			uisnd_sample_snd(NULL, NULL, NULL);
			EQC_MesgBox("ON", 3, FT_MED,MB_BEEP);
			break;

		default:
			break;
		}
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}
#endif
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
#ifdef	FEATURE_VOICEPLAY
extern const unsigned char AnsMent1_EVRC[];
void SKY_API SKA_GVMVoiceTest(SKY_EVENT_T *pInEvent)
{
//    extern word app_image_checksum;

//    BYTE str[21];
//    UINT8 i;
    HCONTROL hControl;

    switch(pInEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
#ifdef FEATURE_WAVELET
		MM_StartAudio();
#endif // FEATURE_WAVELET
//		midd_PlayVocoder(AnsMent1_EVRC,7475);
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 100, CML_NONE, WA_SHOW_HEADER );
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            SKY_GET_TOKEN(TKN_GVM_VOICE_TEST), 
            WA_SHOW_HEADER);

        hControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_VOICE_PLAYING), 
                                    5, 
                                    MBT_NONE, 
                                    MBT_HELP);
        EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_MESSAGE_BOX_TITLE);


        break;

    case SE_IO_MESGBOX_END:
#ifdef FEATURE_WAVELET
		MM_EndAudio();
#endif // FEATURE_WAVELET
        break;

    case SE_APP_DRAW:

        EQS_TextOutStr(20, 30, (BYTE*)"연주가 끝났습니다");
//        EQS_TextOutStr(55, 30, mob_sw_rev);

/*
        if (s_wChkSum)
        {
            sprintf(str, "CHECKSUM: %04X", s_wChkSum);

            EQS_TextOutStr(20, 50, str);

	        if(app_image_checksum == s_wChkSum)
	        {
		        sprintf(str, "CHECKSUM 정상");
	        }
	        else
	        {
		        sprintf(str, "CHECKSUM 오류");
	        }

            EQS_TextOutStr(20, 65, str);
        }
*/
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pInEvent);
        break;
    }
}
#endif

//For melody volume by happykid
CONST SKY_TOKEN aVol_Mode[2] =
{
    TKN_MELODY_NORMAL_VOL,
    TKN_MELODY_HIGH_VOL
};

LOCAL BYTE *ppVolList[2];
boolean IsApplyHighMelodyGain;

void SKY_API SKA_LCDMusicTotalLevelControl(SKY_EVENT_T *pEvent)
{
	int nCtrl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

		ppVolList[0] = (BYTE *)SKY_GET_TOKEN(aVol_Mode[0]);
		ppVolList[1] = (BYTE *)SKY_GET_TOKEN(aVol_Mode[1]);

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY,
            CL_NONE_MOVE_NONE,
            SKY_GET_TOKEN(TKN_MELODY_TEST_TOTAL_LEVEL_TITLE),
            WA_SHOW_HEADER|WA_SHOW_CML);

		SKY_GetNV(NV_AUTO_MUTE_I, (nv_item_type *)&IsApplyHighMelodyGain);

		EQC_SET_STYLE(EQC_RadioButton(10, 10, 10, 2, ppVolList, (IsApplyHighMelodyGain ? 1:0)),
				   CS_RADIO_BUTTON_BORDER);

       break;
	
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;
    case SE_IO_RADIO_BUTTON_SELECT:
		IsApplyHighMelodyGain = (boolean)(pEvent->dwParam);
		SKY_PutNV(NV_AUTO_MUTE_I, (nv_item_type *)&IsApplyHighMelodyGain);
		break;
//    case SE_SOFTKEY_SAVE:
//    case SE_SOFTKEY_CANCEL:
//    case SE_IO_RADIO_BUTTON_CANCEL:
//    case SE_KEY_CLEAR:
	
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
