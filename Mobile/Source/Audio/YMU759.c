/*===========================================================================

                   Y A M A H A   M E L O D Y  D R I V E R

DESCRIPTION
  This file contains functions of the YAMAHA FM synthesizer for the melody
  sound. 

  Copyright (c) 1999 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.08.29    youandi YMU759
00.08.02    tasik   create
===========================================================================*/

/*
	ÇöÀç´Â ¾Æ·¡ÀÇ ¸ðµç ÇÔ¼ö¸¦ ºñ¿öµÎ¾ú´Ù. ¾ÕÀ¸·Î´Â YMU759 Ä¨À» ÅëÇØ¼­ 
	À½¾ÇÀ» ±¸ÇöÇÒ °ÍÀÌ¸é ±× Ä¨À» Áö¿øÇÏ´Â ÇÁ·Î±×·¥À» Ã¤¿ö ³Ö¾î¾ß ÇÑ´Ù.

    ´çÀåÀº A10°ú °øÀ¯µÇ¾î ÀÖÀ¸¹Ç·Î ÄÄÆÄÀÏ ¿¡·¯¸¦ ÇÇÇÏ±â À§ÇØ¼­ ÇÔ¼öÀÌ¸§¸¸À» 
	³²°å´Ù. 

*/

/////////////////////////////////////////////////////////////////////////////////
#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "err.h"
#include "bio.h"
#include "clk.h"
#include "tramp.h"
#include "task.h"
#include "snd.h"
#include "sndi.h"
#include "sndihw.h"
#include "ska.h"

#ifdef FEATURE_YMU759_MELODY // youandi 

#include "hsig.h"			// Backlight test
#include "YMU759.h"		    // YMU757B FM MUSIC synthesizer definition
#include "MusicDB_B1.h"		// Music data base definition
#include "OKI_M7732.h"


extern boolean snd_tone_awake;         // TRUE If Generating a Tone
extern boolean snd_voc_acquired;       // TRUE If Vocoder Acquired
extern boolean snd_need_voc_acquire;   // TRUE If WANT to Vocoder Acquire
extern boolean Codec_sleep_enable;     // TRUE if CODEC allow sleeping
       boolean MUSIC_sleep_enable;     // TRUE If YMU757B really off
extern boolean snd_IsMusicPlaying;
       boolean snd_IsPowerOn;
extern byte    fMotorOn;
extern byte    fLEDOn;
extern BOOL g_fGVMPaused;

static rex_timer_type MUSIC_music_timer;
static rex_timer_type MUSIC_vRef_PDdelay_timer;
UINT8 *pFileDataArea;
#define  MM_OK 0


static	UINT8 *hSmafData;	
static	UINT32 SmafSize1;	
static	int Status;		
static	void *pSmaf;	
static	int seqHd;		

// Ma2lib.c 
static const UINT8 idx2op[] = {
		0x10,0x14,0x11,0x15,0x12,0x16,0x13,0x17,
		0x18,0x1c,0x19,0x1d,0x1a,0x1e,0x1b,0x1f,
		0x20,0x24,0x21,0x25,0x22,0x26,0x23,0x27,
		0x28,0x2c,0x29,0x2d,0x2a,0x2e,0x2b,0x2f
		};

static const UINT8 idx4op[] = {
		0x10,0x14,0x18,0x1c,0x11,0x15,0x19,0x1d,
		0x12,0x16,0x1a,0x1e,0x13,0x17,0x1b,0x1f,
		0x20,0x24,0x28,0x2c,0x21,0x25,0x29,0x2d,
		0x22,0x26,0x2a,0x2e,0x23,0x27,0x2b,0x2f
		};

static  UINT8  regsw;
static  UINT8  regidx;
static  UINT8  save_regsw;
static  UINT8  save_regidx;
//static  UINT8  timercnt;
static  UINT8  _syncch;

static struct MA2_IRQ_SERVICE Ma2IrqHandler[MA2_IRQ_HANDLE_MAX];
static UINT8  bMa2IrqHandler;   

static UINT8  fm_IrqCtrl, adpcm_IrqCtrl;
static UINT16 fifoirq;
static UINT8  wavefifoirq;
static UINT8  Ma2IrqEnable;

// Ma2lib.c
// MA2Manager.c
static struct MA2_RESOURCE_INFO  Ma2_resource_info;
// MA2Manager.c
// Ma2seq.c
static UINT8 mapFifo, usedFifo;	
static PCMSEQINF infPcm;		
static enum STATE_PCM statePcm;	
static enum STATE_FM stateFm;	
static FMSEQINF infFm;			
static UINT8 fmIrqCtrl;			
static UINT8 pcmIrqCtrl;		
static UINT8 ma2Ctrl;			
static UINT8 ma2FifoCtrl;		

static SINT8 ma2IrqHdl;	

int (*PcmIntProc)(PCMCBEVTYPE, int, DATABLK *, void *);
int (*FmIntProc)(FMCBEVTYPE, int, DATABLK *, void *);

void Seq_Irq_Proc(void);
static void PutDuration(UINT8 fifoNo, UINT32 t);
static void PutPcmDuration(long t);
UINT32 WriteToFmFifo(UINT8 fifoNo, UINT32 fifoSpace, PFIFOINF hFifoInf);
UINT32 WriteToPcmFifo(UINT32 fifoSpace, PFIFOINF hFifoInf);
UINT32 WriteToWaveFifo(UINT32 fifoSpace, PFIFOINF hFifoInf);

// Ma2seq.c
//smafchecker.c

static SINT32	dwOverallChunkSize;
static SINT32	dwContentsInfoChunkSize;
static SINT32	dwScoreTrackChunkSize;
static SINT32	dwSeekPhraseInfoChunkSize;
static SINT32	dwSetupDataChunkSize;
static SINT32	dwSequenceDataChunkSize; 
static SINT32	dwAudioTrackChunkSize;
static SINT32	dwGraphicTrackChunkSize;
static SINT32	dwMasterTrackChunkSize;
static SINT32	dwWaveDataChunkSize;

static UINT32	dwAbsTime;
static UINT16   wGatetime;

static UINT8 *pSmafBuf;

struct OVERALL_CHUNK SmafInfo;
struct SCORE_TRACK_CHUNK ScoreTrackInfo[MAX_SCORE_TRACK_CHUNK];
struct PCM_AUDIO_TRACK_CHUNK AudioTrackInfo[MAX_AUDIO_TRACK_CHUNK];

static UINT8  bScoreTrack;
static UINT8  bAudioTrack;
static UINT8  bWaveData;
static UINT8  bGraphicTrack;
static UINT8  bMasterTrack;

static UINT8 bVoiceMsg1;
static UINT8 bVoiceMsg2;
static UINT8 bMa1ScoreTrack;
static UINT8 bMa2ScoreTrack;

struct CONTROL_EVENT {
	UINT8 ProgramChange;
	UINT8 BankSelect;
	UINT8 OctaveShift;
	UINT8 Modulation;
	UINT8 PitchBend;
	UINT8 Volume;
	UINT8 Panpot;
	UINT8 Expression;
};

const UINT8 expression[] = {0x00,0x00,0x10,0x20,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,0x7f};
const UINT8 pitchbend[] = {0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70};

static unsigned short crctable[256] = {
  0x0000U,0x1021U,0x2042U,0x3063U,0x4084U,0x50A5U,0x60C6U,0x70E7U,
  0x8108U,0x9129U,0xA14AU,0xB16BU,0xC18CU,0xD1ADU,0xE1CEU,0xF1EFU,
  0x1231U,0x0210U,0x3273U,0x2252U,0x52B5U,0x4294U,0x72F7U,0x62D6U,
  0x9339U,0x8318U,0xB37BU,0xA35AU,0xD3BDU,0xC39CU,0xF3FFU,0xE3DEU,
  0x2462U,0x3443U,0x0420U,0x1401U,0x64E6U,0x74C7U,0x44A4U,0x5485U,
  0xA56AU,0xB54BU,0x8528U,0x9509U,0xE5EEU,0xF5CFU,0xC5ACU,0xD58DU,
  0x3653U,0x2672U,0x1611U,0x0630U,0x76D7U,0x66F6U,0x5695U,0x46B4U,
  0xB75BU,0xA77AU,0x9719U,0x8738U,0xF7DFU,0xE7FEU,0xD79DU,0xC7BCU,
  0x48C4U,0x58E5U,0x6886U,0x78A7U,0x0840U,0x1861U,0x2802U,0x3823U,
  0xC9CCU,0xD9EDU,0xE98EU,0xF9AFU,0x8948U,0x9969U,0xA90AU,0xB92BU,
  0x5AF5U,0x4AD4U,0x7AB7U,0x6A96U,0x1A71U,0x0A50U,0x3A33U,0x2A12U,
  0xDBFDU,0xCBDCU,0xFBBFU,0xEB9EU,0x9B79U,0x8B58U,0xBB3BU,0xAB1AU,
  0x6CA6U,0x7C87U,0x4CE4U,0x5CC5U,0x2C22U,0x3C03U,0x0C60U,0x1C41U,
  0xEDAEU,0xFD8FU,0xCDECU,0xDDCDU,0xAD2AU,0xBD0BU,0x8D68U,0x9D49U,
  0x7E97U,0x6EB6U,0x5ED5U,0x4EF4U,0x3E13U,0x2E32U,0x1E51U,0x0E70U,
  0xFF9FU,0xEFBEU,0xDFDDU,0xCFFCU,0xBF1BU,0xAF3AU,0x9F59U,0x8F78U,
  0x9188U,0x81A9U,0xB1CAU,0xA1EBU,0xD10CU,0xC12DU,0xF14EU,0xE16FU,
  0x1080U,0x00A1U,0x30C2U,0x20E3U,0x5004U,0x4025U,0x7046U,0x6067U,
  0x83B9U,0x9398U,0xA3FBU,0xB3DAU,0xC33DU,0xD31CU,0xE37FU,0xF35EU,
  0x02B1U,0x1290U,0x22F3U,0x32D2U,0x4235U,0x5214U,0x6277U,0x7256U,
  0xB5EAU,0xA5CBU,0x95A8U,0x8589U,0xF56EU,0xE54FU,0xD52CU,0xC50DU,
  0x34E2U,0x24C3U,0x14A0U,0x0481U,0x7466U,0x6447U,0x5424U,0x4405U,
  0xA7DBU,0xB7FAU,0x8799U,0x97B8U,0xE75FU,0xF77EU,0xC71DU,0xD73CU,
  0x26D3U,0x36F2U,0x0691U,0x16B0U,0x6657U,0x7676U,0x4615U,0x5634U,
  0xD94CU,0xC96DU,0xF90EU,0xE92FU,0x99C8U,0x89E9U,0xB98AU,0xA9ABU,
  0x5844U,0x4865U,0x7806U,0x6827U,0x18C0U,0x08E1U,0x3882U,0x28A3U,
  0xCB7DU,0xDB5CU,0xEB3FU,0xFB1EU,0x8BF9U,0x9BD8U,0xABBBU,0xBB9AU,
  0x4A75U,0x5A54U,0x6A37U,0x7A16U,0x0AF1U,0x1AD0U,0x2AB3U,0x3A92U,
  0xFD2EU,0xED0FU,0xDD6CU,0xCD4DU,0xBDAAU,0xAD8BU,0x9DE8U,0x8DC9U,
  0x7C26U,0x6C07U,0x5C64U,0x4C45U,0x3CA2U,0x2C83U,0x1CE0U,0x0CC1U,
  0xEF1FU,0xFF3EU,0xCF5DU,0xDF7CU,0xAF9BU,0xBFBAU,0x8FD9U,0x9FF8U,
  0x6E17U,0x7E36U,0x4E55U,0x5E74U,0x2E93U,0x3EB2U,0x0ED1U,0x1EF0U
};

//smafchecker.c
//smafseq.c
static SMAF	Smaf[MAX_SMAF_DATA_NUM];
static SMAFSEQINFO	SeqInf[MAX_SMAF_DATA_NUM];

struct OVERALL_CHUNK SmafInfo;
struct SCORE_TRACK_CHUNK ScoreTrackInfo[MAX_SCORE_TRACK_CHUNK];
struct PCM_AUDIO_TRACK_CHUNK AudioTrackInfo[MAX_AUDIO_TRACK_CHUNK];

//smafseq.c
//soundseq.c

#define _mgetc(a, b)	((a)[(b)++])


static PSOUNDSEQINFO seqhandle;
static int nextwave;

static SOUNDSEQINFO SoundSeqInf[MAX_SMAF_DATA_NUM];

static UINT8 CtrlDataBuf[0x100];

static UINT8 LoopDataBuf[0x40];
							
static UINT8 *pLoopDataBuf;	
static UINT32 LoopDataSize;	
static UINT8 fgLoopData;	

int FmFifo_Proc(FMCBEVTYPE event, int fifoNum, DATABLK *pBufData, void *prm);
int PcmFifo_Proc(PCMCBEVTYPE event, int fifoNum, DATABLK *pBufData, void *prm);

static UINT32 WriteDuration(UINT8 *pBuf, UINT32 t);

static UINT32 AtsqNextEvent[MAX_PCM_TRACK_NUM];
											

	
//soundseq.c



static void __wait(void)
{
	clk_busy_wait(2);
}
void Ma2_write_reg(int index,UINT8 ctrl)//	1.
{
	AUDIO_OUTP_REG_INDEX( (UINT8)index );
	regidx = (UINT8)index;						// Save index
	AUDIO_OUTP_CTL_DATA( ctrl );
	__wait();
}
static void Ma2_write_fifo(UINT8 index,UINT8 *data, UINT16 n)//	2.
{
	UINT16   i;
	
	AUDIO_OUTP_REG_INDEX( (UINT8)index );
	regidx = index;						// Save index
	__wait();
	for( i=0 ; i<n ; i++ ) {
	      AUDIO_OUTP_CTL_DATA( *data++ );
	}
}
static UINT8 Ma2_read_reg(UINT8 index)//	3.
{	
	AUDIO_OUTP_REG_INDEX( (UINT8)index );
	regidx = index;						// Save index
	__wait();
	return AUDIO_INP_CTL_DATA();
}
/***********************************************************************
	Name     : Ma2w_switch_reg
	Function : WX^P^WX^QðØèÖ¦é
	Input    : sel 			... 0=reg1, 1=reg2
	Return   : None
***********************************************************************/
static void Ma2w_switch_reg(UINT8 sel)
{     
	Ma2_write_reg(MA2_REGISTER_SW, (UINT8)(sel&0x01));
	regsw = ((UINT8)(sel&0x01));	
}

/***********************************************************************
	Name     : Ma2r_version
	Function : YMU759Ìo[Wðæ¾
	Input    : None
	Return   : UINT8 version
***********************************************************************/
UINT8 Ma2r_version(void)
{
	return Ma2_read_reg(MA2_VERSION);
}

/***********************************************************************
	Name     : Ma2r_seq_fifo_status
	Function : FM/ADPCM V[PXf[^FIFOÌXe[^XðÇÝo·
	Input    : None
	Return   : Status

				  15    14    13    12    11    10     9     8
			      ERR  ADEND FMEND  AFLG  FLG3  FLG2  FLG1  FLG0
                   7     6     5     4     3     2     1     0
				 ADERR FMERR  GEND  SEND  END3  END2  END1  END0

***********************************************************************/
UINT16 Ma2r_seq_fifo_status(void)
{
UINT16  stat1,stat2;

	Ma2w_switch_reg(0);

	stat1 = (UINT16)Ma2_read_reg(MA2_SEQ_FIFO_STAT1);
	stat2 = (UINT16)Ma2_read_reg(MA2_SEQ_FIFO_STAT2);

	return  ((UINT16)((stat1<<8)|stat2));
}

/***********************************************************************
	Name     : Ma2r_wave_fifo_status
	Function : ADPCM g`FIFOÌXe[^XðÇÝo·
	Input    : None
	Return   : Status

				  7     6     5     4     3     2     1     0
				ADERR   -   GEND  SEND  TFLG  DEMP DFULL  DFLG

***********************************************************************/
UINT8 Ma2r_wave_fifo_status(void)
{
UINT8 stat;

	Ma2w_switch_reg(0);
	stat = Ma2_read_reg(MA2_WAVE_FIFO_STAT);

    return stat;
}

/***********************************************************************
	Name     : Ma2r_seq_fifo_flags
	Function : FM/ADPCM V[PXf[^FIFOÌFull/Empty FlagðÇÝo·
	Input    : None
	Return   : Status

				  15    14    13    12    11    10     9     8
				  ERR  ADEND FMEND AFULL  FUL3  FUL2  FUL1  FUL0
				   7     6     5     4     3     2     1     0
				  ERR  ADEND FMEND  AEMP  EMP3  EMP2  EMP1  EMP0

***********************************************************************/
UINT16 Ma2r_seq_fifo_flags(void)
{
UINT16  flag1,flag2;

	Ma2w_switch_reg(0);

	flag1 = Ma2_read_reg((UINT8)MA2_SEQ_FIFO_FULL_FLAG);		// Full Flag
	flag2 = Ma2_read_reg((UINT8)MA2_SEQ_FIFO_EMPTY_FLAG);		// Empty Flag

	return ((UINT16)(((UINT16)flag1<<8)|(UINT16)flag2));
}

/**********************************************************************
	Name     : Ma2r_fm_voice_assign
	Function : ¹FtðÇÝo·
	Input    : UINT8 ch ... Channel #
	Return   : UINT8 	... voice #
***********************************************************************/
UINT8 Ma2r_fm_voice_assign(UINT8 ch)
{
UINT8 v;

	Ma2w_switch_reg(1);
	v = Ma2_read_reg((UINT8)(MA2_FM_CHANNEL_ASSIGN + ch/2));

	if( (ch&0x01)==0 )					// Even Channel
		return ((UINT8)(v&0x0f));
	else								// Odd Channel
		return  ((UINT8)((v>>4)&0x0f));
		
}

/***************************************************************************
	Name     : Ma2w_fm_seq_data
	Function : FMV[PXf[^FIFOÉPoCg«Þ
	Input    : UINT8  tr		.. track(0-3)
			   UINT8  data		.. sequence data
	Return   : None
***************************************************************************/
void Ma2w_fm_seq_data(UINT8 tr, UINT8 data)
{
	Ma2w_switch_reg(0);
	Ma2_write_reg(MA2_FM_SEQ_DATA+(tr&0x03), data);
}

/*************************************************************************
	Name     : Ma2w_fm_seq_datan
	Function : FMV[PXf[^FIFOÉoCg«Þ
	Input    : UINT8  tr		.. track(0-3)
			   UINT8  *data		.. sequence data
			   UINT16 n			.. num of data
	Return   : None
*************************************************************************/
void Ma2w_fm_seq_datan(UINT8 tr, UINT8 *data, UINT16 n)
{
	Ma2w_switch_reg(0);
	Ma2_write_fifo((UINT8)(MA2_FM_SEQ_DATA+(tr&0x03)), data, n);
}

/***********************************************************************
	Name     : Ma2w_fm_timebase
	Function : FM Sequence DataÌîÔðÝè·é
	Input    : UINT8  tb_d		... DurationîÔ (0-7)
			   UINT8  tb_g		... GateTimeîÔ (0-7)

								  Ýèl  îÔ(mS)
									0h	 	  1(default)
									1h		  2
									2h		  4
									3h		  5
									4h		 10
									5h		 20
									6h		 40
									7h		 50
	Return   : None
***********************************************************************/
void Ma2w_fm_timebase(UINT8 tb_d, UINT8 tb_g)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_FM_TIMEBASE, (UINT8)(((tb_d&0x07)<<4) + (tb_g&0x07)));
}

/***********************************************************************
	Name     : Ma2w_fm_kcon
	Function : e`lÌKey ControlðÝè·é
	Input    : UINT16 ch	... channel
			   UINT8  v		... val
	Return   : None
***********************************************************************/
void Ma2w_fm_kcon(UINT16 ch, UINT8 val)
{
	Ma2w_switch_reg(1);

	Ma2_write_reg(MA2_KCONCH0,(UINT8)(ch>>8));
	Ma2_write_reg(MA2_KCONCH1,(UINT8)(ch&0xff));
	Ma2_write_reg(MA2_KCONV, (UINT8)(val&0x0f));
}

/***********************************************************************
	Name     : Ma2w_fm_master_vol
	Function : FM Master VolumeðÝè·é
	Input    : UINT8 vol	... vol(0x00 - 0x1F)
	Return   : None
***********************************************************************/
void Ma2w_fm_master_vol(UINT8 vol)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_FM_MASTER_VOL, (UINT8)(vol&0x1f));
}

/***********************************************************************
	Name     : Ma2w_fm_channel_vol
	Function : FMe`lÌVolumeðÝè·é
	Input    : UINT8 ch		... channel(0x00-0x0f)
			   UINT8 vol	... volume (0x00-0x1f)
	Return   : None
***********************************************************************/
void Ma2w_fm_channel_vol(UINT8 ch, UINT8 vol)
{
	Ma2w_switch_reg(0);
	Ma2_write_reg(MA2_FM_CHANNEL_VOL+(ch&0x0f), (UINT8)(vol&0x1f));
}

/***********************************************************************
	Name     : Ma2w_fm_channel_pan
	Function : FMe`lÌPanpodièÊjðÝè·é
	Input    : UINT8 ch		... channel(0x00-0x0f)
			   UINT8 pan	... Panpod (0x00-0x7f)
	Return   : None
***********************************************************************/
void Ma2w_fm_channel_pan(UINT8 ch, UINT8 pan)
{
	Ma2w_switch_reg(0);
	Ma2_write_reg(MA2_FM_CHANNEL_PAN+(ch&0x0f), pan);
}

/***********************************************************************
	Name     : Ma2w_fm_modulation (for ES2.0)
	Function : FMe`lÌModulationðÝè·é
	Input    : UINT8 ch		... channel(0x00-0x0f)
			   UINT8 mod	... Modulation
	Return   : None
***********************************************************************/
void Ma2w_fm_modulation(UINT8 ch, UINT8 mod)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_FM_MODULATION+(ch&0x0f), (UINT8)(mod&0x07));
}

/***********************************************************************
	Name     : Ma2w_fm_basic_octave (for ES2.0)
	Function : FMe`lÌBasic OctaveðÝè·é
	Input    : UINT8 ch		... channel(0x00-0x0f)
			   UINT8 bo	... Basic Octave
	Return   : None
***********************************************************************/
void Ma2w_fm_basic_octave(UINT8 ch, UINT8 bo)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_FM_BO+(ch&0x0f), (UINT8)(bo&0x03));
}

/***********************************************************************
	Name     : Ma2w_fm_expression
	Function : FMe`lÌExpressionðÝè·é
	Input    : UINT8 ch		... channel(0x00-0x0f)
			   UINT8 mod	... expression
	Return   : None
***********************************************************************/
void Ma2w_fm_expression(UINT8 ch, UINT8 exp)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_FM_EXPRESSION+(ch&0x0f), (UINT8)(exp&0x1f));
}


/***********************************************************************
	Name     : Ma2w_fm_control
	Function : FMe`lÌF-number/Block#ðÝè·é
			@ KON/KOFFð§ä·é
	Input    : UINT8  ch	... channel(0x00-0x0f)
			   UINT16 fnum	... fnum[9..0]
			   UINT8  blk	... block[2..0]
			   UINT8  kon	... KON  :  1 .. Strat
										0 .. Stop
	Return   : None
***********************************************************************/
void Ma2w_fm_control(UINT8 ch, UINT16 fnum, UINT8 blk, UINT8 kon)
{
	Ma2w_switch_reg(0);
	Ma2_write_reg(MA2_FM_FNUM_LO+(ch&0x0f), (UINT8)(fnum&0xff));
	Ma2_write_reg(MA2_FM_FNUM_HI+(ch&0x0f), (UINT8)(((fnum>>8)&0x03)|
						((blk&0x07)<<2)|((kon&0x01)<<5)));
}

/***********************************************************************
	Name     : Ma2r_fm_control
	Function : FMe`lÌKON/KOFFAF-number/Block#ðÇÝo·
	Input    : UINT8  ch	... channel(0x00-0x0f)
	Return   : 
				  15    14    13    12    11    10     9     8
				   -     -    KON      BLOCK[2:0]     F-Num[9:8]
				   7     6     5     4     3     2     1     0
				                   F-Num[7:0]
***********************************************************************/
UINT16 Ma2r_fm_control(UINT8 ch)
{
	UINT8 val1, val2;
	UINT16 val;
	
	Ma2w_switch_reg(0);
	val1 = Ma2_read_reg((UINT8)(MA2_FM_FNUM_LO+(ch&0x0f)));
	val2 = Ma2_read_reg((UINT8)(MA2_FM_FNUM_HI+(ch&0x0f)));

	val = (UINT16)((((UINT16)val2) << 8) | ((UINT16)val1));
	
	return val;
}

/***********************************************************************
	Name     : Ma2w_fm_algorithm
	Function : FMe`lÌASYÆtB[hobNFMALFOðÝè·é
	Input    : UINT8 ch		... channel(0x00-0x0f)
			   UINT8 alg		... ASY
			   UINT8 fb			... tB[hobNFM
			   UINT8 lfo		... LFO
	Return   : None
***********************************************************************/
void Ma2w_fm_algorithm(UINT8 ch, UINT8 alg, UINT8 fb, UINT8 lfo)
{
	Ma2w_switch_reg(0);
	Ma2_write_reg(MA2_FM_ALGORITHM+(ch&0x0f), (UINT8)(((lfo&0x03)<<6)|((fb&0x07)<<3)|(alg&0x07)));
}


/**********************************************************************
	Name     : Ma2w_fm_voice_assign
	Function : ¹Ftðs¤
	Input    : UINT8 ch ... Channel #
			   UINT8 v  ... Voice #
	Return   : None
***********************************************************************/
void Ma2w_fm_voice_assign(UINT8 ch, UINT8 v)
{
UINT8 old;
UINT8 new;

	Ma2w_switch_reg(1);
	old = Ma2_read_reg((UINT8)(MA2_FM_CHANNEL_ASSIGN + ch/2));

	if( (ch&0x01)==0 )						// Even Channel
		new = (UINT8)((old&0xf0)|(v&0x0f));
	else									// Odd Channel
		new = (UINT8)((old&0x0f)|((v<<4)&0xf0));

	Ma2_write_reg(MA2_FM_CHANNEL_ASSIGN + ch/2, (UINT8)new); 
	
}

/**********************************************************************
	Name     : Ma2w_fm_voice_parameter
	Function : Voice ParameterðÝè·é
	Input    : struct MA2_VOICE_PARAMETER *vp
	Return   : None
***********************************************************************/
void Ma2w_fm_voice_parameter(struct MA2_VOICE_PARAMETER *vp)
{
UINT8  num;
UINT8  alg;
UINT8  idx;

	num = vp->Num;							// voice No.
	alg = (UINT8)(vp->Info&0x07); 					// 2op/4op ?
	
	Ma2w_fm_basic_octave(num, vp->Bo);
	
	Ma2w_switch_reg(0);

	Ma2_write_reg(0xd0+num,vp->Info);		// LFO,FB,ALG
	
	if( alg==0x00 || alg==0x01) {			// 2op
		idx = idx2op[num*2];
		Ma2_write_reg(idx     , vp->op1.d0);	// Operator 1
		Ma2_write_reg(idx+0x20, vp->op1.d1);
		Ma2_write_reg(idx+0x40, vp->op1.d2);
		Ma2_write_reg(idx+0x60, vp->op1.d3);
		Ma2_write_reg(idx+0x80, vp->op1.d4);
		idx = idx2op[num*2+1];
		Ma2_write_reg(idx     , vp->op2.d0);	// Operator 2
		Ma2_write_reg(idx+0x20, vp->op2.d1);
		Ma2_write_reg(idx+0x40, vp->op2.d2);
		Ma2_write_reg(idx+0x60, vp->op2.d3);
		Ma2_write_reg(idx+0x80, vp->op2.d4);
	}	                                     
	else {                         			// 4op
		if (num>=8) num -= 4;
		idx = idx4op[num*4];
		Ma2_write_reg(idx     , vp->op1.d0);	// Operator 1
		Ma2_write_reg(idx+0x20, vp->op1.d1);
		Ma2_write_reg(idx+0x40, vp->op1.d2);
		Ma2_write_reg(idx+0x60, vp->op1.d3);
		Ma2_write_reg(idx+0x80, vp->op1.d4);
		idx = idx4op[num*4+1];
		Ma2_write_reg(idx     , vp->op2.d0);	// Operator 2
		Ma2_write_reg(idx+0x20, vp->op2.d1);
		Ma2_write_reg(idx+0x40, vp->op2.d2);
		Ma2_write_reg(idx+0x60, vp->op2.d3);
		Ma2_write_reg(idx+0x80, vp->op2.d4);
		idx = idx4op[num*4+2];
		Ma2_write_reg(idx     , vp->op3.d0);	// Operator 3
		Ma2_write_reg(idx+0x20, vp->op3.d1);
		Ma2_write_reg(idx+0x40, vp->op3.d2);
		Ma2_write_reg(idx+0x60, vp->op3.d3);
		Ma2_write_reg(idx+0x80, vp->op3.d4);
		idx = idx4op[num*4+3];
		Ma2_write_reg(idx     , vp->op4.d0);	// Operator 4
		Ma2_write_reg(idx+0x20, vp->op4.d1);
		Ma2_write_reg(idx+0x40, vp->op4.d2);
		Ma2_write_reg(idx+0x60, vp->op4.d3);
		Ma2_write_reg(idx+0x80, vp->op4.d4);
	}
}

/***********************************************************************
	Name     : Ma2w_fm_rr_dr
	Function : wè³ê½¹FÌRelease RateÆDecay RateðÝè·é
	Input    : UINT8 vnum		... voice #(0x00-0x0f)
			   UINT8 val		... [0..3] Decay Rate
			   									[4..7] Release Rate
	Return   : None
***********************************************************************/
void Ma2w_fm_rr_dr(UINT8 vnum, UINT8 val)
{
	UINT8 idx;
	
	Ma2w_switch_reg(0);

	idx = idx2op[vnum*2];
	Ma2_write_reg(idx+0x20, val);
	idx = idx2op[vnum*2+1];
	Ma2_write_reg(idx+0x20, val);
}

/***********************************************************************
	Name     : Ma2_fm_ch_reset
	Function : FM`lRg[lðZbg·é
							(channel volume/panpot/modulation/expression/octave shift)
	Input    : UINT8 ch		... channel(0x00-0x0f)
	Return   : None
***********************************************************************/
void Ma2_fm_ch_reset(UINT8 ch)
{
	Ma2w_fm_channel_vol(ch, 0x18);
	Ma2w_fm_channel_pan(ch, 0x40);
	Ma2w_fm_expression(ch, 0x1f);
	Ma2w_fm_modulation(ch, 0x01);
}

/***********************************************************************
	Name     : Ma2w_fm_irq_ctrl
	Function : FM IRQ ControlWX^ðÝè·é
	Input    : UINT8 ctrl

				   7     6     5     4     3     2     1     0
				 IRQE3 IRQE2 IRQE1 IRQE0  FIFO  IRQ_Point[2:0]

	Return   : None
***********************************************************************/
void Ma2w_fm_irq_ctrl(UINT8 ctrl)
{
	fm_IrqCtrl = ctrl;
	
	Ma2w_switch_reg(0);
	if (Ma2IrqEnable) {	// èÝEnable //
		Ma2_write_reg(MA2_FM_IRQ_CTRL,ctrl);
	}
	else {	// èÝDisable //
					// èÝ|CgÌÝÝè //
		Ma2_write_reg(MA2_FM_IRQ_CTRL,(UINT8)(ctrl & 0x0f));
	}

	fifoirq = (UINT16)((((UINT16)fm_IrqCtrl&0xf0)<<4) | (((UINT16)adpcm_IrqCtrl&0x80)<<5));
	
}

