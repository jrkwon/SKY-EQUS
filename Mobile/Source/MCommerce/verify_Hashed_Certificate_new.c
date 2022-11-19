//-------------------------------
//
//  verifyHashedCertificate.c
//
//  - verify the Hashed-Certificate
//  - Create out of band hash
//  - verify out of band hash
//
//  2001. 6. created by Ji-Hyun Sohn
//
//-------------------------------

#include "stdio.h"
#include "string.h"
#include "crypto.h"
#include "wap_mime.h"
//swjung
#include "wcert.h"
#include "sha.h"

#include "sky.h"
#include "clnt.h"

char *createOutOfBandHash( unsigned char *hash, int *errMsg );
char *createGroup( int tNum, int *errMsg );
int validateBandHash( char *strOutOfBandHash );
int validateGroup( char *group );

int verifyHashedCertificate( unsigned char *hashedCert, int hashedCertLen, char *strOutOfBandHash, int efs_pos, int log_pos) {
	
	char *resultHash = NULL;
	char *newOutofBand = NULL;
	int errMsg;

	// res : added for parsing the mime
	// wCert :  added for parsing the trustedCA Certificate
	TBHTrustedCAInfo *res;
	WTLSCert *wCert = NULL;

	//swjung
	unsigned char mdSKID[SHA_DIGEST_LENGTH];//Inserted by swjung
	unsigned char mdAKID[SHA_DIGEST_LENGTH];//Inserted by swjung

    //---------------------------------------------
    //  validate the out of band hash
    //---------------------------------------------

	CLNTa_log(1,0,"[VeriHash]Start------");
    if( (errMsg = validateBandHash( strOutOfBandHash )) != 0 ) {
		CLNTa_log(1,0,"[VeriHash]Error11------");
		return errMsg;
    }

	// add for parsing the mime
	if( (res = parseHashedCertificate( hashedCert, hashedCertLen, &errMsg )) == NULL ) {
		CLNTa_log(1,0,"[VeriHash]Error22------");
		return errMsg;
	}

	// add to check the hash algorithm
	switch( res->hash_alg ) {
		case eSHA1 :
			break;
		default :
			return 10020007;
	}

	//-------------------------------------
	//  hash the TBHCert    
	//-------------------------------------
	//swjung type casting
	//if( (resultHash = (unsigned char *)Malloc(SHA_DIGEST_LENGTH)) == NULL ) {
	if( (resultHash = (char *)Malloc(SHA_DIGEST_LENGTH)) == NULL ) {
		CLNTa_log(1,0,"[VeriHash]Error33------");
		return 10020001; // Memory Allocation Error
	}

    //swjung type casting
    SHA1( hashedCert, hashedCertLen, (unsigned char *)resultHash );
	
    //---------------------------------------------
    //  create the out of band hash from the hash of the TBHCert
    //---------------------------------------------
    //swjung type casting
	newOutofBand = createOutOfBandHash( (unsigned char *)resultHash, &errMsg );

    //---------------------------------------------
    //  compare two strings
    //---------------------------------------------
	// modified from strcmp() to memcmp()
	// delete the else statement
    if( memcmp( newOutofBand, strOutOfBandHash, SHA_DIGEST_LENGTH ) != 0 ) {
		CLNTa_log(1,0,"[VeriHash]Error44------");
        return 10020005;
    }

	if( resultHash ) Free( resultHash );

	// Store the trustedCA Certificate to storage
    if( (wCert = (WTLSCert *)Malloc(sizeof(WTLSCert))) == NULL ){
		CLNTa_log(1,0,"[VeriHash]Error55------");
		return 10020001;
     }

    if( parseWTLSCert(res->trustedCACert, res->trustedCACert_len, wCert) ){
		CLNTa_log(1,0,"[VeriHash]Error66------");
		return 10020008;
	}


	/* 저장해야할 값들 가져오기
	 * ------------------------
	 * Cert Type   : res->trustedCACertFormat
	 * Display     : res->displayName
	 * Cert        : res->trustedCACert
	 * ca_info url : res->cainfo_url
	 * SKID        : Cert Type이 WTLS인 경우는 SHA1(wCert->subject),
	 * 					X509인 경우는 wCert->subject
	 * AKID        : Cert Type이 WTLS인 경우는 SHA1(wCert->issuer),
	 * 					X509인 경우는 wCert->issuer
	 * subject DN  : wCert->subject
	 * Not After   : wCert->notAfter
	 */
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
	SHA1((const unsigned char *)wCert->subject, strlen(wCert->subject), mdSKID);		
	SHA1((const unsigned char *)wCert->issuer, strlen(wCert->issuer), mdAKID);


	//Substitute this field for TCA Cert lengthing storing
	SKY_SET_MW_TCACERT_LENGTH(efs_pos, res->trustedCACert_len);
	
	//SKY_SET_MW_TCACERT_CHECKFIELD(efs_pos,BYTE *s,int len);		
	SKY_SET_MW_TCACERT_DISPLAY(efs_pos,     (unsigned char *)res->displayName, strlen(res->displayName));		
	SKY_SET_MW_TCACERT_SUBJECT_KID(efs_pos, mdSKID, 20);		
	SKY_SET_MW_TCACERT_ISSUER_KID(efs_pos,  mdAKID, 20);
	SKY_SET_MW_TCACERT_CERT(efs_pos,        (unsigned char *)res->trustedCACert, res->trustedCACert_len);		
	SKY_SET_MW_TCACERT_CA_INFO_URL(efs_pos, (unsigned char *)res->cainfo_url, strlen(res->cainfo_url));		
	SKY_SET_MW_TCACERT_SUBJECT_DN(efs_pos,  (unsigned char *)wCert->subject, strlen(wCert->subject));		
	SKY_SET_MW_TCACERT_NOT_AFTER(efs_pos,   (unsigned long)wCert->notAfter);		
	SKY_SET_MW_TCACERT(efs_pos);

	SKY_SET_MW_TCACERT_DATA_SAVED(efs_pos, log_pos);

	//Save field info
	SKY_SET_MW_WALLET();
	
	CLNTa_log(1,0,"[VeriHash]End Oh yeah!!!!!!!");
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

	if(wCert) Free(wCert);
	if(res) Free(res);
//	if(rsa) Free(rsa);

	return errMsg;
}

