/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqs_mtxt.c
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
** ===========================================================================
*/ 

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#ifdef FEATURE_Q_PAGE
#define NUM_NAM_MENUITEM_MSM5000       8
#else
#define NUM_NAM_MENUITEM_MSM5000       7
#endif //FEATURE_Q_PAGE

#define NUM_NAM_SUBMENUITEM_MSM5000    7


#define NUM_SIMPLE_NAM_MENUITEM_MSM5000 5

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

////////////////////////////////////////////////////n0body 2001.6.15//////
//#include "MenuBase112x88x256c.XIS" ////메뉴 바탕화면					//
/*																		//
#include "Ediary46x36x256c.XAS"//전자수첩								//
#include "Speeker46x36x256c.XAS"//소리설정								//
#include "Screen46x36x256c.XAS"//화면설정								//
#include "Autorply46x36x256c.XAS"//자동응답								//
#include "Time46x36x256c.XAS"//통화시간									//
#include "Setup46x36x256c.XAS"//환경설정								//
#include "Game46x36x256c.XAS"//게임/GVM									//
#include "Data46x36x256c.XAS"//데이터 통신								//
#include "Irda46x36x256c.XAS"//적외선 통신								//
*/
																		//
//////////////////////////// A4 새로 바뀐 메뉴용 /////////////////////////


////////////////////////////////////////////////////n0body 2001.6.15//////
//#include "PbookBase112x88x256c.XIS" ////메뉴 바탕화면					//
																		//
/*
#include "Speed46x36x256c.XAS"//스피드 찾기								//
#include "Name46x36x256c.XAS"//이름으로 찾기							//
#include "Numb46x36x256c.XAS"//전화번호로 찾기 							//
#include "Pinput46x36x256c.XAS"//전화부 입력							//
#include "Plus46x36x256c.XAS"//011부가서비스 							//
#include "Sdial46x36x256c.XAS"//단축다이얼 편집							//
#include "Group46x36x256c.XAS"//그룹편집								//
#include "Myinfo46x36x256c.XAS"//자기정보 								//
*/																		//
//////////////////////////// A4 폰북 메뉴용 //////////////////////////////


//////////////////
//              //
//  Main Menu   //
//              //
//////////////////

/*
ROM MNODE_BODY_T bdMainMenuTop[] = {
    {ACCEL_KEY_1,       TKN_FACILITY,           &g_AniEdiary46x36x256c, FALSE,    (void *)&mnFacility},
    {ACCEL_KEY_2,       TKN_SET_AUDIO,          &g_AniSpeeker46x36x256c, FALSE,    (void *)&mnSetAudio},
    {ACCEL_KEY_3,       TKN_DISPLAY_SET,        &g_AniScreen46x36x256c, FALSE,    (void *)&mnDisplaySet},
    {ACCEL_KEY_4,       TKN_AUTO_ANSWER,        &g_AniAutorply46x36x256c, FALSE,    (void *)&mnAutoAnswer},
    {ACCEL_KEY_5,       TKN_CALL_USETIME,       &g_AniTime46x36x256c, FALSE,    (void *)&mnCallTimeBattery},
    {ACCEL_KEY_6,       TKN_CONTROL_PANNEL_TITLE,&g_AniSetup46x36x256c, FALSE,    (void *)&mnControlPannel},
    {ACCEL_KEY_7,       TKN_GAME,               &g_AniGame46x36x256c, FALSE,    (void *)&mnGame},
    {ACCEL_KEY_8,       TKN_DATA_COMM,          &g_AniData46x36x256c, FALSE,    (void *)&mnDataComm},
    {ACCEL_KEY_9,       TKN_IRDA,               &g_AniIrda46x36x256c, FALSE,    (void *)&mnIrDA},
};
*/


ROM MNODE_BODY_T bdMainMenuTop[] = {
    {ACCEL_KEY_1,       NULL,TKN_FACILITY,           &g_AniMenuElecBook, FALSE,    (void *)&mnFacility},
    {ACCEL_KEY_2,       NULL,TKN_SET_AUDIO,          &g_AniMenuSoundSett, FALSE,    (void *)&mnSetAudio},
    {ACCEL_KEY_3,       NULL,TKN_DISPLAY_SET,        &g_AniMenuScreenSett, FALSE,    (void *)&mnDisplaySet},
    {ACCEL_KEY_4,       NULL,TKN_AUTO_ANSWER,        &g_AniMenuAutoAnswer, FALSE,    (void *)&mnAutoAnswer},
    {ACCEL_KEY_5,       NULL,TKN_CALL_USETIME,       &g_AniMenuCallingTime, FALSE,    (void *)&mnCallTimeBattery},
    {ACCEL_KEY_6,       NULL,TKN_CONTROL_PANNEL_TITLE,&g_AniMenuEnvironSett, FALSE,    (void *)&mnControlPannel},
    {ACCEL_KEY_7,       NULL,TKN_GAME,               &g_AniMenuGameGVM, FALSE,    (void *)&mnGame},
    {ACCEL_KEY_8,       NULL,TKN_DATA_COMM,          &g_AniMenuDataComm, FALSE,    (void *)&mnDataComm},
    {ACCEL_KEY_9,       NULL,TKN_IRDA,               &g_AniMenuIrDA, FALSE,    (void *)&mnIrDA},
};


ROM MNODE_T mnMainMenuTop = { 
    NULL, // Parent
    9, // Mnode Line
	0, // Hidden Line
    TKN_MAIN_MENU_TOP_TITLE,
    bdMainMenuTop
};

//////////////////////////////////////////////////////////////////////////////
// 1. 전자수첩
ROM MNODE_BODY_T bdFacility[] = {
    {ACCEL_KEY_1,       NULL,TKN_SKY_SCHEDULER,      NULL, TRUE,   (void *)SKA_MainSchedule},
    {ACCEL_KEY_2,       NULL,TKN_ALARM,              NULL, TRUE,   (void *)SKA_MainAlarm},
    {ACCEL_KEY_3,       NULL,TKN_CALCULATOR,         NULL, TRUE,   (void *)SKA_MainCalculator},
    {ACCEL_KEY_4,       NULL,TKN_STOPWATCH,          NULL, TRUE,   (void *)SKA_MainStopWatch},
    {ACCEL_KEY_5,       NULL,TKN_WORLD_TIME,         NULL, TRUE,   (void *)SKA_MainWorldTime},
    {ACCEL_KEY_6,       NULL,TKN_MEMO,               NULL, TRUE,   (void *)SKA_MainMemoCode}
#ifdef FEATURE_MCOMMERCE
	, {ACCEL_KEY_7,     NULL,TKN_MWALLET,               NULL, TRUE,   (void *)SKA_MainMWallet}
#endif
};

ROM MNODE_T mnFacility = { 
    &mnMainMenuTop, // Parent
#ifdef FEATURE_MCOMMERCE
	7,
#else
    6, // Mnode Line
#endif 
	0, // Hidden Line
    TKN_FACILITY_TITLE,
    bdFacility
};

//////////////////////////////////////////////////////////////////////////////
// 2. 소리설정
ROM MNODE_BODY_T bdSetAudio[] = {
    {ACCEL_KEY_1,       NULL,TKN_BELL_VIB,           NULL, TRUE,   (void *)SKA_MainBellVibSetting},
    {ACCEL_KEY_2,       NULL,TKN_BELLVIB_GROUPBELL,  NULL, TRUE,   (void *)SKA_MainGroupBellSetting},
    {ACCEL_KEY_3,       NULL,TKN_BELLVIB_MYBELL,     NULL, TRUE,   (void *)SKA_MainMyBell},
    {ACCEL_KEY_4,       NULL,TKN_VOLUME_SET,         NULL, TRUE,   (void *)&SKA_MainVolumeSetting},
    {ACCEL_KEY_5,       NULL,TKN_BELLVIB_WARNING,    NULL, TRUE,   (void *)SKA_MainWarningSoundSetting}
};

