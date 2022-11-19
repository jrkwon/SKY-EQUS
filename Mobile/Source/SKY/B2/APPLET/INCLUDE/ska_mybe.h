/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_mybe.h
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
** 2001-01-12   huni     Created.
**
** ============================================================================
*/

#ifndef _SKA_MYBE_H_
#define _SKA_MYBE_H_

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
#define		MAX_MYBELL_SIZE		10
#define		MYBELL_DATA_MAX		150
#define     GVMBELL_DATA_MAX    300

#if SKY_SOUND_INTERFACE == SKY_3100_SOUND_INTERFACE
#define  MYMELODY_DATA_MAX   127
#elif SKY_SOUND_INTERFACE == SKY_5000_SOUND_INTERFACE
//#define  MYMELODY_DATA_MAX   500
#define  MYMELODY_DATA_MAX   1500
#endif

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef	struct {
	BYTE	nUsed;							// 0=Unused, 1=Used
	BYTE	nAttr;							// 0x01=New,    0x02 = Read,  0x04 = Sinsegi 
    BYTE    nDate[4];						// Date
	BYTE	nTextSize;						// Chari data size
	BYTE	szText[MYBELL_DATA_MAX];		// Chari data
} mybell_nv_type;

typedef	struct {
	BYTE	nUsed;							// 0=Unused, 1=Used
	BYTE	nAttr;							// 1=New,    2 = Read. 
    BYTE    nDate[4];						// Date
} mybell_buf_type;

  
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
extern  mybell_buf_type       a_MybellBuffer[MAX_MYBELL_SIZE];
extern  mybell_nv_type        MybellRecord;
extern  BYTE                  g_MyBellSize; 
    
    /*
    **---------------------------------------------------------------------------
    **  Function(external use only) Declarations
    **---------------------------------------------------------------------------
    */
extern  void g_NewMybellSave(UI_COMMAND_U *pCommand);
extern  VOID SKA_MybellMessage(SKY_EVENT_T* pEvent);
extern  INT8 g_MybellCount(VOID);
extern  VOID SKY_API SKY_InitMybell(VOID);
extern  word Convert_RingerToNote(uint16 , uint16 );
extern void Make_MyMelody_Header(	
	word *Hp,    /* preplay ram buffer   */
	byte timb    /* Timbre to load     */
);   
extern VOID SKY_API gDeleteAllMybell( VOID );

extern      BOOL    gfIsThereMB( VOID );

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_MYBE_H_ */
