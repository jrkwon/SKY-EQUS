#include "target.h"

#ifdef MMI_TREX
#include <msm7732.h>
#else // #ifdef MMI_TREX

#ifndef	MSM7732_H
#define MSM7732_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                OKI MSM7732 Command Declarations

GENERAL DESCRIPTION
  OKI MSM7732와 관련된 Command를 정의함

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 1998 by SK Teletech, Ltd.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
98/11/03   천형주  OKI_CODEC Updates 

===========================================================================*/

/* <EJECT> */
#ifdef OKI_CODEC 
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "dmod.h"
#include "snd.h"
/* <EJECT> */
/*===========================================================================

               DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
/*
** sound command를 위한 image buffer
*/
//byte snd_cmd_image[9];

/*----------------------------------------------------------------------
  CR0 Basic Operation Mode 1 Register : R/W
------------------------------------------------------------------------*/
#define CR0_BOM0          0x00

#define PCM_CODE_M        0x01
#define PCM_14_PCM        0x01
#define PCM_08_PCM		  0x00

#define SLOP_FILT_M       0x02
#define SLOP_CASE_2       0x02
#define SLOP_CASE_1       0x00

#define SLOP_FIL_M        0x04
#define SLOP_FILT_EN	  0x04
#define SLOP_FILT_DIS     0x00

#define POW_DN_RX_M       0x08
#define POW_DN_RX_DN      0x08
#define POW_DN_RX_ON      0x00

#define POW_DN_TX_M       0x10
#define POW_DN_TX_DN      0x10
#define POW_DN_TX_ON      0x00

#define PDN_ALL_M         0x20
#define PDN_ALL_EN        0x20
#define PDN_ALL_DIS       0x00

#define AMP_PON_M         0x40
#define AMP_PON_EN        0x40
#define AMP_PON_DIS       0x00

#define PCM_LAW_M         0x80
#define PCM_A_LAW		  0x80
#define PCM_MU_LAW		  0x00

/*----------------------------------------------------------------------
  CR1 Basic Operation Mode 2 Register : R/W
------------------------------------------------------------------------*/
#define CR1_BOM1          0x01

#define RX_PAD_M          0x01
#define RX_PAD_EN         0x01
#define RX_PAD_DIS        0x00

#define ASW1_ABC_M        0x02
#define ASW1_ABC_C        0x02
#define ASW1_ABC_A        0x00

#define ASW2_DE_M         0x04
#define ASW2_DE_EN        0x04
#define ASW2_DE_DIS       0x00

#define SHORT_FRAME_M     0x08
#define SHORT_FRAME_EN    0x08
#define SHORT_FRAME_DIS   0x00

/*----------------------------------------------------------------------
  CR2 PCM CODEC Control Register : R/W
------------------------------------------------------------------------*/
#define CR2_PCM_CTL       0x02

#define RX_GAIN_M         0x07
#define RX_GAIN_0         0x00         /* -12 dB */
#define RX_GAIN_1         0x01         /*  -9 dB */
#define RX_GAIN_2         0x02         /*  -6 dB */
#define RX_GAIN_3         0x03         /*  -3 dB */
#define RX_GAIN_4         0x04         /*   0 dB */
#define RX_GAIN_5         0x05         /*  +3 dB */
#define RX_GAIN_6         0x06         /*  +6 dB */
#define RX_GAIN_7         0x07         /*  +9 dB */

#define RX_CODEC_M        0x08
#define RX_CODEC_EN       0x00
#define RX_CODEC_DIS      0x08

#define TX_GAIN_M        0x70
#define TX_GAIN_0         0x00         /*  -6 dB */
#define TX_GAIN_1         0x10         /*  -4 dB */
#define TX_GAIN_2         0x20         /*  -2 dB */
#define TX_GAIN_3         0x30         /*   0 dB */
#define TX_GAIN_4         0x40         /*  +2 dB */
#define TX_GAIN_5         0x50         /*  +4 dB */
#define TX_GAIN_6         0x60         /*  +6 dB */
#define TX_GAIN_7         0x70         /*  +8 dB */

#define TX_CODEC_M        0x80
#define TX_CODEC_EN       0x00
#define TX_CODEC_DIS      0x80

/*----------------------------------------------------------------------
  CR3 Side Tone Gain Select Register : R/W
------------------------------------------------------------------------*/
#define CR3_TONE_GAIN     0x03

#define RX_TONE_GAIN_M    0x0f

#define TONE_ON_OFF_M     0x10
#define TONE_ON           0x10
#define TONE_OFF          0x00

#define SIDE_TONE_GAIN_M  0xe0

