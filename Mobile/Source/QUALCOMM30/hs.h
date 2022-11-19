#ifndef HS_H
#define HS_H
/*===========================================================================

              H A N D S E T   T A S K   H E A D E R   F I L E

DESCRIPTION
  This file contains the data structures used to interface to the Handset
  Task for the various DMSS phones.

REFERENCES
  ALPS, "A Cellular Bus Operation"
  NEC LCD Controller "uPD7228G" Spec Sheet
  Epson LCD Controller SED1230 Spec Sheet
  Hitachi LCD COntroller HS66712 Spec Sheet

Copyright (c) 1992, 1993, 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1995, 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999             by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/hs.h_v   1.5   05 Jan 2001 16:53:44   ioakimt  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/05/01   igt     Moved some includes from dsatcop.h in here.
04/11/00   jwh     Added hs_enable_hfk_pins() and hs_disable_hfk_pins() to 
                   support FEATURE_SDEVMAP_SHARE_HFK. 
03/31/00   va      Added HS_CHARGING_ON_K and OFF_K 
04/22/99   hrk     added keyword PACKED to struct hs_diag_type
01/21/99   hrk      removed FEATURE_UI_SNI around macro HS_AN_SNI as this 
                    was causing LED on LCD display to stay on.
11/02/98   dhh     Clear the extra comma in enum so that ARM compiler does 
                   not complain.
10/22/98   dhh     Added HFK message support for HFK interface.
10/08/98   vhp     Added external function prototype for passing a DTMF
                   keycode to the receiver.
08/25/98   dhh     Moved HS_EXT_DEV_K to the end of the enum list.
07/13/98   dhh     Added HS_EXT_DEV_K.
06/26/98   dhh     Removed the feature condition in handset key definitions.
                   Added Padlock/Slash Icons for 5GP-1.
03/27/98   dhh     Corrected HS_GLYPH_CNT definition for Odie.
03/27/98   skh     Added FEATURE_UI_SNI.
02/06/98   dhh     Include customer.h directly.
01/27/98   dhh     Added T_O for Odie. 
                   Added condition to include the code for Device ID support.
01/16/98   dhh     Added support for Device ID.
07/01/97   dhh     Changed the nest MAX Macros to avoid compile error 
                   when building for T_DM, T_SP or T_PC.
06/26/97   dhh     Fixed the error in the Q target change.
06/17/97   dhh     Added T_Q (Q Phones) support.
06/10/97   fkm     Another set of Code Review Changes
05/23/97   jjn     Configured for Module
05/20/97   fkm     Another set of Code Review Changes
04/29/97   fkm     First set of Code Review Changes
03/14/97   jjn     Defined new target called T_MD for the Module and 
                   added (TG==T_MD) to all the appropriate conditional
                   compile statements
03/03/97   fkm     Documentation Updates
01/06/97   fkm     TGP and Test Block additions
08/21/95   jmk     Removed T_SP cond comp, added change warning next to keycodes
08/01/95   jmk     Added T_SP conditional compilation (exclude rex, protos, etc)
05/24/95   jah     Added T_I2 (ISS-2) Support.
05/02/95   jah     Fixed Max RSSI for Beta to be 7 instead of 8.
03/30/95   jah     Added T_G (Gemini) Support.
10/06/94   jah     Changed height/width defines to have a base definition
                   per target, to facilitate the DM.
09/19/94   jah     Added HS_SET_GLYPH command
07/05/94   jah     Added HS_DIAG_SET_SCRN and HS_DIAG_SCRN_LOCK commands and
                   associated packet data types to support diag screen loads
06/28/94   jah     Added ISS1 Annunciators, updated comments
05/11/94   jah     Added HS_ON_HOOK_K and HS_OFF_HOOK_K.
09/22/93   jah     Added HS_EXT_PWR_ON_K and HS_EXT_PWR_OFF_K for Portable
                   external power sensing.
05/14/93   jah     Added more T_B2 (Beta II) conditional compilation
05/11/93   jah     Added T_B2 (Beta II) conditional compilation
03/18/93   jah     Added prototype for hs_set_char(), Portable-only.
03/15/93   jah     Added prototype for hs_poll_gpio.
01/26/93   jah     Added comment to header per code review.
01/15/93   jah     Moved the Portable "Special graphics characters" from 0..7
                   to 8..F (duplicate map).  Added HS_HALF_BOX.
12/07/92   jah     Added HS_BAD_BATT_K for reporting a bad battery in the
                   mobile.
10/23/92   jah     Added commands HS_ALL_ANNUN and HS_ALL_SCRN, and the assoc.
                   data structures.
10/20/92   jah     Added include of target.h.  Added definitons for the
                   Portable annunciators.
10/06/92   jah     Comment updates for portable/mobile differences.  Added
                   portable user-defined characters and screen size.
08/19/92   jah     Changed HS_PWR_K to HS_PWR_ON/OFF_K's for UI convenience.
08/06/92   jah     Converted entirely over to the new ALPS handset
08/02/92   jah     Added keys_free to hs_key_packet_type, for returning the
                   amount of free space in the input buffer.
07/08/92   jah     Initial revision

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "queue.h"      /* Definition of Queue services         */
#include "rex.h"        /* definition of REX data types         */
#include "customer.h"   /* Customer specific definitions        */
#include "target.h"     /* Target specific definitions          */


