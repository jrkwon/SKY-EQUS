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
#include "hs.h"
#include "uixscrn.h"
#include "bio.h"

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

CONST CHAR* CONST LEDMenu[] = {SF_BLACK_NUM1"»¡", 
					SF_BLACK_NUM2"ÃÊ",
					SF_BLACK_NUM3"ÆÄ",
					SF_BLACK_NUM4"»¡+ÃÊ",
					SF_BLACK_NUM5"ÃÊ+ÆÄ",
					SF_BLACK_NUM6"ÆÄ+»¡",
					SF_BLACK_NUM7"»¡+ÃÊ+ÆÄ",
					SF_BLACK_NUM8"OFF"
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

VOID EQA_LEDTop(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	BYTE bCurrentLevel = 1;		// ÇöÀç ¼³Á¤µÈ °ª
	BYTE iKey = 0;
	BYTE index;
	
	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		hHnd = EQC_ScrollList(LEDMenu, 8, 8, 0, 0, 16, 5, bCurrentLevel-1, SCROLL_DEFAULTS);
		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:
	case EQE_KEY_2:
	case EQE_KEY_3:
	case EQE_KEY_4:
	case EQE_KEY_5:
	case EQE_KEY_6:
	case EQE_KEY_7:
	case EQE_KEY_8:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_SOFTKEY_SELECT:
		index = EQC_GetScrollIndex(hHnd);
		if(index == 0 || index == 3 || index == 5 || index == 6)
		{
			BIO_OUT(DEC_GPIO_0, BIO_LED_RED_M, BIO_LED_RED_HI_V);
		}
		else
		{
			BIO_OUT(DEC_GPIO_0, BIO_LED_RED_M, BIO_LED_RED_LO_V);
		}
		if(index == 1 || index == 3 || index == 4 || index == 6)
		{
			BIO_OUT(DEC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_HI_V);
		}
		else
		{
			BIO_OUT(DEC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V);
		}
		if(index == 2 || index == 4 || index == 5 || index == 6)
		{
			BIO_OUT(DEC_GPIO_0, BIO_LED_BLUE_M, BIO_LED_BLUE_HI_V);
		}
		else
		{
			BIO_OUT(DEC_GPIO_0, BIO_LED_BLUE_M, BIO_LED_BLUE_LO_V);
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
