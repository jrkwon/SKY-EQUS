/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_pic.c
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
** 2000-12-29 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

LOCAL ROM PICTURE_DATA_TYPE_T l_a011PictureList[NUM_ANI_DATA] = {
	{TKN_PIC_WATER_SKY,	    BII_WATER_SKY},
	{TKN_PIC_ORIG_CALL,     BII_RINGING},
    {TKN_PIC_GOODBYE, 		BII_GOODBYE},
    {TKN_PIC_WAP,	    	BII_WAP},
    {TKN_PIC_SPEED011,    	BII_SERVICE_PROVIDER}
};

LOCAL ROM PICTURE_DATA_TYPE_T l_a017PictureList[NUM_ANI_DATA] = {
	{TKN_PIC_WATER_SKY,	    BII_WATER_SKY},
	{TKN_PIC_ORIG_CALL,     BII_RINGING},
    {TKN_PIC_GOODBYE, 		BII_GOODBYE},
    {TKN_PIC_WAP,	    	BII_WAP},
    {TKN_PIC_SKY_LOGO,    	BII_SERVICE_PROVIDER}
};

LOCAL PICTURE_DATA_TYPE_T *l_pPictureList = (PICTURE_DATA_TYPE_T*)l_a011PictureList;

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#ifdef FEATURE_CHARACBELL
#define NUM_ANI_GROUP   5
#else
#define NUM_ANI_GROUP   4
#endif // FEATURE_CHARACBELL

#define PIC_SCROLL_LIST_CHAR   16
/* rosa 2001-09-20 --> @why, what */
#define NUM_WATCH_POS_TYPE  5
/* <-- */

enum tag_SKA_PICTURE_E {
    SP_CALL_TITLE,
    SP_CALL_LIST,
    SP_STARTUP_TITLE,
    SP_STARTUP_LIST,
    SP_POWEROFF_TITLE,
    SP_POWEROFF_LIST,
    SP_NTOP_TITLE,
    SP_NTOP_LIST,
    SP_MAX
};
typedef enum tag_SKA_PICTURE_E SKA_PICTURE_E;

enum tag_SKA_IDLE_PICTURE_E {
    SIP_ANI_TITLE,
    SIP_ANI_LIST,          // Scroll List
    SIP_DISPLAY_WATCH_TITLE,    
    SIP_DISPLAY_WATCH,     // Scroll List
    SIP_NAME_TITLE,
    SIP_NAME,           // Text Editor
    SIP_DISPLAY_NAME,   // Check Box
    SIP_MAX
};


#define SP_SCROLL_LIST_STYLE    ((CS_SCROLL_LIST_DEFAULT \
                                | CS_SCROLL_LIST_COMBO \
                                | CS_SCROLL_LIST_ENUMBERATED) \
                                & ~CS_SCROLL_LIST_SCROLL_BAR)

#define SP_SCROLL_LIST_SIZE    3

#define SP_GET_SLSIZE(n)       (UINT8)((SP_SCROLL_LIST_SIZE < n)?SP_SCROLL_LIST_SIZE:n)

#define sSET_PICTURE_LIST()     \
                                if(SKY_GET_SERVICE_PROVIDER() == SP_STI) \
                                { \
                                    l_pPictureList = (PICTURE_DATA_TYPE_T*)l_a017PictureList; \
                                } \
                                else \
                                { \
                                    l_pPictureList = (PICTURE_DATA_TYPE_T*)l_a011PictureList; \
                                }

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
#ifdef FEATURE_CHARACBELL
LOCAL BYTE *ppCANIList[SKY_MAX_NUM_FILES];
LOCAL UINT8 s_nCANI, s_nCANIAlloc, l_nCAni;
LOCAL BYTE *ppAniList[NUM_ANI_DATA + (3*SKY_MAX_NUM_FILES) + 2];  
#else
LOCAL BYTE *ppAniList[NUM_ANI_DATA + (2*SKY_MAX_NUM_FILES) + 2];  
#endif // FEATURE_CHARACBELL

LOCAL BYTE *ppTypeList[NUM_ANI_GROUP];  
LOCAL BYTE *ppMyANIList[SKY_MAX_NUM_FILES];

LOCAL BYTE *ppMyPhotoList[SKY_MAX_NUM_FILES];
LOCAL BYTE *ppMyPhotoName[SKY_MAX_NUM_FILES+1];

LOCAL BYTE *ppNoAniList[1];

LOCAL UINT8 s_nMyANI, s_nMyANIAlloc;
LOCAL UINT8 s_nMyPhoto;

/* rosa 2001-09-20 --> @why, what */
LOCAL BYTE *ppWatchType[NUM_WATCH_POS_TYPE];
/* <-- */

LOCAL UINT8 l_nAni, l_nMyAni, l_nWatch;
LOCAL BOOL  l_fDisplayName;
LOCAL BYTE  l_szName[MAX_BANNER_LENGTH+1];

LOCAL BOOL  l_fIdleScreenPreview;
/* rosa 2001-09-20 --> @why, what */
//LOCAL BOOL l_fDisplayWatch;
LOCAL IDLE_DISPLAY_WATCH_E l_Watch;
/* <-- */
LOCAL IDLE_SCREEN_E l_Screen;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL UINT8 EQS_API sFindIndex(WORD wPictureID);
LOCAL void EQS_API sAddPictureControls(void);
LOCAL void EQS_API sSavePicture(void);
LOCAL void SKY_API sAddScrollList(PICTURE_TYPE_T Picture, BOOL fIdle);
LOCAL void SKY_API sAddWapScrollList(PICTURE_TYPE_T Picture);

LOCAL UINT8 EQS_API sGetMyAniList(void);
LOCAL UINT8 EQS_API sGetMyPhotoList(BOOL fIdle);

LOCAL UINT8 EQS_API sFindMyANIIndex(BYTE *pszANI);
LOCAL UINT8 EQS_API sFindMyPhotoIndex(BYTE *pszANI);

LOCAL void SKY_API sAddIdleDisplayControls(void);
LOCAL void SKY_API sSaveIdleDisplay(void);

