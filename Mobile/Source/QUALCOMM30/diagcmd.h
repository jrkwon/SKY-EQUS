#ifndef DIAGCMD_H
#define DIAGCMD_H
/*==========================================================================

  D I A G N O S T I C    T A S K    P A C K E T    I D   D E F I N E S

DESCRIPTION
  This file contains packet id definitions for the serial interface to
  the dmss.  

Copyright (c) 1993-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/diagcmd.h_v   1.7   14 Feb 2001 11:21:54   jlindqui  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/13/00   jal     Support to reset RLP and TDSO statistics
09/06/00   bgc     Added support for FEATURE_FACTORY_TESTMODE with
                   DIAG_FTM_CMD_F (set to 59, which is also DIAG_TMOB_F).
08/31/00   lad     Added command code for tunneling capability.
06/23/00   lad     Removed obsolete command codes and marked them "reserved".
06/19/00   lad     Added DIAG_PARM_RETRIEVE_F
05/31/00   jal     Added GPS statistics, session control, and grid support.
05/15/00   lad     Added streaming config support (nice).
02/28/00   lad     Added codes for event reporting service.
02/02/00   lad     Added commands used with FEATURE_DIAG_QCT_EXT.
09/17/99   lcc     Merged in RPC support from PLT archive.
08/17/99   tac     Merged in EFS changes from branch.
07/19/99    sk     Replacing reset_sup_fer with walsh code.
07/19/99    sk     Added walsh code display command.
03/30/99   lad     Added support for FEATURE_IS95B_MDR and FEATURE_SPECIAL_MDR.
11/04/98   lad     Added 1998/1999 copyright information.
10/29/98   jmk     Merged Module command changes into the mainline.
                   (Replaced MOD_GET_STATUS with MOD_EXTENDED_PKT cmd code 75)
10/27/98   jmk     Added cmd IDs for CSS command, and SMS message read/write.
09/11/98   grl     Added feature query command
10/06/97   jjn     Added new commands for the Module Phase 1 interface.  These
                   include Module Status Mask, AKEY and audio control packets.
04/23/97   jjn     Added new packet pair to allow System Unit to access 
                   service option and caller ID information
03/25/97   jjn     Added new command (and packets) that allow writing to NV 
                   without going offline (for the Module only)
02/28/97   jjn     Enabled RSSI packets for the Module, added a packet for
                   module status and sound reporting, and added a pcket for
                   retrieving SMS messages
06/25/96   jmk     Added cmd id for preferred roaming list read.
06/24/96   jmk     Added cmd id for preferred roaming list write.
04/09/96   jmk     Added cmd ids for sending security code, and return code 
                   if phone is not unlocked for operations that require it.
03/06/96   jmk     Added command id for serial mode change (to AT cmd mode)
                   and command id for get rssi (for antenna aiming/WLL only)
08/16/95   jmk     Added command id for parm_get2 (includes MUX2 parms)
08/10/95   jmk     Added command id for Phone State, Pilot Sets and SPC reqs
01/28/95   ptw     Added command id to obtain System Time from the mobile.
12/07/94   jmk     Added command id for portable sleep on/off request.
11/07/94   jmk     Added command to request that seq_nums be used in pkts.
09/26/94   jmk     Put DIAG_ORIG_F and DIAG_END_F back in.
07/23/93   twp     Added DIAG_TMOB_F
01/14/93   twp     First release

===========================================================================*/

/*--------------------------------------------------------------------------

  Command Codes between the Diagnostic Monitor and the mobile. Packets
  travelling in each direction are defined here, while the packet templates
  for requests and responses are distinct.  Note that the same packet id
  value can be used for both a request and a response.  These values
  are used to index a dispatch table in diag.c, so 

  DON'T CHANGE THE NUMBERS ( REPLACE UNUSED IDS WITH FILLERS ). NEW IDs
  MUST BE ASSIGNED AT THE END.
  
----------------------------------------------------------------------------*/

