/*
 * File : mwscript.c
 * - WMLScript Extension Functions for Mobile Wallet
 *
 * Mobile Wallet Program 
 *
 * Copyright(c)2001 TGCORP Co. All rights reserved.
 * Writer : Kim, Hyejeong (lunadic@tgcorp.com)
 * Tabstop=4
 * Revision : 1.2.1
 */

/*
 * WPKI Crypto API
 */
#include "wcert.h"
#include "script_enum.h"
#include "base64.h"
#include "signtext.h"
#include "cert_manage.h"
#include "mwdef.h"
#include "mwscript.h"
#include "clnt.h"

#define PIN_MASK	0x0038


TagReqMsg			stReqMsg = { NULL, 0, 0, NULL, 0, NULL, 0, 0, NULL };
ReqMsg				req = &stReqMsg;

static int		isNullString;

/////////////////////////////////////////////////////////////
//swjung insert global variables
AccLevel isAccess;
CurrentField curfield; 
unsigned char	*pWalletPIN;
int errorNO;
/////////////////////////////////////////////////////////////

/*
 * Return Enveloped Data for append string(product information) and Mobile Wallet Data 
 *	 
 * Parameter :
 *		unsigned char *pAppendString;	//Append String to Mobile Wallet Data
 * 		unsigned int subsetToEnc;		//MWallet Data to Encrypt
 *		unsigned int subsetToSign;		//MWallet Data to Sign
 *		unsigned char *pRecipientCrt;	//Server Certificate
 *		unsigned int keyManageAlg;		//Key Management Algorithm
 *		unsigned char *pContentEncAlg;	//content Encryption Algorithm
 *		unsigned int keyIdType;			//KeyIdType
 *		unsigned char *keyId;			//keyId
 * Returnn :
 * 		char *Enveloped Data;			//Signed and Encrypted Data return
 * 	
 * ==> Caller must free returned data after use. 
 */
