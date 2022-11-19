/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_mbel.c
**
** Description: 
**     MyBell Menu (나만의벨관리)
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-12 woodstok     Created.
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
#define SMB_SCROLL_LIST_STYLE   CS_SCROLL_LIST_BORDER| \
                                CS_SCROLL_LIST_SCROLL_BAR | \
                                CS_SCROLL_LIST_ENUMBERATED | \
                                CS_SCROLL_LIST_MAXIMIZE | \
                                CS_SCROLL_LIST_NO_CML | \
                                CS_SCROLL_LIST_STATUS_BAR
                                
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tag_SKA_MYBELL_MESSAGEBOX_E {
    SMM_NOBELL,
    SMM_DELETE_CONFIRM,
    SMM_DELETE_CONFIRM_ALL,
    SMM_DELETED,
    SMM_DELETED_ALL,
    SMM_RENAME,
    SMM_RENAME_OK,
    SMM_NONE
};

typedef enum tag_SKA_MYBELL_MESSAGEBOX_E SKA_MYBELL_MESSAGEBOX_E;

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

LOCAL BYTE *ppMyBellList[SKY_MAX_NUM_FILES];
LOCAL UINT8 s_nMyBell, s_nMyBellAlloc;

LOCAL HCONTROL s_hListHandle;

LOCAL SKA_MYBELL_MESSAGEBOX_E   MessageReason;
LOCAL BYTE szChangedName[SKY_FILE_NAME_MAX_LENGTH];

LOCAL BYTE      WithoutIcon[SKY_FILE_NAME_MAX_LENGTH+2];
#ifdef FEATURE_CHARACTER_BELL
LOCAL BYTE      WithIcon[SKY_FILE_NAME_MAX_LENGTH+2];
#endif // FEATURE_CHARACTER_BELL
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL BOOL EQS_API sAddMyBellControls(void);
LOCAL void SKY_API sSKA_MyBellRename(SKY_EVENT_T *pEvent);
LOCAL void sFormatBellControls(void);

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
void SKY_API SKA_MainMyBell(SKY_EVENT_T *pEvent)
{
    UINT8 index;
    BYTE pszTmpBuffer[20];
    BYTE pszMessage[MESSAGE_BOX_MAX_MESG_LEN];
    COORD   x, y;
    pb_group_info_type  GroupInfo;
    static UINT16 nBellIndex;
#ifdef FEATURE_CHARACTER_BELL
    HCONTROL        hControl;
#endif // FEATURE_CHARACTER_BELL

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_MENU_NONE_NONE, 
                      SKY_GET_TOKEN(TKN_BELLVIB_MYBELL), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        if(!sAddMyBellControls())
        {
            MessageReason = SMM_NOBELL;
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_NOMYBELL), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);

            break;
        }

        nBellIndex = 0;
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        SKY_StopMusic();

        for(index=0; index < s_nMyBellAlloc; index ++) 
            SKY_Free(ppMyBellList[index]);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(pEvent->wParam)
        {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
            if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                strcpy(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL
            if(STRCMP(WithoutIcon, szChangedName))
            {
                MessageReason = SMM_RENAME;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAMING), 
                                0, MBT_NONE, MBT_HELP);
            }
            else
                EQS_SetIOCActive(s_hListHandle);
        }

        EQS_SetIOCActive(s_hListHandle);

        nBellIndex = EQC_GetScrollLogicalIndex(s_hListHandle);
        
        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
        if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL

        if(SKY_GetDataFileSize(SDF_MELODY, WithoutIcon) < 1024)
            sprintf((char*)pszTmpBuffer, 
                    "%s %5ld B", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    SKY_GetDataFileSize(SDF_MELODY, WithoutIcon));
        else
            sprintf(pszTmpBuffer, 
                    "%s %5ldKB", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    GET_KB_SIZE(SKY_GetDataFileSize(SDF_MELODY, WithoutIcon)));

        EQC_SetScrollStatusBarText(s_hListHandle, pszTmpBuffer);
        break;

    case SE_SOFTKEY_MENU: 
        // 1 벨삭제
        // 2 착신벨지정
        // 3 파일이름변경
        // 4 사용자파일정보
        // 5 벨 전부삭제 
        {
#ifdef FEATURE_CHARACTER_BELL
            BOOL    fActiveMenu[5] = {FALSE, TRUE, TRUE, TRUE, TRUE};
            UINT8   index;
            BOOL    isCharacbell = FALSE;
#endif // FEATURE_CHARACTER_BELL

            SKY_TOKEN paMenu[5] =
            {
                TKN_BELLVIB_MYBELL_DELETE,
                TKN_BELLVIB_MYBELL_SET_ALET,
                TKN_BELLVIB_MYBELL_RENAME,
                TKN_PICTURE_DETAIL,
                TKN_BELLVIB_MYBELL_DELETE_ALL
            };

            SKY_StopMusic();
            EQS_StopTimer(EQS_TIMER_1);

#ifdef FEATURE_CHARACTER_BELL
            nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);

            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));

            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
            if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                strcpy(WithoutIcon, WithIcon+2);
            }

            for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
                if (!(strcmp(SKY_GET_CB_MTITLE(index), WithoutIcon))){
                    // 여기가 같은게 있는 경우이다.
                    if (SKY_GET_CB_MTYPE(index) == SKY_MY_BELL_ID) { 
                        isCharacbell = TRUE;
                        break;
                    }
                }
            }
                    
            if (isCharacbell) 
            {
                hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, 5, 0, fActiveMenu);
                EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
            }
            else 
            {
                fActiveMenu[0] = TRUE;
                hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, 5, 0, fActiveMenu);
                EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
            }
