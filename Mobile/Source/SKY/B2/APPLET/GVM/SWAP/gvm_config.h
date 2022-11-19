#ifndef	_GVM_HEADER_H_
#define	_GVM_HEADER_H_

/*
******************************************************************************
******************************************************************************
**
**
**   GVM 정보 설정
**
**
******************************************************************************
*****************************************************************************
*/
/*===========================================================================
**  GVM 정보
*/
#ifdef FEATURE_GVM


/*===========================================================================
**  Include
*/
#include "SWAP.h"
#include "sky_file.h"


/*===========================================================================
**  응용프로그램 정보 - GVM
*/

#define PPP_TIMEOUT                 (20*1000)

// Script Type
enum tagGVM_SCRIPT_TYPE_E{
    GST_NONE,           // ScriptType starts from 1
    GST_ALONE,          // Stand-alone
    GST_PTP,            // Phone-to-phone
    GST_SERVER,         // TCP/IP
    GST_FREE,           // Connect by stand-alone & free
    GST_ALONE_PTP,      // Stand-alone + Phone-to-phone
    GST_ALONE_SERVER,   // Stand-alone + TCP/IP
    GST_PTP_SERVER,     // Phone-to-phone + TCP/IP
    GST_ALL,            // Stand-alone + Phone-to-phone + TCP/IP
    GST_MAX
};
typedef enum tagGVM_SCRIPT_TYPE_E GVM_SCRIPT_TYPE_E;

enum tagGVM_DISCONNECT_TYPE_E {
    DT_NONE,
    DT_DISCONNECT,
    DT_CALLDROP,
    DT_TOWAP,
    DT_NORESULT,
    DT_WAIT
};
typedef enum tagGVM_DISCONNECT_TYPE_E GVM_DISCONNECT_TYPE_E;

// script info
struct tagGVM_SCRIPT_INFO_T
{
    BYTE    fSSB;
    SSBInfo ssbInfo;	
	BYTE    szUserData[SWAP_USERNV_SIZE+1];
};
typedef struct tagGVM_SCRIPT_INFO_T GVM_SCRIPT_INFO_T;


//
//  Chat Text
//
struct tagGVM_CHATTEXT_T
{
    CHAR    svRsvdChatText[GVM_LEN_CHATTEXT][SWAP_RSVD_CHAT_SIZE+1]; //SWAP_LEN_CHAT_TEXT+1]; //SWAP_RSVD_CHAT_SIZE];
};
typedef struct tagGVM_CHATTEXT_T GVM_CHATTEXT_T;

// ptp dial
struct tagGVM_PTP_DIAL_T 
{
    CHAR    szPTPDial[SWAP_LEN_DIAL+1];
};
typedef struct tagGVM_PTP_DIAL_T    GVM_PTP_DIAL_T;

// config
struct tagGVM_CONFIG_T 
{
    SKY_FILE_HEADER_T   Header;             /* 파일 헤더 */
    GVM_SCRIPT_INFO_T   GVMScriptInfo[SWAP_SSB_SIZE];
    GVM_CHATTEXT_T		GVMChatText;	   /* GVM Chatting Text*/
    GVM_PTP_DIAL_T      GVMPtpDial;
};

typedef struct tagGVM_CONFIG_T      GVM_CONFIG_T;
extern GVM_CONFIG_T        g_GVMConfig;    /* GVM 정보 */

