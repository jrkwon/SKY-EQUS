#ifndef DIAGPKT_H
#define DIAGPKT_H
/*==========================================================================

  D I A G N O S T I C    T A S K    P A C K E T    D E F I N I T I O N S

DESCRIPTION
  Packet definitions to support the interface between the DMSS software
  and the diagnostic monitor.

NOTE
  This file is included by the DMSS and by the DM and TDM, and PST.

Copyright (c) 1992-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

      $Header:   O:/src/asw/COMMON/vcs/diagpkt.h_v   1.7.1.0   02 Mar 2001 16:58:12   ldefauw  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/13/00   jal     Support to clear RLP and TDSO statistics
12/05/00   lad     Updated for new logging API.
11/16/00   lad     Added packet definition macros as well as subsystem 
                   and snapshot commands.
09/25/00   lad     Updated GPS Session Control Response to include subcommand.
09/12/00   jal     Added GPS Acquisition and Sensitivity Assistance support
                   to the GPS Session Control command
09/06/00   bgc     Added FEATURE_FACTORY_TESTMODE including a request and a
                   response.
08/31/00   lad     Changed NV read/write to use size-specific types.
                   Updated DIAG_STREAMING_CONFIG_F packets to allow priorities.
08/15/00   lad     Fixed diag_parm_retrieve_rsp_type to work when IS200 and 
                   IS95B are disabled.
06/23/00   lad     Removed obsolete packet definitions.
06/19/00   lad     Removed obsolete parm_get and added parm_retrieve.
05/31/00   jal     Added GPS statistics, session control, and grid support
05/15/00   lad     Added streaming config support (nice).
05/10/00   lad     Added type for RPC message.
03/23/00   lad     Modified extended logmask to provide mask info if len is 0.
02/28/00   lad     Added streaming (event reporting) and removed obsolete code.
02/02/00   lad     Merged from MSM3000/MSM3100 archives.
01/20/00   lad     Added IS733 support.
01/19/00   lad     Added backwards compatibility for rev 1 DIAG EFS iface.
12/02/99   lad     Added support for MDR service options.
                   Removed obsolete proc_v_peek and proc_v_poke.
10/19/99   lad     Added bitrate change and extended logmask commads.
10/28/99   lad     Changed verno to use hw_version instead of voc_version
                   Walsh code support for sup channels.
07/23/99   lad     Added rev 2 of EFS support.
07/21/99   jal     Fixed some oopses in the Walsh code support
04/26/99   lad     Added first rev of EFS support.  Change MODE_* and SEC_*
                   to use byte values instead of words to save space.
09/07/99   nmn     Removed IS96 support
08/19/99   sk      ARM 2.50 changes.
08/12/99   jkl     Adapted for the new Sound API
03/30/99   lad     Added support for FEATURE_IS95B_MDR and FEATURE_SPECIAL_MDR.
01/15/99   ak      Re-integrating old sio, and featurizing the use of new or
                   old sio.
01/12/99   ak      Made changes in order to support new SIO.  Includes 
                   addition of DIAG_MAX_.._PKT_SIZE #defines.  Also 
                   changed diag_req_pkt_type and diag_rsp_pkt_type structs.
11/04/98   lad     Made necessary changes for ARM port.
10/30/98   jmk     Removed obsolete diag_conf_req_type. Removed diagconf.h 
                   and diagparm.h (not used).
10/27/98   jmk     Added SMS read/write commands.
10/26/98   tkuo    Added FEATURE_MOD_GENERIC_PKT as module status report cmd.
09/11/98   grl     Added feature query interface
08/01/98   kcl     Added RESPONSE comment for diag_audio_control_req_type. 
07/02/98   kcl     Added  DIAG_RL_RD_NV_683A_ERR 
06/24/98   kcl     Added DIAG_RL_RD_683A_OK  
04/08/98   jmk     Added DIAG EVRC service option.
11/13/97   kcl     Added definition of DIAG_OFFLINE_DIGITAL and 
                   DIAG_OFFLINE_ANALOG
10/08/97   jjn     Removed #include "diag.h" and renamed 
                   DIAG_AUDIO_CONTROL_PKT_SIZE to UI_AUDIO_CONTROL_PKT_SIZE
10/06/97   jjn     Added new commands for the Module Phase 1 interface.  These
                   include Module Status Mask, AKEY and audio control packets.
04/23/97   jjn     Added new packet pair to allow System Unit to access 
                   service option and caller ID information
03/25/97   jjn     Added new command (and packets) that allow writing to NV 
                   without going offline (for the Module only)
02/28/97   jjn     Enabled RSSI packets for the Module, added a packet for
                   module status and sound reporting, and added a pcket for
                   retrieving SMS messages
06/25/96   jmk     Added packet defn for reading a PR_LIST from the phone
05/23/96   jmk     Added packet definitions for writing a PR_LIST to the phone
05/17/96   jmk     Added definitions of locked/unlocked security modes.
05/17/96   jmk     Added packet definitions for sending security password.
03/06/96   jmk     Added packet definitions for serial mode change packet, and
                   WLL-only get rssi (for antenna aiming).
02/28/96   dna     Made a trivial change for lint.
01/04/96   jmk     Added SO defn for old 13k markov
08/21/95   jmk     Slightly modified diag_pilot_sets_rsp_type.
08/16/95   jmk     Added packet defn for parm_get2 - includes MUX2 parms
08/15/95   jmk     Added SO defn's for new 8k markov, and 13k loopback
08/10/95   jmk     Added packet defns for Serv. Prog. Code. request/response
07/31/95   jmk     Removed dependency on header files: msg, parm, err, rxc
07/31/95   jmk     Added packets for Phone State and Pilot Sets
06/28/95   jmk     Added 13K Markov Service Option definition
01/28/95   ptw     Added packets for obtaining System Time from the mobile.
12/07/94   jmk     Added packet for turning portable sleep on/off.
11/07/94   jmk     Added packet to tell that seq_nums are desired
09/24/94   jmk     Put packet definitions for ORIG and END back in.
05/18/94   jjw     Added DM multi-drop definitions, huge cleanup for DM
04/20/94   jca     Deleted packets that supported DIAG Data Services.
09/14/93   jjw     Modified to use new SIO structs for run-time mode changes
07/20/93   twp     Added packet for all Test Mobile commands.
05/03/93   twp     Added Parameter retreival, FER data collection
11/03/92   twp     Vocoder PCM & PKT loopback,  analog IS-55 tests
09/14/92   twp     Added release directory info to verno packet.
09/14/92   twp     More status mods.
09/01/92   twp     Modified the status packet definition to bring it back
                   to life.
08/25/92   twp     Added nv_stat to nv read and write packets
07/28/92   twp     Changed DIAG_DIAGVER since DM uses old value (0) for
                   some special reason.
03/27/92   twp     First release

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include  "comdef.h"
#include  "customer.h"
#include  "target.h"
#include  "event.h"
#include  "qw.h"
#include  "diagcmd.h"
#include  "diagt.h"
#include  "feature.h"
#include  "nv.h"
#include  "log.h"
#if MMI_TYPE != SKY_UI  //add_htg_00_xmas : To adapt for EQUS
#include  "hs.h"
#else
#include "sky.h"
#endif
#include  "sio.h"    /* now including this all the time */

#include  "snd.h"

#if (( TG != T_DM ) && (TG != T_SP))
#if MMI_TYPE != SKY_UI  //add_htg_00_xmas : To adapt for EQUS
#include  "ui.h"
#else
#include "sky.h"
#endif
#else
#define UI_A_KEY_DIGITS 26
#endif

#ifdef FEATURE_DIAG_QCT_EXT
#include "diagx.h"
#endif

/* <EJECT> */
/*===========================================================================

                          PACKET DEFINITONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*   Communication from the Diagnostic Monitor to the Mobile               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* DIAG_DIAGVER                                                            
   Diagnostics version, used to ensure compatibility of the DM and the DMSS.      
   1   Original
   2   Changes to status packet and verno packet. Removed verstr packet
   3   for release to factory. Sends RF mode in status packet
   4   Adds release directory to verno packet and dipswitch packets
   5   Many changes in DM.
   6   Added Vocoder PCM and PKT loopback, and Analog IS-55 tests
   7   New FER data in tagraph packet
   8   Streaming Output and Event Reporting
*/

#ifdef FEATURE_DIAG_NON_STREAMING
#define DIAG_DIAGVER 7
#else
#define DIAG_DIAGVER 8
#endif

/*---------------------------------------------------------------------------
   Operating modes for packet processing.  These masks are used to define
   permissions for packet processing in the dispatch table.
---------------------------------------------------------------------------*/
#define MODE_OFF_DIG  0x01  /* Packet accepted in offline-digital        */
#define MODE_OFF_ANA  0x02  /* Packet accepted in offline-analog         */
#define MODE_ONLINE   0x04  /* Packet accepted in online                 */

#ifndef FEATURE_WAP
#define MODE_ANY      0xFF  /* Packet accepted in all modes.             */
#endif //FEATURE_WAP

#define MODE_OFFLINE  ( MODE_OFF_DIG | MODE_OFF_ANA ) 
                              /* Packet accepted in either offline mode    */

/*---------------------------------------------------------------------------
   Security for packet processing.  These masks are used to define
   whether a packet can be accepted when the security is locked or
   unlocked, for packet processing in the dispatch table.
---------------------------------------------------------------------------*/
#define SEC_LOCKED    TRUE  /* Packet accepted if security is locked   */
#define SEC_UNLOCKED  FALSE /* Packet accepted if security is unlocked */


#if (defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR))

#define DIAG_SUP_CHANNEL_MAX 7       /* Max number of supplemental channels
                                        allowed in MDR.  There are 8 total.
                                        (Fundamental channel + up to 7 
                                        supplemental channels)             */

#endif /* (defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)) */


/*-------------------------------------------------------------
           Diag Packet Type Defintion Macros
           
  Diag request/response types use the same naming convention 
  throughout the DMSS.  The naming conventions are as follows:
  
  Command codes use the naming convnetion: DIAG_xxx_F
  
  Requests types:
  DIAG_xxx_F_req_type
  
  Response types:
  DIAG_xxx_F_rsp_type
  
-------------------------------------------------------------*/
#define DIAGPKT_REQ_DEFINE( xx_cmd_code ) \
  typedef struct xx_cmd_code##_req_tag    \
                 xx_cmd_code##_req_type;  \
  PACKED struct  xx_cmd_code##_req_tag {  \
    uint8 cmd_code; /* All diag packets have a command code */

#define DIAGPKT_REQ_END };

/* If the response is the same structure as the request... */
#define DIAGPKT_DEFINE_RSP_AS_REQ( xx_cmd_code ) \
  typedef xx_cmd_code##_req_type xx_cmd_code##_rsp_type;

#define DIAGPKT_RSP_DEFINE( xx_cmd_code ) \
  typedef struct xx_cmd_code##_rsp_tag    \
                 xx_cmd_code##_rsp_type;  \
  PACKED struct  xx_cmd_code##_rsp_tag {  \
    uint8 cmd_code; /* All diag packets have a command code */

#define DIAGPKT_RSP_END };

/* !!! NOTE: For now, this step is done manually.  Modify the 
   dispatch table in diagpkt_dispacther.c to call your routine. */
#define DIAGPKT_DISPATCHER_REGISTER( xx_func_name )
  

/*-------------------------------------------------------------
           DIAG PACKET SUBSYSTEM TYPE DEFINITION MACROS
           
  Diag has a subsystem command that dispatches DIAG commands to
  various subsystems.  All subsystem packet types use the same
  naming convention throughout the DMSS.  The naming
  conventions are as follows:
  
  Command codes use the naming convnetion: DIAG_SUBSYS_xxx_F
  
  Requests types:
  DIAG_SUBSYS_xxx_F_req_type
  
  Response types:
  DIAG_SUBSYS_xxx_F_rsp_type
  
-------------------------------------------------------------*/
/*-------------------------------------------------------------
     DIAG SUBSYSTEM DISPATCH PACKET TYPE DEFINITION MACROS
           
  Diag has a subsystem command that dispatches DIAG commands to
  various subsystems.  All subsystem packet types use the same
  naming convention throughout the DMSS.  The subsystem command
  uses a 16 bit command code per subsystem.  This results in
  user data starting on a 32 bit boundary.
  
  The naming convention is as follows:
  
  Command codes use the naming convnetion: DIAG_SUBSYS_xxx_F
  
  Requests types:
  DIAG_SUBSYS_xxx_yyy_req_type
  
  Response types:
  DIAG_SUBSYS_xxx_yyy_rsp_type
  
-------------------------------------------------------------*/
#define DIAGPKT_SUBSYS_REQ_DEFINE( xx_subsys_cmd_code ) \
  typedef struct DIAG_SUBSYS_##xx_subsys_cmd_code##_req_tag \
                 DIAG_SUBSYS_##xx_subsys_cmd_code##_req_type; \
  PACKED struct  DIAG_SUBSYS_##xx_subsys_cmd_code##_req_tag { \
    uint8  command_code; \
    uint8  subsys_id;  /* use diagpkt_subsys_cmd_enum_type */ \
    uint16 subsys_cmd_code;
  
#define DIAGPKT_SUBSYS_REQ_END };

