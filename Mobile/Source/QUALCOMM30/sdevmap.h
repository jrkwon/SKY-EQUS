
#ifndef SDEVMAP_H
#define SDEVMAP_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          S E R I A L   D E V I C E   M A P P E R   S E R V I C E S 

                         H E A D E R    F I L E
                            

GENERAL DESCRIPTION
  This file contains types and declarations associated with the DMSS
  Serial Device Mapper (SDM).  
  
EXTERNALIZED FUNCTIONS

  sdm_init()
    Initialize Serial Device Mapper Services

  sdm_get_port_id()
    Returns the current SIO Port mapped to the specified Application.
                                                     
  sdm_uart2_opened()
    Informs the Serial Device Mapper that the second UART has been opened.

  sdm_uart2_closed()
    Informs the Serial Device Mapper that the second UART has been closed.

  sdm_mc_dev_map_from_nv()
    Stores the port map retrieved from NV
    
INITIALIZATION AND SEQUENCING REQUIREMENTS

  sdm_init() must be called prior to any of the other functions of
  this module.
                 
Copyright (c) 1999, 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/COMMON/vcs/sdevmap.h_v   1.2   15 Dec 2000 11:03:44   dpendlet  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/15/00   dwp     Make SDM port map interfaces mutually exclusive with
                   FEATURE_RUNTIME_DEVMAP.
11/14/00   dwp     Added support for FEATURE_RUNTIME_DEVMAP.
07/27/00   dwp     Added support for FEATURE_USB_DIAG and FEATURE_BT.
07/05/00   jwh     Fixed featurization with regard to FEATURE_DS, etc.
04/09/00   jwh     Added sdm_mc_dev_map_from_nv().
04/08/00   jwh     Added sdm_config_type, sdm_uart2_opened() and 
                   sdm_uart2_closed().
10/18/99   jwh     Initial Release.
10/05/99   jwh     Created module.

===========================================================================*/


/*===========================================================================

                            INCLUDE FILES

===========================================================================*/

#include "customer.h"              /* Customer Specific Definitions        */
#include "sio.h"                   /* Serial I/O Services.                 */

#ifdef FEATURE_SERIAL_DEVICE_MAPPER

/*===========================================================================

                     CONSTANT AND VARIABLE DECLARATIONS

===========================================================================*/

#ifndef FEATURE_RUNTIME_DEVMAP
/*---------------------------------------------------------------------------
  SDM Application Identifier type.  Allows Apps to be mapped to an SIO Port.
  These must exist regardless of FEATURE set.
---------------------------------------------------------------------------*/
typedef enum 
{
  SDM_APP_DIAG = 0,                /* Diagnostic Application               */
  SDM_APP_DS,                      /* Data Services Application            */
#ifdef FEATURE_BT
#error code not present
#endif
  SDM_APP_MAX                      /* For bounds checking only             */
} sdm_appl_id_type;


/*---------------------------------------------------------------------------
  SDM Configuration Identifier type.  Allows port mappings to be stored into
  a byte.  The "ILLEGAL" entries preserve enum values independent of
  feature set and allow compile time checking.
---------------------------------------------------------------------------*/
typedef enum 
{
  SDM__BOGUS_CONFIG = -1,

#if defined(FEATURE_DS) && defined(FEATURE_SECOND_UART)
  SDM__U1_DS____U2_DIAG = 0,
#else
  SDM__ILLEGAL_CONFIG_1,
#endif

#ifdef FEATURE_DS
  SDM__U1_DS____U2_NULL,
#else
  SDM__ILLEGAL_CONFIG_2,
#endif

#ifdef	MODEL_SKTT
  SDM__U1_DIAG__U2_NULL,
#else	//	#ifdef	MODEL_SKTT
#if !defined(FEATURE_DS)
  SDM__U1_DIAG__U2_NULL,
#else
  SDM__ILLEGAL_CONFIG_3,
#endif
#endif	//	#ifdef	MODEL_SKTT

#if defined(FEATURE_USB_CDC_ACM) && defined(FEATURE_DS)
#error code not present
#else
  SDM__ILLEGAL_CONFIG_4,
#endif

#if defined(FEATURE_USB_DIAG) && defined(FEATURE_DS)
#error code not present
#else
  SDM__ILLEGAL_CONFIG_5,
  SDM__ILLEGAL_CONFIG_6,
#endif

#if defined(FEATURE_BT) && defined(FEATURE_DS)
#error code not present
#else
  SDM__ILLEGAL_CONFIG_7,
  SDM__ILLEGAL_CONFIG_8,
#endif

  SDM__MAP_MAX                      /* For bounds checking only             */
} sdm_config_type;
#endif /* FEATURE_RUNTIME_DEVMAP */



/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION SDM_INIT                                     EXTERNALIZED FUNCTION

DESCRIPTION
  Initializes the Serial Device Mapper Services

DEPENDENCIES
  Should be called just once.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void sdm_init
(
  void
); 


#ifndef FEATURE_RUNTIME_DEVMAP
/*===========================================================================

FUNCTION SDM_GET_PORT_ID                                  REGIONAL FUNCTION

DESCRIPTION
  Return Port ID which is currently assigned to the specified Application.
      
DEPENDENCIES
  None
                                                       
RETURN VALUE
  SIO Port currently mapped to the specified Application.
  If no Port identifier is currently assigned, returns SIO_PORT_NULL.
    
SIDE EFFECTS
  None
  
===========================================================================*/

extern sio_port_id_type sdm_get_port_id
(
  sdm_appl_id_type app_id           /* Application to be mapped to Port ID */
);
#endif /* FEATURE_RUNTIME_DEVMAP */


#if defined(FEATURE_SDEVMAP_SHARE_HFK) && defined(FEATURE_SECOND_UART)
/*===========================================================================

FUNCTION SDM_UART2_OPENED                             EXTERNALIZED FUNCTION

DESCRIPTION
  Informs the Serial Device Mapper that the second UART has been opened.
  Acquires resources being used by others - e.g., Hands Free Kit.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the CODEC_CTL MUX to select the second UART.
  Also see hs_enable_hfk_pins().

===========================================================================*/

void sdm_uart2_opened( void );


/*===========================================================================

FUNCTION SDM_UART2_CLOSED                             EXTERNALIZED FUNCTION

DESCRIPTION
  Informs the Serial Device Mapper that the second UART has been closed.
  Allows resources to be used by others - e.g., Hands Free Kit.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the CODEC_CTL MUX to select the HFK.
  Also see hs_enable_hfk_pins().

===========================================================================*/

void sdm_uart2_closed( void );

#endif  /* FEATURE_SDEVMAP_SHARE_HFK && FEATURE_SECOND_UART */

#ifndef FEATURE_RUNTIME_DEVMAP
#ifdef FEATURE_SDEVMAP_MENU_ITEM_NV
/*===========================================================================

FUNCTION SDM_MC_DEV_MAP_FROM_NV                       EXTERNALIZED FUNCTION

DESCRIPTION
  Stores the port map retrieved from NV.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void sdm_mc_dev_map_from_nv
(
  int1          nv_port_map
);
#endif /* FEATURE_SDEVMAP_MENU_ITEM_NV */
#endif /* FEATURE_RUNTIME_DEVMAP */


#endif /* FEATURE_SERIAL_DEVICE_MAPPER */

#endif /* SDEVMAP_H */

