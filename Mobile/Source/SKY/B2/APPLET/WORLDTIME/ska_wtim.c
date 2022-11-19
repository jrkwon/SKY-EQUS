/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_wtim.c
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
** 2001-01-05 white       Modified.
** 2000-12-24 redstar     Created.
** 
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
#include <stdio.h>
#include <string.h>
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/


#define LTM_OFFSET  18

#define DST_SET_INIT_X              4

#define DST_DATA_TABLE_ONOFF        0
#define DST_DATA_TABLE_START_MON    1
#define DST_DATA_TABLE_START_DAY    2
#define DST_DATA_TABLE_END_MON      3
#define DST_DATA_TABLE_END_DAY      4
#define WIDE                        4

#define	FULL_YEAR(y)	((y) + (((y) >= 96) ? 1900 : 2000)) // moved here from Calendar.c

// 2001.05.14 by yhlee and yoursun
#define City_pos_x 5
#define City_pos_y 68
#define TimeDiff_pos_x 3
#define TimeDiff_pos_y 91
#define Time_pos_x 2 
#define Time_pos_y 107 
#define Map_pos_x 3 
#define Map_pos_y 2 
#define Map_VMax 62
#define Max_X 120
#define Sun_pos_x 108 
#define Sun_pos_y 46
#define HorizontalLine_pos_x 3
#define HorizontalLine_pos_y 87
///////////////////////////////////////
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
enum tag_SKA_WORLD_TIME_E {
//    SWT_AREA_SELECT_TITLE,
    SWT_AREA_SCROLL,
    SWT_TIME_ON_DST,
    SWT_TIME_INFO_TITLE,
    SWT_TIME_TEXT_AREA,
    SWT_MAX
}; 

enum tag_SKA_DST_SET_E {
//    SDS_SELECT_AREA_TITLE,
    SDS_SELECT_SCROLL_LIST,
    SDS_ONOFF_CHECK,
    SDS_START_LABEL,
    SDS_START_MON_NUM_EDIT,
    SDS_START_MONTH,
    SDS_START_DAY_NUM_EDIT,
    SDS_START_DAY,
    SDS_END_LABEL,
    SDS_END_MON_NUM_EDIT,
    SDS_END_MONTH,
    SDS_END_DAY_NUM_EDIT,
    SDS_END_DAY,
    SDS_MAX
};
/*
enum tag_SKA_DSTWORLD_TIME_E {
    SDT_AREA_SCROLL,
    SDT_DURING_DST_TEDIT,
    SDT_TIME_TITLE,
    SDT_TIME_TEXT_AREA,
    SDT_MAX
}; */
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
// 세계시각 나라 이름

CONST SKY_TOKEN aCountryName[NUM_COUNTRY_NAME] =
{
    TKN_SEOUL,
    TKN_NEWYORK,
    TKN_DALLAS,
    TKN_DENVER,
    TKN_DIT,
    TKN_LONDON,
    TKN_ROME,
    TKN_LA,
    TKN_RIS,
    TKN_RIO,
    TKN_MANILA,
    TKN_MADRID,    
    TKN_MEXICO,
    TKN_MOSCOW,
    TKN_MONT,
    TKN_BAGHDAD,
    TKN_BANG,
    TKN_BERLIN,
    TKN_BEIRUT,
    TKN_BOSTON,
    TKN_BEIJING,
    TKN_VIENNA,
    TKN_SAMOA,
    TKN_SANFRAN,
    TKN_SIDNEY,
    TKN_SEATTLE,
    TKN_CICAGO,
    TKN_SINGAPORE,
    TKN_ATHENS,
    TKN_AMSTERDAM,
    TKN_WASHIBNGTON,
    TKN_WELLINGTON,
    TKN_GEORGETOWN,
    TKN_CAIRO,
    TKN_THAILAND,
    TKN_TEHRAN,
    TKN_PARIS,  //
    TKN_PHOENIX,    //
    TKN_HANNOVER,
    TKN_HELSINKI,
    TKN_HONOL
};

CONST INT ppCountryLocation[][2] = {
    { 58,28 },   // 서울
    { 93,25 },   // 뉴욕(미국)
    { 87,21 },   // 달라스(미국)
    { 84,27 },   // 덴버(미국)
    { 89,23 },   // 디트로이트(미국)
    { 25,11 },   // 런던(영국)
    { 23,27 },   // 로마(이탈리아)
    { 75,27 },   // LA(미국)
    { 15,27 },   // RIS(포루투칼)
    { 102,47 },    // RIO (브라질)
    { 57,35 },    // 마닐라(필리핀) 
    { 16,26 },    // 마드리드(스페인) 
    { 82,30 },    // 멕시코시티(멕시코)
    { 28,19 },    // 모스코바(러시아)
    { 91,22 },    // 몬트리올(케나다)
    { 31,28 },    // 바그다드(이라크)
    { 50,36 },    // 방콕(타이)
    { 21,21 },    // 베를린(독일) 
    { 28,28 },    // 베이루트(레바논)
    { 95,22 },    // 보스톤(미국)
    { 55,27 },    // 베이징(중국) 
    { 23,25 },  // 비엔나(오스트리아)
    { 72,43 },  // 사모아(호주 오른쪽 섬)
    { 75,23 },  // 샌프란시스코(미국)
    { 65,49 },  // 시드니(호주)
    { 75,22 },  // 시애틀(미국)
    { 87,23 },   // 시카고(미국)
    { 50,39 },   // 싱가폴(싱가폴)
    { 25,27 },   // 아테네(그리스)
    { 19,22 },   // 암스테르담(네덜란드)
    { 92,25 },   // 워싱턴(미국)
    { 72,52 },   // 웰링턴(뉴질랜드)
    { 97,37 },   // 죠지타운(가이아나)
    { 28,30 },   // 카이로(이집트)
    { 57,30 },   // 타이페이(타이완)
    { 33,28 },   // 테헤란(이란)
    { 19,23 },   // 파리(프랑스)
    { 79,26 },   // 피닉스(미국)
    { 21,21 }, // 하노바(독일)
    { 25,18 },  //헬싱키(필란드)
    { 73,34 }   //호놀루루(미국 하와이)
};

