/*
** ============================================================================
**
** Project: 
**     BUNNY
**
** File:
**     sky_data.h
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
** 2000-10-18 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_DATA_H_
#define _SKY_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
//extern ROM BITMAP g_Bmpeqc_anim.h"
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define BUILT_IN_ANI_COUNT         6        /* 그림없음 포함 */

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
/* main annunciator image */
extern ROM BITMAP g_BmpAnnunAlarm;
extern ROM BITMAP g_BmpAnnunAnsweringMachine;
extern ROM BITMAP g_BmpAnnunBattery;
extern ROM BITMAP g_BmpAnnunBellVibration;
extern ROM BITMAP g_BmpAnnunCall;
extern ROM BITMAP g_BmpAnnunMessage;
extern ROM BITMAP g_BmpAnnunProtocol;
extern ROM BITMAP g_BmpAnnunRoamRSSI;
extern ROM BITMAP g_BmpAnnunRSSI;

/* sub annunciator image */
extern ROM BITMAP g_BmpSubAnnunAlarm;
extern ROM BITMAP g_BmpSubAnnunAnsweringMachine;
extern ROM BITMAP g_BmpSubAnnunBattery;
extern ROM BITMAP g_BmpSubAnnunBellVibration;
extern ROM BITMAP g_BmpSubAnnunMessage;
extern ROM BITMAP g_BmpSubAnnunProtocol;
extern ROM BITMAP g_BmpSubAnnunRoamRSSI;
extern ROM BITMAP g_BmpSubAnnunRSSI;

/* command line image - 4 gray*/
extern BITMAP g_BmpCommandLineCenter;
extern BITMAP g_BmpCommandLineLeft;
extern BITMAP g_BmpCommandLineRight;

/* idle command line image - color */
extern ROM BITMAP g_BmpCommandlineIdleRedT;
extern ROM BITMAP g_BmpCommandlineIdleRedI;
extern ROM BITMAP g_BmpCommandlineIdleBrownT;
extern ROM BITMAP g_BmpCommandlineIdleBrownI;
extern ROM BITMAP g_BmpCommandlineIdleDeepBlueT;
extern ROM BITMAP g_BmpCommandlineIdleDeepBlueI;
extern ROM BITMAP g_BmpCommandlineIdleGreenT;
extern ROM BITMAP g_BmpCommandlineIdleGreenI;
extern ROM BITMAP g_BmpCommandlineIdleBlueT;
extern ROM BITMAP g_BmpCommandlineIdleBlueI;
extern ROM BITMAP g_BmpCommandlineIdleVioletT;
extern ROM BITMAP g_BmpCommandlineIdleVioletI;


/* WAP command line image */
extern BITMAP g_BmpWapCommandLineCenter;
extern BITMAP g_BmpWapCommandLineCenterArrow;
extern BITMAP g_BmpWapCommandLineCenterOK;
extern BITMAP g_BmpWapCommandLineLeft;
extern BITMAP g_BmpWapCommandLineRight;

/* auto answer */
extern ROM ANIMATION_INFO_T g_AniAutoAnswerDrive;
extern ROM ANIMATION_INFO_T g_AniAutoAnswerGeneral;
extern ROM ANIMATION_INFO_T g_AniAutoAnswerMeeting;
extern ROM ANIMATION_INFO_T g_AniAutoAnswerUser;
extern ROM BITMAP g_BmpDateTimePannel;   
extern ROM BITMAP g_BmpAutoAnswerPannelAutoAnswer;
extern ROM BITMAP g_BmpAutoAnswerPannelNewAutoAnswer; 

