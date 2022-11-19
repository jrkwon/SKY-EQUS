#ifndef DSATCOP_H
#define DSATCOP_H
/*===========================================================================

                        D A T A   S E R V I C E S

                A T   C O M M A N D   P R O C E S S O R

                E X T E R N A L   H E A D E R   F I L E


DESCRIPTION
  This file contains the definitions of data structures, defined and
  enumerated constants and function prototypes required for the
  data services AT command processor, and the EIA 617 and telnet
  inband signaling.

Copyright (c) 1995,1996,1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999, 2000, 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/dsatcop.h_v   1.3   20 Feb 2001 16:18:56   dpendlet  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/01   dwp     Change nv_sio_baud_type to nv_sio_baudrate_type and all
                   associations based on these.
02/05/01   na      Removed references to FEATURE_DS_PCM.
12/15/00   ak      New var ds_atcop_qctrtl_val to turn off R-SCH throttle in
                   IS2000.
08/07/00   js/jjw  Merged in the improved inline documentations. 
07/24/00   ak      Support for AT$QCSCRM, which allows SCRM to be enabled/
                   disabled.
06/27/00   js/snn  Merged in changes to allow DNS service via three 
                   additional extended format proprietary commands: 
                   AT$QCDNSP, AT$QCDNSS and AT$QCDOM.
04/16/00   snn     Added a new parameter to the function ds_change_baud
                   change_baud_now.
04/10/00   snn     Added extern declaration for ds_change_baud. 
                   Added sio.h, because ds_change_baud uses typedef's defined
                   in that file.
02/02/00   ak      Merged rlp 3 branch against tip.
11/3/99    snn     Added DS_ATCOP_CXT_ORIG because, it will be used to decide
                   if call is being originated with at+cxt=1 option. Note that 
                   DS_ATCOP_CXT_ORIG is defined as -13 and not as -12  because 
                   some builds used -12 for other definition.
04/25/99   ak      Removed DS_ATCOP_CHNG_BITRATE, as we now use the ioctl
                   for chaning bitrate (under new sio), instead of the 
                   open/close construct.  Also extern ds_atcop_last_ipr_val
                   as its used in dsatcop.c. Lastly remove extern for 
                   ds_atcop_sched_bitrate_chng, since now have ioctl for
                   bitrate change.
04/15/99   smp     (ak) Renamed function do_bitrate_chng() to
                   ds_atcop_sched_bitrate_chng().
04/07/99   smp     Merged in changes to allow user control of automatic packet
                   origination via the AT$QCPKND command.
04/01/99   smp     Updates for IS-707-A format AT+CMUX command.
03/08/99   smp     Added extern for ds_atcop_qcmdr_val.
02/10/99   smp     Put MDR changes under FEATURE_SPECIAL_DR.
01/17/99   ak      Made new SIO a compile time feature.
01/14/99   smp     Added ds_atcop_qcmdr_val variable for storing the MDR Mode.
01/09/99   jjw     New interface definitions in support of new SIO interface
12/07/98   dbd     Added AT$QCFAMP command to force AMPS for testing
08/06/98   dbd     Added $QCDMR
04/27/98   dbd     Added $QCVAD
01/30/98   dbd     Removed obsolete commands;changed $QCSORS to +CMUX
01/26/98   ldg     Changed ds_atcop_qcsso_val to ds_atcop_qcsct_val.
11/09/97   na      Made ds_remote_carrier_state an extern.
11/04/97   ldg     Added AT$QCQNC to enable/disable QNC.
                   Added AT$QCSO to select originating service option set.
                   Added prototype for ds_atcop_nv_sync.
09/05/97   dbd     Fixed AT parmameter externs and types.
12/19/96   dbd     DOTG result code support
11/05/96   dbd     Added AT$QCOTC command
05/24/96   jjw     Gemini & ISS2 Data Services release.
12/04/95   dbd     created file to separate internal and external definitions

===========================================================================*/

#include "customer.h"
#include "sio.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS
                      
===========================================================================*/


