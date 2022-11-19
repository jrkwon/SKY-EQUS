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
** 00-07-25   tasik      Construct Core Program for Smart Battery
** 00-07-21   woodstok   Created.
**
** ===========================================================================
*/

#include "customer.h"
#ifdef FEATURE_SMART_BATTERY

/*  
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <string.h>
#include "eqs.h"
#include "smb.h"
#include "hw.h"

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
CONST CHAR* CONST BatteryMenu[] = {SF_BLACK_NUM1"대기가능시간",
					   SF_BLACK_NUM2"통화가능시간",
					   SF_BLACK_NUM3"충전시간",
					   SF_BLACK_NUM4"초기화"
};

CONST CHAR* CONST BatteryInit[] = {SF_BLACK_NUM1"Cal",
					   SF_BLACK_NUM2"630mA배터리",
					   SF_BLACK_NUM3"830mA배터리"
};

/*
**---------------------------------------------------------------------------
**  Internal Function Declarations
**---------------------------------------------------------------------------
*/

unsigned char* SmbIntToPercentFormat(float);
unsigned char* SmbConvertToTimeFormat(float);

VOID EQA_BatteryStandbyTime(EQS_EVENT *);
VOID EQA_BatteryTalkableTime(EQS_EVENT *);  
VOID EQA_BatteryRequiredChargeTime(EQS_EVENT *);   
VOID EQA_BatteryInit(EQS_EVENT *);  
VOID EQA_BatteryTop(EQS_EVENT *);

/*
**---------------------------------------------------------------------------
**  External Function Declarations
**---------------------------------------------------------------------------
*/

