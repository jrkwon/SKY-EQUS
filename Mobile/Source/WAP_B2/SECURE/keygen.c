#include <string.h>
#include "const.h"
#include "keygen.h"



extern void  wip_free(void* mem);
extern void *wip_malloc(unsigned long size);

void init_A_List(A_List *spList)
{
	memset(spList, '\0', sizeof(A_List));
}

void comp_A_List_MD5(
	A_List *spList, int nCnt, 
	uc *ucSecret, 	ul ulSecLen,
	uc *ucSeed, 	ul ulSeedLen)
{
	int nLoop;

	(*spList).nCnt = nCnt;
	memcpy((*spList).arHash[0].ucHash, ucSeed, (unsigned int)ulSeedLen);  
	(*spList).arHash[0].ulLen = ulSeedLen;

	for (nLoop=1; nLoop <= nCnt; nLoop++) {
		compute_H_MD5(
			(*spList).arHash[nLoop-1].ucHash, (*spList).arHash[nLoop-1].ulLen,
			ucSecret, ulSecLen,
			(*spList).arHash[nLoop].ucHash, &((*spList).arHash[nLoop].ulLen));	
	}
}

void P_MD5(
	uc *ucOut, 		ul *ulpOutLen,
	int nCnt,
	uc *ucSecret,	ul ulSecLen,
	uc *ucSeed, 	ul ulSeedLen)
{
	uc* 	ucBuf;
//	ul 		ulLen = 0;
	A_List	stList;
	int		nLoop;
	ul 		ulOutLen;


	ucBuf =(uc*)wip_malloc(ulSeedLen +MD5_MAC_SIZE +1);
    if(ucBuf == NULL) return;
	init_A_List(&stList);
	comp_A_List_MD5(
		&stList, A_CNT_MD5,
		ucSecret, ulSecLen,
		ucSeed,	ulSeedLen);

	for (nLoop=0; nLoop < nCnt; nLoop++) {
		memset(ucBuf, '\0', ulSeedLen +MD5_MAC_SIZE +1);
		memcpy(ucBuf, stList.arHash[nLoop+1].ucHash, MD5_MAC_SIZE);
		memcpy(ucBuf + MD5_MAC_SIZE, ucSeed, (unsigned int)ulSeedLen);

		compute_H_MD5(
			ucBuf, MD5_MAC_SIZE + ulSeedLen,
			ucSecret, ulSecLen, 
			ucOut + (nLoop * MD5_MAC_SIZE), &ulOutLen);
	}
	(*ulpOutLen) = nCnt * MD5_MAC_SIZE;
	wip_free(ucBuf);
}
	
void computePRF(
	uc *ucOut, 		ul *ulpOutLen,
	uc *ucSecret, 	ul ulSecLen,
	uc *ucLabel, 	ul ulLabelLen,
	uc *ucSeed, 	ul ulSeedLen)
{
	static uc 	ucS1[BYTELEN];//uc 	*ucS1;  		
	static uc 	ucS2[BYTELEN];//uc 	*ucS2;  		
	static uc 	ucHashMD5[BYTELEN]; 	
	static uc   ucInput[500];

	ul	ulSLen;
	ul	ulHashLen;
	
	memset(ucS1, '\0', BYTELEN); 
	memset(ucS2, '\0', BYTELEN); 
	memset(ucHashMD5, '\0', BYTELEN); 
	memset(ucInput, '\0',500); 

	if (ulSecLen % 2 == 0) {
		ulSLen = ulSecLen / 2;
		memcpy(ucS1, ucSecret, (unsigned int)ulSLen);
		memcpy(ucS2, ucSecret + ulSLen, (unsigned int)ulSLen);
	}
	else {
		ulSLen = (ulSecLen + 1) / 2;
		memcpy(ucS1, ucSecret, (unsigned int)ulSLen);
		memcpy(ucS2, ucSecret + ulSLen - 1, (unsigned int)ulSLen);
	}

	memcpy(ucInput, ucLabel, (unsigned int)ulLabelLen);
	memcpy(ucInput + ulLabelLen, ucSeed, (unsigned int)ulSeedLen);

	P_MD5(
		ucHashMD5, &ulHashLen, A_CNT_MD5,
		ucS1, ulSLen,
		ucInput, ulLabelLen + ulSeedLen);

	memcpy(ucOut, ucHashMD5, (unsigned int)ulHashLen);
	(*ulpOutLen) = ulHashLen;	
}

