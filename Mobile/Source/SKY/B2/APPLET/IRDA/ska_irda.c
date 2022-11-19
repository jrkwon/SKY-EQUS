/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_irda.c
**
** Description: 
**     Common IrDA MMI Functions
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-23 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

#if MODEL_ID == MODEL_B0
    #include "IrDA_B0.tbl"
#elif (MODEL_ID == MODEL_B1)
    #include "IrDA_B1.tbl"
#elif (MODEL_ID == MODEL_B2)
    #include "IrDA_B2.tbl"
#endif

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

extern HCONTROL ghState;
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

enum tag_IREX_SERVER_STATE_TYPE_E {
	NONE_S,
	SEND_PING_S,
	WAIT_PING_RES_S,
	SEND_MSG_S,
	WAIT_MSG_RES_S,
	WAIT_RESPONSE_S,
	END_S
};

typedef enum tag_IREX_SERVER_STATE_TYPE_E IREX_SERVER_STATE_TYPE_E;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
extern boolean		IrDA_Check_Rx_Buffer ( void );
extern byte		IrDA_GetOneByte ( void );
extern WORD crc_16_calc(BYTE *buf_ptr, WORD len);
extern DWORD crc_30_calc(BYTE *buf_ptr, WORD len);
extern int		IrCOMM_SendData( int lsap, const void *buf, UINT16 bufsz ) ;

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

IREX_MSG_TYPE_T	gIrex_RxMsg, gIrex_TxMsg;

IREX_RECEIVE_TYPE_E	gIrDACommType;
BOOL	gfPhoneBookOverwriteAll = FALSE;
BOOL	gfPhoneBookSkipAll = FALSE;


IREX_FILETYPE_E gTransferType;
HFILE           gSendFile = NULL;
HFILE           gRecvFile = NULL;
BYTE            *g_pSendFileName;
BYTE            *g_pSendPhotoName;
BYTE            g_ReceiveFileName[SKY_FILE_EXT_NAME_MAX_LENGTH];

BYTE	        g_GroupName[MAX_GROUP_NAME+1];
BYTE	        g_Letters[MAX_BOOK_NAME+1];

#ifdef IRDA_FILE_BUFFER
BYTE            g_RecvFileBuffer[IRDA_FILE_BUFFER_SIZE];   // 10KB
UINT16          g_nBufferOffset;
UINT16          g_nFileOffset;
#endif

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

LOCAL BYTE  irex_SendBuf[MAX_PACKET_LENGTH];
LOCAL BYTE  irex_RxBuf[MAX_PACKET_LENGTH];

LOCAL BYTE  sndSymIndex;
LOCAL BYTE  rcvSymIndex;

LOCAL BYTE  l_nSendRetries = 0;

const BYTE irex_ProtocolVersions[IREX_MAX_MODELS][MODEL_ID_LEN+1] =
				{ 
					"SKTT0105",		// IM1000, IM1200
					"SK010601",		// IM1100 
					"SK010602",		// IM1100 WAP
					"SK010502",		// IM1200N
					"SK010901",		// IM2000
					"SK010001",		// A8
					"SK011501",		// B0
                    "SK011601",     // B1
                    /* rosa 2001-09-19 --> @why, what */
                    "SK011801",     // B2
                    /* <-- */
				};

BYTE   irex_SendingModelID[MODEL_ID_LEN+1] = IREX_MODEL_ID;
BYTE   irex_RxedModelID[MODEL_ID_LEN+1] = IREX_MODEL_ID;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

INT16 SKY_API sExtractMessage(BYTE *pMsgSymbol, IREX_MSG_TYPE_T *pRxMsg);
IREX_RESULT_GETMSG_E SKY_API sGetMessage(void);
BOOL SKY_API sSendMessage(IREX_MSG_TYPE_T *pTxMsg);
UINT16 SKY_API sBuildPacket(IREX_MSG_TYPE_T *pTxMsg, BYTE *pPacketData);

IREX_RESULT_E SKY_API sProcessMessage(IREX_MSG_TYPE_T *pRxMsg, IREX_MSG_TYPE_T *pTxMsg);

IREX_MODEL_E SKY_API sGetServerModel(BYTE *pRxedModel, BYTE *pSendingModel);
IREX_MODEL_E SKY_API sGetClientModel(BYTE *pRxedModel);

