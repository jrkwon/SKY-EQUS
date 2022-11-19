/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_mybe.c
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
** 2001-01-12   huni     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include <stdlib.h>

#include "ska.h"
#include "..\GVM\SWAP\Swap.h"
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#define  FILE_NAME_MBL       SKY_MBL_DIR""SKY_DIR_CHAR"MBL.SKY"

#define INIT_MBL_RECORD_ATTR        0
#define INIT_MBL_RECORD_USED        0
#define INIT_MBL_RECORD_DATE        0
#define INIT_MBL_RECORD_SIZE        0
#define INIT_MBL_RECORD_TEXT        0

#define MAX_MYBELL_BUF_SIZE         512

#define MYBELL_TITLE                8
#define MYBELL_SHINSEGI_TITLE       16

//message box user data
#define MYBELL_SAVE_SUCCESS         0x01
#define MYBELL_SAVE_FAIL            0x02
#define MYBELL_IN_SAVING            0x04
#define MYBELL_SAVE_LACK_SPACE      0x08
#define MYBELL_SAVE_EXIST_SAMENAME  0x10
#define MYBELL_SAVE_NOUSE_BELLNAME  0x20
#define MYBELL_SAVE_INPUT_BELLNAME  0x40
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/*
enum tag_SKA_MY_E {
    MB_SCROLL_LIST_1,
    MB_STATIC_TEXT_1,
    MB_STATIC_TEXT_2,
    MB_MAX
};
*/
/*
enum tag_SKA_MYBELL_SETTING_E {
    SS_MB_NAME_TITLE,
    SS_MB_NAME,
    SS_MB_SET,
    SS_MB_MAX
};
*/

enum tag_SKA_MYBELL_SETTING_E {
    SS_MB_NAME_TITLE,
    SS_MB_NAME,
    SS_MB_SET_TITLE,
    SS_MB_SET,
    SS_MB_MAX
};
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
mybell_buf_type       a_MybellBuffer[MAX_MYBELL_SIZE];
mybell_nv_type        MybellRecord;
mybell_nv_type        SoundPlayBuf;

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
word mb_MyBellPreBuffer[MYMELODY_DATA_MAX];

mus_compact_melody_type mb_MyBellPreBuffer_music = {
  SND_COMPACT_SOUND,
	  0,
  (mus_compact_tone_type *) mb_MyBellPreBuffer
}; 
#elif SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
BYTE mb_MyBellPreBuffer[MYMELODY_DATA_MAX]={0,}; 
#endif
/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
BYTE                        g_MyBellSize;
DWORD                       g_PlayMelSize;                  
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
STATIC	INT8		sMBList[MAX_MYBELL_SIZE];
BYTE                *paMBListBuf[MAX_MYBELL_SIZE];
STATIC  INT8        nMBIndex;

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
STATIC  BYTE        DMelodyName[9];
STATIC  BYTE        DMelodyLength;
STATIC  BYTE        DMelodyTempo;
#endif 


LOCAL BYTE  pNameBuf[SKY_FILE_NAME_MAX_LENGTH]; 

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sInitMybell( VOID );
LOCAL VOID SKA_SaveMybell(SKY_EVENT_T* pEvent);
VOID SKY_SeekReadMybellT( UINT8 unIdxOfMbl, mybell_nv_type* pReadRecord );
VOID SKY_SeekWriteMybellT( UINT8 unIdxOfMbl, mybell_nv_type* pWriteRecord );
void sDeleteMBMessage ( void );


#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
// SMS MyBellData
///////////////////////////////////////////////////////////////////////////////
//	Bit data patch utility
//
static	BYTE	*pBitData;
static	BYTE	nBitIndex;

static void	sInitBitData(BYTE* pData)
{
    pBitData  = pData;
    nBitIndex = 0x80;
}

static UINT16	sGetBitData(BYTE nLen)
{
    UINT16	data = 0;
    BYTE	i;

    for (i = 0; i < nLen; i++) 
    {
        data <<= 1;
        if (nBitIndex & *pBitData)
            data |= 0x01;
        nBitIndex >>= 1;
        if (nBitIndex == 0) 
        {
            nBitIndex = 0x80;
            pBitData++;
        }
    }
    return data;
}

///////////////////////////////////////////////////////////////////////////////
//	Set new download melody
//
void	skm_Skt2SkNote(BYTE Octave, BYTE Half, BYTE Height, BYTE Length)
{
    word note_l, note_h, note_t;

    const word	lengthTable[] = 
    { 
        NT_1,NT_2,NT_2D,NT_2D,NT_4,NT_4D,NT_4D,NT_8,NT_8D,NT_8D,NT_16,NT_16D,NT_16D,NT_32,NT_32,NT_32
    };

    const word	heightTable[5][3][7] =                          // Octave, Half, Height
    {	
        NT_C3P, NT_D3P, NT_E3, NT_F3P, NT_G3P, NT_A3P, NT_B3,
        NT_C3P,  NT_C3P, NT_D3P, NT_F3, NT_F3P,  NT_G3P, NT_A3P,
        NT_C3P, NT_D3, NT_E3, NT_F3, NT_G3,  NT_A3,  NT_B3,

        NT_C4P, NT_D4P, NT_E4, NT_F4P, NT_G4P, NT_A4P, NT_B4,
        NT_C4,  NT_C4P, NT_D4P, NT_F4, NT_F4P, NT_G4P, NT_A4P,
        NT_C4,  NT_D4,  NT_E4,  NT_F4, NT_G4,  NT_A4,  NT_B4,

        NT_C5P, NT_D5P, NT_E5, NT_F5P, NT_G5P, NT_A5P, NT_B5,
        NT_C5,  NT_C5P, NT_D5P, NT_F5, NT_F5P, NT_G5P, NT_A5P,
        NT_C5,  NT_D5,  NT_E5,  NT_F5, NT_G5,  NT_A5,  NT_B5,

        NT_C6P, NT_D6P, NT_E6, NT_F6P, NT_G6P, NT_A6P, NT_B6,
        NT_C6,  NT_C6P, NT_D6P, NT_F6, NT_F6P, NT_G6P, NT_A6P,
        NT_C6,  NT_D6,  NT_E6,  NT_F6, NT_G6,  NT_A6,  NT_B6,

        NT_C7P, NT_D7P, NT_E7, NT_F7P, NT_G7P, NT_A7P, NT_B7,
        NT_C7,  NT_C7P, NT_D7P, NT_F7, NT_F7P, NT_G7P, NT_A7P,
        NT_C7,  NT_D7,  NT_E7,  NT_F7, NT_G7,  NT_A7,  NT_B7
    };

    note_l = lengthTable[Length];

    if (Height == 0) 
    {
        note_h = SND_SILENCE;
    }
    else if (Height > 0 && Octave >= 3 && Octave <= 7) 
    {
        note_h = heightTable[Octave - 3][Half][Height - 1];
    }
    else 
    {
        if (Octave < 4)
            note_h = NT_C3P;
        else
            note_h = NT_B7;
    }

    //Conversion to ma1 note format
    if(note_l >= NT_1 && note_l <= NT_2D)
    {
        note_t = Convert_RingerToNote(note_h, note_l );
        mb_MyBellPreBuffer_music.tone_array[DMelodyLength+42] = note_t ;
        DMelodyLength++;
        if(note_l == NT_1)
        {
            note_t = Convert_RingerToNote(note_h, NT_2 );
            mb_MyBellPreBuffer_music.tone_array[DMelodyLength+42] = note_t ;
            DMelodyLength++;
        }
        else if(note_l == NT_2D)
        {
            note_t = Convert_RingerToNote(note_h, NT_4 );
            mb_MyBellPreBuffer_music.tone_array[DMelodyLength+42] = note_t ;
            DMelodyLength++;
        }
    }
    else
    {
        note_t = Convert_RingerToNote(note_h, note_l );
        mb_MyBellPreBuffer_music.tone_array[DMelodyLength+42] = note_t ;
        DMelodyLength++;
    }
}

///////////////////////////////////////////////////////////////////////////////
//	Mybell Melody parsing
//
void	skm_MelodyDownload(BYTE* pMelodyText)		        // For SKT Format
{
    BYTE	MainID, Option;
    BYTE	Octave = 4;									    // 옥타브
    BYTE	HalfTable[7] = { 2, 2, 2, 2, 2, 2, 2 };		    // 음고별 반음상태
    BYTE	Half = 0xFF;								    // 반음 입력상태
    BYTE	Height;										    // 음고
    BYTE	Length;										    // 음길이
    BYTE	Finish = 0;									    // 데이터끝
    BYTE	SubRoutine = 0;								    // 서브루틴?
    BYTE	Repeat, RepeatNum;							    // 반복처리
    BYTE	Label[10];									    // 서브루틴 레이블
    BYTE*	LabelAddr[10];								    // 서브루틴 주소
    BYTE	LabelSize = 0;								    // 서브루틴 갯수
    BYTE*	Stack[10];									    // 반복스택
    BYTE	StackSize = 0;								    // 반복스택 크기
    BYTE	Temp;
    UINT8	i;

    // Init Buffer
 	memset(mb_MyBellPreBuffer, 0x00, MYMELODY_DATA_MAX);

    // Set new melody flag
    sInitBitData(pMelodyText);

    // Header
    MainID = sGetBitData(4);						        // 주명령ID
    Option = sGetBitData(4);						        // 부명령ID
    sGetBitData(4);								            // 전체 메시지수
    sGetBitData(4);								            // 현 메시지 번호
    sGetBitData(4);								            // 전체 채널수
    sGetBitData(4);								            // 현 채널 번호

    DMelodyLength = 0;
    DMelodyTempo = (BYTE)sGetBitData(8)*2;			        // 곡 템포
    Make_MyMelody_Header( mb_MyBellPreBuffer_music.tone_array + 1, 11);

    mb_MyBellPreBuffer_music.tone_array[34] = (word)(8728 / DMelodyTempo) / 2; 

    if (Option == 0x01)                                     // 곡의 제목
    {							
        memset(DMelodyName, 0, 9);
        memcpy((CHAR*)DMelodyName, pBitData, 8);
        i = 8;
        while (i > 0 && DMelodyName[i - 1] == ' ') 
        {
            DMelodyName[i - 1] = '\0';
            i--;
        }
        sGetBitData(64);							        // 포인터 Skip
    }
    else
	    memcpy((char*)DMelodyName, SKY_GET_TOKEN(TKN_MYBELL_UNTITLE_NAME), 7); // 곡 제목 없음

    // Set melody notes
    while ( MainID == 1 && Finish != 3 &&
            *pBitData != 0 && DMelodyLength < MYMELODY_DATA_MAX - 3 ) 
    {

        if (1 == (BYTE)sGetBitData(1))                   // 음 형식
        {			

            Height = (BYTE)sGetBitData(3);			    // 음높이
            Length = (BYTE)sGetBitData(4);			    // 음길이

            if (SubRoutine == 0) 
            {
                if (Height == 0)					        // 쉼표
                    skm_Skt2SkNote(Octave, 0, Height, Length);
                else 
                {
                    if (Half < 0xFF)                        // 반음처리 요구
                    {				
                        HalfTable[Height - 1] = Half;
                        Half = 0xFF;
                    }
                    skm_Skt2SkNote(Octave, HalfTable[Height - 1], Height, Length);
                }
            }
        }
        else 
        {
            switch ((BYTE)sGetBitData(2)) 
            {
            case 0:									        // 종료 형식, 볼륨처리형식
                sGetBitData(5);					        // Ignore
                Finish = 3;
                break;

            case 1:									        // 옥타브 형식
                Temp = (BYTE)sGetBitData(3);		        // 옥타브
                sGetBitData(2);						    // 예비
                if (SubRoutine == 0) 
                {
                    Octave = Temp;
                }
                break;

            case 2:									        // 반음처리 형식
                Temp = (BYTE)sGetBitData(2);			    // 반음
                sGetBitData(3);						    // 예비
                if (SubRoutine == 0) 
                {
                    Half = Temp;
                }
                break;

            case 3:									        // 반복처리 형식
                Repeat = (BYTE)sGetBitData(2);		    // 반복부 처리
                RepeatNum = (BYTE)sGetBitData(3);	    // 반복부 번호

                switch (Repeat) 
                {
                case 0:									    // 시작
                    if (LabelSize == 10)
                        break;
                    Label[LabelSize] = RepeatNum;		    // Set Label
                    LabelAddr[LabelSize++] = pBitData;
                    SubRoutine = 1;
                    break;

                case 1:									    // 끝
                    if (StackSize  == 0) 
                    {
                        SubRoutine = 0;
                        break;
                    }
                    sInitBitData(Stack[--StackSize]);    // Return
                    break;

                case 2:									    // 호출
                    if (StackSize == 10)
                        break;
                    for (i = 0; i < LabelSize; i++)
                        if (Label[i] == RepeatNum) 
                        {
                            Stack[StackSize++] = pBitData;	// Save Return Point
                            sInitBitData(LabelAddr[i]);	// Call
                        }
                    break;
                }
                break;
            }
        }
    }
    mb_MyBellPreBuffer_music.tone_array[0] = (DMelodyLength + 42) * 2;
}