/* system */
extern ROM BITMAP g_BmpBootLogoMain;
extern ROM BITMAP g_BmpBootProgressBar;
extern ROM BITMAP g_BmpLock;
//extern ROM BITMAP g_BmpGeneralPannel;
extern ROM BITMAP g_BmpBootLogoSub;
extern ROM ANIMATION_INFO_T g_AniSpeed011;
//extern ROM ANIMATION_INFO_T g_AniWaterSky;
extern ROM ANIMATION_INFO_T g_AniStartingWater;
//extern ROM ANIMATION_INFO_T g_AniEndingCloud;
extern ROM ANIMATION_INFO_T g_AniCalling; 
//extern ROM ANIMATION_INFO_T g_AniStartingSun;
//extern ROM ANIMATION_INFO_T g_AniEndingGoodBye;
//extern ROM ANIMATION_INFO_T g_AniPowerDigital017;
extern ROM ANIMATION_INFO_T g_AniSkyLogo;

/* GVM */
extern ROM ANIMATION_INFO_T g_AniDownloadGVM;

/* IrDA */
extern ROM BITMAP g_BmpExchangeDataIrDA;
extern ROM BITMAP g_BmpIrdaPannel;

/* phone book */
extern ROM BITMAP g_BmpFindData;
extern ROM BITMAP g_BmpPBookPannel;

/* call */
extern ROM BITMAP g_BmpHoldOnCall;
extern ROM ANIMATION_INFO_T g_AniInDataService;
extern ROM ANIMATION_INFO_T g_AniMissedCall;
extern ROM ANIMATION_INFO_T g_AniOriginatingCall;
extern ROM BITMAP g_BmpPhoneOpen;
extern ROM BITMAP g_BmpVolumeInCall;
extern ROM BITMAP g_BmpCallPannelMissedCall;
extern ROM BITMAP g_BmpCallPannelVolumeInCall;
extern ROM BITMAP g_BmpCallPannel;

/* record/play */
extern ROM ANIMATION_INFO_T g_AniPlaying;
extern ROM BITMAP g_BmpRecord;
extern ROM ANIMATION_INFO_T g_AniRecording;
//extern ROM BITMAP g_BmpRemainTimePannel;
//extern ROM BITMAP g_BmpRecordPannelVoiceMemo;
extern ROM BITMAP g_BmpRecordPannelStartRecord;
extern ROM BITMAP g_BmpRecordPannelRecordVoice;

/* email */
extern ROM ANIMATION_INFO_T g_AniReceiveEMail;
extern ROM ANIMATION_INFO_T g_AniSendEMail;
extern ROM BITMAP g_BmpConnectDataService;

/* SMS */
extern ROM ANIMATION_INFO_T g_AniReceiveMessage;
extern ROM ANIMATION_INFO_T g_AniSendMessage;
extern ROM BITMAP g_BmpCallPannelTransMessage;
//extern ROM BITMAP g_BmpCallPannelNewMessage;

/* idle */
extern ROM BITMAP g_BmpBigAMPM;
extern ROM BITMAP g_BmpBigWatchNumber;
//extern ROM BITMAP g_BmpNewRecordCount;
extern ROM BITMAP g_BmpRecordCount;
extern ROM BITMAP g_BmpSmallAMPM;
extern ROM BITMAP g_BmpSmallWatchNumber;
extern ROM BITMAP g_BmpWatchPannel;
extern ROM BITMAP g_BmpUserMentRecord;
extern ROM BITMAP g_BmpAutoAnswerPannelNew;
extern ROM BITMAP g_BmpAutoAnswerPannelDrive;
extern ROM BITMAP g_BmpAutoAnswerPannelGeneral;
extern ROM BITMAP g_BmpAutoAnswerPannelMeeting;
extern ROM BITMAP g_BmpAutoAnswerPannelUser;
//extern ROM BITMAP g_BmpIdleSmall;
//extern ROM ANIMATION_INFO_T g_AniIdleBig;
extern ROM BITMAP g_BmpClock01;
extern ROM BITMAP g_BmpClock02;
//extern ROM BITMAP g_BmpStandbyBig;
//extern ROM BITMAP g_BmpStandbyBig-back;
//extern ROM BITMAP g_BmpStandbyBig16;
//extern ROM BITMAP g_BmpStandbyBig-back16;

