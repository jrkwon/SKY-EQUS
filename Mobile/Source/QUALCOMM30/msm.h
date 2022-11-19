#ifndef MSM_H
#define MSM_H
/*===========================================================================

               MSM3000 REGISTER ALIAS HEADER FILE

DESCRIPTION
  This file contains declarations and definitions to support the mapping
  of MSM registers from MSM2300 to MSM3000.

Copyright (c) 1998,1999,2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/msm.h_v   1.10   17 Jul 2000 17:13:08   tsummers  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/17/00   ts      Added YAMN1 defines for RUIM.  Fixed comment line.
02/01/00   lcc     For MSM5000, corrected definition of VC_GPIO_2930_CTL to
                   GPIO_FUNCTION_SEL_WH (16-byte).
01/18/99   bgc     Fixed up #ifdef's with T_MSM5000
01/17/00   ejv     Removed MSM_CLK_CTLX redefines.
09/22/99   lcc     Merged in MSM5000 support from PLT archive:
           hrk     Added enum FPC_CTL_WH_INX to shadow 16 bit register FPC_CTL
           jcw     Resolved name conflicts with addrdefs.h
           rmd     Added support to MSM5000.
04/16/99   bgc     Added extra parenthesis in macros to fix lint warnings.
                   Affected macros : MSM_END, MSM_MAX_INX, MSM_FMAX_INX
01/12/98   ak      Added #defines for second UART, which is used in the new
                   SIO.
11/17/98   udp     Modified msm3rev2.h to be more generic and use msm3reg.h
10/26/98   jkl     Added support for trampm3.c
9/03/98     ss     AGC, NS and VR register definitions added.
7/18/98    udp     Use MSM3000 naming

===========================================================================*/

#ifdef TG
   #ifndef TARGET_H
      #include "target.h"
   #endif
#endif

#ifdef T_MSM5000
   #include "msm50reg.h"
#else
   #include "msm3reg.h"
#endif

#include "comdef.h"

/* Define a common location for all non-mapped register's */
#define MSM3_DUMMY                             MSM_END  /* See def below */

/*===========================================================================

                        START DMOD.H ALIAS

===========================================================================*/


/*---------------------------------------------------------------------------
                          Write Registers
---------------------------------------------------------------------------*/

#define DMOD_RESET            DEMOD_RESET_WB
#define DMOD_W_SRCH_CTL       SRCH_CTL_WB
#define DMOD_ETE_0            SRCH_TH_ENERGY_LOW_WB     /* Bits 7:0 */
#define DMOD_ETE_1            SRCH_TH_ENERGY_HIGH_WB    /* Bits 15:8 */
#define DMOD_SIT              SRCH_INTG_TIME_WB         /* Bits 7:0 */
#define DMOD_ETT              SRCH_TH_TIME_WB           /* Bits 7:0 */
#define DMOD_SBO_0            SRCH_OFFSET_LOW_WB        /* Bits 7:0  */
#define DMOD_SBO_1            SRCH_OFFSET_HIGH_WB       /* Bits 14:8 */
#define DMOD_SPNIM_0          SRCH_MASK_I_LOW_WB        /* Bits 7:0 */
#define DMOD_SPNIM_1          SRCH_MASK_I_HIGH_WB       /* Bits 14:8 */
#define DMOD_SPNQM_0          SRCH_MASK_Q_LOW_WB        /* Bits 7:0 */
#define DMOD_SPNQM_1          SRCH_MASK_Q_HIGH_WB       /* Bits 14:0 */
#define DMOD_SS_0             SRCH_SLEW_LOW_WB          /* Bits 7:0 */
#define DMOD_SS_1             SRCH_SLEW_MID_WB          /* Bits 15:0 */
#define DMOD_SS_2             SRCH_SLEW_HIGH_WB         /* Bits 17:16 */
#define DMOD_SAP              SRCH_ACC_PASS_WB
#define DMOD_SN_0             SRCH_NUM_LOW_WB
#define DMOD_SN_1             SRCH_NUM_HIGH_WB          /* MSM23 Only */
#define DMOD_PC_F0_CELL_EN    FFE_POWER_CTL_F0_CELLN_EN_WB
#define DMOD_PC_F1_CELL_EN    FFE_POWER_CTL_F1_CELLN_EN_WB
#define DMOD_PC_F2_CELL_EN    FFE_POWER_CTL_F2_CELLN_EN_WB
#define DMOD_PC_F3_CELL_EN    FFE_POWER_CTL_F3_CELLN_EN_WB
#define DMOD_W_PCPD           TX_GAIN_ADJ_WR_WB
#define DMOD_PCC              POWER_COMB_CTL_WB
#define DMOD_TT_K1_GAIN       FFE_TT_K1_GAIN_WB         /* K1 Gain Register */
#define DMOD_TT_K2_GAIN       FFE_TT_K2_GAIN_WB         /* K2 Gain Register */
#define DMOD_F_CTL_2          FFE_FINGER_CTL2_WB
#define DMOD_W_TTA_0          FFE_TT_ACCUM_LOWER_WB     /* Bits 7:0 */
#define DMOD_W_TTA_1          FFE_TT_ACCUM_UPPER_WB     /* Bits 15:8 */
#define DMOD_W_FRSSI          FFE_FN_RSSI_INIT_WB
#define DMOD_OFFSET_0         FFE_FN_BINARY_OFFSET_LOWER_WB  /* Bits 7:0 */
#define DMOD_OFFSET_1         FFE_FN_BINARY_OFFSET_UPPER_WB  /* Bits 14:8 */
#define DMOD_IMASK_0          FFE_FN_PN_I_MASK_LOWER_WB /* Bits 7:0 */
#define DMOD_IMASK_1          FFE_FN_PN_I_MASK_UPPER_WB /* Bits 14:8 */
#define DMOD_QMASK_0          FFE_FN_PN_Q_MASK_LOWER_WB /* Bits 7:0 */
#define DMOD_QMASK_1          FFE_FN_PN_Q_MASK_UPPER_WB /* Bits 14:0 */
#define DMOD_WALSH            FFE_FN_CODE_CHAN_ID_WB
#define DMOD_SLEW_0           FFE_SLEW_VALUE_LOWER_WB   /* Bits 7:0 */
#define DMOD_SLEW_1           FFE_SLEW_VALUE_MID_WB     /* Bits 15:8 */
#define DMOD_SLEW_2           FFE_SLEW_VALUE_UPPER_WB   /* Bits 18:16 */
#define DMOD_F_CTL_1          FFE_FINGER_CTL1_WB
#define DMOD_PILOT_GAIN       FFE_PILOT_FILT_GAIN_WB
#define DMOD_F_PWR_THRESH     FFE_FN_PWR_THRESH_WB
#define DMOD_F_LOCK_LO        FFE_LOCK_THRESH_LOWER_WB  /* Not a paged reg in MSM23 */
#define DMOD_F_LOCK_UP        FFE_LOCK_THRESH_UPPER_WB  /* Not a paged reg in MSM23 */
#define DMOD_FREQ_COMB_GAIN   FREQ_COMB_GAIN_WB
#define DMOD_FREQ_COMB_CTL    FREQ_COMB_CTL_WB
#define DMOD_CAR_FREQ_0       CARRIER_FREQ_ERR_WR_LSB_WB  /* Bits 7:0 */
#define DMOD_CAR_FREQ_1       CARRIER_FREQ_ERR_WR_MSB_WB  /* Bits 15:8 */
#define DMOD_W_SC_CTL_0       SYMB_COMB_CTL0_WB

#ifdef T_MSM5000
    #define DMOD_SC_CTL_1         SYMB_COMB_CTL1_WH
#else
    #define DMOD_SC_CTL_1         SYMB_COMB_CTL1_WB
#endif

