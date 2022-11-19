#include "Ecdsa.h"

extern void    CLNTa_log (unsigned char viewId, signed short logNo, const char *format, ...);
#ifdef BLOCK
int testEcc(void)
{
	ECC *ecc;
	EC_ELGAMAL *ctext;
	ECDSA_SIG *sig;
	unsigned char M[] = "I Love You!!";
	unsigned char R[32];
	int i;
	unsigned long size;

	ecc = ECC_new();


	if(ECC_generate_key(ecc)) CLNTa_log(1,0,"Key Generation Success\n");  
	else { CLNTa_log(1,0,"Key Generation Fail\n"); return 0; }

	if(ValidPoint(ecc->pub_key, ecc->curve)) 
		CLNTa_log(1,0,"Public Key is Correct\n");
	else { CLNTa_log(1,0,"Public Key is Incorrect\n"); return 0; }

	if((sig = ecdsa_do_sign(M, (unsigned long)strlen((char *)M), ecc))==NULL)
	{
		CLNTa_log(1,0,"Signature Generation Fail\n");
		return 0;
	}else
	{
		CLNTa_log(1,0,"Signature Generation Success\n");
	}

	if(ecdsa_do_verify(M, (unsigned long)strlen((char *)M), sig, ecc)) CLNTa_log(1,0,"Signature Verification Sucess\n");
	else CLNTa_log(1,0,"Signature Verification Failed\n");

	if((ctext = ec_elgamal_do_encrypt(M, (unsigned long)strlen((char *)M), ecc))==NULL)
	{
		CLNTa_log(1,0,"Encryption Fail\n");
		return 0;
	}else
	{
		CLNTa_log(1,0,"Encryption Success\n");
	}
	if(!ec_elgamal_do_decrypt(R, &size, ctext, ecc))
	{
		CLNTa_log(1,0,"Decryption Fail\n");
		return 0;
	}else CLNTa_log(1,0,"Decryption Success\n");

	ECC_free(ecc);

	return 1;
}
#endif //#ifdef BLOCK