#define DIAGPKT_SUBSYS_RSP_DEFINE( xx_subsys, xx_subsys_cmd_code ) \
    typedef struct DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_tag   \
                   DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_type; \
    PACKED struct  DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_tag { \
      uint8 cmd_code; \
      uint8 subsys_id;  /* use diagpkt_subsys_cmd_enum_type */   \
      uint16 subsys_cmd_code;

#define DIAGPKT_SUBSYS_RSP_END };

/* For now, registration is done manually */
#define DIAGPKT_SUBSYS_DISPATCHER_REGISTER( xx_func_name )  


/* <EJECT> */
/*===========================================================================
FUNCTION DIAGPKT_SET_CMD_CODE 
DESCRIPTION
  This function sets the command code in the specified diag packet.

===========================================================================*/
INLINE void diagpkt_set_cmd_code(PACKED void *ptr, uint8 cmd_code) 
{
  /* Diag command codes are the first byte */
  *((uint8 *) ptr) = cmd_code;
} /* diag_get_cmd_code */

/*===========================================================================
FUNCTION DIAGPKT_GET_CMD_CODE 
DESCRIPTION
  This function returns the command code in the specified diag packet.

===========================================================================*/
INLINE uint8 diagpkt_get_cmd_code(PACKED void *ptr) 
{
  /* Diag command codes are the first byte */
  return *((uint8 *) ptr);
} /* diag_get_cmd_code */



/*---------------------------------------------------------------------------

       Declare the packet types for DM to Mobile communication

---------------------------------------------------------------------------*/

/*============================================================================

PACKET   diag_verno_req_type

ID       DIAG_VERNO_F

PURPOSE  Sent from the DM to the DMSS requesting the DMSS send its
         version number

RESPONSE The DMSS sends back a diag_verno_rsp_type packet containing the
         version number

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
} diag_verno_req_type;


/*============================================================================

PACKET   diag_verstr_req_type

ID       DIAG_VERSTR_F

PURPOSE  Sent from the DM to the DMSS to request the version string

RESPONSE The DMSS sends back a diag_verstr_rsp_type packet containing the
         version string.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
} diag_verstr_req_type;


/*===========================================================================

PACKET   diag_esn_req_type

ID       DIAG_ESN_F

PURPOSE  Sent from the DM to the DMSS to request the Mobile's ESN

RESPONSE The DMSS sends back a diag_esn_rsp_type packet containing ESN

============================================================================*/
typedef PACKED struct
{
  byte  cmd_code;              /* Command code. */
} diag_esn_req_type;


/*==========================================================================

PACKET   diag_peek_req_type

IDs      DIAG_PEEKB_F
         DIAG_PEEKW_F
         DIAG_PEEKD_F

PURPOSE  Sent from the DM to the DMSS to request a read of a block of data.
         The cmd_code specifies byte, word or dword.

RESPONSE The DMSS reads the address and sends the data back in a
         diag_peekb_rsp_type or diag_peekw_rsp_type or diag_peekd_rsp_type

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  byte *ptr;                   /* starting address for peek operation */
  word length;                 /* number of bytes, words, or dwords  to be 
                                  returned */
} diag_peek_req_type;


/*===========================================================================

PACKET   diag_pokeb_req_type

ID       DIAG_POKEB_F

PURPOSE  Sent by the DM to the DMSS to request that a block of data be
         written starting at a specified address.

RESPONSE If the poke is successful, the DMSS sends exactly the same packet
         back to the DM, otherwise an error packet is sent.

============================================================================*/
#define DIAG_MAX_POKE_B 4
  /* Maximum number of bytes allowed for one pokeb request */

typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  byte *ptr;                   /* starting address for poke operation */
  byte length;                 /* number of bytes to be poked */
  byte data[DIAG_MAX_POKE_B];  /* bytes to be placed at the addr address */
} diag_pokeb_req_type;


/*===========================================================================

PACKET   diag_pokew_req_type

ID       DIAG_POKEW_F

PURPOSE  Sent by the DM to the DMSS to request that a block of data be
         written starting at a specified address.

RESPONSE If the poke is successful, the DMSS sends exactly the same packet
         back to the DM, otherwise an error packet is sent.

============================================================================*/
#define DIAG_MAX_POKE_W 2
  /* Maximum number of words allowed for one pokew request */

typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word *ptr;                   /* starting address for poke operation */
  byte length;                 /* number of words to be poked */
  word data[DIAG_MAX_POKE_W];  /* words to be placed at the addr address */
} diag_pokew_req_type;


/*===========================================================================

PACKET   diag_poked_req_type

ID       DIAG_POKED_F

PURPOSE  Sent by the DM to the DMSS to request that a block of data be
         written starting at a specified address.

RESPONSE If the poke is successful, the DMSS sends exactly the same packet
         back to the DM, otherwise an error packet is sent.

============================================================================*/
#define DIAG_MAX_POKE_D 2
  /* Maximum number of dwords allowed for one poked request this is 2 to
     allow possible growth to handle qwords. */

typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  dword *ptr;                  /* starting address for poke operation */
  byte length;                 /* number of dwords to be poked */
  dword data[DIAG_MAX_POKE_D]; /* dword to be placed at the addr address */
} diag_poked_req_type;


/*===========================================================================

PACKET   diag_outp_req_type

ID       DIAG_OUTP_F

PURPOSE  Sent by the DMSS to the DM to request a byte of data be output
         to a specified port.

RESPONSE The DMSS outputs the byte to the port and echos the
         diag_outp_req_type packet back to the DM.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word port;                   /* number of port to output to */
  byte data;                   /* data to write to port */
} diag_outp_req_type;


/*===========================================================================

PACKET   diag_outpw_req_type

ID       DIAG_OUTPW_F

PURPOSE  Sent by the DMSS to the DM to request a word of data be output
         to a specific port.

RESPONSE The DMSS outputs the word, then echos back the diag_outpw_req_type
         packet to the DMSS.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word port;                   /* number of port to output to */
  word data;                   /* data to write to port */
} diag_outpw_req_type;


/*===========================================================================

PACKET   diag_inp_req_type

ID       DIAG_INP_F

PURPOSE  Sent by the DM to request a byte to be input from a given port.

RESPONSE The DMSS inputs the byte, then returns it in a diag_inp_rsp_type
         packet.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word port;                   /* port to read from */
} diag_inp_req_type;


/*===========================================================================

PACKET   diag_inpw_req_type

ID       DIAG_INPW_F

PURPOSE  Sent by the DM to request a word to be input from a given port.

RESPONSE The DMSS inputs the word, then returns it in a diag_inpw_rsp_type
         packet.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word port;                   /* port to read from */
} diag_inpw_req_type;


/*===========================================================================

PACKET   diag_status_req_type

ID       DIAG_STATUS_F

PURPOSE  Sent by the DM to request a set of status data

RESPONSE The DMSS gathers the status data and sends it in a
         diag_status_rsp_type packet to the DM.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
} diag_status_req_type;


/*===========================================================================

PACKET   diag_logmask_req_type

ID       DIAG_LOGMASK_F

PURPOSE  Sent by the DM to set the logging mask in the DMSS

RESPONSE The DMSS establishes the logging mask and replies with a
         diag_logmask_rsp_type packet

===========================================================================*/
typedef PACKED struct
{
  byte  cmd_code;          /* Command code */
  dword mask;              /* mask to use  */
} diag_logmask_req_type;

#ifdef FEATURE_SKT_DEBUG

#ifdef	FEATURE_SKT_RLPDUMP_DS	//	00/04/15
//heroine 00.04.13
#ifdef	FEATURE_SKT_RLP3DUMP_DS	//	khekim 00/05/15
typedef PACKED struct
{
	byte				rlp_type;
	byte				direction;
	byte				q_dump;
	byte				q_miss;
	log_rlp_frames_type	rlp_frame;
} diag_rlp_dump_rsp_type;
#else	//	#ifdef	FEATURE_SKT_RLP3DUMP_DS
typedef PACKED struct
{
	byte rlp_type;
	byte rate_set;
	byte rlp_delay;
	byte type;
	byte frame_rate;
	byte format;
	byte num_sup;
	byte q_dump;
	byte q_miss;
	byte data[6];
} diag_rlp_dump_rsp_type;
#endif	//	#ifdef	FEATURE_SKT_RLP3DUMP_DS

//typedef PACKED struct 
typedef PACKED struct 
{
	byte	cmd_code;
	byte	sub_code;
	boolean valid;
	diag_rlp_dump_rsp_type rlp_dump;
} diag_sktest_skrlpdump_rsp_type;

//heroine 00.04.13
#endif	//	#ifdef	FEATURE_SKT_RLPDUMP_DS

#define DIAG_SKTEST_FING      0       // Get Fingers Data upto 4;
#define DIAG_SKTEST_RLP_INIT  1       // Rlp Dump with out DM Command.
#define DIAG_SKTEST_RLP_STOP  2
#define DIAG_SKTEST_RLP_QUERY 3       // Rlp Dump from DM Command.
#define DIAG_SKTEST_RLP_DUMP  4       // RlpDump Request
#define DIAG_SKTEST_RLP_PARAM 5       // obsolete

typedef PACKED struct
{
	byte cmd_code;
	byte sub_code;
} diag_sktest_req_type;


typedef PACKED struct
{
	byte cmd_code;
	byte sub_code;
	word fing_pilot[4];
	word fing_x[4];
	byte fing_y[4];
} diag_sktest_skfing_rsp_type;

typedef PACKED struct
{
	byte cmd_code;
	byte sub_code;
} diag_sktest_skcmd_rsp_type;

typedef PACKED struct
{
	byte cmd_code;
	byte sub_code;
	byte rlp_type;
	byte rate_set;
	byte rlp_delay;
	/////more field must be required for type-3.
} diag_sktest_skrlpparam_rsp_type;
#endif	//	#ifdef FEATURE_SKT_DEBUG


#ifdef FEATURE_DIAG_NON_STREAMING
/*===========================================================================

PACKET   diag_log_req_type

ID       DIAG_LOG_F

PURPOSE  Sent by the DM to request accumulated logging information

RESPONSE The DMSS retreives a logging message and sends it in a
         diag_log_rsp_type packet

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
} diag_log_req_type;
#endif

/*===========================================================================

PACKET   diag_nv_peek_req_type

ID       DIAG_NV_PEEK_F

PURPOSE  Sent by DM to view an area of DMSS NV memory.  Operates strictly
         by address.

RESPONSE The DMSS reads the specified NV memory and returns its value in
         a diag_nv_peek_rsp_type packet.

===========================================================================*/
typedef PACKED struct
{
  byte  cmd_code;               /* Command code */
  word  address;                /* NV address */
  byte  length;                 /* length */
} diag_nv_peek_req_type;


/*===========================================================================

PACKET   diag_nv_poke_req_type

ID       DIAG_NV_POKE_F

PURPOSE  Sent by the DM to request that DMSS NV memory be set to specified
         values. DIAG_MAX_NV_POKE is the max size of the memory block that
         can be written.

RESPONSE DMSS writes specified block of NV data and echos the 
         diag_nv_poke_req_type packet back to the DM.

============================================================================*/
/* Maximum number of bytes that can be poked in NVM with one request */
#define DIAG_MAX_NV_POKE 4

typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  nv_poke_type  nvpoke;        /* nv item type */
} diag_nv_poke_req_type;


/*===========================================================================

PACKET   diag_tagraph_req_type

ID       DIAG_TAGRAPH_F

PURPOSE  Sent by the DM to request info for lower-right corner of TA

RESPONSE The DMSS prepares and sends a diag_tagraph_rsp_type packet.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;                /* Command code */
} diag_tagraph_req_type;


/*===========================================================================

PACKET   diag_markov_req_type

ID       DIAG_MARKOV_F

PURPOSE  Sent by the DM to reqest Markov Statistics

RESPONSE The DMSS prepares and sends a diag_markov_rsp_type packet.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;                /* Command code */
} diag_markov_req_type;


/*===========================================================================

PACKET   diag_diag_ver_req_type

ID       DIAG_DIAG_VER_F

PURPOSE  Sent by DM to request the version of the diag

RESPONSE DMSS returns DIAG_DIAGVER in a diag_diag_ver_rsp_type packet

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                /* Command code */
} diag_diag_ver_req_type;


/*==========================================================================

PACKET   diag_ts_req_type

ID       DIAG_TS_F

PURPOSE  Sent by the DM to request a timestamp from the DMSS

RESPONSE The DMSS returns a timestamp in a diag_ts_rsp_type packet.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;                /* Command code */
} diag_ts_req_type;


/*===========================================================================

PACKET   diag_ta_parm_req_type

ID       DIAG_TA_PARM_F

PURPOSE  Sent by the DM to set TA parameters in the DMSS

RESPONSE The DMSS sets the TA parameters and sends a diag_ta_parm_rsp_type
         packet.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                /* Command code */
  word set_mask;
    /* Bit 15 = enable reporting of active set members
       Bit 14 = enable reporting of candidate set members
       Bit 13 = enable reporting of neighbor set members
       Bit 12 = enable reporting of remaining set members 
       Bit 11 = enable reporting of pre-candidate set members */
  word win_siz;  /* specifies maximum window size in half chip units */

} diag_ta_parm_req_type;


/*===========================================================================

PACKET   diag_markov_reset_req_type

ID       DIAG_MARKOV_RESET_F
PURPOSE  Sent by the DM to reset the DMSS's Markov Statistics

RESPONSE DMSS resets the accumulated Markov Statistics and replies with
         a diag_m_reset_rsp_type packet.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;                /* Command code */
} diag_markov_reset_req_type;


/*===========================================================================

PACKET   diag_msg_req_type

ID       DIAG_MSG_F

PURPOSE  Sent by the DM to request a buffered msg

RESPONSE The DMSS selects an appropriate message and sends it in a
         diag_msg_rsp_type packet.

============================================================================*/
typedef PACKED struct
{
 byte cmd_code;                    /* Command code */
 word msg_level;                   /* highest type desired */
} diag_msg_req_type;


/*===========================================================================

PACKET   diag_err_clr_type

ID       DIAG_ERR_CLEAR_F

PURPOSE  Sent by the DM to clear the requested buffered error records

RESPONSE The DMSS clears the specified error records.

===========================================================================*/
typedef PACKED struct {
  byte                  cmd_code;   /* Command code --  DIAG_ERR_CLEAR_F */
  byte                  rec;        /* record id, or -1 (0xFF) for all */
} diag_err_clr_req_type;


/*===========================================================================

PACKET   diag_err_req_type

ID       DIAG_ERR_READ_F

PURPOSE  Sent by the DM to request the buffered error records

RESPONSE The DMSS retreives error records

===========================================================================*/
typedef PACKED struct {
  byte                   cmd_code;   /* Command code --  DIAG_ERR_READ_F */
} diag_err_req_type;


/*===========================================================================

PACKET   diag_hs_key_req_type

ID       DIAG_HS_KEY_F

PURPOSE  Sent by DM to cause a keypress input to the handset interface.

RESPONSE Diag queues a keypress to hsi and echos the request packet.

===========================================================================*/
typedef PACKED struct
{
 byte    cmd_code;                 /* Command code */
 boolean hold;                     /* If true, diag witholds key release */
 byte    key;                      /* enumerated key, e.g. HS_DOWN_K */
} diag_hs_key_req_type;


/*===========================================================================

PACKET   diag_hs_lock_req_type

ID       DIAG_HS_LOCK_F

PURPOSE  Sent by DM to lock or unlock the real handset.

RESPONSE Diag queues a command to hsi with new hs lock state, then
         echos the packet to the DM.

===========================================================================*/
typedef enum {
  HS_LOCK_F,                       /* command to lock real handset */
  HS_REL_F                         /* command to release real handset */
} diag_hs_lock_type;

typedef PACKED struct
{
 byte    cmd_code;        /* Command code                   */
 word        lock;        /* diag locks or unlocks real hs
                             Use diag_hs_lock_type.          */
} diag_hs_lock_req_type;


