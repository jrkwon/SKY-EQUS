/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     sky_annu.c
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
** 2001-01-26 jrkwon     Annuciators for the sub LCD
** 2000-12-13 girin      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"
//#include "msg.h"
//#include "err.h"
//#include "srch.h"
//#include "rxc.h"
//#include "cm.h"
#include "vbatt.h"
#include "sky.h"
#include "ska.h"
#ifdef FEATURE_SKT_CAI
#include "mccdma.h"
#endif

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
///////////////////
// BATTERY LEVEL //
///////////////////

// Define the values used to represent the number of bars displayed in the
// battery icon.
// And, also these defines are used for the offset of battery bitmap offset.
#define BARS_0		0
#define BARS_1		1
#define BARS_2		2
#define BARS_3		3
#ifdef FEATURE_SMART_BATTERY
#define BARS_4		4
#define BARS_5		5
#define BARS_6		6
#define BARS_7		7
#define BARS_8		8
#define BARS_9		9
#define BARS_10		10
#endif//#ifdef FEATURE_SMART_BATTERY



//////////
// RSSI //
//////////
#define CMPH_RSSI_NO_SIGNAL   125 /* No CDMA signal index value */


//////////////////
// IN USE STATE //
//////////////////
//#define NO_USE_STATE    0
//#define IN_USE_STATE    1


/////////
// SMS //
/////////
#define SMS_NO_MESSAGE      0
#define SMS_NEW_MESSAGE     1
#define SMS_READ_MESSAGE    2 /* <- no use */      

/////////////////////
// Bell & Vibrator //
/////////////////////
/*#define BELLVIB_BELLVIB     0
#define BELLVIB_BELL        1
#define BELLVIB_VIB         2
#define BELLVIB_NONE        3*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
#ifdef SW_RESET_ENABLE
struct tagSAVE_SW_RESET_T {
    BOOL fAutoLock;
    DWORD   dwCTime;
    UINT8   nBars;
    UINT8   nSmartBar;
    UINT8   RSSIBar;
    UINT8   SMS;
    UINT8   BellVib;
    UINT8   Protocol;
};

typedef struct tagSAVE_SW_RESET_T SAVE_SW_RESET_T;

#endif
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
BOOL g_fNoService = TRUE;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
STATIC UINT8 l_nLastBars = BARS_3;        /* init to 4 bars */
STATIC UINT8 l_nLastSmartBars = 10;       /* init to 100% */
STATIC UINT8 l_nLastRSSIBars = 0;
//STATIC UINT8 l_nLastInUseState = NO_USE_STATE;
LOCAL IN_USE_STATE_E l_LastInUseState = IUS_DISCONNECT;
STATIC UINT8 l_nLastSMS = SMS_NO_MESSAGE;
//STATIC UINT8 l_nLastBellVib = BELLVIB_BELLVIB;
LOCAL BELLVIB_ICON_E l_LastBellVib = BELLVIB_ICON_BELL_ONLY;
LOCAL PROTOCOL_REVISION_E l_LastProtocol = PR_IS95C;

LOCAL BOOL l_fSMB = FALSE;

