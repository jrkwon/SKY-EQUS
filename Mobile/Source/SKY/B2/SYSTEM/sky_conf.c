/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_conf.c
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
** 2000-10-30 jrkwon     Created.
**
** ===========================================================================
*/

 
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h" // 01.05.12 hjcho -> 녹음/재생 VoiceInfo를 초기화 하기 위해
#ifdef FEATURE_GVM
#include "..\APPLET\GVM\SWAP\gvm_config.h"
#endif
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

//#define sIS_CONFIG_DIRTY(SkyConfig)     g_aSkyConfig[SkyConfig].fDirty
#define sRESET_CONFIG_DIRTY(SkyConfig)  g_aSkyConfig[SkyConfig].fDirty = FALSE


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
/* EFS에 기록되는 정보 */
EXT_CONFIG_T        g_ExtConfig;    /* 기타 추가로 필요한 정보 - 데이터 파일 생성 정보 */
MISC_CONFIG_T       g_MiscConfig;   /* 부가 기능 정보 */
SYSTEM_CONFIG_T     g_SysConfig;    /* 시스템 정보 */
APP_CONFIG_T        g_AppConfig;    /* 응용프로그램 정보 */
#ifdef FEATURE_GVM
GVM_CONFIG_T        g_GVMConfig;    /* GVM 정보 */
#endif//#ifdef FEATURE_GVM
#ifdef FEATURE_MCOMMERCE
MW_TEMPCERT_CONFIG_T	g_MWTempCertConfig1;
MW_TEMPCERT_CONFIG_T	g_MWTempCertConfig2;
MW_MYCERT_CONFIG_T		 g_MWMYCertConfig1;
MW_MYCERT_CONFIG_T		 g_MWMYCertConfig2;
MW_MYCERT_CONFIG_T		 g_MWMYCertConfig3;
MW_MYCERT_CONFIG_T		 g_MWMYCertConfig4;
MW_MYCERT_CONFIG_T		 g_MWMYCertConfig5;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig1;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig2;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig3;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig4;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig5;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig6;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig7;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig8;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig9;
MW_TCACERT_CONFIG_T		g_MWTCACertConfig10;
MW_WALLET_CONFIG_T		g_MWWalletConfig;
//MW_CONFIG_T			g_MWConfig;
#endif /* FEATURE_MCOMMERCE */
#ifdef FEATURE_CHARACTER_BELL
CHARACBELL_CONFIG_T     g_CharacbellConfig;
#endif // FEATURE_CHARACTER_BELL

/* SKY 환경 설정 */
SKY_CONFIG_T g_aSkyConfig[] = {
    { FALSE, SKY_WriteExtConfig },
    { FALSE, SKY_WriteMiscConfig },
    { FALSE, SKY_WriteSysConfig },
    { FALSE, SKY_WriteAppConfig },
#ifdef FEATURE_GVM
    { FALSE, SKY_WriteGVMConfig },
#endif//#ifdef FEATURE_GVM
#ifdef FEATURE_MCOMMERCE
    { FALSE, SKY_WriteMWTempCert1Config },
	{ FALSE, SKY_WriteMWTempCert2Config },
	{ FALSE, SKY_WriteMWMyCert1Config },
	{ FALSE, SKY_WriteMWMyCert2Config },
	{ FALSE, SKY_WriteMWMyCert3Config },
	{ FALSE, SKY_WriteMWMyCert4Config },
	{ FALSE, SKY_WriteMWMyCert5Config },
	{ FALSE, SKY_WriteMWTCACert1Config },
	{ FALSE, SKY_WriteMWTCACert2Config },
	{ FALSE, SKY_WriteMWTCACert3Config },
	{ FALSE, SKY_WriteMWTCACert4Config },
	{ FALSE, SKY_WriteMWTCACert5Config },
	{ FALSE, SKY_WriteMWTCACert6Config },
	{ FALSE, SKY_WriteMWTCACert7Config },
	{ FALSE, SKY_WriteMWTCACert8Config },
	{ FALSE, SKY_WriteMWTCACert9Config },
	{ FALSE, SKY_WriteMWTCACert10Config },
	{ FALSE, SKY_WriteMWalletConfig },
#endif /* FEATURE_MCOMMERCE */
#ifdef FEATURE_CHARACTER_BELL
    { FALSE, SKY_WriteCharacbellConfig },
#endif // FEATURE_CHARACTER_BELL
    { FALSE, EQS_NA }
};

SYSTEM_STATE_T      g_SysState;     /* 시스템 상태 */
CALL_STATE_T        g_CallState;    /* 통화 상태 */

#ifdef FEATURE_USE_IRDA
UINT8          gIrex_ModelIndex;
#endif
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
LOCAL void SKY_API sInitApp(void);
LOCAL void SKY_API sInitMisc(void);
LOCAL void SKY_API sInitSys(void);
#ifdef FEATURE_GVM
LOCAL void SKY_API sInitGVM(void);
#endif //#ifdef FEATURE_GVM
#ifdef FEATURE_CHARACTER_BELL
LOCAL void SKY_API sInitCharacbell(void);
#endif // FEATURE_CHARACTER_BELL

//#ifdef FEATURE_MCOMMERCE
//extern void SKY_API InitMW(BOOL IsAll);
//#endif /* FEATURE_MCOMMERCE */

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

void SKY_API SKY_InitSystemState(void)
{
    SYSTEM_STATE_T SysState = {
        INIT_IS_DEBUG_SCREEN,           /* fDebugScreen */
//#ifdef FEATURE_DS_DEBUG_UI
        INIT_IS_DATA_DEBUG_SCREEN,      /* fDataDebugScreen */
//#endif /* FEATURE_DS_DEBUG_UI */
//        INIT_IS_OK_TO_SLEEP,            /* fOK2Sleep */
        INIT_IS_POWER_ON,               /* fPowerOn */
        INIT_IS_PHONE_OPEN,             /* fPhoneOpen */
        INIT_IS_EAR_JACK_ON,            /* fEarJack */
#if SKY_MODEL_ID == SKY_MODEL_B1 || (SKY_MODEL_ID == SKY_MODEL_B2)
        INIT_IS_CAMERA_ON,              /* fCamera */
#endif
        INIT_IS_MUTE,                   /* fMute */
        INIT_IS_SHUT_DOWN,              /* fShutDown */
        INIT_IS_OFFLINE,                /* fOffline */
//        INIT_IS_GETKEY,                 /* fGetKeys */
        INIT_IS_VALID_SYSTEM_TIME,      /* fSysTimeValid */
        INIT_IS_HANDS_FREE,             /* fHandsFree */
        INIT_IS_CHARGING,               /* fCharging */
        INIT_IS_EXTERNAL_POWER,         /* fExtPower */
        INIT_KEYPAD_TEST,               /* diag keypad test */
/* GVM */
/* rosa 01-05-03 --> @why, what */
        INIT_TO_WAP,                    /* Go To WAP state */
        INIT_FROM_WAP,                  /* Go From WAP state */
#ifdef FEATURE_GVM
        INIT_IN_GVM,                    /* GVM state */
        INIT_PLAYING_GVM,
        INIT_TO_GVM,                    /* Go To GVM state */
        INIT_FROM_GVM,
#endif //FEATURE_GVM
        INIT_IN_ALARM,                  /* Alarm state */
/* <-- */
#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) || defined(FEATURE_WAVELET)
        INIT_SOCK_MODE,                 /* fSockMode; */
        INIT_SOCK_CONNECT,              /* fSockConnect; */
#endif	//	#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) ||  defined(FEATURE_WAVELET)

#ifdef	FEATURE_WAVELET	//	khekim 01/02/11
		INIT_WAVELET_STATE,             /* fWavelet state */
		INIT_FROM_WAVELET,              /* From Wavelet */
		INIT_TO_WAVELET,                /* To Wavelet */
#endif	//	#ifdef	FEATURE_WAVELET
        INIT_POWER_OFF_MODE,            /* fPowerOffMode */
        INIT_AUTO_LOCK,                 /* fAutoLock */
        INIT_SMS_LOCK,                  /* fSMSLock*/
        INIT_NO_OFF_BACKLIGHT,          /* fNoOffBacklight */
        INIT_DATAMANAGER,               /* fDataManager*/
        INIT_AIR_DATAMANAGER,           /* fAirDataManagerOrg */
        INIT_AIR_DATAMANAGER,           /* fAirDataManagerRcv */
        INIT_IP_AIR_DATAMANAGER,        /* fIPAirDataManager */
        INIT_DATA_CALL,                 /*fDataCall */
        INIT_DTR_DISP,                  /* bDtrDisp */    
        INIT_MAINT_REQ                  /* maintenace request */
#ifdef FEATURE_AGILENT_TEST
        ,
        INIT_AGILENT_TEST,              /* fAgilentTest */
        INIT_LOOP_BACK,
        INIT_POWER_CONTROL
#endif//#ifdef FEATURE_AGILENT_TEST
#ifdef FEATURE_SKT_NDSS_DS
        ,INIT_NDSS_ORG_FAIL             /* fNDSSOriFail */
#endif //FEATURE_SKT_NDSS_DS
        /* rosa 01-05-03 --> @why, what */
#ifdef FEATURE_SKTT_PM
        ,INIT_TO_PM                  /* Go To PM state */
#endif //FEATURE_SKTT_PM 
        /* <-- */
#ifdef FEATURE_SKVM
		,INIT_IN_SKVM				 /* State In SKVM */	
        ,INIT_TO_SKVM                /* Go To SKVM  */
        ,INIT_FROM_SKVM              /* From  SKVM  */
		,INIT_PUSH_SMS_IN_SKVM		 /* Push SMS In SKVM */	
#endif //FEATURE_SKVM
#ifdef FEATURE_WAP_WAVELET
        ,INIT_WAP_TO_WAVELET                /* Go To WAVELET  */
        ,INIT_WAVELET_TO_WAP            /* WAVELET GO TO WAP  */
#endif //FEATURE_WAP_WAVELET      
        ,
        INIT_DM_TEMPFILE,
        INIT_IGNORE_BACKLIGHT,
        INIT_AUTO_ANSWER_FROM_POWER_UP,
        INIT_NEW_MESSAGE,
        INIT_IN_GAME,
        INIT_EMAIL_INPROG
#ifdef FEATURE_WAP
        ,
        INIT_WAP_STATE                 /*SKY_IS_IN_WAP() = FALSE*/
#endif //#ifdef FEATURE_WAP
        ,
        INIT_VIRGIN_SYSTEM
    };

    /* copy the initial values */
    g_SysState = SysState;
    
}

void SKY_API SKY_InitCallState(void)
{
    memset(&(g_CallState.PhoneInfo), NULL, sizeof(CM_PHONE_INFO_T));
    memset(&(g_CallState.CNI), NULL, sizeof(CALLING_NUMBER_ID_T));
    g_CallState.fOTASPFailed       = INIT_IS_OTASP_FAILED;
    g_CallState.fAlert             = INIT_IS_ALERT;
    g_CallState.fRoamCall          = INIT_IS_ROAM_CALL;
    g_CallState.fDigital           = INIT_IS_DIGITAL;
    g_CallState.fOriginate         = INIT_IS_ORIGINATE;
    g_CallState.fTransmitDTMF      = INIT_IS_TRANSMIT_DTMF;
    g_CallState.ForceServiceOption = INIT_FORCE_SERVICE_OPTION;
#ifdef FEATURE_UI_DORMANT
    g_CallState.fDormant           = INIT_DORMANT;
#endif
}

/*
** dirty 플래그
** 쓰기는 값이 변경되었을 때만 수행되도록 한다.
** 읽기는 처음에 한번만 읽어두면, 저장매체에 있는 값과 항상 동기되므로 
** 다시 읽을 필요가 없다.
*/

/* called by 시스템 초기화 */
void SKY_API SKY_ResetConfig(void)
{
    /* remove */
    SKY_DeleteExtConfig();
    SKY_DeleteMiscConfig();
    SKY_DeleteSysConfig();
    SKY_DeleteAppConfig();
#ifdef FEATURE_GVM
    SKY_DeleteGVMConfig();
#endif//#ifdef FEATURE_GVM
#ifdef FEATURE_MCOMMERCE
    SKY_DeleteMWTempCertConfig(0);
	SKY_DeleteMWTempCertConfig(1);
	SKY_DeleteMWMyCertConfig(0);
	SKY_DeleteMWMyCertConfig(1);
	SKY_DeleteMWMyCertConfig(2);
	SKY_DeleteMWMyCertConfig(3);
	SKY_DeleteMWMyCertConfig(4);
	SKY_DeleteMWTCACertConfig(0);
	SKY_DeleteMWTCACertConfig(1);
	SKY_DeleteMWTCACertConfig(2);
	SKY_DeleteMWTCACertConfig(3);
	SKY_DeleteMWTCACertConfig(4);
	SKY_DeleteMWTCACertConfig(5);
	SKY_DeleteMWTCACertConfig(6);
	SKY_DeleteMWTCACertConfig(7);
	SKY_DeleteMWTCACertConfig(8);
	SKY_DeleteMWTCACertConfig(9);
	SKY_DeleteMWalletConfig();
#endif /* FEATURE_MCOMMERCE */
#ifdef FEATURE_CHARACTER_BELL
    SKY_DeleteCharacbellConfig();
#endif // FEATURE_CHARACTER_BELL

    /* initialize */
    SKY_InitMiscConfig();
    SKY_InitSysConfig();
    SKY_InitAppConfig();
#ifdef FEATURE_GVM
    SKY_InitGVMConfig();
#endif//#ifdef FEATURE_GVM
#ifdef FEATURE_MCOMMERCE
	SKY_InitMWTempCert1Config();
	SKY_InitMWTempCert2Config();
	SKY_InitMWMyCert1Config();
	SKY_InitMWMyCert2Config();
	SKY_InitMWMyCert3Config();
	SKY_InitMWMyCert4Config();
	SKY_InitMWMyCert5Config();
	SKY_InitMWTCACert1Config();
	SKY_InitMWTCACert2Config();
	SKY_InitMWTCACert3Config();
	SKY_InitMWTCACert4Config();
	SKY_InitMWTCACert5Config();
	SKY_InitMWTCACert6Config();
	SKY_InitMWTCACert7Config();
	SKY_InitMWTCACert8Config();
	SKY_InitMWTCACert9Config();
	SKY_InitMWTCACert10Config();
	SKY_InitMWalletConfig();
#endif /* FEATURE_MCOMMERCE */
#ifdef FEATURE_CHARACTER_BELL
    SKY_InitCharacbellConfig();
#endif // FEATURE_CHARACTER_BELL
}

