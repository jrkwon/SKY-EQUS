/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_cam.c
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
** 4334-01-11 white     Created.
**
** ===========================================================================
*/
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdlib.h>
#include "sky.h"
#include "ska.h"

extern byte *CameraCapturedRGB (void);
extern void InitCamera (void);
extern void StopCamera (void);
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define     SELECT_POPUP                1
#define     CLEAR_POPUP                 2
#define     RENAME_POPUP                3
#define     REVIEW_POPUP                4

#define     CAMERA_DETECTING_TIME       3
#define     PHOTO_H_STRING              20
//#define     sMAX_PHOTO_COUNT            50
#define     NEED_MIN_SPACE              sizeof(DWORD) + SKY_MAX_CAMERA_IMAGE_FILE_LEN + PHOTO_TOTAL_SIZE + sizeof(BitmapCameraInfo.Header)
/*
#define IMAGE_HEADER_SIZE               9 + 1    
#define IMAGE_VIEWER_WIDTH              120
#define IMAGE_VIEWER_HEIGHT             120
#define GET_IMAGE_BYTE_SIZE(BitmapCameraInfo.Header)         14400
#define PHOTO_LIST_LINE                 7

#define PHOTO_HEADER_SIZE               IMAGE_HEADER_SIZE   
#define PHOTO_H_SIZE                    120//96
#define PHOTO_V_SIZE                    90//72
#define PHOTO_TOTAL_SIZE                10800//6912    
#define PHOTO_MAX_DATA                  40
#define COLOR_IDLE_IMAGE_BPP            8

#define PHOTO_DRAW_X                    0//10
#define PHOTO_DRAW_Y                    0//8
#define CAMERA_BUF_SPACE                1800//3600
*/
#define PHOTO_TOTAL_SIZE                (PHOTO_H_SIZE*PHOTO_V_SIZE*2)//6912
#define COLOR_IDLE_IMAGE_BPP            16      
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
enum tag_PHOTO_NAME_E {
    PHOTO_TITLE,
    PHOTO_TEXT,
    PHOTO_MAX
};

struct tagPHOTO_REVIEW_T{
    BOOL fFormReview;
    WORD    wViewIndex; 
};

typedef struct tagPHOTO_REVIEW_T PHOTO_REVIEW_T;



/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
// cameratest.c에서 사용함 by jnlee 01.06.19
byte YUV[21700];	//header 32byte data 120*90*2 end2byte = 21600+34
byte rgb[21600];

//youandi 01.06.12
//이 부분은 멜로디 파일 플레이를 위해서 넣었습니다.
//멜로디 최대 크기는 50k로 잡았고 문제가 생기면 youandi에게 연력주세요
BYTE pMusicDataPoint[50000];
void Music_Get_Share_Memory(BYTE **pAllocBlock)
{
	*pAllocBlock=pMusicDataPoint;
}

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
PHOTO_REVIEW_T l_Review;
/*
LOCAL BITMAP BitmapImageInfo = {
    { EQUS_IMAGE_ID, { IMAGE_VIEWER_WIDTH, IMAGE_VIEWER_HEIGHT}, 
      IMAGE_VIEWER_WIDTH, COLOR_IDLE_IMAGE_BPP, FALSE, MAX_COLOR , 0}
};
*/

LOCAL BITMAP BitmapCameraInfo = {
    { EQUS_IMAGE_ID, { PHOTO_H_SIZE, PHOTO_V_SIZE}, 
      PHOTO_H_SIZE, COLOR_IDLE_IMAGE_BPP, FALSE, MAX_COLOR, 0}
};

//LOCAL UINT8 s_nImage;
//LOCAL WORD s_wIndex=0;
//LOCAL BYTE *ppImageList[SKY_MAX_NUM_FILES];
UINT  nNumOfPhoto;
PHOTO_LIST_T g_PhotoList[MAX_PHOTO_COUNT];

BYTE* l_paListBuf[MAX_PHOTO_COUNT];

LOCAL WORD s_aCameraData[PHOTO_TOTAL_SIZE/2];
LOCAL WORD s_aCameraRevData[PHOTO_TOTAL_SIZE/2];

LOCAL BYTE      WithoutIcon[SKY_FILE_NAME_MAX_LENGTH+2];
#ifdef FEATURE_CHARACTER_BELL
LOCAL BYTE      WithIcon[SKY_FILE_NAME_MAX_LENGTH+2];
#endif // FEATURE_CHARACTER_BELL
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
UINT SKY_InitCameraList(VOID);

LOCAL void SKY_API sSKA_PhotoSave(SKY_EVENT_T *pEvent);