/*===========================================================================

PACKET   diag_screen_req_type

ID       DIAG_HS_SCREEN_F 

PURPOSE  Sent by DM to request an image of the handset screen and annun-
         ciators.

RESPONSE DMSS returns diag_screen_rsp_type packet that contains handset
         screen and annunciators.

===========================================================================*/
typedef PACKED struct
{
 byte    cmd_code;                 /* Command code */
} diag_screen_req_type;


/*===========================================================================

PACKET   diag_parm_set_req_type

ID       DIAG_PARM_SET_F

PURPOSE  Sent by DM to set one of the retreivable parameters.
         Parameters referred to here are the CAI's settable and
         retreival parameters listed in section 6.4.5 and Appendix E.

RESPONSE DMSS replaces current value of the given parameter with the one
         supplied, and sends a diag_parm_set_rsp_type packet back.
         

============================================================================*/
typedef PACKED struct {
  byte                cmd_code;               /* Command code               */
  word                parm_id;                /* Parameter value, from
                                                 cai.h.  A parm_id of 0 will
                                                 cause ALL parmameters to
                                                 be cleared.                */
  dword               parm_value;             /* New value for parameter    */
} diag_parm_set_req_type;


/*==========================================================================

PACKET   diag_nv_read_req_type

ID       DIAG_NV_READ_F

PURPOSE  Sent by DM to request the value of some NV item.

RESPONSE DMSS reads the NV item and returns it in a diag_nv_read_rsp
         (which uses the same packet id, DIAG_NV_READ)

============================================================================*/
#define DIAG_NV_ITEM_SIZE 128
typedef PACKED struct
{
 byte   cmd_code;                     /* Command code */

 uint16 item;                         /* Which item - use nv_items_enum_type */
 uint8  item_data[DIAG_NV_ITEM_SIZE]; /* Item itself - use nv_item_type      */
 uint16 nv_stat;                      /* Status of operation
                                                     - use nv_stat_enum_type */
} diag_nv_read_req_type;


/*==========================================================================

PACKET   diag_nv_write_req_type

ID       DIAG_NV_WRITE_F

PURPOSE  Sent by DM to cause DMSS to write the value of some NV item.

RESPONSE DMSS reads the NV item and returns it in a diag_nv_write_rsp
         (which uses the same packet id, DIAG_NV_WRITE_F)

============================================================================*/
typedef PACKED struct
{
 byte                cmd_code;                 /* Command code */
 
 uint16 item;                         /* Which item - use nv_items_enum_type */
 uint8  item_data[DIAG_NV_ITEM_SIZE]; /* Item itself - use nv_item_type      */
 uint16 nv_stat;                      /* Status of operation
                                                     - use nv_stat_enum_type */
} diag_nv_write_req_type;


/*===========================================================================

PACKET   diag_control_req_type

ID       DIAG_CONTROL_F

PURPOSE  Sent by DM to direct the DMSS to go offline or reset

RESPONSE DMSS changes mode or resets

===========================================================================*/
typedef enum {
    MODE_OFFLINE_A_F,           /* Go to offline analog */
    MODE_OFFLINE_D_F,           /* Go to offline digital */
    MODE_RESET_F,               /* Reset. Only exit from offline */
    MODE_MAX_F                  /* Last (and invalid) mode enum value */
} diag_mode_enum_type;

typedef PACKED struct {
  byte     cmd_code;
  word     mode;        /* Use diag_mode_enum_type. */
} diag_control_req_type;


/*===========================================================================

PACKET   diag_ser_reset_req_type

ID       DIAG_SER_RESET_F

PURPOSE  Sent by DM to direct the DMSS reset the SER counts

RESPONSE DMSS resets counts and sends reply packet acknowledging the action

===========================================================================*/

typedef PACKED struct
{
  byte cmd_code;
    /* Command code */
} diag_ser_reset_req_type;


/*===========================================================================

PACKET   diag_ser_report_req_type

ID       DIAG_SER_REPORT_F

PURPOSE  Sent by DM to direct the DMSS to report the SER counts.  They
         are not reset.

RESPONSE DMSS send reply packet containing the counts.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;
    /* Command code */
} diag_ser_report_req_type;


/*===========================================================================

PACKET   diag_test_req_type

ID       DIAG_TEST_F

PURPOSE  Sent by DM to direct the DMSS to perform a specified test.

RESPONSE DMSS performs the test, then responds.

===========================================================================*/
typedef PACKED struct {
  byte                  cmd_code;
  word                  test_code; /* Use diag_test_enum_type */
  diag_test_parm_type   parm;
} diag_test_req_type;


/*===========================================================================

PACKET   diag_dipsw_req_type

ID       DIAG_DIPSW_F

PURPOSE  Sent by DM to retreive the current dip switch settings

RESPONSE DMSS performs the test, then responds.

===========================================================================*/
typedef PACKED struct {
  byte                  cmd_code;
  word                  switches;
} diag_dipsw_req_type;


/*===========================================================================

PACKET   diag_voc_pcm_lb_type

ID       DIAG_VOC_PCM_LB_F

PURPOSE  Sent by DM to start/stop vocoder PCM loopback.

RESPONSE DMSS sends command to vocoder, then replies.

===========================================================================*/
typedef PACKED struct {
  byte                  cmd_code;
  boolean               pcm_lb;           /* TRUE => start, FALSE => stop */
} diag_voc_pcm_lb_type;


/*===========================================================================

PACKET   diag_voc_pkt_lb_type

ID       DIAG_VOC_PKT_LB_F

PURPOSE  Sent by DM to start/stop vocoder PKT loopback.

RESPONSE DMSS sends command to vocoder, then replies.

===========================================================================*/
typedef PACKED struct {
  byte                  cmd_code;
  boolean               pkt_lb;           /* TRUE => start, FALSE => stop */
} diag_voc_pkt_lb_type;


/*===========================================================================

PACKET   diag_orig_req_type

ID       DIAG_ORIG_F

PURPOSE  Sent by DM to originate a call.

RESPONSE DMSS originates a call using the specified number.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;       /* Command code */
  byte cnt;            /* number of digits in dialed_digits array */
  char dialed_digits[ NV_MAX_DIAL_DIGITS ];
  word so;
    /* desired service option - 0 = Reserved,  1 = Voice (IS96A), 2 = Loopback, 
                  8001 = Voice (IS96), 8002 = Markov, 8003 = Data (see cai.h) */
} diag_orig_req_type;


/*===========================================================================

PACKET   diag_end_req_type

ID       DIAG_END_F

PURPOSE  Sent by DM to end a call.

RESPONSE DMSS ends the current call.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;    /* Command code */
} diag_end_req_type;


/*===========================================================================

PACKET   diag_dload_req_type

ID       DIAG_DLOAD_F

PURPOSE  Sent by external device to initiate a code download.

RESPONSE DMSS prepares to receive download. Packet acknowledged,  but
         normal processing ceases.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;            /* Command code */
} diag_dload_req_type;


#ifdef MODEL_SKTT	// 00.08.23 kmh insert for chksum
/*===========================================================================

PACKET   diag_chksum_req_type

ID       DIAG_CHKSUM_F

PURPOSE  Sent by the DM to request a set of chksum data

RESPONSE The DMSS gathers the chksum and sends it in a
         diag_chksum_rsp_type packet to the DM.

============================================================================*/

typedef PACKED struct
{
  byte		cmd_code;               /* Command code */
} diag_chksum_req_type;

typedef PACKED struct
{
  byte    cmd_code;               /* Command code */
	boolean valid;
	word    chk_sum;
	word    fix_sum;
} diag_chksum_rsp_type;

#endif	// #ifdef MODEL_SKTT

/*===========================================================================

PACKET   diag_tmob_req_type

ID       DIAG_TMOB_F

PURPOSE  Sent by external device to give a command to a test mobile.

RESPONSE Test Mobile takes appropriate action, send results or ack.

NOTE     Only sent by Tester DM and only processed by Tester Mobile.         

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  tmob_cmd_type   tmob_cmd;   /* Test Mobile command */
} diag_tmob_req_type;


#ifdef FEATURE_FACTORY_TESTMODE
/*===========================================================================

PACKET   diag_ftm_req_type

ID       DIAG_FTM_CMD_F

PURPOSE  Sent by DM to request Factory Test Mode commands.

RESPONSE DMSS sends back status of the requested command.

===========================================================================*/

typedef PACKED struct
{
  byte cmd_code;
  word sub_cmd;
  word length;      /* Length of the request[] */
  byte request[1];  /* Actual data content varies with request */
} diag_ftm_req_type;
#endif


/*===========================================================================

PACKET   diag_state_req_type

ID       DIAG_STATE_F

PURPOSE  Sent by external device to obtain the current state of operation
         the phone is in.

RESPONSE The diag_state_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;              /* Command code */
} diag_state_req_type;


/*===========================================================================

PACKET   diag_pilot_sets_req_type

ID       DIAG_PILOT_SETS_F

PURPOSE  Sent by external device to obtain the current contents of the 
         various pilot sets.

RESPONSE The diag_pilot_sets_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;              /* Command code */
} diag_pilot_sets_req_type;


/*===========================================================================

PACKET   diag_spc_req_type

ID       DIAG_SPC_F

PURPOSE  Sent by external device to enter the Service Programming Code,
         to then allow Service Programming.

RESPONSE The diag_spc_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  nv_sec_code_type sec_code;  /* The security code */
} diag_spc_req_type;


/*===========================================================================

PACKET   diag_parm_get2_req_type

ID       DIAG_PARM_GET2_F

PURPOSE  Sent by DM to request the set of retreivable parameters.
         Parameters referred to here are the CAI's settable and
         retreival parameters listed in section 6.4.5 and Appendix E.
         This packet includes the MUX2 parameters listed in 6.4.5.2

RESPONSE DMSS places current value of all parameters in a diag_parm_
         get2_rsp_type packet and sends it back.
         

============================================================================*/
typedef PACKED struct {
  byte                cmd_code;               /* Command code */
} diag_parm_get2_req_type;


/*===========================================================================

PACKET   diag_ser_mode_chng_req_type

ID       DIAG_SERIAL_CHG_F

PURPOSE  Sent by payphone, or DM to change from Call Control Mode to 
         Data Call Mode

RESPONSE DMSS commands the data services to switch to AT command processor 
         mode, and sends a diag_ser_mode_chng_rsp_type packet with whether
         the command was successful or not.

============================================================================*/
typedef PACKED struct {
  byte                cmd_code;               /* Command code */
} diag_ser_mode_chng_req_type;


/*===========================================================================

PACKET   diag_password_req_type

ID       DIAG_PASSWORD_F

PURPOSE  Sent by external device to enter the Security Password.

RESPONSE The diag_password_rsp_type will be sent in response.

===========================================================================*/
#define DIAG_PASSWORD_SIZE 8
typedef PACKED struct
{
  byte cmd_code;                      /* Command code */
  byte password[DIAG_PASSWORD_SIZE];  /* The security password */
} diag_password_req_type;


/*===========================================================================

PACKET   diag_pr_list_wr_req_type

ID       DIAG_PR_LIST_WR_F

PURPOSE  Sent by external device to write a Preferred Roaming List to 
         the phone.

RESPONSE The diag_pr_list_wr_rsp_type will be sent in response.

===========================================================================*/
#define DIAG_PR_LIST_BLOCK_SIZE 120   /* Size in bytes of the PR_LIST block */
typedef PACKED struct
{
  byte cmd_code;                      /* Command code */
  byte seq_num;                       /* Sequence number */
  byte more;                          /* More to come? */
  byte nam;                           /* which NAM this is associated with */
  word num_bits;                      /* length in bits of valid data */
  byte pr_list_data[DIAG_PR_LIST_BLOCK_SIZE];  /* The block of PR_LIST */
} diag_pr_list_wr_req_type;


/*===========================================================================

PACKET   diag_pr_list_rd_req_type

ID       DIAG_PR_LIST_RD_F

PURPOSE  Sent by external device to read a Preferred Roaming List from 
         the phone.

RESPONSE The diag_pr_list_rd_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                      /* Command code */
  byte seq_num;                       /* Sequence number */
  byte nam;                           /* PR_LIST for this NAM */
} diag_pr_list_rd_req_type;


/*===========================================================================

PACKET   diag_feature_query_req_type

ID       DIAG_FEATURE_QUERY_F

PURPOSE  Sent by external device to query the phone for a bit mask detailing
         which phone features are turned on.

RESPONSE The diag_feature_query_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                      /* Command code */
} diag_feature_query_req_type;


/*===========================================================================

PACKET   diag_sms_read_req_type

ID       DIAG_SMS_READ_F

PURPOSE  This message requests a read of the given SMS message out of NV

RESPONSE The diag_sms_read_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  byte address;                                /* Which message to get */
} diag_sms_read_req_type;


/*===========================================================================

PACKET   diag_sms_write_req_type

ID       DIAG_SMS_WRITE_F

PURPOSE  This message requests a write of the given SMS message into NV.
         It is a variable length request message - only as long as dictated
         by the length field in sms_message.

RESPONSE The diag_sms_write_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  nv_sms_type sms_message;                     /* SMS message to write */
} diag_sms_write_req_type;

// chojw 00.09.30 for SKTT AS Tool
#if defined(FEATURE_SKTT_ASTOOL) && ((MODEL_ID == MODEL_D1) || (MODEL_ID == MODEL_A10))

typedef PACKED struct {
	byte								cmd_code;
} diag_phonebook_num_req_type;

typedef PACKED struct {
	byte								cmd_code;
	boolean							isdial[300];
} diag_phonebook_num_rsp_type;

typedef PACKED struct {
	byte								cmd_code;
	int 								address;
} diag_phonebook_read_req_type;

typedef PACKED struct {
	byte								cmd_code;
	nv_dial_type				dial;
} diag_phonebook_read_rsp_type;

typedef PACKED struct {
	byte								cmd_code;
	nv_dial_type 				dial;
} diag_phonebook_write_req_type;

typedef PACKED struct {
	byte								cmd_code;
} diag_phonebook_write_rsp_type;

typedef PACKED struct {
	byte								cmd_code;
} diag_sdial_num_req_type;

typedef PACKED struct {
	byte								cmd_code;
	boolean							issdial[NV_MAX_SDIAL];
} diag_sdial_num_rsp_type;

typedef PACKED struct {
	byte								cmd_code;
} diag_group_num_req_type;

typedef PACKED struct {
	byte								cmd_code;
	boolean							isgroup[NV_MAX_GROUP];
} diag_group_num_rsp_type;

typedef PACKED struct {
	byte								cmd_code;
} diag_invalid_req_type;

