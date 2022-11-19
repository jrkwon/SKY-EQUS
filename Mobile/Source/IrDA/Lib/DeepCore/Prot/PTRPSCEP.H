


#include	<ctype.h>
#include	<string.h>
#include	<stdio.h>
#include	<stdlib.h>


#ifdef	_DOS


#include	<irbios.h>
#define	cprintf printf


#else


#include	<irda.h>


#ifdef	USE_MONITOR
#include	"console.h"
#endif


#endif


#include	<ircomm.h>
#include	<irtranp.h>


#define	DUMP_SHORT


static	SCEPTBL	SCEP_Tbl;


static	int			Scep_Status;


static	byte		*Scep_Pdu;
static	dword		Scep_PduSize;
static	byte		*Scep_pPdu;


static	void		(*Scep_NextFunc)(byte c);
static	void		(*Scep_MoreFunc)(void);


static	dword		Scep_Dlen;


static	byte		Scep_ReqAddr[8];
static	byte		Scep_SelfAddr[8];


static	byte		*Scep_SelfNegInf;
static	size_t		Scep_SelfNegSize;


static	word		Scep_SelfPid;
static	size_t		Scep_SelfPduSize;
static	int			Scep_SelfPduSno;


static	word		Scep_SelfCmdId;


static	byte		Scep_YourAddr[8];
static	byte		Scep_YourNegInf[256];
static	size_t		Scep_YourNegSize;


static	word		Scep_YourPid;
static	size_t		Scep_YourPduSize;
static	size_t		Scep_YourDataSize;


static	word		Scep_YourCmdId;


static	int			Scep_Primary;


static	int			Scep_PduType;
static	dword		Scep_SeqNo;
static	dword		Scep_RestNo;


static	char		*Scep_Smsg;
static	char 		*Scep_Emsg;


int	SCEP_QueryPduBusy(void)
{
#ifdef	PDU_NOBUSY
	return	IrTranPR_ACCEPT;
#else
	return	PDU_QueryTxBusy((int)Scep_SelfPduSize);
#endif
}


static int Scep_SendPdu(size_t len,word status)
{
int		ret;
size_t	size;
byte	*dp;


#ifdef	SCEP_SENDMSG 
	cprintf("SendReq<%db>",len);
	SCEP_HexDump(Scep_Smsg,Scep_Pdu,len);
#endif


	while(PDU_QueryTxBusy(len)){
		if(!PDU_isConnected()){
			ret = IrTranPR_PDU_ERROR;
			goto err;
		}
	}
	ret	= PDU_SendTxData(Scep_Pdu,(int)len);
	if( ret != (int)len){
		ret = IrTranPR_PDU_ERROR;
		goto err;
	}
	Scep_Status = status;
	return	len;
err:
#ifdef	SCEP_SENDMSG
	cprintf("PDU<Send>:Err(%d)\n",ret);
#endif
	return	ret;
}


size_t	SCEP_GetDstDataSize(void)
{
	return	Scep_YourDataSize;
}


dword	SCEP_GetBig(byte *dp,int size)
{
dword	num,n;
int		len,data;
	num = 0;
	for(len = 0;len < size;len++){
		num *= 256;
		n	=  *dp++;
		num += n & 0xff;
	}
	return	num;
}


byte	*SCEP_PutBig(byte *dp,dword data,int size)
{
int		len;
	for(len = size - 1;len >= 0;len--){
		dp[len] = (byte)data;
		data >>= 8;
	}
	return	dp + size;
}


void	SCEP_DispAddr(void)
{
#ifdef	SCEP_DEBUG
 
	cprintf("YourAddr:");SCEP_HexPrint(Scep_YourAddr,8);
	cprintf("YourNegInf:%s",Scep_YourNegInf+1);
	cprintf("Your PduSize:%d\n",Scep_YourPduSize);


	cprintf("OurAddr :");SCEP_HexPrint(Scep_SelfAddr,8);
	cprintf("OurNegInf:%s",Scep_SelfNegInf);
	cprintf("OurPduSize[%d]:%d\n",Scep_SelfPduSno,Scep_SelfPduSize);


#endif
}


void	SCEP_HexPrint(byte *p,size_t len)
{
#ifdef	SCEP_DEBUG
byte	c;
	cprintf("[");
	while(len-- > 0){
		c = *p;
		cprintf("%02x.",c);
		p++;
	}
	cprintf("]");
#endif
}


void	SCEP_HexDump(char *msg,byte *p,dword len)
{
#ifdef	SCEP_DEBUG


byte	c;
	cprintf("%s:%dbytes\n[",msg,len);
#ifdef	DUMP_SHORT
	if(len > 16)
		len = 16;
#endif
	while(len-- > 0){
		c = *p;
		if((0x20 <= c) && (c <= 0x7e))
			cprintf("%02x(%c).",c,c);
		else
			cprintf("%02x.",c);
		p++;
	}
#ifdef	DUMP_SHORT
	cprintf(".....]\n");
#else
	cprintf("]\n");
#endif
#endif
}


static void	Scep_Error()
{
#ifdef	SCEP_DEBUG
dword	len;
	cprintf("#SCEP Error:<%s:%s>\n",Scep_Smsg,Scep_Emsg);
	len = Scep_pPdu - Scep_Pdu;
	SCEP_HexDump("ErrorPdu",Scep_Pdu,len);
#endif
}	


