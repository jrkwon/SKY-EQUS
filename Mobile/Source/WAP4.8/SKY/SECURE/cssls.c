#include <string.h>
#include "rsa.h"
#include "bn.h"
#include "const.h"
#include "md5.h"
#include "keygen.h"
#include "cssls.h"
#include "alert.h"
#include "rec.h"
#include "cFunc.h"
#include "bitt.h"
#include "comdef.h"
#include "aapiclnt.h"
#include "ui.h"

#ifdef WAP_USE_LEAK
	void close_wtls_out_mem(void);
	extern VOID* WIPAlloc(UINT32 size);
    void ssls_clr_buf2(SSLS *spSsls);
#endif//#ifdef WAP_USE_LEAK

/*=====================================================================
							GLOBAL VALUE
 =====================================================================*/
SSLS    g_SKT;
SSLS*   spSKT = NULL;

ul	    ulMsgBufSizeOpen = 0  ;
uc	    ucMsgBufOpen[1500] ;
uc		ucVerifyDataOpen[1500];
ul		ulVerifyDataLenOpen = 0;


//uc	ucMsgBufOpen[HANDSHAKE_BUF_SIZE];



extern VOID    CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);
extern void    RAND_buffer(unsigned char *ucpBuf, int nBufLen);
//extern void gen_ccs(char *cpRec, us usSeqNum);



extern void  wip_free(void* mem);
extern void *wip_malloc(unsigned long size);
//extern unsigned long ui_get_local_time(void);
/*
extern int clnt_wsp_send(
        stPaq *stpPaq,
        unsigned char *ucpSendData, unsigned  int nSendLen,
        char *cpUrl, int nMode, char *cpMin);
*/

void close_wtls_session(void);

void fmtCliHello(
		SSLS *spSsls, uc *ucpBuf, ul *ulpBufLen, uc ucCipher
);

void fmtKeyExchange(
		uc *ucpBuf, ul *ulpBufLen, uc *ucpEncSec, us usEncSecLen 
);

void fmtRecHeader(
		SSLS *spSsls, uc *ucpMsgBuf, ul ulMsgBufSize, uc ucRecType
);

int send_record(
		stPaq *sptPaq, SSLS *spSsls, uc *ucpRec, ul ulRecLen, int *npErr,
		char *cpUrl, int nMode
);

int recv_record(
        stPaq *stpPaq, SSLS *spSsls, int *npErr,
        char *cpUrl, int nMode,
        uc *ucRecBuf, int *npRecBufSize, primitiveC *ipPrim,
        int nTimeOut, int nReadRetry
);

int parse_record(
        stPaq *stpPaq, SSLS *spSsls, int *npErr,
        uc *ucRecBuf, int nRecBufSize,
        uc *ucVerifyData, ul *ulpVerifyDataLen,
        char *cpUrl, int nMode
);

void ssls_clr_buf(SSLS *spSsls);

int ssls_str_buf(SSLS *spSsls, uc *ucpBuf, ul ulBufLen);

void Bin2Ascii(uc *ucBin, us usBinLen, uc *ucAscii, int nBufLen);


void convertBytes(uc *ucpOutBuf, uc *ucpInBuf, int nLen)
{
        int nLoop;
        for (nLoop = 0; nLoop < nLen; nLoop++) 
		{
            ucpOutBuf[nLen - nLoop - 1] = ucpInBuf[nLoop];
        }
}

void reverseBytes(uc *ucpBuf, int nLen)
{
    int nLoop;
    uc ucpTempBuf[4];

    for (nLoop = 0; nLoop < nLen; nLoop++) {
       ucpTempBuf[nLen - nLoop - 1] = ucpBuf[nLoop];
	   }
    memcpy(ucpBuf, ucpTempBuf, nLen);
}
/*====================================================================
              for ssls_open_send & receive data
 ====================================================================*/