char *getMWData(
	unsigned char *appendString,					
	unsigned int setStringEnc,
	unsigned int setStringSign,
	unsigned char *recipientCrt,
	unsigned int keyManageAlg,
	unsigned char *contentEncAlg,
	unsigned int symmetricKeyId, 
	unsigned int keyIdType,					
	unsigned char *keyId,
	int errorMMI)
{

	unsigned char	*pSignedData = NULL;
	unsigned char	*pEncryptedData = NULL;
	unsigned char	*pEncodedResult = NULL;
	unsigned char	*pCertificate = NULL;

	int	nEncAlgo=-1;
	char	*p;

	unsigned char	*pErrorReturn = (unsigned char *)wwMalloc( 80 );

	//Added by swjung
	MSG_HIGH("For removing warning ^^;",0,0,0); 
	//Added by swjung
	///////////////////////////////////////////////////////////////////////
	//Codes for SKY MMI triggered error handling
	///////////////////////////////////////////////////////////////////////

	errorNO = errorMMI;

	if(errorNO != MMI_NO_ERROR)
	{
		strcpy((char *)pErrorReturn, (const char *)getErrorMsg());
		goto error;
	}
	///////////////////////////////////////////////////////////////////////

	//Added by swjung
	isAccess.personal=0;
	isAccess.delivery= 0;
	isAccess.card= 0;
	isAccess.account= 0;
	isAccess.cardext= 0;

	/** Initialize Parameter to Static Memory Area **/
	if(appendString == NULL)
	{
		isNullString = 1;
	}else
	{
		req->pAppendString = appendString;
	}
	req->subsetToEnc = setStringEnc;
	req->subsetToSign = setStringSign;

	if(recipientCrt == NULL)
	{
		errorNO = 0;
		strcpy( (char *)pErrorReturn, (const char *)getErrorMsg() );
		goto error;
	} else
	{	
		req->pRecipientCrt = recipientCrt;
	}

	req->keyManageAlg = keyManageAlg;
	if(contentEncAlg == NULL)
	{
		errorNO = 0;
		strcpy( (char *)pErrorReturn, (const char *)getErrorMsg() );
		goto error;
	} else
	{
		req->pContentEncAlg = contentEncAlg;
	}
	req->symmetricKeyId = symmetricKeyId;
	req->keyIdType = keyIdType;
	req->keyId = keyId;


	/** Check signText() Parameter **/
	// TODO( Securesoft ): keyIdType, keyId, 개인키 존재여부 확인	
	// 단말업체에서 제공되어야 하는 함수임.(SecureSoft에서 정의한 Function)
/*
	if((nEnum = getEnumByID(nKeyIdType, keyId)) == -1) {
		//Added by swjung
 		errorNO = 9;
		strcpy( (char *)pErrorReturn, (const char *)getErrorMsg() );
		goto error;
	}
*/

	/** Check encryptText() Parameter **/
	// TODO( Securesoft ): keyManageAlg, contentEncAlg para
	// keyManagementAlgorithm 지원 여부 확인
	if(keyManageAlg != ka_algo_rsa) { 
		//Added by swjung
		errorNO = 3;
		strcpy((char *) pErrorReturn, getErrorMsg() );
    	goto error;
	}
 
	for(p = (char *)contentEncAlg; *p!='\0'; ++p) 
	{
 		if(*p==des_ede3_cbc || *p==seed_cbc) 
		{
  			nEncAlgo = *p;
  			break;
 		}
	}
	if(nEncAlgo==-1) 
	{
 		errorNO = 4;
		strcpy((char *) pErrorReturn, getErrorMsg() );
		goto error;
	}

	/** Check SubSetRule **/
	if(checkSubsetRule(req->subsetToEnc, req->subsetToSign))
	{
		strcpy( (char *)pErrorReturn, (const char *)getErrorMsg() );
		goto error;
	}

	/** Parse Certificate **/ 
	// TODO : accessabelwd, certificate param 전달 문제 
	
	if(parseCert( (unsigned char**)&pCertificate, req->pRecipientCrt))
	{
		strcpy( (char *)pErrorReturn, (const char *)getErrorMsg() );
		goto error;
	}


	/** Set PlainWalletData Current Filed **/
	setCurrentField(req->subsetToEnc, req->subsetToSign);



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
	//Deleted by swjung
#ifdef BLOCK
	/** AppendString Process - Device Part **/
	if(appendStrProcess(isNullString, req->subsetToSign))
	{
		strcpy( (char *)pErrorReturn, (const char *)getErrorMsg() );
		goto error;
	}


	/*
     * AppendString Process - Device Part 
     * 수행 조건 :
     * appendString이 Null이 아니고 전자서명에 포함되지 않을 경우 수행.
     */
    if( !(isNullString) && !(req->subsetToSign & MASK_Append_Info) )
    {
        if(DisplayAppendString(req->pAppendString))
        {
            strcpy( pErrorReturn, getErrorMsg() );
            goto error;
        }
    }


	/** MWallet PIN Input Process - Device Part **/
	if(walletPINProcess(req->subsetToEnc))
	{
		strcpy( (char *)pErrorReturn, (const char *)getErrorMsg() );
		goto error;
	}
#endif //#ifdef BLOCK
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


	/** Sign Wallet Data 	
	 * signWD returns signed_Wallet_data		 *
	 * caller must free returned data after use. */
	if(curfield.signedWD != 0)
	{
		if(signWD( (unsigned char**) &pSignedData, req->pAppendString, req->subsetToEnc, req->keyIdType, req->keyId))
		{

			if(pSignedData != NULL)
			{
				strcpy( (char *)pErrorReturn, (const char *)pSignedData);
				goto error;
			}
			else
			{
				strcpy( (char *)pErrorReturn, (const char *)getErrorMsg() );
				goto error;
			}
			
		}
	}else
	{
		pSignedData = NULL;
	}


	/** Encrypt Wallet Data **/
	if(encryptWD( (unsigned char**) &pEncryptedData, pSignedData, req->pAppendString, req->subsetToEnc, pCertificate, req->keyManageAlg, req->pContentEncAlg))
	{
		if (pSignedData) free( pSignedData );

		if(pEncryptedData != NULL)    
		{       
			strcpy( (char *)pErrorReturn, (const char *)pEncryptedData);       
			goto error;     
		}
		else
		{
			strcpy((char *) pErrorReturn, (const char *)getErrorMsg() );
			goto error;
		}
	}

	free(pSignedData);
	free(pCertificate);

	/* caller must free.(pEncodedResult) */
	/** Base64 Encoding */
	pEncodedResult = encodResult( pEncryptedData);
	free(pEncryptedData);
		
	/** Return Enveloped data **/
	//swjung type casting
    return (char *)pEncodedResult;


error: 
	if (pSignedData) free(pSignedData);
	if (pEncryptedData) free(pEncryptedData);
	if (pEncodedResult) free(pEncodedResult);
	if (pCertificate) free(pCertificate);	
	//Deleted by swjung
	//if (pErrorReturn) free( pErrorReturn );

	/** Return Error Msg **/
	//swjung type casting
    return (char *)pErrorReturn;

}


