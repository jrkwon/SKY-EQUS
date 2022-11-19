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
 * »ç¿ëµÈ ÇØ½¬,¾ÏÈ£È­ ¾Ë°í¸®Áò
*/
#define	WALLET_ALG1			SHA1			//PIN °ËÁõ ¾Ë°í¸®Áò 
#define HASHED_LEN			SHA_DIGEST_LENGTH	//SHA1 ¾Ë°í¸®ÁòÀÇ Ãâ·Â ±æÀÌ
#define PER_FORMAT_SIZE		46+3			//a°³ÀÎÁ¤º¸ µ¥ÀÌÅÍ ÀüÃ¼ ±æÀÌ

//$$#define DEL_FORMAT_SIZE		124+5			//¹è¼ÛÁö µ¥ÀÌÅÍ ÀüÃ¼ ±æÀÌ
//$$ ¹è¼ÛÁö Á¦¸ñ ÇÊµå »èÁ¦µÊ
#define DEL_FORMAT_SIZE		108+4			//¹è¼ÛÁö µ¥ÀÌÅÍ ÀüÃ¼ ±æÀÌ

#define CARD_FORMAT_SIZE	39+4			//½Å¿ëÄ«µå µ¥ÀÌÅÍ ÀüÃ¼ ±æÀÌ
#define CardExt_FORMAT_SIZE		20+2			//POINT µ¥ÀÌÅÍ ÀüÃ¼ ±æÀÌ
#define PIN_LEN				4			//PIN ±æÀÌ

/*
 * Wallet Data Structure
 * - Person Information
 * - Delivery Informatiom
 * - CreditCard Information
 * - Point Information
 */

typedef struct {
	 unsigned char userName[10+1];		// »ç¿ëÀÚ ¸í(10)
	 unsigned char userPhone[12+1];		// Áö¿ª ¹øÈ£ Æ÷ÇÔ »ç¿ëÀÚ ÀüÈ­ ¹øÈ£ [4]-[4]-[4](12)
	 unsigned char userEmail[24+1];		// »ç¿ëÀÚ e_Mail(24)
}WWPersonalInfo;			// Total Length : 46

typedef struct {
//$$	 unsigned char dvTitle[16+1];		// ¹è¼ÛÁö Á¦¸ñ(16)
         unsigned char dvName[10+1];               // ¼ö·ÉÀÚ¸í(10)
         unsigned char dvPhone[12+1];              // ¼ö·ÉÁö ¿¬¶ô ¹øÈ£(12)
         unsigned char dvZip[6+1];                 // ¹è¼ÛÁö ¿ìÆí¹øÈ£(6)
         unsigned char dvAddr[80+1];               // ¹è¼ÛÁö ÁÖ¼Ò(80)
}WWDeliveryInfo;                       // Total Length : 124

typedef struct {
	 unsigned char cardNO[18+1];		// card number without '-'(18)
	 unsigned char cardPwd[4+1];		// Ä«µå ºñ¹Ğ¹øÈ£(4)
	 unsigned char cardValid[4+1];		// Ä«µå À¯È¿±â°£(4)
	 unsigned char cardRno[13+1];		// ÁÖ¹Î µî·Ï ¹øÈ£(13)
}WWCreditCard;				// Total Length : 39

typedef struct {
    	unsigned char pointNo[16+1];		// Á¦ÈŞÄ«µå¹øÈ£
	unsigned char pointPwd[4+1]; 		// OKCashbag ºñ¹Ğ¹øÈ£
}WWCardExtInfo;				// Total Length : 20

/*
 * Notice :
 * - WPKI functionÀº ½ÃÅ¥¾î½ºÇÁÆ®¿¡¼­ Á¦°øÇÏ´Â °ÍÀ» »ç¿ëÇÑ´Ù.
 */


/*
 * Declaration of Wallet Data Structure
 */
extern WWPersonalInfo *PersonInfo;
extern WWDeliveryInfo *DelInfo;
extern WWCreditCard *CardInfo;
extern WWCardExtInfo *CardExtInfo;


