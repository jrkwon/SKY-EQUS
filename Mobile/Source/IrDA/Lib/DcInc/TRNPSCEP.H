/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1997 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       trnpscep.h                                   */
/*             Created:    9/28/97                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/

/**************************************************************

	IrTran-P SCEP Protocol Header Ver 1.0

***************************************************************/
/*--------------------------------------------------------------------------*/
/*																			*/
/* 概要																		*/
/*		IrTran-P SCEPプロトコル定義											*/
/*																			*/
/* 作成者																	*/
/*		kitazumi@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/

#ifndef	_TRNPSCEP_H_
#define	_TRNPSCEP_H_

#include	<ScepData.h>

/*	SCEP Buffer size	*/

#define	SCEP_PDU_SIZE1		 512
#define	SCEP_PDU_SIZE2		1024
#define	SCEP_PDU_SIZE3		2048
#define	SCEP_PDU_SIZE4		4096

/*	SCEP VALUE			*/

#define	SCEP_SYNC_CHAR		0x00

/* Message Type			*/
#define	SCEP_MTYPE_CONREQ	0x10
#define	SCEP_MTYPE_CONCNF	0x11
#define	SCEP_MTYPE_COMMAND	0x20
#define	SCEP_MTYPE_DISCON	0x30

/* Information Type		*/
#define	SCEP_ITYPE_VERSION	0x00
#define	SCEP_ITYPE_NEGINF	0x01
#define	SCEP_ITYPE_USERDATA	0x03
#define	SCEP_ITYPE_EXTEND	0x10
#define	SCEP_ITYPE_REASON	0x20

/* Information Version	*/
#define	SCEP_INFVER			0x10

/* Command Flag			*/
#define	SCEP_CFLG_NOEXEC	0x00
#define	SCEP_CFLG_EXECUTE	0x04

/* use Length extention Next 2 bytes	*/

#define	SCEP_USELEN2		0xff

/* Ptype 					*/
#define	SCEP_PTYPE_MASK		0xc0
#define	SCEP_PTYPE_REQ		0x00
#define	SCEP_PTYPE_RPL_ACK	0x40
#define	SCEP_PTYPE_RPL_NACK	0x80
#define	SCEP_PTYPE_ABORT	0xc0

/* Data Flag	*/
#define	SCEP_DFLG_NOSEG		0xc0
#define	SCEP_DFLG_SINGL		0xc1
#define	SCEP_DFLG_STOP		0xc2
#define	SCEP_DFLG_NACK		0xc3

#define	SCEP_DFLG_SEG1		0x41
#define	SCEP_DFLG_SEGI		0x01
#define	SCEP_DFLG_SEGE		0x81

#define	SCEP_CMDHDR			0x58

/* SCEP Status	*/
#define	SCEP_ST_DISC		0
#define	SCEP_ST_CONREQ		1
#define	SCEP_ST_CONIND		2
#define	SCEP_ST_CONRES		3
#define	SCEP_ST_CONCNF		4

#define	SCEP_ST_CMDREQ		10
#define	SCEP_ST_CMDIND		11
#define	SCEP_ST_CMDRES		12
#define	SCEP_ST_CMDCNF		13
#define	SCEP_ST_CMDABT		14
#define	SCEP_ST_STOP		15

#define	SCEP_ST_CMDPRC		19

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		コールバック関数テーブル											*/
/*																			*/
/* 2.概要																	*/
/*		各種イベント発生時に呼び出すコールバック関数を管理する。			*/
/*																			*/
/* 3.機能説明																*/
/*		bFTP で各種イベント発生時に呼び出すコールバック関数を管理する。		*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct t_SCEPTBL {
		word	Use;							/* Use Table Flag 				*/ 
		byte	*PduBuffer;           			/* Define Pdu Buffer Address	*/
		int		PduSize;
		
		byte	*Address;						/* Machine Address			*/
		byte	*NegInf;						/* Negotiation Info			*/
		int		NegSize;						/* Negotiation info size	*/


		/* PDU Interface				*/
		void	(*CBPduEnter)(void);
		void	(*CBPduExit)(void);
		void	(*CBPduMode)(int Mode);

		/* SCEP Connection Callbacks	*/
		void	(*CBConInd)(byte CFLG,byte *ReqAddr,byte *YourAddr,byte *YourNegInf,word YourNegSize);
		void	(*CBConCnf)(byte CFLG,byte *YourAddr,byte *YourNegInf,word YourNegSize);
		void	(*CBCmdIdInd)(word CmdId);
		void	(*CBStpInd)(void );
		void	(*CBDiscInd)(word Reason);

		/* SCEP Req/Rpl Callbacks		*/
	 	void	(*CBCmdHdrInd)(	byte *DstAddr,byte *SrcAddr,word SelfPid,word YourPid,word CmdId,byte PduType);
		void 	(*CBCmdBodyInd)(	byte DFLG,dword SeqNo,dword RestNo,byte *pData,word len);
		void	(*CBCmdCountInd)(	byte DFLG,dword SeqNo,dword RestNo);

} SCEPTBL;


extern	void	SCEP_HexDump(char *Msg,byte *Data,dword Size);
extern	void	SCEP_HexPrint(byte *Data,size_t Size);
extern	void	SCEP_DispAddr(void);

extern dword	SCEP_GetBig(byte *dp,int size);
extern byte		*SCEP_PutBig(byte *dp,dword data,int size);

extern	int		SCEP_GetStatus();
extern	size_t	SCEP_GetDstDataSize(void);
extern	void	SCEP_BindPdu(int PduType,SCEPTBL *Stbl);
extern	void	SCEP_UnbindPdu(void);

extern	void	SCEP_SetNegInf(byte *MyAddr,byte *NegInf,word Size);
extern	int		SCEP_SetPduBuffer(byte *Buffer,int Size);

extern	int		SCEP_QueryPduBusy(void );

extern	int 	SCEP_ConnectReq(byte *SecAddr,byte CFLG);
extern	int		SCEP_ConnectRes(byte AckOrNack);
extern	int 	SCEP_DisconnectReq(word Reason);

extern	int 	SCEP_CommandSglPdu(word DstPid,word SrcPid,byte PduType,byte *Data,size_t DataLen);
extern	int 	SCEP_CommandHdrPdu(word DstPid,word SrcPid,byte PduType,dword DataLen);
extern	int 	SCEP_CommandNextPdu(byte DFLG,byte *UserData,dword DataLen);

extern	word 	SCEP_PreCommandSegPdu(void *pData,	word DstPid,word SrcPid,byte PduType,dword DataLen);
extern	int 	SCEP_SendCommandSegPdu(void);


extern	void	SCEP_Sequence(byte c);

#endif
