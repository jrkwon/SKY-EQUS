


#if !defined(__ptrpbftp_h__)
#define	__ptrpbftp_h__


#include	<ctype.h>
#include	<string.h>
#include	<stdio.h>
#include	<stdlib.h>


#ifdef	_DOS


#include	<irbios.h>
#define	cprintf	printf


#else


#include	<irda.h>


#ifdef	USE_MONITOR
#include	"console.h"
#endif


#endif


#include	<ircomm.h>
#include	<irtranp.h>


static	BFTPTBL	Bftp_Tbl;


static	int		BftpStatus;


static	byte TRANPFAR	*BftpUserData;
static	dword			BftpUserDataSize;


static	byte TRANPFAR	*pBftpData;
static	byte TRANPFAR	*pBftpBody;


static	word			BftpAttNum;
static	dword			BftpDataSize;


static	word			BftpDstPid;
static	word			BftpSrcPid;
static	word			BftpCmdId;
static	size_t			BftpDstDataSize;
static	int				BftpPduType;


static	byte			BftpDstAddr[8];
static	byte			BftpSrcAddr[8];


static	byte			BftpFileName[256];


static	byte TRANPFAR	*pBftpFileBody;
static	dword			BftpFileSize;
static	dword			BftpPutCurSize;
static	byte			*BftpPutCurPos;


void Bftp_SetUserBuffer(byte TRANPFAR *Buffer,dword Size)
{
	BftpUserData		= Buffer;
	BftpUserDataSize	= Size;
}


static byte TRANPFAR *Bftp_SetAttr(
	byte TRANPFAR *pData,
	byte *Name,
	byte Type,byte AFLG,
	dword Size
	)
{
F_ATTHDR TRANPFAR *pAttr;


	pAttr = (F_ATTHDR TRANPFAR *)pData;
	memcpy(pAttr->Name,Name,sizeof(pAttr->Name));
	SCEP_PutBig(pAttr->Length,Size + 2,sizeof(pAttr->Length));
	pAttr->Type	= Type;
	pAttr->AFLG	= AFLG;
	return	pAttr->Value;
}


int	Bftp_RplSglCnack(word ErrCode)
{
byte TRANPFAR *pData;
dword	size;
	pData =	SCEP_PutBig(BftpUserData,1,sizeof(word));
	pData =	Bftp_SetAttr(
				pData,
				(byte*)BFTP_ANAME_ERROR,BFTP_ATYPE_BINARY,BFTP_AFLG_NOSUP,
				(dword)sizeof(word)
			);
	pData	= SCEP_PutBig(pData,ErrCode,sizeof(word));
	size =	pData - BftpUserData; 
	return SCEP_CommandSglPdu(
		BftpSrcPid,BFTP_PID,
		SCEP_PTYPE_RPL_NACK,
		BftpUserData,(word)size
	);
}


int Bftp_RplSglCack(byte *Value,dword Size)
{
byte TRANPFAR *pData;
dword	size;
	pData =	SCEP_PutBig(BftpUserData,1,sizeof(word));
	pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_REPLY,	BFTP_ATYPE_TEXT,	BFTP_AFLG_NOSUP,
				Size
			);
	memcpy(pData,Value,(word)Size);
	size =	pData - BftpUserData;
	size += Size; 
	return SCEP_CommandSglPdu(
		BftpSrcPid,BFTP_PID,
		SCEP_PTYPE_RPL_ACK,
		BftpUserData,(word)size
	);
}


int Bftp_RplQueryCack(byte *Value,dword Size)
{
byte TRANPFAR *pData;
dword	size;
	pData =	SCEP_PutBig(BftpUserData,1,sizeof(word));
	pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_BODY,	BFTP_ATYPE_BINARY,	BFTP_AFLG_NOSUP,
				Size
			);


	memcpy(pData,Value,(word)Size);
	size =	pData - BftpUserData;
	size += Size; 


	return SCEP_CommandSglPdu(
		BftpSrcPid,BFTP_PID,
		SCEP_PTYPE_RPL_ACK,
		BftpUserData,(word)size
	);
}


