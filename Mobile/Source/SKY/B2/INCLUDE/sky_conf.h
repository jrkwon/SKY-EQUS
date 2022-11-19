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
** SKY�� ȯ�� ���� ���� ��� 
**
** SKY UI������ �� �̻� NV �������� ������� �ʴ´�.
** ��� �����ʹ� EFS�� ���� ���·� ����Ǹ�, �� �׸� ������ �� �ִ� ��ũ�ΰ� �����ȴ�
** ��, �� ���� ���ܰ� �ִµ�, �ٷ� �ٸ� �½�ũ������ ���Ǵ� NV �����ۿ� ���� �� �־�
** �� �ϴ� ����̴�. �� ���� �ٸ� �½�ũ�� �������� �ʾƵ� �ǵ��� ������ NV ������
** ���ٹ���� ����ؾ� �Ѵ�.
** ������ NV ���������� ó���� �־�� �ϴ� �κ��̴�.
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
** SKY ȯ�� ���� 
*/
enum tagSKY_CONFIG_E
{
    SC_EXT,             /* ��Ÿ ���� */
    SC_MISC,            /* �ΰ� ���� */
    SC_SYS,             /* �ܸ��� ���� */
    SC_APP,             /* �������α׷� ���� */
    SC_GVM,
	SC_MW_TEMPCERT1,   /* �������� ���� ���⼭���� */
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
	SC_MW_WALLET,           /* �������� ���� �������..*/
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
** ȯ�� ���� ����
*/
extern SKY_CONFIG_T        g_aSkyConfig[];

#define sSET_CONFIG_DIRTY(SkyConfig) g_aSkyConfig[SkyConfig].fDirty = (TRUE)
#define sIS_CONFIG_DIRTY(SkyConfig)  (g_aSkyConfig[SkyConfig].fDirty)

/* 
** �ý��� �ʱ�ȭ �Ŀ� NV_SPARE_1_I ���� 0xAA�� �����ȴ�.
** �ѹ��̶� ���� �� ���� NV_SPARE_1_I���� 0xFF�� �����Ѵ�.
** <-- �������� �ܸ��� ���¸� Ȯ���ϱ� ���� ���.
** g_fVirginSystem�� �ý��� ���� �ÿ� NV_SPARE_1_I���� �о
** 0xAA�� TRUE�� �ǰ�, �ƴϸ� FALSE�� �����ȴ�.
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

/* ������ �����ü�� ��� */
#define SKY_WRITE_CONFIG(SkyConfig)  \
{ \
    if(sIS_CONFIG_DIRTY(SkyConfig)) \
    { \
        g_aSkyConfig[SkyConfig].pfnWrite(); \
        SKY_MAKE_NO_VIRGIN(); \
    } \
}

/*�� ���� Write ���� */
#define sSET_MEMORY_VALUE(SkyConfig, a, b) \
        { \
            (a) = (b); \
	        sSET_CONFIG_DIRTY(SkyConfig); \
        }

/* ���ڿ� ���� Write ���� */
#define sSET_MEMORY_STR(SkyConfig, a, b)  \
        { \
            STRCPY((BYTE*)(a), (BYTE*)(b));\
            sSET_CONFIG_DIRTY(SkyConfig); \
        }                       

/* �޸� ���� Write ���� */
#define sSET_MEMORY_COPY(SkyConfig, a, b, len) \
        { \
            memcpy((BYTE*)(a), (BYTE*)(b), len); \
			sSET_CONFIG_DIRTY(SkyConfig); \
        }
                           
/* �� ���� */
#define sSET_CONFIG_VALUE(SkyConfig, a, b) \
        { \
            (a) = (b); \
	        sSET_CONFIG_DIRTY(SkyConfig); \
            SKY_WRITE_CONFIG(SkyConfig); \
        }

/* ���ڿ� ���� */
#define sSET_CONFIG_STR(SkyConfig, a, b)  \
        { \
            STRCPY((BYTE*)(a), (BYTE*)(b)); \
            sSET_CONFIG_DIRTY(SkyConfig); \
            SKY_WRITE_CONFIG(SkyConfig); \
        }

/* �޸� ���� */
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
**  ��Ÿ ����
**
**
******************************************************************************
******************************************************************************
*/
struct tagEXT_CONFIG_T
{
    SKY_FILE_HEADER_T       Header;             /* ���� ��� */
    BYTE szOpenDataFileName[SKY_PATH_NAME_MAX_LENGTH];
};

typedef struct tagEXT_CONFIG_T  EXT_CONFIG_T;

/*---------------------------------------------------------------------------
** WRITE�� ���� ���� ������ ���� �̸� (��ü ���)
*/
#define SKY_GET_OPEN_DATA_FILE_NAME()           g_ExtConfig.szOpenDataFileName
#define SKY_IS_OPEN_DATA_FILE()                 (SKY_GET_OPEN_DATA_FILE_NAME()[0] != '\0')

/*---------------------------------------------------------------------------
** WRITE�� ���� ���� ������ ���� �̸� (��ü ���)
*/
#define SKY_SET_OPEN_DATA_FILE_NAME(s)          sSET_CONFIG_STR(SC_EXT, SKY_GET_OPEN_DATA_FILE_NAME(), s)
#define SKY_CLEAR_OPEN_DATA_FILE_NAME()         sSET_CONFIG_STR(SC_EXT, SKY_GET_OPEN_DATA_FILE_NAME(), "")



/*---------------------------------------------------------------------------
** 011 �ΰ� ���� ��ȣ ����                        //white 2000.12.19
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
** 017 �ΰ� ���� ��ȣ ����
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
**  ��
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
    WORD    wBellID;                            /* �� ID */
    BYTE    szFileName[MAX_FILE_NAME_LENGTH+1];   /* ������ �� ���� �̸� */
};
typedef struct tagBELL_TYPE_T               BELL_TYPE_T;

#define sGET_BELL_ID(s)            (s).wBellID
#define sGET_BELL_FILE_NAME(s)     (s).szFileName 

/*---------------------------------------------------------------------------
**  ��/���� ����
*/
enum tagBELL_VIBRATION_E
{
    BV_BELL,                    /* �� */
    BV_VIBRATION,               /* ���� */
    BV_BELL_AFTER_VIBRATION,    /* ���� �� �� */
    BV_BOTH_BELL_VIBRATION      /* ������ �� ���� */
};
typedef enum tagBELL_VIBRATION_E            BELL_VIBRATION_E;

/*---------------------------------------------------------------------------
**  �� �︲ �ð� ����
*/
enum tagBELL_TIME_E
{
    BT_CONTINUATION,            /* ��� �︲ */
    BT_5SECOND,                 /* 5�ʰ� �︲ */
    BT_10SECOND,                /* 10�ʰ� �︲ */
    BT_15SECOND                 /* 15�ʰ� �︲ */
};
typedef enum tagBELL_TIME_E                 BELL_TIME_E;

/*===========================================================================
**  ��/����
*/
struct tagBELL_VIBRATION_T
{
    BELL_TYPE_T         Type;           /* �� ���� */
    BELL_VIBRATION_E    BellVibration;  /* ��/���� ���� */
    INT8                nVolume;        /* �� ���� */
    BELL_TIME_E         Time;           /* �� �︲ �ð� ���� */
};
typedef struct tagBELL_VIBRATION_T          BELL_VIBRATION_T;

#define sGET_BELL_TYPE(s)           (s).Type
#define sGET_BELL_VIBRATION(s)      (s).BellVibration
#define sGET_BELL_VOLUME(s)         (s).nVolume
#define sGET_BELL_TIME(s)           (s).Time

/*---------------------------------------------------------------------------
**  �׸�
*/
#define sPICTURE_NOT_USED   0xFFFF  /* �׸��� ������ ���� */
#define sMY_PICTURE         0xFFFE  /* ������ �׸� */
#define sMY_PHOTO           0xFFFD  /* ������ ���� */
#define sMY_PHOTO_ALL       0xFFFC  /* ������ ���� - �����̵� */

#define IS_PICTURE_NOT_USED(w)    ((w) == sPICTURE_NOT_USED)
#define IS_MY_PICTURE(w)    ((w) == sMY_PICTURE)
#define IS_MY_PHOTO(w)      ((w) == sMY_PHOTO)
#define IS_MY_PHOTO_ALL(w)      ((w) == sMY_PHOTO_ALL)

#ifdef FEATURE_CHARACBELL
#define sC_PICTURE          0xFFFB  // ĳ����
#define IS_CHARAC_PICTURE(w)    ((w) == sC_PICTURE)
#endif // FEATURE_CHARACBELL

/* PictureID�� PICTURE_NOT_USED�̸� �ش� ��쿡 �׸��� ������� ����
   ���� �ʵ带 �δ� �ͺ��� �޸𸮸� ������ �� �־... */
struct tagPICTURE_TYPE_T 
{
    WORD    wPictureID;                         /* �׸� ID */
    BYTE    szFileName[MAX_FILE_NAME_LENGTH+1];   /* ������ �׸� ���� �̸� */
};
typedef struct tagPICTURE_TYPE_T            PICTURE_TYPE_T;

#define sGET_PICTURE_TYPE_ID(s)     (s).wPictureID
#define sGET_PICTURE_FILE_NAME(s)   (sIS_MY_PICTURE(sGET_PICTURE_TYPE_ID(s))\
                                    ? (s).szFileName : SKY_ASSERT(FALSE);

#define sSET_PICTURE_TYPE_ID(s, w)  sGET_PICTURE_TYPE_ID(s) = w

/*---------------------------------------------------------------------------
**  �׸� ����
*/
struct tagPICTURE_IN_CASE_T
{
    BOOL            fUse;           /* FALSE --> ���� ��� ���� */
    PICTURE_TYPE_T  Idle;           /* �ø��� ������ ��, ���� ������ �� */
    PICTURE_TYPE_T  IncomingCall;   /* ��ȭ�� ���� �� */
    PICTURE_TYPE_T  PowerOn;        /* ������ �� �� */
    PICTURE_TYPE_T  PowerOff;       /* ������ �� �� */
    PICTURE_TYPE_T  WAPStart;       /* WAP ���� �� */
};
typedef struct tagPICTURE_IN_CASE_T         PICTURE_IN_CASE_T;

#define sIS_PICTURE_USE(s)              (s).fUse
#define sGET_IDLE_PICTURE_TYPE(s)       (s).Idle
#define sGET_INCOMING_PICTURE_TYPE(s)   (s).IncomingCall
#define sGET_POWER_ON_PICTURE_TYPE(s)   (s).PowerOn
#define sGET_POWER_OFF_PICTURE_TYPE(s)  (s).PowerOff
#define sGET_WAP_PICTURE_TYPE(s)        (s).WAPStart

/*---------------------------------------------------------------------------
**  ���ȭ�� ����
*/
enum tagIDLE_SCREEN_E
{
    IS_CLOCK,               /* �ð踸 ���� */
    IS_PICTURE             /* �׸����� */
};
typedef enum tagIDLE_SCREEN_E               IDLE_SCREEN_E;

/* rosa 2001-09-20 --> @why, what */
/*---------------------------------------------------------------------------
**  �ð�ȭ�� ǥ�� ���
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
**  �׸� ����
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
**  �ΰ� ���
**
**
******************************************************************************
******************************************************************************
*/
/*---------------------------------------------------------------------------
**  �ڵ� ���� ����
*/
enum tagAUTO_BACKLIGHT_E
{
    AB_10SECOND,         /* �׻� �ڵ� ���� - Ű ������ 10�� ���� ���� ���� */
    AB_ALWAYS,           /* �׻� �� ���� - ���� ���� ŰƮ�� ���� ���� �� */
    AB_TIME,             /* �ð��� �ڵ� ���� */
    AB_NO                /* ���� ��� ���� */
};
typedef enum tagAUTO_BACKLIGHT_E        AUTO_BACKLIGHT_E;

#define AUTO_BACKLIGHT_TIME             10   /* 10�� */