WORD SKY_API sGetMessageLength(WORD MsgType);
WORD SKY_API sGetC5MessageLength(word MsgType);
WORD SKY_API sGetB5MessageLength(word MsgType);
WORD SKY_API sGetB5WAPMessageLength(word MsgType);
WORD SKY_API sGetC5WAPMessageLength(word MsgType);
WORD SKY_API sGetA7MessageLength(word MsgType);
WORD SKY_API sGetB1MessageLength(word MsgType);
/* rosa 2001-09-19 --> @why, what */
WORD SKY_API sGetB2MessageLength(word MsgType);
/* <-- */

WORD SKY_API sGetWord(BYTE *pSymbols);
void SKY_API sGetWordSymbol(WORD Word,BYTE *pSymbols);
BYTE SKY_API sGetSymIndex(BYTE* symIndex, BYTE add);
BYTE SKY_API sGetByte(BYTE *pSymbols);
void SKY_API sGetByteSymbol(BYTE Byte,BYTE *pSymbols);
INT16 SKY_API sGetStream(BYTE *pSymbols, UINT16 nSymbols, BYTE *Bytes);
void SKY_API sGetStreamSymbol(BYTE *Bytes, UINT16 nBytes, BYTE *pSymbols);

BOOL SKY_API sServerSendPing(void);
BOOL SKY_API sServerSendMessage(void);
WORD sGetResultCode(IREX_MSG_TYPE_T *pRxMsg);
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKY_IrDAClientStateMachine()
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

