#ifndef _CWSP_H
#define _CWSP_H

#define MAX_MIN_LEN         15

/* Peer Address Quadruple */
typedef struct stPaq {
    unsigned char   ucpSrvIp[4];
    int             nSrvPortNum;

    unsigned char   ucpCntIp[4];
    int             nCntPortNum;

	char 			cpMin[MAX_MIN_LEN];
}stPaq;



#define SSLS_MODE			1
#define E2E_MODE			2
#define E2E_READ_NULL_MODE	3
/*
int wsp_open(
		char *cpCfgPath, 
		int	nPortNum, int nSslsMqKIn, int nSslsWipMqKIn, int nCWspMqKIn
);

int wsp_read(
		stPaq		*stpPaq,
		char 		*cpData, int *npRecvLen,
		primitiveC 	*ipPrimitive,
		int			nTimeOut);
*/
//	=> non-blocking read.
//  => RetVal : SUCCESS/NO_EVENT_RECVD/FAIL
//	=> received ipPrimitive should freeed by *_send() or ssls


/****	for real-data transmission *****/
/*
int wsp_send(
		unsigned char 		*ucpSendData, unsigned  int nSendLen,
		primitiveC 	*ipPrimitive, int nMode);
*/
/****	for hand-shaking or closing *****/
//remove_error
//int clnt_wsp_send(
//        stPaq       *stpPaq,
//		unsigned char 		*ucpSendData, unsigned  int nSendLen,
//		char *cpUrl, int nMode, char *cpMin);
#endif
