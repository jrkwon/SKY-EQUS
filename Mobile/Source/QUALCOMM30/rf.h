#ifndef RF_H
#define RF_H
/*===========================================================================

                      RF Unit Header File

DESCRIPTION
  This header file contains all the definitions necessary in order to
  interface with the RF Unit.  The RF unit provides interface calls
  for accessing the analog common card and PCN applique synthesizers
  and other miscellaneous items.

Copyright (c) 1991, 1992, 1993 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 1994, 1995, 1996 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/rf.h_v   1.3   03 Apr 2000 12:09:50   varavamu  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/03/00   va      Added prototype for rf_digital_band_class_select()
03/16/00   sr      Added prototype for rx_tx_gain_adj_step_size().
09/26/99   bgc     Added functions rf_set_frq_trk_acc, rf_get_frq_trk_acc
01/06/99   ychan   Modified for targets NGP, SURF and Q-1900.
01/06/99   ychan   Updated Copyright date.
07/10/98   kss     Added RF_WARMUP_TIME_MS.
06/25/98   kss     Added external declaration for rf_complete_warmup().
05/14/98   bgc     Added RF_CODEC_RF Mask to rf_codec_on() enum..
12/08/97   thh     Added external declaration for rf_enable_cdma_temp_comp().
09/26/97   thh     Removed RF NV external declaration.  This and other changes
                   to support the new interface from the RF driver breakup.
07/17/97   thh     Added extern declaration for rf_tx_start() and rf_tx_stop().
06/17/97   dhh     Deleted target conditions #if (TG==T_G) ... and added
                   T_Q for Q Phone.
03/14/97   jjn     Added (TG==T_MD) where ever appropriate for the Module
01/10/97   fkm     Added RF_CODEC_HS Mask to rf_codec_on() enum.
12/10/96   thh     Added support for TGP by defining T_T.
09/25/96   dak     Added T_I2 to some ifdefs to support the updated RF driver
                   for ISS2.
07/16/96   thh     Add extern declaration for rf_cdma_temp_comp_init() and
                   rf_cdma_do_temp_comp().
12/22/95   jah     Changed rf_get_rssi() comments to be generic
10/25/95   jah     Changed rf_init_delayed() to return mc_name_type.
                   Added rf_post_cdma_reacq() for drooping Rx AGC bug.
06/28/95   jca     Added RF_DFM_FADE_OR_SCAN to rf_dfm_mode_type.
05/23/95   tst     Added rf_init_delayed().
05/16/95   tst     Added RF_CODEC_MC.
05/12/95   tst     Removed conditional compile for rf_get_rssi().  Now all
                   targets use "word" for *raw_rssi.
05/04/95   tst     Fixed comments for rf_get_cdma_tx_pwr_limit
05/03/95   tst     Fixed comments for rf_get_cdma_rx_agc() and
                   rf_get_cdma_tx_agc().
04/17/95   tst     Changed RF_TUNE_CHAN_WAIT to 40ms for Gemini and ISS2.
04/11/95   tst     Added rf_codec_type and associated constants for use in
                   modified rf_codec_on() "caller ID" plan.
03/29/95   tst     Changed #if logic for *raw_rssi byte/word definition.
03/27/95   tst     Changed rf_get_rssi() *raw_rssi from byte to word.
03/21/95   tst     Added function rf_set_dac().
03/02/95   tst     Added functions rf_get_cdma_tx_agc() and
                   rf_get_cdma_tx_pwr_limit().
12/15/94   tst     Fixed comment and if statement for rf_ratchet_mode() to
                   include ISS1.
12/01/94   tst     Fixed comment for rf_get_rssi() scaled_rssi.
11/28/94   tst     Incorporated comments from readiness review.
11/16/94   tst     Added target.h include file and conditional compile around
                   MSM2P-only functions.
11/15/94   tst     Added rf_get_cdma_rx_agc().
11/09/94   tst     Added rf_set_dfm_mode() and enums for mode definition.
11/08/94   tst     Added rf_stay_asleep() to actually set SLEEP/ low.
                   Updated dependencies and side effects for functions
                   which depend on and modify data base items.
                   Added rf_codec_on().
11/03/94   tst     Added RF_CODEC_WAIT_TIME.  Corrected comments for
                   RF_PA_WAIT, RF_PA_LEVEL_WAIT, and RF_ENABLE_PWR_WAIT.
03/16/94   jca     Changed interface to function rf_get_rssi().
12/09/93   jah     Changed WAIT times to account for tick granularity.
07/01/93   jca     Added function rf_ratchet_mode for Beta 2 target.
06/11/93   jca     Added new functions to support powering on RF hardware
                   and then delaying before programming synthesizers.
05/13/93   jca     Removed rf_mode parameters from rf_tx_enable().
05/10/93   jca     Added missing include.  Changed name of procedure
                   rf_put_nv_item() to rf_read_nv_item().
03/26/93   jca     Added rf_sleep() procedure.
02/16/93   jca     Added rf_put_nv_item() procedure.
01/14/93   arh     Removed rf_shutdown() procedure.
12/28/92   jca     Added new functions to support portable operation.
07/22/92   arh     Creation

===========================================================================*/
#include "rficap.h"
#include "comdef.h"
#include "rfc.h"
#include "nv.h"
#include "clk.h"
#include "target.h"
#include "mc.h"

