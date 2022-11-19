/*******************************************************************************
* 1999. 6.29.
* FILE:         seed.c - originally "seedx.c"
*
* DESCRIPTION: Core routines for the enhanced SEED
* 
*******************************************************************************/

#include "seed.h"
#include "seed.tab"
//---------------------------------------------------------------------------

/******************* Encryption/Decryption *******************/

#ifdef L_ENDIAN
	#undef BIG_ENDIAN
#elif defined B_ENDIAN
	#undef BIG_ENDIAN
	#define BIG_ENDIAN
#endif


#define GetB0(A)  ( (BYTE_T)((A)    ) )
#define GetB1(A)  ( (BYTE_T)((A)>> 8) )
#define GetB2(A)  ( (BYTE_T)((A)>>16) )
#define GetB3(A)  ( (BYTE_T)((A)>>24) )

#define SeedRound(L0, L1, R0, R1, K) { T0 = R0 ^ (K)[0]; T1 = R1 ^ (K)[1]; T1 ^= T0; T1 = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^ SS2[GetB2(T1)] ^ SS3[GetB3(T1)]; T0 += T1; T0 = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^ SS2[GetB2(T0)] ^ SS3[GetB3(T0)]; T1 += T0; T1 = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^ SS2[GetB2(T1)] ^ SS3[GetB3(T1)]; T0 += T1; L0 ^= T0; L1 ^= T1; }


#define EndianChange(dwS) ( (ROTL((dwS),  8) & (DWORD_T)0x00ff00ff) | (ROTL((dwS), 24) & (DWORD_T)0xff00ff00) )


/************************ Block Encryption *************************/
#ifdef BLOCK
	void SeedEncrypt(BYTE_T *pbData, DWORD_T *pdwRoundKey)
#endif //#ifdef BLOCK by swjung for symbol redefinition
void SeedEncrypt_New(BYTE_T *pbData, DWORD_T *pdwRoundKey)
{
	DWORD_T L0, L1, R0, R1, T0, T1, *K = pdwRoundKey;

    L0 = ((DWORD_T *)pbData)[0];
    L1 = ((DWORD_T *)pbData)[1];
	R0 = ((DWORD_T *)pbData)[2];
    R1 = ((DWORD_T *)pbData)[3];
#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

    SeedRound(L0, L1, R0, R1, K   ); /*   1 */
    SeedRound(R0, R1, L0, L1, K+ 2); /*   2 */
    SeedRound(L0, L1, R0, R1, K+ 4); /*   3 */
    SeedRound(R0, R1, L0, L1, K+ 6); /*   4 */
    SeedRound(L0, L1, R0, R1, K+ 8); /*   5 */
    SeedRound(R0, R1, L0, L1, K+10); /*   6 */
    SeedRound(L0, L1, R0, R1, K+12); /*   7 */
    SeedRound(R0, R1, L0, L1, K+14); /*   8 */
    SeedRound(L0, L1, R0, R1, K+16); /*   9 */
    SeedRound(R0, R1, L0, L1, K+18); /*  10 */
    SeedRound(L0, L1, R0, R1, K+20); /*  11 */
    SeedRound(R0, R1, L0, L1, K+22); /*  12 */
#if NoRounds == 16
    SeedRound(L0, L1, R0, R1, K+24); /*  13 */
    SeedRound(R0, R1, L0, L1, K+26); /*  14 */
    SeedRound(L0, L1, R0, R1, K+28); /*  15 */
    SeedRound(R0, R1, L0, L1, K+30); /*  16 */
#endif

#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif
    ((DWORD_T *)pbData)[0] = R0;
    ((DWORD_T *)pbData)[1] = R1;
    ((DWORD_T *)pbData)[2] = L0;
    ((DWORD_T *)pbData)[3] = L1;
}


