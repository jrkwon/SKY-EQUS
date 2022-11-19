/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     sky_mtxt.h
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
** 2000-11-10 girin     Created.
**
** ============================================================================
*/

#ifndef _SKY_MTXT_H_
#define _SKY_MTXT_H_

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
extern ROM MNODE_T TestMenuList;
extern ROM MNODE_T TestMenuListChild;

// NAM programming menu
extern ROM MNODE_T mnNAMProgTop;
extern ROM MNODE_T mnNAM1ProgSub;
extern ROM MNODE_T mnNAM2ProgSub;
extern ROM MNODE_T mnNAM3ProgSub;
extern ROM MNODE_T mnNAM4ProgSub;
extern ROM MNODE_T mnNAM5ProgSub;

//extern ROM MNODE_T mnSimpleNAMProgTop;


// Main Menu
extern ROM MNODE_T mnMainMenuTop;

extern ROM MNODE_T mnFacility;
extern ROM MNODE_T mnSetAudio;
extern ROM MNODE_T mnDisplaySet;
extern ROM MNODE_T mnAutoAnswer;
extern ROM MNODE_T mnCallTimeBattery;
extern ROM MNODE_T mnControlPannel;
extern ROM MNODE_T mnSystemSetting;
extern ROM MNODE_T mnGame;
extern ROM MNODE_T mnDataComm;
extern ROM MNODE_T mnIrDA;
extern ROM MNODE_T mnEMail;
extern ROM MNODE_T mnTestMenu;
extern ROM MNODE_T mnControlTestMenu;

//extern ROM MNODE_T mnVolumeWarn;
extern ROM MNODE_T mnBacklightSet;
extern ROM MNODE_T mnAirDataMgr;
extern ROM MNODE_T mnIrDABook;
extern ROM MNODE_T mnIrDAPhone;
extern ROM MNODE_T mnIrDAPC;
//TMP FOR WAP_FLAG TEST
extern ROM MNODE_T mnWAPTEST;

extern ROM MNODE_T mnCamera;
extern ROM MNODE_T mnPhoneBook;
extern ROM MNODE_T mnSMSMenu1;
extern ROM MNODE_T mnSMSMenu2;

extern ROM MNODE_T mn017Srvc;

extern ROM MNODE_T mn011Srvc;
extern ROM MNODE_T mn011SrvcSori;
extern ROM MNODE_T mn011RevCall;
extern ROM MNODE_T mn011InfoSam;
extern ROM MNODE_T mn011TrafficInfo;
extern ROM MNODE_T mn011SecretCall;
extern ROM MNODE_T mn011ServiceEdit;


//extern ROM MNODE_T mnHotMenu3;
//extern ROM MNODE_T mnHotMenu4;

extern ROM MNODE_T mnAlertType;

extern ROM MNODE_T mnDmKind;

//extern ROM MNODE_T mnBellVib;
extern ROM MNODE_T mnPhoneBookTest;
//extern ROM MNODE_T mnDataService;
//extern ROM MNODE_T mnControlPanel;
extern ROM MNODE_T mnSMS;
extern ROM MNODE_T mnMelodyTest;
extern ROM MNODE_T mnCameraTest;
extern ROM MNODE_T mnCallInfo;
extern ROM MNODE_T mnSmartBattTest;
extern ROM MNODE_T mnSystemSet;
extern ROM MNODE_T mnEFSTest;
extern ROM MNODE_T mnRFTest;

extern ROM MNODE_T mnSmsAlarm;

/*--------------------------------------------------------
** data service menu : legacy menu from old version.
*/
//extern ROM MNODE_T mnDataKind;
//extern ROM MNODE_T mnFacilitySet;

// IrDA
//extern ROM MNODE_T mnIncomingMode;
//extern ROM MNODE_T mnInternetSpeed;
//extern ROM MNODE_T mnRmSpeed;

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API SKA_MainNotYet(SKY_EVENT_T* pEvent);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_MTXT_H_ */