/*---------------------------------------------------------------------------
**  �ð��� �ڵ� ����
*/
struct tagAUTO_BACKLIGHT_TIME_T 
{
    /* Number of seconds since base date */
    DWORD   dwStartTime;                /* ���� �ð� */
    DWORD   dwEndTime;                  /* ���� �ð� */
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
**  �ڵ� ����
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
**  �ѹ� �÷��� / �� �ѹ� ����
*/
struct tagNUMBER_PLUS_T
{
    BOOL    fDisplay;                              /* ȭ�� ǥ�� */   
    BYTE    szSKTNumber[MAX_DIAL_DIGIT_LENGTH+1];  /* SKT �߰� ��ȣ */
    BYTE    szSTINumber[MAX_DIAL_DIGIT_LENGTH+1];  /* STI �߰� ��ȣ */
    BELL_TYPE_T PlusBell;                          /* ������*/
};
typedef struct tagNUMBER_PLUS_T         NUMBER_PLUS_T;

#define sIS_NUMBER_PLUS_DISPLAY(s)          (s).fDisplay
//#define sGET_NUMBER_PLUS_NUMBER(s)          (s).szNumber
#define sGET_NUMBER_PLUS_NUMBER_SKT(s)      (s).szSKTNumber
#define sGET_NUMBER_PLUS_NUMBER_STI(s)      (s).szSTINumber
#define sGET_NUMBER_PLUS_BELL_ID(s)         (s).PlusBell.wBellID
#define sGET_NUMBER_PLUS_BELL_FILENAME(s)   (s).PlusBell.szFileName
/*============================================================================
**  �ΰ� ��� - CONFIG_T �� ������ ����ü�� file system�� ����.
*/
struct tagMISC_CONFIG_T
{
    SKY_FILE_HEADER_T       Header;             /* ���� ��� */
    BOOL                    fAutoAreaCode;      /* �ڵ� ���� ��ȣ ON/OFF */
    BYTE                    szAreaCode[MAX_AREA_CODE_LENGTH+1]; /* ���� ��ȣ */
    AUTO_BACKLIGHT_E        AutoBacklight;      /* �ڵ� ���� */
    AUTO_BACKLIGHT_TIME_T   AutoBacklightTime;  /* �ð��� �ڵ� ���� */
    AUTO_BACKLIGHT_E        AutoSubBacklight;      /* ID LCD �ڵ� ���� */
    AUTO_BACKLIGHT_TIME_T   AutoSubBacklightTime;  /* ID LCD �ð��� �׻� ���� */
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    ID_LCD_COLOR_SELECT_T   SubLcdColorSelect;
#endif
    BOOL                    fAutoRedial;        /* �ڵ� �� ���̾� */
    AUTO_RECEIPT_T          AutoReceipt;        /* �ڵ� ���� */
    NUMBER_PLUS_T           NumberPlus;         /* �ѹ� �÷���/�� �ѹ� ���� */
    BYTE                    SRVC011[SKY_MAX_NUM_011_SERVICE][SKY_MAX_DIAL_011_SERVICE+1]; /* 011 �ΰ� ����*/
    BYTE                    SRVC017[SKY_MAX_NUM_017_SERVICE][SKY_MAX_DIAL_017_SERVICE+1]; /* 017 �ΰ� ����*/
};
typedef struct tagMISC_CONFIG_T         MISC_CONFIG_T;

/*
**===========================================================================
**  �ܺ� LCD ����� ���� ���� �б�/����
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
**  011�ΰ� ���� ���� �б�
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
**  011�ΰ� ���� ���� ����
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
**  017�ΰ� ���� ���� �б�
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
**  017 �ΰ� ���� ���� ����
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
**  �ΰ� ��� ���� �б�
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
**  �ΰ� ��� ���� ����
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
**   �ܸ��� �⺻ ��� ����
**
**
******************************************************************************
******************************************************************************
*/

/*---------------------------------------------------------------------------
**  �ڵ� ���� �� �︲ �ð�
*/
enum tagAUTO_ANSWER_BELL_E              /* �ڵ� ���� �� �︲ �ð� */
{
    AAB_IMMEDIATE,                      /* ��� �ڵ����� */
    AAB_VIB_ONCE,                       /* ���� 1ȸ */
    AAB_BELL_5SECOND,                   /* �� 5�� */
    AAB_BELL_10SECOND                   /* �� 10�� */
};
typedef enum tagAUTO_ANSWER_BELL_E      AUTO_ANSWER_BELL_E;

/*---------------------------------------------------------------------------
**  �ȳ� ����
*/
enum tagGREETING_MESSAGE_E              /* �ȳ� ���� ���� */
{
    GM_GENERAL,                         /* �Ϲ� - �ý��� �⺻ */
    GM_DRIVING,                         /* ���� �� */
    GM_MEETING,                         /* ȸ�� �� */
    GM_USER                             /* ����� ���� */
};
typedef enum tagGREETING_MESSAGE_E      GREETING_MESSAGE_E;

/*===========================================================================
**  �ڵ� ����
*/
struct tagAUTO_ANSWER_T                 /* �ڵ� ���� */
{
    BOOL                fOn;            /* on/off */
    GREETING_MESSAGE_E  Greeting;       /* �ȳ� ���� */
    AUTO_ANSWER_BELL_E  BellTime;       /* �ڵ� ���� �� �︲ �ð� */
    INT8                nCount;         /* �ڵ� ���� �޽��� ����  */
    INT8                nRing;          /* �ڵ� ���� ������ ���︲ Ƚ��*/
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
**  ��� ���
*/
struct tagPHONE_LOCK_T                  /* ��� ��� */
{
    BOOL fAuto;                         /* �ڵ� ��� */
    BOOL fOverseasCall;                 /* ���� ��ȭ ��� */
    BOOL fPhoneBook;                    /* ��ȭ�� ��� */
    BOOL fMenu;                         /* �޴� ��� */
    BOOL fCallStack;                    /* ���߽� �̷� ��� */
    BOOL fRecordPlay;                   /* ����/��� ��� */
    BOOL fMemo;                         /* �޸� ��� */
    BOOL fWAP;                          /* n.TOP or i-Touch ��� */
    BOOL fSMS;                          /* SMS ��� */
	BOOL fCamera;						/* Camera ��� */
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
**  ��ȭ �ޱ� ��� - ����
*/
enum tagHOW_TO_ANSWER_E
{
    HTA_OPEN,           /* ���� ���� */
    HTA_OPEN_AND_KEY    /* ���� ���� Ű ���� */
};
typedef enum tagHOW_TO_ANSWER_E         HOW_TO_ANSWER_E;

//#else defined FEATURE_SKY_MODEL_TYPE_FLIP

/*---------------------------------------------------------------------------
**  �ø�Ű ���� - �ø�
*/
enum tagFLIP_KEY_E
{
    FK_AUTO_ANSWER,     /* �ڵ� ��������  */
    FK_MEMORY_DIAL      /* �޸� ���̾�� */
};
typedef enum tagFLIP_KEY_E              FLIP_KEY_E;

//#endif /* FEATURE_SKY_MODEL_TYPE */

/*---------------------------------------------------------------------------
**  NAM ���� -- cm.h �� cm_nam_e_type(CM_NAM_E)�� ����Ѵ�.
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
**  WAP ����
*/
struct tagWAP_SETTING_T
{
    BYTE    szProxyURL[MAX_NAME_LENGTH+1];     /* ���Ͻ� �ּ� */
    BYTE    szInitURL[MAX_URL_LENGTH+1];      /* �ʱ� URL */
	BYTE    szInitMAIL[MAX_WAPMAIL_LENGTH+1]; /* �ʱ� WAP mail Server�ּ� */
	WORD	szWapDormanttime;                 /* Wap Dormant Time*/
};
typedef struct tagWAP_SETTING_T             WAP_SETTING_T;

/*===========================================================================
**  SKVM ����
*/
struct tagSKVM_SETTING_T
{
	BOOL    fSKVMColorEnv;        /* skvm ��� Į�� ����*/
	BOOL	fSetJavaAsBackGround; /* �ڹ� �������� ���ȭ������ ���� */	
    BYTE    nArgcInSkvm;          /* ���ȭ������ �ڹ� �������� �����ϱ� ���� xvm_setargs(argc,argv)�� ����ϰԵ� argc */
    BYTE    szArgvInSkvm[MAX_OF_ARGV+1];		  /* ���ȭ������ �ڹ� �������� �����ϱ� ���� xvm_setargs(argc,argv)�� ����ϰԵ� argv */
};

typedef struct tagSKVM_SETTING_T              SKVM_SETTING_T;

#define sGET_WAP_PROXY_URL(s)               (s).szProxyURL
#define sGET_WAP_INIT_URL(s)                (s).szInitURL
#define sGET_WAP_INIT_MAIL(s)               (s).szInitMAIL
#define sGET_WAP_DORMANT_TIME(s)            (s).szWapDormanttime


/*---------------------------------------------------------------------------
**  ����ũ ũ��
*/
enum tagMIC_VOLUME_E 
{
    MV_HIGH,        /* ũ�� */
    MV_MEDIUM,      /* �߰� */
    MV_LOW          /* �۰� */
};
typedef enum tagMIC_VOLUME_E                MIC_VOLUME_E;

/*===========================================================================
**  ����
*/
struct tagVOLUME_T
{
    INT8            nSpeaker;   /* ��ȭ�� ũ�� */
    INT8            nKeyTone;   /* Ű�� ũ�� */
    MIC_VOLUME_E    MicVolume;  /* ����ũ ũ�� */
    INT8            nGameVolume;    /* ���� ���� */
};
typedef struct tagVOLUME_T                  VOLUME_T;

#define sGET_SPEAKER_VOLUME(s)      (s).nSpeaker
#define sGET_KEY_TONE_VOLUME(s)     (s).nKeyTone
#define sGET_MIC_VOLUME(s)          (s).MicVolume
#define sGET_GAME_VOLUME(s)          (s).nGameVolume

/*===========================================================================
**  �溸��
*/
struct tagWARNING_SIGNAL_T
{
    BOOL            fConnect;   /* ��ȭ������ */
    BOOL            f1Minute;   /* 1�� ����� */
    BOOL            fNoService; /* ���� ��Ż �溸 */
    BOOL            fLoBatt;    /* ������ �溸 */
    BOOL            fMsgBeep;   /* �޽���ȿ���� */
};
typedef struct tagWARNING_SIGNAL_T         WARNING_SIGNAL_T;

#define sIS_CONNECT_WARNING_SIGNAL(s)       (s).fConnect
#define sIS_1MINUTE_WARNING_SIGNAL(s)       (s).f1Minute
#define sIS_NOSERVICE_WARNING_SIGNAL(s)     (s).fNoService
#define sIS_LOBATT_WARNING_SIGNAL(s)        (s).fLoBatt
#define sIS_MESSAGE_BEEP_SOUND(s)           (s).fMsgBeep

/*---------------------------------------------------------------------------
**  ���� ���
*/
enum tagRECEIPT_MODE_E
{
    RM_VOICE,       /* ���� ���� */
    RM_MODEM,       /* �� ���� */
    RM_FAX          /* �ѽ� ���� */
};
typedef enum tagRECEIPT_MODE_E              RECEIPT_MODE_E;

/*---------------------------------------------------------------------------
**  ���ͳݼӵ�����
*/
enum tagINTERNET_SPEED_E
{
    IS_MAX,         /* �ִ� ���� */
    IS_NORMAL       /* ���� ���� */
};
typedef enum tagINTERNET_SPEED_E            INTERNET_SPEED_E;

/*---------------------------------------------------------------------------
**  PC�� ��� �ӵ�
*/
enum tagBPS_PC_E
{
    BP_115200,      /* 115,200 bps */
    BP_19200        /* 19,200 bps */
};
typedef enum tagBPS_PC_E                    BPS_PC_E;

/*---------------------------------------------------------------------------
**  AIR ������ �Ŵ���
*/
struct tagAIR_DATA_MANAGER_T
{
    BYTE szPhoneNumber[MAX_DIAL_DIGIT_LENGTH+1];  /* ������ ��ȭ ��ȣ(������) */
    BYTE szIPAddress[MAX_IP_LENGTH+1];            /* ������ IP �ּ� */
};
typedef struct tagAIR_DATA_MANAGER_T        AIR_DATA_MANAGER_T;

#define sGET_AIR_DATA_MANAGER_PHONE_NUMBER(s)   (s).szPhoneNumber
#define sGET_AIR_DATA_MANAGER_IP_ADDRESS(s)     (s).szIPAddress

/*===========================================================================
**  ������ ����
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
**  ���ڸ��� ����
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
**  ȭ�� �׸�(ȭ�� ���, ��Ʈ, ��ũ�ѹ� ũ�� 
*/
struct tagSCREEN_THEME_T {
    /* rosa 2001-09-25 --> @why, what */
    COLOR_SCHEME_E      ColorScheme; //<-- ���߿� ����������.
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
**  ���� ���� - 0,0,0,0
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
**  �ֱ� �߽� ����
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
**  �ܸ��� ȯ�� ���� - CONFIG_T �� ������ ����ü�� file system�� ����.
**		AS TOOL������ ����ϰ� �ֽ��ϴ�. �������� ������ �̱������ ������ �ֽñ� ��Ź�帳�ϴ�.
*/
/* XXXX_CONFIG_T --> being saved to the file system */
struct tagSYSTEM_CONFIG_T
{
    SKY_FILE_HEADER_T   Header;                             /* ���� ��� */
    EQS_COUNTRY         Country;                            /* ���� ���� */
    BOOL                fEtiquette;                         /* ��Ƽ�� */
    BOOL                fLoudSpeaker;                       /* �ܺ� ����Ŀ */
    BYTE                szBanner[MAX_BANNER_LENGTH+1];        /* �޴��� �̸� */
    BYTE                szPassword[MAX_PASSWORD_LENGTH+1];  /* ��ȣ */
    AUTO_ANSWER_T       AutoAnswer;                         /* �ڵ� ���� */
    PHONE_LOCK_T        PhoneLock;                          /* ��� ��� */
//#if (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FOLDER)
    HOW_TO_ANSWER_E     HowToAnswer;                        /* ��ȭ�ޱ� ��� */
//#elif (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FLIP)
    FLIP_KEY_E          FlipKey;                            /* �ø� Ű ���� */
    BYTE                szMemoryDial[MAX_DIAL_DIGIT_LENGTH+1];/* �޸� ���̾��ȣ */
//#endif /* FEATURE_SKY_MODEL_TYPE */
    CM_NAM_E            NAMType;                            /* NAM */
    INT8                nContrast;                          /* ȭ�� ��� */
    WAP_SETTING_T       WapSetting;                         /* WAP ���� */
    BELL_VIBRATION_T    BellVibration;                      /* ��/���� */
    PICTURE_SETTING_T   PictureSetting;                     /* �׸� ���� */
    VOLUME_T            Volume;                             /* ���� */
    WARNING_SIGNAL_T    WarningSignal;                      /* �溸�� */
    DATA_SERVICE_T      DataService;                        /* �����ͼ��� */
    EMAIL_ACCOUNT_T     EMailAccount;                       /* ���ڸ��� ���� */
    SCREEN_THEME_T      ScreenTheme;                        /* ��� */
    VERSION_INFO_T      VersionInfo;                        /* ���� */
    EDITORINFO_T        LastDialInfo;                       /* �ֱ� �߽� ��ȣ */
    DWORD               dwCameraCount;                      /* ī�޶� ī��Ʈ */
    BOOL                fSKYHangul;                         /* �ѱ��Է¹�� */
    BOOL                fIconMenu;                          /* ������ ��� �޴� */
    BOOL                fSmartBattery;                      /* ����Ʈ ���͸� ǥ�� */
    BOOL                f011Service;                        /* 011 �ΰ� ����(�Ҹ�����ÿ���, ������ȯ)*/
    DWORD               dwPMCount;                          /* ������� ī��Ʈ */
    INT8                nIDContrast;                        /* ID LCD ȭ�� ��� */
    BOOL                fShortLock;                         /* #Ű�� �̿��� ��ݱ�� */
    BOOL                fSelfInfoLock;                      /* �ڱ� ���� ��� */
	SKVM_SETTING_T		SkvmSetting;
};
typedef struct tagSYSTEM_CONFIG_T        SYSTEM_CONFIG_T;

/*
**===========================================================================
**  �ý��� ���� �б�
**===========================================================================
*/

/*---------------------------------------------------------------------------
** ���� ���� 
*/
#define SKY_GET_COUNTRY()              g_SysConfig.Country

/*---------------------------------------------------------------------------
** ��Ƽ�� 
*/
#define SKY_IS_ETIQUETTE()             g_SysConfig.fEtiquette

/*---------------------------------------------------------------------------
** ����Ŀ
*/
#define SKY_IS_LOUD_SPEAKER()          g_SysConfig.fLoudSpeaker

/*---------------------------------------------------------------------------
** �޴��� �̸� 
*/
#define SKY_GET_BANNER()               g_SysConfig.szBanner

/*---------------------------------------------------------------------------
** ��ȣ 
*/
#define SKY_GET_PASSWORD()             g_SysConfig.szPassword

/*---------------------------------------------------------------------------
** �ڵ� ���� 
*/
/* ���� */
#define SKY_IS_AUTO_ANSWER()           sIS_AUTO_ANSWER(g_SysConfig.AutoAnswer)
/* �λ縻 */
#define SKY_GET_AUTO_ANSWER_GREETING() sGET_AUTO_ANSWER_GREETING( \
                                                    g_SysConfig.AutoAnswer)
/* �� �︲ �ð� */
#define SKY_GET_AUTO_ANSWER_BELL_TIME() sGET_AUTO_ANSWER_BELL_TIME( \
                                                    g_SysConfig.AutoAnswer)
/* �޽��� ���� */
#define SKY_GET_AUTO_ANSWER_COUNT()     sGET_AUTO_ANSWER_COUNT( \
                                                    g_SysConfig.AutoAnswer)
/* �ڵ����� ���� �� ȸ�� */
#define SKY_GET_AUTO_ANSWER_RINGNUM()   sGET_AUTO_ANSWER_RINGNUM(\
                                                    g_SysConfig.AutoAnswer)

