/* -----------------------------------------------------------------------------------
**
**		Implement the parsing of AT specific Command for SK Telecom
** 
** -----------------------------------------------------------------------------------
*/

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====

GENERAL DESCRIPTION
  This module implements the parsing of AT modem commands for
  SK Telecom

EXTERNALIZED FUNCTIONS
	dsInitVariables
	setPktDsConnect
	setRawDsConnect
	setDsAtSrvcTypeOnIdle
	setDsAtSrvcTypeOnAlert
	doAtCopCrm
	doAtCopCta
	ds_atcop_do_prev
	dsPktAtCopProcEscape
	goToPktOnLine
	uiAnswerDataCall
	getPageRespServiceOption
	getPageRespPktServiceOption
	displayFirstDataBanner
	displayDataBanner
	dsInitAtCmdValue

when		who			what, where, why
--------	---			---------------------------------------------------------------
00/12/26	khekim		dsPktAtCopProcEscape()에서 dsmgr_cancel_timer( DS_RLP_IDLE_TIMEOUT_SIG) 수행함
						goToPktOnLine()에서 dsmgr_start_timer( DS_RLP_IDLE_TIMEOUT_SIG) 수행함
00/10/05    khekim		AirData 서비스시 SS-QNC(SO=4, RateSet1)호 수행하도록  Page Response처리함
00/09/28	khekim		getPageRespPktServiceOption()에서 Packet SO page수신시 Dormant
						State인 경우에만 Page Response(with PKT SO) message를 송신하고
						그렇지 않은 경우, Page Response(with SO=0) message를 송신한다.
00/09/25	khekim		doAtCopCrm()에서 at+crm=135, at+crm=151를 ERROR로 처리함
00/08/07    khekim      MC_IS2000_1X_PKT_SRVC(at+crm=150,151) 정의==>MC_ATPKT_SRVC로 대치
00/07/26	khekim		SetSerialEnabledMode(), SetDsSerialEnableMode() 정의
00/07/25	khekim		setDsAtSrvcTypeOnAlert() 정의
00/07/12	khekim		MC_ATPKT_SRVC 정의: 전화접속네트워킹에서 Dormant 기능이 되는 PKT_SRVC
00/07/25    khekim      MC_IS2000_1X_PKT_SRVC(at+crm=151) 정의
00/07/12	khekim		MC_ATPKT_SRVC 정의
00/05/09	khekim		FEATURE_AIRDATA를 FEATURE_SKT_PTPAPI로 rename
00/05/09    khekim      MC_IS2000_1X_PKT_SRVC(at+crm=150) 정의
00/04/01	khekim		FEATURE_DS_NET_MODEL 정의되어 있을때 at+crm=2동작하도록 수정
00/03/20	khekim		displayFirstDataBanner() 구현: Power On후 DTR On/착신모드 설정된 경우에만
                        LCD의 첫번째 라인에 display
00/03/02	khekim		데이터 착신관련 getPageRespServiceOption() 수정
00/03/01	khekim		uiscrn_SetPageType(), uiscrn_SetPageLineReverseType() 다시 사용함
00/03/01	khekim		FEATURE_AIRDATA 추가 구현
00/02/29	khekim		displayDataBanner() 수정 
00/02/29	khekim		dsInitAtCmdValue() 구현: at+fclass, at&e값을 default값으로 설정
00/02/24	khekim		word dsSrvcOpt 사용문장 제거
00/02/23	khekim		FEATURE_SKT_DR_DS 조건부 선언문 제거
00/02/23	khekim		uiscrn_SetPageType(), uiscrn_SetPageLineReverseType() 사용안함
00/02/21	khekim		displayDataBanner()에서 Banner Line을 반전하는 부분 삽입
00/02/21	khekim		displayDataBanner() 구현
00/02/20	khekim		when parsing at+crm, at+fclass, dsAtSrvcType is determined
00/02/20	khekim		ds_atcop_do_prev() 구현
00/02/20	khekim		AT+CTA (doAtCopCta()) parsing 구현
00/02/17	khekim		dsAtSktDrVal is defined
00/02/15	khekim		getPageRespServiceOption() 구현
00/02/14	khekim		uiAnswerDataCall(): UI Task에서 Data Call에 대해서 
                        Any key Answer로 응답하는 루틴 구현
00/02/09	khekim		dsPktAtCopProcEscape(), goToPktOnLine() 구현
						각각 packet(piwf) online상태에서 +++ 인식 및 
						online command 상태에서 online 상태로 진입함
00/02/08	khekim		setPktDsConnect(), setRawDsConnect() 구현   
						online 상태 진입시 dsConnectMode=TRUE로 설정함
00/01/21	khekim		AT+CRM (doAtCopCrm()) parsing 구현
00/01/21	khekim		setDsAtSrvcTypeOnIdle()구현: origination시도시 service option 결정을  
						위해 service type 결정
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "comdef.h"
#include "customer.h"
#ifdef	FEATURE_SKT_DS	//	khekim 00/01/21
#include "target.h"
#include "err.h"
#include "bbver.h"
#include "dsm.h"
#include "ds.h"
#include "dsi.h"
#include "dsmgr.h"
#include "dsatcopi.h"
#include "db.h"
#include "tcp.h"
#include "mobile.h"
#include "sio.h"
#include "dsrmio.h"
#include "dssocfg.h"
#include "cai.h"
#include "ps.h"
#include "psi.h"
#include "snm.h"
#include "memory.h"
#include "assert.h"
#include "diag.h"
#include "nv.h"
#include "msg.h"
#include "mc.h"		//	MC_END_F
#include "err.h"

#if MMI_TYPE == SKY_UI	//	khekim 01/02/02
#include "sky.h"
#include "ska_gvm.h"
#ifdef FEATURE_SKVM
#include "ska_skvm.h"
#endif
#else	//		#if MMI_TYPE == SKY_UI
#include <ui.h>
#ifdef	MMI_TREX	//	khekim 00/07/3
#include <uinstate.h>
#include "uinscrn.h"
#else	//	#ifdef	MMI_TREX
#include "uiuint.h"		// ui_send_mc_cmd() 
#include "uixscrn.h"	// uiscrn_bind_act(), uiscrn_refresh()
#include "uiutxti.h"	// UI_MODEM_F
#ifdef	FEATURE_SKT_DS_MMI	//	khekim 00/02/21
#if (MMI_TYPE == MMI_A10)
#include <uistate.h>
#include <PageType.h>
#else
#include "uistate.h"
#include "..\LCD\PageType.h"
#endif //#if (MMI_TYPE == MMI_A10)
#endif	//	#ifdef	FEATURE_SKT_DS_MMI
#endif	//	#ifdef	MMI_TREX

#ifdef	FEATURE_SKT_PTPAPI	//	khekim 00/03/01
#if (MMI_TYPE == MMI_A10)
#include	"DataManager\AirData.h"
#else	//	#if (MMI_TYPE == MMI_A10)
#include "AirData.h"
#endif	//	#if (MMI_TYPE == MMI_A10)
#endif	//	#ifdef	FEATURE_SKT_PTPAPI	//	khekim 00/03/01

#ifdef	FEATURE_GVM	//	khekim 01/02/18
#include "uicommon.h"
#include "netgame.h"	//	SvrType==GAME_TYPE, DLOAD_TYPE
#endif	//	#ifdef	FEATURE_GVM

#endif	//		#if MMI_TYPE == SKY_UI

#ifdef	FEATURE_SKT_PTPAPI	//	khekim 00/10/30
#include "uisockApi.h"
#endif	//	#ifdef	FEATURE_SKT_PTPAPI

#include	"dsskt.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
// E-mail, Netgame, IP Airdata wouldn't support dormant function.
#define	DS_IS_DORMANT(so)	(DS_ISDATA(so) && DS_ISDATA_PPP_PKT(so) && so!=CAI_SO_DATA && !DS_ISDATA_RLP_TYPE2(so))

// !!!!!!!!!!!!!!   Copyed from DS_DORMANT_IDLE_MASK in dsmgr. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define DS_DORMANT_IDLE_MASK  0x0000   /* Traffic channel is idle, idle    */
                                       /* timer running. When a call starts*/
                                       /* this is the default bitmask (so  */
                                       /* idle timer starts running as soon*/ 
                                       /* as call comes up                 */

//////////////////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
// The following type is defined in dsatcop.c
	typedef enum
	{                     /*  ds_atcop_atoi result returns:   */
		ATOI_OK,              /*  conversion complete             */
		ATOI_NO_ARG,          /*  nothing to convert              */
		ATOI_OUT_OF_RANGE     /*  character or value out of range */
	} atoi_enum_type;

	typedef enum            /*  specifies the format type for   */
	{                     /*    displaying AT parameters      */

		RESPONSE,             /*  Generates response to command - */
							/*    No name for +Fxxx             */

		RESPONSE_ALL,         /*  Generates response to command - */
							/*    Names all commands            */

		SET                   /*  Generates a SET command - used  */
							/*    to transmit configuaration    */
							/*    to the base station           */
	} fmt_enum_type;

/*---------------------------------------------------------------------------
  Type that represents the current protocol state of the PS task.
---------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
boolean	dsConnectMode;
boolean	dsPktOnCommandState;
boolean	dsDtrValue;
#ifdef	FEATURE_SKT_CANCEL_CALL	//	khekim 00/02/24
boolean	dsCalling;
#endif	//	#ifdef	FEATURE_SKT_CANCEL_CALL
#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
boolean dsSerialEnabledMode = FALSE;
#endif	//	#ifdef	FEATURE_SKT_ENABLE_SERIAL
ds_atcop_num_item_type	dsOldCtaVal;	//	khekim 00/09/19

//	PIWF서비스시 online command 상태로 진입했다가 다시 online 상태로 재진입했을 때 srvc type 유지하기 위함
mc_srvc_req_enum_type		dsOldSrvcType;	
mc_srvc_req_enum_type		dsAtSrvcType;	//	service type is determined according to at+crm, at+fclass
// data 착신 관련 변수: is is referenced in dsatskt.c or atskt.c.
ds_atcop_num_item_type dsAtSktDrVal;


#ifdef	FEATURE_SKT_DS_REDIAL
boolean dsRedialing = FALSE;
boolean	dsEnableRedial = FALSE;
#endif	//	#ifdef	FEATURE_SKT_DS_REDIAL
#ifdef	FEATURE_SKT_PIWFPTPAPI	//	khekim 00/08/28
DS_SRVC_TYPE	dsAirDataSrvc;
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI

#ifdef	FEATURE_RM_DORMANT	//	khekim 00/11/13
boolean dsDtrOffOnDormant;
#endif	//	#ifdef	FEATURE_RM_DORMANT

#ifdef	FEATURE_SKT_SOCK_SUP_95AC	//	khekim 00/11/22
byte	dsOrigDialStrLen = 4;
const	byte	dsOrigDialStr_95A[] = "\x1b\x00\x00\x1b";	//	ESC NULL NULL ESC
const	byte	dsOrigDialStr_95C[] = "1501";	//	1501
char	*dsAuthUserName_95A="sktelecom";
char	*dsAuthPasswd_95A="";
#ifdef	FEATURE_DS_IS2000
char	*dsAuthUserName_95C="sktelecom@wap95c.n-top.com";
char	*dsAuthPasswd_95C="";
#endif	//	#ifdef	FEATURE_DS_IS2000

#ifdef FEATURE_STI_DS
byte	dsOrigDialStrLen_STI = 10;
char	*dsAuthUserName_STI_95C="shinsegi@wap95c.n-top.com";
const	byte	dsOrigDialStr_STI_95A[] = "#546*7264#";	//	#546*7264#
#endif //#ifdef FEATURE_STI_DS

#ifdef FEATURE_STI_WAP

#ifndef FEATURE_STI_DS
byte	dsOrigDialStrLen_STI = 10;
#endif //#ifndef FEATURE_STI_DS

byte	dsOrigDialStrLen_Normal = 4;
const	byte	dsOrigDialStr_95A_STI[] = "#546*7264#";	//2G용 STI번호(단,GVM, PM,WAP)일시만 사용 
const	byte	dsOrigDialStr_Normal[]  = "0017";	//	1501

char	*dsAuthUserName_95C_STI="shinsegi@wap95c.n-top.com";//2.5G STI NAI
char	*dsAuthUserName_95A_STI="power017";//2G일시 사용하며,GVM,WAP,PM일시는 PAP자체를 사용하지 않음 
#endif//#ifdef FEATURE_STI_WAP

boolean	dsSockMode = FALSE;
boolean	dsNetGameMode = FALSE;
#endif	//	#ifdef	FEATURE_SKT_SOCK_SUP_95AC
#ifdef	FEATURE_SKT_SIMPLEIP	//	khekim 00/11/22
boolean	dsPPPRelayResynced;
boolean	dsPZIDChanged=FALSE;
#endif	//	#ifdef	FEATURE_SKT_SIMPLEIP
boolean	dsDormantEnabled;
word    dsSrvcOpt;

#ifdef FEATURE_NETWORK
const byte dsOrigDialStr_UP[] = "999331";
#endif//FEATURE_NETWORK

#ifdef	FEATURE_DS_IS2000
const snm_sch_mo_list_type	so33SchDefaultFwdMO =	//	Forward MO = 0x921
{
                                          /* # of mux options for fwd sch  */
    (byte)8,                              /* preferences for fwd sch mos   */
    {
      (word)CAI_MUX_16X_RS1_DOUBLE,       /* MuxPDU 3, 16x, double sch     */
      (word)CAI_MUX_8X_RS1_DOUBLE,        /* MuxPDU 3,  8x, double sch     */
      (word)CAI_MUX_8X_RS1_SINGLE,        /* MuxPDU 3,  8x, single sch     */
      (word)CAI_MUX_4X_RS1_DOUBLE,        /* MuxPDU 3,  4x, double sch     */
      (word)CAI_MUX_4X_RS1_SINGLE,        /* MuxPDU 3,  4x, single sch     */
      (word)CAI_MUX_2X_RS1_DOUBLE,        /* MuxPDU 3,  2x, double sch     */
      (word)CAI_MUX_2X_RS1_SINGLE,        /* MuxPDU 3,  2x, single sch     */
      (word)CAI_MUX_1X_RS1,               /* MuxPDU 1,  1x, single sch     */
      (word)0, (word)0, (word)0, (word)0,
      (word)0, (word)0, (word)0
    }
};

