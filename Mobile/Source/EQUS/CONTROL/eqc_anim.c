/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_anim.c
**
** Description: 
**     Animation Pictures
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-09-26   redstar    edit
** 00-08-03   girin      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"


/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define DEFAULT_DISP_MILLI_SECOND   250

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagANIMATION_T  ANIMATION_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagANIMATION_T
{
    CONTROL_HEADER_T  Header;
    ANIMATION_INFO_T  *pAnimationInfo;
    UINT8             nIndex;
    UINT8             nRepeat;
    COORD             yStart;
    //COORD             wStartPos;
    COORD             yHeight; /* 각 프레임에서 일부만 보여주고 싶을 때 사용 */
    BYTE*             pszPathFileName; /* full path file name */
};
//	ANIM_CUT          *Anim;

#define GET_ANI_INFO(pAnimation)            (pAnimation)->pAnimationInfo
#define GET_DISP_MILLI_SECOND(pAnimation, i)\
                            GET_ANI_INFO(pAnimation)->AnimationHeader.awDelayTime[i]//dwDispMilliSecond
#define GET_INDEX(pAnimation)               (pAnimation)->nIndex

#define GET_BITMAP_DATA_POS(pAnimation) \
                            GET_ANI_INFO(pAnimation)->pbData \
                            + EQC_GET_FRAME_SIZE(\
                                    GET_ANI_INFO(pAnimation)->BitmapHeader)\
                              * pAnimation->nIndex 


#define GET_FRAME_COUNT(pAnimation)     GET_ANI_INFO(pAnimation)->AnimationHeader.nFrame


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
LOCAL BITMAP l_FrameBitmap; /* local global */
LOCAL HTIMER l_hTimer = INVALID_TIMER_HANDLE;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(ANIMATION_INFO_T *pAnimationInfo, BYTE* pszPathFileName);
LOCAL void  EQS_API sHandleInstance(HCONTROL hControl, EQS_EVENT* pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL BITMAP* EQS_API sGetFrameBitmap(ANIMATION_T *pAnimation);
            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

LOCAL void* EQS_API sAllocInstance(ANIMATION_INFO_T *pAnimationInfo, BYTE* pszPathFileName)
{
	ANIMATION_T *pAnimation;

	pAnimation = (ANIMATION_T *)EQS_Malloc(sizeof(ANIMATION_T));
    pAnimation->pAnimationInfo = (ANIMATION_INFO_T *)EQS_Malloc(sizeof(ANIMATION_INFO_T));
    if(pszPathFileName != NULL && pszPathFileName[0] != NULL)
    {
        pAnimation->pszPathFileName = (BYTE *)EQS_Malloc(STRLEN(pszPathFileName)+1);
        STRCPY(pAnimation->pszPathFileName, pszPathFileName);
    }
    else
    {
        pAnimation->pszPathFileName = NULL; /* not memory allocation flag */
    }

    /* no display time. set it as the default value */
    if(pAnimationInfo->AnimationHeader.nFrame > 1 
        && pAnimationInfo->AnimationHeader.awDelayTime[0] == 0)
    {
        INT8 i;

        for(i = 0; i < pAnimationInfo->AnimationHeader.nFrame; i++)
            pAnimationInfo->AnimationHeader.awDelayTime[i] = DEFAULT_DISP_MILLI_SECOND;
    }

    /* copy the contents of the pAnimationInfo structure */
    *pAnimation->pAnimationInfo = *pAnimationInfo;
    pAnimation->nIndex   = 0;
    pAnimation->nRepeat  = 0;
    pAnimation->yStart = 0;
    pAnimation->yHeight = pAnimationInfo->BitmapHeader.Size.yHeight;

	return (void *)pAnimation;
}

LOCAL void EQS_API sHandleInstance(HCONTROL hControl, EQS_EVENT* pEvent)
{
	ANIMATION_T *pAnimation;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_ANIMATION);

	pAnimation = (ANIMATION_T *) EQC_GET_INSTANCE( hControl );
    
	
	if (pEvent->EventID == EQE_APP_DRAW && ((hControl = EQS_GetModal()) == INVALID_CONTROL_HANDLE))
	{
        BITMAP *pBitmap = sGetFrameBitmap(pAnimation);
        if(pBitmap)
        {
            EQS_PutBitmap(pAnimation->Header.x, pAnimation->Header.y, pBitmap);
            EQS_UpdateControlDisplay();  
        }
	}
    else if (pEvent->EventID == EQE_TM_TIMER1)
    {
        pAnimation->nIndex++;
        if(pAnimation->nIndex < GET_FRAME_COUNT(pAnimation))
        {

            EQS_SendEvent(EQE_APP_DRAW, (WORD)EQS_DRAW_LEVEL_APP, /*EQS_DRAW_LEVEL_ALL, */EQS_NA);

            l_hTimer = EQS_StartControlTimer(hControl, EQS_TIMER_1, 
                                      GET_DISP_MILLI_SECOND(pAnimation, pAnimation->nIndex), 
                                      FALSE);
        }
        else
        {
            pAnimation->nRepeat++;
            EQS_SendEvent(EQE_IO_ANIM_TURN_AROUND, (WORD)hControl, (DWORD)pAnimation->nRepeat);

            if ((GET_FRAME_COUNT(pAnimation) > 1) && (EQC_GET_STYLE(hControl) & CS_ANIMATION_REPEAT))
            {
                pAnimation->nIndex = 0;
                EQS_SendEvent(EQE_APP_DRAW, (WORD)EQS_DRAW_LEVEL_APP/*EQS_DRAW_LEVEL_ALL*/, EQS_NA);

                l_hTimer = EQS_StartControlTimer(hControl, EQS_TIMER_1, 
                                      GET_DISP_MILLI_SECOND(pAnimation, pAnimation->nIndex), 
                                      FALSE);
            }
            else
            {
                pAnimation->nIndex = 0;
            }
        }
    }
    else if (pEvent->EventID == EQE_IO_DELETE)
    {
        // Send ending event you want..
        // Free instance
        sFreeInstance(hControl);

    }

}