UINT SKY_InitCameraList(VOID)
{   
    HFILE   hFile,nFiles;
    DWORD   AvailMem;
    UINT8   i,j;
    PHOTO_LIST_T tmpPhotoList[MAX_PHOTO_COUNT];
   
    if(!SKY_IsFileOrDirExist(FILE_NAME_CAMERA))
    {   
        nNumOfPhoto = 0;
        
        // Create file
        hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_CREATE);

        memset(g_PhotoList, NULL, sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
        
        SKY_WriteFile(hFile, (CONST void *)g_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                
        SKY_CloseFile(hFile);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READONLY);

        SKY_ReadFile(hFile, (CONST void *)tmpPhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);

		SKY_CloseFile(hFile);
                
        nNumOfPhoto = 0;

        memset(g_PhotoList, NULL, sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
        
        for(i = 0,j = 0; i < MAX_PHOTO_COUNT; i++)
        {
            if(tmpPhotoList[i].PhotoList != NULL)
            {  
                g_PhotoList[j]=tmpPhotoList[i];
                nNumOfPhoto++;
                j++;
            }
        }

    }
    return nNumOfPhoto;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainMenuCamera()
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
void SKY_API SKA_MainMenuCamera(SKY_EVENT_T *pEvent)
{

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);
        
        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        EQC_MenuList(6, 4, 18, 6, &mnCamera, 0,SKY_GET_SEL_MENU_MODE());
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_RESUME:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
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


/*============================================================================= 
Function:    
SKA_MainCamera

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
void SKY_API SKA_MainCamera(SKY_EVENT_T *pEvent)
{
    STATIC BOOL s_fCameraMode = FALSE;
    STATIC BOOL s_fReverse = TRUE;
    STATIC HCONTROL s_hStrtControl, s_hPhotoMail, s_hCameraDetect;
    WORD        wMaxDataCount = 0, i, j;
    STATIC BYTE s_bFromVideoMail = 0;
    HCONTROL    hControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        s_fReverse = TRUE;
#ifdef FEATURE_SKTT_PM       
        if(EQS_GET_PARENT()==SKA_EditNewPM)
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_BOTTOMUP,
                SKY_GET_TOKEN(TKN_TAKE_PHOTO), 
                WA_SHOW_HEADER | WA_SHOW_CML);
        }else
#endif
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_VIEW_SAVE_BOTTOMUP,
                SKY_GET_TOKEN(TKN_TAKE_PHOTO), 
                WA_SHOW_HEADER | WA_SHOW_CML);
        }
        
        s_bFromVideoMail = pEvent->wParam;

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(AB_ALWAYS);
        s_fCameraMode = TRUE;
        BitmapCameraInfo.Header.abID[0] = 'I';
        BitmapCameraInfo.Header.abID[1] = 'M';
        BitmapCameraInfo.Header.abID[2] = 'G';
        BitmapCameraInfo.Header.Size.xWidth = PHOTO_H_SIZE;
        BitmapCameraInfo.Header.Size.yHeight = PHOTO_V_SIZE;
        BitmapCameraInfo.Header.xWidth = PHOTO_H_SIZE;//COLOR_IDLE_IMAGE_WIDTH;
        BitmapCameraInfo.Header.nBitsPerPixel = COLOR_IDLE_IMAGE_BPP;
        BitmapCameraInfo.Header.fPalette = FALSE;
        BitmapCameraInfo.Header.TransparentColor = MAX_COLOR;
        BitmapCameraInfo.Header.yStart = 0;
        
        s_hCameraDetect = EQC_MessageBox(SKY_GET_TOKEN(TKN_CAMERA_DETECTING), CAMERA_DETECTING_TIME, MBT_NONE, MBT_HELP);
        EQC_SET_STYLE(s_hCameraDetect, EQC_GET_STYLE(s_hCameraDetect) | CS_MESSAGE_BOX_IGNORE_KEYS | CS_MESSAGE_BOX_DEFAULT);
        break;

    case SE_KEY_CLEAR:
#ifdef FEATURE_SKTT_PM       
        if(EQS_GET_PARENT()==SKA_EditNewPM)
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_CLEAR_POPUP, EQS_NA);
            break;
        }
        else
        {
            EQS_End();
        }
#else
        EQS_End();
#endif
        
    case SE_IO_MESGBOX_END:
#ifdef FEATURE_SKTT_PM       
        if((pEvent->wParam == s_hPhotoMail) && (EQS_GET_PARENT()==SKA_EditNewPM))
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_CLEAR_POPUP, EQS_NA);
            break;
        }
#endif
        if(pEvent->wParam == s_hPhotoMail)
        {
                EQS_End();
        }else if(pEvent->wParam == s_hCameraDetect)
        {
            if(SKY_IS_CAMERA_ON() == TRUE)
            {
                s_fCameraMode = TRUE;

                InitCamera();
            
                BitmapCameraInfo.Header.abID[0] = 'I';
                BitmapCameraInfo.Header.abID[1] = 'M';
                BitmapCameraInfo.Header.abID[2] = 'G';
                BitmapCameraInfo.Header.Size.xWidth = PHOTO_H_SIZE;
                BitmapCameraInfo.Header.Size.yHeight = PHOTO_V_SIZE;
                BitmapCameraInfo.Header.xWidth = PHOTO_H_SIZE;//COLOR_IDLE_IMAGE_WIDTH;
                BitmapCameraInfo.Header.nBitsPerPixel = COLOR_IDLE_IMAGE_BPP;
                BitmapCameraInfo.Header.fPalette = FALSE;
                BitmapCameraInfo.Header.TransparentColor = MAX_COLOR;
                BitmapCameraInfo.Header.yStart = 0;
                s_hStrtControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_RUN_CAMERA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_STYLE(s_hStrtControl, EQC_GET_STYLE(s_hStrtControl) | CS_MESSAGE_BOX_IGNORE_KEYS | CS_MESSAGE_BOX_DEFAULT);
            }
            else
            {
                BitmapCameraInfo.Header.abID[0] = 'I';
                BitmapCameraInfo.Header.abID[1] = 'M';
                BitmapCameraInfo.Header.abID[2] = 'G';
                BitmapCameraInfo.Header.Size.xWidth = PHOTO_H_SIZE;
                BitmapCameraInfo.Header.Size.yHeight = PHOTO_V_SIZE;
                BitmapCameraInfo.Header.xWidth = PHOTO_H_SIZE;//COLOR_IDLE_IMAGE_WIDTH;
                BitmapCameraInfo.Header.nBitsPerPixel = COLOR_IDLE_IMAGE_BPP;
                BitmapCameraInfo.Header.fPalette = FALSE;
                BitmapCameraInfo.Header.TransparentColor = MAX_COLOR;
                //BitmapCameraInfo.pbData = s_aCameraData;
                BitmapCameraInfo.Header.yStart = 0;

                s_hPhotoMail = EQC_MessageBox(SKY_GET_TOKEN(TKN_CAMERA_OFF), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_STYLE(s_hPhotoMail, EQC_GET_STYLE(s_hPhotoMail) | CS_MESSAGE_BOX_IGNORE_KEYS | CS_MESSAGE_BOX_DEFAULT);
            }
            EQS_StartTimer(EQS_TIMER_1, 300, TRUE);        
        }
        break;

    case SE_APP_END :   /* The very last event */ 
        BitmapCameraInfo.pbData = NULL;
        s_fReverse = TRUE;
        s_bFromVideoMail = 0;
        s_hStrtControl = 0;
        s_fCameraMode = FALSE;
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        StopCamera();
        EQS_StopTimer(EQS_TIMER_1);
        UI_ENABLE_SLEEP();
        EQS_ReleaseDC();
        break;

    case SE_SOFTKEY_BOTTOMUP:     /* Reverse */
        if(SKY_IS_CAMERA_ON() == TRUE)
        {
            if(s_fReverse)
                s_fReverse = FALSE;
            else
                s_fReverse = TRUE;
            hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_DISP_TURN), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_MESSAGE_BOX_IGNORE_KEYS | CS_MESSAGE_BOX_DEFAULT);
        }
        break;

    case SE_KEY_CAMERA_JACK_ON:
        if(!s_fCameraMode)
        {
            s_fCameraMode = TRUE;
            InitCamera();
            memcpy(s_aCameraData, CameraCapturedRGB(), PHOTO_TOTAL_SIZE);
            if(s_fReverse)
            {
                wMaxDataCount = PHOTO_TOTAL_SIZE/2;
                j = 0;
			    i = wMaxDataCount-1;
			    for(;;)
			    {
				    s_aCameraRevData[j++] = s_aCameraData[i];	
				    if ( i == 0 )	break;
				    i--;
			    }   
                BitmapCameraInfo.pbData = s_aCameraRevData;
            }else
            {
                BitmapCameraInfo.pbData = s_aCameraData;
            }
        }
        break;

    case SE_KEY_CAMERA_JACK_OFF:
        s_fCameraMode = FALSE;
        hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CAMERA_OFF), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_MESSAGE_BOX_IGNORE_KEYS | CS_MESSAGE_BOX_DEFAULT);
        break;

    case SE_TM_TIMER1:
        if(SKY_IS_CAMERA_ON() == TRUE)
        {
            UI_DISABLE_SLEEP();
            memcpy(s_aCameraData, CameraCapturedRGB(), PHOTO_TOTAL_SIZE);
            if(s_fReverse)
            {
                wMaxDataCount = PHOTO_TOTAL_SIZE/2;
                j = 0;
			    i = wMaxDataCount-1;
			    for(;;)
			    {
				    s_aCameraRevData[j++] = s_aCameraData[i];	
				    if ( i == 0 )	break;
				    i--;
			    }   
                BitmapCameraInfo.pbData = s_aCameraRevData;
            }else
            {
                BitmapCameraInfo.pbData = s_aCameraData;
            }
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        break;

    case SE_APP_DRAW :
        if(SKY_IS_CAMERA_ON() == TRUE)
        {
            BitmapCameraInfo.Header.abID[0] = 'I';
            BitmapCameraInfo.Header.abID[1] = 'M';
            BitmapCameraInfo.Header.abID[2] = 'G';
            BitmapCameraInfo.Header.Size.xWidth = PHOTO_H_SIZE;
            BitmapCameraInfo.Header.Size.yHeight = PHOTO_V_SIZE;
            BitmapCameraInfo.Header.xWidth = PHOTO_H_SIZE;//COLOR_IDLE_IMAGE_WIDTH;
            BitmapCameraInfo.Header.nBitsPerPixel = COLOR_IDLE_IMAGE_BPP;
            BitmapCameraInfo.Header.fPalette = FALSE;
            BitmapCameraInfo.Header.TransparentColor = MAX_COLOR;
            BitmapCameraInfo.Header.yStart = 0;
			BitmapCameraInfo.pbData = (s_fReverse) ? s_aCameraRevData : s_aCameraData;
            EQS_PutBitmap(PHOTO_DRAW_X, PHOTO_DRAW_Y, &BitmapCameraInfo);
        }
        break;

#ifdef FEATURE_SKTT_PM
    case SE_SOFTKEY_SELECT:
        SKY_DoShutter();
        memset((VOID*)g_abBmpBuf, 0xFF,  PHOTO_TOTAL_SIZE);
        memcpy(&g_abBmpBuf,CameraCapturedRGB(),PHOTO_TOTAL_SIZE);
        if(s_fReverse)
            EQS_START_CHILD_EVENT(SKA_EditPM, SE_APP_START, CAMERA_TURN, EQS_NA);
        else
            EQS_START_CHILD_EVENT(SKA_EditPM, SE_APP_START, EQS_NA, EQS_NA);

        break;
#endif

    case SE_SOFTKEY_VIEW:
        SKY_InitCameraList();
        if( nNumOfPhoto == 0)
        {
            hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_DATA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_MESSAGE_BOX_IGNORE_KEYS | CS_MESSAGE_BOX_DEFAULT);
        }else
        {
            EQS_StopTimer(EQS_TIMER_1);
            //StopCamera();
            EQS_START_CHILD(SKA_PhotoList);
        }
        break;

    case SE_SOFTKEY_SAVE:
        SKY_DoShutter();
        EQS_StopTimer(EQS_TIMER_1);
            
        if((SKY_GetFileNumberInDataFile(SDF_PHOTO)) < MAX_PHOTO_COUNT && \
            SKY_GetFreeSpaceInDataFile() >= NEED_MIN_SPACE)
        {
            //SKY_DoShutter(); //2001.07.23
            if(SKY_IS_CAMERA_ON() == TRUE)
            {
                //StopCamera();
                EQS_START_CHILD_EVENT(sSKA_PhotoSave, SE_APP_START, s_fReverse,EQS_NA);
            }else
            {
                EQS_StartTimer(EQS_TIMER_1, 300, TRUE);        
                hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CAMERA_OFF), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_MESSAGE_BOX_IGNORE_KEYS | CS_MESSAGE_BOX_DEFAULT);
            }
        }else
        {
            EQS_StartTimer(EQS_TIMER_1, 300, TRUE);        
            hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_MORE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_MESSAGE_BOX_IGNORE_KEYS | CS_MESSAGE_BOX_DEFAULT);
        }
        break;

    case SE_SOFTKEY_CANCEL:
        EQS_End();
        break;

    case SE_KEY_1:
