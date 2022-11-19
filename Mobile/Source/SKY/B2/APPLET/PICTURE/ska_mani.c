/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_mani.c
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
** 2001-02-26 woodstok     Created.
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

#define SMB_SCROLL_LIST_STYLE   CS_SCROLL_LIST_DEFAULT| \
                                CS_SCROLL_LIST_SCROLL_BAR | \
                                CS_SCROLL_LIST_ENUMBERATED | \
                                CS_SCROLL_LIST_MAXIMIZE | \
                                CS_SCROLL_LIST_NO_CML | \
                                CS_SCROLL_LIST_STATUS_BAR


#define IMAGE_VIEWER_WIDTH              120
#define IMAGE_VIEWER_HEIGHT             120
#define COLOR_IDLE_IMAGE_BPP                8
#define     RENAME_POPUP                3

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tag_SKA_MYANI_MESSAGEBOX_E {
    SMM_NOANI,
    SMM_DELETE_CONFIRM,
    SMM_DELETE_CONFIRM_ALL,
    SMM_DELETED,
    SMM_DELETED_ALL,
    SMM_RENAME,
    SMM_NONE
};

typedef enum tag_SKA_MYANI_MESSAGEBOX_E SKA_MYANI_MESSAGEBOX_E;

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

LOCAL BYTE *ppMyANIList[SKY_MAX_NUM_FILES];
LOCAL UINT8 s_nMyANI, s_nMyANIAlloc;

LOCAL HCONTROL s_hListHandle;

LOCAL SKA_MYANI_MESSAGEBOX_E   MessageReason;

//LOCAL BYTE *l_CurrentFile;
LOCAL INT16 l_CurrentIndex;
//LOCAL BYTE szChangedName[SKY_FILE_NAME_MAX_LENGTH];
BYTE g_szChangedName[SKY_FILE_NAME_MAX_LENGTH];

LOCAL BYTE      WithoutIcon[SKY_FILE_NAME_MAX_LENGTH+2];
#ifdef FEATURE_CHARACTER_BELL
LOCAL BYTE      WithIcon[SKY_FILE_NAME_MAX_LENGTH+2];
#endif // FEATURE_CHARACTER_BELL
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL BOOL EQS_API sAddMyANIControls(void);
LOCAL void  sFormatMyANIControls(void);
LOCAL void SKY_API sSKA_ViewMyAni(SKY_EVENT_T *pEvent);
//LOCAL void SKY_API sSKA_MyAniRename(SKY_EVENT_T *pEvent);

