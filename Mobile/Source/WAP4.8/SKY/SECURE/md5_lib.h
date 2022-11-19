#ifndef _SSLS_MD5_H
#define _SSLS_MD5_H

/* 	MD5 hash function */
int computeMD5(unsigned char *cpIn, unsigned long ulLen, unsigned char *cpOut);

/*	keyed MD5 hash function */
void compute_H_MD5(
    	unsigned char *cpIn, unsigned long ulLen,
    	unsigned char *cpKey, unsigned long ulKeyLen,
    	unsigned char *cpOut, unsigned long *ulpOutLen
);

#endif
