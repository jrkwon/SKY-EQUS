 /*==========================================================================
                              History
 99/8/24 Add dsm.h,udp.h for buffer usage
 99/8/24 Modify UDPa_sendReqeust( )
 99/8/24 Add msg.h for message log
 ==========================================================================*/

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

#include <ctype.h>
#include <string.h>
#include "capiudp.h"
#include "aapiclnt.h"
#include "cmmnrsrc.h"

#include "waptask.h"
#include "aapiudp.h"
#include "customer.h" // jwpark 00.04.27
#include "err.h"
#include "rex.h"
#include "ps.h"
#include "ds.h"
#include "dsi.h"
#include "comdef.h"
#include "dsm.h"
#include "udp.h" 
#include "msg.h"
#include "psi.h"
#include "ip.h"
#include "psmisc.h"
#include "netuser.h"
#include "dsm.h"

#if (defined WAP_B1) || (defined WAP_B2)
#include "dssock.h"
#include "psglobal.h"
#else //WAP_B1
#include "..\MMI\SK\email\dssock.h"
#endif //#if (defined WAP_B1) || (defined WAP_B2)

#if MMI_TYPE == SKY_UI
#include "sky.h"
#else //#if MMI_TYPE == SKY_UI
#include "ui.h"
#endif //#if MMI_TYPE == SKY_UI
#include "udp.h"
#include "clnt.h"
#include "mccdma.h"
#include "mmi_func.h"

#define MAX_DATA 536
#define UDPERROR

boolean dataval;
UINT16 SAport = 0;

extern unsigned long jjip_addr(char* cp);

static dsm_item_type     *payload_ptr = NULL; /* pointer to UDP payload                */
static dsm_item_type     *paytail_ptr = NULL; /* pointer to UDP payload                */

extern void wip_free(void* mem);
extern void *wip_malloc (UINT32 size);
extern BOOL Wtlshello;
extern VOID CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);
char save_source[4];char save_dest[4];

#ifdef USE_DNS
	extern BOOL dnsaccess;
#endif//#ifdef USE_DNS

#ifdef BROWSER_UPDATE
#define PDUTYPE       0x78
#define INVOKEPDU     0x08
#define ACKPDU        0x18
#define ABORTPDU      0x20
#define SEGPDU        0x28
#define RID			  0x01
#endif//#ifdef USE_CO

#ifdef  ABORT_IN_CONTENT  
BOOL bCall_content = FALSE;
#endif//#ifdef ABORT_IN_CONTENT  

#ifdef WAP_USE_DORM
	extern void wap_reorigination_attempt(void);
	extern unsigned short  wap_duration;
	extern void SKY_stop (UINT8 viewId);
#endif//#ifdef WAP_USE_DORM

#ifdef WAP_USE_SKT_NEW
	
    extern wap_release_state_enum_type wap_rel_state;
	boolean SetReleaseFun(void);
	int ReleaseErrorKind = 0;
#endif//#ifdef WAP_USE_SKT_NEW

#ifdef  WAP_USE_SECURE
#if MODEL_ID == MODEL_B1 || (MODEL_ID == MODEL_B2)
	#include "cwsplib.h"
	#include "const.h"
	#include "cssls.h"
#elif MODEL_ID == MODEL_B0 
    #include "..\WAP\secure\cwsplib.h"
	#include "..\WAP\secure\const.h"
	#include "..\WAP\secure\cssls.h"
#endif 
	#define SKT_SECURE_PORT	 "9091"
	const  int wtlsMode   = E2E_MODE;
	const  uc  SktCipher  = T_SEED;
	BOOL   wtls_stop = FALSE;
	BYTE* bExtractUrl = NULL;
	CHAR* savedata = NULL;UINT16 savedatalen = 0;
	CHAR* pNewpacket = NULL;UINT32 newlen =0;
	boolean bInDealloc = FALSE;
	boolean dispWtls   = FALSE;
	CHAR  AckPdu[4];

	static int wtlserr  = 0;
	//static UINT16 tempTid =0;
	static CHAR  tempTid[2] ={0};
	static CHAR  Wtpdata[4];
	static CHAR  Abortdata[6];
	static UINT16 saveport  = 0;
	static UINT16 Abortport = 0;

	extern BOOL Portdetect;
	extern CHAR SaveWtls[1000];
	extern uc ucVerifyDataOpen[1500];extern ul ulVerifyDataLenOpen;
	extern SSLS* spSKT;extern stPaq  ui_stpaq;	
	extern BOOL wtls_enc_start;

#ifdef WAP_USE_LEAK
	int  eXtractSecureUrl(CHAR* data);
#else
	BOOL eXtractSecureUrl(CHAR* data);
#endif//#ifdef WAP_USE_LEAK
	BOOL SendAckPdu(CHAR* data,INT16 state);
	void ProcessHandshake(void);
	void ClearSaveData(void);
	void SaveSecdata(CHAR* data, UINT16 datalength);
	void process_enc_start(void);
	void SendStopPacket2(CHAR* data,UINT16 port);

	extern void displayloading(int catagory,boolean setting);
#if (defined WAP_B1) || (defined WAP_B2)
	extern void SendUDPError(void);	
#endif //WAP_B1
	extern void close_udp_port(void);	
	extern boolean clear_ssls(void);
	extern int ssls_recv_hello(stPaq *stpPaq, int *npErr, uc *ucRecBuf, int nRecBufSize,char *cpUrl, int nMode);
	extern int ssls_open_send(stPaq *stpPaq,int *npErr,char *cpUrl, int nMode,uc ucCipher);
#endif//#ifdef WAP_USE_SECURE

#ifdef SOCKET_AUWAP

