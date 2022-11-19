/************************************************************************
;*                                                                      *
;*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      *
;*     All Rights Reserved                                              *
;*                                                                      *
;*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  *
;*     The copyright notice above does not evidence any                 *
;*     actual or intended publication of such source code.              *
;*                                                                      *
;*             File:       irdaconf.h                                   *
;*             Created:    03/23/97                                     *
;*             Author:     Gontaro Kitazumi								*
;*                                                                      *
;*             Version:    1.7                                          *
;*                                                                      *
;***********************************************************************/

#if !defined(__irdaconf_h__)
#define	__irdaconf_h__

#include "target.h"                /* Target specific definitions          */

extern	int	IrDA_MoraTime;

#ifdef	FEATURE_IRDA_UPGRADE
#define	USE_SYSTEM_TASKCALLBACK		//Protocol System Running	OSW:Takagaki	2000/01/21 
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

#undef	USE_MONITOR    // HTG				

//#define		DEVICENAME			"(c) 1996 Okaya Systemware"
#define		SHORTDEVICENAME		"SK Teletech"
#define		DEVICENAME			"(C) 2000 SK Teletech"


/*	IrSIR Capability
*/
#define		IrDACAPABILITY		\
	{							\
		0x01, 0x01, 0x3F,		\
		0x82, 0x01, 0x01,		\
		0x86, 0x01, 0xFF		\
	}



#define		USE_TinyTP										/* TinyTP	���g�p����			*/
#define		USE_IrCOMM										/* IrCOMM	���g�p����			*/
#define		USE_IrOBEX										/* IrOBEX	���g�p����			*/
#define		USE_RTCON										/* RTCON	���g�p����			*/
//#define		USE_IrTranP									/* IrTranP	���g�p����			*/

/* Windows DOS �� ����	*/

//#define		IrDASWAP									/* IrBIOS �Ŏg�p����				*/
#define		IrDAAPI											/*	IrDA �� API 		�}�[�J�[	*/
#define		IrDACB											/*	IrDA �� Callback 	�}�[�J�[	*/

/*	IrDA �݊����ۏ�	*/

#define		IrDAFORWIN95									/*	Windows 95 �o�O���				*/
#define		IrDAPRIMARY										/*	�P���ǃR�[�h����				*/

//#define		IrLAPSLOTTIME		80

#define		IrDABROKENCOMPILER01							/*	�R���p�C���G���[�΍�			*/

#ifdef	FEATURE_IRDA_UPGRADE


#if MODEL_ID == MODEL_D1
#define		IrLAPRECVSZ			555 //???2048						/*      ��M�t���[���T�C�Y			*/
#define		IrLAPSENDSZ			3000//2048						/*      ���M�t���[���T�C�Y          */
#elif MODEL_ID == MODEL_A10
#define		IrLAPRECVSZ			555 //???2048						/*      ��M�t���[���T�C�Y			*/
#define		IrLAPSENDSZ			2500//2048						/*      ���M�t���[���T�C�Y          */
#else
#define		IrLAPRECVSZ			555 //???2048						/*      ��M�t���[���T�C�Y			*/
#define		IrLAPSENDSZ			2048						/*      ���M�t���[���T�C�Y          */
#endif // #if MODEL_ID == MODEL_D1

#define		IrDARECVADD			4							/*	��M�ǉ��o�C�g�� CRC ��			*/
#define		IrDASENDADD			4							/*	���M�ǉ��o�C�g�� CRC ��			*/

#define		IrLAPRECVWINSIZE	1							/*	��M�E�C���h�E�T�C�Y			*/
#define		IrLAPSENDWINSIZE	1							/*	���M�E�C���h�E�T�C�Y			*/

#else	// #ifdef	FEATURE_IRDA_UPGRADE

#define		IrLAPRECVSZ			555// by chos512							/*      ��M�t���[���T�C�Y			*/
#define		IrLAPSENDSZ			512							/*      ���M�t���[���T�C�Y          */

#define		IrDARECVADD			4							/*	��M�ǉ��o�C�g�� CRC ��			*/
#define		IrDASENDADD			4							/*	���M�ǉ��o�C�g�� CRC ��			*/

#define		IrLAPRECVWINSIZE	1							/*	��M�E�C���h�E�T�C�Y			*/
#define		IrLAPSENDWINSIZE	2							/*	���M�E�C���h�E�T�C�Y			*/

#endif	// #ifdef	FEATURE_IRDA_UPGRADE

#define		IrDARECVSZ			(IrLAPRECVSZ+IrDARECVADD)	/*	�����̂P��M�t���[���T�C�Y		*/
#define		IrDASENDSZ			(IrLAPSENDSZ+IrDASENDADD)	/*	�����̂P���M�t���[���T�C�Y		*/
#define		IrDARECVBUFSZ		(IrDARECVSZ*IrLAPRECVWINSIZE)	/*	����M�o�b�t�@�T�C�Y		*/
#define		IrDASENDBUFSZ		(IrDASENDSZ*IrLAPSENDWINSIZE)	/*	�����M�o�b�t�@�T�C�Y		*/
#define		IrLAPSTRICTMINTAT	/*	IrLAP�ŏ��^�[���A���E���h�^�C����0.001ms�ȏ�1ms�����̒l��1ms�Ƃ���	*/

/*		IrLAP ����		*/

//#define		IrLAPNORXCHG								/*	LAP ���[���C�N�X�e�F���W���T�|�[�g���Ȃ�	*/
#define		IrLAPNOSNIFF								/*	LAP �X�j�t���T�|�[�g���Ȃ�					*/

