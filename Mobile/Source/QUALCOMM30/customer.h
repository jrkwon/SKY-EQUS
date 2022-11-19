#ifndef CUSTOMER_H
#define CUSTOMER_H
/*===========================================================================

                   C U S T O M E R    H E A D E R    F I L E

DESCRIPTION
  This header file provides customer specific information for the current
  build.  It expects the compile time switch /DCUST_H=CUSTxxxx.H.  CUST_H
  indicates which customer file is to be used during the current build.
  Note that cust_all.h contains a list of ALL the option currently available.
  The individual CUSTxxxx.H files define which options a particular customer
  has requested.


Copyright (c) 1996, 1997 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/customer.h_v   1.5   14 Jul 2000 17:29:54   tsummers  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/06/00   ts      Added checks for descrepancies with RUIM Driver feature 
                   switches.
04/12/00   va      New checks for SIO feature dependencies.
01/18/00   lcc     Removed FEATURE_SPECIAL_MDR which is not supported in this
                   archive.
09/16/99   lcc     Merged in changes to support UART clocks for MSM5000 from
                   PLTarchive.
08/05/99   sh      added check for FEATURE_PRL_FORCE_MODE not defined
05/27/99   kmp     Removed the check for FEATURE_UI_OTKSL and FEATURE_JSTD008.
                   They should not be linked.
04/21/99   kjn     Removed checking for language features.
04/05/99   kjn     Removed checking for FEATURE_PRL_DLOAD.
02/12/99   lcc     Added checking for FEATURE_SPECIAL_MDR.
01/16/99   ram     Merged in the changes from MDR branch.
12/08/98   lcc     Added checking for FEATURE_MDR_DPT_MARKOV and
                   FEATURE_IS95B_MINUS.
11/30/98   lcc     Added header keyword and checking for MDR feature.
12/16/98   jct     Removed support for outdated or unused targets, removed
                   target specific revision history
04/07/98   BK      Corrected a syntax error and copyright notice.
04/06/98   BK      Ensure that FEATURE_UI_ENHANCED_PRIVACY_SET and
                   FEATURE_VOICE_PRIVACY are mutually exclusive.
03/11/96   fkm     Created file.

===========================================================================*/


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Make sure that CUST_H is defined and then include whatever file it
** specifies.
*/
#ifdef CUST_H
#include CUST_H
#else
#error Must Specify /DCUST_H=CUSTxxxx.H on the compile line
#endif

/* Now perform certain Sanity Checks on the various options and combinations
** of option.  Note that this list is probably NOT exhaustive, but just
** catches the obvious stuff.
*/

#if defined( FEATURE_UI_ENHANCED_PRIVACY_SET ) && defined( FEATURE_VOICE_PRIVACY_SWITCH)
#error code not present
#endif

#if defined(FEATURE_SMS) != defined(FEATURE_UI_SMS)
#error defined(FEATURE_SMS) != defined(FEATURE_UI_SMS)
#endif

#if defined(FEATURE_UI_TIME_YES) && defined(FEATURE_ACP)
#error defined(FEATURE_UI_TIME_YES) && defined(FEATURE_ACP)
#endif

#if defined(FEATURE_DS_AMPS) && !defined(FEATURE_ACP)
#error defined(FEATURE_DS_AMPS) && !defined(FEATURE_ACP)
#endif

#if defined(FEATURE_UI_FORCE_CALL) && !defined(FEATURE_ACP)
#error defined(FEATURE_UI_FORCE_CALL) && !defined(FEATURE_ACP)
#endif

#if defined(FEATURE_UI_SEND_PIN) && !defined(FEATURE_ACP)
#error defined(FEATURE_UI_SEND_PIN) && !defined(FEATURE_ACP)
#endif

#if defined(FEATURE_UI_IMSI_ALL_0_OK) && defined(FEATURE_OTASP)
#error defined(FEATURE_UI_IMSI_ALL_0_OK) && defined(FEATURE_OTASP)
#endif

#if defined(FEATURE_PREFERRED_ROAMING) && \
    !defined(FEATURE_CDMA_1900) && !defined(FEATURE_CDMA_800)
#error !defined(FEATURE_CDMA_1900) && !defined(FEATURE_CDMA_800)
#endif

#if defined(FEATURE_IS95B_MDR) && !defined(FEATURE_DS)
#error defined(FEATURE_IS95B_MDR) && !defined(FEATURE_DS)
#endif

#if defined(FEATURE_MDR_DPT_MARKOV) && !defined(FEATURE_IS95B_MDR)
#error code not present
#endif

#if defined(FEATURE_DIAG_RPC) && !defined(FEATURE_NEW_SIO)
#error code not present
#endif

