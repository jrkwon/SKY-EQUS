#ifndef _CFUNC_H
#define _CFUNC_H

SSLS *ssls_open(
		stPaq *stpPaq,
		int *npErr,
		char *cpUrl, int nMode,
		uc	ucCipher,
		int nReadRetry, int nTimeOut
);
/*
int ssls_write(
	SSLS *spSsls, primitiveC *ipPrim, uc *ucpSend, int nSendLen, int *npErr,
	int	nMode
);

int ssls_read(
	SSLS *spSsls, primitiveC *ipPrim, uc *ucpRecv, int nBufLen, int *npErr,
	int nMode,
	int nReadRetry, int nTimeOut
);

int ssls_close(
	SSLS *spSsls, primitiveC *ipPrim, char *cpUrl, int nMode, int nEncFlag,
	int nTimeOut, char *cpMin
);
*/
#endif
