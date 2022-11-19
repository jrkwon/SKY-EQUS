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
** 00-07-14   girin      Created.
**
** ===========================================================================
*/
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"
#include "eqa.h"

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
UINT8 l_MenuMode = 0; // text mode

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

/*
VOID EQA_MainMenuTop(EQS_EVENT *pEvent)
{
    HCONTROL hControl;

	switch (pEvent->EventID)
	{
	case EQE_APP_START:

        EQS_GetDC(TRUE);
        EQS_SetWindow(0, 0, 120, 200, CML_BROWSE, WA_NONE);

		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

    case EQE_APP_DRAW:

        EQS_ClearScreen();

        EQS_TextOut(2, 2, TKN_TEST_PRIMITIVE);
        EQS_TextOut(2, 16, TKN_TEST_TEXT);
        EQS_TextOut(2, 30, TKN_TEST_IMAGE);
        EQS_TextOut(2, 44, TKN_TEST_DRAWMODE);
        EQS_TextOut(2, 58, TKN_TEST_MULTILANG);
        EQS_TextOut(2, 72, TKN_TEST_GUI);
        EQS_TextOut(2, 86, TKN_TEST_SLIDING);

        EQS_UpdateDisplay();

        break;

	default:
		EQS_DefEventProc(pEvent);
		break;
	}
}
*/


VOID EQA_MainMenuTop(EQS_EVENT *pEvent)
{
/*
    HCONTROL hControl;

	switch (pEvent->EventID)
	{
	case EQS_APP_START:

        EQS_GetDC(TRUE);
        SKY_SetWindow(0, 0, 120, 200, CML_BROWSE, WA_NONE);

        if (l_MenuMode == 0) // Text Mode.
        {
            //EQC_MenuList(6, 10, 18, 6, &mnMainMenuTop, 0);
            EQC_MenuList(6, 4, 18, 6, &mnMainMenuTop, 0);
        }
        else
        {
            EQC_SET_STYLE(EQC_MenuList(6, 4, 18, 6, &mnMainMenuTop, 0), 
                CS_MENU_LIST_LEFT_ALIGN 
                | CS_MENU_LIST_ICON 
                | CS_MENU_LIST_CAPTION
                );
        }

        EQS_SetIOCActive(EQS_GetNthIOC(0));

		break;

    case EQE_APP_RESUME:

        hControl = EQS_GetNthIOC(0);

        if (EQS_GetChild() == EQA_MainMenuSet)
        {
            l_MenuMode = pEvent->wParam;
            if ( l_MenuMode == 1) // Text Mode Menu.
            {
                EQC_SET_STYLE(hControl,
                              CS_MENU_LIST_LEFT_ALIGN 
                            | CS_MENU_LIST_ICON 
                            | CS_MENU_LIST_CAPTION
                            );
            }
            else
            {
                EQC_SET_STYLE(hControl, CS_MENU_LIST_DEFAULT);
            }
        }

        EQS_SetIOCActive(hControl);
        break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;


    case EQE_IO_MENU_LIST_SELECT:
        EQS_StartChild((EQS_Applet)pEvent->dwParam);
        break;

    case EQE_IO_MENU_LIST_CANCEL:
        EQS_End();
        break;

	default:
		EQS_DefEventProc(pEvent);
		break;
	}
    */
}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
