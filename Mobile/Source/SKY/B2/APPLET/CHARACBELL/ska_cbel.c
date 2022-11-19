/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_cbel.c
**
** Description: 
**     CharacBell Menu (캐릭벨 설정) 
**      by bgjung
**      처음 캐릭벨 목록이 보이는 ScrollList의 경우
**      PhysicalLine을 MAX_NUM_OF_CHARACBELL로 고정시켜놓았다.
**      그러므로 EQC_GetScrollDataLine 같은거 쓸때 주의하기
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When         who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-08-20  bgjung       Created.
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
#define NUM_IMAGE_MODE   4

#define PIC_SCROLL_LIST_CHAR    16
#define BELL_SCROLL_LIST_CHAR   16
#define GROUP_CHECK_BOX_CHAR    14

#define SMB_SCROLL_LIST_STYLE   CS_SCROLL_LIST_BORDER| \
                                CS_SCROLL_LIST_SCROLL_BAR | \
                                CS_SCROLL_LIST_ENUMBERATED | \
                                CS_SCROLL_LIST_MAXIMIZE | \
                                CS_SCROLL_LIST_NO_CML

#define sRESET_CONFIG_DIRTY(SkyConfig)  g_aSkyConfig[SkyConfig].fDirty = FALSE
                                
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tag_SKA_CHABELL_MESSAGEBOX_E {
    SMM_NOBELL,
    SMM_ADDED,
    SMM_CHANGED,
    SMM_DELETE_CONFIRM,
    SMM_DELETED,
    SMM_DELETEALL_CONFIRM,
    SMM_ALLDELETED,
    SMM_SELECTED,
    SMM_NOCHANGE
    //SMM_NONE
};

typedef enum tag_SKA_CHABELL_MESSAGEBOX_E SKA_CHABELL_MESSAGEBOX_E;

enum tag_SKA_CHABELL_RESULTVALUE_E {
    SMM_NOTDELETE,
    SMM_NUMBERDELETE,
    SMM_CHABELLDELETE
};

typedef enum tag_SKA_CHABELL_RESULTVALUE_E SKA_CHABELL_RESULTVALUE_E;
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
enum tag_SKA_CHARACBELL_E {
    CB_CHABELL_TITLE,
    CB_CHABELL_TITLEINPUT,
    CB_BELL_TYPE_TITLE,
    CB_BELL_ID,
    CB_IMAGE_TYPE_TITLE,
    CB_IMAGE_ID,
    CB_PHONE_NUMBER,
    CB_PHONE_NUMBER_TITLE1,
    CB_PHONE_NUMBER1,
    CB_PHONE_NUMBER_TITLE2,
    CB_PHONE_NUMBER2,
    CB_PHONE_NUMBER_TITLE3,
    CB_PHONE_NUMBER3,
    CB_PHONE_NUMBER_TITLE4,
    CB_PHONE_NUMBER4,
    CB_PHONE_NUMBER_TITLE5,
    CB_PHONE_NUMBER5,
    CB_PHONE_NUMBER_TITLE6,
    CB_PHONE_NUMBER6,
    CB_GROUP_BELL_TITLE,
    //CB_GROUP_BELL,
    CB_MAX
};

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
// 착신번호 전화부와 연동하는데 필요
HCONTROL    hControlGetPN;
char        sGetPhoneNumber[MAX_TITLE_LENGTH];

extern CONST BELL_NAME_TYPE_T aBell_List[NUM_BELL_LIST];
//extern CONST PICTURE_DATA_TYPE_T l_pPictureList[NUM_ANI_DATA];
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL BYTE *ppBellGroup[NUM_BELL_GROUP+1];  // 벨종류
LOCAL WORD wBellList[NUM_BELL_GROUP+1]; // 기본벨목록에 나만의 벨과 캐릭벨이 추가됨.

LOCAL BYTE *ppMyBellList[SKY_MAX_NUM_FILES];
LOCAL UINT8 s_nMyBell, s_nMyBellAlloc;

LOCAL HCONTROL s_hListHandle;

LOCAL SKA_CHABELL_MESSAGEBOX_E   MessageReason;
LOCAL SKA_CHABELL_RESULTVALUE_E sResultValue;  
LOCAL UINT8 s_nBellGroup;   // 그룹벨지정해제 index

LOCAL BYTE *ppTypeList[NUM_IMAGE_MODE-1]; // 그림없음은 없앤다.
LOCAL BYTE *ppAniList[NUM_ANI_DATA + (2*SKY_MAX_NUM_FILES)];  
LOCAL BYTE *ppMyANIList[SKY_MAX_NUM_FILES];
LOCAL BYTE *ppMyPhotoList[SKY_MAX_NUM_FILES];
LOCAL BYTE *ppMyPhotoName[SKY_MAX_NUM_FILES+1];

LOCAL BYTE *ppChaBellList[MAX_NUM_OF_CHARACBELL]; // 캐릭벨 제목 넣는다.
LOCAL UINT8 s_nChaBellAlloc;

LOCAL UINT8 s_nMyANI, s_nMyANIAlloc, s_nMyPhoto;

LOCAL UINT SelectedIndex;
LOCAL BOOL bCharacbellChange;

LOCAL BYTE *ppBellList[NUM_BELL_LIST+SKY_MAX_NUM_FILES+2];  // 벨 list (나만의벨/캐릭벨)

LOCAL BYTE *ppSortedGroupName[1];

LOCAL pb_group_info_type  s_GroupInfo;
LOCAL BOOL sFirst;

LOCAL BITMAP l_BmpBuf = {
    { EQUS_IMAGE_ID, { SKY_SCREEN_BUFFER_WIDTH, SKY_SCREEN_BUFFER_HEIGHT}, 
        SKY_SCREEN_WIDTH, BIT_PER_PIXEL_256C, FALSE, MAX_COLOR, 0 },
    g_abBmpBuf
};

LOCAL PICTURE_DATA_TYPE_T *l_pPictureList;

//int sSavedValue = -1;
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL BOOL EQS_API sAddChaBellControls(void); // 메인화면 등록

LOCAL void SKY_API sSKA_ChaBellAdd(SKY_EVENT_T *pEvent); // 캐릭벨 설정
LOCAL void EQS_API sAddCharacbellControls(void); // 설정시 컨트롤 등록

LOCAL void SKY_API sSKA_ChaBellChange(SKY_EVENT_T *pEvent); // 캐릭벨 편집
LOCAL void EQS_API sAddCharacbellInfoControls(void); // 편집시 컨트롤 등록

LOCAL void SKY_API sSKA_ChaBellSelected(SKY_EVENT_T *pEvent); // 캐릭벨 미리 보기

LOCAL void DeleteCharacbell(int index, BOOL sFromMenu); // 캐릭벨 해제
LOCAL void DeleteAllCharacbell(void); // 캐릭벨 전부 해제

LOCAL void SKY_API sSaveCharacbell(void); // 저장 (설정/편집)
// 저장시 중복된 번호가 있는지 확인
void CheckDuplicateNumber (char *temp, int DeletedIndex);

// 설정/편집을 위해 필요한 벨 목록과 그림 목록을 불러들이는데 필요
LOCAL void EQS_API sGetCBMelodyListInfo(void);
LOCAL void EQS_API sAddCBMelodyListControl(void);
LOCAL void EQS_API sGetCBImageListInfo(void);
LOCAL void EQS_API sAddCBImageListControl(void);

// 그룹에서 캐릭벨 세팅하느라 필요
LOCAL void EQS_API sMakeGroupList(void);

LOCAL BOOL EQS_API sIsMyBell(void);
LOCAL BOOL EQS_API sIsMyPicture(void);
LOCAL BOOL EQS_API sIsPhoto(void);

LOCAL void EQS_API CBPlayCurrentBell(void);

LOCAL UINT8 EQS_API sGetCBMyAniList(void);
LOCAL UINT8 EQS_API sGetCBMyPhotoList(BOOL fIdle);

// 기존에 저장된 값을 로딩할때 필요한 부분.
LOCAL UINT8 FindMyBellIndex(BYTE *pszBell);
LOCAL UINT8 FindBellIndex(WORD wBellId);

LOCAL UINT8 FindMyAniIndex(BYTE *pszANI);
LOCAL UINT8 FindMyPhotoIndex(BYTE *pszANI);
LOCAL UINT8 FindImageIndex(WORD wImageId);

