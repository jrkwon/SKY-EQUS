#ifndef BIO_H

#define BIO_H
/*===========================================================================

                   B I T   I / O   D E C L A R A T I O N S

DESCRIPTION
  This file contains definitions to facilitate interaction with the DMSS
  GPIO and Miscellaneous Bit I/O ports.  This module is designed to provide
  appropriate buffering and control for these shared GPIO ports.

Copyright (c) 1992, 1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1994, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997, 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/bio.h_v   1.1   10 Sep 1999 18:10:38   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.06.20   jwpark  BIO를 SK_BIO로 변환 (FEATURE_SK_BIO)
--------   ---     ----------------------------------------------------------
05/28/97   dhh     Added target T_Q for Q phones
03/11/97   jjn     Added target T_MD for 1900 Module
08/05/96   jah     Added target T_T for TGP (Third Generation Portable).
10/10/95   fkm     Minor Macro Changes to save ROM.
09/21/95   fkm     Changed BIO_OUT() to allow better Compiler Opimization
06/30/95   jah     Added BIO_SW9_M through BIO_SW16_M, moved target-specific
                   LEDs/Switches out.  Cleaned up LED/Switch #defines
06/05/95   jah     Added BIO_OUTPUT_ENA_V, BIO_OUTPUT_DIS_V.
06/05/95   ras     Added target T_I2 for iss2, and T_C1 for css #1.
03/23/95   jah     Added target T_G for Gemini.
06/29/94   ras     Added target T_I1 for ISS1.
04/20/93   jah     Added target T_B2 for Beta II.
09/15/92   jah     Added bio_set_sw() for Diag, to complement BIO_GET_SW().
09/10/92   jah     Added capability to use a soft DIP switch.
08/02/92   jah     Correct typos in stretch definitions
06/05/92   jah     initial revision, brassboard port and ASIC GPIO's

===========================================================================*/

#include "comdef.h"     /* Definitions for ROM, etc.                       */
#include "target.h"     /* Target specific definitions                     */
#include "processor.h"      /* Definition of chip selects                      */
#include "enci.h"       /* Encoder GPIO definitions                        */
#include "deci.h"       /* Decoder GPIO definitions                        */
#include "dmod.h"       /* Demodulator GPIO definitions                    */

#if defined (T_MSM3)
#include "msm.h"
#endif /* T_MSM3 */
/*-------------------------------------------------------------------------*/

/* Lint never thinks these files gets used, but still we need them.
** So the following line shuts up lint, as we know we're right.
*/
/*lint -efile(766,deci.h,enci.h,dmod.h) */

#if (TG==T_M)
#error code not present
#elif (TG==T_P)
#error code not present
#elif (TG==T_S)
#error code not present
#elif (TG==T_B2)
#error code not present
#elif (TG==T_G)
#include "biog.h"       /* Gemini BIO bit definitions   */

#elif (TG==T_I1)
#error code not present
#elif (TG==T_I2)
#error code not present
#elif (TG==T_C1)
#error code not present
#elif (TG==T_T)
#error code not present
#elif (TG==T_MD)
#error code not present
#elif (TG==T_Q)
#error code not present
#else
#error code not present
#endif



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



// 각 GPIO 레지스터들의 in/out address와 buffer들을 지정한다.
typedef struct {
  const  dword   out_addr;		// GPIO output port address
  const  dword   in_addr;		// GPIO input  port address
  const  dword   tsen_addr;		// GPIO tristate control port address
  word           out_buf;		// GPIO output port shadow buffer
  word           ts_buf;		// GPIO TSEN port shadow buffer
} bio_gpio_info_type;
extern bio_gpio_info_type bio_gpio[];



// GPIO와 관련된 레지스터들의 bio_gpio[]에서의 인덱스를 표시한다.
typedef enum {
  MSM5_GPIO_0 = 0,				// DMOD_GPIO
  MSM5_GPIO_1 = 1,				// ENC_GPIO_0
  MSM5_GPIO_2 = 2,				// ENC_GPIO_1
  MSM5_GPIO_3 = 3,				// DEC_GPIO_0
  MSM5_GPIO_4 = 4,				// DEC_GPIO_1
  MSM5_GPIO_5 = 5,				// DMOD_GPIO_INT
  MSM5_GPIO_6 = 6				// DMOD_KEY_READ_IN
} bio_gpio_type;



