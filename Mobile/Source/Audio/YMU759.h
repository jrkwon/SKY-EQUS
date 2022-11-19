#ifndef YMU759_H
#define YMU759_H
/*===========================================================================

               Y M U 7 5 7 B   B I O   H E A D E R    F I L E

DESCRIPTION
  This file contains definitions of the bits of the YAMAHA FM synthesizer for
  the melody sound.  These bits are the control lines which can be read
  or written using the descipted register.

  Copyright (c) 1999 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.08.29   youandi  add FEATURE_YMU759_MELODY
00.08.02   tasik    create
===========================================================================*/


/*
	æ∆∑°¿« ∏µÁ ¡§¿«¥¬ YMU759ø°º≠¥¬ ºˆ¡§µ«æÓæﬂ «—¥Ÿ. 

    ¥Á∫–∞£¿∫ A10 «¡∑Œ¡ß∆ÆøÕ ∞¯¿Øµ«æÓ ¿÷¿∏π«∑Œ ƒƒ∆ƒ¿œ ø°∑Ø∏¶ «««œ±‚ ¿ß«ÿº≠ 
	±◊¥Î∑Œ µ–¥Ÿ.
*/


#ifdef FEATURE_YMU759_MELODY // youandi 00.08.29

#include "biog.h"
#include "comdef.h"

//machdef.h
#define SINT8  signed char
#define SINT16 short int
#define SINT32 int
#define UINT8  unsigned char
#define UINT16 unsigned short int
#define UINT32 unsigned int

#define bool int
#define true 1
#define false 0

#define SEQ_SUCCEED	0
#define SEQ_ERROR		-1

void machdepWait100ns(void);
void machdepWait400ns(void);
void machdepWait1300ns(void);
void machdepWait1024clki(void);
void machdepWait10ms(void);
void machdepWait10us(void);
void machdepSetIrqVector(void);
int machdepWaitFifoFlags(UINT16 waitflg, UINT16 errflg);

//machdef.h
//smafchecker.h
#define		SIZE_OF_CHUNKHEADER   8   /* The size of chunk header */
#define		SIZE_OF_CRC			  2	  /* The size of CRC */
#define 	SIZE_OF_MIN_CNTI	  5	  /* The size of minimum CNTI */

#define		MAX_SCORE_TRACK_CHUNK 5   /* Max number of Score track chunk */
#define		MAX_AUDIO_TRACK_CHUNK 1   /* Max number of PCM Audio track chunk */
#define		MAX_WAVE_DATA 64          /* Max number of wave data */

#define		NUM_PHRASE_TAG		33
#define		MAX_SUB_SEQUENCE	64

/* Error Status */
#define SMAF_ERR_FILE		0x0001
#define SMAF_ERR_CLASS	0x0002
#define SMAF_ERR_TYPE		0x0004
#define SMAF_ERR_SIZE		0x0008
#define SMAF_ERR_CHUNK	0x0010


/* Seek & Phrase Info */
struct PHRASE_LIST {
	SINT32 dwStart[NUM_PHRASE_TAG];
	SINT32 dwStop[NUM_PHRASE_TAG];
};
	
typedef struct SEEK_PHRASE_INFO {
	UINT32 dwStartPoint;
	UINT32 dwStopPoint;
	struct PHRASE_LIST *pPhraseList;
	char   *pSubSequenceList;
} SEEKPHRASEINFO;

/* SMAF data analyzed info */
typedef struct OVERALL_CHUNK {
	UINT8 *pOverallChunk;             /* Pointer to Overall Chunk body */
	UINT32 dwOverallChunkSize;        /* The size of Overall Chunk body */
	UINT8 *pContentsInfoChunk;        /* Pointer to Contents Info Chunk body */
	UINT32 dwContentsInfoChunkSize;   /* The size of Contents Info Chunk body */
	UINT8  bContentsClass;            /* Contents Class */
	UINT8  bContentsType;             /* Contents Type */
	UINT8  bContentsCodeType;         /* Contents Code */
	UINT8  bCopyStatus;               /* Copy Status */
	UINT8  bCopyCounts;               /* Copy Counts */
	UINT8 *pOption;                   /* Pointer to Option data */
	UINT32 dwOptionSize;              /* The size of Option data */
} OVERALLINFO;

/* Score Track analyzed info */
typedef struct SCORE_TRACK_CHUNK {
	UINT8 *pScoreTrackChunk;          /* Pointer to Score Track Chunk body */
	UINT32 dwScoreTrackChunkSize;     /* The size of Score Track Chunk body*/
	UINT8 *pSeekPhraseInfoChunk;      /* Pointer to Seek & Phrase Info Chunk body */
	UINT32 dwSeekPhraseInfoChunkSize; /* The size of Seek & Phrase Info Chunk body */
	UINT8 *pSetupDataChunk;           /* Pointer to Setup Data Chunk body */
	UINT32 dwSetupDataChunkSize;      /* The size of Setup Data Chunk body */
	UINT8 *pSequenceDataChunk;        /* Pointer to Sequence Data Chunk body */
	UINT32 dwSequenceDataChunkSize;   /* The size of Sequence Data Chunk body */
	UINT8  bFormatType;     /* Format Type */
	UINT8  bSequenceType;   /* Sequence Data */
	UINT32 dwTimeBaseD;     /* Duration TimeBase (msec) */
	UINT32 dwTimeBaseG;     /* Gatetime TimeBase (msec) */
	UINT16 wChannelStatus;  /* Channel Status */
	UINT32 ScoreAbsTime;    /* Playback period of track data (Timebase) */
	UINT32 NopDuration;     /* Period between last event start and playback stop (Timebase) */
	SEEKPHRASEINFO SeekPhraseInfo; /* Pointer to Seek & Phrase info  */
} MTRINFO;

