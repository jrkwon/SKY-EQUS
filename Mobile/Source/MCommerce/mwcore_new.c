/*
 * File : mwcore.c
 * - Core Functions for Mobile Wallet
 *
 * Mobile Wallet Program 
 *
 * Copyright(c)2001 TGCORP Co. All rights reserved.
 * Writer : Kim, Hyejeong (lunadic@tgcorp.com)
 * Tabstop=4
 * Revision : 1.2.1
 */

#include "mwdef.h"
#include "ska.h"  //by yhlee
#include "nv.h"   // by yhlee

// by yhlee
STATIC nv_item_type  nvi;

/*
 *  general dynamic memory allocation
 *	Malloc() is provided by Crypto library
 */
void *wwMalloc( int sz )
{
	void	*ptr = NULL;
	ptr = Malloc(sz);
	if (ptr==NULL) {

		//MW¼öÁ¤ 
		//ERR_FATAL or something else should be inserted!!!
#ifdef BLOCK
		// this function or caller function must handle NULL value
		errorMessage("=== wwMalloc() : Fatal error - cannot allocate memory for operation.\n");
		// <Warning 1> exit;
		exit(EXIT_FAILURE);
#endif //#ifdef BLOCK
		return (void *)ptr;
	}else
	{
		//debugMessage("=== wwGetPerInfo() : Malloc successful \n");
		return (void *)ptr;
	}
}

/*
 * general dynamic memory free
 *	Free() is provided by Crypto library
 */
void wwFree(void *ptr)
{
	if (ptr!=NULL) Free(ptr);
}

/*
 * PINÀ» ¾ÏÈ£È­(Hash)ÇÏ¿© ¸®ÅÏÇÑ´Ù.
 * NV Memory 
 *
 * Parameter :
 *		char *pin			4 char PIN Number		
 * Return :
 * 		unsigned char *hashedPIN;		Hashed PIN(20 Bytes)
 */
unsigned char *wwSetHashedPIN(char *pin_in)
{
	static unsigned char Res[32];
	unsigned char src[32];

	// by yhlee
	unsigned char esn[9];
	unsigned char hash_string[18] = "hash verification";

//#ifdef BLOCK
	// ESN(Electronic Serial Number) => ÀÓ½Ã°ª ¼³Á¤
	(void) SKY_GetNV(NV_ESN_I,&nvi);
	sprintf((char *)esn,"%x",nvi.esn.esn);
	//esn = (unsigned char *)"1234";
	//hash_string = (unsigned char *)"hash verification";
	
	strcpy((char *)src, pin_in);
	strcat((char *)src, (char *)esn);
	strcat((char *)src, (char *)hash_string);
//#endif //#ifdef BLOCK
        
    WALLET_ALG1(src, strlen((const char *)src), Res);		//SHA1 
		
    return (unsigned char *)Res;
}

/*
 * ¾ÏÈ£È­(Hash)µÈ PIN °ªÀ» °¡Á®¿Â´Ù.	
 *
 * Parameter :
 * 			-
 * Return :
 *		unsigned char *hashedPIN;		Hashed PIN(20 Bytes)
 */
unsigned char *wwGetHashedPIN(void)
{
    
	MSG_HIGH("Just for removing warning ^^;",0,0,0); 

	if(gHashedPIN != NULL) return gHashedPIN;
}	

/*
 * ´Ü¸»±â¿¡¼­ ÀÔ·ÂµÈ »ç¿ëÀÚÀÇ PIN °ªÀ» °ËÁõÇÑ´Ù. 
 *
 * Parameter :
 *		char *pin		4 char PIN Number		
 * Return :
 *		0 if verified else -1
 */
int wwVerifyPIN(char *pinVP) 
{

	//´Ü¸»±â¿¡ ÀúÀåµÈ ¾ÏÈ£È­µÈ PIN Á¤º¸¿Í »ç¿ëÀÚÀÇ ÀÔ·Â Á¤º¸¸¦ ºñ±³ÇÑ´Ù.
	if (strncmp((const char *)wwSetHashedPIN(pinVP), (char *)wwGetHashedPIN(), HASHED_LEN)==0)
	{ 
		return 0;
	}else
	{
		errorMessage("=== wwVerifyPIN() : return -1\n"); 
		return -1;
	}
}

/*
 * Wallet µ¥ÀÌÅÍ¸¦ ¾Ï/º¹È£È­ÇÏ±â À§ÇÑ Secret Key¸¦ ¸®ÅÏÇÑ´Ù.
 *
 * Parameter :
 *              char *pin                       4 char PIN
 * Return :
 *              unsigned char *sKey                      SecretKey
 */
