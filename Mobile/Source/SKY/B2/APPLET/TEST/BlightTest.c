/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     
**
** Description: 
**     <@Desciption about this file here>
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-07-21   woodstok   Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"
#include "uixscrn.h"

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
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

CONST CHAR* CONST BLightMenu[] = {SF_BLACK_NUM1"ON",
					  SF_BLACK_NUM2"OFF"
};

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

VOID EQA_BLightTop(EQS_EVENT *pInEvent)
{
	static HCONTROL	hHnd;
	UINT8	iKey = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		hHnd = EQC_ScrollList(BLightMenu, 2, 2, 0, 0, 13, 2, 0, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_SOFTKEY_CANCEL:
		EQS_End();
		break;

	case EQE_KEY_1:
	case EQE_KEY_2:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_SOFTKEY_SELECT:
		switch(EQC_GetScrollIndex(hHnd)){
		case 0:	
			// To Do Here!!!
			uiscrn_backlight(TRUE);
			// Background MC ON

			EQC_MesgBox("ON", 3, FT_MED,MB_BEEP);
			break;
		case 1:		
			// To Do Here!!!
			uiscrn_backlight(FALSE);
			// Background MC OFF

			EQC_MesgBox("OFF", 3,FT_MED, MB_BEEP);
			break;
		default:
			break;
		}
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
