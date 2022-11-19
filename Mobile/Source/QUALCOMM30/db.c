/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     D M S S    D a t a b a s e

GENERAL DESCRIPTION
  This module implements the necessary functions to provide to higher
  layer modules a simple interface to the DMSS database.

EXTERNALIZED FUNCTIONS
  db_put() and db_get()

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The initialization of each one of the items in the database is
  responsibility of the owner of the item.

Copyright (c) 1992,1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1992,1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999           by QUALCOMM Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   O:/src/asw/COMMON/vcs/db.c_v   1.2   24 Jan 2001 19:08:22   ydharc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/24/01   ych     Merged JCDMA features.
10/16/00   lad     Removed event reports (these were for test).
12/14/99   lad     Added event reports.
05/17/99   SIU     Added hold origination counter db item.
12/10/98   dna     Fix CR7143.  Add new db item for powerup scanning
04/24/98   dbd     Added DB_VOICE_AS_DATA for service negotiation
12/20/96   rdh     Changed functions to Pascal calling convention to save ROM.
12/21/95   jca     Added DB_BS_P_REV for 13K/8K service negotiation.
10/26/95   jca     Deleted DB_ORIG_MIN_INDEX.
10/04/95   rdh     Separated SLEEP_ACTIVE from SLEEP_MODE.
09/20/95   rdh     Added SLEEP_ACTIVE to tell whether or not we are sleeping.
08/10/95   cah     Added Voice Privacy active flag.
07/06/95   rah     Added 13K_VOC_AVAILABLE for 13kbps vocoder support.
06/28/95   rdh     Added ACQ_MODE for setting acquisition mode.
06/21/95   rdh     Added SLEEP_MODE for disabling sleep.
12/09/94   ras     Added OFF_HOOK for hook switch state.
02/02/94   jjw     Removed SIO_INIT, Added DB_DATA_SRVC_ENABLED (noticed the
                   addition of DB_RF_900MHZ???)
06/29/93   jca     Fixed some Lint errors.
04/27/93   jai     Deleted DB_SID_NID_LIST item.
04/13/93   jjw     Added DB_SIO_INIT item
09/16/92   ARH     Added DB_VOX
09/09/92   ARH     Added DB_HOLD_ORIG
09/04/92   ARH     Added DB_ORIG_MIN_INDEX
09/02/92   ARH     Added DB_CODE_CHAN and DB_PILOT_PN_OFF
09/01/92   JJW     Changed 'udpated' to 'updated' to conform to English
07/24/92   ARH     Added DB_RF_MODE item
06/10/92   ARH     Added SID_NID_LIST and SYSTEM_STATE item
04/22/92   ARH     Updated per SDD review
03/16/92   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "db.h"
#include "rex.h"
#include "msg.h"
#include "err.h"

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*
** Database Items
*/

