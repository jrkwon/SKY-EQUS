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
 *     $Header: DCOLDNM.HV  170.12  97/12/17 17:34:40  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* äTóv																		*/
/*		DeepCoreãåñºíËã`													*/
/*																			*/
/* çÏê¨é“																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__dcoldnm_h__)
#define	__dcoldnm_h__


#define	QUEUE			MISCQUEUE

#define	InitRandomize		MiscInitRandomize
#define	Random8				MiscRandom8
#define	Random16			MiscRandom16
#define	Random32			MiscRandom32
#define	InitQueue			MiscInitQueue
#define	FlushQueue			MiscFlushQueue
#define	GetUsedQueueSize	MiscGetUsedQueueSize
#define	GetFreeQueueSize	MiscGetFreeQueueSize
#define	GetReadableQueue	MiscGetReadableQueue
#define	GetWritableQueue	MiscGetWritableQueue
#define	SkipReadQueue		MiscSkipReadQueue
#define	SkipWriteQueue		MiscSkipWriteQueue
#define	GetQueue			MiscGetQueue
#define	SenseQueue			MiscSenseQueue
#define	SetQueue			MiscSetQueue
#define	CopyQueue			MiscCopyQueue
#define	SetBigEndian16		MiscSetBigEndian16
#define	GetBigEndian16		MiscGetBigEndian16
#define	SetBigEndian32		MiscSetBigEndian32
#define	GetBigEndian32		MiscGetBigEndian32


#endif	/*!__dcoldnm_h__*/