CONST SKY_TOKEN aDuring_FROM[NUM_COUNTRY_NAME] =
{
    TKN_NOT_APPLY,//TKN_BLANK,
    TKN_MAY_STRT,
    TKN_MAY_STRT,
    TKN_MAY_STRT,
    TKN_MAY_STRT,
    TKN_MARCH_END,
    TKN_MARCH_END,
    TKN_MAY_STRT,
    TKN_MARCH_END,
    TKN_OCT_MED,
    TKN_NOT_APPLY,//TKN_BLANK,
    TKN_MARCH_END,
    TKN_MAY_STRT,
    TKN_MARCH_END,
    TKN_MAY_STRT,
    TKN_MAY_STRT,
    TKN_NOT_APPLY,//TKN_BLANK,
    TKN_MARCH_END,
    TKN_MARCH_END,
    TKN_MAY_STRT,
    TKN_NOT_APPLY,//TKN_BLANK,
    TKN_MARCH_END,
    TKN_NOT_APPLY,//TKN_BLANK,
    TKN_MAY_STRT,
    TKN_OCT_END,
    TKN_MAY_STRT,
    TKN_MAY_STRT,
    TKN_NOT_APPLY,//TKN_BLANK,
    TKN_MARCH_END,
    TKN_MARCH_END,
    TKN_MAY_STRT,
    TKN_OCT_STRT,
    TKN_MAY_STRT,
    TKN_MAY_END,
    TKN_NOT_APPLY,//TKN_BLANK,
    TKN_MARCH_MED,
    TKN_MARCH_END,
    TKN_NOT_APPLY,//TKN_BLANK,
    TKN_MARCH_END,
    TKN_MARCH_END,
    TKN_NOT_APPLY//TKN_BLANK

};

CONST SKY_TOKEN aDuring_TO[NUM_COUNTRY_NAME] =
{
    TKN_BLANK,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_FEB_END,
    TKN_BLANK,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_STRT,
    TKN_BLANK,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_BLANK,
    TKN_OCT_END,
    TKN_BLANK,
    TKN_OCT_END,
    TKN_MAY_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_BLANK,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_MAY_END,
    TKN_OCT_END,
    TKN_SEP_END,
    TKN_BLANK,
    TKN_SEP_END,
    TKN_OCT_END,
    TKN_BLANK,
    TKN_OCT_END,
    TKN_OCT_END,
    TKN_BLANK,
};

CONST INT aTimeDiff[] = {
	 0+LTM_OFFSET,		-14*2+LTM_OFFSET,	-15*2+LTM_OFFSET,	-16*2+LTM_OFFSET,	-14*2+LTM_OFFSET,   
	-9*2+LTM_OFFSET,	-8*2+LTM_OFFSET,	-17*2+LTM_OFFSET,   -9*2+LTM_OFFSET,	-12*2+LTM_OFFSET,
	-1*2+LTM_OFFSET,	-8*2+LTM_OFFSET,	-15*2+LTM_OFFSET,   -6*2+LTM_OFFSET,	-14*2+LTM_OFFSET,   
	-6*2+LTM_OFFSET,	-2*2+LTM_OFFSET,	-8*2+LTM_OFFSET,	-7*2+LTM_OFFSET,	-14*2+LTM_OFFSET,
	-1*2+LTM_OFFSET,	-8*2+LTM_OFFSET,	-20*2+LTM_OFFSET,	-17*2+LTM_OFFSET,    1*2+LTM_OFFSET,  
	-17*2+LTM_OFFSET,	-15*2+LTM_OFFSET,   -1*2+LTM_OFFSET,	-7*2+LTM_OFFSET,	-8*2+LTM_OFFSET,
	-14*2+LTM_OFFSET,    3*2+LTM_OFFSET,	-14*2+LTM_OFFSET,   -7*2+LTM_OFFSET,	-1*2+LTM_OFFSET, 
	-5*2-1+LTM_OFFSET,  -8*2+LTM_OFFSET,	-16*2+LTM_OFFSET,   -8*2+LTM_OFFSET,	-7*2+LTM_OFFSET,
	-19*2+LTM_OFFSET  
};

CONST INT aDSTTimeDiff[] = {                 // TimeDiff + 1
	 1*2+LTM_OFFSET,	-13*2+LTM_OFFSET,	-14*2+LTM_OFFSET,	-15*2+LTM_OFFSET,	-13*2+LTM_OFFSET,   
	-8*2+LTM_OFFSET,	-7*2+LTM_OFFSET,	-16*2+LTM_OFFSET,	-8*2+LTM_OFFSET,	-11*2+LTM_OFFSET,
	 0*2+LTM_OFFSET,	-7*2+LTM_OFFSET,	-14*2+LTM_OFFSET,   -5*2+LTM_OFFSET,	-13*2+LTM_OFFSET,
	-5*2+LTM_OFFSET,	-1*2+LTM_OFFSET,	-7*2+LTM_OFFSET,	-6*2+LTM_OFFSET,	-13*2+LTM_OFFSET,
	 0*2+LTM_OFFSET,	-7*2+LTM_OFFSET,	-19*2+LTM_OFFSET,	-16*2+LTM_OFFSET,    2*2+LTM_OFFSET,  
	-16*2+LTM_OFFSET,	-14*2+LTM_OFFSET,    0*2+LTM_OFFSET,	-6*2+LTM_OFFSET,	-7*2+LTM_OFFSET,
	-13*2+LTM_OFFSET,    4*2+LTM_OFFSET,	-13*2+LTM_OFFSET,   -6*2+LTM_OFFSET,     0*2+LTM_OFFSET, 
	-4*2-1+LTM_OFFSET,  -7*2+LTM_OFFSET,	-15*2+LTM_OFFSET,   -7*2+LTM_OFFSET,	-6*2+LTM_OFFSET,
	-18*2+LTM_OFFSET
};

