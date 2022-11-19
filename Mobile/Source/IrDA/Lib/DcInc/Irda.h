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
 *     $Header: irda.hv  171.28  99/03/12 07:24:56  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* �T�v																		*/
/*		DeepCore��`														*/
/*																			*/
/* �쐬��																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


/*
**	���̃w�b�_��windows.h�̌�ɃC���N���[�h����K�v������܂��B
*/

#if !defined(__irda_h__)
#define	__irda_h__

#include "comdef.h"


/*  -----  Compile Switches  -----  */
/*
**	DMSG					Use Debug Messages
**	SEQCHK					�f�[�^�`�F�b�N�p�̃V�[�P���T���g�p
**	TxPTRCHK				���M�E�B���h�E�̊Ǘ��|�C���^�̃`�F�b�N���s��
**	USERDBG					for user debug
**
**	MAKE_DOCUMENT			�uC++�H�[�v�Ńh�L�������g���쐬����ۂɕK�{
**	IrDANOSTATIC			���[�U�[�h�L�������g�쐬�p�Ƀ��[�J���֐��A���[�J���ϐ����W�F�l���[�g���Ȃ��B
**
**	NOTXMULTIWINDOW			���M���ɕ����t���[����A�����đ��M���Ȃ�
**	IrLAPMAXINFRAMERETRYCOUNT P/F Timer, WD Timer��InFrame���̊��荞�݉���ׂ̈̉񐔁i�f�t�H���g10��j
**	IrLAPNONEWADDR			NEW ADDRESS���T�|�[�g���Ȃ��B
**	AUTONEWADDRHIST			�A�h���X�Փˏ����̗����e�[�u�����i�f�t�H���g�F16�j
**	IrLAPNEWADDRRETRYCOUNT	�A�h���X�Փˎ��̎������g���C�񐔁i�f�t�H���g3��j
**
**	IrDAPRIMARY				Use Primary
**	IrDASECONDARY			Use Secondary
**	IrDANEWADDRBYDISCOVERY	�f�B�X�J�o���̓x�Ƀf�o�C�X�A�h���X��U�蒼��
**	IrDASWAP				Swappable
**	IrDA_ACCEPTBEFORERNR	RR���M���LOCALBUSY��ݒ肳�ꂽ�ꍇ�ɁA���̎�M�t���[����IFrame��������󂯕t����悤�ɂ���
**	IrDA_DELAIEDSECONDARY	�Q���ǐڑ����A���M���ׂ��t���[����RR/RNR�������ꍇ�ɂ�����x���Ԃ������Ă��瑗�M�o����悤�ɂ���
**	IrDANOINCLUDEDCVER		DeepCore �̃o�[�W������IAS�Ŗ₢���킹���� "IrDA:OSW", "DeepCoreVersion"�����Ȃ�
**
**	TIME_SNIFFSENSE			Sniff Open����O��IrBUS�Ď�����
**	TIME_SNIFF				Sniff Open����Sniff-XID-Rsp���o���IrBUS�Ď�����
**	TIME_SLEEP				Sniff Open���̃X���[�v�^�C�}�[�l
**	TIME_WAITSNIFFXIDRSP	Sniff Open����Sniff-XID-Rsp�҂�����
**
**	IrLAPSTRICTMINTAT		IrLAP�ŏ��^�[���A���E���h�^�C����0.001ms�ȏ�1ms�����̒l��1ms�Ƃ���B
**	IrLAPMINTATMICRO		IrLAP�ŏ��^�[���A���E���h�^�C����1ms�����̒l���T�|�[�g����B
**
**	LSAPMAX					IrLMP LSAP��
**
**	IASNOHIDDENCONTROL		IAS�G���g���̉������R���g���[�����T�|�[�g���Ȃ��B
**	IASANSWERBUFSZ			IAS�����p�P�b�g�g�ݗ��ăo�b�t�@�T�C�Y
**	IASCLASSNAMEMAX			IAS�ɓo�^����N���X���̍ő�T�C�Y�i�f�t�H���g�F32�j
**	IASATTRNAMEMAX			IAS�ɓo�^����A�g���r���[�g���̍ő�T�C�Y�i�f�t�H���g�F32�j
**	IASATTRVALUEMAX			IAS�ɓo�^����l�̍ő�T�C�Y�i�f�t�H���g�F32�j
**	IASCLASSMAX				IAS�ɓo�^����N���X��
**	IASATTRMAX				IAS�ɓo�^����A�g���r���[�g��
**
**	TTPMAX					TinyTP LSAP��
**	TTPRxBUF				��MFIFO�p�o�b�t�@�T�C�Y�B��M����I Frame�����傫���Ȃ���΂Ȃ�Ȃ��B�i�f�t�H���g�F2048�j
**
**	IrCOMMNO3WIRERAW		IrCOMM 3wire raw���g�p���Ȃ��B
**	NOTTP					IrCOMM cooked mode���g�p���Ȃ��B
**	COMMMAX					IrCOMM LSAP��
**	COMMTXSDUSZ				IrCOMM ���M�p�P�b�g�g�ݗ��ăo�b�t�@�T�C�Y
**	CTLQUEUESZ				IrCOMM �R���g���[���L���[�o�b�t�@�T�C�Y
**
**	IrDA_LDiv				32bit�̏��Z���s���}�N��
**	IrDA_LMul				32bit�̏�Z���s���}�N��
**	IrDA_LSift				32bit�̍��V�t�g���s���}�N��
**
**	IrDA_memcmp				�������Ԕ�r�}�N��
**	IrDA_memcpy				�������ԃR�s�[�}�N��
**	IrDA_memset				�������Z�b�g�}�N��
**
**	���C��
**	IrLAP_MORATORIUMTIME	IrLAP1���ǂƂȂ����ꍇ�ɍŌ��IFrame����M���Ă���ARRCmd��P�^�C�}�ɂ��x�������đ��M����悤�ɂȂ�܂ł̎��ԁi�f�t�H���g�F1000�j
**	IrLAPSLOTTIME			Discovery���̃X���b�g�Ԋu
**	IrLAPNOUSEDISCTIMER		Primary���̐ؒf�܂ł̎��Ԃ�RRCmd���o�񐔂Ō��肷��
**
**	IrLAPNORESET			reset�������T�|�[�g���Ȃ��B
**	IrLAPNORXCHG			Role Exchange ���T�|�[�g���Ȃ��B
**	IrLAPNOSNIFF			SNIFF Open���T�|�[�g���Ȃ��B
**	IrLAPNOUI				UI Data�̏������Ȃ�
**	IrLAPFRMRDISC			FRMR����M�����ꍇ�Ƀ��Z�b�g���s�킸�ɐؒf����B
**
**	IrLMPNOUI				UI Data�̏������Ȃ�
**	IrOBEXNOULTRA			IrOBEX under ULTRA���T�|�[�g���Ȃ��B
**
**	���C�Ӂi�T�C�Y�֘A)
**	OBEXTXBUFSZ				IrOBEX���M�p�P�b�g�g�ݗ��ăo�b�t�@�T�C�Y�i�f�t�H���g�F2048)
**	OBEXRXBUFSZ				IrOBEX��M�p�P�b�g�g�ݗ��ăo�b�t�@�T�C�Y�i�f�t�H���g�F2048)
**
**	��GENOA Test System �΍�
**	IrDAFORGENOASIRTEST		GENOA�΍��S�Ċ܂�
**	GENOA_A1120				DiscoveryCmd����肱�ڂ����ۂɂ��΂₭�Ώ��o
**							�؂�悤�ɂ����`���[�j���O���AGENOA A1120��
**							�o�O�ƕ񍐂���Ă��܂��Ă���B
**	GENOA_T8107				�{���ATESTCmd�ɑ΂���TESTRsp�́A��M�����f�[
**							�^��Ԃ��邾���Ԃ��悤�ɋK�肳��Ă���B����
**							�ׁA�����R�}���h�o�b�t�@��64Byte�Ȃ̂ł��̕�
**							�����Ԃ��Ă��邪�AGENOA �ł͑S�Ă�Ԃ��Ȃ���
**							Fail�ƕ񍐂���Ă��܂��B
**
**	���p�t�H�[�}���X�v���p�֐��}�N��
**		�ȉ��̊֐��}�N���͂��ׂĂ̎��̃v���g�^�C�v�Ƃ���B
**			void	a( int f ) ;
**		����f�͊֐��Ăяo���O�����A�ォ��\���Ă���A0���Ăяo���O�A1�����A
**		��ƂȂ��Ă���B
**	IrDA_LAPDATACNFPRB		LAP�̃f�[�^�R���t�@�[���Ăяo���O��ɌĂ΂��}�N���B
**	IrDA_LAPDATAINDPRB		LAP�̃f�[�^�C���f�B�P�[�g�Ăяo���O��ɌĂ΂��}�N���B
**	IrDA_LAPTXEMPTYPRB		LAP��TxEmpty�R�[���o�b�N�Ăяo���O��ɌĂ΂��}�N���B
**	IrDA_LMPDATACNFPRB		LMP�̃f�[�^�R���t�@�[���Ăяo���O��ɌĂ΂��}�N���B
**	IrDA_LMPDATAINDPRB		LMP�̃f�[�^�C���f�B�P�[�V�����Ăяo���O��ɌĂ΂��}�N���B
**	IrDA_TTPDATACNFPRB		TTP�̃f�[�^�R���t�@�[���Ăяo���O��ɌĂ΂��}�N���B
**	IrDA_TTPDATAINDPRB		TTP�̃f�[�^�C���f�B�P�[�V�����Ăяo���O��ɌĂ΂��}�N���B
**	IrDA_COMDATAINDPRB		COMM�̃f�[�^�C���f�B�P�[�V�����Ăяo���O��ɌĂ΂��}�N���B
**
**	IrDA_LMPDATAREQPRB		LMP�̃f�[�^�]���v��API���s�O��ŌĂ΂��}�N��
**	IrDA_TTPDATAREQPRB		TTP�̃f�[�^�]���v��API���s�O��ŌĂ΂��}�N��
**	IrDA_COMMDATAREQPRB		IrCOMM�̃f�[�^�]���v��API���s�O��ŌĂ΂��}�N��
*/