ROM MNODE_T mnSetAudio = { 
    &mnMainMenuTop, // Parent
    5, // Mnode Line
	0, // Hidden Line
    TKN_SET_AUDIO,
    bdSetAudio
};


//////////////////////////////////////////////////////////////////////////////
// 3. 화면설정
ROM MNODE_BODY_T bdDisplaySet[] = {
    {ACCEL_KEY_1,       NULL,TKN_PICTURE_IDLE,       NULL, TRUE,   (void *)SKA_MainIdleDisplaySet},
    {ACCEL_KEY_2,       NULL,TKN_PICTURE_SET,        NULL, TRUE,   (void *)SKA_MainPictureSet},
    {ACCEL_KEY_3,       NULL,TKN_PICTURE_MYANI,      NULL, TRUE,   (void *)SKA_MainMyAnimation},
    {ACCEL_KEY_4,       NULL,TKN_PICTURE_MYPHOTO,    NULL, TRUE,   (void *)SKA_PhotoList},
    {ACCEL_KEY_5,       NULL,TKN_LCD_BRIGHTNESS,     NULL, TRUE,   (void *)SKA_MainBright},
    {ACCEL_KEY_6,       NULL,TKN_BACKLIGHT_SET,      NULL, TRUE,   (void *)SKA_MainAutoBacklight},
    {ACCEL_KEY_7,       NULL,TKN_THEME_TITLE,        NULL, TRUE,   (void *)SKA_MainSetTheme}
};

ROM MNODE_T mnDisplaySet = { 
    &mnMainMenuTop, // Parent
    7, // Mnode Line
	0, // Hidden Line
    TKN_DISPLAY_SET,
    bdDisplaySet
};

/* 0000000000000000
ROM MNODE_BODY_T bdBacklightSet[] = {
    {ACCEL_KEY_1,       TKN_SEL_BACKLIGHT, NULL, TRUE,   (void *)SKA_MainAutoBacklight}
//    {ACCEL_KEY_2,       TKN_AUTO_IDLCD_BACKLIGHT,NULL, TRUE,  (void *)SKA_MainAutoSubLCDBacklight},
#if ((SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2))
    ,{ACCEL_KEY_2,       TKN_IDLCD_BACKLIGHT_COLOR,NULL, TRUE, (void *)SKA_SubLcdColorSel},
    {ACCEL_KEY_3,       TKN_GROUP_BACKLIGHT_COLOR,   NULL, TRUE,   (void *)SKA_GroupLcdSel}
#endif
};

ROM MNODE_T mnBacklightSet = { 
    &mnDisplaySet, // Parent
#if ((SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2))
    3,//4, // Mnode Line
#else
    1,//2,
#endif
	0, // Hidden Line
    TKN_DISPLAY_SET,
    bdBacklightSet
};
000000000000000 */

//////////////////////////////////////////////////////////////////////////////
// 4. 자동응답
ROM MNODE_BODY_T bdAutoAnswer[] = {
    {ACCEL_KEY_1,       NULL,TKN_AUTO_ANSWER_SET,    NULL, TRUE,   (void *)SKA_SetAutoAnswer},
    {ACCEL_KEY_2,       NULL,TKN_USER_MENT_RECORD,   NULL, TRUE,   (void *)SKA_RECPLAYUserMent}
};

ROM MNODE_T mnAutoAnswer = { 
    &mnMainMenuTop, // Parent
    2, // Mnode Line
	0, // Hidden Line
    TKN_AUTO_ANSWER_TITLE,
    bdAutoAnswer
};

//////////////////////////////////////////////////////////////////////////////
// 5. 통화시간/배터리
ROM MNODE_BODY_T bdCallTimeBattery[] = {
    {ACCEL_KEY_1, NULL,TKN_CALL_LASTCALL, NULL, TRUE, (void *)SKA_MainLastCallInfo},
	{ACCEL_KEY_2, NULL,TKN_CALL_CUMMULATIVE,  NULL, TRUE,   (void *)SKA_MainCumulativeCallInfo},
	{ACCEL_KEY_3, NULL,TKN_CALL_TOTAL, NULL, TRUE,  (void *)SKA_MainTotalCallInfo}
#ifdef FEATURE_SMART_BATTERY
    ,{ACCEL_KEY_4, NULL,TKN_SMART_BATTERY_INFO,  NULL, TRUE,   (void *)SKA_MainBatteryInfo}
#endif
};

ROM MNODE_T mnCallTimeBattery = { 
    &mnMainMenuTop, // Parent
#ifdef FEATURE_SMART_BATTERY
    4, // Mnode Line]
#else
    3,
#endif
	0, // Hidden Line
    TKN_CALL_USETIME,//TKN_CALLTIME_BATTERY,
    bdCallTimeBattery
};

//////////////////////////////////////////////////////////////////////////////
// 6. 환경설정
ROM MNODE_BODY_T bdControlPannel[] = {
    {ACCEL_KEY_1,       NULL,TKN_AUTO_FUNCTION,          NULL, TRUE,   (void *)SKA_MainAutoFnc},
    {ACCEL_KEY_2,       NULL,TKN_LOCK,                   NULL, TRUE,   (void *)SKA_MainLock},
    {ACCEL_KEY_3,       NULL,TKN_PASSWORD_CHANGE,        NULL, TRUE,   (void *)SKA_PasswordChange},
    {ACCEL_KEY_4,       NULL,TKN_OPTION_SELECT,          NULL, TRUE,   (void *)SKA_MainConfigSet},
    {ACCEL_KEY_5,       NULL,TKN_TWO_NUM_SRVC_SET,       NULL, TRUE,   (void *)SKA_MainTwoNum},
    {ACCEL_KEY_6,       NULL,TKN_NTOP_SET,               NULL, TRUE,   (void *)SKA_MainNtopSet},
    {ACCEL_KEY_7,       NULL,TKN_NAM_SET,                NULL, TRUE,   (void *)SKA_MainNAMSelect},
    {ACCEL_KEY_8,       NULL,TKN_SW_VERSION,             NULL, TRUE,   (void *)SKA_MainVersion},    
	{ACCEL_KEY_9,       NULL,TKN_PBOOK_DATA_INIT,        NULL, TRUE,   (void *)SKA_InitPBookData},	
	{ACCEL_KEY_POUND,   NULL,TKN_SYSTEM_INIT,            NULL, TRUE,   (void *)SKA_MainSysInit},
    {ACCEL_KEY_0,       NULL,TKN_TEST_MENU,              NULL, TRUE,   (void *)SKA_SeviceMenu} // hidden menu 항상 끝에 있어야 함.
};

ROM MNODE_T mnControlPannel = { 
    &mnMainMenuTop, // Parent
    10,//4,//,6,//8,//12, // Mnode Line
	1, // Hidden Line
    TKN_CONTROL_PANNEL_TITLE,
    bdControlPannel
};

ROM MNODE_BODY_T bdSystemSetting[] = {
    {ACCEL_KEY_1,       NULL,TKN_PASSWORD_CHANGE,        NULL, TRUE,   (void *)SKA_PasswordChange},
    {ACCEL_KEY_2,       NULL,TKN_LOCK,                   NULL, TRUE,   (void *)SKA_MainLock},
    {ACCEL_KEY_3,       NULL,TKN_TWO_NUM_SRVC_SET,       NULL, TRUE,   (void *)SKA_MainTwoNum},
    {ACCEL_KEY_4,       NULL,TKN_NTOP_SET,               NULL, TRUE,   (void *)SKA_MainNtopSet},
    {ACCEL_KEY_5,       NULL,TKN_NAM_SET,                NULL, TRUE,   (void *)SKA_MainNAMSelect},
    {ACCEL_KEY_6,       NULL,TKN_PBOOK_DATA_INIT,        NULL, TRUE,   (void *)SKA_InitPBookData},
    {ACCEL_KEY_7,       NULL,TKN_SYSTEM_INIT,            NULL, TRUE,   (void *)SKA_MainSysInit}
};

