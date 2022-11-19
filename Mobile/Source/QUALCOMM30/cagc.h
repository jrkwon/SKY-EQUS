#ifndef CAGC_H
#define CAGC_H
/*===========================================================================

                        C A G C   H E A D E R   F I L E

DESCRIPTION
  This module contains all the definitions necessary to interface with the
  CAGC.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/cagc.h_v   1.2   05 Oct 1999 12:05:22   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/06/99   ychan   Modified for targets NGP, SURF and Q-1900.
01/06/99   ychan   Updated Copyright date.
11/06/98   kss     Added support for MSM3000.
03/17/98   thh     Added CAGC_AGC_MIN_HIGHEST/LOWEST and CAGC_AGC_MIN_HIGHEST/
                   LOWEST values to define max/min values for Rx AGC limit
                   registers.
                   Updated copyright date.
10/08/97   thh     Added MSM2.3 support.
08/21/97   thh     Created on this date.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "processor.h"
#include "comdef.h"     /* Definitions for byte, word, etc.     */
#if defined (T_MSM3)
#include "msm.h"
#endif /* T_MSM3 */


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=

                 CDMA AGC (CAGC) I/O PORTS AND DATA

=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#if !defined (T_MSM3)
#define CAGC_BASE_IO      P_PCS2_IO /* Base I/O addr for CAGC registers */

#define CAGC_IO_SIZ             128 /* size of CDMA AGC I/O space */
#endif /* !T_MSM3 */


/*===========================================================================

MACRO CAGC_IO

DESCRIPTION
  Computes the address of the CAGC port given the port offset from the
  base address.

PARAMETERS
  io    the offset from the CAGC base address

DEPENDENCIES
  None

RETURN VALUE
  16 bit absolute I/O address

SIDE EFFECTS
  None

===========================================================================*/
#if !defined (T_MSM3)
#define CAGC_IO( inx ) (CAGC_BASE_IO + ( inx ))
#endif /* !T_MSM3 */


/*===========================================================================

MACRO CAGC_INX

DESCRIPTION
  The inverse of CAGC_IO - this calculates the offset from the port
  address.

PARAMETERS
  io    the address within the CAGC I/O space

DEPENDENCIES
  None

RETURN VALUE
  offset from CAGC base address

SIDE EFFECTS
  None

===========================================================================*/
#if !defined (T_MSM3)
#define CAGC_INX( io ) ((io) - CAGC_BASE_IO)
#endif /* !T_MSM3 */


/*===========================================================================

MACRO CAGC_IN

DESCRIPTION
  This macro reads and returns the value from a specified port.  No mask is
  applied to the read value.

PARAMETERS
  io    the offset from the CAGC base address

DEPENDENCIES
  None

RETURN VALUE
  byte contents of specified port

SIDE EFFECTS
  None

===========================================================================*/
#if defined (T_MSM3)
#define CAGC_IN( io )  MSM_IN( io )
#else
#define CAGC_IN( io )  ((byte)inp( io ))
#endif /* T_MSM3 */


/*===========================================================================

MACRO CAGC_INM

DESCRIPTION
  This macro reads the value from a specified I/O port address and
  then applies a specified mask to the value.  Bits not corresponding
  to the mask are returned as zero.

PARAMETERS
  io    the offset from the CAGC base address
  mask  the mask to use, 1s enable bit update

DEPENDENCIES
  None

RETURN VALUE
  byte value of port contents ANDed with mask

SIDE EFFECTS
  None

===========================================================================*/
#if defined (T_MSM3)
#define  CAGC_INM( io, mask )  MSM_INM( io, mask )
#else
#define  CAGC_INM( io, mask )  ( (byte)inp( io ) & (mask) )
#endif /* T_MSM3 */


/*===========================================================================

MACRO CAGC_OUT

DESCRIPTION
  This macro outputs a specified value to a specified port.  All 8 bits
  of the port are written (i.e., the port is not buffered by software).

PARAMETERS
  io    the offset from the CAGC base address
  val   the byte value to write to the port

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if defined (T_MSM3)
#define CAGC_OUT( io, val ) MSM_OUT( io, val )
#else
#define CAGC_OUT( io, val ) ((void)outp( (io), (val) ))
#endif /* T_MSM3 */