/*===========================================================================

MACRO SK_BIO_OUT

DESCRIPTION
  This macro outputs a specified value, qualified by a specified mask, to a
  specified port.  Only the bits corresponding to the mask are actually
  affected; other bits retain their previous values.  To do this, an image is
  maintained of the previous value written to the port which is combined with
  the new value prior to writing.

PARAMETERS
  name  GPIO pin name to write
  val   Value to write to the masked bits in the port

DEPENDENCIES
  If the port needs to be tri-stated to allow output, it must be
  tri-stated before the output will be effected by this macro call.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define SK_BIO_OUT(name, val)                                       \
  INTLOCK();                                                        \
  bio_gpio[name].out_buf = (bio_gpio[name].out_buf & ~(name##_M)) | \
	                       ((name##_M) & (val));                    \
  outp(bio_gpio[name].out_addr, bio_gpio[name].out_buf);            \
  INTFREE();



/*===========================================================================

MACRO SK_BIO_TRISTATE

DESCRIPTION
  This macro tri-states specified bits in a port, qualified by a specified
  mask, of a specified port.  Only the bits corresponding to the mask are
  actually affected; other bits retain their previous values.  To do this,
  an image is maintained of the previous value written to the port which is
  combined with the new value prior to writing.  Writing a bit to 1 enables
  it as an output, writing a 0 enables it as an input.

PARAMETERS
  name  GPIO pin mane to write
  val   Value to write to the masked bits in the port to tri-state enable

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define SK_BIO_TRISTATE(name, val)                                  \
  INTLOCK();                                                        \
  bio_gpio[name].ts_buf = (bio_gpio[name].ts_buf & ~(name##_M)) |   \
	                      ((name##_M) & (val));                     \
  outp(bio_gpio[name].tsen_addr, bio_gpio[name].ts_buf);            \
  INTFREE();



/*===========================================================================

MACRO SK_BIO_IN

DESCRIPTION
  This macro reads a value from a specified Basic I/O port address.

PARAMETERS
  name  GPIO pin name to read
  
DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  byte read from the I/O register.

SIDE EFFECTS
  None

===========================================================================*/
#define SK_BIO_IN(name)  inp(bio_gpio[name].in_addr)



/*===========================================================================

MACRO SK_BIO_INM

DESCRIPTION
  This macro reads a value from a specified Basic I/O port address and then
  applies a specified mask to the value.  Bits not corresponding to the mask
  are returned as zero.

PARAMETERS
  name  GPIO pin name to read

DEPENDENCIES
  None

RETURN VALUE
  Value read from the I/O register AND'd with 'mask'.

SIDE EFFECTS
  None

===========================================================================*/
#define SK_BIO_INM(name) ((name##_M) & inp(bio_gpio[name].in_addr))



// 기존의 BIO_OUT(), BIO_TRISTATE(), BIO_IN(), BIO_INM()도 같이 사용할
// 수 있도록 아래와 같이 BUFFER들을 지정한다.
#define DMOD_GPIO_BUF           bio_gpio[MSM5_GPIO_0].out_buf
#define DMOD_GPIO_TS_BUF        bio_gpio[MSM5_GPIO_0].ts_buf

#define ENC_GPIO_0_BUF          bio_gpio[MSM5_GPIO_1].out_buf
#define ENC_GPIO_0_TS_BUF       bio_gpio[MSM5_GPIO_1].ts_buf

#define ENC_GPIO_1_BUF          bio_gpio[MSM5_GPIO_2].out_buf
#define ENC_GPIO_1_TS_BUF       bio_gpio[MSM5_GPIO_2].ts_buf

#define DEC_GPIO_0_BUF          bio_gpio[MSM5_GPIO_3].out_buf
#define DEC_GPIO_0_TS_BUF       bio_gpio[MSM5_GPIO_3].ts_buf

#define DEC_GPIO_1_BUF          bio_gpio[MSM5_GPIO_4].out_buf
#define DEC_GPIO_1_TS_BUF       bio_gpio[MSM5_GPIO_4].ts_buf

#define DMOD_GPIO_INT_BUF       bio_gpio[MSM5_GPIO_5].out_buf
#define DMOD_GPIO_INT_TS_BUF    bio_gpio[MSM5_GPIO_5].ts_buf