ROM MNODE_T mnSystemSetting = { 
    &mnControlPannel, // Parent
    7,
	0, // Hidden Line
    TKN_SYSTEM_SET,
    bdSystemSetting
};

//////////////////////////////////////////////////////////////////////////////
// 7. 게임
ROM MNODE_BODY_T bdGame[] = {
    {ACCEL_KEY_1,       NULL,TKN_APACHE,             NULL, TRUE,   (void *)SKA_MainApache},
    {ACCEL_KEY_2,       NULL,TKN_POLAR,              NULL, TRUE,   (void *)SKA_MainPolar},
    {ACCEL_KEY_3,       NULL,TKN_MOON_PATROL,        NULL, TRUE,   (void *)SKA_Mars},
    {ACCEL_KEY_4,       NULL,TKN_SAMTRIS,            NULL, TRUE,   (void *)SKA_MainSamtris},
    {ACCEL_KEY_5,       NULL,TKN_PUZZLE,             NULL, TRUE,   (void *)SKA_MainPuzzle},
#ifdef FEATURE_SKVM
    {ACCEL_KEY_6,       NULL,TKN_GVM_TITLE,          NULL, TRUE,   (void *)SKA_MainSKVM}
#else
#ifdef FEATURE_GVM
    {ACCEL_KEY_6,       NULL,TKN_GVM_TITLE,          NULL, TRUE,   (void *)SKA_MainGVM}
#endif // FEATURE_GVM
#endif // FEATURE_SKVM
};

ROM MNODE_T mnGame = { 
    &mnMainMenuTop, // Parent
#ifdef FEATURE_GVM
    6, // Mnode Line
#else
    5, // Mnode Line
#endif //FEATURE_GVM
	0, // Hidden Line
    TKN_GAME_TITLE,
    bdGame
};

//////////////////////////////////////////////////////////////////////////////
// 8. 데이터 통신
ROM MNODE_BODY_T bdDataComm[] = {
    {ACCEL_KEY_1,       NULL,TKN_DATA_SERVICE_SET,   NULL, TRUE,   (void *)&SKA_MainDataServiceSet},
    {ACCEL_KEY_2,       NULL,TKN_AIR_DATA_MANAGER, NULL, FALSE,   (void *)&mnAirDataMgr}
};

ROM MNODE_T mnDataComm = { 
    &mnMainMenuTop, // Parent
    2,//4, // Mnode Line
	0, // Hidden Line
    TKN_DATA_SERVICE_TITLE,
    bdDataComm
};

//////////////////////////// Data Service Menu Implementation. ///////////////////////////
ROM MNODE_BODY_T bdAirDataMgr[3] = {
    {ACCEL_KEY_1,       NULL,TKN_AIR_DATAMGR_SEND,  NULL, TRUE,   (void *)SKA_AirDmgrSend},
    {ACCEL_KEY_2,       NULL,TKN_AIR_DATAMGR_GET,   NULL, TRUE,   (void *)SKA_AirDmgrGet},
    {ACCEL_KEY_3,       NULL,TKN_IP_AIR_DATAMGR,    NULL, TRUE,   (void *)SKA_IPDmgr}
};

ROM MNODE_T mnAirDataMgr = { 
    &mnDataComm, // Parent
    3, // Mnode Line
	0, // Hidden Line
    TKN_AIR_DATA_MANAGER,
    bdAirDataMgr
};

//////////////////////////////////////////////////////////////////////////////
// 9. 적외선 통신
ROM MNODE_BODY_T bdIrDA[2] = {
    {ACCEL_KEY_1,       NULL,TKN_PHONE_TO_PHONE,     NULL, FALSE,   (void *)&mnIrDAPhone},
    {ACCEL_KEY_2,       NULL,TKN_PHONE_TO_PC,        NULL, TRUE,    (void *)SKA_MainIrDAPC}
};

ROM MNODE_T mnIrDA = { 
    &mnMainMenuTop, // Parent
    2, // Mnode Line
	0, // Hidden Line
    TKN_IRDA_TITLE,
    bdIrDA
};
#ifdef FEATURE_USE_IRDA
ROM MNODE_BODY_T bdIrDAPhone[] = {
    {ACCEL_KEY_1,       NULL,TKN_PHONE_TO_PHONE_RECEIVE,     NULL, TRUE,   (void *)SKA_MainIrDAPhoneReceive},
    {ACCEL_KEY_2,       NULL,TKN_PHONE_TO_PHONE_BELL,        NULL, TRUE,   (void *)SKA_MainIrDAMelody},
    {ACCEL_KEY_3,       NULL,TKN_PHONE_TO_PHONE_ANIMATION,   NULL, TRUE,   (void *)SKA_MainIrDAAnimation},
    {ACCEL_KEY_4,       NULL,TKN_PHONE_TO_PHONE_PHOTO,       NULL, TRUE,   (void *)SKA_MainIrDAPhoto},
    {ACCEL_KEY_5,       NULL,TKN_PHONE_TO_PHONE_BOOK,        NULL, TRUE,   (void *)SKA_MainPbookCodeInIrda}
};
#else
ROM MNODE_BODY_T bdIrDAPhone[] = {
    {ACCEL_KEY_1,       NULL,TKN_PHONE_TO_PHONE_RECEIVE,     NULL, TRUE,   (void *)SKA_MainNotYet},
    {ACCEL_KEY_2,       NULL,TKN_PHONE_TO_PHONE_BELL,        NULL, TRUE,   (void *)SKA_MainNotYet},
    {ACCEL_KEY_3,       NULL,TKN_PHONE_TO_PHONE_ANIMATION,   NULL, TRUE,   (void *)SKA_MainNotYet},
    {ACCEL_KEY_4,       NULL,TKN_PHONE_TO_PHONE_BOOK,        NULL, TRUE,   (void *)SKA_MainNotYet}
};
#endif

ROM MNODE_T mnIrDAPhone = { 
    &mnIrDA, // Parent
    5, // Mnode Line
	0, // Hidden Line
    TKN_PHONE_TO_PHONE,
    bdIrDAPhone
};

ROM MNODE_BODY_T bdIrDABook[] = {
#ifdef FEATURE_USE_IRDA
    {ACCEL_KEY_1,       NULL,TKN_PHONE_TO_PHONE_SEND_ALL,    NULL, TRUE,   (void *)SKA_MainIrDABookAll},
    {ACCEL_KEY_2,       NULL,TKN_PHONE_TO_PHONE_SEND_GROUP,  NULL, TRUE,   (void *)SKA_MainIrDABookGroup},
    {ACCEL_KEY_3,       NULL,TKN_PHONE_TO_PHONE_SEND_NAME,   NULL, TRUE,   (void *)SKA_MainIrDABookName},
    {ACCEL_KEY_4,       NULL,TKN_PHONE_TO_PHONE_SEND_SDIAL,  NULL, TRUE,   (void *)SKA_MainIrDABookSDial},
    {ACCEL_KEY_5,       NULL,TKN_PHONE_TO_PHONE_SEND_MYINFO,  NULL, TRUE,  (void *)SKA_MainMySelfInfoCode}//SKA_MainIrDABookMyInfo}
#else
    {ACCEL_KEY_1,       NULL,TKN_PHONE_TO_PHONE_SEND_ALL,    NULL, TRUE,   (void *)SKA_MainNotYet},
    {ACCEL_KEY_2,       NULL,TKN_PHONE_TO_PHONE_SEND_GROUP,  NULL, TRUE,   (void *)SKA_MainNotYet},
    {ACCEL_KEY_3,       NULL,TKN_PHONE_TO_PHONE_SEND_NAME,   NULL, TRUE,   (void *)SKA_MainNotYet},
    {ACCEL_KEY_4,       NULL,TKN_PHONE_TO_PHONE_SEND_SDIAL,  NULL, TRUE,   (void *)SKA_MainNotYet}
#endif
};