#define DMOD_POS_LATCH        SYMB_COMB_POS_DUMP_WB
#define DMOD_FREQ_ADJ_0       SYMB_COMB_FREQ_ADJ_LOWER_WB  /* Bits 7:0 */
#define DMOD_FREQ_ADJ_1       SYMB_COMB_FREQ_ADJ_UPPER_WB  /* Bits 15:8 */
#define DMOD_W_I_PDM          I_OFFSET_WR_WB
#define DMOD_W_Q_PDM          Q_OFFSET_WR_WB
#define DMOD_AGC_CTL          IQ_OFFSET_CTL_WB
#define DMOD_LONG_0           SYMB_COMB_LONG_CODE_LD_0_WB  /* Bits 7:0 */
#define DMOD_LONG_1           SYMB_COMB_LONG_CODE_LD_1_WB  /* Bits 15:8 */
#define DMOD_LONG_2           SYMB_COMB_LONG_CODE_LD_2_WB  /* Bits 23:16 */
#define DMOD_LONG_3           SYMB_COMB_LONG_CODE_LD_3_WB  /* Bits 31:24 */
#define DMOD_LONG_4           SYMB_COMB_LONG_CODE_LD_4_WB  /* Bits 39:32 */
#define DMOD_LONG_5           SYMB_COMB_LONG_CODE_LD_5_WB  /* Bits 41:40 */
#define DMOD_LONG_MASK_0      SYMB_COMB_LONG_CODE_MASK_0_WB   /* Bits 7:0 */
#define DMOD_LONG_MASK_1      SYMB_COMB_LONG_CODE_MASK_1_WB   /* Bits 15:8 */
#define DMOD_LONG_MASK_2      SYMB_COMB_LONG_CODE_MASK_2_WB   /* Bits 23:16 */
#define DMOD_LONG_MASK_3      SYMB_COMB_LONG_CODE_MASK_3_WB   /* Bits 31:24 */
#define DMOD_LONG_MASK_4      SYMB_COMB_LONG_CODE_MASK_4_WB   /* Bits 39:32 */
#define DMOD_LONG_MASK_5      SYMB_COMB_LONG_CODE_MASK_5_WB   /* Bits 41:40 */
#define DMOD_PDM2             PDM2_CTL_WB
#define DMOD_PA_VGG_CTL       PDM2_CTL_WB
#define DMOD_PDM1             PDM1_CTL_WB
#define DMOD_TX_GAIN_COMP     PDM1_CTL_WB
#define DMOD_TCXO_BLK_TSEN    TCXO_PDM_CTL_WB
#define DMOD_EPOCH            EPOCH_WR_WB
#define DMOD_F_PAGE           FFE_FINGER_PAGE_REG_WB
#define DMOD_INT_CLEAR_0      INT_CLEAR_0_WH
#define DMOD_INT_MASK_0       IRQ_MASK_0_WH
#define DMOD_INT_CLEAR_1      INT_CLEAR_1_WH
#define DMOD_INT_MASK_1       IRQ_MASK_1_WH
#define DMOD_GPIO_OUT         GPIO_OUT_0_WB
#define DMOD_GPIO_TSEN        GPIO_TSEN_0_WB
#define DMOD_GPIO_INT_OUT     GPIO_INT_OUT_WB
#define DMOD_GPIO_INT_TSEN    GPIO_INT_TSEN_WB
#define DMOD_POLARITY         INT_POLARITY_WH
#define DMOD_SLEEP_CTL        SLEEP_CTL_WB
#define DMOD_SLEEP_TIME_0     POWER_DOWN_SLEEP_INTERVAL_0_WH  /* Bits 7:0 */
#define DMOD_SLEEP_TIME_1     POWER_DOWN_SLEEP_INTERVAL_0_WH  /* Bits 15:8 */
#define DMOD_SLEEP_TIME_2     POWER_DOWN_SLEEP_INTERVAL_1_WH  /* Bits 23:16 */
#define DMOD_SLEEP_TIME_3     POWER_DOWN_SLEEP_INTERVAL_1_WH  /* Bits 31:24 */
#define DMOD_WARM_TIME_0      POWER_DOWN_WU_TIME_WH     /* Bits 7:0 */
#define DMOD_WARM_TIME_1      POWER_DOWN_WU_TIME_WH     /* Bits 15:8 */
#define DMOD_FM_CLK_MDIV_0    YAMN1_CLK_MDIV_LSB_WB
#define DMOD_FM_CLK_MDIV_1    YAMN1_CLK_MDIV_MSB_WB     /* MSM23 Only */
#define DMOD_FM_CLK_NDIV_0    YAMN1_CLK_NDIV_LSB_WB
#define DMOD_FM_CLK_NDIV_1    YAMN1_CLK_NDIV_MSB_WB     /* MSM23 Only */
#define DMOD_FM_CLK_DUTY_0    YAMN1_CLK_DUTY_LSB_WB
#define DMOD_FM_CLK_DUTY_1    YAMN1_CLK_DUTY_MSB_WB     /* MSM23 Only */

/*---------------------------------------------------------------------------
                          Read Registers
---------------------------------------------------------------------------*/

#define DMOD_R_SRCH_CTL       SRCH_DMA_ERROR_RB
#define DMOD_SP_0             SRCH_POSITION_LOW_RB      /* Bits 7:0 */
#define DMOD_SP_1             SRCH_POSITION_MID_RB      /* Bits 15:8 */
#define DMOD_SP_2             SRCH_POSITION_HIGH_RB     /* Bits 17:16 */
#define DMOD_DMA_0            SRCH_DMA_DATA_RB
#define DMOD_SRCH_MAX_ENG_0   SRCH_MAX_ENERGY_LOW_RB    /* Bits 7:0 */
#define DMOD_SRCH_MAX_ENG_1   SRCH_MAX_ENERGY_HIGH_RB   /* Bits 15:8 */
#define DMOD_SRCH_MAX_SEL     SRCH_MAX_SELECT_WB        /* Bits 1:0 */
#define DMOD_SRCH_MAX_INX_0   SRCH_MAX_INDEX_LOW_RB     /* Bits 7:0 */
#define DMOD_SRCH_MAX_INX_1   SRCH_MAX_INDEX_HIGH_RB    /* Bits 10:8 */
#define DMOD_R_PCPD           TX_GAIN_ADJ_RD_RB
#define DMOD_LOCK_RSSI_GAIN   FFE_RSSI_FILT_GAIN_WB     /* Bits 7:0 */
#define DMOD_R_FRSSI_0        FN_LOCK_RSSI_LOWER_RB     /* Bits 7:0 */
#define DMOD_R_FRSSI_1        FN_LOCK_RSSI_UPPER_RB     /* Bits 15:8 */
#define DMOD_POS_0            FN_POSITION_LOWER_RB      /* Bits 7:0 */
#define DMOD_POS_1            FN_POSITION_MID_RB        /* Bits 15:8 */
#define DMOD_POS_2            FN_POSITION_UPPER_RB      /* Bits 17:16 */
#define DMOD_FREQ_SET_0       CARRIER_FREQ_ERR_RD_LSB_RB  /* Bits 7:0 */
#define DMOD_FREQ_SET_1       CARRIER_FREQ_ERR_RD_MSB_RB  /* Bits 15:8 */
#define DMOD_R_SC_CTL_0       SYMB_COMB_STATUS0_RB
#define DMOD_POS_PW_0         SYMB_COMB_POSITION2_LOWER_RB  /* Bits 7:0 */
#define DMOD_POS_PW_1         SYMB_COMB_POSITION2_UPPER_RB  /* Bits 15:8 */
#define DMOD_R_I_PDM          I_OFFSET_RD_RB
#define DMOD_R_Q_PDM          Q_OFFSET_RD_RB
#define DMOD_R_LONG_0         SYMB_COMB_LONG_CODE_RD_0_RB   /* Bits 7:0 */
#define DMOD_R_LONG_1         SYMB_COMB_LONG_CODE_RD_1_RB   /* Bits 15:8 */
#define DMOD_R_LONG_2         SYMB_COMB_LONG_CODE_RD_2_RB   /* Bits 23:16 */
#define DMOD_R_LONG_3         SYMB_COMB_LONG_CODE_RD_3_RB   /* Bits 31:24 */
#define DMOD_R_LONG_4         SYMB_COMB_LONG_CODE_RD_4_RB   /* Bits 39:32 */
#define DMOD_R_LONG_5         SYMB_COMB_LONG_CODE_RD_5_RB   /* Bits 41:40 */
#define DMOD_INT_STATUS_0     INT_STATUS_0_RH
#define DMOD_INT_STATUS_1     INT_STATUS_1_RH
#define DMOD_GPIO_IN          GPIO_IN_0_RB
#define DMOD_GPIO_INT_IN      GPIO_INT_IN_RB
#define DMOD_KEY_READ_IN      KEYSENSE_RD_RB

