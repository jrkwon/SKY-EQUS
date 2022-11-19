#ifndef DSATCOPI_H
#define DSATCOPI_H

/*===========================================================================

                        D A T A   S E R V I C E S

                A T   C O M M A N D   P R O C E S S O R

                I N T E R N A L   H E A D E R   F I L E


DESCRIPTION
  This file contains the definitions of data structures, defined and
  enumerated constants and function prototypes required for the 
  data services AT command processor, and the EIA 617 and telnet
  inband signaling.

   Copyright (c) 1991,1992 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1995,1996 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1997,1998 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 2000, 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/dsatcopi.h_v   1.2   06 Feb 2001 12:09:16   nabrol  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/05/01   na      Removed references to FEATURE_DS_PCM.
12/15/00   ak      New var dflm_type ds_atcop_qctrtl_dflm[] used to init
                   ds_atcop_qctrtl_val, which is used in turning off R-SCH
                   throttling in IS2000.
08/07/00   js/jjw  Merged in the improved inline documentations. 
07/24/00   ak      Support for AT$QCSCRM, which allows SCRM to be enabled/
                   disabled.
06/27/00   js/snn  Merged in changes to allow DNS service via three 
                   additional extended format proprietary commands: 
                   AT$QCDNSP, AT$QCDNSS and AT$QCDOM.
06/22/00   igt     new #def's to support AT$QCRL3D and AT$QCRL3R
02/02/00   ak      Merged rlp3 branch against tip.
01/26/00   na      Fixed compile time error in #define.
01/06/00   na      Put SPECIAL_QCMDR also under FEATURE_DS_RLP3.
11/15/99   snn     Added extern ds_atcop_sizeof_fns_val to support reversion 
                   of ds_atcop_fns_val.
08/05/99   rc      Added support for UDP debug statistics for Sockets.
04/07/99   smp     Merged in changes to allow user control of automatic packet
                   origination via the AT$QCPKND command.
03/08/99   smp     Added extern for ds_atcop_qcmdr_dflm[].
02/10/99   smp     Put MDR changes under FEATURE_SPECIAL_MDR.
01/14/99   smp     Added define for SPECIAL_QCMDR under FEATURE_IS95B_MDR.
01/09/99   jjw     Interface changes to support generic Browser interface
12/07/98   dbd     Added AT$QCFAMP command to force AMPS for testing
11/12/98   ldg     Added const qualifier to some declarations and typedefs.
                   Moved const qualifer to beginning of line where possible.
10/27/98   ldg     ARM porting: ROM to ROM const.
08/19/98   dbd     Added $QCQMIP command under FEATURE_QMIP
08/06/98   dbd     Added $QCDMR; added support for a LIST type
05/01/98   dbd     Added prototype for ds_atcop_is_status()
04/27/98   dbd     Added $QCCLR and $QCVAD
03/04/98   dbd     Added +CMUX to SPECIAL list.  Deleted S88
01/30/98   dbd     Added FAX XON/XOFF supoort
01/27/98   ldg     Removed the commented-out DS_SLIM switch.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
11/12/97   dbd     Removed obsolete define SPECIAL_AMP_C.
11/04/97   ldg     Added AT$QCQNC and AT$QCSO.
09/05/97   dbd     Added AT$QCMTOM. Fixed AT parmameter externs and types.
06/16/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/12/97   ldg     Added AT commands for new FEATURE_DS_PSTATS
04/02/97   jjw     Fixed format for +MS and +MA AT commands
02/13/97   na      Dave added new AT commands. CTIA show build.
12/19/96   dbd     DOTG result code support; fixed bad enum name
11/05/96   dbd     Removed DS_SLIM switch.
09/05/96   dbd     Added new command reflection for long items; fixed bugs
05/24/96   jjw     Gemini & ISS2 Data Services release.
12/04/95   dbd     created file to separate internal and external definitions

===========================================================================*/

#include "customer.h"
#include "comdef.h"
#include "dsatcop.h"
#include "target.h"

#ifdef FEATURE_DS
/*===========================================================================
                      AT Command Processor constants
===========================================================================*/

/*--------------------------------------------------------------------------
                   Common Limitation Constants
--------------------------------------------------------------------------*/
#define MAX_ARG 12              /*  Maximum number of args for compound    */
                                /*  AT command                             */

#define MAX_LINE_SIZE 260       /*  Maximum length of AT command line     */
                                /*  (including terminating null char      */

