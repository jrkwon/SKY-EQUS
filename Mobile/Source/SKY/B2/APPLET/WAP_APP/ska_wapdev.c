#ifdef FEATURE_WAP
/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_wapdev.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-07-18   jmpark     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"
#include "psglobal.h"
#include "psmisc.h"
#include "wapcommon.h"
#include "wapconnector.h"

#include "sisdecoder.h"

#include "sky_sock.h"
#include "netuser.h"

#include "clnt.h"
#include "capimem.h"
#include "capimmi.h"
#include "tapimmi.h"

#include "msg.h"
#include "err.h"

#include "psmisc.h"
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#define ChannelID        1
#define contentChannel   128
#define mobilePort       1000
#define INITURL_TIMEOUT  60*60

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  External variables
**---------------------------------------------------------------------------
*/
extern const    memSizeType   cCacheSize;
extern const    memSizeType   crestoredSize;

#ifdef  FEATURE_SKT_EIF //yjahn96 01.10.08 - eif for wap
extern INT  g_nEIFWapstatus;
extern char EIF_url[192];
extern char Dest_IP[4];
extern ip4a Dest_url_IP;
#endif  //#ifdef  FEATURE_SKT_EIF

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
extern uint16 SAport;

struct sockaddr_in addr;	

// To exit WAP applet, decide what state is in.  SetupConection used.
enum {
	NULL_S,         //Initial State, Dormant State
	SETUP_S,        //CLNTc_setupConnection Called
	SETUPDONE_S,    //CLNTa_setupConnectionDone Called
	SETUPFAIL_S     //CLNTa_setupConnectionDone Fail
};

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
SOCKET   g_fd  = DSS_ERROR;
unsigned long destinationIP = 0x0UL;
unsigned char SetupConnection = NULL_S ;

unsigned char szTemp_Url[100];
unsigned char szProxy_Url[100];
unsigned char szEmail_Url[100];

#ifdef WAPDORMANT
boolean gb2GHandoff = FALSE;
boolean gbDormant   = FALSE;  
#endif//wapdormant
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void ProcessEvents(void);
extern void UDPc_receivedRequest(const CHAR *data, 
						  UINT16 dataLength, 
						  const CHAR *destination, 
						  UINT8 destLength, 
						  const CHAR *source, 
						  UINT8 sourceLength, 
						  UINT16 destinationPort, 
						  UINT16 sourcePort,
						  UINT8 newChannelId );
extern void wap_abnormal_end_cmd(void);

#ifdef WAPDORMANT
extern void wap_2G_setup_cmd(void);
extern void wap_dormant_end_cmd(void);
extern void wap_connection_done_dormant_cmd( unsigned char objId, 
							 unsigned char chID, 
							 unsigned char b_success);
#endif//wapdormant

/*
**---------------------------------------------------------------------------
*/

void DevProcessEvents(void)
{
	extern void CLNTc_run(void);
	extern BOOL CLNTc_wantsToRun(void);

	while( CLNTc_wantsToRun() )
	{
		CLNTc_run();
	}
}


