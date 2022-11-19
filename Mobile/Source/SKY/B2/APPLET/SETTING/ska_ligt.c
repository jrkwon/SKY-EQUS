/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_ligt.c
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
** 4333-12-20 white     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include <stdio.h>
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/*#define LCD_CONTRAST_1  0
#define LCD_CONTRAST_2  1
#define LCD_CONTRAST_3  2
#define LCD_CONTRAST_4  3
#define LCD_CONTRAST_5  4
#define LCD_CONTRAST_6  5
#define LCD_CONTRAST_7  6
#define LCD_CONTRAST_8  7
#define LCD_CONTRAST_9  8*/

// Temp for B1 Project
/*#if (MODEL_ID==MODEL_B2)
#define HS_LCD_CONTRAST_MIN     0x5c
#define HS_LCD_CONTRAST_LLL     0x60
#define HS_LCD_CONTRAST_LL      0x63
#define HS_LCD_CONTRAST_L       0x66
#define HS_LCD_CONTRAST_D       0x69
#define HS_LCD_CONTRAST_H       0x6c
#define HS_LCD_CONTRAST_HH      0x70
#define HS_LCD_CONTRAST_HHH     0x73
#define HS_LCD_CONTRAST_MAX     0x76
#endif  // B2*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

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
extern BITMAP* g_AniBatt32x32x256C;
BYTE ContrastLevel = (LCD_CONTRAST_DEFAULT&0x1F);
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
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
//#define PROGRESS_BAR_WIDTH      118
//#define PROGRESS_BAR_HEIGHT      15
enum tag_LCD_CONTRAST_E {
    LC_LCD_TEXT,
    LC_LCD_PBAR,
    LC_IDLCD_TEXT,
    LC_IDLCD_PBAR,
    LC_MAX
};
BYTE abContrast[HCL_MAX+1];
void SKY_API SKA_MainBright(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;
    HCONTROL    hControl;
    //UINT8 sContrst;
    HS_CONTRAST_E Contrast;
    STATIC BOOL s_fEndToRoot = FALSE;
    INT8    nCtrl;
//	BYTE abContrast[HCL_MAX+1];
	
	/* initialize contrast value */
	SKY_GetNV( NV_LCD_I, (nv_item_type *)&(abContrast[HCL_D]));

	abContrast[HCL_MIN] = abContrast[HCL_D]-LCD_CONTAST_STEP*4;
    abContrast[HCL_LLL] = abContrast[HCL_D]-LCD_CONTAST_STEP*3;
    abContrast[HCL_LL]	= abContrast[HCL_D]-LCD_CONTAST_STEP*2;
    abContrast[HCL_L]	= abContrast[HCL_D]-LCD_CONTAST_STEP;
    //abContrast[HCL_D] = ;
    abContrast[HCL_H]	= abContrast[HCL_D]+LCD_CONTAST_STEP;
    abContrast[HCL_HH]  = abContrast[HCL_D]+LCD_CONTAST_STEP*2;
    abContrast[HCL_HHH] = abContrast[HCL_D]+LCD_CONTAST_STEP*3;
    abContrast[HCL_MAX] = abContrast[HCL_D]+LCD_CONTAST_STEP*4;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_LCD_BRIGHTNESS), 
            WA_SHOW_HEADER|WA_SHOW_CML);

        EQC_StaticText(EQS_NA, 
		           EQS_NA, 
				   (EQS_TOKEN)TKN_MAINLCD_TITLE);
        Contrast = (HS_CONTRAST_E)SKY_GET_CONTRAST();
        hControl = EQC_ProgressBar(EQS_NA, EQS_NA, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT, 
                                         HCL_MIN, HCL_MAX, Contrast, FALSE);

        EQC_StaticText(EQS_NA, 
		           EQS_NA, 
				   (EQS_TOKEN)TKN_SUBLCD_TITLE);
        
        Contrast = (HS_CONTRAST_E)SKY_GET_IDCONTRAST();
        hControl = EQC_ProgressBar(EQS_NA, EQS_NA,PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT, 
                                         HCL_MIN, HCL_MAX, Contrast, FALSE);
        
        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < LC_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        EQS_EndFormatter();
        //EQC_SET_STYLE(hControl,CS_PROGRESS_BAR_ALONE);
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }
        break;
    
    case SE_IO_PROGRESS_BAR_CANCEL:
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
        if(SKY_GET_CONTRAST() != (HS_CONTRAST_E) EQC_GetProgressBar(EQS_GetNthIOC(LC_LCD_PBAR)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
*/
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
        s_fEndToRoot = TRUE;
        if(SKY_GET_CONTRAST() != (HS_CONTRAST_E) EQC_GetProgressBar(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
*/
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;
    case SE_IO_PROGRESS_BAR_UP :
    case SE_IO_PROGRESS_BAR_DOWN :	                                                                                                                                                                                                                                                                                                                                         		
		if(pEvent->wParam == EQS_GetNthIOC(LC_LCD_PBAR))
        {
			Contrast = (HS_CONTRAST_E) EQC_GetProgressBar(EQS_GetNthIOC(LC_LCD_PBAR));
			//SKY_SetDisplayContrast((HS_CONTRAST_E)Contrast,ContrastLevel); // use CONTRAST_LEVEL_E type */
			if(Contrast >= HCL_MIN && Contrast <= HCL_MAX)
				SKY_SetLCDContrastTest(abContrast[(int)Contrast]);
			else
				SKY_SetLCDContrastTest(LCD_CONTRAST_DEFAULT);
        }
		
		else if (pEvent->wParam == EQS_GetNthIOC(LC_IDLCD_PBAR))
        {
            Contrast = (HS_CONTRAST_E) EQC_GetProgressBar(EQS_GetNthIOC(LC_IDLCD_PBAR));
            SKY_SetSubDisplayContrast((HS_CONTRAST_E)Contrast,SubContrastLevel); // use CONTRAST_LEVEL_E type */
        }

        break;
        
    case SE_SOFTKEY_SAVE:
    //case SE_IO_PROGRESS_BAR_SELECT://SE_SOFTKEY_SAVE:
        Contrast = (HS_CONTRAST_E) EQC_GetProgressBar(EQS_GetNthIOC(LC_LCD_PBAR));
        SKY_SET_CONTRAST(Contrast);

        Contrast = (HS_CONTRAST_E) EQC_GetProgressBar(EQS_GetNthIOC(LC_IDLCD_PBAR));
        SKY_SET_IDCONTRAST(Contrast);

//        SKY_SetDisplayContrast(Contrast); // use CONTRAST_LEVEL_E type */
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_APP_END :   /* The very last event */ 
        SKY_SetDisplayContrast((HS_CONTRAST_E)SKY_GET_CONTRAST(), abContrast[HCL_D]);
        SKY_SetSubDisplayContrast((HS_CONTRAST_E)SKY_GET_IDCONTRAST(), SubContrastLevel);
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


enum tag_SMART_BATTERY_E {
//    SB_SET_TITLE,
    SB_CHBX_BUTTON = 0,
//    SB_INFO_TITLE,
    SB_INFO_IDLE_STRING,
    SB_INFO_CALL_STRING,
    SB_INFO_VOLUME,
    SB_PBAR,
    SB_MAX
};
	
#define BATTERY_INFO_DISPLAY_X	1
#define BATTERY_INFO_DISPLAY_Y	1
#define BATTERY_INFO_DY         16
#define BATTERY_INFO_PBAR_X     30
#define BATTERY_INFO_PBAR_Y     36
#define BATTERY_INFO_PBAR_WIDTH 78
#define BATTERY_INFO_PBAR_HEIGHT    10
#define BATTERY_INFO_PBAR_MIN       0
#define BATTERY_INFO_PBAR_MAX       200

#ifdef FEATURE_SMART_BATTERY
void SKY_API SKA_MainBatteryInfo(SKY_EVENT_T *pEvent) 
{
    CHAR szBuf[32];
    FLOAT fMinutes, fPercent;
    UINT8 nDy;
    BOOL  fValue;
    STATIC HCONTROL s_hControl;
    WORD   wCurrentProgress;
    CONST BYTE* sHow2Info[1];
    STATIC HCONTROL s_hMsgControl, s_hNotApply, s_hIdleString, s_hCallString, s_hVolume, s_hPbar;
    STATIC BOOL s_fEndToRoot = FALSE, s_fBettryInfoDisp = FALSE;

    sHow2Info[0] = (BYTE*)SKY_GET_TOKEN(TKN_BATT_ON);
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_SMART_BATTERY_INFO), 
            WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_CheckBox(6, 3, (BYTE **)sHow2Info, 1);
        EQS_SetIOCActive(EQS_GetNthIOC(SB_CHBX_BUTTON));

        if(SKY_IS_SMART_BATTERY())
            EQC_SetCheckBox(EQS_GetNthIOC(SB_CHBX_BUTTON), 0);
        //EQC_RadioButton(BATTERY_INFO_DISPLAY_X, BATTERY_INFO_DISPLAY_Y,          //설정 라디오 버튼
        //    6, 2, (BYTE **)sHow2Info, (UINT8)SKY_IS_SMART_BATTERY());
        nDy = EQC_GET_DY(EQS_GetNthIOC(SB_CHBX_BUTTON));

        if(SKY_IS_SMART_BATTERY())     //스마트 배터리 설정
        {
            if (smb_is_operating() == BQ2018_RAM_OK_S)
            {
                s_fBettryInfoDisp = TRUE;
                nDy += 10;
                s_hIdleString = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_IDLE);       //대기시간 스트링
                nDy += EQC_GET_DY(EQS_GetNthIOC(SB_INFO_IDLE_STRING));

                s_hCallString = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_CALL);       //통화시간 스트링
                nDy += EQC_GET_DY(EQS_GetNthIOC(SB_INFO_CALL_STRING));

                s_hVolume = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_VOLUME);     //용량 스트링

                wCurrentProgress = 0;
                s_hPbar = EQC_ProgressBar(BATTERY_INFO_PBAR_X,         //용량 바
                                          nDy + 2, 
                                          PROGRESS_BAR_WIDTH,
                                          PROGRESS_BAR_HEIGHT,
                                          BATTERY_INFO_PBAR_MIN, 
                                          BATTERY_INFO_PBAR_MAX, 
                                          wCurrentProgress,
                                          TRUE);

                EQC_SetProgressBarStep(s_hPbar, VM_PBAR_STEPSIZE);
                EQC_SET_FOCUS(s_hPbar, FALSE); /* 포커스를 갖지 못하도록 하는 매크로 */ 
                
                fMinutes = smb_idle_state_time();
                fMinutes = (fMinutes >= 0.0) ? fMinutes : 0.0;
                sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_IDLE), 
                    (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                EQC_SetStaticText(s_hIdleString,szBuf);
        
                fMinutes = smb_talk_state_time();
                fMinutes = (fMinutes    >= 0.0) ? fMinutes    : 0.0;
                sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_CALL),
                    (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                EQC_SetStaticText(s_hCallString,szBuf);
        
                fPercent = (UINT8)smb_percent_capacity();
                fPercent    = (fPercent    >= 0.0) ? fPercent    : 0.0;

                wCurrentProgress = fPercent * 2;
                EQC_SetProgressBar(s_hPbar, wCurrentProgress);   
            }
            else
            {
                s_hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_BATT_NO_INFO), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);            
            }
        }else
        {
           s_hNotApply = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_BATT_NOT_APPLY), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);  
        }
        EQS_StartTimer(EQS_TIMER_1, 30000, TRUE);
        break;

    case SE_APP_END :   /* The very last event */ 
        s_fBettryInfoDisp = FALSE;
        EQS_ReleaseDC();
        break;

    case EQE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(SB_CHBX_BUTTON));
        break;

    case SE_TM_TIMER1:
        if(SKY_IS_SMART_BATTERY())     //스마트 배터리 설정
        {
            if (smb_is_operating() == BQ2018_RAM_OK_S)
            {
                if(s_fBettryInfoDisp)
                {
                    fMinutes = smb_idle_state_time();
                    fMinutes = (fMinutes >= 0.0) ? fMinutes : 0.0;
                    sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_IDLE), 
                        (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                    EQC_SetStaticText(EQS_GetNthIOC(SB_INFO_IDLE_STRING),szBuf);
        
                    fMinutes = smb_talk_state_time();
                    fMinutes = (fMinutes    >= 0.0) ? fMinutes    : 0.0;
                    sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_CALL), (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                    EQC_SetStaticText(EQS_GetNthIOC(SB_INFO_CALL_STRING),szBuf);
        
                    fPercent = (UINT8)smb_percent_capacity();
                    fPercent    = (fPercent    >= 0.0) ? fPercent    : 0.0;

                    wCurrentProgress = fPercent * 2;
                    EQC_SetProgressBar(EQS_GetNthIOC(SB_PBAR), wCurrentProgress);   
                    EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
                    if(fPercent < 3.0 )
                    {
                        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_BATT_NEED_CHARGE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    }
                }else
                {
 
                    s_fBettryInfoDisp = TRUE;
                    nDy = EQC_GET_DY(EQS_GetNthIOC(SB_CHBX_BUTTON));
                    nDy += 10;
                    s_hIdleString = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_IDLE);       //대기시간 스트링
                    nDy += EQC_GET_DY(EQS_GetNthIOC(SB_INFO_IDLE_STRING));

                    s_hCallString = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_CALL);       //통화시간 스트링
                    nDy += EQC_GET_DY(EQS_GetNthIOC(SB_INFO_CALL_STRING));

                    s_hVolume = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_VOLUME);     //용량 스트링

                    wCurrentProgress = 0;
                    s_hPbar = EQC_ProgressBar(BATTERY_INFO_PBAR_X,         //용량 바
                                              nDy + 2, 
                                              PROGRESS_BAR_WIDTH,
                                              PROGRESS_BAR_HEIGHT,
                                              BATTERY_INFO_PBAR_MIN, 
                                              BATTERY_INFO_PBAR_MAX, 
                                              wCurrentProgress,
                                              TRUE);

                    EQC_SetProgressBarStep(s_hPbar, VM_PBAR_STEPSIZE);
                    EQC_SET_FOCUS(s_hPbar, FALSE); /* 포커스를 갖지 못하도록 하는 매크로 */ 

                    fMinutes = smb_idle_state_time();
                    fMinutes = (fMinutes >= 0.0) ? fMinutes : 0.0;
                    sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_IDLE), 
                        (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                    EQC_SetStaticText(s_hIdleString,szBuf);
        
                    fMinutes = smb_talk_state_time();
                    fMinutes = (fMinutes    >= 0.0) ? fMinutes    : 0.0;
                    sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_CALL),
                        (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                    EQC_SetStaticText(s_hCallString,szBuf);
        
                    fPercent = (UINT8)smb_percent_capacity();
                    fPercent    = (fPercent    >= 0.0) ? fPercent    : 0.0;

                    wCurrentProgress = fPercent * 2;
                    EQC_SetProgressBar(s_hPbar, wCurrentProgress);                   
                    EQS_SEND_EVENT(SE_APP_DRAW,EQS_DRAW_LEVEL_ALL, 0 );                
                }
            }else
            {
                s_hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_BATT_NO_INFO), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);            
            }
        }
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(SB_CHBX_BUTTON));
        }
        break;

    case SE_IO_CHECK_BOX_CHECK:
    case SE_IO_CHECK_BOX_UNCHECK:
        fValue = EQC_GetCheckBox(EQS_GetNthIOC(SB_CHBX_BUTTON),0);
        //SKY_SET_SMART_BATTERY(fValue);
        if(fValue)
        {
            if (smb_is_operating() == BQ2018_RAM_OK_S)
            {
                if(s_fBettryInfoDisp)
                {
                    EQC_SetCheckBox(EQS_GetNthIOC(SB_CHBX_BUTTON), 0);
                    fMinutes = smb_idle_state_time();
                    fMinutes = (fMinutes >= 0.0) ? fMinutes : 0.0;
                    sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_IDLE), 
                        (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                    EQC_SetStaticText(EQS_GetNthIOC(SB_INFO_IDLE_STRING),szBuf);
    
                    fMinutes = smb_talk_state_time();
                    fMinutes = (fMinutes    >= 0.0) ? fMinutes    : 0.0;
                    sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_CALL),
                        (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                    EQC_SetStaticText(EQS_GetNthIOC(SB_INFO_CALL_STRING),szBuf);
    
                    fPercent = (UINT8)smb_percent_capacity();
                    fPercent    = (fPercent    >= 0.0) ? fPercent    : 0.0;

                    wCurrentProgress = fPercent * 2;
                    EQC_SetProgressBar(EQS_GetNthIOC(SB_PBAR), wCurrentProgress);   
                }else
                {
                    s_fBettryInfoDisp = TRUE;
                    nDy = EQC_GET_DY(EQS_GetNthIOC(SB_CHBX_BUTTON));
                    nDy += 10;
                    s_hIdleString = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_IDLE);       //대기시간 스트링
                    nDy += EQC_GET_DY(EQS_GetNthIOC(SB_INFO_IDLE_STRING));

                    s_hCallString = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_CALL);       //통화시간 스트링
                    nDy += EQC_GET_DY(EQS_GetNthIOC(SB_INFO_CALL_STRING));

                    s_hVolume = EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_VOLUME);     //용량 스트링

                    wCurrentProgress = 0;
                    s_hPbar = EQC_ProgressBar(BATTERY_INFO_PBAR_X,         //용량 바
                                              nDy + 2, 
                                              PROGRESS_BAR_WIDTH,
                                              PROGRESS_BAR_HEIGHT,
                                              BATTERY_INFO_PBAR_MIN, 
                                              BATTERY_INFO_PBAR_MAX, 
                                              wCurrentProgress,
                                              TRUE);

                    EQC_SetProgressBarStep(s_hPbar, VM_PBAR_STEPSIZE);
                    EQC_SET_FOCUS(s_hPbar, FALSE); /* 포커스를 갖지 못하도록 하는 매크로 */ 

                    fMinutes = smb_idle_state_time();
                    fMinutes = (fMinutes >= 0.0) ? fMinutes : 0.0;
                    sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_IDLE), 
                        (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                    EQC_SetStaticText(s_hIdleString,szBuf);
        
                    fMinutes = smb_talk_state_time();
                    fMinutes = (fMinutes    >= 0.0) ? fMinutes    : 0.0;
                    sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_CALL),
                        (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                    EQC_SetStaticText(s_hCallString,szBuf);
        
                    fPercent = (UINT8)smb_percent_capacity();
                    fPercent    = (fPercent    >= 0.0) ? fPercent    : 0.0;

                    wCurrentProgress = fPercent * 2;
                    EQC_SetProgressBar(s_hPbar, wCurrentProgress);                   
                    EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
                }
            }
            else
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_BATT_NO_INFO), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);            
            }
        }else
        {
            if (s_fBettryInfoDisp)
            {
                s_fBettryInfoDisp = FALSE;
                EQS_DeleteControl(s_hIdleString);
                EQS_DeleteControl(s_hCallString);
                EQS_DeleteControl(s_hVolume);
                EQS_DeleteControl(s_hPbar);
            }
        }
        //EQS_START_SIBLING_EVENT(SKA_MainBatteryInfo, SE_APP_START, CAMERA_TURN, EQS_NA, FALSE);
        //(SKA_MainBatteryInfo ,FALSE);
        break;

    case SE_IO_CHECK_BOX_UP:
    case SE_IO_CHECK_BOX_DOWN:
        EQS_SetIOCActive(EQS_GetNthIOC(SB_CHBX_BUTTON));
        break;
        
    case SE_SOFTKEY_SAVE:
        SKY_SET_SMART_BATTERY(EQC_GetCheckBox(EQS_GetNthIOC(SB_CHBX_BUTTON), 0));
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
     
    case SE_IO_CHECK_BOX_CANCEL:
    case SE_KEY_CLEAR:
        if(SKY_IS_SMART_BATTERY() != EQC_GetCheckBox(EQS_GetNthIOC(SB_CHBX_BUTTON), 0))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(SKY_IS_SMART_BATTERY() != EQC_GetCheckBox(EQS_GetNthIOC(SB_CHBX_BUTTON) , 0))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case EQE_APP_DRAW:
        break;

    case SE_KEY_0:
        EQS_START_CHILD(SKA_BatteryTest);
        break;

    case SE_KEY_1:
        EQS_START_CHILD(SKA_ViewSMB);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_ViewSMB()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