int ssls_open_send(
		stPaq *stpPaq,
		int *npErr,
		char *cpUrl, int nMode,
		uc	ucCipher
)
{

	int            nRet = 0;
 
	ulMsgBufSizeOpen    = 0;
	ulVerifyDataLenOpen = 0;

	memset(stpPaq, 0x00, sizeof(stPaq) );
	memset(ucMsgBufOpen,0x00,sizeof(ucMsgBufOpen) );
	memset(ucVerifyDataOpen,0x00,sizeof(ucVerifyDataOpen) );

	(*npErr) = 0;

	if (ucCipher != T_SKA3 && ucCipher != T_SEED) {
		(*npErr) = E_CIPHER;
		return NULL;
	}

	/* clear spSKT */
	if(spSKT!=NULL)
	{
		close_wtls_session();
	}

	/*	create SSLS info */
//	spSKT = (SSLS*)wip_malloc(sizeof(SSLS));
	spSKT = &g_SKT;

//	if (spSKT == NULL)
//	{
//		(*npErr) = E_MEMORY;
//#ifdef WAP_USE_LEAK
//		return 2;
//#else
//		return NULL;
//#endif//#ifdef WAP_USE_LEAK	
//	}
	memset((SSLS*)spSKT, '\0', sizeof(SSLS));

	/*	copy address info */
	memcpy(&(spSKT->stAddrPaq), stpPaq, sizeof(stPaq));

	/* initiallize SSLS info */
	spSKT->ucState = ST_NULL;
	spSKT->ucCipherType = ucCipher;
	spSKT->usCliSeqNum  = 0;
	spSKT->ulBufSize   = 0;

	/* format client hello message */
	fmtCliHello(
		spSKT, ucMsgBufOpen + REC_HEAD_LEN, &ulMsgBufSizeOpen, ucCipher
	);

	fmtRecHeader(
		spSKT, ucMsgBufOpen, ulMsgBufSizeOpen, T_HANDSHAKE
	);

	/* store handshake messages */
	/* 제대로 length값이 설정 되었음*/
	memcpy(ucVerifyDataOpen, ucMsgBufOpen + REC_HEAD_LEN, (unsigned int)ulMsgBufSizeOpen);
	ulVerifyDataLenOpen = ulMsgBufSizeOpen;

	nRet = 
	send_record(
        stpPaq, spSKT, ucMsgBufOpen, ulMsgBufSizeOpen + REC_HEAD_LEN, npErr,
		cpUrl, nMode
	);

	if (nRet < 0) {
        close_wtls_session();
		return NULL;
	}
#ifdef WAP_USE_LEAK
	else if(nRet == 2)
	{
		close_wtls_out_mem();
		return nRet;
	}
#endif//#ifdef WAP_USE_LEAK

	/* change state */
	//jmpark should be moved to invoking position
	spSKT->ucState = ST_WAIT;
	return 1;
}
/*====================================================================
                   ---- CLOSE CONNECTION ----
			usage:    ssls open fail..
			 type:    global..........
 =====================================================================*/
void close_wtls_session(void)
{
	CLNTa_log(1,0," CLOSE WTLS SESSION ENTER");
	if(spSKT!=NULL)
	{
		ssls_clr_buf(spSKT);
//		wip_free(spSKT);
		spSKT = NULL;
	}
}

#ifdef WAP_USE_LEAK
void close_wtls_out_mem(void)
{
	if(spSKT!=NULL)
	{
		ssls_clr_buf2(spSKT);
		spSKT = NULL;
	}
}
#endif//#ifdef WAP_USE_LEAK
/*==================================================================================
   Server Hello message처리,Key exchange,Finished message처리등 
   Handshake관련 처리를 행하여야 하나, 일단 Server hello및 Key exchange Message의 
   처리만을 행하도록 한다.
 ==================================================================================*/
int ssls_recv_hello(stPaq *stpPaq, int *npErr, uc *ucRecBuf, int nRecBufSize,char *cpUrl, int nMode)
{
	int nRet;
	nRet = parse_record(
		stpPaq, spSKT, npErr, ucRecBuf,nRecBufSize,	ucVerifyDataOpen, &ulVerifyDataLenOpen, cpUrl, nMode
	);
	return nRet;
}
/*===================================================================================================
					                     Send END...
  ===================================================================================================*/