snm_sch_mo_list_type	so33SchMedFwdMO =	//	Forward MO = 0x01
{
                                          /* # of mux options for fwd sch  */
    (byte)8,                              /* preferences for fwd sch mos   */
    {
      (word)CAI_MUX_16X_RS1_DOUBLE,       /* MuxPDU 3, 16x, double sch     */
      (word)CAI_MUX_8X_RS1_DOUBLE,        /* MuxPDU 3,  8x, double sch     */
      (word)CAI_MUX_8X_RS1_SINGLE,        /* MuxPDU 3,  8x, single sch     */
      (word)CAI_MUX_4X_RS1_DOUBLE,        /* MuxPDU 3,  4x, double sch     */
      (word)CAI_MUX_4X_RS1_SINGLE,        /* MuxPDU 3,  4x, single sch     */
      (word)CAI_MUX_2X_RS1_DOUBLE,        /* MuxPDU 3,  2x, double sch     */
      (word)CAI_MUX_2X_RS1_SINGLE,        /* MuxPDU 3,  2x, single sch     */
      (word)CAI_MUX_1X_RS1,               /* MuxPDU 1,  1x, single sch     */
      (word)0, (word)0, (word)0, (word)0,
      (word)0, (word)0, (word)0
    }
};

const snm_sch_mo_list_type	so33SchMinFwdMO =	//	Forward MO = 0x01
{
                                          /* # of mux options for fwd sch  */
    (byte)0,                              /* preferences for fwd sch mos   */
    {
      (word)CAI_MUX_16X_RS1_DOUBLE,       /* MuxPDU 3, 16x, double sch     */
      (word)CAI_MUX_8X_RS1_DOUBLE,        /* MuxPDU 3,  8x, double sch     */
      (word)CAI_MUX_8X_RS1_SINGLE,        /* MuxPDU 3,  8x, single sch     */
      (word)CAI_MUX_4X_RS1_DOUBLE,        /* MuxPDU 3,  4x, double sch     */
      (word)CAI_MUX_4X_RS1_SINGLE,        /* MuxPDU 3,  4x, single sch     */
      (word)CAI_MUX_2X_RS1_DOUBLE,        /* MuxPDU 3,  2x, double sch     */
      (word)CAI_MUX_2X_RS1_SINGLE,        /* MuxPDU 3,  2x, single sch     */
      (word)CAI_MUX_1X_RS1,               /* MuxPDU 1,  1x, single sch     */
      (word)0, (word)0, (word)0, (word)0,
      (word)0, (word)0, (word)0
    }
};

const snm_rc_list_type so33SchDefaultFwdRC =
{
    2,                                  /* number of rc's in the list      */
    {                                   /* forward radio channel           */
      CAI_RC_3,                         /* 1x phys channel (preferred)     */ 
      CAI_RC_4,                         /* 1x phys channel                 */ 
      (cai_radio_config_type)0
    }
};

#endif	//	#ifdef	FEATURE_DS_IS2000

//////////////////////////////////////////////////////////////////////////////////////////////
// EXTERNAL VARIABLES
// dsi_so_async_data[], dsi_so_g3_fax[], dsi_so_ppp_pkt_data[] is defined in dsmgr.c
extern	word dsi_so_async_data[DS_RATESET_COUNT];
extern	word dsi_so_g3_fax[DS_RATESET_COUNT];
extern	word dsi_so_ppp_pkt_data[DS_RATESET_COUNT];
extern  ds_rm_pkt_call_ctl_enum_type ds_rm_pkt_call_ctl_state;
extern	word dsi_dormant_bitmask; /* dormant bitmask */

#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
extern	void dsi_set_serial_mode
( 
	dsi_serial_mode_enum_type new_serial_mode
);
#endif	//	#ifdef	FEATURE_SKT_ENABLE_SERIAL


// dsi_set_srvc_mode() is defined in dsmgr.c 
extern	void dsi_set_srvc_mode
(
	dsi_data_srvc_enum_type new_srvc_mode
);

// ds_atcop_fmt_num_val_ex() is defined in dsmgr.c 
extern	unsigned int ds_atcop_fmt_num_val_ex
(
  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
  byte *rb_ptr,                 /*  point to buffer for result  */
  fmt_enum_type fmt_sw          /*  selects response or set fmt */
);

// ds_atcop_srch_list() is defined in dsmgr.c 
extern	atoi_enum_type ds_atcop_srch_list
(
	ds_atcop_num_item_type *val_arg_ptr,
	const list_type l,
	byte * arg
);

// ds_atcop_atoi() is defined in dsmgr.c 
extern	atoi_enum_type ds_atcop_atoi
(
  ds_atcop_num_item_type *val,  /*  value returned  */
  const byte *s,                /*  points to string to eval  */
  unsigned int r                /*  radix */
);

extern void dsi_send_mc_cmd
( 
	mc_msg_type *mc_cmd_ptr, 
	mc_name_type cmd
);

extern boolean ui_send_mc_cmd ( mc_name_type cmd );

#ifdef  FEATURE_STI_DS   // heroine 01/06/08
boolean dsIsSTINumber(void)
{
  MSG_ERROR("imsi_s2 = %x", cdma.imsi_s2, 0, 0);

	if(cdma.imsi_s2 == 0x38a)
    return TRUE;
  else
    return FALSE;
}

boolean dsIsSTISID(void)
{
  db_items_value_type db;
	db_get(DB_SID,&db);

  if(db.sid == 1700)
    return TRUE;
  else
    return FALSE;
}
#endif  // FEATURE_STI_DS   // heroine 01/06/08

//////////////////////////////////////////////////////////////////////////////////////////////
// khekim 00/01/21 dsmgr.c에서 Idle상태에서 변수 초기화
void	dsInitVariables
(
	void
)
{
    if (ds_rm_pkt_call_ctl_state!=DS_PKT_DORM_IDLE_STATE
		&& dsi_protocol_state!=DS_PROTOCOL_OPEN) // khekim 00/07/26 not Dormant Mode
	{
#ifdef	FEATURE_SKT_CANCEL_CALL	//	khekim 00/02/24
		dsCalling = FALSE;
#endif	//	#ifdef	FEATURE_SKT_CANCEL_CALL
  	    dsConnectMode = FALSE;
	    dsPktOnCommandState = FALSE;
#ifdef	FEATURE_RM_DORMANT	//	khekim 00/11/13
		dsDtrOffOnDormant = FALSE;
#endif	//	#ifdef	FEATURE_RM_DORMANT
		dsDormantEnabled = FALSE;
    dsSrvcOpt = 0;
	}
#ifdef	FEATURE_SKT_DS_REDIAL	//	khekim 00/02/22
	dsRedialing = FALSE;
	dsEnableRedial = FALSE;
#endif	//	#ifdef	FEATURE_SKT_DS_REDIAL	//	khekim 00/02/22
#ifdef	FEATURE_SKT_PIWFPTPAPI	//	khekim 00/08/28
	dsAirDataSrvc = DS_ASYNC_SRVC;
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
#ifdef	FEATURE_SKT_SIMPLEIP	//	khekim 00/11/22
	dsPPPRelayResynced = FALSE;
#endif	//	#ifdef	FEATURE_SKT_SIMPLEIP
}


