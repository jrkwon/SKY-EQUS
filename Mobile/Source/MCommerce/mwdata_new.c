/*
 * File : mwdata.c
 * - Global DATA for Mobile wallet
 *
 * Mobile Wallet Program 
 *
 * Copyright(c)2001 TGCORP Co. All rights reserved.
 * Writer : Kim, Hyejeong (lunadic@tgcorp.com)
 * Tabstop=4
 * Revision : 1.2.1
 */

#include "mwdef.h"

/************************************
 * NVRAM Data Section
 ************************************
 */
unsigned char gHashedPIN[HASHED_LEN];		//SHA1(PIN)

/*
* 실제 메모리에 저장되는 암호화된 Wallet Data Variable
*/
unsigned char encPerson[56];				//Encrypted Person Data with padding
unsigned char encDel1[136];				//Encrypted Delivery Data with padding
unsigned char encDel2[136];				//Encrypted Delivery Data with padding
unsigned char encCCard1[48];				//Encrypted CreditCard Data with padding
unsigned char encCCard2[48];				//Encrypted CreditCard Data with padding
unsigned char encCCard3[48];				//Encrypted CreditCard Data with padding
unsigned char encCardExt1[32];				//Encrypted CardExt Data with padding 
//!!unsigned char encCardExt2[32];				//Encrypted CardExt Data with padding 
//!!unsigned char encCardExt3[32];				//Encrypted CardExt Data with padding 

/************************************
 * Wallet Global Data Section
 ************************************
 */
long	dPerlen=0, dCardlen=0, dDellen=0, dCardExtlen=0;			//Encrypted Data Length
//!!unsigned char	dPeriv[16], dCardiv[16], dDeliv[16], dCardExtiv[16]; 		//Random initial Vector in SEED
unsigned char	dCardiv[16], dCardExtiv[16]; 		//Random initial Vector in SEED

// Wallet Data Access Pointers
WWPersonalInfo *PersonInfo;
WWCreditCard *CardInfo;
WWDeliveryInfo *DelInfo;
WWCardExtInfo *CardExtInfo;

void remove_warning(void)
{
	MSG_HIGH("Just for removing warning ^^;",0,0,0); 
}


