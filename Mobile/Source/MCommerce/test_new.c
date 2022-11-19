#include "rsa_new.h"
#include "sha.h"
#include "md5_new.h"
#include "userkey1.h"


extern void    CLNTa_log (unsigned char viewId, signed short logNo, const char *format, ...);

int testRSA(void)
{

	RSA * rsa = NULL;
	int err;
	unsigned char * p = _userKey1;
	unsigned char teststr[] = "test string!";
	unsigned char buf1[256], buf2[256];

	CLNTa_log(1,0,"test start!\n");	
	if(!d2i_RSAPrivateKey(&rsa, &p, sizeof(_userKey1))){ 
		CLNTa_log(1,0,"d2i error!\n"); 
		return -1;
	}

	CLNTa_log(1,0,"d2i_RSAPrivateKey end!\n");	

	if((err=rsaPublicEncrypt(strlen(teststr)+1, teststr, buf1, rsa, RSA_PKCS1_PADDING))==-1){
		CLNTa_log(1,0,"RSA Public Encrypt Error!\n");
		return -1;
	}

	CLNTa_log(1,0,"rsaPublicEncrypt end!\n");	

	if((err=rsaPrivateDecrypt(err, buf1, buf2, rsa, RSA_PKCS1_PADDING))==-1){
		CLNTa_log(1,0,"RSA Private Decrypt Error!\n");
		return -1;
	}

	CLNTa_log(1,0,"rsaPrivateDecrypt end!\n");	

	CLNTa_log(1,0,"%s\n", buf2);
	if((err=rsaPrivateEncrypt(strlen(teststr)+1, teststr, buf1, rsa, RSA_PKCS1_PADDING))==-1){
		CLNTa_log(1,0,"RSA Private Encrypt Error!\n");
		return -1;
	}

	CLNTa_log(1,0,"rsaPrivateEncrypt end!\n");	

	if((err=rsaPublicDecrypt(err, buf1, buf2, rsa, RSA_PKCS1_PADDING))==-1){
		CLNTa_log(1,0,"RSA Public Decrypt Error!\n");
		return -1;
	}
	CLNTa_log(1,0,"rsaPublicDecrypt end!\n");	

	CLNTa_log(1,0,"%s\n", buf2);
	
	CLNTa_log(1,0,"OK!\n");	
	
	return 0;
}