LOCAL BYTE *ppCountryName[NUM_COUNTRY_NAME];
LOCAL UINT8 City_index;
LOCAL CHAR Str[36];
LOCAL BOOL IsDay;
LOCAL CHAR szTimeDiff[17];
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
//LOCAL void SKY_API sSKA_DST(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_DSTSet(SKY_EVENT_T *pEvent);
UINT8 EQS_API sGetMaxDay(UINT8 year2, UINT8 month);
INT8 EQS_API sGetLocalTimeOffset(VOID);
BOOL EQS_API sCurCityTimeDisplay(DWORD current_time, UINT8 index);
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainWorldTime()
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
BOOL EQS_API sCurCityTimeDisplay(DWORD current_time, UINT8 index)
{
    UINT8  nStartMon, nStartDay, nEndMon, nEndDay;
    CHAR  szOnInfo[18];
    BYTE    bCurrentMon, bCurrentDay;
    WORD    bCurrentYear;
    BOOL    fDstSet;
    clk_julian_type julian;
    clk_julian_type cmpTime;
    CHAR    Str2[4], sAMPM[3];
	BOOL    DST_ON;
    DWORD   dwStart_time = 1;
    DWORD   dwEnd_time = 2;		
    
    szOnInfo[0] = 0;
    if (current_time >= ONE_YEAR)
    {
        clk_secs_to_julian(current_time, &julian);

        fDstSet = SKY_GET_DST_ON(index);
        nStartMon = SKY_GET_DST_START_MON(index);
        nStartDay = SKY_GET_DST_START_DAY(index);
        nEndMon = SKY_GET_DST_END_MON(index);
        nEndDay = SKY_GET_DST_END_DAY(index);

        cmpTime.day_of_week = julian.day_of_week;	
        cmpTime.hour = julian.hour;	
        cmpTime.minute = julian.minute;	
        cmpTime.second = julian.second;

        cmpTime.year = bCurrentYear = julian.year;
        cmpTime.month = bCurrentMon = julian.month;
        cmpTime.day	= bCurrentDay = julian.day;


        cmpTime.year = bCurrentYear; cmpTime.month = nStartMon; cmpTime.day = nStartDay;
        dwStart_time = clk_julian_to_secs(&cmpTime);

        cmpTime.year = bCurrentYear; cmpTime.month = nEndMon ; cmpTime.day = nEndDay;
        dwEnd_time = clk_julian_to_secs(&cmpTime);

        
        if(fDstSet)     
        {
            if(dwStart_time < dwEnd_time)
            {
                if((dwStart_time < current_time && current_time < dwEnd_time) \
                    || (dwStart_time == current_time || current_time == dwEnd_time))
                {
                    DST_ON = 1;
                    
                    current_time += ((aDSTTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                    clk_secs_to_julian( current_time, &julian );

					if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					else IsDay = 0;

                    if(julian.hour < 12)
						STRCPY(sAMPM,"AM");
					else
					{	
						if(julian.hour != 12)
						{
							julian.hour -= 12;
						}
						STRCPY(sAMPM,"PM");
					}
						sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_ON_DST), 
						julian.month, julian.day, 
						(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));
					
                    tem_sprintf((double)((aDSTTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                    sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_2), Str2,(CHAR *)SKY_GET_TOKEN(TKN_DST));
                }else
                {
                    DST_ON = 0;
					
                    current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                    clk_secs_to_julian( current_time, &julian );

                    if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					else IsDay = 0;

					if(julian.hour < 12)
						STRCPY(sAMPM,"AM");
					else
					{	
						if(julian.hour != 12)
						{
							julian.hour -= 12;
						}
						STRCPY(sAMPM,"PM");
					}

                    sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_OFF_DST), 
						julian.month, julian.day, 
						(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));

                    tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                    sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_1), Str2);
                }
            }else if(dwStart_time > dwEnd_time)
            {
			    if ((nStartMon == nEndMon) && (nStartMon == bCurrentMon) && (bCurrentDay >= nStartDay))
			    {
				    cmpTime.year = bCurrentYear + 1;
				    cmpTime.month = nEndMon;
				    cmpTime.day = nEndDay;
				    dwEnd_time = clk_julian_to_secs(&cmpTime);

				    if( (dwStart_time < current_time && current_time < dwEnd_time) \
                    ||  (dwStart_time == current_time || current_time == dwEnd_time))
				    {
					    DST_ON = 1;
                    
                        current_time += ((aDSTTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                        clk_secs_to_julian( current_time, &julian );

					    if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					    else IsDay = 0;

                        if(julian.hour < 12)
							STRCPY(sAMPM,"AM");
						else
						{	
							if(julian.hour != 12)
							{
								julian.hour -= 12;
							}
							STRCPY(sAMPM,"PM");
						}

					    sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_ON_DST), 
						    julian.month, julian.day, 
						    (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						    julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));
					    
                        tem_sprintf((double)((aDSTTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_2), Str2,(CHAR *)SKY_GET_TOKEN(TKN_DST));
				    }
				    else
				    {
					    DST_ON = 0;
					    
                        current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                        clk_secs_to_julian( current_time, &julian );

                        if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					    else IsDay = 0;

					    if(julian.hour < 12)
							STRCPY(sAMPM,"AM");
						else
						{	
							if(julian.hour != 12)
							{
								julian.hour -= 12;
							}
							STRCPY(sAMPM,"PM");
						}
							sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_OFF_DST), 
						    julian.month, julian.day, 
						    (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						    julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));

                        tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_1), Str2);
				    }
                } 
			    else if ((nStartMon == nEndMon) && (nEndMon == bCurrentMon) && (bCurrentDay <= nEndDay))
			    {
				    cmpTime.year = bCurrentYear - 1;
				    cmpTime.month = nStartMon;
				    cmpTime.day = nStartDay;
				    dwStart_time = clk_julian_to_secs(&cmpTime);

				    if( (dwStart_time < current_time && current_time < dwEnd_time) \
                    || (dwStart_time == current_time || current_time == dwEnd_time))
				    {
                        DST_ON = 1;
                    
                        current_time += ((aDSTTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                        clk_secs_to_julian( current_time, &julian );

					    if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					    else IsDay = 0;

                        if(julian.hour < 12)
							STRCPY(sAMPM,"AM");
						else
						{	
							if(julian.hour != 12)
							{
								julian.hour -= 12;
							}
							STRCPY(sAMPM,"PM");
						}

					    sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_ON_DST), 
						    julian.month, julian.day, 
						    (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						    julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));
					    
                        tem_sprintf((double)((aDSTTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_2), Str2,(CHAR *)SKY_GET_TOKEN(TKN_DST));
                    }
				    else
				    {
					    DST_ON = 0;
					    
                        current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                        clk_secs_to_julian( current_time, &julian );

                        if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					    else IsDay = 0;

					    if(julian.hour < 12)
							STRCPY(sAMPM,"AM");
						else
						{	
							if(julian.hour != 12)
							{
								julian.hour -= 12;
							}
							STRCPY(sAMPM,"PM");
						}
                        sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_OFF_DST), 
						    julian.month, julian.day, 
						    (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						    julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));

                        tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_1), Str2);
				    }
			    }
			    else if ((nStartMon != nEndMon) && (bCurrentMon >= nStartMon) && (bCurrentMon <= 12))
			    {
				    cmpTime.year = bCurrentYear + 1;
				    cmpTime.month = nEndMon;
				    cmpTime.day = nStartDay;
				    dwEnd_time = clk_julian_to_secs(&cmpTime);

				    if( (dwStart_time < current_time && current_time < dwEnd_time) \
                    || (dwStart_time == current_time || current_time == dwEnd_time))
				    {
                        DST_ON = 1;
                    
                        current_time += ((aDSTTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                        clk_secs_to_julian( current_time, &julian );

					    if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					    else IsDay = 0;

                        if(julian.hour < 12)
							STRCPY(sAMPM,"AM");
						else
						{	
							if(julian.hour != 12)
							{
								julian.hour -= 12;
							}
							STRCPY(sAMPM,"PM");
						}

					    sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_ON_DST), 
						    julian.month, julian.day, 
						    (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						    julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));
					    
                        tem_sprintf((double)((aDSTTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_2), Str2,(CHAR *)SKY_GET_TOKEN(TKN_DST));
                        
				    }
				    else
				    {
					    DST_ON = 0;
					    
                        current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                        clk_secs_to_julian( current_time, &julian );

                        if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					    else IsDay = 0;

					    if(julian.hour < 12)
							STRCPY(sAMPM,"AM");
						else
						{	
							if(julian.hour != 12)
							{
								julian.hour -= 12;
							}
							STRCPY(sAMPM,"PM");
						}

                        sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_OFF_DST), 
						    julian.month, julian.day, 
						    (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						    julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));

                        tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_1), Str2);
                    }
			    }
			    else if ((nStartMon != nEndMon) && (bCurrentMon >= 1) && (bCurrentMon <= nEndMon))
			    {
				    cmpTime.year = bCurrentYear - 1;
				    cmpTime.month = nStartMon;
				    cmpTime.day = nStartDay;
				    dwStart_time = clk_julian_to_secs(&cmpTime);

				    if ((dwStart_time < current_time && current_time < dwEnd_time) \
                        || (dwStart_time == current_time || current_time == dwEnd_time))
				    {
                        DST_ON = 1;
                    
                        current_time += ((aDSTTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                        clk_secs_to_julian( current_time, &julian );

					    if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					    else IsDay = 0;

                        if(julian.hour < 12)
							STRCPY(sAMPM,"AM");
						else
						{	
							if(julian.hour != 12)
							{
								julian.hour -= 12;
							}
							STRCPY(sAMPM,"PM");
						}

					    sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_ON_DST), 
						    julian.month, julian.day, 
						    (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						    julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));
					    
                        tem_sprintf((double)((aDSTTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_2), Str2,(CHAR *)SKY_GET_TOKEN(TKN_DST));
                        
				    }
				    else
				    {
					    DST_ON = 0;
					    
                        current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                        clk_secs_to_julian( current_time, &julian );

                        if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					    else IsDay = 0;

					    if(julian.hour < 12)
							STRCPY(sAMPM,"AM");
						else
						{	
							if(julian.hour != 12)
							{
								julian.hour -= 12;
							}
							STRCPY(sAMPM,"PM");
						}

                        sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_OFF_DST), 
						    julian.month, julian.day, 
						    (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						    julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));

                        tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_1), Str2);
                    }
			    }
			    else
			    {
					DST_ON = 0;
					
                    current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                    clk_secs_to_julian( current_time, &julian );

                    if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					else IsDay = 0;

					if(julian.hour < 12)
						STRCPY(sAMPM,"AM");
					else
					{	
						if(julian.hour != 12)
						{
							julian.hour -= 12;
						}
						STRCPY(sAMPM,"PM");
					}

                    sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_OFF_DST), 
						julian.month, julian.day, 
						(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));

                    tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                    sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_1), Str2);
			    }
            }else if ((dwStart_time == dwEnd_time) && (nStartMon == bCurrentMon) && (nStartDay == bCurrentDay))
            {
					DST_ON = 1;
                    
					current_time += ((aDSTTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                    clk_secs_to_julian( current_time, &julian );

                    if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
					else IsDay = 0;

                    if(julian.hour < 12)
						STRCPY(sAMPM,"AM");
					else
					{	
						if(julian.hour != 12)
						{
							julian.hour -= 12;
						}
						STRCPY(sAMPM,"PM");
					}

					sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_ON_DST), 
						julian.month, julian.day, 
						(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
						julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));

                    tem_sprintf((double)((aDSTTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                    sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_2), Str2,(CHAR *)SKY_GET_TOKEN(TKN_DST));
                    //STRCAT(Str, szTimeDiff);            
            }else  
            {
				DST_ON = 0;
                
				current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
                clk_secs_to_julian( current_time, &julian );

				if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
				else IsDay = 0;
                
                if(julian.hour < 12)
					STRCPY(sAMPM,"AM");
				else
				{	
					if(julian.hour != 12)
					{
						julian.hour -= 12;
					}
					STRCPY(sAMPM,"PM");
				}

				sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_OFF_DST), 
					julian.month, julian.day, 
					(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
					julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));

                tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
                sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_1), Str2);
            }
        }else  // not using DST
        {
            //STRCAT(szOnInfo, (BYTE *)SKY_GET_TOKEN(TKN_NOT_DST_SYSTEM));
			DST_ON = 0;
            
			current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
            clk_secs_to_julian( current_time, &julian );
    
			if(julian.hour >= 6 && julian.hour <= 18 ) IsDay = 1;
			else IsDay = 0;

            if(julian.hour < 12)
                STRCPY(sAMPM,"AM");
			else
            {	
				if(julian.hour != 12)
				{
					julian.hour -= 12;
				}
                STRCPY(sAMPM,"PM");
            }

			sprintf((CHAR*)Str, (CHAR *)SKY_GET_TOKEN(TKN_WTIME_INFO_OFF_DST), 
				    julian.month, julian.day, 
					(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
					julian.hour,julian.minute,sAMPM);   // (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM));


            tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
            sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF_1), Str2);
           // STRCAT(Str, szTimeDiff);            

        }
