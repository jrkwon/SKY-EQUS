#ifndef	DSSKT_H
#define	DSSKT_H

#include	<stdio.h>
#include	"customer.h"
#include	"comdef.h"
#include	"target.h"
#include	"ds.h"
#include	"dsi.h"
#include	"dsatcopi.h"
#include	"uisockApi.h"

#ifdef	FEATURE_SKT_DS	//	khekim 00/02/25
#ifdef	FEATURE_SKT_95C_DS
#define	IS_95AB				0
#define	IS_95C				1
#define	NUM_OF_IS_95ABC		2
//typedef	enum
//{
//	IS_95AB=0,
//	IS_95C,
//	NUM_OF_IS_95ABC
//}	CDMA_SERVICE;
extern	const def_list_type ds_atcop_cmux_list[NUM_OF_IS_95ABC][2];
#endif	//	#ifdef	FEATURE_SKT_95C_DS

//////////////////////////////////////////////////////////////////////////////////////////////
// EXTERNAL VARIABLES
//////////////////////////////////////////////////////////////////////////////////////////////
// dsi_srvc_mode and dsi_cur_srvc_type is defined in dsmgr.c
extern	dsi_data_srvc_enum_type		dsi_srvc_mode;
extern	mc_srvc_req_enum_type		dsi_cur_srvc_type;
extern	dsi_serial_mode_enum_type   dsi_serial_mode;  /* Current serial mode */
extern	boolean dsi_no_carrier_txed;

//	check_dtr() is defined in dsmgr.c
extern boolean	check_dtr(void);	

//////////////////////////////////////////////////////////////////////////////////////////////
// SKT-specific variables
extern	boolean	dsConnectMode;
extern	boolean	dsPktOnCommandState;
extern	boolean	dsDtrValue;
#ifdef	FEATURE_SKT_CANCEL_CALL	//	khekim 00/02/24
extern	boolean	dsCalling;
#endif	//	#ifdef	FEATURE_SKT_CANCEL_CALL
#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
extern	boolean dsSerialEnabledMode;
#endif	//	#ifdef	FEATURE_SKT_ENABLE_SERIAL
extern	ds_atcop_num_item_type	dsOldCtaVal;	//	khekim 00/09/19

extern	mc_srvc_req_enum_type		dsOldSrvcType;
extern	mc_srvc_req_enum_type		dsAtSrvcType;

// dsAtSktDrVal is defined in dsatskt.c
extern	ds_atcop_num_item_type dsAtSktDrVal;

#ifdef	FEATURE_SKT_DS_REDIAL
extern	boolean dsRedialing;
extern	boolean	dsEnableRedial;
#endif	//	#ifdef	FEATURE_SKT_DS_REDIAL
#ifdef	FEATURE_SKT_PIWFPTPAPI	//	khekim 00/08/28
extern	DS_SRVC_TYPE	dsAirDataSrvc;
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
#ifdef	FEATURE_RM_DORMANT	//	khekim 00/11/13
extern	boolean dsDtrOffOnDormant;
#endif	//	#ifdef	FEATURE_RM_DORMANT

#ifdef	FEATURE_SKT_SOCK_SUP_95AC	//	khekim 00/11/22
extern	byte	dsOrigDialStrLen;
extern	const	byte	dsOrigDialStr_95A[];
extern	const	byte	dsOrigDialStr_95C[];
extern	char	*dsAuthUserName_95A;
extern	char	*dsAuthPasswd_95A;
#ifdef	FEATURE_DS_IS2000
extern	char	*dsAuthUserName_95C;
extern	char	*dsAuthPasswd_95C;
#endif	//	#ifdef	FEATURE_DS_IS2000
extern	boolean	dsSockMode;
#endif	//	#ifdef	FEATURE_SKT_SOCK_SUP_95AC
#ifdef	FEATURE_SKT_SIMPLEIP	//	khekim 00/11/22
extern	boolean	dsPPPRelayResynced;
extern	boolean	dsPZIDChanged;
#endif	//	#ifdef	FEATURE_SKT_SIMPLEIP
extern	boolean	dsDormantEnabled;
extern  word    dsSrvcOpt;

#ifdef FEATURE_STI_DS
extern byte	dsOrigDialStrLen_STI;
extern char	*dsAuthUserName_STI_95C;
extern const byte	dsOrigDialStr_STI_95A[];
extern boolean dsIsSTISID(void);        // reo 2001/10/16
extern boolean dsIsSTINumber(void);	    // reo 2001/10/16
#endif //#ifdef FEATURE_STI_DS

