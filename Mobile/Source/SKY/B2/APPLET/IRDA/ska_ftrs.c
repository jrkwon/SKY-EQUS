/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_ftrs.c
**
** Description: 
**     IrDA File Transfer
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-24 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define PHOTO_TOTAL_SIZE    10800*2
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

enum tag_FILE_RX_STATE_E {
	FT_NONE_C,
	FT_GOT_INF_C,
	FT_BLOCK_WAIT_C
};

typedef enum tag_FILE_RX_STATE_E FILE_RX_STATE_E;

enum tag_FILE_TX_STATE_E {
	FT_NONE_S,
	FT_MODEL_REQ_S,
	FT_MODEL_WAIT_S,
	FT_INF_RSP_WAIT_S,
	FT_BLOCK_RSP_WAIT_S
};

typedef enum tag_FILE_TX_STATE_E FILE_TX_STATE_E;

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

extern HCONTROL    ghMsgLine1, ghMsgLine2, ghState;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

LOCAL WORD  ft_Seq;
LOCAL DWORD ft_nRestData;
LOCAL IREX_FT_FILEINF_TYPE_T		irex_FileInf;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

void EQS_API sSendErrorMessage(IREX_FTRSPCODE_TYPE_E ErrorCode);
void EQS_API sSendInfRspMessage(void);
void EQS_API sSendBlockRspMessage(BOOL fAck);
void EQS_API sSendFTWaitMessage(void);
void EQS_API sSendFinishMessage(void);

BOOL EQS_API sCheckFileServerModel(IREX_FILETYPE_E FileType);

BOOL EQS_API sCheckFileClientModel(IREX_FILETYPE_E FileType);
void EQS_API sSendFileInf(IREX_FILETYPE_E FileType, BYTE *pFileName);
void EQS_API sSendFileBlock(void);