extern void SmbCalOffset(void);
extern void SmbInit630mA(void);
extern void SmbInit830mA(void);

      
/*
**---------------------------------------------------------------------------
**  Internal Function Definitions
**---------------------------------------------------------------------------
*/
VOID EQA_BatteryStandbyTime(EQS_EVENT *pInEvent)		// 대기 가능시간
{
	static float relative, standby_time;
	static unsigned char uszRelative[8], uszStandbyTime[7];
	static unsigned int len_relative, len_standby_time;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
		
		relative = 	SmbGetRelativeCapacity();
		standby_time = SmbGetStandbyableTime(); 

		len_relative = strlen((char *)SmbIntToPercentFormat(relative));
		len_standby_time = strlen((char *)SmbConvertToTimeFormat(standby_time)); 
		
		memcpy( (unsigned char*)uszStandbyTime, (unsigned char*)SmbConvertToTimeFormat(standby_time), len_standby_time);
		memcpy( (unsigned char*)uszRelative, (unsigned char*)SmbIntToPercentFormat(relative), len_relative);

        EQS_GetDC(TRUE);
		EQC_StaticText("대기 가능시간", 0, 0, FT_MED, STEXT_DEFAULTS);

		EQS_TextOutStr(20, 20, (byte*)uszStandbyTime);
		EQS_TextOutStr(40, 40, (byte*)uszRelative);
		EQS_UpdateDisplay();

		// To Do Here!!!
		// 대기 가능시간 표시
		
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

VOID EQA_BatteryTalkableTime(EQS_EVENT *pInEvent)		// 통화가능시간
{
    static float relative, talkable_time ;
	static unsigned char uszRelative[8], uszTalkableTime[7];
	static int len_talkable_time, len_relative;	

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
		
		relative = SmbGetRelativeCapacity();
		talkable_time = SmbGetTalkableTime();
		
		len_relative= strlen((char *)SmbIntToPercentFormat(relative));
		len_talkable_time = strlen((char *)SmbConvertToTimeFormat(talkable_time));
		
		memcpy( (unsigned char*)uszTalkableTime, (unsigned char*)SmbConvertToTimeFormat(talkable_time), len_talkable_time);
		memcpy( (unsigned char*)uszRelative, (unsigned char*)SmbIntToPercentFormat(relative), len_relative);
	
        EQS_GetDC(TRUE);
		EQC_StaticText("통화 가능시간", 0, 0, FT_MED, STEXT_DEFAULTS);
		
		EQS_TextOutStr(20, 20, (byte*)uszTalkableTime);
		EQS_TextOutStr(40, 40, (byte*)uszRelative);
		EQS_UpdateDisplay();

		// To Do Here!!!
		// 통화 가능시간 표시


		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

VOID EQA_BatteryRequiredChargeTime(EQS_EVENT *pInEvent)		// 충전시간
{
    static float relative, required_charge_time ;
	static unsigned char uszRelative[8], uszRequiredCharegeTime[7];
	static int len_required_charge_time, len_relative;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
		
		relative = SmbGetRelativeCapacity();
	    required_charge_time = SmbGetRequiredChargeTime();
	
		len_required_charge_time = strlen((char *)SmbConvertToTimeFormat(required_charge_time));
		len_relative = strlen((char *)SmbIntToPercentFormat(relative));	
	
		memcpy( (unsigned char*)uszRequiredCharegeTime, (unsigned char*)SmbConvertToTimeFormat(required_charge_time), len_required_charge_time);
		memcpy( (unsigned char*)uszRelative, (unsigned char*)SmbIntToPercentFormat(relative), len_relative);
			
		EQS_GetDC(TRUE);
		EQC_StaticText("만충전 시간", 0, 0, FT_MED, STEXT_DEFAULTS);
			
		EQS_TextOutStr(20, 20, (byte*)uszRequiredCharegeTime);
		EQS_TextOutStr(40, 40, (byte*)uszRelative);
		EQS_UpdateDisplay();

		// To Do Here!!!
		// 충전시간 표시

		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

VOID EQA_BatteryInit(EQS_EVENT *pInEvent)		// 초기화
{
	static HCONTROL hHnd;
	UINT8	iKey=0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		hHnd = EQC_ScrollList(BatteryInit, 3, 3, 0, 0, 16, 3, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:
	case EQE_KEY_2:
	case EQE_KEY_3:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_SOFTKEY_SELECT:
		switch(EQC_GetScrollIndex(hHnd)){
		case 0:		// Cal Init
			// To Do Here!
			SmbCalOffset();
			EQC_MesgBox("Cal", 3, MB_BEEP);
			hw_reset();
			break;
		case 1:		// 630mA
			// To Do Here!
			SmbInit630mA();
			EQC_MesgBox("630mA", 3, MB_BEEP);
			hw_reset();
			break;

		case 2:		// 830mA
			// To Do Here!
			SmbInit830mA();
			EQC_MesgBox("830mA", 3, MB_BEEP);
			hw_reset();
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

unsigned char* SmbIntToPercentFormat( float input)
{
	int i;
	unsigned char PercentFormat[8];

	i = (int)(input*10);
	memset( PercentFormat, NULL, 6 );
	if ( i >= 1000 )
	{
		PercentFormat[0] = '1';
		PercentFormat[1] = '0';
		PercentFormat[2] = '0';
		PercentFormat[3] = '.';
		PercentFormat[4] = '0';
		PercentFormat[5] = ' ';
		PercentFormat[6] = '%';
		PercentFormat[7] = 0;
	}
	else if(( 100 <= i ) && ( i <= 999 ))
	{
		PercentFormat[0] = i/100 + '0';
        PercentFormat[1] = (i%100)/10 + '0';
		PercentFormat[2] = '.';
		PercentFormat[3] = i%100%10 + '0';
		PercentFormat[4] = ' ';
		PercentFormat[5] = '%';
		PercentFormat[6] = 0;

	}
	else if(( 10 <= i ) && ( i <= 99 ))
	{
		PercentFormat[0] = i/10 + '0';
		PercentFormat[1] = '.';
        PercentFormat[2] = i%10 + '0';
		PercentFormat[3] = ' ';
		PercentFormat[4] = '%';
		PercentFormat[5] = 0;
	}
	else if(( 0 <= i ) && ( i <= 9 ))
	{
		PercentFormat[0] = '0';
		PercentFormat[1] = '.';
        PercentFormat[2] = i%10 + '0';
		PercentFormat[3] = ' ';
		PercentFormat[4] = '%';
		PercentFormat[5] = 0;
	}

    return PercentFormat;
}

unsigned char* SmbConvertToTimeFormat( float input)
{
	int hour, minute;
	unsigned char TimeFormat[7];

	memset( TimeFormat, NULL, 7);
    
	hour = (int) input;
	minute = (int) (input*60.0)%60;

	// 3자리수까지만 표현을 함 

	// 시 처리 
	if(( 100 <= hour ) && ( hour <= 999 ))
	{
		TimeFormat[0] = hour/100 + '0';
        TimeFormat[1] = (hour%100)/10 + '0';
		TimeFormat[2] = hour%100%10 + '0';
		TimeFormat[3] = ':';
		
	}
	else if(( 10 <= hour ) && ( hour <= 99 ))
	{
		TimeFormat[0] = ' ';
		TimeFormat[1] = hour/10 + '0';
	    TimeFormat[2] = hour%10 + '0';
		TimeFormat[3] = ':';
		
	}
	else if(( 0 <= hour ) && ( hour <= 9 ))
	{
		TimeFormat[0] = ' ';
		TimeFormat[1] = ' ';
        TimeFormat[2] = hour + '0';
     	TimeFormat[3] = ':';
			
	}

	// 분 처리 
	if(( 10 <= minute ) && ( minute <= 99 ))
	{
		TimeFormat[4] = minute/10 + '0';
    	TimeFormat[5] = minute%10 + '0';
		TimeFormat[6] = 0;
	}
	else 
	{
		TimeFormat[4] = ' ';
        TimeFormat[5] = minute+'0';
		TimeFormat[6] = 0;
	}

    return TimeFormat;
}


/*
**---------------------------------------------------------------------------
**  External Function Definitions
**---------------------------------------------------------------------------
*/

VOID EQA_BatteryTop(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	UINT8	iKey=0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		hHnd = EQC_ScrollList(BatteryMenu, 4, 4, 0, 0, 16, 4, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:
	case EQE_KEY_2:
	case EQE_KEY_3:
	case EQE_KEY_4:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_SOFTKEY_SELECT:
		switch(EQC_GetScrollIndex(hHnd)){
		case 0:	
			EQS_StartChild(EQA_BatteryStandbyTime);
			break;
		case 1:		
			EQS_StartChild(EQA_BatteryTalkableTime);
			break;
		case 2:		
			EQS_StartChild(EQA_BatteryRequiredChargeTime);
			break;
		case 3:		
			EQS_StartChild(EQA_BatteryInit);
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


#endif // #ifdef FEATURE_SMART_BATTERY
/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