/* PCM Audio Track analyzed info */
typedef struct PCM_AUDIO_TRACK_CHUNK {
	UINT8 *pAudioTrackChunk;          /* Pointer to PCM Audio Track Chunk body */
	UINT32 dwAudioTrackChunkSize;     /* The size of PCM Audio Track Chunk body */
	UINT8 *pAudioTrackSeekPhraseInfoChunk;      /* Pointer to Seek & Phrase Info Chunk body */
	UINT32 dwAudioTrackSeekPhraseInfoChunkSize; /* The size of Seek & Phrase Info Chunk body */
	UINT8 *pSetupDataChunk;           /* Pointer to Setup Data Chunk body */
	UINT32 dwSetupDataChunkSize;      /* The size of Setup Data Chunk body */
	UINT8 *pSequenceDataChunk;        /* Pointer to Sequence Data Chunk body */
	UINT32 dwSequenceDataChunkSize;   /* The size of Sequence Data Chunk body */
	UINT8 *pWaveDataChunk[MAX_WAVE_DATA];       /* Pointer to Wave Data Chunk body */
	UINT32 dwWaveDataChunkSize[MAX_WAVE_DATA];  /* The size of Wave Data Chunk body */
	UINT8  bFormatType;   /* Format Type */
	UINT8  bSequenceType; /* Sequence Data */
	UINT16 wWaveType;     /* Wave Type */
	UINT32 dwTimeBaseD;   /* Duration TimeBase (msec) */
	UINT32 dwTimeBaseG;   /* Gatetime TimeBase (msec) */
	UINT32 AudioAbsTime;  /* Playback period of Track data (Timebase) */
	SEEKPHRASEINFO SeekPhraseInfo; /* Pointer to Seek & Phrase info */
} ATRINFO;
// πŸ≤Ÿ¿⁄!!!!!
/*
unsigned short makeCRC(UINT32 n, UINT8 *c);
int SmafChecker( UINT8* pBuffer, UINT32 dwSize, UINT16 *pStatus );
bool GetSmafOptionData(UINT16 reqTag, UINT8 **pData, UINT8 *pDataSize);
bool SmafScoreTrackSeek(UINT8 bScoreTrack, UINT32 dwSeekAbs,
                        UINT8 **pSeekPoint, UINT32 *dwSeekDuration);
UINT16 SmafScoreTrackControlSeek(UINT8 bScoreTrack, UINT8 *pSeekPoint,
                                 UINT8 *pControl, UINT8 bCtrl);
bool SmafAudioTrackSeek(UINT8 bAudioTrack, UINT32 dwSeekAbs,
                        UINT8 **pSeekPoint, UINT32 *dwSeekDuration);
UINT16 SmafAudioTrackControlSeek(UINT8 bAudioTrack, UINT8 *pSeekPoint,
                                 UINT8 *pControl);
*/
// ÉvÉçÉgÉ^ÉCÉvêÈåæ
int SmafChecker( UINT8* pBuffer );
bool GetSmafOptionData(UINT16 reqTag, UINT8 **pData, UINT8 *pDataSize);
bool SmafScoreTrackSeek(UINT8 bScoreTrack, UINT32 dwSeekAbs,
							  UINT8 **pSeekPoint, UINT32 *dwSeekDuration);
UINT16 SmafScoreTrackControlSeek(UINT8 bScoreTrack, UINT8 *pSeekPoint,
												 UINT8 *pControl, UINT8 bCtrl);
bool SmafAudioTrackSeek(UINT8 bAudioTrack, UINT32 dwSeekAbs,
							  UINT8 **pSeekPoint, UINT32 *dwSeekDuration);
UINT16 SmafAudioTrackControlSeek(UINT8 bAudioTrack, UINT8 *pSeekPoint,
												 UINT8 *pControl);

//smafchecker.h

//smafseq.h

#define HEADER_SIZE           8
#define MAX_SMAF_DATA_NUM     1     /* The number of SMAF data which can support (1) */
#define MAX_SCORE_TRACK_NUM   5     /* Max number of Score Track */
#define MAX_PCM_TRACK_NUM     1     /* Max number of PCM Audio Track */
#define MAX_GRAPHIC_TRACK_NUM 1     /* Max number of Graphic Track */
#define MAX_MASTER_TRACK_NUM  1     /* Max number of Master Track */
#define MAX_WAVE_DATA_NUM     0x40  /* Max number of Wave Data */

/* Flag function bit in playback */
#define	PLAYER_SCORE    1
#define	PLAYER_PCM      2
#define	PLAYER_GRAPHICS 4
#define	PLAYER_MASTER   8

/* #define PLAYMODE_ONE 1 */
#define PLAYMODE_LOOP 0


/* Smaf_Control()ctrlNum value */
#define SMAFCTRL_SET_HPVOL  0x0001  /* Set Headphone volume */
#define SMAFCTRL_SET_SPVOL  0x0002  /* Set Speaker volume */
#define SMAFCTRL_SET_LED    0x0040  /* Set LED on/off control */
#define SMAFCTRL_SET_MOTOR  0x0041  /* Set Vibrator control */
#define SMAFCTRL_SET_KEYCON 0x0042  /* Set Key control */

#define SMAFCTRL_GET_HPVOL  0x8001  /* Read Headphone volume */
#define SMAFCTRL_GET_SPVOL  0x8002  /* Read Speaker volume */
#define SMAFCTRL_GET_LED    0x8040  /* Read LED on/off control */
#define SMAFCTRL_GET_MOTOR  0x8041  /* Read Vibrator control */
#define SMAFCTRL_GET_KEYCON 0x8042  /* Read Key control */


/* status of SMAF Sequencer */
enum STATE_SMAF_SEQ {
	SMAF_OPENED,
	SMAF_READY,
	SMAF_PLAYING
};

/* SMAF CallBack event type */
typedef enum {
  SMAF_PLAYSTART,
	SMAF_PLAYSTOP,
	SMAF_PLAYEND,
	SMAF_ERROR
} SMAFCBEVTYPE;


/* Master Track info (dummy) */
typedef struct _TtrInfo {
	UINT8 *hBuffer;
	UINT32 dChunkSize;
} MSTRINFO, *PMSTRINFO;

/* Graphic Track info (dummy) */
typedef struct _GtrInfo {
	UINT8 *hBuffer;
	UINT32 dChunkSize;
} GTRINFO, *PGTRINFO;


/* SMAF analyzed info */
typedef struct _SmafInfo {
	OVERALLINFO *hAllChunk;                      /* Pointer to Overall Chunk info */
	MTRINFO *hMtrChunk[MAX_SCORE_TRACK_NUM];     /* Pointer to Score Track Chunk info */
	ATRINFO *hAtrChunk[MAX_PCM_TRACK_NUM];       /* Pointer to PCM Audio Track Chunk info */
	GTRINFO	*hGtrChunk[MAX_GRAPHIC_TRACK_NUM];   /* Pointer to Graphic Track Chunk info */
	MSTRINFO	*hMstrChunk[MAX_MASTER_TRACK_NUM]; /* Pointer to Master Track Chunk info */
	UINT8 MtrNum;    /* The number of Score Track */
	UINT8 AtrNum;    /* The number of PCM Audio Track */
	UINT8 GtrNum;    /* The number of Graphic Track */
	UINT8 MstrNum;   /* The number of Master Track */
	UINT8 isMa2;     /* 1:MA2 data   0:MA1 data */
} SMAF, *PSMAF;

/* SMAF sequencer info */
typedef struct _SmafSeqInfo{
	SMAF *pSmaf;          /* SMAF data analyzed info in process */
	int hSeqHd;           /* Handle of the internal sequencer info */
	UINT32 hSoundInf;     /* Handle of the internal sequencer sound portion info */
	enum STATE_SMAF_SEQ stateSmaf;  /* State flag of SMAF sequencer */
	UINT8 playingSeq;     /* Check flag of Playbacking Track */
	int playMode;         /* Playback mode   0 eternal loop  !=0 playback times */
	int playTimes;        /* Playback times*/
	int (*pSmafCBProc)(SMAFCBEVTYPE event, void *prm); /* Callback function to SMAF-API */
} SMAFSEQINFO, *PSMAFSEQINFO;


/* Resource info structure used at Smaf_GetSmafResInfo() */
typedef struct _SmafResInfo{
	unsigned long FmChMap;
	unsigned long AdpcmChMap;
} SMAFRESINFO, *PSMAFRESINFO;


