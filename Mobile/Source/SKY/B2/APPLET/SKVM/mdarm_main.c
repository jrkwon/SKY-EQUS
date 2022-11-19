/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
		MDARM_MAIN.C
GENERAL DESCRIPTION
  This Software unit contains all scheduling functions and system events 
  for XVM engine.This includes all REX Interfacing functions and XVM engine 
  Interfacing functions.
  
EXTERNALIZED FUNCTIONS
	
INITIALIZATION AND SEQUENCING REQUIREMENTS

   Copyright (c) 2001 by SK Teletech Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/******************************************************************/
/*	   include Header Files				                          */
/******************************************************************/

#include "extern.h" 
#include "ska.h" 
#include "sky.h"
#include "mdarm_def.h"
#include "sky_sock.h"
#include "graphics.h"
#include "cai.h"

 
/******************************************************************/
/*	   define constans and macro		                          */
/******************************************************************/
#ifdef SKVM_IN_UI_TASK
	#define	XVM_TCB			(&ui_tcb)
#else
	#define	XVM_TCB			(&vas_tcb)
#endif

#ifdef SKVM_IN_UI_TASK
#define SKVM_DO_EVENT(e)		{SKY_DoEvent(e);}
#else
#define SKVM_DO_EVENT(e)		{SKY_InitKey( &ui_tcb, UI_KEY_SIG );	SKVMSendEvent2UI(e);}
#endif

#define	FD_HASH(x)	((x) & 0x1f)
#define	END_KEY	-2

#ifndef TEST_TORTOL
#define TEST_TORTOL
#endif
/******************************************************************/
/*	   grobal variables					                          */
/******************************************************************/
short   xsig_flags;
short   xsig_flags_skt = 0x0000;
int     sound_play_result;
int     gvm_app_idx;
BOOL	music_play_in_skvm = FALSE;
BOOL	flip_close_status = FALSE;
ALARM_DISP_TYPE_T           AlarmTypeInSKVM;
BOOL	g_bxvm_sig_timer2 = FALSE;

#ifdef SUPPORT_REX_TIMER_IN_SKVM
rex_timer_type xvm_sig_timer1;
rex_timer_type xvm_sig_timer2;
rex_timer_type xvm_sig_timer3;
#endif //SUPPORT_REX_TIMER_IN_SKVM

/******************************************************************/
/*	   static variables					                          */
/******************************************************************/
static	clk_cb_type	sleep_cb;
static	clk_cb_type	delay_cb;
static	clk_cb_type	timer1_cb;
static	clk_cb_type	timer2_cb;
static	clk_cb_type	annun_timer_cb;
static	clk_cb_type	dog_sleep_rpt_cb;
static	clk_cb_type	delay_cb;
static	clk_cb_type	pppclose_cb;
static	int			RAM pressed_keycode = -1;
static	qword		RAM stop_pressed_time;
static  unsigned int checkeventcnt = 0;
static  BOOL		g_bRefreshAnnun = FALSE;

/* added by sinclair for MUX */
int java_data_so = 1;

/******************************************************************/
/*	   extern variables					                          */
/******************************************************************/
extern int	g_WouldBlock;
extern BOOL	g_bWouldBlockTimer;

//Temporarily added by swjung for WAP_B2 TEST
extern CHAR		  gWAPinit_url[130];
extern unsigned long	  g_lpreservetime;
//

/******************************************************************/
/*	   extern functions					                          */
/******************************************************************/
extern	void  mdarm_get_gap(qword*);
extern	void  mdarm_set_lock_SharedBuffer(short i);
extern	void  mdarm_fd_init(void);
extern	void  mdarm_fd_finalize(void);
extern	int	  md_finalizesocket(void);

// SKVM Core external functions
extern	void  enqueKeyDownEvent( int keyCode );
extern	void  enqueKeyUpEvent( int keyCode );
extern	void  enqueTimerEvent( int action );
extern	void  enqueStopEvent( void );
extern	void  xvm_signal_sound_play_end(int ret);
extern	void  _jexit(int status);
extern  int   xvm_run(void);   
extern  void  xvm_seturl(char *msdURL,char *retURL,int svcOption);
extern  void  xvm_user_pause(void);
extern  void  xvm_user_resume(int repaint);
extern  void  xvm_process_applicationdata(unsigned char * msg,int msgLen);
extern  void  xvm_setargs(int argc,char *argv[]);
extern  int   xvm_nextargs(void);
extern  void  xvm_user_stop(void);
extern void   xvm_dereg_annun(void);
extern void   xvm_reg_annun(void);
extern void	  DrawAnnunciator(BOOL f);
/******************************************************************/
/*     function declare											  */
/******************************************************************/
static	void xvm_dog_sleep_rpt_cb(int4 interval);
void SKVMSendEvent2UI(UI_COMMAND_E cmd);
/******************************************************************/
/*     function define											  */
/******************************************************************/