int parse_record(
		stPaq *stpPaq, SSLS *spSsls, int *npErr,uc *ucRecBuf, int nRecBufSize, uc *ucVerifyData, 
		ul *ulpVerifyDataLen,char *cpUrl, int nMode
)
{
		int 	nRecNum; 
		int 	nLoop;
		us		usSeqNum;
		uc		ucRecType;

		char 	*cpRec = NULL;
		us		usRecLen;

		char 	*cpPlain;
		char	cpPlainText[LINELEN];
		int		nPlainMsgLen;

		char	*cpMsg;
		us		usMsgLen;		

		static  ul 	ulMasterLen;
		static  ul  ulHash;

		static uc	ucpSec[LINELEN];
		static uc	ucSecLen = PRE_MASTER_LEN;

		ul	ulMsgBufSize = 0;
		uc	*ucMsgBuf;

		RSA *pRsa;		

		static uc 	ucFinished[BYTELEN];
		ul 	ulFinishedLen;
		
		int nRet;	
		uc	ucHandshakeHash[MD5_MAC_SIZE];

		signed short usExpLen; uc ucExp[300];
		signed short usModLen; uc ucMod[300];

//wip_malloc으로 바꿀것..		
		int  nEncLen ;
		static char cpEncSec[128];
		
//================================================================================
		static uc ucKeyBlock[BYTELEN]; 
		static ul ulKeyBlockLen;
		static uc ucMacKeyLen;
		static uc ucEncKeyLen;
		static uc ucIvKeyLen;

//===memory 할당 및 초기화 ===========================================================

		memset(ucFinished,0x00,BYTELEN);
		memset(ucExp,0x00,300);
		memset(ucMod,0x00,300);
		memset(ucKeyBlock,0x00,BYTELEN);
		memset(cpPlainText,0x00,LINELEN);

//=====================================================================================
		ucMsgBuf = (uc*)wip_malloc(PAGELEN);
        if(ucMsgBuf == NULL)
		{
#ifdef WAP_USE_LEAK
			return 2;
#else
			return FAILURE;
#endif//#ifdef WAP_USE_LEAK
		}                  
		(*npErr) = 0;

		memset(cpEncSec,0x00,128);
		memset(ucMsgBuf,0x00,PAGELEN);

		nRecNum = getRecNum((char *)ucRecBuf, nRecBufSize); 

		if (nRecNum < 0) {
			(*npErr) = E_MSGINVALID;
			wip_free(ucMsgBuf);
			return FAILURE;
		}
		
//=============================================================================================
		/* for each record */
		cpRec = (char *)ucRecBuf;

		CLNTa_log(1,0,"start recive hello");


		for (nLoop = 0; nLoop < nRecNum; nLoop++) {
		
			cpPlain = cpRec;

			// get sequence # and record length  
/*================================================================================
                       Record Header information extract
 ================================================================================*/
			memcpy(&usSeqNum, cpPlain + SEQ_NUM_FIELD, sizeof(us));
			reverseBytes((uc *)&usSeqNum, sizeof(us));

			memcpy(&usRecLen, cpPlain + REC_LEN_FIELD, sizeof(us));
			reverseBytes((uc *)&usRecLen, sizeof(us));
//================================================================================
		
			// store received record hash 
			compAlrtHash(cpPlain, usRecLen + REC_HEAD_LEN, (char *)&ulHash);
			str_rhash(spSsls, ulHash);
	
			/* get record type */
			ucRecType = chkbit(cpPlain[0], REC_TYPE);

			/* check if ciphered message */
			if (chkbit(cpPlain[0], BIT_CIPHER)) {
				nRet = 	cipher2Plain(
					cpPlain + REC_HEAD_LEN, 		usRecLen,
					cpPlainText, 					&nPlainMsgLen,
					usSeqNum, ucRecType,
					(char *)(*spSsls).ucSerMacKey,	(uc)MD5_MAC_KEY_SIZE,
            		(char *)(*spSsls).ucSerEncKey, 	(uc)CIPHER_KEY_SIZE,
            		(char *)(*spSsls).ucSerIv, 		(uc)CIPHER_IV_SIZE,
					(*spSsls).ucCipherType
				);
				if (nRet < 0) {
					(*npErr) = E_MAC;
					wip_free(ucMsgBuf);
					//CLNTa_log(1,0,"nRet less than 0");
					return FAILURE;
				}
                if(nPlainMsgLen < 0)
				{
					(*npErr) = E_MAC;
					wip_free(ucMsgBuf);					
					return FAILURE;
				}
				usRecLen = nPlainMsgLen;
				memcpy(cpPlain + REC_HEAD_LEN, cpPlainText, nPlainMsgLen);
			}

			cpMsg = cpPlain + REC_HEAD_LEN;
			if (ucRecType == T_HANDSHAKE) 
			{
	 /*==========================================================================	
						 	MSG Header extract..
		  - Verify data 저장을 위하여 Msg length 필드를 reverse로 저장한다.
	  ==========================================================================*/	
				memcpy(&usMsgLen, cpMsg + 1, sizeof(us));
				reverseBytes((uc*)&usMsgLen, sizeof(us));

	 			if (usMsgLen + HANDSHAKE_HEAD_LEN != usRecLen) {
					(*npErr) = E_MSGINVALID;
					wip_free(ucMsgBuf);
					//CLNTa_log(1,0,"LEngth is bad!!!");
					return FAILURE;
				}
			}

			switch (ucRecType) {
			case T_ALERT:
				if (chk_alert(spSsls, cpMsg + 2) < 0) {
					break;
				}
				if (cpMsg[0] == ALRT_FATAL) {
					wip_free(ucMsgBuf);
					return -1;
				}
            	break;

			case T_HANDSHAKE: {
				int nMsgPos = HANDSHAKE_HEAD_LEN;
				nRet = -1;

				switch(cpMsg[0]) {
				case T_SERVER_HELLO: {
					if (spSsls->ucState != ST_WAIT) {
						break;
					}
	
					/* check protocol version */
					if (cpMsg[nMsgPos] != PROTO_VER) {
						(*npErr) = E_VERSION;
						wip_free(ucMsgBuf);
						return FAILURE;
					}
					nMsgPos++;
					
					/*====================================================
					                  Random Variable
					 ====================================================*/
					memcpy((*spSsls).ucSerRandom,cpMsg + nMsgPos,RANDOM_LEN);
					
					nMsgPos += RANDOM_LEN;
					
					/*====================================================
					                    RSA Exponent
					 ====================================================*/
					memcpy(&usExpLen, cpMsg + nMsgPos, sizeof(us));
					reverseBytes((uc *)&usExpLen, sizeof(us));
					nMsgPos += sizeof(us);
                    if(usExpLen < 0)
					{
						(*npErr) = E_VERSION;
						wip_free(ucMsgBuf);
						return FAILURE;
					}
					memcpy(ucExp, cpMsg + nMsgPos, usExpLen);

					nMsgPos += usExpLen;

					/*====================================================
					                    RSA Modulus
					 ====================================================*/		
					memcpy(&usModLen, cpMsg + nMsgPos, sizeof(us));
					reverseBytes((uc *)&usModLen, sizeof(us));

					nMsgPos += sizeof(us);
                    if(usModLen < 0)
					{
						(*npErr) = E_VERSION;
						wip_free(ucMsgBuf);
						return FAILURE;
					}
					memcpy(ucMod, cpMsg + nMsgPos, usModLen);

					nMsgPos += usModLen;
					
			/*=======================================================================
			             Store Handshake Messages
				Verification data처리를 위하여 각각의 length를 Network order로 바꾸어
				처리한후 ucVerifydata에	저장 하였음
			 =======================================================================*/
                    if(usRecLen < 0)
					{
						(*npErr) = E_VERSION;
						wip_free(ucMsgBuf);
						return FAILURE;
					}
					memcpy(ucVerifyDataOpen + ulVerifyDataLenOpen,cpMsg,usRecLen);

					(*ulpVerifyDataLen) += usRecLen;
					
			//==============================================================
			//  Server Hello MSG ( 5 + 3 + 1 + 16 + n + m) Bytes			
			//	Record Header (5 Byte) + Handshake Message Header (3 Byte)  
			//	+ Server Version (1 Byte) + Server Random (16 Byte)	+		
			//	RSA exponent (n Byte) + RSA modulus	(m Byte)				
			//==============================================================				
					/* generate pre-master secret */
					RAND_buffer(ucpSec, ucSecLen);
					
					pRsa = IA_RSA_New();
                    if(pRsa == NULL) 
					{
			#ifdef WAP_USE_LEAK
						return 2;
			#else
						return FAILURE;
			#endif//#ifdef WAP_USE_LEAK
					}

					pRsa->n = BN_New();
					pRsa->e = BN_New();

					BN_Bin2Bn((uc *)ucMod, 128, pRsa->n);
					BN_Bin2Bn((uc *)ucExp,   1, pRsa->e);
										
					nEncLen = IA_RSA_PublicEncrypt(16, ucpSec, (uc *)cpEncSec, pRsa);
                    if(nEncLen < 0)
					{
			#ifdef WAP_USE_LEAK
						return 2;
			#else
						return FAILURE;
			#endif//#ifdef WAP_USE_LEAK
					}

					IA_RSA_Free(pRsa);
					
					/*jmpark start key exchange messgae */
					/* format key exchange message */
					fmtKeyExchange(
						ucMsgBuf + REC_HEAD_LEN, &ulMsgBufSize,
						(uc *)cpEncSec, (us)nEncLen
					);

					fmtRecHeader(
						spSsls, ucMsgBuf, ulMsgBufSize, T_HANDSHAKE
					);

		//store handshake messages 
					memcpy(ucVerifyDataOpen + ulVerifyDataLenOpen,
							ucMsgBuf + REC_HEAD_LEN,
							(unsigned int)ulMsgBufSize);
					ulVerifyDataLenOpen += ulMsgBufSize;
			
					nRet = send_record(stpPaq, spSsls, 
						ucMsgBuf, ulMsgBufSize + REC_HEAD_LEN, 
						npErr,cpUrl, nMode);

					if (nRet < 0) 
					{
						wip_free(ucMsgBuf);
                        return FAILURE;
					}
            #ifdef WAP_USE_LEAK
					else if(nRet == 2)
					{
						close_wtls_out_mem();
						return nRet;
					}
			#endif//#ifdef WAP_USE_LEAK

					spSsls->ucState = ST_EXKEY;

					computeMasterSecret(
							(*spSsls).ucMaster,     &ulMasterLen,
							ucpSec,                 (ul)ucSecLen,
							(*spSsls).ucCliRandom,  (ul)RANDOM_LEN,
							(*spSsls).ucSerRandom,  (ul)RANDOM_LEN
						);
	
					genKeyBlock(
							ucKeyBlock,             &ulKeyBlockLen,
							(*spSsls).ucMaster,     ulMasterLen,
							(*spSsls).ucCliRandom,  (ul)RANDOM_LEN,
							(*spSsls).ucSerRandom,  (ul)RANDOM_LEN
						);

					getKeyItem(ucKeyBlock,
							(*spSsls).ucCliMacKey, &ucMacKeyLen,
							(*spSsls).ucCliEncKey, &ucEncKeyLen,
							(*spSsls).ucCliIv,     &ucIvKeyLen
						);

					getKeyItem(ucKeyBlock + ucMacKeyLen + ucEncKeyLen + ucIvKeyLen,
							(*spSsls).ucSerMacKey, &ucMacKeyLen,
							(*spSsls).ucSerEncKey, &ucEncKeyLen,
							(*spSsls).ucSerIv,     &ucIvKeyLen
						);					
				}
				break;

				case T_FINISHED: {
					if (spSsls->ucState != ST_EXKEY) {
						break;
					}
		
					computeMD5(ucVerifyDataOpen, ulVerifyDataLenOpen, ucHandshakeHash);	
								
					computePRF(
						ucFinished, &ulFinishedLen,				
						(*spSsls).ucMaster, MASTER_SEC_LEN,
						(uc *)SFINISH_LABEL, (ul)strlen(SFINISH_LABEL),
						ucHandshakeHash, MD5_MAC_SIZE
					);
					ulFinishedLen = FINISHED_DATA_LEN;

					nRet = 	memcmp(	cpMsg + HANDSHAKE_HEAD_LEN, ucFinished,	(unsigned int)ulFinishedLen);

					if (nRet != 0) {
						CLNTa_log(1,0,"FAIL....");
						wip_free(ucMsgBuf);
						return FAILURE;
					}
					spSsls->ucState = ST_OPEN;
				}
				break;

				default:
					{
					wip_free(ucMsgBuf);
					return FAILURE;
					}
				}//switch1
			}
            break;


			case T_DATA:
				break;

			default:
				wip_free(ucMsgBuf);
				return FAILURE;

			}//switch2
			/* next record */
			if(nRecNum > 1)
			{
				cpRec += usRecLen + REC_HEAD_LEN; 
			}
		}
		wip_free(ucMsgBuf);
		return SUCCESS;	
}	


