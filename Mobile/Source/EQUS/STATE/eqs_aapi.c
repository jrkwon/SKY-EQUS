/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_aapi.c
**
** Description: 
**     The core functions that provide EQS functionality.
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
**
** 01-08-30   redstar    l_fEnableRefresh  추가 실제 lcd로 화면이 갱신되는 것을 조절할 수 있도록 함 
** 01-08-30   redstar    EQS_EndModalIO   함수수정 (draw event를 바로 받을 수 있도록 변경)
** 01-08-29   redstar    EQS_StartModalIO 함수수정 (modal 컨트롤만 draw 이벤트를 받도록 수정)
** 01-08-23   redstar    partial update routine 추가 
**                       sMoveSlideWindow  함수 수정( focus가 변화된 컨트롤만 다시 그리도록 함 )
** 01-06-28   redstar    MODAL이 종료된 이후는  WM_APPLET_MODE 모드로 돌아가도록 수정 
**                       컨트롤이 deactive 되었다가 active되는데 active되는 시점이 모호하기 
**                       때문에 명시적으로 deactive되도록 함.
** 01-03-15   redstar    VIEW DRAW 영역만 다시 그리는 모드 추가 및 control validity
**                       체크함수 추가
** 01-02-08   redstar    SetCurrentWindowIOC 수정 (focus 변경 시 
**                       EQE_IO_FOCUS_CHG 이벤트 발생시킴
** 01-01-14   redstar    EQS_FitCanvasSize 함수에서 sliding window의 dy 크기를 
**                       조정하도록 변경
** 01-01-02   redstar    timer handler 관련  FeedTimerEvent 수정. 시스템 시간을 
**                       이용하여 timer 관련 지난 시간 보정.
** 00-12-19   girin      WM_MODAL_IO_MODE 추가. Modal I/O Control 이 생성되는
    경우, SWindow의 윈도우 모드를 변경하지 않도록 함. 대신 GetWindowMode() 
    함수에서 GET_MODAL()로 Modal 상태인지 검사하여 WM_MODAL_IO_MODE 를 리턴.
    (이전엔 IO Mode 로 바꾸었었음).  이렇게 하는 이유는 I/O Mode에서 Modal 
    I/O Control 이 생성된 후 소멸되어도 여전히 I/O Mode를 유지하도록 하기 위해
    이며 I/O Mode 상태에서 System Message Box가 생성되는 경우가 이에 해당함.
**
** 00-12-07   redstar    EQUS Event 와 SKY_Event 분리를 위한 함수 추가 및 수정
**
** 00-11-29   girin      중복된 Display 갱신에 의한 성능 저하를 막기 위해선 
                         두가지 측면이 고려되어야 함.

  첫째. 각 event processing entity (i.e. Applet, I/O Control, SWindow) 에 의해 
  EQE_APP_DRAW 가 처리되면서 LCD Buffer에 무언가를 그릴때 화면 갱신이 필요 없는 
  entity로는 EQE_APP_DRAW 이벤트가 전달되지 않도록 해야 한다.

  둘째. UpdateDisplay() / RefreshDispaly() 의 redundent function call에 의해 
  LCD Screen 으로 데이타가 불필요하게 전송되는 것을 방지하여야 한다.

  우선 첫째 문제를 해결하기 위해 EQE_APP_DRAW 이벤트가 발생되는 경우를 열거
  해 보면 다음과 같다.

  (1) Applet 자신이 다시 그려질 필요에 의해 (EQE_APP_DRAW, 0, 0)를 발생.
  (2) Inactive I/O Control 이 Timer 이벤트 등에 의해 자신에게만 DRAW 이벤트를
      보내고자 함. (EQE_APP_DRAW, hControl, 0)
  (3) I/O Control 생성 시 (EQE_APP_DRAW, hControl_Newly_Created, 0)
  (4) I/O Control 이 제공하는 API를 사용하여 대상 I/O Control 의 상태에 변화를
      주고 (EQE_APP_DRAW, hControl, 0) 
  (5) [Engine] APPLET Mode에서 I/O Mode로 전환 시 (EQE_APP_DRAW, 0, 0)
  (6) [Engine] EQE_APP_START in-queue 시 바로 이어서 (EQE_APP_DRAW, 0, 0)
  (7) [Engine] EQE_APP_RESUME in-queue 시 바로 이어서 (EQE_APP_DRAW, 0, 0)
  (8) [Engine] sMoveSlideWindow() 에서 Canvas 이동 혹은 IOC focus이동 시 
      (EQE_APP_DRAW, 0, 0)
  (9) EQS_SetIOCFocus() / EQS_SetIOCActive() / EQS_EndIO() / 
      EQS_DeleteControl() 수행 시 (EQE_APP_DRAW, 0, 0)



  아래와 같은 방식으로 EQE_APP_DRAW 이벤트를 처리하도록 함.
  (1) (EQE_APP_DRAW, hControl, 0) 이벤트는 hControl을 핸들로 가지는 
      I/O Control에만 전달.
  (2) (EQE_APP_DRAW, 0, 0) 이벤트는 Draw Layer 순서에 따라 모든 entity에
      전달.
  (3) APPLET MODE에서 I/O MODE로 전환 시 (EQE_APP_DRAW, hControl, 0)를 
      발생시켜 새로 Active되는 I/O Control에만 전달
  (4) (EQE_APP_DRAW, 0, 0) 이벤트 in-queue 시 현재 Aplet-Event-Queue에 
      있는 모든 EQE_APP_DRAW 이벤트를 없애고 (EQE_APP_DRAW, 0, 0)를 queue
      tail에 넣음.
  (5) (EQE_APP_DRAW, 0, 0) 이벤트가 이미 Applet-Event-Queue에 있는 경우 
      모든 새로운 EQE_APP_DRAW이벤트는 더이상 in-queue하지 않고 대신 
      (EQE_APP_DRAW, 0, 0) 를 queue tail로 이동.
  (6) (EQE_APP_DRAW, hControl, 0)이벤트를 in-queue하려 할때 
      (EQE_APP_DRAW, 0, 0) 가 queue에 없고 (EQE_APP_DRAW, hControl, 0)가 이미
      있는 경우 (EQE_APP_DRAW, hCOntrol, 0)를 queue tail 로 이동.
  (7) sMoveSlideWindow() 에서 Canvas 이동 시 (EQE_APP_DRAW, 0, 0) in-queue
  (8) sMoveSlideWindow() 에서 IOC Focus만 이동 시 
      (EQE_APP_DRAW, hPrevFocusedIOC, 0), (EQE_APP_DRAW, hNewlyFocusedIOC, 0)
      두개 이벤트를 in-queue.
  (9) EQS_SetIOCFocus() / EQS_SetIOCActive() / EQS_EndIO() / EQS_DeleteControl()
      은 일단 (EQE_APP_DRAW, 0, 0)를 in-queue


  #ifdef FEED_DRAW_EVENT_SELECTIVELY 로 featuring.





** 00-07-18   jrkwon     Change EQS_Init, InitDisplay.
**                       Add ClearDisplay, EQS_RefreshDisplay
**                       Remove some global variable and external modules 
**                       access.
**                       Increase memory pool size to 10K
** 00-07-17   jrkwon     Change NUI to EQS
** 00-06-28   girin      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "msg.h"
#include "eqs.h"
//#include "eqa.h"
//#include "eqs_swin.h" // Why didn't you put this header file into eqs.h ? That's
                      // becase ... 
#ifdef MIN
#undef MIN
#endif
#ifdef MAX
#undef MAX
#endif 

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))


/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
////////////////////////
// Internal Features  //
////////////////////////
#define USE_PARTIAL_DISPLAY_UPDATE  1

#define FEED_DRAW_EVENT_SELECTIVELY // Feed EQE_APP_DRAW event to the entities
                                    // that need it.

#define EXTENDED_EVENT_REGIME       // Split event regime. Make difference between
                                    // EQUS events and other events which are used
                                    // by external terrestrial. 

//#define SURPLUS_APP_EVENT           // Send EQE_APP_BEFORE_STAR and 
                                    // EQE_APP_AFTER_END event for debug purpose.

//#define MEM_LEAK_CHECK              // Memory leak check.

/* rosa 01-02-07 --> @why, what */
//#define DIRECT_APPLET_DRAW  --------> move to eqs_aapi.h
/* <-- */


// Features. end. //---------------------------------------------------------------

#define MAX_CONTROL_PER_APPLET      50
#define MAX_APPLET_EVENT_QUEUE_SIZ  20
#define MAX_APPLET_STATE_STACK_SIZ  30


                                                      //redstar  00.12.07
#define IS_VALID_EVENT_ID(EventID)  EventID < EQS_OFFSET_MAX_EVENT ? TRUE : FALSE
        
// (*CurrentAppletState.pApplet)() function call using function pointer should always 
// use '&CurrentEvent' as input parameter , because the current applet alway process
// current event , not some other next event. To guarantee this, use macro !!!
#ifdef EXTENDED_EVENT_REGIME
#define FEED_CURRENT_EVENT_TO_CURRENT_APPLET \
                         l_pfnDispatcher(*CurrentAppletState.pApplet, &CurrentEvent)

#define FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC \
                        l_pfnDispatcher(l_pfnDefEventProc, &CurrentEvent)
#else
#define FEED_CURRENT_EVENT_TO_CURRENT_APPLET \
                          (*CurrentAppletState.pApplet)(&CurrentEvent)

#define FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC \
                            EQS_DefEventProc(&CurrentEvent)

#endif

#define SLIDING_UNIT                EQS_CharHeight('A') // 13
#define DEFAULT_CANVAS_SIZE         200

#define IS_MODAL(hControl) ((EQC_GET_TYPE(hControl) == CT_POPUP_MENU) || (EQC_GET_TYPE(hControl) == CT_MESSAGE_BOX))
#define GET_MODAL() (l_hModalControl)
#define SET_MODAL(hControl) (l_hModalControl = hControl)
#define RESET_MODAL() (l_hModalControl = INVALID_CONTROL_HANDLE)


#ifdef DIRECT_APPLET_DRAW
STATIC BOOL l_fIsDirectAppletDraw = FALSE;
#define IS_DIRECT_APPLET_DRAW()             (l_fIsDirectAppletDraw)
#define SET_DIRECT_APPLET_DRAW(fDirect)     (l_fIsDirectAppletDraw = (fDirect))
#endif


//.......................................................................................

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
 
enum tagAPPLET_START_FLAG
{ 
    EQS_APPLET_NONE_READY, 
    EQS_APPLET_CHILD_READY, 
    EQS_APPLET_SIBLING_READY, 
    EQS_APPLET_END_READY 
};

typedef enum  tagAPPLET_START_FLAG  APPLET_START_FLAG;

struct tagAPPLET_STATE {
    EQS_APPLET          *pApplet;
    BOOL                fSubRoot;
    
    HTIMER              TimerHandle; // restar  --> for timer info
    COORD               dyCanvasSize;
    CONST BYTE*         pszSWindowTitle;
    UINT8               nCML;
    WORD                wCmlAttr;
    UINT8               AttachedIOControls[MAX_CONTROL_PER_APPLET];
	COORD               CanvasOffset;
	UINT8               nFocusedIOC;
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM
    INT16               nCurMemAllocCount;
#endif
#endif
};
typedef struct tagAPPLET_STATE  APPLET_STATE;

struct tagWIN {
    
    COORD yWindowOffset;            // y-axis offset of the Sliding Window on MM_TEXT mapping mode.
    COORD dyWindowSize;             // y-axis size of the Sliding Window.
    COORD dyCanvasSize;             // y-axis size of the Applet Canvas.
    UINT8 CurrentIOCIndex;           // handle to the IO Control which is ready to get active.
    WINDOW_MODE tWindowMode;        // Applet mode or IO mode.
}; 

/*
**---------------------------------------------------------------------------
**  General Declarations
**---------------------------------------------------------------------------
*/

//extern VOID pEQA_Root(EQS_EVENT *pInEvent);



/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
#ifdef SKTT_RELEASE
BOOL g_fSKTT_Release = TRUE;
#else
BOOL g_fSKTT_Release = FALSE;
#endif


/* timer set function */
BOOL EQS_API (*g_pfnSetTimer)(EQS_TIMER_E nTimerID, LONG lInterval, BOOL fRepeat);
void EQS_API (*g_pfnGetSystemTime)(QWORD CurrentTime);         // redstar 2001.1.2
void EQS_API (*l_pfnSetPaceMaker)(LONG lIntervalMS, BOOL fRepeat);




/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL BYTE l_abMemPool[MAX_EQUS_MEM_POOL_SIZE];

// Event Var.
#ifdef EXTENDED_EVENT_REGIME
LOCAL EQS_EVENT CurrentEvent = { EQE_NONE, (WORD)0, (DWORD)0, 0};
LOCAL EQS_EVENT NextEvent = { EQE_NONE, (WORD)0, (DWORD)0, 0};
#else
LOCAL EQS_EVENT CurrentEvent = { EQE_NONE, (WORD)0, (DWORD)0 };
LOCAL EQS_EVENT NextEvent = { EQE_NONE, (WORD)0, (DWORD)0 };
#endif
LOCAL BOOL      bPassEventFlag = FALSE;
LOCAL WINDOW_TSYMBOL_MODE  l_WindoTitleSymbolMode;

// Applet State.
LOCAL APPLET_STATE CurrentAppletState;
LOCAL APPLET_STATE NextAppletState;
LOCAL APPLET_STATE ParentAppletState;
LOCAL APPLET_STATE ChildAppletState;

LOCAL EQS_APPLET *EndDestApplet;
LOCAL APPLET_START_FLAG AppletStartFlag = EQS_APPLET_NONE_READY;
LOCAL EQS_POPUP PopToPlace = EQS_POP_TO_PARENT;
LOCAL BOOL bDispInherit = FALSE;

// Sliding Window.
LOCAL struct tagWIN sSlideWindow = { 0, 0, 0, 0, WM_APPLET_MODE };

// Applet State Stack
LOCAL APPLET_STATE Applet_State_Stack[MAX_APPLET_STATE_STACK_SIZ];
LOCAL UINT8 uiAppletStackIndex = 0;

// Applet Event Queue
LOCAL EQS_EVENT Applet_Event_Queue[MAX_APPLET_EVENT_QUEUE_SIZ];
LOCAL UINT8 uiQueueHead = 1;
LOCAL UINT8 uiQueueTail = 0;

LOCAL BOOL l_fRefreshDisplay;
LOCAL BOOL l_fRefreshControlDisplay;
LOCAL BOOL l_fEnableRefresh;
/* Root Applet function pointer */
LOCAL EQS_APPLET *pEQA_Root;

/* run current applet with event */
LOCAL VOID EQS_API (*l_pfnDispatcher)(EQS_APPLET *pfnApplet, EQS_EVENT* pEvent);

/* run default event handler with event */
LOCAL EQS_APPLET *l_pfnDefEventProc;

/* Display refresh function pointer */
LOCAL VOID EQS_API (*l_pfnRefreshDisplayDevice)(VOID);
VOID EQS_API (*l_pfnRefreshDisplayRect)(BYTE* pBuffer, UINT8 xleft, UINT8 ytop, UINT8 xright, UINT8 ybottom );

// Modal I/O Control Handle
LOCAL HCONTROL l_hModalControl = INVALID_CONTROL_HANDLE;



/*
**---------------------------------------------------------------------------
**  Local Function Declaration
**---------------------------------------------------------------------------
*/
LOCAL EQS_RESULT         InitEventQueue(void);
LOCAL UINT8              sPeekEventQueue(EQS_EVENT_ID EventID);
LOCAL void               sSqueezeEventOnEventQueue(UINT8 nIndex);
LOCAL void               sPurgeEventOnEventQueue(EQS_EVENT_ID EventID);
LOCAL EQS_RESULT         GetEventQueue(EQS_EVENT* pInEvent);
LOCAL EQS_RESULT         AddEventQueue(EQS_EVENT* pInEvent);

LOCAL EQS_RESULT         sMakeEvent(EQS_EVENT* pAnEvent, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam);

LOCAL EQS_RESULT         InitAppletState(APPLET_STATE* pAppState, EQS_APPLET *pAnApplet);
LOCAL EQS_RESULT         InitAppletStateDispUntouched(APPLET_STATE* pAppState, EQS_APPLET *pAnApplet);
//LOCAL EQS_RESULT         InitDisplay(SCREEN_INFO* pScreenInfo);
//LOCAL EQS_RESULT         InitDisplay(void);
//LOCAL EQS_RESULT         ClearDisplay(CLEAR_SCREEN_TYPE ClearType);
//LOCAL EQS_RESULT         ClearDisplay(VOID);
LOCAL EQS_RESULT         PushAppletState(APPLET_STATE* pInState);
LOCAL EQS_RESULT         PopAppletState(APPLET_STATE* pOutState);

LOCAL EQS_RESULT         Prepare2StartNextApplet( EQS_APPLET *pAnApplet, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam, BOOL bDisp);
LOCAL EQS_RESULT         Prepare2EndApplet(EQS_POPUP PopToWhere, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam);

LOCAL BOOL               DeserveEventByIOC(EQS_EVENT_ID EventID);
//restar  2001.01.02  --- for timer handler
LOCAL EQS_RESULT         FeedTimerEvent(void);


LOCAL EQS_RESULT         FeedEventIOControl(EQS_EVENT* pInEvent);
//LOCAL EQS_RESULT         FeedEventAllIOControl(EQS_EVENT* pInEvent);
//LOCAL EQS_RESULT         FeedEventAllIOCandSWin(EQS_EVENT* pInEvent);
#ifdef FEED_DRAW_EVENT_SELECTIVELY
LOCAL EQS_RESULT         sFeed_EQE_APP_DRAW_OnAnControl(void);
#endif
LOCAL EQS_RESULT         sFeed_EQE_APP_DRAW(void);
//LOCAL void               sDrawFocusMark(void);  // redstar 2001.06.26
LOCAL void               sGetSWindowViewAreaRect(RECT* pRect);
LOCAL COORD              sGetWindowSize(void);
LOCAL BOOL               sValidAppletControl(HCONTROL hControl); //01.03.15 redstar
LOCAL EQS_RESULT         FreeIOCIndex(UINT8 hControl);
LOCAL EQS_RESULT         sDeleteAllAttachedIOControl(APPLET_STATE* pAnState);
LOCAL BOOL               HaveFocus(HCONTROL hControl);
LOCAL BOOL               IsActive(HCONTROL hControl);
LOCAL EQS_EVENT_CATEGORY GetCategory(EQS_EVENT_ID EventID);

LOCAL BOOL               IsAnIOCInWindowExclusive(HCONTROL hControl);
LOCAL BOOL               IsAnIOCInWindowInclusive(HCONTROL hControl);

LOCAL BOOL               sMoveSlideWindow(EQS_EVENT_ID EventID);
LOCAL WINDOW_MODE        GetWindowMode(void);
LOCAL void               SetWindowCML(UINT8 nCML);
LOCAL UINT8              GetWindowCML(void);
LOCAL void               SetWindowMode(WINDOW_MODE);
LOCAL HCONTROL           GetFocusedIOC(void);
LOCAL HCONTROL           GetActiveIOC(void);
LOCAL HCONTROL           GetNthIOC(UINT8 unNth);
LOCAL HCONTROL           GetLastIOC(void);
LOCAL EQS_RESULT         SetCurrentWindowIOC(HCONTROL hControl);
//LOCAL HCONTROL           GetPrevWindowIOC(void);
//LOCAL HCONTROL           GetNextWindowIOC(void);
//LOCAL HCONTROL           GetCurrentWindowIOC(void);
LOCAL HCONTROL           GetPrevFocusibleIOC(void);
LOCAL HCONTROL           GetNextFocusibleIOC(void);
LOCAL HCONTROL           GetFirstFocusibleIOC(void);
LOCAL HCONTROL           GetLastFocusibleIOC(void);
LOCAL INT8               CompareIOCPosition(HCONTROL hCont1, HCONTROL hCont2);
LOCAL void               sAlignFocus(void);

LOCAL BOOL               sDeserveEventByIOC(HCONTROL hControl, EQS_EVENT_ID EventID);
LOCAL BOOL               sIsActivateControl(HCONTROL hControl, EQS_EVENT_ID EventID);
LOCAL void               ActivateControl(HCONTROL hControl, EQS_EVENT* pEvent);