////////////////// AUWAP
//////////////////
extern sint15 g_sock_fd;
//int error_cnt = 0;
VOID  UDPa_sendRequest(const CHAR *data, UINT16 dataLength, const CHAR *destination, UINT8 destLength, const CHAR *source, UINT8 sourceLength, UINT16 destinationPort, UINT16 sourcePort)
{   
	sint15 sockfd;                    /* socket related variables */
	uint16 flags;
	struct sockaddr_in toaddr;
	sint15 errno;
	sint15 nbytes;
	sint15 bytes_per_write;
	uint32 tmp_toaddress;
	byte *tmp_dest;

	byte *tmp_src;
	uint32 tmp_fromaddress;

	tmp_dest = (byte *)destination;
	sockfd = g_sock_fd;
	memset( (char *) &toaddr, 0, sizeof(toaddr) );
	toaddr.sin_family = AF_INET;
	tmp_toaddress = get32(tmp_dest);
	toaddr.sin_addr.s_addr = dss_htonl(tmp_toaddress);

	toaddr.sin_port = dss_htons(destinationPort);
	flags = 0;
	nbytes = dataLength;


	tmp_src = (byte *)source;
	tmp_fromaddress = get32(tmp_src);
//	MSG_HIGH("pos[%d], port[%d]", pos, toaddr.sin_port, 0);
	MSG_HIGH("dest[%x] src[%x] dstlen[%d]", tmp_toaddress, tmp_fromaddress, destLength);

//#ifdef TEMP
//	if( error_cnt < 6 ) {
//		error_cnt++;
///////////////////
	bytes_per_write = dss_sendto( sockfd, 
								  (byte *)data,
								  nbytes,
								  flags,
								  (struct sockaddr *)&toaddr,
								  sizeof(struct sockaddr),
								  &errno
								);
	if( bytes_per_write == DSS_ERROR ) {
		MSG_HIGH("UDPa_sendRequest error[%d] ", errno, 0, 0);
	} else {
		MSG_HIGH("UDPa_send[%d] data[%x] nbyte[%d]", bytes_per_write, data, nbytes);
	}
//////////////////
//	}
//#endif

}
#else
VOID  UDPa_sendRequest(const CHAR *data, UINT16 dataLength, const CHAR *destination, UINT8 destLength, const CHAR *source, UINT8 sourceLength, UINT16 destinationPort, UINT16 sourcePort)
{    
  static udp_hdr_type      udp_hdr;     /* UDP header structure                  */
  ip_addr_type             dest_ip_ptr; /* Ptr to destination ip address         */
#if (defined WAP_B1) || (defined WAP_B2)
  ip4a                     temp_ip;     /* holds IP dest address, in correct form*/  
#else
  dword                    temp_ip;     /* holds IP dest address, in correct form*/  
#endif //WAP_B1
  sint15                   txdata_len;
  sint15                   payload_len; /* Length of the payload pkt.            */
  int i ;
  byte                     test[4];
#if (defined WAP_B1) || (defined WAP_B2)
  dsm_mempool_id_enum_type dsm_id;
#endif //WAP_B1

  extern BOOL   bCreateReq;
  extern BOOL   initialaccess;
  extern ip4a wapip;
  CHAR  temp_char;

  static boolean fstaccess = TRUE;
  static dsm_item_type* buf_item_ptr = NULL;

  ui_cmd_type   *ui_cmd_ptr;      
  extern void ClearWTLSdata(void);

  dataval =TRUE;
  payload_ptr  = NULL; paytail_ptr  = NULL;  buf_item_ptr = NULL;

  temp_char = data[0];
  

#ifdef WAP_USE_DORM
  /*===================================================================
		1. Check dormant state인지 아닌지..
		2. Release order를 받은 것인지 아닌지...
   ===================================================================*/
  CLNTa_log(1,0,"dorm %d;dorm_act %d",SKY_IS_WAP_DORM(),wap_dorm_active()); 

#ifdef WAP_USE_PZID2
  if((wap_dorm_active() == TRUE) && (SKY_IS_WAP_DORM() || SKY_IS_WAP_PZID()))
#else //#ifdef WAP_USE_PZID2
  if((wap_dorm_active() == TRUE) && SKY_IS_WAP_DORM())
#endif//#ifdef WAP_USE_PZID2
  {
    CLNTa_log(1,0,"Here aapid called");
    if(!Wtlshello) SKY_stop(1);

    //일단 dormant 모드일시는 session이 끊어지지 않으므로  close_upd_port()를 생략한다.
    //close_udp_port();

	if(wap_duration !=0 && cdma.p_rev_in_use >= 6)
	{
		if((data[0] & PDUTYPE) != ABORTPDU)
		{
			CLNTa_log(1,0,"reorigin PDU type %x method %x",data[0],data[4]);
			if ( ((data[0] & PDUTYPE)==INVOKEPDU) && (data[4] ==0x05) )
			{
				CLNTa_log(1,0,"disconnect pdu skip");
				return;
			}
			else
			{
				CLNTa_log(1,0,"Real reorigination");
				wap_reorigination_attempt();
			}
		}
		else
		{
			CLNTa_log(1,0," Abort pdu send when dormant");
		}
	}
    else
	{
        CLNTa_log(1,0," Protocol revision 5이하");
        CLNTa_log(1,0,"2.5G --> 2G");
		if( ((data[0] & PDUTYPE)==INVOKEPDU) && (data[4] == 0x05) ) 
		{
			CLNTa_log(1,0,"Disconnect PDU SKIP -1 ");
			return;
		}

        if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q))!= NULL)
		{
#ifdef WAP_USE_SKT_NEW
			ui_cmd_ptr->hdr.cmd        = UI_CALL_ENDED_F;//UI_CALL_RELEASE_F;
#else //WAP_USE_SKT_NEW
			ui_cmd_ptr->hdr.cmd        = UI_WAP_DORM_EXIT_F; //UI_DORM_EXIT_REQ_F
#endif//#ifdef WAP_USE_SKT_NEW
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
		}	
	}
	return;		
  }
  else//dormant 모드가 아닐 경우..
  {
	  extern void wap_cont_process(void);
	  if(cdma.p_rev_in_use >= 6  )
	  {
#ifdef WAP_USE_SKT_NEW
        //revision이 6이고 2G release가 행해졌을 경우,  
		if(SKY_IS_WAP_RELEASE()) //ui.wap_release
        {
            CLNTa_log(1,0," 2G-->2.5G");
			if( ((data[0] & PDUTYPE)==INVOKEPDU) && (data[4] == 0x05) ) 
			{
				CLNTa_log(1,0,"Disconnect PDU SKIP - 2");
				return;
			}

            if(SetReleaseFun() == TRUE) 
            {
                CLNTa_log(1,0," GO new session start");
                return;
            }
            else if(!dnsaccess)//dns packet을 제외하고 모두 skip
            {
                return;
            }
        }
        else
#endif//#ifdef WAP_USE_SKT_NEW
        {
		    CLNTa_log(1,0,"Dorm mode not entered yet");	  
		    wap_cont_process();
        }
	  }//if(cdma.p_rev_in_use >= 6  )
#ifdef WAP_USE_SKT_NEW
	  else if(SKY_IS_WAP_RELEASE())//ui.wap_release
      {
         if( ((data[0] & PDUTYPE)==INVOKEPDU) && (data[4] == 0x05) ) 
		 {
				CLNTa_log(1,0,"Disconnect PDU SKIP - 3");
				return;
		 }

		 if(SetReleaseFun() ==TRUE) 
         {
             CLNTa_log(1,0," GO new session start--2");
             return;
         }
         else if(!dnsaccess)//dns packet을 제외하고 모두 skip
         {
             return;
         }
      }
#endif//#ifdef WAP_USE_SKT_NEW
  }