//////////////////////////////////////////////////////////////////////////////////////////////
// khekim 00/02/08 Packet DS(PIWF, SS-QNC, MDR PKT)에서 Connect되었을 때 수행
void	setPktDsConnect
(
	void
)
{
	if (
		dsConnectMode==FALSE && // khekim 00/07/26
		dsi_srvc_mode==DS_PKT_SRVC_MODE && 
		(
		FALSE
#ifdef	FEATURE_SKT_PIWF_DS
		|| dsi_cur_srvc_type==MC_PIWF_SRVC 
#endif	//	#ifdef	FEATURE_SKT_PIWF_DS
#ifdef	FEATURE_SKT_ATPKT_DS	//	khekim 00/07/12
		|| dsi_cur_srvc_type==MC_ATPKT_SRVC
#endif	//	#ifdef	FEATURE_SKT_ATPKT_DS
		) )
	{
		dsConnectMode = TRUE;
		ds_atcop_return_cell_result( DS_ATCOP_CONNECT ); // Transmit Connect result to UART
		ds_set_dcd();
#ifdef	FEATURE_SKT_DS_CELLRESULT	//	khekim 00/04/04
		ds_atcop_return_cell_result(DS_ATCOP_CPSR_3);
#endif	//	#ifdef	FEATURE_SKT_DS_CELLRESULT	//	khekim 00/04/04
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// khekim 00/02/08 Circuit DS에서 Connect되었을 때 수행
void	setRawDsConnect
(
	void
)
{
	if (dsConnectMode==FALSE)
	{
		dsConnectMode = TRUE;
#if defined(FEATURE_SKT_PTPAPI)	//	khekim 00/10/30
		if (dsi_srvc_mode == DS_AIRDATA_SRVC_MODE)
		{
            /* undersea 4334-08-08 --> @why, what */            
			//PtpLinkResult(dsConnectMode);
            	
			ui_cmd_type *cmd_ptr; 

		    if (dsConnectMode==TRUE)
		    {
			    MSG_HIGH( "DS=>UI: CONNECT_SUCCESS_F", 0, 0, 0);
			    //cmd_ptr->hdr.cmd         = UI_PTP_CONNECT_SUCCESS_F; //UI_SOCK_TCPCONNECT_SUCCESS_F;
                send_ui_cmd(UI_PTP_CONNECT_SUCCESS_F);
		    }
		    else
		    {
			    MSG_HIGH( "DS=>UI: CONNECT_FAIL_F", 0, 0, 0);
			    //cmd_ptr->hdr.cmd         = UI_PTP_CONNECT_FAIL_F; //UI_SOCK_TCPCONNECT_FAIL_F;
                send_ui_cmd(UI_PTP_CONNECT_FAIL_F);
		    }
			//ui_cmd(cmd_ptr);
            /* <-- */
		}
#endif	//	#if defined(FEATURE_SKT_PTPAPI)
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// khekim 00/01/21 origination 시도시 service option 결정
mc_srvc_req_enum_type	setDsAtSrvcTypeOnIdle
(
	void
)
{
	mc_srvc_req_enum_type	mcSrvcType;

#ifndef	A10I		// heroine 00/12/15
	if (ds_atcop_fclass_val==0)	
#endif	// #ifndef	A10I		// heroine 00/12/15
	{
#ifndef	A10I		// heroine 00/12/15
		if (ds_atcop_crm_val==0)
		{
			mcSrvcType = MC_ASYNC_SRVC;
		}
		else 
#endif	// #ifndef	A10I		// heroine 00/12/15
		if (ds_atcop_crm_val==1)
		{
#ifdef	FEATURE_SKT_ATPKT_DS	//	khekim 00/07/12
			mcSrvcType = MC_ATPKT_SRVC;
#else	//	#ifdef	FEATURE_SKT_DS
			mcSrvcType = MC_PKT_SRVC;
#endif	//	#ifdef	FEATURE_SKT_DS
		}
#ifdef FEATURE_DS_NET_MODEL	//	khekim 00/04/01
		else if (ds_atcop_crm_val==2)
		{
#ifdef	FEATURE_SKT_ATPKT_DS	//	khekim 00/07/12
			mcSrvcType = MC_ATPKT_SRVC;
#else	//	#ifdef	FEATURE_SKT_DS
			mcSrvcType = MC_PKT_SRVC;
#endif	//	#ifdef	FEATURE_SKT_DS
		}
#endif	//	#ifdef FEATURE_DS_NET_MODEL
#ifndef	A10I		// heroine 00/12/15
#ifdef	FEATURE_SKT_PIWF_DS	// khekim 00/1/21
		else if (ds_atcop_crm_val==129)
		{
			mcSrvcType = MC_PIWF_SRVC;
		}
#endif	// #ifdef	FEATURE_SKT_PIWF_DS	// khekim 00/1/21
		else if (ds_atcop_crm_val==131)
		{
#ifdef	FEATURE_SKT_ATPKT_DS	//	khekim 00/07/12
			mcSrvcType = MC_ATPKT_SRVC;
#else	//	#ifdef	FEATURE_SKT_DS
			mcSrvcType = MC_PKT_SRVC;
#endif	//	#ifdef	FEATURE_SKT_DS
		}
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)	//	khekim 00/1/21
		else if (ds_atcop_crm_val==135)
		{
#ifdef	FEATURE_SKT_ATPKT_DS	//	khekim 00/07/12
			mcSrvcType = MC_ATPKT_SRVC;
#else	//	#ifdef	FEATURE_SKT_DS
			mcSrvcType = MC_PKT_SRVC;
#endif	//	#ifdef	FEATURE_SKT_DS
		}
#endif	//	#ifdef	FEATURE_SPECIAL_MDR	//	khekim 00/1/21
#endif	// #ifndef	A10I		// heroine 00/12/15
#ifdef FEATURE_DS_IS2000	//	khekim 00/05/09
		else if (ds_atcop_crm_val==150)
		{
#ifdef	FEATURE_SKT_ATPKT_DS	//	khekim 00/07/12
			mcSrvcType = MC_ATPKT_SRVC;
#else	//	#ifdef	FEATURE_SKT_DS
			mcSrvcType = MC_PKT_SRVC;
#endif	//	#ifdef	FEATURE_SKT_DS
		}
#endif	//	#ifdef FEATURE_DS_IS2000
		else
		{
#ifdef  FEATURE_STI_DS    // heroine 01/07/05
            if (SKY_GET_SERVICE_PROVIDER() == SP_STI)    // 017-xxx-xxxx
            {
                ds_atcop_crm_val = 1;
#ifdef	FEATURE_SKT_ATPKT_DS	//	khekim 00/07/12
			    mcSrvcType = MC_ATPKT_SRVC;
#else	//	#ifdef	FEATURE_SKT_DS
			    mcSrvcType = MC_PKT_SRVC;
#endif	//	#ifdef	FEATURE_SKT_DS
            }
            else                          // 011-xxx-xxxx
#endif  // FEATURE_STI_DS    // heroine 01/07/05
            {
			    ds_atcop_crm_val = 0;
			    mcSrvcType = MC_ASYNC_SRVC;
            }			
		}
	}
#ifndef	A10I		// heroine 00/12/15
	else	// ds_atcop_fclass_val=2
	{
		ds_atcop_crm_val = 0;
		mcSrvcType = MC_FAX_SRVC;
	}
#endif	// #ifndef	A10I		// heroine 00/12/15
	MSG_HIGH("setDsAtSrvcTypeOnIdle()>> mcSrvcType %x\n", mcSrvcType, 0, 0);
	dsAtSrvcType = mcSrvcType;	//	save dsi_cur_srvc_type
	return (dsAtSrvcType);
}



//////////////////////////////////////////////////////////////////////////////////////////////
// DS_ALERT_CMD 수신시 dsAtSrvcType 설정
void	setDsAtSrvcTypeOnAlert
(
	void
)
{
	dsAtSrvcType = dsi_cur_srvc_type;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// khekim 00/01/21 at+crm command 입력시 수행됨
void doAtCopCrm
(
  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
  tokens_struct_type *tok_ptr,  /*  Command tokens from parser  */
  byte *rb_ptr                  /*  point to buffer for result  */
)
{
#ifdef	A10I		// heroine 00/12/15
	#ifdef FEATURE_DS_NET_MODEL	//	khekim 00/04/01
		#ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
			static const byte crm_range[] = "+CRM: (1,2,150)";
		#else	// #ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
			static const byte crm_range[] = "+CRM: (1,2)";
		#endif	// #ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
	#else	// #ifdef FEATURE_DS_NET_MODEL	//	khekim 00/04/01
		#ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
			static const byte crm_range[] = "+CRM: (1,150)";
		#else	// #ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
			static const byte crm_range[] = "+CRM: (1)";
		#endif	// #ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
	#endif	// 
#else	// #ifdef	A10I		// heroine 00/12/15
	#ifdef FEATURE_DS_NET_MODEL	//	khekim 00/04/01
	#ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
		static const byte crm_range[] = "+CRM: (0,1,2,129,131,150)";
	#else	//	#ifdef	FEATURE_DS_IS2000
		static const byte crm_range[] = "+CRM: (0,1,2,129,131,135)";
	#endif	//	#ifdef	FEATURE_DS_IS2000
	#else	//	#ifdef FEATURE_DS_NET_MODEL
	#ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
		static const byte crm_range[] = "+CRM: (0,1,129,131,150)";
	#else	//	#ifdef	FEATURE_DS_IS2000
		static const byte crm_range[] = "+CRM: (0,1,129,131,135)";
	#endif	//	#ifdef	FEATURE_DS_IS2000
	#endif	//	#ifdef FEATURE_DS_NET_MODEL
#endif	// #ifdef	A10I		// heroine 00/12/15

  const byte *s_ptr;
  ds_atcop_num_item_type scr_val [MAX_ARG];
  byte flag [MAX_ARG];
  atoi_enum_type e;
  word radix;
  unsigned int j;


  *rb_ptr = '\0';

	if (tok_ptr->op == (NA))            /*  Name only - set to default  */
	{
		* ((ds_atcop_num_item_type *)tab_entry->val_ptr) = 
			((dflm_type *) tab_entry->def_lim_ptr)[0].default_v;
	}

	else if (tok_ptr->op == (NA|EQ|QU)) /*  TEST command  */
	{
//    s_ptr = tab_entry->name;
//    *rb_ptr++ = '+';
//
//    while ( (c = *s_ptr++) != 0)
//      {
//      *rb_ptr++ = c;
//      }
//
//    *rb_ptr++ = ':';
//    *rb_ptr++ = ' ';

		s_ptr = crm_range;
		while ( (*rb_ptr++ = *s_ptr++) != 0)
			;
	}

	else if (tok_ptr->op == (NA|QU))    /*  READ command  */
	{
		(void) ds_atcop_fmt_num_val_ex (tab_entry, rb_ptr, RESPONSE);
	}

	else if (tok_ptr->op == (NA|EQ|AR)) /*  SET command   */
	{
//	(void) ds_atcop_put_num_arg (tab_entry, tok_ptr);
// 아래 문장은 ds_atcop_put_num_arg ()를 변형한 것임
		radix = ( (tab_entry->attrib & HEX) ? 16 : 10);

		if (tok_ptr->args_found > tab_entry->compound)
		{
			ds_atcop_result = DS_ATCOP_ERROR; /*  Too many arguments  */
			DS_ATCOP_ERROR_MSG();
			return;
		}

		for (j = 0; j < tok_ptr->args_found; ++j)
		{
			flag [j] = 0;
			if ((tab_entry->attrib & LIST) == 0)
			{
				e = ds_atcop_atoi (&scr_val [j], tok_ptr->arg[j], radix);
				if (e == ATOI_OK &&
					(scr_val [j] < ((dflm_type *) tab_entry->def_lim_ptr)[j].lower
					|| scr_val [j] > ((dflm_type *) tab_entry->def_lim_ptr)[j].upper))
				{
					e = ATOI_OUT_OF_RANGE;
				}
			}
			else
			{
				e = ds_atcop_srch_list (&scr_val [j], ((def_list_type *) tab_entry->def_lim_ptr)->list_v, *tok_ptr->arg);
			}
#ifdef DUMP_ARG /*  Unit test debugging */
			printf ("arg[%d] = %d, err=%d\n", j, scr_val[j], e);
#endif  /*  DUMP_ARG  */
			if (e == ATOI_OUT_OF_RANGE)
			{
				DS_ATCOP_ERROR_MSG();
				ds_atcop_result = DS_ATCOP_PARAM;
				return;
			}
			else if (e == ATOI_OK)
			{
				flag [j] = 1;
				switch (scr_val[j])
				{
#ifdef	FEATURE_SKT_PIWF_DS	// khekim 00/1/21
					case	129:					// PIWF Data
#endif	// #ifdef	FEATURE_SKT_PIWF_SRVC	// khekim 99/2/11
					case	131:					// Single-Stack	QNC
#ifndef	FEATURE_DS_IS2000	//	khekim 00/09/25
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)	//	khekim 00/1/21
					case	135:					// SPECIAL_MDR
#endif	//	#ifdef	FEATURE_SPECIAL_MDR	//	khekim 00/1/21
#endif	//	#ifndef	FEATURE_DS_IS2000
					case	1:						// Packet Data 		
#ifdef FEATURE_DS_NET_MODEL	//	khekim 00/04/01
					case	2:
#endif	//	#ifdef FEATURE_DS_NET_MODEL
#ifdef	FEATURE_DS_IS2000	//	khekim 00/05/09
					case	150:
#endif	//	#ifdef	FEATURE_DS_IS2000
#ifdef	FEATURE_SKT_DS_CELLRESULT	//	khekim 00/04/04
						ds_atcop_return_cell_result(DS_ATCOP_CPACKET);
#endif	//	#ifdef	FEATURE_SKT_DS_CELLRESULT	//	khekim 00/04/04
#ifndef	A10I		// heroine 00/12/15
					case	0:						// Async Data and Fax
#endif	//	#ifndef	A10I
						break;
					default:
						DS_ATCOP_ERROR_MSG();
						ds_atcop_result = DS_ATCOP_PARAM;
//						printf ("range error\n");
						return;
				}
#ifdef DUMP_ARG /*  Unit test debugging */
		  printf (", flag = %d\n", flag [j]);
#endif  /*  DUMP_ARG  */
			/*else if (e == ATOI_NO_ARG) do nothing: flag[j] remains unset  */
			/*    and the value is not stored                               */
			}
		}
		for (j = 0; j < tok_ptr->args_found; ++j)
		{
			if (flag[j])
			{
				* ((ds_atcop_num_item_type *)tab_entry->val_ptr + j)
				  = scr_val [j];
			}
		}
	}
	else                                /*  Didn't make sense - ERROR */
	{
		ds_atcop_result = DS_ATCOP_ERROR;
		DS_ATCOP_ERROR_MSG();
		return;		// khekim 00/1/21
	}
	MSG_HIGH("doAtCopCrm()>> crmVal %d", ds_atcop_crm_val, 0, 0);
	(void) setDsAtSrvcTypeOnIdle();	//	determine dsAtSrvcType
} /*  dsAtCopCrm  */
						

//////////////////////////////////////////////////////////////////////////////////////////////
// khekim 00/02/20 parse at+cta command
void doAtCopCta
(
  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
  tokens_struct_type *tok_ptr,  /*  Command tokens from parser  */
  byte *rb_ptr                  /*  point to buffer for result  */
)
{
  static const byte cta_range[] = "+CTA: (0, 20-255)";

  const byte *s_ptr;
  ds_atcop_num_item_type scr_val [MAX_ARG];
  byte flag [MAX_ARG];
  atoi_enum_type e;
  word radix;
  unsigned int j;


  *rb_ptr = '\0';

	if (tok_ptr->op == (NA))            /*  Name only - set to default  */
	{
		* ((ds_atcop_num_item_type *)tab_entry->val_ptr) = 
			((dflm_type *) tab_entry->def_lim_ptr)[0].default_v;
	}

	else if (tok_ptr->op == (NA|EQ|QU)) /*  TEST command  */
	{
//    s_ptr = tab_entry->name;
//    *rb_ptr++ = '+';
//
//    while ( (c = *s_ptr++) != 0)
//      {
//      *rb_ptr++ = c;
//      }
//
//    *rb_ptr++ = ':';
//    *rb_ptr++ = ' ';

		s_ptr = cta_range;
		while ( (*rb_ptr++ = *s_ptr++) != 0)
			;
	}

	else if (tok_ptr->op == (NA|QU))    /*  READ command  */
	{
		(void) ds_atcop_fmt_num_val_ex (tab_entry, rb_ptr, RESPONSE);
	}

	else if (tok_ptr->op == (NA|EQ|AR)) /*  SET command   */
	{
//	(void) ds_atcop_put_num_arg (tab_entry, tok_ptr);
// 아래 문장은 ds_atcop_put_num_arg ()를 변형한 것임
		radix = ( (tab_entry->attrib & HEX) ? 16 : 10);

		if (tok_ptr->args_found > tab_entry->compound)
		{
			ds_atcop_result = DS_ATCOP_ERROR; /*  Too many arguments  */
			DS_ATCOP_ERROR_MSG();
			return;
		}

		for (j = 0; j < tok_ptr->args_found; ++j)
		{
			flag [j] = 0;
			if ((tab_entry->attrib & LIST) == 0)
			{
				e = ds_atcop_atoi (&scr_val [j], tok_ptr->arg[j], radix);
				if (e == ATOI_OK &&
					(scr_val [j] < ((dflm_type *) tab_entry->def_lim_ptr)[j].lower
					|| scr_val [j] > ((dflm_type *) tab_entry->def_lim_ptr)[j].upper))
				{
					e = ATOI_OUT_OF_RANGE;
				}
			}
			else
			{
				e = ds_atcop_srch_list (&scr_val [j], ((def_list_type *) tab_entry->def_lim_ptr)->list_v, *tok_ptr->arg);
			}
#ifdef DUMP_ARG /*  Unit test debugging */
			printf ("arg[%d] = %d, err=%d\n", j, scr_val[j], e);
#endif  /*  DUMP_ARG  */
			if (e == ATOI_OUT_OF_RANGE)
			{
				DS_ATCOP_ERROR_MSG();
				ds_atcop_result = DS_ATCOP_PARAM;
				return;
			}
			else if (e == ATOI_OK)
			{
				flag [j] = 1;
				if (scr_val[j]>0 && scr_val[j]<20)
				{
					DS_ATCOP_ERROR_MSG();
					ds_atcop_result = DS_ATCOP_PARAM;
				}
#ifdef DUMP_ARG /*  Unit test debugging */
		  printf (", flag = %d\n", flag [j]);
#endif  /*  DUMP_ARG  */
			/*else if (e == ATOI_NO_ARG) do nothing: flag[j] remains unset  */
			/*    and the value is not stored                               */
			}
		}
		for (j = 0; j < tok_ptr->args_found; ++j)
		{
			if (flag[j])
			{
				* ((ds_atcop_num_item_type *)tab_entry->val_ptr + j)
				  = scr_val [j];
			}
		}
	}
	else                                /*  Didn't make sense - ERROR */
	{
		ds_atcop_result = DS_ATCOP_ERROR;
		DS_ATCOP_ERROR_MSG();
		return;		// khekim 00/1/21
	}
	dsOldCtaVal = ds_atcop_cta_val;
	MSG_HIGH("doAtCopCta()>> ctaVal %d", ds_atcop_cta_val, 0, 0);
} /*  dsAtCopCta  */


//////////////////////////////////////////////////////////////////////////////////////////////
// khekim 00/02/20 parse at+skprev command
void ds_atcop_do_prev
(

  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
  tokens_struct_type *tok_ptr,  /*  Command tokens from parser  */
  byte *rb_ptr                  /*  point to buffer for result  */
)
{
	static const byte bs_prev[][6] = {"1", "2", "3", "4", "5", "6"};
	const byte *s_ptr;
	db_items_value_type item;


	db_get( DB_BS_P_REV, &item );  /* P Rev of BS */

	*rb_ptr = '\0';
	if (tok_ptr->op == (NA|QU))  // READ command at+skprev?
	{
		if (item.bs_p_rev<1 && item.bs_p_rev>6)
		{
			ds_atcop_result = DS_ATCOP_ERROR;
			DS_ATCOP_ERROR_MSG();
			return;
		}
		else
		{
			s_ptr = bs_prev[item.bs_p_rev-1];
			while ( (*rb_ptr++ = *s_ptr++) != 0)
			;
		}
	}
	else                                /*  Didn't make sense - ERROR */
	{
		ds_atcop_result = DS_ATCOP_ERROR;
		DS_ATCOP_ERROR_MSG();
	}
} /*  ds_atcop_do_prev  */


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// dsPktAtCopProcEscape():
//		Packet(PIWF) 서비스의 online state에서 +++ (escape code) detection하면 online command state로 간다.
void	dsPktAtCopProcEscape
(
	void
)
{
	extern	dsm_item_type		*rlp_tx_data_ptr;
	extern	dsm_watermark_type	*rlp_tx_watermark_ptr;
    word           i_save;

	dsm_item_type		*item_ptr;

MSG_ERROR("dsPktAtCopProcEscape: piwf +++ detect", 0, 0, 0);

	dsPktOnCommandState = TRUE;
	ds_atcop_return_cell_result(DS_ATCOP_OK);

    INTLOCK_SAV(i_save); 
   //------------------------------------------------------------------------
   //  Clean up the serial rx watermark.
   //------------------------------------------------------------------------
	(void)dsm_free_buffer( rlp_tx_data_ptr);
	rlp_tx_data_ptr = NULL;

	while(( item_ptr = dsm_dequeue( rlp_tx_watermark_ptr)) != NULL)
	{
		dsm_free_buffer( item_ptr);
	}
	rlp_tx_watermark_ptr = NULL;

   //------------------------------------------------------------------------
   //  Clean up the serial rx watermark.
   //------------------------------------------------------------------------
   while(( item_ptr = dsm_dequeue( &ds_from_sio_wm_item)) != NULL)
   {
     dsm_free_buffer( item_ptr);
   }
   //------------------------------------------------------------------------
   //  Clean up the serial tx watermark.
   //------------------------------------------------------------------------
   while(( item_ptr = dsm_dequeue( &dsi_to_sio_pkt_autodt_wm_item)) != NULL)
   {
     dsm_free_buffer( item_ptr);
   }

   INTFREE_SAV(i_save);

#ifdef FEATURE_NEW_SIO
    DSRMIO_SET_AUTODETECT_MODE( i_save, dsi_sio_ioctl_param);
#else
    DSRMIO_SET_AUTODETECT_MODE();
#endif 

	dsOldSrvcType = dsi_cur_srvc_type;
	dsi_set_srvc_mode( DS_AUTODETECT_SRVC_MODE);

	if (ds_atcop_cta_val != 0)	//	in order not to DS_RLP_IDLE_TIMEOUT_SIG be occurred
	{
        dsmgr_cancel_timer( DS_RLP_IDLE_TIMEOUT_SIG);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// dsPktAtCopProcEscape():
//		Packet(PIWF) 서비스의 online command state에서 ato command를 TE2로부터 수신하면 online state로 간다.
void	goToPktOnLine
(
	void
)
{
	dsi_cur_srvc_type = dsOldSrvcType;
    dsi_set_srvc_mode( DS_PKT_SRVC_MODE);
	ds_set_dcd();
	dsPktOnCommandState = FALSE;
	if (ds_atcop_cta_val != 0)	//	in order to DS_RLP_IDLE_TIMEOUT_SIG be occurred
	{
        INTLOCK();
        dsi_dormant_bitmask = DS_DORMANT_IDLE_MASK;
        INTFREE();
        if (ds_atcop_cta_val != 0)
        {
          MSG_MED("CALL START - starting idle timer",0,0,0);
          dsmgr_start_timer( DS_RLP_IDLE_TIMEOUT_SIG,
                             (ds_atcop_cta_val * 1000)
                           );         
        }
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// uiAnswerDataCall(): UI Task에서 Data Call에 대해서 Any key Answer로 응답하는 루틴
void	uiAnswerDataCall
(
	void
)
{
	ds_srvcmode_type incoming_srvcmode;
	incoming_srvcmode = dsi_call_incoming();
	if (incoming_srvcmode == DS_IS99_CALL)
	{
		MSG_HIGH("Answer Data Call by pressing key so%x", dsi_cur_srvc_type, 0, 0);
//		if (DS_ISDATA_ASYNC(ui.so))
		if (dsi_cur_srvc_type==MC_ASYNC_SRVC || dsi_cur_srvc_type==MC_FAX_SRVC)
		{
			dsi_answer_is99_call();
		}
		else
		{
			MSG_ERROR("Answer but wrong srvc_type %d", dsi_cur_srvc_type, 0, 0);
		}
	}	
#ifdef	FEATURE_SKT_PIWF_DS	// khekim 00/09/14
	else if (incoming_srvcmode == DS_PIWF_CALL && dsi_cur_srvc_type==MC_PIWF_SRVC)
	{
		MSG_HIGH("Answer Data Call by pressing key so%x", dsi_cur_srvc_type, 0, 0);
		dsiAnswerPiwfCall();
	}
#endif	//	#ifdef	FEATURE_SKT_PIWF_DS
	else
	{
		MSG_ERROR("Answer but wrong srvc_type %d", dsi_cur_srvc_type, 0, 0);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// getPageRespServiceOption() page msg수신시 page response msg를 송신할 때 응답해야 할 service option을 구한다.
word	getPageRespServiceOption
(
	word	defaultSO
)
{
	dsi_rateset_type	rateset;

#ifdef FEATURE_DS_IS707A_CMUX
#ifdef	FEATURE_SKT_95C_DS	//	khekim 00/12/11
	if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6)	//	IS-95C
		rateset = (dsi_rateset_type)(ds_atcop_cmux_rev_table[(ds_atcop_cmux_val[IS_95C][1])] - 1);
	else
		rateset = (dsi_rateset_type)(ds_atcop_cmux_rev_table[(ds_atcop_cmux_val[IS_95AB][1])] - 1);
#else	//	#ifdef	FEATURE_SKT_95C_DS
	rateset = (dsi_rateset_type)(ds_atcop_cmux_rev_table[(ds_atcop_cmux_val[1])] - 1);
#endif	//	#ifdef	FEATURE_SKT_95C_DS
#else
	rateset = (dsi_rateset_type)(ds_atcop_cmux_val - 1);
#endif

#ifdef	FEATURE_SKT_PTPAPI	//	khekim 00/03/01
	if (dsi_srvc_mode==DS_AIRDATA_SRVC_MODE
#ifdef	FEATURE_SKT_PIWFPTPAPI
		&& dsAirDataSrvc==DS_ASYNC_SRVC
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
		)
	{
		return dsi_so_async_data[rateset];
	}
	else
#endif	//	#ifdef	FEATURE_SKT_PTPAPI
	if (check_dtr())
	{
		if( dsAtSktDrVal==0 || (ds_rm_pkt_call_ctl_state == DS_PKT_DORM_IDLE_STATE || dsi_protocol_state==DS_PROTOCOL_OPEN))	//  Pkt Dormant State
		{		//	Voice Service
			return	defaultSO;
		}
		else
		{
			if( dsAtSktDrVal==1)	//	Async Service
			{	
				return dsi_so_async_data[rateset];
			}
			else if (dsAtSktDrVal==2)	//	Fax Service
			{
				return	dsi_so_g3_fax[rateset];
			}
			else
			{	//	Voice Service
				return	defaultSO;
			}
		}
	}
	else
	{	
		//	Voice Service
		return	defaultSO;
	}
}


// getPageRespPktServiceOption() page msg수신시 page response msg를 송신할 때 응답해야 할 service option을 구한다.
word	getPageRespPktServiceOption
(
	word	defaultSO
)
{
#ifdef	FEATURE_SKT_ATPKT_DS	//	khekim	00/09/28
	if (DS_ISDATA_PPP_PKT(defaultSO) && defaultSO!=CAI_SO_DATA)	//	Packet SO
	{
		if (  (ds_rm_pkt_call_ctl_state == DS_PKT_DORM_IDLE_STATE)
#ifdef WAP_USE_DORM
			|| (dsi_srvc_mode == DS_BROWSER_PKT_SRVC_MODE && dsi_protocol_state == DS_PROTOCOL_OPEN)
#endif//#ifdef WAP_USE_DORM
			// Please consider Dormant state of socket srvc mode later.
			|| (dsi_srvc_mode == DS_SOCKET_SRVC_MODE && dsi_protocol_state == DS_PROTOCOL_OPEN)
			)
		{
			return	defaultSO;
		}
		else
		{
			return	CAI_SO_REJ;	//	Page Response (SO=0): Page Reject
		}
	}
	else	//	other special service option
#endif	//	#ifdef	FEATURE_SKT_ATPKT_DS
	{
		return	defaultSO;
	}
}

#ifdef	FEATURE_SKT_DS_MMI	//	khekim 00/04/04
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// displayDataBanner(): LCD의 첫번째 라인 Banner에 데이터 착신 관련 display
void	displayDataBanner
(
	boolean	dtr_asserted
)
{
#if defined(MMI_TREX)	
///*
//	dsDtrValue = check_dtr();
//	if (dtr_asserted != dsDtrValue)
//	{
//		MSG_ERROR("dtr value mismatch dtrValue%d", dsDtrValue, 0, 0);
//		dtr_asserted = dsDtrValue;
//	}
//*/
  if (dtr_asserted)
  {
	MSG_HIGH("DTR_ASSERTED: data%d", dsAtSktDrVal, 0, 0);

		if ( dsAtSktDrVal == 1)
		{
			  //uiscrn_bind_act(UI_MODEM_F, "  모뎀착신모드  1");
			  //uiscrn_refresh();

		  }
		else if ( dsAtSktDrVal == 2 )
		{
			  //uiscrn_bind_act(UI_MODEM_F, "  FAX 착신모드  1");
			  //uiscrn_refresh();
		}
		else
		{
			//uiscrn_deact(UI_MODEM_F);
		}
  }
  else
  {
	  {
		  //uiscrn_deact(UI_MODEM_F);
	  }
  }

#else	//	#ifdef	MMI_TREX

	ui_cmd_type *cmd_ptr; 

//
//	dsDtrValue = check_dtr();
//	if (dtr_asserted != dsDtrValue)
//	{
//		MSG_ERROR("dtr value mismatch dtrValue%d", dsDtrValue, 0, 0);
//		dtr_asserted = dsDtrValue;
//	}
//
	if ((cmd_ptr = (ui_cmd_type*) q_get(&ui_cmd_free_q)) != NULL)
	{
		cmd_ptr->hdr.cmd        = UI_DTR_DISPLAY_F;
	}
	else
	{
		ERR_FATAL("No free buffers on ui_cmd_free_q", 0, 0, 0);
	}

	if (dtr_asserted)
	{

		MSG_HIGH("DTR_ASSERTED: data%d", dsAtSktDrVal, 0, 0);

		if ( dsAtSktDrVal == 1)
		{
			// 모뎀착신모드
			cmd_ptr->dtr_disp.mode = 0x01;
			cmd_ptr->hdr.task_ptr   = NULL;
			cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
		}
		else if ( dsAtSktDrVal == 2 )
		{
			// FAX 착신모드
			cmd_ptr->dtr_disp.mode = 0x02;
			cmd_ptr->hdr.task_ptr   = NULL;
			cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
		}
		else
		{ // 음성착신
			cmd_ptr->dtr_disp.mode = 0x00;
			cmd_ptr->hdr.task_ptr   = NULL;
			cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
		}
	}
	else
	{ // DTR OFF
		cmd_ptr->dtr_disp.mode = 0x00;
		cmd_ptr->hdr.task_ptr   = NULL;
		cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
	}

	ui_cmd(cmd_ptr);

#endif	//	#ifdef	MMI_TREX
}
#endif	//	#ifdef	FEATURE_SKT_DS_MMI

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// dsInitAtCmdValue(): DTR OFF시 at+fclass, at&e값을 default 값으로 설정
void	dsInitAtCmdValue
(
	boolean	dtr_asserted
)
{
	if ( dtr_asserted==FALSE && ds_atcop_amp_d_val==2 )
	{
		ds_atcop_fclass_val = 0;
		ds_atcop_e_val = 1;
	}
}

#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
////////////////////////////////////////////////////////////////////////////////////////////////
// SetSerialEnabledMode() 00/07/26
// DS Task가 아닌 UI Task가 DS Task로 WAP이나 NetGame중일 때 Serial Mode를 Enable/Disable하는
// command를 송신
void SetSerialEnabledMode
(
	boolean bSerialEnabledMode
)
{
  ds_cmd_type ds_cmd_item;                    /* item to hold cmd to dsmgr */

  ds_cmd_item.cmd_name = DS_ENABLE_SERIAL_CMD;
  ds_cmd_item.parms.bEnableSerial = bSerialEnabledMode;
  ds_cmd(&ds_cmd_item);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// SetDsSerialEnabledMode() 00/07/26
// DS Task가 DS_BROWSWER_SRVC_MODE, DS_AIRDATA_SRVC_MODE, DS_SOCKET_SRVC_MODE일때 serial mode가 
// Enable/Disable인지를 설정
void SetDsSerialEnabledMode
(
	boolean bSerialEnabledMode
)
{
	if ( bSerialEnabledMode==TRUE)
	{
		if ( dsi_serial_mode != DS_SERIAL_OPENED_MODE)
			dsi_set_serial_mode( DS_SERIAL_OPENED_MODE);
	}
	dsSerialEnabledMode = bSerialEnabledMode;
	MSG_HIGH("dsSerialEnabledMode %d", dsSerialEnabledMode, 0, 0);
}
#endif	//	#ifdef FEATURE_SKT_ENABLE_SERIAL

#ifdef	FEATURE_UI_DORMANT	//	khekim 00/11/09
void SetUIPktRelease
(
	void
)
{
#ifdef	FEATURE_SKT_UIRELEASE_DS	//	khekim 00/04/24
//	if (ds_atcop_crm_val != 2)	//	Relay Layer Model
	{
		MSG_HIGH("UI=>MC: MC_END_F", 0, 0, 0);
		ui_send_mc_cmd ( MC_END_F );
	}
//	else	//	Network Layer Model
//	{
//		ds_cmd_type ds_cmd_item;
//		MSG_HIGH("UI=>DS: DS_UI_RELEASE_CMD", 0, 0, 0);
//		ds_cmd_item.cmd_name = DS_UI_RELEASE_CMD;
//		ds_cmd(&ds_cmd_item);
//	}
#endif	//	#ifdef	FEATURE_SKT_UIRELEASE_DS
}
#endif	//	#ifdef	FEATURE_UI_DORMANT

void	SetIsDormantEnabled
(
	word	srvOpt
)
{
	// DS_IS_DORMANT(srvOpt)
#ifndef	FEATURE_SKT_GVM_ONLINE    
	//if (ds_rm_pkt_call_ctl_state != DS_PKT_DORM_IDLE_STATE && dsi_protocol_state != DS_PROTOCOL_OPEN)	//	int Non-Dormant State
#endif	//	#ifdef	FEATURE_SKT_GVM_ONLINE	
    {
		if (DS_ISDATA(srvOpt))
		{
			if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6 && DS_ISDATA_PPP_PKT(srvOpt) && srvOpt!=CAI_SO_DATA && !DS_ISDATA_RLP_TYPE2(srvOpt))
			{
				dsDormantEnabled = TRUE;
			}
			else
			{
				dsDormantEnabled = FALSE;
			}

			if (dsDormantEnabled==FALSE 
#ifdef	FEATURE_SKT_SOCK_SUP_95AC
				|| dsSockMode==TRUE
#endif	//	#ifdef	FEATURE_SKT_SOCK_SUP_95AC
				)					
			{	
				/*
				SS-QNC, DS-QNC, PIWF, E-mail, IP AirData, GVM
				*/
#ifdef FEATURE_SKVM
				if (SKY_IS_IN_SKVM())
				{
					// DOWNLOAD 모드일 때는 dormant기능 지원 안함 
					if (!SKVM_IS_DOWNLOAD_MODE())
						dsDormantEnabled = TRUE;
					else dsDormantEnabled = FALSE;
				}
				else
#endif // FEATURE_SKVM
#ifdef FEATURE_WAP
				if(	SKY_IS_IN_GVM() )
				{
					dsDormantEnabled = TRUE;

				}else if(SKY_IS_IN_WAP())
				{
					extern boolean wap_dormant_state(void);

					//To prevent 20sec from storing as default value in dormant
					if(wap_dormant_state() == FALSE)
					{
						dsOldCtaVal = ds_atcop_cta_val;
					}

					ds_atcop_cta_val = SKY_GET_WAP_DORMANT_TIME();
					dsDormantEnabled = TRUE;

				}else
#endif//feature_wap
				{
					//	Disable Dormant State
					dsDormantEnabled = FALSE;
					dsOldCtaVal = ds_atcop_cta_val;
					ds_atcop_cta_val = 0;
				}
			}
		}
	}
	MSG_HIGH("DormantEnabled %d", dsDormantEnabled, 0, 0);
}


#ifdef FEATURE_STI_WAP
void SetNAIUserid(void)
{
	if(SKY_GET_SERVICE_PROVIDER() == SP_STI)
	{
		ds_write_auth_info(strlen(dsAuthUserName_95C_STI), dsAuthUserName_95C_STI, strlen(dsAuthPasswd_95C), dsAuthPasswd_95C);	//	shinsegi@wap95C.n-top.com
	}
	else if(SKY_GET_SERVICE_PROVIDER() == SP_SKT)
	{
		ds_write_auth_info(strlen(dsAuthUserName_95C), dsAuthUserName_95C, strlen(dsAuthPasswd_95C), dsAuthPasswd_95C);	//	sktelecom@wap95c.n-top.com
	}
	else
	{
		MSG_ERROR("Not used min2",0,0,0);
	}
}

void SetOriginationNum(void)
{
	db_items_value_type db_use;
	db_get(DB_SID,&db_use);
	if(db_use.sid == 1700)
	{
		memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95A_STI, dsOrigDialStrLen_STI);	//#546*7264#
		//Auth는 사용하지 않으므로 일단 Sktelecom으로 넣어 놓음 
	}
	else
	{
		memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95A, dsOrigDialStrLen);	//	ESC NULL NULL ESC
	}
}

void SetNormal(void)
{
	db_items_value_type db_use;
	db_get(DB_SID,&db_use);
	if(db_use.sid == 1700)
	{
		memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_Normal, dsOrigDialStrLen_Normal);	//	0017
		ds_write_auth_info(strlen(dsAuthUserName_95A_STI), dsAuthUserName_95A_STI, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);//power017
	}
	else
	{
		memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95C, dsOrigDialStrLen);	//	1501
		ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);
	}	
}
#endif//#ifdef FEATURE_STI_WAP


#ifdef	FEATURE_SKT_SOCK_SUP_95AC	//	khekim 00/11/23
void	SetDsEnterSockMode
(
	void
)
{
#ifdef FEATURE_STI_WAP
    db_items_value_type db_use;
#endif //#ifdef FEATURE_STI_WAP

	dsSockMode = TRUE;

#ifdef	FEATURE_GVM
	if (SKY_IS_IN_GVM()==TRUE)
	{
#ifdef	FEATURE_DS_IS2000
		if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6/*IS-95C*/)
		{
			memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95C, dsOrigDialStrLen);	//	1501
#ifdef FEATURE_STI_WAP
			SetNAIUserid();
#else
			ds_write_auth_info(strlen(dsAuthUserName_95C), dsAuthUserName_95C, strlen(dsAuthPasswd_95C), dsAuthPasswd_95C);	//	sktelecom@wap95c.n-top.com
#endif//#ifdef FEATURE_STI_WAP
			if (SKY_IS_IN_GVM()==TRUE)
			{
/* rosa 01-03-01 --> @why, what */
				// Download Server에 접속할 때 SO=33, MO=0x921, Game Server에 접속할 때 SO=33, MO=0x01 접속 필요
				if( g_TypeConnectSvr == GCST_DOWNLOAD )
				{
					/* When Connecting to Download Server, SO=33, MO=0x921 */
					MSG_HIGH("CONNECTING GVM DOWNLOAD Server", 0, 0, 0);
					snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchDefaultFwdMO);
					snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
				}
				else
				{
					/* When Connecting to Game Server, SO=33, MO=0x01 */
					MSG_HIGH("CONNECTING GVM GAME Server", 0, 0, 0);
					snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
					snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
				}
            }
		}
		else
#endif	//	#ifdef	FEATURE_DS_IS2000
		{
#ifdef FEATURE_STI_WAP
			SetOriginationNum();
			ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);	//	sktelecom
#else
			memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95A, dsOrigDialStrLen);	//	ESC NULL NULL ESC
			ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);	//	sktelecom