//UNUSED REGISTERS
#define DMOD_INT_CLEAR_2        MSM3_DUMMY
#define DMOD_INT_MASK_2         MSM3_DUMMY
#define DMOD_INT_STATUS_2       MSM3_DUMMY
#define DMOD_W_AGC_PDM          MSM3_DUMMY     // DMOD_IO(0x90)
#define DMOD_SCAN_IN            MSM3_DUMMY     // DMOD_IO(0xF0)
#define DMOD_PHI1               MSM3_DUMMY     // DMOD_IO(0xF8)
#define DMOD_PHI2               MSM3_DUMMY     // DMOD_IO(0xFA)
#define DMOD_TE                 MSM3_DUMMY     // DMOD_IO(0xFC)
#define DMOD_VER_STAT           MSM3_DUMMY     // DMOD_IO(0x00)
#define DMOD_R_TTA_0            MSM3_DUMMY     // DMOD_IO(0x44)
#define DMOD_R_TTA_1            MSM3_DUMMY     // DMOD_IO(0x46)
#define DMOD_F_PILOT_I          MSM3_DUMMY     // DMOD_IO(0x48)
#define DMOD_F_PILOT_Q          MSM3_DUMMY     // DMOD_IO(0x4A)
#define DMOD_POS_SD_0           MSM3_DUMMY     // DMOD_IO(0x82)
#define DMOD_POS_SD_1           MSM3_DUMMY     // DMOD_IO(0x84)
#define DMOD_POS_SD_2           MSM3_DUMMY     // DMOD_IO(0x86)
#define DMOD_R_AGC_PDM          MSM3_DUMMY     // DMOD_IO(0x90)
#define DMOD_SCAN_OUT           MSM3_DUMMY     // DMOD_IO(0xF0)
#define DMOD_BAUD_CLK_DUTY_0    MSM3_DUMMY     // DMOD_IO(0x3A)
#define DMOD_BAUD_CLK_DUTY_1    MSM3_DUMMY     // DMOD_IO(0x3C)
#define DMOD_BAUD_CLK_M_0       MSM3_DUMMY     // DMOD_IO(0x0C)
#define DMOD_BAUD_CLK_M_1       MSM3_DUMMY     // DMOD_IO(0x0E)
#define DMOD_BAUD_CLK_N_0       MSM3_DUMMY     // DMOD_IO(0x1C)
#define DMOD_BAUD_CLK_N_1       MSM3_DUMMY     // DMOD_IO(0x1E)
#define DMOD_F_CTL_0            MSM3_DUMMY     // DMOD_IO(0x5E)
#define DMOD_SCAN_CTL_0         MSM3_DUMMY     // DMOD_IO(0xF2)
#define DMOD_SCAN_CTL_1         MSM3_DUMMY     // DMOD_IO(0xF4)

/*===========================================================================

                        END DMOD.H ALIAS

===========================================================================*/

/*===========================================================================

                        START ENCI.H ALIAS

===========================================================================*/


/*---------------------------------------------------------------------------
                          Write Registers
---------------------------------------------------------------------------*/

#define ENC_I_PN_S0           I_PN_STATE_0_WB           /* bits 7:0  */
#define ENC_I_PN_S1           I_PN_STATE_1_WB           /* bits 14:8 */
#define ENC_Q_PN_S0           Q_PN_STATE_0_WB           /* bits 7:0  */
#define ENC_Q_PN_S1           Q_PN_STATE_1_WB           /* bits 14:8 */
#define ENC_U_PN_S0           U_PN_STATE_0_WB           /* bits 7:0   */
#define ENC_U_PN_S1           U_PN_STATE_1_WB           /* bits 15:8  */
#define ENC_U_PN_S2           U_PN_STATE_2_WB           /* bits 23:16 */
#define ENC_U_PN_S3           U_PN_STATE_3_WB           /* bits 31:24 */
#define ENC_U_PN_S4           U_PN_STATE_4_WB           /* bits 39:32 */
#define ENC_U_PN_S5           U_PN_STATE_5_WB           /* bits 41:40 and bozo bit */
#define ENC_U_PN_M0           U_PN_MASK_0_WB            /* bits 7:0   */
#define ENC_U_PN_M1           U_PN_MASK_1_WB            /* bits 15:8  */
#define ENC_U_PN_M2           U_PN_MASK_2_WB            /* bits 23:16 */
#define ENC_U_PN_M3           U_PN_MASK_3_WB            /* bits 31:24 */
#define ENC_U_PN_M4           U_PN_MASK_4_WB            /* bits 39:32 */
#define ENC_U_PN_M5           U_PN_MASK_5_WB            /* bits 41:40 */
#define ENC_PA_WARMUP         PA_WARMUP_WB
#define ENC_WSYM_STATE        WSYM_STATE_WB
#define ENC_TXSYNC_ST_0       TXSYNC_ST_0_WB            /* Bits 7:0   */
#define ENC_TXSYNC_ST_1       TXSYNC_ST_1_WB            /* Bits 13:8  */
#define ENC_SYSFR_STATE       SYSFR_STATE_WB            /* Bits 5:0   */
#define ENC_INT_ST            ENC_INT_ST_WB
#define ENC_DATA_WR           ENC_DATA_WB
#define ENC_CTL               ENC_CTL_WB                /* Encoder Control */

#ifndef T_MSM5000           // 16-bit register in MSM5000
#define ENC_CLK_CTL           MOD_CLK_CTL_WB            /* Clock Control */
#else
#define ENC_CLK_CTL           MOD_CLK_CTL_WH
#endif

#define ENC_MISC_CTL          MOD_MISC_CTL_WB           /* Miscellanious control */
#define ENC_FRAME_OFF         FRAME_OFF_WB
#define ENC_GPIO_0_OUT        GPIO_OUT_1_WB
#define ENC_GPIO_1_OUT        GPIO_OUT_2_WB
#define ENC_GPIO_0_TSEN       GPIO_TSEN_1_WB
#define ENC_GPIO_1_TSEN       GPIO_TSEN_2_WB
#define ENC_TX_TST            TX_DATA_TEST_WB

// MOD_TEST_CTL is a 16-bit register (9 bits used) in MSM5000
#ifdef T_MSM5000
#define ENC_TST_CTL           MOD_TEST_CTL_WH
#else
#define ENC_TST_CTL           MOD_TEST_CTL_WB
#endif

#define ENC_CRC_UB            CRC_MSB_WB                /* CRC Polynomial Upper Bits     */
#define ENC_CRC_LB            CRC_LSB_WB                /* CRC Polynomial Lower Bits     */
#define ENC_TEST_POINT_SEL    TEST_POINT_SEL_WH

/*---------------------------------------------------------------------------
                       Address-Activated Registers
---------------------------------------------------------------------------*/

#define ENC_RESET             MOD_RESET_WB              /* Reset the chip state. */
#define ENC_I_CLK             TX_I_CLK_WB               /* Pulse TX_I_SCLK in test mode */

/*---------------------------------------------------------------------------
                             Read Registers
---------------------------------------------------------------------------*/

#define ENC_MASK_DATA         MASK_DATA_RB              /* MASK_DATA from DBR         */
#define ENC_STATUS            MOD_STATUS_RB
#define ENC_GPIO_0_IN         GPIO_IN_1_RB
#define ENC_GPIO_1_IN         GPIO_IN_2_RB


//UNUSED REGISTERS
#define ENC_ADDR0               MSM3_DUMMY     // ENC_IO(0x40)
#define ENC_ADDR1               MSM3_DUMMY     // ENC_IO(0x42)
#define ENC_RAM_CTL             MSM3_DUMMY     // ENC_IO(0x44)
#define ENC_RAM_IN              MSM3_DUMMY     // ENC_IO(0x46)
#define ENC_SCAN_IN             MSM3_DUMMY     // ENC_IO(0x4A)
#define ENC_SCAN_CTL            MSM3_DUMMY     // ENC_IO(0x4e)
#define ENC_VERSION             MSM3_DUMMY     // ENC_IO(0x00)
#define ENC_SCAN_SEL            MSM3_DUMMY     // ENC_IO(0x50)
#define ENC_SCAN_OUT            MSM3_DUMMY     // ENC_IO(0x04)



