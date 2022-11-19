/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_mwlt.h
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
** 2001-05-25 yhlee      Created.
**
** ============================================================================
*/

#ifndef _SKA_MWLT_H_
#define _SKA_MWLT_H_


#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
//#include "mwdef.h"


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

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
VOID SKY_API SKA_MainMWallet(SKY_EVENT_T *pEvent);

VOID SKY_API SKA_CertPINInput(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_process_GetMWData(SKY_EVENT_T *pEvent); 
VOID SKY_API SKA_SelectCertForSigning(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_EncryptSigningTest(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_MWCertResponse(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_MWHashedCertResponse(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_MWSignedCertResponse(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_MWShowCertResult(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_MWCheckTempCert(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_MWDeleteCert(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_MWSignText(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_MWEncryptText(SKY_EVENT_T *pEvent);

int MW_GetSavePosition(BYTE MyCert, int *efs_pos, int *log_pos);

extern char certPIN[9];
extern int PromptLen;
extern char userPromptData[256];


//extern  byte no_alpha_lines ;	// woodstok 00.4.28
//extern unsigned char gHashedPIN[ALG1_LEN];		//SHA1(PIN)
//extern unsigned char wEncSecretKey[ALG2_LEN*2];	//세션키에 의해 암호화된 비밀키	
//extern boolean input_code;
//extern nv_stat_enum_type wap_process_nv_item
//(
//  nv_items_enum_type  item_code,       /* Item to get                      */
//  nv_item_type        *item_ptr,        /* Pointer where to put the item    */
//  boolean              read			   /* read = TRUE; READ_F, read = FALSE;WRITE_F */
//);

//extern char final_shopping_list[MAX_SHOPPING_LIST_LINE*LCD_LINE_SIZE];
//extern char final_shopping_list2[MAX_SHOPPING_LIST_LINE*LCD_LINE_SIZE];
//extern char shopping_list[MAX_SHOPPING_LIST_LINE*LCD_LINE_SIZE];

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_MWLLT_H_ */
