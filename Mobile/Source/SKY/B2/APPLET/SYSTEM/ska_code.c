/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_code.c
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
** 4333-12-21 white     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"
#ifndef WIN32
#include "dsskt.h"
#endif

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#define ALLCODE     0
#define INTERCODE   1
#define PBOOKCODE   2
#define MENUCODE    3
#define STACKCODE   4
#define RECCODE     5
#define MEMOCODE    6
#define NTOPCODE    7
#define SMSCODE     8
#define SYSTEM_SET   9
#define PBOOKCODEIRDA   10

#define MAX_EMERGENCY_CODE  3

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
BYTE* pszEmergencyCode[MAX_EMERGENCY_CODE] 
                                 = {(BYTE*)"112", (BYTE*)"113", (BYTE*)"119"};



/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
extern VOID SKA_SMSMenu(SKY_EVENT_T* pEvent);
extern VOID SKY_API SKA_MainCallList(SKY_EVENT_T *pEvent);

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

UINT8   IS_PLAY = 0;
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

LOCAL void SKY_API sSKA_Enter(SKY_EVENT_T *pEvent);


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
#define INPUT_CODE_MSG_POS_X  48
#define INPUT_CODE_MSG_POS_Y  IMAGE_Y

void SKY_API TextOutInputCodeMessage(void)
{
    EQS_TextOutStr(INPUT_CODE_MSG_POS_X, INPUT_CODE_MSG_POS_Y, 
                   (BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_1));
    EQS_TextOutStr(INPUT_CODE_MSG_POS_X, (INPUT_CODE_MSG_POS_Y + EQS_CharHeight('A')), 
                   (BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_2));
    EQS_TextOutStr(INPUT_CODE_MSG_POS_X, (INPUT_CODE_MSG_POS_Y + EQS_CharHeight('A')*2), 
                   (BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_3));
}

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
#define sALL_CODE_Y_MARGIN               15

#define sALL_CODE_TITLE_BOX_MARGINE      2

#define INPUT_CODE_MSG_AUTO_LOCK_POS_X          48
#define INPUT_CODE_MSG_AUTO_LOCK_POS_Y          (IMAGE_Y + sALL_CODE_Y_MARGIN)
#define INPUT_CODE_MSG_AUTO_LOCK_TITLE_POS_X    (SKY_SCREEN_WIDTH/2 - (STRLEN(SKY_GET_TOKEN(TKN_LOCK_AUTO_TITLE))*EQS_CharWidth('A'))/2)
#define INPUT_CODE_MSG_AUTO_LOCK_TITLE_POS_Y    18

#define sALL_LOCK_TITLE_LENGTH                  (STRLEN(SKY_GET_TOKEN(TKN_LOCK_AUTO_TITLE))*EQS_CharWidth('A'))
#define sALL_LOCK_TITLE_BOX_TOP_X               (INPUT_CODE_MSG_AUTO_LOCK_TITLE_POS_X-sALL_CODE_TITLE_BOX_MARGINE)
#define sALL_LOCK_TITLE_BOX_TOP_Y               (INPUT_CODE_MSG_AUTO_LOCK_TITLE_POS_Y-sALL_CODE_TITLE_BOX_MARGINE)

#define sALL_LOCK_TITLE_BOX_BOTTOM_X            (sALL_LOCK_TITLE_BOX_TOP_X + sALL_LOCK_TITLE_LENGTH +sALL_CODE_TITLE_BOX_MARGINE*2)
#define sALL_LOCK_TITLE_BOX_BOTTOM_Y            (sALL_LOCK_TITLE_BOX_TOP_Y + EQS_CharHeight('A') + sALL_CODE_TITLE_BOX_MARGINE)        


