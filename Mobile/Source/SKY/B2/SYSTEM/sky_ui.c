/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_ui.c
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
** 2000-10-30 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "srch.h"
#include "rxc.h"
#include "cm.h"
#include "uixuasms.h"

#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */

#include "sky.h"
#include "ska.h"

// happykid 00.12.28
#ifdef FEATURE_SMART_BATTERY

//#include "bq2018.h"
//#include "smb.h"
//#include "smbdev.h"

#endif
// happykid end

#ifdef FEATURE_WAP
#include "wapcommon.h"
#endif //#ifdef FEATURE_WAP

#ifdef WIN32
extern void EQS_RefreshDisplayDevice();  //redstar
extern void EQS_RefreshDisplayDeviceRect(BYTE* pBuffer, BYTE , BYTE, BYTE, BYTE );
#endif

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
DWORD g_dwNeedStop;
BOOL  g_fOK2Sleep ;

//DWORD  dwSleepFlag, dwSleepOldFlag;        /* flag for checking ok to sleep   */

#ifdef SW_RESET_ENABLE
BOOL    mc_bSwReset = FALSE;
DWORD   g_PseudoTimeOnSWReset = 0;
DWORD   g_savedTime;
#endif

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
#define sRESERVED_BYTE      35000 //39936
ROM BYTE l_abReservedForFuture[sRESERVED_BYTE] = {0, };


#ifndef FEATURE_EQS_ALLOCATED_SCREEN_BUFFER
//BYTE abMainScreenBuffer[(SKY_SCREEN_BUFFER_WIDTH/8)*SKY_SCREEN_BUFFER_HEIGHT*SKY_COLOR_SPACE];
WORD l_awMainScreenBuffer[(SKY_SCREEN_BUFFER_WIDTH/8)*SKY_SCREEN_BUFFER_HEIGHT*SKY_COLOR_SPACE/2];
BYTE l_abSubScreenBuffer[(SKY_SUB_SCREEN_BUFFER_WIDTH/8)*SKY_SUB_SCREEN_BUFFER_HEIGHT*SKY_SUB_COLOR_SPACE];
#endif /* #ifndef FEATURE_EQS_ALLOCATED_SCREEN_BUFFER */

/*
** UI Command Queue buffers for outside tasks to communicate to UI.
** We use ten because it seems reasonablly small, so far there's never been
** a problem with that number, and it "feels" right.
*/
#define UI_CMD_BUFFERS 50   //30 : original
LOCAL ui_cmd_type ui_buffers[ UI_CMD_BUFFERS ];

#ifdef FEATURE_WAP
q_type      ui_cmd_q;       /* queue for commands - ui_cmd does this */
#else
LOCAL q_type      ui_cmd_q;       /* queue for commands - ui_cmd does this */
#endif //#ifdef FEATURE_WAP
      q_type      ui_cmd_free_q;  /* free buffers for talking to UI */

#ifdef FEATURE_UASMS
#ifdef NEW_SMS
/*
** UI SMS Queue buffers for outside task to send SMS message to the UI.
** We use three because no good test could be generated to verify a smaller
** number of buffers. (ie. like two)
*/
#define UI_SMS_BUFFERS 6
LOCAL ui_sms_data_type ui_sms_buffers[ UI_SMS_BUFFERS ];
      q_type      ui_sms_free_q;  /* free buffers for sms messages */
#ifdef FEATURE_DS_UP
#error code not present
#endif
#endif
#endif /* FEATURE_UASMS */

/*
** Info buffers hold informational messages from the base station to be
** displayed on the handset.
*/
#define UI_INFO_BUFFERS 10
//LOCAL ui_info_type ui_info_buffers[ UI_INFO_BUFFERS ];
LOCAL INFO_MESSAGE_T ui_info_buffers[UI_INFO_BUFFERS];
LOCAL q_type      ui_info_q;       /* queue for info - ui_cmd does this */
      q_type      ui_info_free_q;  /* free buffers for talking to UI */

//이부분은 키톤 깨지는 코드를 넣기 위해서 바꾸었습니다.
//ska_strt.c에 그 코드가 있습니다.
//키톤문제는 MSM5000에만 있으니 다른 MSM을 쓰시는 분들은 원상복귀해 주세요.
//youandi 01.07.20
rex_timer_type  ui_rpt_timer;
//youandi 01.07.20

#ifdef FEATURE_VAS
rex_timer_type ui_ps_timer;
#endif//FEATURE_VAS

#ifdef FEATURE_WAP
rex_timer_type  bar_ani_timer;			/* Timer for animaton */
rex_timer_type  wap_animation_timer;    /* Timer for SIS animation   */
rex_timer_type  wap_nowrap_timer;       /* Timer for Nowrap          */
rex_timer_type  wap_browser_timer;
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_WAVELET
extern  rex_timer_type  wavelet_alive_timer;
#endif // FEATURE_WAVELET

#ifdef FEATURE_DS_UP
#error code not present
#endif

#ifdef FEATURE_SKVM
	#ifdef SUPPORT_REX_TIMER_IN_SKVM
		extern rex_timer_type xvm_sig_timer1;
		extern rex_timer_type xvm_sig_timer2;
		extern rex_timer_type xvm_sig_timer3;
	#endif //SUPPORT_REX_TIMER_IN_SKVM
