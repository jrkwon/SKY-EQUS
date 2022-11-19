/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_evnt.h
**
** Description:
**     EQUS event
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-05-02 redstar    add menu event into nui eventdata type
** 2000-05-02 redstar    add io applet event into nui eventdata type
** 2000-04-03 redstar    add event category into nui eventdata type
** 2000-03-21 redstar    First revision
** 1999-11-30 girin      Created
** ============================================================================
*/

#ifndef _EQS_EVNT_H_
#define _EQS_EVNT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
/* do not include eqs_type file. this file is included in the EQUS Simulator */
//#include "eqs_type.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define EQS_OFFSET_APP_EVENT        1       /* 1 ~ 99 */
#define EQS_OFFSET_IO_EVENT         100     /* 100 ~ 199 */
#define EQS_OFFSET_TM_EVENT         200     /* 200 ~ 299 */
#define EQS_OFFSET_KEY_EVENT        300     /* 300 ~ 399 */
#define EQS_OFFSET_SOFTKEY_EVENT    400
#define EQS_OFFSET_MAX_EVENT        500     

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagEQS_EVENT_ID 
{
#undef DEFINE_EVENT
#undef DEFINE_EVENT_CATEGORY
#define DEFINE_EVENT_CATEGORY(nID, nCategory) EQE_##nID = nCategory,
#define DEFINE_EVENT(nID) EQE_##nID,
#include "eqs_evnt.tbl"
    EQE_BYPASS = EQS_OFFSET_MAX_EVENT
};
typedef enum   tagEQS_EVENT_ID           EQS_EVENT_ID;

enum tagEQS_EVENT_CATEGORY 
{
    EQC_APP,
    EQC_IO,
    EQC_KEY,
    EQC_TM,
    EQC_ERROR
};
typedef enum tagEQS_EVENT_CATEGORY  EQS_EVENT_CATEGORY;

typedef short int         NATIVE_EVENT_ID;

struct tagEQS_EVENT
{
    EQS_EVENT_ID        EventID;        /* EQUS event */
    unsigned short      wParam;         /* WORD */
    unsigned long       dwParam;        /* DWORD */
    unsigned long       dwExtEventID;   /* the external event. e.g. SKY */
}; 
typedef struct tagEQS_EVENT       EQS_EVENT;
  
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

#endif  /* _EQS_EVNT_H_*/


