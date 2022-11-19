//-------------------------------
//
//  verifySignedCertificate.c
//
//  - verify the Signed-Certificate mime
//
//  2001. 6. created by Ji-Hyun Sohn
//
//-------------------------------

#include "crypto.h"
#include "wap_mime.h"
#include "wcert.h"
#include "signtext.h"
#include "rsa.h"
#include "sha.h"
#include "string.h"

//swjung
#include "sky.h"
//#include "clnt.h"//For CLNTa_log(...)
#include "aapiclnt.h"//For CLNTa_log(...)



int checkWTLSSignerCert( unsigned char *cert, int certLen);
int verifySignature( unsigned char *msg, int msgLen, unsigned char * sig,
		int sig_alg, int sigLen );

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
int checkWTLSTrustedCACert( unsigned char *cert, int certLen,
		unsigned char *signerCert, int signerCertLen,
		char *displayName, int cert_format, char *cainfo_url, int efs_pos, int log_pos);

int searchCACert( int keyIDType, unsigned char *keyID);
//Redefined by swjung for better fitting to SKY mobile station. Really? ^^;
#ifdef BLOCK
int checkWTLSTrustedCACert( unsigned char *cert, int certLen,
		unsigned char *signerCert, int signerCertLen,
		char *displayName, int cert_format, char *cainfo_url);
unsigned char *searchCACert( int keyIDType, unsigned char *keyID, int keyIDLen, 
		int *certLen, int *errMsg );
#endif //#ifdef BLOCK
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int storeNewCAInfo( unsigned char *skid, int skidLen, char *displayName,
		unsigned char *cert, int certLen, unsigned char *akid, int akidLen,
		int cert_type, unsigned long not_after );

int verifySignedCertificate(unsigned char *signedCert, int signedCertLen, int efs_pos, int log_pos) {
	SignedTrustedCAInfo *res = NULL;
	unsigned char *tc_info = NULL;
	unsigned char *signature = NULL;
	int errMsg, length;
	int nTemp;
	
	CLNTa_log(1,0,"[VeriSigned]Start!!!!!!!");

	res = parseSignedCertificate( signedCert, signedCertLen, &errMsg );

	if( errMsg != 0 ) return errMsg;

	// signerCert
	switch( res->signerCertFormat ) {
		case V_WTLS__WTLS_CERT :
			// SignerCert ����
			if( (errMsg = checkWTLSSignerCert( res->signerCert, 
							res->signerCertLen)) != 0 ) { 
				return errMsg;
			}
			break;
		case V_WTLS__X509_CERT :
		case V_WTLS__X968_CERT :
		case V_WTLS__CERT_URL :
		default :
			return 10030004;
	}

	// signature ����
	nTemp = res->charSet[0] << 8;
	nTemp |= res->charSet[1]; 

	tc_info = createTBSTrustedCAInfo( res->version, nTemp,
			res->displayName, res->trustedCACertFormat,
			res->trustedCACert, res->trustedCACertLen, res->cainfo_url,
			res->signerCertFormat, res->signerCert, res->signerCertLen,
			res->sig_alg, &length, &errMsg );

	signature = createSignature( signedCert, signedCertLen, length, &errMsg );

	if( errMsg != 0 ) return errMsg;
	if( (errMsg = verifySignature( tc_info, length, signature, res->sig_alg, 
					signedCertLen - length )) != 0 ) {
		return errMsg;
	}

	// trustedCACert
	switch( res->trustedCACertFormat ) {
		case V_WTLS__WTLS_CERT :
			// trustedCACert ����
			if( (errMsg = checkWTLSTrustedCACert( res->trustedCACert, 
						res->trustedCACertLen, res->signerCert, 
						res->signerCertLen,  res->displayName, 
						(int)V_WTLS__WTLS_CERT, res->cainfo_url, efs_pos, log_pos)) != 0 ) {
				return errMsg;
			}
			break;
		case V_WTLS__X509_CERT :
		case V_WTLS__X968_CERT :
		case V_WTLS__CERT_URL :
		default :
			return 10030004;
	}
	
	return 0;
}