#endif // FEATURE_SKVM
/*
** Keep track of key events to pass on to the state machine.
** It's a standard circular queue.  16 gives room for 8 keys and releases.
*/

#define MAX_EVENT_STACK_SIZE                32
#define UI_EVENT_COUNT_TO_WARN              3

struct tagEVENT_STACK_T
{
    INT8        nHead;
    INT8        nTail;
    SKY_EVENT_E Buffer[MAX_EVENT_STACK_SIZE];
};
typedef struct tagEVENT_STACK_T        EVENT_STACK_T;

LOCAL EVENT_STACK_T   l_KeyEventStack;          /* keyboard buffer */
LOCAL EVENT_STACK_T   l_PriorityEventStack;     /* all other buffer */

LOCAL BOOL            l_fGetKeys = FALSE;               /* getkeys */

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

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


/*===========================================================================
FUNCTION UI_ADD_EVENT_AT_TAIL

DESCRIPTION
  Push an event onto the TAIL of the event queue, so it will
  be the next event to be processed.


===========================================================================*/
void SKY_API SKY_AddEventAtTail(SKY_EVENT_E Event)
{
    INT8 nTemp;

    nTemp = (l_KeyEventStack.nTail != 0 
             ? l_KeyEventStack.nTail-1 : MAX_EVENT_STACK_SIZE-1);
             
    if(nTemp != l_KeyEventStack.nHead)
    {
        l_KeyEventStack.Buffer[l_KeyEventStack.nTail] = Event;
        l_KeyEventStack.nTail = nTemp;
    }
}

/*===========================================================================
FUNCTION UI_ADD_EVENT

DESCRIPTION
  Push an event onto the event queue.


===========================================================================*/
void SKY_API SKY_AddEvent(SKY_EVENT_E Event)
{
#if 0
    if(((l_KeyEventStack.nHead + UI_EVENT_COUNT_TO_WARN) 
         % MAX_EVENT_STACK_SIZE) == l_KeyEventStack.nTail)
    {
        SKY_ERROR("UI event queue is almost full");
    }
#endif
    
    if(((l_KeyEventStack.nHead+1) % MAX_EVENT_STACK_SIZE) 
       != l_KeyEventStack.nTail)
    {				
        l_KeyEventStack.nHead = (l_KeyEventStack.nHead+1) % MAX_EVENT_STACK_SIZE;		
		ASSERT( l_KeyEventStack.nHead  < MAX_EVENT_STACK_SIZE );
        l_KeyEventStack.Buffer[l_KeyEventStack.nHead] = Event;
    }
}

/* ==========================================================================
  jrkwon : 2001.06.19

  FUNCTION SKY_ClearEvent

  DESCRIPTION
     initialize keyboard buffer

===========================================================================*/
void SKY_API SKY_ClearAllEvent(void)
{
   /* initialize our keyboard buffer */
    l_KeyEventStack.nTail = l_KeyEventStack.nHead = 0;
}

/*===========================================================================
FUNCTION UI_ADD_PEVENT

DESCRIPTION
  Push an event onto the priority event queue.


===========================================================================*/
void SKY_API SKY_AddPriorityEvent(SKY_EVENT_E Event)
{
#if 0
    if(((l_PriorityEventStack.nHead + UI_EVENT_COUNT_TO_WARN) 
         % MAX_EVENT_STACK_SIZE) == l_PriorityEventStack.nTail)
    {
        SKY_ERROR("UI priority event queue is almost full");
    }
#endif
    
    if(((l_PriorityEventStack.nHead+1) % MAX_EVENT_STACK_SIZE) 
       != l_PriorityEventStack.nTail)
    {
        l_PriorityEventStack.nHead 
                = (l_PriorityEventStack.nHead+1) % MAX_EVENT_STACK_SIZE;
		ASSERT( l_PriorityEventStack.nHead < MAX_EVENT_STACK_SIZE );
        l_PriorityEventStack.Buffer[l_PriorityEventStack.nHead] = Event;
    }
}

/*===========================================================================
FUNCTION UI_INIT

DESCRIPTION
  Initialize the user interface - called immediately after UI_TASK is
  started.  Initialize screen handler, queues, timers, etc.

RETURN VALUE
  None

DEPENDENCIES
  Called only once from UI_INIT
===========================================================================*/

#define TOTAL_MEM_POOL_SIZE         (68*1024)
#define MAX_MEM_POOL_SIZE           (TOTAL_MEM_POOL_SIZE-MAX_EQUS_MEM_POOL_SIZE)

LOCAL BYTE l_aSKYMemPool[MAX_MEM_POOL_SIZE];

BOOL SKY_API SKY_ReinitMemory(void)
{
    if (SKY_InitMemory(l_aSKYMemPool, MAX_MEM_POOL_SIZE) != 0)
    {
        return FALSE;
    }
	return TRUE;
}


