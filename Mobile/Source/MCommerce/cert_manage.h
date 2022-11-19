//------------------------------------
//
//	cert_manage.h
//
//	management of certificates in the
//	mobile terminal
//
//	2000. 11. by Byung-Jun Yoon
//
//------------------------------------

#ifndef CERT_MANAGE__H
#define CERT_MANAGE__H

#ifdef SCRIPT_RSA
	#include "rsa.h"
#endif // SCRIPT_RSA

#ifdef SCRIPT_ECC
	#include "Ecdsa.h"
#endif // SCRIPT_ECC

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//Not used functions
/*
unsigned char * getCertFromURL(const char * url, int * nLength);

unsigned char * getCertFromDN(const char * dn, int * nLength);
*/
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

unsigned char * getKeyID(int nEnum);

unsigned char * getCertificate(int nEnum, int * nLength);

int getEnumByID(int nIdType, const unsigned char * keyID);
char * getCertURL(int nEnum);

#ifdef SCRIPT_ECC
ECC * getPrivateKey(int nEnum);
#endif // SCRIPT_ECC

#ifdef SCRIPT_RSA
RSA * getPrivateKey(int nEnum);
#endif // SCRIPT_RSA



#endif
