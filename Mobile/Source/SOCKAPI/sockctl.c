/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               S O C K C T L

GENERAL DESCRIPTION
  This Software unit contains all the control functions for the Subscriber
  SOCK application Task. This includes all REX Interfacing functions, the task
  entry point and main processing loop, and the SOCK app. task-specific Wait 
  function.

EXTERNALIZED FUNCTIONS
  sock_cmd()
    Pass a command to the SOCK Task

  sock_task()
    Sock Task entry point and main processing loop.


INITIALIZATION AND SEQUENCING REQUIREMENTS
  SOCK_cmd() cannot be invoked until sock_task() has been invoked and MC has
  issued the Start command to sock task.

===========================================================================
00/10/05	khekim	TCP connecting도중 UI Task에 의한 SOCK_RELEASE_CMD 처리 구현
00/10/06	khekim	TCP connecting도중 UI Task에 의한 SOCK_RELEASE_CMD 수신시
					gb_Released=TRUE로 설정

   Copyright (c) 2000 by SK Teletech Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"
#include "queue.h"
#include "ds.h"
#include "dsm.h"
#include "mc.h"
#include "nv.h"
#include "msg.h"
#include "err.h"
#include "task.h"
#include "ui.h"
#include "dog.h"
#include <memory.h>
#include <stdio.h>

#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/06
#include "sockctl.h"
#include "sockApi.h"	//	khekim 01/02/11 Temporary commentout
#include "uisockApi.h"

/*===========================================================================

                  LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*--------------------------------------------------------------------------
  Timers needed for the SOCK Task.
---------------------------------------------------------------------------*/

LOCAL rex_timer_type sock_dog_rpt_timer; /* for monitoring watchdog reporting*/

rex_timer_type sock_ps_timer; /* for PS/TCP/IP time out */

sock_sigs_type	sock_signaled_events_mask;
sock_sigs_type	sock_requested_sig_mask;

/*--------------------------------------------------------------------------
  SOCK Task Command links and queues
---------------------------------------------------------------------------*/

typedef struct     /* Type for storing SOCK Task Commands on Command Queue */
{
  q_link_type	link;                        /* for linking onto a queue     */
  sock_cmd_type cmd;                         /* contains actual command data */
} socki_cmd_type;

#ifdef FEATURE_ANALOG_DATA
#error code not present
#else
#define SOCK_CMD_BUF_CNT    7
#endif

LOCAL socki_cmd_type sock_cmd_buf[SOCK_CMD_BUF_CNT];

LOCAL q_type sock_cmd_q;
LOCAL q_type sock_cmd_free_q;

/*---------------------------------------------------------------------------
  Function proto-type for the Wait procedure
---------------------------------------------------------------------------*/
sock_sigs_type sock_wait
(
  sock_sigs_type requested_mask               /* mask of signals to wait for */
);

/*===========================================================================

FUNCTION SOCK_POWERUP_INIT

DESCRIPTION
  Powerup (coldstart) initialization function for the SOCK Task. This function
  will perform all first time thru functions for the SOCK Task.

DEPENDENCIES
  This function must be called only once at system powerup.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
LOCAL void sock_powerup_init( void)
{
  word i;               /* array indexer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /*------------------------------------------------------------------------
    Define the timers needed for the SOCK Task then initialize all the queues.
  -------------------------------------------------------------------------*/
  rex_clr_sigs( rex_self(), SOCK_SOCKET_CB_SIG);
  rex_clr_sigs( rex_self(), SOCK_NET_CB_SIG);