/* 
** called by SKY 초기화 
** - 파일이 없다면 새로 만들고, 초기값을 넣어준다. 
** - 파일이 있다면, 그 내용을 각 전역 변수에 읽어 둔다. 
*/
void SKY_API SKY_InitConfig(void)
{
    /* the directory is not exist */
    if(SKY_IsFileOrDirExist(SKY_ROOT_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_ROOT_DIR);
    }
    if(SKY_IsFileOrDirExist(SKY_SYS_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_SYS_DIR);
    }

    /* initialize */
    SKY_InitExtConfig();
    SKY_InitMiscConfig();
    SKY_InitSysConfig();
    SKY_InitAppConfig();
#ifdef FEATURE_GVM
    SKY_InitGVMConfig();
#endif//#ifdef FEATURE_GVM
#ifdef FEATURE_MCOMMERCE
	SKY_InitMWTempCert1Config();
	SKY_InitMWTempCert2Config();
	SKY_InitMWMyCert1Config();
	SKY_InitMWMyCert2Config();
	SKY_InitMWMyCert3Config();
	SKY_InitMWMyCert4Config();
	SKY_InitMWMyCert5Config();
	SKY_InitMWTCACert1Config();
	SKY_InitMWTCACert2Config();
	SKY_InitMWTCACert3Config();
	SKY_InitMWTCACert4Config();
	SKY_InitMWTCACert5Config();
	SKY_InitMWTCACert6Config();
	SKY_InitMWTCACert7Config();
	SKY_InitMWTCACert8Config();
	SKY_InitMWTCACert9Config();
	SKY_InitMWTCACert10Config();
	SKY_InitMWalletConfig();
#endif /* FEATURE_MCOMMERCE */
#ifdef FEATURE_CHARACTER_BELL
    SKY_InitCharacbellConfig();
#endif // FEATURE_CHARACTER_BELL
}

/*---------------------------------------------------------------------------
**  기타 정보 파일
**  1. 데이터 파일이 성공적으로 만들어 졌는지 확인할 때 사용.
*/
LOCAL void SKY_API sInitExt(void)
{

    EXT_CONFIG_T ExtConfig = { 
        /* file header */
        {
            { 0, 0 },
            INIT_RESERVED_AREA
        },
        /* data file name that is open now */
        ""//INIT_OPEN_DATA_FILE_NAME
    };
    ExtConfig.Header.Version.nMajor = INIT_EXT_MAJOR_VERSION;
    ExtConfig.Header.Version.nMinor = INIT_EXT_MINOR_VERSION;

    /* copy the initial values */
    g_ExtConfig = ExtConfig;

    sSET_CONFIG_DIRTY(SC_EXT);
    SKY_WriteExtConfig();
}

void SKY_API SKY_InitExtConfig(void)
{
    HFILE hFile;
    DWORD dwCount;
    
    /* open the file */
    if(SKY_IsFileOrDirExist(FILE_NAME_EXT) == FALSE)
    {
        /* set the initial value */
        sInitExt();
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_EXT, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        /* if file is already exist, read the data into the global buffer */
        dwCount = SKY_ReadFile(hFile, &g_ExtConfig, sizeof(g_ExtConfig));
        //SKY_ASSERT(dwCount == sizeof(g_MiscConfig));
        if(dwCount != sizeof(g_ExtConfig))
        {
            /* some changes happened. make rebuild */
            g_ExtConfig.Header.Version.nMajor = INIT_EXT_MAJOR_VERSION+1;
        }

        /* close the file */
        SKY_CloseFile(hFile);
        /* 
        ** compare the current version with the written file version 
        ** if the file major version is changed, remove the old file and 
        ** create new data file
        */
        if(g_ExtConfig.Header.Version.nMajor != INIT_EXT_MAJOR_VERSION)
        {
            /* set the initial value */
            sInitExt();
        }
    }
}

void SKY_API SKY_DeleteExtConfig(void)
{
    SKY_DeleteFile(FILE_NAME_EXT);
}

void SKY_API SKY_WriteExtConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_EXT))
        return;
    
    /* is the file exist? */
    if(SKY_IsFileOrDirExist(FILE_NAME_EXT) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_EXT, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_EXT, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    dwCount = SKY_WriteFile(hFile, &(g_ExtConfig), 
                            sizeof(g_ExtConfig));
    
    SKY_ASSERT(dwCount == sizeof(g_ExtConfig));
    
    SKY_CloseFile(hFile);

    sRESET_CONFIG_DIRTY(SC_EXT);
}

/*---------------------------------------------------------------------------
**  부가 기능 정보 파일
*/
LOCAL void SKY_API sInitMisc(void)
{    
    MISC_CONFIG_T MiscConfig = { 
        /* file header */
        {
            { 0, 0 },
            INIT_RESERVED_AREA
        },
        /* area code */
        INIT_IS_AUTO_AREA_CODE,
        /* area code number */
        INIT_AUTO_AREA_CODE,
        /* backlight settign */
        INIT_AUTO_BACKLIGHT,
        /* backlight start and end time */
        {
            INIT_AUTO_BACKLIGHT_START_TIME,
            INIT_AUTO_BACKLIGHT_END_TIME
        },
        INIT_AUTO_BACKLIGHT,
        /* backlight start and end time */
        {
            INIT_AUTO_BACKLIGHT_START_TIME,
            INIT_AUTO_BACKLIGHT_END_TIME
        },
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
        {
            INIT_ID_LCD_IDLE,
            INIT_ID_LCD_ALERT,
            INIT_ID_LCD_SMS,
            INIT_ID_LCD_ALARM
        },
#endif
        /* auto redial */
        INIT_IS_AUTO_REDIAL,
        {
            INIT_IS_HANDS_FREE_AUTO_RECEIPT,
            INIT_IS_EAR_PHONE_AUTO_RECEIPT
        },
        /* number plus, two number service */
        {
            INIT_IS_NUMBER_PLUS_DISPLAY,
            INIT_NUMBER_PLUS_NUMBER_011,
            INIT_NUMBER_PLUS_NUMBER_017,
            { 
                INIT_BELL_ID,
                INIT_BELL_FILE_NAME
            }
        },
        {
            INIT_VOICE_POND,
	        INIT_VOICE_POND_SET,
	        INIT_VOICE_POND_IMME,
	        INIT_VOICE_POND_RESET,
	        INIT_AUTO_CONNECT_SET,
	        INIT_AUTO_CONNECT_RESET,
	        INIT_CALL_REDIR_SET,
	        INIT_CALL_REDIR_RESET,
	        INIT_INFO_POND,
	        INIT_TRAFFIC_INFO,
	        INIT_ORIG_BAN_FOREIGN,
	        INIT_ORIG_BAN_ALL,
	        INIT_ORIG_BAN_RESET,
	        INIT_CALL_FORWARD_HOT_SKT,
	        INIT_CALL_PENDING_HOT_SKT
        },
        {
            INIT_VOICE_MAIL_CONNECT,
	        INIT_VOICE_MAIL_SET,
	        INIT_VOICE_MAIL_CANCEL,
	        INIT_AUTO_CONNECT_REGISTER,
	        INIT_AUTO_CONNECT_CANCEL,
	        INIT_CALL_REDIR_REGISTER,
	        INIT_CALL_REDIR_CANCEL,
	        INIT_CALL_WAITING_REGISTER,
	        INIT_CALL_WAITING_CANCEL,
	        INIT_POWER_REMOTE,
	        INIT_POWER_MELODY,
	        INIT_CALL_FORWARD_HOT_STI,
	        INIT_CALL_PENDING_HOT_STI            
        }
    };       


    MiscConfig.Header.Version.nMajor = INIT_MISC_MAJOR_VERSION;
    MiscConfig.Header.Version.nMinor = INIT_MISC_MINOR_VERSION;

    /* copy the initial values */
    g_MiscConfig = MiscConfig;

    sSET_CONFIG_DIRTY(SC_MISC);
    SKY_WriteMiscConfig();
}



void SKY_API SKY_InitMiscConfig(void)
{
    HFILE hFile;
    DWORD dwCount;
    
    /* open the file */
    if(SKY_IsFileOrDirExist(FILE_NAME_MISC) == FALSE)
    {
        /* set the initial value */
        sInitMisc();
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MISC, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        /* if file is already exist, read the data into the global buffer */
        dwCount = SKY_ReadFile(hFile, &g_MiscConfig, sizeof(g_MiscConfig));
        //SKY_ASSERT(dwCount == sizeof(g_MiscConfig));
        if(dwCount != sizeof(g_MiscConfig))
        {
            /* some changes happened. make rebuild */
            g_MiscConfig.Header.Version.nMajor = INIT_MISC_MAJOR_VERSION+1;
        }

        /* close the file */
        SKY_CloseFile(hFile);
        /* 
        ** compare the current version with the written file version 
        ** if the file major version is changed, remove the old file and 
        ** create new data file
        */
        if(g_MiscConfig.Header.Version.nMajor != INIT_MISC_MAJOR_VERSION)
        {
            /* set the initial value */
            sInitMisc();
        }
    }
}

void SKY_API SKY_DeleteMiscConfig(void)
{
    SKY_DeleteFile(FILE_NAME_MISC);
}

void SKY_API SKY_WriteMiscConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MISC))
        return;
    
    /* is the file exist? */
    if(SKY_IsFileOrDirExist(FILE_NAME_MISC) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_MISC, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_MISC, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    dwCount = SKY_WriteFile(hFile, &(g_MiscConfig), 
                            sizeof(g_MiscConfig));
    
    SKY_ASSERT(dwCount == sizeof(g_MiscConfig));
    
    SKY_CloseFile(hFile);

    sRESET_CONFIG_DIRTY(SC_MISC);
}

/*---------------------------------------------------------------------------
**  단말기 상태 정보 파일
*/
LOCAL void SKY_API sInitSys(void)
{
    static nv_item_type    nvi;

    SYSTEM_CONFIG_T SysConfig = {
        /* header */
        {
            { 0, 0 },
            INIT_RESERVED_AREA
        },
        INIT_COUNTRY,
        INIT_IS_ETIQUETTE,
        INIT_IS_LOUD_SPEAKER,
        INIT_BANNER,
        INIT_PASSWORD,
        {
            INIT_IS_AUTO_ANSWER,
            INIT_AUTO_ANSWER_GREETING,
            INIT_AUTO_ANSWER_BELL_TIME,
            INIT_AUTO_ANSWER_COUNT
        },
        {
            INIT_IS_AUTO_LOCK,
            INIT_IS_OVERSEAS_CALL_LOCK,
            INIT_IS_PHONE_BOOK_LOCK,
            INIT_IS_MENU_LOCK,
            INIT_IS_CALL_STACK_LOCK,
            INIT_IS_RECORD_PLAY_LOCK,
            INIT_IS_MEMO_LOCK,
            INIT_IS_WAP_LOCK,
            INIT_IS_SMS_LOCK
        },
//#if (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FOLDER)
        INIT_HOW_TO_ANSWER,
//#elif (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FLIP)
        INIT_FLIP_KEY,
        INIT_MEMORY_DIAL_NUMBER,
//#endif /* FEATURE_SKY_MODEL_TYPE */
        INIT_NAM,
        INIT_CONTRAST,
        {
// WAP Use
            INIT_WAP_PROXY_URL,
            INIT_WAP_INIT_URL,
			INIT_WAP_INIT_MAIL
#ifdef FEATURE_UI_DORMANT
			,INIT_WAP_DORMANT_TIME
#endif
        },
        {
            { 
                INIT_BELL_ID,
                INIT_BELL_FILE_NAME
            },
            INIT_BELL_VIBRATION,
            INIT_BELL_VOLUME,
            INIT_BELL_TIME
        },
        {
            INIT_IDLE_DISPLAY_NAME,
            INIT_IDLE_SCREEN,
            INIT_IDLE_DISPLAY_WATCH,
            {
                INIT_IS_PICTURE_USE,
                { INIT_IDLE_PICTURE, "" },
                { INIT_INCOMING_PICTURE, "" },
                { INIT_POWER_ON_PICTURE, "" },
                { INIT_POWER_OFF_PICTURE, "" },
                { INIT_WAP_PICTURE, "" }
            }
        },
        {
            INIT_SPEAKER_VOLUME,
            INIT_KEY_TONE_VOLUME,
            INIT_MIC_VOLUME,
            INIT_GAME_VOLUME
        },
        {
            INIT_IS_CONNECT_WARNING_SIGNAL,
            INIT_IS_1MINUTE_WARNING_SIGNAL,
            INIT_IS_NOSERVICE_WARNING_SIGNAL,
            INIT_IS_LOBATT_WARNING_SIGNAL,
            INIT_IS_MESSAGE_BEEP_SOUND
        },
        {
            INIT_DATA_SERVICE_RECEIPT_MODE,
            INIT_DATA_SERVICE_INTERNET_SPEED,
            INIT_DATA_SERVICE_BPS_PC,
            {
                INIT_AIR_DATA_MANAGER_PHONE_NUMBER,
                INIT_AIR_DATA_MANAGER_IP_ADDRESS
            }
        },
        {
            INIT_EMAIL_ACCOUNT,
            INIT_EMAIL_USERNAME,
            INIT_EMAIL_USERID,
            INIT_EMAIL_PASSWORD,
            INIT_EMAIL_SMTP,
            INIT_EMAIL_POP3,
            INIT_EMAIL_USERINFO,
            INIT_EMAIL_ADDRESS,
            INIT_EMAIL_SERVER
        },
        {
            INIT_COLOR_SCHEME,
            /* rosa 2001-09-25 --> @why, what */
            //INIT_COLOR_SCHEME_BACK,
            //INIT_COLOR_SCHEME_FORE,
            /* <-- */
            INIT_FONT_TYPE,
            INIT_SCROLLBAR_SIZE
        },
        {
            INIT_DMSS_VERSION_INFO,
            INIT_EQUS_VERSION_INFO,
            INIT_SKY_VERSION_INFO
        },
        {
            INIT_SKY_LASTDIALINFO_SECRET,
            INIT_SKY_LASTDIALINFO_BUFF,    
            INIT_SKY_LASTDIALINFO_POS
        },
        INIT_SKY_CAMERA_COUNT,
        INIT_SKY_HANGUL,
        INIT_SKY_MENU_MODE,
        INIT_SKY_SMART_BATTERY,
        INIT_SKY_011_SERVICE,
        INIT_SKY_PHOTO_MAIL,
        INIT_SKY_ID_CONTRAST,
        INIT_SKY_SHORTLOCK,
        INIT_SKY_SELFINFO_LOCK,
		{
			INIT_SKY_SKVMCOLORENV,
			INIT_SKY_SKVMSETBG,
			INIT_SKY_SKVMARGC,
			{INIT_SKY_SKVMARGV}
		}
    };
    
    /* SKT, STI 통합버전에서는 시스템 초기화 후 무조건 NAM1을 Default로    */
    /* 설정해 주는 것이 아니라 Service Provider에 따라 설정되도록 바뀌었다. */
    (void) SKY_GetNV( NV_CURR_NAM_I, &nvi ); /* get current nam */
    /* If the NAM is out of range, default to the first NAM */
    if ( nvi.curr_nam >= NV_MAX_NAMS ) 
    {
        SysConfig.NAMType = CM_NAM_1;
        nvi.curr_nam = CM_NAM_1;        
        (void) SKY_PutNV ( NV_CURR_NAM_I, &nvi );
    }
    else
    {
        SysConfig.NAMType = nvi.curr_nam;
    }

    SysConfig.Header.Version.nMajor = INIT_SYS_MAJOR_VERSION;
    SysConfig.Header.Version.nMinor = INIT_SYS_MINOR_VERSION;

    g_SysConfig = SysConfig;

    sSET_CONFIG_DIRTY(SC_SYS);
    SKY_WriteSysConfig();
}

