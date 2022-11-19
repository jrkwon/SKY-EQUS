#ifndef ADC_H
#define ADC_H
/*===========================================================================

A N A L O G   T O   D I G I T A L   C O N V E R T E R   H E A D E R   F I L E

DESCRIPTION
  This header file contains all of the definitions necessary for use of the
  Analog-to-Digital Converter.

REFERENCES
  CDMA Baseband Analog ASIC II Objective Specification 80-7900
  Mobile Station Modem ASIC (MSM 2P) High Level Design 80-12259-1

Copyright (c) 1993,1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1995,1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999      by QUALCOMM, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/adc.h_v   1.3   05 Oct 1999 12:59:30   lchan  $   

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/05/99   lcc     Removed ifdef around ADC_RESET_MACRO which is not needed.
08/02/99   jcw     Port to MSM5000
05/27/99   kmp     Changed FEATURE_JSTD008 to FEATURE_CDMA_1900.
03/26/99   snn     Added Support for the T_SURF inside the #ifdef FEATURE_JSTD008.
10/15/98   dlb     Added support for ARM/MSM_3.
02/17/98   kss     New GPIO line added for HDET high/low selection on Q-800.
01/16/98   kss     Fixed problem with the previous value array on certain 
                   targets, added ADC_ADDR_M.
12/18/97   aks     Moved ADC_HDET_LOW to share same port signal as ADC_BATT_ID.
12/09/97   kss     Code Review changes: Moved initial values for adc_prev_val
                   to adcg.c, updated copyright. 
12/08/97   aks     Re-defined GPIO HDET Low to Battery ID for the Q target.
10/06/97   kss     Added ADC_SAVE_POWER_ADDR.
09/08/97   kss     Added initial values for adc sources (in case of bad read)
06/17/97   dhh     Added target T_Q for Q phones
03/14/97   jjn     Added (TG==T_MD) where ever appropriate for the Module
01/15/97   thh     Correct TGP HDET address definition wrongly truncated
                   error.
01/07/97   thh     Added defs/support for the TGP.  Updated copyright date.
09/25/96   DAK     Added defs for ISS2 1900 under FEATURE_JSTD008.
10/30/95   jah     Copy T_I2 (ISS-2) defs for T_C1 (CSS-1)
08/22/95   jah     Changed Gemini/ISS-2 names to match the definitions, and
                   corrected the ADC_AMP_SETTLE_TIME.
05/12/95   tst     Added ADC_AMP_SETTLE_TIME.
03/06/95   tst     Added ISS2 target.
12/21/94   tst     Moved macros  ADC_SET_ADDR, ADC_CONVERT, ADC_WAIT,
                   ADC_READ, and ADC_STATUS from adc.h to adcg.c
                   for Gemini target only.
11/29/94   tst     Incorporated changes from readiness review
11/08/94   tst     Added Gemini target
05/31/94   jah     Added ISS1 target
03/28/94   jah     Commented constraint in ADC_WAIT, re: EOC guard time.
07/27/93   jah     Improved comments per code review.
05/24/93   jah     Added Mobile and Beta II targets
04/30/93   jah     Created

===========================================================================*/

#include "processor.h"  /* Definitions for byte, word, etc.     */
#include "bio.h"        /* Bit I/O macros                       */
#include "customer.h"   /* Customer specific definitions        */
#include "target.h"     /* Target specific definitions          */
#include "clk.h"        /* Clock function needed for ADC_WAIT   */

#ifdef T_MSM3
# include "msm.h"
#endif


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


#if (TG==T_P)
#error code not present
#elif (TG==T_M)
#error code not present
#elif (TG==T_B2)
#error code not present
#elif (TG==T_I1)
#error code not present
#elif ((TG==T_MD) || (TG==T_G))

/* The ADC is integrated into the MSM2P.  The input selector bits
** are accessed through these GPIOs.
*/

#define ADC_ADDR_M    BIO_ADC_MUX_M   /* ADC source address mask */

#ifdef T_SURF 
/* ADC_THERM and ADC_VBATT are read from Potentiometers in SURF .*/
#if MODEL_ID == MODEL_B1
#define ADC_CAMERA_SENSE BIO_ADC_MUX_0_V 
#define ADC_LIGHT_SENSE	BIO_ADC_MUX_0_V 
#define ADC_HDET_HIGH	BIO_ADC_MUX_1_V 
#define ADC_VBATT		BIO_ADC_MUX_2_V 
#define ADC_THERM		BIO_ADC_MUX_3_V 
#elif MODEL_ID == MODEL_B2
#define ADC_CAMERA_SENSE BIO_ADC_MUX_0_V 
#define ADC_HDET_HIGH	BIO_ADC_MUX_1_V 
#define ADC_VBATT		BIO_ADC_MUX_2_V 
#define ADC_THERM		BIO_ADC_MUX_3_V 
#else
#define ADC_HDET_LOW  BIO_ADC_MUX_0_V /* GPIO bits select HDET low range  */
#define ADC_HDET_HIGH BIO_ADC_MUX_1_V /* GPIO bits select HDET high range */
#define ADC_VBATT     BIO_ADC_MUX_2_V /* GPIO bits select battery volts   */
#define ADC_THERM     BIO_ADC_MUX_3_V /* GPIO bits select thermistor      */
#endif
#else
#define ADC_HDET_LOW  BIO_ADC_MUX_0_V /* GPIO bits select HDET low range  */
#define ADC_THERM     BIO_ADC_MUX_1_V /* GPIO bits select thermistor      */
#define ADC_HDET_HIGH BIO_ADC_MUX_2_V /* GPIO bits select HDET high range */
#define ADC_VBATT     BIO_ADC_MUX_3_V /* GPIO bits select battery volts   */
#endif  /* T_SURF */