/***********************************************************************
	Name     : Ma2w_adpcm_seq_data
	Function : ADPCMV[PXFIFOÉPoCg«Þ
	Input    : UINT8  data		.. sequence data
	Return   : None
***********************************************************************/
void Ma2w_adpcm_seq_data(UINT8 data)
{
	Ma2w_switch_reg(0);
	Ma2_write_reg(MA2_ADPCM_SEQ_DATA, data);
}

/***********************************************************************
	Name     : Ma2w_adpcm_seq_datan
	Function : ADPCMV[PXFIFOÉoCg«Þ
	Input    : UINT8  *data		.. sequence data
			   UINT16 n			.. num of data
	Return   : None
***********************************************************************/
void Ma2w_adpcm_seq_datan(UINT8 *data, UINT16 n)
{
	Ma2w_switch_reg(0);
	Ma2_write_fifo(MA2_ADPCM_SEQ_DATA, data, n);
}

/***********************************************************************
	Name     : Ma2w_adpcm_wave_data
	Function : ADPCMg`f[^FIFOÉPoCg«Þ
	Input    : UINT8  data		.. wave data
	Return   : None
***********************************************************************/
void Ma2w_adpcm_wave_data(UINT8 data)
{
	Ma2w_switch_reg(0);
	Ma2_write_reg(MA2_ADPCM_WAVE_DATA, data);
}

/***********************************************************************
	Name     : Ma2w_adpcm_wave_datan
	Function : ADPCMg`f[^FIFOÉoCg«Þ
	Input    : UINT8  *data		.. wave data
			   UINT16 n			.. num of data
	Return   : None
***********************************************************************/
void Ma2w_adpcm_wave_datan(UINT8 *data, UINT16 n)
{
	Ma2w_switch_reg(0);
	Ma2_write_fifo(MA2_ADPCM_WAVE_DATA, data, n);
}

/***********************************************************************
	Name     : Ma2w_adpcm_timebase
	Function : ADPCM Sequence DataÌîÔðÝè·é
	Input    : UINT8  tb_d		... DurationîÔ (0-7)
			   UINT8  tb_g		... GateTimeîÔ (0-7)

								  Ýèl  îÔ(mS)
									0h	 	  1(default)
									1h		  2
									2h		  4
									3h		  5
									4h		 10
									5h		 20
									6h		 40
									7h		 50
	Return   : None
***********************************************************************/
void Ma2w_adpcm_timebase(UINT8 tb_d, UINT8 tb_g)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_ADPCM_TIMEBASE, (UINT8)(((tb_d&0x07)<<4) + (tb_g&0x07)));
}

/***********************************************************************
	Name     : Ma2w_adpcm_master_vol
	Function : ADPCM Master VolumeðÝè·é
	Input    : UINT8 vol	... volume (0x00-0x1f)
	Return   : None
***********************************************************************/
void Ma2w_adpcm_master_vol(UINT8 vol)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_ADPCM_MASTER_VOL, (UINT8)(vol&0x1f));
}

/***********************************************************************
	Name     : Ma2w_adpcm_channel_vol
	Function : ADPCM Channel VolumeðÝè·é
	Input    : UINT8 vol	... volume (0x00-0x1f)
	Return   : None
***********************************************************************/
void Ma2w_adpcm_channel_vol(UINT8 vol)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_ADPCM_CHANNEL_VOL, (UINT8)(vol&0x1f));
}

/***********************************************************************
	Name     : Ma2_adpcm_ch_reset
	Function : ADPCM`lRg[lðZbg·é
							(channel volume)
	Input    : None
	Return   : None
***********************************************************************/
void Ma2_adpcm_ch_reset(void)
{
	Ma2w_adpcm_channel_vol(0x1f);
}

/***********************************************************************
	Name     : Ma2w_adpcm_irq_ctrl
	Function : ADPCM IRQ ControlWX^ðÝè·é
	Input    : UINT8 ctrl

				   7     6     5     4     3     2     1     0
				 IRQES IRQED GENDE IRQ_PointS   IRQ_PointD[2:0]

	Return   : None
***********************************************************************/
void Ma2w_adpcm_irq_ctrl(UINT8 ctrl)
{
	adpcm_IrqCtrl = ctrl;

	Ma2w_switch_reg(0);

	if (Ma2IrqEnable) {	// èÝEnable //
		Ma2_write_reg(MA2_ADPCM_IRQ_CTRL,ctrl);
	}
	else {	// èÝDisable //
					// èÝ|CgÌÝÝè //
		Ma2_write_reg(MA2_ADPCM_IRQ_CTRL,(UINT8)(ctrl & 0x1f));
	}

	fifoirq = (UINT16)((((UINT16)fm_IrqCtrl&0xf0)<<4) | (((UINT16)adpcm_IrqCtrl&0x80)<<5));
	wavefifoirq = (UINT8)(((adpcm_IrqCtrl&0x40)>>6) | (adpcm_IrqCtrl&0x20));
}

/***********************************************************************
	Name     : Ma2r_adpcm_irq_ctrl
	Function : ADPCM IRQ ControlWX^ðÇÝo·
	Input    : 
	Return   : 
				   7     6     5     4     3     2     1     0
				 IRQES IRQED GENDE IRQ_PointS   IRQ_PointD[2:0]

***********************************************************************/
UINT8 Ma2r_adpcm_irq_ctrl(void)
{
	UINT8 val;
	
	Ma2w_switch_reg(0);
	val = Ma2_read_reg((UINT8)MA2_ADPCM_IRQ_CTRL);
	
	return val;
}

/***********************************************************************
	Name     : Ma2w_clock_adjust
	Function : NbNügð²®·é
	Input    : long clock		... clock frequency(kHz)
	Return   : None
***********************************************************************/
void Ma2w_clock_adjust(long clock)
{
int adjust1;
int adjust2;

	Ma2w_switch_reg(1);

	adjust1 = (int)(clock+500);
        adjust1 /= 1000;
	adjust2 = 50910L * 1000L * adjust1 / clock + 500L;
        adjust2 /= 1000;

	Ma2_write_reg(MA2_CLOCK_ADJUST1, (UINT8)(adjust1&0x1f) );
	Ma2_write_reg(MA2_CLOCK_ADJUST2, (UINT8)(adjust2&0x3f) );
}


/***********************************************************************
	Name     : Ma2w_timer_timebase
	Function : TimerÌªð\ðÝè·é
	Input    : UINT8  tb		... ªð\(0-7)

								  Ýèl  ªð\(mS)
									0h	 	  1(default)
									1h		  2
									2h		  4
									3h		  5
									4h		 10
									5h		 20
									6h		 40
									7h		 50
	Return   : None
***********************************************************************/
void Ma2w_timer_timebase(UINT8 tb)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_TIMER_TIMEBASE, (UINT8)(tb&0x07));
}

/***********************************************************************
	Name     : Ma2w_eq_vol
	Function : EQ VolumeðÝè·é
	Input    : UINT8 vol	... volume (0x00-0x1f)
	Return   : None
***********************************************************************/
void Ma2w_eq_vol(UINT8 vol)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_EQ_VOL, (UINT8)(vol&0x1f));
}

/***********************************************************************
	Name     : Ma2w_hp_vol
	Function : wbhzVolumeðÝè·é
	Input    : UINT8 vol_L	... L vol (0x00-0x1f)
			   UINT8 vol_R  ... R vol (0x00-0x1f)
	Return   : None
***********************************************************************/
void Ma2w_hp_vol(UINT8 vol_L, UINT8 vol_R)
{
	Ma2w_switch_reg(1);

	Ma2_write_reg(MA2_HP_VOL_L, (UINT8)(vol_L&0x1f));
	Ma2_write_reg(MA2_HP_VOL_R, (UINT8)(vol_R&0x1f));
}

/***********************************************************************
	Name     : Ma2w_sp_vol
	Function : Xs[J[VolumeðÝè·é
	Input    : UINT8 vol	... volume (0x00-0x1f)
	Return   : None
***********************************************************************/
void Ma2w_sp_vol(UINT8 vol)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_SP_VOL, vol);			// !!!!!
	
}

/***********************************************************************
	Name     : Ma2r_hpl_vol
	Function : wbhzVolume LchðÇÝo·
	Input    : None
	Return   : UINT8 vol	... L vol (0x00-0x1f)
***********************************************************************/
UINT8 Ma2r_hpl_vol(void)
{
	UINT8 vol;
	
	Ma2w_switch_reg(1);

	vol = Ma2_read_reg(MA2_HP_VOL_L);
	
	return (UINT8)(vol&0x1f);
}

/***********************************************************************
	Name     : Ma2r_hpr_vol
	Function : wbhzVolume RchðÇÝo·
	Input    : None
	Return   : UINT8 vol	... R vol (0x00-0x1f)
***********************************************************************/
UINT8 Ma2r_hpr_vol(void)
{
	UINT8 vol;
	
	Ma2w_switch_reg(1);

	vol = Ma2_read_reg(MA2_HP_VOL_R);
	
	return (UINT8)(vol&0x1f);
}

/***********************************************************************
	Name     : Ma2r_sp_vol
	Function : Xs[J[VolumeðÇÝo·
	Input    : None
	Return   : UINT8 vol	... volume (0x00-0x1f)
***********************************************************************/
UINT8 Ma2r_sp_vol(void)
{
	UINT8 vol;
	
	Ma2w_switch_reg(1);
	vol = Ma2_read_reg(MA2_SP_VOL);			// !!!!!
	
	return vol;
}

/***********************************************************************
	Name     : Ma2w_power_management
	Function : Power ManagementWX^ðÝè·é
	Input    : UINT8 ctrl

				   7     6     5     4     3     2     1     0
				  CLR   AP4   AP3   AP2   AP1   AP0   DP1   DP0

	Return   : None
***********************************************************************/
void Ma2w_power_management(UINT8 ctrl)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_POWER_MANAGEMENT,ctrl);
}

/***********************************************************************
	Name     : Ma2w_led_control
	Function : OLED_Å§äðÝè·é
	Input    : UINT8 ctrl

				  7     6     5     4     3     2     1     0
			 	 OUT    0     0       FREQ[2:0]      MODE[1:0]

			   UINT8 syncch		.. ¯ú·é`l(0x00-0x0f)
	Return   : None
***********************************************************************/
void Ma2w_led_control(UINT8 ctrl,UINT8 syncch)
{
	Ma2w_switch_reg(1);

	Ma2_write_reg(MA2_LED_CTRL,ctrl);
	_syncch &= 0xf0;
	_syncch |= (syncch&0x0f);
	Ma2_write_reg(MA2_SYNCCH,_syncch);
}

/***********************************************************************
	Name     : Ma2w_motor_control
	Function : OoCu[^p[^[§äðÝè·é
	Input    : UINT8 ctrl

				  7    6    5    4    3    2    1   0
			 	 OUT   0    HW[1:0]   LW[1:0] MODE[1:0]

			   UINT8 syncch

	Return   : None
***********************************************************************/
void Ma2w_motor_control(UINT8 ctrl,UINT8 syncch)
{
	Ma2w_switch_reg(1);

	Ma2_write_reg(MA2_MOTOR_CTRL,ctrl);
	_syncch &= 0x0f;
	_syncch |= ((syncch<<4)&0xf0);
	Ma2_write_reg(MA2_SYNCCH,_syncch);
}

/***********************************************************************
	Name     : Ma2w_ext_pin_control
	Function : O[q@\ðÝè·é
	Input    : UINT8 ctrl

				  7   @6@   5     4   @3     2     1     0
			 	D7LED D6MTR D5SDO   -     -     -   IRQEXT[1:0]

	Return   : None
***********************************************************************/
void Ma2w_ext_pin_control(UINT8 ctrl)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_EXT_PIN_CTRL,ctrl);
}

/***********************************************************************
	Name     : Ma2w_fifo_control
	Function : FIFOðZbg·é
	Input    : UINT8 ctrl

				  7   @6@   5     4   @3     2     1     0
				ADCLR SQCLR FMCLR RPTE   RPT  DFCLR AFCLR FFCLR

	Return   : None
	
	Note 	 : n[hEFAÉË¶·é½ßn[hEFA
			   Éí¹Ä«¼·Kvª é
***********************************************************************/
void Ma2w_fifo_control(UINT8 ctrl)
{
	Ma2w_switch_reg(1);
	
	Ma2_write_reg(MA2_FIFO_CTRL,ctrl);

	if (ctrl & 0xE0) {

		// CLKIÅ1024NbNÈãWait·éR[h}ü //

		Ma2_write_reg(MA2_FIFO_CTRL, (UINT8)0x00);
	}
	
}

/***********************************************************************
	Name     : Ma2w_control
	Function : Sequencer/ADPCM/Timerð§ä·é
	Input    : UINT8 ctrl

				  7   @6@   5     4   @3     2     1     0
			 	MA-1  PLLDW   -   TMST   FS    ST   ADST  FMST

	Return   : None
***********************************************************************/
void Ma2w_control(UINT8 ctrl)
{
	Ma2w_switch_reg(1);
	Ma2_write_reg(MA2_CONTROL,ctrl);
}
 
/***********************************************************************
	Name     : Ma2r_control
	Function : Read Sequencer/ADPCM/Timerð§ä·é
	Input    : None
	Return   : UINT8 ctrl

				  7   @6@   5     4   @3     2     1     0
			 	MA-1  PLLDW   -   TMST   FS    ST   ADST  FMST

***********************************************************************/
UINT8 Ma2r_control(void)
{
	Ma2w_switch_reg(1);
	return Ma2_read_reg((UINT8)MA2_CONTROL);
}

/***********************************************************************
	Name     : Ma2_Irq_Control
	Function : MA-2ÌèÝðEnable/Disable·é
	Input    : UINT8 bCtrl
	            0 ... Disable
	            1 ... Enable
	Return   : None
***********************************************************************/
void Ma2_Irq_Control(UINT8 bCtrl)
{
	Ma2IrqEnable = bCtrl;

	Ma2w_fm_irq_ctrl(fm_IrqCtrl);
	Ma2w_adpcm_irq_ctrl((UINT8)(adpcm_IrqCtrl));
	
}

/***********************************************************************
	Name     : Ma2_Save_Regs
	Function : MA-2èÝÌO
	Input    : None
	Return   : None
***********************************************************************/
static void Ma2_Save_Regs(void)
{
	save_regsw  = regsw;
	save_regidx = regidx;
}

/***********************************************************************
	Name     : Ma2_Restore_Regs
	Function : MA-2èÝÌã
	Input    : None
	Return   : None
***********************************************************************/

static void Ma2_Restore_Regs(void)
{
	Ma2w_switch_reg(save_regsw);
	regidx = save_regidx;
	AUDIO_OUTP_REG_INDEX( regidx );			// restore index
	
}
/**********************************************************************
	Name	 :	Ma2_Irq_Proc
	Function :	MA-2èÝGg
	Input 	 :	None
	Return 	 :	None

	Note		 :	MA2©çÌn[hEFAèÝª­¶µ½êÉÍA
							±ÌÖðÄÑo·æ¤o^·é±Æ
**********************************************************************/
void Ma2_Irq_Proc(void)
{
UINT8  i;
UINT8 count=0;

	if( bMa2IrqHandler==0 ) return;

	Ma2_Save_Regs();					// Save Register Environment
	
	Ma2_Irq_Control(0);		// IRQ Disable
	
	do {	// èÝÉstatusªÏíÁ½êAA±iÅå10ñj //
		
		for( i=0 ; i<bMa2IrqHandler ; i++ ) {
			if( Ma2IrqHandler[i].p!=NULL )
				Ma2IrqHandler[i].p();		// Call IRQ Handler
		}

		if (count++ > 10) break;
		
	} while ((Ma2r_wave_fifo_status() & wavefifoirq) || 
				(Ma2r_seq_fifo_status() & fifoirq));

	Ma2_Irq_Control(1);		// IRQ Enable

	Ma2_Restore_Regs();					// Restore Register Environment
}

/**********************************************************************
		Name :		AddMa2IrqHandler
		Function :	MA-2èÝÌÖðo^·é
		Input :		void (*func)()		.. èÝÌCallbackÖ
		Output :	handle #(0,1...)
**********************************************************************/
SINT8 AddMa2IrqHandler(void (*func)(void))
{
SINT8 i;

	if( bMa2IrqHandler>MA2_IRQ_HANDLE_MAX ) return -1;

	for( i=0 ; i<MA2_IRQ_HANDLE_MAX ; i++ ) {
		if( Ma2IrqHandler[i].p==NULL ) {
			Ma2IrqHandler[i].p = func;
			++bMa2IrqHandler;
			return i;
		}
	}
	
	return -1;
}

/**********************************************************************
		Name :		RemeveMa2IrqHandler
		Function :	MA-2èÝÌÖðí·é
		Input :		handle #
		Output :	1 : ³í
					0 : G[
**********************************************************************/
bool RemoveMa2IrqHandler(SINT8 handle)
{
	if( handle>MA2_IRQ_HANDLE_MAX || bMa2IrqHandler==0 ) return false;
	if( Ma2IrqHandler[handle].p==NULL ) return false;

	Ma2IrqHandler[handle].p = NULL;
	--bMa2IrqHandler;
	return true;
}

//MA2Manager.c
/******************************************************************
		Name :		MA2Manager_Initialize
		Function :	MA2 ManagerÌú»
******************************************************************/
void  MA2Manager_Initialize(void)
{
	Ma2_resource_info.NumFmCh    = 16;
	Ma2_resource_info.FmChStatus = 0;
	Ma2_resource_info.NumAdpcmCh = 1;
	Ma2_resource_info.AdpcmChStatus = 0;
}

/******************************************************************
		Name :		MA2Manager_AllocFmCh
		Function :	FM ChannelðmÛ·é
		Input :		UINT32  ch
					mÛµ½¢CHÌBitð '1'
					( bit0,bit1... <-> Ch0,Ch1...)
		Output :	³í  	 0
					G[	-1
******************************************************************/
bool MA2Manager_AllocFmCh(UINT32 ch)
{
	UINT8 chno;

	if( (ch&0xffff0000)!=0 ) return -1;
	if( (Ma2_resource_info.FmChStatus & ch)!=0 ) return -1;

	for (chno=0; chno<Ma2_resource_info.NumFmCh; chno++) {
		if (ch & (0x01L<<chno)) {
			if (((chno >= 4) && (chno < 8)) || (chno >= 12)) {
				if ((Ma2_resource_info.FmChStatus & (0x01L<<(chno-4))) == 0)
					Ma2w_fm_algorithm((UINT8)(chno-4), (UINT8)0, (UINT8)0, (UINT8)0);
			}
			Ma2w_fm_algorithm(chno, (UINT8)0, (UINT8)0, (UINT8)0);
			Ma2_fm_ch_reset(chno);
		}
	}

	Ma2_resource_info.FmChStatus |= ch;
	
	return 0;
}

/******************************************************************
		Name :		MA2Manager_FreeFmCh
		Function :	FM ChannelðJú·é
		Input :		Júµ½¢CHÌBitð '1'
					(bit0,bit1... <-> Ch0,Ch1...)
		Output :	³í	 0
					G[	-1
******************************************************************/
bool MA2Manager_FreeFmCh(UINT32 ch)
{
	if( (ch&0xffff0000)!=0 ) return -1;
	if( (Ma2_resource_info.FmChStatus & ch)!=ch ) return -1;

	Ma2_resource_info.FmChStatus ^= ch;

	return 0;
}

/******************************************************************
		Name :		MA2Manager_AllocAdpcmCh
		Function :	Adpcm ChannelðmÛ·é
		Input :		mÛµ½¢CHÌBitð '1'
					(bit0,bit1... <-> Ch0,Ch1...)
		Output :	³í	 0
					G[	-1
******************************************************************/
bool MA2Manager_AllocAdpcmCh(UINT8 ch)
{          
	UINT8 chno;
	
	if( (ch&0xfe)!=0 ) return -1;
	if( (Ma2_resource_info.AdpcmChStatus & ch)!=0 ) return -1;

	Ma2_resource_info.AdpcmChStatus |= ch;

	for (chno=0; chno<Ma2_resource_info.NumAdpcmCh; chno++) {
		if (ch & (0x01L<<chno))
			Ma2_adpcm_ch_reset();
	}

	return 0;
}

/******************************************************************
		Name :		MA2Manager_FreeAdpcmCh
		Function :	Adpcm ChannelðJú·é
		Input :		Júµ½¢CHÌBitð '1'
					(bit0,bit1... <-> Ch0,Ch1...)
		Output :	³í	 0
					G[	-1
******************************************************************/
bool MA2Manager_FreeAdpcmCh(UINT8 ch)
{
	if( (ch&0xfe)!=0 ) return -1;
	if( (Ma2_resource_info.AdpcmChStatus & ch)!=ch ) return -1;

	Ma2_resource_info.AdpcmChStatus ^= ch;

	return 0;
}

/******************************************************************
		Name :		MA2Manager_GetResourceInfo
		Function :	MA2\[XÌgpóµð|[g·é
		Input :		
		Output :	
******************************************************************/
void  MA2Manager_GetResourceInfo(struct MA2_RESOURCE_INFO *info)
{
	info->NumFmCh       = Ma2_resource_info.NumFmCh;
	info->FmChStatus    = Ma2_resource_info.FmChStatus;
	info->NumAdpcmCh    = Ma2_resource_info.NumAdpcmCh;
	info->AdpcmChStatus = Ma2_resource_info.AdpcmChStatus;
}

/******************************************************************
		Name :		MA2Manager_Ma2IrqControl
		Function :	MA2©çÌèÝðêIÉEnable/Disable·é
		Input :		0 ... Disable
					1 ... Enable
		Output :	None
******************************************************************/
void MA2Manager_Ma2IrqControl(UINT8 bCtrl)
{
	switch( bCtrl ) {
		case 0:					// Disable
			Ma2_Irq_Control(0);
			break;
		case 1:					// Enable
			Ma2_Irq_Control(1);
			break;
	}

}

void Ma2_initialize(void)
{
	UINT8  i;
	bMa2IrqHandler = 0;
	for( i=0 ; i<MA2_IRQ_HANDLE_MAX ; i++ )
		Ma2IrqHandler[i].p = NULL;

	regsw = 1;
	Ma2w_switch_reg(0);
	_syncch = 0;

	Ma2w_power_management(0x7f);			// set DP0     0111 1111
	Ma2w_power_management(0x7e);			// clear DP0   0111 1110
	Ma2w_power_management(0x7a);			// clear AP0   0111 1010
	Ma2w_control(0x00);					    // clear PLLDW 0000 0000 (addr index : 0x01)
	// 10ms Wait
	clk_busy_wait(15000);
	Ma2w_power_management(0x78);			// clear DP1    0111 1000
	Ma2w_power_management(0xf8);			// set CLR      1111 1000
	// CLKI 1024 Wait
	clk_busy_wait(55);
	Ma2w_power_management(0x78);			// clear CLR    0111 1000
	
   	Ma2w_ext_pin_control(0x00); 			// Init Ext Pin Function
	Ma2w_fifo_control(0x1f);				// Reset All FIFO

	  //****- MA2º¼·ý ÃÊ±â ¼³Á¤  ********//
	snd_IsPowerOn=TRUE;
	
	if(fMotorOn)
		Ma2w_motor_control(0x80,0);
	if(fLEDOn)
		Ma2w_led_control(0x80,0);

}
//MA2Manager.c
//main.c
int SmafCBFunc(SMAFCBEVTYPE event, void *prm)
{	

	switch (event) {
		case SMAF_PLAYSTART:	/* Playback start */
				break;
		case SMAF_PLAYSTOP:		/* Playback stop */
				break;
		case SMAF_PLAYEND:	/* Playback end */
				break;
		case SMAF_ERROR:		/* Error */
				break;
	}
	
	return 1;
}
//main.c
//Ma2seq.c

//==========================================================================
//  UINT8 CnvTimebase(UINT32 tbase)
//
//  Description: TimeBaselÌÏ·@msec@->@MA2Ýèl
//  Parameter:
//  Return :	0xff	 G[
//  					!=0xff TimebaseÝèl
//==========================================================================
UINT8 CnvTimebase(UINT32 tbase)
{
	switch (tbase) {
		case  1: return 0x00;
		case  2: return 0x01;
		case  4: return 0x02;
		case  5: return 0x03;
		case 10: return 0x04;
		case 20: return 0x05;
		case 40: return 0x06;
		case 50: return 0x07;
	}
	
	return 0xff;
}


//==========================================================================
//  void Seq_Initialize()
//
//  Description: MA2V[PTW[Ìú»
//  Parameter:	Èµ
//  Return :		Èµ
//==========================================================================
void Seq_Initialize()
{    
	//-- W[àÏÌú» ----------------------//
	mapFifo = 0;
	usedFifo = 0;
	stateFm = FM_NOTEXIST;
	statePcm = PCM_NOTEXIST;
	infFm.fifoSize = MAX_FM_FIFO_SIZE;
	infFm.irqSize = IRQ_FM_FIFO_SIZE;   
	infPcm.seqFifoSize = MAX_PCM_SEQFIFO_SIZE;
	infPcm.wavFifoSize = MAX_PCM_WAVFIFO_SIZE;
	infPcm.seqIrqSize = IRQ_PCM_SEQFIFO_SIZE;
	infPcm.wavIrqSize = IRQ_PCM_WAVFIFO_SIZE;  
	fmIrqCtrl = 0;     
	pcmIrqCtrl = 0;
	ma2Ctrl = 0;     
	ma2FifoCtrl = 0;

	//-- MA2foCXhCo©çÌR[obNÖo^ -//
	ma2IrqHdl = -1;
	
}


