#ifndef AUDTEST_H
#define AUDTEST_H

typedef struct {
	byte	MicSel;
	byte	AmpSel;
	byte	MicAmp1Gain;
    boolean MicAmp2Bpass;
    boolean TxHPFdis;
	boolean RxHPFdis;
	boolean TxSlopeFiltDis;
	sint15	CodecTxGain;
	sint15	CodecStGain;
	sint15	CodecRxGain;
	sint15	TxVolume;
	sint15	RxVolume;
	word	DtmfTxGain;
	sint15	DtmfRxGain;
	word	TxPCMFlt[7];
	word	RxPCMFlt[7];
	boolean NSSwitch;
	boolean ECSwitch;
	byte	ECmode;
#ifdef YAMAHA_MELODY
	byte	SPKvol;
	byte	FMvol;
	byte	HPvol;
#endif
#ifdef BUZZER_MELODY
	word	Ringerlevel;
#endif
	//오디오 테스트 아이템을 추가하려거든 기냥 추가하지 말고
	//아래 Reserved_audio에서 빼서 쓰기 바람. 전체 크기가 변하지 않도록.
	byte	Reserved_audio[20];
}audio_param_type;


// by heroine 00/09/26 building 에러 때문에 comment를 열었음.
/*
typedef struct 
{
	sint15 CodecRx;
	sint15 Rx;
	sint15 CodecTx;
	sint15 Tx;
	sint15 Dtmf;
	sint15 SideTone;
	byte SPKvol;
	byte FMvol;
	byte HPvol;
} ControlVol;
//#define TONE_TAG(m)  m[0],m[1],m[2]
//#define SOUND_TAG(m)  m[0],m[1]
*/
#endif // #ifndef AUDTEST_H