/*******************************************************************************

DESCRIPTION
	This function processes the FILE TRANSFER messages. This function is called
	by CLIENT.

INPUT

OUTPUT
	NONE

RETURN
	RESULT CODE
		IREX_OK_RES					0x0001
		IREX_FINISHED_RES			0x000f
		IREX_WAIT_FOR_USER_RES		0x0010

*******************************************************************************/
IREX_RESULT_E SKY_API SKY_ProcessFileTransferMsg(void)
{
    static FILE_RX_STATE_E          FTState = FT_NONE_C;
    static BOOL                     fIREX_FT_PHOTO = FALSE;
    PHOTO_LIST_T                    l_PhotoList[MAX_PHOTO_COUNT];
	DWORD							AvailMem;
	IREX_RESULT_E					RetVal = IREX_OK_RES;
	WORD							nFiles;
//	IREX_FTRSPCODE_TYPE_E			ErrorCode = IREX_FT_ERR;
    BYTE                            Str[SKY_FILE_EXT_NAME_MAX_LENGTH];
    DWORD                           dwCount;
    HFILE                           hFile;
    UINT                            i=0;
    BYTE                            s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1];
    DWORD                           dwTimeInfo;
    BITMAP                          BitmapCameraInfo;

	if(gIrex_RxMsg.Msg.FileTrans_s.Type == IREX_FT_FILE_INF)
		FTState = FT_GOT_INF_C;

	switch(FTState)
	{
	case FT_GOT_INF_C:
		if(gIrex_RxMsg.Msg.FileTrans_s.Type != IREX_FT_FILE_INF)    // Send Ping
        {
			gIrex_TxMsg.MsgType = IREX_PING_C;
			gIrex_ModelIndex = sGetServerModel(irex_RxedModelID, irex_SendingModelID);
			STRCPY((BYTE*)&gIrex_TxMsg.Msg.Ping_c.Model, irex_SendingModelID);
			sSendMessage(&gIrex_TxMsg);
            break;
        }

        SKY_IrDACloseFile();

		ft_Seq = gIrex_RxMsg.Msg.FileTrans_s.Seq;
		irex_FileInf = gIrex_RxMsg.Msg.FileTrans_s.Body.Inf;
		irex_FileInf.FileName[SKY_FILE_EXT_NAME_MAX_LENGTH] = '\0';

        STRCPY(g_ReceiveFileName, (void *)irex_FileInf.FileName);

        // Check Client model
        if(!sCheckFileServerModel((IREX_FILETYPE_E)irex_FileInf.Type))
        {
            if(irex_FileInf.Type == IREX_FT_MELODY)
                RetVal = IREX_INVAILD_MELODY_RES;
            else if(irex_FileInf.Type == IREX_FT_ANI)
                RetVal = IREX_INVAILD_ANI_RES;
            else
                RetVal = IREX_ERR_FILE_VER;

            sSendErrorMessage(IREX_FT_ERR_VER);
            break;
        }

		// Check avalable file number & file exist
		switch(irex_FileInf.Type)
		{
		case IREX_FT_MELODY:
			if(SKY_IsDataFile(SDF_MELODY, (void *)irex_FileInf.FileName))
			{
                sSendFTWaitMessage();
				return IREX_WAIT_FOR_USER_RES;
			}
            EQC_SetStaticText(ghState, (BYTE*)SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_STATE_BELL));
			nFiles = SKY_GetFileNumberInDataFile(SDF_MELODY);
            gIrDACommType = IRT_MELODY;
			break;
		case IREX_FT_ANI:
			if(SKY_IsDataFile(SDF_IMAGE, (void *)irex_FileInf.FileName))
			{
                sSendFTWaitMessage();
				return IREX_WAIT_FOR_USER_RES;
			}
            EQC_SetStaticText(ghState, (BYTE*)SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_STATE_ANI));
			nFiles = SKY_GetFileNumberInDataFile(SDF_IMAGE);
            gIrDACommType = IRT_IMAGE;
			break;
		case IREX_FT_PHOTO:
            EQC_SetStaticText(ghState, (BYTE*)SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_STATE_PHOTO));
			nFiles = SKY_GetFileNumberInDataFile(SDF_PHOTO);
            gIrDACommType = IRT_PHOTO;
			break;
        default:
            nFiles = 0;
            break;
		}

		if((nFiles == 0xFFFF) || (nFiles >= SKY_MAX_NUM_FILES))
		{
            RetVal = IREX_ERR_FULE_FILE;
            sSendErrorMessage(IREX_FT_ERR_FILE_NUM);
			break;
		}

		// Check available memory
   		AvailMem = SKY_GetFreeSpaceInDataFile();
		if(irex_FileInf.FileSize > AvailMem )
		{
            RetVal = IREX_NO_MEM_AVAIL;
            sSendErrorMessage(IREX_FT_ERR_NO_MEM);
			break;
		}

		switch(irex_FileInf.Type)
		{
		case IREX_FT_MELODY:
            SKY_CreateDataFile(SDF_MELODY,
                               (void *)irex_FileInf.FileName,
                               &gRecvFile);
			break;

		case IREX_FT_ANI:
            SKY_CreateDataFile(SDF_IMAGE,
                               (void *)irex_FileInf.FileName,
                               &gRecvFile);
			break;

		case IREX_FT_PHOTO:
            dwCount = SKY_GET_CAMERA_COUNT();

            ConvertCameraImageFileName((void *)irex_FileInf.FileName, dwCount);
            gRecvFile = SKY_MakeCameraImageFile(dwCount);
            fIREX_FT_PHOTO =TRUE;

			break;
		}

#ifdef IRDA_FILE_BUFFER
        g_nBufferOffset = 0;
