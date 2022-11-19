/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_hsig.c
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
** 00-11-04   jrkwon     re-created
** 00-10-12   redstar    created
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"

#include "eqs.h"
#include "sky.h"
#include "FileSys.h"
///Added by happykid in 2001.1.2
#ifdef FEATURE_SMART_BATTERY
#include "smb.h"    
extern boolean smbKeyTouchFlag;            //youandi 2001.02.21          
#endif
#ifdef FEATURE_SKVM
#include "ska_skvm.h"
#endif

#ifdef FEATURE_WAP
#include "wapcommon.h"
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_IRDA_SIGNAL
DWORD  g_wIrdaSignalCount = 0;
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
**---------------------------------------------------------------------------
**  General Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
#ifdef FEATURE_WAP
extern rex_timer_type  wap_animation_timer;    /* Timer for SIS animation   */
extern rex_timer_type  wap_nowrap_timer;       /* Timer for Nowrap          */
extern rex_timer_type  wap_browser_timer;      /* Timer for Browser         */
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_WAVELET
extern rex_timer_type  wavelet_alive_timer;
#endif // FEATURE_WAVELET
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Local Function Declaration
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Local Function Definitions
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global Function Definitions
**---------------------------------------------------------------------------
*/

/* 

startup에서 사용되는 다음의 플래그를 

ui_pwrdown_complete
ui_mc_initiated_pwrdown

signal hander 쪽으로 옮긴다.

*/


BOOL g_fPowerDownComplete;
BOOL g_fMCInitiatedPowerDown;

/*===========================================================================
FUNCTION EQS_HandleSignal

DESCRIPTION
  This function handles all signals received by UI task. This function is to
  be invoked for every signal UI recives. It calls the appropriate functions
  to handle each signal.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void SKY_API SKY_HandleSignal(REX_SIGNAL Signal, QUEUE_T *pCommandQueue)
{
    STATIC UI_COMMAND_U *pCommand;
    STATIC REX_TCB_T     *pTask;
    STATIC REX_SIGNAL    CompletionSignal;
#ifdef FEATURE_WAP
    wap_cmd_type	 *cmd_ptr;
#endif //#ifdef FEATURE_WAP
    
#ifdef FEATURE_DS_UP
#error code not present
#endif

#ifndef WIN32
#ifdef FEATURE_SKVM
	 int retVal = SKVMMain();	
	 if (retVal >= 0)
	 {
		// SKVM core에서 UI_KEY_SIG을 clear하기때문
		if (!SKVM_IS_PLAY_BG()) 
			Signal = (rex_get_sigs(rex_self()) & (~UI_KEY_SIG));
		if (retVal == 0)
			Signal = Signal & (~UI_CMD_Q_SIG);
	 }
#endif
#endif

    /* one of several timers went off */
    if( Signal & UI_TIMERS_SIG ) 
    {
        SKY_HandleTimers();
        (void) SKY_Clear_Signal( &ui_tcb, UI_TIMERS_SIG );
    }

#ifdef FEATURE_OTASP
    /* Stop task - nothing special to do */
    if ( ( Signal & TASK_STOP_SIG ) != 0 )
    {
        /* If MC initiates the power-down we will get the signal, before */
        /* we went through our power-down procedure. In that case we do  */
        /* not want to ack right now.                                    */
        if ( g_fPowerDownComplete == TRUE )
        {
            (void) SKY_Set_Signal( &mc_tcb, MC_ACK_SIG );
            (void) SKY_Clear_Signal( rex_self(), TASK_STOP_SIG );
            g_fPowerDownComplete = FALSE ;    /* clear flag */       
        }       
        else       
        {
            /* MC decided to power-down, let's tell UI about it */
            g_fMCInitiatedPowerDown = TRUE ;

            SKY_DoEvent(SE_SYS_TO_IDLE);       /* back to idle */
            SKY_DoEvent(SE_KEY_POWER_OFF);     /* power key is switched 'off' */
        }
    }
#else
    /* Stop task - nothing special to do */
    if ( ( Signal & TASK_STOP_SIG ) != 0 ) 
    {
        (void) SKY_Set_Signal( &mc_tcb, MC_ACK_SIG );
        (void) SKY_Clear_Signal( rex_self(), TASK_STOP_SIG );
    }