/* Prototyping */
int Smaf_GetStatus(int seqHd);
int Smaf_Control(int seqHd, int ctrlNum, void *prm);
int Smaf_Seek(int seqHd, int pos);
int Smaf_Stop(int serHd);
int Smaf_Start(int seqHd, int playMode);
int Smaf_Standby(int seqHd, int pos);
int Smaf_Close(int seqHd);
int Smaf_Open(void *pSmaf, int openMode, int (*SmafCBProc)(SMAFCBEVTYPE event, void *prm));
int Smaf_GetSmafResInfo(void *pSmaf, SMAFRESINFO *pInfo);
int Smaf_Unload(void *pSmaf);
void * Smaf_Load(void *pSmaf, UINT8 *pFileBuf, int fileSize, int loadMode, int *pStatus);
int Smaf_Initialize(void);
int Smaf_CheckStatus(void);
/*πŸ≤Ÿ¿⁄!!!!!!
int Smaf_GetStatus(int seqHd);
int Smaf_Control(int seqHd, int ctrlNum, void *prm);
int Smaf_Seek(int seqHd, unsigned long pos);
int Smaf_Stop(int serHd);
int Smaf_Start(int seqHd, int playMode);
int Smaf_Standby(int seqHd, unsigned long pos);
int Smaf_Close(int seqHd);
int Smaf_Open(void *pSmaf, int openMode, int (*SmafCBProc)(SMAFCBEVTYPE event, void *prm));
int Smaf_GetSmafResInfo(void *pSmaf, SMAFRESINFO *pInfo);
int Smaf_Unload(void *pSmaf);
void * Smaf_Load(void *pSmaf, unsigned char *pFileBuf, unsigned long fileSize, int loadMode, int *pStatus);
int Smaf_Initialize();
int Smaf_CheckStatus();
unsigned long Smaf_GetDataPlayTime(void *pSmaf);
unsigned short Smaf_GetPLLOut();

int PlayEnd(unsigned long pSeqInf, int player);
*/
//smafseq.h

//Madef.h
/*
-----------------------------------------------------------------------
					AUDIO Register index & Control data Address, MACRO
-----------------------------------------------------------------------
*/
#define AUDIO_REGISTER_INDEX_ADDRESS			0x2800000    /* B1 ADDR : AUDIO Register Index √‚∑¬ */
#define	AUDIO_CONTROL_DATA_ADDRESS			    0x2800002    /* B1 ADDR : AUDIO Control Data   √‚∑¬ */

#define	AUDIO_OUTP_REG_INDEX( Value )		outp ( AUDIO_REGISTER_INDEX_ADDRESS , (Value) )
#define	AUDIO_OUTP_CTL_DATA( Value )	    outp ( AUDIO_CONTROL_DATA_ADDRESS , (Value) )
#define AUDIO_INP_CTL_DATA() 				inp  ( AUDIO_CONTROL_DATA_ADDRESS )

/***********************************************************
		Register Definition Page 1
***********************************************************/

#define		MA2_FM_SEQ_DATA			0x00
#define		MA2_FM_SEQ_DATA0		0x00
#define		MA2_FM_SEQ_DATA1		0x01
#define		MA2_FM_SEQ_DATA2		0x02
#define		MA2_FM_SEQ_DATA3		0x03
#define		MA2_ADPCM_SEQ_DATA		0x04
#define		MA2_ADPCM_WAVE_DATA		0x05
#define		MA2_FM_IRQ_CTRL			0x06
#define		MA2_ADPCM_IRQ_CTRL		0x07
#define		MA2_SEQ_FIFO_STAT1		0x08
#define		MA2_SEQ_FIFO_STAT2		0x09
#define		MA2_WAVE_FIFO_STAT		0x0a
#define		MA2_SEQ_FIFO_FULL_FLAG	0x0b
#define		MA2_SEQ_FIFO_EMPTY_FLAG	0x0c
#define		MA2_VERSION				0x0e
#define		MA2_REGISTER_SW			0x0f

#define		MA2_FM_FNUM_LO			0xb0
#define		MA2_FM_FNUM_HI			0xc0
#define		MA2_FM_ALGORITHM		0xd0
#define		MA2_FM_CHANNEL_VOL		0xe0
#define		MA2_FM_CHANNEL_PAN		0xf0


/***********************************************************
		Register Definition Page 2
***********************************************************/

#define		MA2_POWER_MANAGEMENT	0x00
#define		MA2_CONTROL				0x01
#define		MA2_FM_TIMEBASE			0x02
#define		MA2_ADPCM_TIMEBASE		0x03 
#define		MA2_FIFO_CTRL			0x04

#define		MA2_CLOCK_ADJUST1		0x05
#define		MA2_CLOCK_ADJUST2		0x06

#define		MA2_LED_CTRL			0x07
#define		MA2_MOTOR_CTRL			0x08
#define		MA2_SYNCCH				0x09

#define		MA2_TIMER_TIMEBASE		0x0a
#define		MA2_TIMER_COUNT			0x0b

#define		MA2_EXT_PIN_CTRL		0x0d

#define		MA2_FM_MASTER_VOL		0x10

#define		MA2_KCONCH				0x11
#define		MA2_KCONCH0				0x11
#define		MA2_KCONCH1				0x12
#define		MA2_KCONV  				0x13

#define		MA2_FM_CHANNEL_ASSIGN	0x18

#define		MA2_ADPCM_CHANNEL_VOL	0x20
#define		MA2_ADPCM_MASTER_VOL	0x21

#define		MA2_EQ_VOL				0x22
#define		MA2_HP_VOL_L			0x23
#define		MA2_HP_VOL_R			0x24
#define		MA2_SP_VOL				0x25

#define		MA2_FM_MODULATION		0xc0
#define		MA2_FM_BO						0xd0

#define		MA2_FM_EXPRESSION		0xe0


/***************************************************************
	Control Reg(SW=1/Index 01h) Bit Definition
***************************************************************/

#define		MA2C_CTRL_MA1			0x80			/* FM Volume -6dB */
#define		MA2C_CTRL_PLLDW		0x40			/* Power down PLL */
#define		MA2C_CTRL_TMST		0x10	/* Timer Start/Stop */
#define		MA2C_CTRL_FS		0x08	/* ADPCM Fs  0:4kHz , 1:8kHz */
#define		MA2C_CTRL_ST		0x04	/* ADPCM Direct Mode Start/Stop */
#define		MA2C_CTRL_ADST		0x02	/* ADPCM Sequencer Start/Stop */
#define		MA2C_CTRL_FMST		0x01	/* FM Sequencer Start/Stop */

/***************************************************************
	FIFO Control Reg(SW=0/Index 04h) Bit Definition
***************************************************************/

#define		MA2C_ADCLR				0x80	/* ADPCM Soft reset */
#define		MA2C_SQCLR				0x40	/* Seq. Soft reset */
#define		MA2C_FMCLR				0x20	/* FM Soft reset */
#define		MA2C_FIFO_RPTE		0x10
#define		MA2C_FIFO_RPT		0x08
#define		MA2C_FIFO_DFCLR		0x04	/* ADPCM Wave FIFO Reset */
#define		MA2C_FIFO_AFCLR		0x02	/* ADPCM Seq. FIFO  Reset */
#define		MA2C_FIFO_FFCLR		0x01	/* FM Seq. FIFO Reset */