//==========================================================================
//  int FmSeq_Open(int (*FmFifoProc)(FMCBEVTYPE, int, DATABLK *, void *), 
//                 UINT8 fifoMap)
//
//  Description: FMV[PTOPEN
//  Parameter:	
//  					FmFifoProc	MA2à FMV[PXf[^Rg[pR[obNÖ
//  											ÖÌ|C^
//  					fifoMap			gp·éFIFOÌ}bv	lª1ÌrbgÌÔÌFIFOðgp
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmSeq_Open(int (*FmFifoProc)(FMCBEVTYPE, int, DATABLK *, void *), UINT8 fifoMap)
{
 	//-- Xe[^XªNOT EXISTÅÈ¯êÎG[ðÔ· ---//
	if (stateFm != FM_NOTEXIST) {
		return SEQ_ERROR;
	}
	
	//-- MA-2foCXhCoÖÌèÝÖÌo^  ---//
	//  MA-2ÌèÝª­¶µ½êASeq_Irq_Proc()ª  //
	//  ÄÑo³êé                                    //
	if (ma2IrqHdl == -1) {
		ma2IrqHdl = AddMa2IrqHandler(Seq_Irq_Proc);
		if (ma2IrqHdl == -1)
			return SEQ_ERROR;
	}

	//-- FMV[PXf[^Rg[pCallbackÖÌo^ --//
	FmIntProc = FmFifoProc;

	//-- gp·éFIFOÔÌo^ --//
	mapFifo = fifoMap;
	
	//-- àóÔðOPENEDÖÚs --//
	stateFm = FM_OPENED;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int FmSeq_Close()
//
//  Description: FMV[PTN[Y
//  Parameter:
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmSeq_Close()
{
 	//----- Xe[^XªOPENE é¢ÍREADYÅÈ¯êÎG[ðÔ· -------//
	if ((stateFm != FM_OPENED) && (stateFm != FM_READY)) {
		return SEQ_ERROR;
	}

	//----- àÏÌú» -------------------------//
	FmIntProc = 0;
	mapFifo = 0;
	usedFifo = 0;
	fmIrqCtrl = 0;
	infFm.useFmCh = 0;     
	
	//----- MA-2foCXhCoÖÌèÝÖÌí -----------------//
	if (statePcm == PCM_NOTEXIST) {
		RemoveMa2IrqHandler(ma2IrqHdl);
		ma2IrqHdl = -1;
	}

	//----- àóÔðNOT EXISTÖÚs ----------------------------------//
	stateFm = FM_NOTEXIST;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int FmSeq_Standby(FMSEQINIDATA *fmi, void *prm)
//
//  Description: FMV[PTÄ¶õ
//  Parameter:
//  					fmi		FMV[PTÌúÝèîñði[µ½\¢ÌÖÌ|C^
//  					prm		R[obNÖÖn·øl
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmSeq_Standby(FMSEQINIDATA *fmi, void *prm)
{
	DATABLK BufData;
	UINT8 timebaseD, timebaseG;
	UINT8 fifoNo;                
	UINT8 mssgSize;
	UINT32 chunkcount, mssgcount;
	UINT8 egtVal, rrVal;

	//-- Xe[^XªOPENEDÅÈ¯êÎG[ðÔ· ----------------------//
	if (stateFm != FM_OPENED) {
		return SEQ_ERROR;
	}
	
	//----- R[obNÖÖn·p[^ðL^ ---------------------//
	infFm.prm = prm;
	
	//----- fmiÌf[^ÅMA2ðú» -----------------------------------//
	//-- FMV[PTÌ^Cx[XÝè ----------------//
	timebaseD = CnvTimebase(fmi->dTimebaseD);
	timebaseG = CnvTimebase(fmi->dTimebaseG);
	Ma2w_fm_timebase(timebaseD, timebaseG);
	
	//-- MA2Ì­¹`lðo^ ---------------------//
	infFm.useFmCh = fmi->reqFmCh;
	
	//----- CallbackÖÅStandbyðs¤ ----------------------------//
	for (fifoNo=0; fifoNo<MAX_FIFO_NUM; fifoNo++) {
		if (mapFifo & (0x01<<fifoNo)) {
			FmIntProc(FM_STANDBY, fifoNo, &BufData, prm);
			if (BufData.Size) {
				chunkcount = 0;
				while (chunkcount < BufData.Size) {
					//----- ExclusivebZ[Wð1Â¸ÂFIFO0ÉüêÄ·é -----//
					//      FF F0 ...... F7 : Exclusive Message                   //
					mssgSize = 0;
					mssgcount = 0;
					if (*(BufData.pBuf)++ == 0xFF) {
						if (*(BufData.pBuf)++ == 0xF0) {
							Ma2w_fm_seq_data(0, 0x00);	// Duration=0ðÇÁ //
							Ma2w_fm_seq_data(0, 0xFF);
							Ma2w_fm_seq_data(0, 0xF0);
							mssgSize = *(BufData.pBuf);						// MessageTCYð¾é //
							Ma2w_fm_seq_data(0, *(BufData.pBuf)++);
							
							// MessageTCYªFIFOÖ«Ý           //
							// MA-1pVoice MessageÌêp[^ÏX //
							if ((*(BufData.pBuf) == 0x43) && (*(BufData.pBuf+1) == 0x02)) { 
								// for MA-1 //
								Ma2w_fm_seq_data(0, *(BufData.pBuf)++);	// MakerID //
								Ma2w_fm_seq_data(0, *(BufData.pBuf)++);	// FormatID //
								mssgcount += 2;
								if ((*(BufData.pBuf) & 0xf8) == 0x00) { 
									// MA-1 Voice Message //
									for ( ; mssgcount<10; mssgcount++) {
										Ma2w_fm_seq_data(0, *(BufData.pBuf)++);
									}
									egtVal = (UINT8)((*(BufData.pBuf)) & 0x08);
									rrVal = (UINT8)((((*(BufData.pBuf)) & 0x03) << 2) | (((*(BufData.pBuf+1)) & 0xc0) >> 6));
									if ((egtVal == 0) && (rrVal <= 5)) { 
										// p[^ÏX è //
										Ma2w_fm_seq_data(0, (UINT8)((*(BufData.pBuf) & 0xf4) | 0x0a));
										Ma2w_fm_seq_data(0, (UINT8)((*(BufData.pBuf+1) & 0x3f) | 0x40));
									}
									else { 
										// p[^ÏXÈµ //
										Ma2w_fm_seq_data(0, *(BufData.pBuf));
										Ma2w_fm_seq_data(0, *(BufData.pBuf+1));
									}
									BufData.pBuf += 2;
									mssgcount += 2;
								}
								// cèf[^«Ý //
								for ( ; mssgcount<mssgSize; mssgcount++) {
									Ma2w_fm_seq_data(0, *(BufData.pBuf)++);
								}
							}
							else {	
								// Not MA-1 //
								for (mssgcount=0; mssgcount<mssgSize; mssgcount++) {
									Ma2w_fm_seq_data(0, *(BufData.pBuf)++);
								}
							}
							
							chunkcount += mssgSize + 3;

							//-- 1bZ[W«ñ¾çFIFO -----//
							MA2Manager_Ma2IrqControl(0);
							ma2Ctrl = MA2C_CTRL_FMST;
							Ma2w_control((UINT8)(Ma2r_control() | ma2Ctrl));
							MA2Manager_Ma2IrqControl(1);
							//-- FIFOÉ¢ê½f[^ª³êIíéÜÅÒÂ --//
							//-- n[hEFAÌÇÝoµª³µ­Å«ÈêÉ --//
							//-- õ¦ÄA^C}[Éæè³À[vÉ×çÈ¢ --//
							//-- æ¤VXeÉ í¹ÄÎµÄ­¾³¢     --//
							//-- ÈºAáÅ·B                             --//
			
							// I^C}[Zbg

							while (!(Ma2r_seq_fifo_flags() & 0x0001)) {
								if (Ma2r_seq_fifo_status() & MA2S_SQ2_FMERR) {
									return SEQ_ERROR;
								}

								//	if (^C}[ªÝèlð´¦½ç) {
								//		^C}[XgbvG
								//		return SEQ_ERROR;
								//	}
							}

							// ^C}[Xgbv;
			
							//-- 	FIFOXgbv -------------------//
							MA2Manager_Ma2IrqControl(0);
							ma2Ctrl = MA2C_CTRL_FMST;
							Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
							MA2Manager_Ma2IrqControl(1);
						}
					}
				}
			}
		}
	}

	//----- àóÔðREADYÖÚs --------------------------------------//
	stateFm = FM_READY;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int FmSeq_Start()
//
//  Description: FMV[PTX^[g
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmSeq_Start()
{                   
 	//----- Xe[^XªREADYÅÈ¯êÎG[ðÔ· --------------------//
	if (stateFm != FM_READY) {
		return SEQ_ERROR;
	}
                        
	//----- àóÔðPLAYINGÖÚs ------------------------------------//
	stateFm = FM_PLAYING;

	//----- gpÌFIFOÌtOð§Äé -------------------------------//
	usedFifo = mapFifo;

	//----- èÝ|CgÝè ---------------------------------------//
	fmIrqCtrl = IRQ_FM_FIFO_POINT; 
	Ma2w_fm_irq_ctrl(fmIrqCtrl);
	
	//----- èÝEnable ---------------------------------------------//
	fmIrqCtrl |= (usedFifo << 4);
	Ma2w_fm_irq_ctrl(fmIrqCtrl);

	//----- MA-2 FM SequencerX^[g ----------------------------------//
	ma2Ctrl = MA2C_CTRL_FMST;
	MA2Manager_Ma2IrqControl(0);
	Ma2w_control((UINT8)(Ma2r_control() | ma2Ctrl));
	MA2Manager_Ma2IrqControl(1);

	return SEQ_SUCCEED;
}


//==========================================================================
//  int FmSeq_Stop()
//
//  Description: FMV[PT[Xgbv
//  Parameter:	Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmSeq_Stop()
{
	UINT8 fmch;
	UINT16 sval;
	
 	//----- Xe[^XªPLAYINGÅÈ¯êÎI¹ ------------------//
	if (stateFm != FM_PLAYING) {
		return SEQ_SUCCEED;
	}

	//----- FM FIFO èÝ Disable -----------------------------------//
	fmIrqCtrl &= ~(MA2C_FM_IRQE0 | MA2C_FM_IRQE1 | MA2C_FM_IRQE2 | MA2C_FM_IRQE3);
	Ma2w_fm_irq_ctrl(fmIrqCtrl); 

	//----- MA-2 FM SequencerXgbv ----------------------------------//
	ma2Ctrl = MA2C_CTRL_FMST;
	MA2Manager_Ma2IrqControl(0);
	Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
	MA2Manager_Ma2IrqControl(1);

	//----- MA-2 FM ­¹Xgbv ---------------------------------------//
	//	­¹Ì¹ð­§Xgbv·é                                    //
	for (fmch=0; fmch<MAX_FM_CHANNEL_NUM; fmch++) {
		if (infFm.useFmCh & (0x0001 << fmch)) {
			sval = Ma2r_fm_control(fmch);
			Ma2w_fm_control(fmch, (UINT16)(sval&0x3ff), (UINT8)((sval&0x1c00)>>10), (UINT8)0);
			Ma2w_fm_channel_vol(fmch, (UINT8)0x00);
			Ma2w_fm_rr_dr(fmch, (UINT8)0xff); // Release RateÌÏX //
		}
	}

	//----- FM FIFO clear ----------------------------------------------//
	if (statePcm != PCM_PLAYING)
		ma2FifoCtrl = MA2C_FIFO_FFCLR | MA2C_SQCLR;
	else
		ma2FifoCtrl = MA2C_FIFO_FFCLR;
	Ma2w_fifo_control(ma2FifoCtrl);
	
	//----- gpFIFOÌtOð0É·é --------------------------------//
	usedFifo = 0;

	//----- àóÔðOPENEDÖÚs -------------------------------------//
	stateFm = FM_OPENED;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int FmSeq_FifoClear(void)
//
//  Description: FMV[PTFIFOÌNA
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmSeq_FifoClear(void)
{
	//----- FM FIFO clear ----------------------------------------------//
	ma2FifoCtrl = MA2C_FIFO_FFCLR; 
	Ma2w_fifo_control(ma2FifoCtrl);

	return SEQ_SUCCEED;
}



//==========================================================================
//  int FmSeq_ControlSeek(UINT8 fifoNo, UINT8 *hCtrlData, UINT16 CtrlDataSize)
//
//  Description: FMV[PTSeekÌRg[bZ[W
//  Parameter:
//  					fifoNo				FIFOÔ
//  					hCtrlData			oµ½Rg[bZ[WÌi[³ê½ÌæÖÌ|C^
//  					CtrlDataSize	oµ½Rg[bZ[WÌi[³ê½ÌæÌTCY
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmSeq_ControlSeek(UINT8 fifoNo, UINT8 *hCtrlData, UINT16 CtrlDataSize)
{
	FIFOINF FifoInf;
	UINT32 writeSize;

	//-- Xe[^XªREADYÅÈ¯êÎG[ðÔ· ----------------------//
	if (stateFm != FM_READY) {
		return SEQ_ERROR;
	}

	if (!(mapFifo & (0x01<<fifoNo))) {
		return SEQ_SUCCEED;
	}

	if (CtrlDataSize) {
		FifoInf.hBuffer = hCtrlData;
		FifoInf.dBufSize = (UINT32)CtrlDataSize;
		FifoInf.dBufCount = 0;
		while (FifoInf.dBufCount < FifoInf.dBufSize) {
			writeSize = WriteToFmFifo(fifoNo, infFm.fifoSize, &FifoInf);
			//-- FIFO -----//
			MA2Manager_Ma2IrqControl(0);
			ma2Ctrl = MA2C_CTRL_FMST;
			Ma2w_control((UINT8)(Ma2r_control() | ma2Ctrl));
			MA2Manager_Ma2IrqControl(1);
			//-- FIFOÉ¢ê½f[^ª³êIíéÜÅÒÂ --//
			//-- n[hEFAÌÇÝoµª³µ­Å«ÈêÉ --//
			//-- õ¦ÄA^C}[Éæè³À[vÉ×çÈ¢ --//
			//-- æ¤VXeÉ í¹ÄÎµÄ­¾³¢     --//
			//-- ÈºAáÅ·B                             --//
			
			// I^C}[Zbg

			while (!(Ma2r_seq_fifo_flags() & (0x0001 << fifoNo))) {
				if (Ma2r_seq_fifo_status() & MA2S_SQ2_FMERR) {
					return SEQ_ERROR;
				}
			//	if (^C}[ªÝèlð´¦½ç) {
			//		^C}[XgbvG
			//		return SEQ_ERROR;
			//	}
			}

			// ^C}[Xgbv;
			
			//-- 	FIFOXgbv -------------------//
			MA2Manager_Ma2IrqControl(0);
			ma2Ctrl = MA2C_CTRL_FMST;
			Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
			MA2Manager_Ma2IrqControl(1);
			
			FifoInf.dBufCount += writeSize;
		}
	}
	

	return SEQ_SUCCEED;
}


//==========================================================================
//  int FmSeq_Seek(UINT8 fifoNo, UINT8 *SeekPoint, UINT32 SeekDuration)
//
//  Description: FMV[PTSeek
//  Parameter:
//  					fifoNo				FIFOÔ
//  					SeekPoint			V[PXf[^ÌSeekÊuFCxgÌæªðw·
//  					SeekDuration	SeekJn©çÅÌCxg­¶ÜÅÌDuration
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmSeq_Seek(UINT8 fifoNo, DATABLK *pBufData, UINT32 SeekDuration)
{
	PFIFOINF hFifoInf;

	//-- Xe[^XªREADYÅÈ¯êÎG[ðÔ· ----------------------//
	if (stateFm != FM_READY) {
		return SEQ_ERROR;
	}

	hFifoInf = &(infFm.fifoInf[fifoNo]);
	
	hFifoInf->hBuffer = pBufData->pBuf;
	hFifoInf->dBufSize = pBufData->Size;
	hFifoInf->dBufCount = 0;
	
	//-- f[^ðFIFOÉ«Þ --// 
	if (hFifoInf->dBufCount < hFifoInf->dBufSize) { 
		if (SeekDuration != 0xFFFFFFFF) { // æªÖÌSeekÌêDurationÍKvÈ¢ //
			PutDuration(fifoNo, SeekDuration);
			hFifoInf->dBufCount += WriteToFmFifo(fifoNo, infFm.fifoSize-10, hFifoInf);
		}
		else {
			hFifoInf->dBufCount += WriteToFmFifo(fifoNo, infFm.fifoSize, hFifoInf);
		}
	}
	
	return SEQ_SUCCEED;
}


//==========================================================================
//  int PcmSeq_Open(int (*PcmFifoProc)(PCMCBEVTYPE, int, DATABLK *, void *))
//
//  Description: ADPCMV[PT[I[v
//  Parameter:
//  					PcmFifoProc		MA2à ADPCMV[PTRg[pR[obNÖ
//  												ÖÌ|C^
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmSeq_Open(int (*PcmFifoProc)(PCMCBEVTYPE, int, DATABLK *, void *))
{
 	//-- Xe[^XªNOT EXISTÅÈ¯êÎG[ðÔ· ---//
	if (statePcm != PCM_NOTEXIST) {
		return SEQ_ERROR;
	}
	
	// MA-2foCXhCoÖÌèÝÖÌo^ //
	if (ma2IrqHdl == -1) {
		ma2IrqHdl = AddMa2IrqHandler(Seq_Irq_Proc);
		if (ma2IrqHdl == -1)
			return SEQ_ERROR;
	}

	// ADPCMf[^Rg[pCallbackÖÌo^ //
	PcmIntProc = PcmFifoProc;

	// àóÔðOPENEDÖÚs //
	statePcm = PCM_OPENED;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int PcmSeq_Close()
//
//  Description: ADPCMV[PT[N[Y
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmSeq_Close()
{
 	//----- Xe[^XªOPENE é¢ÍREADYÅÈ¯êÎG[ðÔ· ------//
	if ((statePcm != PCM_OPENED) && (statePcm != PCM_READY)) {
		return SEQ_ERROR;
	}

	//----- àÏÌú» ------------------------//
	PcmIntProc = 0;
	pcmIrqCtrl = 0;
	infPcm.usePcmCh = 0;     

	//----- MA-2foCXhCoÖÌèÝÖÌí -----------------//
	if (stateFm == FM_NOTEXIST) {
		RemoveMa2IrqHandler(ma2IrqHdl);
		ma2IrqHdl = -1;
	}

	//----- àóÔðNOT EXISTÖÚs -------------------------------------//
	statePcm = PCM_NOTEXIST;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int PcmSeq_Standby(PCMSEQINIDATA *pci, void *prm)
//
//  Description: ADPCMV[PT[Ä¶õ
//  Parameter:
//  					pci		ADPCMV[PTÌúÝèîñði[µ½\¢ÌÖÌ|C^
//  					prm		R[obNÖÖn·øl
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmSeq_Standby(PCMSEQINIDATA *pci, void *prm)
{
	DATABLK BufData;     
	UINT8 timebaseD, timebaseG;

	//-- Xe[^XªOPENEDÅÈ¯êÎG[ðÔ· ----------------------//
	if (statePcm != PCM_OPENED) {
		return SEQ_ERROR;
	}

	//----- R[obNÖÖn·p[^ðL^ ---------------------//
	infPcm.prm = prm;   
	
	//----- pciÌf[^ÅMA2ðú» -----------------------------------//
	//-- ADPCMV[PTÌ^Cx[XÝè -------------//
	timebaseD = CnvTimebase(pci->dTimebaseD);
	timebaseG = CnvTimebase(pci->dTimebaseG);
	Ma2w_adpcm_timebase(timebaseD, timebaseG);
	//-- g`ÌTvOügÝè ------------------//
	MA2Manager_Ma2IrqControl(0);
	ma2Ctrl = MA2C_CTRL_FS;
	if (pci->waveFS ==0)
		Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
	else
		Ma2w_control((UINT8)(Ma2r_control() | ma2Ctrl));
	MA2Manager_Ma2IrqControl(1);

	//-- MA2Ì­¹`lðo^ ---------------------//
	infPcm.usePcmCh = pci->reqAdpcmCh;

	//----- CallbackÖÅStandbyðs¤ ----------------------------//
	PcmIntProc(PCM_STANDBY, 0, &BufData, prm);

	// àóÔðREADYÖÚs //
	statePcm = PCM_READY;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int PcmSeq_Start()
//
//  Description: ADPCMV[PT[X^[g
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmSeq_Start()
{
 	//----- Xe[^XªREADYÅÈ¯êÎG[ðÔ· --------------------//
	if (statePcm != PCM_READY) {
		return SEQ_ERROR;
	}
	
	//----- àóÔðPLAYINGÖÚs ------------------------------------//
	statePcm = PCM_PLAYING;

	//----- èÝ|CgÝè ---------------------------------------//
	pcmIrqCtrl = IRQ_PCM_SEQFIFO_POINT | IRQ_PCM_WAVFIFO_POINT;
	Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);

	//----- èÝEnable ---------------------------------------------//
	pcmIrqCtrl |= (MA2C_ADPCM_IRQES | MA2C_ADPCM_IRQED | MA2C_ADPCM_GENDE);
	Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);

	//----- MA-2 PCM SequencerX^[g ---------------------------------//     
	ma2Ctrl = MA2C_CTRL_ADST;

	MA2Manager_Ma2IrqControl(0);
	Ma2w_control((UINT8)(Ma2r_control() | ma2Ctrl));
	MA2Manager_Ma2IrqControl(1);
	
	return SEQ_SUCCEED;
}


//==========================================================================
//  int PcmSeq_Stop()
//
//  Description: ADPCMV[PT[Xgbv
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmSeq_Stop()
{
 	//----- Xe[^XªPLAYINGÅÈ¯êÎI¹ --------------------------//
	if (statePcm != PCM_PLAYING) {
		return SEQ_SUCCEED;
	}

	//----- ADPCM FIFO èÝDisable ---------------------------------//
	pcmIrqCtrl &= ~(MA2C_ADPCM_IRQES | MA2C_ADPCM_IRQED | MA2C_ADPCM_GENDE); 
	Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);

	//----- MA-2 ADPCM SequencerXgbv -------------------------------//   
	ma2Ctrl = MA2C_CTRL_ADST;
	MA2Manager_Ma2IrqControl(0);
	Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
	MA2Manager_Ma2IrqControl(1);

	//----- ADPCM FIFO clear -------------------------------------------//
	if (stateFm != FM_PLAYING)
		ma2FifoCtrl = MA2C_SQCLR | MA2C_ADCLR | MA2C_FIFO_AFCLR | MA2C_FIFO_DFCLR | MA2C_FIFO_RPT;
	else
		ma2FifoCtrl = MA2C_ADCLR | MA2C_FIFO_AFCLR | MA2C_FIFO_DFCLR | MA2C_FIFO_RPT;
	Ma2w_fifo_control(ma2FifoCtrl);
	
	//----- àóÔðOPENEDÖÚs -------------------------------------//
	statePcm = PCM_OPENED;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int PcmSeq_FifoClear(void)
//
//  Description: PCMV[PTFIFO/g`FIFOÌNA
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmSeq_FifoClear(void)
{
	//----- PCM FIFO clear ----------------------------------------------//
	ma2FifoCtrl = MA2C_FIFO_AFCLR | MA2C_FIFO_DFCLR | MA2C_FIFO_RPT;
		
	Ma2w_fifo_control(ma2FifoCtrl);
	
	return SEQ_SUCCEED;
}


//==========================================================================
//  int PcmSeq_ControlSeek(UINT8 fifoNo, UINT8 *hCtrlData, UINT16 CtrlDataSize)
//
//  Description: PCMV[PTSeekÌRg[bZ[W
//  Parameter:
//  					fifoNo				FIFOÔ
//  					hCtrlData			oµ½Rg[bZ[WÌi[³ê½ÌæÖÌ|C^
//  					CtrlDataSize	oµ½Rg[bZ[WÌi[³ê½ÌæÌTCY
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmSeq_ControlSeek(UINT8 fifoNo, UINT8 *hCtrlData, UINT16 CtrlDataSize)
{
	FIFOINF FifoInf;
	UINT32 writeSize;

	//-- Xe[^XªREADYÅÈ¯êÎG[ðÔ· ----------------------//
	if (statePcm != PCM_READY) {
		return SEQ_ERROR;
	}
	
	if (fifoNo != 0)
		return SEQ_ERROR;
		
	if (CtrlDataSize) {
		FifoInf.hBuffer = hCtrlData;
		FifoInf.dBufSize = (UINT32)CtrlDataSize;
		FifoInf.dBufCount = 0;
		
		while (FifoInf.dBufCount < FifoInf.dBufSize) {
			writeSize = WriteToPcmFifo(infPcm.seqFifoSize, &FifoInf);
			//-- FIFO -----//
			MA2Manager_Ma2IrqControl(0);
			ma2Ctrl = MA2C_CTRL_ADST;
			Ma2w_control((UINT8)(Ma2r_control() | ma2Ctrl));
			MA2Manager_Ma2IrqControl(1);
			//-- FIFOÉ¢ê½f[^ª³êIíéÜÅÒÂ --//
			//-- n[hEFAÌÇÝoµª³µ­Å«ÈêÉ --//
			//-- õ¦ÄA^C}[Éæè³À[vÉ×çÈ¢ --//
			//-- æ¤VXeÉ í¹ÄÎµÄ­¾³¢     --//
			//-- ÈºAáÅ·B                             --//
			
			// I^C}[Zbg

			while (!(Ma2r_seq_fifo_flags() & MA2S_FIFO_AEMP)) {
				if (Ma2r_seq_fifo_status() & MA2S_SQ2_ADERR) {
					return SEQ_ERROR;
				}
			//	if (^C}[ªÝèlð´¦½ç) {
			//		^C}[XgbvG
			//		return SEQ_ERROR;
			//	}

			}

			// ^C}[Xgbv;

			//-- 	FIFOXgbv -------------------//
			MA2Manager_Ma2IrqControl(0);
			ma2Ctrl = MA2C_CTRL_ADST;
			Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
			MA2Manager_Ma2IrqControl(1);
			
			FifoInf.dBufCount += writeSize;
		}
	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int PcmSeq_Seek(UINT8 fifoNo, UINT8 *SeekPoint, UINT32 SeekDuration)
//
//  Description: ADPCMV[PTSeek
//  Parameter:
//  					fifoNo				FIFOÔ
//  					SeekPoint			V[PXf[^ÌSeekÊuFCxgÌæªðw·
//  					SeekDuration	SeekJn©çÅÌCxg­¶ÜÅÌDuration
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmSeq_Seek(UINT8 fifoNo, DATABLK *pBufData, UINT32 SeekDuration)
{
	PFIFOINF hFifoInf;
	DATABLK BufData;

	//-- Xe[^XªREADYÅÈ¯êÎG[ðÔ· ----------------------//
	if (statePcm != PCM_READY) {
		return SEQ_ERROR;
	}
	
	//-- FIFOÔª0ÅÈ¯êÎG[ -----------------------------------//
	if (fifoNo != 0)
		return SEQ_ERROR;
		
	hFifoInf = &(infPcm.seqFifoInf);
	
	hFifoInf->hBuffer = pBufData->pBuf;
	hFifoInf->dBufSize = pBufData->Size;
	hFifoInf->dBufCount = 0;
	
	//-- f[^ðFIFOÉ«Þ --// 
	if (hFifoInf->dBufCount < hFifoInf->dBufSize) { 
		if (SeekDuration != 0xFFFFFFFF) { // æªÖÌSeekÌêDurationÍKvÈ¢ //
			PutPcmDuration(SeekDuration);
			hFifoInf->dBufCount += WriteToPcmFifo(infPcm.seqFifoSize-10, hFifoInf);
		}
		else {
			hFifoInf->dBufCount += WriteToPcmFifo(infPcm.seqFifoSize, hFifoInf);
		}
	}
	
	//----- CallbackÖðÄÑAPCMg`f[^ðàç¢FIFOÖ«Þ //
	// BufDataÉFIFOÖ«Þf[^ÖÌ|C^ÆTCYªwè³êé //
	// TCYª0ÈçÎAf[^ÈµÅ é@@@@@@@@@@@@@@ //
	PcmIntProc(PCMWAV_DATA_EMPTY, 0, &BufData, infPcm.prm);
	hFifoInf = &(infPcm.wavFifoInf);
	hFifoInf->hBuffer = BufData.pBuf;
	hFifoInf->dBufSize = BufData.Size;
	hFifoInf->dBufCount = 0;
	//-- f[^ðFIFOÉ«Þ --//  
	if (hFifoInf->dBufSize != 0) {
		hFifoInf->dBufCount += WriteToWaveFifo(infPcm.wavFifoSize, hFifoInf);
	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Seq_Reset(void)
//
//  Description: MA-2àV[PTÌZbg
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Seq_Reset(void)
{
	//----- FM FIFO clear ----------------------------------------------//
	Ma2w_fifo_control(MA2C_SQCLR);

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Seq_Start()
//
//  Description: FMAADPCMV[PT¯X^[g
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Seq_Start()
{
 	//----- Xe[^XªREADYÅÈ¯êÎG[ðÔ· --------------------//
	if ((stateFm != FM_READY) || (statePcm != PCM_READY)) {
		return SEQ_ERROR;
	}

	//----- àóÔðPLAYINGÖÚs ------------------------------------//
	stateFm = FM_PLAYING;
	statePcm = PCM_PLAYING;

	//----- gpÌFIFOÌtOð§Äé -------------------------------//
	usedFifo = mapFifo;

	//----- èÝ|CgÝè ---------------------------------------//
	fmIrqCtrl = IRQ_FM_FIFO_POINT; 
	Ma2w_fm_irq_ctrl(fmIrqCtrl);
	pcmIrqCtrl = IRQ_PCM_SEQFIFO_POINT | IRQ_PCM_WAVFIFO_POINT;
	Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);

	
	//----- èÝEnable ---------------------------------------------//
	fmIrqCtrl |= (usedFifo << 4);
	Ma2w_fm_irq_ctrl(fmIrqCtrl); 

	pcmIrqCtrl |= (MA2C_ADPCM_IRQES | MA2C_ADPCM_IRQED | MA2C_ADPCM_GENDE);
	Ma2w_adpcm_irq_ctrl(pcmIrqCtrl); 
	
	
	//----- MA-2 FMAPCM SequencerX^[g ---------------------------------// 
	ma2Ctrl = MA2C_CTRL_FMST | MA2C_CTRL_ADST;

	MA2Manager_Ma2IrqControl(0);
	Ma2w_control((UINT8)(Ma2r_control() | ma2Ctrl));
	MA2Manager_Ma2IrqControl(1);

	return SEQ_SUCCEED;
}

//==========================================================================
//  int Seq_Stop()
//
//  Description: FMAADPCMV[PTXgbv
//  Parameter: Èµ
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Seq_Stop()
{                
	UINT8 fmch;
	UINT16 sval;

 	//----- Xe[^XªPLAYINGÅÈ¯êÎI¹ --------------------------//
	if ((stateFm != FM_PLAYING) && (statePcm != PCM_PLAYING))
		return SEQ_SUCCEED;      
		
	//----- FM, ADPCM FIFO èÝDisable ---------------------------------//
	fmIrqCtrl &= ~(MA2C_FM_IRQE0 | MA2C_FM_IRQE1 | MA2C_FM_IRQE2 | MA2C_FM_IRQE3);
	Ma2w_fm_irq_ctrl(fmIrqCtrl); 
	pcmIrqCtrl &= ~(MA2C_ADPCM_IRQES | MA2C_ADPCM_IRQED | MA2C_ADPCM_GENDE);
	Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);  

	//----- MA-2 FMAADPCM SequencerXgbv -------------------------------//   
	ma2Ctrl = MA2C_CTRL_FMST | MA2C_CTRL_ADST;
	MA2Manager_Ma2IrqControl(0);
	Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
	MA2Manager_Ma2IrqControl(1);

	//----- MA-2 FM ­¹Xgbv ---------------------------------------//
	//	­¹Ì¹ð­§Xgbv·é                                    //
	for (fmch=0; fmch<MAX_FM_CHANNEL_NUM; fmch++) {
		if (infFm.useFmCh & (0x0001 << fmch)) {
			sval = Ma2r_fm_control(fmch);
			Ma2w_fm_control(fmch, (UINT16)(sval&0x3ff), (UINT8)((sval&0x1c00)>>10), (UINT8)0);
			Ma2w_fm_channel_vol(fmch, 0x00);
			Ma2w_fm_rr_dr(fmch, 0xff); // Release RateÌÏX //
		}
	}

	//----- FIFO clear ----------------------------------------------//
	ma2FifoCtrl = MA2C_SQCLR | MA2C_FIFO_FFCLR | MA2C_ADCLR | MA2C_FIFO_AFCLR | MA2C_FIFO_DFCLR | MA2C_FIFO_RPT; 
	Ma2w_fifo_control(ma2FifoCtrl);
	
	//----- gpFIFOÌtOð0É·é --------------------------------//
	usedFifo = 0;

	//----- àóÔðOPENEDÖÚs -------------------------------------//
	stateFm = FM_OPENED;
	statePcm = PCM_OPENED;

	return SEQ_SUCCEED;
}



//==========================================================================
//  UINT32 WriteToFmFifo(UINT8 fifoNo, UINT32 fifoSpace, PFIFOINF hFifoInf)
//
//  Description: FMV[PTFIFOÖf[^ð«Þ
//  Parameter: 
//  					fifoNo			FIFOÔ
//  					fifoSpace		FIFOÌ«ÝÂ\ÈTCY(byte)
//  					hFifoInf		«Ýf[^Ìîñi|C^ATCYAÇÝÝÏÝTCYj
//  Return :	«ñ¾TCY(byte)
//==========================================================================
UINT32 WriteToFmFifo(UINT8 fifoNo, UINT32 fifoSpace, PFIFOINF hFifoInf)
{
	UINT32 remSize;
	UINT32 writeSize;
	UINT8 *pwrite;
	UINT8 *peos;
	
	if (hFifoInf->dBufCount < hFifoInf->dBufSize) {
		//-- f[^ª é --//

		// f[^ÌcèTCY //
		remSize = hFifoInf->dBufSize - hFifoInf->dBufCount;

		// f[^Ì«ÝJnÊuÖÌ|C^ //
		pwrite = hFifoInf->hBuffer + hFifoInf->dBufCount;
		
		// «ÝTCYÝè            //
		// End Of SequenceÍ«ÜÈ¢ //
		if (remSize >= fifoSpace+4) {
			writeSize = fifoSpace;
		}
		else {
			peos = hFifoInf->hBuffer+hFifoInf->dBufSize-4;
			if ((*peos==0x00) && (*(peos+1)==0x00) && (*(peos+2)==0x00) && (*(peos+3)==0x00)) {
				writeSize = remSize-4;
			}
			else {
				if (remSize > fifoSpace) {
					writeSize = fifoSpace;
				}
				else {
					writeSize = remSize;
				}
			}
		}

		// f[^«Ý //
		Ma2w_fm_seq_datan(fifoNo, pwrite, (UINT16)writeSize);

		// «ñ¾TCYðÔ· //
		return writeSize;
	}
	
	return 0;
}


//==========================================================================
//  UINT32 WriteToPcmFifo(UINT32 fifoSpace, PFIFOINF hFifoInf)
//
//  Description: ADPCMV[PTFIFOÖf[^ð«Þ
//  Parameter: 
//  					fifoSpace		FIFOÌ«ÝÂ\ÈTCY(byte)
//  					hFifoInf		«Ýf[^Ìîñi|C^ATCYAÇÝÝÏÝTCYj
//  Return :	«ñ¾TCY(byte)
//==========================================================================
UINT32 WriteToPcmFifo(UINT32 fifoSpace, PFIFOINF hFifoInf)
{
	UINT32 remSize;
	UINT32 writeSize;
	UINT8 *pwrite;    
	
	if (hFifoInf->dBufCount < hFifoInf->dBufSize) {
		//-- f[^ª é --//

		// f[^ÌcèTCY //
		remSize = hFifoInf->dBufSize - hFifoInf->dBufCount;

		// f[^Ì«ÝJnÊuÖÌ|C^ //
		pwrite = hFifoInf->hBuffer + hFifoInf->dBufCount;

		// «ÝTCYÝè //
		if (remSize > fifoSpace) {
			writeSize = fifoSpace;
		}
		else {
			writeSize = remSize;
		}

		// f[^«Ý //
		Ma2w_adpcm_seq_datan(pwrite, (UINT16)writeSize);

		// «ñ¾TCYðÔ· //
		return writeSize;
	}
	
	return 0;
}


//==========================================================================
//  UINT32 WriteToWaveFifo(UINT32 fifoSpace, PFIFOINF hFifoInf)
//
//  Description: ADPCMV[PTFIFOÖf[^ð«Þ
//  Parameter: 
//  					fifoSpace		FIFOÌ«ÝÂ\ÈTCY(byte)
//  					hFifoInf		«Ýf[^Ìîñi|C^ATCYAÇÝÝÏÝTCYj
//  Return :	«ñ¾TCY(byte)
//==========================================================================
UINT32 WriteToWaveFifo(UINT32 fifoSpace, PFIFOINF hFifoInf)
{
	UINT32 remSize;
	UINT32 writeSize;
	UINT8 *pwrite;  
	
	if (hFifoInf->dBufCount < hFifoInf->dBufSize) {
		//-- f[^ª é --//

		// f[^ÌcèTCY //
		remSize = hFifoInf->dBufSize - hFifoInf->dBufCount;

		// f[^Ì«ÝJnÊuÖÌ|C^ //
		pwrite = hFifoInf->hBuffer + hFifoInf->dBufCount;

		// «ÝTCYÝè //
		if (remSize > fifoSpace) {
			writeSize = fifoSpace;
		}
		else {
			writeSize = remSize;
		}

		// f[^«Ý //
		Ma2w_adpcm_wave_datan(pwrite, (UINT16)writeSize);
		
		// «ñ¾TCYðÔ· //
		return writeSize;
	}
	
	return 0;
}


/*************************************************************
	Name:		PutDuration
	Function:	
*************************************************************/
static void PutDuration(UINT8 fifoNo, UINT32 t)
{
	if( t<=127 ) {
		Ma2w_fm_seq_data(fifoNo, (UINT8)(t&0x7f));
	}
	else if (t<=16511) {
		Ma2w_fm_seq_data(fifoNo, (UINT8)((((t-128)>>7)&0x7f)|0x80));
		Ma2w_fm_seq_data(fifoNo, (UINT8)((t-128)&0x7f));
	}
	else {
		Ma2w_fm_seq_data(fifoNo, (UINT8)(((16511L-128)>>7)&0x7f)|0x80);
		Ma2w_fm_seq_data(fifoNo, (UINT8)((16511L-128)&0x7f));
		Ma2w_fm_seq_data(fifoNo, 0xFF);
		Ma2w_fm_seq_data(fifoNo, 0x00);
		PutDuration(fifoNo, t-16511L);
	}
}


/*************************************************************
	Name:		PutPcmDuration
	Function:	
*************************************************************/
static void PutPcmDuration(long t)
{
	if( t<=127 ) {
		Ma2w_adpcm_seq_data((UINT8)(t&0x7f));
	}
	else if (t<=16511) {
		Ma2w_adpcm_seq_data((UINT8)((((t-128)>>7)&0x7f)|0x80));
		Ma2w_adpcm_seq_data((UINT8)((t-128)&0x7f));
	}
	else {
		Ma2w_adpcm_seq_data((UINT8)(((16511L-128)>>7)&0x7f)|0x80);
		Ma2w_adpcm_seq_data((UINT8)((16511L-128)&0x7f));
		Ma2w_adpcm_seq_data(0xFF);
		Ma2w_adpcm_seq_data(0x00);
		PutPcmDuration(t-16511L);
	}
}


//==========================================================================
//  int GetFmSeqState()
//
//  Description: FMV[PTÌàóÔðÔ·
//  Parameter: Èµ
//  Return :	FMV[PTÌàóÔ
//==========================================================================
int GetFmSeqState()
{
	return stateFm;
}

//==========================================================================
//  int GetPcmSeqState()
//
//  Description: PCMV[PTÌàóÔðÔ·
//  Parameter: Èµ
//  Return :	PCMV[PTÌàóÔ
//==========================================================================
int GetPcmSeqState()
{
	return statePcm;
}

//==========================================================================
//  void Seq_Irq_Proc()
//
//  Description: MA2ÌèÝª­¶µ½êÌ
//  Parameter:
//  Return : Èµ
//==========================================================================
void Seq_Irq_Proc(void)
{                
	UINT16 fifoStat;
	UINT8 fifoStat1, fifoStat2;   
	UINT8 wavFifoStat;
	UINT16 irqType=1;
	DATABLK BufData; 
	PFIFOINF hFifoInf;
	UINT8 fifono;       
	UINT8 fmch;
	UINT16 sval;
	UINT32 fifoSpace;
	UINT32 writeSize;
	
	//----- MA2ÌèÝXe[^XðÇÝo· --------------------------//
	fifoStat = Ma2r_seq_fifo_status();
	fifoStat1 = (UINT8)(fifoStat >> 8);
	fifoStat2 = (UINT8)(fifoStat & 0xFF);
	wavFifoStat = Ma2r_wave_fifo_status();   

	//----- ½ÌèÝÅ é©²¸ -----------------------------------//
	irqType = 0;
	//-- FMV[PT ----------------------------------//
	if (infFm.useFmCh) {
		if (fmIrqCtrl & MA2C_FM_IRQE) {
			if (fifoStat2 & MA2S_SQ2_FMERR)
				irqType |= IRQ_FM_ERR;
			else {
				if (fifoStat1 & MA2S_SQ1_FMEND) 
					irqType |= IRQ_FM_END;
				if ((fifoStat1 & MA2S_SQ1_FLG) & ((fmIrqCtrl & MA2C_FM_IRQE) >> 4)) {
					if (fmIrqCtrl & MA2C_FM_IRQP)
						irqType |= IRQ_FM_DATA;
					else
						irqType |= IRQ_FM_EMPTY;
				}
			}
		}
	}
	
	if (infPcm.usePcmCh) {
		//-- ADPCMV[PT -------------------------------//
		if (pcmIrqCtrl & MA2C_ADPCM_IRQES) {
			if (fifoStat2 & MA2S_SQ2_ADERR)
				irqType |= IRQ_PCMSEQ_ERR;
			else if (fifoStat2 & MA2S_SQ2_SEND)
				irqType |= IRQ_PCMSEQ_END;
			else if (fifoStat1 & MA2S_SQ1_AFLG) {
				if (pcmIrqCtrl & MA2C_ADPCM_IRQPS)
					irqType |= IRQ_PCMSEQ_DATA;
				else 
					irqType |= IRQ_PCMSEQ_EMPTY;
			}
		}

		//-- ADPCMg`f[^ -------------------------------//
		if ((pcmIrqCtrl & MA2C_ADPCM_GENDE) && (fifoStat2 & MA2S_SQ2_GEND)) {
			irqType |= IRQ_PCMWAV_END;
		}
		else if ((pcmIrqCtrl & MA2C_ADPCM_IRQED) && (wavFifoStat & MA2S_WAVE_DFLG)) {
			if (pcmIrqCtrl & MA2C_ADPCM_IRQPD)
				irqType |= IRQ_PCMWAV_DATA;
			else
				irqType |= IRQ_PCMWAV_EMPTY;
		}
	}

	//===== FMV[PTFIFOG[ =====================================//

	if (irqType & IRQ_FM_ERR) {

		//-- FMV[PTXgbv ----------------------//
		ma2Ctrl = MA2C_CTRL_FMST; 
		Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
		usedFifo = 0;
		fmIrqCtrl = 0;
		Ma2w_fm_irq_ctrl(0);

		//-- MA-2 FM ­¹Xgbv ------------------------//
		//	­¹Ì¹ð­§Xgbv·é                  //
		for (fmch=0; fmch<MAX_FM_CHANNEL_NUM; fmch++) {
			if (infFm.useFmCh & (0x0001 << fmch)) {
				sval = Ma2r_fm_control(fmch);
				Ma2w_fm_control(fmch, (UINT16)(sval&0x3ff), (UINT8)((sval&0x1c00)>>10), (UINT8)0);
			}
		}
		
		Ma2w_fifo_control(MA2C_FIFO_FFCLR);

		// àóÔðOPENEDÖÚs //
		stateFm = FM_OPENED;
		
		// È~ÌFMèÝðð //
		irqType &= 0xFFF0;

		//-- ·×ÄÌFIFOÉÂ¢ÄI¹ðA ------------//
		for (fifono=0; fifono<MAX_FIFO_NUM; fifono++) {
			if (usedFifo & (0x01 << fifono)) {
				FmIntProc(FMSEQ_DATA_EMPTY, fifono, &BufData, infFm.prm);
			}
		}          
	}
		                      
	//===== FMV[PXf[^I¹ =====================================//
	if (irqType & IRQ_FM_END) {

		//-- ·×ÄÌgpFM FIFOÉÂ¢ÄENDðð`FbN --//
		for (fifono=0; fifono<MAX_FIFO_NUM; fifono++) {
			if ((usedFifo & (0x01<<fifono)) && (fifoStat2 & (MA2S_SQ2_END0<<fifono))) {

				//-- ENDÆÈÁ½FIFOÌèÝDisable ------//
				fmIrqCtrl &= ~(MA2C_FM_IRQE0<<fifono);
				Ma2w_fm_irq_ctrl(fmIrqCtrl);

				//-- ENDÆÈÁ½FIFOÌgptOð0É·é --//
				usedFifo &= ~(0x01<<fifono);

				//-- ·×ÄÌFIFOªI¹ÈçFMV[PTXgbv --//
				if (!usedFifo) {
					ma2Ctrl = MA2C_CTRL_FMST; 
					Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
					fmIrqCtrl = 0;
					Ma2w_fm_irq_ctrl(fmIrqCtrl);

					//----- MA-2 FM ­¹Xgbv ---------------------------------------//
					//	­¹Ì¹ð­§Xgbv·é                                    //
					for (fmch=0; fmch<MAX_FM_CHANNEL_NUM; fmch++) {
						if (infFm.useFmCh & (0x0001 << fmch)) {
							sval = Ma2r_fm_control(fmch);
							Ma2w_fm_control(fmch, (UINT16)(sval&0x3ff), (UINT8)((sval&0x1c00)>>10), (UINT8)0);
						}
					}

					Ma2w_fifo_control(MA2C_FIFO_FFCLR);

					//-- àóÔðREADYÖÚs --------------//
					stateFm = FM_READY;
					
					// È~ÌFMèÝðð //
					irqType &= 0xFFF0;

					//-- FMV[PXf[^I¹ðA --//
					FmIntProc(FMSEQ_DATA_EMPTY, fifono, &BufData, infFm.prm);
				
					//-- for[v©ç²¯é --//
					break;
				}

				//-- FMV[PXf[^I¹ðA --//
				FmIntProc(FMSEQ_DATA_EMPTY, fifono, &BufData, infFm.prm);
			}
		}                   
	}
		
	//===== FMV[PXFIFO èÝ|Cg == EMPTY =================//
	if (irqType & IRQ_FM_EMPTY) {         

		//-- ·×ÄÌgpFM FIFOÉÂ¢ÄèÝð`FbN --//
		for (fifono=0; fifono<MAX_FIFO_NUM; fifono++) {
			if ((usedFifo & (0x01<<fifono)) && (fifoStat1 & (MA2S_SQ1_FLG0<<fifono))) {

				//-- EmptyÆÈÁ½FIFOÌèÝðDisable --//
				fmIrqCtrl &= ~(MA2C_FM_IRQE0<<fifono);
				Ma2w_fm_irq_ctrl(fmIrqCtrl);

				//-- EmptyÆÈÁ½FIFOÌgptOð0É·é --//
				usedFifo &= ~(0x01<<fifono);

				//-- ·×ÄÌFIFOªEmptyÈçFMV[PTXgbv --//
				if (!usedFifo) {
					ma2Ctrl = MA2C_CTRL_FMST;
					Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));
					fmIrqCtrl = 0;
					Ma2w_fm_irq_ctrl(fmIrqCtrl);

					//----- MA-2 FM ­¹Xgbv ---------------------------------------//
					//	­¹Ì¹ð­§Xgbv·é                                    //
					for (fmch=0; fmch<MAX_FM_CHANNEL_NUM; fmch++) {
						if (infFm.useFmCh & (0x0001 << fmch)) {
							sval = Ma2r_fm_control(fmch);
							Ma2w_fm_control(fmch, (UINT16)(sval&0x3ff), (UINT8)((sval&0x1c00)>>10), (UINT8)0);
						}
					}

					Ma2w_fifo_control(MA2C_FIFO_FFCLR);

					//-- àóÔðREADYÖÚs ------------//
					stateFm = FM_READY;
					
					irqType &= 0xFFF0;

					//-- FMV[PXf[^I¹ðA --//
					FmIntProc(FMSEQ_DATA_EMPTY, fifono, &BufData, infFm.prm);
				
					//-- for[v©ç²¯é --//
					break;
				}

				//-- FMV[PXf[^I¹ðA --//
				FmIntProc(FMSEQ_DATA_EMPTY, fifono, &BufData, infFm.prm);
			}
		}                
	}

	//===== FMV[PXFIFOèÝ|Cg f[^«Ý ============//
	// ÌV[PXf[^ðFIFOÉ«Þ                         //
	if (irqType & IRQ_FM_DATA) {

		//-- ·×ÄÌFIFOð`FbN ------------------//    
		for (fifono=0; fifono<MAX_FIFO_NUM; fifono++) {        
			fifoSpace = infFm.irqSize;

			//-- èÝÝèµÄ¢È¢FIFOÍÆÎ· ----//
			if (!(fmIrqCtrl & (MA2C_FM_IRQE0<<fifono)))
				continue;

			//-- FIFOèÝ|CgÉBµÄ¢½çFIFOÉf[^«Ý --//
			if (fifoStat1 & (MA2S_SQ1_FLG0<<fifono)) { 

				hFifoInf = &(infFm.fifoInf[fifono]);
				//-- FIFOÖÌ«Ý --------------------//
				writeSize = WriteToFmFifo(fifono, fifoSpace, hFifoInf);
				hFifoInf->dBufCount += writeSize;
				fifoSpace -= writeSize;
				if (fifoSpace) {
					//-- f[^ªI¹µ½½ßAVµ¢f[^ðv·é --//
					FmIntProc(FMSEQ_DATA_REQUEST, fifono, &BufData, infFm.prm);
					if (BufData.Size == 0) {
						if (fifoSpace >= 4) {
							//-- Vµ¢f[^Èµ èÝðDisable --//
							fmIrqCtrl &= ~(MA2C_FM_IRQE0<<fifono);
							Ma2w_fm_irq_ctrl(fmIrqCtrl);

							//-- End Of Sequence}ü --//
							Ma2w_fm_seq_data(fifono, 0x00);
							Ma2w_fm_seq_data(fifono, 0x00);
							Ma2w_fm_seq_data(fifono, 0x00);
							Ma2w_fm_seq_data(fifono, 0x00);
						}
					}
					else {
						//-- FIFOÖÌ«Ý --------------------//
						hFifoInf->hBuffer = BufData.pBuf;
						hFifoInf->dBufSize = BufData.Size;  
						hFifoInf->dBufCount = 0;
						hFifoInf->dBufCount += WriteToFmFifo(fifono, fifoSpace, hFifoInf);
					}
				}
			}
		} // for
				
		//-- ·×ÄÌFIFOÌèÝªDisable¾Á½çèÝ|CgðEmptyÉ --//
		// ·×ÄÌFIFOÌÉ«Þf[^ªI¹µ½Æ¢¤±Æ                 //
		if (!(fmIrqCtrl & MA2C_FM_IRQE)) {
			fmIrqCtrl = (UINT8)((usedFifo << 4) & ~MA2C_FM_IRQP);
			Ma2w_fm_irq_ctrl(fmIrqCtrl);
		}
	}
  

 
	//===== ADPCMV[PXFIFOG[ ==================================//
  if (irqType & IRQ_PCMSEQ_ERR) {

		//-- ADPCMV[PTXgbv ---------------------//
		pcmIrqCtrl = 0;
		Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);

		ma2Ctrl = MA2C_CTRL_ADST;
		Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));

		Ma2w_fifo_control(MA2C_ADCLR | MA2C_FIFO_DFCLR | MA2C_FIFO_AFCLR);

		//-- àóÔðOPENEDÖÚs ----------------------//
		statePcm = PCM_OPENED;
		
		// È~ÌADPCMèÝðð //
		irqType &= 0x000F;

		//-- V[PTI¹ðA ------------------------//
		PcmIntProc(PCMSEQ_DATA_EMPTY, 0, &BufData, infPcm.prm);
	}
	
	//===== ADPCMV[PXFIFOI¹ ====================================//
	if (irqType & IRQ_PCMSEQ_END) {

		//-- ADPCMV[PTXgbv ---------------------//
		pcmIrqCtrl = 0;
		Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);

		ma2Ctrl = MA2C_CTRL_ADST;
		Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));

		Ma2w_fifo_control(MA2C_ADCLR | MA2C_FIFO_DFCLR | MA2C_FIFO_AFCLR);

		//-- àóÔðREADYÖÚs ----------------------//
		statePcm = PCM_READY;
		
		irqType &= 0x000F;
		
		//-- V[PTI¹ðA ------------------------//
		PcmIntProc(PCMSEQ_DATA_EMPTY, 0, &BufData, infPcm.prm);
	}
	
	
	//===== ADPCMV[PXFIFO èÝ|Cg == EMPTY =================//
	if (irqType & IRQ_PCMSEQ_EMPTY) {
		//----- ADPCMV[PTFIFO = empty ----------//

		//-- V[PTXgbv ----------------------//
		pcmIrqCtrl = 0;
		Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);

		ma2Ctrl = MA2C_CTRL_ADST;
		Ma2w_control((UINT8)(Ma2r_control() & ~ma2Ctrl));

		Ma2w_fifo_control(MA2C_ADCLR | MA2C_FIFO_DFCLR | MA2C_FIFO_AFCLR);

			
		//-- àóÔðREADYÖÚs ------------------//
		statePcm = PCM_READY;
		
		// È~ÌADPCMèÝðð //
		irqType &= 0x000F;

		//-- V[PXFIFOI¹ðA ----------------//
		PcmIntProc(PCMSEQ_DATA_EMPTY, 0, &BufData, infPcm.prm);

	}
	
	//===== ADPCMV[PTFIFOèÝ|Cg f[^«Ý =========//
	if (irqType & IRQ_PCMSEQ_DATA) {

		hFifoInf = &(infPcm.seqFifoInf);
		fifoSpace = infPcm.seqIrqSize;
		//-- FIFOÖÌf[^«Ý ------------------//
		writeSize = WriteToPcmFifo(fifoSpace, hFifoInf);
		hFifoInf->dBufCount += writeSize;
		fifoSpace -= writeSize;
		if (fifoSpace) {
			//-- f[^ªI¹µ½½ßAVµ¢f[^ðv·é --//
			PcmIntProc(PCMSEQ_DATA_REQUEST, 0, &BufData, infPcm.prm);
			//-- Ìf[^ª êÎFIFOÖÌf[^«Ý ------------------//
			if (hFifoInf->dBufSize != 0) {
				hFifoInf->hBuffer = BufData.pBuf;
				hFifoInf->dBufSize = BufData.Size;  
				hFifoInf->dBufCount = 0;    
				writeSize = WriteToPcmFifo(fifoSpace, hFifoInf);
				hFifoInf->dBufCount += writeSize;
			}
			else {
				//-- Vµ¢f[^Èµ èÝ|CgðEmptyÖÏX --//
				Ma2w_adpcm_seq_data(0x00);
				Ma2w_adpcm_seq_data(0x00);
				Ma2w_adpcm_seq_data(0x00);
				Ma2w_adpcm_seq_data(0x00);
				pcmIrqCtrl &= ~MA2C_ADPCM_IRQPS;
				Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);
			}
		}
	}
	
	
	//===== ADPCM Gatetime I¹ ========================================//
	if (irqType & IRQ_PCMWAV_END) {

		//-- Gatetime end flagðð ---------------------//
		Ma2_write_reg(MA2_REGISTER_SW, 0);
		Ma2_write_reg(MA2_SEQ_FIFO_STAT2, MA2S_SQ2_GEND);
		
		//-- Ìg`f[^ðv ---------------------//
		PcmIntProc(GATE_TIME_UP, 0, &BufData, infPcm.prm);
		hFifoInf = &(infPcm.wavFifoInf);
		hFifoInf->hBuffer = BufData.pBuf;
		hFifoInf->dBufSize = BufData.Size;
		hFifoInf->dBufCount = 0;

		//-- Ìg`f[^ª êÎFIFOÖ«Ý ------//
		if (hFifoInf->dBufSize != 0) {
			hFifoInf->dBufCount += WriteToWaveFifo(infPcm.wavFifoSize, hFifoInf);
			pcmIrqCtrl |= (MA2C_ADPCM_IRQED | IRQ_PCM_WAVFIFO_POINT);
			Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);
		}
	}
		
	//===== ADPCMg`f[^FIFO èÝ|Cg==EMPTY ================//
	if (irqType & IRQ_PCMWAV_EMPTY) {

		//-- GatetimeI¹Ò¿ ----------------------------//
		pcmIrqCtrl &=~ MA2C_ADPCM_IRQED;
		Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);
		
	}

	//===== ADPCMg`f[^FIFOèÝ|Cg f[^«Ý =========//
	if (irqType & IRQ_PCMWAV_DATA) {

		hFifoInf = &(infPcm.wavFifoInf);
		fifoSpace = infPcm.wavIrqSize;
		//-- FIFOÖÌf[^«Ý ------------------//
		writeSize = WriteToWaveFifo(fifoSpace, hFifoInf);
		hFifoInf->dBufCount += writeSize;
		fifoSpace -= writeSize;
		if (fifoSpace) {
			//-- f[^ªI¹µ½½ßAVµ¢f[^ðv·é --//
			PcmIntProc(PCMWAV_DATA_REQUEST, 0, &BufData, infPcm.prm);
			hFifoInf->hBuffer = BufData.pBuf;
			hFifoInf->dBufSize = BufData.Size;
			hFifoInf->dBufCount = 0;
			//-- Vµ¢f[^ª êÎFIFOÖÌf[^«Ý ------------------//
			if (hFifoInf->dBufSize != 0) {
				hFifoInf->dBufCount += WriteToWaveFifo(fifoSpace, hFifoInf);
			}
			else {
				//-- Vµ¢f[^Èµ èÝ|CgðEmptyÖÏX --//
				pcmIrqCtrl &= ~MA2C_ADPCM_IRQPD;
				Ma2w_adpcm_irq_ctrl(pcmIrqCtrl);
			}
		}
	}
	
}