#endif

    /* Go offline signal */
    if( ( Signal & TASK_OFFLINE_SIG ) != 0 ) 
    {
        (void) SKY_Clear_Signal( rex_self(), TASK_OFFLINE_SIG );
        SKY_TRACE(TL_MED, "UI got offline sig");

        /* If we're not offline already, tell the state machine about it */
        if(!SKY_IS_OFFLINE())
        {
            SKY_DoEvent(SE_SYS_TO_IDLE);       /* back to idle */
            SKY_DoEvent(SE_SYS_OFFLINE);
        }
        /* Signal back to MC that we're offline */
        (void) SKY_Set_Signal( &mc_tcb, MC_ACK_SIG );
    }

    /* SKY : 
    ** UI_RING_SIG는 사운드 태스크에서 링 사운드가 끝났나는 것을 알려주는
    ** 시그널이다. 하지만, 멜로디 벨의 경우에는 의미가 없다.
    ** 자동응답이나 자동 착신 등에서는 벨이 울린 수가 아니라 시간을 기준으로
    ** 동작한다.
    */

    /* Multitone has ended, let UI sound manager know about it */
    if ( Signal & UI_MULTI_STOP_SIG ) 
    {
        (void) SKY_Clear_Signal( rex_self(), UI_MULTI_STOP_SIG );

        SKY_DoEvent(SE_SND_MULTI_TONE_DONE);   /* let state machine know */
    }

    /* Command queue signal.*/
    if( Signal & UI_CMD_Q_SIG )
    {
        (void) SKY_Clear_Signal( &ui_tcb, UI_CMD_Q_SIG );

        /* Handle commands on queue till the queue is empty. */
        while( ( pCommand = (UI_COMMAND_U *) q_get( pCommandQueue )) != NULL )
        {
            SKY_HandleCommands( pCommand );         /* actually handle it */

            pTask = pCommand->hdr.task_ptr; /* save for when the buffer*/
            CompletionSignal = pCommand->hdr.sigs;         /* is back on the queue    */

            if( pCommand->hdr.done_q_ptr )
            {
                /* Put it back on a queue. */
                q_put( pCommand->hdr.done_q_ptr, &pCommand->hdr.link );
            }

            if( pTask )
            { 
                /* Signal back to the calling task. */
                (void) SKY_Set_Signal( pTask, CompletionSignal );
            }      
        }
    }

    /* Report back to the watchdog.  Whack!  Yipe yipe yipe... */
    if( Signal & UI_RPT_TIMER_SIG ) 
    {
        SKY_KickDog();
    }

    if( Signal & UI_KEY_SIG ) 
    {
///Added by happykid in 2001.1.2
#ifdef FEATURE_SMART_BATTERY
#if MODEL_ID == MODEL_B0 // youandi 2000.01.02
	smbKeyTouchFlag = TRUE;
#elif MODEL_ID == MODEL_B1 || (MODEL_ID == MODEL_B2)// youandi 2000.01.02
	smbKeyTouchFlag = TRUE;	
#endif    
#endif
		(void) SKY_Clear_Signal( rex_self(), UI_KEY_SIG );
		SKY_HandleKeys();
        
    } /* key signal set */

	/* Key event signal - we have keys from the handset */
    // 녹음 재생시 flfread,flfwrite에서 fs를 호출하지 않고
	//UI_PLAY_REC_SIG를 UI로 보내서 ui에서 fs를 호출하도록 함
    if( Signal & UI_PLAY_REC_SIG ) 
    {	//by hjcho  2000.12.21
        (void) SKY_Clear_Signal( rex_self(), UI_PLAY_REC_SIG );
	    subcmd_play_rec ();
	}

	//Socket으로부터 Data 수신
    if( Signal & UI_RECVDATA_Q_SIG )
	{
		(void) SKY_Clear_Signal( rex_self(), UI_RECVDATA_Q_SIG );
		SKY_DoEvent(SE_RECV_DATA);
	}
#ifdef FEATURE_SKT_EIF

    if( Signal & UI_UPONG_SIG )
	{
		(void) SKY_Clear_Signal( rex_self(), UI_UPONG_SIG );
		SKY_DoEvent(SE_WAP_EIF_UPONG);
	}

#endif	//#ifdef FEATURE_SKT_EIF

