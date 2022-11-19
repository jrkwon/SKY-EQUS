/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_hs.c
**
** Description: 
**     SKY HANDSET TASK. 
**     This task provides an interface between the handset and the other 
**     software tasks.  It processes input received from the handset keypad, 
**     and displays information on the handset display.
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-10-30 jrkwon     Created.
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"   // jwpark
#include "target.h"     // by chos
#include "comdef.h"     /* Definitions for byte, word, etc.        */
#include "bio.h"        /* Bit I/O macros                          */
#include "voc_core.h"   /* Vocoder Core prototypes and definitions */
#ifdef T_SLEEP
#include "sleep.h"      /* Sleep task signaling                    */
#endif
#include "processor.h"  /* Processor architecture definitions      */
#include "rex.h"        /* REX services                            */
#include "mc.h"         /* Main Control typedefs and prototypes    */
#include "dog.h"        /* Watchdog signal and time definitions    */
#include "err.h"        /* Error macros                            */
#include "msg.h"        /* Message macros                          */
#include <memory.h>     /* Memory manipulation routines            */
#include "queue.h"      /* Queue declaration package               */
#include "task.h"       /* Global task declarations                */
#include "srch.h"       /* prototype for srch_get_dmod_id()        */
#include "voc_core.h"   /* Vocoder Core prototypes and definitions */
#include "db.h"         /* Phone Data Base Interface               */
#include "rf.h"         /* RF Interface for rf_codec_on()          */
#include "snd.h"        /* Sound Interface                         */

#include "sky.h"
#include "ska_call.h"   /* Because of Group_SNI */
#include "ska_clid.h"
#include "adc.h"
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/*#define HS_CALL_LED_ON() \
    BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_HI_V)

#define HS_CALL_LED_OFF() \
    BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V)
*/

#define HS_RGBLED(bred,bgreen,bblue) FALSE
/*GPIO의 할당을 모두 하기 전에 아래의 매크로는 잠시 막아 놓는다.*/
/*
    BIO_OUT(ENC_GPIO_1, BIO_LED_RED_M | BIO_LED_BLUE_M, \
    ((bred) ? BIO_LED_RED_HI_V:BIO_LED_RED_LO_V) | ((bblue) ? BIO_LED_BLUE_HI_V : BIO_LED_BLUE_LO_V)); \
    BIO_OUT(ENC_GPIO_1, BIO_LED_GREEN_M, ((bgreen) ? BIO_LED_GREEN_HI_V : BIO_LED_GREEN_LO_V))
*/

/* Handset Task Signals */
#define HS_SNI_BLINK_SIG        0x0200  /* SNI blinking signal */
#define HS_NV_DONE_SIG          0x0100  /* NV command "done" signal       */
#define HS_GPIO_TIMER_SIG       0x0080  /* GPIO 'key' poll timer signal   */
#define HS_CMD_Q_SIG            0x0004  /* Queued commands pending        */
#define HS_RPT_TIMER_SIG        0x0001  /* Time to report to the watchdog */
#define HS_RSVD_2000_SIG        0x2000      /* TASK_OFFLINE_SIG */
#define HS_RSVD_4000_SIG        0x4000      /* TASK_STOP_SIG    */
#define HS_RSVD_8000_SIG        0x8000      /* TASK_START_SIG   */

/* How often (in milliseconds) to poll the GPIO 'keys' for their state.
*/
#define HS_GPIO_POLL_TIME       100

/* How long to delay (in microseconds) when resetting the one's detector
*/
#define HS_ONES_DET_DELAY       200

/* Time (in milliseconds) between turning on chipx8 in FM
*/
#define HS_FM_ONES_TIME         1200

/* How many polling periods between turning on chipx8 in FM
*/
#define HS_FM_ONES_PERIOD       HS_FM_ONES_TIME/HS_GPIO_POLL_TIME

#if (MODEL_ID == MODEL_B1) || (MODEL_ID== MODEL_B2)
#define CAMERA_JACKSENSE	// by jnlee 01.02.09  camera 인식 
#endif
/*---------------------------------------------------------------------------

  LCD Controller macros, commands, definitions, and types

---------------------------------------------------------------------------*/

/* Mask of GPIO bits for the LCD controller serial interface
*/
#define HS_LCD_MASK     ( BIO_LCD_DATA_M   \
                        | BIO_LCD_CLK_M    \
                        | BIO_LCD_A0_M     \
                        | BIO_LCD_SELECT_M )

/* A0 bit of an LCD command
*/
#define HS_LCD_A0       0x0100

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Epson SED1230 Instructions
*/

/* Cursor Home:  clear the address counter and move the cursor to
** the left edge of the first line.
*/
#define HS_LCD_HOME_I           0x0010

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Static Display Control:  control COMS1 and SEGS1 on/off/blink
** Set as All display segments ON
*/
#define HS_LCD_STATIC_I         0x0023

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Display on/off control:  on/off control for the display and cursors
** Set the display on/off, with no cursors.
*/
#define HS_LCD_DISPLAY_ON_I     0x0031
#define HS_LCD_DISPLAY_OFF_I    0x0030

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Power Save control:  setup or cancel powersave mode
** Powersave with the oscillator on, or non-power-save w/oscillator
*/
#define HS_LCD_POWERSAVE_OFF_I  0x0042
#define HS_LCD_POWERSAVE_ON_I   0x0043

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Power supply control:  control the internal power supply circuit.
** Turn on all of the supplies
*/
#define HS_LCD_POW_SUPPLY_I     0x005F

#define HS_LCD_POW_SUPPLY(p)    (0x0050 | (p))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* RAM control:  configure the RAM shape and CGRAM on/off
** 4 lines, use CGRAM
*/
#define HS_LCD_RAM_CTL_I        0x0069

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Contrast control:  electronic contrast control
** Set the contrast to midscale
*/
#define HS_LCD_CONTRAST_I       0x0078

#define HS_LCD_CONTRAST(c)      (0x0070 | (c))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Address select:  set the DDRAM, CGRAM, symbol address
*/
#define HS_LCD_ADDR_I(addr)     ( 0x0080 | (addr) )

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Write Data:  load data into the DDRAM, CGRAM, or symbol register
*/
#define HS_LCD_DATA_I(data)     ( HS_LCD_A0 | (data) )

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Number of rows in a character (characters are 5x7)
*/
#define HS_LCD_FONT_ROWS        7

/* Size to step to next character
*/
#define HS_LCD_FONT_CSTEP       (7+1)

/* Number of characters in the user-defined font
*/
#define HS_LCD_FONT_CHARS       4

/* Size of the entire user-defined font
*/
#define HS_LCD_FONT_SIZE        (HS_LCD_FONT_CSTEP * HS_LCD_FONT_CHARS)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

//#define HS_LCD_MAX_LINES        4       /* Number of lines on the display    */
//#define HS_LCD_MAX_CHARS        12      /* Number of characters per line     */

#define HS_LCD_LINE0_ADDR       0x0030  /* Start address of display line 0   */
#define HS_LCD_LINE1_ADDR       0x0040  /* Start address of display line 1   */
#define HS_LCD_LINE2_ADDR       0x0050  /* Start address of display line 2   */
#define HS_LCD_LINE3_ADDR       0x0060  /* Start address of display line 3   */

#define HS_LCD_SYMB_ADDR        0x0070  /* Start address of display symbols  */

#define HS_LCD_CGRAM_ADDR       0x0000  /* Start address of RAM font         */

/* Useful values for specifying line numbers
*/
#define HS_LCD_LINE0            0
#define HS_LCD_LINE1            1
#define HS_LCD_LINE2            2
#define HS_LCD_LINE3            3

/*---------------------------------------------------------------------------

  Keypad polling macros, commands, definitions, and types

---------------------------------------------------------------------------*/

/* Number of scan columns on the keypad
*/
#define HS_KEY_COLUMNS          6

/* Number of scan rows on the keypad
*/
#define HS_KEY_ROWS             4

/* Size of the row decode array (2^HS_KEY_ROWS) 2 raised to HS_KEY_ROWS.
*/
#define HS_2_TO_THE_KEY_ROWS    32

/* Mask of all keypad columns
*/
#define HS_KEYPAD_COL_M         ( BIO_KYPD_17_M | BIO_KYPD_15_M | \
                                  BIO_KYPD_13_M | BIO_KYPD_11_M | \
                                  BIO_KYPD_9_M  | BIO_KYPD_MEMO_M  )

/* Mask of all keypad rows
*/
#define HS_KEYPAD_ROW_M         ( BIO_KYPD_1_M | BIO_KYPD_3_M | \
                                  BIO_KYPD_5_M | BIO_KYPD_7_M )

/* Value meaning that no rows (keys) on this keypad column are down.
*/
#define HS_KEY_NO_ROW_V         ( BIO_KYPD_1_HI_V | \
                                  BIO_KYPD_3_HI_V | \
                                  BIO_KYPD_5_HI_V | \
                                  BIO_KYPD_7_HI_V )

/* Generate a mask for setting the column bits in the keyscan column outputs.
*/
#define HS_KEY_COL_MASK(i)      ((byte) (1 << (i)))

/* Generate a mask for reading the row bits in the keyscan row inputs.
*/
#define HS_KEY_ROW_MASK(i)      ((byte) (1 << (i)))

/* Initialize the keypad column drivers, and set them to 0's
** The MSM pulls up the rows through 100 Kohms to +5.  This
** is a max loss of ~(.05ma * 4) while keys are pressed, but
** this allows for a 0 setup transition to sleep/key_sense_int.
*/
#define HS_KEYPAD_INIT() \
  BIO_TRISTATE( DMOD_GPIO, HS_KEYPAD_COL_M, 0xFF); \
  BIO_OUT( DMOD_GPIO, HS_KEYPAD_COL_M, 0x00)


/*---------------------------------------------------------------------------

  BIO accessed bits which are taken as keys

---------------------------------------------------------------------------*/


/*===========================================================================

MACRO HS_ON_HOOK

DESCRIPTION
  Return the on-hook-in-hands-free-adapter status.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  True if the portable is in a hands-free adapter
  False is the portable is not in a hands-free adapter

SIDE EFFECTS
  None

===========================================================================*/
/* On the MSM3000, we have two UART's to use.  If using a second UART for DM, 
 * then the 2nd UART's RX line causes changes to AUX_PCM_DIN, which is read
 * by this MACRO.  If AUX_PCM_DIN is read with a particular value 2x in a 
 * row, then the code assumes it is in a hands-free kit.  That causes the 
 * SURF's handset receiver to be shut-off.  We don't want that, in the case
 * where we are using the second UART for DM.
 */
#ifndef WIN32
#define HS_ON_HOOK_INIT() \
  BIO_TRISTATE( ENC_GPIO_0,BIO_CRDL_M, BIO_OUTPUT_DIS_V);

#define HS_ON_HOOK() \
  ( BIO_INM( ENC_GPIO_0, BIO_CRDL_M ) == BIO_CRDL_ON_V )
#else
#define HS_ON_HOOK_INIT()       
#define HS_ON_HOOK()    FALSE
#endif

/*===========================================================================

MACRO HS_EXT_PWR_ON

DESCRIPTION
  Return the ignition on/off status when in the hands-free car kit.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  True if external power is supplied
  False is external power is not supplied

SIDE EFFECTS
  None

===========================================================================*/
#ifndef WIN32
#define HS_EXT_PWR_ON() \
      ( BIO_INM( ENC_GPIO_0, BIO_EXT_PWR_M ) == BIO_EXT_PWR_ON_V )
#else
#define ADC_CAMERA_SENSE   0x00
#define HS_EXT_PWR_ON()    TRUE
#endif

BOOL HS_EXT_PWR_ON_4GVM(void)
{
    return HS_EXT_PWR_ON();
}

/*===========================================================================

MACRO HS_EAR_UP

DESCRIPTION
  Return the earpiece up/down status.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  True if the earpiece is up
  False if earpiece is down

SIDE EFFECTS
  None

===========================================================================*/
// tasik 00.04.25, The code for FLIP is necessary.

#define HS_EAR_INIT() \
  BIO_TRISTATE( DEC_GPIO_0, BIO_FLIP_M, BIO_OUTPUT_DIS_V)

#ifndef WIN32
#define HS_EAR_UP() \
    ( BIO_INM( DEC_GPIO_0, BIO_FLIP_M ) == BIO_FLIP_OPEN_V )
#else
#define HS_EAR_UP()  TRUE
#endif

/*===========================================================================

MACRO HS_JACK_SENSE / HS_JACK_SENSE_INIT

DESCRIPTION
  Return the earphone jack in/out status and initializing GPIO pin

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  True if earphone is in
  False if earphone is out

SIDE EFFECTS
  None

===========================================================================*/
#if MODEL_ID == MODEL_B2
	#define HS_JACK_SENSE_INIT() \
			BIO_TRISTATE( DEC_GPIO_1, BIO_EAR_JACK_SENSE_M, BIO_OUTPUT_DIS_V);\
			BIO_TRISTATE( ENC_GPIO_0, BIO_JACK_SEND_M, BIO_JACK_SEND_LO_V)

	#ifndef WIN32
	#define HS_JACK_SENSE() \
			( BIO_INM ( DEC_GPIO_1, BIO_EAR_JACK_SENSE_M ) == BIO_EAR_JACK_SENSE_ON_V )
	#else
	#define HS_JACK_SENSE()  FALSE
	#endif

	#define HS_EAR_JACK_KEY() \
			( BIO_INM(ENC_GPIO_0, BIO_JACK_SEND_M ) == BIO_JACK_SEND_LO_V )

	#define BIO_JACK_SEND_ON_V 1
	#define BIO_JACK_SEND_OFF_V 0

