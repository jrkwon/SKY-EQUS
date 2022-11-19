#include <stdio.h>
#include <stdlib.h>

#include "mwdef.h"
#include "mwscript.h"


static int personIndex = 0;
static int delIndex = 1;
static int cardIndex = 3;
static int cardExtIndex = 6;
static int accIndex = 9;

/** ToBeUnsigned_PlainWalletData  **/
unsigned char *generateUnsignedWD(unsigned char *pAppendStr, unsigned int toEnc)
{
    unsigned char   *pTemp;
    int    offset;
	///////////////////////////////////////////////////////////
	//swjung
	//Remove warning
    //unsigned int  contentEncoding;
	unsigned char *temp = NULL;
	///////////////////////////////////////////////////////////
    unsigned int  lenAppendString=0;
    int     lenUnsign;
	unsigned int len;

	pTemp = (unsigned char *)wwMalloc(1024);

    offset = 2;
    pTemp[offset++] = MW_VERSION;
    pTemp[offset++] = TEXT;
    pTemp[offset++] = ((KSC_5601>>8) | 0x80);
    pTemp[offset++] = KSC_5601;
	//Added by swjung
	pTemp[offset++]= ((curfield.unsignedWD>>8) | 0x80);
	pTemp[offset++] = curfield.unsignedWD;

    if(toEnc & MASK_Append_Info)
    {
        if(curfield.unsignedWD & MASK_Append_Info)
        {
            lenAppendString = strlen((const char *)pAppendStr);
            pTemp[offset++] = lenAppendString>>8 | 0x80;
            pTemp[offset++] = lenAppendString;
            COPYTO(pTemp, offset, pAppendStr, lenAppendString);
			//Added by swjung
			memcpy(&pTemp[offset++], DELIMITER, 1);
        }
    }
    if(toEnc & MASK_Per_Info)
    {
        if(isAccess.personal)
        {
            if(curfield.unsignedWD & MASK_Per_Info)
            {
                temp = getMWPersonal(personIndex);
				len = (unsigned int)temp[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp[1];
                COPYTO(pTemp, offset, temp, len);
				free(temp);
            }
        }else
        {
			goto error1;
        }
    }
    if(toEnc & MASK_Del_Info)
    {
        if(isAccess.delivery)
        {
            if(curfield.unsignedWD & MASK_Del_Info)
            {
                temp = getMWDelivery(delIndex);
				len = (unsigned int)temp[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp[1];
                COPYTO(pTemp, offset, temp, len);
				free(temp);
            }
        }else
        {
			goto error1;
        }
    }
    if(toEnc & MASK_Card_Info)
    {
        if(isAccess.card)
        {
            if(curfield.unsignedWD & MASK_Card_Info)
            {
                temp = getMWCard(pWalletPIN, cardIndex);
				len = (unsigned int)temp[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp[1];
                COPYTO(pTemp, offset, temp, len);
				free(temp);
            }
        }else
        {
			goto error1;
        }
    }
    if(toEnc & MASK_Account_Info)
    {
        if(isAccess.account)
        {
            if(curfield.unsignedWD & MASK_Account_Info)
            {
                temp = getMWAccount(pWalletPIN, accIndex);
				len = (unsigned int)temp[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp[1];
                COPYTO(pTemp, offset, temp, len);
				free(temp);
            }
        }else
        {
			goto error1;
        }
    }
    if(toEnc & MASK_CardExt_Info)
    {
        if(isAccess.cardext)
        {
            if(curfield.unsignedWD & MASK_CardExt_Info)
            {
                temp = getMWCardExt(pWalletPIN, cardExtIndex);
				len = (unsigned int)temp[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp[1];
                COPYTO(pTemp, offset, temp, len);
				free(temp);
            }
        }else
        {
			goto error1;
        }
    }

    lenUnsign= offset;
    pTemp[0] = lenUnsign>>8;
    pTemp[1] = lenUnsign;

    return pTemp;

error1:
	if(pTemp) free(pTemp);
	//Substituted by swjung
	if(temp) free(temp);

    errorNO = 1;
    return NULL;

}

/** ToBeSigned_PlainWalletData 생성  **/
unsigned char *generateSignedWD(unsigned char *pAppendStr_, unsigned int toEnc_)
{
    unsigned char   *pTemp;
    int     offset;
    int     lenSign;
    ///////////////////////////////////////////////////////////
	//swjung
	//Remove warning
    //unsigned int  contentEncoding;
	unsigned char *temp_ = NULL;
	///////////////////////////////////////////////////////////
    unsigned int  lenAppendString=0;
	//unsigned char *temp_;
	unsigned int len;

	pTemp = (unsigned char *)wwMalloc(1024);

    offset = 2;
    pTemp[offset++] = MW_VERSION;
    pTemp[offset++] = TEXT;
    pTemp[offset++] = ((KSC_5601>>8) | 0x80);
    pTemp[offset++] = KSC_5601;

	//Added by swjung
	pTemp[offset++] = ((curfield.signedWD>>8) | 0x80);
	pTemp[offset++] = curfield.signedWD;


    if(toEnc_ & MASK_Append_Info)
    {
        if(curfield.signedWD & MASK_Append_Info)
        {
            // TODO: Display Append Info

            lenAppendString = strlen((const char *)pAppendStr_);
            pTemp[offset++] = lenAppendString>>8 | 0x80;
            pTemp[offset++] = lenAppendString;
            COPYTO(pTemp, offset, pAppendStr_, lenAppendString);
			//Added by swjung
			memcpy(&pTemp[offset++], DELIMITER, 1);
        }
    }
   if(toEnc_ & MASK_Per_Info)
    {
        if(isAccess.personal)
        {
            if(curfield.signedWD & MASK_Per_Info)
            {
                temp_ = getMWPersonal(personIndex);
				len = (unsigned int)temp_[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp_[1];
                COPYTO(pTemp, offset, temp_, len);
				free(temp_);
            }
        }else
        {	
			goto error2;
        }
    }
    if(toEnc_ & MASK_Del_Info)
    {
        if(isAccess.delivery)
        {
            if(curfield.signedWD & MASK_Del_Info)
            {
                temp_ = getMWDelivery(delIndex);
				len = (unsigned int)temp_[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp_[1];
                COPYTO(pTemp, offset, temp_, len);
				free(temp_);
            }
        }else
        {
			goto error2;
        }
    }
    if(toEnc_ & MASK_Card_Info)
    {
        if(isAccess.card)
        {
            if(curfield.signedWD & MASK_Card_Info)
            {
                temp_ = getMWCard(pWalletPIN, cardIndex);
				len = (unsigned int)temp_[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp_[1];
                COPYTO(pTemp, offset, temp_, len);
				free(temp_);
            }
        }else
        {
			goto error2;
        }
    }
    if(toEnc_ & MASK_Account_Info)
    {
        if(isAccess.account)
        {
            if(curfield.signedWD & MASK_Account_Info)
            {
                temp_ = getMWAccount(pWalletPIN, accIndex);
				len = (unsigned int)temp_[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp_[1];
                COPYTO(pTemp, offset, temp_, len);
				free(temp_);
            }
        }else
        {
			goto error2;
        }
    }
    if(toEnc_ & MASK_CardExt_Info)
    {
        if(isAccess.cardext)
        {
            if(curfield.signedWD & MASK_CardExt_Info)
            {
                temp_ = getMWCardExt(pWalletPIN, cardExtIndex);
				len = (unsigned int)temp_[0];
				len = (len<<8 & 0x7f00) + (unsigned int)temp_[1];
                COPYTO(pTemp, offset, temp_, len);
				free(temp_);
            }
        }else
        {
			goto error2;
        }
    }

    lenSign = offset;
    pTemp[0] = lenSign>>8;
    pTemp[1] = lenSign;

    return pTemp;

error2:
	if(pTemp) free(pTemp);
	if(temp_) free(temp_);
	
    errorNO = 1;
    return NULL;
}

/**
 * Terminal에 저장된 Wallet 개인정보를 리턴한다.
 *
 *  Parameter :
 *              int Order;
 *  Return :
 *              Unsigned char *mwPersonInfo;
 *
 */
unsigned char *getMWPersonal(int perOrder)
{
    WWPersonalInfo  *pPersonalInfo=NULL;    // 개인정보 저장 공간
    unsigned char *personalData;            // 개인정보 리턴 데이터
    int     pfield1, pfield2, pfield3;      // String length for Person fields
    int     nOffset=0;
    unsigned int    lenPersonal=0;

    pPersonalInfo = (WWPersonalInfo  *)wwGetPerInfo(perOrder);

    pfield1 = strlen((const char *)pPersonalInfo->userName);
    pfield2 = strlen((const char *)pPersonalInfo->userPhone);
    pfield3 = strlen((const char *)pPersonalInfo->userEmail);

    /* length of person data, [each field + DELIMITER + length size] */
    lenPersonal = pfield1+pfield2+pfield3+12+2;

    personalData = (unsigned char *)wwMalloc(lenPersonal);

    /* Personal Info length(2byte) */
    nOffset = 0;
    personalData[nOffset++] = (lenPersonal>>8)|0x80;
    personalData[nOffset++] = lenPersonal;

    /* Basic Field : 사용자 이름*/
    COPYTO(personalData, nOffset, "u1=", 3);
    COPYTO(personalData, nOffset, pPersonalInfo->userName, pfield1);
    memcpy(&personalData[nOffset++], DELIMITER, 1);

    /* Basic Field : 사용자 전화번호 */
    COPYTO(personalData, nOffset, "u2=", 3);
    COPYTO(personalData, nOffset, pPersonalInfo->userPhone, pfield2);
    memcpy(&personalData[nOffset++], DELIMITER, 1);

    /* Extension Field : 이메일 */
    COPYTO(personalData, nOffset, "u3=", 3);
    COPYTO(personalData, nOffset, pPersonalInfo->userEmail, pfield3);
    memcpy(&personalData[nOffset++], DELIMITER, 1);

    free(pPersonalInfo);

    return personalData;
}


/**
 * Terminal에 저장된 Wallet 배송지 정보를 리턴한다.
 *
 *  Parameter :
 *              int Order;
 *  Return :
 *              Unsigned char *mwDeliveryInfo;
 *
 */
unsigned char *getMWDelivery(int delOrder)
{

    WWDeliveryInfo  *pDelInfo = NULL;       // 배송지 정보 저장 공간
    unsigned char *deliveryData=NULL;       // 배송지 정보 리턴 데이터
    int dfield1, dfield2, dfield3, dfield4; // String length for Delivery fields
    int nOffset=0;
    unsigned int    lenDelivery=0;

    pDelInfo = (WWDeliveryInfo  *)wwGetDelInfo(delOrder);

    dfield1 = strlen((const char *)pDelInfo->dvName);
    dfield2 = strlen((const char *)pDelInfo->dvPhone);
    dfield3 = strlen((const char *)pDelInfo->dvZip);
    dfield4 = strlen((const char *)pDelInfo->dvAddr);

    /* length of delivery data, [each field + DELIMITER + length size] */
    lenDelivery = dfield1+dfield2+dfield3+dfield4+16+2;

    deliveryData = (unsigned char *)wwMalloc(lenDelivery);

    /* Delivery Info length(2byte) */
    nOffset = 0;
    deliveryData[nOffset++] = (lenDelivery>>8)|0x80;
    deliveryData[nOffset++] = lenDelivery;

    /* Basic Field : 이름 */
    COPYTO(deliveryData, nOffset, "d1=", 3);
    COPYTO(deliveryData, nOffset, pDelInfo->dvName, dfield1);
    memcpy(&deliveryData[nOffset++], DELIMITER, 1);

    /* Basic Field : 연락처 */
    COPYTO(deliveryData, nOffset, "d2=", 3);
    COPYTO(deliveryData, nOffset, pDelInfo->dvPhone, dfield2);
    memcpy(&deliveryData[nOffset++], DELIMITER, 1);

    /* Basic Field : 우편번호 */
    COPYTO(deliveryData, nOffset, "d3=", 3);
    COPYTO(deliveryData, nOffset, pDelInfo->dvZip, dfield3);
    memcpy(&deliveryData[nOffset++], DELIMITER, 1);

    /* Basic Field : 주소 */
    COPYTO(deliveryData, nOffset, "d4=", 3);
    COPYTO(deliveryData, nOffset, pDelInfo->dvAddr, dfield4);
    memcpy(&deliveryData[nOffset++], DELIMITER, 1);

    free(pDelInfo);

    return deliveryData;
}

      





/**
 * Terminal에 저장된 Wallet Card 정보를 리턴한다.
 *
 *  Parameter :
 *              Unsigned char *pin;     // 4byte
 *              int Order;
 *  Return :
 *              Unsigned char *mwCardInfo;
 *
 */
unsigned char *getMWCard(unsigned char *cPIN, int cardOrder)
{
    WWCreditCard    *pCardInfo;       // 카드 정보 저장 공간
    unsigned char 			*cardData;
    int     cfield1, cfield2, cfield3, cfield4; // String length for Card 데이터 필드
    int     nOffset;
    int     lenCard=0;

	//swjung type casting
    pCardInfo = (WWCreditCard    *)wwGetCCardInfo((char *)cPIN, cardOrder);

    cfield1 = strlen((const char *)pCardInfo->cardNO);
    cfield2 = strlen((const char *)pCardInfo->cardPwd);
    cfield3 = strlen((const char *)pCardInfo->cardValid);
    cfield4 = strlen((const char *)pCardInfo->cardRno);

    /* length of card data, [each field + DELIMITER + length size] */
    lenCard = cfield1+cfield2+cfield3+cfield4+16 +2;
    cardData = (unsigned char *)wwMalloc(lenCard);

    // Card Info length(2byte)
    nOffset = 0;
    cardData[nOffset++] = (lenCard>>8) | 0x80;
    cardData[nOffset++] = lenCard;

    // Basic Field : 카드 번호
    COPYTO(cardData, nOffset, "c1=", 3);
    COPYTO(cardData, nOffset, pCardInfo->cardNO, cfield1);
    memcpy(&cardData[nOffset++], DELIMITER, 1);

    // Basic Field : 카드 비밀번호
    COPYTO(cardData, nOffset, "c2=", 3);
    COPYTO(cardData, nOffset, pCardInfo->cardPwd, cfield2);
    memcpy(&cardData[nOffset++], DELIMITER, 1);

    // Basic Field : 유효기간
    COPYTO(cardData, nOffset, "c3=", 3);
    COPYTO(cardData, nOffset, pCardInfo->cardValid, cfield3);
    memcpy(&cardData[nOffset++], DELIMITER, 1);

    // Basic Field : 주민등록번호
    COPYTO(cardData, nOffset, "c4=", 3);
    COPYTO(cardData, nOffset, pCardInfo->cardRno, cfield4);
    memcpy(&cardData[nOffset], DELIMITER, 1);

    free(pCardInfo);

    return cardData;
}



unsigned char *getMWAccount(unsigned char *aPIN, int accOrder)
{
    return NULL;
}


/**
 * Terminal에 저장된 Wallet CardExt 정보를 리턴한다.
 *
 *  Parameter :
 *              Unsigned char *pin;     // 4byte
 *              int Order;
 *  Return :
 *              Unsigned char *mwCardExtInfo;
 *
 */
unsigned char *getMWCardExt(unsigned char *oPIN, int ocbOrder)
{
    WWCardExtInfo       *pCardExtInfo;   // CardExt 정보 저장 공간
    unsigned char       *CardExtData;      // CardExt 리턴 데이터
    int ofield1, ofield2;                       // String length for CardExt fields
    int nOffset;
    int lenCardExt=0;

	MSG_HIGH("Just for removing warning ^^;",0,0,0); 

	//swjung type casting
    pCardExtInfo = (WWCardExtInfo *)wwGetCardExtInfo((char *)oPIN, ocbOrder);

    ofield1 = strlen((const char *)pCardExtInfo->pointNo);
    ofield2 = strlen((const char *)pCardExtInfo->pointPwd);

    /* length of cardExt data, [each field + DELIMITER + length size] */
    lenCardExt = ofield1+ofield2+8+2;

    CardExtData = (unsigned char *)wwMalloc(lenCardExt);

    /* CardExt Info length(2byte) */
    nOffset = 0;
    CardExtData[nOffset++] = (lenCardExt>>8)|0x80;
    CardExtData[nOffset++] = lenCardExt;

    /* Basic Field : CardExt 카드 번호 */
    COPYTO(CardExtData, nOffset, "o1=", 3);
    COPYTO(CardExtData, nOffset, pCardExtInfo->pointNo, ofield1);
    memcpy(&CardExtData[nOffset++], DELIMITER, 1);

    /* Basic Field : CardExt 비밀번호 */
    COPYTO(CardExtData, nOffset, "o2=", 3);
    COPYTO(CardExtData, nOffset, pCardExtInfo->pointPwd, ofield2);
    memcpy(&CardExtData[nOffset++], DELIMITER, 1);

    free(pCardExtInfo);

    return CardExtData;
}