/*		
        current_time += ((aTimeDiff[index]+sGetLocalTimeOffset()) * 30 * 60);
        clk_secs_to_julian( current_time, &julian );
    
		if(julian.hour > 6 && julian.hour < 18 ) IsDay = 1;
		else IsDay = 0;

        if(julian.hour < 12)
            STRCPY(sAMPM,"AM");
		else
        {	
			if(julian.hour != 12)
			{
				julian.hour -= 12;
			}
            STRCPY(sAMPM,"PM");
        }

		if (DST_ON) {
			sprintf((CHAR*)Str, "%2d/%2d(%s) %2d:%2d%s %s",	 
					julian.month, julian.day, 
					(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
					julian.hour,julian.minute,sAMPM,(CHAR *)SKY_GET_TOKEN(TKN_DST));
		}
		else { 
			sprintf((CHAR*)Str, "%2d/%2d(%s) %2d:%2d%s", 
					julian.month, julian.day, 
					(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
					julian.hour,julian.minute,sAMPM);
		}

        tem_sprintf((double)((aTimeDiff[index]+sGetLocalTimeOffset())/2.0), Str2, 10);
        sprintf(szTimeDiff,(BYTE *)SKY_GET_TOKEN(TKN_TIME_DIFF), Str2);
*/	
        return TRUE;
    }else
    {
        return FALSE;
    }
}

#define TO_RIGHT_WORLD_TIME     1
#define TO_LEFT_WORLD_TIME      2

LOCAL BYTE SKY_API sSearchSideCountry(BYTE  country, BYTE   where)
{
    BYTE    bIndex, bSaveIndex;
    INT    wDistance, wMinDistance = 0;

    bSaveIndex = country;
        
    if(where == TO_RIGHT_WORLD_TIME)
    {
        if(country == 9)
        {
            bSaveIndex = 8;   
        }else
        {

            wMinDistance = 100;
            for(bIndex = 0; bIndex < NUM_COUNTRY_NAME ; bIndex++)
            {
                if(bIndex == country)
                    continue;
        
                wDistance = (ppCountryLocation[bIndex][0] - ppCountryLocation[country][0]);
                if(wDistance > 0 && wDistance < wMinDistance)
                {
                    wMinDistance = wDistance;
                    bSaveIndex = bIndex;
                }
            }
        }
    }else if(where == TO_LEFT_WORLD_TIME)
    {
        if(country == 8)    //가장 왼쪽 나라 
        {
            bSaveIndex = 9;
        }else
        {

            wMinDistance = -100;
            for(bIndex = 0; bIndex < NUM_COUNTRY_NAME ; bIndex++)
            {
                if(bIndex == country)
                    continue;
        
                wDistance = (ppCountryLocation[bIndex][0] - ppCountryLocation[country][0]);
                if( wDistance < 0 && wDistance > wMinDistance)
                {
                    wMinDistance = wDistance;
                    bSaveIndex = bIndex;
                }
            }    
        }
    }
    return bSaveIndex;
}

