/*
** ============================================================================
**
** Project: 
**     ori
**
** File:
**     eqs_aapi.h
**
** Description:
**     NUI event
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-05-02 redstar    add API FUnction header related MENU, IO applet
** 2000-04-20 redstar    add funcation releated with screen display 
** 2000-03-21 redstar    First revision
** 1999-11-30 girin      Created
** ============================================================================
*/

#ifndef _EQS_AAPI_H_
#define _EQS_AAPI_H_

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"

#define RTYU
/* rosa 01-02-07 --> @why, what */
#define DIRECT_APPLET_DRAW
/* <-- */

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define MAX_EQUS_MEM_POOL_SIZE               (25*1024) // (1024*(74)) 

#define EQS_DRAW_LEVEL_ALL              0
#define EQS_DRAW_LEVEL_CML              (MAX_CONTROL_NUM + 1)
#define EQS_DRAW_LEVEL_SWINDOW          (EQS_DRAW_LEVEL_CML + 1)
#define EQS_DRAW_LEVEL_APP              (EQS_DRAW_LEVEL_SWINDOW+1)
#define EQS_DRAW_LEVEL_APPLET_ONLY      (EQS_DRAW_LEVEL_APP + 1)

#define EQS_INVALID_CONTROL_ORDER       (MAX_CONTROL_PER_APPLET + 1)



#define EQS_DoEvent(NativeEventID)   \
                                { \
                                    EQS_EVENT InEvent; \
                                    EQS_Native2EQSEvent(ui.event, &InEvent);\
                                    EQS_Scheduler(InEvent.EventID); \
                                }
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef void (EQS_APPLET)(void* pInEvent); 

/* NU PopUp type  */
enum tagEQS_POPUP {
    EQS_POP_TO_PARENT,
    EQS_POP_TO_PARENT_OF_PARENT,
    EQS_POP_TO_SUBROOT,
    EQS_POP_TO_PARENT_OF_SUBROOT,
    EQS_POP_TO_ROOT,
    EQS_POP_TO_DEST
};
typedef enum tagEQS_POPUP          EQS_POPUP;

enum tagWM // SWindow Mode ( Tri-Mode )
{
    WM_APPLET_MODE = 0,
    WM_IO_MODE,
    WM_MODAL_IO_MODE,
    WM_NATIVE_MODE
};

typedef enum tagWM WINDOW_MODE;


enum tagWTSM // SWindow Title Symbol Mode
{
    WTSM_NONE_MODE = 0,
    WTSM_NORMAL_MODE,
    WTSM_CALL_MODE,
	WTSM_DORMONT_MODE,
    WTSM_NATIVE_MODE
};

typedef enum tagWTSM WINDOW_TSYMBOL_MODE;

