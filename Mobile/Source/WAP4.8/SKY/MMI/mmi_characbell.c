/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     mmi_characbell.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When        who        what, where, why
** ---------- --------   ------------------------------------------------------
** 01-09-07    bgjung     Created.
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
#include "MMFdecoder.h"
#include "SISdecoder.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
//#define MAX_CHARACBELL_SIZE         10240 // 10K
//#define TITLE_STRING_MAX            2048

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
//unsigned char *CBImageData, 
unsigned char   *CBMelodyData, *CBtitle, *CBtext;
UINT16          s_nDecodedImageSize = 0;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
// ĳ���� �ٿ�ε带 ���� �Լ�
int Process_DownloadedCharacbell(char *Data, int Length, 
						char *URL, char *cbelltitle, 
						char *imagetitle, char *melodytitle,
                        char *callernum1, char *callernum2, char *callernum3);
void sExitDownloadCharacbell(void);
// ĳ���� ��������� ������� Ȯ���Ѵ�.
// Ȯ���ϴ� ���� ĳ���� �ִ����尳���� �ʰ��ϴ��� & ������ ��������� �ִ���..
//BOOL CheckDiskSpaceForSetCharacbell(void); 
/*
// ��������� �����Ͽ� ĳ������ �����ϴ� ��� ��� �����ֱ�
void Characbell_getlistTitle(char *Result);

// ������ ĳ���� ����
void Characbell_delete(char *Title, char *Result);
*/
// �ܺ� �Լ� ���
//extern void BeforeDelete (int index); // ���� �� ���� ���� Ȯ��.
extern void CheckDuplicateNumber (char *temp, int DeletedIndex); // ���Ź�ȣ �ߺ� Ȯ��
extern BOOL SaveWAP_AnimationToFS(char *pSISData, int leng, char *imgName, char *url);
extern BOOL SaveWAP_BMPToFS(char *imgData, int leng, char *imgName);
extern BOOL SaveWAP_WBMPToFS(char *pWBMPData, int leng, char* imgName);
extern void SetCharacbellSelected(BOOL selected);

extern void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen);
extern byte get_length( byte *chk_str, byte buff_length);
extern void *wip_malloc (UINT32 size);
extern void wip_free (void* mem);

// ��ũ ���� Ȯ��.
/*BOOL CheckDiskSpaceForSetCharacbell(void)
{
	MSG_HIGH("CheckDiskSpaceForSetCharacbell",0,0,0);
	
        // ������ ������ �ִ��� Ȯ�� (MAX_CHARACBELL_SIZE = 10K)
    	if(SKY_GetFreeSpaceInDataFile() < MAX_CHARACBELL_SIZE)
		{
		    MSG_ERROR("+++++++++ space is full!! ++++++++",0,0,0);
			return FALSE;	//������ ������ ���� ���
		}
		else
		{
			MSG_ERROR("+++++++++ space is ok !! ++++++++",0,0,0);
			return TRUE;	//������ ������ �ִ� ���
		}
} // CheckDiskSpaceForSetCharacbell*/

