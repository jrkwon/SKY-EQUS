/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_data.c
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
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#define		FEATURE_USE_LZB // NKHA 2001.11.07

#include "sky.h"

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

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
/* main annunciator image */
#include "AnnunAlarm.EIS"
#include "AnnunAnsweringMachine.EIS"
#include "AnnunBattery.EIS"
#include "AnnunBellVibration.EIS"
#include "AnnunCall.EIS"
#include "AnnunMessage.EIS"
#include "AnnunProtocol.EIS"
#include "AnnunRoamRSSI.EIS"
#include "AnnunRSSI.EIS"

/* sub annunciator image */
#include "SubAnnunAlarm.EIS"
#include "SubAnnunAnsweringMachine.EIS"
#include "SubAnnunBattery.EIS"
#include "SubAnnunBellVibration.EIS"
#include "SubAnnunMessage.EIS"
#include "SubAnnunProtocol.EIS"
#include "SubAnnunRoamRSSI.EIS"
#include "SubAnnunRSSI.EIS"

/* command line image - 4gray */
#include "CommandLineCenter.EIS"
#include "CommandLineLeft.EIS"
#include "CommandLineRight.EIS"

/* idle command line image - color */
#include "CommandlineIdleBlueI.EIS"
#include "CommandlineIdleBlueT.EIS"
#include "CommandlineIdleGreenI.EIS"
#include "CommandlineIdleGreenT.EIS"
#include "CommandlineIdleRedI.EIS"
#include "CommandlineIdleRedT.EIS"
#include "CommandlineIdleDeepBlueI.EIS"
#include "CommandlineIdleDeepBlueT.EIS"
#include "CommandlineIdleVioletI.EIS"
#include "CommandlineIdleVioletT.EIS"
#include "CommandlineIdleBrownI.EIS"
#include "CommandlineIdleBrownT.EIS"


/* WAP command line image */
#include "WapCommandLineCenter.EIS"
#include "WapCommandLineCenterArrow.EIS"
#include "WapCommandLineCenterOK.EIS"
#include "WapCommandLineLeft.EIS"
#include "WapCommandLineRight.EIS"

/* auto answer */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.06	    
#include "AutoAnswerDrive.ZAS"
#include "AutoAnswerGeneral.ZAS"
#include "AutoAnswerMeeting.ZAS"
#include "AutoAnswerUser.ZAS"
#else
#include "AutoAnswerDrive.XAS"
#include "AutoAnswerGeneral.XAS"
#include "AutoAnswerMeeting.XAS"
#include "AutoAnswerUser.XAS"
#endif
//#include "DateTimePannel.XIS"
#include "AutoAnswerPannelAutoAnswer.EIS"
#include "AutoAnswerPannelNewAutoAnswer.EIS"

/* system - no compression */
#include "BootLogoMain.EIS"
#include "BootProgressBar.EIS"
#include "BootLogoSub.EIS"

#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07	    
#include "Lock.ZIS"
//#include "GeneralPannel.XIS"
//#include "StartingSun.XAS"
#include "Calling.ZAS"
#include "EndingCloud.ZAS"
#include "StartingWater.ZAS"
#include "Speed011.ZAS"
//#include "EndingGoodBye.XAS"
//#include "PowerDigital017.XAS"
#include "SkyLogo.ZAS"

/* GVM */
#include "DownloadGVM.ZAS"
#else
#include "Lock.XIS"
//#include "GeneralPannel.XIS"
//#include "StartingSun.XAS"
#include "Calling.XAS"
#include "EndingCloud.XAS"
#include "StartingWater.XAS"
#include "Speed011.XAS"
//#include "EndingGoodBye.XAS"
//#include "PowerDigital017.XAS"
#include "SkyLogo.XAS"

/* GVM */
#include "DownloadGVM.XAS"
#endif
/* IrDA */
#include "ExchangeDataIrDA.EIS"
#include "IrdaPannel.XIS"

/* phone book */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07	    
#include "FindData.ZIS"
#include "PBookPannel.ZIS"
#else
#include "FindData.XIS"
#include "PBookPannel.XIS"
#endif

