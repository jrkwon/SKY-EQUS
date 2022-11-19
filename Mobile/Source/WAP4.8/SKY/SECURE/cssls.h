#ifndef _SSLS_H
#define _SSLS_H

#include "const.h"
#include "cwsplib.h"

typedef int primitiveC ;
/* error code used in client side */
		#define E_WSP			1
		#define E_MEMORY		2
		#define E_TIMEOUT       3
		#define E_RESEND        4
		#define E_VERSION       5
        #define E_RECLEN        6
        #define E_MAC           7
        #define E_NOENC         8
        #define E_ALERTCHECK   	9
        #define E_ALERTFATAL   	10
        #define E_ALERTWARN     11
        #define E_RECTYPE       12
        #define E_CIPHER       	13
        #define E_MSGINVALID    14


typedef struct {
	
	/* security parameters */
	uc		ucMaster[MASTER_SEC_LEN];
	uc		ucCliRandom[RANDOM_LEN];
	uc		ucSerRandom[RANDOM_LEN];
	
	/* client connection states */
	uc		ucCliMacKey[MD5_MAC_KEY_SIZE];
	uc		ucCliEncKey[CIPHER_KEY_SIZE];
	uc		ucCliIv[CIPHER_IV_SIZE];

	/* server connection states */
	uc		ucSerMacKey[MD5_MAC_KEY_SIZE];
	uc		ucSerEncKey[CIPHER_KEY_SIZE];
	uc		ucSerIv[CIPHER_IV_SIZE];

	/* handshake state */
	uc		ucState;

	uc		ucCipherType;

	/* 
		variables related record hash cash 
		its purpose is check to validity of the alert record 
	*/

	#define MAX_CASH_SIZE	10
	uc		ucHashIdx;				
	/* circular buffer to store the hash values of the sended records */
	ul		arHash[MAX_CASH_SIZE];		

	/* 
		hash value of the most recently received record 
		might be used in case of sending alert record 
	*/
	ul		ulRecvHash;

	/* client write sequence number */
	us				usCliSeqNum;

	/* record buffer used for resending */
	ul				ulBufSize;
	uc 				*ucpBuffer;

	/* 
		address quadruple used for client wsp interface 

		WARNING:
		it should be adapted to the specific wsp WAP stack interface
	*/
	stPaq			stAddrPaq;

	/* 
		wsp primitive structure used for client wsp interface 

		WARNING:
		it should be adapted to the specific wsp WAP stack interface
	*/

	primitiveC		iPrim;

}SSLS;

#define GET_METHOD		1
#define PUT_METHOD		2

#endif