// in ska_dmgr.c
extern BOOL FileRenameForMMFile(SKY_DATA_FILE_E SkyDataFile, const BYTE* pszSrcName, const BYTE* pszDestName);

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
void SKY_API SKA_MainMyAnimation(SKY_EVENT_T *pEvent)
{
    UINT8 index;
    BYTE pszTmpBuffer[20];
    BYTE pszMessage[MESSAGE_BOX_MAX_MESG_LEN];
    COORD   x, y;
    PICTURE_TYPE_T tmpPicture;
    static BOOL fSet;
    static UINT8 nANIIndex;
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
                      CL_MENU_BEFOREVIEW_NONE, 
                      SKY_GET_TOKEN(TKN_PICTURE_MYANI), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        if(!sAddMyANIControls())
        {
            MessageReason = SMM_NOANI;
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_NOFILE), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        else
        {
            EQS_FitCanvasSize();
        }
        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(index=0; index < s_nMyANIAlloc; index ++)
            SKY_Free(ppMyANIList[index]);

        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(pEvent->wParam)
        {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
            if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL
            if(STRCMP(WithoutIcon, g_szChangedName))
            {
                MessageReason = SMM_RENAME;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAMING), 
                                0, MBT_NONE, MBT_HELP);
            }
            else
                EQS_SetIOCActive(s_hListHandle);
        }
        else
        {
            EQC_SetScrollLogicalIndex(s_hListHandle, l_CurrentIndex);
            EQS_SetIOCActive(s_hListHandle);
        }

        nANIIndex = EQC_GetScrollLogicalIndex(s_hListHandle);
        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
        if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL

        if(SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon) < 1024)
            sprintf((char*)pszTmpBuffer, 
                    "%s %5ld B", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon));
        else
            sprintf((char*)pszTmpBuffer, 
                    "%s %5ldKB", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    GET_KB_SIZE(SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon)));

        EQC_SetScrollStatusBarText(s_hListHandle, pszTmpBuffer);
        break;

    case SE_SOFTKEY_MENU: 
        {
#ifdef FEATURE_CHARACTER_BELL
            BOOL    fActiveMenu[5] = {FALSE, TRUE, TRUE, TRUE, TRUE};
            UINT8   index;
            BOOL    isCharacbell = FALSE;
#endif // FEATURE_CHARACTER_BELL
            SKY_TOKEN paMenu[5] =
            {
                TKN_PICTURE_DELETE,
                TKN_PICTURE_SET_MYANI,
                TKN_BELLVIB_MYBELL_RENAME,
                TKN_PICTURE_DETAIL,
                TKN_PICTURE_DELETE_ALL
            };

#ifdef FEATURE_CHARACTER_BELL
            nANIIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
            
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));

            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
            if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithoutIcon, WithIcon+2);
            } 

            for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
                if (!(STRCMP((BYTE*)SKY_GET_CB_ITITLE(index), WithoutIcon))){
                    // 여기가 같은게 있는 경우이다.
                    if (SKY_GET_CB_ITYPE(index) == sMY_PICTURE) { 
                        isCharacbell = TRUE;
                        break;
                    }
                }
            }
                    
            if (isCharacbell) 
            {
                hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paMenu, 5, 0, fActiveMenu);
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
        
        fSet = FALSE;
        break;

    case SE_IO_POPUP_SELECT:
        nANIIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);

        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
        if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL

        if(fSet)    // 그림지정
        {
            PICTURE_TYPE_T tmpPicture;

            fSet = FALSE;
            //nANIIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
            MessageReason = SMM_NONE;

            tmpPicture.wPictureID = sMY_PICTURE;
            STRCPY(tmpPicture.szFileName, WithoutIcon);

            switch(pEvent->dwParam)
            {
            case 0: // 대기화면
                SKY_SET_IDLE_PICTURE(tmpPicture);
                SKY_SET_IDLE_SCREEN(IS_PICTURE);

                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MSG_MYANI_IDLE), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                break;
            case 1: // 전화왔을때
                SKY_SET_INCOMING_PICTURE(tmpPicture);

                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MSG_CALL), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                break;
            case 2: // 시작시
                SKY_SET_POWER_ON_PICTURE(tmpPicture);

                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MSG_STARTUP), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                break;
            case 3: // 종료시
                SKY_SET_POWER_OFF_PICTURE(tmpPicture);

                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MSG_POWEROFF), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                break;
            case 4: // N.Top시작시
                SKY_SET_WAP_PICTURE(tmpPicture);

                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MSG_WAP), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                break;
            }

        }
        else
        {
            switch(pEvent->dwParam)
            {
#ifdef FEATURE_CHARACTER_BELL
            case TKN_PICTURE_DELETE: // 그림 삭제
#else
            case 0:     // 그림 삭제
#endif // FEATURE_CHARACTER_BELL
                //nANIIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
                sprintf((char*)pszMessage, "\"%s\" ", WithoutIcon);

                STRNCAT(pszMessage, (BYTE*)SKY_GET_TOKEN(TKN_DELETE_IMAGE),
                        MESSAGE_BOX_MAX_MESG_LEN);

                MessageReason = SMM_DELETE_CONFIRM;
                EQC_MessageBox(pszMessage, 
                                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_PICTURE_SET_MYANI: // 그림지정하기
#else
            case 1: // 그림지정하기
#endif // FEATURE_CHARACTER_BELL
                {
                    SKY_TOKEN paMenu[5] =
                    {
                        TKN_PICTURE_MYANI_IDLE,
                        TKN_PICTURE_CALL,
                        TKN_PICTURE_STARTUP,
				        TKN_PICTURE_POWEROFF,
                        TKN_PICTURE_WAP
                    };

                    EQC_PopUpMenu((EQS_TOKEN_EX *)paMenu, 5, 0);
                }
        
                fSet = TRUE;
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_BELLVIB_MYBELL_RENAME: // 이름변경
#else
            case 2: // 이름변경
#endif // FEATURE_CHARACTER_BELL
                //nANIIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
                l_CurrentIndex = nANIIndex;
                STRCPY(g_szChangedName, WithoutIcon);

                EQS_START_CHILD_EVENT(SKA_MyAniRename, SE_APP_START, POPUP_MENU_MY_PICTURE, EQS_NA);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_PICTURE_DETAIL: // 파일정보
#else
            case 3: // 파일정보
#endif // FEATURE_CHARACTER_BELL
                //nANIIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
                l_CurrentIndex = nANIIndex;
                EQS_START_CHILD(SKA_MainMyFileInfo);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_PICTURE_DELETE_ALL: // 그림 전부삭제
#else
            case 4: //그림 전부삭제
#endif // FEATURE_CHARACTER_BELL
                MessageReason = SMM_DELETE_CONFIRM_ALL;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DELETE_IMAGE_ALL),MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            
            }
        }
        break;

    case SE_IO_MESGBOX_CLEAR:
    case SE_IO_MESGBOX_OK:
    case SE_IO_MESGBOX_END:
        switch(MessageReason){
        case SMM_NOANI:
            EQS_End();
            break;
        
        case SMM_RENAME:
            //SKY_RenameDataFile(SDF_IMAGE, EQC_GetScrollData(s_hListHandle, nANIIndex), g_szChangedName);
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
            if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL

            /* rosa 2001-10-04 --> @why, what */
            //FileRenameForMMFile(SDF_IMAGE, WithoutIcon, g_szChangedName);
            if(FileRenameForMMFile(SDF_IMAGE, WithoutIcon, g_szChangedName)
                == FALSE)
            {
                MessageReason = SMM_NONE;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAME_NOSPACE), 
                                1, MBT_NONE, MBT_FAIL);
                break;
            }
            /* <-- */
