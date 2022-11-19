/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_game.c
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
** 2001-02-07 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

#include "ska.h"
#include "..\SJ_GAMES\GLib\GLib.h"

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
enum tag_ENUM_GAME_TYPE_E {
    EGT_APACHE,
    EGT_POLAR,
    EGT_MARS,
    EGT_SAMTRIS,
    EGT_PUZZLE
};

typedef enum tag_ENUM_GAME_TYPE_E ENUM_GAME_TYPE_E;


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

//BOOL    fIsGame = FALSE;

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
LOCAL void SKY_API sSKA_SinjiGame(SKY_EVENT_T *pEvent);
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


void SKY_API SKA_MainApache(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetWindow(SKY_DEF_WIN_X,
					SKY_DEF_WIN_Y, 
					SKY_DEF_WIN_DX, 
					SKY_DEF_WIN_DY, 
					NULL,  
					NULL, // show annunciators.
					WA_NONE);
        
        EQS_START_CHILD_EVENT(sSKA_SinjiGame, SE_APP_START, EGT_APACHE, 0);  
        break;
 
	case SE_APP_DRAW:
		// Do nothing... midd_DrawLCD() will update 
		// applet canvas display area.
		break;
        
    case SE_APP_END :   /* The very last event */ 
        
//        gext_CloseMemory();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainPolar(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetWindow(SKY_DEF_WIN_X,
					SKY_DEF_WIN_Y, 
					SKY_DEF_WIN_DX, 
					SKY_DEF_WIN_DY, 
					NULL,  
					NULL, // show annunciators.
					WA_NONE);
        
        EQS_START_CHILD_EVENT(sSKA_SinjiGame, SE_APP_START, EGT_POLAR, 0);  
        break;
 
	case SE_APP_DRAW:
		// Do nothing... midd_DrawLCD() will update 
		// applet canvas display area.
		break;
        
    case SE_APP_END :   /* The very last event */ 
        
//        gext_CloseMemory();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_Mars(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetWindow(SKY_DEF_WIN_X,
					SKY_DEF_WIN_Y, 
					SKY_DEF_WIN_DX, 
					SKY_DEF_WIN_DY, 
					NULL,  
					NULL, // show annunciators.
					WA_NONE);
        
        EQS_START_CHILD_EVENT(sSKA_SinjiGame, SE_APP_START, EGT_MARS, 0);  
//        EQS_START_CHILD_EVENT(sSKA_SinjiGame, SE_APP_START, EGT_DUDEOGI, 0);  
        break;
 
	case SE_APP_DRAW:
		// Do nothing... midd_DrawLCD() will update 
		// applet canvas display area.
		break;
        
    case SE_APP_END :   /* The very last event */
        
//        gext_CloseMemory();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


void SKY_API SKA_MainSamtris(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetWindow(SKY_DEF_WIN_X,
					SKY_DEF_WIN_Y, 
					SKY_DEF_WIN_DX, 
					SKY_DEF_WIN_DY, 
					NULL,  
					NULL, // show annunciators.
					WA_NONE);
        
        EQS_START_CHILD_EVENT(sSKA_SinjiGame, SE_APP_START, EGT_SAMTRIS, 0);  
        break;
 
	case SE_APP_DRAW:
		// Do nothing... midd_DrawLCD() will update 
		// applet canvas display area.
		break;
        
    case SE_APP_END :   /* The very last event */ 
        
//        gext_CloseMemory();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainPuzzle(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetWindow(SKY_DEF_WIN_X,
					SKY_DEF_WIN_Y, 
					SKY_DEF_WIN_DX, 
					SKY_DEF_WIN_DY, 
					NULL,  
					NULL, // show annunciators.
					WA_NONE);
        
        EQS_START_CHILD_EVENT(sSKA_SinjiGame, SE_APP_START, EGT_PUZZLE, 0);  
        break;
 
	case SE_APP_DRAW:
		// Do nothing... midd_DrawLCD() will update 
		// applet canvas display area.
		break;
        
    case SE_APP_END :   /* The very last event */ 
        