/*---------------------------------------------------------------------------
    The numerical values in this "enum" are important:  
    The values for OK thru NO_ANSWER must match the numeric codes in EIA-602.                         
    The values for the cellular extention codes must begin with the 
      value MIN_CELL_RES (defined in dsatcopi.h), and increase in sequence.            
    The values for the cellular error codes must being with the value 
      MIN_CELL_ERR (defined in dsatcopi.h), and increase in sequence.            
    The values for the internal codes must be negative. 
---------------------------------------------------------------------------*/

typedef enum
  {
  /*-------------------------------------------------------------------------
    Numbered per EIA-602 
  -------------------------------------------------------------------------*/
  DS_ATCOP_OK             = 0,    /*  AT: OK            */
  DS_ATCOP_CONNECT        = 1,    /*  AT: CONNECT       */
  DS_ATCOP_RING           = 2,    /*  AT: RING          */
  DS_ATCOP_NO_CARRIER     = 3,    /*  AT: NO CARRIER    */
  DS_ATCOP_ERROR          = 4,    /*  AT: ERROR         */
  DS_ATCOP_NO_DIALTONE    = 6,    /*  AT: NO DIAL TONE  */
  DS_ATCOP_BUSY           = 7,    /*  AT: BUSY          */
  DS_ATCOP_NO_ANSWER      = 8,    /*  AT: NO ANSWER     */

  /*-------------------------------------------------------------------------
      Numbered per DOTG Spec  
      Numeric values reported only toDOTG 
  -------------------------------------------------------------------------*/
  DS_ATCOP_RING_AMPS      = 10,   /*  IS-99 Cell exten: RING AMPS        */
  DS_ATCOP_RING_ASYNC     = 11,   /*  IS-99 Cell exten: RING ASYNC       */
  DS_ATCOP_RING_FAX       = 12,   /*  IS-99 Cell exten: RING FAX         */
  DS_ATCOP_RING_PACKET    = 13,   /*  IS-99 Cell exten: RING PACKET      */

  /*-------------------------------------------------------------------------
      Numbered per DOTG Spec  
      Numeric values reported only toDOTG 
  -------------------------------------------------------------------------*/
  DS_ATCOP_NO_AMPS_SERVICE= 20,   /*  DOTG:             NO AMPS SERVICE   */
  DS_ATCOP_NO_SERVICE     = 21,   /*  IS-99 Cell exten: NO SERVICE        */
  DS_ATCOP_NO_ASYNC       = 22,   /*  IS-99 Cell exten: NO ASYNC SERVICE  */
  DS_ATCOP_NO_FAX         = 23,   /*  IS-99 Cell exten: NO FAX SERVICE    */
  DS_ATCOP_NO_PACKET      = 24,   /*  DOTG:             NO PACKET SERVICE */
  DS_ATCOP_BAD_REQUEST    = 25,   /*  IS-99 Cell exten: BAD REQUEST       */
  DS_ATCOP_PAGED          = 26,   /*  IS-99 Cell exten: PAGED             */
  DS_ATCOP_RETRY          = 27,   /*  is-99 Cell exten: RETRY             */
  DS_ATCOP_PAGE_FAIL      = 28,   /*  IS-99 Cell exten: PAGE FAIL         */
  DS_ATCOP_LINK_FAIL      = 29,   /*  IS-99 Cell exten: LINK FAIL         */

  /*-------------------------------------------------------------------------
    Not used by DOTG
  -------------------------------------------------------------------------*/
  DS_ATCOP_RELEASED       = 30,  /*  IS-99 Cell exten: RELEASED           */

#ifdef	FEATURE_SKT_DS_CELLRESULT	//	khekim 00/02/20
  DS_ATCOP_CPACKET			= 31,	// IS-707 Packet Cell extended result code
  DS_ATCOP_CPSR_0			= 32,	// Inactive State
  DS_ATCOP_CPSR_1			= 33,	// Active State, Init/Idle state
  DS_ATCOP_CPSR_2			= 34,	// Active State, Init/Traffic state
  DS_ATCOP_CPSR_3			= 35,	// Active State, Connected state/primary traffic
  DS_ATCOP_CPSR_4			= 36,	// Active State, Connected state/secondary traffic
  DS_ATCOP_CPSR_5			= 37,	// Active State, Dormant/Idle state
  DS_ATCOP_CPSR_6			= 38,	// Active State, Dormant/Traffic state
  DS_ATCOP_CPSR_7			= 39,	// Active State, Reconnect/Idle state
  DS_ATCOP_CPSR_8			= 40,	// Active State, Reconnect/Traffic state
  DS_ATCOP_CPER_0			= 41,	// Enter Idle State
  DS_ATCOP_CPER_1			= 42,	// Idle handoff, same system
  DS_ATCOP_CPER_2			= 43,	// Idle handoff, new system
  DS_ATCOP_CPER_3			= 44,	// Page received
  DS_ATCOP_CPER_4			= 45,	// Origination sent
  DS_ATCOP_CPER_5			= 46,	// Traffic Channel assigned
  DS_ATCOP_CPER_6			= 47,	// Hard handoff
#endif	/* FEATURE_SKT_DS_CELLRESULT */

  /*-------------------------------------------------------------------------
     Not reported to TE2
     Note that DS_ATCOP_CXT_ORIG is defined as -13 and not as -12  
     because some builds used -12 for other definition.
  -------------------------------------------------------------------------*/
#ifdef	FEATURE_SKT_DS	//	khekim 00/02/09
  DS_ATCOP_DO_ONLINE      = -15,
  DS_ATCOP_DO_HANGUP      = -14,
#endif	/* FEATURE_SKT_DS */
  DS_ATCOP_CXT_ORIG       = -13,  /*  internal: originate w/ unknown cmd  */

  DS_ATCOP_DO_QCOTC       = -11,  /*  internal: open traffic channel    */
  DS_ATCOP_DO_DM          = -10,  /*  internal: change to DM mode       */
  DS_ATCOP_DO_QCDAM       = -9,   /*  internal: analog dial for PCMCIA  */
  DS_ATCOP_DO_HARD_PARSE  = -8,   /*  internal: parse cmd w/o delimiter */
  DS_ATCOP_DO_CHV         = -7,   /*  internal: cellular hang up voice  */
  DS_ATCOP_DO_CDV         = -6,   /*  internal: cellular dial voice     */
  DS_ATCOP_TEST           = -5,   /*  internal: remote test command     */
  DS_ATCOP_PARAM          = -4,   /*  internal: parameter out of range  */
  DS_ATCOP_UNREC          = -3,   /*  internal: unrecognized command    */
  DS_ATCOP_DO_ANS         = -2,   /*  internal: answer  (auto or ATA)   */
  DS_ATCOP_DO_ORIG        = -1    /*  internal: originate data call     */
  } ds_atcop_result_enum_type;