/*
**---------------------------------------------------------------------------
**  Local Function Definitions
**---------------------------------------------------------------------------
*/
EQS_RESULT EQS_Initialize(EQS_APPLET *pStartApplet, 
                          EQS_APPLET *pRootApplet, 
//                          SCREEN_INFO* pScreenInfo,
                          void (*pfnDrawSWindowHeader)(void),
                          ROM CML_INFO_T*  pCMLInfo,
                          VOID EQS_API (*pfnDispatcher)(EQS_APPLET *pfnApplet, EQS_EVENT* pEvent),
                          EQS_APPLET *pfnDefEventProc,
                          BOOL EQS_API (*pfnSetTimer)(EQS_TIMER_E nTimerID, LONG lInterval, BOOL fRepeat),
                          void EQS_API (*pfnGetSystemTime)(QWORD CurrentTime),
                          ROM BYTE* ROM *aszToken[],
                          VOID EQS_API (*pfnRefreshDisplayDevice)(VOID),
						  VOID EQS_API (*pfnRefreshDisplayRect)(BYTE* , UINT8 xleft, UINT8 ytop, UINT8 xright, UINT8 ybottom ),
						  void EQS_API (*pfnSetPaceMaker)(LONG lIntervalMS, BOOL fRepeat))
{
    if (EQS_InitMemory(l_abMemPool, MAX_EQUS_MEM_POOL_SIZE) != 0)
    {
        return RESULT_OUT_OF_MEMORY; // Catastrophy !!!
    }

//    InitDisplay(pScreenInfo);
    
    EQS_InitLocale(aszToken);

    EQS_InitSWindowHeader(pfnDrawSWindowHeader);
    EQS_InitCMLInfo(pCMLInfo);

    l_fRefreshDisplay = FALSE;
	l_fRefreshControlDisplay = FALSE;
	l_fEnableRefresh         = TRUE;
    pEQA_Root = pRootApplet;

    /* set function pointer */
    l_pfnRefreshDisplayDevice = pfnRefreshDisplayDevice;
	l_pfnRefreshDisplayRect   = pfnRefreshDisplayRect;
    l_pfnDispatcher = pfnDispatcher;
    l_pfnDefEventProc = pfnDefEventProc;
    g_pfnSetTimer = pfnSetTimer;
    g_pfnGetSystemTime = pfnGetSystemTime;
	l_pfnSetPaceMaker  = pfnSetPaceMaker;	

	l_WindoTitleSymbolMode = WTSM_NORMAL_MODE;

    CurrentEvent.EventID = EQE_NONE;
    CurrentEvent.wParam = 0;
    CurrentEvent.dwParam = 0;
#ifdef EXTENDED_EVENT_REGIME
    CurrentEvent.dwExtEventID = 0;
#endif

    NextEvent.EventID = EQE_NONE;
    NextEvent.wParam = 0;
    NextEvent.dwParam = 0;

    bPassEventFlag = FALSE;

    InitAppletState(&CurrentAppletState, pStartApplet);
    InitAppletState(&NextAppletState, pStartApplet);
    InitAppletState(&ParentAppletState, pStartApplet);
    InitAppletState(&ChildAppletState, pStartApplet);

    PopToPlace = EQS_POP_TO_PARENT;
    bDispInherit = FALSE;

    uiAppletStackIndex = 0;

    InitEventQueue();
    //InitDisplay();

    EQC_InitControl();//NIOC_InitCommonIOCHeader();
    //redstar  2000.09.21
    EQS_InitTimers();
    CurrentAppletState.TimerHandle = EQS_InitAppletTimer();
    //CurrentAppletState.dyCanvasSize = DEFAULT_CANVAS_SIZE;
    CurrentAppletState.dyCanvasSize = EQS_GetMaxHeight();
    CurrentAppletState.nCML = 0;
    CurrentAppletState.wCmlAttr = WA_SHOW_HEADER;
	//l_pfnSetPaceMaker(1000,TRUE);

    
    return RESULT_OK;
}

BOOL EQS_ReinitMemory(void)
{
    if (EQS_InitMemory(l_abMemPool, MAX_EQUS_MEM_POOL_SIZE) != 0)
    {
        return FALSE;
    }
	return TRUE;
}

#if 0
LOCAL VOID EQS_API sSetParam(EQS_EVENT_ID EventID, EQS_EVENT* pEQSEvent)
{
    pEQSEvent->EventID = EventID;

    switch (EventID)
    {
    default:
        pEQSEvent->wParam = 0;
        pEQSEvent->dwParam = 0;
        break;
    }
}
#endif