/*  
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL BOOL sHasAlarmData(void);

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define ANNUNCIATORS_POS_Y  0

#define RSSI_POS_X         0
#define ROAM_POS_X         0
#define PRTC_POS_X         RSSI_POS_X+21
#define CALL_POS_X         PRTC_POS_X+11
#define AUTO_POS_X         CALL_POS_X+15
#define BLVB_POS_X         AUTO_POS_X+17
#define ALRM_POS_X         BLVB_POS_X+15
#define SMS_POS_X          ALRM_POS_X+13
#define BATT_POS_X         SMS_POS_X+18

BOOL SKY_CheckAnnunciators(void){
	BOOL fEven = FALSE;
    if (SKY_SetThenCompareRSSI(SKY_GET_PHONE_INFO_RSSI()) ||
        SKY_SetThenCompareProtocolRevision(SKY_GET_PROTOCOL_REV()))
	{
		fEven = TRUE;
    }
    if (SKY_SetThenCompareBattLevel()) // if batt. level is changed
    {
		fEven = TRUE;
    }
	if (SKY_SetThenCompareInUseState((IN_USE_STATE_E)SKY_GetInUseState()))
	{
		fEven = TRUE;
	}
	if(l_nLastSMS != SMS_NO_MESSAGE)
	{
		fEven = TRUE;
	}
    if (SKY_IS_NEW_MESSAGE()) { // 확인
        fEven = TRUE;
    }
	return fEven;
}

/* undersea 2001-08-27 --> @GVM에서 256 partial display를 하기 위함 */
#if defined(FEATURE_GVM) || defined(FEATURE_SKVM)
void SKY_Draw256Annunciators(void) 
{
    BITMAP Bitmap;
    UINT8 nBattLevel;
    STATIC BOOL s_fDrawNewMessage = TRUE;

    /* RSSI & ROAMING */
    if(SKY_GET_PHONE_INFO_ROAM_STATUS() == CM_ROAM_STATUS_OFF)
    {
        Bitmap = g_BmpAnnunRSSI;
        Bitmap.pbData = g_BmpAnnunRSSI.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_nLastRSSIBars;
        
        SKA_Put256Bitmap(RSSI_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());        
        //SKA_Put256Bitmap(RSSI_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer);        
    }
    else
    {
        Bitmap = g_BmpAnnunRoamRSSI;
        Bitmap.pbData = g_BmpAnnunRoamRSSI.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_nLastRSSIBars;
        SKA_Put256Bitmap(ROAM_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());                
    }

    /* protocol - IS95A, IS95B, IS95C */
    Bitmap = g_BmpAnnunProtocol;
    //Bitmap.pbData = g_BmpNew95AB11x14x256C.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_LastProtocol;
    /* 프로토콜 표시 하지 않는 것으로 결정.. 95C의 경우 별도의 표시 없으므로 95C로 고정함. */
    Bitmap.pbData = g_BmpAnnunProtocol.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * PR_IS95C;
    SKA_Put256Bitmap(PRTC_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());            

    /* call .. 4 gray image with a palette information */
    Bitmap.Header = g_BmpAnnunCall.Header;
    Bitmap.pbData = g_BmpAnnunCall.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_LastInUseState;
    Bitmap.pbPalette = g_BmpAnnunCall.pbPalette;
    SKA_Put256Bitmap(CALL_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());            
    
    /* auto answer */
    if(SKY_IS_AUTO_ANSWER())
    {
        Bitmap = g_BmpAnnunAnsweringMachine;
        Bitmap.pbData = g_BmpAnnunAnsweringMachine.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 1;
        SKA_Put256Bitmap(AUTO_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());                    
    }
    else /* empty */
    {
        Bitmap = g_BmpAnnunAnsweringMachine;
        SKA_Put256Bitmap(AUTO_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());         
    }

    Bitmap = g_BmpAnnunBellVibration;
    Bitmap.pbData = g_BmpAnnunBellVibration.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*l_LastBellVib;
    SKA_Put256Bitmap(BLVB_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer()); 
        
    /* alarm */
    if(sHasAlarmData())
    {
        Bitmap = g_BmpAnnunAlarm;
        Bitmap.pbData = g_BmpAnnunAlarm.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 1;
        SKA_Put256Bitmap(ALRM_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());         
    }
    else /* empty */
    {
        Bitmap = g_BmpAnnunAlarm;
        SKA_Put256Bitmap(ALRM_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());         
    }

    /* SMS */
    if(l_nLastSMS != SMS_NO_MESSAGE)
    {
        Bitmap = g_BmpAnnunMessage;
        if(s_fDrawNewMessage)
        {
            Bitmap.pbData = g_BmpAnnunMessage.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 1;
            s_fDrawNewMessage = FALSE;
        }
        else
        {
            s_fDrawNewMessage = TRUE;
        }
        SKA_Put256Bitmap(SMS_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());                 
    }
    else /* empty */
    {
        Bitmap = g_BmpAnnunMessage;
        SKA_Put256Bitmap(SMS_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());         
    }

    /* 
    ** batter level 
    */
#ifdef FEATURE_SMART_BATTERY
    nBattLevel = (l_fSMB) ? l_nLastSmartBars : l_nLastBars;
    
    Bitmap.Header = (l_fSMB) ? g_BmpAnnunBattery.Header : g_BmpAnnunBattery.Header;
    Bitmap.pbData = ((l_fSMB) ? g_BmpAnnunBattery.pbData : g_BmpAnnunBattery.pbData)
                    + EQC_GET_FRAME_SIZE(Bitmap.Header) * nBattLevel;
#else
    nBattLevel = l_nLastBars;
    
    Bitmap.Header = g_BmpAnnunBattery.Header;
    Bitmap.pbData = g_BmpAnnunBattery.pbData
                    + EQC_GET_FRAME_SIZE(Bitmap.Header) * nBattLevel;

#endif
    SKA_Put256Bitmap(BATT_POS_X, ANNUNCIATORS_POS_Y, &Bitmap, (BYTE*)GetScreenBuffer());             
    
    /* 
    ** jrkwon 2001.06.12 --> default event handler로 옮김 --> it makes bugs bugs bugs..
    ** jrkwon 2001.06.18 --> draw when main lcd annunciators are changed.
    */
    /* jrkwon 2001-05-04 --> */
#ifdef WAPPER
    if(SKY_IS_IN_WAP())
        SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
    else
#endif
    /* <-- */
        SKY_DrawSubAnnunciators();
}
#endif // FEATURE_GVM
/* <-- */

