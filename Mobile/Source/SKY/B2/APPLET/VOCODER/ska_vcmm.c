/*    Header include    */
#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "rex.h"
#include "ska.h"
/////////////////////////////////////////////////////////////////////////////
//
// ** Note : VocCommon.c는 TREX에서는 음성인식 부분이 추가되어 구성되어 있다.
//			하지만, A10에서는 사용하지 않는 부분이므로 제외하였다.
//          음성인식, 음성다이얼, EFS를 사용하지 않는 경우 등은 TREX의
//          소스를 참조해야 한다.
//
/////////////////////////////////////////////////////////////////////////////


#ifdef TREX_VOICE_APP
#include <stdio.h>
#include <string.h>
    

#include "bio.h"
#include "rex.h"
#include "misc.h"
#include "voc.h"
#include "voc_core.h"
#include "rf.h"
#include "enc.h"
#include "clk.h"
#include "err.h"
#include "sndi.h"
#include "sndihw.h"
#include "fs.h"
#include "nvim.h"
#include "fs_alloc.h"
#include "fs_dir.h"
#include "fsi.h"

#include "FileSys.h"


unsigned char		nVocoderReportControl = IDLE_VOCODER_DOWN; 
VOICE_RECORD_DATA   RecoredVoiceData      = {0, 0, VoiceDataBuffer};
word				Data_Record_time      = 0;
byte				rate_frame_sizes[7]   = {0, 2, 5, 10, 22, 0 , 22};
unsigned char		VoiceDataBuffer[MAX_RECORD_VOICE_LENGTH];
//nv_fn_sequence_type Fn_sequence[NV_MAX_FL_FILE_NUM];
FN_SEQUENCE_T       Fn_sequence[SKY_MAX_FL_FILE_NUM];
char				AR_recording_Timer_str[3];
int					audio_recorded_packet_num;
long				dwVoiceDataOffset;
VOICE_PLAY_DATA		VoiceData;
static boolean      defence_double;
static dword        Voice_play_length;
static word         RT_Refval;
static word         RT_Testval;
static qword        Real_Record_time;


byte				packet_dec_rate[7] = {
	// Vocoder data was blanked
	(VC_DEC_EIGHTH_V | VC_DEC_BLANK_V
					 | VC_DEC_NOT_ERASED_V | VC_DEC_FRAME_OK_V),
	// Rate 1/8 vocoder data
	(VC_DEC_EIGHTH_V | VC_DEC_NOT_BLANK_V
					 | VC_DEC_NOT_ERASED_V | VC_DEC_FRAME_OK_V),
	// Rate 1/4 vocoder data
	(VC_DEC_QTR_V    | VC_DEC_NOT_BLANK_V
					 | VC_DEC_NOT_ERASED_V | VC_DEC_FRAME_OK_V),
	// Rate 1/2 vocoder data
	(VC_DEC_HALF_V   | VC_DEC_NOT_BLANK_V
					 | VC_DEC_NOT_ERASED_V | VC_DEC_FRAME_OK_V),
	// Full rate vocoder data
	(VC_DEC_FULL_V   | VC_DEC_NOT_BLANK_V
					 | VC_DEC_NOT_ERASED_V | VC_DEC_FRAME_OK_V),
	// Frame erasure
	(VC_DEC_EIGHTH_V | VC_DEC_NOT_BLANK_V
					 | VC_DEC_ERASE_V      | VC_DEC_FRAME_OK_V),
	// Full-rate-likely data
	(VC_DEC_EIGHTH_V | VC_DEC_NOT_BLANK_V
					 | VC_DEC_NOT_ERASED_V | VC_DEC_FULL_LIKELY_V)};



static byte vc_enc_rates[4] = {
  VOC_1_RATE, // Full rate
  VOC_2_RATE, // Half rate
  VOC_4_RATE, // Quarter rate
  VOC_8_RATE  // Eighth rate
};


RECDATA_T   	gCurrentDataHeader;
BYTE            Current_Fn;
VMEMOINFO_T     gVMemoInfo; 
INT8            gnVocoderMode;
RECDATA_T       gRECDataHeader;
BOOL            gHoldOn;


BOOL GetHoldOn()
{
  return gHoldOn;
}

void SetHoldOn(BOOL fHoldOn)
{
  gHoldOn = fHoldOn;
}


INT8 GetVocoderMode()
{
  return gnVocoderMode;
}

