/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1997 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       trnpbftp.h                                   */
/*             Created:    9/28/97                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/

/*--------------------------------------------------------------------------*/
/*																			*/
/* 概要																		*/
/*		IrTran-P Bftpプロトコル定義											*/
/*																			*/
/* 作成者																	*/
/*		kitazumi@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/

#ifndef	_TRNPBFTP_H_
#define	_TRNPBFTP_H_

#define	BFTP_PID		0x0008			/* BFTP PID		*/
#define	BFTP_SONY		0x00ff			/* SONY BUG ID	*/

#define	BFTP_FILE_NAME_SIZE	256

#define	BFTP_CTYPE_PUT		0x00000000
#define	BFTP_CTYPE_QUERY	0x00010040

#define	BFTP_ATYPE_BINARY	0x00
#define BFTP_ATYPE_TEXT		0x01
#define	BFTP_ATYPE_TIME		0x06

#define	BFTP_AFLG_NOSUP		0x00

#define	BFTP_ANAME_ERROR	"ERR0"
#define	BFTP_ANAME_REPLY	"RPL0"
#define	BFTP_ANAME_COMMAND	"CMD0"

#define	BFTP_ANAME_QUERY	"WHT0"

#define	BFTP_ANAME_BODY		"BDY0"
#define	BFTP_ANAME_FILE		"FIL0"
#define	BFTP_ANAME_LFILE    "LFL0"

#define	BFTP_AVALUE_QIMAGE	"RIMG"
#define	BFTP_AVALUE_QINFO	"RINF"
#define	BFTP_AVALUE_QCMD 	"RCMD"

#define	BFTP_ERROR_DATA		0x0001
#define	BFTP_ERROR_PID		0x0002
#define	BFTP_ERROR_ATTR		0x0010
#define	BFTP_ERROR_COMMAND	0x0011
#define	BFTP_ERROR_FILEFULL	0x0020
#define	BFTP_ERROR_NOFILE	0x0021
#define	BFTP_ERROR_BATTLOW	0x0030
#define	BFTP_ERROR_CMDABORT	0x0031
#define	BFTP_ERROR_UNDEFINE	0x0000


#define	BFTP_ST_OTHER_IND		0
#define	BFTP_ST_QUERY_IND		1
#define	BFTP_ST_PUT_IND			2


#define	BFTP_ST_ACK_IND			11
#define	BFTP_ST_NACK_IND		12

#define	BFTP_ST_ABORT_IND		90

#define	BFTP_ST_INVALID		-1

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
typedef struct t_BFTPTBL {

	int	 Use;						/* 使用中フラグ							*/
	byte TRANPFAR 	*Buffer;		/* ワークエリアのアドレス				*/
	dword			BufferSize;		/* ワークエリアのバイトサイズ			*/
	void (*CBPutInd)(	byte TRANPFAR *FileName,byte TRANPFAR *FileBody,dword Size);	/* Put Indication	*/
	void (*CBQueryInd)(	F_ATTHDR TRANPFAR * pAttr,word AttNum);
	void (*CBAckInd)(	F_ATTHDR TRANPFAR * pAttr,word AttNum);
	void (*CBNackInd)(	F_ATTHDR TRANPFAR * pAttr,word AttNum);
	void (*CBAbortInd)(	void );

} BFTPTBL;


extern	void			Bftp_BindSCEP(BFTPTBL *Btbl);
extern	void			Bftp_UnbindSCEP(void);

extern	void 			Bftp_DispAttribute(F_ATTHDR TRANPFAR *pAttr,word AttNum);

extern	void			Bftp_SetUserBuffer(byte TRANPFAR *Buffer,dword Size);
extern	int				Bftp_RplSglCnack(word ErrCode);
extern	int				Bftp_RplSglCack(byte *Value,dword Size);
extern	int				Bftp_RplQueryCack(byte *Value,dword Size);

extern	int				Bftp_QueryReq(byte *Value,dword Size);

extern	byte TRANPFAR	*Bftp_SetFileHeader(char *FileName,word FileNameLen,dword FileSize);

extern	void			Bftp_CBCmdHdrInd(byte *DstAddr,byte *SrcAddr,word DstPid,word SrcPid,word CmdId,byte PduType);
extern	void			Bftp_CBCmdBodyInd(byte DFLG,dword SeqNo,dword RestNo,byte *pData,word DataSize);

extern	int				Bftp_PutReq(dword Size);
extern	int				Bftp_PutNext(void );

extern	byte TRANPFAR	*Bftp_PreFileHeader(char *FileName,word FileNameLen,dword FileSize);
extern	int				Bftp_PutBody(void);

#if	0
extern	byte			BftpFileName[BFTP_FILE_NAME_SIZE];
extern	word			BftpFileNameSize;
extern	dword			BftpFileSize;
extern	byte TRANPFAR	*pBftpFileBody;
extern	int				BftpStatus;
#endif

#endif
