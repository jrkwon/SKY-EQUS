/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_img.c
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
** 2000-11-21 redstar    modify
** 2000-10-17 jrkwon     Created.
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
#define VIEW_PHOTO_IMAGE   TRUE
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagIMAGE_T  IMAGE_T;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagIMAGE_T
{
    /* control header */
    CONTROL_HEADER_T Header;   
    /* <-- Add here --> the information about this control */
    BITMAP           *pBitmap; 
	WORD              yStart;
	RECT              ViewArea;
};

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

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(BITMAP* pBitmap);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);

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

LOCAL void* EQS_API sAllocInstance(BITMAP* pBitmap)
{
    IMAGE_T *pImage;
    
    /* allocate memory for the instance itself */
    pImage = (IMAGE_T *)EQS_Malloc(sizeof(IMAGE_T));

    /* --> Add here <-- allocate memory for this instance information */
    pImage->pBitmap = pBitmap;    	
    return (void *)pImage;
}

LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent)
{
    IMAGE_T *pImage;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_IMAGE);
    
    /* get the pointer of teh control instance */
    pImage = (IMAGE_T *)EQC_GET_INSTANCE(hControl);
    
	if( pEvent->EventID == EQE_APP_IO_MODE )
	{
		EQS_SendEvent( EQE_IO_IMAGE_SELECT, hControl, EQS_NA );
        EQS_EndIO();
	}
    else if(pEvent->EventID == EQE_APP_DRAW)
    {
        /* <-- Add here --> drawing routine */        
#if VIEW_PHOTO_IMAGE
#if 0
#ifdef FEATURE_EQS_COLOR_256
        if(pImage->pBitmap->Header.nBitsPerPixel == BIT_PER_PIXEL_256C)
		    PutBitmapRect256C(EQC_GET_X(hControl), EQC_GET_Y(hControl), pImage->pBitmap, pImage->ViewArea);
        else
#endif /* FEATURE_EQS_COLOR_256 */
#endif
        /* jrkwon 2001.08.30 --> 이미지가 4gray나 bw인 경우는 일반 PutBitmap 함수 사용해야 함 */
        if(pImage->pBitmap->Header.nBitsPerPixel == BIT_PER_PIXEL_256C 
           || pImage->pBitmap->Header.nBitsPerPixel == BIT_PER_PIXEL_64KC)
        	EQS_PutBitmapRect(EQC_GET_X(hControl), EQC_GET_Y(hControl), pImage->pBitmap, pImage->ViewArea);
        else
    		EQS_PutBitmap(EQC_GET_X(hControl), EQC_GET_Y(hControl), pImage->pBitmap);
#else
		EQS_PutBitmap(EQC_GET_X(hControl), EQC_GET_Y(hControl), pImage->pBitmap);
#endif

		if( EQC_GET_STYLE(hControl) & CS_IMAGE_BORDER )
		{
			EQC_DrawBorder(hControl,\
				            EQC_GET_X(hControl), EQC_GET_Y(hControl),\
				            EQC_GET_X(hControl)+EQC_GET_DX(hControl)-1,\
							EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-1);
		}        
		if( EQS_HaveFocus(hControl) )
		{
		    COLOR ForeColor;

			ForeColor = EQS_GetForeColor();
			EQS_SetForeColor( BLUE );//EQS_GetBackColor() );
			EQS_Rectangle( EQC_GET_X(hControl), EQC_GET_Y(hControl),\
				           EQC_GET_X(hControl)+EQC_GET_DX(hControl),\
						   EQC_GET_Y(hControl)+EQC_GET_DY(hControl));
			EQS_SetForeColor(ForeColor);			
		}
        EQS_UpdateControlDisplay();
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
    /* free the memory that was allocated */
    /* <-- Add here --> */
    
    /* free the instance itself */
    EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}


HCONTROL EQS_API EQC_Image(COORD x, COORD y, BITMAP* pBitmap)
{
    void *pInstance;
    HCONTROL hControl;
    COORD dx, dy;

    IMAGE_T *pImage;

    /* <-- Add here --> local variables */
    
    pInstance = sAllocInstance(pBitmap);
    
    /* get dx, dy */
    dx = pBitmap->Header.Size.xWidth;/* <-- Add here --> */
    dy = pBitmap->Header.Size.yHeight;/* <-- Add here --> */

/* Original - woodstok 01.1.8    
    hControl = EQC_AllocControl(CT_IMAGE,  pInstance, x, y, dx, dy, 
                                sHandleEvent, sFreeInstance );
*/
    hControl = EQC_AllocControl(CT_IMAGE,  pInstance, x, y, dx, dy, 
                                sHandleEvent );

    ASSERT(hControl != INVALID_CONTROL_HANDLE);


    pImage = (IMAGE_T *)EQC_GET_INSTANCE(hControl);
    pImage->ViewArea.xLeft   = 0;
	pImage->ViewArea.xRight  = pBitmap->Header.Size.xWidth-1;
	pImage->ViewArea.yTop    = pImage->pBitmap->Header.yStart;
	pImage->ViewArea.yBottom = pImage->ViewArea.yTop + pBitmap->Header.Size.yHeight-1;


    EQC_SET_FOCUS(hControl, FALSE);
    /* <-- Add here --> set the control init value */
    EQC_SET_STYLE(hControl, CS_IMAGE_DEFAULT);
    
    /* send draw event to the state machine engine */
    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);
    
    return hControl;
}

void EQS_API EQC_SetImage(HCONTROL hControl, BITMAP* pBitmap/* <-- Add here --> */)
{
    IMAGE_T *pImage;
    pImage = (IMAGE_T *)EQC_GET_INSTANCE(hControl);
    ASSERT( EQC_GET_TYPE( hControl ) == CT_IMAGE);
    pImage->pBitmap = pBitmap;

    /* send draw event to the state machine engine */
    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);
}

BITMAP* EQS_API EQC_GetBitmap(HCONTROL hControl)
{
    IMAGE_T *pImage;
    pImage = (IMAGE_T *)EQC_GET_INSTANCE(hControl);
    ASSERT( EQC_GET_TYPE( hControl ) == CT_IMAGE);

    return pImage->pBitmap;   
}


//PutBitmapRect


BOOL EQS_API EQC_SetImageViewArea(HCONTROL hControl, RECT* pRect)
{
    IMAGE_T *pImage;
    pImage = (IMAGE_T *)EQC_GET_INSTANCE(hControl);
    ASSERT( EQC_GET_TYPE( hControl ) == CT_IMAGE);

	pImage->ViewArea = *pRect;
	EQC_SET_DX(hControl, pRect->xRight - pRect->xLeft + 1);
	EQC_SET_DY(hControl, pRect->yBottom - pRect->yTop + 1);
    return TRUE;
}


RECT EQS_API EQC_GetImageViewArea(HCONTROL hControl)
{
    IMAGE_T *pImage;
    pImage = (IMAGE_T *)EQC_GET_INSTANCE(hControl);
    ASSERT( EQC_GET_TYPE( hControl ) == CT_IMAGE);

    return pImage->ViewArea;
}