#ifndef DIAG_H 
#define DIAG_H
/*==========================================================================

    D I A G N O S T I C    T A S K   H E A D E R    F I L E

DESCRIPTION
  Global Data declarations of the diag_task.

Copyright (c) 1991-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

      $Header:   O:/src/asw/COMMON/vcs/diag.h_v   1.2.1.0   14 May 2001 20:12:48   jlindqui  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/14/01   jal     Exposed diag_close() so other modules can order Diag to
                   close its port.
11/14/00   lad     Added support for FEATURE_RUNTIME_DEVMAP.
11/02/00   lad     Re-assigned signal bits to avoid conflict with task.h
10/17/00   jal     Changed value of DIAG_EVENT_TIMER_SIG, to avoid conflict
                   with TASK_OFFLINE_SIG in task.h
07/06/00   jal     Added missing #define of DIAG_GPS_GRID_DONE_SIG
05/10/00   lad     Removed SIO close sig (unused).  RPC cleanup.
04/11/00   lad     Added support for FEATURE_DIAG_UI_BAUD.
02/28/00   lad     Added support for streaming DIAG and event reporting service.
02/02/00   lad     Removed obsolete signal definitions.
11/10/99   bgc     Fixed declaration of diag_get_rx_pkt() to match diag.c
09/17/99   lcc     Merged in RPC and PLT support from PLT archive:
           jcw     Merged in the latest changes for RPC via mini-DM
           dgy     Added signal for PLT command interface.
08/17/99   tac     Merged in EFS changes from branch.
03/31/99   lad     Added interface to update NV cache after OTASP update.
                   Added interface to put DIAG in LPM mode (and ONLINE).
02/08/99   ak      Added signal for when diag should switch between UART 1
                   and UART2.
01/14/99   ak      Re-integrated in old SIO code.  Removed references to 
                   "arjun"
01/12/99   ak      Changed to incorporate new SIO.  Includes new signal masks
                   inclusion of sio.h and a new function prototype.
10/30/97   jjn     Forced "Module Status" response packet to go out after the
                   falling edge of the SYS_WAKE pulse
10/08/97   jjn     Changed DIAG_AUDIO_CONTROL_PKT_SIZE to 
                   UI_AUDIO_CONTROL_PKT_SIZE
10/06/97   jjn     Added new commands for the Module Phase 1 interface.  These
                   include Module Status Mask, AKEY and audio control packets.
04/23/97   jjn     Utilized a (previously reserved) signal to talk to UI task
08/29/95   jmk     Modified phone state defns, removed diag_set_state.
07/10/95   jmk     Added phone state defns, and protos for diag_set_state,
                   diag_inc_event_cnt; DIAG_TOUT_TIMER_SIG, diag_tout_timer.
12/09/93   jah     Added DIAG_TX_SIG
01/05/93   jai     Moved data service support to diagd.h
11/20/92   twp     Added DIAG_OFFLINE_COMPLETE_SIG
11/03/92   twp     Vocoder PCM & PKT loopback,  analog IS-55 tests
09/04/92   twp     Added queue externs for Data Services.
07/07/92   twp     First release

===========================================================================*/


/*===========================================================================

                       INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "rex.h"
#include  "sio.h"   
#ifdef	FEATURE_SKT_RLPDUMP_DS	//	00/04/15
// added by heroine 00.04.13 for RLP frame dump
#include "diagpkt.h"
extern boolean gbRlpInit;
extern boolean gbSigUse;

// finish heroine 00.04.13
#endif	//	#ifdef	FEATURE_SKT_RLPDUMP_DS


/*===========================================================================

                          SIGNAL DEFINITIONS

===========================================================================*/

#define  DIAG_RPT_TIMER_SIG        0x00000001 /* This signal is set when the 
                                                 diag_rpt_timer expires      */
                                              
#define  DIAG_RX_SIG               0x00000002 /* This signal is set by the 
                                                 Serial I/O Services when an 
                                                 HDLC packet is received.    */

#define  DIAG_NV_CMD_SIG           0x00000004 /* This signal is specified when
                                                 issuing commands to the
                                                 nv_cmd_q of the Non-volatile
                                                 Database Task.  When the
                                                 command is completed, the
                                                 Non-volatile Database Task
                                                 sets this signal to notify
                                                 the Diagnostic Task.        */