/***************************************************************
	FM IRQ Control Reg(SW=0/Index 06h) Bit Definition
***************************************************************/

#define		MA2C_FM_IRQE3		0x80	/* IRQ3 Enable/Disable */
#define		MA2C_FM_IRQE2		0x40	/* IRQ2 Enable/Disable */
#define		MA2C_FM_IRQE1		0x20	/* IRQ1 Enable/Disable */
#define		MA2C_FM_IRQE0		0x10	/* IRQ0 Enable/Disable */
#define		MA2C_FM_FIFO		0x08	/* FIFO Usage */
#define		MA2C_FM_IRQP		0x07	/* IRQ Point */

/***************************************************************
	ADPCM IRQ Control Reg(SW=0/Index 07h) Bit Definition
***************************************************************/

#define		MA2C_ADPCM_IRQES	0x80	/* IRQES Enable/Disable */
#define		MA2C_ADPCM_IRQED	0x40	/* IRQED Enable/Disable */
#define		MA2C_ADPCM_GENDE	0x20	/* IRQGENDE Enable/Disable */
#define		MA2C_ADPCM_IRQPS	0x18	/* IRQ Point S */
#define		MA2C_ADPCM_IRQPD	0x07	/* IRQ Point D */

/***************************************************************
	SEQ FIFO Status Reg 1(SW=0/Index 08h) Bit Definition
***************************************************************/

#define		MA2S_SQ1_ERR		0x80	/* Error */
#define		MA2S_SQ1_ADEND		0x40	/* ADPCM Seq. End */
#define		MA2S_SQ1_FMEND		0x20	/* FM Seq. End */
#define		MA2S_SQ1_AFLG		0x10	/* ADPCM Seq. FIFO Interrupt status */
#define		MA2S_SQ1_FLG3		0x08	/* FM FIFO3 Interrupt status */
#define		MA2S_SQ1_FLG2		0x04	/* FM FIFO2 Interrupt status */
#define		MA2S_SQ1_FLG1		0x02	/* FM FIFO1 Interrupt status */
#define		MA2S_SQ1_FLG0		0x01	/* FM FIFO0 Interrupt status */

/***************************************************************
	SEQ FIFO Status Reg 2(SW=0/Index 09h) Bit Definition
***************************************************************/

#define		MA2S_SQ2_ADERR		0x80	/* ADPCM Seq. Error */
#define		MA2S_SQ2_FMERR		0x40	/* FM Seq. Error */
#define		MA2S_SQ2_GEND		0x20	/* ADPCM Gatetime End */
#define		MA2S_SQ2_SEND		0x10	/* ADPCM Seq. End */
#define		MA2S_SQ2_END3		0x08	/* FM Seq. END */
#define		MA2S_SQ2_END2		0x04	/* FM Seq. END */
#define		MA2S_SQ2_END1		0x02	/* FM Seq. END */
#define		MA2S_SQ2_END0		0x01	/* FM Seq. END */

/***************************************************************
	ADPCM Wave FIFO Status Reg(SW=0/Index 0Ah) Bit Definition
***************************************************************/

#define		MA2S_WAVE_DEMP		0x04	/* Wave Data FIFO Empty */
#define		MA2S_WAVE_DFULL		0x02	/* Wave Data FIFO Full */
#define		MA2S_WAVE_DFLG		0x01	/* Wave Data FIFO Interrupt status */

/***************************************************************
	SEQ FIFO Full Flag(SW=0/Index 0Bh) Bit Definition
***************************************************************/

#define		MA2S_FIFO_ERR			0x80	/* Seq. Error */
#define		MA2S_FIFO_ADEND		0x40	/* ADPCM Seq. End */
#define		MA2S_FIFO_FMEND		0x20	/* FM Seq. End */
#define		MA2S_FIFO_AFULL		0x10	/* ADPCM Seq. FIFO Full */
#define		MA2S_FIFO_FUL3		0x08	/* FM Seq. FIFO Full */
#define		MA2S_FIFO_FUL2		0x04	/* FM Seq. FIFO Full */
#define		MA2S_FIFO_FUL1		0x02	/* FM Seq. FIFO Full */
#define		MA2S_FIFO_FUL0		0x01	/* FM Seq. FIFO Full */

/***************************************************************
	SEQ FIFO Empty Flag(SW=0/Index 0Ch) Bit Definition
***************************************************************/

#define		MA2S_FIFO_AEMP		0x10	/* ADPCM Seq. FIFO Empty */
#define		MA2S_FIFO_EMP3		0x08	/* FM Seq. FIFO Empty */
#define		MA2S_FIFO_EMP2		0x04	/* FM Seq. FIFO Empty */
#define		MA2S_FIFO_EMP1		0x02	/* FM Seq. FIFO Empty */
#define		MA2S_FIFO_EMP0		0x01	/* FM Seq. FIFO Empty */


#define MA2_IRQ_HANDLE_MAX	4
#define MAX_FM_CHANNEL_NUM	16		/* MA2 Max FM Voices */

struct OPERATOR {
	UINT8	d0;						/* MULTI, VIB, EGT, SUS, KSR */
	UINT8	d1;						/* RR, DR */
	UINT8	d2;						/* AR, SL */
	UINT8	d3;						/* TL, KSL */
	UINT8	d4;						/* DVB, DAM, AM, WS */
};

struct MA2_VOICE_PARAMETER {
	UINT8	Num;					/*  #1  Voice Num */
	UINT8	Bank;					/*  #2  Bank Select */
	UINT8	PrgChng;				/*  #3  Program Change */
	UINT8	Info;					/*  #4  Voice Info */
	UINT8	Bo;						/*  #5  Basic Octave */
	struct OPERATOR op1;
	struct OPERATOR op2;
	struct OPERATOR op3;
	struct OPERATOR op4;
}; 

struct MA2_IRQ_SERVICE {
	void (*p)(void);
};
//Madef.h
//MA2Manager.h

typedef struct MA2_RESOURCE_INFO {
	UINT8	NumFmCh;
	UINT32	FmChStatus;
	UINT8	NumAdpcmCh;
	UINT8	AdpcmChStatus;
} MA2RESOURCEINFO;

void  MA2Manager_Initialize(void);
bool MA2Manager_AllocFmCh(UINT32 ch);
bool MA2Manager_FreeFmCh(UINT32 ch);
bool MA2Manager_AllocAdpcmCh(UINT8 ch);
bool MA2Manager_FreeAdpcmCh(UINT8 ch);
void  MA2Manager_GetResourceInfo(struct MA2_RESOURCE_INFO *info);
void MA2Manager_Ma2IrqControl(UINT8 bCtrl);

//MA2Manager.h

//Ma2Seq.h

#define MAX_FIFO_NUM	4           /* Max number of FM sequence FIFO */

#define MAX_FM_FIFO_SIZE  96      /* The Max data size when FM sequence FIFO */
                                  /* interrupt point is empty                */

#define IRQ_FM_FIFO_SIZE  48      /* The Max data size when FM sequence FIFO */
                                  /* interrupt point is 48                   */

#define IRQ_FM_FIFO_POINT 0x03    /* 48byte */

#define MAX_PCM_SEQFIFO_SIZE  32  /* The Max data size when PCM sequence FIFO */
                                  /* interrupt point is empty                 */

