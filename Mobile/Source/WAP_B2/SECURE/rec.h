#ifndef _REC_H
#define _REC_H

/*	
	get the number of record from received data 
*/
int getRecNum(char *cpRec, int nRecLen);

/*
	convert fragment field of SSLPlainText into 
	GenericBlockCipher field of SSLCipherText

	its argument is input data, record sequence number,
	MAC key, encryption key and initialization vector
*/
void plain2Cipher(
        char *cpPlain,  int nPlainLen,
        char *cpCipher, int *npCipherLen,
        us  usSeqNum,   uc ucRecType,
        char *cpMac,    uc ucMacLen,
        char *cpEnc,    uc ucEncLen,
        char *cpIv,     uc ucIvLen,
		uc	ucCipher
);

/*
	convert GenericBlockCipher field of SSLCipherText into
	fragment field of SSLPlainText 

	it also check MAC value
	it it's wrong return FAILURE

	its argument is input data, record sequence number,
	MAC key, encryption key and initialization vector
*/
int cipher2Plain(
        char *cpCipher, int nCipherLen,
        char *cpPlain,  int *npPlainLen,
        us   usSeqNum,  uc ucRecType,
        char *cpMac,    uc ucMacLen,
        char *cpEnc,    uc ucEncLen,
        char *cpIv,     uc ucIvLen,
		uc	ucCipher
);

/*	compute 4 byte hash used for alert hash */
void compAlrtHash(char *cpData, int nDataLen, char *cpHash);

#endif