unsigned char *wwGetSecretKey(char *pinSK)
{
	static unsigned char skey[32];
	unsigned char src2[28];
//by yhlee
	BYTE_T esn[9];
	BYTE_T skey_string[18]="create secret key";
//	unsigned char *esn=NULL;
//	unsigned char *skey_string=NULL;

	(void) SKY_GetNV(NV_ESN_I,&nvi);
	sprintf((char *)esn,"%x",nvi.esn.esn);
//    esn = (unsigned char *)"1234";
//    skey_string= (unsigned char *)"create secret key";
    
    strcpy((char *)src2, pinSK);
    strcat((char *)src2, (char *)esn);
    strcat((char *)src2, (char *)skey_string);

    WALLET_ALG1(src2, strlen((char *)src2), skey);             //SHA1 
//#endif //#ifdef BLOCK
                    
    return (unsigned char *)skey;
}


/*
 * Wallet¿¡ ¼³Á¤µÈ PIN Á¤º¸¸¦  º¯°æÇÑ´Ù.
 * 
 * Parameter :
 *      char *pinNew			»õ·Î¿î PIN value 
 *      char *pinOld			ÀÌÀü  PIN value 
 * Return :
 *		0 if successful else -1
 */
int wwChangePIN(char *pinNew, char *pinOld) 
{
	//int i=0;
	//$$	WWPersonalInfo *tPerson;
	WWCreditCard *tCard;
	//$$	WWDeliveryInfo *tDel;
	WWCardExtInfo *tCardExt;
	
	// <Warning 2> implicit cast from (void *), C++ forbids
	//$$	tPerson = (WWPersonalInfo *)wwMalloc(PER_FORMAT_SIZE);
	tCard = (WWCreditCard *)wwMalloc(CARD_FORMAT_SIZE);
	//$$	tDel = (WWDeliveryInfo *)wwMalloc(DEL_FORMAT_SIZE);
	tCardExt = (WWCardExtInfo *)wwMalloc(CardExt_FORMAT_SIZE);
    	
	// ±âÁ¸ PIN °ËÁõ
   	if(wwVerifyPIN(pinOld) == 0)
	{
    	// »õ·Î¿î PINÀ» »ç¿ëÇÏ¿© ¾ÏÈ£È­µÈ PIN ÀúÀåÁ¤º¸(gHashedPIN) Àç¼³Á¤
		memcpy(gHashedPIN, (const void *)wwSetHashedPIN(pinNew), HASHED_LEN);
	    
    	// Wallet Á¤º¸ ·Îµù ÇÏ¿© Àç¾ÏÈ£È­ÇÏ¿© ÀúÀå
		//<Warning 3 > implicit cast from (void *), C++ forbids
		
		//$$	tPerson = (WWPersonalInfo *)wwGetData(pinOld, 0);
		//$$	wwSetData(tPerson, pinNew, 0);

		/*$$
		for(i=1; i<3; i++)
		{
			tDel = (WWDeliveryInfo *)wwGetData(pinOld, i);
			wwSetData(tDel, pinNew, i);
		}
		*/

		// by yhlee ÀúÀåµÈ µ¥ÀÌÅÍ°¡ ÀÖÀ» °æ¿ì¿¡¸¸ ¼öÇàÇÑ´Ù.
		if(SKY_GET_MW_WALLET_DATA_SAVED(3) == 1) {
			tCard = (WWCreditCard *)wwGetData(pinOld, 3);
			wwSetData(tCard, pinNew, 3);
			wwFree(CardInfo);
		} 
		if(SKY_GET_MW_WALLET_DATA_SAVED(4) == 1) {
			tCard = (WWCreditCard *)wwGetData(pinOld, 4);
			wwSetData(tCard, pinNew, 4);
			wwFree(CardInfo);
		} 
		if(SKY_GET_MW_WALLET_DATA_SAVED(5) == 1) {
			tCard = (WWCreditCard *)wwGetData(pinOld, 5);
			wwSetData(tCard, pinNew, 5);
			wwFree(CardInfo);
		} 

		if(SKY_GET_MW_WALLET_DATA_SAVED(6) == 1) {
			tCardExt = (WWCardExtInfo *)wwGetData(pinOld, 6);
			wwSetData(tCardExt, pinNew, 6);
			wwFree(CardInfo);
		} 

		/* commented by yhlee
		for(i=3; i<6; i++)
		{
			tCard = (WWCreditCard *)wwGetData(pinOld, i);
			wwSetData(tCard, pinNew, i);
		}

		tCardExt = (WWCardExtInfo *)wwGetData(pinOld, 6);
		wwSetData(tCardExt, pinNew, 6);
		*/
		/*$$	
		for(i=6; i<9; i++)
		{
			tCardExt = (WWCardExtInfo *)wwGetData(pinOld, i);
			wwSetData(tCardExt, pinNew, i);
		}
		*/	
		
		return 0;
	}else
    {
	    //debugMessage("wwChangePIN Test failure : pinOld : %s\n", pinOld);
		// PIN °ËÁõ ½ÇÆÐ
		return -1;
    }		
	
	//$$wwFree(tPerson);
	//$$wwFree(tDel);
	wwFree(tCard);
	wwFree(tCardExt);
}


