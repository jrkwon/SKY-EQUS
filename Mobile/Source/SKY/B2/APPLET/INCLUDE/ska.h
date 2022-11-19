/*
** ============================================================================
**
** Project: 
**     Athens
**
** File:
**     eqa.h
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
** 2000-07-20 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKA_H_
#define _SKA_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "customer.h"

#include "sky.h"
#include "ska_off.h"
#include "ska_strt.h"
#include "ska_call.h"
#include "ska_clid.h"
#include "ska_nam.h"
#include "ska_idle.h"
#include "ska_bstr.h"
#include "ska_bapi.h"
#include "ska_butl.h"
#include "ska_pbo.h"
#include "ska_sbel.h"
#include "ska_scal.h"
#include "ska_schl.h"
#include "ska_sfil.h"
#include "ska_shld.h"
#include "ska_slst.h"
#include "ska_alrm.h"
#include "ska_memo.h"
#include "ska_menu.h"
#include "ska_vmemo.h"
#include "ska_test.h"
#include "ska_efs.h"
#include "ska_calc.h"
#include "ska_mo.h"
#include "ska_sms.h"
#include "ska_info.h"
#include "ska_ligt.h"
#include "ska_lock.h"
#include "ska_ntop.h"
#include "ska_nams.h"
#include "ska_tnum.h"
#include "ska_2get.h"
#include "ska_011.h"
#include "ska_init.h"
#include "ska_ver.h"
#include "ska_vcmm.h"
#include "ska_volm.h"
#include "ska_code.h"
#include "ska_stop.h"
#include "ska_wtim.h"
#include "ska_bell.h"
#include "ska_smsl.h"
#include "ska_aans.h"
#include "ska_dscf.h"
#include "ska_aapi.h"
#include "ska_vcore.h"
#include "ska_vms.h"
#include "ska_pic.h"
#include "ska_add.h"
#include "ska_crec.h"
#include "ska_hdon.h"
#include "ska_mybe.h"
#include "ska_dmgr.h"
#include "ska_dsvc.h"
#include "ska_game.h"
#include "ska_mail.h"
#include "ska_smtp.h"
#include "ska_pop3.h"
#include "ska_eif.h"
#include "ska_thme.h"
#include "ska_clst.h"
#include "ska_017.h"

//#ifdef FEATURE_SKTT_PM
#include "ska_vm.h"
//#include "ska_vmfl.h"
//#endif/*FEATURE_SKTT_PM*/ 

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
#include "ska_cam.h"
#endif

#ifdef	FEATURE_WAVELET	//	khekim 01/01/17
#include "ska_wav.h"
#endif	//	#ifdef	FEATURE_WAVELET

#ifdef FEATURE_WAP
#include "ska_WAPAPP.h"
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_GVM
#include "ska_gvm.h"
#endif //FEATURE_GVM

#ifdef FEATURE_SKVM
#include "ska_skvm.h"
#endif //FEATURE_SKVM

#ifdef FEATURE_USE_IRDA
#include "ska_irph.h"
#include "ska_irda.h"
#endif

#ifdef FEATURE_SKTT_EQUS_LOCATION
#include "ska_loca.h"
#endif // FEATURE_SKTT_EQUS_LOCATION

#ifdef FEATURE_MCOMMERCE
#include "ska_mwlt.h"
#endif // FEATURE_MCOMMERCE

#ifdef FEATURE_CHARACTER_BELL
#include "ska_cbell.h"
#endif // FEATURE_CHARACTER_BELL
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
#define  BEEP_VOL  0          //for adjust Key tone volume
#define  VOICE_VOL 1          //for adjust voice volume
#define  MUSIC_VOL 2          //for adjust music bell volume

#define  NUMBER_PANNEL_COLOR   EQS_RGB(56,205,0)
#define  NUMBER_TEXT_COLOR     BLACK


extern HCONTROL g_hMenuControl;    // for changing menu mode ( icon or text )
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_H_ */
