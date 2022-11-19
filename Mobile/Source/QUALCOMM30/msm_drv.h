#ifndef MSM_DRV_H
#define MSM_DRV_H
/*===========================================================================

              M S M   R E G I S T E R   A C C E S S   M A C R O S

                            H E A D E R    F I L E

DESCRIPTION
  This file contains macros to support interaction with the QUALCOMM
  MSM3000 to MSM5000 Register Set.
  
  The Macros appear in approximately the same order as the register
  descriptions in the MSM3000 User's Manual 93-24207-1 X1

Copyright (c) 1999,2000,2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/



/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/msm_drv.h_v   1.14   07 Mar 2001 18:33:04   evanmell  $



when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/07/01   ejv     Added clear/set of LOCK_RSSI_EXT_RANGE for chip/symbol macros.
03/01/01   rm      Check for DIP Switch Setting before enable RPC.
02/16/01   dgy     Modified macro COMBINER_SELECT_CH0_512_CHIP_SYMBOL to
                   support CCI decoding.
01/31/01   rm      Added/fixed macros for finger merge detect in hw.
01/22/01   ejv     Added QPCH_SET_ION_QON for MSM5100.
01/18/01   ejv/rm  Added new macros for MSM5100 support.
12/14/00   ejv     Fixed shadowing for QOF enable/disable macros.
12/13/00   hrk     Removed #ifdef FEATURE_GPSONE around Macro CAGC_WRITE_8P2.
10/31/00   rm      Enable Hardware duty cycle fix for MSM5105.
10/25/00   rm      Initialization's for MSM5105. Defined New Macros.
09/21/00   day     Backed out changes to I and Q OFFSET macros to correct 
                   argument list error.
09/10/00   ejv     Added FINGER_FCH_QOF_ENABLE, FINGER_FCH_QOF_DISABLE,
                   FINGER_SCH_QOF_ENABLE, and FINGER_SCH_QOF_DISABLE.
07/31/00   ejv     Modified TRK_LO_ADJ_SET_OVERRIDE.
07/25/00   day     Code cleanup on I offset and Q offset macros.
07/18/00   eh      Added macro to initialize viterbi decoder for MSM5000.
07/13/00   yll/day Added support for "Tune Away Without Drop" for FEATURE_GPSONE -  
                   Fixed I_OFFSET_GET_ACCUMULATOR and Q_OFFSET_GET_ACCUMULATOR macros.
                   Added macro FING_COMB_OFFSET_READ.
06/29/00   abh/day Added MSM3300 support - FEATURE_GPSONE.  Changed SRCH2_CLK_ENA 
                   to enable only Srch2 Block(shadowed writes).
06/12/00   ejv     Added DEMOD_ENABLE/DISABLE_FPC and FINGER_SET_TTL_FAST/SLOW.
06/06/00   bgc     Added revision check for DEMOD_ENABLE_MAC_ENGINE_FOR_SCH.
03/17/00   ajn     Sleep init now assumes DUTY_CYCLE fix for PPanther.
03/02/00   ajn     Added "slow clock" Frequency Error Estimator (FEE) support
02/24/00    eh     Defined SEARCH_ENABLE_DEMOD_BYPASS and
                   SEARCH_DISABLE_DEMOD_BYPASS.  
01/27/00   ejv     Define SRCH2_CLK_ENA for MSM3100 only.
01/14/00   ejv     Added macros for adjusting the AGC loop gain constant.
01/13/00   ejv     Added macros for uP int. bypass and sleep controller read.
12/13/99   ejv     Added comments and masks for some macros.
12/06/99   ajn     Added Decoder Zero-Initialize modes
11/29/99   ejv     Mask the bottom 2 bits in SRCH_GET_STATUS_DUMP.
11/10/99   ejv     Added FEATURE_SEARCH2 macros.
10/27/99   ajn     Added QPCH related macros.
10/21/99   rm      Enable DSP for CH2 SCCH demodulation.
10/20/99   dgy     Added new macro to configure QOF.
09/24/99   rm      Added Macros to Configure MAC ENGINE.
09/13/99   rm      Added new macros to control ULPN descramble for RC4.
08/11/99   rm      Added new macros to enable/disable I or Q channel from the 
                   Demodulator.
08/11/99   rm      Added new macros for Symbol combiner symbol accumulation.
08/06/99   ejv     Merged jcw's initial Tiger2 support.
07/23/99   ajn     First cut of file.

===========================================================================*/

#include "dmod.h"
#include "msm_help.h"
#include "cagc.h"
#include "hw.h"
#include "bio.h"


/*===========================================================================

                        DEMODULATION REGISTERS
                        
===========================================================================*/



/*===========================================================================

MACRO DEMODULATOR_RESET

DESCRIPTION     This macro resets the demodulator circuitry.

DEPENDENCIES    CHIPX8 must be oscillating.

RETURN VALUE    None.
      
SIDE EFFECTS    Too numerous to mention.

===========================================================================*/

#define DEMODULATOR_RESET( )    \
    MSM_WB( DEMOD_RESET, 0x00 )



#ifdef T_MSM5000
/*=========================================================================
MACRO DEMOD_ENABLE_MAC_ENGINE

DESCRIPTION    This macro enables the MAC engine for CH2 supplemental channel 
               demodulation.

DEPENDENCIES   These macros must be called from inside the context of a 
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE   None

SIDE EFFECTS   None
==========================================================================*/
#ifdef T_MSM5000A_SUPPORT

/* The trial version of MSM5000 (called MSM5000A) requires that DSP_SUP_OFF 
 * be cleared in order to work correctly.  Later revisions of MSM5000 interpret
 * this bit as defined.
 */

#define DEMOD_ENABLE_MAC_ENGINE_FOR_SCH( ) \
    { \
      if ( hw_version() == MSM_50_A0 ) { \
        MSM_WB_CLEAR( DEM_CTL, DSP_SUP_OFF ); \
      } else { \
        MSM_WB_SET( DEM_CTL, DSP_SUP_OFF ); \
      } \
      MSM_WB_SET( DEM_CTL, MAC_EN ); \
    }

#else

#define DEMOD_ENABLE_MAC_ENGINE_FOR_SCH( ) \
      MSM_WB_SET( DEM_CTL, DSP_SUP_OFF ); \
      MSM_WB_SET( DEM_CTL, MAC_EN )

#endif   
/*=========================================================================
MACRO DEMOD_DISABLE_MAC_ENGINE

DESCRIPTION    This macro disables the MAC engine.

DEPENDENCIES   These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE   None

SIDE EFFECTS   None
==========================================================================*/

#define DEMOD_DISABLE_MAC_ENGINE( ) \
      MSM_WB_CLEAR( DEM_CTL, MAC_EN ),\
      MSM_WB_CLEAR( DEM_CTL, DSP_SUP_OFF );
      
#ifdef T_MSM_5100
#error code not present
#endif /* T_MSM_5100 */

/*=========================================================================
MACRO DEMOD_SET_MAC_RND_128_64_CHIPS

DESCRIPTION  This macro sets MAC Engine rounding factors for spreading
              rate of 128 or 64 chips.

RETURN VALUE  None

SIDE EFFECTS  None
==========================================================================*/

#define DEMOD_SET_MAC_RND_128_64_CHIPS(decoder)\
        MSM_WH_MASKED( MAC_RND, COMB_RND1, DMOD_COMB_RND1_128_64_CHIPS_V ), \
        MSM_WH_MASKED( MAC_RND, COMB_RND2, DMOD_COMB_RND2_128_64_CHIPS_V ), \
        MSM_WH_MASKED( MAC_RND, COMB_RND3, decoder )

/*=========================================================================
MACRO DEMOD_SET_MAC_RND_32_16_CHIPS

DESCRIPTION  This macro sets MAC Engine rounding factors for spreading
              rate of 32 or 16 chips.

RETURN VALUE  None

SIDE EFFECTS  None
==========================================================================*/

#define DEMOD_SET_MAC_RND_32_16_CHIPS(decoder ) \
        MSM_WH_MASKED( MAC_RND, COMB_RND1, DMOD_COMB_RND1_32_16_CHIPS_V ), \
        MSM_WH_MASKED( MAC_RND, COMB_RND2, DMOD_COMB_RND2_32_16_CHIPS_V ), \
        MSM_WH_MASKED( MAC_RND, COMB_RND3, decoder )

/*=========================================================================
MACRO DEMOD_SET_MAC_RND_8_4_CHIPS

DESCRIPTION  This macro sets MAC Engine rounding factors for spreading
              rate of 8 or 4 chips.

RETURN VALUE  None

SIDE EFFECTS  None
==========================================================================*/

#define DEMOD_SET_MAC_RND_8_4_CHIPS(decoder ) \
        MSM_WH_MASKED( MAC_RND, COMB_RND1, DMOD_COMB_RND1_8_4_CHIPS_V ), \
        MSM_WH_MASKED( MAC_RND, COMB_RND2, DMOD_COMB_RND2_8_4_CHIPS_V ), \
        MSM_WH_MASKED( MAC_RND, COMB_RND3, decoder )

/*===================================================================== 
MACRO DEMOD_SET_CH2_SPR

DESCRIPTION This macro controls the spreading rate of the demodulator 
            channel 2.

RETURN VALUE None

SIDE EFFECTS None

======================================================================*/

#define DEMOD_SET_CH2_SPR(spr)\
       MSM_WB( DEM_CH2_SPR, spr )

/*=====================================================================
MACRO DEMOD_SET_SW_BETA_FCH

DESCRIPTION  This macro sets the FCH frame scaling factor for demod soft 
             decison symbols to the SVD, for Channel 0.

RETURN VALUE  None

SIDE EFFECTS  This scaling factor takes effect at the next frame boundary.
=========================================================================*/

#define DEMOD_SET_SW_BETA_FCH(scale) \
       MSM_WB( SW_BETA_FCH, scale )

#ifdef T_MSM_5100
#error code not present
#endif /* T_MSM_5100 */
     
/*=====================================================================
MACRO DEMOD_SET_SW_BETA_SCH

DESCRIPTION  This macro sets the SCH frame scaling factor for demod soft 
             decison symbols to the SVD.

RETURN VALUE  None

SIDE EFFECTS  This scaling factor takes effect at the next frame boundary.
=========================================================================*/

#define DEMOD_SET_SW_BETA_SCH(scale) \
       MSM_WB( SW_BETA_SCH, scale )
     
/*==========================================================================
MACRO DEMOD_ENABLE_I_CHANNEL

DESCRIPTION    This macro sets the demodulator to send I channel soft decisions 
               to the Viterbi Decoder.  
         
DEPENDECIES    These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.


RETURN VALUE   None.

SIDE EFFECTS   None.

=============================================================================*/

#define DEMOD_ENABLE_I_CHANNEL( ) \
        MSM_WB_CLEAR( DEM_CTL, I_CHAN_OFF )
  
/*==========================================================================
MACRO DEMOD_ENABLE_Q_CHANNEL

DESCRIPTION    This macro sets the demodulator to send Q channel soft decisions 
               to the Viterbi Decoder. Enable Q Channel when receiving RC3, RC4
               and RC5 Traffic Channel Frames.  
         
DEPENDECIES    These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.


RETURN VALUE   None

SIDE EFFECTS   None.

=============================================================================*/

#define DEMOD_ENABLE_Q_CHANNEL( ) \
        MSM_WB_CLEAR( DEM_CTL, Q_CHAN_OFF )
       
/*==========================================================================
MACRO DEMOD_DISABLE_I_CHANNEL

DESCRIPTION    This macro stops the demodulator from sending I channel soft decisions 
               to the Viterbi Decoder. Disable I channel when receiving QPCH.
                        
DEPENDECIES    These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.


RETURN VALUE   None

SIDE EFFECTS   None.

=============================================================================*/

#define DEMOD_DISABLE_I_CHANNEL( ) \
        MSM_WB_SET( DEM_CTL, I_CHAN_OFF )

/*==========================================================================
MACRO DEMOD_DISABLE_Q_CHANNEL

DESCRIPTION    This macro stops the demodulator from sending Q channel soft decisions 
               to the Viterbi Decoder. Disable Q channel when receiving Sync, Paging,
               RC1 & RC2 traffic channel frames
                        
DEPENDECIES    These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.


RETURN VALUE   None

SIDE EFFECTS   None.

=============================================================================*/

#define DEMOD_DISABLE_Q_CHANNEL( ) \
        MSM_WB_SET( DEM_CTL, Q_CHAN_OFF )

/*==========================================================================
MACRO DEMOD_ENABLE_FPC
MACRO DEMOD_DISABLE_FPC

DESCRIPTION    These macros are used for enabling/disabling forward power
               control.

DEPENDECIES    These macros must be called from inside the context of a
               MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.


RETURN VALUE   None

SIDE EFFECTS   None.

=============================================================================*/

#define DEMOD_ENABLE_FPC( ) \
        MSM_WH_SET( FPC_CTL, FPC_EN )

#define DEMOD_DISABLE_FPC( ) \
        MSM_WH_CLEAR( FPC_CTL, FPC_EN )