void SKY_API SKY_InitSysConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    /* open the file */
    if(SKY_IsFileOrDirExist(FILE_NAME_SYS) == FALSE)
    {
        /* set the initial value */
        sInitSys();
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_SYS, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        /* if file is already exist, read the data into the global buffer */
        dwCount = SKY_ReadFile(hFile, &(g_SysConfig), sizeof(g_SysConfig));
        //SKY_ASSERT(dwCount == sizeof(g_SysConfig));
        if(dwCount != sizeof(g_SysConfig))
        {
            /* some changes happened. make rebuild */
            g_SysConfig.Header.Version.nMajor = INIT_SYS_MAJOR_VERSION+1;
        }

        /* close the file */
        SKY_CloseFile(hFile);
        /* 
        ** compare the current version with the written file version 
        ** if the file major version is changed, remove the old file and 
        ** create new data file
        */
        if(g_SysConfig.Header.Version.nMajor != INIT_SYS_MAJOR_VERSION)
        {
            /* set the initial value */
            sInitSys();
        }
    }
}

void SKY_API SKY_DeleteSysConfig(void)
{
    SKY_DeleteFile(FILE_NAME_SYS);
}

void SKY_API SKY_WriteSysConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_SYS))
        return;

    /* is the file exist? */
    if(SKY_IsFileOrDirExist(FILE_NAME_SYS) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_SYS, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_SYS, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    
    dwCount = SKY_WriteFile(hFile, &(g_SysConfig), sizeof(g_SysConfig));

    SKY_ASSERT(dwCount == sizeof(g_SysConfig));

    SKY_CloseFile(hFile);

    sRESET_CONFIG_DIRTY(SC_SYS);
}

/*
** ***************************************************************************
** APP
** ***************************************************************************
*/
/*---------------------------------------------------------------------------
**  통화 상태 정보 파일 - 통화 시간 --> 따로 두지 말고, 애플리케이션 정보 중의 하나로..
*/
LOCAL void SKY_API sInitApp(void)
{
    /* rosa 00-12-14 --> @why, what */
    /* header */
    //SKY_FILE_HEADER_T Header = {
    //    { 0, 0 },
    //    INIT_RESERVED_AREA
    //};
    ALARM_BELL_T AlarmBell = {
        { 
            INIT_ALARM_BELL_ID,
            INIT_ALARM_BELL_FILE_NAME
        },
        {
            INIT_SCHL_ALERT_BELL_ID,
            INIT_SCHL_ALERT_BELL_FILE_NAME
        },
        INIT_ALARM_BELL_VOLUME
    };
    //ALARM_ACTIVE_E AlarmActive = INIT_ALARM_ACTIVE;

    ALARM_TIME_T AlarmTime = {
        INIT_ALARM_TIME_TYPE,
		INIT_ALARM_TIME_ACTIVE,
		INIT_ALARM_TIME_USED,
        INIT_ALARM_TIME,
        INIT_ALARM_TITLE
    };
    MEMO_T Memo = 
    {
        INIT_MEMO_TYPE,
        INIT_MEMO_TIME,
        INIT_MEMO_DATA
    };
    SMS_ALERT_T SMSAlert = {
        INIT_SMS_ALERT_TYPE,
        INIT_SMS_ALERT_BELL_ID,
        INIT_SMS_ALERT_BELL_VOLUME
    };
    CALL_TIME_T CallTime = {
        {
            INIT_VOICE_LAST_USING_TIME,
            INIT_VOICE_CUMULATION_USING_TIME,
            INIT_VOICE_TOTAL_USING_TIME
        },
        {
            INIT_WAP_LAST_USING_TIME,
            INIT_WAP_CUMULATION_USING_TIME,
            INIT_WAP_TOTAL_USING_TIME
        }
    };

	/* reo 2001.03.16 {-- */
#ifdef FEATURE_SKTT_EQUS_LOCATION
	LOCATION_INFO_T Location = {
		INIT_LASTSID,
		INIT_LASTNID,
		INIT_LASTZONE,
		INIT_LASTBASEID,
		INIT_LASTPN,
		INIT_LASTBASELAT,
		INIT_LASTBASELONG,
		INIT_RCVINFO,						// 위치통보 정보 수신 여부 결정
		{0,0,0,0,0},						// 위치확인자 ID
		INIT_ONOFFSTATE,
		INIT_STARTTIME,
		INIT_ENDTIME,
		INIT_STARTMIN,
		INIT_ENDMIN,
		INIT_CYCLE,
		INIT_LAM3VER,
		INIT_REPORTWAIT
	};		
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
/* reo 2001.03.16 --} */
#ifdef LOCATION_ORDER
    CONST DST_TIME_T INIT_DST_VALUE[] = {
      {TRUE, 3,25,10,28},      // 리스본
      {TRUE, 3,25,10,28},      // 마드리드
      {TRUE, 3,25,10,28},      // 런던
      {TRUE, 3,25,10,28},      // 파리
      {TRUE, 3,25,10,28},      // 암스테르담
      {TRUE, 3,25,10,28},      // 하노버
      {TRUE, 3,25,10,28},      // 베를린
      {TRUE, 3,25,10,28},      // 비엔나
      {TRUE, 3,25,10,28},      // 로마 
      {TRUE, 3,25,10,28},      // 헬싱키
      {TRUE, 3,25,10,28},      // 아테네
      {TRUE, 3,25,10,28},      // 베이루트 
      {TRUE, 4,27, 9,28},        // 카이로
      {TRUE, 3,25,10,28},      // 모스크바
      {TRUE, 4, 1,10, 1},      // 바그다드
      {TRUE, 3,21, 9,23},      // 테헤란
      {FALSE, 0, 0, 0, 0},      // 방콕
      {FALSE, 0, 0, 0, 0},      // 싱가포르
      {FALSE, 0, 0, 0, 0},      // 북경
      {FALSE, 0, 0, 0, 0},      // 타이페이
      {FALSE, 0, 0, 0, 0},      // 마닐라/홍콩
      {FALSE, 0, 0, 0, 0},     // 서울
      {TRUE,10,28, 3,25},      // 시드니/맬버른
      {TRUE,10, 7, 3,18},      // 웰링톤
      {FALSE, 0, 0, 0, 0},      // 사모아
      {FALSE, 0, 0, 0, 0},       // 호놀룰루 
      {TRUE, 4, 1,10,28},      // 시애틀
      {TRUE, 4, 1,10,28},      // 샌프란시스코
      {TRUE, 4, 1,10,28},      // 로스엔젤레스
      {FALSE, 0, 0, 0, 0},      // 피닉스
      {TRUE, 4, 1,10,28},      // 덴버
      {TRUE, 4, 1,10,28},      // 멕시코시티
      {TRUE, 4, 1,10,28},      // 달라스 
      {TRUE, 4, 1,10,28},      // 시카고
      {TRUE, 4, 1,10,28},      // 디트로이트 
      {TRUE, 4, 1,10,28},      // 몬트리올
      {TRUE, 4, 1,10,28},      // 워싱턴
      {TRUE, 4, 1,10,28},      // 뉴욕/마이애미
      {TRUE, 4, 1,10,28},      // 보스톤 
      {TRUE, 4, 1,10,28},      // 죠지타운
      {TRUE,10,14, 2,18}      // 리오데자네이로
    };
#else
    CONST DST_TIME_T INIT_DST_VALUE[] = {
      {FALSE, 0, 0, 0, 0},     // 서울
      {TRUE, 4, 1,10,28},      // 뉴욕/마이애미
      {TRUE, 4, 1,10,28},      // 달라스 
      {TRUE, 4, 1,10,28},      // 덴버
      {TRUE, 4, 1,10,28},      // 디트로이트 
      {TRUE, 3,25,10,28},      // 런던
      {TRUE, 3,25,10,28},      // 로마 
      {TRUE, 4, 1,10,28},      // 로스엔젤레스
      {TRUE, 3,25,10,28},      // 리스본
      {TRUE,10,14, 2,18},      // 리오데자네이로
      {FALSE, 0, 0, 0, 0},      // 마닐라/홍콩
      {TRUE, 3,25,10,28},      // 마드리드
      {TRUE, 4, 1,10,28},      // 멕시코시티
      {TRUE, 3,25,10,28},      // 모스크바
      {TRUE, 4, 1,10,28},      // 몬트리올
      {TRUE, 4, 1,10, 1},      // 바그다드
      {FALSE, 0, 0, 0, 0},      // 방콕
      {TRUE, 3,25,10,28},      // 베를린
      {TRUE, 3,25,10,28},      // 베이루트 
      {TRUE, 4, 1,10,28},      // 보스톤 
      {FALSE, 0, 0, 0, 0},      // 북경
      {TRUE, 3,25,10,28},      // 비엔나
      {FALSE, 0, 0, 0, 0},      // 사모아
      {TRUE, 4, 1,10,28},      // 샌프란시스코
      {TRUE,10,28, 3,25},      // 시드니/맬버른
      {TRUE, 4, 1,10,28},      // 시애틀
      {TRUE, 4, 1,10,28},      // 시카고
      {FALSE, 0, 0, 0, 0},      // 싱가포르
      {TRUE, 3,25,10,28},      // 아테네
      {TRUE, 3,25,10,28},      // 암스테르담
      {TRUE, 4, 1,10,28},      // 워싱턴
      {TRUE,10, 7, 3,18},      // 웰링톤
      {TRUE, 4, 1,10,28},      // 죠지타운
      {TRUE, 4,27, 9,28},        // 카이로
      {FALSE, 0, 0, 0, 0},      // 타이페이
      {TRUE, 3,21, 9,23},      // 테헤란
      {TRUE, 3,25,10,28},      // 파리
      {FALSE, 0, 0, 0, 0},      // 피닉스
      {TRUE, 3,25,10,28},      // 하노버
      {TRUE, 3,25,10,28},      // 헬싱키
      {FALSE, 0, 0, 0, 0}       // 호놀룰루 
    };
#endif
    INT8 i;

    g_AppConfig.Header.Version.nMajor = INIT_APP_MAJOR_VERSION;
    g_AppConfig.Header.Version.nMinor = INIT_APP_MINOR_VERSION;

    g_AppConfig.AlarmBell = AlarmBell;
    //g_AppConfig.AlarmActive = AlarmActive;
    for(i = 0; i < MAX_ALARM_COUNT; i++)
    {
        g_AppConfig.AlarmTime[i] = AlarmTime;
		g_AppConfig.AlarmHolidayActive[i] = INIT_ALARM_HOLIDAY_ACTIVE;
    }
    for(i = 0; i < MAX_MEMO_COUNT; i++)
    {
        g_AppConfig.Memo[i] = Memo;
    }
    /* <-- */
    g_AppConfig.SMSAlert = SMSAlert;
    g_AppConfig.CallTime = CallTime;

    for(i = 0; i < NUM_COUNTRY_NAME; i++)
    {
        g_AppConfig.DSTWorldTime[i].fOn = INIT_DST_VALUE[i].fOn;
        g_AppConfig.DSTWorldTime[i].nEndDay = INIT_DST_VALUE[i].nEndDay;
        g_AppConfig.DSTWorldTime[i].nEndMon = INIT_DST_VALUE[i].nEndMon;
        g_AppConfig.DSTWorldTime[i].nStartDay = INIT_DST_VALUE[i].nStartDay;
        g_AppConfig.DSTWorldTime[i].nStartMon = INIT_DST_VALUE[i].nStartMon;
    }

#ifdef FEATURE_SKTT_EQUS_LOCATION
	g_AppConfig.LocationInfo = Location;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

	for(i = 0; i < SKY_MAX_FL_FILE_NUM; i++)
	{
		g_AppConfig.FnSequence[i].address = i;
		g_AppConfig.FnSequence[i].Fn      = NV_MAX_FL_FILE_NUM;
		g_AppConfig.FnSequence[i].Time    = 0;
		g_AppConfig.FnSequence[i].Type    = 0;
	}

	/* 음성 통화 (발신) */
    g_AppConfig.CallTime.VoiceUsingTime.dwLast       = 0;
	g_AppConfig.CallTime.VoiceUsingTime.dwCumulation = 0;
	g_AppConfig.CallTime.VoiceUsingTime.dwTotal      = 0;

	/* 음성 통화 (착신) */
	g_AppConfig.CallTime.VoiceReceivedCallTime.dwLast       = 0;
	g_AppConfig.CallTime.VoiceReceivedCallTime.dwCumulation = 0;
	g_AppConfig.CallTime.VoiceReceivedCallTime.dwTotal      = 0;

    /* 데이터 통신 (발신) */
	g_AppConfig.CallTime.DataUsingTime.dwLast       = 0;
	g_AppConfig.CallTime.DataUsingTime.dwCumulation = 0;
	g_AppConfig.CallTime.DataUsingTime.dwTotal      = 0;

	/* 데이터 통신 (착신) */
	g_AppConfig.CallTime.DataReceivedCallTime.dwLast        =  0;
	g_AppConfig.CallTime.DataReceivedCallTime.dwCumulation  =  0;
	g_AppConfig.CallTime.DataReceivedCallTime.dwTotal       =  0;
    
    /* 녹음/재생 */
    g_AppConfig.VoiceInfo.bNewAutoAnsNumber    = 0;
    g_AppConfig.VoiceInfo.bTotalRecNumber      = 0;
    g_AppConfig.VoiceInfo.bUserVoiceFn         = NV_MAX_FL_FILE_NUM;
    g_AppConfig.VoiceInfo.dwRemainedByte       = TOTAL_REC_BYTE;
    g_AppConfig.VoiceInfo.fUserVoice           = FALSE;
    g_AppConfig.VoiceInfo.wRemainedTime        = TOTAL_REC_TIME;
    g_AppConfig.VoiceInfo.wUsedTime            = 0;
    g_AppConfig.VoiceInfo.wVoiceMemoReocrdingTime = 0;
    voice_erase_all_when_nv_setup();		// inserted by jnlee 2000.1.20

    sSET_CONFIG_DIRTY(SC_APP);
    SKY_WriteAppConfig();
}

void SKY_API SKY_InitAppConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_APP) == FALSE)
    {
        /* set the initial value */
        sInitApp();
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_APP, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        /* if file is already exist, read the data into the global buffer */
        dwCount = SKY_ReadFile(hFile, &(g_AppConfig), sizeof(g_AppConfig));
        //SKY_ASSERT(dwCount == sizeof(g_AppConfig));
        if(dwCount != sizeof(g_AppConfig))
        {
            /* some changes happened. make rebuild */
            g_AppConfig.Header.Version.nMajor = INIT_APP_MAJOR_VERSION+1;
        }

        /* close the file */
        SKY_CloseFile(hFile);
        /* 
        ** compare the current version with the written file version 
        ** if the file major version is changed, remove the old file and 
        ** create new data file
        */
        if(g_AppConfig.Header.Version.nMajor != INIT_APP_MAJOR_VERSION)
        {
            /* set the initial value */
            sInitApp();
        }
    }
}

void SKY_API SKY_DeleteAppConfig(void)
{
    SKY_DeleteFile(FILE_NAME_APP);
}

