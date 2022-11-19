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
 * ���� �ؽ�,��ȣȭ �˰���
*/
#define	WALLET_ALG1			SHA1			//PIN ���� �˰��� 
#define HASHED_LEN			SHA_DIGEST_LENGTH	//SHA1 �˰����� ��� ����
#define PER_FORMAT_SIZE		46+3			//a�������� ������ ��ü ����

//$$#define DEL_FORMAT_SIZE		124+5			//����� ������ ��ü ����
//$$ ����� ���� �ʵ� ������
#define DEL_FORMAT_SIZE		108+4			//����� ������ ��ü ����

#define CARD_FORMAT_SIZE	39+4			//�ſ�ī�� ������ ��ü ����
#define CardExt_FORMAT_SIZE		20+2			//POINT ������ ��ü ����
#define PIN_LEN				4			//PIN ����

/*
 * Wallet Data Structure
 * - Person Information
 * - Delivery Informatiom
 * - CreditCard Information
 * - Point Information
 */

typedef struct {
	 unsigned char userName[10+1];		// ����� ��(10)
	 unsigned char userPhone[12+1];		// ���� ��ȣ ���� ����� ��ȭ ��ȣ [4]-[4]-[4](12)
	 unsigned char userEmail[24+1];		// ����� e_Mail(24)
}WWPersonalInfo;			// Total Length : 46

typedef struct {
//$$	 unsigned char dvTitle[16+1];		// ����� ����(16)
         unsigned char dvName[10+1];               // �����ڸ�(10)
         unsigned char dvPhone[12+1];              // ������ ���� ��ȣ(12)
         unsigned char dvZip[6+1];                 // ����� �����ȣ(6)
         unsigned char dvAddr[80+1];               // ����� �ּ�(80)
}WWDeliveryInfo;                       // Total Length : 124

typedef struct {
	 unsigned char cardNO[18+1];		// card number without '-'(18)
	 unsigned char cardPwd[4+1];		// ī�� ��й�ȣ(4)
	 unsigned char cardValid[4+1];		// ī�� ��ȿ�Ⱓ(4)
	 unsigned char cardRno[13+1];		// �ֹ� ��� ��ȣ(13)
}WWCreditCard;				// Total Length : 39

typedef struct {
    	unsigned char pointNo[16+1];		// ����ī���ȣ
	unsigned char pointPwd[4+1]; 		// OKCashbag ��й�ȣ
}WWCardExtInfo;				// Total Length : 20

/*
 * Notice :
 * - WPKI function�� ��ť���Ʈ���� �����ϴ� ���� ����Ѵ�.
 */


/*
 * Declaration of Wallet Data Structure
 */
extern WWPersonalInfo *PersonInfo;
extern WWDeliveryInfo *DelInfo;
extern WWCreditCard *CardInfo;
extern WWCardExtInfo *CardExtInfo;


/*
 * PIN ���� ���� �ÿ� ��ȣȭ�Ǵ� ������
 * SHA1[PIN + ESN(Electronic Serial Number) + "Hash Verification"]
 * ESN Length : 4 byte
 */
extern unsigned char gHashedPIN[HASHED_LEN];		// ��ȣȭ�� PIN ���� ������ ��� ����


/*
* ���� �޸�(NVMemory)�� ����Ǵ� ��ȣȭ�� Wallet Data Variable
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
 * Wallet Data ��ȣȭ�� ���Ǵ� Parameters 
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
 * ����ڷκ��� �Է¹��� PIN�� ��ȣȭ�� ���� �����Ѵ�.
 * parameter:
 *		char *pin		4 char PIN Number		
 * return:
 *		char *hashedPIN;		4 Byte�� PIN Number
 */
extern unsigned char *wwSetHashedPIN(char *pin_in);

/*
 * ��ȣȭ�Ǿ� ����� PIN ���� �����´�.
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
 * �ܸ��⿡�� �Էµ� ������� PIN ���� �����Ѵ�. 
 *
 * Parameter :
 *		char *pin		4 char PIN Number		
 * Return :
 *		0 if verified else -1
 */
extern int wwVerifyPIN(char *pinVP);