LOCAL boolean      db_in_use;   /* Call in progress indication             */
LOCAL boolean      db_no_svc;   /* Cannot find cellular service indication */
LOCAL boolean      db_digital;  /* Indicates CDMA/Analog mode of operation */
LOCAL boolean      db_conv;     /* Indicates Conversation is taking place  */
LOCAL db_roam_type db_roam;     /* Roaming status indication               */
LOCAL word         db_channel;  /* Current channel mobile is tuned to      */
LOCAL byte         db_code_channel; /* Current code channel                */
LOCAL word         db_pilot_pn_off; /* Current pilot PN offset             */
LOCAL word         db_sid;      /* Current System ID                       */
LOCAL word         db_locaid;   /* Current Location ID                     */
LOCAL word         db_nid;      /* Current Network ID                      */
LOCAL byte         db_lp_sec;   /* Leap Seconds from System Time           */
LOCAL byte         db_ltm_off;  /* Local time offset from System Time      */
LOCAL boolean      db_daylt;    /* Daylight savings time indicator         */
LOCAL db_dmss_state_type  db_dmss_state; /* Current system state           */
LOCAL byte         db_power;    /* Current mobile output power level       */
LOCAL boolean      db_privacy_mode; /* Current privacy mode                */
LOCAL word         db_rssi;     /* Current RSSI level                      */
LOCAL word         db_cdma_rssi; /* Current CDMA RSSI level                */
LOCAL word         db_battery_level; /* Current battery level              */
LOCAL word         db_temperature;   /* Current temperature                */
LOCAL db_rf_mode_type db_rf_mode;    /* Current RF mode                    */
LOCAL boolean      db_hold_orig;     /* Hold origination commands flag     */
LOCAL word         db_hold_orig_cnt; /* Hold origination change counter    */
LOCAL boolean      db_powerup_scanning; /* Hold origination on powerup     */
LOCAL boolean      db_on_hook;       /* Is the subscriber unit on hook?    */
LOCAL boolean      db_vox;           /* Current VOX mode                   */
LOCAL boolean      db_data_srvc_enabled;  /* current state of Data task    */
LOCAL word         db_sleep_mode;    /* Is sleep enabled?                  */
LOCAL word         db_sleep_active;  /* Is sleep currently occurring?      */
LOCAL word         db_acq_mode;      /* Type of CDMA acquisition desired   */
LOCAL boolean      db_13k_voc_available;/* Flag to indicate 13k capability */
LOCAL boolean      db_voice_privacy;    /* Flag to indicate Voice Privacy  */
LOCAL byte         db_bs_p_rev;      /* Base station's protocol rev level  */
LOCAL db_voice_as_data_type db_voice_as_data; /* Answer next call as data  */
#if defined (MMI_TREX) || (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)
LOCAL word         db_ecio;          /* Base station's protocol rev level  */
LOCAL word         db_rx_state;      /* Base station's protocol rev level  */
LOCAL byte         db_num_sup_lcd;  //kjh99.8.13
LOCAL word         db_reg_zone;      /* Registration zone */
LOCAL word         db_base_id;       /* Base station identification */

LOCAL db_pwron_type db_pwron;
#endif // TREX_C5_B5

#ifdef FEATURE_SCH_FER
LOCAL word         db_sch_fer_lcd;
#endif
#ifdef FEATURE_FCH_FER
LOCAL word         db_fch_fer_lcd;
#endif
/* <EJECT> */
/*===========================================================================

                         PUBLIC PROCEDURES

===========================================================================*/