#define	SCEP_POLL


static	const struct t_NegSize	{
		byte 	frid[7];
		size_t	PduSize;
} NegSize[] = {


	{	"\021fr:1\r\n",	SCEP_PDU_SIZE1	},
	{	"\021fr:2\r\n",	SCEP_PDU_SIZE2	},
	{	"\021fr:3\r\n",	SCEP_PDU_SIZE3	},
	{	"\021fr:4\r\n",	SCEP_PDU_SIZE4	},
	{	"\0"		  ,	SCEP_PDU_SIZE1	}


};


static	void	Scep_Copy(byte c);
static	void	Scep_Idle(byte c);
static	void	Scep_Abort(void);
static	void	Scep_Wait(byte c);
static	void	Scep_MsgType(byte c);


int		SCEP_GetStatus(void)
{
	return	Scep_Status;
}


static size_t	Scep_GetPduSize(byte const *inf)
{
int	no;
	for(no = 0;NegSize[no].frid[0];no++){
		if(!memcmp(inf,NegSize[no].frid,sizeof(NegSize[0].frid)))
			break;
	}
	return	NegSize[no].PduSize;
}


static	void SCEP_Install(void)
{
	Scep_Dlen			= 0;
	Scep_NextFunc		= Scep_Wait;
	Scep_Status			= SCEP_ST_DISC;
	Scep_SelfPduSize	= SCEP_PDU_SIZE1;
	Scep_SelfPduSno		= 0;
}


static	int	SCEP_SetPduBuffer(byte *Buffer,int Size)
{
	if(Size < SCEP_PDU_SIZE1)
		return	IrTranPR_INVALID_PARAM;


	if(Size < SCEP_PDU_SIZE2){
		Scep_SelfPduSize	= SCEP_PDU_SIZE1;
		Scep_SelfPduSno		= 0;


	}else if(Size < SCEP_PDU_SIZE3){
		Scep_SelfPduSize	= SCEP_PDU_SIZE2;
		Scep_SelfPduSno		= 1;


	}else if(Size < SCEP_PDU_SIZE4){
		Scep_SelfPduSize	= SCEP_PDU_SIZE3;
		Scep_SelfPduSno		= 2;


	}else{
		Scep_SelfPduSize	= SCEP_PDU_SIZE4;
		Scep_SelfPduSno		= 3;
	}
	Scep_Pdu = Buffer;


}


static	void SCEP_SetNegInf(byte *MyAddr,byte *NegInf,word Size)
{
	memcpy(Scep_SelfAddr,MyAddr,sizeof(Scep_SelfAddr));
	Scep_SelfNegInf	= NegInf;
	Scep_SelfNegSize= Size;
}


static	void Scep_CBConInd(	byte CFLG,byte *ReqAddr,byte *YourAddr,
							byte *YourNegInf,word YourNegSize )
{
	if(SCEP_Tbl.CBConInd){
		(*SCEP_Tbl.CBConInd)(	CFLG,ReqAddr,YourAddr,
								YourNegInf,YourNegSize
		);
	}
}


static	void Scep_CBConCnf(	byte CFLG,byte *YourAddr,
							byte *YourNegInf,word YourNegSize )
{
	if(SCEP_Tbl.CBConCnf){
		(*SCEP_Tbl.CBConCnf)(	CFLG,YourAddr,
								YourNegInf,YourNegSize
		);
	}
}


static	void Scep_CBCmdIdInd(word CmdId )
{
	if(SCEP_Tbl.CBCmdIdInd){
		(*SCEP_Tbl.CBCmdIdInd)(	CmdId );
	}
}


static	void Scep_CBCmdHdrInd(	byte *DstAddr,byte *SrcAddr,
								word SelfPid,word YourPid,word CmdId,
								byte PduType )
{
	if(SCEP_Tbl.CBCmdHdrInd){
		(*SCEP_Tbl.CBCmdHdrInd)(DstAddr,SrcAddr,
								SelfPid,YourPid,CmdId,
								Scep_PduType );
	}
}


static	void Scep_CBCmdBodyInd(	byte DFLG,dword SeqNo,dword RestNo,
								byte *pData,word len )
{
int	dflg;
	dflg = DFLG;
	if(SCEP_Tbl.CBCmdCountInd){
		switch(dflg){
			case	SCEP_DFLG_SEG1:
			case	SCEP_DFLG_SEGI:
			case	SCEP_DFLG_SEGE:
				(*SCEP_Tbl.CBCmdCountInd)(DFLG,SeqNo,RestNo);
				break;
		}
	}
	if(SCEP_Tbl.CBCmdBodyInd){
		(*SCEP_Tbl.CBCmdBodyInd)(DFLG,SeqNo,RestNo,pData,len);
	}
}


void	Scep_CBStpInd(void)
{
	if(SCEP_Tbl.CBStpInd){
		(*SCEP_Tbl.CBStpInd)();
	}	
}


void	Scep_CBDiscInd(word Reason)
{
	if(SCEP_Tbl.CBDiscInd){
		(*SCEP_Tbl.CBDiscInd)(Reason);
	}


}