#define MIN_ROOM  270           /*  A data item must have at least this   */
                                /*  much room if a new item is to be put  */
                                /*  in it. Based on longest name, longest */
                                /*  response, formatting and some padding */



/*--------------------------------------------------------------------------
                Syntax Flags For the AT Command Parser 
--------------------------------------------------------------------------*/
#define NA    1                 /*  Name field found      */
#define EQ    2                 /*  <=> found             */
#define QU    4                 /*  <?> found             */
#define AR    8                 /*  Argument field found  */




/*===========================================================================
                      Special Processing Codes 
                        
    The following definitions represent the special processing codes that 
    must be assigned to each AT command that requires some special processing 
    in addition to, or instead of, the standard processing performed by the 
    AT command processing engine. 
    SPECIAL_NONE if no special processing is required. 
===========================================================================*/
#define SPECIAL_NONE              0x00
#define SPECIAL_FCLASS            0x01
#define SPECIAL_FCC               0x02
#define SPECIAL_CAD               0x03
#define SPECIAL_CBC               0x04
#define SPECIAL_CBIP              0x05
#define SPECIAL_CHV               0x06
#define SPECIAL_CMIP              0x07
#define SPECIAL_CMUX              0x08
#define SPECIAL_CSQ               0x09
#define SPECIAL_CSS               0x0A
#define SPECIAL_FBS               0x0B
#define SPECIAL_FDR               0x0C
#define SPECIAL_FDT               0x0D
#define SPECIAL_FHS               0x0E
#define SPECIAL_FKS               0x0F
#define SPECIAL_FMI               0x10
#define SPECIAL_FMM               0x11
#define SPECIAL_FMR               0x12
#define SPECIAL_FNS               0x13
#define SPECIAL_GCAP              0x14
#define SPECIAL_GMI               0x15
#define SPECIAL_GMM               0x16
#define SPECIAL_GMR               0x17
#define SPECIAL_GOI               0x18
#define SPECIAL_GSN               0x19
#define SPECIAL_IPR               0x1A
#define SPECIAL_QCDMG             0x1B
#define SPECIAL_QCDMR             0x1C
#define SPECIAL_QCPMA             0x1D
#define SPECIAL_AMP_F             0x1E
#define SPECIAL_AMP_V             0x1F
#define SPECIAL_Z                 0x20
#define SPECIAL_QCOTC             0x21
#define SPECIAL_QCRLPD            0x22  /*  Used only if DS_PSTATS defined */
#define SPECIAL_QCRLPR            0x23  /*  Used only if DS_PSTATS defined */
#define SPECIAL_QCPPPD            0x24  /*  Used only if DS_PSTATS defined */
#define SPECIAL_QCPPPR            0x25  /*  Used only if DS_PSTATS defined */
#define SPECIAL_QCIPD             0x26  /*  Used only if DS_PSTATS defined */
#define SPECIAL_QCIPR             0x27  /*  Used only if DS_PSTATS defined */
#define SPECIAL_QCCLR             0x28  /*  Used only if DS_PSTATS defined */

#if defined(FEATURE_DS_BROWSER_INTERFACE) || defined(FEATURE_DS_SOCKETS)
#define SPECIAL_QCUDPD            0x29  /*  Used only if DS_PSTATS defined */
#define SPECIAL_QCUDPR            0x2A  /*  Used only if DS_PSTATS defined */
#endif /* FEATURE_DS_BROWSER_INTERFACE || FEATURE_DS_SOCKETS*/

#define SPECIAL_QCTCPD            0x2B  /*  Used only if DS_PSTATS defined */
#define SPECIAL_QCTCPR            0x2C  /*  Used only if DS_PSTATS defined */

#ifdef  FEATURE_DS_MTOM
#define SPECIAL_QCMTOM            0x2D  /*  Used only if FEATURE_DS_MTOM   */ 
#endif  /*  FEATURE_DS_MTOM */

#ifdef FEATURE_DS_QNC
#define SPECIAL_QCQNC             0x2E  /*  Used only if FEATURE_DS_QNC    */ 
#endif

#ifdef FEATURE_DS_CHOOSE_SO
#define SPECIAL_QCSO              0x2F  /*  Only if FEATURE_DS_CHOOSE_SO   */
#endif

#ifdef FEATURE_DS_VOICE_AS_DATA             
#define SPECIAL_QCVAD             0x30
#endif