LOCAL void SKY_API sTextOutInputCodeMessageInAutoLock(void)
{
    EQS_Rectangle(sALL_LOCK_TITLE_BOX_TOP_X,  
                  sALL_LOCK_TITLE_BOX_TOP_Y,
                  sALL_LOCK_TITLE_BOX_BOTTOM_X, 
                  sALL_LOCK_TITLE_BOX_BOTTOM_Y);

    EQS_TextOutStr(INPUT_CODE_MSG_AUTO_LOCK_TITLE_POS_X,
                   INPUT_CODE_MSG_AUTO_LOCK_TITLE_POS_Y,
                   (BYTE*)SKY_GET_TOKEN(TKN_LOCK_AUTO_TITLE));   
    
    EQS_TextOutStr(INPUT_CODE_MSG_AUTO_LOCK_POS_X, INPUT_CODE_MSG_AUTO_LOCK_POS_Y, 
                   (BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_1));
    EQS_TextOutStr(INPUT_CODE_MSG_AUTO_LOCK_POS_X, (INPUT_CODE_MSG_AUTO_LOCK_POS_Y + EQS_CharHeight('A')), 
                   (BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_2));
    EQS_TextOutStr(INPUT_CODE_MSG_AUTO_LOCK_POS_X, (INPUT_CODE_MSG_AUTO_LOCK_POS_Y + EQS_CharHeight('A')*2), 
                   (BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_3));
}






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
BOOL IsEmergencyCall(BYTE sPasswd[])
{
  UINT8 i;

  for(i=0; i<MAX_EMERGENCY_CODE; i++)
  {
    if(STRCMP(sPasswd, pszEmergencyCode[i]) == 0) return TRUE;
  }

  return FALSE;
}


// All Message Delete Routine.
VOID SKY_API SKA_DeleteAllMsgCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_CANCEL, 
            SKY_GET_TOKEN(TKN_SMS_MENU5), 
            WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

        //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
        sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
        //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

        EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
        EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
        EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
		EQS_SetIOCActive(sNumHandle);
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
      
    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
        
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_START_SIBLING(SKA_DeleteAllMsg, FALSE);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


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
void SKY_API SKA_MainAllCode(SKY_EVENT_T *pEvent)
{
	STATIC  UINT8              nPreviousMinute;
    
	clk_julian_type            julian;
	DWORD                      dwTime;
	ALARM_DISP_TYPE_T          AlarmType;


    BYTE   sPasswd[5];
    HCONTROL NumHandle;
    UINT8    nNumDigit;
    
    switch(pEvent->EventID)
    { 
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE, 
                NULL, 
                (WA_SHOW_HEADER|WA_SHOW_CML));

        EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y + sALL_CODE_Y_MARGIN, 
               (BITMAP*)&g_BmpLock
			   //(BITMAP*)&g_BmpSecret120x106x256c
			  ); 


        //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
        NumHandle = EQC_NumberEdit(PASS_X, PASS_Y+sALL_CODE_Y_MARGIN, 4, 1, 4,(BYTE *)"");  //EQC_NumberEdit(44, 73, 4, 1, 4,(BYTE *)"");
        //EQC_SetNumberEditBackColor(NumHandle, NUMBER_EDIT_BACK_COLOR);

        EQC_SET_STYLE(NumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
        EQC_SetNumberEditBigFont(NumHandle, TRUE);   // Set Big Font
        EQC_NotifyNumberEditMatchLen(NumHandle, 4);
        EQS_SetIOCActive(NumHandle);
#ifdef FEATURE_SKY_SUBLCD
        SKY_DrawSubAnnunciators();
        SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_AUTO_LOCK);
#endif //FEATURE_SKY_SUBLCD

		dwTime = SKY_GetLocalTime();
		clk_secs_to_julian(dwTime, &julian);
		nPreviousMinute = julian.minute;
        break;

    case SE_APP_DRAW :
        sTextOutInputCodeMessageInAutoLock();
        SKY_IdleScreenNotify();
        break;

    case SE_SYS_PACEMAKER:  //2001.08.2 redstar
#ifdef FEATURE_WAP
        if(SKY_IS_IN_WAP()) 
        {
			EQS_DEF_EVENT_PROC(pEvent);
			break;
		}
#endif //FEATURE_WAP
		dwTime = SKY_GetLocalTime();
		clk_secs_to_julian(dwTime, &julian);
		if( nPreviousMinute != julian.minute)
		{
			nPreviousMinute = julian.minute;

            if( SKY_GetActiveAlarmFlag() == FALSE )
			{
				sFindAlarmOn( &julian, &AlarmType );
				if( AlarmType.Type > 0 )
				{
					EQS_START_CHILD_EVENT( sSKA_PlayAlarm, SE_APP_START, AlarmType.Type, AlarmType.nAddr );
					break;
				}
			}		
		}        
		EQS_DEF_EVENT_PROC(pEvent);
		break;
        
    case SE_KEY_MANNERS:
        break;

    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            SKY_SET_SHORTLOCK(FALSE);
            SKY_SET_AUTO_LOCK_MODE(FALSE);
			if( EQS_GET_PARENT() == SKA_MissedCall )  //redstar 2001.08.21
			{
				EQS_END_EVENT(SE_APP_RESUME, SE_IO_NEDIT_MATCH_LENGTH, EQS_NA );
			}
			else
			{
				EQS_End();
			}            