/*---------------------------------------------------------------------------
  TG         FEATURE_FFA  File to be used
  
  ON         ON           hsio.h
  ON         OFF          hsig.h
  OFF        X            hsio.h
---------------------------------------------------------------------------*/
#if (TG == T_G)
#if defined (FEATURE_FFA)
#error code not present
#else
#include "hsig.h"
#endif /*  FEATURE_FFA */
#endif  /*  (TG == T_G) */

#if (TG == T_O)
#error code not present
#endif  /*  (TG == T_O) */

#ifdef FEATURE_IDS
#error code not present
#endif

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Commands and Command Header
---------------------------------------------------------------------------*/

/* Handset Operations which can be performed/requested.  Note that not all
** targets support all operations.  For backwards compatability, we never
** delete items from this table, so some of these operations do not apply
** to newer phones and visa versa.
*/
typedef enum {
  HS_ACTIVE = 0,        /* unused                                            */
  HS_SLEEP,             /* unused                                            */
  HS_SCRN_CTL,          /* Turn on/off the LCD screen                        */
  HS_BACKLIGHT,         /* Turn on/off the screen & keypad backlights        */
  HS_SCRN_UPDATE,       /* Update the screen image                           */
  HS_ANNUN,             /* Turn on/off the annunciators                      */
  HS_BLINK_ANNUN,       /* Set which annunciators blink                      */
  HS_SCRN_RSSI,         /* Set the level of the RSSI bar                     */
  HS_AUD_IN,            /* Set the audio input path                          */
  HS_AUD_OUT,           /* Set the audio output path                         */
  HS_SKI,               /* Simulate keypad input                             */
  HS_UPDATE_BLINK,      /* Update the screen used for blinking               */
  HS_BLINK_RATE,        /* Set the blink rate                                */
  HS_DIAG,              /* Diag state dump                                   */
  HS_DIAG_LOCK,         /* Keypad lock                                       */
  HS_DTMF_DET,          /* Control polling of the DTMF detector              */
  HS_ALL_ANNUN,         /* Set both regular and blinking annunciators        */
  HS_ALL_SCRN,          /* Update both the blink and regular screen          */
  HS_DIAG_SET_SCRN,     /* Set the contents of the screen                    */
  HS_DIAG_SCRN_LOCK,    /* Lock screen updates, restrict diag commands       */
  HS_SET_GLYPH,         /* Set the bitmap of a glyph                         */
  HS_SCRN_BATTERY,      /* Set the battery level display                     */
  HS_SEVEN_SEG          /* Set Seven Segment Display(s)                      */
#ifdef FEATURE_IDS
#error code not present
#endif
} hs_op_type;

/* Command header common to all command packets
*/
typedef struct {
  q_link_type   link;        /* Queue link                            */
  q_type        *done_q_ptr; /* Queue to put 'this' on when done      */
  rex_tcb_type  *task_ptr;   /* Task to signal when done              */
  rex_sigs_type sigs;        /* Signals to signal *task_ptr when done */
  hs_op_type    cmd;         /* Handset Command                       */
} hs_hdr_type;

/*---------------------------------------------------------------------------
  Keypad declarations
---------------------------------------------------------------------------*/