#elif SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE


////////////////////////////////////////////////////////////////////////////
//                          added by youandi    2001.02.16                 //            
////////////////////////////////////////////////////////////////////////////
#define  CONTROL_DATA_SIZE   30  // 1: 96  2: 30

static UINT8 CHNN=0x00;//,NoteTemp1=0x40,NoteTemp2=0x80,NoteTemp3=0xc0 ; // Channel 0,1,2,3
static UINT8 *pBitData;
static UINT16 nBitIndex;
static UINT16 MelodySize=0;
static UINT16 DataPoint=0;
static float MelodyTempo=0;
static UINT16 lpChunkSize=0;
static UINT16 lpScoreTrack=0;
static UINT16 lpSeqData=0;
static UINT8 IsFirstNote=0;
static UINT8 to=0x0c,re=0x02,mi=0x04,pa=0x05,sol=0x07,ra=0x09,si=0x0b,ShiftToHalf=0;

static void InitBitData(UINT8* pData)
{
	pBitData = pData;
	nBitIndex = 0x80;
}
static UINT16 GetBitData(UINT8 nLen)
{
	UINT16 data=0;
	UINT16  i;

	for(i=0; i<nLen; i++)
	{
		data<<=1;
		if(nBitIndex & *pBitData)
			data |= 0x01;
		nBitIndex >>=1;
		if(nBitIndex == 0)
		{
			nBitIndex = 0x80;
			pBitData++;
		}
	}
	return data;
}
static void PutCharData(UINT8 nData)
{
	mb_MyBellPreBuffer[DataPoint]=nData;
	++DataPoint;
}
static void PutDWordData(UINT32 nData)
{
	UINT32 nTemp;
	UINT8 nSwap;
	
	nTemp=nData;
	nTemp>>=24;
	nTemp = nTemp & 0x000000ff;
	nSwap =(UINT8)(nTemp);
	PutCharData(nSwap);

	nTemp=nData;
	nTemp>>=16;
	nTemp = nTemp & 0x000000ff;
	nSwap =(UINT8)(nTemp);
	PutCharData(nSwap);
		
	nTemp=nData;
	nTemp>>=8;
	nTemp = nTemp & 0x000000ff;
	nSwap =(UINT8)(nTemp);
	PutCharData(nSwap);
		
	nTemp=nData;
	nTemp = nTemp & 0x000000ff;
	nSwap =(UINT8)(nTemp);
	PutCharData(nSwap);
}
static void PutDWordDataL(UINT16 pDP, UINT32 nData)
{
	UINT32 nTemp;
	UINT8 nSwap;
	
	nTemp=nData;
	nTemp>>=24;
	nTemp = nTemp & 0x000000ff;
	nSwap =(UINT8)(nTemp);
	mb_MyBellPreBuffer[pDP]=nSwap;

	nTemp=nData;
	nTemp>>=16;
	nTemp = nTemp & 0x000000ff;
	nSwap =(UINT8)(nTemp);
	mb_MyBellPreBuffer[pDP+1]=nSwap;
		
	nTemp=nData;
	nTemp>>=8;
	nTemp = nTemp & 0x000000ff;
	nSwap =(UINT8)(nTemp);
	mb_MyBellPreBuffer[pDP+2]=nSwap;
		
	nTemp=nData;
	nTemp = nTemp & 0x000000ff;
	nSwap =(UINT8)(nTemp);
	mb_MyBellPreBuffer[pDP+3]=nSwap;
}


void MakeSMAFEnd(void)
{
	//NOP
//	PutCharData(0x00);	
	PutCharData(0xff);
	MelodySize++;
	PutCharData(0x00);
	MelodySize++;
	PutCharData(0x00);
	MelodySize++;
	PutCharData(0xff);
	MelodySize++;
	PutCharData(0x00);
	MelodySize++;
	//END of DATA
	PutCharData(0x00);
	MelodySize++;
	PutCharData(0x00);
	MelodySize++;
	PutCharData(0x00);
	MelodySize++;
	PutCharData(0x00);
	MelodySize++;
	
	//DATA SIZE
	PutDWordDataL(lpChunkSize,8+5+8+6+8+CONTROL_DATA_SIZE+8+MelodySize+2);
	PutDWordDataL(lpScoreTrack,6+8+CONTROL_DATA_SIZE+8+MelodySize);
	PutDWordDataL(lpSeqData,MelodySize);
    g_PlayMelSize = 8+8+5+8+8+CONTROL_DATA_SIZE+8+MelodySize+6+2;

	//Make CRC
	PutCharData(0x17);	PutCharData(0x1b);
}
void MakeSMAFHeader(void)
{
/*
const UINT8 ControlDataSample[CONTROL_DATA_SIZE]={ 
0xff, 0xf0, 0x05, 0x43, 0x02, 0x80, 0x15, 0xf7, 
0xff, 0xf0, 0x13, 0x43, 0x02, 0x00, 0x00, 0x00, 0x21, 0x7c, 0x1f, 0xcd, 0x94, 0xb4, 0x9b, 0xf8, 0x08, 0x00, 0x35, 0x00, 0x00, 0xf7, \
0xff, 0xf0, 0x13, 0x43, 0x02, 0x01, 0x00, 0x01, 0x62, 0xc0, 0x2a, 0x0f, 0xfa, 0x59, 0x9b, 0xfc, 0x00, 0x00, 0x31, 0x62, 0x00, 0xf7, \
0xff, 0xf0, 0x13, 0x43, 0x02, 0x02, 0x00, 0x02, 0xf1, 0xb4, 0x25, 0x43, 0xf8, 0x95, 0x1b, 0xe8, 0x00, 0x00, 0x54, 0x64, 0x00, 0xf7, \
0xff, 0xf0, 0x13, 0x43, 0x02, 0x03, 0x00, 0x03, 0xf0, 0x0a, 0xd3, 0xfd, 0x65, 0x1a, 0x97, 0xf8, 0x00, 0x00, 0x11, 0x22, 0x44, 0xf7 };
*/
	const UINT8 ControlDataSample[CONTROL_DATA_SIZE]={ 
		0xff, 0xf0, 0x05, 0x43, 0x02, 0x80, 0x15, 0xf7, 
		0xff, 0xf0, 0x13, 0x43, 0x02, 0x00, 0x00, 0x00, 0xf1, 0x4b, 0x23, 0x45, 0x66, 0x5b, 0x3f, 0x40, 0x00, 0x00, 0x11, 0x22, 0x44, 0xf7
	}; 
//GRAND PIANO
/*	const UINT8 ControlDataSample[CONTROL_DATA_SIZE]={ 
		0xff, 0xf0, 0x05, 0x43, 0x02, 0x80, 0x15, 0xf7, 
		0xff, 0xf0, 0x13, 0x43, 0x02, 0x00, 0x00, 0x00, 0xf1, 0x71, 0x7b, 0x89, 0x36, 0x20, 0x5f, 0xa8, 0x00, 0x00, 0x11, 0x22, 0x44, 0xf7
	}; */
	UINT16 i;

	for(i=0; i<MYMELODY_DATA_MAX; ++i)
		mb_MyBellPreBuffer[i]=0;

	//"MMMD"
	PutCharData(0x4d);	PutCharData(0x4d);	
	PutCharData(0x4d);	PutCharData(0x44);
	//ChunkSize = ContentInfoHeader(8) + ContentInfo(5)+
	//                  ScoreTrackHeader(8) + ScoreTrack(ScoreTrackSize) + CRC(2)
	lpChunkSize=DataPoint;
	PutDWordData(0x00000000);//XXXX
	
	//"CNTI"  - ContentInfoType
	PutCharData(0x43);	PutCharData(0x4e);	
	PutCharData(0x54);	PutCharData(0x49);
	//ContentInfoSize = 5
	PutDWordData(0x00000005);
	//ContentInfo
	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0x00);	PutCharData(0x00);  PutCharData(0x00);

	//"MTR0" - ScoreTrackType
	PutCharData(0x4d);	PutCharData(0x54);	
	PutCharData(0x52);	PutCharData(0x00);
	//ScoreTrackSize = ScoreTrackInfo(6) + SetupDataHeader(8) + SetupData(96)+
	//					 SquenceDataHeader(8) + 
	lpScoreTrack=DataPoint;
	PutDWordData(0x00000000);//XXXX
	//ScoreTrackInfo
	PutCharData(0x00);	PutCharData(0x00);  PutCharData(0x02);
	PutCharData(0x02);	PutCharData(0x00);  PutCharData(0x00);

	//"Mtsu" SetupDataInfotype
	PutCharData(0x4d);	PutCharData(0x74);	
	PutCharData(0x73);	PutCharData(0x75);
	//SetupDataSize