/*
**---------------------------------------------------------------------------
**  General Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern BOOL g_fSKTT_Release;

extern BOOL (*g_pfnSetTimer)(EQS_TIMER_E nTimerID, LONG lInterval, BOOL fRepeat);   //redstar  2000.9.16
extern void EQS_API (*g_pfnGetSystemTime)(QWORD CurrentTime);                       // redstar 2001.1.2

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
// Should be moved to eqs_aapi.h
//extern int EQS_Init(void);
//extern int EQS_Sched(EQS_EVENT InEvent);
EQS_DCL EQS_RESULT EQS_Initialize(EQS_APPLET *pStartApplet, EQS_APPLET *pRootApplet, 
//                                  SCREEN_INFO* pScreenInfo,
                                  void (*pfnDrawSWindowHeader)(void),
                                  ROM CML_INFO_T*  pCMLInfo,
                                  VOID EQS_API (*pfnDispatcher)(EQS_APPLET *pfnApplet, EQS_EVENT* pEvent),
                                  EQS_APPLET *pfnDefEventProc,
                                  BOOL EQS_API (*pfnSetTimer)(EQS_TIMER_E nTimerID, LONG lInterval, BOOL fRepeat),
                                  void EQS_API (*pfnGetSystemTime)(QWORD CurrentTime),
                                  ROM BYTE* ROM *aszToken[],
                                  VOID EQS_API (*pfnRefreshDisplayDevice)(VOID),
                                  VOID EQS_API (*pfnRefreshDisplayRect)(BYTE* , UINT8 , UINT8, UINT8 , UINT8 ),
								  void EQS_API (*pfnSetPaceMaker)(LONG lIntervalMS, BOOL fRepeat));

EQS_DCL EQS_RESULT EQS_ReinitializeEQS_MEM_POOL(void);

EQS_DCL VOID EQS_Scheduler(EQS_EVENT *pEvent);
/* jrkwon 2000-07-18 --> plug in EQUS display in to state machine engine */
EQS_DCL VOID EQS_API EQS_UpdateDisplay(VOID);
EQS_DCL VOID EQS_API EQS_UpdateControlDisplay(VOID);
EQS_DCL VOID EQS_API EQS_SetRefreshDisplaySetting(BOOL fEnable);
EQS_DCL BOOL EQS_API EQS_GetRefreshDisplaySetting(VOID);
EQS_DCL VOID EQS_API EQS_RefreshDisplay(VOID);
EQS_DCL VOID EQS_API EQS_RefreshDisplayRect(HCONTROL hControl, UINT8 xleft, UINT8 ytop, UINT8 xright, UINT8 ybottom);
/* <-- */
EQS_DCL BOOL EQS_ReinitMemory(void);

extern HWINDOW      EQS_SetWindow(COORD xPos, 
                                  COORD yPos, 
                                  COORD dxWidth, 
                                  COORD dyHeight, 
                                  UINT8 nCML, 
                                  CONST BYTE* pszHeadTitle,
                                  WORD wAttr);
extern EQS_RESULT   EQS_SetWindowCML(UINT8 nCML);
extern UINT8        EQS_GetWindowCML(void);
extern EQS_RESULT   EQS_SetWindowAttr(WORD wAttr);
extern WORD         EQS_GetWindowAttr(void);
extern void EQS_API EQS_SetSWindowHeaderStr(CONST BYTE* pszHeaderTitle);
extern CONST BYTE*  EQS_GetCurrentHeaderStr(void);
extern EQS_RESULT   EQS_SetCanvasSize(COORD dxSize, COORD dySize);
extern COORD        EQS_GetCanVasHeight(void);
extern COORD EQS_API EQS_FitCanvasSize(void);
extern EQS_RESULT   EQS_SetIOCActive(HCONTROL hControl);
extern EQS_RESULT   EQS_SetIOCFocus(HCONTROL hControl);
extern EQS_RESULT   EQS_SetNthIOCFocus(UINT8 nOrder);
extern EQS_RESULT   EQS_GetIOCWindowPosX(HCONTROL hControl, COORD* pX);
extern EQS_RESULT   EQS_GetIOCWindowPosY(HCONTROL hControl, COORD* pY);
extern void         EQS_GetSWindowViewAreaRect(RECT* pRect);
extern COORD        EQS_GetSWindowOffset(void);
extern HCONTROL     EQS_GetFocusedIOC(void);
extern HCONTROL     EQS_GetActiveIOC(void);
extern HCONTROL     EQS_GetNthIOC(UINT8 unNth);
extern UINT8        EQS_GetIOCOrderly(HCONTROL hControl);
extern UINT8        EQS_GetIOCNum(void);
extern INT8 EQS_API EQS_GetIOCOrder(HCONTROL hControl);
extern BOOL         EQS_HaveFocus(HCONTROL hControl);
extern BOOL         EQS_IsActive(HCONTROL hControl);
extern HCONTROL     EQS_GetModal(void);
extern BOOL         EQS_IsControlDataChanged(void);

