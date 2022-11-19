#if !defined(_EQS_FILE_H_)
#define _EQS_FILE_H_

#include "fs.h"
#include "uih.h"
#include "uistate.h"
#include "eqa_call.h"
#include "uixsnd.h"


/**---------------------------------------------------------------------------
**                  FILE MANAGE 
**---------------------------------------------------------------------------*/


typedef struct tagSYSTEM_STATE_T  SYSTEM_STATE_T;
struct tagSYSTEM_STATE_T
{
    EFS_PHONESTATE_T  PhoneState;
    EFS_SNDSTATE_T    SoundState;
    EFS_SNDSETTING_T  SoundSetting;

    BYTE bBackLight;
    BOOL fUTCTime;                  // for SMS  UTCTIME Information
};


typedef struct tagSYSTEM_CALLINFO_T  SYSTEM_CALLINFO_T;
struct tagSYSTEM_CALLINFO_T
{

// call state 
  BOOL               fAutoAnswer;                     //ui.isanswer
  BYTE               nAnswer;                         //ui.answer
    
// call info
  EFS_CALLINFO_T     CInfoData;

};


typedef struct tagSYSTEM_CALLSTACK_T   SYSTEM_CALLSTACK_T;
struct  tagSYSTEM_CALLSTACK_T
{
// call stack 
  nv_stack_idx_type  Index;                     //ui.sidx
  nv_stdial_type     Stack[NV_MAX_STACK_DIALS]; //ui.st
};

enum tagSYSTEM_CONFIG_E
{
    SYSTEM_STATE_E = 0,
    SYSTEM_CALLINFO_E =1,
    SYSTEM_CALLSTACK_E =2
};
typedef enum tagSYSTEM_CONFIG_E  SYSTEM_CONFIG_E;

#define MAX_SYSTEM_FILE   3


//extern BOOL EQS_OpenData ( SYSTEM_CONFIG_E  Config );
//extern BOOL EQS_CloseData( SYSTEM_CONFIG_E  Config );
extern BOOL EQS_DataFileSetup(BOOL fReset);
extern INT EQS_ReadData(  SYSTEM_CONFIG_E  Config, void* pData );
extern INT EQS_WriteData( SYSTEM_CONFIG_E  Config, void* pData );



#endif  /* _EQS_H_ */