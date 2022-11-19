//---------------------------------------
//
//	sig_template.c
//
//	2000. 11
//
//	by Byung-Jun Yoon
//
//----------------------------------------


#include "sha.h"

// In order to construct the input for hash calculation,
// the following steps are performed
//  - use initially a 95-byte buffer as above (bytes 1...95)
//  - replace bytes 46...58 with the value of UTC time expressed as
//    YYMMDDHHMMSSZ (ASCII-encoded)
//  - replace bytes 76...95 with the 20-byte value of the SHA-1 hash
// The next step is to calculate the hash from the above 95-byte buffer.
// Finally, the signature is calculated. 


static const unsigned char sig_template_signingtime [] =
{
	0x31, 0x5d,
		0x30, 0x18,
			// contentType
			0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x03,
			// data
			0x31, 0x0b,
				0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01,
		0x30, 0x1c,
			// signingTime
			0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x05,
			// UTCTime
			0x31, 0x0f,
				0x17, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
		0x30, 0x23,
			// messageDigest
			0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x04,
			// SHA-1 digest
			0x31, 0x16,
				0x04, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


// In order to construct the input for hash calculation,
// the following steps are performed
// - use initially a 91-byte buffer as above (bytes 1...91)
// - replace bytes 47...54 with the value of the 8 byte nonce.
// - replace bytes 72...91 with the 20-byte value of the SHA-1 hash
// The next step is to calculate the hash from the above 91-byte buffer.
// Finally, the signature is calculated. 


static const unsigned char sig_template_signernonce [] =
{
	0x31, 0x59,
		0x30, 0x18,
			// contentType
			0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x03,
			// data
			0x31, 0x0b,
				0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01,
		0x30, 0x18,
			// signerNonce
			0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x19,
			0x03,
			0x31, 0x0a,
				0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x30, 0x23,
			// messageDigest
			0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x04,
			// SHA-1 digest
			0x31, 0x16,
				0x04, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void setSigTemplate(unsigned char * template, const char * strTime,
					const unsigned char * strToSign, int nLength)
{
	unsigned char md[SHA_DIGEST_LENGTH];
	char temp[1024];

	memcpy(temp, strToSign, nLength);
	temp[nLength] = '\0';

	/*
	printf("calling setSigTemplate...\n");
	printf(" ->strToSign  :[%s]\n", temp);
	printf(" ->signingTime:[%s]\n", strTime);
	*/

	SHA1(strToSign, nLength, md);
	memcpy(template, sig_template_signingtime, sizeof(sig_template_signingtime));
	memcpy(&template[45], strTime, 13);
	memcpy(&template[75], md, 20);
}




//
//	End of sig_template.c
//
//-------------------------------