/*======================================================================*/
/*	Client Hello Message : 21 Byte										*/
/*	input :	ucpBuf, *ulpBufLen, ucCipher ( Cipher Type ? SKA3 : SEED )	*/
/*	Output: ucMsgBuf, (*ulpBufLen) = Total Message Length				*/
/*	ucpBuf: [0]		: T_CLIENT_HELLO MSG	(0x01)						*/
/*			[1][2]	: usLen = Hello MSG(21) - HANDSHAKE_HEADER_LEN(3)	*/
/*			[3]		: PROTO_VER(0x01)									*/
/*			[4]		: Cipher Type = SEED (0x02)							*/
/*			[5 ~ 8]	: time (GMT Byte)									*/
/*			[9 ~20]	: Client Random Number								*/
/*======================================================================*/
void fmtCliHello(
		SSLS *spSsls, uc *ucpBuf, ul *ulpBufLen, uc ucCipher
)
{
	ul 		ulPos = 0;
	us		usLen;
	us		usConvLen;

#ifdef debug
	//CLNTa_log(1,0,"MAke CLient Message");
#endif//#ifdef debug

	/* msg_type */
	ucpBuf[ulPos] = T_CLIENT_HELLO;
	ulPos++;

	ulPos += sizeof(us);

	/* client_version */
	ucpBuf[ulPos] = PROTO_VER;
	ulPos++;

	/* cipher */
	ucpBuf[ulPos] = ucCipher;
	ulPos++;

	RAND_buffer(ucpBuf + ulPos, RANDOM_LEN);
	memcpy((*spSsls).ucCliRandom, ucpBuf + ulPos,RANDOM_LEN);

	ulPos += RANDOM_LEN;

	/* length */
	usLen = (us) (ulPos - HANDSHAKE_HEAD_LEN);
	convertBytes((uc*)&usConvLen,(uc*)&usLen, sizeof(us));
	memcpy(ucpBuf + 1, &usConvLen, sizeof(us)); 

	/* total message length */
	(*ulpBufLen) = ulPos;
}
/*======================================================================*/
/*	KEY exchagne Message												*/
/*	input :	ucpBuf, *ulpBufLen, ucpEncSec ( Encryped pre master key )	*/
/*	Output: ucMsgBuf, (*ulpBufLen) = Total Message Length				*/
/*	ucpBuf: [0]		: T_KEY_EXCHANGE MSG	(0x03)						*/
/*			[1][2]	: Total Length(128 + 5) - HANDSHAKE_HEADER_LEN(3)	*/
/*			[3][4]	: Encrypted Pre master Key (128) length 			*/
/*======================================================================*/
void fmtKeyExchange(
		uc *ucpBuf, ul *ulpBufLen, uc *ucpEncSec, us usEncSecLen 
)
{
	ul 		ulPos = 0;
	us		usSecLen = usEncSecLen;
	us		usTotalLen;
	us		usConvSecLen;
	us		usConvTotalLen;

	#ifdef debug
		//CLNTa_log(1,0,"usEncSecLen %d",usEncSecLen);
	#endif//#ifdef debug

	/* msg_type */
	ucpBuf[ulPos] = T_KEY_EXCHANGE;
	ulPos++;

	ulPos += sizeof(us);

	convertBytes((uc*)&usConvSecLen, (uc*)&usSecLen, sizeof(us));
	memcpy(ucpBuf + ulPos, &usConvSecLen, sizeof(us));
	ulPos += sizeof(us);

	memcpy(ucpBuf + ulPos, ucpEncSec, usSecLen);
	ulPos += usSecLen;

	/* length */
	usTotalLen = (us) (ulPos - HANDSHAKE_HEAD_LEN);
	convertBytes((uc*)&usConvTotalLen,(uc*)&usTotalLen, sizeof(us));
	memcpy(ucpBuf + 1, &usConvTotalLen, sizeof(us)); 

	/* total message length */
	(*ulpBufLen) = ulPos;
}
/*======================================================================*/
/*	input :	ucpBuf, *ucMsgBuf, ucRecType ( Rec Type ? T_HANDSHAKE )	*/
/*	Output: ucpMsgBuf, (usMsgBufSize) = Total Message Length			*/
/*	ucpBuf: [0]		: ucRecType = T_HANDSHAKE	(0x03)					*/
/*			[1][2]	: usCliSeqNum										*/
/*			[3][4]	: usMsgBufSize = MESSAGE Size (21)					*/
/*======================================================================*/
void fmtRecHeader(
        SSLS *spSsls, uc *ucpMsgBuf, ul ulMsgBufSize, uc ucRecType
)
{
	us usMsgBufSize;
	us usConvCliSeqNum;
	uc tempbuf[2];

    /* fotmat handshake record */
    ucpMsgBuf[0] = ucRecType;

	convertBytes((uc*)&usConvCliSeqNum,(uc*)&((*spSsls).usCliSeqNum),sizeof(us));
    memcpy(ucpMsgBuf + 1,&usConvCliSeqNum, sizeof(us));
	(*spSsls).usCliSeqNum++;

    usMsgBufSize = (us)ulMsgBufSize;
	tempbuf[1] =((uc*)&usMsgBufSize)[0];
	tempbuf[0] =((uc*)&usMsgBufSize)[1];
    memcpy(ucpMsgBuf + 3, tempbuf, sizeof(us));
}

