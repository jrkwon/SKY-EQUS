/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_add.c
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
** 4334-01-03 white     Created.
**
** ===========================================================================
*/
 
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define FULL_GROUP_CONTROL          //2001.05.13
#define ADVANCED_BACKLIGHT_CONTROL      //2001.06.05
#define SCROLL_LIST_WIDTH       16
#define H_SIZE               12
#define NUM_SCROLLLIST_SIZE  2
#define SELECT_YES           0
#define SELECT_NO            1

#define SIC_SCROLL_LIST_STYLE   CS_SCROLL_LIST_BORDER| \
                                CS_SCROLL_LIST_COMBO | \
                                CS_SCROLL_LIST_ENUMBERATED// | \
                                //CS_SCROLL_LIST_WRAP

#define SGC_SCROLL_LIST_SIZE    3

#define SGC_GET_SLSIZE(n)       (UINT8)((SGC_SCROLL_LIST_SIZE < n)?SGC_SCROLL_LIST_SIZE:n)
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
#if 0
enum tag_AUTO_AREA_E {
    AA_TITLE = 0,
    AA_NUMBER,
    AA_SELECT,
    AA_MAX
};

enum tag_SKA_AUTOLIGHT_E { 
    SA_SELECT_LIGHT = 0,
    SA_TIME_LIGHT,
    SA_MAX
};
#endif

enum tag_AUTORECEIPT_E
{
  AS_HANDSFREE = 0,   //핸즈프리
  AS_HANDSFREELIST,   //스크롤리스트
  AS_EARPHONEMIC,     //이어폰마이크
  AS_EARPHONEMICLIST, //스크롤리스트
  AS_MAX                 
};  

enum tag_SKA_IDLCDCOLOR_E { 
    SIC_IDLE_LABEL = 0,
    SIC_IDLE_SCRL,
    SIC_SMS_LABEL,
    SIC_SMS_SCRL,
    SIC_ALARM_LABEL,
    SIC_ALARM_SCRL,
    SIC_ALERT_LABEL,
    SIC_ALERT_SCRL,
    //SIC_GROUP_ALERT_LABEL,
    SIC_MAX
};
  
enum tag_SKA_GROUP_COLOR_E {
    SGC_GROUP_TITLE = 0,
    SGC_GROUP_SCRL,
    SGC_ALERT_TITLE,
    SGC_ALERT_SCRL,
    SGC_MAX
};    
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
CONST SKY_TOKEN aColorSelect[NUM_COLOR] =
{
    TKN_BLUE,
    TKN_GREEN,
    TKN_RED,
    TKN_VIOLET,
    TKN_BLUE_GREEN,
//    TKN_ORANGE,
//    TKN_WHITE,
    TKN_NOTUSED
};

CONST SKY_TOKEN aColorSelectAlert[NUM_COLOR+1] =
{
    TKN_RAINBOW,
    TKN_BLUE,
    TKN_GREEN,
    TKN_RED,
    TKN_VIOLET,
    TKN_BLUE_GREEN,
//    TKN_ORANGE,
//    TKN_WHITE,
    TKN_NOTUSED
};

CONST SKY_TOKEN aColorGroupSelectAlert[NUM_COLOR+1] =
{
    TKN_RAINBOW,
    TKN_BLUE,
    TKN_GREEN,
    TKN_RED,
    TKN_VIOLET,
    TKN_BLUE_GREEN,
//    TKN_ORANGE,
//    TKN_WHITE,
    TKN_GROUP_LIGHT_NONE
};

CONST BYTE* bpBacklight[4];
CONST BYTE* bpIDBacklight[4];

LOCAL BYTE *ppIdLcdColor[NUM_COLOR];            //조명색설정
LOCAL BYTE *ppIdLcdColorAlert[NUM_COLOR+1];     //착신시 조명색설정
#ifndef FULL_GROUP_CONTROL
LOCAL BYTE *ppGroupList[MAX_GROUP_NUMBER];      // 전화부그룹
#endif
#ifdef ADVANCED_BACKLIGHT_CONTROL
enum tag_SKA_BACKLIGHT_NONE_TIME_E {
    SBN_IN_TITLE = 0,
    SBN_IN_SELECT,
    SBN_OUT_TITLE,
    SBN_OUT_SELECT,
    SBN_MAX
};    

enum tag_SKA_BACKLIGHT_IN_TIME_E {
    SBI_IN_TITLE = 0,
    SBI_IN_SELECT,
    SBI_IN_STRT_TIME,
    SBI_IN_HYPEN,
    SBI_IN_END_TIME,
    SBI_OUT_TITLE,
    SBI_OUT_SELECT,
    SBI_MAX
};    

enum tag_SKA_BACKLIGHT_OUT_TIME_E {
    SBO_IN_TITLE = 0,
    SBO_IN_SELECT,
    SBO_OUT_TITLE,
    SBO_OUT_SELECT,
    SBO_OUT_STRT_TIME,
    SBO_OUT_HYPEN,
    SBO_OUT_END_TIME,
    SBO_MAX
};    

enum tag_SKA_BACKLIGHT_INOUT_TIME_E {
    SBIO_IN_TITLE = 0,
    SBIO_IN_SELECT,
    SBIO_IN_STRT_TIME,
    SBIO_IN_HYPEN,
    SBIO_IN_END_TIME,
    SBIO_OUT_TITLE,
    SBIO_OUT_SELECT,
    SBIO_OUT_STRT_TIME,
    SBIO_OUT_HYPEN,
    SBIO_OUT_END_TIME,
    SBIO_MAX
};    

enum tag_CASE_BACKLIGHT_INOUT_E {
    NOT_USED_TIME = 0,
    IN_USED_TIME,
    OUT_USED_TIME,
    IN_OUT_USED_TIME
};

typedef enum tag_CASE_BACKLIGHT_INOUT_E CASE_BACKLIGHT_INOUT_E;

enum tag_SKA_WHERE_FOCUS_E {
    SWF_UPPER_FOCUS = 0,
    SWF_DOWN_FOCUS
};
typedef enum tag_SKA_WHERE_FOCUS_E SKA_WHERE_FOCUS_E;

LOCAL CASE_BACKLIGHT_INOUT_E BacklightCase;
LOCAL DWORD l_dwInStrtTime, l_dwInEndTime, l_dwOutStrtTime, l_dwOutEndTime;
#endif //ADVANCED_BACKLIGHT_CONTROL   

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

void EQS_API sAddColorSelControls(void);
void EQS_API sAddGroupColorSelControls(void);

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

#ifdef ADVANCED_BACKLIGHT_CONTROL
LOCAL BOOL SKY_API sConfirmControlChg(CASE_BACKLIGHT_INOUT_E CaseValue)
{
    AUTO_BACKLIGHT_E InValue=AB_NO, OutValue = AB_NO;
    DWORD   dwInStrtTime = 0, dwInEndTime = 0, dwOutStrtTime = 0, dwOutEndTime = 0;
    clk_julian_type cjSetTime;

    switch(CaseValue)
    {
    case NOT_USED_TIME:
        InValue = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_IN_SELECT));
        OutValue = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_OUT_SELECT));
        break;

    case IN_USED_TIME:
        InValue = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_IN_SELECT));
        OutValue = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_OUT_SELECT));
            
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBI_IN_STRT_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBI_IN_STRT_TIME));
        cjSetTime.second = 0;
        dwInStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBI_IN_END_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBI_IN_END_TIME));
        cjSetTime.second = 0;
        dwInEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        
        break;

    case OUT_USED_TIME:
        InValue = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_IN_SELECT));
        OutValue = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_OUT_SELECT));

        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBO_OUT_STRT_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBO_OUT_STRT_TIME));
        cjSetTime.second = 0;
        dwOutStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBO_OUT_END_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBO_OUT_END_TIME));
        cjSetTime.second = 0;
        dwOutEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        
        break;

    case IN_OUT_USED_TIME:
        InValue = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_IN_SELECT));
        OutValue = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_OUT_SELECT));

        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_IN_STRT_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_IN_STRT_TIME));
        cjSetTime.second = 0;
        dwInStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_IN_END_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_IN_END_TIME));
        cjSetTime.second = 0;
        dwInEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        
        
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_OUT_STRT_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_OUT_STRT_TIME));
        cjSetTime.second = 0;
        dwOutStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_OUT_END_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_OUT_END_TIME));
        cjSetTime.second = 0;
        dwOutEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        break;

    default:
        break;
    }

    if(InValue != SKY_GET_AUTO_BACKLIGHT() || OutValue != SKY_GET_AUTO_SUBBACKLIGHT())
        return FALSE;
    
    if(SKY_GET_AUTO_BACKLIGHT() == AB_TIME && InValue == AB_TIME)
    {
        if(SKY_GET_AUTO_BACKLIGHT_START_TIME() != dwInStrtTime || \
            SKY_GET_AUTO_BACKLIGHT_END_TIME() != dwInEndTime)
            return FALSE;
    }

    if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME && OutValue == AB_TIME)
    {
        if(SKY_GET_AUTO_SUBBACKLIGHT_START_TIME() != dwOutStrtTime || \
            SKY_GET_AUTO_SUBBACKLIGHT_END_TIME() != dwOutEndTime)
            return FALSE;
    }
    return TRUE;
}


LOCAL void SKY_API sSaveBacklightValue(CASE_BACKLIGHT_INOUT_E CaseValue)
{
    INT8    nValue;
    clk_julian_type cjSetTime;
    DWORD sTime;

    g_fSubBackLightOn = FALSE;
    g_fSubBackLightOff = FALSE;

    switch(CaseValue)
    {
    case NOT_USED_TIME:
        nValue = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_IN_SELECT));
        SKY_SET_AUTO_BACKLIGHT((AUTO_BACKLIGHT_E)nValue);
        if(nValue == AB_NO)
            SKY_BacklightOff();
        SKY_SET_AUTO_SUBBACKLIGHT((AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_OUT_SELECT)));
        break;

    case IN_USED_TIME:
        SKY_SET_AUTO_BACKLIGHT((AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_IN_SELECT)));
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBI_IN_STRT_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBI_IN_STRT_TIME));
        cjSetTime.second = 0;

        sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        SKY_SET_AUTO_BACKLIGHT_START_TIME(sTime);
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBI_IN_END_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBI_IN_END_TIME));
        cjSetTime.second = 0;
        sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        SKY_SET_AUTO_BACKLIGHT_END_TIME(sTime);

        SKY_SET_AUTO_SUBBACKLIGHT((AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_OUT_SELECT)));
        break;

    case OUT_USED_TIME:
        nValue = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_IN_SELECT));
        SKY_SET_AUTO_BACKLIGHT((AUTO_BACKLIGHT_E)nValue);
        if(nValue == AB_NO)
            SKY_BacklightOff();

        SKY_SET_AUTO_SUBBACKLIGHT((AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_OUT_SELECT)));
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBO_OUT_STRT_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBO_OUT_STRT_TIME));
        cjSetTime.second = 0;

        sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        SKY_SET_AUTO_SUBBACKLIGHT_START_TIME(sTime);
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBO_OUT_END_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBO_OUT_END_TIME));
        cjSetTime.second = 0;
        sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        SKY_SET_AUTO_SUBBACKLIGHT_END_TIME(sTime);

        break;

    case IN_OUT_USED_TIME:
        SKY_SET_AUTO_BACKLIGHT((AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_IN_SELECT)));
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_IN_STRT_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_IN_STRT_TIME));
        cjSetTime.second = 0;

        sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        SKY_SET_AUTO_BACKLIGHT_START_TIME(sTime);
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_IN_END_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_IN_END_TIME));
        cjSetTime.second = 0;
        sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        SKY_SET_AUTO_BACKLIGHT_END_TIME(sTime);

        SKY_SET_AUTO_SUBBACKLIGHT((AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_OUT_SELECT)));
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_OUT_STRT_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_OUT_STRT_TIME));
        cjSetTime.second = 0;

        sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        SKY_SET_AUTO_SUBBACKLIGHT_START_TIME(sTime);
        cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_OUT_END_TIME));
        cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_OUT_END_TIME));
        cjSetTime.second = 0;
        sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        SKY_SET_AUTO_SUBBACKLIGHT_END_TIME(sTime);
        g_fSubBackLightOn = FALSE;
        g_fSubBackLightOff = FALSE;
        break;

    default:
        break;
    }   
}

