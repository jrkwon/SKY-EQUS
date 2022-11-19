/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_cm.h
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
** 2000-10-30 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_CM_H_
#define _SKY_CM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "cm.h"

#include "uixcm.h"

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
typedef cm_call_info_s_type         CM_CALL_INFO_T;
typedef cm_oprt_mode_e_type         CM_OPERATION_MODE_E;
typedef cm_sys_pref_e_type          CM_SYSTEM_PREFERENCE_E;
typedef cm_mode_pref_e_type         CM_MODE_PREFERENCE_E;
typedef cm_mode_term_e_type         CM_MODE_TERMINATION_E;
typedef cm_answer_voice_e_type      CM_ANSWER_VOICE_E;

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

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/*---------------------------------------------------------------------------
**  SKY wrapper function
*/
extern void SKY_API SKY_InitCallManager(void);
extern void SKY_API SKY_SetPhoneInfoAvail(BOOL fAvail);
extern BOOL SKY_API SKY_IsPhoneInfoAvail(void);
extern BOOL SKY_API SKY_CallIsFromUI(CONST CM_CALL_INFO_T *pCallInfo);
extern void SKY_API SK_OriginateVoiceCall(CONST BYTE *pbNumber,
                                           UINT8 nNumberLength,
                                           CONST BYTE *pbName,
                                           UINT8 nNameLength);
extern void SKY_API SKY_OriginateTestCall(WORD wCDMA_SO,
                                           CONST BYTE *pbNumber,
                                           UINT8 nNumberLength,
                                           CONST BYTE *pbName,
                                           UINT8 nNameLength);
extern void SKY_API SKY_EndCall(void);
extern void SKY_API SKY_AnswerCall(void);
extern void SKY_API SKY_AnswerSMSCall(void);
extern void SKY_API SKY_SendFlash(CONST BYTE *pszNumber, INT8 nNumberLength);
extern void SKY_API SKY_SendPowerDownToCM(void);
extern BOOL SKY_API SKY_SetPhoneOperationMode(CM_OPERATION_MODE_E OperationMode);
extern void SKY_API SKY_SetPhoneSystemPrefernce(CM_SYSTEM_PREFERENCE_E SysPref);
extern void SKY_API SKY_SetPhoneModePreference(CM_MODE_PREFERENCE_E ModePref, 
                                               CM_MODE_TERMINATION_E ModeTerm,
                                               DWORD dwDuration);
extern void SKY_API SKY_SetPhoneCurrentNam(CM_NAM_E CurrentNam);
extern void SKY_API SKY_SetPhoneAutoNAM(BOOL fOnOff);
extern void SKY_API SKY_SetPhoneVoiceAsData(CM_ANSWER_VOICE_E AnswerVoice);
extern BOOL SKY_API SKY_BurstDTMF(UINT8 nPulseWidth, UINT8 nInterDigitInterval, 
                                   UINT8 nDigitCount, CONST BYTE *pbDTMF);
extern BOOL SKY_API SKY_StartContinuosDTMF(BYTE bDigit);
extern BOOL SKY_API SKY_StopContinuosDTMF(WORD wDuration);
extern BOOL SKY_API SKY_PhoneCommandIsModeValid(CM_MODE_PREFERENCE_E PrefMode);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_CM_H_ */