LOCAL void SKY_API sSKA_IdleDisplayPreview(SKY_EVENT_T *pEvent);
/* rosa 2001-10-22 --> @why, what */
LOCAL void SKY_API sSKA_PictureDisplayPreview(SKY_EVENT_T *pEvent);
/* <-- */

#ifdef FEATURE_CHARACBELL
LOCAL UINT8 EQS_API sGetCAniList(void);
LOCAL UINT8 EQS_API sFindCANIIndex(BYTE *pszANI);
#endif // FEATURE_CHARACBELL

PICTURE_DATA_TYPE_T * SKY_API GetBuiltInPictureList(void)
{
    return l_pPictureList;
}

void SKY_API SKA_MainPictureSet(SKY_EVENT_T *pEvent)
{
    UINT8 index, i;
    PICTURE_TYPE_T tmpPicture;
    UINT16  xPos = 0;
    UINT16 yPos = 0;
    static HCONTROL hControl;
    static BOOL fEndMessage, fSaveMessage, fReActive, fIdleMessage;
    /* rosa 2001-10-22 --> @why, what */
    static HCONTROL hActiveControl;
    /* <-- */

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_BEFOREVIEW_SELECT_SAVE, //CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_PICTURE_SET), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        sSET_PICTURE_LIST();

        // Add Controls

        fEndMessage = fSaveMessage = fIdleMessage = FALSE;

/*        if(SKY_GetFileNumberInDataFile(SDF_PHOTO) > 0)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAITING_A_MOMENT), 
                            0, MBT_NONE, MBT_HELP);
            fLoadMessage = TRUE;
        }
        else*/
        {
            sAddPictureControls();
            //fLoadMessage = FALSE;
        }
        break;

    case SE_APP_RESUME: /* child applet is ended */
        if(hActiveControl != INVALID_CONTROL_HANDLE)
            EQS_SetIOCActive(hActiveControl);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(index=0; index < s_nMyANIAlloc; index ++)
            SKY_Free(ppMyANIList[index]);


        for(index=0; index < s_nMyPhoto; index ++)
        {
            SKY_Free(ppMyPhotoList[index]);
            SKY_Free(ppMyPhotoName[index]);
        }

#ifdef FEATURE_CHARACBELL
        for(index=0; index < s_nCANIAlloc; index ++)
            SKY_Free(ppCANIList[index]);
#endif // FEATURE_CHARACBELL
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdleMessage = TRUE;
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;

        /* rosa 2001-10-22 --> @why, what */
    case SE_SOFTKEY_BEFOREVIEW:
        hActiveControl = EQS_GetFocusedIOC();
      
        for(i=0; i<SP_MAX;i++)
        {
            if(EQS_GetNthIOC(i)== hActiveControl)
            {
                l_nMyAni = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(i));
                break;
            }
        }

        if(!EQS_IsActive(hActiveControl))
            hActiveControl = INVALID_CONTROL_HANDLE;

        // display
        EQS_START_CHILD_EVENT(sSKA_PictureDisplayPreview, SE_APP_START, (WORD)i, EQS_NA);
        break;
        /* <-- */

    case SE_SOFTKEY_SAVE:
        sSavePicture();

        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

        fEndMessage = TRUE;
        break;

    case SE_SOFTKEY_INIT:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_CONFIRM), 
                        MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES:
        if(fSaveMessage)
        {
            sSavePicture();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        else
        {
            // 초기화
            tmpPicture.wPictureID = INIT_INCOMING_PICTURE;
            STRCPY(tmpPicture.szFileName, (BYTE*)"");
            SKY_SET_INCOMING_PICTURE(tmpPicture);

            tmpPicture.wPictureID = INIT_POWER_ON_PICTURE;
            STRCPY(tmpPicture.szFileName, (BYTE*)"");
            SKY_SET_POWER_ON_PICTURE(tmpPicture);

            tmpPicture.wPictureID = INIT_POWER_OFF_PICTURE;
            STRCPY(tmpPicture.szFileName, (BYTE*)"");
            SKY_SET_POWER_OFF_PICTURE(tmpPicture);

            tmpPicture.wPictureID = INIT_WAP_PICTURE;
            STRCPY(tmpPicture.szFileName, (BYTE*)"");
            SKY_SET_WAP_PICTURE(tmpPicture);

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_PROCESS), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdleMessage)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    case SE_IO_MESGBOX_END:
        if(fIdleMessage)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_END();
        else if(fReActive)
            EQS_SetIOCActive(hControl);
        
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sGetPicture(UINT16 index, PICTURE_TYPE_T *tmpPicture, BOOL fIdle)
{
#ifdef FEATURE_CHARACBELL
    if(index == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI && !fIdle)
    {
        tmpPicture->wPictureID = sPICTURE_NOT_USED;
    }
    else if(index == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI && fIdle)
    {
        tmpPicture->wPictureID = sPICTURE_NOT_USED;
    }
    else if(index >= NUM_ANI_DATA + s_nMyANI+s_nMyPhoto) {
        tmpPicture->wPictureID = sC_PICTURE;
        STRCPY(tmpPicture->szFileName, 
            ppCANIList[index - (NUM_ANI_DATA + s_nMyANI + s_nMyPhoto)]);
    } // 캐릭벨
    else if(index >= NUM_ANI_DATA + s_nMyANI)
    {
        if(fIdle)
        {
            if(index == NUM_ANI_DATA + s_nMyANI)
            {
                tmpPicture->wPictureID = sMY_PHOTO_ALL;
            }
            else
            {
                tmpPicture->wPictureID = sMY_PHOTO;
                STRCPY(tmpPicture->szFileName, 
                    ppMyPhotoList[index - (NUM_ANI_DATA + s_nMyANI +1)]);
            }
        }
        else
        {
            tmpPicture->wPictureID = sMY_PHOTO;
            STRCPY(tmpPicture->szFileName, 
                ppMyPhotoList[index - (NUM_ANI_DATA + s_nMyANI)]);
        }

    }
    else if(index >= NUM_ANI_DATA)
    {
        tmpPicture->wPictureID = sMY_PICTURE;
        STRCPY(tmpPicture->szFileName, 
            ppMyANIList[index - NUM_ANI_DATA]);
    }
    else
    {
        tmpPicture->wPictureID = 
            l_pPictureList[index].wPictureID;
    }
#else
    if(index == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto && !fIdle)
    {
        tmpPicture->wPictureID = sPICTURE_NOT_USED;
    }
    else if(index == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto && fIdle)
    {
        tmpPicture->wPictureID = sPICTURE_NOT_USED;
    }
    else if(index >= NUM_ANI_DATA + s_nMyANI)
    {
        if(fIdle)
        {
            if(index == NUM_ANI_DATA + s_nMyANI)
            {
                tmpPicture->wPictureID = sMY_PHOTO_ALL;
            }
            else
            {
                tmpPicture->wPictureID = sMY_PHOTO;
                STRCPY(tmpPicture->szFileName, 
                    ppMyPhotoList[index - (NUM_ANI_DATA + s_nMyANI +1)]);
            }
        }
        else
        {
            tmpPicture->wPictureID = sMY_PHOTO;
            STRCPY(tmpPicture->szFileName, 
                ppMyPhotoList[index - (NUM_ANI_DATA + s_nMyANI)]);
        }

    }
    else if(index >= NUM_ANI_DATA)
    {
        tmpPicture->wPictureID = sMY_PICTURE;
        STRCPY(tmpPicture->szFileName, 
            ppMyANIList[index - NUM_ANI_DATA]);
    }
    else
    {
        tmpPicture->wPictureID = 
            l_pPictureList[index].wPictureID;
    }
#endif // FEATURE_CHARACBELL
}

