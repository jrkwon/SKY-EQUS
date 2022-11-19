/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_aapi.c
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
** 2000-12-27   hjcho      Created.
**
** ===========================================================================
*/
#include "customer.h"
#include "ska.h"

#include "voc_core.h"

#include "voc.h"
#include "cai.h"
#include "rxc.h"
#include "txc.h"
#include "FileSys.h"
#include "tramp.h"
#include "misc.h"
#include <memory.h>


#ifdef TREX_VOICE_APP

////////////////
//            //
//  exported  //
//            //
////////////////

boolean bRecordingTraffic = FALSE;
boolean bPlayingTraffic = FALSE;


static dword dwVocPacketOffset;

// Ment
#define MAX_VOICE_MENT_NUM	3



// defined in AnsMent.c
extern const unsigned char AnsMent1_EVRC[];
extern const unsigned char AnsMent2_EVRC[];
extern const unsigned char AnsMent3_EVRC[];
extern const unsigned char Ending_Beep[];
#if !defined(T_MSM31) && !defined(T_MSM5000)
//3100부터는 QCELP안쓴다.  2000.5.10 ( happykid )
extern const unsigned char AnsMent1_QCELP[];
extern const unsigned char AnsMent2_QCELP[];
extern const unsigned char AnsMent3_QCELP[];
#endif
#ifdef FEATURE_HOLDON
extern const unsigned char Hold_on_Ment[];
#endif

static const unsigned char	*VoiceDataEVRC[MAX_VOICE_MENT_NUM] = { AnsMent1_EVRC, AnsMent2_EVRC, AnsMent3_EVRC };
static const short			VoiceDataLengthEVRC[MAX_VOICE_MENT_NUM] = { 7475, 8603, 8616 };

//static const unsigned char	*VoiceDataEVRC[MAX_VOICE_MENT_NUM] = { AnsMent1_EVRC, AnsMent2_EVRC, AnsMent3_EVRC, Ending_Beep };
//static const short			VoiceDataLengthEVRC[MAX_VOICE_MENT_NUM] = { 7475, 8603, 8616, 363 };


#if !defined(T_MSM31) && !defined(T_MSM5000)
//3100부터는 QCELP안쓴다.  2000.5.10 ( happykid )
static const unsigned char	*VoiceDataIS96A[MAX_VOICE_MENT_NUM] = { AnsMent1_QCELP, AnsMent2_QCELP, AnsMent3_QCELP };
static const short			VoiceDataLengthIS96A[MAX_VOICE_MENT_NUM] = { 7350, 8466, 8503 };
#endif

#ifdef FEATURE_HOLDON
static const short			HoldOnVoiceDataLength = 3945;
#endif
static word VoiceData_SO ;
static byte open_fn;
static byte open_answertype;

static voc_rate_type tramp_voc_rate;
static byte tramp_voc_bits[22];


///////////////////////////////////////////////////////////
//
//		Internal routines. Used by rxc / txc
//
///////////////////////////////////////////////////////////
#ifdef FEATURE_FOLDER_PHONE
extern clk_cb_type poweron_led_cb;
extern void far ui_poweron_led_cb (
  int4  interval                  /* just ignore it */
/*lint -esym(715,interval) */
);
#endif