/*void BeforeDelete (int index); // 캐릭벨 삭제시 제반사항 체크
LOCAL void BeforeDeleteAll (void); // 캐릭벨 전체삭제시 제반사항 체크 */

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
void SKY_API SKA_MainCharacBell(SKY_EVENT_T *pEvent)
{
    static UINT8    index, nCBIndex;
    BYTE            pszMessage[MESSAGE_BOX_MAX_MESG_LEN];
    HCONTROL        hControl;
        
    switch(pEvent->EventID)
    {
    case SE_APP_START : // The very first event 
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_MENU_BEFOREVIEW_SET, 
                      SKY_GET_TOKEN(TKN_CHARACBELL_MENU), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        l_pPictureList = GetBuiltInPictureList();

        for(index=0; index < MAX_NUM_OF_CHARACBELL; index ++)
            ppChaBellList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

        sFirst = TRUE;
        sResultValue = SMM_NOTDELETE;

        sAddChaBellControls();
        nCBIndex = 0;
        SelectedIndex = 0;
        if (s_nChaBellAlloc == MAX_NUM_OF_CHARACBELL)
        {
            EQS_SetWindowCML(CL_MENU_BEFOREVIEW_NONE);
        }
        else if (s_nChaBellAlloc == 0)
        {
            EQS_SetWindowCML(CL_MENU_NONE_SET);
        }
        else
        {
            EQS_SetWindowCML(CL_MENU_BEFOREVIEW_SET);
        }

        break;
        
    case SE_APP_END :   // The very last event 
        for(index=0; index < MAX_NUM_OF_CHARACBELL; index ++)
            SKY_Free(ppChaBellList[index]);
        break;
        
    case SE_APP_SUSPEND: // child applet is started 
        break;
        
    case SE_APP_RESUME: // child applet is ended 
        if (MessageReason == SMM_ADDED || MessageReason == SMM_CHANGED) 
        { 
            if (sResultValue == SMM_NOTDELETE) {
                if(sAddChaBellControls())
                {
                    EQC_SetScrollLogicalIndex(s_hListHandle, SelectedIndex);
                    EQS_SetIOCActive(s_hListHandle);
                    nCBIndex = SelectedIndex;

                    if (s_nChaBellAlloc == MAX_NUM_OF_CHARACBELL) 
                    {
                        EQS_SetWindowCML(CL_MENU_BEFOREVIEW_NONE);
                    }
                    else if (s_nChaBellAlloc == 0)
                    {
                        EQS_SetWindowCML(CL_MENU_NONE_SET);
                    }
                    else
                    {
                        EQS_SetWindowCML(CL_MENU_BEFOREVIEW_SET);
                    }
                }
            }
            else if (sResultValue == SMM_NUMBERDELETE)
            {
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_CHARACBELL_DELETENUMBER), 
                                 MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            }
            else if (sResultValue == SMM_CHABELLDELETE)
            {
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_CHARACBELL_DELETECHABELL), 
                                 MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            }
        }
        else 
        {
            if (SKY_GET_CB_ADDRESS() == 0)
            {
                break;
            }
            else
            {
                EQC_SetScrollLogicalIndex(s_hListHandle, SelectedIndex);
                EQS_SetIOCActive(s_hListHandle);
                nCBIndex = SelectedIndex;
            }
        }
        break;

    case SE_SOFTKEY_MENU: 
        // 1 캐릭벨 설정
        // 2 캐릭벨 편집
        // 2 캐릭벨 해제
        // 3 캐릭벨 전부해제
        {
            SKY_TOKEN paMenu[4] =
            {
                TKN_CHARACBELL_NEW,
                TKN_CHARACBELL_CHANGE,
				TKN_CHARACBELL_DELETE,
                TKN_CHARACBELL_DELETEALL,
            };

            BOOL    fActiveMenu[4];
            for (index = 0; index < 4; index++)
            {
                fActiveMenu[index] = TRUE;
            }

            if (SKY_GET_CB_ADDRESS() == MAX_NUM_OF_CHARACBELL) 
            {
                fActiveMenu[0] = FALSE;
            }
            else if (SKY_GET_CB_ADDRESS() == 0) 
            {
                fActiveMenu[1] = FALSE;
                fActiveMenu[2] = FALSE;
                fActiveMenu[3] = FALSE;
            }

            hControl = EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paMenu, 4, 0, fActiveMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
        break;

    case SE_IO_POPUP_SELECT:
        switch(pEvent->dwParam)
        {   
        case TKN_CHARACBELL_NEW: // 캐릭벨 설정
            //SelectedIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
            MessageReason = SMM_ADDED;
            EQS_START_CHILD(sSKA_ChaBellAdd);
            break;

        case TKN_CHARACBELL_CHANGE: // 캐릭벨 편집
            //SelectedIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
            MessageReason = SMM_CHANGED;
            EQS_START_CHILD(sSKA_ChaBellChange);
            break;

        case TKN_CHARACBELL_DELETE: // 캐릭벨 해제 
            sprintf(pszMessage, "\"%s\" ", EQC_GetScrollData(s_hListHandle, nCBIndex));
            STRNCAT(pszMessage, (BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_CONFIRM_DELETE),
                    MESSAGE_BOX_MAX_MESG_LEN);
        
            MessageReason = SMM_DELETE_CONFIRM;
            EQC_MessageBox(pszMessage, 
                            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;

        case TKN_CHARACBELL_DELETEALL: // 캐릭벨 전부 해제
            SelectedIndex = 0;
            sprintf(pszMessage, "\"%s\" ", (BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_CONFIRM_DELETEALL));
                
            MessageReason = SMM_DELETEALL_CONFIRM;
            EQC_MessageBox(pszMessage, 
                            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        break;

    case SE_IO_POPUP_CANCEL:
        EQS_SetIOCActive(s_hListHandle);
        break;

    case SE_SOFTKEY_SET: // [설정]키를 선택한 경우
        //SelectedIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
        MessageReason = SMM_ADDED;
        EQS_START_CHILD(sSKA_ChaBellAdd);
        break;

    case SE_IO_MESGBOX_OK:
    case SE_IO_MESGBOX_CLEAR:
    case SE_IO_MESGBOX_END:
        if ((sResultValue == SMM_NUMBERDELETE) || (sResultValue == SMM_CHABELLDELETE))
        {
            if(sAddChaBellControls())
            {
                EQC_SetScrollLogicalIndex(s_hListHandle, SelectedIndex);
                EQS_SetIOCActive(s_hListHandle);
                nCBIndex = SelectedIndex;

                if (s_nChaBellAlloc == MAX_NUM_OF_CHARACBELL) 
                {
                    EQS_SetWindowCML(CL_MENU_BEFOREVIEW_NONE);
                }
                else if (s_nChaBellAlloc == 0)
                {
                    EQS_SetWindowCML(CL_MENU_NONE_SET);
                }
                else
                {
                    EQS_SetWindowCML(CL_MENU_BEFOREVIEW_SET);
                }
            }
            sResultValue = SMM_NOTDELETE;
        }
        else
        {
            switch(MessageReason)
            {
            case SMM_NOBELL:
                //EQS_END();
                EQS_END_TO(EQS_POP_TO_ROOT);
                break;

            /*case SMM_NONE:
                break;*/

            case SMM_DELETED:
                // 지우고나서 캐릭벨이 없다는 메시지를 띄우면
                // 잘 지웠다는 메시지랑 두번이 뜨는 것이니까 우선은 막았당.
                if (SKY_GET_CB_ADDRESS() < MAX_NUM_OF_CHARACBELL)
                {
                    EQS_SetWindowCML(CL_MENU_BEFOREVIEW_SET);
                }
                else if (s_nChaBellAlloc == 0)
                {
                    EQS_SetWindowCML(CL_MENU_NONE_SET);
                }
                else
                {
                    EQS_SetWindowCML(CL_MENU_BEFOREVIEW_SET);
                }

                EQC_SetScrollLogicalIndex(s_hListHandle, 0);
                EQS_SetIOCActive(s_hListHandle);
                break;
        
            case SMM_ALLDELETED:
                EQS_END_TO(EQS_POP_TO_ROOT);
                break;
            }
        }        
        break;

    case SE_IO_MESGBOX_YES:
        switch (MessageReason) 
        {
        case SMM_DELETE_CONFIRM:
            DeleteCharacbell(SelectedIndex, TRUE);
            //EQC_DeleteScrollData(s_hListHandle, SelectedIndex);
            if (sAddChaBellControls())
            {
                MessageReason = SMM_DELETED;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_DELETED), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            }
            else 
            {
                MessageReason = SMM_NOBELL;
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_DELETED), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            }
            break;        
        case SMM_DELETEALL_CONFIRM:
            DeleteAllCharacbell();
            /*for (index = 0; index < SKY_GET_CB_ADDRESS(); index++)
                EQC_DeleteScrollData(s_hListHandle, index);*/
            MessageReason = SMM_ALLDELETED;
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_ALLDELETED), 
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            break;
        }
        break;

    case SE_IO_MESGBOX_NO:
        EQS_SetIOCActive(s_hListHandle);
        nCBIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_End();
        //EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_SCROLL_LIST_CHG:
        nCBIndex = pEvent->dwParam;
        SelectedIndex = nCBIndex;
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        MessageReason = SMM_SELECTED;
        SelectedIndex = (UINT8)EQC_GetScrollLogicalIndex(s_hListHandle);
        EQS_START_CHILD(sSKA_ChaBellSelected);
        break;
    
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
} // SKA_MainCharacBell

LOCAL BOOL EQS_API sAddChaBellControls(void)
{
    UINT8   index, i;

    // Load user file list
    s_nChaBellAlloc = SKY_GET_CB_ADDRESS(); // 현재 저장된 캐릭벨 개수

    if(!s_nChaBellAlloc) // 저장된 캐릭벨이 없는 경우.
        return FALSE;

    for(index=0, i=0; i < MAX_NUM_OF_CHARACBELL; i++) 
    {
        memset(ppChaBellList[i], 0x00, SKY_PATH_NAME_MAX_LENGTH);
        if (SKY_GET_CB_EMPTY(i)) 
        {
            STRCPY(ppChaBellList[index], SKY_GET_CB_TITLE(i));
            index++;
        }
    } // for

    if (sFirst) 
    { // SKA_MainCharacbell (APP_START)
        s_hListHandle = EQC_ScrollList(EQS_NA, EQS_NA, 18, 
                       s_nChaBellAlloc, ppChaBellList, 
                       MAX_NUM_OF_CHARACBELL, 
                       s_nChaBellAlloc,
                       0);

        EQC_SET_X(s_hListHandle, 0);
        EQS_SetIOCActive(s_hListHandle);
        EQC_SET_STYLE(s_hListHandle, SMB_SCROLL_LIST_STYLE);
        sFirst = FALSE;
    }
    else
    { // 설정, 편집, 해제
        EQC_SetScrollViewLine (s_hListHandle, s_nChaBellAlloc);
        EQC_SetScrollMaxDataLine (s_hListHandle, s_nChaBellAlloc);
        for (index = 0; index < s_nChaBellAlloc; index++) 
        {
            EQC_SetScrollData (s_hListHandle, index, ppChaBellList[index]);
        }
    }
    return TRUE;
} // sAddChaBellControls