void wap_enter_cmd(void)
{
    extern const char* UserAgent(void);

	MSG_HIGH("wap_enter_cmd",0,0,0);

	CLNTc_start();
		DevProcessEvents();

	MEMc_initCache(MAX_CACHE_SIZE);
		DevProcessEvents();

    MEMc_initPushRepository(PUSH_STORAGESIZE);
		DevProcessEvents();

	//START WML_USER_AGENT	
	MMIc_startUserAgent(VIEW_ID_WML, WML_USER_AGENT); //(1, 1)
		DevProcessEvents();
	// set for channel
	CLNTc_setIntConfig(VIEW_ID_WML, configDEFAULT_CHANNEL, ChannelID);
		DevProcessEvents();
	CLNTc_setIntConfig(VIEW_ID_WML, configHISTORY_SIZE, 15); //0
		DevProcessEvents();
    // WAP WSP ISO 639 language assignment code : Korean 0x3D
	CLNTc_setStrConfig(VIEW_ID_WML, configWSP_Language, "\x3D", 1);
		DevProcessEvents();
	CLNTc_setIntConfig(VIEW_ID_WML, configCACHE_AGE, INITURL_TIMEOUT); //1HR
		DevProcessEvents();
    CLNTc_setIntConfig(VIEW_ID_WML, configCACHE_MODE, 1); //check if new version available
		DevProcessEvents();
	CLNTc_setIntConfig(VIEW_ID_WML, configDISPLAY_IMAGES, 1);
		DevProcessEvents();
	CLNTc_setIntConfig(VIEW_ID_WML, configUPDATE_IMAGES,  0);
		DevProcessEvents();

	CLNTc_setStrConfig(VIEW_ID_WML, configUSERAGENT, UserAgent(), strlen(UserAgent()));
//	CLNTc_setStrConfig(VIEW_ID_WML, configUSERAGENT, "WAPPER", strlen("WAPPER"));
		DevProcessEvents();
	CLNTc_setIntConfig(VIEW_ID_WML, configPUSH_SECURITY_LEVEL,  0); // all allowed
		DevProcessEvents();
	CLNTc_setDCHIntConfig(VIEW_ID_WML, ChannelID, configACCESS_TYPE, (unsigned long)BEARER_ANY_UDP);
		DevProcessEvents();
	CLNTc_setDCHIntConfig(VIEW_ID_WML, ChannelID, configONLINE, (unsigned long)FALSE); //offline
		DevProcessEvents();
	CLNTc_setDCHIntConfig(VIEW_ID_WML, ChannelID, configCLIENT_LOCAL_PORT, mobilePort);
		DevProcessEvents();
	CLNTc_setDCHStrConfig(VIEW_ID_WML, ChannelID, configUDP_IP_SRC, "\x11\x22\x33\x44", 4);
		DevProcessEvents();
	CLNTc_setDCHStrConfig(VIEW_ID_WML, ChannelID, configUDP_IP_GW,  "\xcb\xec\x2b\x5d", 4);
		DevProcessEvents();
	CLNTc_setDCHStrConfig(VIEW_ID_WML, ChannelID, configSMS_C,  "460703123450", 12);
		DevProcessEvents();
	CLNTc_setDCHStrConfig(VIEW_ID_WML, ChannelID, configSMS_GW, "460703123451", 12);
		DevProcessEvents();
    CLNTc_setDCHIntConfig(VIEW_ID_WML, ChannelID, configTIMEOUT, 40);
		DevProcessEvents();
    CLNTc_setDCHIntConfig(VIEW_ID_WML, ChannelID, configSTACKMODE, MODE_CO_WSP);
		DevProcessEvents();

	//START CONTENT_USER_AGENT
	CLNTc_setDCHIntConfig(CONTENT_USER_AGENT, contentChannel, configACCESS_TYPE, (unsigned long)BEARER_ANY_UDP);
		DevProcessEvents();
    CLNTc_setDCHIntConfig(CONTENT_USER_AGENT, contentChannel, configSTACKMODE, MODE_CO_WSP);
		DevProcessEvents();
	CLNTc_setDCHIntConfig(CONTENT_USER_AGENT, contentChannel, configCLIENT_LOCAL_PORT, mobilePort);
		DevProcessEvents();
	CLNTc_setDCHStrConfig(CONTENT_USER_AGENT, contentChannel, configUDP_IP_SRC, "\x11\x22\x33\x44", 4);
		DevProcessEvents();
	CLNTc_setDCHStrConfig(CONTENT_USER_AGENT, contentChannel, configUDP_IP_GW,  "\xcb\xec\x2b\x5d", 4);
		DevProcessEvents();
	CLNTc_setDCHIntConfig(CONTENT_USER_AGENT, contentChannel, configONLINE, (unsigned long)TRUE); //offline
		DevProcessEvents();

	//REPOSITORY_USER_AGENT

	//PUSH_USER_AGENT

    //Do loading ...
#ifdef FEATURE_SKT_EIF//EIF_WAP
	// ====================================================================
	// yjahn96 01.03.14	EIF WAP(External Interface for WAP) UOPEN command opens URL directly!!
	if (g_nEIFWapstatus == EIF_WAP_UOPEN)
	{
		extern void uopen_command(char*);
		g_nEIFWapstatus = EIF_WAP_NONE;

		CLNTa_log(1, 0, "=== UOPEN function called [%s] ===", EIF_url);

		uopen_command(EIF_url);	
	}
	else
#endif  //#ifdef FEATURE_SKT_EIF
    if(SKY_IS_TO_WAP())
    {
        if(P_REV_IS_6_OR_GREATER)
        {
            WAPUsedTime(START_WAPTIME);
        }
        SKY_SET_TO_WAP(FALSE);
        MMIc_loadURL(VIEW_ID_WML, (const CHAR *)g_szGoToURL,0);
    }
    else if(SKY_IS_FROM_SKVM())
    {
         if(P_REV_IS_6_OR_GREATER)
        {
            WAPUsedTime(START_WAPTIME);
        }
        SKY_SET_FROM_SKVM(FALSE);
        MMIc_loadURL(VIEW_ID_WML, (const CHAR *)g_pszretURL,0);
    }
#ifdef FEATURE_WAP_WAVELET
    else if(SKY_IS_WAVELET_TO_WAP())
    {
		extern char g_wretURL[101];
        if(P_REV_IS_6_OR_GREATER)
        {
            WAPUsedTime(START_WAPTIME);
        }
        SKY_SET_WAVELET_TO_WAP(FALSE);
        MMIc_loadURL(VIEW_ID_WML, (const CHAR *)g_wretURL,0);
    }
#endif //#ifdef FEATURE_WAP_WAVELET
    else if(g_KindofPUSH == INITURL_IS_WAPPUSH)
    {
        g_KindofPUSH = 0;
        MMIc_loadURL(VIEW_ID_WML, (const CHAR *)g_pszPushUrl,0);

    }
    else if(g_KindofPUSH == INITURL_IS_WAPMAIL)
    {
        g_KindofPUSH = 0;
        MMIc_loadURL(VIEW_ID_WML, (const CHAR *)gWAPEmail_Url,0);
    }
    else
    {
	    MMIc_loadURL(VIEW_ID_WML, (const CHAR *)gWAPinit_url,0);
    }
	//MMIc_loadURL(VIEW_ID_WML, "http://wap.n-top.com",0);
		ProcessEvents();

    //WAP_Time_Stmap(TRUE); //위치조정이 필요하지 않을까???

}

