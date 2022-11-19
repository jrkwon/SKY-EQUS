#ifndef HEADER_MD5_H
#define HEADER_MD5_H

#define MD5_CBLOCK	64
#define MD5_LBLOCK	16
#define MD5_BLOCK	16
#define MD5_LAST_BLOCK  56
#define MD5_LENGTH_BLOCK 8
#define MD5_DIGEST_LENGTH 16

typedef struct MD5state_st {
	unsigned long A,B,C,D;
	unsigned long Nl,Nh;
	unsigned long data[MD5_LBLOCK];
	int num;
} MD5_CTX;

void ID_MD5_Init(MD5_CTX *c);
void ID_MD5_Update(MD5_CTX *c, unsigned char *data, unsigned long len);
void ID_MD5_Final(unsigned char *md, MD5_CTX *c);
unsigned char *ID_MD5_Block(unsigned char *d, unsigned long n, unsigned char *md);

#endif
