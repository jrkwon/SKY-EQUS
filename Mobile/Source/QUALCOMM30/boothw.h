/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      B O O T  H A R D W A R E  I N I T I A L I Z A T I O N   H E A D E R

GENERAL DESCRIPTION
  This header file contains the definitions of the MSM2.x memory and peripheral
  control hardware registers and reserved memory locations.

Copyright (c) 1991,1992,1995,1996,1997,1998,1999 by QUALCOMM Incorporated.
All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/boothw.h_v   1.3   04 Aug 2000 16:06:14   bcalder  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/28/00   kmh     2.0 Alpha Version Upgrade
00.03.01   chos    GPIO 변경에 따른 관련 부분 수정
00.03.01   chos    Start for Ori(D1) project
--------   ---     ----------------------------------------------------------
06/15/00   jcw     Added defines to support 27MHz div 1 operation
12/10/98   dnn     Changed #ifdef FEATURE_SLEEP_32KHZ to FEATURE_SLEEP_SLOW_CLOCK
                   Added define for Sleep Oscillator Gain setting of 01
12/08/98   bgc     Added T_MSM3 conditionals for TG==T_Q
11/11/98   mdd     Removed dependency on TG==T_G -- not needed for MSM3
09/16/98   dnn     Added define used for sleep oscillator.
09/14/98   ih,rcn  Added 32kHz sleep support.
           ajn,dnn
09/13/98   hcg     Moved well-known location definitions to target specific
                   header file.
08/08/98   hcg     Changed RAM interrupt vector locations.
08/04/98   hcg     Changed mobile model number location after moving check.
07/30/98   hcg     Changed application entry point after moving check segment.
07/28/98   udp     Fixed logical error in BSIZER_CTL2, would only affect PCS6 RD CNT
07/20/98   udp/cly Removed Panther ref's, fixed if defined to use TG.
04/08/96   dna     Renamed to proc_good_frames() to be consistent.
02/28/96   dna     Created module from parts of diagp.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#if defined (T_MSM3)
#include  "msm.h"
#endif

#if defined (T_MSM3)

/*----------------------------------------------------------------------------
------------------------------------------------------------------------------
  ASB Register Defines
------------------------------------------------------------------------------
----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
  ASB Memory Wait 1
----------------------------------------------------------------------------*/

//      Bit [3..0] EEPROM Minimum number of Cycles used on 16 bit access
#define MEMORY_WAIT1_WH__EEPROM_HWORD_WAIT_1CLK         (0x01)
#define MEMORY_WAIT1_WH__EEPROM_HWORD_WAIT_2CLK         (0x02)
#define MEMORY_WAIT1_WH__EEPROM_HWORD_WAIT_3CLK         (0x03)
#define MEMORY_WAIT1_WH__EEPROM_HWORD_WAIT_4CLK         (0x04)

//      Bit [7..4] ROM Minimum number of Cycles used on 16 bit access
#define MEMORY_WAIT1_WH__ROM_HWORD_WAIT_1CLK            (0x01 << 4)
#define MEMORY_WAIT1_WH__ROM_HWORD_WAIT_2CLK            (0x02 << 4)
#define MEMORY_WAIT1_WH__ROM_HWORD_WAIT_3CLK            (0x03 << 4)
#define MEMORY_WAIT1_WH__ROM_HWORD_WAIT_4CLK            (0x04 << 4)

//      Bit [13..8] Configure EEPROM_CS_N base address.  Specifies size of
//      Device on ROM_CS_N and EEPROM_CS_N.  For values less than
//      size specified below, ROM_CS_N will be asserted.  For address values
//      greater than or equal to specified side, EEPROM_CS_N will be
//      asserted

