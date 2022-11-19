#include <stdio.h>
#include <stdlib.h>
#include "ska.h"
#include "dsm.h"
#include "sky_sock.h"
#include "msg.h"
#include "err.h"
#include "dssocket.h"
#include "psmisc.h"

#include "wav_multisock.h"

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
///////////////////////////// TCP와 UDP Socket의 개수를 지정///////////////////////////
enum {
	max_tcp_num = 1,							// 최대 TCP Socket 개수(현제 1개)
	max_udp_num = 2,							// 최대 UDP Socket 개수(현재 2개)
	max_socket_num = max_tcp_num + max_udp_num	// 최대 사용되는 Socket 개수(현재 3개)
};
///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////// Socket Descriptor Number ////////////////////////////////////////
#define FD_TCP		0							// For TCP Socket Descriptor
#define	FD_UDP1		1							// For UDP1 Socket Descriptor
#define	FD_UDP2		2							// For UDP2 Socket Descriptor
///////////////////////////////////////////////////////////////////////////////////////


//////////////////////////// Message //////////////////////////////////////////////////
// ska_wav.c에서 사용하는 메시지를 사용하기 위해 재정의  
enum tagWAVELET_MESSAGE_E{
    WAV_MES_1 = 1,         
    WAV_MES_2,            
    WAV_MES_3,            
    WAV_MES_4,          
    WAV_MES_5,
    WAV_MES_6,
    WAV_MES_7
};
///////////////////////////////////////////////////////////////////////////////////////

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

SOCKET sock_fd_wav[max_socket_num] = {-1,};		// Socket Descriptor 변수
extern HCONTROL g_hWavControl;					// ska_wav.c에 정의된 메시지 control

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

static	struct sockaddr_in		g_UDP1HostAddr;	
static	uint16					g_sizeofUDP1HostAddr;
static	struct sockaddr_in		g_UDP2HostAddr;	
static	uint16					g_sizeofUDP2HostAddr;

extern BOOL	fDownload; 
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
#define GET_FD	( gFD )
#define SET_FD(s)   	( gFD = s )

int MM_PPPOpen(void)
{
	sint15 result = DSS_ERROR ;

	MSG_ERROR("Wavelet Pppopen ",0,0,0);

	SetKindOfService(WAVELET);
	result = PPPOPEN();
	if(result == DSS_ERROR)
	{
		if(NetworkGetState() == NET_PPPOPENING_S){
			MSG_ERROR("ppp_open wouldblock! ", 0,0,0);	
			return WAV_SOCK_WOULDBLOCK;
		}
		else
		{
			MSG_ERROR("ppp_open error occurred! ",0,0,0);	
			return WAV_SOCK_ERROR;
		}        
	}
	MSG_ERROR("Wavelet pppopen 완료",0,0,0);
	return WAV_SOCK_SUCCESS;
}

int MM_PPPClose(void)
{
	sint15 result = DSS_ERROR ;
	result = PPPCLOSE();
	if(result == DSS_ERROR)
	{
		if(NetworkGetState() == NET_CLOSING_S){
			MSG_ERROR("PPP CLOSE wouldblock! ", 0,0,0);	
			return WAV_SOCK_WOULDBLOCK;
		}
		else
		{
			MSG_HIGH("PPP CLOSE fail",0,0,0);
			return WAV_SOCK_ERROR;
		}        
	}
	MSG_ERROR("Wavelet pppclose 완료",0,0,0);
	return WAV_SOCK_SUCCESS;
}

void MM_InitSocket(void)
{
	int i;
	for( i = 0 ; i < max_socket_num; i++) 
	{
		sock_fd_wav[i] = DSS_ERROR; 
	}
}