extern ds_atcop_result_enum_type ds_atcop_result; /* running result of     */
                                                  /* processing a command  */
                                                  /* line                  */

/*---------------------------------------------------------------------------
   Dial string for Packet/QNC calls
---------------------------------------------------------------------------*/
extern byte ds_atcop_pkt_orig_str[NV_MAX_PKT_ORIG_DIGITS];


/*===========================================================================
      Definitions of the parameters set by the AT commands                  
===========================================================================*/
typedef byte  ds_atcop_string_item_type;
typedef unsigned int ds_atcop_num_item_type;
extern ds_atcop_num_item_type ds_atcop_qcpma_val;

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) || defined(FEATURE_DS_IS2000)
extern ds_atcop_num_item_type ds_atcop_qcmdr_val;
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR || FEATURE_DS_IS2000 */

#if defined(FEATURE_DS_IS2000) && defined(FEATURE_IS2000_R_SCH)
extern ds_atcop_num_item_type ds_atcop_qcscrm_val;
#endif
#if defined(FEATURE_DS_IS2000) && defined(FEATURE_IS2000_R_SCH)
extern ds_atcop_num_item_type ds_atcop_qctrtl_val;
#endif
#ifdef FEATURE_DS_QNC
extern ds_atcop_num_item_type ds_atcop_qcqnc_val;
#endif

#ifdef FEATURE_DS_CHOOSE_SO
extern ds_atcop_num_item_type ds_atcop_qcso_val;
#endif

#ifdef FEATURE_DS_VOICE_AS_DATA
extern ds_atcop_num_item_type ds_atcop_qcvad_val;
#endif

extern ds_atcop_num_item_type ds_atcop_qcpknd_val;

