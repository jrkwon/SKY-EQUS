#ifndef _SSLS_CIPHER_H
#define _SSLS_CIPHER_H

#define ENC_FLAG	1
#define DEC_FLAG	0

/* 
	encryption/decryption function 

	its argument is input data, encryption key, initialization vector,
	number of round and encryption/decryption mode 
	
	in case of other cipher algorithms, its argument might be changed
*/

int cipherEncrypt(
    uc *ucpIn,      ul ulInLen,
    uc *ucpOut,     ul *ulpOutLen,
    uc *ucpKey,     uc *ucpInitVec,
    uc	ucCipher,	int nMode
);

#endif
