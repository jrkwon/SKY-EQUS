//----------------------------------
//
//	esc2str.c
//
//	convert 4 byte escape character
//	sequence to string
//
//	2001. 1. by Byung-Jun Yoon
//
//----------------------------------

#include <string.h>
#include "ctype.h"
#include "esc2str.h"


static const char HEX_VAL_STR[] = 

{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
/*	 0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,	*/
	'a', 'b', 'c', 'd', 'e', 'f'
/*	 10,  11,  12,  13,  14,  15	*/
};


int hex2int(char c)
{
	int i;
	char cHex = isupper(c)?(c+'a'-'A'):c;

	for(i=0; i<16; ++i){
		if(cHex==HEX_VAL_STR[i]){
			return i;
		}
	}
	
	return -1;
}


int getHexValue(const char * escapedBlock)
{
	const char * p = escapedBlock;
	int nVal, nTemp;

	if(*(p++)!='\\'){
		return -1;
	}

	if(*p!='x' && *p!='X'){
		return -1;
	}

	if((nVal = hex2int(*(++p)))==-1){
		return -1;
	}
	if((nTemp = hex2int(*(++p)))==-1){
		return -1;
	}

	nVal = nVal<<4 | nTemp;
	
	return nVal;
}


long esc2str(const char * escapedStr, unsigned char * str)
{
	long lTemp, lOffset=0;
	char * p;

	lTemp = strlen(escapedStr);
	if(lTemp%4){
		return -1;
	}

	for(lOffset=0, p=(char *)escapedStr; *p!='\0'; p+=4, lOffset++){
		if((lTemp = getHexValue(p))==-1){
			return -1;
		}
		str[lOffset] = (unsigned char)lTemp;
	}

	return lOffset;
	
}