void EQS_API sSavePicture(void)
{
    UINT16 index;
    PICTURE_TYPE_T tmpPicture;

    index = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SP_CALL_LIST));

    sGetPicture(index, &tmpPicture, FALSE);
    SKY_SET_INCOMING_PICTURE(tmpPicture);

    // 시작시
    index = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SP_STARTUP_LIST));
    sGetPicture(index, &tmpPicture, FALSE);
    SKY_SET_POWER_ON_PICTURE(tmpPicture);

    // 종료시
    index = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SP_POWEROFF_LIST));
    sGetPicture(index, &tmpPicture, FALSE);
    SKY_SET_POWER_OFF_PICTURE(tmpPicture);

    // N.TOP 시작시
    index = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SP_NTOP_LIST));
    sGetPicture(index, &tmpPicture, FALSE);
    SKY_SET_WAP_PICTURE(tmpPicture);
}

void EQS_API sAddPictureControls(void)
{
    UINT8   index;

    ppTypeList[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_BASIC);
    ppTypeList[1] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_IDLE_MYANI);
    ppTypeList[2] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MY_PHOTO);
#ifdef FEATURE_CHARACBELL
    ppTypeList[3] = (BYTE*)SKY_GET_TOKEN(TKN_BELL_GROUP_CHARAC_BELL);
    ppTypeList[4] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);
#else
    ppTypeList[3] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);
#endif // FEATURE_CHARACBELL

    ppNoAniList[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);

    // Init List
    for(index=0; index<NUM_ANI_DATA; index++)
        ppAniList[index] = (BYTE *)SKY_GET_TOKEN(l_pPictureList[index].PictureName);

    sGetMyAniList();

    for(index=NUM_ANI_DATA; index < NUM_ANI_DATA+s_nMyANI; index++)
        ppAniList[index] = ppMyANIList[index-NUM_ANI_DATA];

    sGetMyPhotoList(FALSE);

    for(index=NUM_ANI_DATA+s_nMyANI; index < NUM_ANI_DATA+s_nMyANI+s_nMyPhoto; index++)
        ppAniList[index] = ppMyPhotoName[index-NUM_ANI_DATA-s_nMyANI];
    
#ifdef FEATURE_CHARACBELL
    sGetCAniList();

    for(index=NUM_ANI_DATA+s_nMyANI+s_nMyPhoto; index < NUM_ANI_DATA+s_nMyANI+s_nMyPhoto+s_nCANI; index++)
        ppAniList[index] = ppCANIList[index-NUM_ANI_DATA-s_nMyANI-s_nMyPhoto];
    ppAniList[NUM_ANI_DATA+s_nMyANI+s_nMyPhoto+s_nCANI] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);
#else
    ppAniList[NUM_ANI_DATA+s_nMyANI+s_nMyPhoto] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);
#endif // FEATURE_CHARACBELL
    
    // 전화올때
    EQC_StaticText(EQS_NA, EQS_NA, TKN_PICTURE_CALL);
    sAddScrollList(SKY_GET_INCOMING_PICTURE(), FALSE);

    // 전원을 켤때
    EQC_StaticText(EQS_NA, EQS_NA, TKN_PICTURE_STARTUP);
    sAddScrollList(SKY_GET_POWER_ON_PICTURE(), FALSE);

    // 전원을 끌때
    EQC_StaticText(EQS_NA, EQS_NA, TKN_PICTURE_POWEROFF);
    sAddScrollList(SKY_GET_POWER_OFF_PICTURE(), FALSE);

    // N.TOP시작시
    EQC_StaticText(EQS_NA, EQS_NA, TKN_PICTURE_WAP);
    sAddWapScrollList(SKY_GET_WAP_PICTURE());

    // Screen Formatting
    EQS_AutoFormatting(SP_MAX, EHA_LEFT, EVA_CENTER);
    EQS_SetNthIOCFocus(SP_CALL_LIST);
}