//Ma2seq.c
//smafchecker.c

/*********************************************************************
		Name:		_getb
		Function:	obt@©çPbyteæèo·
*********************************************************************/
static UINT8 _getb(void)
{
	return *pSmafBuf++;
}

/*********************************************************************
		Name:		_getw
		Function:	obt@©çPword(2byte)æèo·
*********************************************************************/
static UINT16 _getw(void)
{                                                 
UINT16  d16;
	d16 =  (UINT16)((((UINT16)*pSmafBuf)<<8) | ((UINT16)*(pSmafBuf+1)));
	pSmafBuf += 2;
	return d16;
}

/*********************************************************************
		Name:		_getdw
		Function:	obt@©çPdword(4byte)æèo·
*********************************************************************/
static UINT32 _getdw(void)
{
UINT32 d32;
	d32 = ((UINT32)*pSmafBuf<<24) | ((UINT32)*(pSmafBuf+1)<<16) |
				((UINT32)*(pSmafBuf+2)<<8) | (UINT32)*(pSmafBuf+3);
	pSmafBuf += 4;
	return d32;
}

/*********************************************************************
		Name:		_get_shortmodulation
		Function:	Modulation 3bytef[^©ç2bytef[^lðßé
*********************************************************************/
static UINT8 _get_shortmodulation( UINT8 value )
{
	if (value == 0x00) return 0x01;
	if (value <= 0x08) return 0x02;
	if (value <= 0x10) return 0x03;
	if (value <= 0x18) return 0x04;
	if (value <= 0x20) return 0x05;
	if (value <= 0x28) return 0x06;
	if (value <= 0x30) return 0x07;
	if (value <= 0x38) return 0x08;
	if (value <= 0x40) return 0x09;
	if (value <= 0x48) return 0x0a;
	if (value <= 0x50) return 0x0b;
	if (value <= 0x60) return 0x0c;
	if (value <= 0x70) return 0x0d;
	if (value <= 0x7f) return 0x0e;
	
	return 0x0e;
}

/*********************************************************************
		Name:		_get_timebase
		Function:	indexÉÎ·égatetimelðÔ·
*********************************************************************/
static UINT16 _get_timebase( UINT8 index )
{
UINT16 _timebase;

	switch( index ) {
		case 0x00:	_timebase = 1;	break;
		case 0x01:	_timebase = 2;	break;
		case 0x02:	_timebase = 4;	break;
		case 0x03:	_timebase = 5;	break;
		case 0x10:	_timebase = 10;	break;
		case 0x11:	_timebase = 20;	break;
		case 0x12:	_timebase = 40;	break;
		case 0x13:	_timebase = 50;	break;
		default:		_timebase = 0;	// ERROR
	}
	return _timebase;
}


/*************************************************************
	Name:		makeCRC
	Function:	CRCðvZ·é
*************************************************************/

static unsigned short makeCRC(int n, UINT8 *c)
{
  UINT16 res;
  UINT16 res1;
  UINT16 res2;
  UINT16 crc;
  UINT8 data0;

  res = 0xFFFFU;
  res1 = 0xFFFFU;
  res2 = 0xFFFFU;
  crc = 0;
  // 
  while( --n >= 2 ){
    data0 = *c++;
    res2 = res1;
    res1 = res;
    res = (UINT16)((res << 8) ^ crctable[(UINT8)(res >> 8) ^ data0]);
    crc = (UINT16)((crc << 8) | data0);
  }
  // Compare CRC
  //return (UINT16)(~res & 0xFFFFU); 
  return 10;
}


/*********************************************************************

		Name:		SmafGetOptionData
		Function:	Contents Info ChunkÌOption©çAwèÌ^uÌf[^ð
							æ¾·éB
		Input :		UINT16 	reqTag	... f[^ðæèo·^O
					UINT8 **pData	... f[^ªi[³êÄ¢éÌæÖÌ|C^
														ðÔ·ÌæÖÌ|C^
					UINT32 *pDataSize	... f[^ÌTCYði[·éÌæÖÌ|C^
																0ÌêÍwèÌ^OªÈ¢±ÆðÓ¡·é
		Output :

*********************************************************************/
bool GetSmafOptionData(UINT16 reqTag, UINT8 **pData, UINT8 *pDataSize)
{
	UINT8 *pOption;
	UINT16 tag;
	UINT8 cdata;
	
	pOption = SmafInfo.pOption;
	*pData = NULL;
	*pDataSize = 0;
	
	while (pOption < SmafInfo.pOption + SmafInfo.dwOptionSize) {
		//-- TagÇÝoµ ----------//
		tag = (UINT16)((((UINT16)*pOption) << 8) | ((UINT16)*(pOption+1)));
		pOption += 2;
		if (tag == reqTag) { // wè³ê½TagÆêv //
			cdata = *(pOption++);
			if (cdata == 0x3A) {
				*pData = pOption;
				while (*(pOption++) != 0x2C) { // ","ÜÅÌf[^TCYð¦é //
					(*pDataSize)++;
					if (pOption >= SmafInfo.pOption + SmafInfo.dwOptionSize)
						break;
					if (*(pOption) == 0x5C) { // "\"ÍGXP[vLN^ //
						pOption++;
						(*pDataSize)++;
						if ((*(pOption) == 0x2C) || (*(pOption) == 0x5C)) {
							pOption++;
							(*pDataSize)++;
						}
					}
				}
				return true;
			}
		}
		else {
			while (*(pOption++) != 0x2C) { // ","ÜÅÇÝòÎ· //
				if (pOption >= SmafInfo.pOption + SmafInfo.dwOptionSize)
					break;
				if (*(pOption) == 0x5C) { // "\"ÍGXP[vLN^ //
					pOption++;
					if ((*(pOption) == 0x2C) || (*(pOption) == 0x5C)) {
						pOption++;
					}
				}
			}
		}
	}
	
	return true;
}



/*********************************************************************

		Name:		SmafScoreTrackSeek
		Function:	Score TrackÌSequence DataðèÌÊuÜÅSeek·é
		Input :		UINT8  bScoreTrack	... Score Track No.
					UINT32 dwSeekAbs	... SeekPointÜÅÌPÊÔ
					UINT8 **pSeekPoint	... SeekÀsãÅÌEventÜ½Í
											Control MessageÌÊu
					UINT32 *dwSeekDuration ... SeekãÅÌEventÜ½Í
											Control MessageÜÅÌDuration
		Output :

*********************************************************************/
bool SmafScoreTrackSeek(UINT8 bScoreTrack, UINT32 dwSeekAbs,
							  UINT8 **pSeekPoint, UINT32 *dwSeekDuration)
{
	UINT8  *pSrc;
	UINT32  dwSrcDataSize;
	UINT8	bData;
	UINT8	bData0;
	UINT8	bData1;
	UINT8	bData2;
	UINT16  wDuration;
	UINT8   iMessageSize, i;
	
	if (!(ScoreTrackInfo[bScoreTrack].bSequenceType)) {
		pSrc = ScoreTrackInfo[bScoreTrack].pSequenceDataChunk
					+ ScoreTrackInfo[bScoreTrack].SeekPhraseInfo.dwStartPoint;
		dwSrcDataSize = ScoreTrackInfo[bScoreTrack].dwSequenceDataChunkSize
									- ScoreTrackInfo[bScoreTrack].SeekPhraseInfo.dwStartPoint;
	}
	else {
		pSrc = ScoreTrackInfo[bScoreTrack].pSequenceDataChunk;
		dwSrcDataSize = ScoreTrackInfo[bScoreTrack].dwSequenceDataChunkSize;
	}

	if(dwSrcDataSize==0 ) return false;

	dwAbsTime = 0;	
	for ( ; ; )  {
		if ( dwSrcDataSize < 3 ||
				((*pSrc==0)&&(*(pSrc+1)==0)&&(*(pSrc+2)==0)&&(*(pSrc+3)==0)) ) {
			*pSeekPoint = pSrc;
			*dwSeekDuration = 0;
			return true;
		}
		else  {
			//	Duration 1 or 2byte
			bData = *pSrc++;
			wDuration = (UINT16)bData;
			dwSrcDataSize--;			
			if ( wDuration & 0x80 ) {
				bData = *pSrc++;
				wDuration = (UINT16)(( (wDuration & 0x7F) << 7 ) + (UINT16)bData + 128);
				dwSrcDataSize--;
			}
			dwAbsTime += (UINT32)wDuration;
			
		/////////////////////////////////////////////////////////////
		
			if( dwAbsTime>=dwSeekAbs ) {
				*pSeekPoint = pSrc;
				*dwSeekDuration = dwAbsTime - dwSeekAbs;
				return true;
			}
		///////////////////////////////////////////////////////////

			//	Event
			bData0 = *pSrc++;
			dwSrcDataSize--;

			switch ( bData0 )  {
			case 0x00:
				// Control Message

				bData1 = *pSrc++;
				dwSrcDataSize--;

				if ( ( bData1 & 0x30 ) != 0x30 ) {			// 2byte
				}
				else {								// 3byte
					bData2 = *pSrc++;
					dwSrcDataSize--;
				}
				break;

			case 0xFF:

				// Exclusive Message or NOP
				bData1 = *pSrc++;
				dwSrcDataSize--;

				if ( bData1 == 0xF0 ) {				// Exclusive Message
					iMessageSize = *pSrc++;
					dwSrcDataSize--;

					for ( i=0 ; i < iMessageSize; i++ ) {
						bData = *pSrc++;
						dwSrcDataSize--;
					}
				}
				else {								// NOP
				}
				break;

			default:
				// Note Message

				bData = *pSrc++;				// Gatetime 1st byte
				dwSrcDataSize--;

				if ( bData & 0x80 )  {
					bData = *pSrc++;			// Gatetime 2nd byte
					dwSrcDataSize--;
				}
				break;
			}
		}
	}
}

