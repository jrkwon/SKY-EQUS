/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_irpc.c
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
** 2001-01-04 woodstok     Created.
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
#define IRDA_PC_ON  0
#define IRDA_PC_OFF 1

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

BOOL    g_fIrDAPortForPC = FALSE;

extern short	IrDA_AutoOffTimeOutCounter;
extern void	MDM_GetSetting(int Prime,int Second);

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL BYTE *ppIrPCItem[2];
LOCAL HCONTROL hRadioButton;
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

LOCAL void EQS_API sAddIrDAPCControls(void);

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
void SKY_API SKA_MainIrDAPC(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_CONFIRM_NONE, 
                      SKY_GET_TOKEN(TKN_PHONE_TO_PC), 
                      WA_SHOW_HEADER | WA_SHOW_CML);

        sAddIrDAPCControls();
        EQS_FitCanvasSize();
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_RADIO_BUTTON_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
            EQS_End();
        break;

    case SE_SOFTKEY_CANCEL:
        EQS_END();
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
        if(EQC_GetRadioButton(hRadioButton) == IRDA_PC_ON)
        {
            g_fIrDAPortForPC = TRUE;
            //SKY_SET_USE_SHARED_MEMORY(TRUE);
            IrDA_AutoOffTimeOutCounter = 200;
            SKY_IrDAStatusCheck_set (100);
        
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PHONE_TO_PC_ON_MSG), 
                       MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        else
        {
            g_fIrDAPortForPC = FALSE;

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PHONE_TO_PC_OFF_MSG), 
                       MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }

        fIrDAPortClosed = !g_fIrDAPortForPC;
        MDM_GetSetting(FALSE,TRUE);
        ChangeIrDAMode ( g_fIrDAPortForPC );
        break;

    case SE_IO_MESGBOX_END:
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKY_CloseIrDAForPC(void)
{
	if(!fIrDAPortClosed)
	{
		ChangeIrDAMode ( 0 );
		// Phone to PC IrDA가 off 되었음을 표시
		fIrDAPortClosed = TRUE;
        //SKY_SET_USE_SHARED_MEMORY(FALSE);
        g_fIrDAPortForPC = FALSE;
	}
}

LOCAL void EQS_API sAddIrDAPCControls(void)
{
    UINT8 nCtrl;

    ppIrPCItem[IRDA_PC_ON] 
        = (BYTE *)SKY_GET_TOKEN(TKN_PHONE_TO_PC_ON);
    ppIrPCItem[IRDA_PC_OFF] 
        = (BYTE *)SKY_GET_TOKEN(TKN_PHONE_TO_PC_OFF);

    hRadioButton = EQC_RadioButton(EQS_NA, EQS_NA, 16, 2, (BYTE **)ppIrPCItem, 2);

    EQC_SET_STYLE(hRadioButton, CS_RADIO_BUTTON_MAXIMIZE);
    if(g_fIrDAPortForPC)
        EQC_SetRadioButton(hRadioButton, IRDA_PC_ON);
    else
        EQC_SetRadioButton(hRadioButton, IRDA_PC_OFF);

    EQS_SetIOCActive(hRadioButton);
}