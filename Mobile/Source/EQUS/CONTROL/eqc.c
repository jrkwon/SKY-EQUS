/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc.c
**
** Description: 
**     EQUS I/O Controls Implementations
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-07-03   girin      Created.
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
#define INIT_CONTROL_INDEX      1
#define SKIP_CONTROL_INDEX      0

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
CONTROL_T     g_Control[MAX_CONTROL_NUM];

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL HCONTROL l_ControlIndex = 0; /* the index of g_Control */

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL HCONTROL sAllocFreeHeader(void);
            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

LOCAL HCONTROL sAllocFreeHeader(void)
{
	HCONTROL i;

	for (i = MOD_INCREMENT(l_ControlIndex, MAX_CONTROL_NUM); 
		 i != l_ControlIndex;
		 i = MOD_INCREMENT(i, MAX_CONTROL_NUM))
		 {
            if (i == SKIP_CONTROL_INDEX) 
                continue; // Don't use it. It will used on special purpose.

			if (EQC_GET_TYPE(i) == CT_NONE) // free slot found.
			{
				l_ControlIndex = i;
				return i;
			}
		 }

    // i == l_ControlIndex
	if (EQC_GET_TYPE(i) == CT_NONE) // free slot found.
	{
		l_ControlIndex = i;
		return i;
	}
	else
		return INVALID_CONTROL_HANDLE;
}

/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

void EQS_API    EQC_InitControl(void)
{
	HCONTROL i;

	for (i = 0; i < MAX_CONTROL_NUM; i++)
		EQC_SET_TYPE(i, CT_NONE);

	l_ControlIndex = INIT_CONTROL_INDEX;

}

HCONTROL EQS_API EQC_AllocControl(CONTROL_TYPE_E Type,
                                  void *pInstance,
                                  COORD x, COORD y, 
                                  COORD dx, COORD dy,
                                  void (*pfnHandleEvent)(HCONTROL hControl, \
                                                         EQS_EVENT *pEvent))
//                                  void (*pfnFree)(HCONTROL hControl))
{
	HCONTROL hControl;

	hControl = sAllocFreeHeader();

	if (hControl == INVALID_CONTROL_HANDLE)
	{
		ASSERT( hControl != INVALID_CONTROL_HANDLE );
		return INVALID_CONTROL_HANDLE;
	}

    /* set control */
    EQC_SET_TYPE(hControl, Type);
    EQC_SET_INSTANCE(hControl, pInstance);

    /* set control header */
    EQC_SET_X(hControl, x);
    EQC_SET_Y(hControl, y);
    EQC_SET_DX(hControl, dx);
    EQC_SET_DY(hControl, dy);
//    EQC_SET_FREE(hControl, pfnFree);
    EQC_SET_HANDLE_EVENT(hControl, pfnHandleEvent);

    EQC_SET_FOCUS(hControl, INIT_FOCUS);
    EQC_SET_ENABLE(hControl, INIT_ENABLE);
	EQC_SET_DIRTY(hControl,  FALSE);
	EQC_SET_EDIT(hControl,   TRUE);
	EQC_SET_USERDATA(hControl, EQS_NA);
	EQC_SET_SELF_DRAW(hControl, TRUE);

    EQC_SET_STYLE(hControl, INIT_STYLE);

    /* use the dc value */
    EQC_SET_FONT_TYPE(hControl, EQS_GetFontType());
    EQC_SET_TEXT_COLOR(hControl, EQS_GetTextColor());
	

    /* attach the control to the current applet -- eqs_aapi.c */
    EQS_AttachControl(hControl);

	return hControl;
}

void EQS_API EQC_DeleteControl(HCONTROL hControl , BOOL fActiveDelete)
{
    EQS_EVENT s_DeleteControlEvent = {EQE_IO_DELETE, EQS_NA, EQS_NA, EQS_NA};

	if( fActiveDelete == FALSE )
		s_DeleteControlEvent.wParam = (WORD) INVALID_CONTROL_HANDLE;
	else
		s_DeleteControlEvent.wParam = (WORD) hControl;


	ASSERT(hControl < MAX_CONTROL_NUM);

    //(EQC_GET_FREE(hControl))(hControl);
    EQC_GET_HANDLE_EVENT(hControl)(hControl, (EQS_EVENT *)&s_DeleteControlEvent);
    EQC_SET_TYPE(hControl, CT_NONE);
}

