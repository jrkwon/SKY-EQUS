/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_conf.h
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
** ============================================================================
*/

#ifndef _SKY_CONF_H_
#define _SKY_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
** SKY의 환경 정보 저장 방법 
**
** SKY UI에서는 더 이상 NV 아이템을 사용하지 않는다.
** 모든 데이터는 EFS에 파일 형태로 저장되며, 각 항목에 접근할 수 있는 매크로가 제공된다
** 단, 한 가지 예외가 있는데, 바로 다른 태스크에서도 사용되는 NV 아이템에 값을 써 주어
** 야 하는 경우이다. 이 때는 다른 태스크를 변경하지 않아도 되도록 기존의 NV 아이템
** 접근방식을 사용해야 한다.
** 다음은 NV 아이템으로 처리해 주어야 하는 부분이다.
**
** NV_CURR_NAM_I    : current nam number
** NV_PCDMACH_I     : primary cdma channel
** NV_MIN1          : MIN1
** NV_MIN2          : MIN2
*/

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
#define MAX_NAME_LENGTH                 (32)
#define MAX_PASSWORD_LENGTH             (4)
#define MAX_DIAL_DIGIT_LENGTH           (32)
#define MAX_AREA_CODE_LENGTH            (4)
#define MAX_BANNER_LENGTH               (16)
#define MAX_URL_LENGTH                  (70)
#define MAX_WAPMAIL_LENGTH              (130)
#define MAX_IP_LENGTH                   (9+2)
//#define MAX_FILE_NAME_LENGTH            (8+1+3)
#define MAX_FILE_NAME_LENGTH            SKY_FILE_EXT_NAME_MAX_LENGTH
#define MAX_VERSION_INFO_LENGTH         (4+3)
#define MAX_NAM_COUNT                   (4)
#define MAX_MIN_COUNT                   (2)
#define MAX_DIAL_DIGITS                 (32)
#define MAX_OF_ARGV						(128)
/* file name definition */
#define FILE_NAME_EXT               SKY_SYS_DIR""SKY_DIR_CHAR"EXT.SKY"
#define FILE_NAME_SYS               SKY_SYS_DIR""SKY_DIR_CHAR"SYS.SKY"
#define FILE_NAME_MISC              SKY_SYS_DIR""SKY_DIR_CHAR"MISC.SKY"
#define FILE_NAME_APP               SKY_SYS_DIR""SKY_DIR_CHAR"APP.SKY"
#define FILE_NAME_CAMERA            SKY_SYS_DIR""SKY_DIR_CHAR"CAMERA.SKY"
#define FILE_NAME_PM                SKY_SYS_DIR""SKY_DIR_CHAR"PM.SKY"
#ifdef FEATURE_GVM
#define FILE_NAME_GVM               SKY_SYS_DIR""SKY_DIR_CHAR"GVM.SKY"
#endif//#ifdef FEATURE_GVM
#ifdef FEATURE_MCOMMERCE
#define FILE_NAME_MW_TEMPCERT1		SKY_SYS_DIR""SKY_DIR_CHAR"MWTmp1.SKY"
#define FILE_NAME_MW_TEMPCERT2		SKY_SYS_DIR""SKY_DIR_CHAR"MWTmp2.SKY"
#define FILE_NAME_MW_MYCERT1		  SKY_SYS_DIR""SKY_DIR_CHAR"MWMyCrt1.SKY"
#define FILE_NAME_MW_MYCERT2		  SKY_SYS_DIR""SKY_DIR_CHAR"MWMyCrt2.SKY"
#define FILE_NAME_MW_MYCERT3		  SKY_SYS_DIR""SKY_DIR_CHAR"MWMyCrt3.SKY"
#define FILE_NAME_MW_MYCERT4		  SKY_SYS_DIR""SKY_DIR_CHAR"MWMyCrt4.SKY"
#define FILE_NAME_MW_MYCERT5		  SKY_SYS_DIR""SKY_DIR_CHAR"MWMyCrt5.SKY"
#define FILE_NAME_MW_TCACERT1		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA1.SKY"
#define FILE_NAME_MW_TCACERT2		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA2.SKY"
#define FILE_NAME_MW_TCACERT3		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA3.SKY"
#define FILE_NAME_MW_TCACERT4		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA4.SKY"
#define FILE_NAME_MW_TCACERT5		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA5.SKY"
#define FILE_NAME_MW_TCACERT6		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA6.SKY"
#define FILE_NAME_MW_TCACERT7		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA7.SKY"
#define FILE_NAME_MW_TCACERT8		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA8.SKY"
#define FILE_NAME_MW_TCACERT9		  SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA9.SKY"
#define FILE_NAME_MW_TCACERT10      SKY_SYS_DIR""SKY_DIR_CHAR"MWTCA10.SKY"
#define FILE_NAME_MW_WALLET		       SKY_SYS_DIR""SKY_DIR_CHAR"MWALLET.SKY"
#endif /* FEATURE_MCOMMERCE */
#ifdef FEATURE_CHARACTER_BELL
#define FILE_NAME_CB_LIST             SKY_SYS_DIR""SKY_DIR_CHAR"CBELL.SKY"
#endif // FEATURE_CHARACTER_BELL
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

/*---------------------------------------------------------------------------
** SKY 환경 정보 
*/
enum tagSKY_CONFIG_E
{
    SC_EXT,             /* 기타 정보 */
    SC_MISC,            /* 부가 정보 */
    SC_SYS,             /* 단말기 정보 */
    SC_APP,             /* 응용프로그램 정보 */
    SC_GVM,
	SC_MW_TEMPCERT1,   /* 전자지갑 정보 여기서부터 */
	SC_MW_TEMPCERT2,
	SC_MW_MYCERT1,
	SC_MW_MYCERT2,
	SC_MW_MYCERT3,
	SC_MW_MYCERT4,
	SC_MW_MYCERT5,
	SC_MW_TCACERT1,
	SC_MW_TCACERT2,
	SC_MW_TCACERT3,
	SC_MW_TCACERT4,
	SC_MW_TCACERT5,
	SC_MW_TCACERT6,
	SC_MW_TCACERT7,
	SC_MW_TCACERT8,
	SC_MW_TCACERT9,
	SC_MW_TCACERT10,
	SC_MW_WALLET,           /* 전자지갑 정보 여기까지..*/
#ifdef FEATURE_CHARACTER_BELL
    SC_CHABELL,
#endif // FEATURE_CHARACTER_BELL
    SC_MAX
};
typedef enum tagSKY_CONFIG_E            SKY_CONFIG_E;

typedef void SKY_API (SKY_WriteConfig)(void);

struct tagSKY_CONFIG_T
{
    BOOL            fDirty;
    SKY_WriteConfig *pfnWrite;
};
typedef struct tagSKY_CONFIG_T  SKY_CONFIG_T;

/*---------------------------------------------------------------------------
** 환경 정보 변경
*/
extern SKY_CONFIG_T        g_aSkyConfig[];

#define sSET_CONFIG_DIRTY(SkyConfig) g_aSkyConfig[SkyConfig].fDirty = (TRUE)
#define sIS_CONFIG_DIRTY(SkyConfig)  (g_aSkyConfig[SkyConfig].fDirty)

/* 
** 시스템 초기화 후엔 NV_SPARE_1_I 값이 0xAA로 설정된다.
** 한번이라도 값이 써 지면 NV_SPARE_1_I값을 0xFF로 설정한다.
** <-- 공정에서 단말의 상태를 확인하기 위해 사용.
** g_fVirginSystem은 시스템 시작 시에 NV_SPARE_1_I값을 읽어서
** 0xAA면 TRUE로 되고, 아니면 FALSE로 설정된다.
*/
#define SKY_VIRGIN_SYSTEM_FLAG       0xAA
#define SKY_NOVIRGIN_SYSTEM_FLAG     0xFF

#define SKY_MAKE_NO_VIRGIN() \
        if(SKY_IS_VIRGIN_SYSTEM()) \
        { \
            WORD wSpare1 = SKY_NOVIRGIN_SYSTEM_FLAG; \
            SKY_PutNV(NV_SPARE_1_I, (NV_ITEM_U*)&wSpare1); \
            SKY_SET_VIRGIN_SYSTEM(FALSE); \
        }

/* 정보를 저장매체에 기록 */
#define SKY_WRITE_CONFIG(SkyConfig)  \
{ \
    if(sIS_CONFIG_DIRTY(SkyConfig)) \
    { \
        g_aSkyConfig[SkyConfig].pfnWrite(); \
        SKY_MAKE_NO_VIRGIN(); \
    } \
}

/*값 복사 Write 없음 */
#define sSET_MEMORY_VALUE(SkyConfig, a, b) \
        { \
            (a) = (b); \
	        sSET_CONFIG_DIRTY(SkyConfig); \
        }

/* 문자열 설정 Write 없음 */
#define sSET_MEMORY_STR(SkyConfig, a, b)  \
        { \
            STRCPY((BYTE*)(a), (BYTE*)(b));\
            sSET_CONFIG_DIRTY(SkyConfig); \
        }                       

/* 메모리 복사 Write 없음 */
#define sSET_MEMORY_COPY(SkyConfig, a, b, len) \
        { \
            memcpy((BYTE*)(a), (BYTE*)(b), len); \
			sSET_CONFIG_DIRTY(SkyConfig); \
        }
                           
/* 값 설정 */
#define sSET_CONFIG_VALUE(SkyConfig, a, b) \
        { \
            (a) = (b); \
	        sSET_CONFIG_DIRTY(SkyConfig); \
            SKY_WRITE_CONFIG(SkyConfig); \
        }

/* 문자열 설정 */
#define sSET_CONFIG_STR(SkyConfig, a, b)  \
        { \
            STRCPY((BYTE*)(a), (BYTE*)(b)); \
            sSET_CONFIG_DIRTY(SkyConfig); \
            SKY_WRITE_CONFIG(SkyConfig); \
        }

/* 메모리 복사 */
#define sSET_CONFIG_COPY(SkyConfig, a, b, len)  \
        { \
            memcpy((BYTE*)(a), (BYTE*)(b), len); \
            sSET_CONFIG_DIRTY(SkyConfig); \
            SKY_WRITE_CONFIG(SkyConfig); \
        }

/*
******************************************************************************
******************************************************************************
**
**
**  기타 정보
**
**
******************************************************************************
******************************************************************************
*/
struct tagEXT_CONFIG_T
{
    SKY_FILE_HEADER_T       Header;             /* 파일 헤더 */
    BYTE szOpenDataFileName[SKY_PATH_NAME_MAX_LENGTH];
};

typedef struct tagEXT_CONFIG_T  EXT_CONFIG_T;

/*---------------------------------------------------------------------------
** WRITE로 열어 놓은 데이터 파일 이름 (전체 경로)
*/
#define SKY_GET_OPEN_DATA_FILE_NAME()           g_ExtConfig.szOpenDataFileName
#define SKY_IS_OPEN_DATA_FILE()                 (SKY_GET_OPEN_DATA_FILE_NAME()[0] != '\0')

/*---------------------------------------------------------------------------
** WRITE로 열어 놓은 데이터 파일 이름 (전체 경로)
*/
#define SKY_SET_OPEN_DATA_FILE_NAME(s)          sSET_CONFIG_STR(SC_EXT, SKY_GET_OPEN_DATA_FILE_NAME(), s)
#define SKY_CLEAR_OPEN_DATA_FILE_NAME()         sSET_CONFIG_STR(SC_EXT, SKY_GET_OPEN_DATA_FILE_NAME(), "")



/*---------------------------------------------------------------------------
** 011 부가 서비스 번호 저장                        //white 2000.12.19
*/
#define SKY_MAX_NUM_011_SERVICE      15
#define SKY_MAX_DIAL_011_SERVICE	 (4 + 32)       

enum tagSERVICE011_E
{
	SKY_VOICE_POND = 0,
	SKY_VOICE_POND_SET,
	SKY_VOICE_POND_IMME,		
	SKY_VOICE_POND_RESET,		
	SKY_AUTO_CONNECT_SET,
	SKY_AUTO_CONNECT_RESET,
	SKY_CALL_REDIR_SET,		
	SKY_CALL_REDIR_RESET,		
	SKY_INFO_POND,			
	SKY_TRAFFIC_INFO,			
	SKY_ORIG_BAN_FOREIGN,		
	SKY_ORIG_BAN_ALL,			
	SKY_ORIG_BAN_RESET,
	SKY_CALL_FORWARD_HOT,
	SKY_CALL_PENDING_HOT
};

typedef enum tagSERVICE011_E        SERVICE011_E;


/*---------------------------------------------------------------------------
** 017 부가 서비스 번호 저장
*/
enum tagSERVICE017_E
{
	S017_VOICE_MAIL_CONNECT,
	S017_VOICE_MAIL_SET,
	S017_VOICE_MAIL_CANCEL,
	S017_AUTO_CONNECT_REGISTER,
	S017_AUTO_CONNECT_CANCEL,
	S017_CALL_REDIR_REGISTER,		
	S017_CALL_REDIR_CANCEL,		
	S017_CALL_WAITING_REGISTER,
	S017_CALL_WAITING_CANCEL,
	S017_POWER_REMOTE,		
	S017_POWER_MELODY,			
	S017_CALL_FORWARD_HOT,
	S017_CALL_PENDING_HOT,
    S017_MAX
};
typedef enum tagSERVICE017_E  S017_E;

#define SKY_MAX_NUM_017_SERVICE      S017_MAX //13
#define SKY_MAX_DIAL_017_SERVICE	 (4 + 32)       


/*---------------------------------------------------------------------------
**  벨
*/
#define SKY_MY_BELL_ID                  0xFFFF
#define SKY_IS_MY_BELL(w)                   ((w) == SKY_MY_BELL_ID)

#ifdef FEATURE_CHARACBELL
#define SKY_CHARAC_BELL_ID              0x0FFFD
#define SKY_IS_CHARAC_BELL(w)           ((w) == SKY_CHARAC_BELL_ID)
#endif // FEATURE_CHARACBELL

#ifdef FEATURE_CHARACTER_BELL
#define SKY_CB_GROUPBELL                0x0FFFC
#define SKY_IS_CB_GROUPBELL(w)          ((w) == SKY_CB_GROUPBELL)
#endif // FEATURE_CHARACTER_BELL

struct tagBELL_TYPE_T 
{
    WORD    wBellID;                            /* 벨 ID */
    BYTE    szFileName[MAX_FILE_NAME_LENGTH+1];   /* 나만의 벨 파일 이름 */
};
typedef struct tagBELL_TYPE_T               BELL_TYPE_T;

#define sGET_BELL_ID(s)            (s).wBellID
#define sGET_BELL_FILE_NAME(s)     (s).szFileName 

/*---------------------------------------------------------------------------
**  벨/진동 설정
*/
enum tagBELL_VIBRATION_E
{
    BV_BELL,                    /* 벨 */
    BV_VIBRATION,               /* 진동 */
    BV_BELL_AFTER_VIBRATION,    /* 진동 후 벨 */
    BV_BOTH_BELL_VIBRATION      /* 진동과 벨 동시 */
};
typedef enum tagBELL_VIBRATION_E            BELL_VIBRATION_E;

/*---------------------------------------------------------------------------
**  벨 울림 시간 조절
*/
enum tagBELL_TIME_E
{
    BT_CONTINUATION,            /* 계속 울림 */
    BT_5SECOND,                 /* 5초간 울림 */
    BT_10SECOND,                /* 10초간 울림 */
    BT_15SECOND                 /* 15초간 울림 */
};
typedef enum tagBELL_TIME_E                 BELL_TIME_E;

/*===========================================================================
**  벨/진동
*/
struct tagBELL_VIBRATION_T
{
    BELL_TYPE_T         Type;           /* 벨 종류 */
    BELL_VIBRATION_E    BellVibration;  /* 벨/진동 설정 */
    INT8                nVolume;        /* 벨 음량 */
    BELL_TIME_E         Time;           /* 벨 울림 시간 조절 */
};
typedef struct tagBELL_VIBRATION_T          BELL_VIBRATION_T;

#define sGET_BELL_TYPE(s)           (s).Type
#define sGET_BELL_VIBRATION(s)      (s).BellVibration
#define sGET_BELL_VOLUME(s)         (s).nVolume
#define sGET_BELL_TIME(s)           (s).Time

/*---------------------------------------------------------------------------
**  그림
*/
#define sPICTURE_NOT_USED   0xFFFF  /* 그림이 사용되지 않음 */
#define sMY_PICTURE         0xFFFE  /* 나만의 그림 */
#define sMY_PHOTO           0xFFFD  /* 나만의 사진 */
#define sMY_PHOTO_ALL       0xFFFC  /* 나만의 사진 - 슬라이드 */

#define IS_PICTURE_NOT_USED(w)    ((w) == sPICTURE_NOT_USED)
#define IS_MY_PICTURE(w)    ((w) == sMY_PICTURE)
#define IS_MY_PHOTO(w)      ((w) == sMY_PHOTO)
#define IS_MY_PHOTO_ALL(w)      ((w) == sMY_PHOTO_ALL)

#ifdef FEATURE_CHARACBELL
#define sC_PICTURE          0xFFFB  // 캐릭벨
#define IS_CHARAC_PICTURE(w)    ((w) == sC_PICTURE)
#endif // FEATURE_CHARACBELL

/* PictureID가 PICTURE_NOT_USED이면 해당 경우에 그림을 사용하지 않음
   따로 필드를 두는 것보다 메모리를 절약할 수 있어서... */
struct tagPICTURE_TYPE_T 
{
    WORD    wPictureID;                         /* 그림 ID */
    BYTE    szFileName[MAX_FILE_NAME_LENGTH+1];   /* 나만의 그림 파일 이름 */
};
typedef struct tagPICTURE_TYPE_T            PICTURE_TYPE_T;

#define sGET_PICTURE_TYPE_ID(s)     (s).wPictureID
#define sGET_PICTURE_FILE_NAME(s)   (sIS_MY_PICTURE(sGET_PICTURE_TYPE_ID(s))\
                                    ? (s).szFileName : SKY_ASSERT(FALSE);

#define sSET_PICTURE_TYPE_ID(s, w)  sGET_PICTURE_TYPE_ID(s) = w

/*---------------------------------------------------------------------------
**  그림 설정
*/
struct tagPICTURE_IN_CASE_T
{
    BOOL            fUse;           /* FALSE --> 전부 사용 안함 */
    PICTURE_TYPE_T  Idle;           /* 플립이 닫혔을 때, 폴더 열었을 때 */
    PICTURE_TYPE_T  IncomingCall;   /* 전화가 왔을 때 */
    PICTURE_TYPE_T  PowerOn;        /* 전원을 켤 때 */
    PICTURE_TYPE_T  PowerOff;       /* 전원을 끌 때 */
    PICTURE_TYPE_T  WAPStart;       /* WAP 시작 시 */
};
typedef struct tagPICTURE_IN_CASE_T         PICTURE_IN_CASE_T;

#define sIS_PICTURE_USE(s)              (s).fUse
#define sGET_IDLE_PICTURE_TYPE(s)       (s).Idle
#define sGET_INCOMING_PICTURE_TYPE(s)   (s).IncomingCall
#define sGET_POWER_ON_PICTURE_TYPE(s)   (s).PowerOn
#define sGET_POWER_OFF_PICTURE_TYPE(s)  (s).PowerOff
#define sGET_WAP_PICTURE_TYPE(s)        (s).WAPStart

/*---------------------------------------------------------------------------
**  대기화면 종류
*/
enum tagIDLE_SCREEN_E
{
    IS_CLOCK,               /* 시계만 보임 */
    IS_PICTURE             /* 그림보임 */
};
typedef enum tagIDLE_SCREEN_E               IDLE_SCREEN_E;

/* rosa 2001-09-20 --> @why, what */
/*---------------------------------------------------------------------------
**  시계화면 표시 방법
*/
enum tagIDLE_DISPLAY_WATCH_E
{
    IDW_NO,
    IDW_SMALL,
    IDW_BIG,
    IDW_ANALOG1,
    IDW_ANALOG2
};
typedef enum tagIDLE_DISPLAY_WATCH_E   IDLE_DISPLAY_WATCH_E;
/* <-- */

/*===========================================================================
**  그림 설정
*/
struct tagPICTURE_SETTING_T
{
    BOOL                        fDisplayName;
    IDLE_SCREEN_E               IdleScreen;
    /* rosa 2001-09-20 --> @why, what */
    //BOOL                      fDisplayWatch;
    IDLE_DISPLAY_WATCH_E        IdleDisplayWatch;
    /* <-- */
    PICTURE_IN_CASE_T           Picture;
};
typedef struct tagPICTURE_SETTING_T         PICTURE_SETTING_T;

#define sGET_IDLE_DISPLAY_NAME(s)       (s).fDisplayName
#define sGET_IDLE_SCREEN(s)             (s).IdleScreen
/* rosa 2001-09-20 --> @why, what */
#define sGET_IDLE_DISPLAY_WATCH(s)      (s).IdleDisplayWatch
/* <-- */
#define sGET_PICTURE_IN_CASE(s)         (s).Picture


/*
******************************************************************************
******************************************************************************
**
**
**  부가 기능
**
**
******************************************************************************
******************************************************************************
*/
/*---------------------------------------------------------------------------
**  자동 조명 설정
*/
enum tagAUTO_BACKLIGHT_E
{
    AB_10SECOND,         /* 항상 자동 조명 - 키 누르고 10초 동안 켜져 있음 */
    AB_ALWAYS,           /* 항상 켜 있음 - 핸즈 프리 키트에 놓여 있을 때 */
    AB_TIME,             /* 시간별 자동 조명 */
    AB_NO                /* 조명 사용 안함 */
};
typedef enum tagAUTO_BACKLIGHT_E        AUTO_BACKLIGHT_E;

#define AUTO_BACKLIGHT_TIME             10   /* 10초 */

/*---------------------------------------------------------------------------
**  시간별 자동 조명
*/
struct tagAUTO_BACKLIGHT_TIME_T 
{
    /* Number of seconds since base date */
    DWORD   dwStartTime;                /* 시작 시간 */
    DWORD   dwEndTime;                  /* 종료 시간 */
};
typedef struct tagAUTO_BACKLIGHT_TIME_T AUTO_BACKLIGHT_TIME_T;

#define sGET_AUTO_BACKLIGHT_START_TIME(s)    (s).dwStartTime
#define sGET_AUTO_BACKLIGHT_END_TIME(s)      (s).dwEndTime

#ifdef FEATURE_COLOR_ID_LCD       //white
enum tag_ID_LCD_COLOR_E {
    BLUE_LCD_COLOR,
    GREEN_LCD_COLOR,
    RED_LCD_COLOR,
    VIOLET_LCD_COLOR,
    BLUE_GREEN_LCD_COLOR,
//    ORANGE_LCD_COLOR,
//    WHITE_LCD_COLOR,
    NONE_LCD_COLOR,
    RAINBOW_LCD_COLOR,
    ILC_NO /* internal use only */
};

typedef enum tag_ID_LCD_COLOR_E ID_LCD_COLOR_E;

struct tagID_LCD_COLOR_SELECT_T
{
    ID_LCD_COLOR_E   WhenIdle;
    ID_LCD_COLOR_E   WhenCallAlert;          
    ID_LCD_COLOR_E   WhenSmsAlert; 
    ID_LCD_COLOR_E   WhenAlarm;     /* jrkwon : 2001-03-27 Alarm */
};

typedef struct tagID_LCD_COLOR_SELECT_T       ID_LCD_COLOR_SELECT_T;

#define sGET_IDLCD_COLOR_IDLE(s)        (s).WhenIdle
#define sGET_IDLCD_COLOR_ALERT(s)       (s).WhenCallAlert
#define sGET_IDLCD_COLOR_SMS(s)         (s).WhenSmsAlert
#define sGET_IDLCD_COLOR_ALARM(s)       (s).WhenAlarm

#define NUM_LCD_SELECT_PLACE                3
#define NONE_GROUP_ALERT_LCD                10
#define MAX_ID_LCD_NUMBER                   8
#define MAX_ALERT_ID_LCD_NUMBER             9
#endif
/*---------------------------------------------------------------------------
**  자동 착신
*/
struct tagAUTO_RECEIPT_T
{
    BOOL    fHandsFree;
    BOOL    fEarPhone;
};
typedef struct tagAUTO_RECEIPT_T    AUTO_RECEIPT_T;

#define sIS_HANDS_FREE_AUTO_RECEIPT(s)      (s).fHandsFree
#define sIS_EAR_PHONE_AUTO_RECEIPT(s)       (s).fEarPhone

/*---------------------------------------------------------------------------
**  넘버 플러스 / 투 넘버 서비스
*/
struct tagNUMBER_PLUS_T
{
    BOOL    fDisplay;                              /* 화면 표시 */   
    BYTE    szSKTNumber[MAX_DIAL_DIGIT_LENGTH+1];  /* SKT 추가 번호 */
    BYTE    szSTINumber[MAX_DIAL_DIGIT_LENGTH+1];  /* STI 추가 번호 */
    BELL_TYPE_T PlusBell;                          /* 벨선택*/
};
typedef struct tagNUMBER_PLUS_T         NUMBER_PLUS_T;

#define sIS_NUMBER_PLUS_DISPLAY(s)          (s).fDisplay
//#define sGET_NUMBER_PLUS_NUMBER(s)          (s).szNumber
#define sGET_NUMBER_PLUS_NUMBER_SKT(s)      (s).szSKTNumber
#define sGET_NUMBER_PLUS_NUMBER_STI(s)      (s).szSTINumber
#define sGET_NUMBER_PLUS_BELL_ID(s)         (s).PlusBell.wBellID
#define sGET_NUMBER_PLUS_BELL_FILENAME(s)   (s).PlusBell.szFileName
/*============================================================================
**  부가 기능 - CONFIG_T 로 끝나는 구조체는 file system에 저장.
*/
struct tagMISC_CONFIG_T
{
    SKY_FILE_HEADER_T       Header;             /* 파일 헤더 */
    BOOL                    fAutoAreaCode;      /* 자동 지역 번호 ON/OFF */
    BYTE                    szAreaCode[MAX_AREA_CODE_LENGTH+1]; /* 지역 번호 */
    AUTO_BACKLIGHT_E        AutoBacklight;      /* 자동 조명 */
    AUTO_BACKLIGHT_TIME_T   AutoBacklightTime;  /* 시간별 자동 조명 */
    AUTO_BACKLIGHT_E        AutoSubBacklight;      /* ID LCD 자동 조명 */
    AUTO_BACKLIGHT_TIME_T   AutoSubBacklightTime;  /* ID LCD 시간별 항상 조명 */
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    ID_LCD_COLOR_SELECT_T   SubLcdColorSelect;
#endif
    BOOL                    fAutoRedial;        /* 자동 재 다이얼 */
    AUTO_RECEIPT_T          AutoReceipt;        /* 자동 착신 */
    NUMBER_PLUS_T           NumberPlus;         /* 넘버 플러스/투 넘버 서비스 */
    BYTE                    SRVC011[SKY_MAX_NUM_011_SERVICE][SKY_MAX_DIAL_011_SERVICE+1]; /* 011 부가 서비스*/
    BYTE                    SRVC017[SKY_MAX_NUM_017_SERVICE][SKY_MAX_DIAL_017_SERVICE+1]; /* 017 부가 서비스*/
};
typedef struct tagMISC_CONFIG_T         MISC_CONFIG_T;

/*
**===========================================================================
**  외부 LCD 조명색 설정 정보 읽기/쓰기
**===========================================================================
*/
#define SKY_GET_IDLCD_COLOR_IDLE()   \
        sGET_IDLCD_COLOR_IDLE(g_MiscConfig.SubLcdColorSelect)
#define SKY_GET_IDLCD_COLOR_ALERT()   \
        sGET_IDLCD_COLOR_ALERT(g_MiscConfig.SubLcdColorSelect)