#endif  /* T_MSM5000 */


/*===========================================================================

MACRO SEARCH_ENABLE_MICRO_BYPASS
MACRO SEARCH_DISABLE_MICRO_BYPASS

DESCRIPTION     These macros enable or disable the uP interface bypass.
                
DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_ENABLE_MICRO_BYPASS( ) \
    MSM_WH_SET( MSM_CLK_CTL1, MICRO_INTF_BYPASS );

#define SEARCH_DISABLE_MICRO_BYPASS( ) \
    MSM_WH_CLEAR( MSM_CLK_CTL1, MICRO_INTF_BYPASS );


#ifdef T_MSM5000
/*===========================================================================

MACRO SEARCH_ENABLE_DEMOD_BYPASS
MACRO SEARCH_DISABLE_DEMOD_BYPASS

DESCRIPTION     These macros enable and disable the demod uP interface bypass.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_ENABLE_DEMOD_BYPASS( ) \
    MSM_WH_SET( MSM_CLK_CTL1, RXCX8_MICRO_BYPASS )

#define SEARCH_DISABLE_DEMOD_BYPASS( ) \
    MSM_WH_CLEAR( MSM_CLK_CTL1, RXCX8_MICRO_BYPASS )
#endif /* T_MSM5000 */

/*===========================================================================

MACRO SEARCH_GET_SLEEP_CONTROLLER

DESCRIPTION     This macros reads the raw value from the sleep controller.
                
DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_GET_SLEEP_CONTROLLER( wtime ) \
    MSM_R2H( SLEEP_COUNT, LSHW, MSHW, wtime );


/*===========================================================================

                  DEMODULATOR SEARCH ENGINE REGISTERS
                        
===========================================================================*/

#ifdef FEATURE_SEARCH2

#if defined (T_MSM31) && !defined(T_MSM5105)
#error code not present
#endif /* T_MSM31 */

/*===========================================================================

MACRO SRCH2_8X_STATUS_DUMP_ENA
      SRCH2_8X_STATUS_DUMP_DIS

DESCRIPTION     This macros enables/disables the 8x searcher from causing
      status dumps.

DEPENDENCIES    None.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_8X_STATUS_DUMP_ENA( ) \
    MSM_WB_SET( SEARCH_STATUS_DUMP_CTL, SEARCH1_DUMP_EN );

#define SRCH2_8X_STATUS_DUMP_DIS( ) \
    MSM_WB_CLEAR( SEARCH_STATUS_DUMP_CTL, SEARCH1_DUMP_EN );

#else    

#define SRCH2_8X_STATUS_DUMP_ENA( ) \
    MSM_WB_SET( SRCH_STATUS_DUMP_CTL, SEARCH1_DUMP_EN );

#define SRCH2_8X_STATUS_DUMP_DIS( ) \
    MSM_WB_CLEAR( SRCH_STATUS_DUMP_CTL, SEARCH1_DUMP_EN );

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_16X_STATUS_DUMP_ENA
      SRCH2_16X_STATUS_DUMP_DIS

DESCRIPTION     This macro enables/disables the 16x searcher from causing
      status dumps.

DEPENDENCIES    None.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_16X_STATUS_DUMP_ENA( ) \
    MSM_WB_SET( SEARCH_STATUS_DUMP_CTL, SEARCH2_DUMP_EN );


#define SRCH2_16X_STATUS_DUMP_DIS( ) \
    MSM_WB_CLEAR( SEARCH_STATUS_DUMP_CTL, SEARCH2_DUMP_EN );

#else    

#define SRCH2_16X_STATUS_DUMP_ENA( ) \
    MSM_WB_SET( SRCH_STATUS_DUMP_CTL, SEARCH2_DUMP_EN );


#define SRCH2_16X_STATUS_DUMP_DIS( ) \
    MSM_WB_CLEAR( SRCH_STATUS_DUMP_CTL, SEARCH2_DUMP_EN );

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_RETURN_4_PEAKS

DESCRIPTION     This macro clears this bit for normal operating mode.

DEPENDENCIES    None.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_RETURN_4_PEAKS( ) \
    MSM_WB_CLEAR( SEARCH2_CTL, ALL_ENERGIES );

#else

#define SRCH2_RETURN_4_PEAKS( ) \
    MSM_WB_CLEAR( SRCH2_CTL, ALL_ENERGIES );

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH_GET_STATUS_DUMP

DESCRIPTION     This macro reads the status dump register in the search
      hardware, which signals which searcher has generated a DUMP.  Both
      bits may be set at the same time.

DEPENDENCIES    None.

RETURN VALUE    2 lsbs bits representing which searcher/s have requested
      a status DUMP.
      
      Bits  --  Which searcher requested a status DUMP?
      ----------------------------------------------------
       01   --    8x searcher.
       10   --    16x searcher
       11   --    Both searchers
      
SIDE EFFECTS    The status register is cleared after being read.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH_GET_STATUS_DUMP( ) \
    ( MSM_RB( SEARCH_STATUS_DUMP_STATUS ) & SEARCH_STATUS_DUMP_STATUS_RB_MASK )
#else

#define SRCH_GET_STATUS_DUMP( ) \
    ( MSM_RH( SRCH_STATUS_DUMP_STATUS ) & SRCH_STATUS_DUMP_STATUS_RH_MASK )
#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_TRIGGER_DUMP

DESCRIPTION     This macro causes a status dump request from the 16x searcher.

DEPENDENCIES    None.

RETURN VALUE    None.
      
SIDE EFFECTS    Results in a search done interrupt if the status dump is
      enabled for the 16x searcher.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_TRIGGER_DUMP( ) \
    MSM_WB_STROBE_HIGH( SEARCH2_CTL, STATUS_DUMP )

#else

#define SRCH2_TRIGGER_DUMP( ) \
    MSM_WB_STROBE_HIGH( SRCH2_CTL, STATUS_DUMP )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_GET_POSITION

DESCRIPTION     This macro reads the position of the 16x searcher.

DEPENDENCIES    None.

RETURN VALUE    The absolute position of the 16x searcher.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_GET_POSITION( position ) \
    W_PTR(position)[ 0 ] = MSM_RH( SEARCH2_POSITION_HIGH ), \
    position <<= 16, \
    W_PTR(position)[ 0 ] |= (MSM_RH( SEARCH2_POSITION_LOW ) \
                             & SEARCH2_POSITION_LOW_RH_MASK)
  /* The 2 msbs are read first, then shifted up by 16 to make room for the
     lower 16 lsbs on the subsequent read */

#else

#define SRCH2_GET_POSITION( position ) \
    W_PTR(position)[ 0 ] = MSM_RH( SRCH2_POSITION_COARSE ), \
    position <<= 3, \
    W_PTR(position)[ 0 ] |= (MSM_RH( SRCH2_POSITION_FINE ) \
                             & SRCH2_POSITION_FINE_RH_MASK)
  /* The 15 msbs are read first, then shifted up by 3 to make room for the
     lower 3 lsbs on the subsequent read */

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_ABORT

DESCRIPTION     This macro performs an abort of the 16x searcher.

DEPENDENCIES    None.

RETURN VALUE    None.
      
SIDE EFFECTS    Causes a status dump which will then cause a search done
      interrupt.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_ABORT( ) \
    MSM_WB_STROBE_HIGH( SEARCH2_CTL, SEARCH_ABORT )

#else

#define SRCH2_ABORT( ) \
    MSM_WB_STROBE_HIGH( SRCH2_CTL, SEARCH_ABORT )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_SET_SEARCH_PARAM

DESCRIPTION     This macro programs one of the search parameters into the
      16x h/w queue.

DEPENDENCIES    None.

RETURN VALUE    None.
      
SIDE EFFECTS    A search will begin after every 6 consecutive writes.  The
      queue write index will also increment after every 6 writes, while
      the queue write sub-index will increment after every write.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_SET_SEARCH_PARAM( parameter ) \
    MSM_WH ( SEARCH2_QUEUE, parameter )

#else

#define SRCH2_SET_SEARCH_PARAM( parameter ) \
    MSM_WH ( SRCH2_QUEUE, parameter )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_GET_SEARCH_RES

DESCRIPTION     This macro reads the results from the 16x searcher register.

DEPENDENCIES    None.

RETURN VALUE    None.
      
SIDE EFFECTS    The results read index will increment after every 8 reads.
      Also, the results read sub-index will increment after every read.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_GET_SEARCH_RES( val ) \
    ( val = MSM_RH ( SEARCH2_RESULTS ) )

#else

#define SRCH2_GET_SEARCH_RES( val ) \
    ( val =  MSM_RH ( SRCH2_RESULTS ) )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_GET_Q_READ_PTR

DESCRIPTION    This macro reads the index indicating which task the 16x
      searcher will read and execute next.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   4 bits representing the index of the queue read pointer,
      from 0000 to 0111.

SIDE EFFECTS   None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_GET_Q_READ_PTR( ) \
    ( MSM_RH_MASKED( SEARCH2_QUEUE_PTRS, QUEUE_READ_PTR ) >> 4 )

#else

#define SRCH2_GET_Q_READ_PTR( ) \
    ( MSM_RH_MASKED( SRCH2_QUEUE_PTRS, QUEUE_READ_PTR ) >> 4 )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_GET_Q_READ_SUB_PTR

DESCRIPTION    This macro reads the sub-index indicating which paramter of
      the current task the 16x searcher will read next.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   4 bits representing the sub-index of the queue read pointer,
      from 0000 to 0101.

SIDE EFFECTS   None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_GET_Q_READ_SUB_PTR( ) \
    ( MSM_RH_MASKED( SEARCH2_QUEUE_PTRS, QUEUE_READ_SUB_PTR ) >> 0 )

#else

#define SRCH2_GET_Q_READ_SUB_PTR( ) \
    ( MSM_RH_MASKED( SRCH2_QUEUE_PTRS, QUEUE_READ_SUB_PTR ) >> 0 )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_GET_RES_READ_PTR

DESCRIPTION    This macro reads the index indicating which search results
      the software will read from on the next results read.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   4 bits representing the index of the results read pointer,
      from 0000 to 0111.

SIDE EFFECTS   None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_GET_RES_READ_PTR( ) \
    ( MSM_RH_MASKED( SEARCH2_RESULTS_PTRS, RESULTS_READ_PTR ) >> 4 )

#else

#define SRCH2_GET_RES_READ_PTR( ) \
    ( MSM_RH_MASKED( SRCH2_RESULTS_PTRS, RESULTS_READ_PTR ) >> 4 )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_GET_Q_WRITE_PTR

DESCRIPTION    This macro reads the index indicating where the next search
      task will be written to on the 16x hardware queue.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   4 bits representing the index of the queue write pointer,
      from 0000 to 0111.

SIDE EFFECTS   None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_GET_Q_WRITE_PTR( ) \
    ( MSM_RH_MASKED( SEARCH2_QUEUE_PTRS, QUEUE_WRITE_PTR ) >> 12 )

#else

#define SRCH2_GET_Q_WRITE_PTR( ) \
    ( MSM_RH_MASKED( SRCH2_QUEUE_PTRS, QUEUE_WRITE_PTR ) >> 12 )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_GET_RES_WRITE_PTR

DESCRIPTION    This macro reads the index indicating where on the 16x
      hardware queue the next set of search results will be written.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   4 bits representing the index of the queue write pointer,
      from 0000 to 0111.

SIDE EFFECTS   None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_GET_RES_WRITE_PTR( ) \
    ( MSM_RH_MASKED( SEARCH2_RESULTS_PTRS, RESULTS_WRITE_PTR ) >> 12 )

#else

#define SRCH2_GET_RES_WRITE_PTR( ) \
    ( MSM_RH_MASKED( SRCH2_RESULTS_PTRS, RESULTS_WRITE_PTR ) >> 12 )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_GET_RES_WRITE_SUB_PTR

DESCRIPTION    This macro reads the sub-index indicating where on the 16x
      hardware queue the next set of search results will be written.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   4 bits representing the sub-index of the queue write pointer,
      from 0000 to 0111.

SIDE EFFECTS   None.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_GET_RES_WRITE_SUB_PTR( ) \
    ( MSM_RH_MASKED( SEARCH2_RESULTS_PTRS, RESULTS_WRITE_SUB_PTR ) >> 8 )

#else

#define SRCH2_GET_RES_WRITE_SUB_PTR( ) \
    ( MSM_RH_MASKED( SRCH2_RESULTS_PTRS, RESULTS_WRITE_SUB_PTR ) >> 8 )

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_RESET_QUEUE_PTRS

DESCRIPTION    This macro resets the 16x h/w queue ptrs.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   All queue read/write pointers and sub-pointers are reset.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_RESET_QUEUE_PTRS( ) \
    MSM_WB_STROBE_HIGH ( SEARCH2_CTL, RESET_QUEUE_PTRS );

#else

#define SRCH2_RESET_QUEUE_PTRS( ) \
    MSM_WB_STROBE_HIGH ( SRCH2_CTL, RES_QUEUE_PTRS );

#endif /* T_MSM5000 */