void SKY_API SKA_MainWorldTime(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hEndControl;
    STATIC DWORD current_time;
    UINT8   nIndex, nTemp;
	HCONTROL hControl;
	static UINT8 vCurrent_pos_x,vCurrent_pos_y;
    static UINT8 hCurrent_pos_x,hCurrent_pos_y;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */ 
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_MOVE_DST,             
                SKY_GET_TOKEN(TKN_WORLD_TIME), 
                WA_SHOW_HEADER | WA_SHOW_CML);
		
        City_index = 0; 
		current_time = SKY_GetLocalTime();

        if(!SKY_IS_VALID_LOCAL_TIME(current_time))
        {
            s_hEndControl =
                EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_KNOWN_TIME), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;        
        }

        for(nIndex=0; nIndex < NUM_COUNTRY_NAME ; nIndex++) 
			ppCountryName[nIndex] = (BYTE *)SKY_GET_TOKEN(aCountryName[nIndex]);

		vCurrent_pos_x = ppCountryLocation[City_index][0];
		vCurrent_pos_y = Map_pos_y;
		hCurrent_pos_x = Map_pos_x;
		hCurrent_pos_y = ppCountryLocation[City_index][1];

        if(!sCurCityTimeDisplay( current_time, 0))
        {
            s_hEndControl =
                EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_KNOWN_TIME), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;        
        }else
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_GetNthIOC(SWT_TIME_TEXT_AREA),EQS_NA);
        }

		hControl = EQC_ScrollList(City_pos_x, City_pos_y, 17, 1, ppCountryName, \
                                   NUM_COUNTRY_NAME, \
                                   NUM_COUNTRY_NAME, City_index);
        EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*16+6);

		EQC_SET_STYLE(hControl, CS_SCROLL_LIST_BORDER | CS_SCROLL_LIST_ENUMBERATED|CS_SCROLL_LIST_NO_CML);
        EQC_SetShowScrollBar( hControl, FALSE);
        EQS_SetIOCActive(hControl);
        break;
  
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;

    case SE_IO_SCROLL_LIST_LEFT:
        hControl = EQS_GetNthIOC(SWT_AREA_SCROLL);
        nTemp = EQC_GetScrollLogicalIndex(hControl);
        City_index = sSearchSideCountry(nTemp,TO_LEFT_WORLD_TIME);
        EQC_SetScrollLogicalIndex(hControl, City_index);
		vCurrent_pos_x = ppCountryLocation[City_index][0];
		hCurrent_pos_y = ppCountryLocation[City_index][1];
        EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);
        EQS_SetIOCActive(EQS_GetNthIOC(SWT_AREA_SCROLL));
        break;

    case SE_IO_SCROLL_LIST_RIGHT:
        hControl = EQS_GetNthIOC(SWT_AREA_SCROLL);
        nTemp = EQC_GetScrollLogicalIndex(hControl);
        City_index = sSearchSideCountry(nTemp,TO_RIGHT_WORLD_TIME);
        EQC_SetScrollLogicalIndex(hControl, City_index);
		vCurrent_pos_x = ppCountryLocation[City_index][0];
		hCurrent_pos_y = ppCountryLocation[City_index][1];
        EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);
        EQS_SetIOCActive(EQS_GetNthIOC(SWT_AREA_SCROLL));
        break;

	case SE_APP_DRAW : 
        if(SKY_IS_VALID_LOCAL_TIME(current_time))
        {
		    EQS_PutBitmap(Map_pos_x,Map_pos_y,&g_BmpWorldMap);
		    
		    Line(vCurrent_pos_x,Map_pos_y+1,vCurrent_pos_x,62+Map_pos_y-2);   // vertical
		    Line(Map_pos_x+1,hCurrent_pos_y+Map_pos_y,122+Map_pos_x-2,hCurrent_pos_y+Map_pos_y);  // horizontal

	        current_time = SKY_GetLocalTime();
        	    
		    if(!sCurCityTimeDisplay(current_time, City_index))
            {
                s_hEndControl =
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_KNOWN_TIME), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                break;        
            }
		    //EQS_TextOutStr(City_pos_x,City_pos_y,"                    ");
		    //EQS_TextOutStr(City_pos_x,City_pos_y,ppCountryName[City_index]);
		    EQS_TextOutStr(TimeDiff_pos_x,TimeDiff_pos_y,"                    ");
		    EQS_TextOutStr(TimeDiff_pos_x,TimeDiff_pos_y,szTimeDiff);
		    EQS_TextOutStr(Time_pos_x,Time_pos_y,"                            ");
		    EQS_TextOutStr(Time_pos_x,Time_pos_y,Str);
/*            
            //szTimeDiff,Str NULL로 초기화하세여..
            hControl = EQC_StaticText(TimeDiff_pos_x,TimeDiff_pos_y,TKN_BLANK);
            EQC_SetStaticText(hControl, szTimeDiff);
            EQC_SetStaticTextColor(hControl,GRAY);
            hControl = EQC_StaticText(Time_pos_x,Time_pos_y,TKN_BLANK);
            EQC_SetStaticText(hControl, Str);
            EQC_SetStaticTextColor(hControl,GRAY);
*/
		    if ( IsDay == 1) {
			    EQS_PutBitmap(Sun_pos_x, Sun_pos_y,&g_BmpWTimeSun);
		    }
		    else {
			    EQS_PutBitmap(Sun_pos_x, Sun_pos_y,&g_BmpWTimeMoon);
		    }
            EQS_PutBitmap(HorizontalLine_pos_x, HorizontalLine_pos_y,&g_BmpHorizontalLine);
        }
		break;

    case SE_APP_RESUME:
 		EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);
        EQS_SetIOCActive(EQS_GetNthIOC(SWT_AREA_SCROLL));
        break;


	case SE_IO_SCROLL_LIST_CHG :
		City_index = pEvent->dwParam;
		
		vCurrent_pos_x = ppCountryLocation[City_index][0];
		hCurrent_pos_y = ppCountryLocation[City_index][1];
		EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);

		break;

	case SE_SOFTKEY_CANCEL:
        EQS_End();
        break;

	case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
		break;
    case SE_IO_SCROLL_LIST_SELECT:
        EQS_SetIOCActive(EQS_GetNthIOC(SWT_AREA_SCROLL));
        break;

    case SE_SOFTKEY_DST:         //DST
        EQS_START_CHILD_EVENT(sSKA_DSTSet, SE_APP_START, City_index , 0);              
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
       
    case SE_TM_TIMER:
        break;


    case SE_IO_MESGBOX_END:
        if( pEvent->wParam == s_hEndControl)
            EQS_End();

        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