/*===========================================================================

MACRO CAGC_OUTM

DESCRIPTION
  This macro outputs a specified value, qualified by a specified mask,
  to a specified port address.  Only the bits corresponding to the mask
  are actually affected; other bits retain their previous values.  To do
  this, an image is maintained of the previous value written to the port
  which is then combined with the new value.

PARAMETERS
  io    the offset from the CAGC base address
  mask  the mask to use, 1s enable bit update
  val   the byte value to write to the port

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if defined (T_MSM3)
#define CAGC_OUTM( io, mask, val ) MSM_OUTM( io, mask, val)
#else
#define CAGC_OUTM( io, mask, val )                    \
   INTLOCK( );                                        \
   cagc_image[ CAGC_INX(io) ] =                       \
     (cagc_image[ CAGC_INX(io) ] & (byte) ~(mask)) |  \
     ((byte) ((val) & (mask)));                       \
   (void)outp( (io), cagc_image[ CAGC_INX(io) ] );    \
  INTFREE( )                                          
#endif /* T_MSM3 */

#ifdef T_MSM5000

/*===========================================================================

MACRO CAGC_OUTHM

DESCRIPTION
  This macro is identical to CAGC_OUTM (but only defined for MSM5000) except
  that it writes a 16-bit word to the port instead of 8-bit.

PARAMETERS
  io    the offset from the CAGC base address
  mask  the mask to use, 1s enable bit update
  val   the word value to write to the port

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define CAGC_OUTHM( io, mask, val ) MSM_OUTHM( io, mask, val)
#endif
/* -----------------------------------------------------------------------
** CAGC:  AGC_CNTL write port
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_CNTL              CAGC_IO(0x62)
#endif /* !T_MSM3 */

                                    /* MSM23 ONLY: Tx gain adj step size*/
#define CAGC_TX_GAIN_ADJ_STP_M 0xC0 /* mask to set power adj step size  */
#define CAGC_TX_STEP_FULL_DB_V 0x00 /* value for 1.0  dB step size      */
#define CAGC_TX_STEP_HALF_DB_V 0x40 /* value for 0.5  dB step size      */
#define CAGC_TX_STEP_QUAR_DB_V 0x80 /* value for 0.25 dB step size      */

#define CAGC_LIN_RANGE_SEL_M   0x20 /* mask to ctl range of TX_AGC_ADJ. */
#define CAGC_LIN_RANGE_ENA_V   0x20 /* MSM2.3 only.      102.4 dB range */
#define CAGC_LIN_RANGE_DIS_V   0x00 /* MSM2.2 compatible. 85.3 dB range */

#define CAGC_ENA_M             0x10 /* mask for CAGC enable/disable bit */
#define CAGC_ENA_V             0x00 /* CAGC enable                      */
#define CAGC_DIS_V             0x10 /* CAGC disable                     */

#define CAGC_READ_SYNC_M       0x08 /* mask for uP_READ_SYNC bit        */
#define CAGC_READ_SYNC_ENA_V   0x08 /* READ_SYNC action                 */
#define CAGC_READ_SYNC_DIS_V   0x00 /* no READ_SYNC action              */

#define CAGC_RATCHET_M         0x04 /* mask for RATCHET_MODE bit        */
#define CAGC_RATCHET_ENA_V     0x04 /* RATCHET_MODE force               */
#define CAGC_RATCHET_DIS_V     0x00 /* RATCHET_MODE normal              */

#define CAGC_RX_PDM_OE_M       0x02 /* mask for RX PDM OE bit           */
#define CAGC_RX_PDM_ENA_V      0x02 /* RX PDM enable                    */
#define CAGC_RX_PDM_DIS_V      0x00 /* RX PDM disable                   */

#define CAGC_TX_PDM_OE_M       0x01 /* mask for TX PDM OE bit           */
#define CAGC_TX_PDM_ENA_V      0x01 /* TX PDM enable                    */
#define CAGC_TX_PDM_DIS_V      0x00 /* TX PDM disable                   */

/* -----------------------------------------------------------------------
** CAGC:  OUTPUT_CNTL write port for output controls
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_OUT_CTL           CAGC_IO(0x64)
#endif /* !T_MSM3 */

#define CAGC_RX_AGC_POL_M      0x80 /* mask for Rx AGC polarity bit   */
#define CAGC_RX_AGC_POL_NORM_V 0x00 /* Rx AGC normal polarity         */
#define CAGC_RX_AGC_POL_INV_V  0x80 /* Rx AGC inverted polarity       */