LOCAL void SKY_API sAddScrollList(PICTURE_TYPE_T Picture, BOOL fIdle)
{
    UINT16  l_nAniIndex;
    WORD    wAniList[NUM_ANI_GROUP];
    HCONTROL    hControl;

    if(IS_MY_PICTURE(Picture.wPictureID))
    {
        l_nAniIndex = sFindMyANIIndex(Picture.szFileName) + NUM_ANI_DATA;
    }
    else if(IS_MY_PHOTO_ALL(Picture.wPictureID))
    {
        l_nAniIndex = NUM_ANI_DATA + s_nMyANI;
    }
    else if(IS_MY_PHOTO(Picture.wPictureID))
    {
        if(fIdle)
            l_nAniIndex = sFindMyPhotoIndex(Picture.szFileName) + NUM_ANI_DATA + s_nMyANI + 1;
        else
            l_nAniIndex = sFindMyPhotoIndex(Picture.szFileName) + NUM_ANI_DATA + s_nMyANI;
    }
#ifdef FEATURE_CHARACBELL
    else if(IS_CHARAC_PICTURE(Picture.wPictureID))
    {
        l_nAniIndex = sFindCANIIndex(Picture.szFileName) + NUM_ANI_DATA + s_nMyANI + s_nMyPhoto;
    }
    else if(IS_PICTURE_NOT_USED(Picture.wPictureID))
    {
        if(fIdle)
            l_nAniIndex = NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI;
        else
            l_nAniIndex = NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI;
    }
#else
    else if(IS_PICTURE_NOT_USED(Picture.wPictureID))
    {
        if(fIdle)
            l_nAniIndex = NUM_ANI_DATA + s_nMyANI+s_nMyPhoto;
        else
            l_nAniIndex = NUM_ANI_DATA + s_nMyANI+s_nMyPhoto;
    }
#endif // FEATURE_CHARACBELL
    else
    {
        l_nAniIndex = sFindIndex(Picture.wPictureID);
    }

    wAniList[0] = NUM_ANI_DATA;
    wAniList[1] = s_nMyANI;
#ifdef FEATURE_CHARACBELL
    wAniList[3] = s_nCANI;
    wAniList[4] = 1;

	hControl = EQC_ScrollList(EQS_NA, EQS_NA,PIC_SCROLL_LIST_CHAR, 
                                4, 
                                ppAniList, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI+1, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI+1, 
                                l_nAniIndex);
    wAniList[2] = s_nMyPhoto;
    //EQC_SetScrollTab(hControl,NUM_ANI_GROUP,4,FALSE,0);
#else
    wAniList[3] = 1;

	hControl = EQC_ScrollList(EQS_NA, EQS_NA,PIC_SCROLL_LIST_CHAR, 
                                4, 
                                ppAniList, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+1, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+1, 
                                l_nAniIndex);
    wAniList[2] = s_nMyPhoto;
#endif // FEATURE_CHARACBELL
    EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
				            CS_SCROLL_LIST_COMBO |\
							CS_SCROLL_LIST_TAB );

    EQC_SetScrollTab(hControl,NUM_ANI_GROUP,3,FALSE,0);
    EQC_SetScrollTabData(hControl, wAniList, ppTypeList);
}

LOCAL void SKY_API sAddWapScrollList(PICTURE_TYPE_T Picture)
{
    UINT16  l_nAniIndex;
    WORD    wAniList[NUM_ANI_GROUP-1];
    HCONTROL    hControl;

    if(IS_MY_PICTURE(Picture.wPictureID))
        l_nAniIndex = sFindMyANIIndex(Picture.szFileName) + NUM_ANI_DATA;
    else if(IS_MY_PHOTO(Picture.wPictureID))
        l_nAniIndex = sFindMyPhotoIndex(Picture.szFileName) + NUM_ANI_DATA + s_nMyANI;
#ifdef FEATURE_CHARACBELL
    else if(IS_CHARAC_PICTURE(Picture.wPictureID))
    {
        l_nAniIndex = sFindCANIIndex(Picture.szFileName) + NUM_ANI_DATA + s_nMyANI + s_nMyPhoto;
    }
    else
        l_nAniIndex = sFindIndex(Picture.wPictureID);
    
    hControl = EQC_ScrollList(EQS_NA, EQS_NA,PIC_SCROLL_LIST_CHAR, 
                                4, 
                                ppAniList, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI,
                                l_nAniIndex);
#else
    else
        l_nAniIndex = sFindIndex(Picture.wPictureID);

    hControl = EQC_ScrollList(EQS_NA, EQS_NA,PIC_SCROLL_LIST_CHAR, 
                                4, 
                                ppAniList, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto, 
                                NUM_ANI_DATA + s_nMyANI+s_nMyPhoto, 
                                l_nAniIndex);
#endif // FEATURE_CHARACBELL    

    wAniList[0] = NUM_ANI_DATA;
    wAniList[1] = s_nMyANI;
    wAniList[2] = s_nMyPhoto;
#ifdef FEATURE_CHARACBELL
    wAniList[3] = s_nCANI;
#endif // FEATURE_CHARACBELL

	EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
				            CS_SCROLL_LIST_COMBO |\
							CS_SCROLL_LIST_TAB );

    EQC_SetScrollTab(hControl,NUM_ANI_GROUP-1,3,FALSE,0);
    EQC_SetScrollTabData(hControl, wAniList, ppTypeList);
}

LOCAL UINT8 EQS_API sGetMyAniList(void)
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
}

#ifdef FEATURE_CHARACBELL
LOCAL UINT8 EQS_API sGetCAniList(void)
{
    UINT8   index;

    // Load user file list
    s_nCANIAlloc = (UINT8)SKY_GetFileNumberInDataFile(SDF_CHA_IMAGE);

    if(!s_nCANIAlloc)
    {
        ppCANIList[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NO_MYANI);
        s_nCANI = 0;
    }
    else
    {
        for(index=0; index < s_nCANIAlloc; index ++)
            ppCANIList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

        s_nCANI = SKY_GetFileExtList(SKY_CHA_IMAGE_DIR, ppCANIList);
    }

    return s_nCANI;
}

UINT8 EQS_API sFindCANIIndex(BYTE *pszANI)
{
    UINT8   index;

    for(index=0; index < s_nCANI; index ++)
    {
        if(!STRCMP(ppCANIList[index], pszANI))
            return index;
    }

    return 0;
}
#endif // FEATURE_CHARACBELL

LOCAL UINT8 EQS_API sGetMyPhotoList(BOOL fIdle)
{
    UINT8   index,i,j;
    HFILE   hFile,nFiles;
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
                sprintf((char *)ppMyPhotoList[j],"%lu",tmpPhotoList[i].PhotoList);

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
}