//VOID EQS_Scheduler(EQS_EVENT_ID EventID)
VOID EQS_Scheduler(EQS_EVENT *pEvent)
{
//    HTIMER      hTimer;
    HCONTROL      hControl;

	if( pEvent->EventID == EQE_KEY_NONE )
		return ;
    AddEventQueue(pEvent);

    while (GetEventQueue(&CurrentEvent) == RESULT_OK)
    {
        AppletStartFlag = EQS_APPLET_NONE_READY; 
        bDispInherit = FALSE;
        bPassEventFlag = FALSE;
        
        //redstar  2000.09.21  process local timer event
        if( IsEQSTimerEvent(CurrentEvent.EventID) == TRUE )    
        {            
			FeedTimerEvent();     // redstar  2001.1.2 			
        }
        else if ((CurrentEvent.EventID == EQE_APP_START) || (CurrentEvent.EventID == EQE_APP_RESUME))
        {									
			
            sSlideWindow.yWindowOffset = CurrentAppletState.CanvasOffset;   // y-axis offset of the Sliding Window on MM_TEXT mapping mode.
            sSlideWindow.dyCanvasSize = CurrentAppletState.dyCanvasSize;    // y-axis size of the Applet Canvas.
            // sSlideWindow.CurrentIOCIndex is set by sAlignFocus().
            SetWindowMode(WM_APPLET_MODE);
            sSlideWindow.dyWindowSize = sGetWindowSize();
			sSlideWindow.CurrentIOCIndex = CurrentAppletState.nFocusedIOC;

            SetSlidingWindowState(TRUE);
            SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
			

#ifdef MEM_LEAK_CHECK
            if (CurrentEvent.EventID == EQE_APP_START)
            {
                CurrentAppletState.nCurMemAllocCount = EQS_GetMemAllocCount();
            }
#endif
#ifdef SURPLUS_APP_EVENT
            if (CurrentEvent.EventID == EQE_APP_START)
            {
                NextEvent = CurrentEvent;

                sMakeEvent(&CurrentEvent, EQE_APP_BEFORE_START, EQS_NA, EQS_NA);
                FEED_CURRENT_EVENT_TO_CURRENT_APPLET;

                CurrentEvent = NextEvent;
            }
#endif
            ///////////////////////////////////////////////////
            // Feed the current event to the current applite //
            ///////////////////////////////////////////////////
            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                          
            ///////////////////////////////////////////////////////////
            // Feed the current event to DefEventproc(), if required //
            ///////////////////////////////////////////////////////////
            if (bPassEventFlag)
                FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC;
            bPassEventFlag = FALSE;
            //.........................................................

            if( CurrentEvent.EventID == EQE_APP_START)  //redstar 2001.01.12 
            {
                //!!!!!!!!!!!!
                sAlignFocus();
                //!!!!!!!!!!!!
            }
        }
        else if (GetWindowMode() == WM_NATIVE_MODE) 
        { // All events go to current applet.

            ///////////////////////////////////////////////////
            // Feed the current event to the current applite //
            ///////////////////////////////////////////////////
            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                          
            ///////////////////////////////////////////////////////////
            // Feed the current event to DefEventproc(), if required //
            ///////////////////////////////////////////////////////////
            if (bPassEventFlag)
                FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC;
            bPassEventFlag = FALSE;
            //.........................................................
        }
        else if (GetWindowMode() == WM_APPLET_MODE)
        {
            if (CurrentEvent.EventID == EQE_KEY_CENTER)
            {		
				hControl = GetFocusedIOC(); 
                if( hControl != INVALID_CONTROL_HANDLE) 
                {   // Toggle SWindow mode to IO-mode.

					if( EQC_GET_ENABLE(hControl) && EQC_GET_FOCUS(hControl) )
					{
						ActivateControl(hControl, &CurrentEvent);
					}
					else
					{
						FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
					}
                }
                else if (EQS_DeserveEventByCML(CurrentEvent.EventID)) // Function Key.
                {
                    EQS_ProcessCMLEvent(&CurrentEvent);
                }
                else // Applet-mode and input event is EQE_KEY_CENTER but there is no I/O Control
                {
                    // Put EQE_KEY_CENTER into Applet-Event-Queue to be processed by current applet.
                    ///////////////////////////////////////////////////
                    // Feed the current event to the current applite //
                    ///////////////////////////////////////////////////
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                              
                    ///////////////////////////////////////////////////////////
                    // Feed the current event to DefEventproc(), if required //
                    ///////////////////////////////////////////////////////////
                    if (bPassEventFlag)
                        FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC;
                    bPassEventFlag = FALSE;
                    //.........................................................
                }
            }
			else if( CurrentEvent.EventID == EQE_APP_IO_END )
			{				
				if( CurrentEvent.wParam == EQS_GetFocusedIOC())
				{ // 다음 가능한 컨트롤로 focus 이동
					HCONTROL hControl;
					switch( CurrentEvent.dwParam )
					{
					case EQE_KEY_UP:
						hControl = GetPrevFocusibleIOC();
						break;
					case EQE_KEY_CENTER:
						hControl = INVALID_CONTROL_HANDLE;
						break;
					case EQE_KEY_DOWN:
						hControl = GetNextFocusibleIOC();
						break;
					default:
						hControl = GetNextFocusibleIOC();
						break;
					}					
					
					if( hControl != INVALID_CONTROL_HANDLE )
					{
						//sMoveSlideWindow( EQE_KEY_DOWN );
						EQS_SetIOCFocus(hControl);
					}
				}						
			}
#if 0
			else if( CurrentEvent.EventID == EQE_SYS_PACEMAKER )
			{

				//FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC;
				FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
				CurrentEvent.EventID = EQE_APP_DRAW;				
				CurrentEvent.wParam  = EQS_GetFocusedIOC();
				if( CurrentEvent.wParam != INVALID_CONTROL_HANDLE )
				{
					sFeed_EQE_APP_DRAW_OnAnControl();						
					l_fRefreshDisplay = TRUE;					
				}

			}
#endif
            else if ((CurrentEvent.EventID == EQE_KEY_UP) || (CurrentEvent.EventID == EQE_KEY_DOWN) ) //|| 
                     //(CurrentEvent.EventID == EQE_KEY_LEFT) || (CurrentEvent.EventID == EQE_KEY_RIGHT)) // Browse keys.
            {
			
				hControl = GetFocusedIOC(); 
				if( sIsActivateControl(hControl, CurrentEvent.EventID) )
				{
                    ActivateControl(hControl, &CurrentEvent);
				}
				else
				{						
					//if (FALSE == sMoveSlideWindow(CurrentEvent.EventID))
                    sMoveSlideWindow(CurrentEvent.EventID);
					{ // If there were no window sliding...

						///////////////////////////////////////////////////
						// Feed the current event to the current applite //
						///////////////////////////////////////////////////
						FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                              
						///////////////////////////////////////////////////////////
						// Feed the current event to DefEventproc(), if required //
						///////////////////////////////////////////////////////////
						if (bPassEventFlag)
							FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC;
						bPassEventFlag = FALSE;
						//.........................................................
					}
				}
            }
            else if (EQS_DeserveEventByCML(CurrentEvent.EventID))
            {
                EQS_ProcessCMLEvent(&CurrentEvent);
            }
            else if (CurrentEvent.EventID == EQE_APP_DRAW)
            {
                sFeed_EQE_APP_DRAW();
                //l_fRefreshDisplay = TRUE;
            }
            else
            {
                ///////////////////////////////////////////////////
                // Feed the current event to the current applite //
                ///////////////////////////////////////////////////

				hControl = GetFocusedIOC(); 
				if( sIsActivateControl(hControl, CurrentEvent.EventID) )
				{// active focused control with key
                    ActivateControl(hControl, &CurrentEvent);
				}
				else
				{
					FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
				}
                          
                ///////////////////////////////////////////////////////////
                // Feed the current event to DefEventproc(), if required //
                ///////////////////////////////////////////////////////////
                if (bPassEventFlag)
                    FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC;
                bPassEventFlag = FALSE;
                //.........................................................
            }
        }
        else if ((GetWindowMode() == WM_IO_MODE) || (GetWindowMode() == WM_MODAL_IO_MODE))
        {
            if( CurrentEvent.EventID == EQE_APP_IO_MODE )  //redstar 2001.02.28
            {				
                FEED_CURRENT_EVENT_TO_CURRENT_APPLET;						
                EQC_HandleEvent(GetActiveIOC(), &CurrentEvent);
            }
			else if( CurrentEvent.EventID == EQE_APP_IO_DELETE )
			{
				EQC_DeleteControl(CurrentEvent.wParam, TRUE); //01.04.15
				//EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
			}
            else if (CurrentEvent.EventID == EQE_APP_DRAW)
            {

                if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
                {  						
					CurrentEvent.wParam = hControl;				
                } 				
                sFeed_EQE_APP_DRAW();                
                //l_fRefreshDisplay = TRUE;				
            }
            else
            {
                if (DeserveEventByIOC(CurrentEvent.EventID))
                {
                    FeedEventIOControl(&CurrentEvent);
                }
                else if (EQS_DeserveEventByCML(CurrentEvent.EventID))
                {
                    EQS_ProcessCMLEvent(&CurrentEvent);
                }
                else
                {
                    ///////////////////////////////////////////////////
                    // Feed the current event to the current applite //
                    ///////////////////////////////////////////////////
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                          
                    ///////////////////////////////////////////////////////////
                    // Feed the current event to DefEventproc(), if required //
                    ///////////////////////////////////////////////////////////
                    if (bPassEventFlag)
                        FEED_CURRENT_EVENT_TO_DEF_EVENT_PROC;
                    bPassEventFlag = FALSE;
                    //.........................................................
                }
            }
        }
        else
        {
            ASSERT(0);
        }


        if (AppletStartFlag != EQS_APPLET_NONE_READY)
        {
			HCONTROL hModal;
			if ((hModal = GET_MODAL()) != INVALID_CONTROL_HANDLE)
			{
				EQC_DeleteControl(hModal, FALSE);            			
				RESET_MODAL();
			}

            // feed the last event to current applite.
            switch( AppletStartFlag )
            {
            case EQS_APPLET_CHILD_READY:
				hControl = EQS_GetActiveIOC(); // redstar 2001.06.26
				if( hControl != INVALID_CONTROL_HANDLE )
				{
					sMakeEvent(&CurrentEvent, EQE_APP_IO_END, (WORD)hControl, EQS_NA); 
					EQC_HandleEvent(hControl, &CurrentEvent);
				}
                sMakeEvent(&CurrentEvent, EQE_APP_SUSPEND, (WORD)0, (DWORD)0);
                break;

            case EQS_APPLET_SIBLING_READY:
                sMakeEvent(&CurrentEvent, EQE_APP_END, (WORD)0, (DWORD)0);
                break;

            case EQS_APPLET_END_READY:
                sMakeEvent(&CurrentEvent, EQE_APP_END, (WORD)0, (DWORD)0);
                break;

            default:
                ASSERT(0);
                break;
            }

            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;

            //////////////////////////////////////////////////////////////////////////////
            // Just now, spawning condition may be changed by executing spawning API    // 
            // call on EQS_APP_END     event of dying current applite. (NOTE: Spawning  //
            // API includes EQS_StartChile(), EQS_StartChildEvent(),                    //
            // EQS_StartChildCurrentEvent(), EQS_StartSibling(),                        //
            // EQS_StartSiblingEvent() and EQS_StartSiblingCurrentEvent().              //
            // Ending API includes EQS_End(), EQS_EndEvent() and EQS_EndCurrentEvent(). //
            // In short, 'Applite manipulation API is divided into two groups, i.e.     //
            // spawing API and ending API. )                                            //
            //////////////////////////////////////////////////////////////////////////////


            if ( (AppletStartFlag == EQS_APPLET_CHILD_READY)   ||
                 (AppletStartFlag == EQS_APPLET_SIBLING_READY)
                 )
            {

				CurrentAppletState.nFocusedIOC  = sSlideWindow.CurrentIOCIndex;
				CurrentAppletState.CanvasOffset = sSlideWindow.yWindowOffset;
                if (AppletStartFlag == EQS_APPLET_CHILD_READY)
                {
                    // push current applite                    
                    ParentAppletState = CurrentAppletState;
                    PushAppletState(&CurrentAppletState);

                    ////////////////////////
                    // Set current applet //
                    ////////////////////////
                    ASSERT(NextAppletState.pApplet != NULL);

					if (!IS_DIRECT_APPLET_DRAW())
					{
						ClearScreen(); // ClearDisplay();
					}
                    InitAppletState(&CurrentAppletState, NextAppletState.pApplet);

                    CurrentAppletState.TimerHandle = EQS_InitAppletTimer();  //redstar prepare new local timer info

                }
                else  // EQS_APPLET_SIBLING_READY
                {
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM
                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                    if (bDispInherit)
                    {
                        // Untouch the attached I/O Controls
                        ASSERT(NextAppletState.pApplet != NULL);
                        InitAppletStateDispUntouched(&CurrentAppletState, NextAppletState.pApplet);
                    }
                    else
                    {
                        sDeleteAllAttachedIOControl(&CurrentAppletState);
						if (!IS_DIRECT_APPLET_DRAW())
						{
							ClearScreen(); // ClearDisplay();
						}					
                        ASSERT(NextAppletState.pApplet != NULL);
                        InitAppletState(&CurrentAppletState, NextAppletState.pApplet);
                        EQS_FreeAppletTimer();  //redstar  -- remove  local timer info
                        CurrentAppletState.TimerHandle = EQS_InitAppletTimer();
                    }

                }

                ///////////////////////////////////////////////////
                // Set Applet-Event-Queue for new current applet //
                ///////////////////////////////////////////////////
        
                //InitEventQueue();   redstar 01.08.27

                if (NextEvent.EventID != EQE_APP_START)
                {
                    sMakeEvent(&CurrentEvent, EQE_APP_START, (WORD)NextEvent.EventID, (DWORD)0);
                    AddEventQueue(&CurrentEvent);
                }
                AddEventQueue(&NextEvent);
                if( bDispInherit == FALSE )  //redstar 2001.03.22 for sibling
				{
					sMakeEvent(&NextEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
					AddEventQueue(&NextEvent);
				}

                continue; // Not necessary , just for instructive use.
                //.................................................

            }
            else if (AppletStartFlag == EQS_APPLET_END_READY)
            {
                switch (PopToPlace)
                {
                case EQS_POP_TO_PARENT:
                    ChildAppletState.pApplet = CurrentAppletState.pApplet;
                    sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                    PopAppletState(&CurrentAppletState);

                    break;

                case EQS_POP_TO_PARENT_OF_PARENT:
                    sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                    PopAppletState(&CurrentAppletState);

                    sMakeEvent(&CurrentEvent, EQE_APP_END, (WORD)0, (DWORD)0);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                    ChildAppletState.pApplet = CurrentAppletState.pApplet;
                    sDeleteAllAttachedIOControl(&CurrentAppletState);
            
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                    PopAppletState(&CurrentAppletState);

                    break;

                case EQS_POP_TO_SUBROOT:
                    sDeleteAllAttachedIOControl(&CurrentAppletState);

#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                    for (PopAppletState(&CurrentAppletState); 
                         CurrentAppletState.fSubRoot == FALSE && 
                         CurrentAppletState.pApplet != pEQA_Root; 
                         PopAppletState(&CurrentAppletState))
                         {

                            sMakeEvent(&CurrentEvent, EQE_APP_END, (WORD)0, (DWORD)0);
                            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                            ChildAppletState.pApplet = CurrentAppletState.pApplet;
                            sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                            sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                         }

                    break;

                case EQS_POP_TO_PARENT_OF_SUBROOT:                    
                    sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                    for (PopAppletState(&CurrentAppletState); 
                         CurrentAppletState.fSubRoot == FALSE && CurrentAppletState.pApplet != pEQA_Root;
                         PopAppletState(&CurrentAppletState))
                         {
                            sMakeEvent(&CurrentEvent, EQE_APP_END, (WORD)0, (DWORD)0);
                            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                            sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                            sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                         }

                    sMakeEvent(&CurrentEvent, EQE_APP_END, (WORD)0, (DWORD)0);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                    ChildAppletState.pApplet = CurrentAppletState.pApplet;
                    sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                    PopAppletState(&CurrentAppletState);
            

                    break;

                case EQS_POP_TO_ROOT:
                    sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                    for (PopAppletState(&CurrentAppletState); 
                         CurrentAppletState.pApplet != pEQA_Root; 
                         PopAppletState(&CurrentAppletState))
                         {
                            sMakeEvent(&CurrentEvent, EQE_APP_END, (WORD)0, (DWORD)0);
                            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                            ChildAppletState.pApplet = CurrentAppletState.pApplet;
                            sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                            sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                         }


                    break;

                case EQS_POP_TO_DEST:
                    sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                    sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                     for (PopAppletState(&CurrentAppletState); 
                         CurrentAppletState.pApplet != EndDestApplet && 
                         CurrentAppletState.pApplet != pEQA_Root; 
                         PopAppletState(&CurrentAppletState))
                         {
                            sMakeEvent(&CurrentEvent, EQE_APP_END, (WORD)0, (DWORD)0);
                            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
                            ChildAppletState.pApplet = CurrentAppletState.pApplet;
                            sDeleteAllAttachedIOControl(&CurrentAppletState);
#ifdef MEM_LEAK_CHECK
#ifdef FEATURE_EQS_DEBUG_MEM

                    ASSERT(EQS_GetMemAllocCount() == CurrentAppletState.nCurMemAllocCount);
#endif
#endif
#ifdef SURPLUS_APP_EVENT
                            sMakeEvent(&CurrentEvent, EQE_APP_AFTER_END, EQS_NA, EQS_NA);
                            FEED_CURRENT_EVENT_TO_CURRENT_APPLET;
#endif
                         }


                    break;

                default:
                    break;
                }

                // Set parent applet state for EQS_GetParent()
                PopAppletState(&ParentAppletState);
                if (ParentAppletState.pApplet != pEQA_Root)
                    PushAppletState(&ParentAppletState);

                //InitEventQueue();   redstar 01.08.27
				if (!IS_DIRECT_APPLET_DRAW())
				{
					ClearScreen(); // ClearDisplay();
				}

                if (NextEvent.EventID != EQE_APP_RESUME)
                {
                    sMakeEvent(&CurrentEvent, EQE_APP_RESUME, (WORD)NextEvent.EventID, (DWORD)0);
                    AddEventQueue(&CurrentEvent);
                }
                AddEventQueue(&NextEvent);



                if( CurrentAppletState.pApplet != pEQA_Root )
                {
                    sMakeEvent(&NextEvent, EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
                    AddEventQueue(&NextEvent);
                }

                continue; // Not necessary , just for instructive use.
            }
            else
            {
                //TRACE0(ERR_TKN_INVALID_APPLET_START_FLAG);
                ASSERT(0); /* ERR_TKN_INVALID_APPLET_START_FLAG */
            }

        } // end-if (AppletStartFlag != EQS_APPLET_NONE_READY)

        EQS_RefreshDisplay();

    } // end-while (GetEventQueue(&CurrentEvent) == RESULT_OK)
}

/* Set the flag for updating display */
VOID EQS_API EQS_UpdateDisplay(VOID)
{
    l_fRefreshDisplay = TRUE;
}

VOID EQS_API EQS_UpdateControlDisplay(VOID)
{
	l_fRefreshControlDisplay = TRUE;
}

/* if fresh flag is true, refresh the display device */
VOID EQS_API EQS_RefreshDisplay(VOID)
{
    if(l_fRefreshDisplay)
    {
        l_pfnRefreshDisplayDevice();
		//l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(),0,0,128,160);
        l_fRefreshDisplay = FALSE;
    }
}

LOCAL EQS_RESULT        InitEventQueue(void)
{
    uiQueueHead = 1;
    uiQueueTail = 0;

    return RESULT_OK;
}

LOCAL UINT8 sPeekEventQueue(EQS_EVENT_ID EventID)
{
    UINT8 uiTraverseIndex;
    UINT8 uiCount; // for safety code.

    uiTraverseIndex = uiQueueTail; 
    for (uiCount = 0; uiCount < MAX_APPLET_EVENT_QUEUE_SIZ; uiCount++) // for-loop counting for safety code.
    {
        if (MOD_INCREMENT(uiTraverseIndex, MAX_APPLET_EVENT_QUEUE_SIZ) == uiQueueHead) // Not found.
            break;
        else
        {
            // increment, and then compare.
            uiTraverseIndex = MOD_INCREMENT(uiTraverseIndex, MAX_APPLET_EVENT_QUEUE_SIZ);

			ASSERT( uiTraverseIndex  < MAX_APPLET_EVENT_QUEUE_SIZ );
			
            if (EventID == Applet_Event_Queue[uiTraverseIndex].EventID) // Found
                return uiTraverseIndex;
        }
    }

    return MAX_APPLET_EVENT_QUEUE_SIZ; // Not found.
}

LOCAL void sSqueezeEventOnEventQueue(UINT8 nIndex)
{
    UINT8 nTraverseIndex;

	ASSERT( uiQueueTail  < MAX_APPLET_EVENT_QUEUE_SIZ );	
    for (nTraverseIndex = nIndex; 
        MOD_DECREMENT(nTraverseIndex, MAX_APPLET_EVENT_QUEUE_SIZ) != uiQueueTail; 
        nTraverseIndex = MOD_DECREMENT(nTraverseIndex, MAX_APPLET_EVENT_QUEUE_SIZ))
    {

		ASSERT( nTraverseIndex  < MAX_APPLET_EVENT_QUEUE_SIZ );
        Applet_Event_Queue[nTraverseIndex] 
            = Applet_Event_Queue[MOD_DECREMENT(nTraverseIndex, MAX_APPLET_EVENT_QUEUE_SIZ)];
		ASSERT( MOD_DECREMENT(nTraverseIndex, MAX_APPLET_EVENT_QUEUE_SIZ)  < MAX_APPLET_EVENT_QUEUE_SIZ );
    }

    uiQueueTail = MOD_INCREMENT(uiQueueTail, MAX_APPLET_EVENT_QUEUE_SIZ);
	ASSERT( uiQueueTail  < MAX_APPLET_EVENT_QUEUE_SIZ );
}

LOCAL void sPurgeEventOnEventQueue(EQS_EVENT_ID EventID)
{
    UINT8 nTraverseIndex;
    UINT8 nPutIndex;
    UINT8 nPutCount;

    
    nTraverseIndex = sPeekEventQueue(EventID);

    if (nTraverseIndex < MAX_APPLET_EVENT_QUEUE_SIZ) // There is events to be purged.
    {
        nPutCount = 1;

        for (nPutIndex = nTraverseIndex,
            nTraverseIndex = MOD_DECREMENT(nTraverseIndex, MAX_APPLET_EVENT_QUEUE_SIZ); 
            nTraverseIndex != uiQueueTail; 
            nTraverseIndex = MOD_DECREMENT(nTraverseIndex, MAX_APPLET_EVENT_QUEUE_SIZ)
        )
        {
			ASSERT( nTraverseIndex  < MAX_APPLET_EVENT_QUEUE_SIZ );
            if (Applet_Event_Queue[nTraverseIndex].EventID == EventID)
            {
                nPutCount++;
            }
            else
            {
				ASSERT( nPutIndex  < MAX_APPLET_EVENT_QUEUE_SIZ );
                Applet_Event_Queue[nPutIndex] = Applet_Event_Queue[nTraverseIndex];
                nPutIndex = MOD_DECREMENT(nPutIndex, MAX_APPLET_EVENT_QUEUE_SIZ);
            }
        }
        uiQueueTail = (uiQueueTail + nPutCount) % MAX_APPLET_EVENT_QUEUE_SIZ;
		ASSERT( uiQueueTail  < MAX_APPLET_EVENT_QUEUE_SIZ );
    }
}

LOCAL EQS_RESULT        GetEventQueue(EQS_EVENT* pOutEvent)
{
    if (MOD_INCREMENT(uiQueueTail, MAX_APPLET_EVENT_QUEUE_SIZ) == uiQueueHead)
        return RESULT_EVENT_QUEUE_EMPTY;
    else
    {
		WORD Flag;		
        // increment, and then dequeue.
        uiQueueTail = MOD_INCREMENT(uiQueueTail, MAX_APPLET_EVENT_QUEUE_SIZ);		

		ASSERT( uiQueueTail  < MAX_APPLET_EVENT_QUEUE_SIZ );

        *pOutEvent = Applet_Event_Queue[uiQueueTail];
        
        return RESULT_OK;
    }
}

LOCAL EQS_RESULT        AddEventQueue(EQS_EVENT* pInEvent)
{
	WORD Flag;
#ifdef FEED_DRAW_EVENT_SELECTIVELY
    UINT8 nIndex;
    EQS_EVENT OldDrawEvent;
#endif
   
    if (uiQueueHead == uiQueueTail) // Queue full
    {
        return RESULT_EVENT_QUEUE_FULL;
    }
    else
    {
        // Applet manipulation events are 'NON-REPETITIVE'. So, if same event is in event queue,
        // don't put onto queue anymore.
        if (GetCategory(pInEvent->EventID) == EQC_APP)
        {
///////////////////////////////////////////////////////////////////////////////
#ifdef FEED_DRAW_EVENT_SELECTIVELY ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
            /////////////////////////////
            // process EQE_APP_DRAW    //
            /////////////////////////////
            if (pInEvent->EventID == EQE_APP_DRAW)
            {
                nIndex = sPeekEventQueue(EQE_APP_DRAW);
                if (nIndex < MAX_APPLET_EVENT_QUEUE_SIZ) // EQE_APP_DRAW event already in the queue
                {
                    OldDrawEvent = Applet_Event_Queue[nIndex];
					
					if (OldDrawEvent.wParam == EQS_DRAW_LEVEL_ALL)
					{
						sSqueezeEventOnEventQueue(nIndex);
						pInEvent->wParam = EQS_DRAW_LEVEL_ALL;					
					}			
					else if (OldDrawEvent.wParam == pInEvent->wParam)
					{
						sSqueezeEventOnEventQueue(nIndex);				
					}
					else if (pInEvent->wParam == EQS_DRAW_LEVEL_ALL)
					{
						sPurgeEventOnEventQueue(EQE_APP_DRAW);
					}
					else ;					
                }
                // add, and then increment.


				ASSERT( uiQueueHead  < MAX_APPLET_EVENT_QUEUE_SIZ );
                Applet_Event_Queue[uiQueueHead] = *pInEvent;
				
                uiQueueHead = MOD_INCREMENT(uiQueueHead, MAX_APPLET_EVENT_QUEUE_SIZ);			

                return RESULT_OK;

            }
            else 
///////////////////////////////////////////////////////////////////////////////
#endif ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
            if (sPeekEventQueue(pInEvent->EventID) < MAX_APPLET_EVENT_QUEUE_SIZ) // found. 
            {
                // Don't put the same event anymore.
                return RESULT_OK;
            }
            else
            {
                // add, and then increment.
				ASSERT( uiQueueHead  < MAX_APPLET_EVENT_QUEUE_SIZ );
                Applet_Event_Queue[uiQueueHead] = *pInEvent;
				
                uiQueueHead = MOD_INCREMENT(uiQueueHead, MAX_APPLET_EVENT_QUEUE_SIZ);	

				
                return RESULT_OK;
                //..........................
            }
        }
        else
        {
            // add, and then increment.
			ASSERT( uiQueueHead  < MAX_APPLET_EVENT_QUEUE_SIZ );			
            Applet_Event_Queue[uiQueueHead] = *pInEvent;

            uiQueueHead = MOD_INCREMENT(uiQueueHead, MAX_APPLET_EVENT_QUEUE_SIZ);	
			          
            return RESULT_OK;
            //..........................
        }
    }
}



LOCAL EQS_RESULT        sMakeEvent(EQS_EVENT* pAnEvent, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam)
{
#ifdef EXTENDED_EVENT_REGIME
    if( IS_VALID_EVENT_ID( EventID ) == TRUE )    //redstar   00.12.07
    {
        pAnEvent->EventID = EventID;
        pAnEvent->dwExtEventID =  0;
    }
    else
    {
        pAnEvent->dwExtEventID = EventID;
        pAnEvent->EventID      = EQE_BYPASS;
    }
#else
    pAnEvent->EventID = EventID;
#endif
    pAnEvent->wParam = wParam;
    pAnEvent->dwParam = dwParam;

    return RESULT_OK;
}

LOCAL EQS_RESULT        InitAppletState(APPLET_STATE* pAppState, EQS_APPLET *pAnApplet)
{
    UINT8 i;

    pAppState->pApplet = pAnApplet;
    pAppState->fSubRoot = FALSE;
    pAppState->TimerHandle = 1;
    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
        pAppState->AttachedIOControls[i] = INVALID_CONTROL_HANDLE;

    //pAppState->dyCanvasSize = DEFAULT_CANVAS_SIZE;
    pAppState->dyCanvasSize = EQS_GetMaxHeight();
    pAppState->nCML = 0;
    pAppState->wCmlAttr = WA_NONE;// WA_DEFAULT;
	pAppState->nFocusedIOC = 0;
	pAppState->CanvasOffset = 0;
    pAppState->pszSWindowTitle = NULL;

    return RESULT_OK;
}

LOCAL EQS_RESULT        InitAppletStateDispUntouched(APPLET_STATE* pAppState, EQS_APPLET *pAnApplet)
{

    pAppState->pApplet = pAnApplet;
    pAppState->fSubRoot = FALSE;

    return RESULT_OK;
}

//LOCAL EQS_RESULT        InitDisplay(SCREEN_INFO* pScreenInfo)
//{
//    EQS_RESULT Result = RESULT_OK;
//
//    /* init equs graphics */
//    if((Result = OpenGraph(pScreenInfo)) == RESULT_OK)
//    {
//        SetFontType(INIT_FONT_TYPE);    /* system font */
//        EQS_SetTheme(INIT_COLOR_SCHEME, INIT_FONT_TYPE, INIT_SCROLLBAR_SIZE);
//        EQS_InitDC();
//    }
//
//    return Result;
//}

#if 0
LOCAL EQS_RESULT        ClearDisplay(VOID)
{
    EQS_ClearScreen();

    return RESULT_OK;
}
#endif

LOCAL EQS_RESULT        PushAppletState(APPLET_STATE* pInState)
{
    ASSERT(uiAppletStackIndex < MAX_APPLET_STATE_STACK_SIZ);

    //if( uiAppletStackIndex >= MAX_APPLET_STATE_STACK_SIZ )
    //{
    //    TRACE0(ERR_TKN_APPLET_STACK_OVERFLOW);
    //    ASSERT(0);
    //    return RESULT_UNEXPECTED;
    //}
    //else
    //{
    Applet_State_Stack[uiAppletStackIndex] = *pInState;
    uiAppletStackIndex++;
    return RESULT_OK;
    //}
}

LOCAL EQS_RESULT        PopAppletState(APPLET_STATE* pOutState)
{
    UINT8 i;
    BOOL  fPopTimer;    //redstar

    if( pOutState == &CurrentAppletState )  //redstar  for pop local timer info
        fPopTimer = TRUE;
    else 
        fPopTimer = FALSE;

    if(uiAppletStackIndex == 0)
    {
        // return pEQA_Root, for safety code.
        pOutState->pApplet = pEQA_Root;
        pOutState->fSubRoot = TRUE;    
        for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
            pOutState->AttachedIOControls[i] = INVALID_CONTROL_HANDLE;

        return RESULT_UNEXPECTED;
    }

    uiAppletStackIndex--;
    *pOutState = Applet_State_Stack[uiAppletStackIndex]; 
    if( fPopTimer == TRUE )   //redstar
    {
        EQS_FreeAppletTimer();  //redstar  -- remove  local timer info
        EQS_PopAppletTimer();
    }

    return RESULT_OK;
}

LOCAL BOOL sDeserveEventByIOC(HCONTROL hControl, EQS_EVENT_ID EventID)
{
	CONTROL_TYPE_E  ControlType;

	if( hControl >= INVALID_CONTROL_HANDLE )
		return FALSE;

	if( EQC_GET_ENABLE(hControl) == FALSE)
		return FALSE;

    if( EventID == EQE_APP_IO_KILL )
        return TRUE;    // Control이 자신을 End하기 위해 보내는 Event

    ControlType = EQC_GET_TYPE(hControl);


    switch(ControlType)
    {
    case CT_IMAGE :
    case CT_PUSH_BUTTON:
        return FALSE;
    case CT_ANIMATION :
        return FALSE;
    case CT_ACTIVE_ICON :
        if (EventID == EQE_KEY_CENTER)//OK)
            return TRUE;
        else
            return FALSE;

    case CT_STATIC_TEXT :
        return FALSE;
    case CT_SCROLL_LIST :
    case CT_MENU_LIST:
        switch(EventID)
        {
        case EQE_KEY_UP:
        case EQE_KEY_DOWN:
        case EQE_KEY_LEFT:
        case EQE_KEY_RIGHT:
        case EQE_KEY_CENTER:
        case EQE_KEY_CLEAR:

        case EQE_KEY_1:
        case EQE_KEY_2:
        case EQE_KEY_3:
        case EQE_KEY_4:
        case EQE_KEY_5:
        case EQE_KEY_6:
        case EQE_KEY_7:
        case EQE_KEY_8:
        case EQE_KEY_9:
        case EQE_KEY_0:
        case EQE_KEY_STAR:
        case EQE_KEY_POUND:

            return TRUE;

        default:
            return FALSE;
        }

    case CT_TEXT_EDIT : 
        switch (EventID)
        {
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
		case EQE_KEY_UP:
		case EQE_KEY_DOWN:
        case EQE_KEY_F1:
        case EQE_KEY_F2:
        case EQE_KEY_F2_LONG:
        case EQE_KEY_POUND:
        case EQE_KEY_STAR:
        case EQE_KEY_CENTER:
        case EQE_KEY_0_LONG:
        case EQE_KEY_1_LONG:
        case EQE_KEY_2_LONG:
        case EQE_KEY_3_LONG:
        case EQE_KEY_4_LONG:
        case EQE_KEY_5_LONG:
        case EQE_KEY_6_LONG:
        case EQE_KEY_7_LONG:
        case EQE_KEY_8_LONG:
        case EQE_KEY_9_LONG:
        case EQE_KEY_RIGHT_LONG:
        case EQE_KEY_LEFT_LONG:
        case EQE_KEY_CLEAR_LONG:
        case EQE_KEY_STAR_LONG:
        case EQE_KEY_MANNERS:
        case EQE_KEY_POUND_LONG:
            return TRUE;
        default:
            return FALSE;
        }
    case CT_NUMBER_EDIT :
        switch (EventID)
        {
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
        case EQE_KEY_POUND:
        case EQE_KEY_STAR:
        case EQE_KEY_CLEAR:
        case EQE_KEY_LEFT:
		case EQE_KEY_RIGHT:
        case EQE_KEY_CENTER:
        case EQE_KEY_DIGIT_LONG:
        case EQE_KEY_CLEAR_LONG:
        case EQE_KEY_RIGHT_LONG:
        case EQE_KEY_LEFT_LONG:
        case EQE_KEY_F1:
		case EQE_KEY_UP:
		case EQE_KEY_DOWN:
            return TRUE;

        default:
            return FALSE;
        }           
    case CT_RADIO_BUTTON :
        switch( EventID)
        {
        case EQE_KEY_DOWN:
        case EQE_KEY_UP:
        case EQE_KEY_CENTER:
        case EQE_KEY_CLEAR:
            return TRUE;
        default:
            return FALSE;
        }
    case CT_CHECK_BOX :
        switch( EventID )
        {
        case EQE_KEY_UP:
        case EQE_KEY_DOWN:
        case EQE_KEY_CENTER:
        case EQE_KEY_CLEAR:
        //case EQE_KEY_F1:
        //case EQE_KEY_F2:
            return TRUE;
        default:
            return FALSE;
        }    
    case CT_MESSAGE_BOX :
        switch (EventID)
        {
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
        case EQE_KEY_POUND:
        case EQE_KEY_STAR:
        case EQE_KEY_CENTER:
        case EQE_KEY_F1:
        case EQE_KEY_F2:
        case EQE_KEY_UP:
        case EQE_KEY_END:
        case EQE_KEY_DOWN:
        case EQE_KEY_LEFT:
        case EQE_KEY_RIGHT:
        case EQE_KEY_CLEAR:
        case EQE_KEY_DIGIT_LONG:
        case EQE_KEY_0_LONG:
        case EQE_KEY_1_LONG:
        case EQE_KEY_2_LONG:
        case EQE_KEY_3_LONG:
        case EQE_KEY_4_LONG:
        case EQE_KEY_5_LONG:
        case EQE_KEY_6_LONG:
        case EQE_KEY_7_LONG:         
        case EQE_KEY_8_LONG:
        case EQE_KEY_9_LONG:    
        case EQE_KEY_STAR_LONG:
        case EQE_KEY_POUND_LONG:
        case EQE_KEY_CLEAR_LONG: 
        case EQE_KEY_END_LONG:
        case EQE_KEY_CENTER_LONG:
        case EQE_KEY_LEFT_LONG:
        case EQE_KEY_RIGHT_LONG:
        case EQE_KEY_UP_LONG:
        case EQE_KEY_DOWN_LONG:
        case EQE_KEY_MANNERS:
        case EQE_KEY_SEND:
        case EQE_KEY_EAR_LONG_CLICK:
        case EQE_KEY_F1_LONG:
        case EQE_KEY_F2_LONG:
        case EQE_KEY_AUTO_ANSWER:
        case EQE_KEY_PLAY:
        case EQE_KEY_RECORD:
        case EQE_KEY_AUTO_ANSWER_LONG:
        case EQE_KEY_PLAY_LONG:
        case EQE_KEY_RECORD_LONG:
            return TRUE;
        default:
            return FALSE;
        }

    case CT_POPUP_MENU  :
        switch (EventID)
        {
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
        case EQE_KEY_POUND:
        case EQE_KEY_STAR:
        case EQE_KEY_CENTER://OK:
        case EQE_KEY_F1: // Cancel
        case EQE_KEY_F2: 
        case EQE_KEY_CLEAR: // Cancel
        case EQE_KEY_UP:
        case EQE_KEY_DOWN:
        case EQE_KEY_DIGIT_LONG:
        case EQE_KEY_0_LONG:
        case EQE_KEY_1_LONG:
        case EQE_KEY_2_LONG:
        case EQE_KEY_3_LONG:
        case EQE_KEY_4_LONG:
        case EQE_KEY_5_LONG:
        case EQE_KEY_6_LONG:
        case EQE_KEY_7_LONG:         
        case EQE_KEY_8_LONG:
        case EQE_KEY_9_LONG:    
        case EQE_KEY_STAR_LONG:
        case EQE_KEY_POUND_LONG:
        case EQE_KEY_CLEAR_LONG: 
        case EQE_KEY_CENTER_LONG:
        case EQE_KEY_LEFT_LONG:
        case EQE_KEY_RIGHT_LONG:
        case EQE_KEY_UP_LONG:
        case EQE_KEY_DOWN_LONG:
        case EQE_KEY_MANNERS:
        case EQE_KEY_F1_LONG:
        case EQE_KEY_F2_LONG:
        case EQE_KEY_SEND:
        case EQE_KEY_EAR_LONG_CLICK:
        case EQE_KEY_AUTO_ANSWER:
        case EQE_KEY_PLAY:
        case EQE_KEY_RECORD:
        case EQE_KEY_AUTO_ANSWER_LONG:
        case EQE_KEY_PLAY_LONG:
        case EQE_KEY_RECORD_LONG:
//        case EQE_KEY_END:
            return TRUE;
        default:
            return FALSE;
        }
     case CT_PROGRESS_BAR :
         switch(EventID)
         {
         case EQE_KEY_CENTER:
         case EQE_KEY_LEFT:
         case EQE_KEY_RIGHT:
         case EQE_KEY_DOWN:
         case EQE_KEY_UP:
         case EQE_KEY_CLEAR:
             return TRUE;
         default:
             return FALSE;
         }
         
     case CT_SPIN :
         switch(EventID)
         {
         case EQE_KEY_CENTER:
         case EQE_KEY_UP:
         case EQE_KEY_DOWN:
         case EQE_KEY_CLEAR:
             return TRUE;
         default:
             return FALSE;
         }
     case CT_TIME:
         switch(EventID)
         {
         case EQE_KEY_CENTER:
         case EQE_KEY_UP:
         case EQE_KEY_DOWN:
         case EQE_KEY_LEFT:
         case EQE_KEY_RIGHT:
         case EQE_KEY_CLEAR:
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
         case EQE_KEY_POUND:
         case EQE_KEY_STAR:
		 case EQE_KEY_F1:
             return TRUE;
         default:
             return FALSE;
         }
    }    
    return FALSE;
}



LOCAL BOOL DeserveEventByIOC(EQS_EVENT_ID EventID)
{

    
    HCONTROL    hControl;

    if ((hControl = GET_MODAL()) == INVALID_CONTROL_HANDLE)
    {
        if ((hControl = GetFocusedIOC()) == INVALID_CONTROL_HANDLE)
        {
            return FALSE;
        }
    }

	return sDeserveEventByIOC( hControl, EventID );
}


LOCAL BOOL IsAnIOCInWindowExclusive(HCONTROL hControl)
{
    COORD y, dy;
    //RECT ViewRect;

    if (hControl == INVALID_CONTROL_HANDLE)
        return FALSE;

    y = EQC_GET_Y(hControl);
    dy = EQC_GET_DY(hControl);
    //sGetSWindowViewAreaRect(&ViewRect);

    if ((y >= sSlideWindow.yWindowOffset) && 
        ((y + dy - 1) <= (sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize - 1)))
        return TRUE;
    else
        return FALSE;
}

LOCAL BOOL IsAnIOCInWindowInclusive(HCONTROL hControl)
{
    COORD y, dy;
    //RECT ViewRect;
    COORD yUpperBound, yLowerBound;

    if (hControl == INVALID_CONTROL_HANDLE)
        return FALSE;

    y = EQC_GET_Y(hControl);
    dy = EQC_GET_DY(hControl);
    //sGetSWindowViewAreaRect(&ViewRect);

    yUpperBound = sSlideWindow.yWindowOffset;
    yLowerBound = sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize - 1;

/*
    if (((y + dy) > (sSlideWindow.yWindowOffset)) ||
        (y < (sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize - 1)))
        return TRUE;
    else
        return FALSE;
*/
	
    if (((y + dy) < yUpperBound) || (y > yLowerBound))
        return FALSE;
    else
        return TRUE;

}

// redstar 2001.1.2  -- for  timer execution
LOCAL EQS_RESULT FeedTimerEvent(void)
{
   HTIMER       hTimer;
   HCONTROL     hControl;
   EQS_EVENT    Event;
   EQS_EVENT_ID EventID;
   RECT         Rect;

   hTimer = EQS_GetFirstDeserveTimer();
   while( hTimer != INVALID_TIMER_HANDLE )
   {
       hControl = GetTimerControlHandle( hTimer );
       EventID  = (EQS_EVENT_ID)(EQE_TM_TIMER1 + (GetTimerID(hTimer) - EQS_TIMER_1));
	   
       if( hControl == INVALID_CONTROL_HANDLE ) // feed timer event to applet
       {
           sMakeEvent(&Event, EventID, (WORD)hTimer, (DWORD) CurrentAppletState.pApplet);
           l_pfnDispatcher(*CurrentAppletState.pApplet, &Event);
       }
       else if( sValidAppletControl(hControl) == TRUE )  // feed timer event to control
       {
            sMakeEvent(&Event, EventID, (WORD)hTimer, (DWORD) hControl); 
            EQC_HandleEvent(hControl, &Event);//NIOC_ExecIOControl(hControl, pAnEvent);   

			if( l_fRefreshControlDisplay  && l_fEnableRefresh )
			{
#if USE_PARTIAL_DISPLAY_UPDATE				
				if( EQC_GET_TYPE(hControl) == CT_TEXT_EDIT )//|| GET_MODAL() != INVALID_CONTROL_HANDLE )
				{
					EQS_UpdateDisplay();
				}
				else if( hControl == GET_MODAL() )
				{
					l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), EQC_GET_X(hControl), EQC_GET_Y(hControl),
												                   EQC_GET_X(hControl)+EQC_GET_DX(hControl),
												                   EQC_GET_Y(hControl)+EQC_GET_DY(hControl));
				}
				else
				{
                    /* jrkwon 2001.09.24 --> */
                    COORD xWidth = EQC_GET_DX(hControl);

                    if(EQC_GET_TYPE(hControl) == CT_STATIC_TEXT)
                    {
                        COORD xPrevWidth = EQC_GetStaticTextPreviousLength(hControl);
                        
                        xWidth = xPrevWidth > xWidth ? xPrevWidth : xWidth;
                    }
					sGetSWindowViewAreaRect(&Rect);
					l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), EQC_GET_X(hControl), Rect.yTop+EQC_GET_Y(hControl)-sSlideWindow.yWindowOffset,
												  EQC_GET_X(hControl)+xWidth,
												  Rect.yTop+EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-sSlideWindow.yWindowOffset);
					//l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), EQC_GET_X(hControl), Rect.yTop+EQC_GET_Y(hControl)-sSlideWindow.yWindowOffset,
					//							  EQC_GET_X(hControl)+EQC_GET_DX(hControl),
					//							  Rect.yTop+EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-sSlideWindow.yWindowOffset);
                    /* <-- */
				}
#else
				EQS_UpdateDisplay();
#endif
				l_fRefreshControlDisplay = FALSE;

			}
			else if( l_fRefreshControlDisplay )
			{
				l_fRefreshControlDisplay = FALSE ;
			}
			else ;
			
       }
	   else 
		   ;
       hTimer = EQS_GetNextDeserveTimer(hTimer);
   }
   return RESULT_OK;
}