int send_record(
		stPaq *stpPaq, SSLS *spSsls, uc *ucpRec, ul ulRecLen, int *npErr,
		char *cpUrl, int nMode
)
{
	ul ulHash;
    int nRet = 0;
	
    if(ssls_str_buf(spSsls, ucpRec, ulRecLen) < 0) 
	{
        (*npErr) = E_MEMORY;
#ifdef WAP_USE_LEAK
		return 2;
#else
        return FAILURE;
#endif//#ifdef WAP_USE_LEAK
    }

     /* store record hash */
    compAlrtHash( (char *)spSsls->ucpBuffer, (int)spSsls->ulBufSize, (char *)&ulHash );
	str_shash(spSsls, ulHash);

	return SUCCESS;
}


void ssls_clr_buf(SSLS *spSsls)
{
	spSsls->ulBufSize = 0;
	if (spSsls->ucpBuffer != NULL) {	
		wip_free(spSsls->ucpBuffer);
		spSsls->ucpBuffer = NULL;
	}
}

#ifdef WAP_USE_LEAK
void ssls_clr_buf2(SSLS *spSsls)
{
	spSsls->ulBufSize = 0;
	if (spSsls->ucpBuffer != NULL) {	
		spSsls->ucpBuffer = NULL;
	}
}
#endif//#ifdef WAP_USE_LEAK