/*===========================================================================

FUNCTION db_put

DESCRIPTION
  Changes the value of the item specified. The tasks that need to know that
  the value of the item has changed are notified.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void db_put
(
  db_items_type        item,        /* Item to be changed                  */
  db_items_value_type  *value_ptr   /* Value to change the item to         */

)
{

  switch(item) {

    case DB_IN_USE:
      db_in_use  = value_ptr->in_use;
      MSG_LOW("DB_IN_USE updated to %d",db_in_use,0,0);

      /* Add notifications here */
      break;

    case DB_NO_SVC:
      db_no_svc  = value_ptr->no_svc;
      MSG_LOW("DB_NO_SVC updated to %d",db_no_svc,0,0);

      /* Add notifications here */
      break;

    case DB_DIGITAL:
      db_digital = value_ptr->digital;
      MSG_LOW("DB_DIGITAL updated to %d",db_digital,0,0);

      /* Add notifications here */
      break;

    case DB_CONV:
      db_conv    = value_ptr->conv;
      MSG_LOW("DB_CONV updated to %d",db_conv,0,0);

      /* Add notifications here */
      break;

    case DB_ROAM:
      db_roam    = value_ptr->roam;
      MSG_LOW("DB_ROAM updated to %d",db_roam,0,0);

      /* Add notifications here */
      break;

    case DB_CHANNEL:
      db_channel = value_ptr->channel;
      MSG_LOW("DB_CHANNEL updated to %d",db_channel,0,0);

      /* Add notifications here */
      break;

    case DB_CODE_CHANNEL:
      db_code_channel = value_ptr->code_channel;
      MSG_LOW("DB_CODE_CHANNEL updated to %d",db_code_channel,0,0);

      /* Add notifications here */
      break;

    case DB_PILOT_PN_OFF:
      db_pilot_pn_off = value_ptr->pilot_pn_off;
      MSG_LOW("DB_PILOT_PN_OFF updated to %d",db_pilot_pn_off,0,0);

      /* Add notifications here */
      break;

    case DB_SID:
      db_sid     = value_ptr->sid;
      MSG_LOW("DB_SID updated to %d",db_sid,0,0);

      /* Add notifications here */
      break;

    case DB_LOCAID:
      db_locaid  = value_ptr->locaid;
      MSG_LOW("DB_LOCAID updated to %d",db_locaid,0,0);

      /* Add notifications here */
      break;

    case DB_NID:
      db_nid     = value_ptr->nid;
      MSG_LOW("DB_NID updated to %d",db_nid,0,0);

      /* Add notifications here */
      break;

    case DB_LP_SEC:
      db_lp_sec  = value_ptr->lp_sec;
      MSG_LOW("DB_LP_SEC updated to %d",db_lp_sec,0,0);

      /* Add notifications here */
      break;

    case DB_LTM_OFF:
      db_ltm_off = value_ptr->ltm_off;
      MSG_LOW("DB_LTM_OFF updated to %d",db_ltm_off,0,0);

      /* Add notifications here */
      break;

    case DB_DAYLT:
      db_daylt   = value_ptr->daylt;
      MSG_LOW("DB_DAYLT updated to %d",db_daylt,0,0);

      /* Add notifications here */
      break;

    case DB_DMSS_STATE:
      db_dmss_state = value_ptr->dmss_state;
      MSG_LOW("DB_DMSS_STATE updated to %d",value_ptr->dmss_state,0,0);

      /* Add notifications here */
      break;

    case DB_POWER:
      db_power   = value_ptr->power;
      MSG_LOW("DB_POWER updated to %d",db_power,0,0);

      /* Add notifications here */
      break;

    case DB_PRIVACY_MODE:
      db_privacy_mode = value_ptr->privacy_mode;
      MSG_LOW("DB_PRIVACY_MODE updated to %d",db_privacy_mode,0,0);

      /* Add notifiactions here */
      break;

    case DB_RSSI:
      db_rssi = value_ptr->rssi;
      MSG_LOW("DB_RSSI updated to %d",db_rssi,0,0);

      /* Add notifications here */
      break;

    case DB_CDMA_RSSI:
      db_cdma_rssi = value_ptr->cdma_rssi;
      MSG_LOW("DB_CDMA_RSSI updated to %d",db_cdma_rssi,0,0);

      /* Add notifications here */
      break;

    case DB_BATTERY_LEVEL:
      db_battery_level = value_ptr->battery_level;
      MSG_LOW("DB_BATTERY_LEVEL updated to %d",db_battery_level,0,0);

      /* Add notifications here */
      break;

    case DB_TEMPERATURE:
      db_temperature = value_ptr->temperature;
      MSG_LOW("DB_TEMPERATURE updated to %d",db_temperature,0,0);

      /* Add notifications here */
      break;

    case DB_RF_MODE:
      db_rf_mode = value_ptr->rf_mode;
      MSG_LOW("DB_RF_MODE updated to %d",db_rf_mode,0,0);

      /* Add notifications here */
      break;

    case DB_HOLD_ORIG:
      db_hold_orig = value_ptr->hold_orig;
      MSG_LOW("DB_HOLD_ORIG updated to %d",db_hold_orig,0,0);

      /* Add notifications here */
      break;

    case DB_POWERUP_SCANNING:
      db_powerup_scanning = value_ptr->powerup_scanning;
      MSG_LOW("DB_POWERUP_SCANNING updated to %d",db_powerup_scanning,0,0);

      /* Add notifications here */
      break;

    case DB_ON_HOOK:
      db_on_hook = value_ptr->on_hook;
      MSG_LOW("DB_ON_HOOK updated to %d",db_on_hook,0,0);

      /* Add notifications here */
      break;

    case DB_VOX:
      db_vox = value_ptr->vox;
      MSG_LOW("DB_VOX updated to %d",db_vox,0,0);

      /* Add notifications here */
      break;

    case DB_DATA_SRVC_STATE:
      db_data_srvc_enabled = value_ptr->data_srvc_enabled;
      MSG_LOW("DB_DATA_SRVC_ENABLED updated to %d",db_data_srvc_enabled,0,0);

      /* Add notifications here */
      break;

    case DB_SLEEP_MODE:
      db_sleep_mode = value_ptr->sleep_mode;
      MSG_LOW("DB_SLEEP_MODE updated to %d",db_sleep_mode,0,0);

      /* Add notifications here */
      break;

    case DB_SLEEP_ACTIVE:
      db_sleep_active = value_ptr->sleep_active;
      MSG_LOW("DB_SLEEP_ACTIVE updated to %d",db_sleep_active,0,0);

      /* Add notifications here */
      break;

    case DB_ACQ_MODE:
      db_acq_mode = value_ptr->acq_mode;
      MSG_LOW("DB_ACQ_MODE updated to %d",db_acq_mode,0,0);

      /* Add notifications here */
      break;

    case DB_13K_VOC_AVAILABLE:
      db_13k_voc_available = value_ptr->voc13k_available;
      MSG_LOW("DB_13K_VOC_AVAILABLE updated to %d",db_13k_voc_available,0,0);

      /* Add notifications here */
      break;

    case DB_VOICE_PRIVACY:
      db_voice_privacy = value_ptr->voice_privacy;
      MSG_LOW("DB_VOICE_PRIVACY updated to %d",db_voice_privacy,0,0);

      /* Add notifications here */
      break;

    case DB_BS_P_REV:
      db_bs_p_rev = value_ptr->bs_p_rev;
      MSG_LOW("DB_BS_P_REV updated to %d",db_bs_p_rev,0,0);

      /* Add notifications here */
      break;

    case DB_VOICE_AS_DATA:
      db_voice_as_data = value_ptr->voice_as_data;
      MSG_LOW("DB_VOICE_AS_DATA updated to %d",db_voice_as_data,0,0);

      /* Add notifications here */
      break;

#if defined (MMI_TREX) || (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)
    case DB_ECIO:
      db_ecio = value_ptr->ecio;
      MSG_LOW("DB_ECIO updated to %d",db_ecio,0,0);
      /* Add notifications here */
      break;
    case DB_RX_STATE:
      db_rx_state = value_ptr->rx_state;
      /* Add notifications here */
      break;
    case DB_NUM_SUP:							 //kjh99.8.13
      db_num_sup_lcd = value_ptr->num_sup_lcd;
      /* Add notifications here */
      break;
    case DB_REG_ZONE:
      db_reg_zone     = value_ptr->reg_zone;
      MSG_LOW("DB_REG_ZONE updated to %d",db_reg_zone,0,0);
      /* Add notifications here */
      break;
    case DB_BASE_ID:
      db_base_id     = value_ptr->base_id;
      MSG_LOW("DB_BASE_ID updated to %d",db_base_id,0,0);
      /* Add notifications here */
      break;

	case DB_PWRON:
		db_pwron = value_ptr->pwron;
		break;
#endif // #if defined (MMI_TREX) || (MMI_TYPE == MMI_A10)

#ifdef FEATURE_SCH_FER
    case DB_SCH_FER:	
      db_sch_fer_lcd = value_ptr->sch_fer_lcd;
      /* Add notifications here */
      break;
#endif
#ifdef FEATURE_FCH_FER
    case DB_FCH_FER:	
      db_fch_fer_lcd = value_ptr->fch_fer_lcd;
      /* Add notifications here */
      break;
#endif
    default:
      ERR_FATAL("Bad item parameter %d ",item, 0,0);

  } /* end switch on item */

} /* end db_put */