void xvm_dereg_annun(void){
	clk_dereg( &annun_timer_cb);
}

static void xvm_draw_annun_cb(int4 interval)
{
    XSIG_SKT_SET(UPDATE_ANNU_XSIG);
}

void xvm_reg_annun(void){
	g_bRefreshAnnun = FALSE;
	clk_def( &annun_timer_cb);
	clk_reg( &annun_timer_cb, xvm_draw_annun_cb,(int4)1,1000L,TRUE);
}

void md_init(void)
{
    qword gap;

	mdarm_get_gap(&gap); 
	//fd_init();
	mdarm_fd_init();
    // for correct time system don't delete this line by tortol 01.04.30
	qw_set(gap, 0x49, 0x8f09d980); /* 315932400000 */
    xsig_flags = 0x0000;
    //lock_SharedBuffer = 0;
    mdarm_set_lock_SharedBuffer(0);
	pressed_keycode = -1;
	flip_close_status = FALSE;
    clk_def( &sleep_cb );
    clk_def( &timer1_cb );
    clk_def( &timer2_cb );
	clk_def( &delay_cb );
	clk_def( &pppclose_cb );
	clk_def( &dog_sleep_rpt_cb);
	java_data_so = 0;

	SetKindOfService(JAVA);
	SKVM_SET_MUSIC_PLAY(FALSE);
	SKVM_SET_DORMANT(FALSE);	
	clk_reg( &dog_sleep_rpt_cb,xvm_dog_sleep_rpt_cb,(int4)1,DOG_SLEEP_RPT_TIME,TRUE);
	
	g_bxvm_sig_timer2 = FALSE;
#ifdef SUPPORT_REX_TIMER_IN_SKVM
    (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER1_SIG); /* Clear the signal */
    (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER2_SIG); /* Clear the signal */
    (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER3_SIG); /* Clear the signal */
#endif // SUPPORT_REX_TIMER_IN_SKVM
}

void md_finalize(void)
{
    int ret;
	//fd_finalize();
	
	SKY_StopMusic();	
    mdarm_fd_finalize();
	ret = md_finalizesocket();
    clk_dereg( &sleep_cb );
    clk_dereg( &timer1_cb );
    clk_dereg( &timer2_cb );
	clk_dereg( &delay_cb );
	clk_dereg( &dog_sleep_rpt_cb);
	clk_dereg( &pppclose_cb );
    rex_clr_sigs(XVM_TCB, XVM_TIMERS_SIG);
    rex_clr_sigs(XVM_TCB, XVM_TIMERS2_SIG);

#ifdef SUPPORT_REX_TIMER_IN_SKVM
    (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER1_SIG); /* Clear the signal */
    (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER2_SIG); /* Clear the signal */
    (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER3_SIG); /* Clear the signal */
#endif // SUPPORT_REX_TIMER_IN_SKVM

	
	if (SKVM_IS_DOWNLOAD_MODE())
		SKVM_SET_DOWNLOAD_MODE(FALSE);
	else 
	{	// 호가 연결된 상태에서 컨텐츠가 종료될 때...
		// md_finalize()에서 임의로 호를 끊을 수 있도록 setting한다
		if (NetworkGetState() == NET_PPPOPEN_S)
		{
			if (md_pppclose()){
				XSIG_CLR(DS_PPP_XSIG);
				CLOSE_NETLIB();
			}
			else XSIG_SKT_SET(DS_PPP_XSIG);
		}
	}
}

static void xvm_timer1_cb (int4 interval)
{
    XSIG_SET(TIMER1_XSIG);
    rex_set_sigs(XVM_TCB, XVM_TIMERS_SIG);
}

static void xvm_timer2_cb (int4 interval)
{

    XSIG_SET(TIMER2_XSIG);
    rex_set_sigs(XVM_TCB, XVM_TIMERS_SIG);

}