#ifdef FEATURE_SKTT_PM        
//        EQS_START_CHILD(SKA_MainVideoMail);
#endif
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        //EQS_StopTimer(EQS_TIMER_1);
        break;

    case SE_APP_RESUME:
        
        if(pEvent->wParam == PM_END_POPUP)  //#define PM_END_POPUP 7115 ska_vm.h
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_END_POPUP, EQS_NA);
        }
        else if(pEvent->wParam== PM_TRAN_POPUP)
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);
        }
        else
        {
#ifdef FEATURE_SKTT_PM
            if(EQS_GET_CHILD() == sSKA_PhotoSave && s_bFromVideoMail == TO_CAMERA_FROM_VMAIL)
            {    
                EQS_END_PARAM(EQS_NA, (SKY_GET_CAMERA_COUNT() - 1));
                break;
            }
#endif
            memcpy(s_aCameraData, CameraCapturedRGB(), PHOTO_TOTAL_SIZE);
            BitmapCameraInfo.Header.abID[0] = 'I';
            BitmapCameraInfo.Header.abID[1] = 'M';
            BitmapCameraInfo.Header.abID[2] = 'G';
            BitmapCameraInfo.Header.Size.xWidth = PHOTO_H_SIZE;
            BitmapCameraInfo.Header.Size.yHeight = PHOTO_V_SIZE;
            BitmapCameraInfo.Header.xWidth = PHOTO_H_SIZE;//COLOR_IDLE_IMAGE_WIDTH;
            BitmapCameraInfo.Header.nBitsPerPixel = COLOR_IDLE_IMAGE_BPP;
            BitmapCameraInfo.Header.fPalette = FALSE;
            BitmapCameraInfo.Header.TransparentColor = MAX_COLOR;
            BitmapCameraInfo.Header.yStart = 0;
            
            if(s_fReverse)
            {
                wMaxDataCount = PHOTO_TOTAL_SIZE/2;
                j = 0;
			    i = wMaxDataCount-1;
			    for(;;)
			    {
				    s_aCameraRevData[j++] = s_aCameraData[i];	
				    if ( i == 0 )	break;
				    i--;
			    }   
                BitmapCameraInfo.pbData = s_aCameraRevData;
            }else
            {
                BitmapCameraInfo.pbData = s_aCameraData;
            }

            //BitmapCameraInfo.pbData = s_aCameraData;       
            //InitCamera();
            SKY_SET_NO_OFF_BACKLIGHT(TRUE);
            SKY_BacklightOn(AB_ALWAYS);       
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
            EQS_StartTimer(EQS_TIMER_1, 300, TRUE);  
        }
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
enum tag_PHOTO_VIEWER_E {
    PV_FILENAME,
    PV_IMAGE,
    PV_TIMEINFO,
    PV_MAX
};

/*============================================================================= 
 Function:    
     SKA_PhotoViewer

 Description: 
     @Description

 Input:
     pEvent->dwParam : Photo List Number

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
===============================================================================*/
void SKY_API SKA_PhotoViewer(SKY_EVENT_T *pEvent)
{
    STATIC BYTE s_psFileName[PHOTO_H_STRING + 1]; 
    STATIC BYTE s_szTimeInfo[PHOTO_H_STRING + 1];
	BYTE psFileList[SKY_FILE_EXT_NAME_MAX_LENGTH+1]; 
    BYTE s_szTemp[PHOTO_H_STRING + 1];
    STATIC WORD s_wIndex = 0;
    BOOL    fAmPm;
    INT8 bStrLen, i , j, nCount, n;
    HFILE hFile;  
    clk_julian_type julian;
    STATIC DWORD       dwPhoto = 0;
    DWORD   dwTimeInfo = 0;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        EQS_GetDC(TRUE);
        if(EQS_GET_PARENT()!=SKA_PhotoList)
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                105, 
                CL_NONE_SELECT_NONE,
                SKY_GET_TOKEN(TKN_PHOTO_VIEW), 
                WA_SHOW_CML| WA_SHOW_HEADER);
        }else
        {
           
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                120,  
                CL_NONE,
                SKY_GET_TOKEN(TKN_PHOTO_VIEW), 
                WA_SHOW_CML|WA_SHOW_HEADER);
        }
        
#ifdef FEATURE_WAP
        if(SKY_IS_IN_WAP())
        {
            EQS_END_EVENT(SE_APP_RESUME, CLEAR_POPUP, dwPhoto);
            break;
        }
