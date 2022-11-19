#ifndef CMDBG_H
#define CMDBG_H
/*===========================================================================

     C A L L   M A N A G E R   D E B U G   H E A D E R   F I L E

DESCRIPTION
  This header file contains debug macros and definitions necessary to
  interface with CMDBG.C


Copyright (c) 1991 - 1999 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/cmdbg.h_v   1.1   08 Feb 2001 09:09:18   rotemc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/17/00   RC      Added support for System Determination 2.0
08/31/99   SH      Corrected some comments
06/21/99   SH      Enabled CM_DEBUG
01/07/99   RC      Changes per ARM compiler.
12/30/98   RC      Changes per 4th week of I&T.
12/08/98   RC      Changes per second week of I&T.
11/24/98   RC      Initial release.



===========================================================================*/

#include "comdef.h"    /* Definition for basic types and macros */
#include "target.h"    /* Target configuration file */
#include "cm.h"        /* External interface to cm.c */
#include "err.h"       /* Interface to error log services */
#include "msg.h"       /* Interface to F3 messages services */


/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Compile CM with debug information, asserts and messages.
*/
//#define CM_DEBUG


/* Compile CM with test-register-client.
*/
//#define CM_TEST_REG_CLIENT


/* Compile CM with test-commanding-client.
*/
//#define CM_TEST_CMD_CLIENT


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Debug/PC independent macros.
*/

#define CM_MSG_LOW( str, a, b, c )       MSG_LOW( "=CM= " str, a, b, c )
#define CM_MSG_MED( str, a, b, c )       MSG_MED( "=CM= " str, a, b, c )
#define CM_MSG_HIGH( str, a, b, c )      MSG_HIGH( "=CM= " str, a, b, c )
#define CM_ERR_FATAL( str, a, b, c )     ERR_FATAL( "=CM= " str, a, b, c )