UINT8 EQS_API sFindIndex(WORD wPictureID)
{
    UINT8   nIndex;

    for(nIndex = 0; nIndex < NUM_ANI_DATA; nIndex ++)
    {
        if(l_pPictureList[nIndex].wPictureID == wPictureID)
            return nIndex;
    }

    return 0;
}

UINT8 EQS_API sFindMyANIIndex(BYTE *pszANI)
{
    UINT8   index;

    for(index=0; index < s_nMyANI; index ++)
    {
        if(!STRCMP(ppMyANIList[index], pszANI))
            return index;
    }

    return 0;
}

UINT8 EQS_API sFindMyPhotoIndex(BYTE *pszANI)
{
    UINT8   index;

    for(index=0; index < s_nMyPhoto; index ++)
    {
        if(!STRCMP(ppMyPhotoList[index], pszANI))
            return index;
    }

    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_IdleDisplaySet()
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
void SKY_API SKA_MainIdleDisplaySet(SKY_EVENT_T *pEvent)
{
    PICTURE_TYPE_T tmpPicture;
    UINT8 index;
    UINT16  xPos, yPos;
    static BOOL fEndMessage, fSaveMessage, fReActive, fIdleMessage;
    static HCONTROL hControl;
    static HCONTROL hActiveControl;

	static UINT8    nPictureIndex;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_BEFOREVIEW_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_PICTURE_IDLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        sSET_PICTURE_LIST();

        // Add Controls

        fEndMessage = fSaveMessage = fIdleMessage = FALSE;

/*        if(SKY_GetFileNumberInDataFile(SDF_PHOTO) > 0)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WAITING_A_MOMENT), 
                            0, MBT_NONE, MBT_HELP);
            fLoadMessage = TRUE;
        }
        else*/
        {
            sAddIdleDisplayControls();
			nPictureIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIP_ANI_LIST));
//            fLoadMessage = FALSE;
        }
        break;
        
    case SE_APP_END :   /* The very last event */ 
        
        for(index=0; index < s_nMyANIAlloc; index ++)
            SKY_Free(ppMyANIList[index]);

        for(index=0; index < s_nMyPhoto; index ++)
        {
            SKY_Free(ppMyPhotoList[index]);
            SKY_Free(ppMyPhotoName[index+1]);
        }
              
        
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(hActiveControl != INVALID_CONTROL_HANDLE)
            EQS_SetIOCActive(hActiveControl);
        break;
        
    case SE_KEY_CLEAR:
    case SE_KEY_END:
		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdleMessage = TRUE;
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;
	case SE_IO_SCROLL_LIST_CANCEL:
		if( pEvent->wParam == EQS_GetNthIOC(SIP_ANI_LIST) )
		{
			EQC_SetScrollLogicalIndex(EQS_GetNthIOC(SIP_ANI_LIST), nPictureIndex);
            /* rosa 2001-09-20 --> @why, what */
			//if(nPictureIndex == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto)
			//	EQC_SET_FOCUS(EQS_GetNthIOC(SIP_DISPLAY_WATCH), FALSE);
			//else
			//	EQC_SET_FOCUS(EQS_GetNthIOC(SIP_DISPLAY_WATCH), TRUE);
            /* <-- */

		}
		break;
    case SE_IO_SCROLL_LIST_SELECT:
		if( pEvent->wParam ==  EQS_GetNthIOC(SIP_ANI_LIST) )
		{
			nPictureIndex = pEvent->dwParam;		
		}  
		// pass  next
    case SE_IO_SCROLL_LIST_CHG:
        /*
        if(EQS_GetIOCOrder((UINT8)pEvent->wParam) !=  SIP_ANI_LIST)
            break;

        if(pEvent->dwParam == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto)
        {
            EQC_SET_FOCUS(EQS_GetNthIOC(SIP_DISPLAY_WATCH), FALSE);
        }
        else
        {
            EQC_SET_FOCUS(EQS_GetNthIOC(SIP_DISPLAY_WATCH), TRUE);
        }*/
        break;

    case SE_SOFTKEY_SAVE:
        sSaveIdleDisplay();
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

        fEndMessage = TRUE;
        break;

    case SE_SOFTKEY_BEFOREVIEW:
        hActiveControl = EQS_GetFocusedIOC();

        if(!EQS_IsActive(hActiveControl))
            hActiveControl = INVALID_CONTROL_HANDLE;

        l_nMyAni = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIP_ANI_LIST));
        l_Screen = (l_nMyAni == (NUM_ANI_DATA + s_nMyANI+s_nMyPhoto)) ? IS_CLOCK : IS_PICTURE;
        /* rosa 2001-09-20 --> @why, what */
        //l_fDisplayWatch = EQC_GetCheckBox(EQS_GetNthIOC(SIP_DISPLAY_WATCH), 0);
        l_Watch = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIP_DISPLAY_WATCH));
        /* <-- */
        l_fDisplayName = EQC_GetCheckBox(EQS_GetNthIOC(SIP_DISPLAY_NAME), 0);
        EQC_GetTextEditData(EQS_GetNthIOC(SIP_NAME), l_szName);

        EQS_START_CHILD(sSKA_IdleDisplayPreview);
        break;

    case SE_IO_MESGBOX_YES:
        if(fSaveMessage)
        {
            sSaveIdleDisplay();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdleMessage)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    case SE_IO_MESGBOX_END:
        if(fIdleMessage)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_END();
        else if(fReActive)
            EQS_SetIOCActive(hControl);
        
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSaveIdleDisplay(void)
{
    BOOL fDisplayWatch;
    UINT8 nIndex;
    PICTURE_TYPE_T tmpPicture;
    BYTE   szBanner[MAX_BANNER_LENGTH+1];

    // 대기중이미지
    nIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIP_ANI_LIST));

    sGetPicture(nIndex, &tmpPicture, TRUE);
    SKY_SET_IDLE_PICTURE(tmpPicture);

#ifdef FEATURE_CHARACBELL
    if(nIndex == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto+s_nCANI)
#else
    if(nIndex == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto)