void wap_retry_cmd(void)
{
    MMIc_loadURL(VIEW_ID_WML, (const CHAR *)gWAPinit_url,0);
		ProcessEvents();
//	wap_enter_cmd();
}


//When user only presses END key, wap_end_cmd() called
void wap_end_cmd(void)
{
	MSG_HIGH("wap_end_cmd",0,0,0);

#ifdef WAPDORMANT
	if(gb2GHandoff)
	{
		//Already network shut downed, so just issues NET_DISCONNECTED event
		//SetupConnection is False
		MSG_MED("while 2G connection setup, issue end key to exit wap",0,0,0);
		gb2GHandoff = FALSE;
        EQS_SEND_EVENT( SE_NET_DISCONNECTED, EQS_NA, EQS_NA );
		return;
	}
#endif//wapdormant

	switch(SetupConnection)
	{
	case NULL_S:
		/*--------------------------------------------------------------------
		  When a user presses End key when Handset is in dormant mode
		  , PPPCLOSE() should be done.
		---------------------------------------------------------------------*/
		MSG_HIGH("End key when Handset is in dormant mode",0,0,0);
		if(NetworkGetState() == NET_PPPOPEN_S)
		{
			wap_dormant_end_cmd();
			break;
		}else
		{
			ERR("Ilegal state, anyway do exit",0,0,0); 
			//break; through down
		}

	case SETUPFAIL_S:
		MSG_HIGH("SETUPFAIL_S do exit w/o CLNTc_setupConnectionDone()",0,0,0);
		wap_abnormal_end_cmd();
		break;

	case SETUP_S:
		/*--------------------------------------------------------------------
		  When a user presses End key as soon as he presses nate button
		  CLNTc_closeConnection() doesn't work until CLNTc_setupConnectionDone()
		  informed of Core
        ---------------------------------------------------------------------*/
		MSG_HIGH("SETUP_S",0,0,0);
		CLNTc_setupConnectionDone(VIEW_ID_WML, ChannelID, TRUE);
		//break; through down

	case SETUPDONE_S:
		MSG_HIGH("SETUPDONE_S",0,0,0);	
        CLNTc_closeConnection(VIEW_ID_WML,ChannelID,FALSE);    
		    ProcessEvents();
		break;
	}

	CLNTc_setDCHIntConfig(VIEW_ID_WML, ChannelID, configONLINE, (unsigned long)FALSE); //offline
		ProcessEvents();	

	SetupConnection = NULL_S;
}