int Process_DownloadedCharacbell(char *Data, int Length, 
						char *URL, char *cbelltitle, 
						char *imagetitle, char *melodytitle,
                        char *callernum1, char *callernum2, char *callernum3)
{
    BYTE        szMelodyPathName[SKY_PATH_NAME_MAX_LENGTH];
    BYTE        szImagePathName[SKY_PATH_NAME_MAX_LENGTH];
	char        CBTitle[22];
	char        CBImageTitle[22];
    char        CBMelodyTitle[22];
    char        CBPhone[MAX_PHONENUMBER_LENGTH];
    char        temp[MAX_TITLE_LENGTH];
    int         titleSize, ImageSize, MelodySize, textSize;
    int         infoCB, resultCB, ImageType, MelodyType;
    boolean     bMelodyExist = FALSE;
    boolean     bImageExist = FALSE;


    MSG_ERROR("Characbell_download",0,0,0);

	// ERROR Check
    if (SKY_GET_CB_ADDRESS() >= MAX_NUM_OF_CHARACBELL) 
    {
        MSG_ERROR("Characbell Address >= 5", 0, 0, 0);
        return 1004;
    }
    else if (callernum1 == NULL)
    {// phone1�� �ʼ��Է��׸� üũ�� �Ǿ���� ���̴�.
        MSG_ERROR("PhoneNumber1 is Invaild",0,0,0);
        return 1003;
    }
    else if (Data == NULL) 
    {// ���⼭ Length�� ��� ���̴°ɱ�?
        MSG_ERROR("Data is Null",0,0,0);
        return 1006;
	}
    /*else if (!(CheckDiskSpaceForSetCharacbell())) 
    {
        MSG_ERROR("Space is full!!", 0, 0, 0);
        return 1005;
    }*/

    
    // if title is over 14 then truncation...
	memset(CBTitle, NULL, 22);
    memset(CBImageTitle, NULL, 22);
    memset(CBMelodyTitle, NULL, 22);
	WAP_HanCopy(CBTitle, (char *)cbelltitle, get_length((byte *)cbelltitle, SKY_FILE_NAME_MAX_LENGTH-1));
    WAP_HanCopy(CBImageTitle, (char *)imagetitle, get_length((byte *)imagetitle, SKY_FILE_NAME_MAX_LENGTH-1));
    WAP_HanCopy(CBMelodyTitle, (char *)melodytitle, get_length((byte *)melodytitle, SKY_FILE_NAME_MAX_LENGTH-1));

	// MMF Data�� InfoȮ��...
    infoCB = getMMFInfo((OCTET* )Data, &titleSize, &ImageSize, 
	                    &MelodySize, &textSize);
    if (infoCB == 0)
        return 1002; // ERROR
    else 
    {
        if(CBtitle)
        {
			wip_free((void*)CBtitle);
			CBtitle =NULL;
		}
        CBtitle = (BYTE*)wip_malloc(titleSize);

        /*if(CBImageData)
        {
			wip_free((void*)CBImageData);
			CBImageData =NULL;
		}
        CBImageData = (BYTE*)wip_malloc(ImageSize);*/

        if(CBMelodyData)
        {
			wip_free((void*)CBMelodyData);
			CBMelodyData =NULL;
		}
        CBMelodyData = (BYTE*)wip_malloc(MelodySize);

        if(CBtext)
        {
			wip_free((void*)CBtext);
			CBtext =NULL;
		}
        CBtext = (BYTE*)wip_malloc(textSize);

        memset((unsigned char*)CBtitle, 0, titleSize);
        //memset((unsigned char*)m_gUncompBuf, 0, ImageSize);
        memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
        memset((unsigned char*)CBMelodyData, 0, MelodySize);
        memset((unsigned char*)CBtext, 0, textSize);

        resultCB = mmfDecode ((OCTET*)Data, CBtitle, m_gUncompBuf, &ImageType,
                                CBMelodyData, &MelodyType, CBtext);
		
        if (resultCB == 0)
        {
            sExitDownloadCharacbell();
            return 1002;
        }
        if ((MelodyType != 12) || ((ImageType != 1) && (ImageType != 3) && (ImageType != 4))) 
        {
            sExitDownloadCharacbell();
            return 1002;
        } // �����ϴ� ������ �ƴ� ��� ��/�׸� �ƹ��͵� ���� �ȵǵ���..

        // mmf (ma2) �� �Ҹ� ����
        if(SKY_GetFileNumberInDataFile(SDF_MELODY) >= SKY_MAX_NUM_FILES)
        {
		    ERR("Music Filenum_Full",0,0,0);
            sExitDownloadCharacbell();
		    return 1005;	//file�� ���� full
	    }
	    else if (MelodySize > SKY_GetFreeSpaceInDataFile())
        {
            ERR("Characbell ������� ������",0,0,0);
            sExitDownloadCharacbell();
            return 1005;
        }

        /* make complete path */
        sprintf((char*)szMelodyPathName, "%s%s%s", SKY_MELODY_DIR, SKY_DIR_CHAR, CBMelodyTitle);

        /* is there this file ? */
        if (SKY_IsFileOrDirExist(szMelodyPathName)) 
        {
            SKY_DeleteDataFile(SDF_MELODY,(BYTE *)CBMelodyTitle);
            bMelodyExist = TRUE;
        }

        // �������
        if(SKY_WriteDataFile(SDF_MELODY, (BYTE*)CBMelodyTitle, MelodySize, CBMelodyData) != SDR_OK) 
        {
            if (SKY_IsFileOrDirExist(szMelodyPathName) && !bMelodyExist) 
                SKY_DeleteDataFile(SDF_MELODY,(BYTE *)CBMelodyTitle);

            sExitDownloadCharacbell();
            return 1006;
        }
                
        // IMAGE
        if(SKY_GetFileNumberInDataFile(SDF_IMAGE) >= SKY_MAX_NUM_FILES)
        {
		    ERR("Image Filenum_Full",0,0,0);
            if (SKY_IsFileOrDirExist(szMelodyPathName) && !bMelodyExist)
                    SKY_DeleteDataFile(SDF_MELODY,(BYTE *)CBMelodyTitle);

            sExitDownloadCharacbell();
		    return 1005;	//file�� ���� full
	    }
	    else if (s_nDecodedImageSize > SKY_GetFreeSpaceInDataFile())
        {
            ERR("Characbell ������� ������",0,0,0);
            if (SKY_IsFileOrDirExist(szMelodyPathName) && !bMelodyExist)
                    SKY_DeleteDataFile(SDF_MELODY,(BYTE *)CBMelodyTitle);

            sExitDownloadCharacbell();
            return 1005;
        }

        sprintf((char*)szImagePathName, "%s%s%s", SKY_IMAGE_DIR, 
                                         SKY_DIR_CHAR,
                                         CBImageTitle);
        /* is there this file ? */
        if(SKY_IsFileOrDirExist(szImagePathName)) 
        {
	        SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)CBImageTitle);
            bImageExist = TRUE;
        }

        if (ImageType == 1)	
        {		// BMP type
#ifdef FEATURE_CHARACBELL
	        SetCharacbellSelected(TRUE);
#endif // FEATURE_CHARACBELL
            //if(!SaveWAP_BMPToFS((char *)CBImageData, ImageSize, CBImageTitle)) {
            if(!SaveWAP_BMPToFS((char *)m_gUncompBuf, ImageSize, CBImageTitle)) 
            {
                if (SKY_IsFileOrDirExist(szMelodyPathName) && !bMelodyExist)
                    SKY_DeleteDataFile(SDF_MELODY,(BYTE *)CBMelodyTitle);
                if (SKY_IsFileOrDirExist(szImagePathName) && !bImageExist)
                    SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)CBImageTitle);

                sExitDownloadCharacbell();
			    return 1006;		// invalid image type
            }
	    } 
	    else if (ImageType == 3)	// WBMP type
	    {
#ifdef FEATURE_CHARACBELL
            SetCharacbellSelected(TRUE);
#endif // FEATURE_CHARACBELL
            //if(!SaveWAP_WBMPToFS((char *)CBImageData, ImageSize, CBImageTitle)) {
            if(!SaveWAP_WBMPToFS((char *)m_gUncompBuf, ImageSize, CBImageTitle)) 
            {
                if (SKY_IsFileOrDirExist(szMelodyPathName) && !bMelodyExist)
                    SKY_DeleteDataFile(SDF_MELODY,(BYTE *)CBMelodyTitle);
                if (SKY_IsFileOrDirExist(szImagePathName) && !bImageExist)
                    SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)CBImageTitle);

                sExitDownloadCharacbell();
			    return 1006;
            }
	    } 
	    else if (ImageType == 4)	// Animation type
	    {
#ifdef FEATURE_CHARACBELL
            SetCharacbellSelected(TRUE);
#endif // FEATURE_CHARACBELL
            //if(!SaveWAP_AnimationToFS((char *)CBImageData, ImageSize, CBImageTitle, "")) {
            if(!SaveWAP_AnimationToFS((char *)m_gUncompBuf, ImageSize, CBImageTitle, "")) 
            {
                if (SKY_IsFileOrDirExist(szMelodyPathName) && !bMelodyExist)
                    SKY_DeleteDataFile(SDF_MELODY,(BYTE *)CBMelodyTitle);
                if (SKY_IsFileOrDirExist(szImagePathName) && !bImageExist)
                    SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)CBImageTitle);

                sExitDownloadCharacbell();
			    return 1006;
            }
	    }

        // ĳ���� ������ ���Ͽ� ����.
        // ���Ź�ȣ �ߺ��� ���ؼ� üũ�ؾ� �Ѵ�.
        {
            WORD        index, i;

            index = 0;
            for (i = 0; i < MAX_NUM_OF_CHARACBELL; i++) 
            {
                if (SKY_GET_CB_EMPTY(i) == 0) 
                    break;
            }
            index = i; // ������ �ε���
            
            if (*CBTitle == NULL) 
            {
                memset(temp, 0x00, MAX_TITLE_LENGTH);
                if ((strlen(callernum1) > 0) && (callernum1[0] != '0'))
                {
                    if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                    {
                        strcpy(temp, "011");
                    }
                    else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                    {
                        strcpy(temp, "017");
                    }
                    strcat(temp, callernum1);
                }
                else
                {
                    strcpy(temp, callernum1);
                }

                SKY_SET_CB_TITLE(index, temp);
            }
            else 
            {
                SKY_SET_CB_TITLE(index, CBTitle);
            }

            SKY_SET_CB_MTYPE(index, SKY_MY_BELL_ID);
            SKY_SET_CB_MTITLE(index, CBMelodyTitle);
            SKY_SET_CB_ITYPE(index, sMY_PICTURE);
            SKY_SET_CB_ITITLE(index, CBImageTitle);
                        
            // Check duplicated number
            memset(CBPhone, 0x00, MAX_PHONENUMBER_LENGTH);
            memset(temp, 0x00, MAX_TITLE_LENGTH);
            if ((strlen(callernum1) > 0) && (callernum1[0] != '0'))
            {
                if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                {
                    strcpy(temp, "011");
                }
                else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                {
                    strcpy(temp, "017");
                }
                strcat(temp, callernum1);
            }
            else
            {
                strcpy(temp, callernum1);
            }
            temp[strlen(temp)] = ',';
            CheckDuplicateNumber (temp, index);
            strcpy(CBPhone, temp);

            memset(temp, 0x00, MAX_TITLE_LENGTH);
            if ((strlen(callernum2) > 0) && (callernum2[0] != '0'))
            {
                if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                {
                    strcpy(temp, "011");
                }
                else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                {
                    strcpy(temp, "017");
                }
                strcat(temp, callernum2);
            }
            else
            {
                strcpy(temp, callernum2);
            }
            temp[strlen(temp)] = ',';
            CheckDuplicateNumber (temp, index);
            strcat(CBPhone, temp);

            memset(temp, 0x00, MAX_TITLE_LENGTH);
            if ((strlen(callernum3) > 0) && (callernum3[0] != '0'))
            {
                if (SKY_GET_SERVICE_PROVIDER() == SP_SKT)
                {
                    strcpy(temp, "011");
                }
                else if (SKY_GET_SERVICE_PROVIDER() == SP_STI)
                {
                    strcpy(temp, "017");
                }
                strcat(temp, callernum3);
            }
            else
            {
                strcpy(temp, callernum3);
            }
            temp[strlen(temp)] = ',';
            CheckDuplicateNumber (temp, index);
            strcat(CBPhone, temp);

            strcat(CBPhone, ",,,");

            SKY_SET_CB_PHONE(index, CBPhone);
            SKY_SET_CB_EMPTY(index, 1);
            SKY_SET_CB_ADDRESS(SKY_GET_CB_ADDRESS() + 1);  
        }
        sExitDownloadCharacbell();
        MSG_ERROR("======== Characbell_download END =======",0,0,0);
        return 0; // ����
    }
}// Process_DownloadedCharacbell