IREX_RESULT_E SKY_API SKY_IrDAClientStateMachine(IREX_EVENT_E Event)
{
	IREX_RESULT_E	Ret;
    UINT16          OldType;
	IREX_RESULT_GETMSG_E	Result;

    Ret = IREX_NONE_RES;

    switch(Event)
    {
    case TIMEOUT_E:
    case SEND_MSG_E:
		if(!sSendMessage(&gIrex_TxMsg))
			EQS_StartTimer(EQS_TIMER_1, IREX_SEND_AGAIN_TIME, FALSE);

        if(gIrex_TxMsg.MsgType == IREX_PHONEBOOK_C ||gIrex_TxMsg.MsgType == IREX_FINISHED_C)
            EQS_StartTimer(EQS_TIMER_2, IREX_BOOK_RESP_RETRY_TIME, FALSE);
        break;

    case GET_MSG_E:
		Result = sGetMessage();
		// Packet not yet made
		if( Result != IREX_MSG_COMPLETE)
		{
			break;
		}

        EQS_StopTimer(EQS_TIMER_1);
		// CRC error
		if( Result == IREX_CRE_ERROR)
		{
			OldType = gIrex_TxMsg.MsgType;
			gIrex_TxMsg.MsgType = IREX_CRC_ERROR_C;
			sSendMessage(&gIrex_TxMsg);
			EQS_StartTimer(EQS_TIMER_1, IREX_MSG_WAIT_TIME, FALSE);
			gIrex_TxMsg.MsgType = OldType;
			break;
		}
        
		if(gIrex_RxMsg.MsgType == IREX_PING_S)
		{
			sProcessMessage(&gIrex_RxMsg, &gIrex_TxMsg);
			// At the first phone book item received, reset overwirte all flag
			gfPhoneBookOverwriteAll = FALSE;
            gfPhoneBookSkipAll = FALSE;
            STRCPY(g_ReceiveFileName, (BYTE*)"");

			gIrex_TxMsg.MsgType = IREX_PING_C;
			gIrex_ModelIndex = sGetServerModel(irex_RxedModelID, irex_SendingModelID);
			STRCPY((BYTE*)&gIrex_TxMsg.Msg.Ping_c.Model, irex_SendingModelID);
			sSendMessage(&gIrex_TxMsg);
		}
		// Data receipt is completed
		else if(gIrex_RxMsg.MsgType == IREX_FINISHED_S)
		{
			gIrex_TxMsg.MsgType = IREX_FINISHED_C;
			sSendMessage(&gIrex_TxMsg);

			return IREX_FINISHED_RES;
		}
		else if(gIrex_RxMsg.MsgType == IREX_FILE_TRANS_S)
		{
			Ret = sProcessMessage(&gIrex_RxMsg, &gIrex_TxMsg);
		}
		else if(gIrex_RxMsg.MsgType)
		{
            EQS_StopTimer(EQS_TIMER_2);
			Ret = sProcessMessage(&gIrex_RxMsg, &gIrex_TxMsg);

/*
            // 전화부 전송시 UI Command buffer가 넘치는 현상을 막기 위해 ACK를 늦게 보내줌
            if(gIrex_TxMsg.MsgType == IREX_PHONEBOOK_C ||
               gIrex_TxMsg.MsgType == IREX_PHONEBOOK_SDIAL_C)
            {
                EQS_StartTimer(EQS_TIMER_1, IREX_BOOK_WAIT_RESP_TIME, FALSE);
                if(gIrex_TxMsg.MsgType == IREX_PHONEBOOK_C)
                    EQS_StartTimer(EQS_TIMER_2, IREX_BOOK_RESP_RETRY_TIME, FALSE);
            }
            */
			if(!sSendMessage(&gIrex_TxMsg))
				EQS_StartTimer(EQS_TIMER_1, IREX_SEND_AGAIN_TIME, FALSE);

            if(gIrex_TxMsg.MsgType == IREX_PHONEBOOK_C || gIrex_TxMsg.MsgType == IREX_FINISHED_C)
                EQS_StartTimer(EQS_TIMER_2, IREX_BOOK_RESP_RETRY_TIME, FALSE);

		}

        break;

    default:
        break;
    }

	return Ret;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKY_IrDAServerStateMachine()
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

IREX_STATE_E SKY_API SKY_IrDAServerStateMachine(IREX_EVENT_E Event,
                                                 WORD Attrib,
                                                 WORD *pResultCode)
{
	static IREX_SERVER_STATE_TYPE_E State = SEND_PING_S;
	static	byte	nMsgWaitRetries;

	switch(Event)
	{
	case SEND_MSG_E:
        if(State == WAIT_RESPONSE_S)    break;

		if(Attrib == IREX_SKIP_PING_S_ATT)
			State = SEND_MSG_S;
		else if(Attrib == IREX_USE_PING_S_ATT)
			State = SEND_PING_S;
		break;

	case WAIT_RESPONSE_E:
        EQS_StopTimer(EQS_TIMER_1);
        EQS_StopTimer(EQS_TIMER_2);
		State = WAIT_RESPONSE_S;
		break;

    case GET_MSG_E:
        //EQS_StopTimer(EQS_TIMER_1);
        break;
	}

	nMsgWaitRetries = 0;

	switch(State)
	{
	case SEND_PING_S:
		switch(Event)
		{
		case SEND_MSG_E:
			l_nSendRetries = 0;
		case TIMEOUT_E:
            if(sServerSendPing())
           		State = WAIT_PING_RES_S;
			break;
		}
		break;

	case WAIT_PING_RES_S:
		switch(Event)
		{
		case GET_MSG_E:
			if(sGetMessage() != IREX_MSG_COMPLETE)
				break;

			// Packet made
			if(gIrex_RxMsg.MsgType == IREX_PING_C)
			{
				memcpy(irex_RxedModelID, (void *)gIrex_RxMsg.Msg.Ping_c.Model, MODEL_ID_LEN+1);
				gIrex_ModelIndex = sGetClientModel(irex_RxedModelID);

				if(gIrex_ModelIndex != -1)
				{
					nMsgWaitRetries = 0;
                    State = WAIT_MSG_RES_S;

                    /* rosa 2001-10-06 --> @why, what */
                    if(!sServerSendMessage())
                        return IREX_FAILURE;
                    /* <-- */
				}
                EQS_StopTimer(EQS_TIMER_2);
				return IREX_PING_FINISHED;
			}
			break;

		case TIMEOUT_E:
			nMsgWaitRetries++;
			if(nMsgWaitRetries >= NUM_MSG_RES_WAIT_RETRIES) 
                return IREX_FAILURE;

            if(sServerSendPing())
           		State = WAIT_PING_RES_S;
            else
                State = SEND_PING_S;
			break;
		}
		break;

	case SEND_MSG_S:
		switch(Event)
		{
		case SEND_MSG_E:
			l_nSendRetries = 0;
		case TIMEOUT_E:
            State = WAIT_MSG_RES_S;

            if(!sServerSendMessage())
                return IREX_FAILURE;
			break;
		}
		break;

	case WAIT_MSG_RES_S:
		switch(Event)
		{
		case GET_MSG_E:
			if(sGetMessage() != IREX_MSG_COMPLETE)
				break;

//            EQS_StopTimer(EQS_TIMER_1);

			if(gIrex_RxMsg.MsgType == IREX_CRC_ERROR_C)
			{
                State = WAIT_MSG_RES_S;

                if(!sServerSendMessage())
                    return IREX_FAILURE;
			}
			else if(gIrex_TxMsg.MsgType == 
				(gIrex_RxMsg.MsgType & IREX_SERVER_MASK))
			{
				*pResultCode = sGetResultCode(&gIrex_RxMsg);
		        State = SEND_PING_S;
                EQS_StopTimer(EQS_TIMER_1);
		        return IREX_SUCCESS;
			}
			break;

		case TIMEOUT_E:
			// Go to previous
			nMsgWaitRetries++;
			if(nMsgWaitRetries >= NUM_MSG_RES_WAIT_RETRIES) return IREX_FAILURE;

            State = WAIT_MSG_RES_S;

            if(!sServerSendMessage())
                return IREX_FAILURE;
			break;
		}
		break;

	// 데이터 전송을 계속하라는 메세지가 올때 까지 기다린다
	case WAIT_RESPONSE_S:
		switch(Event)
		{
		case GET_MSG_E:
			if(sGetMessage() != IREX_MSG_COMPLETE)
				break;

            switch(gIrex_RxMsg.MsgType)
			{
			case IREX_PHONEBOOK_C:
            case IREX_PHONEBOOK_SDIAL_C:
				*pResultCode =  sGetResultCode(&gIrex_RxMsg);				
			}
		    State = SEND_PING_S;
            EQS_StopTimer(EQS_TIMER_1);
            EQS_StopTimer(EQS_TIMER_2);
		    return IREX_SUCCESS;
			break;

		case TIMEOUT_E:
			nMsgWaitRetries++;
			if(nMsgWaitRetries >= NUM_MSG_RES_WAIT_RETRIES) 
                return IREX_FAILURE;
			break;
		}
		break;
	}
	return IREX_ON_GOING;
}

BOOL SKY_API sServerSendPing(void)
{
	gIrex_TxMsg.MsgType = IREX_PING_S;
	STRCPY((BYTE*)&gIrex_TxMsg.Msg.Ping_s.Model, (BYTE *)IREX_MODEL_ID);
	if(sSendMessage(&gIrex_TxMsg))
	{
        EQS_StartTimer(EQS_TIMER_2, IREX_PING_WAIT_TIME, FALSE);
        return TRUE;
	}
	else
	{
		l_nSendRetries++;
		if(l_nSendRetries >= NUM_PING_RETRIES) return FALSE;
        EQS_StartTimer(EQS_TIMER_2, IREX_SEND_AGAIN_TIME, FALSE);

        return FALSE;
	}
}

BOOL SKY_API sServerSendMessage(void)
{
	if(sSendMessage(&gIrex_TxMsg))
	{
        if(gIrex_TxMsg.MsgType == IREX_PHONEBOOK_S
            || gIrex_TxMsg.MsgType == IREX_PHONEBOOK_SDIAL_S)
            EQS_StartTimer(EQS_TIMER_1, IREX_BOOK_MSG_WAIT_TIME, FALSE);
        else if(gIrex_TxMsg.MsgType == IREX_FILE_TRANS_S)
            EQS_StartTimer(EQS_TIMER_1, IREX_BOOK_MSG_WAIT_TIME, FALSE);
		else
            EQS_StartTimer(EQS_TIMER_1, IREX_BOOK_MSG_WAIT_TIME, FALSE);
        return TRUE;
	}
	else
	{
		l_nSendRetries++;
		if(l_nSendRetries >= NUM_PING_RETRIES) 
            return FALSE;
        EQS_StartTimer(EQS_TIMER_1, IREX_SEND_AGAIN_TIME, FALSE);
	}

    return FALSE;
}

WORD sGetResultCode(IREX_MSG_TYPE_T *pRxMsg)
{
	switch(pRxMsg->MsgType)
	{
	case IREX_MELODY_C:
		return pRxMsg->Msg.Melody_c.ResultCode;
	case IREX_ANI_C:
		return pRxMsg->Msg.Ani_c.ResultCode;
	case IREX_PHONEBOOK_C:
		return pRxMsg->Msg.PhoneBook_c.ResultCode;
	case IREX_PHONEBOOK_SDIAL_C:
		return pRxMsg->Msg.PhoneBook_SDial_c.ResultCode;
	}
	return IREX_NONE_RES;
}


INT16 SKY_API sExtractMessage(BYTE *pMsgSymbol, IREX_MSG_TYPE_T *pRxMsg)
{
	UINT16	MsgLen;

	MsgLen = sGetMessageLength(pRxMsg->MsgType);
	
	return sGetStream(pMsgSymbol, (UINT16)(MsgLen*2), (byte*)pRxMsg);
}


IREX_RESULT_GETMSG_E SKY_API sGetMessage(void)
{
	static			IREX_PACKET_STATE_TYPE_E	State = START_S;
	static BYTE     *pSymPos, *pSym;
	static WORD		RxChecksum, Checksum;
	DWORD			RxCRC_30, CRC_30;
	static UINT16	NextLen;
	static WORD     MsgSymbolLength, MsgType;
	INT				nData;							 

	while ( IrDA_Check_Rx_Buffer () )
	{
		nData = IrDA_GetOneByte ();

		if(nData == IREX_P_START_BYTE || nData == IREX_P_END_BYTE)
		{
			State = START_S;
		}

		switch(State)
		{
		case START_S:
			if(nData == IREX_P_START_BYTE){
				memset(irex_RxBuf, 0, MAX_PACKET_LENGTH);
				State = LENGTH_S;
				NextLen = 4;
				pSym = pSymPos = irex_RxBuf;

				rcvSymIndex = 0x80;
			}
			break;

		case LENGTH_S:
			*pSym++ = nData;
			NextLen--;
			if(NextLen==0)
			{
				MsgSymbolLength = sGetWord(pSymPos);
				// Check buffer overflow
				if(MsgSymbolLength > MAX_MSG_LENGTH*2)
				{
					State = START_S;
					break;
				}
				NextLen = 4;
				pSymPos = pSym;
				State = MSG_TYPE_S;
			}
			break;

		case MSG_TYPE_S:
			*pSym++ = nData;
			NextLen--;
			if(NextLen==0)
			{
				MsgType = sGetWord(pSymPos);

				NextLen = MsgSymbolLength;
				pSymPos = pSym;
				State = MSG_S;
			}
			break;

		case MSG_S:
			*pSym++ = nData;
			NextLen--;
			if(NextLen==0)
			{
				NextLen = 4;
				pSymPos = pSym;
				State = CHECKSUM_S;
			}
			break;

		case CHECKSUM_S:
			*pSym++ = nData;
			NextLen--;
			if(NextLen==0)
			{
				RxChecksum = sGetWord(pSymPos);
				NextLen = 8;
				pSymPos = pSym;
				State = CRC_S;
			}
			break;

		case CRC_S:
			*pSym++ = nData;
			NextLen--;
			if(NextLen==0)
			{
				State = START_S;

				RxCRC_30 = sGetWord(pSymPos) << 16;
				RxCRC_30 |= sGetWord(pSymPos + 4);

                gIrex_RxMsg.MsgType = MsgType;
				// Find message data for calculating checksum
				if(sExtractMessage(irex_RxBuf+8, &gIrex_RxMsg) == -1)
				{
					return IREX_CRE_ERROR;
				}
				// Check message type
				if(gIrex_RxMsg.MsgType != MsgType)
				{
					return IREX_CRE_ERROR;
				}

				Checksum = crc_16_calc((BYTE*)&gIrex_RxMsg, (WORD)(MsgSymbolLength/2));
				CRC_30 = crc_30_calc((BYTE*)&gIrex_RxMsg, (WORD)(MsgSymbolLength/2));

				if(Checksum != RxChecksum)
				{
					return IREX_CRE_ERROR;
				}
				if(CRC_30 != RxCRC_30)
				{
					return IREX_CRE_ERROR;
				}

				return IREX_MSG_COMPLETE;
			}
			break;
		}
	}

    return IREX_NOT_COMPLETE;
}

BOOL SKY_API sSendMessage(IREX_MSG_TYPE_T *pTxMsg)
{
	UINT16 Length, i;
	
	if(!SKY_CheckLAPConnection()) 
        return FALSE;

	Length = sBuildPacket(pTxMsg, irex_SendBuf);
	i = IrCOMM_SendData( MDM_Slsap,	irex_SendBuf, Length );

    SKY_SNI(HS_SNI_TRANS_IRDA, TRUE);

    EQS_StopTimer(EQS_TIMER_1);
	return TRUE;
}

UINT16 SKY_API sBuildPacket(IREX_MSG_TYPE_T *pTxMsg, BYTE *pPacketData)
{
	WORD	MsgLen;
	WORD	Checksum;
	DWORD	CRC_30;
	BYTE	*pPacket = pPacketData;

	MsgLen = sGetMessageLength(pTxMsg->MsgType);
	Checksum = crc_16_calc((byte*)pTxMsg, MsgLen);
	CRC_30 = crc_30_calc((byte*)pTxMsg, MsgLen);

	// Packet start
	*pPacket++ = 'a';
	*pPacket++ = IREX_P_START_BYTE;

	sndSymIndex = 0x80;

	sGetWordSymbol((WORD)(MsgLen*2), pPacket);
	pPacket += 4;

	// Message Type
	sGetWordSymbol(pTxMsg->MsgType, pPacket);
	pPacket += 4;

	// Message
	sGetStreamSymbol((byte*)pTxMsg, MsgLen, pPacket);
	pPacket += MsgLen*2;
	
	// Make row CRC16
	sGetWordSymbol(Checksum, pPacket);
	pPacket += 4;

	// Make CRC30
	sGetWordSymbol((WORD)(CRC_30 >> 16), pPacket);
	pPacket += 4;
	sGetWordSymbol((WORD)(CRC_30 & 0xFFFF), pPacket);
	pPacket += 4;

	// Add end of packet
	*pPacket = IREX_P_END_BYTE;

	return (MsgLen*2 + 19 + 4);
}

IREX_RESULT_E SKY_API sProcessMessage(IREX_MSG_TYPE_T *pRxMsg, IREX_MSG_TYPE_T *pTxMsg)
{
	IREX_RESULT_E	Ret = IREX_NONE_RES;

	switch(pRxMsg->MsgType)
	{
	case IREX_PING_S:
		Ret = IREX_NONE_RES;
		memcpy(irex_RxedModelID, (void *)gIrex_RxMsg.Msg.Ping_s.Model, MODEL_ID_LEN+1);
		break;

	case IREX_MELODY_S: // C5,B5 Melody Format
        return IREX_INVAILD_MELODY_RES;
		break;

	case IREX_ANI_S:    // C5,B5 Melody Format
        return IREX_INVAILD_ANI_RES;
		break;

	case IREX_PHONEBOOK_S:
        EQS_StopTimer(EQS_TIMER_2);
        if(gIrDACommType != IRT_PHONEBOOK)
            EQC_SetStaticText(ghState, (BYTE*)SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_STATE_BOOK));
		Ret = SKY_ProcessPhoneBookMsg(FALSE);
		pTxMsg->MsgType = pRxMsg->MsgType | IREX_CLIENT_MASK;
		pTxMsg->Msg.PhoneBook_c.ResultCode = Ret;
		gIrDACommType = IRT_PHONEBOOK;
		break;

	case IREX_FILE_TRANS_S:
		Ret = SKY_ProcessFileTransferMsg();
		break;

    case IREX_PHONEBOOK_SDIAL_S:
        if(gIrDACommType != IRT_PHONEBOOK_SDIAL)
            EQC_SetStaticText(ghState, (BYTE*)SKY_GET_TOKEN(TKN_PHONE_TO_PHONE_STATE_BOOK));
		Ret = SKY_ProcessSDialMsg(FALSE);
		pTxMsg->MsgType = pRxMsg->MsgType | IREX_CLIENT_MASK;
		pTxMsg->Msg.PhoneBook_c.ResultCode = Ret;
		gIrDACommType = IRT_PHONEBOOK_SDIAL;
        break;

	default:
		return IREX_NONE_RES;
	}

    dog_report(DOG_PS_RPT | DOG_DS_RPT | DOG_SLEEP_RPT);
    SKY_KickDog();

	return Ret;
}