#ifdef FEATURE_SKY_SUBLCD
            SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD
        }   
            //EQS_START_SIBLING_EVENT(sSKA_Enter, EQE_APP_START, ALLCODE, 0 , FALSE);              
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_KEY_SEND:
        nNumDigit = EQC_GetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), sPasswd);
        if(IsEmergencyCall(sPasswd))
        {
          if(sPasswd[0] != '\0')
            {
//                SKY_SET_ORIGINATE(TRUE);
                SET_NUMEDIT_BUFFER(sPasswd, nNumDigit);
                SET_NUMEDIT_POS(nNumDigit);                   
                                
                SET_NUMEDIT_SECRET(FALSE);
                SET_ALPHAEDIT_SECRET(FALSE);
                EQS_START_SIBLING(SKA_StartOrigination, FALSE);

#ifdef FEATURE_SKY_SUBLCD
                SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD

//                ui_orig_voice_call(GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,  
//                                   GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
//                EQS_START_CHILD(SKA_MainCall);
            }
        }
        break;

    case SE_SYS_TO_IDLE: /* ignore this event */
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_IO_TO_IDLE:
    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_END:
#ifdef FEATURE_UI_DORMANT
        if(pEvent->EventID == SE_KEY_END && SKY_IS_DORMANT())
        {
            SKY_SET_DORMANT(FALSE);
            SKY_SetThenCompareInUseState((IN_USE_STATE_E)SKY_GetInUseState());
            SetUIPktRelease();
        }
#endif
        if(SKY_IsInCall())
            EQS_END_CURRENT_EVENT();
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;        

    case SE_CALL_COMMAND_ERROR : /* UI_CALL_CMD_ERR_E: */
    case SE_CALL_ENDED:
#ifdef FEATURE_UI_DORMANT
        if(pEvent->EventID == SE_KEY_END && SKY_IS_DORMANT())
        {
            SKY_SET_DORMANT(FALSE);
            SKY_SetThenCompareInUseState((IN_USE_STATE_E)SKY_GetInUseState());
            SetUIPktRelease();
        }
#endif
            EQS_END_CURRENT_EVENT();
        break;


    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        break;
    
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;        
	case SE_APP_RESUME:
		if( EQS_GET_CHILD() == sSKA_PlayAlarm )
		{
#ifdef FEATURE_SKY_SUBLCD
			SKY_DrawAnnunciators();
			SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_AUTO_LOCK);
#endif //FEATURE_SKY_SUBLCD
			EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
		}
		break;                        
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_TIMEOUT:
		CheckCurrentTime();
		break;