void SKY_API sSKA_ChaBellAdd(SKY_EVENT_T *pEvent)
{
    static BOOL fEndMessage, fSaveMessage, fIdle;
    char        temp[MAX_PHONENUMBER_LENGTH];
    UINT        index;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_CHARACBELL_NEW), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        l_pPictureList = GetBuiltInPictureList();

        sAddCharacbellControls();
        fEndMessage = fSaveMessage = fIdle = FALSE;
        EQS_FitCanvasSize();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        SelectedIndex = -1;
        break;

    case SE_APP_END :   // The very last event 
        for(index = 0; index < s_nMyBellAlloc; index++)
            SKY_Free(ppMyBellList[index]);
        for(index = 0; index < s_nMyANIAlloc; index++)
            SKY_Free(ppMyANIList[index]);
        for(index=0; index < s_nMyPhoto; index ++)
		{
			SKY_Free(ppMyPhotoList[index]);
			if (fIdle)
			{
				SKY_Free(ppMyPhotoName[index+1]);
			}
			else
			{
				SKY_Free(ppMyPhotoName[index]);
			}
		}
        SKY_Free(ppSortedGroupName[0]);
        
        SelectedIndex = 0;
        SKY_StopMusic();
        break;

    case SE_APP_SUSPEND: // child applet is started 
        SKY_StopMusic();
        break;
        
    case SE_APP_RESUME: // child applet is ended 
        // 전화부 검색을 마치고 돌아올 때
        if (pEvent->dwParam) 
        {// 값을 선택하고 돌아오는 경우
            // 여기서 값을 세팅해주는 부분이 필요하다.
            EQC_SetNumberEditValue(hControlGetPN, sGetPhoneNumber);
            EQC_SET_DIRTY(EQS_GetFocusedIOC(), TRUE);
            EQC_SET_FOCUS(EQS_GetFocusedIOC()+2, TRUE);
            EQS_SetIOCFocus(EQS_GetFocusedIOC()+2); 
        }
        break;

    case SE_KEY_CLEAR:        
    case SE_KEY_END :   // The very last event 
        // 상위로 나가기 전에 데이타가 바뀌었는지 확인하고
        // 바뀐 내용이 있는 경우는 저장여부를 확인해야 한다.
        SKY_StopMusic();
        
        if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdle = TRUE;
		}
		else
		{
            MessageReason = SMM_NOCHANGE;
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;


    /* This Event Comes From SKA_MainPbookCode  hjcho 01.11.09*/
    case SE_SOFTKEY_PBOOK:
        if(pEvent->wParam == BOOKCODE_SEARCH_IN_CBELL)
        {
            memset(sGetPhoneNumber, 0x00, MAX_TITLE_LENGTH);
            EQS_START_CHILD(SKA_CBSearchNumber);
        }
        break;

        
    case SE_SOFTKEY_FIND: // 전화부 검색을 하는 경우..
        hControlGetPN = EQS_GetFocusedIOC();
        if( EQC_GET_TYPE(hControlGetPN) == CT_NUMBER_EDIT ) 
        {
            if(SKY_IS_PHONE_BOOK_LOCK())
            {
                EQS_START_CHILD_EVENT(SKA_MainPbookCode,SE_APP_START,BOOKCODE_SEARCH_IN_CBELL, EQS_NA);
            }
            else
            {
                memset(sGetPhoneNumber, 0x00, MAX_TITLE_LENGTH);
                EQS_START_CHILD(SKA_CBSearchNumber);
            }
        }
        break;

    case SE_IO_NEDIT_DONE:
        EQC_SET_FOCUS(EQS_GetNthIOC(EQS_GetIOCOrder(pEvent->wParam) + 2), TRUE);

    case SE_TM_TIMER1:
        if (EQC_GET_TYPE(EQS_GetFocusedIOC()) == CT_NUMBER_EDIT)
        {
            EQS_SetWindowCML(CL_FIND_SELECT_SAVE);
        }
        else
        {
            EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
        }

        if(EQS_HaveFocus(EQS_GetNthIOC(CB_BELL_ID)) && EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            SKY_StopMusic();
            CBPlayCurrentBell();
        }
        break;

    // OK키를 이용해서 각각의 컨트롤을 선택할때 
    case SE_APP_IO_MODE:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
            case CB_BELL_ID: // 벨 종류를 선택하는 경우
                SKY_StopMusic();
                EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
                break;
        
            default:
                break;
        }
        break;

    // 컨트롤들을 움직이면서 돌아다닐때
    case SE_IO_SCROLL_LIST_SELECT:
	case SE_IO_FOCUS_CHG:
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        SKY_StopMusic();
        break;

    case SE_IO_SCROLL_LIST_CHG:
    case SE_IO_SCROLL_TAB_CHG:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
            case CB_BELL_ID:
                SKY_StopMusic();
                EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
                break;
            
            default:
                break;
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->wParam == EQS_GetNthIOC(CB_BELL_ID))
        {
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        }
        break;

    case SE_IO_CHECK_BOX_CHECK:
    case SE_IO_CHECK_BOX_UNCHECK:
        break;

    case SE_IO_CHECK_BOX_UP:
    case SE_IO_CHECK_BOX_DOWN:
        break;

    case SE_IO_MESGBOX_END:
        if(fIdle)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        else if(fEndMessage)
        {
            EQS_End();
        }
        break;

    case SE_IO_MESGBOX_YES:
        // 착신번호1이 비어있는 경우는 필수입력사항이니까..
        memset (temp, 0x00, MAX_PHONENUMBER_LENGTH);
        EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER1), temp);
        if (*temp == NULL) 
        { 
            EQS_SetIOCFocus(EQS_GetNthIOC(CB_PHONE_NUMBER1));
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_NOPHONENUMBER), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if (fSaveMessage) 
        {
            sSaveCharacbell();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_NO:
        MessageReason = SMM_NOCHANGE;
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    case SE_SOFTKEY_SAVE:
        SKY_StopMusic();

        memset (temp, 0x00, MAX_PHONENUMBER_LENGTH);
        EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER1), temp);
        if (*temp == NULL) 
        { 
            EQS_SetIOCFocus(EQS_GetNthIOC(CB_PHONE_NUMBER1)); 
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_NOPHONENUMBER), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else 
        { 
            sSaveCharacbell();

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            fEndMessage = TRUE;
        }
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
} // sSKA_ChaBellAdd