/* EJECT  */
/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/

#define RF_PA_WAIT         ( 5+CLK_MS_PER_TICK)
  /* wait  5 ms after rf_pa_on()         */

#define RF_PA_LEVEL_WAIT   (10+CLK_MS_PER_TICK)
  /* wait 10 ms after rf_set_pa_level()  */

#define RF_ENABLE_PWR_WAIT ( 5+CLK_MS_PER_TICK)
  /* wait  5 ms after rf_enable_analog_rx_pwr(),
     rf_enable_digital_rx_pwr() or rf_enable_tx_pwr() */

#define RF_CODEC_WAIT      ((word)10)
  /* wait 10 us for BB2 hardware to settle before using CHIPX8 */

#define RF_TUNE_CHAN_WAIT  RFC_TUNE_CHAN_WAIT
  /* time to wait after tuning to a channel */

/* ----------------------------------------------------------------
** These enums define the legal mode settings for rf_set_dfm_mode()
** ---------------------------------------------------------------- */
typedef enum {
  RF_DFM_ACQUISITION,
    /* digital FM acquisition mode - wide loop bandwidth for fast
       DPLL acquisition */
  RF_DFM_TRACKING,
    /* digital FM tracking mode - narrow loop bandwidth for lowest noise
       DPLL tracking */
  RF_DFM_FADE_OR_SCAN
    /* Tracking temporarily disabled during channel scan or channel fade */
} rf_dfm_mode_type;

/* --------------------------------------------------------------------
** CHIPX8 control, rf_codec_on(), is "voted", so that multiple callers
** can ask for CHIPX8 and codec power on and off separately.  A counting
** semaphore was not appropriate due to the problems with the caller
** counting enables/disables properly.  Each caller has a specifed mask
** which identifies it.
** -------------------------------------------------------------------- */
typedef enum {
  RF_CODEC_NONE = 0x00,  /* disabled */
  RF_CODEC_VOC  = 0x01,  /* codec enable bit for the vocoder task */
  RF_CODEC_SND  = 0x02,  /* codec enable bit for the sound task   */
  RF_CODEC_MC   = 0x04,  /* codec enable bit for MC               */
  RF_CODEC_HS   = 0x08,  /* codec enable bit for the handset task */
  RF_CODEC_RF   = 0x10   /* codec enable bit for the rf driver    */
#ifdef MMI_TREX
  ,
  RF_CODEC_OKI   = 0x20   /* codec enable bit for the rf driver    */
#endif
} rf_codec_type;

/* EJECT  */
/*===========================================================================

FUNCTION RF_GET_RSSI

DESCRIPTION
  This function returns the signal strength for the current channel.

DEPENDENCIES
  None

RETURN VALUE
  raw_rssi    - relative RSSI value

  scaled_rssi - calibrated RSSI value scaled 0..255 over the operating range

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_get_rssi
(
  word *raw_rssi,
    /* raw RSSI value */
  word *scaled_rssi
    /* Calibrated RSSI value */
);