int MM_OpenSocket(void) 
{
	sock_fd_wav[FD_TCP] = socket((byte)AF_INET, 
						(byte)SOCK_STREAM, 
						(byte)IPPROTO_TCP);
	if( sock_fd_wav[FD_TCP] == DSS_ERROR ) 
	{
		MSG_ERROR("fail to open socket [%x]", sock_fd_wav[FD_TCP], 0, 0);		
		return WAV_SOCK_ERROR;
	}	
	sock_fd_wav[FD_UDP1] = socket((byte)AF_INET, 
						(byte)SOCK_DGRAM, 
						(byte)IPPROTO_UDP);
	if( sock_fd_wav[FD_UDP1] == DSS_ERROR ) 
	{
		MSG_ERROR("fail to open socket [%x]", sock_fd_wav[FD_UDP1], 0, 0);		
		return WAV_SOCK_ERROR;
	}	

	sock_fd_wav[FD_UDP2] = socket((byte)AF_INET, 
						(byte)SOCK_DGRAM, 
						(byte)IPPROTO_UDP);
	if( sock_fd_wav[FD_UDP2] == DSS_ERROR ) 
	{
		MSG_ERROR("fail to open socket [%x]", sock_fd_wav[FD_UDP2], 0, 0);		
		return WAV_SOCK_ERROR;
	}	

	MSG_ERROR("Success to open socket %x,%x,%x" , sock_fd_wav[FD_TCP], sock_fd_wav[FD_UDP1], sock_fd_wav[FD_UDP2]);
	return WAV_SOCK_SUCCESS;
}

int MM_ReleaseSocket(void)
{
	sint15 result = DSS_ERROR;
//	int result = -1;
	int i;

	MSG_ERROR("Wavelet Socket release",0,0,0);

	DisableReadEvent(sock_fd_wav[FD_TCP]);
	DisableWriteEvent(sock_fd_wav[FD_TCP]);
	DisableReadEvent(sock_fd_wav[FD_UDP1]);
	DisableWriteEvent(sock_fd_wav[FD_UDP1]);
	DisableReadEvent(sock_fd_wav[FD_UDP2]);
	DisableWriteEvent(sock_fd_wav[FD_UDP2]);

	for( i = 0; i < max_socket_num ; i++ )
	{

		if(sock_fd_wav[i] == DSS_ERROR)
			continue;

		result = closesocket(sock_fd_wav[i]);
		if(result == DSS_ERROR)
		{
			MSG_ERROR("Wavelet close socket fail %d ",sock_fd_wav[i],0,0);
//			result = closesocket(sock_fd_wav[i]);
			if(result == DSS_ERROR)
			{
				MSG_ERROR("Wavelet close socket fail %x ",sock_fd_wav[i],0,0);
				return WAV_SOCK_ERROR;			
			}
		}
	}
	MSG_ERROR("Wavelet close TCP(1), UDP(2) sockets Success",0,0,0);
	MM_InitSocket();
	return WAV_SOCK_SUCCESS;
}