LOCAL void EQS_API sFreeInstance(HCONTROL hControl)
{
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);
    if (l_hTimer != INVALID_TIMER_HANDLE)
    {
        EQS_StopControlTimer(hControl, EQS_TIMER_1);
        l_hTimer = INVALID_TIMER_HANDLE;
    }

    if(((ANIMATION_T*)EQC_GET_INSTANCE(hControl))->pszPathFileName)
        EQS_Free((void**)&((ANIMATION_T*)EQC_GET_INSTANCE(hControl))->pszPathFileName);
    EQS_Free((void**)&((ANIMATION_T*)EQC_GET_INSTANCE(hControl))->pAnimationInfo);
	EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);

}

#include "sky_efs.h"
#define SKY_API
#ifdef WIN32
#include "skywefs.h"
#else
#include "skyrefs.h"
#endif

#define MAX_BITMAP_WIDTH        128
#define MAX_BITMAP_HEIGHT       160
#define MAX_BITMAP_BITPERPIXEL  16

LOCAL BYTE l_abBuffer[MAX_BITMAP_WIDTH*MAX_BITMAP_HEIGHT*MAX_BITMAP_BITPERPIXEL/8];

LOCAL CONST BYTE* sGetBitmapData(ANIMATION_T* pAnimation)
{
    WORD wFrameSize;
    UINT8 i;
    BYTE* pbData;

    wFrameSize = 0;
    pbData = (BYTE*)GET_ANI_INFO(pAnimation)->pbData;

    if(IS_EQUS_X_IMAGE(&(GET_ANI_INFO(pAnimation)->BitmapHeader)) 
       || IS_EQUS_X_ANIMATION(&(GET_ANI_INFO(pAnimation)->BitmapHeader)))
    {
        for(i = 0; i < pAnimation->nIndex; i++)
        {
            wFrameSize = ((*(pbData)) << 8) + *(pbData+1);
            /* sizeof(WORD) is for frame size data */
            pbData += wFrameSize + sizeof(WORD);
        }
    }
	/* LZB 압축 - haengeun  2001-09-26 --> */
	else if(IS_EQUS_Z_IMAGE(&(GET_ANI_INFO(pAnimation)->BitmapHeader)) 
       || IS_EQUS_Z_ANIMATION(&(GET_ANI_INFO(pAnimation)->BitmapHeader))) 
	{  
	  for(i = 0; i < pAnimation->nIndex; i++)
      {
            wFrameSize = ((*(pbData)) << 8) + *(pbData+1);
            /* sizeof(WORD) is for frame size data */
            pbData += wFrameSize + sizeof(WORD);
      }
    }	        
    /* <-- */
    else /* not packed data */
    {
        pbData = (BYTE*)GET_BITMAP_DATA_POS(pAnimation);
    }
    return pbData;
}