#endif//#ifdef FEATURE_STI_WAP
		}
	#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
		SetDsSerialEnabledMode(TRUE);
	#endif	//	#ifdef	FEATURE_SKT_ENABLE_SERIAL
	}
	else
#endif	//FEATURE_GVM

#ifdef FEATURE_SKVM
	if (( SKY_IS_IN_SKVM() )
#ifdef FEATURE_SKTT_PM
        || SKY_IS_PM())
#else
        )
#endif //#ifdef FEATURE_SKTT_PM

	{
#ifdef	FEATURE_DS_IS2000
		if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6/*IS-95C*/)
		{
			memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95C, dsOrigDialStrLen);	//	1501
#ifdef FEATURE_STI_WAP
			SetNAIUserid();
#else
			ds_write_auth_info(strlen(dsAuthUserName_95C), dsAuthUserName_95C, strlen(dsAuthPasswd_95C), dsAuthPasswd_95C);	//	sktelecom@wap95c.n-top.com
#endif//	#ifdef FEATURE_STI_WAP
#ifdef FEATURE_SKVM
			if (SKY_IS_IN_SKVM())
			{
				// If Down Load Mode is set,Forward SO:33, MO:0x921, Reverse SO:33, MO:0x1
				// If Down Load Mode isn't set,Forward SO:33, MO:0x01, Reverse SO:33, MO:0x1
				if (SKVM_IS_DOWNLOAD_MODE())
				{
					MSG_HIGH("CONNECTING SKVM DOWNLOAD Server", 0, 0, 0);
					snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchDefaultFwdMO);
				}
				else
				{
					MSG_HIGH("CONNECTING SKVM NET GAME Server", 0, 0, 0);
					snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
				}
				snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);

			}
