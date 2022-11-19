/*
 *     IrDA Driver DeepCore(tm) Version 1
 *
 *     Copyright(c) 1996-1998 Okaya Systemware Co.,Ltd.
 *     All Rights Reserved
 *
 *     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware
 *     The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *     $Header: irlmp.hv  171.8  98/05/13 14:28:34  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* �T�v																		*/
/*		IrLMP�v���g�R����`													*/
/*																			*/
/* �쐬��																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__irlmp_h__)
#define	__irlmp_h__


#define		FUTURE


#include	"irda.h"


/*  -----  return codes  -----  */

#define	LMPR_BUSYWAIT		IrDAR_BUSYWAIT			/*���͖Z�����̂Ń��g���C����*/
#define	LMPR_NORESOURCE		IrDAR_NORESOURCE		/*�v�����ꂽ���\�[�X�͖���	*/
#define	LMPR_TOOLONG		IrDAR_TOOLONG			/*�v�����傫������			*/
#define	LMPR_BUFFULL		IrDAR_BUFFULL			/*�o�b�t�@�t��				*/
#define	LMPR_TIMEOUT		IrDAR_TIMEOUT			/*�^�C���A�E�g				*/
#define	LMPR_MEDIABUSY		IrDAR_MEDIABUSY			/*���f�B�A�r�W�[			*/
#define	LMPR_INVALID		IrDAR_INVALID			/*�v������					*/
#define	LMPR_ACCEPT			IrDAR_ACCEPT			/*�v����					*/
#define	LMPR_DISCOVERY		IrDAR_DISCOVERY			/*�f�B�X�J�o����			*/
#define	LMPR_CONNECTING		IrDAR_CONNECTING		/*�ڑ���					*/
#define	LMPR_ENDDISCOVERY	IrDAR_ENDDISCOVERY		/*�f�B�X�J�o���I��			*/
#define	LMPR_DONE			IrDAR_DONE				/*���M�I��					*/


/*  -----  Service hints  -----  */

#define	IrLMP_SH_PnPCompatible	0					/*PnP Compatible		*/
#define	IrLMP_SH_PDA			1					/*PDA/Palmtop			*/
#define	IrLMP_SH_Computer		2					/*Computer				*/
#define	IrLMP_SH_Printer		3					/*Printer				*/
#define	IrLMP_SH_Modem			4					/*Modem					*/
#define	IrLMP_SH_Fax			5					/*Fax					*/
#define	IrLMP_SH_LAN			6					/*LAN Access			*/
#define	IrLMP_SH_Telephony		(8+0)				/*Telephony				*/
#define	IrLMP_SH_FileServer		(8+1)				/*File Server			*/
#define	IrLMP_SH_IrCOMM			(8+2)				/*IrCOMM				*/
#define	IrLMP_SH_IrOBEX			(8+5)				/*IrOBEX				*/


/*  -----  Character set  -----  */