void sExitDownloadCharacbell (void)
{
    if (CBMelodyData)
    {
		wip_free(CBMelodyData);	
		CBMelodyData =NULL;	//�ʱ�ȭ
    }
    /*if (CBImageData)
    {
		wip_free(CBImageData);	
		CBImageData =NULL;	//�ʱ�ȭ
    }*/
    if (CBtitle)
    {
		wip_free(CBtitle);	
		CBtitle =NULL;	//�ʱ�ȭ
    }
    if (CBtext)
    {
		wip_free(CBtext);	
		CBtext =NULL;	//�ʱ�ȭ
    }
} // sExitDownloadCharacbell
/*
void Characbell_getlistTitle (char *Result)
{
    UINT8            index, numOfCB, i;
	STATIC BYTE     *ppFileList[MAX_NUM_OF_CHARACBELL];
        
	MSG_ERROR("Characbell_getlistTitle",0,0,0);

	memset(Result, NULL, TITLE_STRING_MAX);

    numOfCB = SKY_GET_CB_ADDRESS();
    MSG_ERROR("����� ĳ���� ���� [%d]", numOfCB, 0, 0);

    // Initialize
    for(index=0; index < numOfCB; index ++)
    {
		ppFileList[index] = (BYTE*)wip_malloc(SKY_PATH_NAME_MAX_LENGTH);
		EQS_ASSERT(ppFileList[index] != NULL);
	}

    // Load characbell data
    i = 0;
    for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
        if (SKY_GET_CB_EMPTY(index) == 1) {
            strcpy((char*)ppFileList[i], SKY_GET_CB_TITLE(index));
            i++;
        }
    }

    if(numOfCB > 0) {
        for(index=0; index < numOfCB; index ++) {
            strcat(Result, (char*)ppFileList[index]);
            if(index < (numOfCB - 1))    strcat(Result, ",");
        }
    }
    else {
		//���� ����� data�� �ϳ��� ���� ���
		MSG_ERROR("No stored data",0,0,0);
	}	

    for(index=0; index < numOfCB; index ++)
    {
        wip_free(ppFileList[index]);
        ppFileList[index] = NULL;
    }
} // Characbell_getlistTitle 

void Characbell_delete(char *Title, char *Result)
{
	boolean IsValid = FALSE;
    UINT8    index;
    char    imageTitle[MAX_TITLE_LENGTH], melodyTitle[MAX_TITLE_LENGTH];

    MSG_ERROR("Characbell_deleteImage [%c][%c][%c]",Title[0],Title[1],Title[2]);

    for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
        if (!(strcmp(Title, SKY_GET_CB_TITLE(index)))) {
            strcpy(imageTitle, SKY_GET_CB_ITITLE(index));
            strcpy(melodyTitle, SKY_GET_CB_MTITLE(index));
            break;
        }
    }

    if (index < MAX_NUM_OF_CHARACBELL) {
        memset(Result, NULL, 10);
        // ����� ���� ������� �ϴ� �۾��� ���⼭�� ����� �Ѵ�.
        BeforeDelete(index);

        SKY_SET_CB_ADDRESS(SKY_GET_CB_ADDRESS() - 1);
        SKY_SET_CB_EMPTY(index, 0x00);
        SKY_SET_CB_TITLE(index, 0x00);
        SKY_SET_CB_MTYPE(index, 0x00);
        SKY_SET_CB_MTITLE(index, 0x00);
        SKY_SET_CB_ITYPE(index, 0x00);
        SKY_SET_CB_ITITLE(index, 0x00);
        SKY_SET_CB_PHONE(index, 0x00);

		strcpy(Result, "0");	//success
	}
    else
        MSG_ERROR("Not found File",0,0,0);
} // Characbell_delete */