LOCAL void SKY_API sDeleteBacklightCtrl(CASE_BACKLIGHT_INOUT_E CaseValue)
{
    INT8    index = 0;

    switch(CaseValue)
    {
    case NOT_USED_TIME:
        for(index = 0; index< SBN_MAX; index++)
            EQS_DeleteControl(EQS_GetNthIOC(SBN_IN_TITLE));
        break;

    case IN_USED_TIME:
        for(index = 0; index< SBI_MAX; index++)
            EQS_DeleteControl(EQS_GetNthIOC(SBI_IN_TITLE));
        break;

    case OUT_USED_TIME:
        for(index = 0; index< SBO_MAX; index++)
            EQS_DeleteControl(EQS_GetNthIOC(SBO_IN_TITLE));
        break;

    case IN_OUT_USED_TIME:
        for(index = 0; index< SBIO_MAX; index++)
            EQS_DeleteControl(EQS_GetNthIOC(SBIO_IN_TITLE));
        break;

    default:
        break;
    }   
}

LOCAL void SKY_API sAddBacklightCtrl(AUTO_BACKLIGHT_E in, AUTO_BACKLIGHT_E out,SKA_WHERE_FOCUS_E where)
{
    INT8    index;

    EQS_SetCanvasSize(SKY_DEF_WIN_DX, 200);

    if(in != AB_TIME && out != AB_TIME)
    {
        EQC_StaticText(EQS_NA,  EQS_NA, TKN_AUTO_LCD_BACKLIGHT); 
        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, \
            3, bpBacklight, 4, 4, in), SIC_SCROLL_LIST_STYLE);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_AUTO_IDLCD_BACKLIGHT);
        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, \
            3, bpIDBacklight, 4, 4, out), SIC_SCROLL_LIST_STYLE);
        
        BacklightCase = NOT_USED_TIME;
        EQS_StartFormatter();

        for(index = 0; index< SBN_MAX; index++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(index), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        if(where == SWF_UPPER_FOCUS )
            EQS_SetIOCFocus(EQS_GetNthIOC(SBN_IN_SELECT));
        else
            EQS_SetIOCFocus(EQS_GetNthIOC(SBN_OUT_SELECT));

    }else if(in == AB_TIME && out != AB_TIME)
    {
        EQC_StaticText(EQS_NA,  EQS_NA, TKN_AUTO_LCD_BACKLIGHT); 
        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, \
                   3, bpBacklight, 4, 4, in), SIC_SCROLL_LIST_STYLE);
        
        EQC_Time(EQS_NA, EQS_NA, l_dwInStrtTime/60 , TIME_TIME_CONTROL ); 
        EQC_StaticText(EQS_NA , EQS_NA, TKN_SCHEDULE_START_END_HYPEN);
        EQC_Time(EQS_NA ,EQS_NA, l_dwInEndTime/60, TIME_TIME_CONTROL );

        EQC_StaticText(EQS_NA, EQS_NA, TKN_AUTO_IDLCD_BACKLIGHT);
        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, \
                   3, bpIDBacklight, 4, 4, out), SIC_SCROLL_LIST_STYLE);
        
        BacklightCase = IN_USED_TIME;
        EQS_StartFormatter();

        for(index = 0; index< SBI_MAX; index++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(index), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            if(index != SBI_IN_STRT_TIME && index != SBI_IN_HYPEN )
                EQS_NewLine();
        }

        EQS_EndFormatter();
        if(where == SWF_UPPER_FOCUS )
            EQS_SetIOCFocus(EQS_GetNthIOC(SBI_IN_SELECT));
        else
            EQS_SetIOCFocus(EQS_GetNthIOC(SBI_OUT_SELECT));
        
    }else if(in != AB_TIME && out == AB_TIME)
    {
        EQC_StaticText(EQS_NA,  EQS_NA, TKN_AUTO_LCD_BACKLIGHT); 
        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, \
                   3, bpBacklight, 4, 4, in), SIC_SCROLL_LIST_STYLE);
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_AUTO_IDLCD_BACKLIGHT);
        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, \
                   3, bpIDBacklight, 4, 4, out), SIC_SCROLL_LIST_STYLE);
        
        EQC_Time(EQS_NA, EQS_NA, l_dwOutStrtTime/60 , TIME_TIME_CONTROL ); 
        EQC_StaticText(EQS_NA , EQS_NA, TKN_SCHEDULE_START_END_HYPEN);
        EQC_Time(EQS_NA ,EQS_NA, l_dwOutEndTime/60, TIME_TIME_CONTROL );
        
        BacklightCase = OUT_USED_TIME;
        EQS_StartFormatter();

        for(index = 0; index< SBO_MAX; index++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(index), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            
            if(index != SBO_OUT_STRT_TIME && index != SBO_OUT_HYPEN)
                EQS_NewLine();
        }

        EQS_EndFormatter();    
        if(where == SWF_UPPER_FOCUS )
            EQS_SetIOCFocus(EQS_GetNthIOC(SBO_IN_SELECT));
        else
            EQS_SetIOCFocus(EQS_GetNthIOC(SBO_OUT_SELECT));
    }else if(in == AB_TIME && out == AB_TIME)
    {
        EQC_StaticText(EQS_NA,  EQS_NA, TKN_AUTO_LCD_BACKLIGHT); 
        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, 
                   3, bpBacklight, 4, 4, in), SIC_SCROLL_LIST_STYLE);

        EQC_Time(EQS_NA, EQS_NA, l_dwInStrtTime/60 , TIME_TIME_CONTROL ); 
        EQC_StaticText(EQS_NA , EQS_NA, TKN_SCHEDULE_START_END_HYPEN);
        EQC_Time(EQS_NA ,EQS_NA, l_dwInEndTime/60, TIME_TIME_CONTROL );
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_AUTO_IDLCD_BACKLIGHT);
        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, 
                   3, bpIDBacklight, 4, 4, out ), SIC_SCROLL_LIST_STYLE);
        
        EQC_Time(EQS_NA, EQS_NA, l_dwOutStrtTime/60 , TIME_TIME_CONTROL ); 
        EQC_StaticText(EQS_NA , EQS_NA, TKN_SCHEDULE_START_END_HYPEN);
        EQC_Time(EQS_NA ,EQS_NA, l_dwOutEndTime/60, TIME_TIME_CONTROL );

        BacklightCase = IN_OUT_USED_TIME;
        EQS_StartFormatter();

        for(index = 0; index< SBIO_MAX; index++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(index), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            if(index != SBIO_IN_STRT_TIME && index != SBIO_IN_HYPEN &&
                index != SBIO_OUT_STRT_TIME && index != SBIO_OUT_HYPEN )
                EQS_NewLine();
        }
        EQS_EndFormatter();    

        if(where == SWF_UPPER_FOCUS )
            EQS_SetIOCFocus(EQS_GetNthIOC(SBIO_IN_SELECT));
        else
            EQS_SetIOCFocus(EQS_GetNthIOC(SBIO_OUT_SELECT));
    }
    EQS_FitCanvasSize();
}
#endif //ADVANCED_BACKLIGHT_CONTROL

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

void SKY_API SKA_MainAutoBacklight(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;
    STATIC HCONTROL hStartTime = INVALID_CONTROL_HANDLE;
    STATIC HCONTROL hIDStartTime = INVALID_CONTROL_HANDLE;
    clk_julian_type cjSetTime;

#ifndef ADVANCED_BACKLIGHT_CONTROL
    STATIC HCONTROL s_hScrolList, s_hEndText, s_hEndTime;//,s_hStrtText;
    STATIC HCONTROL s_hIDScrolList, s_hIDEndText, s_hIDEndTime ;//,s_hIDStrtText ; 
    UINT8 nIndex , nDY, nDX;
    DWORD sTime;
#endif
#ifdef ADVANCED_BACKLIGHT_CONTROL
    SKA_WHERE_FOCUS_E whereFocus;
    AUTO_BACKLIGHT_E  nInVal, nOutVal;
#endif // ADVANCED_BACKLIGHT_CONTROL

    STATIC BOOL s_fEndToRoot = FALSE;
    HCONTROL hControl;

    
    bpBacklight[0] = (BYTE*)SKY_GET_TOKEN(TKN_AUTO_LCD_ON);
    bpBacklight[1] = (BYTE*)SKY_GET_TOKEN(TKN_ALWAYS_ON);
    bpBacklight[2] = (BYTE*)SKY_GET_TOKEN(TKN_TIME_AUTO_ON);
    bpBacklight[3] = (BYTE*)SKY_GET_TOKEN(TKN_NOTUSED);

    bpIDBacklight[0] = (BYTE*)SKY_GET_TOKEN(TKN_AUTO_LCD_ON);
    bpIDBacklight[1] = (BYTE*)SKY_GET_TOKEN(TKN_ALWAYS_ON);
    bpIDBacklight[2] = (BYTE*)SKY_GET_TOKEN(TKN_TIME_ALWAYS_ON);
    bpIDBacklight[3] = (BYTE*)SKY_GET_TOKEN(TKN_NOTUSED);

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            100,//SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_BACKLIGHT_SET), 
            WA_SHOW_HEADER | WA_SHOW_CML);
        
        l_dwInStrtTime = SKY_GET_AUTO_BACKLIGHT_START_TIME(); 
        l_dwInEndTime = SKY_GET_AUTO_BACKLIGHT_END_TIME();
        l_dwOutStrtTime = SKY_GET_AUTO_SUBBACKLIGHT_START_TIME();
        l_dwOutEndTime = SKY_GET_AUTO_SUBBACKLIGHT_END_TIME();

#ifdef ADVANCED_BACKLIGHT_CONTROL
        sAddBacklightCtrl(SKY_GET_AUTO_BACKLIGHT(),SKY_GET_AUTO_SUBBACKLIGHT(), SWF_UPPER_FOCUS);