void SetVocoderMode(INT8 nVMode)
{
  gnVocoderMode = nVMode;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetVoiceMemoRemainTime()
**
** Description: 
**     Return Voice Memo(voice memo, auto answer, call recording) remain time
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
WORD  GetVoiceMemoRemainTime()
{
    VMEMOINFO_T* VoiceInfo;
    WORD         wVoiceMemoRemainTime;

    VoiceInfo = GetVoiceInfo();
    wVoiceMemoRemainTime = TOTAL_REC_VOICE_MEMO_TIME - 
                           VoiceInfo->wVoiceMemoReocrdingTime;

    if((wVoiceMemoRemainTime < 0) || (wVoiceMemoRemainTime > TOTAL_REC_VOICE_MEMO_TIME))
    {
      VoiceInfo->wVoiceMemoReocrdingTime = TOTAL_REC_VOICE_MEMO_TIME;
      wVoiceMemoRemainTime = 0;
    }

    return wVoiceMemoRemainTime;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetVoiceMemoRemainTime()
**
** Description: 
**     Return Voice Memo(voice memo, auto answer, call recording) remain time
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
WORD  GetUserMentRemainTime()
{
    VMEMOINFO_T* VoiceInfo;
    WORD         wUserMentRemainTime;

    VoiceInfo = GetVoiceInfo();
    wUserMentRemainTime = TOTAL_REC_USER_MENT_TIME - 
                           VoiceInfo->wUsedTime;

    if((wUserMentRemainTime < 0) || (wUserMentRemainTime > TOTAL_REC_USER_MENT_TIME))
    {
      VoiceInfo->wUsedTime = TOTAL_REC_USER_MENT_TIME;
      wUserMentRemainTime = 0;
    }

    return wUserMentRemainTime;
}










/*===========================================================================
FUNCTION UI_GET_MC_BUF

DESCRIPTION
  Get an MC buffer for passing commands to MC

RETURN VALUE
  Pointer to the buffer - NULL if none available

DEPENDENCIES
  MC should be humming along and have the buffers ready

SIDE EFFECTS
  None
===========================================================================*/
mc_msg_type *ui_get_mc_buf( void )
{
  mc_msg_type *msg_ptr;

  if( ( msg_ptr = (mc_msg_type *) q_get( &mc_cmd_free_q )) == NULL ) 
  {
    //ERR_FATAL( "Couldn't get free MC buffer!  Horrors!", 0, 0, 0 );
  } else {
    /* Set up queue to place on when done, and */
    /* specify no task signalled when done     */
    msg_ptr->hdr.cmd_hdr.done_q_ptr = &mc_cmd_free_q;
    msg_ptr->hdr.cmd_hdr.task_ptr = NULL;
  }
  return( msg_ptr );
}



/*===========================================================================
FUNCTION UI_SEND_MC_CMD

DESCRIPTION
  Send MC_END_F or MC_POWERDOWN_F command to MC.

DEPENDENCIES

RETURNS
  boolean denoting successful send.
===========================================================================*/
boolean ui_send_mc_cmd ( mc_name_type cmd ) 
{
    mc_msg_type *mc_ptr;
//#ifdef FEATURE_EMAIL
//	extern void eml_release_cmd(void);
//	extern byte is_idle_state;
//	extern boolean ui_endkey_pressed;
//
//	if(ui.email_mode && (cmd == MC_END_F ) )
//	{
//     MSG_HIGH("ui_send_mc_cmd(MC_END_F) for Email",0,0,0);
//	   ui_endkey_pressed = TRUE;
//	   ui.email_val = 0;
//	   is_idle_state = 0; //000508
//	   eml_release_cmd() ;
//	   return( TRUE ) ;
// 	}
//#endif
//#if defined(FEATURE_NETGAME) || defined(FEATURE_IP_AIRDATA)		// heroine 00/08/10
//	if (ui.sockMode==TRUE && cmd==MC_END_F)
//	{
//		{
//			SvrLinkReleasebyUi();
//#ifdef FEATURE_IP_AIRDATA
//			if (!ui.ip_airdata)		
//#endif // FEATURE_IP_AIRDATA
//				ui.event = HS_END_K;
//			return (TRUE);
//		}
//	} 
//#endif	// #if defined(FEATURE_NETGAME) || defined(FEATURE_IP_AIRDATA)		// heroine 00/08/10

  mc_ptr = ui_get_mc_buf(); /* tell MC to end call */
  if( mc_ptr == NULL ) 
  {
//    if( ui.event == UI_OFFLINE_F ) 
//    {
//      ERR_FATAL( " ... can't terminate for offline", 0, 0, 0 );
//    } 
//    else 
//    {
//      MSG_HIGH( " ... no call termination sent", 0, 0, 0 );
//      ERR ("Can not get MC buffer",0,0,0 ) ;
//    }
    return (FALSE);
  }
//#ifdef FEATURE_DS
//  /* indicate that the UI is the one to end the call */
//  if( cmd == MC_END_F )
//  {
//    ui_ended_call = TRUE;
//  }
//#endif /* ifdef FEATURE_DS */
  mc_ptr->hdr.cmd = cmd;
  mc_cmd( mc_ptr );
  return (TRUE);
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetVoiceInfo()
**
** Description: 
**     Return a Pointer which have voice information
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VMEMOINFO_T* GetVoiceInfo()
{
#ifdef WIN32
    gVMemoInfo.bTotalRecNumber = 0;
    gVMemoInfo.wRemainedTime = 30;
    gVMemoInfo.dwRemainedByte = VM_MIN_REMAINED_BYTE+1;
#endif
  return &gVMemoInfo;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**    VOID SetVoiceInfo(VMEMOINFO_T* Vmemo)
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetVoiceInfo(VMEMOINFO_T* VMemo)
{
  memcpy(&gVMemoInfo, VMemo, sizeof(gVMemoInfo));
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetRECDataHeader(void)
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
RECDATA_T* GetRECDataHeader()
{
  return &gRECDataHeader;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetRECDataHeader(RECDATA_T* RECDataHeader)
{
  memcpy(&gRECDataHeader, RECDataHeader, sizeof(gRECDataHeader));
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetCurrentFn(void)
**
** Description: 
**     Get Current Fn
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BYTE GetCurrentFn()
{
  return Current_Fn; 
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetCurrentFn(BYTE CurrentFn)
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetCurrentFn(BYTE CurrentFn)
{
  Current_Fn = CurrentFn;
}

//아래의 함수는 더이상 필요치 않다. by happykid in 2001.5.3
// from voc_init_digital() in vocm2.c --------------------------------------->
/*
static void voc_init_Idle_play_rec(word SO)
{

  // Enable DEC PCM sample override
  VC_OUTM(VC_TEST_CONTROL_1,VC_TEST1_PCM_DEC_W_EN_M,VC_TEST1_PCM_DEC_W_ENA_V);


  // rate control
  if(gnVocoderMode == VR_RECOG || gnVocoderMode == VR_REGIST )
	{
	  VC_OUTM(VC_ENC_RATE_CONTROL, \
			  VC_INIT_BG_NOISE_M | VC_RATE_LIMIT_TYPE_M | VC_RATE_LIMIT_EN_M
			  | VC_BLANK_FRAME_M | VC_MAX_RATE_M	    | VC_MIN_RATE_M,
			  VC_INIT_BG_DONE_V  | VC_RATE_LIMIT_S_V    | VC_RATE_LIMIT_DIS_V
			  | VC_BLANK_DONE_V  | VC_MAX_FULL_V	    | VC_MIN_FULL_V);
	}
  else
	{
	  VC_OUTM(VC_ENC_RATE_CONTROL,
			  VC_INIT_BG_NOISE_M | VC_RATE_LIMIT_TYPE_M | VC_RATE_LIMIT_EN_M
			  | VC_BLANK_FRAME_M | VC_MAX_RATE_M	    | VC_MIN_RATE_M,
			  VC_INIT_BG_DONE_V  | VC_RATE_LIMIT_S_V    | VC_RATE_LIMIT_DIS_V
			  | VC_BLANK_DONE_V  | VC_MAX_FULL_V        | VC_MIN_EIGHTH_V);
	}


  // Encoder Rate Limit factor (encoder rate limiting is disabled above)
  VC_OUT(VC_ENC_RATE_LIMIT_FACTOR, 1);


  {
   	extern void vocm2_select_offsets(voc_capability_type local_option);
	// Select Vocoder TX & RX offsets, based on vc_local_option.
	vocm2_select_offsets((voc_capability_type)SO);
  }


  VC_OUT(VC_DEC_INT_ADVANCE, 0);


  // Set the encode and decode frame offsets
  {
    extern byte vc_rx_offset;
    VC_OUT(VC_DEC_FRAME_ADVANCE, vc_rx_offset);
  }


  // IO_CONTROL
  VC_OUTM(VC_IO_CONTROL,		\
		  VC_ENC_INT_EN_M		\
		  | VC_DEC_INT_EN_M		\
		  | VC_ENC_INT_POL_M	\
		  | VC_DEC_INT_POL_M,	\
		  VC_ENC_INT_ENA_V		\
		  | VC_DEC_INT_ENA_V	\
		  | VC_ENC_INT_HI_V		\
		  | VC_DEC_INT_HI_V);


  // Disable PCM loopback
  if( SO == VOC_CAP_EVRC)
	{
	  VC_OUTM(VC_MODE_CONTROL,			\
			  VC_MODE_PKT_LOOPBK_M		\
			  | VC_MODE_IS96_VER_M		\
			  | VC_MODE_B0_DC_BLOCK_M	\
			  | VC_SELECT_VOC_M			\
			  | VC_MODE_DIAG_EN_M		\
			  | VC_MODE_QTR_RATE_M,		\
			  VC_MODE_PKT_LOOPBK_DIS_V	\
			  | VC_MODE_IS96_V			\
			  | VC_MODE_SUB_DC_BLOCK_V	\
			  | VC_8K_V					\
			  | VC_MODE_DIAG_EN_DIS_V	\
			  |VC_MODE_QTR_RATE_DIS_V);
	}
  else
	{
	  VC_OUTM(VC_MODE_CONTROL,			\
			  VC_MODE_PKT_LOOPBK_M		\
			  | VC_MODE_IS96_VER_M		\
			  | VC_MODE_B0_DC_BLOCK_M	\
			  | VC_SELECT_VOC_M			\
			  | VC_MODE_DIAG_EN_M		\
			  | VC_MODE_QTR_RATE_M,		\
			  VC_MODE_PKT_LOOPBK_DIS_V	\
			  | VC_MODE_IS96A_V			\
			  | VC_MODE_SUB_DC_BLOCK_V	\
			  | VC_8K_V					\
			  | VC_MODE_DIAG_EN_DIS_V	\
			  |VC_MODE_QTR_RATE_ENA_V);
	}

  VC_OUTM(VC_MODE_CONTROL, VC_MODE_EXTPC_LPBK_M, VC_MODE_EXTPC_LPBK_DIS_V);

  // Reset the Vocoder Core again to put it into an idle state.
  if(SO == VOC_CAP_EVRC)
	{
	  VC_OUTM(VC_RESET, VC_RESET_FIRMWARE_START_M 
			  | VC_RESET_ROM_BANK_SEL_M
			  , VC_RESET_FIRMWARE_BOOT 
			  | VC_RESET_ROM_BANK_SEL_EVRC);
	}
  else
	{
	  VC_OUTM(VC_RESET, VC_RESET_FIRMWARE_START_M 
			  | VC_RESET_ROM_BANK_SEL_M
			  , VC_RESET_FIRMWARE_BOOT 
			  | VC_RESET_ROM_BANK_SEL_8K13K);
	}

  {
    extern byte vc_tx_offset;
	VC_OUT(VC_ENC_FRAME_ADVANCE, vc_tx_offset);
  }
  VC_OUTM(VC_TEST_CONTROL_1,VC_TEST1_PCM_DEC_W_EN_M,VC_TEST1_PCM_DEC_W_DIS_V);
}
// from voc_init_digital() in vocm2.c <---------------------------------------

*/


static void Fn_Packet_play(void)
{
  byte vc_dec_packet_format;
  byte packet_rate;
  byte play_buf[23];
  // Check the decoder interrupt status to assure that it is ready to
  // receive a frame.  If there is no interrupt, we are either not in
  // the decode window or the vocoder is not available.

  flfread(Current_Fn, &packet_rate , 1);

  if(VC_INM(VC_STATUS, VC_DEC_INT_M) == VC_DEC_INT_V)
    {
	  if(packet_rate <= 6)
		{
		  vc_dec_packet_format = packet_dec_rate[packet_rate];
		  vc_dec_packet_format |= VC_DEC_NO_FIR_V;

		  VC_OUT(VC_DEC_PACKET, vc_dec_packet_format);

		  if(packet_rate !=0)
			flfread(Current_Fn, play_buf, rate_frame_sizes[packet_rate]);

		  outsb_rep((void *)VC_DEC_PACKET,
					play_buf,
					rate_frame_sizes[packet_rate]);
		  dwVoiceDataOffset += (rate_frame_sizes[packet_rate] + 1);
		}
	  else
		{
		  dwVoiceDataOffset += (rate_frame_sizes[0] + 1);
		  flfread(Current_Fn , play_buf , rate_frame_sizes[0]);
		}
	}
  VC_OUTM(VC_CLEAR_ENC_INT, VC_CLEAR_ENC_INT_M, VC_CLEAR_ENC_INT_V);
  VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);

  audio_recorded_packet_num++;

  if(dwVoiceDataOffset >= Voice_play_length) PLAY_voice_stop();
}




static void Packet_Record(void)
{
  byte vc_enc_packet_format;
  byte packet_rate = 0;
  byte x;


  // Check the decoder interrupt status to assure that it is ready to
  // receive a frame.  If there is no interrupt, we are either not in
  // the decode window or the vocoder is not available.

  if(VC_INM(VC_STATUS, VC_ENC_INT_M) == VC_ENC_INT_V)
	{
	  vc_enc_packet_format = VC_IN(VC_ENC_PACKET);
	  
	  // If the the frame data is not blanked, read it out of the FIFO
	  if ((vc_enc_packet_format & VC_ENC_FMT_BLANK_M) == VC_ENC_NOT_BLANK_V)
		{
		  // Figure out the rate and size from the format byte
		  packet_rate =
			vc_enc_rates[vc_enc_packet_format & VC_ENC_FMT_RATE_M ];
			
		  if( packet_rate <= 4 && packet_rate > 0)
			{
			  // Read the packet data from the Vocoder's FIFO
			  for(x = 1; x <= rate_frame_sizes[ packet_rate ] ; x++)
				RecoredVoiceData.pData[dwVoiceDataOffset + x] =
				  inp((byte *)VC_ENC_PACKET);
			}
		  else  packet_rate = VOC_0_RATE;
		}
	  else
		{
		  // Blank
		  packet_rate = VOC_0_RATE;
		}
	  RecoredVoiceData.pData [dwVoiceDataOffset ] = packet_rate;
	  dwVoiceDataOffset += (rate_frame_sizes[ packet_rate ] + 1);
	}
  audio_recorded_packet_num ++;

  VC_OUTM(VC_CLEAR_ENC_INT, VC_CLEAR_ENC_INT_M, VC_CLEAR_ENC_INT_V);
  VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);
	
  if(audio_recorded_packet_num > MAX_FRAME_RECORDING_VOICE || //Voice Dialing
	 dwVoiceDataOffset >= MAX_RECORD_VOICE_LENGTH)            //Voice memo
	{
	  RecoredVoiceData.dwRecordDataLength = dwVoiceDataOffset;
	  REC_voice_stop();
	}
}




static void Fn_Packet_Record(void)
{
  byte rec_buf[23];
  byte vc_enc_packet_format;
  byte packet_rate;
  byte x;
  VMEMOINFO_T* VoiceInfo = GetVoiceInfo();


  // Check the decoder interrupt status to assure that it is ready to
  // receive a frame.  If there is no interrupt, we are either not in
  // the decode window or the vocoder is not available.

  if(VC_INM( VC_STATUS, VC_ENC_INT_M ) == VC_ENC_INT_V)
	{
	  vc_enc_packet_format = VC_IN(VC_ENC_PACKET);

	  // If the the frame data is not blanked, read it out of the FIFO
	  if ( (vc_enc_packet_format & VC_ENC_FMT_BLANK_M) == VC_ENC_NOT_BLANK_V )
		{

		  // Figure out the rate and size from the format byte
		  packet_rate = vc_enc_rates[vc_enc_packet_format & VC_ENC_FMT_RATE_M];

		  if( packet_rate <= 4 && packet_rate > 0)
			{
			  // Read the packet data from the Vocoder's FIFO
			  for(x = 1; x <= rate_frame_sizes[packet_rate]; x++)
				  rec_buf[x] = inp( (byte *) VC_ENC_PACKET);
			}
		  else packet_rate = VOC_0_RATE;
		}
	  else
		{
		  // Blank
		  packet_rate = VOC_0_RATE;
		}
	  rec_buf[0] = packet_rate;
	  flfwrite(Current_Fn, rec_buf , rate_frame_sizes[ packet_rate ]+1);
	  dwVoiceDataOffset += (rate_frame_sizes[ packet_rate ] + 1);
	}
  audio_recorded_packet_num ++;

  VC_OUTM(VC_CLEAR_ENC_INT, VC_CLEAR_ENC_INT_M, VC_CLEAR_ENC_INT_V);
  VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);

  if(Data_Record_time >= VoiceInfo->wRemainedTime)       
  {                                                      
    REC_voice_stop();                                    
  }                                                      
}




void VM_record_timer_init(void)
{
//  byte Minute_count, Second_count;
//
//  Minute_count = ui.voice_info.remained_time / 60;
//  Second_count = ui.voice_info.remained_time % 60;
//
//  clk_read_ms( Real_Record_time );
//  RT_Refval = qw_lo(Real_Record_time);
//
//  sprintf(AR_recording_Timer_str, "%d:%d%d\206\332          ", 
//		  Minute_count, 
//		  Second_count/10,
//		  Second_count%10);
//
//  make_bar_display(&(AR_recording_Timer_str[6]), 10, TOTAL_REC_TIME,
//				   ui.voice_info.remained_time); 
}



void VM_record_timer(void)
{
//  byte Minute_count, Second_count;
//
//  clk_read_ms( Real_Record_time );
//  RT_Refval = qw_lo(Real_Record_time) ;
//  Data_Record_time++;
//
//  if (ui.voice_info.remained_time < Data_Record_time)
//	Data_Record_time = ui.voice_info.remained_time; 
//
//  Minute_count = (ui.voice_info.remained_time - Data_Record_time) / 60;
//  Second_count = (ui.voice_info.remained_time - Data_Record_time) % 60;
//
//  sprintf(AR_recording_Timer_str, "%d:%d%d\206\332          ", 
//		  Minute_count, 
//		  Second_count/10,
//		  Second_count%10);
//
//  make_bar_display(&(AR_recording_Timer_str[6]), 10, TOTAL_REC_TIME,
//				   (ui.voice_info.remained_time - Data_Record_time)); 
//  kj_TextOutY(1, AR_recording_Timer_str);
}



void VM_time_display(byte line)
{
//  byte Minute_count, Second_count;
//
//  Minute_count = ui.voice_info.remained_time / 60;
//  Second_count = ui.voice_info.remained_time % 60;
//
//  sprintf(AR_recording_Timer_str , "남은시간 %d:%d%d", 
//		  Minute_count, 
//		  Second_count/10,
//		  Second_count%10);
//
//  kj_SetTextYCenterAlign(line, AR_recording_Timer_str);
}



void Read_Current_Fn(byte IDX)
{
  Current_Fn = Fn_sequence[IDX].Fn;
}



void RecDataIDplay(byte IDX)
{
//  if(     Fn_sequence[IDX].Type == VTYPE_MEMO)
//	kj_SetTextY(1,"\210\334\210\375\210\376\210\377" ); 
//  else if(Fn_sequence[IDX].Type == VTYPE_NEWANS)
//	kj_SetTextY(1,"\210\330\210\331\210\332\210\333" ); 
//  else if(Fn_sequence[IDX].Type == VTYPE_OLDANS)
//	kj_SetTextY(1,"\210\334\210\335\210\336\210\337" ); 
//  else
//	kj_SetTextY(1,"잘못된 음성" ); 
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sGetRecordType()
**
** Description: 
**     Play a saved voice memo.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BYTE GetRecordType(BYTE bIDX)
{
  if(Fn_sequence[bIDX].Type == VTYPE_MEMO)
  {
    return VTYPE_MEMO;
	//sprintf((CHAR*)pszVMemo, "%s", SKY_GET_TOKEN(TKN_VMEMO_VOICE_MEMO));
  }
  else if(Fn_sequence[bIDX].Type == VTYPE_NEWANS)
  {
    return VTYPE_NEWANS;
	//sprintf((CHAR*)pszVMemo, "%s", SKY_GET_TOKEN(TKN_VMEMO_NEW_AUTOANS));
  }
  else if(Fn_sequence[bIDX].Type == VTYPE_OLDANS)
  {
    return VTYPE_OLDANS;
	//sprintf((CHAR*)pszVMemo, "%s", SKY_GET_TOKEN(TKN_VMEMO_OLD_AUTOANS));
  }
  else
  {
	return VTYPE_NONE;
  }
    //sprintf((CHAR*)pszVMemo, "%s", SKY_GET_TOKEN(TKN_VMEMO_ERROR));
}




void Play_rec_init(word Serv_O)
{
  // EVRC Vocoder Initialization
  voc_acquire(VOC_ACQ_UI, NULL);
  if(voc_config(VOC_ACQ_UI, (voc_capability_type)Serv_O ) != VOC_DONE_S)
	{
//	  kj_TextOutY(0, "vocoder Error" );
//	  ERR_FATAL( "Bad vocoder status ", 0, 0, 0 );
	}
  nVocoderReportControl = IDLE_VOCODER_WARMUP;
  vmux_acquire();
//아래의 함수는 더이상 필요치 않다. by happykid in 2001.5.3
//  voc_init_Idle_play_rec(Serv_O);

//youandi 01.06.28 녹음/재생 시작부분 확인 바람 
//Play_rec_int는 불안함.
//Play_rec_init 앞에서 snd_task를 반드시 깨워야 함.
//보통 키톤함수를 사용한다. 
}




void Fn_PLAY_voice_start(byte Fn)
{
  BYTE i;
  //nv_voice_info_type v_info;
  //VOICEINFO_T  v_info;
  VMEMOINFO_T* VoiceInfo = GetVoiceInfo();

  if(nVocoderReportControl == IDLE_DECODER_PLAYBACK ||
	 nVocoderReportControl == IDLE_ENCODER_RECORD)
	return;

  for(i = 0; i < VoiceInfo->bTotalRecNumber; i++)
	{
	  if(Fn_sequence[i].Fn == Fn)
		{
		  if(Fn_sequence[i].Type == VTYPE_NEWANS)
			{
			  if (VoiceInfo->bNewAutoAnsNumber > 0)
				{
				  VoiceInfo->bNewAutoAnsNumber--;
                  SKY_SET_VOICE_INFO(VoiceInfo, sizeof(VMEMOINFO_T));

                  /* hjcho 01.05.12
                  v_info.total_rec_num = VoiceInfo->bTotalRecNumber;
				  v_info.new_ans_num   = VoiceInfo->bNewAutoAnsNumber;
				  v_info.is_user_voice = VoiceInfo->fUserVoice;
				  v_info.user_voice_fn = VoiceInfo->bUserVoiceFn;
				  v_info.user_voice_l  = VoiceInfo->wUsedTime;
				  v_info.remained_byte = VoiceInfo->dwRemainedByte;
				  v_info.remained_time = VoiceInfo->wRemainedTime;
                  SKY_PutNV(NV_VOICE_INFO_I, (nv_item_type *)&v_info);
                  */
				}
			  Fn_sequence[i].Type = VTYPE_OLDANS;
			  Fn_sequence[i].address = i;
              //SKY_PutNV(NV_FN_SEQUENCE_I, (nv_item_type *)&Fn_sequence[i]);
			  //hjcho-modify
			  //jrkwon SKY_SET_FN_SEQUENCE(i, Fn_sequence[i]);
			}
		  break;
		}
	}
    //jrkwon
  SKY_SET_FN_SEQUENCE_ALL(Fn_sequence);

  Current_Fn                = Fn;
  Voice_play_length         = flfsize(Fn) - REC_DATA_HEADER_SIZE;
  dwVoiceDataOffset         = 0;
  audio_recorded_packet_num = 0;
  nVocoderReportControl     = IDLE_DECODER_PLAYBACK;

  gnVocoderMode = VR_MEMO;

  VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);
  tramp_set_isr( TRAMP_VOC_DEC_ISR, Fn_Packet_play );
  VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);
  SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_MUTED, TRUE);
}




void PLAY_voice_stop(void)
{
  SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
  tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL);
  nVocoderReportControl = IDLE_VOCODER_DOWN;
  SKY_AddEvent(SE_VM_PLAY_END);
}



void PLAY_voice_stop_with_no_event(void)
{
  SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
  tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL);
  nVocoderReportControl = IDLE_VOCODER_DOWN;
}



void REC_voice_start(void )
{
  VMEMOINFO_T*  VoiceInfo = GetVoiceInfo();

  if(nVocoderReportControl == IDLE_DECODER_PLAYBACK ||
	 nVocoderReportControl == IDLE_ENCODER_RECORD)
	return;
  Voice_play_length         = VoiceInfo->wRemainedTime;
//  ui.pad                    = TRUE;
//  ui.multi_key              = FALSE;
  dwVoiceDataOffset         = 0;
  audio_recorded_packet_num = 0;
  Data_Record_time          = 0;
  nVocoderReportControl     = IDLE_ENCODER_RECORD;

  VC_OUTM(VC_CLEAR_ENC_INT, VC_CLEAR_ENC_INT_M, VC_CLEAR_ENC_INT_V);
  if(gnVocoderMode == VR_MEMO)
	tramp_set_isr(TRAMP_VOC_ENC_ISR, Fn_Packet_Record);
  else
	tramp_set_isr(TRAMP_VOC_ENC_ISR, Packet_Record);
  VC_OUTM(VC_CLEAR_ENC_INT, VC_CLEAR_ENC_INT_M, VC_CLEAR_ENC_INT_V);
  
  SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_UNMUTED, TRUE);
  VC_OUTM(VC_VOX_CONTROL, VC_VOX_TX_MUTE_M, VC_VOX_TX_MUTE_DIS_V);
}