#endif // FEATURE_CHARACBELL
    {
        SKY_SET_IDLE_SCREEN(IS_CLOCK);
        //SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);
        //SKY_SET_IDLE_DISPLAY_NAME(EQC_GetCheckBox(EQS_GetNthIOC(SIP_DISPLAY_NAME), 0));
    }
    else
    {
        SKY_SET_IDLE_SCREEN(IS_PICTURE);
    }

    SKY_SET_IDLE_DISPLAY_WATCH((IDLE_DISPLAY_WATCH_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIP_DISPLAY_WATCH)));
    SKY_SET_IDLE_DISPLAY_NAME(EQC_GetCheckBox(EQS_GetNthIOC(SIP_DISPLAY_NAME), 0));

    // 휴대폰 이름
    EQC_GetTextEditData(EQS_GetNthIOC(SIP_NAME), szBanner);
    SKY_SET_BANNER(szBanner);
}

//#define sRIGHT_MARGIN   

LOCAL void SKY_API sAddIdleDisplayControls(void)
{
    UINT8   index;
    BYTE *pszToken[1];

    ppTypeList[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_BASIC);
    ppTypeList[1] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_IDLE_MYANI);
    ppTypeList[2] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_MY_PHOTO);
#ifdef FEATURE_CHARACBELL
    ppTypeList[3] = (BYTE*)SKY_GET_TOKEN(TKN_BELL_GROUP_CHARAC_BELL);
    ppTypeList[4] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_IDLE_NOANI);
#else
    ppTypeList[3] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_IDLE_NOANI);
#endif // FEATURE_CHARACBELL
    ppNoAniList[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_IDLE_NOANI);

    // Init List
    for(index=0; index<NUM_ANI_DATA; index++)
        ppAniList[index] = (BYTE *)SKY_GET_TOKEN(l_pPictureList[index].PictureName);

    sGetMyAniList();

    for(index=NUM_ANI_DATA; index < NUM_ANI_DATA+s_nMyANI; index++)
        ppAniList[index] = ppMyANIList[index-NUM_ANI_DATA];

    sGetMyPhotoList(TRUE);
    if(s_nMyPhoto)
        s_nMyPhoto++;

    for(index=NUM_ANI_DATA+s_nMyANI; index < NUM_ANI_DATA+s_nMyANI+s_nMyPhoto; index++)
        ppAniList[index] = ppMyPhotoName[index-NUM_ANI_DATA-s_nMyANI];

#ifdef FEATURE_CHARACBELL
    sGetCAniList();

    for(index=NUM_ANI_DATA+s_nMyANI+s_nMyPhoto; index < NUM_ANI_DATA+s_nMyANI+s_nMyPhoto+s_nCANI; index++)
        ppAniList[index] = ppCANIList[index-NUM_ANI_DATA-s_nMyANI-s_nMyPhoto];
    ppAniList[NUM_ANI_DATA+s_nMyANI+s_nMyPhoto+s_nCANI] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);
#else
    ppAniList[NUM_ANI_DATA+s_nMyANI+s_nMyPhoto] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_NOANI);
#endif // FEATURE_CHARACBELL

    // 대기중
    EQC_StaticText(EQS_NA, EQS_NA, TKN_PICTURE_IDLE_ANI);
    sAddScrollList(SKY_GET_IDLE_PICTURE(), TRUE);

    /* rosa 2001-09-20 --> @why, what */
    // 시계 선택
    EQC_StaticText(EQS_NA, EQS_NA, TKN_PICTURE_WATCH);

    ppWatchType[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_WATCH_NO);
    ppWatchType[1] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_WATCH_SMALL);
    ppWatchType[2] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_WATCH_BIG);
    ppWatchType[3] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_WATCH_ANAL1);
    ppWatchType[4] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_WATCH_ANAL2);

    EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, 
                                  EQS_NA, 
                                  PIC_SCROLL_LIST_CHAR, 
                                  SP_GET_SLSIZE(NUM_WATCH_POS_TYPE), 
                                  ppWatchType, 
                                  NUM_WATCH_POS_TYPE, 
                                  NUM_WATCH_POS_TYPE,
                                  SKY_GET_IDLE_DISPLAY_WATCH()),
                  SP_SCROLL_LIST_STYLE);
/*
    // 시계 표시
    EQC_StaticText(EQS_NA, EQS_NA, TKN_PICTURE_WATCH);

    // 시계 화면에 표시함
    pszToken[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_IDLE_WATCH);
    EQC_CheckBox(0, 0, (BYTE **)pszToken, 1);
    EQC_SET_STYLE(EQS_GetNthIOC(SIP_DISPLAY_WATCH), 0);
    if(SKY_GET_IDLE_DISPLAY_WATCH())
        EQC_SetCheckBox(EQS_GetNthIOC(SIP_DISPLAY_WATCH), 0);
    else
        EQC_ResetCheckBox(EQS_GetNthIOC(SIP_DISPLAY_WATCH), 0);
*/
    /* <-- */

    // 휴대폰 이름
    EQC_StaticText(EQS_NA, EQS_NA, TKN_PHONE_NAME);
    EQC_SET_STYLE(EQC_TextEdit(EQS_NA,EQS_NA, PIC_SCROLL_LIST_CHAR, 1 , TEM_KOREAN, MAX_BANNER_LENGTH, SKY_GET_BANNER(),0),
        CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_USE_SKFONT);

    pszToken[0] = (BYTE*)SKY_GET_TOKEN(TKN_PICTURE_IDLE_NAME);

    EQC_CheckBox(0, 0, (BYTE **)pszToken, 1);
    EQC_SET_STYLE(EQS_GetNthIOC(SIP_DISPLAY_NAME), 0);

    if(SKY_GET_IDLE_DISPLAY_NAME())
        EQC_SetCheckBox(EQS_GetNthIOC(SIP_DISPLAY_NAME), 0);
    else
        EQC_ResetCheckBox(EQS_GetNthIOC(SIP_DISPLAY_NAME), 0);

    // Screen Formatting
    EQS_StartFormatter();

    for(index = 0; index < SIP_MAX; index++)
    {
        if(index == SIP_DISPLAY_NAME)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(index), 
                ES_AUTO, 0, EHA_RIGHT, EVA_CENTER, FALSE);
            /* jrkwon 2001.06.28 --> pretty screen - -; */
            EQC_GET_X(EQS_GetNthIOC(index)) -= (R_SCROLLBAR_SIZE)/2;// + sRIGHT_MARGIN);

        }
        else
            EQS_SetControlPosition(EQS_GetNthIOC(index), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

        EQS_NewLine();
    }

    EQS_EndFormatter();
    EQS_FitCanvasSize();

    //if(EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SIP_ANI_LIST)) == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto)
    //{
        //EQC_SET_FOCUS(EQS_GetNthIOC(SIP_DISPLAY_NAME), FALSE);
        //EQC_SET_FOCUS(EQS_GetNthIOC(SIP_DISPLAY_WATCH), FALSE);
    //}
    EQS_SetNthIOCFocus(SIP_ANI_LIST);
}

