#include<string.h>
#include "const.h"
#include "md5.h"
#include "md5_lib.h"
#include "rec.h"
#include "cipher.h"
#include "tapicmmn.h"

extern VOID    CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);

extern void *wip_malloc(unsigned long size);
extern void wip_free(void* mem);

extern void reverseBytes(uc *ucpBuf, int nLen);
extern void convertBytes(uc *ucpOutBuf, uc *ucpInBuf, int nLen);

int getRecNum(char *cpRec, int nRecLen)
{
    int nPos = 0;
    us  usLen;
    int nRecNum = 0;

    while (nPos < nRecLen) {
        if (nPos + REC_HEAD_LEN > nRecLen) {
            return FAILURE;
        }
        memcpy(&usLen, cpRec + nPos + REC_HEAD_LEN - sizeof(us), sizeof(us));
		reverseBytes((uc *)&usLen, sizeof(us));
		CLNTa_log(1,0,"RECEIVE REC LENGTH %d",usLen);
        nRecNum++;
        nPos += REC_HEAD_LEN + usLen;

    }
    if (nPos == nRecLen) {
        return nRecNum;
    }
	CLNTa_log(1,0,"nPOS %d, nRecLen %d",nPos,nRecLen);
    return FAILURE;
}

void genHashInput(
	char *cpHashData, ul *ulpDataLen,
	us usSeqNum, uc ucRecType,
	char *cpPlain, int nPlainLen
)
{
	ul	ulPos = 0;
	us	_usSeq;
	us	_usLen;

	us	usConvSeq;
	us	usConvLen;

	_usSeq = usSeqNum;
	_usLen = nPlainLen;

	/* sequence number */
	convertBytes((uc *)&usConvSeq, (uc *)&_usSeq, sizeof(us));
	memcpy(cpHashData + ulPos, &usConvSeq, sizeof(us));
	ulPos += sizeof(us);

	/* record type */
	cpHashData[ulPos] = ucRecType;
	ulPos++;

	/* length */
	convertBytes((uc *)&usConvLen, (uc *)&_usLen, sizeof(us));
	memcpy(cpHashData + ulPos, &usConvLen, sizeof(us));
	ulPos += sizeof(us);

	/* fragment */
    if(nPlainLen < 0)
	{
		CLNTa_log(1,0,"ÀÌ·±...");
		(*ulpDataLen) = 0;	
		return;
	}
	memcpy(cpHashData + ulPos, cpPlain, nPlainLen);
	ulPos += nPlainLen;

	(*ulpDataLen) = ulPos;
}

void plain2Cipher(
		char *cpPlain, 	int nPlainLen,
		char *cpCipher, int *npCipherLen,
		us	 usSeqNum,	uc ucRecType,
		char *cpMac, 	uc ucMacLen,
		char *cpEnc, 	uc ucEncLen,
		char *cpIv,		uc ucIvLen,
		uc	ucCipher
)
{
	char	cpHash[MD5_MAC_SIZE];
	ul		ulHashLen;

	char 	*cpHashData;
	ul		ulHashDataLen;
	char 	*cpData;
	ul		ulPos = 0;

	uc		ucPadSize = 0;
	int		nLoop;

	ul		ulOutLen;
    
	cpHashData = (char*)wip_malloc(BUFLEN);
   if(cpHashData == NULL) return;

   memset(cpHashData,0x00,BUFLEN);

   if(nPlainLen < 0)
   {
	   wip_free(cpHashData);
	   return;
   }

   genHashInput(cpHashData, &ulHashDataLen,usSeqNum,ucRecType,cpPlain,(unsigned int)nPlainLen);
   compute_H_MD5((uc *)cpHashData,	ulHashDataLen,(uc *)cpMac,(ul)ucMacLen,(uc *)cpHash,&ulHashLen);

	
   if (ucCipher == T_SKA3) 
   {
		ucPadSize = 
		(uc)(SKA3_BLOCK_SIZE - ((nPlainLen + ulHashLen) % SKA3_BLOCK_SIZE));
   }
   else if (ucCipher == T_SEED)
   {
       ucPadSize =
       (uc)(SEED_BLOCK_SIZE - ((nPlainLen + ulHashLen) % SEED_BLOCK_SIZE));
   }
   
   
    cpData     = (char*)wip_malloc(BUFLEN); 
	if(cpData == NULL) return;

    memset(cpData,0x00,BUFLEN);
	ulPos = 0;

	memcpy(cpData + ulPos, cpPlain, nPlainLen);
	ulPos += nPlainLen;
	memcpy(cpData + ulPos, cpHash, (unsigned int)ulHashLen);

	ulPos += ulHashLen;

	for (nLoop = 0; nLoop < ucPadSize; nLoop++) {
		//cptemp[ulPos + nLoop] = (uc) ucPadSize - 1;
		cpData[ulPos + nLoop] = (uc) ucPadSize - 1;
	}
	ulPos += ucPadSize;

	(*npCipherLen) = (int)ulPos;

	cipherEncrypt((uc *)cpData,ulPos,(uc *)cpCipher,&ulOutLen,(uc *)cpEnc,(uc *)cpIv,ucCipher, ENC_FLAG);
	(*npCipherLen) = (int)ulOutLen;

	wip_free(cpHashData);
	wip_free(cpData);
}


