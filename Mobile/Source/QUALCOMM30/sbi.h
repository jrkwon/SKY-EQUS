#ifndef SBI_H

#define SBI_H
/*===========================================================================

               SBI Declarations

DESCRIPTION
  This is the external interface for all external modules into the SBI
  interface.  Calls, commands,  and external definitions.  DMSS software.

Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/sbi.h_v   1.1   10 Sep 1999 18:11:02   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.04.24   msrock  change ADC
--------   ---     ----------------------------------------------------------
11/17/98   udp     Removed unused comment line
11/13/98   snn     added definitions for OOE and AGC_DCONV register.
                   Added the describtion and Edit history for this file.
===========================================================================*/


/****************************************************************************/
#if defined (FEATURE_SBI)

#define SBI_IFR_RESET_REG                       0x00
#define SBI_IFR_SBI_ID_REG                      0x01
#define SBI_IFR_CFILT_GAIN_REG                  0x02
#define SBI_IFR_BLOCK_CTL_REG                   0x04
#define SBI_IFR_CLK_GEN_MODE_REG                0x09
#define SBI_IFR_CFILT_BW_PDM_OFFSET_REG         0x0A
#define SBI_IFR_AGC_DCONV_REG                   0x0C
#define SBI_IFR_FMFILT_GAIN_REG                 0x0E
#define SBI_IFR_FMFILT_BW_REG                   0x31

#define SBI_IFR_RESET_DEFAULT_VAL               0x00
#define SBI_IFR_CFILT_GAIN_DEFAULT_VAL          0x24
#define SBI_IFR_BLOCK_CTL_DEFAULT_VAL           0x3C
#define SBI_IFR_CLK_GEN_MODE_DEFAULT_VAL        0x0C
#define SBI_IFR_CFILT_BW_PDM_OFFSET_DEFAULT_VAL 0x1C
#define SBI_IFR_AGC_DCONV_DEFAULT_VAL           0x0B
#define SBI_IFR_FMFILT_GAIN_DEFAULT_VAL         0x24
#define SBI_IFR_FMFILT_BW_DEFAULT_VAL           0x00
/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

/* Clock Generation Mode Control
*/
#define IFR_CLK_GEN_MODE_R              0x09

#define IFR_CLK_GEN_MODE_PWRDWN_M       0x10
#define IFR_CLK_GEN_MODE_PWRDWN_ENA_V   0x10
#define IFR_CLK_GEN_MODE_PWRDWN_DIS_V   0x00

#define IFR_CLK_GEN_MODE_BUFF_DRIVE_M   0x08
#define IFR_CLK_GEN_MODE_BUFF_WEAK_V    0x08
#define IFR_CLK_GEN_MODE_BUFF_HIGH_V    0x00

#define IFR_CLK_GEN_MODE_DIV4_M         0x04
#define IFR_CLK_GEN_MODE_DIV4_ENA_V     0x04
#define IFR_CLK_GEN_MODE_DIV4_DIS_V     0x00

#define IFR_CLK_GEN_MODE_CX8_MODE_M     0x03
#define IFR_CLK_GEN_MODE_CX8_NORM_V     0x00
#define IFR_CLK_GEN_MODE_CX8_VIA_PIN_V  0x01
#define IFR_CLK_GEN_MODE_CX8_TXCO2_V    0x02
#define IFR_CLK_GEN_MODE_CX8_OFF_V      0x03


#define SBI_IFT_RESET_REG                       0x00
#define SBI_IFT_SBI_ID_REG                      0x01
#define SBI_IFT_PLLN0_REG                       0x02
#define SBI_IFT_PLLN1_REG                       0x03
#define SBI_IFT_PLLR0_REG                       0x04
#define SBI_IFT_PLLR1_REG                       0x05
#define SBI_IFT_PLL_MODE_REG                    0x06
#define SBI_IFT_TXMODE_CTL_REG                  0x0A
#define SBI_IFT_GPADC_RESULT_REG                0x0B
#define SBI_IFT_GPADC_MODE_REG                  0x0C
#define SBI_IFT_ON_OFF_REG                      0x0E

#define SBI_IFT_RESET_DEFAULT_VAL               0x00
#define SBI_IFT_PLLN0_DEFAULT_VAL               0xC4
#define SBI_IFT_PLLN1_DEFAULT_VAL               0x80
#define SBI_IFT_PLLR0_DEFAULT_VAL               0x0F
#define SBI_IFT_PLLR1_DEFAULT_VAL               0xD0
#define SBI_IFT_PLL_MODE_DEFAULT_VAL            0x00
#define SBI_IFT_TXMODE_CTL_DEFAULT_VAL          0x02
#define SBI_IFT_GPADC_MODE_DEFAULT_VAL          0x90
#define SBI_IFT_ON_OFF_DEFAULT_VAL              0x08