#define CAGC_TX_AGC_POL_M      0x40 /* mask for Rx AGC polarity bit   */
#define CAGC_TX_AGC_POL_NORM_V 0x00 /* Rx AGC normal polarity         */
#define CAGC_TX_AGC_POL_INV_V  0x40 /* Rx AGC inverted polarity       */

#define CAGC_LNA_POL_M         0x20 /* mask for LNA polarity bit      */
#define CAGC_LNA_POL_NORM_V    0x00 /* LNA normal polarity            */
#define CAGC_LNA_POL_INV_V     0x20 /* LNA inverted polarity          */

#define CAGC_PAR_POL_M         0x10 /* mask for PA range polarity bit */
#define CAGC_PAR_POL_NORM_V    0x00 /* PA range normal polarity       */
#define CAGC_PAR_POL_INV_V     0x10 /* PA range invert polarity       */

#define CAGC_RX_AGC_CTL_M      0x08 /* mask for Rx AGC mode bit       */
#define CAGC_RX_AGC_CTL_INT_V  0x08 /* Rx AGC internal control        */
#define CAGC_RX_AGC_CTL_CPU_V  0x00 /* Rx AGC CPU control             */

#define CAGC_TX_AGC_CTL_M      0x04 /* mask for Tx AGC mode bit       */
#define CAGC_TX_AGC_CTL_INT_V  0x04 /* Tx AGC internal control        */
#define CAGC_TX_AGC_CTL_CPU_V  0x00 /* Tx AGC CPU control             */

#define CAGC_LNA_CTL_M         0x02 /* mask for LNA mode bit          */
#define CAGC_LNA_CTL_INT_V     0x02 /* LNA internal control           */
#define CAGC_LNA_CTL_CPU_V     0x00 /* LNA CPU control                */

#define CAGC_PAR_CTL_M         0x01 /* mask for PA range mode bit     */
#define CAGC_PAR_CTL_INT_V     0x01 /* PA range internal control      */
#define CAGC_PAR_CTL_CPU_V     0x00 /* PA range CPU control           */

/* -----------------------------------------------------------------------
** CAGC:  AGC_DATA_1 write port
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_DATA_1            CAGC_IO(0x66)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  AGC_DATA_2 write port
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_DATA_2            CAGC_IO(0x72)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  AGC_DATA_3 write port
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_DATA_3            CAGC_IO(0x74)
#endif /* !T_MSM3 */

#define CAGC_TX_AGC_M          0x20 /* mask for LSB of Tx AGC value     */
#define CAGC_TX_AGC_LSB_SHIFT  5    /* bit-shift to move LSB into place */

#define CAGC_LNA_M             0x10 /* mask for LNA range bit           */
#define CAGC_LNA_HI_V          0x00 /* High gain                        */
#define CAGC_LNA_LO_V          0x10 /* Low gain                         */

#define CAGC_PAR_M             0x0c /* mask for PA range bits           */

/* PA_R1 and PA_R0 values.  Note that PA_R1 and PA_R0 are not affected
** by the polarity when these signals are being set by the CPU (software).
*/
#define CAGC_PAR_00_V          0x0c /* PA R1,R0 = 0,0                   */
#define CAGC_PAR_01_V          0x08 /* PA R1,R0 = 0,1                   */
#define CAGC_PAR_10_V          0x04 /* PA R1,R0 = 1,0                   */
#define CAGC_PAR_11_V          0x00 /* PA R1,R0 = 1,1                   */

#define CAGC_LNA_SWITCH_UB_M   0x03 /* mask for 2 MSBs of LNA switch    */

/* -----------------------------------------------------------------------
** CAGC:  AGC_DATA_4 write port
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_DATA_4            CAGC_IO(0x76)
#endif /* !T_MSM3 */

                                    /* MSM23: loop gain ctl during IM   */
#define CAGC_GAIN_IM_M         0xc0 /* mask for CAGC IM bits            */
#define CAGC_TCIM_0073_US      0xc0 /*   73 us time constant            */
#define CAGC_TCIM_0098_US      0x80 /*   98 us time constant            */
#define CAGC_TCIM_0196_US      0x40 /*  196 us time constant            */
#define CAGC_TCIM_GC_V         0x00 /*  Use GAIN_CONSTANT values        */