void EQS_API EQC_HandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	if( hControl < INVALID_CONTROL_HANDLE )
	{
		if( EQC_GET_TYPE(hControl) != CT_NONE )
		{
			EQC_GET_HANDLE_EVENT(hControl)(hControl, pEvent);
		}
	}
}

void EQS_API EQC_DrawBorder(HCONTROL hControl, COORD x, COORD y, COORD dx, COORD dy)
{
	COLOR OldColor;

	OldColor = EQS_GetForeColor();
   
#if 0
	if( EQC_GET_EDIT(hControl) == FALSE)
    {		
		OldColor = EQS_GetForeColor();
		EQS_SetForeColor(GRAY);//R_BLACK);
		EQS_Rectangle(x-1,y-1,dx+1,dy+1);
		EQS_SetForeColor(OldColor);
	}    
    else if( (EQC_GET_TYPE(hControl) == CT_SCROLL_LIST) &&\
		     ( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO || EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )&&\
			 (EQS_IsActive(hControl) == FALSE))
	{
			 
		EQS_Rectangle(x-1,y-1,dx+1,dy+1);
	}
#endif
	if( EQS_GetActiveDraw() == FALSE )
	{ // non active
		if( hControl = EQS_GetFocusedIOC() )
		{
			EQS_SetForeColor(CS_R_WHITE);
			EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(dx+1), (COORD)(y-1));
			EQS_Line((COORD)(x-1), (COORD)(y), (COORD)(x-1), (COORD)(dy+1));
			EQS_SetForeColor(CS_R_GRAY);
			EQS_Line((COORD)(x-1), (COORD)(dy+1), (COORD)(dx+1), (COORD)(dy+1));
			EQS_Line((COORD)(dx+1), (COORD)(y), (COORD)(dx+1), (COORD)(dy+1));
		}
		else
		{
			if( EQC_GET_FOCUS(hControl) == TRUE )
			{ 
				EQS_SetForeColor(CN_R_WHITE);
				EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(dx+1), (COORD)(y-1));
				EQS_Line((COORD)(x-1), (COORD)(y), (COORD)(x-1), (COORD)(dy+1));
				EQS_SetForeColor(CN_R_GRAY);
				EQS_Line((COORD)(x-1), (COORD)(dy+1), (COORD)(dx+1), (COORD)(dy+1));
				EQS_Line((COORD)(dx+1), (COORD)(y), (COORD)(dx+1), (COORD)(dy+1));

			}
			else
			{
				EQS_SetForeColor(CD_R_WHITE);
				EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(dx+1), (COORD)(y-1));
				EQS_Line((COORD)(x-1), (COORD)(y), (COORD)(x-1), (COORD)(dy+1));
				EQS_SetForeColor(CD_R_GRAY);
				EQS_Line((COORD)(x-1), (COORD)(dy+1), (COORD)(dx+1), (COORD)(dy+1));
				EQS_Line((COORD)(dx+1), (COORD)(y), (COORD)(dx+1), (COORD)(dy+1));
			}
		}
	}
	else
	{// active 
/*
		if( EQC_GET_TYPE(hControl) == CT_SCROLL_LIST && !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE))
		{

		}
		else
*/
		{
			EQS_SetForeColor(CA_R_WHITE);
			EQS_Line((COORD)(x-1), (COORD)(y-1), (COORD)(dx+1), (COORD)(y-1));
			EQS_Line((COORD)(x-1), (COORD)(y), (COORD)(x-1), (COORD)(dy+1));
			EQS_SetForeColor(CA_R_GRAY);
			EQS_Line((COORD)(x-1), (COORD)(dy+1), (COORD)(dx+1), (COORD)(dy+1));
			EQS_Line((COORD)(dx+1), (COORD)(y), (COORD)(dx+1), (COORD)(dy+1));
		}

	}

	EQS_SetForeColor(OldColor);

}