int cipher2Plain(
		char *cpCipher, int nCipherLen,
		char *cpPlain,  int *npPlainLen,
		us	 usSeqNum,	uc ucRecType,
		char *cpMac, 	uc ucMacLen,
		char *cpEnc, 	uc ucEncLen,
		char *cpIv,		uc ucIvLen,
		uc 	ucCipher
)
{
	char* 	cpHashData;
	ul		ulHashDataLen;

	char	cpHash[MD5_MAC_SIZE];
	ul		ulHashLen;

	ul		ulOutLen;
    
	//cpHashData = (char*)wip_malloc(1500);
	//memset(cpHashData,0,1500);
	cpHashData = (char*)wip_malloc(nCipherLen);
    if(cpHashData == NULL)
	{
		return 2;
	}
	memset(cpHashData,0,nCipherLen);

	cipherEncrypt(
		(uc *)cpCipher, nCipherLen, 
		(uc *)cpPlain, 	&ulOutLen,
		(uc *)cpEnc,	(uc *)cpIv,
		ucCipher, DEC_FLAG);

	(*npPlainLen) = (int)(ulOutLen - cpPlain[nCipherLen - 1] - 1 - MD5_MAC_SIZE);
    if(*npPlainLen < 0)
	{
		wip_free(cpHashData);
		return FAILURE;	
	}

	genHashInput(
		cpHashData, &ulHashDataLen,
		usSeqNum, 	ucRecType,
		cpPlain, 	(*npPlainLen));

    if(ulHashDataLen == 0) 
	{
		wip_free(cpHashData);
		return FAILURE;	
	}
	
	
	compute_H_MD5(
		(uc *)cpHashData, 	ulHashDataLen, 
		(uc *)cpMac, 		(ul)ucMacLen, 
		(uc *)cpHash, 		&ulHashLen);
	
	
	if (memcmp(cpHash, cpPlain + (*npPlainLen), (unsigned int)ulHashLen) == 0) {
		wip_free(cpHashData);
		return SUCCESS;
	}
	wip_free(cpHashData);
	return FAILURE;
}

void gen_ccs(char *cpRec, us usSeqNum)
{
	us	usMsgLen = CCS_MSG_LEN;

	cpRec[0] = T_CCS;
	memcpy(cpRec + SEQ_NUM_FIELD, &usSeqNum, sizeof(us));
	memcpy(cpRec + REC_LEN_FIELD, &usMsgLen, sizeof(us));
	cpRec[REC_HEAD_LEN] = T_CCS;
}

void compAlrtHash(char *cpData, int nDataLen, char *cpHash)
{
	//char 	cpHashData[500];
	char*   cpHashData;
	int		nRound;
	char	*cpPos;
	ul		ulThisBlock;
	ul		ulNextBlock;
	int 	nLoop;
    
	cpHashData =(char*)wip_malloc(nDataLen);
    if(cpHashData == NULL) return;
	
	nRound = nDataLen / sizeof(ul);

	memset(cpHashData, '\0', nDataLen);
	//memset(cpHashData, '\0', 500);
	memcpy(cpHashData, cpData, nDataLen);

	memcpy(&ulThisBlock, cpHashData, sizeof(ul));
	cpPos = cpHashData + sizeof(ul);
	memcpy(&ulNextBlock, cpPos, sizeof(ul));

	for (nLoop = 0; nLoop < nRound; nLoop++) {
		ulThisBlock ^= ulNextBlock;
		cpPos += sizeof(ul);
		memcpy(&ulNextBlock, cpPos, sizeof(ul));
	}

	memcpy(cpHash, &ulThisBlock, sizeof(ul));

	wip_free(cpHashData);
}