/*---------------------------------------------------------------------------
** �ڵ� ���
*/
/* �ڵ� ��� */
#define SKY_IS_AUTO_LOCK()            sIS_AUTO_LOCK(g_SysConfig.PhoneLock)
/* ���� ��ȭ ��� */
#define SKY_IS_OVERSEAS_CALL_LOCK()   sIS_OVERSEAS_CALL_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* ��ȭ�� ��� */
#define SKY_IS_PHONE_BOOK_LOCK()      sIS_PHONE_BOOK_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* �޴� ��� */
#define SKY_IS_MENU_LOCK()            sIS_MENU_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* ���߽� �̷� ��� */
#define SKY_IS_CALL_STACK_LOCK()      sIS_CALL_STACK_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* ����/��� ��� */
#define SKY_IS_RECORD_PLAY_LOCK()     sIS_RECORD_PLAY_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* �޸� ��� */
#define SKY_IS_MEMO_LOCK()            sIS_MEMO_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* WAP ��� */
#define SKY_IS_WAP_LOCK()             sIS_WAP_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* SMS ��� */
#define SKY_IS_SMS_LOCK()             sIS_SMS_LOCK( \
                                                    g_SysConfig.PhoneLock)
/* CAMERA ��� */
#define SKY_IS_CAMERA_LOCK()          sIS_CAMERA_LOCK( \
                                                    g_SysConfig.PhoneLock)

//#ifdef FEATURE_SKY_MODEL_TYPE_FOLDER
/*---------------------------------------------------------------------------
** ��ȭ �ޱ� ���
*/
#define SKY_GET_HOW_TO_ANSWER()       g_SysConfig.HowToAnswer


/*----------------------------------------------------------------------------
** �ѱ� �Է� ��� ����
*/
#define SKY_GET_SEL_HANGUL()       g_SysConfig.fSKYHangul

/*---------------------------------------------------------------------------
** �޴� ��� ����
*/
#define SKY_GET_SEL_MENU_MODE()       g_SysConfig.fIconMenu 

/*---------------------------------------------------------------------------
** ����Ʈ ���͸� ǥ��
*/
#define SKY_IS_SMART_BATTERY()       g_SysConfig.fSmartBattery

/*---------------------------------------------------------------------------
** 011 �ΰ� ����(�Ҹ��� ��ÿ���, ������ȯ)
*/
#define SKY_IS_011_SERVICE()       g_SysConfig.f011Service

//#elif FEATURE_SKY_MODEL_TYPE_FLIP
/*---------------------------------------------------------------------------
** �ø�Ű ����
*/
#define SKY_GET_FLIP_KEY()            g_SysConfig.FlipKey
#define SKY_GET_MEMORY_DIAL_NUMBER()  g_SysConfig.szMemoryDial

//#endif /* FEATURE_SKY_MODEL_TYPE_XXX */

/*---------------------------------------------------------------------------
** NAM -- NV�� �����ؾ� ��
*/
#define SKY_GET_CURRENT_NAM()         g_SysConfig.NAMType

/*---------------------------------------------------------------------------
** ȭ�� ��� 
*/
#define SKY_GET_CONTRAST()            g_SysConfig.nContrast

/*---------------------------------------------------------------------------
** ȭ�� ��� 
*/
#define SKY_GET_IDCONTRAST()            g_SysConfig.nIDContrast

/*---------------------------------------------------------------------------
** #Ű�� �̿��� ��ݱ�� 
*/
#define SKY_GET_SHORTLOCK()            g_SysConfig.fShortLock

/*---------------------------------------------------------------------------
** �ڱ����� ��ݱ�� 
*/
#define SKY_GET_SELFINFOLOCK()            g_SysConfig.fSelfInfoLock

/*---------------------------------------------------------------------------
** SKVM ���� 
*/
/* SKVM Į�� ������  */
#define SKY_GET_SKVMCOLORENV()              g_SysConfig.SkvmSetting.fSKVMColorEnv
/* SKVM ��� ȭ�� ���� ���� */
#define SKY_GET_JAVAASBG()		            g_SysConfig.SkvmSetting.fSetJavaAsBackGround
/* SKVM argc */
#define SKY_GET_ARGCINSKVM()	            g_SysConfig.SkvmSetting.nArgcInSkvm
/* SKVM argv */
#define SKY_GET_ARGVINSKVM()				g_SysConfig.SkvmSetting.szArgvInSkvm

/*---------------------------------------------------------------------------
** WAP ����
*/
/* ���Ͻ� �ּ� */
#define SKY_GET_WAP_PROXY_URL()       sGET_WAP_PROXY_URL(g_SysConfig.WapSetting)
/* �ʱ� URL */
#define SKY_GET_WAP_INIT_URL()        sGET_WAP_INIT_URL(g_SysConfig.WapSetting)
/* �ʱ� WAP mail Server�ּ� */
#define SKY_GET_WAP_INIT_MAIL()       sGET_WAP_INIT_MAIL(g_SysConfig.WapSetting)
/* WAP Dormant Time*/
#define SKY_GET_WAP_DORMANT_TIME()    sGET_WAP_DORMANT_TIME(g_SysConfig.WapSetting)



/*---------------------------------------------------------------------------
** ��/����
*/
/* �� ���̵� */
#define SKY_GET_BELL_ID()             sGET_BELL_ID(      \
                                            sGET_BELL_TYPE( \
                                                g_SysConfig.BellVibration))
/* �� ���� �̸� */
#define SKY_GET_BELL_FILE_NAME()      sGET_BELL_FILE_NAME(    \
                                            sGET_BELL_TYPE( \
                                                g_SysConfig.BellVibration))
/* ��/���� ���� */
#define SKY_GET_BELL_VIBRATION()      sGET_BELL_VIBRATION( \
                                                g_SysConfig.BellVibration)
/* �� ���� */
#define SKY_GET_BELL_VOLUME()         sGET_BELL_VOLUME( \
                                                g_SysConfig.BellVibration)
/* �� �︲ �ð� */
#define SKY_GET_BELL_TIME()           sGET_BELL_TIME( \
                                                g_SysConfig.BellVibration)

/*---------------------------------------------------------------------------
** �׸� ����
*/
/* ��� ȭ�� ���� */
#define SKY_GET_IDLE_DISPLAY_NAME()   sGET_IDLE_DISPLAY_NAME( \
                                            g_SysConfig.PictureSetting)
#define SKY_GET_IDLE_SCREEN()         sGET_IDLE_SCREEN( \
                                            g_SysConfig.PictureSetting)
/* ��� ȭ�� ǥ�� ��� */
#define SKY_GET_IDLE_DISPLAY_WATCH()    sGET_IDLE_DISPLAY_WATCH( \
                                            g_SysConfig.PictureSetting)
/* �׸� �����. FALSE --> ���� ������  */
#define SKY_IS_PICTURE_USE()          sIS_PICTURE_USE( \
                                            g_SysConfig.PictureSetting)
/* �ø��� ������ ��, ������ ������ �� */
#define SKY_GET_IDLE_PICTURE()        sGET_IDLE_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))
/* ��ȭ�� ���� �� */
#define SKY_GET_INCOMING_PICTURE()    sGET_INCOMING_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))
/* ������ �� �� */
#define SKY_GET_POWER_ON_PICTURE()    sGET_POWER_ON_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))
/* ������ �� �� */
#define SKY_GET_POWER_OFF_PICTURE()   sGET_POWER_OFF_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))
/* WAP ������ �� */
#define SKY_GET_WAP_PICTURE()         sGET_WAP_PICTURE_TYPE( \
                                            sGET_PICTURE_IN_CASE( \
                                            g_SysConfig.PictureSetting))

/*---------------------------------------------------------------------------
** ����
*/
/* ��ȭ�� ũ�� */
#define SKY_GET_SPEAKER_VOLUME()    sGET_SPEAKER_VOLUME(g_SysConfig.Volume)
/* Ű�� ũ�� */
#define SKY_GET_KEY_TONE_VOLUME()   sGET_KEY_TONE_VOLUME(g_SysConfig.Volume)
/* ����ũ ũ�� */
#define SKY_GET_MIC_VOLUME()        sGET_MIC_VOLUME(g_SysConfig.Volume)
/* ���� ���� */
#define SKY_GET_GAME_VOLUME()        sGET_GAME_VOLUME(g_SysConfig.Volume)