void EQC_SetStyle(HCONTROL hControl, WORD StyleA)
{

    switch( EQC_GET_TYPE(hControl) )
    {
    case CT_SCROLL_LIST:
        EQC_SetScrollStyle(hControl, StyleA);
        break;
    case CT_STATIC_TEXT:
        EQC_SetStaticTextStyle(hControl, StyleA);
        break;

    case CT_MENU_LIST:
        EQC_SetMenuListStyle(hControl, StyleA);
        break;
	case CT_CHECK_BOX:
		EQC_SetCheckBoxStyle(hControl, StyleA);
		break;
	case CT_RADIO_BUTTON:
		EQC_SetRadioButtonStyle(hControl, StyleA);
		break;
    case CT_MESSAGE_BOX:
		EQC_SetMessageBoxStyle(hControl, StyleA);
		break;
    case CT_POPUP_MENU:
		EQC_SetPopupMenuStyle(hControl, StyleA);
		break;
    default:
        EQC_GET_INSTANCE(hControl)->Style = (StyleA);
        break;
    }        
}





/* --- eqs_ioc.h --- 
enum tagCONTROL_TYPE_E {
{
    CT_NONE             = -1,
    
    CT_IMAGE            = 0,
    CT_ANIMATION        = 1,
    CT_ACTIVE_ICON      = 2,

    CT_STATIC_TEXT      = 3,
    CT_SCROLL_LIST      = 4,
    
    CT_TEXT_EDIT        = 5,
    CT_NUMBER_EDIT      = 6,
    
    CT_PUSH_BUTTON      = 7,
    CT_RADIO_BUTTON     = 8,
    CT_CHECK_BUTTON     = 9,
    
    CT_MESSAGE_BOX      = 10,
    CT_POPUP_MENU       = 11,
    
    CT_MAX
};
*/
#if 0
struct tagCONTROL_TABLE_T
{
    void (*pfnHandleEvent)(HCONTROL hControl, EQS_EVENT *pEvent);
    void (*pfnFree)(void* pInstance);
}
typedef struct tagCONTROL_TABLE  CONTROL_TABLE_T;

ROM CONTROL_TABLE_T aControlTable[] = {
    { EQC_HandleImageEvent,         EQC_FreeImage },        /* CT_IMAGE            = 0, */
    { EQC_HandleAnimationEvent,     EQC_FreeAnimation },    /* CT_ANIMATION        = 1, */
    { EQC_HandleActiveIconEvent,    EQC_FreeActiveIcon },   /* CT_ACTIVE_ICON      = 2, */
    
    { EQC_HandleStaticTextEvent,    EQC_FreeStaticText },   /* CT_STATIC_TEXT      = 3, */
    { EQC_HandleScrollListEvent,    EQC_FreeScrollList },   /* CT_SCROLL_LIST      = 4, */
    
    { EQC_HandleTextEditEvent,      EQC_FreeTextEdit },     /* CT_TEXT_EDIT        = 5, */
    { EQC_HandleNumberEditEvent,    EQC_FreeNumberEdit },   /* CT_NUMBER_EDIT      = 6, */
    
    { EQC_HandlePushButtonEvent,    EQC_FreePushButton },   /* CT_PUSH_BUTTON      = 7,*/
    
    { EQC_HandleRadioButtonEvent,   EQC_FreeRadioButton },  /* CT_RADIO_BUTTON     = 8,*/
    { EQC_HandleCheckButtonEvent,   EQC_FreeCheckButton },  /* CT_CHECK_BUTTON     = 9,*/
    
    { EQC_HandleMessageBoxEvent,    EQC_FreeMessageBox },   /* CT_MESSAGE_BOX      = 10,*/
    { EQC_HandlePopupMenuEvent,     EQC_FreePopupMenu },    /* CT_POPUP_MENU       = 11,*/
};
#endif /* 0 */

