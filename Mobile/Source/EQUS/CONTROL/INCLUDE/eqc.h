/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc.h
**
** Description: 
**     EQUS Controls Interfaces
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-01-01   girin       Created.
**
** ===========================================================================
*/
#ifndef _EQC_H_
#define _EQC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define EQS_DEFAULT_STYLE           0x0000 //0xFFFF

/*#define MAX_IOC_NUM                 100
#define INVALID_IOC_HANDLE          MAX_IOC_NUM
#define MAX_IOC_PER_APPLET          30*/

#define MAX_CONTROL_NUM             150
#define INVALID_CONTROL_HANDLE      MAX_CONTROL_NUM
//#define MAX_CONTROL_PER_APPLET      30

/* define some init values */
#define INIT_FOCUS                  TRUE
#define INIT_ENABLE                 TRUE
#define INIT_STYLE                  EQS_DEFAULT_STYLE

#define BG_DARK_COUNT               1

#define CTRL_DEACTIVE_COLOR  RGB332(192,160, 128)

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagCONTROL_TYPE_E
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
    CT_CHECK_BOX        = 9,    
    
    CT_MESSAGE_BOX      = 10,
    CT_POPUP_MENU       = 11,
    CT_PROGRESS_BAR     = 12,
    CT_SPIN             = 13,
    CT_MENU_LIST        = 14,
    CT_TIME             = 15,
    CT_CALENDAR         = 16,
    CT_MAX
};
typedef enum tagCONTROL_TYPE_E       CONTROL_TYPE_E;

typedef WORD                        CONTROL_STYLE;
typedef struct tagCONTROL_HEADER_T  CONTROL_HEADER_T;
typedef struct tagCONTROL_T         CONTROL_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagCONTROL_T
{
    /* control type */
    CONTROL_TYPE_E  Type;
    /* instance */
    void            *pInstance;
};

#define EQC_GET_TYPE(hControl)                  g_Control[hControl].Type
#define EQC_GET_INSTANCE(hControl)              ((CONTROL_HEADER_T*)(g_Control[hControl].pInstance))

#define EQC_SET_TYPE(hControl, TypeA)           g_Control[hControl].Type = (TypeA)
#define EQC_SET_INSTANCE(hControl, pInstance)   g_Control[hControl].pInstance = (pInstance)

struct tagCONTROL_HEADER_T
{
    /* position */
    COORD           x;
    COORD           y;
    /* size */
    COORD           dx;
    COORD           dy;
    /* font, text */
    FONT_TYPE       FontType;
    COLOR           TextColor;
    /* Style */
    CONTROL_STYLE   Style;
    /* can be focused, enable/disable */
    BOOL            fFocus; /* if FALSE, skip while control navigation */
    BOOL            fEnable;
	BOOL            fEdit;
	BOOL            fDirty;
	BOOL            fSelfDraw;
	INT             UserData;      // for user data
    /* function pointer */
    void (*pfnHandleEvent)(HCONTROL hControl, EQS_EVENT *pEvent);
//    void (*pfnFree)(void* pInstance);
//    void (*pfnFree)(HCONTROL hControl);
};


typedef enum 
{	
	ACCEL_KEY_0 = 0,
	ACCEL_KEY_1,
	ACCEL_KEY_2,
	ACCEL_KEY_3,
	ACCEL_KEY_4,
	ACCEL_KEY_5,
	ACCEL_KEY_6,
	ACCEL_KEY_7,
	ACCEL_KEY_8,
	ACCEL_KEY_9,
	ACCEL_KEY_STAR,
	ACCEL_KEY_POUND,
	ACCEL_KEY_NONE
} ACCEL_KEY_T;


extern  void EQC_SetStyle(HCONTROL hControl, WORD StyleA);

#define EQC_ACTIVATE                            0xffff0000
#define EQC_DELETED                             0xffffffff


#define EQC_GET_X(hControl)                     EQC_GET_INSTANCE(hControl)->x
#define EQC_GET_Y(hControl)                     EQC_GET_INSTANCE(hControl)->y
#define EQC_GET_DX(hControl)                    EQC_GET_INSTANCE(hControl)->dx
#define EQC_GET_DY(hControl)                    EQC_GET_INSTANCE(hControl)->dy
#define EQC_GET_FONT_TYPE(hControl)             EQC_GET_INSTANCE(hControl)->FontType
#define EQC_GET_TEXT_COLOR(hControl)            EQC_GET_INSTANCE(hControl)->TextColor
#define EQC_GET_STYLE(hControl)                 EQC_GET_INSTANCE(hControl)->Style
#define EQC_GET_FOCUS(hControl)                 EQC_GET_INSTANCE(hControl)->fFocus
#define EQC_GET_ENABLE(hControl)                EQC_GET_INSTANCE(hControl)->fEnable
#define EQC_GET_DIRTY(hControl)                 EQC_GET_INSTANCE(hControl)->fDirty
#define EQC_GET_EDIT(hControl)                  EQC_GET_INSTANCE(hControl)->fEdit
#define EQC_GET_USERDATA(hControl)              EQC_GET_INSTANCE(hControl)->UserData
#define EQC_GET_SELF_DRAW(hControl)             EQC_GET_INSTANCE(hControl)->fSelfDraw
//#define EQC_GET_FREE(hControl)                  EQC_GET_INSTANCE(hControl)->pfnFree
#define EQC_GET_HANDLE_EVENT(hControl)          EQC_GET_INSTANCE(hControl)->pfnHandleEvent