LOCAL EQS_RESULT FeedEventIOControl(EQS_EVENT* pAnEvent)
{ 

    HCONTROL hControl;

    if ((hControl = GET_MODAL()) != INVALID_CONTROL_HANDLE)
    {
        EQC_HandleEvent(hControl, pAnEvent);
        return RESULT_OK;
    }

    //EQC_HandleEvent(hControl, pAnEvent);//NIOC_ExecIOControl(hControl, pAnEvent);
    //return RESULT_OK;
#if 0
    if ((hControl = GetFocusedIOC()) != INVALID_CONTROL_HANDLE)
#endif
	if( (hControl = CurrentAppletState.AttachedIOControls[sSlideWindow.CurrentIOCIndex]) != INVALID_CONTROL_HANDLE)
    {

        EQC_HandleEvent(hControl, pAnEvent);//NIOC_ExecIOControl(hControl, pAnEvent);
        return RESULT_OK;
    }
    else
    {
        return RESULT_INVALID_HANDLE;
    }
}

/*
LOCAL EQS_RESULT FeedEventAllIOControl(EQS_EVENT* pAnEvent)
{
    // Feed an event to all I/O Controls which are within the Sliding Window.

    HCONTROL hControl;
    UINT8 i;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
        {
            if (i != sSlideWindow.CurrentIOCIndex)
            {
                if (IsAnIOCInWindowInclusive(hControl))
                    EQC_HandleEvent(hControl, pAnEvent);//NIOC_ExecIOControl(hControl, pAnEvent);
            }
        }
        else
        {
            break;
        }
    }

    if ((hControl = GetFocusedIOC()) != INVALID_CONTROL_HANDLE)
        EQC_HandleEvent(hControl, pAnEvent);//NIOC_ExecIOControl(hControl, pAnEvent);

    return RESULT_OK;
}


LOCAL EQS_RESULT FeedEventAllIOCandSWin(EQS_EVENT* pAnEvent)
{
    // Feed an event to all I/O Controls which are within the Sliding Window.

    HCONTROL hControl;
    UINT8 i;

    // Feed the input event to the IO Controls not having focus.
    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
        {
            //if (i != sSlideWindow.CurrentIOCIndex)
            if (hControl != GetFocusedIOC())
            {
                if (IsAnIOCInWindowInclusive(hControl))
                    EQC_HandleEvent(hControl, pAnEvent);//NIOC_ExecIOControl(hControl, pAnEvent);
            }
        }
        else
        {
            break;
        }
    }

    // Feed EQE_APP_DRAW to SWindow.
    EQS_ProcessSWindowEvent(pAnEvent);

    // Feed EQE_APP_DRAW to the IO Control which has focus.
    if ((hControl = GetFocusedIOC()) != INVALID_CONTROL_HANDLE)
        EQC_HandleEvent(hControl, pAnEvent);

    return RESULT_OK;
}

*/
#ifdef FEED_DRAW_EVENT_SELECTIVELY
LOCAL EQS_RESULT sFeed_EQE_APP_DRAW_OnAnControl(void)
{

    HCONTROL hDrawControl, hControl;
    VIEW_PORT OldViewPort;
    VIEW_PORT ViewPort;
    RECT Rect;
    BOOL fOldSWindowState;
    WORD wSWindowAttr;
	COLOR  BackColor, ForeColor, TextBackColor, TextColor;



    wSWindowAttr = EQS_GetWindowAttr();

    fOldSWindowState = GetSlidingWindowState();
    GetViewPort(&OldViewPort);


    hDrawControl = (HCONTROL)CurrentEvent.wParam;

	if( EQC_GET_TYPE(hDrawControl) == CT_NONE )
		return RESULT_ERROR;
					
	
    if ((hControl = GET_MODAL()) == hDrawControl) // Feed EQE_APP_DRAW to MODAL I/O Control
    {
        SetSlidingWindowState(FALSE);
        ResetViewPort();
        EQC_HandleEvent(hDrawControl, &CurrentEvent);
    }
    else if ((!IsActive(hDrawControl)) && IsAnIOCInWindowExclusive(hDrawControl))
    {
        SetSlidingWindowState(TRUE);
        sGetSWindowViewAreaRect(&Rect);
        ViewPort.Rect = Rect;
        ViewPort.CoordSystem = CS_SCREEN;
        ViewPort.Origin.x = 0;
        ViewPort.Origin.y = 0;
        SetViewPort(&ViewPort);

		if(EQS_GetFocusedIOC() == hDrawControl)
		{								
			EQS_SetFocusDraw( TRUE );			
		}
		if( EQC_GET_FOCUS(hDrawControl) == FALSE || EQC_GET_ENABLE(hDrawControl) == FALSE ) //redstar 2001.10.10  Add "EQC_GET_ENABLE(hDrawControl) == FALSE"
		{
			EQS_SetInActiveDraw(TRUE);
			//TextBackColor = EQS_GetTextBackColor();
			//EQS_SetTextBackColor(EQS_GetBackColor());
		}
        EQC_HandleEvent(hDrawControl, &CurrentEvent);

		EQS_SetInActiveDraw(FALSE);
		EQS_SetFocusDraw( FALSE );
/* 01.04.16
        /////////////////////
        // Draw focus mark //
        /////////////////////
        if (GetWindowMode() == WM_APPLET_MODE)
            if (hDrawControl == GetFocusedIOC())
                sDrawFocusMark();
*/
    }
    else if (hDrawControl == GetActiveIOC()) // Feed EQE_APP_DRAW to "Active Modeless" I/O Control
    {	       

        if (GET_MODAL() == INVALID_CONTROL_HANDLE) // There is no MODAL Control.
        {									
			EQS_SetActiveDraw(TRUE);			
            switch (EQC_GET_TYPE(hDrawControl))
            {
            case CT_TEXT_EDIT:
                SetSlidingWindowState(FALSE);
                sGetSWindowViewAreaRect(&Rect);

                if ((wSWindowAttr & WA_SHOW_CML) == WA_SHOW_CML)
                {
                    Rect.yBottom += EQS_GetCMLHeight();
                }

                ViewPort.Rect = Rect;
                ViewPort.CoordSystem = CS_SCREEN;
                ViewPort.Origin.x = 0;
                ViewPort.Origin.y = 0;
                SetViewPort(&ViewPort);
                break;
            default:


				if( EQC_GET_TYPE(hDrawControl) == CT_MENU_LIST )
				{
					EQS_SetActiveDraw(FALSE);
				}
				else if( EQC_GET_TYPE(hDrawControl) == CT_SCROLL_LIST &&\
					     EQC_GET_STYLE(hDrawControl) & CS_SCROLL_LIST_MAXIMIZE )
				{
					EQS_SetActiveDraw(FALSE);
				}
				else 
					;

                SetSlidingWindowState(TRUE);
                sGetSWindowViewAreaRect(&Rect);				
                ViewPort.Rect = Rect;
                ViewPort.CoordSystem = CS_SCREEN;
                ViewPort.Origin.x = 0;
                ViewPort.Origin.y = 0;
                SetViewPort(&ViewPort);
                break;
            }
			
            EQC_HandleEvent(hDrawControl, &CurrentEvent);
			EQS_SetActiveDraw(FALSE);
        }		
    }
	else
	{
		SetViewPort(&OldViewPort);
		SetSlidingWindowState(fOldSWindowState);
		return RESULT_OK;
	}

   
    SetViewPort(&OldViewPort);
    SetSlidingWindowState(fOldSWindowState);

	
	if( l_fEnableRefresh )
	{
#if USE_PARTIAL_DISPLAY_UPDATE
		if( l_fRefreshControlDisplay  )
		{
			if( EQC_GET_TYPE(hDrawControl) == CT_TEXT_EDIT ) //|| GET_MODAL() != INVALID_CONTROL_HANDLE )
			{
				EQS_UpdateDisplay();
			}
			else if( hDrawControl == GET_MODAL() )
			{
				l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), EQC_GET_X(hDrawControl), EQC_GET_Y(hDrawControl),
															   EQC_GET_X(hDrawControl)+EQC_GET_DX(hDrawControl),
															   EQC_GET_Y(hDrawControl)+EQC_GET_DY(hDrawControl));
			}
			else
			{
                /* jrkwon 2001.09.24 --> */
                COORD xWidth = EQC_GET_DX(hDrawControl);

                if(EQC_GET_TYPE(hDrawControl) == CT_STATIC_TEXT)
                {
                    COORD xPrevWidth = EQC_GetStaticTextPreviousLength(hDrawControl);
                    
                    xWidth = xPrevWidth > xWidth ? xPrevWidth : xWidth;
                }
				sGetSWindowViewAreaRect(&Rect);
				if( EQC_GET_X(hDrawControl)  > 0 &&\
					EQC_GET_Y(hDrawControl)  > 0 &&\
					EQC_GET_DX(hDrawControl) < DevGetMaxX() &&\
					EQC_GET_DY(hDrawControl) < DevGetMaxY() )
				{				
					l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), EQC_GET_X(hDrawControl)-1,\
						                                           Rect.yTop+EQC_GET_Y(hDrawControl)-sSlideWindow.yWindowOffset-1,
																   EQC_GET_X(hDrawControl)+xWidth+1,
																   Rect.yTop+EQC_GET_Y(hDrawControl)+EQC_GET_DY(hDrawControl)-sSlideWindow.yWindowOffset+1);

				}
				else
				{
					l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), EQC_GET_X(hDrawControl),\
						                                           Rect.yTop+EQC_GET_Y(hDrawControl)-sSlideWindow.yWindowOffset,
																   EQC_GET_X(hDrawControl)+xWidth,
																   Rect.yTop+EQC_GET_Y(hDrawControl)+EQC_GET_DY(hDrawControl)-sSlideWindow.yWindowOffset);

				}
                /* <-- */
				/* sGetSWindowViewAreaRect(&Rect);
				l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), EQC_GET_X(hDrawControl), Rect.yTop+EQC_GET_Y(hDrawControl)-sSlideWindow.yWindowOffset,
											  EQC_GET_X(hDrawControl)+EQC_GET_DX(hDrawControl),
											  Rect.yTop+EQC_GET_Y(hDrawControl)+EQC_GET_DY(hDrawControl)-sSlideWindow.yWindowOffset);
                                              */


			}
			l_fRefreshControlDisplay = FALSE;
		}
