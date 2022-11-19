/*--------------------------------------------------------------------------*/
/*																			*/
/* 概要																		*/
/*		IrTran-P SCEPデータ構造体定義										*/
/*																			*/
/* 作成者																	*/
/*		kitazumi@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/

#ifndef	_SCEPDATA_H_
#define	_SCEPDATA_H_

#ifdef _DOS
#pragma	pack(1)
#endif

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		SCEP ConnPdu のトレーラ定義											*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	t_S_CONTLR{
	byte	InfType;
	byte	Length;
	byte	Next[1];
} S_CONTLR;

#define	SIZE_S_CONTLR	4

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		SCEP Pdu ジェネリックヘッダーの定義									*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	t_S_GENHDR {
	byte	SyncData;
	byte	MsgType;
	byte	InfType;
	byte	Length;
	byte	Length2[2];
} S_GENHDR;

#define	SIZE_S_GENHDRS	4
#define	SIZE_S_GENHDRL	6

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		SCEP ConnPdu のヘッダー定義											*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	t_S_CONHDR {
	byte	SyncData;		/*	1	1	*/
	byte	MsgType;		/*	1	2	*/
	byte	InfType;		/*	1	3	*/
	byte	MsgType2;		/*	1	4	*/
	byte	InfType2;		/*	1	5	*/
	byte	Length;			/*	1	6	*/

	byte	InfVer;			/*	1	7	*/	/*	1	1	*/
	byte	CFLG;			/*	1	8	*/	/*	1	2	*/
	byte	SecAddr[8];		/*	8	16	*/	/*	1	10	*/
	byte	PrmAddr[8];		/*	8	24	*/	/*	1	18	*/
	byte	NegInf[1];
} S_CONHDR;

#define		SIZE_S_CONHDR_PRE	6
#define		SIZE_S_CONHDR		24

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		SCEP CackPdu のヘッダー定義											*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	t_S_CACK{
	byte	SyncData;		/*	1	1	*/
	byte	MsgType;		/*	1	2	*/
	byte	InfType;		/*	1	3	*/
	byte	Length;			/*	1	4	*/
	byte	InfVer;			/*	1	5	*/	/*	1	1	*/
	byte	CFLG;			/*	1	6	*/	/*	1	2	*/
	byte	PrmAddr[8];		/*	1	14	*/	/*	1	10	*/
	byte	SecAddr[8];		/*	1	22	*/	/*	1	18	*/
	byte	NegInf[1];
} S_CACK;

#define		SIZE_S_CACK		22

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		SCEP CnackPdu のヘッダー定義										*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct t_C_CNACK {
	byte	SyncData;		/*	1	1	*/
	byte	MsgType;		/*	1	2	*/
	byte	InfType;		/*	1	3	*/
	byte	Length;			/*	1	4	*/
	byte	InfVer;			/*	1	5	*/
	byte	DFLG;			/*	1	6	*/
	byte	Length3[2];		/*	1	8	*/
} S_CNACK;

#define	SIZE_S_CNACK	8

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		SCEP CommandPdu のヘッダー定義										*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct t_CMDHDR	{
	byte	SyncData;			/* 1	1	*/
	byte	MsgType;			/* 1	2	*/
	byte	InfType;			/* 1	3	*/
	byte	Length;				/* 1	4	*/
	byte	Length2[2];			/* 2	6	*/

	byte	InfVer;				/* 1	7	*/
	byte	DFLG;				/* 1	8	*/
	byte	Length3[2];			/* 2	10	*/

	byte	SeqNo[4];			/* 4	14	*/
	byte	RestNo[4];			/* 4	18	*/
	byte	Next[1];
} S_CMDHDR;

#define	SIZE_S_CMDHDRS	10
#define	SIZE_S_CMDHDRL	18

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		SCEP DisconnectPdu のヘッダー定義									*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	t_S_DISCHDR{
	byte	SyncData;			/*	1	1	*/
	byte	MsgType;			/*	1	2	*/
	byte	InfType;			/*	1	3	*/
	byte	Length;				/*	1	4	*/
	byte	ReasonCode[2];		/*	1	6	*/
} S_DISCHDR;

#define	SIZE_S_DISCHDR	6

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		SCEP Command のヘッダー定義											*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	t_COMMAND{
	byte	CmdHdr;					/* 1	1	*/
	byte	PduType;				/* 1	2	*/
	byte	Length[4];				/* 4	6	*/
	byte	DstAddr[8];				/* 8	14	*/
	byte	SrcAddr[8];				/* 8	22	*/
	byte	DstPid[2];				/* 2	24	*/
	byte	SrcPid[2];				/* 2	26	*/
	byte	CmdId[2];				/* 2	28	*/
	byte	Next[1];
} S_COMMAND;

#define	SIZE_S_COMMAND		28
#define	SIZE_S_COMMANDONLY	22


/*

	Bftp Attribute Header
	
*/

/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		Bftp アトリビュートヘッダー定義										*/
/*																			*/
/* 2.概要																	*/
/*		無し																*/
/*																			*/
/* 3.機能説明																*/
/*		無し																*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	t_F_ATTHDR{
	byte	Name[4];
	byte	Length[4];
	byte	Type;
	byte	AFLG;
	byte	Value[1];
} F_ATTHDR;

#ifdef _DOS
#pragma	pack()
#endif

#endif