/*============================================================================= 
 Function:    
     sSKA_DSTSet

 Description: 
    #define DST_DATA_TABLE_ONOFF        0
    #define DST_DATA_TABLE_START_MON    1
    #define DST_DATA_TABLE_START_DAY    2
    #define DST_DATA_TABLE_END_MON      3
    #define DST_DATA_TABLE_END_DAY      4

     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
#define SKY_WORLD_TIME_COMBO_STYLE   CS_SCROLL_LIST_DEFAULT| \
                                    CS_SCROLL_LIST_COMBO | \
                                    CS_SCROLL_LIST_ENUMBERATED 

LOCAL void SKY_API sSKA_DSTSet(SKY_EVENT_T *pEvent)
{
    UINT8   nDX, nDY, nMaxStartDay, nMaxEndDay, nArea;
    UINT8   aStartMonth[3], aStartDay[3], aEndMonth[3], aEndDay[3]; 
    UINT8   nStartMonth, nStartDay, nEndMonth, nEndDay;
    clk_julian_type	TM;
    DWORD dwCurrentTime;
    STATIC HCONTROL s_hEndControl, s_hMsgControl;
    STATIC BOOL s_fEndKey = FALSE;
    CONST BYTE* sHow2Dst[1];
    HCONTROL hControl;

    sHow2Dst[0] = SKY_GET_TOKEN(TKN_WTIME_DST_ON);
    //sHow2Dst[1] = SKY_GET_TOKEN(TKN_WTIME_DST_ON);
        
    switch(pEvent->EventID)
    {
    case SE_APP_START : // The very first event 
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_SAVE, 
                SKY_GET_TOKEN(TKN_DAYLIGHT_SAVING_SET), 
                WA_SHOW_HEADER | WA_SHOW_CML);
        
        dwCurrentTime = SKY_GetLocalTime();

        if (dwCurrentTime < ONE_YEAR)
        {
            s_hEndControl =
                EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_KNOWN_TIME), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;        
        }

//       EQC_StaticText(DST_SET_INIT_X, 2, TKN_SELECT_COUNTRY);     //지역 선택
//        nDY = EQC_GET_DY(EQS_GetNthIOC(SDS_SELECT_AREA_TITLE)) + WIDE;

        EQC_ScrollCombo(DST_SET_INIT_X, 4, 17, 1, ppCountryName,    
            NUM_COUNTRY_NAME, NUM_COUNTRY_NAME, pEvent->wParam);
        EQC_SET_STYLE(EQS_GetNthIOC(SDS_SELECT_SCROLL_LIST), SKY_WORLD_TIME_COMBO_STYLE);
        nDY = EQC_GET_DY(EQS_GetNthIOC(SDS_SELECT_SCROLL_LIST)) + WIDE;
        nDY += 6;
        
        //EQC_RadioButton(DST_SET_INIT_X, nDY, 6, 2, (BYTE **)sHow2Dst, SKY_GET_DST_ON(pEvent->wParam));
        EQC_CheckBox( DST_SET_INIT_X, nDY, (BYTE **)sHow2Dst, 1);
        
        if(SKY_GET_DST_ON(pEvent->wParam))
            EQC_SetCheckBox(EQS_GetNthIOC(SDS_ONOFF_CHECK), 0);
        else
            EQC_ResetCheckBox(EQS_GetNthIOC(SDS_ONOFF_CHECK), 0);
        //EQC_StaticText(DST_SET_INIT_X, nDY, TKN_SCHEDULE_START_END_TITLE);   //시작과 종료
        nDY += EQC_GET_DY(EQS_GetNthIOC(SDS_ONOFF_CHECK)) + WIDE;
        nDY += 2;

        EQC_StaticText(DST_SET_INIT_X, nDY, TKN_SCHEDULE_START_TIME);        //시작
        nDX = EQC_GET_DX(EQS_GetNthIOC(SDS_START_LABEL)) + WIDE*2;
        
        sprintf(aStartMonth, "%2d", SKY_GET_DST_START_MON(pEvent->wParam)); 
        hControl =EQC_NumberEdit(nDX, nDY, 2, 1, 2, aStartMonth);   
        EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_BORDER | CS_NUMBER_EDIT_CURSOR|CS_NUMBER_EDIT_PLAIN_NUM);        
        nDX += EQC_GET_DX(EQS_GetNthIOC(SDS_START_MON_NUM_EDIT)) + WIDE;
        EQC_StaticText(nDX, nDY, TKN_DAT_MONTH);                  //월
        nDX += EQC_GET_DX(EQS_GetNthIOC(SDS_START_MONTH)) + WIDE;
        sprintf(aStartDay, "%2d", SKY_GET_DST_START_DAY(pEvent->wParam)); 
        hControl= EQC_NumberEdit(nDX, nDY, 2, 1, 2, aStartDay);    
        EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_BORDER | CS_NUMBER_EDIT_CURSOR|CS_NUMBER_EDIT_PLAIN_NUM);        
        nDX += EQC_GET_DX(EQS_GetNthIOC(SDS_START_DAY_NUM_EDIT)) + WIDE;
        EQC_StaticText(nDX, nDY, TKN_DAT_DAY);                    //일
        nDY += EQC_GET_DY(EQS_GetNthIOC(SDS_START_DAY)) + WIDE;
        nDY += 2;

        EQC_StaticText(DST_SET_INIT_X, nDY, TKN_SCHEDULE_END_TIME);          //종료
        nDX = EQC_GET_DX(EQS_GetNthIOC(SDS_END_LABEL)) + WIDE*2;
        sprintf(aEndMonth, "%2d", SKY_GET_DST_END_MON(pEvent->wParam)); 
        hControl= EQC_NumberEdit(nDX, nDY, 2, 1, 2, aEndMonth); 
        EQC_SET_STYLE(hControl,CS_NUMBER_EDIT_BORDER |  CS_NUMBER_EDIT_CURSOR|CS_NUMBER_EDIT_PLAIN_NUM);        
        nDX += EQC_GET_DX(EQS_GetNthIOC(SDS_END_MON_NUM_EDIT)) + WIDE;
        EQC_StaticText(nDX, nDY, TKN_DAT_MONTH);                  //월
        nDX += EQC_GET_DX(EQS_GetNthIOC(SDS_END_MONTH)) + WIDE;
        sprintf(aEndDay, "%2d", SKY_GET_DST_END_DAY(pEvent->wParam)); 
        hControl = EQC_NumberEdit(nDX, nDY, 2, 1, 2, aEndDay);    
        EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_BORDER | CS_NUMBER_EDIT_CURSOR|CS_NUMBER_EDIT_PLAIN_NUM);        
        nDX += EQC_GET_DX(EQS_GetNthIOC(SDS_END_DAY_NUM_EDIT)) + WIDE;
        EQC_StaticText(nDX, nDY, TKN_DAT_DAY);                    //일
        break;

    case SE_SOFTKEY_SAVE:
        nArea = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SDS_SELECT_SCROLL_LIST));
        EQC_GetNumberEditValue(EQS_GetNthIOC(SDS_START_MON_NUM_EDIT), aStartMonth);
        EQC_GetNumberEditValue(EQS_GetNthIOC(SDS_START_DAY_NUM_EDIT), aStartDay);
        EQC_GetNumberEditValue(EQS_GetNthIOC(SDS_END_MON_NUM_EDIT), aEndMonth);
        EQC_GetNumberEditValue(EQS_GetNthIOC(SDS_END_DAY_NUM_EDIT), aEndDay);
        dwCurrentTime = SKY_GetLocalTime();
        clk_secs_to_julian( dwCurrentTime, &TM );
        
        nStartMonth = EQS_atof(aStartMonth);
        nStartDay = EQS_atof(aStartDay);
        nEndMonth = EQS_atof(aEndMonth);
        nEndDay = EQS_atof(aEndDay);

        if((nStartMonth < 1 || nStartMonth > 12) || (nEndMonth < 1 || nEndMonth > 12)) 
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_INVALID_DST_DAY), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;
        }
        nMaxStartDay = sGetMaxDay((TM.year - 1900)%100, nStartMonth);
        nMaxEndDay = sGetMaxDay((TM.year - 1900)%100, nEndMonth);
        if((nStartDay < 1 || nStartDay > nMaxStartDay)
                    || (nEndDay < 1 || nEndDay > nMaxEndDay))
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_INVALID_DST_DAY), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;        
        }
        SKY_SET_DST_ON(nArea, EQC_GetCheckBox(EQS_GetNthIOC(SDS_ONOFF_CHECK), 0));
        SKY_SET_DST_START_MON(nArea, nStartMonth);
        SKY_SET_DST_START_DAY(nArea, nStartDay);
        SKY_SET_DST_END_MON(nArea, nEndMonth);
        SKY_SET_DST_END_DAY(nArea, nEndDay);

        s_hMsgControl =
            EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
     
    case SE_IO_NEDIT_CANCEL:
        nArea = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SDS_SELECT_SCROLL_LIST));
        if(pEvent->wParam == EQS_GetNthIOC(SDS_START_MON_NUM_EDIT))
        {
            sprintf(aStartMonth, "%2d", SKY_GET_DST_START_MON(nArea)); 
            EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_START_MON_NUM_EDIT),aStartMonth);
        }
        else if(pEvent->wParam == EQS_GetNthIOC(SDS_START_DAY_NUM_EDIT))
        {
            sprintf(aStartDay, "%2d", SKY_GET_DST_START_DAY(nArea)); 
            EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_START_DAY_NUM_EDIT), aStartDay);
        }
        else if(pEvent->wParam == EQS_GetNthIOC(SDS_END_MON_NUM_EDIT))
        {
            sprintf(aEndMonth, "%2d", SKY_GET_DST_END_MON(nArea)); 
            EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_END_MON_NUM_EDIT), aEndMonth);
        }
        else if(pEvent->wParam ==  EQS_GetNthIOC(SDS_END_DAY_NUM_EDIT))
        {
            sprintf(aEndDay, "%2d", SKY_GET_DST_END_DAY(nArea)); 
            EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_END_DAY_NUM_EDIT),aEndDay);
        }
        break;

    case SE_APP_IO_MODE:
        if(pEvent->wParam == EQS_GetNthIOC(SDS_START_MON_NUM_EDIT))
        {
            EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_START_MON_NUM_EDIT), "");
        }
        else if(pEvent->wParam == EQS_GetNthIOC(SDS_START_DAY_NUM_EDIT))
        {
            EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_START_DAY_NUM_EDIT), "");
        }
        else if(pEvent->wParam == EQS_GetNthIOC(SDS_END_MON_NUM_EDIT))
        {
            EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_END_MON_NUM_EDIT), "");
        }
        else if(pEvent->wParam ==  EQS_GetNthIOC(SDS_END_DAY_NUM_EDIT))
        {
            EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_END_DAY_NUM_EDIT), "");
        }
        break;
       
    case SE_IO_FOCUS_CHG:
        if(pEvent->wParam == EQS_GetNthIOC(SDS_ONOFF_CHECK))
        {
            if(EQC_GetCheckBox(EQS_GetNthIOC(SDS_ONOFF_CHECK), 0))
                EQS_SetWindowCML(CL_NONE_DISSOLVE_SAVE);
            else
                EQS_SetWindowCML(CL_NONE_SET_SAVE);
        }else
        {
            EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
        }
        break;

    case SE_IO_SCROLL_LIST_CHG: 
	case SE_IO_SCROLL_LIST_CANCEL:
        nArea = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SDS_SELECT_SCROLL_LIST));
        sprintf(aStartMonth, "%2d", SKY_GET_DST_START_MON(nArea)); 
        sprintf(aStartDay, "%2d", SKY_GET_DST_START_DAY(nArea)); 
        sprintf(aEndMonth, "%2d", SKY_GET_DST_END_MON(nArea)); 
        sprintf(aEndDay, "%2d", SKY_GET_DST_END_DAY(nArea)); 

        //EQC_SetRadioButton(EQS_GetNthIOC(SDS_ONOFF_CHECK), SKY_GET_DST_ON(nArea));
        if(SKY_GET_DST_ON(nArea))
            EQC_SetCheckBox(EQS_GetNthIOC(SDS_ONOFF_CHECK), 0);
        else
            EQC_ResetCheckBox(EQS_GetNthIOC(SDS_ONOFF_CHECK), 0);

        EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_START_MON_NUM_EDIT), aStartMonth);
        EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_START_DAY_NUM_EDIT), aStartDay);
        EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_END_MON_NUM_EDIT), aEndMonth);
        EQC_SetNumberEditValue(EQS_GetNthIOC(SDS_END_DAY_NUM_EDIT), aEndDay);
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
			EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_End();
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndKey)
        {
            s_fEndKey = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
        }else
        {
            EQS_End();
        }
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndKey = TRUE;
			EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_APP_END :   // The very last event  
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: // child applet is started 
        break;
        
    case SE_APP_RESUME: // child applet is ended 
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hEndControl)
            EQS_End();
        else if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndKey)
            {
                s_fEndKey = FALSE;
                EQS_END_TO(EQS_POP_TO_ROOT);
            }else
            {
                EQS_End();
            }        
        }
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

UINT8 EQS_API sGetMaxDay(UINT8 year2, UINT8 month)
{
    CONST UINT8	nMaxDay[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    UINT8 nYear = FULL_YEAR(year2);

    if(month != 2)
        return nMaxDay[month - 1];

    if(nYear % 4 == 0)
        if (nYear % 400 == 0 || nYear % 100 != 0)
			return 29;

	return 28;
}

INT8 EQS_API sGetLocalTimeOffset(VOID)
{
    INT   nDiffTime;	

    db_items_value_type dbi; /* DB items */
    db_get( DB_LTM_OFF, &dbi ); /* time zone */
    nDiffTime = dbi.ltm_off;

    if( nDiffTime < 0x20 ) {      /* ltm_off is a 6 bit signed integer */
	    return - nDiffTime;          /* ltm_off positive */
    }
    else
    {
	    return ( 0x40 - nDiffTime );   /* ltm_off negative */
    }
}