#define EQC_SET_X(hControl, xA)                  EQC_GET_INSTANCE(hControl)->x = (xA)
#define EQC_SET_Y(hControl, yA)                  EQC_GET_INSTANCE(hControl)->y = (yA)
#define EQC_SET_DX(hControl, dxA)                EQC_GET_INSTANCE(hControl)->dx = (dxA)
#define EQC_SET_DY(hControl, dyA)                EQC_GET_INSTANCE(hControl)->dy = (dyA)
#define EQC_SET_FONT_TYPE(hControl, FontTypeA)   EQC_GET_INSTANCE(hControl)->FontType = (FontTypeA)
#define EQC_SET_TEXT_COLOR(hControl, TextColorA) EQC_GET_INSTANCE(hControl)->TextColor = (TextColorA)
#define EQC_SET_STYLE(hControl, StyleA)          EQC_SetStyle(hControl, StyleA)
#define EQC_SET_FOCUS(hControl, fFocusA)         EQC_GET_INSTANCE(hControl)->fFocus   = (fFocusA)
#define EQC_SET_ENABLE(hControl, fEnableA)       EQC_GET_INSTANCE(hControl)->fEnable  = (fEnableA)
#define EQC_SET_DIRTY(hControl,  fDirtyA)        EQC_GET_INSTANCE(hControl)->fDirty   = (fDirtyA)
#define EQC_SET_EDIT(hControl,  fEditA)          EQC_GET_INSTANCE(hControl)->fEdit    = (fEditA)
#define EQC_SET_USERDATA(hControl, UserDataA)    EQC_GET_INSTANCE(hControl)->UserData = (INT)(UserDataA)
#define EQC_SET_SELF_DRAW(hControl, fSelfDrawA)  EQC_GET_INSTANCE(hControl)->fSelfDraw = (fSelfDrawA)
                                                                            
//#define EQC_SET_FREE(hControl, pfnFreeA)         EQC_GET_INSTANCE(hControl)->pfnFree = (pfnFreeA)
#define EQC_SET_HANDLE_EVENT(hControl, pfnHandleEventA) \
                                                EQC_GET_INSTANCE(hControl)->pfnHandleEvent = (pfnHandleEventA)

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
extern CONTROL_T   g_Control[];

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

            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

/*
EQS_RESULT  NIOC_InitCommonIOCHeader(void);
EQS_RESULT  NIOC_ExecIOControl(HCONTROL hControl, EQS_EVENT* pAnEvent);
HCONTROL    NIOC_AllocCommonIOCHeader(IOC_Type IOCType, UINT8 x, UINT8 y, UINT8 dx, UINT8 dy, void* pInstance);
EQS_RESULT  NIOC_DeleteOneIOControl(HCONTROL hControl);
IOC_Type    NIOC_GetIOCType(HCONTROL hControl);
*/
extern void     EQS_API EQC_InitControl(void);
/* Original - woodstok 01.1.8
extern HCONTROL EQS_API EQC_AllocControl(CONTROL_TYPE_E Type,
                                         void *pInstance,
                                         COORD x, COORD y, 
                                         COORD dx, COORD dy,
                                         void (*pfnHandleEvent)( \
                                                     HCONTROL hControl, \
                                                     EQS_EVENT *pEvent),
                                         void (*pfnFree)(HCONTROL hControl));
*/
extern HCONTROL EQS_API EQC_AllocControl(CONTROL_TYPE_E Type,
                                         void *pInstance,
                                         COORD x, COORD y, 
                                         COORD dx, COORD dy,
                                         void (*pfnHandleEvent)( \
                                                     HCONTROL hControl, \
                                                     EQS_EVENT *pEvent));

extern void     EQS_API EQC_DeleteControl(HCONTROL hControl , BOOL fActiveDelete);
extern void     EQS_API EQC_HandleEvent(HCONTROL hControl, EQS_EVENT* pEvent);
//extern CONTROL_TYPE EQS_API EQC_GetControlType(HCONTROL hControl);
extern void EQS_API EQC_DrawBorder(HCONTROL hControl, COORD x, COORD y, COORD dx, COORD dy);

#ifdef __cplusplus
}
#endif

#endif /* _EQC_H_ */

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