void wap_abnormal_end_cmd(void)
{
	extern void CLNTa_closeConnection(UINT8 objectId, UINT8 channelID);

	MSG_HIGH("wap_abnormal_end_cmd",0,0,0);

	CLNTa_closeConnection(VIEW_ID_WML,ChannelID);
		ProcessEvents();
}

void wap_exit_cmd(void)
{
	MSG_HIGH("wap_exit_cmd",0,0,0);

	MEMc_prepareCache(MAX_CACHE_SIZE);
		ProcessEvents();
}


void CLNTa_setupConnection(UINT8 objectId, UINT8 channelID)
{
	int result = DSS_ERROR;

	MSG_HIGH("CLNTa_setupConnection objID[%d] chID[%d]",objectId,channelID,0);

	//decide to call CLNTc_closeConnection()
	//to avoid a redundant calling wihout CLNTa_setupConneciton()
	SetupConnection = SETUP_S;

#ifdef WAPDORMANT	
	if(NetworkGetState() == NET_PPPOPEN_S)
	{
		if(gbDormant)
		{
			if(P_REV_IS_6_OR_GREATER /*&& test_flag26 == 0 2G handoff test*/)
			{
				MSG_HIGH("Dormant state , so skip setupConnection",0,0,0);
				gbDormant = FALSE;
				wap_connection_done_dormant_cmd(1,1, TRUE);
				return;
			}else
			{
				//Dormant state but 2G handoff, so setup with PPP negotiattion
				MSG_HIGH("dormant state but 2G handoff",0,0,0);
				gbDormant = FALSE;
				gb2GHandoff = TRUE;
				wap_dormant_end_cmd();
				return;
			}
		}
	}
#endif //wapdormant

	result = PPPOPEN();
	if(result == DSS_SUCCESS)
	{
		MSG_MED("PPPOPEN already done",0,0,0);
        EQS_SEND_EVENT( SE_NET_CONNECTED, EQS_NA, EQS_NA );
	}
}

void CLNTa_requestConnection(UINT8 objectId)
{
	MSG_HIGH("CLNTa_requestConnection objID[%d]",objectId,0,0);
}


