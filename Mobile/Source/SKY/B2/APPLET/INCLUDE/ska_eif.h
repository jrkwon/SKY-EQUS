/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_eif.h
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
** 2001-02-07 redstar     Created.
**
** ============================================================================
*/

#ifndef _SKA_EIF_H_
#define _SKA_EIF_H_

//----------------------------------------------
//               EIF for wap
//----------------------------------------------

// EIF WAP Status
#define     EIF_WAP_NONE        0
#define 	EIF_WAP_IWFCON      1
#define 	EIF_WAP_WAPCON      2
#define     EIF_WAP_INPUT       3
#define     EIF_WAP_SELECT      4
#define		EIF_WAP_UPING		5
#define		EIF_WAP_UOPEN		6

// EIF WAP Error Type
#define     EIF_ERR_UOPEN_UNKONWN_CONTENT       1
#define 	EIF_ERR_UOPEN_FILE_NOT_FOUND        2
#define 	EIF_ERR_UOPEN_NOANSWER_SERVER       3
#define 	EIF_ERR_UOPEN_INVALID_URL           4
#define 	EIF_ERR_UOPEN_CONNECT_FAIL          5
#define 	EIF_ERR_IWFCON_NOANSWER_SERVER      6
#define 	EIF_ERR_IWFCON_CONNECT_FAIL         7
#define     EIF_ERR_IWFCON_NOT_IDLE             8
#define 	EIF_ERR_UPING_CONNECT_FAIL          9
#define     EIF_ERR_WAPCON_NOT_IN_WAP           10
#define 	EIF_ERR_WAPCON_CONNECT_FAIL         11

BOOL        SKY_API IsEIF_AutoReceive(void);
VOID        SKY_API SKA_MainEif(SKY_EVENT_T* pEvent);
                
#endif