#define MAX_PCM_WAVFIFO_SIZE  384 /* The Max data size when PCM wave data FIFO */
                                  /* interrupt point is empty                  */

#define IRQ_PCM_SEQFIFO_SIZE  16  /* The Max data size when PCM sequence FIFO */
                                  /* interrupt point is 16                    */

#define IRQ_PCM_WAVFIFO_SIZE  192 /* The Max data size when PCM wave data FIFO */
                                  /* interrupt point is 192                    */

#define IRQ_PCM_SEQFIFO_POINT 0x10  /* 16byte */
#define IRQ_PCM_WAVFIFO_POINT 0x04  /* 192byte */

#define MAX_FM_CHANNEL_NUM   16   /* Max number of MA2 FM channel */
#define MAX_PCM_CHANNEL_NUM   1   /* Max number of MA2 ADPCM channel*/

/* SEQ FIFO STATUS */
#define MA2S_SQ1_FLG		0x0F
#define MA2S_SQ2_END		0x0F  
#define MA2C_FM_IRQE		0xF0
#define MA2S_FM_EMP			0x0F


/* IRQ TYPE */
#define IRQ_FM_ERR					0x0001
#define IRQ_FM_END					0x0002
#define IRQ_FM_DATA					0x0004
#define IRQ_FM_EMPTY				0x0008
#define IRQ_PCMSEQ_ERR			0x0010
#define IRQ_PCMSEQ_END			0x0020
#define IRQ_PCMSEQ_DATA			0x0040
#define IRQ_PCMSEQ_EMPTY		0x0080
#define IRQ_PCMWAV_END			0x0100
#define IRQ_PCMWAV_DATA			0x0200
#define IRQ_PCMWAV_EMPTY		0x0400

#define ADPCM_TEST_IMPREMENT

#ifdef ADPCM_TEST_IMPREMENT

#define ADPCMCHECK_ENDPACKET	1
#define ADPCMCHECK_NORMAL		0
#define ADPCMCHECK_ERROR		-1

typedef struct _PACKETINFO
{
	UINT8	duration_type;		/* */
	UINT8	duration_size;		/* */
	UINT8	duration_byte[2];	/* */
	UINT8	event_type;			/* */
	UINT8	event_size;			/* */
	UINT8	event_byte[3];		/* */
	UINT8	end_of_sequence;	/* */
} PACKETINFO, * PPACKETINFO;
#endif

/* FM Initialize data for MA-2 */
/* This data is used for FM sequencer standby */
typedef struct _FmSeqIniData{
	UINT32 dTimebaseD;  /* Timebase of Duration */
	UINT32 dTimebaseG;  /* Timebase of Gatetime */
	UINT32 reqFmCh;     /* MAP of MA-2 FM channel to be used */
} FMSEQINIDATA, *PFMSEQINIDATA;

/* PCM Initialize data for MA-2 */
/* This data is used for PCM Audio sequencer satandby */
typedef struct _PcmSeqIniData{
	UINT32 dTimebaseD;  /* Timebase of Duration */
	UINT32 dTimebaseG;  /* Timebase of Gatetime */
	UINT8 waveFS;       /* Sampling frequency of wave data */
	UINT32 reqAdpcmCh;  /* MAP of MA-2 ADPCM channel to be used */
} PCMSEQINIDATA, *PPCMSEQINIDATA;

/* FM FIFO Information */
typedef struct _FmFifoInf{
	UINT8 *hBuffer;    /* Pointer to the first data to be written to the FIFO */
	UINT32 dBufSize;   /* The data size to be written to the FIFO */
	UINT32 dBufCount;  /* The data size which is written to the FIFO */
} FIFOINF, *PFIFOINF;

/* FM Sequence Information */
typedef struct _FmSeqInf{
	UINT16 fifoSize;  /* The size of sequence FIFO */
	UINT16 irqSize;   /* Writable size to the FIFO when interrupt has occured */
	FIFOINF fifoInf[MAX_FIFO_NUM]; /* The FIFO data info */
	void *prm;        /* Pointer to the parameter for SMAF sequencer interrupt function */
	UINT32 useFmCh;   /* MAP of MA2 FM channel in use */
} FMSEQINF, *PFMSEQINF;

/* PCM Information */
typedef struct _PcmSeqInf{
	UINT16 seqFifoSize;   /* The size of ssequence FIFO */
	UINT16 seqIrqSize;    /* Writable size of sequence FIFO at interrupt */
	UINT16 wavFifoSize;   /* The size of wave data FIFO */
	UINT16 wavIrqSize;    /* Writable size of wave data FIFO at interrupt */
	FIFOINF seqFifoInf;   /* sequence FIFO data info */
	FIFOINF wavFifoInf;   /* wave data FIFO data info */
	void *prm;            /* Pointer to the parameter for SMAF sequencer interrupt function */
	UINT32 usePcmCh;      /* MAP of MA2 PCM channel in use */
} PCMSEQINF, *PPCMSEQINF;

/* Sequence Data Handle for Callback function */
typedef struct _DataBlock {
	UINT8 *pBuf;   /* Pointer to FIFO write data */
	UINT32 Size;   /* The size of FIFO write data */
} DATABLK, *PDATABLK;

/* status of FM Sequencer */
enum STATE_FM {
	FM_NOTEXIST,
	FM_OPENED,
	FM_READY,
	FM_PLAYING
};

/* status of PCM Sequencer */
enum STATE_PCM {
	PCM_NOTEXIST,
	PCM_OPENED,
	PCM_READY,
	PCM_PLAYING
};

/* event type for FM callback function */
typedef enum _FmCBEVTYPE {
	FM_STANDBY,         /* Callback from FmSeq_Standby() */
	FMSEQ_DATA_REQUEST, /* Callback while playback */
	FMSEQ_DATA_EMPTY    /* Callback when the playback data is empty */
} FMCBEVTYPE;

/* event type for PCM callback function */
typedef enum {
	PCM_STANDBY,          /* Callback from PcmSeq_Standby() */
	PCMSEQ_DATA_REQUEST,  /* Callback to request the continuous data of sequence data while playback */
	PCMSEQ_DATA_EMPTY,    /* Callback when playback sequence data is empty */
	PCMWAV_DATA_REQUEST,  /* Callback to request the continuous data of wave data while playback */
	PCMWAV_DATA_EMPTY,    /* Callback when playback wave data is empty */
	GATE_TIME_UP          /* Callback when Gatetime of wave data playback times up */
} PCMCBEVTYPE;

                 
/* Prototyping */
void Seq_Initialize(void);
void Seq_Irq_Proc(void);
int Seq_Stop(void);
int Seq_Start(void);
int Seq_Reset(void);
int PcmSeq_FifoClear(void);
int PcmSeq_Seek(UINT8 fifoNo, DATABLK *pBufData, UINT32 SeekDuration);
int PcmSeq_ControlSeek(UINT8 fifoNo, UINT8 *hCtrlData, UINT16 CtrlDataSize);
int PcmSeq_Close(void);
int PcmSeq_Stop(void);
int PcmSeq_Start(void);
int PcmSeq_Standby(PCMSEQINIDATA *pci, void *prm);
int PcmSeq_Open(int (*PcmFifoProc)(PCMCBEVTYPE, int, DATABLK *, void *));
int FmSeq_Close(void);
int FmSeq_FifoClear(void);
int FmSeq_Seek(UINT8 fifoNo, DATABLK *pBufData, UINT32 SeekDuration);
int FmSeq_ControlSeek(UINT8 fifoNo, UINT8 *hCtrlData, UINT16 CtrlDataSize);
int FmSeq_Stop(void);
int FmSeq_Start(void);
int FmSeq_Standby(FMSEQINIDATA *fmi, void *prm);
int FmSeq_Open(int (*FmFifoProc)(FMCBEVTYPE, int, DATABLK *, void *), UINT8 fifoMap);

