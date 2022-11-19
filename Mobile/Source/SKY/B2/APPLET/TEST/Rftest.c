/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     rftest.c
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
** 2001-03-05 happykid    Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdlib.h>
#include "sky.h"
#include "ska.h"
#include "..\..\..\..\rf\NewCal.h"
#include "MSM_Drv.h"
#include "rfnv.h"
#include "diagpkt.h"
#include "stdio.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define NUM_NAM_MENUITEM_MSM5000       7
#define NUM_NAM_SUBMENUITEM_MSM5000    7

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


#ifdef FEATURE_SKTT_NEW_CAL
void	rf_test_TxOn ( void )
{
	// enable RF H/W Tx
	rf_DigitalTxOn ();

	// Tx max power output
	MSM_LOCK ();
	TX_SET_GAIN_ADJUST ( 0x80 );
	MSM_UNLOCK ();
}

void	rf_test_TxOff ( void )
{
	// disable RF H/W Tx
	rf_DigitalTxOff ();
}

void rf_check_offline(void)
{
	// check offline mode
	if ( !SKY_IS_OFFLINE() )
	{
		// jwpark 00.09.14
        extern diag_rsp_type *diagp_cont(diag_req_type *, word);
        //extern word proc_cont(diag_req_type *, word, diag_rsp_type *);
		diag_control_req_type control_req;

		// continue UI menu mode
		SKY_SET_OFFLINE(TRUE);
		// set digital offline mode use diagp_cont() ( in diagp.c )
		control_req.cmd_code = DIAG_CONTROL_F;
		control_req.mode = MODE_OFFLINE_D_F;
		// jwpark 00.09.14
		diagp_cont((diag_req_type *)&control_req, sizeof(control_req));
	}
}
#endif



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainXxx()
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
enum tag_SKA_RF_TEST_E {
    SAB_RF_TX_ON_TITLE,
    SAB_RF_TX_ON,
    SAB_CHANNEL_TITLE,
	SAB_CHANNEL_EDIT,
    SAB_MAXPOWER_TITLE,
    SAB_MAX_POWER_SPIN,
    SAB_MAX
};

CONST SKY_TOKEN aTx_Mode[2] =
{
    TKN_RF_TX_ON,
    TKN_RF_TX_OFF
};

LOCAL BYTE *ppTxOnOff[2];
extern word		rf_chan;	// define in rfmsm.c

/*
void SKY_API InitializeRFTestMainScreen(void)
{
    UINT8   nCtrl;

	WORD wChannelNo;
    HCONTROL hControl;

	char cDisplayBuffer[30];

	// Init List
	ppTxOnOff[0] = (BYTE *)SKY_GET_TOKEN(aTx_Mode[0]);
	ppTxOnOff[1] = (BYTE *)SKY_GET_TOKEN(aTx_Mode[1]);

	EQC_StaticText(EQS_NA, EQS_NA, TKN_TX_ONOFF);
	hControl = EQC_ScrollCombo(EQS_NA, EQS_NA, 10, 
				   2, 
				   ppTxOnOff, 
				   2, 2,
				   1);
    EQC_SET_STYLE( hControl, EQC_GET_STYLE(hControl) | CS_SCROLL_LIST_ENUMBERATED);
				  

	wChannelNo = 10;
	sprintf ( &cDisplayBuffer[0], "%3d", wChannelNo );

	EQC_StaticText(EQS_NA, EQS_NA, TKN_RF_TUME_CHANNEL);
    hControl = EQC_NumberEdit(EQS_NA, EQS_NA, 3, 1, 
        3, (BYTE *) cDisplayBuffer );
    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_NO_CML|CS_NUMBER_EDIT_BORDER);


	EQC_StaticText(EQS_NA, EQS_NA, TKN_RF_ADJUST_POWER);
	EQC_Spin(EQS_NA, EQS_NA, -128, 127, 1, 10);
	

	// Screen Formatting
	EQS_StartFormatter();

	for(nCtrl = 0; nCtrl < SAB_MAX ; nCtrl++)
	{
		EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
			ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

		EQS_NewLine();
	}


	EQS_EndFormatter();


	EQS_FitCanvasSize();

}
*/

