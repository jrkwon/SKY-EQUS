#ifndef SIORSREG_H
#define SIORSREG_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      S I O   M S M   U A R T   R E G I S T E R    D E S C R I P T I O N

                   SIO Register Description for RS-232 UARTs

DESCRIPTION

  This file contains the SIO UART register descriptions.

Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

  $Header:   O:/src/asw/COMMON/vcs/siorsreg.h_v   1.1   26 Jan 2001 17:31:38   ydharc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/26/01   ych     Merged T-53/JCDMA features for SK.
08/22/00   dsb     Added BSP support for TCXO.
06/15/00   jwh     Featurized MSMU_MR1_RFR_LVL_MASK.
06/05/00   jwh     Added support for FEATURE_TCXO1920.
03/09/00   jwh     Created file from siors232.c register descriptions

===========================================================================*/

/*===========================================================================
 Register Descriptions
===========================================================================*/

/* Mode Register 1 */

#define MSMU_MR1_RFRC         0x80     /* Ready-for-receiving Control      */
#define MSMU_MR1_CTSC         0x40     /* Clear-to-send Control            */

#define MSMU_MR1_RFR_LVL_MASK 0x3f     /* Mask to use when writing to lower
                                          six bits of MSMU_MR1 register    */
#ifdef SIORS232_RX_AUTO_HRDWR_FCTL
#error code not present
#endif 

/* Mode Register 2 */

#define MSMU_MR2_LOOPBACK 0x80         /* Channel Mode                     */
#define MSMU_MR2_ERRMODE  0x40         /* Error Mode                       */
#define MSMU_MR2_5BPC     0x00         /* 5 Bits per character             */
#define MSMU_MR2_6BPC     0x10         /* 6 Bits per character             */
#define MSMU_MR2_7BPC     0x20         /* 7 Bits per character             */
#define MSMU_MR2_8BPC     0x30         /* 8 Bits per character             */
#define MSMU_MR2_1SB      0x04         /* 1 Stop Bit                       */
#define MSMU_MR2_2SB      0x06         /* 2 Stop Bits                      */
#define MSMU_MR2_OPAR     0x01         /* Odd Parity                       */
#define MSMU_MR2_EPAR     0x02         /* Even Parity                      */
#define MSMU_MR2_SPAR     0x03         /* Space Parity                     */

/* Clock Select Register */

#ifdef FEATURE_UART_TCXO_CLK_FREQ

/* If the UART gets TCXO instead of TCXO/4 then of all of the codes give   */
/* baud rates that are four times faster because we're leaving the M/N     */
/* counter alone for the (upper) standard bauds.                                   */

#define MSMU_CSR_75_BPS    0x00        /* 75 bits per second               */
#define MSMU_CSR_150_BPS   0x00        /* 150 bits per second              */
#define MSMU_CSR_300_BPS   0x00        /* 300 bits per second              */
#define MSMU_CSR_600_BPS   0x11        /* 600 bits per second              */
#define MSMU_CSR_1200_BPS  0x22        /* 1200 bits per second             */
#define MSMU_CSR_2400_BPS  0x33        /* 2400 bits per second             */
#define MSMU_CSR_4800_BPS  0x44        /* 4800 bits per second             */
#define MSMU_CSR_9600_BPS  0x55        /* 9600 bits per second             */
#define MSMU_CSR_14RK_BPS  0x66        /* 14.4K bits per second            */
#define MSMU_CSR_192K_BPS  0x77        /* 19.2K bits per second            */
#define MSMU_CSR_288K_BPS  0x88        /* 28.8K bits per second            */
#define MSMU_CSR_384K_BPS  0x99        /* 38.4K bits per second            */
#define MSMU_CSR_576K_BPS  0xAA        /* 57.6K bits per second            */
#define MSMU_CSR_1152K_BPS 0xCC        /* 115.2K bits per second           */
#define MSMU_CSR_2304K_BPS 0xEE        /* 230.4K bits per second           */

#else   /* not #ifdef FEATURE_UART_TCXO_CLK_FREQ */

#define MSMU_CSR_75_BPS    0x00        /* 75 bits per second               */
#define MSMU_CSR_150_BPS   0x11        /* 150 bits per second              */
#define MSMU_CSR_300_BPS   0x22        /* 300 bits per second              */
#define MSMU_CSR_600_BPS   0x33        /* 600 bits per second              */
#define MSMU_CSR_1200_BPS  0x44        /* 1200 bits per second             */
#define MSMU_CSR_2400_BPS  0x55        /* 2400 bits per second             */
#define MSMU_CSR_3600_BPS  0x66        /* 3600 bits per second             */
#define MSMU_CSR_4800_BPS  0x77        /* 4800 bits per second             */
#define MSMU_CSR_7200_BPS  0x88        /* 7200 bits per second             */
#define MSMU_CSR_9600_BPS  0x99        /* 9600 bits per second             */
#define MSMU_CSR_144K_BPS  0xAA        /* 14.4K bits per second            */
#define MSMU_CSR_192K_BPS  0xBB        /* 19.2K bits per second            */
#define MSMU_CSR_288K_BPS  0xCC        /* 28.8K bits per second            */
#define MSMU_CSR_384K_BPS  0xDD        /* 38.4K bits per second            */
#define MSMU_CSR_576K_BPS  0xEE        /* 57.6K bits per second            */
#define MSMU_CSR_1152K_BPS 0xFF        /* 115.2K bits per second           */
#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

#endif  /* #ifdef FEATURE_UART_TCXO_CLK_FREQ */

#define MSMU_CSR_TX_ONLY_MASK  0x0F    /* Mask to change only TX CSR       */
#define MSMU_CSR_RX_ONLY_MASK  0xF0    /* Mask to change only RX CSR       */
   