/*===========================================================================

MACRO SRCH2_RESET_RESULTS_PTRS

DESCRIPTION    This macro resets the 16x h/w results ptrs.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   All results read/write pointers and sub-pointers are reset.

===========================================================================*/

#ifdef T_MSM5000

#define SRCH2_RESET_RESULTS_PTRS( ) \
    MSM_WB_STROBE_HIGH ( SEARCH2_CTL, RESET_RESULTS_PTRS );

#else

#define SRCH2_RESET_RESULTS_PTRS( ) \
    MSM_WB_STROBE_HIGH ( SRCH2_CTL, RES_RESULTS_PTRS );

#endif /* T_MSM5000 */

#ifdef T_MSM5105
#error code not present
#endif /* T_MSM5105 */

#endif /* FEATURE_SEARCH2 */



/*===========================================================================

MACRO SEARCH_ENABLE_8X
MACRO SEARCH_DISABLE_8X

DESCRIPTION     These macros enable or disable the 8x searcher.
                
DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_ENABLE_8X( ) \
    MSM_WB_SET( SRCH_CTL, SEARCH_SPEED )

#define SEARCH_DISABLE_8X( ) \
    MSM_WB_CLEAR( SRCH_CTL, SEARCH_SPEED )


/*===========================================================================

MACRO SEARCH_ENABLE_DMA
MACRO SEARCH_DISABLE_DMA

DESCRIPTION     These macros enable or disable the searcher DMA engine.
                
DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
                DMA may only be used with the 1x searcher.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_ENABLE_DMA( ) \
    MSM_WB_CLEAR( SRCH_CTL, DMA_DISABLE )

#define SEARCH_DISABLE_DMA( ) \
    MSM_WB_SET( SRCH_CTL, DMA_DISABLE )


/*===========================================================================

MACRO SEARCH_TRIGGER_DUMP

DESCRIPTION     These macros triggers a status dump.
                
DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_TRIGGER_DUMP( ) \
    MSM_WB_PULSE( SRCH_CTL, STATUS_DUMP )


/*===========================================================================

MACRO SEARCH_SET_GAIN

DESCRIPTION     This macro set the gain value for the searcher.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SET_GAIN( gain_value ) \
    MSM_WB_MASKED( SRCH_CTL, SEARCHER_GAIN, (gain_value) )


/*===========================================================================

MACRO SEARCH_SET_EARLY_DUMP

DESCRIPTION     This macro set the searcher's early dump time and energy
                threshold for the 1x searcher.
                
                Time must be between 4 and INTEGRATION_TIME-2

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SET_EARLY_DUMP( thresh, time ) \
    MSM_W2B( SRCH_TH_ENERGY, LOW, HIGH, (thresh) ), \
    MSM_WB( SRCH_TH_TIME, (time) )


/*===========================================================================

MACRO SEARCH_SET_INTEGRATION_LENGTH

DESCRIPTION     This macro set the integration length for the searcher, in
                units of 4-chips

DEPENDENCIES    None.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SET_INTEGRATION_LEN( integration_length ) \
    MSM_WB( SRCH_INTG_TIME, (integration_length) )


/*===========================================================================

MACRO SEARCH_SET_BINARY_OFFSET

DESCRIPTION     This macro set the searcher's binary offset value.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SET_BINARY_OFFSET( bin_offset ) \
    MSM_W2B( SRCH_OFFSET, LOW, HIGH, (bin_offset) )


/*===========================================================================

MACRO SEARCH_SET_IMASK

DESCRIPTION     This macro set the searcher's IMASK.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SET_IMASK( i_mask ) \
    MSM_W2B( SRCH_MASK_I, LOW, HIGH, (i_mask) )


/*===========================================================================

MACRO SEARCH_SET_QMASK

DESCRIPTION     This macro set the searcher's QMASK.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SET_QMASK( q_mask ) \
    MSM_W2B( SRCH_MASK_Q, LOW, HIGH, (q_mask) )


/*===========================================================================

MACRO SEARCH_SLEW

DESCRIPTION     This macro slews the searcher the specified number of chipx8.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    Causes the searcher to start searching after completion of
                the programmed slew.

===========================================================================*/

#define SEARCH_SLEW( slew ) \
    MSM_W3B( SRCH_SLEW, LOW, MID, HIGH, (slew) )


/*===========================================================================

MACRO SEARCH_GET_POSITION

DESCRIPTION     This macro retrieves the searcher position as of the last
                status dump.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                
                The position parameter must be an l-value.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define SEARCH_GET_POSITION( position ) \
    W_PTR(position)[ 0 ] = MSM_RH( SRCH_POSITION_LOW ), \
    B_PTR(position)[ 2 ] = MSM_RB( SRCH_POSITION_HIGH )

#else

#define SEARCH_GET_POSITION( position ) \
    MSM_R3B( SRCH_POSITION, LOW, MID, HIGH, position )

#endif


/*===========================================================================

MACRO SEARCH_SET_NONCOHERENT_PASSES

DESCRIPTION     This macro set the number of non-coherent integration passes.
                Value 0 .. 7 corresponds to 1 .. 8 nc-integration passes.

DEPENDENCIES    None.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SET_NONCOHERENT_PASSES( nci_passes ) \
    MSM_WB( SRCH_ACC_PASS, (nci_passes) )


/*===========================================================================

MACRO SEARCH_SET_WINDOW_SIZE

DESCRIPTION     This macro sets the size of the search window, in half-chip
                units.

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SET_WINDOW_SIZE( num_half_chips ) \
    MSM_W2B( SRCH_NUM, LOW, HIGH, (num_half_chips) )


/*===========================================================================

MACRO SEARCH_SELECT_MAXIMA

DESCRIPTION     This macro selects the 1st (0), 2nd (1), 3rd (2) or 4th (3)
                best search result local maxima.

DEPENDENCIES    This macro, along with the SEARCH_GET_MAXIMA_ENG( ) and
                SEARCH_GET_MAXIMA_POS( ), must be called from inside the
                context of a MSM_LOCK / MSM_UNLOCK pair to ensure proper
                operation.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_SELECT_MAXIMA( index ) \
    MSM_WB( SRCH_MAX_SELECT, (index) )


/*===========================================================================

MACRO SEARCH_GET_MAXIMA_ENG

DESCRIPTION     This macro returns the energy of the selected local maxima.

DEPENDENCIES    This macro, along with the SEARCH_SELECT_MAXIMA( ), must be
                called from inside the context of a MSM_LOCK / MSM_UNLOCK
                pair to ensure proper operation.
                
                The eng parameter must be an l-value.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define SEARCH_GET_MAXIMA_ENG( eng ) \
    eng = MSM_RH( SRCH_MAX_ENERGY )

#else

#define SEARCH_GET_MAXIMA_ENG( eng ) \
    MSM_R2B( SRCH_MAX_ENERGY, LOW, HIGH, eng )

#endif


/*===========================================================================

MACRO SEARCH_GET_MAXIMA_POS

DESCRIPTION     This macro returns the position (in 1/2-chips) of the
                selected local maxima.

DEPENDENCIES    This macro, along with the SEARCH_SELECT_MAXIMA( ), must be
                called from inside the context of a MSM_LOCK / MSM_UNLOCK
                pair to ensure proper operation.
                
                The pos parameter must be an l-value.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define SEARCH_GET_MAXIMA_POS( pos ) \
    pos = MSM_RH( SRCH_MAX_INDEX )

#else

#define SEARCH_GET_MAXIMA_POS( pos ) \
    MSM_R2B( SRCH_MAX_INDEX, LOW, HIGH, pos )

#endif

#ifdef T_MSM_5100
#error code not present
#endif /* T_MSM_5100 */



/*===========================================================================

                        DEMODULATOR FINGER CONTROL
                        
The following macros operate on the currently selected demodulator finger.
The macros must only be used within the context of a MSM_FINGER_SELECT /
MSM_FINGER_DESELECT pair, inside the context of a MSM_LOCK / MSM_UNLOCK
pair.

Eg)
    MSM_LOCK( );
        
    for ( f = 0; f < NUM_FINGERS; f++)
    {
        MSM_FINGER_SELECT( f );
          
        FINGER_SET_IMASK( imask );
        FINGER_SET_QMASK( qmask );
        FINGER_SET_BINARY_OFFSET( offset );
          
        MSM_FINGER_UNSELECT( );
    }
        
    MSM_UNLOCK( );
    

===========================================================================*/


#ifdef T_MSM_5100
#error code not present
#endif /* T_MSM_5100 */

/*===========================================================================

MACRO FINGER_CFG_QOF

DESCRIPTION     This macro configures QOF for the given channel.

DEPENDENCIES    The channel number must be a literal constant.  It may not
                be a variable.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM_5100
#error code not present
#elif defined(T_MSM5000)
    #define FINGER_CFG_QOF( channel, qof_sel ) \
        MSM_WH( Fn_CH##channel##_QOF_SEL, qof_sel )
#endif

/*===========================================================================

MACRO FINGER_FCH_QOF_ENABLE
MACRO FINGER_FCH_QOF_DISABLE

DESCRIPTION     These macros enable/disable QOF for the given channel.

DEPENDENCIES    The channel number must be a literal constant.  It may not
                be a variable.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM_5100
#error code not present
#elif defined( T_MSM5000 )
    #define FINGER_FCH_QOF_ENABLE( ) \
        MSM_FINGER_WB_CLEAR( Fn_OTD_CFG, FCH_QOF )
        
    #define FINGER_FCH_QOF_DISABLE( ) \
        MSM_FINGER_WB_SET( Fn_OTD_CFG, FCH_QOF )
#endif /* T_MSM5000 */

/*===========================================================================

MACRO FINGER_SCH_QOF_ENABLE
MACRO FINGER_SCH_QOF_DISABLE

DESCRIPTION     These macros enable/disable QOF for the given channel.

DEPENDENCIES    The channel number must be a literal constant.  It may not
                be a variable.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM_5100
#error code not present
#elif defined( T_MSM5000 )
    #define FINGER_SCH_QOF_ENABLE( ) \
        MSM_FINGER_WB_CLEAR( Fn_OTD_CFG, SCH_QOF )
        
    #define FINGER_SCH_QOF_DISABLE( ) \
        MSM_FINGER_WB_SET( Fn_OTD_CFG, SCH_QOF )
#endif /* T_MSM5000 */


/*===========================================================================

MACRO FINGER_POWER_CTL_SET_CELL
MACRO FINGER_POWER_CTL_DISABLE

DESCRIPTION     These macros set the currently selected finger to a power
                control cell, or disable power control for the selected
                finger.

DEPENDENCIES    This function must be called from inside the context of a
                MSM_FINGER_SELECT / MSM_FINGER_UNSELECT pair, inside the
                context of a MSM_LOCK / MSM_UNLOCK pair to ensure proper
                operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

/* Note: MSM_WB macro is not possible here due to address calculation */

#define FINGER_POWER_CONTROL_SET_CELL( cell ) \
    MSM_OUT( (FFE_POWER_CTL_F0_CELLN_EN_WB + _dmod_fing*4), \
             (DMOD_PC_F0_CELL_A_V << (cell)) )

#define FINGER_POWER_CONTROL_DISABLE( ) \
    MSM_OUT( (FFE_POWER_CTL_F0_CELLN_EN_WB + _dmod_fing*4), \
             0x00 )