/*
RETURN 
	Model index of received model ID
*/
IREX_MODEL_E SKY_API sGetServerModel(BYTE *pRxedModel, BYTE *pSendingModel)
{
	INT16 i;
	for(i = IREX_MAX_MODELS-1; i>=0; i--)
	{
		if(STRNCMP(pRxedModel, irex_ProtocolVersions[i], MODEL_ID_LEN) == 0)
		{
			if(pSendingModel != NULL)
			{
				STRCPY(pSendingModel, irex_ProtocolVersions[i]);
			}
			return (IREX_MODEL_E)i;
		}
	}
	return (IREX_MODEL_E)(IREX_MAX_MODELS - 1);
}

IREX_MODEL_E SKY_API sGetClientModel(BYTE *pRxedModel)
{
	INT16 i;
	for(i = IREX_MAX_MODELS-1; i>=0; i--)
	{
		if(STRNCMP(pRxedModel, irex_ProtocolVersions[i], MODEL_ID_LEN) == 0)
		{
			return (IREX_MODEL_E)i;
		}
	}
	return (IREX_MODEL_E)(-1);
}

/*
	Get the length of message body
*/
WORD SKY_API sGetC5MessageLength(word MsgType)
{
	switch(MsgType)
	{
	case IREX_MELODY_S:
		return sizeof(IREX_OLD_MELODY_S_TYPE_T);
	case IREX_ANI_S:		
		return sizeof(IREX_C5_ANI_S_TYPE_T);
	case IREX_PHONEBOOK_S:
		return sizeof(IREX_C5_PHONEBOOK_S_TYPE_T);
	}
	return 0;
}