#define MEMORY_WAIT1_WH__EEPROM_BASE_256KB              (1 << 8)
#define MEMORY_WAIT1_WH__EEPROM_BASE_512KB              (1 << 9)
#define MEMORY_WAIT1_WH__EEPROM_BASE_1MB                (1 << 10)
#define MEMORY_WAIT1_WH__EEPROM_BASE_2MB                (1 << 11)
#define MEMORY_WAIT1_WH__EEPROM_BASE_4MB                (1 << 12)
#define MEMORY_WAIT1_WH__EEPROM_BASE_8MB                (1 << 13)


/*----------------------------------------------------------------------------
  ASB Memory Wait 2
----------------------------------------------------------------------------*/

//      Bit [3..0] PCS6 Minimum number of cycles inserted on access to PCS6_N
#define MEMORY_WAIT2_WH__PCS6_MIN_WAIT_1CLK             (0x01)
#define MEMORY_WAIT2_WH__PCS6_MIN_WAIT_2CLK             (0x02)
#define MEMORY_WAIT2_WH__PCS6_MIN_WAIT_3CLK             (0x03)
#define MEMORY_WAIT2_WH__PCS6_MIN_WAIT_4CLK             (0x04)

//      Bit [7..4] RAM Minimum number of Cycles inserted on access to RAM_CS_N
#define MEMORY_WAIT2_WH__RAM_MIN_WAIT_1CLK              (0x01 << 4)
#define MEMORY_WAIT2_WH__RAM_MIN_WAIT_2CLK              (0x02 << 4)
#define MEMORY_WAIT2_WH__RAM_MIN_WAIT_3CLK              (0x03 << 4)
#define MEMORY_WAIT2_WH__RAM_MIN_WAIT_4CLK              (0x04 << 4)

//      Bit [13..8] Configure PCS6_N base address.  Specifies size of
//      Device on RAM_CS_N and PCS6_N.  For values less than
//      size specified below, RAM_CS_N will be asserted.  For address values
//      greater than or equal to specified side, PCS6_N will be
//      asserted

#define MEMORY_WAIT2_WH__PCS6_BASE_256KB                (1 << 8)
#define MEMORY_WAIT2_WH__PCS6_BASE_512KB                (1 << 9)
#define MEMORY_WAIT2_WH__PCS6_BASE_1MB                  (1 << 10)
#define MEMORY_WAIT2_WH__PCS6_BASE_2MB                  (1 << 11)
#define MEMORY_WAIT2_WH__PCS6_BASE_4MB                  (1 << 12)
#define MEMORY_WAIT2_WH__PCS6_BASE_8MB                  (1 << 13)


/*----------------------------------------------------------------------------
  Bus Sizer Control 1
----------------------------------------------------------------------------*/

//      Bit [2..0] EEPROM_CS_N extra clock cycles added to the beginning of a read
//      from EEPROM_CS_N
#define BSIZER_CTL1_WH__EEPROM_RD_CNT_0CLK              0x00

//      Bit [5..3] EEPROM_CS_N extra clock cycles added to the beginning of a write
//      to EEPROM_CS_N
#define BSIZER_CTL1_WH__EEPROM_WR_CNT_0CLK              (0x00 << 3)

//      Bit [8..6] ROM_CS_N extra clock cycles added to the beginning of a read
//      from ROM_CS_N
#define BSIZER_CTL1_WH__ROM_RD_CNT_0CLK                 (0x00 << 6)

//      Bit [11..9] ROM_CS_N extra clock cycles added to the beginning of a write
//      from ROM_CS_N
#define BSIZER_CTL1_WH__ROM_WR_CNT_0CLK                 (0x00 << 9)


/*----------------------------------------------------------------------------
  General Purpose Chip Select Wait States
----------------------------------------------------------------------------*/

//      Bit [4..0] GP_CS_N_WAIT number of wait states inserted during a GP_CS_N
#define GP_CS_N_WAIT_WH__GP_WAIT_1CLK                   0x01
#define GP_CS_N_WAIT_WH__GP_WAIT_2CLK                   0x02
#define GP_CS_N_WAIT_WH__GP_WAIT_3CLK                   0x03
#define GP_CS_N_WAIT_WH__GP_WAIT_4CLK                   0x04

