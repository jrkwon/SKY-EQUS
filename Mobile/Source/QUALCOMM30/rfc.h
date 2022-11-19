#ifndef RFC_H
#define RFC_H
/* EJECT  */
/*===========================================================================

                      RF Card Driver Header File

DESCRIPTION
  This header file contains all the definitions necessary in order to
  interface with the RF Card Driver.  This interface is exclusive for the RF
  unit to manipulate the RF card (hardware).

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/rfc.h_v   1.1   10 Sep 1999 18:12:30   lchan  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/06/99   ychan   Modified for targets NGP, SURF and Q-1900.
01/06/99   ychan   Updated Copyright date.
03/24/98   kss     Adjusted RFC_THERM_60_DEG_C for the Module.
02/18/98   jjn     Added rfc_do_lna_call_back and rfc_do_125ms_isr to
                   facilitate work-arounds that are specific to WLL targets.
02/09/98   thh     Added support for the Q-1900 & Q-800.
                   Externalize rfc_retune_for_rxtx() and rfc_retune_for_rx()
                   in RF_CAP_RXTX_RETUNE scope.
02/03/98   kss     Partitioned Module RF to conform to new RF architecture
11/17/97   jjn     Partitioned ISS2 RF to conform to the new RF architecture
08/14/97   thh     Initial version.  Originally this was part of the "big"
                   RF driver.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rficap.h"
#include "comdef.h"

/* EJECT  */
/*===========================================================================

                            PUBLIC DEFINITIONS

===========================================================================*/

#if ((TG==T_MD) || (TG==T_G) || (TG==T_I2) || (TG==T_T) || (TG==T_O) || (TG==T_Q))
#define RFC_TUNE_CHAN_WAIT  (40+CLK_MS_PER_TICK)
  /* wait 40 ms after rfc_tune_to_chan()  */
#else
#error code not present
#endif

#if ( (TG==T_O) || (TG==T_PC) )
#error code not present
#elif (TG==T_Q)             
#error code not present
#elif (TG==T_G)                             
/* -----------------------------------------------------------------------
** With the thermistor scaled +85C (0) to -30C (255), this is the value
** which corresponds to +60C.  This value cannot be computed, as the
** curve is not linear and is not a known function.  This number is
** determined by examining a quantity of thermistor data taken from
** a "representative sample" of phones.
**
** Step 1) Get the thermistor ADC reading for +60C by looking at the
**         data.
**
** Step 2) Scale this over the thermistor min/max range.
**
**     (value - therm_min) * THERM_SCALED_MAX / (therm_max - therm_min)
** ----------------------------------------------------------------------- */

#define RFC_THERM_60_DEG_C       57

/* -------------------------------------------------------------------------
** FM power level at which to switch between the high & low power HDET
** ------------------------------------------------------------------------- */

#define RF_HDET_HIGH_RANGE_THRESHOLD    5

/* -------------------------------------------------------------------------
** FM power level at which to change PA R0 & R1 values
**
** For power levels 0..4, R1,R0 = 0,0
** For power levels 5..7, R1,R0 = 1,1
** ------------------------------------------------------------------------- */

#define RF_HIGH_POWER_R0R1_THRESHOLD    5

#elif (TG==T_T)
#error code not present
#elif (TG==T_MD)
#error code not present
#elif (TG==T_I2)
#error code not present
#else
#error code not present
#endif

/* EJECT  */


/* -------------------------------------------------------------------------

                            DATA TYPES

---------------------------------------------------------------------------- */

/* -----------------------------------------
** RF hardware configuration for this phone.
** ----------------------------------------- */

typedef enum {
  RFC_FM_CDMA_800MHZ,       /* 0 FM or CDMA at 800MHz with GRAS 1 or 2 */
  RFC_FM_CDMA_800MHZ_GRAS3, /* 1 FM or CDMA at 800MHz with GRAS 3 */
  RFC_CDMA_1800MHZ,         /* 2 CDMA only at 1800MHz with GRASP 0 */
  RFC_FM_CDMA_800MHZ_GRAS4, /* 3 FM or CDMA at 800MHz with GRAS 4 */
  RFC_CDMA_1800MHZ_GRASP1,  /* 4 CDMA only at 1800MHz with GRASP 1 */
  RFC_CDMA_1800MHZ_GRASP2,  /* 5 CDMA only at 1800MHz with GRASP 2 */
  RFC_FM_CDMA_800MHZ_GRAS5, /* 6 FM or CDMA at 800MHz with GRAS 5 */
  RFC_CDMA_1800MHZ_GRASP5   /* 7 CDMA only at 1800MHz with GRASP 5 */
} rfc_config_type;


/*===========================================================================

                           FUNCTION PROTORYPES

===========================================================================*/


/*===========================================================================

FUNCTION RFC_INIT_CARD_CDMA                                  INTERNAL FUNCTION

DESCRIPTION
  Initialize the RF card and its synthesizer for CDMA mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean rfc_init_card_cdma( void );


/*===========================================================================

FUNCTION RFC_INIT_CARD_PCS                                   INTERNAL FUNCTION

DESCRIPTION
  Initialize the RF card and its synthesizer for PCS mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean rfc_init_card_pcs( void );


/*===========================================================================

FUNCTION RFC_INIT_CARD_ANALOG                              INTERNAL FUNCTION

DESCRIPTION
  Initialize the RF card and its synthesizer for FM mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean rfc_init_card_analog( void );


/*===========================================================================

FUNCTION RFC_TUNE_SYNTH                                 INTERNAL FUNCTION

DESCRIPTION
  This function programs the synthesizer to tune to a specific channel.  The
  frequency of the channel is dependent on the current mode of the RF card.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
extern void rfc_tune_synth
(
  word chan       /* Channel to tune to */
);


/*===========================================================================

FUNCTION RFC_INIT_CARD                                       INTERNAL FUNCTION

DESCRIPTION
  This function initializes the synthesizer to the RF configuration.  It also
  performs other necessary intializations for the RF card.  And the it test-
  tunes the synthesizer to a low and high channel of the appropriate band.

DEPENDENCIES
  None

RETURN VALUE
  MC_OFFLINE_DIGITAL_F if then RF card could not be initialized.
  MC_ONLINE_F if the RF card was properly initialized.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean rfc_init_card
(
  byte rfc_config    /* RF configuration */
);


/*===========================================================================

FUNCTION RFC_CHAN_OFFSET                                     INTERNAL FUNCTION

DESCRIPTION
  Function to calculate offset from the channel with the lowest frequency.

DEPENDENCIES
  None

RETURN VALUE
  word value 0..831 representing the channel offset

SIDE EFFECTS
  None

===========================================================================*/
extern word rfc_chan_offset
(
  word channel          /* input channel number */
);

#ifdef RF_CAP_RXTX_RETUNE

/*===========================================================================

FUNCTION RFC_RETUNE_FOR_RXTX                                EXTERNAL FUNCTION

DESCRIPTION
  This function re-tunes the synthesizer for RxTx.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
extern void rfc_retune_for_rxtx( void );


/*===========================================================================

FUNCTION RFC_RETUNE_FOR_RX                                  EXTERNAL FUNCTION

DESCRIPTION
  This function re-tunes the synthesizer for Rx.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
extern void rfc_retune_for_rx( void );

#endif

#if (TG==T_I2)
#error code not present
#endif /* TG==T_I2 */
#endif /* RFC_H */