/*  -----  for other bugs  -----  */
/*	IrDAFORTRANXIT01	TRANXIT(IBMIR.DLL)�̑��x�Ɋւ���l�S�V�G�[�V��
**						���p�����[�^�̃o�O������B
**	IrDABROKENCOMPILER01	switch���̒��� byte ���� int �ւ̕����g����
**							�������s���Ȃ��R���p�C���p�B�ȉ��A�K�v�ȃR��
**							�p�C���B
**							V850 Family Optimize C Compiler V1.10 [13 Aug 1996]
*/
#define	IrDAFORTRANXIT01


/*  -----  for GENOA Test Suites  -----  */
#if defined(IrDAFORGENOASIRTEST)
#undef	GENOA_A1120
#define	GENOA_A1120
#undef	GENOA_T8107
#define	GENOA_T8107
#endif	/*IrDAFORGENOASIRTEST*/

/*	-----  Version Control  -----  */
/*
**	�o�[�W�����Ԃ̍����z������ׂɁAIrDAVER�Ɍ��݂̃o�[�W�����ԍ����
**	�`����K�v������܂��B
*/
#define	IrDAVER000	200
/*
**		���݂̃o�[�W����
*/
/*#define	IrDAVER	IrDAVER200*/


/*  -----  for Swappable module  -----  */