ROM MNODE_T mnIrDABook = { 
    &mnIrDAPhone, // Parent
#if MODEL_ID == MODEL_B1 || (MODEL_ID == MODEL_B2)
    5,
#else
    4, // Mnode Line
#endif
	0, // Hidden Line
    TKN_PHONE_TO_PHONE_BOOK,
    bdIrDABook
};
//#define WAP_TEST
//////////////////////////////////////////////////////////////////////////////
// 0. 전자우편
ROM MNODE_BODY_T bdEMail[] = {
    {ACCEL_KEY_1,       NULL,TKN_EMAIL_IN_BOX,             NULL, TRUE,   (void *)SKA_MainMailInBox},
    {ACCEL_KEY_2,       NULL,TKN_EMAIL_SENT_BOX,           NULL, TRUE,   (void *)SKA_MainMailOutBox},
    {ACCEL_KEY_3,       NULL,TKN_EMAIL_EDIT,               NULL, TRUE,   (void *)SKA_MainMailSend},
    {ACCEL_KEY_4,       NULL,TKN_EMAIL_ACCOUNT_SET,        NULL, TRUE,   (void *)SKA_MainEMailAccount}
};

ROM MNODE_T mnEMail = { 
    &mnMainMenuTop, // Parent
    4,
	0, // Hidden Line
    TKN_EMAIL_TITLE,
    bdEMail
};


ROM MNODE_BODY_T bdCamera[] = {
    {ACCEL_KEY_1,       NULL,TKN_TAKE_PHOTO,                     NULL, TRUE,   (void *)SKA_MainCamera},
#ifdef FEATURE_SKTT_PM    
    {ACCEL_KEY_2,       NULL,TKN_PM_PHOTO_MAIL,                  NULL, TRUE,   (void *)SKA_MainPMList},
    {ACCEL_KEY_3,       NULL,TKN_PICTURE_MYPHOTO,                NULL, TRUE,   (void *)SKA_PhotoList}
#else
    {ACCEL_KEY_2,       NULL,TKN_PICTURE_MYPHOTO,                NULL, TRUE,   (void *)SKA_PhotoList}
#endif
};

ROM MNODE_T mnCamera = { 
    NULL, //&mnMainMenuTop, // Parent
#ifdef FEATURE_SKTT_PM    
    3,  // Mnode Line
#else
    2,
#endif
	0, // Hidden Line
    TKN_CAMERA,
    bdCamera
};

//////////////////////////////////////////////////////////////////////////////
// 전화부
/*
ROM MNODE_BODY_T bdPhoneBook[] = {
    {ACCEL_KEY_1,       TKN_SPEED_SEARCH,           &g_AniSpeed46x36x256c, TRUE,   (void *)SKA_SpeedSearch},
    {ACCEL_KEY_2,       TKN_NAME_SEARCH,            &g_AniName46x36x256c, TRUE,   (void *)SKA_SearchName},
    {ACCEL_KEY_3,       TKN_NUMBER_SEARCH,          &g_AniNumb46x36x256c, TRUE,   (void *)SKA_SearchNumber},
    {ACCEL_KEY_4,       TKN_PHONE_BOOK_SAVE,        &g_AniPinput46x36x256c, TRUE,   (void *)SKA_BookInput},
    {ACCEL_KEY_5,       TKN_011_SERVICE_SET,        &g_AniPlus46x36x256c, TRUE,   (void *)SKA_MainMenu011Service},//&mn011Srvc},               //white 00.12.19
    {ACCEL_KEY_6,       TKN_EDIT_SHORT_DIAL,        &g_AniSdial46x36x256c, TRUE,   (void *)SKA_EditSDial},
    {ACCEL_KEY_7,       TKN_GROUP_EDIT,             &g_AniGroup46x36x256c, TRUE,   (void *)SKA_EditGroup},
    {ACCEL_KEY_POUND,   TKN_PBOOK_PERSON_INFO,      &g_AniMyinfo46x36x256c, TRUE,   (void *)SKA_MainSelfInfoCode4View}//SKA_ViewPersonInfo},
};
*/
// 전화부
ROM MNODE_BODY_T bdPhoneBook[] = {
    {ACCEL_KEY_1,       NULL,TKN_SPEED_SEARCH,           &g_AniPhoneSpeed, TRUE,   (void *)SKA_SpeedSearch},
    {ACCEL_KEY_2,       NULL,TKN_NAME_SEARCH,            &g_AniPhoneName, TRUE,   (void *)SKA_SearchName},
    {ACCEL_KEY_3,       NULL,TKN_NUMBER_SEARCH,          &g_AniPhoneNumber, TRUE,   (void *)SKA_SearchNumber},
    {ACCEL_KEY_4,       NULL,TKN_PHONE_BOOK_SAVE,        &g_AniPhoneNumberInput, TRUE,   (void *)SKA_BookInput},
    {ACCEL_KEY_5,       NULL,TKN_011_SERVICE_SET,        &g_AniPhone011plus, TRUE,   (void *)SKA_MainMenuService},//&mn011Srvc},               //white 00.12.19
    {ACCEL_KEY_6,       NULL,TKN_EDIT_SHORT_DIAL,        &g_AniPhoneShortening, TRUE,   (void *)SKA_EditSDial},
    {ACCEL_KEY_7,       NULL,TKN_GROUP_EDIT,             &g_AniPhoneGroupEdit, TRUE,   (void *)SKA_EditGroup},
    {ACCEL_KEY_POUND,   NULL,TKN_PBOOK_PERSON_INFO,      &g_AniPhoneSelf, TRUE,   (void *)SKA_MainSelfInfoCode4View}//SKA_ViewPersonInfo},
};

ROM MNODE_T mnPhoneBook = { 
    NULL, //&mnMainMenuTop, // Parent
    8,//9, // Mnode Line
	0, // Hidden Line
    TKN_PHONE_BOOK_TITLE,
    bdPhoneBook
};



//extern ROM ANIMATION_INFO_T g_AniSMSReceiptAlarm;

ROM MNODE_BODY_T bdSMSMenu1[] = {
    {ACCEL_KEY_1,       (BYTE*)TKN_SMS_IN_BOX,TKN_SMS_IN_BOX,           &g_AniSMSReceiptBox, TRUE,   (void *)SKA_ReceiveMsg},
    {ACCEL_KEY_2,       NULL,TKN_SMS_SENT_BOX,            &g_AniSMSSendingBox, TRUE,   (void *)SKA_SendMsg},
    {ACCEL_KEY_3,       NULL,TKN_SMS_MENU4,            &g_AniSMSReceiptAlarm, TRUE,   (void *)SKA_MsgAlarm},
    {ACCEL_KEY_4,       NULL,TKN_SMS_MENU5,        &g_AniSMSDelete, TRUE,   (void *)SKA_DeleteAllMsgCode},
    {ACCEL_KEY_5,       NULL,TKN_SMS_MENU6,        &g_AniSMSEmail, TRUE,   (void *)SKA_EnterEmailMenu},//&mn011Srvc},               //white 00.12.19
#ifdef FEATURE_WAP
    {ACCEL_KEY_6,       NULL,TKN_NTOP_MAIL_TITLE,        &g_AniSMSNTopMail, TRUE,   (void *)SKA_SendWAPMsg}
#endif //FEATURE_WAP
};
ROM MNODE_T mnSMSMenu1 = { 
    NULL, //&mnMainMenuTop, // Parent
#ifdef FEATURE_WAP
    6,//9, // Mnode Line
#else //FEATURE_WAP
	5,
#endif//FEATURE_WAP
	0, // Hidden Line
    TKN_SMS_TITLE,
    bdSMSMenu1
};



