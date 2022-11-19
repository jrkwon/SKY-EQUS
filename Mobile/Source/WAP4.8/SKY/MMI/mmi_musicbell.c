/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     mmi_musicbell.c
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
** 01-01-12 yjahn96     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sky.h"
#include "clnt.h"
#include "aapiclnt.h"
#include "GenDsp.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define TITLE_STRING_MAX		2048

#define SBV_BELL            0   // ���ź� (in ska_bell.c)
#define Cal_mbSize(dword, byte1, byte2, byte3, byte4)  \
			(dword = (((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | (byte4)) + 8 ))

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef enum {
	Memory_File_Play  =  1,
	Streaming_Decode_Play  =  2
} MusicbellPlayType ;

typedef enum {
	StandardMIDI  =  1,
	MP3           =  2,
	MP4           =  3,
	WMA           =  4,
	Mybell        =  5,
    YAMAH_ma1     =  20, 
	YAMAH_mmf     =  21, 
	ROHM          =  22,
    Qualcomm_CMX  =  40
} MusicbellAudioType ;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
/*
enum{
	MAX_MUSICBELL_TITLE = 20,
	MAX_MUSICBELL_SIZE  = 4096,//     -- 16 poly
                              //2048 --  4 poly
    MUSICBELL_PLAY_MODE = 0,
    MUSICBELL_DOWNLOAD_MODE = 1
};
*/
enum tagWAP_MUSIC_BELL_INFO_E {
	MAX_MUSICBELL_TITLE = 20,
	MAX_MUSICBELL_SIZE  = 4096,//     -- 16 poly
                              //2048 --  4 poly
    MUSICBELL_PLAY_MODE = 0,
    MUSICBELL_DOWNLOAD_MODE = 1
};
//typedef enum tagWAP_MUSIC_BELL_INFO_E    WAP_MUSIC_BELL_INFO_E;

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
boolean		g_fMusicbellOn; 

extern boolean g_bStopMusic;
extern DownMusicbellType   MusicbellInfo;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
byte *mb_MusicBellPreBuffer;	//ma1������ musicbell�� �����ϴ� RAM�� buffer
static char musicResult[10];

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
BOOL WAP_IsValidFile(SKY_DATA_FILE_E SkyDataFile, CONST BYTE* pszFileName);
BOOL CheckDiskSpaceForSetAudio(void);
DWORD sCompareLengthofMusicData(int audioType, int Length, int mode);

void musicbell_check(void);
void musicbell_cmd(void);
void Musicbell_stop(char *Result);
void Musicbell_getlistTitle(char *Result);
void Musicbell_deletelist(char *Title, char *Result);
void Musicbell_memplay(const char* Title, char *Result);
void Musicbell_play(char *Data, int Length, 
					char *URL, const char* Title, 
					const char* AudioType, const char* PlayType, char *Result);
void Musicbell_download(char *Data, int Length, 
						char *URL, const char *Title, 
						const char* AudioType, const char* Attribute, char *Result);


extern void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen);
extern WORD sGetCurrentBellId(UINT8 nType);
extern byte get_length( byte *chk_str, byte buff_length);
extern void WAP_Repaint(void);
extern void *wip_malloc (UINT32 size);
extern void wip_free (void* mem);

/* 
** ---------------------------------------------------------------------------
*/
BOOL WAP_IsValidFile(SKY_DATA_FILE_E SkyDataFile,
                                  CONST BYTE* pszFileName)
{
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];

    BYTE *l_pszDataDir[] = {
        SKY_MELODY_DIR,
        SKY_IMAGE_DIR,
        SKY_GVM_DIR
    };

    /* make complete path */
    sprintf((char*)szPathName, "%s%s%s", l_pszDataDir[SkyDataFile], 
                                         SKY_DIR_CHAR,
                                         pszFileName);
    /* is there this file ? */
    if(SKY_IsFileOrDirExist(szPathName) == FALSE)
        return FALSE;
    else
        return TRUE;
}

/* 
** ---------------------------------------------------------------------------
** space check before downloading !!
*/
BOOL CheckDiskSpaceForSetAudio(void)
{
	MSG_HIGH("CheckDiskSpaceForSetAudio",0,0,0);
	//file�� ������ full���� check!!
    if(SKY_GetFileNumberInDataFile(SDF_MELODY) >= SKY_MAX_NUM_FILES)
    {
		MSG_ERROR("Filenum_Full",0,0,0);
		return FALSE;	//file�� ���� full
	}
	else
	{
        // ������ ������ �ִ��� Ȯ��
    	if(SKY_GetFreeSpaceInDataFile() < MAX_MUSICBELL_SIZE + MM_NUM_FILE_TYPE )  //header "MMF "
		{
		    MSG_ERROR("+++++++++ space is full!! ++++++++",0,0,0);
			return FALSE;	//������ ������ ���� ���
		}
		else
		{
			MSG_ERROR("+++++++++ space is ok !! ++++++++",0,0,0);
			return TRUE;	//������ ������ �ִ� ���
		}
	}
}