/*
 * PIN Á¤º¸ ÀúÀå ½Ã¿¡ ¾ÏÈ£È­µÇ´Â µ¥ÀÌÅÍ
 * SHA1[PIN + ESN(Electronic Serial Number) + "Hash Verification"]
 * ESN Length : 4 byte
 */
extern unsigned char gHashedPIN[HASHED_LEN];		// ¾ÏÈ£È­µÈ PIN °ü·Ã Á¤º¸¸¦ ´ã´Â º¯¼ö


/*
* ½ÇÁ¦ ¸Ş¸ğ¸®(NVMemory)¿¡ ÀúÀåµÇ´Â ¾ÏÈ£È­µÈ Wallet Data Variable
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
 * Wallet Data ¾ÏÈ£È­¿¡ »ç¿ëµÇ´Â Parameters 
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
 * »ç¿ëÀÚ·ÎºÎÅÍ ÀÔ·Â¹ŞÀº PINÀ» ¾ÏÈ£È­ÇÑ °ªÀ» ¸®ÅÏÇÑ´Ù.
 * parameter:
 *		char *pin		4 char PIN Number		
 * return:
 *		char *hashedPIN;		4 ByteÀÇ PIN Number
 */
extern unsigned char *wwSetHashedPIN(char *pin_in);

/*
 * ¾ÏÈ£È­µÇ¾î ÀúÀåµÈ PIN °ªÀ» °¡Á®¿Â´Ù.
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
 * ´Ü¸»±â¿¡¼­ ÀÔ·ÂµÈ »ç¿ëÀÚÀÇ PIN °ªÀ» °ËÁõÇÑ´Ù. 
 *
 * Parameter :
 *		char *pin		4 char PIN Number		
 * Return :
 *		0 if verified else -1
 */
extern int wwVerifyPIN(char *pinVP);

/*
 * Wallet µ¥ÀÌÅÍ¸¦ ¾Ï/º¹È£È­ÇÏ±â À§ÇÑ Secret Key¸¦ ¸®ÅÏÇÑ´Ù.
 *
 * Parameter :
 *		char *pin			4 char PIN
 * Return :
 *		unsigned char *sKey			SecretKey
 */
extern unsigned char *wwGetSecretKey(char *pinGS);

/*
 * Wallet¿¡ ¼³Á¤µÈ PIN Á¤º¸¸¦  º¯°æÇÑ´Ù.
 * 
 * Parameter :
 *      char *pinNew			»õ·Î¿î PIN value 
 *      char *pinOld			ÀÌÀü  PIN value 
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
 * °³ÀÎ Á¤º¸¸¦ ÀÔ·Â¹Ş¾Æ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù.Ğ
 *
 * Parameter :
 *		WWPersonalInfo *ptr;		»ç¿ëÀÚ ÀÔ·Â °³ÀÎ Á¤º¸ 
 *		char *pin;			4 char PIN 
 * 		int order;
 * Return :
 * 			void	
 */
//$$extern void wwSetPerInfo(WWPersonalInfo *ptrP, char *pinSP, int order);
extern void wwSetPerInfo(WWPersonalInfo *ptrP, int order);

/*
 * ½Å¿ëÄ«µå Á¤º¸¸¦ ÀÔ·Â¹Ş¾Æ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù. Ğ
 *
 * Parameter :
 * 		WWCreditCard *ptr;			»ç¿ëÀÚ ÀÔ·Â Ä«µå Á¤º¸	
 *		char *pin;				4 char PIN 
 * 		int order;
 * Return :
 *		void
 */
extern void wwSetCardInfo(WWCreditCard *ptrC, char *pinSC, int order);

/*
 * ¹è¼ÛÁö Á¤º¸¸¦ ÀÔ·Â¹Ş¾Æ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù.
 *
 * Parameter :
 *		WWDeliveryInfo *ptr;		»ç¿ëÀÚ ÀÔ·Â ¹è¼ÛÁö Á¤º¸
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *		void
 */