int	SCEP_ConnectReq(byte *SecAddr,byte CFLG)
{
size_t		len;
S_CONHDR	*pHdr;
S_CONTLR	*pTlr;
	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;


	pHdr	= (S_CONHDR*)Scep_Pdu;
	len		= sizeof(NegSize[0].frid);
	memcpy(pHdr->NegInf,NegSize[Scep_SelfPduSno].frid,(size_t)len);
	memcpy(pHdr->NegInf + len,Scep_SelfNegInf,Scep_SelfNegSize);
	len += Scep_SelfNegSize;


	
	pHdr->SyncData	= SCEP_SYNC_CHAR;
	pHdr->MsgType	= SCEP_MTYPE_CONREQ;
	pHdr->InfType	= SCEP_ITYPE_VERSION;
	pHdr->MsgType2	= 0x01;
	pHdr->InfType2	= SCEP_ITYPE_NEGINF;
	pHdr->Length	= (byte)(len + (SIZE_S_CONHDR - SIZE_S_CONHDR_PRE));
	pHdr->InfVer	= SCEP_INFVER;
	pHdr->CFLG		= CFLG;


	
	memcpy(pHdr->SecAddr,SecAddr,sizeof(pHdr->SecAddr));
	memcpy(pHdr->PrmAddr,Scep_SelfAddr,sizeof(pHdr->SecAddr));
	
	
	len	+= SIZE_S_CONHDR;


	pTlr	= (S_CONTLR*)(Scep_Pdu + len);


	
	pTlr->InfType	= SCEP_ITYPE_EXTEND;
	pTlr->Length	= 0x02;
	pTlr->Next[0]	= 0x00;
	pTlr->Next[1]	= 0x00;
	len += SIZE_S_CONTLR;


#ifdef	SCEP_SENDMSG
	Scep_Smsg = "ConReq";
#endif
	return	Scep_SendPdu(len ,SCEP_ST_CONREQ);
}


int	SCEP_ConnectRes(byte AckOrNack)
{
size_t	len;
S_CACK	*pAck;
S_CNACK	*pNack;
int		Stat;
	Stat = AckOrNack;
	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;
	switch(Stat){
		case	IrTranP_ACK:


				pAck	= (S_CACK*)Scep_Pdu;


				
				len		= sizeof(NegSize[0].frid);
				memcpy(pAck->NegInf,NegSize[Scep_SelfPduSno].frid,len);
				memcpy(pAck->NegInf + len,Scep_SelfNegInf,Scep_SelfNegSize);


				
				len		+= Scep_SelfNegSize;
				pAck->SyncData	= SCEP_SYNC_CHAR;
				pAck->MsgType	= SCEP_MTYPE_CONCNF;
				pAck->InfType	= SCEP_ITYPE_NEGINF;
				pAck->Length	= (byte)(len + (SIZE_S_CACK - SIZE_S_GENHDRS));
				pAck->InfVer	= SCEP_INFVER;
				pAck->CFLG		= SCEP_CFLG_EXECUTE;


				
				memcpy(pAck->PrmAddr,Scep_YourAddr,sizeof(pAck->PrmAddr));
				memcpy(pAck->SecAddr,Scep_SelfAddr,sizeof(pAck->SecAddr));


				len	+= SIZE_S_CACK;
#ifdef	SCEP_SENDMSG
	Scep_Smsg = "ConRes(ACK)";
#endif
				break;


		case	IrTranP_NACK:
				pNack	= (S_CNACK*)Scep_Pdu;


				
				pNack->SyncData	= SCEP_SYNC_CHAR;
				pNack->MsgType	= SCEP_MTYPE_COMMAND;
				pNack->InfType	= SCEP_ITYPE_USERDATA;
				pNack->Length	= SIZE_S_CNACK - SIZE_S_GENHDRS;
				pNack->InfVer	= SCEP_INFVER;
				pNack->DFLG		= SCEP_DFLG_NACK;
				SCEP_PutBig(pNack->Length3,0,sizeof(pNack->Length3));


				
				len	= SIZE_S_CNACK;
#ifdef	SCEP_SENDMSG
	Scep_Smsg = "ConRes(NACK)";
#endif
				break;


		default:
				return	IrTranPR_INVALID_PARAM;
	}


	return	Scep_SendPdu(len ,SCEP_ST_CONRES);
}


int	SCEP_DisconnectReq(word Reason)
{
int	len;
S_DISCHDR	*pDisc;
	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;
	pDisc	= (S_DISCHDR*)Scep_Pdu;


	
	pDisc->SyncData	= SCEP_SYNC_CHAR;
	pDisc->MsgType	= SCEP_MTYPE_DISCON;
	pDisc->InfType	= SCEP_ITYPE_REASON;
	pDisc->Length	= SIZE_S_DISCHDR - SIZE_S_GENHDRS;


	
	SCEP_PutBig(pDisc->ReasonCode,Reason,sizeof(pDisc->ReasonCode));
	len	= SIZE_S_DISCHDR;


#ifdef	SCEP_SENDMSG
	Scep_Smsg = "DiscReq";
#endif
	return	Scep_SendPdu(len ,SCEP_ST_DISC);
}