#endif //#ifdef FEATURE_SKTT_LOCATION
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

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
void SKY_API SKA_MainInterCode(SKY_EVENT_T *pEvent)
{
    BYTE                        sPasswd[5];
    HCONTROL                    sNumHandle;
    EDITORINFO_T                CurrNumInfo;
    BYTE                        aPhoneNumber[NE_MAX_SIZE];
	UINT8                       nRealPhoneNumberLength;
    STATIC  WORD                s_wWhere;
    STATIC  HCONTROL            hCtrlNoService;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        CurrNumInfo = GET_NUMEDIT;
        if(!SKY_IS_OVERSEAS_CALL_LOCK())
        {
//            SKY_SET_ORIGINATE(TRUE);     
//            ui_orig_voice_call ( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,  
//                                     GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
              EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        }
        else
        { 

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_INTERCALLLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);


            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
    
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hCtrlNoService)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;
        }

        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            if(s_wWhere == INTERCODE_IN_CALL)
            {
                EQS_END_EVENT(SE_KEY_SEND, INTERCODE_IN_CALL, EQS_NA);
            }
            else
            {                
                if(SKY_IS_NOSERVICE() == FALSE)
                {
                    SKY_SET_ORIGINATE(TRUE); 
                    if(IsPauseMode(CurrNumInfo.aBuf))   /* Paused Mode 일 경우 */
                    {
                        nRealPhoneNumberLength = ParsePhoneNumber(CurrNumInfo.aBuf, aPhoneNumber);
                        ui_orig_voice_call(aPhoneNumber, nRealPhoneNumberLength,  
                                           GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
                    }
                    else                            /* Paused Mode가 아닐 경우 */
                    {   ui_orig_voice_call ( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,  
                                              GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
                    }
                }
                else  /* No Service에 빠졌을 경우 */
                {
                    hCtrlNoService = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
		                             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
                }
            }
        }
        else
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        s_wWhere = 0;
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
    case SE_IO_TO_IDLE:
    case SE_KEY_EAR_LONG_CLICK:
        if(SKY_IsInCall())
            EQS_END_CURRENT_EVENT();
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;        

    case SE_CALL_COMMAND_ERROR : /* UI_CALL_CMD_ERR_E: */
    case SE_CALL_ENDED:
         EQS_END_CURRENT_EVENT();
        break;        

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


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
void SKY_API SKA_MainPbookCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;
    STATIC  WORD    s_wEnterKind;
    STATIC  DWORD   s_dwData;
    STATIC  HCONTROL    s_hNotSpeedDial;
    pb_sdial_type          VarSpeedDial;
    

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_IS_PHONE_BOOK_LOCK())
        {
            EQS_START_SIBLING_EVENT(sSKA_Enter, EQE_APP_START, PBOOKCODE, 0 , FALSE);  
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_PBOOKLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            s_wEnterKind = pEvent->wParam;
            if(s_wEnterKind == BOOKCODE_VIEW_IN_LIST ||s_wEnterKind == SE_IO_NEDIT_PRESS_LONGKEY || \
               s_wEnterKind == SE_KEY_SEND || s_wEnterKind == BOOKCODE_IN_MO || \
               s_wEnterKind == BOOKCODE_IN_EMAIL || s_wEnterKind == BOOKCODE_IN_PHOTOMAIL||\
               s_wEnterKind == BOOKCODE_IN_SCHEDULE)
                s_dwData = pEvent->dwParam;
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;
        
    case SE_IO_MESGBOX_END:
        if(s_hNotSpeedDial == pEvent->wParam)
        {
            EQS_END_EVENT(SE_APP_RESUME, BOOKCODE_IN_DIALING, EQS_NA);
        }else
        {
            EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
            EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            if(s_wEnterKind == BOOKCODE_IN_CALL)
                EQS_START_SIBLING(SKA_BookInCalling, FALSE);              
            else if(s_wEnterKind == BOOKCODE_NEW_IN_LIST)
                EQS_START_SIBLING_EVENT(SKA_BookInput, SE_APP_START, 0, BFT_SAVE_NEW_DATA_E, FALSE);
            else if(s_wEnterKind == BOOKCODE_OLD_IN_LIST)
                EQS_START_SIBLING_EVENT(SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_SAVE_OLD_DATA_E, FALSE);
            else if(s_wEnterKind == BOOKCODE_VIEW_IN_LIST)
                EQS_END_EVENT(SE_SOFTKEY_PBOOK,  BOOKCODE_VIEW_IN_LIST, EQS_NA); /////////////////
            else if(s_wEnterKind == BOOKCODE_SEARCH_IN_IDLE)
                EQS_END_EVENT(SE_APP_RESUME , BOOKCODE_RETURN_SEARCHING, NULL);
            else if(s_wEnterKind == BOOKCODE_SEARCH_IN_CBELL)
                EQS_END_EVENT(SE_SOFTKEY_PBOOK, BOOKCODE_SEARCH_IN_CBELL, EQS_NA);
            else if(s_wEnterKind == SE_IO_NEDIT_PRESS_LONGKEY)
                EQS_END_EVENT( SE_IO_NEDIT_PRESS_LONGKEY, SE_IO_NEDIT_PRESS_LONGKEY, s_dwData);
            else if(s_wEnterKind == SE_KEY_SEND)
            {
                VarSpeedDial.address = s_dwData;
	            if( ui_get_pb( PB_SDIAL_I, &VarSpeedDial))
                {                    
                    if( VarSpeedDial.dial_address > 0 && VarSpeedDial.dial_address <= MAX_PHONE_NUMBER )
                    {
                        SpeedDial( s_dwData ); 
                    }
                    else
                    {
                        s_hNotSpeedDial = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NONVALID_SDIAL_MSG),\
                                                         MESSAGE_DEFAULT_TIME,\
                                                         MBT_NONE, MBT_HELP);	 

                    }
                }else
                {
                    s_hNotSpeedDial = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NONVALID_SDIAL_MSG),\
                                   MESSAGE_DEFAULT_TIME,\
                                   MBT_NONE, MBT_HELP);	                
                }
                //EQS_END_EVENT( SE_KEY_SEND, SE_KEY_SEND, s_dwData);
            }else if(s_wEnterKind == BOOKCODE_IN_MO || \
               s_wEnterKind == BOOKCODE_IN_EMAIL )
                EQS_END_EVENT( SE_APP_RESUME, s_wEnterKind, s_dwData);
            else if(s_wEnterKind == BOOKCODE_IN_PHOTOMAIL)
			{
                //EQS_START_SIBLING(SKA_SearchBookNumber, FALSE);
				EQS_START_SIBLING_EVENT( SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_SEARCH_BOOK_E, FALSE);
			}
            else if(s_wEnterKind == BOOKCODE_IN_DIAL)
                EQS_END_EVENT( SE_IO_NEDIT_DONE, s_wEnterKind, s_dwData);
            else if(s_wEnterKind ==  BOOKCODE_IN_SCHEDULE)
                EQS_START_SIBLING_EVENT(SKA_ViewBookContent, SE_APP_START, s_dwData, BFT_EDIT_PBOOK_E, FALSE);
            else if(s_wEnterKind == BOOKCODE_IN_VM)
                EQS_START_SIBLING(SKA_ViewEmailList, FALSE);
            else if(s_wEnterKind == BOOKCODE_IN_DATASRV)
                EQS_END_EVENT( SE_SOFTKEY_FIND, s_wEnterKind, s_dwData);
            else
                EQS_START_SIBLING_EVENT(sSKA_Enter, EQE_APP_START, PBOOKCODE, 0 , FALSE);              
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        s_wEnterKind = 0;
        s_dwData = 0;
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
      
    case SE_SYS_TO_IDLE: /* ignore this event */
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
    case SE_IO_TO_IDLE:
    case SE_KEY_EAR_LONG_CLICK:
        if(SKY_IsInCall())
            EQS_END_CURRENT_EVENT();
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;        

    case SE_CALL_COMMAND_ERROR : /* UI_CALL_CMD_ERR_E: */
    case SE_CALL_ENDED:
         EQS_END_CURRENT_EVENT();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainSelfInfoCode4View(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_GET_SELFINFOLOCK())
        {
            EQS_START_SIBLING(SKA_ViewPersonInfo, FALSE);  
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_SELF_INFOLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            EQS_START_SIBLING(SKA_ViewPersonInfo, FALSE);     
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
      
    case SE_SYS_TO_IDLE: /* ignore this event */
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
    case SE_IO_TO_IDLE:
    case SE_KEY_EAR_LONG_CLICK:
        if(SKY_IsInCall())
            EQS_END_CURRENT_EVENT();
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;    

    case SE_CALL_COMMAND_ERROR : /* UI_CALL_CMD_ERR_E: */
    case SE_CALL_ENDED:
         EQS_END_CURRENT_EVENT();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainMySelfInfoCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_GET_SELFINFOLOCK())
        {
            EQS_START_SIBLING(SKA_MainIrDABookMyInfo, FALSE);  
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_SELF_INFOLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            EQS_START_SIBLING(SKA_MainIrDABookMyInfo, FALSE);     
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
      
    case SE_SYS_TO_IDLE: /* ignore this event */
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

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
void SKY_API SKA_MainPbookCodeInIrda(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;
    

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_IS_PHONE_BOOK_LOCK())
        {
            EQS_START_SIBLING_EVENT(sSKA_Enter, EQE_APP_START, PBOOKCODEIRDA, 0 , FALSE);  
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_PBOOKLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            EQS_START_SIBLING_EVENT(sSKA_Enter, EQE_APP_START, PBOOKCODEIRDA, 0 , FALSE);              
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
      
    case SE_SYS_TO_IDLE: /* ignore this event */
        break;
       
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
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
void SKY_API SKA_MainMenuCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;
    STATIC WORD s_wKind;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_IS_MENU_LOCK())
        {
            EQS_START_SIBLING(SKA_MainMenuTop, FALSE);
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_MENULOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            s_wKind = pEvent->wParam;
            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
    
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
     
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            if(MENUCODE_IN_CALL == s_wKind)
            {
                EQS_END_EVENT( SE_SOFTKEY_MENU, s_wKind, EQS_NA);                
            }else
                EQS_START_SIBLING(SKA_MainMenuTop, FALSE);
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break; 

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
        
    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
    case SE_IO_TO_IDLE:
    case SE_KEY_EAR_LONG_CLICK:
        if(SKY_IsInCall())
            EQS_END_CURRENT_EVENT();
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;      

    case SE_CALL_COMMAND_ERROR : /* UI_CALL_CMD_ERR_E: */
    case SE_CALL_ENDED:
        EQS_END_CURRENT_EVENT();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


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
void SKY_API SKA_MainStackCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_IS_CALL_STACK_LOCK())
        {
            EQS_START_SIBLING(SKA_MainCallList, FALSE);
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_CSTACKLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
   
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_START_SIBLING(SKA_MainCallList, FALSE);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

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
void SKY_API SKA_MainRecCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;
    STATIC WORD s_wKind;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_RECLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
        
        s_wKind = pEvent->wParam;

        //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
        sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
        //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

        EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
        EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
        EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
		EQS_SetIOCActive(sNumHandle);

#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_IN_IDLE_RECORD_LOCK);
#endif //FEATURE_SKY_SUBLCD
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
     
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            if(s_wKind == RECCODE_IN_CALL)
            {
                EQS_START_SIBLING_EVENT(SKA_MainCallRecord, SE_KEY_RECORD_LONG , 0, 0 , FALSE);
                break;
            }

            if(IS_PLAY == 2)
                EQS_START_SIBLING_EVENT(SKA_MainVMemo, SE_KEY_PLAY , 0, 0 , FALSE);
            else if(IS_PLAY == 1)
                EQS_START_SIBLING_EVENT(SKA_MainVMemo, SE_KEY_RECORD, 0, 0 , FALSE);
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_KEY_RECORD:
        IS_PLAY = 1;
        break;
    case SE_KEY_PLAY:
        IS_PLAY = 2;
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_SYS_TO_IDLE: /* ignore this event */
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
    case SE_IO_TO_IDLE:
    case SE_KEY_EAR_LONG_CLICK:
        if(SKY_IsInCall())
            EQS_END_CURRENT_EVENT();
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;        

    case SE_CALL_COMMAND_ERROR : /* UI_CALL_CMD_ERR_E: */
    case SE_CALL_ENDED:
        EQS_END_CURRENT_EVENT();
        break;

    case SE_APP_END :   /* The very last event */ 
        s_wKind = 0;
        EQS_ReleaseDC();
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


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
void SKY_API SKA_MainMemoCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_IS_MEMO_LOCK())
        { 
            EQS_START_SIBLING(SKA_MainMemo, FALSE);
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_MEMOLOCK_STAT),
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
    
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_START_SIBLING(SKA_MainMemo, FALSE);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
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
void SKY_API SKA_MainCameraCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_IS_CAMERA_LOCK())
        { 
            EQS_START_SIBLING(SKA_MainMenuCamera, FALSE);
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_CAMERALOCK_STAT),
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
    
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_START_SIBLING(SKA_MainMenuCamera, FALSE);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



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
void SKY_API SKA_MainNtopCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;
    STATIC WORD wConnectKind = 0;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_IS_WAP_LOCK())
        {
#ifdef FEATURE_WAP
            EQS_START_SIBLING( SKA_MainWAP, FALSE); 
#endif //FEATURE_WAP
        }
        else
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_NTOPLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
            
            wConnectKind = pEvent->wParam;
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
   
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
#ifdef FEATURE_WAP

        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            if(wConnectKind == INITURL_IS_WAPPUSH)
                EQS_START_SIBLING_EVENT(SKA_MainWAP, SE_APP_START,
                                            EQS_NA,INITURL_IS_WAPPUSH ,FALSE);
            else if(wConnectKind == INITURL_IS_WAPMAIL)
                EQS_START_SIBLING_EVENT(SKA_MainWAP, SE_APP_START,
                                            EQS_NA,INITURL_IS_WAPMAIL ,FALSE);
            else if(wConnectKind == NATE_CODE_CHOOSE_MENU)
                EQS_START_SIBLING_EVENT(SKA_ChooseMenu,SE_APP_START, NATE_CODE_CHOOSE_MENU, EQS_NA, FALSE);                 

            else
                EQS_START_SIBLING( SKA_MainWAP, FALSE); 
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
#endif //FEATURE_WAP
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#ifdef FEATURE_SKTT_EQUS_LOCATION
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
VOID SKY_API SKA_LocationCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    LOCATION_INFO_T*         pLocation;
    
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_CANCEL, 
            SKY_GET_TOKEN(TKN_LOCATION_SETTING), 
            WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

        //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
        sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
        //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

        EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
        EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
        EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
		EQS_SetIOCActive(sNumHandle);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
      
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
   		pLocation = &SKY_GET_LOCATION_INFO();

        if(!pLocation->rcvinfo)     //2001.06.27 추가 
        {
            EQS_END();               
        }
        else
        {
            EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
            if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            {
                EQS_START_SIBLING(SKA_LocationView, FALSE);
            }
            else
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        }
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

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
void SKY_API SKA_MainSmsCode(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;
    STATIC UINT8    s_nSMSWhere;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        s_nSMSWhere = pEvent->wParam;
        if(!SKY_IS_SMS_LOCK())
        {
            if(s_nSMSWhere == SMSCODE_KEY)
                //EQS_START_SIBLING_EVENT(SKA_MainSMS,SE_SOFTKEY_SMS,0,0, FALSE);
				EQS_START_SIBLING(SKA_SMSMenu,FALSE);
            else if(s_nSMSWhere == SMSCODE_MESSAGE)
                EQS_START_SIBLING_EVENT(SKA_MessageNotif,SE_SOFTKEY_SELECT,0,0, FALSE);
				
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_SMSLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
    
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
        {
            if(s_nSMSWhere == SMSCODE_KEY)
            {
                //EQS_START_SIBLING_EVENT(SKA_MainSMS,SE_SOFTKEY_SMS,0,0, FALSE);
				EQS_START_SIBLING(SKA_SMSMenu, FALSE);
            }else if(s_nSMSWhere == SMSCODE_MESSAGE)
            {
                EQS_START_SIBLING_EVENT(SKA_MessageNotif,SE_SOFTKEY_CONFIRM,0,0, FALSE);
                SKY_SET_SMS_LOCK_MODE(FALSE);
            }else if(s_nSMSWhere == SMSCODE_IN_LIST)
            {
                EQS_START_SIBLING(SKA_SMSEditMessage, FALSE);
            }else if(s_nSMSWhere == SMSCODE_IN_DETAIL_LIST)
            {
                EQS_START_SIBLING(SKA_SMSEditMessage, FALSE);
            }else if( s_nSMSWhere == SMSCODE_IN_VIEW_CONTENT)
            {
                EQS_END_EVENT(SE_IO_POPUP_SELECT, SMSCODE_IN_VIEW_CONTENT, TKN_PBOOK_SEND_SMS);
            }else if( s_nSMSWhere == SMSCODE_IN_VIEW_SEARCH_NAME)
            {
                EQS_END_EVENT(SE_IO_POPUP_SELECT, SMSCODE_IN_VIEW_SEARCH_NAME, TKN_PBOOK_SEND_SMS);
            }else if( s_nSMSWhere == SMSCODE_IN_VIEW_SEARCH_NUM)
            {
                EQS_END_EVENT(SE_IO_POPUP_SELECT, SMSCODE_IN_VIEW_SEARCH_NUM, TKN_PBOOK_SEND_SMS);
            }else if( s_nSMSWhere == SMSCODE_IN_VIEW_BOOK_LIST)
            {
                EQS_END_EVENT(SE_IO_POPUP_SELECT, SMSCODE_IN_VIEW_BOOK_LIST, TKN_PBOOK_SEND_SMS);
            }else if(s_nSMSWhere == SMSCODE_IN_VIEW_BOOK_LIST_EMAIL)
            {
                EQS_END_EVENT(SE_SOFTKEY_SMS, SMSCODE_IN_VIEW_BOOK_LIST_EMAIL, EQS_NA);
            }
        }
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        s_nSMSWhere = SMSCODE_INIT;
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

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
void SKY_API SKA_MainSmsCode2(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        if(!SKY_IS_SMS_LOCK())
        {
            EQS_START_SIBLING_EVENT(sSKA_Enter,EQE_APP_START, SMSCODE,0, FALSE);
        }
        else
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_SMSLOCK_STAT), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;
     
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_START_SIBLING_EVENT(sSKA_Enter,EQE_APP_START, SMSCODE,0, FALSE);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

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
void SKY_API SKA_MainSystemSet(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_SYSTEM_SET), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
        
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_START_SIBLING_EVENT(sSKA_Enter,EQE_APP_START, SYSTEM_SET,0, FALSE);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
/*============================================================================= 
Function:    
sSKA_Xxx

  Description: 
  @Description
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      tag_PASS_INPUT_E
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
LOCAL void SKY_API sSKA_Enter(SKY_EVENT_T *pEvent)
{
    HCONTROL hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);

        switch(pEvent->wParam)
        {
        case ALLCODE:
            break;

        case INTERCODE:
            break;

        case PBOOKCODE:
#if (SKY_MODEL_ID == SKY_MODEL_B1 || SKY_MODEL_ID == SKY_MODEL_B2)
            hControl = EQC_MenuList(6, 4, 18, 6, &mnPhoneBook, 0,SKY_GET_SEL_MENU_MODE());
	        if(SKY_GET_SERVICE_PROVIDER() == SP_STI)
			{
				EQC_SetMenuItemText(hControl, 4, (BYTE*)SKY_GET_TOKEN(TKN_017_SERVICE));
			}						
            EQC_SetMenuImage(hControl, &g_BmpPBookMenuBase);//&g_BmpPbookBase112x88x256c);
//			EQC_SetMenuBackColor( hControl, MENU_BACK_COLOR );
#else
            hControl = EQC_MenuList(6, 4, 18, 7, &mnPhoneBook, 0,SKY_GET_SEL_MENU_MODE());
			EQC_SetMenuImage(hControl, &g_BmpPBookMenuBase);//&g_BmpPbookBase112x88x256c);
#endif
            break;

        case MENUCODE:
#if (SKY_MODEL_ID == SKY_MODEL_B1 || SKY_MODEL_ID == SKY_MODEL_B2)
            hControl = EQC_MenuList(6, 4, 18, 6, &mnMainMenuTop, 0,SKY_GET_SEL_MENU_MODE());
			EQC_SetMenuImage(hControl, &g_BmpMainMenuBase);
#else
            EQC_MenuList(6, 4, 18, 7, &mnMainMenuTop, 0,SKY_GET_SEL_MENU_MODE());
#endif
            break;

        case STACKCODE:
            //EQC_MenuList(6, 4, 18, 7, &mnMainMenuTop, 0);
            break;

        case RECCODE:
            //EQC_MenuList(6, 4, 18, 7, &mnMainMenuTop, 0);
            break;

        case MEMOCODE:
            //EQS_START_SIBLING(SKA_MainMemo, FALSE);
            break;

        case NTOPCODE:
            //EQC_MenuList(6, 4, 18, 7, &mnMainMenuTop, 0);
            break;

        case SMSCODE:
            /* jrkwon : 2001-04-13
            ** SMS use the scroll list control to display menu item instead of menu list control.
            ** so, we don't need to check SMSCODE
            */
//#if (SKY_MODEL_ID == SKY_MODEL_B1)
//            EQC_MenuList(6, 4, 18, 6, &mnSMS, 0,SKY_GET_SEL_MENU_MODE());
//#else
//            EQC_MenuList(6, 4, 18, 7, &mnSMS, 0,SKY_GET_SEL_MENU_MODE());
//#endif
            break;

        case PBOOKCODEIRDA:
            EQC_MenuList(6, 4, 18, 6, &mnIrDABook, 0,SKY_GET_SEL_MENU_MODE());
            break;

        case SYSTEM_SET:
            //EQC_MenuList(6, 4, 18, 6, &mnSystemSet, 0);
            EQC_MenuList(6, 4, 18, 6, &mnSystemSetting, 0,SKY_GET_SEL_MENU_MODE());
            break;

        }

        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_RESUME:
        hControl = EQS_GetNthIOC(0);
        EQS_SetIOCActive(hControl);
        break;

    case SE_IO_MENU_LIST_SELECT:
        EQS_START_CHILD(pEvent->dwParam);
        break;
        
    case SE_IO_MENU_LIST_CANCEL:
        EQS_END();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