void SKY_DrawAnnunciators(void)
{
    BITMAP Bitmap;
    UINT8 nBattLevel;
    STATIC BOOL s_fDrawNewMessage = TRUE;
    VIEW_PORT ViewPort;

    GetViewPort(&ViewPort);
    ResetViewPort();


    /* RSSI & ROAMING */
    if(SKY_GET_PHONE_INFO_ROAM_STATUS() == CM_ROAM_STATUS_OFF)
    {
        Bitmap = g_BmpAnnunRSSI;
        Bitmap.pbData = g_BmpAnnunRSSI.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_nLastRSSIBars;
        PutBitmap(RSSI_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);        
    }
    else
    {
        Bitmap = g_BmpAnnunRoamRSSI;
        Bitmap.pbData = g_BmpAnnunRoamRSSI.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_nLastRSSIBars;
        PutBitmap(ROAM_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);
    }

    /* protocol - IS95A, IS95B, IS95C */
    Bitmap = g_BmpAnnunProtocol;
    //Bitmap.pbData = g_BmpNew95AB11x14x256C.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_LastProtocol;
    /* 프로토콜 표시 하지 않는 것으로 결정.. 95C의 경우 별도의 표시 없으므로 95C로 고정함. */
    Bitmap.pbData = g_BmpAnnunProtocol.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * PR_IS95C;
    PutBitmap(PRTC_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);

    /* call .. 4 gray image with a palette information */
    Bitmap.Header = g_BmpAnnunCall.Header;
    Bitmap.pbData = g_BmpAnnunCall.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_LastInUseState;
    Bitmap.pbPalette = g_BmpAnnunCall.pbPalette;
    PutBitmap(CALL_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);

    /* auto answer */
    if(SKY_IS_AUTO_ANSWER())
    {
        Bitmap = g_BmpAnnunAnsweringMachine;
        Bitmap.pbData = g_BmpAnnunAnsweringMachine.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 1;
        PutBitmap(AUTO_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);
    }
    else /* empty */
    {
        Bitmap = g_BmpAnnunAnsweringMachine;
        PutBitmap(AUTO_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);
    }

    Bitmap = g_BmpAnnunBellVibration;
    Bitmap.pbData = g_BmpAnnunBellVibration.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*l_LastBellVib;
    PutBitmap(BLVB_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);
    
    /* alarm */
    if(sHasAlarmData())
    {
        Bitmap = g_BmpAnnunAlarm;
        Bitmap.pbData = g_BmpAnnunAlarm.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 1;
        PutBitmap(ALRM_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);
    }
    else /* empty */
    {
        Bitmap = g_BmpAnnunAlarm;
        PutBitmap(ALRM_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);
    }

    /* SMS */
    if(l_nLastSMS != SMS_NO_MESSAGE)
    {
        Bitmap = g_BmpAnnunMessage;
        if(s_fDrawNewMessage)
        {
            Bitmap.pbData = g_BmpAnnunMessage.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 1;
            s_fDrawNewMessage = FALSE;
        }
        else
        {
            s_fDrawNewMessage = TRUE;
        }
        PutBitmap(SMS_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);
    }
    else /* empty */
    {
        Bitmap = g_BmpAnnunMessage;
        PutBitmap(SMS_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);
    }

    /* 
    ** batter level 
    */
#ifdef FEATURE_SMART_BATTERY
    nBattLevel = (l_fSMB) ? l_nLastSmartBars : l_nLastBars;
    
    Bitmap.Header = (l_fSMB) ? g_BmpAnnunBattery.Header : g_BmpAnnunBattery.Header;
    Bitmap.pbData = ((l_fSMB) ? g_BmpAnnunBattery.pbData : g_BmpAnnunBattery.pbData)
                    + EQC_GET_FRAME_SIZE(Bitmap.Header) * nBattLevel;
#else
    nBattLevel = l_nLastBars;
    
    Bitmap.Header = g_BmpAnnunBattery.Header;
    Bitmap.pbData = g_BmpAnnunBattery.pbData
                    + EQC_GET_FRAME_SIZE(Bitmap.Header) * nBattLevel;

#endif
    PutBitmap(BATT_POS_X, ANNUNCIATORS_POS_Y, &Bitmap);

    SetViewPort(&ViewPort);

    /* 
    ** jrkwon 2001.06.12 --> default event handler로 옮김 --> it makes bugs bugs bugs..
    ** jrkwon 2001.06.18 --> draw when main lcd annunciators are changed.
    */
    /* jrkwon 2001-05-04 --> */
#ifdef WAPPER
    if(SKY_IS_IN_WAP())
        SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
    else
#endif
    /* <-- */
        SKY_DrawSubAnnunciators();
}

#if SKY_MODEL_ID == SKY_MODEL_B1 || (SKY_MODEL_ID == SKY_MODEL_B2)

#define sPOS_Y              2

#define sRSSI_POS_X         0
#define sROAM_POS_X         0
#define sPRTC_POS_X         14
#define sAUTO_POS_X         22
#define sBLVB_POS_X         34
#define sALRM_POS_X         46
#define sSMS_POS_X          56
#define sBATT_POS_X         68

#elif SKY_MODEL_ID == SKY_MODEL_B0  

/* not yet implemented */

#define sPOS_Y              2

#define sRSSI_POS_X         0
#define sROAM_POS_X         0
#define sPRTC_POS_X         15
#define sAUTO_POS_X         23
#define sBLVB_POS_X         35
#define sALRM_POS_X         47
#define sSMS_POS_X          57
#define sBATT_POS_X         69

#else

#error What's the MODEL ID?

#endif

LOCAL BOOL sHasAlarmData(void)
{
    INT8 i;

    for(i = 0; i < MAX_ALARM_COUNT; i++)
    {
        if((SKY_GET_ALARM_TIME_TYPE(i) != ATT_NONE) && 
           (SKY_IS_ALARM_TIME_ACTIVE(i) == TRUE))
            return TRUE;
    }
    return FALSE;
}