#else
			{
				// Forward SO:33, MO:0x921
				// Reverse SO:33, MO:0x1
				snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchDefaultFwdMO);
				snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
			}
#endif 
		}
		else
#endif	//	#ifdef	FEATURE_DS_IS2000
		{
#ifdef FEATURE_STI_WAP
			SetOriginationNum();
			ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);	//	sktelecom
#else					
			memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95A, dsOrigDialStrLen);	//	ESC NULL NULL ESC
			ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);	//	sktelecom
#endif//#ifdef FEATURE_STI_WAP
		}

	}
	else
#endif // #ifdef FEATURE_SKVM
#ifdef	FEATURE_WAVELET	//	khekim 01/02/11
	if (SKY_IS_IN_WAVELET()==TRUE)
	{
#ifdef	FEATURE_DS_IS2000
		if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6/*IS-95C*/)
		{
			memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95C, dsOrigDialStrLen);	//	1501
#ifdef FEATURE_STI_WAP
			SetNAIUserid();
#else
			ds_write_auth_info(strlen(dsAuthUserName_95C), dsAuthUserName_95C, strlen(dsAuthPasswd_95C), dsAuthPasswd_95C);	//	sktelecom@wap95c.n-top.com
#endif//	#ifdef FEATURE_STI_WAP
		}
		else