/*
 * Encrypted wallet data size
 *
 *	Parameter :
 * 		int order
 *  Return :
 *      long size
 */

long wwEncDataSize(int ord) 
{
	switch(ord){
		case 0 : return seed_enc_block_size_with_padding(46);
		case 1 : return seed_enc_block_size_with_padding(124);
		case 2 : return seed_enc_block_size_with_padding(124);
		case 3 : return seed_enc_block_size_with_padding(39);
		case 4 : return seed_enc_block_size_with_padding(39);
		case 5 : return seed_enc_block_size_with_padding(39);
		case 6 : return seed_enc_block_size_with_padding(20);
		case 7 : return seed_enc_block_size_with_padding(20);
		case 8 : return seed_enc_block_size_with_padding(20);
	}
	//swjung
	//for removing warning
	return 0;
}


/*
 * Copy Array from source to destination
 *
 * Parameter :
 * 		unsigned char *dest			desstination array
 * 		int  dpos			offset of destination
 *		unsigned char *src			source array
 *		int  spos			offset of source
 *		int  len			copy length
 * Return :
 *		void
 */
void arrayCopy(unsigned char *dest, int dpos, unsigned char *src, int spos, int len)
{
	int i;

	for(i = 0; i<len; i++) 
		dest[i+dpos] = src[i+spos];
}		


/*****************************
 * »ç¿ëÀÚ ÀÔ·Â Á¤º¸ µî·Ï 
 ****************************/

/*
 * »ç¿ëÀÚ °³ÀÎ Á¤º¸¸¦ ÀÔ·Â¹Þ¾Æ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù.Ð
 *
 * Parameter :
 *		WWPersonalInfo *ptr;		»ç¿ëÀÚ ÀÔ·Â °³ÀÎ Á¤º¸ 
 *		char *pin					4 char PIN Number		
 *		int order
 * Return :
 * 			void	
 */
//$$void wwSetPerInfo(WWPersonalInfo *ptrData_, char *pinSP, int ordSP)
void wwSetPerInfo(WWPersonalInfo *ptrData_, int ordSP)
{
	unsigned char *concatePerson;
	//$$unsigned char gsession1[16];	
	//$$unsigned long int keySchedule1[32];
	
	concatePerson = (unsigned char *)wwMalloc(PER_FORMAT_SIZE);
	memset(concatePerson, '\0', PER_FORMAT_SIZE);		
	
	arrayCopy(concatePerson, 0, ptrData_->userName, 0, strlen((const char *)ptrData_->userName));
	arrayCopy(concatePerson, 11, ptrData_->userPhone, 0, strlen((const char *)ptrData_->userPhone));
	arrayCopy(concatePerson, 24, ptrData_->userEmail, 0, strlen((const char *)ptrData_->userEmail));

	//$$memcpy(gsession1, wwGetSecretKey(pinSP), 16);
	//$$seedSetKey(gsession1, keySchedule1);
	
	//$$dPerlen = seedEncryptCBC(concatePerson, encPerson, PER_FORMAT_SIZE, keySchedule1, dPeriv);
	
	memcpy(encPerson, (const void *)concatePerson, PER_FORMAT_SIZE);
	
	wwFree(concatePerson);
	//debugMessage("=== wwSetPerInfo() : <Person info Encryption>\n");
}

/*
 * »ç¿ëÀÚ ½Å¿ëÄ«µå Á¤º¸¸¦ ÀÔ·Â¹Þ¾Æ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù. Ð
 *
 * Parameter :
 * 		WWCreditCard *ptr;			»ç¿ëÀÚ ÀÔ·Â Ä«µå Á¤º¸	
 *		char *pin					4 char PIN Number		
 *		int order
 * Return :
 *		void
 */
