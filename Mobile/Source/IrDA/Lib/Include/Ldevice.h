/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       ldevice.h                                    */
/*             Created:    7/25/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

/*************************************************************
	ASSGIN Phsical device to Logical device
**************************************************************/

/*
	CONSOLE/MODEM DEVICE
*/

#if !defined(__ldevice_h__)
#define	__ldevice_h__

#if 0
#define	ConsGetc			MsmUart1DbgGetc
#define	ConsPutc			MsmUart1DbgPutc
#define	ConsPut				MsmUart1DbgPut
#define	ConsFlush			MsmUart1DbgFlush
#define	ConsSetBaud			MsmUart1DbgSetBaud
#define	ConsRxRead			MsmUart1DbgRxRead
#else
#define	ConsGetc			MsmUart2Getc
#define	ConsPutc			MsmUart2Putc
#define	ConsPut				MsmUart2Put
#define	ConsFlush			MsmUart2Flush
#define	ConsSetBaud			MsmUart2SetBaud
#endif

#define	ConsInit			MsmUart1Init

/*
	IRDA DEVICE
*/
#define	IrdaGetc		MsmUart1Getc
#define	IrdaFlush		MsmUart1Flush
#define	IrdaPutc		MsmUart1Putc
#define	IrdaRxIntOn		SirRxIntOn
#define	IrdaTxIntOn		SirTxIntOn
#define	IrdaIntOff		SirIntOff
#define	IrEyeSetBaud	SirSetBaud

extern	void	SirSetBaud( long baud ) ;

extern	int		MsmUart1Getc( void );
extern	void	MsmUart1Flush( void );
extern	void	MsmUart1Putc( int c );

extern	void	MsmUart1Init(void);
extern	void	MsmUart1Intr(void);
extern	void	MsmUart1DbgPutc(char c);
extern	void	MsmUart1DbgSetBaud( long baud );
extern	int		MsmUart1DbgGetc(void);

extern	void	MsmUart2Init(void);
extern	void	MsmUart2Intr(void);
extern	void	MsmUart2Putc(char c);
extern	void	MsmUart2SetBaud( long baud );
extern	int		MsmUart2Getc(void);
extern	void	MsmUart2Flush( void );



#endif	/*!__ldevice_h__*/