#else
		EQS_UpdateDisplay();
#endif
	}
	else if( l_fRefreshControlDisplay )
	{
		l_fRefreshControlDisplay = FALSE ;
	}
	else
		;

    return RESULT_OK;
}
#endif


LOCAL EQS_RESULT sFeed_EQE_APP_DRAW(void)
{
// < Reference >
//
//VOID EQS_API GetViewPort(VIEW_PORT* pViewPort)
//VOID EQS_API GetViewPortRect(RECT *pRect)
//VOID EQS_API SetViewPort(VIEW_PORT* pViewPort)
//VOID EQS_API SetViewPortRect(RECT* pRect)
//VOID EQS_API ResetViewPort(VOID)
//COORD EQS_API DevGetMaxWidth(VOID)
//COORD EQS_API DevGetMaxHeight(VOID)
//COORD EQS_API DevGetMaxX(VOID)
//COORD EQS_API DevGetMaxY(VOID)
//
//struct tagVIEW_PORT {
//    RECT            Rect;
//    COORD_SYSTEM    CoordSystem;
//    POINT           Origin;
//};

    HCONTROL hControl;
    UINT8 i;
    VIEW_PORT OldViewPort;
    VIEW_PORT ViewPort;
    RECT Rect;
    BOOL fOldSWindowState;
    WORD wSWindowAttr;
	COLOR BackColor, ForeColor, TextBackColor, TextColor;

    INT  nHeight;

    wSWindowAttr = EQS_GetWindowAttr();

    fOldSWindowState = GetSlidingWindowState();
    GetViewPort(&OldViewPort);

#ifdef FEED_DRAW_EVENT_SELECTIVELY
    if ((CurrentEvent.wParam > 0) && (CurrentEvent.wParam < MAX_CONTROL_NUM))
    {
		if( EQC_GET_SELF_DRAW(CurrentEvent.wParam) == TRUE )
			return sFeed_EQE_APP_DRAW_OnAnControl();
		else
			return RESULT_OK;
    }
    else if (CurrentEvent.wParam == EQS_DRAW_LEVEL_CML)
    {
        // Feed EQE_APP_DRAW to CML.
        SetSlidingWindowState(FALSE);
        ResetViewPort();
    
        EQS_ProcessCMLEvent(&CurrentEvent);

        SetViewPort(&OldViewPort);
        SetSlidingWindowState(fOldSWindowState);

		if( l_fEnableRefresh  )
		{
#if USE_PARTIAL_DISPLAY_UPDATE

			l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(),0,DevGetMaxY() - EQS_GetCMLHeight(),\
			                                        DevGetMaxX(), DevGetMaxY());

#else
			EQS_UpdateDisplay();
#endif
		}

        return RESULT_OK;
    }
    else if (CurrentEvent.wParam == EQS_DRAW_LEVEL_SWINDOW)
    {
        // Feed EQE_APP_DRAW to SWindow Frame
        SetSlidingWindowState(FALSE);
        ResetViewPort();
    
        EQS_ProcessSWindowEvent(&CurrentEvent);

        SetViewPort(&OldViewPort);
        SetSlidingWindowState(fOldSWindowState);

		if( l_fEnableRefresh )
		{
#if USE_PARTIAL_DISPLAY_UPDATE
			l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(),0,0,DevGetMaxX(),EQS_GetSWindowHeaderHeight());

#else
			EQS_UpdateDisplay();
#endif
		}
        return RESULT_OK;
    }
	else if( CurrentEvent.wParam == EQS_DRAW_LEVEL_APPLET_ONLY )
	{

		SetSlidingWindowState(TRUE);
		sGetSWindowViewAreaRect(&Rect);

		ViewPort.Rect = Rect;
		ViewPort.CoordSystem = CS_SCREEN;
		ViewPort.Origin.x = 0;
		ViewPort.Origin.y = 0;
		SetViewPort(&ViewPort);
		// Feed EQE_APP_DRAW current applet.
		FEED_CURRENT_EVENT_TO_CURRENT_APPLET;   //redstar reorder of applet and controls
						
#if USE_PARTIAL_DISPLAY_UPDATE	
		l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom );		
		l_fRefreshControlDisplay = FALSE;
#else
		EQS_UpdateDisplay();
#endif

        SetViewPort(&OldViewPort);
        SetSlidingWindowState(fOldSWindowState);


		return RESULT_OK;
	}
	else ;
#endif

    // Draw All Components.

    // Clear Screen.
    // Clear Screen.
#ifdef DIRECT_APPLET_DRAW
    if (IS_DIRECT_APPLET_DRAW())
	{
		SetSlidingWindowState(FALSE);		
		Rect.xLeft   = 0;
		Rect.yTop    = 0;
        Rect.xRight  = DevGetMaxX();
        Rect.yBottom = DevGetMaxY();
	}
	else
#endif
	{
        if( CurrentEvent.wParam == EQS_DRAW_LEVEL_ALL  )
            ClearScreen(); // grph function.
	
		SetSlidingWindowState(TRUE);
		sGetSWindowViewAreaRect(&Rect);
	}

    //SetSlidingWindowState(TRUE);    
    ViewPort.Rect = Rect;
    ViewPort.CoordSystem = CS_SCREEN;
    ViewPort.Origin.x = 0;
    ViewPort.Origin.y = 0;
    SetViewPort(&ViewPort);


    // Feed EQE_APP_DRAW the IO Controls not active.
    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
        {            
            if (!IsActive(hControl))
            {
                if (IsAnIOCInWindowInclusive(hControl))
                {
					if(EQS_GetFocusedIOC() == hControl)
					{
						if( EQC_GET_FOCUS(hControl) != FALSE && GetWindowMode() != WM_MODAL_IO_MODE)
						{								
							EQS_SetFocusDraw( TRUE ); 							
						}
					}										
					if( EQC_GET_ENABLE(hControl) == FALSE || EQC_GET_FOCUS(hControl) == FALSE)
					{
						EQS_SetInActiveDraw(TRUE);
					}
					
                    EQC_HandleEvent(hControl, &CurrentEvent);	
					EQS_SetFocusDraw( FALSE ); 
					EQS_SetInActiveDraw(FALSE);										
                }				
            }
        }
        else
        {
            break;
        }
    }

    // Feed EQE_APP_DRAW current applet.
    FEED_CURRENT_EVENT_TO_CURRENT_APPLET;   //redstar reorder of applet and controls

/* 01.04.16
    /////////////////////
    // Draw focus mark //
    /////////////////////
    if (GetWindowMode() == WM_APPLET_MODE)
        if (INVALID_CONTROL_HANDLE != GetFocusedIOC())
            sDrawFocusMark();
*/


    if( CurrentEvent.wParam == EQS_DRAW_LEVEL_ALL )  // draw command line
    {
        // Feed EQE_APP_DRAW to CML.    
        SetSlidingWindowState(FALSE);
        ResetViewPort();
        EQS_ProcessCMLEvent(&CurrentEvent);
    }
		
	//Draw Scroll Bar  
	sGetSWindowViewAreaRect(&Rect);
	nHeight = Rect.yBottom - Rect.yTop+1;
	//nIndicator = EQS_GetMaxHeight() - nHeight;
	if( EQS_GetIOCNum() > 1 && CurrentAppletState.dyCanvasSize > nHeight )
	{
		//HCONTROL         hControl;
		SCROLL_BAR       ScrollBar;
		SCROLL_BAR_INFO  SBarInfo;
		UINT8 nWidth;

		nWidth = R_SCROLLBAR_SIZE;
		EQS_AddScrollBar(NULL, &ScrollBar,SBT_VERT,
						 Rect.xRight - nWidth,\
						 Rect.yTop,\
						 nHeight,0,\
						 CurrentAppletState.dyCanvasSize-nHeight-1,\
						 nHeight,0);			
		SBarInfo.nMax = CurrentAppletState.dyCanvasSize-nHeight-1;
		SBarInfo.nMin = 0;
		SBarInfo.nPageSize = nHeight;
		SBarInfo.nPos = sSlideWindow.yWindowOffset;
		EQS_SetScrollBarInfo(&ScrollBar, &SBarInfo, TRUE);  
	}


    // Feed EQE_APP_DRAW to "Active Modeless" I/O Control
    if ((hControl = GetActiveIOC()) != INVALID_CONTROL_HANDLE)
    {       
        if (GET_MODAL() != hControl)
        {								
			EQS_SetActiveDraw( TRUE );							
            switch (EQC_GET_TYPE(hControl))
            {
            case CT_TEXT_EDIT:
                SetSlidingWindowState(FALSE);
                sGetSWindowViewAreaRect(&Rect);

                if ((wSWindowAttr & WA_SHOW_CML) == WA_SHOW_CML)
                {
                    Rect.yBottom += EQS_GetCMLHeight();
                }

                ViewPort.Rect = Rect;
                ViewPort.CoordSystem = CS_SCREEN;
                ViewPort.Origin.x = 0;
                ViewPort.Origin.y = 0;
                SetViewPort(&ViewPort);
                break;				
            default:
				if(  EQC_GET_TYPE(hControl) == CT_MENU_LIST )
				{
					EQS_SetActiveDraw(FALSE);
				}
				else if( EQC_GET_TYPE(hControl) == CT_SCROLL_LIST &&\
					     EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
				{
					EQS_SetActiveDraw(FALSE);
				}
				else ;

                SetSlidingWindowState(TRUE);
                sGetSWindowViewAreaRect(&Rect);
                ViewPort.Rect = Rect;
                ViewPort.CoordSystem = CS_SCREEN;
                ViewPort.Origin.x = 0;
                ViewPort.Origin.y = 0;
                SetViewPort(&ViewPort);
                break;
            }
            EQC_HandleEvent(hControl, &CurrentEvent);
			
			EQS_SetActiveDraw( FALSE );
        }	
    }
    
    if( CurrentEvent.wParam == EQS_DRAW_LEVEL_ALL )   // draw annunciator
    {
        // Feed EQE_APP_DRAW to SWindow Frame
        SetSlidingWindowState(FALSE);
        ResetViewPort();    
        EQS_ProcessSWindowEvent(&CurrentEvent);
    }


    // Feed EQE_APP_DRAW to MODAL I/O Control
    if ((hControl = GET_MODAL()) != INVALID_CONTROL_HANDLE)
    {
        EQC_HandleEvent(hControl, &CurrentEvent);
    }
#if 0
	else
	{
		//redstar 2001.02.24
		//Draw Scroll Bar  

		sGetSWindowViewAreaRect(&Rect);
		nHeight = Rect.yBottom - Rect.yTop+1;
		//nIndicator = EQS_GetMaxHeight() - nHeight;
		if( EQS_GetIOCNum() > 1 && CurrentAppletState.dyCanvasSize > nHeight )
		{
			//HCONTROL         hControl;
			SCROLL_BAR       ScrollBar;
			SCROLL_BAR_INFO  SBarInfo;
			UINT8 nWidth;

			nWidth = R_SCROLLBAR_SIZE;
			EQS_AddScrollBar(NULL, &ScrollBar,SBT_VERT,
							 Rect.xRight - nWidth,\
							 Rect.yTop,\
							 nHeight,0,\
							 CurrentAppletState.dyCanvasSize-nHeight-1,\
							 nHeight,0);			
			SBarInfo.nMax = CurrentAppletState.dyCanvasSize-nHeight-1;
			SBarInfo.nMin = 0;
			SBarInfo.nPageSize = nHeight;
			SBarInfo.nPos = sSlideWindow.yWindowOffset;
			EQS_SetScrollBarInfo(&ScrollBar, &SBarInfo, TRUE);  
		}
	}
#endif  
	
	SetViewPort(&OldViewPort);
	SetSlidingWindowState(fOldSWindowState);


	if( l_fEnableRefresh )
	{
#if USE_PARTIAL_DISPLAY_UPDATE
		if( CurrentEvent.wParam == EQS_DRAW_LEVEL_ALL )   // draw annunciator
		{			
			EQS_UpdateDisplay();
		}
		else
		{
			l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(), Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom );
		}
		l_fRefreshControlDisplay = FALSE;
#else
		EQS_UpdateDisplay();
#endif
	}
	else
	{
		l_fRefreshControlDisplay = FALSE;
	}

    return RESULT_OK;
}

#ifdef DIRECT_APPLET_DRAW

void EQS_SetDirectDraw(BOOL fDirectDraw)
{
    SET_DIRECT_APPLET_DRAW(fDirectDraw);
}

/* for WAP */
/** void EQS_AppletHeaderDrawDirect(void)
{
    HCONTROL hControl;
    VIEW_PORT OldViewPort;
    VIEW_PORT ViewPort;
    RECT Rect;
    BOOL fOldSWindowState;

#ifdef EXTENDED_EVENT_REGIME
    CONST EQS_EVENT DrawEvent = { EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA, EQS_NA};
#else
    CONST EQS_EVENT DrawEvent = { EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA};
#endif

    fOldSWindowState = GetSlidingWindowState();
    GetViewPort(&OldViewPort);

    // Feed EQE_APP_DRAW to CML.
    SetSlidingWindowState(FALSE);
    ResetViewPort();
    
    //EQS_ProcessSWindowEvent((EQS_EVENT *)&DrawEvent);

    // Feed EQE_APP_DRAW to MODAL I/O Control
    if ((hControl = GET_MODAL()) != INVALID_CONTROL_HANDLE)
    {
        EQC_HandleEvent(hControl, (EQS_EVENT *)&DrawEvent);
    }

    SetViewPort(&OldViewPort);
    SetSlidingWindowState(fOldSWindowState);

    l_pfnRefreshDisplayDevice();
}
***/
void EQS_AppletDrawDirect(void)
{
    HCONTROL hControl;
    VIEW_PORT OldViewPort;
    //VIEW_PORT ViewPort;
    //RECT Rect;
    BOOL fOldSWindowState;

#ifdef EXTENDED_EVENT_REGIME
    CONST EQS_EVENT DrawEvent = { EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA, EQS_NA};
// jrkwon 2001.02.09    CONST EQS_EVENT DrawEvent = { EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA, EQS_NA};
#else
    CONST EQS_EVENT DrawEvent = { EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA};
// jrkwon 2001.02.09    CONST EQS_EVENT DrawEvent = { EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA};
#endif

    fOldSWindowState = GetSlidingWindowState();
    GetViewPort(&OldViewPort);

    // Clear Screen.
    //ClearScreen(); // grph function.

     // Feed EQE_APP_DRAW to CML.
    SetSlidingWindowState(FALSE);
    ResetViewPort();
    
    //EQS_ProcessCMLEvent(&CurrentEvent);


    // Feed EQE_APP_DRAW to SWindow Frame
    //SetSlidingWindowState(FALSE);
    //ResetViewPort();
    
    // jrkwon 2001.02.09 do not draw swindow header(in this case, it's annunciators' bar 
    // EQS_ProcessSWindowEvent((EQS_EVENT *)&DrawEvent);


    // Feed EQE_APP_DRAW to MODAL I/O Control
    if ((hControl = GET_MODAL()) != INVALID_CONTROL_HANDLE)
    {
        EQC_HandleEvent(hControl, (EQS_EVENT *)&DrawEvent);
    }

    SetViewPort(&OldViewPort);
    SetSlidingWindowState(fOldSWindowState);

    l_pfnRefreshDisplayDevice();
}
#endif


/*
LOCAL ROM BYTE abIOCPtr[] = {
    #include "arrw1616.ebi"
};
*/
#if 0

#define IOCPTR_WIDTH              16
#define IOCPTR_HEIGHT             15
#define IOCPTR_BPP                2

LOCAL ROM BITMAP l_BitmapInfoIOCPtr = {                  //redstar 00.12.07
    { { IOCPTR_WIDTH, IOCPTR_HEIGHT }, 
      IOCPTR_WIDTH, IOCPTR_BPP, FALSE },
    abIOCPtr
};

#endif /* 00000 */

#if 0 // redstar 2001.06.25
LOCAL void sDrawFocusMark(void)
{

    HCONTROL hControl;
    COORD x, y;
    //DRAW_MODE OldDrawMode;
    COLOR OldColor;

    hControl = GetFocusedIOC(); // At this point, hControl cannot be INVALID_CONTROL_HANDLE.

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

    x = EQC_GET_X(hControl);
    y = EQC_GET_Y(hControl);

    OldColor = EQS_GetForeColor();
    EQS_SetForeColor(RED);
    EQS_Rectangle(x-2, y-2, x+EQC_GET_DX(hControl)+1, y+EQC_GET_DY(hControl)+1);
    EQS_SetForeColor(OldColor);

    //OldDrawMode = EQS_GetDrawMode();
    //EQS_SetDrawMode(DM_OR);
    //EQS_PutBitmap((COORD)(x+2), (COORD)(y+2), &l_BitmapInfoIOCPtr); 
    //EQS_SetDrawMode(OldDrawMode);

}
#endif

LOCAL void sGetSWindowViewAreaRect(RECT* pRect)
{
    WORD   wSWindowAttr;
    COORD  Left, Top, Right, Bottom;

    wSWindowAttr = EQS_GetWindowAttr();

    if ((wSWindowAttr & WA_SHOW_HEADER) == WA_SHOW_HEADER)
    {
        Left = 0;
        Top = EQS_GetSWindowHeaderHeight();
    }
    else
    {
        Left = 0;
        Top = 0;
    }

    if ((wSWindowAttr & WA_SHOW_CML) == WA_SHOW_CML)
    {
        Right = DevGetMaxX();
        Bottom = DevGetMaxY() - EQS_GetCMLHeight();
    }
    else
    {
        Right = DevGetMaxX();
        Bottom = DevGetMaxY();
    }

    SetRect(pRect, Left, Top, Right, Bottom);
}


LOCAL COORD sGetWindowSize(void)
{
    RECT ViewRect;

    sGetSWindowViewAreaRect(&ViewRect);
    return (ViewRect.yBottom - ViewRect.yTop + 1); // y-axis size of the Sliding Window.
}


LOCAL EQS_EVENT_CATEGORY GetCategory(EQS_EVENT_ID EventID)
{
    if( EventID > EQE_APP && EventID < EQE_IO )
        return EQC_APP;
    if( EventID > EQE_IO && EventID < EQE_KEY )
        return EQC_IO;
    if( EventID > EQE_KEY)
        return EQC_KEY;

/*    if( EventID > EQE_KEY && EventID < EQE_SOFTKEY )
        return EQC_KEY;    
        
    if( EventID > EQE_SOFTKEY && EventID < EQE_SMS )
        return EQC_SOFTKEY;
        
    if( EventID > EQE_SMS && EventID < EQE_CM )
        return EQC_SMS;
        
    if( EventID > EQE_CM && EventID < EQE_PH )
        return EQC_CM;
        
    if( EventID > EQE_PH && EventID < EQE_TM )
        return EQC_PH;
        
    if( EventID > EQE_TM && EventID < EQE_IN )
        return EQC_TM;
        
    if( EventID > EQE_IN && EventID < EQE_X )
        return EQC_IN;
    
    if( EventID > EQE_X && EventID < EQE_ERROR )
        return EQC_X;
*/
    else
        return EQC_ERROR;
}

LOCAL EQS_RESULT Prepare2StartNextApplet( EQS_APPLET *pAnApplet, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam, BOOL bDisp)
{
    // save pending event
    sMakeEvent(&NextEvent, EventID, wParam, dwParam);

    // save parameters
    NextAppletState.pApplet = pAnApplet;

    // Display - I/O Control Inheritance.
    bDispInherit = bDisp;

    return RESULT_OK;
}

LOCAL EQS_RESULT Prepare2EndApplet(EQS_POPUP PopToWhere, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam)
{
    sMakeEvent(&NextEvent, EventID, wParam, dwParam);

    PopToPlace = PopToWhere;
    return RESULT_OK;
}
LOCAL BOOL sValidAppletControl(HCONTROL hControl) //01.03.15 redstar
{
    UINT8 i;

	if( hControl == INVALID_CONTROL_HANDLE )
		return FALSE;
	if( hControl == EQS_GetModal() )
		return TRUE;
    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (hControl == CurrentAppletState.AttachedIOControls[i])
            return TRUE;
    }
	return FALSE;
}