#if defined(IrDASWAP)
#  if !defined(IrDAAPI)
#    define	IrDAAPI		__far __cdecl
#  endif	/*!IrDAAPI*/
#  if !defined(IrDAFAR)
#    define	IrDAFAR		__far
#  endif	/*!IrDAFAR*/
#  if !defined(IrDACB)
#    define	IrDACB		__far __cdecl
#  endif	/*!IrDACB*/
#endif	/*IrDASWAP*/

#if 0
#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/
extern	void	IrDACB	IrDA_SwapIn( void ) ;
extern	void	IrDACB	IrDA_SwapOut( void ) ;
#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/
#endif


#if !defined(MAKE_DOCUMENT)
#if !defined(IrDAAPI)
#define		IrDAAPI
#endif	/*!IrDAAPI*/
#if !defined(IrDAFAR)
#define		IrDAFAR
#endif	/*!IrDAFAR*/
#if !defined(IrDACB)
#define		IrDACB
#endif	/*!IrDACB*/
#endif	/*!MAKE_DOCUMENT*/
#if !defined(IrDAMISCAPI)
#define		IrDAMISCAPI
#endif	/*!IrDAMISCAPI*/


/*  -----  typedefs  -----  */

#if !defined(__byte_defined)
#define	__byte_defined
//htgtypedef	unsigned char		byte ;
#endif	/*!defined(__byte_defined)*/
#if !defined(__irdabool_defined__)
#define	__irdabool_defined__
typedef	unsigned int		IrDABOOL ;
#endif	/*defined(__irdabool_defined__)*/
#if !defined(__word_defined__)
#define	__word_defined__
//htg typedef	unsigned short		word ;					/*2byte �ȏ��unsigned	*/
#endif	/*defined(__word_defined__)*/
#if !defined(__dword_defined__)
#define	__dword_defined__
//htgtypedef	unsigned long		dword ;
#endif	/*defined(__dword_defined__)*/
#if !defined(__uint_defined__)
#define	__uint_defined__
typedef	unsigned int		uint ;
#endif	/*defined(__uint_defined__)*/
#if !defined(__ulong_defined__)
#define	__ulong_defined__
typedef	unsigned long		ulong ;
#endif	/*defined(__ulong_defined__)*/