#endif//#ifdef WAP_USE_DORM

#ifdef WAP_USE_SECURE
  if( Portdetect && Wtlshello == FALSE && wtls_enc_start == FALSE && ((temp_char & PDUTYPE) == INVOKEPDU) ) 
  {
#ifdef WAP_USE_DORM
    if(!SKY_IS_WAP_DORM() && !SKY_IS_WAP_RELEASE())	//!ui.wap_dorm_mode && !ui.wap_release
#endif//#ifdef WAP_USE_DORM
	{
		//dispWtls = TRUE;
		//displayloading(6,TRUE);
		if( (data[4] == 0x40) || (data[4] == 0x60) )
		{
            int tempRet = 0;
			tempRet = eXtractSecureUrl((CHAR*)data);
			if(tempRet == 1)
			{				
				ClearSaveData();
				SaveSecdata((CHAR*)data,dataLength);
				ProcessHandshake();
				return;
			}
#ifdef WAP_USE_LEAK
			else if(tempRet == 2)
			{
				return;
			}
#endif//#ifdef WAP_USE_LEAK
			else
			{
				SendUDPError();
				return;
			}
		}//if( (data[4] == 0x40) || (data[4] == 0x60) )
		else 
		{
			CLNTa_log(1,0,"다른 packet은 Skip when Secure state");
			//CLNTa_log(1,0,"type:%x,TID1=%x,TID2=%x method %d",data[0],data[1],data[2],data[4]);			
		}
	}//	if(!ui.wap_dorm_mode && !ui.wap_release)	
  }//if( Portdetect && Wtlshello == FALSE && wtls_enc_start == FALSE && ((temp_char & PDUTYPE) == INVOKEPDU) ) 
  else if(!Portdetect) 
  {
	  extern void close_wtls_not_air(void);
	  
	  close_wtls_not_air();memset(SaveWtls,0x00,1000);
	  //jjkyg 2001/02/12
	  ClearWTLSdata();
  }
  if(Wtlshello)
  {
	if((data[0] & PDUTYPE)==INVOKEPDU)
	{
		if( (data[4] == 0x40) || (data[4] == 0x60) )
		{
			CLNTa_log(1,0,"Processing");
		}
		else
		{
			CLNTa_log(1,0,"Not processing");
			return;
		} 
	}
	else if( (data[0] & PDUTYPE) == ABORTPDU  )
	{
		if(!wtls_stop)
		{
			CLNTa_log(1,0,"SKIP PDU"); return;
		}
		else wtls_stop = FALSE;
	}  	
  }  
#endif//#ifdef WAP_USE_SECURE

#ifdef BROWSER_UPDATE
  /*=========================================================================
	   - Initial access일 경우 End key및 CLR_k와 같은 종료를 일시 막기위한 것
	   - jjkyg 00/06/19 porting
 ==========================================================================*/
	if(bCreateReq && initialaccess)
	{
		if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q))!= NULL)
		{
#ifdef PORTING_B0_WAP
			ui_cmd_ptr->hdr.cmd        = UI_WAP_REQ_F;
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
#endif //PORTING_B0_WAP
		}	
	}