// added
#define sGET_GVM_SSB(s, i)		        (s).GVMScriptInfo[i].fSSB
#define sGET_GVM_DIR_NAME(s,i)          (s).GVMScriptInfo[i].ssbInfo.ScriptDir
#define sGET_GVM_USER_ID(s,i)		    (s).GVMScriptInfo[i].ssbInfo.UserID
#define sGET_GVM_GAME_CPID(s,i)         (s).GVMScriptInfo[i].ssbInfo.ScriptCPID
#define sGET_GVM_GAME_ID(s,i)           (s).GVMScriptInfo[i].ssbInfo.ScriptID
#define sGET_GVM_RUN_SVR_IP(s,i)        (s).GVMScriptInfo[i].ssbInfo.SvrIP
#define sGET_GVM_RUN_SVR_PORT(s,i)      (s).GVMScriptInfo[i].ssbInfo.SvrPort
#define sGET_GVM_GAME_NAME(s,i)         (s).GVMScriptInfo[i].ssbInfo.ScriptName
#define sGET_GVM_GAME_VER(s,i)          (s).GVMScriptInfo[i].ssbInfo.ScriptVer
#define sGET_GVM_GAME_TYPE(s,i)         (s).GVMScriptInfo[i].ssbInfo.ScriptType
#define sGET_GVM_GAME_SIZE(s,i)         (s).GVMScriptInfo[i].ssbInfo.ScriptSize
#define sGET_GVM_VALID_COUNT(s, i)      (s).GVMScriptInfo[i].ssbInfo.ValidCount
#define sGET_GVM_VALID_TERM(s, i, j)    (s).GVMScriptInfo[i].ssbInfo.ValidTerm[j]
#define sGET_GVM_IMAGE_TYPE(s, i)       (s).GVMScriptInfo[i].ssbInfo.ImageType

#define sGET_GVM_SSBINFO(s, i)          (s).GVMScriptInfo[i].ssbInfo
#define sGET_GVM_USER(s, i)		        (s).GVMScriptInfo[i].szUserData
#define sGET_GVM_CHATTEXT(s, i)		    (s).svRsvdChatText[i]
#define sGET_GVM_PTP_DIAL(s)            (s).szPTPDial

/* GVM 데이터 Handling */
#define SKY_GET_GVM_SSB(i)              sGET_GVM_SSB(g_GVMConfig, i)
#define SKY_GET_GVM_DIR_NAME(i)         sGET_GVM_DIR_NAME(g_GVMConfig, i)
#define SKY_GET_GVM_USER_ID(i)          sGET_GVM_USER_ID(g_GVMConfig, i)
#define SKY_GET_GVM_GAME_CPID(i)        sGET_GVM_GAME_CPID(g_GVMConfig, i)
#define SKY_GET_GVM_GAME_ID(i)          sGET_GVM_GAME_ID(g_GVMConfig, i)
#define SKY_GET_GVM_RUN_SVR_IP(i)       sGET_GVM_RUN_SVR_IP(g_GVMConfig, i)
#define SKY_GET_GVM_RUN_SVR_PORT(i)     sGET_GVM_RUN_SVR_PORT(g_GVMConfig, i)
#define SKY_GET_GVM_GAME_NAME(i)        sGET_GVM_GAME_NAME(g_GVMConfig, i)
#define SKY_GET_GVM_GAME_VER(i)         sGET_GVM_GAME_VER(g_GVMConfig, i)
#define SKY_GET_GVM_GAME_TYPE(i)        sGET_GVM_GAME_TYPE(g_GVMConfig, i)
#define SKY_GET_GVM_GAME_SIZE(i)        sGET_GVM_GAME_SIZE(g_GVMConfig, i)
#define SKY_GET_GVM_VALID_COUNT(i)      sGET_GVM_VALID_COUNT(g_GVMConfig, i)
#define SKY_GET_GVM_VALID_TERM(i, j)    sGET_GVM_VALID_TERM(g_GVMConfig, i, j)
#define SKY_GET_GVM_IMAGE_TYPE(i)       sGET_GVM_IMAGE_TYPE(g_GVMConfig, i)

#define SKY_GET_GVM_SSBINFO(i)          sGET_GVM_SSBINFO(g_GVMConfig, i)
#define SKY_GET_GVM_USER(i)             sGET_GVM_USER(g_GVMConfig, i)
#define SKY_GET_GVM_CHATTEXT(i)         sGET_GVM_CHATTEXT(g_GVMConfig.GVMChatText, i)
#define SKY_GET_GVM_PTP_DIAL()          sGET_GVM_PTP_DIAL(g_GVMConfig.GVMPtpDial)