#else
        hControl = EQC_StaticText(4 ,  3, TKN_AUTO_LCD_BACKLIGHT);
        nDY = 3;
        nDY += EQC_GET_DY(hControl);
        nDY += 3;
        s_hScrolList = EQC_ScrollCombo(4, nDY, SCROLL_LIST_WIDTH, 
                   3, 
                   bpBacklight, 
                   4, 4,
                   (UINT8)SKY_GET_AUTO_BACKLIGHT());

        EQC_SET_STYLE(s_hScrolList, SIC_SCROLL_LIST_STYLE);
        nDY += 3;
        nDY += EQC_GET_DY(s_hScrolList);
        
        if(SKY_GET_AUTO_BACKLIGHT() == AB_TIME)
        {
            nDX = 3;
            hStartTime = EQC_Time(nDX, nDY, SKY_GET_AUTO_BACKLIGHT_START_TIME()/60 , TIME_TIME_CONTROL ); 
            nDX += EQC_GET_DX(hStartTime);
            nDX += 3;

            s_hEndText = EQC_StaticText(nDX , nDY, TKN_SCHEDULE_START_END_HYPEN);
            nDX += EQC_GET_DX(s_hEndText);
            nDX += 3;
            s_hEndTime = EQC_Time(nDX ,nDY, SKY_GET_AUTO_BACKLIGHT_END_TIME()/60, TIME_TIME_CONTROL );
            nDY += 3;
            nDY += EQC_GET_DY(s_hEndTime);
            nDY += 3;
        }
        
        hControl = EQC_StaticText(4, 51, TKN_AUTO_IDLCD_BACKLIGHT);
        nDY = 51;
        nDY += 3;
        nDY += EQC_GET_DY(hControl);
        s_hIDScrolList = EQC_ScrollCombo(4, nDY, SCROLL_LIST_WIDTH, 
                   3, 
                   bpIDBacklight, 
                   4, 4,
                   (UINT8)SKY_GET_AUTO_SUBBACKLIGHT());
        EQC_SET_STYLE(s_hIDScrolList, SIC_SCROLL_LIST_STYLE);
        nDY += 3;
        nDY += EQC_GET_DY(s_hIDScrolList);
        nDX = 3;
        if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME)
        {
            hIDStartTime = EQC_Time(nDX, nDY, SKY_GET_AUTO_SUBBACKLIGHT_START_TIME()/60 , TIME_TIME_CONTROL ); 

            nDX += EQC_GET_DX(hIDStartTime);
            nDX += 3;

            s_hIDEndText = EQC_StaticText(nDX , nDY, TKN_SCHEDULE_START_END_HYPEN);
            nDX += EQC_GET_DX(s_hIDEndText);
            nDX += 3;
            s_hIDEndTime = EQC_Time(nDX ,nDY, SKY_GET_AUTO_SUBBACKLIGHT_END_TIME()/60, TIME_TIME_CONTROL );
        }

#endif //ADVANCED_BACKLIGHT_CONTROL
        break;

    case SE_APP_IO_MODE:
#ifdef ADVANCED_BACKLIGHT_CONTROL
/*        if(BacklightCase == NOT_USED_TIME)
        {
            if(pEvent->wParam == EQS_GetNthIOC(SBN_IN_SELECT) || pEvent->wParam == EQS_GetNthIOC(SBN_OUT_SELECT))
            {
                hControl = EQS_GetFocusedIOC();
                if(hControl == EQS_GetNthIOC(SBN_IN_SELECT))
                    whereFocus = SWF_UPPER_FOCUS;
                else
                    whereFocus = SWF_DOWN_FOCUS;
                nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_IN_SELECT));
                nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_OUT_SELECT));
                sDeleteBacklightCtrl(BacklightCase);
                sAddBacklightCtrl(nInVal, nOutVal, whereFocus);
            }
        }
        else if(BacklightCase == IN_USED_TIME)
        {
            if(pEvent->wParam == EQS_GetNthIOC(SBI_IN_SELECT) || pEvent->wParam == EQS_GetNthIOC(SBI_OUT_SELECT))
            {
                hControl = EQS_GetFocusedIOC();
                if(hControl == EQS_GetNthIOC(SBI_IN_SELECT))
                    whereFocus = SWF_UPPER_FOCUS;
                else
                    whereFocus = SWF_DOWN_FOCUS;
                nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_IN_SELECT));
                nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_OUT_SELECT));
                sDeleteBacklightCtrl(BacklightCase);
                sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
            }
        }
        else if(BacklightCase == OUT_USED_TIME)
        {
            if(pEvent->wParam == EQS_GetNthIOC(SBO_IN_SELECT) || pEvent->wParam == EQS_GetNthIOC(SBO_OUT_SELECT))
            {
                hControl = EQS_GetFocusedIOC();
                if(hControl == EQS_GetNthIOC(SBO_IN_SELECT))
                    whereFocus = SWF_UPPER_FOCUS;
                else
                    whereFocus = SWF_DOWN_FOCUS;
                nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_IN_SELECT));
                nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_OUT_SELECT));
                sDeleteBacklightCtrl(BacklightCase);
                sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
            }
        }
        else if(BacklightCase == IN_OUT_USED_TIME)
        {
            if(pEvent->wParam == EQS_GetNthIOC(SBIO_IN_SELECT) || pEvent->wParam == EQS_GetNthIOC(SBIO_OUT_SELECT))
            {
                hControl = EQS_GetFocusedIOC();
                if(hControl == EQS_GetNthIOC(SBIO_IN_SELECT))
                    whereFocus = SWF_UPPER_FOCUS;
                else
                    whereFocus = SWF_DOWN_FOCUS;
                nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_IN_SELECT));
                nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_OUT_SELECT));
                sDeleteBacklightCtrl(BacklightCase);
                sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
            }
        }
*/
#else
        if(pEvent->wParam == s_hScrolList && hStartTime != INVALID_CONTROL_HANDLE)
        {
            //EQS_DeleteControl(s_hStrtText);
            EQS_DeleteControl(hStartTime);
            EQS_DeleteControl(s_hEndText);
            EQS_DeleteControl(s_hEndTime);
            hStartTime = INVALID_CONTROL_HANDLE;
        }
        if(pEvent->wParam == s_hIDScrolList && hIDStartTime != INVALID_CONTROL_HANDLE)
        {
            //EQS_DeleteControl(s_hIDStrtText);
            EQS_DeleteControl(hIDStartTime);
            EQS_DeleteControl(s_hIDEndText);
            EQS_DeleteControl(s_hIDEndTime);
            hIDStartTime = INVALID_CONTROL_HANDLE;
        }
#endif //ADVANCED_BACKLIGHT_CONTROL
        break;

    case SE_IO_SCROLL_LIST_CHG:
#ifdef ADVANCED_BACKLIGHT_CONTROL
        if(EQS_IsActive(pEvent->wParam) == FALSE)
        {
            if(BacklightCase == NOT_USED_TIME)
            {   
                hControl = EQS_GetFocusedIOC();
                if(hControl == EQS_GetNthIOC(SBN_IN_SELECT))
                    whereFocus = SWF_UPPER_FOCUS;
                else
                    whereFocus = SWF_DOWN_FOCUS;
                nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_IN_SELECT));
                nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_OUT_SELECT));

                sDeleteBacklightCtrl(BacklightCase);
                sAddBacklightCtrl(nInVal, nOutVal, whereFocus);
            }
            else if(BacklightCase == IN_USED_TIME)
            {
                hControl = EQS_GetFocusedIOC();
                if(hControl == EQS_GetNthIOC(SBI_IN_SELECT))
                    whereFocus = SWF_UPPER_FOCUS;
                else
                    whereFocus = SWF_DOWN_FOCUS;

                cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBI_IN_STRT_TIME));
                cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBI_IN_STRT_TIME));
                cjSetTime.second = 0;
                l_dwInStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
        
                cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBI_IN_END_TIME));
                cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBI_IN_END_TIME));
                cjSetTime.second = 0;
                l_dwInEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

                nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_IN_SELECT));
                nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_OUT_SELECT));
                sDeleteBacklightCtrl(BacklightCase);
                sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
            }
            else if(BacklightCase == OUT_USED_TIME)
            {
                hControl = EQS_GetFocusedIOC();
                if(hControl == EQS_GetNthIOC(SBO_IN_SELECT))
                    whereFocus = SWF_UPPER_FOCUS;
                else
                    whereFocus = SWF_DOWN_FOCUS;

                cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBO_OUT_STRT_TIME));
                cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBO_OUT_STRT_TIME));
                cjSetTime.second = 0;
                l_dwOutStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

                cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBO_OUT_END_TIME));
                cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBO_OUT_END_TIME));
                cjSetTime.second = 0;
                l_dwOutEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

                nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_IN_SELECT));
                nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_OUT_SELECT));
                sDeleteBacklightCtrl(BacklightCase);
                sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
            }
            else if(BacklightCase == IN_OUT_USED_TIME)
            {
                hControl = EQS_GetFocusedIOC();
                if(hControl == EQS_GetNthIOC(SBIO_IN_SELECT))
                    whereFocus = SWF_UPPER_FOCUS;
                else
                    whereFocus = SWF_DOWN_FOCUS;

                cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_IN_STRT_TIME));
                cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_IN_STRT_TIME));
                cjSetTime.second = 0;
                l_dwInStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            
                cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_IN_END_TIME));
                cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_IN_END_TIME));
                cjSetTime.second = 0;
                l_dwInEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

                cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_OUT_STRT_TIME));
                cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_OUT_STRT_TIME));
                cjSetTime.second = 0;
                l_dwOutStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

                cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_OUT_END_TIME));
                cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_OUT_END_TIME));
                cjSetTime.second = 0;
                l_dwOutEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

                nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_IN_SELECT));
                nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_OUT_SELECT));
                sDeleteBacklightCtrl(BacklightCase);
                sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
            }
        }
#endif //ADVANCED_BACKLIGHT_CONTROL
        break;

    case SE_IO_SCROLL_LIST_SELECT:
#ifdef ADVANCED_BACKLIGHT_CONTROL
        if(BacklightCase == NOT_USED_TIME)
        {
            hControl = EQS_GetFocusedIOC();
            if(hControl == EQS_GetNthIOC(SBN_IN_SELECT))
                whereFocus = SWF_UPPER_FOCUS;
            else
                whereFocus = SWF_DOWN_FOCUS;
            nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_IN_SELECT));
            nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBN_OUT_SELECT));
            sDeleteBacklightCtrl(BacklightCase);
            sAddBacklightCtrl(nInVal, nOutVal, whereFocus);
        }
        else if(BacklightCase == IN_USED_TIME)
        {
            hControl = EQS_GetFocusedIOC();
            if(hControl == EQS_GetNthIOC(SBI_IN_SELECT))
                whereFocus = SWF_UPPER_FOCUS;
            else
                whereFocus = SWF_DOWN_FOCUS;

            cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBI_IN_STRT_TIME));
            cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBI_IN_STRT_TIME));
            cjSetTime.second = 0;
            l_dwInStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            
            cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBI_IN_END_TIME));
            cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBI_IN_END_TIME));
            cjSetTime.second = 0;
            l_dwInEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

            nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_IN_SELECT));
            nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBI_OUT_SELECT));
            sDeleteBacklightCtrl(BacklightCase);
            sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
        }
        else if(BacklightCase == OUT_USED_TIME)
        {
            hControl = EQS_GetFocusedIOC();
            if(hControl == EQS_GetNthIOC(SBO_IN_SELECT))
                whereFocus = SWF_UPPER_FOCUS;
            else
                whereFocus = SWF_DOWN_FOCUS;

            cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBO_OUT_STRT_TIME));
            cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBO_OUT_STRT_TIME));
            cjSetTime.second = 0;
            l_dwOutStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

            cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBO_OUT_END_TIME));
            cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBO_OUT_END_TIME));
            cjSetTime.second = 0;
            l_dwOutEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_IN_SELECT));
            nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBO_OUT_SELECT));
            sDeleteBacklightCtrl(BacklightCase);
            sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
        }
        else if(BacklightCase == IN_OUT_USED_TIME)
        {
            hControl = EQS_GetFocusedIOC();
            if(hControl == EQS_GetNthIOC(SBIO_IN_SELECT))
                whereFocus = SWF_UPPER_FOCUS;
            else
                whereFocus = SWF_DOWN_FOCUS;

            cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_IN_STRT_TIME));
            cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_IN_STRT_TIME));
            cjSetTime.second = 0;
            l_dwInStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            
            cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_IN_END_TIME));
            cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_IN_END_TIME));
            cjSetTime.second = 0;
            l_dwInEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

            cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_OUT_STRT_TIME));
            cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_OUT_STRT_TIME));
            cjSetTime.second = 0;
            l_dwOutStrtTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

            cjSetTime.hour = EQC_GetTimeHour(EQS_GetNthIOC(SBIO_OUT_END_TIME));
            cjSetTime.minute = EQC_GetTimeMinute(EQS_GetNthIOC(SBIO_OUT_END_TIME));
            cjSetTime.second = 0;
            l_dwOutEndTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;

            nInVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_IN_SELECT));
            nOutVal = (AUTO_BACKLIGHT_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBIO_OUT_SELECT));
            sDeleteBacklightCtrl(BacklightCase);
            sAddBacklightCtrl(nInVal,nOutVal, whereFocus);
        }