/*---------------------------------------------------------------------------
** �溸��
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
** ������ ����
*/
/* ���� ��� */
#define SKY_GET_DATA_SERVICE_RECEIPT_MODE()   sGET_DATA_SERVICE_RECEIPT_MODE(g_SysConfig.DataService)
/* ���ͳ� �ӵ� */
#define SKY_GET_DATA_SERVICE_INTERNET_SPEED() sGET_DATA_SERVICE_INTERNET_SPEED(g_SysConfig.DataService)
/* PC�� ��� �ӵ� */
#define SKY_GET_DATA_SERVICE_PBS_PC()      sGET_DATA_SERVICE_BPS_PC(g_SysConfig.DataService)
/* AIR ������ �Ŵ��� */
#define SKY_GET_AIR_DATA_MANAGER_PHONE_NUMBER() \
                                      sGET_AIR_DATA_MANAGER_PHONE_NUMBER(g_SysConfig.DataService)
#define SKY_GET_AIR_DATA_MANAGER_IP_ADDRESS()   \
                                      sGET_AIR_DATA_MANAGER_IP_ADDRESS(g_SysConfig.DataService)

/*---------------------------------------------------------------------------
** ���ڸ��� ����
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
** ���
*/
#define SKY_GET_COLOR_SCHEME()      sGET_COLOR_SCHEME(g_SysConfig.ScreenTheme)
/* rosa 2001-09-25 --> @why, what */
//#define SKY_GET_COLOR_SCHEME_BACK() sGET_COLOR_SCHEME_BACK(g_SysConfig.ScreenTheme)
//#define SKY_GET_COLOR_SCHEME_FORE() sGET_COLOR_SCHEME_FORE(g_SysConfig.ScreenTheme)
/* <-- */
#define SKY_GET_FONT_TYPE()         sGET_FONT_TYPE(g_SysConfig.ScreenTheme)
#define SKY_GET_SCROLLBAR_SIZE()    sGET_SCROLLBAR_SIZE(g_SysConfig.ScreenTheme)

/*---------------------------------------------------------------------------
** ���� ����
*/
#define SKY_GET_DMSS_VERSION_INFO()     sGET_DMSS_VERSION_INFO(g_SysConfig.VersionInfo)
#define SKY_GET_EQUS_VERSION_INFO()     sGET_EQUS_VERSION_INFO(g_SysConfig.VersionInfo)
#define SKY_GET_SKY_VERSION_INFO()      sGET_SKY_VERSION_INFO(g_SysConfig.VersionInfo)

/*---------------------------------------------------------------------------
** �ֱ� �߽� ��ȣ
*/
#define SKY_GET_LASTDIAL_INFO_BUFF()      sGET_LASTDIALINFO_BUFF(g_SysConfig.LastDialInfo)
#define SKY_GET_LASTDIAL_INFO_SECRET()    sGET_LASTDIALINFO_SECRET(g_SysConfig.LastDialInfo)
#define SKY_GET_LASTDIAL_INFO_POS()       sGET_LASTDIALINFO_POS(g_SysConfig.LastDialInfo)

/*---------------------------------------------------------------------------
** ī�޶� ī��Ʈ
*/
#define SKY_GET_CAMERA_COUNT()              g_SysConfig.dwCameraCount

/*---------------------------------------------------------------------------
** ������� ī��Ʈ
*/
#define SKY_GET_PM_COUNT()              g_SysConfig.dwPMCount
/*
**===========================================================================
**  �ý��� ���� ���� 
**===========================================================================
*/

/*---------------------------------------------------------------------------
** ���� ���� 
*/
#define SKY_SET_COUNTRY(c)          sSET_CONFIG_VALUE(SC_SYS, SKY_GET_COUNTRY(), c)

/*---------------------------------------------------------------------------
** ��Ƽ�� 
*/
#define SKY_SET_ETIQUETTE(f)        sSET_CONFIG_VALUE(SC_SYS, SKY_IS_ETIQUETTE(), f)

/*---------------------------------------------------------------------------
** ����Ŀ
*/
#define SKY_SET_LOUD_SPEAKER(f)     sSET_CONFIG_VALUE(SC_SYS, SKY_IS_LOUD_SPEAKER(), f)

/*---------------------------------------------------------------------------
** �޴��� �̸� 
*/
#define SKY_SET_BANNER(s)           sSET_CONFIG_STR(SC_SYS, SKY_GET_BANNER(), s)

/*---------------------------------------------------------------------------
** ��ȣ 
*/
#define SKY_SET_PASSWORD(s)         sSET_CONFIG_STR(SC_SYS, SKY_GET_PASSWORD(), s)

/*---------------------------------------------------------------------------
** �ڵ� ���� 
*/
/* ���� */
#define SKY_SET_AUTO_ANSWER(f)      sSET_CONFIG_VALUE(SC_SYS, SKY_IS_AUTO_ANSWER(), f)
/* �λ縻 */
#define SKY_SET_AUTO_ANSWER_GREETING(g)  sSET_CONFIG_VALUE(SC_SYS, SKY_GET_AUTO_ANSWER_GREETING(), g)
/* �� �︲ �ð� */
#define SKY_SET_AUTO_ANSWER_BELL_TIME(t)  sSET_CONFIG_VALUE(SC_SYS, SKY_GET_AUTO_ANSWER_BELL_TIME(), t)
/* �޽��� ���� */
#define SKY_SET_AUTO_ANSWER_COUNT(n)    sSET_CONFIG_VALUE(SC_SYS, SKY_GET_AUTO_ANSWER_COUNT(), n)
/* �� ȸ��     */
#define SKY_SET_AUTO_ANSWER_RINGNUM(n)  sSET_CONFIG_VALUE(SC_SYS, SKY_GET_AUTO_ANSWER_RINGNUM(), n)

/*---------------------------------------------------------------------------
** �ڵ� ���
*/
/* �ڵ� ��� */
#define SKY_SET_AUTO_LOCK(f)          sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_IS_AUTO_LOCK(), f)
/* ���� ��ȭ ��� */
#define SKY_SET_OVERSEAS_CALL_LOCK(f) sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_IS_OVERSEAS_CALL_LOCK(), f)
/* ��ȭ�� ��� */
#define SKY_SET_PHONE_BOOK_LOCK(f)    sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_PHONE_BOOK_LOCK(), f)
/* �޴� ��� */
#define SKY_SET_MENU_LOCK(f)          sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_MENU_LOCK(), f)
/* ���߽� �̷� ��� */
#define SKY_SET_CALL_STACK_LOCK(f)    sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_CALL_STACK_LOCK(), f)
/* ����/��� ��� */
#define SKY_SET_RECORD_PLAY_LOCK(f)    sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_RECORD_PLAY_LOCK(), f)
/* �޸� ��� */
#define SKY_SET_MEMO_LOCK(f)           sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_MEMO_LOCK(), f)
/* WAP ��� */
#define SKY_SET_WAP_LOCK(f)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_WAP_LOCK(), f)
/* SMS ��� */
#define SKY_SET_SMS_LOCK(f)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_SMS_LOCK(), f)
/* CAMERA ��� */
#define SKY_SET_CAMERA_LOCK(f)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_CAMERA_LOCK(), f)

//#ifdef FEATURE_SKY_MODEL_TYPE_FOLDER
/*---------------------------------------------------------------------------
** ��ȭ �ޱ� ���
*/
#define SKY_SET_HOW_TO_ANSWER(h)       sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_HOW_TO_ANSWER(), h)

/*----------------------------------------------------------------------------
** �ѱ� �Է� ��� ����
*/
#define SKY_SET_SEL_HANGUL(h)       sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_SEL_HANGUL(), h)
/*----------------------------------------------------------------------------
** �޴� ��� ����
*/
#define SKY_SET_SEL_MENU_MODE(h)       sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_SEL_MENU_MODE(), h)

/*----------------------------------------------------------------------------
** ����Ʈ ���͸�
*/
#define SKY_SET_SMART_BATTERY(h)       sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_SMART_BATTERY(), h)

/*----------------------------------------------------------------------------
** 011 �ΰ� ����
*/
#define SKY_SET_011_SERVICE(h)        sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_IS_011_SERVICE(), h)

//#elif FEATURE_SKY_MODEL_TYPE_FLIP
/*---------------------------------------------------------------------------
** �ø�Ű ����
*/
#define SKY_SET_FLIP_KEY(k)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_FLIP_KEY(), k)
#define SKY_SET_MEMORY_DIAL_NUMBER(n)  sSET_CONFIG_STR(SC_SYS, \
                                               SKY_GET_MEORY_DIAL_NUMBER, n)
                                               
//#endif /* FEATURE_SKY_MODEL_TYPE_XXX */

/*---------------------------------------------------------------------------
** NAM -- NV�� �����ؾ� ��
*/
#define SKY_SET_CURRENT_NAM(n)         sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_CURRENT_NAM(), n)
/*---------------------------------------------------------------------------
** ȭ�� ��� 
*/
#define SKY_SET_CONTRAST(n)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_CONTRAST(), n)

/*---------------------------------------------------------------------------
** ȭ�� ��� 
*/
#define SKY_SET_IDCONTRAST(n)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_IDCONTRAST(), n)
/*---------------------------------------------------------------------------
** ȭ�� ��� 
*/
#define SKY_SET_SHORTLOCK(n)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_SHORTLOCK(), n)

/*---------------------------------------------------------------------------
** �ڱ����� ���
*/
#define SKY_SET_SELFINFOLOCK(n)            sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_SELFINFOLOCK(), n)
/*---------------------------------------------------------------------------
** SKVM ���� 
*/
/* SKVM Į�� ���� */
#define SKY_SET_SKVMCOLORENV(n)            sSET_CONFIG_VALUE(SC_SYS, \
	                                                  SKY_GET_SKVMCOLORENV(), n)
/* SKVM ��� ȭ�� ���� ���� */
#define SKY_SET_JAVAASBG(n)		           sSET_CONFIG_VALUE(SC_SYS, \
	                                                  SKY_GET_JAVAASBG(), n)
/* SKVM argc */
#define SKY_SET_ARGCINSKVM(n)	           sSET_CONFIG_VALUE(SC_SYS, \
	                                                  SKY_GET_ARGCINSKVM(), n)
/* SKVM argv */
#define SKY_SET_ARGVINSKVM(s,l)			   sSET_CONFIG_COPY(SC_SYS, \
	                                                  SKY_GET_ARGVINSKVM(), s ,l)

/*---------------------------------------------------------------------------
** WAP ����
*/
/* ���Ͻ� �ּ� */
#define SKY_SET_WAP_PROXY_URL(s)       sSET_CONFIG_STR(SC_SYS, \
                                                    SKY_GET_WAP_PROXY_URL(), s)
/* �ʱ� URL */
#define SKY_SET_WAP_INIT_URL(s)        sSET_CONFIG_STR(SC_SYS, \
                                                    SKY_GET_WAP_INIT_URL(), s)
/* �ʱ� WAP mail Server�ּ� */
#define SKY_SET_WAP_INIT_MAIL(s)       sSET_CONFIG_STR(SC_SYS, \
                                                    SKY_GET_WAP_INIT_MAIL(), s)
/* WAP Dormant Time */
#define SKY_SET_WAP_DORMANT_TIME(n)    sSET_CONFIG_VALUE(SC_SYS, \
                                                    SKY_GET_WAP_DORMANT_TIME(), n)


/*---------------------------------------------------------------------------
** ��/����
*/
/* �� ���̵� */
#define SKY_SET_BELL_ID(n)         sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_BELL_ID(), n)
/* �� ���� �̸� */
#define SKY_SET_BELL_FILE_NAME(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_BELL_FILE_NAME(), s)
/* ��/���� ���� */
#define SKY_SET_BELL_VIBRATION(b)       sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_BELL_VIBRATION(), b)
/* �� ���� */
#define SKY_SET_BELL_VOLUME(n)          sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_BELL_VOLUME(), n)
/* �� �︲ �ð� */
#define SKY_SET_BELL_TIME(t)            sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_BELL_TIME(), t)

/*---------------------------------------------------------------------------
** �׸� ����
*/
/* ��� ȭ�� ���� */
#define SKY_SET_IDLE_DISPLAY_NAME(f)   sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_IDLE_DISPLAY_NAME(), f)

#define SKY_SET_IDLE_SCREEN(n)         sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_IDLE_SCREEN(), n)
/* ��� ȭ�� ǥ�� ��� */
#define SKY_SET_IDLE_DISPLAY_WATCH(i)    sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_IDLE_DISPLAY_WATCH(), i)
/* �׸� �����. FALSE --> ���� ������  */
#define SKY_SET_PICTURE_USE(f)         sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_IS_PICTURE_USE(), f)
/* �ø��� ������ ��, ������ ������ �� */
#define SKY_SET_IDLE_PICTURE(p)        sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_IDLE_PICTURE(), p)
/* ��ȭ�� ���� �� */
#define SKY_SET_INCOMING_PICTURE(p)    sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_INCOMING_PICTURE(), p)
/* ������ �� �� */
#define SKY_SET_POWER_ON_PICTURE(p)    sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_POWER_ON_PICTURE(), p)
/* ������ �� �� */
#define SKY_SET_POWER_OFF_PICTURE(p)   sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_POWER_OFF_PICTURE(), p)
/* WAP ������ �� */
#define SKY_SET_WAP_PICTURE(p)         sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_WAP_PICTURE(), p)