/*----------------------------------------------------------------------------
  Bus Sizer Control 2
----------------------------------------------------------------------------*/

//      Bit [2..0] PCS6_RD_CNT extra clock cycles added to the beginning of a read
//      from PCS6_N
#define BSIZER_CTL2_WH__PCS6_RD_CNT_0CLK                0x00

//      Bit [5..3] PCS6_WR_CNT extra clock cycles added to the beginning of a write
//      to PCS6_N
#define BSIZER_CTL2_WH__PCS6_WR_CNT_0CLK                (0x00 << 3)

//      Bit [8..6] RAM_RD_CNT extra clock cycles added to the beginning of a read
//      from RAM_CS_N
#define BSIZER_CTL2_WH__RAM_RD_CNT_0CLK                 (0x00 << 6)

//      Bit [11..9] RAM_WR_CNT extra clock cycles added to the beginning of a write
//      to RAM_CS_N
#define BSIZER_CTL2_WH__RAM_WR_CNT_0CLK                 (0x00 << 9)

//      Bit [12] PCS6_BSIZE_EN Enable or Disable 8-bit Bus sizing for PCS6_N
#define BSIZER_CTL2_WH__PCS6_BSIZE_EN_8BIT              (1 << 12)
#define BSIZER_CTL2_WH__PCS6_BSIZE_EN_16BIT             (0 << 12)

//      Bit [13] RAM_BSIZE_EN Enable or Disable 8-bit Bus sizing for RAM_CS_N
#define BSIZER_CTL2_WH__RAM_BSIZE_EN_8BIT               (1 << 13)
#define BSIZER_CTL2_WH__RAM_BSIZE_EN_16BIT              (0 << 13)


/*----------------------------------------------------------------------------
  LCD Control
----------------------------------------------------------------------------*/

//      Bit [3..0] LCD_E_HIGH how many clock cycles are used on from LCD_E pin
//      asserting high to LCD_E de-asserting low
#define LCD_CTL_WH__LCD_E_HIGH_2CLK                     (0x02)
#define LCD_CTL_WH__LCD_E_HIGH_3CLK                     (0x03)
#define LCD_CTL_WH__LCD_E_HIGH_6CLK                     (0x06)
#define LCD_CTL_WH__LCD_E_HIGH_8CLK                     (0x08)

//      Bit [7..4] LCD_E_SETUP how many clock cycles are used on from the start
//      of LCD_CS_N access to LCD_E pin asserting high
#define LCD_CTL_WH__LCD_E_SETUP_1CLK                    (0x01 << 4)
#define LCD_CTL_WH__LCD_E_SETUP_2CLK                    (0x02 << 4)
#define LCD_CTL_WH__LCD_E_SETUP_7CLK                    (0x07 << 4)

//      Bit [12..8] LCD_ACCESS how many clock cycles are used on a 16 bit access
//      to LCD_CS_N
#define LCD_CTL_WH__LCD_WAIT_3CLK                       (0x03 << 8)
#define LCD_CTL_WH__LCD_WAIT_5CLK                       (0x05 << 8)
#define LCD_CTL_WH__LCD_WAIT_10CLK                      (0x0A << 8)
#define LCD_CTL_WH__LCD_WAIT_1fCLK                      (0x1f << 8)

/*----------------------------------------------------------------------------
  Chip Select Control
----------------------------------------------------------------------------*/

//      Bit [0] EEPROM_CS_EN
#define CS_CTL_WH__EEPROM_CS_ENA                        0x01
#define CS_CTL_WH__EEPROM_CS_DIS                        0x00

//      Bit [1] RAM_CS_EN
#define CS_CTL_WH__RAM_CS_ENA                           (0x01 << 1)
#define CS_CTL_WH__RAM_CS_DIS                           (0x00 << 1)

//      Bit [2] PCS6_EN
#define CS_CTL_WH__PCS6_ENA                             (0x01 << 2)
#define CS_CTL_WH__PCS6_DIS                             (0x00 << 2)