#else	// #ifdef FEATURE_SK_BIO
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* First 8 GPIO bits (all the bits in an 8-bit port)
*/
#define BIO_BIT_0_M             0x01    /* Mask for bit 0  */
#define BIO_BIT_1_M             0x02    /* Mask for bit 1  */
#define BIO_BIT_2_M             0x04    /* Mask for bit 2  */
#define BIO_BIT_3_M             0x08    /* Mask for bit 3  */
#define BIO_BIT_4_M             0x10    /* Mask for bit 4  */
#define BIO_BIT_5_M             0x20    /* Mask for bit 5  */
#define BIO_BIT_6_M             0x40    /* Mask for bit 6  */
#define BIO_BIT_7_M             0x80    /* Mask for bit 7  */

/* Second 8 GPIO bits (only useful in a 16-bit port)
*/
#define BIO_BIT_8_M             0x0100  /* Mask for bit  8 */
#define BIO_BIT_9_M             0x0200  /* Mask for bit  9 */
#define BIO_BIT_A_M             0x0400  /* Mask for bit 10 */
#define BIO_BIT_B_M             0x0800  /* Mask for bit 11 */
#define BIO_BIT_C_M             0x1000  /* Mask for bit 12 */
#define BIO_BIT_D_M             0x2000  /* Mask for bit 13 */
#define BIO_BIT_E_M             0x4000  /* Mask for bit 14 */
#define BIO_BIT_F_M             0x8000  /* Mask for bit 15 */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* BIO Port #0
*/
#define DMOD_GPIO_BUF           bio_dmod_gpio_buf       /* Output buffer    */
#define DMOD_GPIO_TS_BUF        bio_dmod_gpio_ts_buf    /* Tri-state buffer */

#if !defined (T_MSM3)
#define DMOD_GPIO_IN_OP         inp                     /* Input operation  */
#define DMOD_GPIO_OUT_OP        outp                    /* Output operation */
#endif /* !T_MSM3 */
 
extern byte bio_dmod_gpio_buf;                     /* GPIO output buffer    */
extern byte bio_dmod_gpio_ts_buf;                  /* GPIO tri-state buffer */

/*   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  */

/* BIO Port #1
*/
#define ENC_GPIO_0_BUF          bio_enc_gpio_0_buf      /* Output buffer    */
#define ENC_GPIO_0_TS_BUF       bio_enc_gpio_0_ts_buf   /* Tri-state buffer */

#if !defined (T_MSM3)
#define ENC_GPIO_0_IN_OP        inp                     /* Input operation  */
#define ENC_GPIO_0_OUT_OP       outp                    /* Output operation */
#endif /* !T_MSM3 */

extern byte bio_enc_gpio_0_buf;                    /* GPIO output buffer    */
extern byte bio_enc_gpio_0_ts_buf;                 /* GPIO tri-state buffer */

/*   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  */

/* BIO Port #2
*/
#define ENC_GPIO_1_BUF          bio_enc_gpio_1_buf      /* Output buffer    */
#define ENC_GPIO_1_TS_BUF       bio_enc_gpio_1_ts_buf   /* Tri-state buffer */

#if !defined (T_MSM3)
#define ENC_GPIO_1_IN_OP        inp                     /* Input operation  */
#define ENC_GPIO_1_OUT_OP       outp                    /* Output operation */
#endif /* !T_MSM3 */

extern byte bio_enc_gpio_1_buf;                    /* GPIO output buffer    */
extern byte bio_enc_gpio_1_ts_buf;                 /* GPIO tri-state buffer */

/*   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  */

/* BIO Port #3
*/
#define DEC_GPIO_0_BUF          bio_dec_gpio_0_buf      /* Output buffer    */
#define DEC_GPIO_0_TS_BUF       bio_dec_gpio_0_ts_buf   /* Tri-state buffer */

#if !defined (T_MSM3)
#define DEC_GPIO_0_IN_OP        inp                     /* Input operation  */
#define DEC_GPIO_0_OUT_OP       outp                    /* Output operation */
#endif /* !T_MSM3 */

extern byte bio_dec_gpio_0_buf;                    /* GPIO output buffer    */
extern byte bio_dec_gpio_0_ts_buf;                 /* GPIO tri-state buffer */

/*   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  */

/* BIO Port #4
*/
#define DEC_GPIO_1_BUF          bio_dec_gpio_1_buf      /* Output buffer    */
#define DEC_GPIO_1_TS_BUF       bio_dec_gpio_1_ts_buf   /* Tri-state buffer */

#if !defined (T_MSM3)
#define DEC_GPIO_1_IN_OP        inp                     /* Input operation  */
#define DEC_GPIO_1_OUT_OP       outp                    /* Output operation */
#endif /* !T_MSM3 */