void REC_voice_stop(void)
{
  SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
  tramp_set_isr(TRAMP_VOC_ENC_ISR, NULL);

//  ui.pad                              = FALSE;
//  ui.multi_key                        = TRUE;
  nVocoderReportControl               = IDLE_VOCODER_DOWN;
  RecoredVoiceData.dwRecordDataLength = dwVoiceDataOffset;
  SKY_AddEvent(SE_VM_REC_END);
}


void REC_voice_stop_with_no_event(void)
{
  SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
  tramp_set_isr(TRAMP_VOC_ENC_ISR, NULL);

//  ui.pad                              = FALSE;
//  ui.multi_key                        = TRUE;
  nVocoderReportControl               = IDLE_VOCODER_DOWN;
  RecoredVoiceData.dwRecordDataLength = dwVoiceDataOffset;
}



void Release_play_rec(void)
{
  SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);

  tramp_set_isr(TRAMP_VOC_ENC_ISR, NULL);
  tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL);
  
  voc_release(VOC_ACQ_UI);

//  ui.pad                = FALSE;
//  ui.multi_key          = TRUE;
  nVocoderReportControl = IDLE_VOCODER_DOWN;
}

void MakeBeep()
{
const int DATASIZE=20*18+3;
const byte beepdata[]={
      0x01,0x7f,0x00,0x01,0x7f,0x00,0x01,0x7f,0x00,0x01,0x7f,0x00,0x01,0x7f,0x00,0x01,0x7f,0x00,0x04,0x03,
	  0x69,0x2f,0x18,0xde,0x80,0x08,0x0a,0xca,0x04,0x00,0x03,0x00,0x02,0xe0,0x00,0x3e,0x1e,0x0a,0xeb,0xc0,
	  0x00,0x04,0x9b,0xc3,0xdb,0x68,0xa6,0x84,0x15,0x73,0x40,0x02,0x4e,0xd4,0xbe,0xbf,0x3b,0x7e,0xa0,0x2b,
	  0x54,0xc0,0x67,0x40,0x04,0x1b,0xc7,0x5b,0x68,0xb8,0x99,0x60,0x92,0x73,0x1f,0x9d,0xab,0xe1,0x44,0x62,
	  0x2d,0x5b,0xed,0x5d,0x96,0x05,0x40,0x04,0x1b,0xc7,0x5b,0x68,0xb8,0x36,0x79,0x21,0xe1,0x6f,0x8f,0x2f,
	  0x6f,0x6f,0x3a,0xed,0x46,0xe2,0x60,0xc6,0x9d,0x80,0x04,0x1b,0xc5,0xdb,0x68,0xb8,0x64,0x32,0x2a,0xe8,
	  0x07,0x6b,0x13,0x47,0x86,0x84,0x2f,0x80,0xd9,0x24,0xad,0xe5,0xc0,0x04,0x1b,0xc7,0x5b,0x68,0xa7,0xe0,
	  0xf0,0xf8,0x4c,0x2f,0x5b,0x3c,0x07,0xdd,0xf9,0x29,0x48,0x92,0xda,0x9c,0x6d,0x80,0x04,0x1b,0xc7,0x5b,
	  0x68,0x97,0xda,0x78,0x9a,0x20,0xa9,0x86,0x07,0x17,0x5e,0x75,0x32,0xb9,0x0c,0xd4,0xad,0x0e,0xc0,0x04,
	  0x1b,0xc7,0x5b,0x68,0xa8,0xa0,0x9a,0xf4,0x32,0xa5,0xb9,0xc8,0x66,0xb1,0xe0,0xf5,0x66,0xf6,0x23,0xdc,
	  0x9e,0x00,0x04,0x1b,0xc7,0x5b,0x68,0xb8,0xec,0x30,0xf5,0x8d,0x33,0x7d,0x2d,0x6e,0x05,0x2b,0xa9,0xb7,
	  0x9a,0x19,0x6c,0x35,0x00,0x04,0x1b,0xc7,0x5b,0x68,0xb8,0x61,0xe1,0x0c,0xfd,0x75,0x6b,0xad,0x54,0x6f,
	  0xf5,0xe9,0x99,0xe6,0xc8,0xca,0x84,0xc0,0x04,0x1b,0xc7,0x5b,0x68,0xa7,0xe0,0xf0,0x72,0x79,0x6f,0x59,
	  0xaf,0x66,0x37,0x7d,0x6f,0xa0,0xf9,0x0d,0x25,0x75,0x00,0x04,0x1b,0xc6,0x5b,0x68,0xa8,0x4d,0xb0,0x92,
	  0x28,0xa3,0x89,0xca,0x6f,0x5d,0xf3,0x73,0x79,0x0d,0xa7,0x89,0xbd,0xc0,0x04,0x1b,0xc7,0x5b,0x68,0xa8,
	  0x58,0xf3,0x81,0x60,0xbb,0x7b,0x86,0x5d,0x6e,0x67,0xb1,0x63,0xfc,0x0d,0xd2,0xde,0x00,0x04,0x1b,0xc7,
	  0x5b,0x68,0xa8,0x63,0x30,0xf0,0x73,0x3d,0x78,0x4f,0x2b,0x9e,0x2e,0xb3,0x62,0x4b,0xd3,0xc5,0x8e,0x40,
	  0x04,0x1b,0xc7,0x5b,0x68,0xb8,0xe2,0xa2,0x21,0x72,0xa5,0x5c,0x75,0x13,0xee,0x25,0x2b,0x94,0xe0,0x16,
	  0xbb,0x3c,0xc0
};
	flfwrite(Current_Fn, beepdata , DATASIZE);
	dwVoiceDataOffset += DATASIZE;
}