//      Bit [3] LCD_CS_EN
#define CS_CTL_WH__LCD_CS_ENA                           (0x01 << 3)
#define CS_CTL_WH__LCD_CS_DIS                           (0x00 << 3)

//      Bit [4] LCD_E
#define CS_CTL_WH__LCD_E_ENA                            (0x01 << 4)
#define CS_CTL_WH__LCD_E_DIS                            (0x00 << 4)

//      Bit [5] GP_CS_EN
#define CS_CTL_WH__GP_CS_ENA                            (0x01 << 5)
#define CS_CTL_WH__GP_CS_DIS                            (0x00 << 5)

//      Bit [6] NATIVE_BUS_OE, allows MSM accesses to be visible on uP pins in
//      Native mode
// ENSURE NATIVE_BUS_OE IS DISABLED IN ANY PRODUCTION CODE!!!
#define CS_CTL_WH__NATIVE_BUS_OE_ENA                    (0x01 << 6)
#define CS_CTL_WH__NATIVE_BUS_OE_DIS                    (0x00 << 6)

//      Bit [7] DEAD_CYCLE_DIS, allows dead cycle to be inserted after rising
//      lwr_n or hwr_n
#define CS_CTL_WH__DEAD_CYCLE_ENA                       (0x00 << 7)
#define CS_CTL_WH__DEAD_CYCLE_DIS                       (0x01 << 7)

//      Bit [9]: BY16_SRAM_EN -> x16 SRAM mode enable or not 
#define CS_CTL_WH__BY16_SRAM_ENA                        (1 << 9)/*tasik 00.04.19*/
#define CS_CTL_WH__BY16_SRAM_DIS                        (0x00 << 9)


//      Reset state of CS_CTL_WH, used to verify chip @ RESET vector used to
//      verify read-back of CS_CTL_WH
#define CS_CTL_WH_RESET               ( CS_CTL_WH__EEPROM_CS_DIS | \
                                        CS_CTL_WH__RAM_CS_DIS | \
                                        CS_CTL_WH__PCS6_DIS | \
                                        CS_CTL_WH__LCD_CS_DIS | \
                                        CS_CTL_WH__LCD_E_DIS | \
                                        CS_CTL_WH__GP_CS_DIS | \
                                        CS_CTL_WH__NATIVE_BUS_OE_DIS |  \
                                        CS_CTL_WH__DEAD_CYCLE_ENA )

//      Default for MSM3000 Development Only, used to verify read-back
//      of CS_CTL_WH
#define CS_CTL_WH_DEFAULT               ( CS_CTL_WH__EEPROM_CS_DIS | \
                                        CS_CTL_WH__RAM_CS_ENA | \
                                        CS_CTL_WH__PCS6_DIS | \
                                        CS_CTL_WH__LCD_CS_DIS | \
                                        CS_CTL_WH__LCD_E_DIS | \
                                        CS_CTL_WH__GP_CS_ENA | \
                                        CS_CTL_WH__NATIVE_BUS_OE_ENA |  \
                                        CS_CTL_WH__DEAD_CYCLE_ENA )

/*----------------------------------------------------------------------------
  GPIO Interrupt/Address Select
----------------------------------------------------------------------------*/

//      Bit [0] ADDR_20_EN, when set address bit 20 is selected as the source
//      for GPIO_INT4
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_20_ENA            0x01
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_20_DIS            0x00

//      Bit [1] ADDR_21_EN, when set address bit 21 is selected as the source
//      for GPIO_INT3
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_21_ENA            (0x01 << 1)
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_21_DIS            (0x00 << 1)

//      Bit [2] ADDR_22_EN, when set address bit 22 is selected as the source
//      for GPIO_INT2
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_22_ENA            (0x01 << 2)
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_22_DIS            (0x00 << 2)


/*----------------------------------------------------------------------------
  MSM Wait
----------------------------------------------------------------------------*/