void computeMasterSecret(
	uc *ucOut, 				ul *ulpOutLen,
	uc *ucPreMasterSecret, 	ul ulSecLen,
	uc *ucCliRandom, 		ul ulCliRandomLen,
	uc *ucServRandom, 		ul ulServRandomLen)
{
	//static uc ucOutBuf[BYTELEN];
	uc ucOutBuf[BYTELEN];
	ul ulOutBufLen;
	uc* ucSeed;
	//uc* ucMasterSecret;
	
	memset(ucOutBuf,0,BYTELEN);
	ucSeed = (uc*)wip_malloc(ulCliRandomLen + ulServRandomLen +1);
    if(ucSeed == NULL) return;
	memset(ucSeed, '\0', ulCliRandomLen + ulServRandomLen +1);


	memcpy(ucSeed, ucCliRandom, (unsigned int)ulCliRandomLen);
	memcpy(
		ucSeed + ulCliRandomLen, ucServRandom, (unsigned int)ulServRandomLen
	);

	computePRF(
		ucOutBuf, &ulOutBufLen,
		ucPreMasterSecret, ulSecLen,
		(uc *)MSEC_LABLE, strlen(MSEC_LABLE),
		ucSeed, ulCliRandomLen + ulServRandomLen);

	memcpy(ucOut, ucOutBuf, MASTER_SEC_LEN);
	(*ulpOutLen) = MASTER_SEC_LEN;

	wip_free(ucSeed);
	
}

void genKeyBlock(
	uc *ucKeyBlock, 	ul *ulpKeyBlockLen,
	uc *ucMasterSecret, ul ulSecLen,
	uc *ucCliRandom, 	ul ulCliRandomLen,
	uc *ucServRandom, 	ul ulServRandomLen
)
{
	uc* 	ucSeed;

	ucSeed = (uc*)wip_malloc(ulCliRandomLen + ulServRandomLen +1);
    if(ucSeed == NULL ) return;
	memset(ucSeed , 0,ulCliRandomLen + ulServRandomLen +1);
	/* compute key_block */
	memcpy(ucSeed, ucServRandom, (unsigned int)ulServRandomLen);
	memcpy(ucSeed + ulServRandomLen, ucCliRandom, (unsigned int)ulCliRandomLen);

	computePRF(
		ucKeyBlock, ulpKeyBlockLen,
		ucMasterSecret, ulSecLen,
		(uc *)KEY_BLOCK_LABEL, strlen(KEY_BLOCK_LABEL),
		ucSeed, ulCliRandomLen + ulServRandomLen);

	wip_free(ucSeed);
}

void getKeyItem(
	uc *ucKeyBlock, 
    uc *ucMacKey,   uc *ucpMacKeyLen, 
    uc *ucEncKey,   uc *ucpEncKeyLen,
    uc *ucIvKey,    uc *ucpIvKeyLen
)
{
	int nPos = 0;

	(*ucpMacKeyLen) = MD5_MAC_KEY_SIZE;
	(*ucpEncKeyLen) = CIPHER_KEY_SIZE;
	(*ucpIvKeyLen) = CIPHER_IV_SIZE;

	memcpy(ucMacKey, ucKeyBlock + nPos, (*ucpMacKeyLen));
	nPos += (*ucpMacKeyLen);
	memcpy(ucEncKey, ucKeyBlock + nPos, (*ucpEncKeyLen));
	nPos += (*ucpEncKeyLen);
	memcpy(ucIvKey, ucKeyBlock + nPos, (*ucpIvKeyLen));
	nPos += (*ucpIvKeyLen);
}