#else
        if(pEvent->wParam == s_hScrolList)
        {
            nIndex = EQC_GetScrollLogicalIndex(s_hScrolList);
            if(nIndex == AB_TIME)
            {
                nDX = 4;
                nDY = 35;
                hStartTime = EQC_Time(nDX, nDY, SKY_GET_AUTO_BACKLIGHT_START_TIME()/60 , TIME_TIME_CONTROL ); 
                nDX += EQC_GET_DX(hStartTime);
                nDX += 3;

                s_hEndText = EQC_StaticText(nDX , nDY, TKN_SCHEDULE_START_END_HYPEN);
                nDX += EQC_GET_DX(s_hEndText);
                nDX += 3;
                s_hEndTime = EQC_Time(nDX ,nDY, SKY_GET_AUTO_BACKLIGHT_END_TIME()/60, TIME_TIME_CONTROL );
                nDY += 3;
                nDY += EQC_GET_DY(s_hEndTime);
                nDY += 3;            
            }

        }else if(pEvent->wParam == s_hIDScrolList)
        {
            nIndex = EQC_GetScrollLogicalIndex(s_hIDScrolList);
            if(nIndex == AB_TIME)
            {
                nDX = 4;
                nDY = 82;
                hIDStartTime = EQC_Time(nDX, nDY, SKY_GET_AUTO_SUBBACKLIGHT_START_TIME()/60 , TIME_TIME_CONTROL ); 

                nDX += EQC_GET_DX(hIDStartTime);
                nDX += 3;

                s_hIDEndText = EQC_StaticText(nDX , nDY, TKN_SCHEDULE_START_END_HYPEN);
                nDX += EQC_GET_DX(s_hIDEndText);
                nDX += 3;
                s_hIDEndTime = EQC_Time(nDX ,nDY, SKY_GET_AUTO_SUBBACKLIGHT_END_TIME()/60, TIME_TIME_CONTROL );
            }
        }
#endif //ADVANCED_BACKLIGHT_CONTROL
        break;

    case SE_SOFTKEY_SAVE:
#ifdef ADVANCED_BACKLIGHT_CONTROL
        sSaveBacklightValue(BacklightCase);
#else
        nIndex = EQC_GetScrollLogicalIndex(s_hScrolList);
        //nIndex = EQC_GetRadioButton(EQS_GetNthIOC(LB_LCD_RADIO_BUTTON));
        SKY_SET_AUTO_BACKLIGHT((AUTO_BACKLIGHT_E)nIndex);
        if(nIndex == AB_TIME && hStartTime != INVALID_CONTROL_HANDLE)
        {
            cjSetTime.hour = EQC_GetTimeHour(hStartTime);
            cjSetTime.minute = EQC_GetTimeMinute(hStartTime);
            cjSetTime.second = 0;
    
            sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            SKY_SET_AUTO_BACKLIGHT_START_TIME(sTime);
            cjSetTime.hour = EQC_GetTimeHour(s_hEndTime);
            cjSetTime.minute = EQC_GetTimeMinute(s_hEndTime);
            cjSetTime.second = 0;
            sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            SKY_SET_AUTO_BACKLIGHT_END_TIME(sTime);
        }
        if(nIndex == AB_NO )
            SKY_BacklightOff();
        
        nIndex = EQC_GetScrollLogicalIndex(s_hIDScrolList);
        //nIndex = EQC_GetRadioButton(EQS_GetNthIOC(LB_ID_LCD_RADIO_BUTTON));
        SKY_SET_AUTO_SUBBACKLIGHT((AUTO_BACKLIGHT_E)nIndex);
        if(nIndex == AB_TIME && hIDStartTime != INVALID_CONTROL_HANDLE)
        {
            cjSetTime.hour = EQC_GetTimeHour(hIDStartTime);
            cjSetTime.minute = EQC_GetTimeMinute(hIDStartTime);
            cjSetTime.second = 0;
        
            sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            SKY_SET_AUTO_SUBBACKLIGHT_START_TIME(sTime);

            cjSetTime.hour = EQC_GetTimeHour(s_hIDEndTime);
            cjSetTime.minute = EQC_GetTimeMinute(s_hIDEndTime);
            cjSetTime.second = 0;

            sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            SKY_SET_AUTO_SUBBACKLIGHT_END_TIME(sTime);
        }    
        if(nIndex == AB_NO )
            SKY_SubBacklightOff();        
#endif //ADVANCED_BACKLIGHT_CONTROL
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_APP_END :   /* The very last event */ 
        hStartTime = INVALID_CONTROL_HANDLE;
        hIDStartTime = INVALID_CONTROL_HANDLE;
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
            {
                SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
                EQS_End();
            }
        }
        break;

    case SE_KEY_CLEAR:
#ifdef ADVANCED_BACKLIGHT_CONTROL
        if(!sConfirmControlChg(BacklightCase))
#else
        if(EQS_IsControlDataChanged())
#endif
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
        EQS_End();
        break;
    
    case SE_KEY_END:
        s_fEndToRoot = TRUE;
#ifdef ADVANCED_BACKLIGHT_CONTROL
        if(!sConfirmControlChg(BacklightCase))
#else
        if(EQS_IsControlDataChanged())
#endif
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
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
        {
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            EQS_End();
        }
        break;

    case SE_IO_MESGBOX_CLEAR:
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

#if 0 
/*============================================================================= 
Function:    
SKA_MainAutoLCDBacklight

  Description: 
  @Description
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
void SKY_API SKA_MainAutoLCDBacklight(SKY_EVENT_T *pEvent)
{

    CONST BYTE* sBacklight[4];
    STATIC HCONTROL s_hMsgControl, hTextEdit1, hTextEdit2; 
    STATIC HCONTROL hStartTime = INVALID_CONTROL_HANDLE;
    STATIC HCONTROL hEndTime = INVALID_CONTROL_HANDLE;
    UINT8 nIndx , nDY;
    STATIC BOOL s_fEndToRoot = FALSE;

    clk_julian_type cjSetTime;
    DWORD sTime;
    
    sBacklight[0] = (BYTE*)SKY_GET_TOKEN(TKN_AUTO_LCD_ON);
    sBacklight[1] = (BYTE*)SKY_GET_TOKEN(TKN_ALWAYS_ON);
    sBacklight[2] = (BYTE*)SKY_GET_TOKEN(TKN_TIME_AUTO_ON);
    sBacklight[3] = (BYTE*)SKY_GET_TOKEN(TKN_NOTUSED);

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 160, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            160, //SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_AUTO_LCD_BACKLIGHT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_RadioButton(EQS_NA, EQS_NA, 6, 4, (BYTE **)sBacklight, (UINT8)SKY_GET_AUTO_BACKLIGHT());
        EQS_SetIOCActive(EQS_GetNthIOC(SA_SELECT_LIGHT));

        EQS_StartFormatter();

        EQS_SetControlPosition(EQS_GetNthIOC(SA_SELECT_LIGHT), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();

        EQS_EndFormatter();

        break;

    case SE_APP_IO_MODE:
        if(pEvent->wParam == EQS_GetNthIOC(SA_SELECT_LIGHT) && hStartTime != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hStartTime);
            EQS_DeleteControl(hEndTime);
            EQS_DeleteControl(hTextEdit1);
            EQS_DeleteControl(hTextEdit2);
            hStartTime = INVALID_CONTROL_HANDLE;
        }
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
        nIndx = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        nDY = EQC_GET_DY(EQS_GetNthIOC(SA_SELECT_LIGHT));
        if(nIndx == AB_TIME && hStartTime == INVALID_CONTROL_HANDLE)
        {
            hTextEdit1 = EQC_StaticText(1, nDY+5, TKN_SCHEDULE_START_TIME); 
            hStartTime = EQC_Time( 28, nDY+5, SKY_GET_AUTO_SUBBACKLIGHT_START_TIME()/60 , TIME_TIME_CONTROL );

            hTextEdit2 = EQC_StaticText(1, nDY+20, TKN_SCHEDULE_END_TIME);
            hEndTime = EQC_Time( 28,nDY+20, SKY_GET_AUTO_SUBBACKLIGHT_END_TIME()/60, TIME_TIME_CONTROL );
            
            EQS_SetIOCActive(hStartTime);
        }else
            EQS_SetIOCActive(EQS_GetNthIOC(SA_SELECT_LIGHT));

        break;

    case SE_SOFTKEY_SAVE:
        nIndx = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        nDY = EQC_GET_DY(EQS_GetNthIOC(SA_SELECT_LIGHT));
        
        if(nIndx == AB_TIME && hStartTime != INVALID_CONTROL_HANDLE)
        {
            SKY_SET_AUTO_BACKLIGHT((AUTO_BACKLIGHT_E)nIndx);
            cjSetTime.hour = EQC_GetTimeHour(hStartTime);
            cjSetTime.minute = EQC_GetTimeMinute(hStartTime);
            cjSetTime.second = 0;
        
            sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            SKY_SET_AUTO_BACKLIGHT_START_TIME(sTime);

            cjSetTime.hour = EQC_GetTimeHour(hEndTime);
            cjSetTime.minute = EQC_GetTimeMinute(hEndTime);
            cjSetTime.second = 0;

            sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            SKY_SET_AUTO_BACKLIGHT_END_TIME(sTime);
            
            s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE),MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

        }else if(nIndx == AB_TIME && hStartTime == INVALID_CONTROL_HANDLE)
        {

            hTextEdit1 = EQC_StaticText(1, nDY+5, TKN_SCHEDULE_START_TIME); 
            hStartTime = EQC_Time( 28, nDY+5, SKY_GET_AUTO_BACKLIGHT_START_TIME()/60, TIME_TIME_CONTROL );

            hTextEdit2 = EQC_StaticText(1, nDY+20, TKN_SCHEDULE_END_TIME);
            hEndTime = EQC_Time( 28,nDY+20, SKY_GET_AUTO_BACKLIGHT_END_TIME()/60, TIME_TIME_CONTROL );
            
            EQS_SetIOCActive(hStartTime);

        }else
        {
            SKY_SET_AUTO_BACKLIGHT((AUTO_BACKLIGHT_E)nIndx);
            
            if(nIndx == AB_NO )
                SKY_BacklightOff();

            s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        break;

    case SE_APP_END :   /* The very last event */ 
        hStartTime = INVALID_CONTROL_HANDLE;
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
            {
                SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
                EQS_End();
            }
        }
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*        nDY = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        if((UINT8)SKY_GET_AUTO_BACKLIGHT() != nDY)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }else if((nDY == AB_TIME) && (SKY_GET_AUTO_BACKLIGHT() == AB_TIME))
        {
            if(hStartTime != INVALID_CONTROL_HANDLE)
            {
                cjSetTime.hour = EQC_GetTimeHour(hStartTime);
                cjSetTime.minute = EQC_GetTimeMinute(hStartTime);
                cjSetTime.second = 0;
        
                sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
                if(SKY_GET_AUTO_BACKLIGHT_START_TIME() != sTime)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                cjSetTime.hour = EQC_GetTimeHour(hEndTime);
                cjSetTime.minute = EQC_GetTimeMinute(hEndTime);
                cjSetTime.second = 0;

                sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
                if(SKY_GET_AUTO_BACKLIGHT_END_TIME() != sTime)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }

            }else
                EQS_End();
        }
