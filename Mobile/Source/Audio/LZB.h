/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

				Lempel-Ziv 77 변형 LZB 앞축 Algorithm

=============================================================================

                        EDIT HISTORY FOR MODULE

when      who   what, where, why
--------  ---   ------------------------------------------------------------
01-09-06 jwpark add comments
01-09-03 jwpark created
=============================================================================*/


#ifndef LZB_H_2001_09_03_JWPARK
#define LZB_H_2001_09_03_JWPARK


typedef enum {
  LZB_MATCH		= 0,
  LZB_SINGLE	= 1,
  LZB_MAX_PAD	= 0xFFFF
} LZBHeaderType;


#ifdef _LZB_WITH_PC
#include <stdio.h>
#else
#include "comdef.h"
#endif // _LZB_WITH_PC


#ifdef _LZB_WITH_PC
typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned long	dword;

typedef enum { LZB_FAILURE	= 0, LZB_SUCCESS= 1 } LZBReturnType;

typedef struct _LZBListType LZBListType;
typedef struct _LZBListType {
  LZBHeaderType			 header;
  char					 singleData;
  long					 offset;
  long					 match;
  struct _LZBListType	*next;
} LZBListType;

typedef struct {
  long offsetBits;
  long matchBits;
  long compressSize;
} LZBInfoType;


///////////////////////////////////////////////////////////////////////////////
//
// 이름:
//	LZBCompress()
//
// 내용:
//	data를 압축하여 compress data list를 만든다.
// 
///////////////////////////////////////////////////////////////////////////////
LZBReturnType LZBCompress(char         *data,
						  long          size,
						  long          maxOffset,
						  long          maxMatch,
						  LZBListType **compressList);


///////////////////////////////////////////////////////////////////////////////
//
// 이름:
//	LZBCheckList()
//
// 내용:
//	compress data list와 original data가 서로 같은 지를 검사한다.
// 
///////////////////////////////////////////////////////////////////////////////
LZBReturnType LZBCheckList(char        *data,
						   long		    size,
						   LZBListType *list);


///////////////////////////////////////////////////////////////////////////////
//
// 이름:
//	LZBGetInfo()
//
// 내용:
//	compress data list를 압축할 경우 압축파일의 크기와 offset, math의 bit수를
//  구한다.
// 
///////////////////////////////////////////////////////////////////////////////
LZBInfoType LZBGetInfo(LZBListType *list);



///////////////////////////////////////////////////////////////////////////////
//
// 이름:
//	LZBWriteList()
//
// 내용:
//	compress data list를 파일에 저장한다.
// 
///////////////////////////////////////////////////////////////////////////////
void LZBWriteList(FILE		  *stream,
				  LZBListType *list,
				  long		   offsetBits,
				  long		   matchBits,
				  long		   size);


///////////////////////////////////////////////////////////////////////////////
//
// 이름:
//	LZBCheckFile()
//
// 내용:
//	압축된 파일의 내용과 original data를 비교한다.
// 
///////////////////////////////////////////////////////////////////////////////
LZBReturnType LZBCheckFile(byte	 *cdata,
						   dword  csize,
						   byte	 *odata,
						   dword  osize);


///////////////////////////////////////////////////////////////////////////////
//
// 이름:
//	LZBFindOptimal()
//
// 내용:
//	압축할 때 사용할 optimal offset, math의 bit 수를 구한다.
// 
///////////////////////////////////////////////////////////////////////////////
LZBReturnType LZBFindOptimal(char *data,
							 long  size,
							 long *optimalOffsetBits,
							 long *optimalMatchBits);
#endif // _LZB_WITH_PC


///////////////////////////////////////////////////////////////////////////////
//
// 이름:
//	LZBDecompress()
//
// 내용:
//	cdata에 있는 압축 data를 odata에 풀어 넣는다.
// 
// 주의:
//  odata는 cdata를 풀었을 때 필요한 공간이 충분히 확보되어 있어야 한다.
// 
///////////////////////////////////////////////////////////////////////////////
void LZBDecompress(dword *cdata,	// compressed data buffer
				   byte	 *odata);	// decompressed data buffer


#endif // LZB_H_2001_09_03_JWPARK
