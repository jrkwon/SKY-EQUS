/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_tokn.h
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
** 2000-06-28 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_TOKN_H_
#define _EQS_TOKN_H_

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

/* index list of the string resource */
enum tagEQS_TOKEN {
#undef DEFINE_TOKEN
#define DEFINE_TOKEN(nIdOfStr, pszToken)    TKN_##nIdOfStr,
#include "eqs_kor.tkn"
    EQS_TKN_ENDMARK
};
typedef enum tagEQS_TOKEN           EQS_TOKEN;    /* internal token */
#ifdef WIN32
typedef INT                         EQS_TOKEN_EX; /* external token */
#else
typedef WORD/* SKY_TOKEN */         EQS_TOKEN_EX; /* external token */
#endif

#define EQS_GET_TOKEN(nIdOfStr) \
                            aLocaleTable[EQS_GetCountry()].ppszToken[nIdOfStr]

#define EQS_GET_KOR_TOKEN(nIdOfStr) \
                            aLocaleTable[COUNTRY_KOREA].ppszToken[nIdOfStr]
#define EQS_GET_USA_TOKEN(nIdOfStr) \
                            aLocaleTable[COUNTRY_USA].ppszToken[nIdOfStr]
/* new language template ver 1.0 ------------------------------------------- *\
#define EQS_GET_XXX_TOKEN(nIdOfStr) \
                            aLocaleTable[COUNTRY_XXX].ppszToken[nIdOfStr]
\* ------------------------------------------------------------------------- */

#define EQS_GET_TOKEN_EX(nIdOfStr) \
                            g_aExStringTable[EQS_GetCountry()][nIdOfStr]

#define EQS_GET_KOR_TOKEN_EX(nIdOfStr) \
                            g_aExStringTable[COUNTRY_KOREA][nIdOfStr]
#define EQS_GET_USA_TOKEN_EX(nIdOfStr) \
                            g_aExStringTable[COUNTRY_USA][nIdOfStr]
/* new language template ver 1.0 ------------------------------------------- *\
#define EQS_GET_XXX_TOKEN(nIdOfStr) \
                            aLocaleTable[COUNTRY_XXX].ppszToken[nIdOfStr]
\* ------------------------------------------------------------------------- */


EQS_DCL ROM BYTE* ROM paszKorTokenEQUS[];
EQS_DCL ROM BYTE* ROM paszUsaTokenEQUS[];
/* new language template ver 1.0 ------------------------------------------- *\
EQS_DCL ROM BYTE* ROM paszXxxToken[];
\* ------------------------------------------------------------------------- */

/* TEMP TEMP TEMP TEMP ------------------------------------------------- */
#define EQS_TKN_APP_VERSION               EQS_T("1.0.0")
#define EQS_TKN_APP_NAME                  EQS_T("NUI")

#define ERR_TKN_OUT_OF_MEMORY             EQS_T("Out of Memory")
#define ERR_TKN_FREE_NULL_POINTER         EQS_T("Free null pointer")

#define ERR_TKN_INVALID_NEXT_APPLET_STATE EQS_T("Invalid next applet state");
#define ERR_TKN_INVALID_APPLET_START_FLAG EQS_T("Invalid applet start flag");
#define ERR_TKN_APPLET_STACK_OVERFLOW     EQS_T("Applet stack overflow");
#define ERR_TKN_NO_APPLET_IN_STACK        EQS_T("No Applet in the stack");

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
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}
#endif

#endif  /* _EQS_TOKN_H_ */