STATIC SKY_API void sInitialize( void )
{
    SCREEN_INFO MainScreenInfo = {
        SKY_COLOR_SPACE,
        { SKY_SCREEN_WIDTH,        SKY_SCREEN_HEIGHT },
        { SKY_SCREEN_BUFFER_WIDTH, SKY_SCREEN_BUFFER_HEIGHT }
    };
    SCREEN_INFO SubScreenInfo = {
        SKY_SUB_COLOR_SPACE,
        { SKY_SUB_SCREEN_WIDTH, SKY_SUB_SCREEN_HEIGHT },
        { SKY_SUB_SCREEN_BUFFER_WIDTH, SKY_SUB_SCREEN_BUFFER_HEIGHT }
    };
    SCREEN MainScreen;
    SCREEN SubScreen;
    int i; 

    if (SKY_InitMemory(l_aSKYMemPool, MAX_MEM_POOL_SIZE) != 0)
    {
        SKY_ASSERT(0); // RESULT_OUT_OF_MEMORY; // Catastrophy !!!
    }

    /* ---------------------
    ** LCD 화면 정보 설정 
    */
    MainScreen.Info     = MainScreenInfo;
    SubScreen.Info      = SubScreenInfo;
#ifndef FEATURE_EQS_ALLOCATED_SCREEN_BUFFER
    MainScreen.pbBuffer = (BYTE*)l_awMainScreenBuffer;
    SubScreen.pbBuffer  = l_abSubScreenBuffer;
#endif /* #ifndef FEATURE_EQS_ALLOCATED_SCREEN_BUFFER */

    /* 환경 정보 파일 초기화 */
    //SKY_InitConfig();
    SKY_InitSystemState();
    SKY_InitCallState();

    l_fGetKeys = FALSE;        /* State Machine doesn't want keys yet */

    /* initialize screen */
    //SKY_InitDisplay(&ScreenInfo);    
    SKY_InitScreen(&MainScreen, &SubScreen);

    /* initialize sound */
    SKY_InitSound();              /* packets for talking to sound task */

    /* Initialize the command queues and buffers for communication with ui */
    (void) q_init( &ui_cmd_q );
    (void) q_init( &ui_cmd_free_q );
    for( i=0; i<UI_CMD_BUFFERS; i++ ) 
    {
        ui_buffers[i].hdr.done_q_ptr = &ui_cmd_free_q;
        q_put( &ui_cmd_free_q, q_link( &ui_buffers[i], 
               &ui_buffers[i].hdr.link ));
    }

#ifdef FEATURE_UASMS
#ifdef NEW_SMS
    /* new sms buffer system define by NEW_SMS. Used only with uin* files */
    (void) q_init(&ui_sms_free_q);
    for (i=0; i<UI_SMS_BUFFERS;i++) 
    {
        q_put (&ui_sms_free_q, q_link( &ui_sms_buffers[i], 
               &ui_sms_buffers[i].link));
    }
#ifdef FEATURE_DS_UP
#error code not present
#endif
#endif
#endif /* FEATURE_UASMS */

    /* Initialize the informational display queues and buffers */
    (void) q_init( &ui_info_q );
    (void) q_init( &ui_info_free_q );
    for( i=0; i<UI_INFO_BUFFERS; i++ ) 
    {
        q_put( &ui_info_free_q, q_link( &ui_info_buffers[i], 
               &ui_info_buffers[i].Link ));
    }  

    /* Initialize timers */
    rex_def_timer( &ui_rpt_timer, &ui_tcb, UI_RPT_TIMER_SIG );
#ifdef FEATURE_VAS
    rex_def_timer( &ui_ps_timer, &ui_tcb, UI_PS_TIMER_SIG );
#endif//feature_vas

#ifdef FEATURE_WAP
    rex_def_timer( &bar_ani_timer, &ui_tcb, BAR_ANI_SIG );
    rex_def_timer( &wap_animation_timer, &ui_tcb, WAP_ANIMATION_TIMER_SIG );
    rex_def_timer( &wap_nowrap_timer,  &ui_tcb, WAP_NOWRAP_TIMER_SIG );
    rex_def_timer( &wap_browser_timer, &ui_tcb, WAP_BROWSER_TIMER_SIG );
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_WAVELET
    rex_def_timer( &wavelet_alive_timer, &ui_tcb, WAVELET_ALIVE_TIMER_SIG );
#endif //FEATURE_WAVELET

#ifdef FEATURE_SKVM
	#ifdef SUPPORT_REX_TIMER_IN_SKVM
	rex_def_timer( &xvm_sig_timer1, &ui_tcb, XVM_ACTIVE_TIMER1_SIG);
	rex_def_timer( &xvm_sig_timer2, &ui_tcb, XVM_ACTIVE_TIMER2_SIG);
	rex_def_timer( &xvm_sig_timer3, &ui_tcb, XVM_ACTIVE_TIMER3_SIG);
	#endif
#endif

#ifdef FEATURE_DS_UP
#error code not present
#endif

    mc_task_start( UI_RPT_TIMER_SIG, DOG_UI_RPT, &ui_rpt_timer );
   
    SKY_Clear_Signal( rex_self(), TASK_START_SIG );
    //(void) rex_clr_sigs( rex_self(), TASK_START_SIG );

    /* Set the watchdog report timer signal so that we'll
    ** reset the watchdog as soon as we start checking for events
    ** and start our first tick timer
    */
    
    //(void) SKY_Set_Signal( &ui_tcb, UI_RPT_TIMER_SIG );
    SKY_Set_Signal( &ui_tcb, UI_RPT_TIMER_SIG);

    /* initialize our keyboard buffer */
    l_KeyEventStack.nTail = l_KeyEventStack.nHead = 0;
  
    SKY_InitCallbackTimer();
    
    g_dwNeedStop = 0;

    /* tell the handset task that we want key signals to come to us */
#ifdef FEATURE_BSPUI_API  
    #error code not present
#else
    SKY_InitKey( rex_self(), UI_KEY_SIG );
#endif /* FEATURE_BSPUI_API */

    /* Assume that power and ignition are off to begin with.  HS will */
    /* correct any misassumptions immediately */
    SKY_SET_POWER_ON(FALSE);
    SKY_SET_EXTERNAL_POWER(FALSE);
    SKY_SET_CHARGING(FALSE);
    SKY_SET_PHONE_CLOSE();
    SKY_SET_HANDS_FREE(FALSE);
#ifdef FEATURE_OTASP
    SKY_SET_OTASP_FAILED(TRUE);   /* indicates if an otasp call is failed */
#endif
    SKY_SET_SHUT_DOWN(FALSE);
    g_fOK2Sleep = FALSE;
//    dwSleepFlag = 0;   /* ok to sleep */
//    dwSleepOldFlag = ~dwSleepFlag;
    UI_ENABLE_SLEEP();

    //SKY_SetupBacklight();
    SKY_SNI(HS_SNI_POWER_ON, TRUE);
    
#ifdef FEATURE_UI_KEYGUARD
#error code not present
#endif

#ifdef FEATURE_ANALOG_HFK
#error code not present
#endif /* FEATURE_ANALOG_HFK */

    SKY_InitCallManager();

#if defined(FEATURE_UASMS) && !defined(WIN32)
    uiuasms_init();//    SKY_InitSMS();
#endif /* FEATURE_UASMS */

// happykid 00.12.28
#ifdef FEATURE_SMART_BATTERY
//  smb_init(); // 01.03.30 youandi 스마트배터리초기회 시키는 곳을 ui에서 mc로 옮김......
#endif 

#ifndef WIN32
    EQS_Initialize( (EQS_APPLET *)SKA_MainOff, 
                                 (EQS_APPLET *)SKA_MainIdle, 
//                                 &ScreenInfo, 
                                 SKY_DrawAnnunciators,
                                 &SKY_CML,
                                 g_pfnEQS_Dispatcher,
                                 (EQS_APPLET *)SKY_DefEventProc,
                                 g_pfnSKY_SetTimer,
                                 clk_uptime_ms,
                                 g_aStringTable,
                                 SKY_RefreshDisplay,
                                 SKY_RefreshDisplayRect,
								 SKY_SetPaceMakerTimer);
#else
    EQS_Initialize( (EQS_APPLET *)SKA_MainOff, 
                                 (EQS_APPLET *)SKA_MainIdle, 
//                                 &ScreenInfo, 
                                 SKY_DrawAnnunciators,
                                 &SKY_CML,
                                 g_pfnEQS_Dispatcher,
                                 (EQS_APPLET *)SKY_DefEventProc,
                                 g_pfnSKY_SetTimer,
                                 clk_uptime_ms,
                                 g_aStringTable,
                                 EQS_RefreshDisplayDevice,
								 EQS_RefreshDisplayDeviceRect,
								 SKY_SetPaceMakerTimer);

#endif

} /* end of ui_init */