#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
  rex_clr_sigs( rex_self(), SOCK_DNS_CB_SIG);
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API

  rex_def_timer( &sock_dog_rpt_timer, &sock_tcb, SOCK_DOG_RPT_TIMER_SIG);
  rex_def_timer( &sock_ps_timer, &sock_tcb, SOCK_PS_TIMER_SIG); 

  (void)q_init( &sock_cmd_q);
  (void)q_init( &sock_cmd_free_q);

	(void)q_init(&sockSendDataQ);
	(void)q_init(&sockRecvDataQ);

  
  /*------------------------------------------------------------------------
    Link the Command items onto the command free queue.
  -------------------------------------------------------------------------*/
  for (i = 0; i<SOCK_CMD_BUF_CNT; i++)
  {
    q_put(&sock_cmd_free_q, q_link( &sock_cmd_buf[i], &sock_cmd_buf[i].link));
  }
  /*------------------------------------------------------------------------
    Start the watchdog timer to wake up for the 1st petting.
  -------------------------------------------------------------------------*/
  (void)rex_set_timer( &sock_dog_rpt_timer, DOG_SOCK_RPT_TIME);
  
} /* sock_powerup_init */



boolean sock_check_cmd
(
	sock_cmd_type *cmd_ptr
)
{
  socki_cmd_type *link_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------
    Check for a command available by attempting to remove an item from the
    command queue. ERR_FATAL if no command is available.
  -------------------------------------------------------------------------*/
  if (( link_ptr = (socki_cmd_type *)q_check( &sock_cmd_q)) == NULL)
  {
    MSG_ERROR( "SOCK empty command queue", 0, 0, 0);
    return( FALSE);
  }
  else
  {
    /*----------------------------------------------------------------------
      Assign command data to passed buffer item, and return with TRUE.
    -----------------------------------------------------------------------*/
    *cmd_ptr = link_ptr->cmd;
    return( TRUE);
  }
} /* sock_check_cmd */



boolean sock_get_cmd(sock_cmd_type *cmd_ptr)
{
  socki_cmd_type *link_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------
    Check for a command available by attempting to remove an item from the
    command queue. ERR_FATAL if no command is available.
  -------------------------------------------------------------------------*/
  if (( link_ptr = (socki_cmd_type *)q_get( &sock_cmd_q)) == NULL)
  {
    MSG_ERROR( "SOCKET empty command queue", 0, 0, 0);
    return( FALSE);
  }
  else
  {
    /*----------------------------------------------------------------------
      Assign command data to passed buffer item, put queue link onto free
      queue and return with TRUE.
    -----------------------------------------------------------------------*/
    *cmd_ptr = link_ptr->cmd;
    q_put( &sock_cmd_free_q, &link_ptr->link);
    return( TRUE);
  }
} /* sock_get_cmd */

/*===========================================================================

FUNCTION SOCK_INIT

DESCRIPTION
  Initialization function for the socket task. This function 
  performs the functions needed for the scket Task to exit disabled state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void sock_cancel_timer( void)
{
	rex_clr_timer(&sock_ps_timer); 
	rex_clr_sigs(&sock_tcb, SOCK_PS_TIMER_SIG);
} /* sock_cancel_timer */

void sock_init( void)
{
	sock_cancel_timer();
	cleanSockDataQueue();
} /* sock_init */


/*==========================================================================
  
   FUNCTION SOCK_WAIT
  
   DESCRIPTION 
     This function performs the SOCK Task waiting function. This function will
     not return until a signal from the requested mask is set or a queue
     associated with a requested signal contains an item. Checks are made to
     insure that signals associated with queues are not set without items in
     the queue. 
     The watchdog is also kicked at least once.
   
   DEPENDENCIES 
     None

   RETURN VALUE 
     A signal mask respresenting the set sigs or non-empty queues for the
     requested signals
  
   SIDE EFFECTS
     None

===========================================================================*/