typedef	void (IrDACB *IrDACBP)( void ) ;


/*  -----  return status  -----  */

#define	IrDAR_DONE			1					/*�v���I��					*/
#define	IrDAR_ACCEPT		0					/*Accept Request			*/
#define	IrDAR_NOCONNECT		-1					/*not connecting			*/
#define	IrDAR_NORESPONSE	-2					/*no response				*/
#define	IrDAR_DISCONNECT	-3					/*cancel by DISCONNECT		*/
#define	IrDAR_CONNECTING	-4					/*�ڑ���					*/
#define	IrDAR_MEDIABUSY		-5					/*���f�B�A�r�W�[			*/
#define	IrDAR_EPARM			-6					/*Error Parameter			*/
#define	IrDAR_INVALIDVER	-7
#define	IrDAR_NOPROTOCOL	-8
#define	IrDAR_INUSE			-9					/*In Used					*/
#define	IrDAR_PRIMARY		-10					/*�v���C�}���Ɉڍs����		*/
#define	IrDAR_DENY			-11					/*���s						*/

#if 1
#define	IrDAR_BUSYWAIT		-17					/*���͖Z�����̂Ń��g���C����*/
#define	IrDAR_NORESOURCE	-16					/*�v�����ꂽ���\�[�X������	*/
#define	IrDAR_TOOLONG		-15					/*�v�����傫������			*/
#define	IrDAR_BUFFULL		-14					/*�o�b�t�@�t��				*/
#define	IrDAR_TIMEOUT		-13					/*�^�C���A�E�g				*/
#define	IrDAR_INVALID		-12					/*�v������					*/
#define	IrDAR_DISCOVERY		11					/*�f�B�X�J�o����			*/
#endif


/*  -----  OEM ID  -----  */

#define	MAKER_SII		0x00
#define	MAC_BP			0x00
#define	OEMID_SIIBP		((MAKER_SII<<8)+MAC_BP)
#define	OEMID_MITS10	0x0200
#define	OEMID_PUBLIC	0xFFFF


/*  -----  misc  -----  */

#ifndef YES
    #define	NO					0
    #define	YES					1
#endif
#ifndef TRUE
    #define	FALSE				0
    #define	TRUE				1
#endif
#ifndef ON
    #define	OFF					0
    #define	ON					1