#ifdef FEATURE_CHARACTER_BELL  
            // 나만의 그림이 캐릭벨에 쓰인 경우 바뀐 이름이 캐릭벨 정보에 반영되어야 한다.
            {
                char    sourceName[MAX_TITLE_LENGTH];
                UINT8   index;
                                
                strcpy(sourceName, (char*)WithoutIcon);
                for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
                    if (SKY_GET_CB_ITYPE(index) == sMY_PICTURE) {
                        if (!(strcmp(SKY_GET_CB_ITITLE(index), sourceName)))
                            SKY_SET_CB_ITITLE(index, g_szChangedName);
                    }
                }

            }
#endif // FEATURE_CHARACTER_BELL

            tmpPicture.wPictureID = sMY_PICTURE;
            STRCPY(tmpPicture.szFileName, g_szChangedName);
            
            // 대기중
            if(IS_MY_PICTURE(SKY_GET_IDLE_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_IDLE_PICTURE().szFileName, WithoutIcon))
                    SKY_SET_IDLE_PICTURE(tmpPicture);
            }
            // 시작시
            if(IS_MY_PICTURE(SKY_GET_POWER_ON_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_POWER_ON_PICTURE().szFileName, WithoutIcon))
                    SKY_SET_POWER_ON_PICTURE(tmpPicture);
            }
            // 종료시
            if(IS_MY_PICTURE(SKY_GET_POWER_OFF_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_POWER_OFF_PICTURE().szFileName, WithoutIcon))
                    SKY_SET_POWER_OFF_PICTURE(tmpPicture);
            }
            // 전화왔을때
            if(IS_MY_PICTURE(SKY_GET_INCOMING_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_INCOMING_PICTURE().szFileName, WithoutIcon))
                    SKY_SET_INCOMING_PICTURE(tmpPicture);
            }

            // N.TOP 시작시
            if(IS_MY_PICTURE(SKY_GET_WAP_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_WAP_PICTURE().szFileName, WithoutIcon))
                    SKY_SET_WAP_PICTURE(tmpPicture);
            }
#ifdef FEATURE_CHARACTER_BELL
            if (STRCMP(WithIcon, WithoutIcon)) { // 원래 잠금이었던거면..
                memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRNCPY(WithIcon, (BYTE*)SF_CHARACBELL_1, 2);
                STRCAT(WithIcon, g_szChangedName);
                EQC_SetScrollData(s_hListHandle, nANIIndex, WithIcon);
            }
            else
                EQC_SetScrollData(s_hListHandle, nANIIndex, g_szChangedName);
#else
            EQC_SetScrollData(s_hListHandle, nANIIndex, g_szChangedName);
#endif // FEATURE_CHARACTER_BELL
            MessageReason = SMM_DELETED;
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAMED), 
                            1, MBT_NONE, MBT_SUCCESS);
            break;
        
        case SMM_DELETED_ALL:
            
            s_nMyANI = SKY_GetFileExtList(SKY_IMAGE_DIR, ppMyANIList);
            
            if(s_nMyANI == 0)
            {
                MessageReason = SMM_NOANI;
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_NOFILE), 
                             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                break;
            }
#ifdef FEATURE_CHARACTER_BELL
            // 남아있는 그림이 있어도 우선은 다 삭제후 상위로 나가는 경우
            EQS_End();

            // 남아있는게 있으면 delete처럼 되는 경우.
            // 밑의 부분을 가져다가 넣으면 된당.