byte *SCEP_MakeSglCmdHdr(byte DFLG,dword DataLen)
{
S_CMDHDR	*pHdr;
	pHdr	= (S_CMDHDR*)	Scep_Pdu;
	
	pHdr->SyncData	= SCEP_SYNC_CHAR;
	pHdr->MsgType	= SCEP_MTYPE_COMMAND;
	pHdr->InfType	= SCEP_ITYPE_USERDATA;


	SCEP_PutBig(pHdr->Length3,DataLen,sizeof(pHdr->Length3));


	pHdr->Length	= 0xff;
	DataLen	+= SIZE_S_CMDHDRS - SIZE_S_GENHDRL;
	SCEP_PutBig(pHdr->Length2,DataLen,sizeof(pHdr->Length2));


	pHdr->InfVer	= SCEP_INFVER;
	pHdr->DFLG		= DFLG;
	return	pHdr->SeqNo;
}


byte *SCEP_MakeSegCmdHdr(byte DFLG,dword DataLen,dword SeqNo,dword RestNo)
{
S_CMDHDR	*pHdr;
	pHdr	= (S_CMDHDR*)	Scep_Pdu;
	
	pHdr->SyncData	= SCEP_SYNC_CHAR;
	pHdr->MsgType	= SCEP_MTYPE_COMMAND;
	pHdr->InfType	= SCEP_ITYPE_USERDATA;


	SCEP_PutBig(pHdr->Length3,DataLen,sizeof(pHdr->Length3));


	pHdr->Length	= 0xff;
	DataLen	+= SIZE_S_CMDHDRL - SIZE_S_GENHDRL;
	SCEP_PutBig(pHdr->Length2,DataLen,sizeof(pHdr->Length2));


	pHdr->InfVer	= SCEP_INFVER;
	pHdr->DFLG		= DFLG;


	SCEP_PutBig(pHdr->SeqNo,SeqNo,		sizeof(pHdr->SeqNo));
	SCEP_PutBig(pHdr->RestNo,RestNo,	sizeof(pHdr->RestNo));
	return	pHdr->Next;
}


byte *SCEP_MakeCmdHdr(void *pData,
			word DstPid,word SrcPid,
			byte PduType,dword DataLen,word CmdId)
{
S_COMMAND *pHdr;


	
	DataLen += SIZE_S_COMMANDONLY;


	pHdr = (S_COMMAND*)pData; 


	pHdr->CmdHdr 	= SCEP_CMDHDR;
	pHdr->PduType	= PduType;


	SCEP_PutBig(pHdr->Length,DataLen,	sizeof(pHdr->Length));
	memcpy(pHdr->DstAddr,Scep_YourAddr,	sizeof(pHdr->DstAddr));
	memcpy(pHdr->SrcAddr,Scep_SelfAddr,	sizeof(pHdr->SrcAddr));
	SCEP_PutBig(pHdr->DstPid, DstPid,	sizeof(pHdr->DstPid));
	SCEP_PutBig(pHdr->SrcPid, SrcPid,	sizeof(pHdr->SrcPid));
	SCEP_PutBig(pHdr->CmdId,  CmdId,	sizeof(pHdr->CmdId));
	return pHdr->Next;	
}


int	SCEP_CommandSglPdu(
			word DstPid,word SrcPid,
			byte PduType,byte *Data,size_t DataLen
	)
{
size_t	len;
byte	*pData;
word	CmdId;
	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;


	pData	= SCEP_MakeSglCmdHdr(SCEP_DFLG_SINGL,DataLen + 28);
	
	if((PduType & SCEP_PTYPE_MASK) == SCEP_PTYPE_REQ){
		CmdId	= Scep_SelfCmdId ++;
		Scep_CBCmdIdInd(CmdId);
	}else{
		CmdId	= Scep_YourCmdId;
	}
	pData = SCEP_MakeCmdHdr(pData,DstPid,SrcPid,PduType,DataLen,CmdId);
	
	memcpy(pData,Data,DataLen);
	pData += DataLen;
	len	= (size_t)(pData - Scep_Pdu);
#ifdef	SCEP_SENDMSG
	Scep_Smsg = "CmdSglPdu";
#endif
	return	Scep_SendPdu(len ,SCEP_ST_CMDREQ);
}


#ifndef	IrTRANPNOOLDAPI


int	SCEP_CommandHdrPdu(word DstPid,word SrcPid,byte PduType,dword DataLen)
{
size_t	len;
byte	*pData;
word	CmdId;
	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;


	Scep_SeqNo	= 0;
	
	Scep_RestNo	= (DataLen / Scep_YourDataSize) + 1;
	if(DataLen % Scep_YourDataSize){
		Scep_RestNo ++;
	}


#ifdef	SCEP_SENDMSG
	cprintf("<%ld/%ld>",Scep_SeqNo,Scep_RestNo);
	Scep_Smsg = "Cmd1StPdu";
#endif


	
	pData	= SCEP_MakeSegCmdHdr(SCEP_DFLG_SEG1,SIZE_S_COMMAND,Scep_SeqNo,Scep_RestNo);


	
	if((PduType & SCEP_PTYPE_MASK) == SCEP_PTYPE_REQ){
		CmdId	= Scep_SelfCmdId ++;
		Scep_CBCmdIdInd(CmdId);
	}else{
		CmdId	= Scep_YourCmdId;
	}


	pData = SCEP_MakeCmdHdr(pData,DstPid,SrcPid,PduType,DataLen,CmdId);
	len	= (size_t)(pData - Scep_Pdu);
	if(SCEP_Tbl.CBCmdCountInd){
		(*SCEP_Tbl.CBCmdCountInd)(SCEP_DFLG_SEG1,Scep_SeqNo,Scep_RestNo);
	}


	Scep_SeqNo ++;
	Scep_RestNo--;
	return	Scep_SendPdu(len ,SCEP_ST_CMDREQ);
}