void SKY_API SKY_WriteAppConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_APP))
        return;

    /* is the file exist? */
    if(SKY_IsFileOrDirExist(FILE_NAME_APP) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_APP, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_APP, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    
    dwCount = SKY_WriteFile(hFile, &(g_AppConfig), 
                            sizeof(g_AppConfig));
    
    SKY_ASSERT(dwCount == sizeof(g_AppConfig));
    
    SKY_CloseFile(hFile);

    sRESET_CONFIG_DIRTY(SC_APP);
}

/*
** ***************************************************************************
** GVM
** ***************************************************************************
*/

#ifdef FEATURE_GVM

LOCAL void SKY_API sInitGVM(void)
{
    GVM_CHATTEXT_T      GVMChatText;
    GVM_PTP_DIAL_T      GVMPtpDial;
    
    // header
    g_GVMConfig.Header.Version.nMajor = INIT_GVM_MAJOR_VERSION;
    g_GVMConfig.Header.Version.nMinor = INIT_GVM_MINOR_VERSION;

    memset( g_GVMConfig.GVMScriptInfo, 0x00, (SIZE_T)( sizeof(GVM_SCRIPT_INFO_T) * SWAP_SSB_SIZE )); // * SWAP_SSB_SIZE ) );
    
    // chatting text
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[0], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT1) );
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[1], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT2) );
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[2], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT3) );
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[3], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT4) );
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[4], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT5) );
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[5], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT6) );
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[6], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT7) );
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[7], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT8) );
	STRCPY( (BYTE *)GVMChatText.svRsvdChatText[8], (BYTE *)SKY_GET_TOKEN(TKN_GVM_CHATTEXT9) );

    g_GVMConfig.GVMChatText = GVMChatText;

    
    // PTP Dial
    memset((BYTE *)GVMPtpDial.szPTPDial, 0x00, SWAP_LEN_DIAL+1);
    STRCPY( (BYTE *)GVMPtpDial.szPTPDial, (BYTE *)"");
    g_GVMConfig.GVMPtpDial = GVMPtpDial;

    SKY_WriteGVMConfig();
}

void SKY_API SKY_InitGVMConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

	/* 폰 부팅시 최초 한번 srand가 불려져야 한다.           */
	/* 이부분이 빠지면 일부스크립트가 제대로 동작하지 않음. */
	/* 2001.10.12 added by undersea                         */
	srand(1);
	rand();

    /* is the file exist? */
    if(SKY_IsFileOrDirExist(FILE_NAME_GVM) == FALSE)
    {
        /* set the initial value */
        sInitGVM();
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_GVM, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        /* if file is already exist, read the data into the global buffer */
        dwCount = SKY_ReadFile(hFile, &(g_GVMConfig), sizeof(g_GVMConfig));
        //SKY_ASSERT(dwCount == sizeof(g_GVMConfig));
        if(dwCount != sizeof(g_GVMConfig))
        {
            /* some changes happened. make rebuild */
            g_GVMConfig.Header.Version.nMajor = INIT_GVM_MAJOR_VERSION+1;
        }

        /* close the file */
        SKY_CloseFile(hFile);
        /* 
        ** compare the current version with the written file version 
        ** if the file major version is changed, remove the old file and 
        ** create new data file
        */
        if(g_GVMConfig.Header.Version.nMajor != INIT_GVM_MAJOR_VERSION)
        {
            /* set the initial value */
            sInitGVM();
        }
    }
}

void SKY_API SKY_DeleteGVMConfig(void)
{
    SKY_DeleteFile(FILE_NAME_GVM);
}

void SKY_API SKY_WriteGVMConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_GVM))
        return;

    /* is the file exist? */
    if(SKY_IsFileOrDirExist(FILE_NAME_GVM) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_GVM, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_GVM, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    
    dwCount = SKY_WriteFile(hFile, &(g_GVMConfig), 
                            sizeof(g_GVMConfig));
    
    SKY_ASSERT(dwCount == sizeof(g_GVMConfig));
    
    SKY_CloseFile(hFile);

    sRESET_CONFIG_DIRTY(SC_GVM);
}

#endif //#ifdef FEATURE_GVM

// ############################################################
#ifdef FEATURE_MCOMMERCE

//LOCAL void SKY_API sInitMW(void)
void SKY_API InitMWTempCert(int i)
{
	MW_TEMPCERT_INFO_T	    InitMWTempCert;

//	memset(InitMWTempCert.Length,NULL,MW_TEMPCERT_LENGTH);
	InitMWTempCert.Length = 0;
	memset(InitMWTempCert.PIN_Hash,NULL,MW_TEMPCERT_PIN_HASH);
	memset(InitMWTempCert.PRV_KEY,NULL,MW_TEMPCERT_PRV_KEY);
	memset(InitMWTempCert.SKID,NULL,MW_TEMPCERT_SKID);
	//memset(InitMWTempCert.RequestDate,NULL,MW_TEMPCERT_REQUESTDATE);
	InitMWTempCert.RequestDate = 0;
	memset(InitMWTempCert.SUBJECT_DN,NULL,MW_TEMPCERT_SUBJECT_DN);
	InitMWTempCert.KEY_USAGE = FALSE;

	if(i == 0) {
		g_MWTempCertConfig1.MWTempCert = InitMWTempCert;
		SKY_WriteMWTempCert1Config();

	} else {
		g_MWTempCertConfig2.MWTempCert = InitMWTempCert;
		SKY_WriteMWTempCert2Config();
	}
}

void SKY_API InitMWMyCert(int i)
{
	MW_MYCERT_INFO_T		 InitMWMyCert;

	InitMWMyCert.Length = 0;
	InitMWMyCert.CheckField = 0;
//	memset(InitMWMyCert.Length,NULL,MW_MYCERT_LENGTH);
//	memset(InitMWMyCert.CheckField,NULL,MW_MYCERT_CHECKFIELD);
	memset(InitMWMyCert.PIN_HASH,NULL,MW_MYCERT_PIN_HASH);
	memset(InitMWMyCert.PRV_KEY,NULL,MW_MYCERT_PRV_KEY);
	memset(InitMWMyCert.DISPLAY,NULL,MW_MYCERT_DISPLAY);
	memset(InitMWMyCert.SUBJECT_KID,NULL,MW_MYCERT_SUBJECT_KID);
	memset(InitMWMyCert.ISSUER_KID,NULL,MW_MYCERT_ISSUER_KID);
	memset(InitMWMyCert.CERT_URL,NULL,MW_MYCERT_CERT_URL);
	memset(InitMWMyCert.SUBJECT_DN,NULL,MW_MYCERT_SUBJECT_DN);

	if(i == 0) {
		g_MWMYCertConfig1.MWMyCert = InitMWMyCert;
		SKY_WriteMWMyCert1Config();
	} else if(i ==1) {
		g_MWMYCertConfig2.MWMyCert = InitMWMyCert;
		SKY_WriteMWMyCert2Config();
	} else if (i ==2) {
		g_MWMYCertConfig3.MWMyCert = InitMWMyCert;
		SKY_WriteMWMyCert3Config();
	} else if (i ==3) {
		g_MWMYCertConfig4.MWMyCert = InitMWMyCert;
		SKY_WriteMWMyCert4Config();
	} else {
		g_MWMYCertConfig5.MWMyCert = InitMWMyCert;
		SKY_WriteMWMyCert5Config();
	}
}