int ssls_str_buf(SSLS *spSsls, uc *ucpBuf, ul ulBufLen)
{
	uc *ucpTmp = NULL;
	ucpTmp = (uc *)wip_malloc((unsigned int)(spSsls->ulBufSize + ulBufLen));

	if (ucpTmp == NULL) return FAILURE;
	
	memset(ucpTmp ,0,spSsls->ulBufSize + ulBufLen);

	if (spSsls->ulBufSize != 0) 
	{ 
		//jjkyg add 2001/05/01
		if(spSsls->ulBufSize < 0)
		{
			spSsls->ulBufSize = 0;
			if(spSsls->ucpBuffer !=NULL) 
			{
				wip_free(spSsls->ucpBuffer );
				spSsls->ucpBuffer = NULL;
			}
			if(ucpTmp!=NULL) wip_free(ucpTmp);

			return FAILURE;
		}

		memcpy(ucpTmp, spSsls->ucpBuffer, (unsigned int)spSsls->ulBufSize);
		wip_free(spSsls->ucpBuffer);
		spSsls->ucpBuffer = NULL;
	}

	memcpy(ucpTmp + spSsls->ulBufSize, ucpBuf, (unsigned int)ulBufLen);
	spSsls->ulBufSize += ulBufLen;

	spSsls->ucpBuffer =(uc*)wip_malloc((unsigned int)spSsls->ulBufSize);
	if(spSsls->ucpBuffer == NULL) return FAILURE;

	memcpy(spSsls->ucpBuffer,ucpTmp,spSsls->ulBufSize);

	wip_free(ucpTmp);
	return SUCCESS;
}