//	PutDWordData(0x00000060);
	PutDWordData(CONTROL_DATA_SIZE);
	//SetupDataInfo
	for(i=0; i<CONTROL_DATA_SIZE; ++i)
		PutCharData((UINT8)(ControlDataSample[i]));
	
	//"Mtsq" SequenceDataInfotype
	PutCharData(0x4d);	PutCharData(0x74);	
	PutCharData(0x73);	PutCharData(0x71);
	//SeqeunceDataSize
	lpSeqData=DataPoint;
	PutDWordData(0x00000000);//XXXX
}
void MakeSMAFHeader_1msec(void)
{
	const UINT8 ControlDataSample[CONTROL_DATA_SIZE]={ 
		0xff, 0xf0, 0x05, 0x43, 0x02, 0x80, 0x15, 0xf7, 
		0xff, 0xf0, 0x13, 0x43, 0x02, 0x00, 0x00, 0x00, 0xf1, 0x4b, 0x23, 0x45, 0x66, 0x5b, 0x3f, 0x40, 0x00, 0x00, 0x11, 0x22, 0x44, 0xf7
	}; 
	UINT16 i;

	for(i=0; i<MYMELODY_DATA_MAX; ++i)
		mb_MyBellPreBuffer[i]=0;

	//"MMMD"
	PutCharData(0x4d);	PutCharData(0x4d);	
	PutCharData(0x4d);	PutCharData(0x44);
	//ChunkSize = ContentInfoHeader(8) + ContentInfo(5)+
	//                  ScoreTrackHeader(8) + ScoreTrack(ScoreTrackSize) + CRC(2)
	lpChunkSize=DataPoint;
	PutDWordData(0x00000000);//XXXX
	
	//"CNTI"  - ContentInfoType
	PutCharData(0x43);	PutCharData(0x4e);	
	PutCharData(0x54);	PutCharData(0x49);
	//ContentInfoSize = 5
	PutDWordData(0x00000005);
	//ContentInfo
	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0x00);	PutCharData(0x00);  PutCharData(0x00);

	//"MTR0" - ScoreTrackType
	PutCharData(0x4d);	PutCharData(0x54);	
	PutCharData(0x52);	PutCharData(0x00);
	//ScoreTrackSize = ScoreTrackInfo(6) + SetupDataHeader(8) + SetupData(96)+
	//					 SquenceDataHeader(8) + 
	lpScoreTrack=DataPoint;
	PutDWordData(0x00000000);//XXXX
	//ScoreTrackInfo
	PutCharData(0x00);	PutCharData(0x00);  PutCharData(0x00);
	PutCharData(0x00);	PutCharData(0x00);  PutCharData(0x00);

	//"Mtsu" SetupDataInfotype
	PutCharData(0x4d);	PutCharData(0x74);	
	PutCharData(0x73);	PutCharData(0x75);
	//SetupDataSize
//	PutDWordData(0x00000060);
	PutDWordData(CONTROL_DATA_SIZE);
	//SetupDataInfo
	for(i=0; i<CONTROL_DATA_SIZE; ++i)
		PutCharData((UINT8)(ControlDataSample[i]));
	
	//"Mtsq" SequenceDataInfotype
	PutCharData(0x4d);	PutCharData(0x74);	
	PutCharData(0x73);	PutCharData(0x71);
	//SeqeunceDataSize
	lpSeqData=DataPoint;
	PutDWordData(0x00000000);//XXXX
}
void MakeCONTROLHeader(void)
{
	PutCharData(0x00);	PutCharData(0xff);	
	PutCharData(0xf0);	PutCharData(0x06);
	PutCharData(0x43);	PutCharData(0x01);	
	PutCharData(0x80);	PutCharData(0x31);
	PutCharData(0x11);	PutCharData(0xf7);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0x31);	PutCharData(0x00);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0x71);	PutCharData(0x00);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0xb1);	PutCharData(0x00);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0xf1);	PutCharData(0x00);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0x30);	PutCharData(0x00);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0x70);	PutCharData(0x00);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0xb0);	PutCharData(0x00);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0xf0);	PutCharData(0x00);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0x37);	PutCharData(0xf7);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0x77);	PutCharData(0xf7);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0xb7);	PutCharData(0x7f);

	PutCharData(0x00);	PutCharData(0x00);	
	PutCharData(0xf7);	PutCharData(0x7f);
}
static void MyBell2SMAFNote(UINT8 Octave, UINT8 Half, UINT8 Height, UINT8 Length, UINT8 HalfNote)
{
	UINT8 NoteTemp=0x00;
	UINT8 OctaveTemp=Octave;
	float q=0;
	UINT8  nTime,nGateTime;
	UINT16 nDuration=0;

	switch(HalfNote)
	{
		case 1:	
			if(Half == 0){
				to=0x01;
				ShiftToHalf=1;
			}
			else if(Half == 2){
				to=0x0c;
				ShiftToHalf=0;
			}
			break;
		case 2:
			if(Half == 0){
				re=0x03;
			}
			else if(Half == 1){
				re=0x01;
			}
			else if(Half == 2){
				re=0x02;
			}
			break;
		case 3:
			if(Half == 1){
				mi=0x03;
			}
			else if(Half == 2){
				mi=0x04;
			}
			break;
		case 4:
			if(Half == 0){
				pa=0x06;
			}
			else if(Half == 2){
				pa=0x05;
			}
			break;
		case 5:
			if(Half == 0){
				sol=0x08;
			}
			else if(Half == 1){
				sol=0x06;
			}
			else if(Half == 2){
				sol=0x07;
			}
			break;
		case 6:
			if(Half == 0){
				ra=0x0a;
			}
			else if(Half == 1){
				ra=0x08;
			}
			else if(Half == 2){
				ra=0x09;
			}
			break;
		case 7:
			if(Half == 1){
				si=0x0a;
			}
			else if(Half == 2){
				si=0x0b;
			}
			break;
    }

    switch(Height)	// Key Code
	{
		case 1:			NoteTemp = CHNN | to;
			Octave = Octave-1+ShiftToHalf;
			break;
		case 2:			NoteTemp = CHNN | re;
			break;
		case 3:			NoteTemp = CHNN | mi;
			break;
		case 4:			NoteTemp = CHNN | pa;
			break;
		case 5:			NoteTemp = CHNN | sol;
			break;
		case 6:			NoteTemp = CHNN | ra;
			break;
		case 7:			NoteTemp = CHNN | si;
			break;
    }					// Key Code

	if(IsFirstNote == 0){
		PutCharData(0x09);
		MelodySize++;
		IsFirstNote = 1;
	}
	
	switch(Octave)		// Octave    
	{
		case 0:	
			OctaveTemp = 0x00;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x82);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		case 1:
			OctaveTemp = 0x00;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x81);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		case 2:			OctaveTemp = 0x00;
			break;
		case 3:			OctaveTemp = 0x01;
			break;
		case 4:			OctaveTemp = 0x02;
			break;
		case 5:			OctaveTemp = 0x03;
			break;
		case 6:			
			OctaveTemp = 0x03;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x01);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		case 7:			
			OctaveTemp = 0x03;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x02);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		default:		OctaveTemp = 0x01;
	}					// Octave  
  
	// Legnth    
	q=(float) (60/MelodyTempo);
	switch(Length)
	{
		case 0:			nDuration = (UINT16)(4*q*250);		
			break;
		case 1:			nDuration = (UINT16)(2*q*250);		
			break;
		case 2:			nDuration = (UINT16)(3*q*250);		
			break;
		case 3:			nDuration = (UINT16)(3.5*q*250);		
			break;
		case 4:			nDuration = (UINT16)(q*250);		
			break;
		case 5:			nDuration = (UINT16)(1.5*q*250);		
			break;
		case 6:			nDuration = (UINT16)(1.75*q*250);		
			break;
		case 7:			nDuration = (UINT16)(0.5*q*250);		
			break;
		case 8:			nDuration = (UINT16)(0.75*q*250);		
			break;
		case 9:			nDuration = (UINT16)(0.875*q*250);		
			break;
		case 10:		nDuration = (UINT16)(0.25*q*250);		
			break;
		case 11:		nDuration = (UINT16)(0.325*250);		
			break;
		case 12:		nDuration = (UINT16)(0.4375*q*250);		
			break;
		case 13:		nDuration = (UINT16)(0.125*q*250);		
			break;
		case 14:		nDuration = (UINT16)(0.1625*q*250);		
			break;
		case 15:		nDuration = (UINT16)(0.21875*q*250);		
			break;
	}	// Length
	
	//note message summation
	OctaveTemp <<= 4;
	if(Height == 0){
		NoteTemp = 0x0d;
	}else{
		NoteTemp = NoteTemp | OctaveTemp;
	}

	if(nDuration <= 0x7f){	 
		//note message
		PutCharData(NoteTemp);
		MelodySize++;

		//gate time
		PutCharData(nDuration);
		MelodySize++;
		
		//duration
		PutCharData(nDuration);
		MelodySize++;
	}
	else{
		nDuration = nDuration - 128;
		nTime=(UINT8) ((nDuration>>7)&(0x7f));
		nGateTime=(UINT8)(nDuration & 0x007f);

		//note message
		PutCharData(NoteTemp);
		MelodySize++;

		//gate time 
		PutCharData(nTime | 0x80);
		MelodySize++;
		PutCharData(nGateTime);
		MelodySize++;

		//duration 
		PutCharData(nTime | 0x80);
		MelodySize++;
		PutCharData(nGateTime);
		MelodySize++;		
	}
}