#endif // FEATURE_CHARACTER_BELL
            break;
            
        case SMM_DELETED:
            // Load user file list

            //s_nMyANI = SKY_GetFileExtList(SKY_IMAGE_DIR, ppMyANIList);

            s_nMyANI = EQC_GetScrollDataLine(s_hListHandle);

            if(s_nMyANI == 0)
            {
                MessageReason = SMM_NOANI;
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_IRDA_MSG_ANI_NOFILE), 
                             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                break;
            }

            nANIIndex = EQC_GetScrollLogicalIndex(s_hListHandle);

            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
            if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL
            if(SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon) < 1024)
                sprintf((char*)pszTmpBuffer, 
                        "%s %5ld B", 
                        (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                        SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon));
            else
                sprintf((char*)pszTmpBuffer, 
                        "%s %5ldKB", 
                        (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                        GET_KB_SIZE(SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon)));

            EQC_SetScrollStatusBarText(s_hListHandle, pszTmpBuffer);
            EQS_SetIOCActive(s_hListHandle);
            break;

        case SMM_NONE:
            EQS_SetIOCActive(s_hListHandle);
            break;
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
        {
            EQS_End();
        }
        break;

    case SE_IO_POPUP_CANCEL:
        EQS_SetIOCActive(s_hListHandle);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        nANIIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);

        l_CurrentIndex = nANIIndex;
//        l_CurrentFile = EQC_GetScrollData(s_hListHandle, nANIIndex);
        EQS_START_CHILD(sSKA_ViewMyAni);
        break;

    case SE_IO_SCROLL_LIST_CHG:
        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, pEvent->dwParam));
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, pEvent->dwParam));
        if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL
        if(SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon) < 1024)
            sprintf((char*)pszTmpBuffer, 
                    "%s %5ld B", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon));
        else
            sprintf((char*)pszTmpBuffer, 
                    "%s %5ldKB", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    GET_KB_SIZE(SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon)));

        EQC_SetScrollStatusBarText(s_hListHandle, pszTmpBuffer);
        break;

    case SE_IO_MESGBOX_YES:
        if(MessageReason != SMM_DELETE_CONFIRM && MessageReason !=SMM_DELETE_CONFIRM_ALL)
            break;
        if(MessageReason == SMM_DELETE_CONFIRM)
        {
            nANIIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);

            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
#ifdef FEATURE_CHARACTER_BELL
            memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, nANIIndex));
            if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithoutIcon, WithIcon+2);
            }
#endif // FEATURE_CHARACTER_BELL
            // 대기중
            if(IS_MY_PICTURE(SKY_GET_IDLE_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_IDLE_PICTURE().szFileName, WithoutIcon))
                {
                    tmpPicture.wPictureID = INIT_IDLE_PICTURE;
                    STRCPY(tmpPicture.szFileName, (BYTE*)"");
                    SKY_SET_IDLE_PICTURE(tmpPicture);
                
                    SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                    SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);
                    SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);
                }
            }
            // 시작시
            if(IS_MY_PICTURE(SKY_GET_POWER_ON_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_POWER_ON_PICTURE().szFileName, WithoutIcon))
                {
                    tmpPicture.wPictureID = INIT_POWER_ON_PICTURE;
                    STRCPY(tmpPicture.szFileName, (BYTE*)"");
                    SKY_SET_POWER_ON_PICTURE(tmpPicture);
                }
            }
            // 종료시
            if(IS_MY_PICTURE(SKY_GET_POWER_OFF_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_POWER_OFF_PICTURE().szFileName, WithoutIcon))
                {
                    tmpPicture.wPictureID = INIT_POWER_OFF_PICTURE;
                    STRCPY(tmpPicture.szFileName, (BYTE*)"");
                    SKY_SET_POWER_OFF_PICTURE(tmpPicture);
                }
            }
            // 전화왔을때
            if(IS_MY_PICTURE(SKY_GET_INCOMING_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_INCOMING_PICTURE().szFileName, WithoutIcon))
                {
                    tmpPicture.wPictureID = INIT_INCOMING_PICTURE;
                    STRCPY(tmpPicture.szFileName, (BYTE*)"");
                    SKY_SET_INCOMING_PICTURE(tmpPicture);
                }
            }

            // N.TOP 시작시
            if(IS_MY_PICTURE(SKY_GET_WAP_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_WAP_PICTURE().szFileName, WithoutIcon))
                {
                    tmpPicture.wPictureID = INIT_WAP_PICTURE;
                    STRCPY(tmpPicture.szFileName, (BYTE*)"");
                    SKY_SET_WAP_PICTURE(tmpPicture);
                }
            }
            SKY_DeleteDataFile(SDF_IMAGE, WithoutIcon);

            // 삭제되었습니다..
            EQC_DeleteScrollData(s_hListHandle, nANIIndex);

            MessageReason = SMM_DELETED;
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DELETE_DONE_IMAGE), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(MessageReason == SMM_DELETE_CONFIRM_ALL)
        {
            // 대기중
            if(IS_MY_PICTURE(SKY_GET_IDLE_PICTURE().wPictureID))
            {
                    tmpPicture.wPictureID = INIT_IDLE_PICTURE;
                    STRCPY(tmpPicture.szFileName, (BYTE*)"");
                    SKY_SET_IDLE_PICTURE(tmpPicture);
                
                    SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                    SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);
                    SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);
            }
            // 시작시
            if(IS_MY_PICTURE(SKY_GET_POWER_ON_PICTURE().wPictureID))
            {
                tmpPicture.wPictureID = INIT_POWER_ON_PICTURE;
                STRCPY(tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_POWER_ON_PICTURE(tmpPicture);
            }
            // 종료시
            if(IS_MY_PICTURE(SKY_GET_POWER_OFF_PICTURE().wPictureID))
            {                
                tmpPicture.wPictureID = INIT_POWER_OFF_PICTURE;
                STRCPY(tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_POWER_OFF_PICTURE(tmpPicture);
            }
            // 전화왔을때
            if(IS_MY_PICTURE(SKY_GET_INCOMING_PICTURE().wPictureID))
            {
                tmpPicture.wPictureID = INIT_INCOMING_PICTURE;
                STRCPY(tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_INCOMING_PICTURE(tmpPicture);
            }

            // N.TOP 시작시
            if(IS_MY_PICTURE(SKY_GET_WAP_PICTURE().wPictureID))
            {
                    tmpPicture.wPictureID = INIT_WAP_PICTURE;
                    STRCPY(tmpPicture.szFileName, (BYTE*)"");
                    SKY_SET_WAP_PICTURE(tmpPicture);
            }
#ifdef FEATURE_CHARACTER_BELL
            {
                UINT8   i, j, count;
                BOOL    isCharacbell = FALSE, isExist = FALSE;

                count = EQC_GetScrollDataLine(s_hListHandle);

                for(i = 0; i < count; i++) {
                    memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                    STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, i));
                    memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                    STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, i));
                    if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                        STRCPY(WithoutIcon, WithIcon+2);
                    }

                    for (j = 0; j < MAX_NUM_OF_CHARACBELL; j++) {
                        if(!(STRCMP(WithoutIcon, (BYTE*)SKY_GET_CB_ITITLE(j)))) {
                            // 같은게 있는 경우
                            if (SKY_GET_CB_ITYPE(j) == sMY_PICTURE) { 
                                isCharacbell = TRUE;
                                break;
                            }
                        }

                    }
                    if (!isCharacbell) {
                        SKY_DeleteDataFile(SDF_IMAGE, EQC_GetScrollData(s_hListHandle, i));
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
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_NOTDELETEIMAGE), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                }
                else {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DELETE_DONE_IMAGE), 
                                   MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                }
            }