typedef PACKED struct {
	byte								cmd_code;
} diag_invalid_rsp_type;

#endif // defined(FEATURE_SKTT_ASTOOL) && (MODEL_ID == MODEL_D1)


#if (defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR))
/*===========================================================================

PACKET   diag_sup_fer_req_type

ID       DIAG_SUP_FER_F

PURPOSE  Sent by the DM to request FER info for MDR targets - which use
         supplemental channels.

RESPONSE The DMSS prepares and sends a diag_sup_fer_rsp_type packet.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;                /* Command code */
} diag_sup_fer_req_type;


/*===========================================================================

PACKET   diag_sup_walsh_codes_req_type

ID       DIAG_SUP_WALSH_CODES_F

PURPOSE  Sent by DM to request pn offset and walsh codes for supplemental
         channels.

RESPONSE The diag_sup_walsh_codes_rsp_type will be sent in response.

============================================================================*/
typedef PACKED struct {
  byte                cmd_code;               /* Command code */
} diag_sup_walsh_codes_req_type;


/*===========================================================================

PACKET   diag_set_max_sup_ch_req_type

ID       DIAG_SET_MAX_SUP_CH_F

PURPOSE  This message requests to set the maximum number of allowable 
         supplemental channels to be used by MDR.
         The service option represents a rate set (MUX1, MUX2).

RESPONSE The diag_set_max_sup_ch_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;            /* Command code */
  word service_option;      /* Specifies service option (rate set) */
  byte num_sup_ch;          /* Number of supplemental channels to be allowed*/
} diag_set_max_sup_ch_req_type;


/*===========================================================================

PACKET   diag_parm_get_is95b_req_type

ID       DIAG_PARM_GET_IS95B_F

PURPOSE  Sent by DM to request the set of retreivable parameters.
         Parameters referred to here are the CAI's settable and
         retreival parameters listed in section 6.4.5 and Appendix E.
         This packet includes the MUX2 parameters listed in 6.4.5.2 as
         well as the SUPP (supplemental channel) parameters in 6.4.5.2 of
         the TIA/EIA IS-95-B standard.        

RESPONSE DMSS places current value of all parameters in a 
         diag_parm_get_is95b_rsp_type packet and sends it back.
         

============================================================================*/
typedef PACKED struct {
  byte                cmd_code;               /* Command code */
} diag_parm_get_is95b_req_type;
#endif /* defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) */


#ifdef FEATURE_EFS
/*===========================================================================

PACKET   diag_fs_op_req_type

ID       DIAG_FS_OP_F

PURPOSE  This message requests a filesystem operation.  This message contains
         sub-commands to specify which file system operation to perform.
         It is a variable length request message - depending on the file
         system operation.

RESPONSE The diag_fs_op_rsp_type will be sent in response.

===========================================================================*/
typedef enum
{
  DIAG_FS_MK_DIR      = 0,      /* Create directory                */
  DIAG_FS_RM_DIR      = 1,      /* Remove Directory                */
  DIAG_FS_DISP_DIRS   = 2,      /* Display directories list        */
  DIAG_FS_DISP_FILES  = 3,      /* Display file list               */
                                /* NOTE: Display ops are temporary 
                                   implementations                 */ 
  DIAG_FS_READ_FILE   = 4,      /* Read a file in FS               */
  DIAG_FS_WRITE_FILE  = 5,      /* Write a file in FS              */  
  DIAG_FS_REMOVE_FILE = 6,      /* Remove a file from FS           */  
  DIAG_FS_GET_ATTRIB  = 7,      /* Get file attributes             */
  DIAG_FS_SET_ATTRIB  = 8,      /* Set file attributes             */
  DIAG_FS_REMOTE_LINK = 9,      /* Create a remote file link       */
  DIAG_FS_ITER_DIRS   = 10,     /* Iterative display directories   */
  DIAG_FS_ITER_FILES  = 11,     /* Iterative display files         */
  DIAG_FS_SPACE_AVAIL = 12,     /* Amount of free space in EFS     */
  DIAG_FS_LAST_OP               /* Last OP.  For range checking.   */
} diag_fs_op_enum_type;

/*--------------------------------------------------------------
   Generic structure definitions used in multiple operations.
--------------------------------------------------------------*/

#define DIAG_FS_MAX_FILENAME_LEN      60 /* Specified by FS              */
#define DIAG_FS_MAX_FILE_BLOCK_SIZE  256 /* Limited by req buffer of 256 */

typedef PACKED struct
{
  byte len;                            /* Length of filename string 
                                          including NULL terminating char   */
  char name[DIAG_FS_MAX_FILENAME_LEN]; /* Filename string.  NULL terminated.
                                          Valid data len == filename_len+1  */
} diag_fs_filename_type;

typedef PACKED struct
{
  word len;                               /* Length of data block */
  byte data[DIAG_FS_MAX_FILE_BLOCK_SIZE]; /* Data block           */
} diag_fs_data_block_type;

/* File attribute mask */
typedef enum {
  DIAG_FS_FA_UNRESTRICTED =0x00FF,  /* No file access restrictions           */
  DIAG_FS_FA_PERMANENT    =0x007F,  /* File can't be removed nor truncated   */
  DIAG_FS_FA_READONLY     =0x00BF,  /* File can't be opened for update       */
  DIAG_FS_FA_SYS_PERMANENT=0x0037,  /* File persists across system reformats */
  DIAG_FS_FA_REMOTE       =0x001F   /* Remote file (resides outside file     */
                                    /* system address space)                 */
} diag_fs_attribute_mask_type;     

/* File OPEN operation buffering options */
typedef enum {
  DIAG_FS_OB_PROHIBIT,             /* Prohibit file buffering                */
  DIAG_FS_OB_ALLOW                 /* Allow file buffering                   */
} diag_fs_buffering_option_type;

/* File OPEN operation clean-up options */
typedef enum {
  DIAG_FS_OC_CLOSE     = 0x00, /* Close file as-is                           */
  DIAG_FS_OC_DISCARD   = 0x01, /* Delete file and remove it from directory   */
  DIAG_FS_OC_TRUNCATE  = 0x02, /* Truncate file from specified position      */
  DIAG_FS_OC_REVERT    = 0x03  /* Revert to last checkpointed version        */
} diag_fs_cleanup_option_type;

typedef PACKED struct
{
  word      attribute_mask;  /* Use diag_fs_attribute_mask_type   */
  byte    buffering_option;  /* Use diag_fs_buffering_option_type */
  byte      cleanup_option;  /* Use diag_fs_cleanup_option_type   */
} diag_fs_attributes_type;

/*-------------------------------------------------------
  Definitions of data for specific operations.
-------------------------------------------------------*/
/* "Create Directory" operation */
typedef diag_fs_filename_type diag_fs_mkdir_req_type;

/* "Remove Directory" operation */
typedef diag_fs_mkdir_req_type diag_fs_rmdir_req_type;

/* "Display Directory List" operation */
typedef diag_fs_filename_type diag_fs_disp_dirs_req_type;

/* "Display File List" operation */
typedef diag_fs_disp_dirs_req_type diag_fs_disp_files_req_type;

/* "Read File" operation */
typedef PACKED struct
{
  byte seq_num;                        /* Sequence number for mult blocks */
  
  diag_fs_filename_type filename_info; /* File name info                  */
                                       /* Used only if seq_num == 0       */
} diag_fs_read_req_type;

/* "Write File" operation */
typedef enum
{
  DIAG_FS_NO_OVERWRITE = 0,
  DIAG_FS_OVERWRITE    = 1
} diag_fs_write_mode_enum_type;

typedef PACKED struct
{ 
  byte                          mode; /* Use diag_fs_write_mode_enum_type  */
  dword                 total_length; /* Total length of this file         */
  diag_fs_attributes_type     attrib; /* Attributes for this file          */
  
  PACKED union
  {
    diag_fs_filename_type  name_info;    /* File name info                 */
    byte raw_data[sizeof(diag_fs_filename_type) +
                  sizeof(diag_fs_data_block_type)];
                                         /* Full possible size of variable
                                            length buffer                  */
  } var_len_buf;
} diag_fs_write_begin_req_type;

typedef PACKED union
{
  diag_fs_write_begin_req_type  begin; /* First block of a write           */
  diag_fs_data_block_type      subseq; /* Subsequent blocks for write      */
} diag_fs_write_block_type;

typedef PACKED struct
{
  byte seq_num;                        /* Sequence number for mult blocks    */
  byte    more;                        /* Flag if more packets are needed to
                                          complete write                     */
  diag_fs_write_block_type     block;  /* Block for this portion of the file */
} diag_fs_write_req_type;

/* "Get File Attributes" operation */
typedef diag_fs_filename_type diag_fs_get_attrib_req_type;

/* "Set File Attributes" operation */
typedef PACKED struct
{
  diag_fs_attributes_type       attribs;
  dword                   creation_date;
  diag_fs_filename_type   filename_info;
} diag_fs_set_attrib_req_type;

/* "Remove File" operation */
typedef diag_fs_filename_type diag_fs_rmfile_req_type;

/* "Remote File Link" operation */
typedef PACKED struct
{
  dword               base_address;
  dword                     length;
  diag_fs_filename_type  name_info;
} diag_fs_remote_link_req_type;

/* "Iterate Directories" operation */
typedef PACKED struct
{
  dword seq_num;
  diag_fs_filename_type dir_name;
} diag_fs_iter_dirs_req_type;

/* "Iterate Files" operation */
typedef diag_fs_iter_dirs_req_type diag_fs_iter_files_req_type;

/* Union of all possible operations.  Determined by cmd_code */
typedef PACKED union
{                               
  byte                             seq_num; /* Seq number in same place for
                                               all packets that use them */
  diag_fs_mkdir_req_type             mkdir;
  diag_fs_rmdir_req_type             rmdir;
  diag_fs_read_req_type               read;
  diag_fs_write_req_type             write;
  diag_fs_disp_dirs_req_type     disp_dirs;
  diag_fs_disp_files_req_type   disp_files;
  diag_fs_get_attrib_req_type   get_attrib;
  diag_fs_set_attrib_req_type   set_attrib;
  diag_fs_rmfile_req_type           rmfile;
  diag_fs_remote_link_req_type remote_link;
  diag_fs_iter_dirs_req_type     iter_dirs;
  diag_fs_iter_files_req_type   iter_files;
} diag_fs_req_type;

typedef PACKED struct
{
  byte cmd_code;                /* Command code                  */
  byte file_op;                 /* From diag_fs_op_enum_type     */
                          
  diag_fs_req_type fs_req;      /* Filesystem request data union */
} diag_fs_op_req_type;
#endif /* FEATURE_EFS */



/*===========================================================================

PACKET   diag_akey_verify_req_type

ID       DIAG_AKEY_VERIFY_F

PURPOSE  Sent by DM to verify AKEY update.  Contains AKEY.  DM or System
         Unit can ask AKEY update for a particular NAM or if NAM is 0xff, 
         AKEY for current nam will be updated.

RESPONSE DMSS responds with message saying whether or not AKEY update was
         successful.

============================================================================*/
#define AKEY_REQ_RESERVED 8

typedef PACKED struct {
  byte                cmd_code;                    /* Command code          */
  byte                akey[UI_A_KEY_DIGITS];       /* AKEY with             */
                                                   /* cryptographic check   */
  byte                nam;                         /* NAM for which AKEY    */
                                                   /* applies               */
                                                   /* If NAM is set to 0xff,*/
                                                   /* AKEY for current NAM  */
                                                   /* is changed            */
  byte                reserved[AKEY_REQ_RESERVED]; /* for future expansion  */
} diag_akey_verify_req_type;



#if defined(FEATURE_BITMAP_RASTER)
#error code not present
#endif



/*===========================================================================

PACKET   diag_config_comm_req_type

ID       DIAG_CONFIG_COMM_F 

PURPOSE  Sent by DM to request a baud rate changefor DIAG serial 
         communications.

RESPONSE DMSS returns diag_config_comm_rsp_type packet that contains 
         possible configurations or an acknowledgement to a change request.
         Upon sending 

===========================================================================*/
typedef enum
{
  DIAG_RS232_CONFIG_INFO = 0, /* Request for baud rate capabilities */
  DIAG_RS232_CONFIG,          /* Set baud rate                      */
  DIAG_USB_CONFIG_INFO,       /* UNIMPLEMENTED!!!                   */
  DIAG_USB_CONFIG             /* UNIMPLEMENTED!!!                   */
} diag_config_comm_enum_type;

typedef PACKED struct
{
 byte    cmd_code;  /* Command code                         */
 byte     sub_cmd;  /* Use diag_config_comm_enum_type       */
 dword   baudrate;  /* Baud rate to be set - use only in a  */
                    /* config command, not an info request  */
} diag_config_comm_req_type;


/*===========================================================================

PACKET   diag_extended_logmask_req_type

ID       DIAG_EXT_LOGMASK_F

PURPOSE  Sent by the DM to set the logging mask in the DMSS.  This is
         necessary for logmasks > 32 bits.

RESPONSE The DMSS establishes the logging mask and replies with a
         diag_ext_logmask_rsp_type packet

===========================================================================*/
#define DIAG_LOG_MASK_NUMBYTES ((LOG_LAST_C / 8) + 1)
typedef PACKED struct
{
  byte  cmd_code;                     /* Command code            */
  word  num_bits;                     /* Number of valid bits    */
  byte  mask[DIAG_LOG_MASK_NUMBYTES]; /* mask to use             */
} diag_ext_logmask_req_type;


#if !defined(FEATURE_DIAG_NO_EVENTS)
/*===========================================================================

PACKET   diag_event_report_req_type

ID       DIAG_EVENT_REPORT_F

PURPOSE  Sent by the DM to configure static event reporting in the DMSS.

RESPONSE The DMSS will return an acknowledgement 

===========================================================================*/
typedef PACKED struct
{
  byte  cmd_code;    /* Command code             */
  byte  enable;      /* Boolean-events enabled?  */
//  word  watermark;   /* Maximum size (in bytes) of a event report         */ 
//  word  stale_timer; /* Time (in ms) to allow event buffer to accumulate  */
} diag_event_report_req_type;
#endif


/*===========================================================================

PACKET   diag_streaming_config_req_type

ID       DIAG_STREAMING_CONFIG_F

PURPOSE  Sent by the DM to configure and tweak streaming DIAG output services.

RESPONSE The DMSS will return diag_streaming_config_rsp_type.

===========================================================================*/
typedef enum
{
  DIAG_READ_NICE_C  = 0, /* Read "Nice" values for LOG and MSG services  */
  DIAG_WRITE_NICE_C = 1, /* Write "Nice" values for LOG and MSG services */
  DIAG_READ_PRI_C   = 2, /* Read "priority" values for LOG and MSG services  */
  DIAG_WRITE_PRI_C  = 3  /* Write "priority" values for LOG and MSG services */

} diag_streaming_config_subcommand_enum_type;


