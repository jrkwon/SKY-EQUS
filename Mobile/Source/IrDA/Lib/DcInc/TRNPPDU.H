/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1997 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       trnppdu.h                                    */
/*             Created:    9/28/97                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/

#ifndef	_TRNPPDU_H_
#define	_TRNPPDU_H_

extern	void	PDU_Enter(void);
extern	void	PDU_Exit(void);
extern	void	PDU_Mode(int Mode);
extern	word	PDU_isConnected(void );
extern	int		PDU_ConnectReq(void );
extern	int		PDU_DisconnectReq(void );
extern	int		PDU_PrimaryProc(void );

extern	int		PDU_QueryTxBusy(int size);
extern	int		PDU_SendTxData(byte *Data,int Size);

extern	void	PDU_DispData(void);

extern	void	PDU_PollData(void);

#endif
