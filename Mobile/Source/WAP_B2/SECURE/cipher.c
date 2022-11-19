#include <string.h>
#include <stdlib.h>
#include "const.h"
#include "cipher.h"
#include "seedx.h"

extern void prtBuf(uc *ucpBuf, ul ulBufLen);

int cipherSeed(
    uc *ucpIn,      ul ulInLen,
    uc *ucpOut,     ul *ulpOutLen,
    uc *ucpKey,
    int nMode
)
{
    DWORD   dwRoundKey[NoRoundKeys];
    uc      ucpInput[SeedBlockSize];
    int     nLoop;

    for (nLoop = 0; nLoop < (ulInLen / SEED_BLOCK_SIZE); nLoop++) {
        SeedEncRoundKey(dwRoundKey, ucpKey);
        memcpy(ucpInput, ucpIn + nLoop * SEED_BLOCK_SIZE, SEED_BLOCK_SIZE);
        if (nMode == ENC_FLAG) {
            SeedEncrypt(ucpInput, dwRoundKey);
        }
        else if (nMode == DEC_FLAG) {
            SeedDecrypt(ucpInput, dwRoundKey);
        }
        memcpy(ucpOut + nLoop * SEED_BLOCK_SIZE, ucpInput, SEED_BLOCK_SIZE);
    }
    (*ulpOutLen) = ulInLen;
    return 0;
}

int cipherEncrypt(
	uc *ucpIn, 		ul ulInLen,
	uc *ucpOut, 	ul *ulpOutLen,
	uc *ucpKey, 	uc *ucpInitVec, 
	uc	ucCipher,	int nMode
)
{
	if (ucCipher == T_SEED) {
        return cipherSeed(
        	ucpIn, ulInLen, ucpOut, ulpOutLen, ucpKey, nMode
    	);
    }

	return FAILURE;
}