/*---------------------------------------------------------------------------
** ����
*/
/* ��ȭ�� ũ�� */
#define SKY_SET_SPEAKER_VOLUME(n)      sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_SPEAKER_VOLUME(), n)
/* Ű�� ũ�� */
#define SKY_SET_KEY_TONE_VOLUME(n)     sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_KEY_TONE_VOLUME(), n)
/* ����ũ ũ�� */
#define SKY_SET_MIC_VOLUME(m)          sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_MIC_VOLUME(), m)
/* ���� ���� */
#define SKY_SET_GAME_VOLUME(m)          sSET_CONFIG_VALUE(SC_SYS, \
                                            SKY_GET_GAME_VOLUME(), m)

/*---------------------------------------------------------------------------
** �溸��
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
** ������ ����
*/
/* ���� ��� */
#define SKY_SET_DATA_SERVICE_RECEIPT_MODE(r)  \
                                    sSET_CONFIG_VALUE(SC_SYS, \
                                        SKY_GET_DATA_SERVICE_RECEIPT_MODE(), r)
/* ���ͳ� �ӵ� */
#define SKY_SET_DATA_SERVICE_INTERNET_SPEED(i)  \
                                    sSET_CONFIG_VALUE(SC_SYS, \
                                      SKY_GET_DATA_SERVICE_INTERNET_SPEED(), i)
/* PC�� ��� �ӵ� */
#define SKY_SET_DATA_SERVICE_PBS_PC(b)      sSET_CONFIG_VALUE(SC_SYS, \
                                              SKY_GET_DATA_SERVICE_PBS_PC(), b)
/* AIR ������ �Ŵ��� */
#define SKY_SET_AIR_DATA_MANAGER_PHONE_NUMBER(s)  \
                                 sSET_CONFIG_STR(SC_SYS, \
                                    SKY_GET_AIR_DATA_MANAGER_PHONE_NUMBER(), s)
#define SKY_SET_AIR_DATA_MANAGER_IP_ADDRESS(s)   \
                                 sSET_CONFIG_STR(SC_SYS, \
                                      SKY_GET_AIR_DATA_MANAGER_IP_ADDRESS(), s)

/*---------------------------------------------------------------------------
** ���ڸ��� ����
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
** ���
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
** ���� ����
*/
#define SKY_SET_DMSS_VERSION_INFO(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_DMSS_VERSION_INFO(), s)
#define SKY_SET_EQUS_VERSION_INFO(s)       sSET_CONFIG_STR(SC_SYS, \
                                                SKY_GET_EQUS_VERSION_INFO(), s)
#define SKY_SET_SKY_VERSION_INFO(s)        sSET_CONFIG_STR(SC_SYS, \
                                                 SKY_GET_SKY_VERSION_INFO(), s)


/*---------------------------------------------------------------------------
** �ֱ� �߽� ����
*/
#define SKY_SET_LASTDIALINFO_BUFF(t, l)       sSET_CONFIG_COPY(SC_SYS, \
                                                SKY_GET_LASTDIAL_INFO_BUFF(), t, l)


#define SKY_SET_LASTDIALINFO_SECRET(s)     sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_LASTDIAL_INFO_SECRET(), s)

#define SKY_SET_LASTDIALINFO_POS(s)        sSET_CONFIG_VALUE(SC_SYS, \
                                                SKY_GET_LASTDIAL_INFO_POS(), s)
/*---------------------------------------------------------------------------
** ī�޶� ī��Ʈ
*/
#define SKY_SET_CAMERA_COUNT(n)              sSET_CONFIG_VALUE(SC_SYS, \
                                    SKY_GET_CAMERA_COUNT(), n)
#define SKY_ADD_CAMERA_COUNT()              sSET_CONFIG_VALUE(SC_SYS, \
                                    SKY_GET_CAMERA_COUNT(), SKY_GET_CAMERA_COUNT()+1)

/*---------------------------------------------------------------------------
** ������� ī��Ʈ
*/
#define SKY_SET_PM_COUNT()              sSET_CONFIG_VALUE(SC_SYS, \
                                    SKY_GET_PM_COUNT(), SKY_GET_PM_COUNT()+1)

#define SKY_INIT_PM_COUNT(n)             sSET_CONFIG_VALUE(SC_SYS, \
                                    SKY_GET_PM_COUNT(), n)


/* rosa 01-01-26 --> @why, what */

/*===========================================================================
**  �������α׷� ���� - �˶�
*/
/* �˶� ���� */
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
**  �������α׷� ���� - �˶�
**  �˶��� �ִ� 10������ ����.
*/
/* �˶� �ð�/�˶� �� ����/�˶� �� ũ�� */

/* �˶� �ð� */
enum tagALARM_TIME_TYPE_E
{
    ATT_NONE,                        /* �ʱⰪ */
	ATT_AFTER_10M,                   /* 10���� */
	ATT_AFTER_20M,                   /* 20���� */
	ATT_AFTER_30M,                   /* 30���� */
	ATT_AFTER_1H,                    /* 1�ð��� */
	ATT_SET_ANYTIME,                 /* �ð����� */
	ATT_EVERY_DAY,                   /* ���� */
    ATT_EVERY_WEEK,                  /* ���� */
    ATT_EVERY_MONTH                  /* �Ŵ� */
};
typedef enum tagALARM_TIME_TYPE_E        ALARM_TIME_TYPE_E;

#define MAX_ALARM_TITLE             18
#define MAX_ALARM_COUNT             10

struct tagALARM_TIME_T
{
    ALARM_TIME_TYPE_E   Type;  /* �˶� �ð� ���� */
    DWORD               dwTime;         /* �˶� �ð� ���� */
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
    MT_NONE,                        /* �ʱⰪ */
    MT_MEMO
};
typedef enum tagMEMO_TYPE_E        MEMO_TYPE_E;

#define MAX_MEMO_DATA_LENGTH       (16*4)
#define MAX_MEMO_COUNT             10

struct tagMEMO_T
{
    MEMO_TYPE_E         Type;       /* �޸� ���� */
    DWORD               dwTime;     /* �޸� �ۼ� �ð� */
    BYTE                szData[MAX_MEMO_DATA_LENGTH+1];
};
typedef struct tagMEMO_T            MEMO_T;

#define sGET_MEMO_TYPE(s)           (s).Type
#define sGET_MEMO_TIME(s)           (s).dwTime
#define sGET_MEMO_DATA(s)           (s).szData

/* <-- */



/*===========================================================================
**  �������α׷� ���� - �޽��� ���� �˸�
*/
/* �˶� ���� */
enum tagSMS_ALERT_TYPE_E
{
    SAT_NO,              /* �Ҹ� �ȳ� */
    SAT_ONCE,            /* 1ȸ �︲ */
    SAT_EVERY_2MINIUTE   /* 2�и��� �︲ */
};
typedef enum tagSMS_ALERT_TYPE_E         SMS_ALERT_TYPE_E;

/* �˶� ���� - ��1~7 */

struct tagSMS_ALERT_T
{
    SMS_ALERT_TYPE_E    AlertType;      /* �˶� ���� */
    WORD                BellID;         /* �˶� �� */
    INT8                nVolume;        /* �˶� ũ�� */
};
typedef struct tagSMS_ALERT_T     SMS_ALERT_T;

#define sGET_SMS_ALERT_TYPE(s)      (s).AlertType
#define sGET_SMS_BELL_ID(s)         (s).BellID
#define sGET_SMS_BELL_VOLUME(s)     (s).nVolume

/*---------------------------------------------------------------------------
**  ��ȭ �ð�
*/
struct tagUSING_TIME_T
{
    DWORD   dwLast;         /* ���� ��� �ð� */
    DWORD   dwCumulation;   /* ���� ��� �ð� */
    DWORD   dwTotal;        /* �� ��ȭ �ð� */
};
typedef struct tagUSING_TIME_T          USING_TIME_T;

#define sGET_LAST_USING_TIME(s)         (s).dwLast
#define sGET_CUMULATION_USING_TIME(s)   (s).dwCumulation
#define sGET_TOTAL_USING_TIME(s)        (s).dwTotal