int	SCEP_CommandNextPdu(byte DFLG,byte *UserData,dword DataLen)
{
size_t	len;
byte	*pData;
int		dflg;
	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;
#ifdef	SCEP_SENDMSG
	cprintf("<%ld/%ld>",Scep_SeqNo,Scep_RestNo);
	Scep_Smsg = "CmdNextPdu";
#endif
	
	dflg	= DFLG;
	pData	= SCEP_MakeSegCmdHdr(DFLG,DataLen,Scep_SeqNo,Scep_RestNo);
	if(SCEP_Tbl.CBCmdCountInd){
		switch(dflg){
			case	SCEP_DFLG_SEG1:
			case	SCEP_DFLG_SEGI:
			case	SCEP_DFLG_SEGE:
				(*SCEP_Tbl.CBCmdCountInd)(DFLG,Scep_SeqNo,Scep_RestNo);
		}
	}
	Scep_SeqNo++;
	Scep_RestNo--;
	memcpy(pData,UserData,(word)DataLen);
	pData += DataLen;


	len	= (size_t)(pData - Scep_Pdu);
	return	Scep_SendPdu(len ,SCEP_ST_CMDREQ);
}


#endif


static	dword	CurrentDataLen;
static	byte	*pCurrentData;


word 	SCEP_PreCommandSegPdu(void *pData,	word DstPid,word SrcPid,byte PduType,dword DataLen)
{
word	CmdId;
	
	if((PduType & SCEP_PTYPE_MASK) == SCEP_PTYPE_REQ){
		CmdId	= Scep_SelfCmdId ++;
		Scep_CBCmdIdInd(CmdId);
	}else{
		CmdId	= Scep_YourCmdId;
	}
	SCEP_MakeCmdHdr(pData,DstPid,SrcPid,PduType,DataLen,CmdId);
	
	CurrentDataLen	= DataLen + SIZE_S_COMMAND;
	pCurrentData	= pData;
	Scep_SeqNo		= 0;
	Scep_RestNo		= (CurrentDataLen / Scep_YourDataSize);
	if(CurrentDataLen % Scep_YourDataSize){
		Scep_RestNo ++;
	}
	return	CmdId;
}


int	SCEP_SendCommandSegPdu(void)
{
size_t	csize;
byte	*pData;
int		DFLG;


	if(CurrentDataLen == 0)
		return	IrTranPR_ACCEPT;


	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;


#ifdef	SCEP_SENDMSG
	cprintf("<%ld/%ld>",Scep_SeqNo,Scep_RestNo);
	Scep_Smsg = "SegCmdPdu";
#endif


	
	if(Scep_SeqNo == 0){
		DFLG	= SCEP_DFLG_SEG1;
		csize	= Scep_YourDataSize;
	}else{
		if(CurrentDataLen	<= Scep_YourDataSize){
			csize	= CurrentDataLen;
			DFLG	= SCEP_DFLG_SEGE;
		}else{
			csize	= Scep_YourDataSize;
			DFLG	= SCEP_DFLG_SEGI;
		}
	}
	if(SCEP_Tbl.CBCmdCountInd){
		(*SCEP_Tbl.CBCmdCountInd)(DFLG,Scep_SeqNo,Scep_RestNo);
	}
	pData	= SCEP_MakeSegCmdHdr(DFLG,csize,Scep_SeqNo,Scep_RestNo);
	memcpy(pData,pCurrentData,csize);
	pData += csize;


	Scep_SeqNo++;	Scep_RestNo--;
	pCurrentData	+= csize;
	CurrentDataLen	-= csize;


	csize	= (size_t)(pData - Scep_Pdu);
	return	Scep_SendPdu(csize ,SCEP_ST_CMDREQ);
}


static	void Scep_Reset()
{
	Scep_NextFunc	= Scep_Wait;
}


static	void Scep_Copy(byte c)
{
	*Scep_pPdu++	=	c;
	Scep_Dlen--;
	if(Scep_Dlen > 0)
		return;
	(*Scep_MoreFunc)();
}


static	void Scep_Idle(byte c)
{
#ifdef	SCEP_RECVMSG
	cprintf("Idle:");
#endif
}


static	void Scep_Abort(void)
{
#ifdef	SCEP_RECVMSG
	cprintf("Abort:");
#endif
	Scep_NextFunc = Scep_Wait;
}