#define CM_FUNC_NOT_IMP( func ) \
    CM_MSG_HIGH( #func " NOT Implemented!",0,0,0 )

#define CM_CASE_NOT_IMP( _case ) \
    CM_ERR( #_case " case NOT Implemented",0,0,0 )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Debug dependent macros.
*/

#ifdef CM_DEBUG
#error code not present
#else
  
  #define CM_ERR( str,a,b,c )            ERR( "=CM= " str,a,b,c )
  #define CM_MSG_ERROR( str,a,b,c )      MSG_ERROR( "=CM= " str,a,b,c )
  #define CM_DBG_ERR( str,a,b,c )
  #define CM_CASE_ERR( str,a,b,c )       ERR_FATAL( "=CM= " str,a,b,c )
  #define CM_SWITCH_ERR( str,a,b,c )     ERR_FATAL( "=CM= " str,a,b,c )
  #define CM_ELSE_ERR( str ) \
               else { MSG_MED( "=CM= " str " else make no sense",0,0,0 ); }

#endif



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* PC dependent macros.
*/

#if (TG==T_PC)
#error code not present
#else  
  
  #define CM_FUNC_START( str,a,b,c )
  #define CM_FUNC_END( str,a,b,c )

#endif


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



/* Debug and PC dependent macros.
*/

#ifdef CM_DEBUG
#error code not present
#else  

  #define CM_DBG_ERR_FATAL( str,a,b,c )
  
#endif


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Asserts and verifications macros.
*/

#ifdef CM_DEBUG
#error code not present
#else
  #define CM_ASSERT( exp )
#endif


#ifdef CM_DEBUG
#error code not present
#else
  #define CM_ASSERT_ERR( exp )                         \
    if( !(exp) )                                       \
    {                                                  \
      ERR( "=CM= ASSERTION FAILED! " #exp ,0,0,0);      \
    }
#endif


#define CM_NONE_DBG_ASSERT( exp )                      \
    if( !(exp) )                                       \
    {                                                  \
      ERR_FATAL( "ASSERTION FAILED! " #exp ,0,0,0 );    \
    }


#ifdef CM_DEBUG
#error code not present
#else
  #define CM_VERIFY( exp )                             \
    if( !(exp) )                                       \
    {                                                  \
      return FALSE;                                    \
    }
#endif


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enum print macros.
*/

//#ifdef CM_DEBUG
//  #define CMDBG_PRT_FUNC( prt_func, enum_val ) prt_func( enum_val ) 
//#else    
  #define CMDBG_PRT_FUNC( prt_func, enum_val )
//#endif    


extern void cm_call_state_prt( dword enum_val );
#define CM_CALL_STATE_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_call_state_prt, enum_val )
    

extern void cm_call_type_prt( dword  enum_val );
#define CM_CALL_TYPE_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_call_type_prt, enum_val )

    
extern void cm_call_end_prt( cm_call_end_e_type  enum_val );
#define CM_CALL_END_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_call_end_prt, enum_val )

    
extern void cm_digit_mode_prt( dword enum_val );
#define CM_DIGIT_MODE_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_digit_mode_prt, enum_val )

    
extern void cm_call_cmd_prt( dword enum_val );
#define CM_CALL_CMD_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_call_cmd_prt, enum_val )

    
extern void cm_call_event_prt( dword enum_val );
#define CM_CALL_EVENT_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_call_event_prt, enum_val )

    
extern void cm_call_cmd_err_prt( dword enum_val );
#define CM_CALL_CMD_ERR_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_call_cmd_err_prt, enum_val )

    
extern void cm_srv_state_prt( dword enum_val );
#define CM_SRV_STATE_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_srv_state_prt, enum_val )

    
extern void cm_oprt_mode_prt( dword enum_val );
#define CM_OPRT_MODE_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_oprt_mode_prt, enum_val )

    
extern void cm_mode_pref_prt( dword enum_val );
#define CM_MODE_PREF_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_mode_pref_prt, enum_val )

    
extern void cm_mode_term_prt( dword enum_val );
#define CM_MODE_TERM_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_mode_term_prt, enum_val )

    
extern void cm_sys_pref_prt( dword enum_val );
#define CM_SYS_PREF_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_sys_pref_prt, enum_val )

    
extern void cm_band_pref_prt( dword enum_val );
#define CM_BAND_PREF_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_band_pref_prt, enum_val )

extern void cm_roam_pref_prt( dword enum_val );
#define CM_ROAM_PREF_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_roam_pref_prt, enum_val )

extern void cm_answer_voice_prt( dword enum_val );
#define CM_ANSWER_VOICE_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_answer_voice_prt, enum_val )

    
extern void cm_nam_prt( dword enum_val );
#define CM_NAM_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_nam_prt, enum_val )

    
extern void cm_cdma_lock_mode_prt( dword enum_val );
#define CM_CDMA_LOCK_MODE_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_cdma_lock_mode_prt, enum_val )

    
extern void cm_ph_cmd_prt( dword enum_val );
#define CM_PH_CMD_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_ph_cmd_prt, enum_val )

    
extern void cm_ph_event_prt( dword enum_val );
#define CM_PH_EVENT_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_ph_event_prt, enum_val )

    
extern void cm_ph_cmd_err_prt( dword enum_val );
#define CM_PH_CMD_ERR_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_ph_cmd_err_prt, enum_val )

    
extern void cm_in_use_prt( dword enum_val );
#define CM_IN_USE_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_in_use_prt, enum_val )

    
extern void cm_client_id_prt( cm_client_id_type  client );
#define CM_CLIENT_ID_PRT( client )  \
    CMDBG_PRT_FUNC( cm_client_id_prt, client )


extern void cm_ui_cmd_prt( dword enum_val );
#define CM_UI_CMD_PRT( enum_val )  \
    CMDBG_PRT_FUNC( cm_ui_cmd_prt, enum_val )


extern void  mc_cmd_prt( dword enum_val );
#define CM_MC_CMD_PRT( enum_val )  \
    CMDBG_PRT_FUNC( mc_cmd_prt, enum_val )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to create a bit array of arbitrary size. The first argument
** (bit_arr) is the name of the array. The second argument (bit_size)
** is the bit size of the array.
*/
#define BIT_ARR_T( bit_arr, bit_size )             \
    byte _##bit_arr[1 + (bit_size)/sizeof(byte)];  \
    bit_arr_s_type bit_arr = { _##bit_arr, bit_size }


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


extern void cmdbg_init( void );
extern void cmdbg_call_info_print(const cm_call_info_s_type  *call_info_ptr);
extern void cmdbg_ph_info_print( const cm_ph_info_s_type  *ph_info_ptr );


#endif /* CMDBG_H */





