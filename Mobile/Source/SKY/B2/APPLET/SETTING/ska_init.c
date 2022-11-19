/*
** ===========================================================================
**
** Project: 
**     SkySimul
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
** 4333-12-15 white     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"

extern void SKY_MMInvsetup(void);
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

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

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void SKY_API sSKA_SysInit(SKY_EVENT_T *pEvent);

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

void SKY_API SKA_MainSysInit(SKY_EVENT_T *pEvent)//
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_CANCEL, 
            (BYTE*)SKY_GET_TOKEN(TKN_SYSTEM_INIT), 
            WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_Image( 
		   IMAGE_X, 
		   IMAGE_Y, 
		   //(BITMAP*)&g_BmpSecret120x93x256c
           (BITMAP*)&g_BmpLock
		  ); 

        //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
        sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
        //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

        EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
        EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
        EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
		EQS_SetIOCActive(sNumHandle);
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_START_SIBLING(sSKA_SysInit, FALSE);              
        else
            EQC_MessageBox((BYTE *)(BYTE*)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;        
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sInitAllFile(void)
{
    UINT8 i;

    /* -------------------------------
    ** Delete all data and directories 
    */

    /* delete data files */
    SKY_DeleteDataFiles(SDF_GVM);       /* GVM data */
    SKY_DeleteDataFiles(SDF_MELODY);    /* Melody data */
    SKY_DeleteDataFiles(SDF_IMAGE);     /* image/animation data */
    SKY_DeleteDataFiles(SDF_PHOTO);     /* photo image data */
    SKY_DeleteDataFiles(SDF_MAIL);      /* photo image data */
#ifdef FEATURE_SKTT_PM
    SKY_DeleteDataFiles(SDF_VM);
#endif
#ifdef FEATURE_CHARACBELL
    SKY_DeleteDataFiles(SDF_CHA_MELODY);    // Characbell melody data
    SKY_DeleteDataFiles(SDF_CHA_IMAGE);     // Characbell image data
#endif // FEATURE_CHARACBELL

    /* application data files */
    PBookClose();                   //close phone book files 
    SKY_DeleteAllFile(SKY_PBK_DIR); //BookCleanUp();                      /* phone book */
    SKY_DeleteAllFile(SKY_SCHL_DIR);    /* schedular */
    SKY_DeleteAllFile(SKY_VMS_DIR); //SKY_InitSmsData();                  /* SMS */
    SKY_DeleteAllFile(SKY_MBL_DIR);
    SKY_DeleteAllFile(SKY_SMSMO_DIR);
    SKY_DeleteAllFile(SKY_CALLSTACK_DIR); /* call stack */

#ifdef	FEATURE_WAVELET	//	
    SKY_DeleteDataFiles(SDF_WAVELET);
    SKY_DeleteDataFiles(SDF_TEMPWAV);
#endif	//	#ifdef	FEATURE_WAVELET

    if(SKY_IsFileOrDirExist(SKY_TEMP_FILE_NAME))
    {
        if(SKY_GET_DM_TEMP_FILE() != SKY_INVALID_FILE_HANDLE)
            SKY_CloseFile(SKY_GET_DM_TEMP_FILE());

        SKY_SET_DM_TEMP_FILE(SKY_INVALID_FILE_HANDLE);
        SKY_DeleteFile(SKY_TEMP_FILE_NAME);
    }

    /* system files */
    SKY_DeleteAllFile(SKY_SYS_DIR);
//    SKY_DeleteSysConfig();              /* system info */
//    SKY_DeleteMiscConfig();             /* misc. info */
//    SKY_DeleteAppConfig();              /* system app info */
//#ifdef FEATURE_GVM
//    SKY_DeleteGVMConfig();              /* GVM info */
//#endif //#ifdef FEATURE_GVM

    /* -------------------------
    ** remove dir 
    */
    /* data dir */
    SKY_RemoveDir(SKY_GVM_DIR);
    SKY_RemoveDir(SKY_MELODY_DIR);
    SKY_RemoveDir(SKY_IMAGE_DIR);
#ifdef FEATURE_CHARACBELL
    SKY_RemoveDir(SKY_CHA_MELODY_DIR);
    SKY_RemoveDir(SKY_CHA_IMAGE_DIR);
    SKY_RemoveDir(SKY_CHARACBELL_DIR);
#endif // FEATURE_CHARACBELL
    SKY_RemoveDir(SKY_PHOTO_DIR);    
    SKY_RemoveDir(SKY_MAIL_DIR);

    /* application dir */
    SKY_RemoveDir(SKY_PBK_DIR);
    SKY_RemoveDir(SKY_SCHL_DIR);
    SKY_RemoveDir(SKY_VMS_DIR);
    SKY_RemoveDir(SKY_MBL_DIR);
    SKY_RemoveDir(SKY_SMSMO_DIR);
    SKY_RemoveDir(SKY_CALLSTACK_DIR); /* call stack */