int ssls_write
(	
	SSLS *spSsls, primitiveC *ipPrim, uc *ucpSend, 
	int nSendLen, int *npErr,int nMode,char* WcpRec
)
{
	int 	nRecLen;
	us		usRecLen;
	int     enc_length;
	ul		ulHash;

	us		usConvSeqNum;
	us		usConvRecLen;

    int     nRet;

    *npErr = 0;
	enc_length = 0;
	nRecLen    = 0;
	nRet       = 0;

	ssls_clr_buf(spSsls);

	plain2Cipher((char *)ucpSend,nSendLen,WcpRec + REC_HEAD_LEN,&nRecLen,(*spSsls).usCliSeqNum,	T_DATA,
		(char *)(*spSsls).ucCliMacKey,MD5_MAC_KEY_SIZE,(char *)(*spSsls).ucCliEncKey,CIPHER_KEY_SIZE,
		(char *)(*spSsls).ucCliIv,CIPHER_IV_SIZE,(*spSsls).ucCipherType	);
    
	usRecLen = (us)nRecLen;
	
	WcpRec[0] = setbit(T_DATA, BIT_CIPHER);

	convertBytes((uc*)&usConvSeqNum,(uc*)&(*spSsls).usCliSeqNum, sizeof(us));
	memcpy((uc *)WcpRec + SEQ_NUM_FIELD, &usConvSeqNum, sizeof(us));

	convertBytes((uc*)&usConvRecLen,(uc*)&usRecLen, sizeof(us));
	memcpy((uc *)WcpRec + REC_LEN_FIELD, &usConvRecLen, sizeof(us));

	(*spSsls).usCliSeqNum++;

#ifdef WAP_USE_LEAK
	 nRet = ssls_str_buf(spSsls, (uc *)WcpRec, nRecLen + REC_HEAD_LEN);
	 if(nRet < 0)
	 {
		 return 0;
	 }
#else
	(void)ssls_str_buf(spSsls, (uc *)WcpRec, nRecLen + REC_HEAD_LEN);
#endif//#ifdef WAP_USE_LEAK
	
	// store record hash 
	compAlrtHash(WcpRec, nRecLen + REC_HEAD_LEN, (char *)&ulHash);
	str_shash(spSsls, ulHash);

//#ifdef debug	   
	//CLNTa_log(1,0,"enc_length %d  non-enc length %d  return lengh %d",usRecLen,nSendLen,usRecLen+REC_HEAD_LEN);
//#endif//#ifdef debug	

	return (usRecLen + REC_HEAD_LEN);
}