//#if defined(FEATURE_UART_CLK_TEST_INPUT) && !defined(T_MSM5000)
#if defined(FEATURE_UART_CLK_TEST_INPUT) && !defined(T_MSM5000)
#error FEATURE_UART_CLK_TEST_INPUT is only supported on MSM5000
#endif

//#if defined(FEATURE_UART_CLK_TEST_INPUT) && !defined(T_MSM5000)
#if defined(FEATURE_UART_TCXO_CLK_FREQ) && !defined(T_MSM5000)
#error FEATURE_UART_TCXO_CLK_FREQ is only supported on MSM5000
#endif

#if defined(FEATURE_UART_TCXO_CLK_FREQ) && !defined(FEATURE_NEW_SIO)
#error must define FEATURE_NEW_SIO to use FEATURE_UART_TCXO_CLK_FREQ
#endif

#if defined(T_MSM5000) && !defined(FEATURE_IS2000)
#error MSM5000 requires FEATURE_IS2000
#endif

#if !defined(FEATURE_PRL_FORCE_MODE) && ( defined(FEATURE_CDMA_800) || defined(FEATURE_ACP) )
#error !defined(FEATURE_PRL_FORCE_MODE) && ( defined(FEATURE_CDMA_800) || defined(FEATURE_ACP) )
#endif

#if defined(FEATURE_SDEVMAP_SHARE_HFK) && \
   !defined(FEATURE_SECOND_UART)
#error FEATURE_SDEVMAP_SHARE_HFK requires FEATURE_SECOND_UART
#endif

#if defined(FEATURE_SDEVMAP_UI_MENU) && \
   !defined(FEATURE_SERIAL_DEVICE_MAPPER)
#error FEATURE_SDEVMAP_UI_MENU requires FEATURE_SERIAL_DEVICE_MAPPER
#endif

#if defined(FEATURE_SDEVMAP_MENU_ITEM_NV) && \
   !defined(FEATURE_SDEVMAP_UI_MENU)
#error FEATURE_SDEVMAP_MENU_ITEM_NV requires FEATURE_SDEVMAP_UI_MENU
#endif

#if defined(FEATURE_SDEVMAP_UI_MENU) && \
   !defined(FEATURE_SDEVMAP_MENU_ITEM_NV)
#error FEATURE_SDEVMAP_UI_MENU requires FEATURE_SDEVMAP_MENU_ITEM_NV
#endif

#if defined(FEATURE_SDEVMAP_SHARE_HFK) && \
   !defined(FEATURE_SERIAL_DEVICE_MAPPER)
#error FEATURE_SDEVMAP_SHARE_HFK requires FEATURE_SERIAL_DEVICE_MAPPER
#endif

#if defined(FEATURE_DS_UI_BAUD) && \
   !defined(FEATURE_DS_DEFAULT_BITRATE_NV)
#error FEATURE_DS_UI_BAUD requires FEATURE_DS_DEFAULT_BITRATE_NV
#endif

#if defined(FEATURE_DS_DEFAULT_BITRATE_NV) && \
   !defined(FEATURE_DS_UI_BAUD)
#error FEATURE_DS_DEFAULT_BITRATE_NV requires FEATURE_DS_UI_BAUD
#endif

#if defined(FEATURE_DIAG_UI_BAUD) && \
   !defined(FEATURE_DIAG_DEFAULT_BITRATE_NV)
#error FEATURE_DIAG_UI_BAUD requires FEATURE_DIAG_DEFAULT_BITRATE_NV
#endif

#if defined(FEATURE_DIAG_DEFAULT_BITRATE_NV) && \
   !defined(FEATURE_DIAG_UI_BAUD)
#error FEATURE_DIAG_DEFAULT_BITRATE_NV requires FEATURE_DIAG_UI_BAUD
#endif

/* Required & Invalid features for RUIM */
/* Make sure the second UART is not defined with the RUIM driver */
#if defined ( FEATURE_RUIM_DRIVER ) && defined (FEATURE_SECOND_UART)
  #error defined ( FEATURE_RUIM_DRIVER ) && defined (FEATURE_SECOND_UART)
#endif /* FEATURE_RUIM_DRIVER */

/*
   Make sure the RUIM time test does not interfere with the TRAMP time 
   test
 */
#if defined (RUIM_DRIVER_TIME_TEST) && defined (TRAMP_INT_TIMING_TEST)
  #error defined (RUIM_DRIVER_TIME_TEST) && defined (TRAMP_INT_TIMING_TEST)
#endif /* RUIM_DRIVER_TIME_TEST */


#endif /* CUSTOMER_H */