//      Bit [3..0] MSM_WAIT sets minimum number of wait states for a MSM register
//      access
#define MSM_WAIT_WH_0WAIT                               (0x00)
#define MSM_WAIT_WH_1WAIT                               (0x01)

/*----------------------------------------------------------------------------
  ASB Decode Control
----------------------------------------------------------------------------*/

//      Bit [0] ASB_DECODE_CTL if set will force the decoder to add a decode cycle
//      to all subsequent access
#define ASB_DECODE_CTL_WH_0WAIT                         0x00

/*----------------------------------------------------------------------------
  UP Clock Control 1
----------------------------------------------------------------------------*/

//      Bit [0] POWER_DOWN, entry into powerdown by two successive writes of a 1
//      followed by a 0
#define UP_CLK_CTL1_WH__POWER_DOWN_DIS                  0x00
#define UP_CLK_CTL1_WH__POWER_DOWN_EN1                  0x01
#define UP_CLK_CTL1_WH__POWER_DOWN_EN0                  0x00

//      Bit [3..1] XTAL_WU_DURATION, defines time durations in sleep controller clock
//      cycles required by the external resonator (on the XTAL_IN pin) which
//      generates the uP clk to become stable
#ifdef FEATURE_SLEEP_SLOW_CLOCK
#define UP_CLK_CTL1_WH__XTAL_WU_DURATION_52USEC         (2 << 1)
#else
#define UP_CLK_CTL1_WH__XTAL_WU_DURATION_52USEC         (6 << 1)
#endif

//      Bit [5..4] UP_CLK_SRC_SEL, Selects clock source PIN for the ARM processor
//      Note pin clock frequency may not match associated label
#define UP_CLK_CTL1_WH__UP_CLK_SRC_XTAL_IN              (0 << 4)
#define UP_CLK_CTL1_WH__UP_CLK_SRC_CHIPX8               (1 << 4)
#define UP_CLK_CTL1_WH__UP_CLK_SRC_TCXO                 (2 << 4)
#define UP_CLK_CTL1_WH__UP_CLK_SRC_SLEEP_XTAL_IN        (3 << 4)



/*----------------------------------------------------------------------------
  UP Clock Control 2
----------------------------------------------------------------------------*/

//      Bit [2..0] MCLK_RATE, determine active clock rate for ARM
#define UP_CLK_CTL2_WH__MCLK_RATE_DIV1                  0
#define UP_CLK_CTL2_WH__MCLK_RATE_DIV2                  1
#define UP_CLK_CTL2_WH__MCLK_RATE_DIV4                  2
#define UP_CLK_CTL2_WH__MCLK_RATE_DIV8                  3
#define UP_CLK_CTL2_WH__MCLK_RATE_DIV16                 4
#define UP_CLK_CTL2_WH__MCLK_RATE_DIV32                 5
#define UP_CLK_CTL2_WH__MCLK_RATE_DIV64                 6

/* These values can be used to restore the settings to the default */

/*----------------------------------------------------------------------------
  MSM CLK Control 4
----------------------------------------------------------------------------*/

//      Bit [0] TCXO_DIV2, set this bit to specify that a 9.84 MHz clock is input
//      to the MSM3000 TCXO pin
#define MSM_CLK_CTL4_WH__TCXO_DIV2_ENA                  1
#define MSM_CLK_CTL4_WH__TCXO_DIV2_DIS                  0

//      Bit [5] TCXO_DIV4, set this bit to specify that a 19.68 MHz clock is input
//      to the MSM3000 TCXO pin
#define MSM_CLK_CTL4_WH__TCXO_DIV4_ENA                  (1 << 5)
#define MSM_CLK_CTL4_WH__TCXO_DIV4_DIS                  (0 << 5)