#else	//#if MODEL_ID == MODEL_B2
	#define HS_JACK_SENSE_INIT() \
			BIO_TRISTATE( DEC_GPIO_1, BIO_EAR_JACK_SENSE_M, BIO_OUTPUT_DIS_V);\
			BIO_TRISTATE( DEC_GPIO_1, BIO_JACK_SEND_M, BIO_OUTPUT_DIS_V)

	#ifndef WIN32
	#define HS_JACK_SENSE() \
			( BIO_INM ( DEC_GPIO_1, BIO_EAR_JACK_SENSE_M ) == BIO_EAR_JACK_SENSE_ON_V )
	#else
	#define HS_JACK_SENSE()  FALSE
	#endif

	#ifndef WIN32
	#define HS_EAR_JACK_KEY() \
			( BIO_INM(DEC_GPIO_1, BIO_JACK_SEND_M ) == BIO_JACK_SEND_ON_V )
	#else
	#define HS_EAR_JACK_KEY() FALSE
	#endif
#endif //#if MODEL_ID == MODEL_B2
/*---------------------------------------------------------------------------

  Backlight initialization and control

---------------------------------------------------------------------------*/


/*===========================================================================

MACRO HS_BACKLIGHT_INIT

DESCRIPTION
  Initialize the GPIO output to drive the LCD and keypad backlight

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#ifndef WIN32
#define HS_BACKLIGHT_INIT() \
	BIO_TRISTATE( DEC_GPIO_1, BIO_DC_ON_M, BIO_DC_ON_HI_V);\
	BIO_TRISTATE( DEC_GPIO_0, BIO_LCD_LED_M, 0xFF)

#else
#define HS_BACKLIGHT_INIT() 
#endif


/*===========================================================================

MACRO HS_BACKLIGHT_ON

DESCRIPTION
  Turn on the LCD and keypad backlight

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if MODEL_ID == MODEL_B1 || (MODEL_ID== MODEL_B2)	// by youandi 01.05.28
byte fLEDOn;
void Ma2w_led_control(UINT8,UINT8);				// ymu759.h
#endif

#ifndef WIN32
#define HS_BACKLIGHT_ON() \
	BIO_OUT ( DEC_GPIO_1, BIO_DC_ON_M, BIO_DC_ON_HI_V);\
	BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_HI_V);\
	HS_SLEEP_FORBID()
#else
#define HS_BACKLIGHT_ON() 
#endif



/*===========================================================================

MACRO HS_BACKLIGHT_OFF

DESCRIPTION
  Turn off the LCD and keypad backlight

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifndef WIN32
#define HS_BACKLIGHT_OFF() \
	BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_LO_V);\
	BIO_OUT ( DEC_GPIO_1, BIO_DC_ON_M, BIO_DC_ON_LO_V);\
    HS_SLEEP_ALLOW()
#else
#define HS_BACKLIGHT_OFF()
#endif

/*---------------------------------------------------------------------------

  Call LED initialization and control

---------------------------------------------------------------------------*/
#define HS_CALL_LED_ON() \
	INTLOCK(); \
	fLEDOn = 1;\
	Ma2w_led_control(0x80,0); \
	INTFREE()

#define HS_CALL_LED_OFF() \
	INTLOCK(); \
	fLEDOn = 0;\
	Ma2w_led_control(0,0); \
	INTFREE()

#ifdef FEATURE_SKY_SUBLCD
#ifdef WIN32
#define HS_SUB_BACKLIGHT_INIT() 
#define HS_SUB_BACKLIGHT_ON() 
#define HS_SUB_BACKLIGHT_OFF()
#else
#define HS_SUB_BACKLIGHT_INIT()     BIO_TRISTATE (ENC_GPIO_1, BIO_EL_M, BIO_EL_HI_V)
#define HS_SUB_BACKLIGHT_ON()       BIO_OUT (ENC_GPIO_1, BIO_EL_M, BIO_EL_HI_V)
#define HS_SUB_BACKLIGHT_OFF()      BIO_OUT (ENC_GPIO_1, BIO_EL_M, BIO_EL_LO_V)
#endif
#endif /* #ifdef FEATURE_SKY_SUBLCD */



/*===========================================================================
MACRO HS_MOTOR_INIT
MACRO HS_MOTOR_ON
MACRO HS_MOTOR_OFF
===========================================================================*/
#if MODEL_ID == MODEL_B1 || (MODEL_ID== MODEL_B2)	// by youandi 01.05.28
byte fMotorOn;
void Ma2w_motor_control(UINT8,UINT8);
#endif

#define HS_MOTOR_INIT() 

#define HS_MOTOR_ON()\
	INTLOCK(); \
	Ma2w_motor_control(0x80,0);\
	fMotorOn=1;\
	INTFREE()

#define HS_MOTOR_OFF()\
	INTLOCK(); \
	Ma2w_motor_control(0,0);\
	fMotorOn=0;\
	INTFREE()


/*===========================================================================
MACRO HS_MEMO_INIT
MACRO HS_MEMO_ON
===========================================================================*/
#if MODEL_ID == MODEL_B1
#define HS_MEMO_INIT()\
	BIO_TRISTATE( DMOD_GPIO_INT, BIO_MEMO_M, BIO_MEMO_LO_V)		// input
#define HS_MEMO_ON()\
	(BIO_INM(DMOD_GPIO_INT, BIO_MEMO_M) ==  BIO_MEMO_LO_V)
#elif MODEL_ID == MODEL_B2	
#define HS_MEMO_INIT()\
	BIO_TRISTATE( ENC_GPIO_1, BIO_MEMO_M, BIO_MEMO_LO_V)		// input
#define HS_MEMO_ON()\
	(BIO_INM(ENC_GPIO_1, BIO_MEMO_M) ==  BIO_MEMO_LO_V)
#endif



/*---------------------------------------------------------------------------

  SLEEP allow/disallow macros

---------------------------------------------------------------------------*/

/*===========================================================================

MACRO HS_SLEEP_ALLOW

DESCRIPTION
  Tell the sleep task that we are in a state where we can sleep.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The sleep task may take us up on our offer.

===========================================================================*/
#ifdef T_SLEEP
#define HS_SLEEP_ALLOW() \
  { if(!HS_ON_HOOK())(void) SKY_Set_Signal( &sleep_tcb, SLEEP_HS_OKTS_SIG ); }
#else
#define HS_SLEEP_ALLOW()
#endif


/*===========================================================================

MACRO HS_SLEEP_FORBID

DESCRIPTION
  Tell the sleep task that we are NOT in a state where we can sleep.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef T_SLEEP
#define HS_SLEEP_FORBID() \
  { (void) SKY_Clear_Signal( &sleep_tcb, SLEEP_HS_OKTS_SIG ); }
#else
#define HS_SLEEP_FORBID()
#endif

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
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
byte bNumOfEarjIn = 0;		// earmic를 check하기 위하여 사용함  jnlee 00.12.28
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void SKY_API sPassKeyCode(SKY_EVENT_E KeyCode);
LOCAL void SKY_API sReadKeyCode(INT32 nInterval);
LOCAL void SKY_API sDetectEarJack(void);
//LOCAL boolean sPollGPIO(void);
//LOCAL void sDisableHFKPins(void);
//LOCAL void sEnableHFKPins(void);
LOCAL void SKY_API sHandleCommand(void);
LOCAL void SKY_API sSetSNIBlinkTimer(DWORD dwInterval);
LOCAL void SKY_API sSNITimerCallback(void);
LOCAL void SKY_API sSNIOn(HS_SNI_E SNI);
LOCAL void SKY_API sSNIOff(void);
LOCAL void SKY_API sTurnSNIOnOff(HS_SNI_E SNI, ONOFF OnOff);

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

#ifdef FEATURE_IDS
#error code not present
#endif

/* Set local message level
*/
word hs_msg_level = MSG_LVL_LOW;

#undef MSG_THRESHOLD
#define MSG_THRESHOLD   hs_msg_level
 
/*-----------------------------------------------------------------------
                         General Task Definitions 
-----------------------------------------------------------------------*/

/* REX timers */
LOCAL REX_TIMER_T l_RepeatTimer;      /* Timer for the handset task      */
LOCAL REX_TIMER_T l_GPIOTimer;        /* Timer for gpio 'key' polling    */
LOCAL REX_TIMER_T l_SNITimer;         /* Timer for SNI blinking */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Handset command queue
*/
LOCAL q_type hs_cmd_q;

/* Handset "free" command queue, holds unused/not-busy command
** structures for other tasks to use.
*/
q_type hs_cmd_free_q;

/* Buffers to link onto the "free" command queue.  The size is based on
** the consensus of the users of this queue.  UI wants to be able to use
** 7 or 8 "at once".
*/
#define                            HS_NUM_CMDS          8
LOCAL HS_COMMAND_U hs_cmd_bufs[ HS_NUM_CMDS ];

/*-----------------------------------------------------------------------
                           Keypad Definitions 
-----------------------------------------------------------------------*/

LOCAL REX_TCB_T     *l_pKeyReceiverTCB; /* Task receiving keypad signals   */
LOCAL REX_SIGNAL    l_KeyReceiverSignal;/*  Signal to send to key_rcvr task */

/* Ring buffer for holding keycodes.  When read index == write index, it is
** empty.  When read index is 1 ahead of write index, the buffer is full.
** Note:  this "burns" 1 datum (a byte) in order to tell full from empty.
*/
#define HS_KEY_BUF_SIZE 32      /* Max keycodes to buffer    */
#define HS_KEY_BUF_MASK 0x1f    /* Mask for modulus in ring  */

/* keycode input ring buffer */
struct tagHS_KEY_BUFFER_T
{
    INT8 iRead;                             /* read index */
    INT8 iWrite;                            /* write index */
    SKY_EVENT_E KeyCode[HS_KEY_BUF_SIZE];   /* key code buffer */
};
typedef struct tagHS_KEY_BUFFER_T   HS_KEY_BUFFER_T;

LOCAL HS_KEY_BUFFER_T   l_KeyBuffer;

/* Keypad-input Processing States
**
** While processing key inputs, we need to know what "state" we are
** in so that we can process events properly.
**
**      KIS_NO_KEY      No key has been received.  When we get one,
**                      change to KIS_DEBOUNCE.
**
**      KIS_DEBOUNCE    We have a key, delay for the debounce interval
**                      ignoring all other key codes.  If the key is still
**                      pressed at the end of the interval, go to KIS_UP_WAIT.
**                      If not, go back to KIS_NO_KEY.
**
**      KIS_UP_WAIT     Buffer the keycode and signal the task that is waiting
**                      for keycodes.  Then wait for _all_ keys to be released.
**                      The keys are polled once every interval.  When the keys
**                      have been released, buffer up SE_KEY_RELEASE keycode and
**                      signal the task again.
*/

/* current key processing state */
enum tagKEY_INPUT_STATE_E
{
    KIS_NO_KEY = 1,
    KIS_DEBOUNCE,
    KIS_UP_WAIT
};
typedef enum tagKEY_INPUT_STATE_E   KEY_INPUT_STATE_E;

LOCAL KEY_INPUT_STATE_E l_KeyInputState = KIS_NO_KEY;


/* Last/current keycode being processed.
*/
LOCAL SKY_EVENT_E   l_LastKeyCode = SE_KEY_NONE;

/* Keep track if the Handset is currently locked so the user cannot use it.
** this functionality is used via the DM.
*/
LOCAL BOOL l_fLocked = FALSE;
LOCAL BOOL l_fPhoneOpen = FALSE;

enum tagEAR_JACK_STATE_E
{
    EJS_FIRST,
    EJS_LONG,
    EJS_PAUSE,
    EJS_EXIT
};
typedef enum tagEAR_JACK_STATE_E    EAR_JACK_STATE_E;

//LOCAL EAR_JACK_STATE_E    l_EarJackState = EJS_FIRST;
//LOCAL SKY_EVENT_E         l_EarJackOnOffKey = SE_KEY_EAR_JACK_OFF;
//LOCAL BYTE                l_bEarJackKey;
//LOCAL BOOL                l_fEarJackEnalbe = FALSE;

/* Clock call-back structure used to register the keypad scanning routine
** to be called at regular intervals.
*/
//LOCAL clk_cb_type hs_cb_keypad;

LOCAL CLOCK_CALLBACK    l_KeyPadCAllback;


/*-----------------------------------------------------------------------
                    External Device Interface Data   
-----------------------------------------------------------------------*/

#ifdef FEATURE_IDS
#error code not present
#endif

/* Flags to indicate that handset initiated GPIO polling the very
 * first time.
 */
LOCAL BOOL l_fFirstGPIOPoll = FALSE;

/* Variable to hold the HS_FREE_K or individual device ID key 
 */
LOCAL SKY_EVENT_E l_DeviceIDKey = SE_KEY_HANDS_FREE_ON;

/* Flag to indicate if it is a coil cord carkit
 */
//boolean hs_coil_cord_hfk = FALSE;

/* Used to dynamically switch between the second UART and the HFK.
 * When TRUE  the HFK is connected to the shared MSM pins.
 * When FALSE the 2nd UART is connected to the shared MSM pins.
 */
LOCAL BOOL l_fHandsFreePinsEnabled = TRUE;


/*-----------------------------------------------------------------------
                              Keypad Routines
-----------------------------------------------------------------------*/

/*===========================================================================

FUNCTION HS_KEY_GET

DESCRIPTION
  Read a keycode from the input buffer.

DEPENDENCIES
  None

RETURN VALUE
  This procedure returns the oldest unread keycode.  If no keycode
  is currently available, HS_NONE_K is returned.

SIDE EFFECTS
  None

===========================================================================*/
SKY_EVENT_E SKY_API SKY_GetKey(void)
{
    SKY_EVENT_E KeyCode;
	WORD wInterruptState;   /* PSW parameter for INTLOCK/FREE_SAV    */

	INTLOCK_SAV( wInterruptState ); /* Disable interrupts and save PSW */
    /* buffer is empty */
    if (l_KeyBuffer.iWrite == l_KeyBuffer.iRead) 
    { 
        KeyCode = SE_KEY_NONE;
    }
    /* get code & increment ring read index */
    else 
    {
        KeyCode = l_KeyBuffer.KeyCode[l_KeyBuffer.iRead];
        l_KeyBuffer.iRead = (l_KeyBuffer.iRead+1) & HS_KEY_BUF_MASK;
    }

	INTFREE_SAV( wInterruptState ); /* Restore interrupts (PSW) */
    return KeyCode;

} /* end of hs_key_get */