#endif        
        ft_Seq = gIrex_RxMsg.Msg.FileTrans_s.Seq;
        sSendInfRspMessage();
        FTState = FT_BLOCK_WAIT_C;

        /*
        //file name
        STRCPY(Str, g_ReceiveFileName);
        MakeShortString(Str, 16);
		EQC_SetStaticText(ghMsgLine1, Str);
        */
		break;

	case FT_BLOCK_WAIT_C:
		if(gIrex_RxMsg.Msg.FileTrans_s.Type == IREX_FT_BLOCK)
		{
			// Already received block
			if(ft_Seq == gIrex_RxMsg.Msg.FileTrans_s.Seq)
			{
                sSendBlockRspMessage(TRUE);
            }
			else if(ft_Seq + 1 != gIrex_RxMsg.Msg.FileTrans_s.Seq)
			{
                sSendBlockRspMessage(FALSE);
			}
			else
			{
                STRCPY(Str, g_ReceiveFileName);
                MakeShortString(Str, 16);
				EQC_SetStaticText(ghMsgLine1, Str); // Line1 display

				sprintf((char*)Str, "(%3d%% %s)", 
					(word)((float)(ft_Seq+1) / (float)irex_FileInf.nBlocks * 100),
                   (BYTE *)(SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_RECEIVED)));
                EQC_SetStaticText(ghMsgLine2, Str); // Line2 display

#ifdef IRDA_FILE_BUFFER
                if(g_nBufferOffset + IREX_MAX_BLOCK_SIZE >= IRDA_FILE_BUFFER_SIZE)
                {
                    /* rosa 2001-12-12 --> 수신중에 빠져나갔다가 이전 수신데이터를 이어받는 중이면 파일에 저장안함 */
                    if(gRecvFile != NULL)
                    {
                        SKY_WriteDataBlock(gRecvFile, 
                                           (CONST void *)g_RecvFileBuffer,
                                           g_nBufferOffset);
                    }
                    /* <-- */

                    memcpy(&g_RecvFileBuffer[0],
                            (void *)gIrex_RxMsg.Msg.FileTrans_s.Body.Block, IREX_MAX_BLOCK_SIZE);
            
                    g_nBufferOffset = IREX_MAX_BLOCK_SIZE;
                    
                }
                else
                {
                    memcpy(&g_RecvFileBuffer[g_nBufferOffset],
                            (void *)gIrex_RxMsg.Msg.FileTrans_s.Body.Block, IREX_MAX_BLOCK_SIZE);
            
                    g_nBufferOffset += IREX_MAX_BLOCK_SIZE;
                }
#else
                /* rosa 2001-12-12 --> 수신중에 빠져나갔다가 이전 수신데이터를 이어받는 중이면 파일에 저장안함 */
                if(gRecvFile != NULL)
                {
                    SKY_WriteDataBlock(gRecvFile, 
                                       (CONST void *)gIrex_RxMsg.Msg.FileTrans_s.Body.Block,
                                       IREX_MAX_BLOCK_SIZE) ;
                }
                /* <-- */
#endif

				ft_Seq = gIrex_RxMsg.Msg.FileTrans_s.Seq;

                sSendBlockRspMessage(TRUE);
			}
            FTState = FT_BLOCK_WAIT_C;
		}
		else if(gIrex_RxMsg.Msg.FileTrans_s.Type == IREX_FT_LAST_BLOCK)
		{
			// Already received block
			if(ft_Seq == gIrex_RxMsg.Msg.FileTrans_s.Seq)
			{
                sSendBlockRspMessage(TRUE);
                FTState = FT_BLOCK_WAIT_C;
            }
			if(ft_Seq + 1 != gIrex_RxMsg.Msg.FileTrans_s.Seq)
			{
                sSendBlockRspMessage(FALSE);
                FTState = FT_BLOCK_WAIT_C;
			}
			else
			{
                /* rosa 2001-12-12 --> 수신중에 빠져나갔다가 이전 수신데이터를 이어받는 중이면 파일에 저장안함 */
                if(gRecvFile != NULL)
                {
#ifdef IRDA_FILE_BUFFER           
                    SKY_WriteDataBlock(gRecvFile, 
                                       (CONST void *)g_RecvFileBuffer,
                                       g_nBufferOffset);

                    SKY_WriteDataBlock(gRecvFile, 
                                       (CONST void *)gIrex_RxMsg.Msg.FileTrans_s.Body.Block,
                                       MIN(IREX_MAX_BLOCK_SIZE, gIrex_RxMsg.Msg.FileTrans_s.nRest));
#else
                    SKY_WriteDataBlock(gRecvFile, 
                                       (CONST void *)gIrex_RxMsg.Msg.FileTrans_s.Body.Block,
                                       MIN(IREX_MAX_BLOCK_SIZE, gIrex_RxMsg.Msg.FileTrans_s.nRest)) ;
#endif
                    if(fIREX_FT_PHOTO)
                    {   
                        SKY_SeekFile(gRecvFile, FS_SO_SET, 0);
                        SKY_ReadDataBlock(gRecvFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
               
                        if (gIrex_ModelIndex == IREX_MODEL_B1)
                        {
                            SKY_ReadDataBlock(gRecvFile, &dwTimeInfo, sizeof(DWORD));
                            SKY_ReadDataBlock(gRecvFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header)); 
                            SKY_ReadDataBlock(gRecvFile, &g_abBmpBuf[i],PHOTO_TOTAL_SIZE/2+120*15);

                            BitmapCameraInfo.Header.Size.yHeight = 90;

                            SKY_SeekFile(gRecvFile, FS_SO_SET, 0);
                            SKY_WriteDataBlock(gRecvFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                            SKY_WriteDataBlock(gRecvFile, &dwTimeInfo, sizeof(DWORD));
                            SKY_WriteDataBlock(gRecvFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));
                            SKY_WriteDataBlock(gRecvFile, &g_abBmpBuf[120*15],PHOTO_TOTAL_SIZE/2); 
                        }
                   
                                     
                        // Change List
                        hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
                        SKY_ReadFile(hFile, (void *)l_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
        
                        for(i=0 ; i<MAX_PHOTO_COUNT;i++)
                        { 
                            if(l_PhotoList[i].PhotoList == NULL)
                            {
                                l_PhotoList[i].PhotoList = SKY_GET_CAMERA_COUNT()-1;
                                STRNCPY(l_PhotoList[i].PhotoName, s_psFileName,SKY_MAX_CAMERA_IMAGE_FILE_LEN);

                                i = MAX_PHOTO_COUNT;
                            }
                        }   
                        SKY_SeekFile(hFile, FS_SO_SET, 0);
                        SKY_WriteFile(hFile, (CONST void *)l_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                        SKY_CloseFile(hFile);

                        fIREX_FT_PHOTO = FALSE;
                    }

                    SKY_CloseDataFile(gRecvFile);
                    gRecvFile = NULL;
                } //if(gRecvFile != NULL)
                /* <-- */

                ft_Seq = gIrex_RxMsg.Msg.FileTrans_s.Seq;
				sSendFinishMessage();
                RetVal = IREX_FILE_TRANS_FINISHED;
                FTState = FT_GOT_INF_C;

                EQC_SetStaticText(ghMsgLine2, (BYTE *)SKY_GET_TOKEN(TKN_IRDA_MSG_RCV_RECEIVE_FILE));
                EQC_SetStaticText(ghState, (BYTE *)SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_STATE_WAIT));
                gIrDACommType = IRT_READY;

			}
		}
		else
		{
			//MSG_ERROR("IrDA error", 0,0,0);
		}
		break;

	}

	return RetVal;
}