//      Bit [6] SLEEP_CLK_SEL, this bit specifies the frequency range of the sleep
//      controller crystal oscillator run of the SLEEP_XTAL_IN pin
#define MSM_CLK_CTL4_WH__SLEEP_CLK_SEL_32KHZ            (0 << 6)
#define MSM_CLK_CTL4_WH__SLEEP_CLK_SEL_1P92MHZ          (1 << 6)

//      Bit [7] SLEEP_XTAL_EN, this bit specifies if the sleep controller is to be
//      run of the SLEEP_XTAL_IN pin or TCXO/4
#define MSM_CLK_CTL4_WH__SLEEP_XTAL_EN_DIS              (0 << 7)
#define MSM_CLK_CTL4_WH__SLEEP_XTAL_EN_ENA              (1 << 7)

//      Bit [8] SLEEP_OSC_RD_BYPASS, set this bit to bypass the resistance in the
//      internal oscillator circuit
#define MSM_CLK_CTL4_WH__SLEEP_OSC_RD_BYPASS_ENA         (1 << 8)
#define MSM_CLK_CTL4_WH__SLEEP_OSC_RD_BYPASS_DIS         (0 << 8)

//      Bit [9] SLEEP_OSC_RF_BYPASS, set this bit to enable the internal resistance
//      on the schmitt trigger path
#define MSM_CLK_CTL4_WH__SLEEP_OSC_RF_BYPASS_ENA         (1 << 9)
#define MSM_CLK_CTL4_WH__SLEEP_OSC_RF_BYPASS_DIS         (0 << 9)

//      Bit [11..10] SLEEP_OSC_GAIN, configure gain of internal oscillator circuit
#define MSM_CLK_CTL4_WH__SLEEP_OSC_GAIN_000             (0 << 10)
#define MSM_CLK_CTL4_WH__SLEEP_OSC_GAIN_001             (1 << 10)

/*----------------------------------------------------------------------------
  IMEM CTL
----------------------------------------------------------------------------*/
#define IMEM_CTL_WB__IMEM_ENA                           (1 << 0)
#define IMEM_CTL_WB__IMEM_DIS                           (1 << 0)

#define IMEM_CTL_WB__IMEM_0WAIT                         (0 << 1)
#define IMEM_CTL_WB__IMEM_1WAIT                         (1 << 1)
#define IMEM_CTL_WB__IMEM_2WAIT                         (2 << 1)
#define IMEM_CTL_WB__IMEM_3WAIT                         (3 << 1)

#ifdef FEATURE_IRAM
#define IMEM_CTL_WB__STATUS    IMEM_CTL_WB__IMEM_ENA
#else
#define IMEM_CTL_WB__STATUS    IMEM_CTL_WB__IMEM_DIS
#endif

#ifdef FEATURE_ICE_DEBUG
/* If we are emulating IRAM on the ICE, run div2 0ws, otherwise div1, 1ws */
#define IMEM_CTL_WB__WAITCFG   IMEM_CTL_WB__IMEM_0WAIT
#else
#define IMEM_CTL_WB__WAITCFG   IMEM_CTL_WB__IMEM_1WAIT
#endif /* FEATURE_ICE_IRAM_DEBUG */

#endif /* T_MSM3 */
/***************************************************************************

  Replace with bio.h, etc ...

***************************************************************************/

#define BOOTHW_KICK_WATCHDOG() \
      outp(SLEEP_CTL_WB, SLEEP_CTL_WB__WATCH_DOG_MASK); \
      outp(SLEEP_CTL_WB, 0)

#if !defined (T_MSM3)
#define PS_HOLD_TSEN_GEMINI_R (ENC_PCS_BASE + 0x3a)
#define PS_HOLD_GPIO_GEMINI_R (ENC_PCS_BASE + 0x3e)
#else
#define PS_HOLD_TSEN_GEMINI_R ENC_GPIO_1_OUT
#define PS_HOLD_GPIO_GEMINI_R ENC_GPIO_1_TSEN
#endif /* T_MSM3 */

#define PS_HOLD_BIT_GEMINI_V  0x08
#define PS_HOLD_OFF_GEMINI_V  0