#define SKY_GET_IDLCD_COLOR_SMS()   \
        sGET_IDLCD_COLOR_SMS(g_MiscConfig.SubLcdColorSelect)
#define SKY_GET_IDLCD_COLOR_ALARM()   \
        sGET_IDLCD_COLOR_ALARM(g_MiscConfig.SubLcdColorSelect)

#define SKY_SET_IDLCD_COLOR_IDLE(f)   \
        sSET_CONFIG_VALUE(SC_MISC, SKY_GET_IDLCD_COLOR_IDLE(), f)
#define SKY_SET_IDLCD_COLOR_ALERT(f)   \
        sSET_CONFIG_VALUE(SC_MISC, SKY_GET_IDLCD_COLOR_ALERT(), f)
#define SKY_SET_IDLCD_COLOR_SMS(f)   \
        sSET_CONFIG_VALUE(SC_MISC, SKY_GET_IDLCD_COLOR_SMS(), f)
#define SKY_SET_IDLCD_COLOR_ALARM(f)   \
        sSET_CONFIG_VALUE(SC_MISC, SKY_GET_IDLCD_COLOR_ALARM(), f)


/*
**===========================================================================
**  011부가 서비스 정보 읽기
**===========================================================================
*/
#define SKY_GET_011SERVICE_SORISAM_CON()   g_MiscConfig.SRVC011[SKY_VOICE_POND]

#define SKY_GET_011SERVICE_SORISAM_SET()   g_MiscConfig.SRVC011[SKY_VOICE_POND_SET]

#define SKY_GET_011SERVICE_SORISAM_IMME() g_MiscConfig.SRVC011[SKY_VOICE_POND_IMME]

#define SKY_GET_011SERVICE_SORISAM_RESET() g_MiscConfig.SRVC011[SKY_VOICE_POND_RESET]

#define SKY_GET_011SERVICE_AUTOCON_SET() g_MiscConfig.SRVC011[SKY_AUTO_CONNECT_SET]

#define SKY_GET_011SERVICE_AUTOCON_RESET() g_MiscConfig.SRVC011[SKY_AUTO_CONNECT_RESET]

#define SKY_GET_011SERVICE_CALLREDIR_SET() g_MiscConfig.SRVC011[SKY_CALL_REDIR_SET]

#define SKY_GET_011SERVICE_CALLREDIR_RESET() g_MiscConfig.SRVC011[SKY_CALL_REDIR_RESET]

#define SKY_GET_011SERVICE_INFO_POND() g_MiscConfig.SRVC011[SKY_INFO_POND]

#define SKY_GET_011SERVICE_TRAFFIC_INFO() g_MiscConfig.SRVC011[SKY_TRAFFIC_INFO]

#define SKY_GET_011SERVICE_BAN_FOREIGN() g_MiscConfig.SRVC011[SKY_ORIG_BAN_FOREIGN]

#define SKY_GET_011SERVICE_BAN_ALL() g_MiscConfig.SRVC011[SKY_ORIG_BAN_ALL]

#define SKY_GET_011SERVICE_BAN_RESET() g_MiscConfig.SRVC011[SKY_ORIG_BAN_RESET]

#define SKY_GET_011SERVICE_FORWARD_HOT() g_MiscConfig.SRVC011[SKY_CALL_FORWARD_HOT]

#define SKY_GET_011SERVICE_PENDING_HOT() g_MiscConfig.SRVC011[SKY_CALL_PENDING_HOT]
/*
**===========================================================================
**  011부가 서비스 정보 쓰기
**===========================================================================
*/
#define SKY_SET_011SERVICE_SORISAM_CON(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_SORISAM_CON(), f)

#define SKY_SET_011SERVICE_SORISAM_SET(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_SORISAM_SET(), f)

#define SKY_SET_011SERVICE_SORISAM_IMME(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_SORISAM_IMME(), f)

#define SKY_SET_011SERVICE_SORISAM_RESET(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_SORISAM_RESET(), f)

#define SKY_SET_011SERVICE_AUTOCON_SET(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_AUTOCON_SET(), f)

#define SKY_SET_011SERVICE_AUTOCON_RESET(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_AUTOCON_RESET(), f)

#define SKY_SET_011SERVICE_CALLREDIR_SET(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_CALLREDIR_SET(), f)

#define SKY_SET_011SERVICE_CALLREDIR_RESET(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_CALLREDIR_RESET(), f)

#define SKY_SET_011SERVICE_INFO_POND(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_INFO_POND(), f)

#define SKY_SET_011SERVICE_TRAFFIC_INFO(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_TRAFFIC_INFO(), f)

#define SKY_SET_011SERVICE_BAN_FOREIGN(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_BAN_FOREIGN(), f)

#define SKY_SET_011SERVICE_BAN_ALL(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_BAN_ALL(), f)

#define SKY_SET_011SERVICE_BAN_RESET(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_BAN_RESET(), f)

#define SKY_SET_011SERVICE_FORWARD_HOT(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_FORWARD_HOT(), f)

#define SKY_SET_011SERVICE_PENDING_HOT(f)   \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_011SERVICE_PENDING_HOT(), f)

/*
**===========================================================================
**  017부가 서비스 정보 읽기
**===========================================================================
*/
#define SKY_GET_S017_VOICE_MAIL_CONNECT()     g_MiscConfig.SRVC017[S017_VOICE_MAIL_CONNECT]
#define SKY_GET_S017_VOICE_MAIL_SET()         g_MiscConfig.SRVC017[S017_VOICE_MAIL_SET]
#define SKY_GET_S017_VOICE_MAIL_CANCEL()      g_MiscConfig.SRVC017[S017_VOICE_MAIL_CANCEL]
#define SKY_GET_S017_AUTO_CON_REGISTER()      g_MiscConfig.SRVC017[S017_AUTO_CONNECT_REGISTER]
#define SKY_GET_S017_AUTO_CON_CANCEL()        g_MiscConfig.SRVC017[S017_AUTO_CONNECT_CANCEL]
#define SKY_GET_S017_REDIR_CALL_REGISTER()    g_MiscConfig.SRVC017[S017_CALL_REDIR_REGISTER]
#define SKY_GET_S017_REDIR_CALL_CANCEL()      g_MiscConfig.SRVC017[S017_CALL_REDIR_CANCEL]
#define SKY_GET_S017_CALL_WAITING_REGISTER()  g_MiscConfig.SRVC017[S017_CALL_WAITING_REGISTER]
#define SKY_GET_S017_CALL_WAITING_CANCEL()    g_MiscConfig.SRVC017[S017_CALL_WAITING_CANCEL]
#define SKY_GET_S017_POWER_REMOTE()           g_MiscConfig.SRVC017[S017_POWER_REMOTE]
#define SKY_GET_S017_POWER_MELODY()           g_MiscConfig.SRVC017[S017_POWER_MELODY]
#define SKY_GET_S017_FORWARD_HOT()            g_MiscConfig.SRVC017[S017_CALL_FORWARD_HOT]
#define SKY_GET_S017_PENDING_HOT()            g_MiscConfig.SRVC017[S017_CALL_PENDING_HOT]

/*
**===========================================================================
**  017 부가 서비스 정보 쓰기
**===========================================================================
*/
#define SKY_SET_S017_VOICE_MAIL_CONNECT(f)    sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_VOICE_MAIL_CONNECT(), f)
#define SKY_SET_S017_VOICE_MAIL_SET(f)        sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_VOICE_MAIL_SET(), f)
#define SKY_SET_S017_VOICE_MAIL_CANCEL(f)     sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_VOICE_MAIL_CANCEL(), f)
#define SKY_SET_S017_AUTO_CON_REGISTER(f)     sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_AUTO_CON_REGISTER(), f)
#define SKY_SET_S017_AUTO_CON_CANCEL(f)       sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_AUTO_CON_CANCEL(), f)
#define SKY_SET_S017_REDIR_CALL_REGISTER(f)   sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_REDIR_CALL_REGISTER(), f)
#define SKY_SET_S017_REDIR_CALL_CANCEL(f)     sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_REDIR_CALL_CANCEL(), f)
#define SKY_SET_S017_CALL_WAITING_REGISTER(f) sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_CALL_WAITING_REGISTER(), f)
#define SKY_SET_S017_CALL_WAITING_CANCEL(f)   sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_CALL_WAITING_CANCEL(), f)
#define SKY_SET_S017_POWER_REMOTE(f)          sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_POWER_REMOTE(), f)
#define SKY_SET_S017_POWER_MELODY(f)          sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_POWER_MELODY(), f)
#define SKY_SET_S017_FORWARD_HOT(f)           sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_FORWARD_HOT(), f)
#define SKY_SET_S017_PENDING_HOT(f)           sSET_CONFIG_STR(SC_MISC, SKY_GET_S017_PENDING_HOT(), f)
/* <-- */


/*
**===========================================================================
**  부가 기능 정보 읽기
**===========================================================================
*/
#define SKY_IS_AUTO_AREA_CODE()       g_MiscConfig.fAutoAreaCode
#define SKY_GET_AREA_CODE()           g_MiscConfig.szAreaCode

#define SKY_GET_AUTO_BACKLIGHT()      g_MiscConfig.AutoBacklight

#define SKY_GET_AUTO_SUBBACKLIGHT()      g_MiscConfig.AutoSubBacklight

//            SKY_ASSERT(SKY_GET_AUTO_BACKLIGHT == AB_TIME);
#define SKY_GET_AUTO_BACKLIGHT_START_TIME() \
            sGET_AUTO_BACKLIGHT_START_TIME(g_MiscConfig.AutoBacklightTime)

#define SKY_GET_AUTO_BACKLIGHT_END_TIME() \
            sGET_AUTO_BACKLIGHT_END_TIME(g_MiscConfig.AutoBacklightTime)

#define SKY_GET_AUTO_SUBBACKLIGHT_START_TIME() \
            sGET_AUTO_BACKLIGHT_START_TIME(g_MiscConfig.AutoSubBacklightTime)

#define SKY_GET_AUTO_SUBBACKLIGHT_END_TIME() \
            sGET_AUTO_BACKLIGHT_END_TIME(g_MiscConfig.AutoSubBacklightTime)

#define SKY_IS_AUTO_REDIAL()            g_MiscConfig.fAutoRedial

#define SKY_IS_HANDS_FREE_AUTO_RECEIPT()  \
                    sIS_HANDS_FREE_AUTO_RECEIPT(g_MiscConfig.AutoReceipt)

#define SKY_IS_EAR_PHONE_AUTO_RECEIPT()  \
                    sIS_EAR_PHONE_AUTO_RECEIPT(g_MiscConfig.AutoReceipt)

#define SKY_IS_NUMBER_PLUS() sIS_NUMBER_PLUS_DISPLAY(g_MiscConfig.NumberPlus)

#define SKY_GET_NUMBER_PLUS_NUMBER_SKT()  sGET_NUMBER_PLUS_NUMBER_SKT( \
                                                    g_MiscConfig.NumberPlus)

#define SKY_GET_NUMBER_PLUS_NUMBER_STI()  sGET_NUMBER_PLUS_NUMBER_STI( \
                                                    g_MiscConfig.NumberPlus)

#define SKY_GET_NUMBER_PLUS_BELL_ID()   sGET_NUMBER_PLUS_BELL_ID( \
                                                    g_MiscConfig.NumberPlus)   

#define SKY_GET_NUMBER_PLUS_BELL_FILENAME() sGET_NUMBER_PLUS_BELL_FILENAME( \
                                                    g_MiscConfig.NumberPlus) 
/*
**===========================================================================
**  부가 기능 정보 쓰기
**===========================================================================
*/
#define SKY_SET_AUTO_AREA_CODE(f)   \
        sSET_CONFIG_VALUE(SC_MISC, SKY_IS_AUTO_AREA_CODE(), f)

#define SKY_SET_AREA_CODE(a)        \
        sSET_CONFIG_STR(SC_MISC, SKY_GET_AREA_CODE(), a)

#define SKY_SET_AUTO_BACKLIGHT(b)   \
        sSET_CONFIG_VALUE(SC_MISC, SKY_GET_AUTO_BACKLIGHT(), b)

#define SKY_SET_AUTO_SUBBACKLIGHT(b)   \
        sSET_CONFIG_VALUE(SC_MISC, SKY_GET_AUTO_SUBBACKLIGHT(), b)

#define SKY_SET_AUTO_BACKLIGHT_START_TIME(t)                            \
{                                                                       \
    if(SKY_GET_AUTO_BACKLIGHT() == AB_TIME)                               \
    {   sSET_CONFIG_VALUE(SC_MISC,                                      \
                    SKY_GET_AUTO_BACKLIGHT_START_TIME(), t)               \
    } \
}
#define SKY_SET_AUTO_BACKLIGHT_END_TIME(t)                              \
{                                                                       \
    if(SKY_GET_AUTO_BACKLIGHT() == AB_TIME)                               \
    {   sSET_CONFIG_VALUE(SC_MISC,                                      \
                    SKY_GET_AUTO_BACKLIGHT_END_TIME(), t)                 \
    } \
}

#define SKY_SET_AUTO_SUBBACKLIGHT_START_TIME(t)                            \
{                                                                       \
    if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME)                               \
    {   sSET_CONFIG_VALUE(SC_MISC,                                      \
                    SKY_GET_AUTO_SUBBACKLIGHT_START_TIME(), t)               \
    } \
}
#define SKY_SET_AUTO_SUBBACKLIGHT_END_TIME(t)                              \
{                                                                       \
    if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME)                               \
    {   sSET_CONFIG_VALUE(SC_MISC,                                      \
                    SKY_GET_AUTO_SUBBACKLIGHT_END_TIME(), t)                 \
    } \
}

#define SKY_SET_AUTO_REDIAL(f)                                              \
        sSET_CONFIG_VALUE(SC_MISC, SKY_IS_AUTO_REDIAL(), f) 

#define SKY_SET_HANDS_FREE_AUTO_RECEIPT(f)                                  \
        sSET_CONFIG_VALUE(SC_MISC,                                          \
                          SKY_IS_HANDS_FREE_AUTO_RECEIPT(), f)
#define SKY_SET_EAR_PHONE_AUTO_RECEIPT(f)                                   \
        sSET_CONFIG_VALUE(SC_MISC,                                          \
                          SKY_IS_EAR_PHONE_AUTO_RECEIPT(), f)
#define SKY_SET_NUMBER_PLUS(f)                                              \
        sSET_CONFIG_VALUE(SC_MISC, SKY_IS_NUMBER_PLUS(), f)
#define SKY_SET_NUMBER_PLUS_NUMBER_SKT(s)                                   \
        sSET_CONFIG_STR(SC_MISC,                                            \
                          SKY_GET_NUMBER_PLUS_NUMBER_SKT(), s)
#define SKY_SET_NUMBER_PLUS_NUMBER_STI(s)                                   \
        sSET_CONFIG_STR(SC_MISC,                                            \
                          SKY_GET_NUMBER_PLUS_NUMBER_STI(), s)

#define SKY_SET_NUMBER_PLUS_BELL_ID(s)                                      \
        sSET_CONFIG_VALUE(SC_MISC,                                          \
                          SKY_GET_NUMBER_PLUS_BELL_ID(), s)
#define SKY_SET_NUMBER_PLUS_BELL_FILENAME(s)                                \
        sSET_CONFIG_STR(SC_MISC,                                            \
                          SKY_GET_NUMBER_PLUS_BELL_FILENAME(), s)

/*
******************************************************************************
******************************************************************************
**
**
**   단말기 기본 기능 설정
**
**
******************************************************************************
******************************************************************************
*/

/*---------------------------------------------------------------------------
**  자동 응답 벨 울림 시간
*/
enum tagAUTO_ANSWER_BELL_E              /* 자동 응답 벨 울림 시간 */
{
    AAB_IMMEDIATE,                      /* 즉시 자동응답 */
    AAB_VIB_ONCE,                       /* 진동 1회 */
    AAB_BELL_5SECOND,                   /* 벨 5초 */
    AAB_BELL_10SECOND                   /* 벨 10초 */
};
typedef enum tagAUTO_ANSWER_BELL_E      AUTO_ANSWER_BELL_E;

/*---------------------------------------------------------------------------
**  안내 음성
*/
enum tagGREETING_MESSAGE_E              /* 안내 음성 선택 */
{
    GM_GENERAL,                         /* 일반 - 시스템 기본 */
    GM_DRIVING,                         /* 운전 중 */
    GM_MEETING,                         /* 회의 중 */
    GM_USER                             /* 사용자 음성 */
};
typedef enum tagGREETING_MESSAGE_E      GREETING_MESSAGE_E;

/*===========================================================================
**  자동 응답
*/
struct tagAUTO_ANSWER_T                 /* 자동 응답 */
{
    BOOL                fOn;            /* on/off */
    GREETING_MESSAGE_E  Greeting;       /* 안내 음성 */
    AUTO_ANSWER_BELL_E  BellTime;       /* 자동 응답 벨 울림 시간 */
    INT8                nCount;         /* 자동 응답 메시지 개수  */
    INT8                nRing;          /* 자동 응답 시작전 벨울림 횟수*/
};
typedef struct tagAUTO_ANSWER_T         AUTO_ANSWER_T;

#define sIS_AUTO_ANSWER(s)               (s).fOn
#define sGET_AUTO_ANSWER_GREETING(s)     (s).Greeting
#define sGET_AUTO_ANSWER_BELL_TIME(s)    (s).BellTime
#define sGET_AUTO_ANSWER_COUNT(s)        (s).nCount
#define sGET_AUTO_ANSWER_RINGNUM(s)      (s).nRing

//#define sSET_AUTO_ANSWER(s, f)           (s).fOn        = f
//#define sSET_AUTO_ANSWER_GREETING(s, g)  (s).Greeting   = g
//#define sSET_AUTO_ANSWER_BELL_TIME(s, b) (s).BellTime   = b
//#define sSET_AUTO_ANSWER_COUNT(s, c)     (s).nCount     = c


/*---------------------------------------------------------------------------
**  잠금 기능
*/
struct tagPHONE_LOCK_T                  /* 잠금 기능 */
{
    BOOL fAuto;                         /* 자동 잠금 */
    BOOL fOverseasCall;                 /* 국제 전화 잠금 */
    BOOL fPhoneBook;                    /* 전화부 잠금 */
    BOOL fMenu;                         /* 메뉴 잠금 */
    BOOL fCallStack;                    /* 착발신 이력 잠금 */
    BOOL fRecordPlay;                   /* 녹음/재생 잠금 */
    BOOL fMemo;                         /* 메모 잠금 */
    BOOL fWAP;                          /* n.TOP or i-Touch 잠금 */
    BOOL fSMS;                          /* SMS 잠금 */
	BOOL fCamera;						/* Camera 잠금 */
};
typedef struct tagPHONE_LOCK_T          PHONE_LOCK_T;

#define sIS_AUTO_LOCK(s)              (s).fAuto
#define sIS_OVERSEAS_CALL_LOCK(s)     (s).fOverseasCall
#define sIS_PHONE_BOOK_LOCK(s)        (s).fPhoneBook
#define sIS_MENU_LOCK(s)              (s).fMenu
#define sIS_CALL_STACK_LOCK(s)        (s).fCallStack
#define sIS_RECORD_PLAY_LOCK(s)       (s).fRecordPlay
#define sIS_MEMO_LOCK(s)              (s).fMemo
#define sIS_WAP_LOCK(s)               (s).fWAP
#define sIS_SMS_LOCK(s)               (s).fSMS
#define sIS_CAMERA_LOCK(s)            (s).fCamera

#define sSET_AUTO_LOCK(s, f)          (s).fAuto          = f
#define sSET_OVERSEAS_CALL_LOCK(s, f) (s).fOverseasCall  = f
#define sSET_PHONE_BOOK_LOCK(s, f)    (s).fPhoenBook     = f
#define sSET_MENU_LOCK(s, f)          (s).fMenu          = f
#define sSET_CALL_STACK_LOCK(s, f)    (s).fCallStack     = f
#define sSET_RECORD_PLAY_LOCK(s, f)   (s).fRecordPlay    = f
#define sSET_MEMO_LOCK(s, f)          (s).fMemo          = f
#define sSET_WAP_LOCK(s, f)           (s).fWAP           = f
#define sSET_SMS_LOCK(s, f)           (s).fSMS           = f
#define sSET_CAMERA_LOCK(s, f)        (s).fCamera        = f

//#ifdef FEATURE_SKY_MODEL_TYPE_FOLDER

/*---------------------------------------------------------------------------
**  전화 받기 방법 - 폴더
*/
enum tagHOW_TO_ANSWER_E
{
    HTA_OPEN,           /* 폴더 열기 */
    HTA_OPEN_AND_KEY    /* 폴더 열고 키 누름 */
};
typedef enum tagHOW_TO_ANSWER_E         HOW_TO_ANSWER_E;

//#else defined FEATURE_SKY_MODEL_TYPE_FLIP

/*---------------------------------------------------------------------------
**  플립키 설정 - 플립
*/
enum tagFLIP_KEY_E
{
    FK_AUTO_ANSWER,     /* 자동 응답으로  */
    FK_MEMORY_DIAL      /* 메모리 다이얼로 */
};
typedef enum tagFLIP_KEY_E              FLIP_KEY_E;

//#endif /* FEATURE_SKY_MODEL_TYPE */

/*---------------------------------------------------------------------------
**  NAM 설정 -- cm.h 의 cm_nam_e_type(CM_NAM_E)을 사용한다.
*/
/*enum tagNAM_TYPE_E
{
    NT_NAM1,
    NT_NAM2,
    NT_NAM3,
    NT_NAM4
};
typedef enum tagNAM_TYPE_E                  NAM_TYPE_E;
*/

/*===========================================================================
**  WAP 설정
*/
struct tagWAP_SETTING_T
{
    BYTE    szProxyURL[MAX_NAME_LENGTH+1];     /* 프록시 주소 */
    BYTE    szInitURL[MAX_URL_LENGTH+1];      /* 초기 URL */
	BYTE    szInitMAIL[MAX_WAPMAIL_LENGTH+1]; /* 초기 WAP mail Server주소 */
	WORD	szWapDormanttime;                 /* Wap Dormant Time*/
};
typedef struct tagWAP_SETTING_T             WAP_SETTING_T;

/*===========================================================================
**  SKVM 설정
*/
struct tagSKVM_SETTING_T
{
	BOOL    fSKVMColorEnv;        /* skvm 사용 칼라 지정*/
	BOOL	fSetJavaAsBackGround; /* 자바 컨텐츠가 배경화면으로 지정 */	
    BYTE    nArgcInSkvm;          /* 배경화면으로 자바 컨텐츠를 실행하기 위함 xvm_setargs(argc,argv)에 사용하게될 argc */
    BYTE    szArgvInSkvm[MAX_OF_ARGV+1];		  /* 배경화면으로 자바 컨텐츠를 실행하기 위함 xvm_setargs(argc,argv)에 사용하게될 argv */
};

typedef struct tagSKVM_SETTING_T              SKVM_SETTING_T;

#define sGET_WAP_PROXY_URL(s)               (s).szProxyURL
#define sGET_WAP_INIT_URL(s)                (s).szInitURL
#define sGET_WAP_INIT_MAIL(s)               (s).szInitMAIL
#define sGET_WAP_DORMANT_TIME(s)            (s).szWapDormanttime


/*---------------------------------------------------------------------------
**  마이크 크기
*/
enum tagMIC_VOLUME_E 
{
    MV_HIGH,        /* 크게 */
    MV_MEDIUM,      /* 중간 */
    MV_LOW          /* 작게 */
};
typedef enum tagMIC_VOLUME_E                MIC_VOLUME_E;

/*===========================================================================
**  음량
*/
struct tagVOLUME_T
{
    INT8            nSpeaker;   /* 통화음 크기 */
    INT8            nKeyTone;   /* 키톤 크기 */
    MIC_VOLUME_E    MicVolume;  /* 마이크 크기 */
    INT8            nGameVolume;    /* 게임 음량 */
};
typedef struct tagVOLUME_T                  VOLUME_T;

#define sGET_SPEAKER_VOLUME(s)      (s).nSpeaker
#define sGET_KEY_TONE_VOLUME(s)     (s).nKeyTone
#define sGET_MIC_VOLUME(s)          (s).MicVolume
#define sGET_GAME_VOLUME(s)          (s).nGameVolume

/*===========================================================================
**  경보음
*/
struct tagWARNING_SIGNAL_T
{
    BOOL            fConnect;   /* 통화연결음 */
    BOOL            f1Minute;   /* 1분 경과음 */
    BOOL            fNoService; /* 서비스 이탈 경보 */
    BOOL            fLoBatt;    /* 저전압 경보 */
    BOOL            fMsgBeep;   /* 메시지효과음 */
};
typedef struct tagWARNING_SIGNAL_T         WARNING_SIGNAL_T;

#define sIS_CONNECT_WARNING_SIGNAL(s)       (s).fConnect
#define sIS_1MINUTE_WARNING_SIGNAL(s)       (s).f1Minute
#define sIS_NOSERVICE_WARNING_SIGNAL(s)     (s).fNoService
#define sIS_LOBATT_WARNING_SIGNAL(s)        (s).fLoBatt
#define sIS_MESSAGE_BEEP_SOUND(s)           (s).fMsgBeep

/*---------------------------------------------------------------------------
**  착신 모드
*/
enum tagRECEIPT_MODE_E
{
    RM_VOICE,       /* 음성 착신 */
    RM_MODEM,       /* 모뎀 착신 */
    RM_FAX          /* 팩스 착신 */
};
typedef enum tagRECEIPT_MODE_E              RECEIPT_MODE_E;

/*---------------------------------------------------------------------------
**  인터넷속도설정
*/
enum tagINTERNET_SPEED_E
{
    IS_MAX,         /* 최대 설정 */
    IS_NORMAL       /* 보통 설정 */
};
typedef enum tagINTERNET_SPEED_E            INTERNET_SPEED_E;

/*---------------------------------------------------------------------------
**  PC와 통신 속도
*/
enum tagBPS_PC_E
{
    BP_115200,      /* 115,200 bps */
    BP_19200        /* 19,200 bps */
};
typedef enum tagBPS_PC_E                    BPS_PC_E;

/*---------------------------------------------------------------------------
**  AIR 데이터 매니저
*/
struct tagAIR_DATA_MANAGER_T
{
    BYTE szPhoneNumber[MAX_DIAL_DIGIT_LENGTH+1];  /* 연결할 전화 번호(모뎀착신) */
    BYTE szIPAddress[MAX_IP_LENGTH+1];            /* 접속할 IP 주소 */
};
typedef struct tagAIR_DATA_MANAGER_T        AIR_DATA_MANAGER_T;

#define sGET_AIR_DATA_MANAGER_PHONE_NUMBER(s)   (s).szPhoneNumber
#define sGET_AIR_DATA_MANAGER_IP_ADDRESS(s)     (s).szIPAddress

/*===========================================================================
**  데이터 서비스
*/
struct tagDATA_SERVICE_T
{
    RECEIPT_MODE_E      ReceiptMode;
    INTERNET_SPEED_E    InternetSpeed;
    BPS_PC_E            BpsPC;
    AIR_DATA_MANAGER_T  AirDataManager;
};
typedef struct tagDATA_SERVICE_T            DATA_SERVICE_T;

#define sGET_DATA_SERVICE_RECEIPT_MODE(s)       (s).ReceiptMode
#define sGET_DATA_SERVICE_INTERNET_SPEED(s)     (s).InternetSpeed
#define sGET_DATA_SERVICE_BPS_PC(s)             (s).BpsPC
#define sGET_AIR_DATA_MANAGER(s)                (s).AirDataManager

/*===========================================================================
**  전자메일 계정
*/

#define MAIL_USERNAME       33
#define MAIL_USERID         33
#define MAIL_PASSWORD       33
#define MAIL_SMTP           33
#define MAIL_POP3           33
#define MAIL_USERINFO       65
#define MAX_EMAIL_ADDR      34
#define MAX_SERVER_NAME     34