#ifdef FEATURE_DS_IS707A_CMUX
extern ds_atcop_num_item_type ds_atcop_cmux_val[];
extern const word ds_atcop_cmux_fwd_table[];
extern const word ds_atcop_cmux_rev_table[];
#else
extern ds_atcop_num_item_type ds_atcop_cmux_val;
#endif  /* FEATURE_DS_IS707A_CMUX */

extern ds_atcop_num_item_type ds_atcop_qcsct_val;
extern ds_atcop_num_item_type ds_atcop_amp_c_val;
extern ds_atcop_num_item_type ds_atcop_amp_d_val;
extern ds_atcop_num_item_type ds_atcop_amp_f_val;
extern ds_atcop_num_item_type ds_atcop_cdr_val;
extern ds_atcop_num_item_type ds_atcop_cds_val[];
extern ds_atcop_num_item_type ds_atcop_cfc_val;
extern ds_atcop_num_item_type ds_atcop_cqd_val;
extern ds_atcop_num_item_type ds_atcop_crc_val;
extern ds_atcop_num_item_type ds_atcop_crm_val;
extern ds_atcop_num_item_type ds_atcop_cta_val;
extern ds_atcop_num_item_type ds_atcop_cxt_val;
extern ds_atcop_num_item_type ds_atcop_dr_val;
extern ds_atcop_num_item_type ds_atcop_ds_val[];
extern ds_atcop_num_item_type ds_atcop_eb_val[];
extern ds_atcop_num_item_type ds_atcop_efcs_val;
extern ds_atcop_num_item_type ds_atcop_er_val;
extern ds_atcop_num_item_type ds_atcop_es_val[];
extern ds_atcop_num_item_type ds_atcop_esr_val;
extern ds_atcop_num_item_type ds_atcop_etbm_val[];
extern ds_atcop_num_item_type ds_atcop_faa_val;
extern ds_atcop_num_item_type ds_atcop_fap_val[];
extern ds_atcop_num_item_type ds_atcop_fbo_val;
extern ds_atcop_num_item_type ds_atcop_fbu_val;
extern ds_atcop_num_item_type ds_atcop_fcc_val[];
extern ds_atcop_num_item_type ds_atcop_fclass_val;
extern ds_atcop_num_item_type ds_atcop_fcq_val[];
extern ds_atcop_num_item_type ds_atcop_fcr_val;
extern ds_atcop_num_item_type ds_atcop_fct_val;
extern ds_atcop_num_item_type ds_atcop_fea_val;
extern ds_atcop_num_item_type ds_atcop_ffc_val[];
extern ds_atcop_num_item_type ds_atcop_fhs_val;
extern ds_atcop_num_item_type ds_atcop_fie_val;
extern ds_atcop_num_item_type ds_atcop_fip_val;
extern ds_atcop_num_item_type ds_atcop_fis_val[];
extern ds_atcop_num_item_type ds_atcop_flo_val;
extern ds_atcop_num_item_type ds_atcop_flp_val;
extern ds_atcop_num_item_type ds_atcop_fms_val;
extern ds_atcop_num_item_type ds_atcop_fnr_val[];
extern ds_atcop_num_item_type ds_atcop_fpp_val;
extern ds_atcop_num_item_type ds_atcop_fpr_val;
extern ds_atcop_num_item_type ds_atcop_fps_val;
extern ds_atcop_num_item_type ds_atcop_frq_val[];
extern ds_atcop_num_item_type ds_atcop_fry_val;
extern ds_atcop_num_item_type ds_atcop_fsp_val;
extern ds_atcop_num_item_type ds_atcop_icf_val[];
extern ds_atcop_num_item_type ds_atcop_ifc_val[];
extern ds_atcop_num_item_type ds_atcop_ilrr_val;
#ifdef	FEATURE_SKT_DS	//	khekim 00/02/20
extern ds_atcop_num_item_type ds_atcop_bsprev_val;
#endif	/* FEATURE_SKT_DS */
#ifdef	FEATURE_SKT_DS_CELLRESULT	//	khekim 00/02/20
extern ds_atcop_num_item_type ds_atcop_cps_val; 
extern ds_atcop_num_item_type ds_atcop_cpsr_val;
extern ds_atcop_num_item_type ds_atcop_cptc_val;
extern ds_atcop_num_item_type ds_atcop_cper_val;
#endif	/* FEATURE_SKT_DS_CELLRESULT */

