#ifndef DB_H
#define DB_H
/*===========================================================================

                                           Database Interface

DESCRIPTION
   This header file contains the definitions necessary to use
   the Database from other modules.

Copyright (c) 1992,1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1995,1996,1998 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/db.h_v   1.1   31 Aug 2000 14:15:42   chonda  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/31/00   cah     (dsb) Added DB_RF_GPS to db_rf_mode_type.
12/10/98   dna     Fix CR7143.  Add new db item for powerup scanning
02/26/99   ejv     Merged changes for FEATURE_QUICK_SCAN.  Two db_acq_mode
                   types are no longer necessary for QUICK_SCAN.
06/25/98   na      Made the following changes to db_voice_as_data_type: Added
                   DB_VOICE_AS_FAX_ONCE, DB_VOICE_AS_FAX_ALWAYS,
                   DB_VOICE_AS_MODEM_ONCE, DB_VOICE_AS_MODEM_ALWAYS. Deleted
                   DB_VOICE_AS_DATA_ONCE, DB_VOICE_AS_DATA_ALWAYS.
04/24/98   dbd     Added DB_VOICE_AS_DATA for service negotiation
02/02/98   dgy/smo Added new interfaces for CDMA pilot acquisition.
12/20/96   rdh     Changed functions to Pascal calling convention to save ROM.
12/21/95   jca     Added DB_BS_P_REV for 13K/8K service negotiation.
10/26/95   jca     Deleted DB_ORIG_MIN_INDEX.
10/04/95   rdh     Separated SLEEP_ACTIVE from SLEEP_MODE.
09/20/95   rdh     Added SLEEP_ACTIVE to tell whether or not we are sleeping.
08/10/95   cah     Added Voice Privacy active flag.
07/06/95   rah     Added 13K_VOC_AVAILABLE for 13kbps vocoder support.
06/29/95   rdh     Enums fixed for acq and sleep mode.
06/28/95   rdh     Added ACQ_MODE for setting acquisition mode.
06/21/95   rdh     Added SLEEP_MODE for disabling sleep.
12/09/94   ras     Added OFF_HOOK for hook switch state.
04/13/94   jjw     Added DB_DATA_SRVC_STATE boolean item
01/24/94   jca     Added DB_RF_900_MHZ to db_rf_mode_type
04/27/93   jai     Deleted DB_SID_NID_LIST item.
04/13/93   jjw     Added DB_SIO_INIT item
03/26/93   jca     Added DB_RF_SLEEP to db_rf_mode_type
09/16/92   ARH     Added DB_VOX
09/09/92   ARH     Added DB_HOLD_ORIG
09/04/92   ARH     Added DB_ORIG_MIN_INDEX
09/02/92   ARH     Added DB_CODE_CHANNEL and DB_PILOT_PN_OFF
07/26/92   ARH     Added DB_RF_MODE
06/20/92   ARH     Added SID_NID_LIST
03/10/92   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"


/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
** Enumeration of items in the database
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

typedef enum {
  DB_IN_USE,               /* Call in progress indication                  */
  DB_NO_SVC,               /* Cannot find cellular service indication      */
  DB_DIGITAL,              /* Indicates Digital service                    */
  DB_CONV,                 /* Indicates Conversation is taking place       */
  DB_ROAM,                 /* Roaming status indication                    */
  DB_CHANNEL,              /* Current channel mobile is tuned to           */
  DB_CODE_CHANNEL,         /* Current code-channel mobile is tuned to      */
  DB_PILOT_PN_OFF,         /* Current pilot PN offset                      */
  DB_SID,                  /* Current System ID                            */
  DB_LOCAID,               /* Current Location ID                          */
  DB_NID,                  /* Current Network ID                           */
  DB_LP_SEC,               /* Leap Seconds since start of System Time      */
  DB_LTM_OFF,              /* Offset local time from System Time           */
  DB_DAYLT,                /* Daylight savings time indicator              */
  DB_DMSS_STATE,           /* DMSS System State                            */
  DB_POWER,                /* Current mobile output power level            */
  DB_PRIVACY_MODE,         /* Current privacy mode                         */
  DB_RSSI,                 /* Current FM RSSI level                        */
  DB_CDMA_RSSI,            /* Current CDMA RSSI level                      */
  DB_BATTERY_LEVEL,        /* Current Battery level                        */
  DB_TEMPERATURE,          /* Current Temperature                          */
  DB_RF_MODE,              /* Current RF mode (analog, CDMA, PCN)          */
  DB_HOLD_ORIG,            /* Hold incoming origination cmds               */
  DB_POWERUP_SCANNING,     /* Hold origination on powerup                  */
  DB_ON_HOOK,              /* Is the subscriber unit on hook?              */
  DB_VOX,                  /* Current VOX mode                             */
  DB_DATA_SRVC_STATE,      /* Current state of data services task          */
  DB_SLEEP_MODE,           /* Is sleep enabled?                            */
  DB_SLEEP_ACTIVE,         /* Is sleep occurring?                          */
  DB_ACQ_MODE,             /* Type of CDMA acquisition desired             */
  DB_13K_VOC_AVAILABLE,    /* Vocoder 13k availability flag                */
  DB_VOICE_PRIVACY,        /* Voice Privacy                                */
  DB_BS_P_REV,             /* Base Station's protocol revision level       */
  DB_VOICE_AS_DATA,        /* Answer next call as data                     */