/*===========================================================================

LOCAL FUNCTION PASS_KEY_CODE

DESCRIPTION
  Pass a keycode from the Handset task to the receiver.  This involves
  queueing the keycode on the ring buffer, and signaling the task which
  registered as the receiver task, by calling hs_key_init().  If there
  is no room in the buffer, an error is logged and the task is signalled
  anyway.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void SKY_API sPassKeyCode(SKY_EVENT_E KeyCode)
{
    WORD wInterruptState;   /* PSW parameter for INTLOCK/FREE_SAV    */

    /* If the ring buffer is not full (full is when the write index is
    ** circularly 1 less than the read index), put the keycode into the
    ** ring buffer.
    */
    INTLOCK_SAV( wInterruptState ); /* Disable interrupts and save PSW */

    if (((l_KeyBuffer.iWrite + 1) & HS_KEY_BUF_MASK) != l_KeyBuffer.iRead) 
    {
        l_KeyBuffer.KeyCode[l_KeyBuffer.iWrite] = KeyCode;
        l_KeyBuffer.iWrite = (l_KeyBuffer.iWrite+1) & HS_KEY_BUF_MASK;

		/* If we have something to signal, signal it that we have keycodes.
		** Signal it even if the buffer was too full to fit in the keycode.
		*/
		if (l_KeyReceiverSignal) 
		{
			(void) SKY_Set_Signal( l_pKeyReceiverTCB, l_KeyReceiverSignal );
		}

    }
    else 
    {
        SKY_ERROR1("Input buffer overflow - size", HS_KEY_BUF_SIZE); 
    }

    INTFREE_SAV( wInterruptState ); /* Restore interrupts (PSW) */
    
    SKY_TRACE1(TL_LOW, "Keycode = 0x%0X", KeyCode);
} /* end of pass_key_code */

/*===========================================================================

LOCAL FUNCTION HS_SCAN_KEYPAD

DESCRIPTION
  Read a keycode from the keypad.  This includes handling the state machine
  for debouncing the key and the all-keys-up key-release.

DEPENDENCIES
  This routine is called from interrupt context, and HS task cannot preempt
  it's access to the key buffer or the hs_key_state variable.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void SKY_API sReadKeyCode(INT32 nInterval)
{
    /* Given a row and column, decode it into a keycode.  The comment above
    ** each entry is it's row/column address.
    */
    STATIC ROM SKY_EVENT_E aKeyCode[HS_KEY_ROWS][HS_KEY_COLUMNS] = {
		SE_KEY_UP,      SE_KEY_LEFT,    SE_KEY_F1,		
		SE_KEY_CLEAR,   SE_KEY_F2,		SE_KEY_RECORD,

		SE_KEY_POUND,   SE_KEY_9,		SE_KEY_6,       
		SE_KEY_3,       SE_KEY_RIGHT,   SE_KEY_CENTER,  

		SE_KEY_0,		SE_KEY_8,       SE_KEY_5,       
		SE_KEY_2,       SE_KEY_DOWN,    SE_KEY_NONE,

		SE_KEY_STAR,    SE_KEY_7,       SE_KEY_4,       
		SE_KEY_1,       SE_KEY_SEND,	SE_KEY_NONE
    };

    /* Take a column scan and return 1 row number for a key decode.
    ** The low order rows are given priority, for no particular reason.
    ** The row number is selected if the scan has a 0 at that position.
    **
    ** Note: 1111 (HS_KEY_NO_ROW_V) is not a level scan value for this
    ** array to decode.
    */
    STATIC ROM INT8 nRowDecode[] = {
        0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
        0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 0
    };
    INT8 nRow, nColumn;
    SKY_EVENT_E KeyCode;
    BYTE bKeyScan;
    
    /* unused parameter */
    UNUSED(nInterval);

    /* If the keypad is locked, don't scan any keys. */
    if(l_fLocked)
        return;
        
    HS_SLEEP_FORBID();              /* Keys being processed, no napping */

    /* 
    ** Initialize the keycode to 'no key' so that if we fall through
    ** the scan logic without finding a key, we'll know it.
    */
    KeyCode = SE_KEY_NONE;
    bKeyScan = HS_KEY_NO_ROW_V;

    /* 
    ** Check the Power key (It's a separate line), 
    ** note this is also the end key
    */
    if ( BIO_INM( DMOD_KEY_READ, BIO_ON_SW_SENSE_M ) == BIO_ON_SW_ON_V ) 
    {
        SKY_TRACE(TL_MED, "Power key pressed");
        KeyCode = SE_KEY_END;
        //KeyCode = SE_KEY_POWER;
    }
	// memo key pushed		by jnlee 01.01.29
	else if ( HS_MEMO_ON())
	{
        KeyCode = SE_KEY_AUTO_ANSWER;
	}
    /* If the power key is not pressed (checked above),
    ** check to see if any other key is pressed.
    */
    else 
    {
        /* Keys are scanned by driving the columns HIGH (1), and syncing one
        ** column at a time to LOW (0).  A pressed button connects a row and
        ** column together.  The rows are pulled HIGH (1), and driving the
        ** column with a LOW (0) drains the charge down the connection to the
        ** column driver.  This shows as a 0 when the row is read.
        */

        /* Set the columns LOW
        */
#ifndef WIN32
        BIO_OUT( DMOD_GPIO, HS_KEYPAD_COL_M, 0x00);
#endif
        /* Read the row and see if any key is still 'down' (0)
        */
        bKeyScan = BIO_INM( DMOD_KEY_READ, HS_KEYPAD_ROW_M );
        if (bKeyScan != HS_KEY_NO_ROW_V ) 
        {
          KeyCode = l_LastKeyCode;
        }

    }

    /* If no key yet scanned, and a scanned key has been pressed,
    ** scan the keypad to see which one it was.  Note: that the power
    ** key is a completely separate key, and is checked above.
    */
    if ( KeyCode == SE_KEY_NONE && bKeyScan != HS_KEY_NO_ROW_V ) 
    {
        /* Scan for a new key over all columns
        **
        ** Keys are scanned by driving the columns HIGH (1), and syncing one
        ** column at a time to LOW (0).  A pressed button connects a row and
        ** column together.  The rows are pulled HIGH (1), and driving the
        ** column with a LOW (0) drains the charge down the connection to the
        ** column driver.  This shows as a 0 when the row is read.
        ** 
        ** When done scanning, leave the columns as 0.  This costs ~0.05ma
        ** during key presses, but allows for 0 setup when going to sleep.
        */
        for (nColumn = 0; nColumn < HS_KEY_COLUMNS; nColumn++ ) 
        {

            /* Set the column LOW (0), the pressed key will show a LOW (0)
            ** in it's position in the row being scanned.  This sets the other
            ** columns HIGH(1).
            */
#ifndef WIN32
            BIO_OUT( DMOD_GPIO, HS_KEYPAD_COL_M, ~( HS_KEY_COL_MASK( nColumn ) ) );
#endif
            bKeyScan = BIO_INM( DMOD_KEY_READ, HS_KEYPAD_ROW_M ); /* Scan the row */

            /* If key(s) are pressed in this column
            */
            if (bKeyScan != HS_KEY_NO_ROW_V ) 
            {
                /* Scan across the rows looking for a key 
                */
                nRow = nRowDecode[ bKeyScan ];
                KeyCode = aKeyCode[nRow][nColumn];
                break;
            }
        } /* for */
#ifndef WIN32
        BIO_OUT( DMOD_GPIO, HS_KEYPAD_COL_M, 0x00); /* Set the columns LOW (0)  */
#endif
    } /* if */
    

    /* Process the key based on the current state */
    switch(l_KeyInputState)
    {
    /* If no keycode is being processed, start debouncing this key. */
    case KIS_NO_KEY:
        l_LastKeyCode = KeyCode;
        if(KeyCode != SE_KEY_NONE ) 
        {
            l_KeyInputState = KIS_DEBOUNCE; /* next poll, debounce the key */

            SKY_TRACE1(TL_MED, "New key %d", KeyCode);
        } 
        else 
        {
            HS_SLEEP_ALLOW();               /* No keys waited on, sleep now     */
        }
        break;

    /* If waiting for all keys to be up  */
    case KIS_UP_WAIT:
        /* if all keys released  */
        if(KeyCode == SE_KEY_NONE ) 
        {
            l_LastKeyCode = SE_KEY_NONE;
            l_KeyInputState = KIS_NO_KEY;   /* go wait for a new key-down       */
            sPassKeyCode( SE_KEY_RELEASE );   /* pass a HS_RELEASE_K to receiver  */
            SKY_TRACE(TL_MED, "All keys up");
            HS_SLEEP_ALLOW();               /* No keys waited on, sleep now     */
        }
        break;

    /* If debouncing a key-down */
    case KIS_DEBOUNCE:
        if(KeyCode == l_LastKeyCode ) 
        {
            l_KeyInputState = KIS_UP_WAIT;  /* Now we wait for key release      */
            sPassKeyCode( KeyCode );        /* Pass the keycode to the receiver */
            SKY_TRACE1(TL_MED, "Debounced %d", KeyCode);
        }
        /* Start debounce on a new key */
        else 
        {
            l_LastKeyCode = KeyCode;      /* key changed, go with it          */
            if ( KeyCode == SE_KEY_NONE ) 
            {
                l_KeyInputState = KIS_NO_KEY;     /* key glitch, ignore it            */
                l_LastKeyCode = SE_KEY_NONE;
                SKY_TRACE(TL_MED, "All keys up");
                HS_SLEEP_ALLOW();             /* No keys waited on, sleep now     */
            }
            else 
            {
                SKY_TRACE1(TL_MED, "Re-bounce, key %d", KeyCode);
            }
        }
        break;

    default:
        SKY_ERROR_FATAL1("Illegal key state %d", l_KeyInputState);
        /* does not return */
    }

} /* end of hs_scan_keypad */


/*===========================================================================

FUNCTION HS_KEY_INIT

DESCRIPTION
  This procedure performs any keypad initialization, and remembers what task
  to signal when keycodes become available.  This routine stuffs any steady-
  state keys into the key buffer, Ignition on/off and in/out-of hands-free
  cradle.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*
** pTask : 
**      Task to signal when keycodes arrive
** KeySignal :
**      Signal to send task when keycodes arrive 
*/
void SKY_API SKY_InitKey(REX_TCB_T *pTask, REX_SIGNAL KeySignal)
{
    INTLOCK();                       /* Do both updates TOGETHER     */
    l_pKeyReceiverTCB = pTask;       /* remember task to signal      */
    l_KeyReceiverSignal = KeySignal; /* remember signal to send      */
    INTFREE();

    /* If one or more keys are already in the buffer, and the task is 
    ** properly registered, then send the signal NOW!
    */
#ifdef FEATURE_SKVM
    if (l_KeyReceiverSignal) /* But Task IS Registered  */
    {                      
            (void) SKY_Set_Signal( l_pKeyReceiverTCB, l_KeyReceiverSignal );
    }
#else
    if (l_KeyBuffer.iWrite != l_KeyBuffer.iRead) /* Buffer NOT Empty  */
    { 
        if (l_KeyReceiverSignal) /* But Task IS Registered  */
        {                      
            (void) SKY_Set_Signal( l_pKeyReceiverTCB, l_KeyReceiverSignal );
        }
    }
#endif
} /* end of hs_key_init */

void SKY_EmptyKey(void) //redstar 2001.06.24
{
    INTLOCK();  
    l_KeyBuffer.iWrite = l_KeyBuffer.iRead ;
    INTFREE();
}

//#if 0 /* temp 0000000000000000000000000000000000000000000000000000000000000 */

/*-----------------------------------------------------------------------
                External Device and GPIO Polling Routines
-----------------------------------------------------------------------*/


/*===========================================================================

FUNCTION HS_PASS_ACCESSSORY_KEYS

DESCRIPTION
  This routine decides what keys to pass to UI based on the status of 
  attached accessory.

DEPENDENCIES
  hs_first_gpio_poll

RETURN VALUE
  None

SIDE EFFECTS
  hs_first_gpio_poll

===========================================================================*/

LOCAL void hs_pass_accessory_keys
(
  boolean ext_dev,       /* TRUE if an external device has     */
                         /* been detected at the ONES Detector */

  boolean ext_pwr        /* TRUE if External Power is          */
                         /* currently detected                 */
)
{
  /* Remember the status of last time 
   */
  static boolean last_ext_dev = FALSE;
  static boolean last_ext_pwr = FALSE;

  /* Pass accessory keys for initial power up
   */
  if ( l_fFirstGPIOPoll )
  {
    /* Pass device key first if it is initial power up
     */
    if ( ext_dev ) 
      sPassKeyCode( l_DeviceIDKey );
    else
      sPassKeyCode( SE_KEY_HANDS_FREE_OFF );
    sPassKeyCode( SE_KEY_RELEASE );
  
    /* Pass external power key
     */
    if ( ext_pwr )
    {
      sPassKeyCode( SE_KEY_EXT_POWER_ON );
      sPassKeyCode( SE_KEY_RELEASE );
    }

    /* Update the status flags
     */ 
    last_ext_pwr = ext_pwr;
    last_ext_dev = ext_dev;
    l_fFirstGPIOPoll = FALSE;
    
    return;
  }

  /* Decide whether External Power key 
   */
  if ( ext_pwr != last_ext_pwr )
  {
    sPassKeyCode( (ext_pwr ? SE_KEY_EXT_POWER_ON : SE_KEY_EXT_POWER_OFF) );
    
    last_ext_pwr = ext_pwr;
  }

  /* Decide whether to pass HS_FREE_K / Device ID key
   */ 
  if ( ext_dev != last_ext_dev && ext_dev )
  {
    sPassKeyCode( l_DeviceIDKey );
    last_ext_dev = ext_dev;
  }

  /* This routine tests for the need to send a "fake" ignition key to the
  ** UI.  Note that it is NOT enclosed in a ONES detect change if().  This
  ** is because the state of BOTH External Power and Ones detect figure
  ** into the decision.  Also note that we don't worry about state_changed
  ** here because this routine only sends a key if SOMETHING ELSE has
  ** also changed and thus state_changed will already be TRUE.
  */
//  if ( !hs_coil_cord_hfk )
//    hs_ignition_key( ext_dev, ext_pwr );

  /* Pass Phone Key if no external device is detected
   */
  if ( ext_dev != last_ext_dev )
  {
    sPassKeyCode( SE_KEY_HANDS_FREE_OFF );
    last_ext_dev = ext_dev;
  }
}

