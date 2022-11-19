#ifndef SOCKCTL_H
#define SOCKCTL_H


/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"
#include "mc.h"
#include "ds.h"

#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/06
#include "nv.h"
#include "sockApi.h"

/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/

typedef rex_sigs_type sock_sigs_type;



/* Intertask Command & Generic task signals */

#define SOCK_CMD_Q_SIG            0x0001   /* Command Rx'd signal            */
#define SOCK_NET_CB_SIG           0x0002   /* Network CB signal              */
#define SOCK_SOCKET_CB_SIG        0x0004   /* Socket CB signal          */
#define SOCK_STATE_TIMER_SIG      0x0010   /* State-related Processing signal*/
#define	SOCK_PS_TIMER_SIG		  0x0020	/* TCP/IP time out signal */
#define SOCK_DOG_RPT_TIMER_SIG    0x0040   /* Watchdog Report timeout signal */

/* Currently NOT USED signals                                              */

#define SOCK_SENDDATA_Q_SIG       0x0400

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
#define SOCK_DNS_CB_SIG			  0x0800
#else	//	#ifdef	FEATURE_SOCKET_DNS_API
#define SOCK_RSVD_0800_SIG		  0x0800
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API

#define SOCK_RSVD_1000_SIG        0x1000      /* not used         */
#define SOCK_RSVD_2000_SIG        0x2000      /* TASK_OFFLINE_SIG */
#define SOCK_RSVD_4000_SIG        0x4000      /* TASK_STOP_SIG    */
#define SOCK_RSVD_8000_SIG        0x8000      /* TASK_START_SIG   */


typedef enum{
	SOCKET_SVR_PPPCONNECT_CMD = 0,
	SOCKET_SVR_TCPCONNECT_CMD,
	SOCKET_SVR_TCPRECONNECT_CMD,
	SOCKET_SVR_UDPCONNECT_CMD,
	SOCKET_RELEASE_CMD
} sock_cmd_name_type;


typedef struct 
{
  sock_cmd_name_type cmd_name;   /* actual command for Data Task to process  */
  uint16			 num;
  union
  {
	char			dialStr[NV_MAX_DIAL_DIGITS+1];
	char			serverName[HOSTNAME_LEN];
	char			data[DATA_LEN];
  } parms;
} sock_cmd_type;


extern	sock_sigs_type	sock_signaled_events_mask;
extern	sock_sigs_type	sock_requested_sig_mask;

extern	void sock_cancel_timer
( 
	void
);

extern	void sock_init
( 
	void
);

sock_sigs_type sock_wait
(
  sock_sigs_type requested_mask               /* mask of signals to wait for */
);                 


extern	void	sock_cmd
(
	sock_cmd_type *cmd_ptr
);


extern	void	sock_task
(
	dword
);

#ifdef	FEATURE_SKT_MULTISOCKAPI	//	khekim 01/02/03
extern	uint32 sock_wait_sig
(
	uint32 timer_cnt, 
	uint32 wait_sig, 
	uint32 wait_sig_mask,
	uint32 *sigCmd
);
#else	//	#ifdef	FEATURE_SKT_MULTISOCKAPI
extern	int sock_wait_sig
(
	long int timer_cnt, 
	long int wait_sig, 
	long int wait_sig_mask
);
#endif	//	#ifdef	FEATURE_SKT_MULTISOCKAPI


extern	sock_sigs_type sock_mgr
(
	sock_sigs_type signal_mask                /* mask of signals that occurred */
);


#endif	//	#ifdef	FEATURE_SKT_SOCKAPI

#endif  //	#ifndef	SOCKCTL_H