LOCAL void EQS_API sAddCharacbellControls(void) 
{
    UINT8           nIndex, nGroupIndex;
    HCONTROL hControl;

    // 캐릭벨 제목
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_CHARACBELL_TITLE);
    EQC_TextEdit(EQS_NA, EQS_NA, SKY_FILE_NAME_MAX_LENGTH+1, 1, 
                     TEM_KOREAN, SKY_FILE_NAME_MAX_LENGTH+1,
                     NULL, 0);

    // 벨 종류들을 load
    sGetCBMelodyListInfo();
    sAddCBMelodyListControl();

    // 그림 종류들을 load
    sGetCBImageListInfo();
    sAddCBImageListControl();
    
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_CHARACBELL_PHONENUMBER); // 상대방 전화번호 

    for(nIndex = 0; nIndex < MAX_NUM_OF_PHONENUMBER; nIndex++)
    {
        EQC_StaticText(EQS_NA, EQS_NA, (WORD)(TKN_SMS_DONGBO_NULL01 + nIndex));
        hControl = EQC_NumberEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE-2, 1, MAX_DONGBO_DIAL, "");
        if( nIndex > 0 )
        {
            EQC_SET_FOCUS(hControl, FALSE);
        }
    }

    // 그룹 설정 추가
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_CHARACBELL_GROUPNAME);
    sMakeGroupList ();

    // Screen Formatting
    //EQS_AutoFormatting(CB_MAX, EHA_LEFT, EVA_CENTER);
    EQS_StartFormatter();
    
    for(nIndex = 0; nIndex < CB_MAX; nIndex++)
    {
        if(nIndex == CB_CHABELL_TITLEINPUT) 
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nIndex), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
            EQC_GET_X(EQS_GetNthIOC(nIndex)) -= (R_SCROLLBAR_SIZE)/2;
        }
        else if((nIndex >= CB_PHONE_NUMBER_TITLE1) && (nIndex <= CB_PHONE_NUMBER6)) 
        {
            if (nIndex == CB_PHONE_NUMBER_TITLE1)
                EQC_SetStaticText(EQS_GetNthIOC(nIndex), (BYTE*)SF_BLACK_NUM1);  
            else if (nIndex == CB_PHONE_NUMBER_TITLE2)
                EQC_SetStaticText(EQS_GetNthIOC(nIndex), (BYTE*)SF_BLACK_NUM2);  
            else if (nIndex == CB_PHONE_NUMBER_TITLE3)
                EQC_SetStaticText(EQS_GetNthIOC(nIndex), (BYTE*)SF_BLACK_NUM3);  
            else if (nIndex == CB_PHONE_NUMBER_TITLE4)
                EQC_SetStaticText(EQS_GetNthIOC(nIndex), (BYTE*)SF_BLACK_NUM4);  
            else if (nIndex == CB_PHONE_NUMBER_TITLE5)
                EQC_SetStaticText(EQS_GetNthIOC(nIndex), (BYTE*)SF_BLACK_NUM5);  
            else if (nIndex == CB_PHONE_NUMBER_TITLE6)
                EQC_SetStaticText(EQS_GetNthIOC(nIndex), (BYTE*)SF_BLACK_NUM6);  
          
            EQS_SetControlPosition(EQS_GetNthIOC(nIndex), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            nIndex++;
            EQS_SetControlPosition(EQS_GetNthIOC(nIndex), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQC_GET_X(EQS_GetNthIOC(nIndex)) -= (R_SCROLLBAR_SIZE)/2;
        }
        else
        {

            EQS_SetControlPosition(EQS_GetNthIOC(nIndex), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        }
        EQS_NewLine();
    }
    for(nGroupIndex = 0; nGroupIndex < g_nGroup; nGroupIndex++)
    {
        EQS_SetControlPosition(EQS_GetNthIOC(nGroupIndex+nIndex), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();
    }
    EQS_EndFormatter();
} // sAddCharacbellControls method

LOCAL void SKY_API sSKA_ChaBellChange(SKY_EVENT_T *pEvent) 
{
    static BOOL     fEndMessage, fSaveMessage, fIdle;
    static char     temp[MAX_PHONENUMBER_LENGTH];
    UINT8           index;

    switch(pEvent->EventID)
    {
    case SE_APP_START : // The very first event 
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_FIND_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_CHARACBELL_CHANGE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        l_pPictureList = GetBuiltInPictureList();

        bCharacbellChange = TRUE;

        // 헤더 부분을 캐릭벨 제목으로 바꿔주는 부분.
        memset(temp, 0x00, 30);
        STRCPY(temp, EQC_GetScrollData(s_hListHandle, SelectedIndex));
        EQS_SetSWindowHeaderStr((BYTE*)temp);

        sAddCharacbellInfoControls();
        fEndMessage = fSaveMessage = fIdle = FALSE;
        EQS_FitCanvasSize();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;

    case SE_APP_END :   // The very last event 
        for(index = 0; index < s_nMyBellAlloc; index++)
            SKY_Free(ppMyBellList[index]);
        for(index = 0; index < s_nMyANIAlloc; index++)
            SKY_Free(ppMyANIList[index]);
        
		for(index=0; index < s_nMyPhoto; index ++)
		{
			SKY_Free(ppMyPhotoList[index]);
			if (fIdle)
			{
				SKY_Free(ppMyPhotoName[index+1]);
			}
			else
			{
				SKY_Free(ppMyPhotoName[index]);
			}
		}
        SKY_Free(ppSortedGroupName[0]);

        SKY_StopMusic();
        bCharacbellChange = FALSE;
        break;
    
    case SE_KEY_CLEAR:        
    case SE_KEY_END :   // The very last event  
        // 상위로 나가기 전에 데이타가 바뀌었는지 확인하고
        // 바뀐 내용이 있는 경우는 저장여부를 확인해야 한다.
        SKY_StopMusic();

        if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdle = TRUE;
		}
		else
		{
            MessageReason = SMM_NOCHANGE;
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;
        
    case SE_APP_SUSPEND: // child applet is started 
        SKY_StopMusic();
        break;
        
    case SE_APP_RESUME: // child applet is ended 
        // 전화부 검색을 마치고 돌아올 때
        if (pEvent->dwParam) 
        {// 값을 선택하고 돌아오는 경우
            EQC_SetNumberEditValue(hControlGetPN, sGetPhoneNumber);
            EQC_SET_DIRTY(EQS_GetFocusedIOC(), TRUE);
            EQC_SET_FOCUS(EQS_GetFocusedIOC()+2, TRUE);
            EQS_SetIOCFocus(EQS_GetFocusedIOC()+2); 
        }
        break;

    /* This Event Comes From SKA_MainPbookCode  hjcho 01.11.09*/
    case SE_SOFTKEY_PBOOK:
        if(pEvent->wParam == BOOKCODE_SEARCH_IN_CBELL)
        {
            memset(sGetPhoneNumber, 0x00, MAX_TITLE_LENGTH);
            EQS_START_CHILD(SKA_CBSearchNumber);
        }
        break;

    case SE_SOFTKEY_FIND: // 전화부 검색을 하는 경우..
        hControlGetPN = EQS_GetFocusedIOC();
        if( EQC_GET_TYPE(hControlGetPN) == CT_NUMBER_EDIT ) 
        {
            if(SKY_IS_PHONE_BOOK_LOCK())
            {
                EQS_START_CHILD_EVENT(SKA_MainPbookCode,SE_APP_START,BOOKCODE_SEARCH_IN_CBELL, EQS_NA);
            }
            else
            {
                memset(sGetPhoneNumber, 0x00, MAX_TITLE_LENGTH);
                EQS_START_CHILD(SKA_CBSearchNumber);
            }
        }
        break;

    case SE_IO_NEDIT_DONE:
        EQC_SET_FOCUS(EQS_GetNthIOC(EQS_GetIOCOrder(pEvent->wParam) + 2), TRUE);

    case SE_TM_TIMER1:
        if (EQC_GET_TYPE(EQS_GetFocusedIOC()) == CT_NUMBER_EDIT)
        {
            EQS_SetWindowCML(CL_FIND_SELECT_SAVE);
        }
        else
        {
            EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
        }

        if(EQS_HaveFocus(EQS_GetNthIOC(CB_BELL_ID)) && EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            SKY_StopMusic();
            CBPlayCurrentBell();
        }
        break;

    // OK키를 이용해서 각각의 컨트롤을 선택할때 
    case SE_APP_IO_MODE:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case CB_BELL_ID: // 벨 종류를 선택하는 경우
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;
        
        default:
            break;
        }
        break;

    // 컨트롤들을 움직이면서 돌아다닐때
    case SE_IO_SCROLL_LIST_SELECT:
	case SE_IO_FOCUS_CHG:
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        SKY_StopMusic();
        break;

    case SE_IO_SCROLL_LIST_CHG:
    case SE_IO_SCROLL_TAB_CHG:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case CB_BELL_ID:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        default:
            break;
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->wParam == EQS_GetNthIOC(CB_BELL_ID))
        {
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        }
        break;
    
    case SE_IO_MESGBOX_END:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_End();
        break;

    case SE_IO_MESGBOX_YES:
        // 착신번호1이 비어있는 경우는 필수입력사항이니까..
        memset (temp, 0x00, MAX_PHONENUMBER_LENGTH);
        EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER1), temp);
        if (*temp == NULL) 
        { 
            EQS_SetIOCFocus(EQS_GetNthIOC(CB_PHONE_NUMBER1));
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_NOPHONENUMBER), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if (fSaveMessage) 
        {
            sSaveCharacbell();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_NO:
        MessageReason = SMM_NOCHANGE;
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    case SE_SOFTKEY_SAVE:
        SKY_StopMusic();

        memset (temp, 0x00, MAX_PHONENUMBER_LENGTH);
        EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER1), temp);
        if (*temp == NULL) 
        { 
            EQS_SetIOCFocus(EQS_GetNthIOC(CB_PHONE_NUMBER1));
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_NOPHONENUMBER), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else { 
            sSaveCharacbell();

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            fEndMessage = TRUE;
        }
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
} // sSKA_ChaBellChange

