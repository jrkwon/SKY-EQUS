/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 B A S I C   I / O   S E R V I C E S

GENERAL DESCRIPTION
  This module contains control and data to facilitate interaction with the
  GPIO and Miscellaneous Bit I/O ports.  This module is designed to provide
  appropriate buffering and control for these ports, but not to guarantee
  the behaviour or activation levels of the ports.

REFERENCES
  CDMA ASIC specification sheets, all.
  Schematic appropriate to the target application using this service.

EXTERNALIZED FUNCTIONS
  None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None, all values are initialized to 0 by the C run-time environment.

Copyright (c) 1992, 1993, 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997, 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/bio.c_v   1.1   10 Sep 1999 18:10:38   lchan  $

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.06.20   jwpark  BIO를 SK_BIO로 변환 (FEATURE_SK_BIO)
00.03.01   chos    GPIO 변경에 따른 관련 부분 수정
00.03.01   chos    Start for Ori(D1) project
--------   ---     ----------------------------------------------------------
05/28/97   dhh     Configured for Q phones
05/01/97   jjn     Configured for Module (T_MD)
04/24/97   dhh     Delete Gemini CALL_LED initialization if
                   FEATURE_HOLD_PWRKEY_TO_PWRUP is defined.
11/02/96   jah     TGP (T_T) PS_HOLD initialization.
12/14/94   jah     Gemini PS_HOLD and Call LED initialization.
03/05/93   jah     Changed tri-state buffers to drive the Portables unused
                   outputs (BIO_GPIO_12 and BIO_GPIO_INT_0).
12/10/92   jah     Added PS-HOLD initialization from boot for the portable.
10/06/92   jah     Added PS-HOLD initialization from boot for the stretch.
09/15/92   jah     Added bio_set_sw() for Diag, to complement BIO_GET_SW().
09/10/92   jah     Added bio_switches, for 'soft' DIP switches.
06/16/92   jah     Created

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "rex.h"
#include "bio.h"                /* BIO typedef for bio_sw_type          */



#undef  FEATURE_SK_BIO	// jwpark 00.06.21
//#define FEATURE_SK_BIO	// 이후로 사용하는 모델(D1, A10, B1...)들은
#ifdef  FEATURE_SK_BIO	// 반드시 SK_BIO를 사용한다.
//
//								[ SK_BIO ]
//
// bio.c :		기존의 Qualcomm source의 buffer들을 structure의 형태로 정리하였다.
// bio.h :		SK_BIO_OUT(), SK_BIO_TRISTATE(), SK_BIO_IN(), SK_BIO_INM()을
//				정의하고, 기존의 BIO_OUT(), BIO_TRISTATE(), BIO_IN(), BIO_INM()을
//				사용할 수 있도록 buffer들의 위치를 바꾸었다.
// biogA10.h :	각 GPIO의 정의 방식을, 기존의 mask, on_v, off_v에서 하나를 추가하여
//
//					#define BIO_PS_HOLD         MSM5_GPIO_2
//					#define BIO_PS_HOLD_M       0x01
//					#define BIO_PS_HOLD_ON_V    0x01
//					#define BIO_PS_HOLD_OFF_V   0x00
//
//				와 같은 형식을 사용한다.
//
//
//	각각의 register mapping은 아래표와 같고,
//	---------------------------------------------
//	|	Qualcomm source	|	new register name	|
//	---------------------------------------------
//	|	DMOD_GPIO		|	MSM5_GPIO_0			|
//	|	ENC_GPIO_0		|	MSM5_GPIO_1			|
//	|	ENC_GPIO_1		|	MSM5_GPIO_2			|
//	|	DEC_GPIO_0		|	MSM5_GPIO_3			|
//	|	DEC_GPIO_1		|	MSM5_GPIO_4			|
//	|	DMOD_GPIO_INT	|	MSM5_GPIO_5			|
//	|	DMOD_KEY_READ	|	MSM5_GPIO_6			|
//	---------------------------------------------
//
//
//	프로그램에서 사용할 경우, 기존의
//	
//		BIO_OUT(ENC_GPIO_1, BIO_PS_HOLD_M, PS_HOLD_ON_V);
//	대신,
//		SK_BIO_OUT(BIO_PS_HOLD, PS_HOLD_ON_V);
//	와 같이 표기한다.
//

bio_gpio_info_type bio_gpio[] = {
	{DMOD_GPIO_OUT,     DMOD_GPIO_IN,     DMOD_GPIO_TSEN,     0,                0},
	{ENC_GPIO_0_OUT,    ENC_GPIO_0_IN,    ENC_GPIO_0_TSEN,    0,                0},
	{ENC_GPIO_1_OUT,    ENC_GPIO_1_IN,    ENC_GPIO_1_TSEN,    BIO_PS_HOLD_ON_V, BIO_PS_HOLD_ON_V},
	{DEC_GPIO_0_OUT,    DEC_GPIO_0_IN,    DEC_GPIO_0_TSEN,    0,                0},
	{DEC_GPIO_1_OUT,    DEC_GPIO_1_IN,    DEC_GPIO_1_TSEN,    0,                0},
	{DMOD_GPIO_INT_OUT, DMOD_GPIO_INT_IN, DMOD_GPIO_INT_TSEN, 0,                0},
	{0x00,              DMOD_KEY_READ_IN, 0x00,               0,                0}
	// MSM5_GPIO_6는 읽기만 하는 register 이다.
	// 그래서, OUT의 address와 TSEN의 address가 없다. 만약 사용하려고 시도한다면
	// 예상하지 못한 상황을 맞이 하게 된다.
};