void REC_voice_start_traffic(byte fn)
{
    RECDATA_T*    CurrentDataHeader = GetRECDataHeader();

#ifdef FEATURE_FOLDER_PHONE
	if(!ui.earpieceup )
		clk_reg( &poweron_led_cb, ui_poweron_led_cb, 500L, 500L, TRUE);
#endif

	open_fn = fn;
    CurrentDataHeader->wServiceOption = VoiceData_SO = rxc_get_mode();
	
    if ((CurrentDataHeader->dwTime = SKY_GetLocalTime()) <= ONE_YEAR)
		CurrentDataHeader->dwTime = (dword)0;

    
    flfwrite(open_fn, (byte*)&(CurrentDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);

	// Should be after writing voice_data_header on the beginning part of a file.
	bRecordingTraffic = TRUE;
	VoiceData.dwDataLength = 0;
	dwVocPacketOffset = 0;
	Data_Record_time = 0;
	//youandi 01.06.28 녹음/재생 시작부분 확인 바람 
	//Play_rec_int는 불안함.
	//Play_rec_init 앞에서 snd_task를 반드시 깨워야 함.
	//보통 키톤함수를 사용한다. 여기는 통화중임으로 별다른 필요없음.
}

void REC_voice_stop_traffic(void)
{
#ifdef FEATURE_FOLDER_PHONE
	clk_dereg( &poweron_led_cb);
#endif

	bRecordingTraffic = FALSE;
    SKY_AddEvent(SE_VM_REC_END);
}


void Packet_play_traffic(void)
{
	byte vc_dec_packet_format;
	byte packet_rate;

	/* Check the decoder interrupt status to assure that it is ready to
	* receive a frame.  If there is no interrupt, we are either not in
	* the decode window or the vocoder is not available.
	*/

	packet_rate = tramp_voc_rate;

	if ( VC_INM( VC_STATUS, VC_DEC_INT_M ) == VC_DEC_INT_V )
    {
		if(packet_rate <= 4 && packet_rate > 0)
		{
			vc_dec_packet_format = packet_dec_rate[ packet_rate ];
			vc_dec_packet_format |= VC_DEC_NO_FIR_V;

			VC_OUT( VC_DEC_PACKET, vc_dec_packet_format );

			outsb_rep((void *)VC_DEC_PACKET, tramp_voc_bits,
					  rate_frame_sizes[ packet_rate ] );
		}
	}

	VC_OUTM( VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V );
}


boolean PLAY_voice_start_traffic(void)
{
	word SO;
//	RECDATA_T voice_data_header;
    VMEMOINFO_T*  VoiceInfo; 
    RECDATA_T*    CurrentDataHeader = GetRECDataHeader();
    
    VoiceInfo = GetVoiceInfo();

	SO = rxc_get_mode();
	if (SO == CAI_SO_VOICE_EVRC)
	{
		
        if (SKY_GET_AUTO_ANSWER_GREETING() == GM_USER) // user voice.
		{
			
            if (VoiceInfo->fUserVoice == FALSE) 
			{ // use general Auto-Answering Ment instead user-recorded Ment.
				open_answertype = 0;
				VoiceData.pData = (byte *)VoiceDataEVRC[0];
				VoiceData.dwDataLength = VoiceDataLengthEVRC[0];
			}
			else
			{
				// get SO of user recorded ment.
                open_fn = VoiceInfo->bUserVoiceFn;

				if (TRUE == flfopen(open_fn, RD_MODE))
				{
					flfread(open_fn, (byte *)&(CurrentDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);
					VoiceData_SO = CurrentDataHeader->wServiceOption;

					if (VoiceData_SO != CAI_SO_VOICE_EVRC)
					{ // Cannot use user-recorded voice, cause mis-matching service option.

						open_answertype = 0;
						VoiceData_SO = CAI_SO_VOICE_EVRC;
						VoiceData.pData = (byte *)VoiceDataEVRC[0];
						VoiceData.dwDataLength = VoiceDataLengthEVRC[0];

						flfclose(open_fn);
					}
					else
					{ // success.
						open_answertype = 3;
						VoiceData.dwDataLength = flfsize(open_fn) - REC_DATA_HEADER_SIZE;
					}
				}
				else
				{
					open_answertype = 0;
					VoiceData_SO = CAI_SO_VOICE_EVRC;
					VoiceData.pData = (byte *)VoiceDataEVRC[0];
					VoiceData.dwDataLength = VoiceDataLengthEVRC[0];
				}
			}

		}
		else
		{
			open_answertype = 0;
			VoiceData_SO = CAI_SO_VOICE_EVRC;
			VoiceData.pData = (byte *)VoiceDataEVRC[SKY_GET_AUTO_ANSWER_GREETING()];
			VoiceData.dwDataLength = VoiceDataLengthEVRC[SKY_GET_AUTO_ANSWER_GREETING()];
		}
		
		bPlayingTraffic = TRUE;
		dwVocPacketOffset = 0;

		// jwpark 00.10.05
		// for sounding on earpiece. -------------------------------------
		VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);
		tramp_set_isr(TRAMP_VOC_DEC_ISR, Packet_play_traffic);
		VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);
		//----------------------------------------------------------------

#ifdef FEATURE_FOLDER_PHONE
        if(SKY_IS_PHONE_CLOSE() && SKY_IS_EAR_JACK_OFF() && !SKY_IS_HANDS_FREE())
			VoiceSPKmute(TRUE);
#endif
	}
	else
	{
		open_answertype = 0;
		bPlayingTraffic = FALSE;
		SKY_AddEvent(SE_AANS_PLAY_END);
	}

	return TRUE;
	//youandi 01.06.28 녹음/재생 시작부분 확인 바람 
	//Play_rec_int는 불안함.
	//Play_rec_init 앞에서 snd_task를 반드시 깨워야 함.
	//보통 키톤함수를 사용한다. 여기는 통화중임으로 별다른 필요없음.
    //sSka_StartRecording
}