LOCAL CONST BYTE* sGetBitmapDataFromFile(ANIMATION_T* pAnimation)
{
    HFILE hFile;
    DWORD dwFramePos;
    WORD wFrameSize;
    UINT8 i;
    BYTE* pbData;

    dwFramePos = wFrameSize = i = 0;
    
    hFile = SKY_OpenFile(pAnimation->pszPathFileName, FS_AM_READONLY);
    if(hFile == SKY_INVALID_FILE_HANDLE)
        return NULL;

    if(IS_EQUS_X_IMAGE(&(GET_ANI_INFO(pAnimation)->BitmapHeader)) 
       || IS_EQUS_X_ANIMATION(&(GET_ANI_INFO(pAnimation)->BitmapHeader)))
    {
        /* get the frame position */
        do 
        {
            SKY_SeekReadFile(hFile, FS_SO_SET, 
                             sizeof(BITMAP_HEADER) + sizeof(ANIMATION_HEADER_T)
                             + dwFramePos,// + pAnimation->wStartPos, 
                             &wFrameSize, 
                             sizeof(WORD));
            dwFramePos += wFrameSize + sizeof(WORD);
        } while(i++ < pAnimation->nIndex);

        /* read the packed frame data */
        SKY_SeekReadFile(hFile, FS_SO_SET, 
                         sizeof(BITMAP_HEADER) + sizeof(ANIMATION_HEADER_T)
                         + (dwFramePos-wFrameSize-sizeof(WORD)),// + pAnimation->wStartPos, 
                         l_abBuffer, 
                         wFrameSize+sizeof(WORD));

    }
    else /* not packed data */
    {
        wFrameSize = EQC_GET_FRAME_SIZE(GET_ANI_INFO(pAnimation)->BitmapHeader);

        SKY_SeekReadFile(hFile, FS_SO_SET, 
                         sizeof(BITMAP_HEADER) + sizeof(ANIMATION_HEADER_T)
                         + (wFrameSize * pAnimation->nIndex),// + pAnimation->wStartPos, 
                         l_abBuffer, 
                         wFrameSize);
    }
    pbData = l_abBuffer;
    SKY_CloseFile(hFile);
    return pbData;
}


LOCAL BITMAP* EQS_API sGetFrameBitmap(ANIMATION_T *pAnimation)
{
    l_FrameBitmap.Header = pAnimation->pAnimationInfo->BitmapHeader;
    l_FrameBitmap.Header.yStart = pAnimation->yStart;
    l_FrameBitmap.Header.Size.yHeight = pAnimation->yHeight;

    if(pAnimation->pszPathFileName)
    {
        l_FrameBitmap.pbData = sGetBitmapDataFromFile(pAnimation);
        /*HFILE hFile;
        DWORD dwFrameSize;

        dwFrameSize = EQC_GET_FRAME_SIZE(GET_ANI_INFO(pAnimation)->BitmapHeader);

        hFile = SKY_OpenFile(pAnimation->pszPathFileName, FS_AM_READONLY);
        SKY_SeekReadFile(hFile, FS_SO_SET, 
                         sizeof(BITMAP_HEADER) + sizeof(ANIMATION_HEADER_T)
                         + (dwFrameSize * pAnimation->nIndex) + pAnimation->wStartPos, 
                         l_abBuffer, 
                         dwFrameSize);
        l_FrameBitmap.Header = pAnimation->pAnimationInfo->BitmapHeader;
        l_FrameBitmap.pbData = l_abBuffer;
        SKY_CloseFile(hFile);*/
    }
    else
    {
        l_FrameBitmap.pbData = sGetBitmapData(pAnimation);//(BYTE*)GET_BITMAP_DATA_POS(pAnimation);
    }
    return (l_FrameBitmap.pbData == NULL) ? NULL : &l_FrameBitmap;
}

                           
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