void SKY_API InitMWTCACert(int i)
{
    HFILE hFile;
    MW_TCACERT_INFO_T		 InitMWTCACert;

	BYTE nesignskid[] = { 0x51, 0xC0, 0x5C, 0xE5, 0xEF, 0xCC, 0x5E, 0xEC, 0x68, 0x20, 0xBF, 
									  0xFB, 0x6B, 0x14, 0x35, 0xC1, 0x27, 0x12, 0x88, 0x27 };
	BYTE kikaskid[] = { 0x5B, 0x79, 0xB5, 0xCA, 0x04, 0x03, 0x85, 0xF1, 0x44, 0xD6,
								  0x3B, 0xE9, 0xC7, 0x63, 0x50, 0x4C, 0x2D, 0xA2, 0xC1, 0xB0 };
	BYTE signkoreaskid[] = { 0xEB, 0xBC, 0x86, 0x3D, 0xBF, 0x8B, 0x03, 0xF9, 0x8B,
										   0xC2, 0x08, 0x80, 0xF8, 0x08, 0xD1, 0x51, 0x4D, 0xB9, 0xC4, 0x99 };

	BYTE nesigncert[] = {0x01, 0x02, 0x01, 0x00, 0x04, 0x23, 0x6d, 0x6f, 0x62, 0x69,
									 0x6c, 0x65, 0x43, 0x41, 0x53, 0x65, 0x72, 0x76, 0x3b, 0x20,
									 0x53, 0x4b, 0x54, 0x65, 0x6c, 0x65, 0x63, 0x6f, 0x6d, 0x3b,
									 0x20, 0x4b, 0x52, 0x3b, 0x20, 0x4e, 0x65, 0x53, 0x69, 0x67,
									 0x6e, 0x3b, 0xad, 0xf8, 0xf0, 0x4e, 0x7c, 0x9e, 0xef, 0x01,
									 0x00, 0x04, 0x23, 0x6d, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x43,
									 0x41, 0x53, 0x65, 0x72, 0x76, 0x3b, 0x20, 0x53, 0x4b, 0x54,
									 0x65, 0x6c, 0x65, 0x63, 0x6f, 0x6d, 0x3b, 0x20, 0x4b, 0x52,
									 0x3b, 0x20, 0x4e, 0x65, 0x53, 0x69, 0x67, 0x6e, 0x02, 0x00,
									 0x00, 0x03, 0x01, 0x00, 0x01, 0x00, 0x80, 0xcd, 0xc9, 0x1b,
									 0xc1, 0xba, 0x9a, 0x1b, 0x9f, 0x37, 0xcf, 0xbe, 0x0c, 0x97,
									 0x56, 0x2e, 0x65, 0x6a, 0x93, 0x5d, 0xcb, 0x33, 0xdd, 0x70,
									 0x90, 0x09, 0xc1, 0xe7, 0x08, 0x8c, 0x2e, 0x83, 0x06, 0x59,
									 0xc7, 0x25, 0x0b, 0x43, 0x5b, 0x6a, 0x50, 0xea, 0x06, 0x3b,
									 0x13, 0x62, 0xaf, 0x4c, 0x79, 0xb5, 0x2a, 0xf9, 0xe9, 0xd5,
									 0x76, 0x8c, 0x45, 0x0b, 0x9a, 0x46, 0x46, 0x1a, 0x6d, 0x73,
									 0x5c, 0xe7, 0xa2, 0x83, 0x92, 0xbc, 0xdb, 0xb8, 0x4b, 0xc5,
									 0xf5, 0x43, 0xc6, 0xd7, 0x0c, 0x69, 0xe7, 0x97, 0x9e, 0x05,
									 0x78, 0xb8, 0x00, 0xf0, 0x28, 0x9d, 0x18, 0xcf, 0x64, 0x20,
									 0xf5, 0xbc, 0x88, 0xd3, 0x5f, 0x4f, 0xb2, 0xe0, 0x93, 0xb9,
									 0xa3, 0x1c, 0x2a, 0x42, 0x5b, 0x3f, 0xdf, 0x6c, 0x57, 0xd3,
									 0xc3, 0xbb, 0x1b, 0x2b, 0x2b, 0xd8, 0xf8, 0xed, 0x47, 0x0c,
									 0x41, 0x1b, 0xaa, 0xb5, 0xa9, 0x00, 0x80, 0x2e, 0x8e, 0x6c,
									 0x60, 0x2d, 0x0b, 0xca, 0x89, 0x1e, 0x5f, 0x2f, 0x8b, 0xf5,
									 0x73, 0x6d, 0x45, 0x0a, 0xa2, 0x3f, 0xbb, 0x23, 0x8b, 0x4a,
									 0xfa, 0xd8, 0xa9, 0xc3, 0x15, 0x45, 0x54, 0x0e, 0x86, 0x4d,
									 0xf5, 0x9b, 0x72, 0xc5, 0x40, 0x1b, 0x8b, 0x6d, 0x74, 0xdb,
									 0xc9, 0x01, 0xd5, 0xb7, 0x38, 0x1a, 0x79, 0x23, 0x09, 0x73,
									 0xb3, 0x4a, 0xf1, 0xa7, 0x1a, 0x68, 0x26, 0x5d, 0x16, 0x21,
									 0xfc, 0x8b, 0x91, 0xa3, 0x6a, 0x7b, 0xa0, 0x4e, 0xf0, 0x7f,
									 0x9c, 0xc2, 0x73, 0x3c, 0xb0, 0x60, 0xb1, 0x4b, 0x04, 0x90,
									 0x02, 0x39, 0x4b, 0x99, 0x09, 0x41, 0x36, 0xbb, 0x9a, 0x6f,
									 0xc5, 0xfa, 0xca, 0xd8, 0x10, 0x92, 0x20, 0x14, 0x8d, 0x53,
									 0xa2, 0xbc, 0x2d, 0xc2, 0xf4, 0xa5, 0x5c, 0xe5, 0xfa, 0x07,
									 0x09, 0x3c, 0x99, 0x9f, 0xb2, 0xee, 0x8a, 0x21, 0x03, 0x8a,
									 0x31, 0x8f, 0xc3, 0x8b, 0x90 };

	BYTE kikacert[] = {0x01, 0x02, 0x01, 0x00, 0x04, 0x32, 0x6d, 0x6f, 0x62, 0x69,
								 0x6c, 0x65, 0x43, 0x41, 0x3b, 0x20, 0x4b, 0x49, 0x43, 0x41,
								 0x2d, 0x53, 0x4b, 0x54, 0x65, 0x6c, 0x65, 0x63, 0x6f, 0x6d,
								 0x3b, 0x20, 0x4b, 0x52, 0x3b, 0x20, 0x6d, 0x6f, 0x62, 0x69,
								 0x6c, 0x65, 0x43, 0x41, 0x66, 0x6f, 0x72, 0x53, 0x4b, 0x54,
								 0x65, 0x6c, 0x65, 0x63, 0x6f, 0x6d, 0x3b, 0xad, 0xf8, 0xf0,
								 0x4e, 0x7c, 0x9e, 0xef, 0x01, 0x00, 0x04, 0x32, 0x6d, 0x6f,
								 0x62, 0x69, 0x6c, 0x65, 0x43, 0x41, 0x3b, 0x20, 0x4b, 0x49,
								 0x43, 0x41, 0x2d, 0x53, 0x4b, 0x54, 0x65, 0x6c, 0x65, 0x63,
								 0x6f, 0x6d, 0x3b, 0x20, 0x4b, 0x52, 0x3b, 0x20, 0x6d, 0x6f,
								 0x62, 0x69, 0x6c, 0x65, 0x43, 0x41, 0x66, 0x6f, 0x72, 0x53,
								 0x4b, 0x54, 0x65, 0x6c, 0x65, 0x63, 0x6f, 0x6d, 0x02, 0x00,
								 0x00, 0x03, 0x01, 0x00, 0x01, 0x00, 0x80, 0xb8, 0x87, 0x55,
								 0x00, 0xa9, 0xdc, 0x67, 0xd7, 0xef, 0xb5, 0x86, 0xa4, 0x4e,
								 0x33, 0x79, 0xbb, 0x6f, 0xfc, 0x77, 0x68, 0xf1, 0xfe, 0xe3,
								 0x84, 0x8a, 0x78, 0x8a, 0x78, 0x2e, 0xf1, 0xdb, 0xea, 0x52,
								 0x19, 0xd5, 0xc5, 0x12, 0x73, 0x1d, 0x3c, 0xdc, 0x02, 0x67,
								 0x6e, 0x54, 0x43, 0x42, 0x8e, 0x9b, 0x75, 0xe0, 0xf4, 0x7b,
								 0x12, 0x11, 0x43, 0x14, 0x18, 0xba, 0x28, 0xe1, 0x69, 0xbd,
								 0x23, 0xc3, 0xfc, 0xf7, 0xc9, 0x26, 0x76, 0x2a, 0x0c, 0x30,
								 0x3e, 0xfd, 0x6f, 0x87, 0x8c, 0xf9, 0x7e, 0xff, 0xd5, 0x9c,
								 0x39, 0xf8, 0xa2, 0x73, 0x8e, 0x1f, 0x8b, 0x00, 0xb0, 0x15,
								 0x6e, 0x6a, 0x3c, 0xc5, 0x53, 0x6b, 0x1c, 0xd1, 0x8d, 0xc4,
								 0x7a, 0x01, 0xcd, 0x61, 0x12, 0xfa, 0x14, 0x85, 0x28, 0xa2,
								 0x16, 0x65, 0x61, 0xb5, 0xba, 0xbb, 0xb5, 0x53, 0x73, 0x8a,
								 0x1f, 0xbb, 0x5a, 0xf4, 0x73, 0x00, 0x80, 0x83, 0x7e, 0x47,
								 0xf4, 0xcc, 0xa3, 0x8e, 0xdf, 0xdb, 0x46, 0x58, 0x20, 0xb4,
								 0xf6, 0xaf, 0x28, 0xb8, 0xd8, 0xc8, 0x17, 0xf5, 0xb6, 0xd8,
								 0x7d, 0xf0, 0x0c, 0x4f, 0xd6, 0x97, 0xfc, 0x4c, 0xe3, 0x10,
								 0x22, 0xc9, 0x0d, 0x25, 0xfe, 0x4b, 0x65, 0xb2, 0x58, 0x9a,
								 0x99, 0x75, 0x3a, 0x7f, 0xe3, 0x62, 0xf7, 0xe6, 0xe7, 0x0b,
								 0xc8, 0x3d, 0x3c, 0x37, 0x5a, 0x82, 0x9e, 0x11, 0xe0, 0x24,
								 0x64, 0x0f, 0xe4, 0xe3, 0x60, 0xcd, 0xd0, 0xca, 0x5b, 0x50,
								 0x1e, 0xe1, 0x76, 0xaa, 0x8d, 0x7c, 0xfb, 0xd7, 0x7b, 0xe3,
								 0x43, 0x48, 0xd3, 0x1c, 0x1f, 0xe3, 0xd2, 0xba, 0x84, 0x76,
								 0xb7, 0x45, 0xf0, 0x8c, 0x36, 0x7c, 0x08, 0xb9, 0x50, 0x71,
								 0x5a, 0x8a, 0x18, 0x4d, 0x5e, 0x02, 0x84, 0x13, 0x81, 0x79,
								 0xd8, 0x26, 0x07, 0xab, 0xca, 0x08, 0xc5, 0xc5, 0x89, 0x09,
								 0xa9, 0x39, 0xa3, 0x42, 0xb9 };

	BYTE signkoreacert[] = {0x01, 0x02, 0x01, 0x00, 0x04, 0x37, 0x6d, 0x6f, 0x62, 0x69,
										 0x6c, 0x65, 0x43, 0x41, 0x3b, 0x20, 0x53, 0x69, 0x67, 0x6e,
										 0x4b, 0x6f, 0x72, 0x65, 0x61, 0x2d, 0x53, 0x4b, 0x54, 0x65,
										 0x6c, 0x65, 0x63, 0x6f, 0x6d, 0x3b, 0x20, 0x4b, 0x52, 0x3b,
										 0x20, 0x6d, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x43, 0x41, 0x66,
										 0x6f, 0x72, 0x53, 0x4b, 0x54, 0x65, 0x6c, 0x65, 0x63, 0x6f,
										 0x6d, 0x3b, 0xad, 0xf8, 0xf0, 0x4e, 0x7c, 0x9e, 0xef, 0x01,
										 0x00, 0x04, 0x37, 0x6d, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x43,
										 0x41, 0x3b, 0x20, 0x53, 0x69, 0x67, 0x6e, 0x4b, 0x6f, 0x72,
										 0x65, 0x61, 0x2d, 0x53, 0x4b, 0x54, 0x65, 0x6c, 0x65, 0x63,
										 0x6f, 0x6d, 0x3b, 0x20, 0x4b, 0x52, 0x3b, 0x20, 0x6d, 0x6f,
										 0x62, 0x69, 0x6c, 0x65, 0x43, 0x41, 0x66, 0x6f, 0x72, 0x53,
										 0x4b, 0x54, 0x65, 0x6c, 0x65, 0x63, 0x6f, 0x6d, 0x02, 0x00,
										 0x00, 0x03, 0x01, 0x00, 0x01, 0x00, 0x80, 0xae, 0xf1, 0xda,
										 0x1b, 0x9b, 0xe9, 0xda, 0x08, 0x51, 0x84, 0x96, 0xaa, 0x99,
										 0xd3, 0x10, 0x27, 0xd2, 0xb0, 0x34, 0x02, 0x74, 0x2c, 0xcd,
										 0x11, 0xe1, 0x0c, 0xbf, 0x1a, 0xf5, 0x3b, 0x42, 0xc1, 0x7e,
										 0x31, 0x1b, 0x9a, 0x83, 0xb5, 0x9b, 0x4e, 0xa4, 0x45, 0xc1,
										 0xca, 0x42, 0x39, 0x6c, 0xef, 0x4b, 0x0a, 0xa3, 0xbc, 0x73,
										 0xbc, 0x44, 0xff, 0xdd, 0x2a, 0x8b, 0xc8, 0x99, 0x86, 0x78,
										 0xb4, 0x5e, 0xcb, 0xb9, 0xc1, 0x89, 0x2a, 0xb1, 0x54, 0x9a,
										 0x76, 0x16, 0x9d, 0x6e, 0xd3, 0x0a, 0x7d, 0xdb, 0x30, 0xe0,
										 0xa0, 0x72, 0xa7, 0xba, 0xcb, 0x5b, 0xe8, 0x59, 0xb1, 0xba,
										 0x1f, 0x13, 0x51, 0x41, 0x5a, 0x0e, 0x79, 0x2e, 0xf7, 0x67,
										 0x54, 0xa6, 0x35, 0xfc, 0xc5, 0x79, 0x96, 0x1e, 0x12, 0x78,
										 0x14, 0x75, 0x5e, 0x14, 0xd9, 0x87, 0x07, 0x24, 0x6e, 0x42,
										 0x9f, 0x94, 0x42, 0x7a, 0x05, 0x00, 0x80, 0x99, 0x16, 0x72,
										 0xcb, 0x79, 0xcf, 0x09, 0x72, 0xb9, 0x1d, 0x66, 0xee, 0x09,
										 0x33, 0x07, 0xb6, 0xcc, 0x41, 0xf0, 0x84, 0xc5, 0xb2, 0xf3,
										 0x7b, 0x4c, 0xdf, 0x88, 0xbe, 0x17, 0xca, 0xcc, 0xce, 0x8c,
										 0x1f, 0x1d, 0x84, 0x0d, 0x58, 0x49, 0xe6, 0xcf, 0xb2, 0xf6,
										 0x8f, 0xa8, 0x9c, 0xe3, 0xa8, 0xa9, 0x70, 0x05, 0x7d, 0x83,
										 0x2e, 0x55, 0xf4, 0x0a, 0x74, 0xaf, 0xe4, 0x12, 0x6b, 0x64,
										 0x47, 0x31, 0x94, 0x82, 0xab, 0xe8, 0xd7, 0x58, 0x45, 0x06,
										 0x12, 0x8c, 0x25, 0x22, 0x87, 0x85, 0xba, 0xc7, 0x51, 0xc9,
										 0xf6, 0x07, 0x15, 0x26, 0x73, 0x8d, 0x2e, 0xb4, 0xc4, 0x2f,
										 0xb5, 0xeb, 0x67, 0x92, 0x38, 0x12, 0x83, 0xe7, 0x0c, 0xfe,
										 0x50, 0x2f, 0x58, 0x89, 0xbc, 0x48, 0x82, 0xb2, 0xbe, 0x8f,
										 0xf5, 0x73, 0x57, 0x8a, 0x36, 0x72, 0x4e, 0xf2, 0xda, 0xbf,
										 0xa8, 0x1d, 0xab, 0x71, 0x90 };

	// MWTCACERT 	 default TCA 정보 설치 부분..
	if (i == 0) {  // SK 텔레콤(neSign.wtls)
		InitMWTCACert.Length = 491;
		InitMWTCACert.CheckField = 0;
//		memset(InitMWTCACert.Length,NULL,MW_TCACERT_LENGTH);
//		memset(InitMWTCACert.CheckField,NULL,MW_TCACERT_CHECKFIELD);
		memcpy(InitMWTCACert.DISPLAY,"SK 텔레콤;2011년 09월 23일;인증서 발급용",40);
		memcpy(InitMWTCACert.SUBJECT_KID,nesignskid,20);
		memcpy(InitMWTCACert.ISSUER_KID,nesignskid,20);
		// nesign.wtls
		memcpy(InitMWTCACert.CERT,nesigncert,355);
		memcpy(InitMWTCACert.CA_INFO_URL,"www.neSign.co.kr",MW_TCACERT_CA_INFO_URL);
		memcpy(InitMWTCACert.SUBJECT_DN,"mobileCAServ; SKTelecom; KR; NeSign",MW_TCACERT_SUBJECT_DN);
		//memset(InitMWTCACert.NOT_AFTER,NULL,MW_TCACERT_NOT_AFTER);
		InitMWTCACert.NOT_AFTER = 1316789999;

		g_MWTCACertConfig1.MWTCACert = InitMWTCACert;
		SKY_WriteMWTCACert1Config();
	} else if ( i == 1) {    // 한국정보인증(kica.wtls)
		InitMWTCACert.Length = 540;
		InitMWTCACert.CheckField = 0;
//		memset(InitMWTCACert.Length,NULL,MW_TCACERT_LENGTH);
//		memset(InitMWTCACert.CheckField,NULL,MW_TCACERT_CHECKFIELD);
		memcpy(InitMWTCACert.DISPLAY,"한국정보인증;2011년09월23일;인증서 발급용",41);
		memcpy(InitMWTCACert.SUBJECT_KID,kikaskid,20);
		memcpy(InitMWTCACert.ISSUER_KID,kikaskid,20);
		memcpy(InitMWTCACert.CERT,kikacert,385); // kica.wtls

		memcpy(InitMWTCACert.CA_INFO_URL,"www.neSign.co.kr",16);
		memcpy(InitMWTCACert.SUBJECT_DN,"mobileCA; KICA-SKTelecom; KR; mobileCAforSKTelecom",50);
		//memset(InitMWTCACert.NOT_AFTER,NULL,MW_TCACERT_NOT_AFTER);
		InitMWTCACert.NOT_AFTER = 1316789999;

		g_MWTCACertConfig2.MWTCACert = InitMWTCACert;
		SKY_WriteMWTCACert2Config();
		
	} else if (i == 2) {     // 한국증권전산(signkorea.wtls)
		InitMWTCACert.Length = 555;
		InitMWTCACert.CheckField = 0;
//		memset(InitMWTCACert.Length,NULL,MW_TCACERT_LENGTH);
//		memset(InitMWTCACert.CheckField,NULL,MW_TCACERT_CHECKFIELD);
		memcpy(InitMWTCACert.DISPLAY,"한국증권전산;2011년09월23일;인증서 발급용",41);
		memcpy(InitMWTCACert.SUBJECT_KID,signkoreaskid,20);
		memcpy(InitMWTCACert.ISSUER_KID,signkoreaskid,20);
		memcpy(InitMWTCACert.CERT,signkoreacert,395); // signkorea.wtls
		memcpy(InitMWTCACert.CA_INFO_URL,"www.neSign.co.kr",16);
		memcpy(InitMWTCACert.SUBJECT_DN,"mobileCA; SignKorea-SKTelecom; KR; mobileCAforSKTelecom",55);
		//memset(InitMWTCACert.NOT_AFTER,NULL,MW_TCACERT_NOT_AFTER);
		InitMWTCACert.NOT_AFTER = 1316789999;

		g_MWTCACertConfig3.MWTCACert = InitMWTCACert;
		SKY_WriteMWTCACert3Config();

	} else {
		InitMWTCACert.Length = 0;
		InitMWTCACert.CheckField = 0;
//		memset(InitMWTCACert.Length,NULL,MW_TCACERT_LENGTH);
//		memset(InitMWTCACert.CheckField,NULL,MW_TCACERT_CHECKFIELD);
		memset(InitMWTCACert.DISPLAY,NULL,MW_TCACERT_DISPLAY);
		memset(InitMWTCACert.SUBJECT_KID,NULL,MW_TCACERT_SUBJECT_KID);
		memset(InitMWTCACert.ISSUER_KID,NULL,MW_TCACERT_ISSUER_KID);
		memset(InitMWTCACert.CERT,NULL,MW_TCACERT_CERT);
		memset(InitMWTCACert.CA_INFO_URL,NULL,MW_TCACERT_CA_INFO_URL);
		memset(InitMWTCACert.SUBJECT_DN,NULL,MW_TCACERT_SUBJECT_DN);
		//memset(InitMWTCACert.NOT_AFTER,NULL,MW_TCACERT_NOT_AFTER);
		InitMWTCACert.NOT_AFTER = 0;

/*		if(i == 1) {
			g_MWTCACertConfig2.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert2Config();
		} else if (i ==2) {
			g_MWTCACertConfig3.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert3Config();
		} else*/
		if (i ==3) {
			g_MWTCACertConfig4.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert4Config();
		} else if (i ==4) {
			g_MWTCACertConfig5.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert5Config();
		} else if (i == 5) {
			g_MWTCACertConfig6.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert6Config();
		} else if (i == 6) {
			g_MWTCACertConfig7.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert7Config();
		} else if (i == 7) {
			g_MWTCACertConfig8.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert8Config();
		} else if (i == 8) {
			g_MWTCACertConfig9.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert9Config();
		} else if (i == 9) {
			g_MWTCACertConfig10.MWTCACert = InitMWTCACert;
			SKY_WriteMWTCACert10Config();
		}
	}
}