#ifdef FEATURE_QMIP 
#error code not present
#endif  /*  FEATURE_QMIP  */

#if defined(FEATURE_DS_DOTG_DATA)
#error code not present
#endif /* FEATURE_DS_DOTG_DATA */

#define SPECIAL_QCCAV             0x33  /*  Answer Voice call              */
#define SPECIAL_QCPKND            0x34  /*  Orig Packet without dialling   */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) || defined(FEATURE_DS_IS2000)
#define SPECIAL_QCMDR             0x35
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR || FEATURE_DS_IS2000 */

#define SPECIAL_QCRLPD33          0x36  /*used with DS_PSTATS && DS_IS2000 */
#define SPECIAL_QCRLPR33          0x37  /*used with DS_PSTATS && DS_IS2000 */


#ifdef FEATURE_DS_SOCKETS
#define SPECIAL_QCDNSPRI          0x38
#define SPECIAL_QCDNSSEC          0x39
#endif /* FEATURE_DS_SOCKETS */

#ifdef FEATURE_DS_SOCKETS_TEST_UDP
#error code not present
#endif /* FEATURE_DS_SOCKETS_TEST_UDP */

#if defined(FEATURE_DS_IS2000) && defined(FEATURE_IS2000_R_SCH)
#define SPECIAL_QCSCRM            0x3B
#endif /* FEATURE_DS_IS2000  && FEATURE_IS2000_R_SCH */

#if defined(FEATURE_DS_IS2000) && defined(FEATURE_IS2000_R_SCH)
#define SPECIAL_QCTRTL            0x3C
#endif /* FEATURE_DS_IS2000  && FEATURE_IS2000_R_SCH */

#ifdef	FEATURE_SKT_DS	//	khekim 00/01/23
#define	SPECIAL_CRM					0x40	//	khekim 00/02/11
#define	SPECIAL_CTA					0x41	//	khekim 00/02/20
#define	SPECIAL_SKPREV				0x42	//	khekim 00/02/20
#endif	/* FEATURE_SKT_DS */
#ifdef	FEATURE_SKT_DR_DS	//	khekim 00/02/14
#define	SPECIAL_SKTDR				0x47
#endif	/* FEATURE_SKT_DR_DS */

/*===========================================================================
            Indexes Into the "ds_atcop_pls_table" Array  
       
    WARNING: if the "ds_atcop_pls_table" array is modified or 
    rearranged, these values MUST be changed to match!                        
===========================================================================*/
#define LOC_CFG  1                  /*  Location of CFG command entry */
#define LOC_FCC  2                  /*  Location of FCC command entry */
#define LOC_FIS  3                  /*  Location of FIS command entry */


/*===========================================================================
               Bit-Mapped AT Command Attributes Constants 
===========================================================================*/
#define ATTRIB_NONE   0x0000    /*  No attributes                     */
#define CONFIG        0x8000    /*  Variable: takes argument          */
#define FCLASS2P0     0x4000    /*  Class 2.0 Fax item                */
#define LOCAL_TEST    0x2000    /*  Execute =? command locally        */               
#define ALWAYS        0x1000    /*  always transmit in config         */
#define SIMPLE_DEC    0x0800    /*  3 digit, leading 0 decimal        */
#define STRING        0x0400    /*  Value is string                   */
#define HEX           0x0200    /*  Numeric value is Hex              */
#define NO_QUOTE      0x0200    /*  String value has no '"'s          */
#define EXTENDED      0x0100    /*  Extended syntax                   */
#define READ_ONLY     0x0080    /*  Read only; may have '?' in syntax */
#define DOWN_LOAD     0x0040    /*  Transmit in config if not default */
#define LIST          0x0020    /*  def_lim_ptr points to a list      */

/*---------------------------------------------------------------------------
    If none of the following attributes are set, the command is       
    valid only if AT$QCPMA (ds_atcop_qcpma_val) == 0                  
---------------------------------------------------------------------------*/
#define PMA_X         0x0010    /*  Command valid for any $QCPMA val  */
#define PMA_2         0x0008    /*  Command valid if $QCPMA == 2      */
#define PMA_1         0x0004    /*  Command valid if $QCPMA == 1      */



