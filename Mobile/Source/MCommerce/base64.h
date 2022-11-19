#ifndef BASE64__H
#define BASE64__H

#include "crypto_types.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define BASE64_LINE_LENGTH					64

#define encodeBase64(in, len, out)			_encodeBase64(in, len, -1, out)
#define encodeBase64n						_encodeBase64

int _encodeBase64(const BYTE_T * input, long length, int nLineLen, BYTE_T * output);

int decodeBase64(const BYTE_T * input, long length, BYTE_T * output);

#ifdef  __cplusplus
}
#endif

#endif // BASE64__H