enum tag_VIEW_BATTERY_E {
    VB_INFO_IDLE_STRING = 0,
    VB_INFO_CALL_STRING,
    VB_INFO_VOLUME,
    VB_PBAR,
    VB_IMAGE,
    VB_MAX
};
	
void SKY_API SKA_ViewSMB(SKY_EVENT_T *pEvent)
{
    INT8  nDx, nDy;
    WORD wCurrentProgress;
    HCONTROL hControl;
    FLOAT   fMinutes, fPercent;
    CHAR szBuf[32];
    
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_SMART_BATTERY_INFO), 
            WA_SHOW_HEADER);

        if(SKY_IS_SMART_BATTERY())     //스마트 배터리 설정
        {
            if (smb_is_operating() == BQ2018_RAM_OK_S)
            {
                nDy = 5;
                EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_IDLE);       //대기시간 스트링
                nDy += EQC_GET_DY(EQS_GetNthIOC(VB_INFO_IDLE_STRING));
                nDy += 5;

                EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_CALL);       //통화시간 스트링
                nDy += EQC_GET_DY(EQS_GetNthIOC(VB_INFO_CALL_STRING));
                nDy += 5;

                EQC_StaticText(EQS_NA, nDy, TKN_BATT_INFO_VOLUME);     //용량 스트링
                nDx = EQC_GET_DX(EQS_GetNthIOC(VB_INFO_VOLUME));

                
                wCurrentProgress = 0;
                hControl = EQC_ProgressBar(nDx + 5,         //용량 바
                              nDy + 2, 
                              PROGRESS_BAR_WIDTH,
                              PROGRESS_BAR_HEIGHT,
                              BATTERY_INFO_PBAR_MIN, 
                              BATTERY_INFO_PBAR_MAX, 
                              wCurrentProgress,
                              TRUE);

                EQC_SetProgressBarStep(hControl, VM_PBAR_STEPSIZE);
                EQC_SET_FOCUS(hControl, FALSE); /* 포커스를 갖지 못하도록 하는 매크로 */ 

                nDy += EQC_GET_DY(EQS_GetNthIOC(VB_INFO_VOLUME));
                nDy += 5;
                EQC_Animation(
		               53, 
		               nDy, 
			           (BITMAP*)&g_AniBatt32x32x256C, NULL
			          ); 
                
                fMinutes = smb_idle_state_time();
                fMinutes = (fMinutes >= 0.0) ? fMinutes : 0.0;
                sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_IDLE), 
                    (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                EQC_SetStaticText(EQS_GetNthIOC(VB_INFO_IDLE_STRING),szBuf);
        
                fMinutes = smb_talk_state_time();
                fMinutes = (fMinutes    >= 0.0) ? fMinutes    : 0.0;
                sprintf(szBuf, (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_BATT_INFO_CALL),
                    (INT)fMinutes, (INT)(fMinutes * 60.0) % 60);
                EQC_SetStaticText(EQS_GetNthIOC(VB_INFO_CALL_STRING),szBuf);
        
                fPercent = (UINT8)smb_percent_capacity();
                fPercent    = (fPercent    >= 0.0) ? fPercent    : 0.0;

                wCurrentProgress = fPercent * 2;
                EQC_SetProgressBar(hControl, wCurrentProgress);   

            }
            else
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_BATT_NO_INFO), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);            
            }
        }else
        {
           EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_BATT_NOT_APPLY), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);  
        }
        
        EQS_StartTimer(EQS_TIMER_1, 5000, FALSE);
        break;

    case SE_TM_TIMER1:
        EQS_End();
        break;

    case SE_IO_MESGBOX_END:
        EQS_End();
        break; 

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_BatteryTest()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL BYTE *ppBattery[6];
BYTE *pSMBText;