/*===========================================================================
FUNCTION UI_TASK

DESCRIPTION
  The main user interface function.  This is the big loop outside of the
  state machine which handles signals and all sorts of inputs.  It mostly
  serves to convert inputs into events for the state machine, but it
  also handles keypad incidentals - backlighting and key beeps.

DEPENDENCIES
  REX is running.  NV and DIAG and HS are running.

RETURN VALUE
  Never exits.

SIDE EFFECTS
  Plays with variable ui.

===========================================================================*/

void ui_task(dword dummy)  /*ignore fact that this isn't used */
{
    STATIC REX_SIGNAL Signal;      /* hold signals */

    sInitialize();                   /* initialize queues, timers, etc. */
  
    /* Wait on REX signals, repeat forever */
    while( TRUE )
    {
        Signal = SKY_Wait_Signal(    
                        UI_RPT_TIMER_SIG | UI_KEY_SIG | UI_CMD_Q_SIG |
                        UI_MULTI_STOP_SIG | UI_TIMERS_SIG |    
                        UI_RING_SIG | TASK_OFFLINE_SIG | TASK_STOP_SIG| UI_PLAY_REC_SIG |
						UI_RECVDATA_Q_SIG
#ifdef FEATURE_SKT_EIF	//yjahn96 01.03.15
						| UI_UPONG_SIG
#endif	//#ifdef FEATURE_SKT_EIF
#ifdef FEATURE_IRDA_SIGNAL
                        | UI_IRDA_RX_SIG
#endif
#ifdef FEATURE_VAS
						| UI_NETWORK_CB_SIG | UI_SOCKET_CB_SIG 
						| UI_DNS_CB_SIG | UI_PS_TIMER_SIG
#endif//feature_vas
#ifdef FEATURE_SKVM
						| XVM_TIMERS_SIG | XVM_TIMERS2_SIG
#ifdef SUPPORT_REX_TIMER_IN_SKVM
						| XVM_ACTIVE_TIMER1_SIG 
						| XVM_ACTIVE_TIMER2_SIG
						| XVM_ACTIVE_TIMER3_SIG
#endif //SUPPORT_REX_TIMER_IN_SKVM
#endif
#ifdef FEATURE_WAP
                        | WAP_CMD_Q_SIG | UI_EVT_PROCESS_SIG
                        | WAP_ANIMATION_TIMER_SIG | WAP_NOWRAP_TIMER_SIG
						| WAP_BROWSER_TIMER_SIG
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_WAVELET
						| WAVELET_ALIVE_TIMER_SIG
#endif //FEATURE_WAVELET
                        );


        SKY_HandleSignal( Signal, &ui_cmd_q );
    
        /* Informational message came in from the base station.  Let the */
        /* state machine know to display it if necessary                 */
        if( q_cnt( &ui_info_q ) != 0 )
        {        
            SKY_DoEvent( SE_SYS_INFO );
        }    
    
        /* process all waiting priority events */
        while ( l_PriorityEventStack.nHead != l_PriorityEventStack.nTail ) 
        {
            l_PriorityEventStack.nTail = 
                    (l_PriorityEventStack.nTail+1) % MAX_EVENT_STACK_SIZE;
			ASSERT( l_PriorityEventStack.nTail < MAX_EVENT_STACK_SIZE );
            SKY_DoEvent(
                l_PriorityEventStack.Buffer[l_PriorityEventStack.nTail]);
        }

        /* If the state machine is accepting keys, process all the key */
        /* events that we have queued up                               */
        while(l_fGetKeys && (l_KeyEventStack.nHead != l_KeyEventStack.nTail) ) 
        {  

            l_KeyEventStack.nTail 
                    = (l_KeyEventStack.nTail+1 ) % MAX_EVENT_STACK_SIZE;
			ASSERT( l_KeyEventStack.nTail  < MAX_EVENT_STACK_SIZE );
            //if(l_fGetKeys)
            SKY_DoEvent(l_KeyEventStack.Buffer[l_KeyEventStack.nTail]);
			SKY_KickDog();
        }
//        if( dwSleepOldFlag != dwSleepFlag )    //redstar 2001.02.16
//        {          
//            if ( dwSleepOldFlag ==0 ||  dwSleepFlag ==0 ) 
//            {            
//                if( (dwSleepOldFlag = dwSleepFlag ) == 0 ) 
//                {   /* okay to sleep */              
//                    UI_ENABLE_SLEEP();
//                }
//                else 
//                {
//                    UI_DISABLE_SLEEP();              
//                }
//            }
//        } 
    }
     
}

