/*
** ===========================================================================
**
** Project: 
**     
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
** 00-01-01   girin      Created.
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"

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
//CHAR* CONST Month[] = {"January", "February", "March", "April", "May"};
//CHAR* CONST Week[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
CHAR* Month[] = {"January", "February", "March", "April", "May"};
CHAR* Week[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
/* rosa 00-07-11 --> @why, what */
UCHAR* pszInit = (UCHAR*)"";
UCHAR* pszInputed;
UINT8  hDialEditBox1;
/* <-- */

VOID EQA_Sample1(EQS_EVENT *pInEvent)
{

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
/* rosa 00-07-10 --> to test DialEditBox control */
		
		hDialEditBox1 = DialEditBox((UCHAR*)"",0,10,10,2, NUMBER_FONT, DEDIT_DEFAULTS);
/* <-- */		
		ScrollList(Week, 9, 7, 120, 10, 9, 2, 0, TRUE, SCROLL_DEFAULTS);
		CmdConsole(CONSOLE_TYPE_MENU_SEL_CANCEL, CMD_DEFAULTS);
		break;

/* rosa 00-07-13 --> @why, what */
    case EQE_KEY_STAR:
		if( hDialEditBox1 != INVALID_IOC_HANDLE )
		{
			pszInputed = GetDialEditBoxValue(hDialEditBox1);
		}
		break;
/* <-- */

	case EQE_KEY_POUND:
		EQS_SetFocusNext();
		break;

	case EQE_SOFTKEY_SELECT:
	case EQE_KEY_1:
		MesgBox("SELECTED", 3, MB_BEEP); // 3 sec.
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}

//	return 0;
}


/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