/* call */
#include "VolumeInCall.EIS"
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "HoldOnCall.ZIS"
#include "InDataService.ZAS"
#include "MissedCall.ZAS"
#include "OriginatingCall.XAS"
#include "PhoneOpen.XIS"
#else  
#include "HoldOnCall.XIS"
#include "InDataService.XAS"
#include "MissedCall.XAS"
#include "OriginatingCall.XAS"
#include "PhoneOpen.XIS"
#endif
#include "CallPannelMissedCall.EIS"
#include "CallPannelVolumeInCall.EIS"
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "CallPannel.ZIS"
#else
#include "CallPannel.XIS"
#endif

/* record/play */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "Playing.ZAS"
#include "Record.ZIS"
//#include "Recording.EAS"
#include "Recording.ZAS"
//#include "RemainTimePannel.ZIS"
#else
#include "Playing.XAS"
#include "Record.XIS"
//#include "Recording.EAS"
#include "Recording.XAS"
//#include "RemainTimePannel.XIS"
#endif
#include "RecordPannelRecordVoice.EIS"
#include "RecordPannelStartRecord.EIS"
//#include "RecordPannelVoiceMemo.EIS"

/* email */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "ReceiveEMail.ZAS"
#include "SendEMail.ZAS"
#include "ConnectDataService.ZIS"
#else
#include "ReceiveEMail.XAS"
#include "SendEMail.XAS"
#include "ConnectDataService.XIS"
#endif

/* SMS */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "ReceiveMessage.ZAS"
#include "SendMessage.ZAS"
#else
#include "ReceiveMessage.XAS"
#include "SendMessage.XAS"
#endif
#include "CallPannelTransMessage.EIS"
//#include "CallPannelNewMessage.EIS"

/* idle */
#include "BigAMPM.EIS"
#include "BigWatchNumber.EIS"
//#include "NewRecordCount.EIS"
#include "RecordCount.EIS"
#include "SmallAMPM.EIS"
#include "SmallWatchNumber.EIS"
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "UserMentRecord.ZIS"
#else
#include "UserMentRecord.XIS"
#endif
#include "WatchPannel.EIS"
//#include "IdleSmall.XIS"
//#include "IdleBig.XAS"
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "AutoAnswerPannelNew.ZIS"
#else
#include "AutoAnswerPannelNew.XIS"
#endif
#include "AutoAnswerPannelDrive.EIS"
#include "AutoAnswerPannelGeneral.EIS"
#include "AutoAnswerPannelUser.EIS"
#include "AutoAnswerPannelMeeting.EIS"

//#include "StandbyBig-back.XIS"
//#include "StandbyBig.XIS"
//#include "StandbyBig-back16.XIS"
//#include "StandbyBig16.XIS"
#include "Clock01.EIS"
#include "Clock02.EIS"


/* stopwatch */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "RunningCheetah.ZAS"
#include "StopWatchPannel.ZIS"
#else
#include "RunningCheetah.XAS"
#include "StopWatchPannel.XIS"
#endif

/* alarm */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "Anniversary.ZAS"
#include "GeneralAlarm.ZAS"
#else
#include "Anniversary.XAS"
#include "GeneralAlarm.XAS"
#endif

/* calculator */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "CalCulator.ZIS"
#include "CalCulatorOutputBox.ZIS"
#else
#include "CalCulator.XIS"
#include "CalCulatorOutputBox.XIS"
#endif
#include "CalCulatorSym.EIS"

/* main menu */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "MenuAutoAnswer.ZAS"
#include "MenuCallingTime.ZAS"
#include "MenuDataComm.ZAS"
#include "MenuElecBook.ZAS"
#include "MenuEnvironSett.ZAS"
#include "MenuGameGVM.ZAS"
#include "MenuIrDA.ZAS"
#include "MenuScreenSett.ZAS"
#include "MenuSoundSett.ZAS"
#include "MainMenuBase.ZIS"
#else
#include "MenuAutoAnswer.ZAS"
#include "MenuCallingTime.ZAS"
#include "MenuDataComm.ZAS"
#include "MenuElecBook.ZAS"
#include "MenuEnvironSett.ZAS"
#include "MenuGameGVM.ZAS"
#include "MenuIrDA.ZAS"
#include "MenuScreenSett.ZAS"
#include "MenuSoundSett.ZAS"
#include "MainMenuBase.ZIS"
#endif