//$$extern void wwSetDelInfo(WWDeliveryInfo *ptrD, char *pinSD, int order);
extern void wwSetDelInfo(WWDeliveryInfo *ptrD, int order);

/*
 * Point Á¤º¸¸¦ ÀÔ·Â¹Ş¾Æ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù.
 * 
 * Parameter : 
 *		WWCardExtInfo *ptr;		»ç¿ëÀÚ ÀÔ·Â Point Á¤º¸
 * 		char *pin;			4 char PIN 
 *		int order;
 * Return : 
 * 		void
 */
extern void wwSetCardExtInfo(WWCardExtInfo *ptrO, char *pinSO, int order);


/*
 * Wallet Á¤º¸¸¦ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù.
 *
 * Parameter :
 *		void *ptr;
 *		char *pin;			4 char PIN
 *		int order;			Á¤º¸ À¯Çü ±¸ºĞ ÀÎÀÚ(°³ÀÎ, Ä«µå, ¹è¼ÛÁö)
 * Return :
 *		void 
 */
extern void wwSetData(void *ptrData, char *pinSD, int setOrd);

/*
 * Wallet¿¡ ÀúÀåµÈ »ç¿ëÀÚ °³ÀÎ Á¤º¸¸¦ º¹È£È­ÇÏ¿© ÀĞ¾î¿Â´Ù.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *		WWPersonalInfo *ptr;	»ç¿ëÀÚ °³ÀÎ Á¤º¸¸¦ ´ã°í ÀÖ´Â Æ÷ÀÎÅÍ
 */
//$$extern void *wwGetPerInfo(char *pinGP, int ordGP);
extern void *wwGetPerInfo(int ordGP);

/*
 * Wallet¿¡ ÀúÀåµÈ »ç¿ëÀÚ Ä«µå  Á¤º¸¸¦ º¹È£È­ÇÏ¿© ÀĞ¾î¿Â´Ù.
 *
 * Parameter : 
 *		char *pin;			4 char PIN 
 *		int order;
 * Return :
 *      WWCreditCard *ptr;    »ç¿ëÀÚ  Ä«µå Á¤º¸¸¦ ´ã°í ÀÖ´Â Æ÷ÀÎÅÍ
 */
extern void *wwGetCCardInfo(char *pinGC, int ordGC);

/*
 * Wallet¿¡ ÀúÀåµÈ »ç¿ëÀÚ ¹è¼ÛÁö  Á¤º¸¸¦ º¹È£È­ÇÏ¿© ÀĞ¾î¿Â´Ù.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *      WWDeliveryInfo *ptr;    »ç¿ëÀÚ ¹è¼ÛÁö  Á¤º¸¸¦ ´ã°í ÀÖ´Â Æ÷ÀÎÅÍ
 */
//extern void *wwGetDelInfo(char *pinGD, int ordGD);
extern void *wwGetDelInfo(int ordGD);

/*
 * Wallet¿¡ ÀúÀåµÈ »ç¿ëÀÚ Point(OkCashBag) Á¤º¸¸¦ º¹È£È­ÇÏ¿© ÀĞ¾î¿Â´Ù.
 *
 * Parameter : 
 *		char *pin;			4 char PIN
 *		int order;
 * Return :
 *	WWCardExtInfo *ptr;	Point Á¤º¸¸¦ ´ã°í ÀÖ´Â Æ÷ÀÎÅÍ
 */
extern void *wwGetCardExtInfo(char *pinGP, int ordGP);

/*
 * WalletÀÇ ÀÚ·áÁß ÇØ´ç ¸Ş¸ğ¸® ºí·°ÀÇ ³»¿ëÀ»
 * Æò¹® ÇüÅÂ·Î º¹È£È­ÇÏ¿©  ÀĞ¾î ¿Â´Ù.
 *
 * Parameter:
 *		char *pin			4 char PIN Number		
 *		int order;			Á¤º¸ À¯Çü ±¸ºĞ ÀÎÀÚ(°³ÀÎ, Ä«µå, ¹è¼ÛÁö Á¤º¸)
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