#endif	//	#ifdef	FEATURE_DS_IS2000
		{
#ifdef FEATURE_STI_WAP
			SetOriginationNum();
			ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);	//	sktelecom
#else					
			memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95A, dsOrigDialStrLen);	//	ESC NULL NULL ESC
			ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);	//	sktelecom
#endif//#ifdef FEATURE_STI_WAP
		}	
	}
	else
#endif	//	#ifdef	FEATURE_WAVELET
#ifdef FEATURE_WAP
	if( SKY_IS_IN_WAP() )
	{
#ifdef	FEATURE_DS_IS2000
		if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6/*IS-95C*/)
		{
			memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95C, dsOrigDialStrLen);	//	1501
#ifdef FEATURE_STI_WAP
			SetNAIUserid();
#else
			ds_write_auth_info(strlen(dsAuthUserName_95C), dsAuthUserName_95C, strlen(dsAuthPasswd_95C), dsAuthPasswd_95C);	//	sktelecom@wap95c.n-top.com
#endif//	#ifdef FEATURE_STI_WAP
			// Reverse SO:33, MO:0x01
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
		}
		else
#endif	//	#ifdef	FEATURE_DS_IS2000
		{
#ifdef FEATURE_STI_WAP
			SetOriginationNum();
			ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);	//	sktelecom
#else					
			memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95A, dsOrigDialStrLen);	//	ESC NULL NULL ESC
			ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);	//	sktelecom
#endif//#ifdef FEATURE_STI_WAP
            // Reverse SO:33, MO:0x01
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
		}
#ifdef FEATURE_SKT_EIF 
#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	yjahn96 01.10.11 -- for eif
		SetDsSerialEnabledMode(TRUE);
#endif	//	#ifdef	FEATURE_SKT_ENABLE_SERIAL
#endif//#ifdef FEATURE_SKT_EIF 
	}
	else
#endif//FEATURE_WAP
	{
#ifdef	FEATURE_DS_IS2000
		if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6/*IS-95C*/)
		{
			// Reverse SO:33, MO:0x01
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
//			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
//			snm_set_rc_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchDefaultFwdRC);
		}
#endif	//	#ifdef	FEATURE_DS_IS2000
		// SKTT E-Mail IP-AirData
#ifdef FEATURE_STI_WAP
		SetNormal();
#else
		memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95C, dsOrigDialStrLen);	//	1501
		ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);
#endif//#ifdef FEATURE_STI_WAP
	}
}

void	SetDsExitSockMode
(
	void
)
{
	memcpy(ds_atcop_pkt_orig_str,dsOrigDialStr_95C, dsOrigDialStrLen);
	ds_write_auth_info(strlen(dsAuthUserName_95A), dsAuthUserName_95A, strlen(dsAuthPasswd_95A), dsAuthPasswd_95A);
	dsSockMode = FALSE;	
	//dsNetGameMode = FALSE;
#ifdef	FEATURE_DS_IS2000
#ifdef	FEATURE_GVM
#ifdef	FEATURE_SKT_GVM_ONLINE	//	khkeim 01/04/16
#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO	//	khekim 01/02/18    
	if (
#if MMI_TYPE == SKY_UI	//	khekim 01/02/02
		SKY_IS_IN_GVM()==TRUE
#else	//	#if MMI_TYPE == SKY_UI
		ui.netgame==TRUE
#endif	//	#if MMI_TYPE == SKY_UI
		&& SKY_IS_TO_WAP()==TRUE && (SKY_IS_SOCK_CONNECT()==TRUE|| SKY_IS_FROM_WAP()==TRUE))
	{
		SetMOinSO33(0);	//	WAP: MO=0x01
	}
	else
#endif	//	#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO
#endif	//	#ifdef	FEATURE_SKT_GVM_ONLINE
#endif	//	#ifdef	FEATURE_GVM
    {
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchDefaultFwdMO);
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchDefaultFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchDefaultFwdMO);
    }
#endif	//	#ifdef	FEATURE_DS_IS2000
}

#endif	//	#ifdef	FEATURE_SKT_SOCK_SUP_95AC

