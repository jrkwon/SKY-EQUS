/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


           Wireless Application Protocol   M A I N  M O D U L E

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS
  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/18/99   swjung  Create WAP task for WAP service
===========================================================================*/
/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "mc.h"
#include "task.h"
#include "dog.h"
#include "target.h"
#include "msg.h"
#include "rex.h" 
#include "waptask.h"
#include "wapadapter.h"
#include "wapconnector.h"
#include "clnt.h"
#include "confvars.h"
#include <memory.h>

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

#include "aapimem.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "capimem.h"
#include "capimmi.h"
#include "netuser.h"

#include "tapicmmn.h"
#if MMI_TYPE == SKY_UI
#include "sky.h"
#endif //#if MMI_TYPE == SKY_UI
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
LOCAL nv_cmd_type  wap_nv_cmd_buf;
 
#define INITURL_TIMEOUT 60*60

/////////////////////////////////////////////////////////////////
unsigned long g_AnimationTime = 200;
unsigned long g_NowrapTime    = 1000;
extern void RestartAnimationTimer(unsigned long ms);
extern void CancelAnimationTimer(void);
extern void RestartNowrapTimer(unsigned long ms);
extern void CancelNowrapTimer(void);
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/* Timer for kicking the watchdog.  Bad doggie! */
rex_timer_type  wap_rpt_timer;			/* Timer for polling the received events */
rex_timer_type  wap_control_timer;
rex_timer_type  wap_animation_timer;    /* Timer for SIS animation   */
rex_timer_type  wap_nowrap_timer;       /* Timer for Nowrap          */
#ifdef SOCKET_AUWAP
rex_timer_type  wait_ani_timer;			/* Timer for animaton */
#endif //SOCKET_AUWAP
/////////////////////////////////////////////////////////////////


/* Allocate a buffer to hold the queue elements  */
wap_cmd_type wap_buffers[ WAP_CMD_BUFFERS ];

/* Define queue head */
q_type           wap_cmd_q;      /* WAP command queue              */
q_type           wap_cmd_free_q; /* WAP command free queue         */

rex_sigs_type wap_sig_mask = 0;
#ifdef SOCKET_AUWAP

rex_sigs_type wap_sig_mask1 = (WAP_RPT_TIMER_SIG | WAP_SIG |TASK_STOP_SIG | WAP_CMD_Q_SIG | WAP_ANIMATION_SIG |WAP_BAR_SIG | NET_CB_SIG | SOCK_CB_SIG | WAIT_ANI_SIG | READ_EVT_SIG);

#else //SOCKET_AUWAP

#ifdef WAP_NOWRAP
rex_sigs_type wap_sig_mask1 = (WAP_RPT_TIMER_SIG | WAP_SIG |TASK_STOP_SIG | WAP_CMD_Q_SIG | WAP_ANIMATION_SIG | WAP_NOWRAP_TIMER_SIG |WAP_BAR_SIG);
#else
rex_sigs_type wap_sig_mask1 = (WAP_RPT_TIMER_SIG | WAP_SIG |TASK_STOP_SIG | WAP_CMD_Q_SIG | WAP_ANIMATION_SIG|WAP_BAR_SIG);
#endif//#ifdef WAP_NOWRAP

#endif //SOCKET_AUWAP
rex_sigs_type wap_sig_mask2 = (WAP_RPT_TIMER_SIG | TASK_STOP_SIG | WAP_CMD_Q_SIG|WAP_BAR_SIG);


/* Determine initializing browser */
boolean	check_browser_init = TRUE;
boolean wap_poweron = TRUE;
ip4a wapip;   /* Our IP address */

#ifdef SKTELETCH
	boolean savedns = FALSE;
#endif//#ifdef SKTELETCH
   
#ifdef WAP_USE_SECURE
	wap_handshake_state handShakestate = HAND_NULL_STATE;
#endif//#ifdef WAP_USE_SECURE

void wap_set_exit(void)
{
	rex_clr_sigs(&wap_tcb,WAP_SIG);	
	rex_clr_timer(&wap_control_timer); /* Remove polling timer from the active timer list */
	wap_sig_mask = wap_sig_mask2;
}