ROM MNODE_BODY_T bdSMSMenu2[] = {
    {ACCEL_KEY_1,       (BYTE*)TKN_SMS_IN_BOX,TKN_SMS_IN_BOX,           &g_AniSMSReceiptBox, TRUE,   (void *)SKA_ReceiveMsg},
    {ACCEL_KEY_2,       NULL,TKN_SMS_SENT_BOX,            &g_AniSMSSendingBox, TRUE,   (void *)SKA_SendMsg},
    {ACCEL_KEY_3,       NULL,TKN_SMS_MENU4,            &g_AniSMSReceiptAlarm, TRUE,   (void *)SKA_MsgAlarm},
    {ACCEL_KEY_4,       NULL,TKN_SMS_MENU5,        &g_AniSMSDelete, TRUE,   (void *)SKA_DeleteAllMsgCode},
    {ACCEL_KEY_5,       NULL,TKN_SMS_MENU6,        &g_AniSMSEmail, TRUE,   (void *)SKA_EnterEmailMenu},//&mn011Srvc},               //white 00.12.19
#ifdef FEATURE_WAP
    {ACCEL_KEY_6,       NULL,TKN_NTOP_MAIL_TITLE,        &g_AniSMSNTopMail, TRUE,   (void *)SKA_SendWAPMsg},
#endif //FEATURE_WAP
//	{ACCEL_KEY_STAR,    NULL,TKN_LOCATION_SETTING,        &g_AniSMSLocationTrace, TRUE,   (void *)SKA_LocationCode}
};
ROM MNODE_T mnSMSMenu2 = { 
    NULL, //&mnMainMenuTop, // Parent
#ifdef FEATURE_WAP
    7,//9, // Mnode Line
#else//FEATURE_WAP
	6,
#endif//FEATURE_WAP
	0, // Hidden Line
    TKN_SMS_TITLE,
    bdSMSMenu2
};


/*
** STI 부가 서비스 
*/

/*
** ***************************************************************************
** 1. 음성사서함
** ***************************************************************************
*/
ROM MNODE_BODY_T bd017VoiceMail[3] = {
    {ACCEL_KEY_1,       NULL, TKN_CONNECT,          NULL, TRUE,   (void *)SKA_017VMConnect},
    {ACCEL_KEY_2,       NULL, TKN_SET,             NULL, TRUE,   (void *)SKA_017VMSet},
    {ACCEL_KEY_3,       NULL, TKN_CANCEL,                 NULL, TRUE,   (void *)SKA_017VMCancel}
};

ROM MNODE_T mn017VoiceMail = { 
    &mn017Srvc, // Parent
    3,          // Mnode Line
	0,          // Hidden Line
    TKN_017_VOICE_MAIL,
    bd017VoiceMail
};


/*
** ***************************************************************************
** 2. 자동연결
** ***************************************************************************
*/
ROM MNODE_BODY_T bd017AutoCon[2] = {
    {ACCEL_KEY_1,       NULL, TKN_REGISTRY,       NULL, TRUE,   (void *)SKA_017ACRegister},
    {ACCEL_KEY_2,       NULL, TKN_CANCEL,         NULL, TRUE,   (void *)SKA_017ACCancel}
};

ROM MNODE_T mn017AutoCon = { 
    &mn017Srvc, // Parent
    2,          // Mnode Line
	0,          // Hidden Line
    TKN_017_AUTO_CONECT,
    bd017AutoCon
};

/*
** ***************************************************************************
** 3. 무조건 전환
** ***************************************************************************
*/
ROM MNODE_BODY_T bd017RedCall[2] = {
    {ACCEL_KEY_1, NULL,  TKN_REGISTRY,             NULL, TRUE,   (void *)SKA_017RCRegister},
    {ACCEL_KEY_2, NULL,  TKN_CANCEL,               NULL, TRUE,   (void *)SKA_017RCCancel}
};

ROM MNODE_T mn017RedCall = { 
    &mn017Srvc, // Parent
    2,          // Mnode Line
	0,          // Hidden Line
    TKN_017_REDIRECT_CALL,
    bd017RedCall
};

/*
** ***************************************************************************
** 4. 통화중대기
** ***************************************************************************
*/

ROM MNODE_BODY_T bd017CallWaiting[2] = {
    {ACCEL_KEY_1,  NULL,  TKN_REGISTRY,             NULL, TRUE,   (void *)SKA_017CWRegister},
    {ACCEL_KEY_2,  NULL,  TKN_CANCEL,               NULL, TRUE,   (void *)SKA_017CWCancel}
};

ROM MNODE_T mn017CallWaiting = { 
    &mn017Srvc, // Parent
    2,          // Mnode Line
	0,          // Hidden Line
    TKN_017_CALL_WAITING,
    bd017CallWaiting
};

/*
** ***************************************************************************
** 5. 파워리모콘
** ***************************************************************************
*/
ROM MNODE_BODY_T bd017PowerRemote[1] = {
    {ACCEL_KEY_1,   NULL,  TKN_CONNECT,              NULL, TRUE,   (void *)SKA_017PRConnect}
};

ROM MNODE_T mn017PowerRemote = { 
    &mn017Srvc, // Parent
    1,          // Mnode Line
	0,          // Hidden Line
    TKN_017_POWER_REMOTE,
    bd017PowerRemote
};

/*
** ***************************************************************************
** 6. 파워멜로디
** ***************************************************************************
*/
ROM MNODE_BODY_T bd017PowerMelody[1] = {
    {ACCEL_KEY_1,   NULL,  TKN_CONNECT,              NULL, TRUE,   (void *)SKA_017PMConnect}
};

ROM MNODE_T mn017PowerMelody = { 
    &mn017Srvc, // Parent
    1,          // Mnode Line
	0,          // Hidden Line
    TKN_017_POWER_MELODY,
    bd017PowerMelody
};

/*
** ***************************************************************************
** 7. 부가서비스 편집
** ***************************************************************************
*/
ROM MNODE_BODY_T bd017ServiceEdit[6] = {
    {ACCEL_KEY_1, NULL,  TKN_017_VOICE_MAIL,       NULL, TRUE,   (void *)SKA_017VMEdit},
    {ACCEL_KEY_2, NULL,  TKN_017_AUTO_CONECT,      NULL, TRUE,   (void *)SKA_017ACEdit},
    {ACCEL_KEY_3, NULL,  TKN_017_REDIRECT_CALL,    NULL, TRUE,   (void *)SKA_017RCEdit},
    {ACCEL_KEY_4, NULL,  TKN_017_CALL_WAITING,     NULL, TRUE,   (void *)SKA_017CWEdit},
    {ACCEL_KEY_5, NULL,  TKN_017_POWER_REMOTE,     NULL, TRUE,   (void *)SKA_017PREdit},
    {ACCEL_KEY_6, NULL,  TKN_CALL_HOT_MENU3,       NULL, TRUE,   (void *)SKA_017HotMenu3Edit}
};

ROM MNODE_T mn017ServiceEdit = { 
    &mn017Srvc, // Parent
    6,         // Mnode Line
	0,          // Hidden Line
    TKN_017_SRVC_EDIT,
    bd017ServiceEdit
};