#endif //#ifdef FEATURE_WAP
        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(AB_ALWAYS);
        dwPhoto = pEvent->dwParam;
        s_wIndex = pEvent->wParam;
        ConvertCameraImageFileName(s_psFileName, pEvent->dwParam); 
        if(SKY_OpenDataFile(SDF_PHOTO, s_psFileName, &hFile, SFT_READ) == SDR_OK)
        {
            SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
            SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
            SKY_ReadDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));  
            SKY_ReadDataBlock(hFile, g_abBmpBuf, PHOTO_TOTAL_SIZE);   

            BitmapCameraInfo.pbData = g_abBmpBuf;  // woodstok 01.4.8
            SKY_CloseDataFile(hFile); 
            //BitmapCameraInfo.Header.yStart = 0;
            
            clk_secs_to_julian(dwTimeInfo, &julian);
            fAmPm = (BOOL) ((julian.hour < 12) ? TRUE : FALSE);
            if(fAmPm == FALSE && julian.hour != 12)
               julian.hour -= 12;
            else if(fAmPm == FALSE && julian.hour == 24)
               julian.hour = 0;

            sprintf((CHAR*)s_szTimeInfo, " %02d.%02d(%s)%02d:%02d%s", 
                         julian.month, 
                         julian.day, 
                         (CHAR*)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
                         julian.hour,
                         julian.minute,
                         (fAmPm) ? SKY_GET_TOKEN(TKN_IDLE_AM): SKY_GET_TOKEN(TKN_IDLE_PM));
            
            EQC_Image(4,18,(BITMAP*)&BitmapCameraInfo);
            
            //EQS_SetSWindowHeaderStr(s_psFileName);
            EQC_StaticText(0,2,TKN_PHOTO_BLANK);
            //if(EQS_GET_PARENT()==SKA_PhotoList)     
            EQC_StaticText(0,109,TKN_PHOTO_BLANK);
           
            bStrLen = STRLEN(s_psFileName);

            if(bStrLen  < SKY_MAX_CAMERA_IMAGE_FILE_LEN - 1)
            {
                j = bStrLen;
                for(i = (bStrLen/2); j >= 0 ; i--)
                {
                    s_psFileName[9 + i] = s_psFileName[j--];
                }    
                for(i = 9 + i; i >= 0 ; i --)
                {
                    s_psFileName[i] = ' ';
                }

                EQC_SetStaticText(EQS_GetNthIOC(PV_FILENAME),s_psFileName);
            
			}else
            {
                s_szTemp[0] = 0;
                STRCAT(s_szTemp,"  ");
                STRCAT(s_szTemp,s_psFileName);
                EQC_SetStaticText(EQS_GetNthIOC(PV_FILENAME),s_szTemp);
            }
			
            //if(EQS_GET_PARENT()==SKA_PhotoList)     
            EQC_SetStaticText(EQS_GetNthIOC(PV_TIMEINFO),s_szTimeInfo);

        }else
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CANT_OPEN_DATA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        break;

    case SE_KEY_END:
        if(EQS_GET_PARENT()==SKA_EditNewPM)
            EQS_END_EVENT(SE_APP_RESUME, PM_END_POPUP, EQS_NA); 
        else
            EQS_END_CURRENT_EVENT();
        break;

    case SE_KEY_UP:
    case SE_KEY_DOWN:
        if(EQS_GET_PARENT() == SKA_PhotoList || EQS_GET_PARENT() == SKA_PhotoListNView)
        {
            nCount = SKY_GetFileNumberInDataFile(SDF_PHOTO);
            if(pEvent->EventID == SE_KEY_UP)
            {
                if(s_wIndex == 0)
                    s_wIndex = nCount-1;
                else
                    s_wIndex--;
            }
            else if(pEvent->EventID == SE_KEY_DOWN)
            {
                if(s_wIndex+1 == nCount)
                    s_wIndex = 0;
                else
                    s_wIndex++;
            }
			ConvertCameraImageFileName(psFileList, g_PhotoList[s_wIndex].PhotoList); 
            if(SKY_OpenDataFile(SDF_PHOTO, psFileList, &hFile, SFT_READ) == SDR_OK)
			{
                SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
                SKY_ReadDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));  
                SKY_ReadDataBlock(hFile, g_abBmpBuf, PHOTO_TOTAL_SIZE);   

                BitmapCameraInfo.pbData = g_abBmpBuf;  // woodstok 01.4.8
                SKY_CloseDataFile(hFile); 
                //BitmapCameraInfo.Header.yStart = 0;
            
                if(EQS_GET_PARENT() == SKA_PhotoList)
                {
                    clk_secs_to_julian(dwTimeInfo, &julian);
                    fAmPm = (BOOL) ((julian.hour < 12) ? TRUE : FALSE);
                    if(fAmPm == FALSE && julian.hour != 12)
                       julian.hour -= 12;
                    else if(fAmPm == FALSE && julian.hour == 24)
                       julian.hour = 0;

                    sprintf((CHAR*)s_szTimeInfo, " %2d.%02d(%s)%02d:%02d%s", 
                                 julian.month, 
                                 julian.day, 
                                 (CHAR*)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
                                 julian.hour,
                                 julian.minute,
                                 (fAmPm) ? SKY_GET_TOKEN(TKN_IDLE_AM): SKY_GET_TOKEN(TKN_IDLE_PM));
                }
            
                EQC_SetImage( EQS_GetNthIOC(PV_IMAGE), (BITMAP*)&BitmapCameraInfo);

                bStrLen = STRLEN(s_psFileName);
                if(bStrLen  < SKY_MAX_CAMERA_IMAGE_FILE_LEN - 1)
                {
                    j = bStrLen;
                    for(i = (bStrLen/2); j >= 0 ; i--)
                    {
                        if(i == (bStrLen/2))
                        {
                            for(n = i ; n < 9 ; n++ )
                                s_psFileName[10 + n] = ' ';
                            s_psFileName[10 + n] = 0;
                            s_psFileName[9 + i] = ' ';
                            j--;
                        }else
                            s_psFileName[9 + i] = s_psFileName[j--];
                    }    
                
                    for(i = 9 + i; i >= 0 ; i --)
                    {
                        s_psFileName[i] = ' ';
                    }

                    EQC_SetStaticText(EQS_GetNthIOC(PV_FILENAME),s_psFileName);
				}else
                {
                    s_szTemp[0] = 0;
                    STRCAT(s_szTemp,"  ");
                    STRCAT(s_szTemp,s_psFileName);
                    EQC_SetStaticText(EQS_GetNthIOC(PV_FILENAME),s_szTemp);
                }

                if(EQS_GET_PARENT() == SKA_PhotoList)
                {
                    EQC_SetStaticText(EQS_GetNthIOC(PV_TIMEINFO),s_szTimeInfo);
                }
                EQS_SEND_EVENT(SE_APP_DRAW,EQS_DRAW_LEVEL_ALL, 0 );
            }
        }
        break;

    case SE_APP_DRAW :
        //EQS_PutBitmap(PHOTO_DRAW_X, PHOTO_DRAW_Y, &BitmapCameraInfo);
        //EQS_TextOutStr(EQS_NA, 105, s_szTimeInfo); 
        //EQS_TextOutStr(EQS_NA, EQS_NA, s_psFileName); 
        break;

    case SE_SOFTKEY_SELECT:
        dwPhoto = g_PhotoList[s_wIndex].PhotoList;
        EQS_END_EVENT(SE_APP_RESUME, SELECT_POPUP, dwPhoto);
        break;

    case SE_IO_MESGBOX_END:
    case SE_KEY_CLEAR:
        dwPhoto = g_PhotoList[s_wIndex].PhotoList;
        l_Review.fFormReview = TRUE;
        l_Review.wViewIndex = s_wIndex;
        EQS_END_EVENT(SE_APP_RESUME, CLEAR_POPUP, dwPhoto);
        break;

    case SE_APP_END :   /* The very last event */
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);    
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        if(pEvent->wParam== PM_END_POPUP)  //#define PM_END_POPUP 7115 ska_vm.h
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_END_POPUP, EQS_NA);
        }
        break;

    default :
        SKA_BookDefaultProcess(pEvent);
        //EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