void CLNTa_closeConnection(UINT8 objectId, UINT8 channelID)
{
	int result = DSS_ERROR;
	extern BOOL g_bDrawBar;

	MSG_HIGH("CLNTa_closeConnection objID[%d] chID[%d]",objectId,objectId,0);

	//bar draw stop
	g_bDrawBar = FALSE;	
	WAP_Repaint();

	SetupConnection = NULL_S;

#ifdef WAPDORMANT

	if( gbDormant && (NetworkGetState() == NET_PPPOPEN_S) )
	{
		MSG_HIGH("SKIP dormant state close operation",0,0,0);
		return;
	}
#endif//wapdormant

	closesocket(g_fd);
	g_fd = DSS_ERROR;

#ifdef FEATURE_WAP_WAVELET
    if (P_REV_IS_6_OR_GREATER && 
		(SKY_IS_TO_SKVM() || SKY_IS_TO_GVM() || 
		SKY_IS_WAP_TO_WAVELET()) )
#else
    if (P_REV_IS_6_OR_GREATER && 
		( SKY_IS_TO_SKVM() 
		|| SKY_IS_TO_GVM()) )
#endif //#ifdef FEATURE_WAP_WAVELET
	{
		MSG_HIGH("skip PPPCLOSE",0,0,0);
        EQS_SEND_EVENT(SE_NET_DISCONNECTED, EQS_NA, EQS_NA); 
		return;
	}
	result = PPPCLOSE();	
	if(result == DSS_SUCCESS)
	{
		MSG_MED("PPPCLOSE already done",0,0,0);
        EQS_SEND_EVENT( SE_NET_DISCONNECTED, EQS_NA, EQS_NA );
	}
}


void wap_connection_done_cmd( unsigned char objId, 
							 unsigned char chID, 
							 unsigned char b_success)
{
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr  ;
	int      result = DSS_ERROR; 

	MSG_HIGH("wap_connection_done_cmd success[%d]",b_success,0,0);

	if(b_success)
	{
		//hostentry = gethostbyname("wgw.n-top.com");
        hostentry = gethostbyname((const char*)gWAPproxy_url);
        
        
		if(hostentry == NULL)
		{
			MSG_ERROR("DNS query fail",0,0,0);
			b_success = FALSE;
		}else
		{
			g_fd = socket( AF_INET,
						 SOCK_DGRAM,
						 IPPROTO_UDP);

			memset((char *)&addr, 0, sizeof(struct sockaddr_in));

			addr.sin_family      = AF_INET;        
			addr.sin_port        = dss_htons(mobilePort);
			addr.sin_addr.s_addr = 0; 

			result = bind(g_fd, (struct sockaddr *) &addr, sizeof(addr) );			
			MSG_ERROR("bind %d %d", g_fd,result,0); 

			destinationIP = get32((byte *)(hostentry->h_addr_list[0]));        

			CLNTc_setDCHIntConfig(objId, chID, configONLINE, (unsigned long)TRUE);
				ProcessEvents();	
		}//hostentry
	}

	CLNTc_setupConnectionDone(objId, chID, b_success);
		ProcessEvents();

	if(b_success)
		SetupConnection = SETUPDONE_S;
	else
		SetupConnection = SETUPFAIL_S;

}


#ifdef WAPDORMANT

void wap_connection_done_dormant_cmd( unsigned char objId, 
							 unsigned char chID, 
							 unsigned char b_success)
{
	int result = DSS_ERROR;

	MSG_HIGH("wap_connection_done_dormant_cmd success[%d]",b_success,0,0);

	if(b_success)
	{
		if(g_fd == DSS_ERROR)
		{
			g_fd = socket( AF_INET,
						 SOCK_DGRAM,
						 IPPROTO_UDP);

			memset((char *)&addr, 0, sizeof(struct sockaddr_in));

			addr.sin_family      = AF_INET;        
			addr.sin_port        = dss_htons(mobilePort);
			addr.sin_addr.s_addr = 0; 

			result = bind(g_fd, (struct sockaddr *) &addr, sizeof(addr) );			
			MSG_ERROR("bind %d %d", g_fd,result,0);      
		}

		CLNTc_setDCHIntConfig(objId, chID, configONLINE, (unsigned long)TRUE);
			ProcessEvents();	
	}

	CLNTc_setupConnectionDone(objId, chID, b_success);
		ProcessEvents();

	if(b_success)
		SetupConnection = SETUPDONE_S;
	else
		SetupConnection = SETUPFAIL_S;
}