#define  DIAG_HS_CMD_SIG           0x00000008 /* This signal is specified when
                                                 issuing commands to the
                                                 hs_cmd_q of the handset 
                                                 driver.  When the command
                                                 is completed, the hs driver
                                                 sets this signal to notify
                                                 the Diagnostic Task.        */

#define  DIAG_MC_CMD_SIG           0x00000010 /* This signal is specified when
                                                 issuing commands to the 
                                                 mc_cmd_q of the mc task.
                                                 When the command is
                                                 completed, the mc task sets
                                                 this signal to notify the
                                                 Diagnstic Task.             */

#define  DIAG_SND_CMD_SIG          0x00000020 /* This signal is specified when
                                                 issuing commands to the snd 
                                                 driver. When the command is
                                                 completed, the snd driver 
                                                 sets this signal to notify
                                                 the diag task               */

#define  DIAG_OFFLINE_COMPLETE_SIG 0x00000040 /* This signal is set when the 
                                                 change to offline (analog
                                                 or digital) is complete.    */
                            
#define  DIAG_TX_SIG               0x00000080 /* This signal is set by the 
                                                 Serial I/O Services when a
                                                 packet has been sent.       */

#define  DIAG_TOUT_TIMER_SIG       0x00000100 /* This signal is a timer
                                                 signal for timeout if bad 
                                                 SPC is rec'd                */

#define  DIAG_UI_CMD_SIG           0x00000200 /* Signal that UI_CMD 
                                                 succeeded                   */

#ifdef	FEATURE_SKT_RLPDUMP_DS	//	00/04/19
#define DIAG_RLPDUMP_SIG		   0x00000400 /* RLP DUMP signal */
#endif	//	#ifdef	FEATURE_SKT_RLPDUMP_DS

#ifdef FEATURE_OTASP

#define  DIAG_NV_CACHE_REF_SIG     0x00000800 /* Signal for DIAG to refresh
                                                 NV cache                    */
#else

#define  DIAG_RSVD_SIG_0800        0x00000800 /* Reserved signal             */

#endif

#define  DIAG_SIO_OPEN_SIG         0x00010000 /* Signal that SIO is availabe
                                                 to be opened for use.       */

#define DIAG_SIO_SWITCH_UART_SIG  0x00020000 /* Signal indicating that the 
                                                DIAG should go back to the
                                                aux uart                    */

#define DIAG_EVENT_TIMER_SIG      0x00080000 /* Signal to finish terminate
                                                the current event reporting
                                                buffer                      */

#if (defined(MMI_TREX) && defined(FEATURE_USE_IRDA))
#define  DIAG_SIO_CLOSE_SIG		  0x00040000   /* Signal that sio_close operation */
#endif

/*-------------------------------------------------------------------------
   Definitions of Analog and CDMA operating states of the phone
-------------------------------------------------------------------------*/
#define  DIAG_NO_SVC_S       0x00

/* (A)nalog States */
#define  DIAG_A_INIT_S       0x01
#define  DIAG_A_IDLE_S       0x02
#define  DIAG_A_VC_INIT_S    0x03
#define  DIAG_A_WFO_S        0x04
#define  DIAG_A_WFA_S        0x05
#define  DIAG_A_CONV_S       0x06
#define  DIAG_A_RELEASE_S    0x07
#define  DIAG_A_SA_S         0x08

/* CDMA (D)igital States */
#define  DIAG_D_INIT_S       0x81
#define  DIAG_D_IDLE_S       0x82
#define  DIAG_D_VC_INIT_S    0x83
#define  DIAG_D_WFO_S        0x84
#define  DIAG_D_WFA_S        0x85
#define  DIAG_D_CONV_S       0x86
#define  DIAG_D_RELEASE_S    0x87
#define  DIAG_D_OVHD_S       0x88
#define  DIAG_D_ORIG_S       0x89
#define  DIAG_D_PAGE_RES_S   0x8a
#define  DIAG_D_ORD_RES_S    0x8b
#define  DIAG_D_REG_S        0x8c
#define  DIAG_D_MSG_S        0x8d