#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO	//	khekim 01/02/18
void	SetMOinSO33
(
	word wNewFwdMuxOpt
)
{

#ifdef	FEATURE_DS_IS2000
	word	wCurFwdMuxOpt, wCurRevMuxOpt;
	if (ds_socfg_get_p_rev_index()<DS_BS_PREV_6/*IS-95C*/)
		return;

	snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);

	GetCurrentMuxOptionFromSO(CAI_SO_PPP_PKT_DATA_3G, &wCurFwdMuxOpt, &wCurRevMuxOpt);

	MSG_HIGH("SetMO curMO%x new MO%x", wCurFwdMuxOpt, wNewFwdMuxOpt, 0);

	if (wCurFwdMuxOpt==wNewFwdMuxOpt)
	{
		return;
	}

	switch (wNewFwdMuxOpt)
	{
	case CAI_MUX_16X_RS1_DOUBLE:/* MuxPDU 3, 16x, double sch     */
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchDefaultFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchDefaultFwdMO);
		break;
	case 0:/* MO=0x01   */
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
		break;
	case CAI_MUX_8X_RS1_DOUBLE:/* MuxPDU 3,  8x, double sch     */
		so33SchMedFwdMO.num_mos = 7;
		so33SchMedFwdMO.list[0] = CAI_MUX_8X_RS1_DOUBLE;
		so33SchMedFwdMO.list[1] = CAI_MUX_8X_RS1_SINGLE;
		so33SchMedFwdMO.list[2] = CAI_MUX_4X_RS1_DOUBLE;
		so33SchMedFwdMO.list[3] = CAI_MUX_4X_RS1_SINGLE;
		so33SchMedFwdMO.list[4] = CAI_MUX_2X_RS1_DOUBLE;
		so33SchMedFwdMO.list[5] = CAI_MUX_2X_RS1_SINGLE;
		so33SchMedFwdMO.list[6] = CAI_MUX_1X_RS1;
		so33SchMedFwdMO.list[7] = 0;
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
		break;
	case CAI_MUX_8X_RS1_SINGLE:/* MuxPDU 3,  8x, single sch     */
		so33SchMedFwdMO.num_mos = 6;
		so33SchMedFwdMO.list[0] = CAI_MUX_8X_RS1_SINGLE;
		so33SchMedFwdMO.list[1] = CAI_MUX_4X_RS1_DOUBLE;
		so33SchMedFwdMO.list[2] = CAI_MUX_4X_RS1_SINGLE;
		so33SchMedFwdMO.list[3] = CAI_MUX_2X_RS1_DOUBLE;
		so33SchMedFwdMO.list[4] = CAI_MUX_2X_RS1_SINGLE;
		so33SchMedFwdMO.list[5] = CAI_MUX_1X_RS1;
		so33SchMedFwdMO.list[6] = 0;
		so33SchMedFwdMO.list[7] = 0;
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
		break;
    case CAI_MUX_4X_RS1_DOUBLE:/* MuxPDU 3,  4x, double sch     */
		so33SchMedFwdMO.num_mos = 5;
		so33SchMedFwdMO.list[0] = CAI_MUX_4X_RS1_DOUBLE;
		so33SchMedFwdMO.list[1] = CAI_MUX_4X_RS1_SINGLE;
		so33SchMedFwdMO.list[2] = CAI_MUX_2X_RS1_DOUBLE;
		so33SchMedFwdMO.list[3] = CAI_MUX_2X_RS1_SINGLE;
		so33SchMedFwdMO.list[4] = CAI_MUX_1X_RS1;
		so33SchMedFwdMO.list[5] = 0;
		so33SchMedFwdMO.list[6] = 0;
		so33SchMedFwdMO.list[7] = 0;
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
		break;
	case CAI_MUX_4X_RS1_SINGLE:/* MuxPDU 3,  4x, single sch     */
		so33SchMedFwdMO.num_mos = 4;
		so33SchMedFwdMO.list[0] = CAI_MUX_4X_RS1_SINGLE;
		so33SchMedFwdMO.list[1] = CAI_MUX_2X_RS1_DOUBLE;
		so33SchMedFwdMO.list[2] = CAI_MUX_2X_RS1_SINGLE;
		so33SchMedFwdMO.list[3] = CAI_MUX_1X_RS1;
		so33SchMedFwdMO.list[4] = 0;
		so33SchMedFwdMO.list[5] = 0;
		so33SchMedFwdMO.list[6] = 0;
		so33SchMedFwdMO.list[7] = 0;
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
		break;
	case CAI_MUX_2X_RS1_DOUBLE:/* MuxPDU 3,  2x, double sch     */
		so33SchMedFwdMO.num_mos = 3;
		so33SchMedFwdMO.list[0] = CAI_MUX_2X_RS1_DOUBLE;
		so33SchMedFwdMO.list[1] = CAI_MUX_2X_RS1_SINGLE;
		so33SchMedFwdMO.list[2] = CAI_MUX_1X_RS1;
		so33SchMedFwdMO.list[3] = 0;
		so33SchMedFwdMO.list[4] = 0;
		so33SchMedFwdMO.list[5] = 0;
		so33SchMedFwdMO.list[6] = 0;
		so33SchMedFwdMO.list[7] = 0;
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
		break;
	case CAI_MUX_2X_RS1_SINGLE:/* MuxPDU 3,  2x, single sch     */
		so33SchMedFwdMO.num_mos = 2;
		so33SchMedFwdMO.list[0] = CAI_MUX_2X_RS1_SINGLE;
		so33SchMedFwdMO.list[1] = CAI_MUX_1X_RS1;
		so33SchMedFwdMO.list[2] = 0;
		so33SchMedFwdMO.list[3] = 0;
		so33SchMedFwdMO.list[4] = 0;
		so33SchMedFwdMO.list[5] = 0;
		so33SchMedFwdMO.list[6] = 0;
		so33SchMedFwdMO.list[7] = 0;
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
		break;
	case CAI_MUX_1X_RS1:/* MuxPDU 3,  2x, single sch     */
		so33SchMedFwdMO.num_mos = 1;
		so33SchMedFwdMO.list[0] = CAI_MUX_1X_RS1;
		so33SchMedFwdMO.list[1] = 0;
		so33SchMedFwdMO.list[2] = 0;
		so33SchMedFwdMO.list[3] = 0;
		so33SchMedFwdMO.list[4] = 0;
		so33SchMedFwdMO.list[5] = 0;
		so33SchMedFwdMO.list[6] = 0;
		so33SchMedFwdMO.list[7] = 0;
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
		break;
	default:
		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
	}

//	snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
//	snm_set_rc_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchDefaultFwdRC);
	MSG_HIGH("SetMOinSO33 MO %x MC_INTRAFFIC_MONEGO_F", wNewFwdMuxOpt, 0, 0);

	if (dsi_callstate == DS_CALL_ACTIVE_CSTATE && ds_socfg_get_p_rev_index()>=DS_BS_PREV_6/*IS-95C*/)
	{
		mc_msg_type   *mc_cmd_ptr;              /* for commanding MC             */

		mc_cmd_ptr = dsi_get_mc_cmd();
		mc_cmd_ptr->hdr.cmd = MC_INTRAFFIC_MONEGO_F;
		mc_cmd( mc_cmd_ptr);
	}
#endif	//	#ifdef	FEATURE_DS_IS2000
}
#endif	//	#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO

#ifdef FEATURE_WAP
void	SetMOinSO33toMIN(void)
{
	if (ds_socfg_get_p_rev_index()<DS_BS_PREV_6/*IS-95C*/)
		return;
	snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
    snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
}

#endif //#ifdef FEATURE_WAP
#ifdef	FEATURE_SKT_NDSS_DS	//	khekim 01/04/11
extern void ds_send_ui_cmd(ui_name_type);
void SetDSOriginationByNDSS
(
	byte	*pszCalledAddress,
	byte	*pnDialCnt,
	boolean *pfDigitMode,
	word	*pwSrvOpt

)
{
	dsi_rateset_type       rateset;       /* Stores the rateset for the call */
    word p_rev_index;

#ifdef FEATURE_STI_WAP
    db_items_value_type db_use;
#endif //#ifdef FEATURE_STI_WAP
    
	if (DS_ISDATA(*pwSrvOpt)==FALSE)
	{
		return;
	}

#ifdef FEATURE_DS_IS707A_CMUX
	rateset = (dsi_rateset_type)(ds_atcop_cmux_rev_table[(ds_atcop_cmux_val[1])] - 1);
#else
	rateset = (dsi_rateset_type)(ds_atcop_cmux_val - 1);
#endif

	MSG_ERROR("rateset = %d dsSOCKMODE= %d", rateset, dsSockMode,0);
#ifdef	FEATURE_SKT_SOCK_SUP_95AC	//	khekim 00/11/23
	if ( dsSockMode==TRUE )	//	Global Variable I know Wrong Coding Custom
	{	//	E-mail, Netgame, IP Airdata

		SetDsEnterSockMode();	//	PPP username/password, Multiplex Option Set

#ifdef FEATURE_PAL_NET_SOCKAPI	//khekim 01.08.16
		if (PAL_IS_SKP_OPRT_MODE() && g_PacketParam!=NULL)
		{
			memcpy((byte*)pszCalledAddress, 
					(byte*)g_PacketParam[PAL_net_GetBSSYS()].aDialDigits, *pnDialCnt);
			*pnDialCnt = g_PacketParam[PAL_net_GetBSSYS()].nDialCnt;
			if (g_PacketParam[PAL_net_GetBSSYS()].fASCII==TRUE)
				*pfDigitMode = CM_DIGIT_MODE_8BIT_ASCII;
			else
				*pfDigitMode = CM_DIGIT_MODE_4BIT_DTMF;
		}
		else
#endif /*FEATURE_PAL_NET_SOCKAPI*/
		if (
			FALSE
#ifdef	FEATURE_GVM	//	khekim 01/04/11
			|| SKY_IS_IN_GVM()==TRUE
#endif /*FEATURE_GVM*/
#ifdef FEATURE_SKTT_PM
			|| SKY_IS_PM()
#endif /*FEATURE_SKTT_PM*/
#ifdef FEATURE_WAP
            || SKY_IS_IN_WAP()
#endif //#ifdef FEATURE_WAP
			)
		{
			if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6/*IS-95C*/)
			{
				*pfDigitMode = CM_DIGIT_MODE_4BIT_DTMF;
				*pnDialCnt = dsOrigDialStrLen;	//	1501=>4
				memcpy((byte*)pszCalledAddress, dsOrigDialStr_95C, dsOrigDialStrLen);
			}
			else
			{
#ifdef FEATURE_STI_WAP
                db_get(DB_SID,&db_use);
                if(db_use.sid == 1700)
                {
                    *pfDigitMode  = CM_DIGIT_MODE_4BIT_DTMF;
			        *pnDialCnt = dsOrigDialStrLen_STI;	//	sizeof(ESC NULL NULL ESC) => 4
			        memcpy((byte*)pszCalledAddress,dsOrigDialStr_95A_STI, dsOrigDialStrLen_STI);	//	#546*7264#
                }
                else            
#endif //#ifdef FEATURE_STI_WAP
                {
				    *pfDigitMode  = CM_DIGIT_MODE_8BIT_ASCII;
				    *pnDialCnt = dsOrigDialStrLen;			//	ESC NULL NULL ESC
				    memcpy((byte*)pszCalledAddress, dsOrigDialStr_95A, dsOrigDialStrLen);
                }
			}
		}
		else
		{
			*pfDigitMode = CM_DIGIT_MODE_4BIT_DTMF;
			*pnDialCnt = dsOrigDialStrLen;			//1501
			memcpy((byte*)pszCalledAddress, dsOrigDialStr_95C, dsOrigDialStrLen);
		}

		if (ds_socfg_get_p_rev_index()>=DS_BS_PREV_6/*IS-95C*/)
		{
			*pwSrvOpt = ds_socfg_pkt_orig_so_tbl[DS_BS_PREV_6]
												  [DS_MDR_MODE_SO33_PREF]
												  [DS_SO_SET_PRE707]
												  [rateset];	//	SO=33	
		}
		else
		{
			// SO 15 or 7
			//*pwSrvOpt = dsi_so_ppp_pkt_data[rateset];
			// SO 12 or 4
			*pwSrvOpt = dsi_so_async_data[rateset];
		}
		if (dsi_callstate == DS_CALL_ACTIVE_CSTATE)
		{
			ps_process_pzid();
			dsi_cleanup_queues();                
			dsi_set_callstate(DS_ORIG_CALL_CSTATE);
			dsi_protocol_state = DS_PROTOCOL_CLOSED;
		}
	}
	else
#endif	//	#ifdef	FEATURE_SKT_SOCK_SUP_95AC
	{	// not socket mode and not wap mode
	    MSG_HIGH("CRM=%d COP =%d", ds_atcop_crm_val, ds_atcop_qcmdr_val, 0);

		if (ds_atcop_crm_val==150)	//	origination with so 33 unconditionally
		{
		  *pwSrvOpt = ds_socfg_pkt_orig_so_tbl[DS_BS_PREV_6]
		  								    [DS_MDR_MODE_SO33_PREF]
											[DS_SO_SET_PRE707]
											[rateset];
// CM Task에서 CONNECT EVENT를 DS Task로 보냈더니 문제가 발생 하였음
// 일단 B1은 여기서 Dormant Enable 해주고 B2에서 문제 해결 해야함.
		  if( dsDormantEnabled == FALSE )
		  {
		    dsDormantEnabled = TRUE;
		    ds_atcop_cta_val = dsOldCtaVal;
		  }
		}
		else
		{
			if (ds_atcop_crm_val==1 || ds_atcop_crm_val==2)
			{
			  p_rev_index = ds_socfg_get_p_rev_index();
			  if (p_rev_index>=DS_BS_PREV_6)
			  {
				*pwSrvOpt = ds_socfg_pkt_orig_so_tbl[p_rev_index]
		   										  [ds_atcop_qcmdr_val]
												  [DS_SO_SET_PRE707]
		   										  [rateset];
// bwpark CM Task에서 CONNECT EVENT를 DS Task로 보냈더니 문제가 발생 하였음
// 일단 B1은 여기서 Dormant Enable 해주고 B2에서 문제 해결 해야함.
			    if( dsDormantEnabled == FALSE )
				{
			      dsDormantEnabled = TRUE;
			      ds_atcop_cta_val = dsOldCtaVal;
				}
			  }
			  else //	DS_BS_PREV_3, DS_BS_PREV_2
			  {
				*pwSrvOpt = dsi_so_ppp_pkt_data[rateset];	// Packet SO:7,15
			  }
			}
			// in ds_atcop_crm==131, no change
		}
		  
//		if( cdma.entry_ptr->param.orig.service_option == 0x21 )
//		{
//		  MSG_HIGH("send UI_NDSS_RECONNECT_F SO%d %d", cdma.entry_ptr->param.orig.service_option,
//		      	                                       dsOldCtaVal,0);
// 이거 임시로 막았음 풀던가 다른 대안을 마련해야 함 		  ds_send_ui_cmd(UI_NDSS_RECONNECT_F);
//		}
	}

}
#endif	//	#ifdef	FEATURE_SKT_NDSS_DS