extern EQS_RESULT   EQS_StartChild(EQS_APPLET *pAppFuncA);
extern EQS_RESULT   EQS_StartChildEvent(EQS_APPLET *pAppFunA, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam);
extern EQS_RESULT   EQS_StartChildCurrentEvent(EQS_APPLET *pAppFuncA);
extern EQS_RESULT   EQS_StartSibling(EQS_APPLET *pAppFuncA, BOOL fDispUntouched);
extern EQS_RESULT   EQS_StartSiblingEvent(EQS_APPLET *pAppFuncA, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam, BOOL fDispUntouched);
extern EQS_RESULT   EQS_StartSiblingCurrentEvent(EQS_APPLET *pAppFuncA, BOOL fDispUntouched);

extern EQS_RESULT   EQS_EndTo(EQS_POPUP ReturnPlace);
extern EQS_RESULT   EQS_EndToParam(EQS_POPUP ReturnPlace, WORD wParam, DWORD dwParam);
extern EQS_RESULT   EQS_EndToEvent(EQS_POPUP ReturnPlace, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam);
extern EQS_RESULT   EQS_EndToCurrentEvent(EQS_POPUP ReturnPlace);
extern EQS_RESULT   EQS_EndDest(EQS_APPLET *pAppFuncA);
extern EQS_RESULT   EQS_EndDestParam(EQS_APPLET *pAppFuncA, WORD wParam, DWORD dwParam);
extern EQS_RESULT   EQS_EndDestEvent(EQS_APPLET *pAppFuncA, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam);
extern EQS_RESULT   EQS_EndDestCurrentEvent(EQS_APPLET *pAppFuncA);
extern EQS_RESULT   EQS_End(void);
extern EQS_RESULT   EQS_EndParam(WORD wParam, DWORD dwParam);
extern EQS_RESULT   EQS_EndEvent(EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam);
extern EQS_RESULT   EQS_EndCurrentEvent(void);

extern EQS_RESULT   EQS_EndIO(void);
extern EQS_RESULT   EQS_EndModalIO(void);
extern BOOL         EQS_StartModalIO(void);
extern BOOL         EQS_PassIO(INT8 nDirect);


extern EQS_RESULT   EQS_SendEvent(EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam);
extern EQS_RESULT   EQS_PassCurrentEvent(void);
extern EQS_EVENT_CATEGORY EQS_GetCategory(EQS_EVENT_ID EventID);

extern EQS_RESULT   EQS_PinUp(void);
extern EQS_APPLET   *EQS_GetChild(void);
extern EQS_APPLET   *EQS_GetParent(void);
extern EQS_APPLET   *EQS_GetAncestor(UINT8 nTh_Ancestor);
extern EQS_APPLET   *EQS_GetApplet(void);
extern HTIMER       EQS_GetAppletTimerInfo(void);
extern EQS_EVENT    EQS_GetEvent(void);

extern void EQS_API EQS_DefEventProc(EQS_EVENT* pInEvent);

extern EQS_RESULT   EQS_DeleteControl(HCONTROL hControl);
extern BOOL EQS_API EQS_AttachControl(HCONTROL hControl);



//extern EQS_RESULT   NX_FreeIOCIndex(HCONTROL hControl);
//extern BOOL         NX_PutIOCIndex(HCONTROL hControl);

/*
extern EQS_RESULT EQS_SetFocus(UINT8 hControl);
extern EQS_RESULT EQS_ClearFocus(UINT8 hControl);
extern EQS_RESULT EQS_SetFocusNext(void);
*/
#ifdef DIRECT_APPLET_DRAW
void EQS_SetDirectDraw(BOOL fDirectDraw);
void EQS_AppletDrawDirect(void);
#endif

extern WINDOW_TSYMBOL_MODE EQS_API EQS_GetWindowSymbolMode(void);
extern void EQS_API EQS_SetWindowSymbolMode(WINDOW_TSYMBOL_MODE WindowSymbolMode);
extern void EQS_SetSWindowPosition(INT16 nOffset);

extern void ScrollScreen(UINT8 nDestX, UINT8 nDestY, UINT8 nSourceX, UINT8 nSourceY, UINT8 nWidth, UINT8 nHeight);

#endif /* _EQS_AAPI_H_ */
