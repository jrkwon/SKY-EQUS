/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_vmemo.h
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
** 2000-12-12   hjcho      Created.
**
** ============================================================================
*/

#ifndef _SKA_VMEMO_H_
#define _SKA_VMEMO_H_

#ifdef __cplusplus
extern "C" {
#endif
  
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "ska_vcmm.h"  
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define VM_MIN_REMAINED_BYTE     1024
/*
**  This definition is used in progree bar 
**  which reports available recording time.
*/
#define VM_MAX_PROGRESS             TOTAL_REC_VOICE_MEMO_TIME//270
#define VM_MIN_PROGRESS             0

#define USERMENT_MAX_PROGRESS       TOTAL_REC_USER_MENT_TIME//30

#define VM_ONE_SEC                  1000
#define VM_TWO_SEC                  2000
#define VM_THREE_SEC                3000

#define VM_ONE                      1
#define VM_TWO                      2
#define VM_THREE                    3

#define VM_SEC_PER_COLUME           3
#define VM_PBAR_SIZE                105
#define VM_PBAR_HEIGHT              4
#define VM_PBAR_STEPSIZE            1
#define VM_MAX_REMAINTIME           269
    


/* 녹음/재생 관련 화면 구성에 사용되는 좌표 정의 */
#define VM_RECORD_ANIMATION_XPOS          2
#define VM_RECORD_ANIMATION_YPOS          (25 - SKY_ANNUNCIATORS_HEIGHT) //3

#define VM_NEW_RECORD_ANIMATION_XPOS      VM_RECORD_ANIMATION_XPOS
#define VM_NEW_RECORD_ANIMATION_YPOS      VM_RECORD_ANIMATION_YPOS

#define VM_NEW_PLAY_ANIMATION_XPOS        VM_NEW_RECORD_ANIMATION_XPOS
#define VM_NEW_PLAY_ANIMATION_YPOS        VM_NEW_RECORD_ANIMATION_YPOS

#define VM_RECORD_BACKGROUND_XPOS         4//3
#define VM_RECORD_BACKGROUND_YPOS         (82 - SKY_ANNUNCIATORS_HEIGHT)//63//43//41 //45

#define VM_RECORD_NUMBER_XPOS              (VM_RECORD_REC_NUM_IMAGE_XPOS + 32)//32
#define VM_RECORD_NUMBER_YPOS              (VM_RECORD_REC_NUM_IMAGE_YPOS - 2)

#define VM_RECORD_PBAR_XPOS               11
#define VM_RECORD_PBAR_YPOS               (110 - SKY_ANNUNCIATORS_HEIGHT)//89

#define VM_RECORD_REMAIN_TIME_TITLE_XPOS  17 //VM_RECORD_REMAINTIME_XPOS - 54

#define VM_RECORD_REMAINTIME_XPOS         VM_RECORD_REMAIN_TIME_TITLE_XPOS + EQS_TextWidth(SKY_GET_TOKEN(TKN_VMEMO_TIME))
#define VM_RECORD_REMAINTIME_YPOS         (120 - SKY_ANNUNCIATORS_HEIGHT)

#define VM_RECORD_RECORDINGTIME_XPOS      73//75
#define VM_RECORD_RECORDINGTIME_YPOS      (55 - SKY_ANNUNCIATORS_HEIGHT)

#define VM_RECORD_REC_NUM_IMAGE_XPOS      10//(VM_RECORD_BACKGROUND_XPOS + 5)
#define VM_RECORD_REC_NUM_IMAGE_YPOS      (94 - SKY_ANNUNCIATORS_HEIGHT)//(VM_RECORD_BACKGROUND_YPOS + 6)

#define VM_HORIZONTAL_LINE_XPOS           4
#define VM_HORIZONTAL_LINE_YPOS           (85 - SKY_ANNUNCIATORS_HEIGHT)




/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

enum tagCALLINCOMINGFROMWHERE_E
{
    CIFW_VMEMO,
    CIFW_USERMENT
};

typedef enum tagCALLINCOMINGFROMWHERE_E  CALLINCOMINGFWHERE;



/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
  
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern CHAR pszDayOfWeek[7][3];
	
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API SKA_MainVMemo(SKY_EVENT_T* pEvent);
extern BOOL GetIsPlayVMemo(void);      
extern void SetPlayVMemo(BOOL fCurrentState);
extern void UpdateTime(DWORD dwCurrTime, CHAR pszVMemo[]);
extern void SKY_API SKA_CallIncomingInRecordingVMemo(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_CallIncomingInPlayingVMemo(SKY_EVENT_T* pEvent);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_VMEMO_H_ */