/* <EJECT> */
/*===========================================================================

FUNCTION db_get

DESCRIPTION
  Retrieve the value of an item from the database

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void db_get
(
  db_items_type         item,       /* Item to get value from               */
  db_items_value_type  *value_ptr   /* Buffer to retrieve item into         */
)
{

  switch(item) {

    case DB_IN_USE:
      value_ptr->in_use              = db_in_use;
      break;

    case DB_NO_SVC:
      value_ptr->no_svc              = db_no_svc;
      break;

    case DB_DIGITAL:
      value_ptr->digital             = db_digital;
      break;

    case DB_CONV:
      value_ptr->conv                = db_conv;
      break;

    case DB_ROAM:
      value_ptr->roam                = db_roam;
      break;

    case DB_CHANNEL:
      value_ptr->channel             = db_channel;
      break;

    case DB_CODE_CHANNEL:
      value_ptr->code_channel        = db_code_channel;
      break;

    case DB_PILOT_PN_OFF:
      value_ptr->pilot_pn_off        = db_pilot_pn_off;
      break;

    case DB_SID:
      value_ptr->sid                 = db_sid;
      break;

    case DB_LOCAID:
      value_ptr->locaid              = db_locaid;
      break;

    case DB_NID:
      value_ptr->nid                 = db_nid;
      break;

    case DB_LP_SEC:
      value_ptr->lp_sec              = db_lp_sec;
      break;

    case DB_LTM_OFF:
      value_ptr->ltm_off             = db_ltm_off;
      break;

    case DB_DAYLT:
      value_ptr->daylt               = db_daylt;
      break;

    case DB_DMSS_STATE:
      value_ptr->dmss_state          = db_dmss_state;
      break;

    case DB_POWER:
      value_ptr->power               = db_power;
      break;

    case DB_PRIVACY_MODE:
      value_ptr->privacy_mode        = db_privacy_mode;
      break;

    case DB_RSSI:
      value_ptr->rssi                = db_rssi;
      break;

    case DB_CDMA_RSSI:
      value_ptr->cdma_rssi           = db_cdma_rssi;
      break;

    case DB_BATTERY_LEVEL:
      value_ptr->battery_level       = db_battery_level;
      break;

    case DB_TEMPERATURE:
      value_ptr->temperature         = db_temperature;
      break;

    case DB_RF_MODE:
      value_ptr->rf_mode             = db_rf_mode;
      break;

    case DB_HOLD_ORIG:
      value_ptr->hold_orig           = db_hold_orig;
      break;

    case DB_POWERUP_SCANNING:
      value_ptr->powerup_scanning    = db_powerup_scanning;
      break;

    case DB_ON_HOOK:
      value_ptr->on_hook             = db_on_hook;
      break;

    case DB_VOX:
      value_ptr->vox                 = db_vox;
      break;

    case DB_DATA_SRVC_STATE:
        value_ptr->data_srvc_enabled = db_data_srvc_enabled;
        break;

    case DB_SLEEP_MODE:
        value_ptr->sleep_mode        = (db_sleep_mode_type) db_sleep_mode;
        break;

    case DB_SLEEP_ACTIVE:
        value_ptr->sleep_active      = (db_sleep_active_type) db_sleep_active;
        break;

    case DB_ACQ_MODE:
        value_ptr->acq_mode          = (db_acq_mode_type) db_acq_mode;
        break;

    case DB_13K_VOC_AVAILABLE:
        value_ptr->voc13k_available = db_13k_voc_available;
        break;

    case DB_VOICE_PRIVACY:
        value_ptr->voice_privacy    = db_voice_privacy;
        break;

    case DB_BS_P_REV:
        value_ptr->bs_p_rev         = db_bs_p_rev;
        break;

    case DB_VOICE_AS_DATA:
        value_ptr->voice_as_data    = db_voice_as_data;
        break;

#if defined (MMI_TREX) || (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)
    case DB_ECIO:
        value_ptr->ecio			    = db_ecio;
        break;

    case DB_RX_STATE:
        value_ptr->rx_state		    = db_rx_state;
        break;

    case DB_NUM_SUP:
      value_ptr->num_sup_lcd		= db_num_sup_lcd;
      /* Add notifications here */
      break;

    case DB_REG_ZONE:
      value_ptr->reg_zone		    = db_reg_zone;
      /* Add notifications here */
      break;

    case DB_BASE_ID:
      value_ptr->base_id		    = db_base_id;
      /* Add notifications here */
      break;

	case DB_PWRON:
		value_ptr->pwron = db_pwron;
		break;
#endif // #if defined (MMI_TREX) || (MMI_TYPE == MMI_A10)

#ifdef FEATURE_SCH_FER
    case DB_SCH_FER:
      value_ptr->sch_fer_lcd		= db_sch_fer_lcd;
      /* Add notifications here */
      break;
#endif
#ifdef FEATURE_FCH_FER
    case DB_FCH_FER:
      value_ptr->fch_fer_lcd		= db_fch_fer_lcd;
      /* Add notifications here */
      break;
#endif
    default:
      ERR_FATAL("Bad item parameter %d ",item, 0,0);

  } /* end switch on item */

} /* end db_get */