//#endif /* temp 0000000000000000000000000000000000000000000000000000000000000 */


///// Add by happykid in 2000.6.27
/*===========================================================================

FUNCTION Detect_ear_jack_button

DESCRIPTION
  This procedure polls the GPIOs which are ear jack button
  signals as needed. This routine is called per 50msec
  
DEPENDENCIES
  jack_sense

RETURN VALUE

SIDE EFFECTS
  hs_accessory_state

===========================================================================*/
#define sMAX_KEY_COUNT      7

LOCAL void SKY_API sDetectEarJack(void)
{
    WORD wInterruptState;   /* PSW parameter for INTLOCK/FREE_SAV    */
    BYTE bEarJack;          /* ear jack button */
    STATIC EAR_JACK_STATE_E s_EarJackState = EJS_FIRST; /* ear jack state */
    /* the counter for detecting double click or long click */
    STATIC INT8 s_nKeyCount = 0;    
    
    INTLOCK_SAV(wInterruptState); /* Disable interrupts, protect hs_key_state */
#if MODEL_ID==MODEL_B2
    bEarJack = ((HS_EAR_JACK_KEY()) 
                    ? BIO_JACK_SEND_ON_V : BIO_JACK_SEND_OFF_V);
/*********for B2 by jnlee 01.06.18***************/
/*
	if(adc_read (ADC_CAMERA_SENSE) <= 10)	// send키를 누르면 micj가 adc가 0v가 됨 
		bEarJack = BIO_JACK_SEND_ON_V;
	else
		bEarJack = BIO_JACK_SEND_OFF_V;
*/
#else    
    bEarJack = ((HS_EAR_JACK_KEY()) 
                    ? BIO_JACK_SEND_ON_V : BIO_JACK_SEND_OFF_V);
#endif
    switch(s_EarJackState)
    {
    /* first try to detect ear jack */
    case EJS_FIRST:
        /* if ear jack button is pressed */
        if( bEarJack == BIO_JACK_SEND_ON_V)
        {
            /* go to try to detect long pressing */
            s_nKeyCount = 0;
            s_EarJackState = EJS_LONG;
            SKY_TRACE(TL_MED, "ear jack button is pressed");
        }
        break;
    case EJS_LONG:
        /* if ear jack button is pressed */
        if( bEarJack == BIO_JACK_SEND_ON_V)
        {
            /* increase the counter */
            s_nKeyCount++;
            /* if the counter is larger than the max value */
            if(s_nKeyCount > sMAX_KEY_COUNT)
            {
                /* pass the ear long click keycode to the receiver task */
                sPassKeyCode(SE_KEY_EAR_LONG_CLICK); 
                sPassKeyCode(SE_KEY_RELEASE);
                s_EarJackState = EJS_EXIT;
            }
            SKY_TRACE1(TL_MED, "ear jack button is pressing (%d)", s_nKeyCount);
        }
        /* ear jack button is released before detecting as long key */
        else
        {
            /* reset the counter */
            s_nKeyCount =0;
            /* pass the event as an ear jack button click */
            sPassKeyCode(SE_KEY_EAR_CLICK);
            sPassKeyCode(SE_KEY_RELEASE);

            HS_SLEEP_ALLOW(); /* No keys waited on, sleep now */
            /* go to try to detect the ear jack button double click */
            s_EarJackState = EJS_PAUSE;
            SKY_TRACE(TL_MED, "ear jack button is released");
        }
        break;
    case EJS_PAUSE :
        /* 
        ** ear jack button is pressed before going exit state.
        ** it means double clicking the ear jack button.
        */
        if(bEarJack == BIO_JACK_SEND_ON_V)
        {
            sPassKeyCode(SE_KEY_EAR_DOUBLE_CLICK);
            sPassKeyCode(SE_KEY_RELEASE);

            HS_SLEEP_ALLOW(); /* No keys waited on, sleep now */
            s_EarJackState = EJS_EXIT;
            SKY_TRACE(TL_MED, "ear jack button is pressed again --> double click");
        }
        /* ear jack button is released */
        else
        {
            /* increase key count */
            s_nKeyCount++ ;
            /* key count is larger than the max value + 1 */
            if(s_nKeyCount > sMAX_KEY_COUNT+1)
            {
                /* nothing happened. 
                ** it means an ear jack button was just clicked.
                ** go to exit state 
                */
                s_EarJackState = EJS_EXIT;
            }
        }
        break;
    case EJS_EXIT:
        if( bEarJack == BIO_JACK_SEND_OFF_V) 
            s_EarJackState = EJS_FIRST;
        break;
    } // switch(ear_jack_key_state )
    
    INTFREE_SAV(wInterruptState); /* Restore interrupts */
}
///End happykid

/*===========================================================================

FUNCTION HS_POLL_GPIO

DESCRIPTION
  This procedure polls the GPIOs which are keys, handling key event
  signals for new keycodes as needed.
  
  NOTE: this routine is NOT just called by the Handset Task!  It is also
  called by other tasks in some cases!

DEPENDENCIES
  hs_locked
  hs_accessory_state
  hs_first_gpio_poll

RETURN VALUE
  True if anything changed or if keys are waiting.
  False if no changes/keys occurred.

  Note: while not used by the Handset task, this return values IS USED when
    this routine is called by othoer tasks!

SIDE EFFECTS
  hs_accessory_state

===========================================================================*/
/* we need hs_poll_gpio function, because srchdz.c srchzz.c use it */
//boolean (*hs_poll_gpio)(void) = sPollGPIO;
//LOCAL boolean sPollGPIO(void)

#if MODEL_ID == MODEL_B1 || (MODEL_ID== MODEL_B2)		// by jnlee 01.04.06
// camera삽입시에만 (TRUE)인 경우에만 
// sleep 시 audio power off하도록 함 
byte fAudioPwrOff = 0;

STATIC BOOL fAbnormalEarphone=FALSE;
static int sMAX_ABNORMAL_CAMERA_DEALY=4;

#endif

#if MODEL_ID == MODEL_B2
	BOOL fFolderKeyChanged = 0;
	BOOL IdleScreen = 0;	//partial display상태 일때 1
	BOOL fPhoneOpen = TRUE;//FALSE;
	BOOL PartialDisplayOn = 0;
#endif