#ifndef FEATURE_ANALOG_DATA   /*  These commands not applicable */
                              /*  to SSS products               */
extern ds_atcop_num_item_type ds_atcop_ipr_val;
extern ds_atcop_num_item_type ds_atcop_last_ipr_val;
#endif  /*  not FEATURE_ANALOG_DATA */
extern ds_atcop_num_item_type ds_atcop_mr_val;
extern ds_atcop_num_item_type ds_atcop_mv18r_val;
extern ds_atcop_num_item_type ds_atcop_mv18s_val[];
extern ds_atcop_num_item_type ds_atcop_e_val;
extern ds_atcop_num_item_type ds_atcop_l_val;
extern ds_atcop_num_item_type ds_atcop_m_val;
extern ds_atcop_num_item_type ds_atcop_o_val;
extern ds_atcop_num_item_type ds_atcop_q_val;
extern ds_atcop_num_item_type ds_atcop_s0_val;
extern ds_atcop_num_item_type ds_atcop_s10_val;
extern ds_atcop_num_item_type ds_atcop_s11_val;
extern ds_atcop_num_item_type ds_atcop_s3_val;
extern ds_atcop_num_item_type ds_atcop_s4_val;
extern ds_atcop_num_item_type ds_atcop_s5_val;
extern ds_atcop_num_item_type ds_atcop_s6_val;
extern ds_atcop_num_item_type ds_atcop_s7_val;
extern ds_atcop_num_item_type ds_atcop_s8_val;
extern ds_atcop_num_item_type ds_atcop_s9_val;
extern ds_atcop_num_item_type ds_atcop_v_val;
extern ds_atcop_num_item_type ds_atcop_x_val;
extern ds_atcop_num_item_type ds_atcop_z_val;
extern ds_atcop_string_item_type ds_atcop_cfg_val[];
extern ds_atcop_string_item_type ds_atcop_fli_val[];
extern ds_atcop_string_item_type ds_atcop_fns_val[];
extern ds_atcop_string_item_type ds_atcop_fpa_val[];
extern ds_atcop_string_item_type ds_atcop_fpi_val[];
extern ds_atcop_string_item_type ds_atcop_fpw_val[];
extern ds_atcop_string_item_type ds_atcop_fsa_val[];
extern ds_atcop_string_item_type ds_atcop_ma_val[];
extern ds_atcop_string_item_type ds_atcop_ms_val[];

#ifdef FEATURE_DS_SOCKETS_TEST_UDP
#error code not present
#endif /* FEATURE_DS_SOCKETS_TEST_UDP */

#ifdef FEATURE_DS_SOCKETS
extern ds_atcop_string_item_type ds_atcop_qcdnspri_val[];
extern ds_atcop_string_item_type ds_atcop_qcdnssec_val[];

extern uint32 ds_atcop_qcdns_val[];
#endif /* FEATURE_DS_SOCKETS */
#ifndef FEATURE_ANALOG_DATA   /*  Not applicable  */
                              /*  to SSS products */
extern ds_atcop_num_item_type ds_atcop_qcdmr_val;
#endif  /*  not FEATURE_ANALOG_DATA */
#if defined(FEATURE_DS_DOTG_DATA)
#error code not present
#endif  /*  FEATURE_DS_DOTG_DATA */ 
#ifdef  FEATURE_DS_MTOM
extern ds_atcop_string_item_type ds_atcop_qcmtom_val[];
#endif  /* FEATURE_DS_MTOM  */
extern ds_atcop_string_item_type ds_atcop_qcotc_val[];
extern boolean ds_remote_carrier_state;            /* Remote carrier state */
extern boolean ds_pkt_orig_ok;





/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/