void wap_set_start(void)
{
	rex_clr_sigs(&wap_tcb,WAP_SIG);
	(void) rex_set_timer( &wap_control_timer,5 );
	wap_sig_mask = wap_sig_mask1;	
}

/* EJECT */
/*===========================================================================
FUNCTION BROWSER_INIT

DESCRIPTION
  
RETURN VALUE
  None

DEPENDENCIES
  
===========================================================================*/
extern const    memSizeType   cCacheSize;
extern const    memSizeType   crestoredSize;
extern int	    wap_mode ;


extern INT16 wip_initmalloc (void *mem, UINT32 memsize);
extern void InitWipMalloc(void);

void browser_init( void )
{
    extern boolean  load_data;
#ifdef WAP_USE_LEAK
	extern BOOL blocknullfree;
#endif//#ifdef WAP_USE_LEAK
	extern AddressAssign(void);

#ifndef WAP_USE_INIT
    CLNTa_log(1,0,"browser_init started");

    AddressAssign();
	InitWipMalloc();

    CLNTc_start();
#endif//#ifndef WAP_USE_INIT
    MMIc_openView(VIEW_ID_WML, WML_UA);
    MEMc_initCache(cCacheSize, crestoredSize);
  
  CLNTc_setIntConfig(0, configBEARER, BEARER_UDP);
  CLNTc_setStrConfig(0, configUDP_IP_SRC, "\x11\x22\x33\x44", 4);
  CLNTc_setStrConfig(0, configUDP_IP_GW, "\x55\x66\x77\x88", 4);
  CLNTc_setStrConfig(0, configSMS_C, "460703123450", 12);
  CLNTc_setStrConfig(0, configSMS_GW, "460703123451", 12);
  CLNTc_setIntConfig(0, configHISTORY_SIZE, 15);
  

  /*
  ** TIMEOUT value should be considerd to 30 because there is example in AUS Browser manual
  */
#ifdef BROWSER_UPDATE
  CLNTc_setIntConfig(0, configTIMEOUT, 40);
#else
  CLNTc_setIntConfig(0, configTIMEOUT, 50);
#endif//#ifdef BROWSER_UPDATE
 
  CLNTc_setIntConfig(0, configCACHE_MODE,1);
  CLNTc_setIntConfig(0, configCACHE_AGE, INITURL_TIMEOUT);
   
  CLNTc_setIntConfig(0, configDISPLAY_IMAGES, TRUE);
  CLNTc_setIntConfig(0, configUPDATE_IMAGES, FALSE);

  /*
  ** WAP WSP ISO 639 language assignment code : Korean 0x3D
  */
  CLNTc_setStrConfig(0, configWSP_Language, "\x3D", 1);


#ifdef BROWSER_UPDATE
  CLNTc_setIntConfig(0, configSTACKMODE, MODE_CO_WSP);
  wap_mode =1;	
#else
  CLNTc_setIntConfig(0, configSTACKMODE, MODE_CL_WSP);
  wap_mode = 0;
#endif //#ifdef USE_MODE_CL_WSP

  MSG_ERROR("wap mode =%d",wap_mode,0,0);

  CLNTc_initialised(); 

    load_data = TRUE;
#ifdef WAP_USE_LEAK
  blocknullfree = FALSE;
#endif//#ifdef WAP_USE_LEAK
}


/*===========================================================================
FUNCTION WAP_INIT_BROWSER

DESCRIPTION
  
DEPENDENCIES

===========================================================================*/
void wap_init_browser(void)
{
	CLNTa_log(1,0,"------WAP Browser started------");
	if( check_browser_init) 
    {
		browser_init();
		check_browser_init = FALSE;
	} 
}


/*===========================================================================
FUNCTION WAP_INIT

DESCRIPTION

    1.start wap browser
    2.define timer for wap brower execution control
    3.check memory initialization
    4.check, set configurations
    5.notify configurations
  
RETURN VALUE
  None

DEPENDENCIES
  
===========================================================================*/
extern void initCache(void);

/////////////////////////////////////////////////////////////////////////////////////

extern clk_cb_type clk_timer;


/////////////////////////////////////////////////////////////////////////////////////

