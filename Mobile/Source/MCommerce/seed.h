/*******************************************************************************
*
* FILE:         seedx.h
*
* DESCRIPTION:  header file for seedx.c
*
*******************************************************************************/

#ifndef SEED_H
#define SEED_H


// Decide whether to use padding in
// SEED-CBC Encrypt/Decrypt

#define SEED_DO_PADDING			1

#ifdef  __cplusplus
extern "C" {
#endif

/********************** Include files ************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//swjung
#include "crypto_types.h"

/********************* Type Definitions **********************/

typedef unsigned char seed_cblock[16];
typedef DWORD_T seed_key_schedule[32];

#define SEED_CBLOCK			seed_cblock
#define SEED_KEY_SCHEDULE	seed_key_schedule


/******************* Constant Definitions *********************/

#define NoRounds         16
#define NoRoundKeys      (NoRounds*2)
#define SeedBlockSize    16    /* in bytes */
#define SeedBlockLen     128   /* in bits */
#define SEED_BLOCK_SIZE  16


/********************** Common Macros ************************/

#if defined(_MSC_VER)
    #define ROTL(x, n)     (_lrotl((x), (n)))
    #define ROTR(x, n)     (_lrotr((x), (n)))
#else
    #define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))
    #define ROTR(x, n)     (((x) >> (n)) | ((x) << (32-(n))))
#endif


/**************** Function Prototype Declarations **************/

#ifndef PROTOTYPES
#define PROTOTYPES  1
#endif

#if PROTOTYPES
#define PROTO_LIST(list)    list
#else
#define PROTO_LIST(list)    ()
#endif

void SeedEncrypt 
    PROTO_LIST((BYTE_T *pbData, DWORD_T *pdwRoundKey));
void SeedDecrypt
    PROTO_LIST((BYTE_T *pbData, DWORD_T *pdwRoundKey));
void SeedEncRoundKey
    PROTO_LIST((DWORD_T *pdwRoundKey, BYTE_T *pbUserKey));
void SeedDecRoundKey
    PROTO_LIST((DWORD_T *pdwRoundKey, BYTE_T *pbUserKey));

/******************************************************************/

/*
 *
 *	Added by Byung-Jun Yoon
 *
 */

#define C2DW(c, dw) 	dw = ((DWORD_T)(*(c)));dw |= ((DWORD_T)(*((c)+1))) << 8L; dw |= ((DWORD_T)(*((c)+2))) << 16L; dw |= ((DWORD_T)(*((c)+3))) << 24L;

#define DW2C(dw, c) (*(c)) = (unsigned char)((dw) & 0xff); (*((c)+1)) = (unsigned char)(((dw)>>8L) & 0xff); (*((c)+2)) = (unsigned char)(((dw)>>16L) & 0xff); (*((c)+3)) = (unsigned char)(((dw)>>24L) & 0xff);
						


void SeedEncryptData(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey);

void SeedDecryptData(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey);

#if SEED_DO_PADDING
long SeedEncryptCBC(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey, BYTE_T * iv);

long SeedDecryptCBC(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey, BYTE_T * iv);
#else
void SeedEncryptCBC(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey, BYTE_T * iv);

void SeedDecryptCBC(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey, BYTE_T * iv);
#endif	// SEED_DO_PADDING

//int seed_rand_key(unsigned char * seedKey);
int seed_rand_key(SEED_CBLOCK * seedKey);

#define seedEncryptCBC			SeedEncryptCBC
#define seedDecryptCBC			SeedDecryptCBC
#define seedEncryptData			SeedEncryptData
#define seedDecryptData			SeedDecryptData
#define seedRandomKey			seed_rand_key
#ifdef BLOCK
#define seedSetKey(key, ks)		SeedEncRoundKey(ks, (unsigned char *) key)
#endif //#ifdef BLOCK
#define seedSetKey(key, ks)		SeedEncRoundKey_New(ks, (unsigned char *) key)

//swjung
void SeedEncRoundKey_New(DWORD_T *pdwRoundKey, BYTE_T *pbUserKey);

#ifdef  __cplusplus
}
#endif

#endif