#ifdef FEATURE_VAS
/*===========================================================================

LOCAL FUNCTION APP_NETWORK/SOCKET_CALLBACK

DESCRIPTION
  This procedure set the signals relating to call back

DEPENDENCIES
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void app_network_callback(void *p)         /* network callback function */
{
//	MSG_HIGH("net_callback",0,0,0);
    (void) rex_set_sigs(&ui_tcb, UI_NETWORK_CB_SIG);
}

void app_socket_callback(void *p)         /* socket callback function */
{
    (void) rex_set_sigs(&ui_tcb, UI_SOCKET_CB_SIG);
}

void app_dns_callback(void)
{
	(void) rex_set_sigs(&ui_tcb, UI_DNS_CB_SIG);
}


REX_SIGNAL ui_wait(REX_SIGNAL wait_sigs)
{
     REX_SIGNAL Signal;
     REX_SIGNAL queue_mask = 0;
/*
	if( (wait_sigs & UI_CMD_Q_SIG) != 0)
	{
		(void) SKY_Clear_Signal( &ui_tcb, UI_CMD_Q_SIG );
		
		if( q_cnt(&ui_cmd_q) == 0)
		{
			queue_mask |= UI_CMD_Q_SIG ;
		}
	}
*/
	Signal = rex_get_sigs(&ui_tcb);

	do
	{
		if( ((Signal | queue_mask) & wait_sigs) == 0)
		{
			Signal = SKY_Wait_VASSignal(wait_sigs | UI_RPT_TIMER_SIG) ;		
		}

			if ( Signal & UI_RPT_TIMER_SIG ) 
			{
				dog_report(DOG_UI_RPT);
				(void) SKY_Set_Timer( &ui_rpt_timer, DOG_UI_RPT_TIME );
			}

			/* Handle the nework signal, acknowledge and continue */
			if ( (Signal & UI_NETWORK_CB_SIG) != 0 ) 
			{
			  (void) SKY_Clear_Signal( &ui_tcb, UI_NETWORK_CB_SIG); /* Clear the signal */
			}
			/* Handle the socket signal, acknowledge and continue */
			if ( (Signal & UI_SOCKET_CB_SIG) != 0 ) 
			{
			  (void) SKY_Clear_Signal( &ui_tcb, UI_SOCKET_CB_SIG); /* Clear the signal */
			}	
			/* Handle the dns signal, acknowledge and continue */
			if ( (Signal & UI_DNS_CB_SIG) != 0 ) 
			{
			  (void) SKY_Clear_Signal( &ui_tcb, UI_DNS_CB_SIG); /* Clear the signal */
			}	

			if ( (Signal & UI_PS_TIMER_SIG) != 0 ) 
			{
			  (void) SKY_Clear_Timer( &ui_ps_timer);
			  (void) SKY_Clear_Signal( &ui_tcb, UI_PS_TIMER_SIG); /* Clear the signal */
			}
/*			
			if ( (Signal & UI_CMD_Q_SIG) != 0 ) 
			{
			    (void) SKY_Clear_Signal( &ui_tcb, UI_CMD_Q_SIG); 
				if( q_cnt(&ui_cmd_q) == 0)
				{
					Signal &= ~UI_CMD_Q_SIG ;
				}
			}
*/
	} while ( ((Signal | queue_mask) & wait_sigs) == 0 );

	return ((Signal | queue_mask));
}