static void wap_init( void )
{
  int i;
  SKY_SET_IN_WAP(FALSE); //ui.wap = FALSE;
#ifdef PORTING_B0_WAP  
  ui.wtlslength = 0;
#endif 
#ifdef WAP_USE_DORM
  SKY_SET_WAP_DORM(FALSE);//  ui.wap_dorm_mode = FALSE;
#endif//#ifdef WAP_USE_DORM

#ifdef WAP_USE_PZID2
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
  SKY_SET_WAP_PZID(FALSE);
#else
  ui.wap_pzid = FALSE;
#endif //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
#endif//#ifdef WAP_USE_PZID

#ifdef WAP_USE_SKT_NEW
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)|| (MODEL_ID == MODEL_B2)
  SKY_SET_WAP_RELEASE(FALSE);
#else
  ui.wap_release = FALSE;
#endif //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
#endif//#ifdef WAP_USE_SKT_NEW

  MSG_ERROR("wap_init started",0,0,0);
#ifdef SOCKET_AUWAP
  rex_def_timer( &wait_ani_timer, &wap_tcb, WAIT_ANI_SIG );
#endif //SOCKET_AUWAP

  /** timer for watch dog **/
  rex_def_timer( &wap_rpt_timer, &wap_tcb, WAP_RPT_TIMER_SIG );
  rex_def_timer( &wap_animation_timer, &wap_tcb, WAP_ANIMATION_SIG );
  rex_def_timer( &wap_nowrap_timer, &wap_tcb, WAP_NOWRAP_TIMER_SIG );

  mc_task_start( WAP_RPT_TIMER_SIG, DOG_WAP_RPT, &wap_rpt_timer );
    
  /* Initialize queues */
  (void) q_init( &wap_cmd_q );
  (void) q_init( &wap_cmd_free_q );

  /* Initialize each element of the free queue and build the free queue */
  for (i=0; i < WAP_CMD_BUFFERS; i++) 
  {
	wap_buffers[i].hdr.done_q_ptr = &wap_cmd_free_q;
    q_put( &wap_cmd_free_q,
           q_link( &wap_buffers[i],
                   &wap_buffers[i].hdr.link ));
  }
  
  initCache();	 

  //jjkyg 11/5
  rex_def_timer( &wap_control_timer, &wap_tcb, WAP_SIG );	

#ifdef WAP_USE_INIT  
  CLNTa_log(1,0,"browser_init started");
  InitWipMalloc();
  CLNTc_start();
#endif//#ifdef WAP_USE_INIT  

  ///////////////////////////////////////////////////////////////////////////////

  clk_def(&clk_timer); /* Initilize callback */

  ///////////////////////////////////////////////////////////////////////////////

  wap_sig_mask = wap_sig_mask1;

  //for allocate IP address...
  wapip = 0;
}