void ConvMybellData2SMAF(UINT8* pMelodyTextTemp,BYTE nTextSize )
{
	UINT8 MainID,Option,Temp,i;
	UINT8 Height,Length,Octave=4,Half=2;
	UINT8 Finish=0;
	UINT8 Repeat, RepeatNum;
	UINT8 SubRoutine=0;
	UINT8 Label[10];									    // 서브루틴 레이블
    UINT8 LabelSize = 0;								    // 서브루틴 갯수
    UINT8* LabelAddr[10];								    // 서브루틴 주소
    UINT8 StackSize = 0;	
    UINT8* Stack[10];									    // 반복스택
	UINT8 ChangeHalf=0,HalfNote=100;
	UINT8 pMelodyText[MYBELL_DATA_MAX+1];

    memset(pMelodyText, NULL, MYBELL_DATA_MAX+1);
	memcpy(pMelodyText, pMelodyTextTemp, nTextSize);

	// buffer loading
	InitBitData(pMelodyText);
	MelodySize=0;									// 멜로디 사이즈 초기화
	DataPoint=0;										// 데이터 포인트 초기화
	IsFirstNote=0;
	CHNN=0x00;//,NoteTemp1=0x40,NoteTemp2=0x80,NoteTemp3=0xc0 ; // Channel 0,1,2,3
	to=0x0c;
	re=0x02;
	mi=0x04;
	pa=0x05;
	sol=0x07;
	ra=0x09;
	si=0x0b;
	ShiftToHalf=0;

	// Header reading
	MainID = GetBitData(4);						        // 주명령ID
	Option = GetBitData(4);						        // 부명령ID
	Temp   = GetBitData(4);						        // 전체 메시지수
	Temp   = GetBitData(4);						        // 현 메시지 번호
	Temp   = GetBitData(4);						        // 전체 채널수
	Temp   = GetBitData(4);						        // 현 채널 번호
	// Tempo data reading
    MelodyTempo = (float)(GetBitData(8)*2);   // 곡 템포

    if (Option == 0x01){                                 // 곡의 제목
        GetBitData(64);							        // 포인터 Skip
    }
    
	MakeSMAFHeader();									// 헤더 생성
	// Set melody notes
    while ( MainID == 1 && Finish != 3 && \
		    *pBitData != 0 && MelodySize < MYMELODY_DATA_MAX ) 
    {
        if (0x01 == (byte)GetBitData(1))                   // 음 형식
        {	
            Height = (byte)GetBitData(3);			    // 음높이
            Length = (byte)GetBitData(4);		    // 음길이
			if(ChangeHalf){
				HalfNote = Height;
				ChangeHalf =0;
			}
            MyBell2SMAFNote(Octave, Half, Height, Length,HalfNote);
			HalfNote = 100;
        }
        else 
        {
            switch ((byte)GetBitData(2)) 
            {
            case 0:									    // 종료 형식, 볼륨처리형식
                GetBitData(5);					        // Ignore
                Finish = 3;
                break;
            case 1:									    // 옥타브 형식
                Temp = (byte)GetBitData(3);		        // 옥타브
                GetBitData(2);						    // 예비
                Octave = Temp;
                break;
            case 2:									    // 반음처리 형식
                Temp = (byte)GetBitData(2);			    // 반음
                GetBitData(3);						    // 예비
                Half = Temp;
				ChangeHalf=1;
                break;
            case 3:									    // 반복처리 형식
                Repeat = (byte)GetBitData(2);		    // 반복부 처리
                RepeatNum = (byte)GetBitData(3);	    // 반복부 번호
                switch (Repeat) 
                {
                case 0:									    // 시작
                    if (LabelSize == 10)
                        break;
                    Label[LabelSize] = RepeatNum;		    // Set Label
                    LabelAddr[LabelSize] = pBitData;
					++LabelSize;
                    SubRoutine = 1;
                    break;
                case 1:									    // 끝
                    if (StackSize  == 0) 
                    {
                        SubRoutine = 0;
                        break;
                    }
                    InitBitData(Stack[--StackSize]);    // Return
                    break;

                case 2:									    // 호출
                    if (StackSize == 10)
                        break;
                    for (i = 0; i < LabelSize; i++)
                        if (Label[i] == RepeatNum) 
                        {
                            Stack[StackSize] = pBitData;	// Save Return Point
							++StackSize;
                            InitBitData(LabelAddr[i]);	// Call
                        }
                    break;
                }
                break;
            }
        }
    }
	MakeSMAFEnd();
}
static UINT8 MULTIPLETEMP=3;
static void GVMBell2SMAFNote(UINT8 Height, UINT8 Length)
{
	UINT8 NoteTemp=0x00;
	UINT8 OctaveTemp=0x00,Octave=100;
	UINT8  nTime=0,nGateTime=0;
	UINT16 nDuration=0;

	switch(Height)		// Octave    
	{
		case SWAP_NOTE_G3:	
			NoteTemp = CHNN | 0x07;			Octave = 3;
			break;
		case SWAP_NOTE_G3P:	
			NoteTemp = CHNN | 0x08;			Octave = 3;
			break;
		case SWAP_NOTE_A3:	
			NoteTemp = CHNN | 0x09;			Octave = 3;
			break;
		case SWAP_NOTE_A3P:	
			NoteTemp = CHNN | 0x0a;			Octave = 3;
			break;
		case SWAP_NOTE_B3:	
			NoteTemp = CHNN | 0x0b;			Octave = 3;
			break;
		case SWAP_NOTE_C4:	
			NoteTemp = CHNN | 0x0c;			Octave = 3;
			break;


		case SWAP_NOTE_C4P:	
			NoteTemp = CHNN | 0x01;			Octave = 4;
			break;
		case SWAP_NOTE_D4:	
			NoteTemp = CHNN | 0x02;			Octave = 4;
			break;
		case SWAP_NOTE_D4P:	
			NoteTemp = CHNN | 0x03;			Octave = 4;
			break;
		case SWAP_NOTE_E4:	
			NoteTemp = CHNN | 0x04;			Octave = 4;
			break;
		case SWAP_NOTE_F4:	
			NoteTemp = CHNN | 0x05;			Octave = 4;
			break;
		case SWAP_NOTE_F4P:	
			NoteTemp = CHNN | 0x06;			Octave = 4;
			break;
		case SWAP_NOTE_G4:	
			NoteTemp = CHNN | 0x07;			Octave = 4;
			break;
		case SWAP_NOTE_G4P:	
			NoteTemp = CHNN | 0x08;			Octave = 4;
			break;
		case SWAP_NOTE_A4:	
			NoteTemp = CHNN | 0x09;			Octave = 4;
			break;
		case SWAP_NOTE_A4P:	
			NoteTemp = CHNN | 0x0a;			Octave = 4;
			break;
		case SWAP_NOTE_B4:	
			NoteTemp = CHNN | 0x0b;			Octave = 4;
			break;
		case SWAP_NOTE_C5:	
			NoteTemp = CHNN | 0x0c;			Octave = 4;
			break;    
 
		case SWAP_NOTE_C5P:	
			NoteTemp = CHNN | 0x01;			Octave = 5;
			break;
		case SWAP_NOTE_D5:	
			NoteTemp = CHNN | 0x02;			Octave = 5;
			break;
		case SWAP_NOTE_D5P:	
			NoteTemp = CHNN | 0x03;			Octave = 5;
			break;
		case SWAP_NOTE_E5:	
			NoteTemp = CHNN | 0x04;			Octave = 5;
			break;
		case SWAP_NOTE_F5:	
			NoteTemp = CHNN | 0x05;			Octave = 5;
			break;
		case SWAP_NOTE_F5P:	
			NoteTemp = CHNN | 0x06;			Octave = 5;
			break;
		case SWAP_NOTE_G5:	
			NoteTemp = CHNN | 0x07;			Octave = 5;
			break;
		case SWAP_NOTE_G5P:	
			NoteTemp = CHNN | 0x08;			Octave = 5;
			break;
		case SWAP_NOTE_A5:	
			NoteTemp = CHNN | 0x09;			Octave = 5;
			break;
		case SWAP_NOTE_A5P:	
			NoteTemp = CHNN | 0x0a;			Octave = 5;
			break;
		case SWAP_NOTE_B5:	
			NoteTemp = CHNN | 0x0b;			Octave = 5;
			break;
		case SWAP_NOTE_C6:	
			NoteTemp = CHNN | 0x0c;			Octave = 5;
			break;    

		case SWAP_NOTE_C6P:	
			NoteTemp = CHNN | 0x01;			Octave = 6;
			break;
		case SWAP_NOTE_D6:	
			NoteTemp = CHNN | 0x02;			Octave = 6;
			break;
		case SWAP_NOTE_D6P:	
			NoteTemp = CHNN | 0x03;			Octave = 6;
			break;
		case SWAP_NOTE_E6:	
			NoteTemp = CHNN | 0x04;			Octave = 6;
			break;
		case SWAP_NOTE_F6:	
			NoteTemp = CHNN | 0x05;			Octave = 6;
			break;
		case SWAP_NOTE_F6P:	
			NoteTemp = CHNN | 0x06;			Octave = 6;
			break;
		case SWAP_NOTE_G6:	
			NoteTemp = CHNN | 0x07;			Octave = 6;
			break;
		case SWAP_NOTE_R:	
			NoteTemp = CHNN | 0x0d;			Octave = 4;
			break;
		case SWAP_NOTE_END:
		default:		return;
     }  
	if(IsFirstNote == 0){
		PutCharData(0x09);
		MelodySize++;
		IsFirstNote = 1;
	}

	switch(Octave)		// Octave    
	{
		case 0:	
			OctaveTemp = 0x00;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x82);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		case 1:
			OctaveTemp = 0x00;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x81);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		case 2:			OctaveTemp = 0x00;
			break;
		case 3:			OctaveTemp = 0x00;
			break;
		case 4:			OctaveTemp = 0x01;
			break;
		case 5:			OctaveTemp = 0x02;
			break;
		case 6:			
			OctaveTemp = 0x03;