/*
** ***************************************************************************
** 017 부가서비스 메뉴
** ***************************************************************************
*/
ROM MNODE_BODY_T bd017Srvc[7] = {
    {ACCEL_KEY_1, NULL, TKN_017_VOICE_MAIL,       NULL, FALSE,   (void *)&mn017VoiceMail},
    {ACCEL_KEY_2, NULL, TKN_017_AUTO_CONECT,      NULL, FALSE,   (void *)&mn017AutoCon},
    {ACCEL_KEY_3, NULL, TKN_017_REDIRECT_CALL,    NULL, FALSE,   (void *)&mn017RedCall},
    {ACCEL_KEY_4, NULL, TKN_017_CALL_WAITING,     NULL, FALSE,   (void *)&mn017CallWaiting},
    {ACCEL_KEY_5, NULL, TKN_017_POWER_REMOTE,     NULL, FALSE,   (void *)&mn017PowerRemote},
    {ACCEL_KEY_6, NULL, TKN_017_POWER_MELODY,     NULL, FALSE,   (void *)&mn017PowerMelody},
    {ACCEL_KEY_7, NULL, TKN_017_SRVC_EDIT,        NULL, FALSE,   (void *)&mn017ServiceEdit}
};

ROM MNODE_T mn017Srvc = { 
    &mnPhoneBook, //&mnControlPanel, // Parent
    7,            // Mnode Line
	0,            // Hidden Line
    TKN_017_SERVICE,
    bd017Srvc
};




/* 011 부가 서비스 */
//white 2000.12.15 inrt 제어판 -> 시스템 설정 메뉴 --> 전화부
ROM MNODE_BODY_T bd011SrvcAutoCon[2] = {
    {ACCEL_KEY_1,       NULL,TKN_REGISTRY,             NULL, TRUE,   (void *)SKA_AutoConReg},
    {ACCEL_KEY_2,       NULL,TKN_CANCEL,               NULL, TRUE,   (void *)SKA_AutoConCancel}
    //{ACCEL_KEY_3,       TKN_EDITING,              NULL, TRUE,   (void *)SKA_AutoConEdit}
};

ROM MNODE_T mn011SrvcAutoCon = { 
    &mn011Srvc, // Parent
    2,//3,         // Mnode Line
	0, // Hidden Line
    TKN_011_AUTOCONEC,
    bd011SrvcAutoCon
};

ROM MNODE_BODY_T bd011RevCall[2] = {
    {ACCEL_KEY_1,       NULL,TKN_REGISTRY,             NULL, TRUE,   (void *)SKA_RevCallReg},
    {ACCEL_KEY_2,       NULL,TKN_CANCEL,               NULL, TRUE,   (void *)SKA_RevCallCancel}
    //{ACCEL_KEY_3,       TKN_EDITING,              NULL, TRUE,   (void *)SKA_RevCallEdit}
};

ROM MNODE_T mn011RevCall = { 
    &mn011Srvc, // Parent
    2,//3,         // Mnode Line
	0, // Hidden Line
    TKN_011_REVERSECALL,
    bd011RevCall
};

ROM MNODE_BODY_T bd011InfoSam[1] = {
    {ACCEL_KEY_1,       NULL,TKN_CONNECT,              NULL, TRUE,   (void *)SKA_InfoSamCon}
};

ROM MNODE_T mn011InfoSam = { 
    &mn011Srvc, // Parent
    1,//2,         // Mnode Line
	0, // Hidden Line
    TKN_011_INFOSAM,
    bd011InfoSam
};

ROM MNODE_BODY_T bd011TrafficInfo[1] = {
    {ACCEL_KEY_1,       NULL,TKN_CONNECT,              NULL, TRUE,   (void *)SKA_TrafficInfoCon}
};

ROM MNODE_T mn011TrafficInfo = { 
    &mn011Srvc, // Parent
    1,//2,         // Mnode Line
	0, // Hidden Line
    TKN_011_TRAFFICINFO,
    bd011TrafficInfo
};

ROM MNODE_BODY_T bd011SecretCall[3] = {
    {ACCEL_KEY_1,       NULL,TKN_INTERCALL_CTRL,       NULL, TRUE,   (void *)SKA_InterCallCTRL},
    {ACCEL_KEY_2,       NULL,TKN_ALLCALL_CTRL,         NULL, TRUE,   (void *)SKA_AllCallCTRL},
    {ACCEL_KEY_3,       NULL,TKN_CANCEL,               NULL, TRUE,   (void *)SKA_SecretCancel}
};

ROM MNODE_T mn011SecretCall = { 
    &mn011Srvc, // Parent
    3,//4,         // Mnode Line
	0, // Hidden Line
    TKN_011_SECRETCALL,
    bd011SecretCall
};


ROM MNODE_BODY_T bd011ServiceEdit[8] = {
    {ACCEL_KEY_1,       NULL,TKN_011_SORISAM,            NULL, TRUE,   (void *)SKA_SorisamEditing},
    {ACCEL_KEY_2,       NULL,TKN_011_AUTOCONEC,          NULL, TRUE,   (void *)SKA_AutoConEdit},
    {ACCEL_KEY_3,       NULL,TKN_011_REVERSECALL,        NULL, TRUE,   (void *)SKA_RevCallEdit},
    {ACCEL_KEY_4,       NULL,TKN_011_INFOSAM,            NULL, TRUE,   (void *)SKA_InfoSamEdit},
    {ACCEL_KEY_5,       NULL,TKN_011_TRAFFICINFO,        NULL, TRUE,   (void *)SKA_TrafficEdit},
    {ACCEL_KEY_6,       NULL,TKN_011_SECRETCALL,         NULL, TRUE,   (void *)SKA_SecretEdit},
    {ACCEL_KEY_7,       NULL,TKN_CALL_HOT_MENU3,         NULL, TRUE,   (void *)SKA_HotMenu3Edit},
    {ACCEL_KEY_8,       NULL,TKN_CALL_HOT_MENU4,         NULL, TRUE,   (void *)SKA_HotMenu4Edit}
};

ROM MNODE_T mn011ServiceEdit = { 
    &mn011Srvc,
    8,
	0,
    TKN_011SRVC_EDIT,
    bd011ServiceEdit
};

ROM MNODE_BODY_T bd011SrvcSori[4] = {
    {ACCEL_KEY_1,       NULL,TKN_CONNECT,              NULL, TRUE,   (void *)SKA_SorisamConnect},
    {ACCEL_KEY_2,       NULL,TKN_REGISTRY,             NULL, TRUE,   (void *)SKA_SorisamRegi},
    {ACCEL_KEY_3,       NULL,TKN_DIRECT_REGI,          NULL, TRUE,   (void *)SKA_SorisamDirectRegi},
    {ACCEL_KEY_4,       NULL,TKN_CANCEL,               NULL, TRUE,   (void *)SKA_SorisamCancel}
};

ROM MNODE_T mn011SrvcSori = { 
    &mn011Srvc,
    4,
	0,
    TKN_011_SORISAM,
    bd011SrvcSori
};

ROM MNODE_BODY_T bd011Srvc[7] = {
    {ACCEL_KEY_1,       NULL,TKN_011_SORISAM,            NULL, FALSE,   (void *)&mn011SrvcSori},
    {ACCEL_KEY_2,       NULL,TKN_011_AUTOCONEC,          NULL, FALSE,   (void *)&mn011SrvcAutoCon},
    {ACCEL_KEY_3,       NULL,TKN_011_REVERSECALL,        NULL, FALSE,   (void *)&mn011RevCall},
    {ACCEL_KEY_4,       NULL,TKN_011_INFOSAM,            NULL, FALSE,   (void *)&mn011InfoSam},
    {ACCEL_KEY_5,       NULL,TKN_011_TRAFFICINFO,        NULL, FALSE,   (void *)&mn011TrafficInfo},
    {ACCEL_KEY_6,       NULL,TKN_011_SECRETCALL,         NULL, FALSE,   (void *)&mn011SecretCall},
    {ACCEL_KEY_7,       NULL,TKN_011SRVC_EDIT,           NULL, FALSE,   (void *)&mn011ServiceEdit}
};