/*----------------------------------------------------------------------
  CR4 Tone Frequency Control Register : R/W
------------------------------------------------------------------------*/
#define CR4_TONE_FREQ     0x04

#define TONE_FREQ_M       0x1f

/*
#define DTMF_1_V		  0x00
#define DTMF_2_V		  0x01
#define DTMF_3_V		  0x02
#define DTMF_4_V		  0x03
#define DTMF_5_V		  0x04
#define DTMF_6_V		  0x05
#define DTMF_7_V		  0x06
#define DTMF_8_V		  0x07
#define DTMF_9_V		  0x0a
#define DTMF_*_V		  0x0c
#define DTMF_0_V		  0x0d
#define DTMF_#_V		  0x0e
*/
#define TOS_M

#define SELECT_TONE_M     0x40
#define SELECT_VOICE_V    0x00
#define SELECT_TONE_V     0x40

#define TONE_OUT_M        0x20
#define TONE_OUT_SAO      0x20
#define TONE_OUT_VFRO     0x00

#define TONE_SEL_M        0x80
#define DTMF_SEL          0x80
#define OTHER_TONE_SEL    0x00


/*----------------------------------------------------------------------
  CR5 
------------------------------------------------------------------------*/
#define CR5_NOT_USED      0x05

/*----------------------------------------------------------------------
  CR6  VOX Function Control Register : R/W
------------------------------------------------------------------------*/
#define CR6_VOX_CTL       0x06

#define VOICE_DET_LEVEL_M 0x60
#define VOICE_20          0x00
#define VOICE_26		  0x20
#define VOICE_32		  0x40
#define VOICE_38		  0x60

#define VOX_CTL_M         0x80
#define VOX_EN            0x80
#define VOX_DIS           0x00

/*----------------------------------------------------------------------
  CR7  Detect Register : R
------------------------------------------------------------------------*/
#define CR7_DET_CTL       0x07

#define NOISE_DET_LEVEL_M 0x60
#define NOISE_30          0x60
#define NOISE_30_40       0x40
#define NOISE_40_50       0x20
#define NOISE_50          0x00

#define VOICE_CTL_M       0x80
#define VOICE_DET_EN      0x80
#define VOICE_DET_DIS     0x00

/*----------------------------------------------------------------------
  AUDIO Processor General Define
------------------------------------------------------------------------*/
#define MSM7732_TOTAL_BITS     12
#define MSM7732_MSB_MASK       (((dword) 0x1) << (MSM7732_TOTAL_BITS - 1))

#define MSM7732_REG_READ       0x01
#define MSM7732_REG_WRITE      0x00

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Master Clock M/N counter */

//#define DMOD_M_CLK_MDIV_0       DMOD_IO( 0xEE )
//#define DMOD_M_CLK_MDIV_1       DMOD_IO( 0x1D4 ) /* MSM23 Only   */

//#define DMOD_M_CLK_NDIV_0       DMOD_IO( 0xF6 )
//#define DMOD_M_CLK_NDIV_1       DMOD_IO( 0x1D6 ) /* MSM23 Only   */

//#define DMOD_M_CLK_DUTY_0       DMOD_IO( 0xFE )
//#define DMOD_M_CLK_DUTY_1       DMOD_IO( 0x1D8 ) /* MSM23 Only   */


/*===========================================================================

FUNCTION SNDHW_MSM7732_OUT

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void sndhw_msm7732_out
(
  word audio_data      /* 12 bit의 audio codec용 직렬 데이타 */
);


/*===========================================================================

FUNCTION SNDHW_MSM7732_CMD

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void sndhw_msm7732_cmd
(
  byte snd_rw,	     	/* 1 bit의 control register R/W     */
  byte snd_addr,		/* 3 bit의 control register address */
  byte snd_mask,        /* */
  byte snd_cmd          /* 8 bit의 control command          */
);



/*===========================================================================

FUNCTION SND_MSM7732_INIT

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void snd_msm7732_init( void );
extern void msm7732_tx_gain( byte volumes);
extern void msm7732_rx_gain( byte volumes);
extern void msm7732_dtmf_tone_gain( byte gain);
extern void msm7732_set_tx_gain( boolean manner);
extern void msm7732_set_rx_gain( void);
extern void msm7732_set_dtmf_gain( int Idx);
extern void snd_msm7732_init( void );
extern void msm7732_trx_control(snd_mute_control_type Tx, snd_mute_control_type Rx, snd_device_type Cur_Path);
extern void msm7732_dtmf_gen( byte key);
extern void	msm7732_dtmf_stop(void);

#endif /* OKI_CODEC */
#endif 

#endif // #ifdef MMI_TREX