#define CAGC_GAIN_M            0x3c /* mask for CAGC gain bits          */
#define CAGC_TC_0073_US        0x3c /*   73 us time constant            */
#define CAGC_TC_0098_US        0x00 /*   98 us time constant            */
#define CAGC_TC_0147_US        0x04 /*  147 us time constant            */
#define CAGC_TC_0196_US        0x08 /*  196 us time constant            */
#define CAGC_TC_0294_US        0x0c /*  294 us time constant            */
#define CAGC_TC_0392_US        0x10 /*  392 us time constant            */
#define CAGC_TC_0587_US        0x14 /*  587 us time constant            */
#define CAGC_TC_0783_US        0x18 /*  783 us time constant            */
#define CAGC_TC_1175_US        0x1c /* 1175 us time constant            */
#define CAGC_TC_1566_US        0x20 /* 1566 us time constant            */
#define CAGC_TC_2349_US        0x24 /* 2349 us time constant            */

#define CAGC_PWR_LIM_UB_M      0x03 /* mask for 2 MSBs of power limit   */

/*  NEW register for MSM23 */
/* -----------------------------------------------------------------------
** CAGC:  AGC_DATA_5 write port
** ----------------------------------------------------------------------- */
#if !defined (T_MSM3)
#define CAGC_CTL_5             CAGC_IO(0xAC)
#endif /* !T_MSM3 */

#define CAGC_LNA_FILT_OVRD_M   0x80    /* Mask for LNA filter override */
#define CAGC_LNA_FILT_FRZ_V    0x80    /* Override LNA filter          */
#define CAGC_LNA_FILT_NRM_V    0x00    /* Normal auto LNA filter ops   */

#define CAGC_LNA_RG_FILT_SEL_M 0x40    /* Mask for dig. LNA input sel  */
#define CAGC_LNA_RG_FILT_ENA_V 0x40    /* Input from LPF               */
#define CAGC_LNA_RG_FILT_DIS_V 0x00    /* Input from AGC_VALUE         */

#define CAGC_LNA_GN_FILT_SEL_M 0X20    /* Mask for lin. LNA input sel  */
#define CAGC_LNA_GN_FILT_ENA_V 0X20    /* Input from LPF               */
// kmh correct 0x20 -> 0x00
#define CAGC_LNA_GN_FILT_DIS_V 0X00    /* Input from AGC_VALUE         */

#define CAGC_LNA_RG_OVRD_M     0x10    /* Mask for LNA range override  */
#define CAGC_LNA_RG_OVRD_ENA_V 0x10    /* Override LNA range           */
#define CAGC_LNA_RG_OVRD_DIS_V 0x00    /* Normal LNA range ops         */

// kmh correct 0x80 -> 0x08
#define CAGC_LNA_RG_FORCE_M    0x08    /* Mask for LNA force value   */
#define CAGC_LNA_RG_FORCE_0    0x00    /* Force LNA value to 0       */
#define CAGC_LNA_RG_FORCE_1    0x08    /* Force LNA value to 1       */

// kmh correct 0x40 -> 0x04
#define CAGC_LNA_GN_OVRD_M     0x04    /* Mask for LNA gain override   */
#define CAGC_LNA_GN_OVRD_ENA_V 0x04    /* Override LNA gain            */
#define CAGC_LNA_GN_OVRD_DIS_V 0x00    /* Normal LNA gain ops          */

// kmh correct 0x20 -> 0x02
#define CAGC_LNA_GN_POL_M      0x02    /* Mask for LNA gain polarity   */
#define CAGC_LNA_GN_POL_NRM_V  0x00    /* Use normal polarity          */
#define CAGC_LNA_GN_POL_INV_V  0x02    /* Use inverted polarity        */

// kmh correct 0x10 -> 0x01
#define CAGC_LNA_GN_OE_M       0x01    /* Mask for LNA gain output ena */
#define CAGC_LNA_GN_OE_ENA_V   0x01    /* Enable LNA gain output       */
#define CAGC_LNA_GN_OE_DIS_V   0x00    /* Hi-Z LNA gain output         */


/*  NEW register for MSM23 */
/* -----------------------------------------------------------------------
** CAGC:  AGC_DATA_6 write port
** ----------------------------------------------------------------------- */
#if !defined (T_MSM3)
#define CAGC_CTL_6             CAGC_IO(0xAE)
#endif /* !T_MSM3 */

// kmh correct 0x80 -> 0x08
#define CAGC_LNA_GN_SEL_M      0x08    /* LNA_GAIN pin control             */
#define CAGC_LNA_GN_GAIN_V     0x08    /* LNA_GAIN mode                    */
#define CAGC_LNA_GN_PDM3_V     0x00    /* PDM3 mode                        */