LOCAL EQS_RESULT FreeIOCIndex(UINT8 hControl)
{
    UINT8 i;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (hControl == CurrentAppletState.AttachedIOControls[i])
            break;
    }

    ASSERT(i < MAX_CONTROL_PER_APPLET);

    if (i <= sSlideWindow.CurrentIOCIndex)
    {
        if (sSlideWindow.CurrentIOCIndex > 0) // If sSlideWindow.CurrentIOCIndex == 0 , it is also that i == 0 .
        {
            sSlideWindow.CurrentIOCIndex -= 1;
        }
    }

    if (i == MAX_CONTROL_PER_APPLET - 1) // The last one.
    {
        CurrentAppletState.AttachedIOControls[i] = MAX_CONTROL_PER_APPLET;
    }
    else
    {
        for (  ; i < MAX_CONTROL_PER_APPLET - 1; i++)
        {
            CurrentAppletState.AttachedIOControls[i] = CurrentAppletState.AttachedIOControls[i+1];
        }
        CurrentAppletState.AttachedIOControls[i] = INVALID_CONTROL_HANDLE; // i == MAX_CONTROL_PER_APPLET - 1
    }
    
    return RESULT_OK;
}


LOCAL EQS_RESULT sDeleteAllAttachedIOControl(APPLET_STATE* pAnState)
{
    UINT8 i;
    HCONTROL hControl;


	InitEventQueue(); //01.04.09 redstar
#if 0
    if ((hControl = GET_MODAL()) != INVALID_CONTROL_HANDLE)
    {
        if ((EQC_GET_TYPE(hControl) == CT_MESSAGE_BOX) && (EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_SYSTEM))
        {
            // Don't delete System Message Box.
        }
        else
        {
            EQC_DeleteControl(GET_MODAL(), FALSE);
            // Reset modal hControl
		
			//sSlideWindow.tWindowMode = WM_APPLET_MODE;  //deactive
            RESET_MODAL();
        }
    }
#endif
    // find free index
    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (pAnState->AttachedIOControls[i] == INVALID_CONTROL_HANDLE)
        {
            break; // stop looping.
        }
        else
        {
            EQC_DeleteControl(pAnState->AttachedIOControls[i], FALSE);
        }
    }

    // girin. 2000.12.17
    // EQC_DeleteControl(hControl) sends EQE_IO_DELETE event to the I/O 
    // Control of hControl. The I/O Control sends EQE_IO_***_END event usually.
    // InitEventQueue() here keeps Applet-Event-Queue from being full.


    //InitEventQueue(); 01.04.07 redstar

    return RESULT_OK;
}


LOCAL BOOL HaveFocus(HCONTROL hControl)
{
    return (GetFocusedIOC() == hControl);
}


LOCAL BOOL IsActive(HCONTROL hControl)
{
    if (GET_MODAL() == INVALID_CONTROL_HANDLE)
    {
        return ((GetFocusedIOC() == hControl) && (GetWindowMode() == WM_IO_MODE));
    }
    else
    {
        return (GET_MODAL() == hControl);
    }
}

LOCAL BOOL sMoveSlideWindow(EQS_EVENT_ID EventID)
{
    EQS_EVENT TmpEvent;
    HCONTROL  hControl, hFocusedControl, hTmpControl;
	COORD     PreviousX;
	RECT      Rect;
	UINT8     nOffset;

	hFocusedControl = GetFocusedIOC();

    if (EventID == EQE_KEY_UP)
    {
        if ( hFocusedControl == INVALID_CONTROL_HANDLE)
        {
            if (sSlideWindow.yWindowOffset > 0)
            {				
				nOffset = MIN((sSlideWindow.yWindowOffset), SLIDING_UNIT);
                sSlideWindow.yWindowOffset -= nOffset;
				if( sSlideWindow.yWindowOffset < 0  )
				{
					sSlideWindow.yWindowOffset = 0;
				}
                SetSlidingWindowOffsetY((COORD)sSlideWindow.yWindowOffset);

                if ((hControl = GetLastFocusibleIOC()) != INVALID_CONTROL_HANDLE)
                {
                    SetCurrentWindowIOC(hControl);
					sMakeEvent(&TmpEvent, EQE_APP_DRAW, hControl, EQS_NA);
                }
				else // 01.06.01
				{
					sMakeEvent(&TmpEvent, EQE_APP_SLIDE_MOVE, (WORD)0, (DWORD)0);
					AddEventQueue(&TmpEvent);
					sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
				}                
                AddEventQueue(&TmpEvent);

                //ScrollScreen(0,0, 0, nOffset, DevGetMaxX()+1, sSlideWindow.dyWindowSize - nOffset+1);					
                return TRUE;
            }
        }
        else if ((hControl = GetPrevFocusibleIOC()) != INVALID_CONTROL_HANDLE)
        {
			int y, dy;

			y = EQC_GET_Y(hControl);
			dy = EQC_GET_DY(hControl);

            if ((y >= sSlideWindow.yWindowOffset +2) && 
               ((y + dy - 1) <= (sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize -1 -2)))
			{				
				sMakeEvent(&TmpEvent, EQE_APP_DRAW, hFocusedControl, EQS_NA); // draw changed control
				AddEventQueue(&TmpEvent);
                sMakeEvent(&TmpEvent, EQE_APP_DRAW, hControl, EQS_NA); // draw changed control
			}
			else
			{	
				PreviousX = sSlideWindow.yWindowOffset;
				sSlideWindow.yWindowOffset =  y;

				if( sSlideWindow.yWindowOffset > SLIDING_UNIT*2 )
					sSlideWindow.yWindowOffset -= SLIDING_UNIT*2;
				else 
					sSlideWindow.yWindowOffset = 0;				
				SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);

				//nOffset = PreviousX - sSlideWindow.yWindowOffset;
				//ScrollScreen(0,nOffset, 0, 0, DevGetMaxX()+1, sSlideWindow.dyWindowSize - nOffset);
				//sGetSWindowViewAreaRect(&Rect);
				//l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(),Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);		                  
				sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);  //draw all  01.08.23
			}

            SetCurrentWindowIOC(hControl);
/*
			if ((hTmpControl = GetPrevFocusibleIOC()) != INVALID_CONTROL_HANDLE) // 수직 라인을 맞춘다.
			{
				if( EQC_GET_Y(hControl) == EQC_GET_Y(hFocusedControl) )
				{
					hControl = hTmpControl;
					y = EQC_GET_Y(hControl);
					dy = EQC_GET_DY(hControl);

					if ((y >= sSlideWindow.yWindowOffset +2) && 
					   ((y + dy - 1) <= (sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize -1 -2)))
					{
						;
					}
					else
					{	
						sSlideWindow.yWindowOffset =  y;

						if( sSlideWindow.yWindowOffset - 2 >= 0 )
							sSlideWindow.yWindowOffset -=2;

						SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
					}
					SetCurrentWindowIOC(hControl);								
					if ((hTmpControl = GetPrevFocusibleIOC()) != INVALID_CONTROL_HANDLE)
					{
						if( EQC_GET_Y(hControl) == EQC_GET_Y(hTmpControl) )
						{
							if( EQC_GET_X(hFocusedControl) == EQC_GET_X(hTmpControl) )
							{
								SetCurrentWindowIOC(hTmpControl);
							}
						}
					}
				}
				else
				{
					if( EQC_GET_Y(hControl) == EQC_GET_Y(hTmpControl) )
					{
						if( EQC_GET_X(hFocusedControl) == EQC_GET_X(hTmpControl) )
						{
							SetCurrentWindowIOC(hTmpControl);
						}
					}
				}
			}
*/

/* 01.06.01 필요없음 
            sMakeEvent(&TmpEvent, EQE_APP_SLIDE_MOVE, (WORD)0, (DWORD)0);
            AddEventQueue(&TmpEvent);
*/
            //sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);  //01.08.23
            AddEventQueue(&TmpEvent);

            return TRUE;
        }
        else
        {
            // Else, slide the window.
            if (sSlideWindow.yWindowOffset > 0)
            {
                sSlideWindow.yWindowOffset -= MIN((sSlideWindow.yWindowOffset), SLIDING_UNIT);
				if( sSlideWindow.yWindowOffset < 0  )
				{
					sSlideWindow.yWindowOffset = 0;
				}
                SetSlidingWindowOffsetY((COORD)sSlideWindow.yWindowOffset);

                if ((hControl = GetFirstFocusibleIOC()) != INVALID_CONTROL_HANDLE)
                {
                    SetCurrentWindowIOC(hControl);
                }
				else // 01.06.01
				{
					sMakeEvent(&TmpEvent, EQE_APP_SLIDE_MOVE, (WORD)0, (DWORD)0);
					AddEventQueue(&TmpEvent);
				}
                sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
                AddEventQueue(&TmpEvent);

                return TRUE;
            }
        }
    }
    else if (EventID == EQE_KEY_DOWN)
    {
        if ( hFocusedControl == INVALID_CONTROL_HANDLE)
        {
            if (sSlideWindow.yWindowOffset < sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize)
            {
                sSlideWindow.yWindowOffset += MIN((sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize - sSlideWindow.yWindowOffset), SLIDING_UNIT);
                SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);

                if ((hControl = GetFirstFocusibleIOC()) != INVALID_CONTROL_HANDLE)
                {
                    SetCurrentWindowIOC(hControl);
					sMakeEvent(&TmpEvent, EQE_APP_DRAW, hControl, EQS_NA);
                }
				else // 01.06.01
				{
					sMakeEvent(&TmpEvent, EQE_APP_SLIDE_MOVE, (WORD)0, (DWORD)0);
					AddEventQueue(&TmpEvent);
					sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
				}                
                AddEventQueue(&TmpEvent);

                return TRUE;
            }

        }
        else if ((hControl = GetNextFocusibleIOC()) != INVALID_CONTROL_HANDLE)
        {
			int y, dy;

			y = EQC_GET_Y(hControl);
			dy = EQC_GET_DY(hControl);

            if ((y >= sSlideWindow.yWindowOffset) && 
               ((y + dy - 1) <= (sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize - 1 -2)))
			{
				sMakeEvent(&TmpEvent, EQE_APP_DRAW, hFocusedControl, EQS_NA);
				AddEventQueue(&TmpEvent);
				sMakeEvent(&TmpEvent, EQE_APP_DRAW, hControl, EQS_NA);
			}
			else
			{	
				sSlideWindow.yWindowOffset =  (y+dy-1) - sSlideWindow.dyWindowSize + 4;
				if( sSlideWindow.yWindowOffset < 0  )
				{
					sSlideWindow.yWindowOffset = 0;
				}
				SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
				sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);  
			}
			SetCurrentWindowIOC(hControl);
/*
			if ((hTmpControl = GetNextFocusibleIOC()) != INVALID_CONTROL_HANDLE) // 수직라인을 맞춘다.
			{
				if( EQC_GET_Y(hControl) == EQC_GET_Y(hFocusedControl) )
				{
					hControl = hTmpControl;
					y = EQC_GET_Y(hControl);
					dy = EQC_GET_DY(hControl);

					if ((y >= sSlideWindow.yWindowOffset) && 
					   ((y + dy - 1) <= (sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize - 1 -2)))
					{
						;
					}
					else
					{	
						sSlideWindow.yWindowOffset =  (y+dy-1) - sSlideWindow.dyWindowSize + 4;
						SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
					}
					SetCurrentWindowIOC(hControl);								
					if ((hTmpControl = GetNextFocusibleIOC()) != INVALID_CONTROL_HANDLE)
					{
						if( EQC_GET_Y(hControl) == EQC_GET_Y(hTmpControl) )
						{
							if( EQC_GET_X(hFocusedControl) == EQC_GET_X(hTmpControl) )
							{
								SetCurrentWindowIOC(hTmpControl);
							}
						}
					}
				}
				else
				{
					if( EQC_GET_Y(hControl) == EQC_GET_Y(hTmpControl) )
					{
						if( EQC_GET_X(hFocusedControl) == EQC_GET_X(hTmpControl) )
						{
							SetCurrentWindowIOC(hTmpControl);
						}
					}
				}
			}
*/
/*          01.06.01
			sMakeEvent(&TmpEvent, EQE_APP_SLIDE_MOVE, (WORD)0, (DWORD)0);
			AddEventQueue(&TmpEvent);
*/
			//sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);  01.08.23
			AddEventQueue(&TmpEvent);
            return TRUE;
        }
        else
        {
            // Else, slide the window.
            if (sSlideWindow.yWindowOffset < sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize)
            {
                sSlideWindow.yWindowOffset += MIN((sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize - sSlideWindow.yWindowOffset), SLIDING_UNIT);
                SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);

                if ((hControl = GetLastFocusibleIOC()) != INVALID_CONTROL_HANDLE)
                {
                    SetCurrentWindowIOC(hControl);
                }
				else // 01.06.01
				{
					sMakeEvent(&TmpEvent, EQE_APP_SLIDE_MOVE, (WORD)0, (DWORD)0);
					AddEventQueue(&TmpEvent);
				}
                sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
                AddEventQueue(&TmpEvent);

                return TRUE;
            }
        }
    }
/*
	else if (EventID == EQE_KEY_LEFT)
	{
		hControl = GetFocusedIOC();
		if (hControl != INVALID_CONTROL_HANDLE)
		{
			HCONTROL PreControl;

			PreControl = GetPrevFocusibleIOC();
			if( PreControl != INVALID_CONTROL_HANDLE )
			{
				if( EQC_GET_Y(hControl) == EQC_GET_Y(PreControl) )
				{
					SetCurrentWindowIOC(PreControl);
					sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
                    AddEventQueue(&TmpEvent);
				}
			}
		}
	}
	else if (EventID == EQE_KEY_RIGHT)
	{
		hControl = GetFocusedIOC();
		if (hControl != INVALID_CONTROL_HANDLE)
		{
			HCONTROL NextControl;

			NextControl = GetNextFocusibleIOC();
			if( NextControl != INVALID_CONTROL_HANDLE )
			{
				if( EQC_GET_Y(hControl) == EQC_GET_Y(NextControl) )
				{
					SetCurrentWindowIOC(NextControl);
					sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
                    AddEventQueue(&TmpEvent);
				}
			}
		}
	}
*/
	else ;

    return FALSE;
}


LOCAL WINDOW_MODE GetWindowMode(void)
{
    if (GET_MODAL() == INVALID_CONTROL_HANDLE)
        return sSlideWindow.tWindowMode;
    else
        return WM_MODAL_IO_MODE;
}

LOCAL void SetWindowCML(UINT8 nCML)
{
    CurrentAppletState.nCML = nCML;
}

LOCAL UINT8 GetWindowCML(void)
{
    return CurrentAppletState.nCML;
}

LOCAL void SetWindowMode(WINDOW_MODE WindowMode)
{
    sSlideWindow.tWindowMode = WindowMode;
}

LOCAL HCONTROL GetFocusedIOC(void)
{
    HCONTROL hControl;

    if ((hControl = CurrentAppletState.AttachedIOControls[sSlideWindow.CurrentIOCIndex]) == INVALID_CONTROL_HANDLE)
        return INVALID_CONTROL_HANDLE;

    if ((EQC_GET_FOCUS(hControl) == TRUE) && (IsAnIOCInWindowExclusive(hControl) == TRUE))
    {
        return hControl;
    }
    else
    {
        return INVALID_CONTROL_HANDLE;
    }
}

LOCAL HCONTROL GetActiveIOC(void)
{
    HCONTROL hControl;

    hControl = CurrentAppletState.AttachedIOControls[sSlideWindow.CurrentIOCIndex];

    if (GetWindowMode() == WM_IO_MODE)
    {
        return hControl;
    }
    else
    {
        return INVALID_CONTROL_HANDLE;
    }
}


LOCAL HCONTROL GetNthIOC(UINT8 unNth)
{
    if (unNth < MAX_CONTROL_PER_APPLET)
        return CurrentAppletState.AttachedIOControls[unNth];
    else
        return INVALID_CONTROL_HANDLE;
}

LOCAL HCONTROL GetLastIOC(void)
{
    UINT8 i;
    HCONTROL hControl = INVALID_CONTROL_HANDLE;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (CurrentAppletState.AttachedIOControls[i] != INVALID_CONTROL_HANDLE)
        {
            hControl = CurrentAppletState.AttachedIOControls[i];
        }
        else
        {
            break;
        }
    }

    return hControl;
}

LOCAL EQS_RESULT SetCurrentWindowIOC(HCONTROL hControl)
{
    EQS_EVENT  InfoEvent;
    UINT8 i;

	if( hControl == INVALID_CONTROL_HANDLE )
		return RESULT_INVALID_HANDLE;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (CurrentAppletState.AttachedIOControls[i] == hControl)
        {
            if( sSlideWindow.CurrentIOCIndex != i )
            {
				sSlideWindow.CurrentIOCIndex = i;
                InfoEvent.EventID = EQE_IO_FOCUS_CHG;
                InfoEvent.wParam  = (WORD)hControl;
                InfoEvent.dwParam = EQS_NA;
                InfoEvent.dwExtEventID = EQS_NA;
                (*CurrentAppletState.pApplet)(&InfoEvent);
            }
			else
			{
				sSlideWindow.CurrentIOCIndex = i;
			}
            return RESULT_OK;
        }
        else if (CurrentAppletState.AttachedIOControls[i] == INVALID_CONTROL_HANDLE)
            return RESULT_INVALID_HANDLE;
    }

    return RESULT_INVALID_HANDLE;
}

/*
** 
*/

/*
LOCAL HCONTROL GetPrevWindowIOC(void)
{
    if (sSlideWindow.CurrentIOCIndex > 0)
    {
        return CurrentAppletState.AttachedIOControls[sSlideWindow.CurrentIOCIndex - 1];
    }
    else
    {
        return INVALID_CONTROL_HANDLE;
    }
}

LOCAL HCONTROL GetNextWindowIOC(void)
{
    if (sSlideWindow.CurrentIOCIndex + 1 < MAX_CONTROL_PER_APPLET)
    {
        return CurrentAppletState.AttachedIOControls[sSlideWindow.CurrentIOCIndex + 1];
    }
    else
    {
        return INVALID_CONTROL_HANDLE;
    }
}

LOCAL HCONTROL GetCurrentWindowIOC(void)
{
    return CurrentAppletState.AttachedIOControls[sSlideWindow.CurrentIOCIndex];
}
*/

LOCAL HCONTROL GetPrevFocusibleIOC(void)
{
    UINT8 i;
    HCONTROL hControl;

    if (sSlideWindow.CurrentIOCIndex == 0)
        return INVALID_CONTROL_HANDLE;

    for (i = sSlideWindow.CurrentIOCIndex - 1; i >0; i--)
    {
        if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
        {
            if (EQC_GET_FOCUS(hControl) == TRUE)
            {
				return hControl;
/*
                if (IsAnIOCInWindowExclusive(hControl) == TRUE)
                {
                    return hControl;
                }
                else
                {
                    return INVALID_CONTROL_HANDLE;
                }
*/
            }
        }
        else
        {
            return INVALID_CONTROL_HANDLE;
        }
    }

    // i == 0
    if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
    {
        if (EQC_GET_FOCUS(hControl) == TRUE)
        {
			return hControl;
/* 
            if (IsAnIOCInWindowExclusive(hControl) == TRUE)
            {
                return hControl;
            }
            else
            {
                return INVALID_CONTROL_HANDLE;
            }
*/
        }
        else
        {
            return INVALID_CONTROL_HANDLE;
        }
    }
    else
    {
        return INVALID_CONTROL_HANDLE;
    }

}
LOCAL HCONTROL GetNextFocusibleIOC(void)
{
    UINT8 i;
    HCONTROL hControl;

    if (sSlideWindow.CurrentIOCIndex >= (MAX_CONTROL_PER_APPLET - 1))
        return INVALID_CONTROL_HANDLE;

    for (i = sSlideWindow.CurrentIOCIndex + 1; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
        {
            if (EQC_GET_FOCUS(hControl) == TRUE)
            {
				return hControl;
/*
                if (IsAnIOCInWindowExclusive(hControl) == TRUE)
                {
                    return hControl;
                }
                else
                {
                    return INVALID_CONTROL_HANDLE;
                }
*/
            }
        }
        else
        {
            return INVALID_CONTROL_HANDLE;
        }
    }

    return INVALID_CONTROL_HANDLE;
}