void SKY_API SKY_DrawSubAnnunciatorsIcon()
{
    BITMAP Bitmap;
    UINT8 nBattLevel;
    STATIC BOOL s_fDrawNewMessage = TRUE;    

    if(SKY_SubLCDIsEndText(LN_UPPER) == FALSE)
        return;

    OPEN_SUB_LCD();

    /* clear the upper area of LCD */
    CLEAR_SUB_LCD_UPPER();
    //CLOSE_SUB_LCD();

    /* RSSI & ROAMING */
    if(SKY_GET_PHONE_INFO_ROAM_STATUS() == CM_ROAM_STATUS_OFF)
    {
        Bitmap.Header = g_BmpSubAnnunRSSI.Header;
        Bitmap.pbData = g_BmpSubAnnunRSSI.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_nLastRSSIBars;
        //SKY_SubLCDPutBitmap(sRSSI_POS_X, sPOS_Y, &Bitmap);
        PutBitmap(sRSSI_POS_X, sPOS_Y, &Bitmap);
    }
    else
    {
        Bitmap.Header = g_BmpSubAnnunRoamRSSI.Header;
        Bitmap.pbData = g_BmpSubAnnunRoamRSSI.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_nLastRSSIBars;
        //SKY_SubLCDPutBitmap(sROAM_POS_X, sPOS_Y, &Bitmap);
        PutBitmap(sROAM_POS_X, sPOS_Y, &Bitmap);
    }

    /* protocol - IS95A, IS95B, IS95C */
    Bitmap = g_BmpSubAnnunProtocol;
    //Bitmap.pbData = g_BmpSubAnnunProtocol.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_LastProtocol;
    /* 프로토콜 표시 하지 않는 것으로 결정.. 95C의 경우 별도의 표시 없으므로 95C로 고정함. */
    Bitmap.pbData = g_BmpSubAnnunProtocol.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * PR_IS95C;
    EQS_PutBitmap(sPRTC_POS_X, sPOS_Y, &Bitmap);


    /* auto answer */
    if(SKY_IS_AUTO_ANSWER())
    {
        Bitmap = g_BmpSubAnnunAnsweringMachine;
        //SKY_SubLCDPutBitmap(sAUTO_POS_X, sPOS_Y, &Bitmap);
        PutBitmap(sAUTO_POS_X, sPOS_Y, &Bitmap);
    }

    if(l_LastBellVib != 0) /* 즉시 자동응답인 경우, 표시하지 않음 */
    {
        Bitmap = g_BmpSubAnnunBellVibration;
        Bitmap.pbData = g_BmpSubAnnunBellVibration.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*l_LastBellVib;
        PutBitmap(sBLVB_POS_X, sPOS_Y, &Bitmap);
    }

    /* alarm */
    if(sHasAlarmData())
    {
        Bitmap = g_BmpSubAnnunAlarm;
        //SKY_SubLCDPutBitmap(sALRM_POS_X, sPOS_Y, &Bitmap);
        PutBitmap(sALRM_POS_X, sPOS_Y, &Bitmap);
    }

    /* SMS */
    if(l_nLastSMS != SMS_NO_MESSAGE)
    {
        if(s_fDrawNewMessage)
        {
            Bitmap = g_BmpSubAnnunMessage;
            PutBitmap(sSMS_POS_X, sPOS_Y, &Bitmap);

            s_fDrawNewMessage = FALSE;
        }
        else
        {
            s_fDrawNewMessage = TRUE;
        }
    }

    /* 
    ** batter level 
    */
    nBattLevel = (l_fSMB) ? l_nLastSmartBars : l_nLastBars;

    Bitmap.Header = g_BmpSubAnnunBattery.Header;
    Bitmap.pbData = g_BmpSubAnnunBattery.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nBattLevel;
    PutBitmap(sBATT_POS_X, sPOS_Y, &Bitmap);
    
    CLOSE_SUB_LCD();
}

void SKY_API SKY_DrawSubAnnunciators(void)
{
#ifdef WAPPER
    if(SKY_IS_IN_WAP())
    {
        SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
        return;
    }
#endif
    SKY_DrawSubAnnunciatorsIcon();
}

// girin. 2000


/////////////////////////////////////////////////////////////////
// girin. 2000.5.27
//
// Select batt. level display scheme  
// by defining one of below MACRO's   
//
// 1) #define MIN_MAX_DROPPING_AVG
// 2) #define IIR_4_FILTERING
// 3) #define THROSHOLD_CUTOFF_IIR_FILTERING
// 4) No #define for N=8 IIR filtering
/////////////////////////////////////////////////////////////////
//#define MIN_MAX_DROPPING_AVG // girin. 2000.5.27
#define THROSHOLD_CUTOFF_IIR_FILTERING // girin. 2000.5.30
// #define IIR_4_FILTERING

#if defined (MIN_MAX_DROPPING_AVG) // girin. 2000.5.27
#define BATT_HISTORY_BUF_NUM	8
static word min_max_droping_avg(word in_battery_level)
{
	static byte cur_pos = 1;
	static word batt_history_buf[BATT_HISTORY_BUF_NUM] = { 0xffff, 0, 0, 0, 0, 0, 0, 0 };

	byte i;
	dword sum = 0;
	word min_candidate = 0xffff;
	word max_candidate = 0x0000;
	
	// put the input value on batt_history_buf
	batt_history_buf[cur_pos++] = in_battery_level;
	cur_pos %= BATT_HISTORY_BUF_NUM;

	if (batt_history_buf[0] == 0xffff) // not yet fully quanched.
	{
		return (in_battery_level * 2);
	}

	// add 
	for (i = 0, sum = 0; i < BATT_HISTORY_BUF_NUM; i++)
	{
		sum += batt_history_buf[i];

		if (min_candidate > batt_history_buf[i])
		{
			min_candidate = batt_history_buf[i];
		}

		if (max_candidate < batt_history_buf[i])
		{
			max_candidate = batt_history_buf[i];
		}
	}

	sum = sum - min_candidate - max_candidate;

	return (word)(sum / 3); // i.e. (sum * 2 / (BATT_HISTORY_BUF_NUM - 2) );
}
#endif

#if defined (THROSHOLD_CUTOFF_IIR_FILTERING)

#define THRESHOLD_EXTRA_CHANGE	15 // max. endurable ADC fluctuation.
#define MAX_EXTRA_NUM		    10//5

#define ABS_DIFF(a, b) ((a > b) ? (a - b) : (b - a))
 
