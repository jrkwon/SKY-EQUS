/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_aicn.c
**
** Description: 
**     *pAnimationInfoated Icon control
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-10-12   redstar    edit
** 00-08-05   girin      Created.
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

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagACTIVE_ICON_T    ACTIVE_ICON_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagACTIVE_ICON_T
{
    CONTROL_HEADER_T    Header;
	ANIMATION_INFO_T    *pAnimationInfo;
    EQS_EVENT_ID        SoftKey;
    UINT8               nIndex;
};

#define GET_ANI_INFO(pActiveIcon) \
                                (pActiveIcon)->pAnimationInfo
#define GET_DISP_MILLI_SECOND(pActiveIcon, i)\
                                GET_ANI_INFO(pActiveIcon)->AnimationHeader.awDelayTime[i]//dwDispMilliSecond
#define GET_INDEX(pActiveIcon)  (pActiveIcon)->nIndex

#define GET_BITMAP_DATA_POS(pActiveIcon) \
                            GET_ANI_INFO(pActiveIcon)->pbData \
                            + EQC_GET_FRAME_SIZE(\
                                    GET_ANI_INFO(pActiveIcon)->BitmapHeader)\
                              * pActiveIcon->nIndex

#define GET_FRAME_COUNT(pActiveIcon)    GET_ANI_INFO(pActiveIcon)->AnimationHeader.nFrame


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

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(ANIMATION_INFO_T *pAnimationInfo, 
                                   EQS_EVENT_ID SoftKey);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL BITMAP* EQS_API sGetFrameBitmap(ACTIVE_ICON_T *pActiveIcon);
            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

LOCAL void* EQS_API sAllocInstance(ANIMATION_INFO_T *pAnimationInfo, 
                                   EQS_EVENT_ID SoftKey)
{
	ACTIVE_ICON_T *pActiveIcon;

	pActiveIcon = (ACTIVE_ICON_T *)EQS_Malloc(sizeof(ACTIVE_ICON_T));

    pActiveIcon->pAnimationInfo = pAnimationInfo;
    pActiveIcon->SoftKey = SoftKey;
    pActiveIcon->nIndex = 0;

	return (void *)pActiveIcon;
}

void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	ACTIVE_ICON_T *pActiveIcon;
    //COLOR ColorSav;

	ASSERT( EQC_GET_TYPE(hControl) == CT_ACTIVE_ICON);

	pActiveIcon = (ACTIVE_ICON_T *) EQC_GET_INSTANCE(hControl) ;
    	
    // Note that EQE_APP_DRAW and EQE_TM_IOC event is received by I/O Controls even though they have no focus.
	if (pEvent->EventID == EQE_APP_DRAW)
	{
        if( EQS_HaveFocus(hControl) == TRUE )
        {
            if (sGetFrameBitmap(pActiveIcon) != NULL)
            {
                EQS_PutBitmap((COORD)(pActiveIcon->Header.x + 1), 
                              (COORD)(pActiveIcon->Header.y + 1), 
                              sGetFrameBitmap(pActiveIcon));
                EQS_Rectangle(  pActiveIcon->Header.x , 
                                pActiveIcon->Header.y ,
                                (COORD)(pActiveIcon->Header.x \
                                        + pActiveIcon->Header.dx), 
                                (COORD)(pActiveIcon->Header.y \
                                        + pActiveIcon->Header.dy));                
                EQS_UpdateControlDisplay();
            }
        }
        else
        {
            EQS_PutBitmap(pActiveIcon->Header.x, pActiveIcon->Header.y, 
                          sGetFrameBitmap(pActiveIcon));//GET_BITMAP(pActiveIcon));
            EQS_UpdateControlDisplay();
        }   

	}
    else if (pEvent->EventID == EQE_TM_TIMER1)
    {
        if( EQS_HaveFocus(hControl) == TRUE )
        {
            pActiveIcon->nIndex++;

            if(pActiveIcon->nIndex == GET_FRAME_COUNT(pActiveIcon))
            {
                pActiveIcon->nIndex = 0; // Repeat
            }

            EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
        }

    }
    else if (pEvent->EventID == EQE_KEY_CENTER)
    {
        if( EQS_IsActive(hControl) == TRUE );
           EQS_SendEvent(pActiveIcon->SoftKey, (WORD)hControl, EQS_NA);
    }
    else if (pEvent->EventID == EQE_IO_DELETE)
    {
        // Send ending event you want..

        // Free instance
        sFreeInstance(hControl);

    }

}