#define MAX_VAL_NUM_ITEM 0xFFFF /*  Maximum value of a numeric AT parameter */
/*---------------------------------------------------------------------------
    The following defines MUST match the values in                     
    "ds_atcop_result_enum_type" in dsatcop.h".        
    MIN_CELL_RES must be LESS than MIN_CELL_ERR       
---------------------------------------------------------------------------*/
#define MIN_CELL_RES  10  /*  First cellular result code. */
#define MIN_CELL_ERR  20  /*  First cellular result code with +CERROR: prefix */

#ifdef	FEATURE_SKT_DS_CELLRESULT	//	khekim 00/02/20
#define MIN_CELL_EXTRES	31	//	Last Cellular result code: (including +CPACKET)
#endif	/* FEATURE_SKT_DS_CELLRESULT */

/*===========================================================================
                      EIA-617 In-Band Signals Definitions 
 ==========================================================================*/

/*---------------------------------------------------------------------------
   Leading byte  
---------------------------------------------------------------------------*/
#define EIA617_EM               0x19

/*---------------------------------------------------------------------------
  Primary codes mobile to base  
---------------------------------------------------------------------------*/
#define EIA617_EXTEND1_M2B      0x41

/*---------------------------------------------------------------------------
  Sub codes for EXTEND1_M2B     
---------------------------------------------------------------------------*/
#define EIA617_ESCAPE           0x42
#define EIA617_UNREC            0x44
#define EIA617_PARAM            0x45
#define EIA617_VALID            0x46

#ifndef FEATURE_DS_IS99_REFL  /*  Define this switch for 'original' IS-99 reflection  */
#define EIA617_RESP_2B          0x47  /*  New code for response from BS */
#else
#define EIA617_RESP_2B          0x46  /*  Old: response same as VALID       */
#endif

#define EIA617_SINGLE_EM_M2B    0x5C
#define EIA617_DOUBLE_EM_M2B    0x5D
        
/*---------------------------------------------------------------------------
    Primary codes base to mobile  
---------------------------------------------------------------------------*/
#define EIA617_EM_109_OFF       0x66
#define EIA617_EM_109_ON        0x67
#define EIA617_EM_125_OFF       0x6A
#define EIA617_EM_125_ON        0x6B
#define EIA617_FLOW_OFF         0x7A
#define EIA617_FLOW_ON          0x7B
#define EIA617_SINGLE_EM_B2M    0x7C
#define EIA617_DOUBLE_EM_B2M    0x7D
#define EIA617_LEN_OFFSET       0x1F

#define EIA617_EXTEND0_B2M      0x60

/*---------------------------------------------------------------------------
  Sub codes for EXTEND0_B2M     
---------------------------------------------------------------------------*/  
#define EIA617_CONTROL          0x42
#define EIA617_STATUS           0x62

#define EIA617_EXTEND1_B2M      0x61

/*---------------------------------------------------------------------------  
  Sub codes for EXTEND0_B2M     
---------------------------------------------------------------------------*/  
#define EIA617_RESP_2M          0x43



/*===========================================================================
                      IS-99 In-Band Signals Definitions 
===========================================================================*/
#define IS99_IAC            255
#define IS99_DONT           254
#define IS99_DO             253
#define IS99_WONT           252
#define IS99_WILL           251
#define IS99_SB             250
#define IS99_SE             240



/*===========================================================================
                     Debugging Message Definitions
===========================================================================*/
#define DS_ATCOP_ERROR_MSG()  MSG_MED("DS_ATCOP_ERROR result", 0, 0, 0)




/*===========================================================================
                 AT Command Processor Data Structures
===========================================================================*/

/*---------------------------------------------------------------------------
     The following enumerated type represents the the two states of ATCoP: 
     command state and online state. The MT2 is in command state when the 
     transport layer is not in the ESTABLISHED state. When in command state, 
     all AT commands received on Rm are processed. When the MT2 is in the 
     online state, the MT2 passes all data received on the Rm directly to 
     the IWF, and does not process AT commands received on the Rm.                                                              
---------------------------------------------------------------------------*/
typedef enum
  {
    DS_ATCOP_CMD, 
    DS_ATCOP_ONLINE
  } ds_atcop_mode_enum_type;


/*---------------------------------------------------------------------------
  The following structure is the data type for tokens in the parsing of   
  the AT commands. 

  working_at_line - Stores command lines to be processed. Each line is 
                    referenced by a line number.  
  name            - The name of the AT command.
  op              - Syntax flags. Can be one of four valid values (NA, 
                    EQ, QU, AR) or an invalid value. 
  arg             - AT command arguments.
  args_found      - Keeps track of the number of AT command arguments 
                    found.
---------------------------------------------------------------------------*/
typedef struct
  {
   byte working_at_line[MAX_LINE_SIZE];                                           
   byte *name;                        
   unsigned int op;                    
   byte * arg[MAX_ARG];                  
   unsigned int args_found;              
  } tokens_struct_type;