/*******************************************************************************

DESCRIPTION
	This function processes the FILE TRANSFER messages. This function is called
	by SERVER

INPUT
	ui_FileTransFileType :
		#define MM_MEL_DIR_ID		0
		#define	MM_ANI_DIR_ID		1
	ui_UserMenuIndex : The selected user file to send
	ui_pUserMenuText

OUTPUT
	ui_irex_bFileTxFinished

RETURN

*******************************************************************************/
IREX_RESULT_E SKY_API SKY_ProcessFileSend(IREX_STATE_E Ret, BOOL fStart)
{
    static FILE_TX_STATE_E  FTState = FT_NONE_S;
    WORD  ResultCode;
    IREX_RESULT_E   ReturnValue;

    ReturnValue = IREX_NONE_RES;

    if(fStart)
        FTState = FT_MODEL_WAIT_S;

	switch(FTState)
	{
	case FT_MODEL_REQ_S:
		SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);
		FTState = FT_MODEL_WAIT_S;
		break;

	case FT_MODEL_WAIT_S:
		if(Ret == IREX_PING_FINISHED)
		{
			if(!sCheckFileClientModel(gTransferType))
			{
                return IREX_ERR_FILE_VER;
			}
			
            sSendFileInf(gTransferType, g_pSendFileName);
			FTState = FT_INF_RSP_WAIT_S;
		}
		else if(Ret == IREX_SUCCESS)
		{
			SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_USE_PING_S_ATT, &ResultCode);
			FTState = FT_MODEL_WAIT_S;
		}
		break;

	case FT_INF_RSP_WAIT_S:
		if(Ret == IREX_PING_FINISHED)
		{
            sSendFileInf(gTransferType, g_pSendFileName);
			FTState = FT_INF_RSP_WAIT_S;
		}
		else if(Ret == IREX_SUCCESS)
		{
			// What response code we got ?
			switch(gIrex_RxMsg.Msg.FileTrans_c.RspCode)
			{
			case IREX_FT_NAK:
                sSendFileInf(gTransferType, g_pSendFileName);
				FTState = FT_INF_RSP_WAIT_S;
				break;
			case IREX_FT_ACK:
				sSendFileBlock();
				FTState = FT_BLOCK_RSP_WAIT_S;
				break;
			case IREX_FT_WAIT:
				SKY_IrDAServerStateMachine(WAIT_RESPONSE_E, 0, &ResultCode);
				break;

			case IREX_FT_ESC:
                SKY_IrDACloseFile();
                ReturnValue = IREX_FINISHED_RES;
                FTState = FT_NONE_S;
				break;

			case IREX_FT_FIN:
                SKY_IrDACloseFile();
                ReturnValue = IREX_FINISHED_RES;
				FTState = FT_NONE_S;
				break;

			case IREX_FT_ERR:
                SKY_IrDACloseFile();
                ReturnValue = IREX_ERR_CLIENT; // 수신측에서 파일을 받을수 없습니다
                FTState = FT_NONE_S;
				break;

			case IREX_FT_ERR_NO_MEM:
                SKY_IrDACloseFile();
                ReturnValue = IREX_NO_MEM_AVAIL; // 수신측에 메모리가 부족합니다
				FTState = FT_NONE_S;
				break;

			case IREX_FT_ERR_FILE_NUM:
                SKY_IrDACloseFile();
                ReturnValue = IREX_ERR_FULE_FILE;   // 수신측에 저장된 파일이 너무 많습니다
				FTState = FT_NONE_S;
				break;

			case IREX_FT_ERR_VER:
                SKY_IrDACloseFile();
                ReturnValue = IREX_ERR_FILE_VER;   // 수신측에서 전송하려는파일을 지원하지않습니다
				FTState = FT_NONE_S;
				break;
			}
		}
		break;

	case FT_BLOCK_RSP_WAIT_S:
		if(Ret == IREX_PING_FINISHED)
		{
            sSendFileInf(gTransferType, g_pSendFileName);
			FTState = FT_INF_RSP_WAIT_S;
		}
		else if(Ret == IREX_SUCCESS)
		{
			switch(gIrex_RxMsg.Msg.FileTrans_c.RspCode)
			{
				case IREX_FT_NAK:
				case IREX_FT_ACK:
				    sSendFileBlock();
				    FTState = FT_BLOCK_RSP_WAIT_S;
					break;

				case IREX_FT_FIN:
                    ReturnValue = IREX_FINISHED_RES;
                    SKY_IrDACloseFile();
					FTState = FT_NONE_S;
					break;

				case IREX_FT_ERR:
				case IREX_FT_ESC:
				case IREX_FT_ERR_NO_MEM:
				case IREX_FT_ERR_FILE_NUM:
				case IREX_FT_ERR_VER:
                    ReturnValue = IREX_ERR_CLIENT;
                    SKY_IrDACloseFile();
					FTState = FT_NONE_S;
					break;
			}
		}
		break;
	}

    dog_report(DOG_PS_RPT | DOG_DS_RPT | DOG_SLEEP_RPT);
    SKY_KickDog();

    return ReturnValue;
}