LOCAL UINT16 throshold_cutoff(UINT16 in_battery_level)
{
	STATIC UINT8 extra_num = MAX_EXTRA_NUM;
	STATIC UINT16 batt_history_buf = 0;

	// calculate the diff. value and throw in-value off if it is inadequate.
	if (ABS_DIFF(batt_history_buf, in_battery_level) > THRESHOLD_EXTRA_CHANGE)
	{
		extra_num++;

		if (extra_num > MAX_EXTRA_NUM)
		{ // Then, accept the chang... You can't help accepting the changes.
			batt_history_buf = in_battery_level;
			extra_num = 0;
			return (in_battery_level * 2);
		}
		else
		{
			// throw in_battery_level and use the value ever saved.
			return (batt_history_buf * 2);
		}
	}
	else
	{
		batt_history_buf = in_battery_level;
		extra_num = 0;
		return (in_battery_level * 2);
	}

	// cannot reach here, just for pacifying the compiler.
	return (in_battery_level * 2);
}
#endif

#ifdef FEATURE_SMART_BATTERY

#define LOW_BATT_LIMIT      3.0

LOCAL BOOL sSetThenCompareSmartBattLevel(void)
{
    STATIC float s_fLastPercent;
    float fCurrentPercent;
    BOOL fReturn = TRUE;

    /* get current battery level */
    fCurrentPercent = smb_percent_capacity(); /* 0.0 ~ 100.0 */

    /* compare the current value with the last value */
    fReturn = (fCurrentPercent == s_fLastPercent) ? FALSE : TRUE;
    
    if(fCurrentPercent == 100.0)
        l_nLastSmartBars = 10;
    else if(fCurrentPercent < LOW_BATT_LIMIT)
        l_nLastSmartBars = 0;
    else
        l_nLastSmartBars = fCurrentPercent / 10 + 1;

    /* save the current battery level */
    s_fLastPercent = fCurrentPercent;

    return fReturn;
}

#endif//#ifdef FEATURE_SMART_BATTERY

#define UNSET_BATT ((VBATT_SCALE * 2) + 1)     /* battery never read */
// girin. 2000.5.2: four batt. level
#define BATT_LEVEL_2_BAR    (ibatt_ref[0]*2)  //    -- 2 bars
#define BATT_LEVEL_1_BAR    (ibatt_ref[1]*2)  //    -- 1 bar
#define BATT_LEVEL_0_BAR    (ibatt_ref[2]*2)  //    -- 0 bars
#define BATT_LEVEL_PWR_OFF  (ibatt_ref[3]*2)

#define TRAF_BATT_LEVEL_2_BAR    (tbatt_ref[0]*2)  //    -- 2 bars
#define TRAF_BATT_LEVEL_1_BAR    (tbatt_ref[1]*2)  //    -- 1 bar
#define TRAF_BATT_LEVEL_0_BAR    (tbatt_ref[2]*2)  //    -- 0 bars
#define TRAF_BATT_LEVEL_PWR_OFF  (tbatt_ref[3]*2)

// downward value
/*
#define DOWNWARD_HALF_BAR_IN_IDLE        10
#define DOWNWARD_HALF_BAR_IN_TALK        13
*/

HALF_BAR_T g_HalfBar = { 20, 55 }; // upward