LOCAL void EQS_API sAddCharacbellInfoControls(void) 
{
    char    phone[MAX_NUM_OF_PHONENUMBER][MAX_TITLE_LENGTH];
    char    temp1[MAX_PHONENUMBER_LENGTH], temp2[MAX_PHONENUMBER_LENGTH], *loc;
    char    CBTitle[SKY_FILE_NAME_MAX_LENGTH+1];
    UINT8               pos, length, i, index, nNumber, nGroupIndex;
    HCONTROL     hControl;
    
    memset(temp1, 0x00, MAX_PHONENUMBER_LENGTH);
    memset(temp2, 0x00, MAX_PHONENUMBER_LENGTH);
    memset(phone, 0x00, MAX_NUM_OF_PHONENUMBER*MAX_TITLE_LENGTH);

    index = 0;
    for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
    {
        if (SKY_GET_CB_EMPTY(i) == 1) 
        {
            if (index == SelectedIndex) break;
            else index++;
        }
    }
    index = i;

    STRCPY(temp1, SKY_GET_CB_PHONE(index));
    STRCPY(temp2, temp1);
    pos = 0;
    for (i = 0, nNumber = 0 ; i < MAX_NUM_OF_PHONENUMBER; i++) 
    {
        loc = STRCHR(temp2, ',');
        length = loc - temp2;
        STRNCPY(phone[i], temp2, length);
        if (STRCMP(phone[i][0], NULL))
        {
            nNumber++;
        }

        pos += length;
        pos++;
        memset(temp2, 0x00, MAX_PHONENUMBER_LENGTH);
        STRCPY(temp2, temp1+pos);
    }

    // 캐릭벨 제목
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_CHARACBELL_TITLE);
    memset(CBTitle, 0x00, SKY_FILE_NAME_MAX_LENGTH+1);
    STRCPY(CBTitle, SKY_GET_CB_TITLE(index));
    EQC_TextEdit(EQS_NA, EQS_NA, SKY_FILE_NAME_MAX_LENGTH+1, 1, 
                     TEM_KOREAN, SKY_FILE_NAME_MAX_LENGTH+1,
                     CBTitle, 0);

    // 벨 종류들을 load
    sGetCBMelodyListInfo();
    sAddCBMelodyListControl();

    // 그림 종류들을 load
    sGetCBImageListInfo();
    sAddCBImageListControl();

    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_CHARACBELL_PHONENUMBER); // 상대방 전화번호 

    for(i = 0; i < MAX_NUM_OF_PHONENUMBER; i++)
    {
        EQC_StaticText(EQS_NA, EQS_NA, (WORD)(TKN_SMS_DONGBO_NULL01 + i));
        hControl = EQC_NumberEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE-2, 1, MAX_DONGBO_DIAL, phone[i]);
        if (i > nNumber) 
        {
            EQC_SET_FOCUS(hControl, FALSE);
        }
    }
        
    // 그룹 설정 추가
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_CHARACBELL_GROUPNAME);
    sMakeGroupList ();

    // Screen Formatting
    //EQS_AutoFormatting(CB_MAX, EHA_LEFT, EVA_CENTER);
    EQS_StartFormatter();
    
    for(i = 0; i < CB_MAX; i++)
    {
        if(i == CB_CHABELL_TITLEINPUT) 
        {
            EQS_SetControlPosition(EQS_GetNthIOC(i), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
            EQC_GET_X(EQS_GetNthIOC(i)) -= (R_SCROLLBAR_SIZE)/2;
        }
        else if((i >= CB_PHONE_NUMBER_TITLE1) && (i <= CB_PHONE_NUMBER6)) 
        {
            if (i == CB_PHONE_NUMBER_TITLE1)
                EQC_SetStaticText(EQS_GetNthIOC(i), (BYTE*)SF_BLACK_NUM1);  
            else if (i == CB_PHONE_NUMBER_TITLE2)
                EQC_SetStaticText(EQS_GetNthIOC(i), (BYTE*)SF_BLACK_NUM2);  
            else if (i == CB_PHONE_NUMBER_TITLE3)
                EQC_SetStaticText(EQS_GetNthIOC(i), (BYTE*)SF_BLACK_NUM3);  
            else if (i == CB_PHONE_NUMBER_TITLE4)
                EQC_SetStaticText(EQS_GetNthIOC(i), (BYTE*)SF_BLACK_NUM4);  
            else if (i == CB_PHONE_NUMBER_TITLE5)
                EQC_SetStaticText(EQS_GetNthIOC(i), (BYTE*)SF_BLACK_NUM5);  
            else if (i == CB_PHONE_NUMBER_TITLE6)
                EQC_SetStaticText(EQS_GetNthIOC(i), (BYTE*)SF_BLACK_NUM6);  
          
            EQS_SetControlPosition(EQS_GetNthIOC(i), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            i++;
            EQS_SetControlPosition(EQS_GetNthIOC(i), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQC_GET_X(EQS_GetNthIOC(i)) -= (R_SCROLLBAR_SIZE)/2;
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(i), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        }
        EQS_NewLine();
    }
    for(nGroupIndex = 0; nGroupIndex < g_nGroup; nGroupIndex++)
    {
        EQS_SetControlPosition(EQS_GetNthIOC(nGroupIndex+i), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();
    }
    EQS_EndFormatter();
} // sAddCharacbellInfoControls method

LOCAL void SKY_API sSKA_ChaBellSelected(SKY_EVENT_T *pEvent)
{
    static BOOL fEndMessage, fSaveMessage, fIdle;
    HCONTROL hControl = 0;
    static char    temp[30];
    BYTE    *szFile, *szMelodyFile;
    WORD    ImageType, MelodyType;
    HFILE   hFile;
    DWORD   dwTimeInfo = 0;
    UINT8   index, i;
    BYTE    s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6];
    static UINT8   nScrollData;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      NULL, 
                      SKY_GET_TOKEN(TKN_CHARACBELL_SELECTED), 
                      WA_SHOW_HEADER);

        l_pPictureList = GetBuiltInPictureList();

        nScrollData = s_nChaBellAlloc;
        fEndMessage = fSaveMessage = fIdle = FALSE;
        //index = SelectedIndex;

        index = 0;
        for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
        {
            if (SKY_GET_CB_EMPTY(i) == 1) 
            {
                if (index == SelectedIndex) break;
                else index++;
            }
        }
        index = i;

        // 사운드 플레이 
        MelodyType = SKY_GET_CB_MTYPE(index);
        szMelodyFile = SKY_GET_CB_MTITLE(index);
        if (MelodyType == SKY_MY_BELL_ID) 
        {
            SKY_PlayMusicFileEx(szMelodyFile, SKY_GET_BELL_VOLUME());
        }
        else 
        {
            SKY_PlayMusicEx(MelodyType, SKY_GET_BELL_VOLUME(), 0);
        }

        // 그림 보여주기
        memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));
        ImageType = SKY_GET_CB_ITYPE(index);
        szFile = SKY_GET_CB_ITITLE(index);

        if (ImageType == sMY_PICTURE) 
        { // 나만의 그림
            if (SKY_OpenDataFile(SDF_IMAGE, szFile, &hFile, SFT_READ) == SDR_OK) 
            {
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));
                if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
                {
                    SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                    SKY_CloseDataFile(hFile);
                    l_BmpBuf.Header.yStart = 0;
                    //hControl = EQC_Image(0, 0, &l_BmpBuf);
                    hControl = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
                }
                else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
                {
                    ANIMATION_HEADER_T AniHeader;
                    BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

                    SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                    SKY_CloseDataFile(hFile);
                    sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, szFile);
                    //hControl = EQC_Animation(0, 0, NULL, abBuf);
                    hControl = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);                        
                    //EQC_SetStartYAnimation(hControl, SKY_ANNUNCIATORS_HEIGHT);
                    //EQC_SetAnimationHeight(hControl, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
                    EQC_StartAnimation(hControl, TRUE);
                }
                else
                {
                    hControl = EQC_StaticText(0, 50, TKN_PICTURE_NOT_SUPPORT);
                }
                STRCPY(s_psFileName, szFile);
            }
            memset(temp, 0x00, 30);
            STRCPY(temp, EQC_GetScrollData(s_hListHandle, SelectedIndex));
            EQS_SetSWindowHeaderStr((BYTE*)temp);
        }
        else if (ImageType == sMY_PHOTO) 
        { // 나만의 사진
            if (SKY_OpenDataFile(SDF_PHOTO, szFile, &hFile, SFT_READ) == SDR_OK) 
            {
                SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
                SKY_CloseDataFile(hFile); 

                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
                //EQC_Image(0, 0, &l_BmpBuf);
                hControl = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
            memset(temp, 0x00, 30);
            STRCPY(temp, EQC_GetScrollData(s_hListHandle, SelectedIndex));
            EQS_SetSWindowHeaderStr((BYTE*)temp);
        }
        else 
        { // 기본제공 그림
            hControl = EQC_Animation(0, 0, g_pBuiltInAnimation[ImageType], NULL);

            //EQC_SetStartYAnimation(hControl, SKY_ANNUNCIATORS_HEIGHT);
            //EQC_SetAnimationHeight(hControl, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
            EQC_StartAnimation(hControl, TRUE);
            STRCPY(s_psFileName, szFile);
            // 이 부분은 바꿔주는 것이 좋을 듯 함.
            // 왜냐면 통일성 있게 가야쥐...  그치?
            //EQS_SetWindowAttr(WA_NONE);
            memset(temp, 0x00, 30);
            STRCPY(temp, EQC_GetScrollData(s_hListHandle, SelectedIndex));
            EQS_SetSWindowHeaderStr((BYTE*)temp);
        }
        break;

    case SE_APP_END :   // The very last event 
        SKY_StopMusic();
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

    case SE_KEY_END :   // The very last event
        SKY_StopMusic();
		if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
        break;
        /*EQS_End();
        break;*/
    
    case EQE_KEY_UP:
	case EQE_KEY_DOWN:
        if(pEvent->EventID == EQE_KEY_UP)
        {
            SelectedIndex --;
            if(SelectedIndex > MAX_NUM_OF_CHARACBELL)
                SelectedIndex = nScrollData -1;
        }
        else
        {
            SelectedIndex ++;
            if(SelectedIndex >= nScrollData)
                SelectedIndex = 0;
        }
        
        hControl = EQS_GetNthIOC(0);
        EQS_DeleteControl(hControl);
        /*if (hControl)
        {
            EQS_DeleteControl(hControl);
        }*/

        index = 0;
        for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
        {
            if (SKY_GET_CB_EMPTY(i) == 1) 
            {
                if (index == SelectedIndex) break;
                else index++;
            }
        }
        index = i;

        // 사운드 플레이 
        SKY_StopMusic();
        MelodyType = SKY_GET_CB_MTYPE(index);
        szMelodyFile = SKY_GET_CB_MTITLE(index);
        if (MelodyType == SKY_MY_BELL_ID) 
        {
            SKY_PlayMusicFileEx(szMelodyFile, SKY_GET_BELL_VOLUME());
        }
        else 
        {
            SKY_PlayMusicEx(MelodyType, SKY_GET_BELL_VOLUME(), 0);
        }

        // 그림 보여주기
        memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));
        ImageType = SKY_GET_CB_ITYPE(index);
        szFile = SKY_GET_CB_ITITLE(index);

        if (ImageType == sMY_PICTURE) 
        { // 나만의 그림
            if (SKY_OpenDataFile(SDF_IMAGE, szFile, &hFile, SFT_READ) == SDR_OK) 
            {
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));
                if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
                {
                    SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                    SKY_CloseDataFile(hFile);
                    l_BmpBuf.Header.yStart = 0;
                    //hControl = EQC_Image(0, 0, &l_BmpBuf);
                    hControl = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
                }
                else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
                {
                    ANIMATION_HEADER_T AniHeader;
                    BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

                    SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                    SKY_CloseDataFile(hFile);
                    sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, szFile);
                    //hControl = EQC_Animation(0, 0, NULL, abBuf);
                    hControl = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);                        
                    //EQC_SetStartYAnimation(hControl, SKY_ANNUNCIATORS_HEIGHT);
                    //EQC_SetAnimationHeight(hControl, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
                    EQC_StartAnimation(hControl, TRUE);
                }
                else
                {
                    hControl = EQC_StaticText(0, 50, TKN_PICTURE_NOT_SUPPORT);
                }
                STRCPY(s_psFileName, szFile);
            }
            memset(temp, 0x00, 30);
            STRCPY(temp, EQC_GetScrollData(s_hListHandle, SelectedIndex));
            EQS_SetSWindowHeaderStr((BYTE*)temp);
        }
        else if (ImageType == sMY_PHOTO) 
        { // 나만의 사진
            if (SKY_OpenDataFile(SDF_PHOTO, szFile, &hFile, SFT_READ) == SDR_OK) 
            {
                SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
                SKY_CloseDataFile(hFile); 

                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
                //EQC_Image(0, 0, &l_BmpBuf);
                hControl = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
            memset(temp, 0x00, 30);
            STRCPY(temp, EQC_GetScrollData(s_hListHandle, SelectedIndex));
            EQS_SetSWindowHeaderStr((BYTE*)temp);
        }
        else 
        { // 기본제공 그림
            hControl = EQC_Animation(0, 0, g_pBuiltInAnimation[ImageType], NULL);

            //EQC_SetStartYAnimation(hControl, SKY_ANNUNCIATORS_HEIGHT);
            //EQC_SetAnimationHeight(hControl, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
            EQC_StartAnimation(hControl, TRUE);
            STRCPY(s_psFileName, szFile);
            // 이 부분은 바꿔주는 것이 좋을 듯 함. (통일성을 위해서..)
            //EQS_SetWindowAttr(WA_NONE);
            memset(temp, 0x00, 30);
            STRCPY(temp, EQC_GetScrollData(s_hListHandle, SelectedIndex));
            EQS_SetSWindowHeaderStr((BYTE*)temp);
        }
        break;
   
    case SE_APP_SUSPEND: // child applet is started 
        break;
        
    case SE_APP_RESUME: // child applet is ended 
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
} // sSKA_ChaBellSelected