static	void Scep_Wait(byte c)
{
#ifdef	SCEP_RECVMSG
	cprintf("W:");
#endif
	if(c == SCEP_SYNC_CHAR){
		Scep_pPdu		= Scep_Pdu;
		*Scep_pPdu++	= c;
		Scep_NextFunc	= Scep_MsgType;
	}else{
		Scep_NextFunc	= Scep_Wait;
	}
}


static	void Scep_MakeYourPduSize(byte *NegInf)
{
	Scep_YourPduSize	= Scep_GetPduSize(NegInf);
	if(Scep_YourPduSize > Scep_SelfPduSize)
		Scep_YourPduSize	= Scep_SelfPduSize;


	Scep_YourDataSize	= Scep_YourPduSize - SIZE_S_CMDHDRL;


#ifdef	SCEP_DEBUG
	cprintf("NegInf[%-4.4s],size=%d\n",NegInf+1,Scep_YourPduSize);
#endif
}


static	void Scep_ConReqInd(void)
{
S_CONHDR	*pPdu;
S_CONTLR	*pTlr;
int			len;


#ifdef	SCEP_DEBUG
	Scep_Smsg = "ConInd";
#endif


	pPdu = (S_CONHDR*)Scep_Pdu;
	if(pPdu->InfVer != SCEP_INFVER){
#ifdef	SCEP_DEBUG
	Scep_Emsg = "InfVer";
#endif
		goto err;
	}


	len	= pPdu->Length;


	
	pTlr = (S_CONTLR*)(&pPdu->InfVer + len);


	if(pTlr->InfType != SCEP_INFVER){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "InfVer2";
#endif
		goto err;
	}


	if(pTlr->Length != 0x02){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "Length != 2";
#endif
		goto err;
	}


	if(pTlr->Next[0] != 0x00){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "Data[0] != 0";
#endif
		goto err;
	}


	if(pTlr->Next[1] != 0x00){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "Data[1] != 0";
#endif
		goto err;
	}


	
	len	-= SIZE_S_CONHDR - SIZE_S_GENHDRL;
	memcpy(Scep_ReqAddr ,pPdu->SecAddr,sizeof(Scep_ReqAddr));
	memcpy(Scep_YourAddr,pPdu->PrmAddr,sizeof(Scep_YourAddr));
	memcpy(Scep_YourNegInf,pPdu->NegInf,len);
	Scep_YourNegSize	= len;


	
	Scep_MakeYourPduSize(pPdu->NegInf);


	Scep_PduSize		= Scep_pPdu - Scep_Pdu;


#ifdef	SCEP_RECVMSG
	SCEP_HexDump("ConInd",Scep_Pdu,Scep_PduSize);
#endif


	Scep_CBConInd(	pPdu->CFLG,
					Scep_ReqAddr,Scep_YourAddr,
					Scep_YourNegInf,Scep_YourNegSize
	);


	Scep_Status 		= SCEP_ST_CONIND;
	Scep_NextFunc		= Scep_Wait;
	


	return;
err:


#ifdef	SCEP_DEBUG
		Scep_Error();
#endif


	Scep_NextFunc	= Scep_Wait;
}


static	void Scep_ConReqCheck(void)
{
S_CONHDR	*pPdu;
int			len;
	pPdu = (S_CONHDR*)Scep_Pdu;
	if(pPdu->MsgType2 != 0x01){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "MsgType2 != 1";
#endif
		goto err;
	}


	if(pPdu->InfType2 != SCEP_ITYPE_NEGINF){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "InfType2";
#endif
		goto err;
	}
	len	= pPdu->Length;
	if(len > 228){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "Length > 228";
#endif
		goto err;
	}
	Scep_Dlen		= len + 4;
	Scep_MoreFunc	= Scep_ConReqInd;
	Scep_NextFunc	= Scep_Copy;
	return;
err:
#ifdef	SCEP_DEBUG
		Scep_Error();
#endif
	Scep_Abort();
}


static	void Scep_ConReqInfType(byte c)
{
	if(c == SCEP_ITYPE_VERSION){
		Scep_Dlen		= 3;
		Scep_MoreFunc	= Scep_ConReqCheck;
		Scep_NextFunc	= Scep_Copy;
		*Scep_pPdu++	= c;
	}else{
#ifdef	SCEP_DEBUG
		Scep_Emsg = "InfType";
		Scep_Error();
#endif
		Scep_NextFunc = Scep_Wait;
	}
}


static	void Scep_ConCnfInd(void)
{
S_CACK	*pPdu;
int			len;


	pPdu = (S_CACK*)Scep_Pdu;
	if(pPdu->InfVer != SCEP_INFVER){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "InfVer";
#endif
		goto err;
	}
	len	= pPdu->Length - (SIZE_S_CONHDR-SIZE_S_CONHDR_PRE);
	memcpy(Scep_YourAddr ,pPdu->SecAddr,sizeof(Scep_YourAddr));
	memcpy(Scep_YourNegInf,pPdu->NegInf,len);
	Scep_YourNegSize	= len;


	Scep_MakeYourPduSize(pPdu->NegInf);


	Scep_PduSize		= Scep_pPdu - Scep_Pdu;
#ifdef	SCEP_RECVMSG
	SCEP_HexDump("ConCnf",Scep_Pdu,Scep_PduSize);
#endif


	Scep_CBConCnf(	
					pPdu->CFLG,
					Scep_YourAddr,
					Scep_YourNegInf,
					Scep_YourNegSize
	);


	Scep_Status 		= SCEP_ST_CONCNF;
	Scep_NextFunc		= Scep_Wait;
	return;
err:
#ifdef	SCEP_DEBUG
		Scep_Error();
#endif
	Scep_NextFunc	= Scep_Wait;
}