BOOL SKY_SetThenCompareBattLevel(void)
{
    // in rfnv.c
    extern unsigned char ibatt_ref[];
    extern unsigned char tbatt_ref[];
    db_items_value_type dbi; // db_items_value_type isn't big (it's small union var. type
    STATIC UINT16 lastbatt=UNSET_BATT, batt=0;  /* last battery value */
    //STATIC BOOL lastextpwr=FALSE;  /* last external power */
    UINT8 i;
	STATIC	UINT16 batt_power_off = 0;
	STATIC	UINT16 batt_level_0 = 0;
	STATIC	UINT16 batt_level_1 = 0;
	STATIC	UINT16 batt_level_2 = 0;
    UINT8 HALF_BAR;// = 13; // girin. 2000.5.10:
    BOOL fRet;
    STATIC BOOL s_fLowBatt = FALSE;
    STATIC UINT8 s_nLowBattAlarmCount = 0;

    if (SKY_IS_EXTERNAL_POWER())
        dbi.battery_level = 0xff;
    else
        dbi.battery_level = vbatt_read();

    if( lastbatt == UNSET_BATT ) 
    {   // First time thru
        batt = (word)dbi.battery_level * 2 ;  /* Start with first reading */
    } 
    else 
    {                                /* Otherwise, IIR */
#ifdef IIR_4_FILTERING 
        // girin. 2000.5.11: get the N factor of (N-1)/N IIR filtering smaller for test.
        batt = ((batt*3) + (word)(dbi.battery_level * 2)) / 4; /* IIR */
#elif defined (MIN_MAX_DROPPING_AVG)
        batt = min_max_droping_avg(dbi.battery_level);
#elif defined (THROSHOLD_CUTOFF_IIR_FILTERING)
        batt = ((batt*7) + (word)(throshold_cutoff(dbi.battery_level))) / 8;
#else
        batt = ((batt*7) + (word)(dbi.battery_level * 2)) / 8; // IIR 
#endif
    }

    if( batt != lastbatt )// || SKY_IS_EXTERNAL_POWER() ) 
    {
        db_put( DB_BATTERY_LEVEL, &dbi);
    }

    // if batt == lastbatt, it's downward battery level
    //fUpwardBattery = (batt > lastbatt) ? TRUE : FALSE;

    /* get db */
    db_get(DB_IN_USE, &dbi);

    if(dbi.in_use) // SKY_GetInUseState() == IUS_DISCONNECT)//SKY_IS_IN_USE())
    {
        batt_power_off = TRAF_BATT_LEVEL_PWR_OFF;
        batt_level_0 = TRAF_BATT_LEVEL_0_BAR;
        batt_level_1 = TRAF_BATT_LEVEL_1_BAR;
        batt_level_2 = TRAF_BATT_LEVEL_2_BAR;
        //HALF_BAR = fUpwardBattery ? g_HalfBar.nInTalk : DOWNWARD_HALF_BAR_IN_TALK;
        HALF_BAR = g_HalfBar.nInTalk;
    }
    else 
    {
        batt_power_off = BATT_LEVEL_PWR_OFF;
        batt_level_0 = BATT_LEVEL_0_BAR;
        batt_level_1 = BATT_LEVEL_1_BAR;
        batt_level_2 = BATT_LEVEL_2_BAR;
        //HALF_BAR = fUpwardBattery ? g_HalfBar.nInIdle : DOWNWARD_HALF_BAR_IN_IDLE;
        HALF_BAR = g_HalfBar.nInIdle;
    }

    if( batt <= batt_power_off ) 
    {

#ifdef FEATURE_SMART_BATTERY           // jwpark 00.05.18 : Smart Battery
        SmbEvent(SMB_LOW_BATT_S);
#endif // #ifdef FEATURE_SMART_BATTERY // jwpark 00.05.18 end

        i = BARS_0; // girin. add. 2000.5.15

#ifdef YAMAHA_MELODY
        //uisnd_sample_snd( UI_MUSIC_SND, MUS_LOW_BATT, UISND_1ST_VOL, 1);
#else
        //uisnd_tone( UI_EARBEEP_SND, SND_ERR, 100 );  /* so power down */
#endif
        EQS_SEND_EVENT(SE_SYS_BAD_BATT, EQS_NA, EQS_NA);
    }
    else if ( batt <= batt_level_0 ) // jrkwon 2001-05-25 else if ( batt < batt_level_0 ) 
    {
        i = BARS_0;
    }
    else if ( batt <= batt_level_1 ) // jrkwon 2001-05-25 else if ( batt < batt_level_1 ) 
    {
        if ((l_nLastBars != BARS_0) || (batt > (batt_level_0 + HALF_BAR))) 
        {
            i = BARS_1;
        } 
        // girin. 2000.5.11
        else if (l_nLastBars == BARS_0)
        {
            i = l_nLastBars;
        }
        // girin. end.
        else 
        {
            i = l_nLastBars;
        }
    }
    else if ( batt <= batt_level_2 ) // jrkwon 2001-05-25 else if ( batt < batt_level_2 ) 
    {
        if ((l_nLastBars != BARS_1) || (batt > (batt_level_1 + HALF_BAR))) 
        {
            /* display 2 batt bar  */
            i = BARS_2;
        } 
        else 
        {
            i = l_nLastBars;
        }
    }
    else if ((l_nLastBars >= BARS_3) || (batt > (batt_level_2 + HALF_BAR))) 
    {
        /* display 3 batt bars */
        i = BARS_3;
    }
    else 
    {
        i = l_nLastBars;
    }

    lastbatt = batt;     /* save the battery reading */

    // Low Batt.
    if (i == BARS_0)
    {
        if (s_fLowBatt == FALSE) // for the first time of Low Batt.
        {
            s_fLowBatt = TRUE;
            s_nLowBattAlarmCount = 0;
            EQS_SEND_EVENT(SE_SYS_LOW_BATT, EQS_NA, EQS_NA);
        }
    
        if (((s_nLowBattAlarmCount++) % (60000 / CLOCK_PACEMAKER_REPEAT_TIME)) == 0)
            EQS_SEND_EVENT(SE_SYS_LOW_BATT, EQS_NA, EQS_NA);
    }
    else
    {
        s_fLowBatt = FALSE;
    }

    if (l_nLastBars != i)
    {
        fRet = TRUE; // Annunciator would change
        l_nLastBars = i;       /* save the number of battery bars */
    }
    else
    {
        fRet = FALSE;
    }

#ifdef FEATURE_SMART_BATTERY

    if(SKY_IS_SMART_BATTERY() && (smb_is_operating() == BQ2018_RAM_OK_S))
    {
        l_fSMB = TRUE;
        fRet = sSetThenCompareSmartBattLevel();
    }
    else
    {
        l_fSMB = FALSE;
    }

#endif//#ifdef FEATURE_SMART_BATTERY

    return fRet;
}

#ifdef FEATURE_SKT_CAI
static byte no_svc_cnt;
static db_items_value_type  srchdz_db_value_debug;
extern int mcsyspr_scan_list_count;
#endif 

#define sMAX_RSSI_HISTROY       6

LOCAL WORD sGetRSSI(WORD* pRSSI)
{
    INT8 i;
    LONG lSum;

    for(i = 0, lSum = 0; i < sMAX_RSSI_HISTROY; i++)
    {
        lSum += pRSSI[i];
    }
    return lSum / sMAX_RSSI_HISTROY;
}