void SKY_API SKY_IrDACloseFile(void)
{
    if(gSendFile)
    {
	    SKY_CloseDataFile(gSendFile);
        gSendFile = NULL;
    }

    if(gRecvFile)   // IREX_FT_LAST_BLOCK 을 받지않고 종료하는 경우 File Delete
    {
	    SKY_CloseDataFile(gRecvFile);

        switch(gIrDACommType){
        case IRT_MELODY:
            SKY_DeleteDataFile(SDF_MELODY, (void *)irex_FileInf.FileName);
            break;
        case IRT_IMAGE:
            SKY_DeleteDataFile(SDF_IMAGE, (void *)irex_FileInf.FileName);
            break;
        case IRT_PHOTO:
            SKY_DeleteDataFile(SDF_PHOTO, (void *)irex_FileInf.FileName);
            break;
        }
        gRecvFile = NULL;
    }
}

void SKY_API SKY_IrDASendESCtoServer(void)
{
	gIrex_TxMsg.MsgType = IREX_FILE_TRANS_C;
	gIrex_TxMsg.Msg.FileTrans_c.RspCode = IREX_FT_ESC;
	gIrex_TxMsg.Msg.FileTrans_c.SeqRsp = ft_Seq;
	SKY_IrDAClientStateMachine(SEND_MSG_E);
}

