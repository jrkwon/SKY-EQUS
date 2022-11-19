/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_iani.c
**
** Description: 
**     IrDA Animation Transfer
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-26 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SIA_LIST_STYLE      CS_SCROLL_LIST_DEFAULT| \
                                CS_SCROLL_LIST_ENUMBERATED | \
                                CS_SCROLL_LIST_MAXIMIZE | \
                                CS_SCROLL_LIST_NO_CML
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
enum tag_SKA_IRDA_ANI_MESSAGEBOX_E {
    SIA_NOANI
};

typedef enum tag_SKA_IRDA_ANI_MESSAGEBOX_E SKA_IRDA_ANI_MESSAGEBOX_E;

enum tag_SKA_IRDA_ANI_OPTION_E {
    SIA_ALL,
    SIA_ANY
};
/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
extern HCONTROL    ghMsgLine1, ghMsgLine2, ghState;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL UINT8 l_nMyAni, l_nMyAniAlloc;
LOCAL BYTE *l_ppMyAniList[SKY_MAX_NUM_FILES+1];
LOCAL HCONTROL l_hListHandle=NULL;

//LOCAL SKA_IRDA_ANI_MESSAGEBOX_E   MessageReason;
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL BOOL EQS_API sAddIrDAAniControls(void);

LOCAL void SKY_API sSKA_IrDASendAniAll(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_IrDASendAniAny(SKY_EVENT_T *pEvent);
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

void SKY_API SKA_MainIrDAAnimation(SKY_EVENT_T *pEvent)
{
    UINT8   index;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SEND_NONE, 
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_ANIMATION), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        //SKY_SET_USE_SHARED_MEMORY(TRUE);

        if(!sAddIrDAAniControls())
        {
//            MessageReason = SIA_NOANI;
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_NOFILE), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        EQS_SetIOCActive(l_hListHandle);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(index=1; index < l_nMyAniAlloc+1; index ++)
            SKY_Free(l_ppMyAniList[index]);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_SetIOCActive(l_hListHandle);
        break;

    case SE_IO_MESGBOX_END:
//        switch(MessageReason){
//        case SIA_NOANI:
            EQS_End();
//            break;
//        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:  
        if(pEvent->dwParam != EQC_DELETED)
        {
            EQS_End();
        }
        break;
    
    case SE_IO_SCROLL_LIST_SELECT:
    case SE_SOFTKEY_SEND:  
        if(EQC_GetScrollPhysicalIndex(l_hListHandle) == 0)
        {
            EQS_START_CHILD(sSKA_IrDASendAniAll);
        }
        else
        {
            EQS_START_CHILD(sSKA_IrDASendAniAny);
        }
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*============================================================================= 
Function:    
sSKA_IrDASendAniAll

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
LOCAL void SKY_API sSKA_IrDASendAniAll(SKY_EVENT_T *pEvent)
{
    static s_nCurrentAni;
    static BOOL  fStart = TRUE;

    BYTE Str[SKY_FILE_EXT_NAME_MAX_LENGTH+2];
    WORD  ResultCode;
    IREX_RESULT_E   Result;
	IREX_STATE_E   Ret;
    /* rosa 01-08-06 --> @why, what */
    static BITMAP   bmpSend;
    /* <-- */

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_ANIMATION), 
                      WA_SHOW_HEADER);

		if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
            g_fIrDAPortForPC = FALSE;
		}

        SKY_OpenIrDAPort(0);	// Set to secondary

        s_nCurrentAni = 1;
        g_pSendFileName = l_ppMyAniList[s_nCurrentAni];

    	if(!SKY_IsDataFile(SDF_IMAGE, g_pSendFileName))
            EQS_END();

        /* rosa 01-08-17 --> @why, what */
        // Message Line
        ghState = EQC_StaticText(IRDA_STATE_X, IRDA_STATE_Y, TKN_PHONE_TO_PHONE_STATE_ANI);
        ghMsgLine1 = EQC_StaticText(IRDA_MESSAGE1_X, IRDA_MESSAGE1_Y, TKN_BLANK);
        ghMsgLine2 = EQC_StaticText(IRDA_MESSAGE2_X, IRDA_MESSAGE2_Y, TKN_BLANK);

        //EQC_SET_STYLE(ghState, CS_STATIC_TEXT_ORING);
        //EQC_SET_STYLE(ghMsgLine1, CS_STATIC_TEXT_ORING);
        //EQC_SET_STYLE(ghMsgLine2, CS_STATIC_TEXT_ORING);

        //EQC_Image(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);        

        bmpSend = g_BmpExchangeDataIrDA;
        bmpSend.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpSend.Header) * IB_SEND; //2

        EQC_Image( IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpSend);
        /* <-- */

        sprintf((CHAR *)Str, "\"%s\"", g_pSendFileName);
        EQC_SetStaticText(ghMsgLine1, Str);
        EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_FILE_SENDING));

        SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);

        gTransferType = IREX_FT_ANI;
        gSendFile = gRecvFile = NULL;
        fStart = TRUE;

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;

        /* rosa 2001-09-21 --> @why, what */
    //case SE_APP_DRAW:
        //EQS_PutBitmap(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);
        //break;
        /* <-- */

    case SE_APP_END :   /* The very last event */ 
        SKY_IrDACloseFile();

		if(!fIrDAPortClosed)
		{
			// 수신 모드로 들어간후 전화를 못받는 문제를 해결하기 위해
			if(SKY_IS_ALERT())
			{
				IrDA_Off = TRUE;
				SKY_IrDAStatusCheck_set (500);
			}
			else
				SKY_CloseIrDAPort();
		}    
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_TM_TIMER1:      // started by ServerStateMachine
    case SE_TM_TIMER2:      // ping response
        SKY_IrDAServerStateMachine(TIMEOUT_E, 0, &ResultCode);
        break;

    case SE_IO_MESGBOX_END:
        EQS_End();
        break;

    case SE_IRDA_EX_RX_DATA_F:
        Ret = SKY_IrDAServerStateMachine(GET_MSG_E, 0, &ResultCode);

		if(Ret == IREX_FAILURE)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_FAIL_SEND), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;
        }
        else if(Ret == IREX_ON_GOING)
        {
            break;
        }
        else
        {
            Result = SKY_ProcessFileSend(Ret, fStart);
            fStart = FALSE;

            switch(Result)
            {
            case IREX_ERR_FILE_VER:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_NOT_SUPPORT), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                break;
            case IREX_ERR_CLIENT:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_RECEIVER_FAIL), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                break;
            case IREX_NO_MEM_AVAIL:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NO_MEMORY), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                break;
            case IREX_ERR_FULE_FILE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_FULL_FILE), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                break;
            case IREX_FINISHED_RES:
                s_nCurrentAni ++;
                if(l_nMyAni+1 == s_nCurrentAni)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_END_SEND), 
                                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                    break;
                }
                else
                {
                    g_pSendFileName = l_ppMyAniList[s_nCurrentAni];

    	            if(!SKY_IsDataFile(SDF_IMAGE, g_pSendFileName))
                        EQS_END();

                    sprintf((CHAR *)Str, "\"%s\"", g_pSendFileName);
                    EQC_SetStaticText(ghMsgLine1, Str);

                    SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);
                    fStart = TRUE;
                }
                break;
            default:
                break;
            }
        }

        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
          