/*===========================================================================

FUNCTION DS_ATCOP_PROCESS_SIO_COMMAND

DESCRIPTION
  This function builds the AT command line from data obtained from the SIO
  watermark, ds_from_sio_wm_item. The command line begins with the "AT"
  characters (either case or mixed case, but with nothing in between), 
  and is terminated with the end of line character stored in modem 
  register S3.  The backspace character (in modem register S5) is processed.
  
  When a complete line has been accumulated, ds_atcop (AT_CMD) is called.
  The line will have been edited per the backspace (above), and the AT
  removed.  The end of line character will have been replaced with the null
  character, '\0'.  Any spaces will still be present. The eighth bit of all 
  characters will have been set to zero.

  If the character combination, "A/" or "a/" is encountered while looking
  for the beginning of a command line, the previous command line, if any,
  will be immediately passed to ds_atcop (AT_CMD).

  If the command line buffer overflows, the data will be discarded and
  an error flag set.  The error will be reported when the end of line
  character in S3 is encountered.

  Input data will be echoed to the serial port via DSRMIO_SETUP_DS_TX ().
  Data is echoed at the end of each input data item, when a small data item
  (used for echo output) is filled, or when the S3 character is found, 
  or the buffer overflows, whichever comes first.  The echo is 
  controlled by the 'ATE' flag.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Buffer accumulates data until a full line is found.

  See also ds_atcop which has many side effects.

===========================================================================*/
extern void  ds_atcop_process_sio_command
(
  void
) ;


/*===========================================================================

FUNCTION DS_ATCOP_PROCESS_SIO_ONLINE

DESCRIPTION
  This function reads the data stream from the ds_from_sio_wm_item. 
  Transparency sequences for the EIA-617 <EM> codes and the and the 
  IS-99 <IAC> compression negotiation transparency codes are inserted.  
  Data is queued using dsm_enqueue for transmission to the Protocol 
  Services Task and the base station.

  (Eventually, ds_atcop_process_sio_online will process the <DLE> encoded 
  buffer query as specified in EIA-592 and return the status via a <DLE> 
  code on SIO.  This is pending further study in IS-99.  When this is 
  implemented, the <DLE> sequence must be presented in its entirety to 
  DSRMIO_SETUP_DS_TX.)

LIMITATION
  The transparency codes specified in EIA-617 include a special code for
  two consecutive occurances of the <EM> byte.  This function implements
  this feature EXCEPT when the two consecutive <EM> bytes are in 
  different buffers.  In that case, the output stream contains two
  instances of the single <EM> transparency code.  This causes a minor
  drop in efficiency, but should cause no other error.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/

extern void  ds_atcop_process_sio_online
(
  void
) ;


/*===========================================================================

FUNCTION DS_ATCOP_PROCESS_PS

DESCRIPTION
  This function reads the data stream from the protocol stack.
  It strips off the <IAC> compression negotiation codes and
  transparency sequences.  Responses to the compression negotiation
  are generated, and sent back to the base station as <IAC> sequences
  via dsm_enqueue.  The <IAC> sequence will be presented in its entirety
  to dsm_enqueue.  If enabled by the AT+CDR command, an unsolicited
  status message will be sent to the user via DSRMIO_SETUP_DS_TX().

  It then strips out all EIA-617 <EM> in-band signals, and <EM>
  transparency codes. The reflected AT codes transmitted as EIA-617
  in-band signals are use to call ds_atcop (DS_ATCOP_ONLINE). Any spaces
  remain.

  EIA-617 modem pin status signals are transmitted to the SIO driver
  via the sio_xxx_procedure calls.

  Response messages are removed from the EIA-617 STATUS messages, and
  passed to the serial port.

  The remainder of the data stream is passed to the serial port via
  DSRMIO_SETUP_DS_TX for transmission to the TE2.

DEPENDENCIES
  The "AT" characters and the end of line character are assumed not
  to be present in the reflected AT codes carried in in-band signals.

  The necessary transparency codes are assumed to have been imbedded
  at the base station.

  'cnt_617', 'index_617', 'cnt_iac', 'index_iac', and 'sub_neg_iac'
  must be set to 0  before starting to process the input data stream'.
  This is done by ds_atcop_call_init().

  The output items used by do_em and do_iac may be longer than
  the input item.  This is safe for the following reasons:

  The output items are modified by applying DSM_ITEM_HEAD which
  increases its size by the size of the TCP header and footer.

  The maximum size gain in do_iac is 14 bytes. The maximum
  size gain in do_em is 99 bytes.  (See the discussion of
  the individual functions.)

  NB: As long as DSM_ITEM_HEAD increases the available space
      in the output item by more than the sum of the maximum
      expansion in do_em and do_iac (113 bytes),  and

      as long as the header area has _not_ been used in the
      input item received from the protocol stack,

      it is safe to allow the output items to expand.

  Both do_em and do_iac terminate when the output exceeds the
  basic size of a large item, even if there is still input to
  process.  They set em_flag or iac_flag so that processing of
  the input item will be resumed after the output item has been
  processed.

RETURN VALUE
  None.

SIDE EFFECTS
  No side effects except thru call to ds_atcop.

===========================================================================*/
extern void  ds_atcop_process_ps
(
  dsm_item_type *in_item_ptr
) ;