int Bftp_QueryReq(byte *Value,dword Size)
{
byte TRANPFAR *pData;
dword	size;
	pData =	SCEP_PutBig(BftpUserData,2,sizeof(word));
	pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_COMMAND,	BFTP_ATYPE_BINARY,	BFTP_AFLG_NOSUP,
				sizeof(dword)
			);
	pData = SCEP_PutBig(pData,BFTP_CTYPE_QUERY,sizeof(dword));
	pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_QUERY,	BFTP_ATYPE_TEXT,	BFTP_AFLG_NOSUP,
				Size
			);
	memcpy(pData,Value,(word)Size);
	size =	pData - BftpUserData;
	size += Size; 
	return SCEP_CommandSglPdu(
		BFTP_PID,BFTP_PID,
		SCEP_PTYPE_REQ,BftpUserData,(word)size
	);
}


byte TRANPFAR *Bftp_SetFileHeader(char *FileName,word FileNameLen,dword FileSize)
{
byte TRANPFAR *pData;


	pData =	SCEP_PutBig(BftpUserData,3,2);
	
	
	pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_COMMAND,BFTP_ATYPE_BINARY,BFTP_AFLG_NOSUP,
				sizeof(dword)
			);
	pData	= SCEP_PutBig(pData,BFTP_CTYPE_PUT,sizeof(dword));
	
    
	if(FileNameLen == 12){
		pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_FILE,BFTP_ATYPE_TEXT,BFTP_AFLG_NOSUP,
				FileNameLen
			);
	}else{
		pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_LFILE,BFTP_ATYPE_TEXT,BFTP_AFLG_NOSUP,
				FileNameLen
			);
	}	
	memcpy(pData,FileName,(word)FileNameLen);
	pData += FileNameLen;


	
	return	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_BODY,BFTP_ATYPE_BINARY,BFTP_AFLG_NOSUP,
				FileSize
			);
}


static	void BftpDispAttr(F_ATTHDR TRANPFAR *pAttr)
{
#ifdef	SCEP_DEBUG
dword	AttLen;
		AttLen	= SCEP_GetBig(pAttr->Length,sizeof(pAttr->Length));
		cprintf("[%-4.4s]Len(%ld)Type(%02x)Value\n",
			pAttr->Name,AttLen,pAttr->Type
		);
		AttLen -= 2;
		if(AttLen > 16)
			AttLen = 16;
		SCEP_HexPrint(pAttr->Value,(int)AttLen);
		cprintf("\n");
#endif
}


void Bftp_DispAttribute(F_ATTHDR TRANPFAR *pAttr,word AttNum)
{
byte 	 TRANPFAR 	*pData;
dword				AttLen;
word				AttNo;


	pData 		= (byte TRANPFAR *)pAttr;
	for(AttNo = 0;AttNo < AttNum;AttNo++){
		pAttr	= (F_ATTHDR TRANPFAR *)pData;
		AttLen	= SCEP_GetBig(pAttr->Length,sizeof(pAttr->Length));
		BftpDispAttr(pAttr);
		pData += 8 + AttLen;
	}
}