/* same as encrypt, except that round keys are applied in reverse order. */
#ifdef BLOCK
void SeedDecrypt(BYTE_T *pbData, DWORD_T *pdwRoundKey)
#endif //#ifdef BLOCK by swjung against symbol redefinition
void SeedDecrypt_New(BYTE_T *pbData, DWORD_T *pdwRoundKey)
{
    DWORD_T L0, L1, R0, R1, T0, T1, *K=pdwRoundKey;

    L0 = ((DWORD_T *)pbData)[0];
    L1 = ((DWORD_T *)pbData)[1];
    R0 = ((DWORD_T *)pbData)[2];
    R1 = ((DWORD_T *)pbData)[3];
#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

#if NoRounds == 16
    SeedRound(L0, L1, R0, R1, K+30); /*   1 */
    SeedRound(R0, R1, L0, L1, K+28); /*   2 */
    SeedRound(L0, L1, R0, R1, K+26); /*   3 */
    SeedRound(R0, R1, L0, L1, K+24); /*   4 */
#endif
    SeedRound(L0, L1, R0, R1, K+22); /*   5 */
    SeedRound(R0, R1, L0, L1, K+20); /*   6 */
    SeedRound(L0, L1, R0, R1, K+18); /*   7 */
    SeedRound(R0, R1, L0, L1, K+16); /*   8 */
    SeedRound(L0, L1, R0, R1, K+14); /*   9 */
    SeedRound(R0, R1, L0, L1, K+12); /*  10 */
    SeedRound(L0, L1, R0, R1, K+10); /*  11 */
    SeedRound(R0, R1, L0, L1, K+ 8); /*  12 */
    SeedRound(L0, L1, R0, R1, K+ 6); /*  13 */
    SeedRound(R0, R1, L0, L1, K+ 4); /*  14 */
    SeedRound(L0, L1, R0, R1, K+ 2); /*  15 */
    SeedRound(R0, R1, L0, L1, K+ 0); /*  16 */

#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif
    ((DWORD_T *)pbData)[0] = R0;
    ((DWORD_T *)pbData)[1] = R1;
    ((DWORD_T *)pbData)[2] = L0;
    ((DWORD_T *)pbData)[3] = L1;
}



/******************** Key Scheduling **********************/

/* Constants for key schedule:
KC0 = golden ratio; KCi = ROTL(KCi-1, 1) */
#define KC0     0x9e3779b9UL
#define KC1     0x3c6ef373UL
#define KC2     0x78dde6e6UL
#define KC3     0xf1bbcdccUL
#define KC4     0xe3779b99UL
#define KC5     0xc6ef3733UL
#define KC6     0x8dde6e67UL
#define KC7     0x1bbcdccfUL
#define KC8     0x3779b99eUL
#define KC9     0x6ef3733cUL
#define KC10    0xdde6e678UL
#define KC11    0xbbcdccf1UL
#if NoRounds == 16
#define KC12    0x779b99e3UL
#define KC13    0xef3733c6UL
#define KC14    0xde6e678dUL
#define KC15    0xbcdccf1bUL
#endif

#define EncRoundKeyUpdate0(K, A, B, C, D, KC) { T0 = A; A = (A>>8) ^ (B<<24); B = (B>>8) ^ (T0<<24); T0 = A + C - KC; T1 = B + KC - D; (K)[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^ SS2[GetB2(T0)] ^ SS3[GetB3(T0)]; (K)[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^ SS2[GetB2(T1)] ^ SS3[GetB3(T1)]; }


#define EncRoundKeyUpdate1(K, A, B, C, D, KC) { T0 = C; C = (C<<8) ^ (D>>24); D = (D<<8) ^ (T0>>24); T0 = A + C - KC; T1 = B + KC - D; (K)[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^ SS2[GetB2(T0)] ^ SS3[GetB3(T0)]; (K)[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^ SS2[GetB2(T1)] ^ SS3[GetB3(T1)]; }