// kmh correct 0x40 -> 0x04
#define CAGC_LIN_SEL_M         0x04    /* Mask: subtract LNA_GAIN output?  */
#define CAGC_LIN_ENA_V         0x04    /* Subtract output from Rx pwr path */
#define CAGC_LIN_DIS_V         0x00    /* No subtration of LNA's output    */

#define CAGC_LNA_FILT_BW_M     0x03    /* Mask: behavior of LPF struct.    */

/*  NEW register for MSM23 */
/* -----------------------------------------------------------------------
** AGC_VALUE_MAX port
** ----------------------------------------------------------------------- */
#if !defined (T_MSM3)
#define CAGC_AGC_VALUE_MAX     CAGC_IO(0xB6)
#endif /* !T_MSM3 */

#define CAGC_AGC_MAX_ENA_M     0x80    /* Mask: limit max AGC_VALUE ouput */
#define CAGC_AGC_MAX_ENA_V     0x80    /* Limit AGC_VALUE's max output    */
#define CAGC_AGC_MAX_DIS_V     0x00    /* No AGC_VALUE's max limit        */

#define CAGC_AGC_VALUE_MAX_M   0x7F    /* Max AGC_VALUE's limit value     */
#define CAGC_AGC_MAX_HIGHEST   (int1)0x7F /* Highest value of AGC max limit */
#define CAGC_AGC_MAX_LOWEST    (int1)0x00 /* Lowest value of AGC max limit  */


/*  NEW register for MSM23 */
/* -----------------------------------------------------------------------
** AGC_VALUE_MIN port
** ----------------------------------------------------------------------- */
#if !defined (T_MSM3)
#define CAGC_AGC_VALUE_MIN          CAGC_IO(0xB8)
#endif /* !T_MSM3 */

#define CAGC_AGC_MIN_ENA_M     0x80    /* Mask: limit min AGC_VALUE output */
#define CAGC_AGC_MIN_ENA_V     0x80    /* Limit AGC_VALUE's min output     */
#define CAGC_AGC_MIN_DIS_V     0x00    /* No AGC_VALUE's min limit         */

#define CAGC_AGC_VALUE_MIN_M   0x7F    /* Min AGC_VALUE's limit value      */
#define CAGC_AGC_MIN_HIGHEST   (int1)0xFF /* Highest value of AGC min limit */
#define CAGC_AGC_MIN_LOWEST    (int1)0x80 /* Lowest value of AGC min limit  */

/*  NEW register for MSM23 */
/* -----------------------------------------------------------------------
** IM_STATE_CTL port
** ----------------------------------------------------------------------- */
#if !defined (T_MSM3)
#define CAGC_IM_STATE_CTL      CAGC_IO(0xBA)
#endif /* !T_MSM3 */

#define CAGC_NONBYPASS_TIMER_M 0xE0    /* Mask: duration of nonbypass */
#define CAGC_BYPASS_TIMER_M    0x1F    /* Mask: duration of bypass    */

/* -----------------------------------------------------------------------
** CAGC:  TX_GAIN_LIMIT_LB write port (8 LSBs of Tx pwr limit)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_PWR_LIM_LB        CAGC_IO(0x78)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  PA_R1_RISE write port (unused function)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_PA_R1_RISE        CAGC_IO(0x52)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  PA_R1_FALL write port (unused function)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_PA_R1_FALL        CAGC_IO(0x54)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  PA_R2_RISE write port (unused function)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_PA_R2_RISE        CAGC_IO(0x56)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  PA_R2_FALL write port (unused function)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_PA_R2_FALL        CAGC_IO(0x58)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  PA_R3_RISE write port (unused function)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_PA_R3_RISE        CAGC_IO(0x5a)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  PA_R3_FALL write port (unused function)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_PA_R3_FALL        CAGC_IO(0x5c)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  LNA_OFFSET_LB (LNA switch lower bits) write port (unused)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_LNA_SWITCH_LB     CAGC_IO(0x7a)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  LNA_FALL (LNA fall threshold) write port (unused function)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_LNA_FALL          CAGC_IO(0x7c)
#endif /* !T_MSM3 */

#define CAGC_CDMA_LNA_CTL_FALL_ON_V      0x7f    /* Fall value for LNA on   */
#define CAGC_CDMA_LNA_CTL_FALL_OFF_V     0x80    /* Fall value for LNA off  */