#else
            EQC_PopUpMenu((EQS_TOKEN_EX *)paMenu, 5, 0);
#endif // FEATURE_CHARACTER_BELL
        }
        break;

    case SE_IO_POPUP_SELECT:
        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);

        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
        if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL

        switch(pEvent->dwParam)
        {   
#ifdef FEATURE_CHARACTER_BELL
            case TKN_BELLVIB_MYBELL_DELETE: // 벨 삭제
#else
            case 0: // 벨 삭제
#endif // FEATURE_CHARACTER_BELL
                //nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
                sprintf(pszMessage, "\"%s\" ", WithoutIcon);

                STRNCAT(pszMessage, (BYTE*)SKY_GET_TOKEN(TKN_MYBELL_CONFIRM_DELETE),
                        MESSAGE_BOX_MAX_MESG_LEN);

                SKY_StopMusic();
                EQS_StopTimer(EQS_TIMER_1);

                MessageReason = SMM_DELETE_CONFIRM;
                EQC_MessageBox(pszMessage, 
                                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_BELLVIB_MYBELL_SET_ALET: // 착신벨 설정
#else
            case 1: // 착신벨설정
#endif // FEATURE_CHARACTER_BELL
                //nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
                SKY_SET_BELL_FILE_NAME(WithoutIcon);
                SKY_SET_BELL_ID(SKY_MY_BELL_ID);

                MessageReason = SMM_NONE;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_SET_ALERT), 
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_BELLVIB_MYBELL_RENAME: // 이름변경
#else
            case 2: // 이름변경
#endif // FEATURE_CHARACTER_BELL
                SKY_StopMusic();
                EQS_StopTimer(EQS_TIMER_1);

                //nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
                STRCPY(szChangedName, WithoutIcon);

                EQS_START_CHILD(sSKA_MyBellRename);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_PICTURE_DETAIL: // 파일정보
#else
            case 3: // 파일정보
#endif // FEATURE_CHARACTER_BELL
                EQS_START_CHILD(SKA_MainMyFileInfo);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_BELLVIB_MYBELL_DELETE_ALL: // 벨 전부삭제
#else
            case 4: // 벨 전부삭제
#endif // FEATURE_CHARACTER_BELL
                MessageReason = SMM_DELETE_CONFIRM_ALL;
                EQC_MessageBox(SKY_GET_TOKEN(TKN_MYBELL_CONFIRM_DELETE_ALL),MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            
        }
        break;

    case SE_IO_MESGBOX_OK:
    case SE_IO_MESGBOX_CLEAR:
    case SE_IO_MESGBOX_END:
        switch(MessageReason){
        case SMM_NOBELL:
            SKY_StopMusic();
            EQS_End();
            break;

        case SMM_RENAME:
            //SKY_RenameDataFile(SDF_MELODY, EQC_GetScrollData(s_hListHandle, nBellIndex), szChangedName);
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
            if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                strcpy(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL

            /* rosa 2001-10-04 --> @why, what */
            //FileRenameForMMFile(SDF_MELODY, WithoutIcon, szChangedName);
            if(FileRenameForMMFile(SDF_MELODY, WithoutIcon, szChangedName)
                == FALSE)
            {
                MessageReason = SMM_NONE;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAME_NOSPACE), 
                                1, MBT_NONE, MBT_FAIL);
                break;
            }
            /* <-- */
#ifdef FEATURE_CHARACTER_BELL 
            // 나만의 벨이 캐릭벨에 쓰인 경우 바뀐 이름이 캐릭벨 정보에 반영되어야 한다.
            {
                char    sourceName[MAX_TITLE_LENGTH];
                UINT8   index;
                                
                strcpy(sourceName, WithoutIcon);
                for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
                    if (SKY_GET_CB_MTYPE(index) == SKY_MY_BELL_ID) {
                        if (!(strcmp(SKY_GET_CB_MTITLE(index), sourceName)))
                            SKY_SET_CB_MTITLE(index, szChangedName);
                    }
                }

            }