/*			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x01);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;*/
			break;
		case 7:			
			OctaveTemp = 0x03;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x02);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
	}					// Octave  
	
	//note message summation
	OctaveTemp <<= 4;
	NoteTemp = NoteTemp | OctaveTemp;

	// Legnth    
	nDuration=(UINT16)(Length*MULTIPLETEMP); 

	if(nDuration <= 0x7f){	 
		//note message
		PutCharData(NoteTemp);
		MelodySize++;

		//gate time
		PutCharData(nDuration);
		MelodySize++;
		
		//duration
		PutCharData(nDuration);
		MelodySize++;
	}
	else{
		nDuration = nDuration - 128;
		nTime=(UINT8) ((nDuration>>7)&(0x7f));
		nGateTime=(UINT8)(nDuration & 0x007f);

		//note message
		PutCharData(NoteTemp);
		MelodySize++;

		//gate time 
		PutCharData(nTime | 0x80);
		MelodySize++;
		PutCharData(nGateTime);
		MelodySize++;

		//duration 
		PutCharData(nTime | 0x80);
		MelodySize++;
		PutCharData(nGateTime);
		MelodySize++;		
	}
}
void ConvGVMBellData2SMAF(UINT8* pMelodyTextTemp, UINT16 nTextSize )
{
	UINT8 Height,Length,i;
	UINT8 pMelodyText[GVMBELL_DATA_MAX+1];
	
	//local variable reset
    memset(pMelodyText, NULL, GVMBELL_DATA_MAX+1);
	if(nTextSize < GVMBELL_DATA_MAX){
		memcpy(pMelodyText, pMelodyTextTemp, nTextSize);
	}else{
		nTextSize=GVMBELL_DATA_MAX;
		memcpy(pMelodyText, pMelodyTextTemp, nTextSize);
	}
	i=0;
	Height=0;
	Length=0;
    
	//global variable reset
	InitBitData(pMelodyText);
	CHNN=0x00;//,NoteTemp1=0x40,NoteTemp2=0x80,NoteTemp3=0xc0 ; // Channel 0,1,2,3
	MelodySize=0;								
	DataPoint=0;								
	MelodyTempo=0;
	IsFirstNote=0;
	
    MakeSMAFHeader_1msec();	
    while ( i < nTextSize && MelodySize < MYMELODY_DATA_MAX ) 
    {
		Height = GetBitData(8);
		if(Height == SWAP_NOTE_END)
			break;
		Length = GetBitData(8);
		GVMBell2SMAFNote(Height, Length);
		i+=2;
    }
	MakeSMAFEnd();
}
static void STIBell2SMAFNote(UINT8 Height, UINT8 Length)
{
	UINT8 NoteTemp=0x00;
	UINT8 OctaveTemp=0x00,Octave=100;
	float q=0;
	UINT8  nTime=0,nGateTime=0;
	UINT16 nDuration=0;

	if(Height == 0){
		NoteTemp=0x0C;
		Octave = 3;
	}else if(Height > 0 && Height < 0x0d){
		NoteTemp = CHNN | Height;
		Octave = 4;
	}else if(Height > 0x0c && Height < 0x19){
		NoteTemp = Height - 0x0c;
		NoteTemp = CHNN | NoteTemp;
		Octave = 5;
	}else if(Height > 0x18 && Height < 0x24){
		NoteTemp = Height - 0x18;
		NoteTemp = CHNN | NoteTemp;
		Octave = 6;
	}else if(Height == 0x28){
		Octave = 2;
	}else{
		return;
	}
	if(IsFirstNote == 0){
		PutCharData(0x09);
		MelodySize++;
		IsFirstNote = 1;
	}
	switch(Octave)		// Octave    
	{
		case 0:	
			OctaveTemp = 0x00;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x82);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		case 1:
			OctaveTemp = 0x00;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x81);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		case 2:			OctaveTemp = 0x00;
			break;
		case 3:			OctaveTemp = 0x01;
			break;
		case 4:			OctaveTemp = 0x02;
			break;
		case 5:			OctaveTemp = 0x03;
			break;
		case 6:			
			OctaveTemp = 0x03;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x01);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
		case 7:			
			OctaveTemp = 0x03;
			PutCharData(0x00);
			MelodySize++;
			PutCharData(0x32);
			MelodySize++;
			PutCharData(0x02);
			MelodySize++;
			PutCharData(0x00);
			MelodySize++;
			break;
	}					// Octave  
  
	// Legnth    
	q=(float) (MelodyTempo);
	switch(Length)
	{
		case 0x00:			nDuration = (UINT16)(q*16);
			break;
		case 0x01:			nDuration = (UINT16)(q*8);
			break;
		case 0x02:			nDuration = (UINT16)(q*4);
			break;
		case 0x03:			nDuration = (UINT16)(q*2);
			break;
		case 0x04:			nDuration = (UINT16)(q);
			break;
		case 0x05:			nDuration = (UINT16)(q*0.5);
			break;
		case 0x0c:			nDuration = (UINT16)(q*1.5);
			break;
		case 0x0d:			nDuration = (UINT16)(q*3);
			break;
		case 0x0e:			nDuration = (UINT16)(q*6);
			break;
		case 0x0f:			nDuration = (UINT16)(q*12);
			break;
		default:		return;
	}	// Length
	
	//note message summation
	OctaveTemp <<= 4;
	if(Height == 0x28){
		NoteTemp = NoteTemp | 0x0d;  // 쉼표처리 
	}else{
		NoteTemp = NoteTemp | OctaveTemp;
	}


	if(nDuration <= 0x7f){	 
		//note message
		PutCharData(NoteTemp);
		MelodySize++;

		//gate time
		PutCharData(nDuration);
		MelodySize++;
		
		//duration
		PutCharData(nDuration);
		MelodySize++;
	}
	else{
		nDuration = nDuration - 128;
		nTime=(UINT8) ((nDuration>>7)&(0x7f));
		nGateTime=(UINT8)(nDuration & 0x007f);

		//note message
		PutCharData(NoteTemp);
		MelodySize++;

		//gate time 
		PutCharData(nTime | 0x80);
		MelodySize++;
		PutCharData(nGateTime);
		MelodySize++;

		//duration 
		PutCharData(nTime | 0x80);
		MelodySize++;
		PutCharData(nGateTime);
		MelodySize++;		
	}
}
void ConvSTIbellData2SMAF(UINT8* pMelodyTextTemp,BYTE nTextSize )
{
	UINT8 i;
	UINT8 Height,Length,nNoteNUM;
	UINT8 pMelodyText[MYBELL_DATA_MAX+1];
	
	//local variable reset
	memset(pMelodyText, NULL, MYBELL_DATA_MAX+1);
	if(nTextSize < MYBELL_DATA_MAX){
		memcpy(pMelodyText, pMelodyTextTemp, nTextSize);
	}else{
		nTextSize=MYBELL_DATA_MAX;
		memcpy(pMelodyText, pMelodyTextTemp, nTextSize);
	}
	Height=0;
	Length=0;

	//global variable reset
	InitBitData(pMelodyText);
	CHNN=0x00;//,NoteTemp1=0x40,NoteTemp2=0x80,NoteTemp3=0xc0 ; // Channel 0,1,2,3
	MelodySize=0;									// 멜로디 사이즈 초기화
	DataPoint=0;										// 데이터 포인트 초기화
	MelodyTempo=0;
	IsFirstNote=0;

    for(i =0; i < 16; i++){							
        GetBitData(8);                   // 곡의 제목
		--nTextSize;
    }
	i=0;
    MelodyTempo = (float)(GetBitData(8));     // 빠르기
	nNoteNUM   = GetBitData(8);	     		  // 음갯수
	nTextSize-=2;
	MakeSMAFHeader_1msec();
    while ( i < nTextSize && MelodySize < MYMELODY_DATA_MAX ) 
    {
		Height   = GetBitData(8);
		Length   = GetBitData(8);
		STIBell2SMAFNote(Height, Length);
		i+=2;
    }
	MakeSMAFEnd();
}


#endif
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

///////////////////////////////////////////////////////////////////////////////
//	Sort MyBell List
//
void	sInitMyBellList()
{
    mybell_buf_type	*pMsg1, *pMsg2;
    DWORD			nDate1, nDate2;
    UINT8			i, j;
    BYTE			temp;

    // Retrive used buffer
    for (i = 0, j = 0; i < MAX_MYBELL_SIZE; i++)
    {
	    if (a_MybellBuffer[i].nUsed == 1) 
        {
		    sMBList[j++] = i;
	    }
    }

    g_MyBellSize = j;

    // Sort the list
    for (i = 0; i < g_MyBellSize - 1; i++) 
    {
	    for (j = i + 1; j < g_MyBellSize; j++) 
        {
		    pMsg1  = &a_MybellBuffer[sMBList[i]];
		    nDate1 = (pMsg1->nDate[0] << 24) | (pMsg1->nDate[1] << 16) | (pMsg1->nDate[2] << 8) | pMsg1->nDate[3];
		    pMsg2  = &a_MybellBuffer[sMBList[j]];
		    nDate2 = (pMsg2->nDate[0] << 24) | (pMsg2->nDate[1] << 16) | (pMsg2->nDate[2] << 8) | pMsg2->nDate[3];

		    if (nDate1 < nDate2) 
            {
			    temp = sMBList[i];
			    sMBList[i] = sMBList[j];
			    sMBList[j] = temp;
		    }
	    }
    }
}

///////////////////////////////////////////////////////////////////////////////
//	Get free buffer
//
//	return	 0~MAX_RCVSMS_MSG-1         : Free buffer
//
UINT8	sFindFreeMyBellBuffer()
{
    UINT8       i;

    sInitMyBellList();

    if ( g_MyBellSize < MAX_MYBELL_SIZE)              // Empty buffer exist
    {	
	    for (i = 0; i < MAX_MYBELL_SIZE; i++)         // Find empty buffer
        {			
		    if (a_MybellBuffer[i].nUsed == 0)
			    return i;
	    }
    }

    // Buffer full, overwrite oldest
    return sMBList[MAX_MYBELL_SIZE - 1];
}