#define ENC_SC_PH1              MSM3_DUMMY     // ENC_IO(0x62)
#define ENC_SC_PH2              MSM3_DUMMY     // ENC_IO(0x64)
#define ENC_UPDT_STAT           MSM3_DUMMY     // ENC_IO(0x66)
#define ENC_UP_CLK              MSM3_DUMMY     // ENC_IO(0x68)
#define ENC_Q_CLK               MSM3_DUMMY     // ENC_IO(0x6c)
#define ENC_VOC_ACCESS          MSM3_DUMMY     // ENC_IO(0x70)
#define ENC_ENC_TEST            MSM3_DUMMY     // ENC_IO(0x06)
#define ENC_INT_TEST            MSM3_DUMMY     // ENC_IO(0x08)

/*===========================================================================

                        END ENCI.H ALIAS

===========================================================================*/

/*===========================================================================

                        START CAGC.H ALIAS
                        START RFG.C ALIAS

===========================================================================*/

#define CAGC_CNTL             AGC_CTL_WB
#define RF_CAGC_CNTL          AGC_CTL_WB
#define CAGC_OUT_CTL          AGC_CTL2_WB
#define RF_CAGC_OUT_CTL       AGC_CTL2_WB
#define CAGC_DATA_1           RX_AGC_ADJ_WR_WB
#define RF_CAGC_DATA_1        RX_AGC_ADJ_WR_WB
#define CAGC_DATA_2           TX_AGC_ADJ_WR_MSB_WB
#define RF_CAGC_DATA_2        TX_AGC_ADJ_WR_MSB_WB
// AGC_CTL3 is a 16-bit register (9 bits used) in MSM5000
#ifdef T_MSM5000
#define CAGC_DATA_3           AGC_CTL3_WH
#define RF_CAGC_DATA_3        AGC_CTL3_WH
#else
#define CAGC_DATA_3           AGC_CTL3_WB
#define RF_CAGC_DATA_3        AGC_CTL3_WB
#endif

#define CAGC_DATA_4           AGC_CTL4_WB
#define RF_CAGC_DATA_4        AGC_CTL4_WB
#define CAGC_CTL_5            AGC_CTL5_WB
#define RF_CAGC_CTL_5         AGC_CTL5_WB
#define CAGC_CTL_6            AGC_CTL6_WB
#define RF_CAGC_CTL_6         AGC_CTL6_WB
#define CAGC_AGC_VALUE_MAX    AGC_VALUE_MAX_WB
#define RF_AGC_VALUE_MAX      AGC_VALUE_MAX_WB
#define CAGC_AGC_VALUE_MIN    AGC_VALUE_MIN_WB
#define RF_AGC_VALUE_MIN      AGC_VALUE_MIN_WB
#define CAGC_IM_STATE_CTL     IM_STATE_CTL_WB
#define RF_IM_STATE_CTL       IM_STATE_CTL_WB
#define CAGC_PWR_LIM_LB       TX_GAIN_LIMIT_LSB_WB
#define RF_CAGC_PWR_LIM_LB    TX_GAIN_LIMIT_LSB_WB
#define CAGC_PA_R1_RISE       PA_R1_RISE_WB
#define RF_PA_R1_RISE         PA_R1_RISE_WB
#define CAGC_PA_R1_FALL       PA_R1_FALL_WB
#define RF_PA_R1_FALL         PA_R1_FALL_WB
#define CAGC_PA_R2_RISE       PA_R2_RISE_WB
#define RF_PA_R2_RISE         PA_R2_RISE_WB
#define CAGC_PA_R2_FALL       PA_R2_FALL_WB
#define RF_PA_R2_FALL         PA_R2_FALL_WB
#define CAGC_PA_R3_RISE       PA_R3_RISE_WB
#define RF_PA_R3_RISE         PA_R3_RISE_WB
#define CAGC_PA_R3_FALL       PA_R3_FALL_WB
#define RF_PA_R3_FALL         PA_R3_FALL_WB
#define CAGC_LNA_SWITCH_LB    LNA_OFFSET_LSB_WB
#define RF_LNA_SWITCH_LB      LNA_OFFSET_LSB_WB
#define CAGC_LNA_FALL         LNA_FALL_WB
#define RF_LNA_FALL           LNA_FALL_WB
#define CAGC_LNA_RISE         LNA_RISE_WB
#define RF_LNA_RISE           LNA_RISE_WB
#define CAGC_RAS_RAM_DATA_UB  RAS_RAM_DATA_MSB_WB
#define CAGC_RAS_RAM_DATA_LB  RAS_RAM_DATA_LSB_WB
#define RF_RAS_RAM_DATA_UB    RAS_RAM_DATA_MSB_WB
#define RF_RAS_RAM_DATA_LB    RAS_RAM_DATA_LSB_WB
#define CAGC_RAS_RAM_CNTL     AGC_RDWR_CTL_WB
#define RF_RAS_RAM_CNTL       AGC_RDWR_CTL_WB
#define CAGC_TEST_CNTL        AGC_TEST_CTL_WB
#define RF_CAGC_TEST_CNTL     AGC_TEST_CTL_WB
#define CAGC_AGC_VALUE        AGC_VALUE_WR_WB
#define RF_CAGC_AGC_VALUE     AGC_VALUE_WR_WB
#define CAGC_TX_GAIN_CTL_UB   TX_GAIN_CTL_MSB_RB
#define RF_CAGC_TX_GAIN_CTL_UB TX_GAIN_CTL_MSB_RB
#define CAGC_TX_GAIN_CTL_LB    TX_GAIN_CTL_LSB_RB
#define RF_CAGC_TX_GAIN_CTL_LB TX_GAIN_CTL_LSB_RB
#define RF_SYNTH_STATUS_R_X    PA_ON_STATUS_RB

/*===========================================================================

                        END CAGC.H ALIAS
                        END RFG.C ALIAS

===========================================================================*/


/*===========================================================================

                        START DFM.H ALIAS

===========================================================================*/

/*---------------------------------------------------------------------------
                      IQ Demodulator WRITE Registers
---------------------------------------------------------------------------*/

#define DFM_INITIAL           DFM_INITIAL_WB
#define DFM_DC_OFFSET_GAIN    DFM_DC_OFFSET_GAIN_WB
#define DFM_AGC_REF           DFM_AGC_REF_WB
#define DFM_AGC_ACC_MIN       DFM_AGC_ACC_MIN_WB
#define DFM_AGC_ACC_MAX       DFM_AGC_ACC_MAX_WB
#define DFM_AGC_GAIN          DFM_AGC_GAIN_WB
#define DFM_FREQ_LOOP_CFG     DFM_FREQ_LOOP_CONFIG_WB
#define DFM_PDM_CFG           DFM_PDM_CONFIG_WB
#define DFM_I_PDM_LSB         DFM_DC_PDM_0_WB
#define DFM_Q_PDM_LSB         DFM_DC_PDM_1_WB
#define DFM_IQ_PDM_MSB        DFM_DC_PDM_2_WB
#define DFM_RX_AGC_PDM        DFM_RXAGC_PDM_0_WB
#define DFM_FREQ_TRK_PDM_LSB  DFM_FREQ_PDM_0_WB
#define DFM_FREQ_TRK_PDM_MSB  DFM_FREQ_PDM_1_WB
#define DFM_VOC_INTF_CFG      DFM_VOC_INTF_CONFIG_WB
#define DFM_IQDMOD_LOOP_STAT_0 DFM_RXIQ_STATUS_0_RB
#define DFM_IQDMOD_LOOP_STAT_1 DFM_RX_AGC_FILTER_RB
#define DFM_IQDMOD_LOOP_STAT_2 DFM_RX_AGC_RSSI_RB

/*---------------------------------------------------------------------------
                     TX Wideband Data WRITE Registers
---------------------------------------------------------------------------*/

#define DFM_TX_WBD_MSB        DFM_TXWBD_INTF_0_WB
#define DFM_TX_WBD_LSB        DFM_TXWBD_INTF_1_WB
#define DFM_TX_AGC_PDM_LSB    DFM_MAX_TX_PWR_0_WB
#define DFM_TX_AGC_PDM_MSB    DFM_MAX_TX_PWR_1_WB
#define DFM_FREQ_SENSE_GAIN   DFM_FREQ_SENS_GAIN_WB
#define DFM_TX_FM_CFG         DFM_TXFM_CONFIG_WB

/*---------------------------------------------------------------------------
                      TX Wideband Data READ Registers
---------------------------------------------------------------------------*/