typedef enum
{
  DIAG_VERNO_F  = 0,  /* Version Number Request/Response     */
  DIAG_ESN_F    = 1,  /* Mobile Station ESN Request/Response */
  DIAG_PEEKB_F  = 2,  /* Peek byte Request/Response          */
  DIAG_PEEKW_F  = 3,  /* Peek word Request/Response          */
  DIAG_PEEKD_F  = 4,  /* Peek dword Request/Response         */
  DIAG_POKEB_F  = 5,  /* Poke byte Request/Response          */
  DIAG_POKEW_F  = 6,  /* Poke word Request/Response          */
  DIAG_POKED_F  = 7,  /* Poke dword Request/Response         */
  DIAG_OUTP_F   = 8,  /* Byte output Request/Response        */
  DIAG_OUTPW_F  = 9,  /* Word output Request/Response        */
  DIAG_INP_F    = 10, /* Byte input Request/Response         */
  DIAG_INPW_F   = 11, /* Word input Request/Response         */
  DIAG_STATUS_F = 12, /* DMSS status Request/Response        */
/* 13-14 Reserved */
  DIAG_LOGMASK_F  = 15, /* Set logging mask Request/Response  */
  DIAG_LOG_F      = 16, /* Log packet Request/Response        */
  DIAG_NV_PEEK_F  = 17, /* Peek at NV memory Request/Response */
  DIAG_NV_POKE_F  = 18, /* Poke at NV memory Request/Response */
  DIAG_BAD_CMD_F  = 19, /* Invalid Command Response           */
  DIAG_BAD_PARM_F = 20, /* Invalid parmaeter Response         */
  DIAG_BAD_LEN_F  = 21, /* Invalid packet length Response     */
/* 22-23 Reserved */
  DIAG_BAD_MODE_F     = 24, /* Packet not allowed in this mode       */
                            /*    ( online vs offline )              */
  DIAG_TAGRAPH_F      = 25, /* info for TA power and voice graphs    */
  DIAG_MARKOV_F       = 26, /* Markov statistics                     */
  DIAG_MARKOV_RESET_F = 27, /* Reset of Markov statistics            */
  DIAG_DIAG_VER_F     = 28, /* Return diag version for comparison to */
                            /* detect incompatabilities              */
  DIAG_TS_F           = 29, /* Return a timestamp                    */
  DIAG_TA_PARM_F      = 30, /* Set TA parameters                     */
  DIAG_MSG_F          = 31, /* Request for msg report                */
  DIAG_HS_KEY_F       = 32, /* Handset Emulation -- keypress         */
  DIAG_HS_LOCK_F      = 33, /* Handset Emulation -- lock or unlock   */
  DIAG_HS_SCREEN_F    = 34, /* Handset Emulation -- display request  */
/* 35 Reserved */
  DIAG_PARM_SET_F     = 36, /* Parameter Download                    */
/* 37 Reserved */
  DIAG_NV_READ_F  = 38, /* Read NV item  */
  DIAG_NV_WRITE_F = 39, /* Write NV item */
/* 40 Reserved */
  DIAG_CONTROL_F    = 41, /* Mode change request                     */
  DIAG_ERR_READ_F   = 42, /* Error record retreival                  */
  DIAG_ERR_CLEAR_F  = 43, /* Error record clear                      */
  DIAG_SER_RESET_F  = 44, /* Symbol error rate counter reset         */
  DIAG_SER_REPORT_F = 45, /* Symbol error rate counter report        */
  DIAG_TEST_F       = 46, /* Run a specified test                    */
  DIAG_GET_DIPSW_F  = 47, /* Retreive the current dip switch setting */
  DIAG_SET_DIPSW_F  = 48, /* Write new dip switch setting            */
  DIAG_VOC_PCM_LB_F = 49, /* Start/Stop Vocoder PCM loopback         */
  DIAG_VOC_PKT_LB_F = 50, /* Start/Stop Vocoder PKT loopback         */
/* 51-52 Reserved */
  DIAG_ORIG_F = 53, /* Originate a call */
  DIAG_END_F  = 54, /* End a call       */
/* 55-57 Reserved */
// 00.08.23 kmh insert for Chksum
#ifdef MODEL_SKTT
  DIAG_CHKSUM_F = 57,
#endif

  DIAG_DLOAD_F = 58, /* Switch to downloader */
  DIAG_TMOB_F  = 59, /* Test Mode Commands   */   
  DIAG_FTM_CMD_F  = 59, /* Test Mode Commands and FTM commands*/
/* 60-62 Reserved */
  DIAG_STATE_F        = 63, /* Return the current state of the phone      */
  DIAG_PILOT_SETS_F   = 64, /* Return all current sets of pilots          */
  DIAG_SPC_F          = 65, /* Send the Service Prog. Code to allow SP    */
  DIAG_BAD_SPC_MODE_F = 66, /* Invalid nv_read/write because SP is locked */
  DIAG_PARM_GET2_F    = 67, /* get parms obsoletes PARM_GET               */
  DIAG_SERIAL_CHG_F   = 68, /* Serial mode change Request/Response        */
/* 69 Reserved */
  DIAG_PASSWORD_F     = 70, /* Send password to unlock secure operations  */
  DIAG_BAD_SEC_MODE_F = 71, /* An operation was attempted which required
                                 the phone to be in a security state that
                                 is wasn't - like unlocked.               */
  DIAG_PR_LIST_WR_F   = 72, /* Write Preferred Roaming list to the phone. */
  DIAG_PR_LIST_RD_F   = 73, /* Read Preferred Roaming list from the phone.*/
/* 74 Reserved */
  DIAG_SUBSYS_CMD_F   = 75, /* Dispatches a command to a subsystem        */
/* 76-80 Reserved */
  DIAG_FEATURE_QUERY_F   = 81, /* Asks the phone what it supports    */
/* 82 Reserved */
  DIAG_SMS_READ_F        = 83, /* Read SMS message out of NV         */
  DIAG_SMS_WRITE_F       = 84, /* Write SMS message into NV          */
  DIAG_SUP_FER_F         = 85, /* info for Frame Error Rate
                                  on multiple channels               */
  DIAG_SUP_WALSH_CODES_F = 86, /* Supplemental channel walsh codes   */
  DIAG_SET_MAX_SUP_CH_F  = 87, /* Sets the maximum # supplemental 
                                  channels                           */
  DIAG_PARM_GET_IS95B_F  = 88, /* get parms including SUPP and MUX2: 
                                  obsoletes PARM_GET and PARM_GET_2  */
  DIAG_FS_OP_F           = 89, /* Performs an Embedded File 
                                    System (EFS) operation.          */
  DIAG_AKEY_VERIFY_F     = 90, /* AKEY Verification.                 */
  DIAG_BMP_HS_SCREEN_F   = 91, /* Handset emulation - Bitmap screen  */
  DIAG_CONFIG_COMM_F     = 92, /* Configure communications           */
  DIAG_EXT_LOGMASK_F     = 93, /* Extended logmask for > 32 bits.    */
/* 94-95 reserved */
  DIAG_EVENT_REPORT_F    = 96, /* Static Event reporting.            */
  DIAG_STREAMING_CONFIG_F= 97, /* Load balancing, and more!          */
  DIAG_PARM_RETRIEVE_F   = 98, /* Parameter retrieval                */
  DIAG_STATUS_SNAPSHOT_F = 99, /* A state/status snapshot of the DMSS*/  
  DIAG_RPC_F               = 100, /* Used for RPC                 */
  DIAG_GET_PROPERTY_F      = 101, /* Get_property requests        */
  DIAG_PUT_PROPERTY_F      = 102, /* Put_property requests        */
  DIAG_GET_GUID_F          = 103, /* Get_guid requests            */
  DIAG_USER_CMD_F          = 104, /* Invocation of user callbacks */
  DIAG_GET_PERM_PROPERTY_F = 105, /* Get permanent properties     */
  DIAG_PUT_PERM_PROPERTY_F = 106, /* Put permanent properties     */
  DIAG_PERM_USER_CMD_F     = 107, /* Permanent user callbacks     */

  DIAG_GPS_SESS_CTRL_F     = 108, /* GPS Session Control          */
  DIAG_GPS_GRID_F          = 109, /* GPS search grid              */
  DIAG_GPS_STATISTICS_F    = 110, /* GPS Statistics               */
  DIAG_TUNNEL_F            = 111, /* DIAG Tunneling command code  */
  DIAG_IS2000_STATUS_F     = 112, /* IS2000 status                */
  DIAG_RLP_STAT_RESET_F    = 113, /* RLP statistics reset         */
  DIAG_TDSO_STAT_RESET_F   = 114, /* (S)TDSO statistics reset     */

//#ifdef FEATURE_SKTT_ASTOOL
  DIAG_PHONEBOOK_NUM_F	   = 115,
  DIAG_PHONEBOOK_READ_F    = 116, /* Get the phone book 1 item					*/
  DIAG_PHONEBOOK_WRITE_F   = 117, /* Put the phone book 1 item					*/
  DIAG_SDIAL_NUM_F		   = 118,
  DIAG_GROUP_NUM_F		   = 119,
  DIAG_INVALID_F		   = 120,
//#endif //FEATURE_SKTT_ASTOOL

  DIAG_MAX_F /* Number of packets defined. */
} diag_cmd_code_enum_type;

#endif  /* DIAGCMD_H */