/*===========================================================================

MACRO FINGER_ENABLE
MACRO FINGER_DISABLE

DESCRIPTION     These macros enable and disabled locking for the selected
                finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGER_ENABLE( ) \
    MSM_FINGER_WB_CLEAR( FFE_FINGER_CTL1, FINGER_DISABLE )

#define FINGER_DISABLE( ) \
    MSM_FINGER_WB_SET( FFE_FINGER_CTL1, FINGER_DISABLE )


/*===========================================================================

MACRO FINGER_ENABLE_LOCK
MACRO FINGER_DISABLE_LOCK

DESCRIPTION     These macros enable and disabled locking for the selected
                finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGER_ENABLE_LOCK( ) \
    MSM_FINGER_WB_SET( FFE_FINGER_CTL1, LOCK_EN )

#define FINGER_DISABLE_LOCK( ) \
    MSM_FINGER_WB_CLEAR( FFE_FINGER_CTL1, LOCK_EN )


/*===========================================================================

MACRO FINGER_FORCE_LOCK
MACRO FINGER_CLEAR_FORCE_LOCK

DESCRIPTION     These macros set and clear the FORCE LOCK parameter of the
                selected finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGER_FORCE_LOCK( ) \
    MSM_FINGER_WB_SET( FFE_FINGER_CTL1, LOCK_SET_HYST )

#define FINGER_CLEAR_FORCE_LOCK( ) \
    MSM_FINGER_WB_CLEAR( FFE_FINGER_CTL1, LOCK_SET_HYST )


/*===========================================================================

MACRO FINGER_FORCE_UNLOCK
MACRO FINGER_CLEAR_FORCE_UNLOCK

DESCRIPTION     These macros set and clear the FORCE UNLOCK parameter of the
                selected finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGER_FORCE_UNLOCK( ) \
    MSM_FINGER_WB_SET( FFE_FINGER_CTL1, LOCK_RES_HYST )

#define FINGER_CLEAR_FORCE_UNLOCK( ) \
    MSM_FINGER_WB_CLEAR( FFE_FINGER_CTL1, LOCK_RES_HYST )


/*===========================================================================

MACRO FINGER_PULSE_FORCE_UNLOCK

DESCRIPTION     This macro sets and then clears the FORCE UNLOCK parameter
                of the selected finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGER_PULSE_FORCE_UNLOCK( ) \
    MSM_FINGER_WB_PULSE( FFE_FINGER_CTL1, LOCK_RES_HYST )


/*===========================================================================

MACRO FINGER_ENABLE_TIMETRACK
MACRO FINGER_DISABLE_TIMETRACK

DESCRIPTION     These macros sets and clear the time-tracking parameter
                of the selected finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGER_ENABLE_TIMETRACK( ) \
    MSM_FINGER_WB_SET( FFE_FINGER_CTL2, TT_EN )

#define FINGER_DISABLE_TIMETRACK( ) \
    MSM_FINGER_WB_CLEAR( FFE_FINGER_CTL2, TT_EN )



/*===========================================================================

MACRO FINGER_GET_RSSI

DESCRIPTION     This macro gets the select finger's RSSI value, latched as
                of the last status dump.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define FINGER_GET_RSSI( rssi ) \
    rssi = MSM_RH( FN_LOCK_RSSI )

#else

#define FINGER_GET_RSSI( rssi ) \
    MSM_R2B( FN_LOCK_RSSI, LOWER, UPPER, rssi )

#endif


/*===========================================================================

MACRO FINGER_SET_RSSI

DESCRIPTION     This macro sets the after slew RSSI for the select finger.

RETURN VALUE    None.
      
SIDE EFFECTS    On completion of a microprocessor directed slew, this
                value is latched to the finger's signal level estimator.

===========================================================================*/

#define FINGER_SET_RSSI( rssi ) \
    MSM_WB( FFE_FN_RSSI_INIT, (rssi) )


/*===========================================================================

MACRO FINGER_SET_BINARY_OFFSET

DESCRIPTION     This macro sets the binary offset of the PN sequence for the
                current selected finger.  The value written should be:
                    Pilot_Offset * 64  +  2

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else
#define FINGER_SET_BINARY_OFFSET( bin_offset ) \
    MSM_W2B( FFE_FN_BINARY_OFFSET, LOWER, UPPER, (bin_offset) )
#endif


/*===========================================================================

MACRO FINGER_SET_IMASK

DESCRIPTION     This macro sets the PN mask for the I channel for the
                currently selected finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else
#define FINGER_SET_IMASK( imask ) \
    MSM_W2B( FFE_FN_PN_I_MASK, LOWER, UPPER, (imask) )
#endif


/*===========================================================================

MACRO FINGER_SET_QMASK

DESCRIPTION     This macro sets the PN mask for the Q channel for the
                currently selected finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else
#define FINGER_SET_QMASK( qmask ) \
    MSM_W2B( FFE_FN_PN_Q_MASK, LOWER, UPPER, (qmask) )
#endif


/*===========================================================================

MACRO FINGER_SET_WALSH

DESCRIPTION     This macro sets the walsh code for the select finger.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGER_SET_WALSH( walsh_code ) \
    MSM_WB( FFE_FN_CODE_CHAN_ID, (walsh_code) )


/*===========================================================================

MACRO FINGER_SLEW

DESCRIPTION     This macro command the select finger to slew.

RETURN VALUE    None.
      
SIDE EFFECTS    On completion of a microprocessor directed slew, the RSSI
                value is latched to the finger's signal level estimator.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else
#define FINGER_SLEW( slew ) \
    MSM_W3B( FFE_SLEW_VALUE, LOWER, MID, UPPER, (slew) )
#endif



/*===========================================================================

MACRO FINGER_GET_POSITION

DESCRIPTION     This macro returns the select finger's position as of the
                last status dump.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

#define FINGER_GET_POSITION( position ) \
    W_PTR(position)[ 0 ] = MSM_RH( FN_POSITION_LOWER ), \
    B_PTR(position)[ 2 ] = MSM_RB( FN_POSITION_UPPER )

#else

#define FINGER_GET_POSITION( position ) \
    MSM_R3B( FN_POSITION, LOWER, MID, UPPER, position )

#endif


/*===========================================================================

MACRO FINGER_SET_PWR_THRESH

DESCRIPTION     This macro sets the finger's power threshold.  The finger's
                energy must exceed this value before its symbols are used
                in power combining.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGER_SET_PWR_THRESH( pwr_thresh ) \
    MSM_WB( FFE_FN_PWR_THRESH, (pwr_thresh) )



#ifdef T_MSM5000
#ifndef T_MSM_5100
/*===========================================================================

MACRO FINGER_SET_TTL_FAST
MACRO FINGER_SET_TTL_SLOW

DESCRIPTION     These macros are used to set the finger TTL speed when
                FPC is enabled.  Channel 3 enable/disable bit is used
                for this purpose.

DEPENDENCIES    This function must be called from inside the context of a
                MSM_FINGER_SELECT / MSM_FINGER_UNSELECT pair, inside the
                context of a MSM_LOCK / MSM_UNLOCK pair to ensure proper
                operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    These macros must not be called when FPC is disabled, since
                firmware will not interpret these values for the finger TTL.
                Instead it will enable/disable channel 3, as default.

===========================================================================*/

#define FINGER_SET_TTL_FAST( ) \
    MSM_FINGER_WB_SET_BIT( FFE_FN_SUP_NUM, 3 )

#define FINGER_SET_TTL_SLOW( ) \
    MSM_FINGER_WB_CLEAR_BIT( FFE_FN_SUP_NUM, 3 )

#endif /* T_MSM_5100 */

/*===========================================================================

MACRO FINGER_CHANNEL_ENABLE
MACRO FINGER_CHANNEL_DISABLE

DESCRIPTION     These macros enable and disable the channels that are
                demodulated by the finger.

RETURN VALUE    None.
      
SIDE EFFECTS

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else

#define FINGER_CHANNEL_ENABLE( channel ) \
    MSM_FINGER_WB_SET_BIT( FFE_FN_SUP_NUM, channel )

#define FINGER_CHANNEL_DISABLE( channel ) \
    MSM_FINGER_WB_CLEAR_BIT( FFE_FN_SUP_NUM, channel )

#endif

#endif /* T_MSM5000 */

#ifdef T_MSM_5100
#error code not present
#endif /* T_MSM_5100 */

/*===========================================================================

MACRO FINGER_SET_NUM_SUP_CODE

DESCRIPTION     This macro sets the number of supplemental code channels 
                being demodulated by the finger.

RETURN VALUE    None.
      
SIDE EFFECTS    Takes effect on the 26.67ms boundary for the finger.

===========================================================================*/
#ifdef T_MSM_5100
#error code not present
#elif defined( T_MSM5000 )

#define FINGER_SET_NUM_SUP_CODE( channels ) \
    MSM_FINGER_WB_SHADOW( FFE_FN_SUP_NUM, (1 << ((channels)+1)) - 1 )

#else

#define FINGER_SET_NUM_SUP_CODE( channels ) \
    MSM_WB( FFE_FN_SUP_NUM, (channels) )

#endif

/*===========================================================================

MACRO FINGER_SET_SUP_CODE_WALSH

DESCRIPTION     This macros sets the finger's walsh code for the given
                supplemental code channel.

DEPENDENCIES    The channel number must be a literal constant.  It may not
                be a variable.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM_5100
#error code not present
#elif defined( T_MSM5000 )

#define FINGER_SET_SUP_CODE_WALSH( channel, walsh ) \
    MSM_WB( FN_CODE##channel, (walsh) )

#else

#define FINGER_SET_SUP_CODE_WALSH( channel, walsh ) \
    MSM_WB( FFE_FN_SUP_CODE##channel, (walsh) )

#endif


/*===========================================================================

                        DEMODULATOR FINGER CONTROL
                        
The following macros apply to all demodulator fingers.

===========================================================================*/



/*===========================================================================

MACRO FINGERS_SET_TT_ACCUMULATOR

DESCRIPTION     This macro sets the time tracking error accumulator.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGERS_SET_TT_ACCUMULATOR( value ) \
    MSM_W2B( FFE_TT_ACCUM, LOWER, UPPER, (value) )


/*===========================================================================

MACRO FINGERS_SET_TIMETRACK_K1_GAIN
MACRO FINGERS_SET_TIMETRACK_K2_GAIN
MACRO FINGERS_SET_TIMETRACK_GAINS

DESCRIPTION     These macros sets the "k1" and "k2" time tracking filter
                gains.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGERS_SET_TIMETRACK_K1_GAIN( k1_gain ) \
    MSM_WB( FFE_TT_K1_GAIN, (k1_gain) )

#define FINGERS_SET_TIMETRACK_K2_GAIN( k2_gain ) \
    MSM_WB( FFE_TT_K2_GAIN, (k2_gain) )

#define FINGERS_SET_TIMETRACK_GAINS( k1_gain, k2_gain ) \
    FINGERS_SET_TIMETRACK_K1_GAIN( (k1_gain) ),   \
    FINGERS_SET_TIMETRACK_K2_GAIN( (k2_gain) )


/*===========================================================================

MACRO FINGERS_SET_LOCK_RSSI_GAIN

DESCRIPTION     This macro sets the energy RSSI filter gain for all fingers.

DEPENDENCIES    None.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGERS_SET_LOCK_RSSI_GAIN( gain ) \
    MSM_WB( FFE_RSSI_FILT_GAIN, (gain) )


/*===========================================================================

MACRO FINGERS_SET_PILOT_FILTER_GAIN

DESCRIPTION     This macro sets the pilot filter gain for all fingers.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGERS_SET_PILOT_FILTER_GAIN( value ) \
    MSM_WB( FFE_PILOT_FILT_GAIN, (value) )


/*===========================================================================

MACRO FINGERS_SET_UPPER_LOCK_THRESHOLD
MACRO FINGERS_SET_LOWER_LOCK_THRESHOLD
MACRO FINGERS_SET_LOCK_THRESHOLDS

DESCRIPTION     These macros sets the upper and lower lock threshold for all
                fingers.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGERS_SET_UPPER_LOCK_THRESHOLD( upper_thresh ) \
    MSM_WB( FFE_LOCK_THRESH_UPPER, (upper_thresh) )

#define FINGERS_SET_LOWER_LOCK_THRESHOLD( lower_thresh ) \
    MSM_WB( FFE_LOCK_THRESH_LOWER, (lower_thresh) )

#define FINGERS_SET_LOCK_THRESHOLDS( upper_thresh, lower_thresh ) \
    FINGERS_SET_UPPER_LOCK_THRESHOLD( (upper_thresh) ), \
    FINGERS_SET_LOWER_LOCK_THRESHOLD( (lower_thresh) )


/*===========================================================================

MACRO FINGERS_SET_FREQ_COMB_GAIN

DESCRIPTION     This macro sets the Carrier Error Frequency Combiner Gain,
                which defines the bandwidth of the frequency tracking
                loop filter.

DEPENDENCIES    None.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define FINGERS_SET_FREQ_COMB_GAIN( gain ) \
    MSM_WB( FREQ_COMB_GAIN, (gain) )


#ifdef T_MSM5000
/*===========================================================================

MACRO COMBINER_CHANNEL_ENABLE
MACRO COMBINER_CHANNEL_DISABLE

DESCRIPTION     These macros enable and disable the channels that are
                demodulated in the combiner.

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS

===========================================================================*/

#define COMBINER_CHANNEL_ENABLE( channel ) \
    MSM_WB_SET_BIT( COMB_CHAN_CTL, channel )

#define COMBINER_CHANNEL_DISABLE( channel ) \
    MSM_WB_CLEAR_BIT( COMB_CHAN_CTL, channel )


#endif

/*===========================================================================

MACRO FINGERS_SET_NUM_SCCH_SOFT_COMBINED

DESCRIPTION     This macro sets the number of supplemental code channels 
                being soft combined.

DEPENDENCIES    None.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
    
#ifdef T_MSM5000

#define FINGERS_SET_NUM_SCCH_SOFT_COMBINED( channels ) \
        MSM_WB_SHADOW( COMB_CHAN_CTL, (1 << ((channels)+1)) - 1 )

#else

#define FINGERS_SET_NUM_SCCH_SOFT_COMBINED( channels ) \
        MSM_WB( COMB_SUP_NUM, (channels) )

#endif /* T_MSM5000 */

#ifdef T_MSM5105
#error code not present
#endif /* T_MSM5105 */



/*===========================================================================

                        DEMODULATOR SYMBOL COMBINER
                        
===========================================================================*/