#else             
            SKY_DeleteDataFiles(SDF_IMAGE);
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DELETE_DONE_IMAGE), 
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

LOCAL BOOL EQS_API sAddMyANIControls(void)
{
    UINT8   index;
    BYTE pszTmpBuffer[20];
#ifdef FEATURE_CHARACTER_BELL
    UINT8   nIndex;
    BYTE    temp[MAX_TITLE_LENGTH+2];
#endif // FEATURE_CHARACTER_BELL

    // Load user file list
    s_nMyANIAlloc = (UINT8)SKY_GetFileNumberInDataFile(SDF_IMAGE);

    if(!s_nMyANIAlloc)
        return FALSE;

    for(index=0; index < s_nMyANIAlloc; index ++)
        ppMyANIList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

    s_nMyANI = SKY_GetFileExtList(SKY_IMAGE_DIR, ppMyANIList);

#ifdef FEATURE_CHARACTER_BELL
    for (index = 0; index < s_nMyANI; index++) {
        for (nIndex = 0; nIndex < MAX_NUM_OF_CHARACBELL; nIndex++) {
            if (SKY_GET_CB_ITYPE(nIndex) == sMY_PICTURE) {
                if (!STRCMP((BYTE*)SKY_GET_CB_ITITLE(nIndex), ppMyANIList[index])) { 
                    // 캐릭벨에 쓰인 나만의 벨인 경우
                    STRCPY(temp, ppMyANIList[index]);
                    STRNCPY(ppMyANIList[index], (BYTE*)SF_CHARACBELL_1, 2);
                    STRCPY(ppMyANIList[index]+2, temp);
                    break;
                }
            }
        }
    } // for
#endif // FEATURE_CHARACTER_BELL

    s_hListHandle = EQC_ScrollList(EQS_NA, EQS_NA, 18, 
                   5, ppMyANIList, 
                   s_nMyANI, 
                   s_nMyANI,
                   0);

    memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
    STRCPY(WithoutIcon, ppMyANIList[0]);
#ifdef FEATURE_CHARACTER_BELL
    memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
    STRCPY(WithIcon, ppMyANIList[0]);
    if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithoutIcon, WithIcon+2);
    }