typedef PACKED struct
{
  uint8  cmd_code;   /* Command code  */

  uint8  subcommand; /* Sub-command - diag_streaming_config_subcommand_enum_type */

  PACKED union {

    /* No data for DIAG_READ_NICE_C */

    /* DIAG_WRITE_NICE_C */
    PACKED struct {
      uint8 num_entries; /* Number of entries in the following array */

      PACKED struct {
        uint16 code; /* MSG_LVL value or Log code */

        int16  val;
      
      } entry[LOGMASK_NUM_BITS + 5 /* 5 Message Levels */];

    } write;

  } command_info;

} diag_streaming_config_req_type;



/*===========================================================================

PACKET   diag_parm_retrieve_req_type

ID       DIAG_PARM_RETRIEVE_F

PURPOSE  Sent by DM to set or retriecce a all or a subset set of retreivable
         parameters.  Parameters referred to here are the CAI's settable and
         retreival parameters listed in section 6.4.5 and Appendix E.

RESPONSE DMSS places the current value of all parameters in the range 
         specified.
         A range of 0--0 will return the full list of parameters.
         If an invalid range is speciied, DIAG_BAD_PARM_F is returned.

============================================================================*/
typedef enum {
  DIAG_PARM_STANDARD_RANGE_C = 0, /* Return range of valid standard PARMs */
  DIAG_PARM_STANDARD_C       = 1, /* PARM defined in TIA/EIA standard     */
  DIAG_PARM_EXTRA_RANGE_C    = 2, /* Return range of valid extra PARMs    */
  DIAG_PARM_EXTRA_C          = 3  /* PARM defined in TIA/EIA standard         */

} diag_parm_subcmd_enum_type;

typedef PACKED struct {
  uint8  cmd_code;      /* Command code */

  uint8  subcmd;        /* Use diag_parm_subcmd_enum_type */

  uint16 start_parm_id; /* refer to CAI */
  uint16 end_parm_id;   /* refer to CAI */

} diag_parm_retrieve_req_type;



#if defined(FEATURE_DIAG_RPC)
#error code not present
#endif

#ifdef FEATURE_FACTORY_TESTMODE

/*===========================================================================

PACKET   diag_ftm_req_type

ID       DIAG_FTM_CMD_F

PURPOSE  Sent by DM to request Factory Test Mode commands.

RESPONSE DMSS sends back status of the requested command.

===========================================================================*/

typedef PACKED struct
{
  byte cmd_code;
  word sub_cmd;
  word length;      /* Length of the request[] */
  byte request[1];  /* Actual data content varies with request */
} diag_ftm_req_type;
#endif


/*---------------------------------------------------------------------------
   Operating modes for packet processing.  These masks are used to define
   permissions for packet processing in the dispatch table.
---------------------------------------------------------------------------*/
#define MODE_OFF_DIG  0x01  /* Packet accepted in offline-digital        */
#define MODE_OFF_ANA  0x02  /* Packet accepted in offline-analog         */
#define MODE_ONLINE   0x04  /* Packet accepted in online                 */

#ifndef FEATURE_WAP
#define MODE_ANY      0xFF  /* Packet accepted in all modes.             */
#endif //FEATURE_WAP

#define MODE_OFFLINE  ( MODE_OFF_DIG | MODE_OFF_ANA ) 
                              /* Packet accepted in either offline mode    */

/*---------------------------------------------------------------------------
   Security for packet processing.  These masks are used to define
   whether a packet can be accepted when the security is locked or
   unlocked, for packet processing in the dispatch table.
---------------------------------------------------------------------------*/
#define SEC_LOCKED    TRUE  /* Packet accepted if security is locked   */
#define SEC_UNLOCKED  FALSE /* Packet accepted if security is unlocked */
// taegon
#define ESN_LOCKED    TRUE
#define ESN_UNLOCKED  FALSE


/*===========================================================================

PACKET   DIAG_SUBSYS_CMD_F
PURPOSE  This is a wrapper command to dispatch DIAG commands to various 
         subsystems in the DMSS.  This allows new DIAG commands to be 
         developed in those subsystems without requiring a change to DIAG 
         code. 
         
         The use of this command helps de-couple diag.
===========================================================================*/
typedef enum
{
  DIAG_SUBSYS_CMD_RSVD_0 = 0,
  DIAG_SUBSYS_ZREX       = 1,
  DIAG_SUBSYS_SD         = 2,
  DIAG_SUBSYS_BT         = 3,
  DIAG_SUBSYS_LAST
} diag_subsys_cmd_enum_type;

DIAGPKT_REQ_DEFINE(DIAG_SUBSYS_CMD_F)
  
  uint8 subsys_code; /* Subsystem ID */
  uint8 pkt[1];

DIAGPKT_REQ_END

DIAGPKT_RSP_DEFINE(DIAG_SUBSYS_CMD_F)
  
  uint8 subsys_code; 
  byte pkt[1];

DIAGPKT_RSP_END


/*===========================================================================

PACKET   DIAG_STATUS_SNAPSHOT_F
PURPOSE  This command gives a variable length response snapshot of all 
  information needed at the start of logging to accuratley analyze the log 
  data.

===========================================================================*/
DIAGPKT_REQ_DEFINE(DIAG_STATUS_SNAPSHOT_F)
  
DIAGPKT_REQ_END

DIAGPKT_RSP_DEFINE(DIAG_STATUS_SNAPSHOT_F)
  
  uint32 esn;
  
  PACKED struct {
    uint32 imsi_s1;       /* First part of Operational IMSI (IMSI_O_S1) */
    uint16 imsi_s2;        /* Second part of Operational IMSI (IMSI_O_S2) */
    uint64 imsi_s;        /* IMSI_S1 and IMSI_S2 converted to a qword imsi_s */
    uint8  imsi_11_12;     /* Mobile Network Code (MNC) for IMSI_O address */
    uint16 mcc;            /* Mobile Country Code (MCC) for IMSI_O address */
    uint8  imsi_addr_num;  /* Number of digits in NMSI - 4, or class 0 IMSI flag */
  } full_imsi;

  uint16 sid;
  uint16 nid;
  uint8  p_rev;       
  uint8  prev_in_use;
  uint8  mob_p_rev;
  uint8  band_class;
  uint16 frequency;
  uint8  operation_mode; //CDMA, Analog, GPS, etc (AKA rf_mode) -LD 
  
  /* MC state: Initialization, Idle, Access, Traffic */
  uint8  state; 
  
  /* The sub-state depends on the value in the above 'state' value.
  ** init state:    System Determination
  **                Pilot Channel Acquisition
  **                Sync Channel Acquisition
  **                Timing Change
  ** idle state:    Slotted mode 
  **                Non-slotted mode
  ** access state:  Update Overhead Information
  **                Registration Access
  **                Order/Message Response
  **                Transmission
  **                Origination Attempt
  **                Page Response
  ** traffic state: Traffic Channel Initialization
  **                Waiting for Order
  **                Waiting for MS Answer
  **                Conversation
  **                Release
  */
  uint8  sub_state;
  
DIAGPKT_RSP_END


/*===========================================================================

PACKET   DIAG_RLP_STAT_RESET_F
PURPOSE  Reset RLP accumulated statistics

===========================================================================*/

DIAGPKT_REQ_DEFINE( DIAG_RLP_STAT_RESET_F )

  uint8  sr_id;

DIAGPKT_REQ_END

DIAGPKT_RSP_DEFINE( DIAG_RLP_STAT_RESET_F )

DIAGPKT_RSP_END

/*===========================================================================

PACKET   DIAG_TDSO_STAT_RESET_F
PURPOSE  Reset (S)TDSO accumulated statistics

===========================================================================*/

DIAGPKT_REQ_DEFINE( DIAG_TDSO_STAT_RESET_F )

DIAGPKT_REQ_END

DIAGPKT_DEFINE_RSP_AS_REQ( DIAG_TDSO_STAT_RESET_F )

/*===========================================================================

Union of packet types which convey requests from the Diagnostic Monitor to 
   the Mobile

===========================================================================*/

typedef PACKED union
{
  byte                       cmd_code;
  diag_verno_req_type           verno;
  diag_verstr_req_type         verstr;
  diag_esn_req_type               esn;
  diag_peek_req_type             peek;
  diag_pokeb_req_type           pokeb;
  diag_pokew_req_type           pokew;
  diag_poked_req_type           poked;
  diag_outp_req_type             outp;
  diag_outpw_req_type           outpw;
  diag_inp_req_type               inp;
  diag_inpw_req_type             inpw;
  diag_status_req_type           stat;
  diag_logmask_req_type       logmask;
#if defined(FEATURE_DIAG_NON_STREAMING)
  diag_log_req_type               log;
#endif
  diag_nv_peek_req_type       nv_peek;
  diag_nv_poke_req_type       nv_poke;
  diag_tagraph_req_type       tagraph;
  diag_markov_req_type         markov;
  diag_markov_reset_req_type  m_reset;
  diag_diag_ver_req_type     diag_ver;
  diag_ts_req_type                 ts;
  diag_ta_parm_req_type       ta_parm;
  diag_msg_req_type           msg_req;
  diag_hs_key_req_type         hs_key;
  diag_hs_lock_req_type       hs_lock;
  diag_screen_req_type     screen_req;
  diag_parm_set_req_type     parm_set;
  diag_nv_read_req_type       nv_read;
  diag_nv_write_req_type     nv_write;
  diag_control_req_type          cont;
  diag_err_req_type           err_req;
  diag_err_clr_req_type       err_clr;
  diag_ser_reset_req_type   ser_reset;
  diag_ser_report_req_type ser_report;
  diag_test_req_type             test;
  diag_dipsw_req_type       dipsw_req;
  diag_voc_pcm_lb_type     vocpcm_req;
  diag_voc_pkt_lb_type     vocpkt_req;
  diag_orig_req_type             orig;
  diag_end_req_type               end;
  diag_dload_req_type           dload;
  diag_tmob_req_type             tmob;
  diag_state_req_type       state_req;
  diag_pilot_sets_req_type pilot_sets;
  diag_spc_req_type           spc_req;
  diag_parm_get2_req_type   parm_get2;
  diag_ser_mode_chng_req_type     ser_mode_chng;
  diag_password_req_type           password_req;
  diag_pr_list_wr_req_type       pr_list_wr_req;
  diag_pr_list_rd_req_type       pr_list_rd_req;
  diag_feature_query_req_type feature_query_req;
  diag_sms_read_req_type               sms_read;
  diag_sms_write_req_type             sms_write;
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  diag_sup_fer_req_type                 sup_fer;
  diag_sup_walsh_codes_req_type     walsh_codes;  
  diag_set_max_sup_ch_req_type   set_max_sup_ch;
  diag_parm_get_is95b_req_type   parm_get_is95b;
#endif
#ifdef FEATURE_EFS
  diag_fs_op_req_type                     fs_op;
#endif
  diag_akey_verify_req_type         akey_verify;
#if defined(FEATURE_BITMAP_RASTER)
#error code not present
#endif
  diag_config_comm_req_type         config_comm;
  diag_ext_logmask_req_type         ext_logmask;
#ifdef FEATURE_SKT_DEBUG
  diag_sktest_req_type				sktest;
#endif
#if !defined(FEATURE_DIAG_NO_EVENTS)
  diag_event_report_req_type       event_report;
#endif
  diag_streaming_config_req_type  stream_config;
  diag_parm_retrieve_req_type     parm_retrieve;
#if defined(FEATURE_DIAG_RPC) 
#error code not present
#endif
#ifdef FEATURE_DIAG_QCT_EXT
  diag_get_property_req_type        get_property;
  diag_put_property_req_type        put_property;
  diag_get_guid_req_type            get_guid;
  diag_user_cmd_req_type            user_cmd;
  diag_get_perm_property_req_type   get_perm_property;
  diag_put_perm_property_req_type   put_perm_property;
  diag_perm_user_cmd_req_type       perm_user_cmd;
#endif
#ifdef FEATURE_FACTORY_TESTMODE
  diag_ftm_req_type                 ftm;
#endif
#if defined(FEATURE_SKTT_ASTOOL) && ((MODEL_ID == MODEL_D1) || (MODEL_ID == MODEL_A10))// 00.09.30 chojw
	diag_phonebook_num_req_type     phonebook_num;
	diag_phonebook_read_req_type	phonebook_read;
	diag_phonebook_write_req_type	phonebook_write;
	diag_sdial_num_req_type			sdial_num;
	diag_group_num_req_type			group_num;
	diag_invalid_req_type			invalid;
#endif // defined(FEATURE_SKTT_ASTOOL) && (MODEL_ID == MODEL_D1)

#ifdef MODEL_SKTT	// 00.08.23 kmh insert for chksum
	diag_chksum_req_type    chksum;
#endif	// #ifdef MODEL_SKTT
} diag_req_type;


/*--------------------------------------------------------------------------
  Define the structure for the incoming packet.  The sio services place the
  crc byte into the packet for us, so a buffer will have a packet plus the
  crc.
--------------------------------------------------------------------------*/


typedef PACKED struct
{
  diag_req_type        req;              /* Body of the packet            */
  word                 crc;              /* CRC                           */
  byte                 control_char;     /* Control character             */
} diag_req_pkt_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*     Communication from the Mobile to the Diagnostic Monitor             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/*============================================================================

PACKET   diag_verno_rsp_type

ID       DIAG_VERNO_F

PURPOSE  Sent by the DMSS, contains version and date information

============================================================================*/
#define VERNO_DATE_STRLEN 11
#define VERNO_TIME_STRLEN  8
#define VERNO_DIR_STRLEN   8

typedef PACKED struct
{
  byte cmd_code;                              /* Command code               */
  char comp_date[ VERNO_DATE_STRLEN ];        /* Compile date Jun 11 1991   */
  char comp_time[ VERNO_TIME_STRLEN ];        /* Compile time hh:mm:ss      */
  char rel_date [ VERNO_DATE_STRLEN ];        /* Release date               */
  char rel_time [ VERNO_TIME_STRLEN ];        /* Release time               */
  char ver_dir  [ VERNO_DIR_STRLEN ];         /* Version directory          */
  byte scm;                                   /* Station Class Mark         */
  byte mob_cai_rev;                           /* CAI rev                    */
  byte mob_model;                             /* Mobile Model               */
  word mob_firm_rev;                          /* Firmware Rev               */
  byte slot_cycle_index;                      /* Slot Cycle Index           */
  byte hw_maj_ver;                            /* Hardware Version MSB       */
  byte hw_min_ver;                            /* Hardware Version LSB       */
} diag_verno_rsp_type;