struct tagEMAIL_ACCOUNT_T
{
    BOOL fInUse;
    BYTE userName[MAIL_USERNAME];                 
    BYTE userID[MAIL_USERID];                 
    BYTE password[MAIL_PASSWORD]; 
    BYTE smtp[MAIL_SMTP]; 
    BYTE pop3[MAIL_POP3];  
    BYTE user_info[MAIL_USERINFO];   
	BYTE email[MAX_EMAIL_ADDR];              
	BYTE server[MAX_SERVER_NAME];            
};
typedef struct tagEMAIL_ACCOUNT_T            EMAIL_ACCOUNT_T;

#define sIS_EMAIL_ACCOUNT(s)            (s).fInUse
#define sGET_EMAIL_USERNAME(s)          (s).userName
#define sGET_EMAIL_USERID(s)            (s).userID
#define sGET_EMAIL_PASSWORD(s)          (s).password
#define sGET_EMAIL_SMTP(s)              (s).smtp
#define sGET_EMAIL_POP3(s)              (s).pop3
#define sGET_EMAIL_USERINFO(s)          (s).user_info
#define sGET_EMAIL_ADDRESS(s)           (s).email
#define sGET_EMAIL_SERVER(s)            (s).server

/*===========================================================================
**  화면 테마(화면 배색, 폰트, 스크롤바 크기 
*/
struct tagSCREEN_THEME_T {
    /* rosa 2001-09-25 --> @why, what */
    COLOR_SCHEME_E      ColorScheme; //<-- 나중에 지워져야함.
    //COLOR_SCHEME_BACK_E     ColorSchemeBack;
    //COLOR_SCHEME_FORE_E     ColorSchemeFore;
    /* <-- */
    FONT_TYPE       FontType;
    UINT8           nScrollBarSize;
};
typedef struct tagSCREEN_THEME_T            SCREEN_THEME_T;

#define sGET_COLOR_SCHEME(s)                (s).ColorScheme
/* rosa 2001-09-25 --> @why, what */
//#define sGET_COLOR_SCHEME_BACK(s)           (s).ColorSchemeBack
//#define sGET_COLOR_SCHEME_FORE(s)           (s).ColorSchemeFore
/* <-- */
#define sGET_FONT_TYPE(s)                   (s).FontType
#define sGET_SCROLLBAR_SIZE(s)              (s).nScrollBarSize


/*===========================================================================
**  버전 정보 - 0,0,0,0
*/
struct tagVERSION_INFO_T
{
    BYTE szDMSS[MAX_VERSION_INFO_LENGTH+1];
    BYTE szEQUS[MAX_VERSION_INFO_LENGTH+1];
    BYTE szSKY[MAX_VERSION_INFO_LENGTH+1];
};
typedef struct tagVERSION_INFO_T            VERSION_INFO_T;

#define sGET_DMSS_VERSION_INFO(s)           (s).szDMSS
#define sGET_EQUS_VERSION_INFO(s)           (s).szEQUS
#define sGET_SKY_VERSION_INFO(s)            (s).szSKY



/*===========================================================================
**  최근 발신 정보
*/
typedef struct tagEDIT_T                EDITORINFO_T;   
struct tagEDIT_T{                        /* editor buffer type */
  BOOL    fSecret;                       /* secret number or not */
  BYTE    aBuf[MAX_DIAL_DIGITS+2];       /* the data */
  UINT8   nPos;                          /* length of the data */
};

#define sGET_LASTDIALINFO_BUFF(s)          (&((s).aBuf))
#define sGET_LASTDIALINFO_SECRET(s)        (s).fSecret
#define sGET_LASTDIALINFO_POS(s)           (s).nPos





/*===========================================================================
**  단말기 환경 정보 - CONFIG_T 로 끝나는 구조체는 file system에 저장.
**		AS TOOL에서도 사용하고 있습니다. 변경점이 있으면 이길수에게 말씀해 주시길 부탁드립니다.
*/
/* XXXX_CONFIG_T --> being saved to the file system */
struct tagSYSTEM_CONFIG_T
{
    SKY_FILE_HEADER_T   Header;                             /* 파일 헤더 */
    EQS_COUNTRY         Country;                            /* 국가 설정 */
    BOOL                fEtiquette;                         /* 에티켓 */
    BOOL                fLoudSpeaker;                       /* 외부 스피커 */
    BYTE                szBanner[MAX_BANNER_LENGTH+1];        /* 휴대폰 이름 */
    BYTE                szPassword[MAX_PASSWORD_LENGTH+1];  /* 암호 */
    AUTO_ANSWER_T       AutoAnswer;                         /* 자동 응답 */
    PHONE_LOCK_T        PhoneLock;                          /* 잠금 기능 */
//#if (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FOLDER)
    HOW_TO_ANSWER_E     HowToAnswer;                        /* 전화받기 방법 */
//#elif (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FLIP)
    FLIP_KEY_E          FlipKey;                            /* 플립 키 설정 */
    BYTE                szMemoryDial[MAX_DIAL_DIGIT_LENGTH+1];/* 메모리 다이얼번호 */
//#endif /* FEATURE_SKY_MODEL_TYPE */
    CM_NAM_E            NAMType;                            /* NAM */
    INT8                nContrast;                          /* 화면 밝기 */
    WAP_SETTING_T       WapSetting;                         /* WAP 설정 */
    BELL_VIBRATION_T    BellVibration;                      /* 벨/진동 */
    PICTURE_SETTING_T   PictureSetting;                     /* 그림 설정 */
    VOLUME_T            Volume;                             /* 음량 */
    WARNING_SIGNAL_T    WarningSignal;                      /* 경보음 */
    DATA_SERVICE_T      DataService;                        /* 데이터서비스 */
    EMAIL_ACCOUNT_T     EMailAccount;                       /* 전자메일 계정 */
    SCREEN_THEME_T      ScreenTheme;                        /* 배색 */
    VERSION_INFO_T      VersionInfo;                        /* 버전 */
    EDITORINFO_T        LastDialInfo;                       /* 최근 발신 번호 */
    DWORD               dwCameraCount;                      /* 카메라 카운트 */
    BOOL                fSKYHangul;                         /* 한글입력방안 */
    BOOL                fIconMenu;                          /* 아이콘 모드 메뉴 */
    BOOL                fSmartBattery;                      /* 스마트 배터리 표시 */
    BOOL                f011Service;                        /* 011 부가 서비스(소리샘즉시연결, 착신전환)*/
    DWORD               dwPMCount;                          /* 포토메일 카운트 */
    INT8                nIDContrast;                        /* ID LCD 화면 밝기 */
    BOOL                fShortLock;                         /* #키를 이용한 잠금기능 */
    BOOL                fSelfInfoLock;                      /* 자기 정보 잠금 */
	SKVM_SETTING_T		SkvmSetting;
};
typedef struct tagSYSTEM_CONFIG_T        SYSTEM_CONFIG_T;

/*
**===========================================================================
**  시스템 정보 읽기
**===========================================================================
*/

/*---------------------------------------------------------------------------
** 국가 설정 
*/
#define SKY_GET_COUNTRY()              g_SysConfig.Country

/*---------------------------------------------------------------------------
** 에티켓 
*/
#define SKY_IS_ETIQUETTE()             g_SysConfig.fEtiquette

/*---------------------------------------------------------------------------
** 스피커
*/
#define SKY_IS_LOUD_SPEAKER()          g_SysConfig.fLoudSpeaker

/*---------------------------------------------------------------------------
** 휴대폰 이름 
*/
#define SKY_GET_BANNER()               g_SysConfig.szBanner

/*---------------------------------------------------------------------------
** 암호 
*/
#define SKY_GET_PASSWORD()             g_SysConfig.szPassword

/*---------------------------------------------------------------------------
** 자동 응답 
*/
/* 설정 */
#define SKY_IS_AUTO_ANSWER()           sIS_AUTO_ANSWER(g_SysConfig.AutoAnswer)
/* 인사말 */
#define SKY_GET_AUTO_ANSWER_GREETING() sGET_AUTO_ANSWER_GREETING( \
                                                    g_SysConfig.AutoAnswer)
/* 벨 울림 시간 */
#define SKY_GET_AUTO_ANSWER_BELL_TIME() sGET_AUTO_ANSWER_BELL_TIME( \
                                                    g_SysConfig.AutoAnswer)
/* 메시지 개수 */
#define SKY_GET_AUTO_ANSWER_COUNT()     sGET_AUTO_ANSWER_COUNT( \
                                                    g_SysConfig.AutoAnswer)
/* 자동응답 시작 벨 회수 */
#define SKY_GET_AUTO_ANSWER_RINGNUM()   sGET_AUTO_ANSWER_RINGNUM(\
                                                    g_SysConfig.AutoAnswer)

/*---------------------------------------------------------------------------
** 자동 잠금
*/
/* 자동 잠금 */
#define SKY_IS_AUTO_LOCK()            sIS_AUTO_LOCK(g_SysConfig.PhoneLock)
/* 국제 전화 잠금 */
#define SKY_IS_OVERSEAS_CALL_LOCK()   sIS_OVERSEAS_CALL_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* 전화부 잠금 */
#define SKY_IS_PHONE_BOOK_LOCK()      sIS_PHONE_BOOK_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* 메뉴 잠금 */
#define SKY_IS_MENU_LOCK()            sIS_MENU_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* 착발신 이력 잠금 */
#define SKY_IS_CALL_STACK_LOCK()      sIS_CALL_STACK_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* 녹음/재생 잠금 */
#define SKY_IS_RECORD_PLAY_LOCK()     sIS_RECORD_PLAY_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* 메모 잠금 */
#define SKY_IS_MEMO_LOCK()            sIS_MEMO_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* WAP 잠금 */
#define SKY_IS_WAP_LOCK()             sIS_WAP_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* SMS 잠금 */
#define SKY_IS_SMS_LOCK()             sIS_SMS_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* CAMERA 잠금 */
#define SKY_IS_CAMERA_LOCK()          sIS_CAMERA_LOCK( \
                                                    g_SysConfig.PhoneLock)

//#ifdef FEATURE_SKY_MODEL_TYPE_FOLDER
/*---------------------------------------------------------------------------
** 전화 받기 방법
*/
#define SKY_GET_HOW_TO_ANSWER()       g_SysConfig.HowToAnswer


/*----------------------------------------------------------------------------
** 한글 입력 방안 선택
*/
#define SKY_GET_SEL_HANGUL()       g_SysConfig.fSKYHangul

/*---------------------------------------------------------------------------
** 메뉴 모드 선택
*/
#define SKY_GET_SEL_MENU_MODE()       g_SysConfig.fIconMenu 

/*---------------------------------------------------------------------------
** 스마트 배터리 표시
*/
#define SKY_IS_SMART_BATTERY()       g_SysConfig.fSmartBattery

/*---------------------------------------------------------------------------
** 011 부가 서비스(소리샘 즉시연결, 착신전환)
*/
#define SKY_IS_011_SERVICE()       g_SysConfig.f011Service

//#elif FEATURE_SKY_MODEL_TYPE_FLIP
/*---------------------------------------------------------------------------
** 플립키 설정
*/
#define SKY_GET_FLIP_KEY()            g_SysConfig.FlipKey
#define SKY_GET_MEMORY_DIAL_NUMBER()  g_SysConfig.szMemoryDial

//#endif /* FEATURE_SKY_MODEL_TYPE_XXX */

/*---------------------------------------------------------------------------
** NAM -- NV에 저장해야 함
*/
#define SKY_GET_CURRENT_NAM()         g_SysConfig.NAMType

/*---------------------------------------------------------------------------
** 화면 밝기 
*/
#define SKY_GET_CONTRAST()            g_SysConfig.nContrast

/*---------------------------------------------------------------------------
** 화면 밝기 
*/
#define SKY_GET_IDCONTRAST()            g_SysConfig.nIDContrast

/*---------------------------------------------------------------------------
** #키를 이용한 잠금기능 
*/
#define SKY_GET_SHORTLOCK()            g_SysConfig.fShortLock

/*---------------------------------------------------------------------------
** 자기정보 잠금기능 
*/
#define SKY_GET_SELFINFOLOCK()            g_SysConfig.fSelfInfoLock

/*---------------------------------------------------------------------------
** SKVM 설정 
*/
/* SKVM 칼라 설정값  */
#define SKY_GET_SKVMCOLORENV()              g_SysConfig.SkvmSetting.fSKVMColorEnv
/* SKVM 배경 화면 설정 여부 */
#define SKY_GET_JAVAASBG()		            g_SysConfig.SkvmSetting.fSetJavaAsBackGround
/* SKVM argc */
#define SKY_GET_ARGCINSKVM()	            g_SysConfig.SkvmSetting.nArgcInSkvm
/* SKVM argv */
#define SKY_GET_ARGVINSKVM()				g_SysConfig.SkvmSetting.szArgvInSkvm

/*---------------------------------------------------------------------------
** WAP 설정
*/
/* 프록시 주소 */
#define SKY_GET_WAP_PROXY_URL()       sGET_WAP_PROXY_URL(g_SysConfig.WapSetting)
/* 초기 URL */
#define SKY_GET_WAP_INIT_URL()        sGET_WAP_INIT_URL(g_SysConfig.WapSetting)
/* 초기 WAP mail Server주소 */
#define SKY_GET_WAP_INIT_MAIL()       sGET_WAP_INIT_MAIL(g_SysConfig.WapSetting)
/* WAP Dormant Time*/
#define SKY_GET_WAP_DORMANT_TIME()    sGET_WAP_DORMANT_TIME(g_SysConfig.WapSetting)



/*---------------------------------------------------------------------------
** 벨/진동
*/
/* 벨 아이디 */
#define SKY_GET_BELL_ID()             sGET_BELL_ID(      \
                                            sGET_BELL_TYPE( \
                                                g_SysConfig.BellVibration))
/* 벨 파일 이름 */
#define SKY_GET_BELL_FILE_NAME()      sGET_BELL_FILE_NAME(    \
                                            sGET_BELL_TYPE( \
                                                g_SysConfig.BellVibration))
/* 벨/진동 설정 */
#define SKY_GET_BELL_VIBRATION()      sGET_BELL_VIBRATION( \
                                                g_SysConfig.BellVibration)
/* 벨 음량 */
#define SKY_GET_BELL_VOLUME()         sGET_BELL_VOLUME( \
                                                g_SysConfig.BellVibration)
/* 벨 울림 시간 */
#define SKY_GET_BELL_TIME()           sGET_BELL_TIME( \
                                                g_SysConfig.BellVibration)

/*---------------------------------------------------------------------------
** 그림 설정
*/
/* 대기 화면 종류 */
#define SKY_GET_IDLE_DISPLAY_NAME()   sGET_IDLE_DISPLAY_NAME( \
                                            g_SysConfig.PictureSetting)
#define SKY_GET_IDLE_SCREEN()         sGET_IDLE_SCREEN( \
                                            g_SysConfig.PictureSetting)
/* 대기 화면 표시 방법 */
#define SKY_GET_IDLE_DISPLAY_WATCH()    sGET_IDLE_DISPLAY_WATCH( \
                                            g_SysConfig.PictureSetting)
/* 그림 사용함. FALSE --> 전부 사용안함  */
#define SKY_IS_PICTURE_USE()          sIS_PICTURE_USE( \
                                            g_SysConfig.PictureSetting)
/* 플립이 닫혔을 때, 폴더를 열었을 때 */
#define SKY_GET_IDLE_PICTURE()        sGET_IDLE_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))
/* 전화가 왔을 때 */
#define SKY_GET_INCOMING_PICTURE()    sGET_INCOMING_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))
/* 전원을 켤 때 */
#define SKY_GET_POWER_ON_PICTURE()    sGET_POWER_ON_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))
/* 전원을 끌 때 */
#define SKY_GET_POWER_OFF_PICTURE()   sGET_POWER_OFF_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))
/* WAP 시작할 때 */
#define SKY_GET_WAP_PICTURE()         sGET_WAP_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))

/*---------------------------------------------------------------------------
** 음량
*/
/* 통화음 크기 */
#define SKY_GET_SPEAKER_VOLUME()    sGET_SPEAKER_VOLUME(g_SysConfig.Volume)
/* 키톤 크기 */
#define SKY_GET_KEY_TONE_VOLUME()   sGET_KEY_TONE_VOLUME(g_SysConfig.Volume)
/* 마이크 크기 */
#define SKY_GET_MIC_VOLUME()        sGET_MIC_VOLUME(g_SysConfig.Volume)
/* 게임 음량 */
#define SKY_GET_GAME_VOLUME()        sGET_GAME_VOLUME(g_SysConfig.Volume)

/*---------------------------------------------------------------------------
** 경보음
*/
#define SKY_IS_CONNECT_WARNING_SIGNAL()  sIS_CONNECT_WARNING_SIGNAL( \
                                                g_SysConfig.WarningSignal)
#define SKY_IS_1MINUTE_WARNING_SIGNAL()  sIS_1MINUTE_WARNING_SIGNAL( \
                                                g_SysConfig.WarningSignal)
#define SKY_IS_NOSERVICE_WARNING_SIGNAL() sIS_NOSERVICE_WARNING_SIGNAL( \
                                                g_SysConfig.WarningSignal)
#define SKY_IS_LOBATT_WARNING_SIGNAL()    sIS_LOBATT_WARNING_SIGNAL( \
                                                g_SysConfig.WarningSignal)
#define SKY_IS_MESSAGE_BEEP_SOUND()     sIS_MESSAGE_BEEP_SOUND( \
                                                g_SysConfig.WarningSignal)

/*---------------------------------------------------------------------------
** 데이터 서비스
*/
/* 착신 모드 */
#define SKY_GET_DATA_SERVICE_RECEIPT_MODE()   sGET_DATA_SERVICE_RECEIPT_MODE(g_SysConfig.DataService)
/* 인터넷 속도 */
#define SKY_GET_DATA_SERVICE_INTERNET_SPEED() sGET_DATA_SERVICE_INTERNET_SPEED(g_SysConfig.DataService)
/* PC와 통신 속도 */
#define SKY_GET_DATA_SERVICE_PBS_PC()      sGET_DATA_SERVICE_BPS_PC(g_SysConfig.DataService)
/* AIR 데이터 매니저 */
#define SKY_GET_AIR_DATA_MANAGER_PHONE_NUMBER() \
                                      sGET_AIR_DATA_MANAGER_PHONE_NUMBER(g_SysConfig.DataService)
#define SKY_GET_AIR_DATA_MANAGER_IP_ADDRESS()   \
                                      sGET_AIR_DATA_MANAGER_IP_ADDRESS(g_SysConfig.DataService)

/*---------------------------------------------------------------------------
** 전자메일 계정
*/
#define SKY_IS_EMAIL_ACCOUNT()      sIS_EMAIL_ACCOUNT( \
                                        g_SysConfig.EMailAccount)

#define SKY_GET_EMAIL_USERNAME()    sGET_EMAIL_USERNAME(g_SysConfig.EMailAccount)
#define SKY_GET_EMAIL_USERID()      sGET_EMAIL_USERID(g_SysConfig.EMailAccount)
#define SKY_GET_EMAIL_PASSWORD()    sGET_EMAIL_PASSWORD(g_SysConfig.EMailAccount)
#define SKY_GET_EMAIL_SMTP()        sGET_EMAIL_SMTP(g_SysConfig.EMailAccount)
#define SKY_GET_EMAIL_POP3()        sGET_EMAIL_POP3(g_SysConfig.EMailAccount)
#define SKY_GET_EMAIL_USERINFO()    sGET_EMAIL_USERINFO(g_SysConfig.EMailAccount)
#define SKY_GET_EMAIL_ADDRESS()     sGET_EMAIL_ADDRESS(g_SysConfig.EMailAccount)
#define SKY_GET_EMAIL_SERVER()      sGET_EMAIL_SERVER(g_SysConfig.EMailAccount)

/*---------------------------------------------------------------------------
** 배색
*/
#define SKY_GET_COLOR_SCHEME()      sGET_COLOR_SCHEME(g_SysConfig.ScreenTheme)
/* rosa 2001-09-25 --> @why, what */
//#define SKY_GET_COLOR_SCHEME_BACK() sGET_COLOR_SCHEME_BACK(g_SysConfig.ScreenTheme)
//#define SKY_GET_COLOR_SCHEME_FORE() sGET_COLOR_SCHEME_FORE(g_SysConfig.ScreenTheme)
/* <-- */
#define SKY_GET_FONT_TYPE()         sGET_FONT_TYPE(g_SysConfig.ScreenTheme)
#define SKY_GET_SCROLLBAR_SIZE()    sGET_SCROLLBAR_SIZE(g_SysConfig.ScreenTheme)

/*---------------------------------------------------------------------------
** 버전 정보
*/
#define SKY_GET_DMSS_VERSION_INFO()     sGET_DMSS_VERSION_INFO(g_SysConfig.VersionInfo)
#define SKY_GET_EQUS_VERSION_INFO()     sGET_EQUS_VERSION_INFO(g_SysConfig.VersionInfo)
#define SKY_GET_SKY_VERSION_INFO()      sGET_SKY_VERSION_INFO(g_SysConfig.VersionInfo)

/*---------------------------------------------------------------------------
** 최근 발신 번호
*/
#define SKY_GET_LASTDIAL_INFO_BUFF()      sGET_LASTDIALINFO_BUFF(g_SysConfig.LastDialInfo)
#define SKY_GET_LASTDIAL_INFO_SECRET()    sGET_LASTDIALINFO_SECRET(g_SysConfig.LastDialInfo)
#define SKY_GET_LASTDIAL_INFO_POS()       sGET_LASTDIALINFO_POS(g_SysConfig.LastDialInfo)

/*---------------------------------------------------------------------------
** 카메라 카운트
*/
#define SKY_GET_CAMERA_COUNT()              g_SysConfig.dwCameraCount

/*---------------------------------------------------------------------------
** 포토메일 카운트
*/
#define SKY_GET_PM_COUNT()              g_SysConfig.dwPMCount
/*
**===========================================================================
**  시스템 정보 쓰기 
**===========================================================================
*/

/*---------------------------------------------------------------------------
** 국가 설정 
*/
#define SKY_SET_COUNTRY(c)          sSET_CONFIG_VALUE(SC_SYS, SKY_GET_COUNTRY(), c)

/*---------------------------------------------------------------------------
** 에티켓 
*/
#define SKY_SET_ETIQUETTE(f)        sSET_CONFIG_VALUE(SC_SYS, SKY_IS_ETIQUETTE(), f)

/*---------------------------------------------------------------------------
** 스피커
*/
#define SKY_SET_LOUD_SPEAKER(f)     sSET_CONFIG_VALUE(SC_SYS, SKY_IS_LOUD_SPEAKER(), f)

/*---------------------------------------------------------------------------
** 휴대폰 이름 
*/
#define SKY_SET_BANNER(s)           sSET_CONFIG_STR(SC_SYS, SKY_GET_BANNER(), s)

/*---------------------------------------------------------------------------
** 암호 
*/
#define SKY_SET_PASSWORD(s)         sSET_CONFIG_STR(SC_SYS, SKY_GET_PASSWORD(), s)

/*---------------------------------------------------------------------------
** 자동 응답 
*/
/* 설정 */
#define SKY_SET_AUTO_ANSWER(f)      sSET_CONFIG_VALUE(SC_SYS, SKY_IS_AUTO_ANSWER(), f)
/* 인사말 */
#define SKY_SET_AUTO_ANSWER_GREETING(g)  sSET_CONFIG_VALUE(SC_SYS, SKY_GET_AUTO_ANSWER_GREETING(), g)
/* 벨 울림 시간 */
#define SKY_SET_AUTO_ANSWER_BELL_TIME(t)  sSET_CONFIG_VALUE(SC_SYS, SKY_GET_AUTO_ANSWER_BELL_TIME(), t)
/* 메시지 개수 */
#define SKY_SET_AUTO_ANSWER_COUNT(n)    sSET_CONFIG_VALUE(SC_SYS, SKY_GET_AUTO_ANSWER_COUNT(), n)
/* 벨 회수     */
#define SKY_SET_AUTO_ANSWER_RINGNUM(n)  sSET_CONFIG_VALUE(SC_SYS, SKY_GET_AUTO_ANSWER_RINGNUM(), n)

/*---------------------------------------------------------------------------
** 자동 잠금
*/
/* 자동 잠금 */
#define SKY_SET_AUTO_LOCK(f)          sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_IS_AUTO_LOCK(), f)
/* 국제 전화 잠금 */
#define SKY_SET_OVERSEAS_CALL_LOCK(f) sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_IS_OVERSEAS_CALL_LOCK(), f)
/* 전화부 잠금 */
#define SKY_SET_PHONE_BOOK_LOCK(f)    sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_PHONE_BOOK_LOCK(), f)
/* 메뉴 잠금 */
#define SKY_SET_MENU_LOCK(f)          sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_MENU_LOCK(), f)
/* 착발신 이력 잠금 */
#define SKY_SET_CALL_STACK_LOCK(f)    sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_CALL_STACK_LOCK(), f)
/* 녹음/재생 잠금 */
#define SKY_SET_RECORD_PLAY_LOCK(f)    sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_RECORD_PLAY_LOCK(), f)
/* 메모 잠금 */
#define SKY_SET_MEMO_LOCK(f)           sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_MEMO_LOCK(), f)
/* WAP 잠금 */
#define SKY_SET_WAP_LOCK(f)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_WAP_LOCK(), f)
/* SMS 잠금 */
#define SKY_SET_SMS_LOCK(f)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_SMS_LOCK(), f)
/* CAMERA 잠금 */
#define SKY_SET_CAMERA_LOCK(f)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_CAMERA_LOCK(), f)

//#ifdef FEATURE_SKY_MODEL_TYPE_FOLDER
/*---------------------------------------------------------------------------
** 전화 받기 방법
*/
#define SKY_SET_HOW_TO_ANSWER(h)       sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_HOW_TO_ANSWER(), h)

/*----------------------------------------------------------------------------
** 한글 입력 방안 선택
*/
#define SKY_SET_SEL_HANGUL(h)       sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_SEL_HANGUL(), h)
/*----------------------------------------------------------------------------
** 메뉴 모드 선택
*/
#define SKY_SET_SEL_MENU_MODE(h)       sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_SEL_MENU_MODE(), h)

/*----------------------------------------------------------------------------
** 스마트 배터리
*/
#define SKY_SET_SMART_BATTERY(h)       sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_SMART_BATTERY(), h)

/*----------------------------------------------------------------------------
** 011 부가 서비스
*/
#define SKY_SET_011_SERVICE(h)        sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_011_SERVICE(), h)

//#elif FEATURE_SKY_MODEL_TYPE_FLIP
/*---------------------------------------------------------------------------
** 플립키 설정
*/
#define SKY_SET_FLIP_KEY(k)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_FLIP_KEY(), k)
#define SKY_SET_MEMORY_DIAL_NUMBER(n)  sSET_CONFIG_STR(SC_SYS, \
                                               SKY_GET_MEORY_DIAL_NUMBER, n)
                                               
//#endif /* FEATURE_SKY_MODEL_TYPE_XXX */

/*---------------------------------------------------------------------------
** NAM -- NV에 저장해야 함
*/
#define SKY_SET_CURRENT_NAM(n)         sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_CURRENT_NAM(), n)
/*---------------------------------------------------------------------------
** 화면 밝기 
*/
#define SKY_SET_CONTRAST(n)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_CONTRAST(), n)

/*---------------------------------------------------------------------------
** 화면 밝기 
*/
#define SKY_SET_IDCONTRAST(n)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_IDCONTRAST(), n)
/*---------------------------------------------------------------------------
** 화면 밝기 
*/
#define SKY_SET_SHORTLOCK(n)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_SHORTLOCK(), n)

/*---------------------------------------------------------------------------
** 자기정보 잠금
*/
#define SKY_SET_SELFINFOLOCK(n)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_SELFINFOLOCK(), n)
/*---------------------------------------------------------------------------
** SKVM 설정 
*/
/* SKVM 칼라 설정 */
#define SKY_SET_SKVMCOLORENV(n)            sSET_CONFIG_VALUE(SC_SYS, \
	                                                  SKY_GET_SKVMCOLORENV(), n)