/* These are the keycodes returned by calls to hs_key_get().  Do not depend
** on their numeric value, just use the symbolic values.  Some of these have
** been set to specific values to make decoding the hardware responses
** more convenient.  Note that not all keys apply to all targets and in fact
** some are very ancient in origin.  However, for backward compatiblity
** we never delete keys.
**
** NOTE: These values are used in the interface to external devices.  When
** defining new keys, be sure to choose a unique new value, to preserve the
** interface.
*/
typedef enum {          /* KEYS                                    */
  HS_NONE_K = 0,        /* No more keycodes available              */
  HS_ON_HOOK_K,         /* phone has gone on-hook                  */
  HS_OFF_HOOK_K,        /* phone has gone off-hook                 */
  HS_RING_VOL_0_K,      /* ringer volume 0 (Keep 0-2 in order)     */
  HS_RING_VOL_1_K,      /* ringer volume 1 ( and sequential!!)     */
  HS_RING_VOL_2_K,      /* ringer volume 2                         */
  HS_EAR_UP_K,          /* earpiece up                             */
  HS_EAR_UP_END_K,      /* earpiece up + end pressed               */
  HS_EAR_DOWN_K,        /* earpiece down                           */
  HS_PF1_K,             /* softkey #1 (left-most)                  */
  HS_PF2_K,             /* softkey #2 (right-most)                 */
  HS_MSG_K,             /* message waiting                         */
  HS_POUND_K = 0x23,    /* '#' key, ASCII '#'                      */
  HS_STAR_K = 0x2A,     /* '*' key, ASCII '*'                      */
  HS_0_K = 0x30,        /* '0' key, ASCII '0'                      */
  HS_1_K = 0x31,        /* '1' key, ASCII '1'                      */
  HS_2_K = 0x32,        /* '2' key, ASCII '2'                      */
  HS_3_K = 0x33,        /* '3' key, ASCII '3'                      */
  HS_4_K = 0x34,        /* '4' key, ASCII '4'                      */
  HS_5_K = 0x35,        /* '5' key, ASCII '5'                      */
  HS_6_K = 0x36,        /* '6' key, ASCII '6'                      */
  HS_7_K = 0x37,        /* '7' key, ASCII '7'                      */
  HS_8_K = 0x38,        /* '8' key, ASCII '8'                      */
  HS_9_K = 0x39,        /* '9' key, ASCII '9'                      */
  HS_SEND_K = 0x50,     /* Send key                                */
  HS_END_K = 0x51,      /* End key or Power key (Based on Target)  */ 
  HS_CLR_K = 0x52,      /* Clear key                               */
  HS_STO_K = 0x53,      /* Store key                               */
  HS_UP_K = 0x54,       /* Up-arrow key was pressed                */
  HS_DOWN_K = 0x55,     /* Down-arrow key was pressed              */
  HS_MUTE_K = 0x56,     /* Mute Key                                */
  HS_RCL_K = 0x57,      /* Recall key                              */
  HS_SD1_K = 0x58,      /* speed dial #1                           */
  HS_SD2_K = 0x59,      /* speed dial #2                           */
  HS_SD3_K = 0x5A,      /* speed dial #3                           */
  HS_MENU_K = 0x5B,     /* Menu key                                */
  HS_ALPHA_K = 0x5C,    /* Alpha key                               */
  HS_DEC_POUND_K,       /* Decoded # DTMF                          */
  HS_DEC_STAR_K,        /* Decoded * DTMF                          */
  HS_DEC_0_K,           /* Decoded 0 DTMF                          */
  HS_DEC_1_K,           /* Decoded 1 DTMF                          */
  HS_DEC_2_K,           /* Decoded 2 DTMF                          */
  HS_DEC_3_K,           /* Decoded 3 DTMF                          */
  HS_DEC_4_K,           /* Decoded 4 DTMF                          */
  HS_DEC_5_K,           /* Decoded 5 DTMF                          */
  HS_DEC_6_K,           /* Decoded 6 DTMF                          */
  HS_DEC_7_K,           /* Decoded 7 DTMF                          */
  HS_DEC_8_K,           /* Decoded 8 DTMF                          */
  HS_DEC_9_K,           /* Decoded 9 DTMF                          */
  HS_DEC_A_K,           /* Decoded A DTMF                          */
  HS_DEC_B_K,           /* Decoded B DTMF                          */
  HS_DEC_C_K,           /* Decoded C DTMF                          */
  HS_DEC_D_K,           /* Decoded D DTMF                          */
  HS_PWR_ON_K,          /* Power key is switched 'on'              */
  HS_PWR_OFF_K,         /* Power key is switched 'off'             */ 
  HS_PWR_K,             /* Power key                               */ 
  HS_INFO_K,            /* Info key                                */
  HS_FREE_K,            /* Phone was placed in hands-free cradle   */
  HS_PHONE_K,           /* Phone was lifted from hands-free cradle */
  HS_IGN_OFF_K,         /* Ignition was turned off                 */
  HS_IGN_ON_K,          /* Ignition was turned on                  */
  HS_TMR_ON_K,          /* Power-up timer 'on' pseudo-key          */
  HS_TMR_OFF_K,         /* Power-up timer 'off' pseudo-key         */
  HS_BAD_BATT_K,        /* The car battery is failing              */
  HS_EXT_PWR_ON_K,      /* External power was turned on            */
  HS_EXT_PWR_OFF_K,     /* External power was turned off           */
  HS_REDIAL_K,          /* Redial key                              */
  HS_RSSI_K,            /* RSSI key                                */
  HS_HFK_CORD_K,        /* Coil cord carkit                        */
  HS_HFK_NOCORD_K,      /* Non-coil cord carkit                    */
  HS_HEADSET_K,         /* Headset                                 */
  HS_DEV_UNKNOWN_K,     /* Unknown device ID                       */
  HS_EXT_DEV_K,         /* ONES detector finds external device     */
  HS_CHARGING_ON_K,     /* Key to mote battery charging is active  */
  HS_CHARGING_OFF_K,    /* Key to note battery charging stopped    */
#ifdef MODEL_SKTT 		//tasik 00.03.03
  HS_REC_K,             /* (S) VOICE MEMO RECORDING key            */
  HS_PLAY_K,            /* (S) VOICE MEMO PLAYING key              */
  HS_MANNERS_K,         /* (S) MANNER TALK key                     */ 
  HS_SENSING_ON_K,      /*                                         */
  HS_SENSING_OFF_K,     /*                                         */
  HS_MANNER_K,          /* (S) MANNER TALK key                     */ 
  HS_AR_K,              /* (S) Auto response key                   */
  HS_WAP_K,             /* (S) WAP key                             */
  HS_JACK_ON_K,         /* (S) Jack Sense On                       */ 
  HS_JACK_OFF_K,		/* (S) Jack Sense Off                      */
  HS_EAR_K,				/* (S) ear jack long key                   */
  HS_EAR_LONG_K,		/* (S) ear jack long key                   */
  HS_EAR_DOUBLE_K,      /* (S) ear jack key double click           */
  HS_POUND_LONG_K,      /* (S) Pound(#) long key                   */
  HS_STAR_LONG_K,       /* (S) Star(*) long key                    */
  HS_FLIPKEY_LONG_K,    /* (s) Flip function key                   */
  HS_PF3_K,			/* softkey #3 (left-most)                 */
#endif
  HS_RELEASE_K = 0xff   /* Key to note that all keys are up        */
} hs_key_type;