/*============================================================================= 
 Function:    
     sSKA_PhotoSave

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
LOCAL void SKY_API sSKA_PhotoSave(SKY_EVENT_T *pEvent)
{
    BYTE pMsgStr[SKY_MAX_CAMERA_IMAGE_FILE_LEN+1];
    HFILE hFile;
    BYTE nCtrl;
    DWORD   dwTimeInfo;
    STATIC HCONTROL s_hSaveDone, s_hSameName;
    WORD        wMaxDataCount = 0, i, j;
    STATIC  BOOL    s_fRev = FALSE;
	UINT k;
    BYTE  szNumber[SKY_MAX_CAMERA_IMAGE_FILE_LEN+1];
    UINT8 nLen;    


    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_CONFIRM_NONE,
            SKY_GET_TOKEN(TKN_SAVE_PHOTO), 
            WA_SHOW_HEADER | WA_SHOW_CML);
    
        pMsgStr[0] = 0;
        s_fRev = pEvent->wParam;
    
        EQC_StaticText(EQS_NA, EQS_NA, TKN_PHOTO_FILE_NAME);
        EQC_TextEdit(EQS_NA, EQS_NA, 16, \
                    1, TEM_KOREAN, CAMERA_NAME_EDIT_MAX_LEN, pMsgStr,0);
        //EQS_SetIOCActive(EQS_GetNthIOC(PHOTO_TEXT));

        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < PHOTO_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        EQS_SetIOCActive(EQS_GetNthIOC(PHOTO_TEXT));

        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_TEDIT_DONE:
        EQC_GetTextEditData(EQS_GetNthIOC(PHOTO_TEXT), pMsgStr);
        pMsgStr[CAMERA_NAME_NULL_POS] = 0;
        if(pMsgStr[0] == 0)
        {
            s_hSameName = EQC_MessageBox(SKY_GET_TOKEN(TKN_NONE_PHOTO_NAME), MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
        }else
        {
            //memcpy(s_aCameraData, CameraCapturedRGB(), PHOTO_TOTAL_SIZE);
            if(s_fRev)
            {
                wMaxDataCount = PHOTO_TOTAL_SIZE/2;
                j = 0;
			    i = wMaxDataCount-1;
			    for(;;)
			    {
				    s_aCameraRevData[j++] = s_aCameraData[i];	
				    if ( i == 0 )	break;
				    i--;
			    }   
                BitmapCameraInfo.pbData = s_aCameraRevData;
            }else
            {
                BitmapCameraInfo.pbData = s_aCameraData;    
            }
            s_hSaveDone = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVING), 0, MBT_NONE,MBT_HELP);
        }
        break;

    case SE_SOFTKEY_SAVE:
        EQS_SetIOCActive(EQS_GetNthIOC(PHOTO_TEXT));
        break;

    case SE_IO_TEDIT_CANCEL:
        EQS_End();
        break;

    case SE_IO_MESGBOX_END:
        EQC_GetTextEditData(EQS_GetNthIOC(PHOTO_TEXT), pMsgStr);
        pMsgStr[CAMERA_NAME_NULL_POS] = 0;
        if(pEvent->wParam == s_hSaveDone)
        {
            BitmapCameraInfo.Header.abID[0] = 'I';
            BitmapCameraInfo.Header.abID[1] = 'M';
            BitmapCameraInfo.Header.abID[2] = 'G';
            BitmapCameraInfo.Header.Size.xWidth = PHOTO_H_SIZE;
            BitmapCameraInfo.Header.Size.yHeight = PHOTO_V_SIZE;
            BitmapCameraInfo.Header.xWidth = PHOTO_H_SIZE;//COLOR_IDLE_IMAGE_WIDTH;
            BitmapCameraInfo.Header.nBitsPerPixel = COLOR_IDLE_IMAGE_BPP;
            BitmapCameraInfo.Header.fPalette = FALSE;
            BitmapCameraInfo.Header.TransparentColor = MAX_COLOR;
            BitmapCameraInfo.Header.yStart = 0;
            //BitmapCameraInfo.pbData = s_aCameraData;

            dwTimeInfo = SKY_GetLocalTime();
            //memset((VOID*)g_abBmpBuf, 0xFF, CAMERA_BUF_SPACE);    //init
            hFile = SKY_MakeCameraImageFile(SKY_GET_CAMERA_COUNT());
            if(hFile != SKY_INVALID_FILE_HANDLE)
            {
                SKY_WriteDataBlock(hFile, pMsgStr, SKY_MAX_CAMERA_IMAGE_FILE_LEN);      //File Name
                SKY_WriteDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));              //Time Info
                SKY_WriteDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));
                //SKY_WriteDataBlock(hFile, g_abBmpBuf, CAMERA_BUF_SPACE);
                SKY_WriteDataBlock(hFile, BitmapCameraInfo.pbData, PHOTO_TOTAL_SIZE);
                //SKY_WriteDataBlock(hFile, g_abBmpBuf, CAMERA_BUF_SPACE);
                SKY_CloseDataFile(hFile);
				 // Change List
                for(k=0 ; k<MAX_PHOTO_COUNT;k++)
                { 
                    if(g_PhotoList[k].PhotoList == NULL)
                    {
                        g_PhotoList[k].PhotoList = SKY_GET_CAMERA_COUNT()-1;
                        STRCPY(g_PhotoList[k].PhotoName, pMsgStr);

                        k = MAX_PHOTO_COUNT;
                    }
                }   
                
                hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
                SKY_WriteFile(hFile, (CONST void *)g_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                SKY_CloseFile(hFile);

                EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            }else
            {
                EQC_MessageBox(SKY_GET_TOKEN(TKN_FAIL_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            }        
        }else if(pEvent->wParam == s_hSameName)
        {
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }else
            EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*============================================================================= 
Function:    
SKA_PhotoListNView

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
void SKY_API SKA_PhotoListNView(SKY_EVENT_T *pEvent)
{
    DWORD  dwPhoto = 0; 
    BYTE psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH+1]; 
    STATIC WORD s_wIdx=0;
    static HCONTROL hControl, s_hMsgControl;
    UINT8 index = 0;
    //STATIC UINT8 s_ScrollIndex = 0;
    HFILE   hFile = NULL;
    HCONTROL  hTmpControl;  
	UINT i;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_VIEW,
            SKY_GET_TOKEN(TKN_PHOTO_LIST), 
            WA_SHOW_HEADER | WA_SHOW_CML);
        s_wIdx = 0;
        s_hMsgControl = 0;
#ifdef WAPPER
        if(SKY_IS_IN_WAP())
        {
            EQS_END_EVENT(SE_APP_RESUME, CLEAR_POPUP, dwPhoto);
            break;
        }
#endif
        SKY_InitCameraList();

        if(nNumOfPhoto != 0)
        {
            //EQS_StopTimer(EQS_TIMER_1);
            for( i = 0 ; i < nNumOfPhoto; i++)
            {
                 l_paListBuf[i] = (BYTE *)SKY_Calloc(SKY_FILE_EXT_NAME_MAX_LENGTH+1, sizeof(BYTE)); 
                 STRCPY(l_paListBuf[i], g_PhotoList[i].PhotoName);
                                  
            }
            
            hControl = EQC_ScrollList(0,0,18,PHOTO_LIST_LINE, \
                            l_paListBuf,nNumOfPhoto,nNumOfPhoto,0);
            EQC_SET_STYLE( hControl, CS_SCROLL_LIST_MAXIMIZE|CS_SCROLL_LIST_ENUMBERATED);
            EQS_SetIOCActive(hControl);
            EQS_SEND_EVENT(SE_SOFTKEY_CONFIRM, EQS_NA, EQS_NA);
            
        }else
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_DATA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);            
        }
        break;
 
    case SE_KEY_END:
        if(EQS_GET_PARENT()==SKA_EditNewPM)
            EQS_END_EVENT(SE_APP_RESUME, PM_END_POPUP, EQS_NA); 
        else
            EQS_END_CURRENT_EVENT();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam != s_hMsgControl)
            EQS_End();
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
    case SE_KEY_CLEAR:
        EQS_END_EVENT(SE_APP_RESUME, CLEAR_POPUP, dwPhoto);
        break;

    case SE_APP_END :   /* The very last event */ 
        for( i = 0 ; i < nNumOfPhoto; i++)
        {
             SKY_Free(l_paListBuf[i]); 
        }
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(pEvent->wParam == SELECT_POPUP)
        {
            dwPhoto = pEvent->dwParam;
            EQS_END_EVENT(SE_APP_RESUME, EQS_NA, dwPhoto);
        }
        else if(pEvent->wParam== PM_END_POPUP)  //#define PM_END_POPUP 7115 ska_vm.h
       {
            EQS_END_EVENT(SE_APP_RESUME, PM_END_POPUP, EQS_NA);
        }
        else
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
      
    case SE_SOFTKEY_CONFIRM:
        //EQS_DeleteControl(s_hMsgControl);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        hControl = EQS_GetNthIOC(0);
        index = EQC_GetScrollLogicalIndex(hControl);
        dwPhoto = g_PhotoList[index].PhotoList;
        EQS_END_EVENT(SE_APP_RESUME, EQS_NA, dwPhoto);
        break;

    case SE_SOFTKEY_VIEW:
        hControl = EQS_GetNthIOC(0);
        index = EQC_GetScrollLogicalIndex(hControl);
        dwPhoto = g_PhotoList[index].PhotoList;
        EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, index , dwPhoto);
        break;

    
    default:
        SKA_BookDefaultProcess(pEvent);
        //EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