void SKY_API InitMWallet(int cert)
{
    UINT8   i;
    MW_WALLET_T				     InitMWWallet;

	// MW_WALLET
	memcpy(InitMWWallet.HashedPin,(const void*)wwSetHashedPIN((char *)"0000"),MW_WALLET_HASHEDPIN_LENGTH);
	memset(InitMWWallet.user,NULL,MW_WALLET_USER_LENGTH);
	memset(InitMWWallet.card[0],NULL,MW_WALLET_CARD_LENGTH);
	memset(InitMWWallet.card[1],NULL,MW_WALLET_CARD_LENGTH);
	memset(InitMWWallet.card[2],NULL,MW_WALLET_CARD_LENGTH);
	memset(InitMWWallet.ocb,NULL,MW_WALLET_OCB_LENGTH);
	memset(InitMWWallet.delivery[0],NULL,MW_WALLET_DELIVERY_LENGTH);
	memset(InitMWWallet.delivery[1],NULL,MW_WALLET_DELIVERY_LENGTH);
	memset(InitMWWallet.dCardiv,'0',MW_WALLET_DCARDIV_LENGTH);
	memset(InitMWWallet.dOcbiv,'0',MW_WALLET_DOCBIV_LENGTH);
	
	InitMWWallet.trial_num = 0;
	InitMWWallet.user_length = 0; 
	InitMWWallet.card_length = 0;
	InitMWWallet.ocb_length = 0;
	InitMWWallet.delivery_length = 0;

	// Wallet 정보
	for(i = 0; i < 7 ; i++)	InitMWWallet.WalletDataSaved[i] = 0; 

	if(cert) {
		// TempCert정보
		for(i = 0; i < MW_TEMPCERT_NUM ; i++)	InitMWWallet.TempCertDataSaved[i] = 0; 
		// MyCert 정보
		for(i = 0; i < MW_MYCERT_NUM ; i++)	InitMWWallet.MyCertDataSaved[i] = 0; 
		// TcaCert정보
		// TCA정보 한개는 default로 설치되므로..
		InitMWWallet.TCACertDataSaved[0] = 1;
		InitMWWallet.TCACertDataSaved[1] = 2;
		InitMWWallet.TCACertDataSaved[2] = 3;
		for(i = 3; i < MW_TCACERT_NUM ; i++)	InitMWWallet.TCACertDataSaved[i] = 0; 
	} else {
		for(i = 0; i < MW_TEMPCERT_NUM ; i++)
			InitMWWallet.TempCertDataSaved[i] = SKY_GET_MW_TEMPCERT_DATA_SAVED(i);
		for(i = 0; i < MW_MYCERT_NUM ; i++)
			InitMWWallet.MyCertDataSaved[i] = SKY_GET_MW_MYCERT_DATA_SAVED(i);
		InitMWWallet.TCACertDataSaved[0] = 1;
		InitMWWallet.TCACertDataSaved[1] = 2;
		InitMWWallet.TCACertDataSaved[2] = 3;
		for(i = 3; i < MW_TCACERT_NUM ; i++)
			InitMWWallet.TCACertDataSaved[i] = SKY_GET_MW_TCACERT_DATA_SAVED(i);
	}

	g_MWWalletConfig.MWWallet = InitMWWallet;
	
	SKY_WriteMWalletConfig();
}

void SKY_API SKY_InitMWTempCert1Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TEMPCERT1) == FALSE)  
    {
        /* set the initial value */
		InitMWTempCert(0);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TempCert
		hFile = SKY_OpenFile(FILE_NAME_MW_TEMPCERT1, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTempCertConfig1), sizeof(g_MWTempCertConfig1));
        SKY_CloseFile(hFile);
	}
}

void SKY_API SKY_InitMWTempCert2Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TEMPCERT2) == FALSE)  
    {
        /* set the initial value */
		InitMWTempCert(1);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TempCert
		hFile = SKY_OpenFile(FILE_NAME_MW_TEMPCERT2, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTempCertConfig2), sizeof(g_MWTempCertConfig2));
        SKY_CloseFile(hFile);
	}
}

void SKY_API SKY_InitMWMyCert1Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT1) == FALSE)  
    {
        /* set the initial value */
		InitMWMyCert(0);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// MyCert
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT1, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWMYCertConfig1), sizeof(g_MWMYCertConfig1));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWMyCert2Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT2) == FALSE)  
    {
        /* set the initial value */
		InitMWMyCert(1);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// MyCert
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT2, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWMYCertConfig2), sizeof(g_MWMYCertConfig2));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWMyCert3Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT3) == FALSE)  
    {
        /* set the initial value */
		InitMWMyCert(2);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// MyCert
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT3, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWMYCertConfig3), sizeof(g_MWMYCertConfig3));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWMyCert4Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT4) == FALSE)  
    {
        /* set the initial value */
		InitMWMyCert(3);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// MyCert
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT4, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWMYCertConfig4), sizeof(g_MWMYCertConfig4));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWMyCert5Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT5) == FALSE)  
    {
        /* set the initial value */
		InitMWMyCert(4);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// MyCert
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT5, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWMYCertConfig5), sizeof(g_MWMYCertConfig5));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert1Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT1) == FALSE)  
    {
		InitMWTCACert(0);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT1, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig1), sizeof(g_MWTCACertConfig1));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert2Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT2) == FALSE)  
    {
		InitMWTCACert(1);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT2, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig2), sizeof(g_MWTCACertConfig2));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert3Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT3) == FALSE)  
    {
		InitMWTCACert(2);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT3, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig3), sizeof(g_MWTCACertConfig3));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert4Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT4) == FALSE)  
    {
		InitMWTCACert(3);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT4, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig4), sizeof(g_MWTCACertConfig4));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert5Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT5) == FALSE)  
    {
		InitMWTCACert(4);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT5, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig5), sizeof(g_MWTCACertConfig5));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert6Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT6) == FALSE)  
    {
		InitMWTCACert(5);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT6, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig6), sizeof(g_MWTCACertConfig6));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert7Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT7) == FALSE)  
    {
		InitMWTCACert(6);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT7, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig7), sizeof(g_MWTCACertConfig7));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert8Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT8) == FALSE)  
    {
		InitMWTCACert(7);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT8, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig8), sizeof(g_MWTCACertConfig8));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert9Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT9) == FALSE)  
    {
		InitMWTCACert(8);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT9, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig9), sizeof(g_MWTCACertConfig9));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWTCACert10Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT10) == FALSE)  
    {
		InitMWTCACert(9);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// TCACert
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT10, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWTCACertConfig10), sizeof(g_MWTCACertConfig10));
        SKY_CloseFile(hFile);
    } 
}

void SKY_API SKY_InitMWalletConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MW_WALLET) == FALSE)  
    {
        /* set the initial value */
		InitMWallet(1);
    }
    else
    {  // 버젼 관리는 하지 않는다.
		// MWallet
		hFile = SKY_OpenFile(FILE_NAME_MW_WALLET, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
		dwCount = SKY_ReadFile(hFile, &(g_MWWalletConfig), sizeof(g_MWWalletConfig));
        SKY_CloseFile(hFile);
	}
}

void SKY_API SKY_DeleteMWTempCertConfig(int i)
{
	if (i == 0) {
		SKY_DeleteFile(FILE_NAME_MW_TEMPCERT1);
	} else {
		SKY_DeleteFile(FILE_NAME_MW_TEMPCERT2);
	}
}

void SKY_API SKY_DeleteMWMyCertConfig(int i)
{
	if (i ==0) {
		SKY_DeleteFile(FILE_NAME_MW_MYCERT1);
	} else if (i == 1) {
		SKY_DeleteFile(FILE_NAME_MW_MYCERT2);
	} else if (i ==2) {
		SKY_DeleteFile(FILE_NAME_MW_MYCERT3);
	} else if(i==3) {
		SKY_DeleteFile(FILE_NAME_MW_MYCERT4);
	} else if (i ==4) {
		SKY_DeleteFile(FILE_NAME_MW_MYCERT5);
	}
}

void SKY_API SKY_DeleteMWTCACertConfig(int i)
{
	if (i == 0) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT1);
	} else if (i ==1 ) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT2);
	} else if (i==2) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT3);
	} else if(i ==3 ) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT4);
	} else if(i==4) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT5);
	} else if(i==5) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT6);
	} else if(i ==6 ) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT7);
	} else if(i ==7) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT8);
	} else if(i == 8) {
		SKY_DeleteFile(FILE_NAME_MW_TCACERT9);
	} else if (i == 9 ){
		SKY_DeleteFile(FILE_NAME_MW_TCACERT10);
	}
}

void SKY_API SKY_DeleteMWalletConfig(void)
{
	SKY_DeleteFile(FILE_NAME_MW_WALLET);
}

void SKY_API SKY_WriteMWTempCert1Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TEMPCERT1))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TEMPCERT1) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TEMPCERT1, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TEMPCERT1, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTempCertConfig1), sizeof(g_MWTempCertConfig1));
	SKY_ASSERT(dwCount == sizeof(g_MWTempCertConfig1));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TEMPCERT1);
}

void SKY_API SKY_WriteMWTempCert2Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TEMPCERT2))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TEMPCERT2) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TEMPCERT2, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TEMPCERT2, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTempCertConfig2), sizeof(g_MWTempCertConfig2));
	SKY_ASSERT(dwCount == sizeof(g_MWTempCertConfig2));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TEMPCERT2);
}

void SKY_API SKY_WriteMWMyCert1Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_MYCERT1))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT1) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT1, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT1, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWMYCertConfig1), sizeof(g_MWMYCertConfig1));
	SKY_ASSERT(dwCount == sizeof(g_MWMYCertConfig1));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_MYCERT1);
}

void SKY_API SKY_WriteMWMyCert2Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_MYCERT2))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT2) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT2, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT2, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWMYCertConfig2), sizeof(g_MWMYCertConfig2));
	SKY_ASSERT(dwCount == sizeof(g_MWMYCertConfig2));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_MYCERT2);
}

void SKY_API SKY_WriteMWMyCert3Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_MYCERT3))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT3) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT3, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT3, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWMYCertConfig3), sizeof(g_MWMYCertConfig3));
	SKY_ASSERT(dwCount == sizeof(g_MWMYCertConfig3));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_MYCERT3);
}

void SKY_API SKY_WriteMWMyCert4Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_MYCERT4))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT4) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT4, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT4, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWMYCertConfig4), sizeof(g_MWMYCertConfig4));
	SKY_ASSERT(dwCount == sizeof(g_MWMYCertConfig4));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_MYCERT4);
}

void SKY_API SKY_WriteMWMyCert5Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_MYCERT5))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_MYCERT5) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT5, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_MYCERT5, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWMYCertConfig5), sizeof(g_MWMYCertConfig5));
	SKY_ASSERT(dwCount == sizeof(g_MWMYCertConfig5));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_MYCERT5);
}

void SKY_API SKY_WriteMWTCACert1Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT1))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT1) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT1, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT1, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig1), sizeof(g_MWTCACertConfig1));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig1));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT1);
}

void SKY_API SKY_WriteMWTCACert2Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT2))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT2) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT2, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT2, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig2), sizeof(g_MWTCACertConfig2));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig2));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT2);
}

void SKY_API SKY_WriteMWTCACert3Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT3))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT3) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT3, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT3, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig3), sizeof(g_MWTCACertConfig3));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig3));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT3);
}

void SKY_API SKY_WriteMWTCACert4Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT4))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT4) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT4, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT4, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig4), sizeof(g_MWTCACertConfig4));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig4));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT4);
}

void SKY_API SKY_WriteMWTCACert5Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT5))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT5) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT5, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT5, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig5), sizeof(g_MWTCACertConfig5));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig5));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT5);
}

void SKY_API SKY_WriteMWTCACert6Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT6))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT6) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT6, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT6, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig6), sizeof(g_MWTCACertConfig6));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig6));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT6);
}

void SKY_API SKY_WriteMWTCACert7Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT7))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT7) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT7, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT7, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig7), sizeof(g_MWTCACertConfig7));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig7));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT7);
}

void SKY_API SKY_WriteMWTCACert8Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT8))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT8) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT8, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT8, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig8), sizeof(g_MWTCACertConfig8));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig8));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT8);
}

void SKY_API SKY_WriteMWTCACert9Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT9))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT9) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT9, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT9, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig9), sizeof(g_MWTCACertConfig9));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig9));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT9);
}

void SKY_API SKY_WriteMWTCACert10Config(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_TCACERT10))
        return;

	if(SKY_IsFileOrDirExist(FILE_NAME_MW_TCACERT10) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT10, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_TCACERT10, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWTCACertConfig10), sizeof(g_MWTCACertConfig10));
	SKY_ASSERT(dwCount == sizeof(g_MWTCACertConfig10));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_TCACERT10);
}

void SKY_API SKY_WriteMWalletConfig(void)
{
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_MW_WALLET))
        return;

	// 전자지갑 정보
	if(SKY_IsFileOrDirExist(FILE_NAME_MW_WALLET) == FALSE)
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_WALLET, FS_AM_CREATE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}
	else
	{
		hFile = SKY_OpenFile(FILE_NAME_MW_WALLET, FS_AM_READWRITE);
		SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
	}

	dwCount = SKY_WriteFile(hFile, &(g_MWWalletConfig), sizeof(g_MWWalletConfig));
	SKY_ASSERT(dwCount == sizeof(g_MWWalletConfig));
	SKY_CloseFile(hFile);
	sRESET_CONFIG_DIRTY(SC_MW_WALLET);
}