#endif//#ifdef BROWSER_UPDATE

   if((dataLength&0x8000) || (dataLength==0) ||(sourceLength==0))
  {
	  dataval =FALSE;UDPc_errorRequest(2,sourcePort);
	  return;	 
  }

  txdata_len = (sint15) dataLength;

  memcpy(save_source,source,4);memcpy(save_dest,destination,4);
 
  if((data[0] & PDUTYPE)==INVOKEPDU)
  {
	 CLNTa_log(1,0,"TID1 =%x,TID2=%x,Method=%x datalength %d",data[1],data[2],data[4],dataLength);			 
  }
  else if((data[0] & PDUTYPE) == ACKPDU)
  {
	  CLNTa_log(1,0,"ACK SEND");	
  }
  else if((data[0] & PDUTYPE) == ABORTPDU)
  {
#ifdef ABORT_IN_CONTENT  
	  if(bCall_content) 
	  {
		  CLNTa_log(1,0,"ABORT SEND when called getcontent");
		  UDPc_errorRequest(2,SAport);		  
	  }
#endif//#ifdef ABORT_IN_CONTENT  

	  CLNTa_log(1,0,"ABORT SEND reason %x",data[3]);
//	  CLNTa_log(1,0,"A_TID1 =%x,TID2=%x,Method=%x datalength %d",data[1],data[2],data[4],dataLength);			 
  }
  else if((data[0] & PDUTYPE) == SEGPDU)
  {
	 CLNTa_log(1,0,"Segment %x",data[3]);	
  }
  else
  {
	 CLNTa_log(1,0,"Wrong PDU %x datalen %d ",data[0],txdata_len);	 
	 if(txdata_len!=0)
	 {
		 CLNTa_log(1,0,"PDU length %d,PDU type%x",data[1],data[2]);
	 }	 
  }

  if(destLength!=4) dataval =FALSE;
  else
  {
 	if(dnsaccess)
	{
#if (defined WAP_B1) || (defined WAP_B2)
		temp_ip = get32((byte*)destination);
#else //WAP_B1
		temp_ip = ((dword*)&destination[0])[0];
#endif //WAP_B1
		for(i =0;i<4;i++)
		{
			test[i] = ((byte*)&temp_ip)[i];
			CLNTa_log(1,0,"DNS DEST IP %d",test[i]);				
		}
	}
	else//!dnsaccess
	{
		temp_ip = wapip;
		for(i =0;i<4;i++)
		{
			test[i] = ((byte*)&temp_ip)[i];
			CLNTa_log(1,0,"DEST IP %d",test[i]);				
		}
	}
	dest_ip_ptr =(ip_addr_type)temp_ip;
  }//end of else

  //udp setting
  udp_hdr.source = sourcePort;
  MSG_ERROR("s-port %d",udp_hdr.source,0,0);
	
  udp_hdr.dest   = destinationPort;
  MSG_ERROR("D-port %d",udp_hdr.dest,0,0);

  udp_hdr.length = 0;
  udp_hdr.chksum = 0;

  if(txdata_len > MAX_DATA)  payload_len = MAX_DATA;
  else payload_len = txdata_len;

  while(payload_len!=0)
  {
	if(fstaccess)
	{
#if (defined WAP_B1) || (defined WAP_B2)
		if((payload_len+sizeof(udp_hdr_type)+4) < DSM_DS_SMALL_ITEM_SIZ)
		{
			payload_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);
#else //WAP_B1
		if((payload_len+sizeof(udp_hdr_type)+4) < DSM_SMALL_ITEM_SIZ)
		{
			payload_ptr = dsm_new_buffer(DSM_SMALL_ITEM_SIZ);
#endif //WAP_B1
			if (payload_ptr==NULL)
			{
		        CLNTa_log(1,0,"WAP has no DSM items!");
			}
		}
		else
		{
#if (defined WAP_B1) || (defined WAP_B2)
			payload_ptr = dsm_new_buffer(DSM_DS_LARGE_ITEM_POOL);
#else //WAP_B1
			payload_ptr = dsm_new_buffer(DSM_LARGE_ITEM_SIZ);
#endif //WAP_B1
			if (payload_ptr==NULL)
			{
		        CLNTa_log(1,0,"WAP has no DSM items!");
			}
		}

		paytail_ptr  = payload_ptr;	buf_item_ptr = payload_ptr;fstaccess = FALSE;
		MSG_ERROR("buf allocaton1",0,0,0);
	}
	else
	{
#if (defined WAP_B1) || (defined WAP_B2)
        if((payload_len+sizeof(udp_hdr_type)+4) < DSM_DS_SMALL_ITEM_SIZ)
		{
			buf_item_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);
#else //WAP_B1
		if((payload_len+sizeof(udp_hdr_type)+4) < DSM_SMALL_ITEM_SIZ)
		{
			buf_item_ptr = dsm_new_buffer(DSM_SMALL_ITEM_SIZ);
#endif //WAP_B1
			if (buf_item_ptr==NULL)
			{
		        //Error처리 할것
				CLNTa_log(1,0,"WAP has no DSM items!");
			}
		}
		else
		{
#if (defined WAP_B1) || (defined WAP_B2)
			buf_item_ptr = dsm_new_buffer(DSM_DS_LARGE_ITEM_POOL);
#else //WAP_B1
			buf_item_ptr = dsm_new_buffer(DSM_LARGE_ITEM_SIZ);
#endif //WAP_B1
			if (buf_item_ptr==NULL)
			{
		        //Error처리 할것
				CLNTa_log(1,0,"WAP has no DSM items!");
			}
		}
		paytail_ptr->pkt_ptr = buf_item_ptr;paytail_ptr = buf_item_ptr;
		MSG_ERROR("buf allocaton2",0,0,0);
	}

	memcpy(buf_item_ptr->data_ptr,data,(UINT16)payload_len);
	buf_item_ptr->used = payload_len;
	MSG_ERROR("buf_item_ptr->used %d %x",buf_item_ptr->used,buf_item_ptr->used,0);
	
	data = data + payload_len;

#ifndef newsrc
	//IP가 0이라는 것은 아직 할당 받지 했다는 것
	if((psi_state != PS_STATE_OPEN)||(Ip_addr==0)){
		ds_wap_fst =TRUE;		
	}
	MSG_ERROR("set sig",0,0,0);
#endif//newsrc

	udp_hdr.length +=payload_len;
	txdata_len -= payload_len;

	if(txdata_len > MAX_DATA)  payload_len = MAX_DATA;
	else  payload_len = txdata_len;

	if(payload_len < 0 )
	{
	  UDPc_errorRequest(2,sourcePort);return;
	}
  }//end of while

  udp_hdr.length += sizeof(udp_hdr_type);

  
#if (defined WAP_B1) || (defined WAP_B2)
    //dsm id determine
  dsm_id = DSM_DS_SMALL_ITEM_POOL;
  if(udp_hdr.length  >  DSM_DS_SMALL_ITEM_SIZ) dsm_id = DSM_DS_LARGE_ITEM_POOL;  
  if (dsm_pushdown( &payload_ptr, &udp_hdr, sizeof(udp_hdr_type),dsm_id) == FALSE)
  {
	dsm_free_packet(&payload_ptr);
	return;
  }
  if(dsm_pushdown( &payload_ptr, &dest_ip_ptr, 4, DSM_DS_SMALL_ITEM_POOL) == FALSE)
  {
	dsm_free_packet(&payload_ptr);
	return;
  } 
#else //WAP_B1
  dsm_pushdown( &payload_ptr,&udp_hdr,sizeof(udp_hdr_type));
  dsm_pushdown( &payload_ptr, &dest_ip_ptr, 4);