#ifdef	FEATURE_SKT_MULTISOCKAPI	//	khekim 01/02/03
uint32 sock_wait_sig(uint32 timer_cnt, uint32 wait_sig, uint32 wait_sig_mask, uint32 *sigCmd)
{
	long int				rex_sigs;
	sock_cmd_type           cmd_item;        /* to hold the de-queued command */

	rex_set_timer(&sock_ps_timer, timer_cnt); 
	*sigCmd = 0;

	if( (rex_sigs=(sock_wait(wait_sig) & wait_sig_mask)) != 0)
	{
		if ( (rex_sigs&SOCK_CMD_Q_SIG) != 0)
		{
			if (!sock_check_cmd(&cmd_item)) 
			{
			   MSG_ERROR("Checking empty command queue",0,0,0);
			}
			MSG_HIGH("sock_mgr: CMDQcnt%x", q_cnt( &sock_cmd_q), 0, 0);
			if (cmd_item.cmd_name==SOCKET_RELEASE_CMD)
			{
				MSG_HIGH("rxed SOCKET_RELEASE_CMD by UI during TCP connecting", 0, 0, 0);
				*sigCmd = SOCKET_RELEASE_CMD;
			}
			if ( sock_get_cmd( &cmd_item) == FALSE) 
			{
				MSG_ERROR("sock_mgr: No command on cmd_q",0,0,0);
				 /* Pathological error check - should err_fatal in function itself */
			}
			sock_cancel_timer();
			return (SOCK_CMD_Q_SIG);
		}
		else if ( (rex_sigs&SOCK_PS_TIMER_SIG) != 0)
		{
			return (SOCK_PS_TIMER_SIG);
		}
		else	//	SOCK_SOCKET_CB_SIG or SOCK_NET_CB_SIG or SOCK_DNS_CB_SIG
		{
			sock_cancel_timer();
			if ((rex_sigs&SOCK_NET_CB_SIG) != 0)
			{
				return (SOCK_NET_CB_SIG);
			}	
			else if ((rex_sigs&SOCK_SOCKET_CB_SIG) != 0)
			{
				return (SOCK_SOCKET_CB_SIG);
			}
#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
			else if ((rex_sigs&SOCK_DNS_CB_SIG) != 0)
			{
				return (SOCK_DNS_CB_SIG);
			}
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API
			else
			{
				return 0;
			}
		}
	}
	else
	{
		sock_cancel_timer();
		return 0;
	}
}
#else	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
int sock_wait_sig(long int timer_cnt, long int wait_sig, long int wait_sig_mask)
{
	long int				rex_sigs;
	sock_cmd_type           cmd_item;        /* to hold the de-queued command */

	rex_set_timer(&sock_ps_timer, timer_cnt); 

	if( (rex_sigs=(sock_wait(wait_sig) & wait_sig_mask)) != 0)
	{
		if ( (rex_sigs&SOCK_CMD_Q_SIG) != 0)
		{
			if (!sock_check_cmd(&cmd_item)) 
			{
			   MSG_ERROR("Checking empty command queue",0,0,0);
			}
			MSG_HIGH("sock_mgr: CMDQcnt%x", q_cnt( &sock_cmd_q), 0, 0);
			if (cmd_item.cmd_name==SOCKET_RELEASE_CMD)
			{
				MSG_HIGH("rxed SOCKET_RELEASE_CMD by UI during TCP connecting", 0, 0, 0);
				g_bReleased = TRUE;
			}
			if ( sock_get_cmd( &cmd_item) == FALSE) 
			{
				MSG_ERROR("sock_mgr: No command on cmd_q",0,0,0);
				 /* Pathological error check - should err_fatal in function itself */
			}
			sock_cancel_timer();
#ifdef	FEATURE_SKT_MULTISOCKAPI	//	khekim 01/02/03
			return (SOCK_CMD_Q_SIG);
#else	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
			return (DSS_ERROR);
#endif	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
		}
		else if ( (rex_sigs&SOCK_PS_TIMER_SIG) != 0)
		{
#ifdef	FEATURE_SKT_MULTISOCKAPI	//	khekim 01/02/03
			return (SOCK_PS_TIMER_SIG);
#else	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
			return (DSS_ERROR);
#endif	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
		}
		else	//	SOCK_SOCKET_CB_SIG or SOCK_NET_CB_SIG or SOCK_DNS_CB_SIG
		{
			sock_cancel_timer();
#ifdef	FEATURE_SKT_MULTISOCKAPI	//	khekim 01/02/03
			if ((rex_sigs&SOCK_NET_CB_SIG) != 0)
			{
				return (SOCK_NET_CB_SIG);
			}	
			else if ((rex_sigs&SOCK_SOCKET_CB_SIG) != 0)
			{
				return (SOCK_SOCKET_CB_SIG);
			}
#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
			else if ((rex_sigs&SOCK_DNS_CB_SIG) != 0)
			{
				return (SOCK_DNS_CB_SIG);
			}
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API
			else
			{
				return 0;
			}
#else	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
			return (DSS_SUCCESS);
#endif	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
		}
	}
	else
	{
#ifdef	FEATURE_SKT_MULTISOCKAPI	//	khekim 01/02/03
		return 0;
#else	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
		sock_cancel_timer();
		return(DSS_ERROR);
#endif	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
	}
}
#endif	//	#ifdef	FEATURE_SKT_MULTISOCKAPI