void PLAY_voice_stop_traffic(void)
{

	tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL);
	bPlayingTraffic = FALSE;
	SKY_AddEvent(SE_AANS_PLAY_END);
	if (open_answertype == 3)
		flfclose(open_fn);
	open_answertype = 0;
  //youandi 01.06.28 
  //tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL)를 빨리 처리할 필요가 있음.
  //이부분은 홀드온이나 자동응답 메세지를 스톱하는 것입니다.
  //콜엔드가 일어나고 홀드온이나 자동응답 메세지가 종료될 때 
  //메시지 종료시점이 너무 늦어지면 
  //보코더에 보이스팻킷이 남이있는 상태로 보코더릴리즈 되어 키톤이 깨집니다.
  //따라서 메세지 종료시점을 보코더 릴리즈(이건 콜 종료되면서릴리즈 되는 것입니다.)
  //보다 VOCODER_DELAY msec 전에 두어야 하며 최대한 콜엔드가 시작하는 앞단으로 위치시켜야 합니다.
  //기존의 방법은 UI_TASK의 콜엔드 처리부분에서 메세지를 스톱했는데
  //현재는 CM_TASK의 cmcall.c 의 cmcall_end에서 종료합니다.
}


void PLAY_voice_stop_traffic_with_noevent(void)
{
	tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL);
	bPlayingTraffic = FALSE;
	if (open_answertype == 3)
		flfclose(open_fn);
	open_answertype = 0;
  //youandi 01.06.28 
  //tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL)를 빨리 처리할 필요가 있음.
  //이부분은 홀드온이나 자동응답 메세지를 스톱하는 것입니다.
  //콜엔드가 일어나고 홀드온이나 자동응답 메세지가 종료될 때 
  //메시지 종료시점이 너무 늦어지면 
  //보코더에 보이스팻킷이 남이있는 상태로 보코더릴리즈 되어 키톤이 깨집니다.
  //따라서 메세지 종료시점을 보코더 릴리즈(이건 콜 종료되면서릴리즈 되는 것입니다.)
  //보다  VOCODER_DELAY msec 전에 두어야 하며 최대한 콜엔드가 시작하는 앞단으로 위치시켜야 합니다.
  //기존의 방법은 UI_TASK의 콜엔드 처리부분에서 메세지를 스톱했는데
  //현재는 CM_TASK의 cmcall.c 의 cmcall_end에서 종료합니다.
}



#ifdef FEATURE_HOLDON
boolean PLAY_HO_voice_start_traffic(void)
{
	VoiceData_SO = CAI_SO_VOICE_EVRC;
	VoiceData.pData = (byte *)Hold_on_Ment;
	VoiceData.dwDataLength = HoldOnVoiceDataLength;
// jwpark test for B1 hold-on
//    extern const unsigned char AnsMent1_QCELP[7350];
//	VoiceData_SO = CAI_SO_VOICE_IS96A;
//	VoiceData.pData = (byte *)AnsMent1_QCELP;
//	VoiceData.dwDataLength = 7350;
		
	bPlayingTraffic   = TRUE;
	dwVocPacketOffset = 0;
	open_answertype   = 0;

	// jwpark 00.10.05
	// for sounding on earpiece. -------------------------------------
	VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);
	tramp_set_isr(TRAMP_VOC_DEC_ISR, Packet_play_traffic);
	VC_OUTM(VC_CLEAR_DEC_INT, VC_CLEAR_DEC_INT_M, VC_CLEAR_DEC_INT_V);
	//----------------------------------------------------------------

	return TRUE;
}

void PLAY_HO_voice_stop_traffic(void)
{
	// jwpark 00.11.29
	tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL);

	bPlayingTraffic = FALSE;
//	ui_add_event(UI_PLAY_END_F);

	open_answertype = 0;
}

#endif

