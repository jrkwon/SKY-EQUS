/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_i18n.c
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
** 2000-08-14 jrkwon     Created.
**
** ===========================================================================
*/

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

ROM TIME_FORMAT aKorTimeFormat[] = {
    { TM_AMPM_h_MM_SS,  (BYTE*)"%s %2d%c%02d%c%02d"},
    { TM_AMPM_hh_MM_SS, (BYTE*)"%s %2d%c%02d%c%02d"},
    { TM_H_MM_SS,       (BYTE*)"%2d%c%02d%c%02d"},
    { TM_HH_MM_SS,      (BYTE*)"%02d%c%02d%c%02d"}
};

ROM TIME_FORMAT aUsaTimeFormat[] = {
    { TM_h_MM_SS_AMPM,  (BYTE*)"%2d%c%02d%c%02d %s"},
    { TM_hh_MM_SS_AMPM, (BYTE*)"%2d%c%02d%c%02d %s"},
    { TM_H_MM_SS,       (BYTE*)"%2d%c%02d%c%02d"},
    { TM_HH_MM_SS,      (BYTE*)"%02d%c%02d%c%02d"}
};

/* new language template ver 1.0 ------------------------------------------- *\
ROM TIME_FORMAT aXxxTimeFormat[] = {
    { TM_h_MM_SS_AMPM,  (BYTE*)"%2d%c%02d%c%02d %s"},
    { TM_hh_MM_SS_AMPM, (BYTE*)"%2d%c%02d%c%02d %s"},
    { TM_H_MM_SS,       (BYTE*)"%2d%c%02d%c%02d"},
    { TM_HH_MM_SS,      (BYTE*)"%02d%c%02d%c%02d"}
};
\* ------------------------------------------------------------------------- */

ROM DATE_FORMAT aKorDateFormat[] = {
    /* -----------
    ** Simple form
    */
    { DT_YYYY_M_D,              (BYTE*)"%4d%c%d%c%d" },        /* 2000-8-18 */
    { DT_YYYY_MM_DD,            (BYTE*)"%4d%c%02d%c%02d" },    /* 2000-08-18 */
    { DT_YY_MM_DD,              (BYTE*)"%02d%c%02d%c%02d" },   /* 00-08-18 */
    { DT_YY_M_D,                (BYTE*)"%02d%c%d%c%d" },       /* 00-8-18 */
    /* -----------
    ** detail form
    */
    { DT_L_YYYY_M_D_DDDD,       (BYTE*)"%4d년 %d월 %d일 %s요일" }, /* 2000년 8월 7일 화요일 */
    { DT_L_YYYY_M_D,            (BYTE*)"%4d년 %d월 %d일" },       /* 2000년 8월 7일 */
    { DT_L_YY_M_D_DDDD,         (BYTE*)"%02d년 %d월 %d일 %s요일" },/* 00년 8월 7일 화요일 */
    { DT_L_YY_M_D,              (BYTE*)"%02d년 %d월 %d일" },       /* 00년 8월 7일 */
    { DT_L_YYYY_MM_DD_DDDD,     (BYTE*)"%4d년 %02d월 %02d일 %s요일" }, /* 2000년 08월 07일 화요일 */
    { DT_L_YYYY_MM_DD,          (BYTE*)"%4d년 %02d월 %02d일" }     /* 2000년 08월 07일 */
};

ROM DATE_FORMAT aUsaDateFormat[] = {
    /* -----------
    ** Simple form
    */
    { DT_M_D_YYYY,              (BYTE*)"%d%c%d%c%4d" }, /* 8/18/2000 */
    { DT_M_D_YY,                (BYTE*)"%d%c%d%02d" }, /* 8/18/00 */
    { DT_MM_DD_YY,              (BYTE*)"%02d%c%02d%c%02d" }, /* 08/18/00 */
    { DT_MM_DD_YYYY,            (BYTE*)"%02d%c%02d%c%4d" }, /* 08/18/2000 */
    { DT_YY_MM_DD,              (BYTE*)"%02d%c%02d%c%02d" }, /* 00/08/18 */
    { DT_DD_MMM_YY,             (BYTE*)"%02d%c%s%c%02d" }, /* 18-Aug-00 */

    /* -----------
    ** detail form
    */
    { DT_L_DDDD_C_MMMM_DD_C_YYYY, (BYTE*)"%s, %s %02d, %4d" }, /* Tuesday, August 07, 2000 */
    { DT_L_MMMM_DD_C_YYYY,        (BYTE*)"%s %02d, %4d" }, /* August 07, 2000 */
    { DT_L_DDDD_C_DD_MMMM_C_YYYY, (BYTE*)"%s, %02d %s, %4d" }, /* Tuesday, 07 August, 2000 */
    { DT_L_DD_MMMM_C_YYYY,        (BYTE*)"%02d %s, %4d" } /* 07 August, 2000 */
};