static void BftpPutInd(void)
{
byte 	 TRANPFAR 	*pData;
F_ATTHDR TRANPFAR	*pAttr
;
dword	AttLen;
word	AttNo;
int		Name,Body;
	if(Bftp_Tbl.CBPutInd == 0){
		Bftp_RplSglCnack(BFTP_ERROR_COMMAND);
		return;
	}
	Name = FALSE;
	Body = FALSE;


	pData 		= pBftpBody;
	for(AttNo = 0;AttNo < BftpAttNum;AttNo++){
		pAttr	= (F_ATTHDR TRANPFAR *)pData;
		BftpDispAttr(pAttr);
		AttLen	= SCEP_GetBig(pAttr->Length,sizeof(pAttr->Length));
		if(!memcmp(pAttr->Name,"FIL0",4)){
			memcpy(BftpFileName,pAttr->Value,(word)AttLen - 2);
			BftpFileName[AttLen - 2] = '\0';
			Name = TRUE;	
		}else
		if(!memcmp(pAttr->Name,"LFL0",4)){
			memcpy(BftpFileName,pAttr->Value,(word)AttLen - 2);
			BftpFileName[AttLen - 2] = '\0';
			Name = TRUE;	
		}else
		if(!memcmp(pAttr->Name,"BDY0",4)){
			BftpFileSize	= AttLen - 2;
			pBftpFileBody	= pAttr->Value;
			Body = TRUE;
		}
		pData += 8 + AttLen;
	}
	if(Name && Body){
		(*Bftp_Tbl.CBPutInd)( BftpFileName,pBftpFileBody,BftpFileSize);
	}else{
		Bftp_RplSglCnack(BFTP_ERROR_DATA);
	}
}


static void BftpQueryInd(void)
{
byte TRANPFAR 	*pData;
word AttNum;	
	if(Bftp_Tbl.CBQueryInd){
		pData		= BftpUserData;
		AttNum		= (word)SCEP_GetBig(pData,sizeof(word));
		pData 		+= sizeof(word);
		(*Bftp_Tbl.CBQueryInd)((F_ATTHDR TRANPFAR *)pData,AttNum);
	}else{
		Bftp_RplSglCnack(BFTP_ERROR_COMMAND);
	}
}


static void BftpReqInd(void)
{
byte 		TRANPFAR *pData;
F_ATTHDR	TRANPFAR *pAttr;
word 		AttNum;
dword		AttLen,CmdType;


	pData		= BftpUserData;
	AttNum		= (word)SCEP_GetBig(pData,sizeof(word));
	pData 		+= 2;


	pAttr		= (F_ATTHDR TRANPFAR *)pData;
	AttLen		= SCEP_GetBig(pAttr->Length,sizeof(pAttr->Length));
	pData 		+= 8 + AttLen;
	
	if(memcmp(pAttr->Name,"CMD0",sizeof(pAttr->Name))){
#ifdef	SCEP_DEBUG
		cprintf("<Bftp:Invalid Command\n");
#endif
		Bftp_RplSglCnack(BFTP_ERROR_DATA);
		BftpStatus = BFTP_ST_INVALID;
		return;
	}
	pBftpBody	= pData;
	BftpAttNum	= AttNum - 1; 


	
	CmdType = SCEP_GetBig(pAttr->Value,4);
	switch(CmdType){


		case	BFTP_CTYPE_PUT:
#ifdef	SCEP_DEBUG
				cprintf("<Bftp:PUT>\n");
#endif
				BftpPutInd();
				BftpStatus	= BFTP_ST_PUT_IND;				
				break;
				
		case	BFTP_CTYPE_QUERY:
#ifdef	SCEP_DEBUG
				cprintf("<Bftp:QUERY>\n");
#endif
				BftpQueryInd();
				BftpStatus	= BFTP_ST_QUERY_IND;				
				break;
				
		default:
#ifdef	SCEP_DEBUG
				cprintf("<Bftp:UNKNOWN(%04lx)>\n",CmdType);
#endif
				Bftp_RplSglCnack(BFTP_ERROR_COMMAND);
				BftpStatus	= BFTP_ST_OTHER_IND;				
				break;
	}
	return;
}


static void BftpAckInd(void)
{
byte 		TRANPFAR *pData;
word 		AttNum;
	if(Bftp_Tbl.CBAckInd){
		pData		= BftpUserData;
		AttNum		= (word)SCEP_GetBig(pData,sizeof(word));
		pData 		+= sizeof(word);
		(*Bftp_Tbl.CBAckInd)((F_ATTHDR TRANPFAR *)pData,AttNum);
	}
}


static void BftpNackInd(void)
{
byte 		TRANPFAR *pData;
word 		AttNum;
	if(Bftp_Tbl.CBNackInd){
		pData		= BftpUserData;
		AttNum		= (word)SCEP_GetBig(pData,sizeof(word));
		pData 		+= sizeof(word);
		(*Bftp_Tbl.CBNackInd)((F_ATTHDR TRANPFAR *)pData,AttNum);
	}
}