BOOL SKY_SetThenCompareRSSI(UINT16 nRSSI)
{
    UINT8 nRSSIBars;
    UINT8 i;
    BOOL fRet;
#ifdef FEATURE_CDMA_1900
    static byte rssi_threshold_val[6] = { 0, 0, 108, 103, 98, 93 };
#else /* !FEATURE_CDMA_1900 */
    // jnlee 01.02.27
	STATIC UINT8 rssi_threshold_val[6] = { 0, 0, 104, 98, 93, 89 };  // -98 -93 -88 -83
#endif /* !FEATURE_CDMA_1900 */
    STATIC WORD s_awRSSI[sMAX_RSSI_HISTROY] = { 98, 98, 98, 98, 98, 98 };
    STATIC UINT8 s_nCount = 0;

    s_awRSSI[s_nCount++] = nRSSI;
    s_nCount %= sMAX_RSSI_HISTROY;

    nRSSI = sGetRSSI(s_awRSSI);

    /* Digital RSSI */
    if ( nRSSI != CMPH_RSSI_NO_SIGNAL ) 
    {  /* Turn RSSI off */
        /* map real RSS value into RSSI bar level 1 through 5 */
        for( i = 5; i > 1; i-- )
        {
            if( nRSSI < rssi_threshold_val[i] )
            {
                break;
            }
        }

        nRSSIBars = i;
    }
    else
    {
        nRSSIBars = 0;
    }

    // Qualcomm original:: hcmd_ptr->rssi.blink_rssi = hcmd_ptr->rssi.rssi = nRSSI;
#ifdef SW_RESET_ENABLE
    if(mc_bSwReset && nRSSIBars == 0)
    {
        return FALSE;
    }
#endif
     
    if (l_nLastRSSIBars != nRSSIBars)
    {
        fRet = TRUE;
        l_nLastRSSIBars = nRSSIBars;

        // No Service
        if(!nRSSIBars)
        {
            if(SKY_IS_NOSERVICE_WARNING_SIGNAL())
            {
#if (MODEL_ID == MODEL_B0)
                SKY_PlaySoundEx( UI_MSG_SND, MUS_LOW_BATT, 1, 1);
#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
                SKY_PlaySoundEx( UI_MSG_SND, SND_SVC_CHANGE, 1, 1);
#endif
            }

            g_fNoService = TRUE;
        }
        else
            g_fNoService = FALSE;
    }
    else
    {
        fRet = FALSE;
    }

    return fRet;
}

IN_USE_STATE_E SKY_API SKY_GetInUseState(void)
{
    if(SKY_IS_IN_USE())
        return IUS_CONNECT;
    else if(SKY_IS_DORMANT())
        return IUS_DORMANT;
    else
        return IUS_DISCONNECT;
}

BOOL SKY_SetThenCompareInUseState(IN_USE_STATE_E InUseState)
{
    //UINT8 nInUseState;
    //BOOL fRet;

    /*switch(InUseState)
    {
    case IUS_DISCONNECT :
        break;
    case IUS_CONNECT :
        break;
    case IUS_DORMANT :
        break;
    }*/

    if(l_LastInUseState != InUseState)
    {
        l_LastInUseState = InUseState;
        return TRUE;
    }
    return FALSE;

    /*if (InUseState == IUS_CONNECT)
    {
        nInUseState = IN_USE_STATE;
    }
    else
    {
        nInUseState = NO_USE_STATE;
    }

    if (l_nLastInUseState != nInUseState)
    {
        fRet = TRUE;
        l_nLastInUseState = nInUseState;
    }
    else
    {
        fRet = FALSE;
    }

    return fRet;*/
}

#define sFAST_PACEMAKER_SECOND      500    /* millisecond */

BOOL SKY_SetThenCompareSMS(SMS_ICON_T SmsIcon)
{
    UINT8 nSMS;
    BOOL fRet;

    /* default value */
    SKY_SET_NEW_MESSAGE(FALSE);

    switch (SmsIcon)
    {
    case SMS_ICON_NEW_MESSAGE:
        nSMS = SMS_NEW_MESSAGE;
        SKY_SET_NEW_MESSAGE(TRUE);
        /* set 1 sec. pacemaker event interval */
        SKY_SetPaceMakerTimer(sFAST_PACEMAKER_SECOND, TRUE);//LONG lIntervalMS, BOOL fRepeat);

        break;
    case SMS_ICON_READ_MESSAGE:
        nSMS = SMS_READ_MESSAGE;
        SKY_SetPaceMakerTimer(CLOCK_PACEMAKER_REPEAT_TIME, TRUE);//LONG lIntervalMS, BOOL fRepeat);

        break;
    case SMS_ICON_NO_MESSAGE:
        nSMS = SMS_NO_MESSAGE;
        SKY_SetPaceMakerTimer(CLOCK_PACEMAKER_REPEAT_TIME, TRUE);//LONG lIntervalMS, BOOL fRepeat);

        break;
    default: // error
        SKY_SetPaceMakerTimer(CLOCK_PACEMAKER_REPEAT_TIME, TRUE);//LONG lIntervalMS, BOOL fRepeat);
        return FALSE;
    }

    if (l_nLastSMS != nSMS)
    {
        fRet = TRUE;
        l_nLastSMS = nSMS;
    }
    else
    {
        fRet = FALSE;
    }

    return fRet;
}

