#ifndef CRYPTO_TYPES__H
#define CRYPTO_TYPES__H

////////////////////////////////////////////////////////////////////////////////////////
//BYTE type is already used by EQUS and WAP source code.
//So We have no choice but to redefine BYTE to BYTE_T for open SSL source package
//swjung
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TYPE_DEFINITION
#define TYPE_DEFINITION
	#if defined(__alpha)
	typedef unsigned int        DWORD;
	typedef unsigned short      WORD;
	#else
	typedef unsigned long int   DWORD_T;
	typedef unsigned short int  WORD_T;
	#endif // __alpha
	typedef unsigned char           BYTE_T;
#endif	// TYPE_DEFINITION



#endif // CRYPTO_TYPES__H