/*===========================================================================

FUNCTION wap_check_and_run()

DESCRIPTION
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void wap_check_and_run(void)
{
	if( CLNTc_wantsToRun() ) {
		CLNTc_run();
	}
}

#ifdef SOCKET_AUWAP
///////////////////////////////////////////////
//////////////// AUWAP ////////////////////////
///////////////////////////////////////////////
extern void displayloading(int catagory);
extern void received_request();
extern UINT8 g_nAniBarCount;
LOCAL int wait_ani_time = 1000;
LOCAL int wait_ani_type = 1000;
 
byte wait_animation =0;
void wait_ani_start(int type, int time)
{
	wait_ani_type = type;
	wait_ani_time = time;

	g_nAniBarCount = 0;
	wait_animation = 1;
	(void) rex_set_timer( &wait_ani_timer, time );
	displayloading( type );
}

void wait_ani_stop(void)
{
	(void) rex_clr_sigs( &wap_tcb, WAIT_ANI_SIG );
	(void) rex_clr_timer( &wait_ani_timer);
	wait_animation = 0;
}
///////////////////////////////////////////////
#endif //SOCKET_AUWAP
/* <EJECT> */
/*===========================================================================
FUNCTION WAP_TASK

DESCRIPTION
  The main wap function.  

DEPENDENCIES
  
RETURN VALUE

SIDE EFFECTS
  
===========================================================================*/
void wap_task (
  dword dummy                       /* lint -esym(715,dummy) */
                                    /* ignore fact that this isn't used */
)
{ 
  rex_sigs_type	 sigs;        /* hold signals */
  static rex_tcb_type		*ctask_ptr;  /* Pointer to requesting task TCB */
  static rex_sigs_type		csigs;       /* Signal to set upon cmd completion */
  static rex_sigs_type		ccsigs;       /* Signal to set upon cmd completion */
  wap_cmd_type	 *cmd_ptr;       /* get command from WAP command queue */

  /* 1.start wap browser
  ** 2.define timer for wap brower execution control
  ** 3.check memory initialization
  ** 4.check, set configurations
  ** 5.notify configurations
  */
  
  MSG_ERROR("wap task started",0,0,0);

  wap_init();

  for( ;; ) 
  {

	/* receiving signals
	** 1. watchdog signal
	** 2. wap event execution control signal
	** 3. power down signal
	** 4. command execution signal from other tasks
	*/
	
	ccsigs = 0;

    ccsigs = rex_wait(wap_sig_mask);
	sigs   = ccsigs & wap_sig_mask;

#ifdef SOCKET_AUWAP
	if((sigs & SOCK_CB_SIG) != 0)
	{ 
        MSG_HIGH( "SOCK_CB_SIG recv", 0, 0, 0);
		rex_clr_sigs(&wap_tcb, SOCK_CB_SIG);
		proc_sockcb();
	}

	if((sigs & NET_CB_SIG) != 0)
	{ 
        MSG_HIGH( "NET_CB_SIG recv", 0, 0, 0);
		rex_clr_sigs(&wap_tcb, NET_CB_SIG);
		proc_netcb();
	}

	/* Play wait animation AUWAP */
    if( sigs & WAIT_ANI_SIG ) {
		(void) rex_clr_sigs( &wap_tcb, WAIT_ANI_SIG );
		(void) rex_set_timer( &wait_ani_timer, wait_ani_time );
		displayloading(wait_ani_type);
	}

	/* Processing the received data */
	if((sigs & READ_EVT_SIG) != 0)
	{ 
        MSG_HIGH( "READ_EVT_SIG recv", 0, 0, 0);
		rex_clr_sigs(&wap_tcb, READ_EVT_SIG);
		received_request();
	}
#endif //SOCKET_AUWAP

	if( sigs & WAP_BAR_SIG)
	{
		extern void process_draw_bar(void);
		(void) rex_clr_sigs( &wap_tcb, WAP_BAR_SIG );
		//process_draw_bar();
	}	

#ifdef WAP_NOWRAP	//yjahn96 00.10.27
	if( sigs & WAP_NOWRAP_TIMER_SIG)
	{
		extern void Support_NoWrapping(void);

		(void) rex_clr_sigs( &wap_tcb, WAP_NOWRAP_TIMER_SIG );
		(void) rex_set_timer( &wap_nowrap_timer, g_NowrapTime );

		Support_NoWrapping();
	}
#endif //#ifdef WAP_NOWRAP

	if( sigs & WAP_ANIMATION_SIG) //wap task
	{
		extern void Play_SIS_Animation(void);

		(void) rex_clr_sigs( &wap_tcb, WAP_ANIMATION_SIG );
		(void) rex_set_timer( &wap_animation_timer, g_AnimationTime );
		Play_SIS_Animation();
	}

    /* Report back to the watchdog.  Whack!  Yipe yipe yipe... */
    if( sigs & WAP_RPT_TIMER_SIG ) {
		(void) rex_clr_sigs( &wap_tcb, WAP_RPT_TIMER_SIG );
		//MSG_ERROR("WAP DOGG", 0, 0, 0);
		(void) rex_set_timer( &wap_rpt_timer, DOG_WAP_RPT_TIME );
		dog_report( DOG_WAP_RPT );
	}

	/* Check the availibility of WAP Browser, run the evnt */
	if( sigs & WAP_SIG ) {

		(void) rex_clr_sigs( &wap_tcb, WAP_SIG );
		wap_check_and_run();
		(void) rex_set_timer( &wap_control_timer,5);
    }

	/* Command queue signal */
    if( sigs & WAP_CMD_Q_SIG ) 
	{
		(void) rex_clr_sigs( &wap_tcb, WAP_CMD_Q_SIG );
			
		/* Handle commands on queue till the queue is empty */
		while( ( cmd_ptr = (wap_cmd_type *)q_get( &wap_cmd_q )) != NULL ) 
		{
			wap_get_cmd( cmd_ptr ); 
			wap_check_and_run();
			
	        ctask_ptr = cmd_ptr->hdr.task_ptr; /* save for when the buffer */
		    csigs = cmd_ptr->hdr.sigs;         /* is back on the queue */

			if( cmd_ptr->hdr.done_q_ptr ) 
			{  /* Put it back on a queue */
				q_put( cmd_ptr->hdr.done_q_ptr, &cmd_ptr->hdr.link );
			}
			if( ctask_ptr )
			{         /* Signal back to the calling task */
				(void) rex_set_sigs( ctask_ptr, csigs );
			}
		}
    }

	/* Stop task - nothing special to do */
    if( ( sigs & TASK_STOP_SIG ) != 0 ) 
	{
		MSG_MED(">Power down case detected....", 0, 0, 0);
		(void) rex_set_sigs( &mc_tcb, MC_ACK_SIG );
		(void) rex_clr_sigs( rex_self(), TASK_STOP_SIG );
    }
	sigs = 0;ccsigs = 0;

  }//End of for loop
} 