byte VoiceRecordTransacStart(byte voice_type)
{
  byte Fn = NV_MAX_FL_FILE_NUM;
  defence_double = FALSE;

  if((Fn = getfn()) < NV_MAX_FL_FILE_NUM)
	if(TRUE == flfopen(Fn, WR_MODE))
	{
		if(voice_type == VTYPE_MEMO || voice_type == VTYPE_USERMENT) 
			gnVocoderMode = VR_MEMO;
	return Fn;
	}

  return NV_MAX_FL_FILE_NUM; 
}




void VoiceRecordTransacCancel(byte fn, byte voice_type)
{
  VMEMOINFO_T*  VoiceInfo = GetVoiceInfo();

  if(defence_double) return;

  VoiceInfo->bUserVoiceFn = NV_MAX_FL_FILE_NUM;
  VoiceInfo->fUserVoice   = FALSE;
  VoiceInfo->wUsedTime    = 0;			

  flfclose(fn);
  flferase(fn);

  defence_double = TRUE;
}




void VoiceRecordTransacEnd(byte Fn, byte voice_type)
{
	
  BYTE i;
  //nv_voice_info_type voice_info;
  VMEMOINFO_T*  VoiceInfo =  GetVoiceInfo();
  //VMEMOINFO_T   SaveVoiceInfo;

  if(defence_double) return;

  flfclose(Fn);

  if(Fn_sequence[0].Fn == Fn) return;

  clk_read_ms( Real_Record_time );
  RT_Testval = qw_lo(Real_Record_time);
  
  if(RT_Testval - RT_Refval > 50)	Data_Record_time++;
  
  if(voice_type == VTYPE_MEMO || voice_type == VTYPE_NEWANS)
	{
	  for (i= VoiceInfo->bTotalRecNumber; i > 0;i--)
		memcpy((void*)&(Fn_sequence[i].Time),
			   (void*)&(Fn_sequence[i-1].Time), sizeof(nv_fn_sequence_type));
	  Fn_sequence[0].Fn   = Fn ;
	  Fn_sequence[0].Type = voice_type ;
	  Fn_sequence[0].Time = Data_Record_time;
	}

  if(voice_type == VTYPE_MEMO || voice_type == VTYPE_NEWANS)
	{
	  VoiceInfo->bTotalRecNumber++;
	  if(voice_type == VTYPE_NEWANS) VoiceInfo->bNewAutoAnsNumber++;
	}

  if (voice_type == VTYPE_USERMENT)
  {
      VoiceInfo->fUserVoice   =  TRUE;
      VoiceInfo->bUserVoiceFn =  Fn;
      VoiceInfo->wUsedTime    =  Data_Record_time;
  }
  else /* 01.05.13 인사말녹음시간과 음성녹음시간을 분리 하기 위해서 */
  {
      VoiceInfo->wVoiceMemoReocrdingTime
                              = VoiceInfo->wVoiceMemoReocrdingTime + Data_Record_time;
  }

  VoiceInfo->dwRemainedByte = (DWORD)getfreeindxnum() * 1024;

  if (VoiceInfo->wRemainedTime <= Data_Record_time)
	VoiceInfo->wRemainedTime = 0;
  else
	VoiceInfo->wRemainedTime -= Data_Record_time;

  if ((voice_type == VTYPE_MEMO) || (voice_type == VTYPE_NEWANS))
	{
	  for (i = 0; i < NV_MAX_FL_FILE_NUM; i++)
		{
		  Fn_sequence[i].address = i;
          //SKY_PutNV(NV_FN_SEQUENCE_I, (nv_item_type *)&Fn_sequence[i]);
		  //hjcho-modify
		  //jrkwon SKY_SET_FN_SEQUENCE(i, Fn_sequence[i]);
		}
        //jrkwon
        SKY_SET_FN_SEQUENCE_ALL(Fn_sequence);
	}
/*
   voice_info.total_rec_num = VoiceInfo->bTotalRecNumber;
   voice_info.new_ans_num   = VoiceInfo->bNewAutoAnsNumber;
   voice_info.is_user_voice = VoiceInfo->fUserVoice;
   voice_info.user_voice_fn = VoiceInfo->bUserVoiceFn;
   voice_info.user_voice_l  = VoiceInfo->wUsedTime; 
   voice_info.remained_byte = VoiceInfo->dwRemainedByte;
   voice_info.remained_time = VoiceInfo->wRemainedTime;
   memcpy(&SaveVoiceInfo, VoiceInfo, sizeof(SaveVoiceInfo));
   SKY_PutNV(NV_VOICE_INFO_I, (nv_item_type *)&SaveVoiceInfo);
*/
  SKY_SET_VOICE_INFO(VoiceInfo, sizeof(VMEMOINFO_T));

  defence_double = TRUE;
}