#define DFM_TX_WBD_STAT       DFM_TXWBD_STATUS_RB


/*---------------------------------------------------------------------------
                      RX Wideband Data WRITE Registers
---------------------------------------------------------------------------*/

#define DFM_MIN1_0_7          DFM_MIN1_BYTE_0_WB        /* MIN1 bits 0..7  */
#define DFM_MIN1_8_15         DFM_MIN1_BYTE_1_WB        /* 8..15           */
#define DFM_MIN1_16_23        DFM_MIN1_BYTE_2_WB        /* 16..23          */
#define DFM_RX_WBD_BW         DFM_RXWBD_BANDWIDTH_WB
#define DFM_RX_WBD_CFG        DFM_RXWBD_CONFIG_0_WB
#define DFM_RX_WBD_WR         DFM_RXWBD_WR_WB

/*---------------------------------------------------------------------------
                     RX Wideband Data READ Registers
---------------------------------------------------------------------------*/

#define DFM_RX_WBD_0_7        DFM_RXWBD_STAT_0_RB
#define DFM_RX_WBD_8_15       DFM_RXWBD_STAT_1_RB
#define DFM_RX_WBD_16_23      DFM_RXWBD_STAT_2_RB
#define DFM_RX_WBD_24_27      DFM_RXWBD_STAT_3_RB
#define DFM_RX_WBD_RD         DFM_RXWBD_RD_RB
#define DFM_WORD_SYNC_COUNT   DFM_WORD_SYNC_COUNT_RB

/*===========================================================================

                        END DFM.H ALIAS

===========================================================================*/


/*===========================================================================

                        START SNDRING.H ALIAS

===========================================================================*/

#define SND_RINGER_A          RINGER_MN_A_DUTY_LSB_WB
#define SND_RINGER_B          RINGER_MN_B_DUTY_LSB_WB

/*===========================================================================

                        END SNDRING.H ALIAS

===========================================================================*/


/*===========================================================================

                        START CLKM2P.C AND CLKARM.C ALIAS

===========================================================================*/

#define CLK_TICK_CTL          TIME_TICK_CTL_WB
#define CLK_TICK_CNT          TIME_TICK_INT_MSB_RB

/*===========================================================================

                        END CLKM2P.C AND CLKARM.C ALIAS

===========================================================================*/


/*===========================================================================

                        START VOC_CORE.H

===========================================================================*/

#define VC_RESET                 VOC_RESET_WB
#define VC_ENC_FRAME_ADVANCE     ENC_FRAME_ADVANCE_WB
#define VC_DEC_FRAME_ADVANCE     DEC_FRAME_ADVANCE_WB
#define VC_DEC_INT_ADVANCE       DEC_INT_ADVANCE_WB

#define VC_TX_PCM_CONTROL        TX_PCM_CONTROL_WB
#define VC_RX_PCM_CONTROL        RX_PCM_CONTROL_WB

#define VC_DEC_PCM_WR_MSB        DEC_PCM_WR_MSB_WB
#define VC_DEC_PCM_WR_LSB        DEC_PCM_WR_LSB_WB

#define VC_TEST_CONTROL_1        TEST_CONTROL_1_WB
#define VC_TEST_CONTROL_2        MSM_CLK_CTL5_WB

#define VC_MODE_CONTROL          MODE_CONTROL_WB
#define VC_VOX_CONTROL           VOX_CONTROL_WB
#define VC_PEEK_POKE_CONTROL     PEEK_POKE_CONTROL_WB

#define VC_ENC_SAMP_CNT          VOC_ENC_SAMP_CNT_RB
#define VC_POKE_ADDR_LSB         PEEK_POKE_ADDR_LSB_WB

#define VC_DEC_SAMP_CNT          VOC_DEC_SAMP_CNT_RB
#define VC_POKE_DATA_MSB         POKE_DATA_MSB_WB
#define VC_POKE_DATA_LSB         POKE_DATA_LSB_WB

#define VC_ENC_RATE_CONTROL      ENC_RATE_CONTROL_WB
#define VC_ENC_RATE_LIMIT_FACTOR ENC_RATE_LIMIT_FACTOR_WB
#define VC_FM_AUDIO_CONFIG       FM_AUDIO_CONFIG_WB

/* Alias VOX Parameter Registers (for qdsp1) as AGC control for qdsp1+. */
#define VC_ENERGY_DECAY_MSB      ENERGY_DECAY_FAC_MSB_WB
#define VC_ENERGY_DECAY_LSB      ENERGY_DECAY_FAC_LSB_WB
#define VC_BG_NOISE_DECAY_MSB    BGN_DECAY_FAC_MSB_WB
#define VC_BG_NOISE_DECAY_LSB    BGN_DECAY_FAC_LSB_WB
#define VC_FWD_AGC_MSB           ENERGY_DECAY_FAC_MSB_WB
#define VC_FWD_AGC_LSB           ENERGY_DECAY_FAC_LSB_WB
#define VC_REV_AGC_MSB           BGN_DECAY_FAC_MSB_WB
#define VC_REV_AGC_LSB           BGN_DECAY_FAC_LSB_WB

#define VC_FM_TEST_CONTROL       FM_TEST_CTL_WB

#define VC_SAT_ECC               VOC_FM_CONFIG_WB
#define VC_SAT_LEVEL             SAT_LEVEL_WB
#define VC_SAT_THRESHOLD         SAT_THRESHOLD_WB

/* Alias Dec Voc Thresh MSB Register (qdsp1) as NS Control for qdsp1+. */
#define VC_NS_CONTROL            DEC_VOX_THRESHOLD_MSB_WB
#define VC_DEC_VOX_THRESH_MSB    DEC_VOX_THRESHOLD_MSB_WB

#define VC_DEC_VOX_THRESH_LSB    DEC_VOX_THRESHOLD_LSB_WB

#define VC_TX_DEVIATION_LIMIT    TX_DEVIATION_LIMIT_WB
#define VC_FM_TX_GAIN            FM_TX_GAIN_WB

/* Alias Enc Weighting MSB Registers (qdsp1) as VR Control for qdsp1+. */
#define VC_VR_CONTROL_MSB        ENC_WEIGHTING_FAC_MSB_WB
#define VC_VR_CONTROL_LSB        ENC_WEIGHTING_FAC_LSB_WB
#define VC_ENC_WEIGHTING_MSB     ENC_WEIGHTING_FAC_MSB_WB
#define VC_ENC_WEIGHTING_LSB     ENC_WEIGHTING_FAC_LSB_WB

#define VC_FM_RX_GAIN            FM_RX_GAIN_WB

#define VC_BG_NOISE_FLOOR_MSB    DEC_BGN_FLOOR_MSB_WB
#define VC_BG_NOISE_FLOOR_LSB    DEC_BGN_FLOOR_LSB_WB

#define VC_DTMF_DURATION_MSB     DTMF_DURATION_MSB_WB
#define VC_DTMF_DURATION_LSB     DTMF_DURATION_LSB_WB

#define VC_DTMF_HIGH_MSB         DTMF_HIGH_MSB_WB
#define VC_DTMF_HIGH_LSB         DTMF_HIGH_LSB_WB

#define VC_DTMF_LOW_MSB          DTMF_LOW_MSB_WB
#define VC_DTMF_LOW_LSB          DTMF_LOW_LSB_WB

#define VC_DTMF_VOL_MSB          DTMF_VOLUME_MSB_WB
#define VC_DTMF_VOL_LSB          DTMF_VOLUME_LSB_WB

#define VC_FM_DTMF_RX_GAIN       DTMF_FM_RX_GAIN_WB
#define VC_FM_DTMF_TX_GAIN       DTMF_FM_TX_GAIN_WB

#define VC_DEC_VOLUME_MSB        DEC_VOLUME_MSB_WB
#define VC_DEC_VOLUME_LSB        DEC_VOLUME_LSB_WB

#define VC_PCM_PAD_CONTROL       PCM_PAD_CONTROL_WB

#define VC_PCM_CONTROL_WORD_MSB  PCM_CONTROL_WORD_MSB_WB
#define VC_PCM_CONTROL_WORD_LSB  PCM_CONTROL_WORD_LSB_WB

#define VC_IO_CONTROL            IO_CONTROL_WB

#define VC_CLEAR_ENC_INT         CLEAR_ENC_INT_WB
#define VC_CLEAR_DEC_INT         CLEAR_DEC_INT_WB