ROM MNODE_T mn011Srvc = { 
    &mnPhoneBook,
    7,
	0,
    TKN_011_SERVICE_SET,
    bd011Srvc
};


ROM MNODE_BODY_T bdPhoneTest[5] = {
    {ACCEL_KEY_1,       NULL,TKN_PBOOK_TEST_INIT,           NULL, TRUE,   (void *)SKA_BookTestInit},
    {ACCEL_KEY_2,       NULL,TKN_PBOOK_TEST_GROUPSAVE,      NULL, TRUE,   (void *)SKA_BookTestGroupSave},
    {ACCEL_KEY_3,       NULL,TKN_PBOOK_TEST_MEMBERSAVE,     NULL, TRUE,   (void *)SKA_BookTestMemberSave},
    {ACCEL_KEY_4,       NULL,TKN_PBOOK_TEST_DATASAVE,       NULL, TRUE,   (void *)SKA_BookTestDataSave},
    {ACCEL_KEY_5,       NULL,TKN_PBOOK_TEST_APPLET,         NULL, TRUE,   (void *)SKA_BookTestApplet}
};


ROM MNODE_T mnPhoneBookTest = { 
    &mnPhoneBook,
    5,
	0,
    TKN_PHONE_BOOK_TITLE,
    bdPhoneTest
};

//////////////////////////////////////////////////////////////////////////////
// 기타 ??????????
///////////////
// TEST MENU //
///////////////
//add_htg_00_12_26 : Data Service menu implementation(<-----------------------------------.

ROM MNODE_BODY_T bdDmKind[2] = {
    {ACCEL_KEY_1,       NULL,TKN_DM_SPEED_TITLE,   NULL, TRUE,   (void *)&SKA_SetDmSpeed},
    {ACCEL_KEY_2,       NULL,TKN_DM_PORT_TITLE,    NULL, TRUE,   (void *)&SKA_SetDmPort},
};

ROM MNODE_T mnDmKind = { 
    &mnControlTestMenu, // Parent
    2, // Mnode Line
	0, // Hidden Line
    TKN_DM_SETTING_TITLE,
    bdDmKind 
};
//asdf 01.10.22 lcd contrast control test를 위해서
extern void SKA_MainLCDContrastControl();
extern void SKA_SubLCDContrastControl();
ROM MNODE_BODY_T bdLCDContrastControl[2] = {
    {ACCEL_KEY_1,       NULL,TKN_MAINLCD_TITLE,        NULL, TRUE,   (void *)SKA_MainLCDContrastControl},
    {ACCEL_KEY_2,       NULL,TKN_SUBLCD_TITLE,    NULL, TRUE,   (void *)SKA_SubLCDContrastControl},
};

ROM MNODE_T mnLCDContrastControl = { 
                         &mnTestMenu, // Parent
                         2, // Mnode Line
						 0, // Hidden Line
                         TKN_LCD_BRIGHTNESS,
                         bdLCDContrastControl
                        };

#ifdef FEATURE_AGILENT_TEST
extern void SKY_API SKA_AgilentTest(SKY_EVENT_T *pEvent);
#endif

extern void SKY_API SKA_LCDColorTest(SKY_EVENT_T *pEvent);
extern void EQA_CameraTestMenuTop(EQS_EVENT *pInEvent);
extern void SKA_RecoverPhotoList(SKY_EVENT_T *pEvent);

ROM MNODE_BODY_T bdControlTestMenu[] = {
    {ACCEL_KEY_1,       NULL,TKN_DEBUG_SCREEN_TOGGLE,        NULL, TRUE,   (void *)EQA_MainDebugScreenToggle},
    {ACCEL_KEY_2,       NULL,TKN_DM_SETTING_TITLE,           NULL, FALSE,   (void *)&mnDmKind}
};


ROM MNODE_T mnControlTestMenu = { 
                         &mnMainMenuTop, // Parent
                         2, // Mnode Line
						 0, // Hidden Line
                         TKN_DEBUG_MENU_TITLE,
                         bdControlTestMenu
                        }; 


ROM MNODE_BODY_T bdTestMenu[] = {
    {ACCEL_KEY_1,       NULL,TKN_SW_VERSION_TEST,               NULL, TRUE,   (void *)SKY_API SKA_MainTestMenuVersion},
	{ACCEL_KEY_2,       NULL,TKN_SMART_BATTERY_TEST,         NULL, TRUE,   (void *)SKA_MainNotYet},
    {ACCEL_KEY_3,       NULL,TKN_RF_TEST,					NULL, TRUE,   (void *)SKA_MainRFTest},
#ifdef FEATURE_AGILENT_TEST
    {ACCEL_KEY_4,       NULL,TKN_AGILENT_TEST,               NULL, TRUE,    (void *)SKA_AgilentTest},
#else
    {ACCEL_KEY_4,       NULL,TKN_AGILENT_TEST,               NULL, TRUE,    (void *)SKA_MainNotYet},
#endif//#ifdef FEATURE_AGILENT_TEST
    {ACCEL_KEY_5,       NULL,TKN_MELODY_TEST,                NULL, FALSE,   (void *)&mnMelodyTest},
    {ACCEL_KEY_6,       NULL,TKN_CAMERA_TEST,                NULL, FALSE,   (void *)&mnCameraTest},
    {ACCEL_KEY_7,       NULL,TKN_AUDIO_GAIN_TEST,			NULL, TRUE,   (void *)SKA_MainAudioTest},
    {ACCEL_KEY_8,       NULL,TKN_LCD_COLOR_TEST,             NULL, TRUE,   (void *)SKA_LCDColorTest },
    {ACCEL_KEY_9,       NULL,TKN_FLIP_COUNTER_TEST,             NULL, TRUE,   (void *)SKA_MainFlipCounterTesst },
#ifdef	FEATURE_VOICEPLAY
    {ACCEL_KEY_0,       NULL,TKN_GVM_VOICE_TEST,             NULL, TRUE,   (void *)SKA_GVMVoiceTest},
#else
    {ACCEL_KEY_0,       NULL,TKN_LED_TEST,                   NULL, TRUE,   (void *)SKA_MainNotYet},
#endif
	//addedby happykid in 01.05.08
    {ACCEL_KEY_STAR,    NULL,TKN_LCD_BRIGHTNESS,             NULL, FALSE,   (void *)&mnLCDContrastControl}
#ifdef SW_RESET_ENABLE
    ,{ACCEL_KEY_POUND,    NULL,TKN_SW_RESET_TEST,             NULL, TRUE,   (void *)SKA_MainSWResetTest}
#endif
};

ROM MNODE_T mnTestMenu = { 
                         &mnMainMenuTop, // Parent
#ifdef SW_RESET_ENABLE
                         12,
#else
                         11, // Mnode Line
#endif
						 0, // Hidden Line
                         TKN_TEST_MENU_TITLE,
                         bdTestMenu
                        }; 

ROM MNODE_BODY_T bdCameraTest[2] = {
    {ACCEL_KEY_1,       NULL,TKN_MELODY_TEST_MELODY_LIST,        NULL, TRUE,   (void *)EQA_CameraTestMenuTop},
    {ACCEL_KEY_2,       NULL,TKN_MELODY_TEST_EFS_MELODY_LIST,    NULL, TRUE,   (void *)SKA_RecoverPhotoList}
    
};
ROM MNODE_T mnCameraTest = { 
    &mnTestMenu,  // Parent
    2,
	0, // Hidden Line
    TKN_CAMERA_TEST,
    bdCameraTest
};

