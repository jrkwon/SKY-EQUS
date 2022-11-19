/*
 * Copyright (C) Ericsson Mobile Communications AB, 2001.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */

/* Program generated by SDT Cmicro, version 1.33 */

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif


/*************************************************************************
**                SECTION Types and Forward references                  **
*************************************************************************/

/*****
* PROCESS WAE_ME
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WAE_ME>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wae_me.spr,4,1)
******/
YPAD_PROTOTYPE(yPAD_z00008_WAE_ME)
typedef struct {
    PROCESS_VARS
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wae_me.spr(Init),254(5,25),2) */
    z_ctypes_4_VoidStar  z00008G_WAEManagementObject;
} yVDef_z00008_WAE_ME;
#define z000088_WaitForTerminateRepository  1
#define z000089_WaitForTerminatePush  2
#define z00008A_StandBy  3
#define z00008B_StartingURL  4
#define z00008C_Idle  5
#define z00008D_TerminatingURL  6
#define z00008E_TerminatingWMLS  7
#define z00008F_TerminatingUA  8

/*****
* SYNONYM wmls_more
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),4)
******/
#define z000240D_wmls_more SDL_INTEGER_LIT(1)

/*****
* SYNONYM wmls_done
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),5)
******/
#define z000240E_wmls_done SDL_INTEGER_LIT(2)

/*****
* SYNONYM wmls_donePrev
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),6)
******/
#define z000240F_wmls_donePrev SDL_INTEGER_LIT(3)

/*****
* SYNONYM wmls_doneGo
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),7)
******/
#define z000240G_wmls_doneGo SDL_INTEGER_LIT(4)

/*****
* SYNONYM wmls_error
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),8)
******/
#define z000240H_wmls_error SDL_INTEGER_LIT(5)

/*****
* SYNONYM wmls_external
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),9)
******/
#define z000240I_wmls_external SDL_INTEGER_LIT(6)

/*****
* SYNONYM wmls_lib
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),10)
******/
#define z000240J_wmls_lib SDL_INTEGER_LIT(7)

/*****
* SYNONYM lib_dialogs
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),13)
******/
#define z000240K_lib_dialogs SDL_INTEGER_LIT(5)

/*****
* SYNONYM libFunc_prompt
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),14)
******/
#define z000240L_libFunc_prompt SDL_INTEGER_LIT(0)

/*****
* SYNONYM libFunc_confirm
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),15)
******/
#define z000240M_libFunc_confirm SDL_INTEGER_LIT(1)

/*****
* SYNONYM libFunc_alert
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),16)
******/
#define z000240N_libFunc_alert SDL_INTEGER_LIT(2)

/*****
* SYNONYM libFunc_refresh
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),17)
******/
#define z000240O_libFunc_refresh SDL_INTEGER_LIT(6)

/*****
* SYNONYM libFunc_loadString
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),18)
******/
#define z000240P_libFunc_loadString SDL_INTEGER_LIT(13)

/*****
* SYNONYM lib_browser
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),19)
******/
#define z000240Q_lib_browser SDL_INTEGER_LIT(4)

/*****
* SYNONYM lib_url
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),20)
******/
#define z000240R_lib_url SDL_INTEGER_LIT(3)

/*****
* SYNONYM lib_wta_pub
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),23)
******/
#define z000240S_lib_wta_pub SDL_INTEGER_LIT(512)

/*****
* SYNONYM lib_wta_vcc
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),24)
******/
#define z000240T_lib_wta_vcc SDL_INTEGER_LIT(513)

/*****
* SYNONYM lib_wta_nt
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),25)
******/
#define z000240U_lib_wta_nt SDL_INTEGER_LIT(514)

/*****
* SYNONYM lib_wta_pb
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),26)
******/
#define z000240V_lib_wta_pb SDL_INTEGER_LIT(515)

/*****
* SYNONYM lib_wta_misc
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),27)
******/
#define z000240W_lib_wta_misc SDL_INTEGER_LIT(516)

/*****
* SYNONYM lib_wta_cl
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),28)
******/
#define z000240X_lib_wta_cl SDL_INTEGER_LIT(519)

/*****
* SYNONYM lib_wta_gsm_nf
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),29)
******/
#define z000240Y_lib_wta_gsm_nf SDL_INTEGER_LIT(518)