static void nop2(void)
{
	return;
}

boolean ui_wait_sig(long int timer_cnt, REX_SIGNAL wait_sig)
{

    STATIC REX_SIGNAL Signal;
    
    (void) SKY_Clear_Signal(&ui_tcb, wait_sig);

	(void) SKY_Set_Timer( &ui_ps_timer, timer_cnt );

	if( (Signal = ui_wait(wait_sig) ) != 0)
	{
			(void) SKY_Clear_Timer( &ui_ps_timer );
		    (void) SKY_Clear_Signal( &ui_tcb, UI_PS_TIMER_SIG); /* Clear the signal */

			/* Handle the nework signal, acknowledge and continue */
			if ( (Signal & UI_NETWORK_CB_SIG) != 0 ) 
			{
			   MSG_ERROR("UI_NETWORK_CB_SIG",0,0,0);
			   nop2();
			  return TRUE;
			}
			/* Handle the socket signal, acknowledge and continue */
			if ( (Signal & UI_SOCKET_CB_SIG) != 0 ) 
			{
			   MSG_ERROR("UI_SOCKET_CB_SIG",0,0,0);
			   nop2();
			  return TRUE;
			}	
			/* Handle the dns signal, acknowledge and continue */
			if ( (Signal & UI_DNS_CB_SIG) != 0 ) 
			{
			   MSG_ERROR("UI_DNS_CB_SIG",0,0,0);
			   nop2();
			  return TRUE;
			}	

			if ( (Signal & UI_PS_TIMER_SIG) != 0 ) 
			{
			   MSG_ERROR("UI_PS_TIMER_SIG",0,0,0);
			   nop2();
			  return FALSE;
			}
/*			
			if ( (Signal & UI_CMD_Q_SIG) != 0 ) 
			{
				MSG_ERROR("UI_CMD_Q_SIG",0,0,0);
				// You have to process this command.
//			   (void) SKY_Clear_Signal( &ui_tcb, UI_CMD_Q_SIG); 
			   nop2();
			   return FALSE;
			}		
*/
	}//if
	MSG_ERROR("Unwanted Signal [%d]", Signal,0,0);
	return FALSE;
}

void send_ui_cmd
(
  ui_name_type cmd                            /* holds the name of the cmd */
)
{
  ui_cmd_type   *ui_cmd_ptr;                        /* to hold the UI cmd. */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MSG_HIGH("Sending UI cmd %d", cmd, 0, 0);

  /*-------------------------------------------------------------------------
    Check if there are free buffers on the UI command queue.
  -------------------------------------------------------------------------*/
  if(( ui_cmd_ptr = (ui_cmd_type *)q_get( &ui_cmd_free_q)) == NULL)
  {
    ERR_FATAL( "No UI cmd buffers",0,0,0);
  }

  /*-------------------------------------------------------------------------
    Make up the command to the UI.
  -------------------------------------------------------------------------*/
  ui_cmd_ptr->hdr.cmd        = cmd;
  ui_cmd_ptr->hdr.task_ptr   = NULL;
  ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd( ui_cmd_ptr);                    /* Queue the cmd to the UI layer */

}  /* send_uicmd() */
//FEATURE_WAP
void send_ui_cmd_id
(
  ui_name_type  cmd,                            /* holds the name of the cmd */
  unsigned char objID,
  unsigned char chID,
  unsigned char value
)
{
  ui_cmd_type   *ui_cmd_ptr;                        /* to hold the UI cmd. */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MSG_HIGH("Sending UI cmd %d", cmd, 0, 0);

  /*-------------------------------------------------------------------------
    Check if there are free buffers on the UI command queue.
  -------------------------------------------------------------------------*/
  if(( ui_cmd_ptr = (ui_cmd_type *)q_get( &ui_cmd_free_q)) == NULL)
  {
    ERR_FATAL( "No UI cmd buffers",0,0,0);
  }

  /*-------------------------------------------------------------------------
    Make up the command to the UI.
  -------------------------------------------------------------------------*/
  ui_cmd_ptr->Command.hdr.cmd        = cmd;
  ui_cmd_ptr->Command.hdr.task_ptr   = NULL;
  ui_cmd_ptr->Command.hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd_ptr->Command.objectID  = objID;
  ui_cmd_ptr->Command.channelID = chID;
  ui_cmd_ptr->Command.value     = value;

  ui_cmd( ui_cmd_ptr);                    /* Queue the cmd to the UI layer */

}  /* send_ui_cmd_id() */