/*===========================================================================
**  ��ȭ ����
*/
struct tagCALL_TIME_T
{
    USING_TIME_T        VoiceUsingTime;          /*  �����߽Žð�   */
	USING_TIME_T        VoiceReceivedCallTime;   /*  �������Žð�   */
	USING_TIME_T        DataUsingTime;           /*  �����͹߽Žð� */
	USING_TIME_T        DataReceivedCallTime;    /*  ���������Žð� */
    USING_TIME_T        WAPUsingTime;            /*  WAP ��� �ð�  */
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
** ����ð����� ����Ÿ��(DST) ����
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
** ��ġ ���� ����
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
	BYTE	rcvinfo;							// ��ġ�뺸 ���� ���� ���� ����
	BYTE	cfrmid[5];							// ��ġȮ���� ID
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


#define SKY_MAX_FL_FILE_NUM  	 21          /* ����/����� ���� ����  */
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
**  �������α׷� ���� - CONFIG_T �� ������ ����ü�� file system�� ����.
**  --> CALL/WAP ���ø����� �����ϰ�, �޴����� ���� ��û�ϸ� ���� �����ϸ� ��.
*/
struct tagAPP_CONFIG_T
{
    SKY_FILE_HEADER_T   Header;             /* ���� ��� */
/* rosa 00-12-14 --> @why, what */
    ALARM_BELL_T        AlarmBell;          /* �����ٷ�, �˶� */
    BOOL                AlarmHolidayActive[MAX_ALARM_COUNT];
    ALARM_TIME_T        AlarmTime[MAX_ALARM_COUNT]; /* �˶� �ð� ���� */
    MEMO_T              Memo[MAX_MEMO_COUNT];
/* <-- */
    SMS_ALERT_T         SMSAlert;           /* SMS �˶� ����, �˶� ��, �˶� ũ�� */
    CALL_TIME_T         CallTime;           /* ����, WAP ��ȭ �ð� */
    DST_TIME_T          DSTWorldTime[NUM_COUNTRY_NAME];    
                                            /* ����ð��� ����Ÿ�� ���� �ð� */
#ifdef FEATURE_SKTT_EQUS_LOCATION
	LOCATION_INFO_T		LocationInfo;
#endif
	FN_SEQUENCE_T       FnSequence[SKY_MAX_FL_FILE_NUM]; /* ����/��� */
    VMEMOINFO_T         VoiceInfo;    
};
typedef struct tagAPP_CONFIG_T     APP_CONFIG_T;

/*
**===========================================================================
**  �������α׷� ���� �б�
**===========================================================================
*/

/* rosa 01-01-26 --> @why, what */

/* �˶� �� ���� */
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

/* �˶� ���� */
#define SKY_IS_ALARM_HOLIDAY_ACTIVE(i)     g_AppConfig.AlarmHolidayActive[i]
/* �˶� �ð� ���� */
#define SKY_GET_ALARM_TIME_TYPE(i)  sGET_ALARM_TIME_TYPE(g_AppConfig.AlarmTime[i])
#define SKY_IS_ALARM_TIME_ACTIVE(i) sGET_ALARM_TIME_ACTIVE(g_AppConfig.AlarmTime[i])
#define SKY_IS_ALARM_TIME_USED(i)   sGET_ALARM_TIME_USED(g_AppConfig.AlarmTime[i])
#define SKY_GET_ALARM_TIME(i)       sGET_ALARM_TIME(g_AppConfig.AlarmTime[i])
#define SKY_GET_ALARM_TITLE(i)      sGET_ALARM_TITLE(g_AppConfig.AlarmTime[i])

/* �޸� ���� */
#define SKY_GET_MEMO_TYPE(i)        sGET_MEMO_TYPE(g_AppConfig.Memo[i])
#define SKY_GET_MEMO_TIME(i)        sGET_MEMO_TIME(g_AppConfig.Memo[i])
#define SKY_GET_MEMO_DATA(i)        sGET_MEMO_DATA(g_AppConfig.Memo[i])

/* <-- */

/* SMS �˶� ���� */
#define SKY_GET_SMS_ALERT_TYPE()           \
                                sGET_SMS_ALERT_TYPE(g_AppConfig.SMSAlert)
#define SKY_GET_SMS_BELL_ID()      \
                                sGET_SMS_BELL_ID(g_AppConfig.SMSAlert)

#define SKY_GET_SMS_BELL_VOLUME()    \
                                sGET_SMS_BELL_VOLUME(g_AppConfig.SMSAlert)
/* ������ȭ �߽Žð� */
#define SKY_GET_VOICE_LAST_USING_TIME()  \
                        sGET_VOICE_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_VOICE_CUMULATION_USING_TIME() \
                        sGET_VOICE_CUMULATION_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_VOICE_TOTAL_USING_TIME()  \
                        sGET_VOICE_TOTAL_USING_TIME(g_AppConfig.CallTime)

/* ������ȭ ���Žð� */
#define SKY_GET_VOICE_RECEIVEDCALL_LAST_USING_TIME() \
            sGET_VOICE_RECEIVEDCALL_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME() \
	        sGET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME() \
	        sGET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(g_AppConfig.CallTime)

/* ��������� �߽Žð� */
#define SKY_GET_DATA_ORIGINATEDCALL_LAST_USING_TIME() \
	                    sGET_DATA_ORIGINATEDCALL_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME() \
                        sGET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(g_AppConfig.CallTime)	                   
#define SKY_GET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME() \
                        sGET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME(g_AppConfig.CallTime)	   

/* ��������� ���Žð� */
#define SKY_GET_DATA_RECEIVEDCALL_LAST_USING_TIME() \
                        sGET_DATA_RECEIVEDCALL_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME() \
                        sGET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_DATA_RECEIVEDCALL_TOTAL_USING_TIME() \
                        sGET_DATA_RECEIVEDCALL_TOTAL_USING_TIME(g_AppConfig.CallTime)
        
/* WAP ��ȭ�ð� */
#define SKY_GET_WAP_LAST_USING_TIME() \
                        sGET_WAP_LAST_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_WAP_CUMULATION_USING_TIME() \
                        sGET_WAP_CUMULATION_USING_TIME(g_AppConfig.CallTime)
#define SKY_GET_WAP_TOTAL_USING_TIME() \
                        sGET_WAP_TOTAL_USING_TIME(g_AppConfig.CallTime)

/* ����ð� ����Ÿ�� DST ���� */
#define SKY_GET_DST_ON(i)           sGET_DST_ON(g_AppConfig.DSTWorldTime[i])  
#define SKY_GET_DST_START_MON(i)    sGET_DST_START_MON(g_AppConfig.DSTWorldTime[i])     
#define SKY_GET_DST_START_DAY(i)    sGET_DST_START_DAY(g_AppConfig.DSTWorldTime[i])       
#define SKY_GET_DST_END_MON(i)      sGET_DST_END_MON(g_AppConfig.DSTWorldTime[i])       
#define SKY_GET_DST_END_DAY(i)      sGET_DST_END_DAY(g_AppConfig.DSTWorldTime[i])       

/* ��ġ ���� ���� */
#ifdef FEATURE_SKTT_EQUS_LOCATION
#define SKY_GET_LOCATION_INFO()		g_AppConfig.LocationInfo
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

/* ����/��� */
#define SKY_GET_FN_SEQUENCE(i)      g_AppConfig.FnSequence[i]
#define SKY_GET_VOICE_INFO()        (&((g_AppConfig).VoiceInfo))



/*
**===========================================================================
**  �������α׷� ���� ���
**===========================================================================
*/
/* rosa 01-01-26 --> @why, what */

/* �˶� �� ���� */
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

/* �˶� ���� */
#define SKY_SET_ALARM_HOLIDAY_ACTIVE(i,f) sSET_MEMORY_VALUE(SC_APP,SKY_IS_ALARM_HOLIDAY_ACTIVE(i), f)

/* �˶� �ð� ���� */
#define SKY_SET_ALARM_TIME_TYPE(i, t)   sSET_MEMORY_VALUE(SC_APP, SKY_GET_ALARM_TIME_TYPE(i), t)
#define SKY_SET_ALARM_TIME_ACTIVE(i, f) sSET_MEMORY_VALUE(SC_APP, SKY_IS_ALARM_TIME_ACTIVE(i), f)
#define SKY_SET_ALARM_TIME_USED(i, f)   sSET_MEMORY_VALUE(SC_APP, SKY_IS_ALARM_TIME_USED(i), f)
#define SKY_SET_ALARM_TIME(i,t)         sSET_MEMORY_VALUE(SC_APP, SKY_GET_ALARM_TIME(i), t)
#define SKY_SET_ALARM_TITLE(i, sz)      sSET_MEMORY_STR(SC_APP, SKY_GET_ALARM_TITLE(i), sz)

/* �޸� ���� */
#define SKY_SET_MEMO_TYPE(i, t)         sSET_CONFIG_VALUE(SC_APP, SKY_GET_MEMO_TYPE(i), t)
#define SKY_SET_MEMO_TIME(i,t)          sSET_CONFIG_VALUE(SC_APP, SKY_GET_MEMO_TIME(i), t)
#define SKY_SET_MEMO_DATA(i, sz)        sSET_CONFIG_STR(SC_APP, SKY_GET_MEMO_DATA(i), sz)

/* <-- */

/* SMS �˶� ���� */
#define SKY_SET_SMS_ALERT_TYPE(a)           \
                    sSET_CONFIG_VALUE(SC_APP, SKY_GET_SMS_ALERT_TYPE(), a) 
#define SKY_SET_SMS_ALERT_BELL_ID(b)      \
                    sSET_CONFIG_VALUE(SC_APP, SKY_GET_SMS_BELL_ID(), b) 
#define SKY_SET_SMS_ALERT_BELL_VOLUME(v)    \
                    sSET_CONFIG_VALUE(SC_APP, SKY_GET_SMS_BELL_VOLUME(), v) 

/* ���� ��ȭ �ð�(�߽�) */
#define SKY_SET_VOICE_LAST_USING_TIME(t)  \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_LAST_USING_TIME(), t)
#define SKY_SET_VOICE_CUMULATION_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_CUMULATION_USING_TIME(), t)
#define SKY_SET_VOICE_TOTAL_USING_TIME(t)  \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_TOTAL_USING_TIME(), t)


/* ���� ��ȭ �ð�(����) */
#define SKY_SET_VOICE_RECEIVEDCALL_LAST_USING_TIME(t) \
	        sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_RECEIVEDCALL_LAST_USING_TIME(), t)
#define SKY_SET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(t) \
	        sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(), t)
#define SKY_SET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(t) \
	        sSET_CONFIG_VALUE(SC_APP, SKY_GET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(), t)


/* ������ ��� �ð�(�߽�) */
#define SKY_SET_DATA_ORIGINATEDCALL_LAST_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_ORIGINATEDCALL_LAST_USING_TIME(), t)	 
#define SKY_SET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(), t)
#define SKY_SET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME() , t)


/* ������ ��� �ð�(����) */
#define SKY_SET_DATA_RECEIVEDCALL_LAST_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_RECEIVEDCALL_LAST_USING_TIME(), t)	 
#define SKY_SET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(), t)
#define SKY_SET_DATA_RECEIVEDCALL_TOTAL_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_DATA_RECEIVEDCALL_TOTAL_USING_TIME() , t)	        


/* WAP ��ȭ �ð� */
#define SKY_SET_WAP_LAST_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_WAP_LAST_USING_TIME(), t)
#define SKY_SET_WAP_CUMULATION_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_WAP_CUMULATION_USING_TIME(), t)
#define SKY_SET_WAP_TOTAL_USING_TIME(t) \
            sSET_CONFIG_VALUE(SC_APP, SKY_GET_WAP_TOTAL_USING_TIME(), t)

/* ����ð� ����Ÿ�� DST ���� */
#define SKY_SET_DST_ON(i, t)   sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_ON(i), t) 
#define SKY_SET_DST_START_MON(i, t)    \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_START_MON(i), t) 
#define SKY_SET_DST_START_DAY(i, t)    \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_START_DAY(i), t) 
#define SKY_SET_DST_END_MON(i, t)    \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_END_MON(i), t) 
#define SKY_SET_DST_END_DAY(i, t)    \
                sSET_CONFIG_VALUE(SC_APP, SKY_GET_DST_END_DAY(i), t) 

/* ��ġ ���� ���� */
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
	Mobile Wallet ���� ���� 
******************************************************************************/
#ifdef FEATURE_MCOMMERCE
/*===========================================================================
**  �������α׷� ���� - Mobile Wallet
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
#define	MW_WALLET_DELIVERY_LENGTH			120 //137  //����� ���� �����. 
#define	MW_WALLET_KEYIV_LENGTH				  16				
#define	MW_WALLET_DPERIV_LENGTH				16
#define	MW_WALLET_DCARDIV_LENGTH			16
#define	MW_WALLET_DOCBIV_LENGTH				16
#define MW_WALLET_DDELIV_LENGTH				16

/* ==   TempCert ���� ======================================================*/
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
    SKY_FILE_HEADER_T			Header;			/* ���� ��� */
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

// TempCertInfo ������ ��ġ ����
#define SKY_SET_MW_TEMPCERT_LENGTH(i,s,len)	      sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_LENGTH(i),s,len)
#define SKY_SET_MW_TEMPCERT_PIN_HASH(i,s,len)	      sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_PIN_HASH(i),s,len)
#define SKY_SET_MW_TEMPCERT_PRV_KEY(i,s,len)	       sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_PRV_KEY(i),s,len)
#define SKY_SET_MW_TEMPCERT_SKID(i,s,len)			       sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_SKID(i),s,len)
#define SKY_SET_MW_TEMPCERT_REQUESTDATE(i,s,len)	       sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_REQUESTDATE(i),s,len)
#define SKY_SET_MW_TEMPCERT_SUBJECT_DN(i,s,len)		       sSET_MEMORY_COPY(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_SUBJECT_DN(i),s,len)
#define SKY_SET_MW_TEMPCERT_KEY_USAGE(i,s)		      sSET_MEMORY_VALUE(SC_MW_TEMPCERT1,SKY_GET_MW_TEMPCERT_KEY_USAGE(i),s,len)
#define SKY_SET_MW_TEMPCERT(i)								       SKY_WRITE_CONFIG(SC_MW_TEMPCERT1)
*/
/* ==   MYCert ����  ======================================================*/
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
    SKY_FILE_HEADER_T			Header;			/* ���� ��� */
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

// MyCertInfo ������ ����
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
/* ==   TCACert ����  ======================================================*/

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
    SKY_FILE_HEADER_T			Header;			/* ���� ��� */
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

// TCACertInfo ������ ����
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
/* ==   �������� ����  ======================================================*/

// ������� �������� ������ ���� 
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
    SKY_FILE_HEADER_T   Header;			/* ���� ��� */
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
    SKY_FILE_HEADER_T   Header;			/* ���� ��� */
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
**   GVM ���� ����
**
**
******************************************************************************
*****************************************************************************
*/

/* rosa 01-06-07 --> @why, what */
/*===========================================================================
**  GVM ����
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
**  �������α׷� ���� - GVM
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
    SKY_FILE_HEADER_T   Header;            /* ���� ��� */
	GVM_SYSTEM_TYPE_T	GVMSystem;		   /* GVM ȯ�� ���� */
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

/* GVM ������ Handling */
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


/* GVM ������ Handling */
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
    POM_NONE,       /* power off�� ���� ������� �ʾ��� */
    POM_DOWN,       /* ������ ������ ���� */
    POM_RESET       /* ������ ���� �� �ٽ� �� */
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
**   �ܸ��� ���� ����
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
    BYTE            bDtrDisp;           /* ���Ÿ�� */
    BOOL            fMaintReq;          /* �̵�ϴܸ��� */
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
**  �ܸ��� ���� ���� ���
**===========================================================================
*/
/* ���� ��ȭ - ����� ȭ�� ���� */
#define SKY_IS_DEBUG_SCREEN()             g_SysState.fDebugScreen
/* ������ ���� - ����� ȭ�� ����  data_debug_is_enabled() */
#define SKY_IS_DATA_DEBUG_SCREEN()        g_SysState.fDataDebugScreen
/* sleep ���·� ��� �� �� �ִ� ���� */
//#define SKY_IS_OK_TO_SLEEP()              g_SysState.fOK2Sleep
/* ������ ���Գ�? - off ���¸� �����ߴٴ� ��  */
#define SKY_IS_POWER_ON()                 g_SysState.fPowerOn
/* �ø��� �����ų�, ������ �����ų� */
#define SKY_IS_PHONE_OPEN()               g_SysState.fPhoneOpen
#define SKY_IS_PHONE_CLOSE()              (!g_SysState.fPhoneOpen)
/* ear jack�� �����ų� �����ų� */
#define SKY_IS_EAR_JACK_ON()              g_SysState.fEarJack
#define SKY_IS_EAR_JACK_OFF()             (!g_SysState.fEarJack)

#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
#define SKY_IS_CAMERA_ON()                 g_SysState.fCamera
#define SKY_IS_CAMERA_OFF()                (!g_SysState.fCamera)
#endif