#define VC_DEC_PACKET            DEC_PACKET_WB

#define VC_AUX_CODEC_CONTROL     CODEC_CTL1_WB

/* Following register is 9-bit in MSM5000 */
#ifdef T_MSM5000
#define VC_GPIO_2930_CTL         GPIO_FUNCTION_SEL_WH
#else
#define VC_GPIO_2930_CTL         GPIO_FUNCTION_SEL_WB
#endif

#define VC_ONES_POLARITY         CODEC_CTL2_WB

#define VC_ENC_PCM_RD_MSB        ENC_PCM_RD_MSB_RB
#define VC_ENC_PCM_RD_LSB        ENC_PCM_RD_LSB_RB

#define VC_ENC_PCM_WR_MSB        ENC_PCM_WR_MSB_WB
#define VC_ENC_PCM_WR_LSB        ENC_PCM_WR_LSB_WB

#define VC_STATUS                VOC_STATUS_RB
#define VC_HW_VERSION            HW_REVISION_NUMBER_RH

#define VC_FM_STATUS             FM_STATUS_RB
#define VC_ENC_PACKET            ENC_PACKET_RB

//UNUSED REGISTERS
#define VC_SW_VERSION            MSM3_DUMMY

/*===========================================================================

                        END VOC_CORE.H

===========================================================================*/


/*===========================================================================

                        START VOC13K.C

===========================================================================*/

//UNUSED REGISTERS
#define  VOCEXT_IO              MSM3_DUMMY       // (P_PCS6_IO+0x00)
#define  VOC13K_IO              MSM3_DUMMY       // added for PHONE_T

/*===========================================================================

                        END VOC13K.C

===========================================================================*/


/*===========================================================================

                        START DECI.H

===========================================================================*/

#define DEC_MODE           DECMODE_WB
#define DEC_SMTDATA        SMTDATA_WB
#define DEC_OBADDR         OBADDRESS_WB

#define DEC_Q2448          QT8_QT4_WB
#define DEC_Q96192         QT2_QT1_WB

#define DEC_GPIO_0_OUT     GPIO_OUT_3_WB
#define DEC_GPIO_1_OUT     GPIO_OUT_4_WB
#define DEC_GPIO_0_TSEN    GPIO_TSEN_3_WB
#define DEC_GPIO_1_TSEN    GPIO_TSEN_4_WB

#define DEC_TESTCON        DEC_TESTCON_WB

#define DEC_TESTMUX        DEC_TESTSEL_WB
#define DEC_TEST_RAM_SEL   TEST_MEM_SEL_WH
#define DEC_TEST_POINT_SEL TEST_POINT_SEL_WH
#define DEC_SYNC           DEC_TESTSYNC_WB
#define DEC_CRC            DEC_CRC_WH
#define DEC_RESET          DEC_RESET_WB

#define DEC_CLK_CTL_1     MSM_CLK_CTL1_WH
#define DEC_CLK_CTL_2     MSM_CLK_CTL2_WH
#define DEC_CLK_CTL_3     MSM_CLK_CTL3_WH
#define DEC_CLK_CTL_4     MSM_CLK_CTL4_WH

#define DEC_DATA          DECDATA_RH
#define DEC_STATUS        DECSTATUS_RB

#define DEC_SER8          SER8_RB
#define DEC_SER4          SER4_RB
#define DEC_SER2          SER2_RB
#define DEC_SER1          SER1_RB

#define DEC_GPIO_0_IN     GPIO_IN_3_RB
#define DEC_GPIO_1_IN     GPIO_IN_4_RB
#define DEC_TESTOUT       DEC_TESTOUT_RH

//UNUSED REGISTERS
#define DEC_SYMIN          MSM3_DUMMY
#define DEC_CLK            MSM3_DUMMY
#define DEC_PINCON         MSM3_DUMMY
#define DEC_SCAN_IN        MSM3_DUMMY
#define DEC_TEST_RAM_CS    MSM3_DUMMY
#define DEC_TEST_RAM_ADDR  MSM3_DUMMY
#define DEC_TEST_DATA      MSM3_DUMMY
#define DEC_SCAN_CTL       MSM3_DUMMY
#define DEC_SCAN_MUX       MSM3_DUMMY
#define DEC_PHI1           MSM3_DUMMY
#define DEC_PHI2           MSM3_DUMMY
#define DEC_ID             MSM3_DUMMY
#define DEC_ACSRAM         MSM3_DUMMY
#define DEC_SCAN_OUT       MSM3_DUMMY

/*===========================================================================

                        END DECI.H

===========================================================================*/



/*===========================================================================

                        START DLOAD2I.H SIOG2.C

===========================================================================*/

#ifndef T_MSM5000    //UARTCHG
#define UART_MR1     UART_MR1_WB
#define UART_IPR     UART_IPR_WB
#else
#define UART_MR1     UART_MR1_WH
#define UART_IPR     UART_IPR_WH
#endif            //END UARTCHG

#define UART_MR2     UART_MR2_WB
#define UART_SR      UART_SR_RB
#define UART_CSR     UART_CSR_WB
#define UART_MISR    UART_MISR_RB
#define UART_CR      UART_CR_WB
#define UART_ISR     UART_ISR_RB
#define UART_IMR     UART_IMR_WB
#define UART_HCR     UART_HCR_WB
#define UART_RFIFO   UART_RF_RB
#define UART_TFIFO   UART_TF_WB

#ifndef T_MSM5000 //UARTCHG
#define MSMU_MR1     UART_MR1_WB       /* Mode Register 1                  */
#define UART_TXWAT   UART_TFWR_WB
#define UART_RXWAT   UART_RFWR_WB
#else
#define MSMU_MR1     UART_MR1_WH       /* Mode Register 1                  */
#define UART_TXWAT   UART_TFWR_WH
#define UART_RXWAT   UART_RFWR_WH
#endif         //UARTCHG

#define UART_MVR     UART_MREG_WB
#define UART_NVR     UART_NREG_WB
#define UART_DVR     UART_DREG_WB
#define UART_MNDR    UART_MNDREG_WB


#define MSMU_MR2     UART_MR2_WB       /* Mode Register 2                  */
#define MSMU_SR      UART_SR_RB        /* Status Register                  */
#define MSMU_CSR     UART_CSR_WB       /* Clock Select Register            */
#define MSMU_RF      UART_RF_RB        /* Receive FIFO                     */
#define MSMU_TF      UART_TF_WB        /* Transmit FIFO                    */
#define MSMU_MISR    UART_MISR_RB      /* Masked Interrupt Status Register */
#define MSMU_CR      UART_CR_WB        /* Command Register                 */
#define MSMU_ISR     UART_ISR_RB       /* Interrupt Status Register        */
#define MSMU_IMR     UART_IMR_WB       /* Interrupt Mask Register          */

#ifndef T_MSM5000    //UARTCHG
#define MSMU_IPR     UART_IPR_WB       /* Interrupt Programming Register   */
#define MSMU_TFWR    UART_TFWR_WB      /* TX FIFO Watermark Register       */
#define MSMU_RFWR    UART_RFWR_WB      /* RX FIFO watermark Register       */
#else
#define MSMU_IPR     UART_IPR_WH       /* Interrupt Programming Register   */
#define MSMU_TFWR    UART_TFWR_WH      /* TX FIFO Watermark Register       */
#define MSMU_RFWR    UART_RFWR_WH      /* RX FIFO watermark Register       */
#endif            //END UARTCHG

#define MSMU_HCR     UART_HCR_WB       /* Hunt Character Register          */
#define MSMU_MVR     UART_MREG_WB      /* M Value Register                 */
#define MSMU_NVR     UART_NREG_WB      /* N Value Register                 */
#define MSMU_DVR     UART_DREG_WB      /* D Value Register                 */
#define MSMU_MNDR    UART_MNDREG_WB    /* Extra Value Register             */
#define MSMU_IRDA    UART_IRDA_WB      /* UART IRDA Register               */