static int BftpCheckPid(void)
{
	switch(BftpDstPid){
		case	BFTP_PID:
			return	0;
		case	BFTP_SONY:
			return	0;
		default:
#ifdef	SCEP_DEBUG
			cprintf("[PID(%d)]\n",BftpDstPid);
#endif
			break;
	}
	return	1;
}


static void BftpDataInd(void)
{
	if(BftpCheckPid()){
		Bftp_RplSglCnack(BFTP_ERROR_PID);
		return;
	}
	
	switch(	BftpPduType & SCEP_PTYPE_MASK){


		case	SCEP_PTYPE_REQ:
				BftpReqInd();
				break;
				
		case	SCEP_PTYPE_RPL_ACK:


#ifdef	SCEP_DEBUG
				cprintf("<Bftp:Rpl/Ack>\n");
#endif
				BftpStatus	= BFTP_ST_ACK_IND;
				BftpAckInd();
				break;


		case	SCEP_PTYPE_RPL_NACK:


#ifdef	SCEP_DEBUG
				cprintf("<Bftp:Rpl/Nack>\n");
#endif
				BftpNackInd();
				BftpStatus	= BFTP_ST_NACK_IND;
				break;
				
		case	SCEP_PTYPE_ABORT:


#ifdef	SCEP_DEBUG
				cprintf("<Bftp:Abort>\n");
#endif
				BftpStatus	= BFTP_ST_ABORT_IND;				
				break;
				
		default:
				break;
	}
	return;
}


void Bftp_CBCmdHdrInd(
			byte	*DstAddr,byte *SrcAddr,
			word	 DstPid, word  SrcPid, word CmdId,
			byte	 PduType
		)
{
int	type;
	type = PduType;
	memcpy(BftpDstAddr,DstAddr,sizeof(BftpDstAddr));
	memcpy(BftpSrcAddr,SrcAddr,sizeof(BftpSrcAddr));
	BftpDstPid	= DstPid;
	BftpSrcPid	= SrcPid;
	BftpCmdId	= CmdId;
	BftpPduType	= PduType;


#ifdef	SCEP_RECVMSG


	cprintf("\nFrom ");
	SCEP_HexPrint(BftpSrcAddr,8);
	cprintf(".Pid(%d)\n",BftpSrcPid);


	cprintf("To   ");
	SCEP_HexPrint(BftpDstAddr,8);
	cprintf(".Pid(%d)\n",BftpDstPid);


	cprintf("CmdId(%d)/PduType(%02x/",BftpCmdId,PduType);
	switch(type & 0xc0){
		case	0x00:
				cprintf("ReqPdu)\n");
				break;
		case	0x40:
				cprintf("Rpl[ACK])\n");
				break;
		case	0x80:
				cprintf("Rpl[NACK])\n");
				break;
		case	0xc0:
				cprintf("AbtPdu)\n");
				break;
	}


#endif
}


void Bftp_CBCmdBodyInd(
				byte DFLG,dword SeqNo,dword RestNo,
				byte *pData,word DataSize
		)
{
int		IndFlg,Dflg;
dword	Size;
#ifdef	SCEP_DEBUG
	cprintf("DFLG[%02x]<%ld/%ld>\n",DFLG,SeqNo,RestNo);
#endif
	Dflg = DFLG;
	Size = (int) DataSize;
	switch(Dflg){
		case	SCEP_DFLG_SINGL:		
		case	SCEP_DFLG_STOP:		
				IndFlg	= TRUE;
				memcpy(BftpUserData,pData,Size);
				BftpDataSize = Size;
				break;


		case	SCEP_DFLG_SEG1:	
				IndFlg	= FALSE;


				pBftpData = BftpUserData;
				memcpy(pBftpData,pData,DataSize);
				BftpDataSize = Size;
				pBftpData	+= Size;
				break;


		case	SCEP_DFLG_SEGI:	
				IndFlg	= FALSE;
				memcpy(pBftpData,pData,DataSize);
				BftpDataSize = Size;
				pBftpData	+= Size;
				break;


		case	SCEP_DFLG_SEGE:	
				IndFlg	= TRUE;
				memcpy(pBftpData,pData,DataSize);
				BftpDataSize = Size;
				pBftpData	+= Size;
				break;


		default:
				return;
				break;
	}
	if(IndFlg){
		BftpDataInd();
	}
}