/* ��ȭ�� ���� */
#define SKY_IS_MUTE()                     g_SysState.fMute
/* SHUT DOWN ���ΰ�? */
#define SKY_IS_SHUT_DOWN()                g_SysState.fShutDown
/* ui task�� offline �����ΰ�? */
#define SKY_IS_OFFLINE()                  g_SysState.fOffline
/* Whenther state machine is accepting keys */
//#define SKY_IS_GETKEY()                   g_SysState.fGetKeys
/* is valid system time? entering DIGITAL service */
#define SKY_IS_VALID_SYSTEM_TIME()        g_SysState.fSysTimeValid
/* ���� ���� ���� */
#define SKY_IS_HANDS_FREE()               g_SysState.fHandsFree
/* ���� �� */
#define SKY_IS_CHARGING()                 g_SysState.fCharging
/* �ܺ� ���� */
#define SKY_IS_EXTERNAL_POWER()           g_SysState.fExtPower
/* Diag Key Pad Test */
#define SKY_IS_KEYPAD_TEST()              g_SysState.fDiagKeyTest
/* rosa 01-05-03 --> @why, what */
/* WAP���� �̵� ���� */
#define SKY_IS_TO_WAP()                   g_SysState.fToWAP
#define SKY_IS_FROM_WAP()                 g_SysState.fFromWAP
#ifdef FEATURE_GVM
/* GVM���� */
#define SKY_IS_IN_GVM()                   g_SysState.fGVM
/* GVM �÷��� ���� */
#define SKY_IS_PLAYING_GVM()              g_SysState.fGVMPlaying
/* GVM���� �̵� ���� */
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
/* ������ ���� �� �� ������ off��Ű�� ������, reset ��Ű�� ������ �����Ѵ�. */
#define SKY_GET_POWER_OFF_MODE()           g_SysState.fPowerOffMode
/* �ڵ����*/
#define SKY_GET_AUTO_LOCK_MODE()          g_SysState.fAutoLock
/* SMS ��� ����*/
#define SKY_GET_SMS_LOCK_MODE()             g_SysState.fSMSLock
/* ���� ���� �ʴ� ����*/
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
/* ���Ÿ�� */
#define SKY_GET_DTR_DISP()                  g_SysState.bDtrDisp
/* �̵�� �ܸ��� */
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
**  �ܸ��� ���� ���� ����
**===========================================================================
*/
/* ���� ��ȭ - ����� ȭ�� ���� */
#define SKY_SET_DEBUG_SCREEN(f)             SKY_IS_DEBUG_SCREEN() = f
/* ������ ���� - ����� ȭ�� ���� */
#define SKY_SET_DATA_DEBUG_SCREEN(f)        SKY_IS_DATA_DEBUG_SCREEN() = f
/* sleep ���·� ��� �� �� �ִ� ���� */
//#define SKY_SET_OK_TO_SLEEP(f)              SKY_IS_OK_TO_SLEEP() = f
/* ������ ���Գ�? - off ���¸� �����ߴٴ� ��  */
#define SKY_SET_POWER_ON(f)                 SKY_IS_POWER_ON() = f
/* �ø��� �����ų�, ������ �����ų� */
#define SKY_SET_PHONE_OPEN()                SKY_IS_PHONE_OPEN() = TRUE
#define SKY_SET_PHONE_CLOSE()               SKY_IS_PHONE_OPEN() = FALSE
/* ear jack�� �����ų� �����ų� */
#define SKY_SET_EAR_JACK_ON()               SKY_IS_EAR_JACK_ON() = TRUE
#define SKY_SET_EAR_JACK_OFF()              SKY_IS_EAR_JACK_ON() = FALSE
/* Camera jack �ν�*/
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
#define SKY_SET_CAMERA_ON()                 SKY_IS_CAMERA_ON() = TRUE
#define SKY_SET_CAMERA_OFF()                SKY_IS_CAMERA_ON() = FALSE
#endif
/* ��ȭ�� ���� */
#define SKY_SET_MUTE(f)                     SKY_IS_MUTE() = f
/* SHUT DOWN ���ΰ�? */
#define SKY_SET_SHUT_DOWN(f)                SKY_IS_SHUT_DOWN() = f
/* ui task�� offline �����ΰ�? */
#define SKY_SET_OFFLINE(f)                  SKY_IS_OFFLINE() = f
/* Whenther state machine is accepting keys */
//#define SKY_SET_GETKEY(f)                   SKY_IS_GETKEY() = f
/* is valid system time? entering DIGITAL service */
#define SKY_SET_VALID_SYSTEM_TIME(f)        SKY_IS_VALID_SYSTEM_TIME() = f
/* ���� ���� ���� */
#define SKY_SET_HANDS_FREE(f)               SKY_IS_HANDS_FREE() = f
/* ���� �� */
#define SKY_SET_CHARGING(f)                 SKY_IS_CHARGING() = f
/* �ܺ� ���� */
#define SKY_SET_EXTERNAL_POWER(f)           SKY_IS_EXTERNAL_POWER() = f
/* Diag Key Pad Test */
#define SKY_SET_DIAG_KEYPAD_TEST(f)			SKY_IS_KEYPAD_TEST() = f
/* rosa 01-05-03 --> @why, what */
/* WAP���� �̵� ���� */
#define SKY_SET_TO_WAP(f)			        SKY_IS_TO_WAP() = f
#define SKY_SET_FROM_WAP(f)			        SKY_IS_FROM_WAP() = f
#ifdef FEATURE_GVM
/* GVM���� */
#define SKY_SET_IN_GVM(f)			        SKY_IS_IN_GVM() = f
/* GVM �÷��� ���� */
#define SKY_SET_PLAYING_GVM(f)		        SKY_IS_PLAYING_GVM() = f
/* GVM���� �̵� ���� */
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
/* ������ ���� �� �� ������ off��Ű�� ������, reset ��Ű�� ������ �����Ѵ�. */
#define SKY_SET_POWER_OFF_MODE(m)           SKY_GET_POWER_OFF_MODE() = m
/* �ڵ����*/
#define SKY_SET_AUTO_LOCK_MODE(f)               SKY_GET_AUTO_LOCK_MODE() = f
/* SMS ��� */
#define SKY_SET_SMS_LOCK_MODE(f)                SKY_GET_SMS_LOCK_MODE() = f
/* ���� ���� �ʴ� ����*/
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
/* ���Ÿ�� */
#define SKY_SET_DTR_DISP(m)                      SKY_GET_DTR_DISP() = m
/* �̵�� �ܸ��� */
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
**   ��ȭ ���� ����
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
**  ��ȭ ���� ����
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
**  ��ȭ ���� ���� ���
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
**  ��ȭ ���� ���� ����
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
**  �ʱⰪ ����
**
**
******************************************************************************
******************************************************************************
*/

/*----------------------------------------------------------------------------
**  ���� ����
**  --------
**  ���ź�                      BELL
**  ��ȭ��                      SPEAKER
**  Ű��                        KEY_TONE
**  ����ũ                      MIC
**  SMS �˶�                    SMS_BELL
**  �˶�(��ī�������� ����)       ALARM_BELL
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
**  ��Ÿ ���� 
**===========================================================================
*/
/* ���� ���� */
#define INIT_EXT_MAJOR_VERSION          SKY_GET_FILE_MAJOR_VERSION(SFL_EXT)
#define INIT_EXT_MINOR_VERSION          SKY_GET_FILE_MINOR_VERSION(SFL_EXT)
/* ���� WRITE�� ���� ���� ������ ���� �̸� */
#define INIT_OPEN_DATA_FILE_NAME        ""

/*
**===========================================================================
**  �ΰ� ���
**===========================================================================
*/
/* ���� ���� */
#define INIT_MISC_MAJOR_VERSION         SKY_GET_FILE_MAJOR_VERSION(SFL_MISC)
#define INIT_MISC_MINOR_VERSION         SKY_GET_FILE_MINOR_VERSION(SFL_MISC)
/* �ڵ� ���� ��ȣ */
#define INIT_IS_AUTO_AREA_CODE          FALSE
#define INIT_AUTO_AREA_CODE             ""
/* �ڵ� ���� �ʱⰪ */
#define INIT_AUTO_BACKLIGHT             AB_10SECOND /* �׻� �ڵ� ���� */
/* �ð��� �ڵ� ���� - ���� �ð��� ���� �ð� */
#define INIT_AUTO_BACKLIGHT_START_TIME  64800       /* ���� 7�� 00�� */
#define INIT_AUTO_BACKLIGHT_END_TIME    25200       /* ���� 6�� 00�� */
/*�ܺ� LCD ����� ����*/
#define INIT_ID_LCD_IDLE                BLUE_GREEN_LCD_COLOR               
#define INIT_ID_LCD_ALERT               RAINBOW_LCD_COLOR
#define INIT_ID_LCD_SMS                 VIOLET_LCD_COLOR
#define INIT_ID_LCD_ALARM               BLUE_LCD_COLOR //ORANGE_LCD_COLOR              
/* �ڵ� ����̾� */
#define INIT_IS_AUTO_REDIAL             TRUE
/* �ڵ� ���� */
#define INIT_IS_HANDS_FREE_AUTO_RECEIPT FALSE       /* ���� ���� */
#define INIT_IS_EAR_PHONE_AUTO_RECEIPT  FALSE       /* �̾� �� */
/* �ѹ� �÷���/�� �ѹ� ���� */
#define INIT_IS_NUMBER_PLUS_DISPLAY     FALSE       /* ȭ�� ǥ�� ���� */
/* ���� �ĺ� ��ȣ 011-, 017- */
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


/* �����缭�� */
#define INIT_VOICE_MAIL_CONNECT     "*88\0\0"
#define INIT_VOICE_MAIL_SET	        "*82\0\0"
#define INIT_VOICE_MAIL_CANCEL	    "*83\0\0"
/* �ڵ����� */
#define INIT_AUTO_CONNECT_REGISTER  "*72\0\0"
#define INIT_AUTO_CONNECT_CANCEL    "*73\0\0"
/* ������ ��ȯ */
#define INIT_CALL_REDIR_REGISTER    "*71\0\0"
#define INIT_CALL_REDIR_CANCEL      "*73\0\0"
/* ��ȭ�ߴ�� */
#define INIT_CALL_WAITING_REGISTER  "*41\0\0"
#define INIT_CALL_WAITING_CANCEL    "*43\0\0"
/* �Ŀ������� */
#define INIT_POWER_REMOTE           "0172008282\0"
/* �Ŀ���ε� */
#define INIT_POWER_MELODY           "0177005782\0"
/* ȣ��ȯ/���� */
#define INIT_CALL_FORWARD_HOT_STI   "*20\0\0"
#define INIT_CALL_PENDING_HOT_STI   "*40\0\0"


/*
**===========================================================================
**  �ý��� ���
**===========================================================================
*/
/* ���� ���� */
#define INIT_SYS_MAJOR_VERSION          SKY_GET_FILE_MAJOR_VERSION(SFL_SYS)
#define INIT_SYS_MINOR_VERSION          SKY_GET_FILE_MINOR_VERSION(SFL_SYS)
/* -------------------------------------------------------------------------
** ���� ���� 
*/
#define INIT_COUNTRY                    COUNTRY_KOR
/* -------------------------------------------------------------------------
** ��Ƽ�� 
*/
#define INIT_IS_ETIQUETTE               FALSE
/* -------------------------------------------------------------------------
** �ܺ� ����Ŀ
*/
#define INIT_IS_LOUD_SPEAKER            FALSE
/* -------------------------------------------------------------------------
** �޴��� �̸� 
*/
#define INIT_BANNER                     SKY_MODEL_NAME
/* -------------------------------------------------------------------------
** �⺻ ��ȣ 
*/
#define INIT_PASSWORD                   SKY_DEFAULT_PASSWORD
/* -------------------------------------------------------------------------
** �ڵ�����
*/
#define INIT_IS_AUTO_ANSWER             FALSE
/* �ȳ� ���� ���� - �Ϲ� */
#define INIT_AUTO_ANSWER_GREETING       GM_GENERAL          /* �Ϲ� */
/* �� �︲ �ð� */
#define INIT_AUTO_ANSWER_BELL_TIME      AAB_VIB_ONCE        /* ���� 1ȸ */
/* �޽��� ���� */
#define INIT_AUTO_ANSWER_COUNT          0
/* -------------------------------------------------------------------------
** ��� ��� 
*/
/* �ڵ� ��� */
#define INIT_IS_AUTO_LOCK               FALSE
/* ���� ��ȭ ��� */
#define INIT_IS_OVERSEAS_CALL_LOCK      FALSE
/* ��ȭ�� ��� */
#define INIT_IS_PHONE_BOOK_LOCK         FALSE
/* �޴� ��� */
#define INIT_IS_MENU_LOCK               FALSE
/* ���߽� �̷� ��� */
#define INIT_IS_CALL_STACK_LOCK         FALSE
/* ����/��� ��� */
#define INIT_IS_RECORD_PLAY_LOCK        FALSE
/* �޸� ��� */
#define INIT_IS_MEMO_LOCK               FALSE
/* WAP ��� */
#define INIT_IS_WAP_LOCK                FALSE
/* SMS ��� */
#define INIT_IS_SMS_LOCK                FALSE

//#ifdef FEATURE_SKY_MODEL_TYPE_FOLDER
/*---------------------------------------------------------------------------
** ��ȭ �ޱ� ���
*/
#define INIT_HOW_TO_ANSWER              HTA_OPEN