boolean	SetSubMuxOptOfSrvOpt
(
	word wSrvOpt,
	word wFwdSubMuxOpt,
	word wRevSubMuxOpt
)
{
	word	wCurFwdMuxOpt, wCurRevMuxOpt;
	boolean fResult;


	GetCurrentMuxOptionFromSO(wSrvOpt, &wCurFwdMuxOpt, &wCurRevMuxOpt);

	MSG_HIGH("SetMO curFwdMO%x RevMO%x new MO%x", wCurFwdMuxOpt, wCurRevMuxOpt, wFwdSubMuxOpt);

	if (wCurFwdMuxOpt==wFwdSubMuxOpt && wCurRevMuxOpt==wRevSubMuxOpt)
	{
		return TRUE;
	}

	if (wSrvOpt==CAI_SO_MDR_PKT_DATA_FRS1_RRS1 || wSrvOpt==CAI_SO_MDR_PKT_DATA_FRS2_RRS2)
	{
#if (defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)) && defined(FEATURE_DS_IS707A_CMUX)

//		if (dsi_callstate == DS_CALL_ACTIVE_CSTATE)
//			return FALSE;
		/*-------------------------------------------------------------------------
		Set the maximum forward supplementals for MDR service options, based on
		the AT+CMUX default value.
		-------------------------------------------------------------------------*/
		if (wFwdSubMuxOpt > 15 || wFwdSubMuxOpt<1 || wRevSubMuxOpt!=0)
			return FALSE;

		wCurFwdMuxOpt = ds_atcop_cmux_fwd_table[wFwdSubMuxOpt-1];

		if( (wCurFwdMuxOpt & 0x01) && wSrvOpt==CAI_SO_MDR_PKT_DATA_FRS1_RRS1)
		{
			/*-----------------------------------------------------------------------
			  Odd multiplex option. Set the maximum number of forward supplementals
			  for the Rate Set 1 MDR Packet Data service option (i.e. SO 22).
			-----------------------------------------------------------------------*/
			snm_set_max_for_sups(CAI_SO_MDR_PKT_DATA_FRS1_RRS1, (byte)((wCurFwdMuxOpt-1)/2));
			fResult = TRUE;
		}
		else if (!(wCurFwdMuxOpt & 0x01) && wSrvOpt==CAI_SO_MDR_PKT_DATA_FRS2_RRS2)
		{
			/*-----------------------------------------------------------------------
			  Even multiplex option. Set the maximum number of forward supplementals
			  for the Rate Set 2 MDR Packet Data service option (i.e. SO 25).
			-----------------------------------------------------------------------*/
			snm_set_max_for_sups(CAI_SO_MDR_PKT_DATA_FRS2_RRS2, (byte)((wCurFwdMuxOpt-2)/2));
			fResult = TRUE;
		}
		else
		{
			fResult = FALSE;
		}

		if (fResult==TRUE && dsi_callstate == DS_CALL_ACTIVE_CSTATE && cdma.so_cur_pri==wSrvOpt)
		{
			mc_msg_type   *mc_cmd_ptr;	/*for commanding MC*/

			MSG_HIGH("SetSubMuxOptOfSrvOpt FwdMO%x RevMO%x MC_INTRAFFIC_MONEGO_F", wFwdSubMuxOpt, wRevSubMuxOpt, 0);

			mc_cmd_ptr = dsi_get_mc_cmd();
			mc_cmd_ptr->hdr.cmd = MC_INTRAFFIC_MONEGO_F;
			mc_cmd( mc_cmd_ptr);
		}
		else
		{
			MSG_HIGH("SetSubMuxOptOfSrvOpt FwdMO%x RevMO%x", wFwdSubMuxOpt, wRevSubMuxOpt, 0);
		}

		return fResult;

#else /* (FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR) && FEATURE_DS_IS707A_CMUX */
		return FALSE;
#endif  /* ! (FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR) && FEATURE_DS_IS707A_CMUX */
	}
	else 
#ifdef	FEATURE_DS_IS2000
	if (wSrvOpt==CAI_SO_PPP_PKT_DATA_3G)
	{
		switch (wFwdSubMuxOpt)
		{
		case CAI_MUX_16X_RS1_DOUBLE:/* MuxPDU 3, 16x, double sch     */
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchDefaultFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchDefaultFwdMO);
			break;
		case 0:/* MO=0x01   */
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
			break;
		case CAI_MUX_8X_RS1_DOUBLE:/* MuxPDU 3,  8x, double sch     */
			so33SchMedFwdMO.num_mos = 7;
			so33SchMedFwdMO.list[0] = CAI_MUX_8X_RS1_DOUBLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_8X_RS1_SINGLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_4X_RS1_DOUBLE;
			so33SchMedFwdMO.list[3] = CAI_MUX_4X_RS1_SINGLE;
			so33SchMedFwdMO.list[4] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[5] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[6] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_8X_RS1_SINGLE:/* MuxPDU 3,  8x, single sch     */
			so33SchMedFwdMO.num_mos = 6;
			so33SchMedFwdMO.list[0] = CAI_MUX_8X_RS1_SINGLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_4X_RS1_DOUBLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_4X_RS1_SINGLE;
			so33SchMedFwdMO.list[3] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[4] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[5] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_4X_RS1_DOUBLE:/* MuxPDU 3,  4x, double sch     */
			so33SchMedFwdMO.num_mos = 5;
			so33SchMedFwdMO.list[0] = CAI_MUX_4X_RS1_DOUBLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_4X_RS1_SINGLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[3] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[4] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_4X_RS1_SINGLE:/* MuxPDU 3,  4x, single sch     */
			so33SchMedFwdMO.num_mos = 4;
			so33SchMedFwdMO.list[0] = CAI_MUX_4X_RS1_SINGLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[3] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[4] = 0;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_2X_RS1_DOUBLE:/* MuxPDU 3,  2x, double sch     */
			so33SchMedFwdMO.num_mos = 3;
			so33SchMedFwdMO.list[0] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[3] = 0;
			so33SchMedFwdMO.list[4] = 0;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_2X_RS1_SINGLE:/* MuxPDU 3,  2x, single sch     */
			so33SchMedFwdMO.num_mos = 2;
			so33SchMedFwdMO.list[0] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[2] = 0;
			so33SchMedFwdMO.list[3] = 0;
			so33SchMedFwdMO.list[4] = 0;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_1X_RS1:/* MuxPDU 3,  2x, single sch     */
			so33SchMedFwdMO.num_mos = 1;
			so33SchMedFwdMO.list[0] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[1] = 0;
			so33SchMedFwdMO.list[2] = 0;
			so33SchMedFwdMO.list[3] = 0;
			so33SchMedFwdMO.list[4] = 0;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMedFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		default:
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_FORWARD, &so33SchMinFwdMO);
	//		snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
		}

		switch (wRevSubMuxOpt)
		{
		case CAI_MUX_16X_RS1_DOUBLE:/* MuxPDU 3, 16x, double sch     */
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchDefaultFwdMO);
			break;
		case 0:/* MO=0x01   */
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
			break;
		case CAI_MUX_8X_RS1_DOUBLE:/* MuxPDU 3,  8x, double sch     */
			so33SchMedFwdMO.num_mos = 7;
			so33SchMedFwdMO.list[0] = CAI_MUX_8X_RS1_DOUBLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_8X_RS1_SINGLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_4X_RS1_DOUBLE;
			so33SchMedFwdMO.list[3] = CAI_MUX_4X_RS1_SINGLE;
			so33SchMedFwdMO.list[4] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[5] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[6] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_8X_RS1_SINGLE:/* MuxPDU 3,  8x, single sch     */
			so33SchMedFwdMO.num_mos = 6;
			so33SchMedFwdMO.list[0] = CAI_MUX_8X_RS1_SINGLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_4X_RS1_DOUBLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_4X_RS1_SINGLE;
			so33SchMedFwdMO.list[3] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[4] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[5] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_4X_RS1_DOUBLE:/* MuxPDU 3,  4x, double sch     */
			so33SchMedFwdMO.num_mos = 5;
			so33SchMedFwdMO.list[0] = CAI_MUX_4X_RS1_DOUBLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_4X_RS1_SINGLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[3] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[4] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_4X_RS1_SINGLE:/* MuxPDU 3,  4x, single sch     */
			so33SchMedFwdMO.num_mos = 4;
			so33SchMedFwdMO.list[0] = CAI_MUX_4X_RS1_SINGLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[3] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[4] = 0;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_2X_RS1_DOUBLE:/* MuxPDU 3,  2x, double sch     */
			so33SchMedFwdMO.num_mos = 3;
			so33SchMedFwdMO.list[0] = CAI_MUX_2X_RS1_DOUBLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[2] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[3] = 0;
			so33SchMedFwdMO.list[4] = 0;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_2X_RS1_SINGLE:/* MuxPDU 3,  2x, single sch     */
			so33SchMedFwdMO.num_mos = 2;
			so33SchMedFwdMO.list[0] = CAI_MUX_2X_RS1_SINGLE;
			so33SchMedFwdMO.list[1] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[2] = 0;
			so33SchMedFwdMO.list[3] = 0;
			so33SchMedFwdMO.list[4] = 0;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		case CAI_MUX_1X_RS1:/* MuxPDU 3,  2x, single sch     */
			so33SchMedFwdMO.num_mos = 1;
			so33SchMedFwdMO.list[0] = CAI_MUX_1X_RS1;
			so33SchMedFwdMO.list[1] = 0;
			so33SchMedFwdMO.list[2] = 0;
			so33SchMedFwdMO.list[3] = 0;
			so33SchMedFwdMO.list[4] = 0;
			so33SchMedFwdMO.list[5] = 0;
			so33SchMedFwdMO.list[6] = 0;
			so33SchMedFwdMO.list[7] = 0;
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMedFwdMO);
			break;
		default:
			snm_set_sch_mux_list(CAI_SO_PPP_PKT_DATA_3G, SNM_REVERSE, &so33SchMinFwdMO);
		}

		if (dsi_callstate == DS_CALL_ACTIVE_CSTATE && cdma.so_cur_pri==wSrvOpt)
		{
			mc_msg_type   *mc_cmd_ptr;	/*for commanding MC*/

			MSG_HIGH("SetSubMuxOptOfSrvOpt FwdMO%x RevMO%x MC_INTRAFFIC_MONEGO_F", wFwdSubMuxOpt, wRevSubMuxOpt, 0);

			mc_cmd_ptr = dsi_get_mc_cmd();
			mc_cmd_ptr->hdr.cmd = MC_INTRAFFIC_MONEGO_F;
			mc_cmd( mc_cmd_ptr);
		}
		else
		{
			MSG_HIGH("SetSubMuxOptOfSrvOpt FwdMO%x RevMO%x", wFwdSubMuxOpt, wRevSubMuxOpt, 0);
		}
		return TRUE;
	}
	else
#endif	//	#ifdef	FEATURE_DS_IS2000
	{
		if (dsi_callstate == DS_CALL_ACTIVE_CSTATE)
			return FALSE;
		if (wFwdSubMuxOpt!=0 && wRevSubMuxOpt!=0)
			return FALSE;
		return TRUE;
	}
}

#endif	//	#ifdef	FEATURE_SKT_DS	//	khekim 00/01/21
