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
	//����� �׽�Ʈ �������� �߰��Ϸ��ŵ� ��� �߰����� ����
	//�Ʒ� Reserved_audio���� ���� ���� �ٶ�. ��ü ũ�Ⱑ ������ �ʵ���.
	byte	Reserved_audio[20];
}audio_param_type;


// by heroine 00/09/26 building ���� ������ comment�� ������.
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