WORD SKY_API sGetB5MessageLength(word MsgType)
{
	switch(MsgType)
	{
	case IREX_MELODY_S:
		return sizeof(IREX_OLD_MELODY_S_TYPE_T);
	case IREX_ANI_S:		
		return sizeof(IREX_B5_ANI_S_TYPE_T);
	case IREX_PHONEBOOK_S:
		return sizeof(IREX_C5_PHONEBOOK_S_TYPE_T);
	}
	return 0;
}

WORD SKY_API sGetB5WAPMessageLength(word MsgType)
{
	switch(MsgType)
	{
	case IREX_MELODY_S:
		return sizeof(IREX_OLD_MELODY_S_TYPE_T);
	case IREX_ANI_S:		
		return sizeof(IREX_B5_WAP_ANI_S_TYPE_T);
	case IREX_PHONEBOOK_S:
		return sizeof(IREX_C5_PHONEBOOK_S_TYPE_T);
	}
	return 0;
}

WORD SKY_API sGetC5WAPMessageLength(word MsgType)
{
	switch(MsgType)
	{
	case IREX_MELODY_S:
		return sizeof(IREX_OLD_MELODY_S_TYPE_T);
	case IREX_ANI_S:		
		return sizeof(IREX_C5_WAP_ANI_S_TYPE_T);
	case IREX_PHONEBOOK_S:
		return sizeof(IREX_C5_PHONEBOOK_S_TYPE_T);
	}
	return 0;
}