/*********************************************************************

		Name:		SmafScoreTrackControlSeek
		Function:	SeekPointÜÅÌControl DataîñðûW·éB
		Input :		UINT8  bScoreTrack
					UINT8  *pSeekPoint
					UINT8  *pControl
					UINT8  bCtrl   0:[vÉKvÈîñÌÝ
					               1:SÄÌîñ
		Output :	UINT16				 Num of Control Data

*********************************************************************/
UINT16 SmafScoreTrackControlSeek(UINT8 bScoreTrack, UINT8 *pSeekPoint,
												 UINT8 *pControl, UINT8 bCtrl)
{
	UINT8 *pSrc;
	UINT8	bData;
	UINT8	bData0;
	UINT8	bData1;
	UINT8	bData2;
	UINT16  wDuration;
	UINT8   iMessageSize, i;
	UINT16  wNumControl;
	UINT8 chno;   
	UINT8 index, asch;
	
	struct CONTROL_EVENT ctrlEvent[4];	
	UINT8 exMssg[8][2];
	
	wNumControl = 0;
	
	pSrc = ScoreTrackInfo[bScoreTrack].pSequenceDataChunk;
	
	//-- \¢ÌÌú» --------------------------------//
	for (chno=0; chno<4; chno++) {
		ctrlEvent[chno].ProgramChange = 0x00;
		ctrlEvent[chno].BankSelect = 0x00;
		ctrlEvent[chno].OctaveShift = 0x00;
		ctrlEvent[chno].Modulation = 0x01;
		ctrlEvent[chno].PitchBend = 0x40;
		ctrlEvent[chno].Volume = 0x63;	// MA-2 ftHgl
		ctrlEvent[chno].Panpot = 0x40;
		ctrlEvent[chno].Expression = 0x7f;
	}
	
	for (chno=0; chno<8; chno++) {
		exMssg[chno][0] = 0;
		exMssg[chno][1] = 0;
	}

	if ( pSrc < pSeekPoint) {

		for ( ; ; )  {
			//	Duration 1 or 2byte
			bData = *pSrc++;
			wDuration = (UINT16)bData;
			if ( wDuration & 0x80 ) {
				bData = *pSrc++;
				wDuration = (UINT16)(( (wDuration & 0x7F) << 7 ) + (UINT16)bData + 128);
			}

			/////////////////////////////////////////////////////////////
		
			if ( pSrc>=pSeekPoint ) break;

			///////////////////////////////////////////////////////////

			//	Event
			bData0 = *pSrc++;

			switch ( bData0 )  {
			case 0x00:
				// Control Message
                                 
				bData1 = *pSrc++;

				if ( ( bData1 & 0x30 ) != 0x30 ) {			// 2byte
					chno = (UINT8)((bData1 & 0xc0) >> 6);

					switch ( (bData1 & 0x30) >> 4 )  {
					case 0x0:						// Expression
						ctrlEvent[chno].Expression = expression[bData1 & 0x0f];
						break;
					case 0x1:						// Pitch Bend
						ctrlEvent[chno].PitchBend = pitchbend[bData1 & 0x0f];
						break;
					case 0x2:						// Modulation
						ctrlEvent[chno].Modulation = (UINT8)(bData1 & 0x0f);
						break;
					default:						// reserved
						break;
					}

				}
				else {								// 3byte
					bData2 = *pSrc++;
					chno = (UINT8)((bData1 & 0xc0) >> 6);

					switch ( bData1 & 0xF )  {
					case 0x0:						// Program Change
						ctrlEvent[chno].ProgramChange = bData2;
						break;
					case 0x1:						// Bank Select
						ctrlEvent[chno].BankSelect = bData2;
						break;
					case 0x2:						// Octave Shift
						ctrlEvent[chno].OctaveShift = bData2;
						break;
					case 0x3:						// Modulation
						ctrlEvent[chno].Modulation = _get_shortmodulation(bData2);
						break;
					case 0x4:						// Pitch Bend
						ctrlEvent[chno].PitchBend = bData2;
						break;
					case 0x7:						// Volume
						ctrlEvent[chno].Volume = bData2;
						break;
					case 0xA:						// Pan
						ctrlEvent[chno].Panpot = bData2;
						break;
					case 0xB:						// Expression
						ctrlEvent[chno].Expression = bData2;
						break;
					default:
						break;
					}

				}
				break;

			case 0xFF:

				// Exclusive Message or NOP
				bData1 = *pSrc++;

				if ( bData1 == 0xF0 ) {				// Exclusive Message
					iMessageSize = *pSrc++;
					if ((*pSrc == 0x43) && (*(pSrc+1) == 0x03)) { // Yamaha YMU759ID
						if (*(pSrc+2) == 0x91) {
							index = *(pSrc+3);
							iMessageSize--;
							if ((index >= 0x18) && (index <= 0x1F)) {
								asch = (UINT8)(index - 0x18);
								exMssg[asch][0] = 1;
								exMssg[asch][1] = *(pSrc+4);
							}
						}
					}
					for ( i=0 ; i < iMessageSize; i++ ) {
						bData = *pSrc++;
					}
				}
				else {								// NOP
				}
				break;

			default:
				// Note Message

				bData = *pSrc++;				// Gatetime 1st byte

				if ( bData & 0x80 )  {
					bData = *pSrc++;			// Gatetime 2nd byte
				}
				break;
			}
		}
	
	}
	
	for (chno=0; chno<4; chno++) {
		if (bCtrl) {	// bCtrl==0ÌêÍOctave ShiftAModulationÌÝ //
/*			//-- Bank Select (MA-2ñÎ) --//
			*pControl++ = 0x00;	// Duratio 1 //
			*pControl++ = 0x00;	// Control Messege //
			*pControl++ = (chno << 6) | 0x31;
			*pControl++ = ctrlEvent[chno].BankSelect;
*/		
			//-- Program Change --//
			*pControl++ = 0x00;	// Duratio 0 //
			*pControl++ = 0x00;	// Control Messege //
			*pControl++ = (UINT8)((chno << 6) | 0x30);
			*pControl++ = ctrlEvent[chno].ProgramChange;

/*			//-- Pitch Bend (MA-2ñÎ) --//
			*pControl++ = 0x00;	// Duratio 0 //
			*pControl++ = 0x00;	// Control Messege //
			*pControl++ = (UINT8)((chno << 6) | 0x34));
			*pControl++ = ctrlEvent[chno].PitchBend;
*/		
			//-- Volume --//
			*pControl++ = 0x00;	// Duratio 0 //
			*pControl++ = 0x00;	// Control Messege //
			*pControl++ = (UINT8)((chno << 6) | 0x37);
			*pControl++ = ctrlEvent[chno].Volume;
		
			//-- Panpot --//
			*pControl++ = 0x00;	// Duratio 0 //
			*pControl++ = 0x00;	// Control Messege //
			*pControl++ = (UINT8)((chno << 6) | 0x3a);
			*pControl++ = ctrlEvent[chno].Panpot;
		
			//-- Expression --//
			*pControl++ = 0x00;	// Duratio 0 //
			*pControl++ = 0x00;	// Control Messege //
			*pControl++ = (UINT8)((chno << 6) | 0x3b);
			*pControl++ = ctrlEvent[chno].Expression;
		}
		
		//-- Octave Shift --//
		*pControl++ = 0x00;	// Control Messege //
		*pControl++ = 0x00;	// Duratio 0 //
		*pControl++ = (UINT8)((chno << 6) | 0x32);
		*pControl++ = ctrlEvent[chno].OctaveShift;
		
		//-- Modulation --//
		*pControl++ = 0x00;	// Duratio 0 //
		*pControl++ = 0x00;	// Control Messege //
		*pControl++ = (UINT8)((chno << 6) | 0x20 | ctrlEvent[chno].Modulation);
		
	}
	
	if (bCtrl)
		wNumControl = 23 * 4;
	else
		wNumControl = 7 * 4;	// Octave Shift & Modulation //
	
	// `ltpExclusive Event //
	if (bCtrl) {
		for (chno=0; chno<8; chno++) {
			if (exMssg[chno][0]) {
				*pControl++ = 0x00; // Duration 0 //
				*pControl++ = 0xff; // Exclusive Message //
				*pControl++ = 0xf0;
				*pControl++ = 0x06; // Size //
				*pControl++ = 0x43;
				*pControl++ = 0x03;
				*pControl++ = 0x91;
				*pControl++ = (UINT8)(chno + 0x18);
				*pControl++ = exMssg[chno][1];
				*pControl++ = 0xf7;

				wNumControl += 10;
			}
		}
	}
	
	return  wNumControl;
}


/*********************************************************************

		Name:		SmafAudioTrackSeek
		Function:	Audio TrackÌSequence DataðèÌÊuÜÅSeek·é
		Input :		UINT8  bAudioTrack	... Audio Track No.
					UINT32 dwSeekAbs	... SeekPointÜÅÌPÊÔ
					UINT8 **pSeekPoint	... SeekÀsãÅÌEventÜ½Í
											Control MessageÌÊu
					UINT32 *dwSeekDuration ... SeekãÅÌEventÜ½Í
											Control MessageÜÅÌDuration
		Output :

*********************************************************************/
bool SmafAudioTrackSeek(UINT8 bAudioTrack, UINT32 dwSeekAbs,
							  UINT8 **pSeekPoint, UINT32 *dwSeekDuration)
{
	UINT8  *pSrc;
	UINT32  dwSrcDataSize;
	UINT8	bData;
	UINT8	bData0;
	UINT8	bData1;
	UINT8	bData2;
	UINT16  wDuration;
	UINT8   iMessageSize, i;
	
	if (!(AudioTrackInfo[bAudioTrack].bSequenceType)) {
		pSrc = AudioTrackInfo[bAudioTrack].pSequenceDataChunk
					+ AudioTrackInfo[bAudioTrack].SeekPhraseInfo.dwStartPoint;
		dwSrcDataSize = AudioTrackInfo[bAudioTrack].dwSequenceDataChunkSize
									- AudioTrackInfo[bAudioTrack].SeekPhraseInfo.dwStartPoint;
	}
	else {
		pSrc = AudioTrackInfo[bAudioTrack].pSequenceDataChunk;
		dwSrcDataSize = AudioTrackInfo[bAudioTrack].dwSequenceDataChunkSize;
	}

	if(dwSrcDataSize==0 ) return false;

	dwAbsTime = 0;	

	for ( ; ; )  {
		if ( dwSrcDataSize < 3 ||
				((*pSrc==0)&&(*(pSrc+1)==0)&&(*(pSrc+2)==0)&&(*(pSrc+3)==0)) )  {
			*pSeekPoint = pSrc;
			*dwSeekDuration = 0;
			return true;

		}
		else  {
			//	Duration 1 or 2byte
			bData = *pSrc++;
			wDuration = (UINT16)bData;
			dwSrcDataSize--;
			if ( wDuration & 0x80 ) {
				bData = *pSrc++;
				wDuration = (UINT16)(( (wDuration & 0x7F) << 7 ) + (UINT16)bData + 128);
				dwSrcDataSize--;
			}

			dwAbsTime += (UINT32)wDuration;

		/////////////////////////////////////////////////////////////
		
			if( dwAbsTime>=dwSeekAbs ) {
				*pSeekPoint = pSrc;
				*dwSeekDuration = dwAbsTime - dwSeekAbs;
				return true;
			}
		///////////////////////////////////////////////////////////

			//	Event
			bData0 = *pSrc++;
			dwSrcDataSize--;
			switch ( bData0 )  {
			case 0x00:
				// Control Message

				bData1 = *pSrc++;
				dwSrcDataSize--;

				if ( ( bData1 & 0x30 ) != 0x30 ) {			// 2byte
				}
				else {								// 3byte
					bData2 = *pSrc++;
					dwSrcDataSize--;
				}
				break;

			case 0xFF:
				
				// Exclusive Message or NOP
				bData1 = *pSrc++;
				dwSrcDataSize--;
				if ( bData1 == 0xF0 ) { // Exclusive Message
					iMessageSize = *pSrc++;
					dwSrcDataSize--;
					for ( i = 0; i < iMessageSize; i++ ) {
						bData = *pSrc++;
						dwSrcDataSize--;
					}
				}
				else {  // NOP 
				}
				break;

			default:
				// Note Message
				//	Gatetime 1 or 2byte
				bData = *pSrc++;
				dwSrcDataSize--;
				if ( bData & 0x80 )  {
					bData = *pSrc++;
					dwSrcDataSize--;
				}
				break;
			}
		}
	}
	
}


/*********************************************************************

		Name:		SmafAudioTrackControlSeek
		Function:	SeekPointÜÅÌControl DataîñðûW·éB
		Input :		UINT8  bAudioTrack
					UINT8  *pSeekPoint
					UINT8  *pControl
		Output :	UINT16				 Num of Control Data

*********************************************************************/
UINT16 SmafAudioTrackControlSeek(UINT8 bAudioTrack, UINT8 *pSeekPoint,
												 UINT8 *pControl)
{
	UINT8 *pSrc;
	UINT8	bData;
	UINT8	bData0;
	UINT8	bData1;
	UINT8	bData2;
	UINT16  wDuration;
	UINT8   iMessageSize, i;
	UINT16  wNumControl;
	
	struct CONTROL_EVENT ctrlEvent[1];	 
	UINT8 chno;

	wNumControl = 0;
	
	pSrc = AudioTrackInfo[bAudioTrack].pSequenceDataChunk;
	
	if( pSrc>=pSeekPoint ) return 0; 
	
		//-- \¢ÌÌú» --------------------------------//
	for (chno=0; chno<1; chno++) {
		ctrlEvent[chno].ProgramChange = 0x00;
		ctrlEvent[chno].BankSelect = 0x00;
		ctrlEvent[chno].OctaveShift = 0x00;
		ctrlEvent[chno].Modulation = 0x01;
		ctrlEvent[chno].PitchBend = 0x40;
		ctrlEvent[chno].Volume = 0x63;
		ctrlEvent[chno].Panpot = 0x40;
		ctrlEvent[chno].Expression = 0x7f;
	}

	for ( ; ; )  {
		//	Duration 1 or 2byte
		bData = *pSrc++;
		wDuration = (UINT16)bData;
		if ( wDuration & 0x80 ) {
			bData = *pSrc++;
			wDuration = (UINT16)(( (wDuration & 0x7F) << 7 ) + (UINT16)bData + 128);
		}

		/////////////////////////////////////////////////////////////
		
		if( pSrc>=pSeekPoint )  break;

		///////////////////////////////////////////////////////////

		//	Event
		bData0 = *pSrc++;

		switch ( bData0 )  {
		case 0x00:
			// Control Message

			bData1 = *pSrc++;
			chno = (UINT8)((bData1 & 0xc0) >> 6);

			if ( ( bData1 & 0x30 ) != 0x30 ) {			// 2byte
					
				switch ( (bData1 & 0x30) >> 4 )  {
				case 0x0:						// Expression
					break;
				case 0x1:						// Pitch Bend
					break;
				case 0x2:						// Modulation
					break;
				default:						// reserved
					break;
				}
			}
			else {								// 3byte
				bData2 = *pSrc++;
				switch ( bData1 & 0xF )  {
				case 0x4:						// Pitch Bend
					break;
				case 0x7:						// Volume
					ctrlEvent[chno].Volume = bData2;
					break;
				case 0xA:						// Pan
					break;
				case 0xB:						// Expression
					break;
				default:
					break;
				}
			}
			break;

		case 0xFF:
			
			// Exclusive Message or NOP
			bData1 = *pSrc++;

			if ( bData1 == 0xF0 ) { // Exclusive Message
				iMessageSize = *pSrc++;
				for ( i = 0; i < iMessageSize; i++ ) {
					bData = *pSrc++;
				}
			}
			else { // NOP
			}
			break;

		default:
			// Note Message

			bData = *pSrc++;				// Gatetime 1st byte

			if ( bData & 0x80 )  {
				bData = *pSrc++;			// Gatetime 2nd byte
			}

			break;
		}
	}
	
	*pControl++ = 0x00; // Duration 0 //
	*pControl++ = 0x00; // Control Message //
	*pControl++ = 0x37;
	*pControl++ = ctrlEvent[0].Volume;

	wNumControl = 4;
	
	return wNumControl;
}


/*********************************************************************
		Name:		Smaf_CheckChunkHeader
		Function:	`Nwb_ðär·é
*********************************************************************/
static bool Smaf_CheckChunkHeader(char *hd, UINT32 mask)
{
	UINT8  i;
	UINT32 src;
	UINT32 dst;  
	
	src = dst = 0L;
	for( i=0 ; i<4 ; i++ ) {
		src = (src<<8) + (UINT32)(*(hd+i));
		dst = (dst<<8) + (UINT32)(*(pSmafBuf+i));
	}                    
	
	if( (src&mask) == (dst&mask) ) return true;
	return false;
}

/*********************************************************************
		Name:		Smaf_GetContentsInfoChunkBody
		Function:	ContentsInfoChunk©çîñðæèo·
*********************************************************************/
static bool Smaf_GetContentsInfoChunkBody(void)
{
	UINT8 bTemp;

	//	Contents Class
	SmafInfo.bContentsClass = _getb();
	dwContentsInfoChunkSize--;
	bTemp = SmafInfo.bContentsClass;
	// YAMAHAÈOÍÄ¶µÈ¢                 //
	// T|[g·éNXÉ¶ÄÏX·é±Æ //
	if (bTemp != 0x00) {
		return false;
	}

	//	Contents Type
	SmafInfo.bContentsType = _getb();
	dwContentsInfoChunkSize--;
	bTemp = (UINT8)(SmafInfo.bContentsType & 0xf0);
	if ((bTemp != 0x00) && (bTemp != 0x10) && (bTemp != 0x20)) {
		return false;
	}

	//	Contents Code Type
	SmafInfo.bContentsCodeType = _getb();
	dwContentsInfoChunkSize--;

	//	Copy Status
	SmafInfo.bCopyStatus = _getb();
	dwContentsInfoChunkSize--;

	//	Copy Counts
	SmafInfo.bCopyCounts = _getb();
	dwContentsInfoChunkSize--;

	//	Option
	if ( dwContentsInfoChunkSize != 0 ) {
		SmafInfo.pOption = SmafInfo.pContentsInfoChunk + 5;
		SmafInfo.dwOptionSize = dwContentsInfoChunkSize;
		while( dwContentsInfoChunkSize-- > 0 )
			_getb();
	}

	return  true;
}

/*********************************************************************
		Name:		Smaf_GetScoreTrackChunkBody
		Function:	ScoreTrackChunk©çîñðæèo·
*********************************************************************/
static bool Smaf_GetScoreTrackChunkBody( void )
{
UINT8 timebase;

	//	Format Type
	ScoreTrackInfo[bScoreTrack].bFormatType = _getb();
	dwScoreTrackChunkSize--;
	if (ScoreTrackInfo[bScoreTrack].bFormatType != 0x00) {
		return false;
	}

	//	Sequence Type
	ScoreTrackInfo[bScoreTrack].bSequenceType = _getb();
	dwScoreTrackChunkSize--;
	if (ScoreTrackInfo[bScoreTrack].bSequenceType != 0x00)  {
		return false;
	}

	//	TimeBase_D
	timebase = _getb();
	dwScoreTrackChunkSize--;
	ScoreTrackInfo[bScoreTrack].dwTimeBaseD = _get_timebase( timebase );
	if (ScoreTrackInfo[bScoreTrack].dwTimeBaseD == 0) {
		return false;
	}

	//	TimeBase_G
	timebase = _getb();
	dwScoreTrackChunkSize--;
	ScoreTrackInfo[bScoreTrack].dwTimeBaseG = _get_timebase( timebase );
	if (ScoreTrackInfo[bScoreTrack].dwTimeBaseG == 0) {
		return false;
	}

	if (ScoreTrackInfo[bScoreTrack].dwTimeBaseD != ScoreTrackInfo[bScoreTrack].dwTimeBaseG) {
		return false;
	}

	//	Channel Status
	ScoreTrackInfo[bScoreTrack].wChannelStatus = _getw();
	dwScoreTrackChunkSize -= 2;

	return true;
}

/*********************************************************************
		Name:		Smaf_GetAudioTrackChunkBody	
		Function:	AudioTrackChunk©çîñðæèo·
*********************************************************************/
static bool Smaf_GetAudioTrackChunkBody( void )
{
UINT8 timebase;

	//	Format Type
	AudioTrackInfo[bAudioTrack].bFormatType = _getb();
	dwAudioTrackChunkSize--;
	if (AudioTrackInfo[bAudioTrack].bFormatType != 0x00) {
		return false;
	}

	//	Sequence Type
	AudioTrackInfo[bAudioTrack].bSequenceType = _getb();
	dwAudioTrackChunkSize--;
	if (AudioTrackInfo[bAudioTrack].bSequenceType != 0x00) {
		return false;
	}

	//	Wave Type
	AudioTrackInfo[bAudioTrack].wWaveType = _getw();
	dwAudioTrackChunkSize -= 2;
	if ((AudioTrackInfo[bAudioTrack].wWaveType & 0xfef0) != 0x1000) {
		return false;
	}
	
	//	TimeBase_D
	timebase = _getb();
	dwAudioTrackChunkSize--;
	AudioTrackInfo[bAudioTrack].dwTimeBaseD = _get_timebase( timebase );
	if (AudioTrackInfo[bAudioTrack].dwTimeBaseD == 0) {
		return false;
	}

	//	TimeBase_G
	timebase = _getb();
	dwAudioTrackChunkSize--;
	AudioTrackInfo[bAudioTrack].dwTimeBaseG = _get_timebase( timebase );
	if (AudioTrackInfo[bAudioTrack].dwTimeBaseG == 0) {
		return false;
	}
	
	if (AudioTrackInfo[bAudioTrack].dwTimeBaseD != AudioTrackInfo[bAudioTrack].dwTimeBaseG) {
		return false;
	}

	return true;
}

/*********************************************************************
		Name:		Smaf_GetSeekPhraseInfoChunkBody
		Function:	SeekPhraseInfoChunk©çîñðæèo·
*********************************************************************/
static bool Smaf_GetSeekPhraseInfoChunkBody(SEEKPHRASEINFO *pInfo)
{
UINT16 tag;

	while( dwSeekPhraseInfoChunkSize>0 ) {
		tag = _getw();
		++pSmafBuf;                   			// ':'
		dwSeekPhraseInfoChunkSize -= 3;
		switch( tag ) {
			case 0x7374:			// Start Point
				// Stream Sequence
				if( ScoreTrackInfo[bScoreTrack].bSequenceType==0x00 ) {
					pInfo->dwStartPoint = (SINT32)_getdw();
					dwSeekPhraseInfoChunkSize -= 4;
				}
				// Sub-Sequence
				else {
					pInfo->dwStartPoint = (SINT32)_getb();
					dwSeekPhraseInfoChunkSize -= 1;
				}
				break;
			case 0x7370:			// Stop Point
				// Stream Sequence
				if( ScoreTrackInfo[bScoreTrack].bSequenceType==0x00 ) {
					pInfo->dwStopPoint = _getdw();
					dwSeekPhraseInfoChunkSize -= 4;
				}
				// Sub-Sequence
				else {
					pInfo->dwStopPoint = (SINT32)_getb();
					dwSeekPhraseInfoChunkSize -= 1;
				}
				break;
			default:
				pSmafBuf += dwSeekPhraseInfoChunkSize-1;
				dwSeekPhraseInfoChunkSize = 1;
				break;
		}
		++pSmafBuf;                   			// ','
		--dwSeekPhraseInfoChunkSize;
	}	
	return true;
}

/*********************************************************************
		Name:		SmafGetSetupChunkBody
		Function:	SetupChunk©çîñðæèo·
*********************************************************************/
static bool Smaf_GetSetupDataChunkBody(void)
{
UINT16 wStatus;
UINT32 dwExType;
UINT8  iMessageSize, i;
UINT8  bData;

	while( dwSetupDataChunkSize>0 ) {
		wStatus = _getw();
		dwSetupDataChunkSize -= 2;

		if ( wStatus != 0xFFF0 ) {
			return false;
		}
		else  {
			iMessageSize = _getb();
			dwSetupDataChunkSize--;
			
			// Voice Message Check
			dwExType = _getdw();
			dwSetupDataChunkSize -= 4;
			if ((dwExType & 0xfffff800) == 0x43020000) { // MA-1
				bVoiceMsg1 = 1;
			}
			if ((dwExType & 0xfffff000) == 0x43030000) { // MA-2
				bVoiceMsg2 = 1;
			}
			
			for ( i = 0; i < iMessageSize-4; i++ ) {
				bData = _getb();
				dwSetupDataChunkSize--;
			}
		}
	}
	return true;
}
/*********************************************************************
		Name:		Smaf_GetAudioSetupDataChunkBody
		Function:	AudioSetupDataChunk©çîñðæèo·
*********************************************************************/
static bool Smaf_GetAudioSetupDataChunkBody(void)
{
	pSmafBuf += dwSetupDataChunkSize;
	dwSetupDataChunkSize = 0;
	return true;
}

/*********************************************************************
		Name:		Smaf_GetSequenceDataChunkBody
		Function:	SequenceDataChunk(ScoreTrack)©çîñðæèo·
*********************************************************************/
static bool Smaf_GetSequenceDataChunkBody(void)
{
	UINT8	bData;
	UINT8	bData0;
	UINT8	bData1;
	UINT8	bData2;
	UINT16  wDuration;
	UINT8   iMessageSize, i;
	
	for ( ; ; )  {
		if ( dwSequenceDataChunkSize < 3 ||
					((*pSmafBuf==0)&&(*(pSmafBuf+1)==0)&&(*(pSmafBuf+2)==0)&&(*(pSmafBuf+3)==0)) )  {
			pSmafBuf += dwSequenceDataChunkSize;
			
			if (ScoreTrackInfo[bScoreTrack].ScoreAbsTime > dwAbsTime)
				ScoreTrackInfo[bScoreTrack].NopDuration = ScoreTrackInfo[bScoreTrack].ScoreAbsTime - dwAbsTime;

			return true;
		}
		else  {
			//	Duration 1 or 2byte
			wDuration = (UINT16)_getb();
			dwSequenceDataChunkSize--;
			if ( wDuration & 0x80 ) {
				wDuration = (UINT16)(( ( (wDuration & 0x7F) << 7 ) + (UINT16)_getb() ) + 128);
				dwSequenceDataChunkSize--;
			}
			dwAbsTime += (UINT32)wDuration;
			//	Event
			bData0 = _getb();
			dwSequenceDataChunkSize--;
			switch ( bData0 )  {
			case 0x00:
				// Control Message

				bData1 = _getb();
				dwSequenceDataChunkSize--;

				if ( ( bData1 & 0x30 ) != 0x30 ) {			// 2byte
					switch ( (bData1 & 0x30) >> 4 )  {
					case 0x0:						// Expression
						break;
					case 0x1:						// Pitch Bend
						break;
					case 0x2:						// Modulation
						break;
					default:						// reserved
						break;
					}
				}
				else {								// 3byte
					bData2 = _getb();
					dwSequenceDataChunkSize--;						
					switch ( bData1 & 0xF )  {
					case 0x0:						// Program Change
						break;
					case 0x1:						// Bank Select
						break;
					case 0x2:						// Octave Shift
						break;
					case 0x3:						// Modulation
						break;
					case 0x4:						// Pitch Bend
						break;
					case 0x7:						// Volume
						break;
					case 0xA:						// Pan
						break;
					case 0xB:						// Expression
						break;
					default:
						break;
					}
				}
				break;

			case 0xFF:
				
				// Exclusive Message or NOP
				bData1 = _getb();
				dwSequenceDataChunkSize--;
				if ( bData1 == 0xF0 ) {
					iMessageSize = _getb();
					dwSequenceDataChunkSize--;
					for ( i = 0; i < iMessageSize; i++ ) {
						bData = _getb();
						dwSequenceDataChunkSize--;
					}
				}
				else {
				}
				break;

			default:
				// Note Message

				//	Gatetime 1 or 2byte
				wGatetime = (UINT16)_getb();
				dwSequenceDataChunkSize--;
				if ( wGatetime & 0x80 )  {
					wGatetime = (UINT16)(( ( (wGatetime & 0x7F) << 7 ) + (UINT16)_getb() ) + 128);
					dwSequenceDataChunkSize--;
				}
				if( dwAbsTime+(UINT32)wGatetime > ScoreTrackInfo[bScoreTrack].ScoreAbsTime )
					ScoreTrackInfo[bScoreTrack].ScoreAbsTime = dwAbsTime+(UINT32)wGatetime;

				break;
			}
		}
	}
}

/*********************************************************************
		Name:		Smaf_GetAudioSequenceDataChunkBody
		Function:	SequenceDataChunk(AudioTrack)©çîñðæèo·
*********************************************************************/
static bool Smaf_GetAudioSequenceDataChunkBody(void)
{
	UINT8	bData;
	UINT8	bData0;
	UINT8	bData1;
	UINT8	bData2;
	UINT16  wDuration;
	UINT8   iMessageSize, i;
	
	for ( ; ; )  {
		if ( dwSequenceDataChunkSize < 3 ||
				((*pSmafBuf==0)&&(*(pSmafBuf+1)==0)&&(*(pSmafBuf+2)==0)&&(*(pSmafBuf+3)==0)) )  {
			pSmafBuf += dwSequenceDataChunkSize;
			return true;
		}
		else  {
			//	Duration 1 or 2byte
			wDuration = (UINT16)_getb();
			dwSequenceDataChunkSize--;
			if ( wDuration & 0x80 ) {
				wDuration = (UINT16)(( ( (wDuration & 0x7F) << 7 ) + (UINT16)_getb() ) + 128);
				dwSequenceDataChunkSize--;
			}

			dwAbsTime += (UINT32)wDuration;

			//	Event
			bData0 = _getb();
			dwSequenceDataChunkSize--;
			switch ( bData0 )  {
			case 0x00:
				// Control Message

				bData1 = _getb();
				dwSequenceDataChunkSize--;

				if ( ( bData1 & 0x30 ) != 0x30 ) {			// 2byte
					switch ( (bData1 & 0x30) >> 4 )  {
					case 0x0:						// Expression
						break;
					case 0x1:						// Pitch Bend
						break;
					case 0x2:						// Modulation
						break;
					default:						// reserved
						break;
					}
				}
				else {								// 3byte
					bData2 = _getb();
					dwSequenceDataChunkSize--;						
					switch ( bData1 & 0xF )  {
					case 0x4:						// Pitch Bend
						break;
					case 0x7:						// Volume
						break;
					case 0xA:						// Pan
						break;
					case 0xB:						// Expression
						break;
					default:
						break;
					}
				}
				break;

			case 0xFF:
				
				// Exclusive Message or NOP
				bData1 = _getb();
				dwSequenceDataChunkSize--;
				if ( bData1 == 0xF0 ) {
					iMessageSize = _getb();
					dwSequenceDataChunkSize--;
					for ( i = 0; i < iMessageSize; i++ ) {
						bData = _getb();
						dwSequenceDataChunkSize--;
					}
				}
				else {
				}
				break;

			default:
				// Note Message
				//	Gatetime 1 or 2byte
				wGatetime = (UINT16)_getb();
				dwSequenceDataChunkSize--;
				if ( wGatetime & 0x80 )  {
					wGatetime = (UINT16)(( ( (wGatetime & 0x7F) << 7 ) + (UINT16)_getb() ) + 128);
					dwSequenceDataChunkSize--;
				}
				if( dwAbsTime+(UINT32)wGatetime > AudioTrackInfo[bAudioTrack].AudioAbsTime )
					AudioTrackInfo[bAudioTrack].AudioAbsTime = dwAbsTime+(UINT32)wGatetime;

				break;
			}
		}
	}
}