sock_sigs_type sock_wait
(
  sock_sigs_type requested_mask               /* mask of signals to wait for */
) 
{
  sock_sigs_type queue_mask;          /* signal mask indicating non-empty Qs */
  sock_sigs_type rex_signals_mask;    /* mask of signals returned by rex     */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  queue_mask = 0;                     /* Initialize queue info mask holder */

  /*========================================================================
    Start by checking the queues or watermarks associated with the requested
    signals. Set the queue_mask for any non-empty and requested signals.
    This provides a means to avoid the actual wait call.
  ========================================================================*/
  if(( requested_mask & SOCK_CMD_Q_SIG) != 0)
  {    
    /*-----------------------------------------------------------------------
      The command signal is in requested mask. Clear the signal then check 
      for a non-empty queue. If not empty add the signal into the queue mask.
    -----------------------------------------------------------------------*/
    (void)rex_clr_sigs(&sock_tcb, SOCK_CMD_Q_SIG);
    if ( q_cnt( &sock_cmd_q) != 0)
    {    
      queue_mask |= SOCK_CMD_Q_SIG;
	  MSG_HIGH("sock_cmd_q count is not zero", 0, 0, 0);
    }
  }
  if((requested_mask & SOCK_SENDDATA_Q_SIG) != 0)
  { 
	rex_clr_sigs(&sock_tcb, SOCK_SENDDATA_Q_SIG);
	if ( q_cnt( &sockSendDataQ) != 0)           /* are any elements on queue */
	{  
		queue_mask |= SOCK_SENDDATA_Q_SIG;
		MSG_HIGH("sockSendDataq count is not zero", 0, 0, 0);
	}
  }

  rex_signals_mask = rex_get_sigs(&sock_tcb);
	
//  MSG_HIGH("mask is %x, %x %x", rex_signals_mask, queue_mask, requested_mask);
  /*========================================================================
    Enter into the Wait only if no requested signal events have occurred (or
    the associated queue is empty).
  ========================================================================*/
  do
  {
    if ((( rex_signals_mask | queue_mask) & requested_mask) == 0)
    {
	  //MSG_HIGH("SOCK call rex_wait..", 0, 0, 0);
      rex_signals_mask = rex_wait(requested_mask | SOCK_DOG_RPT_TIMER_SIG);
    }
    if (( rex_signals_mask & SOCK_DOG_RPT_TIMER_SIG) != 0)
    {
      /*---------------------------------------------------------------------
        The watchdog needs a kick. Kick the pooch then set timer for next.
      ---------------------------------------------------------------------*/
      dog_report( DOG_SOCK_RPT);
	  //MSG_HIGH("SOCK alive...", 0, 0, 0);
      (void)rex_set_timer( &sock_dog_rpt_timer, DOG_SOCK_RPT_TIME);
    }
    /*----------------------------------------------------------------------
      Do checks for a signal set but queue EMPTY, remove signal from return
      mask if the signal was set erroneously (ie. sig set but queue empty).
      Also clear all set signals.
    -----------------------------------------------------------------------*/
    if(( rex_signals_mask & SOCK_CMD_Q_SIG) != 0)
    {
        MSG_HIGH( "Cmd Sig rxed in Socket sig%x req%x", rex_signals_mask, requested_mask, 0);
      /*---------------------------------------------------------------------
        REX said the command queue has an item. Clear the signal then check
        the queue. If no item then remove signal from return mask.
      ---------------------------------------------------------------------*/
      (void)rex_clr_sigs( &sock_tcb, SOCK_CMD_Q_SIG);
      if ( q_cnt( &sock_cmd_q) == 0)           /* are any elements on queue */
      {  
        MSG_HIGH( "Command Sig Ignored, Q empty", 0, 0, 0);
        rex_signals_mask &= ~SOCK_CMD_Q_SIG;             /* turn off signal */
      }
    }

	if((rex_signals_mask & SOCK_SENDDATA_Q_SIG) != 0)
	{ 
        MSG_HIGH( "SOCK_SENDDATA_Q_SIG received in Socket %x", rex_signals_mask, 0, 0);
		rex_clr_sigs(&sock_tcb, SOCK_SENDDATA_Q_SIG);
		if ( q_cnt( &sockSendDataQ) == 0)           /* are any elements on queue */
		{  
			MSG_HIGH( "Command Sig Ignored, Q empty", 0, 0, 0);
			rex_signals_mask &= ~SOCK_SENDDATA_Q_SIG;             /* turn off signal */
		}
	}

	if((rex_signals_mask & SOCK_PS_TIMER_SIG) != 0)
	{
		MSG_HIGH( "sock_PS_TIMER_SIG issed", 0, 0, 0);
		rex_clr_timer(&sock_ps_timer); 
		rex_clr_sigs(&sock_tcb, SOCK_PS_TIMER_SIG);
	}

	if((rex_signals_mask & SOCK_NET_CB_SIG) != 0)
	{
		MSG_HIGH( "SOCK_NET_CB_SIG issed", 0, 0, 0);
		rex_clr_sigs(&sock_tcb, SOCK_NET_CB_SIG);
	}

	if((rex_signals_mask & SOCK_SOCKET_CB_SIG) != 0)
	{ 
		MSG_HIGH( "SOCK_SOCKET_CB_SIG issed", 0, 0, 0);
		rex_clr_sigs(&sock_tcb, SOCK_SOCKET_CB_SIG);
	}

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
	if((rex_signals_mask & SOCK_DNS_CB_SIG) != 0)
	{ 
		MSG_HIGH( "SOCK_DNS_CB_SIG issed", 0, 0, 0);
		rex_clr_sigs(&sock_tcb, SOCK_DNS_CB_SIG);
	}
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API

  } while((( rex_signals_mask | queue_mask) & requested_mask) == 0);
  /*========================================================================
    Return all set signals. This includes those not requested.
  ========================================================================*/
  return(( rex_signals_mask | queue_mask));
} /* sock_wait */


