/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     sky_hcmd.h
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
** 2000-11-30 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_HCMD_H_
#define _SKY_HCMD_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#ifdef FEATURE_DS
#include "ds.h"
#endif /* FEATURE_DS */

#include "uasms.h"
#include "smsi.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
// Broadcast SMS 정보 읽기
#define GET_BCSMS_CONFIG()         g_BroadcastSMS.bcsms_config
#define GET_BCSMS_PREF()           g_BroadcastSMS.bcsms_pref
#define GET_BCSMS_TABLE()          g_BroadcastSMS.bcsms_srvc_table
#define GET_BCSMS_INFO()           g_BroadcastSMS.bcsms_srvc_info

// Broadcast SMS 정보 쓰기
#define SET_BCSMS_CONFIG(c)        g_BroadcastSMS.bcsms_config = c
#define SET_BCSMS_PREF(p)          g_BroadcastSMS.bcsms_pref = p
#define SET_BCSMS_TABLE(t)         g_BroadcastSMS.bcsms_srvc_table = t
#define SET_BCSMS_INFO(i)          g_BroadcastSMS.bcsms_srvc_info = i

#define GET_EVENT_PARAM()           (&g_EventParam)
#define SET_EVENT_PARAM(s)          (g_EventParam = s)

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
typedef struct {
    boolean                         active;
    uasms_bc_service_info_type      info;
} ui_uasms_bc_service_info_type;

typedef struct {
    uint16                          size;
    ui_uasms_bc_service_info_type   entries[UASMS_BC_TABLE_MAX];
} ui_uasms_bc_table_type;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */


#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
struct tagBROADCAST_SMS_T
{
    uasms_bc_config_e_type        bcsms_config;
    uasms_bc_pref_e_type          bcsms_pref;
    ui_uasms_bc_table_type        bcsms_srvc_table;
    ui_uasms_bc_service_info_type bcsms_srvc_info;
};
typedef struct tagBROADCAST_SMS_T       BROADCAST_SMS_T;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */



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
extern UI_COMMAND_U        g_EventParam;

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
extern BROADCAST_SMS_T     g_BroadcastSMS;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API SKY_HandleCommands(UI_COMMAND_U *pCommand);
extern BOOL IsCallerIDComplete(void);
extern void SetCallerIDComplete(BOOL);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_HCMD_H_ */