/* These are definitions for second UART */
#define UART2_MR1     UART2_MR1_WB
#define UART2_MR2     UART2_MR2_WB
#define UART2_SR      UART2_SR_RB
#define UART2_CSR     UART2_CSR_WB
#define UART2_RFIFO   UART2_RF_RB
#define UART2_TFIFO   UART2_TF_WB
#define UART2_MISR    UART2_MISR_RB
#define UART2_CR      UART2_CR_WB
#define UART2_ISR     UART2_ISR_RB
#define UART2_IMR     UART2_IMR_WB
#define UART2_IPR     UART2_IPR_WB
#define UART2_TXWAT   UART2_TFWR_WB
#define UART2_RXWAT   UART2_RFWR_WB
#define UART2_HCR     UART2_HCR_WB
#define UART2_MVR     UART2_MREG_WB
#define UART2_NVR     UART2_NREG_WB
#define UART2_DVR     UART2_DREG_WB
#define UART2_MNDR    UART2_MNDREG_WB


#define MSMU2_MR1     UART2_MR1_WB       /* Mode Register 1                  */
#define MSMU2_MR2     UART2_MR2_WB       /* Mode Register 2                  */
#define MSMU2_SR      UART2_SR_RB        /* Status Register                  */
#define MSMU2_CSR     UART2_CSR_WB       /* Clock Select Register            */
#define MSMU2_RF      UART2_RF_RB        /* Receive FIFO                     */
#define MSMU2_TF      UART2_TF_WB        /* Transmit FIFO                    */
#define MSMU2_MISR    UART2_MISR_RB      /* Masked Interrupt Status Register */
#define MSMU2_CR      UART2_CR_WB        /* Command Register                 */
#define MSMU2_ISR     UART2_ISR_RB       /* Interrupt Status Register        */
#define MSMU2_IMR     UART2_IMR_WB       /* Interrupt Mask Register          */
#define MSMU2_IPR     UART2_IPR_WB       /* Interrupt Programming Register   */
#define MSMU2_TFWR    UART2_TFWR_WB      /* TX FIFO Watermark Register       */
#define MSMU2_RFWR    UART2_RFWR_WB      /* RX FIFO watermark Register       */
#define MSMU2_HCR     UART2_HCR_WB       /* Hunt Character Register          */
#define MSMU2_MVR     UART2_MREG_WB      /* M Value Register                 */
#define MSMU2_NVR     UART2_NREG_WB      /* N Value Register                 */
#define MSMU2_DVR     UART2_DREG_WB      /* D Value Register                 */
#define MSMU2_MNDR    UART2_MNDREG_WB    /* Extra Value Register             */
#define MSMU2_IRDA    UART2_IRDA_WB      /* UART IRDA Register               */

/*===========================================================================

                        END DLOAD2I.H SIOG2.C

===========================================================================*/



/*===========================================================================

                        START RUIM.H

===========================================================================*/

#define RUIM_CLK_MDIV_LSB    YAMN1_CLK_MDIV_LSB_WB
#define RUIM_CLK_MDIV_MSB    YAMN1_CLK_MDIV_MSB_WB
#define RUIM_CLK_NDIV_LSB    YAMN1_CLK_NDIV_LSB_WB
#define RUIM_CLK_NDIV_MSB    YAMN1_CLK_NDIV_MSB_WB
#define RUIM_CLK_DUTY_LSB    YAMN1_CLK_DUTY_LSB_WB
#define RUIM_CLK_DUTY_MSB    YAMN1_CLK_DUTY_MSB_WB

/*===========================================================================

                        END RUIM.H

===========================================================================*/


/*===========================================================================

                        START ADC.H
===========================================================================*/

#define ADC_CNTRL_STAT  ADC_RESET_WB
#define ADC_DATA        ADC_DATA_WR_WB

/*===========================================================================

                        END ADC.H

===========================================================================*/





/*===========================================================================

                        START HW.H

===========================================================================*/

//UNUSED REGISTERS
#define QCS_ROM_IO              MSM3_DUMMY     // (QCS_BASE+0x00)
#define QCS_RAM_IO              MSM3_DUMMY     // (QCS_BASE+0x02)
#define QCS_ETC_IO              MSM3_DUMMY     // (QCS_BASE+0x04)
#define QCS_LE_IO               MSM3_DUMMY     // (QCS_BASE+0x06)
#define QCS_EB_IO               MSM3_DUMMY     // (QCS_BASE+0x08)
#define QCS_BIU_IO              MSM3_DUMMY     // (QCS_BASE+0x00A)

/*===========================================================================

                        END HW.H

===========================================================================*/

/*===========================================================================

                        START BIO.H

===========================================================================*/

#define BIO_PORT_7_OUT          0x02800000      // (P_PCS0_IO + 0x3E)
                                                /* For MSM3000 this maps to GP_CS_N
                                                   only added here to complete mapping
                                                   not used in current SW  */

/*===========================================================================

                        END BIO.H

===========================================================================*/

//PHONE_T for PANTHER
#define LNA_FILT_WR                  LNA_FILT_WR_WB
#define LNA_GAIN_WR                  LNA_GAIN_WR_WB
#define LNA_GAIN_MIN                 LNA_GAIN_MIN_WB
#define LNA_GAIN_PWR_MAX     LNA_GAIN_PWR_MAX_WB
#define AGC_VALUE_MAX                AGC_VALUE_MAX_WB
#define AGC_VALUE_MIN                AGC_VALUE_MIN_WB
#define AGC_VALUE_WR                 AGC_VALUE_WR_WB
#define TX_OPEN_LOOP_WR              TX_OPEN_LOOP_WR_WB
#define IM_STATE_CTL                 IM_STATE_CTL_WB

#define RF_CAGC_LNA_CTL_R       LNA_PA_RD_RB

#define LEDS                    BIO_PORT_7_OUT   //PHONE_T


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#ifndef _ARM_ASM_

#define MSM_BASE          CHIP_BASE             /* Base MSM Memory Mapped address */

// These need to be reviewed with other users of MSM_OUT* macros
// --SR
#ifdef T_MSM5000
   #define MSM_END  (DINT_TD_WR_ADDR_WH + 4)   /* End MSM Memory Mapped address */
#else
   #define MSM_END  (UART_IRDA_WB + 4)      /* End MSM Memory Mapped address */
#endif
            /* Add 4 for MSM3_DUMMY location */

#define MSM_MAX_INX       (((MSM_END - CHIP_BASE)/4) + 1)
            /* Max index for MSM registers
               Normalize by CHIP_BASE and
               remove offset of 4, add 1 to account for
               address 0 */

#define MSM_F_BASE_START  FFE_FINGER_CTL1_WB    /* Start Address for Finger Related Register */

#define MSM_F_END         DEM_FRAME_OFF_WB      /* End Address for Finger Related Register */

#define MSM_FMAX_INX      (((MSM_F_END - MSM_F_BASE_START)/4) + 1)
            /* Max index for Finger MSM registers
               Normalize by Finger BASE Start and
               remove offset of 4, add 1 to account for
               address 0 */

#define DMOD_NUM_F        4                     /* Number of fingers (MSM23 Only) */

/* Enum indices used to index the Image locations for the 16 bit declarations
   For any new MSM memory space or ASB memory space write only WORD registers
   requiring a saved image, add the "IO" address name + post-fix "_INX"
   as used in the macro to the enum list below.  The MSM_OUTHM macro will
   during the pre-process stage obtain the correct index from the enum list.
   The last index is used to define the size of the required WORD array.
*/

#ifndef T_MSM5000
enum
{
   MSM_CLK_CTL1_WH_INX,     /* Index for Clock control 1 */
   MSM_CLK_CTL2_WH_INX,     /* Index for Clock control 2 */
   MSM_CLK_CTL3_WH_INX,     /* Index for Clock control 3 */
   MSM_CLK_CTL4_WH_INX,     /* Index for Clock control 4 */
   DMOD_INT_MASK_0_INX,     /* Interrupt Mask 0*/
   DMOD_INT_MASK_1_INX,     /* Interrupt Mask 1*/
   TEST_MEM_SEL_WH_INX,     /* Index for Decoder TEST Memory Select */
   FPC_CTL_WH_INX,          /* Index for Forward Power Control */
   MSM_MAX_H_INX            /* Do not add any index below this */
};
#endif

extern byte MSM_fimage[ DMOD_NUM_F ][ MSM_FMAX_INX ];
  /* Array used by software to buffer the write ports related to the fingers */


#ifdef T_MSM5000
   extern word MSM_image_H[ MSM_MAX_INX ];
   /* Array used by software to buffer the BYTE & WORD write ports of the MSM */