static	void Scep_ConCnfInfCheck(byte c)
{
	Scep_Dlen	= c;
	if(c > 228){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "Length > 288";
#endif
		goto err;
	}
	*Scep_pPdu++	= c;
	Scep_Dlen		= c;
	Scep_MoreFunc	= Scep_ConCnfInd;
	Scep_NextFunc	= Scep_Copy;
	return;
err:
#ifdef	SCEP_DEBUG
		Scep_Error();
#endif
	Scep_Abort();
}


static	void Scep_ConCnfInfType(byte c)
{
	if(c == SCEP_ITYPE_NEGINF){
		Scep_NextFunc	= Scep_ConCnfInfCheck;
		*Scep_pPdu++	= c;
	}else{
#ifdef	SCEP_DEBUG
		Scep_Emsg = "InfType";
		Scep_Error();
#endif
		Scep_NextFunc = Scep_Wait;
	}
}


static void Scep_CmdIndCnf(void)
{
S_CMDHDR	*pHdr;
S_COMMAND	*pCmd;
byte		*pData;
int			DFLG;
dword		SeqNo,RestNo;
word		CmdId;
int			len,stat;


	Scep_PduSize	= Scep_pPdu - Scep_Pdu;
	pHdr	= (S_CMDHDR*)Scep_Pdu;
	if(pHdr->InfVer != SCEP_INFVER){
#ifdef	SCEP_DEBUG
		Scep_Emsg = "InfVer";
#endif
		goto err;
	}
	
	len		= (word)SCEP_GetBig(pHdr->Length3,sizeof(pHdr->Length3));
	DFLG	= pHdr->DFLG;
	if((DFLG & SCEP_DFLG_NOSEG) == SCEP_DFLG_NOSEG){
		
		switch(DFLG){
			case	SCEP_DFLG_SINGL:
				SeqNo	= RestNo = 0;
				pData	= pHdr->SeqNo;
				break;


			case	SCEP_DFLG_STOP:
				Scep_CBStpInd();
				Scep_Status	= SCEP_ST_STOP;
				Scep_NextFunc	= Scep_Wait;
				return;
		}
		SeqNo	= 0;
		RestNo	= 0;
		pData	= Scep_Pdu + SIZE_S_CMDHDRS;
	}else{
		
		SeqNo	= (word)SCEP_GetBig(pHdr->SeqNo ,sizeof(pHdr->SeqNo));
		RestNo	= (word)SCEP_GetBig(pHdr->RestNo,sizeof(pHdr->RestNo));
		pData	= Scep_Pdu + SIZE_S_CMDHDRL;
	}
	if(SeqNo == 0){
		
		pCmd	= (S_COMMAND*)pData;
		if(pCmd->CmdHdr != SCEP_CMDHDR)
			goto err;


		Scep_PduType	= pCmd->PduType;
		Scep_SelfPid	= (word)SCEP_GetBig(pCmd->DstPid,sizeof(pCmd->DstPid));
		Scep_YourPid	= (word)SCEP_GetBig(pCmd->SrcPid,sizeof(pCmd->SrcPid));
		CmdId			= (word)SCEP_GetBig(pCmd->CmdId,sizeof(pCmd->CmdId));


		if((Scep_PduType & SCEP_PTYPE_MASK) == SCEP_PTYPE_REQ)
			Scep_YourCmdId = CmdId;


		Scep_CBCmdHdrInd(
			pCmd->DstAddr,pCmd->SrcAddr,
			Scep_SelfPid,Scep_YourPid,CmdId,
			Scep_PduType
		);
		
		len				-= SIZE_S_COMMAND;
		pData			+= SIZE_S_COMMAND;


	}


	switch(Scep_PduType & SCEP_PTYPE_MASK){
		case	SCEP_PTYPE_REQ:
				stat = SCEP_ST_CMDIND;
				break;
		case	SCEP_PTYPE_RPL_ACK:
				stat = SCEP_ST_CMDCNF;
				break;
		case	SCEP_PTYPE_RPL_NACK:
				stat = SCEP_ST_CMDCNF;
				break;
		case	SCEP_PTYPE_ABORT:
				stat = SCEP_ST_CMDABT;
				break;
		default	:
				stat = SCEP_ST_CMDPRC;
	}


	switch(DFLG){
		case	SCEP_DFLG_SEG1:	
		case	SCEP_DFLG_SEGI:	
				stat	= SCEP_ST_CMDPRC;
				break;
	}


#ifdef	SCEP_DEBUG
	cprintf("SCEP:BodyInd DFLG(%02x) S(%04x)/R(%04x)\n",DFLG,SeqNo,RestNo);
	SCEP_HexDump("Data",pData,len);
#endif


	Scep_CBCmdBodyInd( DFLG,SeqNo,RestNo,pData,len);
	Scep_NextFunc	= Scep_Wait;
	Scep_Status     = stat;
	return;
err:
#ifdef	SCEP_DEBUG
		Scep_Error();
#endif
	Scep_NextFunc	= Scep_Wait;
}