#endif
#define	IrLAP_NNMSZ			32
#define	CLESSLSAP			0x70


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		�ǔ\�͊Ǘ��e�[�u��													*/
/*																			*/
/* 2.�T�v																	*/
/*		IrLAP�̋ǔ\�͂��Ǘ�����B											*/
/*																			*/
/* 3.�@�\����																*/
/*		IrLAP�̋ǔ\�͂��Ǘ�����B											*/
/*																			*/
/* 4.���l																	*/
/*		�e�����o��IrLAP1.1�Œ�`����Ă���r�b�g�}�b�v�ŊǗ�����Ă���B	*/
/*		���ׁ̈A�Q�ƁA�ύX���s���ۂɂ͊e�����o���Ɉȉ��̒�`���g�p����		*/
/*		���B																*/
/*																			*/
/*			LineCapability		IrLAP_BPS_*									*/
/*			MaxTanaroundTime	IrLAP_MaxTurn_*								*/
/*			FrameSize			IrLAP_FS_*									*/
/*			WindowSize			IrLAP_WS_*									*/
/*			NoofBOF				IrLAP_BOF_*									*/
/*			MinTurnaroundTime	IrLAP_MinTurn_*								*/
/*			DisconnectTime		IrLAP_SL_*									*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_PORTCONFIG	{
	byte			LineCapability ;				/*0x01:���x				*/
	byte			MaxTanaroundTime ;				/*0x82:�ő�^�[���A���E���h�^�C��					*/
	byte			FrameSize ;						/*0x83:�f�[�^�T�C�Y		*/
	byte			WindowSize ;					/*0x84:�E�B���h�E��		*/
	byte			NoofBOF ;						/*0x85:�ǉ�BOF��		*/
	byte			MinTurnaroundTime ;				/*0x86:�ŏ��^�[���A���E���h�^�C��*/
	byte			DisconnectTime ;				/*0x08:�ؒf����			*/
}	PORTCONFIG ;


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		�ǔ\�͊Ǘ��e�[�u��(16bit��)											*/
/*																			*/
/* 2.�T�v																	*/
/*		IrLAP�̋ǔ\�͂��Ǘ�����B											*/
/*																			*/
/* 3.�@�\����																*/
/*		IrLAP�̋ǔ\�͂��Ǘ�����B											*/
/*																			*/
/*		���\���̂́APORTCONFIG ��16bit�łł���B���ׁ̈A�e�����o��16bit		*/
/*		�Ɋg������Ă��鎖�������΁APORTCONFIG �Ɠ��l�ł���B				*/
/*																			*/
/* 4.���l																	*/
/*		�e�����o��IrLAP1.1�Œ�`����Ă���r�b�g�}�b�v�ŊǗ�����Ă���B	*/
/*		���ׁ̈A�Q�ƁA�ύX���s���ۂɂ͊e�����o���Ɉȉ��̒�`���g�p����		*/
/*		���B																*/
/*																			*/
/*			LineCapability		IrLAP_BPS_*									*/
/*			MaxTanaroundTime	IrLAP_MaxTurn_*								*/
/*			FrameSize			IrLAP_FS_*									*/
/*			WindowSize			IrLAP_WS_*									*/
/*			NoofBOF				IrLAP_BOF_*									*/
/*			MinTurnaroundTime	IrLAP_MinTurn_*								*/
/*			DisconnectTime		IrLAP_SL_*									*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_PORTCONFIG2 {
	word			LineCapability ;				/*0x01:���x				*/
	word			MaxTanaroundTime ;				/*0x82:�ő�^�[���A���E���h�^�C��					*/
	word			FrameSize ;						/*0x83:�f�[�^�T�C�Y		*/
	word			WindowSize ;					/*0x84:�E�B���h�E��		*/
	word			NoofBOF ;						/*0x85:�ǉ�BOF��		*/
	word			MinTurnaroundTime ;				/*0x86:�ŏ��^�[���A���E���h�^�C��*/
	word			DisconnectTime ;				/*0x08:�ؒf����			*/
}	PORTCONFIG2 ;