/*---------------------------------------------------------------------------
  Screen declarations
---------------------------------------------------------------------------*/

/* Screen dimensions for EACH TARGET.  This are exposed in this way for
** some of the support tools.  Phone software should NEVER use these
** definition directly.  Use HS_SCRN_{HIGH,WIDE} instead.
*/
/* T_M, Mobile */
#define HS_SCRN_HIGH_M  2       /* Screen height in characters */
#define HS_SCRN_WIDE_M  8       /* Screen width in characters  */

/* T_B2, Beta 2 */
#define HS_SCRN_HIGH_B2 2       /* Screen height in characters */
#define HS_SCRN_WIDE_B2 8       /* Screen width in characters  */

/* T_P, Portable */
#define HS_SCRN_HIGH_P  4       /* Screen height in characters */
#define HS_SCRN_WIDE_P  10      /* Screen width in characters  */

/* T_G Gemini */
#define HS_SCRN_HIGH_G  4       /* Screen height in characters */
#define HS_SCRN_WIDE_G  12      /* Screen width in characters  */

/* T_I2 ISS2 - Identical to Gemini */
#define HS_SCRN_HIGH_I2 4       /* Screen height in characters */
#define HS_SCRN_WIDE_I2 12      /* Screen width in characters  */

/* T_T, TGP */
#define HS_SCRN_HIGH_T  4       /* Screen height in characters */
#define HS_SCRN_WIDE_T  12      /* Screen width in characters  */

/* T_MD, Module */
#define HS_SCRN_HIGH_MD  4      /* Screen height in characters */
#define HS_SCRN_WIDE_MD  12     /* Screen width in characters  */

/* T_Q Q Phones */
#define HS_SCRN_HIGH_Q  4       /* Screen height in characters */
#define HS_SCRN_WIDE_Q  12      /* Screen width in characters  */

/* T_O Odie Phones */
#define HS_SCRN_HIGH_O  4       /* Screen height in characters */
#define HS_SCRN_WIDE_O  12      /* Screen width in characters  */

/* Define the maximum size for both Hight and Width accross all targets */
/* These MAX values need to be selected from the above definitions,     */
/* whichever is the largest value.                                      */

#define HS_SCRN_HIGH_MAX  4
#define HS_SCRN_WIDE_MAX  12


#if (TG==T_M)
#error code not present
#elif (TG==T_B2)
#error code not present
#elif (TG==T_P)
#error code not present
#elif (TG==T_G)
#define HS_SCRN_HIGH    HS_SCRN_HIGH_G
#define HS_SCRN_WIDE    HS_SCRN_WIDE_G
#ifdef ENHANCED_TEXT_MODE_DISPLAY // girin. 2000.1.28
#define HS_SCRN_HIGH_EX 5
#define HS_SCRN_WIDE_EX 20
#endif
#elif (TG==T_I2)
#error code not present
#elif (TG==T_T)
#error code not present
#elif ((TG==T_DM) || (TG==T_SP) || (TG==T_PC))
#error code not present
#elif (TG==T_MD)
#error code not present
#elif (TG==T_Q)
#error code not present
#elif (TG==T_O)
#error code not present
#else
#error code not present
#endif