void wap_nv_wait
(
  wap_sigs_type requested_mask       
)
{
    static wap_cmd_type    *cmd_ptr;    /* get command from UI command stack */
    static rex_tcb_type    *ctask_ptr;  /* Pointer to requesting task TCB    */
    static rex_sigs_type    csigs;      /* Signal to set upon cmd completion */
 
	byte dog_report_cnt;	
	wap_sigs_type rex_signals_mask;

	dog_report_cnt = 0;
  
	for( ;; ) 
	{                   
		rex_signals_mask = rex_wait(WAP_RPT_TIMER_SIG | WAP_SIG	| WAP_CMD_Q_SIG |WAP_ANIMATION_SIG | WAP_BAR_SIG |WAP_NOWRAP_TIMER_SIG|requested_mask);

	#ifdef WAP_NOWRAP	//yjahn96 00.10.27
		if( rex_signals_mask & WAP_NOWRAP_TIMER_SIG)
		{
			extern void Support_NoWrapping(void);
			(void) rex_clr_sigs( &wap_tcb, WAP_NOWRAP_TIMER_SIG );
			(void) rex_set_timer( &wap_nowrap_timer, g_NowrapTime );
			Support_NoWrapping();
		}
	#endif //#ifdef WAP_NOWRAP
	
        if( rex_signals_mask & WAP_RPT_TIMER_SIG ) 
		{
#ifdef SW_RESET_ENABLE				
			extern void SW_Reset(void);
#endif//#ifdef SW_RESET_ENABLE	
			(void) rex_clr_sigs( &wap_tcb, WAP_RPT_TIMER_SIG );
			(void) rex_set_timer( &wap_rpt_timer, DOG_WAP_RPT_TIME );
			dog_report( DOG_WAP_RPT );
			if (dog_report_cnt++ >= 5)
			{
			#ifdef SW_RESET_ENABLE	
				SW_Reset();
			#else
				return;
			#endif//#ifdef SW_RESET_ENABLE
			}			
	    }

		if( rex_signals_mask & WAP_SIG)
		{
			(void) rex_clr_sigs( &wap_tcb, WAP_SIG );
			wap_check_and_run();
			(void) rex_set_timer( &wap_control_timer,5);
		}

		
		if ( rex_signals_mask & WAP_CMD_Q_SIG )
		{
			(void) rex_clr_sigs( &wap_tcb, WAP_CMD_Q_SIG );
		
			while((cmd_ptr = (wap_cmd_type *)q_get( &wap_cmd_q )) != NULL ) 
			{
				wap_get_cmd( cmd_ptr );	wap_check_and_run();
				ctask_ptr = cmd_ptr->hdr.task_ptr;csigs = cmd_ptr->hdr.sigs;        

				if( cmd_ptr->hdr.done_q_ptr ) 
				{ 
					q_put( cmd_ptr->hdr.done_q_ptr, &cmd_ptr->hdr.link );
				}
				if( ctask_ptr )
				{
					(void) rex_set_sigs( ctask_ptr, csigs );
				}
			}
		}

		if( rex_signals_mask & WAP_ANIMATION_SIG) 
		{
			extern void Play_SIS_Animation(void);
			(void) rex_clr_sigs( &wap_tcb, WAP_ANIMATION_SIG );
		    (void) rex_set_timer( &wap_animation_timer, g_AnimationTime );
			Play_SIS_Animation();
		}

		if(rex_signals_mask & WAP_BAR_SIG)
		{
			extern void process_draw_bar(void);
			(void) rex_clr_sigs( &wap_tcb, WAP_BAR_SIG );
			process_draw_bar();
		}
		
		if(rex_signals_mask & requested_mask)
		{
			(void) rex_clr_sigs( &wap_tcb, requested_mask );
			return;
		}		
	}//End of for( ;; ) 
}