boolean hs_poll_gpio(void)
{ 
    /* To remember if we are holding codec on */
    STATIC BOOL s_fHoldingCodec = FALSE;
    /* To remember we need to send handset keys when everything is OK. */
    STATIC BOOL s_fAccessoryKeyRequired = FALSE;
    /* Count down to limit time between each CHIPx8 Power Up */
    STATIC INT s_nCHIPx8Count = HS_FM_ONES_PERIOD;
    STATIC BOOL s_fFirstONESPoll = TRUE;          /* Indicates the first poll */

    /* Status of last poll */
    STATIC BOOL s_fLastExtPower = FALSE;        /* external power status    */
    STATIC BOOL s_fLastExtDevice = FALSE;       /* external device status   */
    STATIC BOOL s_fLastONES = FALSE;            /* ONES detector status     */

    /* Status of this poll */
    BOOL fExtPower = FALSE;                     /* external power status    */
    BOOL fExtDevice = FALSE;                    /* external device status   */
    BOOL fONES = FALSE;                         /* ONES detector status     */

    //BOOL fPhoneOpen = TRUE;//FALSE;
    BOOL fConsistentONES = FALSE;               /* ONES status is consitent */

    /* Keep track of this routine's return value in here */
    BOOL fStateChanged;     /* return value */

    /* Temporary to request DB_RF_MODE from DataBase */
    DB_ITEM_VALUE_U RFValue;

    WORD wInterruptState;         /* PSW parameter for INTLOCK/FREE_SAV    */
    SKY_EVENT_E PhoneOpenCloseKey;

    BOOL fEarJack;
    STATIC BOOL s_fJackSenseEnable = FALSE;
    STATIC BOOL s_fLastEarJack = FALSE;
    SKY_EVENT_E EarJackOnOff = SE_KEY_EAR_JACK_OFF;
#if MODEL_ID == MODEL_B1 || (MODEL_ID== MODEL_B2)
	BOOL fCameraJack = FALSE;
    STATIC BOOL s_fLastCameraJack = FALSE;

	STATIC BOOL fCheckAbnormalCamera = FALSE;
	STATIC UINT8 nAbnormalCameraCount = 0;
#endif

    STATIC  UINT8   nEarDelayCount = 0; // for Guard Time
#if MODEL_ID == MODEL_B1 || (MODEL_ID== MODEL_B2)
    STATIC  UINT8   nEarMicDelayCount = 0; // for Guard time
    STATIC  UINT8   nCameraDelayCount = 0; // for Guard Time
#endif

    /* Assume nothing interesting will happen as the default return value. */
    fStateChanged = FALSE;

    /* Even though these are not "keys" on the keypad, they are treated as
    ** key events to the UI.  If the handset is locked then don't look at
    ** these keys here.  (Use DM keys ONLY in that case.)
    */
    if(!l_fLocked) 
    {
        /* Read the status of External Power. */
        fExtPower = HS_EXT_PWR_ON();

        if(fExtPower != s_fLastExtPower) 
        {
            SKY_TRACE1(TL_HIGH, "External power is %d", fExtPower);
            /* Update status */
            s_fLastExtPower = fExtPower; 
            /* Need to send external power key */
            s_fAccessoryKeyRequired = TRUE;  
            /* Allow sleep */
            HS_SLEEP_ALLOW();
            
            fStateChanged = TRUE;
        }

        /* Check to see if the earpiece switch has changed */
        fPhoneOpen = HS_EAR_UP();

		//asdf 01.11.23 count for power saving
		fFolderKeyChanged = (fPhoneOpen) ? 1 : 2 ;
		if((fFolderKeyChanged == 2) && (IdleScreen == 0))//folder close
		{
			if (PartialDisplayOn < 100)//10초뒤에 power saving mode로 들어가기위해 
			{
				if(fPhoneOpen != l_fPhoneOpen)
				{
					PartialDisplayOn = 0;
				}
				else 
				{
					PartialDisplayOn++;
				}
			}
			else	SKY_PartialDisplayOn();
		}

        if( fPhoneOpen != l_fPhoneOpen )
        {
            nEarDelayCount++;
        }
        else
        {
            nEarDelayCount = 0;
        }

//        if(fPhoneOpen != l_fPhoneOpen ) 
        if( ( SKY_IS_ALERT() && (fPhoneOpen != l_fPhoneOpen && nEarDelayCount > 2)) ||
            ( !SKY_IS_ALERT() && (fPhoneOpen != l_fPhoneOpen)) )
        {
            nEarDelayCount = 0;
            l_fPhoneOpen = fPhoneOpen;     /* Invert the state */

            /* Disable interrupts, protect l_KeyInputState */
            INTLOCK_SAV( wInterruptState );

            /* Select ear up or down key */
            PhoneOpenCloseKey = (SKY_EVENT_E) ((fPhoneOpen) ? SE_KEY_PHONE_OPEN : SE_KEY_PHONE_CLOSE);

            if(l_KeyInputState == KIS_UP_WAIT) 
            {
                /* Pass a SE_KEY_RELEASE to receiver */
                sPassKeyCode(SE_KEY_RELEASE);
            }

            /* Pass key to show that the state changed, on/off */
            sPassKeyCode( PhoneOpenCloseKey );
#if MODEL_ID == MODEL_B2
			fFolderKeyChanged = (fPhoneOpen) ? 1 : 2 ;
#endif
            SKY_TRACE1(TL_HIGH, "Earpiece switch is %d", fPhoneOpen);

            /* Pass a HS_RELEASE_K to receiver */
            sPassKeyCode(SE_KEY_RELEASE);

            if (l_KeyInputState != KIS_NO_KEY)
            {
                /* Go wait for keys to be released */
                l_KeyInputState = KIS_UP_WAIT;
            }
            /* Restore interrupts */
            INTFREE_SAV(wInterruptState);

            /* No keys waited on, sleep now */
            HS_SLEEP_ALLOW();               
            fStateChanged = TRUE;
        }
        /* Check to see if the jack switch has changed */
        fEarJack = HS_JACK_SENSE();

#ifdef CAMERA_JACKSENSE 
		if(fEarJack)
		{
			// 현재상태가 camera inserted 상태이면 earjack off만 check해야함
			// camera inserted상태이면 아래는 check할 필요 없음 by jnlee 01.02.22
			if(s_fLastCameraJack == FALSE)
			{
				if (s_fLastEarJack == FALSE)
				{
				/*********** last_camera:F, last_ear:F************/
					volatile byte adread[10],i;
					word sum_adr = 0;
				#if MODEL_ID==MODEL_B1	// by jnlee 01.04.23
					#define EARMIC_THRES 0xfe
					#define CAM_CHECK_TIME 3
				#elif MODEL_ID == MODEL_B2
					#define EARMIC_THRES 200
					#define CAM_CHECK_TIME 1
				#endif
					// cam check하기 위하여 audio_en전원 사용 
					AUDIO_EN_ON();
					fAudioPwrOff |= AUDIO_EN_BY_CAM_CHK;

					for (i=0 ; i<CAM_CHECK_TIME ;i++)
					{
						adread[i] = adc_read (ADC_CAMERA_SENSE);
						sum_adr += adread[i];
					}
					sum_adr /= CAM_CHECK_TIME;
#if MODEL_ID == MODEL_B2
					if(sum_adr == 0 || sum_adr == 0xff)
					{
						fEarJack = FALSE;
						fCameraJack = TRUE;
					}
					else
					{
						fEarJack = TRUE;
						fCameraJack = FALSE;
					}
#else
					if(sum_adr < EARMIC_THRES)
					{
							fEarJack = TRUE;
							fCameraJack = FALSE;
					}
					else
					{
						fEarJack = FALSE;
						fCameraJack = TRUE;
					}
#endif
					#if MODEL_ID == MODEL_B2
					fAudioPwrOff &= ~AUDIO_EN_BY_CAM_CHK;
					AUDIO_EN_OFF();
					#endif
				}
				else
				{
				/*********** last_camera:F, last_ear:T************/
				#if MODEL_ID==MODEL_B1	// by jnlee 01.04.23
					// cam check하기 위하여 사용한 audio_en전원 
					//fAudioPwrOff &= ~AUDIO_EN_BY_CAM_CHK;
					//AUDIO_EN_OFF();
				#endif //#if MODEL_ID==MODEL_B1	
				}
			}
			else
			{
			/*********** last_camera:T, last_ear:NotCare(F)************/
			#if MODEL_ID==MODEL_B1	// by jnlee 01.04.23
				// cam check하기 위하여 사용한 audio_en전원 
				//fAudioPwrOff &= ~AUDIO_EN_BY_CAM_CHK;
				//AUDIO_EN_OFF();
			#endif //#if MODEL_ID==MODEL_B1	
				fEarJack = FALSE;
				fCameraJack = TRUE;
			}
		}
		else
		{
			fCameraJack = FALSE;
		}
//		MSG_ERROR ("**~~**Earjack:%d,CameraJ:%d",fEarJack,fCameraJack,0);
#endif //#ifdef CAMERA_JACKSENSE

        /* 
        ** make delay to detect the ear jack 
        ** because no delay makes putting ear jack as ear jack button event 
        */
        if(!s_fJackSenseEnable)
        {
#define sMAX_EAR_JACK_SENSE_DELAY_COUNT     20
            STATIC INT8 nDelayCount = 0;

            if(fEarJack)
            {
                nDelayCount++;
                if(nDelayCount > sMAX_EAR_JACK_SENSE_DELAY_COUNT)
                {
                    s_fJackSenseEnable = TRUE;
                }
            }
            else
            {
                nDelayCount = 0;
            }
        }

#ifdef CAMERA_JACKSENSE 
#define sMAX_EAR_CAMERA_DEALY	12	// b1=8
#endif

#ifdef CAMERA_JACKSENSE
		if(fCameraJack != s_fLastCameraJack)
		{
			nCameraDelayCount++;
			if(nCameraDelayCount > sMAX_EAR_CAMERA_DEALY)
			{	
				nCameraDelayCount = 0;
				s_fLastCameraJack = fCameraJack;

				fCheckAbnormalCamera = TRUE;
				nAbnormalCameraCount = 0;

				if(fCameraJack)
				{	// camerajack이면 야매hfk과의 구분을 위하여 3v_melody off함 
					// 3V_melody전원 off함 -> 일반 hfk인식을 위하여 강제로 off함 
					INTLOCK();
					BIO_OUT ( DEC_GPIO_0, BIO_AUDIO_EN_M, BIO_AUDIO_EN_OFF_V);
					fAudioPwrOff |= AUDIO_EN_SUPER_F;
					INTFREE();
				}
			}
		}
		else
		{
		  nCameraDelayCount = 0;
		}

	  if(fCheckAbnormalCamera == TRUE)
	  {
		if(fCameraJack)
		{
		  ++nAbnormalCameraCount;
		  if(nAbnormalCameraCount > sMAX_ABNORMAL_CAMERA_DEALY)
		  {
			 byte adc_test;
 		     adc_test= adc_read(ADC_CAMERA_SENSE);
		     #define	STR_HFK_THRES 0xfe	//길거리 HFK인식 레벨 
			 if(adc_test > STR_HFK_THRES)
			 {	// 야매 hfk
				fEarJack = TRUE;
				fAbnormalEarphone = TRUE;
				
				fCameraJack = FALSE;
				s_fLastCameraJack = fCameraJack;

				nAbnormalCameraCount=0;
  			    fCheckAbnormalCamera=FALSE;
			 }else{
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
			    /* Disable interrupts, protect l_KeyInputState */
			    INTLOCK_SAV( wInterruptState );     

			    SKY_TRACE1(TL_HIGH, "fCameraJack = %d", fCameraJack);

			     if(fCameraJack)
				 {  
				    sPassKeyCode( SE_KEY_CAMERA_JACK_ON );
				 }else 
				 { 
				    sPassKeyCode( SE_KEY_CAMERA_JACK_OFF );
				 } 

			     /* Restore interrupts */
			     INTFREE_SAV( wInterruptState ); 
            
			     /* No keys waited on, sleep now */
			     HS_SLEEP_ALLOW();               
    
			     fStateChanged = TRUE;
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
			     nAbnormalCameraCount=0;
			     fCheckAbnormalCamera=FALSE;
			 }   
			 // 3v_melody를 다시 원상 복구함
			 INTLOCK();
			 fAudioPwrOff &= ~AUDIO_EN_SUPER_F;
			 if(fAudioPwrOff){
				BIO_OUT ( DEC_GPIO_0, BIO_AUDIO_EN_M, BIO_AUDIO_EN_ON_V);
			 }
			 INTFREE();
		  }
		}//		if(fCameraJack)
		else
		{
  	      /* Disable interrupts, protect l_KeyInputState */
		  INTLOCK_SAV( wInterruptState );     
 	      SKY_TRACE1(TL_HIGH, "fCameraJack = %d", fCameraJack);
 	      if(fCameraJack)
		  {  
		    sPassKeyCode( SE_KEY_CAMERA_JACK_ON );
		  }else 
		  { 
		    sPassKeyCode( SE_KEY_CAMERA_JACK_OFF );
		  } 
 	      /* Restore interrupts */
		  INTFREE_SAV( wInterruptState ); 
          /* No keys waited on, sleep now */
		  HS_SLEEP_ALLOW();               
		  fStateChanged = TRUE;
		  nAbnormalCameraCount=0;
		  fCheckAbnormalCamera=FALSE;

		 // 3v_melody를 다시 원상 복구함
		 INTLOCK();
		 fAudioPwrOff &= ~AUDIO_EN_SUPER_F;
		 if(fAudioPwrOff){
			BIO_OUT ( DEC_GPIO_0, BIO_AUDIO_EN_M, BIO_AUDIO_EN_ON_V);
		 }
		 INTFREE();
		}
	  }//  if(fCheckAbnormalCamera == TRUE)
	  else{
		 nAbnormalCameraCount=0;
		 fCheckAbnormalCamera=FALSE;
	  }
		  
#endif //#ifdef CAMERA_JACKSENSE


        if(fEarJack != s_fLastEarJack)
        {
		 #ifdef CAMERA_JACKSENSE 
		  nEarMicDelayCount++;
		  if((nEarMicDelayCount > sMAX_EAR_CAMERA_DEALY) || (fAbnormalEarphone) )
		 #endif
		  {
		   #ifdef CAMERA_JACKSENSE 
			nEarMicDelayCount = 0;
		   #endif
			fAbnormalEarphone = FALSE;
            s_fLastEarJack = fEarJack;

            /* Disable interrupts, protect l_KeyInputState */
            INTLOCK_SAV( wInterruptState );     

            SKY_TRACE1(TL_HIGH, "fEarJack = %d", fEarJack);

			if(fEarJack)
			{
				EarJackOnOff = (SKY_EVENT_E) (SE_KEY_EAR_JACK_ON) ;
				// mod_en pin hi -> camera standby for B2 by jnlee 01.06.11
				// jacksense완료시 default hi됨 for B2 01.06.11
#ifndef WIN32
				BIO_OUT (DEC_GPIO_0, BIO_MOD_EN_M , BIO_MOD_EN_HI_V );	
#endif
			}
			else
			{
				EarJackOnOff = (SKY_EVENT_E) (SE_KEY_EAR_JACK_OFF);
				// mod_en pin low  for B2 by jnlee 01.06.11
				// jacksense off시  default low로 함  for B2 01.06.11
#ifndef WIN32
				BIO_OUT (DEC_GPIO_0, BIO_MOD_EN_M , BIO_MOD_EN_LO_V );	
#endif

			}

            sPassKeyCode( EarJackOnOff );

            /* Restore interrupts */
            INTFREE_SAV( wInterruptState ); 
            
            /* No keys waited on, sleep now */
            HS_SLEEP_ALLOW();               
        
            if( !fEarJack)
                s_fJackSenseEnable = FALSE;
    
            fStateChanged = TRUE;
		  }
        }
#ifdef CAMERA_JACKSENSE
		else
		{
		    nEarMicDelayCount = 0;
		}
#endif


        /* May occur abnormal glitch ... adding by ZZZ */
        if(s_fJackSenseEnable)
        {  
            sDetectEarJack();
        }

        /* Sample the state of the on-hook/hands-free sensor over the last
        ** sampling period, HS_GPIO_POLL_TIME.  This sensor is looking at the
        ** PCM data in line, seeing if there are any 0's.  Since there is a
        ** pullup resistor on the PCM data line, there will only be 1's if
        ** there is no Car Kit.  The latch is reset by Setting "Zero Detect"
        ** again.
        **
        ** The Ones Detector in the MSM2.2 will not work unless CHIPx8 is
        ** currently running.  In CDMA mode this does happen occasionally, but
        ** in FM mode we have to explictly request it from the rf_codec_on()
        ** routine.  Note however, we can NOT call this routine in CDMA mode
        ** since it might interfer with "sleep".  So, in FM mode only, we ask
        ** that CHIPx8 be turned on momentarily.  Note that this clock, runs
        ** MUCH MUCH faster than our CPU, so we don't have to wait long!
        ** 
        ** To avoid causing clicks in the earpiece, CHIPx8 cannot be switched
        ** on before the codec power supply returns to zero volts.
        ** HS_FM_ONES_PERIOD has been tuned relative to HS_GPIO_POLL_TIME to
        ** guarantee the proper delay time.
        */
        db_get (DB_RF_MODE,&RFValue);
        
        if ( RFValue.rf_mode == DB_RF_ANALOG ) 
        {
            /* Car Kits ALWAYS provide external power if they are on, so we
            ** use External Power as an extra latch in this decision to save
            ** power when the user just has the phone in his hand.  Additionally,
            ** we need this code to help up get OUT of the Car Kit, so I will
            ** continue to do it as long as I think I am still in the Car Kit.
            */
            if ( (fExtPower || s_fLastExtDevice) && (--s_nCHIPx8Count <= 0) ) 
            {
                s_nCHIPx8Count = HS_FM_ONES_PERIOD; /* Reset Period Counter */
                rf_codec_on(RF_CODEC_HS,TRUE);      /* Turn CHIPx8 On       */
                s_fHoldingCodec = TRUE;
                clk_busy_wait(RF_CODEC_WAIT);       /* Wait for Good CHIPx8 */
            }
        }
        else if ( s_fHoldingCodec ) 
        {
            /* Once we know we are in CDMA mode, we want to release the CODEC
            * if we held it on in previous AMPS mode. This allows SLEEP
            * to shut down CODEC. This code is called at least once before
            * the phone is going to sleep.
            */
            rf_codec_on(RF_CODEC_HS, FALSE);
            s_fHoldingCodec = FALSE;
        }

        /* Make sure ONES detector status is valid to read.
         * Also, read the ONES detector twice to determine real status 
         * in case it has just been reset by other task.
         */
        if ((RFValue.rf_mode != DB_RF_SLEEP) && (rex_self() == &hs_tcb)) 
        {
            /* Handles power up condition */
            if ( s_fFirstONESPoll )
            {
                s_fFirstONESPoll = FALSE;
                l_fFirstGPIOPoll = TRUE;
            }

            fONES = HS_ON_HOOK();

//		    voc_reset_ones_detect();

            /* Update the external device status only if we have two consistent 
            * ONES detector readings in a row.
            */
            if (fONES != s_fLastONES)
            {
                s_fLastONES = fONES;
                fExtDevice = s_fLastExtDevice;             
            }
            else
            {
                fExtDevice = fONES;
                fConsistentONES = TRUE;
            }
        }
        else
        {
            fExtDevice = s_fLastExtDevice;
        }

        if ((RFValue.rf_mode == DB_RF_ANALOG) && !fExtPower) 
        {
            /* Since this is a "voted" interface, we can ask that the clock be
            ** turned off, even if we haven't turned it on.
            ** However, we should keep chipx8 on if external power is present
            ** to avoid clicking sound.
            */
            rf_codec_on(RF_CODEC_HS,FALSE);
            s_fHoldingCodec = FALSE;
        }

        /* See if external device status changed */
        if ( fExtDevice != s_fLastExtDevice ) 
        {
#ifdef FEATURE_IDS
#error code not present
#endif
            //      hs_coil_cord_hfk = FALSE; /* Presume not a coil cord HFK */
            s_fLastExtDevice = fExtDevice;       /* Update the status */
            s_fAccessoryKeyRequired = TRUE;  /* Need to send accessory key  */
            HS_SLEEP_ALLOW();               /* Allow sleep */ 
            fStateChanged = TRUE;
        }

#ifdef FEATURE_IDS
#error code not present
#else
        if ( (l_fFirstGPIOPoll || s_fAccessoryKeyRequired) && fConsistentONES )
        {
			hs_pass_accessory_keys( fExtDevice, fExtPower );
            s_fAccessoryKeyRequired = FALSE;
        }
#endif
    }

    /* If the key buffer is not empty (e.g. Diag sent HS some keys),
    ** tell the caller so that we'll stay-awake/continue-processing.
    */
    if (l_KeyBuffer.iWrite != l_KeyBuffer.iRead) 
    {
        fStateChanged = TRUE;
    }

    return( fStateChanged );

} /* End of hs_poll_gpio */