///////////////////////////////////////////////////////////////////////////////
//      Mybell Count
//
INT8 g_MybellCount()
{
    INT8    bCount, i;

    bCount = 0;

	sInitMyBellList();

    for (i = 0; i < MAX_MYBELL_SIZE; i++) 
    {           
        if ((a_MybellBuffer[i].nUsed == 1) &&
	          (a_MybellBuffer[i].nAttr & 0x01)) 
        {       
            // New Mybell Count
            bCount++;
        }
    }

    return bCount;
}
///////////////////////////////////////////////////////////////////////////////
//	Add new mybell
//
void g_NewMybellSave(UI_COMMAND_U *pCommand)
{
    UINT8               nIndex = sFindFreeMyBellBuffer();
    static              mybell_nv_type	    *pWriteMybell;
    DWORD               nDate;
    CHAR                txt[3];
    mybell_buf_type    *pMsg = &a_MybellBuffer[nIndex];

    pWriteMybell = (mybell_nv_type*)SKY_Malloc( (SIZE_T)(sizeof(mybell_nv_type)) );

	// Set used flag
	pMsg->nUsed = 1;
    pWriteMybell->nUsed = 1;

	// Copy date
	ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.yr, txt);
	nDate =            atoi(txt);
	ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.mon, txt);
	nDate = nDate*13 + atoi(txt);
	ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.day, txt);
	nDate = nDate*32 + atoi(txt);
	ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.hrs, txt);
	nDate = nDate*24 + atoi(txt);
	ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.min, txt);
	nDate = nDate*60 + atoi(txt);
	ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.sec, txt);
	nDate = nDate*60 + atoi(txt);

	pMsg->nDate[0] = pWriteMybell->nDate[0] = (nDate >> 24) & 0xFF;
	pMsg->nDate[1] = pWriteMybell->nDate[1] = (nDate >> 16) & 0xFF;
	pMsg->nDate[2] = pWriteMybell->nDate[2] = (nDate >>  8) & 0xFF;
	pMsg->nDate[3] = pWriteMybell->nDate[3] =  nDate        & 0xFF;

	// Copy MyBell data
    pWriteMybell->nAttr |= 0x01;
    pMsg->nAttr |= 0x01;

    if((pCommand->sms.buf_ptr->data.tele_srv == SMSI_SHINSEGI_MYBELL)
        ||(pCommand->sms.buf_ptr->data.tele_srv == SMSI_SHINSEGI_MYBELL2))
    {
        pWriteMybell->nAttr |= 0x04;
        pMsg->nAttr |= 0x04;
    }

    pWriteMybell->nTextSize = pCommand->sms.buf_ptr->data.msg_data.user_data.num_fields;
    if(pWriteMybell->nTextSize > MYBELL_DATA_MAX )
    {
       pWriteMybell->nTextSize = MYBELL_DATA_MAX;       // safty code      
    } 
    memcpy((void *)pWriteMybell->szText, (const void *)pCommand->sms.buf_ptr->data.msg_data.user_data.chari, pWriteMybell->nTextSize);

	// Update NV
	sInitMyBellList();
    SKY_SeekWriteMybellT( nIndex, pWriteMybell );
    SKY_Free( pWriteMybell );
	ui_sms_update_icons();

   // release SMS buffer
    if (pCommand->sms.buf_ptr != NULL)
    {
        q_put(&ui_sms_free_q, &pCommand->sms.buf_ptr->link);
        pCommand->sms.buf_ptr = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
//	Mybell Menu List
//

LOCAL void sInitMakeMBList ( void )
{
    INT8      i,      len,      sLineNumber;
    static    BYTE    pViewBuf[MAX_MYBELL_SIZE][LCD_COLUME_SIZE - 1];
    mybell_nv_type    *pReadMybell;
    BYTE              szBuf[LCD_COLUME_SIZE+1];

    sLineNumber = 0;
    for(i = 0; i < MAX_MYBELL_SIZE; i++)
    {
        memset( (void *)&pViewBuf[i][0], NULL, LCD_COLUME_SIZE - 1 );    
    }

    // Make menu text
    for (i = 0; i < g_MyBellSize; i++) {
        pReadMybell = (mybell_nv_type*)SKY_Malloc( (SIZE_T)(sizeof(mybell_nv_type)) );
        SKY_SeekReadMybellT( sMBList[i], pReadMybell );
        if(pReadMybell->nUsed)
        {
            if(pReadMybell->nAttr & 0x04)
            {
                len = 0;
                memset(szBuf, NULL, LCD_COLUME_SIZE+1);
                while((len < MYBELL_SHINSEGI_TITLE) 
                        &&(pReadMybell->szText[len] != ' ')
                        &&(pReadMybell->szText[len] != NULL))
                {   
                    szBuf[len] = pReadMybell->szText[len];
                    len++;
                }

                if(len == 0)
                {
                    (void) memcpy( (void *)&pViewBuf[sLineNumber][0],
                            SKY_GET_TOKEN(TKN_MYBELL_UNTITLE_NAME), 6);                                  
                }
                else
                {
                    if(IsMiddleOfMultiByte(&szBuf[0],MYBELL_SHINSEGI_TITLE))
                        szBuf[MYBELL_SHINSEGI_TITLE-1] = '\0';//' ';
                    STRCPY((BYTE *)&pViewBuf[sLineNumber][0], (BYTE *)szBuf);
                }

                if(IsMiddleOfMultiByte(&pViewBuf[sLineNumber][0],MYBELL_SHINSEGI_TITLE))
                    pViewBuf[sLineNumber][MYBELL_SHINSEGI_TITLE-1] = '\0';//' ';               

            }
            else    //SKT
            {
                if ((pReadMybell->szText[0] & 0x01) == 0x01) {				// Name field exist
                    len = 0;
                    while (len < MYBELL_TITLE && pReadMybell->szText[4 + len] != ' ') 
                    {
                        pViewBuf[sLineNumber][len] = pReadMybell->szText[4 + len];
                        len++;
                    }
                }
                else
                {                
                    (void) memcpy( (void *)&pViewBuf[sLineNumber][0],
                                    SKY_GET_TOKEN(TKN_MYBELL_UNTITLE_NAME), 7); 
                } 
                if(IsMiddleOfMultiByte(&pViewBuf[sLineNumber][0],MYBELL_TITLE))
                    pViewBuf[sLineNumber][MYBELL_TITLE-1] = '\0';//' ';                
            }
        }

        paMBListBuf[sLineNumber] = pViewBuf[sLineNumber];
        sLineNumber++;
        SKY_Free( pReadMybell );
    }
}

///////////////////////////////////////////////////////////////////////////////
//	Mybell INfo Display & Sound
//
LOCAL BYTE* sDisplayMBInfo( INT8  nIndex)
{
//	mybell_nv_type*	pMsg = &a_MybellBuffer[sMBList[nIndex]];

    STATIC BYTE         szBuf1[LCD_COLUME_SIZE+1];

	mybell_nv_type	    *pReadMybell = &SoundPlayBuf;
    BYTE            szBuf2[LCD_COLUME_SIZE-1];
    DWORD           nDate;
    BYTE            nYear, nMonth, nDay, nHour, nMin, nSec;
    mybell_buf_type *pMsg = &a_MybellBuffer[sMBList[nIndex]];
    INT8            len = 0;

    g_PlayMelSize = 0;      //play melody buffer size init
    memset(pReadMybell, NULL, sizeof(mybell_nv_type));
    SKY_SeekReadMybellT( sMBList[nIndex], pReadMybell );

    if (g_MyBellSize > 0) 
    {
        memset(szBuf1, NULL, LCD_COLUME_SIZE+1);
        memset(szBuf2, NULL, LCD_COLUME_SIZE - 1);
        memset(pNameBuf, NULL, SKY_FILE_NAME_MAX_LENGTH);

        if (!(pReadMybell->nAttr & 0x02)) {
            pReadMybell->nAttr &=~0x01;     //NEW 속성을 없앤다
            pMsg->nAttr &=~0x01;
            pReadMybell->nAttr |= 0x02;
            pMsg->nAttr |= 0x02;
            SKY_SeekWriteMybellT( sMBList[nIndex], pReadMybell );
            ui_sms_update_icons();
        }

	    nDate  = (pReadMybell->nDate[0] << 24) | (pReadMybell->nDate[1] << 16) | (pReadMybell->nDate[2] << 8) | pReadMybell->nDate[3];
	    nSec   = nDate%60;	nDate = nDate/60;
	    nMin   = nDate%60;	nDate = nDate/60;
	    nHour  = nDate%24;	nDate = nDate/24;
	    nDay   = nDate%32;	nDate = nDate/32;
	    nMonth = nDate%13;	nDate = nDate/13;
	    nYear  = nDate;

        if(nHour == 0)
        {
	        sprintf( (char*)szBuf1, "%2d%s %2d%s %2d:%02d%cM",
		         nMonth, SKY_GET_TOKEN(TKN_DAT_MONTH), nDay, SKY_GET_TOKEN(TKN_DAT_DAY), nHour , nMin, (nHour < 12) ? 'A' : 'P');
        }
        else
        {
	        sprintf( (char*)szBuf1, "%2d%s %2d%s %2d:%02d%cM",
		         nMonth, SKY_GET_TOKEN(TKN_DAT_MONTH), nDay, SKY_GET_TOKEN(TKN_DAT_DAY), ((nHour + 23) % 12) + 1, nMin, (nHour < 12) ? 'A' : 'P');
        }
        if(pReadMybell->nAttr & 0x04)
        {
            len = 0;
            while((len < MYBELL_SHINSEGI_TITLE) 
                    &&(pReadMybell->szText[len] != ' ')
                    &&(pReadMybell->szText[len] != NULL))
            {   
                szBuf2[len] = pReadMybell->szText[len];
                len++;
            }

            if(len == 0)
            {
                (void) memcpy( (void *)pNameBuf, SKY_GET_TOKEN(TKN_MYBELL_UNTITLE_NAME), 6);                                  
            }
            else
            {
                if(IsMiddleOfMultiByte(&szBuf2[0],MYBELL_SHINSEGI_TITLE))
                    szBuf2[MYBELL_SHINSEGI_TITLE-1] = '\0';//' ';
                STRCPY((BYTE *)pNameBuf, (BYTE *)szBuf2);
            }
        }
        else
        {
            if ((pReadMybell->szText[0] & 0x01) == 0x01) {				// Name field exist
                len = 0;
                while (len < MYBELL_TITLE && pReadMybell->szText[4 + len] != ' ') 
                {
                    szBuf2[len] = pReadMybell->szText[4 + len];
                    len++;
                }
            }
            else												
                (void) memcpy( (void *)&szBuf2[0], SKY_GET_TOKEN(TKN_MYBELL_UNTITLE_NAME), 6);

            if(IsMiddleOfMultiByte(&szBuf2[0],MYBELL_TITLE))
                szBuf2[MYBELL_TITLE-1] = '\0';//' ';
            (void) memcpy( (void *)pNameBuf, (const void*)szBuf2, MYBELL_TITLE);
        }

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
        // Melody parsing
	    skm_MelodyDownload((BYTE *)pReadMybell->szText);	  // For SKT Format	
	    // Pre-play
        SKY_PlaySound( UI_MUSIC_SND, MUS_MYBELL_SOUND);

#elif SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE

        // Melody parsing
        if(pReadMybell->nAttr & 0x04)
        {
            ConvSTIbellData2SMAF((BYTE *)pReadMybell->szText, pReadMybell->nTextSize);
        }
        else
        {
            ConvMybellData2SMAF((BYTE *)pReadMybell->szText, pReadMybell->nTextSize); // TO U&I :pReadMybell->nTextSize 를 넣어 주세요 from Reo        
        }
        // Pre-play
        SKY_PlayMusicBufferEx(mb_MyBellPreBuffer, SKY_GET_SMS_BELL_VOLUME(), 0);
#endif
        return  szBuf1;

    }
    else
    {
        return NULL;
    }        
}

BOOL    gfIsThereMB()
{

    sInitMyBellList();

    if (g_MyBellSize > 0)
        return TRUE;
    else
        return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
//	Mybell Main Menu
//
VOID SKA_MybellMessage(SKY_EVENT_T* pEvent)
{
    STATIC  INT8            sWindowSize;
    STATIC  HCONTROL        s_hHnd;

    HCONTROL                hMsg;
 

    SKY_TOKEN paMybellPopMenu[2]=
    {
        TKN_SMS_TEXT_DELETE,
        TKN_SMS_DELETE_ALL    //  전부삭제
    };

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        sInitMyBellList();

        if (g_MyBellSize == 0) 
        {
            EQS_END();
            break;
        }
        else
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                200, // SKY_DEF_WIN_DY, 
                CL_MENU_SAVE_NONE,
                SKY_GET_TOKEN(TKN_SMS_SUB_MENU3), 
                WA_SHOW_CML | WA_SHOW_HEADER);

            sInitMakeMBList( );
            nMBIndex = 0;
            sWindowSize = 5;
            s_hHnd = EQC_ScrollList(EQS_NA, EQS_NA, 18, sWindowSize, paMBListBuf, g_MyBellSize, g_MyBellSize, 0); 

            EQC_SET_STYLE( s_hHnd, EQC_GET_STYLE(s_hHnd) |
                                 CS_SCROLL_LIST_DEFAULT |
                                 CS_SCROLL_LIST_ENUMBERATED |
                                 CS_SCROLL_LIST_NO_CML  |
                                 CS_SCROLL_LIST_MAXIMIZE |
                                 CS_SCROLL_LIST_STATUS_BAR );

            EQC_SetScrollStatusBarText(s_hHnd, sDisplayMBInfo( nMBIndex ));
            EQS_SetIOCActive( s_hHnd );

            EQS_FitCanvasSize();    
        }
        break;

    case SE_APP_END :     /* The very last event */
        ui_sms_update_icons();

        SKY_StopMusic();
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_DRAW :    /* draw */
        break;

    case SE_APP_RESUME :  /* end child */
        nMBIndex = EQC_GetScrollPhysicalIndex(s_hHnd);
        EQC_SetScrollStatusBarText(s_hHnd, sDisplayMBInfo( nMBIndex ));
        EQS_SetIOCActive( s_hHnd );
       break;

    case SE_IO_SCROLL_LIST_SELECT:
        // 나만의 벨에 저장
        nMBIndex = EQC_GetScrollPhysicalIndex(s_hHnd);
        sDisplayMBInfo( nMBIndex ); //NameBuf fill
        
        SKY_StopMusic();

        EQS_START_CHILD( SKA_SaveMybell );
        break;

    case SE_IO_SCROLL_LIST_CHG:
        SKY_StopMusic();
        nMBIndex = EQC_GetScrollPhysicalIndex(s_hHnd);
        EQC_SetScrollStatusBarText(s_hHnd, sDisplayMBInfo( nMBIndex ));
        break;
    
    case SE_IO_POPUP_CANCEL:
        sDisplayMBInfo( nMBIndex );
        break;

    case SE_IO_POPUP_SELECT:
        switch(pEvent->dwParam)
        {
        case 0:
            SKY_StopMusic();
            nMBIndex = EQC_GetScrollPhysicalIndex(s_hHnd);
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MYB_MESSAGE_02), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);
            break;

        case 1:
            SKY_StopMusic();
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MYB_MESSAGE_04), 
                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);
            break;
        }
        break;

    case SE_SOFTKEY_MENU:
        SKY_StopMusic();
        EQC_PopUpMenu((EQS_TOKEN_EX*)paMybellPopMenu, 2, 0);
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            sDeleteMBMessage();
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MYB_MESSAGE_03), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);            
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            gDeleteAllMybell();
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MYB_MESSAGE_05), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);          
        }
        break;

    case SE_IO_MESGBOX_CLEAR:
    case SE_IO_MESGBOX_NO:  
        sDisplayMBInfo( nMBIndex );
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            sInitMyBellList();

            if (g_MyBellSize > 0) 
            {
                EQC_DeleteScrollData(s_hHnd, nMBIndex);
                EQS_SetIOCActive(s_hHnd);
                nMBIndex = EQC_GetScrollPhysicalIndex(s_hHnd); 
                EQC_SetScrollStatusBarText(s_hHnd, sDisplayMBInfo( nMBIndex ));
            }        
            else
            {
                EQS_END();
            }
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            EQS_END();
        }
        else
        {
            sDisplayMBInfo( nMBIndex );   
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam == EQC_DELETED)
            break;

    case SE_KEY_CLEAR:
    case SE_SOFTKEY_CANCEL:
        EQS_END();
        break;

    case SE_SMS_MYBELL:
        EQS_DeleteControl(s_hHnd);

        SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());  

        sInitMakeMBList( );
        nMBIndex = 0;
        s_hHnd = EQC_ScrollList(EQS_NA, EQS_NA, 18, EQS_NA, paMBListBuf, g_MyBellSize, g_MyBellSize, 0); 


        EQC_SET_STYLE( s_hHnd, EQC_GET_STYLE(s_hHnd) |
                             CS_SCROLL_LIST_DEFAULT |
                             CS_SCROLL_LIST_ENUMBERATED |
                             CS_SCROLL_LIST_NO_CML  |
                             CS_SCROLL_LIST_MAXIMIZE |
                             CS_SCROLL_LIST_STATUS_BAR );

        EQC_SetScrollStatusBarText(s_hHnd, sDisplayMBInfo( nMBIndex ));
        EQS_SetIOCActive( s_hHnd );
        break;

    default :
		SKY_DefEventProc(pEvent);
        break;
    }
}