#endif //#if (defined WAP_B1) || (defined WAP_B2)
  
  if((q_cnt(&psi_udp_out_q)< 4)&&(dataval))
  {
	(void)rex_clr_sigs(&ps_tcb,PS_WAP_SIG);	
	payload_ptr->kind = DSM_WAP_DATA;q_put(&psi_udp_out_q,&payload_ptr->link);rex_set_sigs(&ps_tcb,PS_WAP_SIG);	

#ifdef PORTING_B0_WAP
	if((temp_char==INVOKEPDU)||(temp_char==SEGPDU))
	{
		if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q))!= NULL)
		{

			ui_cmd_ptr->hdr.cmd        = UI_CHAT_REQ_F;
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);

		}
	}
#endif//#ifdef PORTING_B0_WAP
  }
  else if(q_cnt(&psi_udp_out_q)>= 4)
  {
	dsm_free_packet(&payload_ptr);
  }
  else//dataval=FALSE 
  {
	CLNTa_log(1,0,"dataval = FALSE");
  }
  fstaccess = TRUE;
}
#endif //SOCKET_AUWAP

#ifdef WAP_USE_SKT_NEW
boolean SetReleaseFun(void)
{
    ui_cmd_type   *ui_cmd_ptr;      
	db_items_value_type db;   
    extern void close_udp_port(void);

    if(!Wtlshello) SKY_stop(1);
    close_udp_port();
    if(wap_rel_state == REL_ENABLE_ORIG)
    {
		extern void do_ds_upenter(void);
		CLNTa_log(1,0,"Enable 2G origination");
		db_get( DB_NO_SVC, &db);
		if(!db.no_svc)
		{
			CLNTa_log(1,0,"2G orig before upenter");
			wap_rel_state = REL_INPROG;
			if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q))!= NULL)
			{
				//ui_cmd_ptr->hdr.cmd        = UI_DORM_EXIT_REQ_F;
				ui_cmd_ptr->hdr.cmd        = UI_WAP_DORM_EXIT_F;				
				ui_cmd_ptr->hdr.task_ptr   = NULL;
				ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
				ui_cmd( ui_cmd_ptr);
			}
			do_ds_upenter();
			return TRUE;
		}
		else
		{
#if (defined WAP_B1) || (defined WAP_B2)
			extern void SendUDPError(void);
			SendUDPError();
#endif //WAP_B1
			ReleaseErrorKind = 1;
			return FALSE;
		}
     }
     else if(wap_rel_state == REL_INPROG)
     {
         CLNTa_log(1,0,"Already call origination");         
     }
     else
     {
         CLNTa_log(1,0,"그럼 뭘까?");
		 if(wap_rel_state == REL_NULL) 
		 {
			 CLNTa_log(1,0,"이렇게 해도 될까?");
			 wap_rel_state = REL_ENABLE_ORIG;
		 }
     }
     return FALSE;
}
#endif//#ifdef WAP_USE_SKT_NEW