/*********************************************************************
		Name:		SmafChecker
		Function:	Smaf DataððÍµAKvÈîñðæèo·
*********************************************************************/
int SmafChecker( UINT8* pBuffer )
{
UINT8 youandi[10];
UINT16 i,j;
SINT32 dwTemp;
UINT32 dwTemp2, dwTemp3;
UINT16 crc, crcval;

	SmafInfo.pOverallChunk = NULL;
	SmafInfo.dwOverallChunkSize = 0;
	SmafInfo.pContentsInfoChunk = NULL;
	SmafInfo.dwContentsInfoChunkSize = 0;
	SmafInfo.pOption = NULL;
	SmafInfo.dwOptionSize = 0;

	for( i=0 ; i<MAX_SCORE_TRACK_CHUNK ; i++ ) {
		ScoreTrackInfo[i].pScoreTrackChunk = NULL;
		ScoreTrackInfo[i].dwScoreTrackChunkSize = 0;
		ScoreTrackInfo[i].pSeekPhraseInfoChunk = NULL;
		ScoreTrackInfo[i].dwSeekPhraseInfoChunkSize = 0;
		ScoreTrackInfo[i].pSetupDataChunk = NULL;
		ScoreTrackInfo[i].dwSetupDataChunkSize = 0;
		ScoreTrackInfo[i].pSequenceDataChunk = NULL ;
		ScoreTrackInfo[i].dwSequenceDataChunkSize = 0;
		ScoreTrackInfo[i].ScoreAbsTime = 0;
		ScoreTrackInfo[i].NopDuration = 0;
		ScoreTrackInfo[i].SeekPhraseInfo.dwStartPoint=0;
		ScoreTrackInfo[i].SeekPhraseInfo.dwStopPoint=0;
	}
	for( i=0 ; i<MAX_AUDIO_TRACK_CHUNK ; i++ ) {

		AudioTrackInfo[i].pAudioTrackChunk = NULL;
		AudioTrackInfo[i].dwAudioTrackChunkSize = 0;
		AudioTrackInfo[i].pAudioTrackSeekPhraseInfoChunk = NULL;
		AudioTrackInfo[i].dwAudioTrackSeekPhraseInfoChunkSize = 0;
		AudioTrackInfo[i].pSetupDataChunk = NULL;
		AudioTrackInfo[i].dwSetupDataChunkSize = 0;
		AudioTrackInfo[i].pSequenceDataChunk = NULL;
		AudioTrackInfo[i].dwSequenceDataChunkSize = 0;
		AudioTrackInfo[i].AudioAbsTime = 0;
		for( j=0 ; j<MAX_WAVE_DATA ; j++ ) {
			AudioTrackInfo[i].pWaveDataChunk[j] = NULL;
			AudioTrackInfo[i].dwWaveDataChunkSize[j] = 0;
		}
		AudioTrackInfo[i].SeekPhraseInfo.dwStartPoint=0;
		AudioTrackInfo[i].SeekPhraseInfo.dwStopPoint=0;
	}
	
	bVoiceMsg1 = 0;
	bVoiceMsg2 = 0;
	bMa1ScoreTrack = 0;
	bMa2ScoreTrack = 0;
 	
	//	t@C|C^ðÝè
	pSmafBuf = pBuffer;

	/////////////////////////////////////////////////////////////////////////
	//	SÌ`Nwb_
	/////////////////////////////////////////////////////////////////////////

	if( Smaf_CheckChunkHeader("MMMD", 0xffffffff)!=true ) return SEQ_ERROR;	

	pSmafBuf += 4;
	dwOverallChunkSize = _getdw();

	SmafInfo.pOverallChunk = pSmafBuf;
	SmafInfo.dwOverallChunkSize = dwOverallChunkSize;   

	/////////////////////////////////////////////////////////////////////////
	//	Contents Info Chunk
	/////////////////////////////////////////////////////////////////////////

	if( Smaf_CheckChunkHeader("CNTI", 0xffffffff)!=true )  return SEQ_ERROR;

	pSmafBuf += 4;
	dwContentsInfoChunkSize = _getdw();
	dwTemp = dwContentsInfoChunkSize;			// Save it

	SmafInfo.pContentsInfoChunk = pSmafBuf;
	SmafInfo.dwContentsInfoChunkSize = dwContentsInfoChunkSize;
	
	//	Chunk Size Check
	if  (dwContentsInfoChunkSize < 5) return SEQ_ERROR;

	//	Contents Info Chunk body
	if ( Smaf_GetContentsInfoChunkBody() != true ) return SEQ_ERROR;

	dwOverallChunkSize -= (  dwTemp + SIZE_OF_CHUNKHEADER );

	while( dwOverallChunkSize>2 ) { 				// except CRC
	
		/////////////////////////////////////////////////////////////////////////
		//	Score Track Chunk
		/////////////////////////////////////////////////////////////////////////
		if ( Smaf_CheckChunkHeader("MTR ", 0xffffff00) == true ) {

		  dwAbsTime   = 0;						//

		  pSmafBuf += 3;
		  bScoreTrack = _getb();				// Score Track #
		  dwScoreTrackChunkSize = _getdw();
		  dwOverallChunkSize -= ( dwScoreTrackChunkSize + SIZE_OF_CHUNKHEADER );

		  if( bScoreTrack>=MAX_SCORE_TRACK_CHUNK ) {
				pSmafBuf += dwScoreTrackChunkSize;
		  }
		  else {
				
			if (bScoreTrack == 0) 
				bMa1ScoreTrack = 1;
			else
				bMa2ScoreTrack = 1;
			
			ScoreTrackInfo[bScoreTrack].pScoreTrackChunk = pSmafBuf;
			ScoreTrackInfo[bScoreTrack].dwScoreTrackChunkSize = dwScoreTrackChunkSize;

			//	ChunkSize Check
			if (dwScoreTrackChunkSize <6) return SEQ_ERROR;


			//	Score Track Chunk body
			if ( Smaf_GetScoreTrackChunkBody()!= true )  {
				return SEQ_ERROR;
			}

			while ( dwScoreTrackChunkSize>0 ) {
				//	Seek & Phrase Info Chunk
				if ( Smaf_CheckChunkHeader("MspI", 0xffffffff) == true )  {

					pSmafBuf += 4;
					dwSeekPhraseInfoChunkSize = _getdw();
					dwTemp = dwSeekPhraseInfoChunkSize + SIZE_OF_CHUNKHEADER;

					ScoreTrackInfo[bScoreTrack].pSeekPhraseInfoChunk = pSmafBuf;
					ScoreTrackInfo[bScoreTrack].dwSeekPhraseInfoChunkSize = dwSeekPhraseInfoChunkSize;

					//	Seek & Phrase Info Chunk body
					if ( Smaf_GetSeekPhraseInfoChunkBody(&ScoreTrackInfo[bScoreTrack].SeekPhraseInfo) != true ) {
						return SEQ_ERROR;
					}
					else  {
						dwScoreTrackChunkSize -= dwTemp;
					}
				}
		
				//	Setup Data Chunk
				else if ( Smaf_CheckChunkHeader("Mtsu", 0xffffffff) == true ) {

					pSmafBuf += 4;
					dwSetupDataChunkSize = _getdw();
					dwTemp = dwSetupDataChunkSize + SIZE_OF_CHUNKHEADER;

					ScoreTrackInfo[bScoreTrack].pSetupDataChunk = pSmafBuf;
					ScoreTrackInfo[bScoreTrack].dwSetupDataChunkSize = dwSetupDataChunkSize;

					//	Setup Data Chunk body
					if ( Smaf_GetSetupDataChunkBody() != true )  {
						return SEQ_ERROR;
					}
					else {
						dwScoreTrackChunkSize -= dwTemp;
					}
				}

				//	Sequence Data Chunk
				else if ( Smaf_CheckChunkHeader("Mtsq", 0xffffffff) == true ) {

					pSmafBuf += 4;
					dwSequenceDataChunkSize = _getdw();
					dwTemp = dwSequenceDataChunkSize + SIZE_OF_CHUNKHEADER;

					ScoreTrackInfo[bScoreTrack].pSequenceDataChunk = pSmafBuf;
					ScoreTrackInfo[bScoreTrack].dwSequenceDataChunkSize = dwSequenceDataChunkSize;

					//	Sequence Data Chunk body
					if ( Smaf_GetSequenceDataChunkBody() != true ) {
						return SEQ_ERROR;
					}
					else {
						dwScoreTrackChunkSize -= dwTemp;
					}
				}
				else {
					return SEQ_ERROR;					// Illegal Chunk Header !!
				}
			}
			
			// Sequence Data Chunk check
			if (ScoreTrackInfo[bScoreTrack].pSequenceDataChunk == NULL) {
				return SEQ_ERROR;
			}
			
			// Start/Stop Point check
			dwTemp2 = ScoreTrackInfo[bScoreTrack].SeekPhraseInfo.dwStartPoint;
			dwTemp3 = ScoreTrackInfo[bScoreTrack].SeekPhraseInfo.dwStopPoint;
			if ((dwTemp2 != 0) || (dwTemp3 != 0)) {
				if (ScoreTrackInfo[bScoreTrack].bSequenceType == 0) {
					if (dwTemp3 != 0) {
						ScoreTrackInfo[bScoreTrack].NopDuration = 0;
					}
					else {
						ScoreTrackInfo[bScoreTrack].SeekPhraseInfo.dwStopPoint = ScoreTrackInfo[bScoreTrack].dwSequenceDataChunkSize;
						dwTemp3 = ScoreTrackInfo[bScoreTrack].SeekPhraseInfo.dwStopPoint;
					}
					if (dwTemp2 > ScoreTrackInfo[bScoreTrack].dwSequenceDataChunkSize) {
						return SEQ_ERROR;
					}
					if (dwTemp3 > ScoreTrackInfo[bScoreTrack].dwSequenceDataChunkSize) {
						return SEQ_ERROR;
					}
				}
				if (dwTemp2 >= dwTemp3) {
					return SEQ_ERROR;
				}
			}
		  }
		}
		/////////////////////////////////////////////////////////////////////////
		//	PCM Audio Track Chunk
		/////////////////////////////////////////////////////////////////////////
		else if ( Smaf_CheckChunkHeader("ATR ", 0xffffff00) == true ) {

		  dwAbsTime   = 0;						//

		  pSmafBuf += 3;
		  bAudioTrack = _getb();				// PCM Audio Track #
		  dwAudioTrackChunkSize = _getdw();
		  dwOverallChunkSize -= ( dwAudioTrackChunkSize + SIZE_OF_CHUNKHEADER );

		  if( bAudioTrack>=MAX_AUDIO_TRACK_CHUNK ) {
				pSmafBuf += dwAudioTrackChunkSize;
		  }
		  else {

			AudioTrackInfo[bAudioTrack].pAudioTrackChunk = pSmafBuf;
			AudioTrackInfo[bAudioTrack].dwAudioTrackChunkSize = dwAudioTrackChunkSize;

			//	ChunkSize Check
			if (dwAudioTrackChunkSize < 6) return SEQ_ERROR;

			//	PCM AUdio Track Chunk body
			if ( Smaf_GetAudioTrackChunkBody() != true )  {
				return SEQ_ERROR;
			}

			while ( dwAudioTrackChunkSize>0 ) {
				//	Seek & Phrase Info Chunk
				if ( Smaf_CheckChunkHeader("AspI", 0xffffffff) == true )  {

					pSmafBuf += 4;
					dwSeekPhraseInfoChunkSize = _getdw();
					dwTemp = dwSeekPhraseInfoChunkSize + SIZE_OF_CHUNKHEADER;

					AudioTrackInfo[bAudioTrack].pAudioTrackSeekPhraseInfoChunk = pSmafBuf;
					AudioTrackInfo[bAudioTrack].dwAudioTrackSeekPhraseInfoChunkSize = dwSeekPhraseInfoChunkSize;

					//	Seek & Phrase Info Chunk body
					if ( Smaf_GetSeekPhraseInfoChunkBody(&AudioTrackInfo[bAudioTrack].SeekPhraseInfo) != true ) {
						return SEQ_ERROR;
					}
					else  {
						dwAudioTrackChunkSize -= dwTemp;
					}
				}
		
				//	Setup Data Chunk
				else if ( Smaf_CheckChunkHeader("Atsu", 0xffffffff) == true ) {

					pSmafBuf += 4;
					dwSetupDataChunkSize = _getdw();
					dwTemp = dwSetupDataChunkSize + SIZE_OF_CHUNKHEADER;

					AudioTrackInfo[bAudioTrack].pSetupDataChunk = pSmafBuf;
					AudioTrackInfo[bAudioTrack].dwSetupDataChunkSize = dwSetupDataChunkSize;

					//	Setup Data Chunk body
					if ( Smaf_GetAudioSetupDataChunkBody() != true )  {
						return SEQ_ERROR;
					}
					else {
						dwAudioTrackChunkSize -= dwTemp;
					}
				}

				//	Sequence Data Chunk
				else if ( Smaf_CheckChunkHeader("Atsq", 0xffffffff) == true ) {

					pSmafBuf += 4;
					dwSequenceDataChunkSize = _getdw();
					dwTemp = dwSequenceDataChunkSize + SIZE_OF_CHUNKHEADER;

					AudioTrackInfo[bAudioTrack].pSequenceDataChunk = pSmafBuf;
					AudioTrackInfo[bAudioTrack].dwSequenceDataChunkSize = dwSequenceDataChunkSize;

					//	Sequence Data Chunk body
					if ( Smaf_GetAudioSequenceDataChunkBody() != true ) {
						return SEQ_ERROR;
					}
					else {
						dwAudioTrackChunkSize -= dwTemp;
					}
				}
				// Wave Data Chunk
				else if ( Smaf_CheckChunkHeader("Awa ", 0xffffff00) == true ) {

					pSmafBuf += 3;
					bWaveData = _getb();
					// Wave Number check
					if ((bWaveData == 0) || (bWaveData > 0x3e)) {
						return SEQ_ERROR;
					}
					
					dwWaveDataChunkSize = _getdw();
					dwTemp = dwWaveDataChunkSize + SIZE_OF_CHUNKHEADER;

					AudioTrackInfo[bAudioTrack].pWaveDataChunk[bWaveData] = pSmafBuf;
					AudioTrackInfo[bAudioTrack].dwWaveDataChunkSize[bWaveData] = dwWaveDataChunkSize;

					pSmafBuf += dwWaveDataChunkSize;
					dwAudioTrackChunkSize -= dwTemp;
				}
				else
					return SEQ_ERROR;				// Illegal Chunk Header !!
			}
			
			// Sequence Data Chunk check
			if (AudioTrackInfo[bAudioTrack].pSequenceDataChunk == NULL) {
				return SEQ_ERROR;
			}
			
			// Start/Stop Point check
			dwTemp2 = AudioTrackInfo[bAudioTrack].SeekPhraseInfo.dwStartPoint;
			dwTemp3 = AudioTrackInfo[bAudioTrack].SeekPhraseInfo.dwStopPoint;
			if ((dwTemp2 != 0) || (dwTemp3 != 0)) {
				if (AudioTrackInfo[bAudioTrack].bSequenceType == 0) {
					if (dwTemp3 == 0) {
						AudioTrackInfo[bAudioTrack].SeekPhraseInfo.dwStopPoint = AudioTrackInfo[bAudioTrack].dwSequenceDataChunkSize;
						dwTemp3 = AudioTrackInfo[bAudioTrack].SeekPhraseInfo.dwStopPoint;
					}
					if (dwTemp2 > AudioTrackInfo[bAudioTrack].dwSequenceDataChunkSize) {
						return SEQ_ERROR;
					}
					if (dwTemp3 > AudioTrackInfo[bAudioTrack].dwSequenceDataChunkSize) {
						return SEQ_ERROR;
					}
				}
				if (dwTemp2 >= dwTemp3) {
					return SEQ_ERROR;
				}
			}
		  }
		}
		
		/////////////////////////////////////////////////////////////////////////
		//	Graphic Track Chunk
		/////////////////////////////////////////////////////////////////////////
		else if ( Smaf_CheckChunkHeader("GTR ", 0xffffff00) == true ) {

			pSmafBuf += 3;
			bGraphicTrack = _getb();
			dwGraphicTrackChunkSize = _getdw();
			dwOverallChunkSize -= ( dwGraphicTrackChunkSize + SIZE_OF_CHUNKHEADER );

			pSmafBuf += dwGraphicTrackChunkSize;		// skip graphic data
		}

		/////////////////////////////////////////////////////////////////////////
		//	Master Track Chunk
		/////////////////////////////////////////////////////////////////////////
		else if ( Smaf_CheckChunkHeader("MSTR", 0xffffffff) == true ) {

			pSmafBuf += 3;
			bMasterTrack = _getb();
			dwMasterTrackChunkSize = _getdw();
			dwOverallChunkSize -= ( dwMasterTrackChunkSize + SIZE_OF_CHUNKHEADER );

			pSmafBuf += dwMasterTrackChunkSize;		// skip master data
		}
		else {
			return SEQ_ERROR;			// Illegal Chunk Header!!
		}
	}

	// Voice Parameter check
	if (bMa2ScoreTrack == 1) {
		if (bVoiceMsg2 == 0) {
			return SEQ_ERROR;
		}
	}
	else {
		if (bMa1ScoreTrack == 1) {
			if (bVoiceMsg1 == 0) {
				return SEQ_ERROR;
			}
		}
	}

	// CRC check
	if (dwOverallChunkSize == 2) {
		crcval = 1;//makeCRC((int)(SmafInfo.dwOverallChunkSize+8L), pBuffer);
		makeCRC(10,youandi);//youandi 01.04.06 ¿Í´×Àâ±â
		crc = _getw();
		if (crc != crcval) {
//			return SEQ_ERROR;
			return SEQ_SUCCEED; // ÀÓ½Ã·Î °íÃÆÀ½.......
		}
	}

	return SEQ_SUCCEED;
}
//smafchecker.c
//smafseq.c

//==========================================================================
//  int Smaf_Initialize()
//
//  Description: SMAFV[PTW[Ìú»
//  Parameter:	Èµ
//  Return :		 0	³íI¹
//==========================================================================
int Smaf_Initialize(void)
{      
	UINT8 count, trackno;
	
	for (count=0; count < MAX_SMAF_DATA_NUM; count++) {
		Smaf[count].hAllChunk = 0;
		for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++)
			Smaf[count].hMtrChunk[trackno] = 0;
		for (trackno=0; trackno<MAX_PCM_TRACK_NUM; trackno++)
			Smaf[count].hAtrChunk[trackno] = 0;
		for (trackno=0; trackno<MAX_GRAPHIC_TRACK_NUM; trackno++)
			Smaf[count].hGtrChunk[trackno] = 0;
		for (trackno=0; trackno<MAX_MASTER_TRACK_NUM; trackno++)
			Smaf[count].hMstrChunk[trackno] = 0;
		Smaf[count].MtrNum = 0;
		Smaf[count].AtrNum = 0;
		Smaf[count].GtrNum = 0;
		Smaf[count].MstrNum = 0;
		Smaf[count].isMa2 = 1;
		
		SeqInf[count].pSmaf = 0;
		SeqInf[count].playingSeq = 0;
		SeqInf[count].playMode = 0;
		SeqInf[count].playTimes = 0;
		SeqInf[count].pSmafCBProc = NULL;
		SeqInf[count].hSoundInf = -1;
	}
		
	Sound_Initialize();

	return SEQ_SUCCEED;
}

//==========================================================================
//  void *Smaf_Load(void *pSmaf, UINT8 *pFileBuf, int fileSize, 
//  								int loadMode, int *pStatus)
//
//  Description: SMAFf[^Ì[h¨æÑðß
//  Parameter:
//  					pSmaf			·ÅÉLoadµ½SMAFf[^îñÖÌ|C^
//  										¡ÌSMAFf[^ÇÝÝ¢ÎÌ½ß0
//  					pFileBuf	SMAFt@Ci[ÌæÖÌ|C^
//  					fileSize	SMAFt@CÌTCY
//  					loadMode	@\g£p@(0)
//  					pStatus		I¹óÔâG[R[hði[·éÌæÖÌ|C^
//  Return :	0		G[
//  					!=0	SMAF\¢ÌÖÌ|C^
//==========================================================================
void * Smaf_Load(void *pSmaf, UINT8 *pFileBuf, int fileSize, int loadMode, int *pStatus)
{
	PSMAF hSmaf;
	UINT8 smafno=0;
	UINT8 trackno;    

	*pStatus = SEQ_ERROR;

	//----- p[^G[`FbN -----------------------------------//
	if ((pFileBuf == 0) || (fileSize == 0)) {
		return 0;
	}

	if (pSmaf == NULL) {
		while ((smafno < MAX_SMAF_DATA_NUM) && (Smaf[smafno].hAllChunk != 0))
			smafno++;
		if (smafno >= MAX_SMAF_DATA_NUM) {
			return 0;   
		}
		else
			hSmaf = &Smaf[smafno];
	}
	else {
		hSmaf = (PSMAF)pSmaf;
	}
	
	//----- SMAFðß ---------------------------------------------------//
	if (SmafChecker(pFileBuf) == SEQ_ERROR) {
		return 0;
	}

	//-- SMAFðßÊðSMAF\¢ÌÖ --------------------//
	// All Chunk //
	if (SmafInfo.dwOverallChunkSize) {
		hSmaf->hAllChunk = &(SmafInfo);
	}
	
	// Score Track //
	for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++) {
		if (ScoreTrackInfo[trackno].dwScoreTrackChunkSize) {
			hSmaf->hMtrChunk[trackno] = &(ScoreTrackInfo[trackno]);
			hSmaf->MtrNum |= (0x01 << trackno);
		}
	}
	
	// PCM Audio Track //
	for (trackno=0; trackno<MAX_PCM_TRACK_NUM; trackno++) {
		if (AudioTrackInfo[trackno].dwAudioTrackChunkSize) { 
			hSmaf->hAtrChunk[trackno] = &(AudioTrackInfo[trackno]);
			hSmaf->AtrNum |= (0x01 << trackno);
		}
	}

	// Score Track Chunk, PCM Audio Track ChunkÆàÉÈ¢êÍG[ //
	if ((hSmaf->MtrNum+hSmaf->AtrNum) == 0) {
		hSmaf->hAllChunk = 0;
		return 0;
	}

	if (hSmaf->MtrNum) {
		// TrackÔ1ÈãÌgbNª êÎAMA1pTrack0ÍÆÎ· //
		// TrackÔ1¨0A2¨1EEEÆTrackÔðVtg·é      //
		// iFIFOÔÆÌÎÌ½ßj                            //
		// È¯êÎMA1pÌTrack0ð·é                       //
		if (hSmaf->MtrNum & 0xFE) {
			hSmaf->isMa2 = 1;
			for (trackno=0; trackno<MAX_SCORE_TRACK_NUM-1; trackno++) {
					hSmaf->hMtrChunk[trackno] = hSmaf->hMtrChunk[trackno+1];
					if (hSmaf->hMtrChunk[trackno] != 0)
						hSmaf->hMtrChunk[trackno]->NopDuration = 0;
			}
			hSmaf->MtrNum = (UINT8)(hSmaf->MtrNum >> 1);
		}
		else {
			hSmaf->isMa2 = 0;
		}
	}

	*pStatus = SEQ_SUCCEED;
	
	return (void *)hSmaf;
}


//==========================================================================
//  int Smaf_Unload(void *pSmaf)
//
//  Description: SMAFf[^ÌA[h
//  Parameter: 
//  					pSmaf		SMAFf[^ÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  									Smaf_Load()Ìßèl
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Smaf_Unload(void *pSmaf)
{
	PSMAF hSmaf;
	UINT8 trackno;
                
	//----- p[^G[`FbN -----------------------------------//
	if (pSmaf == 0) {
		return SEQ_ERROR;
	}

	hSmaf = (PSMAF)pSmaf;
	//----- SMAFf[^ÌæÌú» -----------------------------------------//
	hSmaf->hAllChunk = 0;
	for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++)
		hSmaf->hMtrChunk[trackno] = 0;
	for (trackno=0; trackno<MAX_PCM_TRACK_NUM; trackno++)
		hSmaf->hAtrChunk[trackno] = 0;
	for (trackno=0; trackno<MAX_GRAPHIC_TRACK_NUM; trackno++)
		hSmaf->hGtrChunk[trackno] = 0;
	for (trackno=0; trackno<MAX_MASTER_TRACK_NUM; trackno++)
		hSmaf->hMstrChunk[trackno] = 0;
	hSmaf->MtrNum = 0;
	hSmaf->AtrNum = 0;
	hSmaf->GtrNum = 0;
	hSmaf->MstrNum = 0;
	hSmaf->isMa2 = 1;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Smaf_GetSmafResInfo(void *pSmaf, SMAFRESINFO *pInfo)
//
//  Description: SMAFf[^ÌÄ¶ÉKvÈ\[Xîñð¾é
//  Parameter:
//  					pSmaf			·ÅÉLoadµ½SMAFf[^îñÖÌ|C^
//  					pInfo			KvÈ\[Xîñði[·éÌæÖÌ|C^
//  Return :	-1		G[
//  					 0		³íI¹
//==========================================================================
int Smaf_GetSmafResInfo(void *pSmaf, SMAFRESINFO *pInfo)
{
	PSMAF hSmaf;
	UINT8 *resType;
	UINT8 opSize;

	hSmaf = (PSMAF)pSmaf;
	
	if (hSmaf->MtrNum) {
		GetSmafOptionData(0x4D32, &resType, &opSize);
		if (opSize != 1)
			pInfo->FmChMap = 0x0000FFFF; // OptionÉwèªÈ¢
		else {
			switch (*resType) {
				case 0x00:
					pInfo->FmChMap = 0x00000000;
					break;
				case 0x01:
					pInfo->FmChMap = 0x0000000F;
					break;
				case 0x02:
					pInfo->FmChMap = 0x000000FF;
					break;
				case 0x03:
					pInfo->FmChMap = 0x00000FFF;
					break;
				case 0x04:
					pInfo->FmChMap = 0x0000FFFF;
					break;
				default:
					pInfo->FmChMap = 0x0000FFFF;	// OptionÝèlª³µ­È¢
			}
		}
	}
	else {
		pInfo->FmChMap = 0x00000000;
	}
	
	pInfo->AdpcmChMap = (UINT32)(hSmaf->AtrNum);

	
	return SEQ_SUCCEED;
}



//==========================================================================
//  int Smaf_Open(void *pSmaf, int openMode, int (*SmafCBProc)(SMAFCBEVTYPE event, void *prm))
//
//  Description: SMAFV[PTÌI[v
//  Parameter: 
//  					pSmaf			SMAFf[^ÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  					openMode	@\g£p (0)
//						SmafCBProc SMAF-APIÄÑoµ¤ÖÌR[obNÖÖÌ|C^
//  Return :	NULL			G[
//  					!= NULL		V[PTf[^nh SMAFSEQINF\¢ÌÖÌ|C^
//==========================================================================
int Smaf_Open(void *pSmaf, int openMode, int (*SmafCBProc)(SMAFCBEVTYPE event, void *prm))
{
	PSMAFSEQINFO hSeqInf;
	PSMAF hSmaf;
	UINT8 seqinfno=0;
      
	//----- SMAFîñªÈ¢êÍG[ðÔ· ---------------------------//
	if (pSmaf == 0) {
		return NULL;
	}

	//----- SMAFSEQINF\¢ÌðmÛ -------------------------------//
	while ((seqinfno < MAX_SMAF_DATA_NUM) && (SeqInf[seqinfno].pSmaf != 0))
		seqinfno++;

	if (seqinfno >= MAX_SMAF_DATA_NUM) {
		return NULL;
	}
	else
		hSeqInf = &SeqInf[seqinfno];

	hSmaf = (PSMAF)pSmaf;
	hSeqInf->pSmaf = hSmaf;
	
	//----- R[obNÖÌo^ -------------------------------------//
	hSeqInf->pSmafCBProc = SmafCBProc;

	//----- SoundnÌOpen (SCORE/AUDIOgbNª¶Ý·êÎj--------//
	if (hSmaf->MtrNum + hSmaf->AtrNum) {
		hSeqInf->hSoundInf = Sound_Open(hSeqInf);
		if (hSeqInf->hSoundInf == 0) {
			hSeqInf->pSmaf = 0;
			hSeqInf->pSmafCBProc = NULL;
			hSeqInf->hSoundInf = -1;
			return NULL;    
		}
	}

	//----- àóÔðOPENEDÖÚs -------------------------------------//
	hSeqInf->stateSmaf = SMAF_OPENED;      
	
	//----- SMAFîñði[µ½SMAFSEQINFO\¢ÌÌAhXð ------------//
	//----- V[PTf[^nhÆ·é                  ------------//
	return (int)(hSeqInf);
}