/*===========================================================================

FUNCTION DS_ATCOP_INIT_CONFIG

DESCRIPTION
  This function initializes the complete set of AT command paramters
  to their default values.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/

void ds_atcop_init_config
(
  void
) ;


/*===========================================================================

FUNCTION DS_ATCOP_RETURN_CELL_RESULT

DESCRIPTION
  Sends requested result code to TE2 if ATQ is 0.

  If AT+CRC is 0, only the basic result codes are returned.
  The extended RING codes are translated to the basic code.
  Basic result codes are returned if ATQ is 0.

  Cellular result codes are returned if AT+CRC is 1.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void ds_atcop_return_cell_result
(
  ds_atcop_result_enum_type res_cod
) ;


/*===========================================================================

FUNCTION DS_ATCOP_SEND_CONFIG

DESCRIPTION
  This function transmits the mobile configuration to the base station.
  Configuration parameters are those which have the DOWN_LOAD attribute
  in the parse table.  The only DOWN_LOAD paramters transmitted are those
  with the ALWAYS attribute in the parse table, and those which
  have been changed from their default values.  Parameters with
  the FCLASS2P0 attribute are not transmitted unless the current value
  of the FCLASS parameter is 2.0.

  Follow the configuration with the +CFG string (always, even if empty),
  and then the current command.

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/
void ds_atcop_send_config
(
  byte *
) ;


/*===========================================================================

FUNCTION DS_ATCOP_SEND_ESCAPE

DESCRIPTION
  Sends EIA 617 "escape to online command" message to base station.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void ds_atcop_send_escape
(
  void
) ;


/*===========================================================================

FUNCTION DS_ATCOP_CALL_INIT

DESCRIPTION
  This function does per call initialization:

  * Initialize counters and indices for the EIA 617 and IAC escape
    sequence processing.  Initializes the FDT command seen flag
    for fax flow control.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void ds_atcop_call_init
(
  void
) ;


/*===========================================================================

FUNCTION DS_ATCOP_NV_SYNC

DESCRIPTION
  This function initializes from NV the ATCOP defaults for
  those parameters stored in NV.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/
void ds_atcop_nv_sync(void);

/*===========================================================================

FUNCTION DS_ATCOP_DAT_INIT

DESCRIPTION
  This function initializes the AT command defaults, limits, and lists
  that depend on a run-time determination of the hardware.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/

void ds_atcop_dat_init
(
  void
) ;

/*===========================================================================

FUNCTION DS_CHANGE_BAUD

DESCRIPTION
  This function is called by other modules to write the value to the current 
  baud rate variable, ds_atcop_ipr_val. Whenever the baud rate is changed
  it should also be entered into the NV if FEATURE_DS_DEFAULT_BITRATE_NV
  is defined. So this function also writes into the NV if that feature is
  defined.
  The second parameter in this function is a boolean type. It informs 
  the function if the Baud rate should be changed NOW or later (later
  implies, after sending acknowledgement). This is needed because, when 
  UI changes the baud rate, the baud rate should take effect immediately. 
  But when an AT command changes the baud rate, the baud rate should be 
  changed after an "OK" is sent to the TE2. 
  

DEPENDENCIES
  This code does not check for auto baud enabled or not. So, it is the 
  responsibility of the calling function to ascertain that if 
  FEAUTRE_AUTOBAUD is not enabled then value 0 should not be passed.
  
RETURN VALUE
  None

SIDE EFFECTS
  Modifies the value of the ds_atcop_ipr_val and its corresponding value in 
  the NV, ds_default_baudrate in the NV.    
===========================================================================*/

extern void ds_change_baud
(
    sio_bitrate_type new_baud_rate,
    boolean change_baud_now
);

#endif /* DSATCOP_H */