/* SKVM 배경 화면 설정 여부 */
#define SKY_SET_JAVAASBG(n)		           sSET_CONFIG_VALUE(SC_SYS, \
	                                                  SKY_GET_JAVAASBG(), n)
/* SKVM argc */
#define SKY_SET_ARGCINSKVM(n)	           sSET_CONFIG_VALUE(SC_SYS, \
	                                                  SKY_GET_ARGCINSKVM(), n)
/* SKVM argv */
#define SKY_SET_ARGVINSKVM(s,l)			   sSET_CONFIG_COPY(SC_SYS, \
	                                                  SKY_GET_ARGVINSKVM(), s ,l)

/*---------------------------------------------------------------------------
** WAP 설정
*/
/* 프록시 주소 */
#define SKY_SET_WAP_PROXY_URL(s)       sSET_CONFIG_STR(SC_SYS, \
                                                    SKY_GET_WAP_PROXY_URL(), s)
/* 초기 URL */
#define SKY_SET_WAP_INIT_URL(s)        sSET_CONFIG_STR(SC_SYS, \
                                                    SKY_GET_WAP_INIT_URL(), s)
/* 초기 WAP mail Server주소 */
#define SKY_SET_WAP_INIT_MAIL(s)       sSET_CONFIG_STR(SC_SYS, \
                                                    SKY_GET_WAP_INIT_MAIL(), s)
/* WAP Dormant Time */
#define SKY_SET_WAP_DORMANT_TIME(n)    sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_WAP_DORMANT_TIME(), n)


/*---------------------------------------------------------------------------
** 벨/진동
*/
/* 벨 아이디 */
#define SKY_SET_BELL_ID(n)         sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_BELL_ID(), n)
/* 벨 파일 이름 */
#define SKY_SET_BELL_FILE_NAME(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_BELL_FILE_NAME(), s)
/* 벨/진동 설정 */
#define SKY_SET_BELL_VIBRATION(b)       sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_BELL_VIBRATION(), b)
/* 벨 음량 */
#define SKY_SET_BELL_VOLUME(n)          sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_BELL_VOLUME(), n)
/* 벨 울림 시간 */
#define SKY_SET_BELL_TIME(t)            sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_BELL_TIME(), t)

/*---------------------------------------------------------------------------
** 그림 설정
*/
/* 대기 화면 종류 */
#define SKY_SET_IDLE_DISPLAY_NAME(f)   sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_IDLE_DISPLAY_NAME(), f)

#define SKY_SET_IDLE_SCREEN(n)         sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_IDLE_SCREEN(), n)
/* 대기 화면 표시 방법 */
#define SKY_SET_IDLE_DISPLAY_WATCH(i)    sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_IDLE_DISPLAY_WATCH(), i)
/* 그림 사용함. FALSE --> 전부 사용안함  */
#define SKY_SET_PICTURE_USE(f)         sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_IS_PICTURE_USE(), f)
/* 플립이 닫혔을 때, 폴더를 열었을 때 */
#define SKY_SET_IDLE_PICTURE(p)        sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_IDLE_PICTURE(), p)
/* 전화가 왔을 때 */
#define SKY_SET_INCOMING_PICTURE(p)    sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_INCOMING_PICTURE(), p)
/* 전원을 켤 때 */
#define SKY_SET_POWER_ON_PICTURE(p)    sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_POWER_ON_PICTURE(), p)
/* 전원을 끌 때 */
#define SKY_SET_POWER_OFF_PICTURE(p)   sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_POWER_OFF_PICTURE(), p)
/* WAP 시작할 때 */
#define SKY_SET_WAP_PICTURE(p)         sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_WAP_PICTURE(), p)

/*---------------------------------------------------------------------------
** 음량
*/
/* 통화음 크기 */
#define SKY_SET_SPEAKER_VOLUME(n)      sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_SPEAKER_VOLUME(), n)
/* 키톤 크기 */
#define SKY_SET_KEY_TONE_VOLUME(n)     sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_KEY_TONE_VOLUME(), n)
/* 마이크 크기 */
#define SKY_SET_MIC_VOLUME(m)          sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_MIC_VOLUME(), m)
/* 게임 음량 */
#define SKY_SET_GAME_VOLUME(m)          sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_GAME_VOLUME(), m)

/*---------------------------------------------------------------------------
** 경보음
*/
#define SKY_SET_CONNECT_WARNING_SIGNAL(w)  sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_IS_CONNECT_WARNING_SIGNAL(), w)
#define SKY_SET_1MINUTE_WARNING_SIGNAL(w)  sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_IS_1MINUTE_WARNING_SIGNAL(), w)
#define SKY_SET_NOSERVICE_WARNING_SIGNAL(w)  sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_IS_NOSERVICE_WARNING_SIGNAL(), w)
#define SKY_SET_LOBATT_WARNING_SIGNAL(w)   sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_IS_LOBATT_WARNING_SIGNAL(), w)
#define SKY_SET_MESSAGE_BEEP_SOUND(w)   sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_IS_MESSAGE_BEEP_SOUND(), w)

/*---------------------------------------------------------------------------
** 데이터 서비스
*/
/* 착신 모드 */
#define SKY_SET_DATA_SERVICE_RECEIPT_MODE(r)  \
                                    sSET_CONFIG_VALUE(SC_SYS, \
                                        SKY_GET_DATA_SERVICE_RECEIPT_MODE(), r)
/* 인터넷 속도 */
#define SKY_SET_DATA_SERVICE_INTERNET_SPEED(i)  \
                                    sSET_CONFIG_VALUE(SC_SYS, \
                                      SKY_GET_DATA_SERVICE_INTERNET_SPEED(), i)
/* PC와 통신 속도 */
#define SKY_SET_DATA_SERVICE_PBS_PC(b)      sSET_CONFIG_VALUE(SC_SYS, \
                                              SKY_GET_DATA_SERVICE_PBS_PC(), b)
/* AIR 데이터 매니저 */
#define SKY_SET_AIR_DATA_MANAGER_PHONE_NUMBER(s)  \
                                 sSET_CONFIG_STR(SC_SYS, \
                                    SKY_GET_AIR_DATA_MANAGER_PHONE_NUMBER(), s)
#define SKY_SET_AIR_DATA_MANAGER_IP_ADDRESS(s)   \
                                 sSET_CONFIG_STR(SC_SYS, \
                                      SKY_GET_AIR_DATA_MANAGER_IP_ADDRESS(), s)

/*---------------------------------------------------------------------------
** 전자메일 계정
*/

#define SKY_SET_EMAIL_ACCOUNT(w)    sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_IS_EMAIL_ACCOUNT(), w)

#define SKY_SET_EMAIL_USERNAME(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EMAIL_USERNAME(), s)
#define SKY_SET_EMAIL_USERID(s)         sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EMAIL_USERID(), s)
#define SKY_SET_EMAIL_PASSWORD(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EMAIL_PASSWORD(), s)
#define SKY_SET_EMAIL_SMTP(s)           sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EMAIL_SMTP(), s)
#define SKY_SET_EMAIL_POP3(s)           sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EMAIL_POP3(), s)
#define SKY_SET_EMAIL_USERINFO(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EMAIL_USERINFO(), s)
#define SKY_SET_EMAIL_ADDRESS(s)        sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EMAIL_ADDRESS(), s)
#define SKY_SET_EMAIL_SERVER(s)         sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EMAIL_SERVER(), s)
/*---------------------------------------------------------------------------
** 배색
*/
#define SKY_SET_COLOR_SCHEME(s)         sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_COLOR_SCHEME(), s)
/* rosa 2001-09-25 --> @why, what */
#define SKY_SET_COLOR_SCHEME_BACK(s)    sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_COLOR_SCHEME_BACK(), s)
#define SKY_SET_COLOR_SCHEME_FORE(s)    sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_COLOR_SCHEME_FORE(), s)
/* <-- */
#define SKY_SET_FONT_TYPE(s)            sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_FONT_TYPE(), s)
#define SKY_SET_SCROLLBAR_SIZE(s)       sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_SCROLLBAR_SIZE(), s)


/*---------------------------------------------------------------------------
** 버전 정보
*/
#define SKY_SET_DMSS_VERSION_INFO(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_DMSS_VERSION_INFO(), s)
#define SKY_SET_EQUS_VERSION_INFO(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EQUS_VERSION_INFO(), s)
#define SKY_SET_SKY_VERSION_INFO(s)        sSET_CONFIG_STR(SC_SYS, \
                                                 SKY_GET_SKY_VERSION_INFO(), s)


/*---------------------------------------------------------------------------
** 최근 발신 정보
*/
#define SKY_SET_LASTDIALINFO_BUFF(t, l)       sSET_CONFIG_COPY(SC_SYS, \
                                                SKY_GET_LASTDIAL_INFO_BUFF(), t, l)


#define SKY_SET_LASTDIALINFO_SECRET(s)     sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_LASTDIAL_INFO_SECRET(), s)

#define SKY_SET_LASTDIALINFO_POS(s)        sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_LASTDIAL_INFO_POS(), s)
/*---------------------------------------------------------------------------
** 카메라 카운트
*/
#define SKY_SET_CAMERA_COUNT(n)              sSET_CONFIG_VALUE(SC_SYS, \
                                    SKY_GET_CAMERA_COUNT(), n)
#define SKY_ADD_CAMERA_COUNT()              sSET_CONFIG_VALUE(SC_SYS, \
                                    SKY_GET_CAMERA_COUNT(), SKY_GET_CAMERA_COUNT()+1)

/*---------------------------------------------------------------------------
** 포토메일 카운트
*/
#define SKY_SET_PM_COUNT()              sSET_CONFIG_VALUE(SC_SYS, \
                                    SKY_GET_PM_COUNT(), SKY_GET_PM_COUNT()+1)

#define SKY_INIT_PM_COUNT(n)             sSET_CONFIG_VALUE(SC_SYS, \
                                    SKY_GET_PM_COUNT(), n)


/* rosa 01-01-26 --> @why, what */

/*===========================================================================
**  응용프로그램 정보 - 알람
*/
/* 알람 설정 */
struct tagALARM_BELL_T
{
    BELL_TYPE_T AlarmType;  // Alarm
    BELL_TYPE_T AlertType;  // Scheduler
    INT8        nVolume;
};
typedef struct tagALARM_BELL_T      ALARM_BELL_T;

#define sGET_ALARM_BELL_ID(s)               sGET_BELL_ID((s).AlarmType)
#define sGET_ALARM_BELL_FILE_NAME(s)        sGET_BELL_FILE_NAME((s).AlarmType)

#define sGET_SCHL_ALERT_BELL_ID(s)          sGET_BELL_ID((s).AlertType)
#define sGET_SCHL_ALERT_BELL_FILE_NAME(s)   sGET_BELL_FILE_NAME((s).AlertType)

#define sGET_ALARM_BELL_VOLUME(s)           (s).nVolume

enum tagALARM_ACTIVE_E
{
    AAT_NO_HOLIDAY,
    AAT_YES_HOLIDAY
};
typedef enum tagALARM_ACTIVE_E      ALARM_ACTIVE_E;

/*===========================================================================
**  응용프로그램 정보 - 알람
**  알람은 최대 10개까지 지원.
*/
/* 알람 시간/알람 벨 종류/알람 벨 크기 */

/* 알람 시간 */
enum tagALARM_TIME_TYPE_E
{
    ATT_NONE,                        /* 초기값 */
	ATT_AFTER_10M,                   /* 10분후 */
	ATT_AFTER_20M,                   /* 20분후 */
	ATT_AFTER_30M,                   /* 30분후 */
	ATT_AFTER_1H,                    /* 1시간후 */
	ATT_SET_ANYTIME,                 /* 시간지정 */
	ATT_EVERY_DAY,                   /* 매일 */
    ATT_EVERY_WEEK,                  /* 매주 */
    ATT_EVERY_MONTH                  /* 매달 */
};
typedef enum tagALARM_TIME_TYPE_E        ALARM_TIME_TYPE_E;

#define MAX_ALARM_TITLE             18
#define MAX_ALARM_COUNT             10

struct tagALARM_TIME_T
{
    ALARM_TIME_TYPE_E   Type;  /* 알람 시간 종류 */
    DWORD               dwTime;         /* 알람 시간 설정 */
	BOOL                fActive;        // is valid 
	BOOL                fUsed;         // is Used
    BYTE                szTitle[MAX_ALARM_TITLE+1];
};
typedef struct tagALARM_TIME_T      ALARM_TIME_T;

#define sGET_ALARM_TIME_TYPE(s)     (s).Type
#define sGET_ALARM_TIME_ACTIVE(s)   (s).fActive
#define sGET_ALARM_TIME_USED(s)     (s).fUsed
#define sGET_ALARM_TIME(s)          (s).dwTime
#define sGET_ALARM_TITLE(s)         (s).szTitle





enum tagMEMO_TYPE_E
{
    MT_NONE,                        /* 초기값 */
    MT_MEMO
};
typedef enum tagMEMO_TYPE_E        MEMO_TYPE_E;

#define MAX_MEMO_DATA_LENGTH       (16*4)
#define MAX_MEMO_COUNT             10

struct tagMEMO_T
{
    MEMO_TYPE_E         Type;       /* 메모 종류 */
    DWORD               dwTime;     /* 메모 작성 시간 */
    BYTE                szData[MAX_MEMO_DATA_LENGTH+1];
};
typedef struct tagMEMO_T            MEMO_T;

#define sGET_MEMO_TYPE(s)           (s).Type
#define sGET_MEMO_TIME(s)           (s).dwTime
#define sGET_MEMO_DATA(s)           (s).szData

/* <-- */



/*===========================================================================
**  응용프로그램 정보 - 메시지 도착 알림
*/
/* 알람 설정 */
enum tagSMS_ALERT_TYPE_E
{
    SAT_NO,              /* 소리 안남 */
    SAT_ONCE,            /* 1회 울림 */
    SAT_EVERY_2MINIUTE   /* 2분마다 울림 */
};
typedef enum tagSMS_ALERT_TYPE_E         SMS_ALERT_TYPE_E;

/* 알람 종류 - 벨1~7 */

struct tagSMS_ALERT_T
{
    SMS_ALERT_TYPE_E    AlertType;      /* 알람 설정 */
    WORD                BellID;         /* 알람 벨 */
    INT8                nVolume;        /* 알람 크기 */
};
typedef struct tagSMS_ALERT_T     SMS_ALERT_T;

#define sGET_SMS_ALERT_TYPE(s)      (s).AlertType
#define sGET_SMS_BELL_ID(s)         (s).BellID
#define sGET_SMS_BELL_VOLUME(s)     (s).nVolume

/*---------------------------------------------------------------------------
**  통화 시간
*/
struct tagUSING_TIME_T
{
    DWORD   dwLast;         /* 최종 사용 시간 */
    DWORD   dwCumulation;   /* 누적 사용 시간 */
    DWORD   dwTotal;        /* 총 통화 시간 */
};
typedef struct tagUSING_TIME_T          USING_TIME_T;

#define sGET_LAST_USING_TIME(s)         (s).dwLast
#define sGET_CUMULATION_USING_TIME(s)   (s).dwCumulation
#define sGET_TOTAL_USING_TIME(s)        (s).dwTotal

/*===========================================================================
**  통화 정보
*/
struct tagCALL_TIME_T
{
    USING_TIME_T        VoiceUsingTime;          /*  음성발신시간   */
	USING_TIME_T        VoiceReceivedCallTime;   /*  음성착신시간   */
	USING_TIME_T        DataUsingTime;           /*  데이터발신시간 */
	USING_TIME_T        DataReceivedCallTime;    /*  데이터착신시간 */
    USING_TIME_T        WAPUsingTime;            /*  WAP 사용 시간  */
};
typedef struct tagCALL_TIME_T   CALL_TIME_T;

/* Originated Voice Call Time */
#define sGET_VOICE_LAST_USING_TIME(s)       \
                                sGET_LAST_USING_TIME((s).VoiceUsingTime)
#define sGET_VOICE_CUMULATION_USING_TIME(s) \
                                sGET_CUMULATION_USING_TIME((s).VoiceUsingTime)
#define sGET_VOICE_TOTAL_USING_TIME(s)      \
                                sGET_TOTAL_USING_TIME((s).VoiceUsingTime)

/* Received Voice Call Time */
#define sGET_VOICE_RECEIVEDCALL_LAST_USING_TIME(s)   \
	                             sGET_LAST_USING_TIME((s).VoiceReceivedCallTime)
#define sGET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(s)          \
	                             sGET_CUMULATION_USING_TIME((s).VoiceReceivedCallTime)
#define sGET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(s) \
                                 sGET_TOTAL_USING_TIME((s).VoiceReceivedCallTime)       

/* Originated Data Call Time */
#define sGET_DATA_ORIGINATEDCALL_LAST_USING_TIME(s)  \
	                             sGET_LAST_USING_TIME((s).DataUsingTime)
#define sGET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(s) \
	                             sGET_CUMULATION_USING_TIME((s).DataUsingTime)
#define sGET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME(s) \
	                             sGET_TOTAL_USING_TIME((s).DataUsingTime)

/* Received Data Call Time  */
#define sGET_DATA_RECEIVEDCALL_LAST_USING_TIME(s) \
	                             sGET_LAST_USING_TIME((s).DataReceivedCallTime)
#define sGET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(s) \
                                 sGET_CUMULATION_USING_TIME((s).DataReceivedCallTime)
#define sGET_DATA_RECEIVEDCALL_TOTAL_USING_TIME(s) \
		                         sGET_TOTAL_USING_TIME((s).DataReceivedCallTime)


#define sGET_WAP_LAST_USING_TIME(s)       \
                                sGET_LAST_USING_TIME((s).WAPUsingTime)
#define sGET_WAP_CUMULATION_USING_TIME(s) \
                                sGET_CUMULATION_USING_TIME((s).WAPUsingTime)
#define sGET_WAP_TOTAL_USING_TIME(s)      \
                                sGET_TOTAL_USING_TIME((s).WAPUsingTime)

/*---------------------------------------------------------------------------
** 세계시각에서 서머타임(DST) 정보
*/
struct tagDST_TIME_T
{
    BOOL    fOn;
    UINT8   nStartMon;
    UINT8   nStartDay;
    UINT8   nEndMon;
    UINT8   nEndDay;
};
typedef struct tagDST_TIME_T   DST_TIME_T;

#define NUM_COUNTRY_NAME            41
#define DST_DATA_TABLE_MAX          5

#define sGET_DST_ON(s)          (s).fOn
#define sGET_DST_START_MON(s)   (s).nStartMon
#define sGET_DST_START_DAY(s)   (s).nStartDay
#define sGET_DST_END_MON(s)     (s).nEndMon
#define sGET_DST_END_DAY(s)     (s).nEndDay

/*---------------------------------------------------------------------------
** 위치 추적 정보
*/
/* reo 2001.03.16 {-- */
#ifdef FEATURE_SKTT_EQUS_LOCATION
struct tagLOCATION_INFO_T{
	WORD	lastsid;
	WORD	lastnid;
	WORD	lastzone;
	WORD	lastbaseid;
	WORD	lastpn;
	DWORD	lastbaselat;
	DWORD	lastbaselong;
	BYTE	rcvinfo;							// 위치통보 정보 수신 여부 결정
	BYTE	cfrmid[5];							// 위치확인자 ID
	BYTE	onoffstate;
	BYTE	starttime;
	BYTE	endtime;
	BYTE	startmin;
	BYTE	endmin;
	BYTE	cycle;
	BYTE	lam3ver;
	BYTE	reportwait;
};
typedef struct tagLOCATION_INFO_T	  LOCATION_INFO_T;
#endif //FEATURE_SKTT_EQUS_LOCATION
/* reo 2001.03.16 --} */


#define SKY_MAX_FL_FILE_NUM  	 21          /* 녹음/재생을 위한 정의  */
struct tagVMEMO_FN_SEQUENCE_T{
	BYTE       address;
	WORD       Time;
	BYTE       Fn;
	BYTE       Type;
};
typedef struct tagVMEMO_FN_SEQUENCE_T  FN_SEQUENCE_T;



typedef struct tagVMEMOINFO_T            VMEMOINFO_T;   
struct tagVMEMOINFO_T
{                       
    DWORD  dwRemainedByte;
    WORD   wUsedTime;         /* uservoice_l */
    WORD   wVoiceMemoReocrdingTime;
    WORD   wRemainedTime;
    BYTE   bTotalRecNumber;
    BYTE   bNewAutoAnsNumber;
    BOOL   fUserVoice;
    BYTE   bUserVoiceFn;
};


/*===========================================================================
**  응용프로그램 정보 - CONFIG_T 로 끝나는 구조체는 file system에 저장.
**  --> CALL/WAP 애플릿에서 관리하고, 메뉴에서 값을 요청하면 값을 전달하면 됨.
*/
struct tagAPP_CONFIG_T
{
    SKY_FILE_HEADER_T   Header;             /* 파일 헤더 */
/* rosa 00-12-14 --> @why, what */
    ALARM_BELL_T        AlarmBell;          /* 스케줄러, 알람 */
    BOOL                AlarmHolidayActive[MAX_ALARM_COUNT];
    ALARM_TIME_T        AlarmTime[MAX_ALARM_COUNT]; /* 알람 시간 설정 */
    MEMO_T              Memo[MAX_MEMO_COUNT];
/* <-- */
    SMS_ALERT_T         SMSAlert;           /* SMS 알람 설정, 알람 벨, 알람 크기 */
    CALL_TIME_T         CallTime;           /* 음성, WAP 통화 시간 */
    DST_TIME_T          DSTWorldTime[NUM_COUNTRY_NAME];    
                                            /* 세계시각의 서머타임 적용 시간 */
#ifdef FEATURE_SKTT_EQUS_LOCATION
	LOCATION_INFO_T		LocationInfo;
#endif
	FN_SEQUENCE_T       FnSequence[SKY_MAX_FL_FILE_NUM]; /* 녹음/재생 */
    VMEMOINFO_T         VoiceInfo;    
};
typedef struct tagAPP_CONFIG_T     APP_CONFIG_T;

/*
**===========================================================================
**  응용프로그램 정보 읽기
**===========================================================================
*/

/* rosa 01-01-26 --> @why, what */

/* 알람 벨 설정 */
// Alarm
#define SKY_GET_ALARM_BELL_ID()   \
                        sGET_ALARM_BELL_ID(g_AppConfig.AlarmBell)
#define SKY_GET_ALARM_BELL_FILE_NAME() \
                        sGET_ALARM_BELL_FILE_NAME(g_AppConfig.AlarmBell)
// Scheduler
#define SKY_GET_SCHL_ALERT_BELL_ID()   \
                        sGET_SCHL_ALERT_BELL_ID(g_AppConfig.AlarmBell)
#define SKY_GET_SCHL_ALERT_BELL_FILE_NAME() \
                        sGET_SCHL_ALERT_BELL_FILE_NAME(g_AppConfig.AlarmBell)
// bell size
#define SKY_GET_ALARM_BELL_VOLUME() sGET_ALARM_BELL_VOLUME(g_AppConfig.AlarmBell)

/* 알람 설정 */
#define SKY_IS_ALARM_HOLIDAY_ACTIVE(i)     g_AppConfig.AlarmHolidayActive[i]
/* 알람 시간 설정 */
#define SKY_GET_ALARM_TIME_TYPE(i)  sGET_ALARM_TIME_TYPE(g_AppConfig.AlarmTime[i])
#define SKY_IS_ALARM_TIME_ACTIVE(i) sGET_ALARM_TIME_ACTIVE(g_AppConfig.AlarmTime[i])
#define SKY_IS_ALARM_TIME_USED(i)   sGET_ALARM_TIME_USED(g_AppConfig.AlarmTime[i])
#define SKY_GET_ALARM_TIME(i)       sGET_ALARM_TIME(g_AppConfig.AlarmTime[i])
#define SKY_GET_ALARM_TITLE(i)      sGET_ALARM_TITLE(g_AppConfig.AlarmTime[i])

/* 메모 설정 */
#define SKY_GET_MEMO_TYPE(i)        sGET_MEMO_TYPE(g_AppConfig.Memo[i])
#define SKY_GET_MEMO_TIME(i)        sGET_MEMO_TIME(g_AppConfig.Memo[i])
#define SKY_GET_MEMO_DATA(i)        sGET_MEMO_DATA(g_AppConfig.Memo[i])

/* <-- */

/* SMS 알람 설정 */
#define SKY_GET_SMS_ALERT_TYPE()           \
                                sGET_SMS_ALERT_TYPE(g_AppConfig.SMSAlert)
#define SKY_GET_SMS_BELL_ID()      \
                                sGET_SMS_BELL_ID(g_AppConfig.SMSAlert)

#define SKY_GET_SMS_BELL_VOLUME()    \
                                sGET_SMS_BELL_VOLUME(g_AppConfig.SMSAlert)
/* 음성통화 발신시간 */
#define SKY_GET_VOICE_LAST_USING_TIME()  \
                        sGET_VOICE_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_VOICE_CUMULATION_USING_TIME() \
                        sGET_VOICE_CUMULATION_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_VOICE_TOTAL_USING_TIME()  \
                        sGET_VOICE_TOTAL_USING_TIME(g_AppConfig.CallTime)

/* 음성통화 착신시간 */
#define SKY_GET_VOICE_RECEIVEDCALL_LAST_USING_TIME() \
            sGET_VOICE_RECEIVEDCALL_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME() \
	        sGET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME() \
	        sGET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(g_AppConfig.CallTime)

/* 데이터통신 발신시간 */
#define SKY_GET_DATA_ORIGINATEDCALL_LAST_USING_TIME() \
	                    sGET_DATA_ORIGINATEDCALL_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME() \
                        sGET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(g_AppConfig.CallTime)	                   
#define SKY_GET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME() \
                        sGET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME(g_AppConfig.CallTime)	   

/* 데이터통신 착신시간 */
#define SKY_GET_DATA_RECEIVEDCALL_LAST_USING_TIME() \
                        sGET_DATA_RECEIVEDCALL_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME() \
                        sGET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_DATA_RECEIVEDCALL_TOTAL_USING_TIME() \
                        sGET_DATA_RECEIVEDCALL_TOTAL_USING_TIME(g_AppConfig.CallTime)
        
/* WAP 통화시간 */
#define SKY_GET_WAP_LAST_USING_TIME() \
                        sGET_WAP_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_WAP_CUMULATION_USING_TIME() \
                        sGET_WAP_CUMULATION_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_WAP_TOTAL_USING_TIME() \
                        sGET_WAP_TOTAL_USING_TIME(g_AppConfig.CallTime)

/* 세계시각 서머타임 DST 설정 */
#define SKY_GET_DST_ON(i)           sGET_DST_ON(g_AppConfig.DSTWorldTime[i])  
#define SKY_GET_DST_START_MON(i)    sGET_DST_START_MON(g_AppConfig.DSTWorldTime[i])     
#define SKY_GET_DST_START_DAY(i)    sGET_DST_START_DAY(g_AppConfig.DSTWorldTime[i])       
#define SKY_GET_DST_END_MON(i)      sGET_DST_END_MON(g_AppConfig.DSTWorldTime[i])       
#define SKY_GET_DST_END_DAY(i)      sGET_DST_END_DAY(g_AppConfig.DSTWorldTime[i])       

/* 위치 추적 정보 */
#ifdef FEATURE_SKTT_EQUS_LOCATION
#define SKY_GET_LOCATION_INFO()		g_AppConfig.LocationInfo
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

/* 녹음/재생 */
#define SKY_GET_FN_SEQUENCE(i)      g_AppConfig.FnSequence[i]
#define SKY_GET_VOICE_INFO()        (&((g_AppConfig).VoiceInfo))