/****************************************************************
* Enum that lists the Slaves                                    *
*****************************************************************/
typedef enum
{
  SBI_CTL_WH__SLAVE_ID_IFR = 0x1F,
  SBI_CTL_WH__SLAVE_ID_IFT = 0x2A
} sbi_slave_id_type;


/****************************************************************
* SBI Initialize
*****************************************************************/
extern void SBI_Init( void );

/****************************************************************
* SBI Start Signal
*****************************************************************/
extern void SBI_Start( void );
/****************************************************************
* SBI Stop Signal
*****************************************************************/
extern void SBI_Stop( void );

/****************************************************************
* Function to Select the Slave
*****************************************************************/
extern void SBI_Slave_Sel(sbi_slave_id_type slave_id );

/****************************************************************
* Read from the Slave
*****************************************************************/
extern void SBI_Read( word regAddr);

/****************************************************************
* Write to the Slave
*****************************************************************/
extern void SBI_Write( byte regAddr, byte data);

/****************************************************************
* Function to check if MSBI is busy. Waits till MSBI frees up
*****************************************************************/
extern void SBI_Chk_Busy_Write( void );
extern void SBI_Chk_Busy_Read( void );
extern void SBI_Chk_Mchn_Busy( void );

/****************************************************************
* This function returns the MSBI Status Register
*****************************************************************/
extern word SBI_Get_Status( void );

/* SBI Related definitions */

#define SBI_CLK_CTL_WH__MICRO_RESET_ENA         (1 << 7)
#define SBI_CLK_CTL_WH__MICRO_RESET_DIS         (0 << 7)

#define SBI_CLK_CTL_WH__CNT_EN_ENA              (1 << 6)
#define SBI_CLK_CTL_WH__CNT_EN_DIS              (0 << 6)

#define SBI_CLK_CTL_WH__CLKCTL_164Khz           0x1E
#define SBI_CLK_CTL_WH__CLKCTL_100KHz           0x31
#define SBI_CLK_CTL_WH__CLKCTL_MIN              0x3F
#define SBI_CLK_CTL_WH__CLKCTL_200Khz           0x14
#define SBI_CLK_CTL_WH__CLKCTL_500Khz           0x0A
#define SBI_CLK_CTL_WH__CLKCTL_1Mhz             0x05

#define SBI_READ_CTR                            50
#define SBI_WRITE_CTR                           50
#define SBI_BUSY_CTR                            50

/* SBI_CTL Related flags                 */

#define SBI_CTL_WH__MSM_SBI_EN_ENA             (1 << 11)
#define SBI_CTL_WH__MSM_SBI_EN_DIS             (0 << 11)

#define SBI_CTL_WH__OVR_MODE_ENA               (1 << 10)
#define SBI_CTL_WH__OVR_MODE_DIS               (0 << 10)

#define SBI_CTL_WH__DIAG_MODE_ENA              (1 << 9)
#define SBI_CTL_WH__DIAG_MODE_DIS              (0 << 9)

#define SBI_CTL_WH__LAST_WORD_ENA              (1 << 8)
#define SBI_CTL_WH__LAST_WORD_DIS              (0 << 8)


/* SBI_CTL  MODE FLAGS */
#define SBI_CTL_WH__I3Q_MODE_IS               ( 00 << 6 )   /* ( 0x00 ) */
#define SBI_CTL_WH__I3Q_MODE_FTM              ( 01 << 6 )   /* ( 0x40 ) */
#define SBI_CTL_WH__I3Q_MODE_BTM              ( 02 << 6 )   /* ( 0x80 ) */

/* SB_WR related flags */

#define SBI_WR_WH__RD_WR_N_READ                ( 1 << 15)
#define SBI_WR_WH__RD_WR_N_WRITE               ( 0 << 15)


/*  SBI_START_CTL Register flags */

#define SBI_START_CTL_WH__START_FLAG_ENA       0x01
#define SBI_START_CTL_WH__START_FLAG_DIS       0x00


/* SBI Interrupt defines */
#define SBI_INT_MASK                           0x0001
#define INT_SET                                0x1
#define INT_CLEAR                              0x0

/* SBI CLK Regime Defines */
#define SBI_CLK_REG_MASK                       0x0100
#define CLK_SET                                0x0100
#define CLK_CLEAR                              0x0000