int GetFmSeqState(void);
int GetPcmSeqState(void);

//Ma2Seq.h

//soundseq.h
//πŸ≤Ÿ¿⁄!!!!!!
// SMAFÉVÅ[ÉPÉìÉTÉTÉEÉìÉhïîèÓïÒ //
typedef struct _SoundSeqInfo{   
	PSMAFSEQINFO hSeqInf;     // SMAFÉVÅ[ÉPÉìÉTèÓïÒÇ÷ÇÃÉ|ÉCÉìÉ^
	PSMAF pSmaf;							// èàóùíÜÇÃSMAFÉfÅ[É^ÇÃâêÕèÓïÒ
	UINT8 playingMtrTrack;		// égópíÜÇÃScoreÉgÉâÉbÉNÇé¶Ç∑
	UINT32 reqFmCh;						// égópÇµÇƒÇ¢ÇÈMA2ÇÃFMÉ`ÉÉÉìÉlÉãÇÃMAP
	UINT8 reqAdpcmCh;					// égópÇµÇƒÇ¢ÇÈMA2ÇÃADPCMÉ`ÉÉÉìÉlÉãÇÃMAP
} SOUNDSEQINFO, *PSOUNDSEQINFO;

/* SMAF Sequencer Sound function information */
//typedef struct _SoundSeqInfo{   
//	PSMAFSEQINFO hSeqInf;     /* Pointer to SMAF sequencer information */
//	PSMAF pSmaf;              /* SMAF data analysis information in process */
//	UINT8 playingMtrTrack;    /* Show score track ini use */
//	UINT32 reqFmCh;           /* MAP of MA2 FM channel in use */
//	UINT8 reqAdpcmCh;         /* MAP of MA2 ADPCM channel in use */
//	UINT8 modeLed;            /* LED mode setting at Smaf_Control() */
//	UINT8 modeVib;            /* Vibration mode setting at Smaf_Control() */
//	UINT8 modeKey;            /* Key control value setting at Smaf_Control() */
//	UINT8 syncLed;            /* synchronized channel for LED */
//	UINT8 syncVib;            /* synchronized channel for Vibrator */
//	UINT16 ctrlKey;           /* channel for Key control */
//} SOUNDSEQINFO, *PSOUNDSEQINFO;
/*
int Sound_Initialize();
int Sound_Seek(UINT32 pSoundSeq, UINT32 pos);
int Sound_Stop(UINT32 pSoundSeq);
int Sound_Start(UINT32 pSoundSeq);
int Sound_Standby(UINT32 pSoundSeq);
int Sound_Close(UINT32 pSoundSeq);
UINT32 Sound_Open(PSMAFSEQINFO pSeqInf);
int Sound_Control(UINT32 pSoundSeq, int ctrlNum, void *prm);
int Sound_SeekStartPoint(UINT32 pSoundSeq, UINT8 ctrl);
*/
int Sound_Initialize(void);
int Sound_Seek(int pSoundSeq, int pos);
int Sound_Stop(int pSoundSeq);
int Sound_Start(int pSoundSeq);
int Sound_Standby(int pSoundSeq);
int Sound_Close(int pSoundSeq);
UINT32 Sound_Open(PSMAFSEQINFO pSeqInf);
int Sound_Control(int pSoundSeq, int ctrlNum, void *prm);
int Sound_SeekStartPoint(int pSoundSeq, UINT8 ctrl);

//soundseq.h

typedef struct {
	uint8	EQ;
	uint8	HP;
	uint8	SPK;
	uint8	FM;
	uint8	PCM;
} MA2AudioGain;
typedef struct {
	uint16	EQ;
	uint16	HP;
	uint16	SPK;
	uint16	FM;
	uint16	PCM;
} MA2SoundLevel;
enum music_device_type {
  MUS_NONE			= 0x00,
  MUS_MELODY		= 0x01,
  MUS_EAR_MELODY	= 0x02,
  MUS_SPK_MELODY	= 0x04
};
typedef enum music_device_type music_acquire_type;
void MUSIC_hw_init(void);
void MUSIC_hw_power_dowm(void);
void MUSIC_hw_power(music_acquire_type dev_type);
void MUSIC_music_play(word nRepeatCount);
void MUSIC_music_stop(void);
void MUSIC_volume_control(MA2SoundLevel value);
void MUSIC_bell_sound_start(UINT8* pBufferData, MA2SoundLevel value, music_acquire_type dev_type);
void MUSIC_music_sound_start(word music_id, MA2SoundLevel value, music_acquire_type dev_type);
void MUSIC_music_buffer_start(UINT8* pBufferData, MA2SoundLevel value, music_acquire_type dev_type);
void MUSIC_music_file_start(char *file_name, MA2SoundLevel value, music_acquire_type dev_type);
void MUSIC_sound_play(void);


/*---------------------------------------------------------------------------
                        MELODY CHIP POWER MANAGEMENT VALUES

The following are the users of the melody.  To use the melody chip, it is
acquired, used, and then later released.
---------------------------------------------------------------------------*/
#define DP_ON	0x10
#define AP4_ON	0x08
#define AP3_ON	0x04
#define AP2_ON	0x02
#define AP1_ON	0x01

#define ANALOG_POWER_MASK  (AP4_ON | AP3_ON | AP2_ON | AP1_ON)
#define MELODY_POWER_MASK  (DP_ON | AP4_ON | AP3_ON | AP2_ON | AP1_ON)
#define EAR_MELODY_POWER_MASK  (DP_ON | AP4_ON | AP1_ON)
#define SPK_POWER_MASK     (DP_ON | AP3_ON | AP2_ON | AP1_ON)





#endif // #ifdef FEATURE_YMU759_MELODY // youandi 00.08.29 



/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#ifdef FEATURE_YMU757B_MELODY // jwpark 00.07.24



/////////////////////////////////////////////////////////////////////////////////
#define MUSIC_NOTE_REST_DATA_INDEX			0x00