*/
        EQS_End();
        break;
    
    case SE_IO_RADIO_BUTTON_CANCEL:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*        nDY = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        if((UINT8)SKY_GET_AUTO_BACKLIGHT() != nDY)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
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
        nDY = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        if((UINT8)SKY_GET_AUTO_BACKLIGHT() != nDY)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }else if((nDY == AB_TIME) && (SKY_GET_AUTO_BACKLIGHT() == AB_TIME))
        {
            if(hStartTime != INVALID_CONTROL_HANDLE)
            {
                cjSetTime.hour = EQC_GetTimeHour(hStartTime);
                cjSetTime.minute = EQC_GetTimeMinute(hStartTime);
                cjSetTime.second = 0;
        
                sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
                if(SKY_GET_AUTO_BACKLIGHT_START_TIME() != sTime)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                cjSetTime.hour = EQC_GetTimeHour(hEndTime);
                cjSetTime.minute = EQC_GetTimeMinute(hEndTime);
                cjSetTime.second = 0;

                sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
                if(SKY_GET_AUTO_BACKLIGHT_END_TIME() != sTime)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }

            }else
            {
                s_fEndToRoot = FALSE;
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
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
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
        {
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            EQS_End();
        }
        break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(SA_SELECT_LIGHT));
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*============================================================================= 
Function:    
SKA_MainAutoIDLCDBacklight

  Description: 
  @Description
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
void SKY_API SKA_MainAutoSubLCDBacklight(SKY_EVENT_T *pEvent)
{
    CONST BYTE* sBacklight[4];
    STATIC HCONTROL s_hMsgControl, hTextEdit1, hTextEdit2; 
    STATIC HCONTROL hStartTime = INVALID_CONTROL_HANDLE;
    STATIC HCONTROL hEndTime = INVALID_CONTROL_HANDLE;
    UINT8 nIndx , nDY;
    STATIC BOOL s_fEndToRoot = FALSE;

    clk_julian_type cjSetTime;
    DWORD sTime;

    sBacklight[0] = (BYTE*)SKY_GET_TOKEN(TKN_AUTO_LCD_ON);
    sBacklight[1] = (BYTE*)SKY_GET_TOKEN(TKN_ALWAYS_ON);
    sBacklight[2] = (BYTE*)SKY_GET_TOKEN(TKN_TIME_ALWAYS_ON);
    sBacklight[3] = (BYTE*)SKY_GET_TOKEN(TKN_NOTUSED);

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 160, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            160, //SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_AUTO_IDLCD_BACKLIGHT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_RadioButton(EQS_NA, EQS_NA, 6, 4, (BYTE **)sBacklight, (UINT8)SKY_GET_AUTO_SUBBACKLIGHT());
        EQS_SetIOCActive(EQS_GetNthIOC(SA_SELECT_LIGHT));

        EQS_StartFormatter();

        EQS_SetControlPosition(EQS_GetNthIOC(SA_SELECT_LIGHT), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();

        EQS_EndFormatter();

        break;

    case SE_APP_IO_MODE:
        if(pEvent->wParam == EQS_GetNthIOC(SA_SELECT_LIGHT) && hStartTime != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hStartTime);
            EQS_DeleteControl(hEndTime);
            EQS_DeleteControl(hTextEdit1);
            EQS_DeleteControl(hTextEdit2);
            hStartTime = INVALID_CONTROL_HANDLE;
        }
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
        nIndx = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        nDY = EQC_GET_DY(EQS_GetNthIOC(SA_SELECT_LIGHT));
        if(nIndx == AB_TIME && hStartTime == INVALID_CONTROL_HANDLE)
        {
            hTextEdit1 = EQC_StaticText(1, nDY+5, TKN_SCHEDULE_START_TIME); 
            hStartTime = EQC_Time( 28, nDY+5, SKY_GET_AUTO_SUBBACKLIGHT_START_TIME()/60 , TIME_TIME_CONTROL );

            hTextEdit2 = EQC_StaticText(1, nDY+20, TKN_SCHEDULE_END_TIME);
            hEndTime = EQC_Time( 28,nDY+20, SKY_GET_AUTO_SUBBACKLIGHT_END_TIME()/60, TIME_TIME_CONTROL );
            
            EQS_SetIOCActive(hStartTime);
        }else
            EQS_SetIOCActive(EQS_GetNthIOC(SA_SELECT_LIGHT));
        break;

    case SE_SOFTKEY_SAVE:
        nIndx = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        nDY = EQC_GET_DY(EQS_GetNthIOC(SA_SELECT_LIGHT));
        
        if(nIndx == AB_TIME && hStartTime != INVALID_CONTROL_HANDLE)
        {

            SKY_SET_AUTO_SUBBACKLIGHT((AUTO_BACKLIGHT_E)nIndx);
            cjSetTime.hour = EQC_GetTimeHour(hStartTime);
            cjSetTime.minute = EQC_GetTimeMinute(hStartTime);
            cjSetTime.second = 0;
        
            sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            SKY_SET_AUTO_SUBBACKLIGHT_START_TIME(sTime);

            cjSetTime.hour = EQC_GetTimeHour(hEndTime);
            cjSetTime.minute = EQC_GetTimeMinute(hEndTime);
            cjSetTime.second = 0;

            sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
            SKY_SET_AUTO_SUBBACKLIGHT_END_TIME(sTime);
            
            s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

        }else if(nIndx == AB_TIME && hStartTime == INVALID_CONTROL_HANDLE)
        {

            hTextEdit1 = EQC_StaticText(1, nDY+5, TKN_SCHEDULE_START_TIME); 
            hStartTime = EQC_Time( 28, nDY+5, SKY_GET_AUTO_SUBBACKLIGHT_START_TIME()/60 , TIME_TIME_CONTROL );

            hTextEdit2 = EQC_StaticText(1, nDY+20, TKN_SCHEDULE_END_TIME);
            hEndTime = EQC_Time( 28,nDY+20, SKY_GET_AUTO_SUBBACKLIGHT_END_TIME()/60, TIME_TIME_CONTROL );
            
            EQS_SetIOCActive(hStartTime);

        }else
        {
            SKY_SET_AUTO_SUBBACKLIGHT((AUTO_BACKLIGHT_E)nIndx);
            
            if(nIndx == AB_NO )
                SKY_SubBacklightOff();
            
            s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        break;

    case SE_APP_END :   /* The very last event */ 
        hStartTime = INVALID_CONTROL_HANDLE;
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                SKY_SubBacklightOn(SKY_GET_AUTO_BACKLIGHT());
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
            {
                SKY_SubBacklightOn(SKY_GET_AUTO_BACKLIGHT());
                EQS_End();
            }
            break;
        }
        break;

    case SE_KEY_CLEAR:
    case SE_IO_RADIO_BUTTON_CANCEL:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;        
        }
/*
        nDY = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        if((UINT8)SKY_GET_AUTO_SUBBACKLIGHT() != nDY)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }else if((nDY == AB_TIME) && (SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME))
        {
            if(hStartTime != INVALID_CONTROL_HANDLE)
            {
                cjSetTime.hour = EQC_GetTimeHour(hStartTime);
                cjSetTime.minute = EQC_GetTimeMinute(hStartTime);
                cjSetTime.second = 0;
        
                sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
                if(SKY_GET_AUTO_SUBBACKLIGHT_START_TIME() != sTime)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                cjSetTime.hour = EQC_GetTimeHour(hEndTime);
                cjSetTime.minute = EQC_GetTimeMinute(hEndTime);
                cjSetTime.second = 0;

                sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
                if(SKY_GET_AUTO_SUBBACKLIGHT_END_TIME() != sTime)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }

            }else
                EQS_End();
        }
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
        nDY = EQC_GetRadioButton(EQS_GetNthIOC(SA_SELECT_LIGHT));
        if((UINT8)SKY_GET_AUTO_SUBBACKLIGHT() != nDY)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }else if((nDY == AB_TIME) && (SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME))
        {
            if(hStartTime != INVALID_CONTROL_HANDLE)
            {
                cjSetTime.hour = EQC_GetTimeHour(hStartTime);
                cjSetTime.minute = EQC_GetTimeMinute(hStartTime);
                cjSetTime.second = 0;
        
                sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
                if(SKY_GET_AUTO_SUBBACKLIGHT_START_TIME() != sTime)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                cjSetTime.hour = EQC_GetTimeHour(hEndTime);
                cjSetTime.minute = EQC_GetTimeMinute(hEndTime);
                cjSetTime.second = 0;

                sTime = (cjSetTime.hour * 60 + cjSetTime.minute)* 60;
                if(SKY_GET_AUTO_SUBBACKLIGHT_END_TIME() != sTime)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }

            }else
			{
				s_fEndToRoot = FALSE;
                EQS_END_TO(EQS_POP_TO_ROOT); 
			}
        }
*/
        EQS_END_TO(EQS_POP_TO_ROOT);
        s_fEndToRoot = FALSE;
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            SKY_SubBacklightOn(SKY_GET_AUTO_BACKLIGHT());
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
        {
            SKY_SubBacklightOn(SKY_GET_AUTO_BACKLIGHT());
            EQS_End();
        }
        break;
        
    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(SA_SELECT_LIGHT));
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif //0
#if (SKY_MODEL_ID == SKY_MODEL_B1 || SKY_MODEL_ID == SKY_MODEL_B2)
/*============================================================================= 
    Function:    
    sAddBellVibControls

    Description: 
    착신벨 화면 설정
  
    Input:
    @ArgumentsList
    
    Output:
    @TheResultOfThisFunction
      
    Return value:
    @ReturnValue
        
    Side effects:
===============================================================================*/