/*===========================================================================

MACRO COMBINER_ENABLE_POWER_CONTROL
MACRO COMBINER_DISABLE_POWER_CONTROL

DESCRIPTION     These macros enable and disable closed loop power control

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define COMBINER_ENABLE_POWER_CONTROL( ) \
    if ( ! BIO_GET_SW ( BIO_SW5_M ) )\
    {\
      MSM_WB_SET( POWER_COMB_CTL, PWR_CTL_EN );\
    }

#define COMBINER_DISABLE_POWER_CONTROL( ) \
    MSM_WB_CLEAR( POWER_COMB_CTL, PWR_CTL_EN )


/*===========================================================================

MACRO COMBINER_ENABLE_PUNCTURING
MACRO COMBINER_DISABLE_PUNCTURING

DESCRIPTION     These macros enable and disable puncturing (erasures) in the
                combined data streams.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define COMBINER_ENABLE_PUNCTURING( ) \
    MSM_WB_SET( POWER_COMB_CTL, ERASE_EN )

#define COMBINER_DISABLE_PUNCTURING( ) \
    MSM_WB_CLEAR( POWER_COMB_CTL, ERASE_EN )

#ifdef T_MSM5000
/*==========================================================================
MACRO COMBINER_SELECT_CH0_128_CHIP_SYMBOL
MACRO COMBINER_SELECT_CH1_128_CHIP_SYMBOL

DESCRIPTION   This macro sets the combiner to accumulate symbols over two PN
              words(128 chips). This should be done when demodulating RC4 frames and 
              for 9.6kbps Quick Paging.


DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else
#define COMBINER_SELECT_CH0_128_CHIP_SYMBOL( ) \
     MSM_WH_CLEAR( SYMB_COMB_CTL1, LOCK_RSSI_EXT_RANGE ), \
     MSM_WH_SET( SYMB_COMB_CTL0, COMB_128 ), \
     MSM_WH_CLEAR( SYMB_COMB_CTL1, SYMB_RATE )
#endif
    
/*==========================================================================
MACRO COMBINER_SELECT_CH0_64_CHIP_SYMBOL
MACRO COMBINER_SELECT_CH1_64_CHIP_SYMBOL

DESCRIPTION   This macro sets the combiner to accumulate symbols over 64 chips.
              This should be done when demodulating RC1, RC2, RC3, RC5 and 
              Paging Channel frames.
           

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM_5100
#error code not present
#else
#define COMBINER_SELECT_CH0_64_CHIP_SYMBOL( ) \
     MSM_WH_CLEAR( SYMB_COMB_CTL1, LOCK_RSSI_EXT_RANGE ), \
     MSM_WH_CLEAR(SYMB_COMB_CTL0, COMB_128), \
     MSM_WH_CLEAR(SYMB_COMB_CTL1, SYMB_RATE)
#endif
    
/*==========================================================================
MACRO COMBINER_SELECT_CH0_256_CHIP_SYMBOL
MACRO COMBINER_SELECT_CH1_256_CHIP_SYMBOL

DESCRIPTION   This macro sets the combiner to accumulate symbols over 256 chips.
              This should be done when demodulating SYNC Channel.           

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM_5100
#error code not present
#else
#define COMBINER_SELECT_CH0_256_CHIP_SYMBOL( ) \
     MSM_WH_CLEAR( SYMB_COMB_CTL1, LOCK_RSSI_EXT_RANGE ), \
     MSM_WH_CLEAR(SYMB_COMB_CTL0, COMB_128), \
     MSM_WH_SET(SYMB_COMB_CTL1, SYMB_RATE)
#endif
    
/*==========================================================================
MACRO COMBINER_SELECT_CH0_512_CHIP_SYMBOL
MACRO COMBINER_SELECT_CH1_512_CHIP_SYMBOL

DESCRIPTION   This macro sets the combiner to accumulate symbols over 256 chips.
              This should be done when demodulating SYNC Channel.           

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM_5100
#error code not present
#else
#define COMBINER_SELECT_CH0_512_CHIP_SYMBOL( ) \
     MSM_WH_SET( SYMB_COMB_CTL1, LOCK_RSSI_EXT_RANGE ), \
     MSM_WH_CLEAR( SYMB_COMB_CTL0, COMB_128 ), \
     MSM_WH_SET( SYMB_COMB_CTL1, SYMB_RATE )
#endif /* T_MSM_5100 */

#ifndef T_MSM_5100
/*==========================================================================
MACRO COMBINER_SET_ULPN_OPTION_FOR_RC4

DESCRIPTION   This macro controls the ulpn option for RC4.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#define COMBINER_SET_ULPN_OPTION_FOR_RC4( ) \
     MSM_WH_SET(SYMB_COMB_CTL0, ULPN_OPTION)
 
/*==========================================================================
MACRO COMBINER_CLEAR_ULPN_OPTION

DESCRIPTION   This macro controls the ulpn option for RC1 RC2 RC3 & RC5

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.

SIDE EFFECTS    None.

===========================================================================*/
#define COMBINER_CLEAR_ULPN_OPTION( ) \
     MSM_WH_CLEAR(SYMB_COMB_CTL0, ULPN_OPTION)

#endif /* !T_MSM_5100 */
#endif /* T_MSM5000 */

/*===========================================================================
MACRO COMBINER_PUNCTURE_1_SYM
MACRO COMBINER_PUNCTURE_2_SYM

DESCRIPTION     These macros control whether 1 or 2 symbols are punctured.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define COMBINER_PUNCTURE_1_SYM( ) \
    MSM_WB_SET( POWER_COMB_CTL, POWER_CTL_PCT )

#define COMBINER_PUNCTURE_2_SYM( ) \
    MSM_WB_CLEAR( POWER_COMB_CTL, POWER_CTL_PCT )




/*===========================================================================

MACRO COMBINER_TIMETRACK_FINGER

DESCRIPTION     This macro selects the finger for the symbol combiner to
                timetrack.
                
DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

  /* Now a Word Register!  _WH_ instead of _WB_ */

#define COMBINER_TIMETRACK_FINGER( finger ) \
    MSM_WH_MASKED( SYMB_COMB_CTL0, \
                    SYMB_COMB_FIN_EN_n, \
                    DMOD_F0_EN_V << (finger) )

#else

#define COMBINER_TIMETRACK_FINGER( finger ) \
    MSM_WB_MASKED( SYMB_COMB_CTL0, \
                    SYMB_COMB_FIN_EN_n, \
                    DMOD_F0_EN_V << (finger) )

#endif


/*===========================================================================

MACRO COMBINER_TIMETRACK_DISABLE

DESCRIPTION     This macro selects no finger for the symbol combiner to
                timetrack.
                
DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

  /* Now a Word Register!  _WH_ instead of _WB_ */

#define COMBINER_TIMETRACK_DISABLE(  ) \
    MSM_WH_MASKED( SYMB_COMB_CTL0, SYMB_COMB_FIN_EN_n, 0x00 )

#else

#define COMBINER_TIMETRACK_DISABLE( ) \
    MSM_WB_MASKED( SYMB_COMB_CTL0, SYMB_COMB_FIN_EN_n, 0x00 )

#endif


/*===========================================================================

MACRO COMBINER_ARM_SLAM

DESCRIPTION     This macro arms a slam to a finger.
                
DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000

  /* Now a Word Register!  _WH_ instead of _WB_ */

#define COMBINER_ARM_SLAM( finger ) \
    COMBINER_TIMETRACK_FINGER( finger ), \
    MSM_WH_PULSE( SYMB_COMB_CTL0, SLAM_EN )

#else

#define COMBINER_ARM_SLAM( finger ) \
    COMBINER_TIMETRACK_FINGER( finger ), \
    MSM_WB_PULSE( SYMB_COMB_CTL0, SLAM_EN )

#endif


#ifdef T_MSM_5100
#error code not present
#else

/*===========================================================================

MACRO COMBINER_LOCK_RSSI_EXT_RANGE

DESCRIPTION     This macro enables the new Finger RSSI extended Range.

                NOTE: It must be enabled, since the 2.2 compatible mode of
                the ASIC (MSM2.3 only?) is broken.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM5000
#define COMBINER_LOCK_RSSI_EXT_RANGE( ) \
    MSM_WH_SET( SYMB_COMB_CTL1, LOCK_RSSI_EXT_RANGE)

#else
#define COMBINER_LOCK_RSSI_EXT_RANGE( ) \
    MSM_WB_SET( SYMB_COMB_CTL1, LOCK_RSSI_EXT_RANGE)
#endif

#endif

#ifdef T_MSM_5100
#error code not present
#endif /* T_MSM_5100 */

/*===========================================================================

MACRO COMBINER_ENABLE
MACRO COMBINER_DISABLE


DESCRIPTION     These macros enable and disable the symbol combiner.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    Combiner is in low-power mode when disabled.

===========================================================================*/
#ifdef T_MSM5000
    #define COMBINER_ENABLE( ) \
        MSM_WH_CLEAR( SYMB_COMB_CTL1, COMBINER_DISABLE )

    #define COMBINER_DISABLE( ) \
        MSM_WH_SET( SYMB_COMB_CTL1, COMBINER_DISABLE )

#else
    #define COMBINER_ENABLE( ) \
        MSM_WB_CLEAR( SYMB_COMB_CTL1, COMBINER_DISABLE )

    #define COMBINER_DISABLE( ) \
        MSM_WB_SET( SYMB_COMB_CTL1, COMBINER_DISABLE )
#endif

/*===========================================================================

MACRO COMBINER_ENABLE_DRIFT_COMPENSATION
MACRO COMBINER_DISABLE_DRIFT_COMPENSATION


DESCRIPTION     These macros enable and disable the drift compensation logic.
                Drift compensation is not needed if CHIPX8 is derived from
                TCXO and TCXO is controlled by TRK_LO_ADJ.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM5000
#define COMBINER_ENABLE_DRIFT_COMPENSATION( ) \
    MSM_WH_SET( SYMB_COMB_CTL1, EN_DDS )

#define COMBINER_DISABLE_DRIFT_COMPENSATION( ) \
    MSM_WH_CLEAR( SYMB_COMB_CTL1, EN_DDS )
#else
#define COMBINER_ENABLE_DRIFT_COMPENSATION( ) \
    MSM_WB_SET( SYMB_COMB_CTL1, EN_DDS )

#define COMBINER_DISABLE_DRIFT_COMPENSATION( ) \
    MSM_WB_CLEAR( SYMB_COMB_CTL1, EN_DDS )
#endif

/*===========================================================================

MACRO COMBINER_80MS_RESET

DESCRIPTION     This macro initializes the combiner reference to align with
                one of 3 pilot PN rollover points of a demodulating finger.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM5000
#define COMBINER_80MS_RESET( rollover_value ) \
    MSM_WH_MASKED( SYMB_COMB_CTL1, 80MS_RESET_CTL, (rollover_value) )

#else
#define COMBINER_80MS_RESET( rollover_value ) \
    MSM_WB_MASKED( SYMB_COMB_CTL1, 80MS_RESET_CTL, (rollover_value) )
#endif

#ifndef T_MSM_5100
/*===========================================================================

MACRO COMBINER_SET_SYMBOL_RATE

DESCRIPTION     This macro sets the symbol rate of the symbol combiner

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/
#ifdef T_MSM5000
#define COMBINER_SET_SYMBOL_RATE( symbol_rate ) \
    MSM_WH_MASKED( SYMB_COMB_CTL1, SYMB_RATE, (symbol_rate) )

#else
#define COMBINER_SET_SYMBOL_RATE( symbol_rate ) \
    MSM_WB_MASKED( SYMB_COMB_CTL1, SYMB_RATE, (symbol_rate) )
#endif

#endif /* !T_MSM_5100 */

/*===========================================================================

MACRO COMBINER_LATCH_POSITION

DESCRIPTION     This macro causes the symbol combiner's position to be latch.

DEPENDENCIES    This macro and the COMBINER_GET_POSITION function must
                be called from inside the context of a MSM_LOCK / 
                MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    Symbol Combiner Time Stamp Register is updated.

===========================================================================*/

#define COMBINER_LATCH_POSITION( ) \
    MSM_WB( SYMB_COMB_POS_DUMP, 0x00 )


/*===========================================================================

MACRO COMBINER_GET_POSITION

DESCRIPTION     This macro reads the latched symbol combiner's position.

DEPENDENCIES    This macro and the DMOD_SYMB_COMB_LATCH_POS function must
                be called from inside the context of a MSM_LOCK / 
                MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else

#define COMBINER_GET_POSITION( position ) \
    MSM_R2B( SYMB_COMB_POSITION2, LOWER, UPPER, position )

#endif /* T_MSM_5100 */




