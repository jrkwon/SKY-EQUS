/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_all.h
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
** 2000-10-30 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_ALL_H_
#define _SKY_ALL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
/* temp ..... not sms */
//#undef FEATURE_UASMS
//#define FEATURE_YMU757B_MELODY /* temp temp temp temp temp */

/*----------------------------------------------------------------------------
**  SKY UI basic support
*/
#include <stdio.h>
#include "eqs.h"                /* equs */
#include "sky_feat.h"           /* feature */
#include "sky_def.h"            /* model definition */
#include "sky_tokn.h"           /* sky string token */
#include "sky_mem.h"
#include "sky_efs.h"            /* EFS service */
#ifdef WIN32
#include "skywefs.h"
#else /* REX */
#include "skyrefs.h"
#endif
#include "sky_file.h"           /* sky data file list */
#include "sky_evnt.h"           /* sky event */
#include "sky_util.h"
#include "sky_img.h"            /* built in image type */
#include "sky_data.h"
#include "sky_mtxt.h"
/*----------------------------------------------------------------------------
**  SKY UI with DMSS 
*/
//#if SKY_MODEL_ID == SKY_MODEL_B0		// jnlee 00.12.12
#include "sky_lcd.h"
#include "sky_slcd.h"
//#endif
#if SKY_MODEL_ID == SKY_MODEL_B0
#include "biogB0.h"
#elif SKY_MODEL_ID == SKY_MODEL_B1
#include "biogB1.h"
#elif SKY_MODEL_ID == SKY_MODEL_B2
#include "biogB2.h"
#else
#error Undefined SKY_MODEL_ID
#endif
#include "sky_brkr.h"           /* broker between sky and equs */
#include "sky_conf.h"           /* system configuration */
#include "sky_ui.h"             /* user interface task */
#include "sky_hcmd.h"           /* command handler */
#include "sky_hkey.h"           /* key handler */
#include "sky_hsig.h"           /* signal handler */
#include "sky_cm.h"             /* call manager */
#include "sky_hs.h"             /* handset task */
#include "sky_hcbt.h"           /* callback timer handler */
#include "sky_scrn.h"           /* screen interace */
#include "sky_snd.h"            /* sound interface */
#include "sky_nv.h"
#include "sky_rex.h"
#include "sky_annu.h"
#include "sky_cml.h"

#if MODEL_ID==MODEL_B2	// by jnlee 01.06.20
	// no smb
#else
#include "smb.h"
#endif

#ifdef FEATURE_USE_IRDA
#include "IrDATool.h"
#include "sky_irda.h"
#endif

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
typedef void (SKY_APPLET)(SKY_EVENT_T* pEvent); 

#define EQS_START_CHILD(s) \
    EQS_StartChild((EQS_APPLET*)(s))
#define EQS_START_CHILD_EVENT(s, e, w ,dw) \
    EQS_StartChildEvent((EQS_APPLET*)(s),(EQS_EVENT_ID)(e), w, dw)
#define EQS_START_CHILD_CURRENT_EVENT(s) \
    EQS_StartChildCurrentEvent((EQS_APPLET*)(s))
#define EQS_START_SIBLING(s, f) \
    EQS_StartSibling((EQS_APPLET*)(s), f)
#define EQS_START_SIBLING_EVENT(s, e, w, dw, f) \
    EQS_StartSiblingEvent((EQS_APPLET*)(s), (EQS_EVENT_ID)(e), w, dw, f)
#define EQS_START_SIBLING_CURRENT_EVENT(s, f) \
    EQS_StartSiblingCurrentEvent((EQS_APPLET*)(s), f)

#define EQS_END_TO(r) \
    EQS_EndTo(r)
#define EQS_END_TO_PARAM(r, w, dw) \
    EQS_EndToParam(r, w, dw)
#define EQS_END_TO_EVENT(r, e, w, dw) \
    EQS_EndToEvent(r, (EQS_EVENT_ID)(e), w, dw)
#define EQS_END_TO_CURRENT_EVENT(r) \
    EQS_EndToCurrentEvent(r)
#define EQS_END_DEST(s) \
    EQS_EndDest((EQS_APPLET *)(s))
#define EQS_END_DEST_PARAM(s, w, dw) \
    EQS_EndDestParam((EQS_APPLET *)(s), w, dw)
#define EQS_END_DEST_EVENT(s, e, w, dw) \
    EQS_EndDestEvent((EQS_APPLET *)(s), (EQS_EVENT_ID)(e), w, dw)
#define EQS_END_DEST_CURRENT_EVENT(s) \
    EQS_EndDestCurrentEvent((EQS_APPLET *)(s))
#define EQS_END() \
    EQS_End()
#define EQS_END_PARAM(w, dw) \
    EQS_EndParam(w, dw)
#define EQS_END_EVENT(e, w, dw) \
    EQS_EndEvent((EQS_EVENT_ID)(e), w, dw)
#define EQS_END_CURRENT_EVENT() \
    EQS_EndCurrentEvent()

#define EQS_SEND_EVENT(e, w, dw) \
    EQS_SendEvent((EQS_EVENT_ID)(e), w, dw)
#define EQS_PASS_CURRENT_EVENT() \
    EQS_PassCurrentEvent()
#define EQS_GET_CATEGORY(e) \
    EQS_GetCategory((EQS_EVENT_ID)(e))
 
#define EQS_PIN_UP() \
    EQS_PinUp()
#define EQS_GET_CHILD() \
    ((SKY_APPLET *)EQS_GetChild())
#define EQS_GET_PARENT() \
    ((SKY_APPLET *)EQS_GetParent())
#define EQS_GET_ANCESTOR(nTh)\
	((SKY_APPLET *)EQS_GetAncestor(nTh))
#define EQS_GET_APPLET() \
    ((SKY_APPLET *)EQS_GetApplet())
#define EQS_GET_APPLET_TIMER_INFO() \
    EQS_GetAppletTimerInfo()

#define EQS_DEF_EVENT_PROC(s) \
    SKY_DefEventProc(s)


#define EQS_START_CHILD(s) \
    EQS_StartChild((EQS_APPLET*)(s))
#define EQS_START_CHILD_EVENT(s, e, w ,dw) \
    EQS_StartChildEvent((EQS_APPLET*)(s),(EQS_EVENT_ID)(e), w, dw)
#define EQS_START_CHILD_CURRENT_EVENT(s) \
    EQS_StartChildCurrentEvent((EQS_APPLET*)(s))
#define EQS_START_SIBLING(s, f) \
    EQS_StartSibling((EQS_APPLET*)(s), f)
#define EQS_START_SIBLING_EVENT(s, e, w, dw, f) \
    EQS_StartSiblingEvent((EQS_APPLET*)(s), (EQS_EVENT_ID)(e), w, dw, f)
#define EQS_START_SIBLING_CURRENT_EVENT(s, f) \
    EQS_StartSiblingCurrentEvent((EQS_APPLET*)(s), f)

#define SKY_DEF_WIN_X   0
#define SKY_DEF_WIN_Y   0
#define SKY_DEF_WIN_DX  SKY_SCREEN_WIDTH
#define SKY_DEF_WIN_DY  (SKY_SCREEN_HEIGHT-EQS_SWINDOW_HEADER_HEIGHT-SKY_COMMANDLINE_HEIGHT)

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

#endif  /* _SKY_ALL_H_ */
