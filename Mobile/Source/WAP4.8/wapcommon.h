/*
** ============================================================================
**
** Project: 
**     BUNNY
**
** File:
**     
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
** 2001-07-16 ytchoi     Created.
**
** ============================================================================
*/


#ifndef _WAPCOMMON_H_
#define _WAPCOMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "wapconnector.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "GenDsp.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define WAP_ANI_TEXT_CONNECTING			1	//접속중
#define WAP_ANI_TEXT_RECEIVING			2	//수신중
#define WAP_ANI_TEXT_SECURITY			3	//보안중
#define WAP_ANI_TEXT_ADDBOOKMARK		4	//추가중
#define WAP_ANI_TEXT_RESETUP			5	//재설정
#define WAP_ANI_BAR_FROM_LEFT			FALSE
#define WAP_ANI_BAR_FROM_RIGHT			TRUE

#define WAP_CMD_BUFFERS         50      /* temporary value for wap command queue buffer */
//temp
#define PUSH_STORAGESIZE   4500

#define FEATURE_WAP_REDIRECT
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
#ifdef FEATURE_WAP_REDIRECT
typedef struct
{
	BOOL   bCorrect;
	BOOL   bPerm_or_not;
	BOOL   bNetinclude;
	BOOL   bPortNuminclude;

    UINT8  Addresslen;

	UINT8  BearerType;
	UINT16 Portnumber;

	CHAR  *Address;
} redirect_config;
#endif //FEATURE_WAP_REDIRECT
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
void SKY_API WAP_HandleCommands(wap_cmd_type *pCommand);
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API WAP_HandleCommands(wap_cmd_type *pCommand);
extern void displayloading(int catagory);
extern void ProcessEvents(void);
extern VOID PlayNTopMainWapImage(void);
//extern void wap_release_cmd(void);
extern void process_draw_bar(void);
extern void bar_ani_start(int type, int time);
extern void bar_ani_stop(void);
extern byte get_length( byte *chk_str, byte buff_length);
extern void send_cmd2wapHandler ( wap_cmd_type *cmd_ptr );
extern void wap_check_and_run(void);
extern void WAP_Repaint(void);
extern void wap_display_exit_message(void);
extern void wap_display_exit_for_skvm(void);
extern void wap_display_exit_for_gvm(void);
extern void BartimerTick(int4  interval);
extern void wap_display_error_message(int kindOfError, int kindOfType);
extern void WAP_Time_Stmap(BOOL bStart);
extern q_type           wap_cmd_q;      /* WAP command queue              */
extern q_type           wap_cmd_free_q; /* WAP command free queue         */
extern q_type           ui_cmd_q;       /* queue for commands - ui_cmd does this */
extern q_type           ui_cmd_free_q;  /* free buffers for talking to UI */
extern rex_timer_type  wap_control_timer;
extern wap_cmd_type wap_buffers[ WAP_CMD_BUFFERS ];
extern clk_cb_type clk_timer; 
extern clk_cb_type wapbar_timer;
extern ip4a wapip;
extern WAP_Content_Input *g_pCurInput;
extern int g_nViewMode;
extern int g_nMenuMode;
extern WAP_Card *g_pCard;
extern int g_nDlgId;
extern char *g_pDlgMessage;
extern char *g_pDlgMessage2;
extern int g_reloaderrorTime;
extern UINT8 ui_errortype ;
extern rex_timer_type  bar_ani_timer;			/* Timer for animaton */
extern BOOL g_bDrawBar;

extern boolean start_disp;
extern int g_nAniTextMode;

#ifdef FEATURE_WAP_REDIRECT
extern redirect_config redirect;
extern CHAR gWAPRedirectUrl[1500];
extern BOOL gredirectloadingbar ;
#endif //#ifdef FEATURE_WAP_REDIRECT

#ifdef __cplusplus
}  /* extern "C" */
#endif

#ifdef LOG_ENABLE
#define MAX_CACHE_SIZE 40000
#else
#define MAX_CACHE_SIZE 40000
#endif //LOG_ENABLE


#endif  /* #ifndef _WAPCOMMON_H_ */