/*
**===========================================================================
**  응용프로그램 정보 기록
**===========================================================================
*/
/* rosa 01-01-26 --> @why, what */

/* 알람 벨 설정 */
// Alarm
#define SKY_SET_ALARM_BELL_ID(b)   \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_ALARM_BELL_ID(), b)
#define SKY_SET_ALARM_BELL_TYPE_FILE_NAME(s)   \
            sSET_CONFIG_STR(SC_APP, SKY_GET_ALARM_BELL_FILE_NAME(), s)
// Scheduler
#define SKY_SET_SCHL_ALERT_BELL_ID(b)   \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_SCHL_ALERT_BELL_ID(), b)
#define SKY_SET_SCHL_ALERT_BELL_TYPE_FILE_NAME(s)   \
            sSET_CONFIG_STR(SC_APP, SKY_GET_SCHL_ALERT_BELL_FILE_NAME(), s)
// bell size
#define SKY_SET_ALARM_BELL_VOLUME(v) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_ALARM_BELL_VOLUME(), v)

/* 알람 설정 */
#define SKY_SET_ALARM_HOLIDAY_ACTIVE(i,f) sSET_MEMORY_VALUE(SC_APP,SKY_IS_ALARM_HOLIDAY_ACTIVE(i), f)

/* 알람 시간 설정 */
#define SKY_SET_ALARM_TIME_TYPE(i, t)   sSET_MEMORY_VALUE(SC_APP, SKY_GET_ALARM_TIME_TYPE(i), t)
#define SKY_SET_ALARM_TIME_ACTIVE(i, f) sSET_MEMORY_VALUE(SC_APP, SKY_IS_ALARM_TIME_ACTIVE(i), f)
#define SKY_SET_ALARM_TIME_USED(i, f)   sSET_MEMORY_VALUE(SC_APP, SKY_IS_ALARM_TIME_USED(i), f)
#define SKY_SET_ALARM_TIME(i,t)         sSET_MEMORY_VALUE(SC_APP, SKY_GET_ALARM_TIME(i), t)
#define SKY_SET_ALARM_TITLE(i, sz)      sSET_MEMORY_STR(SC_APP, SKY_GET_ALARM_TITLE(i), sz)

/* 메모 설정 */
#define SKY_SET_MEMO_TYPE(i, t)         sSET_CONFIG_VALUE(SC_APP, SKY_GET_MEMO_TYPE(i), t)
#define SKY_SET_MEMO_TIME(i,t)          sSET_CONFIG_VALUE(SC_APP, SKY_GET_MEMO_TIME(i), t)
#define SKY_SET_MEMO_DATA(i, sz)        sSET_CONFIG_STR(SC_APP, SKY_GET_MEMO_DATA(i), sz)

/* <-- */

/* SMS 알람 설정 */
#define SKY_SET_SMS_ALERT_TYPE(a)           \
                    sSET_CONFIG_VALUE(SC_APP, SKY_GET_SMS_ALERT_TYPE(), a) 
#define SKY_SET_SMS_ALERT_BELL_ID(b)      \
                    sSET_CONFIG_VALUE(SC_APP, SKY_GET_SMS_BELL_ID(), b) 
#define SKY_SET_SMS_ALERT_BELL_VOLUME(v)    \
                    sSET_CONFIG_VALUE(SC_APP, SKY_GET_SMS_BELL_VOLUME(), v) 

/* 음성 통화 시간(발신) */
#define SKY_SET_VOICE_LAST_USING_TIME(t)  \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_LAST_USING_TIME(), t)
#define SKY_SET_VOICE_CUMULATION_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_CUMULATION_USING_TIME(), t)
#define SKY_SET_VOICE_TOTAL_USING_TIME(t)  \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_TOTAL_USING_TIME(), t)


/* 음성 통화 시간(착신) */
#define SKY_SET_VOICE_RECEIVEDCALL_LAST_USING_TIME(t) \
	        sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_RECEIVEDCALL_LAST_USING_TIME(), t)
#define SKY_SET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(t) \
	        sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(), t)
#define SKY_SET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(t) \
	        sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(), t)


/* 데이터 통신 시간(발신) */
#define SKY_SET_DATA_ORIGINATEDCALL_LAST_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_ORIGINATEDCALL_LAST_USING_TIME(), t)	 
#define SKY_SET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(), t)
#define SKY_SET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME() , t)


/* 데이터 통신 시간(착신) */
#define SKY_SET_DATA_RECEIVEDCALL_LAST_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_RECEIVEDCALL_LAST_USING_TIME(), t)	 
#define SKY_SET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(), t)
#define SKY_SET_DATA_RECEIVEDCALL_TOTAL_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_RECEIVEDCALL_TOTAL_USING_TIME() , t)	        


/* WAP 통화 시간 */
#define SKY_SET_WAP_LAST_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_WAP_LAST_USING_TIME(), t)
#define SKY_SET_WAP_CUMULATION_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_WAP_CUMULATION_USING_TIME(), t)
#define SKY_SET_WAP_TOTAL_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_WAP_TOTAL_USING_TIME(), t)

/* 세계시각 서머타임 DST 설정 */
#define SKY_SET_DST_ON(i, t)   sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_ON(i), t) 
#define SKY_SET_DST_START_MON(i, t)    \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_START_MON(i), t) 
#define SKY_SET_DST_START_DAY(i, t)    \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_START_DAY(i), t) 
#define SKY_SET_DST_END_MON(i, t)    \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_END_MON(i), t) 
#define SKY_SET_DST_END_DAY(i, t)    \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_END_DAY(i), t) 

/* 위치 추적 정보 */
#ifdef FEATURE_SKTT_EQUS_LOCATION
#define SKY_SET_LOCATION_INFO(i)	\
				sSET_CONFIG_VALUE(SC_APP, SKY_GET_LOCATION_INFO(), i) 
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

#define SKY_SET_FN_SEQUENCE(i, t)   \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_FN_SEQUENCE(i), t)

#define SKY_SET_FN_SEQUENCE_ALL(a)   \
                { \
                    int i; \
                    for(i = 0; i < NV_MAX_FL_FILE_NUM; i++) \
                    { \
                        SKY_GET_FN_SEQUENCE(i) = a[i]; \
                    } \
					sSET_CONFIG_DIRTY(SC_APP); \
                    SKY_WRITE_CONFIG(SC_APP); \
                }

#define SKY_SET_VOICE_INFO(t, l) \
             sSET_CONFIG_COPY(SC_APP, SKY_GET_VOICE_INFO(), t, l)
             //sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_INFO(), t)

/*****************************************************************************
	Mobile Wallet 정보 설정 
******************************************************************************/
#ifdef FEATURE_MCOMMERCE
/*===========================================================================
**  응용프로그램 정보 - Mobile Wallet
*/
#define MW_TEMPCERT_NUM							  2
#define MW_MYCERT_NUM								5
#define MW_TCACERT_NUM							  10

#define MW_TEMPCERT_LENGTH						2
#define	MW_TEMPCERT_PIN_HASH				 20
#define	MW_TEMPCERT_PRV_KEY					  96
#define	MW_TEMPCERT_SKID						 20
#define MW_TEMPCERT_REQUESTDATE			   4
#define	MW_TEMPCERT_SUBJECT_DN			  400

#define MW_MYCERT_LENGTH						  2
#define MW_MYCERT_CHECKFIELD				   2
#define	MW_MYCERT_PIN_HASH					   20
#define	MW_MYCERT_PRV_KEY						96
#define MW_MYCERT_DISPLAY						256
#define	MW_MYCERT_SUBJECT_KID				257
#define	MW_MYCERT_ISSUER_KID				 257
#define MW_MYCERT_CERT_URL					 1200
#define	MW_MYCERT_SUBJECT_DN				400

#define MW_TCACERT_LENGTH						 2
#define MW_TCACERT_CHECKFIELD				  2
#define MW_TCACERT_DISPLAY						256
#define MW_TCACERT_SUBJECT_KID				257
#define MW_TCACERT_ISSUER_KID				 257
#define MW_TCACERT_CERT							1400
#define MW_TCACERT_CA_INFO_URL				128
#define MW_TCACERT_SUBJECT_DN				400
#define MW_TCACERT_NOT_AFTER				4

#define	MW_WALLET_HASHEDPIN_LENGTH			20
#define	MW_WALLET_SECRET_KEY_LENGTH			32
#define	MW_WALLET_USER_LENGTH				    57 //54
#define	MW_WALLET_CARD_LENGTH				  51 //47
#define	MW_WALLET_OCB_LENGTH				   32
#define	MW_WALLET_DELIVERY_LENGTH			120 //137  //배송지 제목 사라짐. 
#define	MW_WALLET_KEYIV_LENGTH				  16				
#define	MW_WALLET_DPERIV_LENGTH				16
#define	MW_WALLET_DCARDIV_LENGTH			16
#define	MW_WALLET_DOCBIV_LENGTH				16
#define MW_WALLET_DDELIV_LENGTH				16

/* ==   TempCert 정보 ======================================================*/
struct tagMW_TempCertInfo_T
{
	WORD Length;
	//BYTE Length[MW_TEMPCERT_LENGTH];
	BYTE PIN_Hash[MW_TEMPCERT_PIN_HASH];
	BYTE PRV_KEY[MW_TEMPCERT_PRV_KEY];
	BYTE SKID[MW_TEMPCERT_SKID];
	DWORD RequestDate;
    BYTE SUBJECT_DN[MW_TEMPCERT_SUBJECT_DN];
	BOOL KEY_USAGE;
};
typedef struct tagMW_TempCertInfo_T	MW_TEMPCERT_INFO_T;

struct tagMW_TEMPCERT_CONFIG_T 
{
    SKY_FILE_HEADER_T			Header;			/* 파일 헤더 */
	MW_TEMPCERT_INFO_T		 MWTempCert;
};
typedef struct tagMW_TEMPCERT_CONFIG_T	MW_TEMPCERT_CONFIG_T;

#define sGET_MW_TEMPCERT_LENGTH(s)				(s).Length
#define sGET_MW_TEMPCERT_PIN_HASH(s)		 (s).PIN_Hash
#define sGET_MW_TEMPCERT_PRV_KEY(s)				(s).PRV_KEY
#define sGET_MW_TEMPCERT_SKID(s)					(s).SKID
#define sGET_MW_TEMPCERT_REQUESTDATE(s)		(s).RequestDate
#define sGET_MW_TEMPCERT_SUBJECT_DN(s)		(s).SUBJECT_DN
#define sGET_MW_TEMPCERT_KEY_USAGE(s)		(s).KEY_USAGE

/*
#define SKY_GET_MW_TEMPCERT_LENGTH(i)	 sGET_MW_TEMPCERT_LENGTH(g_MWTempCertConfig1.MWTempCert) 
#define SKY_GET_MW_TEMPCERT_PIN_HASH(i)	       sGET_MW_TEMPCERT_PIN_HASH(g_MWTempCertConfig1.MWTempCert)
#define SKY_GET_MW_TEMPCERT_PRV_KEY(i)	      sGET_MW_TEMPCERT_PRV_KEY(g_MWTempCertConfig1.MWTempCert)
#define SKY_GET_MW_TEMPCERT_SKID(i)			       sGET_MW_TEMPCERT_SKID(g_MWTempCertConfig1.MWTempCert)
#define SKY_GET_MW_TEMPCERT_REQUESTDATE(i)	    sGET_MW_TEMPCERT_REQUESTDATE(g_MWTempCertConfig1.MWTempCert)
#define SKY_GET_MW_TEMPCERT_SUBJECT_DN(i)	     sGET_MW_TEMPCERT_SUBJECT_DN(g_MWTempCertConfig1.MWTempCert)
#define SKY_GET_MW_TEMPCERT_KEY_USAGE(i)	        sGET_MW_TEMPCERT_KEY_USAGE(g_MWTempCertConfig1.MWTempCert)

// TempCertInfo 인증서 설치 정보
#define SKY_SET_MW_TEMPCERT_LENGTH(i,s,len)	      sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_LENGTH(i),s,len)
#define SKY_SET_MW_TEMPCERT_PIN_HASH(i,s,len)	      sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_PIN_HASH(i),s,len)
#define SKY_SET_MW_TEMPCERT_PRV_KEY(i,s,len)	       sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_PRV_KEY(i),s,len)
#define SKY_SET_MW_TEMPCERT_SKID(i,s,len)			       sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_SKID(i),s,len)
#define SKY_SET_MW_TEMPCERT_REQUESTDATE(i,s,len)	       sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_REQUESTDATE(i),s,len)
#define SKY_SET_MW_TEMPCERT_SUBJECT_DN(i,s,len)		       sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_SUBJECT_DN(i),s,len)
#define SKY_SET_MW_TEMPCERT_KEY_USAGE(i,s)		      sSET_MEMORY_VALUE(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_KEY_USAGE(i),s,len)
#define SKY_SET_MW_TEMPCERT(i)								       SKY_WRITE_CONFIG(SC_MW_TEMPCERT1)
*/
/* ==   MYCert 정보  ======================================================*/
struct tagMW_MYCERT_T
{
    //BYTE Length[MW_MYCERT_LENGTH];
	WORD Length;
	//BYTE CheckField[MW_MYCERT_CHECKFIELD];
	WORD CheckField;
	BYTE PIN_HASH[MW_MYCERT_PIN_HASH];
	BYTE PRV_KEY[MW_MYCERT_PRV_KEY];
	BYTE DISPLAY[MW_MYCERT_DISPLAY];
	BYTE SUBJECT_KID[MW_MYCERT_SUBJECT_KID];
	BYTE ISSUER_KID[MW_MYCERT_ISSUER_KID];
	BYTE CERT_URL[MW_MYCERT_CERT_URL];
	BYTE SUBJECT_DN[MW_MYCERT_SUBJECT_DN];
};
typedef struct tagMW_MYCERT_T	MW_MYCERT_INFO_T;

struct tagMW_MYCERT_CONFIG_T 
{
    SKY_FILE_HEADER_T			Header;			/* 파일 헤더 */
	MW_MYCERT_INFO_T		  MWMyCert;
};
typedef struct tagMW_MYCERT_CONFIG_T	MW_MYCERT_CONFIG_T;

#define sGET_MW_MYCERT_LENGTH(s)			   (s).Length
#define sGET_MW_MYCERT_CHECKFIELD(s)		(s).CheckField
#define sGET_MW_MYCERT_PIN_HASH(s)			(s).PIN_HASH
#define sGET_MW_MYCERT_PRV_KEY(s)			  (s).PRV_KEY
#define sGET_MW_MYCERT_DISPLAY(s)			   (s).DISPLAY
#define sGET_MW_MYCERT_SUBJECT_KID(s)		(s).SUBJECT_KID
#define sGET_MW_MYCERT_ISSUER_KID(s)		 (s).ISSUER_KID
#define sGET_MW_MYCERT_CERT_URL(s)			 (s).CERT_URL
#define sGET_MW_MYCERT_SUBJECT_DN(s)		(s).SUBJECT_DN

/*
#define SKY_GET_MW_MYCERT_LENGTH(i)	       sGET_MW_MYCERT_LENGTH(g_MWMYCertConfig1.MWMyCert)
#define SKY_GET_MW_MYCERT_CHECKFIELD(i)	 sGET_MW_MYCERT_CHECKFIELD(g_MWMYCertConfig1.MWMyCert)
#define SKY_GET_MW_MYCERT_PIN_HASH(i)	 sGET_MW_MYCERT_PIN_HASH(g_MWMYCertConfig1.MWMyCert)
#define SKY_GET_MW_MYCERT_PRV_KEY(i)	 sGET_MW_MYCERT_PRV_KEY(g_MWMYCertConfig1.MWMyCert)
#define SKY_GET_MW_MYCERT_DISPLAY(i)	 sGET_MW_MYCERT_DISPLAY(g_MWMYCertConfig1.MWMyCert)
#define SKY_GET_MW_MYCERT_SUBJECT_KID(i)	 sGET_MW_MYCERT_SUBJECT_KID(g_MWMYCertConfig1.MWMyCert)
#define SKY_GET_MW_MYCERT_ISSUER_KID(i)	   sGET_MW_MYCERT_ISSUER_KID(g_MWMYCertConfig1.MWMyCert)
#define SKY_GET_MW_MYCERT_CERT_URL(i)   sGET_MW_MYCERT_CERT_URL(g_MWMYCertConfig1.MWMyCert)
#define SKY_GET_MW_MYCERT_SUBJECT_DN(i)	   sGET_MW_MYCERT_SUBJECT_DN(g_MWMYCertConfig1.MWMyCert)

// MyCertInfo 인증서 정보
#define SKY_SET_MW_MYCERT_LENGTH(i,s,len)	  sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_LENGTH(i),s,len)
#define SKY_SET_MW_MYCERT_CHECKFIELD(i,s,len)	  sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_CHECKFIELD(i),s,len)
#define SKY_SET_MW_MYCERT_PIN_HASH(i,s,len)		  sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_PIN_HASH(i),s,len)
#define SKY_SET_MW_MYCERT_PRV_KEY(i,s,len)	  sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_PRV_KEY(i),s,len)
#define SKY_SET_MW_MYCERT_DISPLAY(i,s,len)	 sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_DISPLAY(i),s,len)
#define SKY_SET_MW_MYCERT_SUBJECT_KID(i,s,len)	   sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_SUBJECT_KID(i),s,len)
#define SKY_SET_MW_MYCERT_ISSUER_KID(i,s,len)	   sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_ISSUER_KID(i),s,len)
#define SKY_SET_MW_MYCERT_CERT_URL(i,s,len)		  sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_CERT_URL(i),s,len)
#define SKY_SET_MW_MYCERT_SUBJECT_DN(i,s,len)	 sSET_MEMORY_COPY(SC_MW_MYCERT1,SKY_GET_MW_MYCERT_SUBJECT_DN(i),s,len)
#define SKY_SET_MW_MYCERT(i)						 SKY_WRITE_CONFIG(SC_MW_MYCERT1);  \
*/
/* ==   TCACert 정보  ======================================================*/

struct tagMW_TCACERT_T
{
	//BYTE Length[MW_TCACERT_LENGTH];
	WORD Length;
	//BYTE CheckField[MW_TCACERT_CHECKFIELD];
	WORD CheckField;
	BYTE DISPLAY[MW_TCACERT_DISPLAY];
	BYTE SUBJECT_KID[MW_TCACERT_SUBJECT_KID];
	BYTE ISSUER_KID[MW_TCACERT_ISSUER_KID];
	BYTE CERT[MW_TCACERT_CERT];
	BYTE CA_INFO_URL[MW_TCACERT_CA_INFO_URL];
	BYTE SUBJECT_DN[MW_TCACERT_SUBJECT_DN];
	DWORD NOT_AFTER;
};
typedef struct tagMW_TCACERT_T MW_TCACERT_INFO_T;	

struct tagMW_TCACERT_CONFIG_T 
{
    SKY_FILE_HEADER_T			Header;			/* 파일 헤더 */
	MW_TCACERT_INFO_T		  MWTCACert;
};
typedef struct tagMW_TCACERT_CONFIG_T	MW_TCACERT_CONFIG_T;

#define sGET_MW_TCACERT_LENGTH(s)				(s).Length
#define sGET_MW_TCACERT_CHECKFIELD(s)		 (s).CheckField
#define sGET_MW_TCACERT_DISPLAY(s)			   (s).DISPLAY
#define sGET_MW_TCACERT_SUBJECT_KID(s)		(s).SUBJECT_KID
#define sGET_MW_TCACERT_ISSUER_KID(s)		(s).ISSUER_KID
#define sGET_MW_TCACERT_CERT(s)					 (s).CERT
#define sGET_MW_TCACERT_CA_INFO_URL(s)	   (s).CA_INFO_URL
#define sGET_MW_TCACERT_SUBJECT_DN(s)		(s).SUBJECT_DN
#define sGET_MW_TCACERT_NOT_AFTER(s)		 (s).NOT_AFTER

/*
#define SKY_GET_MW_TCACERT_LENGTH(i)	      sGET_MW_TCACERT_LENGTH(g_MWTCACertConfig1.MWTCACert)
#define SKY_GET_MW_TCACERT_CHECKFIELD(i)      sGET_MW_TCACERT_CHECKFIELD(g_MWTCACertConfig1.MWTCACert)
#define SKY_GET_MW_TCACERT_DISPLAY(i)	      sGET_MW_TCACERT_DISPLAY(g_MWTCACertConfig1.MWTCACert)
#define SKY_GET_MW_TCACERT_SUBJECT_KID(i)     sGET_MW_TCACERT_SUBJECT_KID(g_MWTCACertConfig1.MWTCACert)
#define SKY_GET_MW_TCACERT_ISSUER_KID(i)	     sGET_MW_TCACERT_ISSUER_KID(g_MWTCACertConfig1.MWTCACert)
#define SKY_GET_MW_TCACERT_CERT(i)		      sGET_MW_TCACERT_CERT(g_MWTCACertConfig1.MWTCACert)
#define SKY_GET_MW_TCACERT_CA_INFO_URL(i)         sGET_MW_TCACERT_CA_INFO_URL(g_MWTCACertConfig1.MWTCACert)
#define SKY_GET_MW_TCACERT_SUBJECT_DN(i)        sGET_MW_TCACERT_SUBJECT_DN(g_MWTCACertConfig1.MWTCACert)
#define SKY_GET_MW_TCACERT_NOT_AFTER(i)		       sGET_MW_TCACERT_NOT_AFTER(g_MWTCACertConfig1.MWTCACert)

// TCACertInfo 인증서 정보
#define SKY_SET_MW_TCACERT_LENGTH(i,s,len)	    sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_LENGTH(i),s,len)
#define SKY_SET_MW_TCACERT_CHECKFIELD(i,s,len)	         sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_CHECKFIELD(i),s,len)
#define SKY_SET_MW_TCACERT_DISPLAY(i,s,len)		      sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_DISPLAY(i),s,len)
define SKY_SET_MW_TCACERT_SUBJECT_KID(i,s,len)		       sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_SUBJECT_KID(i),s,len)
#define SKY_SET_MW_TCACERT_ISSUER_KID(i,s,len)		       sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_ISSUER_KID(i),s,len)
#define SKY_SET_MW_TCACERT_CERT(i,s,len)		       sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_CERT(i),s,len)
#define SKY_SET_MW_TCACERT_CA_INFO_URL(i,s,len)		        sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_CA_INFO_URL(i),s,len)
#define SKY_SET_MW_TCACERT_SUBJECT_DN(i,s,len)		       sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_SUBJECT_DN(i),s,len)
define SKY_SET_MW_TCACERT_NOT_AFTER(i,s,len)		      sSET_MEMORY_COPY(SC_MW_TCACERT1,SKY_GET_MW_TCACERT_NOT_AFTER(i),s,len)
define SKY_SET_MW_TCACERT(i)								     SKY_WRITE_CONFIG(SC_MW_TCACERT1)
*/
/* ==   전자지갑 정보  ======================================================*/

// 사용자의 전자지갑 정보를 저장 
struct tagMW_WALLET_T
{
    BYTE HashedPin[MW_WALLET_HASHEDPIN_LENGTH];
	UINT trial_num;
	BYTE user[MW_WALLET_USER_LENGTH];
	BYTE card[3][MW_WALLET_CARD_LENGTH];
	BYTE ocb[MW_WALLET_OCB_LENGTH];
	BYTE delivery[2][MW_WALLET_DELIVERY_LENGTH];

	LONG user_length;
	LONG card_length;
	LONG ocb_length;
	LONG delivery_length;

	BYTE dCardiv[MW_WALLET_DCARDIV_LENGTH];
	BYTE dOcbiv[MW_WALLET_DOCBIV_LENGTH];

	UINT WalletDataSaved[7];
	UINT TempCertDataSaved[2];
	UINT MyCertDataSaved[5];
	UINT TCACertDataSaved[10];

	// for Debugging
	BYTE Debug_Screen[200];
};
typedef struct tagMW_WALLET_T MW_WALLET_T;

struct tagMW_WALLET_CONFIG_T 
{
    SKY_FILE_HEADER_T   Header;			/* 파일 헤더 */
	MW_WALLET_T			  MWWallet;		/* MW Wallet Data */
};
typedef struct tagMW_WALLET_CONFIG_T	MW_WALLET_CONFIG_T;

#define sGET_MW_WALLET_HASHEDPIN(s)			    (s).HashedPin
#define sGET_MW_WALLET_TRIAL_NUM(s)			    (s).trial_num
#define sGET_MW_WALLET_USER(s)				         (s).user
#define sGET_MW_WALLET_CARD(s,i)			         (s).card[i]
#define sGET_MW_WALLET_OCB(s)				          (s).ocb
#define sGET_MW_WALLET_DELIVERY(s,i)		       (s).delivery[i] 
#define sGET_MW_WALLET_USER_LENGTH(s)		   (s).user_length 
#define sGET_MW_WALLET_CARD_LENGTH(s)		  (s).card_length
#define sGET_MW_WALLET_OCB_LENGTH(s)		   (s).ocb_length
#define sGET_MW_WALLET_DELIVERY_LENGTH(s)	(s).delivery_length 
#define sGET_MW_WALLET_DCARDIV(s)			      (s).dCardiv
#define sGET_MW_WALLET_DOCBIV(s)			        (s).dOcbiv
#define sGET_MW_WALLET_DATA_SAVED(s,i)		   (s).WalletDataSaved[i]
#define sGET_MW_TEMPCERT_DATA_SAVED(s,i)      (s).TempCertDataSaved[i]
#define sGET_MW_MYCERT_DATA_SAVED(s,i)		    (s).MyCertDataSaved[i]
#define sGET_MW_TCACERT_DATA_SAVED(s,i)		    (s).TCACertDataSaved[i]
#define sGET_MW_WALLET_DEBUG_SCREEN(s)		   (s).Debug_Screen

// Data Handling
// Get data
#define SKY_GET_MW_WALLET_HASHEDPIN()			\
						sGET_MW_WALLET_HASHEDPIN(g_MWWalletConfig.MWWallet)
#define SKY_GET_MW_WALLET_TRIAL_NUM()		    \
                        sGET_MW_WALLET_TRIAL_NUM(g_MWWalletConfig.MWWallet)
#define SKY_GET_MW_WALLET_USER()				\
                        sGET_MW_WALLET_USER(g_MWWalletConfig.MWWallet)
#define SKY_GET_MW_WALLET_CARD(i)				\
                        sGET_MW_WALLET_CARD(g_MWWalletConfig.MWWallet,i)
#define SKY_GET_MW_WALLET_OCB()				\
                        sGET_MW_WALLET_OCB(g_MWWalletConfig.MWWallet)
#define SKY_GET_MW_WALLET_DELIVERY(i)		    \
                        sGET_MW_WALLET_DELIVERY(g_MWWalletConfig.MWWallet,i) 
#define SKY_GET_MW_WALLET_USER_LENGTH()		    \
                        sGET_MW_WALLET_USER_LENGTH(g_MWWalletConfig.MWWallet) 
#define SKY_GET_MW_WALLET_CARD_LENGTH()		    \
                        sGET_MW_WALLET_CARD_LENGTH(g_MWWalletConfig.MWWallet)
#define SKY_GET_MW_WALLET_OCB_LENGTH()		    \
                        sGET_MW_WALLET_OCB_LENGTH(g_MWWalletConfig.MWWallet)
#define SKY_GET_MW_WALLET_DELIVERY_LENGTH()		\
                        sGET_MW_WALLET_DELIVERY_LENGTH(g_MWWalletConfig.MWWallet) 
#define SKY_GET_MW_WALLET_DCARDIV()				\
                        sGET_MW_WALLET_DCARDIV(g_MWWalletConfig.MWWallet)
#define SKY_GET_MW_WALLET_DOCBIV()		    \
                        sGET_MW_WALLET_DOCBIV(g_MWWalletConfig.MWWallet)
#define SKY_GET_MW_WALLET_DATA_SAVED(i)		\
						sGET_MW_WALLET_DATA_SAVED(g_MWWalletConfig.MWWallet,i)