#ifdef FEATURE_STI_WAP
#ifndef FEATURE_STI_DS
extern byte	dsOrigDialStrLen_STI;
#endif //NOt FEATURE_STI_DS
extern  byte	dsOrigDialStrLen_Normal;
extern  const	byte	dsOrigDialStr_95A_STI[];	//2G용 STI번호(단,GVM, PM,WAP)일시만 사용 
extern  const	byte	dsOrigDialStr_Normal[];	//	1501

extern  char	*dsAuthUserName_95C_STI;//2.5G STI NAI
extern  char	*dsAuthUserName_95A_STI;//2G일시 사용하며,GVM,WAP,PM일시는 PAP자체를 사용하지 않음 
#endif//#ifdef FEATURE_STI_WAP
//////////////////////////////////////////////////////////////////////////////////////////////
// EXTERNAL FUNCTIONS
extern	void	dsInitVariables
(
	void
);

extern	void	setPktDsConnect
(
	void
);

extern	void	setRawDsConnect
(
	void
);

extern	mc_srvc_req_enum_type	setDsAtSrvcTypeOnIdle
(
	void
);

extern	void	setDsAtSrvcTypeOnAlert
(
	void
);

extern	void doAtCopCrm
(
  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
  tokens_struct_type *tok_ptr,  /*  Command tokens from parser  */
  byte *rb_ptr                  /*  point to buffer for result  */
);

extern	void doAtCopCta
(
  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
  tokens_struct_type *tok_ptr,  /*  Command tokens from parser  */
  byte *rb_ptr                  /*  point to buffer for result  */
);

extern void ds_atcop_do_prev
(

  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
  tokens_struct_type *tok_ptr,  /*  Command tokens from parser  */
  byte *rb_ptr                  /*  point to buffer for result  */
);

extern	void	dsPktAtCopProcEscape
(
	void
);

extern	void	goToPktOnLine
(
	void
);

extern	void	uiAnswerDataCall
(	
	void
);

extern	word	getPageRespServiceOption
(
	word	defaultSO
);

extern	word	getPageRespPktServiceOption
(
	word	defaultSO
);

extern	void	displayFirstDataBanner
(
	boolean	dtr_asserted
);

extern	void	displayDataBanner
(
	boolean	dtr_asserted
);

extern	void	dsInitAtCmdValue
(
	boolean	dtr_asserted
);
#endif	//	#ifdef	FEATURE_SKT_DS	//	khekim 00/02/25

#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
extern void SetSerialEnabledMode
(
	boolean bSerialEnabledMode
);

extern void SetDsSerialEnabledMode
(
	boolean bEnableEnabledMode
);
#endif	//	#ifdef	FEATURE_SKT_ENABLE_SERIAL

#ifdef	FEATURE_UI_DORMANT	//	khekim 00/11/09
extern void SetUIPktRelease
(
	void
);
#endif	//	#ifdef	FEATURE_UI_DORMANT

extern	void SetIsDormantEnabled
(
	word	srvOpt
);

#ifdef	FEATURE_SKT_SOCK_SUP_95AC	//	khekim 00/11/23
extern	void	SetDsEnterSockMode
(
	void
);

extern	void	SetDsExitSockMode
(
	void
);
#endif	//	#ifdef	FEATURE_SKT_SOCK_SUP_95AC

#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO
extern	void	SetMOinSO33
(
	word	nMuxOption
);
#endif	//	#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO

#ifdef FEATURE_WAP
void	SetMOinSO33toMIN(void);
#endif //#ifdef FEATURE_WAP
#ifdef	FEATURE_SKT_NDSS_DS	//	khekim 01/04/11
extern void SetDSOriginationByNDSS
(
	byte	*pCalledAddress,
	byte	*pDialCnt,
	boolean *pDigitMode,
	word	*pwSrvOpt
);
#endif	//	#ifdef	FEATURE_SKT_NDSS_DS


extern boolean	SetSubMuxOptOfSrvOpt
(
	word wSrvOpt,
	word wFwdSubMuxOpt,
	word wRevSubMuxOpt
);
#endif	// #ifndef	DSSKT_H	// khekmi 99/2/11