ROM MNODE_BODY_T bdMelodyTest[6] = {
    {ACCEL_KEY_1,       NULL,TKN_MELODY_TEST_MELODY_LIST,        NULL, TRUE,   (void *)SKA_MainTestMelodyList},
    {ACCEL_KEY_2,       NULL,TKN_MELODY_TEST_EFS_MELODY_LIST,    NULL, TRUE,   (void *)SKA_MainLightSensorTest2},
    {ACCEL_KEY_3,       NULL,TKN_MELODY_TEST_MELODY_OPTION,      NULL, TRUE,   (void *)SKA_MainIDLCDContrastTest},
    {ACCEL_KEY_4,       NULL,TKN_MELODY_TEST_EQ_GAIN,            NULL, TRUE,   (void *)SKA_MainEQVolumeTest},
    {ACCEL_KEY_5,       NULL,TKN_MELODY_TEST_SP_GAIN,            NULL, TRUE,   (void *)SKA_MainMelodyVolumeTest},
	//For melody volume by happykid
	{ACCEL_KEY_6,       NULL,TKN_MELODY_TEST_TOTAL_LEVEL_TITLE,  NULL, TRUE,   (void *)SKA_LCDMusicTotalLevelControl},
};

ROM MNODE_T mnMelodyTest = { 
                         &mnTestMenu, // Parent
                         6, // Mnode Line
						 0, // Hidden Line
                         TKN_MELODY_TEST_TITLE,
                         bdMelodyTest
                        };

ROM MNODE_BODY_T bdCallInfo[2] = {
    {ACCEL_KEY_1,       NULL,TKN_CALL_USETIME,        NULL, TRUE,   (void *)SKA_MainNotYet},
    {ACCEL_KEY_2,       NULL,TKN_NTOP_USETIME,        NULL, TRUE,   (void *)SKA_MainNotYet}
};

ROM MNODE_T mnCallInfo = { 
                         &mnControlTestMenu, // Parent
                         2, // Mnode Line
						 0, // Hidden Line
                         TKN_CALL_TIME,
                         bdCallInfo
                        };



/* Simple NAM Programming Mode hjcho 01-10-18 */
//ROM MNODE_BODY_T bdSimpleNAMProgTop[NUM_SIMPLE_NAM_MENUITEM_MSM5000] = {
//    {ACCEL_KEY_1, NULL,TKN_NAM_1, NULL, TRUE, (void *)SKA_SimpleNAM1SubEntry},
//    {ACCEL_KEY_2, NULL,TKN_NAM_2, NULL, TRUE, (void *)SKA_SimpleNAM2SubEntry},
//    {ACCEL_KEY_3, NULL,TKN_NAM_3, NULL, TRUE, (void *)SKA_SimpleNAM3SubEntry},
//    {ACCEL_KEY_4, NULL,TKN_NAM_4, NULL, TRUE, (void *)SKA_SimpleNAM4SubEntry},
//    {ACCEL_KEY_5, NULL,TKN_NAM_5, NULL, TRUE, (void *)SKA_SimpleNAM5SubEntry}
//};
//ROM MNODE_T mnSimpleNAMProgTop = {
//    NULL, //Parent
//    NUM_SIMPLE_NAM_MENUITEM_MSM5000, // Mnode Line
//    0,    //Hidden Line
//    TKN_NAM_TOP_TITLE,
//    bdSimpleNAMProgTop
//};
/* Simple NAM Programming Mode hjcho 01-10-18 */



ROM MNODE_BODY_T bdNAMProgTop[NUM_NAM_MENUITEM_MSM5000] = {
    {ACCEL_KEY_1, NULL,TKN_NAM_1, NULL, TRUE, (void *)SKA_NAM1SubEntry},
    {ACCEL_KEY_2, NULL,TKN_NAM_2, NULL, TRUE, (void *)SKA_NAM2SubEntry},
    {ACCEL_KEY_3, NULL,TKN_NAM_3, NULL, TRUE, (void *)SKA_NAM3SubEntry},
    {ACCEL_KEY_4, NULL,TKN_NAM_4, NULL, TRUE, (void *)SKA_NAM4SubEntry},
    {ACCEL_KEY_5, NULL,TKN_NAM_5, NULL, TRUE, (void *)SKA_NAM5SubEntry},
    {ACCEL_KEY_6, NULL,TKN_SCI, NULL, TRUE, (void *)SKA_SCI},
    {ACCEL_KEY_7, NULL,TKN_SCM, NULL, TRUE, (void *)SKA_SCM}
#ifdef FEATURE_Q_PAGE
    ,{ACCEL_KEY_8, NULL,TKN_Q_PAGE, NULL, TRUE, (void *)SKA_QPAGE}    
#endif //FEATURE_Q_PAGE
};

ROM MNODE_T mnNAMProgTop = { 
    NULL, // Parent
    NUM_NAM_MENUITEM_MSM5000, // Mnode Line
	0, // Hidden Line
    TKN_NAM_TOP_TITLE,
    bdNAMProgTop
};

  
ROM MNODE_BODY_T bdNAMProgSub[NUM_NAM_SUBMENUITEM_MSM5000] = {
    {ACCEL_KEY_1, NULL,TKN_NAM_PHONENUMBER,    NULL, TRUE, (void *)SKA_PhoneNumber}, 
    {ACCEL_KEY_2, NULL,TKN_NAM_CHANNEL,        NULL, TRUE, (void *)SKA_Channel},
    {ACCEL_KEY_3, NULL,TKN_NAM_HOMESIDNID,     NULL, TRUE, (void *)SKA_SIDNID},
    {ACCEL_KEY_4, NULL,TKN_NAM_PREF_SYS_TITLE, NULL, TRUE, (void *)SKA_PrefSys},
    {ACCEL_KEY_5, NULL,TKN_NAM_MCC,            NULL, TRUE, (void *)SKA_MCC},
    {ACCEL_KEY_6, NULL,TKN_NAM_MNC,            NULL, TRUE, (void *)SKA_MNC},
    {ACCEL_KEY_7, NULL,TKN_NAM_PRL,            NULL, TRUE, (void *)SKA_PRL}
};


ROM MNODE_T mnNAM1ProgSub = { 
    &mnNAMProgTop, // Parent
    NUM_NAM_SUBMENUITEM_MSM5000, // Mnode Line
	0, // Hidden Line
    TKN_NAM_1,
    bdNAMProgSub
};  


ROM MNODE_T mnNAM2ProgSub = { 
    &mnNAMProgTop, // Parent
    NUM_NAM_SUBMENUITEM_MSM5000, // Mnode Line
	0, // Hidden Line
    TKN_NAM_2,
    bdNAMProgSub
};

ROM MNODE_T mnNAM3ProgSub = { 
    &mnNAMProgTop, // Parent
    NUM_NAM_SUBMENUITEM_MSM5000, // Mnode Line
	0, // Hidden Line
    TKN_NAM_3,
    bdNAMProgSub
};

ROM MNODE_T mnNAM4ProgSub = { 
    &mnNAMProgTop, // Parent
    NUM_NAM_SUBMENUITEM_MSM5000, // Mnode Line
	0, // Hidden Line
    TKN_NAM_4,
    bdNAMProgSub
};

ROM MNODE_T mnNAM5ProgSub = { 
    &mnNAMProgTop, // Parent
    NUM_NAM_SUBMENUITEM_MSM5000, // Mnode Line
	0, // Hidden Line
    TKN_NAM_5,
    bdNAMProgSub
};

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

void SKY_API SKA_MainNotYet(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(FALSE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, 
            WA_SHOW_HEADER);
        //EQC_StartAnimation(EQC_Animation(10, 20, &AniInfoUnderConstruction, NULL), TRUE);
        
        break;

    case SE_SOFTKEY_SELECT:
        //EQC_MessageBox((BYTE* )"Not Yet", 5, MBT_YESNO, (BYTE *)"XXX");
        break;

    case SE_IO_MESGBOX_YES:
    case SE_IO_MESGBOX_END:
        EQS_END();
        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