/* new language template ver 1.0 ------------------------------------------- *\
ROM DATE_FORMAT aXxxDateFormat[] = {
    { DT_M_D_YYYY,              (BYTE*)"%d%c%d%c%4d" }, 
    { DT_M_D_YY,                (BYTE*)"%d%c%d%02d" }, 
    { DT_MM_DD_YY,              (BYTE*)"%02d%c%02d%c%02d" }, 
    { DT_MM_DD_YYYY,            (BYTE*)"%02d%c%02d%c%4d" }, 
    { DT_YY_MM_DD,              (BYTE*)"%02d%c%02d%c%02d" },
    { DT_DD_MMM_YY,             (BYTE*)"%02d%c%s%c%02d" }, 

    { DT_L_DDDD_C_MMMM_DD_C_YYYY, (BYTE*)"%s, %s %02d, %4d" }, 
    { DT_L_MMMM_DD_C_YYYY,        (BYTE*)"%s %02d, %4d" }, 
    { DT_L_DDDD_C_DD_MMMM_C_YYYY, (BYTE*)"%s, %02d %s, %4d" }, 
    { DT_L_DD_MMMM_C_YYYY,        (BYTE*)"%02d %s, %4d" } 
};
\* ------------------------------------------------------------------------- */

LOCAL ROM BYTE* paszKorNeedHypenCode[] = {
    /* overseas call by land */
    (BYTE*)"001",(BYTE*)"002",(BYTE*)"008",
    /* service provider ID code */
    (BYTE*)"011",(BYTE*)"012",(BYTE*)"015",(BYTE*)"016",(BYTE*)"017",(BYTE*)"018",(BYTE*)"019",(BYTE*)"080",
    /* area code */
    (BYTE*)"02", (BYTE*)"031",(BYTE*)"032",(BYTE*)"033",
    (BYTE*)"041",(BYTE*)"042",(BYTE*)"043",
	(BYTE*)"051",(BYTE*)"052",(BYTE*)"053",(BYTE*)"054",(BYTE*)"055",
    (BYTE*)"061",(BYTE*)"062",(BYTE*)"063",(BYTE*)"064",
    /* overseas call by mobile */
    (BYTE*)"00700",(BYTE*)"00770",(BYTE*)"00300",
    NULL
};

LOCAL ROM BYTE* paszUsaNeedHypenCode[] = {
    (BYTE*)"", (BYTE*)"", NULL
};

/* new language template ver 1.0 ------------------------------------------- *\
LOCAL ROM BYTE* paszXxxNeedHypenCode[] = {
    "", ""
};

\* ------------------------------------------------------------------------- */

ROM EQS_LOCALE aLocaleTable[] = {
    { 
        COUNTRY_KOR,  
        paszKorTokenEQUS,
        { '.', 2, ',', '-' },
        { '\\', '.', 0, ',' },
        { aKorTimeFormat, ':', (BYTE*)TKN_KOR_DAT_AM, (BYTE*)TKN_KOR_DAT_PM },
        { aKorDateFormat, '-' },
        paszKorNeedHypenCode,
        EQC_KorEditBox
    },
    { 
        COUNTRY_USA, 
        paszUsaTokenEQUS,
        { '.', 2, ',', '-' },
        { '$', '.', 2, ',' },
        { aUsaTimeFormat, ':', (BYTE*)TKN_USA_DAT_AM, (BYTE*)TKN_USA_DAT_PM },
        { aUsaDateFormat, '/' },
        paszUsaNeedHypenCode,
        EQC_UsaEditBox
    }
/* new language template ver 1.0 ------------------------------------------- *\
    ,
    { 
        COUNTRY_XXX, 
        paszXxxToken,
        { '.', 2, ',', '-' },
        { '$', '.', 2, ',' },
        { aXxxTimeFormat, ':', TKN_XXX_DAT_AM, TKN_XXX_DAT_PM },
        { aXxxDateFormat, '/' },
        EQC_XxxEditBox
     }
\* ------------------------------------------------------------------------- */
};

BYTE* ROM * g_aExStringTable[COUNTRY_MAX];

/* the current country setting. used as an index of LocalTable */
LOCAL EQS_COUNTRY Country = DEFAULT_COUNTRY;

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

VOID EQS_API EQS_InitLocale(ROM BYTE* ROM *ppaszToken[])
{
    INT8 i;

    for(i = 0; i < COUNTRY_MAX; i++)
        g_aExStringTable[i] = (BYTE* ROM *)ppaszToken[i];
}

VOID EQS_API EQS_SetCountry(EQS_COUNTRY CountryA)
{
    Country = CountryA;
}

EQS_COUNTRY EQS_API EQS_GetCountry(VOID)
{
    return Country;
}

EQS_LOCALE EQS_API EQS_GetLocale(VOID)
{
    return aLocaleTable[EQS_GetCountry()];
}