#endif//feature_vas

/*===========================================================================

FUNCTION UI_CMD

DESCRIPTION
  Allows other tasks to queue up messages to the UI.  Grab a buffer from the
  ui_cmd_free_q, fill it in, and call ui_cmd with it.  All this will
  do is queue it and set a signal for the UI task.  When the command
  is done, the buffer will be queued as directed in the command header,
  and a task you specify may be signaled.

DEPENDENCIES
  ui_cmd_q already initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/

void ui_cmd (  ui_cmd_type *cmd_ptr  )          /* command block to UI */
{
    EQS_TRACE1(TL_HIGH, "UI_CMD %d", cmd_ptr->hdr.cmd);
    
    (void) q_link( cmd_ptr, &cmd_ptr->hdr.link );  /* init link */  
    q_put( &ui_cmd_q, &cmd_ptr->hdr.link );        /* and queue it */  
    (void) SKY_Set_Signal( &ui_tcb, UI_CMD_Q_SIG );  /* signal the UI task */
}


/*===========================================================================
FUNCTION UI_KICK_DOG

DESCRIPTION
  This function reports to dog task that UI is alive and kicking!

RETURN VALUE
  None

DEPENDENCIES
  Dog Task is running.
===========================================================================*/
void SKY_API SKY_KickDog( void )
{
   (void) SKY_Set_Timer( &ui_rpt_timer, DOG_UI_RPT_TIME );
    dog_report( DOG_UI_RPT );
} /* End of ui_kick_dog */

/* 
** convert a SKY event to an EQS event. 
** feed event to the state machine engine.
*/

void SKY_API SKY_DoEvent(SKY_EVENT_E SkyEventID)
{
    EQS_EVENT Event;

    STATIC UINT8 nSmartBatteryCheckCount = 0;
    STATIC BOOL fEven = TRUE;

    /* Pre-Processing .Start */ // girin. 2000.12.12
    // Please make the if-clauses short for better performance hit.
    if (SkyEventID == SE_SYS_PACEMAKER)
    {
        /* 
        ** jrkwon 2001.07.12
        ** RSSI가 너무 자주 바뀐다는 얘기가 있어서, 2초에 한번만 그리도록 수정
        */
        if(fEven)
        {
            fEven = FALSE;

            // woodstok 01.6.14
            // UI_PH_RSSI_F 나 UI_PH_SRV_STATE_F 받았을때만 rssi를 갱신해서인지,
            // RSSI Update속도가 늦은것 같아서 주기적으로 체크하도록 추가
            if (SKY_SetThenCompareRSSI(SKY_GET_PHONE_INFO_RSSI()))
            {
                EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);

    #ifdef FEATURE_SKY_SUBLCD
    #ifdef WAPPER
                if(SKY_IS_IN_WAP())
                    SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
                else
    #endif
                SKY_DrawSubAnnunciators();
    #endif
            }

        }
        else
        {
            fEven = TRUE;
        }

        if (SKY_SetThenCompareBattLevel() == TRUE) // if batt. level is changed
        {
            // Don't do SKY_DoEvent() to decrease execution overhead.
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, 0); 

#ifdef FEATURE_SKY_SUBLCD
#ifdef WAPPER
            if(SKY_IS_IN_WAP())
                SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
            else
#endif
            SKY_DrawSubAnnunciators();
#endif
        }

        // Smart battery check per every minute.
        if ((nSmartBatteryCheckCount % 60) == 0)
        {
            nSmartBatteryCheckCount = 0;
#ifdef FEATURE_SMART_BATTERY
            smb_check_every_oneminute(SKY_IS_EXTERNAL_POWER());
#endif//#ifdef FEATURE_SMART_BATTERY
        }
        nSmartBatteryCheckCount++;
    }
    /* Pre-Processing .End */

    /* if the SkyEventID is not an EQS native event */
    if(SKY_IS_EQS_NATIVE_EVENT((EQS_EVENT_ID)SkyEventID))
    {
        Event.EventID = (EQS_EVENT_ID)SkyEventID;
        Event.wParam = (WORD)0;
        Event.dwParam = (DWORD)0;
    }
    else
    {
        /* set the EQS event as bypass event */
        Event.EventID = EQE_BYPASS;
        /* set the SKY event into dwParam of EQS event */
        Event.wParam = (WORD)0;
        Event.dwParam = (DWORD)0;
        Event.dwExtEventID = (DWORD)SkyEventID;
    }
    /* feed event to the state machine engine */

    EQS_Scheduler(&Event);

}