WORD SKY_API sGetA7MessageLength(word MsgType)
{
	switch(MsgType)
	{
	case IREX_MELODY_S:
		return sizeof(IREX_OLD_MELODY_S_TYPE_T);
	case IREX_ANI_S:		
		return sizeof(IREX_C5_WAP_ANI_S_TYPE_T);
	case IREX_PHONEBOOK_S:
		return sizeof(IREX_A7_PHONEBOOK_S_TYPE_T);
    case IREX_PHONEBOOK_SDIAL_S:
        return sizeof(IREX_A7_PHONEBOOK_SDIAL_S_TYPE_T);
	}
	return 0;
}

WORD SKY_API sGetB1MessageLength(word MsgType)
{
	switch(MsgType)
	{
	case IREX_MELODY_S:
		return sizeof(IREX_OLD_MELODY_S_TYPE_T);
	case IREX_ANI_S:		
		return sizeof(IREX_C5_WAP_ANI_S_TYPE_T);
	case IREX_PHONEBOOK_S:
        return sizeof(IREX_PHONEBOOK_S_TYPE_T);
    case IREX_PHONEBOOK_SDIAL_S:
        return sizeof(IREX_PHONEBOOK_SDIAL_S_TYPE_T);
	}
	return 0;
}

/* rosa 2001-09-19 --> @why, what */
WORD SKY_API sGetB2MessageLength(word MsgType)
{
	switch(MsgType)
	{
	case IREX_MELODY_S:
		return sizeof(IREX_OLD_MELODY_S_TYPE_T);
	case IREX_ANI_S:		
		return sizeof(IREX_C5_WAP_ANI_S_TYPE_T);
	case IREX_PHONEBOOK_S:
        return sizeof(IREX_PHONEBOOK_S_TYPE_T);
    case IREX_PHONEBOOK_SDIAL_S:
        return sizeof(IREX_PHONEBOOK_SDIAL_S_TYPE_T);
	}
	return 0;
}
/* <-- */

