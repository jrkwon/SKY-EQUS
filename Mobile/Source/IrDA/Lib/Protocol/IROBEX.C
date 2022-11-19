/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       irobex.c                                     */
/*             Created:    7/25/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

#include	<irdaconf.h>

#if defined(USE_IrOBEX)

#include	<string.h>
#define	OBEXMAX			IrOBEX_LSAPMAX
#define	OBEXTXBUFSZ		IrOBEX_TXBUFSZ
#define	OBEXRXBUFSZ		IrOBEX_RXBUFSZ
#include	<pobex.h>
#endif