#define SKY_GET_MW_TEMPCERT_DATA_SAVED(i)		\
						sGET_MW_TEMPCERT_DATA_SAVED(g_MWWalletConfig.MWWallet,i)
#define SKY_GET_MW_MYCERT_DATA_SAVED(i)		\
						sGET_MW_MYCERT_DATA_SAVED(g_MWWalletConfig.MWWallet,i)
#define SKY_GET_MW_TCACERT_DATA_SAVED(i)		\
						sGET_MW_TCACERT_DATA_SAVED(g_MWWalletConfig.MWWallet,i)
#define SKY_GET_MW_WALLET_DEBUG_SCREEN()		\
						sGET_MW_WALLET_DEBUG_SCREEN(g_MWWalletConfig.MWWallet)

// Set data
#define SKY_SET_MW_WALLET_HASHEDPIN(s,len)   \
						sSET_MEMORY_COPY(SC_MW_WALLET,SKY_GET_MW_WALLET_HASHEDPIN(),s,len)
#define SKY_SET_MW_WALLET_TRIAL_NUM(s)		    \
                        sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_WALLET_TRIAL_NUM(),s)
#define SKY_SET_MW_WALLET_USER(s,len)		    \
                        sSET_MEMORY_COPY(SC_MW_WALLET,SKY_GET_MW_WALLET_USER(),s,len)
#define SKY_SET_MW_WALLET_CARD(i,s,len)		    \
                        sSET_MEMORY_COPY(SC_MW_WALLET,SKY_GET_MW_WALLET_CARD(i),s,len)
#define SKY_SET_MW_WALLET_OCB(s,len)		    \
                        sSET_MEMORY_COPY(SC_MW_WALLET,SKY_GET_MW_WALLET_OCB(),s,len)
#define SKY_SET_MW_WALLET_DELIVERY(i,s,len)		    \
                        sSET_MEMORY_COPY(SC_MW_WALLET,SKY_GET_MW_WALLET_DELIVERY(i),s,len)
#define SKY_SET_MW_WALLET_USER_LENGTH(s)		    \
                        sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_WALLET_USER_LENGTH(),s)
#define SKY_SET_MW_WALLET_CARD_LENGTH(s)		    \
                        sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_WALLET_CARD_LENGTH(),s)
#define SKY_SET_MW_WALLET_OCB_LENGTH(s)		    \
                        sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_WALLET_OCB_LENGTH(),s)
#define SKY_SET_MW_WALLET_DELIVERY_LENGTH(s)		    \
                        sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_WALLET_DELIVERY_LENGTH(),s)
#define SKY_SET_MW_WALLET_DCARDIV(s,len)		    \
                        sSET_MEMORY_COPY(SC_MW_WALLET,SKY_GET_MW_WALLET_DCARDIV(),s,len)
#define SKY_SET_MW_WALLET_DOCBIV(s,len)		    \
                        sSET_MEMORY_COPY(SC_MW_WALLET,SKY_GET_MW_WALLET_DOCBIV(),s,len)
#define SKY_SET_MW_WALLET_DATA_SAVED(i,s)		\
						sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_WALLET_DATA_SAVED(i),s)
#define SKY_SET_MW_TEMPCERT_DATA_SAVED(i,s)		\
						sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_TEMPCERT_DATA_SAVED(i),s)
#define SKY_SET_MW_MYCERT_DATA_SAVED(i,s)		\
						sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_MYCERT_DATA_SAVED(i),s)
#define SKY_SET_MW_TCACERT_DATA_SAVED(i,s)		\
						sSET_MEMORY_VALUE(SC_MW_WALLET,SKY_GET_MW_TCACERT_DATA_SAVED(i),s)
#define SKY_SET_MW_WALLET_DEBUG_SCREEN(s,len)	\
						sSET_MEMORY_COPY(SC_MW_WALLET,SKY_GET_MW_WALLET_DEBUG_SCREEN(),s,len)

#define SKY_SET_MW_WALLET()		\
						SKY_WRITE_CONFIG(SC_MW_WALLET)

#endif /* FEATURE_MCOMMERCE */

#ifdef FEATURE_CHARACTER_BELL
#define MAX_NUM_OF_CHARACBELL       5
#define MAX_NUM_OF_PHONENUMBER      6
#define MAX_TITLE_LENGTH            20
#define MAX_PHONENUMBER_LENGTH      (MAX_TITLE_LENGTH+1)* MAX_NUM_OF_PHONENUMBER // (20+,)*6=126

struct tagCHARACBELL_T
{
    UINT8   Empty;
    char    CharacbellTitle[MAX_TITLE_LENGTH];
    WORD    MelodyType;
    char    MelodyTitle[MAX_TITLE_LENGTH];
    WORD    ImageType;
    char    ImageTitle[MAX_TITLE_LENGTH];
    char    PhoneNumber[MAX_PHONENUMBER_LENGTH];
};

typedef struct tagCHARACBELL_T CHARACBELL_T;

struct tagCHARACBELL_CONFIG_T 
{
    SKY_FILE_HEADER_T   Header;			/* 파일 헤더 */
    BYTE                Address;			
	CHARACBELL_T        CBLIST[MAX_NUM_OF_CHARACBELL];        // Characbell Data 
};
typedef struct tagCHARACBELL_CONFIG_T	CHARACBELL_CONFIG_T;

#define sGET_CB_ADDRESS(s)              (s).Address
#define sGET_CB_EMPTY(s, i)             (s).CBLIST[i].Empty
#define sGET_CB_TITLE(s, i)             (s).CBLIST[i].CharacbellTitle
#define sGET_CB_MTYPE(s, i)             (s).CBLIST[i].MelodyType
#define sGET_CB_MTITLE(s, i)            (s).CBLIST[i].MelodyTitle
#define sGET_CB_ITYPE(s, i)             (s).CBLIST[i].ImageType
#define sGET_CB_ITITLE(s, i)            (s).CBLIST[i].ImageTitle
#define sGET_CB_PHONE(s, i)             (s).CBLIST[i].PhoneNumber

#define SKY_GET_CB_ADDRESS()		    \
                        sGET_CB_ADDRESS(g_CharacbellConfig)
#define SKY_GET_CB_EMPTY(i)             \
                        sGET_CB_EMPTY(g_CharacbellConfig, i)
#define SKY_GET_CB_TITLE(i)             \
                        sGET_CB_TITLE(g_CharacbellConfig, i)
#define SKY_GET_CB_MTYPE(i)             \
                        sGET_CB_MTYPE(g_CharacbellConfig, i)
#define SKY_GET_CB_MTITLE(i)            \
                        sGET_CB_MTITLE(g_CharacbellConfig, i)
#define SKY_GET_CB_ITYPE(i)             \
                        sGET_CB_ITYPE(g_CharacbellConfig, i)
#define SKY_GET_CB_ITITLE(i)            \
                        sGET_CB_ITITLE(g_CharacbellConfig, i)
#define SKY_GET_CB_PHONE(i)             \
                        sGET_CB_PHONE(g_CharacbellConfig, i)

#define SKY_SET_CB_ADDRESS(t)		    \
                        sSET_CONFIG_VALUE(SC_CHABELL, SKY_GET_CB_ADDRESS(), t)
#define SKY_SET_CB_EMPTY(i,t)		    \
                        sSET_CONFIG_VALUE(SC_CHABELL, SKY_GET_CB_EMPTY(i), t)
#define SKY_SET_CB_TITLE(i,t)		    \
                        sSET_CONFIG_STR(SC_CHABELL, SKY_GET_CB_TITLE(i), t)
#define SKY_SET_CB_MTYPE(i,t)		    \
                        sSET_CONFIG_VALUE(SC_CHABELL, SKY_GET_CB_MTYPE(i), t)
#define SKY_SET_CB_MTITLE(i,t)		    \
                        sSET_CONFIG_STR(SC_CHABELL, SKY_GET_CB_MTITLE(i), t)
#define SKY_SET_CB_ITYPE(i,t)		    \
                        sSET_CONFIG_VALUE(SC_CHABELL, SKY_GET_CB_ITYPE(i), t)
#define SKY_SET_CB_ITITLE(i,t)		    \
                        sSET_CONFIG_STR(SC_CHABELL, SKY_GET_CB_ITITLE(i), t)
#define SKY_SET_CB_PHONE(i,t)		    \
                        sSET_CONFIG_STR(SC_CHABELL, SKY_GET_CB_PHONE(i), t)
#endif // FEATURE_CHARACTER_BELL
/*
******************************************************************************
******************************************************************************
**
**
**   GVM 정보 설정
**
**
******************************************************************************
*****************************************************************************
*/

/* rosa 01-06-07 --> @why, what */
/*===========================================================================
**  GVM 정보
*/
#ifdef undersea
#ifdef FEATURE_GVM

#define MAX_GVM_USER_ID_LENGTH              8
#define MAX_GVM_DLS_IP_LENGTH               32
#define MAX_GVM_RUN_SVR_LENGTH              32
#define MAX_GVM_URL_LENGTH                  64
#define MAX_GVM_GAME_NAME_LENGTH            16
#define MAX_GVM_CHAT_TEXT_NUM               9
#define MAX_GVM_CHAT_TEXT_LENGTH            20

#define MAX_GVM_SYSTEM_TYPE                 5
#define MAX_GVM_USER_DATA_LENGTH            64
#define MAX_GVM_SLOT					    5

/*===========================================================================
**  응용프로그램 정보 - GVM
*/
struct tagGVM_SYSTEM_T
{
	BYTE	svUserID[MAX_GVM_USER_ID_LENGTH+1]; //8
    WORD    svGameCPID;
    WORD    svGameID;
	BYTE	svDlsIP[MAX_GVM_DLS_IP_LENGTH+1];
	WORD	svDlsPort;
	BYTE	svRunSvrIP[MAX_GVM_RUN_SVR_LENGTH+1];
	WORD	svRunSvrPort;
	BYTE	svRetURL[MAX_GVM_URL_LENGTH+1];
	BYTE	svGameName[MAX_GVM_GAME_NAME_LENGTH+1];
    WORD    svGameVer;
	WORD	svGameType;
	DWORD	svGameSize;
	BYTE	svGameMaxUser;
};
typedef struct tagGVM_SYSTEM_T GVM_SYSTEM_T;

struct tagGVM_SYSTEM_TYPE_T
{
	BYTE            Address;
	GVM_SYSTEM_T    System_Info[MAX_GVM_SYSTEM_TYPE];
};

struct tagGVM_USER_T
{
    BYTE    UserData[MAX_GVM_SLOT][MAX_GVM_USER_DATA_LENGTH+1];
};

struct tagGVM_CHATTEXT_T
{
    CHAR    svRsvdChat[MAX_GVM_CHAT_TEXT_NUM][MAX_GVM_CHAT_TEXT_LENGTH];
};

struct tagGVM_SSB_T
{
    BOOL    svSSB[MAX_GVM_SLOT];
};

typedef struct tagGVM_SYSTEM_TYPE_T GVM_SYSTEM_TYPE_T;
typedef struct tagGVM_USER_T        GVM_USER_T;
typedef struct tagGVM_CHATTEXT_T    GVM_CHATTEXT_T;
typedef struct tagGVM_SSB_T         GVM_SSB_T;
typedef struct tagGVM_CONFIG_T      GVM_CONFIG_T;

struct tagGVM_CONFIG_T 
{
    SKY_FILE_HEADER_T   Header;            /* 파일 헤더 */
	GVM_SYSTEM_TYPE_T	GVMSystem;		   /* GVM 환경 변수 */
	GVM_USER_T			GVMUser;		   /* GVM User Data */
	GVM_CHATTEXT_T		GVMChatText;	   /* GVM Chatting Text*/
	GVM_SSB_T			GVMSSB;            /* GVM SSB */
};

#define sGET_GVM_ADDRESS(s)             (s).Address;
#define sGET_GVM_USER_ID(s,i)		    (s).System_Info[i].svUserID
#define sGET_GVM_GAME_CPID(s,i)         (s).System_Info[i].svGameCPID
#define sGET_GVM_GAME_ID(s,i)           (s).System_Info[i].svGameID
#define sGET_GVM_DLOAD_SVR_IP(s,i)      (s).System_Info[i].svDlsIP
#define sGET_GVM_DLOAD_SVR_PORT(s,i)    (s).System_Info[i].svDlsPort
#define sGET_GVM_RUN_SVR_IP(s,i)        (s).System_Info[i].svRunSvrIP
#define sGET_GVM_RUN_SVR_PORT(s,i)      (s).System_Info[i].svRunSvrPort
#define sGET_GVM_RET_URL(s,i)           (s).System_Info[i].svRetURL
#define sGET_GVM_GAME_NAME(s,i)         (s).System_Info[i].svGameName
#define sGET_GVM_GAME_VER(s,i)          (s).System_Info[i].svGameVer
#define sGET_GVM_GAME_TYPE(s,i)         (s).System_Info[i].svGameType
#define sGET_GVM_GAME_SIZE(s,i)         (s).System_Info[i].svGameSize
#define sGET_GVM_GAME_MAX_USER(s,i)     (s).System_Info[i].svGameMaxUser

#define sGET_GVM_USER(s, i)		        (s).UserData[i]
#define sGET_GVM_CHATTEXT(s, i)		    (s).svRsvdChat[i]
#define sGET_GVM_SSB(s, i)		        (s).svSSB[i]

/* GVM 데이터 Handling */
#define SKY_GET_GVM_ADDRESS()		    \
                        sGET_GVM_ADDRESS(g_GVMConfig.GVMSystem)