void wwSetCardInfo(WWCreditCard *ptrC, char *pinSC, int ordSC)
{
	unsigned char *concateCard;
   	unsigned char gsession2[16];
	unsigned long int keySchedule2[32];

	concateCard = (unsigned char *)wwMalloc(CARD_FORMAT_SIZE);
	memset(concateCard, '\0', CARD_FORMAT_SIZE);

	arrayCopy(concateCard, 0, ptrC->cardNO, 0, strlen((const char *)ptrC->cardNO));
	arrayCopy(concateCard, 19, ptrC->cardPwd, 0, strlen((const char *)ptrC->cardPwd));
	arrayCopy(concateCard, 24, ptrC->cardValid, 0, strlen((const char *)ptrC->cardValid));
	arrayCopy(concateCard, 29, ptrC->cardRno, 0, strlen((const char *)ptrC->cardRno));
	
	memcpy(gsession2, (const void *)wwGetSecretKey(pinSC), 16);
   	seedSetKey(gsession2, keySchedule2);

	switch(ordSC){
		case 3 : dCardlen = seedEncryptCBC(concateCard, encCCard1, CARD_FORMAT_SIZE, keySchedule2, dCardiv);
// by yhlee
					SKY_SET_MW_WALLET_CARD(0,encCCard1,MW_WALLET_CARD_LENGTH);
					SKY_SET_MW_WALLET_DATA_SAVED(3,1);
					break;
		case 4 : dCardlen = seedEncryptCBC(concateCard, encCCard2, CARD_FORMAT_SIZE, keySchedule2, dCardiv);
// by yhlee
					SKY_SET_MW_WALLET_CARD(1,encCCard2,MW_WALLET_CARD_LENGTH);
					SKY_SET_MW_WALLET_DATA_SAVED(4,1);
					break;
		case 5 : dCardlen = seedEncryptCBC(concateCard, encCCard3, CARD_FORMAT_SIZE, keySchedule2, dCardiv);
// by yhlee
					SKY_SET_MW_WALLET_CARD(2,encCCard3,MW_WALLET_CARD_LENGTH);
					SKY_SET_MW_WALLET_DATA_SAVED(5,1);
					break;
	}

	// by yhlee
	SKY_SET_MW_WALLET_CARD_LENGTH(dCardlen);
	SKY_SET_MW_WALLET();
			 
	wwFree(concateCard);
   	//debugMessage("=== wwSetCardInfo() : <CreditCard info Encryption>\n");
}

/*
 * CardExt Á¤º¸¸¦ ÀÔ·Â ¹Þ¾Æ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù.
 *
 * Parameter : 
 *		WWCardExtInfo *ptr;
 *		char *pin;
 *		int order;
 * Return :
 *		void
 */
void wwSetCardExtInfo(WWCardExtInfo *ptrO, char *pinSO, int ordSO)
{
	unsigned char *concateCardExt;
	unsigned char gsession4[16];
	unsigned long int keySchedule4[32];
	
	concateCardExt = (unsigned char *)wwMalloc(CardExt_FORMAT_SIZE);
	memset(concateCardExt, '\0', CardExt_FORMAT_SIZE);
	
	arrayCopy(concateCardExt, 0, ptrO->pointNo, 0, strlen((const char *)ptrO->pointNo));
	arrayCopy(concateCardExt, 17, ptrO->pointPwd, 0, strlen((const char *)ptrO->pointPwd));
	
	memcpy(gsession4, (const void *)wwGetSecretKey(pinSO), 16);
	seedSetKey(gsession4, keySchedule4);
	
	dCardExtlen = seedEncryptCBC(concateCardExt, encCardExt1, CardExt_FORMAT_SIZE, keySchedule4, dCardExtiv);
	// by yhlee
	SKY_SET_MW_WALLET_OCB(encCardExt1,MW_WALLET_OCB_LENGTH);
	SKY_SET_MW_WALLET_DATA_SAVED(6,1);

	/*$$
	switch(ordSO){
		case 6: dCardExtlen = seedEncryptCBC(concateCardExt, encCardExt1, CardExt_FORMAT_SIZE, keySchedule4, dCardExtiv);
			break;
		case 7: dCardExtlen = seedEncryptCBC(concateCardExt, encCardExt2, CardExt_FORMAT_SIZE, keySchedule4, dCardExtiv); 
			break;
		case 8: dCardExtlen = seedEncryptCBC(concateCardExt, encCardExt3, CardExt_FORMAT_SIZE, keySchedule4, dCardExtiv);
			break;
	}
	*/
	// by yhlee
	SKY_SET_MW_WALLET_OCB_LENGTH(dCardExtlen);
	SKY_SET_MW_WALLET();
	
	wwFree(concateCardExt);
	//debugMessage("=== wwSetCardExtInfo() : <CardExt info Encryption>\n");
}

/*
 * »ç¿ëÀÚ ¹è¼ÛÁö Á¤º¸¸¦ ÀÔ·Â¹Þ¾Æ ¾ÏÈ£È­ÇÏ¿© ÀÚÀåÇÑ´Ù.
 *
 * Parameter :
 *		WWDeliveryInfo *ptr;		»ç¿ëÀÚ ÀÔ·Â ¹è¼ÛÁö Á¤º¸
 *		char *pin					4 char PIN Number		
 *		int order
 * Return :
 *		void
 */