int MM_Connect(char* ip,  WORD port)
{
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr,t_addr;
	int result;

	hostentry = gethostbyname(ip);
	if (hostentry == NULL)
	{
		MSG_ERROR("gethostbyname error",0,0,0);
		return WAV_SOCK_ERROR;
	}

/////////////////////////////////// UDP Host Addr Setting //////////////////////////////////////
///////////////////////////////////////////// UDP1 ///////////////////////////////////////////
		memset((char *)&g_UDP1HostAddr, 0, sizeof(struct sockaddr_in));

		g_UDP1HostAddr.sin_family      = AF_INET;           
		g_UDP1HostAddr.sin_port        = dss_htons(UDP_PORT1);

		g_UDP1HostAddr.sin_addr.s_addr = get32( (unsigned char *)(hostentry->h_addr_list[0]) );
		g_UDP1HostAddr.sin_addr.s_addr = dss_htonl(g_UDP1HostAddr.sin_addr.s_addr); 

		g_sizeofUDP1HostAddr = sizeof(g_UDP1HostAddr);

		memset((char *)&t_addr, 0, sizeof(struct sockaddr_in));

		t_addr.sin_family      = AF_INET;        
		t_addr.sin_port        = dss_htons(UDP_PORT1);
		t_addr.sin_addr.s_addr = 0; 

		result = bind(sock_fd_wav[FD_UDP1], (struct sockaddr *) &t_addr, sizeof(t_addr) );
		MSG_HIGH("Bind UDP1 %d",result,0,0);

///////////////////////////////////////////// UDP2 ///////////////////////////////////////////
		memset((char *)&g_UDP2HostAddr, 0, sizeof(struct sockaddr_in));

		g_UDP2HostAddr.sin_family      = AF_INET;           
		g_UDP2HostAddr.sin_port        = dss_htons(UDP_PORT2);

		g_UDP2HostAddr.sin_addr.s_addr = get32( (unsigned char *)(hostentry->h_addr_list[0]) );
		g_UDP2HostAddr.sin_addr.s_addr = dss_htonl(g_UDP2HostAddr.sin_addr.s_addr); 

		g_sizeofUDP2HostAddr = sizeof(g_UDP2HostAddr);

		memset((char *)&t_addr, 0, sizeof(struct sockaddr_in));

		t_addr.sin_family      = AF_INET;        
		t_addr.sin_port        = dss_htons(UDP_PORT2);
		t_addr.sin_addr.s_addr = 0; 

		result = bind(sock_fd_wav[FD_UDP2], (struct sockaddr *) &t_addr, sizeof(t_addr) );
		MSG_HIGH("Bind UDP2 %d",result,0,0);

////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// TCP Host Addr Setting //////////////////////////////////////
	memset((char *)&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family      = AF_INET;           
	addr.sin_port        = dss_htons(port);

	addr.sin_addr.s_addr = get32( (unsigned char *)(hostentry->h_addr_list[0]) );

	MSG_HIGH("REMOTE ADDRESS [0x%x%x]", (int)((addr.sin_addr.s_addr)>>16), 
									    (int)addr.sin_addr.s_addr, 0);

	addr.sin_addr.s_addr = dss_htonl(addr.sin_addr.s_addr); 

	memset((char *)&t_addr, 0, sizeof(struct sockaddr_in));

	t_addr.sin_family      = AF_INET;        
	t_addr.sin_port        = dss_htons(2000);
	t_addr.sin_addr.s_addr = 0; 

	result = bind(sock_fd_wav[FD_TCP], (struct sockaddr *) &t_addr, sizeof(t_addr) );
	MSG_HIGH("Bind TCP %d",result,0,0);

////////////////////////////////////////////////////////////////////////////////////////////////

	if(connect( sock_fd_wav[FD_TCP], &addr, sizeof(struct sockaddr)) == DSS_ERROR)
	{
		MSG_ERROR("TCP connection fail",0,0,0);
		return WAV_SOCK_ERROR;
	}
	MSG_ERROR("TCP connection Success!",0,0,0);
	return WAV_SOCK_SUCCESS;
}

void WAV_SocketStateChanged(void)
{
	int event;
	int i;

	MSG_ERROR("WAV_SocketStateChanged",0,0,0);

//  Should be changed for Application to call CheckSocketState fn.
	for(i = 0; i < max_socket_num; i++)
	{
		if (sock_fd_wav[i] == DSS_ERROR)
		continue;
		event = CheckSocketState(sock_fd_wav[i]);
		if (event == 1) // receive
		{
			MSG_ERROR("data received!!! <- WAV_SocketStateChanged %x", sock_fd_wav[i], 0, 0);
			if (i == FD_UDP1)
			{
				MSG_HIGH("FD_UDP1 Received",0,0,0);
				MM_DecodeTCM1();
				EnableReadEvent(sock_fd_wav[i]);
			}
			else if (i == FD_UDP2)
			{
				MSG_HIGH("FD_UDP2 Received",0,0,0);
				MM_DecodeTCM2();
				EnableReadEvent(sock_fd_wav[i]);
			}
			else if (i == FD_TCP)
			{
				MSG_HIGH("FD_TCP Received",0,0,0);				
				MM_ProcessTCM2ServerCommand();
			}
		}
		else if (event == 3)
		{
			MSG_ERROR("TCP Close!", 0, 0, 0);		
			MM_ProcessTCM2UICommand(/*TCM_UC_STOP*/0x21, 0, 0);
			sReleaseController();
			g_hWavControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_WAVELET_CONTROLLER_ERROR), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(g_hWavControl,WAV_MES_3);
            EQC_SET_STYLE(g_hWavControl, EQC_GET_STYLE(g_hWavControl) |CS_MESSAGE_BOX_IGNORE_KEYS);

		}
	}
}
void	MM_SendUDPData1(BYTE *pData, WORD nSize)  // yoursun 01/07/27 
{
	int length 	= 0;

	length = sendto( sock_fd_wav[FD_UDP1] , 
			 (char*) pData, 
			 (int) nSize, 
			 (int) 0,	//unsuppot flag  
			 (struct sockaddr *)&g_UDP1HostAddr, &g_sizeofUDP1HostAddr);                

	if (length == (int)nSize)
	{
		MSG_ERROR("Send UDP1 Data %x:[%d][%d] \n", sock_fd_wav[FD_UDP1], nSize, length);
	}
	else
	{
		MSG_ERROR("Send UDP1 Data Error ",0,0,0);
	}

}
/*==========================================================================================*/
WORD	MM_RecvUDPData1(BYTE *pData, WORD nSize)  // yoursun 01/07/27 
{
	int             length    = 0;
	length = recvfrom(sock_fd_wav[FD_UDP1] , 
			 (char*) pData, 
			 (int) nSize, 
			 (int) 0,                //unsuppot flag  
			 (struct sockaddr *)&g_UDP1HostAddr, &g_sizeofUDP1HostAddr);

	MSG_ERROR("Receive UDP1 Data %x:[%d][%d] \n",sock_fd_wav[FD_UDP1], nSize, length);
	return (WORD)length;
}
/*==========================================================================================*/
void	MM_SendUDPData2(BYTE *pData, WORD nSize)  // yoursun 01/07/27 
{
	int length 	= 0;

	length = sendto( sock_fd_wav[FD_UDP2] , 
			 (char*) pData, 
			 (int) nSize, 
			 (int) 0,	//unsuppot flag  
			 (struct sockaddr *)&g_UDP2HostAddr, &g_sizeofUDP2HostAddr);                

	if (length == (int)nSize)
	{
		MSG_ERROR("Send UDP2 Data %x:[%d][%d] \n", sock_fd_wav[FD_UDP2], nSize, length);
	}
	else
	{
		MSG_ERROR("Send UDP2 Data Error ",0,0,0);
	}
}
/*==========================================================================================*/
WORD	MM_RecvUDPData2(BYTE *pData, WORD nSize)  // yoursun 01/07/27 
{
	int             length    = 0;
	length = recvfrom(sock_fd_wav[FD_UDP2] , 
			 (char*) pData, 
			 (int) nSize, 
			 (int) 0,	//unsuppot flag  
			 (struct sockaddr *)&g_UDP2HostAddr, &g_sizeofUDP2HostAddr);                

	MSG_ERROR("Receive UDP2 Data %x:[%d][%d] \n",sock_fd_wav[FD_UDP2], nSize, length);
	return (WORD)length;
}
/*==========================================================================================*/
void	MM_SendTCPData(BYTE *pData, WORD nSize) // yoursun 01/07/27  
{
	int length 	= 0;

	length = send( sock_fd_wav[FD_TCP] , 
			 (char*) pData, 
			 (int) nSize, 
			 (int) 0);                //unsuppot flag  

	if (fDownload == FALSE)
	{
		EnableReadEvent(sock_fd_wav[FD_UDP1]);
		EnableReadEvent(sock_fd_wav[FD_UDP2]);
	}

	if (length == (int)nSize)
	{
		MSG_ERROR("Send TCP Data %x:[%d][%d] \n", sock_fd_wav[FD_TCP], nSize, length);
	}
	else
	{
		MSG_ERROR("Send TCP Data Error ",0,0,0);
	}
}
/*==========================================================================================*/
WORD	MM_RecvTCPData(BYTE *pData, WORD nSize) // yoursun 01/07/27 
{
	int             length    = 0;
	length = recv(sock_fd_wav[FD_TCP] , 
			 (char*) pData, 
			 (int) nSize, 
			 (int) 0);                //unsuppot flag  

	if (length <= 0)
	{
		MSG_ERROR("Receive TCP Data Error ",0,0,0);
		MM_ProcessTCM2UICommand(/*TCM_UC_TCP_FAIL*/0x21, 0, 0);
		return 0;
	}
	else
	{
		MSG_ERROR("Receive TCP Data %x:[%d][%d] \n", sock_fd_wav[FD_TCP], nSize, length);
		EnableReadEvent(sock_fd_wav[FD_TCP]);
		return (WORD)length;
	}
}