LOCAL HCONTROL GetFirstFocusibleIOC(void)
{
    UINT8 i;
    HCONTROL hControl;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
        {
            if (IsAnIOCInWindowExclusive(hControl) == TRUE)
            {
                if (EQC_GET_FOCUS(hControl) == TRUE)
                {
                    return hControl;
                }
            }
        }
        else
        {
            return INVALID_CONTROL_HANDLE;
        }
    }

    return INVALID_CONTROL_HANDLE;
}

LOCAL HCONTROL GetLastFocusibleIOC(void)
{
    UINT8 i;
    HCONTROL hControl;
    HCONTROL hControlLast = INVALID_CONTROL_HANDLE;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
        {
            if (IsAnIOCInWindowExclusive(hControl) == TRUE)
            {
                if (EQC_GET_FOCUS(hControl) == TRUE)
                {
                    hControlLast = hControl;
                }
            }
        }
        else
        {
            return hControlLast;
        }
    }

    return hControlLast;
}



LOCAL INT8 CompareIOCPosition(HCONTROL hCont1, HCONTROL hCont2)
{
    if(EQC_GET_Y(hCont1) > EQC_GET_Y(hCont2))// (g_CommonIOControlHeader[hCont1].y > g_CommonIOControlHeader[hCont2].y)
        return 1;
    else if (EQC_GET_Y(hCont1) < EQC_GET_Y(hCont2))//g_CommonIOControlHeader[hCont1].y < g_CommonIOControlHeader[hCont2].y)
        return -1;
    else // same y-axis position
    {
        if (EQC_GET_X(hCont1) > EQC_GET_X(hCont2))//g_CommonIOControlHeader[hCont1].x > g_CommonIOControlHeader[hCont2].x)
            return 1;
        else if (EQC_GET_X(hCont1) < EQC_GET_X(hCont2))//g_CommonIOControlHeader[hCont1].x < g_CommonIOControlHeader[hCont2].x)
            return -1;
        else
            return 0;
    }
}

LOCAL void sAlignFocus(void)
{ // Set the IOC focus on the first IOC which deserves the focus
    UINT8 i;
    HCONTROL hControl;

	hControl = CurrentAppletState.AttachedIOControls[sSlideWindow.CurrentIOCIndex];
	if( hControl != INVALID_CONTROL_HANDLE)
    {
		if( EQC_GET_FOCUS(hControl) == TRUE )
			return ;
	}

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if ((hControl = CurrentAppletState.AttachedIOControls[i]) != INVALID_CONTROL_HANDLE)
        {
            if (EQC_GET_FOCUS(hControl) == TRUE)
            {
                if (IsAnIOCInWindowExclusive(hControl) == TRUE)
                {
                    sSlideWindow.CurrentIOCIndex = i;
                    return;
                }
            }
        }
        else
        {
            sSlideWindow.CurrentIOCIndex = 0; // meaningless
            return;
        }
    }

    sSlideWindow.CurrentIOCIndex = 0; // meaningless
}



/*
**---------------------------------------------------------------------------
**  Global Function Definitions
**---------------------------------------------------------------------------
*/
HWINDOW EQS_SetWindow(COORD xPos_RFU, 
                      COORD yPos, 
                      COORD dxWidth_RFU, 
                      COORD dyHeight, 
                      UINT8 nCML,
                      CONST BYTE* pszHeadTitle,
                      WORD wAttr)
{
    sSlideWindow.yWindowOffset = yPos;
    CurrentAppletState.dyCanvasSize = sSlideWindow.dyCanvasSize = dyHeight;
    CurrentAppletState.nCML = nCML;
    CurrentAppletState.wCmlAttr = wAttr;
    CurrentAppletState.pszSWindowTitle = pszHeadTitle;
    sSlideWindow.dyWindowSize = sGetWindowSize();

	SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);


    EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0);

    return 0;
}

EQS_RESULT EQS_SetWindowCML(UINT8 nCML)
{

    SetWindowCML(nCML);
    sSlideWindow.dyWindowSize = sGetWindowSize();

    EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_CML, EQS_NA);

    return RESULT_OK;
}

UINT8 EQS_GetWindowCML(void)
{
    return GetWindowCML();
}

EQS_RESULT EQS_SetWindowAttr(WORD wAttr)
{
    CurrentAppletState.wCmlAttr = wAttr;
    sSlideWindow.dyWindowSize = sGetWindowSize();

    EQS_SendEvent(EQE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);

    return RESULT_OK;
}


WORD EQS_GetWindowAttr(void)
{
    return CurrentAppletState.wCmlAttr;
}

void EQS_API EQS_SetSWindowHeaderStr(CONST BYTE* pszHeaderTitle)
{
    CurrentAppletState.pszSWindowTitle = pszHeaderTitle;
}

CONST BYTE* EQS_API EQS_GetCurrentHeaderStr(void)
{ 
    return CurrentAppletState.pszSWindowTitle; 
}

EQS_RESULT EQS_SetCanvasSize(COORD dxSize_RFU, COORD dySize)
{
	if( dySize > 0 )
	{
		CurrentAppletState.dyCanvasSize = sSlideWindow.dyCanvasSize = dySize;
		if( sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize > sSlideWindow.dyCanvasSize )
		{
			sSlideWindow.yWindowOffset = sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize;
			if( sSlideWindow.yWindowOffset < 0 )
				sSlideWindow.yWindowOffset = 0;
			SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
		}
	}
    return RESULT_OK;
}

COORD  EQS_GetCanVasHeight(void)
{
	SIZE CanvasSize;

	return sSlideWindow.dyCanvasSize;

}



COORD EQS_API EQS_FitCanvasSize(void)
{
    RECT Rect;
    HCONTROL hControl;
    COORD yDefCanvasSize;
    COORD   nMaxY, nTmpY;
    UINT16  i;

    sGetSWindowViewAreaRect(&Rect);
    yDefCanvasSize = Rect.yBottom - Rect.yTop + 1;

//    hControl = GetLastIOC();

    nMaxY = 0;
    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (CurrentAppletState.AttachedIOControls[i] != INVALID_CONTROL_HANDLE)
        {
            hControl = CurrentAppletState.AttachedIOControls[i];
            nTmpY = EQC_GET_Y(hControl) + EQC_GET_DY(hControl);
            if(nTmpY > nMaxY)
                nMaxY = nTmpY;
        }
        else
        {
            break;
        }
    }

    if (nMaxY == 0) // There is no attached IOC.
    {
        CurrentAppletState.dyCanvasSize = yDefCanvasSize;        
    }
    else
    {
        if (nMaxY < yDefCanvasSize - 1)
        {
            CurrentAppletState.dyCanvasSize = yDefCanvasSize;            
        }
        else
        {
            CurrentAppletState.dyCanvasSize = nMaxY + 2;            
        }
    }
    sSlideWindow.dyCanvasSize = CurrentAppletState.dyCanvasSize;  //redstar 2001.01.14
	if( sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize > sSlideWindow.dyCanvasSize )
	{
		sSlideWindow.yWindowOffset = sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize;
		if( sSlideWindow.yWindowOffset < 0 )
			sSlideWindow.yWindowOffset = 0;
		SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
	}

    return CurrentAppletState.dyCanvasSize;
}


EQS_RESULT EQS_SetIOCFocus(HCONTROL hControl)
{
    EQS_EVENT TmpEvent;


	if( hControl >= INVALID_CONTROL_HANDLE )
		return  RESULT_INVALID_HANDLE;

    if (EQC_GET_FOCUS(hControl) == FALSE)
        return RESULT_INVALID_HANDLE;

    if (SetCurrentWindowIOC(hControl) == RESULT_OK)
    {
		int y, dy;

		y = EQC_GET_Y(hControl);
		dy = EQC_GET_DY(hControl);

        if ((y >= sSlideWindow.yWindowOffset) && 
           ((y + dy - 1) <= (sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize - 1 -2)))
		{
			;
		}
		else
		{	
			sSlideWindow.yWindowOffset =  (y+dy-1) - sSlideWindow.dyWindowSize + 4;
			if( sSlideWindow.yWindowOffset < 0  )
			{
				sSlideWindow.yWindowOffset = 0;
			}
			SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
		}
		SetCurrentWindowIOC(hControl);

#if 0

        if ((y >= sSlideWindow.yWindowOffset +2) && 
           ((y + dy - 1) <= (sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize -1 -2)))
		{
			 if(  sSlideWindow.yWindowOffset + SLIDING_UNIT*2 > y )
			 {							
			    if( sSlideWindow.yWindowOffset > SLIDING_UNIT*2 )
					sSlideWindow.yWindowOffset -= SLIDING_UNIT*2;
				else 
					sSlideWindow.yWindowOffset = 0;
				SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
			 }
		}
		else
		{	
			sSlideWindow.yWindowOffset =  y;
			if( sSlideWindow.yWindowOffset+sSlideWindow.dyWindowSize -1 > sSlideWindow.dyCanvasSize )
			{
				sSlideWindow.yWindowOffset = sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize -2;
			}
			else
			{
				if( sSlideWindow.yWindowOffset  > SLIDING_UNIT*2 )
					sSlideWindow.yWindowOffset  -= SLIDING_UNIT*2;
			}

			SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
		}
#endif
/* IsAnIOCInWindowExclusive(hControl)를 사용하지 않는 이유는 border를 고려해야 하기 때문
   border로 1을 사용하기 때문에  화면에 제대로 보이게 하려면 2정도는 더 고려해야함
        if (!IsAnIOCInWindowExclusive(hControl))
        {
            sSlideWindow.yWindowOffset = MAX((EQC_GET_Y(hControl) - 5), 0);
			if( sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize -1 >\
                sSlideWindow.dyWindowSize )
			{
				sSlideWindow.yWindowOffset = sSlideWindow.dyCanvasSize - \
					                         sSlideWindow.dyWindowSize + 1;
			}				
            SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
        }
*/

        sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
        AddEventQueue(&TmpEvent);

        return RESULT_OK;
    }
    else
        return RESULT_INVALID_HANDLE;
}

EQS_RESULT   EQS_SetNthIOCFocus(UINT8 nOrder)
{
    return EQS_SetIOCFocus(GetNthIOC(nOrder));
}

EQS_RESULT EQS_SetIOCActive(HCONTROL hControl)
{
    EQS_EVENT TmpEvent;

	if( hControl >= INVALID_CONTROL_HANDLE )
		return RESULT_INVALID_HANDLE;

    if (IS_MODAL(hControl))
    {
        SET_MODAL(hControl);

        // girin. 2000. 12.19
        // Don't set window mode here. 
        // The window mode is to remain as it was when the new
        // modal I/O control begins. GetWindowMode() returns 
        // WM_MODAL_IO_MODE if there is modal I/O control active
        // in the window.
        //
        // SetWindowMode(WM_IO_MODE);

        sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
        AddEventQueue(&TmpEvent);

        return RESULT_OK;
    }
    else if (SetCurrentWindowIOC(hControl) == RESULT_OK)
    {
        SetWindowMode(WM_IO_MODE);
        sMakeEvent(&TmpEvent, EQE_APP_IO_MODE, hControl, EQS_NA);
        AddEventQueue(&TmpEvent);

        if (!IsAnIOCInWindowExclusive(hControl))
        {
/*
            sSlideWindow.yWindowOffset = MAX((EQC_GET_Y(hControl) - 5), 0);
            SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
*/
			int y, dy;

			y = EQC_GET_Y(hControl);
			dy = EQC_GET_DY(hControl);			

			sSlideWindow.yWindowOffset =  y;
			if( sSlideWindow.yWindowOffset+sSlideWindow.dyWindowSize -1 > sSlideWindow.dyCanvasSize )
			{
				sSlideWindow.yWindowOffset = sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize -2;
			}
			else
			{
				if( sSlideWindow.yWindowOffset - 2 >= 0 )
					sSlideWindow.yWindowOffset -=2;
			}

			SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);

        }

        sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
        AddEventQueue(&TmpEvent);

        return RESULT_OK;
    }
    else
        return RESULT_INVALID_HANDLE;
}

EQS_RESULT EQS_GetIOCWindowPosX(HCONTROL hControl, COORD* pX)
{
    *pX = EQC_GET_X(hControl);//g_CommonIOControlHeader[hControl].x;
    return RESULT_OK;
}

EQS_RESULT EQS_GetIOCWindowPosY(HCONTROL hControl, COORD* pY)
{
    WORD wSWindowAttr;
    COORD Left, Top;

    wSWindowAttr = EQS_GetWindowAttr();

    if ((wSWindowAttr & WA_SHOW_HEADER) == WA_SHOW_HEADER)
    {
        Left = 0;
        Top = EQS_GetSWindowHeaderHeight();
    }
    else
    {
        Left = 0;
        Top = 0;
    }
    *pY = EQC_GET_Y(hControl) - sSlideWindow.yWindowOffset;
    return RESULT_OK;
}

void EQS_GetSWindowViewAreaRect(RECT* pRect)
{
    sGetSWindowViewAreaRect(pRect);
}
COORD EQS_GetSWindowOffset(void)
{
	return sSlideWindow.yWindowOffset;
}
HCONTROL   EQS_GetFocusedIOC(void)
{
    return GetFocusedIOC();
}
HCONTROL     EQS_GetActiveIOC(void)
{
	HCONTROL hControl;

	hControl = GetFocusedIOC();

	if( hControl != INVALID_CONTROL_HANDLE )
	{
		if( EQS_IsActive(hControl) == TRUE )
		{
			return hControl;
		}
	}
		
	return  INVALID_CONTROL_HANDLE;
}

HCONTROL   EQS_GetNthIOC(UINT8 unNth)
{
    return GetNthIOC(unNth);
}

UINT8 EQS_GetIOCOrderly(HCONTROL hControl)
{
    UINT8 i;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (CurrentAppletState.AttachedIOControls[i] == hControl)
        {
            return i;
        }
        else if (CurrentAppletState.AttachedIOControls[i] == INVALID_CONTROL_HANDLE)
        {
            return EQS_INVALID_CONTROL_ORDER;
        }
    }

    return EQS_INVALID_CONTROL_ORDER;
}


INT8 EQS_API EQS_GetIOCOrder(HCONTROL hControl)
{
    UINT8 i;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (CurrentAppletState.AttachedIOControls[i] == hControl)
        {
            return i;
        }
        else if (CurrentAppletState.AttachedIOControls[i] == INVALID_CONTROL_HANDLE)
            return -1;
    }

    return -1;
}

UINT8       EQS_GetIOCNum(void)
{
    UINT8 i;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        if (CurrentAppletState.AttachedIOControls[i] == INVALID_CONTROL_HANDLE)
        {
            return i;
        }
    }

    return i;
}


//------------------------------------------------------

EQS_RESULT EQS_StartChild( EQS_APPLET *pAppFuncA  )
{

    Prepare2StartNextApplet(pAppFuncA, EQE_APP_START, 0, 0, FALSE);

    // set spawing flag
    AppletStartFlag = EQS_APPLET_CHILD_READY;
    return RESULT_OK;
}

EQS_RESULT EQS_StartChildEvent(EQS_APPLET *pAppFunA, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam)
{

    // preset with parameters
    Prepare2StartNextApplet(pAppFunA, EventID, wParam, dwParam, FALSE);

    // set spawing flag
    AppletStartFlag = EQS_APPLET_CHILD_READY;

    return RESULT_OK;
}

EQS_RESULT EQS_StartChildCurrentEvent(EQS_APPLET *pAppFuncA)
{
#ifdef EXTENDED_EVENT_REGIME
    if (CurrentEvent.EventID == EQE_BYPASS)
        Prepare2StartNextApplet(pAppFuncA, (EQS_EVENT_ID)CurrentEvent.dwExtEventID, CurrentEvent.wParam, CurrentEvent.dwParam, FALSE);
    else
#endif
    // preset with parameters
    Prepare2StartNextApplet(pAppFuncA, CurrentEvent.EventID, CurrentEvent.wParam, CurrentEvent.dwParam, FALSE);

    // set spawing flag
    AppletStartFlag = EQS_APPLET_CHILD_READY;
    return RESULT_OK;
}

EQS_RESULT EQS_StartSibling(EQS_APPLET *pAppFuncA, BOOL fDispUntouched)
{
    Prepare2StartNextApplet(pAppFuncA, EQE_APP_START, 0, 0, fDispUntouched);

    // set spawing flag
    AppletStartFlag = EQS_APPLET_SIBLING_READY;
    return RESULT_OK;
}

EQS_RESULT EQS_StartSiblingEvent(EQS_APPLET *pAppFuncA, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam, BOOL fDispUntouched)
{
    Prepare2StartNextApplet(pAppFuncA, EventID, wParam, dwParam, fDispUntouched);

    // set spawing flag
    AppletStartFlag = EQS_APPLET_SIBLING_READY;
    return RESULT_OK;
}


EQS_RESULT EQS_StartSiblingCurrentEvent(EQS_APPLET *pAppFuncA, BOOL fDispUntouched)
{
#ifdef EXTENDED_EVENT_REGIME
    if (CurrentEvent.EventID == EQE_BYPASS)
        Prepare2StartNextApplet(pAppFuncA, (EQS_EVENT_ID)CurrentEvent.dwExtEventID, CurrentEvent.wParam, CurrentEvent.dwParam, fDispUntouched);
    else
#endif
    Prepare2StartNextApplet(pAppFuncA, CurrentEvent.EventID, CurrentEvent.wParam, CurrentEvent.dwParam, fDispUntouched);

    // set spawing flag
    AppletStartFlag = EQS_APPLET_SIBLING_READY;
    return RESULT_OK;
}

EQS_RESULT  EQS_EndTo(EQS_POPUP ReturnPlace)
{
    Prepare2EndApplet(ReturnPlace, EQE_APP_RESUME, 0, 0);

    // set start flag
    AppletStartFlag = EQS_APPLET_END_READY;
    
    return RESULT_OK;
}

EQS_RESULT EQS_EndToParam(EQS_POPUP ReturnPlace, WORD wParam, DWORD dwParam)
{
    Prepare2EndApplet(ReturnPlace, EQE_APP_RESUME, wParam, dwParam);

    // set start flag
    AppletStartFlag = EQS_APPLET_END_READY;
    
    return RESULT_OK;
}


EQS_RESULT EQS_EndToEvent(EQS_POPUP ReturnPlace, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam)
{
    
    Prepare2EndApplet(ReturnPlace, EventID, wParam, dwParam);

    // set start flag
    AppletStartFlag = EQS_APPLET_END_READY;
    
    return RESULT_OK;
}

       
EQS_RESULT EQS_EndToCurrentEvent(EQS_POPUP ReturnPlace)
{
#ifdef EXTENDED_EVENT_REGIME
    if (CurrentEvent.EventID == EQE_BYPASS)
        Prepare2EndApplet(ReturnPlace, (EQS_EVENT_ID)CurrentEvent.dwExtEventID, CurrentEvent.wParam, CurrentEvent.dwParam);
    else
#endif
    Prepare2EndApplet(ReturnPlace, CurrentEvent.EventID, CurrentEvent.wParam, CurrentEvent.dwParam);

    // set start flag
    AppletStartFlag = EQS_APPLET_END_READY;
    return RESULT_OK;
}

EQS_RESULT  EQS_EndDest(EQS_APPLET *pAppFuncA)
{
    Prepare2EndApplet(EQS_POP_TO_DEST, EQE_APP_RESUME, 0, 0);
    EndDestApplet = pAppFuncA;

    // set start flag
    AppletStartFlag = EQS_APPLET_END_READY;
    
    return RESULT_OK;
}

EQS_RESULT EQS_EndDestParam(EQS_APPLET *pAppFuncA, WORD wParam, DWORD dwParam)
{
    Prepare2EndApplet(EQS_POP_TO_DEST, EQE_APP_RESUME, wParam, dwParam);
    EndDestApplet = pAppFuncA;

    // set start flag
    AppletStartFlag = EQS_APPLET_END_READY;
    
    return RESULT_OK;
}


