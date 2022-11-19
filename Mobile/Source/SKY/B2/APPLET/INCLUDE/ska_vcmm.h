/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_vcmm.h
**
** Description:
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-19   hjcho      Created.
**
** ============================================================================
*/

#ifndef _SKA_VCMM_H_
#define _SKA_VCMM_H_

#ifdef __cplusplus
extern "C" {
#endif
  
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
  
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define	MAX_RECORD_VOICE_LENGTH		0x1300	// max size : 0x12c0
#define MAX_FRAME_RECORDING_VOICE	200	    // 50[frames/sec] * 4[sec]
#define EPD_PROCESSING				0
#define EPD_SUCCESS					1
#define EPD_FAIL					2
#define EPD_NONE					100
#define VR_NONE						0
#define VR_REGIST					1
#define VR_RECOG					2
#define VR_MEMO						3
#define VR_PAUSE					4
#define VR_CALLREC					5
#define VR_AUTO						6
#define VR_RING						7
#define IDLE_VOCODER_DOWN			0
#define IDLE_VOCODER_WARMUP			1
#define IDLE_DECODER_PLAYBACK		2
#define IDLE_ENCODER_RECORD			3

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)

#define TOTAL_REC_TIME				270     // 4분30초  : 60[sec] x 4.5
#define TOTAL_REC_VOICE_MEMO_TIME   240
#define TOTAL_REC_USER_MENT_TIME     30
#define TOTAL_REC_BYTE				0x48000 // 4분30초 : 0x10000 x 4.5
#else
#define TOTAL_REC_TIME				180     // 3분 : 60[sec] x 3
#define TOTAL_REC_BYTE				0x30000 // 3분 : 0x10000 x 3
#endif //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)

#define FL_TOTAL_BYTE				(NV_FL_EOC*1024)
#define REC_DATA_HEADER_SIZE        8		
#define VTYPE_NONE					0
#define VTYPE_MEMO					1
#define VTYPE_NEWANS				2
#define VTYPE_OLDANS				3
#define VTYPE_USERMENT				4

/* vocoder를 release하기 전에 사용되는 time delay이다 */
#define VOCODER_DELAY               150



  
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef	struct
{
	word			SO; // service option   1. CAI_SO_VOICE_EVRC
						//					2. CAI_SO_VOICE_IS95A
						//			defined in "cai.h"
	unsigned long	dwRecordDataLength;
	unsigned char	*pData;
}	VOICE_RECORD_DATA;




typedef	struct
{
  unsigned long	dwDataLength;
  unsigned char	*pData;
            int iRepeatCount;
} VOICE_PLAY_DATA;



typedef	struct
{
	byte Fn;	//file number
	byte Type;  // file type
	word Time;  // time (sec)
} FN_SEQUENCE_DATA;



typedef struct tag_RECDATAHEADER_T            RECDATA_T;   
struct tag_RECDATAHEADER_T
{                    
    WORD      wServiceOption;         /* 0x0003 CAI_SO_VOICE_EVRC  */
    DWORD     dwTime;                 /* 0x0001 CAI_SO_VOICE_IS96A */
};


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
  
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
  
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern BOOL            gHoldOn;
extern BOOL            GetHoldOn(void);
extern void            SetHoldOn(BOOL fHoldOn);

extern VMEMOINFO_T     gVMemoInfo;  
extern VMEMOINFO_T*    GetVoiceInfo(void);
extern void            SetVoiceInfo(VMEMOINFO_T* Vmemo);

extern BYTE            gCurrentFn;
extern BYTE            GetCurrentFn(void);
extern void            SetCurrentFn(BYTE CurrentFn);

extern RECDATA_T       gRECDataHeader;
extern RECDATA_T*      GetRECDataHeader(void);
extern void            SetRECDataHeader(RECDATA_T* RECDataHeader);  

extern unsigned char VoiceDataBuffer[MAX_RECORD_VOICE_LENGTH];
extern unsigned char nVocoderReportControl ; 
extern VOICE_RECORD_DATA	RecoredVoiceData ;  /// 녹음을 위해 사용되는 버퍼
extern VOICE_PLAY_DATA		VoiceData; 
extern byte Current_Fn;
extern int	audio_recorded_packet_num;
extern word Data_Record_time ;

extern char AR_recording_Timer_str[];
extern INT8 gnVocoderMode;

extern boolean bPacketPlay;
extern boolean bFnPacketPlay;
extern boolean bPacketRecord;
extern boolean bFnPacketRecord;

extern byte rate_frame_sizes[7];
extern byte	packet_dec_rate[7];
extern long	dwVoiceDataOffset;

extern void  VM_record_timer_init(void);
extern void  VM_record_timer(void);
extern void  VM_time_display(byte);
extern void  Read_Current_Fn(byte );
extern void  Play_rec_init(word);
extern void  Fn_PLAY_voice_start(unsigned char);
extern void	 PLAY_voice_stop(void);
extern void  REC_voice_start(void);
extern void  REC_voice_stop(void );
extern void  REC_voice_stop_with_no_event(void);
extern void  Release_play_rec(void );
extern byte  VoiceRecordTransacStart(byte voice_type);
extern void  VoiceRecordTransacCancel(byte Fn, byte voice_type);
extern void  VoiceRecordTransacEnd(byte Fn, byte voice_type);
extern void  VoiceEraseTransac(byte Fn);
extern void  VoiceEraseAllTransac(void);
extern BYTE  GetRecordType(BYTE bIDX);
extern INT8  GetVocoderMode(void);
extern void  SetVocoderMode(INT8 nVMode);
extern WORD  GetVoiceMemoRemainTime(void);
extern WORD  GetUserMentRemainTime(void);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_VCMM_H_ */
