//---------------------------------------------
//  create the out of band hash with 30 bytes length
//---------------------------------------------
char *createOutOfBandHash( unsigned char *hash, int *errMsg ) {
    int i, tNum;
    int offset = 0;
    char *result = NULL;
    char *group = NULL;

    if( (result = (char *)Malloc( DECIMAL_DIGIT_OUTOFBANDHASH )) == NULL ) {
		if( result )
			Free( result );
		*errMsg = 10020001;
		return NULL;
    }

    for( i = 0; i < 5; i++ ) {
		tNum = (hash[2*i] << 8) | hash[2*i+1];
		group = createGroup( tNum, errMsg );

		offset = i * 6;
		WRITE_TO_BUF( result, group, 6, offset );

		// add for memory reuse
		if( group ) Free( group );
    }

    return result;
}

//---------------------------------------------
//  create one group
//---------------------------------------------
char *createGroup( int tNum, int *errMsg ) {
    int i, t_digit, sum = 0, check; 
    char *digits = NULL;

    if( (digits = (char *)Malloc(7)) == NULL ) {
		if( digits ) Free( digits );
		*errMsg = 10020001;
		return NULL;
    }

    for( i = 4; i >= 0; i-- ) {
		t_digit = tNum % 10;

		// modified from (char)t_digit to t_digit+'0'
		digits[i] = t_digit + '0';

		tNum = tNum / 10;
		if( i % 2 == 0 ) {
	    	t_digit = t_digit * 2;
		}
		sum = sum + t_digit % 10 + t_digit / 10;
    }

    if( sum % 10 == 0 ) {
		check = 0;
    } else {
		check = 10 - sum % 10;
    }

	// modified from (char)check to check+'0'
    digits[5] = check + '0';

    digits[6] = '\0';

	*errMsg = 0;
    return( digits );
}

//---------------------------------------------
//  validate the out of band hash
//---------------------------------------------
int validateBandHash( char *strOutOfBandHash ) {
    int i;

    for( i = 0; i < 5; i++ ) {
		if( validateGroup( strOutOfBandHash + i*6 ) != 0 ) {
	    	return 10020006;
		}
    }

    return 0;
}

//---------------------------------------------
//  validate one group
//---------------------------------------------
int validateGroup( char *group ) {
    int i, sum = 0;
	// add
	int temp;

	// modified from group[i] to group[i]-'0'
    for( i = 5; i >= 0; i-- ) {
		temp = group[i] - '0';
		if( i % 2 == 0 ) {
	    	sum += (temp * 2) % 10 + (temp * 2) / 10;
		} else {
	    	sum += temp;
		}
    }

    return( sum % 10 );
}