/* 
** ---------------------------------------------------------------------------
*/
void musicbell_check(void)  //�� �Լ� ��������
{
	if (g_fMusicbellOn)//ui.musicbell_on ) 
	{
		Musicbell_stop(musicResult);
		MSG_MED("musicbell stop",0,0,0);
	}else
	{
		MSG_MED("musicbell off state",0,0,0);
	}
}


/* 
** ---------------------------------------------------------------------------
** Compare the Length of Music Data  
** 
** return value : 0    - not equal
**                Size - Size of Music Data 
*/
DWORD sCompareLengthofMusicData(int audioType, int Length, int mode)
{
    //mode == 0 : musicbell_play() 
    //mode == 1 : musicbell_download()  for header "MMF "

    dword Size = 0;

    switch(audioType)
    {
    case YAMAH_ma1:
        //�����δ� down�޾� ma1���� �ٲ� file�� size�̾����
        Size = *mb_MusicBellPreBuffer;
        break;
    case YAMAH_mmf:
        //�����δ� down�޾� ma1���� �ٲ� file�� size�̾����
        Cal_mbSize(Size, mb_MusicBellPreBuffer[4], mb_MusicBellPreBuffer[5],
                 mb_MusicBellPreBuffer[6], mb_MusicBellPreBuffer[7]); 
        break;
    default:
        break;
    }

    Size += (MM_NUM_FILE_TYPE*mode);    //4*0(play) / 1(download)

    if((int)Size != Length + (MM_NUM_FILE_TYPE*mode))
	{
		if(mb_MusicBellPreBuffer)
		{
			MSG_ERROR("mb_MusicBellPreBuffer =NULL",0,0,0);
			wip_free((void*)mb_MusicBellPreBuffer);
			mb_MusicBellPreBuffer =NULL;
		}
        Size = 0;
	}                

    return Size;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     Musicbell_play()
**
** ---------------------------------------------------------------------------
*/
void Musicbell_play(char *Data, int Length, 
					char *URL, const char* Title, 
					const char* AudioType, const char* PlayType, char *Result)
{
	MusicbellPlayType play;
	MusicbellAudioType audio;

	//------------------------------
	// Play�� ��ҵ� ���, ����������
	//  ex)�ٸ� ī����� �̵�(newCard)�� ��� setting�ȴ�
	if(g_bStopMusic) return;

    //------------------------------
    g_fMusicbellOn = TRUE;//	ui.musicbell_on = TRUE;

    memset(Result, NULL, MAX_SIZE_MUSIC_RESULT);
	
	if(Data != NULL && Length > 2)
	{
		MusicbellInfo.Length = Length;	//for "NOT"

		//string���� int������ ��ȯ
		play = (MusicbellPlayType)atoi(PlayType);	// woodstok 00.4.28 cast
		audio = (MusicbellAudioType)atoi(AudioType);

		if(mb_MusicBellPreBuffer){
			wip_free((void*)mb_MusicBellPreBuffer);
			mb_MusicBellPreBuffer =NULL;
		}
		mb_MusicBellPreBuffer = (BYTE*)wip_malloc(Length);
		if(mb_MusicBellPreBuffer == NULL)
		{
			MSG_ERROR("Wip Malloc Fail",0,0,0);
			return;
		}
		memcpy(mb_MusicBellPreBuffer, Data, Length);

        if(0 == sCompareLengthofMusicData(audio, Length, MUSICBELL_PLAY_MODE))
		{
			MSG_ERROR("Music Data Length is different",0,0,0);
            return;
		}

		switch(play)
		{
			case Memory_File_Play:
				// Memory File Play - RAM�� ����Ǿ� �ִ� ����ȭ���� �̸����
    			switch(audio)
				{
                    case YAMAH_ma1:		//YAMAH�� ��츸..
                        break;

                    case YAMAH_mmf:		//YAMAH�� ��츸..
                        SKY_PlayMusicBufferEx((byte*)mb_MusicBellPreBuffer, SKY_GET_BELL_VOLUME(), 0);     //play music in buffer
						strcpy(Result, "0");	//success
						break;
					default:	
						MSG_ERROR("Out of Audio Type",0,0,0);
						break;	//�׿��� ���� ����
				}  
				break;
			case Streaming_Decode_Play:		
				// Streaming Decode Play(MOD) - Down�ϸ鼭 ���ÿ� ��ε���
				switch(audio)
				{
					case YAMAH_ma1:	//YAMAH�� ��츸..
                    case YAMAH_mmf:
						break;
					default:
						MSG_ERROR("Out of Audio Type",0,0,0);
						break;		//�׿��� ���� ����
				}  
				break;
			default:
                MSG_ERROR("Out of Play Type",0,0,0);
				break;
		}	//end of switch(play)
	}else
	{
		MSG_ERROR("DATA is Null",0,0,0);
	}
} //end of void Musicbell_play(char *Data, int Length, char *URL, const char* Title, const char* AudioType, const char* PlayType, char *Result);

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     Musicbell_download()
**
** ---------------------------------------------------------------------------
*/

void Musicbell_download(char *Data, int Length, 
						char *URL, const char *Title, 
						const char* AudioType, const char* Attribute, char *Result)
{
	dword			mbSize;
	int				audio;
	int				attribute;
	char			musicTitle[22];
//    extern boolean  ui_musicbell_default;
	boolean         IsValid = FALSE;	//Title�� ���� ����ȭ���� ���������ϴ��� ����

	MSG_ERROR("Musicbell_download",0,0,0);

	memset(Result, NULL, MAX_SIZE_MUSIC_RESULT);

	audio = atoi(AudioType);
	
	//Invalid parameter check!!
	if (Title == NULL)// || audio != YAMAH_ma1)
	{
			strcpy(Result, "1003");		// Invalid_parameter
			MSG_ERROR("Title is Invaild",0,0,0);
			return;
	}
	// if music name is over 14 then truncation...
	memset(musicTitle, NULL, 22);
	WAP_HanCopy(musicTitle, (char *)Title, get_length((byte *)Title, SKY_FILE_NAME_MAX_LENGTH-1));


	// ����Ÿ�� Null�̰ų� ����Ÿ�� ���̰� 0�� ���, ���� ����
	if(Data == NULL || Length <= 2)
    {
			strcpy(Result, "1006");		// Undefined_Error
			MSG_ERROR("Data is Null",0,0,0);
	}
	else
	{
		MusicbellInfo.Length = Length;	//for "NOT"

		if(mb_MusicBellPreBuffer)
		{
			wip_free((void*)mb_MusicBellPreBuffer);
			mb_MusicBellPreBuffer =NULL;
		}

		mb_MusicBellPreBuffer = (byte *)wip_malloc(Length + 5);
		if(mb_MusicBellPreBuffer == NULL) 
		{
			MSG_ERROR("Wip Malloc Fail",0,0,0);
			return;
		}
		memset(mb_MusicBellPreBuffer, 0x00, Length+5);
		memcpy(mb_MusicBellPreBuffer, "MMF ", MM_NUM_FILE_TYPE);   //(B0���� �ش�)phone�� �����ϱ� ���� header(4 bytes)
		memcpy(mb_MusicBellPreBuffer+MM_NUM_FILE_TYPE, Data, Length);

		//file�� ������ full���� check!!
        if(SKY_GetFileNumberInDataFile(SDF_MELODY) >= SKY_MAX_NUM_FILES)
        {
            strcpy(Result, "1004");		//file�� ���� full
			MSG_ERROR("Filenum_Full",0,0,0);
		}
		else
		{	
            mbSize = sCompareLengthofMusicData(audio, Length, MUSICBELL_DOWNLOAD_MODE);            
            if(mbSize == 0) return;

            // ������ ������ �ִ��� Ȯ��
            /*
            ** jrkwon : 2001.08.18 --> we don't need to check this in here, 
            ** becacue we are going to check this in SKY_WriteDataFile
            **
            */
            //if(mbSize > SKY_GetFreeSpaceInDataFile())
            //{
			//	strcpy(Result, "1005");		//���� ������ ���� ���
			//	MSG_ERROR("Buffer_Full",0,0,0);
			//}
			//else
			{
				//Title�� ���� �����ϴ��� check!! -- �����ϸ� delete��, ���� 
				IsValid = WAP_IsValidFile(SDF_MELODY, (BYTE *)musicTitle);
				if(IsValid)		//�����ϴ� ���
					SKY_DeleteDataFile(SDF_MELODY,(BYTE *)musicTitle);

				// �������
                if(SKY_WriteDataFile(SDF_MELODY, (BYTE*)musicTitle, mbSize, mb_MusicBellPreBuffer) == SDR_OK)
                {
					strcpy(Result, "0");		//success

					attribute = atoi(Attribute);
					if(attribute == 1)
					{
						//�⺻ ���ź��� ����
                        //ui_musicbell_default = TRUE;
                        SKY_SET_BELL_FILE_NAME(musicTitle);
                        SKY_SET_BELL_ID(SKY_MY_BELL_ID);
                    }
				}
                /* jrkwon 2001.08.16 */
                else
                {
			    	strcpy(Result, "1005");		//���� ������ ���� ���
			    	MSG_ERROR("Buffer_Full",0,0,0);
                }
			}	//end of ������ ���� �ִ� ���
		}	//end of file num not full  
	}	//end of if(data != NULL)
	if(mb_MusicBellPreBuffer){
		wip_free((void*)mb_MusicBellPreBuffer);
		mb_MusicBellPreBuffer =NULL;
	}
}  //end of void Musicbell_download(char *Data, int Length, char *URL, const char *Title, const char* AudioType, const char* Attribute, char *Result)

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     Musicbell_stop()
**
** ---------------------------------------------------------------------------
*/
void Musicbell_stop(char *Result)
{
	memset(Result, NULL, MAX_SIZE_MUSIC_RESULT);

	SKY_StopMusic();	

    g_fMusicbellOn = FALSE; //	ui.musicbell_on = FALSE;

	if(mb_MusicBellPreBuffer){
		wip_free(mb_MusicBellPreBuffer);	
		mb_MusicBellPreBuffer =NULL;	//�ʱ�ȭ
	}

	strcpy(Result, "0");	//if ���������� �����ϸ�, result = 0;
}  //end of void Musicbell_stop(char *Result)


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     Musicbell_getlistTitle()
**
** ---------------------------------------------------------------------------
*/
void Musicbell_getlistTitle(char *Result)
{
	UINT8	nLines = 0, index = 0;
    STATIC BYTE *ppFileList[SKY_MAX_NUM_FILES];
    
	MSG_ERROR("Musicbell_getlistTitle",0,0,0);

	memset(Result, NULL, TITLE_STRING_MAX);

    // Load user file list
    for(index=0; index < SKY_MAX_NUM_FILES; index ++)
    {
		ppFileList[index] = (BYTE*)wip_malloc(SKY_PATH_NAME_MAX_LENGTH);
		EQS_ASSERT(ppFileList[index] != NULL);
	}

    nLines = SKY_GetFileExtList(SKY_MELODY_DIR, ppFileList);//SKY_GetFileList(SKY_MELODY_DIR, ppFileList);

	MSG_ERROR("nLines[%d]",nLines,0,0);
	if(nLines > 0)
	{
        for(index=0; index < nLines; index ++)
        {
            strcat(Result, (char*)ppFileList[index]);
            if(index < (nLines - 1))    strcat(Result, ",");
			//jmaprk 010311. why attatched as "0x06" instead of "0x00" ?
//          else                        strcat(Result, '\0');  
        }
    }
	else
	{
		//���� ����� data�� �ϳ��� ���� ���
		MSG_ERROR("No stored data",0,0,0);
	}	
//	CLNTa_log(1, 0, "Result[%s]",Result);
    for(index=0; index < nLines; index ++)
    {
        wip_free(ppFileList[index]);
        ppFileList[index] = NULL;
    }
}  //end of void Musicbell_getlistTitle(char *Result)


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     Musicbell_deleteList()
**
** ---------------------------------------------------------------------------
*/
void Musicbell_deletelist(char *Title, char *Result)
{
	boolean IsValid = FALSE;

	MSG_ERROR("Musicbell_deletelist [%c][%c][%c]",Title[0],Title[1],Title[2]);
	memset(Result, NULL, 10);

	IsValid = WAP_IsValidFile(SDF_MELODY, (BYTE *)Title);
	if(IsValid)
    {
        SKY_DeleteDataFile(SDF_MELODY,(BYTE *)Title);
		strcpy(Result, "0");	//success
	}
	else
    {
		MSG_ERROR("Not found File",0,0,0);
	}
}  //end of char* Musicbell_deletelist(char *Title)


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     Musicbell_memplay()
**
** ---------------------------------------------------------------------------
*/
void Musicbell_memplay(const char* Title, char *Result)
{
	boolean IsValid = FALSE;

    g_fMusicbellOn = TRUE;  //    ui.musicbell_on = TRUE;

	memset(Result, NULL, MAX_SIZE_MUSIC_RESULT);

	IsValid = WAP_IsValidFile(SDF_MELODY, (BYTE *)Title);	
	if(IsValid)
	{
		//========================================================
		//Message for user
		//WAP_ClearScreen();
		//WAP_SetTextAtLine(3, "���� �غ����Դϴ�",WAP_TEXT_ALIGN_CENTER);
		//WAP_DisplayMessage();
		//========================================================

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WAP_DOWNLOAD_BELL_PLAY), 0, MBT_NONE, MBT_HELP);

		SKY_PlayMusicFile((BYTE *)Title);
		strcpy(Result, "0");  //success
	}
	else
	{
		MSG_ERROR("Not found File",0,0,0);
		return;
	}

	//for Repaint Screen
	//musicbell_cmd();
    WAP_Repaint();
}	//end of void Musicbell_memplay(const char* Title, char *)
