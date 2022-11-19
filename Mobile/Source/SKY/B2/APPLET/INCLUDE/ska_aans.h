/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_aans.h
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
** 2000-12-27   hjcho      Created.
**
** ============================================================================
*/

#ifndef _SKA_AANS_H_
#define _SKA_AANS_H_

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
#define MIN_AUTO_ANSWER_SETTING_TIME   10
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
enum tagAUTOANSWER_DISPLAY_E
{
 AAD_ANIMATION,              //애니메이션
 AAD_DATE,                   //날짜,요일
 AAD_TIME,                   //시간
 AAD_AMPM,                   //AM/PM관련 
 AAD_MODE,                   //모드
 AAD_LINE,                   //점선
 AAD_NEW,                    //New이미지
 //AAD_BACKGROUND,           //바탕이미지
 AAD_COUNT,                  //개수
 AAD_PROGRESSBAR,            //Progress Bar
 AAD_REMAINTIME_TITLE,       //남은시간(제목)
 AAD_REMAINTIME              //남은시간
};


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
void SKY_API SKA_MainAutoAnswer(SKY_EVENT_T* pEvent);
void SKY_API SKA_RecordingAutoAnswer(SKY_EVENT_T* pEvent);
void SKY_API SKA_PlayingUserMent(SKY_EVENT_T* pEvent);
void SKY_API SKA_SetAutoAnswer(SKY_EVENT_T* pEvent);
void SKY_API SKA_RecordUserMent(SKY_EVENT_T* pEvent);
void SKY_API SKA_RECPLAYUserMent(SKY_EVENT_T* pEvent);
extern BOOL GetAutoAnswering(void);
extern void SetAutoAnswering(BOOL fAnswering);
extern void InitAutoAnsDisp(BYTE bMode);
extern void InitAutoAnsDispNoSpace(BYTE bMode);
extern void UpdateAutoAnsDisp(BYTE bMode);
extern void UpdateAutoAnsDispInDebugScreen(BYTE bMode);
extern void UpdateCurrentDateandTimeInAutoAnswer(void);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_AANS_H_ */