void VoiceEraseTransac(byte Fn)
{
  BYTE i, j;
  byte voice_type;
  dword voice_time=0;
  byte Fn_to_erase;
  VMEMOINFO_T*    VoiceInfo =  GetVoiceInfo();
//VMEMOINFO_T     SaveVoiceInfo;
//nv_voice_info_type voice_info;


  // find fn in Fnsequence[]
  for (i = 0; i < NV_MAX_FL_FILE_NUM; i++)
	{
	  if (Fn_sequence[i].Fn == Fn)
		break;
	}

  if (i < NV_MAX_FL_FILE_NUM) // Found in Fn_sequence[]
	{
	  voice_type = Fn_sequence[i].Type;
	  voice_time = Fn_sequence[i].Time;
	  Fn_to_erase = Fn_sequence[i].Fn; // Should be same as Fn.
	  j = i;

	  for (; i < VoiceInfo->bTotalRecNumber - 1; i++)
		memcpy((void *)&(Fn_sequence[i].Time),
			   (void*)&(Fn_sequence[i+1].Time),
			   sizeof(nv_fn_sequence_type));
				
	  Fn_sequence[VoiceInfo->bTotalRecNumber - 1].Fn = NV_MAX_FL_FILE_NUM;
	  Fn_sequence[VoiceInfo->bTotalRecNumber - 1].Type = VTYPE_NONE;
	  Fn_sequence[VoiceInfo->bTotalRecNumber - 1].Time = 0;

	  for (; j < NV_MAX_FL_FILE_NUM; j++)
		{
		  Fn_sequence[j].address = j;
		}

        SKY_SET_FN_SEQUENCE_ALL(Fn_sequence);

      if(VoiceInfo->bTotalRecNumber > 0)
	     VoiceInfo->bTotalRecNumber--;

      if (voice_type == VTYPE_NEWANS)
		if (VoiceInfo->bNewAutoAnsNumber > 0)
			VoiceInfo->bNewAutoAnsNumber--;
      
      /* 01.05.13 hjcho 인사말녹음시간과 음성메모를 분리 */
      VoiceInfo->wVoiceMemoReocrdingTime = VoiceInfo->wVoiceMemoReocrdingTime
                                           - voice_time;
      /* voice_time이 TOTAL_REC_VOICE_MEMO_TIME + 1로 저장되는 경우가 있다.*/
      if(voice_time > TOTAL_REC_VOICE_MEMO_TIME)
      {
          VoiceInfo->wVoiceMemoReocrdingTime = 0;
      }
	}
  else if((VoiceInfo->fUserVoice == TRUE) && 
		  (Fn == VoiceInfo->bUserVoiceFn))
	{
	  voice_type = VTYPE_USERMENT;
      voice_time = VoiceInfo->wUsedTime;
	  Fn_to_erase = VoiceInfo->bUserVoiceFn;

	  VoiceInfo->fUserVoice = FALSE;
	  VoiceInfo->bUserVoiceFn = NV_MAX_FL_FILE_NUM;
	  VoiceInfo->wUsedTime = 0;
	}

  VoiceInfo->wRemainedTime += voice_time;
  if(VoiceInfo->wRemainedTime > TOTAL_REC_TIME)
    VoiceInfo->wRemainedTime = TOTAL_REC_TIME;

/*
  voice_info.total_rec_num = VoiceInfo->bTotalRecNumber;
  voice_info.new_ans_num   = VoiceInfo->bNewAutoAnsNumber;
  voice_info.is_user_voice = VoiceInfo->fUserVoice;
  voice_info.user_voice_fn = VoiceInfo->bUserVoiceFn;
  voice_info.user_voice_l  = VoiceInfo->wUsedTime; 
  voice_info.remained_byte = VoiceInfo->dwRemainedByte; // NV_FL_EOC * 4 * 'cell size' = 249*4*256
  voice_info.remained_time = VoiceInfo->wRemainedTime;

  memcpy(&SaveVoiceInfo, VoiceInfo, sizeof(SaveVoiceInfo));
  SKY_PutNV(NV_VOICE_INFO_I, (nv_item_type *)&SaveVoiceInfo);
*/
  SKY_SET_VOICE_INFO(VoiceInfo, sizeof(VMEMOINFO_T));

  // Until now, you updated Fn_sequence and voice_info on EEPROM. 
  // Now, let's update FTAB, and map.

  erase_unused_Fn();

  VoiceInfo->dwRemainedByte = (dword)getfreeindxnum() * 1024;

}