//$$void wwSetDelInfo(WWDeliveryInfo *ptrD, char *pinSD, int ordSD)
void wwSetDelInfo(WWDeliveryInfo *ptrD, int ordSD)
{
	unsigned char *concateDel; 					//½ÇÁ¦ µ¥ÀÌÅÍ ÇÔÀÇ ±æÀÌ : DEL_FORMAT_SIZE 
	//$$unsigned char gsession3[16];
	//$$unsigned long int keySchedule3[32];
	
	concateDel = (unsigned char *)wwMalloc(DEL_FORMAT_SIZE);
	memset(concateDel, '\0', DEL_FORMAT_SIZE);

	//$$arrayCopy(concateDel, 0, ptrD->dvTitle, 0, strlen(ptrD->dvTitle));
	//$$arrayCopy(concateDel, 17, ptrD->dvName, 0, strlen(ptrD->dvName));
	//$$arrayCopy(concateDel, 28, ptrD->dvPhone, 0, strlen(ptrD->dvPhone));
	//$$arrayCopy(concateDel, 41, ptrD->dvZip, 0, strlen(ptrD->dvZip));
	//$$arrayCopy(concateDel, 48, ptrD->dvAddr, 0, strlen(ptrD->dvAddr));

	arrayCopy(concateDel, 0, ptrD->dvName, 0, strlen((const char *)ptrD->dvName));
	arrayCopy(concateDel, 11, ptrD->dvPhone, 0, strlen((const char *)ptrD->dvPhone));
	arrayCopy(concateDel, 24, ptrD->dvZip, 0, strlen((const char *)ptrD->dvZip));
	arrayCopy(concateDel, 31, ptrD->dvAddr, 0, strlen((const char *)ptrD->dvAddr));
	
	//$$memcpy(gsession3, wwGetSecretKey(pinSD), 16);
	//$$seedSetKey(gsession3, keySchedule3);
	
	/*$$
	switch(ordSD){
		case 1 : dDellen = seedEncryptCBC(concateDel, encDel1, DEL_FORMAT_SIZE, keySchedule3, dDeliv);		
			 break;
		case 2 : dDellen = seedEncryptCBC(concateDel, encDel2, DEL_FORMAT_SIZE, keySchedule3, dDeliv);		
			 break;
	}
	*/
	switch(ordSD){
		case 1 : memcpy(encDel1, (const void *)concateDel, DEL_FORMAT_SIZE);
			break;
		case 2 : memcpy(encDel2, (const void *)concateDel, DEL_FORMAT_SIZE);
	}

	wwFree(concateDel);
	//debugMessage("=== wwSetDelInfo() : <Delivery info Encryption>\n");
}


/*
 * Wallet Á¤º¸¸¦ ¾ÏÈ£È­ÇÏ¿© ÀúÀåÇÑ´Ù.
 *
 * Parameter :
 *		void *ptr;
 *		char *pin			4 char PIN Number		
 *		int order;			Á¤º¸ À¯Çü ±¸ºÐ ÀÎÀÚ(°³ÀÎ, Ä«µå, ¹è¼ÛÁö)
 * Return :
 *		void 
 */
void wwSetData(void *ptrData, char *pinSD, int setOrd)
{

    // Check pin for not NULL
	if(pinSD != NULL){
		switch(setOrd){
			
			////////////////////////////////////////////////////////////////////////
			//swjung type casting
			////////////////////////////////////////////////////////////////////////
			//$$case 0	:  wwSetPerInfo(ptrData, pinSD, setOrd);
			case 0	:  wwSetPerInfo((WWPersonalInfo *)ptrData, setOrd);
				break;
			//$$case 1	:  wwSetDelInfo(ptrData, pinSD, setOrd);
			case 1	:    wwSetDelInfo((WWDeliveryInfo *)ptrData, setOrd);
				break;
			//$$case 2	:  wwSetDelInfo(ptrData, pinSD, setOrd);
			case 2	:  wwSetDelInfo((WWDeliveryInfo *)ptrData, setOrd);
				break;
			case 3	:  wwSetCardInfo((WWCreditCard *)ptrData, pinSD, setOrd);
				break;
			case 4	:  wwSetCardInfo((WWCreditCard *)ptrData, pinSD, setOrd);
				break;
			case 5  :  wwSetCardInfo((WWCreditCard *)ptrData, pinSD, setOrd);
				break;
			case 6	:  wwSetCardExtInfo((WWCardExtInfo *)ptrData, pinSD, setOrd);
				break;
					   
			//$$case 7	:  wwSetCardExtInfo(ptrData, pinSD, setOrd);
			//$$case 8	:  wwSetCardExtInfo(ptrData, pinSD, setOrd);

			default :
				//debugMessage("=== wwSetData() : Invalid Number : %d\n", setOrd);	 
				// TODO: Warning Error control
				break;
		}
	}
	else {
		//errorMessage("=== wwSetData() : Invalid PIN(Null)\n");
		// TODO: Error Control
	}
}	


