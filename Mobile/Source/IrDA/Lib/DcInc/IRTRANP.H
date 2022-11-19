/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1997 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       irtranp.h                                    */
/*             Created:    9/28/97                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/
/*************************************************

	IrTran-P Prtocol Header File

**************************************************/

#ifndef	_IRTRANP_H_
#define	_IRTRANP_H_


#define	IrTranP_NOACTIVE			0
#define	IrTranP_PRIMARY				1
#define	IrTranP_SECONDARY			2


#define	IrTranP_ACK					0
#define	IrTranP_NACK				1

#define	IrTranPR_ACCEPT				0
#define	IrTranPR_PDU_BUSY			-1
#define	IrTranPR_PDU_ERROR			-2
#define	IrTranPR_UNDEFINED_ERROR	-3
#define	IrTranPR_INVALID_STATE		-4
#define	IrTranPR_INVALID_PARAM		-5
#define	IrTranPR_PARAM_TOLONG		-6
#define	IrTranPR_PRGRESS			-7

#define	IrTranPr_Busy				-10
#define	IrTranPR_NOTBIND			-100

#ifdef	_DOS
#define	TRANPFAR	__huge
#else
#define	TRANPFAR
#endif


/* SCEP Service APIs	*/
                    
#include	<TrnpPdu.h>
#include	<TrnpSCEP.h>
#include	<TrnpBftp.h>

#endif