/*============================================================================= 
Function:    
SKA_PhotoList

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
#define SMB_SCROLL_LIST_STYLE   CS_SCROLL_LIST_DEFAULT| \
                                CS_SCROLL_LIST_ENUMBERATED | \
                                CS_SCROLL_LIST_MAXIMIZE | \
                                CS_SCROLL_LIST_NO_CML | \
                                CS_SCROLL_LIST_STATUS_BAR

enum tag_SKA_MYPHT_MESSAGEBOX_E {
    SMM_NOANI,
    SMM_DELETE_CONFIRM,
    SMM_DELETE_CONFIRM_ALL,
    SMM_DELETED,
    SMM_RENAME,
    SMM_NO_RENAME,
    SMM_NONE
};

typedef enum tag_SKA_MYPHT_MESSAGEBOX_E tag_SKA_MYPHT_MESSAGEBOX_E;

LOCAL tag_SKA_MYPHT_MESSAGEBOX_E   MessageReason;

void SKY_API SKA_PhotoList(SKY_EVENT_T *pEvent)
{
    DWORD  dwPhoto; 
    BYTE psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH+1]; 
	BYTE psFileList[SKY_FILE_EXT_NAME_MAX_LENGTH+1]; 
    STATIC WORD s_wIdx=0;
    HCONTROL hControl;
    STATIC HCONTROL s_hMsgControl;
    STATIC UINT8 s_index = 0, s_nMyPHT = 0;
    UINT8   index = 0;
    HFILE   hFile = NULL;
    PICTURE_TYPE_T tmpPicture;
    BYTE pszTmpBuffer[20];
    STATIC BOOL fSet = FALSE;
    BYTE pszMessage[MESSAGE_BOX_MAX_MESG_LEN];
    STATIC BOOL    s_fRename = FALSE;
	UINT i;
#ifdef FEATURE_CHARACTER_BELL
    BOOL        isCharacbell = FALSE;
    BYTE        temp[MAX_TITLE_LENGTH+2];
#endif // FEATURE_CHARACTER_BELL

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        //SKY_SET_USE_SHARED_MEMORY(TRUE);
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            //CL_DELETE_NONE_VIEW,
            CL_MENU_BEFOREVIEW_NONE, 
            SKY_GET_TOKEN(TKN_PICTURE_MYPHOTO), //PHOTO_LIST), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        s_fRename = FALSE;

        SKY_InitCameraList();

        if(nNumOfPhoto != 0)
        {            
            for( i = 0 ; i < nNumOfPhoto; i++)
            {
                 l_paListBuf[i] = (BYTE *)SKY_Calloc(SKY_FILE_EXT_NAME_MAX_LENGTH+1, sizeof(BYTE)); 
                 STRCPY(l_paListBuf[i], g_PhotoList[i].PhotoName);
                                  
            }
            
#ifdef FEATURE_CHARACTER_BELL
            for (index = 0; index < nNumOfPhoto; index++) {
                for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) {
                    if (SKY_GET_CB_ITYPE(i) == sMY_PHOTO) {
						ConvertCameraImageFileName(psFileList, g_PhotoList[index].PhotoList); 
                        if (!STRCMP((BYTE*)SKY_GET_CB_ITITLE(i), psFileList)) { 
                            // 캐릭벨에 쓰인 나만의 벨인 경우
                            STRCPY(temp, l_paListBuf[index]);
                            STRNCPY(l_paListBuf[index], (BYTE*)SF_CHARACBELL_1, 2);
                            STRCPY(l_paListBuf[index]+2, temp);
                            break;
                        }
                    }
                }
            } // for
#endif // FEATURE_CHARACTER_BELL

            hControl = EQC_ScrollList(0,0,18,8, \
                        l_paListBuf,nNumOfPhoto,nNumOfPhoto,pEvent->dwParam);
/*
			ConvertCameraImageFileName(psFileList, g_PhotoList[pEvent->dwParam].PhotoList); 
            if(SKY_GetDataFileSize(SDF_PHOTO, psFileList) < 1024)
                sprintf((char*)pszTmpBuffer, 
                        "%s %5ld B", 
                        (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                        SKY_GetDataFileSize(SDF_PHOTO, psFileList));
            else
                sprintf((char*)pszTmpBuffer, 
                        "%s %5ldKB", 
                        (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                        GET_KB_SIZE(SKY_GetDataFileSize(SDF_PHOTO, psFileList)));

            EQC_SetScrollStatusBarText(hControl, pszTmpBuffer);

*/
            EQC_SET_X(hControl, 0);
            //EQC_SET_STYLE(hControl, SMB_SCROLL_LIST_STYLE);
			EQC_SET_STYLE( hControl, EQC_GET_STYLE(hControl) |
                             CS_SCROLL_LIST_ENUMBERATED |
                             CS_SCROLL_LIST_MAXIMIZE );
            EQC_SetScrollLogicalIndex(hControl, pEvent->dwParam);
            EQS_SetIOCActive(hControl);
        }else
        {
            MessageReason = SMM_NOANI;
            EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_DATA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        break;
   

    case SE_APP_END :   /* The very last event */ 
        for( i = 0 ; i < nNumOfPhoto; i++)
        {
             SKY_Free(l_paListBuf[i]); 
        }
        s_index = 0;
        EQS_ReleaseDC();
        break;
    
    case SE_SOFTKEY_CONFIRM:
        EQS_DeleteControl(s_hMsgControl);
        break;

    case SE_SOFTKEY_MENU: 
        {
#ifdef FEATURE_CHARACTER_BELL
            BOOL        fActiveMenu[5] = {FALSE, TRUE, TRUE, TRUE, TRUE};
            UINT8   index;
            BOOL    isCharacbell = FALSE;
#endif // FEATURE_CHARACTER_BELL

            SKY_TOKEN paMenu[5] =
            {
                TKN_PHOTO_DELETE,
                TKN_PHOTO_SET,
                TKN_BELLVIB_MYBELL_RENAME,
                TKN_PICTURE_DETAIL,
                TKN_PHOTO_DELETE_ALL
            };

#ifdef FEATURE_CHARACTER_BELL
            hControl = EQS_GetNthIOC(0);
            s_index = (UINT8)EQC_GetScrollLogicalIndex(hControl);
			ConvertCameraImageFileName(psFileList, g_PhotoList[s_index].PhotoList); 
            STRCPY(psFileName,psFileList);

            for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
                if (!(strcmp(SKY_GET_CB_ITITLE(index), psFileName))){
                    // 여기가 같은게 있는 경우이다.
                    if (SKY_GET_CB_ITYPE(index) == sMY_PHOTO) { 
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
        fSet = FALSE;

        break;

    case SE_KEY_END:
        EQS_END_CURRENT_EVENT();
        break;

///////////////////////////////////////////
    case SE_IO_POPUP_SELECT:
        hControl = EQS_GetNthIOC(0);
        if(fSet)    // 그림지정
        {
            PICTURE_TYPE_T tmpPicture;

            fSet = FALSE;
            s_index = (UINT8)EQC_GetScrollLogicalIndex(hControl);
            MessageReason = SMM_NONE;

            tmpPicture.wPictureID = sMY_PHOTO;
			ConvertCameraImageFileName(psFileList, g_PhotoList[s_index].PhotoList); 
            STRCPY(tmpPicture.szFileName, psFileList);

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
            case TKN_PHOTO_DELETE: // 사진 삭제
#else
            case 0:     // 사진 삭제
#endif // FEATURE_CHARACTER_BELL
                s_index = (UINT8)EQC_GetScrollLogicalIndex(hControl);

                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithoutIcon, EQC_GetScrollData(hControl, s_index));
#ifdef FEATURE_CHARACTER_BELL
                memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithIcon, EQC_GetScrollData(hControl, s_index));
                if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                    memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                    STRCPY(WithoutIcon, WithIcon+2);
                }
#endif // FEATURE_CHARACTER_BELL

                sprintf((char*)pszMessage, "\"%s\" ", WithoutIcon);
                STRNCAT(pszMessage, (BYTE*)SKY_GET_TOKEN(TKN_DELETE_PHOTO),
                            MESSAGE_BOX_MAX_MESG_LEN);

                MessageReason = SMM_DELETE_CONFIRM;
                EQC_MessageBox(pszMessage, 
                                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_PHOTO_SET: // 그림지정하기
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
                s_index = (UINT8)EQC_GetScrollLogicalIndex(hControl);
                
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithoutIcon, EQC_GetScrollData(hControl, s_index));
#ifdef FEATURE_CHARACTER_BELL
                memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithIcon, EQC_GetScrollData(hControl, s_index));
                if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                    memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                    STRCPY(WithoutIcon, WithIcon+2);
                }
#endif // FEATURE_CHARACTER_BELL

                STRCPY(g_szChangedName, WithoutIcon);

                s_fRename = TRUE;
                EQS_START_CHILD_EVENT(SKA_MyAniRename, SE_APP_START, POPUP_MENU_MY_PHOTO, EQS_NA);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_PICTURE_DETAIL: // 파일정보
#else
            case 3: // 파일정보
#endif // FEATURE_CHARACTER_BELL
                s_index = (UINT8)EQC_GetScrollLogicalIndex(hControl);
                EQS_START_CHILD(SKA_MainMyFileInfo);
                break;

#ifdef FEATURE_CHARACTER_BELL
            case TKN_PHOTO_DELETE_ALL: // 사진 전부삭제
#else
            case 4: //사진 전부삭제