WORD SKY_API sGetMessageLength(WORD MsgType)
{
	WORD	MsgLen;

	switch(MsgType)
	{
	case IREX_PING_S:
		MsgLen = sizeof(IREX_PING_S_TYPE_T);
		break;
	case IREX_FINISHED_S:
		MsgLen = sizeof(IREX_FINISHED_S_TYPE_T);
		break;
	case IREX_PING_C:		
		MsgLen = sizeof(IREX_PING_C_TYPE_T);
		break;
	case IREX_PHONEBOOK_C:
		MsgLen = sizeof(IREX_PHONEBOOK_C_TYPE_T);
		break;
	case IREX_FILE_TRANS_S:
		MsgLen = sizeof(IREX_FILE_TRANS_S_TYPE_T) + 2;
		break;
	case IREX_FILE_TRANS_C:
		MsgLen = sizeof(IREX_FILE_TRANS_C_TYPE_T) + 2;
		break;
    case IREX_PHONEBOOK_SDIAL_C:
        MsgLen = sizeof(IREX_PHONEBOOK_SDIAL_C_TYPE_T);
        break;
	default:
		switch(gIrex_ModelIndex)
		{
		case IREX_MODEL_C5:
			MsgLen = sGetC5MessageLength(MsgType);
			break;
		case IREX_MODEL_B5:
			MsgLen = sGetB5MessageLength(MsgType);
			break;
		case IREX_MODEL_B5_WAP:
			MsgLen = sGetB5WAPMessageLength(MsgType);
			break;
		case IREX_MODEL_C5_WAP:
			MsgLen = sGetC5WAPMessageLength(MsgType);
			break;
		case IREX_MODEL_A7:
        case IREX_MODEL_A8:
        case IREX_MODEL_B0:
			MsgLen = sGetA7MessageLength(MsgType);
			break;
        case IREX_MODEL_B1:
            MsgLen = sGetB1MessageLength(MsgType);
            break;
        /* rosa 2001-09-19 --> @why, what */
        case IREX_MODEL_B2:
            MsgLen = sGetB2MessageLength(MsgType);
            break;
        /* <-- */
		default:
			MsgLen = 0;
			break;
		}
		break;
	}
	MsgLen += 2;
	return MsgLen;
}