/* <EJECT> */
/*===========================================================================

FUNCTION DB_INIT

DESCRIPTION
  Initializes the database to default values. No notifications are made
  when setting the values to default.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void db_init( void )
{
  word index;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* ----------------------------------------------------------
  ** Initialize each item of the database to some default value
  ** ---------------------------------------------------------- */
  for (index=0; index < ((word) DB_LAST_ITEM); index++) {
    switch(index) {
      case DB_IN_USE:
        db_in_use = FALSE;
        break;

      case DB_NO_SVC:
#ifdef FEATURE_JCDMA
#error code not present
#else
        db_no_svc = TRUE;
#endif /* FEATURE_JCDMA */
        break;

      case DB_DIGITAL:
#ifdef FEATURE_JCDMA
#error code not present
#else
        db_digital = FALSE;
#endif /* FEATURE_JCDMA */
        break;

      case DB_CONV:
        db_conv = FALSE;
        break;

      case DB_ROAM:
        db_roam = DB_ROAM_OFF;
        break;

      case DB_CHANNEL:
        db_channel = 0;
        break;

      case DB_CODE_CHANNEL:
        db_code_channel = 0;
        break;

      case DB_PILOT_PN_OFF:
        db_pilot_pn_off = 0;
        break;

      case DB_SID:
        db_sid = 0;
        break;

      case DB_LOCAID:
        db_locaid = 0;
        break;

      case DB_NID:
        db_nid = 0;
        break;

      case DB_LP_SEC:
        db_lp_sec = 0;
        break;

      case DB_LTM_OFF:
        db_ltm_off = 0;
        break;

      case DB_DAYLT:
        db_daylt = 0;
        break;

      case DB_DMSS_STATE:
        db_dmss_state = DB_DMSS_POWERUP_S;
        break;

      case DB_POWER:
        db_power = 0;
        break;

      case DB_PRIVACY_MODE:
        db_privacy_mode = FALSE;
        break;

      case DB_RSSI:
        db_rssi = 0;
        break;

      case DB_CDMA_RSSI:
        db_cdma_rssi = 0;
        break;

      case DB_BATTERY_LEVEL:
        db_battery_level = 0;
        break;

      case DB_TEMPERATURE:
        db_temperature = 0;
        break;

      case DB_RF_MODE:
        db_rf_mode = DB_RF_ANALOG;  /* Just assume a mode */
        break;

      case DB_HOLD_ORIG:
        db_hold_orig = TRUE;
        break;

      case DB_POWERUP_SCANNING:
        db_powerup_scanning = FALSE;
        break;

      case DB_ON_HOOK:
        db_on_hook = TRUE;
        break;

      case DB_VOX:
        db_vox = FALSE;
        break;

      case DB_DATA_SRVC_STATE:
        db_data_srvc_enabled = FALSE;
        break;

      case DB_SLEEP_MODE:
        db_sleep_mode = DB_SLEEP_ENABLED;
        break;

      case DB_SLEEP_ACTIVE:
        db_sleep_active = DB_SLEEP_OFF;
        break;

      case DB_ACQ_MODE:
        db_acq_mode = DB_ACQ_FULL;
        break;

      case DB_13K_VOC_AVAILABLE:
        db_13k_voc_available = FALSE;
        break;

      case DB_VOICE_PRIVACY:
        db_voice_privacy = FALSE;
        break;

      case DB_BS_P_REV:
        db_bs_p_rev = 1;
        break;

      case DB_VOICE_AS_DATA:
        db_voice_as_data = DB_VOICE_AS_DATA_NEVER;
        break;

#if defined (MMI_TREX) || (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)
      case DB_ECIO:
        db_ecio = 0;
        break;
      case DB_RX_STATE:
        db_rx_state = 0;
        break;
      case DB_NUM_SUP:  //kjh99.8.13
        db_num_sup_lcd = 0;
        break;
      case DB_REG_ZONE:
        db_reg_zone = 0;
        break;
      case DB_BASE_ID:
        db_base_id = 0;
        break;
	case DB_PWRON:
		db_pwron = DB_PWRKEY_ON;
		break;
#endif // #if defined (MMI_TREX) || (MMI_TYPE == MMI_A10)

#ifdef FEATURE_SCH_FER
      case DB_SCH_FER:  //kjh99.8.13
        db_sch_fer_lcd = 0;
        break;
#endif
#ifdef FEATURE_FCH_FER
      case DB_FCH_FER:  //kjh99.8.13
        db_fch_fer_lcd = 0;
        break;
#endif
      default:
        ERR_FATAL("Bad item parameter %d ",index,0,0);

    } /* end switch on item */

  } /* End loop for each item */

} /* end db_init() */