/*****
* SYNONYM lib_wta_pdc_nf
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),30)
******/
#define z000240Z_lib_wta_pdc_nf SDL_INTEGER_LIT(520)

/*****
* SYNONYM lib_wta_is_136_nf
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),770(80,55),31)
******/
#define z0002410_lib_wta_is_136_nf SDL_INTEGER_LIT(517)

/*****
* PROCESS WMLS_SM
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr,1,1)
******/
YPAD_PROTOTYPE(yPAD_z00024_WMLS_SM)
typedef struct {
    PROCESS_VARS
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),6) */
    SDL_Integer  z0002411_tmp;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),7) */
    SDL_Natural  z0002412_UApidAsInt;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),8) */
    SDL_PId  z0002413_UApid;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),9) */
    SDL_Integer  z0002414_invokeId;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),10) */
    z_ctypes_4_VoidStar  z0002415_resultStr;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),11) */
    SDL_Integer  z0002416_resultBool;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),12) */
    SDL_Integer  z0002417_scriptStatus;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),14) */
    SDL_Integer  z0002418_wmlPrev;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),15) */
    z_ctypes_4_VoidStar  z0002419_wmlGoUrl;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),17) */
    SDL_Integer  z000241A_errorCode;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),19) */
    SDL_Integer  z000241B_IANAcharset;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),20) */
    SDL_Natural  z000241C_strLen;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),22) */
    z_ctypes_4_VoidStar  z000241D_extUrl;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),24) */
    SDL_Natural  z000241E_libIndex;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),25) */
    SDL_Integer  z000241F_libFuncIndex;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),26) */
    z_ctypes_4_VoidStar  z000241G_libArgOpS;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),28) */
    z_ctypes_4_VoidStar  z000241H_incomingBP;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),29) */
    SDL_Natural  z000241I_incomingBPlen;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),30) */
    z_ctypes_4_VoidStar  z000241J_incomingUrl;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),31) */
    z_ctypes_4_VoidStar  z000241K_UAstruct;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),32) */
    SDL_Integer  z000241L_isTopPrio;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),34) */
    z_ctypes_4_VoidStar  z000241M_theSQ;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),36) */
    z_ctypes_4_VoidStar  z000241N_strMessage;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),37) */
    z_ctypes_4_VoidStar  z000241O_strDefaultInput;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),38) */
    z_ctypes_4_VoidStar  z000241P_strOk;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),39) */
    z_ctypes_4_VoidStar  z000241Q_strCancel;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),40) */
    z_ctypes_4_VoidStar  z000241R_strUrl;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),41) */
    z_ctypes_4_VoidStar  z000241S_strContentType;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),43) */
    SDL_Integer  z000241T_viewId;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),45) */
    z_ctypes_4_VoidStar  z000241U_resultVar;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),48) */
    SDL_Integer  z000241V_algoritm;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),49) */
    SDL_Integer  z000241W_sigLen;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),50) */
    z_ctypes_4_VoidStar  z000241X_signature;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),51) */
    z_ctypes_4_VoidStar  z000241Y_hashedKey;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),52) */
    SDL_Integer  z000241Z_hashedKeyLen;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),53) */
    z_ctypes_4_VoidStar  z0002420_certificate;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),54) */
    SDL_Integer  z0002421_certificateLen;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),55) */
    SDL_Integer  z0002422_certificateType;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),203(5,25),56) */
    SDL_Integer  z0002423_err;
} yVDef_z00024_WMLS_SM;

/*****
* SIGNAL Exec
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK WMLS/PROCESS WMLS_SM>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wmls\wmls_sm.spr(1),1127(30,10),2)
******/
#define z000240A_Exec 315
#define z000240B_Idle  1
#define z000240C_Normal  2