#endif // FEATURE_CHARACTER_BELL

    if(SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon) < 1024)
        sprintf((char*)pszTmpBuffer, 
                "%s %5ld B", 
                (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon));
    else
        sprintf((char*)pszTmpBuffer, 
                "%s %5ldKB", 
                (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                GET_KB_SIZE(SKY_GetDataFileSize(SDF_IMAGE, WithoutIcon)));

    EQC_SetScrollStatusBarText(s_hListHandle, pszTmpBuffer);

    EQC_SET_X(s_hListHandle, 0);
    EQS_SetIOCActive(s_hListHandle);

    EQC_SET_STYLE(s_hListHandle, SMB_SCROLL_LIST_STYLE);
    return TRUE;
}

/*============================================================================= 
 Function:    
     sSKA_ImageList

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
LOCAL BITMAP l_BmpBuf = {
    { EQUS_IMAGE_ID, { SKY_SCREEN_BUFFER_WIDTH, SKY_SCREEN_BUFFER_HEIGHT}, 
        SKY_SCREEN_WIDTH, BIT_PER_PIXEL_256C, FALSE, MAX_COLOR, 0 },
    g_abBmpBuf
};

LOCAL void SKY_API sSKA_ViewMyAni(SKY_EVENT_T *pEvent)
{
    HFILE hFile;
    BYTE    *szFile;
    static  HCONTROL    hControl, hName;
    static UINT8   nScrollData;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DX, 
            NULL,
            SKY_GET_TOKEN(TKN_PICTURE_PREVIEW), 
            WA_SHOW_HEADER);

        nScrollData = EQC_GetScrollDataLine(s_hListHandle);

        memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));

        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, l_CurrentIndex));
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, l_CurrentIndex));
        if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL
        szFile = WithoutIcon;
        SKY_OpenDataFile(SDF_IMAGE, szFile, &hFile, SFT_READ);
        SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));
        if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
        {
            SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
            SKY_CloseDataFile(hFile);
            l_BmpBuf.Header.yStart = 0;
            /* rosa 2001-09-26 --> @why, what */
            //hControl = EQC_Image(0, 0, &l_BmpBuf);
            hControl = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            /* <-- */  
        }
        else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
        {
            ANIMATION_HEADER_T AniHeader;
            BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

            SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
            SKY_CloseDataFile(hFile);
            sprintf((char*)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)szFile);
            /* rosa 2001-09-26 --> @why, what */
            //hControl = EQC_Animation(0, 0, NULL, abBuf);
            hControl = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);
            /* <-- */
            EQC_StartAnimation(hControl, TRUE);
        }
        else
        {
            hControl = EQC_StaticText(0, 50, TKN_PICTURE_NOT_SUPPORT);
//                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PICTURE_NOT_SUPPORT), 
//                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
        }

        /* rosa 2001-09-26 --> @why, what */
        //hName = EQC_StaticText(0,107, TKN_NULL);
        //EQC_SetStaticText(hName, (BYTE*)szFile);
        EQS_SetSWindowHeaderStr((BYTE*)szFile);
        /* <-- */
        break;

    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        EQS_End();
        break;

    case EQE_KEY_0:
    case EQE_KEY_1:
    case EQE_KEY_2:
    case EQE_KEY_3:
    case EQE_KEY_4:
    case EQE_KEY_5:
    case EQE_KEY_6:
    case EQE_KEY_7:
    case EQE_KEY_8:
    case EQE_KEY_9:
    case EQE_KEY_CLEAR:
    case EQE_KEY_LEFT:
	case EQE_KEY_RIGHT:
    case EQE_KEY_F1:
    case EQE_KEY_F2:
    case EQE_KEY_POUND:
    case EQE_KEY_STAR:
    case EQE_KEY_CENTER:
        EQS_End();
        break;

	case EQE_KEY_UP:
	case EQE_KEY_DOWN:
        if(pEvent->EventID == EQE_KEY_UP)
        {
            l_CurrentIndex --;
            if(l_CurrentIndex <0)
                l_CurrentIndex = nScrollData -1;
        }
        else
        {
            l_CurrentIndex ++;
            if(l_CurrentIndex >= nScrollData)
                l_CurrentIndex = 0;
        }

        EQS_DeleteControl(hControl);
        memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));

        memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithoutIcon, EQC_GetScrollData(s_hListHandle, l_CurrentIndex));
#ifdef FEATURE_CHARACTER_BELL
        memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
        STRCPY(WithIcon, EQC_GetScrollData(s_hListHandle, l_CurrentIndex));
        if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
            memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
            STRCPY(WithoutIcon, WithIcon+2);
        }
