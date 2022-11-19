#include	"stdio.h"
#include    "sha.h"
#include	"crypto.h"

//Added by swjung
#include	"wap_mime.h"
#include	"wcert.h"

unsigned char *getCACertificate(int certFmt, const unsigned char *keyID, int *nLength)
{
	unsigned char md[20];
	unsigned char	*ca_cert = NULL;
	int keyID_len = strlen(keyID);
	
	// keyId ��, DN ���� Hash ���� ���ϱ� ���� ���� ----->
	SHA1(keyID, keyID_len, md);

	//Modified by swjung
	return getCACert(1, md, nLength);
}