//#define		IrLAP_MORATORIUMTIME	0				/*	LAP 1���ǂł����� RR��Ԃ�������̎���		*/

/*		IrLMP ����		*/

#define		IrLMP_LSAPMAX			5				/*	LMP �o�^�\ LSAP + IAS Client��	*/
#define		IrLMP_IDLETIMER			1000			/*	LMP �Ŗ��ڑ�����LAP��؂鎞��		*/

#define		IrLMP_IASCLASSMAX		5				/*	LMP �o�^�\�N���X��				*/
#define		IrLMP_IASATTRMAX		5				/*	LMP �o�^�\�A�g���r���[�g��		*/
#define		IASATTRVALUEMAX			32				/*	LMP �l�̍ő咷						*/

/*		TinyTP ����		*/

#if defined(USE_TinyTP)
#define		TinyTP_LSAPMAX			3				/*	TinyTP �o�^�\ LSAP ��				*/
#define		TinyTP_SENDBUFSZ		IrLAPSENDSZ		/*	TinyTP ���M�o�b�t�@�T�C�Y			*/
#define		TinyTP_SENDBUFMAX		2				/*	TinyTP ���M�p�t�d�T�C�Y 2�ȏ�ɂ���	*/
#endif	/*	USE_TinyTP	*/

#if defined(USE_IrCOMM)
#define		IrCOMM_LSAPMAX			1				/*	IrCOMM	�o�^�\ LSAP ��			*/
#define		IrCOMM_RXSDUSZ			IrLAPRECVSZ		/*	IrCOMM	��M SDU �T�C�Y				*/
#define		IrCOMM_TXSDUSZ			IrLAPSENDSZ		/*	IrCOMM	���M SDU �T�C�Y				*/

// tasik 00.10.18
#ifdef FEATURE_IRDA_HIGH_RATE
#define		IrCOMM_RXQUESZ		(IrLAPRECVSZ*6)		/*	IrCOMM	��M FIFO �T�C�Y			*/
#define		IrCOMM_TXQUESZ		(IrLAPSENDSZ*6)		/*	IrCOMM	���M FIFO �T�C�Y			*/
#else
// chul.edit
#define		IrCOMM_RXQUESZ		(IrLAPRECVSZ*4)		/*	IrCOMM	��M FIFO �T�C�Y			*/
#define		IrCOMM_TXQUESZ		(IrLAPSENDSZ*4)		/*	IrCOMM	���M FIFO �T�C�Y			*/

//#define		IrCOMM_RXQUESZ		(IrLAPRECVSZ*2)		/*	IrCOMM	��M FIFO �T�C�Y			*/
//#define		IrCOMM_TXQUESZ		(IrLAPSENDSZ*2)		/*	IrCOMM	���M FIFO �T�C�Y			*/
// chul.end
#endif
// tasik end

#define		IrCOMM_CTLQUESZ		64					/*	IrCOMM	���� FIFO �T�C�Y			*/
#endif	/*	USE_IrCOMM	*/

#if defined(USE_IrOBEX)
#define		IrOBEX_LSAPMAX			2				/*	IrOBEX	�o�^�\ LSAP ��			*/
#define		IrOBEX_TXBUFSZ			IrLAPSENDSZ		/*	IrOBEX	���M�o�b�t�@�T�C�Y			*/
#define		IrOBEX_RXBUFSZ			IrLAPSENDSZ		/*	IrOBEX	��M�o�b�t�@�T�C�Y			*/
#endif	/*	USE_IrOBEX	*/

#if !defined(IrDA_LDiv)
#define		IrDA_LDiv(a,b)			(a/b)			/*	����Z�}�N��						*/
#endif	/*!IrDA_LDiv*/
#define		StartTimer				IrSIR_TimerReq	/*	�^�C�}�[�Z�b�g�֐�					*/
#define		StopTimer				IrSIR_TimerRem	/*	�^�C�}�[���Z�b�g�֐�				*/

#include	<irda.h>

#if	0
#define	IrDA_LAPTXEMPTYPRB(f)		P14.1 = !f	//	LAP��TxEmpty�R�[���o�b�N�Ăяo���O��ɌĂ΂��}�N���B
#define	IrDA_LMPDATAINDPRB(f)		P14.2 = !f	//	LMP�̃f�[�^�C���f�B�P�[�V�����Ăяo���O��ɌĂ΂��}�N���B
#define	IrDA_TTPDATAINDPRB(f)		P14.3 = !f	//	TTP�̃f�[�^�C���f�B�P�[�V�����Ăяo���O��ɌĂ΂��}�N���B
#define	IrDA_COMDATAINDPRB(f)		P14.4 = !f	//	COMM�̃f�[�^�C���f�B�P�[�V�����Ăяo���O��ɌĂ΂��}�N���B
#define	IrDA_LMPDATAREQPRB(f)		P14.5 = !f	//	LMP�̃f�[�^�]���v��API���s�O��ŌĂ΂��}�N��
#define	IrDA_TTPDATAREQPRB(f)		P14.6 = !f	//	TTP�̃f�[�^�]���v��API���s�O��ŌĂ΂��}�N��
#define	IrDA_COMMDATAREQPRB(f)		P14.7 = !f	//	IrCOMM�̃f�[�^�]���v��API���s�O��ŌĂ΂��}�N��
#endif

#endif	/*!__irdaconf_h__*/

