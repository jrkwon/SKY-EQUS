//----------------------------
//
//	crypto_size.c
//
//	2001. 1 by Byung-Jun Yoon
//
//----------------------------


#include "des.h"
#include "seed.h"
#include "crypto_size.h"

long des_enc_block_size(long length)
{
	return ((length%DES_BLOCK_SIZE)?(length/DES_BLOCK_SIZE + 1)*DES_BLOCK_SIZE:length);	
}

long des_enc_block_size_with_padding(long length)
{
	return ((length%DES_BLOCK_SIZE)?(length/DES_BLOCK_SIZE + 1)*DES_BLOCK_SIZE:(length+DES_BLOCK_SIZE));

}

long seed_enc_block_size(long length)
{
	return ((length%SEED_BLOCK_SIZE)?(length/SEED_BLOCK_SIZE + 1)*SEED_BLOCK_SIZE:length);
}

long seed_enc_block_size_with_padding(long length)
{
	return ((length%SEED_BLOCK_SIZE)?(length/SEED_BLOCK_SIZE + 1)*SEED_BLOCK_SIZE:(length+SEED_BLOCK_SIZE));
}


long base64_enc_len(long length)
{
	return (((length%3)==0)?(length*4/3):((length/3+1)*4));
}

long base64_dec_len(const char * str)
{
	long len, dec_len;

	len = strlen(str);

	if(len%4 || len==0){
		return -1;
	}

	dec_len = len / 4 * 3;

	if(str[--len]=='='){
		--dec_len;
		if(str[--len]=='='){
			--dec_len;
		}
	}

	return dec_len;
}



//
//	End of crypto_size.c
//
//----------------------------