//        gext_CloseMemory();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_SinjiGame()
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
LOCAL void SKY_API sSKA_SinjiGame(SKY_EVENT_T *pEvent)
{   
    STATIC BOOL fKeyEnd = FALSE;
    static UINT8    nGameID;
    static BOOL     fPause;
    int nData;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetWindow(SKY_DEF_WIN_X,
					SKY_DEF_WIN_Y, 
					SKY_DEF_WIN_DX, 
					SKY_DEF_WIN_DY, 
					NULL,  
					NULL, // show annunciators.
					WA_NONE);
		
		SKY_FillScreenImage();
		SKY_256DisplayStart();
		EQS_SetDirectDraw(TRUE);
        //SKY_SET_KEY_TONE(FALSE);  // 01.4.27 Game 내부에서 tone을 on/off하도록 수정됨

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        SKY_SET_IN_GAME(TRUE);//fIsGame = TRUE;
        fPause = FALSE;

        nGameID = pEvent->wParam;
        glib_Init_SINJISOFT_Game();
        glib_GameEvent(GLIB_START_EVENT, pEvent->wParam);	
        break;
 
	case SE_APP_DRAW:
		// Do nothing... midd_DrawLCD() will update 
		// applet canvas display area.
		break;
        
    case SE_SYS_LOW_BATT:
        break;

    case SE_APP_END :   /* The very last event */ 
        SKY_256DisplayEnd(); 

		if(SKY_IS_IN_GAME())
        {
            glib_GameEvent(GLIB_KEY_EVENT, GLIB_KEY_END);
            SKY_SET_IN_GAME(FALSE);//fIsGame = FALSE;
        }
        glib_GameEvent(GLIB_END_EVENT, 0);
        EQS_SetDirectDraw(FALSE);
                
        gext_CloseMemory();

        SKY_SET_KEY_TONE(TRUE);

        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;

    case SE_TM_TIMER1:
        glib_GameEvent(GLIB_TIMEOUT_EVENT, 0);
        break;
    case SE_TM_TIMER2:
        glib_GameEvent(GLIB_TIMEOUT_EVENT, 1);
        break;
    case SE_TM_TIMER3:
        glib_GameEvent(GLIB_TIMEOUT_EVENT, 2);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
    
    case SE_KEY_END :
         EQS_END_TO(EQS_POP_TO_ROOT); 
        break;
    
    case SE_KEY_CLEAR:      
        if(glib_GameEvent(GLIB_KEY_EVENT, GLIB_KEY_CLR) == 1)   // End Game
        {
            SKY_SET_IN_GAME(FALSE);//fIsGame = FALSE;
            EQS_End();
            break;
        }
        else
        {
            fPause = TRUE;
            SKY_SET_NO_OFF_BACKLIGHT(FALSE);
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        }
        break;

    case SE_KEY_SEND:  
    case SE_KEY_LEFT:
    case SE_KEY_RIGHT: 
    case SE_KEY_UP: 
    case SE_KEY_DOWN:   
    case SE_KEY_1:   
    case SE_KEY_2:   
    case SE_KEY_3:   
    case SE_KEY_4:   
    case SE_KEY_5:   
    case SE_KEY_6:   
    case SE_KEY_7:   
    case SE_KEY_8:   
    case SE_KEY_9:   
    case SE_KEY_0:   
    case SE_KEY_STAR:   
    case SE_KEY_POUND:   
    case SE_KEY_CENTER:   
        if(fPause)
        {
            fPause = FALSE;
            SKY_SET_NO_OFF_BACKLIGHT(TRUE);
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        }

        switch(pEvent->EventID){
        case SE_KEY_SEND:       nData = GLIB_KEY_SND;       break;
        case SE_KEY_LEFT:       nData = GLIB_KEY_LEFT;      break;
        case SE_KEY_RIGHT:      nData = GLIB_KEY_RIGHT;     break;
        case SE_KEY_UP:         nData = GLIB_KEY_UP;        break;
        case SE_KEY_DOWN:       nData = GLIB_KEY_DOWN;      break;
        case SE_KEY_1:          nData = GLIB_KEY_1;         break;
        case SE_KEY_2:          nData = GLIB_KEY_2;         break;
        case SE_KEY_3:          nData = GLIB_KEY_3;         break;
        case SE_KEY_4:          nData = GLIB_KEY_4;         break;
        case SE_KEY_5:          nData = GLIB_KEY_5;         break;
        case SE_KEY_6:          nData = GLIB_KEY_6;         break;
        case SE_KEY_7:          nData = GLIB_KEY_7;         break;
        case SE_KEY_8:          nData = GLIB_KEY_8;         break;
        case SE_KEY_9:          nData = GLIB_KEY_9;         break;
        case SE_KEY_0:          nData = GLIB_KEY_0;         break;
        case SE_KEY_STAR:       nData = GLIB_KEY_STAR;      break;
        case SE_KEY_POUND:      nData = GLIB_KEY_SHARP;     break;
        case SE_KEY_CENTER:     nData = GLIB_KEY_OK;        break;
        }

        glib_GameEvent(GLIB_KEY_EVENT, nData);        
        break;


    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