/* pbook menu */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "Phone011plus.ZAS"
#include "PhoneShortening.ZAS"
#include "PhoneGroupEdit.ZAS"
#include "PhoneName.ZAS"
#include "PhoneNumber.ZAS"
#include "PhoneSelf.ZAS"
#include "PhoneNumberInput.ZAS"
#include "PhoneSpeed.ZAS"
#include "PBookMenuBase.ZIS"
#else
#include "Phone011plus.XAS"
#include "PhoneShortening.XAS"
#include "PhoneGroupEdit.XAS"
#include "PhoneName.XAS"
#include "PhoneNumber.XAS"
#include "PhoneSelf.XAS"
#include "PhoneNumberInput.XAS"
#include "PhoneSpeed.XAS"
#include "PBookMenuBase.XIS"
#endif

/* SMS menu */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "SMSSendingBox.ZAS"
#include "SMSMenuBase.ZIS"
#include "SMSReceiptBox.ZAS"
#include "SMSReceiptAlarm.ZAS"
#include "SMSNTopMail.ZAS"
#include "SMSEmail.ZAS"
#include "SMSDelete.ZAS"
//#include "SMSLocationTrace.ZAS"
#else
#include "SMSSendingBox.XAS"
#include "SMSMenuBase.XIS"
#include "SMSReceiptBox.XAS"
#include "SMSReceiptAlarm.XAS"
#include "SMSNTopMail.XAS"
#include "SMSEmail.XAS"
#include "SMSDelete.XAS"
//#include "SMSLocationTrace.XAS"
#endif

/* WorldTime */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "WorldMap.ZIS"                 
#include "WTimeMoon.ZIS"                 
#include "WTimeSun.ZIS"  
#else
#include "WorldMap.XIS"                 
#include "WTimeMoon.XIS"                 
#include "WTimeSun.XIS"  
#endif

/* Wap */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "Wap.ZAS"
#else
#include "Wap.XAS"
#endif

/* Calender */
#include "SquareBorder.EIS"
#include "SquareBorderBlack.EIS"
#include "SquareBlack.EIS"  
#include "SquareWhite.XIS"  
//#include "Calbox1.EIS"  
//#include "Calbox2.EIS"  
#ifndef FEATURE_USE_LZB	// NKHA 2001.11.07
//#include "CalendarTop.ZIS"  
#include "SquareGray.ZIS"  
#else
//#include "CalendarTop.XIS"  
#include "SquareGray.XIS"  
#endif
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
//#include "CalendarTop.ZIS"  
#include "SquareRed.ZIS"  
#else
//#include "CalendarTop.XIS"  
#include "SquareRed.XIS"  
#endif
#include "Calendar.EIS"  
#ifndef FEATURE_USE_LZB	// NKHA 2001.11.08
//#include "CalendarTop.ZIS"  
#include "SquareBlackOn.ZIS"
#else
//#include "CalendarTop.XIS"  
#include "SquareBlackOn.XIS"
#endif
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
//#include "SunIcon.ZIS"
//#include "MoonIcon.ZIS"
//#include "SquareBlacklineOn.ZIS"
//#include "SquareBlackline.ZIS"
#include "SquareThickBlacklineOn.ZIS"
#include "SquareThickBlackline.ZIS"
#include "SquareBlacredOn.ZIS"
#include "SearchCalendar.ZIS"
//#include "SquareGreenred.ZIS"
#else
//#include "SunIcon.XIS"
//#include "MoonIcon.XIS"
//#include "SquareBlacklineOn.XIS"
//#include "SquareBlackline.XIS"
#include "SquareThickBlacklineOn.XIS"
#include "SquareThickBlackline.XIS"
#include "SquareBlacredOn.XIS"
#include "SearchCalendar.XIS"
//#include "SquareGreenred.XIS"
#endif