#endif // FEATURE_CHARACTER_BELL
                MessageReason = SMM_DELETE_CONFIRM_ALL;
                EQC_MessageBox(SKY_GET_TOKEN(TKN_DELETE_PHOTO_ALL),MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            
            }
        }
        break;
        ////////////////////////////////////////


    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        hControl = EQS_GetNthIOC(0);
		if(s_fRename)
        {
            s_fRename = FALSE;
            if(pEvent->wParam)
            {
                memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithoutIcon, EQC_GetScrollData(hControl, s_index));
#ifdef FEATURE_CHARACTER_BELL
                memset(WithIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                STRCPY(WithIcon, EQC_GetScrollData(hControl, s_index));
                if (!STRNCMP(WithIcon, (BYTE*)SF_CHARACBELL_1, 2)) {
                    memset(WithoutIcon, 0x00, SKY_FILE_NAME_MAX_LENGTH+2);
                    STRCPY(WithoutIcon, WithIcon+2);
                }
#endif // FEATURE_CHARACTER_BELL
                if(STRCMP(WithoutIcon, g_szChangedName))
                {
					ConvertCameraImageFileName(psFileList, g_PhotoList[s_index].PhotoList); 
                    STRCPY(psFileName, psFileList);
                    if(SKY_OpenDataFile(SDF_PHOTO, psFileName, &hFile, SFT_WRITE) == SDR_OK)
                    {
                        SKY_WriteDataBlock(hFile, g_szChangedName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                        SKY_CloseDataFile(hFile);
                        
                        STRCPY(g_PhotoList[s_index].PhotoName, g_szChangedName);

                        //change list
                        hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
                        SKY_WriteFile(hFile, (CONST void *)g_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                        SKY_CloseFile(hFile);

                        MessageReason = SMM_RENAME;
                        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAMING), 
                                        0, MBT_NONE, MBT_HELP);

                    }else
                    {
                        MessageReason = SMM_NO_RENAME;
                        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAMING), 
                            0, MBT_NONE, MBT_HELP);
                    }
            
                }
                else
                    EQS_SetIOCActive(hControl);
            }else
                EQS_SetIOCActive(hControl);
        }
        else
        {
            if(l_Review.fFormReview)
            {
                l_Review.fFormReview = FALSE;
                s_index = l_Review.wViewIndex;
            }
/*			
			ConvertCameraImageFileName(psFileList, g_PhotoList[s_index].PhotoList); 
			if(SKY_GetDataFileSize(SDF_PHOTO, psFileList) < 1024)
                sprintf((char*)pszTmpBuffer, 
                        "%s %5ld B", 
                        (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                        SKY_GetDataFileSize(SDF_PHOTO, psFileList));
            else
                sprintf((char*)pszTmpBuffer, 
                        "%s %5ldKB", 
                        (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                        GET_KB_SIZE(SKY_GetDataFileSize(SDF_PHOTO, psFileList)));

            EQC_SetScrollStatusBarText(hControl, pszTmpBuffer);
*/
            EQC_SetScrollLogicalIndex(hControl, s_index);
            EQS_SetIOCActive(hControl);

			
        }
        break;
        
    case SE_IO_MESGBOX_CLEAR:
    case SE_IO_MESGBOX_OK:
    case SE_IO_MESGBOX_END:
        ////////////////////////////
        switch(MessageReason){
        case SMM_NOANI:
            EQS_End();
            break;

        case SMM_RENAME:
            hControl = EQS_GetNthIOC(0);
            
            //SKY_RenameDataFile(SDF_IMAGE, EQC_GetScrollData(hControl, s_index), g_szChangedName);
            // 보고
            // 아래에 내용들이 왜 들어가있는지 모르겠다.. 그림친구 소스인 듯..
            // 나만의 사진 관리에서는 아래 부분이 필요없다.
            tmpPicture.wPictureID = sMY_PICTURE;
            STRCPY(tmpPicture.szFileName, g_szChangedName);
            
            // 대기중
            if(IS_MY_PICTURE(SKY_GET_IDLE_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_IDLE_PICTURE().szFileName, EQC_GetScrollData(hControl, s_index)))
                    SKY_SET_IDLE_PICTURE(tmpPicture);
            }
            // 시작시
            if(IS_MY_PICTURE(SKY_GET_POWER_ON_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_POWER_ON_PICTURE().szFileName, EQC_GetScrollData(hControl, s_index)))
                    SKY_SET_POWER_ON_PICTURE(tmpPicture);
            }
            // 종료시
            if(IS_MY_PICTURE(SKY_GET_POWER_OFF_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_POWER_OFF_PICTURE().szFileName, EQC_GetScrollData(hControl, s_index)))
                    SKY_SET_POWER_OFF_PICTURE(tmpPicture);
            }
            // 전화왔을때
            if(IS_MY_PICTURE(SKY_GET_INCOMING_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_INCOMING_PICTURE().szFileName, EQC_GetScrollData(hControl, s_index)))
                    SKY_SET_INCOMING_PICTURE(tmpPicture);
            }

            // N.TOP 시작시
            if(IS_MY_PICTURE(SKY_GET_WAP_PICTURE().wPictureID))
            {
                if(!STRCMP(SKY_GET_WAP_PICTURE().szFileName, EQC_GetScrollData(hControl, s_index)))
                    SKY_SET_WAP_PICTURE(tmpPicture);
            }
            
            MessageReason = SMM_DELETED;
            EQS_SetIOCActive(hControl);
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MYBELL_RENAMED), 
                            1, MBT_NONE, MBT_SUCCESS);
            break;

        case SMM_DELETED:
            SKY_InitCameraList();
            if(nNumOfPhoto == 0)
            {
                EQS_END();
            }else
            {            
                for( i = 0 ; i < nNumOfPhoto; i++)
                {
                     l_paListBuf[i] = (BYTE *)SKY_Calloc(SKY_FILE_EXT_NAME_MAX_LENGTH+1, sizeof(BYTE)); 
                     STRCPY(l_paListBuf[i], g_PhotoList[i].PhotoName);
                              
                }

#ifdef FEATURE_CHARACTER_BELL
                for (index = 0; index < nNumOfPhoto; index++) {
                    for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) {
                        if (SKY_GET_CB_ITYPE(i) == sMY_PHOTO) {
							ConvertCameraImageFileName(psFileList, g_PhotoList[index].PhotoList); 
                            if (!STRCMP((BYTE*)SKY_GET_CB_ITITLE(i), psFileList)) { 
                                // 캐릭벨에 쓰인 나만의 벨인 경우
                                STRCPY(temp, l_paListBuf[index]);
                                STRNCPY(l_paListBuf[index], (BYTE*)SF_CHARACBELL_1, 2);
                                STRCPY(l_paListBuf[index]+2, temp);
                                break;
                            }
                        }
                    }
                } // for
#endif // FEATURE_CHARACTER_BELL                
                hControl = EQC_ScrollList(0,0,18,8, \
                            l_paListBuf,nNumOfPhoto,nNumOfPhoto,pEvent->dwParam);