#if defined (MMI_TREX) || (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)
/* bhkang 00-06-05 --> @why, what */
  DB_ECIO,
  DB_RX_STATE,
/* for debug screen <-- */

  DB_NUM_SUP,              //kjh99.8.13
//for WAP kjh99.11.24
  DB_REG_ZONE,             /* Registration zone */
  DB_BASE_ID,              /* Base station identification */
//end kjh99.11.24
  DB_PWRON,
#endif

#ifdef FEATURE_SCH_FER
  DB_SCH_FER,
#endif
#ifdef FEATURE_FCH_FER
  DB_FCH_FER,
#endif
  DB_LAST_ITEM             /* Internal Use Only                            */
} db_items_type;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
** The following are the types associated with items in the database
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*
** Type for DB_ROAM item
*/
typedef enum {
  DB_ROAM_OFF,
  DB_ROAM_ON,
  DB_ROAM_BLINK
} db_roam_type;


/*
** Type for DB_RF_MODE item
*/

typedef enum {
  DB_RF_ANALOG,                   /* RF is in analog (FM) mode of operation */
  DB_RF_CDMA,                     /* RF is in CDMA mode of operation        */
  DB_RF_PCN,                      /* RF is in PCN mode of operation         */
  DB_RF_900MHZ,                   /* RF is in 900 MHz mode of operation     */
  DB_RF_SLEEP,                    /* RF is in sleep mode of operation       */
  DB_RF_GPS                       /* RF is in GPS mode of operation         */
} db_rf_mode_type;



/*
** Type for DB_DMSS_STATE: System state enumeration.
*/

typedef enum {
  DB_DMSS_POWERUP_S,
  DB_DMSS_ONLINE_ANALOG_S,
  DB_DMSS_ONLINE_DIGITAL_S,
  DB_DMSS_OFFLINE_ANALOG_S,
  DB_DMSS_OFFLINE_DIGITAL_S,
  DB_DMSS_RESET_S,
  DB_DMSS_POWERDOWN_S
} db_dmss_state_type;


/*
** Type for DB_SLEEP_MODE items
*/

typedef enum {
  DB_SLEEP_ENABLED,               /* All sleep enabled                      */
  DB_SLEEP_ZZ_OFF,                /* Idle sleep disabled                    */
  DB_SLEEP_DEEP_OFF,              /* Deep sleep disabled                    */
  DB_SLEEP_DISABLED               /* All sleep disabled                     */
} db_sleep_mode_type;


/*
** Type for DB_SLEEP_ACTIVE items
*/