void wap_2G_setup_connection_cmd(void)
{
	int result = DSS_ERROR;

	MSG_MED("wap_2G_setup_connection_cmd",0,0,0);

	gbDormant   = FALSE;
	gb2GHandoff = FALSE;

	result = PPPOPEN();
	if(result == DSS_SUCCESS)
	{
		MSG_MED("PPPOPEN already done",0,0,0);
        EQS_SEND_EVENT( SE_NET_CONNECTED, EQS_NA, EQS_NA );
	}
	//SetupConnection not need because bearer call this function
}

void wap_dormant_end_cmd(void)
{
	int result = DSS_ERROR;

	MSG_MED("wap_dormant_end_cmd",0,0,0);

	closesocket(g_fd);

	result = PPPCLOSE();	
	if(result == DSS_SUCCESS)
	{
		MSG_MED("PPPCLOSE already done",0,0,0); 
        EQS_SEND_EVENT( SE_NET_DISCONNECTED, EQS_NA, EQS_NA );
	}
	//SetupConnection not need because applet exit
//	SKY_ERROR_TONE(); 
}

#endif//wapdormant


#ifdef FEATURE_SKT_EIF
BOOL Eif_dnsquery(void)
{
    struct   hostent *hostentry = NULL ;
	int loop;
    hostentry = gethostbyname((const char*)EIF_url);    

    if(hostentry == NULL)
	{
		MSG_ERROR("UPING: DNS query fail",0,0,0);        

		/*
			DNS query fail시 secondary IP로 retry하는 루틴 필요함....
		*/
        //EQS_SEND_EVENT(SE_WAP_REDIRECTFAIL, EQS_NA, EQS_NA);
        // fail시 event발생 !!!
		return FALSE;
	}
    else
    {
        Dest_url_IP = get32((byte *)(hostentry->h_addr_list[0]));  

		for (loop=0 ; loop<4 ; loop++)
		{
			Dest_IP[4 - loop -1] = ((char*)&Dest_url_IP)[loop];
		}
		
		return TRUE;
    }
	
}

void EIF_wapcon_connection(void)
{	
	extern char EIF_proxyurl[192];
	extern void eif_error_command(int Kind_of_error);
	extern BOOL g_EIF_WAP_WAPCON;

	struct   hostent *hostentry = NULL ;
	ip4a eif_IP = 0;
   
	hostentry = gethostbyname((const char*)EIF_proxyurl);    

   if(hostentry == NULL)
   {
		MSG_ERROR("DNS query fail",0,0,0);    
		/*
			DNS query fail시 secondary IP로 retry하는 루틴 필요함....
		*/		
		//EQS_SEND_EVENT(SE_WAP_REDIRECTFAIL, EQS_NA, EQS_NA);
		// fail시 event발생 !!!

		CLNTa_log(1,0, "WAPCON: DNA query fail");
		eif_error_command(EIF_ERR_WAPCON_CONNECT_FAIL);
	}
	else
	{
		eif_IP = get32((byte *)(hostentry->h_addr_list[0]));        

		g_EIF_WAP_WAPCON = TRUE;	//wapcon에대해서 loadUrl을 성공했을때, 해당 사이트의 카드를 show할 때 성공메시지 출력 위해

		CLNTa_log(1,0, "WAPCON: MMIa_loadURL()함...");
		MMIc_loadURL(VIEW_ID_WML, (const CHAR *)EIF_url,0);
		ProcessEvents();
	}
}
#endif	//#ifdef FEATURE_SKT_EIF