/****************************************************************
* Send File Transfer Response Message   
*****************************************************************/

void EQS_API sSendErrorMessage(IREX_FTRSPCODE_TYPE_E ErrorCode)
{
	gIrex_TxMsg.MsgType = IREX_FILE_TRANS_C;
	gIrex_TxMsg.Msg.FileTrans_c.RspCode = ErrorCode;
	gIrex_TxMsg.Msg.FileTrans_c.SeqRsp = ft_Seq;
	SKY_IrDAClientStateMachine(SEND_MSG_E);
}

void EQS_API sSendInfRspMessage(void)
{
	gIrex_TxMsg.MsgType = IREX_FILE_TRANS_C;
	gIrex_TxMsg.Msg.FileTrans_c.RspCode = IREX_FT_ACK;
	gIrex_TxMsg.Msg.FileTrans_c.SeqRsp = ft_Seq;
	SKY_IrDAClientStateMachine(SEND_MSG_E);
}

void EQS_API sSendBlockRspMessage(BOOL fAck)
{
    if(fAck)
        gIrex_TxMsg.Msg.FileTrans_c.RspCode = IREX_FT_ACK;
    else
        gIrex_TxMsg.Msg.FileTrans_c.RspCode = IREX_FT_NAK;

    gIrex_TxMsg.MsgType = IREX_FILE_TRANS_C;
	gIrex_TxMsg.Msg.FileTrans_c.SeqRsp = ft_Seq;
	SKY_IrDAClientStateMachine(SEND_MSG_E);
}