/*============================================================================= 
Function:    
sSKA_IrDASendAniAny

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
LOCAL void SKY_API sSKA_IrDASendAniAny(SKY_EVENT_T *pEvent)
{
    static BOOL  fStart = TRUE;
    BYTE Str[SKY_FILE_EXT_NAME_MAX_LENGTH+2];
    UINT8 nCurrentAni;
    WORD  ResultCode;
    IREX_RESULT_E   Result;
	IREX_STATE_E   Ret;
    /* rosa 01-08-06 --> @why, what */
    static BITMAP   bmpSend;
    /* <-- */

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_ANIMATION), 
                      WA_SHOW_HEADER);

		if(!fIrDAPortClosed)
		{
			ChangeIrDAMode ( 0 );
			// Phone to PC IrDA가 off 되었음을 표시
			fIrDAPortClosed = TRUE;
            g_fIrDAPortForPC = FALSE;
		}

        SKY_OpenIrDAPort(0);	// Set to secondary

        nCurrentAni = EQC_GetScrollPhysicalIndex(l_hListHandle);
        g_pSendFileName = l_ppMyAniList[nCurrentAni];

    	if(!SKY_IsDataFile(SDF_IMAGE, g_pSendFileName))
            EQS_END();

        // Message Line
        sprintf((CHAR *)Str, "\"%s\"", g_pSendFileName);

        /* rosa 01-08-17 --> @why, what */
        ghState = EQC_StaticText(IRDA_STATE_X, IRDA_STATE_Y, TKN_PHONE_TO_PHONE_STATE_ANI);
        ghMsgLine1 = EQC_StaticText(IRDA_MESSAGE1_X, IRDA_MESSAGE1_Y, TKN_BLANK);
        ghMsgLine2 = EQC_StaticText(IRDA_MESSAGE2_X, IRDA_MESSAGE2_Y, TKN_BLANK);

        //EQC_SET_STYLE(ghState, CS_STATIC_TEXT_ORING);
        //EQC_SET_STYLE(ghMsgLine1, CS_STATIC_TEXT_ORING);
        //EQC_SET_STYLE(ghMsgLine2, CS_STATIC_TEXT_ORING);

        //EQC_Image(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);         

        bmpSend = g_BmpExchangeDataIrDA;
        bmpSend.pbData = g_BmpExchangeDataIrDA.pbData +
                        EQC_GET_FRAME_SIZE(bmpSend.Header) * IB_SEND; //2

        EQC_Image( IRDA_PIC_X, IRDA_PIC_Y, (BITMAP*)&bmpSend);
        /* <-- */        

        EQC_SetStaticText(ghMsgLine1, Str);
        EQC_SetStaticText(ghMsgLine2, (BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_FILE_SENDING));

        SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);

        gTransferType = IREX_FT_ANI;
        gSendFile = gRecvFile = NULL;
        fStart = TRUE;

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;

        /* rosa 2001-09-21 --> @why, what */
    //case SE_APP_DRAW:
        //EQS_PutBitmap(IRDA_FRAME_X, IRDA_FRAME_Y, (BITMAP*)&g_BmpIrdaPannel);
        //break;
        /* <-- */
   
    case SE_TM_TIMER1:      // started by ServerStateMachine
    case SE_TM_TIMER2:      // ping response
        SKY_IrDAServerStateMachine(TIMEOUT_E, 0, &ResultCode);
        break;

    case SE_APP_END :   /* The very last event */ 
        SKY_IrDACloseFile();

		if(!fIrDAPortClosed)
		{
			// 수신 모드로 들어간후 전화를 못받는 문제를 해결하기 위해
			if(SKY_IS_ALERT())
			{
				IrDA_Off = TRUE;
				SKY_IrDAStatusCheck_set (500);
			}
			else
				SKY_CloseIrDAPort();
		}    

        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
      
    case SE_APP_SUSPEND: /* child applet is started */
        break;
      
    case SE_APP_RESUME: /* child applet is ended */
        break;
      
    case SE_IO_MESGBOX_END:
        EQS_End();
        break;

    case SE_IRDA_EX_RX_DATA_F:
        Ret = SKY_IrDAServerStateMachine(GET_MSG_E, 0, &ResultCode);

		if(Ret == IREX_FAILURE)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_FAIL_SEND), 
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            break;
        }
        else if(Ret == IREX_ON_GOING)
        {
            break;
        }
        else
        {
            Result = SKY_ProcessFileSend(Ret, fStart);
            fStart = FALSE;

            switch(Result)
            {
            case IREX_ERR_FILE_VER:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_NOT_SUPPORT), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                break;
            case IREX_ERR_CLIENT:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_RECEIVER_FAIL), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                break;
            case IREX_NO_MEM_AVAIL:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_PBOOK_NO_MEMORY), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                break;
            case IREX_ERR_FULE_FILE:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_FULL_FILE), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                break;
            case IREX_FINISHED_RES:
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_END_SEND), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                break;
            default:
                break;
            }
        }

        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


BOOL EQS_API sAddIrDAAniControls(void)
{
    UINT8 index;

    // Load user file list
    l_nMyAniAlloc = (UINT8)SKY_GetFileNumberInDataFile(SDF_IMAGE);

    if(!l_nMyAniAlloc)
        return FALSE;

    for(index=1; index < l_nMyAniAlloc+1; index ++)
        l_ppMyAniList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

    l_ppMyAniList[0] = (BYTE*)SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_SEND_ALL);
    l_nMyAni = SKY_GetFileExtList(SKY_IMAGE_DIR, &l_ppMyAniList[1]);

    l_hListHandle = EQC_ScrollList(EQS_NA, EQS_NA, 18, 
                   MIN(3,l_nMyAni), 
                   l_ppMyAniList, 
                   l_nMyAni+1, 
                   l_nMyAni+1,
                   0);
    EQC_SET_STYLE(l_hListHandle, SIA_LIST_STYLE);

    return TRUE;
}
