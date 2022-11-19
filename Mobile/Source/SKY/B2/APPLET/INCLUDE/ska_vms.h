/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_vms.h
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
** 2000-12-28   huni     Created.
**
** ============================================================================
*/

#ifndef _SKA_VMS_H_
#define _SKA_VMS_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define     MAX_VMS_MSG			20  // huni vms 20 (10 ->20)
#define	    MAX_SMS_DIAL		32

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef	struct {
    INT8    nAddress;							
    BYTE    nUsed;							// 0=Unused, 1=Used
    BYTE    nAttr;                          // 1=New,    2 = Read. 
    BYTE    nDate[4];						// Date
    UINT8   nDialSize;						// Chari data size
    BYTE    szDial[MAX_SMS_DIAL];		    // Chari data
} vms_nv_type;

typedef	struct {
    INT8    nAddress;							
    BYTE    nUsed;							// 0=Unused, 1=Used
    BYTE    nAttr;                          // 1=New,    2 = Read. 
    BYTE    nDate[4];						// Date
} vms_buf_type;
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
extern      BYTE            g_VmsSize;
extern      vms_buf_type    a_VmsBuffer[MAX_VMS_MSG];

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern      VOID    SKA_VoiceMessage(SKY_EVENT_T* pEvent);
extern      INT8    g_VoiceCount( void );
extern      VOID    g_NewVmsSave(UI_COMMAND_U *pCommand);

extern      VOID    SKY_API SKY_InitVMS(VOID);
extern      VOID    SKY_API gDeleteAllVms( VOID );

extern      BOOL    gfIsThereVms( VOID );

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_VMS_H_ */