void EQS_API sSendFTWaitMessage(void)
{
	gIrex_TxMsg.MsgType = IREX_FILE_TRANS_C;
	gIrex_TxMsg.Msg.FileTrans_c.RspCode = IREX_FT_WAIT;
	gIrex_TxMsg.Msg.FileTrans_c.SeqRsp = ft_Seq;
	SKY_IrDAClientStateMachine(SEND_MSG_E);
}

void EQS_API sSendFinishMessage(void)
{
	gIrex_TxMsg.MsgType = IREX_FILE_TRANS_C;
	gIrex_TxMsg.Msg.FileTrans_c.RspCode = IREX_FT_FIN;
	gIrex_TxMsg.Msg.FileTrans_c.SeqRsp = ft_Seq;
	SKY_IrDAClientStateMachine(SEND_MSG_E);
}


BOOL EQS_API sCheckFileServerModel(IREX_FILETYPE_E FileType)
{
    if(FileType == IREX_FT_MELODY)
    {
        return afReceiveMelody[gIrex_ModelIndex];
    }
    else if(FileType == IREX_FT_ANI)
    {
        return afReceiveAni[gIrex_ModelIndex];
    }
    else if(FileType == IREX_FT_PHOTO)
    {
        /* rosa 2001-09-19 --> @why, what */
        return afReceivePhoto[gIrex_ModelIndex];
        /* <-- */
    }

    return FALSE;
}

BOOL EQS_API sCheckFileClientModel(IREX_FILETYPE_E FileType)
{
    if(FileType == IREX_FT_MELODY)
    {
        return afSendMelody[gIrex_ModelIndex];
    }
    else if(FileType == IREX_FT_ANI)
    {
        return afSendAni[gIrex_ModelIndex];
    }
    else if(FileType == IREX_FT_PHOTO)
    {
        /* rosa 2001-09-19 --> @why, what */
        return afSendPhoto[gIrex_ModelIndex];
        /* <-- */
    }
    
    return FALSE;
}

void EQS_API sSendFileInf(IREX_FILETYPE_E FileType, BYTE *pFileName)
{
	word ResultCode;

    SKY_IrDACloseFile();

    ft_Seq = 0;

	gIrex_TxMsg.MsgType = IREX_FILE_TRANS_S;

	switch(FileType)
	{
	case IREX_FT_MELODY:
		gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.Type = IREX_FT_MELODY;
		ft_nRestData = gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.FileSize
			= SKY_GetDataFileSize(SDF_MELODY, pFileName);

        SKY_OpenDataFile(SDF_MELODY, pFileName, &gSendFile, SFT_READ);
		break;

	case IREX_FT_ANI:
		gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.Type = IREX_FT_ANI;
		ft_nRestData = gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.FileSize
			= SKY_GetDataFileSize(SDF_IMAGE, pFileName);

        SKY_OpenDataFile(SDF_IMAGE, pFileName, &gSendFile, SFT_READ);
		break;

	case IREX_FT_PHOTO:
		gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.Type = IREX_FT_PHOTO;
		ft_nRestData = gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.FileSize
			= SKY_GetDataFileSize(SDF_PHOTO, pFileName);

        SKY_OpenDataFile(SDF_PHOTO, pFileName, &gSendFile, SFT_READ);
		break;
	}

#ifdef IRDA_FILE_BUFFER
    if(ft_nRestData > IRDA_FILE_BUFFER_SIZE)
    {
        SKY_ReadDataBlock(gSendFile, g_RecvFileBuffer, IRDA_FILE_BUFFER_SIZE);
        g_nFileOffset = IRDA_FILE_BUFFER_SIZE;
    }
    else
    {
        SKY_ReadDataBlock(gSendFile, g_RecvFileBuffer, ft_nRestData);
        g_nFileOffset = ft_nRestData;
    }

    g_nBufferOffset = 0;
#endif

	// Set file information type
	gIrex_TxMsg.Msg.FileTrans_s.Type = IREX_FT_FILE_INF;
	gIrex_TxMsg.Msg.FileTrans_s.Seq = 0;
	gIrex_TxMsg.Msg.FileTrans_s.nRest = sizeof(IREX_FT_FILEINF_TYPE_T);

    if(FileType == IREX_FT_PHOTO)
	    STRCPY((void *)gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.FileName,
		       g_pSendPhotoName);
    else
	    STRCPY((void *)gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.FileName,
		       pFileName);

	// Set block size
	gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.BlockSize
		= IREX_MAX_BLOCK_SIZE;

	// Set number of blocks
	gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.nBlocks
		= gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.FileSize / IREX_MAX_BLOCK_SIZE;
	if(gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.FileSize % IREX_MAX_BLOCK_SIZE)
		gIrex_TxMsg.Msg.FileTrans_s.Body.Inf.nBlocks++;

	// Send ft_pMsg
	SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
}