/*DWORD sCompareLengthofMelodyData(int audioType, int Length)//, int mode)
{
    //mode == 0 : characbell_play() 
    //mode == 1 : characbell_download()  for header "MMF "
    dword Size = 0;

    switch(audioType)
    {
    case YAMAH_ma1:
        //�����δ� down�޾� ma1���� �ٲ� file�� size�̾����
        Size = *mb_CharacBellPreBuffer;
        break;
    case YAMAH_mmf:
        //�����δ� down�޾� ma1���� �ٲ� file�� size�̾����
        Cal_mbSize(Size, mb_CharacBellPreBuffer[4], mb_CharacBellPreBuffer[5],
                 mb_CharacBellPreBuffer[6], mb_CharacBellPreBuffer[7]); 
        break;
    default:
        break;
    }

    //Size += (MM_NUM_FILE_TYPE*mode);    //4*0(play) / 1(download)
    //if((int)Size != Length + (MM_NUM_FILE_TYPE*mode))
    Size += (MM_NUM_FILE_TYPE*1);    //4*0(play) / 1(download)
    if((int)Size != Length + (MM_NUM_FILE_TYPE*1))
	{
		if(mb_CharacBellPreBuffer)
		{
			MSG_ERROR("mb_CharacBellPreBuffer =NULL",0,0,0);
			wip_free((void*)mb_CharacBellPreBuffer);
			mb_CharacBellPreBuffer =NULL;
		}
        Size = 0;
	}                

    return Size;
} // sCompareLengthofMelodyData*/