/*===========================================================================
FUNCTION UI_GET_LOCAL_TIME

DESCRIPTION
  Calculate local time based on the system time, leap seconds, local time
    offset and daylight savings time.

    According to IS95A -
    7.7.1.3 Sync Channel Message: Local time = SYS_TIME - LP_SEC + LTM_OFF

    Note:  DAYLT (Daylight Savings Time) is provided but is already
      factored into the LTM_OFF so should not be used as part of the
      local time calculation.  It is provided in case the mobile
      wishes to display PST vs PDT.

    If the time returned by clk_read_secs is invalid, the invalid time
    is returned without any local time offset.

    TEMPORARY change, until deep sleep timing bug fixed.
    TEMPORARILY, we will also check to make sure system time is valid
    based on whether we've acquired CDMA service and never deep slept.

DEPENDENCIES
  ui.systime_valid

RETURNS
  local time in seconds
===========================================================================*/
DWORD SKY_API SKY_GetLocalTime(void)
{
    DWORD dwSysTime;
    UINT8 nLocalTimeOffset;
#ifdef SW_RESET_ENABLE    
    BOOL  f_SetCurTime;
    static BOOL g_UseSWResetTime = FALSE;
    static DWORD SwResetBaseTime = 0;
  	static word	Count = 0;
#endif
    
    dwSysTime = clk_read_secs();

#ifdef SW_RESET_ENABLE 
    f_SetCurTime = TRUE;

  if(mc_bSwReset)
  {
    f_SetCurTime = FALSE;
	if(Count == 0)
	{
		dwSysTime = g_savedTime;
		Count++;
	}
	else
	{
		if(SKY_IS_VALID_LOCAL_TIME(dwSysTime))  /* if time is available */
		{
			mc_bSwReset = 0;
            f_SetCurTime = TRUE;
		}
        else
        {
            Count++;

		    if(Count > 15)
		    {
                mc_bSwReset = 0;

                g_PseudoTimeOnSWReset = SwResetBaseTime = g_savedTime;
                g_UseSWResetTime = TRUE;
		    }
            else
                dwSysTime = g_savedTime;
        }
	}
  }

#endif

    /* TEMPORARY change, until deep sleep timing bug fixed. */
    /* Now make sure the system time is really valid.  It may have */
    /* started to drift due to deep sleeping.                      */
#ifdef SW_RESET_ENABLE
  if(f_SetCurTime && !mc_bSwReset)
  {
#endif
    if(SKY_IS_VALID_LOCAL_TIME(dwSysTime))
    {
        DB_ITEM_VALUE_U dbi;

        db_get( DB_LP_SEC, &dbi );  /* leap seconds */
        dwSysTime -= (DWORD)dbi.lp_sec;

        db_get( DB_LTM_OFF, &dbi ); /* time zone */
        nLocalTimeOffset = (int)dbi.ltm_off;
        /* ltm_off is a 6-bit 2-complement code, make it signed integer */
        nLocalTimeOffset &= 0x3f;
        if (nLocalTimeOffset > 32) 
        {
            nLocalTimeOffset -= 64;
        }
        dwSysTime += (60 * 30 * nLocalTimeOffset);

#ifdef SW_RESET_ENABLE
        if (g_UseSWResetTime)
        {
            g_savedTime = g_PseudoTimeOnSWReset;
        }
        else
        {
            g_savedTime = dwSysTime;
        }

        g_UseSWResetTime = FALSE;
#endif
    }
    /* TEMPORARY change, until deep sleep timing bug fixed. */
    /* Even though the system time appears valid, it's actually drifting */
    else
    {
        dwSysTime = SKY_INVALID_SYS_TIME;
    }
#ifdef SW_RESET_ENABLE
  }

  if (!SKY_IS_VALID_LOCAL_TIME(dwSysTime))
  {
      if (g_UseSWResetTime == TRUE)
      {
          dwSysTime = g_PseudoTimeOnSWReset = SwResetBaseTime + clk_uptime();
      }
  }
#endif

    return dwSysTime;
}

/*===========================================================================
FUNCTION UI_NEW_INFO

DESCRIPTION
  A new informational message from the base station to be displayed.  Put
  it on the queue of informational messages

DEPENDENCIES
  Can only handle text as large as we've allocated for message fields
===========================================================================*/
void SKY_API SKY_PutNewInfoMessage(UI_COMMAND_E    Command,
                                   UINT8           nLength,
                                   BYTE            *pText)
{
    INFO_MESSAGE_T *pInfo;

    if((pInfo = (INFO_MESSAGE_T *) q_get(&ui_info_free_q)) == NULL)
    {
        SKY_TRACE(TL_HIGH, "Can't get info message buffer - dropped msg");
        return;
    }
    
    if(nLength == 0 )
        return;

    STRNCPY(pInfo->szText, pText, MAX_INFO_MESSAGE_LENGTH);

    SKY_TRACE1(TL_HIGH, "Put Info MSG Len %d", nLength);

    q_put( &ui_info_q, &pInfo->Link );
}

void SKY_API SKY_AcceptKey(BOOL fAccept)
{
    if(!l_fGetKeys && fAccept)
    {
        (void) SKY_Set_Signal( &ui_tcb, UI_KEY_SIG );
    }
    l_fGetKeys = fAccept;
}


BOOL SKY_API SKY_IsKeyBufferEmpty(void)
{
   if( l_fGetKeys && (l_KeyEventStack.nHead != l_KeyEventStack.nTail) ) 
	   return FALSE;
   else
	   return TRUE;
}