/*===========================================================================

FUNCTION SOCK_CMD

DESCRIPTION
  This function is the interface function for the SOCK Task. Call this 
  function with a specified command (or RLP RX data frame) and the appropriate
  processing will occur. 

  NOTE: The passed command/frame will be copied to a SOCK Task command/frame 
        item local to the SOCK Task (ie. no need to grab a free link).
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void sock_cmd
(
  sock_cmd_type *cmd_ptr                      /* pointer to the command data */
)
{
	socki_cmd_type        *link_ptr;                  /* for queueing commands */
    /*----------------------------------------------------------------------
      Get a link from the free queue, call error routine if no links 
      available, else copy buffer to internal link.
    -----------------------------------------------------------------------*/
 	if (( link_ptr = (socki_cmd_type *) q_get( &sock_cmd_free_q)) == NULL)
    {
      MSG_ERROR( "SOCK Task Command link not available", 0, 0, 0);
    }
    else /* Assign to the link, put link onto queue and signal SOCK Task.  */
    {
      link_ptr->cmd = *cmd_ptr;
      q_put( &sock_cmd_q, &link_ptr->link);
      (void)rex_set_sigs( &sock_tcb, SOCK_CMD_Q_SIG);
    }
//	MSG_HIGH("cmd_ptr->cmd_name...%d", (int)cmd_ptr->cmd_name, 0, 0);
} /* sock_cmd */