void sMybellSetControls(void)
{

    UINT8                   nCtrl;
    BYTE*                   paOnOffMenu[2]; //ONOFF

    paOnOffMenu[0] = (BYTE *)SKY_GET_TOKEN(TKN_MYBELL_OFF);
    paOnOffMenu[1] = (BYTE *)SKY_GET_TOKEN(TKN_MYBELL_ON);

    /*
    ppSetBell[0] = (BYTE *)SKY_GET_TOKEN(TKN_MYBELL_SET_ALERTBELL);
*/
//    ppSetBell[1] = (BYTE *)SKY_GET_TOKEN(TKN_MYBELL_NO);


    // 벨이름
    EQC_StaticText( EQS_NA, EQS_NA, TKN_MYBELL_NAME );

    EQC_TextEdit( EQS_NA, EQS_NA, 
                SKY_FILE_NAME_MAX_LENGTH-1, 1, TEM_KOREAN, SKY_FILE_NAME_MAX_LENGTH-1, 
                (BYTE*)pNameBuf, 0 );

#if 0
    // 착신벨 설정
    EQC_StaticText( EQS_NA, EQS_NA, TKN_MYBELL_SET_ALERTBELL );

    EQC_SET_STYLE( EQC_CheckBox(EQS_NA, EQS_NA, ppSetBell, 2),
                   CS_CHECK_BOX_DEFAULT | CS_CHECK_BOX_BORDER );
#else // #if 0
/*
    EQC_SET_STYLE( EQC_CheckBox(EQS_NA, EQS_NA, ppSetBell, 1),
                   CS_CHECK_BOX_DEFAULT | CS_CHECK_BOX_BORDER );
*/
    EQC_StaticText(EQS_NA, EQS_NA, TKN_MYBELL_SET_ALERTBELL);
    EQC_ScrollCombo(EQS_NA, EQS_NA, SKY_FILE_NAME_MAX_LENGTH-1, 2, paOnOffMenu, 2, 2, 0);

#endif

    // Formatting
    EQS_StartFormatter();

    for(nCtrl = 0; nCtrl < SS_MB_MAX; nCtrl++)
    {
        if( (nCtrl == SS_MB_NAME)
            ||(nCtrl == SS_MB_SET) )

        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        }

        EQS_NewLine();
    }

//    EQS_GetControlMargin(&Margin);
//    EQS_GetControlSpace(&xSpace, &ySpace);
    
    EQS_EndFormatter();

//    unX = (UINT8)Margin.xLeft + EQC_GET_DX(EQS_GetNthIOC(SS_MB_SET)) + xSpace;
//    unY = EQC_GET_Y(EQS_GetNthIOC(SS_MB_SET));

}

///////////////////////////////////////////////////////////////////////////////
//	Mybell Save Menu
//
VOID SKA_SaveMybell(SKY_EVENT_T* pEvent)
{
    HCONTROL    hMsg;
    FILENAME_VALID_E sIsValidName;
    BYTE    szPathName[SKY_PATH_NAME_MAX_LENGTH];
//	  DWORD   AvailMem; 
//    WORD    nFiles;
    DWORD   mbSize;//, AvailMem;
    WORD    nFiles;
    BOOL    f_IsBell;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */ 
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_INPUT_SAVE,//CML_SAVE_NONE_CANCEL, 
                      SKY_GET_TOKEN(TKN_SMS_SUB_MENU3), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        sMybellSetControls(); // Add Controls
              
        EQS_FitCanvasSize();
        break;

    case SE_APP_END :     /* The very last event */
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_DRAW :    /* draw */
        break;

    case SE_APP_RESUME :  /* end child */
        break;

    case SE_IO_TEDIT_DONE:
        memset(pNameBuf, 0, SKY_FILE_NAME_MAX_LENGTH);
        EQC_GetTextEditData(EQS_GetNthIOC(SS_MB_NAME), pNameBuf);
        sIsValidName = (FILENAME_VALID_E)SKY_IsValidFileName( pNameBuf );
        if ( sIsValidName == FV_NULL )
        {
            hMsg = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_INPUT_BELLNAME), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, MYBELL_SAVE_INPUT_BELLNAME);
            break;
        }
        else if ( sIsValidName == FV_DIRCHAR )
        {
            hMsg = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_NOUSE_BELLNAME), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, MYBELL_SAVE_NOUSE_BELLNAME);
            break;
        }

        /* is there this file ? */
        /* make complete path */
        sprintf((char*)szPathName, "%s%s%s", SKY_MELODY_DIR, SKY_DIR_CHAR, pNameBuf);
        if(SKY_IsFileOrDirExist(szPathName) == TRUE)
        {            
            hMsg = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_EXIST_SAMENAME), 
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, MYBELL_SAVE_EXIST_SAMENAME);
        }
        break;

    case SE_IO_MESGBOX_END:
        if((pEvent->dwParam == MYBELL_SAVE_INPUT_BELLNAME)
            ||(pEvent->dwParam == MYBELL_SAVE_EXIST_SAMENAME)
            ||(pEvent->dwParam == MYBELL_SAVE_NOUSE_BELLNAME))
        {
            EQS_SetIOCActive( EQS_GetNthIOC(SS_MB_NAME) );
        }
        else if((pEvent->dwParam == MYBELL_SAVE_SUCCESS)
                    ||(pEvent->dwParam == MYBELL_SAVE_FAIL)
                    ||(pEvent->dwParam == MYBELL_SAVE_LACK_SPACE))
        {
            EQS_END();
        }
        else if(pEvent->dwParam == MYBELL_IN_SAVING)
        {
            f_IsBell = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SS_MB_SET));  //0:off 1:on

            mbSize = g_PlayMelSize;

            /*
            ** jrkwon : 2001.08.18 --> we don't need to check this in here, 
            ** becacue we are going to check this in SKY_WriteDataFile
            **
            */
            //AvailMem = SKY_GetFreeSpaceInDataFile();
            nFiles = SKY_GetFileNumberInDataFile(SDF_MELODY);

            if( /*( mbSize > AvailMem ) || */(nFiles == 0xFFFF) || (nFiles >= SKY_MAX_NUM_FILES))