#define MUSIC_1ST_TIMBRE_MODUL_INDEX1		0x10
#define MUSIC_1ST_TIMBRE_MODUL_INDEX2		0x11
#define MUSIC_1ST_TIMBRE_CARRIER_INDEX1		0x12
#define MUSIC_1ST_TIMBRE_CARRIER_INDEX2		0x13
#define MUSIC_2ND_TIMBRE_MODUL_INDEX1		0x14
#define MUSIC_2ND_TIMBRE_MODUL_INDEX2		0x15
#define MUSIC_2ND_TIMBRE_CARRIER_INDEX1		0x16
#define MUSIC_2ND_TIMBRE_CARRIER_INDEX2		0x17
#define MUSIC_3RD_TIMBRE_MODUL_INDEX1		0x18
#define MUSIC_3RD_TIMBRE_MODUL_INDEX2		0x19
#define MUSIC_3RD_TIMBRE_CARRIER_INDEX1		0x1A
#define MUSIC_3RD_TIMBRE_CARRIER_INDEX2		0x1B
#define MUSIC_4TH_TIMBRE_MODUL_INDEX1		0x1C
#define MUSIC_4TH_TIMBRE_MODUL_INDEX2		0x1D
#define MUSIC_4TH_TIMBRE_CARRIER_INDEX1		0x1E
#define MUSIC_4TH_TIMBRE_CARRIER_INDEX2		0x1F
#define MUSIC_5TH_TIMBRE_MODUL_INDEX1		0x20
#define MUSIC_5TH_TIMBRE_MODUL_INDEX2		0x21
#define MUSIC_5TH_TIMBRE_CARRIER_INDEX1		0x22
#define MUSIC_5TH_TIMBRE_CARRIER_INDEX2		0x23
#define MUSIC_6TH_TIMBRE_MODUL_INDEX1		0x24
#define MUSIC_6TH_TIMBRE_MODUL_INDEX2		0x25
#define MUSIC_6TH_TIMBRE_CARRIER_INDEX1		0x26
#define MUSIC_6TH_TIMBRE_CARRIER_INDEX2		0x27
#define MUSIC_7TH_TIMBRE_MODUL_INDEX1		0x28
#define MUSIC_7TH_TIMBRE_MODUL_INDEX2		0x29
#define MUSIC_7TH_TIMBRE_CARRIER_INDEX1		0x2A
#define MUSIC_7TH_TIMBRE_CARRIER_INDEX2		0x2B
#define MUSIC_8TH_TIMBRE_MODUL_INDEX1		0x2C
#define MUSIC_8TH_TIMBRE_MODUL_INDEX2		0x2D
#define MUSIC_8TH_TIMBRE_CARRIER_INDEX1		0x2E
#define MUSIC_8TH_TIMBRE_CARRIER_INDEX2		0x2F

#define MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX	0x30
#define MUSIC_TEMPO_DATA_INDEX				0x31
#define MUSIC_FM_CONTROL_INDEX				0x32
#define MUSIC_CLOCK_I_INDEX					0x33
#define MUSIC_IRQ_CONTROL_INDEX				0x34
#define MUSIC_SPK_VOLUME_INDEX				0x35
#define MUSIC_FM_VOLUME_INDEX				0x36
#define MUSIC_HPOUT_VOLUME_INDEX			0x37
#define MUSIC_POWER_MANAGE_INDEX			0x38


// SYNTH_NOTE_REST_DATA_INDEX	Index : 0x00

#define MUSIC_OCT_BLK_SET_M			0xC000	// b15, b14 : Octave Block setting
											// 0(rest), 1,2,3 

#define MUSIC_OCT_BLK_SET_SHIFT			14	// b15, b14 : Octave Block setting

#define MUSIC_PITCH_SET_M			0x3C00	// b13,b12,b11,b10 :Pitch setting
#define NOTE_C_SHARP				0x0400  // 0x01
#define NOTE_D						0x0800  // 0x02
#define NOTE_D_SHARP				0x0C00  // 0x03
#define NOTE_E						0x1400  // 0x05
#define NOTE_F						0x1800  // 0x06
#define NOTE_F_SHARP				0x1C00  // 0x07
#define NOTE_G						0x2400  // 0x09
#define NOTE_G_SHARP				0x2800  // 0x0A
#define NOTE_A						0x2C00  // 0x0B
#define NOTE_A_SHARP				0x3400  // 0x0D
#define NOTE_B						0x3800  // 0x0E
#define NOTE_C						0x3C00  // 0x0F

#define MUSIC_PART_SET_M			0x0300	// b9,b8 : Part setting
#define CHANNEL_0					0x0000	// Channel 0
#define CHANNEL_1					0x0100	// Channel 1
#define CHANNEL_2					0x0200	// Channel 2
#define CHANNEL_3					0x0300	// Channel 3

#define MUSIC_VIBRATO_SET_M			0x0080	// b7 : Vibrato set(6.4Hz, 13.47cent)
#define VIBRATO_ON					0x0080	// Set Vibrato on 
#define VIBRATO_OFF					0x0000	// Set Vibrato off

#define MUSIC_INTERVAL_SET_M		0x0078	// b6, b5, b4 , b3: Interval set
#define NO_INTERV					0x0000  // 0x00
#define A_TWENTYFOURTH_INTERV		0x0008  // 0x01
#define A_SIXTEENTH_INTERV			0x0010  // 0x02
#define A_TWELVETH_INTERV			0x0018  // 0x03
#define A_EIGHTH_INTERV				0x0020  // 0x04
#define A_SIXTH_INTERV				0x0028  // 0x05
#define THREE_SIXTEENTH_INTERV		0x0030  // 0x06
#define A_QUATER_INTERV				0x0038  // 0x07
#define THREE_EIGHTH_INTERV				0x0040  // 0x08
#define A_HALF_INTERV				0x0048  // 0x09
#define WHOLE_INTERV				0x0050  // 0x0A
//#define NO_INTERV					0x0058  // 0x0B
#define A_THIRD_INTERV				0x0060	// 0x0C
//#define A_HALF_INTERV				0x0068  // 0x0D
#define THREE_QUATER_INTERV			0x0070  // 0x0E
//#define WHOLE_INTERV				0x0078  // 0x0F

#define MUSIC_NOTE_LENGTH_M			0x0007	// b2, b1, b0 : sound length designation


#define MUSIC_TIMBER_CE_M			0x0080	// b7 : Timbre change enable
#define MUSIC_TIMBER_NO_M			0x0007	// b2, b1, b0 : changing Timbre number

//#define MUSIC_1ST_TIMBRE_MODUL_INDEX1		0x10

#define MUSIC_TIMBRE_MODUL_MASK		0xFFFF

#define MUSIC_TIMBRE_MULTIPLE_MASK		0xE0000000
#define MUSIC_TIMBRE_VIBRATO_MASK		0x10000000
#define MUSIC_TIMBRE_ENV_TYPE_MASK		0x08000000
#define MUSIC_TIMBRE_SUSTAIN_ONOFF_MASK	0x04000000
#define MUSIC_TIMBRE_RELEASE_L_MASK		0x03C00000
#define MUSIC_TIMBRE_DECAY_L_MASK		0x003C0000
#define MUSIC_TIMBRE_ATTACK_L_MASK		0x0003C000
#define MUSIC_TIMBRE_SUSTAIN_L_MASK		0x00003C00
#define MUSIC_TIMBRE_TOTAL_L_MASK		0x000003F0
#define MUSIC_TIMBRE_WAVEFORM_MASK		0x00000008
#define MUSIC_TIMBRE_FEEDBACK_MASK		0x00000007