#define	IrLMP_CS_ASCII		0
#define	IrLMP_CS_ISO8859_1	1
#define	IrLMP_CS_ISO8859_2	2
#define	IrLMP_CS_ISO8859_3	3
#define	IrLMP_CS_ISO8859_4	4
#define	IrLMP_CS_ISO8859_5	5
#define	IrLMP_CS_ISO8859_6	6
#define	IrLMP_CS_ISO8859_7	7
#define	IrLMP_CS_ISO8859_8	8
#define	IrLMP_CS_ISO8859_9	9
#define	IrLMP_CS_UNICODE	255


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		IrLMP�ڑ������[�U�[�f�[�^											*/
/*																			*/
/* 2.�T�v																	*/
/*		IrLMP�ڑ����ɂ��Ƃ肳��郆�[�U�[�f�[�^���Ǘ�����B				*/
/*																			*/
/* 3.�@�\����																*/
/*		IrLMP�ڑ����ɂ��Ƃ肳��郆�[�U�[�f�[�^���Ǘ�����B				*/
/*																			*/
/* 4.���l																	*/
/*		����																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_LMCONNDAT	{
	byte	IrDAFAR *Data ;							/*User Data											*/
	int				DataSize ;						/*User Data Size									*/
}	LMCONNDAT ;


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		IrLMP�R�[���o�b�N�֐�												*/
/*																			*/
/* 2.�T�v																	*/
/*		�e��C�x���g�������ɌĂяo���R�[���o�b�N�֐����Ǘ�����B			*/
/*																			*/
/* 3.�@�\����																*/
/*		IrLMP�Ŋe��C�x���g�������ɌĂяo���R�[���o�b�N�֐����Ǘ�����B		*/
/*																			*/
/* 4.���l																	*/
/*		����																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_LMCALLBACK	{
	int		(IrDACB *ConnectInd)( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd ) ;
	void	(IrDACB *ConnectCnf)( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status ) ;
	void	(IrDACB *DataInd)( int lsap, const void IrDAFAR *buf, uint size ) ;
	void	(IrDACB	*DataCnf)( int lsap ) ;
	void	(IrDACB *DisconnectInd)( int lsap, int reason, const void IrDAFAR *data, int datasz ) ;
	void	(IrDACB *UDataInd)( int lsap, void IrDAFAR *buf, uint size ) ;
	void	(IrDACB *UDataCnf)( int lsap ) ;
}	LMCALLBACK ;


/*  -----  return status ( for callback )  -----  */

#define	IrLMP_CONNECTEN		IrDA_CONNECTEN
#define	IrLMP_CONNECTDI		IrDA_CONNECTDI


/*  -----  Disconnect Reason  -----  */

#define	IrLMP_DR_UserRequest						0x01
#define	IrLMP_DR_UnexpectedIrLAPDisconnect			0x02
#define	IrLMP_DR_FailedToEstablishIrLAPConnection	0x03
#define	IrLMP_DR_IrLAPReset							0x04
#define	IrLMP_DR_LMInitiatedDisconnect				0x05
#define	IrLMP_DR_LSAPConnection						0x06
#define	IrLMP_DR_NonResponsiveLMClient				0x07
#define	IrLMP_DR_NoAvailableLMClient				0x08
#define	IrLMP_DR_ConnectionHalfOpen					0x09
#define	IrLMP_DR_IllegalSourceAddress				0x0A
#define	IrLMP_DR_UnSpecifiedDisconnectReason		0xFF


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/


/*  -----  Public Functions for Device  -----  */

int		IrDAAPI	IrLMP_Open( void ) ;
void	IrDAAPI	IrLMP_Close( void ) ;
													/*�f�B�X�J�o���[�v��								*/
int		IrDAAPI	IrLMP_DiscoveryReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz )) ;

													/*���Ǒ��u���ݒ�									*/
int		IrDAAPI	IrLMP_SetShortDeviceName( uint charset, const byte IrDAFAR *name, uint namesz ) ;
													/*���ǃT�[�r�X�q���g�ݒ�							*/
int		IrDAAPI	IrLMP_SetServiceHint( uint servicehint ) ;
													/*���ǃT�[�r�X�q���g����							*/
int		IrDAAPI	IrLMP_ResetServiceHint( uint servicehint ) ;
													/*�T�[�r�X�q���g����								*/
IrDABOOL	IrDAAPI	IrLMP_CheckServiceHint( void IrDAFAR *ptr, uint len, uint servicehint ) ;
uint	IrDAAPI	IrLMP_GetDiscoveryName( void IrDAFAR *ptr, uint len ) ;

#if !defined(IrLAPNORXCHG)
int		IrDAAPI	IrLMP_SetPrimaryInd( int (IrDACB *indp)( void )) ;
int		IrDAAPI	IrLMP_PrimaryReq( void (IrDACB *cnfp)( int status )) ;
#endif	/*!IrLAPNORXCHG*/
#if !defined(IrLAPNOSNIFF)
int		IrDAAPI	IrLMP_SetSniffInd( DISCOVERYLOG IrDAFAR *logp, int (IrDACB *indp)( void )) ;
int		IrDAAPI	IrLMP_SniffReq( void ) ;
int		IrDAAPI	IrLMP_SniffAbt( void ) ;
#endif	/*!IrLAPNOSNIFF*/


/*  -----  Public Functions for Service  -----  */

int		IrDAAPI	IrLMP_RequestLSAP( void ) ;			/*��LSAP�擾										*/
													/*LSAP���蓖��										*/
int		IrDAAPI	IrLMP_BindLSAP( int lsap, LMCALLBACK IrDAFAR *lmc ) ;
void	IrDAAPI	IrLMP_UnbindLSAP( int lsap ) ;		/*LSAP�J��											*/
int		IrDAAPI	IrLMP_GetRealLSAP( int lsap ) ;
													/*LSAP�ڑ��v��										*/
int		IrDAAPI	IrLMP_ConnectReq( dword addr, int ssel, int dsel, void IrDAFAR *data, int datasz ) ;
													/*LSAP�ؒf�v��										*/
int		IrDAAPI	IrLMP_DisconnectReq( int lsap, int reason, void IrDAFAR *data, int datasz ) ;
int		IrDAAPI	IrLMP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
uint	IrDAAPI	IrLMP_GetMaxTxSize( int lsap ) ;	/*�ő呗�M�T�C�Y�擾								*/
uint	IrDAAPI	IrLMP_GetMaxRxSize( int lsap ) ;	/*�ő��M�T�C�Y�擾								*/
IrDABOOL	IrDAAPI	IrLMP_QueryTxQueue( void ) ;		/*�󂫑��M�L���[����								*/
int		IrDAAPI	IrLMP_QueryVirginTxQueue( void ) ;	/*�����M�L���[���擾	*/

int		IrDAAPI	IrLMP_QueryConnectedLsap( void ) ;
int		IrDAAPI	IrLMP_DisconnectLink( void ) ;
int		IrDAAPI	IrLMP_DisableIdleTimer( void ) ;
int		IrDAAPI	IrLMP_EnableIdleTimer( void ) ;

#if !defined(IrLMPNOUI)
int		IrDAAPI	IrLMP_CLessDataReq( const void IrDAFAR *bufp, int len ) ;
int		IrDAAPI	IrLMP_SetCLessDataCnf( void (IrDACB* func)( void )) ;
int		IrDAAPI	IrLMP_SetCLessDataInd( void (IrDACB* func)( const void IrDAFAR* data, int datasz )) ;
int		IrDAAPI	IrLMP_QueryMaxCLessTxDataSize( void ) ;
int		IrDAAPI	IrLMP_QueryMaxCLessRxDataSize( void ) ;
#endif	/*!IrLMPNOUI*/


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		IAS ValueByClass�����f�[�^�Ǘ��e�[�u��								*/
/*																			*/
/* 2.�T�v																	*/
/*		IAS ValueByClass�����f�[�^��͏����Ǘ�����B						*/
/*																			*/
/* 3.�@�\����																*/
/*		IAS ValueByClass�����f�[�^��͏����Ǘ�����B						*/
/*																			*/
/* 4.���l																	*/
/*		����																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_IASVALUEBYCLASS	{
	uint			ObjectIdentifier ;	/*�I�u�W�F�N�gID					*/
	byte	IrDAFAR	*EncodedValue ;		/*��͂��������f�[�^�̃A�h���X		*/
}	IASVALUEBYCLASS ;


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		IAS�N���C�A���g�R�[���o�b�N�֐�										*/
/*																			*/
/* 2.�T�v																	*/
/*		�e��C�x���g�������ɌĂяo���R�[���o�b�N�֐����Ǘ�����B			*/
/*																			*/
/* 3.�@�\����																*/
/*		IAS�N���C�A���g�Ŋe��C�x���g�������ɌĂяo���R�[���o�b�N�֐�		*/
/*		���Ǘ�����B														*/
/*																			*/
/* 4.���l																	*/
/*		����																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_IASCALLBACK	{
	void	(IrDACB *ConnectCnf)( int lsap, int status ) ;
	void	(IrDACB *ValueCnf)( int lsap, byte IrDAFAR *value, int valuesz ) ;
	void	(IrDACB *DisconnectInd)( int lsap ) ;
}	IASCALLBACK ;

int		IrDAAPI	IrLMP_AddClass( const void IrDAFAR *classnm, uint classsz ) ;
int		IrDAAPI	IrLMP_DeleteClass( int classid ) ;
int		IrDAAPI	IrLMP_AddAttribute( int classid, const void IrDAFAR *attrnm, uint attrsz ) ;
int		IrDAAPI	IrLMP_DeleteAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_SetAttribute( int attrid, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetIntAttribute( int attrid, int val ) ;
int		IrDAAPI	IrLMP_SetOctetAttribute( int attrid, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetUserStringAttribute( int attrid, uint charset, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetLSAPAttribute( int attrid, int lsap ) ;
#if !defined(IASNOHIDDENCONTROL)
int		IrDAAPI	IrLMP_ShowAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_HiddenAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_ShowClass( int classid ) ;
int		IrDAAPI	IrLMP_HiddenClass( int classid ) ;
#endif	/*!IASNOHIDDENCONTROL*/

int		IrDAAPI	IrLMP_OpenIAS( dword addr, IASCALLBACK IrDAFAR *icb ) ;
int		IrDAAPI	IrLMP_CloseIAS( int lsap ) ;
int		IrDAAPI	IrLMP_GetValueByClass( int lsap, const void IrDAFAR *classnm, uint classsz, const void IrDAFAR *attr, uint attrsz ) ;
int		IrDAAPI	IrLMP_QueryReturnCode( byte IrDAFAR *value, int valuesz ) ;
int		IrDAAPI	IrLMP_QueryValueByClass( byte IrDAFAR *value, int valuesz, IASVALUEBYCLASS IrDAFAR *listp, uint listsz ) ;
int		IrDAAPI	IrLMP_GetIntValue( byte IrDAFAR *value, dword IrDAFAR *valp ) ;
int		IrDAAPI	IrLMP_GetOctetValue( byte IrDAFAR *value, word IrDAFAR *valsz, byte IrDAFAR * IrDAFAR *valp ) ;
int		IrDAAPI	IrLMP_GetUserStringValue( byte IrDAFAR *value, int IrDAFAR *charset, byte IrDAFAR * IrDAFAR *valp ) ;


/*  -----  for indication  -----  */

void	IrDAAPI	IrLMP_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLMP_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLMP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/


#endif	/*__irlmp_h__*/