/*****
* PROCESS RES_ME
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr,4,1)
******/
YPAD_PROTOTYPE(yPAD_z000508_RES_ME)
typedef struct {
    PROCESS_VARS
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),3) */
    z_WAEPackage_32_RequestType  z0005080R_stRequestStruct;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),4) */
    SDL_PId  z0005080S_pidWmlUa;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),4) */
    SDL_PId  z0005080T_pidSessionProc;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),5) */
    z_ctypes_4_VoidStar  z0005080U_vsWAEMainObject;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),5) */
    z_ctypes_4_VoidStar  z0005080V_vsTransList;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),5) */
    z_ctypes_4_VoidStar  z0005080W_vsSessionList;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),5) */
    z_ctypes_4_VoidStar  z0005080X_vsActiveTransList;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),5) */
    z_ctypes_4_VoidStar  z0005080Y_vsDummy;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),5) */
    z_ctypes_4_VoidStar  z0005080Z_vsDummy2;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),6) */
    z_ctypes_A_UnsignedLongInt  z00050810_uiPidIndex;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),7) */
    SDL_Integer  z00050811_iErrorCode;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),7) */
    SDL_Integer  z00050812_iViewID;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),7) */
    SDL_Integer  z00050813_iSID;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),7) */
    SDL_Integer  z00050814_iTID;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),7) */
    SDL_Integer  z00050815_iDummy;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),7) */
    SDL_Integer  z00050816_iDummy2;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),3848(5,25),8) */
    SDL_Boolean  z00050817_bDummy;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr(Declarations_7),4598(5,70),2) */
    z_WAEPackage_33_ResponseType  z00050818_stResponseStruct;
} yVDef_z000508_RES_ME;
#define z0005080J_WaitStart  1
#define z0005080K_WaitCCStarted  2
#define z0005080L_WaitDIStarted  3
#define z0005080M_Idle  4
#define z0005080O_WaitForTerminateSessionDone  5
#define z0005080P_WaitForCCTerminateAck  6
#define z0005080Q_WaitForDITerminateAck  7

/*****
* PROCESS RES_CC
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr,4,1)
******/
YPAD_PROTOTYPE(yPAD_z000509_RES_CC)
typedef struct {
    PROCESS_VARS
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),3) */
    z_WAEPackage_32_RequestType  z0005097_stRequestStruct;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),4) */
    z_WAEPackage_33_ResponseType  z0005098_stResponseStruct;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),5) */
    SDL_PId  z0005099_pidWmlUa;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),6) */
    z_ctypes_4_VoidStar  z000509A_vsWAEMainObject;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),6) */
    z_ctypes_4_VoidStar  z000509B_vsHeader;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),6) */
    z_ctypes_4_VoidStar  z000509C_vsBody;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),7) */
    z_ctypes_7_LongInt  z000509D_liCacheSize;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),7) */
    z_ctypes_7_LongInt  z000509E_liDummy;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),8) */
    z_ctypes_A_UnsignedLongInt  z000509F_uliExpireDate;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),9) */
    SDL_Integer  z000509G_iDummy;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),9) */
    SDL_Integer  z000509H_iHeaderLength;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),9) */
    SDL_Integer  z000509I_iCacheInIdle;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),9) */
    SDL_Integer  z000509J_iSAR_Counter;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),10) */
    SDL_Integer  z000509K_iFirstInit;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),10) */
    SDL_Integer  z000509L_iErrorCode;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),10) */
    SDL_Integer  z000509M_iCacheDirective;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),239(5,175),10) */
    SDL_Integer  z000509N_iRedirected;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),2) */
    SDL_Integer  z000509O_iSID;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),2) */
    SDL_Integer  z000509P_iTID;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),2) */
    SDL_Integer  z000509Q_iBodyLen;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),2) */
    SDL_Integer  z000509R_iTemp;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),2) */
    SDL_Integer  z000509S_iTemp2;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),2) */
    SDL_Integer  z000509T_iNewChannelId;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),3) */
    z_ctypes_4_VoidStar  z000509U_vsEntityList;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),3) */
    z_ctypes_4_VoidStar  z000509V_vsHeaderHandle;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),3) */
    z_ctypes_4_VoidStar  z000509W_vsEntityHeader;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),3) */
    z_ctypes_4_VoidStar  z000509X_vsEntityBody;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr(Start_1),2648(105,190),4) */
    SDL_Boolean  z000509Y_bDummy1;
    SDL_Octet  yDcn_SDL_Octet;
} yVDef_z000509_RES_CC;
#define z0005093_WaitStart  1
#define z0005094_WaitInitCache  2
#define z0005095_Idle  3