#if !defined (T_MSM3)
#define PS_HOLD_TSEN_OTHER_R  (DMOD_PCS_BASE + 0xCC)
#define PS_HOLD_GPIO_OTHER_R  (DMOD_PCS_BASE + 0xCE)
#else
#define PS_HOLD_TSEN_OTHER_R  DMOD_GPIO_OUT
#define PS_HOLD_GPIO_OTHER_R  DMOD_GPIO_TSEN
#endif /* !T_MSM3 */

#define PS_HOLD_BIT_OTHER_V   0x20
#define PS_HOLD_OFF_OTHER_V   0


#if !defined (T_MSM3)
/* Port address of chip      */
#define P_PCS3_IO    0x03000018
#define DEC_BASE_IO  P_PCS3_IO


/* Macro to compute the address of the decoder port given the port
** index and the base address
*/
#define DEC_IO( inx ) (DEC_BASE_IO + (inx))

/* MSM2.3 only - Clock regime control registers */
#define DEC_CLK_CTL_1 DEC_IO( 0x78 )
#define DEC_CLK_CTL_2 DEC_IO( 0x7A )

#define DEC_OUT( io, val )  (void) outp( io, (int) val )

#define DEC_TESTCON       DEC_IO(0x1E)    /* Test mode control             */
#define DEC_TEST_RAM_SEL  DEC_IO(0x22)    /* Test RAM Select               */

#endif /* T_MSM3 */


//----------------------------------------------------------------------------
//                         PCS BASE VALUES
//
// The control registers are conveniently listed relative to the PCS which
// selects the particular block of logic.  Below are the base addresses for
// the blocks used by the boot module.
//----------------------------------------------------------------------------

#if !defined (T_MSM3)
#define DMOD_PCS_BASE 0                       //PCS for Demodulator
#define ENC_PCS_BASE 0x0100                    //PCS for Encoder
#define VC_PCS_BASE 0x0200                    //PCS for Vocoder
#endif /* !T_MSM3 */

//----------------------------------------------------------------------------
//                         DMOD WATCHDOG BIT
//
// The Demodulator is mapped into I/O space at DMOD_PCS_BASE.  At offset
// DMOD_SLEEP_CTL, there is a register which contains the control for the
// GPIO bit which resets the watchdog circuit to keep the phone from being
// reset.  During boot, the other bits in the register can be set to 0.
//----------------------------------------------------------------------------

#if !defined(DMOD_SLEEP_CTL)
#if !defined (T_MSM3)
#define DMOD_SLEEP_CTL DMOD_PCS_BASE + 0x00E0    //Sleep Control Register
#endif
#endif

#define DMOD_DOG_BIT 1                       //Value to reset the watch dog

//----------------------------------------------------------------------------
//                         PS_HOLD BIT
//
// There is a register which contains the control for the GPIO bit which
// sets the hold for the voltage regulators to keep the phone from being
// powered-off.  During boot, the other bits in the register can be left
// as inputs.
//----------------------------------------------------------------------------

#ifdef MODEL_SKTT	// by chos 00.03.01 for D1 GPIO

#define PS_HOLD_GPIO_R ENC_GPIO_1_OUT     //GPIO output register
#define PS_HELD_GPIO_R ENC_GPIO_1_IN      //GPIO input register
#define PS_HOLD_TSEN_R ENC_GPIO_1_TSEN    //GPIO tri-state enable register
#define PS_HOLD_BIT_V 0x0001                     //GPIO Bit mask
#define PS_HOLD_OFF_V 0x0000                     //GPIO PS_HOLD "off" value