void EQS_API sAddColorSelControls(void)
{
    UINT8   index,nAlert, nCtrl;
    
    // 조명 선택 대기시
    for(index=0; index<NUM_COLOR; index++)
        ppIdLcdColor[index] = (BYTE *)(BYTE*)SKY_GET_TOKEN(aColorSelect[index]);

    EQC_StaticText(EQS_NA, EQS_NA, TKN_WHEN_IDLE );
    EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, 
                   5, 
                   ppIdLcdColor, 
                   NUM_COLOR, NUM_COLOR,
                   (UINT8)SKY_GET_IDLCD_COLOR_IDLE()),
                   SIC_SCROLL_LIST_STYLE);

    // 조명 선택 SMS
    EQC_StaticText(EQS_NA, EQS_NA, TKN_WHEN_SMS_ALERT);
    EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, 
                   5, 
                   ppIdLcdColor, 
                   NUM_COLOR, NUM_COLOR,
                   (UINT8)SKY_GET_IDLCD_COLOR_SMS()),
                   SIC_SCROLL_LIST_STYLE);


    // 조명 선택 - 알람
    EQC_StaticText(EQS_NA, EQS_NA, TKN_WHEN_ALARM);
    EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, 
                   5, 
                   ppIdLcdColor, 
                   NUM_COLOR, NUM_COLOR,
                   (UINT8)SKY_GET_IDLCD_COLOR_ALARM()),
                   SIC_SCROLL_LIST_STYLE);

    // 조명 선택 착신시
    for(index=0; index<NUM_COLOR+1; index++)
        ppIdLcdColorAlert[index] = (BYTE *)(BYTE*)SKY_GET_TOKEN(aColorSelectAlert[index]);

    EQC_StaticText(EQS_NA, EQS_NA,  TKN_WHEN_CALL_ALERT);
    nAlert = (UINT8)SKY_GET_IDLCD_COLOR_ALERT();
    if(nAlert == RAINBOW_LCD_COLOR)
    {
        nAlert = 0;
    }else
    {
        nAlert += 1;
    }
    EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, 
                   5,//3, 
                   ppIdLcdColorAlert, 
                   NUM_COLOR +1 , NUM_COLOR + 1,
                   nAlert),
                   SIC_SCROLL_LIST_STYLE);

/*
    EQC_TextEdit(EQS_NA,EQS_NA, 19, 1, TEM_KOREAN, 
                MAX_BANNER_LENGTH,
                (BYTE *)(BYTE*)SKY_GET_TOKEN(TKN_WHEN_GROUP_ALERT),0);

    EQC_SET_STYLE(EQS_GetNthIOC(SIC_GROUP_ALERT_LABEL), CS_TEXT_EDIT_READONLY);
*/
    EQS_StartFormatter();

    for(nCtrl = 0; nCtrl < SIC_MAX; nCtrl++)
    {
        EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
            ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

        EQS_NewLine();
    }

    EQS_EndFormatter();
}

#ifdef FULL_GROUP_CONTROL
void EQS_API sAddGroupColorSelControls(void)
{
    HCONTROL    hControl;
    INT8    nIdx, nColorindex, nCtrl;
    BYTE    szTmp[20];
    pb_group_info_type  GroupInfo;
   
    for(nIdx=0; nIdx<NUM_COLOR+1; nIdx++)    /*ID LCD List*/
        ppIdLcdColorAlert[nIdx] = (BYTE *)(BYTE*)SKY_GET_TOKEN(aColorGroupSelectAlert[nIdx]); 
    
    for(nIdx = 0 ; nIdx < g_nGroup ; nIdx++)
    {
        hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_PHOTO_BLANK);
        GetGroupInfo(&GroupInfo, nIdx);
        sprintf(szTmp, "%s", GroupInfo.group_name);
        EQC_SetStaticText(hControl, szTmp);
        
        nColorindex = GroupInfo.alert_color;
        if(GroupInfo.alert_color == NONE_GROUP_ALERT_LCD)
            nColorindex = 8;
        else if(BLUE_LCD_COLOR <= GroupInfo.alert_color &&  GroupInfo.alert_color <= BLUE_GREEN_LCD_COLOR)//WHITE_LCD_COLOR)
            nColorindex += 1;
        else if(GroupInfo.alert_color == RAINBOW_LCD_COLOR)
            nColorindex = 0;
        else
            nColorindex = 8;

        EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA,EQS_NA, SCROLL_LIST_WIDTH, 
               3, 
               ppIdLcdColorAlert, 
               NUM_COLOR + 1, NUM_COLOR + 1,
               nColorindex),
               SIC_SCROLL_LIST_STYLE);
    }

    EQS_AutoFormatting(g_nGroup*2, EHA_LEFT, EVA_CENTER);
}
#else
void EQS_API sAddGroupColorSelControls(void)
{
    UINT8    groupIndex, index, nCtrl;
    pb_group_info_type  GroupInfo;

    for(index=0; index<NUM_COLOR+1; index++)    /*ID LCD List*/
        ppIdLcdColorAlert[index] = (BYTE *)(BYTE*)SKY_GET_TOKEN(aColorGroupSelectAlert[index]); 

    for(index=0; index < g_nGroup; index++)
        ppGroupList[index] = GetGroupName(index);

    EQC_StaticText(EQS_NA, EQS_NA, TKN_SEL_GROUP_LIGHT);

    EQC_SET_STYLE( EQC_ScrollCombo(EQS_NA, EQS_NA, SCROLL_LIST_WIDTH, 
                   SGC_GET_SLSIZE(g_nGroup), 
                   ppGroupList, 
                   g_nGroup, g_nGroup,
                   0),
                   SIC_SCROLL_LIST_STYLE);

    groupIndex = 0;
    GetGroupInfo(&GroupInfo, groupIndex);
    
    index = GroupInfo.alert_color;
    if(GroupInfo.alert_color == NONE_GROUP_ALERT_LCD)
        index = 8;
    else if(BLUE_LCD_COLOR <= GroupInfo.alert_color &&  GroupInfo.alert_color <= BLUE_GREEN_LCD_COLOR)//WHITE_LCD_COLOR)
        index += 1;
    else if(GroupInfo.alert_color == RAINBOW_LCD_COLOR)
        index = 0;
    else
        index = 8;
        
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SEL_BACKLIGHT);
    EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA,EQS_NA, SCROLL_LIST_WIDTH, 
           3, 
           ppIdLcdColorAlert, 
           NUM_COLOR + 1, NUM_COLOR + 1,
           index),
           SIC_SCROLL_LIST_STYLE);

    EQS_StartFormatter();

    for(nCtrl = 0; nCtrl < SGC_MAX; nCtrl++)
    {
        EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
            ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

        EQS_NewLine();
    }

    EQS_EndFormatter();

}
#endif
/*============================================================================= 
 Function:    
     SKA_SubLcdColorSel

 Description: 
     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
void SKY_API SKA_SubLcdColorSel(SKY_EVENT_T *pEvent)
{
    UINT8               nIndex;
    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC WORD s_wColorIndex = 0;
    
    switch(pEvent->EventID)
    {
    /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            160, //SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_IDLCD_BACKLIGHT_COLOR), 
            WA_SHOW_HEADER | WA_SHOW_CML);
        
        sAddColorSelControls();
        EQS_FitCanvasSize();
        
        nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIC_IDLE_SCRL));
        SKY_SubBacklight_Confirm(nIndex);
        break;

    case SE_APP_END :   /* The very last event */ 
        if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_ALWAYS || \
            (SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME && SKY_IsSubBacklightOnTime()) ||\
            SKY_GET_NO_OFF_BACKLIGHT() || SKY_IS_HANDS_FREE())
        {
            if(!SKY_IS_PHONE_OPEN())
                SKY_SubBacklight_Confirm(SKY_GET_AUTO_SUBBACKLIGHT());
            else
                SKY_SubBacklight_Confirm(NONE_LCD_COLOR);
        }
        else
            SKY_SubBacklight_Confirm(NONE_LCD_COLOR);
        EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_SCROLL_LIST_SELECT:
    case SE_IO_FOCUS_CHG:
    case SE_IO_SCROLL_LIST_CHG:
#if (defined FEATURE_SKY_SUBLCD && defined FEATURE_COLOR_ID_LCD)
        EQS_StopTimer(EQS_TIMER_1);
        nIndex = EQC_GetScrollLogicalIndex(pEvent->wParam);

        if(pEvent->wParam == EQS_GetNthIOC(SIC_ALERT_SCRL))
        {
            if(nIndex == 0)
            {
                EQS_StartTimer(EQS_TIMER_1, 50, TRUE);
                break;
            }else
            {
                nIndex -= 1;
            }
        }
        SKY_SubBacklight_Confirm(nIndex);
        break;

    case SE_TM_TIMER1: 
        s_wColorIndex %= 6;
        SKY_SubBacklight_Confirm(s_wColorIndex);
        s_wColorIndex ++;
#endif
        break;

    case SE_SOFTKEY_SAVE:
        nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIC_IDLE_SCRL));
        SKY_SET_IDLCD_COLOR_IDLE((ID_LCD_COLOR_E)nIndex);
        
        nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIC_SMS_SCRL));
        SKY_SET_IDLCD_COLOR_SMS((ID_LCD_COLOR_E)nIndex);
        
        nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIC_ALARM_SCRL));
        SKY_SET_IDLCD_COLOR_ALARM((ID_LCD_COLOR_E)nIndex);

        nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIC_ALERT_SCRL));
        if(nIndex == 0)
        {
            nIndex = NUM_COLOR;
        }else
        {
            nIndex -= 1;
        }
        SKY_SET_IDLCD_COLOR_ALERT((ID_LCD_COLOR_E)nIndex);
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
    
    case SE_IO_MESGBOX_END:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
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
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
/*
{
    UINT8 nColorIndex, index;

    for(index=0; index < g_nGroup; index++)
    {
        GetGroupInfo(&s_GroupInfo, index);

        nColorIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(index*2 +1));

        if(EQC_GetScrollLogicalIndex(EQS_GetNthIOC(index*2 +1))
                               == NUM_BELL_LIST+s_nMyBell)
        {
            s_GroupInfo.Bell.wBellID = 0;
        }
        else if(sIsMyBell(SGB_BELL))
        {
            nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(index*2 +1));
            STRCPY(s_GroupInfo.Bell.szFileName, ppMyBellList[nBellIndex-NUM_BELL_LIST]);
            s_GroupInfo.Bell.wBellID = SKY_MY_BELL_ID;
        }
        else
        {
            s_GroupInfo.Bell.wBellID = sGetCurrentBellId(SGB_BELL);
        }
        SetGroupInfo(&s_GroupInfo, index);
    }
}
*/
/*============================================================================= 
Function:    
SKA_GroupLcdSel

  Description: 
  @Description
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
void SKY_API SKA_GroupLcdSel(SKY_EVENT_T *pEvent)
{
    UINT8        nGroupIndex, nIndex;
    pb_group_info_type  GroupInfo;
    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC WORD s_wColorIndex = 0;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            160, //SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_GROUP_BACKLIGHT_COLOR), //WHEN_GROUP_ALERT), 
            WA_SHOW_HEADER | WA_SHOW_CML);
        if(g_nGroup == 0)
        {
            EQC_MessageBox((BYTE *)(BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_NOGROUP), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            
        }else
        {
            sAddGroupColorSelControls();
            EQS_FitCanvasSize();

            nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(1));
            if(nIndex == 0)
            {
                EQS_StartTimer(EQS_TIMER_1, 50, TRUE);
                break;            
            }else if(nIndex == NONE_LCD_COLOR + 1)
            {
                nIndex = SKY_GET_IDLCD_COLOR_ALERT();
                if(nIndex == RAINBOW_LCD_COLOR)
                {
                    EQS_StartTimer(EQS_TIMER_1, 50, TRUE);
                    break;
                }
                else
                {
                    //nIndex -= 1;
                    SKY_SubBacklight_Confirm(nIndex);
                }
            }else
            {
                nIndex -= 1;
                SKY_SubBacklight_Confirm(nIndex);
            }
        }
        break;
        
    case SE_APP_END :   /* The very last event */ 
        if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_ALWAYS || \
            (SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME && SKY_IsSubBacklightOnTime()) ||\
            SKY_GET_NO_OFF_BACKLIGHT() || SKY_IS_HANDS_FREE())
        {
            if(!SKY_IS_PHONE_OPEN())
                SKY_SubBacklight_Confirm(SKY_GET_AUTO_SUBBACKLIGHT());
            else
                SKY_SubBacklight_Confirm(NONE_LCD_COLOR);
        }
        else
            SKY_SubBacklight_Confirm(NONE_LCD_COLOR);

        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_SCROLL_LIST_SELECT:
    case SE_IO_FOCUS_CHG:
    case SE_IO_SCROLL_LIST_CHG:
#if (defined FEATURE_SKY_SUBLCD && defined FEATURE_COLOR_ID_LCD)
        EQS_StopTimer(EQS_TIMER_1);
        nIndex = EQC_GetScrollLogicalIndex(pEvent->wParam);

        if(nIndex == 0)
        {
            EQS_StartTimer(EQS_TIMER_1, 50, TRUE);
            break;
        }else if(nIndex == NONE_LCD_COLOR + 1)
        {
            nIndex = SKY_GET_IDLCD_COLOR_ALERT();
            if(nIndex == RAINBOW_LCD_COLOR)
            {
                EQS_StartTimer(EQS_TIMER_1, 50, TRUE);
                break;
            }
            else
            {
                //nIndex -= 1;
                SKY_SubBacklight_Confirm(nIndex);
            }
        }else
        {
            nIndex -= 1;
            SKY_SubBacklight_Confirm(nIndex);
        }
        break;

    case SE_TM_TIMER1: 
        s_wColorIndex %= 6;
        SKY_SubBacklight_Confirm(s_wColorIndex);
        s_wColorIndex ++;
#endif
        break;
       
    case SE_SOFTKEY_SAVE:
#ifdef FULL_GROUP_CONTROL
        for(nGroupIndex=0; nGroupIndex < g_nGroup; nGroupIndex++)
        {
            nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC((nGroupIndex * 2) + 1));
            if(nIndex == NUM_COLOR)
            {
                nIndex = NONE_GROUP_ALERT_LCD;
            }else if(nIndex == 0)
            {
                nIndex = RAINBOW_LCD_COLOR;
            }else if(nIndex <=NUM_COLOR -1 && nIndex >=1)
            {
                nIndex -= 1;
            }
            GetGroupInfo(&GroupInfo, nGroupIndex);
            GroupInfo.alert_color = (ID_LCD_COLOR_E)nIndex;
            SetGroupInfo(&GroupInfo, nGroupIndex);
        }
#else
        nGroupIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SGC_GROUP_SCRL));
        nIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SGC_ALERT_SCRL));
        if(nIndex == NUM_COLOR)
        {
            nIndex = NONE_GROUP_ALERT_LCD;
        }else if(nIndex == 0)
        {
            nIndex = RAINBOW_LCD_COLOR;
        }else if(nIndex <=NUM_COLOR-1 && nIndex >=1)
        {
            nIndex -= 1;
        }
        GetGroupInfo(&GroupInfo, nGroupIndex);
        GroupInfo.alert_color = (ID_LCD_COLOR_E)nIndex;
        SetGroupInfo(&GroupInfo, nGroupIndex);
#endif
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

#ifndef FULL_GROUP_CONTROL
    case SE_IO_SCROLL_LIST_SELECT:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SGC_GROUP_SCRL:        // 그룹
            nGroupIndex = (UINT8)(pEvent->dwParam);
            GetGroupInfo(&GroupInfo, nGroupIndex);
            // 그룹
            nIndex = GroupInfo.alert_color;
            if(nIndex == NONE_GROUP_ALERT_LCD)        //착신 조명 없을 때
            {
                EQC_SetScrollPhysicalIndex(EQS_GetNthIOC(SGC_ALERT_SCRL),
                                           NUM_COLOR);
            }
            else
            {
                if(nIndex == RAINBOW_LCD_COLOR)
                    nIndex = 0;
                else if(nIndex <= BLUE_GREEN_LCD_COLOR)//WHITE_LCD_COLOR)
                    nIndex += 1;
                else 
                    nIndex = 0;

                EQC_SetScrollPhysicalIndex(EQS_GetNthIOC(SGC_ALERT_SCRL),
                                           nIndex);
            }
            break;
        }
        break;
#endif

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
			break;
		}
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
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
    case SE_IO_MESGBOX_END:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_APP_IO_MODE:
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif      //SKY_MODEL_ID == SKY_MODEL_B1
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
#if 0
void sInitAutoReceiveDisplay()
{
  HCONTROL hControl;
  UINT8    nCtrl;
  BYTE     nCurrentSetting;
  BYTE*    pszYesNo[2] ;

  pszYesNo[0] = (BYTE*) (BYTE*)SKY_GET_TOKEN(TKN_NAM_YES); 
  pszYesNo[1] = (BYTE*) (BYTE*)SKY_GET_TOKEN(TKN_NAM_NO);

  EQC_StaticText(EQS_NA, EQS_NA, TKN_AUTO_HANDSFREE);    /* 핸즈프리 */  
  
  if(SKY_IS_HANDS_FREE_AUTO_RECEIPT() == TRUE)     nCurrentSetting = 0;
  else       nCurrentSetting = 1;
  hControl = EQC_ScrollCombo(EQS_NA, EQS_NA, H_SIZE, NUM_SCROLLLIST_SIZE, pszYesNo,
                            NUM_SCROLLLIST_SIZE, NUM_SCROLLLIST_SIZE, nCurrentSetting);
  EQC_SET_STYLE(hControl, 
                CS_SCROLL_LIST_ENUMBERATED| CS_SCROLL_LIST_BORDER | EQC_GET_STYLE(hControl));


  EQC_StaticText(EQS_NA, EQS_NA, TKN_AUTO_EARPHONE_MIC); /* 이어폰 마이크 */

  if(SKY_IS_EAR_PHONE_AUTO_RECEIPT() == TRUE)     nCurrentSetting = 0;
  else       nCurrentSetting = 1;  
  hControl = EQC_ScrollCombo(EQS_NA, EQS_NA, H_SIZE, NUM_SCROLLLIST_SIZE, pszYesNo,
                            NUM_SCROLLLIST_SIZE, NUM_SCROLLLIST_SIZE, nCurrentSetting);
  EQC_SET_STYLE(hControl, 
                CS_SCROLL_LIST_ENUMBERATED| CS_SCROLL_LIST_BORDER | EQC_GET_STYLE(hControl));

  EQS_StartFormatter();  
  for(nCtrl = 0; nCtrl <AS_MAX  ; nCtrl ++)
  {
   EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
   EQS_NewLine();
  }
  EQS_EndFormatter();
}


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
void SKY_API SKA_AutoReceive(SKY_EVENT_T* pEvent)
{
    UINT8 nIdx;
    STATIC BOOL s_fEndToRoot = FALSE;
  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      EQS_GetDC(TRUE);
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               (BYTE*)SKY_GET_TOKEN(TKN_AUTO_GET), 
               WA_SHOW_CML|WA_SHOW_HEADER);
      sInitAutoReceiveDisplay();
      EQS_FitCanvasSize();
        break;

    case SE_APP_END :     /* The very last event */
      EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */
        break;

    case SE_IO_SCROLL_LIST_SELECT:
       break;

    case SE_SOFTKEY_SAVE :
       if(EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AS_HANDSFREELIST)) == SELECT_YES)
       {
         SKY_SET_HANDS_FREE_AUTO_RECEIPT(TRUE);
       }
       else if(EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AS_HANDSFREELIST)) == SELECT_NO)
       {
         SKY_SET_HANDS_FREE_AUTO_RECEIPT(FALSE);
       }

       if(EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AS_EARPHONEMICLIST)) == SELECT_YES)
       {
         SKY_SET_EAR_PHONE_AUTO_RECEIPT(TRUE);
       }
       else if(EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AS_EARPHONEMICLIST)) == SELECT_NO)
       {
         SKY_SET_EAR_PHONE_AUTO_RECEIPT(FALSE);
       }
       EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
      break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */

    case SE_IO_SCROLL_LIST_CANCEL:
    case SE_KEY_CLEAR:
        if(SKY_IS_HANDS_FREE_AUTO_RECEIPT()) nIdx = SELECT_YES;   
        else  nIdx = SELECT_NO;  
        
        if(nIdx != EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AS_HANDSFREELIST)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        if(SKY_IS_EAR_PHONE_AUTO_RECEIPT()) nIdx = SELECT_YES;   
        else  nIdx = SELECT_NO;          

        if(nIdx != EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AS_EARPHONEMICLIST)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(SKY_IS_HANDS_FREE_AUTO_RECEIPT()) nIdx = SELECT_YES;   
        else  nIdx = SELECT_NO;  
        
        if(nIdx != EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AS_HANDSFREELIST)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        if(SKY_IS_EAR_PHONE_AUTO_RECEIPT()) nIdx = SELECT_YES;   
        else  nIdx = SELECT_NO;          

        if(nIdx != EQC_GetScrollPhysicalIndex(EQS_GetNthIOC(AS_EARPHONEMICLIST)))
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
    case SE_IO_MESGBOX_END:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*============================================================================= 
 Function:    
     SKA_MainAreaNumber

 Description: 
     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