/*****
* PROCESS RES_FC
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr,4,1)
******/
YPAD_PROTOTYPE(yPAD_z00050A_RES_FC)
typedef struct {
    PROCESS_VARS
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),2) */
    z_WAEPackage_32_RequestType  z00050A3_stRequestStruct;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),3) */
    z_WAEPackage_33_ResponseType  z00050A4_stResponseStruct;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),4) */
    z_ctypes_4_VoidStar  z00050A5_vsData;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),4) */
    z_ctypes_4_VoidStar  z00050A6_vsContentType;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),4) */
    z_ctypes_4_VoidStar  z00050A7_vsParameters;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),4) */
    z_ctypes_4_VoidStar  z00050A8_vsPidTidList;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),5) */
    SDL_PId  z00050A9_pidWmlUa;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),6) */
    z_ctypes_A_UnsignedLongInt  z00050AA_uiPidIndex;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),7) */
    SDL_Integer  z00050AB_iTID;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),7) */
    SDL_Integer  z00050AC_iErrorCode;
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_fc.spr(WaitForFileResponse_2),662(5,160),7) */
    SDL_Integer  z00050AD_iDataLength;
} yVDef_z00050A_RES_FC;
#define z00050A2_Idle  1

/*****
* SIGNAL StartCC
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\cnf_push_wta__res_hand.sbk(2),251(25,45),7)
******/
#define z00050B_StartCC 378
typedef struct {
    SIGNAL_VARS
    z_ctypes_4_VoidStar  Param1;
} yPDef_z00050B_StartCC;

/*****
* SIGNAL TerminateCC
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\cnf_push_wta__res_hand.sbk(2),251(25,45),8)
******/
#define z00050C_TerminateCC 379

/*****
* SIGNAL ME_UrlInCache
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\cnf_push_wta__res_hand.sbk(2),251(25,45),9)
******/
#define z00050D_ME_UrlInCache 380
typedef struct {
    SIGNAL_VARS
    z_WAEPackage_32_RequestType  Param1;
    SDL_PId  Param2;
} yPDef_z00050D_ME_UrlInCache;

/*****
* SIGNAL StartCCDone
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\cnf_push_wta__res_hand.sbk(2),251(25,45),18)
******/
#define z00050E_StartCCDone 381

/*****
* SIGNAL TerminateCCDone
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\cnf_push_wta__res_hand.sbk(2),251(25,45),19)
******/
#define z00050F_TerminateCCDone 382

/*****
* SIGNAL ME_ReqToWSP
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\cnf_push_wta__res_hand.sbk(2),251(25,45),20)
******/
#define z00050G_ME_ReqToWSP 383
typedef struct {
    SIGNAL_VARS
    z_WAEPackage_32_RequestType  Param1;
    SDL_PId  Param2;
} yPDef_z00050G_ME_ReqToWSP;

/*****
* SIGNAL GetFileContent
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\cnf_push_wta__res_hand.sbk(2),251(25,45),24)
******/
#define z00050H_GetFileContent 384
typedef struct {
    SIGNAL_VARS
    z_WAEPackage_32_RequestType  Param1;
    SDL_PId  Param2;
} yPDef_z00050H_GetFileContent;

/*****
* SIGNAL TerminateFC
* <<SYSTEM WAP/BLOCK WAE/SUBSTRUCTURE WAE/BLOCK ResourceHandler>>
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\cnf_push_wta__res_hand.sbk(2),251(25,45),25)
******/
#define z00050I_TerminateFC 385

/*************************************************************************
**                #CODE directives, #HEADING sections                   **
*************************************************************************/
                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\cnf_push_wta__wae.sbk,4,1) */
#include "errcodes.h"
#include "waedef.h"
#include "wae_cmmn.h"
#include "cache.h"
#include "header.h"
#include "capiclnt.h"
#include "si_sdl.h"
#include "aapimmi.h"
#include "aapiclnt.h"
#include "logcodes.h"
#include "url.h"
#include "confvars.h"
#include "dbfunct.h"
#include "wipdb.h"

                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_me.spr,4,1) */
#include "hdrutil.h"

                /* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\wae\wspif\res_cc.spr,4,1) */
#include "aapimem.h"
#include "wml_uafn.h"