#define		IrLAP_BPS_2400			0x01
#define		IrLAP_BPS_9600			0x02
#define		IrLAP_BPS_19200			0x04
#define		IrLAP_BPS_38400			0x08
#define		IrLAP_BPS_57600			0x10
#define		IrLAP_BPS_115200		0x20
#define		IrLAP_BPS_576000		0x40
#define		IrLAP_BPS_1152000		0x80
#define		IrLAP_BPS_4000000		0x0100
#define		IrLAP_MaxTurn_500ms		0x01
#define		IrLAP_MaxTurn_250ms		0x02
#define		IrLAP_MaxTurn_100ms		0x04
#define		IrLAP_MaxTurn_50ms		0x08
#define		IrLAP_MaxTurn_25ms		0x10
#define		IrLAP_MaxTurn_10ms		0x20
#define		IrLAP_MaxTurn_5ms		0x40
#define		IrLAP_FS_64				0x01
#define		IrLAP_FS_128			0x02
#define		IrLAP_FS_256			0x04
#define		IrLAP_FS_512			0x08
#define		IrLAP_FS_1024			0x10
#define		IrLAP_FS_2048			0x20
#define		IrLAP_WS_1				0x01
#define		IrLAP_WS_2				0x02
#define		IrLAP_WS_3				0x04
#define		IrLAP_WS_4				0x08
#define		IrLAP_WS_5				0x10
#define		IrLAP_WS_6				0x20
#define		IrLAP_WS_7				0x40
#define		IrLAP_BOF_48			0x01
#define		IrLAP_BOF_24			0x02
#define		IrLAP_BOF_12			0x04
#define		IrLAP_BOF_5				0x08
#define		IrLAP_BOF_3				0x10
#define		IrLAP_BOF_2				0x20
#define		IrLAP_BOF_1				0x40
#define		IrLAP_BOF_0				0x80
#define		IrLAP_MinTurn_10ms		0x01
#define		IrLAP_MinTurn_5ms		0x02
#define		IrLAP_MinTurn_1ms		0x04
#define		IrLAP_MinTurn_05ms		0x08
#define		IrLAP_MinTurn_01ms		0x10
#define		IrLAP_MinTurn_005ms		0x20
#define		IrLAP_MinTurn_001ms		0x40
#define		IrLAP_MinTurn_0ms		0x80
#define		IrLAP_SL_3s				0x01
#define		IrLAP_SL_8s				0x02
#define		IrLAP_SL_12s			0x04
#define		IrLAP_SL_16s			0x08
#define		IrLAP_SL_20s			0x10
#define		IrLAP_SL_25s			0x20
#define		IrLAP_SL_30s			0x40
#define		IrLAP_SL_40s			0x80


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		�ǔ������O															*/
/*																			*/
/* 2.�T�v																	*/
/*		�f�B�X�J�o���̌��ʂ��i�[���郍�O									*/
/*																			*/
/* 3.�@�\����																*/
/*		�f�B�X�J�o���̌��ʂ��i�[���郍�O									*/
/*																			*/
/* 4.���l																	*/
/*		����																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_DISCOVERYLOG {
	dword		Address ;							/*���u�A�h���X			*/
	uint		DeviceNameSize ;					/*���u���T�C�Y			*/
	byte		DeviceName[IrLAP_NNMSZ] ;			/*���u��				*/
}	DISCOVERYLOG ;


/*  -----  Response return code  -----  */

#define		IrDA_CONNECTDI		-1
#define		IrDA_CONNECTEN		0
#define		IrDA_RESETDI		-1
#define		IrDA_RESETEN		0
#define		IrDA_DENY			-1
#define		IrDA_ACCEPT			0


/*  -----  functiones  -----  */

#if !defined(IrDA_memcpy)
#define	IrDA_memcpy				memcpy
#endif	/*!IrDA_memcpy*/
#if !defined(IrDA_memcmp)
#define	IrDA_memcmp				memcmp
#endif	/*!IrDA_memcmp*/
#if !defined(IrDA_memset)
#define	IrDA_memset				memset
#endif	/*!IrDA_memset*/
#if !defined(IrDA_LDiv)
#define	IrDA_LDiv( src, des )	((src)/(des))
#endif	/*!IrDA_LDiv*/
#if !defined(IrDA_LMul)
#define	IrDA_LMul( src, des )	((src)*(des))
#endif	/*!IrDA_LMul*/
#if !defined(IrDA_LSift)
#define	IrDA_LSift( src, cnt )	((src)<<(cnt))
#endif	/*!IrDA_LSift*/


/*  -----  macros  -----  */

#if !defined(UNREFERENCE)
# define	UNREFERENCE(i)	i=i
#endif	/*!UNREFERENCE*/


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/


/*  -----  public functions  -----  */

void		IrDAAPI	IrDA_SetRandomSeed( dword seed ) ;
uint		IrDAAPI	IrDA_GetOEMID( void ) ;
													/*���M�o�b�t�@�ݒ�									*/
void		IrDAAPI	IrDA_SetTxFrame( byte IrDAFAR *buf, uint framesz, uint winsz ) ;
													/*��M�o�b�t�@�ݒ�									*/
void		IrDAAPI	IrDA_SetRxFrame( byte IrDAFAR *buf, uint framesz, uint winsz ) ;
													/*�ǔ\�͎擾										*/
void		IrDAAPI	IrDA_GetPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													/*�ǔ\�͐ݒ�										*/
void		IrDAAPI	IrDA_SetPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													/*�ڑ���Ԏ擾										*/
int			IrDAAPI	IrDA_GetConnPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													/*�ǔ\�͎擾										*/
void		IrDAAPI	IrDA_GetPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
													/*�ǔ\�͐ݒ�										*/
void		IrDAAPI	IrDA_SetPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
													/*�ڑ���Ԏ擾										*/