LOCAL void DeleteCharacbell(int index, BOOL sFromMenu) 
{
    UINT8   DeletedIndex, i;
    
    if (sFromMenu) {
        DeletedIndex = 0;
        for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
        {
            if (SKY_GET_CB_EMPTY(i) == 1) 
            {
                if (DeletedIndex == index) break;
                else DeletedIndex++;
            }
        }
        DeletedIndex = i;
    }
    else
    {
        DeletedIndex = index;
    }
    
    for(i=0; i< MAX_GROUP_NUMBER; i++) 
    {
        GetGroupInfo(&s_GroupInfo, i);
        if (STRCMP(s_GroupInfo.Bell.szFileName, SKY_GET_CB_TITLE(DeletedIndex)) == 0) 
        {
            if (s_GroupInfo.Bell.wBellID == SKY_CB_GROUPBELL) 
            {
                // 여기서 해당 그룹벨을 삭제하고 저장해줘야 한다.
                s_GroupInfo.Bell.wBellID = 0;
                STRCPY(s_GroupInfo.Bell.szFileName, "");
                SetGroupInfo(&s_GroupInfo, i);
            }
        }
    } // 그룹벨에서 사용되고 있는 경우

    //BeforeDelete(DeletedIndex);
    
    SKY_SET_CB_EMPTY(DeletedIndex, 0x00);
    SKY_SET_CB_TITLE(DeletedIndex, 0x00);
    SKY_SET_CB_MTYPE(DeletedIndex, 0x00);
    SKY_SET_CB_MTITLE(DeletedIndex, 0x00);
    SKY_SET_CB_ITYPE(DeletedIndex, 0x00);
    SKY_SET_CB_ITITLE(DeletedIndex, 0x00);
    SKY_SET_CB_PHONE(DeletedIndex, 0x00);
    SKY_SET_CB_ADDRESS(SKY_GET_CB_ADDRESS() - 1);
} // DeleteCharacbell

LOCAL void DeleteAllCharacbell (void) 
{
    UINT8   index;
    HFILE hFile;
    DWORD dwCount;

    //BeforeDeleteAll();
    
    for(index=0; index< MAX_GROUP_NUMBER; index++) 
    {
        GetGroupInfo(&s_GroupInfo, index);
        if (s_GroupInfo.Bell.wBellID == SKY_CB_GROUPBELL) 
        {
            // 여기서 해당 그룹벨을 삭제하고 저장해줘야 한다.
            s_GroupInfo.Bell.wBellID = 0;
            STRCPY(s_GroupInfo.Bell.szFileName, "");
            SetGroupInfo(&s_GroupInfo, index);
        }
    } // 그룹벨에서 사용되고 있는 경우

    // 캐릭벨 EFS정보를 모두 NULL로 바꿔준다. 그리고 파일을 써줘야 반영됨.
    memset(g_CharacbellConfig.CBLIST, 0x00, (sizeof(CHARACBELL_T) * MAX_NUM_OF_CHARACBELL));
    g_CharacbellConfig.Address = 0x00;
    
    hFile = SKY_OpenFile(FILE_NAME_CB_LIST, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

    dwCount = SKY_WriteFile(hFile, &(g_CharacbellConfig), sizeof(g_CharacbellConfig));
    SKY_ASSERT(dwCount == sizeof(g_CharacbellConfig));
    SKY_CloseFile(hFile);
    sRESET_CONFIG_DIRTY(SC_CHABELL);
} // DeleteAllCharacbell

LOCAL void SKY_API sSaveCharacbell(void) 
{
    UINT8       nCtrl, index, nCBIndex, length, i;
    HCONTROL    hControl;
    char        temp[MAX_PHONENUMBER_LENGTH], data[MAX_TITLE_LENGTH];

    // EFS구조 중에서 중간에 비어있는 곳 찾기 위해 사용
    index = 0;
    if (bCharacbellChange) 
    {
        for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
        {
            if (SKY_GET_CB_EMPTY(i) == 1) 
            {
                if (index == SelectedIndex) break;
                else index++;
            }
        }
    }
    else 
    {
        index = 0;
        for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
        {
            if (SKY_GET_CB_EMPTY(i) == 0) 
                break;
        }
    }

    index = i; 

    sResultValue = SMM_NOTDELETE;
    for(nCtrl = 0; nCtrl < CB_MAX; nCtrl++)
    {
        switch(nCtrl)
        {
        case CB_CHABELL_TITLEINPUT: // 캐릭벨 제목 저장
            memset(temp, 0x00, MAX_PHONENUMBER_LENGTH);
            EQC_GetTextEditData(EQS_GetNthIOC(CB_CHABELL_TITLEINPUT), temp);
            if (*temp == NULL) // 캐릭벨 제목을 입력하지 않은 경우
            {
                char tempTitle[MAX_TITLE_LENGTH];

                EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER1), temp);

                memset(tempTitle, 0x00, MAX_TITLE_LENGTH);
                if ((STRLEN(temp) > 0) && (temp[0] != '0'))
                {
                    if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                    {
                        STRCPY(tempTitle, "011");
                    }
                    else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                    {
                        STRCPY(tempTitle, "017");
                    }
                    STRCAT(tempTitle, temp);
                }
                else
                {
                    STRCPY(tempTitle, temp);
                }

                SKY_SET_CB_TITLE(index, tempTitle);
            }
            else
            {
                SKY_SET_CB_TITLE(index, temp);
            }
            break;

        case CB_BELL_ID: // 벨 저장
            nCBIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_BELL_ID));
              
            if(sIsMyBell())
            {
                SKY_SET_CB_MTYPE (index, SKY_MY_BELL_ID);
            }
            else
            {
                SKY_SET_CB_MTYPE (index, aBell_List[nCBIndex].wBellId);
            }
            SKY_SET_CB_MTITLE (index, ppBellList[nCBIndex]);
            break;

        case CB_IMAGE_ID: // 이미지 저장 
            nCBIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_IMAGE_ID));
             
            if(sIsMyPicture())
            {
                SKY_SET_CB_ITYPE (index, sMY_PICTURE);
                SKY_SET_CB_ITITLE (index, ppAniList[nCBIndex]);
            }
            else if(sIsPhoto())
            {
                SKY_SET_CB_ITYPE (index, sMY_PHOTO);
                SKY_SET_CB_ITITLE (index, ppMyPhotoList[nCBIndex - (NUM_ANI_DATA + s_nMyANI)]);
            }
            else
            {
                SKY_SET_CB_ITYPE (index, l_pPictureList[nCBIndex].wPictureID);
                SKY_SET_CB_ITITLE (index, ppAniList[nCBIndex]);
            }
            break;

        case CB_PHONE_NUMBER1:
            memset(temp, 0x00, MAX_PHONENUMBER_LENGTH);
            memset(data, 0x00, MAX_TITLE_LENGTH);
            EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER1), data);
            length = STRLEN(data);
            data[length] = ',';
            if (length !=0) 
            {
                if (data[0] != '0') 
                {
                    char tmp[MAX_TITLE_LENGTH];
                    memset(tmp, 0x00, MAX_TITLE_LENGTH);
                    if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                    {
                        STRCPY(tmp, "011");
                    }
                    else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                    {
                        STRCPY(tmp, "017");
                    }
                    STRCAT(tmp, data);
                    memset(data, 0x00, MAX_TITLE_LENGTH);
                    STRCPY(data, tmp);
                }
                CheckDuplicateNumber(data, index);
            }
            STRCPY(temp, data);
            break;

        case CB_PHONE_NUMBER2:
            memset(data, 0x00, MAX_TITLE_LENGTH);
            EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER2), data);
            length = STRLEN(data);
            data[length] = ',';
            if (length !=0)
            {
                if (data[0] != '0') 
                {
                    char tmp[MAX_TITLE_LENGTH];
                    memset(tmp, 0x00, MAX_TITLE_LENGTH);
                    if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                    {
                        STRCPY(tmp, "011");
                    }
                    else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                    {
                        STRCPY(tmp, "017");
                    }
                    STRCAT(tmp, data);
                    memset(data, 0x00, MAX_TITLE_LENGTH);
                    STRCPY(data, tmp);
                }
                CheckDuplicateNumber(data, index);
            }
            STRCPY(temp+STRLEN(temp), data);
            break;

        case CB_PHONE_NUMBER3:
            memset(data, 0x00, MAX_TITLE_LENGTH);
            EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER3), data);
            length = STRLEN(data);
            data[length] = ',';
            if (length !=0)
            {
                if (data[0] != '0') 
                {
                    char tmp[MAX_TITLE_LENGTH];
                    memset(tmp, 0x00, MAX_TITLE_LENGTH);
                    if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                    {
                        STRCPY(tmp, "011");
                    }
                    else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                    {
                        STRCPY(tmp, "017");
                    }
                    STRCAT(tmp, data);
                    memset(data, 0x00, MAX_TITLE_LENGTH);
                    STRCPY(data, tmp);
                }
                CheckDuplicateNumber(data, index);
            }
            STRCPY(temp+STRLEN(temp), data);
            break;

        case CB_PHONE_NUMBER4:
            memset(data, 0x00, MAX_TITLE_LENGTH);
            EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER4), data);
            length = STRLEN(data);
            data[length] = ',';
            if (length !=0)
            {
                if (data[0] != '0') 
                {
                    char tmp[MAX_TITLE_LENGTH];
                    memset(tmp, 0x00, MAX_TITLE_LENGTH);
                    if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                    {
                        STRCPY(tmp, "011");
                    }
                    else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                    {
                        STRCPY(tmp, "017");
                    }
                    STRCAT(tmp, data);
                    memset(data, 0x00, MAX_TITLE_LENGTH);
                    STRCPY(data, tmp);
                }
                CheckDuplicateNumber(data, index);
            }
            STRCPY(temp+STRLEN(temp), data);
            break;

        case CB_PHONE_NUMBER5:
            memset(data, 0x00, MAX_TITLE_LENGTH);
            EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER5), data);
            length = STRLEN(data);
            data[length] = ',';
            if (length !=0)
            {
                if (data[0] != '0') 
                {
                    char tmp[MAX_TITLE_LENGTH];
                    memset(tmp, 0x00, MAX_TITLE_LENGTH);
                    if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                    {
                        STRCPY(tmp, "011");
                    }
                    else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                    {
                        STRCPY(tmp, "017");
                    }
                    STRCAT(tmp, data);
                    memset(data, 0x00, MAX_TITLE_LENGTH);
                    STRCPY(data, tmp);
                }
                CheckDuplicateNumber(data, index);
            }
            STRCPY(temp+STRLEN(temp), data);
            break;

        case CB_PHONE_NUMBER6:
            memset(data, 0x00, MAX_TITLE_LENGTH);
            EQC_GetNumberEditValue(EQS_GetNthIOC(CB_PHONE_NUMBER6), data);
            length = STRLEN(data);
            data[length] = ',';
            if (length !=0)
            {
                if (data[0] != '0') 
                {
                    char tmp[MAX_TITLE_LENGTH];
                    memset(tmp, 0x00, MAX_TITLE_LENGTH);
                    if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                    {
                        STRCPY(tmp, "011");
                    }
                    else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                    {
                        STRCPY(tmp, "017");
                    }
                    STRCAT(tmp, data);
                    memset(data, 0x00, MAX_TITLE_LENGTH);
                    STRCPY(data, tmp);
                }
                CheckDuplicateNumber(data, index);
            }
            STRCPY(temp+STRLEN(temp), data);
            SKY_SET_CB_PHONE (index, temp);
            break;

        default:
            break;
        }
    }

    for(nCtrl = 0; nCtrl < g_nGroup; nCtrl++)
    {
        hControl = EQS_GetNthIOC(nCtrl+CB_MAX);
        GetGroupInfo(&s_GroupInfo, nCtrl);
        if (EQC_GetCheckBox(hControl, 0))
        { // 그룹이 선택되어 있으면...
            s_GroupInfo.Bell.wBellID = SKY_CB_GROUPBELL; 
            STRCPY(s_GroupInfo.Bell.szFileName, SKY_GET_CB_TITLE(index));
        }
        else if (SKY_IS_CB_GROUPBELL(s_GroupInfo.Bell.wBellID))
        { // 선택해제이나 기존에 캐릭벨이었던 경우
            if (STRCMP(s_GroupInfo.Bell.szFileName, SKY_GET_CB_TITLE(index)) == 0)
            {
                s_GroupInfo.Bell.wBellID = 0;
                STRCPY(s_GroupInfo.Bell.szFileName, "");
            }
        }
        SetGroupInfo(&s_GroupInfo, nCtrl);
    }

    SKY_SET_CB_EMPTY (index, 1); // 현재 레코드 사용중
    // 이렇게 index가 아닌 것은 중간에 캐릭벨이 삭제될 경우가 있어서 이렇게..
    if (!bCharacbellChange)
        SKY_SET_CB_ADDRESS(SKY_GET_CB_ADDRESS() + 1);  
} // sSaveCharacbell