#ifdef FEATURE_IRDA_SIGNAL
    if( Signal & UI_IRDA_RX_SIG )
    {
        (void) SKY_Clear_Signal( rex_self(), UI_IRDA_RX_SIG );

        if(g_wIrdaSignalCount)
        {
            SKY_DoEvent(SE_IRDA_EX_RX_DATA_F);
            g_wIrdaSignalCount --;

            if(g_wIrdaSignalCount)
                (void) SKY_Set_Signal( &ui_tcb, UI_IRDA_RX_SIG );  
        }

/*
        while(g_wIrdaSignalCount > 0)
        {
            SKY_DoEvent(SE_IRDA_EX_RX_DATA_F);
            g_wIrdaSignalCount --;
        }
*/
    }
#endif

#ifdef FEATURE_VAS
        /* Handle the nework signal, acknowledge and continue */
        if ( (Signal & UI_NETWORK_CB_SIG) != 0 ) 
        {
          (void) SKY_Clear_Signal( &ui_tcb, UI_NETWORK_CB_SIG); /* Clear the signal */
		  (void) ProcessNetworkCallback();
        }

        /* Handle the socket signal, acknowledge and continue */
        if ( (Signal & UI_SOCKET_CB_SIG) != 0 ) 
        {
          (void) SKY_Clear_Signal( &ui_tcb, UI_SOCKET_CB_SIG); /* Clear the signal */
		  (void) ProcessSocketCallback();
        }
        /* Handle the DNS signal, acknowledge and continue */
        if ( (Signal & UI_DNS_CB_SIG) != 0 ) 
        {
          (void) SKY_Clear_Signal( &ui_tcb, UI_DNS_CB_SIG); /* Clear the signal */
        }
		/* Handle the process event signal */
		if ( (Signal & UI_EVT_PROCESS_SIG) != 0 )
		{
			extern void CLNTc_run(void);
			extern BOOL CLNTc_wantsToRun(void);
			
			(void) SKY_Clear_Signal( &ui_tcb, UI_EVT_PROCESS_SIG );
			while( CLNTc_wantsToRun() )
			{
				CLNTc_run();
			}
		}

#endif//feature_vas
#ifdef FEATURE_WAP
        if ( (Signal & WAP_CMD_Q_SIG) != 0 ) 
        {
            (void) SKY_Clear_Signal( &ui_tcb, WAP_CMD_Q_SIG );

        /* Handle commands on queue till the queue is empty. */
            while( ( cmd_ptr = (wap_cmd_type *)q_get( &wap_cmd_q )) != NULL ) 
            {
	            WAP_HandleCommands( cmd_ptr );          /* actually handle it */
                wap_check_and_run();
                pTask = cmd_ptr->hdr.task_ptr; /* save for when the buffer*/
                CompletionSignal = cmd_ptr->hdr.sigs;         /* is back on the queue    */

                if( cmd_ptr->hdr.done_q_ptr )
                {
                    /* Put it back on a queue. */
                    q_put( cmd_ptr->hdr.done_q_ptr, &cmd_ptr->hdr.link );
                }

                if( pTask )
                { 
                    /* Signal back to the calling task. */
                    (void) SKY_Set_Signal( pTask, CompletionSignal );
                }      
            }            
        }

         if((Signal & BAR_ANI_SIG) != 0 )
        {
    	    (void) SKY_Clear_Signal( &ui_tcb, BAR_ANI_SIG );
		    process_draw_bar();
        }

#ifdef WAP_NOWRAP	//yjahn96 00.10.27
        if ( (Signal & WAP_NOWRAP_TIMER_SIG) != 0 ) 
        {
            extern void Support_NoWrapping(void);

            (void) SKY_Clear_Signal( &ui_tcb, WAP_NOWRAP_TIMER_SIG); /* Clear the signal */
			(void) rex_set_timer( &wap_nowrap_timer, 1000 );
            Support_NoWrapping();
        }
#endif //#ifdef WAP_NOWRAP

        if ( (Signal & WAP_ANIMATION_TIMER_SIG) != 0 ) 
        {
		    extern void Play_SIS_Animation(void);
            (void) SKY_Clear_Signal( &ui_tcb, WAP_ANIMATION_TIMER_SIG); /* Clear the signal */
            (void) rex_set_timer( &wap_animation_timer, 600 );
            Play_SIS_Animation();		  
        }

        if ( (Signal & WAP_BROWSER_TIMER_SIG) != 0 ) 
        {
		    extern void CancelBrowserTimer(void);
			extern void CLNTc_timerExpired(void);
			extern void DevProcessEvents(void);

			MSG_MED("WAP_BROWSER_TIMER_SIG issued",0,0,0);
			CancelBrowserTimer();
            CLNTc_timerExpired();		
			DevProcessEvents();   
        }
		
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_WAVELET
        if ( (Signal & WAVELET_ALIVE_TIMER_SIG) != 0 ) 
        {
			extern void MM_TimerDo(void);
            (void) SKY_Clear_Signal( &ui_tcb, WAVELET_ALIVE_TIMER_SIG); /* Clear the signal */
            (void) rex_set_timer( &wavelet_alive_timer, 500 );
			MM_TimerDo();
        }
