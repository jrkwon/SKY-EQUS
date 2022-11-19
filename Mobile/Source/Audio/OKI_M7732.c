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
00.07.28   jwpark  add Audio Test Code for A10
98/11/03   천형주  OKI_CODEC Updates 

===========================================================================*/


#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "bio.h"
#include "biog.h"
#include "OKI_M7732.h"
#include "snd.h"
#include "task.h"
#include "voc.h"
#include "sndi.h"
#include "AudioTest.h"
#if (MODEL_ID == MODEL_A10)
#include "VocCommon.h"
#include "uistate.h"
#endif



#ifdef OKI_CODEC
#if (MODEL_ID == MODEL_A10)
	#define CODEC_DEN_REG			ENC_GPIO_1
	#define CODEC_DEN_REG_TS_BUF	ENC_GPIO_1_TS_BUF
	#define CODEC_DEN_REG_TSEN		ENC_GPIO_1_TSEN
	#define CODEC_DEN_REG_BUF		ENC_GPIO_1_BUF
	#define CODEC_DEN_REG_OUT		ENC_GPIO_1_OUT

	#define CODEC_CLK_REG			ENC_GPIO_0
	#define CODEC_CLK_REG_TS_BUF	ENC_GPIO_0_TS_BUF
	#define CODEC_CLK_REG_TSEN		ENC_GPIO_0_TSEN
	#define CODEC_CLK_REG_BUF		ENC_GPIO_0_BUF
	#define CODEC_CLK_REG_OUT		ENC_GPIO_0_OUT

	#define CODEC_DATA_REG			ENC_GPIO_0
	#define CODEC_DATA_REG_TS_BUF	ENC_GPIO_0_TS_BUF
	#define CODEC_DATA_REG_TSEN		ENC_GPIO_0_TSEN
	#define CODEC_DATA_REG_BUF		ENC_GPIO_0_BUF
	#define CODEC_DATA_REG_OUT		ENC_GPIO_0_OUT
#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
	#define CODEC_DEN_REG			DEC_GPIO_0
	#define CODEC_DEN_REG_TS_BUF	DEC_GPIO_0_TS_BUF
	#define CODEC_DEN_REG_TSEN		DEC_GPIO_0_TSEN
	#define CODEC_DEN_REG_BUF		DEC_GPIO_0_BUF
	#define CODEC_DEN_REG_OUT		DEC_GPIO_0_OUT

	#define CODEC_CLK_REG			DEC_GPIO_0
	#define CODEC_CLK_REG_TS_BUF	DEC_GPIO_0_TS_BUF
	#define CODEC_CLK_REG_TSEN		DEC_GPIO_0_TSEN
	#define CODEC_CLK_REG_BUF		DEC_GPIO_0_BUF
	#define CODEC_CLK_REG_OUT		DEC_GPIO_0_OUT

	#define CODEC_DATA_REG			DEC_GPIO_0
	#define CODEC_DATA_REG_TS_BUF	DEC_GPIO_0_TS_BUF
	#define CODEC_DATA_REG_TSEN		DEC_GPIO_0_TSEN
	#define CODEC_DATA_REG_BUF		DEC_GPIO_0_BUF
	#define CODEC_DATA_REG_OUT		DEC_GPIO_0_OUT
#else
	#error MODEL_ID is not defined.
#endif



static snd_device_type	     Codec_cur_path     = SND_DEVICE_EARPIECE;
static snd_mute_control_type Codec_Rx_On        = SND_MUTE_MUTED;
static snd_mute_control_type Codec_Tx_On        = SND_MUTE_MUTED;
static byte                  Codec_tx_gain      = 0x00;
static byte                  Codec_rx_gain      = 0x00;
static byte                  Codec_dtmf_gain    = 0x00;


rex_timer_type               snd_codec_off_timer;
boolean						 snd_allow_codec_power_off = TRUE;
boolean						 Codec_voc_acquired        = FALSE;
boolean						 Codec_sleep_enable        = TRUE;
byte						 snd_cmd_image[8];