#ifdef WAP_USE_SECURE
void SendcmdToWAP(wap_name_type name)
{
	wap_cmd_type* wap_event_ptr;
	if(( wap_event_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
	{
		wap_event_ptr->hdr.task_ptr   = NULL;
		wap_event_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
	}
	wap_event_ptr->hdr.cmd = name;
	send_cmd_other2wap(wap_event_ptr);
}

extern wap_handshake_state handShakestate;
extern boolean bSetKey;
void ProcessHandshake(void)
{
  if((SaveWtls[0]!= 0) && (bExtractUrl !=NULL) && (strcmp((char *)bExtractUrl,SaveWtls) ==0))
	{	
		if(bSetKey)	
		{
			Wtlshello = FALSE;wtls_enc_start = TRUE;

			dispWtls = FALSE;
			SKY_stop(1);

			if(bExtractUrl!=NULL)
			{
				wip_free(bExtractUrl);bExtractUrl=NULL;
			}	
			process_enc_start();
			return;
		}
		else 
		{
			close_udp_port();
			Wtlshello = TRUE;bSetKey = FALSE;
			dispWtls = FALSE;

			handShakestate = HAND_CLIENT_STATE;
		}
	}
	else if(bExtractUrl !=NULL)
	{
		close_udp_port();
		Wtlshello = TRUE;bSetKey = FALSE;
		dispWtls  = FALSE;

		handShakestate = HAND_CLIENT_STATE;
		memset(SaveWtls,0x00,1000);strncpy(SaveWtls,(char*)bExtractUrl,1000);	
	}
	 
	if(bExtractUrl!=NULL)
	{
		wip_free(bExtractUrl);bExtractUrl=NULL;
	}	

	SendcmdToWAP(WAP_SECURE_CMD);
}

extern BOOL b_GetPort (const BYTE* pbUrl, BYTE **port);
#ifdef WAP_USE_LEAK
	int eXtractSecureUrl(CHAR* data)
#else
	BOOL eXtractSecureUrl(CHAR* data)
#endif//#ifdef WAP_USE_LEAK
{
	BYTE* bPortptr    = NULL; 

	UINT32 uintvarlen = 0;
	UINT32 Urilen     = 0;

	UINT32 Headervarlen  = 0;
	UINT32 Headerlen  = 0;

	BYTE* ptemp = NULL;
	BOOL  findPort = FALSE;
	
	if(bExtractUrl != NULL) 
	{
		wip_free(bExtractUrl);bExtractUrl = NULL;
	}

/*=================================================================
			Method = 0x40(Get method 사용시)		
	 구조:  WTP header / Method / URILEN / URI / HEADER
 =================================================================*/
	if(data[4] == 0x40)//if Method is Get
	{
		if((uintvarlen = UintvarToInt ((BYTE *)&data[5], &Urilen, 5)) == 0) return FALSE;
		
		bExtractUrl = (BYTE*)wip_malloc(Urilen +1);
        if(bExtractUrl ==  NULL) 
		{
	#ifdef WAP_USE_LEAK			
			return 2;
	#else
			return 0;
	#endif//#ifdef WAP_USE_LEAK	
        }
		memset(bExtractUrl, 0x00,Urilen+1);
		memcpy(bExtractUrl,&data[5+uintvarlen],Urilen);		
	}
/*==============================================================================
			Method = 0x60(Get method 사용시)		
	 구조:  WTP header / Method / URILEN /Headerlen / URI / content type/ HEADER
 ===============================================================================*/
	else if(data[4] ==0x60)//if method is POST
	{
		if((uintvarlen = UintvarToInt ((BYTE *)&data[5], &Urilen, 5)) == 0) return FALSE; 
		if((Headervarlen  = UintvarToInt ((BYTE *)&data[5 + uintvarlen], &Headerlen, 5)) == 0) return FALSE; 

		ptemp = (BYTE *)&data[5+uintvarlen+Headervarlen];
		bExtractUrl = (BYTE*)wip_malloc(Urilen +1);
        if(bExtractUrl ==  NULL) 
		{
	#ifdef WAP_USE_LEAK			
			return 2;
	#else
			return 0;
	#endif//#ifdef WAP_USE_LEAK			
		}
		memset(bExtractUrl, 0x00,Urilen+1);
		memcpy(bExtractUrl,ptemp,Urilen);		
	}
	
	if(bExtractUrl != NULL)
	{
		bInDealloc = TRUE;
		findPort = b_GetPort(bExtractUrl,&bPortptr);
		if( (strcmp((char *)bPortptr,SKT_SECURE_PORT)==0) &&(findPort ==TRUE) ) 
		{
			DEALLOC(&bPortptr);
			bInDealloc = FALSE;
			return TRUE;		
		}
		DEALLOC(&bPortptr);
		bInDealloc = FALSE;
	}
	return FALSE;
}

BOOL CheckspSkt(void)
{
	if(spSKT !=NULL) clear_ssls();
	else 
	{
#if (defined WAP_B1) || (defined WAP_B2)
		SendUDPError();
#endif 
        return FALSE;
	}
	return TRUE;
}

BOOL ExtractPostdata(CHAR* data,UINT16 datalen)
{
	UINT16    SDU_size = datalen;
	UINT32    headlen, bodylen;
	UINT32    hn;
	UINT32    extractlen = 0;

	if ( (data == NULL)||(SDU_size <= 3))
	{
#if (defined WAP_B1) || (defined WAP_B2)
		SendUDPError();
#endif 
        return FALSE;
	}

	extractlen =3 + 1 + 1;//WTP header +  Method  + status 
	if ((hn = UintvarToInt((BYTE *)&data[extractlen],&headlen, 5)) == 0) 
	{
#if (defined WAP_B1) || (defined WAP_B2)
		SendUDPError();
#endif //WAP_B1
        return FALSE;
	}
	extractlen += hn;//header var length

	if((int)(extractlen) > SDU_size) { 
#if (defined WAP_B1) || (defined WAP_B2)
        SendUDPError();
#endif 
        return FALSE; }

	extractlen +=headlen;//header length add

	bodylen = SDU_size - extractlen;

	if(bodylen > 0)
	{
		extern char contentBuffer[MAX_CONTENT_SIZE];
		int nRet;
		memset(contentBuffer,0x00,MAX_CONTENT_SIZE);
		memcpy(contentBuffer,&data[extractlen],bodylen);
		nRet = ssls_recv_hello(&ui_stpaq,&wtlserr,(uc*)contentBuffer,(UINT16)bodylen,(char*)SaveWtls,SktCipher);	
		if(nRet < 0)
        {
#if (defined WAP_B1) || (defined WAP_B2)
            SendUDPError();
#endif 
            return FALSE;
        }
#ifdef WAP_USE_LEAK
		else if( nRet == 2 )
		{
			CLNTa_log(1,0,"Leak occurred");
			return FALSE;
		}
#endif//#ifdef WAP_USE_LEAK
	}
	else {
#if (defined WAP_B1) || (defined WAP_B2)
        SendUDPError();
#endif //#if (defined WAP_B1) || (defined WAP_B2)
        return FALSE;}

	return TRUE;
}

void ClearSaveData(void)
{
	if(savedata!=NULL)
	{
		wip_free(savedata);savedata = NULL;savedatalen = 0;
	}
}

void process_enc_start(void)
{
#if MODEL_ID != MODEL_B0
	extern void go_storeWml_normal(BOOL del);
#endif
	bSetKey = TRUE;//Nego success					
	wtls_enc_start = TRUE;clear_ssls();ClearSaveData();		
	Wtlshello = FALSE;
	handShakestate = HAND_NULL_STATE;

	//Start KEY emulation
#if MODEL_ID != MODEL_B0
	go_storeWml_normal(FALSE);
#endif
}
void process_secure_client(CHAR* data,UINT16 datalen,UINT16 sport)
{
	static int return_ssls;
	
	switch(handShakestate)
	{
		case HAND_CLIENT_STATE:
		{
			return_ssls = 0;
			memset(ucVerifyDataOpen,0x00,1500);
			ulVerifyDataLenOpen = 0;
			memset(&ui_stpaq,0, sizeof(stPaq));
			return_ssls = ssls_open_send(&ui_stpaq,&wtlserr,(char*)SaveWtls,wtlsMode,SktCipher);
#if (defined WAP_B1) || (defined WAP_B2)
			if(return_ssls == 0)
                SendUDPError();
#ifdef WAP_USE_LEAK
			else if(return_ssls == 2) break;
#endif//#ifdef WAP_USE_LEAK
			else handShakestate = HAND_WAIT_SERVER_STATE;
#endif 
			break;
		}
		
		case HAND_WAIT_SERVER_STATE:
		{ 
			if(CheckspSkt() == FALSE) break;

			/*=============================================================
					        먼저 비교 한후 이를 넘김 
			 =============================================================*/
			data[1] = data[1] & 0x7f;
			if(sport != SAport || memcmp(&data[1],&savedata[1],2)!=0)
			{
				SendStopPacket2(data,sport);
				return;
			}

			memset((void *)Wtpdata,0x00,4);memcpy((void *)Wtpdata,data,3);

			CLNTa_log(1,0,"555");
			if(SendAckPdu(Wtpdata,1) == FALSE) return;

			if(spSKT->ucState == ST_WAIT) 
			{
				if(ExtractPostdata(data,datalen)==FALSE) break;
				
				if(spSKT->ucState == ST_OPEN)//Nego finished..
				{
					process_enc_start();
					break;
				}
				handShakestate = HAND_FINISHED_STATE;
			}
			break;
		}

		case HAND_FINISHED_STATE:
		{
			//UINT16 tempfi = 0;
			CHAR tempfi[2] = {0};
			memset(Wtpdata,0x00,4);memcpy(Wtpdata,data,3);
			CLNTa_log(1,0,"666");
			
			/*=============================================================
					        먼저 비교 한후 이를 넘김 
			 =============================================================*/
			data[1] = data[1] & 0x7f;
			//tempfi = ((UINT16*)&data[1])[0];
			//tempfi = (data[2]<<8)+data[1];
			tempfi[0] = data[1];
			tempfi[1] = data[2];

			//if(sport != SAport || tempTid !=tempfi)
			if( sport != SAport || !(tempTid[0] == tempfi[0] && tempTid[1] == tempfi[1])  )
			{
				SendStopPacket2(data,sport);
				return;
			}

			if(SendAckPdu(Wtpdata,2) == FALSE) return;
			if(spSKT->ucState == ST_EXKEY)
			{
				if(ExtractPostdata(data,datalen)==FALSE) break;
				if(spSKT->ucState == ST_OPEN)//Nego finished..
				{
					process_enc_start();
					break;
				}				
			}
			break;
		}
	}//switch(handShakestate)
}

void SaveSecdata(CHAR* data, UINT16 datalength)
{
	savedata = (CHAR*)wip_malloc(datalength +1);
	memset(savedata,0x00,datalength+1);memcpy(savedata,data,datalength);
	savedatalen = datalength;
}

void ClearNewPacket(void)
{
	if(pNewpacket!=NULL)
	{
		wip_free(pNewpacket);pNewpacket = NULL;
		newlen = 0;
	}			
}

#ifdef WAP_USE_LEAK
	int makeWspPacket(unsigned char* data, unsigned int Len)//data : POST handshake data, Len : Post handshake data length
#else
	BOOL makeWspPacket(unsigned char* data, unsigned int Len)//data : POST handshake data, Len : Post handshake data length
#endif//#ifdef WAP_USE_LEAK
{
	/*====================================================================
	   Post일시는 Content type의 length도 header length에 포함되어 진다.		
	 ====================================================================*/
	 CHAR* ptemp = NULL;
	 CHAR *pUrl = NULL;CHAR *pHeader = NULL;
	 UINT32 uintvarlen = 0;UINT32 Urilen = 0;
	 UINT32 Headervarlen  = 0;UINT32 Headerlen  = 0;CHAR newHeader[5];
	 
	/*=====================================================================
	  if Get method일시, content type 및  header부분만 넣으면 된다.
	  =====================================================================*/
	uintvarlen  = UintvarToInt ((BYTE *)&savedata[5], &Urilen, 5);
	if(savedata[4] == 0x40)
	{
		savedata[4] = 0x60;
		pUrl = (CHAR*)wip_malloc(Urilen +1);memset(pUrl,0x00,Urilen+1);
		if(pUrl == NULL)
		{
#ifdef WAP_USE_LEAK
			return 2;
#else
			return 0;
#endif//#ifdef WAP_USE_LEAK
		}
		memcpy(pUrl,&savedata[5+uintvarlen],Urilen);

		Headerlen   = savedatalen - uintvarlen - Urilen - 5;//5 = WTP header + Method
		pHeader     = (CHAR*)wip_malloc(Headerlen+2);//header + content type + spare
        if(pHeader == NULL)
		{
#ifdef WAP_USE_LEAK
			return 2;
#else
			return 0;
#endif//#ifdef WAP_USE_LEAK
		}
		memset(pHeader,0x00,Headerlen + 2);

		pHeader[0] = 0x92;/* application/x-form-urlencoded */
		memcpy(&pHeader[1],&savedata[5+uintvarlen+Urilen],Headerlen);Headerlen++;//for content type field insert

		//InttoVar trans..
		memset(newHeader,0x00,5);
		Headervarlen = IntToUintvar(Headerlen, (BYTE *)newHeader);
	}
	/*=====================================================================
	  if POST method일시, content type 및  POST data만 넣으면 된다.
	 =====================================================================*/	
	else if(savedata[4] == 0x60)
	{
		Headervarlen = UintvarToInt((BYTE *)&savedata[5 + uintvarlen],&Headerlen, 5);
	}
	else return FALSE;

	//make new post packet
	ClearNewPacket();

	newlen = 5+uintvarlen+Headervarlen+Urilen+Headerlen+Len;
	pNewpacket = (CHAR*)wip_malloc(5+uintvarlen+Headervarlen+Urilen+Headerlen+Len);
    if(pNewpacket == NULL)
	{
#ifdef WAP_USE_LEAK
			return 2;
#else
			return 0;
#endif//#ifdef WAP_USE_LEAK
	}
	memset(pNewpacket,0x00,newlen);
	ptemp = pNewpacket;

	if(savedata[4] == 0x40)
	{
		memcpy(ptemp,savedata,5+uintvarlen);ptemp+=5+uintvarlen;
		memcpy(ptemp,newHeader,Headervarlen);ptemp+=Headerlen;
		memcpy(ptemp,pUrl,Urilen);ptemp+=Urilen;
		memcpy(ptemp,pHeader,Headerlen);ptemp+=Headerlen;
		wip_free(pUrl);pUrl = NULL;wip_free(pHeader);pHeader= NULL;
	}
	else if(savedata[4] == 0x60)
	{
		memcpy(ptemp,savedata,5+uintvarlen+Headervarlen+Urilen+Headerlen);
		ptemp+=5+uintvarlen+Headervarlen+Urilen+Headerlen;
	}
	memcpy(ptemp,data,Len);

	UDPa_sendRequest(pNewpacket, newlen, (const char*)&wapip,4,(const char*)&Ip_addr,4,9201,SAport);
	ClearNewPacket();
	return TRUE;
}

int clnt_wsp_send(unsigned char *ucpSendData, unsigned int nSendLen)
{
#ifdef WAP_USE_LEAK	
	int tempRet2 = 0;
#endif//#ifdef WAP_USE_LEAK	
	//tempTid = 0;
	tempTid[0] = 0;tempTid[1] = 0;

	if(nSendLen > 1500) return -1;
    if(savedata == NULL) 
	{
#ifdef WAP_USE_LEAK
		return 2;
#else
		return -1;
#endif//#ifdef WAP_USE_LEAK
	}

	if(spSKT->ucState == ST_WAIT)//TID를 1 증가 시킨다.
	{
		//tempTid = ((UINT16*)&savedata[1])[0];
		//tempTid = (savedata[2]<<8)+savedata[1];
		//tempTid++;
		//savedata[1] = ((CHAR*)&tempTid)[0];// 원본과 달리 hi low가 바뀌는 현상때문에 순서를 바꿨다.!!!
		//savedata[2] = ((CHAR*)&tempTid)[1];		
		tempTid[0] = savedata[1];
		tempTid[1] = savedata[2];
		tempTid[1]++;
		savedata[1] = tempTid[0];
		savedata[2] = tempTid[1];
	}

#ifdef WAP_USE_LEAK
	tempRet2 = makeWspPacket(ucpSendData,nSendLen) ;
	if(tempRet2 == 0 ) return -1;
	else if(tempRet2 == 2 ) return 2;
#else
	if(makeWspPacket(ucpSendData,nSendLen) == FALSE ) return -1;	
#endif//#ifdef WAP_USE_LEAK
	return 0;	
}

void wip_realloc(CHAR* data, UINT16 newlength,UINT16 oldLen)
{
	CHAR* pdata = NULL;
	pdata = (CHAR*)wip_malloc(oldLen);
	memcpy(pdata,data,oldLen);
	wip_free((CHAR*)data);data = NULL;

	data =(CHAR*)wip_malloc(newlength);
	memset(data,0x00,newlength);

	if(newlength > oldLen) 
	{
		memcpy(data, pdata,oldLen);wip_free(pdata);pdata = NULL;		
	}
	else
	{
		ERR_FATAL("assign error",0,0,0);
	}
}

BOOL SendAckPdu(CHAR* data,INT16 state)
{
	/*==============================================================
			  		  Check TID 및 RID
		전에 보냈던 데이타와 동일 할 경우 에는 RID만 setting해서
		보낸다.

		Port number는 ????
	 ===============================================================*/
	if((data[0] & RID) == RID)
	{
		CLNTa_log(1,0,"111");
		data[1] = data[1] & 0x7f;
		if(memcmp(&data[1],&AckPdu[1],2) == 0)
		{
			CLNTa_log(1,0,"222");
			AckPdu[1] = (AckPdu[1] | 0x1);//RID field 
			UDPa_sendRequest((const CHAR*)AckPdu, 3, (const char*)&wapip,4,(const char*)&Ip_addr,4,9201,saveport);					
		}
		else if(memcmp(&data[1],&Abortdata[1],2) == 0)
		{
			CLNTa_log(1,0,"333");
			UDPa_sendRequest((const CHAR*)Abortdata, 4, (const char*)&wapip,4,(const char*)&Ip_addr,4,9201,Abortport);
		}
		else
		{
			CLNTa_log(1,0,"444");
#if (defined WAP_B1) || (defined WAP_B2)
			SendUDPError();			
#endif //WAP_B1
		}
		return FALSE;
	}

	memset(AckPdu,0x00,4);
	AckPdu[0] = ACKPDU;

	if(state == 2) 
	{
		//AckPdu[1] = ((CHAR*)&tempTid)[1];
		//AckPdu[2] = ((CHAR*)&tempTid)[0];
		AckPdu[1] = tempTid[0];
		AckPdu[2] = tempTid[1];
	}
	else
	{
		AckPdu[1] = savedata[1];
		AckPdu[2] = savedata[2];
	}

	UDPa_sendRequest((const CHAR*)AckPdu, 3, (const char*)&wapip,4,(const char*)&Ip_addr,4,9201,SAport);
	saveport = SAport;
	return TRUE;
}

void SendStopPacket(void)
{
	wtls_stop = TRUE;
	memset(Abortdata,0x00,6);

	Abortdata[0] = 0x21;//Abort type
	memcpy(&Abortdata[1],&savedata[1],2);
	Abortdata[3] = 0xe8;//Abort reason
	UDPa_sendRequest((const CHAR*)Abortdata, 4, (const char*)&wapip,4,(const char*)&Ip_addr,4,9201,SAport);
	Abortport = SAport;
}

void SendStopPacket2(CHAR* data,UINT16 port)
{
	wtls_stop = TRUE;
	memset(Abortdata,0x00,6);

	Abortdata[0] = 0x21;//Abort type
	memcpy(&Abortdata[1],&data[1],2);
	Abortdata[3] = 0xe8;//Abort reason
	UDPa_sendRequest((const CHAR*)Abortdata, 4, (const char*)&wapip,4,(const char*)&Ip_addr,4,9201,port);
}

void ClearWTLSdata(void)
{
	if(bExtractUrl != NULL) 
	{
		wip_free(bExtractUrl);bExtractUrl = NULL;
	}

	if(savedata != NULL)
	{
		wip_free(savedata);savedata = NULL;
		savedatalen = 0;
	}

	if(pNewpacket != NULL)
	{
		wip_free(pNewpacket);pNewpacket = NULL;
		newlen =0;
	}

	Wtlshello = FALSE;
	dispWtls = FALSE;

	//JJKYG	01/02/06
	handShakestate = HAND_NULL_STATE; 
} 

#ifdef WAP_USE_LEAK
void ClearWTLSMem(void)
{
	bExtractUrl = NULL;
	savedata = NULL;savedatalen = 0;
	pNewpacket = NULL;
	newlen =0;
	Wtlshello = FALSE;
	dispWtls = FALSE;
	handShakestate = HAND_NULL_STATE; 
}
#endif//#ifdef WAP_USE_LEAK                                          
#endif//#ifdef WAP_USE_SECURE