void EQS_API sFreeInstance( HCONTROL hControl)
{
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);
	EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

LOCAL CONST BYTE* sGetBitmapData(ACTIVE_ICON_T *pActiveIcon)
{
    WORD wFrameSize;
    UINT8 i;
    BYTE* pbData;

    wFrameSize = 0;
    pbData = (BYTE*)GET_ANI_INFO(pActiveIcon)->pbData;

    if(IS_EQUS_X_IMAGE(&(GET_ANI_INFO(pActiveIcon)->BitmapHeader)) 
       || IS_EQUS_X_ANIMATION(&(GET_ANI_INFO(pActiveIcon)->BitmapHeader)))
    {
        for(i = 0; i < pActiveIcon->nIndex; i++)
        {
            wFrameSize = ((*(pbData)) << 8) + *(pbData+1);
            /* sizeof(WORD) is for frame size data */
            pbData += wFrameSize + sizeof(WORD);
        }
    }
    else /* not packed data */
    {
        pbData = (BYTE*)GET_BITMAP_DATA_POS(pActiveIcon);
    }
    return pbData;
}

LOCAL BITMAP* EQS_API sGetFrameBitmap(ACTIVE_ICON_T *pActiveIcon)
{
    l_FrameBitmap.Header = GET_ANI_INFO(pActiveIcon)->BitmapHeader;
    l_FrameBitmap.pbData = sGetBitmapData(pActiveIcon);//(BYTE*)GET_BITMAP_DATA_POS(pActiveIcon);
    return &l_FrameBitmap;
}
                            
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
HCONTROL EQS_API EQC_ActiveIcon(COORD x, COORD y, 
                                ANIMATION_INFO_T CONST *pAnimationInfo, 
                                EQS_EVENT_ID SoftKey)
{
	void *pInstance;
	HCONTROL hControl;
    ACTIVE_ICON_T *pActiveIcon;

	pInstance = sAllocInstance((ANIMATION_INFO_T *)pAnimationInfo, SoftKey);

/* Original - woostok 01.1.8
    hControl = EQC_AllocControl(CT_ACTIVE_ICON,  pInstance, x, y, 
                                (COORD)(pAnimationInfo\
                                        ->BitmapHeader.Size.xWidth + 1),
                                (COORD)(pAnimationInfo\
                                        ->BitmapHeader.Size.yHeight + 1),
                                sHandleEvent, sFreeInstance);
*/
    hControl = EQC_AllocControl(CT_ACTIVE_ICON,  pInstance, x, y, 
                                (COORD)(pAnimationInfo\
                                        ->BitmapHeader.Size.xWidth + 1),
                                (COORD)(pAnimationInfo\
                                        ->BitmapHeader.Size.yHeight + 1),
                                 sHandleEvent);

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

    EQC_SET_STYLE(hControl, CS_ACTIVE_ICON_DEFAULT);
    l_FrameBitmap.Header = pAnimationInfo->BitmapHeader;
	EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);

	pActiveIcon = (ACTIVE_ICON_T *) EQC_GET_INSTANCE(hControl) ;

    /* active icon은 frame 단위로 지연 시간을 따로 줄 수 없다 */
    if (GET_FRAME_COUNT(pActiveIcon) > 1)
    {
        EQS_StartControlTimer(hControl, EQS_TIMER_1, 
                             GET_DISP_MILLI_SECOND(pActiveIcon, 0), TRUE);
    }

	return hControl;
}

