/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_i18n.h
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
** 2000-08-13 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_I18N_H_
#define _EQS_I18N_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "eqs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* default language system */
#define DEFAULT_COUNTRY         COUNTRY_KOR

#define TKN_KOR_DAT_AM          "����"
#define TKN_KOR_DAT_PM          "����"

#define TKN_USA_DAT_AM          "AM"
#define TKN_USA_DAT_PM          "PM"

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagEQS_COUNTRY {
    COUNTRY_KOR,
    COUNTRY_USA,
/* new language template ver 1.0 ------------------------------------------- *\
    COUNTRY_XXX
\* ------------------------------------------------------------------------- */
    COUNTRY_MAX
};
typedef enum tagEQS_COUNTRY     EQS_COUNTRY;

/*
** H = �ð�, M = ��, S = ��, T = ���� �Ǵ� ����
** h = 12�ð�
** H = 24�ð�
** HH, MM, SS = �տ� 0 ǥ��
** H, M, S = �տ� 0 ǥ�� ����
*/
enum tagTIME_DISPLAY {
    /* Korea */
    TM_AMPM_h_MM_SS,    /* TT h:MM:SS   ���� 1:02:18        */
    TM_AMPM_hh_MM_SS,   /* TT hh:MM:SS  ���� 01:02:18       */
    /* USA */
    TM_h_MM_SS_AMPM,    /* h:MM:SS TT   1:02:18 AM        */
    TM_hh_MM_SS_AMPM,   /* hh:MM:SS TT  01:02:18 PM      */
/* new language template ver 1.0 ------------------------------------------- *\
    TM_XX
\* ------------------------------------------------------------------------- */
    /* common */
    TM_H_MM_SS,         /* H:MM:SS      13:02:18            */
    TM_HH_MM_SS         /* HH:MM:SS     02:04:08            */
};
typedef enum tagTIME_DISPLAY       TIME_DISPLAY;

/*
** Y = year, M = month, D = Date, DDDD = Day
** YYYY = 
** YY = 
** h = 12 hours
** H = 24 hours
** HH, MM, SS = start with '0'
** H, M, S = no '0'
** MMM = not numeric month. alphabetical month
** 
** C = comma
*/
enum tagDATE_DISPLAY {
    /* -----------
    ** Simple form
    */
    DT_YY_MM_DD,              /* 00?08?18 */

/* new language template ver 1.0 ------------------------------------------- *\
    DT_XX
\* ------------------------------------------------------------------------- */
    /* Korea */
    DT_YYYY_M_D,              /* 2000-8-18 */
    DT_YYYY_MM_DD,            /* 2000-08-18 */
    DT_YY_M_D,                /* 00-8-18 */
    /* USA */
    DT_M_D_YYYY,              /* 8/18/2000 */
    DT_M_D_YY,                /* 8/18/00 */
    DT_MM_DD_YY,              /* 08/18/00 */
    DT_MM_DD_YYYY,            /* 08/18/2000 */
    DT_DD_MMM_YY,             /* dd-mmm-00 */

    /* -----------
    ** detail form
    */
/* new language template ver 1.0 ------------------------------------------- *\
    DT_L_XX
\* ------------------------------------------------------------------------- */
    /* Korea */
    DT_L_YYYY_M_D_DDDD,         /* 2000�� 8�� 7�� ȭ���� */
    DT_L_YYYY_M_D,              /* 2000�� 8�� 7�� */
    DT_L_YY_M_D_DDDD,           /* 00�� 8�� 7�� ȭ���� */
    DT_L_YY_M_D,                /* 00�� 8�� 7�� */
    DT_L_YYYY_MM_DD_DDDD,       /* 2000�� 08�� 07�� ȭ���� */
    DT_L_YYYY_MM_DD,            /* 2000�� 08�� 07�� */
    /* USA */
    DT_L_DDDD_C_MMMM_DD_C_YYYY, /* Tuesday, August 07, 2000 */
    DT_L_MMMM_DD_C_YYYY,        /* August 07, 2000 */
    DT_L_DDDD_C_DD_MMMM_C_YYYY, /* Tuesday, 07 August, 2000 */
    DT_L_DD_MMMM_C_YYYY         /* 07 August, 2000 */
};
typedef enum tagDATE_DISPLAY        DATE_DISPLAY;