#define SKY_GET_GVM_USER_ID(i)		    \
                        sGET_GVM_USER_ID(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_GAME_CPID(i)        \
                        sGET_GVM_GAME_CPID(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_GAME_ID(i)          \
                        sGET_GVM_GAME_ID(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_DLOAD_SVR_IP(i)     \
                        sGET_GVM_DLOAD_SVR_IP(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_DLOAD_SVR_PORT(i)   \
                        sGET_GVM_DLOAD_SVR_PORT(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_RUN_SVR_IP(i)       \
                        sGET_GVM_RUN_SVR_IP(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_RUN_SVR_PORT(i)     \
                        sGET_GVM_RUN_SVR_PORT(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_RET_URL(i)          \
                        sGET_GVM_RET_URL(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_GAME_NAME(i)        \
                        sGET_GVM_GAME_NAME(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_GAME_VER(i)         \
                        sGET_GVM_GAME_VER(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_GAME_TYPE(i)        \
                        sGET_GVM_GAME_TYPE(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_GAME_SIZE(i)        \
                        sGET_GVM_GAME_SIZE(g_GVMConfig.GVMSystem, i)
#define SKY_GET_GVM_GAME_MAX_USER(i)    \
                        sGET_GVM_GAME_MAX_USER(s).GVMSystem, i)
#define SKY_GET_GVM_USER(i)		    \
                        sGET_GVM_USER(g_GVMConfig.GVMUser, i)
#define SKY_GET_GVM_CHATTEXT(i)		\
                        sGET_GVM_CHATTEXT(g_GVMConfig.GVMChatText, i)
#define SKY_GET_GVM_SSB(i)		        \
                        sGET_GVM_SSB(g_GVMConfig.GVMSSB, i)


/* GVM 데이터 Handling */
#define SKY_SET_GVM_ADDRESS(t)		    \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_ADDRESS(), t)
#define SKY_SET_GVM_USER_ID(i,t)		    \
                        sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_USER_ID(i), t)
#define SKY_SET_GVM_GAME_CPID(i,t)        \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_CPID(i), t)
#define SKY_SET_GVM_GAME_ID(i,t)          \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_ID(i), t)
#define SKY_SET_GVM_DLOAD_SVR_IP(i,t)     \
                        sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_DLOAD_SVR_IP(i), t)
#define SKY_SET_GVM_DLOAD_SVR_PORT(i,t)   \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_DLOAD_SVR_PORT(i), t)
#define SKY_SET_GVM_RUN_SVR_IP(i,t)       \
                        sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_RUN_SVR_IP(i), t)
#define SKY_SET_GVM_RUN_SVR_PORT(i,t)     \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_RUN_SVR_PORT(i), t)
#define SKY_SET_GVM_RET_URL(i,t)          \
                        sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_RET_URL(i), t)
#define SKY_SET_GVM_GAME_NAME(i,t)        \
                        sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_GAME_NAME(i), t)
#define SKY_SET_GVM_GAME_VER(i,t)         \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_VER(i), t)
#define SKY_SET_GVM_GAME_TYPE(i,t)        \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_TYPE(i), t)
#define SKY_SET_GVM_GAME_SIZE(i,t)        \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_SIZE(i), t)
#define SKY_SET_GVM_GAME_MAX_USER(i,t)    \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_GAME_MAX_USER(i), t)
#define SKY_SET_GVM_USER(i,t,len)		    \
                        sSET_CONFIG_COPY(SC_GVM,SKY_GET_GVM_USER(i), t, len)
#define SKY_SET_GVM_CHATTEXT(i,t)		\
                        sSET_CONFIG_STR(SC_GVM,SKY_GET_GVM_CHATTEXT(i), t)
#define SKY_SET_GVM_SSB(i, t)		        \
                        sSET_CONFIG_VALUE(SC_GVM,SKY_GET_GVM_SSB(i), t)

#endif //FEATURE_GVM
#endif // #ifdef undersea

/* <-- */

enum tagPOWER_OFF_MODE {
    POM_NONE,       /* power off가 아직 실행되지 않았음 */
    POM_DOWN,       /* 완전히 전원을 내림 */
    POM_RESET       /* 전원을 내린 후 다시 켬 */
};
typedef enum tagPOWER_OFF_MODE     POWER_OFF_MODE;

enum tagLOOP_BACK_E {
    LB_8K,
    LB_13K
};
typedef enum tagLOOP_BACK_E     LOOP_BACK_E;

enum tagSERVICE_PROVIDER_E {
    SP_SKT,
    SP_STI,
    SP_NONE
};
typedef enum tagSERVICE_PROVIDER_E  SERVICE_PROVIDER_E;

/*
******************************************************************************
******************************************************************************
**
**
**   단말기 상태 정보
**
**
******************************************************************************
******************************************************************************
*/
struct tagSYSTEM_STATE_T
{
    BOOL            fDebugScreen;       /* isdebug */
//#ifdef FEATURE_DS_DEBUG_UI
    BOOL            fDataDebugScreen;   /* isdatadebug */
//#endif /* FEATURE_DS_DEBUG_UI */

//    BOOL            fOK2Sleep;          /* ok_to_sleep */
    BOOL            fPowerOn;           /* pwr */
    BOOL            fPhoneOpen;         /* earpieceup */
    BOOL            fEarJack;
#if (SKY_MODEL_ID == SKY_MODEL_B1 ) || (SKY_MODEL_ID == SKY_MODEL_B2)
    BOOL            fCamera;            /*Camera*/
#endif    
    BOOL            fMute;              /* mute */
    BOOL            fShutDown;          /* ! powerup */
    BOOL            fOffline;           /* offline */
//    BOOL            fGetKeys;           /* getkeys */
    BOOL            fSysTimeValid;      /* systime_valid */
    BOOL            fHandsFree;         /* onhook */
    BOOL            fCharging;          /* charging */
    BOOL            fExtPower;          /* extpwr */
	BOOL			fDiagKeyTest;       /* diag keypad test */
    /* rosa 01-05-03 --> @why, what */
    BOOL            fToWAP;             /* to go to WAP */
    BOOL            fFromWAP;           /* to go from WAP */ 
#ifdef FEATURE_GVM
	BOOL            fGVM;               /* GVM state */
	BOOL            fGVMPlaying;        /* GVM state */
    BOOL            fToGVM;             /* to go GVM */
    BOOL            fFromGVM;             /* to go GVM */
#endif //FEATURE_GVM
    BOOL            fAlarm;             /* ALARM state */
    /* <-- */
#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) || defined(FEATURE_WAVELET)
	BOOL            fSockMode; 
	BOOL            fSockConnect;
#endif	//	#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) ||  defined(FEATURE_WAVELET)

#ifdef	FEATURE_WAVELET	//	khekim 01/02/11
	BOOL            fWavelet;			/* fWavelet state */
	BOOL			fFromWavelet;		/* From Wavelet */
	BOOL            fToWavelet;			/* To Wavelet */
#endif	//	#ifdef	FEATURE_WAVELET
    POWER_OFF_MODE fPowerOffMode;
    BOOL            fAutoLock;
    BOOL            fSMSLock;       
    BOOL            fNoOffBacklight;
    BOOL            fDataManager;     /* DataManager */
    BOOL            fAirDataManagerOrg;    /* AirDataManagerOrg */
    BOOL            fAirDataManagerRcv;     /* AirDataManagerRcv */
    BOOL            fIPAirDataManager;  /* IP AirDataManager */
    BOOL            fDataCall;
    BYTE            bDtrDisp;           /* 착신모드 */
    BOOL            fMaintReq;          /* 미등록단말기 */
#ifdef FEATURE_AGILENT_TEST
    BOOL            fAgilentTest;       /* for Agilent Test */
    LOOP_BACK_E     LoopBack;
    BOOL            fPowerControl;
#endif//#ifdef FEATURE_AGILENT_TEST
#ifdef FEATURE_SKT_NDSS_DS
    BOOL            fNDSSOriFail;
#endif //FEATURE_SKT_NDSS_DS
    /* rosa 01-05-03 --> @why, what */
#ifdef FEATURE_SKTT_PM
    BOOL            fPM;            /*  PM */
#endif //FEATURE_SKTT_PM
    /* <-- */
#ifdef FEATURE_SKVM
    BOOL            finSKVM;
    BOOL            ftoSKVM;
    BOOL            ffromSKVM;
	BOOL			fPushSMSInSkvm;
#endif //FEATURE_SKVM
#ifdef FEATURE_WAP_WAVELET
    BOOL            fWAVELET;
    BOOL            fWAVELETtoWAP;
#endif //FEATURE_WAP_WAVELET
    HFILE           DMTempFile;
    BOOL            fIgnoreKeyEventBacklight;
    BOOL            fAutoAnswerFromPowerUp;
    BOOL            fNewMessage;                /* new SMS message */
    BOOL            fInGame;
    BOOL            fEmailInProg;
#ifdef FEATURE_WAP    
    BOOL            fWAP;       /* ui.wap */
#endif //#ifdef FEATURE_WAP    
    BOOL            fVirginSystem;
    SERVICE_PROVIDER_E  ServiceProvider;
};
typedef struct tagSYSTEM_STATE_T         SYSTEM_STATE_T;

/*
**===========================================================================
**  단말기 상태 정보 얻기
**===========================================================================
*/
/* 음성 통화 - 디버그 화면 상태 */
#define SKY_IS_DEBUG_SCREEN()             g_SysState.fDebugScreen
/* 데이터 서비스 - 디버그 화면 상태  data_debug_is_enabled() */
#define SKY_IS_DATA_DEBUG_SCREEN()        g_SysState.fDataDebugScreen
/* sleep 상태로 들어 갈 수 있는 상태 */
//#define SKY_IS_OK_TO_SLEEP()              g_SysState.fOK2Sleep
/* 전원이 들어왔나? - off 상태를 수행했다는 뜻  */
#define SKY_IS_POWER_ON()                 g_SysState.fPowerOn
/* 플립이 열리거나, 폴더가 열리거나 */
#define SKY_IS_PHONE_OPEN()               g_SysState.fPhoneOpen
#define SKY_IS_PHONE_CLOSE()              (!g_SysState.fPhoneOpen)
/* ear jack이 꼽히거나 뽑히거나 */
#define SKY_IS_EAR_JACK_ON()              g_SysState.fEarJack
#define SKY_IS_EAR_JACK_OFF()             (!g_SysState.fEarJack)

#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
#define SKY_IS_CAMERA_ON()                 g_SysState.fCamera
#define SKY_IS_CAMERA_OFF()                (!g_SysState.fCamera)
#endif

/* 송화음 차단 */
#define SKY_IS_MUTE()                     g_SysState.fMute
/* SHUT DOWN 중인가? */
#define SKY_IS_SHUT_DOWN()                g_SysState.fShutDown
/* ui task가 offline 상태인가? */
#define SKY_IS_OFFLINE()                  g_SysState.fOffline
/* Whenther state machine is accepting keys */
//#define SKY_IS_GETKEY()                   g_SysState.fGetKeys
/* is valid system time? entering DIGITAL service */
#define SKY_IS_VALID_SYSTEM_TIME()        g_SysState.fSysTimeValid
/* 핸즈 프리 상태 */
#define SKY_IS_HANDS_FREE()               g_SysState.fHandsFree
/* 충전 중 */
#define SKY_IS_CHARGING()                 g_SysState.fCharging
/* 외부 전원 */
#define SKY_IS_EXTERNAL_POWER()           g_SysState.fExtPower
/* Diag Key Pad Test */
#define SKY_IS_KEYPAD_TEST()              g_SysState.fDiagKeyTest
/* rosa 01-05-03 --> @why, what */
/* WAP으로 이동 상태 */
#define SKY_IS_TO_WAP()                   g_SysState.fToWAP
#define SKY_IS_FROM_WAP()                 g_SysState.fFromWAP
#ifdef FEATURE_GVM
/* GVM상태 */
#define SKY_IS_IN_GVM()                   g_SysState.fGVM
/* GVM 플레이 상태 */
#define SKY_IS_PLAYING_GVM()              g_SysState.fGVMPlaying
/* GVM으로 이동 상태 */
#define SKY_IS_TO_GVM()                   g_SysState.fToGVM
#define SKY_IS_FROM_GVM()                 g_SysState.fFromGVM
#endif //FEATURE_GVM
#define SKY_IS_IN_ALARM()                 g_SysState.fAlarm
/* <-- */
#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) || defined(FEATURE_WAVELET)
#define SKY_IS_SOCK_MODE()			      g_SysState.fSockMode
#define SKY_IS_SOCK_CONNECT()			  g_SysState.fSockConnect
#endif	//	#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) || defined(FEATURE_WAVELET)

#ifdef	FEATURE_WAVELET	//	khekim 01/02/11
#define SKY_IS_IN_WAVELET()					g_SysState.fWavelet
#define SKY_IS_FROM_WAVELET()                 g_SysState.fFromWavelet
#define SKY_IS_TO_WAVELET()                   g_SysState.fToWavelet
#endif	//	#ifdef	FEATURE_WAVELET
/* 전원이 내려 갈 때 완전히 off시키는 것인지, reset 시키는 것인지 구분한다. */
#define SKY_GET_POWER_OFF_MODE()           g_SysState.fPowerOffMode
/* 자동잠금*/
#define SKY_GET_AUTO_LOCK_MODE()          g_SysState.fAutoLock
/* SMS 잠금 상태*/
#define SKY_GET_SMS_LOCK_MODE()             g_SysState.fSMSLock
/* 조명 끄지 않는 상태*/
#define SKY_GET_NO_OFF_BACKLIGHT()          g_SysState.fNoOffBacklight
/* DataManager */
#define SKY_GET_DATAMANAGER()               g_SysState.fDataManager
/* AirDataManagerOrg */
#define SKY_GET_AIRDATAMANAGER_ORG()        g_SysState.fAirDataManagerOrg
/* AirDataManagerRcv */
#define SKY_GET_AIRDATAMANAGER_RCV()        g_SysState.fAirDataManagerRcv
#define SKY_GET_AIRDATAMANAGER()            SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV()  
/* IPAirDataManager */
#define SKY_GET_IP_AIRDATAMANAGER()         g_SysState.fIPAirDataManager
/* DataCall */
#define SKY_GET_DATACALL()                  g_SysState.fDataCall
/* 착신모드 */
#define SKY_GET_DTR_DISP()                  g_SysState.bDtrDisp
/* 미등록 단말기 */
#define SKY_IS_MAINTREQ()                   g_SysState.fMaintReq

#ifdef FEATURE_AGILENT_TEST
#define SKY_IS_AGILENT_TEST()               g_SysState.fAgilentTest
#define SKY_GET_LOOP_BACK()                 g_SysState.LoopBack
#define SKY_IS_POWER_CONTROL()              g_SysState.fPowerControl
#endif//#ifdef FEATURE_AGILENT_TEST
#ifdef FEATURE_SKT_NDSS_DS
#define SKY_IS_NDSS_ORG_FAIL()              g_SysState.fNDSSOriFail
#endif //FEATURE_SKT_NDSS_DS

#ifdef FEATURE_SKTT_PM
#define SKY_IS_PM()                         g_SysState.fPM
#endif //FEATURE_SKTT_PM

#ifdef FEATURE_SKVM
#define SKY_IS_IN_SKVM()			        g_SysState.finSKVM
#define SKY_IS_TO_SKVM()			        g_SysState.ftoSKVM
#define SKY_IS_FROM_SKVM()			        g_SysState.ffromSKVM
#define	SKY_IS_PUSH_SMS_IN_SKVM()			g_SysState.fPushSMSInSkvm
#endif //FEATURE_SKVM

#ifdef FEATURE_WAP_WAVELET
#define SKY_IS_WAP_TO_WAVELET()			    g_SysState.fWAVELET    
#define SKY_IS_WAVELET_TO_WAP()		        g_SysState.fWAVELETtoWAP    
#endif //FEATURE_WAP_WAVELET
#define SKY_GET_DM_TEMP_FILE()		        g_SysState.DMTempFile    
#define SKY_GET_IGNORE_BACKLIGHT()          g_SysState.fIgnoreKeyEventBacklight
#define SKY_IS_AUTO_ANSWER_FROM_POWERUP()   g_SysState.fAutoAnswerFromPowerUp

#ifdef FEATURE_WAP
#define SKY_IS_IN_WAP()                     g_SysState.fWAP
#endif //FEATURE_WAP

#define SKY_IS_NEW_MESSAGE()                g_SysState.fNewMessage
#define SKY_IS_IN_GAME()                    g_SysState.fInGame
#define SKY_IS_EMAIL_INPROG()               g_SysState.fEmailInProg

#define SKY_IS_VIRGIN_SYSTEM()              g_SysState.fVirginSystem

#define SKY_GET_SERVICE_PROVIDER()          g_SysState.ServiceProvider

/*
**===========================================================================
**  단말기 상태 정보 설정
**===========================================================================
*/
/* 음성 통화 - 디버그 화면 상태 */
#define SKY_SET_DEBUG_SCREEN(f)             SKY_IS_DEBUG_SCREEN() = f
/* 데이터 서비스 - 디버그 화면 상태 */
#define SKY_SET_DATA_DEBUG_SCREEN(f)        SKY_IS_DATA_DEBUG_SCREEN() = f
/* sleep 상태로 들어 갈 수 있는 상태 */
//#define SKY_SET_OK_TO_SLEEP(f)              SKY_IS_OK_TO_SLEEP() = f
/* 전원이 들어왔나? - off 상태를 수행했다는 뜻  */
#define SKY_SET_POWER_ON(f)                 SKY_IS_POWER_ON() = f
/* 플립이 열리거나, 폴더가 열리거나 */
#define SKY_SET_PHONE_OPEN()                SKY_IS_PHONE_OPEN() = TRUE
#define SKY_SET_PHONE_CLOSE()               SKY_IS_PHONE_OPEN() = FALSE
/* ear jack이 꼽히거나 뽑히거나 */
#define SKY_SET_EAR_JACK_ON()               SKY_IS_EAR_JACK_ON() = TRUE
#define SKY_SET_EAR_JACK_OFF()              SKY_IS_EAR_JACK_ON() = FALSE
/* Camera jack 인식*/
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
#define SKY_SET_CAMERA_ON()                 SKY_IS_CAMERA_ON() = TRUE
#define SKY_SET_CAMERA_OFF()                SKY_IS_CAMERA_ON() = FALSE
#endif
/* 송화음 차단 */
#define SKY_SET_MUTE(f)                     SKY_IS_MUTE() = f
/* SHUT DOWN 중인가? */
#define SKY_SET_SHUT_DOWN(f)                SKY_IS_SHUT_DOWN() = f
/* ui task가 offline 상태인가? */
#define SKY_SET_OFFLINE(f)                  SKY_IS_OFFLINE() = f
/* Whenther state machine is accepting keys */
//#define SKY_SET_GETKEY(f)                   SKY_IS_GETKEY() = f
/* is valid system time? entering DIGITAL service */
#define SKY_SET_VALID_SYSTEM_TIME(f)        SKY_IS_VALID_SYSTEM_TIME() = f
/* 핸즈 프리 상태 */
#define SKY_SET_HANDS_FREE(f)               SKY_IS_HANDS_FREE() = f
/* 충전 중 */
#define SKY_SET_CHARGING(f)                 SKY_IS_CHARGING() = f
/* 외부 전원 */
#define SKY_SET_EXTERNAL_POWER(f)           SKY_IS_EXTERNAL_POWER() = f
/* Diag Key Pad Test */
#define SKY_SET_DIAG_KEYPAD_TEST(f)			SKY_IS_KEYPAD_TEST() = f
/* rosa 01-05-03 --> @why, what */
/* WAP으로 이동 상태 */
#define SKY_SET_TO_WAP(f)			        SKY_IS_TO_WAP() = f
#define SKY_SET_FROM_WAP(f)			        SKY_IS_FROM_WAP() = f
#ifdef FEATURE_GVM
/* GVM상태 */
#define SKY_SET_IN_GVM(f)			        SKY_IS_IN_GVM() = f
/* GVM 플레이 상태 */
#define SKY_SET_PLAYING_GVM(f)		        SKY_IS_PLAYING_GVM() = f
/* GVM으로 이동 상태 */
#define SKY_SET_TO_GVM(f)			        SKY_IS_TO_GVM() = f
#define SKY_SET_FROM_GVM(f)			        SKY_IS_FROM_GVM() = f
#endif //FEATURE_GVM
#define SKY_SET_IN_ALARM(f)			        SKY_IS_IN_ALARM() = f
/* <-- */
#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) || defined(FEATURE_WAVELET)
#define SKY_SET_SOCK_MODE(f)			    SKY_IS_SOCK_MODE() = f
#define SKY_SET_SOCK_CONNECT(f)			    SKY_IS_SOCK_CONNECT() = f
#endif	//	#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) || defined(FEATURE_WAVELET)

#ifdef	FEATURE_WAVELET	//	khekim 01/02/11
#define SKY_SET_IN_WAVELET(f)					SKY_IS_IN_WAVELET() = f
#define SKY_SET_FROM_WAVELET(f)					SKY_IS_FROM_WAVELET() = f
#define SKY_SET_TO_WAVELET(f)					SKY_IS_TO_WAVELET() = f
#endif	//	#ifdef	FEATURE_WAVELET
/* 전원이 내려 갈 때 완전히 off시키는 것인지, reset 시키는 것인지 구분한다. */
#define SKY_SET_POWER_OFF_MODE(m)           SKY_GET_POWER_OFF_MODE() = m
/* 자동잠금*/
#define SKY_SET_AUTO_LOCK_MODE(f)               SKY_GET_AUTO_LOCK_MODE() = f
/* SMS 잠금 */
#define SKY_SET_SMS_LOCK_MODE(f)                SKY_GET_SMS_LOCK_MODE() = f
/* 조명 끄지 않는 상태*/
#define SKY_SET_NO_OFF_BACKLIGHT(f)             SKY_GET_NO_OFF_BACKLIGHT() = f
/* DataManager */
#define SKY_SET_DATAMANAGER(f)                  SKY_GET_DATAMANAGER() = f
/* AirDataManagerOrg */
#define SKY_SET_AIRDATAMANAGER_ORG(f)           SKY_GET_AIRDATAMANAGER_ORG() = f
/* AirDataManagerRcv */
#define SKY_SET_AIRDATAMANAGER_RCV(f)           SKY_GET_AIRDATAMANAGER_RCV() = f
/* IPAirDataManager */
#define SKY_SET_IP_AIRDATAMANAGER(f)            SKY_GET_IP_AIRDATAMANAGER() = f
/* DATACALL */
#define SKY_SET_DATACALL(f)                     SKY_GET_DATACALL() = f
/* 착신모드 */
#define SKY_SET_DTR_DISP(m)                      SKY_GET_DTR_DISP() = m
/* 미등록 단말기 */
#define SKY_SET_MAINTREQ(f)                     SKY_IS_MAINTREQ() = f

#ifdef FEATURE_AGILENT_TEST
#define SKY_SET_AGILENT_TEST(f)                 SKY_IS_AGILENT_TEST() = f
#define SKY_SET_LOOP_BACK(s)                    SKY_GET_LOOP_BACK() = s
#define SKY_SET_POWER_CONTROL(f)                SKY_IS_POWER_CONTROL() = f
#endif//#ifdef FEATURE_AGILENT_TEST

#ifdef FEATURE_SKT_NDSS_DS
#define SKY_SET_NDSS_ORG_FAIL(f)		        SKY_IS_NDSS_ORG_FAIL() = f
#endif //FEATURE_SKT_NDSS_DS

#define SKY_SET_PM(f) 		   	            SKY_IS_PM() = f

#ifdef FEATURE_SKVM
#define SKY_SET_IN_SKVM(f)			        SKY_IS_IN_SKVM() = f
#define SKY_SET_TO_SKVM(f)			        SKY_IS_TO_SKVM() = f
#define SKY_SET_FROM_SKVM(f)		        SKY_IS_FROM_SKVM() = f
#define	SKY_SET_PUSH_SMS_IN_SKVM(f)			SKY_IS_PUSH_SMS_IN_SKVM() = f
#endif //FEATURE_SKVM

#ifdef FEATURE_WAP_WAVELET
#define SKY_SET_WAP_TO_WAVELET(f)			        SKY_IS_WAP_TO_WAVELET() = f    
#define SKY_SET_WAVELET_TO_WAP(f)		        SKY_IS_WAVELET_TO_WAP() = f    
#endif //FEATURE_WAP_WAVELET
#define SKY_SET_DM_TEMP_FILE(h)                 SKY_GET_DM_TEMP_FILE() = h
#define SKY_SET_IGNORE_BACKLIGHT(f)             SKY_GET_IGNORE_BACKLIGHT() = f 
#define SKY_SET_AUTO_ANSWER_FROM_POWERUP(f)     SKY_IS_AUTO_ANSWER_FROM_POWERUP() = f

// AU4.3
#ifdef FEATURE_WAP 
#define SKY_SET_IN_WAP(f)		                SKY_IS_IN_WAP() = f
#endif //#ifdef FEATURE_WAP 


#define SKY_SET_NEW_MESSAGE(f)                  SKY_IS_NEW_MESSAGE() = f
#define SKY_SET_IN_GAME(f)                      SKY_IS_IN_GAME() = f 
#define SKY_SET_EMAIL_INPROG(f)                 SKY_IS_EMAIL_INPROG() = f 

#define SKY_SET_VIRGIN_SYSTEM(f)                SKY_IS_VIRGIN_SYSTEM() = f

#define SKY_SET_SERVICE_PROVIDER(s)             SKY_GET_SERVICE_PROVIDER() = s

/*
******************************************************************************
******************************************************************************
**
**
**   통화 상태 정보
**
**
******************************************************************************
******************************************************************************
*/
#include "cai.h" /* CAI_ALLOW_PI, CAI_RES_PI, CAI_NOT_AVAIL_PI */
#include "cm.h"  /* cm_ph_info_s_type, cm_nam_e_type */

typedef cm_ph_info_s_type   CM_PHONE_INFO_T;


enum tagPRESENTATION_INDICATIOR_E
{
    PI_ALLOW            = CAI_ALLOW_PI,
    PI_RESTRICTED       = CAI_RES_PI,
    PI_NOT_AVAILABLE    = CAI_NOT_AVAIL_PI
};
typedef enum tagPRESENTATION_INDICATIOR_E   PRESENTATION_INDICATIOR_E;

/* ui_cni_type */
struct tagCALLING_NUMBER_ID_T 
{
    BYTE szPhoneNumber[MAX_DIAL_DIGIT_LENGTH+1];  /* number */
    BYTE szName[MAX_NAME_LENGTH+1];          /* name associated with number */
    PRESENTATION_INDICATIOR_E PI;          /* Presentation Indication */
};
typedef struct tagCALLING_NUMBER_ID_T   CALLING_NUMBER_ID_T;

#define sGET_CNI_PHONE_NUMBER(s)   (s).szPhoneNumber
#define sGET_CNI_NAME(s)           (s).szName
#define sGET_CNI_PI(s)             (s).PI

/* cai.h */
typedef word    SERVICE_OPTION;

/*===========================================================================
**  통화 상태 정보
*/
struct tagCALL_STATE_T
{
    CM_PHONE_INFO_T     PhoneInfo;              /* ph_info */
    CALLING_NUMBER_ID_T CNI;                    /* cni */
    BOOL                fOTASPFailed;           /* otasp_failed */
    BOOL                fAlert;                 /* alert */
    BOOL                fRoamCall;              /* callroam */
    BOOL                fDigital;               /* digital */
    BOOL                fOriginate;             /* originate */
    BOOL                fTransmitDTMF;          /* txdtmf */
    SERVICE_OPTION      ForceServiceOption;     /* force_so */
    BOOL                fShortBurstDTMF;        /* short burst dtmf */
#ifdef FEATURE_UI_DORMANT
    BOOL                fDormant;               /* dormant */
#endif
};
typedef struct tagCALL_STATE_T          CALL_STATE_T;
/*
**===========================================================================
**  통화 상태 정보 얻기
**===========================================================================
*/
/* CM_PHONE_INFO_T */
#define SKY_GET_PHONE_INFO()              g_CallState.PhoneInfo
/* cm_srv_state_e_type   */
#define SKY_GET_PHONE_INFO_SRV_STATE()    g_CallState.PhoneInfo.srv_state
/* cm_roam_status_e_type */
#define SKY_GET_PHONE_INFO_ROAM_STATUS()   g_CallState.PhoneInfo.roam_status
/* cm_nam_e_type */
#define SKY_GET_PHONE_INFO_CURRENT_NAME() g_CallState.PhoneInfo.curr_nam
#define SKY_GET_PHONE_INFO_NAM_SELECT()   g_CallState.PhoneInfo.nam_sel
#define SKY_GET_PHONE_INFO_RSSI()         g_CallState.PhoneInfo.rssi
/* CNI */
#define SKY_GET_CNI()                     g_CallState.CNI
#define SKY_GET_CNI_PHONE_NUMBER()        sGET_CNI_PHONE_NUMBER(SKY_GET_CNI)
#define SKY_GET_CNI_NAME()                sGET_CNI_NAME(SKY_GET_CNI)
#define SKY_GET_CNI_PI()                  sGET_CNI_PI(SKY_GET_CNI)

/* Phone is in use staet */
#define SKY_IS_IN_USE()                   g_CallState.PhoneInfo.is_in_use
/* OTASP failed */
#define SKY_IS_OTASP_FAILED()             g_CallState.fOTASPFailed
/* alert */
#define SKY_IS_ALERT()                    g_CallState.fAlert
/* roaming call */
#define SKY_IS_ROAM_CALL()                g_CallState.fRoamCall
/* digital - CDMA */
#define SKY_IS_DIGITAL()                  g_CallState.fDigital
/* originate */
#define SKY_IS_ORIGINATE()                g_CallState.fOriginate
/* txdtmf */
#define SKY_IS_TRANSMIT_DTMF()            g_CallState.fTransmitDTMF
/* force service option */
#define SKY_GET_FORCE_SERVICE_OPTION()    g_CallState.ForceServiceOption
/* short burst dtmf     */
#define SKY_IS_SHORTBURST_DTMF()          g_CallState.fShortBurstDTMF
/* dormant */
#ifdef FEATURE_UI_DORMANT
#define SKY_IS_DORMANT()                  g_CallState.fDormant
#endif

/*
**===========================================================================
**  통화 상태 정보 설정
**===========================================================================
*/
/* CM_PHONE_INFO_T */
#define SKY_SET_PHONE_INFO(p)        SKY_GET_PHONE_INFO() = p
/* cm_nam_e_type */
#define SKY_SET_PHONE_INFO_CURRENT_NAME(n)  \
                                     SKY_GET_PHONE_INFO_CURRENT_NAME() = n
#define SKY_SET_PHONE_INFO_NAM_SELECT(n)   \
                                     SKY_GET_PHONE_INFO_NAM_SELECT() = n
/* CNI */
#define SKY_SET_CNI(c)               SKY_GET_CNI() = c
#define SKY_SET_CNI_PHONE_NUMBER(sz) STRCPY(SKY_GET_CNI_PHONE_NUMBER(), sz)
#define SKY_SET_CNI_NAME(sz)         STRCPY(SKY_GET_CNI_NAME(), sz)
#define SKY_SET_CNI_PI(p)            SKY_GET_CNI_PI() = p
/* OTASP failed */
#define SKY_SET_OTASP_FAILED(f)      SKY_IS_OTASP_FAILED() = f
/* alert */
#define SKY_SET_ALERT(f)             SKY_IS_ALERT() = f
/* roaming call */
#define SKY_SET_ROAM_CALL(f)         SKY_IS_ROAM_CALL() = f
/* digital - CDMA */
#define SKY_SET_DIGITAL(f)           SKY_IS_DIGITAL() = f
/* originate */
#define SKY_SET_ORIGINATE(f)         SKY_IS_ORIGINATE() = f
/* txdtmf */
#define SKY_SET_TRANSMIT_DTMF(f)     SKY_IS_TRANSMIT_DTMF() = f
/* force service option */
#define SKY_SET_FORCE_SERVICE_OPTION(o)  SKY_GET_FORCE_SERVICE_OPTION() = o
/* short burst dtmf */
#define SKY_SET_SHORTBURST_DTMF(f)       SKY_IS_SHORTBURST_DTMF() = f
/* dormant mode */
#ifdef FEATURE_UI_DORMANT
#define SKY_SET_DORMANT(f)              SKY_IS_DORMANT() = f
#endif

/*
******************************************************************************
******************************************************************************
**
**
**  초기값 정의
**
**
******************************************************************************
******************************************************************************
*/

/*----------------------------------------------------------------------------
**  음량 종류
**  --------
**  착신벨                      BELL
**  통화음                      SPEAKER
**  키톤                        KEY_TONE
**  마이크                      MIC
**  SMS 알람                    SMS_BELL
**  알람(스카이투데이 포함)       ALARM_BELL
*/
#define BELL_VOLUME_LEVEL               4
#define SPEAKER_VOLUME_LEVEL            4
#define KEY_TONE_VOLUME_LEVEL           4
#define SMS_BELL_VOLUME_LEVEL           4
#define ALARM_BELL_VOLUME_LEVEL         4
#define GAME_VOLUME_LEVEL               4

#define INIT_RESERVED_AREA              NULL

/*
**===========================================================================
**  기타 정보 
**===========================================================================
*/
/* 버전 정보 */
#define INIT_EXT_MAJOR_VERSION          SKY_GET_FILE_MAJOR_VERSION(SFL_EXT)
#define INIT_EXT_MINOR_VERSION          SKY_GET_FILE_MINOR_VERSION(SFL_EXT)
/* 현재 WRITE로 열어 놓은 데이터 파일 이름 */
#define INIT_OPEN_DATA_FILE_NAME        ""

/*
**===========================================================================
**  부가 기능
**===========================================================================
*/
/* 버전 정보 */
#define INIT_MISC_MAJOR_VERSION         SKY_GET_FILE_MAJOR_VERSION(SFL_MISC)
#define INIT_MISC_MINOR_VERSION         SKY_GET_FILE_MINOR_VERSION(SFL_MISC)
/* 자동 지역 번호 */
#define INIT_IS_AUTO_AREA_CODE          FALSE
#define INIT_AUTO_AREA_CODE             ""
/* 자동 조명 초기값 */
#define INIT_AUTO_BACKLIGHT             AB_10SECOND /* 항상 자동 조명 */
/* 시간별 자동 조명 - 시작 시간과 종료 시간 */
#define INIT_AUTO_BACKLIGHT_START_TIME  64800       /* 오후 7시 00분 */
#define INIT_AUTO_BACKLIGHT_END_TIME    25200       /* 오전 6시 00분 */
/*외부 LCD 조명색 설정*/
#define INIT_ID_LCD_IDLE                BLUE_GREEN_LCD_COLOR               
#define INIT_ID_LCD_ALERT               RAINBOW_LCD_COLOR
#define INIT_ID_LCD_SMS                 VIOLET_LCD_COLOR
#define INIT_ID_LCD_ALARM               BLUE_LCD_COLOR //ORANGE_LCD_COLOR              
/* 자동 재다이얼 */
#define INIT_IS_AUTO_REDIAL             TRUE
/* 자동 착신 */
#define INIT_IS_HANDS_FREE_AUTO_RECEIPT FALSE       /* 핸즈 프리 */
#define INIT_IS_EAR_PHONE_AUTO_RECEIPT  FALSE       /* 이어 잭 */
/* 넘버 플러스/투 넘버 서비스 */
#define INIT_IS_NUMBER_PLUS_DISPLAY     FALSE       /* 화면 표시 설정 */
/* 서비스 식별 번호 011-, 017- */
#define INIT_NUMBER_PLUS_NUMBER_011     SKY_SP_ID_SK_TELECOM
#define INIT_NUMBER_PLUS_NUMBER_017     SKY_SP_ID_SHINSEGI


#define INIT_VOICE_POND             "*89\0\0"
#define INIT_VOICE_POND_SET	        "*51\0\0"
#define INIT_VOICE_POND_IMME	    "*50\0\0"
#define INIT_VOICE_POND_RESET       "*52\0\0"
#define INIT_AUTO_CONNECT_SET       "*72\0\0"
#define INIT_AUTO_CONNECT_RESET     "*73\0\0"
#define INIT_CALL_REDIR_SET	        "*71\0\0"
#define INIT_CALL_REDIR_RESET       "*73\0\0"
#define INIT_INFO_POND		        "1511\0"
#define INIT_TRAFFIC_INFO	        "1333\0"
#define INIT_ORIG_BAN_FOREIGN       "*62\0\0"
#define INIT_ORIG_BAN_ALL	        "*61\0\0"
#define INIT_ORIG_BAN_RESET         "*63\0\0"
#define INIT_CALL_FORWARD_HOT_SKT   "*78\0\0"
#define INIT_CALL_PENDING_HOT_SKT   "*77\0\0"


/* 음성사서함 */
#define INIT_VOICE_MAIL_CONNECT     "*88\0\0"
#define INIT_VOICE_MAIL_SET	        "*82\0\0"
#define INIT_VOICE_MAIL_CANCEL	    "*83\0\0"
/* 자동연결 */
#define INIT_AUTO_CONNECT_REGISTER  "*72\0\0"
#define INIT_AUTO_CONNECT_CANCEL    "*73\0\0"
/* 무조건 전환 */
#define INIT_CALL_REDIR_REGISTER    "*71\0\0"
#define INIT_CALL_REDIR_CANCEL      "*73\0\0"
/* 통화중대기 */
#define INIT_CALL_WAITING_REGISTER  "*41\0\0"
#define INIT_CALL_WAITING_CANCEL    "*43\0\0"
/* 파워리모콘 */
#define INIT_POWER_REMOTE           "0172008282\0"
/* 파워멜로디 */
#define INIT_POWER_MELODY           "0177005782\0"
/* 호전환/보류 */
#define INIT_CALL_FORWARD_HOT_STI   "*20\0\0"
#define INIT_CALL_PENDING_HOT_STI   "*40\0\0"


/*
**===========================================================================
**  시스템 기능
**===========================================================================
*/
/* 버전 정보 */
#define INIT_SYS_MAJOR_VERSION          SKY_GET_FILE_MAJOR_VERSION(SFL_SYS)
#define INIT_SYS_MINOR_VERSION          SKY_GET_FILE_MINOR_VERSION(SFL_SYS)
/* -------------------------------------------------------------------------
** 국가 설정 
*/
#define INIT_COUNTRY                    COUNTRY_KOR
/* -------------------------------------------------------------------------
** 에티켓 
*/
#define INIT_IS_ETIQUETTE               FALSE
/* -------------------------------------------------------------------------
** 외부 스피커
*/
#define INIT_IS_LOUD_SPEAKER            FALSE
/* -------------------------------------------------------------------------
** 휴대폰 이름 
*/
#define INIT_BANNER                     SKY_MODEL_NAME
/* -------------------------------------------------------------------------
** 기본 암호 
*/
#define INIT_PASSWORD                   SKY_DEFAULT_PASSWORD
/* -------------------------------------------------------------------------
** 자동응답
*/
#define INIT_IS_AUTO_ANSWER             FALSE
/* 안내 음성 선택 - 일반 */
#define INIT_AUTO_ANSWER_GREETING       GM_GENERAL          /* 일반 */
/* 벨 울림 시간 */
#define INIT_AUTO_ANSWER_BELL_TIME      AAB_VIB_ONCE        /* 진동 1회 */
/* 메시지 개수 */
#define INIT_AUTO_ANSWER_COUNT          0
/* -------------------------------------------------------------------------
** 잠금 기능 
*/
/* 자동 잠금 */
#define INIT_IS_AUTO_LOCK               FALSE
/* 국제 전화 잠금 */
#define INIT_IS_OVERSEAS_CALL_LOCK      FALSE
/* 전화부 잠금 */
#define INIT_IS_PHONE_BOOK_LOCK         FALSE
/* 메뉴 잠금 */
#define INIT_IS_MENU_LOCK               FALSE
/* 착발신 이력 잠금 */
#define INIT_IS_CALL_STACK_LOCK         FALSE
/* 녹음/재생 잠금 */
#define INIT_IS_RECORD_PLAY_LOCK        FALSE
/* 메모 잠금 */
#define INIT_IS_MEMO_LOCK               FALSE
/* WAP 잠금 */
#define INIT_IS_WAP_LOCK                FALSE
/* SMS 잠금 */
#define INIT_IS_SMS_LOCK                FALSE

//#ifdef FEATURE_SKY_MODEL_TYPE_FOLDER
/*---------------------------------------------------------------------------
** 전화 받기 방법
*/
#define INIT_HOW_TO_ANSWER              HTA_OPEN

//#elif FEATURE_SKY_MODEL_TYPE_FLIP
/*---------------------------------------------------------------------------
** 플립키 설정
*/
#define INIT_FLIP_KEY                   FK_AUTO_ANSWER
#define INIT_MEMORY_DIAL_NUMBER         ""

//#endif /* FEATURE_SKY_MODEL_TYPE_XXX */

/*---------------------------------------------------------------------------
** NAM -- NV에 저장해야 함
*/
#define INIT_NAM                        CM_NAM_1

/*---------------------------------------------------------------------------
** 화면 밝기 
*/
#define INIT_CONTRAST                   HCL_D//HS_LCD_CONTRAST_D /* lcddef_xxx.h */
/*---------------------------------------------------------------------------
** WAP 설정
*/
/* 프록시 주소 */
#define INIT_WAP_PROXY_URL              SKY_WAP_PROXY_URL
/* 초기 URL */
#define INIT_WAP_INIT_URL               SKY_WAP_INIT_URL
/* 초기 WAP mail Server주소 */
#define INIT_WAP_INIT_MAIL              SKY_WAP_INIT_MAIL
/* 초기 wap dormant time */
#define INIT_WAP_DORMANT_TIME           SKY_WAP_DORMANT_TIME

/*---------------------------------------------------------------------------
** 벨/진동
*/
/* 벨 아이디 */
#if (MODEL_ID == MODEL_B0)
    #define INIT_BELL_ID                    MUS_JAPAN_TELEPHONE
#elif (MODEL_ID == MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    #define INIT_BELL_ID                    MUS_KURODEN
#endif
/* 벨 파일 이름 */
#define INIT_BELL_FILE_NAME             ""
/* 벨/진동 설정 */
#define INIT_BELL_VIBRATION             BV_BELL
/* 벨 음량 */
#define INIT_BELL_VOLUME                BELL_VOLUME_LEVEL
/* 벨 울림 시간 */
#define INIT_BELL_TIME                  BT_CONTINUATION

/*---------------------------------------------------------------------------
** 그림 설정
** 대기화면 기본값 : 
** - 이름표시 : OK (MODEL NAME)
** - 그림표시 : OK (SERVICE PROVIDER LOGO - SPEED 011 or POWER 017)
** - 시계종류 : 작은 시계
*/
/* 휴대폰 이름 표시 */
#define INIT_IDLE_DISPLAY_NAME          TRUE //FALSE
/* 대기 화면 종류 */
#define INIT_IDLE_SCREEN                IS_PICTURE
/* rosa 2001-09-20 --> @why, what */
/* 대기 화면 표시 방법 */
#define INIT_IDLE_DISPLAY_WATCH         IDW_SMALL //IDW_BIG
/* <-- */
/* 그림 사용함. FALSE --> 전부 사용안함  */
#define INIT_IS_PICTURE_USE             TRUE
/* 플립이 닫혔을 때, 폴더를 열었을 때 */
#define INIT_IDLE_PICTURE               BII_SERVICE_PROVIDER
/* 전화가 왔을 때 */
#define INIT_INCOMING_PICTURE           BII_RINGING
/* 전원을 켤 때 */
#define INIT_POWER_ON_PICTURE           BII_WATER_SKY
/* 전원을 끌 때 */
#define INIT_POWER_OFF_PICTURE          BII_GOODBYE
/* WAP 시작할 때 */
#define INIT_WAP_PICTURE                BII_WAP

/*---------------------------------------------------------------------------
** 음량
*/
/* 통화음 크기 */
#define INIT_SPEAKER_VOLUME             SPEAKER_VOLUME_LEVEL
/* 키톤 크기 */
#define INIT_KEY_TONE_VOLUME            KEY_TONE_VOLUME_LEVEL
/* 마이크 크기 */
#define INIT_MIC_VOLUME                 MV_MEDIUM
/* 게임 음량 */
#define INIT_GAME_VOLUME                GAME_VOLUME_LEVEL

/*---------------------------------------------------------------------------
** 경보음
*/
#define INIT_IS_CONNECT_WARNING_SIGNAL      TRUE
#define INIT_IS_1MINUTE_WARNING_SIGNAL      FALSE
#define INIT_IS_NOSERVICE_WARNING_SIGNAL    FALSE
#define INIT_IS_LOBATT_WARNING_SIGNAL       TRUE
#define INIT_IS_MESSAGE_BEEP_SOUND          TRUE

/*---------------------------------------------------------------------------
** 데이터 서비스
*/
/* 착신 모드 */
#define INIT_DATA_SERVICE_RECEIPT_MODE      RM_VOICE
/* 인터넷 속도 */
#define INIT_DATA_SERVICE_INTERNET_SPEED    IS_MAX
/* PC와 통신 속도 */
#define INIT_DATA_SERVICE_BPS_PC            BP_115200
/* AIR 데이터 매니저 */
#define INIT_AIR_DATA_MANAGER_PHONE_NUMBER  ""
#define INIT_AIR_DATA_MANAGER_IP_ADDRESS    ""

/*---------------------------------------------------------------------------
** 전자메일 계정
*/

#define INIT_EMAIL_ACCOUNT                  FALSE
#define INIT_EMAIL_USERNAME                 ""
#define INIT_EMAIL_USERID                   ""
#define INIT_EMAIL_PASSWORD                 ""
#define INIT_EMAIL_SMTP                     "nate.com"
#define INIT_EMAIL_POP3                     ""
#define INIT_EMAIL_USERINFO                 ""
#define INIT_EMAIL_ADDRESS                  ""
#define INIT_EMAIL_SERVER                   ""

/*---------------------------------------------------------------------------
** 배색
*/
//#define INIT_COLOR_SCHEME                   INIT_COLOR_SCHEME /* eqs_gui.h */
//#define INIT_SCROLLBAR_SIZE                 INIT_SCROLLBAR_SIZE /* eqs_gui.h */
//#define INIT_FONT_TYPE                      INIT_FONT_TYPE /* eqs_grph.h */

/*---------------------------------------------------------------------------
** 버전 정보
*/
#define INIT_DMSS_VERSION_INFO              DMSS_VERSION_INFO
#define INIT_EQUS_VERSION_INFO              EQUS_VERSION_INFO
#define INIT_SKY_VERSION_INFO               SKY_VERSION_INFO

#define INIT_SKY_LASTDIALINFO_SECRET        FALSE
#define INIT_SKY_LASTDIALINFO_BUFF          ""
#define INIT_SKY_LASTDIALINFO_POS           0

#define INIT_SKY_CAMERA_COUNT               1
#define INIT_SKY_HANGUL                     TRUE
#define INIT_SKY_MENU_MODE                  TRUE
#define INIT_SKY_SMART_BATTERY              FALSE
#define INIT_SKY_011_SERVICE                FALSE
#define INIT_SKY_PHOTO_MAIL                 1
/*
**===========================================================================
**  응용프로그램 정보
**===========================================================================
*/
/* 버전 정보 */
#define INIT_APP_MAJOR_VERSION              SKY_GET_FILE_MAJOR_VERSION(SFL_APP)
#define INIT_APP_MINOR_VERSION              SKY_GET_FILE_MINOR_VERSION(SFL_APP)
/* 알람 벨 설정 */
#if (MODEL_ID == MODEL_B0)
    #define INIT_ALARM_BELL_ID              MUS_BICYCLE_BELL//MUS_W_BELL /* musicdb.h */
#elif (MODEL_ID == MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    #define INIT_ALARM_BELL_ID              1//MUS_W_BELL /* musicdb.h */
#endif
#define INIT_ALARM_BELL_FILE_NAME           ""

#if (MODEL_ID == MODEL_B0)
    #define INIT_SCHL_ALERT_BELL_ID         MUS_BICYCLE_BELL//MUS_W_BELL /* musicdb.h */
#elif (MODEL_ID == MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    #define INIT_SCHL_ALERT_BELL_ID         1//MUS_W_BELL /* musicdb.h */
#endif
#define INIT_SCHL_ALERT_BELL_FILE_NAME      ""

#define INIT_ALARM_BELL_VOLUME              ALARM_BELL_VOLUME_LEVEL

/* 알람 설정 */
#define INIT_ALARM_HOLIDAY_ACTIVE           1

/* 알람 시간 설정 */
#define INIT_ALARM_TIME_TYPE                ATT_NONE
#define INIT_ALARM_TIME_ACTIVE              1
#define INIT_ALARM_TIME_USED                0
#define INIT_ALARM_TIME                     0
#define INIT_ALARM_TITLE                    ""  

/* 메모 설정 */
#define INIT_MEMO_TYPE                      MT_NONE
#define INIT_MEMO_TIME                      0
#define INIT_MEMO_DATA                      ""  

/* SMS 알람 설정 */
#define INIT_SMS_ALERT_TYPE                 SAT_ONCE
#if (MODEL_ID == MODEL_B0)
    #define INIT_SMS_ALERT_BELL_ID              MUS_CUCKOO
#elif (MODEL_ID == MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    #define INIT_SMS_ALERT_BELL_ID              MUS_SMS_1
#endif
#define INIT_SMS_ALERT_BELL_VOLUME          SMS_BELL_VOLUME_LEVEL
/* 음성, WAP 통화 시간 */
#define INIT_VOICE_LAST_USING_TIME          0
#define INIT_VOICE_CUMULATION_USING_TIME    0
#define INIT_VOICE_TOTAL_USING_TIME         0
#define INIT_WAP_LAST_USING_TIME            0
#define INIT_WAP_CUMULATION_USING_TIME      0
#define INIT_WAP_TOTAL_USING_TIME           0

#ifdef FEATURE_SKTT_EQUS_LOCATION
#define	INIT_LASTSID						0
#define INIT_LASTNID						0
#define	INIT_LASTZONE						0
#define	INIT_LASTBASEID						0
#define	INIT_LASTPN							0
#define	INIT_LASTBASELAT					0
#define	INIT_LASTBASELONG					0
#define	INIT_RCVINFO						0						// 위치통보 정보 수신 여부 결정
#define	INIT_ONOFFSTATE						0
#define	INIT_STARTTIME						0		
#define	INIT_ENDTIME						0	
#define	INIT_STARTMIN						0	
#define	INIT_ENDMIN							0		
#define	INIT_CYCLE							0		
#define	INIT_LAM3VER						0	
#define	INIT_REPORTWAIT						0	
#endif// #ifdef FEATURE_SKTT_EQUS_LOCATION

/*
**===========================================================================
**  GVM 초기값
**===========================================================================
*/
/* 버전 정보 */
#define INIT_GVM_MAJOR_VERSION              SKY_GET_FILE_MAJOR_VERSION(SFL_GVM)
#define INIT_GVM_MINOR_VERSION              SKY_GET_FILE_MINOR_VERSION(SFL_GVM)


/*
**===========================================================================
**  Mobile Wallet 초기값
**===========================================================================
*/

//#define INIT_MW_MAJOR_VERSION              SKY_GET_FILE_MAJOR_VERSION(SFL_MWALLET)
//#define INIT_MW_MINOR_VERSION               SKY_GET_FILE_MINOR_VERSION(SFL_MWALLET)

/*
**===========================================================================
**  단말기 상태 초기값
**===========================================================================
*/
/* 음성 통화 - 디버그 화면 상태 */
#define INIT_IS_DEBUG_SCREEN                FALSE
/* 데이터 서비스 - 디버그 화면 상태 */
//#ifdef FEATURE_DS_DEBUG_UI
#define INIT_IS_DATA_DEBUG_SCREEN           FALSE
//#endif /* FEATURE_DS_DEBUG_UI */
/* sleep 상태로 들어 갈 수 있는 상태 */
//#define INIT_IS_OK_TO_SLEEP                 FALSE
/* 전원이 들어왔나? - off 상태를 수행했다는 뜻  */
#define INIT_IS_POWER_ON                    ON
/* 플립이 열리거나, 폴더가 열리거나 */
#define INIT_IS_PHONE_OPEN                  FALSE
/* ear jack이 꼽히거나 뽑히거나 */
#define INIT_IS_EAR_JACK_ON                 OFF
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
#define INIT_IS_CAMERA_ON                   OFF
#endif
/* 송화음 차단 */
#define INIT_IS_MUTE                        FALSE
/* SHUT DOWN 중인가? */
#define INIT_IS_SHUT_DOWN                   FALSE
/* ui task가 offline 상태인가? */
#define INIT_IS_OFFLINE                     FALSE
/* Whenther state machine is accepting keys */
//#define INIT_IS_GETKEY                      FALSE
/* is valid system time? entering DIGITAL service */
#define INIT_IS_VALID_SYSTEM_TIME           FALSE
/* 핸즈 프리 상태 */
#define INIT_IS_HANDS_FREE                  FALSE
/* 충전 중 */
#define INIT_IS_CHARGING                    FALSE
/* 외부 전원 */
#define INIT_IS_EXTERNAL_POWER              FALSE
/* 키패드 테스트 */
#define INIT_KEYPAD_TEST                    FALSE /* diag keypad test */
/* rosa 01-05-03 --> @why, what */
#define INIT_TO_WAP                         FALSE /* Go To WAP state */
#define INIT_FROM_WAP                       FALSE /* Go From WAP state */
#ifdef FEATURE_GVM
/* GVM */
#define INIT_IN_GVM                         FALSE /* GVM state */
/* GVM */
#define INIT_PLAYING_GVM                    FALSE /* GVM Play state */
/* GVM으로 이동상태 */
#define INIT_TO_GVM                         FALSE /* Go To GVM state */
#define INIT_FROM_GVM                       FALSE /* Go From GVM state */
#endif //FEATURE_GVM
#define INIT_IN_ALARM                       FALSE /* Alarm state */
/* <-- */
#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) || defined(FEATURE_WAVELET)
#define INIT_SOCK_MODE              	    FALSE /* fSockMode; */
#define INIT_SOCK_CONNECT           	    FALSE /* fSockConnect; */
#endif	//	#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) ||  defined(FEATURE_WAVELET)

#ifdef	FEATURE_WAVELET	//	khekim 01/02/11
#define INIT_WAVELET_STATE          	    FALSE /* fWavelet state */
#define INIT_FROM_WAVELET                   FALSE /* From Wavelet */
#define INIT_TO_WAVELET                     FALSE /* To Wavelet */
#endif	//	#ifdef	FEATURE_WAVELET
/* Power off mode */
#define INIT_POWER_OFF_MODE                 POM_NONE
/* 자동 잠금*/
#define INIT_AUTO_LOCK                      FALSE
/* SMS 잠금 */
#define INIT_SMS_LOCK                       FALSE
/* 조명 끄지 않는 상태*/
#define INIT_NO_OFF_BACKLIGHT               FALSE
/* fDataManager*/
#define INIT_DATAMANAGER                    FALSE
/* fAirDataManager */   
#define INIT_AIR_DATAMANAGER                FALSE
/* fIPAirDataManager */
#define INIT_IP_AIR_DATAMANAGER             FALSE
/* bDtrDisp */
#define INIT_DATA_CALL                      FALSE
/* 착신 모드 */
#define INIT_DTR_DISP                       0x00
/* 미등록 단말기 */
#define INIT_MAINT_REQ                      FALSE

#ifdef FEATURE_AGILENT_TEST
#define INIT_AGILENT_TEST                   FALSE
#define INIT_LOOP_BACK                      LB_8K
#define INIT_POWER_CONTROL                  TRUE
#endif//#ifdef FEATURE_AGILENT_TEST

#ifdef FEATURE_SKT_NDSS_DS
#define INIT_NDSS_ORG_FAIL                  FALSE
#endif //FEATURE_SKT_NDSS_DS
/* rosa 01-05-03 --> @why, what */
#ifdef FEATURE_SKTT_PM
#define INIT_TO_PM                       FALSE /* Go To PM state */
#endif //FEATURE_SKTT_PM

#ifdef FEATURE_SKVM
#define INIT_IN_SKVM                         FALSE /* State In SKVM  */
#define INIT_TO_SKVM                         FALSE /* Go To SKVM  */
#define INIT_FROM_SKVM                       FALSE /* From To SKVM  */
#define INIT_PUSH_SMS_IN_SKVM				 FALSE /* Push SMS In SKVM */ 	
#endif //FEATURE_SKVM

#ifdef FEATURE_WAP_WAVELET
#define INIT_WAP_TO_WAVELET                  FALSE /* Go To WAVELET */
#define INIT_WAVELET_TO_WAP                  FALSE /* Go To WAVELET */
#endif //FEATURE_WAP_WAVELET
#define INIT_DM_TEMPFILE                    SKY_INVALID_FILE_HANDLE
#define INIT_IGNORE_BACKLIGHT               FALSE
#define INIT_AUTO_ANSWER_FROM_POWER_UP      TRUE
#define INIT_NEW_MESSAGE                    FALSE
#define INIT_IN_GAME                        FALSE
#define INIT_EMAIL_INPROG                   FALSE

#define INIT_SKY_ID_CONTRAST                HCL_D    
#define INIT_SKY_SHORTLOCK                  FALSE  
#define INIT_SKY_SELFINFO_LOCK              FALSE 

#ifdef FEATURE_WAP
#define INIT_WAP_STATE                      FALSE /* ui.wap */
#endif //#ifdef FEATURE_WAP         
       
#define INIT_VIRGIN_SYSTEM                  FALSE

#define INIT_SKY_SKVMCOLORENV               FALSE 
#define INIT_SKY_SKVMSETBG		            FALSE
#define INIT_SKY_SKVMARGC		            0x00
#define	INIT_SKY_SKVMARGV					NULL

/* <-- */


/*
**===========================================================================
**  통화 상태 초기값
**===========================================================================
*/
/* CM_PHONE_INFO_T */
//#define INIT_PHONE_INFO                     { }
/* cm_nam_e_type */
#define INIT_PHONE_INFO_CURRENT_NAME        ""
#define INIT_PHONE_INFO_NAM_SELECT          0
/* CNI */
//#define INIT_CNI                            { INIT_CNI_PHONE_NUMBER, INIT_CNI_NAME, INIT_CNI_PI }
//#define INIT_CNI_PHONE_NUMBER               ""
//#define INIT_CNI_NAME                       ""
//#define INIT_CNI_PI                         0
/* OTASP failed */
#define INIT_IS_OTASP_FAILED                FALSE
/* alert */
#define INIT_IS_ALERT                       FALSE
/* roaming call */
#define INIT_IS_ROAM_CALL                   FALSE
/* digital - CDMA */
#define INIT_IS_DIGITAL                     FALSE
/* originate */
#define INIT_IS_ORIGINATE                   FALSE
/* txdtmf */
#define INIT_IS_TRANSMIT_DTMF               FALSE
/* force service option */
#define INIT_FORCE_SERVICE_OPTION           0
/* dormant mode */
#ifdef FEATURE_UI_DORMANT
#define INIT_DORMANT                        FALSE
#endif

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern EXT_CONFIG_T        g_ExtConfig;    /* 기타 정보 */
extern MISC_CONFIG_T       g_MiscConfig;   /* 부가 기능 정보 */
extern SYSTEM_CONFIG_T     g_SysConfig;    /* 시스템 정보 */
extern APP_CONFIG_T        g_AppConfig;    /* 응용프로그램 정보 */
#ifdef undersea
#ifdef FEATURE_GVM
extern GVM_CONFIG_T        g_GVMConfig;    /* GVM 정보 */
#endif//#ifdef FEATURE_GVM
#endif //undersea
#ifdef FEATURE_MCOMMERCE
extern MW_TEMPCERT_CONFIG_T			g_MWTempCertConfig1;		/* Mobile Wallet 정보 */
extern MW_TEMPCERT_CONFIG_T			g_MWTempCertConfig2;		/* Mobile Wallet 정보 */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig1;		/* Mobile Wallet 정보 */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig2;		/* Mobile Wallet 정보 */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig3;		/* Mobile Wallet 정보 */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig4;		/* Mobile Wallet 정보 */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig5;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig1;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig2;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig3;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig4;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig5;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig6;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig7;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig8;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig9;		/* Mobile Wallet 정보 */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig10;		/* Mobile Wallet 정보 */
extern MW_WALLET_CONFIG_T			g_MWWalletConfig;
#endif /* FEATURE_MCOMMERCE */

#ifdef FEATURE_CHARACTER_BELL
extern CHARACBELL_CONFIG_T          g_CharacbellConfig;    // Characbell 정보 
#endif // FEATURE_CHARACTER_BELL

extern SYSTEM_STATE_T      g_SysState;     /* 시스템 상태 */
extern CALL_STATE_T        g_CallState;    /* 통화 상태 */

#ifdef FEATURE_USE_IRDA
extern UINT8		gIrex_ModelIndex;
#endif

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API SKY_InitSystemState(void);
void SKY_API SKY_InitCallState(void);
void SKY_API SKY_ResetConfig(void);
void SKY_API SKY_InitConfig(void);
//void SKY_API sInitMisc(void);
void SKY_API SKY_InitExtConfig(void);
void SKY_API SKY_DeleteExtConfig(void);
void SKY_API SKY_WriteExtConfig(void);

void SKY_API SKY_InitMiscConfig(void);
void SKY_API SKY_DeleteMiscConfig(void);
void SKY_API SKY_WriteMiscConfig(void);
//void SKY_API sInitSys(void);
void SKY_API SKY_InitSysConfig(void);
void SKY_API SKY_DeleteSysConfig(void);
void SKY_API SKY_WriteSysConfig(void);
//void SKY_API sInitApp(void);
void SKY_API SKY_InitAppConfig(void);
void SKY_API SKY_DeleteAppConfig(void);
void SKY_API SKY_WriteAppConfig(void);
#ifdef FEATURE_GVM
//void SKY_API sInitGVM(void);
void SKY_API SKY_InitGVMConfig(void);
void SKY_API SKY_DeleteGVMConfig(void);
void SKY_API SKY_WriteGVMConfig(void);
#endif //#ifdef FEATURE_GVM
#ifdef FEATURE_MCOMMERCE
extern void SKY_API InitMWallet(int cert);
extern void SKY_API InitMWTCACert(int i);
extern void SKY_API InitMWMyCert(int i);
extern void SKY_API InitMWTempCert(int i);

void SKY_API SKY_InitMWTempCert1Config(void);
void SKY_API SKY_InitMWTempCert2Config(void);
void SKY_API SKY_InitMWMyCert1Config(void);
void SKY_API SKY_InitMWMyCert2Config(void);
void SKY_API SKY_InitMWMyCert3Config(void);
void SKY_API SKY_InitMWMyCert4Config(void);
void SKY_API SKY_InitMWMyCert5Config(void);
void SKY_API SKY_InitMWTCACert1Config(void);
void SKY_API SKY_InitMWTCACert2Config(void);
void SKY_API SKY_InitMWTCACert3Config(void);
void SKY_API SKY_InitMWTCACert4Config(void);
void SKY_API SKY_InitMWTCACert5Config(void);
void SKY_API SKY_InitMWTCACert6Config(void);
void SKY_API SKY_InitMWTCACert7Config(void);
void SKY_API SKY_InitMWTCACert8Config(void);
void SKY_API SKY_InitMWTCACert9Config(void);
void SKY_API SKY_InitMWTCACert10Config(void);
void SKY_API SKY_InitMWalletConfig(void);

void SKY_API SKY_DeleteMWTempCertConfig(int i);
void SKY_API SKY_DeleteMWMyCertConfig(int i);
void SKY_API SKY_DeleteMWTCACertConfig(int i);
void SKY_API SKY_DeleteMWalletConfig(void);

void SKY_API SKY_WriteMWTempCert1Config(void);
void SKY_API SKY_WriteMWTempCert2Config(void);
void SKY_API SKY_WriteMWMyCert1Config(void);
void SKY_API SKY_WriteMWMyCert2Config(void);
void SKY_API SKY_WriteMWMyCert3Config(void);
void SKY_API SKY_WriteMWMyCert4Config(void);
void SKY_API SKY_WriteMWMyCert5Config(void);
void SKY_API SKY_WriteMWTCACert1Config(void);
void SKY_API SKY_WriteMWTCACert2Config(void);
void SKY_API SKY_WriteMWTCACert3Config(void);
void SKY_API SKY_WriteMWTCACert4Config(void);
void SKY_API SKY_WriteMWTCACert5Config(void);
void SKY_API SKY_WriteMWTCACert6Config(void);
void SKY_API SKY_WriteMWTCACert7Config(void);
void SKY_API SKY_WriteMWTCACert8Config(void);
void SKY_API SKY_WriteMWTCACert9Config(void);
void SKY_API SKY_WriteMWTCACert10Config(void);
void SKY_API SKY_WriteMWalletConfig(void);

WORD SKY_GET_MW_TEMPCERT_LENGTH(int i);
BYTE *SKY_GET_MW_TEMPCERT_PIN_HASH(int i);		
BYTE *SKY_GET_MW_TEMPCERT_PRV_KEY(int i);		
BYTE *SKY_GET_MW_TEMPCERT_SKID(int i);
DWORD SKY_GET_MW_TEMPCERT_REQUESTDATE(int i);	    
BYTE *SKY_GET_MW_TEMPCERT_SUBJECT_DN(int i);    
BYTE SKY_GET_MW_TEMPCERT_KEY_USAGE(int i);		
void SKY_SET_MW_TEMPCERT_LENGTH(int i,WORD s);	
void SKY_SET_MW_TEMPCERT_PIN_HASH(int i,BYTE *s,int len);
void SKY_SET_MW_TEMPCERT_PRV_KEY(int i,BYTE *s,int len);
void SKY_SET_MW_TEMPCERT_SKID(int i,BYTE *s,int len);
void SKY_SET_MW_TEMPCERT_REQUESTDATE(int i,DWORD s);
void SKY_SET_MW_TEMPCERT_SUBJECT_DN(int i,BYTE *s,int len);
void SKY_SET_MW_TEMPCERT_KEY_USAGE(int i,BYTE s);
void  SKY_SET_MW_TEMPCERT(int i);									
WORD SKY_GET_MW_MYCERT_LENGTH(int i);		
WORD SKY_GET_MW_MYCERT_CHECKFIELD(int i);		
BYTE *SKY_GET_MW_MYCERT_PIN_HASH(int i);		
BYTE *SKY_GET_MW_MYCERT_PRV_KEY(int i);	
BYTE *SKY_GET_MW_MYCERT_DISPLAY(int i);		
BYTE *SKY_GET_MW_MYCERT_SUBJECT_KID(int i);		
BYTE *SKY_GET_MW_MYCERT_ISSUER_KID(int i);		
BYTE *SKY_GET_MW_MYCERT_CERT_URL(int i);	
BYTE *SKY_GET_MW_MYCERT_SUBJECT_DN(int i);		
void SKY_SET_MW_MYCERT_LENGTH(int i,WORD s);		
void SKY_SET_MW_MYCERT_CHECKFIELD(int i,WORD s);		
void SKY_SET_MW_MYCERT_PIN_HASH(int i,BYTE *s,int len);		
void SKY_SET_MW_MYCERT_PRV_KEY(int i,BYTE *s,int len);		
void SKY_SET_MW_MYCERT_DISPLAY(int i,BYTE *s,int len);		
void SKY_SET_MW_MYCERT_SUBJECT_KID(int i,BYTE *s,int len);		
void SKY_SET_MW_MYCERT_ISSUER_KID(int i,BYTE *s,int len);		
void SKY_SET_MW_MYCERT_CERT_URL(int i,BYTE *s,int len);		
void SKY_SET_MW_MYCERT_SUBJECT_DN(int i,BYTE *s,int len);		
void SKY_SET_MW_MYCERT(int i);						
WORD SKY_GET_MW_TCACERT_LENGTH(int i);	
WORD SKY_GET_MW_TCACERT_CHECKFIELD(int i);	
BYTE *SKY_GET_MW_TCACERT_DISPLAY(int i);	
BYTE *SKY_GET_MW_TCACERT_SUBJECT_KID(int i);	
BYTE *SKY_GET_MW_TCACERT_ISSUER_KID(int i);	
BYTE *SKY_GET_MW_TCACERT_CERT(int i);	
BYTE *SKY_GET_MW_TCACERT_CA_INFO_URL(int i);	
BYTE *SKY_GET_MW_TCACERT_SUBJECT_DN(int i);	
DWORD SKY_GET_MW_TCACERT_NOT_AFTER(int i);	
void SKY_SET_MW_TCACERT_LENGTH(int i,WORD s);		
void SKY_SET_MW_TCACERT_CHECKFIELD(int i,WORD s);		
void SKY_SET_MW_TCACERT_DISPLAY(int i,BYTE *s,int len);		
void SKY_SET_MW_TCACERT_SUBJECT_KID(int i,BYTE *s,int len);		
void SKY_SET_MW_TCACERT_ISSUER_KID(int i,BYTE *s,int len);
void SKY_SET_MW_TCACERT_CERT(int i,BYTE *s,int len);		
void SKY_SET_MW_TCACERT_CA_INFO_URL(int i,BYTE *s,int len);		
void SKY_SET_MW_TCACERT_SUBJECT_DN(int i,BYTE *s,int len);		
void SKY_SET_MW_TCACERT_NOT_AFTER(int i,DWORD s);		
void SKY_SET_MW_TCACERT(int i);									

#endif  /* FEATURE_MCOMMERCE */
#ifdef FEATURE_CHARACTER_BELL
void SKY_API SKY_InitCharacbellConfig(void);
void SKY_API SKY_DeleteCharacbellConfig(void);
void SKY_API SKY_WriteCharacbellConfig(void);
#endif // FEATURE_CHARACTER_BELL

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_CONF_H_ */