extern boolean               snd_tone_awake;
extern boolean               snd_voc_acquired;                      
extern boolean               snd_need_voc_acquire;
#if defined (FEATURE_YMU757B_MELODY)
extern boolean				 MUSIC_sleep_enable;
#elif defined (FEATURE_YMU759_MELODY)
extern boolean				 MUSIC_sleep_enable;
#endif
extern boolean               snd_IsVoicePlaying;


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
void sndhw_msm7732_out
(
  word audio_data      /* 12 bit의 audio codec용 직렬 데이타 */
)
{
  dword reg_mask;      /* MSB of the register to be latched */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	word isave;

	INTLOCK_SAV(isave);
  /* --------------------------------------------
  ** Bring DEN low
  ** -------------------------------------------- */
  BIO_OUT(CODEC_DEN_REG, BIO_CODEC_DEN_M, BIO_CODEC_DEN_LO_V);
  BIO_OUT(CODEC_CLK_REG, BIO_CODEC_CLK_M, BIO_CODEC_CLK_LO_V);

  for( reg_mask = MSM7732_MSB_MASK; reg_mask != 0; reg_mask >>= 1 ) {

    /* ----------------------------------------
    ** Set data hi/lo depending on masked value.
    ** ---------------------------------------- */

    if ( (audio_data & reg_mask) != 0 ) {
      BIO_OUT(CODEC_DATA_REG, BIO_CODEC_DATA_M, BIO_CODEC_DATA_HI_V);
    } else {
      BIO_OUT(CODEC_DATA_REG, BIO_CODEC_DATA_M, BIO_CODEC_DATA_LO_V);
    }

    BIO_OUT(CODEC_CLK_REG, BIO_CODEC_CLK_M, BIO_CODEC_CLK_HI_V);
    BIO_OUT(CODEC_CLK_REG, BIO_CODEC_CLK_M, BIO_CODEC_CLK_LO_V);
  } /* end of for */

  BIO_OUT(CODEC_DEN_REG, BIO_CODEC_DEN_M, BIO_CODEC_DEN_HI_V);
  BIO_OUT(CODEC_DATA_REG, BIO_CODEC_DATA_M, BIO_CODEC_DATA_LO_V);

  INTFREE_SAV(isave);
} /* end of sndhw_msm7732_out*/


/*===========================================================================

FUNCTION SNDHW_MSM7732_CMD

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void sndhw_msm7732_cmd
(
  byte snd_rw,	     	/* 1 bit의 control register R/W     */
  byte snd_addr,		/* 3 bit의 control register address */
  byte snd_mask,        /* */
  byte snd_cmd          /* 8 bit의 control command          */
)
{
  word snd_data = 0 ;        /* MSM7732에 전달되는 Control Data  */

  snd_cmd_image[snd_addr] = ( snd_cmd_image[snd_addr] & (byte)( ~(snd_mask) ) ) |
                            ((byte)( (snd_cmd) & (snd_mask) ));

  snd_data = ( (snd_rw << 11) | (snd_addr << 8) | snd_cmd_image[snd_addr] );

  sndhw_msm7732_out(snd_data);
	
} /* end of sndhw_msm7732_cmd */


