/*
** ============================================================================
**
** Project: 
**     BUNNY
**
** File:
**     sky_hs.h
**
** Description:
**     This file contains the data structures used to interface to the Handset
**     Task for the various DMSS phones.
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

#ifndef _SKY_HS_H_
#define _SKY_HS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
//#if (TG == T_G)
//#if defined (FEATURE_FFA)
//#error code not present
//#else
//#include "hsig.h"
//#endif /*  FEATURE_FFA */
//#endif  /*  (TG == T_G) */

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#ifdef WIN32
#define RD_MODE 0
//#define WR_MODE 0
#define MC_RE_DETERMIN_F 0
#endif
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
**  Commands and Command Header
*/

/* Handset Operations which can be performed/requested.  Note that not all
** targets support all operations.  For backwards compatability, we never
** delete items from this table, so some of these operations do not apply
** to newer phones and visa versa.
*/
enum tagHS_COMMAND_E
{
    HC_SCREEN,        /* Turn on/off the LCD screen                        */
    HC_BACKLIGHT,     /* Turn on/off the screen & keypad backlights        */
    HC_KEY_INPUT,     /* Simulate keypad input                             */
    HC_UPDATE_SCREEN, /* Update full screen                                */
    HC_CONTRAST,            /* adjust contrast */
#ifdef FEATURE_SKY_SUBLCD
    HC_SUB_SCREEN,          /* turn on/off the sub LCD screen */
    HC_SUB_BACKLIGHT,       /* turn on/off sub screen backlights */
    HC_UPDATE_SUB_SCREEN,   /* update sub screen */
    HC_SUB_CONTRAST,            /* adjust contrast */
#endif /* FEATURE_SKY_SUBLCD */
    HC_MOTOR_ON,            /* motor start */
    HC_MOTOR_OFF,           /* motor end */
    HC_SNI,                 /* SNI on/off */
#ifdef FEATURE_SKY_SUBLCD
    HC_SUB_BACKLIGHT_CONFIRM,
#endif
#ifdef FEATURE_PARTIAL_DISPLAY
	HC_UPDATE_PARTIAL_SCREEN,
	HC_UPDATE_PARTIAL_DISPLAY_RECT,
	HC_UPDATE_PARTIAL_DISPLAY_RECT_BY_REGION,
	HC_UPDATE_PARTIAL_DISPLAY_LINE,
#endif
	HC_UPDATE_SCREEN_256C,
	HC_UPDATE_PARTIAL_DISPLAY_256C,
    /* 
    ** jrkwon : 2001.09.03
    ** idle 상태 화면 표시가 완료되었음 --> folder 닫혔을 때 화면 부분 표시 시작 시점으로 사용 
    */
    HC_IDLE_SCREEN,
	HS_PARTIAL_DIS_ON,
    HC_MAX
};
typedef enum tagHS_COMMAND_E    HS_COMMAND_E;

/* Command header common to all command packets */
struct tagHS_COMMAND_HEADER_T
{
    QUEUE_LINK_T    Link;    /* Queue link                            */
    QUEUE_T         *pDone;  /* Queue to put 'this' on when done      */
    REX_TCB_T       *pTask;  /* Task to signal when done              */
    REX_SIGNAL      Signal;  /* Signals to signal *task_ptr when done */
    HS_COMMAND_E    Command; /* Handset Command                       */
};
typedef struct tagHS_COMMAND_HEADER_T   HS_COMMAND_HEADER_T;

/* 
** Screen/Backlight on/off command packet for HC_SCREEN_CONTROL, 
** HC_BACKLIGHT
*/
struct tagHS_CONTROL_T
{
    HS_COMMAND_HEADER_T     Header;     /* Command header */
    BOOL                    fOn;        /* on/off control */
	BOOL                    SensorOn;
#ifdef FEATURE_COLOR_ID_LCD
    ID_LCD_COLOR_E          Color;
#endif  //FEATURE_COLOR_ID_LCD
};
typedef struct tagHS_CONTROL_T     HS_CONTROL_T;