/*
We use the shadow registers to write to the IFR3000 or
IFT3000.
*/
extern byte SBI_IFR_BLOCK_CTL_REG_Shadow;
extern byte SBI_IFR_CLK_GEN_MODE_REG_Shadow;
extern byte SBI_IFR_AGC_DCONV_REG_Shadow;
extern byte SBI_IFT_TXMODE_CTL_REG_Shadow;
extern byte SBI_IFT_GPADC_MODE_REG_Shadow;
extern byte SBI_IFT_ON_OFF_REG_Shadow;

#define SBI_IFT_TXMODE_CTL_GPADC_ON             (1 << 7)
/* Clear off the MUX selection bits before we write to the
Mux selection in the TXMODE_CTL register */
#define SBI_IFT_TXMODE_CTL_GPADC_MUX_CLEAR      (0x9F)

#define SBI_IFT_TXMODE_CTL_CDMA_MODE_ENA        (1 << 4)
#define SBI_IFT_TXMODE_CTL_CDMA_MODE_DIS        (0 << 4)
#define SBI_IFT_TXMODE_CTL_IDLE_MODE_ENA        (0 << 3)
#define SBI_IFT_TXMODE_CTL_IDLE_MODE_DIS        (1 << 3)
#define SBI_IFT_TXMODE_CTL_DC_LOOP_LOCK         (1 << 1)
#define SBI_IFT_TXMODE_CTL_DC_LOOP_INACTIVE     (0 << 1)
#define SBI_IFT_TXMODE_CTL_GPADC_ENHMODE_ENA    (1 << 0)
#define SBI_IFT_TXMODE_CTL_GPADC_ENHMODE_DIS    (0 << 0)

/* Definitions for the ON_OFF register. These bits allow the
corresponding ciruitry in the IFT to be switched off when the
PA is off.  */
#define SBI_IFT_ON_OFF_TXDAC_OFF                (1 << 7)
#define SBI_IFT_ON_OFF_TXFILT_OFF               (1 << 6)
#define SBI_IFT_ON_OFF_UPC_OFF                  (1 << 5)
#define SBI_IFT_ON_OFF_TXAGC_OFF                (1 << 4)
#define SBI_IFT_ON_OFF_TXPATH_OFF               (1 << 3)
#define SBI_IFT_ON_OFF_TXPLL_OFF                (1 << 2)
#define SBI_IFT_ON_OFF_TXMIXER_OFF              (1 << 1)


/*
Definitions for the Block Control register bit fields
*/
#define SBI_IFR_BLOCK_CTL_TCXO_DIV4             (1 << 4)
#define SBI_IFR_BLOCK_CTL_TCXO_DIV1             (0 << 4)
#define SBI_IFR_BLOCK_CTL_CHIP_ON_DIS           (0 << 3)
#define SBI_IFR_BLOCK_CTL_CHIP_ON_ENA           (1 << 3)
#define SBI_IFR_BLOCK_CTL_FMB_DIS               (1 << 2)
#define SBI_IFR_BLOCK_CTL_FMB_ENA               (0 << 2)
#define SBI_IFR_BLOCK_CTL_IDLEB_DIS             (1 << 1)
#define SBI_IFR_BLOCK_CTL_IDLEB_ENA             (0 << 1)
#define SBI_IFR_BLOCK_CTL_SLEEPB_DIS            (1 << 0)
#define SBI_IFR_BLOCK_CTL_SLEEPB_ENA            (0 << 0)



/* Definitions of the AGC_DCONV register  Note that we
defined only those bits which we modify. The other bits
should be kept in the default values always */
/* Power ON the downconvertor */
#define SBI_IFR_AGC_DCONV_PWRON                 0x03

/*********************************************************
* These should be in sync with the values given in the   *
* ADC.h. Make these dependent on those values            *
*********************************************************/
//#define SBI_IFT_TXMODE_CTL_GPADC_MUX_HDET_LOW   0x00
//#define SBI_IFT_TXMODE_CTL_GPADC_MUX_HDET_HIGH  0x20
//#define SBI_IFT_TXMODE_CTL_GPADC_MUX_VBATT      0x40
//#define SBI_IFT_TXMODE_CTL_GPADC_MUX_THERM      0x60 // msrock 00.04.24
#define SBI_IFT_TXMODE_CTL_GPADC_MUX_HDET_LOW   0x00
#define SBI_IFT_TXMODE_CTL_GPADC_MUX_VBATT      0x20
#define SBI_IFT_TXMODE_CTL_GPADC_MUX_HDET_HIGH  0x40
#define SBI_IFT_TXMODE_CTL_GPADC_MUX_THERM      0x60

#endif  /* Feature_SBI */
#endif /* SBI_H */