/* EJECT  */
/*===========================================================================

FUNCTION RF_TUNE_TO_CHAN

DESCRIPTION
  This function tunes the synthesizer to the specified channel.  Once
  rf_chan_tune has been called, a delay of RF_CHAN_TUNE_WAIT msecs must
  be performed by the application to allow the synthesizer to settle.
  The channel can be an analog channel number, a CDMA channel number or
  a PCN channel number.  The channel is interpreted according to
  the current mode of operation.

DEPENDENCIES
  database item RF_MODE must be setup prior to call

RETURN VALUE
  None

SIDE EFFECTS
  The database item (DB_CHANNEL) is updated to the new channel

===========================================================================*/
extern void rf_tune_to_chan
(
  word chan          /* Channel number to tune to                          */
);

/* EJECT  */
/*===========================================================================

FUNCTION RF_PA_ON

DESCRIPTION
  This function turns on (TRUE) or off (FALSE) the Power Amplifier.  A
  delay of RF_PA_WAIT milliseconds must be performed by the application
  after power ON to allow the hardware to settle.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_pa_on
(
  boolean pa_state                      /* TRUE or FALSE */
);

/* EJECT  */
/*===========================================================================

FUNCTION RF_SET_PA_LEVEL

DESCRIPTION
  This function sets the power level to one of the 8 power levels (0-7)
  described in CAI Table 2.1.2-1.

  After calling function rf_set_pa_level(), a delay of RF_PA_LEVEL_WAIT
  milliseconds must be performed by the application to allow the hardware
  to settle.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The database item DB_POWER is set to the new level

===========================================================================*/
extern void rf_set_pa_level
(
  byte pa_level                     /* Power level value (0-7) */
);

/* EJECT  */
/*===========================================================================

FUNCTION RF_INIT

DESCRIPTION
  Initializes the RF unit. This procedure should be called once
  upon power-up.  The database item DB_RF_MODE is updated to the
  current hardware configuration found (e.g., if PCN board is found,
  then the database item DB_RF_MODE is updated to DB_RF_PCN).

  For Gemini, initialization is as follows:
      rf_init()
      load all RF NV items
      rf_init_delayed()

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The database item DB_RF_MODE will be updated to DB_RF_PCN or DB_RF_CDMA
  depending on the current hardware configuration.

===========================================================================*/
extern void rf_init( void );

/* EJECT  */