static void xvm_dog_sleep_rpt_cb(int4 interval)
{
	dog_report(
		DOG_SLEEP_RPT|
		DOG_NV_RPT|
		DOG_UI_RPT
#ifdef FEATURE_DS
		|DOG_DS_RPT
		|DOG_PS_RPT
#endif
#ifdef T_AUTH
		|DOG_AUTH_RPT
#endif
#ifdef FEATURE_DH
		|DOG_DH_RPT
#endif
#ifdef FEATURE_EFS
		|DOG_FS_RPT
#endif
#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/06
		|DOG_SOCK_RPT
#endif
#ifdef FEATURE_IS2000_TDSO
		|DOG_TDSO_RPT
#endif
		);
}

static void xvm_ppp_close_cb(int4 interval)
{
    clk_dereg( &pppclose_cb );
	g_lpreservetime = 0;
    XSIG_SET(DS_PPP_XSIG);
}

void xvm_cancel_md_ppp_preserve_time()
{
    clk_dereg( &pppclose_cb );
	XSIG_CLR(DS_PPP_XSIG);
	g_lpreservetime = 0;
}

void xsig_sound_play_end(int ret)
{

	XSIG_SET(SND_END_XSIG);
	sound_play_result =ret;
	SKVM_SET_MUSIC_PLAY(FALSE);
	xvm_signal_sound_play_end(sound_play_result);
}

void xvm_set_md_ppp_preserve_time(unsigned long t)
{
	clk_reg( &pppclose_cb, xvm_ppp_close_cb, (int4)t, 0L, FALSE);
}

void xvm_pppclose()
{
	if (md_pppclose()){
		XSIG_CLR(DS_PPP_XSIG);
		CLOSE_NETLIB();
	}
}
////////////////////////
/////////////////////////
/* msec == 0 : direct call */
void md_setalarm(int task, unsigned long msec)
{

    switch(task) {
    case 1:
		if (msec == 0)
			xvm_timer1_cb(0);
		else
			clk_reg( &timer1_cb, xvm_timer1_cb, (int4)msec, 0L, FALSE);
		break;
    case 2:
		if (msec == 0)
			xvm_timer2_cb(0);
		else
			clk_reg( &timer2_cb, xvm_timer2_cb, (int4)msec, 0L, FALSE);
		break;
	default:
		break;
    }

}

void md_killalarm(int task)
{

    switch(task) {
    case 1:
		clk_dereg( &timer1_cb );
		break;
    case 2:
		clk_dereg( &timer2_cb );
		break;
	default:
		break;
    }

}


static int convertKeyCode(SKY_EVENT_E key )
{

	switch( key )
	{
	case SE_KEY_POUND: return KEY_POUND;
	case SE_KEY_STAR: return KEY_STAR;
	case SE_KEY_0: return KEY_NUM0;
	case SE_KEY_1: return KEY_NUM1;
	case SE_KEY_2: return KEY_NUM2;
	case SE_KEY_3: return KEY_NUM3;
	case SE_KEY_4: return KEY_NUM4;
	case SE_KEY_5: return KEY_NUM5;
	case SE_KEY_6: return KEY_NUM6;
	case SE_KEY_7: return KEY_NUM7;
	case SE_KEY_8: return KEY_NUM8;
	case SE_KEY_9: return KEY_NUM9;

	case SE_KEY_SEND: return KEY_CALL;
	case SE_KEY_END: 
	case SE_KEY_PHONE_CLOSE:
		return KEY_END;
	case SE_KEY_CLEAR: return KEY_CLR; 
	case SE_KEY_F2: return KEY_COMR; 
	case SE_KEY_F1: return KEY_COML; 

	case SE_KEY_UP: return KEY_UP;
	case SE_KEY_DOWN: return KEY_DOWN;
	case SE_KEY_LEFT: return KEY_LEFT;
	case SE_KEY_RIGHT: return KEY_RIGHT;
	case SE_KEY_CENTER: return KEY_FIRE;
    case SE_KEY_RECORD: return KEY_VOL_UP; 
	default:
		return (0x8000 | key);
	}

}

int md_GameActionToKeyCode(int keyCode )
{
	switch(keyCode ){
	case ACT_UP:return KEY_UP;
	case ACT_LEFT:return KEY_LEFT;
	case ACT_RIGHT:return KEY_RIGHT;
	case ACT_DOWN:return KEY_DOWN;
	case ACT_FIRE:return KEY_FIRE;
	case ACT_GAME_A:return KEY_NUM1;
	case ACT_GAME_B:return KEY_NUM3;
	case ACT_GAME_C:return KEY_NUM7;
	case ACT_GAME_D:return KEY_NUM9;
	default:return -1;
	}
}
int md_KeyCodeToGameAction(int actionCode )
{
	switch(actionCode ){
	case KEY_UP:return ACT_UP;
	case KEY_LEFT:return ACT_LEFT;
	case KEY_RIGHT:return ACT_RIGHT;
	case KEY_DOWN:return ACT_DOWN;
	case KEY_FIRE:return ACT_FIRE;
	case KEY_NUM1:return ACT_GAME_A;
	case KEY_NUM3:return ACT_GAME_B;
	case KEY_NUM7:return ACT_GAME_C;
	case KEY_NUM9:return ACT_GAME_D;
	default:return 0;
	}
}