/*===========================================================================

MACRO COMBINER_SET_FREQUENCY_ADJUST

DESCRIPTION     This macro set the symbol combiner's frequency adjust.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define COMBINER_SET_FREQUENCY_ADJUST( freq_adj ) \
    MSM_W2B( SYMB_COMB_FREQ_ADJ, LOWER, UPPER, (freq_adj) )


/*===========================================================================

MACRO COMBINER_SET_LONG_CODE_STATE
MACRO COMBINER_ZERO_LONG_CODE_STATE

DESCRIPTION     These macros set or zero the long code state.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
                The lc_state argument must be l-value.

RETURN VALUE    None.
      
SIDE EFFECTS    The long code is transfered from the load register to the
                long code generator on the next PN roll.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else

#define COMBINER_SET_LONG_CODE_STATE( lc_state ) \
    MSM_W6B( SYMB_COMB_LONG_CODE_LD, 0,1,2,3,4,5, lc_state )

#define COMBINER_ZERO_LONG_CODE_STATE( ) \
    MSM_LOCK_REQUIRED \
    MSM_WB( SYMB_COMB_LONG_CODE_LD_0, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_LD_1, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_LD_2, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_LD_3, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_LD_4, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_LD_0, 0x00 )

#endif


/*===========================================================================

MACRO COMBINER_GET_LONG_CODE_STATE

DESCRIPTION     This macro gets the long code state as of the last PN roll.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
                The lc_state argument must be l-value.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else

#define COMBINER_GET_LONG_CODE_STATE( lc_state ) \
    MSM_R6B( SYMB_COMB_LONG_CODE_RD, 0,1,2,3,4,5, lc_state )

#endif /* T_MSM_5100 */

#ifdef T_MSM_5100
#error code not present
#endif /* T_MSM_5100 */

/*===========================================================================

MACRO COMBINER_SET_LONG_CODE_MASK
MACRO COMBINER_ZERO_LONG_CODE_MASK

DESCRIPTION     These macros set or zero the long code mask.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

                The lc_mask argument must be l-value.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM_5100
#error code not present
#else

#define COMBINER_SET_LONG_CODE_MASK( lc_mask ) \
    MSM_W6B( SYMB_COMB_LONG_CODE_MASK, 0,1,2,3,4,5, lc_mask )

#define COMBINER_ZERO_LONG_CODE_MASK( ) \
    MSM_LOCK_REQUIRED \
    MSM_WB( SYMB_COMB_LONG_CODE_MASK_0, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_MASK_1, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_MASK_2, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_MASK_3, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_MASK_4, 0x00 ), \
    MSM_WB( SYMB_COMB_LONG_CODE_MASK_5, 0x00 )

#endif


/*===========================================================================

MACRO COMBINER_GENERATE_EPOCH

DESCRIPTION     This macro causes the generation of an epoch signal.

DEPENDENCIES    None.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    Fingers, searchers, PN generators, PDM's are initialized.

===========================================================================*/

#define COMBINER_GENERATE_EPOCH( ) \
    MSM_WB( EPOCH_WR, 0x00 )



/*===========================================================================

           DECODER
                        
===========================================================================*/



/*===========================================================================

MACRO DECODER_INITIAL_STATE_3_ZEROS
MACRO DECODER_INITIAL_STATE_4_ZEROS
MACRO DECODER_INITIAL_STATE_8_ZEROS

DESCRIPTION     These macros set the Decoder's State Metrics to values
                corresponding to the preceeding frame ending with 3, 4 or
                8 zeros.

DEPENDENCIES    Continuous mode only.  DECODER_INITIAL_STATE_NORMAL() must
                be used after the desired frame has been decoded.

RETURN VALUE    None.
      
SIDE EFFECTS    Decoders state metrics are zero'd or saturated at all
                future decoder interrupts until normal operation is
                restored.

===========================================================================*/

#ifdef T_MSM5000
#define DECODER_INITIAL_STATE_3_ZEROS( ) \
    MSM_WB_MASKED( VD_MODE, INTMODE, 0x06 )

#define DECODER_INITIAL_STATE_4_ZEROS( ) \
    MSM_WB_MASKED( VD_MODE, INTMODE, 0x04 )

#define DECODER_INITIAL_STATE_8_ZEROS( ) \
    MSM_WB_MASKED( VD_MODE, INTMODE, 0x02 )
#else
#define DECODER_INITIAL_STATE_3_ZEROS( ) \
    MSM_WB_MASKED( DECMODE, INTMODE, 0x60 )

#define DECODER_INITIAL_STATE_4_ZEROS( ) \
    MSM_WB_MASKED( DECMODE, INTMODE, 0x40 )

#define DECODER_INITIAL_STATE_8_ZEROS( ) \
    MSM_WB_MASKED( DECMODE, INTMODE, 0x20 )
#endif /* T_MSM5000 */


/*===========================================================================

MACRO DECODER_INITIAL_STATE_NORMAL

DESCRIPTION     This macro terminates any continuous mode decoder
                initialization started by:
                    DECODER_INITIAL_STATE_3_ZEROS( )
                    DECODER_INITIAL_STATE_4_ZEROS( )
                    DECODER_INITIAL_STATE_8_ZEROS( )

DEPENDENCIES    Continuous mode only.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef T_MSM5000
#define DECODER_INITIAL_STATE_NORMAL( ) \
    MSM_WB_MASKED( VD_MODE, INTMODE, 0x00 )
#else
#define DECODER_INITIAL_STATE_NORMAL( ) \
    MSM_WB_MASKED( DECMODE, INTMODE, 0x00 )
#endif /* T_MSM5000 */



/*===========================================================================

           CDMA AGC, FREQUENCY TRACKING AND BASEBAND OFFSET CONTROL
                        
===========================================================================*/




/*===========================================================================

MACRO SEARCH_FAST_AGC_LOOP
MACRO SEARCH_NORMAL_AGC_LOOP

DESCRIPTION     These macros change the gain constant of the AGC loop, which
                changes the time it will take to settle and the stability of
                the loop.
                
DEPENDENCIES    Must include "cagc.h".
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SEARCH_FAST_AGC_LOOP( ) \
    MSM_OUTM( AGC_CTL4_WB, AGC_CTL4_WB__GAIN_CONSTANT_MASK, CAGC_TC_0147_US );

#define SEARCH_NORMAL_AGC_LOOP( ) \
    MSM_OUTM( AGC_CTL4_WB, AGC_CTL4_WB__GAIN_CONSTANT_MASK, CAGC_TC_0392_US );


/*===========================================================================

MACRO TRK_LO_ADJ_SET_CROSS_MODE

DESCRIPTION     This macro sets the TRK LO ADJ cross product mode

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TRK_LO_ADJ_SET_CROSS_MODE( mode ) \
    MSM_WB_MASKED( FREQ_COMB_CTL, CROSS_MODE, (mode) )


/*===========================================================================

MACRO TRK_LO_ADJ_ENABLE_OUTPUT
MACRO TRK_LO_ADJ_DISABLE_OUTPUT

DESCRIPTION     These macros enable and disable (high-impedance) the
                TRK_LO_ADJ output pin.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TRK_LO_ADJ_ENABLE_OUTPUT( ) \
    MSM_WB_SET( FREQ_COMB_CTL, TRK_LO_ADJ_EN )

#define TRK_LO_ADJ_DISABLE_OUTPUT( ) \
    MSM_WB_CLEAR( FREQ_COMB_CTL, TRK_LO_ADJ_EN )



/*===========================================================================

MACRO TRK_LO_ADJ_SET_NORMAL_POLARITY
MACRO TRK_LO_ADJ_SET_INVERTED_POLARITY

DESCRIPTION     These macros control the polarity of the TRK_LO_ADJUST pin.
                With normal polarity, a positive frequency error will
                decrease the density of high pulse on the pin.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TRK_LO_ADJ_SET_NORMAL_POLARITY( ) \
    MSM_WB_CLEAR( FREQ_COMB_CTL, TRK_LO_ADJ_POLARITY )

#define TRK_LO_ADJ_SET_INVERTED_POLARITY( ) \
    MSM_WB_SET( FREQ_COMB_CTL, TRK_LO_ADJ_POLARITY )


/*===========================================================================

MACRO TRK_LO_ADJ_ENABLE_FREQUENCY_TRACK
MACRO TRK_LO_ADJ_OVERRIDE

DESCRIPTION     These macros enable carrier frequency tracking operation, or
                override the normal operation.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TRK_LO_ADJ_ENABLE_FREQUENCY_TRACK( ) \
    MSM_WB_SET( FREQ_COMB_CTL, CARRIER_FREQ_TRACK_EN )

#define TRK_LO_ADJ_OVERRIDE( ) \
    MSM_WB_CLEAR( FREQ_COMB_CTL, CARRIER_FREQ_TRACK_EN )


/*===========================================================================

MACRO TRK_LO_ADJ_SET_OVERRIDE

DESCRIPTION     This macro controls the pulse density stream from the
                TRK_LO_ADJ pin when the TRK_LO_ADJ_OVERRIDE( ) has been
                called.
                
                Assuming TRK_LO_ADJ_NORMAL_POLARITY():
                  0x0000  -  Stays high 511/512 chipx8 clock cycles.
                  0x8000  -  50%
                  0xffff  -  Stays low throughout 512 chipx8 clock cycles.
                  
                Lower 7 bits are ignored.

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TRK_LO_ADJ_SET_OVERRIDE( override ) \
    MSM_WB_STROBE_LOW( FREQ_COMB_CTL, CARRIER_FREQ_TRACK_EN ), \
    MSM_W2B( CARRIER_FREQ_ERR_WR, LSB, MSB, (override) ), \
    MSM_WB_RESTORE( FREQ_COMB_CTL )


/*===========================================================================

MACRO TRK_LO_ADJ_GET_FREQ_ERR

DESCRIPTION     This macro retrieves the contents of the Frequency Error
                Accumulator

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TRK_LO_ADJ_GET_FREQ_ERR( freq_err_acc ) \
    MSM_R2B( CARRIER_FREQ_ERR_RD, LSB, MSB, (freq_err_acc) )


#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/*===========================================================================

MACRO TRK_LO_ADJ_SET_FREQ_ERR_HIGH

DESCRIPTION     This macro sets the contents of the upper byte of the
                Frequency Error Accumulator, and sets the lower byte to
                0x80 (1/2 of LSB of upper byte).

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TRK_LO_ADJ_SET_FREQ_ERR_HIGH( freq_err_acc ) \
    MSM_WB( CARRIER_FREQ_ERR_WR_LSB, 0x80 ), \
    MSM_WB( CARRIER_FREQ_ERR_WR_MSB, (freq_err_acc) )


/*===========================================================================

MACRO TRK_LO_ADJ_GET_FREQ_ERR_HIGH

DESCRIPTION     This macro retrieves the contents of the upper byte of the
                Frequency Error Accumulator

DEPENDENCIES    None.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TRK_LO_ADJ_GET_FREQ_ERR_HIGH( ) \
    MSM_RB( CARRIER_FREQ_ERR_RD_MSB )


/*===========================================================================

MACRO CAGC_WRITE_8P1
MACRO CAGC_READ_8P1

DESCRIPTION     Helper macro for 8+1 register writes.

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    CAGC_LOW_HIGH/ control is left in HIGH/ state.

===========================================================================*/

#define CAGC_SELECT_LOW( ) \
    MSM_WB_SET( AGC_RDWR_CTL, AGC_LOW_HIGH_N)

#define CAGC_SELECT_HIGH( ) \
    MSM_WB_CLEAR( AGC_RDWR_CTL, AGC_LOW_HIGH_N)

#define CAGC_WRITE_8P1( address, value) \
    CAGC_SELECT_LOW( ), \
    MSM_WB( address, (byte) ((value) << 7) ), \
    CAGC_SELECT_HIGH( ), \
    MSM_WB( address, (byte) ((value) >> 1) )

#define CAGC_READ_8P1( address, var) \
    CAGC_SELECT_LOW( ), \
    B_PTR(var)[ 0 ] = MSM_RB( address ), \
    CAGC_SELECT_HIGH( ), \
    B_PTR(var)[ 1 ] = MSM_RB( address ), \
    var >>= 7


/*===========================================================================

MACRO CAGC_WRITE_8P2
MACRO CAGC_READ_8P2

DESCRIPTION     Helper macro for 8+2 register writes.

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.

RETURN VALUE    None.

SIDE EFFECTS    CAGC_LOW_HIGH/ control is left in HIGH/ state.

===========================================================================*/

#define CAGC_WRITE_8P2( address, value) \
    CAGC_SELECT_LOW( ), \
    MSM_WB( address, (byte) ((value) << 6) ), \
    CAGC_SELECT_HIGH( ), \
    MSM_WB( address, (byte) ((value) >> 2) )

#define CAGC_READ_8P2( address, var) \
    CAGC_SELECT_LOW( ), \
    B_PTR(var)[ 0 ] = MSM_RB( address ), \
    CAGC_SELECT_HIGH( ), \
    B_PTR(var)[ 1 ] = MSM_RB( address ), \
    var >>= 6

/*===========================================================================

MACRO CAGC_VALUE_WRITE
MACRO CAGC_VALUE_READ

DESCRIPTION     This macro writes to or retrieves from the contents of the 
                Adjusted Gain Control

DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    

===========================================================================*/
#define CAGC_VALUE_WRITE(value) \
    CAGC_WRITE_8P2( AGC_VALUE_WR, value ) 

#define CAGC_VALUE_READ(var) \
    CAGC_READ_8P2( AGC_VALUE_RD, var ) 


