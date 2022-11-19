/************************************************************************
;*                                                                      *
;*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      *
;*     All Rights Reserved                                              *
;*                                                                      *
;*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  *
;*     The copyright notice above does not evidence any                 *
;*     actual or intended publication of such source code.              *
;*                                                                      *
;*             File:       sktest.h          		                    *
;*             Created:    05/21/99                                     *
;*             Author:     Katsuhiko Takagaki							*
;*                                                                      *
;*             Version:    1.7                                          *
;*                                                                      *
;***********************************************************************/

#if !defined(__sktest_h__)
#define	__sktest_h__

#incude		"irda.h"
extern	int			MDM_Slsap;
extern	int			MDM_Status;		/* Modem Conneting?				*/


extern	void		MDM_InitProc(void)

#endif	/*!__sktest_h__*/

