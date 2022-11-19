/*
 *     IrDA Driver DeepCore(tm) Version 1
 *
 *     Copyright(c) 1996-97 Okaya Systemware Co.,Ltd.
 *     All Rights Reserved
 *
 *     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware
 *     The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *     $Header: misc.hv  170.27  98/02/20 15:01:50  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* 概要																		*/
/*		汎用関数定義														*/
/*																			*/
/* 作成者																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if	!defined(__misc_h__)
#define		__misc_h__

#include	"irda.h"
#include	"dcoldnm.h"


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		リング構造データキュー												*/
/*																			*/
/* 2.概要																	*/
/*		リング構造になったデータキューを管理する。							*/
/*																			*/
/* 3.機能説明																*/
/*		リング構造になったデータキューを管理する。							*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_MISCQUEUE	{
	byte	IrDAFAR *Data ;		/*使用するバッファのアドレス				*/
	uint		DataSize ;		/*使用するバッファのサイズ					*/
	uint		ReadPtr ;		/*読み出しポインタ（インデックス）			*/
	uint		WritePtr ;		/*書き込みポインタ（インデックス）			*/
}	MISCQUEUE ;


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/

/*  -----  Public Functions  -----  */

void		MiscInitRandomize( void ) ;
byte		MiscRandom8( byte max ) ;
word		MiscRandom16( word max ) ;
dword		MiscRandom32( dword max ) ;
#if defined(NOSTDLIB)
void		*memcpy( void *, const void *, word ) ;
int			_memcmp( const void *des, const void *src, word len ) ;
void		memset( void *, byte c, word len ) ;
#endif	/*NOSTDLIB*/

void		MiscInitQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR *buf, uint len ) ;
void		MiscFlushQueue( MISCQUEUE IrDAFAR *q ) ;
uint		MiscGetUsedQueueSize( MISCQUEUE IrDAFAR *q ) ;
uint		MiscGetFreeQueueSize( MISCQUEUE IrDAFAR *q ) ;
uint		MiscGetReadableQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR * IrDAFAR *ptr ) ;
uint		MiscGetWritableQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR * IrDAFAR *ptr ) ;
void		MiscSkipReadQueue( MISCQUEUE IrDAFAR *q, uint len ) ;
void		MiscSkipWriteQueue( MISCQUEUE IrDAFAR *q, uint len ) ;
uint		MiscGetQueue( void IrDAFAR *des, MISCQUEUE IrDAFAR *q, uint len ) ;
uint		MiscSenseQueue( void IrDAFAR *des, MISCQUEUE IrDAFAR *q, uint len ) ;
uint		MiscSetQueue( MISCQUEUE IrDAFAR *q, const void IrDAFAR *src, uint len ) ;
void		MiscCopyQueue( MISCQUEUE IrDAFAR *dq, MISCQUEUE IrDAFAR *sq ) ;

uint	IrDAMISCAPI	MiscSetBigEndian16( void IrDAFAR *ptr, word var ) ;
uint	IrDAMISCAPI	MiscGetBigEndian16( const void IrDAFAR *ptr, word IrDAFAR *varp ) ;
uint	IrDAMISCAPI	MiscSetBigEndian32( void IrDAFAR *ptr, dword var ) ;
uint	IrDAMISCAPI	MiscGetBigEndian32( const void IrDAFAR *ptr, dword IrDAFAR *varp ) ;

#if defined(SEQCHK)
#define		SEQHNDLSIZE		20
void		InitSequencer( void ) ;
IrDABOOL		CheckDataSequence( int hndl, byte IrDAFAR *ptr, uint size ) ;
IrDABOOL		CheckQueue( int hndl, MISCQUEUE IrDAFAR *q ) ;
#endif	/*SEQCHK*/

#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/

#endif		/*__misc_h__*/
