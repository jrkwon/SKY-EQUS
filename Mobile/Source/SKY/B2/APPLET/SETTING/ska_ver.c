/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     
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
** 4333-12-15 white     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
#include <stdio.h>
#ifdef WIN32
extern const char mob_sw_rev[];
#else
#include "mobile.h"
#endif
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

/*
**    mob_sw_rev[] = "S0119001"
**    
**    S-0119-001
**      ---- -+-
**            |
**            +-- version
**
*/

void SKY_API SKA_MainVersion(SKY_EVENT_T *pEvent)
{
    BYTE str[5];
    UINT8 i;
    COLOR OldColor;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        //EQS_SetWindow(0, 0, 120, 100, CML_NONE, WA_SHOW_HEADER );
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            SKY_GET_TOKEN(TKN_SW_VERSION), 
            WA_SHOW_HEADER);


        //EQS_SetSWindowHeader(sDrawHeader);
        break;
        
    case SE_APP_DRAW:
        i = STRLEN(mob_sw_rev);
		if (i == 8)
		{
			str[0] = mob_sw_rev[5];
			str[1] = '.';
			str[2] = mob_sw_rev[6];
			str[3] = mob_sw_rev[7];
			str[4] = '\0';
		}
		else
		{
			STRCPY((BYTE *)&(str[0]), "n/a");
		}

        EQS_GetDC(TRUE);
        EQS_SetTextBackColor(EQS_GetBackColor());
        EQS_SetTextAlignHoriz(TAH_CENTER);
        EQS_SetTextAlignVert(TAV_CENTER);
        EQS_TextOutStrF(SKY_SCREEN_WIDTH/2, (SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT)/2, "버전: %s", str);
        EQS_ReleaseDC();
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


void SKY_API SKA_MainTestMenuVersion(SKY_EVENT_T *pEvent)
{
    extern word app_image_checksum;
    extern word cal_chksum(void);

    STATIC WORD s_wChkSum = 0;

    BYTE str[21];
    UINT8 i;
    HCONTROL hControl;
    COLOR OldColor;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 100, CML_NONE, WA_SHOW_HEADER );
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            SKY_GET_TOKEN(TKN_SW_VERSION), 
            WA_SHOW_HEADER);

        if (s_wChkSum == 0)
        {
            hControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WAITING_A_MOMENT), 
                                    MESSAGE_DEFAULT_TIME, 
                                    MBT_NONE, 
                                    MBT_HELP);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_MESSAGE_BOX_TITLE);
        }

        break;

    case SE_IO_MESGBOX_END:

        // cal_chksum is time consuming job...
        if (s_wChkSum == 0) // This if-clause is not necessary but just for safty code.
        {
            s_wChkSum = cal_chksum();
        }
        break;

    case SE_APP_DRAW:
        i = STRLEN(mob_sw_rev);
		
        OldColor = EQS_GetTextBackColor();
        EQS_SetTextBackColor(EQS_GetBackColor());

        EQS_TextOutStr(20, 30, "버전:");
        EQS_TextOutStr(55, 30, mob_sw_rev);

        if (s_wChkSum)
        {
            sprintf(str, "CHECKSUM: %04X", s_wChkSum);

            EQS_TextOutStr(20, 50, str);

	        if(app_image_checksum == s_wChkSum)
	        {
		        sprintf(str, "CHECKSUM 정상");
	        }
	        else
	        {
		        sprintf(str, "CHECKSUM 오류");
	        }

            EQS_TextOutStr(20, 65, str);
        }
        EQS_SetTextBackColor(OldColor);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}