// Cert정보 읽는 함수와 쓰는 함수.. -_-;;
// TempCert
WORD SKY_GET_MW_TEMPCERT_LENGTH(int i)
{
	if (i == 0)	return (sGET_MW_TEMPCERT_LENGTH(g_MWTempCertConfig1.MWTempCert));  
	else if(i == 1) return (sGET_MW_TEMPCERT_LENGTH(g_MWTempCertConfig2.MWTempCert));   
	else { 
		ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TEMPCERT_PIN_HASH(int i)		
{	 
	if (i == 0)  return (sGET_MW_TEMPCERT_PIN_HASH(g_MWTempCertConfig1.MWTempCert));  
	else if(i == 1) return (sGET_MW_TEMPCERT_PIN_HASH(g_MWTempCertConfig2.MWTempCert));  
	else {
		ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TEMPCERT_PRV_KEY(int i)		
{  
	if (i == 0) return (sGET_MW_TEMPCERT_PRV_KEY(g_MWTempCertConfig1.MWTempCert));
	else if(i == 1) return (sGET_MW_TEMPCERT_PRV_KEY(g_MWTempCertConfig2.MWTempCert));  
	else {
		ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
		return 0;
	}
} 

BYTE *SKY_GET_MW_TEMPCERT_SKID(int i)
{
	if(i  ==  0)  return (sGET_MW_TEMPCERT_SKID(g_MWTempCertConfig1.MWTempCert));
	else if(i == 1)  return(sGET_MW_TEMPCERT_SKID(g_MWTempCertConfig2.MWTempCert));
	else {
		ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
		return 0;
	}
}

DWORD SKY_GET_MW_TEMPCERT_REQUESTDATE(int i)	    
{  
	if (i == 0)  return (sGET_MW_TEMPCERT_REQUESTDATE(g_MWTempCertConfig1.MWTempCert));  
	else if((i) == 1) return (sGET_MW_TEMPCERT_REQUESTDATE(g_MWTempCertConfig2.MWTempCert)); 
	else {
		ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TEMPCERT_SUBJECT_DN(int i)	    
{  
	if (i == 0) return (sGET_MW_TEMPCERT_SUBJECT_DN(g_MWTempCertConfig1.MWTempCert));  
	else if(i == 1) return(sGET_MW_TEMPCERT_SUBJECT_DN(g_MWTempCertConfig2.MWTempCert));  
	else {
		ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
		return 0;
	}
}

BYTE SKY_GET_MW_TEMPCERT_KEY_USAGE(int i)		
{    
	if (i == 0) return (sGET_MW_TEMPCERT_KEY_USAGE(g_MWTempCertConfig1.MWTempCert)); 
	else if(i == 1) return (sGET_MW_TEMPCERT_KEY_USAGE(g_MWTempCertConfig2.MWTempCert));  
	else {
		ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
		return 0;
	}
}

// TempCertInfo 인증서 설치 정보
void SKY_SET_MW_TEMPCERT_LENGTH(int i,WORD s)	
{	
	if (i == 0)  { sSET_MEMORY_VALUE(SC_MW_TEMPCERT1,g_MWTempCertConfig1.MWTempCert.Length,s);  }
	else if(i == 1) { sSET_MEMORY_VALUE(SC_MW_TEMPCERT2,g_MWTempCertConfig2.MWTempCert.Length,s);  }
	else ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
}

void SKY_SET_MW_TEMPCERT_PIN_HASH(int i,BYTE *s,int len)
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TEMPCERT1,g_MWTempCertConfig1.MWTempCert.PIN_Hash,s,len); }
	else if(i == 1) { sSET_MEMORY_COPY(SC_MW_TEMPCERT2,g_MWTempCertConfig2.MWTempCert.PIN_Hash,s,len); }   
	else ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
}

void SKY_SET_MW_TEMPCERT_PRV_KEY(int i,BYTE *s,int len)
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TEMPCERT1,g_MWTempCertConfig1.MWTempCert.PRV_KEY,s,len);  }
	else if(i == 1) { sSET_MEMORY_COPY(SC_MW_TEMPCERT2,g_MWTempCertConfig2.MWTempCert.PRV_KEY,s,len);  } 
	else ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
}

void SKY_SET_MW_TEMPCERT_SKID(int i,BYTE *s,int len)
{
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TEMPCERT1,g_MWTempCertConfig1.MWTempCert.SKID,s,len);  }
	else if(i == 1) { sSET_MEMORY_COPY(SC_MW_TEMPCERT2,g_MWTempCertConfig2.MWTempCert.SKID,s,len);   }
	else ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0);	
}

void SKY_SET_MW_TEMPCERT_REQUESTDATE(int i,DWORD s)
{	
	if (i == 0) { sSET_MEMORY_VALUE(SC_MW_TEMPCERT1,g_MWTempCertConfig1.MWTempCert.RequestDate,s);  }
	else if(i == 1)  { sSET_MEMORY_VALUE(SC_MW_TEMPCERT2,g_MWTempCertConfig2.MWTempCert.RequestDate,s);  }
	else ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
}

void SKY_SET_MW_TEMPCERT_SUBJECT_DN(int i,BYTE *s,int len)
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TEMPCERT1,g_MWTempCertConfig1.MWTempCert.SUBJECT_DN,s,len);  }
	else if(i == 1) { sSET_MEMORY_COPY(SC_MW_TEMPCERT2,g_MWTempCertConfig2.MWTempCert.SUBJECT_DN,s,len); }  
	else ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
}

void SKY_SET_MW_TEMPCERT_KEY_USAGE(int i,BYTE s)
{	
	if (i == 0) { sSET_MEMORY_VALUE(SC_MW_TEMPCERT1,g_MWTempCertConfig1.MWTempCert.KEY_USAGE,s);  }
	else if(i == 1) { sSET_MEMORY_VALUE(SC_MW_TEMPCERT2,g_MWTempCertConfig2.MWTempCert.KEY_USAGE,s);  } 
	else ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
}

void  SKY_SET_MW_TEMPCERT(int i)									
{	
	if (i == 0) { SKY_WRITE_CONFIG(SC_MW_TEMPCERT1);  }
	else if(i == 1) { SKY_WRITE_CONFIG(SC_MW_TEMPCERT2); } 
	else ERR_FATAL("Bad argument in reading TempCert", 0, 0, 0); 
}