/* -----------------------------------------------------------------------
** CAGC:  LNA_RISE (LNA rise threshold) write port (unused function)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_LNA_RISE          CAGC_IO(0x7e)
#endif /* !T_MSM3 */

#define CAGC_CDMA_LNA_CTL_RISE_ON_V      0x7f    /* Rise value for LNA on   */
#define CAGC_CDMA_LNA_CTL_RISE_OFF_V     0x80    /* Rise value for LNA off  */

/* -----------------------------------------------------------------------
** CAGC:  RAS_RAM_DATA_UB write port (linearizer offset)
** CAGC:  RAS_RAM_DATA_LB write port (linearizer slope)
**
** The Tx and Rx linearizers are loaded through these 2 ports.
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_RAS_RAM_DATA_UB   CAGC_IO(0x36)
#define CAGC_RAS_RAM_DATA_LB   CAGC_IO(0x4a)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  RAS_RAM_CNTL write port
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_RAS_RAM_CNTL       CAGC_IO(0x5e)
#endif /* !T_MSM3 */

#define CAGC_RAS_RAM_TEST_M     0x40 /* mask for RAS RAM test bit        */
#define CAGC_RAS_RAM_TEST_ENA_V 0x40 /* RAS RAM test                     */
#define CAGC_RAS_RAM_TEST_DIS_V 0x00 /* no RAS RAM test                  */

                                       /* MSM23:               */
#define CAGC_AGC_OVRD_M         0x08 /* Selects AGC accum enable bit     */
#define CAGC_AGC_FRZ_V          0x08 /* Freezes AGC accum enable         */
#define CAGC_AGC_NRM_V          0x00 /* Selects Normal AGC accum op      */

#define CAGC_TX_OPENL_M         0x04 /* Selects TX open-loop accum bit   */
#define CAGC_TX_OL_FRZ_V        0x04 /* Freezes TX open-loop accum       */
#define CAGC_TX_OL_NRM_V        0x00 /* TX open-loop accum normal op     */

#define CAGC_RAS_RAM_LO_HI_M    0x02 /* Selects bank-mapped regs in CAGC */
#define CAGC_RAS_RAM_HI_V       0x00 /* Selects high bank reg in CAGC    */
#define CAGC_RAS_RAM_LO_V       0x02 /* Selects low bank reg in CAGC     */
                                     /* End new bits MSM23               */

#define CAGC_RAS_RAM_WRITE_M     0x01 /* mask for RAS RAM write bit       */
#define CAGC_RAS_RAM_WRITE_ENA_V 0x01 /* RAS RAM write                    */
#define CAGC_RAS_RAM_WRITE_DIS_V 0x00 /* no RAS RAM write                 */

/* -----------------------------------------------------------------------
** CAGC_TEST_CNTL write port
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_TEST_CNTL         CAGC_IO(0x6e)
#endif /* !T_MSM3 */

#define CAGC_LI_BYPASS_M       0x01 /* loop integrator bypass mask      */
#define CAGC_LI_BYPASS_ENA_V   0x01 /* bypass integrator for RSSI read  */
#define CAGC_LI_BYPASS_DIS_V   0x00 /* normal loop integrator mode      */

/* -----------------------------------------------------------------------
** CAGC:  AGC_VALUE write port (RSSI when loop integrator is bypassed)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_AGC_VALUE         CAGC_IO(0x52)
#endif /* !T_MSM3 */

/* -----------------------------------------------------------------------
** CAGC:  TX_GAIN_CTL_UB read port (2 MSBs)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_TX_GAIN_CTL_UB    CAGC_IO(0x58)
#endif /* !T_MSM3 */

#define CAGC_TX_GAIN_CTL_UB_M  0x03 /* mask for 2 MSBs of Tx gain ctl   */

/* -----------------------------------------------------------------------
** CAGC:  TX_GAIN_CTL_LB read port (8 LSBs)
** ----------------------------------------------------------------------- */

#if !defined (T_MSM3)
#define CAGC_TX_GAIN_CTL_LB    CAGC_IO(0x5a)
#endif /* !T_MSM3 */

/* Array used by software to buffer the write ports. */
#if !defined (T_MSM3)
extern byte cagc_image[CAGC_IO_SIZ]; /* image of CAGC I/O space */
#endif /* !T_MSM3 */

#endif /* CAGC_H */