BOOL SKY_SetThenCompareBellVib(BELLVIB_ICON_E BellVibIcon)
{
//    UINT8 nBellVib;
    BOOL fRet;

/*    switch (BellVibIcon)
    {
    case BELLVIB_ICON_BELL_AND_VIB:
        nBellVib = BELLVIB_BELLVIB;
        break;
    case BELLVIB_ICON_VIB_AFTER_BELL:
        nBellVib = BELLVIB_BELLVIB;
        break;
    case BELLVIB_ICON_BELL_ONLY:
        nBellVib = BELLVIB_BELL;
        break;
    case BELLVIB_ICON_VIB_ONLY:
        nBellVib = BELLVIB_VIB;
        break;
    case BELLVIB_ICON_ETIQUETTE:
        nBellVib = BELLVIB_VIB;
        break;
    case BELLVIB_ICON_NO_SOUND_NO_VIB:
        nBellVib = BELLVIB_NONE;
        break;
    default: // error
        return FALSE;
    }

    if (l_nLastBellVib != nBellVib)
    {
        fRet = TRUE;
        l_nLastBellVib = nBellVib;
    }
    else
    {
        fRet = FALSE;
    }*/

    if (l_LastBellVib != BellVibIcon)
    {
        fRet = TRUE;
        l_LastBellVib = BellVibIcon;
    }
    else
    {
        fRet = FALSE;
    }


    return fRet;
}


// ui_display_mel_vib_icon()

void SKY_DisplayBellVibIcon(void)
{
    if(SKY_IS_ETIQUETTE())
    {
        SKY_SetThenCompareBellVib(BELLVIB_ICON_ETIQUETTE);
    }
    else if(SKY_IS_AUTO_ANSWER())
    {
        if(SKY_GET_AUTO_ANSWER_BELL_TIME() == AAB_VIB_ONCE)
            SKY_SetThenCompareBellVib(BELLVIB_ICON_VIB_ONLY);
        else if(SKY_GET_AUTO_ANSWER_BELL_TIME() == AAB_IMMEDIATE)
            SKY_SetThenCompareBellVib(BELLVIB_ICON_NO_SOUND_NO_VIB);
        else if(SKY_GET_BELL_VOLUME() > 0)
            SKY_SetThenCompareBellVib(BELLVIB_ICON_BELL_ONLY);
        else
            SKY_SetThenCompareBellVib(BELLVIB_ICON_NO_SOUND_NO_VIB);
    }
    else
    {
        switch(SKY_GET_BELL_VIBRATION()){
        case BV_BELL:
            if(SKY_GET_BELL_VOLUME() > 0)
                SKY_SetThenCompareBellVib(BELLVIB_ICON_BELL_ONLY);
            else
                SKY_SetThenCompareBellVib(BELLVIB_ICON_NO_SOUND_NO_VIB);
            break;

        case BV_VIBRATION:
            SKY_SetThenCompareBellVib(BELLVIB_ICON_VIB_ONLY);
            break;

        case BV_BELL_AFTER_VIBRATION:
            if(SKY_GET_BELL_VOLUME() > 0)
                SKY_SetThenCompareBellVib(BELLVIB_ICON_VIB_AFTER_BELL);
            else
                SKY_SetThenCompareBellVib(BELLVIB_ICON_VIB_ONLY);
            break;

        case BV_BOTH_BELL_VIBRATION:
            if(SKY_GET_BELL_VOLUME() > 0)
                SKY_SetThenCompareBellVib(BELLVIB_ICON_BELL_AND_VIB);
            else
                SKY_SetThenCompareBellVib(BELLVIB_ICON_VIB_ONLY);
            break;
        }
    }
}

#define UI_CMPH_RSSI_NO_SIGNAL 125 // Should be same to CMPH_RSSI_NO_SIGNAL 

PROTOCOL_REVISION_E SKY_API SKY_GET_PROTOCOL_REV(void)
{
    extern word ds_socfg_get_p_rev_index(void);

    if(SKY_GET_PHONE_INFO_RSSI() == UI_CMPH_RSSI_NO_SIGNAL)
        return PR_IS95C;

    if(ds_socfg_get_p_rev_index() == 0)
        return PR_IS95A;
    else if(ds_socfg_get_p_rev_index() == 1)
        return PR_IS95B;
    else 
        return PR_IS95C;
}

BOOL SKY_SetThenCompareProtocolRevision(PROTOCOL_REVISION_E Protocol)
{
    BOOL fReturn = FALSE;

    if (l_LastProtocol != Protocol)
    {
        fReturn = TRUE;
        l_LastProtocol = Protocol;
    }

    return fReturn;
}

#ifdef SW_RESET_ENABLE
void SKY_SaveSWResetData(void)
{
    SAVE_SW_RESET_T *saveValue;

    saveValue = (SAVE_SW_RESET_T *)NV_SW_RESET_CHECK_POS - 12;

    saveValue->fAutoLock = SKY_GET_AUTO_LOCK_MODE();	
    saveValue->dwCTime = SKY_GetLocalTime();

    saveValue->BellVib = l_LastBellVib;
    saveValue->nBars = l_nLastBars;
    saveValue->nSmartBar = l_nLastSmartBars;
    saveValue->Protocol = l_LastProtocol;
    saveValue->SMS = l_nLastSMS;
    saveValue->RSSIBar = l_nLastRSSIBars;
}

void SKY_RestoreSWResetData(void)
{
    SAVE_SW_RESET_T *saveValue;

    saveValue = (SAVE_SW_RESET_T *)NV_SW_RESET_CHECK_POS - 12;

    SKY_SET_AUTO_LOCK_MODE(saveValue->fAutoLock);
    g_savedTime = saveValue->dwCTime;

    l_LastBellVib = saveValue->BellVib;
    l_nLastBars = saveValue->nBars;
    l_nLastSmartBars = saveValue->nSmartBar;
    l_LastProtocol = saveValue->Protocol;
    l_nLastSMS = saveValue->SMS;
    l_nLastRSSIBars = saveValue->RSSIBar;

    if(l_nLastRSSIBars)
        g_fNoService = FALSE;
}
#endif