


#ifndef	_SCEPDATA_H_
#define	_SCEPDATA_H_


#ifdef _DOS
#pragma	pack(1)
#endif


typedef	struct	t_S_CONTLR{
	byte	InfType;
	byte	Length;
	byte	Next[1];
} S_CONTLR;


#define	SIZE_S_CONTLR	4


typedef	struct	t_S_GENHDR {
	byte	SyncData;
	byte	MsgType;
	byte	InfType;
	byte	Length;
	byte	Length2[2];
} S_GENHDR;


#define	SIZE_S_GENHDRS	4
#define	SIZE_S_GENHDRL	6


typedef	struct	t_S_CONHDR {
	byte	SyncData;		
	byte	MsgType;		
	byte	InfType;		
	byte	MsgType2;		
	byte	InfType2;		
	byte	Length;			


	byte	InfVer;				
	byte	CFLG;				
	byte	SecAddr[8];			
	byte	PrmAddr[8];			
	byte	NegInf[1];
} S_CONHDR;


#define		SIZE_S_CONHDR_PRE	6
#define		SIZE_S_CONHDR		24


typedef	struct	t_S_CACK{
	byte	SyncData;		
	byte	MsgType;		
	byte	InfType;		
	byte	Length;			
	byte	InfVer;				
	byte	CFLG;				
	byte	PrmAddr[8];			
	byte	SecAddr[8];			
	byte	NegInf[1];
} S_CACK;


#define		SIZE_S_CACK		22


typedef	struct t_C_CNACK {
	byte	SyncData;		
	byte	MsgType;		
	byte	InfType;		
	byte	Length;			
	byte	InfVer;			
	byte	DFLG;			
	byte	Length3[2];		
} S_CNACK;


#define	SIZE_S_CNACK	8


typedef	struct t_CMDHDR	{
	byte	SyncData;			
	byte	MsgType;			
	byte	InfType;			
	byte	Length;				
	byte	Length2[2];			


	byte	InfVer;				
	byte	DFLG;				
	byte	Length3[2];			


	byte	SeqNo[4];			
	byte	RestNo[4];			
	byte	Next[1];
} S_CMDHDR;


#define	SIZE_S_CMDHDRS	10
#define	SIZE_S_CMDHDRL	18


typedef	struct	t_S_DISCHDR{
	byte	SyncData;			
	byte	MsgType;			
	byte	InfType;			
	byte	Length;				
	byte	ReasonCode[2];		
} S_DISCHDR;


#define	SIZE_S_DISCHDR	6


typedef	struct	t_COMMAND{
	byte	CmdHdr;					
	byte	PduType;				
	byte	Length[4];				
	byte	DstAddr[8];				
	byte	SrcAddr[8];				
	byte	DstPid[2];				
	byte	SrcPid[2];				
	byte	CmdId[2];				
	byte	Next[1];
} S_COMMAND;


#define	SIZE_S_COMMAND		28
#define	SIZE_S_COMMANDONLY	22


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