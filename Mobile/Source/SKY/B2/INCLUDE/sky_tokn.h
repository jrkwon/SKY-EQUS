/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_tokn.h
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

#ifndef _SKY_TOKN_H_
#define _SKY_TOKN_H_

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
/* index list of the string resource */
enum tagSKY_TOKEN {
#undef DEFINE_TOKEN
#define DEFINE_TOKEN(nIdOfStr, pszToken)    TKN_##nIdOfStr,
#include "sky_kor.tkn"
    SKY_TKN_ENDMARK
};
typedef enum tagSKY_TOKEN            SKY_TOKEN;

#define SKY_GET_TOKEN(nIdOfStr) \
                            g_aStringTable[EQS_GetCountry()][nIdOfStr]

#define SKY_GET_KOR_TOKEN(nIdOfStr) \
                            g_aStringTable[COUNTRY_KOREA][nIdOfStr]
#define SKY_GET_USA_TOKEN(nIdOfStr) \
                            g_aStringTable[COUNTRY_USA][nIdOfStr]
/* new language template ver 1.0 ------------------------------------------- *\
#define SKY_GET_XXX_TOKEN(nIdOfStr) \
                            aLocaleTable[COUNTRY_XXX].ppszToken[nIdOfStr]
\* ------------------------------------------------------------------------- */

extern ROM BYTE* ROM paszKorToken[];
extern ROM BYTE* ROM paszUsaToken[];
/* new language template ver 1.0 ------------------------------------------- *\
extern ROM BYTE* ROM paszXxxToken[];
\* ------------------------------------------------------------------------- */

extern ROM BYTE* ROM *g_aStringTable[];

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

#endif  /* _SKY_TOKN_H_ */