#ifdef FEATURE_WAP_REDIRECT
void Redirection_dnsquery(void)
{
    struct   hostent *hostentry = NULL ;

    hostentry = gethostbyname((const char*)gWAPproxy_url);    

    if(hostentry == NULL)
	{
		MSG_ERROR("DNS query fail",0,0,0);        
        EQS_SEND_EVENT(SE_WAP_REDIRECTFAIL, EQS_NA, EQS_NA);
        // fail시 event발생 !!!
	}
    else
    {
        destinationIP = get32((byte *)(hostentry->h_addr_list[0]));        
        if(g_pCard == NULL)
        {
            MMIc_loadURL(VIEW_ID_WML, (const CHAR *)gWAPinit_url,0);
		       ProcessEvents();
        }
        else
        {
            MMIc_reload(VIEW_ID_WML);
		       ProcessEvents();
        }
        EQS_END();
        displayloading(12);
        //EQS_START_SIBLING_EVENT( SKA_WAPHANDLE_KEY, SE_APP_RESUME, EQS_NA, EQS_NA, FALSE);
    }
	
}
#endif //#ifdef FEATURE_WAP_REDIRECT

///////////////////////////////////////////////////////////////////////////////////////
void WAP_SocketStateChanged(void)
{
	extern void DevRecvPDU(void);
	int event;

	MSG_MED("WAP_SocketStateChanged",0,0,0);

	event = CheckSocketState(g_fd);
	if (event == 1)
	{
		DevRecvPDU();
		EnableReadEvent(g_fd);
	}
	else if (event == 3)
	{
		MSG_MED("Socket closed",0,0,0)	;
	}else
	{
		MSG_ERROR("No event",0,0,0);
	}
}

#ifdef WAP_TEST
enum {
	Invoke = 0x08,
	Result = 0x10,
	Ack    = 0x18,
	Abort  = 0x20,
	SegInvoke = 0x28,
	SegResult = 0x30,
	NakAck = 0x38
};

void PDUType(unsigned char pdu)
{
	if( (pdu & 0x80)  )
	{
		MSG_ERROR("[!!!! CONTINUE flag]",0,0,0);
	}
	if( (pdu & 0x38) == Invoke )
	{
		MSG_ERROR("[INVOKE]",0,0,0);
	}else if( (pdu & 0x38) == Result )
	{
		MSG_ERROR("[RESULT]",0,0,0);
	}else if( (pdu & 0x38) == Ack )
	{
		MSG_ERROR("[ACK]",0,0,0);
	}else if( (pdu & 0x38) == Abort )
	{
		MSG_ERROR("[ABORT]",0,0,0);
	}else if( (pdu & 0x38) == SegInvoke )
	{
		MSG_ERROR("[SEG INVOKE]",0,0,0);
	}else if( (pdu & 0x38) == SegResult )
	{
		MSG_ERROR("[SEG RESULT]",0,0,0);
	}else if( (pdu & 0x38) == NakAck )
	{
		MSG_ERROR("[NAK ACK]",0,0,0);
	}else
	{
		MSG_ERROR("[Invalid %x]", (pdu&0x38),0,0);
	}
}
#endif//WAP_TEST


void UDPa_sendRequest
(
	const CHAR *data, 
	UINT16     dataLength, 
	const CHAR *destination, 
	UINT8      destLength, 
	const CHAR *source, 
	UINT8	   sourceLength, 
	UINT16	   destinationPort, 
	UINT16	   sourcePort, 
	UINT8      objectId, 
	UINT8      channelId
)
{
	sint15 bytes_per_write = 0;
    sint15 payload_len = 0;

	MSG_HIGH("_",0,0,0);
	MSG_HIGH(")))))))))))))))UDPa_sendRequest",0,0,0); 

#ifdef WAPTEST
    MSG_HIGH("send tid1 :%x tid2 :%x type:%x",data[1],data[2],data[0]);  
	MSG_HIGH("send 4th :%x 5th :%x", data[3],data[4],0);
	MSG_MED("objectId [%d] channelId [%d]",objectId,channelId,0);

	PDUType(data[0]);
#endif//waptest

	memset((struct sockaddr_in*)&addr, '\0', sizeof(struct sockaddr_in));

//    MSG_MED("destinationIP[0x%x%x] destPort[%d]", (int)((destinationIP)>>16), 
//								(int)(destinationIP&0xFFFF), destinationPort);    
    
	if(g_fd == DSS_ERROR)
	{
		MSG_ERROR("Socket descriptor is invalid",0,0,0);
		return;
	}
	addr.sin_family      = AF_INET;                   //0
	addr.sin_addr.s_addr = dss_htonl(destinationIP);  //0xcbec2b5d or 0x460cca60
	addr.sin_port        = dss_htons(destinationPort);//9201

  /*-------------------------------------------------------------------------
    The incoming Browser packet needs to be split up into one or more large 
    items.Check the length of the packet to make sure and then take care of 
    the splitting in the while loop.
   ------------------------------------------------------------------------*/
	payload_len = MIN( dataLength, 2000);

    /*-----------------------------------------------------------------------
       UDP packet is now ready - process it according to psi_state 
    -----------------------------------------------------------------------*/   
	if( payload_len != 0)
	{

		bytes_per_write = sendto(g_fd,
								(const char*)data,
								payload_len,
								0,
								(struct sockaddr*) &addr, 
								 sizeof(struct sockaddr) );
	}

	if( bytes_per_write < 0 ) 
	{
		MSG_ERROR("UDPa_sendRequest error[%d]", bytes_per_write, 0, 0);
	} else {
		MSG_MED("UDPa_sendRequest sendbytes[%d]", bytes_per_write,0,0);
	}
}