void CheckDuplicateNumber (char *temp, int DeletedIndex) 
{
    // 연동되는 착신번호가 기존에 저장되어 있는 번호인지 확인한다.
    UINT index, pos;
    char phone[MAX_PHONENUMBER_LENGTH], replacedNum[MAX_PHONENUMBER_LENGTH];
    char *loc;

    if (SKY_GET_CB_ADDRESS() > 0) 
    { // 저장되어 있는게 있는 경우
        for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) 
        {
            if ((SKY_GET_CB_EMPTY(index) == 1) && (index != DeletedIndex)) 
            { // 거기에 값이 저장되어 있는 경우
                memset(phone, 0x00, MAX_PHONENUMBER_LENGTH);
                memset(replacedNum, 0x00, MAX_PHONENUMBER_LENGTH);

                STRCPY(phone, SKY_GET_CB_PHONE(index));
                loc = STRSTR(phone, temp);
                if (loc != NULL) 
                { // 일치하는 문자열을 찾았음
                    pos = loc - phone;
                    STRNCPY(replacedNum, phone, pos); // 앞까지 넣고
                    STRNCPY(replacedNum+pos, phone+ pos + STRLEN(temp), STRLEN(phone) - STRLEN(temp) - pos);  // 나머지 뒷부분 넣고.
                    if (!STRCMP(replacedNum, ",,,,,")) 
                    {
                        DeleteCharacbell(index, FALSE);
                        pos = 0;
                        sResultValue = SMM_CHABELLDELETE;
                        break;
                    }
                    else 
                    {
                        replacedNum[STRLEN(replacedNum)] = ',';
                        SKY_SET_CB_PHONE(index, replacedNum);
                        sResultValue = SMM_NUMBERDELETE;
                        break;
                    }
                }
            }
        }
    }
} // CheckDuplicateNumber

// 벨 이름들을 ppBellList에 가져오는 역할을 한다.
// 여기서는 벨이름 로딩 외엔 하는 일이 없당.
void EQS_API sGetCBMelodyListInfo(void)
{
    UINT16   index, i;

    s_nMyBellAlloc = (UINT8)SKY_GetFileNumberInDataFile(SDF_MELODY);

    if(s_nMyBellAlloc > 0) 
    {
        for(index=0; index < s_nMyBellAlloc; index ++)
            ppMyBellList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

        s_nMyBell = SKY_GetFileExtList(SKY_MELODY_DIR, ppMyBellList);
    }
    else 
        s_nMyBell = 0;
    
    // 여기서 벨종류를 로딩하는 과정을 거친다.
    s_nBellGroup = NUM_BELL_GROUP;

    for(index=0; index<s_nBellGroup; index++)
        ppBellGroup[index] = (BYTE *)SKY_GET_TOKEN(aBell_Group[index].GroupName);

    // 기본적으로 제공하는 벨 이름들을 로딩
    for(index=0; index<NUM_BELL_LIST; index++)
        ppBellList[index] = (BYTE *)SKY_GET_TOKEN(aBell_List[index].BellName);

    // 나만의 벨을 로딩
	for(i = index ; i < NUM_BELL_LIST + s_nMyBell; i++)
    {
		ppBellList[i] = ppMyBellList[i - index];
    }
} // sGetCBMelodyListInfo

void EQS_API sGetCBImageListInfo(void) {
    UINT8   index;
    HCONTROL hControl;

    ppTypeList[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_BASIC);
    ppTypeList[1] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_IDLE_MYANI);
    ppTypeList[2] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MY_PHOTO);
    //ppTypeList[4] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);

    // Init List
    for(index=0; index<NUM_ANI_DATA; index++)
        ppAniList[index] = (BYTE *)SKY_GET_TOKEN(l_pPictureList[index].PictureName);

    sGetCBMyAniList();

    for(index=NUM_ANI_DATA; index < NUM_ANI_DATA+s_nMyANI; index++)
        ppAniList[index] = ppMyANIList[index-NUM_ANI_DATA];

    sGetCBMyPhotoList(FALSE);

    for(index=NUM_ANI_DATA+s_nMyANI; index < NUM_ANI_DATA+s_nMyANI+s_nMyPhoto; index++)
        ppAniList[index] = ppMyPhotoName[index-NUM_ANI_DATA-s_nMyANI];
    
    //ppAniList[NUM_ANI_DATA+s_nMyANI+s_nMyPhoto+s_nCANI] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_CHARACBELL_IMAGETITLE);
} // sGetCBImageListInfo

void EQS_API sAddCBMelodyListControl(void)
{
    UINT16      index, nBellIndex = 0;
    //기존에 저장된 것을 로딩할때 필요한 부분.
    WORD        wBellID = 0;
    HCONTROL    hControl;
    UINT8       i, nIndex;

    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_TYPE);

    // 기존에 저장된 것을 로딩할때 필요한 부분.
    if (bCharacbellChange) 
    {
        for (i = 0, nIndex = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
        {
            if (SKY_GET_CB_EMPTY(i) == 1) 
            {
                if (nIndex == SelectedIndex) break;
                else nIndex++;
            }
        }
        wBellID = SKY_GET_CB_MTYPE(i);
        if (wBellID == SKY_MY_BELL_ID) // 나만의 벨
            nBellIndex = FindMyBellIndex(SKY_GET_CB_MTITLE(i)); 
        else // 기본제공 벨
            nBellIndex = FindBellIndex(wBellID);
    }
        
    hControl = EQC_ScrollList(EQS_NA, EQS_NA,BELL_SCROLL_LIST_CHAR, 
                              4, 
                              ppBellList, 
                              NUM_BELL_LIST + s_nMyBell, 
                              NUM_BELL_LIST + s_nMyBell, 
                              nBellIndex);

	    
    for(index=0; index < NUM_BELL_GROUP; index ++) 
    {
		wBellList[index] = aBell_Group[index].nMelodyNumber;
    }
    wBellList[s_nBellGroup-1] = s_nMyBell;

	EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
				            CS_SCROLL_LIST_COMBO |\
							CS_SCROLL_LIST_TAB );

    EQC_SetScrollTab(hControl,s_nBellGroup,3,FALSE,0);
    EQC_SetScrollTabData(hControl, wBellList, ppBellGroup);
} // sAddCBMelodyListControl