/*============================================================================= 
 Function:    
     sSKA_DST

 Description: 
    SDT_AREA_SCROLL,
    SDT_DURING_DST_TEDIT,
    SDT_TIME_TEXT_AREA,
    SDT_MAX

     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
/*
#if 0
LOCAL void SKY_API sSKA_DST(SKY_EVENT_T *pEvent)
{
    HCONTROL hControl;
    STATIC HCONTROL s_hEndControl;
    UINT8 nIndex, nCtrl;
    CHAR szBuf[38], szDiff[21], szNumber[10], szDuring[19];
    DWORD current_time;
    clk_julian_type julian;
    CHAR sAMPM[3];

    switch(pEvent->EventID)
    {
    case SE_APP_START : // The very first event 
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_DAYLIGHT_SAVING_SET), 
                WA_SHOW_HEADER);

        current_time = SKY_GetLocalTime() + aDSTTimeDiff[nIndex] * 30 * 60;

        if (current_time >= ONE_YEAR)
        {
            clk_secs_to_julian(current_time, &julian);
            if(julian.hour < 12)
                STRCPY(sAMPM,"AM");
			else
            {	
				if(julian.hour != 12)
				{
					julian.hour -= 12;
				}
                STRCPY(sAMPM,"PM");
            }

            sprintf(szBuf, "%2d/%2d(%s) %2d:%2d %s ", 
                julian.month, julian.day, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
                julian.hour, julian.minute, sAMPM);
            
            tem_sprintf(aDSTTimeDiff[0]/1.0 ,szNumber,10);
            sprintf(szDiff, SKY_GET_TOKEN(TKN_TIME_DIFF), szNumber);
            strcat(szBuf, szDiff);

        }else
        {
            s_hEndControl =
                EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_KNOWN_TIME), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            
            break;
        }

        if(aDuring_TO[pEvent->wParam] == TKN_BLANK )
            sprintf(szDuring, "%s", SKY_GET_TOKEN(aDuring_FROM[pEvent->wParam]));
        else
            sprintf(szDuring, "%s ~ %s", SKY_GET_TOKEN(aDuring_FROM[pEvent->wParam]), SKY_GET_TOKEN(aDuring_TO[pEvent->wParam]));
        
        for(nIndex=0; nIndex < NUM_COUNTRY_NAME ; nIndex++)
            ppCountryName[nIndex] = (BYTE *)SKY_GET_TOKEN(aCountryName[nIndex]);

        EQC_ScrollList(EQS_NA, EQS_NA, 16, 1, ppCountryName, 
            NUM_COUNTRY_NAME, NUM_COUNTRY_NAME, pEvent->wParam);

        EQC_TextEdit(EQS_NA, EQS_NA, 19, 1, TEM_KOREAN, 19, szDuring,0);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_TIME_INFO);
        EQC_TextEdit(EQS_NA, EQS_NA, 19, 2, TEM_KOREAN, 38, szBuf,0);

        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < SDT_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        hControl = EQS_GetNthIOC(SDT_AREA_SCROLL);

        EQC_SET_STYLE(hControl, CS_SCROLL_LIST_DEFAULT);
        
        EQC_SET_STYLE(EQS_GetNthIOC(SDT_DURING_DST_TEDIT), CS_TEXT_EDIT_READONLY | CS_TEXT_EDIT_BORDER);
        EQC_SET_STYLE(EQS_GetNthIOC(SDT_TIME_TEXT_AREA), CS_TEXT_EDIT_READONLY | CS_TEXT_EDIT_BORDER);
        
        EQS_SetIOCActive(hControl);

        break;
        
    case SE_APP_END :   // The very last event 
        EQS_ReleaseDC();
        break;
      
    case SE_SOFTKEY_CANCEL:
    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_End();
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        EQS_SetIOCActive(EQS_GetNthIOC(SDT_AREA_SCROLL));
        break;
        
    case SE_APP_SUSPEND: // child applet is started 
        break;
        
    case SE_APP_RESUME: // child applet is ended 
        break;   
        
    case SE_TM_TIMER:
        break;

    case SE_IO_SCROLL_LIST_CHG:
    
        hControl = EQS_GetNthIOC(SDT_AREA_SCROLL);
        nIndex = EQC_GetScrollLogicalIndex(hControl);

        current_time = SKY_GetLocalTime() + aDSTTimeDiff[nIndex] * 30 * 60;
        
        if (current_time >= ONE_YEAR)
        {
            clk_secs_to_julian(current_time, &julian);

            if(julian.hour < 12)
                STRCPY(sAMPM,"AM");
			else
            {	
				if(julian.hour != 12)
				{
					julian.hour -= 12;
				}
                STRCPY(sAMPM,"PM");
            }

            sprintf(szBuf, "%2d/%2d(%s) %2d:%2d %s ", 
                julian.month, julian.day, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
                julian.hour, julian.minute, sAMPM);  

            tem_sprintf(aDSTTimeDiff[nIndex]/2.0 ,szNumber,10);
            sprintf(szDiff, SKY_GET_TOKEN(TKN_TIME_DIFF), szNumber);
            strcat(szBuf, szDiff);

        }else
        {
            s_hEndControl =
                EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_KNOWN_TIME), MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
            
            break;
        }
        EQC_SetTextEditData(EQS_GetNthIOC(SDT_TIME_TEXT_AREA), szBuf);
        
        if(aDuring_TO[nIndex] == TKN_BLANK )
            sprintf(szDuring, "%s", SKY_GET_TOKEN(aDuring_FROM[nIndex]));
        else
            sprintf(szDuring, "%s ~ %s", SKY_GET_TOKEN(aDuring_FROM[nIndex]), SKY_GET_TOKEN(aDuring_TO[nIndex]));
        
        EQC_SetTextEditData(EQS_GetNthIOC(SDT_DURING_DST_TEDIT), szDuring);
        
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL,EQS_NA);
        break;

    case SE_IO_MESGBOX_END:
        if( pEvent->wParam == s_hEndControl)
            EQS_End();

        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif
*/