/*
 * Wallet¿¡ ÀúÀåµÈ »ç¿ëÀÚ °³ÀÎ Á¤º¸¸¦ º¹È£È­ÇÏ¿© ÀÐ¾î¿Â´Ù.
 *
 * Parameter : 
 *		char *pin			4 char PIN Number		
 *		int order
 * Return :
 *		WWPersonalInfo *ptr;	»ç¿ëÀÚ °³ÀÎ Á¤º¸¸¦ ´ã°í ÀÖ´Â Æ÷ÀÎÅÍ
 */
//$$void *wwGetPerInfo(char *pinGP, int ordGP)
void *wwGetPerInfo(int ordGP)
{
	//unsigned char *decPerson;
	//$$unsigned char blockP[16];					
	//$$unsigned long int keyScheduleP[32];

	// by yhlee
	WWPersonalInfo PersonInfo;
	//PersonInfo = (WWPersonalInfo *)wwMalloc(PER_FORMAT_SIZE);

	//$$decPerson = wwMalloc(PER_FORMAT_SIZE);	
	//$$memset(decPerson, '\0', PER_FORMAT_SIZE);	

	//$$memcpy(blockP, wwGetSecretKey(pinGP), 16);
	//$$seedSetKey(&blockP, keyScheduleP);
	
	//$$seedDecryptCBC(encPerson, decPerson, dPerlen, keyScheduleP, dPeriv);
	
	/*$$
	arrayCopy(PersonInfo->userName, 0, decPerson, 0, 11);
	arrayCopy(PersonInfo->userPhone, 0, decPerson, 11, 13);
	arrayCopy(PersonInfo->userEmail, 0, decPerson, 24, 25);
	*/
	/* by yhlee
	arrayCopy(PersonInfo->userName, 0, encPerson, 0, 11);
	arrayCopy(PersonInfo->userPhone, 0, encPerson, 11, 13);
	arrayCopy(PersonInfo->userEmail, 0, encPerson, 24, 25); */
	arrayCopy(PersonInfo.userName, 0, encPerson, 0, 11);
	arrayCopy(PersonInfo.userPhone, 0, encPerson, 11, 13);
	arrayCopy(PersonInfo.userEmail, 0, encPerson, 24, 25); 
	
	//debugMessage("=== wwGetPerInfo() : <Personal Info Decryption>\n");
	//$$wwFree(decPerson);

	// by yhlee
	//return PersonInfo;
	return (&PersonInfo);
}



//swjung type paraphrasing "DWORD" to "unsigned long int"

/*
 * Wallet¿¡ ÀúÀåµÈ »ç¿ëÀÚ Ä«µå  Á¤º¸¸¦ º¹È£È­ÇÏ¿© ÀÐ¾î¿Â´Ù.
 *
 * Parameter : 
 *		char *pin			4 char PIN Number		
 *		int order
 * Return :
 *      WWCreditCard *ptr;    »ç¿ëÀÚ  Ä«µå Á¤º¸¸¦ ´ã°í ÀÖ´Â Æ÷ÀÎÅÍ
 */
void *wwGetCCardInfo(char *pinGC, int ordGC)
{
	unsigned char *decCard;
	unsigned char blockC[16];
	unsigned long int keyScheduleC[32];

	// by yhlee
	WWCreditCard CardInfo;
	//CardInfo = (WWCreditCard *)wwMalloc(CARD_FORMAT_SIZE);

	decCard = (unsigned char *)wwMalloc(CARD_FORMAT_SIZE);
	memset(decCard, '\0', CARD_FORMAT_SIZE);

	memcpy(blockC, (const void *)wwGetSecretKey(pinGC), 16);
	seedSetKey(blockC, keyScheduleC);

	// by yhlee
	dCardlen = SKY_GET_MW_WALLET_CARD_LENGTH();

	switch(ordGC){
	    case 3 :
			memcpy(encCCard1,  (void *)SKY_GET_MW_WALLET_CARD(0), MW_WALLET_CARD_LENGTH);
			seedDecryptCBC(encCCard1, decCard, dCardlen, keyScheduleC, dCardiv);
			break;
	    case 4 :
			memcpy(encCCard2,  (void *)SKY_GET_MW_WALLET_CARD(1), MW_WALLET_CARD_LENGTH);
			seedDecryptCBC(encCCard2, decCard, dCardlen, keyScheduleC, dCardiv);
			break;
	    case 5 : 
			memcpy(encCCard3,  (void *)SKY_GET_MW_WALLET_CARD(2), MW_WALLET_CARD_LENGTH);
			seedDecryptCBC(encCCard3, decCard, dCardlen, keyScheduleC, dCardiv);
			break;
	}

	// by yhlee
	/*arrayCopy(CardInfo->cardNO, 0, decCard, 0, 19);
	arrayCopy(CardInfo->cardPwd, 0, decCard, 19, 5);
	arrayCopy(CardInfo->cardValid, 0, decCard, 24, 5);
	arrayCopy(CardInfo->cardRno, 0, decCard, 29, 14); */
	arrayCopy(CardInfo.cardNO, 0, decCard, 0, 19);
	arrayCopy(CardInfo.cardPwd, 0, decCard, 19, 5);
	arrayCopy(CardInfo.cardValid, 0, decCard, 24, 5);
	arrayCopy(CardInfo.cardRno, 0, decCard, 29, 14);

	//debugMessage("=== wwGetCCardInfo() : <CreditCard Info decryption>\n");
	wwFree(decCard);

	// by yhlee
	//return CardInfo;
	return (&CardInfo);
}