/*===========================================================================

FUNCTION SOCK_TASK

DESCRIPTION
  This function is the entry point and main processing loop for the Data
  Service(s) task.

DEPENDENCIES
  Does not Return.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void sock_task
( 
  dword ignored
    /* Parameter received from Main Control task - ignored */
    /*lint -esym(715,ignored)
    ** Have lint not complain about the ignored parameter 'ignored' which is
    ** specified to make this routine match the template for rex_def_task().
    */
) 
{
  rex_sigs_type   state_sigs,            /* SOCK Task Mgr. requested mask  */
                  set_sigs;              /* Return from 'wait' signal mask */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------
    Do power-up initializtion then wait for start signal (from the MC task,
    which controls task startup sequencing)
  -------------------------------------------------------------------------*/
  sock_powerup_init();
  mc_task_start( SOCK_DOG_RPT_TIMER_SIG, DOG_SOCK_RPT, &sock_dog_rpt_timer);

  MSG_HIGH("sock_task: mc_task_start called...", 0, 0, 0);
  
  sock_requested_sig_mask = state_sigs = SOCK_CMD_Q_SIG;
  /*------------------------------------------------------------------------
    Main task loop. Never exit!
  -------------------------------------------------------------------------*/
  for( ;;)
  {
    /*----------------------------------------------------------------------
      Wait for work to do. No return from wait until one of the specified
      signals is set. Note that watchdog kicking is performed in the wait.
    -----------------------------------------------------------------------*/
    set_sigs = sock_wait( TASK_START_SIG | TASK_STOP_SIG | TASK_OFFLINE_SIG | SOCK_CMD_Q_SIG | state_sigs );

	MSG_MED("next sock_wait req%x set%x st%x ", sock_requested_sig_mask, set_sigs, state_sigs);

    /*----------------------------------------------------------------------
      Check if powerup command signal was set.  If set then clear signal, 
      send ack back to MC, and proceed.
    -----------------------------------------------------------------------*/
    if(( set_sigs & TASK_START_SIG) != 0) 
    {
      MSG_MED( "TASK_START_SIG received", 0,0,0);
      (void) rex_clr_sigs( &sock_tcb, TASK_START_SIG);
      (void) rex_set_sigs( &mc_tcb, MC_ACK_SIG);
    }

    /*----------------------------------------------------------------------
      Check if powerdown command signal was set.  If set then clear signal, 
      send ack back to MC, and proceed.
    -----------------------------------------------------------------------*/
    if(( set_sigs & TASK_STOP_SIG) != 0) 
    {
      MSG_MED( "TASK_STOP_SIG received", 0,0,0);
      (void) rex_clr_sigs( &sock_tcb, TASK_STOP_SIG);
      (void) rex_set_sigs( &mc_tcb, MC_ACK_SIG);
    }
    /*----------------------------------------------------------------------
      Check if offline command signal was set.  If set then clear signal, 
      send ack back to MC, and proceed.
    -----------------------------------------------------------------------*/
    if(( set_sigs & TASK_OFFLINE_SIG) != 0) 
    {
      MSG_MED( "TASK_OFFLINE_SIG received", 0,0,0);
      (void) rex_clr_sigs( &sock_tcb, TASK_OFFLINE_SIG);
      (void) rex_set_sigs( &mc_tcb, MC_ACK_SIG);
    }

    /*----------------------------------------------------------------------
      Check if at least one of the currently active State sigs has been set.
    -----------------------------------------------------------------------*/
    if(( set_sigs & state_sigs) != 0)
    {
      /*--------------------------------------------------------------------
        Invoke Socket state machine to service the set signal(s). Pass
        in the set sigs. Use return value for the next state signal mask.
      ---------------------------------------------------------------------*/
      state_sigs = sock_mgr( set_sigs);
    }
  } /* forever */
} /* sock_task */