/*===========================================================================

MACRO I_OFFSET_SET_ACCUMULATOR

DESCRIPTION     This macro sets (overrides) the I_OFFSET accumulator value

DEPENDENCIES    None.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define I_OFFSET_SET_ACCUMULATOR( accumulator ) \
    MSM_WB( I_OFFSET_WR, (accumulator) )

#define I_OFFSET_SET_ACCUMULATOR_8P1( accumulator ) \
    CAGC_WRITE_8P1( I_OFFSET_WR, (accumulator) )


/*===========================================================================

MACRO I_OFFSET_GET_ACCUMULATOR

DESCRIPTION     This macro gets the current I_OFFSET accumulator value

DEPENDENCIES    None.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define I_OFFSET_GET_ACCUMULATOR( accumulator ) \
    accumulator = MSM_RB( I_OFFSET_RD )

#define I_OFFSET_GET_ACCUMULATOR_8P1( accumulator ) \
    CAGC_READ_8P1( I_OFFSET_RD, accumulator )


/*===========================================================================

MACRO Q_OFFSET_SET_ACCUMULATOR

DESCRIPTION     This macro sets (overrides) the Q_OFFSET accumulator value

DEPENDENCIES    None.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define Q_OFFSET_SET_ACCUMULATOR( accumulator ) \
    MSM_WB( Q_OFFSET_WR, (accumulator) )

#define Q_OFFSET_SET_ACCUMULATOR_8P1( accumulator ) \
    CAGC_WRITE_8P1( Q_OFFSET_WR, (accumulator) )


/*===========================================================================

MACRO Q_OFFSET_GET_ACCUMULATOR

DESCRIPTION     This macro gets the current Q_OFFSET accumulator value

DEPENDENCIES    None.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define Q_OFFSET_GET_ACCUMULATOR( accumulator ) \
    accumulator = MSM_RB( Q_OFFSET_RD )

#define Q_OFFSET_GET_ACCUMULATOR_8P1( accumulator ) \
    CAGC_READ_8P1( Q_OFFSET_RD, accumulator )


/*===========================================================================

MACRO I_OFFSET_AGC_CONTROL
MACRO I_OFFSET_OVERRIDE

DESCRIPTION     These macros control whether the I Offset PDM receives or
                overrides data from the AGC unit.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define I_OFFSET_AGC_CONTROL( ) \
    MSM_WB_SET( IQ_OFFSET_CTL, I_OFFSET_OVERRIDE_N )

#define I_OFFSET_OVERRIDE( ) \
    MSM_WB_CLEAR( IQ_OFFSET_CTL, I_OFFSET_OVERRIDE_N )


/*===========================================================================

MACRO Q_OFFSET_AGC_CONTROL
MACRO Q_OFFSET_OVERRIDE

DESCRIPTION     These macros control whether the Q Offset PDM receives or
                overrides data from the AGC unit.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define Q_OFFSET_AGC_CONTROL( ) \
    MSM_WB_SET( IQ_OFFSET_CTL, Q_OFFSET_OVERRIDE_N )

#define Q_OFFSET_OVERRIDE( ) \
    MSM_WB_CLEAR( IQ_OFFSET_CTL, Q_OFFSET_OVERRIDE_N )


/*===========================================================================

MACRO I_OFFSET_SET_NORMAL_POLARITY
MACRO I_OFFSET_SET_INVERTED_POLARITY

DESCRIPTION     These macros the polarity of the I_OFFSET pin.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define I_OFFSET_SET_NORMAL_POLARITY( ) \
    MSM_WB_CLEAR( IQ_OFFSET_CTL, I_OFFSET_POLARITY )

#define I_OFFSET_SET_INVERTED_POLARITY(  ) \
    MSM_WB_SET( IQ_OFFSET_CTL, I_OFFSET_POLARITY )


/*===========================================================================

MACRO Q_OFFSET_SET_NORMAL_POLARITY
MACRO Q_OFFSET_SET_INVERTED_POLARITY

DESCRIPTION     These macros the polarity of the Q_OFFSET pin.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define Q_OFFSET_SET_NORMAL_POLARITY( ) \
    MSM_WB_CLEAR( IQ_OFFSET_CTL, Q_OFFSET_POLARITY )

#define Q_OFFSET_SET_INVERTED_POLARITY(  ) \
    MSM_WB_SET( IQ_OFFSET_CTL, Q_OFFSET_POLARITY )


/*===========================================================================

MACRO IQ_OFFSET_ENABLE
MACRO IQ_OFFSET_DISABLE

DESCRIPTION     These macros the enable and disable (high-impedance) the
                I/Q OFFSET and AGC_REF signals.

DEPENDENCIES    This macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define IQ_OFFSET_ENABLE( ) \
    MSM_WB_SET( IQ_OFFSET_CTL, IQ_OFFSET_EN )

#define IQ_OFFSET_DISABLE(  ) \
    MSM_WB_CLEAR( IQ_OFFSET_CTL, IQ_OFFSET_EN )


/*===========================================================================

MACRO IQ_OFFSET_NORMAL_LOOP_GAIN
MACRO IQ_OFFSET_QUARTER_LOOP_GAIN

DESCRIPTION     These macros the enable and disable (high-impedance) the
                I/Q OFFSET and AGC_REF signals.

DEPENDENCIES    These macros must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define IQ_OFFSET_NORMAL_LOOP_GAIN( ) \
    MSM_WB_CLEAR( IQ_OFFSET_CTL, DC_LOOP_GAIN )

#define IQ_OFFSET_QUARTER_LOOP_GAIN(  ) \
    MSM_WB_SET( IQ_OFFSET_CTL, DC_LOOP_GAIN )


#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */


/*===========================================================================

MACRO TX_SET_GAIN_ADJUST

DESCRIPTION     This macro ....

                The power control enable bit is cleared first, so this write
                takes effect.  Power control is then restored to its previous
                state.

DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define TX_SET_GAIN_ADJUST( gain ) \
    MSM_WB_STROBE_LOW( POWER_COMB_CTL, PWR_CTL_EN ), \
    MSM_WB( TX_GAIN_ADJ_WR, (gain) ), \
    MSM_WB_RESTORE( POWER_COMB_CTL )



/*===========================================================================

                        CDMA SLEEP CONTROL
                        
===========================================================================*/




/*===========================================================================

MACRO SLEEP_CTL_INIT

DESCRIPTION     This macro initializes the Sleep Control Register
                
DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#if defined (T_MSM5000) && !defined(T_MSM5105)

#define SLEEP_CTL_INIT( ) \
    MSM_WB_INIT( SLEEP_CTL )
      /* MSM5000 automatically includes duty cycle fix */

#else
#error code not present
#endif


/*===========================================================================

MACRO SLEEP_CTL_FORCE_WAKEUP

DESCRIPTION     This macro forces the demodulator to wakeup from sleep.
                
DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SLEEP_CTL_FORCE_WAKEUP() \
    MSM_WB_PULSE( SLEEP_CTL, WAKE_UP_EN )


/*===========================================================================

MACRO SLEEP_CTL_ARM_SLEEP

DESCRIPTION     This macro arms the MSM to sleep at the next PN roll.
                
DEPENDENCIES    This macro must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
                SLEEP/WAKEUP timers should be set before arming sleep.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SLEEP_CTL_ARM_SLEEP() \
    MSM_WB_PULSE( SLEEP_CTL, ASIC_SLEEP_ARM )


/*===========================================================================

MACRO SLEEP_SET_SLEEP_INTERVAL

DESCRIPTION     This macro sets the number of clock cycles the MSM should
                sleep for.  
                
DEPENDENCIES    This function must be called from inside the context of a
                MSM_LOCK / MSM_UNLOCK pair to ensure proper operation.
                
                The value written should be 19 less than the actual req'd
                value.
                
RETURN VALUE    None.
      
SIDE EFFECTS    The warmup timer will run after the sleep interval expires.
                If RF_N_MASK_N is set, instead of transitioning into CDMA
                mode the sleep timer will run again.

===========================================================================*/

#define SLEEP_SET_SLEEP_INTERVAL( sleep_clocks ) \
    MSM_W2H( POWER_DOWN_SLEEP_INTERVAL, 0, 1, sleep_clocks)


/*===========================================================================

MACRO SLEEP_SET_WARMUP_TIME

DESCRIPTION     This macro sets the number of clock cycles the MSM should
                wait before enabling CDMA after the sleep timer expires.  
                
DEPENDENCIES    None.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    If RF_N_MASK_N is set, instead of transitioning into CDMA
                mode the sleep timer will run again.

===========================================================================*/

#define SLEEP_SET_WARMUP_TIME( warmup_clocks ) \
    MSM_WH( POWER_DOWN_WU_TIME, (warmup_clocks) )


/*===========================================================================

MACRO SLEEP_SET_CHIPX8_TIME

DESCRIPTION     This macro sets the number of chipx8's in the Power Down
                ChipX8 Sleep Timer.
                
                This value must be preset before going to sleep, and can
                be adjusted before wakeup in the last catnap, to fine
                adjust the duration of the sleep timer.
                
DEPENDENCIES    None.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SLEEP_SET_CHIPX8_TIME( chipx8_clocks ) \
    MSM_WH( POWER_DOWN_CHIPX8_SLEEP_TIME, (chipx8_clocks) )

/*===========================================================================

MACRO SLEEP_GET_CHIPX8_TIME

DESCRIPTION     This macro reads the number of chipx8's in the Power Down
                ChipX8 Sleep Timer.
                
                This value may be used to fine adjust the duration of the
                sleep timer.
                
DEPENDENCIES    None.
                                
RETURN VALUE    Contents of the sleep chipx8 power down timer.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SLEEP_GET_CHIPX8_TIME( ) \
    MSM_RH( POWER_DOWN_CHIPX8_COUNT )

/*===========================================================================

MACRO SLEEP_GET_FREQ_ERROR_ESTIMATE

DESCRIPTION     This macro reads the number of chipx8's in the latest
                calibration period.
                
                For chips that use a fixed period of 255 slow clocks, and
                start the chipx8 count at -161 for each slow clock, the
                value return is automatically increased by 161*255. 
                
DEPENDENCIES    None.
                                
RETURN VALUE    Chipx8's in sample period.
      
SIDE EFFECTS    None.

===========================================================================*/

#define SLEEP_GET_FREQ_ERROR_ESTIMATE( ) \
    ( 41055ul + (uint2) MSM_RH( SLEEP_XTAL_FREQ_ERR ) )


/*===========================================================================

                        Interrupt Controller
                        
===========================================================================*/




/*===========================================================================

MACRO INTERRUPT_CLEAR

DESCRIPTION     These macros reset the interrupt status register for the
                given interrupt line.
                
DEPENDENCIES    None.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define INTERRUPT_CLEAR( interrupt_0_mask, interrupt_1_mask ) \
    MSM_WH( INT_CLEAR_0, (interrupt_0_mask) ), \
    MSM_WH( INT_CLEAR_1, (interrupt_1_mask) )

#define INTERRUPT_CLEAR_SAT_CHANGED( )   MSM_WH( INT_CLEAR_0, 0x8000 )
#define INTERRUPT_CLEAR_AUX_PCM_DIN( )   MSM_WH( INT_CLEAR_0, 0x4000 )
#define INTERRUPT_CLEAR_KEYSENSE( )      MSM_WH( INT_CLEAR_0, 0x2000 )
#define INTERRUPT_CLEAR_TX_WBD( )        MSM_WH( INT_CLEAR_0, 0x1000 )
#define INTERRUPT_CLEAR_RX_WBD( )        MSM_WH( INT_CLEAR_0, 0x0800 )
#define INTERRUPT_CLEAR_UART( )          MSM_WH( INT_CLEAR_0, 0x0400 )
#define INTERRUPT_CLEAR_TIME_TICK( )     MSM_WH( INT_CLEAR_0, 0x0200 )
#define INTERRUPT_CLEAR_26MS( )          MSM_WH( INT_CLEAR_0, 0x0100 )
#define INTERRUPT_CLEAR_SEARCH_DONE( )   MSM_WH( INT_CLEAR_0, 0x0080 )
#define INTERRUPT_CLEAR_MASKED_VC_ENC( ) MSM_WH( INT_CLEAR_0, 0x0040 )
#define INTERRUPT_CLEAR_MASKED_VC_DEC( ) MSM_WH( INT_CLEAR_0, 0x0020 )
#define INTERRUPT_CLEAR_DEC( )           MSM_WH( INT_CLEAR_0, 0x0010 )
#define INTERRUPT_CLEAR_TX_FR( )         MSM_WH( INT_CLEAR_0, 0x0008 )
#define INTERRUPT_CLEAR_TX_1P25MS( )     MSM_WH( INT_CLEAR_0, 0x0004 )
#define INTERRUPT_CLEAR_WAKEUP( )        MSM_WH( INT_CLEAR_0, 0x0002 )
#define INTERRUPT_CLEAR_GPTIMER( )       MSM_WH( INT_CLEAR_0, 0x0001 )

#define INTERRUPT_CLEAR_DP_RX_DATA( )    MSM_WH( INT_CLEAR_1, 0x0100 )
#define INTERRUPT_CLEAR_SEARCH_DMA( )    MSM_WH( INT_CLEAR_1, 0x0080 )
#define INTERRUPT_CLEAR_SLEEP_FEE( )     MSM_WH( INT_CLEAR_1, 0x0040 )
#define INTERRUPT_CLEAR_GPIO_4( )        MSM_WH( INT_CLEAR_1, 0x0020 )
#define INTERRUPT_CLEAR_GPIO_3( )        MSM_WH( INT_CLEAR_1, 0x0010 )
#define INTERRUPT_CLEAR_GPIO_2( )        MSM_WH( INT_CLEAR_1, 0x0008 )
#define INTERRUPT_CLEAR_GPIO_1( )        MSM_WH( INT_CLEAR_1, 0x0004 )
#define INTERRUPT_CLEAR_GPIO_0( )        MSM_WH( INT_CLEAR_1, 0x0002 )
#define INTERRUPT_CLEAR_SBI( )           MSM_WH( INT_CLEAR_1, 0x0001 )


/*===========================================================================

MACRO INTERRUPT_STATUS

DESCRIPTION     These macros check the interrupt status for the
                given interrupt line.
                
DEPENDENCIES    None.
                                
RETURN VALUE    0 if the specified interrupt in not pending
                Non-zero if the interrupt is pending.
      
SIDE EFFECTS    None.

===========================================================================*/

