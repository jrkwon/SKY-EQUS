
#include <string.h>
#include <stdlib.h>
#include "const.h"
#include "keygen.h"
#include "md5.h"
#include "dsm.h"

extern void  wip_free(void* mem);
extern void *wip_malloc(unsigned long size);

int computeMD5(
	unsigned char *cpIn, unsigned long ulLen, unsigned char *cpOut
)
{
	MD5_CTX		hDigest;

	ID_MD5_Init(&hDigest);
	ID_MD5_Update(&hDigest, cpIn, ulLen);
	ID_MD5_Final(cpOut, &hDigest);

	return 0;
}

void compute_H_MD5(
	unsigned char *cpIn, unsigned long ulLen,
	unsigned char *cpKey, unsigned long ulKeyLen,
	unsigned char *cpOut, unsigned long *ulpOutLen
)
{
	unsigned char 	cpExKey[64];
	unsigned char 	cpInnerPad[64];
	unsigned char 	cpOuterPad[64];

	static unsigned char 	cpInnerData[500];
	//unsigned char 	*cpInnerData;

	unsigned char 	cpOuterData[80];
	unsigned char 	cpInnerHash[16];
	unsigned char 	cpOuterHash[16];
	int 			nLoop;

	
	memset(cpExKey, '\0', 64); 
	memset(cpInnerPad, '\0', 64); 
	memset(cpOuterPad, '\0', 64); 
	memset(cpOuterData, '\0', 80); 
	memset(cpInnerHash, '\0', 16); 
	memset(cpOuterHash, '\0', 16); 
	
	memcpy(cpExKey, cpKey, (unsigned int)ulKeyLen);

	for (nLoop = 0; nLoop < 64; nLoop++) {
		cpInnerPad[nLoop] = cpExKey[nLoop] ^ 0x36;
		cpOuterPad[nLoop] = cpExKey[nLoop] ^ 0x5c;
	}

	
	//cpInnerData = (unsigned char *)wip_malloc((unsigned int)ulLen + 64);
	//memset(cpInnerData, '\0', (unsigned int)ulLen + 64);
	memset(cpInnerData, '\0',500);

	memcpy(cpInnerData, cpInnerPad, 64);
	memcpy(cpInnerData + 64, cpIn, (unsigned int)ulLen); 
	computeMD5(cpInnerData, ulLen + 64, cpInnerHash);
	
	//wip_free(cpInnerData);
	

	memcpy(cpOuterData, cpOuterPad, 64);
	memcpy(cpOuterData + 64, cpInnerHash, 16);
	computeMD5(cpOuterData, 16 + 64, cpOuterHash);

	memcpy(cpOut, cpOuterHash, 16);
	(*ulpOutLen) = MD5_DIGEST_LENGTH;
}