#else
   /* Both MSM_OUTM and MSM_OUTHM will use the MSM_image_H to cache shadow
   ** register values
   ** These need to be reviewed with other users of MSM_OUT* macros
   ** --SR
   */
   extern byte MSM_image[ MSM_MAX_INX ];
     /* Array used by software to buffer the BYTE write ports of the MSM */

   extern word MSM_image_H[ MSM_MAX_H_INX ];
     /* Array used by software to buffer the WORD (or ARM Half-Word)
     ** write ports of the MSM
     */
#endif

/*===========================================================================

                        MACRO DECLARATIONS

===========================================================================*/

/* ----------------------------------------------------------------------
** The inverse of MSM_IO: This calculates the index from the memory address
** ---------------------------------------------------------------------- */
#define MSM_INX( io ) (((io) - MSM_BASE)/4)

#define MSM_FINX( io ) (byte)( ((io) - MSM_F_BASE_START )/4)

/*---------------------------------------------------------------------------
                           MACRO MSM_OUT

This macro outputs a specified value to a specified port. All 8 bits of the
port are written (i.e., the port is not buffered by software).
---------------------------------------------------------------------------*/
#define MSM_OUT( io, val )  (void) outp( io, (int) val)


/*---------------------------------------------------------------------------
                           MACRO MSM_OUTM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. To do this,
an image is maintained of the previous value written to the port which
is then combined with the new value.
Uses TASKLOCK/TASKFREE.
---------------------------------------------------------------------------*/

#ifdef T_MSM5000

#define MSM_OUTM( io, mask, val) \
INTLOCK();\
MSM_image_H[MSM_INX (io)] = (MSM_image_H[ MSM_INX (io) ] & (word)(~(mask))) | \
          ((word)((val) & (mask)));\
(void) outp( io, (byte)MSM_image_H[ MSM_INX (io) ]);\
INTFREE();\

#else

#define MSM_OUTM( io, mask, val) \
  INTLOCK();\
  MSM_image[ MSM_INX (io) ] = (MSM_image[ MSM_INX (io) ] & (byte)(~(mask))) | \
            ((byte)((val) & (mask)));\
  (void) outp( io, MSM_image[ MSM_INX(io) ]);\
  INTFREE();\

#endif

/*---------------------------------------------------------------------------
                           MACRO MSM_OUTM_IL

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. To do this,
an image is maintained of the previous value written to the port which
is then combined with the new value.
Uses INTLOCK/INTFREE.
---------------------------------------------------------------------------*/
#ifdef T_MSM5000

#define MSM_OUTM_IL( io, mask, val) \
  INTLOCK(); \
  MSM_image_H[ MSM_INX (io) ] = (MSM_image_H[ MSM_INX (io) ] & (word)(~(mask))) | \
            ((word)((val) & (mask)));\
  (void) outp( io, (byte)MSM_image_H[ MSM_INX(io) ]);\
  INTFREE()
#else
#define MSM_OUTM_IL( io, mask, val) \
  INTLOCK(); \
  MSM_image[ MSM_INX (io) ] = (MSM_image[ MSM_INX (io) ] & (byte)(~(mask))) | \
            ((byte)((val) & (mask)));\
  (void) outp( io, MSM_image[ MSM_INX(io) ]);\
  INTFREE()

#endif


/*---------------------------------------------------------------------------
                           MACRO MSM_STROBEM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. The current
mask is not updated, so the next write will not be effected by the value
'val'.  To do this, an image is maintained of the previous value written
to the port which is then combined with the new value.  This macro uses
the image for registers which are not related to the fingers so IT MUST
ONLY BE USED FOR WRITING TO PORTS WHICH ARE NOT RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#ifdef T_MSM5000
#define  MSM_STROBEM( io, mask, val ) \
   INTLOCK( ); \
   (void) outp( io, (byte)(MSM_image_H[ MSM_INX(io) ] & (word)(~(mask))) \
     | (word)((val) & (mask))); \
   INTFREE( )
#else
#define  MSM_STROBEM( io, mask, val ) \
   INTLOCK( ); \
   (void) outp( io, (MSM_image[ MSM_INX(io) ] & (byte)(~(mask))) \
     | (byte)((val) & (mask))); \
   INTFREE( )
#endif

/*---------------------------------------------------------------------------
         MACRO MSM_STROBEM_H

This macro outputs a specified 16-bits value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. The current
mask is not updated, so the next write will not be effected by the value
'val'.  To do this, an image is maintained of the previous value written
to the port which is then combined with the new value.  This macro uses
the image for registers which are not related to the fingers so IT MUST
ONLY BE USED FOR WRITING TO PORTS WHICH ARE NOT RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/
#ifdef T_MSM5000
#define  MSM_STROBEM_H( io, mask, val ) \
   INTLOCK( ); \
   (void) outpw( io, (MSM_image_H[ MSM_INX(io) ] & (word)(~(mask))) \
     | (word)((val) & (mask))); \
   INTFREE( )
#endif
/* This macro is not used in MSM3 */

/*---------------------------------------------------------------------------

                           MACRO MSM_F_OUTM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. To do this,
an image is maintained of the previous value written to the port which
is then combined with the new value.  This macro uses the image for ports
which are related to the fingers and sets up the finger page port, so
IT MUST ONLY BE USED FOR WRITING TO PORTS WHICH ARE RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/
#define MSM_F_OUTM( finger, io, mask, val ) \
  INTLOCK( );\
  (void) outp( DMOD_F_PAGE, 1<<(finger) );\
  MSM_fimage[ finger ][ MSM_FINX(io) ] = \
         (MSM_fimage[ finger ][ MSM_FINX(io) ] & ((byte)(~(mask)))) \
         | (byte)( (val) & (mask) ); \
  (void) outp( io, MSM_fimage[ finger ][ MSM_FINX(io) ] ); \
  INTFREE( )

/*---------------------------------------------------------------------------
                           MACRO MSM_OUTH

This macro outputs a specified value to a specified port. All 16 bits of the
port are written (i.e., the port is not buffered by software).
---------------------------------------------------------------------------*/
#define MSM_OUTH( io, val )  (void) outpw( io, (word) val)


/*---------------------------------------------------------------------------
                           MACRO MSM_OUTHM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. To do this,
an image is maintained of the previous value written to the port which
is then combined with the new value. This is used to output to 16bit locations.
The image maintained is also 16bits.
---------------------------------------------------------------------------*/
#ifdef T_MSM5000
#define MSM_OUTHM( io, mask, val) \
INTLOCK( );\
MSM_image_H[MSM_INX(io)] = (MSM_image_H[ MSM_INX( io ) ] & (word)(~(mask))) | \
         ((word)((val) & (mask)));\
(void) outpw( io, ((word)MSM_image_H[ MSM_INX( io ) ]));\
INTFREE( )
#else
#define MSM_OUTHM( io, mask, val) \
  INTLOCK( );\
  MSM_image_H[io##_INX] = (MSM_image_H[io##_INX] & (word)(~(mask))) | \
            ((word)((val) & (mask)));\
  (void) outpw( io, ((word)MSM_image_H[io##_INX ]));\
  INTFREE( )
#endif

/*---------------------------------------------------------------------------
                           MACRO MSM_IN
This macro reads and returns the value from a specified port. NO mask is
applied to the read value.
---------------------------------------------------------------------------*/
#define MSM_IN( io )  (byte) inp( io )

/*---------------------------------------------------------------------------
                           MACRO MSM_INM
This macro reads and returns the value from a specified port. The mask is
applied to the read value. The bits that do not correspond to the mask are
returned as zero.
---------------------------------------------------------------------------*/
#define MSM_INM( io, mask ) ( inp( io ) & (mask) )


/*---------------------------------------------------------------------------
                           MACRO MSM_INH
This macro reads and returns the 16 bit value from a specified port. NO mask is
applied to the read value.
---------------------------------------------------------------------------*/
#define MSM_INH( io )  (word) inpw( io )

/*---------------------------------------------------------------------------
                           MACRO MSM_INHM
This macro reads and returns the value from a specified port. The mask is
applied to the read value. The bits that do not correspond to the mask are
returned as zero. This is used to read 16bit I/O ports
---------------------------------------------------------------------------*/
#define MSM_INHM( io, mask ) ( inpw( io ) & (mask) )


#endif /* ! _ARM_ASM_  */
#endif /* MSM_H */