//==========================================================================
//  int Smaf_Close(int seqHd)
//
//  Description: SMAFV[PTÌN[Y
//  Parameter: 
//  					seqHd			Smaf_Open()ÅÔµ½V[PTf[^nh
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Smaf_Close(int seqHd)
{
	PSMAFSEQINFO hSeqInf;
	PSMAF hSmaf;

	//----- V[PXf[^nhªNULLÈçI¹ ---------------------//
	if (seqHd == 0)
		return SEQ_SUCCEED;

	//----- SMAF\¢ÌÌnhðZbg -------------------------------//
	hSeqInf = (PSMAFSEQINFO)seqHd;
	hSmaf = hSeqInf->pSmaf;

	//----- Xe[^XªOPENED é¢ÍREADYÅÈ¯êÎG[ðÔ· ------//
	if ((hSeqInf->stateSmaf != SMAF_OPENED) 
				&& (hSeqInf->stateSmaf != SMAF_READY)) {
		return SEQ_ERROR;
	}

	//----- SoundnÌClose (SCORE/AUDIOgbNª¶Ý·êÎj-------//
	if (hSmaf->MtrNum + hSmaf->AtrNum) {
		if (Sound_Close(hSeqInf->hSoundInf) == SEQ_ERROR) {
			return SEQ_ERROR;
		}
	}
	
	//----- Smaf_Open()ÅmÛµ½SMAFSEQINF\¢ÌÌÌæððú ----------//
	hSeqInf->pSmaf = 0;
	hSeqInf->playingSeq = 0;
	hSeqInf->playMode = 0;
	hSeqInf->playTimes = 0;
	hSeqInf->pSmafCBProc = NULL;
	hSeqInf->hSoundInf = -1;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Smaf_Standby(int seqHd, int pos)
//
//  Description: SMAFÄ¶Ìõ
//  Parameter: 
//  					seqHd			Smaf_Open()ÅÔµ½V[PTf[^nh
//  					pos				Ä¶JnÊu(msec)
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Smaf_Standby(int seqHd, int pos)
{
	PSMAFSEQINFO hSeqInf;
	PSMAF hSmaf;

	//----- V[PXf[^nhªNULLÈçI¹ ---------------------//
	if (seqHd == 0) {
		return SEQ_ERROR;
	}

	//----- SMAF\¢ÌÌnhðZbg -------------------------------//
	hSeqInf = (PSMAFSEQINFO)seqHd;
	hSmaf = hSeqInf->pSmaf;

	//----- Xe[^XªOPENED é¢ÍREADYÅÈ¯êÎG[ðÔ· -//
	if ((hSeqInf->stateSmaf != SMAF_OPENED) 
				&& (hSeqInf->stateSmaf != SMAF_READY)) {
		return SEQ_ERROR;
	}

	//----- SoundnÌStandby (SCORE/AUDIOgbNª¶Ý·êÎj-----//
	if (hSmaf->MtrNum + hSmaf->AtrNum) {
		if (Sound_Standby(hSeqInf->hSoundInf) == SEQ_ERROR) {
			return SEQ_ERROR;
		}
	}

	//----- wè³ê½Ä¶JnÊuÜÅSeek -----------------------------//
	if (pos) {
		if (hSmaf->MtrNum + hSmaf->AtrNum) {
			if (Sound_Seek(hSeqInf->hSoundInf, pos)) {
				return SEQ_ERROR;
			}
		} 
	}
	
	//----- àóÔðREADYÖÚs --------------------------------------//
	hSeqInf->stateSmaf = SMAF_READY;
	
	return SEQ_SUCCEED;
}


//==========================================================================
//  int Smaf_Start(int seqHd, int pMode)
//
//  Description: SMAFÄ¶X^[g
//  Parameter: 
//  					seqHd			Smaf_Open()ÅÔµ½V[PTf[^nh
//  					pMode			Ä¶[h
//  										bit 0-7:	0 ³À[vÄ¶
//  															1-255 Ä¶ñ
//  										bit 8-31:	Reserved (0)
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Smaf_Start(int seqHd, int pMode)
{
	PSMAFSEQINFO hSeqInf;
	PSMAF hSmaf;  
	
	//----- V[PXf[^nhªNULLÈçI¹ ---------------------//
	if (seqHd == 0) {
		return SEQ_ERROR;
	}

	//----- SMAF\¢ÌÌnhðZbg -------------------------------//
	hSeqInf = (PSMAFSEQINFO)seqHd;
	hSmaf = hSeqInf->pSmaf;

	//----- Xe[^XªREADYÅÈ¯êÎG[ðÔ· --------------------//
	if (hSeqInf->stateSmaf != SMAF_READY) {
		return SEQ_ERROR;
	}

	//----- àóÔðPLAYINGÖÚs ------------------------------------//
	hSeqInf->stateSmaf = SMAF_PLAYING;
	
	//----- Ä¶[hðo^ -------------------------------------------//
	if (pMode > 255)
		pMode = 255;
	hSeqInf->playMode = pMode;

	//----- SoundnÌStart (SCORE/AUDIOgbNª¶Ý·êÎj-------//
	if (hSmaf->MtrNum + hSmaf->AtrNum) {
		//-- Ä¶tOð§Äé ------------------------//
		if (hSmaf->MtrNum)	// Score TrackÄ¶
			hSeqInf->playingSeq |= PLAYER_SCORE;
		if (hSmaf->AtrNum)	// PCM Audio TrackÄ¶
			hSeqInf->playingSeq |= PLAYER_PCM;

		if (Sound_Start(hSeqInf->hSoundInf) == SEQ_ERROR) {
			hSeqInf->playingSeq = 0;
			hSeqInf->stateSmaf = SMAF_READY;

			return SEQ_ERROR;
		}
	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Smaf_Stop(int seqHd)
//
//  Description: SMAFÄ¶Xgbv
//  Parameter: 
//  					seqHd			Smaf_Open()ÅÔµ½V[PTf[^nh
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Smaf_Stop(int seqHd)
{
	PSMAFSEQINFO hSeqInf;
	PSMAF hSmaf;

	//----- V[PXf[^nhªNULLÈçI¹ ---------------------//
	if (seqHd == 0) {
		return SEQ_ERROR;
	}

	//----- e\¢ÌÌnhðZbg ---------------------------------//
	hSeqInf = (PSMAFSEQINFO)seqHd;
	hSmaf = hSeqInf->pSmaf;

	//----- Xe[^XªPLAYINGÅÈ¯êÎI¹ ------------------//
	if (hSeqInf->stateSmaf != SMAF_PLAYING) {
		return SEQ_SUCCEED;
	}

	//----- Ä¶[vñÌú» -------------------------------------------//
	hSeqInf->playTimes = 0;

	//----- SoundnÌStop (SCORE/AUDIOgbNª¶Ý·êÎj--------//
	if (hSmaf->MtrNum + hSmaf->AtrNum) {
		if (Sound_Stop(hSeqInf->hSoundInf) == SEQ_ERROR) {
			return SEQ_ERROR;
		}
	}
	
	//-- Ä¶tOðNA ------------------------//
	hSeqInf->playingSeq = 0;

	//----- àóÔðOPENEDÖÚs --------------------------------------//
	hSeqInf->stateSmaf = SMAF_OPENED;

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Smaf_Seek(int seqHd, int pos)
//
//  Description: SMAFÄ¶JnÊuÌwè
//  							Release1ÅÍpos=0ÌÝT|[g
//  Parameter: 
//  					seqHd			Smaf_Open()ÅÔµ½V[PTf[^nh
//  					pos				ÌtÌJnÊu (msec)
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Smaf_Seek(int seqHd, int pos)
{
	PSMAFSEQINFO hSeqInf;
	PSMAF hSmaf;

	//----- V[PXf[^nhªNULLÈçI¹ ---------------------//
	if (seqHd == 0) {
		return SEQ_ERROR;
	}

	//----- e\¢ÌÌnhðZbg ---------------------------------//
	hSeqInf = (PSMAFSEQINFO)seqHd;
	hSmaf = hSeqInf->pSmaf;
	
	//----- Xe[^XªREADYÅÈ¯êÎG[ðÔ· --------------------//
	if (hSeqInf->stateSmaf != SMAF_READY) {
		return SEQ_ERROR;
	}

	//----- SoundnÌSeek (SCORE/AUDIOgbNª¶Ý·êÎj--------//
	if (hSmaf->MtrNum + hSmaf->AtrNum) {
		if (Sound_Seek(hSeqInf->hSoundInf, pos) == SEQ_ERROR) {
			return SEQ_ERROR;
		}
	}
	
	return SEQ_SUCCEED;
}


//==========================================================================
//  int Smaf_Control(int seqHd, int ctrlNum, void *prm)
//
//  Description: SMAFV[PTàÏÌÏXAQÆ
//  Parameter: 
//  					seqHd			Smaf_Open()ÅÔµ½V[PTf[^nh
//  					ctrlNum		àe
//  					prm				ctrlNumÅwè³ê½ÉKvÈp[^
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Smaf_Control(int seqHd, int ctrlNum, void *prm)
{           
	PSMAFSEQINFO hSeqInf;
	
	hSeqInf = (PSMAFSEQINFO)seqHd;

	// V[PXf[^nhªNULLÈçI¹ //
	if (seqHd == 0) {
		return SEQ_ERROR;
	}
		
	if (!(ctrlNum & 0xFF00)) {
		if (Sound_Control(hSeqInf->hSoundInf, ctrlNum, prm) == SEQ_ERROR) {
			return SEQ_ERROR;
		}
	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int PlayEnd(int player)
//
//  Description: SMAFV[PTÄ¶I¹Ç
//  Parameter: 
//  					player		I¹µ½Ä¶n
//  											1 : Score Track
//  											2 : PCM Audio Track
//  											4 : Graphics Track
//  											8 : Master Track
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PlayEnd(int seqHd, int player)
{     
	PSMAFSEQINFO hSeqInf;     
	UINT8 playRepeat;       


	//----- e\¢ÌÌnhðZbg ---------------------------------//
	hSeqInf = (PSMAFSEQINFO)seqHd;
	
	//----- wè³ê½Ä¶nÌÄ¶tOðæèÁ· -------------------//
	hSeqInf->playingSeq &= ~player;
	
	//----- ·×ÄÌÄ¶ªI¹ÈçÌÄ¶ÌX^oC -----------------//
	if (!hSeqInf->playingSeq) {
		
		//-- àóÔðREADYÖÚs -----------------------//
		hSeqInf->stateSmaf = SMAF_READY;

		hSeqInf->playTimes++;
		if ((hSeqInf->playMode == PLAYMODE_LOOP) 
				|| (hSeqInf->playMode - hSeqInf->playTimes))
			playRepeat = 1;
		else
			playRepeat = 0;

		//-- FWp@Ä¶I¹Êm --//
		hSeqInf->pSmafCBProc(SMAF_PLAYEND, 0);

		//-- Ä¶[hª[vÈçÎÄ¶X^[g --------//
		if (playRepeat) {
			//-- Ä¶JnÊuðStartPointÖ ----------------------//
			if (Sound_SeekStartPoint(hSeqInf->hSoundInf, 0) == SEQ_ERROR) {
				hSeqInf->pSmafCBProc(SMAF_ERROR, 0);
				return SEQ_ERROR;
			}
			//-- Ä¶X^[g ------------------------------//
			if (Smaf_Start((int)hSeqInf, hSeqInf->playMode) == SEQ_ERROR) {
				hSeqInf->pSmafCBProc(SMAF_ERROR, 0);
				return SEQ_ERROR;
			}
		}
		else {
			hSeqInf->playTimes = 0;
			if(!SKY_IS_IN_GVM())
				rex_set_timer(&MUSIC_music_timer, (dword)10);
			else
				MUSIC_music_stop();
		}
	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Smaf_GetStatus(int seqHd)
//
//  Description: SMAFV[PTÌàóÔðÔ·
//  Parameter:
//  					seqHd			Smaf_Open()ÅÔµ½V[PTf[^nh
//  Return :	SMAFV[PTÌàóÔ
//==========================================================================
int Smaf_GetStatus(int seqHd)
{             
	PSMAFSEQINFO hSeqInf;
	
	//----- e\¢ÌÌnhðZbg ---------------------------------//
	hSeqInf = (PSMAFSEQINFO)seqHd;
	
	return hSeqInf->stateSmaf;
}

//smafseq.c
//soundseq.c

//==========================================================================
//  int Sound_Initialize()
//
//  Description: SMAFV[PTSoundÌú»
//  Parameter:
//  Return :
//==========================================================================
int Sound_Initialize(void)
{
	UINT8 seqinfno;
	
  //----- SOUNDSEQINFO\¢ÌÌú» ---------------------------------//
  for (seqinfno=0; seqinfno<MAX_SMAF_DATA_NUM; seqinfno++) {
	  SoundSeqInf[seqinfno].hSeqInf = 0;
	  SoundSeqInf[seqinfno].pSmaf = 0;
		SoundSeqInf[seqinfno].playingMtrTrack = 0;
		SoundSeqInf[seqinfno].reqFmCh = 0;
		SoundSeqInf[seqinfno].reqAdpcmCh = 0;
	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Sound_Open(PSMAFSEQINFO pSeqInf)
//
//  Description: SMAFV[PTSoundÌI[v
//  Parameter: 
//  					pSeqInf			V[PTàÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  Return :	0	G[
//  					!=0	Soundàf[^ÌæÖÌ|C^
//==========================================================================
UINT32 Sound_Open(PSMAFSEQINFO pSeqInf)
{
	PSOUNDSEQINFO hSoundSeq;
	PSMAF pSmaf;
	UINT8 *resType;
	UINT8 opSize;
	UINT32 fmChMap;
	UINT8 adpcmChMap;
	UINT8 seqinfno=0;
	UINT32 usedLoopDataSize;
	int retval;   
	
  //----- SOUNDSEQINFO\¢ÌðmÛ -------------------------------//
	while ((seqinfno < MAX_SMAF_DATA_NUM) && (SoundSeqInf[seqinfno].hSeqInf != 0))
		seqinfno++;

	if (seqinfno >= MAX_SMAF_DATA_NUM)
		return 0;
	else
		hSoundSeq = &SoundSeqInf[seqinfno];

  //----- MA2V[PThCoÌú» ------------------------------//
  Seq_Initialize();     

	//----- Openµ½V[PXf[^nhÌîñðo^ ---------------//
	pSmaf = pSeqInf->pSmaf;

	hSoundSeq->hSeqInf = pSeqInf;  
	hSoundSeq->pSmaf = pSeqInf->pSmaf;

	//----- Score Track Chunkª¶Ý·éÈçFMV[PTðOpen·é ------//
	if (pSmaf->MtrNum) {
		
		//-- Ä¶ÉKvÈFM`l\[XÌmÛ ------//
		GetSmafOptionData(0x4D32, &resType, &opSize);
		if (opSize != 1) {
			fmChMap = 0x0000FFFF; // OptionÉwèªÈ¢
		}
		else {
			switch (*resType) {
				case 0x00:	// Type0
					fmChMap = 0x00000000;
					break;
				case 0x01:	// Type1
					fmChMap = 0x0000000F;
					break;
				case 0x02:	// Type2
					fmChMap = 0x000000FF;
					break;
				case 0x03:	// Type3
					fmChMap = 0x00000FFF;
					break;
				case 0x04:	// Type4
					fmChMap = 0x0000FFFF;
					break;
				default:
					fmChMap = 0x0000FFFF;	// OptionÝèlª³µ­È¢
			}
		}
		if (MA2Manager_AllocFmCh(fmChMap) == -1) {
			hSoundSeq->hSeqInf = 0;
			hSoundSeq->pSmaf = 0;
			return 0;
		}
		else {
			hSoundSeq->reqFmCh = fmChMap;
		}
	
		//-- FMV[PTðOpen·é ----------------------//
		retval = FmSeq_Open(FmFifo_Proc, pSmaf->MtrNum);
		if (retval == SEQ_ERROR) {
			hSoundSeq->hSeqInf = 0;
			hSoundSeq->pSmaf = 0;
			MA2Manager_FreeFmCh(hSoundSeq->reqFmCh);
			hSoundSeq->reqFmCh = 0;
			return 0;
		}

		//-- [vÄ¶ÉÝè·éúRg[CxglÌo --//
		usedLoopDataSize = 0;
		LoopDataSize = 0;
		if (!pSmaf->isMa2) {
			// MA-1pSMAFf[^ //
			pLoopDataBuf= &LoopDataBuf[0];
			if (pSmaf->hMtrChunk[0]->NopDuration != 0) {
				usedLoopDataSize += WriteDuration(pLoopDataBuf, pSmaf->hMtrChunk[0]->NopDuration);
				LoopDataBuf[usedLoopDataSize++] = 0xff;
				LoopDataBuf[usedLoopDataSize++] = 0x00;
			}
			usedLoopDataSize += SmafScoreTrackControlSeek(0, 
					pSmaf->hMtrChunk[0]->pSequenceDataChunk+pSmaf->hMtrChunk[0]->SeekPhraseInfo.dwStartPoint,
					pLoopDataBuf+usedLoopDataSize, 0);
			LoopDataSize = usedLoopDataSize;
#if 0
			DBG_put8hex(" LoopDataSize:", LoopDataSize);
			DBG_newline();
#endif
		}
	}

	//----- PCM Audio Track Chunkª¶Ý·éÈçPCMV[PTðOpen·é -//
	if (pSmaf->AtrNum) {

		//-- Ä¶ÉKvÈFM`l\[XÌmÛ ------//
		adpcmChMap = pSmaf->AtrNum;
		if (MA2Manager_AllocAdpcmCh(adpcmChMap) == -1) {
			hSoundSeq->hSeqInf = 0;
			hSoundSeq->pSmaf = 0;
			MA2Manager_FreeFmCh(hSoundSeq->reqFmCh);
			hSoundSeq->reqFmCh = 0;
			return 0;
		}
		else {
			hSoundSeq->reqAdpcmCh = adpcmChMap;
		}

		//-- PCMV[PTðOpen·é ---------------------//
		retval = PcmSeq_Open(PcmFifo_Proc);
		if (retval == SEQ_ERROR) {
			hSoundSeq->hSeqInf = 0;
			hSoundSeq->pSmaf = 0;
			MA2Manager_FreeFmCh(hSoundSeq->reqFmCh);
			hSoundSeq->reqFmCh = 0;
			MA2Manager_FreeAdpcmCh(hSoundSeq->reqAdpcmCh);
			hSoundSeq->reqAdpcmCh = 0;
			return 0;
		}
	}
	
	return (UINT32)hSoundSeq;
}


//==========================================================================
//  int Sound_Close(int pSoundSeq)
//
//  Description: SMAFV[PTSoundÌN[Y
//  Parameter: 
//  					pSoundSeq	TEhÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Sound_Close(int pSoundSeq)
{
	PSOUNDSEQINFO hSoundSeq;
	int stateMa2;
	int retval=SEQ_SUCCEED; 
	
	hSoundSeq = (PSOUNDSEQINFO)pSoundSeq;

	// MA2 FMV[PTªOPNEDED é¢ÍREADYÈçClose·é //
	stateMa2 = GetFmSeqState();
	if ((stateMa2 == FM_OPENED) || (stateMa2 == FM_READY)) {
		retval = FmSeq_Close();
	}

	// MA2 PCMV[PTªOPNEDED é¢ÍREADYÈçClose·é //
	stateMa2 = GetPcmSeqState();
	if ((stateMa2 == PCM_OPENED) || (stateMa2 == PCM_READY)) {
		retval = PcmSeq_Close();
	}
	
	
	//----- Sound_Open()ÅmÛµ½\[XÌðú -----------------------//
	if (hSoundSeq->reqFmCh)
		MA2Manager_FreeFmCh(hSoundSeq->reqFmCh);
	if (hSoundSeq->reqAdpcmCh)
		MA2Manager_FreeAdpcmCh(hSoundSeq->reqAdpcmCh);
	
	//----- Sound_Open()ÅmÛµ½SOUNDSEQINFO\¢ÌÌÌæÌðú -----//
	hSoundSeq->hSeqInf = 0;
	hSoundSeq->pSmaf = 0;
	hSoundSeq->playingMtrTrack = 0;
	hSoundSeq->reqFmCh = 0;
	hSoundSeq->reqAdpcmCh = 0;

	return retval;
}


//==========================================================================
//  int Sound_Standby(int pSoundSeq)
//
//  Description: SMAFÄ¶Ìõ
//  Parameter: 
//  					pSoundSeq	TEhÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Sound_Standby(int pSoundSeq)
{
	PSOUNDSEQINFO hSoundSeq;
	PSMAF pSmaf;
	FMSEQINIDATA fmi;
	PCMSEQINIDATA pci;
//	UINT16 chstat;
//	UINT8 vbval;
	UINT8 wavFreq;
	UINT8 trackno=0;
	UINT8 chno;
	int retval;                        
	
	hSoundSeq = (PSOUNDSEQINFO)pSoundSeq;
	pSmaf = hSoundSeq->pSmaf;
	
	seqhandle = hSoundSeq;
	
	//----- MA-2 V[PTÆFIFOÌZbg ----------------------------//
	Seq_Reset();
	if (pSmaf->MtrNum)
		FmSeq_FifoClear();
	if (pSmaf->AtrNum)
		PcmSeq_FifoClear();

	//----- Score Track Chunkª¶Ý·éÈçFMV[PTStandbyðÀs ---//
	if (pSmaf->MtrNum) {
		//-- MA-2 FM`lRg[NA ------------------//
		for (chno=0; chno<MAX_FM_CHANNEL_NUM; chno++) {
			if (hSoundSeq->reqFmCh & (0x01L << chno)) {
				Ma2_fm_ch_reset(chno);
			}
		}

		//-- KvÈlðMA2V[PThCoÖn·øÉãü ----//
		// ÅÌgbNÌTimebaseðãü                        //
		while (!(pSmaf->MtrNum & (0x01 << trackno))) {
			trackno++;
		}
		fmi.dTimebaseD = pSmaf->hMtrChunk[trackno]->dwTimeBaseD;
		fmi.dTimebaseG = pSmaf->hMtrChunk[trackno]->dwTimeBaseG;
		// MA2Ì­¹`lðãü                             //
		fmi.reqFmCh = hSoundSeq->reqFmCh;
		
/*
		//-- VibratorÌÝè ------------------------------//
		vbval = 0;
		for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++) {
			if (pSmaf->MtrNum & (0x01 << trackno)) {
				chstat = pSmaf->hMtrChunk[trackno]->wChannelStatus;
				switch (chstat & 0x4444) {
					case 0x0000:
						continue;
					case 0x4444:
						vbval = (UINT8)(0x0c + trackno);
						break;
					default:
						if (chstat & 0x4000) vbval = (UINT8)(trackno * 4);
						else if (chstat & 0x0400) vbval = (UINT8)(trackno * 4 + 1);
						else if (chstat & 0x0040) vbval = (UINT8)(trackno * 4 + 2);
						else if (chstat & 0x0004) vbval = (UINT8)(trackno * 4 + 3);
				}
			}
			break;
		}
		Ma2w_motor_control(0x02, vbval); // t¯ú[h1Ìê //
*/

		//-- FMV[PTStandby -------------------------//
		retval = FmSeq_Standby(&fmi, (void *)hSoundSeq);
		if (retval == SEQ_ERROR)
			return SEQ_ERROR;
			
	}
	
	//----- PCM Audio Track Chunkª¶Ý·éÈçPCMV[PTStandbyðÀs //
	if (pSmaf->AtrNum) {
		//-- MA-2 ADPCM`lRg[NA ------------------//
		for (chno=0; chno<MAX_PCM_CHANNEL_NUM; chno++) {
			if (hSoundSeq->reqAdpcmCh & (0x01 << chno)) {
				Ma2_adpcm_ch_reset();
			}
		}
		//-- TimebaselAg`ÌFSðMA2V[PThCoÖn·øÉãü --//
		// ÅÌgbNÌTimebaseðÌp                        //
		trackno = 0;
		while (!(pSmaf->AtrNum & (0x01 << trackno))) {
			trackno++;
		}
		pci.dTimebaseD = pSmaf->hAtrChunk[trackno]->dwTimeBaseD;
		pci.dTimebaseG = pSmaf->hAtrChunk[trackno]->dwTimeBaseG;
		pci.reqAdpcmCh = hSoundSeq->reqAdpcmCh;

		// g`ÌTvOügÝè //
		wavFreq = (UINT8)((pSmaf->hAtrChunk[trackno]->wWaveType & 0x0F00) >> 8);
		switch (wavFreq) {
			case 0:		// 4kHz //
				pci.waveFS = 0;
				break;
			case 1:		// 8kHz //
				pci.waveFS = 1;
				break;
		}

		//-- PCMV[PTStandby-------------------------//
		retval = PcmSeq_Standby(&pci, (void *)hSoundSeq);
		if (retval == SEQ_ERROR)
			return SEQ_ERROR;
	}
	
	//-- StartPointÖÌSeek ------------------------//
	// StartPointÜÅÌControl Message              //
	// StartPoint©çÌSequence DataÌFIFO«±Ý      //
	Sound_SeekStartPoint(pSoundSeq, 1);
	
	return SEQ_SUCCEED;
}


//==========================================================================
//  int Sound_Start(int pSoundSeq)
//
//  Description: SMAFÄ¶X^[g
//  Parameter: 
//  					pSoundSeq	TEhÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Sound_Start(int pSoundSeq)
{
	PSOUNDSEQINFO hSoundSeq;
	PSMAF pSmaf;
	UINT8 chno; 
	UINT8 trackno; 
	int retval;
    
	hSoundSeq = (PSOUNDSEQINFO)pSoundSeq;
	pSmaf = hSoundSeq->pSmaf;

	//-- LoopDataI¹tOÌú» --------------------------------//	
	for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++)
		fgLoopData = 1;
	
	//----- MA2V[PTStart -----------------------------------------//
	
	//-- MA-1f[^ÌêÌ{[_E ------------------//
	if (pSmaf->MtrNum && !pSmaf->isMa2) {
		for (chno=0; chno<MAX_FM_CHANNEL_NUM; chno++) {
			if (hSoundSeq->reqFmCh & (0x01L << chno)) {
				Ma2w_fm_expression(chno, 0x12); // Expression -9dB //
				Ma2w_fm_channel_pan(chno, 0x40);// Pan center -3dB //
			}
		}
	}

	//-- Score TrackAPCM Audio TrackÆà¶Ý·éê --//
	if ((pSmaf->MtrNum) && (pSmaf->AtrNum)) {
		retval = Seq_Start();
		if (retval == SEQ_ERROR)
			return SEQ_ERROR;
		hSoundSeq->playingMtrTrack = pSmaf->MtrNum;
	}
	else {
		
		//-- Score TrackÌÝÌê -----------------------//
		if (pSmaf->MtrNum) {
			retval = FmSeq_Start();
			if (retval == SEQ_ERROR)
				return SEQ_ERROR;
			hSoundSeq->playingMtrTrack = pSmaf->MtrNum;
		}
		
		//-- PCM Audio TrackÌÝÌê -------------------//
		if (pSmaf->AtrNum) {
			retval = PcmSeq_Start();
			if (retval == SEQ_ERROR)
				return SEQ_ERROR;
		}
	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Sound_Stop(int pSoundSeq)
//
//  Description: SMAFÄ¶Xgbv
//  Parameter: 
//  					pSoundSeq	TEhÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Sound_Stop(int pSoundSeq)
{
	PSOUNDSEQINFO hSoundSeq;
	PSMAF pSmaf;
	int retval=SEQ_SUCCEED;

	hSoundSeq = (PSOUNDSEQINFO)pSoundSeq;
	pSmaf = hSoundSeq->pSmaf;

	//----- MA2V[PTStop ------------------------------------------//

	//-- Score TrackAPCM Audio TrackÆà¶Ý·éê --//
	if ((pSmaf->MtrNum) && (pSmaf->AtrNum)) {
		retval = Seq_Stop();
		hSoundSeq->playingMtrTrack = 0;
	}
	else {
		
		//-- Score TrackÌÝÌê -----------------------//
		if (pSmaf->MtrNum) {
			retval = FmSeq_Stop();
			hSoundSeq->playingMtrTrack = 0;
		}
		
		//-- PCM Audio TrackÌÝÌê -------------------//
		if (pSmaf->AtrNum) {
			retval = PcmSeq_Stop();
		}
	}
	
	return retval;
}


//==========================================================================
//  int Sound_Seek(int pSoundSeq, int pos)
//
//  Description: SMAFÄ¶Xgbv
//  Parameter: 
//  					pSoundSeq	TEhÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  					pos				ÌtÌJnÊu (msec)
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Sound_Seek(int pSoundSeq, int pos)
{
	PSOUNDSEQINFO hSoundSeq;
	PSMAF pSmaf;
	UINT8 trackno;
	UINT8 *FmSeekPoint[MAX_SCORE_TRACK_NUM];
	UINT32 FmSeekDuration[MAX_SCORE_TRACK_NUM];
	UINT8 *AdpcmSeekPoint[MAX_PCM_TRACK_NUM];
	UINT32 AdpcmSeekDuration[MAX_PCM_TRACK_NUM];
	UINT16 CtrlDataSize;
	DATABLK BufData;
	struct SCORE_TRACK_CHUNK *hMtrInf;
	struct PCM_AUDIO_TRACK_CHUNK *hAtrInf;
	UINT32 SeekTime;
	UINT32 startpoint;
	int retval;

	hSoundSeq = (PSOUNDSEQINFO)pSoundSeq;
	pSmaf = hSoundSeq->pSmaf;
	
	if (pSmaf->MtrNum) {
		FmSeq_FifoClear();
	}
	if (pSmaf->AtrNum) {
		PcmSeq_FifoClear();
	}

	
	//----- æª(Start Point)ÖÌSeek -----------------------------------------------//
	if (pos == 0) {

		Sound_SeekStartPoint(pSoundSeq, 1);

		return SEQ_SUCCEED;
	}

	//----- æªÈOÖÌSeek -------------------------------------------//
	if (pSmaf->MtrNum) {
		for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++) {
			if (pSmaf->MtrNum & (0x01 << trackno)) {
				hMtrInf = pSmaf->hMtrChunk[trackno];
				if (!hMtrInf->bSequenceType) { // Stream Sequence //
					startpoint = hMtrInf->SeekPhraseInfo.dwStartPoint;
				}
				else { // Sub-Sequence //
					return SEQ_ERROR;
				}

				//----- SeekÔðTimebasePÊÉvZ -----//
				SeekTime = pos / hMtrInf->dwTimeBaseD;
				//----- Sequence dataÌSeekÊuðßé -----//
				if (SmafScoreTrackSeek((UINT8)(trackno+pSmaf->isMa2), SeekTime, &FmSeekPoint[trackno], &FmSeekDuration[trackno])) {
					//----- SeekÊuÜÅÌRg[bZ[Wðo -----//
					CtrlDataSize = SmafScoreTrackControlSeek((UINT8)(trackno+pSmaf->isMa2), 
																										FmSeekPoint[trackno], &CtrlDataBuf[0], 1);
					//----- oµ½Rg[bZ[Wð -----//
					retval = FmSeq_ControlSeek(trackno, &CtrlDataBuf[0], CtrlDataSize);
					if (retval == SEQ_ERROR)
						return SEQ_ERROR;
				}
			}
		}
	}

	if (pSmaf->AtrNum) {
		for (trackno=0; trackno<MAX_PCM_TRACK_NUM; trackno++) {
			if (pSmaf->AtrNum & (0x01 << trackno)) {
				hAtrInf = pSmaf->hAtrChunk[trackno];
				if (!hAtrInf->bSequenceType) { // Stream Sequence //
					startpoint = hAtrInf->SeekPhraseInfo.dwStartPoint;
				}
				else { // Sub-Sequence //
					return SEQ_ERROR;
				}

				//----- SeekÔðTimebasePÊÉvZ -----//
				SeekTime = pos / hAtrInf->dwTimeBaseD;
				//----- Sequence dataÌSeekÊuðßé -----//
				if (SmafAudioTrackSeek(trackno, SeekTime, &AdpcmSeekPoint[trackno], &AdpcmSeekDuration[trackno])) {
					//----- SeekÊuÜÅÌRg[bZ[Wðo -----//
					CtrlDataSize = SmafAudioTrackControlSeek(trackno, 
																										AdpcmSeekPoint[trackno], &CtrlDataBuf[0]);
					//----- oµ½Rg[bZ[Wð -----//
					retval = PcmSeq_ControlSeek(trackno, &CtrlDataBuf[0], CtrlDataSize);
					if (retval == SEQ_ERROR) {
						return SEQ_ERROR;
					}
				}
			}
		}
	}
	
	//----- gpµÄ¢égbNÉÂ¢ÄSeek -----//
	if (pSmaf->MtrNum) {
		for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++) {
			if (pSmaf->MtrNum & (0x01 << trackno)) {
				hMtrInf = pSmaf->hMtrChunk[trackno];
				BufData.pBuf = FmSeekPoint[trackno];
				if(!hMtrInf->bSequenceType) { // StreamSequence //
					if (hMtrInf->SeekPhraseInfo.dwStartPoint != hMtrInf->SeekPhraseInfo.dwStopPoint) {
						BufData.Size = (UINT32)(hMtrInf->pSequenceDataChunk)+hMtrInf->SeekPhraseInfo.dwStopPoint
																-(UINT32)FmSeekPoint[trackno];
					}
					else {
						BufData.Size = (UINT32)(hMtrInf->pSequenceDataChunk)+hMtrInf->dwSequenceDataChunkSize
																-(UINT32)FmSeekPoint[trackno];
					}
				}
				else { // Sub-Sequence //
					return SEQ_ERROR;
				}
				
				if (FmSeq_Seek(trackno, &BufData, FmSeekDuration[trackno]) == SEQ_ERROR)
					return SEQ_ERROR;
			}
		}
	}
	
	if (pSmaf->AtrNum) {
		for (trackno=0; trackno<MAX_PCM_TRACK_NUM; trackno++) {
			if (pSmaf->AtrNum & (0x01 << trackno)) {
				hAtrInf = pSmaf->hAtrChunk[trackno];
				BufData.pBuf = AdpcmSeekPoint[trackno];
				if(!hAtrInf->bSequenceType) { // StreamSequence //
					if (hAtrInf->SeekPhraseInfo.dwStartPoint != hAtrInf->SeekPhraseInfo.dwStopPoint) {
						BufData.Size = (UINT32)(hAtrInf->pSequenceDataChunk)+hAtrInf->SeekPhraseInfo.dwStopPoint
																-(UINT32)AdpcmSeekPoint[trackno];
					}
					else {
						BufData.Size = (UINT32)(hAtrInf->pSequenceDataChunk)+hAtrInf->dwSequenceDataChunkSize
																-(UINT32)AdpcmSeekPoint[trackno];
					}
				}
				else { // Sub-Sequence //
					return SEQ_ERROR;
				}
				AtsqNextEvent[trackno] = (UINT32)AdpcmSeekPoint[trackno] - (UINT32)(hAtrInf->pSequenceDataChunk);
				AtsqNextEvent[trackno] -= 1; // 2byte DurationÅà-1ÅâèÈµ //
				if (PcmSeq_Seek(trackno, &BufData, AdpcmSeekDuration[trackno]) == SEQ_ERROR)
					return SEQ_ERROR;
			}
		}
	}
	
	return SEQ_SUCCEED;
}



//==========================================================================
//  int Sound_Control(int pSoundSeq, int ctrlNum, void *prm)
//
//  Description: SMAFV[PTSoundÌàÏÌÏXAQÆ
//  Parameter: 
//  					pSoundSeq	TEhÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  					ctrlNum		àe
//  					prm				ctrlNumÅwè³ê½ÉKvÈp[^
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Sound_Control(int pSoundSeq, int ctrlNum, void *prm)
{
	UINT8 prm8;
	UINT8 *pprm8;
	
	switch (ctrlNum) {
		case SMAFCTRL_SET_HPVOL:	// wbhtH{[Ýè
			prm8 = *((UINT8 *)prm);
			if (prm8 > 0x7F)
				prm8 = 0x7F;
			prm8 = (UINT8)(prm8 >> 2);
			Ma2w_hp_vol(prm8, prm8);
			break;
		case SMAFCTRL_SET_SPVOL:	// Xs[J{[Ýè
			prm8 = *((UINT8 *)prm);
			if (prm8 > 0x7F)
				prm8 = 0x7F;
			prm8 = (UINT8)(prm8 >> 2);
			Ma2w_sp_vol(prm8);
			break;
			
		case SMAFCTRL_GET_HPVOL:	// wbhtH{[ÇÝoµ
			pprm8 = *((UINT8 **)prm);
			prm8 = Ma2r_hpr_vol();
			*pprm8 = (UINT8)(prm8 << 2);
			break;
		case SMAFCTRL_GET_SPVOL:	// Xs[J{[ÇÝoµ
			pprm8 = *((UINT8 **)prm);
			prm8 = Ma2r_sp_vol();
			*pprm8 = (UINT8)(prm8 << 2);
			break;
	}
	
	return SEQ_SUCCEED;
}


//==========================================================================
//  int FmFifo_Proc(FMCBEVTYPE event, int fifoNum, DATABLK *pBufData, void *prm)
//
//  Description: MA2à FMV[PTRg[pR[obNÖ
//  Parameter: 
//  					event			ÄÑoµCxg
//  											0:Ä¶X^oC
//  											1:tÌV[PXf[^Ì±«ðv
//  											2:V[PXf[^I¹iVµ¢Ä¶f[^vj
//  					fifoNo		ÎÛÆÈéFIFOÔ [0-3]
//  					pBufData	tf[^îñði[·éÌæÖÌ|C^
//  					prm				FmSeq_Standby()Ìæ2øÅnµ½l
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int FmFifo_Proc(FMCBEVTYPE event, int fifoNo, DATABLK *pBufData, void *prm)
{
	PSOUNDSEQINFO hSoundSeq;
	struct SCORE_TRACK_CHUNK *pMtrInf;
	
  //----- øú» -------------------------------------------------//
	if (pBufData) {
		pBufData->pBuf = 0;
		pBufData->Size = 0;
	}
	
	hSoundSeq = (PSOUNDSEQINFO)prm;
	pMtrInf = hSoundSeq->pSmaf->hMtrChunk[fifoNo];
	//----- wè³ê½FIFOÉÎ·égbNªÈ©Á½çG[ -----//
	if (!(pMtrInf))
		return SEQ_ERROR;

	switch (event) {

		//===== elÄ¶X^oC ===================================//

		case FM_STANDBY:
			//----- Setupf[^v -----------------------//
			pBufData->pBuf = pMtrInf->pSetupDataChunk;
			pBufData->Size = pMtrInf->dwSetupDataChunkSize;

 			break;

		//===== f[^Ì±«ðv =======================================//

		case FMSEQ_DATA_REQUEST:

			//-- MA-1pf[^ÈçúRg[bZ[Wði[µ½obt@ðwè --------//
			if (!hSoundSeq->pSmaf->isMa2) {
				// MA-1p //
				if (fgLoopData) {
					pBufData->pBuf = pLoopDataBuf;
					pBufData->Size = LoopDataSize;
					fgLoopData = 0;
				}
			}
			
			break;

		//===== f[^Empty ==============================================//
		
		case FMSEQ_DATA_EMPTY:

			//----- Ä¶f[^I¹ÌÄÑoµ -----------------------------//
			//-- I¹µ½FIFOÉÎ·égbNÌÄ¶tOðæèÁ· --//
			hSoundSeq->playingMtrTrack &= ~(0x01<<fifoNo);
				
			//-- ·×ÄÌgbNÌÄ¶ªI¹µ½çAScore TrackI¹ðA --//
			if (!hSoundSeq->playingMtrTrack) {
				PlayEnd((int)(hSoundSeq->hSeqInf), PLAYER_SCORE);
			}

			break;

	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int GetNextWave(UINT8 *hbuf, UINT32 bufsize, UINT32 *bufcnt)
//
//  Description: ÉÄ¶·ég`f[^ÔðÔ·
//  Parameter: 
//  					hbuf			V[PXf[^æªÖÌ|C^
//  					bufsize		V[PXf[^ÌTCY
//  					bufcnt		ÏÌV[PXf[^TCY
//  Return :	-1	G[
//  					 0	ÉÄ¶·ég`f[^Ô
//==========================================================================
int GetNextWave(UINT8 *hbuf, UINT32 bufsize, UINT32 *bufcnt)
{
	int waveno;
	UINT16 status, data1, data2;
	int count,youandi;
	
	while (*bufcnt < bufsize-2) {
		
		//----- Durationf[^ðÇÝòÎ· -------------------------------//
		data1 = _mgetc(hbuf, *bufcnt);
		if (data1&0x80)
			data1 = (UINT16)((((data1 & 0x7F) << 7) + (UINT16)_mgetc(hbuf, *bufcnt)) + 128);
			
		status = _mgetc(hbuf, *bufcnt);
		switch (status) {
			
			//-- Control Message ÇÝòÎµ ----------------//
			case 0x00:
				data1 = _mgetc(hbuf, *bufcnt);
				if ((data1&0x30) == 0x30) {
					data2 = _mgetc(hbuf, *bufcnt);
				}
				break;
				
			case 0xFF:
				data1 = _mgetc(hbuf, *bufcnt);
				
				//-- Exclusive Message ÇÝòÎµ ------------//
				if (data1 == 0xF0) {
					data2 = _mgetc(hbuf, *bufcnt);
					for (count=0; count<data2; count++)
					youandi=_mgetc(hbuf, *bufcnt); //youandi 01.04.06 ¿ö´×Àâ±â
					}
					
				//-- NOP ÇÝòÎµ --//
				else {
				}
				break;
				
			//-- Note Message ------------------------------//
			default:
				
				//-- `lª0Å é±ÆðmF -----------//
				if (((status&0xC0)>>6) == 0) {
					waveno = status&0x3F;
					
					//-- Gatetimef[^ðÇÝòÎ· ------------//
					data1 = _mgetc(hbuf, *bufcnt);
					if (data1&0x80) {
						data1 = _mgetc(hbuf, *bufcnt);
					}
					
					//-- g`f[^ÔðÔ· ------------------//
					return waveno;
				}
		}
	}

	//----- G[ðÔ· -------------------------------//
	return SEQ_ERROR; 
}


//==========================================================================
//  int PcmFifo_Proc(PCMCBEVTYPE event, int fifoNo, DATABLK *pBufData, void *prm)
//
//  Description: MA2à PCMV[PTRg[pR[obNÖ
//  Parameter: 
//  					event			ÄÑoµCxg
//  											0:Ä¶X^oC
//  											1:tÌV[PXf[^Ì±«ðv
//  											2:V[PXf[^I¹iVµ¢V[PXf[^vj
//  											3:tÌg`f[^Ì±«ðv
//  											4:g`f[^I¹iVµ¢g`f[^vj
//  											5:g`f[^ÌGatetimeI¹
//  					fifoNo		ÎÛÆÈéFIFOÔ [0]
//  					pBufData	tf[^îñði[·éÌæÖÌ|C^
//  					prm				FmSeq_Standby()Ìæ2øÅnµ½l
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int PcmFifo_Proc(PCMCBEVTYPE event, int fifoNo, DATABLK *pBufData, void *prm)
{
	PSOUNDSEQINFO hSoundSeq;
	struct PCM_AUDIO_TRACK_CHUNK *hAtrInf;
	
	hSoundSeq = (PSOUNDSEQINFO)prm;

  //----- øú» -------------------------------------------------//
	if (pBufData) {
		pBufData->pBuf = 0;
		pBufData->Size = 0;
  }

	//----- FIFOÔ0ÈOÍG[ --------------------------------------//
	if (fifoNo != 0)
		return SEQ_ERROR;

	//----- wè³ê½FIFOÉÎ·égbNªÈ©Á½çG[ ---------//
	hAtrInf = hSoundSeq->pSmaf->hAtrChunk[fifoNo];
	if (!hAtrInf)
		return SEQ_ERROR;
		
	switch (event) {

		//===== PCM AudioÄ¶X^oC ==============================//

		case PCM_STANDBY:

			break;

		//===== V[PXf[^Ì±«ðv =============================//

		case PCMSEQ_DATA_REQUEST:

		break;

		//===== V[PXf[^Empty ====================================//

		case PCMSEQ_DATA_EMPTY:

			//----- V[PXf[^I¹ÌÄÑoµ -----------------------//
			//-- PCM Audio TrackI¹ðA --//
			PlayEnd((int)(hSoundSeq->hSeqInf), PLAYER_PCM);

			break;
			
		//===== g`f[^Ì±«ðv ===================================//

		case PCMWAV_DATA_REQUEST:
		
			break;

		//===== f[^Empty Ìg`f[^v ===========================//

		case PCMWAV_DATA_EMPTY:

		//-- GATE_TIME_UP¯lÌg`f[^ÔÇÝoµ ------//
			
		//===== g`f[^ÌQ[g^Coß Ìg`f[^v ==========//
		case GATE_TIME_UP:

			//----- Ìg`f[^ÔÇÝoµ -----------------------------//
			nextwave = GetNextWave(hAtrInf->pSequenceDataChunk, 
										hAtrInf->dwSequenceDataChunkSize, &(AtsqNextEvent[0]));

			//-- g`f[^ª¶Ý·êÎAf[^ÖÌ|C^ÆTCYðÝè --//
			if (nextwave != -1) {
				pBufData->pBuf = hAtrInf->pWaveDataChunk[nextwave];
				pBufData->Size = hAtrInf->dwWaveDataChunkSize[nextwave];
			}
				
			break;

	}

	return SEQ_SUCCEED;
}


//==========================================================================
//  int Sound_SeekStartPoint(int pSoundSeq, UINT8 ctrl)
//
//  Description: StartPointÖÌSeek
//  Parameter: 
//  					pSoundSeq	TEhÌîñðÂ\¢ÌÌÌæÖÌ|C^
//  					ctrl			0:Control MessageÈµ
//  										1:                    è
//  Return :	-1	G[
//  					 0	³íI¹
//==========================================================================
int Sound_SeekStartPoint(int pSoundSeq, UINT8 ctrl)
{
	PSOUNDSEQINFO hSoundSeq;
	PSMAF pSmaf;
	MTRINFO *hMtrInf;
	ATRINFO *hAtrInf;
	UINT8 trackno;
	UINT16 CtrlDataSize;
	DATABLK BufData;     
	
	int retval;
	
	hSoundSeq = (PSOUNDSEQINFO)pSoundSeq;
	pSmaf = hSoundSeq->pSmaf;
	
	//----- FIFOÌNA -----------------------------------------//
	if (pSmaf->MtrNum) {
		FmSeq_FifoClear();
	}
	if (pSmaf->AtrNum) {
		PcmSeq_FifoClear();
	}
	
	if (ctrl) {
		if (pSmaf->MtrNum) {
			//-- Start PointÜÅÌControl -------------------------//
			for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++) {
				if (pSmaf->MtrNum & (0x01<<trackno)) {
					hMtrInf = pSmaf->hMtrChunk[trackno];
					CtrlDataSize = SmafScoreTrackControlSeek((UINT8)(trackno+pSmaf->isMa2), 
										hMtrInf->pSequenceDataChunk+hMtrInf->SeekPhraseInfo.dwStartPoint, &CtrlDataBuf[0], 1);
					retval = FmSeq_ControlSeek(trackno, &CtrlDataBuf[0], CtrlDataSize);
					if (retval == SEQ_ERROR)
						return SEQ_ERROR;
				}
			}
		}

		if (pSmaf->AtrNum) {
			//-- Start PointÜÅÌControl -------------------------//
			for (trackno=0; trackno<MAX_PCM_TRACK_NUM; trackno++) {
				if (pSmaf->AtrNum & (0x01<<trackno)) {
					hAtrInf = pSmaf->hAtrChunk[trackno];
					CtrlDataSize = SmafAudioTrackControlSeek(trackno, 
										hAtrInf->pSequenceDataChunk+hAtrInf->SeekPhraseInfo.dwStartPoint, &CtrlDataBuf[0]);
					retval = PcmSeq_ControlSeek(trackno, &CtrlDataBuf[0], CtrlDataSize);
					if (retval == SEQ_ERROR) {
						return SEQ_ERROR;      
					}
				}
			}
		}
	}
	
	if (pSmaf->MtrNum) {
		for (trackno=0; trackno<MAX_SCORE_TRACK_NUM; trackno++) {
			if (pSmaf->MtrNum & (0x01 << trackno)) {
				hMtrInf = pSmaf->hMtrChunk[trackno];
				BufData.pBuf = hMtrInf->pSequenceDataChunk + hMtrInf->SeekPhraseInfo.dwStartPoint;
				if ((!hMtrInf->bSequenceType) && (hMtrInf->SeekPhraseInfo.dwStartPoint != hMtrInf->SeekPhraseInfo.dwStopPoint)) {
					BufData.Size = hMtrInf->SeekPhraseInfo.dwStopPoint - hMtrInf->SeekPhraseInfo.dwStartPoint;
				}
				else {
					BufData.Size = hMtrInf->dwSequenceDataChunkSize;
				}
				if (FmSeq_Seek(trackno, &BufData, 0xFFFFFFFF) == SEQ_ERROR)
					return SEQ_ERROR;
			}
		}
	}

	if (pSmaf->AtrNum) {
		for (trackno=0; trackno<MAX_PCM_TRACK_NUM; trackno++) {
			if (pSmaf->AtrNum & (0x01 << trackno)) {
				hAtrInf = pSmaf->hAtrChunk[trackno];
				BufData.pBuf = hAtrInf->pSequenceDataChunk + hAtrInf->SeekPhraseInfo.dwStartPoint;
				if ((!hAtrInf->bSequenceType) && (hAtrInf->SeekPhraseInfo.dwStartPoint != hAtrInf->SeekPhraseInfo.dwStopPoint)) {
					BufData.Size = hAtrInf->SeekPhraseInfo.dwStopPoint - hAtrInf->SeekPhraseInfo.dwStartPoint;
				}
				else {
					BufData.Size = hAtrInf->dwSequenceDataChunkSize;
				}
				AtsqNextEvent[trackno] = hAtrInf->SeekPhraseInfo.dwStartPoint;
				if (PcmSeq_Seek(trackno, &BufData, 0xFFFFFFFF) == SEQ_ERROR)
					return SEQ_ERROR;
			}
		}
	}

	return SEQ_SUCCEED;
}

//==========================================================================
//  static UINT32 WriteDuration(UINT8 *pBuf, UINT32 t)
//
//  Description: wè³ê½obt@ÖÌDurationÌ«Ý
//  Parameter: 
//  					pBuf			Durationð«ÞÌæÖÌ|C^
//  					t					Durationl@(Timebase)
//  Return :	
//  					«ñ¾BYTE
//==========================================================================
static UINT32 WriteDuration(UINT8 *pBuf, UINT32 t)
{
	UINT32 writeSize=0;
	
	if( t<=127 ) {
		*pBuf++ = (UINT8)(t&0x7f);
		writeSize++;
	}
	else if (t<=16511) {
		*pBuf++ = (UINT8)((((t-128)>>7)&0x7f)|0x80);
		*pBuf++ = (UINT8)((t-128)&0x7f);
		writeSize += 2;
	}
	else {
		*pBuf++ = (UINT8)((((16511L-128)>>7)&0x7f)|0x80);
		*pBuf++ = (UINT8)((16511L-128)&0x7f);
		*pBuf++ = (UINT8)0xff;
		*pBuf++ = (UINT8)0x00;
		writeSize += 4;
		writeSize += WriteDuration(pBuf, t-16511L);
	}

	return writeSize;
}


//soundseq.c

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

extern boolean IsApplyHighMelodyGain ;
 
void MUSIC_volume_control(MA2SoundLevel plevel)
{
    UINT8  i=0;
	MA2AudioGain value;
    const MA2AudioGain audiogaintable[6]=
	{
		{ 0x10, 0x01, 0x00, 0x1f, 0x1f },
		{ 0x12, 0x02, 0x0b, 0x1F, 0x1F },
		{ 0x12, 0x06, 0x10, 0x1F, 0x1F },
		{ 0x12, 0x0b, 0x15, 0x1F, 0x1F },
		{ 0x12, 0x10, 0x1a, 0x1F, 0x1F },
		{ 0x12, 0x15, 0x1F, 0x1F, 0x1F }
//      { EQ,   HP  , SPK , FM  , ADPCM}
	};
	value = audiogaintable[plevel.SPK];
	if( plevel.EQ != 100  ){
		value.EQ=plevel.EQ;
	}
	//Added by happykid in 2001.5.14
	//For melody volume calibration
	if(IsApplyHighMelodyGain && value.EQ < 0x1F)
		value.EQ++;

	Ma2w_eq_vol(value.EQ);
	Ma2w_hp_vol(value.HP, value.HP);
	Ma2w_sp_vol((UINT8)(0x80+value.SPK));
	Ma2w_fm_master_vol(value.FM);
	for (i=0; i<16; i++) //MAX_FM_CHANNEL_NUM=16
		Ma2w_fm_channel_vol(i, 0x18);
	Ma2w_adpcm_channel_vol(0x1f);
	Ma2w_adpcm_master_vol(value.PCM);
    //º¼·ýÁ¶Á¤ºÎºÐ//
}

void MUSIC_music_buffer_start(UINT8* pBufferData,MA2SoundLevel value, music_acquire_type dev_type)
{
	MUSIC_sleep_enable = FALSE;
    SND_SLEEP_FORBID();

	Ma2w_hp_vol(0x00,0x00);
	Ma2w_sp_vol(0x00);
	if(snd_IsPowerOn == FALSE)
		Ma2_initialize(); 
	MUSIC_hw_power(dev_type);
	// volume control
	MUSIC_volume_control(value);
	// volume control

    Smaf_Initialize();
	MA2Manager_Initialize();
	Status = 0;
	pSmaf = 0;
	seqHd = 0;

    hSmafData = pBufferData;

    if(hSmafData != NULL)
	{
	    SmafSize1 = 100;	
        pSmaf = Smaf_Load(pSmaf, hSmafData, SmafSize1, 0, &Status);
	    if (pSmaf == 0)		
	    return;
	    seqHd = Smaf_Open(pSmaf, 0, SmafCBFunc);	// openMode:0
	    if (seqHd == -1)
		{		
		    Smaf_Unload(pSmaf);
		    return;
		}
 		tramp_set_isr(TRAMP_GPIO_INT_0_ISR,Ma2_Irq_Proc);
 		Smaf_Standby(seqHd, 0);		
	}   
}
void MUSIC_music_file_start(char *file_name, MA2SoundLevel value, music_acquire_type dev_type)
{
	DWORD  wFileSize;
	MUSIC_sleep_enable = FALSE;
    SND_SLEEP_FORBID();

	Ma2w_hp_vol(0x00,0x00);
	Ma2w_sp_vol(0x00);
	if(snd_IsPowerOn == FALSE)
		Ma2_initialize(); 
	MUSIC_hw_power(dev_type);
	// volume control
	MUSIC_volume_control(value);
	// volume control

    Smaf_Initialize();
	MA2Manager_Initialize();
	Status = 0;
	pSmaf = 0;
	seqHd = 0;

	if( !SKY_IS_IN_GVM() && !g_fGVMPaused ){
		Music_Get_Share_Memory((byte **)&pFileDataArea);
	}

	if(mm_BellFileLoadOpen((BYTE *)file_name, &wFileSize,
                         (byte **)&pFileDataArea) != MM_OK) return;

	mm_BellFileBlockLoad(4);
    mm_BellFileLoadClose();    
	hSmafData=pFileDataArea;

	if(hSmafData != NULL)
	{
	    SmafSize1 = 100;	
        pSmaf = Smaf_Load(pSmaf, hSmafData, SmafSize1, 0, &Status);
	    if (pSmaf == 0)		
	     return;
	    seqHd = Smaf_Open(pSmaf, 0, SmafCBFunc);	// openMode:0
	    if (seqHd == -1)
		{		
		    Smaf_Unload(pSmaf);
		    return;
		}
 		tramp_set_isr(TRAMP_GPIO_INT_0_ISR,Ma2_Irq_Proc);
 		Smaf_Standby(seqHd, 0);		
	}
}


void MUSIC_music_sound_start(word music_id, MA2SoundLevel value, music_acquire_type dev_type)
{
	MUSIC_sleep_enable = FALSE;
    SND_SLEEP_FORBID();

	Ma2w_hp_vol(0x00,0x00);
	Ma2w_sp_vol(0x00);
	if(snd_IsPowerOn == FALSE)
		Ma2_initialize(); 
	MUSIC_hw_power(dev_type);
	// volume control
	MUSIC_volume_control(value);
	// volume control

    Smaf_Initialize();
	MA2Manager_Initialize();
	Status = 0;
	pSmaf = 0;
	seqHd = 0;
 
    if(music_id >= MUS_FIRST_SND)
    {
        hSmafData = (byte *)MUSIC_get_music_data(music_id);
	}

    if(hSmafData != NULL)
	{
	    SmafSize1 = 100;	
        pSmaf = Smaf_Load(pSmaf, hSmafData, SmafSize1, 0, &Status);
	    if (pSmaf == 0)		
	    return;
	    seqHd = Smaf_Open(pSmaf, 0, SmafCBFunc);	// openMode:0
	    if (seqHd == -1)
	    {		
		    Smaf_Unload(pSmaf);
		    return;
		}
 		tramp_set_isr(TRAMP_GPIO_INT_0_ISR,Ma2_Irq_Proc);
 		Smaf_Standby(seqHd, 0);		
	}
}

void MUSIC_music_play(word nRepeatCount)
{
	snd_IsMusicPlaying = TRUE;
	rex_clr_timer(&MUSIC_music_timer);
    rex_clr_timer(&MUSIC_vRef_PDdelay_timer);
	Smaf_Start(seqHd, nRepeatCount);	
}
void MUSIC_music_stop(void)
{
	Smaf_Stop(seqHd);
	Smaf_Close(seqHd);
	seqHd = 0;
	Smaf_Unload(pSmaf);
	pSmaf = 0;
	mm_BellMemoryFree();
	snd_IsMusicPlaying = FALSE;

#ifdef FEATURE_SKVM
		if (SKVM_IS_MUSIC_PLAY())
		{
			xsig_sound_play_end(0);
		}
#endif

 	tramp_set_isr(TRAMP_GPIO_INT_0_ISR,NULL);
	rex_set_timer(&MUSIC_vRef_PDdelay_timer, 5000);
} // MUSIC_music_stop() 

void MUSIC_hw_init(void)
{
	UINT8  i;

	BIO_TRISTATE ( DEC_GPIO_0, BIO_YMU759_IRQ_M, BIO_YMU759_IRQ_LO_V);
	rex_def_timer(&MUSIC_music_timer, &snd_tcb, SND_MUSIC_TIMER_SIG);
	rex_def_timer(&MUSIC_vRef_PDdelay_timer, &snd_tcb, SND_VREF_TIMER_SIG);
	fMotorOn=0;
	fLEDOn=0;

	// Initialize Ma2 Irq Handler
	bMa2IrqHandler = 0;
	for( i=0 ; i<MA2_IRQ_HANDLE_MAX ; i++ )
		Ma2IrqHandler[i].p = NULL;

	regsw = 1;
	Ma2w_switch_reg(0);

	_syncch = 0;

	clk_busy_wait(55);
	
	Ma2w_power_management(0xff);			// Reset MA-2 1111 1111
	// CLKIÅ1024NbNÈãWait·éR?h?ü //
	clk_busy_wait(55);
	Ma2w_power_management(0x7f);			// set DP0    0111 1111
	Ma2w_power_management(0x7e);			// clear DP0  0111 1110
	
	// CLKIÉ í¹ÄAWX?1Ì05hA06hÖlðÝè·é±Æ
	// CASE I
	//Y=19.6608MHz / (19) = 1.0348 MHz  (837kHz < Y < 1.25MHz)
	//1.0348 MHz * (49) = 50.7042 MHz
	// CASE II`2
  	//Y=19.6608MHz / 17(11h) = 1.1565 MHz (837kHz < Y < 1.25MHz)
	//1.1565 Mhz * 44(2ch)  = 50.886 Mhz 
	Ma2_write_reg(MA2_CLOCK_ADJUST1, 0x11 );	// (addr index : 0x05)
	Ma2_write_reg(MA2_CLOCK_ADJUST2, 0x2c );	// (addr index : 0x06)


	Ma2w_power_management(0x7a);			// clear AP0   0111 1010


	Ma2w_control(0x00);					    // clear PLLDW 0000 0000 (addr index : 0x01)


	// 10msÈãÌWaitð·éR?h?ü //
	clk_busy_wait(15000);

	Ma2w_power_management(0x78);			// clear DP1    0111 1000
	Ma2w_power_management(0xf8);			// set CLR      1111 1000
	// CLKIÅ1024NbNÈãWait·éR?h?ü //
	clk_busy_wait(55);
	Ma2w_power_management(0x78);			// clear CLR    0111 1000
	Ma2w_power_management(0x70);			// clear AP1    0111 0000
	Ma2w_power_management(0x10);			// clear AP3/AP40001 0000
	// 10usÈãÌWaitð·éR?h?ü //
	clk_busy_wait(15);
	Ma2w_power_management(0x00);			// clear AP2    0000 0000


	Ma2w_ext_pin_control(0x00); 			// Init Ext Pin Function (addr index : 0x0d )
	Ma2w_fifo_control(0x1f);				// Reset All FIFO (addr index : 0x04(1) )


	MUSIC_hw_power_dowm();

} // MUSIC_hw_init()
void MUSIC_hw_power_on(MA2SoundLevel value, music_acquire_type dev_type)
{
	clk_busy_wait(5000);
	MUSIC_sleep_enable = FALSE;
    SND_SLEEP_FORBID();
	Ma2w_hp_vol(0x00,0x00);
	Ma2w_sp_vol(0x00);
	if(snd_IsPowerOn == FALSE)
		Ma2_initialize(); 
	MUSIC_hw_power(dev_type);
	// volume control
	MUSIC_volume_control(value);
    // volume control
	rex_clr_timer(&MUSIC_vRef_PDdelay_timer);
}

void MUSIC_hw_power_dowm(void)
{
	snd_IsPowerOn =FALSE;

	clk_busy_wait(55);
	Ma2w_power_management(0x7e);			// set DP1,AP0-AP4    0111 1110
	Ma2w_control(0x40);					    // set PLLDW          0100 0000 (addr index : 0x01)
	Ma2w_power_management(0xfe);			// set CLR            1111 1110
	clk_busy_wait(55);

	Ma2w_power_management(0x7e);			// clear CLR          0111 1110
	Ma2w_power_management(0x7f);			// clear DP0          0111 1111
	clk_busy_wait(55);

	if(fMotorOn)
		Ma2w_motor_control(0x80,0);
	if(fLEDOn)
		Ma2w_led_control(0x80,0);

	MUSIC_sleep_enable = TRUE;
    if(SND_ALLOW_SLEEPING) SND_SLEEP_ALLOW();
} // MUSIC_hw_power_dowm
void MUSIC_hw_power(music_acquire_type dev_type)
{
	if(dev_type == MUS_MELODY){
		Ma2w_power_management(0x70);			// clear AP1    0111 0000
		Ma2w_power_management(0x10);			// clear AP3/AP40001 0000
		// 10us Wait
		clk_busy_wait(15);
		Ma2w_power_management(0x00);			// clear AP2    0000 0000
	}
	else if(dev_type == MUS_SPK_MELODY)
	{
		Ma2w_power_management(0x70);			// clear AP1    0111 0000
		Ma2w_power_management(0x50);			// clear AP3	0101 0000
		clk_busy_wait(15);
		Ma2w_power_management(0x40);			// clear AP2    0100 0000
	}
	else if(dev_type == MUS_EAR_MELODY)
	{
		Ma2w_power_management(0x18);			// clear AP3/AP40001 1000
	}
}

#endif //FEATURE YMU759  youandi 

/////////////////////////////////////////757b Á¦°Å