void md_pause(void)
{
    rex_clr_sigs(XVM_TCB, XVM_TIMERS_SIG);
    rex_clr_sigs(XVM_TCB, XVM_TIMERS2_SIG);

    while ( (SKY_EVENT_E) SKY_GetKey() != SE_KEY_NONE)
	; // popup key buffer
}

void md_resume(void)
{

    while ( (SKY_EVENT_E) SKY_GetKey() != SE_KEY_NONE)
	; // popup key buffer

    if (pressed_keycode != -1) {
		enqueKeyUpEvent(pressed_keycode);
		pressed_keycode = -1;
    }

    if (flip_close_status) {
		enqueKeyDownEvent(KEY_FLIP_CLOSE);
		enqueKeyUpEvent(KEY_FLIP_CLOSE);
    }
    if (xsig_flags & (TIMER1_XSIG|TIMER2_XSIG)) {
	rex_set_sigs(XVM_TCB, XVM_TIMERS_SIG);
    }
}

 
int md_processevent(void)
{
	SKY_EVENT_E  key;          
    int		keycode;
    rex_sigs_type sig;
    NV_ITEM_U nv_data;

	if (xsig_flags &(DS_PPP_XSIG|SND_END_XSIG)){
		if (XSIG_TEST(DS_PPP_XSIG)){
			if (md_pppclose()){
				XSIG_CLR(DS_PPP_XSIG);
				CLOSE_NETLIB();
			}
			// Dormant 지원
/*			if (P_REV_IS_6_OR_GREATER){ 
				SKVM_SET_DORMANT(TRUE);	
				SKY_EndCall();
			}
			else
			{
				xvm_pppclose();
			}
*/
		}
		if (XSIG_TEST(SND_END_XSIG)){
			XSIG_CLR(SND_END_XSIG);
//			xvm_signal_sound_play_end(sound_play_result);
		}
	}

	sig = rex_get_sigs(XVM_TCB);

	if (sig & XVM_TIMERS_SIG) {
		rex_clr_sigs(XVM_TCB, XVM_TIMERS_SIG);
		if (xsig_flags & (TIMER1_XSIG|TIMER2_XSIG)) {
			if (XSIG_TEST(TIMER1_XSIG)) {
				XSIG_CLR(TIMER1_XSIG);
				enqueTimerEvent(1);
			}
			if (XSIG_TEST(TIMER2_XSIG)) {
				XSIG_CLR(TIMER2_XSIG);
				enqueTimerEvent(2);
			}
			xsig_flags &= ~(TIMER1_XSIG|TIMER2_XSIG);
		}
    }

	if (sig & XVM_TIMERS2_SIG) {
		rex_clr_sigs(XVM_TCB, XVM_TIMERS2_SIG);
    }

	if (!SKVM_IS_PLAY_BG()) {
		if (sig & UI_KEY_SIG) {
			rex_clr_sigs(XVM_TCB, UI_KEY_SIG);
			while((key = SKY_GetKey()) != SE_KEY_NONE)
			{
				keycode = convertKeyCode(key);
				switch(key)
				{
				case SE_KEY_RELEASE:
					if (pressed_keycode == -1)
						break;
					if (pressed_keycode == END_KEY) {   // SKVM실행중 'END'를 누르면 SKVM이 종료됨 
						qword	t;
						clk_read_ms(t);
						qw_dec(t, (dword)STOP_KEY_DELAY);
						if (qw_cmp(t, stop_pressed_time) >= 0)
						{
							extern void _jexit(int status);
							enqueKeyUpEvent(pressed_keycode);
							pressed_keycode = -1;
							enqueStopEvent();
							SKY_GetKey(); /* POP HS_RELEASE_K */
							_jexit(XVMERR_NORMAL); 
							return XVMSTATE_RUNNING; /* CHECK SAFE */
						}
					}
					else clk_read_ms(stop_pressed_time);

					enqueKeyUpEvent(pressed_keycode);
					pressed_keycode = -1;
					break;
				case SE_KEY_PHONE_CLOSE:
					if(SKY_GetNV(NV_FLIP_COUNT_I, &nv_data) == NV_DONE_S)
					{
						nv_data.flip_count += 1;
						SKY_PutNV(NV_FLIP_COUNT_I, &nv_data);
					}

					/* flip or folder is close */
					SKY_SET_PHONE_CLOSE();
					/* UI_BKLT_WHEN_EARPC_UP */

					flip_close_status = TRUE;

					/* always turn off the backlight when folder is closed */
					//if(SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS)
					//{
					SKY_Backlight( FALSE );
					/* jrkwon 2001.06.28 --> don't turn on sub lcd backlight while being shut down */
					if(SKY_IS_SHUT_DOWN() == FALSE)
						SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT()); //white 2001.01.05.
											enqueKeyUpEvent(pressed_keycode);
					pressed_keycode = -1;
					enqueStopEvent();
					SKY_GetKey(); /* POP HS_RELEASE_K */
					_jexit(XVMERR_NORMAL); 
					return XVMSTATE_RUNNING; /* CHECK SAFE */

				default:
					clk_read_ms(stop_pressed_time);
					pressed_keycode = keycode;
					enqueKeyDownEvent(keycode);
					// Key Tone Generation
					if (!XSIG_TEST(SND_TONE_XSIG)){
						SKY_PlayTone(UI_BEEP_SND, SND_CTRL, 200);
					}
					// Back Light On
					if (!XSIG_TEST(BACKLIGHT_XSIG)){
						if(!SKY_GET_IGNORE_BACKLIGHT())
							SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
					}
					break;
				}
			}
		}
	    else if (pressed_keycode != -1 && pressed_keycode != END_KEY) {
			qword	t;
			clk_read_ms(t);
			qw_dec(t, (dword)REPEAT_KEY_DELAY);
			if (qw_cmp(t, stop_pressed_time) >= 0) {
				clk_read_ms(stop_pressed_time);
				qw_dec(stop_pressed_time,
					(dword)(REPEAT_KEY_DELAY - REPEAT_KEY_INTERVAL));
				enqueKeyRepeatEvent(pressed_keycode);
			}
	    }
	}
	return XVMSTATE_RUNNING;
}