void EQS_API sSendFileBlock(void)
{
	WORD	ResultCode;
	WORD	nRest;

	switch(gIrex_RxMsg.Msg.FileTrans_c.RspCode)
	{
	case IREX_FT_NAK:
        // Edited by chul at 2000.4.26
        // Even though got NAK, if client sends the expected sequence number, it's OK.
        // When just wrong seq number is got by client, the client sends IREX_FT_NAK
		//SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
		//break;

	case IREX_FT_ACK:
		if(gIrex_RxMsg.Msg.FileTrans_c.SeqRsp != ft_Seq)
		{
			// Retransmition the last message
			SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
		}
		else
		{
			// Send the next block
			ft_Seq++;

			// Set file information type
			gIrex_TxMsg.MsgType = IREX_FILE_TRANS_S;
			gIrex_TxMsg.Msg.FileTrans_s.Seq = ft_Seq;
			if(ft_nRestData >= IREX_MAX_BLOCK_SIZE)
			{
				nRest = IREX_MAX_BLOCK_SIZE;
				ft_nRestData -= IREX_MAX_BLOCK_SIZE;
				gIrex_TxMsg.Msg.FileTrans_s.Type = IREX_FT_BLOCK;
			}
			else
			{
				// Send the last file blok
				nRest = ft_nRestData;
				ft_nRestData = 0;
				gIrex_TxMsg.Msg.FileTrans_s.Type = IREX_FT_LAST_BLOCK;
			}

			gIrex_TxMsg.Msg.FileTrans_s.nRest = nRest;

#ifdef IRDA_FILE_BUFFER
            if(g_nBufferOffset + nRest <= IRDA_FILE_BUFFER_SIZE)
            {
                memcpy((void *)gIrex_TxMsg.Msg.FileTrans_s.Body.Block, &g_RecvFileBuffer[g_nBufferOffset], nRest);
                g_nBufferOffset += nRest;
            }
            else
            {
                if(ft_nRestData + nRest < IRDA_FILE_BUFFER_SIZE)
                    SKY_ReadDataBlock(gSendFile, g_RecvFileBuffer, ft_nRestData + nRest);
                else
                    SKY_ReadDataBlock(gSendFile, g_RecvFileBuffer, IRDA_FILE_BUFFER_SIZE);

                memcpy((void *)gIrex_TxMsg.Msg.FileTrans_s.Body.Block, &g_RecvFileBuffer[0], nRest);
                g_nBufferOffset = nRest;
            }
#else
            SKY_ReadDataBlock(gSendFile, gIrex_TxMsg.Msg.FileTrans_s.Body.Block, nRest);
#endif

            SKY_IrDAServerStateMachine(SEND_MSG_E, IREX_SKIP_PING_S_ATT, &ResultCode);
		}
		break;

	// File transfer finished
	case IREX_FT_FIN:
		break;

	case IREX_FT_ERR:
		break;

	case IREX_FT_ESC:
		break;
	}
}