#if MODEL_ID == MODEL_B2	// by asdf*^^* 01.07.27 for partial display
// hs_poll_gpio에서 처리하면 srch의 wakeup isr에서 실행될 경우가 있어서 reset발생
void hs_partial_disp(void)	
{
	// partial dispaly 상태로 들어갔다가 folder open 상태일때만  partial display를 빠져나옴  
	if(fFolderKeyChanged == 1 && IdleScreen == 1)	
	{	
		#ifndef WIN32
		BIO_OUT(ENC_GPIO_1, BIO_64KCOLOR_LCD_M, BIO_64KCOLOR_LCD_HI_V);	//by asdf*^^*2001.07.18 
		#endif
		send_ui_cmd(UI_END_PARTIAL_CMD);	//ui쪽에서 annunciator를 그린후 partial display off됨
		SKY_API SKY_PartialDisplay_OFF();
		//SKY_DrawAnnunciators();
		//SKY_UpdateScreen();
		IdleScreen=0;
	}
/*	else if (fFolderKeyChanged == 2)	//IDLE_SCREEN에서 실행 
	{
		SKY_API SKY_PartialDisplay_ON();
		#ifndef WIN32
		BIO_OUT(ENC_GPIO_1, BIO_64KCOLOR_LCD_M, BIO_64KCOLOR_LCD_LO_V);
		#endif
	}*/
	else
	{
		
	}
	fFolderKeyChanged=0;
}
#endif	//#if MODEL_ID == MODEL_B2




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
  The value of l_fHandsFreePinsEnabled is set to FALSE

===========================================================================*/
//void (*hs_disable_hfk_pins)(void) = sDisableHFKPins;
//void (*hs_enable_hfk_pins)(void) = sEnableHFKPins;

//LOCAL void sDisableHFKPins(void)
void hs_disable_hfk_pins(void)
{
    l_fHandsFreePinsEnabled = FALSE;

} /* end of hs_disable_hfk_pins */


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
//LOCAL void sEnableHFKPins(void)
void hs_enable_hfk_pins(void)
{
    l_fHandsFreePinsEnabled = TRUE;
  
} /* end of hs_enable_hfk_pins */


#endif  /* FEATURE_SDEVMAP_SHARE_HFK && FEATURE_SECOND_UART */

#define NOT_USED_TWO_COLOR  //white 2001.06.16

enum LED_COLOR_E {
    LC_BLUE  = 0,
    LC_GREEN ,
    LC_RED   ,
    LC_VIOLET,
    LC_BGREEN,
#ifndef NOT_USED_TWO_COLOR
    LC_ORANGE,
    LC_WHITE ,
#endif
    LC_NONE  
};
typedef enum LED_COLOR_E    LED_COLOR_E;

//LOCAL happykid 01.05.24 - 공정상 폴더가 열려도 LED를 켜야 하기 때문에 
//이함수를 외부에서도 쓸 수 있도록 하겠다.
 void SKY_API sTurnOffLED(void)
{
#if MODEL_ID == MODEL_B2
	//HS_SUB_BACKLIGHT_OFF();
	HS_CALL_LED_OFF();
#else
    HS_LED_RED_OFF();
    HS_LED_GREEN_OFF();
    HS_LED_BLUE_OFF();
#endif
}

//LOCAL happykid 01.05.24 - 공정상 폴더가 열려도 LED를 켜야 하기 때문에 
//이함수를 외부에서도 쓸 수 있도록 하겠다.
void SKY_API sTurnOnLED(LED_COLOR_E LEDColor)
{
    sTurnOffLED();

#if MODEL_ID == MODEL_B2
    UNUSED(LEDColor);

	//HS_SUB_BACKLIGHT_ON();
    HS_CALL_LED_ON();
#else	//#if MODEL_ID == MODEL_B2
    switch(LEDColor)
    {
    case LC_BLUE:
        HS_LED_BLUE_ON();
        break;

    case LC_GREEN:
        HS_LED_GREEN_ON();
        break;

    case LC_RED:
        HS_LED_RED_ON();
        break;

    case LC_VIOLET:
        HS_LED_RED_ON();
        HS_LED_BLUE_ON();
        break;

    case LC_BGREEN:
        HS_LED_GREEN_ON();
        HS_LED_BLUE_ON();
        break;

#ifndef NOT_USED_TWO_COLOR
    case LC_ORANGE:
        HS_LED_RED_ON();
        HS_LED_GREEN_ON();
        break;

    case LC_WHITE:
        HS_LED_RED_ON();
        HS_LED_GREEN_ON();
        HS_LED_BLUE_ON();
        break;
#endif
    case LC_NONE:
        break;
    }
#endif	//#if MODEL_ID == MODEL_B2
}

/*-----------------------------------------------------------------------
                        General Task Routines
-----------------------------------------------------------------------*/


/*===========================================================================

LOCAL FUNCTION HS_REQ

DESCRIPTION
  This procedure processes commands sitting in the command queue.

DEPENDENCIES
  hs_init() must have been called to initialize the queue, and this
  routine should not be called from an ISR.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void SKY_API sHandleCommand(void)
{
    HS_COMMAND_U    *pCommand;       /* Pointer to the current command */
    REX_TCB_T       *pTask;          /* Pointer to task to signal */
    REX_SIGNAL      Signal;          /* Signal for task */
//    WORD            wInterruptState; /* PSW parameter for INTLOCK/FREE_SAV */
    STATIC BOOL     s_fBacklight = FALSE; /* Backlight is currently on */
    STATIC BOOL     s_fSubBacklight = FALSE; /* Backlight is currently on */
	INT8 i,nLightOn=0,nLightOff=0;    
    //LOCAL boolean hs_backlight = FALSE;        

    /* 
    ** Clear the signal prior to emptying the queue, to avoid the
    ** race condition where an item is added just as we are leaving.
    */
    (void) SKY_Clear_Signal( &hs_tcb, HS_CMD_Q_SIG );

    /* Get a command from the command queue */
    while ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_q )) != NULL ) 
    {
        pTask  = pCommand->Header.pTask;   /* task pointer    */
        Signal = pCommand->Header.Signal;  /* signal for task */

        /* Decode/process the command as per the operation requested */
        switch ( pCommand->Header.Command) 
        {
        /* Turn on/off LCD screen */
        case HC_SCREEN :
            if(pCommand->Control.fOn)
                SKY_LCDOn();//LCDDisplayOn();
            else
                SKY_LCDOff();//LCDDisplayOff();
            break;
            
#ifdef FEATURE_SKY_SUBLCD
        /* Turn on/off sub LCD screen */
        case HC_SUB_SCREEN :
            if(pCommand->Control.fOn)
			{
                SKY_SubLCDOn();
			}
            else
			{
                SKY_SubLCDOff();
			}
            break;
#endif//#ifdef FEATURE_SKY_SUBLCD

        /* Turn on/off screen & keypad backlights */
        case HC_BACKLIGHT :
            s_fBacklight = pCommand->Control.fOn;
            SKY_TRACE1(TL_LOW, "Backlight set to %d", s_fBacklight);
            
            if ( s_fBacklight ) 
            {
                /* Turn on the backlight  */
		        HS_BACKLIGHT_ON();
			}
            else 
            {
                /* Turn off the backlight */
				HS_BACKLIGHT_OFF();
            }

			break;
        case HC_CONTRAST :
            SKY_SetLCDContrast((HS_CONTRAST_E)pCommand->Contrast.Contrast,
				(BYTE)pCommand->Contrast.ContrastLevel);
			//SKY_SetLCDContrastTest(pCommand->Contrast.ContrastLevel);
            break;

#ifdef FEATURE_SKY_SUBLCD
        /* Turn on/off sub screen backlights */
        case HC_SUB_BACKLIGHT :
            if(s_fBacklight && (pCommand->Control.fOn == TRUE))
                break;

            s_fSubBacklight = pCommand->Control.fOn;
            SKY_TRACE1(TL_LOW, "Sub backlight set to %d", s_fSubBacklight);
            
            if (s_fSubBacklight) 
            {
                /* Turn on the backlight  */
                //sTurnOnLED((LED_COLOR_E)pCommand->Control.Color);
                HS_SUB_BACKLIGHT_ON();
            } 
            else 
            {
                /* Turn off the backlight */
                //sTurnOffLED();
                HS_SUB_BACKLIGHT_OFF();
            }
            break;

        case HC_SUB_CONTRAST :
            SKY_SetSubLCDContrast((HS_CONTRAST_E)pCommand->Contrast.Contrast,
				(BYTE)pCommand->Contrast.ContrastLevel);
            break;

#endif//#ifdef FEATURE_SKY_SUBLCD

        /* Update the screen image */
        case HC_UPDATE_SCREEN :
            /* ... */
            SKY_UpdateScreen();
            //EQS_hs_update_screen();
            break;

#ifdef FEATURE_SKY_SUBLCD
        /* Update the sub screen image */
        case HC_UPDATE_SUB_SCREEN :
            /* ... */
            SKY_UpdateSubScreen();
            //EQS_hs_update_screen();
            break;
#endif//#ifdef FEATURE_SKY_SUBLCD

        /* Motor on */
        case HC_MOTOR_ON :  
		  #if MODEL_ID==MODEL_B1 || (MODEL_ID== MODEL_B2)	// by jnlee 01.04.23
			AUDIO_EN_ON();
			fAudioPwrOff |= AUDIO_EN_BY_VIB;
		  #endif//	#if MODEL_ID==MODEL_B1	// by jnlee 01.04.23
            HS_MOTOR_ON();
            break;

        /* Motor off */
        case HC_MOTOR_OFF :  
            HS_MOTOR_OFF();
		  #if MODEL_ID==MODEL_B1 || (MODEL_ID== MODEL_B2)	// by jnlee 01.04.23
			fAudioPwrOff &= ~AUDIO_EN_BY_VIB;
			AUDIO_EN_OFF();
		  #endif//	#if MODEL_ID==MODEL_B1	// by jnlee 01.04.23
            break;

        /* Simulate keypad input */
        case HC_KEY_INPUT :  
            /* pass the simulated keycode */
            sPassKeyCode(pCommand->KeyInput.KeyCode );
            break;

        case HC_SNI :
            sTurnSNIOnOff(pCommand->SNI.SNI, pCommand->SNI.OnOff);
            break;

        case HC_SUB_BACKLIGHT_CONFIRM:
            //sTurnOnLED((LED_COLOR_E)pCommand->Control.Color);
            HS_SUB_BACKLIGHT_ON();
            break;
#ifdef FEATURE_PARTIAL_DISPLAY
		case HC_UPDATE_PARTIAL_SCREEN:
			SKY_UpdatePartialScreen(pCommand->PartialScreen.Buffer,
									pCommand->PartialScreen.StartX,
									pCommand->PartialScreen.StartY,
									pCommand->PartialScreen.EndX,
									pCommand->PartialScreen.EndY);
			break;
		case HC_UPDATE_PARTIAL_DISPLAY_RECT:
			SKY_UpdatePartialDisplayRect(pCommand->PartialScreen.Buffer,
									     pCommand->PartialScreen.StartX,
									     pCommand->PartialScreen.StartY,
									     pCommand->PartialScreen.EndX,
									     pCommand->PartialScreen.EndY);
			break;
		case HC_UPDATE_PARTIAL_DISPLAY_RECT_BY_REGION:
			SKY_UpdatePartialDisplayRectByRegion(pCommand->PartialScreen.Buffer,
									     pCommand->PartialScreen.StartX,
									     pCommand->PartialScreen.StartY,
									     pCommand->PartialScreen.EndX,
									     pCommand->PartialScreen.EndY);
			break;
		case HC_UPDATE_PARTIAL_DISPLAY_LINE:
			SKY_UpdatePartialScreenLine(pCommand->PartialScreen.Buffer,
									     pCommand->PartialScreen.StartY,
									     pCommand->PartialScreen.EndY);
			break;
#endif
   		case HC_UPDATE_SCREEN_256C:
			SKY_256ColorDisplay((BYTE *)GetScreenBuffer());
			break;
		case HC_UPDATE_PARTIAL_DISPLAY_256C:
			SKY_256UpdatePartialDisplay(pCommand->PartialScreen.Buffer,
											pCommand->PartialScreen.StartY,
											pCommand->PartialScreen.EndY);
			break;

/*		case HC_IDLE_SCREEN :
			//by asdf*^^* 01.09.04
			//folder가 닫힐때 무조건 partial dispaly를 하면 partial display 에서 
			//빠져나올때 실행되는 SKY_UpdateScreen 때문에 
			//(aplet에서 aplet으로 넘어가는 사이의 클리어 버퍼때문에)
			//화면이 하얗게 보이는 현상이 있어서 
			//idle screen에서 폴더가 닫힐때만 partial dispaly하고 partial display상태에서 
			//폴더가 열릴때만 partial display를 빠져나오도록 수정
			
			 fPhoneOpen = HS_EAR_UP();
			 fFolderKeyChanged = (fPhoneOpen) ? 1 : 2 ;
			 if (fFolderKeyChanged == 2)	// folder close
				{
					SKY_UpdateScreen();	
					SKY_API SKY_PartialDisplay_ON();
					#ifndef WIN32
					BIO_OUT(ENC_GPIO_1, BIO_64KCOLOR_LCD_M, BIO_64KCOLOR_LCD_LO_V);
					#endif
					IdleScreen = 1;
				}
            break;
*/
		case HS_PARTIAL_DIS_ON: 
				fPhoneOpen = HS_EAR_UP();
				fFolderKeyChanged = (fPhoneOpen) ? 1 : 2 ;
					 
				if (fFolderKeyChanged == 2)	// folder close
					{
						SKY_UpdateScreen();	
						SKY_API SKY_PartialDisplay_ON();
						#ifndef WIN32
						BIO_OUT(ENC_GPIO_1, BIO_64KCOLOR_LCD_M, BIO_64KCOLOR_LCD_LO_V);
						#endif
						IdleScreen = 1;
						PartialDisplayOn = 0;
					}
			 break;

        default:
            SKY_ERROR_FATAL1("Invalid command: %d", pCommand->Header.Command);
            /* does not return */
        }

        /* Save the tcb/sig of the commander, so that we can signal that task
        ** after we've freed the command structure.  If there is a 'done queue',
        ** queue the command into that queue.
        */
        if (pCommand->Header.pDone) 
        {
            q_put(pCommand->Header.pDone, &(pCommand->Header.Link) );
        }

        /* If there's a task to signal, signal it with the saved tcb/sig */
        if(pTask)
        {
            (void) SKY_Set_Signal( pTask, Signal);
        }

        /* Report to the watchdog, we may be in this loop a long time */
        dog_report(DOG_HS_RPT);
    } /* while */
} /* end of hs_req */