#define HS_SCRN_ROW_0   0                       /* Index of 0th row */
#if HS_SCRN_HIGH > 1
#define HS_SCRN_ROW_1   (1 * HS_SCRN_WIDE)      /* Index of 1st row */
#endif
#if HS_SCRN_HIGH > 2
#define HS_SCRN_ROW_2   (2 * HS_SCRN_WIDE)      /* Index of 2nd row */
#endif
#if HS_SCRN_HIGH > 3
#define HS_SCRN_ROW_3   (3 * HS_SCRN_WIDE)      /* Index of 3rd row */
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Screen character image type
*/
#ifdef ENHANCED_TEXT_MODE_DISPLAY // girin. 2000.1.28
typedef byte hs_scrn_type[ HS_SCRN_HIGH_EX * HS_SCRN_WIDE_EX ];
#else
typedef byte hs_scrn_type[ HS_SCRN_HIGH * HS_SCRN_WIDE ];
#endif

/* Special graphics characters (or best approximation)
*/
#if ((TG==T_M) || (TG==T_B2))
#error code not present
#elif (TG==T_P)
#error code not present
#elif ((TG==T_MD) || (TG==T_G) || (TG==T_Q))
#define HS_EMPTY_BOX    ((byte) 0xDC)   /* Empty box            */
#define HS_FILL_BOX     ((byte) 0x8F)   /* Filled box           */
#elif (TG==T_T)
#error code not present
#elif (TG==T_O)
#error code not present
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* The LCD controllers contain fixed representations of the various characters
** (A-Z, 0-9, etc) as well as any special characters we had the manufacturer
** define in silicon.  However, each controller also allows us to define
** several characters on the fly as we run.  We call these "Glyph" characters
** and they are used for "late breaking" characters that where not anticipated
** at the time the LCD control was specified.  So far, these characters are all
** 5 columns packed 1 row per byte, 7 rows.  For a given row, 0x00 turns all
** points off, 0x1F turns all (five) points on.
*/
#define HS_GLYPH_WIDTH   5
#define HS_GLYPH_HEIGHT  7
typedef byte hs_glyph_type[ HS_GLYPH_HEIGHT ];

/* Number of glyphs supported
*/
#if ( (TG==T_P) || (TG==T_T) || (TG==T_O) )
#error code not present
#elif ( (TG==T_G) || (TG==T_Q) )
#define HS_GLYPH_CNT     4
#else
#error code not present
#endif

/* Character definition command packet for HS_SET_GLYPH
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header                         */
  byte index;           /* character index of glyph being defined */
  hs_glyph_type bitmap; /* bitmap of glyph being defined          */
} hs_glyph_packet_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Screen/Backlight on/off command packet for HS_SCRN_CTL and HS_BACKLIGHT
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header */
  boolean on_off;       /* on/off control */
} hs_ctl_packet_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Screen update command packet for HS_SCRN_UPDATE and HS_UPDATE_BLINK
*/
typedef struct {
  hs_hdr_type hdr;        /* Command header */
  hs_scrn_type *scrn_ptr; /* Screen image   */
} hs_scrn_packet_type;

/* Screen update command packet for HS_ALL_SCRN
*/
typedef struct {
  hs_hdr_type hdr;              /* Command header     */
  hs_scrn_type *scrn_ptr;       /* Screen image       */
  hs_scrn_type *blink_scrn_ptr; /* Blink screen image */
} hs_scrna_packet_type;

/* Blink command packet for HS_BLINK_RATE
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header                           */
  word blink_rate;      /* rate for blinking milliseconds per image */
} hs_blink_packet_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* RSSI level command packet for HS_SCRN_RSSI
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header   */
  byte rssi;            /* RSSI level       */
  byte blink_rssi;      /* Blink RSSI level */
} hs_rssi_packet_type;

/* Define how RSSI level is displayed for targets which have
** an actual RSSI display
*/
#if ((TG==T_G) || (TG==T_Q))
#define HS_MAX_RSSI     5     /* RSSI range is 0..5 */
#elif (TG==T_I2)
#error code not present
#else
#error code not present
#endif

/* Battery level command packet for HS_SCRN_BATTERY
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header      */
  byte batt;            /* Battery level       */
  byte blink_batt;      /* Blink battery level */
} hs_batt_packet_type;

/* Define how Battery level is displayed for targets which have
** an actual Battery display
*/
#if ((TG==T_G) || (TG==T_Q))
#define HS_MAX_BATTERY  5     /* Battery range is 0..5 */
#endif

/*---------------------------------------------------------------------------
  Annunciator declarations (Both on-screen and LED)
---------------------------------------------------------------------------*/