//            if( ( mbSize > AvailMem ) || (nFiles == 0xFFFF) || (nFiles >= SKY_MAX_NUM_FILES))
            {
                hMsg = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_UNAVAILABLE_SPACE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hMsg, MYBELL_SAVE_LACK_SPACE);                                
            }
            else
            {
                if(SKY_WriteDataFile(SDF_MELODY, (BYTE*)pNameBuf, mbSize, mb_MyBellPreBuffer) == SDR_OK)
                { 
	                if(f_IsBell == TRUE)
	                {
		                //기본 착신벨로 저장
                        SKY_SET_BELL_FILE_NAME(pNameBuf);
                        SKY_SET_BELL_ID(SKY_MY_BELL_ID);
                    }
                    hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_10), 
                                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS); //저장 성공 
                    EQC_SET_USERDATA(hMsg, MYBELL_SAVE_SUCCESS);                                
                }
                else
                {
                    hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_25), //저장 실패  
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);   
                    EQC_SET_USERDATA(hMsg, MYBELL_SAVE_FAIL);                                
                }
            }
        
        }
        break;

    case SE_SOFTKEY_SAVE:
        memset(pNameBuf, 0, SKY_FILE_NAME_MAX_LENGTH);
        EQC_GetTextEditData(EQS_GetNthIOC(SS_MB_NAME), pNameBuf);
        sIsValidName = (FILENAME_VALID_E)SKY_IsValidFileName( pNameBuf );
        if ( sIsValidName == FV_NULL )
        {
            hMsg = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_INPUT_BELLNAME), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, MYBELL_SAVE_INPUT_BELLNAME);
            break;
        }
        else if ( sIsValidName == FV_DIRCHAR )
        {
            hMsg = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_NOUSE_BELLNAME), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, MYBELL_SAVE_NOUSE_BELLNAME);
            break;
        }

        /* is there this file ? */
        /* make complete path */
        sprintf((char*)szPathName, "%s%s%s", SKY_MELODY_DIR, SKY_DIR_CHAR, pNameBuf);
        if(SKY_IsFileOrDirExist(szPathName) == TRUE)
        {            
            hMsg = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_EXIST_SAMENAME), 
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, MYBELL_SAVE_EXIST_SAMENAME);
            break;
        }

        if(pNameBuf != NULL)
        {
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_IN_SAVING), 
                                EQS_NA, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, MYBELL_IN_SAVING);                      
        }
        else
        {
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MYBELL_INPUT_BELLNAME), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, MYBELL_SAVE_INPUT_BELLNAME);
        }
        break;

    case SE_KEY_CLEAR:
    case SE_SOFTKEY_CANCEL:
        EQS_END();
        break;

    case SE_IO_FOCUS_CHG:
        if(pEvent->wParam == EQS_GetNthIOC(SS_MB_NAME))
        {
            EQS_SetWindowCML(CL_NONE_INPUT_SAVE);
        }
        else
        {
            EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
        }            
        break;           

    default :
		SKY_DefEventProc(pEvent);
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
//	Mybell Delete Function
//
void sDeleteMBMessage ( void )
{
//	vms_nv_type*	pVms = &ui.r_VmsBuffer[uis_nVmsList[uis_nVmsSel]];
	static              mybell_nv_type	    *pReadMybell;
    mybell_buf_type     *pMybell;

    pReadMybell = (mybell_nv_type*)SKY_Malloc( (SIZE_T)(sizeof(mybell_nv_type)) );

	//	Delete EFS memory
    pReadMybell->nAttr             = INIT_MBL_RECORD_ATTR;
    pReadMybell->nUsed             = INIT_MBL_RECORD_USED;
    pReadMybell->nDate[0]          = INIT_MBL_RECORD_DATE;
    pReadMybell->nDate[1]          = INIT_MBL_RECORD_DATE;
    pReadMybell->nDate[2]          = INIT_MBL_RECORD_DATE;
    pReadMybell->nDate[3]          = INIT_MBL_RECORD_DATE;
    pReadMybell->nTextSize         = INIT_MBL_RECORD_SIZE;
    memset(pReadMybell->szText, INIT_MBL_RECORD_TEXT, MYBELL_DATA_MAX); 

    SKY_SeekWriteMybellT( sMBList[nMBIndex], pReadMybell );

	//	Delete RAM memory
	pMybell = (mybell_buf_type*)&a_MybellBuffer[sMBList[nMBIndex]];
	pMybell->nUsed = INIT_MBL_RECORD_USED;
    pMybell->nAttr = INIT_MBL_RECORD_ATTR;


	sInitMyBellList();
    SKY_Free( pReadMybell );

    ui_sms_update_icons();
}

/////////////////////////////////////////////////////////////////////////////////////////
////        MYBELL EFS
/////////////////////////////////////////////////////////////////////////////////////////

VOID SKY_API SKY_InitMybell(VOID)
{
    HFILE   hFile;
    DWORD   dwCount, dwExistFileSize;
    INT8    i;

    if(SKY_IsFileOrDirExist(SKY_MBL_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_MBL_DIR);
    }


    /* open the file */
    hFile = SKY_OpenFile(FILE_NAME_MBL, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        /* set the initial value */
        sInitMybell();
    }
    else
    {
        dwExistFileSize = SKY_GetFileSize(FILE_NAME_MBL);
        dwCount = MAX_MYBELL_SIZE * sizeof(mybell_nv_type); 
        
        if(dwCount != dwExistFileSize)
        {
            SKY_CloseFile(hFile);
            SKY_DeleteFile(FILE_NAME_MBL);
            sInitMybell();
        }
        else
        {
            /* if file is already exist, read the data into the global buffer */
            for(i = 0; i < MAX_MYBELL_SIZE; i++)
            {
                SKY_ReadFile(hFile, &(MybellRecord), sizeof(MybellRecord));
                a_MybellBuffer[i].nAttr        = MybellRecord.nAttr;
                a_MybellBuffer[i].nUsed        = MybellRecord.nUsed;
                a_MybellBuffer[i].nDate[0]     = MybellRecord.nDate[0];
                a_MybellBuffer[i].nDate[1]     = MybellRecord.nDate[1];
                a_MybellBuffer[i].nDate[2]     = MybellRecord.nDate[2];
                a_MybellBuffer[i].nDate[3]     = MybellRecord.nDate[3];
            }
            /* close the file */
            SKY_CloseFile(hFile);
        }
    }
    ui_sms_update_icons();
}

LOCAL   VOID SKY_API sInitMybell( VOID )
{
    INT8    i;
    HFILE   hFile;

    /* jrkown : 2000.01.20 
    ** when try to open a file just after closing the file,
    ** fatal error occurs. --> change the method 
    */
    /* my bell file is not exist */
    /* there is not the file, create the file */
    hFile = SKY_OpenFile(FILE_NAME_MBL, FS_AM_CREATE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

    MybellRecord.nAttr          = INIT_MBL_RECORD_ATTR;
    MybellRecord.nUsed          = INIT_MBL_RECORD_USED;
    MybellRecord.nDate[0]       = INIT_MBL_RECORD_DATE;
    MybellRecord.nDate[1]       = INIT_MBL_RECORD_DATE;
    MybellRecord.nDate[2]       = INIT_MBL_RECORD_DATE;
    MybellRecord.nDate[3]       = INIT_MBL_RECORD_DATE;
    MybellRecord.nTextSize      = INIT_MBL_RECORD_SIZE;
    memset(MybellRecord.szText, INIT_MBL_RECORD_TEXT, MYBELL_DATA_MAX); 

    for(i = 0; i < MAX_MYBELL_SIZE; i++)
    {
        /* write data */
        SKY_WriteFile(hFile, &(MybellRecord), sizeof(MybellRecord));

        /* init data */
        a_MybellBuffer[i].nAttr        = MybellRecord.nAttr;
        a_MybellBuffer[i].nUsed        = MybellRecord.nUsed;
        a_MybellBuffer[i].nDate[0]     = MybellRecord.nDate[0];
        a_MybellBuffer[i].nDate[1]     = MybellRecord.nDate[1];
        a_MybellBuffer[i].nDate[2]     = MybellRecord.nDate[2];
        a_MybellBuffer[i].nDate[3]     = MybellRecord.nDate[3];
    }
    SKY_CloseFile(hFile);
}

VOID SKY_SeekReadMybellT( UINT8 unIdxOfMbl, mybell_nv_type* pReadRecord )
{
    HFILE   hFile;
    DWORD   dwCount;

    hFile = SKY_OpenFile(FILE_NAME_MBL, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    dwCount = SKY_SeekReadFile( 
                hFile, FS_SO_SET, unIdxOfMbl*sizeof(mybell_nv_type), pReadRecord, sizeof(mybell_nv_type) );
    SKY_CloseFile( hFile );
}


VOID SKY_SeekWriteMybellT( UINT8 unIdxOfMbl, mybell_nv_type* pWriteRecord )
{

    HFILE hFile;
    DWORD dwCount;

    hFile = SKY_OpenFile(FILE_NAME_MBL, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        hFile = SKY_OpenFile(FILE_NAME_MBL, FS_AM_CREATE);
    }
    dwCount = SKY_SeekWriteFile( 
                hFile, FS_SO_SET, unIdxOfMbl*sizeof(mybell_nv_type), pWriteRecord, sizeof(mybell_nv_type) );
    SKY_CloseFile( hFile );
}
VOID SKY_API gDeleteAllMybell( VOID )
{
    INT8    i;
    HFILE   hFile;

    if(SKY_IsFileOrDirExist(FILE_NAME_MBL) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_MBL, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        MybellRecord.nAttr          = INIT_MBL_RECORD_ATTR;
        MybellRecord.nUsed          = INIT_MBL_RECORD_USED;
        MybellRecord.nDate[0]       = INIT_MBL_RECORD_DATE;
        MybellRecord.nDate[1]       = INIT_MBL_RECORD_DATE;
        MybellRecord.nDate[2]       = INIT_MBL_RECORD_DATE;
        MybellRecord.nDate[3]       = INIT_MBL_RECORD_DATE;
        MybellRecord.nTextSize      = INIT_MBL_RECORD_SIZE;
        memset(MybellRecord.szText, INIT_MBL_RECORD_TEXT, MYBELL_DATA_MAX); 

        for(i = 0; i < MAX_MYBELL_SIZE; i++)
        {
            /* write data */
            SKY_WriteFile(hFile, &(MybellRecord), sizeof(MybellRecord));

            a_MybellBuffer[i].nAttr        = MybellRecord.nAttr;
            a_MybellBuffer[i].nUsed        = MybellRecord.nUsed;
            a_MybellBuffer[i].nDate[0]     = MybellRecord.nDate[0];
            a_MybellBuffer[i].nDate[1]     = MybellRecord.nDate[1];
            a_MybellBuffer[i].nDate[2]     = MybellRecord.nDate[2];
            a_MybellBuffer[i].nDate[3]     = MybellRecord.nDate[3];            
        }
        SKY_CloseFile(hFile);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MBL, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        MybellRecord.nAttr          = INIT_MBL_RECORD_ATTR;
        MybellRecord.nUsed          = INIT_MBL_RECORD_USED;
        MybellRecord.nDate[0]       = INIT_MBL_RECORD_DATE;
        MybellRecord.nDate[1]       = INIT_MBL_RECORD_DATE;
        MybellRecord.nDate[2]       = INIT_MBL_RECORD_DATE;
        MybellRecord.nDate[3]       = INIT_MBL_RECORD_DATE;
        MybellRecord.nTextSize      = INIT_MBL_RECORD_SIZE;
        memset(MybellRecord.szText, INIT_MBL_RECORD_TEXT, MYBELL_DATA_MAX); 

        for(i = 0; i < MAX_MYBELL_SIZE; i++)
        {
            SKY_WriteFile(hFile, &(MybellRecord), sizeof(MybellRecord));

            a_MybellBuffer[i].nAttr        = MybellRecord.nAttr;
            a_MybellBuffer[i].nUsed        = MybellRecord.nUsed;
            a_MybellBuffer[i].nDate[0]     = MybellRecord.nDate[0];
            a_MybellBuffer[i].nDate[1]     = MybellRecord.nDate[1];
            a_MybellBuffer[i].nDate[2]     = MybellRecord.nDate[2];
            a_MybellBuffer[i].nDate[3]     = MybellRecord.nDate[3];
        }
        SKY_CloseFile(hFile);

    }

	ui_sms_update_icons();
    
    g_MyBellSize = 0;
}