/* stopwatch */
extern ROM ANIMATION_INFO_T g_AniRunningCheetah;
extern ROM BITMAP g_BmpStopWatchPannel;

/* alarm */
extern ROM ANIMATION_INFO_T g_AniAnniversary;
extern ROM ANIMATION_INFO_T g_AniGeneralAlarm;

/* calculator */
extern ROM BITMAP g_BmpCalCulatorOutputBox;
extern ROM BITMAP g_BmpCalCulator;
extern ROM BITMAP g_BmpCalCulatorSym;

/* main menu */

extern ROM BITMAP g_BmpMainMenuBase;
extern ROM ANIMATION_INFO_T g_AniMenuSoundSett;
extern ROM ANIMATION_INFO_T g_AniMenuDataComm;
extern ROM ANIMATION_INFO_T g_AniMenuGameGVM;
extern ROM ANIMATION_INFO_T g_AniMenuAutoAnswer;
extern ROM ANIMATION_INFO_T g_AniMenuEnvironSett;
extern ROM ANIMATION_INFO_T g_AniMenuElecBook;
extern ROM ANIMATION_INFO_T g_AniMenuScreenSett;
extern ROM ANIMATION_INFO_T g_AniMenuCallingTime;
extern ROM ANIMATION_INFO_T g_AniMenuIrDA;


/* pbook menu */

extern ROM BITMAP g_BmpPBookMenuBase;
extern ROM ANIMATION_INFO_T g_AniPhoneSpeed;
extern ROM ANIMATION_INFO_T g_AniPhoneShortening;
extern ROM ANIMATION_INFO_T g_AniPhoneGroupEdit;
extern ROM ANIMATION_INFO_T g_AniPhoneNumberInput;
extern ROM ANIMATION_INFO_T g_AniPhoneNumber;
extern ROM ANIMATION_INFO_T g_AniPhoneName;
extern ROM ANIMATION_INFO_T g_AniPhoneSelf;
extern ROM ANIMATION_INFO_T g_AniPhone011plus;


/* SMS menu */

extern ROM ANIMATION_INFO_T g_AniSMSDelete;
extern ROM BITMAP g_BmpSMSMenuBase;
extern ROM ANIMATION_INFO_T g_AniSMSNTopMail;
extern ROM ANIMATION_INFO_T g_AniSMSReceiptBox;
extern ROM ANIMATION_INFO_T g_AniSMSSendingBox;
extern ROM ANIMATION_INFO_T g_AniSMSEmail;
extern ROM ANIMATION_INFO_T g_AniSMSReceiptAlarm;
//extern ROM ANIMATION_INFO_T g_AniSMSLocationTrace;
//extern ROM BITMAP g_BmpSMSLocationTrace;

/* WorldTime */
extern ROM BITMAP g_BmpWorldMap;
extern ROM BITMAP g_BmpWTimeMoon;
extern ROM BITMAP g_BmpWTimeSun;

/* Wap */
//extern ROM ANIMATION_INFO_T g_AniWap;

/* Calender */
extern ROM BITMAP g_BmpSquareBorder;
extern ROM BITMAP g_BmpSquareBorderBlack;
extern ROM BITMAP g_BmpSquareWhite;
extern ROM BITMAP g_BmpSquareThickBlackline;

extern ROM BITMAP g_BmpSquareBlack;
extern ROM BITMAP g_BmpSquareRed;
extern ROM BITMAP g_BmpSquareBlackOn;
extern ROM BITMAP g_BmpSquareThickBlacklineOn;
extern ROM BITMAP g_BmpSquareBlacredOn;

extern ROM BITMAP g_BmpSearchCalendar;