#endif // FEATURE_CHARACTER_BELL

        szFile = WithoutIcon;
        SKY_OpenDataFile(SDF_IMAGE, szFile, &hFile, SFT_READ);
        SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));
        if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
        {
            SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
            SKY_CloseDataFile(hFile);
            l_BmpBuf.Header.yStart = 0;
            /* rosa 2001-09-26 --> @why, what */
            //hControl = EQC_Image(0, 0, &l_BmpBuf);
            hControl = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            /* <-- */
        }
        else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
        {
            ANIMATION_HEADER_T AniHeader;
            BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

            SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
            SKY_CloseDataFile(hFile);
            sprintf((char*)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)szFile);
            /* rosa 2001-09-26 --> @why, what */
            //hControl = EQC_Animation(0, 0, NULL, abBuf);
            hControl = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);
            /* <-- */
            EQC_StartAnimation(hControl, TRUE);
        }
        else
        {
            hControl = EQC_StaticText(0, 50, TKN_PICTURE_NOT_SUPPORT);
        }

        /* rosa 2001-09-26 --> @why, what */
        //EQC_SetStaticText(hName, (BYTE*)szFile);
        EQS_SetSWindowHeaderStr((BYTE*)szFile);
        /* <-- */
        break;
        
    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
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
//LOCAL void SKY_API sSKA_MyAniRename(SKY_EVENT_T *pEvent)
void SKY_API SKA_MyAniRename(SKY_EVENT_T *pEvent)
{
    static BOOL fChanged;
    static HCONTROL hControl;
    STATIC BYTE s_bPhoto = 0;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_CONFIRM_NONE, 
                      SKY_GET_TOKEN(TKN_PICTURE_MYANI_RENAME_TITLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        s_bPhoto = 0;
        if(pEvent->wParam == POPUP_MENU_MY_PHOTO)
        {
            EQS_SetSWindowHeaderStr((BYTE*)SKY_GET_TOKEN(TKN_PHOTO_MYANI_RENAME_TITLE));
            s_bPhoto = RENAME_POPUP;
        }

        EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_MYBELL_RENAME);
        
        if(pEvent->wParam != POPUP_MENU_MY_PHOTO)
        {
            hControl = EQC_TextEdit(EQS_NA, EQS_NA, SKY_FILE_NAME_MAX_LENGTH-1, 1, 
                     TEM_KOREAN, SKY_FILE_NAME_MAX_LENGTH-1,
                     g_szChangedName, 0);
        }else
        {
            hControl = EQC_TextEdit(EQS_NA, EQS_NA, SKY_FILE_NAME_MAX_LENGTH, 1, 
                     TEM_KOREAN, CAMERA_NAME_EDIT_MAX_LEN,
                     g_szChangedName, 0);
        }
        
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

        EQC_GetTextEditData(hControl, g_szChangedName);
        
        if(!s_bPhoto && SKY_IsDataFile(SDF_IMAGE, g_szChangedName))
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_FILENAME_EXIST), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;
        }

        /* rosa 2001-10-04 --> @why, what */
        if(FV_OK == SKY_IsValidFileName(g_szChangedName))
        {
            fChanged = TRUE;
            EQS_END_EVENT( SE_APP_RESUME, (WORD)fChanged, (DWORD)s_bPhoto);
        }else
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_INVALID_FILENAME), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);        
        }
        /* <-- */
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hControl);
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
void SKY_API SKA_MainMyFileInfo(SKY_EVENT_T *pEvent)
{
    BYTE    pszTmp[25];
#ifdef FEATURE_SKVM
    int sUsedSKVMSpace = 0;
#endif // FEATURE_SKVM

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_PICTURE_DETAIL), 
                      WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WAITING_A_MOMENT), 
                     0, MBT_NONE, MBT_HELP);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_MESGBOX_END:
        // 나만의 그림
        sprintf((char*)pszTmp, "그림:    %2d%4dKB", 
                SKY_GetFileNumberInDataFile(SDF_IMAGE),
                GET_KB_SIZE(SKY_GetUsedSpaceInDataFile(SDF_IMAGE)));
        EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);
        
        // 나만의 멜로디
        sprintf((char*)pszTmp, "멜로디:  %2d%4dKB", 
                SKY_GetFileNumberInDataFile(SDF_MELODY),
                GET_KB_SIZE(SKY_GetUsedSpaceInDataFile(SDF_MELODY)));
        EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);
        
        // 나만의 사진
        sprintf((char*)pszTmp, "사진:    %2d%4dKB", 
                SKY_GetFileNumberInDataFile(SDF_PHOTO),
                GET_KB_SIZE(SKY_GetUsedSpaceInDataFile(SDF_PHOTO)));
        EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);

