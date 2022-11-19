#ifndef _EXTERN_H_
#define _EXTERN_H_

// KVM EXTERNAL INCLUDES

#define ONSCREEN_WINDOW   0
#define OFFSCREEN_WINDOW  1

#define color_PLAIN  1
#define color_GRAY   2
#define color_ERASE  3
#define color_INVERT 4


/* ACTION CODE */
#define	ACT_UP		1
#define	ACT_LEFT	2
#define	ACT_RIGHT	5
#define	ACT_DOWN	6
#define	ACT_FIRE	8
#define	ACT_GAME_A	9
#define	ACT_GAME_B	10
#define	ACT_GAME_C	11
#define	ACT_GAME_D	12

/* KEY */
#define	KEY_NUM0	48
#define	KEY_NUM1	49
#define	KEY_NUM2	50
#define	KEY_NUM3	51
#define	KEY_NUM4	52
#define	KEY_NUM5	53
#define	KEY_NUM6	54
#define	KEY_NUM7	55
#define	KEY_NUM8	56
#define	KEY_NUM9	57
#define	KEY_STAR	42
#define	KEY_POUND	35

/* KEY: SK-VM SPEC */
#define KEY_CLR		8
#define KEY_COML	129
#define KEY_COMC	130		// RESERVED
#define KEY_COMR	131
#define	KEY_UP		141
#define	KEY_LEFT	142
#define	KEY_RIGHT	145
#define	KEY_DOWN	146
#define	KEY_FIRE	148
#define KEY_CALL	190
#define KEY_END		191
#define	KEY_FLIP_OPEN	192
#define	KEY_FLIP_CLOSE	193
#define	KEY_VOL_UP	194
#define	KEY_VOL_DOWN	195

extern void getLCDInfo( int *width, int *height, int *isColor, int *depth );

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
#endif
//#if defined(FEATURE_EQUS_A10B1) || defined(MMI_A10)  //  girin. 2000.7.26     		   */
  HS_SENSING_ON_K,      /*                                         */
  HS_SENSING_OFF_K,     /*                                         */
  HS_MANNER_K,          /* (S) MANNER TALK key                     */ 
  HS_AR_K,              /* (S) Auto response key                   */
//#if MODEL_ID == MODEL_A10 // tasik 00.11.07
//  HS_REC_K,             /* (S) VOICE MEMO RECORDING key            */
//  HS_PLAY_K,            /* (S) VOICE MEMO PLAYING key              */
//#endif 
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
  HS_RELEASE_K = 0xff   /* Key to note that all keys are up        */
} hs_key_type;

#endif
