//----------------------------------
//
//	x509_verify.c
//	
//	by Byung-Jun Yoon
//
//----------------------------------

//swjung
//#include "time.h"
#include "string.h"
#include "stdio.h"
#include "rsa_new.h"
#include "md5_new.h"
#include "sha.h"

#include "cert.h"
//swjung
//#include "..\qualcomm\clk.h"
#include "clk.h"

#define OID_SHA1_WITH_RSA_ENCRYPTION		"1 2 840 113549 1 1 5"
#define OID_MD5_WITH_RSA_ENCRYPTION			"1 2 840 113549 1 1 4"
#define OID_RSA_ENCRYPTION					"1 2 840 113549 1 1 1"

extern unsigned long ui_get_local_time(void);
extern unsigned long SKY_GetLocalTime(void);

extern void clk_secs_to_julian(unsigned long secs, clk_julian_type *julian_ptr);

int compWithCurTime( const char * timeStr )
{
	struct tm	*ts;
	//swjung
	//time_t		curTime;
	char		curTimeBuf[12], timeBuf[12];
	int			nYear, nCurYear;
	//swjung
	unsigned long current_time;
    clk_julian_type	julian;

	strncpy(timeBuf, timeStr, 10);	// copy YYMMDDHHMM
	
//swjung
//time()를 삭제하고 SKY_GetLocalTime()을 삽입
#ifdef BLOCK
	curTime = time( NULL );
	ts = gmtime(&curTime);

	sprintf(curTimeBuf, "%02d%02d%02d%02d%02d",
						ts->tm_year % 100	,
						ts->tm_mon + 1		,
						ts->tm_mday			,
						ts->tm_hour			,
						ts->tm_min 	);
#endif //#ifdef BLOCK
	
#ifdef BLOCK
/* Julian time structure
*/
//typedef struct {
//  word year;            /* Year [1980..2100]                            */
//  word month;           /* Month of year [1..12]                        */
//  word day;             /* Day of month [1..31] or day of year [1..366] */
//  word hour;            /* Hour of day [0..23]                          */
//  word minute;          /* Minute of hour [0..59]                       */
//  word second;          /* Second of minute [0..59]                     */
//  word day_of_week;     /* Day of the week [0..6] Monday..Sunday        */
//} clk_julian_type;
#endif //#ifdef BLOCK

    current_time = SKY_GetLocalTime();
    clk_secs_to_julian( current_time, &julian);

	sprintf(curTimeBuf, "%02d%02d%02d%02d%02d",
						julian.year,
						julian.month,
						julian.day,
						julian.hour,
						julian.minute );

	nYear = (timeBuf[0] - '0') * 10 + (timeBuf[1] - '0');
	if(nYear < 50){
		nYear += 100;
	}
	
	nCurYear = (curTimeBuf[0] - '0') * 10 + (curTimeBuf[1] - '0');
	if(nCurYear < 50){
		nCurYear += 100;
	}

	if(nYear != nCurYear){
		return (nYear - nCurYear);
	}

	return strcmp(timeBuf, curTimeBuf);

}

int verify_cert(	unsigned char * user_cert,	long user_cert_len,
						unsigned char * ca_cert,	long ca_cert_len	)
{
	X509 caCert, userCert;
	RSA	 * caPubKey;
	unsigned char digest[20];
	unsigned char plainSig[256];
	unsigned char * pKey;
	int err = 0, i;

	if( parseDER2X509( ca_cert, ca_cert_len, &caCert) != 0){
		return 0;
	}

	if( parseDER2X509( user_cert, user_cert_len, &userCert) != 0){
		return 0;
	}

	// check certificate's validy period
	//
	// current time should be :
	// 		after 'notBefore' and before 'notAfter'
	//

	if( compWithCurTime( userCert.notBefore ) >= 0 ){
		return 0;
	}
	if( compWithCurTime( userCert.notAfter ) <= 0 ){
		return 0;
	}

	caPubKey = RSA_new();
	pKey = caCert.pubKey;


	// get CA public key

	d2i_RSAPublicKey(&caPubKey, &pKey, caCert.pubKeyLen);


	// decrypt the signature with the CA's public key
	
	i = RSA_eay_public_decrypt(userCert.sigLen, userCert.signature, plainSig, caPubKey, RSA_PKCS1_PADDING);

	printf("decrypted signature:\n");

#ifdef MCOMMERCE_PORTING
	Print(plainSig, i);
#endif //#ifdef MCOMMERCE_PORTING
	
	RSA_free( caPubKey );

	if( i < 0 ){
		return 0;
	}

	if( strcmp( userCert.sig_algo, OID_SHA1_WITH_RSA_ENCRYPTION )==0 ){
		SHA1( &user_cert[userCert.certOffset], userCert.certSize, digest );
		if(!memcmp( digest, plainSig+i-SHA_DIGEST_LENGTH, SHA_DIGEST_LENGTH)){
			err = 1;
		}
	}
	else if( strcmp( userCert.sig_algo, OID_MD5_WITH_RSA_ENCRYPTION )==0 ){
		MD5( &user_cert[userCert.certOffset], userCert.certSize, digest );
		if(!memcmp( digest, plainSig+i-MD5_DIGEST_LENGTH, MD5_DIGEST_LENGTH)){
			err = 1;
		}
	}
	else{
		// unknown algorithm
		err = 0;
	}

	return err;

}