void md_waitevent(void)
{
    xvm_state = XVMSTATE_SUSPEND; /* exit xvm_run() */
}

static void xvm_sleep_cb(int4 interval)
{
    clk_dereg( &sleep_cb );
    rex_set_sigs(XVM_TCB, XVM_TIMERS2_SIG);
}

static void xvm_delay_cb(int4 interval)
{
    clk_dereg( &delay_cb );
    rex_set_sigs(XVM_TCB, XVM_TIMERS_SIG);
}

void md_sleep(unsigned long delay)
{
#ifdef SKVM_IN_UI_TASK
    if (rex_self() == &ui_tcb) {
#else
    if (rex_self() == &vas_tcb) {
#endif
		rex_clr_sigs(XVM_TCB, XVM_TIMERS2_SIG);
		clk_reg(&sleep_cb, xvm_sleep_cb, (int4)delay, 0L, FALSE);
		rex_wait(XVM_TIMERS2_SIG);
		rex_clr_sigs(XVM_TCB, XVM_TIMERS2_SIG);
		//	if (!(xsig_flags & (TIMER1_XSIG|TIMER2_XSIG)))
		//	    rex_clr_sigs(XVM_TCB, XVM_TIMERS_SIG);
   }
}

static void xvm_wait_cb(int4 interval)
{
    clk_dereg( &sleep_cb );
//    rex_set_sigs(XVM_TCB, XVM_TIMERS_SIG);
    rex_set_sigs(XVM_TCB, XVM_TIMERS2_SIG);

}

void md_timedwaitevent(unsigned long delay)
{
#ifdef SUPPORT_REX_TIMER_IN_SKVM
	if (delay > 0)
		(void) rex_set_timer( &xvm_sig_timer1, delay);
    xvm_state = XVMSTATE_SUSPEND; /* exit xvm_run() */
#else
    clk_reg(&delay_cb, xvm_delay_cb, (int4)delay, 0L, FALSE);
    xvm_state = XVMSTATE_SUSPEND; /* exit xvm_run() */
#endif // SUPPORT_REX_TIMER_IN_SKVM
}


void md_checkevent(void)
{
    rex_sigs_type sig;


	// 호가 연결된 상태에서 컨텐츠가 종료될 때...
	// md_finalize()에서 임의로 호를 끊을 수 있도록 setting한다
	// md_pppclose()가 1을 return할 때까지 계속해서 md_pppclose를 호출할 수 있도록 한다.
	
	if (XSIG_SKT_TEST(DS_PPP_XSIG)){
		if (md_pppclose()){
			XSIG_SKT_CLR(DS_PPP_XSIG);
			CLOSE_NETLIB();
		}
	}

    md_processevent();
	sig = rex_get_sigs(XVM_TCB);

	// vsp task의 signal들 check 하여 
	// xvm_run을 빠져 나가 vsp signal handler가 종작 할 수 있도록 한다.
#ifdef SKVM_IN_UI_TASK
	if (sig & (
			   UI_CMD_Q_SIG |			// 0x0001
//             UI_RPT_TIMER_SIG |		// 0x0004
			   UI_MULTI_STOP_SIG |		// 0x0020
			   UI_TIMERS_SIG |			// 0x0080
//             UI_RING_SIG |			// 0x0100
			   TASK_OFFLINE_SIG | 
			   TASK_STOP_SIG|
			   UI_PLAY_REC_SIG |		// 0x0010
               UI_RECVDATA_Q_SIG		// 0x0400
#ifdef SUPPORT_REX_TIMER_IN_SKVM
				| XVM_ACTIVE_TIMER1_SIG 
				| XVM_ACTIVE_TIMER2_SIG
				| XVM_ACTIVE_TIMER3_SIG
#endif //SUPPORT_REX_TIMER_IN_SKVM
#ifdef FEATURE_SKT_EIF	//yjahn96 01.03.15
			  | UI_UPONG_SIG			
#endif	//#ifdef FEATURE_SKT_EIF
#ifdef FEATURE_VAS
				| UI_NETWORK_CB_SIG 
				| UI_SOCKET_CB_SIG 
				| UI_DNS_CB_SIG
				| UI_PS_TIMER_SIG
#endif//feature_vas
#ifdef FEATURE_WAP
                | WAP_CMD_Q_SIG | UI_EVT_PROCESS_SIG
                | WAP_ANIMATION_TIMER_SIG | WAP_NOWRAP_TIMER_SIG
				| WAP_BROWSER_TIMER_SIG
#endif //#ifdef FEATURE_WAP

				)
		)
#else
	if (sig & (
				//VAS_RPT_TIMER_SIG | 
				VAS_CMD_Q_SIG |
				TASK_OFFLINE_SIG | TASK_STOP_SIG |    
				NETWORK_CB_SIG    |
				SOCKET_CB_SIG     |
//				VAS_EVT_PROCESS_SIG |
				VAS_TIMER_PROCESS_SIG1 |
				VAS_TIMER_PROCESS_SIG2 |
				VAS_TIMER_PROCESS_SIG3 |
				VAS_TIMER_PROCESS_SIG4 
				)
		)
#endif
	{
//		if (!SKVM_IS_FIRST_REFRESH())
//		{
			rex_set_sigs(XVM_TCB,XVM_TIMERS_SIG);
			xvm_state = XVMSTATE_SUSPEND;
//		}
	}
	else
	{
#ifndef SKVM_USE_SLEEP_TIMER_IN_NETWORK
		if ((g_WouldBlock == DS_EWOULDBLOCK) && (g_bWouldBlockTimer))
		{
			if (g_bxvm_sig_timer2)
			{
#ifdef SUPPORT_REX_TIMER_IN_SKVM
			        (void) SKY_Clear_Signal( &ui_tcb, XVM_ACTIVE_TIMER2_SIG); /* Clear the signal */
#else
					clk_dereg( &sleep_cb );
#endif // SUPPORT_REX_TIMER_IN_SKVM 
			}
			g_bxvm_sig_timer2 = TRUE;
#ifdef SUPPORT_REX_TIMER_IN_SKVM
			rex_set_timer( &xvm_sig_timer2, 500);
#else
			clk_reg(&sleep_cb, xvm_wait_cb, (int4)50, 0L, FALSE);
#endif // SUPPORT_REX_TIMER_IN_SKVM 
			xvm_state = XVMSTATE_SUSPEND;
			g_bWouldBlockTimer = FALSE;
		}
		else 
#endif //#ifndef SKVM_USE_SLEEP_TIMER_IN_NETWORK
		{
//			if (!SKVM_IS_FIRST_REFRESH())
//			{
				if ( xsig_flags_skt & (UPDATE_ANNU_XSIG|REFRESH_LCD_XSIG))
				{
					if (XSIG_SKT_TEST(UPDATE_ANNU_XSIG)){
						XSIG_SKT_CLR(UPDATE_ANNU_XSIG);
						g_bRefreshAnnun = TRUE;
					}
					if (XSIG_SKT_TEST(REFRESH_LCD_XSIG)){
						XSIG_SKT_CLR(REFRESH_LCD_XSIG);
					}
					rex_set_sigs(XVM_TCB,XVM_TIMERS_SIG);
					xvm_state = XVMSTATE_SUSPEND;
				}
//			}
		}
	}
}

int md_bg_set_app(int argc, char **argv)
{
#ifdef SUPPORT_SET_SKVM_IN_BG
	int i,j;
	BYTE temp[MAX_OF_ARGV+1];
	char* p;

	SKY_SET_JAVAASBG(TRUE);
	SKY_SET_ARGCINSKVM(argc);
	
	j=0;

	memset(temp,0x00,sizeof(temp)); 
	SKY_SET_ARGVINSKVM(temp,j);

	for ( i = 0 ; ((i <= MAX_OF_ARGV) && (i < argc) && (argv[i] != NULL)) ; i++)
	{
		p = argv[i];
		strcpy((char*)&temp[j],p);
		j+=strlen(argv[i])+1;
	}

	SKY_SET_ARGVINSKVM(temp,j);
#endif
	return 1;
}

int SKVMMain(void)
{
	int ret = RET_SUSPEND_1;
	
	DWORD						current_time;
	clk_julian_type             julian;

	if (xvmProcessing) {
#ifdef	SKVM_IN_UI_TASK
		UI_DISABLE_SLEEP();
#else
		VAS_SLEEP_FORBID();
#endif
		rex_set_pri(XUI_PRI);
		xvm_run();
#ifdef	SKVM_IN_UI_TASK
		rex_set_pri(UI_PRI);
#else
		rex_set_pri(VAS_PRI);
#endif
#ifdef	SKVM_IN_UI_TASK
		UI_ENABLE_SLEEP();
#else
		VAS_SLEEP_ALLOW();
#endif
		if (xvm_state == XVMSTATE_SUSPEND)
		{
//			if (g_bRefreshAnnun && !SKVM_IS_FIRST_REFRESH())
			if (g_bRefreshAnnun)
			{
				DrawAnnunciator(TRUE);
				g_bRefreshAnnun = FALSE;
//				if (!IsAlarmActive())
				if (!SKY_GetActiveAlarmFlag())
				{

					current_time = SKY_GetLocalTime();
					clk_secs_to_julian( current_time, &julian );
					sFindAlarmOn( &julian, &AlarmTypeInSKVM );
					if( AlarmTypeInSKVM.Type > 0 )
					{
						SKVMSendEvent2UI(UI_SKVM_PLAY_ALARM);
						ret = RET_SUSPEND_0;
						//SKVM_DO_EVENT(SE_SKVM_PLAY_ALARM);
					}
					
				}
			}
		}
		else if (xvm_state == XVMSTATE_STOPPED)
		{
	
			xvm_state = XVMSTATE_DEAD;
			gvm_app_idx = xvm_nextargs();
			if (xvm_state != XVMSTATE_DEAD)
			{
				// 장애발생으로 인한 종료 
				if (xvm_errnum){
					SKVM_DO_EVENT(SE_SKVM_STOPPED_BY_ERROR);
				}
				else
					rex_set_sigs(rex_self(), XVM_TIMERS_SIG);
			}
			else
			{
				if (gvm_app_idx >= 0)
				{
				  // for GVM app start
				  SKVM_DO_EVENT(SE_SKVM_STOPPED_TO_GVM);
				}
				else if (gvm_app_idx == -2)
				{
				  // for WAP app start
				  SKVM_DO_EVENT(SE_SKVM_STOPPED_TO_WAP);
                  gvm_app_idx = -1;
				}
				else
				{
					if (xvm_errnum)
					{
						// 장애발생으로 인한 종료 
		    			SKVM_DO_EVENT(SE_SKVM_STOPPED_BY_ERROR);
					}
					else
					{
		    			// 일반적인 종료  
						SKVM_DO_EVENT(SE_SKVM_STOPPED);
					}
				}
			}
		}
	}
	else ret = RET_NOT_PROCESS;

	return ret;
}

void SKVMStopBG()
{
    enqueStopEvent();
	xvm_nextargs();
	if (xvm_state != XVMSTATE_DEAD){
		rex_set_sigs(XVM_TCB,XVM_TIMERS_SIG);
	}
}

void SKVMTerminate()
{
	if (xvm_state == XVMSTATE_STOPPED){
		xvm_state = XVMSTATE_DEAD;
		xvm_nextargs();
		if (xvm_state != XVMSTATE_DEAD)
			rex_set_sigs(XVM_TCB,XVM_TIMERS_SIG);
		else {
#ifndef SKVM_IN_UI_TASK
			SKVM_DO_EVENT(SE_SKVM_STOPPED);
#endif
		}
	}
	
}

void SKVMStart(void)
{
//	static char *argv[] = {"xvm","-c","__xvmrom__.jar","com.xce.jam.XBrowser"};
	static char *argv[] = {"xvm","-d","0","-c","__xvmrom__.jar","com.xce.jam.XBrowser"};
	
	lcdstdioon();

	xvm_setargs(6,argv);
	xvm_nextargs();
	if (xvm_state != XVMSTATE_DEAD){
		rex_set_sigs(XVM_TCB,XVM_TIMERS_SIG);
	}
}

void SKVMStartBymsdURL(char *msdURL,char *retURL,int svcOption)
{

	MSG_ERROR("SKVMStartBymsdURL called!",0,0,0);
   
	// MO=0x921
	if( P_REV_IS_6_OR_GREATER && SKY_IS_TO_SKVM() )
	{		
	  SetMOinSO33(CAI_MUX_16X_RS1_DOUBLE);	
	}

	// md_pppopen();
	 
    // WAP에서 온 경우 다시 WAP으로 돌아가기 위한 Return URL을 push함 
	if (SKY_IS_TO_SKVM())
	{
		SKY_SET_TO_SKVM(FALSE);
		xvm_seturl(msdURL,retURL,svcOption);
	}
	else
	{
		SKVM_SET_SMS_DOWNLOAD(TRUE);
		xvm_seturl(msdURL,"",0);
	}

	xvm_nextargs();
	if (xvm_state != XVMSTATE_DEAD){
		rex_set_sigs(XVM_TCB,XVM_TIMERS_SIG);
	}
}


void SKVMStartBG(void)
{
	char *argv[128]={""};
	BYTE* p;
	int i,j;
	int argc = SKY_GET_ARGCINSKVM();

	p = SKY_GET_ARGVINSKVM(); 
	j = 0;
	for ( i = 0 ; ((i <= MAX_OF_ARGV) && (i < argc) && (p[j] != NULL)) ; i++)
	{
		argv[i] = (char*)&p[j];
		j+=strlen(argv[i])+1;
	}

	
	xvm_setargs(argc,argv);
	xvm_nextargs();

	if (xvm_state != XVMSTATE_DEAD){
		rex_set_sigs(XVM_TCB,XVM_TIMERS_SIG);
	}

}


void SKVMStop(void)
{
MSG_ERROR("SKVMStop called!",0,0,0);
	SKY_StopMusic();	
    xvm_user_stop();
#ifndef SKVM_IN_UI_TASK
	SKVMSendEvent2UI(UI_SKVM_STOPPED);
#endif
 }

void SKVMPause(void)
{
	SKY_StopMusic();	
    xvm_user_pause();
#ifndef SKVM_IN_UI_TASK
	SKVMSendEvent2UI(UI_SKVM_PAUSED);
#endif
}

void SKVMResume(void)
{
MSG_ERROR("SKVMResume called!",0,0,0);
    xvm_user_resume(1);
#ifndef SKVM_IN_UI_TASK
	SKVMSendEvent2UI(UI_SKVM_RESUMED);
#endif
}

void SKVMInputData(unsigned char * msg,int msgLen)
{
	xvm_process_applicationdata(msg,msgLen);

    xvm_nextargs();
	if (xvm_state != XVMSTATE_DEAD){
		rex_set_sigs(XVM_TCB,XVM_TIMERS_SIG);
	}

#ifndef SKVM_IN_UI_TASK
    SKY_InitKey(XVM_TCB, VAS_KEY_SIG );
	SKVMSendEvent2UI(UI_SKVM_RECEIVED_DATA);
#endif
}

void SKVMSetKeyEvent(void)
{
    XSIG_SET(KEY_XSIG);
}

void SKVMSendEvent2UI(UI_COMMAND_E cmd)
{
	ui_cmd_type  *send_cmd_ptr;                       
	
	if(( send_cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) == NULL)
	{
		MSG_ERROR("Ui No Free Queue!",0,0,0);
	} 
	else	
	{
		send_cmd_ptr->hdr.cmd		   = cmd;
		send_cmd_ptr->hdr.task_ptr     = NULL;
		send_cmd_ptr->hdr.done_q_ptr   = &ui_cmd_free_q;
		ui_cmd( send_cmd_ptr);
	}
}