///////////////////////////////////////////////////////////////////////////////
// Conversion between symbol and byte
///////////////////////////////////////////////////////////////////////////////

// 4 symbols to word
WORD SKY_API sGetWord(BYTE *pSymbols)
{
	WORD Ret;

	Ret = (word)(0x0f & *pSymbols++) << 12;
	Ret |= (word)(0x0f & *pSymbols++) << 8;
	Ret |= (word)(0x0f & *pSymbols++) << 4;
	Ret |= (word)(0x0f & *pSymbols);
	return Ret;
}

// word to 4 symbols
void SKY_API sGetWordSymbol(WORD Word,BYTE *pSymbols)
{
	*pSymbols++ = IREX_P_SYMBOL_BYTE | (byte)(0x000f & (Word >> 12));
	*pSymbols++ = IREX_P_SYMBOL_BYTE | (byte)(0x000f & (Word >> 8));
	*pSymbols++ = IREX_P_SYMBOL_BYTE | (byte)(0x000f & (Word >> 4));
	*pSymbols = IREX_P_SYMBOL_BYTE | (byte)(0x000f & (Word));
}

// 데이터에 대해서만 IREX_P_SYMBOL_BYTE대신
// 0x80, 0x90, ..., 0xF0의 Increased symbol index를 사용하여
// 데이터의 순서가 깨지는 문제를 해결
BYTE SKY_API sGetSymIndex(BYTE* symIndex, BYTE add)
{
	BYTE	ret;

	ret = *symIndex;
	if (add > 0) {
		*symIndex += 0x10;
		if (*symIndex == 0x00)
			*symIndex = 0x80;
	}

	return ret;
}

// 2 symbols to byte
BYTE SKY_API sGetByte(BYTE *pSymbols)
{
	BYTE	Ret;

	Ret = *pSymbols++ << 4;
	Ret |= (0x0f & *pSymbols);
	return Ret;
}

// byte to 2 symbols
void SKY_API sGetByteSymbol(BYTE Byte,BYTE *pSymbols)
{
	*pSymbols++ = IREX_P_SYMBOL_BYTE | (0x0f & (Byte >> 4));
	*pSymbols = IREX_P_SYMBOL_BYTE | (0x0f & (Byte));
}

INT16 SKY_API sGetStream(BYTE *pSymbols, UINT16 nSymbols, BYTE *Bytes)
{
	UINT16  i;

	for(i=0; i<nSymbols/2; i++)
	{
		if((*pSymbols & 0xf0) == sGetSymIndex(&rcvSymIndex, 1))
			*Bytes = *pSymbols++ << 4;
		else
			return -1;
		if((*pSymbols & 0xf0) == sGetSymIndex(&rcvSymIndex, 1))
			*Bytes |= (0x0f & *pSymbols++);
		else
			return -1;
		Bytes++;
	}
	return 1;
}

void SKY_API sGetStreamSymbol(BYTE *Bytes, UINT16 nBytes, BYTE *pSymbols)
{
	UINT16 i;
	
	for(i=0; i<nBytes; i++)
	{
		*pSymbols++ = sGetSymIndex(&sndSymIndex, 1) | (0x0f & (*Bytes >> 4));
		*pSymbols++ = sGetSymIndex(&sndSymIndex, 1) | (0x0f & (*Bytes));
		Bytes++;
	}
}

