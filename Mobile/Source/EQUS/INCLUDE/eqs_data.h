/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     eqs_data.h
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
** 2001-08-01 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_DATA_H_
#define _EQS_DATA_H_

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
/* EQUS control command line bitmap */
extern BITMAP g_BmpControlCommandLineLeft;
extern BITMAP g_BmpControlCommandLineCenter;
extern BITMAP g_BmpControlCommandLineRight;

/* text edit - korean alphabet (SKY-hangul) */
extern ROM BITMAP g_BmpSkyHangulTable;
extern ROM BITMAP g_BmpSkyEnglishNumberTable;

/* text edit - symbol table */
extern ROM BITMAP g_BmpSymbolTable;

/* text edit - cursor */
//extern ROM BITMAP g_BmpCursor;

/* title pannel */
extern ROM BITMAP g_BmpTitlePannelBlue;
extern ROM BITMAP g_BmpTitlePannelDeepBlue;
extern ROM BITMAP g_BmpTitlePannelViolet;
extern ROM BITMAP g_BmpTitlePannelGreen;
extern ROM BITMAP g_BmpTitlePannelBrown;
extern ROM BITMAP g_BmpTitlePannelRed;


/* message box yes/no/ok button */
extern ROM BITMAP g_BmpButtonConfirm;
extern ROM BITMAP g_BmpButtonNo;
extern ROM BITMAP g_BmpButtonNoFocused;
extern ROM BITMAP g_BmpButtonYes;
extern ROM BITMAP g_BmpButtonYesFocused;




/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQS_DATA_H_ */