/*
** ***************************************************************************
** SNI
** ***************************************************************************
*/
LOCAL HS_SNI_E  l_CurrentSNI = HS_SNI_NONE;
LOCAL INT8      l_nSNICount = 0;

LOCAL void SKY_API sSetSNIBlinkTimer(DWORD dwInterval)
{
    SKY_Set_Timer(&l_SNITimer, dwInterval);
    /*
    if(dwInterval == 0)
    {
        rex_clr_timer(&l_SNITimer);
    }
    else
    {
        rex_set_timer(&l_SNITimer, dwInterval);
    }
    */
}

LOCAL void SKY_API sSNITimerCallback(void)
{
    CNI_T*   pCallerID;

    ROM WORD wSNIInterval[] = {
        60, //40,    // HS_SNI_CALL_ALERT
        500,   // HS_SNI_SMS_RECEIVING
        2000,  // HS_SNI_CALL_CONNECT
        500,   // HS_SNI_GENERAL_ALARM
        1000,  // HS_SNI_UNREAD_SMS
        500,   // HS_SNI_TRANS_IRDA
        500,   // HS_SNI_TRANS_DMANGER
        5000  // HS_SNI_POWER_ON
//        1000,  // HS_SNI_ERR_CHARGING
//        3000,  // HS_SNI_DONE_CHARGING
//        3000   // HS_SNI_ON_CHARGING
    };

    pCallerID   =  SKY_GetCallerID();

    if(l_CurrentSNI > HS_SNI_NONE && l_CurrentSNI < HS_SNI_MAX_SNI)
        sSetSNIBlinkTimer(wSNIInterval[l_CurrentSNI-1]);
    else
        sSetSNIBlinkTimer(0);

    switch(l_CurrentSNI)
    {
    case HS_SNI_CALL_ALERT:
        if(pCallerID->GroupBacklight >= BLUE_LCD_COLOR && pCallerID->GroupBacklight <= BLUE_GREEN_LCD_COLOR)// WHITE_LCD_COLOR)
        {
            sTurnOnLED((LED_COLOR_E)pCallerID->GroupBacklight);
        }
        else if(pCallerID->GroupBacklight == RAINBOW_LCD_COLOR)
        {
            switch(l_nSNICount)
            {
            case 0:
                sTurnOnLED(LC_BLUE);
                SKY_SetupBacklight();
                l_nSNICount++;
                break;

            case 1:
                sTurnOnLED(LC_GREEN);
                l_nSNICount++;
                break;

            case 2:
                sTurnOnLED(LC_RED);
                l_nSNICount++;
                break;

            case 3:
                sTurnOnLED(LC_VIOLET);
                l_nSNICount++;
                break;

            case 4:
                sTurnOnLED(LC_BGREEN);
#ifdef NOT_USED_TWO_COLOR
                HS_BACKLIGHT_OFF(); 
                //HS_SUB_BACKLIGHT_OFF();
                sTurnOffLED();
                l_nSNICount = 0;
#else
                l_nSNICount++;
#endif
                break;

#ifndef NOT_USED_TWO_COLOR
            case 5:
                sTurnOnLED(LC_ORANGE);
                //l_nSNICount++;
                HS_BACKLIGHT_OFF();    /* Turn on the backlight  */
#ifdef FEATURE_SKY_SUBLCD
                //HS_SUB_BACKLIGHT_OFF();
                sTurnOffLED();
#endif /* #ifdef FEATURE_SKY_SUBLCD */
                l_nSNICount = 0;
                break;

#if 0
            case 6:
                sTurnOnLED(LC_WHITE);
                HS_BACKLIGHT_OFF();    /* Turn on the backlight  */
#ifdef FEATURE_SKY_SUBLCD
                //HS_SUB_BACKLIGHT_OFF();
                sTurnOffLED();
#endif /* #ifdef FEATURE_SKY_SUBLCD */
                l_nSNICount = 0;
                break;
#endif
#endif
            }
        }
        else //if(pCallerID->GroupBacklight == NONE_GROUP_ALERT_LCD)
        { 
            if(SKY_GET_IDLCD_COLOR_ALERT() == RAINBOW_LCD_COLOR)
            {
                switch(l_nSNICount)
                {
                case 0:
                    sTurnOnLED(LC_BLUE);
                    SKY_SetupBacklight();
                    l_nSNICount++;
                    break;

                case 1:
                    sTurnOnLED(LC_GREEN);
                    l_nSNICount++;
                    break;

                case 2:
                    sTurnOnLED(LC_RED);
                    l_nSNICount++;
                    break;

                case 3:
                    sTurnOnLED(LC_VIOLET);
                    l_nSNICount++;
                    break;

                case 4:
                    sTurnOnLED(LC_BGREEN);
#ifdef NOT_USED_TWO_COLOR
                HS_BACKLIGHT_OFF(); 
                //HS_SUB_BACKLIGHT_OFF();
                sTurnOffLED();
                l_nSNICount = 0;
#else
                    l_nSNICount++;
#endif
                    break;

#ifndef NOT_USED_TWO_COLOR
                case 5:
                    sTurnOnLED(LC_ORANGE);
                    //l_nSNICount++;
                    HS_BACKLIGHT_OFF();    /* Turn on the backlight  */
#ifdef FEATURE_SKY_SUBLCD
                    //HS_SUB_BACKLIGHT_OFF();
                    sTurnOffLED();
#endif /* #ifdef FEATURE_SKY_SUBLCD */
                    l_nSNICount = 0;
                    break;
#if 0
                case 6:
                    sTurnOnLED(LC_WHITE);
                    //HS_SUB_BACKLIGHT_OFF();
                    sTurnOffLED();
#ifdef FEATURE_SKY_SUBLCD
                    HS_SUB_BACKLIGHT_OFF();
#endif /* #ifdef FEATURE_SKY_SUBLCD */
                    l_nSNICount = 0;
                    break;
#endif
#endif
              /*case 2:
                    HS_LED_RED_OFF();//HS_CALL_LED_ON();
                    HS_LED_GREEN_OFF();
                    HS_LED_BLUE_ON();

                    l_nSNICount++;
                    break;*/
                }
            }else
            {
                sTurnOnLED((LED_COLOR_E)(SKY_GET_IDLCD_COLOR_ALERT()));
                HS_SUB_BACKLIGHT_ON();
            }
        }
        break;

    case HS_SNI_SMS_RECEIVING:
        l_nSNICount++;
        //
        sTurnOnLED((LED_COLOR_E)SKY_GET_IDLCD_COLOR_SMS());
        HS_SUB_BACKLIGHT_ON();
        /*
        if(l_nSNICount == 1 || l_nSNICount == 3)
        {
            sTurnOnLED(LC_GREEN);
        }
        else if(l_nSNICount == 2)
        {
            sTurnOffLED();
        } 
        else 
        */
        if(l_nSNICount >= 4)
        {
            sTurnSNIOnOff(HS_SNI_SMS_RECEIVING, OFF);
            HS_SUB_BACKLIGHT_OFF();
        }
        break;

    case HS_SNI_CALL_CONNECT:
        l_nSNICount++;
        sTurnOnLED(LC_RED);
        //HS_SUB_BACKLIGHT_ON();

        if(l_nSNICount >= 2)
        {
            sTurnSNIOnOff(HS_SNI_CALL_CONNECT, OFF);
            //HS_SUB_BACKLIGHT_OFF();
        }
        break;

    case HS_SNI_GENERAL_ALARM:
        if(l_nSNICount)
        {
            sTurnOffLED();
            HS_SUB_BACKLIGHT_OFF();

            l_nSNICount = 0;
        }
        else
        {
            sTurnOnLED((LED_COLOR_E)SKY_GET_IDLCD_COLOR_ALARM());//LC_RED | LC_GREEN);
            HS_SUB_BACKLIGHT_ON();
            
            l_nSNICount = 1;
        }
        break;

    case HS_SNI_UNREAD_SMS:
        l_nSNICount++;
        sTurnOnLED((LED_COLOR_E)SKY_GET_IDLCD_COLOR_SMS());

        if(l_nSNICount >= 2)
        {
            sTurnSNIOnOff(HS_SNI_UNREAD_SMS, OFF);
        }
        break;

    case HS_SNI_TRANS_IRDA:
        l_nSNICount++;
        sTurnOnLED(LC_RED);
        HS_SUB_BACKLIGHT_ON();

        if(l_nSNICount >= 2)
        {
            sTurnSNIOnOff(HS_SNI_TRANS_IRDA, OFF);
            HS_SUB_BACKLIGHT_OFF();
        }
        break;

    case HS_SNI_TRANS_DMANGER:
        if(l_nSNICount)
        {
            sTurnOffLED();
            HS_SUB_BACKLIGHT_OFF();

            l_nSNICount = 0;
        }
        else
        {
            sTurnOnLED(LC_GREEN);
            HS_SUB_BACKLIGHT_ON();

            l_nSNICount = 1;
        }
        break;

    case HS_SNI_POWER_ON:
        l_nSNICount++;
        sTurnOnLED(LC_RED);
        HS_SUB_BACKLIGHT_ON();

        if(l_nSNICount >= 2)
        {
            sTurnSNIOnOff(HS_SNI_POWER_ON, OFF);
            HS_SUB_BACKLIGHT_OFF();
        }
        break;
/*
    case HS_SNI_ERR_CHARGING:
        if(l_nSNICount)
        {
            HS_CALL_LED_OFF();
            l_nSNICount = 0;
        }
        else
        {
            HS_CALL_LED_ON();
            
            l_nSNICount = 1;
        }
        break;

    case HS_SNI_DONE_CHARGING:
        if(l_nSNICount)
        {
            HS_CALL_LED0_OFF();    // Red on
            HS_CALL_LED1_ON();    // Green on
            l_nSNICount = 0;
        }
        else
        {
            HS_CALL_LED0_OFF();    // Red on
            HS_CALL_LED1_ON();    // Green on
            
            l_nSNICount = 1;
        }
        break;

    case HS_SNI_ON_CHARGING:
        if(l_nSNICount)
        {
            HS_CALL_LED0_ON();    // Red on
            l_nSNICount = 0;
        }
        else
        {
            HS_CALL_LED0_ON();    // Red on
            
            l_nSNICount = 1;
        }
        break;
*/
    default:
        break;

    }
}

LOCAL void SKY_API sSNIOn(HS_SNI_E SNI)
{
    if (l_CurrentSNI != SNI) 
    {
        l_CurrentSNI = SNI;
        l_nSNICount = 0;
    }
    sSNITimerCallback();
}

LOCAL void SKY_API sSNIOff(void)
{
    if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_ALWAYS)        
    {
        if(!SKY_IS_PHONE_OPEN())
            SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
    }
    else if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME)
    {
        if(SKY_IsSubBacklightOnTime())
        { 
            if(!SKY_IS_PHONE_OPEN())
                SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
        }
    }

    HS_CALL_LED_OFF(); /* turn off always CALL LED */
    sSetSNIBlinkTimer((int4)0);
}

LOCAL void SKY_API sTurnSNIOnOff(HS_SNI_E SNI, ONOFF OnOff)
{
    STATIC ONOFF s_afSNIOnOff[] = { 
	    OFF, OFF, OFF, OFF, OFF, OFF, 
        OFF, OFF, OFF, OFF, OFF, OFF 
    };
    BOOL fAct;
    INT8 i;

    fAct = FALSE;

    if(OnOff) 
    {
        s_afSNIOnOff[SNI] = TRUE;    // Setting
        for(i = 1; i < SNI; i++) 
        {
            if( s_afSNIOnOff[i] == TRUE ) 
            {  // exist Priority high
                fAct = TRUE;
                break;
            }
        }

        if( !fAct) 
        {                    // do not exist Priority high
            sSNIOff();             // DO SNI OFF
            sSNIOn(SNI);      // DO SNI ON
        }
    } 
    else 
    {
        s_afSNIOnOff[SNI] = FALSE;   // Setting
        for( i=1; i < SNI; i++) 
        {
            if( s_afSNIOnOff[i] == TRUE ) 
            {
                fAct = TRUE;
                break;
            }
        }

        if( !fAct ) 
        {
            sSNIOff();              // DO SNI OFF
            l_CurrentSNI = HS_SNI_NONE;
            for( i=SNI + 1; i < HS_SNI_MAX_SNI; i++) 
            {
                if( s_afSNIOnOff[i] == TRUE ) 
                {
                    sSNIOn((HS_SNI_E)(i));  // DO SNI ON
                    break;
                }
            } /* for */
        } /* if */
    } /* else */
}