int ssls_read(
		SSLS *spSsls, primitiveC *ipPrim, 
		uc *ucpRecv, int nBufLen,//decrpytion한후 저장할 buffer및 데이타
		int *npErr,int nMode,int nReadRetry, int nTimeOut,
		char* cpRec,int nReadLen//receive data
)
{
	char   *cpPlainText;
	char   *cpPlain;

	int	   nPlainMsgLen;

	char	*cpMsg;

	us		usSeqNum;
	us		usRecLen;
	uc		ucRecType;

	ul		ulHash;

	*npErr = 0;
   
		if (nMode == E2E_READ_NULL_MODE) return SUCCESS;

	/*============================================================
       decrypt한 데이타는 아무리 커도 수신된 데이타 사이즈보다
	   크지 않으므로 이것으로 할당한다.
	   이것을 그냥 ucpRecv로 대처 한다.
     =============================================================*/
	cpPlainText =(char*)wip_malloc(nReadLen);
	if(cpPlainText == NULL)
	{
		return 2;
	}
	memset(cpPlainText,0,nReadLen);

    //실제 수신한 data..(Encryption된 데이타)
	cpPlain = cpRec;

	/* get sequence # and record length */ 
	memcpy(&usSeqNum, cpPlain + SEQ_NUM_FIELD, sizeof(us));
	reverseBytes((uc*)&usSeqNum, sizeof(us));	
	memcpy(&usRecLen, cpPlain + REC_LEN_FIELD, sizeof(us));
	reverseBytes((uc*)&usRecLen, sizeof(us));

	if (usRecLen > nReadLen)
	{
		(*npErr) = E_RECLEN; 
		wip_free(cpPlainText);
		return FAILURE;
	}

	/* store received record hash */
	compAlrtHash(cpPlain, usRecLen + REC_HEAD_LEN, (char *)&ulHash);
	str_rhash(spSsls, ulHash);

	/* get record type */
	ucRecType = chkbit(cpPlain[0], REC_TYPE);
	
	if (chkbit(cpPlain[0], BIT_CIPHER))
	{
		int nRet = 
		cipher2Plain(
			cpPlain + REC_HEAD_LEN, 		usRecLen,//encryption 데이타 
			cpPlainText, 					&nPlainMsgLen,//decryption데이타 
			usSeqNum, 						ucRecType,
			(char *)(*spSsls).ucSerMacKey,	(uc)MD5_MAC_KEY_SIZE,
			(char *)(*spSsls).ucSerEncKey, 	(uc)CIPHER_KEY_SIZE,
			(char *)(*spSsls).ucSerIv, 		(uc)CIPHER_IV_SIZE,
			(*spSsls).ucCipherType
		);

		if (nRet < 0) {
			(*npErr) = E_MAC;
			wip_free(cpPlainText);
			return FAILURE;
		}
#ifdef WAP_USE_LEAK
		else if(nRet == 2)
		{
			return 2;
		}
#endif//#ifdef WAP_USE_LEAK
		//jjkyg add 2001/05/01
		if(nPlainMsgLen <0)
		{
			(*npErr) = E_MAC;
			wip_free(cpPlainText);
			return FAILURE;
		}

		usRecLen = nPlainMsgLen;
		memcpy(cpPlain + REC_HEAD_LEN, cpPlainText, nPlainMsgLen);
	}
	else if (ucRecType == T_DATA)
	{
		(*npErr) = E_NOENC;	
		wip_free(cpPlainText);
		return FAILURE;
	}
	
	cpMsg = cpPlain + REC_HEAD_LEN;
	if (ucRecType == T_DATA) 
	{
		memcpy(ucpRecv, cpPlain + REC_HEAD_LEN, MINF(nPlainMsgLen, nBufLen));
		wip_free(cpPlainText);
		return nPlainMsgLen;
	}
	else if (ucRecType == T_ALERT)
	{
		if (chk_alert(spSsls, cpMsg + 2) < 0)
		{
			(*npErr) = E_ALERTCHECK;
		}
		else if (cpMsg[0] == ALRT_FATAL)
		{
			(*npErr) = E_ALERTFATAL;
		}
		else
		{
			(*npErr) = E_ALERTWARN;
		}
	}
	else
	{
		(*npErr) = E_RECTYPE;
	}

	wip_free(cpPlainText);
	return FAILURE;
}

static char cpAsciiList[] = "0123456789ABCDEF";

void Bin2Ascii(uc *ucBin, us usBinLen, uc *ucAscii, int nBufLen)
{
	int nLoop, nPos;
	uc 	ucUpper, ucLower, ucTmp;

	memset(ucAscii, '\0', nBufLen);

	for (nLoop = nPos = 0; nLoop < usBinLen; nLoop++) {
		ucUpper = ucBin[nLoop] >> 4;
		ucTmp = ucBin[nLoop] << 4;
		ucLower = ucTmp >> 4;

		if (nPos >= nBufLen) {
			return;
		}

		ucAscii[nPos++] = cpAsciiList[ucUpper];
		ucAscii[nPos++] = cpAsciiList[ucLower];
	} 
	ucAscii[nPos] = '\0';
}


BOOL CheckspSkt(void)
{
	if(spSKT !=NULL) 
	{
		ssls_clr_buf(spSKT);
		return TRUE;
	}
    return FALSE;
}

/*====================================================================
                   ---- Encryption SSLS----
			usage:   Real encryption function
			type:    global..........
=====================================================================*/ 
int ency_write(primitiveC *ipPrim,unsigned char *ucpSend, int nSendLen,int *npErr, char* Rec)
{
	int nRet;
	nRet	=	ssls_write(spSKT,ipPrim,ucpSend,nSendLen, npErr,E2E_MODE,Rec);	   
	
	return nRet;
}

/***********************************************************************
*                   ---- Decryption SSLS----
*			usage:   Real decryption function
*			type:    global..........
*************************************************************************/
int ency_read( unsigned char *ucpRecv, int nBufLen, int *npErr,char* cpRec,int nReadLen)
{
	int nRet = ssls_read(spSKT,NULL,ucpRecv,nBufLen,npErr,E2E_MODE,0,0,cpRec,nReadLen);

	return nRet;
}