typedef enum {
  DB_SLEEP_ON,                    /* We are asleep                          */
  DB_SLEEP_OFF                    /* We are awake                           */
} db_sleep_active_type;


/*
** Type for DB_ACQ_MODE item
*/

typedef enum {
  DB_ACQ_FULL,                    /* Full acquisition desired               */
  DB_ACQ_MINI,                    /* Mini acquisition desired               */
  DB_ACQ_MICRO                    /* Micro acquisition desired              */
#ifdef FEATURE_QUICK_SCAN
  /* Quickscan makes Poweron-Mini and Fast-Full unnecessary */
#else
  ,
  DB_ACQ_POWERON_MINI,            /* Power-on Mini acquisition desired      */
  DB_ACQ_FAST_FULL                /* Fast-full acquisition desired          */
#endif
} db_acq_mode_type;

/*
** Type for DB_VOICE_AS_DATA item
*/

typedef enum {
  DB_VOICE_AS_DATA_NEVER,         /* Never answer voice call as a data call */
  DB_VOICE_AS_FAX_ONCE,           /* Answer voice as fax once               */
  DB_VOICE_AS_FAX_ALWAYS,         /* Always answer voice as fax             */
  DB_VOICE_AS_MODEM_ONCE,         /* Answer voice as modem once             */
  DB_VOICE_AS_MODEM_ALWAYS        /* Always answer voice as modem           */
} db_voice_as_data_type;

typedef enum {
	DB_CHARGER_ON,
	DB_PWRKEY_ON
} db_pwron_type;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
** The following is the union of all types
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

typedef union {
  boolean               in_use;
  boolean               no_svc;
  boolean               digital;
  boolean               conv;
  db_roam_type          roam;
  word                  channel;
  byte                  code_channel;
  word                  pilot_pn_off;
  word                  sid;
  word                  locaid;
  word                  nid;
  byte                  lp_sec;
  byte                  ltm_off;
  boolean               daylt;
  db_dmss_state_type    dmss_state;
  byte                  power;
  boolean               privacy_mode;
  word                  rssi;
  word                  cdma_rssi;
  word                  battery_level;
  word                  temperature;
  db_rf_mode_type       rf_mode;
  boolean               hold_orig;
  boolean               powerup_scanning;
  boolean               on_hook;
  boolean               vox;
  boolean               data_srvc_enabled;
  db_sleep_mode_type    sleep_mode;
  db_sleep_active_type  sleep_active;
  db_acq_mode_type      acq_mode;
  boolean               voc13k_available;
  boolean               voice_privacy;
  byte                  bs_p_rev;
  db_voice_as_data_type voice_as_data;

/* bhkang 00-06-05 --> @why, what */
// added by chul for D1
#if defined (MMI_TREX) || (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)
  word					ecio;
  word					rx_state;
  byte					num_sup_lcd;
  word                  reg_zone;
  word                  base_id;
  boolean				batt_fet;
  word					chg_level;
  word					rawbat_level;
  byte					verror;
  byte					berror;
  word					coulomb;
  byte					bcharge_state;
  db_pwron_type         pwron;
#endif
#ifdef FEATURE_SCH_FER
  word					sch_fer_lcd;
#endif
#ifdef FEATURE_FCH_FER
  word					fch_fer_lcd;
#endif
/* for debug screen <-- */

} db_items_value_type;



/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/* <EJECT> */
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
extern void db_put
(
  db_items_type        item,        /* Item to be changed                  */
  db_items_value_type  *value_ptr   /* Value to change the item to         */

);



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
extern void db_get
(
  db_items_type         item,     /* Item to get value from                */
  db_items_value_type  *value_ptr /* Buffer to retrieve item into          */
);



/* <EJECT> */
/*===========================================================================

FUNCTION db_init

DESCRIPTION
  Initializes the Database to default values. No notifications are made
  when setting the values to default.


DEPENDENCIES
  None

RETURN VALUE
  None. All values of the database are initialized to a known default
  (Check DB.C for default values)

SIDE EFFECTS
  None

===========================================================================*/
extern void db_init
(
  void
);



#endif