/*===========================================================================

LOCAL FUNCTION HS_INIT

DESCRIPTION
  This procedure initializes the handset and handles startup handshaking
  with the Main Control Task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Initialization causes the display to be cleared and the backlight turned on

===========================================================================*/
void SKY_API InitHS(void)
{
    int i;                       /* Index count           */
    //nv_cmd_type    nv_cmd_buf;   /* Command buffer to NV  */
    //nv_item_type   nv_lcd_cal;   /* NV data buffer        */

    /* Initialize timers - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    rex_def_timer(&l_RepeatTimer,   &hs_tcb, HS_RPT_TIMER_SIG); /* Dog report */
    rex_def_timer(&l_GPIOTimer,  &hs_tcb, HS_GPIO_TIMER_SIG);   /* GPIO poll  */
    rex_def_timer(&l_SNITimer, &hs_tcb, HS_SNI_BLINK_SIG);      /* Blink      */
    /* Initialize command and "free" queues - - - - - - - - - - - - - - - - - -*/

    (void) q_init( &hs_cmd_q );
    (void) q_init( &hs_cmd_free_q );

    /* Place "free" commands on hs_cmd_free_q
    */
    for (i = 0; i < HS_NUM_CMDS; i++ ) {
        //hs_cmd_bufs[i].hdr.done_q_ptr = &hs_cmd_free_q;
        hs_cmd_bufs[i].Header.pDone = &hs_cmd_free_q;
        q_put( &hs_cmd_free_q,
//                          q_link( &hs_cmd_bufs[i], &hs_cmd_bufs[i].hdr.link ) );
                          q_link( &hs_cmd_bufs[i], &hs_cmd_bufs[i].Header.Link ) );
    }

    /* Initialize the LCD controller - - - - - - - - - - - - - - - - - - - - - */
#ifdef SW_RESET_ENABLE
	if(!mc_bSwReset)
	{
#endif
    SKY_InitLCD();//InitialLCD();
#ifdef FEATURE_SKY_SUBLCD
    SKY_InitSubLCD();
#endif /* #ifdef FEATURE_SKY_SUBLCD */
#ifdef SW_RESET_ENABLE
    }
#endif
    /* Initialize the GPIO outputs - - - - - - - - - - - - - - - - - - - - - - */

//    HS_LED_INIT();			/* Call LED control GPIO  */
    HS_BACKLIGHT_INIT();	/* Backlight control GPIO */

#if MODEL_ID == MODEL_B1
	HS_LCD_BACKLIGHT_INIT();
#endif

    // girin. 2000.12.31
    // Display in a hurry
    // Boot logo image, backlight and call led on in a hurry
    //
    // Dependencies: InitLCD(), HS_BACKLIGHT_INIT(), HS_CALL_LED_INIT().
    // 
    SKY_LCDOn();
#ifdef SW_RESET_ENABLE
	if(!mc_bSwReset)
	{
#endif
    HS_BACKLIGHT_ON();

//    HS_LCD_BACKLIGHT_ON();

    SKY_BootLogoDisplay();
    // girin. end.

#ifdef FEATURE_SKY_SUBLCD
    HS_SUB_BACKLIGHT_INIT();          /* Backlight control GPIO */
    HS_SUB_BACKLIGHT_ON();
#endif /* #ifdef FEATURE_SKY_SUBLCD */
    HS_CALL_LED_ON();


#ifdef SW_RESET_ENABLE
    }
#endif

    HS_KEYPAD_INIT();		/* Keypad scan outputs    */

	HS_MEMO_INIT();			// memo key init
	HS_MOTOR_INIT();		/* Motor				  */

    HS_EAR_INIT();
    HS_JACK_SENSE_INIT();

#if MODEL_ID == MODEL_B1 || (MODEL_ID== MODEL_B2)
	HS_ON_HOOK_INIT();
#else
	BIO_TRISTATE(DEC_GPIO_1, BIO_CRDL_M ,   BIO_OUTPUT_DIS_V);/* temperory */
#endif
    BIO_TRISTATE(DEC_GPIO_1, BIO_RI_O_25_M, BIO_OUTPUT_ENA_V);/* temperory */
    BIO_TRISTATE(ENC_GPIO_0, BIO_EXT1_M,    BIO_OUTPUT_DIS_V);/* temperory */    

#if MODEL_ID == MODEL_B1		// camera CPLD용 mod_en high output!
    BIO_TRISTATE(DEC_GPIO_0,BIO_MOD_EN_M, BIO_MOD_EN_HI_V);
	BIO_OUT (DEC_GPIO_0,BIO_MOD_EN_M, BIO_MOD_EN_HI_V);
#elif MODEL_ID == MODEL_B2
    BIO_TRISTATE(DEC_GPIO_0,BIO_MOD_EN_M, BIO_MOD_EN_HI_V);	//by jnlee for B2 01.06.11
	BIO_OUT (DEC_GPIO_0,BIO_MOD_EN_M, BIO_MOD_EN_LO_V);		//default LOW for B2
#endif


    /* Power-up keys for UI - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Set the ones detect latch to detect 0's on PCM the Data-In line, then
    * delay for HS_ONES_DET_DELAY.  
    */
    VC_OUTM( VC_ONES_POLARITY, VC_ONES_POLARITY_M, VC_DETECT_ZEROES_V );
    clk_busy_wait( HS_ONES_DET_DELAY );

    /* Sample the state of the earpiece switch and pass it on.
    */
    l_fPhoneOpen = HS_EAR_UP();
    sPassKeyCode((SKY_EVENT_E) ((l_fPhoneOpen) ? SE_KEY_PHONE_OPEN : SE_KEY_PHONE_CLOSE) );
    sPassKeyCode( SE_KEY_RELEASE );

    /* Sample the state of the ignition switch and pass it on.
    ** If we are not in the hands-free adapter, ignore the ignition sensor.
    */
    if ( !HS_EXT_PWR_ON() )
    {
        sPassKeyCode(SE_KEY_EXT_POWER_OFF);
        sPassKeyCode(SE_KEY_RELEASE);
    }

    /* Sample the state of the power key and pass it on. */

    if ( BIO_INM( DMOD_KEY_READ, BIO_ON_SW_SENSE_M ) == BIO_ON_SW_ON_V ) 
    {
        //l_LastKeyCode = SE_KEY_POWER;  /* Last key is power key   */
        l_LastKeyCode = SE_KEY_END;
        l_KeyInputState = KIS_UP_WAIT; /* Waiting for key release */
        //sPassKeyCode(SE_KEY_POWER);    /* Queue key for receiver  */
        sPassKeyCode(SE_KEY_END);
    }
    else 
    {
        /* No keys waited on, sleep now */
        HS_SLEEP_ALLOW();
    }

    /* Register the keypad scanning routine with the Clock services,
    ** so that it is called at regular intervals to poll the keypad.
    */
    clk_def(&l_KeyPadCAllback); /* Init call-back */
    clk_reg(&l_KeyPadCAllback, sReadKeyCode, 25L, 25L, TRUE );/* Register */

    /* Synchronize task start-up with the Main Control Task - - - - - - - - - -*/

    /* ACK task creation and wait for start signal from the Main Control Task
    */
    mc_task_start( HS_RPT_TIMER_SIG, DOG_HS_RPT, &l_RepeatTimer );

    /* Clear start signal
    */
    (void) SKY_Clear_Signal( &hs_tcb, TASK_START_SIG );

#if 0 /* 0000000000000000000000000000000000000000000000000000000000000000000 */

    /* E F S version --- */

    /* Calibrate the LCD from data stored in the configuration file */
    //SKY_SetLCDContrast(SKY_GET_CONTRAST());

    /* Power supply control */
    //hs_lcd_out( HS_LCD_POW_SUPPLY( ( (nv_lcd_cal.lcd >> 4) & 0x0f) ) );
    /* Contrast control */
    //hs_lcd_out( HS_LCD_CONTRAST( (nv_lcd_cal.lcd & 0x0f) ) );

    /* N V version --- */

    /* Calibrate the LCD from data stored in NV RAM - - - - - - - - - - - - - -*/
    /* Prepare command buffer to NV
    */
    nv_cmd_buf.cmd = NV_READ_F;                 /* Command is Read       */
    nv_cmd_buf.tcb_ptr = &hs_tcb;               /* Signal HS when done   */
    nv_cmd_buf.sigs = HS_NV_DONE_SIG;           /* With this signal      */
    nv_cmd_buf.done_q_ptr = NULL;               /* Do not re-queue       */
    nv_cmd_buf.item = NV_LCD_I;                 /* Get LCD cal value     */
    nv_cmd_buf.data_ptr = &nv_lcd_cal;

    /* Clear signal, issue the command, and wait for the respone.
    */

    (void) SKY_Clear_Signal(&hs_tcb, HS_NV_DONE_SIG); /* Clear signal for NV */
    nv_cmd(&nv_cmd_buf);                          /* Issue the request   */

    /* While waiting, report to the watchdog periodically
    */
    while ( rex_timed_wait( HS_RPT_TIMER_SIG | HS_NV_DONE_SIG, &l_RepeatTimer,
                          DOG_HS_RPT_TIME  ) != HS_NV_DONE_SIG ) {
        dog_report(DOG_HS_RPT);
    }

    (void) SKY_Clear_Signal(&hs_tcb, HS_NV_DONE_SIG); /* Clear signal from NV  */

    /* Check to see if calibration data is available.  If it is, use it. */
    if (nv_cmd_buf.status == NV_DONE_S) 
    {

        /*-----------------------------------
          NV_LCD_I 값으로 LCD Contrast 조정.
        ------------------------------------*/
        //SKY_SetLCDContrast(nv_lcd_cal.lcd);
        // nv에서 읽은 LCD Contrast 값을 저장함.

        /* Power supply control
        */
        //hs_lcd_out( HS_LCD_POW_SUPPLY( ( (nv_lcd_cal.lcd >> 4) & 0x0f) ) );

        /* Contrast control
        */
        //hs_lcd_out( HS_LCD_CONTRAST( (nv_lcd_cal.lcd & 0x0f) ) );

    }
#endif /* 0000000000000000000000000000000000000000000000000000000000000000000 */


} /* end of hs_init */

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
void hs_task(DWORD dwDummy)
{
    REX_SIGNAL Signal;           /* our task's signals */

    UNUSED(dwDummy);    /* parameter from REX -- ignored */

    /* 
    ** Init the handset and wait for a startup message 
    ** from Main Control task
    */
    SKY_InitHS();

    /* 
    ** Set the watchdog report timer signal so that we'll
    ** reset the watchdog as soon as we start checking for events
    */
    (void) SKY_Set_Signal( &hs_tcb, HS_RPT_TIMER_SIG );

    /* Setup the timer for the GPIO polling */
    (void) SKY_Set_Timer( &l_GPIOTimer, HS_GPIO_POLL_TIME );

    /* Loop forever */
    while(TRUE)
    {

        /* Wait for something to happen */
        Signal = SKY_Wait_Signal( HS_RPT_TIMER_SIG |   /* Watchdog report timer    */
                         HS_CMD_Q_SIG |       /* Command Queue signal     */
                         HS_GPIO_TIMER_SIG |  /* GPIO 'key' poll          */
                         HS_SNI_BLINK_SIG |   /* SNI blink signal      */
                         TASK_OFFLINE_SIG |   /* Command to go offline    */
                         TASK_STOP_SIG );     /* Command to shutdown      */

        /* If watchdog report timer expired, report and restart the timer */
        if ( Signal & HS_RPT_TIMER_SIG ) 
        {
            dog_report(DOG_HS_RPT);
            (void) SKY_Set_Timer( &l_RepeatTimer, DOG_HS_RPT_TIME );
        }

        /* Handle the stop signal, acknowledge and continue */
        if ( Signal & TASK_STOP_SIG ) 
        {
            (void) SKY_Set_Signal( &mc_tcb, MC_ACK_SIG );      /* Ack the stop     */
            (void) SKY_Clear_Signal( &hs_tcb, TASK_STOP_SIG);    /* Clear the signal */
        }

        /* Handle the offline signal, acknowledge and continue */
        if ( (Signal & TASK_OFFLINE_SIG) != 0 ) 
        {
          (void) SKY_Set_Signal( &mc_tcb, MC_ACK_SIG );      /* Ack the stop     */
          (void) SKY_Clear_Signal( &hs_tcb, TASK_OFFLINE_SIG); /* Clear the signal */
        }

        /* Handle the SNI blink signal */
        if ( Signal & HS_SNI_BLINK_SIG ) 
        {
            sSNITimerCallback();
            (void) SKY_Clear_Signal( &hs_tcb, HS_SNI_BLINK_SIG); /* Clear the signal */
        }

        /* If we have commands to process, do so.  We may poll the handset,
        ** which would clear the signal HS_POLL_TIMER_SIG.
        */
        if ( Signal & HS_CMD_Q_SIG ) {
            sHandleCommand();
        }

        /* Poll the keys which are signals, and not actually in the handset
        */
        if ( (Signal & HS_GPIO_TIMER_SIG) != 0 ) {
            (void) hs_poll_gpio();//sPollGPIO();
#if MODEL_ID == MODEL_B2
			(void) hs_partial_disp();	// by asdf*^^* 01.07.27
			{
			// by jnlee 01.08.21
			/*	주기적으로 여기서 therm과 vbatt을 읽도록 하고, 
				therm_read, vbatt_Read에서는 여기서 읽을 값을 참조하도록 수정함 */
				extern void vbatt_temp_read(void);
				vbatt_temp_read();
			}
#endif
            (void) SKY_Set_Timer( &l_GPIOTimer, HS_GPIO_POLL_TIME );
        }
    } /* end of infinite loop */
} /* end of hs_task */


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
void hs_cmd
(
    HS_COMMAND_U *pCommand
    /* pointer to a handset command.  When the command has finished, the
    ** command structure is put on the "done queue".  If there is no "done
    ** queue", or this operation is not desirable, set the "done queue"
    ** pointer to NULL.  Also when the command has completed, the specified
    ** task is signaled with the sig parameter.  If no signaling is required,
    ** the task pointer parameter can be set to NULL.
    */
)
{
    (void) q_link(pCommand, &pCommand->Header.Link );  /* Initialize link field */

    q_put( &hs_cmd_q, &pCommand->Header.Link );       /* Put on command queue */
    (void) SKY_Set_Signal( &hs_tcb, HS_CMD_Q_SIG ); /* Signal a queue event */
} /* end of hs_cmd */

void  WIN_ProcessKey(long KeyCode)
{
    sPassKeyCode((SKY_EVENT_E) KeyCode);
}


