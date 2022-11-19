/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_brkr.h
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

#ifndef _SKY_BRKR_H_
#define _SKY_BRKR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "mod.h"
#include "rex.h"
#include "queue.h"
#include "nv.h"
#include "snd.h"
#if SKY_MODEL_ID == SKY_MODEL_B1 || (SKY_MODEL_ID == SKY_MODEL_B2)
#include "musicdb.h"

#elif SKY_MODEL_ID == SKY_MODEL_B0
#include "melodyDB.h"
#include "sndi.h"
#include "snddev.h"
#endif

#include "db.h"
#include "cm.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
//#define REX_SetSignal       rex_set_sigs
//#define REX_ClearSignal     rex_clr_sigs
//#define REX_SetTimer        rex_set_timer

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/* mod.h */
typedef mod_sys_wake_event_type SYS_WAKE_EVENT_E;

/* rex.h */
typedef rex_tcb_type        REX_TCB_T;
typedef rex_sigs_type       REX_SIGNAL;
typedef rex_timer_type      REX_TIMER_T;

/* queue.h */
typedef q_type              QUEUE_T;
typedef q_link_type         QUEUE_LINK_T;

/* nv.h */
typedef nv_cmd_type         NV_COMMAND_T;
typedef nv_items_enum_type  NV_ITEM_E;
typedef nv_item_type        NV_ITEM_U;
typedef nv_stat_enum_type   NV_STATE_E;

/* snd.h */
#if SKY_MODEL_ID == SKY_MODEL_B1 || (SKY_MODEL_ID == SKY_MODEL_B2)
typedef snd_vol_type        SOUND_VOLUME_T;
typedef snd_packets_type    SOUND_COMMAND_U;
typedef snd_predef_type     SOUND_PREDEF_E;
typedef snd_type            DTMF_TONE_T; /* single tone of DTMF */
typedef snd_dtmf_apath_type DTMF_APATH_E; /* DTMF audio path */
typedef snd_mute_control_type SOUND_MUTE_CONTROL_E;
typedef snd_class_type      SOUND_CLASS_E;
#else
typedef snd_status_type     SOUND_STATUS_E;
//typedef snd_device_type     SOUND_DEVICE_E;
typedef snd_method_type     SOUND_METHOD_E;
typedef snd_sound_id_type   SOUND_ID_E;
typedef snd_compact_tone_type SOUND_COMPACT_TONE_T;
typedef snd_compact_sound_type SOUND_COMPACT_SOUND_T;
typedef snd_apath_type      DTMF_APATH_E;
typedef snd_sound_type      SOUND_T;

/* Structure containing a volume setting for each physical path.
*/
typedef struct {
  byte phone;                   /* Volume level for Handset     */
  byte ext;                     /* Volume level for Speaker     */
  byte priv;                    /* Volume level for Privacy HS  */
} snd_vol_type;

/* DTMF audio output path
*/
typedef enum {
  SND_DTMF_APATH_LOCAL,         /* DTMF's on local audio        */
  SND_DTMF_APATH_TX,            /* Transmit DTMFs               */
  SND_DTMF_APATH_BOTH,          /* Tx and sound DTMF's locally  */
  SND_DTMF_APATH_MUTE           /* Mute the DTMFs               */
} snd_dtmf_apath_type;

/* Each audio path has one or more tone generators. The sound's class
** determines which generator is used to play it.
*/
typedef enum {
  SND_CLASS_DEFAULT = 0, /* Same as SND_CLASS_BEEP                         */
  SND_CLASS_BEEP,        /* Use the path's ringer, or voice generator      */
  SND_CLASS_RING,        /* Use the path's ringer or voice generator       */
  SND_CLASS_VOICE_BEEP,  /* Use the path's voice generator at tone volume  */
  SND_CLASS_VOICE,       /* Use the path's voice generator at voice volume */
#ifdef SND_ADSP_TONEGEN
#error code not present
#endif /* SND_ADSP_TONEGEN */
#ifdef SND_AUX_ALERT
#error code not present
#endif /* SND_AUX_ALERT */
  SND_CLASS_MAX          /* Always last in the list                        */
} snd_class_type;



typedef snd_vol_type        SOUND_VOLUME_T;
typedef snd_packets_type    SOUND_COMMAND_U;
typedef int     SOUND_PREDEF_E;
typedef snd_type            DTMF_TONE_T; /* single tone of DTMF */
//typedef snd_dtmf_apath_type DTMF_APATH_E; /* DTMF audio path */
typedef snd_mute_control_type SOUND_MUTE_CONTROL_E;
typedef snd_class_type      SOUND_CLASS_E;

#endif

/* cai.h */
typedef byte                TONE_SIGNAL_E;
typedef byte                ISDN_ALERT;
typedef byte                SIGNAL_TYPE_E;

/* musicdb_b1.h */
/* not yet ... b1 */
//typedef mus_id_type         MUSIC_ID_E;
typedef int                 MUSIC_ID_E;

/* db.h */
typedef db_items_value_type DB_ITEM_VALUE_U;

/* qw.h */
//typedef qword               QWORD;

/* clk.h */
typedef clk_cb_type         CLOCK_CALLBACK;

/* uiusmsl.h */
//typedef ui_sms_op_status_type SMS_OP_STATUS_E;

/* cm.h */
typedef cm_nam_e_type       CM_NAM_E;


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

#endif  /* _SKY_BRKR_H_ */