#endif // FEATURE_CHARACTER_BELL
            // 착신벨
            if(SKY_IS_MY_BELL(SKY_GET_BELL_ID()))
            {
                if(!STRCMP(SKY_GET_BELL_FILE_NAME(), WithoutIcon))
                    SKY_SET_BELL_FILE_NAME(szChangedName);
            }
            //Two Number Bell
            if(SKY_IS_MY_BELL(SKY_GET_NUMBER_PLUS_BELL_ID()))
            {
                if(!STRCMP(SKY_GET_NUMBER_PLUS_BELL_FILENAME(), WithoutIcon))
                    SKY_SET_NUMBER_PLUS_BELL_FILENAME(szChangedName);
            }
            // 알람벨
            if(SKY_IS_MY_BELL(SKY_GET_ALARM_BELL_ID()))
            {
                if(!STRCMP(SKY_GET_ALARM_BELL_FILE_NAME(), WithoutIcon))
                    SKY_SET_ALARM_BELL_TYPE_FILE_NAME(szChangedName);
            }

            // 스케쥴러 알람벨
            if(SKY_IS_MY_BELL(SKY_GET_SCHL_ALERT_BELL_ID()))
            {
                if(!STRCMP(SKY_GET_SCHL_ALERT_BELL_FILE_NAME(), WithoutIcon))
                    SKY_SET_SCHL_ALERT_BELL_TYPE_FILE_NAME(szChangedName);
            }
            // 그룹벨
            for(index = 0; index < g_nGroup; index ++)
            {
                GetGroupInfo(&GroupInfo, index);

                if(SKY_IS_MY_BELL(GroupInfo.Bell.wBellID))
                {
                    if(!STRCMP(GroupInfo.Bell.szFileName, WithoutIcon))
                    {
                        STRCPY(GroupInfo.Bell.szFileName, szChangedName);
                        SetGroupInfo(&GroupInfo, index);
                    }
                }
            }   
#ifdef FEATURE_CHARACTER_BELL
            if (strcmp(WithIcon, WithoutIcon)) { // 원래 잠금이었던거면..
                memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                strncpy(WithIcon, SF_CHARACBELL_1, 2);
                strcat(WithIcon, szChangedName);
                EQC_SetScrollData(s_hListHandle, nBellIndex, WithIcon);
            }
            else
                EQC_SetScrollData(s_hListHandle, nBellIndex, szChangedName);
#else
            EQC_SetScrollData(s_hListHandle, nBellIndex, szChangedName);
#endif // FEATURE_CHARACTER_BELL
            MessageReason = SMM_RENAME_OK;
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAMED), 
                            1, MBT_NONE, MBT_SUCCESS);
            break;
        
        case SMM_DELETED_ALL:
            s_nMyBell = SKY_GetFileExtList(SKY_MELODY_DIR, ppMyBellList);

            if(s_nMyBell == 0)
            {
                MessageReason = SMM_NOBELL;
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_NOMYBELL), 
                             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                break;
            }
#ifdef FEATURE_CHARACTER_BELL
            // 남아있는 벨이 있어도 우선은 다 삭제후 상위로 나가는 경우
            SKY_StopMusic();
            EQS_End();

            // 남아있는게 있으면 delete처럼 되는 경우.
            // 밑의 부분을 가져다가 넣으면 된당.