/*===========================================================================

PACKET   diag_verstr_rsp_type

ID       DIAG_VERSTR_F

PURPOSE  Sent by the DMSS, contains string version number

===========================================================================*/
#define VERSTR_STRLEN 33
typedef PACKED struct
{
  byte cmd_code;
  char version[ VERSTR_STRLEN ];
} diag_verstr_rsp_type;


/*===========================================================================

PACKET   diag_esn_rsp_type

ID       DIAG_ESN_F

PURPOSE  Sent by DMSS, contains ESN in dword format

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  dword  esn;                  /* Electronic Serial Number */
} diag_esn_rsp_type;


/*===========================================================================

PACKET   diag_peekb_rsp_type

ID       DIAG_PEEKB_F

PURPOSE  Sent by DMSS, contains values requested in diag_peekb_req_type

============================================================================*/

/* No special response type is required for Poke, OUTP or OUTPW.  In
   these cases the received request types are returned to the Diagnostic 
   Monitor because the Diagnostic Monitor does not require further 
   information */

#define DIAG_MAX_PEEK_B 16
  /* Maximum number of bytes that can be requested in one Peekb request */

/* Peekb Response type */
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  byte *ptr;                   /* starting address for peek operation */
  word length;                 /* number of bytes to be returned */
  byte data[DIAG_MAX_PEEK_B];  /* bytes from memory */
} diag_peekb_rsp_type;


/*===========================================================================

PACKET   diag_peekw_rsp_type

ID       DIAG_PEEKW_F

PURPOSE  Sent by DMSS, contains values requested in diag_peekw_req_type

============================================================================*/
#define DIAG_MAX_PEEK_W  8 
  /* Maximum number of words that can be requested in one Peekw request */

/* Peekw Response type */
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word *ptr;                   /* starting address for peek operation */
  word length;                 /* number of words to be returned */
  word data[DIAG_MAX_PEEK_W];  /* words from memory */
} diag_peekw_rsp_type;


/*==========================================================================

PACKET   diag_peekd_rsp_type

ID       DIAG_PEEKD_F

PURPOSE  Sent by DMSS, contains dwords requested in diag_peekd_req_type

===========================================================================*/
#define DIAG_MAX_PEEK_D  4
  /* Maximum number of dwords that can be requested in one Peekd request */

/* Peekd Response type */
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  dword *ptr;                  /* starting address for peek operation */
  word length;                 /* number of dwords to be returned */
  dword data[DIAG_MAX_PEEK_D]; /* dwords from memory */
} diag_peekd_rsp_type;


/*===========================================================================

PACKET   diag_inp_rsp_type

ID       DIAG_INP_F

PURPOSE  Sent by DMSS, contains the byte input from io port

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word port;                   /* port read from */
  byte data;                   /* value read from port */
} diag_inp_rsp_type;


/*==========================================================================

PACKET   diag_inpw_rsp_type

ID       DIAG_INPW_F

PURPOSE  Sent by DMSS, contains the word input from io port

==========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word port;                   /* port that was read from */
  word data;                   /* value read from port */
} diag_inpw_rsp_type;


/*==========================================================================

PACKET   diag_status_rsp_type

ID       DIAG_STATUS_F

PURPOSE  Sent by DMSS, contains status information

===========================================================================*/

/*----------------------------------------------------------- 
   The following 2 numbers are for the entry_reason in 
   diag_status_rsp_type 
-----------------------------------------------------------*/

/* Taken from MCC_ENTER_OFFLINE in mcc_entry_name_type.  
   Please refer to mcc.h */
#define DIAG_OFFLINE_DIGITAL 2  

/* Taken from APC_ENTER_OFFLINE in apc_entry_name_type. 
   Please refer to acpmc.h*/
#define DIAG_OFFLINE_ANALOG  6   

typedef PACKED struct {
  byte    cmd_code;               /* Command code                         */
  byte    demod;                  /* Demod id byte                        */
  byte    decode;                 /* Decoder id byte                      */
  byte    inter;                  /* Interleaver id byte                  */
  dword   esn;                    /* Mobile Electronic Serial Number      */

  word    rf_mode;                /* 0->analog,  1->cdma,  2->pcn         */
  dword   min1[NV_MAX_MINS];      /* all MIN1s                            */
  word    min2[NV_MAX_MINS];      /* all MIN2s                            */
  byte    orig_min;               /* index (0-3) of the orig MIN          */
  
  word    cdma_rx_state;          /* current state for cdma only          */
  int1    cdma_good_frames;       /* whether or not frames we are
                                     receiving are good -- cdma only      */
  word    analog_corrected_frames;/* good frame count - analog only       */
  word    analog_bad_frames;      /* bad frame count - analog only        */
  word    analog_word_syncs;      /*  -- analog only                      */

  word    entry_reason;           /* entry reason                         */
  word    curr_chan;              /* center frequency channel             */
  byte    cdma_code_chan;         /* code for current channel - cdma only */
  word    pilot_base;             /* pilot pn of current cell - cdma only */

  word    sid;                    /* Current System ID                    */
  word    nid;                    /* Current Network ID                   */
  word    locaid;                 /* Current Location ID                  */
  word    rssi;                   /* Current RSSI level                   */
  byte    power;                  /* Current mobile output power level    */
} diag_status_rsp_type;


/*===========================================================================

PACKET   diag_logmask_rsp_type

ID       DIAG_LOGMASK_F

PURPOSE  Sent by DMSS, acknowledges that logmask was set.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
} diag_logmask_rsp_type;


/*===========================================================================

PACKET   diag_log_rsp_type

ID       DIAG_LOG_F

PURPOSE  Sent by DMSS, contains log data saved according to the log mask
         Note that this is a variable size packet.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  byte more;                   /* Indicates how many log entries, not
                                   including the one returned with this
                                   packet, are queued up in the Mobile
                                   Station */
  word len;                    /* Indicates the length, in bytes, of the
                                   following log entry */
  log_type log;                /* Contains the log entry data. */
} diag_log_rsp_type;


/*===========================================================================

PACKET   diag_err_type

IDs      DIAG_BAD_CMD_F
         DIAG_BAD_PARM_F
         DIAG_BAD_LEN_F
         DIAG_BAD_VOC_F
         DIAG_BAD_MODE_F
         DIAG_BAD_SPC_MODE_F

PURPOSE  Sent by DMSS when it detects an erroneous packet from DM. Errors
         include command code out of bounds, bad length...  Includes the
         first DIAG_MAX_ERR bytes of the offending input packet.
         Also includes when an nv_read/write is attempted before the correct
         SPC has been entered.

============================================================================*/
#define DIAGPKT_MAX_ERR 16 
  /* maximum number of bytes (starting with the first byte of the packet )
     from the received packet which will be echoed back to the 
     Diagnostic Monitor if an error is detected in the packet */

/* Type to communicate an error in a received packet */
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  byte pkt[DIAGPKT_MAX_ERR];   /* first 16 bytes of received packet */
} diag_err_type;


/*===========================================================================

PACKET   diag_nv_peek_rsp_type

ID       DIAG_NV_PEEK_F

PURPOSE  Sent by DMSS, contains NV memory data requested in a
         diag_nv_peek_rsp_type packet.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  nv_peek_type  peeknv;        /* The nv definition of a peek item */
} diag_nv_peek_rsp_type;
                       

/*===========================================================================

PACKET   diag_tagraph_rsp_type

ID       DIAG_TAGRAPH_F

PURPOSE  Sent by DMSS, contains Temporal Analyzer data for the small
         window.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  byte  rx_rssi;               /* Current value of rx agc register */
  byte  adj_rssi;              /* Current value of tx gain adjust register */
  byte  tx_voc;                /* Current tx vocoder data rate */
  byte  rx_voc;                /* Current rx vocoder data rate */
  dword bad_frames;            /* Number of bad frames */
  dword total_frames;          /* Number of all the frames */
} diag_tagraph_rsp_type;


/*===========================================================================

PACKET   diag_markov_rsp_type

ID       DIAG_MARKOV_F

PURPOSE  Sent by DMSS, contains Markov processing statistics.  Sent when
         requested with a diag_markov_req_type packet.

============================================================================*/
/* Definitions of values for the mode field - service option */
#define DIAG_SO_NONE            0x00
#define DIAG_SO_LOOPBACK        0x02
#define DIAG_SO_MARKOV          0x03
#define DIAG_SO_DATA            0x04
#define DIAG_SO_VOICE_96A       0x05
#define DIAG_SO_VOICE13         0x06 /* 13K (MSM2 only) Voice call */
#define DIAG_SO_RS2_MARKOV      0x07 /* 13K (MSM2 only) Markov call (new) */
#define DIAG_SO_RS1_MARKOV      0x08
#define DIAG_SO_LOOPBACK_13K    0x09
#define DIAG_SO_MARKOV_13K      0x0A /* 13K (MSM2 only) Markov call (old) */
#define DIAG_SO_EVRC            0x0B /* EVRC voice option */
#define DIAG_SO_VOICE_13K_IS733 0x11 /* IS-733 s.o. number */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
/* Medium Data Rate (externally known as High Speed Data) PPP Packet Data Service
 (IS-707A) with rate set 1 forward and reverse.  Default Mux = 9 forward 1 reverse */
#define DIAG_SO_MDR_PKT_DATA_FRS1_RRS1   0x16 /* IS-707A s.o. number = 22 */

/* Medium Data Rate (externally known as High Speed Data) PPP Packet Data Service
 (IS-707A) with rate set 1 forward and rate set 2 reverse.
 Default Mux = 9 forward 2 reverse.  This SO is not support by MSM3000. */
#define DIAG_SO_MDR_PKT_DATA_FRS1_RRS2   0x17 /* IS-707A s.o. number = 23 */

/* Medium Data Rate (externally known as High Speed Data) PPP Packet Data Service
 (IS-707A) with rate set 2 forward and rate set 1 reverse.
 Default Mux = 10 forward 1 reverse.  This SO is not support by MSM3000. */
#define DIAG_SO_MDR_PKT_DATA_FRS2_RRS1   0x18 /* IS-707A s.o. number = 24 */

/* Medium Data Rate (externally known as High Speed Data) PPP Packet Data Service
 (IS-707A) with rate set 2 forward and reverse.  Default Mux = 10 forward 2 reverse */
#define DIAG_SO_MDR_PKT_DATA_FRS2_RRS2   0x19 /* IS-707A s.o. number = 25 */

#define DIAG_SO_MDR_DPT_FRS1_RRS1        0x24 /* MDR Data Pattern Test: Proprietary
                                               Service Option rate set 1 fwd and rev */

#define DIAG_SO_MDR_DPT_FRS2_RRS2        0x25 /* MDR Data Pattern Test: Proprietary
                                               Service Option rate set 2 fwd and rev */
#endif /* defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) */


typedef PACKED struct
{
  byte cmd_code;               /* Command code                        */
  byte mode;                   /* rxc mode                            */
  word markov_rate;            /* rate for markov processing          */
  word rx_state;               /* receive task state                  */
  dword total_frames;          /* total number of Markov frames       */
  dword bad_frames;            /* total number of bad frames          */
  word data[ 4 ][ 10 ];        /* mar_test array items for expected
                                     full, half,  quarter and eighth
                                     rate packets                     */
  dword bit_errs;              /* Markov bit errors                   */
  word good_errs[4];           /* Rate decision good,  but bits bad   */
} diag_markov_rsp_type;


/*==========================================================================

PACKET   diag_markov_reset_rsp_type

ID       DIAG_MARKOV_RESET_F

PURPOSE  Sent by DMSS, contains acknowledgement of the reset of the
         Markov statistics in the mobile.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
} diag_markov_reset_rsp_type;


/*===========================================================================

PACKET   diag_diag_ver_rsp_type

ID       DIAG_DIAG_VER_F

PURPOSE  Sent by DMSS, contains the requested version in word format

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  word ver;                    /* diag version */
} diag_diag_ver_rsp_type;


/*===========================================================================

PACKET   diag_ts_rsp_type

ID       DIAG_TS_F

PURPOSE  Sent by DMSS, contains a mobile time-stamp in qword format

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  qword ts;                    /* Time stamp */
} diag_ts_rsp_type;


/*===========================================================================

PACKET   diag_ta_parm_rsp_type

ID       DIAG_TA_PARM_F

PURPOSE  Sent by DMSS, contains acknowledgement that ta parameters were
         set in the mobile.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
} diag_ta_parm_rsp_type;


/*===========================================================================

PACKET   diag_msg_rsp_type

ID       DIAG_MSG_F

PURPOSE  Sent by DMSS, contains 0 or 1 of the buffered messages the mobile
         is holding.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* command code                              */
  word qty;                    /* number of msg packets in the msg buffer
                                  including the msg packet returned in this
                                  response  -- if this field is 0, no msg
                                  packet is included in this response       */
  dword drop_cnt;              /* number of msgs dropped by error services  */
  dword total_msgs;            /* total number of msgs that have occured    */
  byte  level;                 /* Severity level / Priority of this message */
  char  file[ 13 ];            /* Holds source file name                    */
  word  line;                  /* Line number in source file                */
  char  fmt[ 40 ];             /* printf style format string                */
  dword code1;                 /* parameters to go with the format string   */
  dword code2;
  dword code3;
  qword time;                  /* Time at which message was generated       */
} diag_msg_rsp_type;


/*============================================================================

PACKET   diag_screen_rsp_type

ID       DIAG_HS_SCREEN_F

PURPOSE  Sent by DMSS, contains the screen and annunciator image from the
         handset

============================================================================*/
typedef PACKED struct
{
  byte       cmd_code;             /* Command code */
  boolean backlight_on;         /* Backlight state         */
#if MMI_TYPE != SKY_UI
  byte scrn[ HS_SCRN_HIGH * HS_SCRN_WIDE ];
#endif
  byte reserved1;
  word annun;                   /* Annunciators            */
#if MMI_TYPE != SKY_UI
  byte blink_scrn[ HS_SCRN_HIGH * HS_SCRN_WIDE ];
#endif
  word blink_annun;             /* Annunciators for blink  */
  byte rssi;                    /* RSSI indicator          */
  byte reserved2;
  word blink_rate;              /* Blink rate for blinking */
} diag_screen_rsp_type;