static	void Scep_CmdBody(void)
{
S_CMDHDR	*pHdr;
int			len;
	pHdr = (S_CMDHDR*)Scep_Pdu;
	len	 = (word)SCEP_GetBig(pHdr->Length2,sizeof(pHdr->Length2));
#ifdef	SCEP_DEBUG
	cprintf("[%d] bytes",len);
#endif
	Scep_Dlen		= len;
	Scep_MoreFunc	= Scep_CmdIndCnf;
	Scep_NextFunc	= Scep_Copy;
}


static	void Scep_CmdLen1(byte c)
{
	if(c == SCEP_USELEN2){
		Scep_Dlen		= 2;
		Scep_MoreFunc	= Scep_CmdBody;
		Scep_NextFunc	= Scep_Copy;
		*Scep_pPdu++	= c;
	}else{
		*Scep_pPdu++	= SCEP_USELEN2;
		*Scep_pPdu++	= 0x00;
		*Scep_pPdu++	= c;
		Scep_CmdBody();
	}
}


static	void Scep_CmdInfType(byte c)
{
	if(c == SCEP_ITYPE_USERDATA){
		Scep_NextFunc	= Scep_CmdLen1;
		*Scep_pPdu++	= c;
	}else{
#ifdef	SCEP_DEBUG
		Scep_Emsg = "InfType";
		Scep_Error();
#endif


		Scep_NextFunc = Scep_Wait;
	}
}


static	void Scep_DiscInd(void)
{
S_DISCHDR	*pHdr;
word		Reason;
	pHdr 	= (S_DISCHDR*)Scep_Pdu;
	Reason	= (word)SCEP_GetBig(pHdr->ReasonCode,2);
	Scep_PduSize	= Scep_pPdu - Scep_Pdu;
#ifdef	SCEP_RECVMSG
	cprintf("<%d>",Reason);
	SCEP_HexDump("",Scep_Pdu,Scep_PduSize);
#endif


	Scep_CBDiscInd(Reason);


	Scep_Status 	= SCEP_ST_DISC;
	Scep_NextFunc	= Scep_Wait;
}


static	void Scep_DiscLen(byte c)
{
	*Scep_pPdu++	= c;
	Scep_Dlen		= c;
	Scep_MoreFunc	= Scep_DiscInd;
	Scep_NextFunc	= Scep_Copy;
}


static	void Scep_DiscInfType(byte c)
{
	if(c == SCEP_ITYPE_REASON){
		Scep_NextFunc	= Scep_DiscLen;
		*Scep_pPdu++	= c;
	}else{
#ifdef	SCEP_DEBUG
		Scep_Emsg = "InfType";
		Scep_Error();
#endif
		Scep_NextFunc = Scep_Wait;
	}
}


static	void Scep_MsgType(byte c)
{
int	type;
	type = c;
	switch(type){
		case	SCEP_SYNC_CHAR:
				Scep_Wait(c);
				return;
		case	SCEP_MTYPE_CONREQ:	
#ifdef	SCEP_DEBUG
				Scep_Smsg = "ConReq";
#endif
				Scep_NextFunc	= Scep_ConReqInfType;
				break;


		case	SCEP_MTYPE_CONCNF:	
#ifdef	SCEP_DEBUG
				Scep_Smsg = "ConCnf";
#endif
				Scep_NextFunc	= Scep_ConCnfInfType;
				break;


		case	SCEP_MTYPE_COMMAND:	
#ifdef	SCEP_DEBUG
				Scep_Smsg = "CmdInd";
#endif
				Scep_NextFunc	= Scep_CmdInfType;
				break;


		case	SCEP_MTYPE_DISCON:	
#ifdef	SCEP_DEBUG
				Scep_Smsg = "DiscReq";
#endif
				Scep_NextFunc	= Scep_DiscInfType;
				break;


		default:
#ifdef	SCEP_DEBUG
				Scep_Smsg = "Undefined";
#endif
				Scep_NextFunc	= Scep_Wait;
	}
#ifdef	SCEP_DEBUG
	cprintf("%s:",Scep_Smsg);
#endif
	*Scep_pPdu++	= c;
}


void	SCEP_Sequence(byte c)
{
	(*Scep_NextFunc)(c);
}


void	SCEP_BindPdu(int PduType,SCEPTBL *Stbl)
{
	SCEP_Tbl = *Stbl;
	SCEP_Tbl.Use = TRUE;
	SCEP_Install();
	SCEP_SetNegInf(SCEP_Tbl.Address,SCEP_Tbl.NegInf,SCEP_Tbl.NegSize);
	SCEP_SetPduBuffer(SCEP_Tbl.PduBuffer,SCEP_Tbl.PduSize);
	if(SCEP_Tbl.CBPduEnter)
		(*SCEP_Tbl.CBPduEnter)();
}


void	SCEP_UnbindPdu(void)
{
	if(SCEP_Tbl.CBPduExit)
		(*SCEP_Tbl.CBPduExit)();
	SCEP_Install();
	SCEP_Tbl.Use = FALSE;
}