typedef struct tagLCL_NUMBER        LCL_NUMBER;
typedef struct tagLCL_CURRENCY      LCL_CURRENCY;
typedef struct tagTIME_FORMAT       TIME_FORMAT;
typedef struct tagLCL_TIME          LCL_TIME;
typedef struct tagDATE_FORMAT       DATE_FORMAT;
typedef struct tagLCL_DATE          LCL_DATE;
typedef struct tagEQS_LOCALE        EQS_LOCALE;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagLCL_NUMBER {
    BYTE bSymbolOfDecimalPoint;  /* �Ҽ��� ��ȣ       : , .            */
    INT8 nNumberOfAfterDecimal;  /* �Ҽ��� ���� �ڸ��� : 0 ~ 9          */
    BYTE bSymbolOfDecimalPlace;  /* �ڸ��� ���� ��ȣ   : ,              */
    BYTE bSymbolOfNegativeNumber;/* ���� ��ȣ         : -              */
};

struct tagLCL_CURRENCY {
    BYTE bSymbolOfCurrency;      /* Won, Dollar, ... */
    BYTE bSymbolOfDecimalPoint;  /* . */
    INT8 nNumberOfAfterDecimal;   /* �Ҽ��� ���� �ڸ��� : 0 ~ 9          */
    BYTE bSymbolOfDecimalPlace;  /* �ڸ��� ���� ��ȣ   : ,              */
};    

struct tagTIME_FORMAT {
    TIME_DISPLAY      TimeDisplay;
    BYTE*             pszStringFormat;  /* "%s %d:%02d:%02d" */
};

struct tagLCL_TIME {
    ROM TIME_FORMAT* pTimeFormat;
    BYTE bSymbolOfTimeSeparator;
    BYTE* pszSymbolOfAM;
    BYTE* pszSymbolOfPM;
};

struct tagDATE_FORMAT {
    DATE_DISPLAY      TimeDisplay;
    BYTE*             pszStringFormat;  /* "%s %d:%02d:%02d" */
};

struct tagLCL_DATE {
    ROM DATE_FORMAT* pDateFormat;
    BYTE         bSymbolOfDateSeparator;
};

struct tagEQS_LOCALE {
    EQS_COUNTRY     Country;
    ROM BYTE* ROM   *ppszToken;     /* token */
    //EQS_RC          Resource;
    LCL_NUMBER      Number;
    LCL_CURRENCY    Currency;
    LCL_TIME        Time;
    LCL_DATE        Date;
    ROM BYTE**          ppNeedHypenCode;
    /* the function pointer of input method */
    VOID            (*pfnIME)(HCONTROL hControl, EQS_EVENT* pAnEvent);
};

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
EQS_DCL ROM TIME_FORMAT aKorTimeFormat[];
EQS_DCL ROM TIME_FORMAT aUsaTimeFormat[];
EQS_DCL ROM DATE_FORMAT aKorDateFormat[];
EQS_DCL ROM DATE_FORMAT aUsaDateFormat[];
/* new language template ver 1.0 ------------------------------------------- *\
EQS_DCL ROM DATE_FORMAT aXxxDateFormat[];
\* ------------------------------------------------------------------------- */

EQS_DCL ROM EQS_LOCALE aLocaleTable[];
EQS_DCL BYTE* ROM * g_aExStringTable[];

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
EQS_DCL VOID EQS_API EQS_InitLocale(ROM BYTE* ROM *ppaszToken[]);
EQS_DCL VOID EQS_API EQS_SetCountry(EQS_COUNTRY CountryA);
EQS_DCL EQS_COUNTRY EQS_API EQS_GetCountry(VOID);

/* TEMP TEMP TEMP ------ */
EQS_DCL VOID EQS_API EQC_KorEditBox(HCONTROL hControl, EQS_EVENT* pAnEvent);
EQS_DCL VOID EQS_API EQC_UsaEditBox(HCONTROL hControl, EQS_EVENT* pAnEvent);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQS_I18N_H_ */