/*
				ConvertCameraImageFileName(psFileList, g_PhotoList[pEvent->dwParam].PhotoList); 
                if(SKY_GetDataFileSize(SDF_PHOTO, psFileList) < 1024)
                    sprintf((char*)pszTmpBuffer, 
                            "%s %5ld B", 
                            (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                            SKY_GetDataFileSize(SDF_PHOTO, psFileList));
                else
                    sprintf((char*)pszTmpBuffer, 
                            "%s %5ldKB", 
                            (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                            GET_KB_SIZE(SKY_GetDataFileSize(SDF_PHOTO, psFileList)));

                EQC_SetScrollStatusBarText(hControl, pszTmpBuffer);
*/

                EQC_SET_X(hControl, 0);
                //EQC_SET_STYLE(hControl, SMB_SCROLL_LIST_STYLE);
				EQC_SET_STYLE( hControl, EQC_GET_STYLE(hControl) |
                             CS_SCROLL_LIST_ENUMBERATED |
                             CS_SCROLL_LIST_MAXIMIZE );
				EQS_SetIOCActive(hControl);
                                
                if(nNumOfPhoto>0) 
				{
					if(s_index > EQC_GetScrollMaxDataLine(hControl) -1)
						s_index = EQC_GetScrollMaxDataLine(hControl) -1;
				
					EQC_SetScrollLogicalIndex(hControl , s_index);
					
				}	
                
                                
            }
            //EQS_START_SIBLING_EVENT( SKA_PhotoList, SE_APP_START, EQS_NA , s_index, FALSE );
            break;

        case SMM_NONE:
        case SMM_NO_RENAME:
            hControl = EQS_GetNthIOC(0);
            EQS_SetIOCActive(hControl);
            break;
        }
        ////////////////////////////
        break;

    case SE_IO_POPUP_CANCEL:
        hControl = EQS_GetNthIOC(0);
        EQS_SetIOCActive(hControl);
        break;

    case SE_SOFTKEY_DELETE:
        EQC_MessageBox(SKY_GET_TOKEN(TKN_DELETE_PHOTO), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;
    
    case SE_IO_MESGBOX_YES:
        if(MessageReason != SMM_DELETE_CONFIRM && MessageReason != SMM_DELETE_CONFIRM_ALL )
            break;
        
        if(MessageReason == SMM_DELETE_CONFIRM)
        {
            hControl = EQS_GetNthIOC(0);
            s_index = EQC_GetScrollLogicalIndex(hControl);
            
            //SKY_DeletePMPhoto(atol(PhotoList[s_index]));
			ConvertCameraImageFileName(psFileList, g_PhotoList[s_index].PhotoList); 
            STRCPY(psFileName, psFileList);
    //        STRCAT(psFileName, ".ebi");
            //pbFileName = EQC_GetScrollData(hControl, index);
            tmpPicture = SKY_GET_IDLE_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO && !STRCMP(tmpPicture.szFileName, psFileName))
            {
                tmpPicture.wPictureID = INIT_IDLE_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_IDLE_PICTURE(tmpPicture);

                SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);

                SKY_SET_BANNER(INIT_BANNER);
                SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);
            }
        
            tmpPicture = SKY_GET_INCOMING_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO && !STRCMP(tmpPicture.szFileName, psFileName))
            {
                tmpPicture.wPictureID = INIT_INCOMING_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_INCOMING_PICTURE(tmpPicture);
            }
        
            tmpPicture = SKY_GET_POWER_ON_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO && !STRCMP(tmpPicture.szFileName, psFileName))
            {
                tmpPicture.wPictureID = INIT_POWER_ON_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_POWER_ON_PICTURE(tmpPicture);
            }
        
            tmpPicture = SKY_GET_POWER_OFF_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO && !STRCMP(tmpPicture.szFileName, psFileName))
            {
                tmpPicture.wPictureID = INIT_POWER_OFF_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_POWER_OFF_PICTURE(tmpPicture);
            }

            tmpPicture = SKY_GET_WAP_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO && !STRCMP(tmpPicture.szFileName, psFileName))
            {
                tmpPicture.wPictureID = INIT_WAP_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_WAP_PICTURE(tmpPicture);
            }
		    
		    SKY_DeleteDataFile(SDF_PHOTO, psFileName);

            // Change List
            if(s_index !=49)
            {	
				for(index = s_index; index < nNumOfPhoto-1;index++)
				{
					g_PhotoList[index] = g_PhotoList[index+1];
                }
				memset(&g_PhotoList[nNumOfPhoto-1],NULL,sizeof(PHOTO_LIST_T));
            }
            else
            {
                memset(&g_PhotoList[s_index],NULL,sizeof(PHOTO_LIST_T));
            }
            
            hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
            SKY_WriteFile(hFile, (CONST void *)g_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
            SKY_CloseFile(hFile);
 
            
            if(IS_MY_PHOTO_ALL(SKY_GET_IDLE_PICTURE().wPictureID) && nNumOfPhoto == 0)
            {
                tmpPicture.wPictureID = INIT_IDLE_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_IDLE_PICTURE(tmpPicture);

                SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);

                SKY_SET_BANNER(INIT_BANNER);
                SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);                            
            }
            if(nNumOfPhoto != 0 )
                EQC_SetScrollLogicalIndex(hControl, 0);
            EQC_MessageBox(SKY_GET_TOKEN(TKN_DELETE_DONE_PHOTO), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(MessageReason == SMM_DELETE_CONFIRM_ALL)
        {
            hControl = EQS_GetNthIOC(0);
            tmpPicture = SKY_GET_IDLE_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO )
            {
                tmpPicture.wPictureID = INIT_IDLE_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_IDLE_PICTURE(tmpPicture);

                SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);

                SKY_SET_BANNER(INIT_BANNER);
                SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);
            }
        
            tmpPicture = SKY_GET_INCOMING_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO )
            {
                tmpPicture.wPictureID = INIT_INCOMING_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_INCOMING_PICTURE(tmpPicture);
            }
        
            tmpPicture = SKY_GET_POWER_ON_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO )
            {
                tmpPicture.wPictureID = INIT_POWER_ON_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_POWER_ON_PICTURE(tmpPicture);
            }
        
            tmpPicture = SKY_GET_POWER_OFF_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO )
            {
                tmpPicture.wPictureID = INIT_POWER_OFF_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_POWER_OFF_PICTURE(tmpPicture);
            }

            tmpPicture = SKY_GET_WAP_PICTURE();
            if(tmpPicture.wPictureID == sMY_PHOTO )
            {
                tmpPicture.wPictureID = INIT_WAP_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_WAP_PICTURE(tmpPicture);
            }
#ifdef FEATURE_CHARACTER_BELL
            {
                UINT8   i, j;
                BOOL    isCharacbell = FALSE, isExist = FALSE;

                for(i = 0; i < nNumOfPhoto; i++) {
                    for (j = 0; j < MAX_NUM_OF_CHARACBELL; j++) {
						ConvertCameraImageFileName(psFileList, g_PhotoList[i].PhotoList); 
                        if(!(strcmp(psFileList, SKY_GET_CB_ITITLE(j)))) {
                            // 같은게 있는 경우
                            if (SKY_GET_CB_ITYPE(j) == sMY_PHOTO) { 
                                isCharacbell = TRUE;
                                break;
                            } // 다른 곳에 같은 이름의 파일이 존재할 경우
                        }

                    }
                    if (!isCharacbell) { // 여기에 해당 사진을 지우는 일을 넣어야쥐
                        s_index = i;
                        
                        //SKY_DeletePMPhoto(atol(PhotoList[s_index]));
						ConvertCameraImageFileName(psFileList, g_PhotoList[s_index].PhotoList); 
                        STRCPY(psFileName, psFileList);
                        //STRCAT(psFileName, ".ebi");
                        //pbFileName = EQC_GetScrollData(hControl, index);
                        SKY_DeleteDataFile(SDF_PHOTO, psFileName);

                        // Change List
                        if(s_index !=49)
                        {
                            for(index = s_index; index < nNumOfPhoto-1;index++)
							{
								g_PhotoList[index] = g_PhotoList[index+1];
							}
							memset(&g_PhotoList[nNumOfPhoto-1],NULL,sizeof(PHOTO_LIST_T));
                        }
                        else
                        {
                            memset(&g_PhotoList[s_index],NULL,sizeof(PHOTO_LIST_T));
                        }
                        i--;
                        nNumOfPhoto--;
                        
                        hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
                        SKY_WriteFile(hFile, (CONST void *)g_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                        SKY_CloseFile(hFile);
 
                        
                        if(IS_MY_PHOTO_ALL(SKY_GET_IDLE_PICTURE().wPictureID) && nNumOfPhoto == 0)
                        {
                            tmpPicture.wPictureID = INIT_IDLE_PICTURE;
                            STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                            SKY_SET_IDLE_PICTURE(tmpPicture);

                            SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                            SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);

                            SKY_SET_BANNER(INIT_BANNER);
                            SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);                            
                        }
                    }
                    else {
                        isCharacbell = FALSE;
                        isExist = TRUE;
                    }
                }
                if (isExist) {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CHARACBELL_NOTDELETEPHOTO), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                }
                else {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_DELETE_DONE_PHOTO), 
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                }
                if(nNumOfPhoto != 0 )
                    EQC_SetScrollLogicalIndex(hControl, 0);
            }
#else
		    SKY_DeleteDataFiles(SDF_PHOTO);

            nNumOfPhoto = 0;
    
            // Create file
            hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);

            memset(g_PhotoList, NULL, sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
    
            SKY_WriteFile(hFile, (CONST void *)g_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
            
            SKY_CloseFile(hFile);

            

            if(IS_MY_PHOTO_ALL(SKY_GET_IDLE_PICTURE().wPictureID))
            {
                tmpPicture.wPictureID = INIT_IDLE_PICTURE;
                STRCPY((BYTE *)tmpPicture.szFileName, (BYTE*)"");
                SKY_SET_IDLE_PICTURE(tmpPicture);

                SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);

                SKY_SET_BANNER(INIT_BANNER);
                SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);                            
            }
            EQC_MessageBox(SKY_GET_TOKEN(TKN_DELETE_DONE_PHOTO), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
#endif // FEATURE_CHARACTER_BELL 
        }

        MessageReason = SMM_DELETED;
        break;

    case SE_IO_MESGBOX_NO:
        break;

    case SE_IO_SCROLL_LIST_CHG:
/*
        index = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
		ConvertCameraImageFileName(psFileList, g_PhotoList[index].PhotoList); 
        if(SKY_GetDataFileSize(SDF_PHOTO,psFileList) < 1024)
            sprintf((char*)pszTmpBuffer, 
                    "%s %5ld B", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    SKY_GetDataFileSize(SDF_PHOTO, psFileList));
        else
            sprintf((char*)pszTmpBuffer, 
                    "%s %5ldKB", 
                    (BYTE*)SKY_GET_TOKEN(TKN_BELLVIB_MYBELL_FILESIZE),
                    GET_KB_SIZE(SKY_GetDataFileSize(SDF_PHOTO,psFileList)));

        EQC_SetScrollStatusBarText(EQS_GetNthIOC(0), pszTmpBuffer);
*/
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        hControl = EQS_GetNthIOC(0);
        index = EQC_GetScrollLogicalIndex(hControl);
        s_index = index;
        dwPhoto = g_PhotoList[index].PhotoList;
        l_Review.fFormReview = TRUE;
        l_Review.wViewIndex = s_index;
        EQS_START_CHILD_EVENT(SKA_PhotoViewer, SE_APP_START, index , dwPhoto);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
        {
            EQS_End();
        }
        break;

    default:
        SKA_BookDefaultProcess(pEvent);//EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
