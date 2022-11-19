/*
 * File : mwscript.h
 * - WMLScript Extension Definition of Mobile Wallet
 *
 * Mobile Wallet Program 
 *
 * Copyright(c)2001 TGCORP Co. All rights reserved.
 * Writer : Kim, Hyejeong (lunadic@tgcorp.com)
 * Tabstop=4
 * Revision : 1.2.1
 */


#ifndef _MWSCRIPT_H_
#define _MWSCRIPT_H_

#include "encrypttext.h"

#define DELIMITER 			"&"			// wallet 데이터 필드 구분인자
#define RET_OK				0			// 정상 종료시의 리턴값
#define RET_ERROR			(-1)		// 비정상 종료시의 리턴값
#define MW_VERSION			0x01		// Mobile Wallet Current Version

#define MASK_Append_Info    0x0001      // AppendStringToMWData BitMask
#define MASK_Per_Info       0x0002      // Personal Info BitMask
#define MASK_Del_Info       0x0004      // Delivery Info BitMask
#define MASK_Card_Info      0x0008      // Card Info BitMask
#define MASK_Account_Info   0x0010      // Account Info BitMask
#define MASK_CardExt_Info   0x0020      // CardExt(신용카드 확장 정보) BitMask

#define COPYTO(target, offset, source, length) \
{  memcpy(&target[offset], source, length); offset += length;  }

//typedef unsigned char unsigned char;
//typedef unsigned int  unsigned int;

/****************************************************
 * ContentType										*
 *  - TEXT(1) : ContentEncoding에 의해 Encoding.	*
 *  - DATA(2) : ContentEncoding 필드 무시.			*
 ***************************************************/
#define TEXT			1
#define DATA			2

/************************************************************************
 * Content_Encoding : PlainWalletData Structure에 사용된 CharacterSet	*
 * - CharacterSet : KS_C_5601-1987(Mandatory), UTF-8, ISO-10646-UCS-2 	*
 ***********************************************************************/
#define  KSC_5601		0x0024
#define	UTF8			0x006a 
#define csUnicode		0x03e8

#ifdef __cplusplus
extern "C" {
#endif

/********************************
 * WMLScript Extension			*
 *******************************/
extern char *getMWData(
	unsigned char *appendString,
	unsigned int setStringEnc,
	unsigned int setStringSign,
	unsigned char *recipientCrt,
	unsigned int keyAgreeAlg,
	unsigned char *contentEncAlg,
	unsigned int 	symmetricKeyId,
	unsigned int keyIdType,
	unsigned char *keyId,
	int errorMMI);
#ifdef __cplusplus
}
#endif

typedef struct _MW_REQ_MSG_ 
{
	unsigned char 	*pAppendString;
	unsigned int	subsetToEnc;
	unsigned int	subsetToSign;
	unsigned char 	*pRecipientCrt;
	unsigned int	keyManageAlg;
	unsigned char	*pContentEncAlg;
	unsigned int 	symmetricKeyId;
	unsigned int 	keyIdType;
	unsigned char 	*keyId;
} TagReqMsg, *ReqMsg;

typedef struct{
	unsigned int	length;
	unsigned char	data[1];
} *ResMsg;

typedef struct{
	unsigned int	personal;
	unsigned int 	delivery;
	unsigned int	card;
	unsigned int	account;
	unsigned int	cardext;	
} AccLevel;

typedef struct{
	unsigned int unsignedWD;
	unsigned int signedWD;
} CurrentField;

/** 전역변수 선언 */
extern AccLevel isAccess;
extern CurrentField curfield; 
extern unsigned char	*pWalletPIN;
extern int errorNO;




/****************************************************
 * SubModule function list 
 ****************************************************
 */
/** Check Subset Rule */
extern int 	checkSubsetRule(unsigned int encfield, unsigned int signfield);

/** Set PlainWalletData Current Filed **/
extern void setCurrentField(unsigned int encfield, unsigned int signfield);

/** Check Certificate */
extern int 	parseCert(unsigned char **decCert, unsigned char *encCert);

/** Check AppendString Display */
extern int	appendStrProcess(int nullCheck, unsigned int signfield);	

/** Build To Be Unsigned Wallet Data */
extern unsigned char *generateUnsignedWD(unsigned char *pAppStr, unsigned int encfield);

/** Build To Be Signed Wallet Data */
extern unsigned char *generateSignedWD(unsigned char *pAppStr, unsigned int encfield);


/** Sign Wallet Data */
extern int	signWD(unsigned char **pData, unsigned char *pAppend_, \
			unsigned int endfield_, unsigned int keyIdType, unsigned char *keyId);

/** Encryption Wallet Data */
extern int	encryptWD(unsigned char **peData, unsigned char *psData,\
			unsigned char *pAppend, unsigned int encfield, unsigned char *pCert,\
			unsigned int keyManAlg, unsigned char *pContEncAlg);

/** Base64 Encoding */
extern unsigned char *encodResult(unsigned char *pEncrypt);

//Modified by swjung
/** Return Error Message */
extern const char	*getErrorMsg(void);

#ifdef BLOCK
/** Device Part Method  **/
extern unsigned char *InputPINtoMWData();
extern int DisplayAppendString();
#endif //#ifdef BLOCK


/****************************************************
 * Wallet Data의 길이와 실제 데이터를 리턴하는 함수
 ****************************************************
 */
extern unsigned char *getMWPersonal( int perOrder );
extern unsigned char *getMWDelivery( int delOrder );
extern unsigned char *getMWCard( unsigned char *cPIN, int cardOrder );
extern unsigned char *getMWAccount( unsigned char *aPIN, int accountOrder );
extern unsigned char *getMWCardExt( unsigned char *oPIN, int cardExtOrder );

#endif  /* _MWSCRIPT_H_ */