BOOL SKY_API IsIdleScreenPreview(void)
{
    return l_fIdleScreenPreview;
}

void SKY_API GetIdleScreenPreviewInfo(IDLE_SCREEN_PREVIEW_INFO_T* pIdlePreviewInfo)
{
    pIdlePreviewInfo->Screen = l_Screen;
    /* rosa 2001-09-20 --> @why, what */
    //pIdlePreviewInfo->fDisplayWatch = l_fDisplayWatch;
    pIdlePreviewInfo->Watch = l_Watch;
    /* <-- */
    pIdlePreviewInfo->fDisplayName = l_fDisplayName;
    STRCPY(pIdlePreviewInfo->szDispalyName, l_szName);
}

LOCAL BITMAP l_BmpBuf = {
    { EQUS_IMAGE_ID, { SKY_SCREEN_BUFFER_WIDTH, SKY_SCREEN_BUFFER_HEIGHT}, 
        SKY_SCREEN_WIDTH, BIT_PER_PIXEL_256C, FALSE, MAX_COLOR, 0,  },
    g_abBmpBuf
};

LOCAL void SKY_API sSKA_IdleDisplayPreview(SKY_EVENT_T *pEvent)
{
    HFILE   hFile;
    static HCONTROL hIdlePicture;
    DWORD dwTimeInfo;
    UINT16  nPic;
    DWORD dwCurrentTime;
    clk_julian_type julian;
    static UINT8   s_nPhotoIndex = 0;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, //SKY_GET_TOKEN(TKN_PICTURE_IDLE_PREVIEW), 
            WA_SHOW_HEADER);//WA_SHOW_HEADER | WA_SHOW_CML);

        sSET_PICTURE_LIST();

        hIdlePicture = NULL;
        s_nPhotoIndex = 0;
        l_fIdleScreenPreview = TRUE;

        memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));

        dwCurrentTime = SKY_GetLocalTime();
        if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
        {
            clk_secs_to_julian(dwCurrentTime, &julian);
        }
        else
        {
            julian.month = julian.day = julian.day_of_week = julian.hour = julian.minute = 0;
        }

        if(l_nMyAni == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto)    // 지정안함
        {
            //SKY_DisplayDateTime(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
        }
        else if(l_nMyAni == NUM_ANI_DATA + s_nMyANI)  // 나만의 사진
        {
            BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

			if(SKY_OpenDataFile(SDF_PHOTO, ppMyPhotoList[s_nPhotoIndex], &hFile, SFT_READ) == SDR_OK)
            {
				SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
				SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
				SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
				SKY_CloseDataFile(hFile); 

                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
				hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }

            EQS_StartTimer(EQS_TIMER_1, 1000, TRUE);
        }
        else if(l_nMyAni > NUM_ANI_DATA + s_nMyANI)  // 나만의 사진
        {
			BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

			if(SKY_OpenDataFile(SDF_PHOTO, ppMyPhotoList[l_nMyAni - (NUM_ANI_DATA + s_nMyANI)-1], &hFile, SFT_READ) == SDR_OK)
            {
				SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
				SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
				SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
				SKY_CloseDataFile(hFile); 

                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
				hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }

            //SKY_DisplayDateTimeWithPic(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
        }
        else if(l_nMyAni >= NUM_ANI_DATA )
        {
			if(SKY_OpenDataFile(SDF_IMAGE, ppMyANIList[l_nMyAni-NUM_ANI_DATA], &hFile, SFT_READ) == SDR_OK)
            {
				SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

				if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
				{
					SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
					SKY_CloseDataFile(hFile);
                    //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
					hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
				}
				else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
				{
					ANIMATION_HEADER_T AniHeader;
					BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

					SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
					SKY_CloseDataFile(hFile);
					sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)ppMyANIList[l_nMyAni-NUM_ANI_DATA]);
					hIdlePicture = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);

					//EQC_SetStartYAnimation(hIdlePicture, SKY_ANNUNCIATORS_HEIGHT);
					//EQC_SetAnimationHeight(hIdlePicture, SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT));
					EQC_StartAnimation(hIdlePicture, TRUE);
				}
            }

            //SKY_DisplayDateTimeWithPic(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
        }
        else
		{
            nPic = l_pPictureList[l_nMyAni].wPictureID;

			hIdlePicture = EQC_Animation(0, 0, g_pBuiltInAnimation[nPic], NULL);

			//EQC_SetStartYAnimation(hIdlePicture, SKY_ANNUNCIATORS_HEIGHT);
			//EQC_SetAnimationHeight(hIdlePicture, SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT));
			EQC_StartAnimation(hIdlePicture, TRUE);

            //SKY_DisplayDateTimeWithPic(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
        }
        
        SKY_DisplayDateTime(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);

        break;
        
    case SE_TM_TIMER1:
        {
            BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

            if(hIdlePicture)
                EQS_DeleteControl(hIdlePicture);

            s_nPhotoIndex ++;
            if(s_nPhotoIndex >= s_nMyPhoto-1)
                s_nPhotoIndex = 0;

            memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));

			if(SKY_OpenDataFile(SDF_PHOTO, ppMyPhotoList[s_nPhotoIndex], &hFile, SFT_READ) == SDR_OK)
            {
				SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
				SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
				SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
				SKY_CloseDataFile(hFile); 

                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
				hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
        }
        break;

    case SE_APP_END :   /* The very last event */ 
        l_fIdleScreenPreview = FALSE;
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
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
    case EQE_KEY_LEFT:
	case EQE_KEY_RIGHT:
	case EQE_KEY_UP:
	case EQE_KEY_DOWN:
    case EQE_KEY_F1:
    case EQE_KEY_F2:
    case EQE_KEY_POUND:
    case EQE_KEY_STAR:
    case EQE_KEY_CENTER:
    case EQE_KEY_CLEAR:
        EQS_End();
        break;

    case EQE_KEY_END:
        EQS_EndCurrentEvent();
        break;

    case SE_APP_DRAW:
        dwCurrentTime = SKY_GetLocalTime();
        if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
            clk_secs_to_julian(dwCurrentTime, &julian);
        else
            julian.month = julian.day = julian.day_of_week = julian.hour = julian.minute = 0;

        //if(l_nMyAni == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto)
            SKY_DisplayDateTime(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
        //else
        //    SKY_DisplayDateTimeWithPic(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/* rosa 2001-10-22 --> 그림설정에서 미리보기 메뉴 추가함 */
LOCAL void SKY_API sSKA_PictureDisplayPreview(SKY_EVENT_T *pEvent)
{
    HFILE   hFile;
    static HCONTROL hIdlePicture;
    DWORD dwTimeInfo;
    UINT16  nPic;
    DWORD dwCurrentTime;
    clk_julian_type julian;
    static UINT8   s_nPhotoIndex = 0;
    SKA_PICTURE_E PictureKind;

    switch(pEvent->EventID)
    {
    case SE_APP_START :
        PictureKind = (SKA_PICTURE_E)pEvent->wParam;
        if((PictureKind == SP_CALL_LIST) || (PictureKind == SP_NTOP_LIST)) //전화가 왔을때 || NATE시작할때
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE, 
                NULL,
                WA_SHOW_HEADER);
        }
        else
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE, 
                NULL,
                WA_NONE);
        }

        sSET_PICTURE_LIST();

        hIdlePicture = NULL;
        s_nPhotoIndex = 0;
        l_fIdleScreenPreview = TRUE;

        memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));

        if(l_nMyAni == NUM_ANI_DATA + s_nMyANI+s_nMyPhoto) //그림지정안함
        {
            // 아무것도 표시하지 않는다.
        }
        else if(l_nMyAni >= NUM_ANI_DATA + s_nMyANI) //나만의사진
        {
			BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

			if(SKY_OpenDataFile(SDF_PHOTO, ppMyPhotoList[l_nMyAni - (NUM_ANI_DATA + s_nMyANI)], &hFile, SFT_READ) == SDR_OK)
            {
				SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
				SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
				SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
				SKY_CloseDataFile(hFile); 

                if((PictureKind == SP_CALL_LIST) || (PictureKind == SP_NTOP_LIST)) //전화가 왔을때 || NATE시작할때
                    hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
                else
                    hIdlePicture = EQC_Image(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), &l_BmpBuf);
            }
        }
        else if(l_nMyAni >= NUM_ANI_DATA ) //나만의그림
        {
			if(SKY_OpenDataFile(SDF_IMAGE, ppMyANIList[l_nMyAni-NUM_ANI_DATA], &hFile, SFT_READ) == SDR_OK)
            {
				SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

				if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
				{
					SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
					SKY_CloseDataFile(hFile);

                    if((PictureKind == SP_CALL_LIST) || (PictureKind == SP_NTOP_LIST)) //전화가 왔을때 || NATE시작할때
    					hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
                    else
                        hIdlePicture = EQC_Image(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), &l_BmpBuf);
				}
				else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
				{
					ANIMATION_HEADER_T AniHeader;
					BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

					SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
					SKY_CloseDataFile(hFile);
					sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)ppMyANIList[l_nMyAni-NUM_ANI_DATA]);

                    if((PictureKind == SP_CALL_LIST) || (PictureKind == SP_NTOP_LIST)) //전화가 왔을때 || NATE시작할때
					    hIdlePicture = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);
                    else
                        hIdlePicture = EQC_Animation(GET_IMG_X_FULLSCREEN(l_BmpBuf), GET_IMG_Y_FULLSCREEN(l_BmpBuf), NULL, abBuf);

					EQC_StartAnimation(hIdlePicture, TRUE);
				}
            }
        }
        else //기본 제공 애니메이션
		{
            nPic = l_pPictureList[l_nMyAni].wPictureID;

			hIdlePicture = EQC_Animation(0, 0, g_pBuiltInAnimation[nPic], NULL);

			EQC_StartAnimation(hIdlePicture, TRUE);
        }
        break;
        
    case SE_TM_TIMER1:
        {
            BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

            if(hIdlePicture)
                EQS_DeleteControl(hIdlePicture);

            s_nPhotoIndex ++;
            if(s_nPhotoIndex >= s_nMyPhoto-1)
                s_nPhotoIndex = 0;

            memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));

			if(SKY_OpenDataFile(SDF_PHOTO, ppMyPhotoList[s_nPhotoIndex], &hFile, SFT_READ) == SDR_OK)
            {
				SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
				SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
				SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
				SKY_CloseDataFile(hFile); 

				hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
        }
        break;

    case SE_APP_END:
        l_fIdleScreenPreview = FALSE;
        break;
        
    case SE_APP_SUSPEND:
        break;
        
    case SE_APP_RESUME:
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
    case EQE_KEY_LEFT:
	case EQE_KEY_RIGHT:
	case EQE_KEY_UP:
	case EQE_KEY_DOWN:
    case EQE_KEY_F1:
    case EQE_KEY_F2:
    case EQE_KEY_POUND:
    case EQE_KEY_STAR:
    case EQE_KEY_CENTER:
    case EQE_KEY_CLEAR:
        EQS_End();
        break;

    case EQE_KEY_END:
        EQS_EndCurrentEvent();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/* <-- */