#ifdef FEATURE_SKTT_PM
    SKY_RemoveDir(SKY_VM_DIR);
#endif

#ifdef	FEATURE_WAVELET	//	
    SKY_RemoveDir(SKY_WAVELET_DIR);    
    SKY_RemoveDir(SKY_TEMPWAV_DIR);    
#endif	//	#ifdef	FEATURE_WAVELET

    /* system */
    SKY_RemoveDir(SKY_SYS_DIR);    

    /* remove root(sky) dir */
    SKY_RemoveDir(SKY_ROOT_DIR);


#ifdef FEATURE_SKVM
    /* remove skvm dir & files */
    SKY_RemoveSKVMAllData(SKY_SKVM_DIR);
#endif
    /* 
    ** remvoe voice recording/auto answering recording data file 
    ** NV_MAX_FL_FILE_NUM -- nv.h
    ** stored in root dir.
    ** file name : vmemo? 
    */
    /* ---> when nv is erase, voice_erase_all_when_nv_setup is called..
    for(i = 0; i < NV_MAX_FL_FILE_NUM; i++)
    {
        BYTE szRecFileName[SKY_PATH_NAME_MAX_LENGTH];

        sprintf((char*)szRecFileName, "vmemo%d", i);
        SKY_DeleteFile(szRecFileName);
    }
    ---- */

    /* -------------------------------------------
    ** Make data directories and init applications 
    */
    SKY_InitSystemFileAndDataDir();
    SKY_InitApplicationFiles();
}



//void SKY_API SKY_InitVoiceInfo()
//{
//#if 0    //hjcho 01.05.12   
//
//   BYTE                bIndex; 
//   VMEMOINFO_T         VoiceInfo; 
//   nv_fn_sequence_type sFn_sequence[NV_MAX_FL_FILE_NUM];
//    
//    for(bIndex = 0; bIndex <NV_MAX_FL_FILE_NUM; bIndex++)
//   {
//      sFn_sequence[bIndex].address = bIndex;
//      sFn_sequence[bIndex].Fn      = NV_MAX_FL_FILE_NUM;
//      sFn_sequence[bIndex].Type    = 0;
//      sFn_sequence[bIndex].Time    = 0;
//	  //nv_cmd.data_ptr->Fn_sequence.address = index;	  
//	  //nv_cmd.data_ptr->Fn_sequence.Fn = NV_MAX_FL_FILE_NUM;
//	  //nv_cmd.data_ptr->Fn_sequence.Type = 0; // VTYPE_NONE
//	  //nv_cmd.data_ptr->Fn_sequence.Time = 0;
//      SKY_PutNV(NV_FN_SEQUENCE_I, (nv_item_type *)&sFn_sequence[bIndex]);
//	}
//
//    
//    VoiceInfo.bNewAutoAnsNumber = 0;
//    VoiceInfo.bTotalRecNumber   = 0;
//    VoiceInfo.bUserVoiceFn      = NV_MAX_FL_FILE_NUM;
//    VoiceInfo.dwRemainedByte    = TOTAL_REC_BYTE;
//    VoiceInfo.fUserVoice        = FALSE;
//    VoiceInfo.wRemainedTime     = TOTAL_REC_TIME;
//    VoiceInfo.wUsedTime         = 0;
//    VoiceInfo.wVoiceMemoReocrdingTime = 0;
//
//    SKY_PutNV(NV_VOICE_INFO_I, (nv_item_type *)&VoiceInfo);    
//#endif //0
//}




LOCAL void SKY_API sSKA_SysInit(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl, sOnInit;
    STATIC BOOL fDoneSysInit = FALSE;
    WORD    wSpare1;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                (BYTE*)SKY_GET_TOKEN(TKN_SYSTEM_INIT), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SYS_INIT), MESSAGE_DEFAULT_TIME, MBT_NOYES, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        if(fDoneSysInit)
        {
            fDoneSysInit = FALSE;
            SKY_PowerOff(POM_RESET);//ui_power_reset();
        }else
            fDoneSysInit = FALSE;
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            EQS_End();
        }
        else if(pEvent->wParam == sOnInit)
        {
            SKY_MMInvsetup();
            sInitAllFile();
            gDeleteMsg();
            wSpare1 = SKY_VIRGIN_SYSTEM_FLAG;
            SKY_PutNV(NV_SPARE_1_I, &wSpare1);
            fDoneSysInit = TRUE;
            s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SYS_INIT_DONE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        }
        break;

    case SE_IO_MESGBOX_YES:
        sOnInit = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SYS_ON_INIT), 0, MBT_NONE,MBT_HELP);
        break;

    case SE_IO_MESGBOX_NO:
        EQS_END();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_IO_MESGBOX_CLEAR:
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