/*---------------------------------------------------------------------------
   The data type below is used to define the default value and the lower and 
   upper bounds of the arguments for AT commands with numeric arguments.         
---------------------------------------------------------------------------*/
typedef struct
  {
   ds_atcop_num_item_type default_v;   /* The default value for the argument*/
   const ds_atcop_num_item_type lower; /* Lower limit of the allowed range. */
   const ds_atcop_num_item_type upper; /* Upper limit of the allowed range. */
  } dflm_type;
  

/*--------------------------------------------------------------------------
   The data type below is used to define the default value and the allowed 
   range of values of the arguments, for AT commands of type LIST.               

    default_v   - Default value of the argument.
    list_v      - A pointer to and array of 8-byte strings that represent 
                  all of the valid values that the argument can assume. The 
                  ATCoP performs a string match with the elements in this 
                  array order to determine whether or not an argument's 
                  value is within valid range.
    list_t      - Pointer to a string that is returned word for word in 
                  response to a test command. 
--------------------------------------------------------------------------*/    
typedef struct
  {
   ds_atcop_num_item_type default_v;            
   const ds_atcop_string_item_type (*list_v)[8];
   const ds_atcop_string_item_type *list_t;     
  } def_list_type;


/*---------------------------------------------------------------------------
   The following data type is the data structure for a parser table entry.      
   
   name        -  The name of the AT cmd, including the leading "+", "$" 
                  or "$" and the terminating NULL.
   attrib      -  A 16-bit mask that specifies one or more command 
                  attributes.       
   special     -  The "special processing code," if any, or SPECIAL_NONE. 
                  The special processing codes are defined in this file. 
                  A unique special processing code must be assigned to 
                  each AT cmd that requires some special processing. 
   compound    -  The number of arguments, for numeric commands, or the 
                  maximum string length (not including the terminating NULL), 
                  for string commands.
   val_ptr     -  A pointer to the variable that stores the argument value. 
                  If the variable is an array, this is a pointer to the 
                  first element of the array. For commands with the LIST 
                  attribute the value stored in *val_ptr are not the 
                  actual parameters. Instead, they are indices into array 
                  pointed to by list_v.
   def_lim_ptr -  Defines the default value and the allowed range for the 
                  arguments.     
---------------------------------------------------------------------------*/
typedef struct
  {
  byte name[8];         
  unsigned int attrib;    
  byte special;           
  byte compound;          
  const void *val_ptr;     
  const void *def_lim_ptr; 
  } ds_atcop_cmd_type;

/*---------------------------------------------------------------------------
     Data type for a list strings. Used in function ds_atcop_srch_list.    
---------------------------------------------------------------------------*/  
typedef byte list_type[][8];    

/*---------------------------------------------------------------------------
   Declarations of constant default, lower bound and upper bound for 
   arguments of AT command with numeric arguments.                         
---------------------------------------------------------------------------*/
extern const dflm_type ds_atcop_amp_c_dflm[];
extern const dflm_type ds_atcop_amp_d_dflm[];
extern const dflm_type ds_atcop_flo_dflm[];
extern const dflm_type ds_atcop_ifc_dflm[];
extern const dflm_type ds_atcop_e_dflm[];
extern const dflm_type ds_atcop_q_dflm[];
extern const dflm_type ds_atcop_v_dflm[];

extern dflm_type ds_atcop_qcpknd_dflm[];

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) || defined(FEATURE_DS_IS2000)
extern dflm_type ds_atcop_qcmdr_dflm[];
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR || FEATURE_DS_IS2000 */

#if defined(FEATURE_DS_IS2000) && defined(FEATURE_IS2000_R_SCH)
extern dflm_type ds_atcop_qcscrm_dflm[];
#endif /* FEATURE_DS_IS2000 && FEATURE_IS2000_R_SCH */
#if defined(FEATURE_DS_IS2000) && defined(FEATURE_IS2000_R_SCH)
extern dflm_type ds_atcop_qctrtl_dflm[];
#endif /* FEATURE_DS_IS2000 && FEATURE_IS2000_R_SCH */