sock_sigs_type sock_mgr
(
	sock_sigs_type signal_mask                /* mask of signals that occurred */
)
{
	sock_cmd_type           next_cmd_item, cmd_item;        /* to hold the de-queued command */
 
	sock_signaled_events_mask = (signal_mask & sock_requested_sig_mask);

	if (sock_signaled_events_mask & SOCK_CMD_Q_SIG) 
	{

		if (!sock_check_cmd(&next_cmd_item)) 
		{
		   MSG_ERROR("Checking empty command queue",0,0,0);
		}
		MSG_HIGH("sock_mgr: CMDQcnt%x", q_cnt( &sock_cmd_q), 0, 0);
	
		memcpy((byte *)&cmd_item, (byte*)&next_cmd_item, sizeof(sock_cmd_type));

		if ( sock_get_cmd( &next_cmd_item) == FALSE) 
		{
			MSG_ERROR("sock_mgr: No command on cmd_q",0,0,0);
			 /* Pathological error check - should err_fatal in function itself */
		}
		switch (cmd_item.cmd_name) 	
		{
			case	SOCKET_SVR_PPPCONNECT_CMD:
				{
					MSG_HIGH("process SOCKET_SVR_PPPCONNECT_CMD", 0, 0, 0);
					sockApi_openPPPNetwork();
				}
				break;

			case	SOCKET_SVR_TCPCONNECT_CMD:
				{
					MSG_HIGH("process SOCKET_SVR_TCPCONNECT_CMD: %d", cmd_item.num, 0, 0);
					sockApi_openTCPNetwork(cmd_item.parms.serverName, cmd_item.num);
//					sockApi_openTCPNetwork(cmd_item.parms.serverName, 21);	// FTP portNo = 21
				}
				break;

			case	SOCKET_SVR_TCPRECONNECT_CMD:
				{
					MSG_HIGH("process SOCKET_SVR_TCPRECONNECT_CMD: %d", cmd_item.num, 0, 0);
					sockApi_reopenTCPNetwork(cmd_item.parms.serverName, cmd_item.num);
				}
				break;

			case	SOCKET_SVR_UDPCONNECT_CMD:
				{
					MSG_HIGH("process SOCKET_SVR_UDPCONNECT_CMD: %d", cmd_item.num, 0, 0);
					sockApi_openUDPNetwork(cmd_item.parms.serverName, cmd_item.num);
				}
				break;

			case	SOCKET_RELEASE_CMD:
				{
					MSG_HIGH("process SOCKET_RELEASE_CMD", 0, 0, 0);
					sockApi_closeNetworkbyReleaseCmd();
				}
				break;

			default:
				{
					MSG_ERROR("SOCK_CMD_Q_SIG: invalid cmd %d", cmd_item.cmd_name, 0, 0);
				}

		} //switch
	}
	if (sock_signaled_events_mask & SOCK_NET_CB_SIG) 
	{
		MSG_HIGH("process SOCK_NET_CB_SIG", 0, 0, 0);
		sockApi_processNetCallBackFn();
	}
	if (sock_signaled_events_mask & SOCK_SOCKET_CB_SIG) 
	{
		MSG_HIGH("process SOCK_SOCKET_CB_SIG", 0, 0, 0);
		sockApi_processSockCallBackFn();
	}
#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
	if (sock_signaled_events_mask & SOCK_DNS_CB_SIG) 
	{
		MSG_HIGH("process SOCK_DNS_CB_SIG", 0, 0, 0);
		sockApi_processDnsCallBackFn();	//	Not Processed here
	}
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API
	if (sock_signaled_events_mask & SOCK_SENDDATA_Q_SIG)
	{
		sockApi_sendData();
	}

   	return (sock_requested_sig_mask);
}

#endif // #if defined(FEATURE_SKT_SOCKAPI)