#else // #ifdef FEATURE_SK_BIO

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Buffer for BIO port #0
**
** Set this to PS-HOLD, since this is the value set by boot.asm,
** and it can't write this variable until after it has initialized
** the C data space.
*/
#if (TG==T_P)
#error code not present
#else
byte bio_dmod_gpio_buf;                    /* GPIO output buffer    */
byte bio_dmod_gpio_ts_buf;                 /* GPIO tri-state buffer */
#endif

/* Buffer for BIO port #1
**
** For the portable, GPIO_12 is unused.  To save power, it is
** configured as an output and driven to 0.  The actual tri-state
** takes place when the Interleaver is initialized.
*/
#if (TG==T_P)
#error code not present
#else
byte bio_enc_gpio_0_buf;    /* GPIO output buffer    */
byte bio_enc_gpio_0_ts_buf; /* GPIO tri-state buffer */
#endif

/* Buffer for BIO port #2
*/
#if ((TG==T_MD) || (TG==T_G) || (TG==T_Q))

/* Set this to PS-HOLD, since this is the value set by boot.asm,
** and it can't write this variable until after it has initialized
** the C data space.
**
** Turn on the call LED as soon as possible, to let the user know
** they can take there finger off of the power key (quickly).
*/

#ifdef FEATURE_HOLD_PWRKEY_TO_PWRUP
#error code not present
#else

/* GPIO output buffer
*/
// tasik 00.06.14 for B1 GPIO assignment
#if	(MODEL_ID==MODEL_A10)
byte bio_enc_gpio_1_buf;
//byte bio_enc_gpio_1_buf = BIO_PS_HOLD_ON_V; // msrock 01.02.15 : for long power key
#elif	(MODEL_ID==MODEL_D1) ||(MODEL_ID==MODEL_B1) ||(MODEL_ID==MODEL_B2) 
byte bio_enc_gpio_1_buf = BIO_PS_HOLD_ON_V;
#else	// #if	( MODEL_ID == MODEL_D1 )
byte bio_enc_gpio_1_buf = BIO_PS_HOLD_ON_V | BIO_CALL_LED_ON_V;
#endif	// #if	( MODEL_ID == MODEL_D1 )

/* GPIO tri-state buffer
*/
// tasik 00.06.14 for B1 GPIO assignment
#if	( MODEL_ID == MODEL_B2 ) || (MODEL_ID==MODEL_A10)||(MODEL_ID==MODEL_B1) 	// by chos 00.03.01 for D1 GPIO
byte bio_enc_gpio_1_ts_buf = BIO_PS_HOLD_M;
#else	// #if	( MODEL_ID == MODEL_D1 )
byte bio_enc_gpio_1_ts_buf = BIO_PS_HOLD_M | BIO_CALL_LED_M;
#endif	// #if	( MODEL_ID == MODEL_D1 )

#endif

#elif (TG==T_T)
#error code not present
#else
#error code not present
#endif

/* Buffer for BIO port #3
*/

#if	( MODEL_ID == MODEL_D1 ) 	// by chos 00.03.01 for D1 GPIO
byte bio_dec_gpio_0_buf; //= BIO_CALL_LED_ON_V;    /* GPIO output buffer    */
byte bio_dec_gpio_0_ts_buf; //= BIO_CALL_LED_ON_V; /* GPIO tri-state buffer */ // msrock 00.04.25
#else	// #if	( MODEL_ID == MODEL_D1 )
byte bio_dec_gpio_0_buf;    /* GPIO output buffer    */
byte bio_dec_gpio_0_ts_buf; /* GPIO tri-state buffer */
#endif	// #if	( MODEL_ID == MODEL_D1 )

/* Buffer for BIO port #4
*/
byte bio_dec_gpio_1_buf;    /* GPIO output buffer    */
byte bio_dec_gpio_1_ts_buf; /* GPIO tri-state buffer */

/* Buffers for BIO port #5
**
** For the portable, GPIO_INT_0 is unused.  To save power, it is
** configured as an output and driven to 0.  The actual tri-state
** takes place when the Dmod is initialized.
*/
#if (TG==T_P)
#error code not present
#else
byte bio_dmod_gpio_int_buf;    /* GPIO output buffer    */
byte bio_dmod_gpio_int_ts_buf; /* GPIO tri-state buffer */
#endif
#endif // #ifdef FEATURE_SK_BIO ... #else ...  // jwpark 00.06.21 end

#if (TG==T_S)
#error code not present
#endif

/* Buffer for BIO LED outputs
*/
word bio_led_buf;

#ifndef BIO_USE_DIP

/* RAM image of 'soft' DIP switches.  Init to all False.
*/
bio_sw_type bio_switches = 0;


/*===========================================================================

FUNCTION BIO_SET_SW

DESCRIPTION
  Set the 'soft' DIP switches

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void bio_set_sw
(
  bio_sw_type switches
    /* Value to set the DIP switches to */
)
{
  INTLOCK();
  bio_switches = switches;
  INTFREE();
}

#endif