#define INTERRUPT_STATUS_SAT_CHANGED( )   ( MSM_RH(INT_STATUS_0) & 0x8000 )
#define INTERRUPT_STATUS_AUX_PCM_DIN( )   ( MSM_RH(INT_STATUS_0) & 0x4000 )
#define INTERRUPT_STATUS_KEYSENSE( )      ( MSM_RH(INT_STATUS_0) & 0x2000 )
#define INTERRUPT_STATUS_TX_WBD( )        ( MSM_RH(INT_STATUS_0) & 0x1000 )
#define INTERRUPT_STATUS_RX_WBD( )        ( MSM_RH(INT_STATUS_0) & 0x0800 )
#define INTERRUPT_STATUS_UART( )          ( MSM_RH(INT_STATUS_0) & 0x0400 )
#define INTERRUPT_STATUS_TIME_TICK( )     ( MSM_RH(INT_STATUS_0) & 0x0200 )
#define INTERRUPT_STATUS_26MS( )          ( MSM_RH(INT_STATUS_0) & 0x0100 )
#define INTERRUPT_STATUS_SEARCH_DONE( )   ( MSM_RH(INT_STATUS_0) & 0x0080 )
#define INTERRUPT_STATUS_MASKED_VC_ENC( ) ( MSM_RH(INT_STATUS_0) & 0x0040 )
#define INTERRUPT_STATUS_MASKED_VC_DEC( ) ( MSM_RH(INT_STATUS_0) & 0x0020 )
#define INTERRUPT_STATUS_DEC( )           ( MSM_RH(INT_STATUS_0) & 0x0010 )
#define INTERRUPT_STATUS_TX_FR( )         ( MSM_RH(INT_STATUS_0) & 0x0008 )
#define INTERRUPT_STATUS_TX_1P25MS( )     ( MSM_RH(INT_STATUS_0) & 0x0004 )
#define INTERRUPT_STATUS_WAKEUP( )        ( MSM_RH(INT_STATUS_0) & 0x0002 )
#define INTERRUPT_STATUS_GPTIMER( )       ( MSM_RH(INT_STATUS_0) & 0x0001 )

#define INTERRUPT_STATUS_DP_RX_DATA( )    ( MSM_RH(INT_STATUS_1) & 0x0100 )
#define INTERRUPT_STATUS_SEARCH_DMA( )    ( MSM_RH(INT_STATUS_1) & 0x0080 )
#define INTERRUPT_STATUS_SLEEP_FEE( )     ( MSM_RH(INT_STATUS_1) & 0x0040 )
#define INTERRUPT_STATUS_GPIO_4( )        ( MSM_RH(INT_STATUS_1) & 0x0020 )
#define INTERRUPT_STATUS_GPIO_3( )        ( MSM_RH(INT_STATUS_1) & 0x0010 )
#define INTERRUPT_STATUS_GPIO_2( )        ( MSM_RH(INT_STATUS_1) & 0x0008 )
#define INTERRUPT_STATUS_GPIO_1( )        ( MSM_RH(INT_STATUS_1) & 0x0004 )
#define INTERRUPT_STATUS_GPIO_0( )        ( MSM_RH(INT_STATUS_1) & 0x0002 )
#define INTERRUPT_STATUS_SBI( )           ( MSM_RH(INT_STATUS_1) & 0x0001 )


/*===========================================================================

MACRO INTERRUPT_MASK_?????

DESCRIPTION     These macros reset the interrupt status register for the
                given interrupt line.
                
DEPENDENCIES    None.
                                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

/* ??? */



/*===========================================================================

                        TCXO PDM#1 and TCXO PDM#2
                        
===========================================================================*/




/*===========================================================================

MACRO PDM1_SET_NORMAL_POLARITY
MACRO PDM1_SET_INVERTED_POLARITY


DESCRIPTION     These macros alter the polarity of PDM #1

DEPENDENCIES    These macros must be called within a MSM_LOCK / MSM_UNLOCK
                pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define PDM1_SET_NORMAL_POLARITY( ) \
    MSM_WB_CLEAR( TCXO_PDM_CTL, PDM1_POLARITY)

#define PDM1_SET_INVERTED_POLARITY( ) \
    MSM_WB_SET( TCXO_PDM_CTL, PDM1_POLARITY)


/*===========================================================================

MACRO PDM2_SET_NORMAL_POLARITY
MACRO PDM2_SET_INVERTED_POLARITY


DESCRIPTION     These macros alter the polarity of PDM #2

DEPENDENCIES    These macros must be called within a MSM_LOCK / MSM_UNLOCK
                pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define PDM2_SET_NORMAL_POLARITY( ) \
    MSM_WB_CLEAR( TCXO_PDM_CTL, PDM2_POLARITY)

#define PDM2_SET_INVERTED_POLARITY( ) \
    MSM_WB_SET( TCXO_PDM_CTL, PDM2_POLARITY)


/*===========================================================================

MACRO PDM1_ENABLE
MACRO PDM1_DISABLE


DESCRIPTION     These macros enable and disable PDM #1

DEPENDENCIES    These macros must be called within a MSM_LOCK / MSM_UNLOCK
                pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define PDM1_ENABLE( ) \
    MSM_WB_SET( TCXO_PDM_CTL, PDM1_EN)

#define PDM1_DISABLE( ) \
    MSM_WB_CLEAR( TCXO_PDM_CTL, PDM1_EN)


/*===========================================================================

MACRO PDM2_ENABLE
MACRO PDM2_DISABLE


DESCRIPTION     These macros enable and disable PDM #2

DEPENDENCIES    These macros must be called within a MSM_LOCK / MSM_UNLOCK
                pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define PDM2_ENABLE( ) \
    MSM_WB_SET( TCXO_PDM_CTL, PDM2_EN)

#define PDM2_DISABLE( ) \
    MSM_WB_CLEAR( TCXO_PDM_CTL, PDM2_EN)


/*===========================================================================

MACRO PDM1_SET_PULSE_DENSITY

DESCRIPTION     This macro sets the density of pulses from PDM #1.  Assuming
                normal polarity:
                    0x80 - Low for 255/256 TCXO/4 pulses
                    0x00 - Low for 127/256 TCXO/4 pulses
                    0x7f - Low for   0/256 TCXO/4 pulses

DEPENDENCIES    None.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define PDM1_SET_PULSE_DENSITY( value ) \
    MSM_WB( PDM1_CTL, (value) )


/*===========================================================================

MACRO PDM2_SET_PULSE_DENSITY

DESCRIPTION     This macro sets the density of pulses from PDM #2.  Assuming
                normal polarity:
                    0x80 - Low for 255/256 TCXO/4 pulses
                    0x00 - Low for 127/256 TCXO/4 pulses
                    0x7f - Low for   0/256 TCXO/4 pulses

DEPENDENCIES    None.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define PDM2_SET_PULSE_DENSITY( value ) \
    MSM_WB( PDM2_CTL, (value) )


/*===========================================================================

                        MSM Clocks Control
                        
===========================================================================*/




/*===========================================================================

MACRO CLOCK_RESET_SLEEP_REGIME

DESCRIPTION     This macro resets the Sleep counter/watch-dog TCXO/4 clock
                regime.

DEPENDENCIES    This macro should be called within a MSM_LOCK / MSM_UNLOCK
                pair to ensure proper operation.
                
RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define CLOCK_RESET_SLEEP_REGIME( ) \
    MSM_WH( MSM_CLK_CTL3, MSM_CLK_CTL3_WH__PIN_TCXO_SYS_RESET_MASK), \
    MSM_WH( MSM_CLK_CTL3, 0x0000 );


/*===========================================================================

                        Version Register
                        
===========================================================================*/




/*===========================================================================

MACRO VERSION_GET

DESCRIPTION     This macro the MSM version number

DEPENDENCIES    None.
                
RETURN VALUE    The MSM version number.
      
SIDE EFFECTS    None.

===========================================================================*/

#define VERSION_GET( ) \
    ( MSM_RH( HW_REVISION_NUMBER ) & \
              HW_REVISION_NUMBER_RH_MASK );


/*===========================================================================

MACRO VERSION_GET_PROCESS_ID

DESCRIPTION     This macro the MSM process version number

DEPENDENCIES    None.
                
RETURN VALUE    The MSM process version number.
      
SIDE EFFECTS    None.

===========================================================================*/

#define VERSION_GET_PROCESS_ID( ) \
     ( MSM_RH_MASKED( HW_REVISION_NUMBER, PROCESS_ID );


/*===========================================================================

MACRO VERSION_GET_MSM_ID

DESCRIPTION     This macro the MSM id number

DEPENDENCIES    None.
                
RETURN VALUE    The MSM id number.
      
SIDE EFFECTS    None.

===========================================================================*/

#define VERSION_GET_MSM_ID( ) \
    ( MSM_RH( HW_REVISION_NUMBER ) & \
            HW_REVISION_NUMBER_RH__MSM_ID_MASK );


/*===========================================================================

MACRO VERSION_GET_VERSION_ID

DESCRIPTION     This macro the MSM version id number

DEPENDENCIES    None.
                
RETURN VALUE    The MSM version id number.
      
SIDE EFFECTS    None.

===========================================================================*/

#define VERSION_GET_VERSION_ID( ) \
    ( MSM_RH( HW_REVISION_NUMBER ) & \
            HW_REVISION_NUMBER_RH__VERSION_ID_MASK );


#ifdef T_MSM5000
/*===========================================================================

                          Tiger II Quick Paging Channel

===========================================================================*/


/*===========================================================================

MACRO QPCH_GET_SOFT_DECISION_I/Q
MACRO QPCH_GET_DECISION_THRESHOLD_I/Q

DESCRIPTION     These macros the QPCH soft decision values and thresholds for
                an OOK bit recieved on the Quick Paging Channel.
                
DEPENDENCIES    Information is only valid after the RX_QP_INT interrupt.
                
RETURN VALUE    Decision values (11 bits, signed)
      
SIDE EFFECTS    None.

===========================================================================*/

#define QPCH_GET_SOFT_DECISION_I( )  MSM_RH( QP_I_RD )
#define QPCH_GET_SOFT_DECISION_Q( )  MSM_RH( QP_Q_RD )

#ifdef T_MSM_5100
#error code not present
#else
#define QPCH_GET_DECISION_THRESHOLD_I( )  MSM_RH( QP_I_TH1 )
#define QPCH_GET_DECISION_THRESHOLD_Q( )  MSM_RH( QP_I_TH2 )
#endif


/*===========================================================================

MACRO QPCH_ARM_OOK_DECODE
MACRO QPCH_DISARM_OOK_DECODE

DESCRIPTION     These macros arm the QPCH OOK hardware to decode a bit at a
                given position, or disarm the OOK hardware.
                
                Position (9 bits) is specified in PN words (64 chips), for
                a range of upto 1 PN roll, and indicates the last PN word of
                the OOK bit.  Position is relative to the symbol combiner
                26.67ms roll position.
                
DEPENDENCIES    I/Q Channel configuration, symbol size, etc must be setup
                first for the QPCH channel.
                
                SYMB_COMB_CTL1:DESKEW_OFFSET needs correct offset.
                
RETURN VALUE    None.
      
SIDE EFFECTS    An "RX_QP_INT" interrupt will be generated within 26.7ms.
                QP_I_RD, QP_Q_RD, QP_I_TH1, and QP_I_TH2 will be modified.

===========================================================================*/

#define QPCH_ARM_OOK_DECODE( pos ) \
    MSM_WH( OOK_CTL, ( ( (pos) << 1) | OOK_CTL_WH__OOK_EN_MASK ) )

#define QPCH_DISARM_OOK_DECODE( ) \
    MSM_WH( OOK_CTL, 0x0000 )



#endif /* T_MSM5000 */


/*===========================================================================

                              END OF INTERFACE

===========================================================================*/

#endif /* MSM_DRV.H */

