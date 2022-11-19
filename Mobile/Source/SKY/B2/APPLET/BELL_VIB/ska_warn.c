/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_warn.c
**
** Description: 
**     Warning Sound Setting(경보음설정)
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-12 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SWS_CHECK_BOX       0
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
enum tag_SKA_WARNING_SOUND_TYPE_E {
    SWS_CONNECT,
    SWS_1MINUTE,
    SWS_NOSERVICE,
    SWS_LOBATT,
    SWS_MESSAGE
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
LOCAL BYTE *ppWarningItem[5];
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void EQS_API sAddWarningSoundControls(void);
LOCAL void EQS_API sInitWarningSound(void);
LOCAL void EQS_API sSaveWarningSound(void);
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


/*============================================================================= 
Function:    
SKA_MainXxxx

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
void SKY_API SKA_MainWarningSoundSetting(SKY_EVENT_T *pEvent)
{
    static BOOL fSaveMessage, fEndMessage, fIdle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_BELLVIB_WARNING), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        sAddWarningSoundControls();
        EQS_FitCanvasSize();

        fSaveMessage = fEndMessage = fIdle = FALSE;
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdle = TRUE;
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;

    case SE_IO_CHECK_BOX_UP:
    case SE_IO_CHECK_BOX_DOWN:
        EQS_SetIOCActive(EQS_GetNthIOC(SWS_CHECK_BOX));
        break;

    case SE_IO_CHECK_BOX_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
        {
		    if(EQS_IsControlDataChanged())
		    {
			    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							    MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			    fSaveMessage = TRUE;
		    }
		    else
		    {
			    EQS_End();
		    }
        }
        break;

    case SE_SOFTKEY_SAVE:
        sSaveWarningSound();
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

        fEndMessage = TRUE;
        break;

    case SE_SOFTKEY_INIT:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_CONFIRM), 
                        MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES:
        if(fSaveMessage)
        {
            sSaveWarningSound();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        else
        {
            sInitWarningSound();

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_PROCESS), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_END:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_End();
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

/* rosa 2001-10-29 --> @why, what */
    case EQE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(SWS_CHECK_BOX));
        break;
/* <-- */

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


LOCAL void EQS_API sAddWarningSoundControls(void)
{
    UINT8 nCtrl;

    ppWarningItem[SWS_CONNECT] 
        = (BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_WARNING_CALL);
    ppWarningItem[SWS_1MINUTE] 
        = (BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_WARNING_1MIN);
    ppWarningItem[SWS_NOSERVICE]
        = (BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_WARNING_NOSERVICE);
    ppWarningItem[SWS_LOBATT] 
        = (BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_WARNING_LOBAT);
    ppWarningItem[SWS_MESSAGE] 
        = (BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_MESSAGE_BEEP);

    EQC_SET_STYLE(EQC_CheckBox(0, 0, (BYTE **)ppWarningItem, 5), CS_CHECK_BOX_MAXIMIZE);

    if(SKY_IS_CONNECT_WARNING_SIGNAL())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_CONNECT);

    if(SKY_IS_1MINUTE_WARNING_SIGNAL())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_1MINUTE);

    if(SKY_IS_NOSERVICE_WARNING_SIGNAL())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_NOSERVICE);

    if(SKY_IS_LOBATT_WARNING_SIGNAL())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_LOBATT);

    if(SKY_IS_MESSAGE_BEEP_SOUND())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_MESSAGE);

    EQS_SetIOCActive(EQS_GetNthIOC(SWS_CHECK_BOX));
}

LOCAL void EQS_API sInitWarningSound(void)
{
    SKY_SET_CONNECT_WARNING_SIGNAL(INIT_IS_CONNECT_WARNING_SIGNAL);
    SKY_SET_1MINUTE_WARNING_SIGNAL(INIT_IS_1MINUTE_WARNING_SIGNAL);
    SKY_SET_NOSERVICE_WARNING_SIGNAL(INIT_IS_NOSERVICE_WARNING_SIGNAL);
    SKY_SET_LOBATT_WARNING_SIGNAL(INIT_IS_LOBATT_WARNING_SIGNAL);
    SKY_SET_MESSAGE_BEEP_SOUND(INIT_IS_MESSAGE_BEEP_SOUND);

    if(SKY_IS_CONNECT_WARNING_SIGNAL())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_CONNECT);
    else
        EQC_ResetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_CONNECT);

    if(SKY_IS_1MINUTE_WARNING_SIGNAL())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_1MINUTE);
    else
        EQC_ResetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_1MINUTE);

    if(SKY_IS_NOSERVICE_WARNING_SIGNAL())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_NOSERVICE);
    else
        EQC_ResetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_NOSERVICE);

    if(SKY_IS_LOBATT_WARNING_SIGNAL())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_LOBATT);
    else
        EQC_ResetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_LOBATT);

    if(SKY_IS_MESSAGE_BEEP_SOUND())
        EQC_SetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_MESSAGE);
    else
        EQC_ResetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_MESSAGE);
}

LOCAL void EQS_API sSaveWarningSound(void)
{
    SKY_SET_CONNECT_WARNING_SIGNAL(
        EQC_GetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_CONNECT));

    SKY_SET_1MINUTE_WARNING_SIGNAL(
        EQC_GetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_1MINUTE));

    SKY_SET_NOSERVICE_WARNING_SIGNAL(
        EQC_GetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_NOSERVICE));

    SKY_SET_LOBATT_WARNING_SIGNAL(
        EQC_GetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_LOBATT));

    SKY_SET_MESSAGE_BEEP_SOUND(
        EQC_GetCheckBox(EQS_GetNthIOC(SWS_CHECK_BOX), SWS_MESSAGE));
}