/* Annunciator Masks/Values (comment on each is a "rough" meaning of each)
*/
#define HS_AN_ALL       0xffff                  /* Mask of all annunciators */
#define HS_ANNUNCIATORS 12

#if ((TG==T_M) || (TG==T_B2))
#error code not present
#elif ((TG==T_P)||(TG==T_S))
#error code not present
#elif (TG==T_I1)
#error code not present
#elif (TG==T_T)
#error code not present
#elif ((TG==T_MD) || (TG==T_G) || (TG==T_Q))

#define HS_AN_CDMA      0x0001  /* CDMA                */
#define HS_AN_INUSE     0x0002  /* In Use              */
#define HS_AN_NOSVC     0x0004  /* No Svc (no service) */
#define HS_AN_ROAM      0x0008  /* Roam                */
#define HS_AN_MSG       0x0010  /* Message             */
#define HS_AN_VMAIL     0x0020  /* Voice mail          */
#define HS_AN_SNI       0x0040  /* Silent Notification */
#if (MODEL_ID==MODEL_B1) || (MODEL_ID==MODEL_B2) 
#define HS_AN_RGB       0x0E00
#define HS_AN_RED       0x0200
#define HS_AN_GREEN     0x0400
#define HS_AN_BLUE      0x0800

#define HS_AN_REC       0x0100  /* Silent Notification */
#define HS_AN_ENVOLOPE	0x1000  /* SMS envolope */
#define HS_AN_MELODY    0x2000  /* Silent Notification */
#define	HS_AN_ALL_ON	0x8000  /* All pictographs are ON  */
#define	HS_AN_ALL_OFF	0x4000  /* All pictographs are OFF */
#endif /* (MODEL_ID==MODEL_B1) */
/* <-- */

#elif (TG==T_O)
#error code not present
#elif (TG==T_I2)
#error code not present
#endif

/* Annunciator control type
*/
typedef struct {
  word mask;            /* Mask of annunciators to modify   */
  word value;           /* Values of annunciators to modify */
} hs_ann_type;

/* Annunciator command packet for HS_ANNUN and HS_BLINK_ANNUN
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header     */
  hs_ann_type annun;    /* Annunciator change */
} hs_ann_packet_type;

/* Annunciator command packet for HS_ALL_ANNUN
*/
typedef struct {
  hs_hdr_type hdr;          /* Command header     */
  hs_ann_type annun;        /* Annunciator change */
  hs_ann_type blink_annun;  /* Annunciator change */
} hs_anna_packet_type;

/*---------------------------------------------------------------------------
  Audio control (for the Sound Services)
---------------------------------------------------------------------------*/

/* Handset audio paths
** Not all of the audio paths, just the ones via the handset/cradle.
*/
typedef enum {
  HS_AUD_HS = 0, /* Audio path is the handset         */
  HS_AUD_HF,     /* Audio path is the hands-free path */
  HS_AUD_MUTE    /* Audio path is muted               */
} hs_aud_type;

/* Audio path command packet for HS_AUD_IN and HS_AUD_OUT
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header */
  hs_aud_type apath;    /* Audio path     */
} hs_aud_packet_type;

/* DTMF detector polling on/off command packet for HS_DTMF_DET
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header */
  boolean on_off;       /* on/off control */
} hs_dtmf_packet_type;

/*---------------------------------------------------------------------------
  Diagnostic input/output/control
---------------------------------------------------------------------------*/

/* Simulate-keypad-input command packet for HS_SKI (for DM HS simulator)
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header                  */
  byte keycode;         /* Keycode input                   */
  word keys_free;       /* Number of free key buffer slots */
} hs_key_packet_type;

/* Description of the buffer of information describing the
** handset's screen/annunciator state.
*/
typedef PACKED struct {
  boolean backlight_on;         /* Backlight state         */
  hs_scrn_type scrn;            /* Screen                  */
  word annun;                   /* Annunciators            */
  hs_scrn_type blink_scrn;      /* Screen for blinking     */
  word blink_annun;             /* Annunciators for blink  */
  byte rssi;                    /* RSSI indicator          */
  word blink_rate;              /* Blink rate for blinking */
} hs_diag_type;

/* Handset information dump command packet for HS_DIAG (diag snapshot),
** and screen information command packet for HS_DIAG_SET_SCRN.
*/
typedef struct {
  hs_hdr_type hdr;              /* Command header */
  hs_diag_type *diag_ptr;       /* Handset state snapshot */
} hs_diag_packet_type;

/* Handset keypad lock command packet for HS_DIAG_LOCK and HS_DIAG_SCRN_LOCK
*/
typedef struct {
  hs_hdr_type hdr;      /* Command header */
  boolean lock;         /* Lock/un-lock   */
} hs_lock_packet_type;