#endif // FEATURE_CHARACTER_BELL
            break;

        case SMM_RENAME_OK:
        case SMM_DELETED:
            // Load user file list
            //s_nMyBell = SKY_GetFileExtList(SKY_MELODY_DIR, ppMyBellList);

            s_nMyBell = EQC_GetScrollDataLine(s_hListHandle);

            if(s_nMyBell == 0)
            {
                MessageReason = SMM_NOBELL;
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_NOMYBELL), 
                             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                break;
            }

            EQS_SetIOCActive(s_hListHandle);

            nBellIndex = EQC_GetScrollLogicalIndex(s_hListHandle);
            
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
            if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                strcpy(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL
            if(SKY_GetDataFileSize(SDF_MELODY, WithoutIcon) < 1024)
                sprintf((char*)pszTmpBuffer, 
                        "%s %5ld B", 
                        (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                        SKY_GetDataFileSize(SDF_MELODY, WithoutIcon));
            else
                sprintf(pszTmpBuffer, 
                        "%s %5ldKB", 
                        (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                        GET_KB_SIZE(SKY_GetDataFileSize(SDF_MELODY, WithoutIcon)));

            EQC_SetScrollStatusBarText(s_hListHandle, pszTmpBuffer);

            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        case SMM_NONE:
            EQS_SetIOCActive(s_hListHandle);

            nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;
        }
        break;

    case SE_IO_MESGBOX_NO:
        EQS_SetIOCActive(s_hListHandle);

        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
        {
            SKY_StopMusic();
            EQS_End();
        }
        break;


    case SE_IO_SCROLL_LIST_SELECT:
    case SE_IO_POPUP_CANCEL:
        EQS_SetIOCActive(s_hListHandle);
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;

    case SE_IO_SCROLL_LIST_CHG:
        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, pEvent->dwParam));
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, pEvent->dwParam));
        if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL
        if(SKY_GetDataFileSize(SDF_MELODY, WithoutIcon) < 1024)
            sprintf((char*)pszTmpBuffer, 
                    "%s %5ld B", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    SKY_GetDataFileSize(SDF_MELODY, WithoutIcon));
        else
            sprintf(pszTmpBuffer, 
                    "%s %5ldKB", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    GET_KB_SIZE(SKY_GetDataFileSize(SDF_MELODY, WithoutIcon)));

        EQC_SetScrollStatusBarText(s_hListHandle, pszTmpBuffer);

        SKY_StopMusic();
        nBellIndex = pEvent->dwParam;
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;

    case SE_TM_TIMER1:
        if(EQS_HaveFocus(s_hListHandle) && EQS_GetModal() == INVALID_CONTROL_HANDLE) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, EQC_GetScrollLogicalIndex(s_hListHandle)));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, EQC_GetScrollLogicalIndex(s_hListHandle)));
            if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                strcpy(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL
            SKY_PlayMusicFileEx(WithoutIcon, SKY_GET_BELL_VOLUME());
        }
        break;

    case SE_IO_MESGBOX_YES:
        if(MessageReason != SMM_DELETE_CONFIRM && MessageReason != SMM_DELETE_CONFIRM_ALL )
            break;
        if(MessageReason == SMM_DELETE_CONFIRM)
        {
            nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);

            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, nBellIndex));
            if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                strcpy(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL
            //nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
            // 착신벨
            if(SKY_IS_MY_BELL(SKY_GET_BELL_ID()))
            {
                if(!STRCMP(SKY_GET_BELL_FILE_NAME(), WithoutIcon))
                    SKY_SET_BELL_ID(INIT_BELL_ID);
            }
            //Two Number Bell
            if(SKY_IS_MY_BELL(SKY_GET_NUMBER_PLUS_BELL_ID()))
            {
                if(!STRCMP(SKY_GET_NUMBER_PLUS_BELL_FILENAME(), WithoutIcon))
                    SKY_SET_NUMBER_PLUS_BELL_ID(INIT_BELL_ID);        
            }
            // 알람벨
            if(SKY_IS_MY_BELL(SKY_GET_ALARM_BELL_ID()))
            {
                if(!STRCMP(SKY_GET_ALARM_BELL_FILE_NAME(), WithoutIcon))
                    SKY_SET_ALARM_BELL_ID(INIT_ALARM_BELL_ID);
            }
            // 스케쥴러 알람벨
            if(SKY_IS_MY_BELL(SKY_GET_SCHL_ALERT_BELL_ID()))
            {
                if(!STRCMP(SKY_GET_SCHL_ALERT_BELL_FILE_NAME(), WithoutIcon))
                    SKY_SET_SCHL_ALERT_BELL_ID(INIT_SCHL_ALERT_BELL_ID);
            }
            // 그룹벨
            for(index = 0; index < g_nGroup; index ++)
            {
                GetGroupInfo(&GroupInfo, index);

                if(SKY_IS_MY_BELL(GroupInfo.Bell.wBellID))
                {
                    if(!STRCMP(GroupInfo.Bell.szFileName, WithoutIcon))
                    {
                        GroupInfo.Bell.wBellID = 0;   // NOT Assign.
                        SetGroupInfo(&GroupInfo, index);
                    }
                }
            }
            SKY_DeleteDataFile(SDF_MELODY, WithoutIcon);
            // 삭제되었습니다..

            EQC_DeleteScrollData(s_hListHandle, nBellIndex);

            MessageReason = SMM_DELETED;
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_DELETED), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(MessageReason == SMM_DELETE_CONFIRM_ALL)
        {
            // 착신벨
            if(SKY_IS_MY_BELL(SKY_GET_BELL_ID()))
            {
                SKY_SET_BELL_ID(INIT_BELL_ID);
            }
            //Two Number Bell
            if(SKY_IS_MY_BELL(SKY_GET_NUMBER_PLUS_BELL_ID()))
            {
                SKY_SET_NUMBER_PLUS_BELL_ID(INIT_BELL_ID);        
            }
            // 알람벨
            if(SKY_IS_MY_BELL(SKY_GET_ALARM_BELL_ID()))
            {
                SKY_SET_ALARM_BELL_ID(INIT_ALARM_BELL_ID);
            }

            // 스케쥴러 알람벨
            if(SKY_IS_MY_BELL(SKY_GET_SCHL_ALERT_BELL_ID()))
            {
                SKY_SET_SCHL_ALERT_BELL_ID(INIT_SCHL_ALERT_BELL_ID);
            }
            // 그룹벨
            for(index = 0; index < g_nGroup; index ++)
            {
                GetGroupInfo(&GroupInfo, index);

                if(SKY_IS_MY_BELL(GroupInfo.Bell.wBellID))
                {
                    GroupInfo.Bell.wBellID = 0;   // NOT Assign.
                    SetGroupInfo(&GroupInfo, index);
                }
            }
        
            // 삭제되었습니다..
            // 여기서도 데이터 파일을 다 지우는 것이 아니라 
            // 캐릭벨로 쓰인 것은 지우면 안된다.
#ifdef FEATURE_CHARACTER_BELL
            {
                UINT8   i, j, count;
                BOOL    isCharacbell = FALSE, isExist = FALSE;
                
                count = EQC_GetScrollDataLine(s_hListHandle);

                for(i = 0; i < count; i++) {
                    memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                    strcpy(WithoutIcon, EQC_GetScrollData(s_hListHandle, i));
                    memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                    strcpy(WithIcon, EQC_GetScrollData(s_hListHandle, i));
                    if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
                        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                        strcpy(WithoutIcon, WithIcon+2);
                    }

                    for (j = 0; j < MAX_NUM_OF_CHARACBELL; j++) {
                        if(!(strcmp(WithoutIcon, SKY_GET_CB_MTITLE(j)))) {
                            // 같은게 있는 경우
                            if (SKY_GET_CB_MTYPE(j) == SKY_MY_BELL_ID) { 
                                isCharacbell = TRUE;
                                break;
                            } // 다른 곳에 같은 이름의 파일이 존재할 경우
                        }

                    }
                    if (!isCharacbell) {
                        SKY_DeleteDataFile(SDF_MELODY, WithoutIcon);
                        EQC_DeleteScrollData(s_hListHandle, i);
                        count--;
                        i--;
                    }
                    else {
                        isCharacbell = FALSE;
                        isExist = TRUE;
                    }
                }
                if (isExist) {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_NOTDELETEBELL), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                }
                else {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_DELETED), 
                                   MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                }
            }