/*
 * Wallet �����͸� ��/��ȣȭ�ϱ� ���� Secret Key�� �����Ѵ�.
 *
 * Parameter :
 *		char *pin			4 char PIN
 * Return :
 *		unsigned char *sKey			SecretKey
 */
extern unsigned char *wwGetSecretKey(char *pinGS);

/*
 * Wallet�� ������ PIN ������  �����Ѵ�.
 * 
 * Parameter :
 *      char *pinNew			���ο� PIN value 
 *      char *pinOld			����  PIN value 
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
 * ���� ������ �Է¹޾� ��ȣȭ�Ͽ� �����Ѵ�.�
 *
 * Parameter :
 *		WWPersonalInfo *ptr;		����� �Է� ���� ���� 
 *		char *pin;			4 char PIN 
 * 		int order;
 * Return :
 * 			void	
 */
//$$extern void wwSetPerInfo(WWPersonalInfo *ptrP, char *pinSP, int order);
extern void wwSetPerInfo(WWPersonalInfo *ptrP, int order);

/*
 * �ſ�ī�� ������ �Է¹޾� ��ȣȭ�Ͽ� �����Ѵ�. �
 *
 * Parameter :
 * 		WWCreditCard *ptr;			����� �Է� ī�� ����	
 *		char *pin;				4 char PIN 
 * 		int order;
 * Return :
 *		void
 */
extern void wwSetCardInfo(WWCreditCard *ptrC, char *pinSC, int order);

/*
 * ����� ������ �Է¹޾� ��ȣȭ�Ͽ� �����Ѵ�.
 *
 * Parameter :
 *		WWDeliveryInfo *ptr;		����� �Է� ����� ����
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *		void
 */
//$$extern void wwSetDelInfo(WWDeliveryInfo *ptrD, char *pinSD, int order);
extern void wwSetDelInfo(WWDeliveryInfo *ptrD, int order);

/*
 * Point ������ �Է¹޾� ��ȣȭ�Ͽ� �����Ѵ�.
 * 
 * Parameter : 
 *		WWCardExtInfo *ptr;		����� �Է� Point ����
 * 		char *pin;			4 char PIN 
 *		int order;
 * Return : 
 * 		void
 */
extern void wwSetCardExtInfo(WWCardExtInfo *ptrO, char *pinSO, int order);


/*
 * Wallet ������ ��ȣȭ�Ͽ� �����Ѵ�.
 *
 * Parameter :
 *		void *ptr;
 *		char *pin;			4 char PIN
 *		int order;			���� ���� ���� ����(����, ī��, �����)
 * Return :
 *		void 
 */
extern void wwSetData(void *ptrData, char *pinSD, int setOrd);

/*
 * Wallet�� ����� ����� ���� ������ ��ȣȭ�Ͽ� �о�´�.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *		WWPersonalInfo *ptr;	����� ���� ������ ��� �ִ� ������
 */
//$$extern void *wwGetPerInfo(char *pinGP, int ordGP);
extern void *wwGetPerInfo(int ordGP);

/*
 * Wallet�� ����� ����� ī��  ������ ��ȣȭ�Ͽ� �о�´�.
 *
 * Parameter : 
 *		char *pin;			4 char PIN 
 *		int order;
 * Return :
 *      WWCreditCard *ptr;    �����  ī�� ������ ��� �ִ� ������
 */
extern void *wwGetCCardInfo(char *pinGC, int ordGC);

/*
 * Wallet�� ����� ����� �����  ������ ��ȣȭ�Ͽ� �о�´�.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *      WWDeliveryInfo *ptr;    ����� �����  ������ ��� �ִ� ������
 */
//extern void *wwGetDelInfo(char *pinGD, int ordGD);
extern void *wwGetDelInfo(int ordGD);

/*
 * Wallet�� ����� ����� Point(OkCashBag) ������ ��ȣȭ�Ͽ� �о�´�.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *	WWCardExtInfo *ptr;	Point ������ ��� �ִ� ������
 */
extern void *wwGetCardExtInfo(char *pinGP, int ordGP);

/*
 * Wallet�� �ڷ��� �ش� �޸� ���� ������
 * �� ���·� ��ȣȭ�Ͽ�  �о� �´�.
 *
 * Parameter:
 *		char *pin			4 char PIN Number		
 *		int order;			���� ���� ���� ����(����, ī��, ����� ����)
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
