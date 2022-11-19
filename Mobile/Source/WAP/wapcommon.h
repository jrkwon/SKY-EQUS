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
#define WAP_CMD_BUFFERS         50      /* temporary value for wap command queue buffer */

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
void SKY_API WAP_HandleCommands(wap_cmd_type *pCommand);
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API WAP_HandleCommands(wap_cmd_type *pCommand);
extern void process_draw_bar(void);
extern void bar_ani_start(int type, int time);
extern void bar_ani_stop(void);
extern byte get_length( byte *chk_str, byte buff_length);
extern void send_cmd2wapHandler ( wap_cmd_type *cmd_ptr );
extern void wap_check_and_run(void);
extern void WAP_Repaint(void);
extern void wap_display_exit_message(void);
extern q_type           wap_cmd_q;      /* WAP command queue              */
extern q_type           wap_cmd_free_q; /* WAP command free queue         */
extern q_type           ui_cmd_q;       /* queue for commands - ui_cmd does this */
extern q_type           ui_cmd_free_q;  /* free buffers for talking to UI */
extern rex_timer_type  wap_control_timer;
extern wap_cmd_type wap_buffers[ WAP_CMD_BUFFERS ];
extern clk_cb_type clk_timer; 
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

#ifdef __cplusplus
}  /* extern "C" */
#endif


#endif  /* #ifndef _WAPCOMMON_H_ */
