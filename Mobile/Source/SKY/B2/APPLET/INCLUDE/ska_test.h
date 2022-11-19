/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqa_test.h
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
** 2000-11-14 girin     Created.
**
** ============================================================================
*/

#ifndef _EQA_TEST_H_
#define _EQA_TEST_H_

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
extern boolean IsApplyHighMelodyGain    ;
    /*
    **---------------------------------------------------------------------------
    **  Function(external use only) Declarations
    **---------------------------------------------------------------------------
    */
//extern void EQS_API EQA_CtrlTestMenuTop(EQS_EVENT *pInEvent);
//extern void EQS_API EQA_MainTestGraph(EQS_EVENT *pInEvent);
//extern void EQS_API EQA_MainMelodyTop(EQS_EVENT *pInEvent);

extern void SKY_DrawDebugScreen(void); // defined in sky_test.c girin. 2000.12.16


extern void SKY_API SKA_MainTestMelodyList(SKY_EVENT_T* pInEvent);
extern void SKY_API EQA_MainDebugScreenToggle(SKY_EVENT_T* pInEvent);
extern void SKY_API SKA_BatteryInit(SKY_EVENT_T *pInEvent);		// √ ±‚»≠

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
extern void SKY_API SKA_MainBackLightTest(SKY_EVENT_T* pInEvent);
extern void SKY_API SKA_MainLightSensorTest(SKY_EVENT_T* pInEvent);
extern void SKY_API SKA_MainLightSensorTest2(SKY_EVENT_T* pInEvent);
extern void SKY_API SKA_MainMelodyVolumeTest(SKY_EVENT_T* pInEvent);
extern void SKY_API SKA_MainEQVolumeTest(SKY_EVENT_T* pInEvent);
extern void SKY_API SKA_MainIDLCDContrastTest(SKY_EVENT_T* pInEvent);
extern void SKY_API SKA_MainSMBTest(SKY_EVENT_T *pInEvent);
extern void SKY_API SKA_MainMICVolumeTest(SKY_EVENT_T *pInEvent);
extern void SKY_API SKA_GVMVoiceTest(SKY_EVENT_T *pInEvent);
extern void SKY_API SKA_MainRFTest(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainAudioTest(SKY_EVENT_T *pInEvent);
extern void SKY_API SKA_MainFlipCounterTesst(SKY_EVENT_T *pInEvent);
extern void SKY_API SKA_LCDContrastControl(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_LCDMusicTotalLevelControl(SKY_EVENT_T *pEvent);


#ifdef SW_RESET_ENABLE
extern void SKY_API SKA_MainSWResetTest(SKY_EVENT_T *pEvent);
#endif
#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQA_TEST_H_ */
