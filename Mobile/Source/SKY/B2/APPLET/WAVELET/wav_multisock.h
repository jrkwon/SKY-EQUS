/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     wav_multisock.h
		(Wavelet Multi-Socket)
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/07/30   yoursun       Created.
**  
** ===========================================================================
*/

#ifndef WAV_MSOCK_H

#define WAV_MSOCK_H

//#define NEW_LIB

#define WAV_SOCK_SUCCESS		1
#define WAV_SOCK_WOULDBLOCK		0
#define WAV_SOCK_ERROR			-1

#define UDP_PORT1	1100
#define UDP_PORT2	1101

void MM_InitSocket(void);
int MM_OpenSocket(void);
int MM_PPPOpen(void);
int MM_PPPClose(void);
int MM_ReleaseSocket(void);
int MM_Connect(char* ip,  WORD port);

//#ifdef NEW_LIB
//void	MM_SendUDPData1(BYTE *pData, WORD nSize, WORD port);  // yoursun 01/07/27 
//#else
void	MM_SendUDPData1(BYTE *pData, WORD nSize);  // yoursun 01/07/27 
//#endif // NEW_LIB

WORD	MM_RecvUDPData1(BYTE *pData, WORD nSize);  // yoursun 01/07/27 
//#ifdef NEW_LIB
//void	MM_SendUDPData2(BYTE *pData, WORD nSize, WORD port); // yoursun 01/07/27 
//#else
void	MM_SendUDPData2(BYTE *pData, WORD nSize);  // yoursun 01/07/27 
//#endif // NEW_LIB

WORD	MM_RecvUDPData2(BYTE *pData, WORD nSize);  // yoursun 01/07/27 

void	MM_SendTCPData(BYTE *pData, WORD nSize); // yoursun 01/07/27 
WORD	MM_RecvTCPData(BYTE *pData, WORD nSize); // yoursun 01/07/27 

#endif //WAV_MSOCK_H