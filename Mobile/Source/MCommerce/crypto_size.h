#ifndef CRYPTO_SIZE__H
#define CRYPTO_SIZE__H

//------------------------------
//
//	crypto_size.h
//
//	2001. 1	by Byung-Jun Yoon
//
//------------------------------

#ifdef  __cplusplus
extern "C" {
#endif


long des_enc_block_size(long length);

long des_enc_block_size_with_padding(long length);

long seed_enc_block_size(long length);

long seed_enc_block_size_with_padding(long length);

long base64_enc_len(long length);

long base64_dec_len(const char * str);
	


#if 0
 
// des size

#define des_enc_block_size(len) (((len)%DES_BLOCK_SIZE)?((len)/DES_BLOCK_SIZE + 1)*DES_BLOCK_SIZE:(len))

#define des_enc_block_size_with_padding(len)    (((len)%DES_BLOCK_SIZE)?((len)/DES_BLOCK_SIZE + 1)*DES_BLOCK_SIZE:(len)+DES_BLOCK_SIZE)

// seed size

#define seed_enc_block_size(len) (((len)%SEED_BLOCK_SIZE)?((len)/SEED_BLOCK_SIZE + 1)*SEED_BLOCK_SIZE:(len))

#define seed_enc_block_size_with_padding(len) (((len)%SEED_BLOCK_SIZE)?((len)/SEED_BLOCK_SIZE + 1)*SEED_BLOCK_SIZE:(len)+SEED_BLOCK_SIZE)

#define base64_enc_len(len)     ((((len)%3)==0)?((len)*4/3):(((len)/3+1)*4))

#endif


#ifdef  __cplusplus
}
#endif

#endif	// CRYPTO_SIZE