/* Encryption key schedule */
#ifdef BLOCK
void SeedEncRoundKey(DWORD_T *pdwRoundKey, BYTE_T *pbUserKey)
#endif //#ifdef BLOCK
void SeedEncRoundKey_New(DWORD_T *pdwRoundKey, BYTE_T *pbUserKey)
{
  DWORD_T A, B, C, D, T0, T1, *K=pdwRoundKey;

  A = ((DWORD_T *)pbUserKey)[0];
  B = ((DWORD_T *)pbUserKey)[1];
  C = ((DWORD_T *)pbUserKey)[2];
  D = ((DWORD_T *)pbUserKey)[3];
#ifndef BIG_ENDIAN
  A = EndianChange(A);
  B = EndianChange(B);
  C = EndianChange(C);
  D = EndianChange(D);
#endif

    T0 = A + C - KC0;
    T1 = B - D + KC0;
    K[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^
           SS2[GetB2(T0)] ^ SS3[GetB3(T0)];
    K[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^
           SS2[GetB2(T1)] ^ SS3[GetB3(T1)];

    EncRoundKeyUpdate0(K+ 2, A, B, C, D, KC1 );
    EncRoundKeyUpdate1(K+ 4, A, B, C, D, KC2 );
    EncRoundKeyUpdate0(K+ 6, A, B, C, D, KC3 );
    EncRoundKeyUpdate1(K+ 8, A, B, C, D, KC4 );
    EncRoundKeyUpdate0(K+10, A, B, C, D, KC5 );
    EncRoundKeyUpdate1(K+12, A, B, C, D, KC6 );
    EncRoundKeyUpdate0(K+14, A, B, C, D, KC7 );
    EncRoundKeyUpdate1(K+16, A, B, C, D, KC8 );
    EncRoundKeyUpdate0(K+18, A, B, C, D, KC9 );
    EncRoundKeyUpdate1(K+20, A, B, C, D, KC10);
    EncRoundKeyUpdate0(K+22, A, B, C, D, KC11);
#if NoRounds == 16
    EncRoundKeyUpdate1(K+24, A, B, C, D, KC12);
    EncRoundKeyUpdate0(K+26, A, B, C, D, KC13);
    EncRoundKeyUpdate1(K+28, A, B, C, D, KC14);
    EncRoundKeyUpdate0(K+30, A, B, C, D, KC15);
#endif
}



/*
 *
 * Added by Byung-Jun Yoon
 *
 */


void SeedEncryptData(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey)
{
	BYTE_T	*in = input, *out = output;
	long	l;

	for(l=length; l>=SeedBlockSize; l-=SeedBlockSize){
		memcpy(out, in, SeedBlockSize);
		SeedEncrypt_New(out, pdwRoundKey);
		in += SeedBlockSize;
		out += SeedBlockSize;
	}
	if(l!=0){
		memcpy(out, in, l);
		memset(out+l, 0, SeedBlockSize-l);
		SeedEncrypt_New(out, pdwRoundKey);
	}
}

void SeedDecryptData(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey)
{
	BYTE_T	*in = input, *out = output;
	BYTE_T	buf[SeedBlockSize];
	long	l;

	for(l=length; l>=SeedBlockSize; l-=SeedBlockSize){
		memcpy(out, in, SeedBlockSize);
		SeedDecrypt_New(out, pdwRoundKey);
		in += SeedBlockSize;
		out += SeedBlockSize;
	}
	if(l!=0){
		memcpy(buf, in, SeedBlockSize);
		SeedDecrypt_New(buf, pdwRoundKey);
		memcpy(out, buf, l);
	}
}


#if SEED_DO_PADDING

//-----------------------------------
// SEED-ENCRYPT-CBC : with padding 
//-----------------------------------

long SeedEncryptCBC(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey, BYTE_T * iv)
{
	BYTE_T	*in = input, *out = output;
	DWORD_T	dout0, dout1, dout2, dout3;
	DWORD_T	din0, din1, din2, din3;
	DWORD_T	din[4];
	long	l, i;

	C2DW(&iv[0], dout0);
	C2DW(&iv[4], dout1);
	C2DW(&iv[8], dout2);
	C2DW(&iv[12], dout3);

	for(l=length; l>=SeedBlockSize; l-=SeedBlockSize){
		C2DW(&in[0], din0);
		C2DW(&in[4], din1);
		C2DW(&in[8], din2);
		C2DW(&in[12], din3);

		din0 ^= dout0;
		din1 ^= dout1;
		din2 ^= dout2;
		din3 ^= dout3;

		din[0] = din0;
		din[1] = din1;
		din[2] = din2;
		din[3] = din3;

		SeedEncrypt_New((BYTE_T *)din, pdwRoundKey);

		dout0 = din[0];
		dout1 = din[1];
		dout2 = din[2];
		dout3 = din[3];

		DW2C(dout0, &out[0]);
		DW2C(dout1, &out[4]);
		DW2C(dout2, &out[8]);
		DW2C(dout3, &out[12]);

		in += SeedBlockSize;
		out += SeedBlockSize;
	}

	// pad data and encrypt

	memcpy(out, in, l);
	for(i=l; i<SeedBlockSize; ++i){
		out[i] = SeedBlockSize-l;
	}

	C2DW(&out[0], din0);
	C2DW(&out[4], din1);
	C2DW(&out[8], din2);
	C2DW(&out[12], din3);

	din0 ^= dout0;
	din1 ^= dout1;
	din2 ^= dout2;
	din3 ^= dout3;

	din[0] = din0;
	din[1] = din1;
	din[2] = din2;
	din[3] = din3;

	SeedEncrypt_New((BYTE_T *)din, pdwRoundKey);

	dout0 = din[0];
	dout1 = din[1];
	dout2 = din[2];
	dout3 = din[3];

	DW2C(dout0, &out[0]);
	DW2C(dout1, &out[4]);
	DW2C(dout2, &out[8]);
	DW2C(dout3, &out[12]);

	out += SeedBlockSize;

	return (long)(out-output);

}

//-----------------------------------
// SEED-DECRYPT-CBC : with padding
//-----------------------------------

long SeedDecryptCBC(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey, BYTE_T * iv)
{
	DWORD_T	dout0, dout1, dout2, dout3;
	DWORD_T	din0, din1, din2, din3;
	DWORD_T	xor0, xor1, xor2, xor3;
	DWORD_T	din[4];
	BYTE_T	*in = input, *out = output;
	BYTE_T	buf[SeedBlockSize];
	long	l, j, lBytes;

	if(length%SeedBlockSize){
		return -1;
	}

	C2DW(&iv[0], xor0);
	C2DW(&iv[4], xor1);
	C2DW(&iv[8], xor2);
	C2DW(&iv[12], xor3);

	for(l=length; l>SeedBlockSize; l-=SeedBlockSize){

		C2DW(&in[0], din0);
		C2DW(&in[4], din1);
		C2DW(&in[8], din2);
		C2DW(&in[12], din3);

		din[0] = din0;
		din[1] = din1;
		din[2] = din2;
		din[3] = din3;

		SeedDecrypt_New((BYTE_T *)din, pdwRoundKey);

		dout0 = din[0];
		dout1 = din[1];
		dout2 = din[2];
		dout3 = din[3];

		dout0 ^= xor0;
		dout1 ^= xor1;
		dout2 ^= xor2;
		dout3 ^= xor3;

		DW2C(dout0, &out[0]);
		DW2C(dout1, &out[4]);
		DW2C(dout2, &out[8]);
		DW2C(dout3, &out[12]);

		xor0 = din0;
		xor1 = din1;
		xor2 = din2;
		xor3 = din3;

		in += SeedBlockSize;
		out += SeedBlockSize;
	}

	C2DW(&in[0], din0);
	C2DW(&in[4], din1);
	C2DW(&in[8], din2);
	C2DW(&in[12], din3);

	din[0] = din0;
	din[1] = din1;
	din[2] = din2;
	din[3] = din3;

	SeedDecrypt_New((BYTE_T *)din, pdwRoundKey);

	dout0 = din[0];
	dout1 = din[1];
	dout2 = din[2];
	dout3 = din[3];

	dout0 ^= xor0;
	dout1 ^= xor1;
	dout2 ^= xor2;
	dout3 ^= xor3;

	DW2C(dout0, &buf[0]);
	DW2C(dout1, &buf[4]);
	DW2C(dout2, &buf[8]);
	DW2C(dout3, &buf[12]);

	lBytes = (long) buf[SeedBlockSize-1];	// get padding lengths
	if(lBytes>SeedBlockSize || lBytes<=0){
		return -1;
	}
	for(j=0; j<lBytes; ++j){
		if((long)buf[SeedBlockSize-1-j] != lBytes){
			return -1;
		}
	}
	memcpy(out, buf, SeedBlockSize-lBytes);

	return length - lBytes;
}

#else	// NO PADDING

//-----------------------------------
// SEED-ENCRYPT-CBC : NO padding 
//-----------------------------------

void SeedEncryptCBC(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey, BYTE_T * iv)
{
	BYTE_T	*in = input, *out = output;
	DWORD_T	dout0, dout1, dout2, dout3;
	DWORD_T	din0, din1, din2, din3;
	DWORD_T	din[4];
	long	l;

	C2DW(&iv[0], dout0);
	C2DW(&iv[4], dout1);
	C2DW(&iv[8], dout2);
	C2DW(&iv[12], dout3);

	for(l=length; l>=SeedBlockSize; l-=SeedBlockSize){
		C2DW(&in[0], din0);
		C2DW(&in[4], din1);
		C2DW(&in[8], din2);
		C2DW(&in[12], din3);

		din0 ^= dout0;
		din1 ^= dout1;
		din2 ^= dout2;
		din3 ^= dout3;

		din[0] = din0;
		din[1] = din1;
		din[2] = din2;
		din[3] = din3;

		SeedEncrypt_New((BYTE_T *)din, pdwRoundKey);

		dout0 = din[0];
		dout1 = din[1];
		dout2 = din[2];
		dout3 = din[3];

		DW2C(dout0, &out[0]);
		DW2C(dout1, &out[4]);
		DW2C(dout2, &out[8]);
		DW2C(dout3, &out[12]);

//		memcpy(out, in, SeedBlockSize);

		in += SeedBlockSize;
		out += SeedBlockSize;
	}
	if(l!=0){
		memcpy(out, in, l);
		memset(out+l, 0, SeedBlockSize-l);

		C2DW(&out[0], din0);
		C2DW(&out[4], din1);
		C2DW(&out[8], din2);
		C2DW(&out[12], din3);

		din0 ^= dout0;
		din1 ^= dout1;
		din2 ^= dout2;
		din3 ^= dout3;

		din[0] = din0;
		din[1] = din1;
		din[2] = din2;
		din[3] = din3;

		SeedEncrypt_New((BYTE_T *)din, pdwRoundKey);

		dout0 = din[0];
		dout1 = din[1];
		dout2 = din[2];
		dout3 = din[3];

		DW2C(dout0, &out[0]);
		DW2C(dout1, &out[4]);
		DW2C(dout2, &out[8]);
		DW2C(dout3, &out[12]);
	}
}

//-----------------------------------
// SEED-DECRYPT-CBC : NO padding
//-----------------------------------

void SeedDecryptCBC(BYTE_T * input, BYTE_T * output, long length, DWORD_T * pdwRoundKey, BYTE_T * iv)
{
	DWORD_T	dout0, dout1, dout2, dout3;
	DWORD_T	din0, din1, din2, din3;
	DWORD_T	xor0, xor1, xor2, xor3;
	DWORD_T	din[4];
	BYTE_T	*in = input, *out = output;
	BYTE_T	buf[SeedBlockSize];
	long	l;

	C2DW(&iv[0], xor0);
	C2DW(&iv[4], xor1);
	C2DW(&iv[8], xor2);
	C2DW(&iv[12], xor3);

	for(l=length; l>=SeedBlockSize; l-=SeedBlockSize){

		C2DW(&in[0], din0);
		C2DW(&in[4], din1);
		C2DW(&in[8], din2);
		C2DW(&in[12], din3);

		din[0] = din0;
		din[1] = din1;
		din[2] = din2;
		din[3] = din3;

		SeedDecrypt_New((BYTE_T *)din, pdwRoundKey);

		dout0 = din[0];
		dout1 = din[1];
		dout2 = din[2];
		dout3 = din[3];

		dout0 ^= xor0;
		dout1 ^= xor1;
		dout2 ^= xor2;
		dout3 ^= xor3;

		DW2C(dout0, &out[0]);
		DW2C(dout1, &out[4]);
		DW2C(dout2, &out[8]);
		DW2C(dout3, &out[12]);

		xor0 = din0;
		xor1 = din1;
		xor2 = din2;
		xor3 = din3;

		in += SeedBlockSize;
		out += SeedBlockSize;
	}
	if(l!=0){
		C2DW(&in[0], din0);
		C2DW(&in[4], din1);
		C2DW(&in[8], din2);
		C2DW(&in[12], din3);

		din[0] = din0;
		din[1] = din1;
		din[2] = din2;
		din[3] = din3;

		SeedDecrypt_New((BYTE_T *)din, pdwRoundKey);

		dout0 = din[0];
		dout1 = din[1];
		dout2 = din[2];
		dout3 = din[3];

		dout0 ^= xor0;
		dout1 ^= xor1;
		dout2 ^= xor2;
		dout3 ^= xor3;

		DW2C(dout0, &buf[0]);
		DW2C(dout1, &buf[4]);
		DW2C(dout2, &buf[8]);
		DW2C(dout3, &buf[12]);

		memcpy(out, buf, l);
	}
}

#endif

/************************ END ****************************/
