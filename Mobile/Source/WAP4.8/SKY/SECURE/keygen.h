#ifndef _KEYGEN_H
#define _KEYGEN_H

/* 	str label for constructing master secret */
#define MSEC_LABLE				"master secret"

/* 	str label for constructing key block */
#define KEY_BLOCK_LABEL			"key expansion"

/* 	max hash value size */
#define MAXHASHLEN				20

/* 	max number of the buffer used to store A(i) values */ 
#define MAX_A_CNT				10

/*	A(i) concatenation count */
#define A_CNT_MD5				5

/* 	A(i) */
typedef struct {
	ul 	ulLen;
	uc	ucHash[MAXHASHLEN];
}	A_Val;

/*	structure to store each A(i) value */
typedef struct {
	int 			nCnt;
	A_Val			arHash[MAX_A_CNT];
}	A_List;

/*	
	compute PRF 
	argument of PRF is secret, label and seed
*/
void computePRF(
    uc *ucOut,      ul *ulpOutLen,
    uc *ucSecret,   ul ulSecLen,
    uc *ucLabel,    ul ulLabelLen,
    uc *ucSeed,     ul ulSeedLen
);

/*	MD5 hash function declaration */
int computeMD5(
	uc *cpIn, ul ulLen, uc *cpOut
);

/*	MD5 keyed hash function declaration */
void compute_H_MD5(
    uc *cpIn, 	ul ulLen,
    uc *cpKey, 	ul ulKeyLen,
    uc *cpOut, 	ul *ulpOutLen
);

/* 	
	compute master secret 
	argument of the function is 
		premaster secret, client random and server random
*/
void computeMasterSecret(
    uc *ucOut,              ul *ulpOutLen,
    uc *ucPreMasterSecret,  ul ulSecLen,
    uc *ucCliRandom,        ul ulCliRandomLen,
    uc *ucServRandom,       ul ulServRandomLen
);

/*
	compute key block 
	argument of the function is 
		master secret, expansion label, client random and server random
*/
void genKeyBlock(
    uc *ucKeyBlock,     ul *ulpKeyBlockLen,
    uc *ucMasterSecret, ul ulSecLen,
    uc *ucCliRandom,    ul ulCliRandomLen,
    uc *ucServRandom,   ul ulServRandomLen
);

/*
	extract cryptographic keys from key block
	each key is MAC key, encryption key and initialization vector
	in some encryption algorithm, initialization vector might be useless 
*/
void getKeyItem(
	uc *ucKeyBlock, 
	uc *ucMacKey, 	uc *ucpMacKeyLen, 
	uc *ucEncKey, 	uc *ucpEncKeyLen,
	uc *ucIvKey,  	uc *ucpIvKeyLen
);

#endif
