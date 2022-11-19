/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_bell.h
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
** 2000-12-25 woodstok     Created.
**
** ============================================================================
*/

#ifndef _SKA_BELL_H_
#define _SKA_BELL_H_

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
#define PROGRESS_BAR_HEIGHT 8
#define PROGRESS_BAR_WIDTH  112

#if (MODEL_ID == MODEL_B0)
#ifdef FEATURE_CHARACBELL
    #define NUM_BELL_GROUP  6
    #define BELL_GROUP_CHARAC_BELL   5
    #define BELL_GROUP_MY_BELL_ID   4
#else
    #define NUM_BELL_GROUP  5
    #define BELL_GROUP_MY_BELL_ID   4
#endif // FEATURE_CHARACBELL

    #define NUM_BELL_GROUP_BASIC    24
    #define NUM_BELL_GROUP_CLASSIC1 20
    #define NUM_BELL_GROUP_CLASSIC2 20
    #define NUM_BELL_GROUP_SOUND_EFFECT 28

    #define NUM_BELL_LIST   (NUM_BELL_GROUP_BASIC + NUM_BELL_GROUP_CLASSIC1 \
                           + NUM_BELL_GROUP_CLASSIC2 + NUM_BELL_GROUP_SOUND_EFFECT)

#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
#ifdef FEATURE_CHARACBELL
    #define NUM_BELL_GROUP  9
    #define BELL_GROUP_CHARAC_BELL   8
    #define BELL_GROUP_MY_BELL_ID   7
#else
    #define NUM_BELL_GROUP  8
    #define BELL_GROUP_MY_BELL_ID   7
#endif // FEATURE_CHARACBELL

    #define NUM_BELL_GROUP_BASIC        5
    #define NUM_BELL_GROUP_CHILD        10
    #define NUM_BELL_GROUP_ANNIV        13
    #define NUM_BELL_GROUP_FOLK         6
    #define NUM_BELL_GROUP_CLASSIC1     20
    #define NUM_BELL_GROUP_CLASSIC2     19
    #define NUM_BELL_GROUP_SOUND_EFFECT 20//////////////////////////인간의 음성을 추가하기 위해 12를 20로 바꿨음.

    #define NUM_BELL_LIST   (NUM_BELL_GROUP_BASIC + NUM_BELL_GROUP_CHILD + NUM_BELL_GROUP_ANNIV \
                            + NUM_BELL_GROUP_FOLK  + NUM_BELL_GROUP_CLASSIC1 \
                            + NUM_BELL_GROUP_CLASSIC2 + NUM_BELL_GROUP_SOUND_EFFECT)
#endif

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef struct tagBELL_GROUP_TYPE_T BELL_GROUP_TYPE_T;
typedef struct tagBELL_NAME_TYPE_T BELL_NAME_TYPE_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

struct tagBELL_GROUP_TYPE_T {
    SKY_TOKEN   GroupName;
    UINT8       nMelodyNumber;
};

struct tagBELL_NAME_TYPE_T {
    SKY_TOKEN   BellName;
    WORD        wBellId; 
};

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

extern CONST BELL_GROUP_TYPE_T aBell_Group[NUM_BELL_GROUP];
extern CONST BELL_NAME_TYPE_T aBell_List[NUM_BELL_LIST];

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

// ska_bell.c
extern void SKY_API SKA_MainBellVibSetting(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainAlarmBellSetting(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainSchedulerBellSetting(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainGroupBellSetting(SKY_EVENT_T *pEvent);

#ifdef FEATURE_CHARACBELL
extern void SetCharacbellSelected(BOOL bSelect);
extern BOOL GetCharacbellSelected(void);
#endif // FEATURE_CHARACBELL

/*---  Util  ----*/
extern UINT8 EQS_API SKY_BellFindGroup(UINT8 nBellId, UINT8 nBellGroup);
extern UINT8 EQS_API SKY_BellFindGroupIndex(UINT8 nGroup);
extern UINT8 EQS_API SKY_FindBellIndex(WORD wBellId);
extern UINT8 EQS_API SKY_FindMyBellIndex(BYTE *pszBell);
#ifdef FEATURE_CHARACBELL
extern UINT8 EQS_API SKY_FindCharacBellIndex(BYTE *pszBell);
#endif // FEATURE_CHARACBELL

// ska_mbel.c
extern void SKY_API SKA_MainMyBell(SKY_EVENT_T *pEvent);   

// ska_warn.c
extern void SKY_API SKA_MainWarningSoundSetting(SKY_EVENT_T *pEvent);
extern void SKY_API TwoNumberBellCtrl(void);
extern void SKY_API TwoNumberBellCtrlFree(void);
extern void SKY_API SaveTwoNumBell(void);
extern BOOL SKY_API CompareTwoNumBell(void);
extern void EQS_API PlayTwoNumBell(void);
extern void SKY_API TwoNumberBellChgScrl(UINT8 nCtrl, DWORD dwParam);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_BELL_H_ */
