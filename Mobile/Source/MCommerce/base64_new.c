/*
 *
 * Encode or decode file as MIME base64 (RFC 1341)
 * Port of John Walker's base64 program to library
 * by Joel Reed
 *
 * Modified by Byung-Jun Yoon for buffer I/O
 *
 */

#include <string.h>
#include "base64.h"

BYTE_T Base64Table[256];	      /* Encode / decode table */


/* 
 * ENCODE  --	Encode binary file into base64.
 *
 *  returns the length of the encoded data (-1 if error)
 *
 */

	
int _encodeBase64(const BYTE_T * input, long length, int nLineLen, BYTE_T * output)
{
	int i;
	int op = 0;			// output buffer pointer
	int len = 0;		// input buffer pointer 
	int hitend = 0;		// show whether we have hit the end
	int nCurLen = 0;

	if(nLineLen==0){
		nLineLen = BASE64_LINE_LENGTH;
	}

	/*	Fill Base64Table with character encodings.  */

	for (i = 0; i < 26; i++) {
		Base64Table[i] = 'A' + i;
		Base64Table[26 + i] = 'a' + i;
	}
	for (i = 0; i < 10; i++) {
		Base64Table[52 + i] = '0' + i;
	}
	Base64Table[62] = '+';
	Base64Table[63] = '/';

	while (!hitend) {
		BYTE_T igroup[3], ogroup[4];
		int c=0, n;

		igroup[0] = igroup[1] = igroup[2] = 0;
		for (n = 0; n < 3; n++) {
			c = input[len];
			len++;

			if (len>length) {
				hitend = 1;
				break;
			}
			igroup[n] = (BYTE_T) c;
		}
		if (n > 0) {
			ogroup[0] = Base64Table[igroup[0] >> 2];
			ogroup[1] = Base64Table[((igroup[0] & 3) << 4) | (igroup[1] >> 4)];
			ogroup[2] = Base64Table[((igroup[1] & 0xF) << 2) | (igroup[2] >> 6)];
			ogroup[3] = Base64Table[igroup[2] & 0x3F];

			/* Replace characters in output stream with "=" pad
			   characters if fewer than three characters were
			   read from the end of the input stream. */

			if (n < 3) {
				ogroup[3] = '=';
				if (n < 2) {
					ogroup[2] = '=';
				}
			}
			for (i = 0; i < 4; i++) {
				output[op] = ogroup[i];
				op++;
				nCurLen++;
				if(nLineLen>0){
					if(nCurLen==nLineLen){
						nCurLen = 0;
						output[op] = '\n';
						op++;
					}
				}
			}
		}
	}

	if(output[op-1] == '\n'){
		output[op-1] = '\0';
		op--;
	}
	else{
		output[op] = '\0';
	}

	return op;
}


/* 
 * DECODE  --	Decode base64.
 *
 * returns the length of the decoded data (-1 if error)
 *
 */

int decodeBase64(const BYTE_T * input, long length, BYTE_T * output)
{
	int i;
	int op = 0;			// output buffer pointer
	int len = 0;		// input buffer pointer 
	int hitend = 0;		// show whether we have hit the end

	for (i = 0; i < 255; i++) {
		Base64Table[i] = 0x80;
	}
	for (i = 'A'; i <= 'Z'; i++) {
		Base64Table[i] = 0 + (i - 'A');
	}
	for (i = 'a'; i <= 'z'; i++) {
		Base64Table[i] = 26 + (i - 'a');
	}
	for (i = '0'; i <= '9'; i++) {
		Base64Table[i] = 52 + (i - '0');
	}
	Base64Table['+'] = 62;
	Base64Table['/'] = 63;
	Base64Table['='] = 0;

	/*CONSTANTCONDITION*/

	while (!hitend) {
		BYTE_T a[4], b[4], o[3];

		for (i = 0; i < 4; i++) {
			int c = input[len];
			len++;

			if (len>length) {
				hitend = 1;
				output[op] = '\0';
				return op;
			}
			if (Base64Table[c] & 0x80) {
				i--;
				continue;
			}
			a[i] = (BYTE_T) c;
			b[i] = (BYTE_T) Base64Table[c];
		}
		o[0] = (b[0] << 2) | (b[1] >> 4);
		o[1] = (b[1] << 4) | (b[2] >> 2);
		o[2] = (b[2] << 6) | b[3];
		i = a[2] == '=' ? 1 : (a[3] == '=' ? 2 : 3);

		memcpy(&output[op], o, i);
		op += i;

		if (i < 3){
				output[op] = '\0';
				return op;
		}
	}
	return -1;
}