/* Seven Segment Display Command Packet for HS_SEVEN_SEG.  Each of the
** seven segments is controlled by one bit in a 8 bit byte (sign bit unused).
** Build characters by ORing together these individual Segment values.
** (e.g. "7" = (HS_SEGMENT_HTOP_V|HS_SEGMENT_VTOP_R_V|HS_SEGMENT_VBOT_R_V)
**
** 0x40 ->  ---
** 0x20 ->  | |  <- 0x10
** 0x08 ->  ---
** 0x04 ->  | |  <- 0x02
** 0x01 ->  ---
*/

/* This typedef defines a structure that can hold values for all Seven
** Segment Displays on the current target.
*/
#define HS_NUM_7SEGS            3       /* # of Seven Segment Displays  */
typedef byte hs_7seg_type[HS_NUM_7SEGS];

/* These values can be ORed together to make symbols from on the Seven
** Segment displays.  Just OR in the segments you want "on".  (E.G. the
** number "7" would be:
**
** (HS_SEGMENT_HTOP|HS_SEGMENT_VTOP_R_V|HS_SEGMENT_VBOT_R_V)
*/
#define HS_SEGMENT_HTOP_V       0x40    /* Horizontal Top Segment       */
#define HS_SEGMENT_HMID_V       0x08    /* Horizontal Middle Segment    */
#define HS_SEGMENT_HBOT_V       0x01    /* Horizontal Bottom Segment    */

#define HS_SEGMENT_VTOP_L_V     0x20    /* Veritical Top Left Segment   */
#define HS_SEGMENT_VTOP_R_V     0x10    /* Veritical Top Right Segment  */

#define HS_SEGMENT_VBOT_L_V     0x04    /* Veritical Bottom Left Seg    */
#define HS_SEGMENT_VBOT_R_V     0x02    /* Veritical Bottom Right Seg   */

typedef struct {
  hs_hdr_type hdr;                      /* Command header               */
  hs_7seg_type seven_seg;               /* Non-Blink 7 Seg Values       */
  hs_7seg_type blink_7seg;              /* Blink 7 Seg Values           */
} hs_7seg_packet_type;

#ifdef FEATURE_IDS
#error code not present
#endif

/*---------------------------------------------------------------------------
  Command packet definition
---------------------------------------------------------------------------*/

/* Handset command packet which is passed to hs_cmd()
*/
typedef union {
  hs_hdr_type          hdr;     /* Just a command header                   */
  hs_ctl_packet_type   ctl;     /* LCD control, HS_SCRN_CTL, HS_BACKLIGHT  */
  hs_scrn_packet_type  scrn;    /* Screen, HS_SCRN_UPDATE, HS_UPDATE_BLINK */
  hs_scrna_packet_type scrna;   /* All screens, HS_ALL_SCRN                */
  hs_rssi_packet_type  rssi;    /* Set RSSI, HS_SCRN_RSSI                  */
  hs_ann_packet_type   ann;     /* Annunciator change, HS_ANNUN            */
  hs_anna_packet_type  anna;    /* Annunciator change all, HS_ALL_ANNUN    */
  hs_aud_packet_type   aud;     /* Set audio path, HS_AUD_IN, HS_AUD_OUT   */
  hs_key_packet_type   key;     /* Simulate-keypad-input, HS_SKI           */
  hs_blink_packet_type blink;   /* Screen blink, HS_BLINK_RATE             */
  hs_diag_packet_type  diag;    /* Diag state dump, HS_DIAG                */
  hs_lock_packet_type  lock;    /* Keypad lock, HS_DIAG_LOCK               */
  hs_dtmf_packet_type  dtmf;    /* DTMF detector control, HS_DTMF_DET      */
  hs_diag_packet_type  dscrn;   /* Diag screen load, HS_DIAG_SET_SCRN      */
  hs_lock_packet_type  scrlock; /* Screen lock, HS_DIAG_SCRN_LOCK          */
  hs_glyph_packet_type glyph;   /* glyph packet for HS_SET_GLYPH           */
  hs_batt_packet_type  batt;    /* Set Battery level, HS_SCRN_BATTERY      */
  hs_7seg_packet_type  sevseg;  /* Seven Segment, HS_SEVEN_SEG             */
#ifdef FEATURE_IDS
#error code not present
#endif
} hs_packets_type;

/* Queue of free command buffers.  These can be retrieved by calling
** q_get( &hs_cmd_free_q ).  Note:  *done_q_ptr is set to point to this
** queue so that on completion the commands will be placed back on this
** queue.  Do not alter *done_q_ptr.
*/
extern q_type hs_cmd_free_q;


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION HS_KEY_INIT

DESCRIPTION
  This procedure receives and remembers what task and signal to use when
  keycodes become available.

DEPENDENCIES
  hs_key_buffer

RETURN VALUE
  None

SIDE EFFECTS
  hs_key_rcvr_sig
  hs_key_rcvr_tcb_ptr