//#elif FEATURE_SKY_MODEL_TYPE_FLIP
/*---------------------------------------------------------------------------
** �ø�Ű ����
*/
#define INIT_FLIP_KEY                   FK_AUTO_ANSWER
#define INIT_MEMORY_DIAL_NUMBER         ""

//#endif /* FEATURE_SKY_MODEL_TYPE_XXX */

/*---------------------------------------------------------------------------
** NAM -- NV�� �����ؾ� ��
*/
#define INIT_NAM                        CM_NAM_1

/*---------------------------------------------------------------------------
** ȭ�� ��� 
*/
#define INIT_CONTRAST                   HCL_D//HS_LCD_CONTRAST_D /* lcddef_xxx.h */
/*---------------------------------------------------------------------------
** WAP ����
*/
/* ���Ͻ� �ּ� */
#define INIT_WAP_PROXY_URL              SKY_WAP_PROXY_URL
/* �ʱ� URL */
#define INIT_WAP_INIT_URL               SKY_WAP_INIT_URL
/* �ʱ� WAP mail Server�ּ� */
#define INIT_WAP_INIT_MAIL              SKY_WAP_INIT_MAIL
/* �ʱ� wap dormant time */
#define INIT_WAP_DORMANT_TIME           SKY_WAP_DORMANT_TIME

/*---------------------------------------------------------------------------
** ��/����
*/
/* �� ���̵� */
#if (MODEL_ID == MODEL_B0)
    #define INIT_BELL_ID                    MUS_JAPAN_TELEPHONE
#elif (MODEL_ID == MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    #define INIT_BELL_ID                    MUS_KURODEN
#endif
/* �� ���� �̸� */
#define INIT_BELL_FILE_NAME             ""
/* ��/���� ���� */
#define INIT_BELL_VIBRATION             BV_BELL
/* �� ���� */
#define INIT_BELL_VOLUME                BELL_VOLUME_LEVEL
/* �� �︲ �ð� */
#define INIT_BELL_TIME                  BT_CONTINUATION

/*---------------------------------------------------------------------------
** �׸� ����
** ���ȭ�� �⺻�� : 
** - �̸�ǥ�� : OK (MODEL NAME)
** - �׸�ǥ�� : OK (SERVICE PROVIDER LOGO - SPEED 011 or POWER 017)
** - �ð����� : ���� �ð�
*/
/* �޴��� �̸� ǥ�� */
#define INIT_IDLE_DISPLAY_NAME          TRUE //FALSE
/* ��� ȭ�� ���� */
#define INIT_IDLE_SCREEN                IS_PICTURE
/* rosa 2001-09-20 --> @why, what */
/* ��� ȭ�� ǥ�� ��� */
#define INIT_IDLE_DISPLAY_WATCH         IDW_SMALL //IDW_BIG
/* <-- */
/* �׸� �����. FALSE --> ���� ������  */
#define INIT_IS_PICTURE_USE             TRUE
/* �ø��� ������ ��, ������ ������ �� */
#define INIT_IDLE_PICTURE               BII_SERVICE_PROVIDER
/* ��ȭ�� ���� �� */
#define INIT_INCOMING_PICTURE           BII_RINGING
/* ������ �� �� */
#define INIT_POWER_ON_PICTURE           BII_WATER_SKY
/* ������ �� �� */
#define INIT_POWER_OFF_PICTURE          BII_GOODBYE
/* WAP ������ �� */
#define INIT_WAP_PICTURE                BII_WAP

/*---------------------------------------------------------------------------
** ����
*/
/* ��ȭ�� ũ�� */
#define INIT_SPEAKER_VOLUME             SPEAKER_VOLUME_LEVEL
/* Ű�� ũ�� */
#define INIT_KEY_TONE_VOLUME            KEY_TONE_VOLUME_LEVEL
/* ����ũ ũ�� */
#define INIT_MIC_VOLUME                 MV_MEDIUM
/* ���� ���� */
#define INIT_GAME_VOLUME                GAME_VOLUME_LEVEL

/*---------------------------------------------------------------------------
** �溸��
*/
#define INIT_IS_CONNECT_WARNING_SIGNAL      TRUE
#define INIT_IS_1MINUTE_WARNING_SIGNAL      FALSE
#define INIT_IS_NOSERVICE_WARNING_SIGNAL    FALSE
#define INIT_IS_LOBATT_WARNING_SIGNAL       TRUE
#define INIT_IS_MESSAGE_BEEP_SOUND          TRUE

/*---------------------------------------------------------------------------
** ������ ����
*/
/* ���� ��� */
#define INIT_DATA_SERVICE_RECEIPT_MODE      RM_VOICE
/* ���ͳ� �ӵ� */
#define INIT_DATA_SERVICE_INTERNET_SPEED    IS_MAX
/* PC�� ��� �ӵ� */
#define INIT_DATA_SERVICE_BPS_PC            BP_115200
/* AIR ������ �Ŵ��� */
#define INIT_AIR_DATA_MANAGER_PHONE_NUMBER  ""
#define INIT_AIR_DATA_MANAGER_IP_ADDRESS    ""

/*---------------------------------------------------------------------------
** ���ڸ��� ����
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
** ���
*/
//#define INIT_COLOR_SCHEME                   INIT_COLOR_SCHEME /* eqs_gui.h */
//#define INIT_SCROLLBAR_SIZE                 INIT_SCROLLBAR_SIZE /* eqs_gui.h */
//#define INIT_FONT_TYPE                      INIT_FONT_TYPE /* eqs_grph.h */

/*---------------------------------------------------------------------------
** ���� ����
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
**  �������α׷� ����
**===========================================================================
*/
/* ���� ���� */
#define INIT_APP_MAJOR_VERSION              SKY_GET_FILE_MAJOR_VERSION(SFL_APP)
#define INIT_APP_MINOR_VERSION              SKY_GET_FILE_MINOR_VERSION(SFL_APP)
/* �˶� �� ���� */
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

/* �˶� ���� */
#define INIT_ALARM_HOLIDAY_ACTIVE           1

/* �˶� �ð� ���� */
#define INIT_ALARM_TIME_TYPE                ATT_NONE
#define INIT_ALARM_TIME_ACTIVE              1
#define INIT_ALARM_TIME_USED                0
#define INIT_ALARM_TIME                     0
#define INIT_ALARM_TITLE                    ""  

/* �޸� ���� */
#define INIT_MEMO_TYPE                      MT_NONE
#define INIT_MEMO_TIME                      0
#define INIT_MEMO_DATA                      ""  

/* SMS �˶� ���� */
#define INIT_SMS_ALERT_TYPE                 SAT_ONCE
#if (MODEL_ID == MODEL_B0)
    #define INIT_SMS_ALERT_BELL_ID              MUS_CUCKOO
#elif (MODEL_ID == MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    #define INIT_SMS_ALERT_BELL_ID              MUS_SMS_1
#endif
#define INIT_SMS_ALERT_BELL_VOLUME          SMS_BELL_VOLUME_LEVEL
/* ����, WAP ��ȭ �ð� */
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
#define	INIT_RCVINFO						0						// ��ġ�뺸 ���� ���� ���� ����
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
**  GVM �ʱⰪ
**===========================================================================
*/
/* ���� ���� */
#define INIT_GVM_MAJOR_VERSION              SKY_GET_FILE_MAJOR_VERSION(SFL_GVM)
#define INIT_GVM_MINOR_VERSION              SKY_GET_FILE_MINOR_VERSION(SFL_GVM)


/*
**===========================================================================
**  Mobile Wallet �ʱⰪ
**===========================================================================
*/

//#define INIT_MW_MAJOR_VERSION              SKY_GET_FILE_MAJOR_VERSION(SFL_MWALLET)
//#define INIT_MW_MINOR_VERSION               SKY_GET_FILE_MINOR_VERSION(SFL_MWALLET)

/*
**===========================================================================
**  �ܸ��� ���� �ʱⰪ
**===========================================================================
*/
/* ���� ��ȭ - ����� ȭ�� ���� */
#define INIT_IS_DEBUG_SCREEN                FALSE
/* ������ ���� - ����� ȭ�� ���� */
//#ifdef FEATURE_DS_DEBUG_UI
#define INIT_IS_DATA_DEBUG_SCREEN           FALSE
//#endif /* FEATURE_DS_DEBUG_UI */
/* sleep ���·� ��� �� �� �ִ� ���� */
//#define INIT_IS_OK_TO_SLEEP                 FALSE
/* ������ ���Գ�? - off ���¸� �����ߴٴ� ��  */
#define INIT_IS_POWER_ON                    ON
/* �ø��� �����ų�, ������ �����ų� */
#define INIT_IS_PHONE_OPEN                  FALSE
/* ear jack�� �����ų� �����ų� */
#define INIT_IS_EAR_JACK_ON                 OFF
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
#define INIT_IS_CAMERA_ON                   OFF
#endif
/* ��ȭ�� ���� */
#define INIT_IS_MUTE                        FALSE
/* SHUT DOWN ���ΰ�? */
#define INIT_IS_SHUT_DOWN                   FALSE
/* ui task�� offline �����ΰ�? */
#define INIT_IS_OFFLINE                     FALSE
/* Whenther state machine is accepting keys */
//#define INIT_IS_GETKEY                      FALSE
/* is valid system time? entering DIGITAL service */
#define INIT_IS_VALID_SYSTEM_TIME           FALSE
/* ���� ���� ���� */
#define INIT_IS_HANDS_FREE                  FALSE
/* ���� �� */
#define INIT_IS_CHARGING                    FALSE
/* �ܺ� ���� */
#define INIT_IS_EXTERNAL_POWER              FALSE
/* Ű�е� �׽�Ʈ */
#define INIT_KEYPAD_TEST                    FALSE /* diag keypad test */
/* rosa 01-05-03 --> @why, what */
#define INIT_TO_WAP                         FALSE /* Go To WAP state */
#define INIT_FROM_WAP                       FALSE /* Go From WAP state */
#ifdef FEATURE_GVM
/* GVM */
#define INIT_IN_GVM                         FALSE /* GVM state */
/* GVM */
#define INIT_PLAYING_GVM                    FALSE /* GVM Play state */
/* GVM���� �̵����� */
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
/* �ڵ� ���*/
#define INIT_AUTO_LOCK                      FALSE
/* SMS ��� */
#define INIT_SMS_LOCK                       FALSE
/* ���� ���� �ʴ� ����*/
#define INIT_NO_OFF_BACKLIGHT               FALSE
/* fDataManager*/
#define INIT_DATAMANAGER                    FALSE
/* fAirDataManager */   
#define INIT_AIR_DATAMANAGER                FALSE
/* fIPAirDataManager */
#define INIT_IP_AIR_DATAMANAGER             FALSE
/* bDtrDisp */
#define INIT_DATA_CALL                      FALSE
/* ���� ��� */
#define INIT_DTR_DISP                       0x00
/* �̵�� �ܸ��� */
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
**  ��ȭ ���� �ʱⰪ
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
extern EXT_CONFIG_T        g_ExtConfig;    /* ��Ÿ ���� */
extern MISC_CONFIG_T       g_MiscConfig;   /* �ΰ� ��� ���� */
extern SYSTEM_CONFIG_T     g_SysConfig;    /* �ý��� ���� */
extern APP_CONFIG_T        g_AppConfig;    /* �������α׷� ���� */
#ifdef undersea
#ifdef FEATURE_GVM
extern GVM_CONFIG_T        g_GVMConfig;    /* GVM ���� */
#endif//#ifdef FEATURE_GVM
#endif //undersea
#ifdef FEATURE_MCOMMERCE
extern MW_TEMPCERT_CONFIG_T			g_MWTempCertConfig1;		/* Mobile Wallet ���� */
extern MW_TEMPCERT_CONFIG_T			g_MWTempCertConfig2;		/* Mobile Wallet ���� */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig1;		/* Mobile Wallet ���� */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig2;		/* Mobile Wallet ���� */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig3;		/* Mobile Wallet ���� */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig4;		/* Mobile Wallet ���� */
extern MW_MYCERT_CONFIG_T			 g_MWMYCertConfig5;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig1;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig2;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig3;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig4;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig5;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig6;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig7;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig8;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig9;		/* Mobile Wallet ���� */
extern MW_TCACERT_CONFIG_T			g_MWTCACertConfig10;		/* Mobile Wallet ���� */
extern MW_WALLET_CONFIG_T			g_MWWalletConfig;
#endif /* FEATURE_MCOMMERCE */

#ifdef FEATURE_CHARACTER_BELL
extern CHARACBELL_CONFIG_T          g_CharacbellConfig;    // Characbell ���� 
#endif // FEATURE_CHARACTER_BELL

extern SYSTEM_STATE_T      g_SysState;     /* �ý��� ���� */
extern CALL_STATE_T        g_CallState;    /* ��ȭ ���� */

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