#ifdef FEATURE_DS_QNC
extern dflm_type ds_atcop_qcqnc_dflm[];
#endif

#ifdef FEATURE_DS_CHOOSE_SO
extern dflm_type ds_atcop_qcso_dflm[];
#endif


/*---------------------------------------------------------------------------
    Parse table of regular AT command and table size.  
---------------------------------------------------------------------------*/
extern const ds_atcop_cmd_type ds_atcop_pls_table [];     
extern const unsigned int      ds_atcop_sizeof_pls_table; 
							    

/*---------------------------------------------------------------------------
    Parse table of s-registers command and table size. 
---------------------------------------------------------------------------*/
extern const ds_atcop_cmd_type ds_atcop_sreg_table [];   
extern const unsigned int      ds_atcop_sizeof_sreg_table;

/*---------------------------------------------------------------------------
    Parse table of simple single letter commands and table size. 
---------------------------------------------------------------------------*/
extern const ds_atcop_cmd_type ds_atcop_simp_table [];
extern const unsigned int      ds_atcop_sizeof_simp_table;

/* Size of the function value. */
extern const unsigned int      ds_atcop_sizeof_fns_val; 

extern byte ds_atcop_line_buf[MAX_LINE_SIZE];  /*  buffer for AT cmd line  */ 

extern boolean ds_atcop_fdt_seen;




/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION DS_ATCOP

DESCRIPTION
  This function parses the command line, which may contain multiple commands,
  and executes the commands. Depending on the command, this may mean storing
  or reading parameters, setting up a call.

  When called in DS_ATCOP_CMD mode, it returns result and error codes on the
  serial port via DSRMIO_SETUP_DS_TX.

  In the DS_ATCOP_ONLINE mode, result and error codes are encapsulated
  per the   cellular extensions to EIA-617 and returned to the base
  station via dsm_enqueue.

  If an unrecogized command is encountered in the processing of the command
  line, and if base station processing of unrecognized commands is enabled
  by the AT+CXT parameter, the transport layer will be established, and the
  configuration and the entire command line will be transmitted to the base
  station.

  Successful processing of an ATD or ATA command shall also cause the
  transport layer to be established, and the configuration and the entire
  command line to be transmitted to the base station.

  The configuration is transmitted by calling ds_atcop_send_config.

  Single letter commands are processed immediately.  Configuration parameter
  commands are processed by calling the appropriate "helper" function.

DEPENDENCIES
  Describe here any dependencies that this function may have. Describe any 
  initial actions that are assumed to have taken place prior to calling this
  function.  This includes any regional variables required or modified by
  this function.

RETURN VALUE
  None.
  does not return a value, then state "None".

SIDE EFFECTS
  Modifies the modem configuration data base.
  May cause ds_srvc_mode to change to DS_RAWDATA_SRVC_MODE. (on-line)
  May establish the transport layer.

===========================================================================*/
void ds_atcop
(
  ds_atcop_mode_enum_type mode,
  byte *buffer
) ;


/*===========================================================================

FUNCTION DS_ATCOP_FMT_RESPONSE

DESCRIPTION
  This function formats the response according whether the response
  is to the user (Rm), the base station (Um), and whether or not
  the mode is verbose.

  If the response is a NULL ptr, then the result code is formatted
  and displayed according to the same rules.

  If the mode is DS_ATCOP_ONLINE, appropriate EIA-617 messages are
  generated.  The text string portion of the message will be
  empty if ATQ is 1.

DEPENDENCIES
  The numeric values for 'ds_atcop_result_enum_type' are important!
  See dsatcop.h

RETURN VALUE
  Length of the response in characters.

SIDE EFFECTS


===========================================================================*/
void ds_atcop_fmt_response
(
  const byte *in_ptr,
  const ds_atcop_result_enum_type result,
  dsm_item_type ** out_ptr,   
  const ds_atcop_mode_enum_type mode
) ;

/*===========================================================================

FUNCTION DS_ATCOP_IS_STATUS

DESCRIPTION
  This function returns TRUE if the argument string begins with
  the numeric character (1-8)  or the word corresponding to the
  result specified by the second arg.  Trailing characters are ignored.
  Other wise if returns FALSE.

DEPENDENCIES
  None

RETURN VALUE
  See description.

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_atcop_is_status
(
  byte *a_ptr,
  ds_atcop_result_enum_type result
) ;
     

#endif /* FEATURE_DS */
#endif /* DSATCOPI_H */