void DevRecvPDU(void)
{
	int             length    = 0;
	unsigned short    size    = 0;
	unsigned long    srcIP    = 0x0;
	unsigned short   srcPort  = 0;
	unsigned long    destIP   = 0x0UL;
	unsigned short   destPort = 0;
	struct sockaddr_in addr   ; 
	unsigned char*   m_RXBUF  = NULL;
	int              newChannelId1 = 0;

	MSG_HIGH("_",0,0,0);
	MSG_HIGH("DevRecvPDU((((((((((((",0,0,0);

	memset((struct sockaddr*)&addr, '\0', sizeof(struct sockaddr));
	size = sizeof(struct sockaddr);

	m_RXBUF = SeekWorkingArea(4);
	length = recvfrom(   g_fd, 
						 (char*) m_RXBUF, 
						 (int) MAX_RXBUF_SIZE,     //buf length
						 (int) 0,                  //unsuppot flag 
						 (struct sockaddr*) &addr, //fromaddr
						 (int*)&size); 
	if(length == 0)
	{
		MSG_ERROR("No data received [%d]",length,0,0);
	}

	MSG_MED("Now receive datagrams [%d] \n",length,0,0);

#ifdef WAPTEST
    MSG_HIGH("recv tid1 :%x tid2 :%x type:%x",m_RXBUF[1],m_RXBUF[2],m_RXBUF[0]);  
	MSG_HIGH("recv 4th :%x 5th :%x", m_RXBUF[3],m_RXBUF[4],0);
	
	PDUType(m_RXBUF[0]);
#endif//waptest

	srcIP   = htonl(addr.sin_addr.s_addr);
	srcPort = htons(addr.sin_port);

//    MSG_MED("Server IP[0x%x%x] Port[%d]",(int)((srcIP)>>16),
//								(int)(srcIP&0xFFFF), srcPort);
    
    destPort = mobilePort;
    destIP   = mobileIP;

    UDPc_receivedRequest(
							(const CHAR*) m_RXBUF, 
							(UINT16) length, 
							(const CHAR*) &destIP, 
							(UINT8) size, 
							(const CHAR*) &srcIP, 
							(UINT8) size, 
							(UINT16) destPort, 
							(UINT16) srcPort,
							newChannelId1 ) ;
	ProcessEvents();

}

///////////////////////////////////////////////////////////////////////////////////////

boolean wap_dormant_state(void)
{
	if(NetworkGetState() == NET_PPPOPEN_S)
		return TRUE;
	else
		return FALSE;
}
/*===========================================================================

FUNCTION wap_check_and_run()

DESCRIPTION
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void wap_check_and_run(void)
{
	if( CLNTc_wantsToRun() ) {
		CLNTc_run();
	}
}

#endif//feature_wap