int checkWTLSSignerCert( unsigned char *cert, int certLen) { 
	WTLSCert wcert;
	//Blocked by swjung not used variables
	/*
	unsigned char *pubKey = NULL;
	int tempCertLen;
	*/
	int errMsg = 0;
	RSA *rsa = NULL;
	// added for searching CA Cert
	unsigned char md[20];

	// WTLS �������� �Ľ��Ͽ� SKID�� ����Ű�� ������
	if( (errMsg = parseWTLSCert( cert, certLen, &wcert )) == 1 ) {
		return 10030007;
	}
	
	// SKID�� �̿��Ͽ� �ܸ����� ���念������ CA ������ �˻�
	// added for searching CA Cert
	SHA1((unsigned char *)wcert.subject, strlen(wcert.subject), md);

	// modified from subject to SHA1(subject)

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	//At this point, we do not consider that keyIDtype is other than SKID
	if(!searchCACert(1, md)) 
	{
		CLNTa_log(1,0,"Search Failure[No Matched Cert]");
		return 10040010;
		// errMsg : 1002�� ��� (signerCert�� �ܸ��⿡ �������� ����)
		// user notification & (cainfo_url�� ��ũ or ����)
	}
#ifdef BLOCK
	if( (pubKey = searchCACert( 1, md, 20, &tempCertLen, &errMsg )) == NULL ) {
		return( errMsg + 1 );
		// errMsg : 1002�� ��� (signerCert�� �ܸ��⿡ �������� ����)
		// user notification & (cainfo_url�� ��ũ or ����)
	}
#endif //#ifdef BLOCK
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	
	// signerCert ����
	if( (rsa = getRSAPubKey( &wcert )) == NULL ) {
		return 10040008;
	}

	if( (errMsg = verifyWTLSCert( cert, certLen, rsa )) != 0 ) {
		return 10040009; // signerCert ����
	}

	return errMsg;
}
			