void VoiceEraseAllTransac(void)
{
  BYTE i;
//nv_voice_info_type voice_info;
  int count;
//VMEMOINFO_T     SaveVoiceInfo;
  VMEMOINFO_T*    VoiceInfo = GetVoiceInfo();


  byte file_to_erase[NV_MAX_FL_FILE_NUM];
  byte num_to_erase;

//  DrawSandGlass(NULL,NULL);

  num_to_erase = VoiceInfo->bTotalRecNumber;

  if(num_to_erase != 0){
	for (i = num_to_erase; i > 0; i--)
	  {
//		DrawSandGlass(NULL,NULL);

		file_to_erase[i - 1] = Fn_sequence[i - 1].Fn;

		Fn_sequence[i - 1].Fn = NV_MAX_FL_FILE_NUM;
		Fn_sequence[i - 1].Type = VTYPE_NONE;
		Fn_sequence[i - 1].Time = 0;

		Fn_sequence[i - 1].address = i - 1;
        //SKY_PutNV(NV_FN_SEQUENCE_I, (nv_item_type *)&Fn_sequence[i - 1]);
		//jrkwon SKY_SET_FN_SEQUENCE((i-1), Fn_sequence[i-1]);
	  }
    //jrkwon
    SKY_SET_FN_SEQUENCE_ALL(Fn_sequence);
  }
/*
  voice_info.total_rec_num = VoiceInfo->bTotalRecNumber = 0;
  voice_info.new_ans_num   = VoiceInfo->bNewAutoAnsNumber = 0;
  voice_info.is_user_voice = VoiceInfo->fUserVoice;
  voice_info.user_voice_fn = VoiceInfo->bUserVoiceFn;
  voice_info.user_voice_l  = VoiceInfo->wUsedTime; 
  voice_info.remained_byte = VoiceInfo->dwRemainedByte = 0x28000*3; 

  if (VoiceInfo->fUserVoice == TRUE)
	voice_info.remained_time = VoiceInfo->wRemainedTime = TOTAL_REC_TIME - VoiceInfo->wUsedTime;
  else
 	voice_info.remained_time = VoiceInfo->wRemainedTime = TOTAL_REC_TIME;

  
  memcpy(&SaveVoiceInfo, VoiceInfo, sizeof(SaveVoiceInfo));
  SKY_PutNV(NV_VOICE_INFO_I, (nv_item_type *)&SaveVoiceInfo);
*/

  VoiceInfo->bTotalRecNumber = 0;
  VoiceInfo->bNewAutoAnsNumber = 0;
  VoiceInfo->dwRemainedByte = 0x28000*3; 

  if (VoiceInfo->fUserVoice == TRUE)                                 //
	VoiceInfo->wRemainedTime = TOTAL_REC_TIME - VoiceInfo->wUsedTime;  //
  else                                                               //
  VoiceInfo->wRemainedTime = TOTAL_REC_TIME;                         

  VoiceInfo->wVoiceMemoReocrdingTime = 0;
  SKY_SET_VOICE_INFO(VoiceInfo, sizeof(VMEMOINFO_T));
 
  // Until now, you updated Fn_sequence and voice_info on EEPROM. 
  // Now, let's update FTAB, and map.

  for(count=0;count<NV_MAX_FL_FILE_NUM;count++)
	{
	  if( (VoiceInfo->fUserVoice == TRUE) &&
		  (count == VoiceInfo->bUserVoiceFn))
		{
		  ; // 사용자동응답 녹음이 있으면 해당 녹음은 지우지 않는다.
		}
	  else
		{
//		  DrawSandGlass(NULL,NULL);
		  flferase(count);
		}
	}
  VoiceInfo->dwRemainedByte = (dword)getfreeindxnum()*1024;

}
#endif