extern byte bio_dec_gpio_1_buf;                    /* GPIO output buffer    */
extern byte bio_dec_gpio_1_ts_buf;                 /* GPIO tri-state buffer */

/*   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  */

/* BIO Port #5
*/
#define DMOD_GPIO_INT_BUF       bio_dmod_gpio_int_buf    /* Output buffer   */
#define DMOD_GPIO_INT_TS_BUF    bio_dmod_gpio_int_ts_buf /* Tri-state buffer*/

#if !defined (T_MSM3)
#define DMOD_GPIO_INT_IN_OP     inp                     /* Input operation  */
#define DMOD_GPIO_INT_OUT_OP    outp                    /* Output operation */
#endif /* !T_MSM3 */

extern byte bio_dmod_gpio_int_buf;                 /* GPIO output buffer    */
extern byte bio_dmod_gpio_int_ts_buf;              /* GPIO tri-state buffer */

/*   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  */

/* BIO Port #6 (Read-only)
*/
#if !defined (T_MSM3)
#define DMOD_KEY_READ_IN_OP     inp                     /* Input operation  */
#endif /* !T_MSM3 */

/*   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -  */
#endif // #ifndef FEATURE_SK_BIO  // jwpark 00.06.20 end

#if (TG==T_S)
#error code not present
#endif