/**	Check Wallet Field Subset Rule **/
int checkSubsetRule(unsigned int toEnc, unsigned int toSign)
{
    //-----------------------------------------------------------
    //  Mobile Wallet Field Superset Rule(or Subset Rule) Check
    //  TODO : Error Control - "error:violateSubsetRule"
    //-----------------------------------------------------------		
    if((toEnc ^ toSign) & toSign)
	{
		errorNO = 2;
		return RET_ERROR;
    }

	return RET_OK;
} 

/** Set PlainWalletData Current Filed Status **/
void setCurrentField(unsigned int toEnc, unsigned int toSign)
{

    /** Mobile Wallet Fields SubSet **/

	/* ToBeUnsigned PlainWalletData - current field */
	curfield.unsignedWD = toEnc ^ toSign;

	/* ToBeSigned PlainWalletData - current field */
	curfield.signedWD	= toEnc & toSign;
}


/** parse wtls certificate to check access level (cert DN check) **/
int parseCert(unsigned char **pCert, unsigned char *cert)
{
	WTLSCert 		wcert;				// WTLS Certificate 저장 공간
	unsigned char	*serverDN = NULL;	// 인증서 DN 추출값
	unsigned char			*certificate = (unsigned char *)wwMalloc(1200);
	unsigned char			*pTemp = (unsigned char *)wwMalloc(1200);
	int				nCertLen=0;

#ifdef BLOCK
    /**  certificate base64-decoding(binary type) **/
    nCertLen = decodeBase64(cert, strlen((const char *)cert), certificate);

	/* TODO: 인증서 유효성 확인, TrustedCA 존재여부 확인(Securesoft) */

	/* recipientCertificateChain 확인  */
	/*
	if( ret = verifyCert(1, (unsigned char *)certificate, nCertLen) != 0 ) 
	{
		errorNO = 10;
     	goto errCert;
	}*/
#endif //#ifdef BLOCK

	/** WTLS Certificate Parsing **/
//	if(parseWTLSCert(certificate, nCertLen, &wcert))
    if( (nCertLen=parseWTLSCert(cert, 0, &wcert)) == 1 )     
    {
          errorNO = 6;
          goto errCert;
    }
 
	if(parseWTLSCert(cert, nCertLen, &wcert))
	{
		errorNO = 6;
		goto errCert;
	}

	//swjung MW수정
#ifdef BLOCK
	if(!(serverDN = strstr(wcert.subjectIDValue, "T=")))
	{
		errorNO = 1;
		goto errCert;
	}
#endif //#ifdef BLOCK

	//Modified by swjung for removing error and warning
	serverDN = (unsigned char *)strstr(wcert.subject, "T=");

	if(!serverDN)
	{
		errorNO = 1;
		goto errCert;
	}

	serverDN += 2;
	/**  Wallet Data Access Level Check from Server Cert **/
	if(strncmp((char *)serverDN, "ml", 2) == 0)
	{
		/** ServerDN("ml") : Personal, Delivery */ 
		isAccess.personal 	= 1;
		isAccess.delivery 	= 1;	
	}
	else if(strncmp((char *)serverDN, "pg", 2) == 0)
	{
		/** ServerDN("pg") : Personal, Delivery, Card, Account, CardExt */
		isAccess.personal 	= 1;
		isAccess.delivery 	= 1;	
		isAccess.card		= 1;
		isAccess.account	= 1;
		isAccess.cardext	= 1;
	}
	else
	{	
		errorNO = 1;
		goto errCert;
	}
	
	*(pTemp) = nCertLen>>8;
	*(pTemp+1) = nCertLen;
	memcpy(pTemp+2, certificate, nCertLen);
	free(certificate);

	*pCert = pTemp;

	return RET_OK;

errCert : 
	if(certificate) free(certificate);
	if(pTemp) free(pTemp);

	return RET_ERROR;
}