void SKY_API InitializeRFTestMainScreen(void)
{
    UINT8   nCtrl;

	WORD wChannelNo;

	char cDisplayBuffer[4];

	// Init List
	ppTxOnOff[0] = (BYTE *)SKY_GET_TOKEN(aTx_Mode[0]);
	ppTxOnOff[1] = (BYTE *)SKY_GET_TOKEN(aTx_Mode[1]);

	EQC_StaticText(EQS_NA, EQS_NA, TKN_TX_ONOFF);
	EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, 10, 
				   2, 
				   ppTxOnOff, 
				   2, 2,
				   1),
				   CS_SCROLL_LIST_COMBO | CS_SCROLL_LIST_ENUMBERATED);

	wChannelNo = rf_chan;
	sprintf ( &cDisplayBuffer[0], "%3d", wChannelNo );

	EQC_StaticText(EQS_NA, EQS_NA, TKN_RF_TUME_CHANNEL);
	EQC_SET_STYLE(EQC_NumberEdit(EQS_NA, EQS_NA, 4, 1, 
        4, (BYTE *) cDisplayBuffer ), CS_NUMBER_EDIT_BORDER| CS_NUMBER_EDIT_PLAIN_NUM);


	EQC_StaticText(EQS_NA, EQS_NA, TKN_RF_ADJUST_POWER);
	EQC_SET_STYLE(EQC_Spin(EQS_NA, EQS_NA, -128, 127, 1, rf_GetTxMaxPowerLimit ( rf_GetCurrentFreqIndex ()))
		 ,CS_SPIN_BORDER);

	// Screen Formatting
	EQS_StartFormatter();

	for(nCtrl = 0; nCtrl < SAB_MAX ; nCtrl++)
	{
		EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
			ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

		EQS_NewLine();
	}

	EQS_EndFormatter();

	EQS_FitCanvasSize();

    EQS_SetIOCFocus(EQS_GetNthIOC(SAB_RF_TX_ON));
}

void SKY_API SKA_MainRFTest(SKY_EVENT_T *pEvent)
{

//    UINT8   nCtrl;
//	extern word		rf_chan;	// define in rfmsm.c
//	WORD wChannelNo;
	WORD Channel;

	BYTE bDisplayBuffer[30];

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            200,
            CL_INIT_NONE_SAVE,
            SKY_GET_TOKEN(TKN_RF_TEST), 
            WA_SHOW_HEADER|WA_SHOW_CML);

		if ( !SKY_IS_OFFLINE() )
		{
			EQS_TextOutStr(20, 30, (BYTE*)SKY_GET_TOKEN(TKN_WAIT_RF_TX_ON));

#ifdef FEATURE_SKTT_NEW_CAL
			rf_check_offline();
#endif
			EQS_StartTimer(EQS_TIMER_1, 3000, FALSE);
			//Timer;
		}
		else
		{
			InitializeRFTestMainScreen();
		}

        break;
	case SE_TM_TIMER1:
			EQC_SET_STYLE(EQC_MessageBox((BYTE *)"offlineÁøÀÔ",
										1, 
										MBT_NONE, 
										MBT_HELP), CS_MESSAGE_BOX_TITLE);
		break;
	case SE_IO_MESGBOX_END:
			InitializeRFTestMainScreen();
		break;
		
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
		SKY_PowerOff(POM_RESET);
		break;
		
    case SE_APP_SUSPEND: /* child applet is started */
        break;
		
    case SE_APP_RESUME: /* child applet is ended */
        break;
	/// Tx on/off combo box	
	case SE_IO_SCROLL_LIST_SELECT:
		if(EQS_GetIOCOrder((UINT8)(pEvent->wParam)) == SAB_RF_TX_ON)
		{
			if((UINT8)(pEvent->dwParam) == 0)
			{
#ifdef FEATURE_SKTT_NEW_CAL
				rf_test_TxOn();
#endif
			}
			else
			{
#ifdef FEATURE_SKTT_NEW_CAL
				rf_test_TxOff();
#endif
			}
		}
		break;
	//Edit channel
	case SE_IO_NEDIT_DONE:
		if(EQS_GetIOCOrder((UINT8)(pEvent->wParam)) == SAB_CHANNEL_EDIT)
		{
			EQC_GetNumberEditValue(pEvent->wParam, bDisplayBuffer);
			Channel = atol((char*)bDisplayBuffer); 
			rf_tune_to_chan ( Channel );
		}
		break;
	//Max power limit
	case SE_IO_SPIN_SELECT:
	case SE_IO_SPIN_CHG:
		if(EQS_GetIOCOrder((UINT8)(pEvent->wParam)) == SAB_MAX_POWER_SPIN)
		{
			rf_SetTxMaxPowerLimit ( rf_GetCurrentFreqIndex (), (UINT8)(pEvent->dwParam));
		}
		break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
