/*
 * File : mwdef.h
 * - Global Definition of Mobile Wallet
 *
 * SKT Mobile Wallet Ver 1.0 Program 
 *
 * Copyright(c)2001 TGCORP Co. All rights reserved.
 * Writer : Kim, Hyejeong (lunadic@tgcorp.com)
 * Tabstop=4
 * Revision : 1.2.1
 */

#ifndef _MWDEF_H_
#define _MWDEF_H_

/*
 * WPKI Crypto API
 */
#include "sha.h"
#include "seed.h"
#include "cryptlib.h"
#include "crypto.h"
#include "crypto_types.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//swjung
#include "types.h"
#ifdef BLOCK
	#include "time.h"
	#include "sys/types.h"
	#include "sys/stat.h"
#endif //#ifdef BLOCK
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "util.h"
#include "rand.h"
#include "crypto_size.h"

/*
 * 사용된 해쉬,암호화 알고리즘
*/
#define	WALLET_ALG1			SHA1			//PIN 검증 알고리즘 
#define HASHED_LEN			SHA_DIGEST_LENGTH	//SHA1 알고리즘의 출력 길이
#define PER_FORMAT_SIZE		46+3			//a개인정보 데이터 전체 길이

//$$#define DEL_FORMAT_SIZE		124+5			//배송지 데이터 전체 길이
//$$ 배송지 제목 필드 삭제됨
#define DEL_FORMAT_SIZE		108+4			//배송지 데이터 전체 길이

#define CARD_FORMAT_SIZE	39+4			//신용카드 데이터 전체 길이
#define CardExt_FORMAT_SIZE		20+2			//POINT 데이터 전체 길이
#define PIN_LEN				4			//PIN 길이

/*
 * Wallet Data Structure
 * - Person Information
 * - Delivery Informatiom
 * - CreditCard Information
 * - Point Information
 */

typedef struct {
	 unsigned char userName[10+1];		// 사용자 명(10)
	 unsigned char userPhone[12+1];		// 지역 번호 포함 사용자 전화 번호 [4]-[4]-[4](12)
	 unsigned char userEmail[24+1];		// 사용자 e_Mail(24)
}WWPersonalInfo;			// Total Length : 46

typedef struct {
//$$	 unsigned char dvTitle[16+1];		// 배송지 제목(16)
         unsigned char dvName[10+1];               // 수령자명(10)
         unsigned char dvPhone[12+1];              // 수령지 연락 번호(12)
         unsigned char dvZip[6+1];                 // 배송지 우편번호(6)
         unsigned char dvAddr[80+1];               // 배송지 주소(80)
}WWDeliveryInfo;                       // Total Length : 124

typedef struct {
	 unsigned char cardNO[18+1];		// card number without '-'(18)
	 unsigned char cardPwd[4+1];		// 카드 비밀번호(4)
	 unsigned char cardValid[4+1];		// 카드 유효기간(4)
	 unsigned char cardRno[13+1];		// 주민 등록 번호(13)
}WWCreditCard;				// Total Length : 39

typedef struct {
    	unsigned char pointNo[16+1];		// 제휴카드번호
	unsigned char pointPwd[4+1]; 		// OKCashbag 비밀번호
}WWCardExtInfo;				// Total Length : 20

/*
 * Notice :
 * - WPKI function은 시큐어스프트에서 제공하는 것을 사용한다.
 */


/*
 * Declaration of Wallet Data Structure
 */
extern WWPersonalInfo *PersonInfo;
extern WWDeliveryInfo *DelInfo;
extern WWCreditCard *CardInfo;
extern WWCardExtInfo *CardExtInfo;


/*
 * PIN 정보 저장 시에 암호화되는 데이터
 * SHA1[PIN + ESN(Electronic Serial Number) + "Hash Verification"]
 * ESN Length : 4 byte
 */
extern unsigned char gHashedPIN[HASHED_LEN];		// 암호화된 PIN 관련 정보를 담는 변수


/*
* 실제 메모리(NVMemory)에 저장되는 암호화된 Wallet Data Variable
*/

extern unsigned char encPerson[56];		// Encrypted Person Data with padding
extern unsigned char encDel1[136];		// Encrypted Delivery Data with padding
extern unsigned char encDel2[136];		// Encrypted Delivery Data with padding
extern unsigned char encCCard1[48];		// Encrypted CreditCard Data with padding
extern unsigned char encCCard2[48];		// Encrypted CreditCard Data with padding
extern unsigned char encCCard3[48];		// Encrypted CreditCard Data with padding
extern unsigned char encCardExt1[32];		// Encrypted OB Data with padding
//$$extern unsigned char encCardExt2[32];		// Encrypted CardExt Data with padding
//$$extern unsigned char encCardExt3[32];		// Encrypted CardExt Data with padding


/*
 * Wallet Data 암호화에 사용되는 Parameters 
 */
extern long	dPerlen, dCardlen, dDellen, dCardExtlen;		//Encrypted Data Length
//$$extern unsigned char	dPeriv[16], dCardiv[16], dDeliv[16], dCardExtiv[16]; 		//Random initial Vector in SEED
extern unsigned char	dCardiv[16], dCardExtiv[16]; 		//Random initial Vector in SEED

/*****************************************
 * Native Methods
 *****************************************
 */

/*
 * general dynamic memory allocation
 *	Malloc() is provided by Crypto library
 */
extern void *wwMalloc( int sz );

/*
 * general dynamic memory free
 *	Free() is provided by Crypto library
 */
extern void wwFree(void *ptr);


/*****************************************
 *  Wallet Core Functions
 *****************************************
 */
char *wwInputPIN(char *PIN);