/*
 * Wallet¿¡ ÀúÀåµÈ »ç¿ëÀÚ ¹è¼ÛÁö  Á¤º¸¸¦ º¹È£È­ÇÏ¿© ÀÐ¾î¿Â´Ù.
 *
 * Parameter : 
 *		char *pin			4 char PIN Number		
 *		int order
 * Return :
 *      WWDeliveryInfo *ptr;    »ç¿ëÀÚ ¹è¼ÛÁö  Á¤º¸¸¦ ´ã°í ÀÖ´Â Æ÷ÀÎÅÍ
 */
//$$void *wwGetDelInfo(char *pinGD, int ordGD)
void *wwGetDelInfo(int ordGD)
{
	//$$unsigned char *decDel;
	unsigned char *rawDel;
	//$$unsigned char blockD[16];
	//$$unsigned long int keyScheduleD[32];

	// by yhlee 2001.10.19
	WWDeliveryInfo DelInfo;
	//DelInfo = (WWDeliveryInfo *)wwMalloc(DEL_FORMAT_SIZE);

	//$$decDel = wwMalloc(DEL_FORMAT_SIZE);
	rawDel = (unsigned char *)wwMalloc(DEL_FORMAT_SIZE);
	//$$memset(decDel, '\0', DEL_FORMAT_SIZE);
	memset(rawDel, '\0', DEL_FORMAT_SIZE);

	//$$memcpy(blockD, wwGetSecretKey(pinGD), 16);
	//$$seedSetKey(blockD, keyScheduleD);

	/*$$
	switch(ordGD){
		case 1 : seedDecryptCBC(encDel1, decDel, dDellen, keyScheduleD, dDeliv);
			 break;
		case 2 : seedDecryptCBC(encDel2, decDel, dDellen, keyScheduleD, dDeliv);
			 break;
	}
	*/
	/*$$
	arrayCopy(DelInfo->dvTitle, 0, decDel, 0, 17);
	arrayCopy(DelInfo->dvName, 0, decDel, 17, 11);
	arrayCopy(DelInfo->dvPhone, 0, decDel, 28, 13);
	arrayCopy(DelInfo->dvZip, 0, decDel, 41, 7);
	arrayCopy(DelInfo->dvAddr, 0, decDel, 48, 81);
	*/

	switch(ordGD){
		case 1 : memcpy(rawDel, (const void *)encDel1, DEL_FORMAT_SIZE);
			break;
		case 2 : memcpy(rawDel, (const void *)encDel2, DEL_FORMAT_SIZE);
			break;
	}		 
				 
	// by yhlee
/*	arrayCopy(DelInfo->dvName, 0, rawDel, 0, 11);
	arrayCopy(DelInfo->dvPhone, 0, rawDel, 11, 13);
	arrayCopy(DelInfo->dvZip, 0, rawDel, 24, 7);
	arrayCopy(DelInfo->dvAddr, 0, rawDel, 31, 81); */
	arrayCopy(DelInfo.dvName, 0, rawDel, 0, 11);
	arrayCopy(DelInfo.dvPhone, 0, rawDel, 11, 13);
	arrayCopy(DelInfo.dvZip, 0, rawDel, 24, 7);
	arrayCopy(DelInfo.dvAddr, 0, rawDel, 31, 81); 

	
	//debugMessage("=== wwGetDelInfo(): <Delivery Info Decryption>\n");
	wwFree(rawDel);

	// by yhlee
	//return DelInfo;
	return (&DelInfo);
}

/*
 * Wallet¿¡ ÀúÀåµÈ »ç¿ëÀÚ CardExt Á¤º¸¸¦ º¹È£È­ÇÏ¿© ÀÐ¾î¿Â´Ù.
 *
 * Parameter : 
 *              char *pin                       4 char PIN Number               
 *		int order
 * Return :
 *      WWDeliveryInfo *ptr;    »ç¿ëÀÚ ¹è¼ÛÁö  Á¤º¸¸¦ ´ã°í ÀÖ´Â Æ÷ÀÎÅÍ
 */