#define ADC_ADDR_SHIFT 5          /* To convert address to array location */

/* The settling time for the MUX and A/D amplifier.  The maximum settling
** times is approximately 20us for GRAS5.  For GRAS4, the settling time is
** approximately 140 microseconds.
*/
#define ADC_AMP_SETTLE_TIME      140

#elif (TG==T_Q)
#error code not present
#elif (TG==T_I2)
#error code not present
#elif (TG==T_C1)
#error code not present
#elif (TG==T_T)                       /* TGP only */
#error code not present
#else
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* We're using the simple 1st generation ADC, define the registers and
** macros to work with it.
*/
#ifndef T_MSM2

/* To make some code easier to read, you can ADC this input as
** as "start nothing important" conversion.
*/
#define ADC_GROUND      ADC_UNUSED_0

/* How long it takes, in microseconds, for the A/D Converter
** to integrate a sample.
*/
#define ADC_INTEG_TIME  8


/*===========================================================================

MACRO ADC_READ                                          NON-MSM2 APPLICATIONS

DESCRIPTION
  Read the result of the previous conversion, and start a new one based
  on the passed parameter.  This macro should not be used in applications
  with MSM2 devices.  For applications with the MSM2, use the macros
  ADC_STATUS(), ADC_RESET(), ADC_SET_ADDR(), ADC_CONVERT(), ADC_WAIT(),
  and ADC_READ() defined on the following pages.

PARAMETERS
  adc_in   A/D Converter input to start a conversion on.

DEPENDENCIES
  If not proceeded by ADC_WAIT(), the current conversion may not be done.
  During the course of an ADC_READ/ADC_WAIT, interrupts should be blocked
  to insure that the A/D converter is not being "shared".

RETURN VALUE
  Conversion result of the previous conversion.

SIDE EFFECTS
  None

===========================================================================*/

#define ADC_READ(adc_in)   MSM_IN( adc_in )


/*===========================================================================

MACRO ADC_WAIT                                          NON-MSM2 APPLICATIONS

DESCRIPTION
  Do a multi-microsecond busy wait.  Wait for the A/D converter to report
  that the conversion is done.  This macro should not be used in applications
  with MSM2 devices.  For applications with the MSM2, use the macros
  ADC_STATUS(), ADC_RESET(), ADC_SET_ADDR(), ADC_CONVERT(), ADC_WAIT(),
  and ADC_READ() defined on the following pages.

PARAMETERS
  None

DEPENDENCIES
  During the course of an ADC_READ/ADC_WAIT, interrupts should be blocked
  to insure that the A/D converter is not being "shared".
  
  BIO_INM takes at least 100 nanoseconds to execute (real safe assumption!).
  ADC_INTEG_TIME is multiplied by 10 to get the integration timeout in 100
  nanosecond units.

  EOC does not go low for ~8 clocks after the ADC_READ.  The clock is >9 MHz.
  The value of the EOC input is not used until more than 1 microsecond has
    passed.  The loop setup takes longer than 1us for <40 MHz '186.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if (ADC_INTEG_TIME > 6553)
#error ADC_INTEG_TIME to large for xx_tout in ADC_WAIT
#endif

/* While not timed out from a maximum wait AND EOC not asserted
** OR in the EOC guard time, wait.
*/
#define ADC_WAIT() \
  { \
  word xx_tout = 0; \
  while (xx_tout++ < (10 * ADC_INTEG_TIME) \
        && (BIO_INM( DMOD_GPIO_INT, BIO_EOC_M ) != BIO_EOC_V)) {} \
  }



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* If we're using an MSM 2, define the registers and status bits
*/
#else

/* Type for the ADC source parameter in adc_read() */
typedef byte adc_addr_type;

/* ADC Data port */

/* ADC Control/Status port */

#define ADC_EOC_M        0x01 /* status mask for successful A/D conversion */
#define ADC_OVERRUN_M    0x02 /* status mask for overrun by next request */
#define ADC_EXT_ERR_M    0x04 /* status mask for external ADC_CLK error */

#define ADC_EOC_V        0x01 /* status code for successful A/D conversion */
#define ADC_OVERRUN_V    0x02 /* status code for overrun by next request */
#define ADC_EXT_ERR_V    0x04 /* status code for external ADC_CLK error */



/*===========================================================================

MACRO ADC_RESET

DESCRIPTION
  Resets the ADC interface hardware in the MSM2.  This should be done once
  at power up and whenever an error occurs.

PARAMETERS
  None

DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define ADC_RESET_MACRO()            ((void) outp( ADC_CNTRL_STAT, 0 ))


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ADC_READ

DESCRIPTION
  Selects the specified data source, initiates an A/D conversion and returns
  the result of the conversion.  Interrupts are disabled during the 
  conversion process so this function can be called from outside a task
  context (i.e., an interrupt handler).  Errors are logged as non-fatal 
  errors.  After the first error, one retry is made and if a second error
  occurs, the previous value successfully read from the specified data source
  is returned.

DEPENDENCIES
  None

RETURN VALUE
  raw byte value of the ADC conversion

SIDE EFFECTS
  None

===========================================================================*/
extern byte adc_read
(
  adc_addr_type adc_addr    /* A/D input data source (GPIO bits) to select */
);

#endif /* T_MSM2 */

#endif /* ADC_H */