HCONTROL EQS_API EQC_Animation(COORD x, COORD y, \
                               ANIMATION_INFO_T CONST *pAnimationInfoA,
                               BYTE* pszPathFileName)
{
	void *pInstance;
	HCONTROL hControl;
    ANIMATION_T *pAnimation;
    ANIMATION_INFO_T AniInfo;

    if(pAnimationInfoA)
    {
        AniInfo = *pAnimationInfoA;
    }
    else
    {
        HFILE hFile;

        hFile = SKY_OpenFile(pszPathFileName, FS_AM_READONLY);
        SKY_ReadFile(hFile, &AniInfo.BitmapHeader, sizeof(BITMAP_HEADER));
        SKY_ReadFile(hFile, &AniInfo.AnimationHeader, sizeof(ANIMATION_HEADER_T));
        SKY_CloseFile(hFile);
    }

	pInstance = (ANIMATION_T *)sAllocInstance(&AniInfo, pszPathFileName);
                             
	hControl = EQC_AllocControl(CT_ANIMATION, pInstance, x, y, 
                                AniInfo.BitmapHeader.Size.xWidth, 
                                AniInfo.BitmapHeader.Size.yHeight, 
                                sHandleInstance);


    ASSERT(hControl != INVALID_CONTROL_HANDLE);

    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SET_STYLE(hControl, CS_ANIMATION_DEFAULT);
    
    l_FrameBitmap.Header = AniInfo.BitmapHeader;

	EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);

    pAnimation = (ANIMATION_T *) EQC_GET_INSTANCE( hControl );

    return hControl;
}

void EQS_API EQC_StartAnimation(HCONTROL hControl, BOOL fRepeat)
{
	ANIMATION_T *pAnimation;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_ANIMATION);

	pAnimation = (ANIMATION_T *) EQC_GET_INSTANCE( hControl );

    if( fRepeat == TRUE )
    {
        EQC_SET_STYLE(hControl, 
                      EQC_GET_STYLE(hControl) | CS_ANIMATION_REPEAT );
        pAnimation->nRepeat = 0;
    }
    else
    {
        EQC_SET_STYLE(hControl, 
                      EQC_GET_STYLE(hControl) & ~CS_ANIMATION_REPEAT );
    }
    //if(sGetFrameBitmap(pAnimation))
    //{
    pAnimation->nIndex = 0;
    //}

    //if(pAnimation->pAnimationInfo->AnimationHeader.nFrame > 1)
    l_hTimer = EQS_StartControlTimer(hControl, EQS_TIMER_1, 
                          GET_DISP_MILLI_SECOND(pAnimation, 0), FALSE);
}

void EQS_API EQC_StopAnimation(HCONTROL hControl)
{
    EQS_StopControlTimer(hControl, l_hTimer);//EQS_TIMER_1);
    l_hTimer = INVALID_TIMER_HANDLE;
}

/* 애니메이션의 각 비트맵 프레임 상에서 시작 위치를 정한다. 
   애니메이션 화면 중 일부 만을 보여주기 위해 사용한다. --> PutBitmap 기능으로 옮긴다. */
void EQS_API EQC_SetStartYAnimation(HCONTROL hControl, COORD yStart)
{
	ANIMATION_T *pAnimation;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_ANIMATION);
	pAnimation = (ANIMATION_T *) EQC_GET_INSTANCE( hControl );
    
    pAnimation->yStart = yStart;
}


void EQS_API EQC_SetAnimationHeight(HCONTROL hControl, COORD yHeight)
{
	ANIMATION_T *pAnimation;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_ANIMATION);
	pAnimation = (ANIMATION_T *) EQC_GET_INSTANCE( hControl );
    
    pAnimation->yHeight = yHeight;
    EQC_SET_DY(hControl, yHeight);

    l_FrameBitmap.Header.Size.yHeight = yHeight;
}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
