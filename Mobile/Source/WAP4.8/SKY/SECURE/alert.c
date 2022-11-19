#include <string.h>
#include "const.h"
#include "bitt.h"
#include "cwsplib.h"
#include "cssls.h"
#include "rec.h"

extern void reverseBytes(uc *ucpBuf, int nLen);
extern void convertBytes(uc *ucpOutBuf, uc *ucpInBuf, int nLen);

void str_shash(SSLS *spSsls, ul ulHash)
{	
	(*spSsls).arHash[(*spSsls).ucHashIdx % MAX_CASH_SIZE] = ulHash;
	(*spSsls).ucHashIdx++;
}

void str_rhash(SSLS *spSsls, ul ulHash)
{
	(*spSsls).ulRecvHash = ulHash;
}

int gen_alert(uc *ucpAlert, uc ucLevel, uc ucDesc, SSLS *spSsls, uc ucEnc)
{
	char 	cpRec[BYTELEN];
	us 		usAltDataLen = ALERT_DATA_LEN;
	int		nRecLen;
	us		usRecLen;
	us		usSeqNum;

	us		usConvAltDataLen;
	us		usConvRecLen;
	us		usConvSeqNum;


	usSeqNum = (*spSsls).usCliSeqNum;

	cpRec[REC_HEAD_LEN] = ucLevel;
	cpRec[REC_HEAD_LEN + 1] = ucDesc;
	memcpy(
		cpRec + REC_HEAD_LEN + 2, 
		&((*spSsls).ulRecvHash), 
		ALERT_CHECKSUM_LEN
	);

	cpRec[0] = T_ALERT;
	convertBytes((uc *)&usConvSeqNum, (uc *)&usSeqNum, sizeof(us));
	memcpy(
		cpRec + SEQ_NUM_FIELD, 
		&usConvSeqNum, 
		sizeof(us)
	);

	convertBytes((uc *)&usConvAltDataLen, (uc *)&usAltDataLen, sizeof(us));
	memcpy(
		cpRec + REC_LEN_FIELD, 
		&usConvAltDataLen,
		sizeof(us)
	);

	if (ucEnc) {
		plain2Cipher(
			cpRec + REC_HEAD_LEN,				ALERT_DATA_LEN,
			(char *)ucpAlert + REC_HEAD_LEN,	&nRecLen,
			usSeqNum,  							T_ALERT,
        	(char *)(*spSsls).ucCliMacKey,  	MD5_MAC_KEY_SIZE,
        	(char *)(*spSsls).ucCliEncKey,  	CIPHER_KEY_SIZE,
        	(char *)(*spSsls).ucCliIv,      	CIPHER_IV_SIZE,
			(*spSsls).ucCipherType
    	);

		usRecLen = (us)nRecLen;

		ucpAlert[0] = setbit(T_ALERT, BIT_CIPHER);
		convertBytes((uc *)&usConvSeqNum, (uc *)&usSeqNum, sizeof(us));
    	memcpy(ucpAlert + SEQ_NUM_FIELD, &usConvSeqNum, sizeof(us));

		convertBytes((uc *)&usConvRecLen, (uc *)&usRecLen, sizeof(us));
    	memcpy(ucpAlert + REC_LEN_FIELD, &usConvRecLen, sizeof(us));

		return REC_HEAD_LEN + nRecLen;
	}
	else {
		memcpy(ucpAlert, cpRec, REC_HEAD_LEN + ALERT_DATA_LEN);
		return REC_HEAD_LEN + ALERT_DATA_LEN;
	}
}

int chk_alert(SSLS *spSsls, char *cpAlert)
{
	int nLoop;

	for (nLoop = 0; nLoop < MAX_CASH_SIZE; nLoop++) {
		if (memcmp(cpAlert, &((*spSsls).arHash[nLoop]), sizeof(ul)) == 0) {
			return SUCCESS;
		}  
	}
	return FAILURE;
}