int			IrDAAPI	IrDA_GetConnPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
IrDABOOL	IrDAAPI	IrDA_GetPowerMode( void ) ;
void		IrDAAPI	IrDA_PowerOn( void ) ;
void		IrDAAPI	IrDA_PowerOff( void ) ;
void		IrDAAPI	IrDA_SetPowerLevel( uint level ) ;
uint		IrDAAPI	IrDA_GetPowerLevel( void ) ;
void		IrDA_ProtocolCallBack( void ) ;

#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/


/*  -----  For Debugging  -----  */

#if	defined(DMSG)
#define	DL_TYPE		0xFF00
#define	DL_MODE		0x00FF
#define	DL_LAP		0x0100
#define	DL_LMP		0x0200
#define	DL_TTP		0x0400
#define	DL_COMM		0x0800
#define	DL_MASS		0x1000
#define	DL_MISC		0x2000
#define	DL_SIR		0x3000
#define	DL_USER		0x4000
#define	DL_DANGER	0x0001
#define	DL_WARN		0x0002
#define	DL_INFO		0x0004
#define	DL_DAT		0x0008

#define	DL_SIRDANGER	(DL_SIR|DL_DANGER)
#define	DL_SIRDAT		(DL_SIR|DL_DAT)
#define	DL_SIRWARN		(DL_SIR|DL_WARN)
#define	DL_SIRINFO		(DL_SIR|DL_INFO)
#define	DL_LAPDANGER	(DL_LAP|DL_DANGER)
#define	DL_LAPDAT		(DL_LAP|DL_DAT)
#define	DL_LAPWARN		(DL_LAP|DL_WARN)
#define	DL_LAPINFO		(DL_LAP|DL_INFO)
#define	DL_LMPDANGER	(DL_LMP|DL_DANGER)
#define	DL_LMPDAT		(DL_LMP|DL_DAT)
#define	DL_LMPWARN		(DL_LMP|DL_WARN)
#define	DL_LMPINFO		(DL_LMP|DL_INFO)
#define	DL_TTPDANGER	(DL_TTP|DL_DANGER)
#define	DL_TTPDAT		(DL_TTP|DL_DAT)
#define	DL_TTPWARN		(DL_TTP|DL_WARN)
#define	DL_TTPINFO		(DL_TTP|DL_INFO)
#define	DL_COMMDANGER	(DL_COMM|DL_DANGER)
#define	DL_COMMDAT		(DL_COMM|DL_DAT)
#define	DL_COMMWARN		(DL_COMM|DL_WARN)
#define	DL_COMMINFO		(DL_COMM|DL_INFO)
#define	DL_MASSDANGER	(DL_MASS|DL_DANGER)
#define	DL_MASSDAT		(DL_MASS|DL_DAT)
#define	DL_MASSWARN		(DL_MASS|DL_WARN)
#define	DL_MASSINFO		(DL_MASS|DL_INFO)
#define	DL_MISCDANGER	(DL_MISC|DL_DANGER)
#define	DL_MISCDAT		(DL_MISC|DL_DAT)
#define	DL_MISCWARN		(DL_MISC|DL_WARN)
#define	DL_MISCINFO		(DL_MISC|DL_INFO)
#define	DL_USERDANGER	(DL_USER|DL_DANGER)
#define	DL_USERDAT		(DL_USER|DL_DAT)
#define	DL_USERWARN		(DL_USER|DL_WARN)
#define	DL_USERINFO		(DL_USER|DL_INFO)


#if defined(__cplusplus)
extern "C" {
#endif	/*__cplusplus*/

int		IrDAAPI	IrDBG_Open( void ) ;
void	IrDAAPI	IrDBG_Close( void ) ;
void	IrDAAPI	_DPUTS( int level, const char IrDAFAR *msg ) ;
void	IrDAAPI	_DPUTC( int level, char data ) ;
void	IrDAAPI	_DPUTB( int level, const char IrDAFAR *msg, byte data ) ;
void	IrDAAPI	_DPUTW( int level, const char IrDAFAR *msg, word data ) ;
void	IrDAAPI	_DPUTD( int level, const char IrDAFAR *msg, dword data ) ;
void	IrDAAPI	_DDUMP( int level, const void IrDAFAR *des, word len, word maxLen ) ;

#if defined(__cplusplus)
}
#endif	/*__cplusplus*/
#endif	/*DMSG*/


#endif	/*__irda_h__*/
