/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_cm.c
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
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"

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
**  Global variables
**---------------------------------------------------------------------------
*/

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
void SKY_API SKY_InitCallManager(void)
{
    ui_cm_init();
}

void SKY_API SKY_SetPhoneInfoAvail(BOOL fAvail)
{
    ui_set_ph_info_avail(fAvail);
}

BOOL SKY_API SKY_IsPhoneInfoAvail(void)
{
    return ui_ph_info_is_avail();
}

BOOL SKY_API SKY_CallIsFromUI(CONST CM_CALL_INFO_T *pCallInfo)
{
    return ui_call_is_from_ui (pCallInfo);
}

/*
pbNumber           # for origination, could contain pause/link
nNumberLength      length of the #
pbName             alpha tag (name) of the origination
nNameLength        length of the alpha tag
*/
void SKY_API SK_OriginateVoiceCall(CONST BYTE *pbNumber,
                                   UINT8 nNumberLength,
                                   CONST BYTE *pbName,
                                   UINT8 nNameLength)
{
    SKY_SNI(HS_SNI_CALL_CONNECT, TRUE);
    ui_orig_voice_call(pbNumber, nNumberLength, pbName, nNameLength);
}

/*
WOED wCDMA_SO       service option for origination
pbNumber          # for origination, could contain pause/link
nNumberLength       length of the #
pbName            alpha tag (name) of the origination
nNameLength         length of the alpha tag
*/
void SKY_API SKY_OriginateTestCall(WORD wCDMA_SO,
                           CONST BYTE *pbNumber,
                           UINT8 nNumberLength,
                           CONST BYTE *pbName,
                           UINT8 nNameLength)
{
    SKY_SNI(HS_SNI_CALL_CONNECT, TRUE);
    ui_orig_test_call(wCDMA_SO, pbNumber, nNumberLength, 
                      pbName, nNameLength);
}

void SKY_API SKY_EndCall(void)
{
    // jrkwon 2001.06.05 
    // SKY_SNI(HS_SNI_CALL_ALERT, FALSE);
    SKY_MotorStop();
    SKY_StopMusic();

    ui_end_call();
}

void SKY_API SKY_AnswerCall(void)
{
    SKY_SNI(HS_SNI_CALL_ALERT, FALSE);
    SKY_MotorStop();
    SKY_StopMusic();

    ui_answer_call();
}

void SKY_API SKY_AnswerSMSCall(void)
{
    SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
    ui_answer_sms_call();
}

void SKY_API SKY_SendFlash(CONST BYTE *pszNumber, INT8 nNumberLength)
{
    ui_send_flash(pszNumber, nNumberLength);
}

void SKY_API SKY_SendPowerDownToCM(void)
{
    ui_power_down();
}

BOOL SKY_API SKY_SetPhoneOperationMode(CM_OPERATION_MODE_E OperationMode)
{
    return ui_set_ph_oprt_mode(OperationMode);
}

void SKY_API SKY_SetPhoneSystemPrefernce(CM_SYSTEM_PREFERENCE_E SysPref)
{
    ui_set_ph_sys_pref(SysPref);
}

void SKY_API SKY_SetPhoneModePreference(CM_MODE_PREFERENCE_E ModePref, 
                                        CM_MODE_TERMINATION_E ModeTerm,
                                        DWORD dwDuration)
{
    ui_set_ph_mode_pref(ModePref, ModeTerm, dwDuration);
}

void SKY_API SKY_SetPhoneCurrentNam(CM_NAM_E CurrentNam)
{
    ui_set_ph_curr_nam((byte)CurrentNam);
}

void SKY_API SKY_SetPhoneAutoNAM(BOOL fOnOff)
{
    ui_set_ph_auto_nam(fOnOff);
}

void SKY_API SKY_SetPhoneVoiceAsData(CM_ANSWER_VOICE_E AnswerVoice)
{
    ui_set_ph_voice_as_data(AnswerVoice);
}

BOOL SKY_API SKY_BurstDTMF(UINT8 nPulseWidth, UINT8 nInterDigitInterval, 
                           UINT8 nDigitCount, CONST BYTE *pbDTMF)
{
    return ui_burst_dtmf(nPulseWidth, 
                         nInterDigitInterval, nDigitCount, pbDTMF);
}

BOOL SKY_API SKY_StartContinuosDTMF(BYTE bDigit)
{
    return ui_start_cont_dtmf(bDigit);
}

BOOL SKY_API SKY_StopContinuosDTMF(WORD wDuration)
{
    return ui_stop_cont_dtmf(wDuration);
}

#if (defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900))
BOOL SKY_API SKY_PhoneCommandIsModeValid(CM_MODE_PREFERENCE_E PrefMode)
{
    return ui_ph_cmd_is_mode_valid(PrefMode);
}
#endif