/* Mobile Wallet */
//#include "MWallet01.XIS"
//#include "MWallet02.XIS"
//#include "MWallet03.XIS"
//#include "MWallet04.XIS"
//#include "MWallet05.XIS"
//#include "MWallet06.XIS"
//#include "MWallet07.XIS"
//#include "MWallet08.XIS"
//#include "MWallet09.XIS"
//#include "MWallet10.XIS"
//#include "MWallet11.XIS"
//#include "MWallet12.XIS"
//#include "MWallet13.XIS"
//#include "MWalletPannelCertName.XIS"
//#include "MWalletPannelCertTermOfValidity.XIS"
//#include "MWalletPannelCertUsage.XIS"
//#include "MWalletPannelCertEtc.XIS"
//#include "MWalletPannelPhoneNumber.XIS"
//#include "MWalletPannelName.XIS"
//#include "MWalletPannelEMail.XIS"
//#include "MWalletPannelZipCode.XIS"
//#include "MWalletPannelAddress.XIS"
//#include "MWalletPannelReceiverName.XIS"
//#include "MWalletPannelReceiverPhone.XIS"
//#include "MWalletPannelCreditCardNum.XIS"
//#include "MWalletPannelPassWord.XIS"
//#include "MWalletPannelTermOfValidity.XIS"
//#include "MWalletPannelResidentRegistrationNumber.XIS"
//#include "MWalletPannelOKCardNumber.XIS"
//#include "MWalletPannelOKCashbagPassWord.XIS"
//#include "MWalletPannelUserID.XIS"

/* SMS */
//#include "SMSIcon01.XIS"
//#include "SMSIcon02.XIS"
//#include "SMSIcon03.XIS"

/* Lock */
//#include "SecretNumbericon.XIS"

/* 기타 */
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "HorizontalLine.ZIS"
#else
#include "HorizontalLine.XIS"
#endif
#include "Sandglass.EIS"


// old
//#include "Secret120x106x256c.XIS"

#ifndef FEATURE_USE_LZB	// NKHA 2001.11.07
#include "RGB120x91x256c.ZIS"///////////////////6월 7일 추가 by happykid 
#else
#include "RGB120x91x256c.XIS"///////////////////6월 7일 추가 by happykid 
#endif

/* 
** BITMAP 구조체를 사용하는 카메라나 애니메이션 용 공용 변수
** 빈번한 메모리 할당/해제를 막기 위해 변수를 만들어 두었습니다.
*/
BYTE g_abBmpBuf[SKY_SCREEN_BUFFER_WIDTH*SKY_SCREEN_BUFFER_HEIGHT*PIXEL2BYTES(SKY_COLOR_SPACE)];


/*----------------------------------------------------------------------------
** 기본 제공 이미지(애니메이션 포함)
*/
/*
sky_img.h

enum tagBUILT_IN_IMAGE_E
{
    BII_NONE,
    BII_WATER_SKY,
    BII_GOODBYE,
    BII_RINGING,
    BII_WAP,
    BII_SPEED011,
    BII_SKY_LOGO
};

APPLET\PICTURE\ska_pic.c

CONST PICTURE_DATA_TYPE_T aPictureList[NUM_ANI_DATA] = {
	{TKN_PIC_SKY_EXPLODE,	BII_WATER_SKY},
	{TKN_PIC_ORIG_CALL,     BII_RINGING},
    {TKN_PIC_GOODBYE, 		BII_GOODBYE},
    {TKN_PIC_WAP,	    	BII_WAP},
    {TKN_PIC_SPEED011,    	BII_SPEED011} // BII_SKY_LOGO
};
*/

LOCAL ROM ANIMATION_INFO_T *l_aAnimationData011[] = {
    NULL,
    &g_AniStartingWater,   // 시작
    &g_AniEndingCloud,     // 종료
    &g_AniCalling,         // 착신
    &g_AniWap,             // WAP
    &g_AniSpeed011
};

LOCAL ROM ANIMATION_INFO_T *l_aAnimationData017[] = {
    NULL,
    &g_AniStartingWater,   // 시작
    &g_AniEndingCloud,     // 종료
    &g_AniCalling,         // 착신
    &g_AniWap,             // WAP
    &g_AniSkyLogo		  // SKY LOGO
};

ANIMATION_INFO_T *g_pBuiltInAnimation[BUILT_IN_ANI_COUNT];

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

void SKY_API SKY_SetBuiltInAnimation(void)
{
    UINT8 i;

    if(SKY_GET_SERVICE_PROVIDER() == SP_STI) /* 신세기 */
    {
        for(i = 0; i < BUILT_IN_ANI_COUNT; i++)
            g_pBuiltInAnimation[i] = (ANIMATION_INFO_T *)l_aAnimationData017[i];
    }
    else /* SK텔레콤 or SP_NONE */
    {
        for(i = 0; i < BUILT_IN_ANI_COUNT; i++)
            g_pBuiltInAnimation[i] = (ANIMATION_INFO_T *)l_aAnimationData011[i];
    }
}