/*******************************************************

	REC_packet_traffic

	DESCRIPTION
 	save vocoder packet on FLASH ROM file.

	REVISION HISTORY
	jwpark          00.10.06        remove QDSP feature
	girin			07/15/1999		Created

	ARGUMENTS
	voc_bits		vocoder packet.
	                2 bytes of 1/8 rate, 22 bytes of full rate for EVRC.

	voc_rate		vocoder packet data rate.

	RETURN CODES 
	void

*******************************************************/
void REC_packet_traffic(byte *voc_bits, byte voc_rate) 
{
	VMEMOINFO_T*  VoiceInfo = GetVoiceInfo();
    
    if(voc_rate != VOC_0_RATE)
	{
		// save in FILE ----------------------------------------
		flfwrite(open_fn, &voc_rate, 1);
		flfwrite(open_fn, voc_bits, rate_frame_sizes[voc_rate]);

		dwVocPacketOffset += (rate_frame_sizes[voc_rate] + 1);
	}
	VoiceData.dwDataLength = dwVocPacketOffset;

    /* hjcho 01.05.13 인사말녹음과 음성녹음을 분리하기 위해 */
	//if(VoiceInfo->wRemainedTime <= Data_Record_time)
	//{
	//	REC_voice_stop_traffic();
	//}
    if(GetVoiceMemoRemainTime() <= Data_Record_time)
	{
	  REC_voice_stop_traffic();
	}
}


cai_data_rate_type PLAY_packet_traffic(byte *voc_bits) 
{
	byte x;
	voc_rate_type voc_rate;
	cai_data_rate_type data_rate;   /* Data rate of current frame */

	// set vocoder rate.
	if (open_answertype != 3)
	{
		voc_rate = (voc_rate_type)VoiceData.pData[dwVocPacketOffset];
//		voc_rate |= VC_DEC_NO_FIR_V;
	}
	else // user ment.
	{
		flfread(open_fn, &x, 1);
		voc_rate = (voc_rate_type)x;
	}

	// error vocoder rate check.
	if ((voc_rate <= 6) && (voc_rate >= 0))
	{
		// right vocoder rate
	}
	else
	{
#ifdef FEATURE_HOLDON
		if(GetHoldOn())
			PLAY_HO_voice_stop_traffic();
		else
			PLAY_voice_stop_traffic();
#else
		PLAY_voice_stop_traffic();
#endif
		return CAI_BLANK_RATE;
	}


	// fill the vocoder bits
	if (open_answertype != 3)
	{
		for (x = 1; x <= rate_frame_sizes[voc_rate]; x++)
		{
			voc_bits[x-1] = VoiceData.pData[dwVocPacketOffset + x];
		}
	}
	else
	{
		flfread(open_fn, voc_bits, rate_frame_sizes[voc_rate]);
	}
	

	// update play buffer offset.
	dwVocPacketOffset += (rate_frame_sizes[voc_rate] + 1);

	// give the vocoder packet to Vocoder as well as Layer2 (TX)
	tramp_voc_rate = voc_rate;
	memcpy(tramp_voc_bits, voc_bits, rate_frame_sizes[voc_rate]);

	//***** inserted by jnlee 00.2.1  -> packet_play_traffic을 대신함 ********
//	memcpy(qdsp_cmd_rx_packet, voc_bits, rate_frame_sizes[voc_rate]);
	// 자동응답 메세지 송출과 동시에 voc decoder에도 넣어준다. 00.2.7
//      if ( voc_rx_cmd ( tramp_voc_rate, tramp_voc_bits ) != VOC_DONE_S )
//      {
//        ERR( "Vocoder not responding: status %d", 0, 0, 0 );
//      }
	//************************************************************************

	if (dwVocPacketOffset >= VoiceData.dwDataLength) // Exceed Memory
	{
#ifdef FEATURE_HOLDON
		if(GetHoldOn())
			PLAY_HO_voice_stop_traffic();
		else
			PLAY_voice_stop_traffic();
#else
		PLAY_voice_stop_traffic();
#endif		
	}

	switch (voc_rate)
	{
	case VOC_8_RATE:
      data_rate = CAI_EIGHTH_RATE;
      break;
    case VOC_4_RATE:
      data_rate = CAI_QUARTER_RATE;
      break;
    case VOC_2_RATE:
      data_rate = CAI_HALF_RATE;
      break;
    case VOC_1_RATE:
      data_rate = CAI_FULL_RATE;
      break;
    default:
      data_rate = CAI_BLANK_RATE;
      break;
	}

	return (data_rate);
}



#endif //TREX_VOICE_APP