/* GVM 데이터 Handling */
#define SKY_SET_GVM_SSBINFO(i, t)       sSET_CONFIG_VALUE(SC_GVM, SKY_GET_GVM_SSBINFO(i), t)

#define SKY_SET_GVM_USER_ID(i,t)        sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_USER_ID(i), t)
#define SKY_SET_GVM_IMAGE_TYPE(i, t)    sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_IMAGE_TYPE(i), t)
#define SKY_SET_GVM_DIR_NAME(i, t)      sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_DIR_NAME(i), t)
#define SKY_SET_GVM_GAME_CPID(i,t)      sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_CPID(i), t)
#define SKY_SET_GVM_GAME_ID(i,t)        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_ID(i), t)
#define SKY_SET_GVM_GAME_NAME(i,t)      sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_GAME_NAME(i), t)
#define SKY_SET_GVM_GAME_VER(i,t)       sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_VER(i), t)
#define SKY_SET_GVM_GAME_TYPE(i,t)      sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_TYPE(i), t)
#define SKY_SET_GVM_GAME_SIZE(i,t)      sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_SIZE(i), t)
#define SKY_SET_GVM_VALID_COUNT(i, t)   sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_VALID_COUNT(i), t)
#define SKY_SET_GVM_VALID_TERM(i, j, t) sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_VALID_TERM(i, j), t)
#define SKY_SET_GVM_RUN_SVR_IP(i,t)     sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_RUN_SVR_IP(i), t)
#define SKY_SET_GVM_RUN_SVR_PORT(i,t)   sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_RUN_SVR_PORT(i), t)

#define SKY_SET_GVM_SSB(i, t)           sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_SSB(i), t)
#define SKY_SET_GVM_PTP_DIAL(t)         sSET_CONFIG_STR(SC_GVM, SKY_GET_GVM_PTP_DIAL(), t)
#define SKY_SET_GVM_CHATTEXT(i,t)       sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_CHATTEXT(i), t)
#define SKY_SET_GVM_USER(i,t,len)       sSET_CONFIG_COPY(SC_GVM,SKY_GET_GVM_USER(i), t, len)
#define SKY_SET_GVM_USER_TEMP(i, t, len)	sSET_MEMORY_COPY(SC_GVM, SKY_GET_GVM_USER(i), t, len)

enum tagGVM_RUNNING_TYPE{
    GRT_NONE,
    GRT_DOWNLOAD_START,
    GRT_DOWNLOADING,
    GRT_DOWNLOAD_FAIL,
    GRT_DOWNLOAD_SUCCESS,
    GRT_RUN_START,
    //GRT_RUN_STANDALONE,
    GRT_RUN_PTP_START,
    GRT_RUN_PTPCALL,
    GRT_RUN_PTPWAIT,
    GRT_RUN_PTP,
    GRT_RUN_SERVER,
    GRT_RUN_MAX
};
typedef enum tagGVM_RUNNING_TYPE GVM_RUNNING_TYPE;


//extern BOOL                     g_fAlreadyDisconnectRequested;
//extern BOOL                     g_fCallDrop;
extern GVM_RUNNING_TYPE         g_nRunningType;
extern DWORD                    g_dwFreeSpace;
extern GVM_DISCONNECT_TYPE_E    g_eDisconnectType;
extern S_Result                 *recv_result; 
extern BOOL						g_fFromWAP;

extern int  midd_Connect2(void);
extern void midd_recvdata(char* buf, int len, int *recv_length);
extern int  midd_pppclose(void);
extern void midd_PtpRecvData(byte* data, int* length);
extern void sSetColorDisplay(BOOL flag);
#endif //FEATURE_GVM



#endif // _GVM_HEADER_H_
