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
extern VOID EQA_TestMenuTop(EQS_EVENT *pInEvent);
//extern VOID EQA_Girin(EQS_EVENT *pInEvent); // defined in (EQUS_ROOT)\Applet\Common\Girin.c

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
//CHAR* CONST Who[] = {"Chul", 
CONST CHAR* CONST Who[] = {
                    "TestMenu",
                    "Chul", 
					"Chojw", 
					"Jwkwon", 
					"Reo", 
					"Girin", 
					"Huni", 
					"Wshan", 
					"Hjcho", 
					"Woodstok", 
					"Redstar", 
					"Rosa", 
					"White"
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

VOID EQA_MainMenuTop(EQS_EVENT *pInEvent)
{
	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		EQC_ScrollList(Who, 13, 13, 20, 10, 8, 5, 0, SCROLL_DEFAULTS);
//        EQC_CMDConsole(CONSOLE_TYPE_MENU_SEL_CANCEL, CMD_DEFAULTS);
		break;
	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

    case EQE_IO_SCROLL_LIST_SELECT:

        switch (pInEvent->dwParam)
        {
        case 0: // Test menu.
            EQS_StartChild(EQA_TestMenuTop);
            break;

        case 5: // Girin.
//            EQS_StartChild(EQA_Girin);
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