void EQS_API sAddCBImageListControl(void) 
{
    UINT16      l_nAniIndex = 0;
    WORD        wAniList[NUM_IMAGE_MODE-1];
    // 기존에 저장된 값을 로딩할때 필요한 부분.
    WORD        wImageID;
    HCONTROL    hControl;
    UINT8       i, index;

    wAniList[0] = NUM_ANI_DATA;
    wAniList[1] = s_nMyANI;
    //wAniList[4] = 1;

    // 기존에 저장된 값을 로딩할때 필요한 부분.
    if (bCharacbellChange) 
    {
        for (i = 0, index = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
        {
            if (SKY_GET_CB_EMPTY(i) == 1) 
            {
                if (index == SelectedIndex) break;
                else index++;
            }
        }
        wImageID = SKY_GET_CB_ITYPE(i);

        if (wImageID == sMY_PICTURE) // 나만의 그림
            l_nAniIndex = FindMyAniIndex(SKY_GET_CB_ITITLE(i));
        else if (wImageID == sMY_PHOTO) // 나만의 사진
            l_nAniIndex = FindMyPhotoIndex(SKY_GET_CB_ITITLE(i));
        else // 기본제공 그림
            l_nAniIndex = FindImageIndex(wImageID);
    }
    hControl = EQC_ScrollList(EQS_NA, EQS_NA,PIC_SCROLL_LIST_CHAR, 
                                4, 
                                ppAniList, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto,//+1, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto,//+1, 
                                l_nAniIndex);
    wAniList[2] = s_nMyPhoto;

	EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
				            CS_SCROLL_LIST_COMBO |\
							CS_SCROLL_LIST_TAB );

    EQC_SetScrollTab(hControl,NUM_IMAGE_MODE,3,FALSE,0);
    EQC_SetScrollTabData(hControl, wAniList, ppTypeList);
} // sAddCBImageListControl

void EQS_API sMakeGroupList(void) 
{
    int             nIndex, index, i;
    static HCONTROL hControl;
    BYTE            tempGroupName[SKY_PATH_NAME_MAX_LENGTH];
    // EFS구조 중에서 중간에 비어있는 곳 찾기 위해 사용
    index = 0;
    if (bCharacbellChange) 
    {
        for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
        {
            if (SKY_GET_CB_EMPTY(i) == 1) 
            {
                if (index == SelectedIndex) break;
                else index++;
            }
        }
        index = i;
    }

    for( nIndex=0; nIndex< g_nGroup; nIndex++)
    {
        ppSortedGroupName[0] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);
        STRCPY(tempGroupName, GetGroupName(nIndex));
        if (STRLEN(tempGroupName) > GROUP_CHECK_BOX_CHAR)
        {
            STRNCPY(ppSortedGroupName[0], tempGroupName, GROUP_CHECK_BOX_CHAR-2);
            STRNCAT(ppSortedGroupName[0], "...", 2);
        }
        else
        {
            STRCPY(ppSortedGroupName[0], GetGroupName(nIndex));
        }
        
        hControl = EQC_CheckBox(EQS_NA, EQS_NA, ppSortedGroupName, 1);
        EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) & ~CS_CHECK_BOX_BORDER));
        if (bCharacbellChange)
        {
            GetGroupInfo(&s_GroupInfo, nIndex);
            if (STRCMP(s_GroupInfo.Bell.szFileName, SKY_GET_CB_TITLE(index)) == 0) 
            {
                if (SKY_IS_CB_GROUPBELL(s_GroupInfo.Bell.wBellID))
                {
                    EQC_SetCheckBox(hControl, 0);
                }
            }
        }
    }
} // sMakeGroupList

LOCAL BOOL EQS_API sIsMyBell(void) 
{
    if((EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_BELL_ID))
        >= NUM_BELL_LIST) && 
    (EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_BELL_ID))
        < NUM_BELL_LIST + s_nMyBell))
        return TRUE;
    else
        return FALSE;
} // sIsMyBell

LOCAL BOOL EQS_API sIsMyPicture(void) 
{
    if((EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_IMAGE_ID))
        >= NUM_ANI_DATA) && 
    (EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_IMAGE_ID))
        < NUM_ANI_DATA + s_nMyANI))
        return TRUE;
    else
        return FALSE;
} // sIsMyPicture

LOCAL BOOL EQS_API sIsPhoto(void) 
{
    if((EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_IMAGE_ID))
        >= NUM_ANI_DATA + s_nMyANI) && 
    (EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_IMAGE_ID))
        < NUM_ANI_DATA + s_nMyANI + s_nMyPhoto))
        return TRUE;
    else
        return FALSE;
} // sIsPhoto

LOCAL void EQS_API CBPlayCurrentBell(void)
{
    UINT8   nBellIndex;
    WORD    nBellId;
    WORD    wVolume;

    wVolume = SKY_GET_BELL_VOLUME();
    if(sIsMyBell())
    {
        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_BELL_ID));
        SKY_PlayMusicFileEx(ppMyBellList[nBellIndex-NUM_BELL_LIST], wVolume);
    }
    else
    {
        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(CB_BELL_ID));
        nBellId = aBell_List[nBellIndex].wBellId;
        SKY_PlayMusicEx(nBellId, wVolume, 0);
    }
} // CBPlayCurrentBell

LOCAL UINT8 EQS_API sGetCBMyAniList(void)
{
    UINT8   index;

    // Load user file list
    s_nMyANIAlloc = (UINT8)SKY_GetFileNumberInDataFile(SDF_IMAGE);

    if(!s_nMyANIAlloc)
    {
        ppMyANIList[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NO_MYANI);
        s_nMyANI = 0;
    }
    else
    {
        for(index=0; index < s_nMyANIAlloc; index ++)
            ppMyANIList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

        s_nMyANI = SKY_GetFileExtList(SKY_IMAGE_DIR, ppMyANIList);
    }

    return s_nMyANI;
} // sGetCBMyAniList

LOCAL UINT8 EQS_API sGetCBMyPhotoList(BOOL fIdle)
{
    UINT8   i,j;
    HFILE   hFile;
    PHOTO_LIST_T tmpPhotoList[MAX_PHOTO_COUNT];
    
    // Load user file list
    if(!SKY_IsFileOrDirExist(FILE_NAME_CAMERA))
    {   
        // Create file
        hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_CREATE);

        memset(tmpPhotoList, NULL, sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
        
        SKY_WriteFile(hFile, (CONST void *)tmpPhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                
        SKY_CloseFile(hFile);

        s_nMyPhoto = 0;
        ppMyPhotoName[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NO_MYPHOTO);
       
    }
    else
    {
        
        
        hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READONLY);

        SKY_ReadFile(hFile, (void *)tmpPhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);

		SKY_CloseFile(hFile);
            
        s_nMyPhoto = 0;

        if(fIdle)
        {
            ppMyPhotoName[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MY_PHOTO_SLIDE);
        }

        for(i = 0,j = 0; i < MAX_PHOTO_COUNT; i++)
        {
            if(tmpPhotoList[i].PhotoList != NULL)
            {  
                if(fIdle)
                {
                    ppMyPhotoName[j+1] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);
                    memcpy(ppMyPhotoName[j+1],tmpPhotoList[i].PhotoName,SKY_FILE_EXT_NAME_MAX_LENGTH+1);
                }
                else
                {
                    ppMyPhotoName[j] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);
                    memcpy(ppMyPhotoName[j],tmpPhotoList[i].PhotoName,SKY_FILE_EXT_NAME_MAX_LENGTH+1);
                }

                ppMyPhotoList[j] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);
                sprintf((BYTE *)ppMyPhotoList[j],"%lu",tmpPhotoList[i].PhotoList);

                s_nMyPhoto++;
                j++;
            }
        }

        
        if(!s_nMyPhoto)
        {
            ppMyPhotoName[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NO_MYPHOTO);
        }
        
    }
 
    return s_nMyPhoto;
} // sGetCBMyPhotoList

//기존에 저장된 값을 로딩할때 필요한 부분.
LOCAL UINT8 FindBellIndex(WORD wBellId) 
{
    UINT8   index;

    for(index=0; index < NUM_BELL_LIST; index ++)
    {
        if(aBell_List[index].wBellId == wBellId)
            return index;
    }
    return 0;
} // FindBellIndex

LOCAL UINT8 FindMyBellIndex(BYTE *pszBell) 
{
    UINT8   index;

    for(index=0; index < s_nMyBell; index ++)
    {
        if(!STRCMP(ppMyBellList[index], pszBell))
            return (index + NUM_BELL_LIST);
    }
    return 0;
} // FindMyBellIndex

LOCAL UINT8 FindMyAniIndex(BYTE *pszANI) 
{
    UINT8   index;

    for(index=0; index < s_nMyANI; index ++)
    {
        if(!STRCMP(ppMyANIList[index], pszANI))
            return (index + NUM_ANI_DATA);
    }
    return 0;
} // FindMyAniIndex

LOCAL UINT8 FindMyPhotoIndex(BYTE *pszANI) 
{
    UINT8   index;

    for(index=0; index < s_nMyPhoto; index ++)
    {
        if(!STRCMP(ppMyPhotoList[index], pszANI))
            return (index + NUM_ANI_DATA + s_nMyANI);
    }

    return 0;
} // FindMyPhotoIndex

LOCAL UINT8 FindImageIndex(WORD wImageId) 
{
    UINT8   index;

    for(index = 0; index < NUM_ANI_DATA; index++)
    {
        if(l_pPictureList[index].wPictureID == wImageId)
            return index;
    }
    return 0;
} // FindImageIndex