//Deleted by swjung
#ifdef BLOCK
/** Append String Process **/
int appendStrProcess(int isNull, unsigned int toSign)
{
    if(isNull)
	{	
		// Not Display here & Not Display in signing
	}else
	{
		if(toSign & MASK_Append_Info)
		{
			/* AppendString을 전자서명함 
			 * Not Display here, will be Display in signing */
		}else
		{
			/* AppendString를 전자서명하지 않음
			 * TODO: by native methods {{NATIVE}} */

			if(DisplayAppendString())
			{
				return RET_ERROR;
			}
		}
	}
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/** Mobile Wallet PIN Access Process **/
int walletPINProcess(unsigned int toEnc)
{
	if(PIN_MASK & toEnc)
	{
		/* Terminal Native Method - Call Device Part */
		pWalletPIN = InputPINtoMWData();	

		//TODO:caller must be free();
		if(pWalletPIN == NULL)
		{
			return RET_ERROR;
		}
	}

	return RET_OK;
}
#endif //#ifdef BLOCK
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/** SIGNING **/
int signWD( unsigned char **psData, unsigned char *appStr_, unsigned int toEnc_, unsigned int keyIdType, unsigned char *keyId)
{
	unsigned char *getToBeSign;
	unsigned char *pSign = (unsigned char *)wwMalloc(1024);
	unsigned char *pTemp = NULL;
	int 	lenSignedWD;

	/** GENERATE TO SIGN DATA **/
	getToBeSign = generateSignedWD(appStr_, toEnc_);
	if(getToBeSign == NULL) 
	{
		free(pSign);
		return RET_ERROR;
	}

	// set signed wallet data into pSign

	/** SIGN DATA **/
    if((pTemp = signText((const char *)getToBeSign+2, INCLUDE_CERTIFICATE|INCLUDE_KEY_HASH|INCLUDE_CONTENT, keyIdType, keyId, &lenSignedWD)) == NULL)
	{
		errorNO = 0;
		free(getToBeSign);
		return RET_ERROR;
    }
	
	if( !memcmp(pTemp, "error:", 6) || !memcmp(pTemp, "invalid", 7) )
	{    
		*psData = pTemp;    
		free(getToBeSign);    
		return RET_ERROR;
	}
	else
	{
		*(pSign) = lenSignedWD>>8;
		*(pSign+1) = lenSignedWD;
		memcpy(pSign+2, pTemp, lenSignedWD);

		*psData = pSign;
		free(getToBeSign);

		return RET_OK;
	}
}


/** ENCRYPTING **/
int encryptWD(unsigned char **peData, unsigned char *psData, unsigned char *appStr, unsigned int toEnc, unsigned char *pCert, unsigned int keyManageAlg, unsigned char *pContentEncAlg)
{
	int	offset=0;
	unsigned char *getToBeEnc;
	unsigned char *pToBeEnvelope = (unsigned char *)wwMalloc(1024);
	unsigned char *pEnvelopedWD = (unsigned char *)wwMalloc(1024);	
	unsigned char *pTemp = (unsigned char *)wwMalloc(1024);	
	unsigned char *pCertificate = (unsigned char *)wwMalloc(1040);
	int lenUnsignedWD=0;
	int lenSignedWD=0;
	int lenEncWD=0;
	int lenCert=0;
	
	lenCert = *(pCert);
	lenCert = lenCert<<8 | *(pCert+1);
	pCertificate = pCert+2;
	lenCert = 361;

	/** GENERATE TO ENCRYPT DATA **/
	getToBeEnc = generateUnsignedWD(appStr, toEnc);
	if(getToBeEnc == NULL)
		return RET_ERROR;

	lenUnsignedWD = *(getToBeEnc);
	lenUnsignedWD = lenUnsignedWD<<8 | *(getToBeEnc+1);
	COPYTO(pToBeEnvelope, offset, getToBeEnc, lenUnsignedWD);

	if(psData != NULL)
	{
		lenSignedWD = *(psData);
		lenSignedWD = lenSignedWD<<8 | *(psData+1);
		COPYTO(pToBeEnvelope, offset, psData+2, lenSignedWD);
	}

	/** ENCRYPT DATA **/
    if((pEnvelopedWD = (unsigned char *)encryptText((unsigned char *)pToBeEnvelope, offset, pCertificate, lenCert, keyManageAlg, (const char *)pContentEncAlg, &lenEncWD) ) == NULL)
	{
		errorNO = 0;
		free(getToBeEnc);
		free(pToBeEnvelope);
		return RET_ERROR;
    }

	if( !memcmp(pEnvelopedWD, "error:", 6) || !memcmp(pEnvelopedWD, "invalid", 7) )
	{
		*peData = pEnvelopedWD;    
		free(getToBeEnc);    
		free(pToBeEnvelope);    
		return RET_ERROR;
	}
	else
	{
		pTemp[0] = lenEncWD>>8;
		pTemp[1] = lenEncWD;
		memcpy(pTemp+2, pEnvelopedWD, lenEncWD);

		*peData = pTemp;

		free(getToBeEnc);
		free(pToBeEnvelope);
		free(pEnvelopedWD);
		return RET_OK;
	}
}

/** BASE64 ENCODING **/
unsigned char *encodResult(unsigned char *pEncodedData)
{
	unsigned char	*pResult;
	int		lenEncWD;
	
	pResult = (unsigned char *)wwMalloc(2048);

	lenEncWD = *(pEncodedData);
	lenEncWD = lenEncWD<<8 | *(pEncodedData+1);
	
    /** WAP Enveloped Data base64-encoding(character-string type) **/
    encodeBase64(pEncodedData+2, lenEncWD, pResult);

	return pResult;
}


//Added void as a input arg
const char *getErrorMsg(void)
{
	switch(errorNO)
	{
		case 1 : 
			return "error:unauthorizedCert";
		case 2 : 
			return "error:violateSubsetRule";
		case 3 : 
			return "error:unsupportedKeyAgreementAlgorithm";
		case 4 : 
			return "error:unsupportedContentEncryptionAlgorithm";
		case 5 : 
			return "error:noCert";
		case 6 : 
			return "error:certChainVerificationFailure";
		case 7 : 
			return "error:rootCertNotPresent";
		case 8 : 
			return "error:userCancel";
		case 9 : 
			return "error:unsupportedPublicKey";
		case 10 : 
			return "error:cancelledByUser";
		default : 
			return "Invalid";
	}
}