#define MUSIC_TIMBRE_MULTIPLE_SHIFT			29
#define MUSIC_TIMBRE_VIBRATO_SHIFT			28
#define MUSIC_TIMBRE_ENV_TYPE_SHIFT			27
#define MUSIC_TIMBRE_SUSTAIN_ONOFF_SHIFT	26
#define MUSIC_TIMBRE_RELEASE_L_SHIFT		22
#define MUSIC_TIMBRE_DECAY_L_SHIFT			18
#define MUSIC_TIMBRE_ATTACK_L_SHIFT			14
#define MUSIC_TIMBRE_SUSTAIN_L_SHIFT		10
#define MUSIC_TIMBRE_TOTAL_L_SHIFT			4
#define MUSIC_TIMBRE_WAVEFORM_SHIFT			3
#define MUSIC_TIMBRE_FEEDBACK_SHIFT			0




//#define MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX	0x30

#define ALLOT_CH_0_MASK				0x0007
#define ALLOT_CH_1_MASK				0x0070
#define ALLOT_CH_2_MASK				0x0700
#define ALLOT_CH_3_MASK				0x7000

//#define MUSIC_TEMPO_DATA_INDEX				0x31

#define MUSIC_TEMPO_DATA_MASK		0x00FF

//#define MUSIC_FM_CONTROL_INDEX				0x32

#define FM_CONTOL_START_STOP_MASK	0x0001
#define FM_CONTOL_START				0x0001
#define FM_CONTOL_STOP				0x0000

#define FM_CONTOL_RESET_MASK		0x0002
#define FM_CONTOL_RESET				0x0002


//#define MUSIC_CLOCK_I_INDEX					0x33

#define MUSIC_CLOCK_I_SELECT_MASK	0x0007

#define CLOCK_I_2_688				0x0000
#define CLOCK_I_19_200				0x0001
#define CLOCK_I_19_680				0x0002
#define CLOCK_I_19_800				0x0003
#define CLOCK_I_8_400				0x0004
#define CLOCK_I_14_400				0x0005
#define CLOCK_I_27_821				0x0006
#define CLOCK_I_12_600				0x0007


//#define MUSIC_IRQ_CONTROL_INDEX			0x34

#define MUSIC_IRQ_ENABLE_MASK		0x0020
#define MUSIC_IRQ_POINT_MASK		0x001F

#define WAIT_FIFO_EMPTY_INT			0x0020
#define FILL_DATA_FIFO				0x0000


//#define MUSIC_SPK_VOLUME_INDEX			0x35

#define MUSIC_SPK_VOLUME_MASK		0x001F


//#define MUSIC_FM_VOLUME_INDEX				0x36
#define MUSIC_FM_VOLUME_MASK		0x001F

//#define MUSIC_HPOUT_VOLUME_INDEX			0x37
#define MUSIC_HPOUT_VOLUME_MASK		0x001F


//#define MUSIC_POWER_MANAGE_INDEX			0x38

#define ENTIRE_POWER_REDUCE_MASK	0x0001
#define ENTIRE_POWER_REDUCE_ON		0x0001
#define ENTIRE_POWER_REDUCE_OFF		0x0000

#define VREF_POWER_REDUCE_MASK		0x0002
#define VREF_POWER_REDUCE_ON		0x0002
#define VREF_POWER_REDUCE_OFF		0x0000

#define VOLUME_AMP_REDUCE_MASK		0x0004
#define VOLUME_AMP_REDUCE_ON		0x0004
#define VOLUME_AMP_REDUCE_OFF		0x0000

#define SPKOUT_REDUCE_MASK			0x0008
#define SPKOUT_REDUCE_ON			0x0008
#define SPKOUT_REDUCE_OFF			0x0000

#define EAR_DAC_REDUCE_MASK			0x0010
#define EAR_DAC_REDUCE_ON			0x0010
#define EAR_DAC_REDUCE_OFF			0x0000



#define MUSIC_REG_NUM				0x3A

#define MUSIC_ADDR_SIZ  8
#define MUSIC_ADDR_MSB  (((dword)0x01) << (MUSIC_ADDR_SIZ - 1) )

#define MUSIC_DATA_SIZ  16
#define MUSIC_DATA_MSB  (((dword)0x01) << (MUSIC_DATA_SIZ - 1) )

#define MUSIC_REFILL_REMEAINED		0x10

#define MUSIC_POWER_MASK(a,b) \
			(a&(1<<b))



/*---------------------------------------------------------------------------
                        MELODY CHIP POWER MANAGEMENT VALUES

The following are the users of the melody.  To use the melody chip, it is
acquired, used, and then later released.
---------------------------------------------------------------------------*/
#define DP_ON	0x10
#define AP4_ON	0x08
#define AP3_ON	0x04
#define AP2_ON	0x02
#define AP1_ON	0x01

#define ANALOG_POWER_MASK  (AP4_ON | AP3_ON | AP2_ON | AP1_ON)

#define MELODY_POWER_MASK  (DP_ON | AP4_ON | AP3_ON | AP2_ON | AP1_ON)

#define EAR_MELODY_POWER_MASK  (DP_ON | AP4_ON | AP1_ON)

#define SPK_POWER_MASK     (DP_ON | AP3_ON | AP2_ON | AP1_ON)



typedef enum {
  MUS_NONE			= 0x00,
  MUS_MELODY		= 0x01,
  MUS_EAR_MELODY	= 0x02,
  MUS_SPK			= 0x04
} mus_acquire_type;


/*---------------------------------------------------------------------------
                        MELODY CHIP VOLUME MANAGEMENT VALUES
---------------------------------------------------------------------------*/
typedef enum {
  MUS_FM_VOL		= 0x01,
  MUS_SPK_VOL		= 0x02,
  MUS_HPOUT_VOL		= 0x04
} mus_volume_ctr_type;

#define MELODY_VOL_MASK \
			(MUS_FM_VOL | MUS_SPK_VOL | MUS_HPOUT_VOL)

#define EAR_MELODY_VOL_MASK \
			MUS_HPOUT_VOL

#define SPK_VOL_MASK \
			MUS_SPK_VOL




/////////////////////////////////////////////////////////////////////////////////
extern void		MUSIC_hw_init(void);
extern void		MUSIC_power_on(mus_acquire_type, boolean, boolean);
extern void		MUSIC_power_off(mus_acquire_type, boolean, boolean);
extern void		MUSIC_power_real_off(void);
extern void		MUSIC_soundstart(void);
extern void		MUSIC_soundstop(void);
extern void		MUSIC_music_id(word, word);
extern void		MUSIC_volcmd(mus_volume_ctr_type , uint16);
extern void		MUSIC_sound_start(int songname, AudioTestGainType value);
extern void		MUSIC_sound_play(nRepeatCount);


/////////////////////////////////////////////////////////////////////////////////
extern rex_timer_type	snd_music_timer;
extern byte				MUSIC_power_status;
extern byte				MUSIC_power_real_status;
extern byte				MUSIC_power_sleep_status;
extern boolean			Music_active;
extern byte				MUSIC_power_status ;
extern byte				MUSIC_power_real_status ;
extern byte				MUSIC_power_sleep_status ;



#endif // #ifdef FEATURE_YMU757B_MELODY // jwpark 00.07.24 end


#endif //  YMU759_H