#else	// #if	( MODEL_ID == MODEL_D1 )
#if !defined (T_MSM3)
#define PS_HOLD_GPIO_R ENC_PCS_BASE + 0x003A     //GPIO output register
#define PS_HELD_GPIO_R ENC_PCS_BASE + 0x000C     //GPIO input register
#define PS_HOLD_TSEN_R ENC_PCS_BASE + 0x003E     //GPIO tri-state enable register
#else
#define PS_HOLD_GPIO_R ENC_GPIO_1_OUT     //GPIO output register
#define PS_HELD_GPIO_R ENC_GPIO_1_IN      //GPIO input register
#define PS_HOLD_TSEN_R ENC_GPIO_1_TSEN    //GPIO tri-state enable register
#endif
#define PS_HOLD_BIT_V 0x008                     //GPIO Bit mask
#define PS_HOLD_OFF_V 0x000                     //GPIO PS_HOLD "off" value
#endif	// #if	( MODEL_ID == MODEL_D1 )

//----------------------------------------------------------------------------
//                         VOCODER RESET BIT
//
// The Viterbi decoder is mapped into I/O space at DEC_PCS_BASE.  At offset
// VOC_RST_GPIO, there is a register which contains the control for the GPIO
// bit which resets the vocoder circuit (DSP).  During boot, the other bits
// in the register can be left as inputs.
//----------------------------------------------------------------------------

#if  !defined (T_MSM3)
#define DEC_PCS_BASE 0x00180                   //PCS for Decoder

#define VOC_RST_GPIO DEC_PCS_BASE + 0x014      //Decoder GPIO output register #1

#define DEC_GPIO_0 0x001                     //GPIO #0

#define VOC_GPIO_TSEN DEC_PCS_BASE + 0x016      //GPIO tri-state enable register

#define VOC_RST DEC_GPIO_0              //Value to set VOC-RST

#endif /* !T_MSM3 */
//----------------------------------------------------------------------------
//                       ENC CLOCK CONTROL REGISTER
//
// The Encoder is mapped into I/O space at 0x100.  ENC_CLK_CTL controls the
// MSM ASIC 1.0 baud clock, the CODEC clock, the Vocoder Frame Reference, and
// the Encoder's transmit clock.  The value stuffed into this register at
// boot time sets the CODEC clock low, tri-states the Vocoder Frame Reference,
// and selects a baud rate of 38.4k for the baud clock.
//----------------------------------------------------------------------------

#if !defined(ENC_CLK_CTL)
#if !defined (T_MSM3)
#define ENC_CLK_CTL ENC_PCS_BASE + 0x030      //Encoder clk ctl reg
#else
#define ENC_CLK_CTL MOD_CLK_CTL_WB      //Encoder clk ctl reg
#endif
#endif

#define ENC_CLK_CTL_V 0x00C3                    //Encoder clk ctl value

//----------------------------------------------------------------------------
//                      TEST MODE CONTROL REGISTERS
//
// The Vocoder test mode overlaps GPIO 13 through 18.  The reset state of this
// test mode is indeterminant.
//----------------------------------------------------------------------------

#if !defined (T_MSM3)
#define VC_TST2_CTL VC_PCS_BASE + 0x001A
#else
#define VC_TST2_CTL VC_TEST_CONTROL_2
#endif

#define VC_TST2_CORE_V 0x000                     //do not bypass the Vocoder

#if !defined(VC_GPIO_2930_CTL)
#if !defined (T_MSM3)
#define VC_GPIO_2930_CTL VC_PCS_BASE + 0x007C
#else
#define VC_GPIO_2930_CTL
#endif
#endif

#define VC_GPIO_2930_V 0x000                     //GPIO 29 & 30 as GPIOs

typedef enum {
  BOOTHW_CLK_DIV_1=UP_CLK_CTL2_WH__MCLK_RATE_DIV1,
  BOOTHW_CLK_DIV_2=UP_CLK_CTL2_WH__MCLK_RATE_DIV2
} boothw_clk_div_type;

void boot_hw_set_clk_div(boothw_clk_div_type divisor);
void boot_hw_init_clock_change_cb( void );

extern boothw_clk_div_type boothw_clock_div;
extern boothw_clk_div_type boothw_desired_clock_div;