/*
** Diagnostic input/output/control
*/
struct tagHS_KEY_INPUT_T
{
    HS_COMMAND_HEADER_T     Header;     /* Command header */
    SKY_EVENT_E             KeyCode;    /* Keycode input */
    INT8                    nFreeKeyBufferSlots; 
};
typedef struct tagHS_KEY_INPUT_T   HS_KEY_INPUT_T;


enum tagHS_SNI_E
{
    HS_SNI_NONE = 0,
    HS_SNI_CALL_ALERT,
    HS_SNI_SMS_RECEIVING,
    HS_SNI_CALL_CONNECT,
    HS_SNI_GENERAL_ALARM,
    HS_SNI_UNREAD_SMS,
    HS_SNI_TRANS_IRDA,
    HS_SNI_TRANS_DMANGER,
    HS_SNI_POWER_ON,
//    HS_SNI_ERR_CHARGING,
//    HS_SNI_DONE_CHARGING,
//    HS_SNI_ON_CHARGING,
    HS_SNI_MAX_SNI      
};
typedef enum tagHS_SNI_E    HS_SNI_E;

struct tagHS_SNI_T 
{
    HS_COMMAND_HEADER_T     Header;     /* Command header */
    HS_SNI_E                SNI;
    ONOFF                   OnOff;
};
typedef struct tagHS_SNI_T  HS_SNI_T;

struct tagHS_CONTRAST_T
{
    HS_COMMAND_HEADER_T     Header;
    HS_CONTRAST_E           Contrast;
	BYTE					ContrastLevel;
};
typedef struct tagHS_CONTRAST_T HS_CONTRAST_T;

#ifdef FEATURE_PARTIAL_DISPLAY

struct tagHS_PARTIAL_SCREEN_T
{
    HS_COMMAND_HEADER_T     Header;
    BYTE*					Buffer;
	BYTE					StartX;
	BYTE					StartY;
	BYTE					EndX;
	BYTE					EndY;
};

typedef struct tagHS_PARTIAL_SCREEN_T HS_PARTIAL_SCREEN_T;
#endif
/*---------------------------------------------------------------------------
** Handset command packet which is passed to hs_cmd()
*/
union tagHS_COMMAND_U
{
    HS_COMMAND_HEADER_T     Header;
    HS_CONTROL_T            Control;
    HS_KEY_INPUT_T          KeyInput;
    HS_SNI_T                SNI;
    HS_CONTRAST_T           Contrast;
#ifdef FEATURE_PARTIAL_DISPLAY
	HS_PARTIAL_SCREEN_T		PartialScreen;
#endif
};
typedef union tagHS_COMMAND_U       HS_COMMAND_U;



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
extern q_type hs_cmd_free_q;

//extern boolean (*hs_poll_gpio)(void);
extern boolean hs_poll_gpio(void);
#if defined(FEATURE_SDEVMAP_SHARE_HFK) && defined(FEATURE_SECOND_UART)
//extern void (*hs_disable_hfk_pins)(void);
//extern void (*hs_enable_hfk_pins)(void);
extern void hs_disable_hfk_pins(void);
extern void hs_enable_hfk_pins(void);
#endif  /* FEATURE_SDEVMAP_SHARE_HFK && FEATURE_SECOND_UART */

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern SKY_EVENT_E SKY_API SKY_GetKey(void);

extern void SKY_API SKY_InitKey(REX_TCB_T *pTask, REX_SIGNAL KeySignal);
extern void hs_task(DWORD dwDummy);
extern void hs_cmd(HS_COMMAND_U *pCommand);

extern void SKY_API       InitHS(void);
extern void SKY_API       SKY_InitHS(void);

extern BOOL HS_EXT_PWR_ON_4GVM(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_HS_H_ */