/* Status Register */

#define MSMU_SR_RXHUNT    0x80         /* Hunt Character Received          */
#define MSMU_SR_RXBREAK   0x40         /* Break Received                   */
#define MSMU_SR_PF_ERR    0x20         /* Parity of Framing Error          */
#define MSMU_SR_OVR_ERR   0x10         /* Overrun Error                    */
#define MSMU_SR_TXEMT     0x08         /* Transmitter Empty                */
#define MSMU_SR_TXRDY     0x04         /* Transmitter Ready                */
#define MSMU_SR_RXFULL    0x02         /* Receiver Full                    */
#define MSMU_SR_RXRDY     0x01         /* Receiver Ready                   */

/* Command Register */

#define MSMU_CR_RESET_RX      0x10     /* Reset Receiver                   */
#define MSMU_CR_RESET_TX      0x20     /* Reset Transmitter                */
#define MSMU_CR_RESET_ERR     0x30     /* Reset Error Status               */
#define MSMU_CR_RESET_BRK_INT 0x40     /* Reset Break Change Interrupt     */
#define MSMU_CR_STA_BRK       0x50     /* Start Break                      */
#define MSMU_CR_STO_BRK       0x60     /* Stop Break                       */
#define MSMU_CR_CLR_DCTS      0x70     /* Clear CTS Change (delta)         */
#define MSMU_CR_SAMP_MODE     0x90     /* Sample Data Mode                 */
#define MSMU_CR_TEST_PARITY   0xA0     /* Test Parity                      */
#define MSMU_CR_TEST_FRAME    0xB0     /* Test Frame                       */
#define MSMU_CR_RESET_SAMPLE  0xC0     /* Reset Sample Data Mode           */
#define MSMU_CR_SET_RFR       0xD0     /* Set RFR                          */
#define MSMU_CR_RESET_RFR     0xE0     /* Reset RFR                        */
#define MSMU_CR_DIS_TX        0x08     /* Disable Transmitter              */
#define MSMU_CR_ENA_TX        0x04     /* Enable Transmitter               */
#define MSMU_CR_DIS_RX        0x02     /* Disable Receiver                 */
#define MSMU_CR_ENA_RX        0x01     /* Enable Receiver                  */

/* Interrupt Programming Register */

#define MSMU_IPR_NEW_SAMPLE_MODE  0x40  /* New Sample Mode method          */
#define MSMU_IPR_NEW_RXSTALE      0x20  /* New RXSTALE Interrupt method    */

/* Interrupt Mask Register */

#define MSMU_IMR_CUR_CTS     0x40      /* Current CTS                      */
#define MSMU_IMR_DELTA_CTS   0x20      /* Delta CTS                        */
#define MSMU_IMR_RXLEV       0x10      /* RX Level exceeded                */
#define MSMU_IMR_RXSTALE     0x08      /* Stale RX character occurred      */
#define MSMU_IMR_RXBREAK     0x04      /* RX Break occurred                */
#define MSMU_IMR_RXHUNT      0x02      /* RX Hunt character received       */
#define MSMU_IMR_TXLEV       0x01      /* TX Level or below met            */

/* Interrupt Status Register */

#define MSMU_ISR_CUR_CTS     0x40      /* Current CTS                      */
#define MSMU_ISR_DELTA_CTS   0x20      /* Delta CTS                        */
#define MSMU_ISR_RXLEV       0x10      /* RX Level met or exceeded         */
#define MSMU_ISR_RXSTALE     0x08      /* Stale RX character occurred      */
#define MSMU_ISR_RXBREAK     0x04      /* RX Break occurred                */
#define MSMU_ISR_RXHUNT      0x02      /* RX Hunt character received       */
#define MSMU_ISR_TXLEV       0x01      /* TX Level or below met            */

/* For M/N counter registers */

#define MSMU_MVR_DEFAULT     0xC0   /* Default value of M register         */
#define MSMU_NVR_DEFAULT     0xAF   /* Default value of N register         */
#define MSMU_DVR_DEFAULT     0x80   /* Default value of D register         */
#define MSMU_MNDR_DEFAULT    0x19   /* Default value of M/N/D LSB register */

#define MSMU_MVR_DISABLE     0x00   /* Value to write to M register when 
                                       disabling clock                     */
#define MSMU_NVR_DISABLE     0xff   /* Value to write to N register when 
                                       disabling clock                     */
#define MSMU_DVR_DISABLE     0x00   /* Value to write to D register when
                                       disabling clock                     */
#define MSMU_MNDR_DISABLE_MASK UART_MNDREG_WB_MASK /* Mask to use when 
                                       writing to MND register to disable 
                                       UART clock                          */
#define MSMU_MNDR_DISABLE_VAL  0x1c /* Value to write to MND register 
                                       (using mask) to disable UART clock  */
/* For UART IRDA Register */

#define MSMU_IRDA_DISABLE    0x00   /* Disable UART IRDA transceiver       */

/* General values in enabling/disabling active high bits in registers      */

#ifdef T_MSM31
#error code not present
#else
#define MSMU_ENA             0xff   /* Value to be used in enabling.  
                                       Should be used with masks.          */
#define MSMU_DIS             0x00   /* Value to be used in disabling.
                                       Should be used with masks.          */
#endif /* T_MSM31 */

#ifdef T_MSM3
#define SIO_NOP 
#endif

#define MSMU_OUT( reg, val)	\
	(void)outp( reg, val); \
	SIO_NOP								

#ifdef T_MSM5000
/* macro for 16 bit writes to MSM registers */
#define MSMUW_OUT( reg, val) \
		 (void)outpw( reg, val); \
		SIO_NOP  
#endif

#endif /* SIORSREG_H */