/*==========================================================================

PACKET    diag_parm_set_rsp_type

ID        DIAG_PARM_SET_F

PURPOSE   Sent by DMSS in response to a request to reset one (or all) of
          the settable parameters to a new value.

============================================================================*/
typedef PACKED struct {
  byte      cmd_code;           /* Command code */
  uint64    time;               /* Time stamp for operation */
} diag_parm_set_rsp_type;


/*==========================================================================

PACKET   diag_err_rsp_type

ID       DIAG_ERR_F

PURPOSE  Sent by DMSS to return the error records. All NV_ERR_LOG_SIZE 
         of them.

===========================================================================*/
typedef PACKED struct {
  byte              cmd_code;           /* DIAG_ERR_READ_F */
  word              log_cnt;            /* how many logged */
  word              ignore_cnt;         /* how many ignored */
  nv_err_log_type   err_logs[ NV_MAX_ERR_LOG ];  /* error records */
} diag_err_rsp_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Reset SER statistics response                                           */

typedef PACKED struct
{
  byte  cmd_code;
    /* Command code */
} diag_ser_reset_rsp_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Report SER statistics response                                          */

typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  dword frame_cnt;  /* Total frame count */
  dword ser1;   /* Sum of all 192 bit frame symbol error counts */
  dword ser2;   /* Sum of all 96 bit frame symbol error counts */
  dword ser4;   /* Sum of all 48 bit frame symbol error counts */
  dword ser8;   /* Sum of all 24 bit frame symbol error counts */
} diag_ser_report_rsp_type;


/*===========================================================================

PACKET   diag_test_rsp_type

ID       DIAG_TEST_F
 
PURPOSE  Sent by DMSS to relay test results

===========================================================================*/
typedef PACKED struct {
  byte                      cmd_code;
  word                      test_code; /* Use diag_test_enum_type */
  diag_test_results_type    results;
} diag_test_rsp_type;


/*===========================================================================

PACKET   diag_orig_rsp_type

ID       DIAG_ORIG_F
 
PURPOSE  Sent by DMSS to acknowledge a command from the Data Services DM
         to originate a call.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;    /* Command code */
} diag_orig_rsp_type;


/*===========================================================================

PACKET   diag_end_rsp_type

ID       DIAG_END_F
 
PURPOSE  Sent by DMSS to acknowledge a command from the Data Services DM
         to end a call.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;    /* Command code */
} diag_end_rsp_type;


/*===========================================================================

PACKET   diag_tmob_rsp_type

ID       DIAG_TMOB_F
 
PURPOSE  Sent by Tester DMSS to acknowledge arrival of TMOB command.

NOTE     Only sent by Tester DMSS, only received by Tester DM.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;       /* Command code */
} diag_tmob_rsp_type;


#ifdef FEATURE_FACTORY_TESTMODE
/*===========================================================================

PACKET   diag_ftm_rsp_type

ID       DIAG_FTM_CMD_F

PURPOSE  Sent by DM to request Factory Test Mode commands.

RESPONSE DMSS sends back status of the requested command.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;
  word sub_cmd;
  byte status;
} diag_ftm_rsp_type;
#endif /* FEATURE_FACTORY_TESTMODE */


/*===========================================================================

PACKET   diag_state_rsp_type

ID       DIAG_STATE_F

PURPOSE  Sent by external device to obtain the current state of operation
         the phone is in.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  byte phone_state;           /* Current phone state */
  word event_count;           /* Count of possible state-changing events */
} diag_state_rsp_type;


/*===========================================================================

PACKET   diag_pilot_sets_rsp_type

ID       DIAG_PILOT_SETS_F

PURPOSE  Sent by external device to obtain the current contents of the 
         various pilot sets.

===========================================================================*/
typedef PACKED struct {
    word pilot_pn;            /* Pilot offset   */
    word pilot_strength;      /* Pilot strength */
} diag_pilot_data_type; 

typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  word pilot_inc;             /* Pilot increment for remaining sets */
  byte active_cnt;            /* Count of pilots in active set (up to 6) */
  byte cand_cnt;              /* Count of pilots in candidate set (up to 6) */
  byte neigh_cnt;             /* Count of pilots in neighbor set (up to 40) */
  diag_pilot_data_type pilot_sets[52];
                              /* Sets of pilots (in the above order) (6+6+40) */
} diag_pilot_sets_rsp_type;


/*===========================================================================

PACKET   diag_spc_rsp_type

ID       DIAG_SPC_F

PURPOSE  Sent by external device to enter the Service Programming Code,
         to then allow Service Programming.  This response indicates
         whether the correct SPC was given or not.

NOTE     If the incorrect SPC is entered, DIAG will time out for 10 seconds.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  boolean sec_code_ok;        /* The SPC was entered correctly or not */
} diag_spc_rsp_type;


/*===========================================================================

PACKET    diag_parm_get2_rsp_type

ID        DIAG_PARM_GET2_F

PURPOSE   Sent by DMSS, contains the complete set of retreivable parameters.

==========================================================================*/
typedef PACKED struct {
  byte      cmd_code;                  /* Command code */
  uint64    time;                      /* Time stamp */
  dword     mux_rev[ 14 ];             /* Reverse Traffic Channel params */
  dword     mux_for[ 14 ];             /* Forward Traffic Channel params */
  dword     pag[ 7 ];                  /* Paging Channel params */
  dword     acc[ 8 ];                  /* Access Channel params */
  dword     layer2[ 5 ];               /* Layer2 params */
  uint64    sys_time;                  /* OTH_SYS_TIME parameter */
  dword     mux2_rev[ 26 ];            /* MUX2 Reverse Traffic Channel params */
  dword     mux2_for[ 26 ];            /* MUX2 Forward Traffic Channel params */
} diag_parm_get2_rsp_type;


/*===========================================================================

PACKET   diag_ser_mode_chng_rsp_type

ID       DIAG_SERIAL_CHG_F

PURPOSE  DMSS commands the data services to switch to AT command processor 
         mode, and sends a diag_ser_mode_chng_rsp_type packet with whether
         the command was successful or not.

============================================================================*/
typedef PACKED struct {
  byte                cmd_code;               /* Command code */
  byte                mode_changed;           /* Boolean whether mode changed */
} diag_ser_mode_chng_rsp_type;


/*===========================================================================

PACKET   diag_password_rsp_type

ID       DIAG_PASSWORD_F

PURPOSE  Sent by external device to enter the Security Password,
         to then allow operations protected by security.  This response 
         indicates whether the correct Password was given or not.

NOTE     If the incorrect password is entered, DIAG will POWER DOWN
         the phone.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  boolean password_ok;        /* TRUE if Security Password entered correctly */
} diag_password_rsp_type;


/*===========================================================================

PACKET   diag_pr_list_wr_rsp_type

ID       DIAG_PR_LIST_WR_F

PURPOSE  This response indicates whether the block of the PR_LIST was 
         received successfully or not

===========================================================================*/
#define DIAG_RL_WR_OK          0    /* No errors */
#define DIAG_RL_WR_OK_DONE     1    /* No errors, PR_LIST is complete */
#define DIAG_RL_WR_OUT_OF_SEQ  2    /* Sequence number out of order */
#define DIAG_RL_WR_OVERFLOW    3    /* PR_LIST overflow */

typedef PACKED struct
{
  byte cmd_code;                    /* Command code */
  byte rl_status;                   /* Status of block, as above */
  nv_stat_enum_type nv_stat;        /* NV write status if OK_DONE */
} diag_pr_list_wr_rsp_type;


/*===========================================================================

PACKET   diag_pr_list_rd_rsp_type

ID       DIAG_PR_LIST_RD_F

PURPOSE  This response reports status, repeats sequence number, returns the
         read data block, and tells whether there are more to come.

===========================================================================*/
#define DIAG_RL_RD_OK         0     /* No error, using non-standard PRL */
#define DIAG_RL_RD_NV_ERR     1     /* NV error */
#define DIAG_RL_RD_683A_OK    2     /* No error, using IS683A PRL */ 
#define DIAG_RL_RD_NV_683A_ERR   3  /* NV error, using IS683A PRL */ 

typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  byte rl_status;                              /* Status of block, as above */
  nv_stat_enum_type nv_stat;                   /* Status returned by NV */
  byte seq_num;                                /* Sequence number */
  byte more;                                   /* More to come? */
  word num_bits;                               /* Number of valid data bits */
  byte pr_list_data[DIAG_PR_LIST_BLOCK_SIZE];  /* The block of PR_LIST */
} diag_pr_list_rd_rsp_type;


/*===========================================================================

PACKET   diag_feature_query_rsp_type

ID       DIAG_FEATURE_QUERY_F

PURPOSE  This response contains the bit mask of the features that are 
         turned on in the build.  At this time only the features defined
         in feature_query_enum_type are supported. Use IS_FEATURE_BIT_SET
         on the bit mask to determine whether bits are set.
         
         Note: This is a variable length packet. The largest possible
         feature mask is 938 bytes as defined by FEATURE_MASK_MAX_LENGTH.
         FEATURE_MASK_LENGTH is the length of the mask for this build.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                              /* Command code */
  word feature_mask_size;                     /* Size of the following Mask */
  byte feature_mask[FEATURE_MASK_LENGTH]; /* Space for the largest possible 
                                                               feature mask */
} diag_feature_query_rsp_type;


/*===========================================================================

PACKET   diag_sms_read_rsp_type

ID       DIAG_SMS_READ_F

PURPOSE  This response delivers the read SMS data. It is a variable length
         response message, only as long as dictated by the length field in
         sms_message.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                           /* Command code */
  nv_stat_enum_type nv_stat;               /* Status of operation */
  nv_sms_type sms_message;   /* SMS msg read from the phone - variable size */
} diag_sms_read_rsp_type;


/*===========================================================================

PACKET   diag_sms_write_rsp_type

ID       DIAG_SMS_WRITE_F

PURPOSE  This response tells the NV status of the SMS write.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                             /* Command code */
  nv_stat_enum_type nv_stat;                 /* Status of operation */
} diag_sms_write_rsp_type;


#if (defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR))
/*===========================================================================

PACKET   diag_sup_walsh_codes_rsp_type

ID       DIAG_SUP_WALSH_CODES_F

PURPOSE  Sent by DMSS, contains 
         
============================================================================*/
#define   DIAG_SRCH_SUP_CHANNEL_MAX 7
#define   DIAG_SRCH_ASET_MAX 6

typedef PACKED struct
{
  byte cmd_code;                                /* Command code */

  word num_aset;        
    /* number of active set */
  
  PACKED  struct 
  {
    int2 pilot_off;     
      /* Pilot offset used by serach */
    
    byte num_sup_channels;
      /* Number of supplemental channels */

    word walsh_code[ DIAG_SRCH_SUP_CHANNEL_MAX + 1 ];    
      /* Specifies which walsh code is being used, including fundamental */

  }ch_data [ DIAG_SRCH_ASET_MAX ]; 
    /* One for each Aset */

} diag_sup_walsh_codes_rsp_type;      


/*===========================================================================

PACKET   diag_sup_fer_rsp_type

ID       DIAG_SUP_FER_F

PURPOSE  Sent by DMSS, contains Frame Error Rate data for the small
         window.  This is called if supplemental channels (MDR) is supported.
         This packet gives a snapshot of the FER info for all 8 channels at 
         once.
         
============================================================================*/
typedef PACKED struct
{
  byte cmd_code;                                /* Command code */

  dword bad_frames[1 + DIAG_SUP_CHANNEL_MAX];   /* Number of bad frames for:
                                                   Fundamental + 
                                                   all sup channels. */
  dword total_frames[1 + DIAG_SUP_CHANNEL_MAX]; /* Total frames for:
                                                   Fundamental + 
                                                   all sup channels. */

} diag_sup_fer_rsp_type;      


/*===========================================================================

PACKET   diag_set_max_sup_ch_req_type

ID       DIAG_SET_MAX_SUP_CH_F

PURPOSE  This response acknowledges a valid setting.

============================================================================*/
typedef PACKED struct
{
  byte  cmd_code;                           /* Command code */
} diag_set_max_sup_ch_rsp_type;      


/*===========================================================================

PACKET    diag_parm_get_is95b_rsp_type

ID        DIAG_PARM_GET_IS95B_F

PURPOSE   Sent by DMSS, contains the complete set of retreivable parameters.

============================================================================*/
typedef PACKED struct {
   dword secondary;       /* 9600 bps or 14400 bps frame, blank-and-burst 
                             with secondary traffic only. */
   dword primary;         /* 9600 bps or 14400 bps frame,
                             primary traffic only. */
} diag_parm_sup_ch_type;

typedef PACKED struct {
  byte      cmd_code;                  /* Command code */
  qword     time;                      /* Time stamp */
  dword     mux_rev[ 14 ];             /* Reverse Traffic Channel params */
  dword     mux_for[ 14 ];             /* Forward Traffic Channel params */
  dword     pag[ 7 ];                  /* Paging Channel params */
  dword     acc[ 8 ];                  /* Access Channel params */
  dword     layer2[ 5 ];               /* Layer2 params */
  uint64    sys_time;                  /* OTH_SYS_TIME parameter */
  dword     mux2_rev[ 26 ];            /* MUX2 Reverse Traffic Channel parms */
  dword     mux2_for[ 26 ];            /* MUX2 Forward Traffic Channel parms */
  diag_parm_sup_ch_type sup_rev[DIAG_SUP_CHANNEL_MAX]; /* Reverse supplemental
                                                          channels */ 
  diag_parm_sup_ch_type sup_for[DIAG_SUP_CHANNEL_MAX]; /* Forward supplemental
                                                          channels */

} diag_parm_get_is95b_rsp_type;
#endif /* (defined(FEATURE_IS95B_MDR) || (FEATURE_SPECIAL_MDR)) */


#ifdef FEATURE_EFS
/*===========================================================================

PACKET   diag_fs_op_rsp_type

ID       DIAG_FS_OP_F

PURPOSE  This response contains status and results data from an EFS 
         operation.  The contents of the response depends on the file
         system operation (sub-command) requested.

===========================================================================*/
/*------------------------------------------------
  Operation status values can be found in fs.h
------------------------------------------------*/

/*------------------------------------------------
  Definitions of data for specific operations.
------------------------------------------------*/
/* "Display Directory List" operation */
typedef PACKED struct
{
  word num_dirs;
  diag_fs_data_block_type  dir_list;
} diag_fs_disp_dirs_rsp_type;

/* "Display File List" operation */
typedef PACKED struct
{
  word num_files;
  diag_fs_data_block_type  file_list;
} diag_fs_disp_files_rsp_type;