EQS_RESULT EQS_EndDestEvent(EQS_APPLET *pAppFuncA, EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam)
{
    
    Prepare2EndApplet(EQS_POP_TO_DEST, EventID, wParam, dwParam);
    EndDestApplet = pAppFuncA;

    // set start flag
    AppletStartFlag = EQS_APPLET_END_READY;
    
    return RESULT_OK;
}

       
EQS_RESULT EQS_EndDestCurrentEvent(EQS_APPLET *pAppFuncA)
{
#ifdef EXTENDED_EVENT_REGIME
    if (CurrentEvent.EventID == EQE_BYPASS)
        Prepare2EndApplet(EQS_POP_TO_DEST, (EQS_EVENT_ID)CurrentEvent.dwExtEventID, CurrentEvent.wParam, CurrentEvent.dwParam);
    else
#endif
    Prepare2EndApplet(EQS_POP_TO_DEST, CurrentEvent.EventID, CurrentEvent.wParam, CurrentEvent.dwParam);
    EndDestApplet = pAppFuncA;

    // set start flag
    AppletStartFlag = EQS_APPLET_END_READY;
    return RESULT_OK;
}

EQS_RESULT  EQS_End(void)
{
    return EQS_EndTo(EQS_POP_TO_PARENT); 
}

EQS_RESULT EQS_EndParam(WORD wParam, DWORD dwParam)
{
    return EQS_EndToParam(EQS_POP_TO_PARENT, wParam, dwParam);
}


EQS_RESULT EQS_EndEvent(EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam)
{
    return EQS_EndToEvent(EQS_POP_TO_PARENT, EventID, wParam, dwParam);
}


EQS_RESULT EQS_EndCurrentEvent(void)
{
    return EQS_EndToCurrentEvent(EQS_POP_TO_PARENT);
}

EQS_RESULT EQS_EndIO(void)
{   // Quit IO mode
    EQS_EVENT TmpEvent;

    SetWindowMode(WM_APPLET_MODE);
    SetSlidingWindowState(TRUE);
	sMakeEvent(&TmpEvent, EQE_APP_IO_END, EQS_GetFocusedIOC(), EQS_NA);
	AddEventQueue(&TmpEvent);
    sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
    AddEventQueue(&TmpEvent);

    return RESULT_OK;
}

EQS_RESULT EQS_EndModalIO(void)
{
    EQS_EVENT TmpEvent;

    // Don't reset window mode here. It remains as it was..

    // reset modal setting.
	
	//sSlideWindow.tWindowMode = WM_APPLET_MODE; //deactive
    RESET_MODAL();

    //sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
    //AddEventQueue(&TmpEvent);

	l_fEnableRefresh = FALSE;


	TmpEvent = CurrentEvent;    

	sMakeEvent(&CurrentEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
	sFeed_EQE_APP_DRAW();	
	CurrentEvent = TmpEvent;
	l_fEnableRefresh = TRUE;

    sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
    AddEventQueue(&TmpEvent);
#if 0
	l_fEnableRefresh = TRUE;
	TmpEvent = CurrentEvent; 
	sMakeEvent(&CurrentEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
	sFeed_EQE_APP_DRAW();	
	CurrentEvent = TmpEvent;	
#endif
    return RESULT_OK;
}

EQS_RESULT EQS_SendEvent(EQS_EVENT_ID EventID, WORD wParam, DWORD dwParam)
{
    EQS_EVENT AnEvent;
    EQS_RESULT Result;

    Result = sMakeEvent(&AnEvent, EventID, wParam, dwParam);

    if (Result == RESULT_OK)
        return AddEventQueue(&AnEvent);
    else
        return Result;
}

EQS_EVENT_CATEGORY EQS_GetCategory(EQS_EVENT_ID EventID)
{
    return GetCategory(EventID);
}


EQS_RESULT EQS_PinUp(void)
{
    CurrentAppletState.fSubRoot = TRUE;
    return RESULT_OK;
}

EQS_APPLET *EQS_GetChild(void)
{
    return ChildAppletState.pApplet;
}

EQS_APPLET *EQS_GetParent(void)
{
    return ParentAppletState.pApplet;
}

EQS_APPLET *EQS_GetApplet(void)
{
    return CurrentAppletState.pApplet;
}

EQS_EVENT    EQS_GetEvent(void)
{
	return CurrentEvent;
}
EQS_APPLET* EQS_GetAncestor(UINT8 nTh_Ancestor)
{
	INT  nIndex;

	nIndex = uiAppletStackIndex - nTh_Ancestor;
	if( nIndex < 0 )
	{
		return pEQA_Root;
	}	
	return Applet_State_Stack[nIndex].pApplet;
}
//redstar   for eqs local timer
HTIMER     EQS_GetAppletTimerInfo(void)
{
    return CurrentAppletState.TimerHandle;
}

EQS_RESULT EQS_PassCurrentEvent(void)
{
    bPassEventFlag = TRUE;
    return RESULT_OK;
}

void EQS_API EQS_DefEventProc(EQS_EVENT* pInEvent)
{
    
#ifdef EXTENDED_EVENT_REGIME // NOTICE !!! 2000.12.10 girin EQE_BYPASS processing
    // Just do it ( or anything you can ... )
#endif

    switch (pInEvent->EventID)
    {
    case EQE_APP_DRAW:
        //ClearDisplay();
        break;

    case EQE_KEY_CLEAR:
//    case EQE_SOFTKEY_CANCEL:
        EQS_End();
        break; 

    case EQE_KEY_END:
        EQS_EndTo(EQS_POP_TO_ROOT);
        
        break;


#if 0
    case EQE_KEY_POWER_OFF:      /* power off */                          
        EQS_StartSibling(EQA_ShutDown, TRUE);
        break;

    case EQE_X_TOIDLE : /* very important */
    case EQE_IN_OFFLINE:
    case EQE_CM_INCOMING:
    case EQE_CM_ORIGINATED:
    case EQE_CM_ENDED:
    case EQE_CM_COMMAND_ERROR:
        EQS_EndToCurrentEvent(EQS_POP_TO_ROOT);
        break;
#endif
    default:
        break;
    }
}

EQS_RESULT EQS_DeleteControl(HCONTROL hControl)
{
    EQS_RESULT ret;

    if (IS_MODAL(hControl))
    {
        EQC_DeleteControl(hControl, TRUE);
        EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
        return RESULT_OK;
    }
    else if ((ret = FreeIOCIndex(hControl)) == RESULT_OK)
    {
        if (IsActive(hControl))
            EQS_EndIO();

        EQC_DeleteControl(hControl, TRUE);
        EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
    }

    return ret;
}

/*
EQS_RESULT EQS_SetFocus(UINT8 hControl)
{
    return SetFocus(hControl);
}

EQS_RESULT EQS_ClearFocus(UINT8 hControl)
{
    return ClearFocus(hControl);
}



EQS_RESULT EQS_SetFocusNext(void)
{
    BOOL fDone[MAX_CONTROL_PER_APPLET];
    BOOL fMatched;
    UINT8 i, start, last;
    UINT8 hControl, hControlMatch;

    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        fDone[i] = FALSE;
        if (CurrentAppletState.AttachedIOControls[i] == INVALID_CONTROL_HANDLE)
            break;
    }

    last = i;

    for (i = 0; i < last; i++)
    {
        hControl = CurrentAppletState.AttachedIOControls[i];

        if (g_CommonIOControlHeader[hControl].fFocus)
        {
            if (fDone[i] == FALSE)
            {
                fMatched = FALSE;
                for (start = MOD_INCREMENT(i, last); start != i; start = MOD_INCREMENT(start, last))
                {
                    hControlMatch = CurrentAppletState.AttachedIOControls[start];
                    if (FocusExclusive(hControl, hControlMatch))
                    {
                        if (fMatched)
                        {
                            g_CommonIOControlHeader[hControlMatch].fFocus = FALSE;
                        }
                        else
                        {
                            g_CommonIOControlHeader[hControl].fFocus = FALSE;
                            g_CommonIOControlHeader[hControlMatch].fFocus = TRUE;
                            fDone[start] = TRUE;
                            fMatched = TRUE;
                        }
                    }
                }
            }
        }
    }

    return RESULT_OK;
}
*/

//BOOL NX_PutIOCIndex(HCONTROL hControl) // This function should be called after NIOC_AllocCommonIOCHeader().
//BOOL EQS_API EQS_PutControlIndex(HCONTROL hControl) // This function should be called after NIOC_AllocCommonIOCHeader().
/* use only in EQC_AllocControl, eqc.c */
BOOL EQS_API EQS_AttachControl(HCONTROL hControl)
{
    UINT8 i;

    if (IS_MODAL(hControl))
    {
        if (GET_MODAL() != INVALID_CONTROL_HANDLE) // for safety code
            ASSERT(FALSE);

        SET_MODAL(hControl);
        return TRUE;
    }


    // find free index
    for (i = 0; i < MAX_CONTROL_PER_APPLET; i++)
    {
        // find free slot.
        if (CurrentAppletState.AttachedIOControls[i] == INVALID_CONTROL_HANDLE) // found.
        {
            if (i == 0) // The very first insert.
            {
                CurrentAppletState.AttachedIOControls[i] = hControl; // put.
                sSlideWindow.CurrentIOCIndex = 0; 
            }
            else
            {
                for ( ; i > 0 ; i--)
                {
                    if (CompareIOCPosition(CurrentAppletState.AttachedIOControls[i-1], hControl) > 0)
                    {
                        CurrentAppletState.AttachedIOControls[i] = CurrentAppletState.AttachedIOControls[i-1]; // shift position right.
                    }
                    else
                    {
                        CurrentAppletState.AttachedIOControls[i] = hControl; // put.
                        break; // out of for-loop.
                    }
                }

                if (i == 0)
                {
                    CurrentAppletState.AttachedIOControls[i] = hControl; // put on index 0.
                }

                if (i <= sSlideWindow.CurrentIOCIndex)
                {
					if(CurrentAppletState.AttachedIOControls[sSlideWindow.CurrentIOCIndex+1] != INVALID_CONTROL_HANDLE) 
					{ //2001.03.26 main state of focused control 
						sSlideWindow.CurrentIOCIndex++;
					}
					else
					{
						sSlideWindow.CurrentIOCIndex = i;
					}
                }
            }

            return TRUE;
        }
    }

    return FALSE; // free index not found.
}

BOOL EQS_HaveFocus(HCONTROL hControl)
{
    return HaveFocus(hControl);
}


BOOL EQS_IsActive(HCONTROL hControl)
{
    return IsActive(hControl);
}

HCONTROL EQS_GetModal(void)
{
    return GET_MODAL();
}

BOOL EQS_IsControlDataChanged(void)
{
    UINT8    nIndex, nControl;
	HCONTROL hControl;

	nControl = EQS_GetIOCNum();

	for( nIndex =0; nIndex < nControl; nIndex++ )
	{
		hControl = EQS_GetNthIOC(nIndex);
		if( EQC_GET_DIRTY(hControl) == TRUE )
			return TRUE;
	}
	return FALSE;	    
}

LOCAL BOOL  sIsActivateControl(HCONTROL hControl, EQS_EVENT_ID EventID)
{
	BOOL  fActive;
	if( hControl >= INVALID_CONTROL_HANDLE )
		return FALSE;

	if( EQC_GET_ENABLE(hControl) == FALSE )
		return FALSE;
		
	switch( EventID )
	{
	case EQE_KEY_UP:
		fActive = FALSE;
		break;
	case EQE_KEY_DOWN:
		fActive = FALSE;
		break;

	case EQE_KEY_LEFT:
		if( EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR  ||\
			EQC_GET_TYPE(hControl) == CT_SCROLL_LIST   ||\
			EQC_GET_TYPE(hControl) == CT_TEXT_EDIT     ||\
			EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT   ||\
			EQC_GET_TYPE(hControl) == CT_TIME)
		{
			fActive = TRUE;
		}
		else
		{
			fActive = FALSE;
		}
		break;
	case EQE_KEY_RIGHT:
		if( EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR ||\
			EQC_GET_TYPE(hControl) == CT_SCROLL_LIST  ||\
			EQC_GET_TYPE(hControl) == CT_TEXT_EDIT     ||\
			EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT   ||\
			EQC_GET_TYPE(hControl) == CT_TIME)
		{
			fActive = TRUE;
		}
		else
		{
			fActive = FALSE;
		}
		break;
	case EQE_KEY_CENTER:
		fActive = TRUE;
		break;
	case EQE_KEY_CLEAR:
		fActive = FALSE;
		break;
	case EQE_KEY_F1:
		fActive = FALSE;
		break;
	case EQE_KEY_F2:
		fActive = FALSE;
		break;
	default:
		if( EQC_GET_TYPE(hControl) == CT_TEXT_EDIT   ||\
			EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT ||\
			EQC_GET_TYPE(hControl) == CT_TIME          )
		{
			fActive = sDeserveEventByIOC(hControl, EventID );
		}
		else
		{
			fActive = FALSE;
		}

		break;
	}
	return fActive;
}
LOCAL void  ActivateControl(HCONTROL hControl, EQS_EVENT* pEvent)
{
    EQS_EVENT Event;

	if( hControl >= INVALID_CONTROL_HANDLE )
		return ;
	
	if( pEvent->EventID == EQE_KEY_CENTER )
	{
		sMakeEvent(&Event, EQE_APP_IO_MODE, hControl, EQS_NA);
	}
	else
	{
		sMakeEvent(&Event, EQE_APP_IO_MODE, hControl, pEvent->EventID);
	}

	if( EQC_GET_TYPE(hControl) == CT_SCROLL_LIST && \
		(pEvent->EventID == EQE_KEY_LEFT || pEvent->EventID == EQE_KEY_RIGHT) )
	{
		FeedEventIOControl(pEvent);
		pEvent->EventID  = EQE_KEY_CENTER;
		
	}
	else
	{
		SetWindowMode(WM_IO_MODE);
		AddEventQueue(&Event);
	}

	
	if ( ( EQC_GET_TYPE(hControl) == CT_ACTIVE_ICON ||\
		   EQC_GET_TYPE(hControl) == CT_PUSH_BUTTON ||\
		   EQC_GET_TYPE(hControl) == CT_CHECK_BOX     )&&\
		  pEvent->EventID == EQE_KEY_CENTER  )
	{
		FeedEventIOControl(pEvent); // Direct processing of EQE_KEY_CENTER is needed.
	}
	else if( ( EQC_GET_TYPE(hControl) == CT_TEXT_EDIT     ||\
			   EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT   ||\
			   EQC_GET_TYPE(hControl) == CT_TIME             ) &&\
			 ( pEvent->EventID == EQE_KEY_LEFT || pEvent->EventID == EQE_KEY_RIGHT )\
           )
	{
		pEvent->EventID = EQE_KEY_CENTER;
	}
	else ;


#ifdef FEED_DRAW_EVENT_SELECTIVELY
//sMakeEvent(&CurrentEvent, EQE_APP_DRAW, (WORD)GetFocusedIOC(), EQS_NA);
// Draw all entities not to show scrap of focus mark.
	//sMakeEvent(&Event, EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
	sMakeEvent(&Event, EQE_APP_DRAW, hControl, EQS_NA);
#else
	sMakeEvent(&Event, EQE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
#endif
	AddEventQueue(&Event);
	if( pEvent->EventID != EQE_KEY_CENTER )
	{
		AddEventQueue(pEvent);
	}
}

WINDOW_TSYMBOL_MODE EQS_API EQS_GetWindowSymbolMode(void)
{
	return l_WindoTitleSymbolMode;
}
void EQS_API EQS_SetWindowSymbolMode(WINDOW_TSYMBOL_MODE WindowSymbolMode)
{
	l_WindoTitleSymbolMode = WindowSymbolMode;
}


BOOL  EQS_StartModalIO(void)
{
	EQS_EVENT TmpEvent;

	TmpEvent = CurrentEvent;

	sMakeEvent(&CurrentEvent, EQE_APP_DRAW, GET_MODAL(), EQS_NA);
	//sFeed_EQE_APP_DRAW();	
	sFeed_EQE_APP_DRAW_OnAnControl();
	CurrentEvent = TmpEvent;

	return TRUE;
}


void EQS_SetSWindowPosition(INT16 nOffset)
{
	if( nOffset <= 0 )
	{
		sSlideWindow.yWindowOffset = 0;
	}
	else
	{
		sSlideWindow.yWindowOffset = nOffset;
		if( sSlideWindow.yWindowOffset + sSlideWindow.dyWindowSize > sSlideWindow.dyCanvasSize )
		{
			sSlideWindow.yWindowOffset = sSlideWindow.dyCanvasSize - sSlideWindow.dyWindowSize;
			if( sSlideWindow.yWindowOffset < 0 )
				sSlideWindow.yWindowOffset = 0;
		}
	}

	SetSlidingWindowOffsetY(sSlideWindow.yWindowOffset);
	EQS_SendEvent( EQE_APP_DRAW, EQS_NA, EQS_NA);
}

BOOL  EQS_PassIO(INT8 nDirect)
{
    EQS_EVENT TmpEvent;

    SetWindowMode(WM_APPLET_MODE);
    SetSlidingWindowState(TRUE);
	switch( nDirect )
	{
	case -1:
		sMakeEvent(&TmpEvent, EQE_APP_IO_END, EQS_GetFocusedIOC(), EQE_KEY_UP);
		break;
	case 1:
		sMakeEvent(&TmpEvent, EQE_APP_IO_END, EQS_GetFocusedIOC(), EQE_KEY_DOWN);
		break;
	case 0:	
		sMakeEvent(&TmpEvent, EQE_APP_IO_END, EQS_GetFocusedIOC(), EQE_KEY_CENTER);
		break;
	default:
		sMakeEvent(&TmpEvent, EQE_APP_IO_END, EQS_GetFocusedIOC(), EQS_NA);
		break;
	}	
	AddEventQueue(&TmpEvent);
    sMakeEvent(&TmpEvent, EQE_APP_DRAW, EQS_NA, EQS_NA);
    AddEventQueue(&TmpEvent);

	return TRUE;
}


EQS_DCL VOID EQS_API EQS_RefreshDisplayRect(HCONTROL hControl, UINT8 xleft, UINT8 ytop, UINT8 xright, UINT8 ybottom)
{
	RECT Rect;

	if( hControl == INVALID_CONTROL_HANDLE || GET_MODAL() != INVALID_CONTROL_HANDLE )
	{
		l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(),xleft,ytop,xright,ybottom);
	}
	else
	{
		sGetSWindowViewAreaRect(&Rect);
		l_pfnRefreshDisplayRect(EQS_GetScreenBuffer(),xleft,Rect.yTop+ytop-sSlideWindow.yWindowOffset,\
			                    xright,Rect.yTop+ybottom-sSlideWindow.yWindowOffset);
	}
}



void ScrollScreen(UINT8 nDestX, UINT8 nDestY, UINT8 nSourceX, UINT8 nSourceY, UINT8 nWidth, UINT8 nHeight)
{
	BYTE  *pSrc, *pDest;
	INT    i, j;
	INT    Index1, Index2;
	COORD  x, y;

	RECT Rect;
					
	sGetSWindowViewAreaRect(&Rect);

	nDestY   += Rect.yTop - sSlideWindow.yWindowOffset;
	nSourceY += Rect.yTop - sSlideWindow.yWindowOffset;

	pDest  = EQS_GetScreenBuffer() + nDestY*(DevGetMaxX()+1)*2 ;
	pSrc   = EQS_GetScreenBuffer() + nSourceY*(DevGetMaxX()+1)*2 ;

	if( nDestY > nSourceY )
	{ // scroll down	
		for(i = nHeight -1 ; i >=0; i--)
		{
			memcpy(pDest + i*(DevGetMaxX()+1)*2 + nDestX*2, pSrc + i*(DevGetMaxX()+1)*2 + nSourceX*2, nWidth*2);
		}
	}
	else
	{ // scroll up		
		for(i = 0; i < nHeight; i++)
		{
			memcpy(pDest + i*(DevGetMaxX()+1)*2 + nDestX*2, pSrc + i*(DevGetMaxX()+1)*2 + nSourceX*2, nWidth*2);
		}	

	}
}

EQS_DCL VOID EQS_API EQS_SetRefreshDisplaySetting(BOOL fEnable)
{
	l_fEnableRefresh = fEnable;
}
EQS_DCL BOOL EQS_API EQS_GetRefreshDisplaySetting(VOID)
{
	return l_fEnableRefresh;
}