/*===========================================================================

FUNCTION MSM7732_TX_GAIN

DESCRIPTION
  MSM7732 CODEC의 Tx VOLUME을 조정

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void msm7732_tx_gain( byte volumes)
{
	Codec_tx_gain = volumes;
	sndhw_msm7732_cmd( MSM7732_REG_WRITE,
                      CR2_PCM_CTL,	TX_GAIN_M , volumes);
}

/*===========================================================================

FUNCTION MSM7732_RX_GAIN

DESCRIPTION
  MSM7732 CODEC의 Rx VOLUME을 조정

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void msm7732_rx_gain( byte volumes)
{
	Codec_rx_gain = volumes;
	sndhw_msm7732_cmd( MSM7732_REG_WRITE,
                      CR2_PCM_CTL,	RX_GAIN_M , volumes);
}

/*===========================================================================

FUNCTION MSM7732_DTMF_TONE_GAIN

DESCRIPTION
  MSM7732 CODEC의 DTMF의 VOLUME조정

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void msm7732_dtmf_tone_gain( byte gain)
{
	Codec_dtmf_gain = gain;

	sndhw_msm7732_cmd( MSM7732_REG_WRITE,
					  CR3_TONE_GAIN,
                       RX_TONE_GAIN_M,
                       gain);
}

/*===========================================================================

FUNCTION MSM7732_SET_TX_GAIN

DESCRIPTION
  MSM7732 CODEC의 Tx VOLUME을 조정

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#if ( MODEL_ID == MODEL_B1 )  || (MODEL_ID == MODEL_B2)
void msm7732_set_tx_gain(boolean manner)
{
	// youandi 00.07.28
	word volume=TX_GAIN_0;
    if(IsAudioGainTest == TRUE) volume = AudioTestGain.CODECTx;
	// youandi 00.07.28 end

	if(manner)
	{
		msm7732_tx_gain(TX_GAIN_4);
	}
	else
	{
		if(Codec_cur_path == SND_DEVICE_EARPIECE)
			msm7732_tx_gain(volume);
		else
			msm7732_tx_gain(TX_GAIN_6);	
	}
}
#else
void msm7732_set_tx_gain(boolean manner)
{
	if(manner)
	{
		msm7732_tx_gain(TX_GAIN_6);
	}
	else
	{
		if(Codec_cur_path == SND_DEVICE_EARPIECE)
			msm7732_tx_gain(TX_GAIN_4);
		else
			msm7732_tx_gain(TX_GAIN_6);	
	}
}
#endif
/*===========================================================================

FUNCTION MSM7732_SET_RX_GAIN

DESCRIPTION
  MSM7732 CODEC의 Rx VOLUME을 조정

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if ( MODEL_ID == MODEL_B1 )  || (MODEL_ID == MODEL_B2)
void msm7732_set_rx_gain(void)
{
	// youandi 00.07.28
	word volume=RX_GAIN_7;
	extern boolean snd_IsVoicePlaying;
    if(IsAudioGainTest == TRUE) volume = AudioTestGain.CODECRx;
	if(snd_IsVoicePlaying) volume = RX_GAIN_2;
	// youandi 00.07.28 end

	if(Codec_cur_path == SND_DEVICE_EARPIECE)
		msm7732_rx_gain(volume);
	else
	{
		if(snd_IsVoicePlaying)
			msm7732_rx_gain(volume);
		else
			msm7732_rx_gain(RX_GAIN_6);
	}
}
#else
void msm7732_set_rx_gain(void)
{
	if(Codec_cur_path == SND_DEVICE_EARPIECE)
		msm7732_rx_gain(RX_GAIN_3);
	else
		msm7732_rx_gain(RX_GAIN_2);	
}
#endif
/*===========================================================================

FUNCTION MSM7732_SET_DTMF_GAIN

DESCRIPTION
  MSM7732 CODEC의 DTMF의 VOLUME조정

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void msm7732_set_dtmf_gain(int Idx)
{
	static byte dtmf_gain_tbl[6][2] = {
	//  {EARPIECE, EARPHONE}
		{0x0     , 0x0     },
		{0x4     , 0x1     },
		{0x6     , 0x2     },
		{0x8     , 0x3     },
		{0xa     , 0x4     },
		{0xc     , 0x5     }
	};
	msm7732_dtmf_tone_gain(dtmf_gain_tbl[Idx][Codec_cur_path]);
}

/*===========================================================================

FUNCTION SND_MSM7732_INIT

DESCRIPTION
  MSM7732 초기화
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void snd_msm7732_init(void)
{
	BIO_TRISTATE(CODEC_CLK_REG,  BIO_CODEC_CLK_M,  BIO_OUTPUT_ENA_V);
	BIO_TRISTATE(CODEC_DATA_REG, BIO_CODEC_DATA_M, BIO_OUTPUT_ENA_V);
	BIO_TRISTATE(CODEC_DEN_REG,  BIO_CODEC_DEN_M,  BIO_OUTPUT_ENA_V);

	Codec_cur_path			  = SND_DEVICE_EARPIECE;
	Codec_Rx_On				  = SND_MUTE_MUTED;
	Codec_Tx_On				  = SND_MUTE_MUTED;		
	Codec_tx_gain			  = 0x00;
	Codec_rx_gain			  = 0x00;
	Codec_dtmf_gain			  = 0x00;
	snd_allow_codec_power_off = TRUE;
	Codec_voc_acquired        = FALSE;
	Codec_sleep_enable        = TRUE;
	rex_def_timer(&snd_codec_off_timer, &snd_tcb, SND_CODEC_OFF_TIMER_SIG);

	/* PATH Enable
	*/
	sndhw_msm7732_cmd(MSM7732_REG_WRITE,
					  CR0_BOM0,
					  PCM_LAW_M | AMP_PON_M | PDN_ALL_M | POW_DN_RX_M
					  | POW_DN_TX_M | SLOP_FIL_M | SLOP_FILT_M | PCM_CODE_M,
					  PCM_MU_LAW | AMP_PON_DIS | PDN_ALL_EN |POW_DN_RX_DN
					  | POW_DN_TX_DN | SLOP_CASE_1|PCM_14_PCM | SLOP_FILT_DIS );


	sndhw_msm7732_cmd(MSM7732_REG_WRITE,
		              CR1_BOM1,
					  SHORT_FRAME_M | ASW1_ABC_M| RX_PAD_M | ASW2_DE_M,
					  SHORT_FRAME_EN | ASW1_ABC_C | RX_PAD_DIS | ASW2_DE_DIS);

	/* TX/RX PCM Codec
	*/
	sndhw_msm7732_cmd(MSM7732_REG_WRITE,
					  CR2_PCM_CTL,
					  TX_CODEC_M  | RX_CODEC_M,
					  TX_CODEC_EN | RX_CODEC_EN);
	msm7732_tx_gain(0);
	msm7732_rx_gain(0);

   sndhw_msm7732_cmd(MSM7732_REG_WRITE,
                     CR3_TONE_GAIN,
                     TONE_ON_OFF_M | SIDE_TONE_GAIN_M,
                     TONE_OFF | 0x00);


   sndhw_msm7732_cmd(MSM7732_REG_WRITE,
                     CR4_TONE_FREQ,
					 SELECT_TONE_M,
					 SELECT_VOICE_V);


   /* VOX Enable
   */
   sndhw_msm7732_cmd(MSM7732_REG_WRITE,
                     CR6_VOX_CTL,
					 VOX_CTL_M,
					 VOX_DIS);

}