/*
 * 사용자로부터 입력받은 PIN을 암호화한 값을 리턴한다.
 * parameter:
 *		char *pin		4 char PIN Number		
 * return:
 *		char *hashedPIN;		4 Byte의 PIN Number
 */
extern unsigned char *wwSetHashedPIN(char *pin_in);

/*
 * 암호화되어 저장된 PIN 값을 가져온다.
 * NV Memory 
 *
 * Parameter :
 *		char *pin			4 char PIN Number		
 * Return :
 * 		unsigned char *hashedPIN;		Hashed PIN(20 Bytes)
 */
////////////////////////////////////////////////////
//swjung 
//type casting
//Add void type as an input argument
////////////////////////////////////////////////////
extern unsigned char *wwGetHashedPIN(void);

/*
 * 단말기에서 입력된 사용자의 PIN 값을 검증한다. 
 *
 * Parameter :
 *		char *pin		4 char PIN Number		
 * Return :
 *		0 if verified else -1
 */
extern int wwVerifyPIN(char *pinVP);

/*
 * Wallet 데이터를 암/복호화하기 위한 Secret Key를 리턴한다.
 *
 * Parameter :
 *		char *pin			4 char PIN
 * Return :
 *		unsigned char *sKey			SecretKey
 */
extern unsigned char *wwGetSecretKey(char *pinGS);

/*
 * Wallet에 설정된 PIN 정보를  변경한다.
 * 
 * Parameter :
 *      char *pinNew			새로운 PIN value 
 *      char *pinOld			이전  PIN value 
 * Return :
 *		0 if successful else -1
 */ 
extern int wwChangePIN(char *pinNew, char *pinOld);

/*
 * Encrypted wallet data size
 *
 *	Parameter :
 * 		int order
 *  Return :
 *      long size
 */
extern long wwEncDataSize(int ord);

/*
 * 개인 정보를 입력받아 암호화하여 저장한다.?
 *
 * Parameter :
 *		WWPersonalInfo *ptr;		사용자 입력 개인 정보 
 *		char *pin;			4 char PIN 
 * 		int order;
 * Return :
 * 			void	
 */
//$$extern void wwSetPerInfo(WWPersonalInfo *ptrP, char *pinSP, int order);
extern void wwSetPerInfo(WWPersonalInfo *ptrP, int order);

/*
 * 신용카드 정보를 입력받아 암호화하여 저장한다. ?
 *
 * Parameter :
 * 		WWCreditCard *ptr;			사용자 입력 카드 정보	
 *		char *pin;				4 char PIN 
 * 		int order;
 * Return :
 *		void
 */
extern void wwSetCardInfo(WWCreditCard *ptrC, char *pinSC, int order);

/*
 * 배송지 정보를 입력받아 암호화하여 저장한다.
 *
 * Parameter :
 *		WWDeliveryInfo *ptr;		사용자 입력 배송지 정보
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *		void
 */
//$$extern void wwSetDelInfo(WWDeliveryInfo *ptrD, char *pinSD, int order);
extern void wwSetDelInfo(WWDeliveryInfo *ptrD, int order);

/*
 * Point 정보를 입력받아 암호화하여 저장한다.
 * 
 * Parameter : 
 *		WWCardExtInfo *ptr;		사용자 입력 Point 정보
 * 		char *pin;			4 char PIN 
 *		int order;
 * Return : 
 * 		void
 */
extern void wwSetCardExtInfo(WWCardExtInfo *ptrO, char *pinSO, int order);


/*
 * Wallet 정보를 암호화하여 저장한다.
 *
 * Parameter :
 *		void *ptr;
 *		char *pin;			4 char PIN
 *		int order;			정보 유형 구분 인자(개인, 카드, 배송지)
 * Return :
 *		void 
 */
extern void wwSetData(void *ptrData, char *pinSD, int setOrd);

/*
 * Wallet에 저장된 사용자 개인 정보를 복호화하여 읽어온다.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *		WWPersonalInfo *ptr;	사용자 개인 정보를 담고 있는 포인터
 */
//$$extern void *wwGetPerInfo(char *pinGP, int ordGP);
extern void *wwGetPerInfo(int ordGP);

/*
 * Wallet에 저장된 사용자 카드  정보를 복호화하여 읽어온다.
 *
 * Parameter : 
 *		char *pin;			4 char PIN 
 *		int order;
 * Return :
 *      WWCreditCard *ptr;    사용자  카드 정보를 담고 있는 포인터
 */
extern void *wwGetCCardInfo(char *pinGC, int ordGC);

/*
 * Wallet에 저장된 사용자 배송지  정보를 복호화하여 읽어온다.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *      WWDeliveryInfo *ptr;    사용자 배송지  정보를 담고 있는 포인터
 */
//extern void *wwGetDelInfo(char *pinGD, int ordGD);
extern void *wwGetDelInfo(int ordGD);

/*
 * Wallet에 저장된 사용자 Point(OkCashBag) 정보를 복호화하여 읽어온다.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *	WWCardExtInfo *ptr;	Point 정보를 담고 있는 포인터
 */
extern void *wwGetCardExtInfo(char *pinGP, int ordGP);

/*
 * Wallet의 자료중 해당 메모리 블럭의 내용을
 * 평문 형태로 복호화하여  읽어 온다.
 *
 * Parameter:
 *		char *pin			4 char PIN Number		
 *		int order;			정보 유형 구분 인자(개인, 카드, 배송지 정보)
 * Return:
 *      void *;
*/
extern void *wwGetData(char *pinGD, int ord);


/**
 * For debugging purpose
 */
#ifdef  DEBUG
#define debugMessage     printf
#else
#define debugMessage
#endif

#define errorMessage    printf

#endif	/* _MWDEF_H_ */