int checkWTLSTrustedCACert( unsigned char *cert, int certLen,
		unsigned char *signerCert, int signerCertLen,
		char *displayName, int cert_format, char *cainfo_url, int efs_pos, int log_pos ) 
{
	WTLSCert wcert, signerWCert;
	RSA *rsa;
	//swjung
	unsigned char mdSKID[SHA_DIGEST_LENGTH];//Inserted by swjung
	unsigned char mdAKID[SHA_DIGEST_LENGTH];//Inserted by swjung

	//swjung
	//time_t curTime;
	unsigned long curTime;

	int errMsg;
	// added for searching CA Cert

	//Blocked, not used variables
	/*
	int tempCertLen;
	unsigned char md[20];
	*/

	CLNTa_log(1,0,"TCA Start");
	// WTLS �������� �Ľ��Ͽ� AKID�� ������
	if( (errMsg = parseWTLSCert( cert, certLen, &wcert )) == 1 ) {
		CLNTa_log(1,0,"TCA Parsing error");
		return 10030007;
	}

	//swjung
	//curTime = time( NULL );
	//curTime = SKY_GetLocalTime();
	curTime = CLNTa_currentTime();
	
	if( curTime > wcert.notAfter || curTime < wcert.notBefore ) {
		CLNTa_log(1,0,"TCA Time error");
		return 10030010;	// Invalid Period
					// user notification & (cainfo_url�� ��ũ or ����)
	}

	// Signer�� WTLS �������� �Ľ��Ͽ� SKID�� ����Ű�� ������
	if( (errMsg = parseWTLSCert( signerCert, signerCertLen, &signerWCert )) == 1 ) {
		CLNTa_log(1,0,"TCA SKID error");
		return 10030007;
	}

	// user notification & (cainfo_url�� ��ũ or ����)
	// signer�� SKID�� trustedCAInfo�� AKID�� ������ Ȯ��
	if(strlen(signerWCert.subject) != strlen(wcert.issuer))
	{
		CLNTa_log(1,0,"TCA AKID/SKID Mismatch error");
		return 10030011;
	}
	else if(memcmp(wcert.issuer, signerWCert.subject, strlen(signerWCert.subject)))
	{
		CLNTa_log(1,0,"TCA AKID/SKID Mismatch2 error");
		return 10030011;
	}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#ifdef BLOCK
	// AKID�� �̿��Ͽ� �ܸ����� ���念������ IssuerCA ������ �˻�
	// added for searching CA Cert
	SHA1((unsigned char *)wcert.issuer, strlen(wcert.issuer), md);

	// modified from issuer to SHA1(issuer)
	if( (pubKey = searchCACert( 1, md, 20, &tempCertLen, &errMsg )) == NULL ) {
		return( errMsg + 1 );
		// errMsg : 1002�� ��� (signerCert�� �ܸ��⿡ �������� ����)
		// user notification & (cainfo_url�� ��ũ or ����)
	}	
#endif //#ifdef BLOCK
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

	if( (rsa = getRSAPubKey( &signerWCert )) == NULL ) {
		CLNTa_log(1,0,"TCA PublicKey error");
		return 10030008;
	}
	
	// trustedCACert ����
	if( (errMsg = verifyWTLSCert( cert, certLen, rsa )) != 0 ) {
		CLNTa_log(1,0,"TCA Verification error");
		return 10030009;
	}

	/* �����ؾ��� ���� ��������
	 * ------------------------
	 * Cert Type   : cert_format
	 * Display     : displayName
	 * Cert        : cert
	 * ca_info url : cainfo_url
	 * SKID        : Cert Type�� WTLS�� ���� SHA1(wCert->subject),
	 *                  X509�� ���� wCert->subject
	 * AKID        : Cert Type�� WTLS�� ���� SHA1(wCert->issuer),
	 *                  X509�� ���� wCert->issuer
	 * subject DN  : wCert->subject
	 * Not After   : wCert->notAfter
	 */

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	//Hashed DN is used in this case, contrary to Hashed Public key 
	//swjung
	SHA1((const unsigned char *)wcert.subject, strlen(wcert.subject), mdSKID);		
	SHA1((const unsigned char *)wcert.issuer, strlen(wcert.issuer), mdAKID);

	//SKY_SET_MW_TCACERT_LENGTH(efs_pos,BYTE *s,int len);
	SKY_SET_MW_TCACERT_LENGTH(efs_pos, certLen);

	//SKY_SET_MW_TCACERT_CHECKFIELD(efs_pos,BYTE *s,int len);		
	SKY_SET_MW_TCACERT_DISPLAY(efs_pos,     (unsigned char *)displayName, strlen(displayName));		
	SKY_SET_MW_TCACERT_SUBJECT_KID(efs_pos, mdSKID, 20);		
	SKY_SET_MW_TCACERT_ISSUER_KID(efs_pos,  mdAKID, 20);
	SKY_SET_MW_TCACERT_CERT(efs_pos,        (unsigned char *)cert, certLen);		
	SKY_SET_MW_TCACERT_CA_INFO_URL(efs_pos, (unsigned char *)cainfo_url, strlen(cainfo_url));		
	SKY_SET_MW_TCACERT_SUBJECT_DN(efs_pos,  (unsigned char *)wcert.subject, strlen(wcert.subject));		
	SKY_SET_MW_TCACERT_NOT_AFTER(efs_pos,   (unsigned long)wcert.notAfter);		
	SKY_SET_MW_TCACERT(efs_pos);

	SKY_SET_MW_TCACERT_DATA_SAVED(efs_pos, log_pos);

	//Save field info
	SKY_SET_MW_WALLET();
	
	CLNTa_log(1,0,"[VeriSigned]End Oh yeah!!!!!!!");
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////


	return errMsg;
}

int verifySignature( unsigned char *msg, int msgLen, unsigned char * sig,
		int sig_alg, int sigLen ) {
	unsigned char *hashedMsg = NULL;
	int i;

	if( sigLen != SHA_DIGEST_LENGTH ) {
		return 10030006;	// not SHA1-hash
	}

	switch( sig_alg ) {
		case V_WTLS__ANONYMOUS : 
			return 0; 	// �������� �ʴ´�.
		case V_WTLS__ECDSA_SHA :
		case V_WTLS__RSA_SHA :
			break;		// Valid. Next Step
		default :
			return 10030006;	// Invalid Signature Algorithm
	}

	if( (hashedMsg = (unsigned char *)Malloc(SHA_DIGEST_LENGTH)) == NULL ) {
		return 10030001;	// Memory Shortage
	}

	SHA1( msg, msgLen, hashedMsg );

	for( i = 0; i < SHA_DIGEST_LENGTH; i++ )
		if( sig[i] != hashedMsg[i] )
			return 10030012;	// Signature has been changed

	return 0;
}