void SKY_API SKA_MainAreaNumber(SKY_EVENT_T *pEvent)
{
    UINT8   DX, DY;
    INT8    nIndex;
    CONST BYTE* sAreaNumberSel[2];
    BYTE   sAreaNumber[4];
    STATIC BOOL s_fEndToRoot = FALSE;

    sAreaNumberSel[0] = (BYTE*)SKY_GET_TOKEN(TKN_USE);
    sAreaNumberSel[1] = (BYTE*)SKY_GET_TOKEN(TKN_NOT_USE);

    switch(pEvent->EventID)
    {
    /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               (BYTE*)SKY_GET_TOKEN(TKN_AUTO_LOCALNUM), 
               WA_SHOW_CML|WA_SHOW_HEADER);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_INPUT_AUTO_LOCALNUM);
        DX = EQC_GET_DX(EQS_GetNthIOC(AA_TITLE));
        EQC_NumberEdit(DX + 3, EQS_NA, 3, 1, 34, SKY_GET_AREA_CODE());
        DY = EQC_GET_DY(EQS_GetNthIOC(AA_NUMBER));

        if(SKY_IS_AUTO_AREA_CODE())
            nIndex = 0;
        else
            nIndex = 1;
        EQC_RadioButton(EQS_NA, DY, 6, 2, (BYTE **)sAreaNumberSel, nIndex);

        EQC_SET_STYLE(EQS_GetNthIOC(AA_NUMBER), CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);

        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue(EQS_GetNthIOC(AA_NUMBER), sAreaNumber);
        SKY_SET_AREA_CODE(sAreaNumber);
        nIndex = EQC_GetRadioButton(EQS_GetNthIOC(AA_SELECT));

        if(nIndex == 0)
        {
            SKY_SET_AUTO_AREA_CODE(TRUE);
        }
        else
        {
            SKY_SET_AUTO_AREA_CODE(FALSE);
        }
		EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_KEY_CLEAR:
        EQC_GetNumberEditValue(EQS_GetNthIOC(AA_NUMBER), sAreaNumber);
        
        if(STRCMP(sAreaNumber,SKY_GET_AREA_CODE()))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        if(SKY_IS_AUTO_AREA_CODE())
            nIndex = 0;
        else
            nIndex = 1;
        if(EQC_GetRadioButton(EQS_GetNthIOC(AA_SELECT)) != nIndex)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        EQC_GetNumberEditValue(EQS_GetNthIOC(AA_NUMBER), sAreaNumber);
        if(STRCMP(sAreaNumber,SKY_GET_AREA_CODE()))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        
        if(SKY_IS_AUTO_AREA_CODE())
            nIndex = 0;
        else
            nIndex = 1;
        if(EQC_GetRadioButton(EQS_GetNthIOC(AA_SELECT)) != nIndex)
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

	case SE_IO_MESGBOX_END:
		EQS_End();
		break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/*============================================================================= 
 Function:    
     SKA_MainAutoRedial

 Description: 
     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
void SKY_API SKA_MainAutoRedial(SKY_EVENT_T *pEvent)
{
    CONST BYTE* sAutoRedialSel[2];
    INT8   nIndex;
    HCONTROL hControl;
    STATIC BOOL s_fEndToRoot = FALSE;
    
    sAutoRedialSel[0] = (BYTE*)SKY_GET_TOKEN(TKN_USE);
    sAutoRedialSel[1] = (BYTE*)SKY_GET_TOKEN(TKN_NOT_USE);

    switch(pEvent->EventID)
    {
    /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_CONFIRM_NONE, 
               (BYTE*)SKY_GET_TOKEN(TKN_AUTO_REDIAL), 
               WA_SHOW_HEADER|WA_SHOW_CML);

        if(SKY_IS_AUTO_REDIAL())
            nIndex = 0;
        else
            nIndex = 1;
       
        hControl = EQC_RadioButton( 5, 5, 6, 2, (BYTE **)sAutoRedialSel, nIndex);
        EQS_SetIOCActive(hControl);
        EQS_FitCanvasSize();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
        nIndex = EQC_GetRadioButton(EQS_GetNthIOC(0));
        
        if(nIndex == 0)
        {
            SKY_SET_AUTO_REDIAL(TRUE);
        }
        else
        {
            SKY_SET_AUTO_REDIAL(FALSE);
        }
		EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(SKY_IS_AUTO_REDIAL())
            nIndex = 0;
        else
            nIndex = 1;
        if(nIndex != EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_RADIO_BUTTON_CANCEL:
        if(SKY_IS_AUTO_REDIAL())
            nIndex = 0;
        else
            nIndex = 1;
        if(nIndex != EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_End();
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

	case SE_IO_MESGBOX_END:
		EQS_End();
		break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainAutoFnc()
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

#define AUTO_CONTENTS   4


CONST SKY_TOKEN aAutoFnc[AUTO_CONTENTS] =
{
    TKN_AUTO_REDIAL,
    TKN_AUTO_HANDSFREE,
    TKN_AUTO_EARPHONE_MIC,
    TKN_AUTO_LOCALNUM
};

enum tag_AUTO_FNC_E {
    AF_CHBX_CONTENTS = 0,
    AF_STATIC_TEXT,
    AF_NUM_EDIT,
    AF_MAX
};


void SKY_API SKA_MainAutoFnc(SKY_EVENT_T *pEvent)
{
    CONST BYTE* bpAutoFnc[AUTO_CONTENTS];
    INT8   nIndex, DY = 0, DX;
    HCONTROL hControl;
    STATIC BOOL s_fEndToRoot = FALSE;
    BYTE   baAreaNumber[AUTO_CONTENTS];
    STATIC BYTE   s_baAreaNumber[AUTO_CONTENTS];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL fActive = FALSE;
    
    for(nIndex=0; nIndex < AUTO_CONTENTS ; nIndex++)
        bpAutoFnc[nIndex] = (BYTE *)(BYTE*)SKY_GET_TOKEN(aAutoFnc[nIndex]);

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_SELECT_SAVE, 
               (BYTE*)SKY_GET_TOKEN(TKN_AUTO_FUNCTION), 
               WA_SHOW_HEADER|WA_SHOW_CML);

        EQC_CheckBox(2, 5, (BYTE **)bpAutoFnc, AUTO_CONTENTS);
        EQC_SET_STYLE(EQS_GetNthIOC(AF_CHBX_CONTENTS),  (EQC_GET_STYLE(EQS_GetNthIOC(AF_CHBX_CONTENTS))&~CS_CHECK_BOX_BORDER)| 
                                                         CS_CHECK_BOX_HMAXIMIZE                                              |
                                                         CS_CHECK_BOX_LIST);
        DY = 7;
        DY += EQC_GET_DY(EQS_GetNthIOC(AF_CHBX_CONTENTS));

        fActive = TRUE;
        EQC_StaticText(/*12*/8, DY, TKN_INPUT_AUTO_LOCALNUM);
        fActive = FALSE;

        DX = EQC_GET_DX(EQS_GetNthIOC(AF_STATIC_TEXT));
        EQC_NumberEdit(DX + 10, DY, 4, 1, 4, SKY_GET_AREA_CODE());
        EQC_SET_STYLE(EQS_GetNthIOC(AF_NUM_EDIT), CS_NUMBER_EDIT_PLAIN_NUM | \
            CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_IGNORE_AUTOFOCUSING);
        
        hControl = EQS_GetNthIOC(AF_CHBX_CONTENTS);
        if(SKY_IS_AUTO_REDIAL())
        {
            EQC_SetCheckBox(hControl, 0);
        }
        if(SKY_IS_HANDS_FREE_AUTO_RECEIPT())
        {
            EQC_SetCheckBox(hControl, 1);
        }
        if(SKY_IS_EAR_PHONE_AUTO_RECEIPT())
        {
            EQC_SetCheckBox(hControl, 2);
        }
        if(SKY_IS_AUTO_AREA_CODE())
        {
            EQC_SetCheckBox(hControl, 3);
            EQC_SetStaticTextActive(EQS_GetNthIOC(AF_STATIC_TEXT), TRUE);
        }else
        {
            EQC_SetStaticTextActive(EQS_GetNthIOC(AF_STATIC_TEXT), FALSE);
        }
        EQS_SetIOCActive(hControl);
        break;

    case EQE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(AF_CHBX_CONTENTS));        
        break;

    case SE_IO_NEDIT_DONE:
        hControl = EQS_GetNthIOC(AF_CHBX_CONTENTS);
        if(pEvent->dwParam == NER_DOWN_KEY || pEvent->dwParam == NER_OK_KEY )
        {
            EQC_SetCheckBoxIndex(hControl , 0);
            EQC_GetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT) , baAreaNumber);
            
            //if(baAreaNumber[0] == 0)
                //EQC_SetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT), s_baAreaNumber);
        }
        else if(pEvent->dwParam == NER_UP_KEY)
        {
            EQC_SetCheckBoxIndex(hControl , AUTO_CONTENTS - 1);
            EQC_GetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT) , baAreaNumber);
            
            //if(baAreaNumber[0] == 0)
                //EQC_SetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT), s_baAreaNumber);
        }

        EQS_SetIOCActive(EQS_GetNthIOC(AF_CHBX_CONTENTS));

        break;

    case SE_IO_NEDIT_CANCEL:
        if(pEvent->wParam == EQS_GetNthIOC(AF_NUM_EDIT))
        {
            //EQC_SetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT), s_baAreaNumber);
        }
    case SE_IO_NEDIT_NODIGIT:
        hControl = EQS_GetNthIOC(AF_CHBX_CONTENTS);
        EQC_SetCheckBoxIndex(hControl , AUTO_CONTENTS - 1);

        EQS_SetIOCActive(EQS_GetNthIOC(AF_CHBX_CONTENTS));
        break;

    case SE_IO_CHECK_BOX_UP:
    case SE_IO_CHECK_BOX_DOWN:
        nIndex = EQC_GetCheckBoxIndex(EQS_GetNthIOC(AF_CHBX_CONTENTS));
        if(EQC_GetCheckBox(EQS_GetNthIOC(AF_CHBX_CONTENTS),  AUTO_CONTENTS - 1))
        {
            EQS_SetIOCActive(EQS_GetNthIOC(AF_NUM_EDIT));
            EQC_SetStaticTextActive(EQS_GetNthIOC(AF_STATIC_TEXT), TRUE);
        }
        else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(AF_CHBX_CONTENTS));
            EQC_SetStaticTextActive(EQS_GetNthIOC(AF_STATIC_TEXT), FALSE);
        }
        break;

    case SE_IO_CHECK_BOX_CHECK:
    case SE_IO_CHECK_BOX_UNCHECK:
        if(EQC_GetCheckBox(EQS_GetNthIOC(AF_CHBX_CONTENTS),  AUTO_CONTENTS - 1))
        {
            EQC_SetStaticTextActive(EQS_GetNthIOC(AF_STATIC_TEXT), TRUE);
            //EQS_SetIOCActive(EQS_GetNthIOC(AF_NUM_EDIT));
        }
        else
        {
            EQC_SetStaticTextActive(EQS_GetNthIOC(AF_STATIC_TEXT), FALSE);
        }
        break;

    case SE_APP_IO_MODE:
        if(pEvent->wParam == EQS_GetNthIOC(AF_NUM_EDIT))
        {
            EQC_GetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT), s_baAreaNumber);
            //EQC_SetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT), "");
        }
        break;

    case SE_SOFTKEY_SAVE:
        hControl = EQS_GetNthIOC(AF_CHBX_CONTENTS);
        EQC_GetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT) , baAreaNumber);
        if(EQC_GetCheckBox(hControl, 3) && baAreaNumber[0] == 0)
        {
            EQS_SetIOCFocus(EQS_GetNthIOC(AF_NUM_EDIT));
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INPUT_AREA_CODE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            break;    
        }

        SKY_SET_AUTO_REDIAL(EQC_GetCheckBox(hControl, 0));
        SKY_SET_HANDS_FREE_AUTO_RECEIPT(EQC_GetCheckBox(hControl, 1));
        SKY_SET_EAR_PHONE_AUTO_RECEIPT(EQC_GetCheckBox(hControl, 2));
        SKY_SET_AUTO_AREA_CODE(EQC_GetCheckBox(hControl, 3));
        EQC_GetNumberEditValue(EQS_GetNthIOC(AF_NUM_EDIT) , baAreaNumber);
        SKY_SET_AREA_CODE(baAreaNumber);
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                s_fEndToRoot = FALSE;
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            else
                EQS_End();        
        }
        break;

//    case SE_APP_IO_MODE:
//        break;
    case SE_IO_CHECK_BOX_CANCEL:
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SetIOCActive(EQS_GetNthIOC(AF_CHBX_CONTENTS));
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            s_fEndToRoot = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        else
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