/* "Read File" operation */
typedef PACKED union
{ 
  PACKED struct
  {
    dword                    total_length;   /* Total length of this file */
    diag_fs_data_block_type  block;
  } begin;

  diag_fs_data_block_type  block;
 
} diag_fs_read_block_type;

typedef PACKED struct
{
  byte                  seq_num;  /* Sequence number for mult blocks    */
  byte                     more;  /* Flag if more packets are needed to
                                      complete transfer                 */
  diag_fs_read_block_type  data;  /* Current portion of the file        */
} diag_fs_read_rsp_type;

/* "Write File" operation */
typedef PACKED struct
{
  byte seq_num;
} diag_fs_write_rsp_type;

/* "Get File Attributes" operation */
typedef PACKED struct
{
  diag_fs_attributes_type attrib; /* Attributes         */
  dword            creation_date; /* File creation date */
  dword                     size; /* File size          */
} diag_fs_get_attrib_rsp_type;

/* "Iterate Directories" and "Iterate Files" operation */
typedef PACKED struct
{
  dword seq_num;
  
  diag_fs_attributes_type   attrib; /* Attributes                   */
  dword              creation_date; /* File creation date           */
  dword               logical_size; /* Size of data                 */
  dword              physical_size; /* Physical space on device     */
  byte              dirname_length; /* Length of directory portion  */
                                    /* of file name.                */
  diag_fs_filename_type  item_name;
} diag_fs_iter_rsp_type;

/* "Space Available" operation */
typedef dword diag_fs_space_avail_rsp_type;

typedef PACKED union
{
  byte                             seq_num; /* Seq number in same place for 
                                               all packets that use them */
  diag_fs_disp_dirs_rsp_type     disp_dirs;
  diag_fs_disp_files_rsp_type   disp_files;
  diag_fs_read_rsp_type               read;
  diag_fs_write_rsp_type             write;
  diag_fs_get_attrib_rsp_type   get_attrib;
  diag_fs_iter_rsp_type               iter;
  diag_fs_space_avail_rsp_type space_avail;
} diag_fs_rsp_type;

typedef PACKED struct
{
  byte cmd_code;                     /* Command code                     */
  byte file_op;                      /* Operation requested              */
  byte fs_status;                    /* Status of operation - values in  
                                        fs_status_enum_type              */
  diag_fs_rsp_type  fs_rsp;          /* Only used if returning more than 
                                        just status                      */
} diag_fs_op_rsp_type;  
#endif /* FEATURE_EFS */

/*===========================================================================

PACKET   diag_akey_verify_rsp_type

ID       DIAG_AKEY_VERIFY_F

PURPOSE  Sent by external device to verify the AKEY.  This response indicates
         whether the correct AKEY.

NOTE     If the incorrect AKEY is entered, DIAG will time out for 10 seconds.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  boolean akey_ok;            /* The AKEY was entered correctly or not */
} diag_akey_verify_rsp_type;



#if defined(FEATURE_BITMAP_RASTER)
#error code not present
#endif



/*===========================================================================

PACKET   diag_config_comm_rsp_type

ID       DIAG_CONFIG_COMM_F 

PURPOSE  Sent by DM to request a baud rate changefor DIAG serial 
         communications.

RESPONSE DMSS returns diag_config_comm_rsp_type packet that contains 
         possible configurations or an acknowledgement to a change request.
         Upon sending a response 

===========================================================================*/
typedef PACKED struct
{
 byte                cmd_code;  /* Command code                   */
 byte                 sub_cmd;  /* Use diag_config_comm_enum_type */

 PACKED union
 {
   boolean            success;  /* TRUE if baud rate change ACK   */
   byte           num_options;  /* Number of options following    */
 } cmd_specific;

 /* The following only exist if responding to an info request.    */

 dword            baudrate[4];  /* Baud rates available           */
                                /* Only used if num_options is    */
} diag_config_comm_rsp_type;
 


/*===========================================================================

PACKET   diag_extended_logmask_rsp_type

ID       DIAG_EXT_LOGMASK_F

PURPOSE  Sent by the DM to set the logging mask in the DMSS.  This is
         necessary for logmasks > 32 bits.

RESPONSE The DMSS establishes the logging mask and replies with a
         diag_ext_logmask_rsp_type packet

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;

  word  num_valid_bits;                     /* Number of valid bits    */
  byte  valid_mask[DIAG_LOG_MASK_NUMBYTES]; /* mask of valid log codes */
} diag_ext_logmask_rsp_type;


#if !defined(FEATURE_DIAG_NO_EVENTS)
/*===========================================================================

PACKET   diag_event_report_rsp_type

ID       DIAG_EVENT_REPORT_F

PURPOSE  Sent by the DM to configure static event reporting in the DMSS.

RESPONSE If responding to a event reporting config request, the DMSS will 
         return an acknowledgement (cmd_code) only.  If this is an event 
         report, the entire structure is used.

===========================================================================*/
typedef PACKED struct
{
  byte       cmd_code;  /* Command code               */
  word       length;    /* Number of bytes to follow  */
  event_type events[1]; /* Up to EVENT_WATERMARK_SIZE */
                   /* Buffer to store event data.  Use structures in 
                      event.h for specific structure definitions.    */
} diag_event_report_rsp_type;
#endif


/*===========================================================================

PACKET   diag_streaming_config_rsp_type

ID       DIAG_STREAMING_CONFIG_F

PURPOSE  Sent by the DM to configure and tweak streaming DIAG output services.

RESPONSE The DMSS will return diag_streaming_config_rsp_type.

===========================================================================*/
typedef PACKED struct
{
  uint8  cmd_code;    /* Command code  */

  uint8  subcommand;  /* Sub-command - diag_streaming_config_subcommand_enum_type */

  PACKED union {
    /* DIAG_READ_NICE_C */

    PACKED struct {
      uint8 num_entries; /* Number of entries in the following array */

      PACKED struct {
        uint16 code; /* MSG_LVL value or Log code */

        int16  val;
      
      } entry[LOGMASK_NUM_BITS + 5 /* 5 Message Levels */];

    } read;

    /* No data for DIAG_WRITE_NICE_C */

  } command_info;

} diag_streaming_config_rsp_type;



/*===========================================================================

PACKET   diag_parm_retrieve_rsp_type

ID       DIAG_PARM_RETRIEVE_F

PURPOSE  Sent by DM to set or retriecce a all or a subset set of retreivable
         parameters.  Parameters referred to here are the CAI's settable and
         retreival parameters listed in section 6.4.5 and Appendix E.

RESPONSE DMSS places the current value of all parameters in the range 
         specified.
         A range of 0--0 will return the full list of parameters.
         If an invalid range is speciied, DIAG_BAD_PARM_F is returned.

============================================================================*/
typedef PACKED struct {
  uint8  cmd_code;      /* Command code */

  uint8  subcmd;        /* Use diag_parm_subcmd_enum_type */

  uint16 start_parm_id; /* refer to CAI */
  uint16 end_parm_id;   /* refer to CAI */

  uint64 time;          /* Time stamp of parm retrieval */

  /* PARM value */
#if defined(FEATURE_IS2000) || \
    defined(FEATURE_IS95B_MDR) || defined(FEATURE_IS95B_SPECIAL_MDR)
  uint32 parm[MAX(CAI_LAST_PARM_ID, CAI_XTRA_LAST_PARM_ID)];
#else
  uint32 parm[CAI_LAST_PARM_ID];
#endif

} diag_parm_retrieve_rsp_type;


#if defined(FEATURE_DIAG_RPC)
#error code not present
#endif /* FEATURE_DIAG_RPC */


#if defined( FEATURE_GPSONE )
#error code not present
#endif /* defined( FEATURE_GPSONE ) */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Union of Responses from Mobile to Diagnostic Monitor */

typedef PACKED union
{
  byte                      cmd_code;
  diag_verno_rsp_type       verno;
  diag_verstr_rsp_type      verstr;
  diag_esn_rsp_type         esn;
  diag_peekb_rsp_type       peekb;
  diag_peekw_rsp_type       peekw;
  diag_peekd_rsp_type       peekd;
  diag_pokeb_req_type       pokeb;     /* note: type is a request pkt */
  diag_pokew_req_type       pokew;     /* note: type is a request pkt */
  diag_poked_req_type       poked;     /* note: type is a request pkt */
  diag_outp_req_type        outp;      /* note: type is a request pkt */
  diag_outpw_req_type       outpw;     /* note: type is a request pkt */
  diag_inp_rsp_type         inp;
  diag_inpw_rsp_type        inpw;
  diag_status_rsp_type      stat;
  diag_logmask_rsp_type     logmask;
  diag_log_rsp_type         log;
  diag_err_type             err;
  diag_nv_peek_rsp_type     nv_peek;
  diag_nv_poke_req_type     nv_poke;     /* note: type is a req packet */
  diag_tagraph_rsp_type     tagraph;
  diag_markov_rsp_type      markov;
  diag_markov_reset_rsp_type    m_reset;
  diag_diag_ver_rsp_type    diag_ver;
  diag_ts_rsp_type          ts;
  diag_ta_parm_rsp_type     ta_parm;
  diag_msg_rsp_type         msg_rsp;
  diag_hs_lock_req_type     hs_lock;     /* note: type is a req packet */
  diag_hs_key_req_type      hs_key;      /* note: type is a req packet */
  diag_screen_rsp_type      hs_screen;
  diag_parm_set_rsp_type    parm_set;
  diag_nv_read_req_type     nv_read;     /* note: type is a req packet */
  diag_nv_write_req_type    nv_write;    /* note: type is a req packet */
  diag_err_rsp_type         err_rsp;
  diag_err_clr_req_type     err_clr;     /* note: type is a req packet */
  diag_control_req_type     cont;        /* note: type is a req packet */
  diag_ser_reset_rsp_type   ser_res;
  diag_ser_report_rsp_type  ser_rep;
  diag_test_rsp_type        test_rsp;
  diag_dipsw_req_type       dipsw_rsp;   /* note: type is a req packet */
  diag_voc_pcm_lb_type      vocpcm_rsp;  /* note: type is same as req packet */
  diag_voc_pkt_lb_type      vocpkt_rsp;  /* note: type is same as req packet */
  diag_orig_rsp_type        orig;
  diag_end_rsp_type         end;
  diag_dload_req_type       dload;      /* note: type is same as req packet */
  diag_tmob_rsp_type        tmob;
  diag_state_rsp_type       state_rsp;
  diag_pilot_sets_rsp_type  pilot_sets;
  diag_spc_rsp_type         spc_rsp;
  diag_parm_get2_rsp_type   parm_get2;
  diag_ser_mode_chng_rsp_type     ser_mode_chng;
  diag_password_rsp_type           password_rsp;
  diag_pr_list_wr_rsp_type       pr_list_wr_rsp;
  diag_pr_list_rd_rsp_type       pr_list_rd_rsp;
  diag_feature_query_rsp_type feature_query_rsp;
  diag_sms_read_rsp_type               sms_read;
  diag_sms_write_rsp_type             sms_write;
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  diag_sup_fer_rsp_type                 sup_fer;
  diag_sup_walsh_codes_rsp_type     walsh_codes;  
  diag_set_max_sup_ch_rsp_type   set_max_sup_ch;
  diag_parm_get_is95b_rsp_type   parm_get_is95b;
#endif /* defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) */
#ifdef FEATURE_EFS
  diag_fs_op_rsp_type                     fs_op;
#endif /* FEATURE_EFS */
  diag_akey_verify_rsp_type         akey_verify;
#if defined(FEATURE_BITMAP_RASTER)
#error code not present
#endif
  diag_config_comm_rsp_type         config_comm;
  diag_ext_logmask_rsp_type         ext_logmask;
#ifdef FEATURE_SKT_DEBUG
  diag_sktest_skfing_rsp_type		  skfing;
  diag_sktest_skcmd_rsp_type          skcmd;
#ifdef	FEATURE_SKT_RLPDUMP_DS	//	00/04/15
// added by heroine 00.04.13 for RLP frame dump
  diag_sktest_skrlpdump_rsp_type      skrlpdump;
  diag_sktest_skrlpparam_rsp_type		skrlpparam;
  diag_rlp_dump_rsp_type			rlp_dump;

#endif	//	#ifdef	FEATURE_SKT_RLPDUMP_DS
#endif	//#ifdef FEATURE_SKT_DEBUG
#if !defined(FEATURE_DIAG_NO_EVENTS)
  diag_event_report_rsp_type       event_report;
#endif
  diag_streaming_config_rsp_type  stream_config;
  diag_parm_retrieve_rsp_type     parm_retrieve;
#if defined(FEATURE_DIAG_RPC) 
#error code not present
#endif

#ifdef FEATURE_DIAG_QCT_EXT
  diag_get_property_rsp_type        get_property;
  diag_put_property_rsp_type        put_property;
  diag_get_guid_rsp_type            get_guid;
  diag_user_cmd_rsp_type            user_cmd;
  diag_get_perm_property_rsp_type   get_perm_property;
  diag_put_perm_property_rsp_type   put_perm_property;
  diag_perm_user_cmd_rsp_type       perm_user_cmd;
#endif
#ifdef FEATURE_FACTORY_TESTMODE
  diag_ftm_rsp_type                 ftm;
#endif
#ifdef MODEL_SKTT	// 00.08.23 kmh insert for chksum
	diag_chksum_rsp_type       chksum;
#endif	// #ifdef MODEL_SKTT

#if defined(FEATURE_SKTT_ASTOOL) && ((MODEL_ID == MODEL_D1) || (MODEL_ID == MODEL_A10))// 00.09.30 chojw
	diag_phonebook_num_rsp_type			phonebook_num;
	diag_phonebook_read_rsp_type		phonebook_read;
	diag_phonebook_write_rsp_type		phonebook_write;
	diag_sdial_num_rsp_type				sdial_num;
	diag_group_num_rsp_type				group_num;
	diag_invalid_rsp_type	 		    invalid;
#endif // defined(FEATURE_SKTT_ASTOOL) && (MODEL_ID == MODEL_D1)

} diag_rsp_type;


/* These are the maximum sizes of DM packets.  This takes into account
   the necessary padding for escaping purposes.  Need to account for
   escaping.
*/

#define DIAG_MAX_RX_PKT_SIZ (sizeof(diag_req_type) * 2)
                                     /* sizeof(diag_req_type) == 265.
                                        Therefore receive packet can be
                                        twice that size given worst case
                                        escaping.                          */
#define DIAG_MAX_TX_PKT_SIZ 1882     /* sizeof(diag_rsp_type) == 941.
                                        Therefore trnamit packet can be
                                        twice that size given worst case
                                        escaping.                          */

#endif /* DIAGPKT_H  */