#ifndef	IrTRANPNOOLDAPI


int	Bftp_PutReq(dword Size)
{
#ifdef	SCEP_DEBUG
	cprintf("Bftp:PutReq(Header)\n");
#endif
	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;


	SCEP_CommandHdrPdu(BFTP_PID,BFTP_PID,SCEP_PTYPE_REQ,Size);
	BftpPutCurPos	= BftpUserData;
	BftpPutCurSize	= Size;
	BftpDstDataSize	= SCEP_GetDstDataSize();
} 


int	Bftp_PutNext(void)
{
int		ret;
dword	csize;
byte	DFLG;
	if(BftpPutCurSize == 0)
		return	IrTranPR_ACCEPT;


	if(SCEP_QueryPduBusy())
		return	IrTranPR_PDU_BUSY;


 	if(BftpPutCurSize <= BftpDstDataSize){
		csize 	= BftpPutCurSize;
		DFLG	= SCEP_DFLG_SEGE;
	}else{
		csize	= BftpDstDataSize;
		DFLG	= SCEP_DFLG_SEGI;
	}


	ret = SCEP_CommandNextPdu(DFLG,BftpPutCurPos,csize);
	if(ret > 0){
		BftpPutCurPos	+= csize;
		BftpPutCurSize	-= csize;
		if(BftpPutCurSize == 0){
			return	IrTranPR_ACCEPT;
		}
	}
	return	IrTranPR_PRGRESS;
}


#endif


byte TRANPFAR *Bftp_PreFileHeader(char *FileName,word FileNameLen,dword FileSize)
{
byte 	TRANPFAR *pData;
byte	TRANPFAR *pDataTop;
dword	size;


	
	pDataTop = BftpUserData + SIZE_S_COMMAND;
	pData =	SCEP_PutBig(pDataTop,3,2);	
	pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_COMMAND,BFTP_ATYPE_BINARY,BFTP_AFLG_NOSUP,
				sizeof(dword)
			);
	pData	= SCEP_PutBig(pData,BFTP_CTYPE_PUT,sizeof(dword));


    
	if(FileNameLen == 12){
		pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_FILE,BFTP_ATYPE_TEXT,BFTP_AFLG_NOSUP,
				FileNameLen
			);
	}else{
		pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_LFILE,BFTP_ATYPE_TEXT,BFTP_AFLG_NOSUP,
				FileNameLen
			);
	}	
	memcpy(pData,FileName,(word)FileNameLen);
	pData += FileNameLen;


	
	pData =	Bftp_SetAttr(pData,
				(byte*)BFTP_ANAME_BODY,BFTP_ATYPE_BINARY,BFTP_AFLG_NOSUP,
				FileSize
			);


	size	= pData - pDataTop;	
	size	+= FileSize;


	
 	SCEP_PreCommandSegPdu(BftpUserData,BFTP_PID,BFTP_PID,SCEP_PTYPE_REQ,size);
	return	pData;
}


int	Bftp_PutBody(void)
{
	return	SCEP_SendCommandSegPdu();
}


void Bftp_BindSCEP(BFTPTBL *Btbl)
{
	Bftp_Tbl		= *Btbl;
	Bftp_SetUserBuffer(Bftp_Tbl.Buffer,Bftp_Tbl.BufferSize);
	Bftp_Tbl.Use	= TRUE;
}


void Bftp_UnbindSCEP(void)
{
	Bftp_Tbl.Use	= FALSE;
}


#endif