CONST SKY_TOKEN aSmartBatteryTest[6] =
{
    TKN_LEAVE_VOLUME,
    TKN_IDLE_VOLUME,
    TKN_CALL_VOLUME,
    TKN_MEMORY_DUMP,
    TKN_MEMORY_EDIT,
    TKN_BATTERY_INIT
};

void SKY_API SKA_BatteryTest(SKY_EVENT_T *pEvent)
{
    INT8    Index;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
            
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_SMART_BATTERY_INFO), 
            WA_SHOW_HEADER);
        
        for(Index=0; Index < 6 ; Index++)
            ppBattery[Index] = (BYTE *)(BYTE*)SKY_GET_TOKEN(aSmartBatteryTest[Index]);

        pSMBText = (BYTE *)SKY_Malloc(72);
        
        EQC_ScrollList(1, 1, 16, 3, ppBattery, 6, 6, 0);
        EQC_TextEdit(0, 50, 72, 4, TEM_KOREAN, 72, "",0);
        EQC_SET_STYLE(EQS_GetNthIOC(1), CS_TEXT_EDIT_READONLY | CS_TEXT_EDIT_BORDER);
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
              
    case SE_IO_SCROLL_LIST_SELECT:
        Index = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        switch(Index)
        {
        case 0:
            STRCPY(pSMBText, "잔여소량");
            EQC_SetTextEditData(EQS_GetNthIOC(1),pSMBText);
            break;

        case 1:
            STRCPY(pSMBText, "대기소모량");
            EQC_SetTextEditData(EQS_GetNthIOC(1),pSMBText);
            break;

        case 2:
            STRCPY(pSMBText, "통화소모량");
            EQC_SetTextEditData(EQS_GetNthIOC(1),pSMBText);
            break;

        case 3:
            STRCPY(pSMBText, "메모리텀프");
            EQC_SetTextEditData(EQS_GetNthIOC(1),pSMBText);
            break;

        case 4:
            STRCPY(pSMBText, "메모리편집");
            EQC_SetTextEditData(EQS_GetNthIOC(1),pSMBText);
            break;

        case 5:
            STRCPY(pSMBText, "배터리초기화");
            EQC_SetTextEditData(EQS_GetNthIOC(1),pSMBText);
            break;

        default:
            break;
        }
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        SKY_Free(pSMBText);
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


#endif