#else
            SKY_DeleteDataFiles(SDF_MELODY);
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_DELETED), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
#endif // FEATURE_CHARACTER_BELL
            MessageReason = SMM_DELETED_ALL;
            
        }
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


LOCAL BOOL EQS_API sAddMyBellControls(void)
{
    UINT8   index;
#ifdef FEATURE_CHARACTER_BELL
    UINT8   nIndex;
    char    temp[MAX_TITLE_LENGTH+2];
#endif // FEATURE_CHARACTER_BELL
    BYTE pszTmpBuffer[20];

    // Load user file list
    s_nMyBellAlloc = SKY_GetFileNumberInDataFile(SDF_MELODY);

    if(!s_nMyBellAlloc)
        return FALSE;

    for(index=0; index < s_nMyBellAlloc; index ++) 
        ppMyBellList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

    s_nMyBell = SKY_GetFileExtList(SKY_MELODY_DIR, ppMyBellList);

#ifdef FEATURE_CHARACTER_BELL
    for (index = 0; index < s_nMyBell; index++) {
        for (nIndex = 0; nIndex < MAX_NUM_OF_CHARACBELL; nIndex++) {
            if (SKY_GET_CB_MTYPE(nIndex) == SKY_MY_BELL_ID) {
                if (!strcmp(SKY_GET_CB_MTITLE(nIndex), ppMyBellList[index])) { 
                    // 캐릭벨에 쓰인 나만의 벨인 경우
                    strcpy(temp, ppMyBellList[index]);
                    strncpy(ppMyBellList[index], SF_CHARACBELL_1, 2);
                    strcpy(ppMyBellList[index]+2, temp);
                    break;
                }
            }
        }
    } // for
#endif // FEATURE_CHARACTER_BELL

    s_hListHandle = EQC_ScrollList(EQS_NA, EQS_NA, 18, 
                    5, ppMyBellList, 
                    s_nMyBell, 
                    s_nMyBell,
                    0);

        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        strcpy(WithoutIcon, ppMyBellList[0]);
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        strcpy(WithIcon, ppMyBellList[0]);
        if (!strncmp(WithIcon, SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            strcpy(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL

    if(SKY_GetDataFileSize(SDF_MELODY, WithoutIcon) < 1024)
        sprintf((char*)pszTmpBuffer, 
                "%s %5ld B", 
                (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                SKY_GetDataFileSize(SDF_MELODY, WithoutIcon));
    else
        sprintf(pszTmpBuffer, 
                "%s %5ldKB", 
                (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                GET_KB_SIZE(SKY_GetDataFileSize(SDF_MELODY, WithoutIcon)));

    EQC_SET_X(s_hListHandle, 0);
    EQS_SetIOCActive(s_hListHandle);

    EQC_SET_STYLE(s_hListHandle, SMB_SCROLL_LIST_STYLE);

    EQC_SetScrollStatusBarText(s_hListHandle, pszTmpBuffer);

    return TRUE;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_Xxx()
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
LOCAL void SKY_API sSKA_MyBellRename(SKY_EVENT_T *pEvent)
{
    static BOOL fChanged;
    static HCONTROL hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_CONFIRM_NONE, 
                      SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_RENAME_TITLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_MYBELL_RENAME);
        hControl = EQC_TextEdit(EQS_NA, EQS_NA, SKY_FILE_NAME_MAX_LENGTH-1, 1, 
                     TEM_KOREAN, SKY_FILE_NAME_MAX_LENGTH-1,
                     szChangedName, 0);

        EQS_AutoFormatting(2, EHA_CENTER, EVA_CENTER);
        EQS_SetIOCActive(hControl);
        fChanged = FALSE;
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_TEDIT_CANCEL:
        EQS_END_EVENT( SE_APP_RESUME, (WORD)fChanged, 0);
        break;
        
    case SE_IO_TEDIT_DONE:
        if(pEvent->dwParam == 0)
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_FILENAME_ERR), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;
        }

        EQC_GetTextEditData(hControl, szChangedName);

        if(SKY_IsDataFile(SDF_MELODY, szChangedName))
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_FILENAME_EXIST), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;
        }

        if(FV_OK == SKY_IsValidFileName(szChangedName))
        {
            fChanged = TRUE;
            EQS_END_EVENT( SE_APP_RESUME, (WORD)fChanged, 0);
        }
        else
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_INVALID_FILENAME), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);        
        }
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hControl);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}