===========================================================================*/

extern void hs_key_init
(
  rex_tcb_type  *task_ptr,     /* Task to signal when keycodes arrive      */
  rex_sigs_type key_signal     /* Signal to send task when keycodes arrive */
);

/*===========================================================================

FUNCTION HS_KEY_GET

DESCRIPTION
  Read a keycode from the input buffer.

  NOTE: This routine is not called by the Handset task, but rather is called
  by "another task" to retrieve keys that Handset has collected on its
  behalf.  Note that this routine is NOT multi-thread safe, and is expected
  to be called by only ONE other task.

DEPENDENCIES
  None

RETURN VALUE
  This procedure returns the oldest unread keycode.  If no keycode
  is currently available, HS_NONE_K is returned.

SIDE EFFECTS
  None

===========================================================================*/
extern byte hs_key_get( void );

/*===========================================================================

FUNCTION HS_KEY_PUT

DESCRIPTION
  Write a keycode into the input buffer.  This is a regional routine.  Make
  sure the keys are added to the buffer in compliance with the hs_key_state
  variables.

  NOTE: depending on the configuration (Dial Shuttle and/or IDS) this routine
  can be called from contexts OTHER THAN Handset.

DEPENDENCIES
  hs_key_state

RETURN VALUE
  None

SIDE EFFECTS
  If there is no room for the key in hs_key_buffer, then an ERR() is logged
  and the key is thrown away.

===========================================================================*/
void hs_key_put
(
  hs_key_type new_key
    /* the Key to be put into the Key Buffer */
);

/*===========================================================================

FUNCTION HS_CMD 

DESCRIPTION
  Queue a command for execution by the Handset Task.

DEPENDENCIES
  The queue will not be processed until the Handset Task has started.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_cmd
(
  hs_packets_type *cmd_ptr
    /* pointer to a handset command.  When the command has finished, the
    ** command structure is put on the "done queue".  If there is no "done
    ** queue", or this operation is not desirable, set the "done queue"
    ** pointer to NULL.  Also when the command has completed, the specified
    ** task is signaled with the sig parameter.  If no signaling is required,
    ** the task pointer parameter can be set to NULL.
    */
);

/*===========================================================================

FUNCTION HS_TASK 

DESCRIPTION
  The Main Control Task calls this procedure via rex_def_task() to start up
  this function as the Handset Task.  This procedure calls hs_init() which
  initializes the handset and handles startup handshaking with the Main Control
  Task.

DEPENDENCIES
  This procedure must be called by rex_def_task.  The startup handshaking
  must be completed in order for the handset task to begin normal operations.

RETURN VALUE
  This procedure does not return.

SIDE EFFECTS
  The handset display will change as a result of handset task processing.

===========================================================================*/
void hs_task
(
  dword ignored
    /* parameter from REX -- ignored */
    /*lint -esym(715,ignored)
    ** Have lint not complain about the ignored parameter 'ignored' which is
    ** specified to make this routine match the template for rex_def_task().
    */
);

/*===========================================================================

FUNCTION HS_POLL_GPIO

DESCRIPTION
  This procedure polls the GPIOs which are keys, handling key event
  signals for new keycodes as needed.
  
  NOTE: this routine is NOT just called the Handset Task!  It is also
  called by other tasks in some cases!

DEPENDENCIES
  hs_ext_pwr
  hs_locked
  hs_on_hook

RETURN VALUE
  True if anything changed or if keys are waiting.
  False if no changes/keys occurred.

  Note: while not used by the Handset task, this return values IS USED when
    this routine is called by othoer tasks!

SIDE EFFECTS
  hs_ext_pwr
  hs_on_hook

===========================================================================*/
extern boolean hs_poll_gpio( void );


#if defined(FEATURE_SDEVMAP_SHARE_HFK) && defined(FEATURE_SECOND_UART)
/*===========================================================================

FUNCTION HS_DISABLE_HFK_PINS

DESCRIPTION
  This procedure dynamically disables the operation of the HS_ON_HOOK macro
  when FEATURE_SECOND_UART is defined.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The value of hfk_pins_enabled is set to FALSE

===========================================================================*/
extern void hs_disable_hfk_pins( void );


/*===========================================================================

FUNCTION HS_ENABLE_HFK_PINS

DESCRIPTION
  This procedure dynamically enables the operation of the HS_ON_HOOK macro
  when FEATURE_SECOND_UART is defined.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The value of hfk_pins_enabled is set to TRUE

===========================================================================*/
extern void hs_enable_hfk_pins( void );


#endif  /* FEATURE_SDEVMAP_SHARE_HFK && FEATURE_SECOND_UART */

#if (TG==T_P)
#error code not present
#endif /* TG == T_P */

#endif /* HS_H */