nv_stat_enum_type wap_process_nv_item
(
  nv_items_enum_type  item_code,       /* Item to get                      */
  nv_item_type        *item_ptr,        /* Pointer where to put the item    */
  boolean              read			   /* read = TRUE; READ_F, read = FALSE;WRITE_F */
)
{
  if(read)
  {
	wap_nv_cmd_buf.cmd        = NV_READ_F;             /* Read request        */
	CLNTa_log(1,0,"READ NV processing in WAP");
  }
  else
  {
	wap_nv_cmd_buf.cmd        = NV_WRITE_F;             /* Read request        */
	CLNTa_log(1,0,"WRITE NV processing in WAP");
  }

  wap_nv_cmd_buf.tcb_ptr    = &wap_tcb;              /* Notify back to me   */
  wap_nv_cmd_buf.sigs       = WAP_NV_SIG;			 /* With this signal    */
  wap_nv_cmd_buf.done_q_ptr = NULL;                  /* No buffer to return */
  wap_nv_cmd_buf.item       = item_code;             /* Item to get         */
  wap_nv_cmd_buf.data_ptr   = item_ptr;              /* Where to return it  */

#if	MMI_TYPE == MMI_SK
  if( item_code == NV_DIAL_I )
  {       /* check for speed dial */
    item_ptr->dial.address -= 1;   /* update to real nv address */
  }
#endif

  (void) rex_clr_sigs(&wap_tcb, WAP_NV_SIG);	   /* Clear signal for NV  */
  nv_cmd(&wap_nv_cmd_buf);                      /* Issue the request    */
  (void)wap_nv_wait( WAP_NV_SIG);              /* Wait for completion  */

  if( wap_nv_cmd_buf.status != NV_DONE_S &&
      wap_nv_cmd_buf.status != NV_NOTACTIVE_S )
  {
    CLNTa_log(1,0,"NV Read Failed Item %d Code %d",wap_nv_cmd_buf.item, wap_nv_cmd_buf.status);
  }

#if	MMI_TYPE == MMI_SK
  if ( item_code == NV_DIAL_I )
  {       /* check for speed dial */
    item_ptr->dial.address += 1;   /* restore to user address */
  }
#endif
  return( wap_nv_cmd_buf.status);
}


void CancelAnimationTimer(void)
{
	rex_clr_timer(&wap_animation_timer);
}

void RestartAnimationTimer(unsigned long ms)
{
	g_AnimationTime = ms;

	rex_clr_timer(&wap_animation_timer);
	(void) rex_clr_sigs( &wap_tcb, WAP_ANIMATION_SIG);
	if(ms)
	{
		rex_set_timer(&wap_animation_timer, ms);
	}
}

void CancelNowrapTimer(void)
{
	rex_clr_timer(&wap_nowrap_timer);
}

void RestartNowrapTimer(unsigned long ms)
{
	g_NowrapTime = ms;

	rex_clr_timer(&wap_nowrap_timer);
	(void) rex_clr_sigs( &wap_tcb, WAP_NOWRAP_TIMER_SIG);
	if(ms)
	{
		rex_set_timer(&wap_nowrap_timer, ms);
	}
}