/*===========================================================================

MACRO BIO_OUT

DESCRIPTION
  This macro outputs a specified value, qualified by a specified mask, to a
  specified port.  Only the bits corresponding to the mask are actually
  affected; other bits retain their previous values.  To do this, an image is
  maintained of the previous value written to the port which is combined with
  the new value prior to writing.

PARAMETERS
  io    Basic I/O port defined by this header
  mask  Mask of bits in the port to write
  val   Value to write to the masked bits in the port

DEPENDENCIES
  If the port needs to be tri-stated to allow output, it must be
  tri-stated before the output will be effected by this macro call.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#if !defined (T_MSM3)
#define BIO_OUT(io, mask, val) \
  INTLOCK();\
  io##_BUF = (io##_BUF & ~(mask)) | ((mask) & (val)); \
  (void) io##_OUT_OP( io##_OUT, io##_BUF ); \
  INTFREE()

#else  /* !T_MSM3 */
#define BIO_OUT(io, mask, val) \
  INTLOCK();\
  io##_BUF = (io##_BUF & ~(mask)) | ((mask) & (val)); \
  (void) outp( io##_OUT, io##_BUF ); \
  INTFREE()
#endif /* !T_MSM3 */



/*===========================================================================

MACRO BIO_TRISTATE

DESCRIPTION
  This macro tri-states specified bits in a port, qualified by a specified
  mask, of a specified port.  Only the bits corresponding to the mask are
  actually affected; other bits retain their previous values.  To do this,
  an image is maintained of the previous value written to the port which is
  combined with the new value prior to writing.  Writing a bit to 1 enables
  it as an output, writing a 0 enables it as an input.

PARAMETERS
  io    Basic I/O port defined by this header
  mask  Mask of bits in the port to tri-state enable
  val   Value to write to the masked bits in the port to tri-state enable

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#if !defined (T_MSM3)
#define BIO_TRISTATE(io, mask, val) \
  INTLOCK();\
  io##_TS_BUF = (io##_TS_BUF & ~(mask)) | ((mask) & (val)); \
  (void) io##_OUT_OP( io##_TSEN, io##_TS_BUF ); \
  INTFREE()

#else
#define BIO_TRISTATE(io, mask, val) \
  INTLOCK();\
  io##_TS_BUF = (io##_TS_BUF & ~(mask)) | ((mask) & (val)); \
  (void) outp( io##_TSEN, io##_TS_BUF ); \
  INTFREE()
#endif /* !T_MSM3 */
/* Enable/Disable a GPIO as an output.  GPIO's can always be read as inputs.
*/
#define BIO_OUTPUT_ENA_V    0xFF
#define BIO_OUTPUT_DIS_V    0x00


/*===========================================================================

MACRO BIO_IN

DESCRIPTION
  This macro reads a value from a specified Basic I/O port address.

PARAMETERS
  io    Basic I/O port defined by this header

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  byte read from the I/O register.

SIDE EFFECTS
  None

===========================================================================*/

#if !defined (T_MSM3)
#define BIO_IN(io)              io##_IN_OP( io##_IN )
#else
#define BIO_IN(io)              inp( io##_IN )
#endif /* !T_MSM3 */


/*===========================================================================

MACRO BIO_INM

DESCRIPTION
  This macro reads a value from a specified Basic I/O port address and then
  applies a specified mask to the value.  Bits not corresponding to the mask
  are returned as zero.

PARAMETERS
  io    Basic I/O port defined by this header
  mask  Mask of bits in the port to read

DEPENDENCIES
  None

RETURN VALUE
  Value read from the I/O register AND'd with 'mask'.

SIDE EFFECTS
  None

===========================================================================*/

#if !defined (T_MSM3)
#define BIO_INM(io, mask)       ((mask) & io##_IN_OP( io##_IN ))
#else
#define BIO_INM(io, mask)       ((mask) & inp( io##_IN ))
#endif /* !T_MSM3 */


/*-------------------------------------------------------------------------*/

/* Below are the definitions for the macros which operate on the LEDs and
** switches in the Mobile and Stretch.  For platforms which do not support
** these features, there are definitions which yield no code.
**
** If the LED masks are defined in the target-specific header, the #ifdef
** avoids redefining them.
*/
#ifdef BIO_LED_OUT

#ifndef BIO_LED1_M

#define BIO_LED1_M      0x80          /* Masks for 8 general purpose LEDs. */
#define BIO_LED2_M      0x40
#define BIO_LED3_M      0x20
#define BIO_LED4_M      0x10
#define BIO_LED5_M      0x08
#define BIO_LED6_M      0x04
#define BIO_LED7_M      0x02
#define BIO_LED8_M      0x01

#endif

extern word bio_led_buf;     /* Extern of buffer for Basic I/O LED outputs */

#define BIO_LED_BUF     bio_led_buf


/*===========================================================================

MACRO BIO_SET_LED
MACRO BIO_CLR_LED

DESCRIPTION
  Set/Clear the specified LED output, this macro call effects none of the
  other LEDs.

PARAMETERS
  led   LED(s) to set/clear, use BIO_LEDn_M values defined above.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#define BIO_CLR_LED( led ) \
  INTLOCK();\
  (void) BIO_LED_OUT_OP ( BIO_LED_OUT, (int)(bio_led_buf &= ~(led)) ); \
  INTFREE()

#define BIO_SET_LED( led ) \
  INTLOCK();\
  (void) BIO_LED_OUT_OP ( BIO_LED_OUT, (int)(bio_led_buf |= (led)) ); \
  INTFREE()

#else

/* Cause the following macros to compile to "no code" on targets which
** cannot support them.
*/
#define BIO_CLR_LED( led )
#define BIO_SET_LED( led )

#endif /* TG == T_M or T_S */


/*-------------------------------------------------------------------------*/

/* Masks for accessing the switches via BIO_GET_SW() and bio_set_sw().
** If they are defined specific to a target, do not redefine them.
**
** The strange number order for SW1-8 is due to a labelling error in the
** Beta 1 mobile, the labels were backwards relative to the bus bits.
*/
#ifndef BIO_SW1_M

#define BIO_SW1_M       0x80
#define BIO_SW2_M       0x40
#define BIO_SW3_M       0x20
#define BIO_SW4_M       0x10
#define BIO_SW5_M       0x08
#define BIO_SW6_M       0x04
#define BIO_SW7_M       0x02
#define BIO_SW8_M       0x01

#define BIO_SW9_M       0x0100
#define BIO_SW10_M      0x0200
#define BIO_SW11_M      0x0400
#define BIO_SW12_M      0x0800
#define BIO_SW13_M      0x1000
#define BIO_SW14_M      0x2000
#define BIO_SW15_M      0x4000
#define BIO_SW16_M      0x8000

#endif


/*===========================================================================

MACRO BIO_GET_SW

DESCRIPTION
  Get the current value of the specified switches.

PARAMETERS
  switches   Mask of switches to report the value of, use the BIO_SWn_M
             values defined above.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

/*  Note: when the switch is in the "on" position, it reads back as logic 0.
**  so we invert it here to get logical 1's instead.
*/

#if (defined(BIO_USE_DIP)&&((TG==T_M)||(TG==T_S)))
#error code not present
#else /* RAM DIP switch */

#define BIO_GET_SW(switches)    ( bio_switches & (switches) )

typedef word bio_sw_type;

extern bio_sw_type bio_switches;


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
extern void bio_set_sw
(
  bio_sw_type switches
    /* Value to set the DIP switches to */
);

#endif

#endif /* BIO_H */