#endif // FEATURE_WAVELET
#ifdef SUPPORT_REX_TIMER_IN_SKVM
    if ( (Signal & XVM_ACTIVE_TIMER1_SIG) != 0 ) 
	{
        (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER1_SIG); /* Clear the signal */
       if (xvmActive)
		{
			(void) SKY_Set_Signal( &ui_tcb, XVM_TIMERS_SIG );
		}
	}
    if ( (Signal & XVM_ACTIVE_TIMER2_SIG) != 0 ) 
	{
		extern BOOL	g_bxvm_sig_timer2;
        (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER2_SIG); /* Clear the signal */
        if (xvmActive)
		{
	       (void) SKY_Set_Signal( &ui_tcb, XVM_TIMERS_SIG );
			g_bxvm_sig_timer2 = FALSE;
		}
	}
    if ( (Signal & XVM_ACTIVE_TIMER3_SIG) != 0 ) 
	{
        (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER3_SIG); /* Clear the signal */
        if (xvmActive)
		{
			(void) SKY_Set_Signal( &ui_tcb, XVM_TIMERS_SIG );
		}
	}
#endif

}


#ifdef FEATURE_WAP
void CancelAnimationTimer(void)
{
	MSG_HIGH("CancelAnimationTimer !! ",0,0,0);
	(void) SKY_Clear_Signal( &ui_tcb, WAP_ANIMATION_TIMER_SIG); /* Clear the signal */
	rex_clr_timer(&wap_animation_timer);
}

void RestartAnimationTimer(unsigned long ms)
{
	rex_clr_timer(&wap_animation_timer);

	(void) SKY_Clear_Signal( &ui_tcb, WAP_ANIMATION_TIMER_SIG); /* Clear the signal */
	if(ms)
	{
		MSG_HIGH("RestartAnimationTimer !! ",0,0,0);
		rex_set_timer(&wap_animation_timer, ms);
	}
}

void CancelNowrapTimer(void)
{
    (void) SKY_Clear_Signal( &ui_tcb, WAP_NOWRAP_TIMER_SIG); /* Clear the signal */
	rex_clr_timer(&wap_nowrap_timer);
}

void RestartNowrapTimer(unsigned long ms)
{
	rex_clr_timer(&wap_nowrap_timer);
	(void) SKY_Clear_Signal( &ui_tcb, WAP_NOWRAP_TIMER_SIG); /* Clear the signal */
	if(ms)
	{
		rex_set_timer(&wap_nowrap_timer, ms);
	}

}

void CancelBrowserTimer(void)
{
	rex_clr_timer(&wap_browser_timer);
    (void) SKY_Clear_Signal(&ui_tcb, WAP_BROWSER_TIMER_SIG);
}

void RestartBrowserTimer(unsigned long ms)
{
	rex_clr_timer(&wap_browser_timer);

	(void) SKY_Clear_Signal( &ui_tcb, WAP_BROWSER_TIMER_SIG); /* Clear the signal */
	if(ms)
	{
		rex_set_timer(&wap_browser_timer, ms);
	}
}

void ProcessEvents(void)
{
	(void) SKY_Set_Signal( &ui_tcb, UI_EVT_PROCESS_SIG);
}

#endif  //#ifdef FEATURE_WAP

#ifdef FEATURE_WAVELET
void CancelWaveletAliveTimer(void)
{
	rex_clr_timer(&wavelet_alive_timer);
}

void RestartWaveletAliveTimer(unsigned long ms)
{
	rex_clr_timer(&wavelet_alive_timer);

	(void) SKY_Clear_Signal( &ui_tcb, WAVELET_ALIVE_TIMER_SIG); /* Clear the signal */
	if(ms)
	{
		rex_set_timer(&wavelet_alive_timer, ms);
	}
}
#endif //FEATURE_WAVELET