//extern ROM BITMAP g_BmpCalbox1;
//extern ROM BITMAP g_BmpCalendarTop;
extern ROM BITMAP g_BmpCalendar;
//extern ROM BITMAP g_BmpCalbox2; 
extern ROM BITMAP g_BmpSquareGray;
extern ROM BITMAP g_BmpSquareRedOn;
//extern ROM BITMAP g_BmpSquareBlacklineOn;
//extern ROM BITMAP g_BmpSquareBlackline;
//extern ROM BITMAP g_BmpMoonIcon;
//extern ROM BITMAP g_BmpSunIcon;

//extern ROM BITMAP g_BmpSquareGreenred

/* Mobile Wallet */
/*
extern ROM BITMAP g_BmpMWallet01;
extern ROM BITMAP g_BmpMWallet02;
extern ROM BITMAP g_BmpMWallet03;
extern ROM BITMAP g_BmpMWallet04;
extern ROM BITMAP g_BmpMWallet05;
extern ROM BITMAP g_BmpMWallet06;
extern ROM BITMAP g_BmpMWallet07;
extern ROM BITMAP g_BmpMWallet08;
extern ROM BITMAP g_BmpMWallet09;
extern ROM BITMAP g_BmpMWallet10;
extern ROM BITMAP g_BmpMWallet11;
extern ROM BITMAP g_BmpMWallet12;
extern ROM BITMAP g_BmpMWallet13;
extern ROM BITMAP g_BmpMWalletPannelZipCode;                      //우편번호
extern ROM BITMAP g_BmpMWalletPannelTermOfValidity;               //유효기간
extern ROM BITMAP g_BmpMWalletPannelResidentRegistrationNumber;   //주민등록번호
extern ROM BITMAP g_BmpMWalletPannelReceiverPhone;                //연락받을번호
extern ROM BITMAP g_BmpMWalletPannelAddress;                      //주소
extern ROM BITMAP g_BmpMWalletPannelPhoneNumber;                  //전화번호
extern ROM BITMAP g_BmpMWalletPannelPassWord;                     //비밀번호
extern ROM BITMAP g_BmpMWalletPannelOKCashbagPassWord;            //비밀번호(OK Cashbag)
extern ROM BITMAP g_BmpMWalletPannelOKCardNumber;                 //카드번호
extern ROM BITMAP g_BmpMWalletPannelName;                         //이름
extern ROM BITMAP g_BmpMWalletPannelEMail;                        //전자우편
extern ROM BITMAP g_BmpMWalletPannelCreditCardNum;                //신용카드번호
extern ROM BITMAP g_BmpMWalletPannelReceiverName;                 //수령인이름
extern ROM BITMAP g_BmpMWalletPannelCertUsage;                    //인증서용도(개인인증서)
extern ROM BITMAP g_BmpMWalletPannelCertTermOfValidity;           //유효기간(개인인증서)
extern ROM BITMAP g_BmpMWalletPannelCertName;                     //인증기관(개인인증서)
extern ROM BITMAP g_BmpMWalletPannelCertEtc;                      //기타(개인인증서)
extern ROM BITMAP g_BmpMWalletPannelUserID;                       //사용자ID
*/
/* SMS */
//extern ROM BITMAP g_BmpSMSIcon01;
//extern ROM BITMAP g_BmpSMSIcon02;
//extern ROM BITMAP g_BmpSMSIcon03;

/* Lock */
//extern ROM BITMAP g_BmpSecretNumbericon;

/* 기타 */
extern ROM BITMAP g_BmpHorizontalLine;
extern ROM BITMAP g_BmpSandglass;
extern ANIMATION_INFO_T    *g_pBuiltInAnimation[BUILT_IN_ANI_COUNT];

// old
extern ROM BITMAP g_BmpSecret120x106x256c;
extern ROM BITMAP g_BmpRGB120x91x256c; 

//extern BITMAP g_BmpBuf;
extern BYTE g_abBmpBuf[];//SKY_SCREEN_BUFFER_WIDTH*SKY_SCREEN_BUFFER_HEIGHT];

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API SKY_SetBuiltInAnimation(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_DATA_H_ */
