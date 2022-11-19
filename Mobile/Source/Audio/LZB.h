/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

				Lempel-Ziv 77 ���� LZB ���� Algorithm

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
// �̸�:
//	LZBCompress()
//
// ����:
//	data�� �����Ͽ� compress data list�� �����.
// 
///////////////////////////////////////////////////////////////////////////////
LZBReturnType LZBCompress(char         *data,
						  long          size,
						  long          maxOffset,
						  long          maxMatch,
						  LZBListType **compressList);


///////////////////////////////////////////////////////////////////////////////
//
// �̸�:
//	LZBCheckList()
//
// ����:
//	compress data list�� original data�� ���� ���� ���� �˻��Ѵ�.
// 
///////////////////////////////////////////////////////////////////////////////
LZBReturnType LZBCheckList(char        *data,
						   long		    size,
						   LZBListType *list);


///////////////////////////////////////////////////////////////////////////////
//
// �̸�:
//	LZBGetInfo()
//
// ����:
//	compress data list�� ������ ��� ���������� ũ��� offset, math�� bit����
//  ���Ѵ�.
// 
///////////////////////////////////////////////////////////////////////////////
LZBInfoType LZBGetInfo(LZBListType *list);



///////////////////////////////////////////////////////////////////////////////
//
// �̸�:
//	LZBWriteList()
//
// ����:
//	compress data list�� ���Ͽ� �����Ѵ�.
// 
///////////////////////////////////////////////////////////////////////////////
void LZBWriteList(FILE		  *stream,
				  LZBListType *list,
				  long		   offsetBits,
				  long		   matchBits,
				  long		   size);


///////////////////////////////////////////////////////////////////////////////
//
// �̸�:
//	LZBCheckFile()
//
// ����:
//	����� ������ ����� original data�� ���Ѵ�.
// 
///////////////////////////////////////////////////////////////////////////////
LZBReturnType LZBCheckFile(byte	 *cdata,
						   dword  csize,
						   byte	 *odata,
						   dword  osize);


///////////////////////////////////////////////////////////////////////////////
//
// �̸�:
//	LZBFindOptimal()
//
// ����:
//	������ �� ����� optimal offset, math�� bit ���� ���Ѵ�.
// 
///////////////////////////////////////////////////////////////////////////////
LZBReturnType LZBFindOptimal(char *data,
							 long  size,
							 long *optimalOffsetBits,
							 long *optimalMatchBits);
#endif // _LZB_WITH_PC


///////////////////////////////////////////////////////////////////////////////
//
// �̸�:
//	LZBDecompress()
//
// ����:
//	cdata�� �ִ� ���� data�� odata�� Ǯ�� �ִ´�.
// 
// ����:
//  odata�� cdata�� Ǯ���� �� �ʿ��� ������ ����� Ȯ���Ǿ� �־�� �Ѵ�.
// 
///////////////////////////////////////////////////////////////////////////////
void LZBDecompress(dword *cdata,	// compressed data buffer
				   byte	 *odata);	// decompressed data buffer


#endif // LZB_H_2001_09_03_JWPARK
