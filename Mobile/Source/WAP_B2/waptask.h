#ifndef WAP_H
#define WAP_H

/*=======================================================================================================

           Wireless Applicatin Protocol M A I N   H E A D E R
		   
DESCRIPTION

           This is a header file for wap relating definition
========================================================================================================*/
/*========================================================================================================

                        EDIT HISTORY FOR MODULE

		$Header:    waptask.h   1.23   21 Jun 2000 16:38:38   tliou  		$

when       who     what, where, why
--------   ---     --------------------------------------------------------------------------------------
08/28/00   jjkyg   WAP의 dormant 기능을 위하여 enumtype인 wap_dorm_type와 extern void wap_set_dorm_mode()
                   를 선언함  
========================================================================================================*/

#include "rex.h"
#include "queue.h"
#include "wapconnector.h"

extern void wap_task ( dword dummy );
extern void wap_init_browser(void);
extern boolean check_browser_init;

extern rex_timer_type  wap_control_timer;

/* Definition for function prototype */
extern void wap_check_and_run( void );
extern void wap_get_cmd( wap_cmd_type *cmd_ptr );
extern void make_cmd_other2wap( wap_cmd_type *msg_ptr );
extern void send_cmd_other2wap( wap_cmd_type *cmd_ptr );

extern q_type wap_cmd_q; 
extern q_type wap_cmd_free_q;

/* Definition for signals */
#define WAP_RPT_TIMER_SIG       0x0001  /* Time to kick watchdog */
#define WAP_SIG			        0x0002  /* Time to poll event */
#define WAP_CMD_Q_SIG           0x0004  /* Signal for command execution */
#define WAP_ANIMATION_SIG		0x0008	/* SIS Animation Timer */
#define WAP_NV_SIG              0x0010
#ifdef WAP_NOWRAP
#define WAP_NOWRAP_TIMER_SIG    0x0020  /* Signal for no wrapping feature *///yjahn96 00.10.27
#endif//#ifdef WAP_NOWRAP
#define WAP_BAR_SIG             0x0040

#ifdef SOCKET_AUWAP
#define NET_CB_SIG				0x0080  /* Network call back signal AUWAP*/
#define SOCK_CB_SIG             0x0100  /* Socket call back signal AUWAP*/

#define WAIT_ANI_SIG            0x0200  /* Wait anmation signal AUWAP*/
#define READ_EVT_SIG            0x0400  /* Notify net data arrived   */
#endif //SOCKET_AUWAP

//PORTING_B0_WAP
#if (defined WAP_B1) || (defined WAP_B2)
#ifdef LOG_ENABLE
#define MAX_CACHE_SIZE 10000
#else
#define MAX_CACHE_SIZE 20000
#endif //LOG_ENABLE
#else //WAP_B1

#ifdef LOG_ENABLE
#ifdef SOCKET_AUWAP
#define MAX_CACHE_SIZE 5000
#else //SOCKET_AUWAP
#define MAX_CACHE_SIZE 9000
#endif //SOCKET_AUWAP
#else
#define MAX_CACHE_SIZE 20000
#endif //LOG_ENABLE
#endif//WAP_B1

/* Definition for constant */
#define WAP_EVENT_POLLING_TIME  15      /* temporary value for wap event polling time */
#define WAP_CMD_BUFFERS         50      /* temporary value for wap command queue buffer */

typedef rex_sigs_type wap_sigs_type;
/************************************************************************************/
/*																					*/
/*							WML 문서 Loading 관련									*/
/*																					*/
/************************************************************************************/
void do_ds_upenter(void);
void WAP_Process_Loading(int nKey);

/************************************************************************************/
/*																					*/
/*							MMI Keypad 처리 함수									*/
/*																					*/
/************************************************************************************/
extern int WAP_HandleKeys(int nkey);
#define WAP_HANDLE_KEY_RESULT_EXIT				1
#define WAP_HANDLE_KEY_RESULT_PHONE				2
#define WAP_HANDLE_KEY_RESULT_INPUT				3
#define WAP_HANDLE_KEY_RESULT_NEWURL			4
#define WAP_HANDLE_KEY_RESULT_MAYBE_LOADING		5
#define WAP_HANDLE_KEY_RESULT_GOBACK			6
#define WAP_HANDLE_RECEIVE_RELEASE              7
#define WAP_HANDLE_MAKECALL						8
#define WAP_WTLS_SESSION_START                  9
#define WAP_PUSH_IN_WAP                        10
#define WAP_HANDLE_KEY_RESULT_MAYBE_ENTERTAIN  11
#define WAP_MAIL_IN_WAP                        12
#define WAP_PUSHMAIL_CANCEL_IN_WAP             13
#define WAP_HANDLE_KEY_RESULT_GOBACK_ERROR     14
#define WAP_HANDLE_KEY_RESULT_JUST_BREAK	   15

#define WAP_HANDLE_ACCEPT_LEAK                 16
#define WAP_HANDLE_NOT_ACCEPT_LEAK             17


#ifdef USE_DNS
extern boolean savedns;
#endif

typedef struct redirect
{
	BOOL   bCorrect;
	BOOL   bPerm_or_not;
	BOOL   bNetinclude;
	BOOL   bPortNuminclude;

    UINT8  Addresslen;

	UINT8  BearerType;
	UINT16 Portnumber;

	CHAR  *Address;
}redirect_config;


#ifdef WAP_USE_DORM
	typedef enum{
		WAP_MODE_REORIG = 0,
		WAP_MODE_TRANSIENT,
		WAP_MODE_NOT_DORM
	}wap_dorm_type;
	extern void wap_set_dorm_mode(wap_dorm_type wap_dorm);
	extern boolean wap_dorm_active(void);
#endif//#ifdef WAP_USE_DORM

#ifdef WAP_USE_SKT_NEW
    typedef enum
    {
        REL_NULL =0,
        REL_ENABLE_ORIG,
        REL_INPROG
    }wap_release_state_enum_type;
#endif//#ifdef WAP_USE_SKT_NEW

#ifdef WAP_USE_SECURE
	typedef enum
	{
		HAND_NULL_STATE
		,HAND_CLIENT_STATE
		,HAND_WAIT_SERVER_STATE
		,HAND_FINISHED_STATE
	}wap_handshake_state;
#endif//#ifdef WAP_USE_SECURE
#endif /* WAP_H */