#ifdef FEATURE_SKVM
        // GVM & SKVM
        {
            WORD sNumOfFiles, szDirNum;
            BYTE szDirNameList[SKY_PATH_NAME_MAX_LENGTH * SKY_MAX_NUM_FILES];
            BYTE aszDirNameList[SKY_MAX_NUM_FILES][SKY_PATH_NAME_MAX_LENGTH];
            BYTE *pszPathName;
            int sUsedSpace, i;
            
            if (SKY_IsFileOrDirExist((CONST BYTE*)SKY_SKVM_DIR)) 
            { // xce 폴더가 있는 경우
                szDirNum = SKY_GetDirNameList((const BYTE*)SKY_SKVM_DIR, szDirNameList);
                sNumOfFiles = SKY_GetFileNumberInDataFile(SDF_GVM);
            
                if (szDirNum > 1)
                {
                    sNumOfFiles += (szDirNum - 1);
                }
            
                sUsedSpace = GET_KB_SIZE(SKY_GetUsedSpaceInDataFile(SDF_GVM));
            
                pszPathName = szDirNameList;
                for(i = 0; i < szDirNum; i++)
                {
		            STRCPY(aszDirNameList[i], SKY_SKVM_DIR);
		            aszDirNameList[i][STRLEN(SKY_SKVM_DIR)]='/';
                    STRCPY(&aszDirNameList[i][STRLEN(SKY_SKVM_DIR)+1],pszPathName);
                    pszPathName += (STRLEN(pszPathName) + 1);
                    sUsedSKVMSpace += GET_KB_SIZE(SKY_GetDirSize(aszDirNameList[i]));
                }
                sUsedSpace += sUsedSKVMSpace;
                sprintf((char*)pszTmp, "마법사:  %2d%4dKB", sNumOfFiles, sUsedSpace);
                EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);
            }
            else 
            { // xce 폴더가 없는 경우
                sNumOfFiles = SKY_GetFileNumberInDataFile(SDF_GVM);
                sUsedSpace = GET_KB_SIZE(SKY_GetUsedSpaceInDataFile(SDF_GVM));
                
                sprintf((char*)pszTmp, "마법사:  %2d%4dKB", sNumOfFiles, sUsedSpace);
                EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);
            } 
        }
#else
        // GVM 
        sprintf((char*)pszTmp, "마법사:  %2d%4dKB", 
                SKY_GetFileNumberInDataFile(SDF_GVM),
                GET_KB_SIZE(SKY_GetUsedSpaceInDataFile(SDF_GVM)));
        EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);
#endif // FEATURE_SKVM

#ifdef FEATURE_WAVELET
        // Wavelet
        sprintf((char*)pszTmp, "WAVELET: %2d%4dKB", 
                (SKY_GetFileNumberInDataFile(SDF_WAVELET))/4,
                GET_KB_SIZE(SKY_GetUsedSpaceInDataFile(SDF_WAVELET)));
        EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);
#endif //FEATURE_WAVELET
        
        // 포토메일
        sprintf((char*)pszTmp, "포토메일:%2d%4dKB", 
                (SKY_GetFileNumberInDataFile(SDF_VM)),
                GET_KB_SIZE(SKY_GetUsedSpaceInDataFile(SDF_VM)));
        EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);

        // 남은용량
        //sprintf((char*)pszTmp, "-----------------");
        //EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);
        EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

        // sUsedSKVMSpace을 따로 빼주지 않아도 
        // SKY_GetFreeSpaceInDataFile 이 함수를 거치는 동안 계산되어진다.
        sprintf((char*)pszTmp, "남은량:    %4dKB", 
                (GET_KB_SIZE(SKY_GetFreeSpaceInDataFile())));

        EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), pszTmp);
#ifdef FEATURE_WAVELET
        EQS_AutoFormatting(8, EHA_LEFT, EVA_CENTER);
#else //FEATURE_WAVELET
		EQS_AutoFormatting(7, EHA_LEFT, EVA_CENTER);
#endif //FEATURE_WAVELET
        break;

/*
    case EQE_KEY_0:
    case EQE_KEY_1:
    case EQE_KEY_2:
    case EQE_KEY_3:
    case EQE_KEY_4:
    case EQE_KEY_5:
    case EQE_KEY_6:
    case EQE_KEY_7:
    case EQE_KEY_8:
    case EQE_KEY_9:
    case EQE_KEY_CLEAR:
    case EQE_KEY_LEFT:
	case EQE_KEY_RIGHT:
    case EQE_KEY_F1:
    case EQE_KEY_F2:
    case EQE_KEY_POUND:
    case EQE_KEY_STAR:
    case EQE_KEY_CENTER:
    case EQE_KEY_END:
    case EQE_KEY_UP:
    case EQE_KEY_DOWN:
        EQS_End();
        break;
*/        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