#ifdef	FEATURE_SKT_RLPDUMP_DS	//	00/04/15
// added by heroine 00.04.12 for RLP dump frame
#ifdef	FEATURE_SKT_RLP3DUMP_DS
//#define RLP_DUMP_BUFS	  2048
#define RLP_DUMP_BUFS	  (2048*4)
#else	//	#ifdef	FEATURE_SKT_RLP3DUMP_DS
#define RLP_DUMP_BUFS	  64
#endif	//	#ifdef	FEATURE_SKT_RLP3DUMP_DS
#define TX_RLP_FRAME_DUMP 1
#define RX_RLP_FRAME_DUMP 0
// finish heroine 00.04.12
#endif	//	#ifdef	FEATURE_SKT_RLPDUMP_DS


/*==========================================================================

                                TIMERS

==========================================================================*/

extern rex_timer_type  diag_rpt_timer;      /* Watchdog report timer   */
extern rex_timer_type  diag_tout_timer;     /* Voluntary timeout timer */



/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION DIAG_TASK

DESCRIPTION
  This function contains the diagnostic task.  This task supports commun-
  ication with the Diagnostic Monitor, or other external diagnostic devices.
  Each packet received from the DM is processed and replied to, allowing the
  DM to perform diagnostic functions (peek and poke, commanded tests),
  configuration functions (NV item read and write, service programming),
  calibration functions (power adjustment table tweak), and remote handset
  emulation.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This procedure does not return.

SIDE EFFECTS
  None.

===========================================================================*/
extern void  diag_task ( dword param ) ;



/*===========================================================================

FUNCTION DIAG_INC_EVENT_CNT

DESCRIPTION
  This function increments a count of events that took place in the phone
  which might imply that the phone had changed enough that other kinds of
  status might want to be requested from diag by external devices.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void  diag_inc_event_cnt ( void ) ;




/*===========================================================================

FUNCTION DIAG_RECORD_DM_BITRATE

DESCRIPTION
  This function is used by caller to set baudrate for DM

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void diag_record_dm_baudrate
( 
  sio_bitrate_type bitrate       /* Bitrate at which SIO device should be
                                    configured                             */
);


#if defined(FEATURE_DIAG_UI_BAUD)

/*===========================================================================

FUNCTION DIAG_CHANGE_DEFAULT_SIO_BITRATE

DESCRIPTION
  This function changed the default DIAG bitrate as well as change the 
  current operating bitrate.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void diag_change_default_sio_bitrate
(
  sio_bitrate_type new_baud
);
#endif /* FEATURE_DIAG_UI_BAUD */



/*===========================================================================

FUNCTION DIAG_POWERDOWN

DESCRIPTION
  This procedure sends the command to MC to power down the phone.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
 
===========================================================================*/
void diag_powerdown( void );



/*===========================================================================

FUNCTION DIAG_SET_MODE_ONLINE

DESCRIPTION
  This procedure tells DIAG to change to ONLINE mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
 
===========================================================================*/
void diag_set_mode_online( void );



/*===========================================================================

FUNCTION DIAG_CLOSE

DESCRIPTION
  This closes the current diag connection, if there is one.

  The single argument, close_func_ptr, should be a pointer to a callback
  function to be called on completion of the close.  Pass it NULL if none
  is needed.

===========================================================================*/
void diag_close(void (*close_func_ptr)(void));



#ifdef FEATURE_OTASP
/*===========================================================================

FUNCTION DIAG_OTASP_UPDATE

DESCRIPTION
  This procedure tells DIAG to refresh its status cache from NV.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
 
===========================================================================*/
void diag_otasp_update( void );

#endif /* FEATURE_OTASP */


#ifdef	FEATURE_SKT_RLPDUMP_DS	//	00/04/15
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
// added by heroine 00.04.13 for RLP frame dump
typedef struct {
  q_link_type				link;    /* Command queue link */
  diag_rlp_dump_rsp_type	item;
} rlp_dump_pool_type;
// finish heroine 00.04.13

// added by heroine 00.04.13 for RLP frame dump
void rlp_dump_init(void);
#ifdef	FEATURE_SKT_RLP3DUMP_DS	//	khekim 00/05/16
void rlp_dump_put(byte, log_rlp_frames_type *, byte);
#else	//	#ifdef	FEATURE_SKT_RLP3DUMP_DS	
void rlp_dump_put(byte *, byte, byte,byte);
#endif	//	#ifdef	FEATURE_SKT_RLP3DUMP_DS	
diag_rlp_dump_rsp_type *rlp_dump_get(void);
// finish heroine 00.04.13
#endif	//	#ifdef	FEATURE_SKT_RLPDUMP_DS

#endif /* DIAG_H  */