void *wwGetCardExtInfo(char *pinGO, int ordGO)
{
	unsigned char *decCardExt;
	unsigned char blockO[16];
	unsigned long int keyScheduleO[32];

	// by yhlee 2001.10.19
	WWCardExtInfo CardExtInfo;
	//CardExtInfo = (WWCardExtInfo *)wwMalloc(CardExt_FORMAT_SIZE);

	decCardExt = (unsigned char *)wwMalloc(CardExt_FORMAT_SIZE);
	memset(decCardExt, '\0', CardExt_FORMAT_SIZE);
	
	memcpy(blockO, (const void *)wwGetSecretKey(pinGO), 16);
	seedSetKey(blockO, keyScheduleO);

	// by yhlee
	dCardExtlen = SKY_GET_MW_WALLET_OCB_LENGTH();
	
	/*$$
	switch(ordGO){
		case 6 : seedDecryptCBC(encCardExt1, decCardExt, dCardExtlen, keyScheduleO, dCardExtiv);
			break;
		case 7 : seedDecryptCBC(encCardExt2, decCardExt, dCardExtlen, keyScheduleO, dCardExtiv);
			break;
		case 8 : seedDecryptCBC(encCardExt3, decCardExt , dCardExtlen, keyScheduleO, dCardExtiv);
			break;
	}
	*/
	memcpy(encCardExt1,  (void *)SKY_GET_MW_WALLET_OCB(), MW_WALLET_OCB_LENGTH);
	seedDecryptCBC(encCardExt1, decCardExt, dCardExtlen, keyScheduleO, dCardExtiv);
	
	//arrayCopy(CardExtInfo->pointNo, 0, decCardExt, 0, 17);
	//arrayCopy(CardExtInfo->pointPwd, 0, decCardExt, 17, 5);
	arrayCopy(CardExtInfo.pointNo, 0, decCardExt, 0, 17);
	arrayCopy(CardExtInfo.pointPwd, 0, decCardExt, 17, 5);
	
	//debugMessage("=== wwGetCardExtInfo() : <CardExt Info Decryption>\n");
	wwFree(decCardExt);
	
	//return CardExtInfo;
	return (&CardExtInfo);
}

		
	
/*
 * WalletÀÇ ÀÚ·áÁß ÇØ´ç ¸Þ¸ð¸® ºí·°ÀÇ ³»¿ëÀ»
 * Æò¹® ÇüÅÂ·Î º¹È£È­ÇÏ¿©  ÀÐ¾î ¿Â´Ù.
 *
 * Parameter:
 *		char *pin			4 char PIN Number		
 *      int order;			Á¤º¸ À¯Çü ±¸ºÐ ÀÎÀÚ(°³ÀÎ, Ä«µå, ¹è¼ÛÁö Á¤º¸)
 * Return:
 *      void *;
*/
void *wwGetData(char *pinGD, int getOrd) 
{

	///////////////////////////////////////////////////////////////////////////
	//swjung MW¼öÁ¤
	//Insert break statement
	//If statement must have one more return statement because control may not 
	//go through switch statement
	///////////////////////////////////////////////////////////////////////////
    // Check pin for not NULL
	if(pinGD != NULL)
	{	
		switch(getOrd){
        		//$$case  0 : return wwGetPerInfo(pinGD, getOrd);
        		case  0 : return (WWPersonalInfo *)wwGetPerInfo(getOrd);

				  
        		//$$case  1 : return wwGetDelInfo(pinGD, getOrd);
        		case  1 : return (WWDeliveryInfo *)wwGetDelInfo(getOrd);

        		//$$case  2 : return wwGetDelInfo(pinGD, getOrd);
        		case  2 : return (WWDeliveryInfo *)wwGetDelInfo(getOrd);
				  
        		case  3	: return (WWCreditCard *)wwGetCCardInfo(pinGD, getOrd);
        		case  4 : return (WWCreditCard *)wwGetCCardInfo(pinGD, getOrd);
        		case  5 : return (WWCreditCard *)wwGetCCardInfo(pinGD, getOrd);
				case  6 : return (WWCardExtInfo *)wwGetCardExtInfo(pinGD, getOrd);
				//$$case  7 : return wwGetCardExtInfo(pinGD, getOrd);
				//$$case  8 : return wwGetCardExtInfo(pinGD, getOrd);
				default : 
				//debugMessage("=== wwGetData(): Invalid order number %d\n", getOrd);
				// TODO : Warning error Control
				break;
        }
	}else
	{
		errorMessage("=== wwGetData() : Error -  Invalid PIN(Null)\n");
		// TODO : Error Control
		return (void *)NULL;
	}
}  