/*===========================================================================

FUNCTION MSM7732_TRX_CONTROL

DESCRIPTION
  MSM7732 CODEC의 TX/RX를 Control함

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void msm7732_trx_control(snd_mute_control_type p_tx,
						 snd_mute_control_type p_rx,
						 snd_device_type       p_path)
{
  byte power= 0;
  byte path = 0;
  byte tone = 0;


  if(p_path != SND_DEVICE_EARPIECE && p_path != SND_DEVICE_EARPHONE)
	{
	  p_tx   = SND_MUTE_MUTED;
	  p_rx   = SND_MUTE_MUTED;
	  p_path = SND_DEVICE_EARPHONE;
	}

  
  Codec_Rx_On    = p_rx;
  Codec_Tx_On    = p_tx;
  Codec_cur_path = p_path;


  // snd_allow_codec_power_off
  if(p_tx != SND_MUTE_MUTED ||
     p_rx != SND_MUTE_MUTED  )
	{
	  rex_clr_timer(&snd_codec_off_timer);
	  rex_clr_sigs(&snd_tcb, SND_CODEC_OFF_TIMER_SIG);
	  snd_allow_codec_power_off = FALSE;
	  Codec_sleep_enable        = FALSE;
	  SND_SLEEP_FORBID();
	  if(!Codec_voc_acquired)
	  {
		  voc_acquire(VOC_ACQ_CODEC, NULL);
		  Codec_voc_acquired = TRUE;
	  }
	}


  // real power off control
  power = (((snd_allow_codec_power_off) &&
	        (p_tx == SND_MUTE_MUTED   ) &&
		    (p_rx == SND_MUTE_MUTED   )  ) ? PDN_ALL_EN   : PDN_ALL_DIS ) |
		  ((p_rx  == SND_MUTE_MUTED      ) ? AMP_PON_DIS  : AMP_PON_EN  ) |
	      ((p_tx  == SND_MUTE_MUTED      ) ? POW_DN_TX_DN : POW_DN_TX_ON) |
		  ((p_rx  == SND_MUTE_MUTED      ) ? POW_DN_RX_DN : POW_DN_RX_ON) ;

  path  = ((p_tx   == SND_MUTE_UNMUTED  &&
		    p_path == SND_DEVICE_EARPIECE) ? ASW1_ABC_A   : ASW1_ABC_C  );

  tone  = ((p_rx   == SND_MUTE_UNMUTED  &&
			p_path == SND_DEVICE_EARPIECE) ? TONE_OUT_VFRO: TONE_OUT_SAO);

  
  // write to registers
  sndhw_msm7732_cmd(MSM7732_REG_WRITE,
		            CR0_BOM0,
					AMP_PON_M | PDN_ALL_M | POW_DN_RX_M | POW_DN_TX_M,
					power);

  sndhw_msm7732_cmd(MSM7732_REG_WRITE,
	                CR1_BOM1,
					ASW1_ABC_M,
					path);
  
  if(p_rx == SND_MUTE_UNMUTED) msm7732_rx_gain(Codec_rx_gain);
  if(p_tx == SND_MUTE_UNMUTED) msm7732_tx_gain(Codec_tx_gain);

  sndhw_msm7732_cmd(MSM7732_REG_WRITE,
					CR4_TONE_FREQ,
					TONE_OUT_M,
					tone);

  
  // codec off timer control
  if(!snd_allow_codec_power_off                        &&
	 p_tx == SND_MUTE_MUTED                            &&
	 p_rx == SND_MUTE_MUTED                            &&
	 rex_get_timer(&snd_codec_off_timer) == 0          &&
	 !(rex_get_sigs(&snd_tcb) & SND_CODEC_OFF_TIMER_SIG))
	{
	  rex_set_timer(&snd_codec_off_timer, 5000);
	}

  if(Codec_voc_acquired        && 
	 snd_allow_codec_power_off &&
	 p_tx == SND_MUTE_MUTED    &&
	 p_rx == SND_MUTE_MUTED     )
  {
	  voc_release(VOC_ACQ_CODEC);
	  Codec_voc_acquired = FALSE;
	  rex_set_timer(&snd_codec_off_timer, 1000);
  }
}

/*===========================================================================

FUNCTION MSM7732_DTMF_GEN

DESCRIPTION
  MSM7732 CODEC의 DTMF를 발생 시킴
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void msm7732_dtmf_gen(byte key)
{

	byte power = 0;
	byte path  = 0;
	byte gain  = 0;
	byte tone  = 0;

	// CODEC이 한번 켜지면 최소한 10초는 켜져 있어야 한다.
	rex_clr_timer(&snd_codec_off_timer);
	rex_clr_sigs(&snd_tcb, SND_CODEC_OFF_TIMER_SIG);
	snd_allow_codec_power_off = FALSE;
	Codec_sleep_enable        = FALSE;
	SND_SLEEP_FORBID();
	if(!Codec_voc_acquired)
	{
	  voc_acquire(VOC_ACQ_CODEC, NULL);
	  Codec_voc_acquired = TRUE;
	}

	
	////키톤이 통화중에 나는 경우에 outpath의 gain을 두배로 키워주므로 음성도 2배로 커진다.
	/// 이때 노이즈도 2배로 커지는 효과가 있으므로 이를 없애기 위해 통화중에는 키톤을 2배로
	////키우지 않는다.
	if(Codec_Rx_On        == SND_MUTE_MUTED)  path |= ASW2_DE_EN;
	else                                      path |= ASW2_DE_DIS;
	if(Codec_cur_path == SND_DEVICE_EARPIECE) tone |= TONE_OUT_VFRO;
	else                                      tone |= TONE_OUT_SAO;

	power |= AMP_PON_EN | PDN_ALL_DIS | POW_DN_RX_ON;
	gain  |= TONE_ON;
	tone  |= key;

	sndhw_msm7732_cmd(MSM7732_REG_WRITE,
					  CR0_BOM0,
					  AMP_PON_M | PDN_ALL_M | POW_DN_RX_M,
					  power);
	sndhw_msm7732_cmd(MSM7732_REG_WRITE,
	 				  CR1_BOM1,
					  ASW2_DE_M,
					  path);
	sndhw_msm7732_cmd(MSM7732_REG_WRITE,
					  CR4_TONE_FREQ,
					  TONE_FREQ_M | TONE_OUT_M| TONE_SEL_M,
					  tone);
	msm7732_dtmf_tone_gain(Codec_dtmf_gain);
    sndhw_msm7732_cmd(MSM7732_REG_WRITE,
					  CR3_TONE_GAIN,
					  TONE_ON_OFF_M,
					  gain);
}


/*===========================================================================

FUNCTION MSM7732_DTMF_STOP

DESCRIPTION
  여기서는 CODEC에서 나는 DTMF를 멈추는 역활만 한다.
  즉, D/E를 끊어 일반 통화 모드로 바꾸고, CODEC Tone을 멈춘다.
  path 절체는 snd_tone_timer를 통해 ...

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void	msm7732_dtmf_stop(void)
{
	sndhw_msm7732_cmd(MSM7732_REG_WRITE, CR1_BOM1, ASW2_DE_M, ASW2_DE_DIS);
	sndhw_msm7732_cmd(MSM7732_REG_WRITE, CR3_TONE_GAIN, TONE_ON_OFF_M, TONE_OFF);
}

#endif //OKI_CODEC 