//// MyCertInfo 인증서 정보
WORD SKY_GET_MW_MYCERT_LENGTH(int i)		
{	
	if (i == 0)  return (sGET_MW_MYCERT_LENGTH(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_LENGTH(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_LENGTH(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_LENGTH(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_LENGTH(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

WORD SKY_GET_MW_MYCERT_CHECKFIELD(int i)		
{	
	if (i == 0) return (sGET_MW_MYCERT_CHECKFIELD(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_CHECKFIELD(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_CHECKFIELD(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_CHECKFIELD(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_CHECKFIELD(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_MYCERT_PIN_HASH(int i)		
{	
	if (i == 0) return (sGET_MW_MYCERT_PIN_HASH(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_PIN_HASH(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_PIN_HASH(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_PIN_HASH(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_PIN_HASH(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_MYCERT_PRV_KEY(int i)	
{	
	if (i == 0) return (sGET_MW_MYCERT_PRV_KEY(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_PRV_KEY(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_PRV_KEY(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_PRV_KEY(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_PRV_KEY(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_MYCERT_DISPLAY(int i)		
{	
	if (i == 0) return (sGET_MW_MYCERT_DISPLAY(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_DISPLAY(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_DISPLAY(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_DISPLAY(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_DISPLAY(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_MYCERT_SUBJECT_KID(int i)		
{	
	if (i == 0) return (sGET_MW_MYCERT_SUBJECT_KID(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_SUBJECT_KID(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_SUBJECT_KID(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_SUBJECT_KID(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_SUBJECT_KID(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_MYCERT_ISSUER_KID(int i)		
{	
	if (i == 0) return (sGET_MW_MYCERT_ISSUER_KID(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_ISSUER_KID(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_ISSUER_KID(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_ISSUER_KID(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_ISSUER_KID(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_MYCERT_CERT_URL(int i)	
{	
	if (i == 0)  return (sGET_MW_MYCERT_CERT_URL(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_CERT_URL(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_CERT_URL(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_CERT_URL(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_CERT_URL(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_MYCERT_SUBJECT_DN(int i)		
{	
	if (i == 0) return (sGET_MW_MYCERT_SUBJECT_DN(g_MWMYCertConfig1.MWMyCert));  
	else if(i == 1) return (sGET_MW_MYCERT_SUBJECT_DN(g_MWMYCertConfig2.MWMyCert));   
	else if(i == 2) return (sGET_MW_MYCERT_SUBJECT_DN(g_MWMYCertConfig3.MWMyCert));   
	else if(i == 3) return (sGET_MW_MYCERT_SUBJECT_DN(g_MWMYCertConfig4.MWMyCert));   
	else if(i == 4) return (sGET_MW_MYCERT_SUBJECT_DN(g_MWMYCertConfig5.MWMyCert));   
	else {
		ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
		return 0;
	}
}

// MyCertInfo 인증서 정보
void SKY_SET_MW_MYCERT_LENGTH(int i,WORD s)		
{	
	if (i == 0) { sSET_MEMORY_VALUE(SC_MW_MYCERT1,g_MWMYCertConfig1.MWMyCert.Length,s);  }
	else if(i == 1) { sSET_MEMORY_VALUE(SC_MW_MYCERT2,g_MWMYCertConfig2.MWMyCert.Length,s); }  
	else if(i == 2) { sSET_MEMORY_VALUE(SC_MW_MYCERT3,g_MWMYCertConfig3.MWMyCert.Length,s);  } 
	else if(i == 3) { sSET_MEMORY_VALUE(SC_MW_MYCERT4,g_MWMYCertConfig4.MWMyCert.Length,s); }   
	else if(i == 4) { sSET_MEMORY_VALUE(SC_MW_MYCERT5,g_MWMYCertConfig5.MWMyCert.Length,s); }  
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT_CHECKFIELD(int i,WORD s)		
{	
	if (i == 0) { sSET_MEMORY_VALUE(SC_MW_MYCERT1,g_MWMYCertConfig1.MWMyCert.CheckField,s);  }
	else if(i == 1) { sSET_MEMORY_VALUE(SC_MW_MYCERT2,g_MWMYCertConfig2.MWMyCert.CheckField,s);   }
	else if(i == 2) { sSET_MEMORY_VALUE(SC_MW_MYCERT3,g_MWMYCertConfig3.MWMyCert.CheckField,s);   }
	else if(i == 3) { sSET_MEMORY_VALUE(SC_MW_MYCERT4,g_MWMYCertConfig4.MWMyCert.CheckField,s);   }
	else if(i == 4) { sSET_MEMORY_VALUE(SC_MW_MYCERT5,g_MWMYCertConfig5.MWMyCert.CheckField,s); }  
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT_PIN_HASH(int i,BYTE *s,int len)		
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_MYCERT1,g_MWMYCertConfig1.MWMyCert.PIN_HASH,s,len);  }
	else if(i == 1) { sSET_MEMORY_COPY(SC_MW_MYCERT2,g_MWMYCertConfig1.MWMyCert.PIN_HASH,s,len);   }
	else if(i == 2) { sSET_MEMORY_COPY(SC_MW_MYCERT3,g_MWMYCertConfig1.MWMyCert.PIN_HASH,s,len);   }
	else if(i == 3) { sSET_MEMORY_COPY(SC_MW_MYCERT4,g_MWMYCertConfig1.MWMyCert.PIN_HASH,s,len);   }
	else if(i == 4) { sSET_MEMORY_COPY(SC_MW_MYCERT5,g_MWMYCertConfig1.MWMyCert.PIN_HASH,s,len);   }
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT_PRV_KEY(int i,BYTE *s,int len)		
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_PRV_KEY(i),s,len);  }
	else if(i == 1) { sSET_MEMORY_COPY(SC_MW_MYCERT2,SKY_GET_MW_MYCERT_PRV_KEY(i),s,len);   }
	else if(i == 2) { sSET_MEMORY_COPY(SC_MW_MYCERT3,SKY_GET_MW_MYCERT_PRV_KEY(i),s,len);   }
	else if(i == 3) { sSET_MEMORY_COPY(SC_MW_MYCERT4,SKY_GET_MW_MYCERT_PRV_KEY(i),s,len);   }
	else if(i == 4) { sSET_MEMORY_COPY(SC_MW_MYCERT5,SKY_GET_MW_MYCERT_PRV_KEY(i),s,len);   }
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT_DISPLAY(int i,BYTE *s,int len)		
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_DISPLAY(i),s,len);  }
	else if(i == 1){ sSET_MEMORY_COPY(SC_MW_MYCERT2,SKY_GET_MW_MYCERT_DISPLAY(i),s,len);   }
	else if(i == 2){ sSET_MEMORY_COPY(SC_MW_MYCERT3,SKY_GET_MW_MYCERT_DISPLAY(i),s,len);   }
	else if(i == 3) {sSET_MEMORY_COPY(SC_MW_MYCERT4,SKY_GET_MW_MYCERT_DISPLAY(i),s,len);   }
	else if(i == 4) {sSET_MEMORY_COPY(SC_MW_MYCERT5,SKY_GET_MW_MYCERT_DISPLAY(i),s,len);   }
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT_SUBJECT_KID(int i,BYTE *s,int len)		
{	
	if (i == 0){ sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_SUBJECT_KID(i),s,len);  }
	else if(i == 1){ sSET_MEMORY_COPY(SC_MW_MYCERT2,SKY_GET_MW_MYCERT_SUBJECT_KID(i),s,len);   }
	else if(i == 2){ sSET_MEMORY_COPY(SC_MW_MYCERT3,SKY_GET_MW_MYCERT_SUBJECT_KID(i),s,len);   }
	else if(i == 3){ sSET_MEMORY_COPY(SC_MW_MYCERT4,SKY_GET_MW_MYCERT_SUBJECT_KID(i),s,len);   }
	else if(i == 4){ sSET_MEMORY_COPY(SC_MW_MYCERT5,SKY_GET_MW_MYCERT_SUBJECT_KID(i),s,len);   }
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT_ISSUER_KID(int i,BYTE *s,int len)		
{	
	if (i == 0){ sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_ISSUER_KID(i),s,len);  }
	else if(i == 1){ sSET_MEMORY_COPY(SC_MW_MYCERT2,SKY_GET_MW_MYCERT_ISSUER_KID(i),s,len);   }
	else if(i == 2){ sSET_MEMORY_COPY(SC_MW_MYCERT3,SKY_GET_MW_MYCERT_ISSUER_KID(i),s,len);   }
	else if(i == 3) {sSET_MEMORY_COPY(SC_MW_MYCERT4,SKY_GET_MW_MYCERT_ISSUER_KID(i),s,len);   }
	else if(i == 4) {sSET_MEMORY_COPY(SC_MW_MYCERT5,SKY_GET_MW_MYCERT_ISSUER_KID(i),s,len);   }
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT_CERT_URL(int i,BYTE *s,int len)		
{	
	if (i == 0) {sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_CERT_URL(i),s,len);  }
	else if(i == 1){ sSET_MEMORY_COPY(SC_MW_MYCERT2,SKY_GET_MW_MYCERT_CERT_URL(i),s,len); }  
	else if(i == 2){ sSET_MEMORY_COPY(SC_MW_MYCERT3,SKY_GET_MW_MYCERT_CERT_URL(i),s,len);   }
	else if(i == 3) {sSET_MEMORY_COPY(SC_MW_MYCERT4,SKY_GET_MW_MYCERT_CERT_URL(i),s,len);   }
	else if(i == 4){ sSET_MEMORY_COPY(SC_MW_MYCERT5,SKY_GET_MW_MYCERT_CERT_URL(i),s,len);   }
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT_SUBJECT_DN(int i,BYTE *s,int len)		
{	
	if (i == 0){ sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_SUBJECT_DN(i),s,len);  }
	else if(i == 1){ sSET_MEMORY_COPY(SC_MW_MYCERT2,SKY_GET_MW_MYCERT_SUBJECT_DN(i),s,len); }  
	else if(i == 2){ sSET_MEMORY_COPY(SC_MW_MYCERT3,SKY_GET_MW_MYCERT_SUBJECT_DN(i),s,len);   }
	else if(i == 3){ sSET_MEMORY_COPY(SC_MW_MYCERT4,SKY_GET_MW_MYCERT_SUBJECT_DN(i),s,len);   }
	else if(i == 4){ sSET_MEMORY_COPY(SC_MW_MYCERT5,SKY_GET_MW_MYCERT_SUBJECT_DN(i),s,len);   }
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

void SKY_SET_MW_MYCERT(int i)						
{
	if (i == 0)  { SKY_WRITE_CONFIG(SC_MW_MYCERT1);  }
	else if(i == 1) {   SKY_WRITE_CONFIG(SC_MW_MYCERT2); }  
	else if(i == 2)  {  SKY_WRITE_CONFIG(SC_MW_MYCERT3);   }
	else if(i == 3)  {  SKY_WRITE_CONFIG(SC_MW_MYCERT4); }  
	else if(i == 4)  {  SKY_WRITE_CONFIG(SC_MW_MYCERT5); }   
	else ERR_FATAL("Bad argument in reading MyCert", 0, 0, 0); 
}

// TCA Cert 정보
WORD SKY_GET_MW_TCACERT_LENGTH(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

WORD SKY_GET_MW_TCACERT_CHECKFIELD(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TCACERT_DISPLAY(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TCACERT_SUBJECT_KID(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TCACERT_ISSUER_KID(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TCACERT_CERT(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_CERT(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TCACERT_CA_INFO_URL(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

BYTE *SKY_GET_MW_TCACERT_SUBJECT_DN(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

DWORD SKY_GET_MW_TCACERT_NOT_AFTER(int i)	
{	
	if (i == 0)  return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig1.MWTCACert));
	else if(i == 1) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig2.MWTCACert));   
	else if(i == 2) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig3.MWTCACert));   
	else if(i == 3) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig4.MWTCACert));   
	else if(i == 4) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig5.MWTCACert));   
	else if(i == 5) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig6.MWTCACert));   
	else if(i == 6) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig7.MWTCACert));   
	else if(i == 7) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig8.MWTCACert));   
	else if(i == 8) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig9.MWTCACert));   
	else if(i == 9) return (sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig10.MWTCACert));   
	else { 
		ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
		return 0;
	}
}

// TCACertInfo 인증서 정보
void SKY_SET_MW_TCACERT_LENGTH(int i,WORD s)		
{	
	if (i == 0) { sSET_MEMORY_VALUE(SC_MW_TCACERT1,g_MWTCACertConfig1.MWTCACert.Length,s); }
	else if(i == 1)   { sSET_MEMORY_VALUE(SC_MW_TCACERT2,g_MWTCACertConfig2.MWTCACert.Length,s);   }
	else if(i == 2)   { sSET_MEMORY_VALUE(SC_MW_TCACERT3,g_MWTCACertConfig3.MWTCACert.Length,s);   }
	else if(i == 3)   { sSET_MEMORY_VALUE(SC_MW_TCACERT4,g_MWTCACertConfig4.MWTCACert.Length,s);   }
	else if(i == 4)   { sSET_MEMORY_VALUE(SC_MW_TCACERT5,g_MWTCACertConfig5.MWTCACert.Length,s);   }
	else if(i == 5)   { sSET_MEMORY_VALUE(SC_MW_TCACERT6,g_MWTCACertConfig6.MWTCACert.Length,s);   }
	else if(i == 6)   { sSET_MEMORY_VALUE(SC_MW_TCACERT7,g_MWTCACertConfig7.MWTCACert.Length,s);   }
	else if(i == 7)   { sSET_MEMORY_VALUE(SC_MW_TCACERT8,g_MWTCACertConfig8.MWTCACert.Length,s);   }
	else if(i == 8)   { sSET_MEMORY_VALUE(SC_MW_TCACERT9,g_MWTCACertConfig9.MWTCACert.Length,s);   }
	else if(i == 9)   { sSET_MEMORY_VALUE(SC_MW_TCACERT10,g_MWTCACertConfig10.MWTCACert.Length,s);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT_CHECKFIELD(int i,WORD s)		
{	
	if (i == 0) { sSET_MEMORY_VALUE(SC_MW_TCACERT1,g_MWTCACertConfig1.MWTCACert.CheckField,s); }
	else if(i == 1)  {  sSET_MEMORY_VALUE(SC_MW_TCACERT2,g_MWTCACertConfig2.MWTCACert.CheckField,s);   }
	else if(i == 2)   { sSET_MEMORY_VALUE(SC_MW_TCACERT3,g_MWTCACertConfig3.MWTCACert.CheckField,s);   }
	else if(i == 3)   { sSET_MEMORY_VALUE(SC_MW_TCACERT4,g_MWTCACertConfig4.MWTCACert.CheckField,s);   }
	else if(i == 4)   { sSET_MEMORY_VALUE(SC_MW_TCACERT5,g_MWTCACertConfig5.MWTCACert.CheckField,s);   }
	else if(i == 5)   { sSET_MEMORY_VALUE(SC_MW_TCACERT6,g_MWTCACertConfig6.MWTCACert.CheckField,s);   }
	else if(i == 6)   { sSET_MEMORY_VALUE(SC_MW_TCACERT7,g_MWTCACertConfig7.MWTCACert.CheckField,s);   }
	else if(i == 7)   { sSET_MEMORY_VALUE(SC_MW_TCACERT8,g_MWTCACertConfig8.MWTCACert.CheckField,s);   }
	else if(i == 8)   { sSET_MEMORY_VALUE(SC_MW_TCACERT9,g_MWTCACertConfig9.MWTCACert.CheckField,s);   }
	else if(i == 9)   { sSET_MEMORY_VALUE(SC_MW_TCACERT10,g_MWTCACertConfig10.MWTCACert.CheckField,s);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT_DISPLAY(int i,BYTE *s,int len)		
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_DISPLAY(i),s,len); }
	else if(i == 1) {   sSET_MEMORY_COPY(SC_MW_TCACERT2,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else if(i == 2)  {  sSET_MEMORY_COPY(SC_MW_TCACERT3,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else if(i == 3)  {  sSET_MEMORY_COPY(SC_MW_TCACERT4,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else if(i == 4)  {  sSET_MEMORY_COPY(SC_MW_TCACERT5,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else if(i == 5)  {  sSET_MEMORY_COPY(SC_MW_TCACERT6,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else if(i == 6)  {  sSET_MEMORY_COPY(SC_MW_TCACERT7,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else if(i == 7)  {  sSET_MEMORY_COPY(SC_MW_TCACERT8,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else if(i == 8)  {  sSET_MEMORY_COPY(SC_MW_TCACERT9,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else if(i == 9)  {  sSET_MEMORY_COPY(SC_MW_TCACERT10,SKY_GET_MW_TCACERT_DISPLAY(i),s,len);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT_SUBJECT_KID(int i,BYTE *s,int len)		
{	
	if (i == 0)  { sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len); }
	else if(i == 1)  {  sSET_MEMORY_COPY(SC_MW_TCACERT2,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else if(i == 2)   { sSET_MEMORY_COPY(SC_MW_TCACERT3,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else if(i == 3)  {  sSET_MEMORY_COPY(SC_MW_TCACERT4,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else if(i == 4)  {  sSET_MEMORY_COPY(SC_MW_TCACERT5,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else if(i == 5)  {  sSET_MEMORY_COPY(SC_MW_TCACERT6,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else if(i == 6)  {  sSET_MEMORY_COPY(SC_MW_TCACERT7,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else if(i == 7)  {  sSET_MEMORY_COPY(SC_MW_TCACERT8,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else if(i == 8)  {  sSET_MEMORY_COPY(SC_MW_TCACERT9,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else if(i == 9)  {  sSET_MEMORY_COPY(SC_MW_TCACERT10,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT_ISSUER_KID(int i,BYTE *s,int len)		
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len); }
	else if(i == 1)  {  sSET_MEMORY_COPY(SC_MW_TCACERT2,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else if(i == 2)  {  sSET_MEMORY_COPY(SC_MW_TCACERT3,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else if(i == 3)  {  sSET_MEMORY_COPY(SC_MW_TCACERT4,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else if(i == 4)  {  sSET_MEMORY_COPY(SC_MW_TCACERT5,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else if(i == 5)  {  sSET_MEMORY_COPY(SC_MW_TCACERT6,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else if(i == 6)   { sSET_MEMORY_COPY(SC_MW_TCACERT7,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else if(i == 7)  {  sSET_MEMORY_COPY(SC_MW_TCACERT8,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else if(i == 8)  {  sSET_MEMORY_COPY(SC_MW_TCACERT9,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else if(i == 9)  {  sSET_MEMORY_COPY(SC_MW_TCACERT10,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT_CERT(int i,BYTE *s,int len)		
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_CERT(i),s,len); }
	else if(i == 1) {   sSET_MEMORY_COPY(SC_MW_TCACERT2,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else if(i == 2)  {  sSET_MEMORY_COPY(SC_MW_TCACERT3,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else if(i == 3)  {  sSET_MEMORY_COPY(SC_MW_TCACERT4,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else if(i == 4)  {  sSET_MEMORY_COPY(SC_MW_TCACERT5,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else if(i == 5)  {  sSET_MEMORY_COPY(SC_MW_TCACERT6,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else if(i == 6)  {  sSET_MEMORY_COPY(SC_MW_TCACERT7,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else if(i == 7)   { sSET_MEMORY_COPY(SC_MW_TCACERT8,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else if(i == 8)  {  sSET_MEMORY_COPY(SC_MW_TCACERT9,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else if(i == 9)  {  sSET_MEMORY_COPY(SC_MW_TCACERT10,SKY_GET_MW_TCACERT_CERT(i),s,len);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT_CA_INFO_URL(int i,BYTE *s,int len)		
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len); }
	else if(i == 1) {   sSET_MEMORY_COPY(SC_MW_TCACERT2,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else if(i == 2)  {  sSET_MEMORY_COPY(SC_MW_TCACERT3,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else if(i == 3)  {  sSET_MEMORY_COPY(SC_MW_TCACERT4,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else if(i == 4)  {  sSET_MEMORY_COPY(SC_MW_TCACERT5,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else if(i == 5)  {  sSET_MEMORY_COPY(SC_MW_TCACERT6,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else if(i == 6)  {  sSET_MEMORY_COPY(SC_MW_TCACERT7,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else if(i == 7)  {  sSET_MEMORY_COPY(SC_MW_TCACERT8,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else if(i == 8)   { sSET_MEMORY_COPY(SC_MW_TCACERT9,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else if(i == 9)  {  sSET_MEMORY_COPY(SC_MW_TCACERT10,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT_SUBJECT_DN(int i,BYTE *s,int len)		
{	
	if (i == 0) { sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len); }
	else if(i == 1) {   sSET_MEMORY_COPY(SC_MW_TCACERT2,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else if(i == 2)  {  sSET_MEMORY_COPY(SC_MW_TCACERT3,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else if(i == 3)  {  sSET_MEMORY_COPY(SC_MW_TCACERT4,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else if(i == 4)  {  sSET_MEMORY_COPY(SC_MW_TCACERT5,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else if(i == 5)  {  sSET_MEMORY_COPY(SC_MW_TCACERT6,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else if(i == 6)  {  sSET_MEMORY_COPY(SC_MW_TCACERT7,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else if(i == 7)  {  sSET_MEMORY_COPY(SC_MW_TCACERT8,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else if(i == 8)  {  sSET_MEMORY_COPY(SC_MW_TCACERT9,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else if(i == 9)  {  sSET_MEMORY_COPY(SC_MW_TCACERT10,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT_NOT_AFTER(int i, DWORD s)		
{	
	if (i == 0)  { sSET_MEMORY_VALUE(SC_MW_TCACERT1,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s); }
	else if(i == 1)  {  sSET_MEMORY_VALUE(SC_MW_TCACERT2,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);  } 
	else if(i == 2)  {  sSET_MEMORY_VALUE(SC_MW_TCACERT3,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);   }
	else if(i == 3) {   sSET_MEMORY_VALUE(SC_MW_TCACERT4,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);   }
	else if(i == 4) {   sSET_MEMORY_VALUE(SC_MW_TCACERT5,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);   }
	else if(i == 5)  {  sSET_MEMORY_VALUE(SC_MW_TCACERT6,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);  } 
	else if(i == 6) {   sSET_MEMORY_VALUE(SC_MW_TCACERT7,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);  } 
	else if(i == 7) {   sSET_MEMORY_VALUE(SC_MW_TCACERT8,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);   }
	else if(i == 8) {   sSET_MEMORY_VALUE(SC_MW_TCACERT9,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);   }
	else if(i == 9) {   sSET_MEMORY_VALUE(SC_MW_TCACERT10,g_MWTCACertConfig1.MWTCACert.NOT_AFTER,s);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

void SKY_SET_MW_TCACERT(int i)									
{	
	if (i == 0)      {    SKY_WRITE_CONFIG(SC_MW_TCACERT1);  }
	else if(i == 1) {   SKY_WRITE_CONFIG(SC_MW_TCACERT2);   }
	else if(i == 2) {   SKY_WRITE_CONFIG(SC_MW_TCACERT3);   }
	else if(i == 3)  {  SKY_WRITE_CONFIG(SC_MW_TCACERT4);   }
	else if(i == 4)  {  SKY_WRITE_CONFIG(SC_MW_TCACERT5);   }
	else if(i == 5)  {  SKY_WRITE_CONFIG(SC_MW_TCACERT6);  } 
	else if(i == 6)  {  SKY_WRITE_CONFIG(SC_MW_TCACERT7);   }
	else if(i == 7)  {  SKY_WRITE_CONFIG(SC_MW_TCACERT8);   }
	else if(i == 8)  {  SKY_WRITE_CONFIG(SC_MW_TCACERT9);  } 
	else if(i == 9)  {  SKY_WRITE_CONFIG(SC_MW_TCACERT10);   }
	else ERR_FATAL("Bad argument in reading TCACert", 0, 0, 0); 
}

#endif //#ifdef FEATURE_MCOMMERCE

#ifdef FEATURE_CHARACTER_BELL
LOCAL void SKY_API sInitCharacbell(void) {
    g_CharacbellConfig.Address = 0x00;
    memset(g_CharacbellConfig.CBLIST, 0x00, (sizeof(CHARACBELL_T) * MAX_NUM_OF_CHARACBELL));
    SKY_WriteCharacbellConfig();
}

void SKY_API SKY_InitCharacbellConfig(void) {
    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_CB_LIST) == FALSE) 
    {
        // set the initial value 
        sInitCharacbell();
    }
    else
    {  
        hFile = SKY_OpenFile(FILE_NAME_CB_LIST, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
        dwCount = SKY_ReadFile(hFile, &(g_CharacbellConfig), sizeof(g_CharacbellConfig));
        SKY_CloseFile(hFile);
    }
} // SKY_InitCharacbellConfig

void SKY_API SKY_DeleteCharacbellConfig(void)
{
    SKY_DeleteFile(FILE_NAME_CB_LIST);
}

void SKY_API SKY_WriteCharacbellConfig(void) {
    HFILE hFile;
    DWORD dwCount;

    if(!sIS_CONFIG_DIRTY(SC_CHABELL))
        return;

    if(SKY_IsFileOrDirExist(FILE_NAME_CB_LIST) == FALSE)
    {
        hFile = SKY_OpenFile(FILE_NAME_CB_LIST, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_CB_LIST, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    dwCount = SKY_WriteFile(hFile, &(g_CharacbellConfig), sizeof(g_CharacbellConfig));
    SKY_ASSERT(dwCount == sizeof(g_CharacbellConfig));
    SKY_CloseFile(hFile);
    sRESET_CONFIG_DIRTY(SC_CHABELL);
} // SKY_WriteCharacbellonfig
#endif // FEATURE_CHARACTER_BELL