/*===========================================================================

FUNCTION RF_INIT_DELAYED

DESCRIPTION
  Gemini only.  Finished initializing the RF unit. This procedure should be
  called after all RF NV items have been loaded.  The database item DB_RF_MODE
  is updated to the current hardware configuration found (e.g., if PCN board
  is found, then the database item DB_RF_MODE is updated to DB_RF_PCN).

  For Gemini, initialization is as follows:
      rf_init()
      load all RF NV items
      rf_init_delayed()

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The database item DB_RF_MODE will be updated to DB_RF_PCN or DB_RF_CDMA
  depending on the current hardware configuration.

===========================================================================*/
extern mc_name_type rf_init_delayed( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_INIT_ANALOG

DESCRIPTION
  Programs the RF RX synthesizers for analog mode of operation. This
  procedure should be called every time analog mode is entered.

DEPENDENCIES
  database item RF_MODE must be setup prior to call for "old mode"
  Procedure rf_enable_analog_rx_pwr() must have been called before calling
  this procedure.

RETURN VALUE
  None

SIDE EFFECTS
  The database item DB_RF_MODE is set to DB_RF_ANALOG.

===========================================================================*/
extern void rf_init_analog( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_INIT_DIGITAL

DESCRIPTION
  Programs the RF RX synthesizers unit for digital mode of operation
  (CDMA or PCN).  This procedure should be called every time digital
  mode is entered.

DEPENDENCIES
  database item RF_MODE must be set
  The MSM1 core AGC hardware should have been disabled first
  Procedure rf_enable_digital_rx_pwr() must have been called before calling
  this procedure.

RETURN VALUE
  None

SIDE EFFECTS
  The database item DB_RF_MODE is set to DB_RF_PCN or DB_RF_CDMA
  depending on the current hardware configuration.

===========================================================================*/
extern void rf_init_digital( void );

/* EJECT  */

/*===========================================================================

FUNCTION RF_COMPLETE_WARMUP

DESCRIPTION
  This function completes RF initialization after sleep by switching back
  into digital mode.  It should run after the expiration of the warmup
  timer.

DEPENDENCIES
  rf_init_digital() must have been called earlier.
  The synthesizers should be in lock.
  The CDMA mode PDM clocks must be enabled.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_complete_warmup( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_GET_FRQ_TRK_ACC

DESCRIPTION    This function returns the current value of the frequency
      tracking accumulator.

DEPENDENCIES   None.

RETURN VALUE   The accumulator value.

SIDE EFFECTS   None.

===========================================================================*/
word  rf_get_frq_trk_acc ( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_SET_FRQ_TRK_ACC

DESCRIPTION    This function sets the value of the carrier frequency
      tracking accumulator.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void  rf_set_frq_trk_acc
(
  word accum
    /* The full 16-bit accumulator value we want to write */
);

/* EJECT  */
/*===========================================================================

FUNCTION RF_TX_ENABLE

DESCRIPTION
  This function programs the VHF transmit synthesizer for analog
  or digital (CDMA or PCN) mode of operation.

  Once rf_tx_enable has been called, a delay of RF_TUNE_CHAN_WAIT
  milliseconds must be performed by the application to allow the
  VHF transmit synthesizer to settle.  After the VHF transmit sythesizer
  has settled, only then should the PA be enabled.

DEPENDENCIES
  database item RF_MODE must be set
  Procedure rf_enable_tx_pwr() must have been called before calling
  this procedure.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_tx_enable( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_TX_DISABLE

DESCRIPTION
  This function turns off the VHF transmit synthesizer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_tx_disable( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_SLEEP

DESCRIPTION
  This function sets the database item RF_MODE to DB_RF_SLEEP and configures
  the RF Rx circuits to save power.  Actual control of the SLEEP/
  signal is done by rf_stay_asleep().  This function should be called when
  the sleep state is entered so the database will be updated.  The SLEEP/
  signal control is not done here because SLEEP/ must remain high until the
  MSM is done with CHIPX8.

DEPENDENCIES
  This routine is only called during CDMA operation.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_sleep( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_STAY_ASLEEP

DESCRIPTION
  This function sets the SLEEP/ signal low.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_stay_asleep( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_ENABLE_ANALOG_RX_PWR

DESCRIPTION
  This function turns on the FM receive portion of the RF hardware.

  Once rf_enable_analog_rx_pwr() has been called, a delay of
  RF_ENABLE_PWR_WAIT milliseconds must be performed by the
  application to allow the RF hardware to settle.  After the
  RF hardware has settled, only then should the rf_init_analog()
  function be called.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_enable_analog_rx_pwr( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_ENABLE_DIGITAL_RX_PWR

DESCRIPTION
  This function turns on the digital receive portion of the RF hardware.

  Once rf_enable_digital_rx_pwr() has been called, a delay of
  RF_ENABLE_PWR_WAIT milliseconds must be performed by the
  application to allow the RF hardware to settle.  After the
  RF hardware has settled, only then should the rf_init_digital()
  function be called.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_enable_digital_rx_pwr( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_ENABLE_TX_PWR

DESCRIPTION
  This function turns on the transmit portion of the RF hardware.

  Once rf_enable_tx_pwr() has been called, a delay of RF_ENABLE_PWR_WAIT
  milliseconds must be performed by the application to allow the RF
  hardware to settle.  After the RF hardware has settled, only then should
  the rf_tx_enable() function be called.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_enable_tx_pwr( void );

/* <EJECT>  */
#if ((TG==T_B2)||(TG==T_I1))
#error code not present
#endif /* ((TG==T_B2)||(TG==T_I1)) */

/* EJECT  */

#ifdef T_MSM2P

/*===========================================================================

FUNCTION RF_SET_DFM_MODE

DESCRIPTION
  Sets the digital FM hardware for either acquisition mode
  (RF_DFM_ACQUISITION) or tracking mode (RF_DFM_TRACKING).

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_set_dfm_mode
(
  rf_dfm_mode_type dfm_mode      /* new DFM hardware operating mode to set */
);

/* EJECT  */

/* EJECT  */
/*===========================================================================

FUNCTION RF_GET_CDMA_RX_AGC

DESCRIPTION
  This function returns the CDMA receive AGC setting in 8 bit two's
  complement format.

  WITH Rx LINEARIZER SETUP FOR -106dBm..-20.75dBm, the Rx signal strength is:

    Rx strength in dBm = -63.248 + (( rf_get_cdma_rx_agc() * 334 ) / 1000 )

    That's -0.334dB per count referenced to -63.248dBm.  Since this returns
    int1, this is a numeric Rx signal strength from -106dBm to -20.83dBm.

DEPENDENCIES
  None

RETURN VALUE
  int1 - 8 bit two's complement CDMA Rx AGC value

SIDE EFFECTS
  None

===========================================================================*/
extern int1 rf_get_cdma_rx_agc( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_GET_CDMA_TX_AGC

DESCRIPTION
  This function returns the CDMA Tx AGC setting in 10 bit binary
  format.

  WITH Tx LINEARIZER SETUP FOR -52.25dBm..+33dBm, the Tx signal strength is:

    Tx strength in dBm = -52.25 + (( rf_get_cdma_rx_agc() * 0833 ) / 10000 )

    That's +0.085dB per count referenced to -52.25dBm.  Since this returns
    word, this is a numeric Tx signal strength from -52.25dBm to +32.97dBm.

DEPENDENCIES
  None

RETURN VALUE
  word = 10 bit Tx AGC value

SIDE EFFECTS
  None

===========================================================================*/
extern word rf_get_cdma_tx_agc( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_GET_CDMA_TX_PWR_LIMIT

DESCRIPTION
  This function returns the CDMA Tx power limit setting in 10 bit binary
  format.  Since the hardware register is not directly readable, this
  functions returns a copy of the last setting performed by the RF unit.
  The returned value is interpreted as follows:

    Returned value               0........1023
    Tx power limit          -52.25........+31dBm
    Tx power limit register   +511........-512

DEPENDENCIES
  None

RETURN VALUE
  word = 10 bit Tx power limit value

SIDE EFFECTS
  None

===========================================================================*/
extern word rf_get_cdma_tx_pwr_limit( void );

/* EJECT  */
/*===========================================================================

FUNCTION RF_CODEC_ON

DESCRIPTION
  Enables/disables power to the BB2 CHIPX8 divider, the BB2 Rx circuits, and
  the CODEC.  TRUE turns the power on.  FALSE turns it OFF if IDLE mode and
  leaves it ON if RxTx mode.  A delay of RF_CODEC_WAIT must be performed by
  the application after power ON to allow the hardware to settle.  An "OR"
  of all possible callers is performed for determining if codec and CHIPX8
  power are needed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_codec_on
(
  rf_codec_type mask,   /* mask to identify callers individually       */
  boolean codec_enable  /* TRUE to turn power on, FALSE to turn it off */
);


/*===========================================================================

FUNCTION RF_POST_CDMA_REACQ                             EXTERNALIZED FUNCTION

DESCRIPTION
  Perform RF initialization required following CDMA pilot re-acquisition.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_post_cdma_reacq( void );


/*===========================================================================

FUNCTION RF_CDMA_TEMP_COMP_INIT                             INTERNAL FUNCTION

DESCRIPTION
  This function simply saves the address of the timer block.  This address
  is passed by a task.  There is an understanding between this task and
  us (RF driver) that when this timer block expires, the task will call
  the function rf_cdma_temp_comp_start so that the RF driver may start the
  process of adjusting the linearizers based on the temperature.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Updates temp_cal_timer_blk

===========================================================================*/
extern void rf_cdma_temp_comp_init
(
  rex_timer_type *timer_ptr    /* The timer block to control call backs */
);


/*===========================================================================

FUNCTION RF_CDMA_DO_TEMP_COMP                               INTERNAL FUNCTION

DESCRIPTION
  This function does 2 things.  The first thing it does is to reset the
  timer so that it will get called again.  Then it will start the process
  of updating the temperature and adjusting the linearizers.  This process
  can only be stopped by clearing the timer with the function rex_clr_timer().

DEPENDENCIES
  This function must be called from a task, so that it remains interruptable.
  temp_comp_timer_ptr must have been initialized and setup.

RETURN VALUE
  None

SIDE EFFECTS
  This function will set the timer, so that in will get executed again.

===========================================================================*/
extern void rf_cdma_do_temp_comp( void );


/*===========================================================================

FUNCTION RF_TX_START                                    EXTERNALIZED FUNCTION

DESCRIPTION
  This function re-enables the PA and Tx periodic power adjustments that were
  disabled by the function rf_tx_stop().  This function will update
  rf_tx_status to RF_TX_IS_ON.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_tx_start(void);


/*===========================================================================

FUNCTION RF_TX_STOP                                     EXTERNALIZED FUNCTION

DESCRIPTION
  This function disables the PA and Tx periodic power adjustments while
  remaining in CDMA RxTx state.  This function will update rf_tx_status
  to RF_TX_IS_OFF.

DEPENDENCIES
  rf_tx_start() must be called before exiting CDMA RxTx state.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_tx_stop(void);

#ifdef T_MSM5000


/*===========================================================================

FUNCTION RF_TX_GAIN_ADJ_STEP_SIZE                       EXTERNALIZED FUNCTION

DESCRIPTION
  This function sets the power control step size for each up/down
  bit. The step size can be 1, 0.5 or 0.25 dB.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rf_tx_gain_adj_step_size( byte step_size );

#endif // T_MSM5000

/*===========================================================================

FUNCTION RF_DIGITAL_BAND_SELECT                         EXTERNALIZED FUNCTION

DESCRIPTION
  Initialises the rf card for the current band(CDMA/PCS). This function has
  has to be called each time whenever there is a band change. (from PCS to
  Cellular or vice versa).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Updates the data base and rf_curr_cdma_band with the current
  band setting.

===========================================================================*/
extern void rf_digital_band_select
(
 byte rf_band
);

#ifdef FEATURE_SKTT_CAL		// kmh insert for calibration 00.06.13
/*===========================================================================

FUNCTION RF_SET_LNA_GAIN                                   EXTERNAL FUNCTION

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void rf_set_lna_gain(byte min, byte pwr_max);

/*===========================================================================

FUNCTION RF_GET_RX_AGC_CPDM                      EXTERNAL FUNCTION

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern byte rf_get_rx_agc_cpdm( void );

/*===========================================================================

FUNCTION RF_GET_LNA_GAIN_PDM		                        EXTERNAL FUNCTION

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern byte rf_get_lna_gain_pdm(void);

/*===========================================================================

FUNCTION RF_SET_RX_AGC_INT                      EXTERNAL FUNCTION

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void rf_set_rx_agc_int( void );

/*===========================================================================

FUNCTION RF_GET_RX_AVG_AGC                      EXTERNAL FUNCTION

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern byte rf_get_rx_avg_agc( void );

/*===========================================================================

FUNCTION RF_SET_RX_AGC_PDM                      EXTERNAL FUNCTION

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void rf_set_rx_agc_pdm( byte pdm );

/*===========================================================================

FUNCTION RF_SET_TM                                          EXTERNAL FUNCTION

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void rf_set_tm( void );

/*===========================================================================

FUNCTION                                         EXTERNAL FUNCTION

    rf_rx_agc_set_test(int2);
    rf_tx_agc_set_test(int2);

DESCRIPTION

	diagp_test(), 테스트 프로그램을 위함 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

extern void rf_rx_agc_set_test(int2);
extern void rf_tx_agc_set_test(int2);

#endif	// #ifdef FEATURE_SKTT_CAL

#endif /* T_MSM2P */

#endif /* RF_H */

