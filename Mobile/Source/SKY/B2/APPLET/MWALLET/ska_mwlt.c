/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_mwlt.c
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
** 2001-06-25 yhlee       Created.
** 
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

#include "mwdef.h"
#include "clnt.h"
#include "clntapp.h"   // getmobilewalletdata
										//extern GetMobileWalletDataType	getMobileWalletData;
										//extern unsigned int isAppendString;
#include "cert_manage.h"
//#include "ska_call.h"
#include "ska.h"
#include "wap_mime.h"
#include "clntwallet.h"

/*
**----------------------------------------------------------------------------
**  Definitions 
**----------------------------------------------------------------------------
*/
// �� �ʵ庰 ���ѵ� ���� -----------------------mwdef.h�� ���ǵ� �ʵ� ����
#define USER_TOTAL_LENGTH	  54
#define	USER_INFO_NAME			10
#define	USER_INFO_PHONE			12
#define	USER_INFO_EMAIL			24
#define RESERVED			        	8

#define DELIVERY_TOTAL_LENGTH	132
#define	DELIVERY_INFO_TITLE			16
#define	DELIVERY_INFO_ZIP			  6
#define	DELIVERY_INFO_ADDR			80
#define	DELIVERY_INFO_NAME			10
#define	DELIVERY_INFO_PHONE			12

#define CARD_TOTAL_LENGTH		47
#define	CARD_INFO_NUMBER		18
#define	CARD_INFO_PWD				4
#define	CARD_INFO_VALID				4
#define	CARD_INFO_RNO				13

#define POINT_TOTAL_LENGTH		28
#define	POINT_INFO_NUMBER		16
#define	POINT_INFO_PWD				4

// ��ȣȭ�� ��ȣȭ �ÿ� � ������������ �˷��ֱ� ���ؼ� --------------
enum tagMWDataIndex
{
	MOBILE_WALLET_USER = 0,
	MOBILE_WALLET_DELIVERY1,		
	MOBILE_WALLET_DELIVERY2,
	MOBILE_WALLET_CARD1,
	MOBILE_WALLET_CARD2,
	MOBILE_WALLET_CARD3,
	MOBILE_WALLET_OCB
};
typedef enum tagMWDataIndex MWData_E;

/* to check if all mendatory items are inputted
---------------------------------------------------------------------*/
#define INPUT_MASK          0xFF

// ����� ����
#define USER_NAME			0x0001 //Mandatory
#define USER_PHONE			0x0002 //Mandatory
//#define USER_EMAIL			0x0004 //Extension

//#define USER_FIELD		    (USER_NAME|USER_EMAIL)
#define USER_FIELD		    USER_NAME

// ����� ����
//#define DELIVERY_TITLE		0x0010 //Extension
#define DELIVERY_ZIPCODE	0x0001 //Mandatory
#define DELIVERY_ADDRESS	0x0002 //Mandatory
#define DELIVERY_NAME		0x0004 //Mandatory
#define DELIVERY_PHONE		0x0008 //Mandatory

#define DELIVERY_FIELD		(DELIVERY_ZIPCODE|DELIVERY_ADDRESS|DELIVERY_NAME|DELIVERY_PHONE)

// ī�� ����
#define CARD_NUMBER	        0x0001 //Mandatory
#define CARD_PASSWD	    	0x0002 //Mandatory
#define CARD_VALID			0x0004 //Mandatory
#define CARD_RNO			0x0008 //Mandatory

#define CARD_FIELD		    (CARD_NUMBER|CARD_PASSWD|CARD_VALID|CARD_RNO)

// Point ���� 
#define POINT_NUM			0x0001 //Mandatory
#define POINT_PWD			0x0002 //Mandatory

#define POINT_FIELD			(POINT_NUM|POINT_PWD)

// SignInfo, EncryptInfo
#define	INFO_RESET			0x0000
#define	appendInfo			0x0001
#define	personInfo			0x0002
#define	deliveryInfo		 0x0004
#define	cardInfo			  0x0008
#define accountInfo			0x0010   
#define	cardextInfo			 0x0020

/*-----------------------------------------------------------------------------*/
#define INIT_TRIAL_NUM				           1
#define MAX_PASSWD_TRIAL				 3

#define MAX_DELIVERY_INFO					2
#define MAX_SETTLEMENT_INFO				  3
#define MAX_MYCERT_INFO						5
#define MAX_TCACERT_INFO					10

// �޴� �б�� offline���� online���� �˱� ���ؼ�..
#define online				1      

#define MAX_WIDTH_LEN	16 

enum tagMW_CONTROL_E 
{    
	ICT_U_TITLE_E = 0,
    ICT_U_NAME_E,			// ����� �̸�
    ICT_U_PHONE_E,			// ����� ��ȭ��ȣ
    ICT_U_EMAIL_E,			// ����� �̸��� �ּ�
	ICT_D_TITLE_E,			// ����� ����
	ICT_D_ZIPCODE_E,		// ����� �����ȣ
	ICT_D_ADDRESS_E,		// ����� �ּ�
	ICT_D_NAME_E,			// ����� �����θ�
	ICT_D_PHONE_E,			// ����� ������ ����ó
	ICT_C_TITLE_E,
	ICT_C_NUMBER_E,			// ī�� ��ȣ
	ICT_C_PWD_E,			// ī�� ��й�ȣ
	ICT_C_VALID_E,			// ī�� ��ȿ�Ⱓ
	ICT_C_RNO_E,			// ī�� �ֹε�Ϲ�ȣ
	ICT_P_TITLE_E,
	ICT_P_NUMBER_E,         // ����Ʈ ī�� ��ȣ
    ICT_P_PWD_E,				// ����Ʈ �������
	ICT_S_CARDONLY_E,
	ICT_S_DELIVONLY_E,
	ICT_S_ALL_E,
	ICT_S_SIGNING_E,
	ICT_C_DELETE_OR_CANCEL_E,
	ICT_S_DOWNLOAD,  // ������ ���..
	ICT_S_SUCCESS,
	ICT_E_FORMAT,  // ������ ���...
	ICT_E_DOWNLOAD,
	ICT_E_NOSPACE,
	ICT_E_FAIL,
	ICT_T_INPUT1,				
	ICT_T_INPUT2
};
typedef enum tagMW_CONTROL_E  MW_CONTROL_E;

static WWPersonalInfo	userinfo, *pUserInfo = NULL;
static WWDeliveryInfo	deliveryinfo, *pDeliveryInfo = NULL, tmpdeliveryinfo[2];
static WWCreditCard		cardinfo, *pCardInfo = NULL, tmpcardinfo[3];
static WWCardExtInfo		ocbinfo, *pOCBInfo = NULL;

//Check mandatory items are inputted.
static int userInputStatus = 0;
static int deliveryInputStatus = 0;
static int cardInputStatus = 0;
static int ocbInputStatus = 0;

static unsigned char currentPIN[5];
//You can have three times input change
static UINT	pinTrialCounter = INIT_TRIAL_NUM;
static UINT WhichOne;
static UINT index;
static BYTE  MyNumber[12];

char certPIN[9];
int PromptLen;
char userPromptData[256];

// to save which information will be used for signing
static BYTE updateInfo[4];

// TCA ������ Display ���� ����
struct Display_Name {
	BYTE IssuerName[30];
	BYTE NotAfter[30];
	BYTE CertificateUsage[30];
	BYTE Option[200];
};
typedef struct Display_Name DN;

DN Display;
BYTE texttmp[100];
static char dn[2][50];

// == EncryptTxt ���� �����..=================================
// Substring : substring�� ������ ��� ����
// format : 0�̸� format ����.
//				 1�̸� format ����.
// hidden : 0�̸� plain text
//				1�̸� hidden text
// numlimit : 0�̸� �Է� ���� ���� ����.
//			   	   1�̸� �Է� ���� ���� ����.
// Digit : numlimit�� 1�� ���(��, �Է±��� ������ �ִ� ��� ����� ���������� ����)
// FormatChar : �Է±����� ������ ���� Ȥ�� ���� 
//						"A" : �빮��, ��ȣ  - ���� ���� ����.
//						"a" : �ҹ���, ��ȣ  - ���� ���� ����.
//						"N" : ����
//						"n" : ����, ��ȣ
//						"X" : �빮��, ����, ��ȣ
//						"x" : �ҹ���, ����, ��ȣ
//						"M" : ���� ���� ������ ��� ����
//						"m" : ���� ���� ������ ��� ����
//======================================================
struct tagEncryptTextSt {
	BOOL datasaved;
	BYTE Substring[50];   // MW���� : �迭 ������..
	BOOL format;
	BOOL hidden;
	BOOL numlimit;
	BYTE Digit[2];
	BYTE FormatChar;
};
typedef struct tagEncryptTextSt EncryptTextSt;

EncryptTextSt EncryptTxt[5];	


ROM EDIT_MODE_SELECT_T  MW_AMode = {
    2,     0,
    {TEM_ENGLISH_UPPER, TEM_SYMBOL},
	FALSE
};

ROM EDIT_MODE_SELECT_T  MW_aMode = {
    2,     0,
    {TEM_ENGLISH_LOWER,TEM_SYMBOL},
	FALSE
};

ROM EDIT_MODE_SELECT_T  MW_NMode = {
    1,     0,
    {TEM_NUMBER},
	FALSE
};

ROM EDIT_MODE_SELECT_T  MW_nMode = {
    2,     0,
    {TEM_NUMBER, TEM_SYMBOL},
	FALSE
};

ROM EDIT_MODE_SELECT_T  MW_XMode = {
    3,     0,
    {TEM_ENGLISH_UPPER,TEM_NUMBER, TEM_SYMBOL},
	FALSE
};

ROM EDIT_MODE_SELECT_T  MW_xMode = {
    3,     0,
    {TEM_ENGLISH_LOWER, TEM_NUMBER, TEM_SYMBOL},
	FALSE
};

ROM EDIT_MODE_SELECT_T  MW_MMode = {
    5,     0,
    {TEM_KOREAN, TEM_ENGLISH_UPPER, TEM_ENGLISH_LOWER, TEM_SYMBOL, TEM_NUMBER},
	TRUE
};

ROM EDIT_MODE_SELECT_T  MW_mMode = {
    5,     0,
    {TEM_KOREAN, TEM_ENGLISH_LOWER, TEM_ENGLISH_UPPER, TEM_SYMBOL, TEM_NUMBER},
	TRUE
};

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------*/
LOCAL void SKY_API sSKA_MWallet(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWUserInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWSettlementInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWDeliveryInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWPasswdChange(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWCertificationInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWInitialize(SKY_EVENT_T *pEvent);

LOCAL void SKY_API sSKA_MWTestInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWCardSelect(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWCardInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWOCBInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWDeliverySelect(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWDeliveryInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_EditDelivery(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWPasswordChange(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWTCACertInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWTCACertSelect(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWShowTCACertInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWPRIcertInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWPRICertSelect(SKY_EVENT_T *pEvent);

LOCAL void SKY_API sSKA_MWShowMYCertInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_CertPINInputAgain(SKY_EVENT_T *pEvent);
LOCAL void SKY_API SKA_MWCheckMyCert(SKY_EVENT_T *pEvent);
LOCAL void SKY_API SKA_MWCheckTCACert(SKY_EVENT_T *pEvent);
LOCAL void SKY_API SKA_MWProcessHashedCert(SKY_EVENT_T *pEvent);
LOCAL void SKY_API SKA_MWProcessSignedCert(SKY_EVENT_T *pEvent);
LOCAL void SKY_API SKA_ShowCertForSigning(SKY_EVENT_T *pEvent);

LOCAL void SKY_API sSKA_MWTestTempCert(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWTestTempCertInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWTestMyCert(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWTestMyCertInfo(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWTestCACert(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_MWTestCACertInfo(SKY_EVENT_T *pEvent);



LOCAL HCONTROL sAddInputControl(UINT8 nInsertPosition, UINT8 nViewLine, MW_CONTROL_E Type, BOOL fFocus);
LOCAL void Initialize_MW(void);
LOCAL void MW_add_input_status(int type, int member); 
LOCAL void MW_init_user(void);
LOCAL void MW_init_card(void);
LOCAL void MW_init_emoney(void);
LOCAL void MW_init_ocb(void);
LOCAL void MW_init_delivery(void);
LOCAL int MW_read_user(void);
LOCAL int MW_read_card(void);
LOCAL int MW_read_ocb(void);
LOCAL int MW_read_delivery(void);
LOCAL void MW_write_user(void);
LOCAL void MW_write_card(void);
LOCAL void MW_write_ocb(void);
LOCAL void MW_write_delivery(void);
LOCAL BYTE* sGetMyNumber(void);
LOCAL int MW_parse_display(DWORD ind,int mycert);
LOCAL void MW_parse_SubjectDN(char *dn, int ind);
LOCAL void MW_parse_userPrompt(void);
LOCAL void InitEncryptText(void);
char *char2bin(char* w);

//=================================================================== Online 
LOCAL void SKY_API sSKA_show_appendStringToMWdata(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_PasswdforEncrypt(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_DisplayEncryptData(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_CheckForSign(SKY_EVENT_T *pEvent);

// test 
VOID SKY_API SKA_MWTestShow(SKY_EVENT_T *pEvent);

//-------------------------------------------------------------------------------

void SKY_API SKA_MainMWallet(SKY_EVENT_T *pEvent)
{
    BYTE				sPasswd[5];
    HCONTROL		sNumHandle;
	static BOOL		IsEnd;
	static UINT		checklock;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_MW_PASSWD_CHECK), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
        
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_FitCanvasSize();
            EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
		if (IsEnd ) {
			EQS_END(); 
		}
		else {
			EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
			EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
		}
        break; 

    case EQE_IO_NEDIT_MATCH_LENGTH:                        
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);

		// ��й�ȣ �Է� �õ� Ƚ��
		checklock = SKY_GET_MW_WALLET_TRIAL_NUM();

		// NV�� ����� PIN���� �о�´�. 
		memcpy((const char *)gHashedPIN, SKY_GET_MW_WALLET_HASHEDPIN(),MW_WALLET_HASHEDPIN_LENGTH);  

		// ����ڰ� �Է��� PIN�� �����ϴ� �κ� 
		if (!wwVerifyPIN(sPasswd)) {
			memcpy(currentPIN,(const char *)sPasswd, 5); currentPIN[4] = NULL;
			pinTrialCounter = INIT_TRIAL_NUM;
			SKY_SET_MW_WALLET_TRIAL_NUM(INIT_TRIAL_NUM);
			SKY_SET_MW_WALLET();
			EQS_START_SIBLING(sSKA_MWallet, FALSE);
		}
        else {
			pinTrialCounter++; 
			// password �Է� �õ�Ƚ��(3)���� ������� ���� ��� ���� �޴��� �̵�
			if (pinTrialCounter > MAX_PASSWD_TRIAL) { 
				pinTrialCounter = INIT_TRIAL_NUM; checklock ++;
				SKY_SET_MW_WALLET_TRIAL_NUM(checklock); 
				SKY_SET_MW_WALLET();
				// ��й�ȣ 3�� �Է½��а� ���� ���������� �߻��� ���
				if (checklock > MAX_PASSWD_TRIAL) {
					pinTrialCounter = INIT_TRIAL_NUM; checklock = INIT_TRIAL_NUM;
					IsEnd = TRUE;
					// �������� ���� �ʱ�ȭ
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INITIALIZE_INFORM_MSG), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					Initialize_MW();
				} else {
					EQS_END();
				}
			}
			else {  // �н����� ���Է��� �䱸 "��й�ȣ�� Ʋ���ϴ�."
				IsEnd = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}
		} 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :    
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSKA_MWallet(SKY_EVENT_T *pEvent)
{
	HCONTROL hControl;
	BYTE* pToken[7];

	switch(pEvent->EventID) 
	{
		case SE_APP_START :
	        EQS_GetDC(TRUE);
	        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_NONE,
                      SKY_GET_TOKEN(TKN_MWALLET), 
                      WA_SHOW_HEADER| WA_SHOW_CML);

			pToken[0] = SKY_GET_TOKEN(TKN_MW_USER_INFO);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_SETTLEMENT_INFO);   
			pToken[2] = SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO);   
			pToken[3] = SKY_GET_TOKEN(TKN_MW_PASSWD_CHANGE);    
			pToken[4] = SKY_GET_TOKEN(TKN_MW_CERTIFICATION_INFO); 
			pToken[5] = SKY_GET_TOKEN(TKN_MW_INITIALIZE);
			pToken[6] = SKY_GET_TOKEN(TKN_MW_APPEND_INFO);
			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,7,pToken,7,7,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END :
			EQS_ReleaseDC();
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				EQS_START_CHILD(sSKA_MWUserInfo);
			    break;
			case 1 :  
				EQS_START_CHILD(sSKA_MWSettlementInfo);
				break;
			case 2 :  
				EQS_START_CHILD(sSKA_MWDeliveryInfo);
				break;
			case 3 :  
	            EQS_START_CHILD(sSKA_MWPasswdChange);
		        break;
			case 4 :  		
				EQS_START_CHILD(sSKA_MWCertificationInfo);
				break;
			case 5 : 
				EQS_START_CHILD(sSKA_MWInitialize);
				break;
			case 6 : 
				EQS_START_CHILD(sSKA_MWTestInfo);
				break;
			default:
				break;
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END();
			break;

		case SE_KEY_END:	
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   
	
		case SE_KEY_PHONE_CLOSE:
			EQS_END_CURRENT_EVENT();
			break;

		case SE_IO_MESGBOX_END:
			break;

		default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
    }
}

LOCAL void SKY_API sSKA_MWUserInfo(SKY_EVENT_T *pEvent)
{
	INT8			nIndex, len, nCtrl;
	HCONTROL		hControl;
	static	BOOL	fEndKey;
	static  BOOL	fSave;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_SAVE,
						  SKY_GET_TOKEN(TKN_MW_USER_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);
			
			MW_init_user(); // ����� ���� �ʱ�ȭ

			fEndKey = FALSE; fSave = FALSE; len = 0;
			nIndex = 0; userInputStatus = 0;
			sAddInputControl(nIndex,1,ICT_U_NAME_E,TRUE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_U_PHONE_E,FALSE);
			nIndex += 2;
			sAddInputControl(nIndex,2,ICT_U_EMAIL_E,FALSE);
			nIndex += 2;

			EQS_StartFormatter();
	
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					EQC_GET_X(EQS_GetNthIOC(nCtrl)) += (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();

		
			EQS_SetIOCFocus( EQS_GetNthIOC(1) );


			if ( SKY_GET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_USER) == 1) {
				MW_read_user();
				
				EQC_SetTextEditData(EQS_GetNthIOC(1),&userinfo.userName);
				//EQC_SetNumberEditValue(EQS_GetNthIOC(3),&userinfo.userPhone);
				EQC_SetTextEditData(EQS_GetNthIOC(5),&userinfo.userEmail);
			}

			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_TEDIT_DONE :
			if (pEvent->wParam == EQS_GetNthIOC(1)) {
				MSG_HIGH("�̸� �Է� ",0,0,0);
				len = EQC_GetTextEditData(EQS_GetNthIOC(1),userinfo.userName);
				if (len) { 
					userInputStatus |= USER_NAME; 
				} else {
					userInputStatus &= (~USER_NAME);
				}
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(5)) {
				MSG_HIGH("���ڿ����ּ� �Է� ",0,0,0);
				len = EQC_GetTextEditData(EQS_GetNthIOC(5),userinfo.userEmail);
				/*if (len) { 
					userInputStatus |= USER_EMAIL; 
				} else { 
					userInputStatus &= (~USER_EMAIL); 
				}
				len = 0; */
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}
			break;

		case SE_IO_NEDIT_DONE :
			if (pEvent->wParam == EQS_GetNthIOC(3)) {
				MSG_HIGH("��ȭ��ȣ �Է� ",0,0,0);
				//len = EQC_GetNumberEditValue(EQS_GetNthIOC(3),PersonInfo->userPhone);
				//if (len) 
				//	userInputStatus |= USER_PHONE;
				//else userInputStatus &= (~USER_PHONE);
				//len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}			
			break;

		case SE_SOFTKEY_SAVE :
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
												10, MBT_YESNO, MBT_CONFIRM);
			break;

		case SE_KEY_END : 
			if(EQS_IsControlDataChanged() == FALSE) {
				EQS_END_TO(EQS_POP_TO_ROOT);
			} else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
				fEndKey = TRUE;
			}
			break;

		case SE_KEY_CLEAR : 
			if(EQS_IsControlDataChanged() == FALSE) {
				EQS_END(); 
			} else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
			}
			break;

		case SE_IO_MESGBOX_END  :
			if (fSave) {
				if ( fEndKey == TRUE ) {
					EQS_END_TO(EQS_POP_TO_ROOT);
				}
				else EQS_END();
			} 

			break;

		case SE_IO_MESGBOX_NO :
			if( fEndKey == TRUE )
			{
				EQS_END_TO(EQS_POP_TO_ROOT);
			} else {
				EQS_END();
			}

			break;

		case SE_IO_MESGBOX_YES : 
			if(userInputStatus == USER_FIELD) {
					MW_write_user();
					fSave = TRUE;
					hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ON_SAVING_MSG), 2, MBT_NONE,MBT_HELP);
			} else if(!(userInputStatus & USER_NAME )) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_NAME), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} /*else if(!(userInputStatus & USER_PHONE)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PHONE), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(userInputStatus & USER_EMAIL)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_EMAIL), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} */

			break;

		default : 
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}

}

LOCAL void SKY_API sSKA_MWSettlementInfo(SKY_EVENT_T *pEvent)
{
	HCONTROL hControl;
	BYTE* pToken[2];
	
	switch(pEvent->EventID) 
	{
		case SE_APP_START :
	        EQS_GetDC(TRUE);
	        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_NONE,
                      SKY_GET_TOKEN(TKN_MW_SETTLEMENT_INFO), 
                      WA_SHOW_HEADER| WA_SHOW_CML);

			pToken[0] = SKY_GET_TOKEN(TKN_MW_CARD_INFO);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_OCB_INFO);   
			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,2,2,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END :
			EQS_ReleaseDC();
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				EQS_START_CHILD(sSKA_MWCardSelect);
			    break;
			case 1 :  
				WhichOne = MOBILE_WALLET_OCB;
				EQS_START_CHILD(sSKA_MWOCBInfo);
				break;
			default:
				break;
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END();
			break;

		case SE_KEY_END:	
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   
	
		case SE_KEY_PHONE_CLOSE:
			EQS_END_CURRENT_EVENT();
			break;

		case SE_IO_MESGBOX_END:
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
    }
}

LOCAL void SKY_API sSKA_MWCardSelect(SKY_EVENT_T *pEvent)
{
	HCONTROL hControl;
	BYTE* pToken[3];
	static BOOL	MakeSibling = FALSE;
	
	switch(pEvent->EventID) 
	{
		case SE_APP_START :
	        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_NONE,
                      SKY_GET_TOKEN(TKN_MW_CARD_INFO), 
                      WA_SHOW_HEADER| WA_SHOW_CML);
			
			MakeSibling = pEvent->wParam;

			pToken[0] = SKY_GET_TOKEN(TKN_MW_CARD1_INFO);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_CARD2_INFO);   
			pToken[2] = SKY_GET_TOKEN(TKN_MW_CARD3_INFO);   
			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,3,3,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END :
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				WhichOne = MOBILE_WALLET_CARD1;
				if(MakeSibling)	{
					//cardSel = WhichOne - MOBILE_WALLET_CARD1;
					//EQS_START_SIBLING(sSKA_MWCardInfo,FALSE);
					EQS_END();
				} else {
					EQS_START_CHILD(sSKA_MWCardInfo);
				}
			    break;
			case 1 :  
				WhichOne = MOBILE_WALLET_CARD2;
				if(MakeSibling) {
					//cardSel = WhichOne - MOBILE_WALLET_CARD1;
					//EQS_START_SIBLING(sSKA_MWCardInfo,FALSE);
					EQS_END();
				} else {
					EQS_START_CHILD(sSKA_MWCardInfo);
				}
				break;
			case 2 :  
				WhichOne = MOBILE_WALLET_CARD3;
				if(MakeSibling) {
					//cardSel = WhichOne - MOBILE_WALLET_CARD1;
					//EQS_START_SIBLING(sSKA_MWCardInfo,FALSE);
					EQS_END();
				} else {
					EQS_START_CHILD(sSKA_MWCardInfo);
				}
				break;
			default:
				break;
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			//cardSel = 0;
			EQS_END();
			break;

		case SE_KEY_END:	
			//EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   
	
		case SE_KEY_PHONE_CLOSE:
			EQS_END_CURRENT_EVENT();
			break;

		case SE_IO_MESGBOX_END:
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
    }
}

LOCAL void SKY_API sSKA_MWCardInfo(SKY_EVENT_T *pEvent)
{
	INT8		nIndex,len, nCtrl;
	HCONTROL	hControl;
	static	BOOL	fEndKey, fSave;

	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_SAVE,
						  SKY_GET_TOKEN(TKN_MW_CARD_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);
			
			MW_init_card();

			index = WhichOne - MOBILE_WALLET_CARD1;
			fEndKey = FALSE; fSave = FALSE; len=0;
			nIndex = 0; cardInputStatus = 0;
			sAddInputControl(nIndex,2,ICT_C_NUMBER_E,TRUE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_C_PWD_E,FALSE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_C_VALID_E,FALSE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_C_RNO_E,FALSE);
			nIndex += 2;

			EQS_StartFormatter();
	
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();

			EQS_SetIOCFocus( EQS_GetNthIOC(1) );

			if ( SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
				MW_read_card();
				EQC_SetNumberEditValue(EQS_GetNthIOC(1),&cardinfo.cardNO);
				EQC_SetNumberEditValue(EQS_GetNthIOC(3),&cardinfo.cardPwd);
				EQC_SetNumberEditValue(EQS_GetNthIOC(5),&cardinfo.cardValid);
				EQC_SetNumberEditValue(EQS_GetNthIOC(7),&cardinfo.cardRno);
			}

			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_NEDIT_DONE :
			if (pEvent->wParam == EQS_GetNthIOC(1)) {
				MSG_HIGH("ī���ȣ �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(1),cardinfo.cardNO);
				//if (len == CARD_INFO_NUMBER) {
				if (len > 0) {
					cardInputStatus |= CARD_NUMBER;
				} else { 
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					cardInputStatus &= (~CARD_NUMBER);
				}
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(3)) {
				MSG_HIGH("��й�ȣ �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(3),cardinfo.cardPwd);
				if (len == CARD_INFO_PWD) {
					cardInputStatus |= CARD_PASSWD;
				} else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					cardInputStatus &= (~CARD_PASSWD);
				}
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(5)) {
				MSG_HIGH("valid �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(5),cardinfo.cardValid);
				if (len == CARD_INFO_VALID) { 
					// ��ȿ�Ⱓ�� ����� �ԷµǾ����� üũ..MMYY (MM : 01 ~ 12)
					if (cardinfo.cardValid[0] == '0') {
						//if (cardinfo[index].cardValid[1] >= '1' && cardinfo[index].cardValid[1] <= '9')
						if(cardinfo.cardValid[1] != '0') {
							cardInputStatus |= CARD_VALID;
						} else {
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_WRONG_VALID), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
							memset(cardinfo.cardValid,NULL,5);
							EQC_SetNumberEditValue(EQS_GetNthIOC(5),cardinfo.cardValid);
							cardInputStatus &= (~CARD_VALID);
						}
					}
					else if (cardinfo.cardValid[0] == '1') {
						if (cardinfo.cardValid[1] >= '0' && cardinfo.cardValid[1] <= '2') {
							cardInputStatus |= CARD_VALID;
						} else {
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_WRONG_VALID), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
							memset(cardinfo.cardValid,NULL,5);
							EQC_SetNumberEditValue(EQS_GetNthIOC(5),cardinfo.cardValid);
							cardInputStatus &= (~CARD_VALID);
						}
					}
					else {
						EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_WRONG_VALID), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						memset(cardinfo.cardValid,NULL,5);
						EQC_SetNumberEditValue(EQS_GetNthIOC(5),cardinfo.cardValid);
						cardInputStatus &= (~CARD_VALID);
					}
				}
				else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					cardInputStatus &= (~CARD_VALID);
				}
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(7)) {
				MSG_HIGH("RNO �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(7),cardinfo.cardRno);
				if (len == CARD_INFO_RNO ) cardInputStatus |= CARD_RNO;
				else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					cardInputStatus &= (~CARD_RNO);
				}
				len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}			
			break;

		case SE_SOFTKEY_SAVE :
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
										10, MBT_YESNO, MBT_CONFIRM);
			break;

		case SE_KEY_END : 
			if(EQS_IsControlDataChanged() == FALSE) EQS_END_TO(EQS_POP_TO_ROOT); 
			else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
				fEndKey = TRUE;
			}

			break;

		case SE_KEY_CLEAR : 
			if(EQS_IsControlDataChanged() == FALSE) EQS_END(); 
			else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
			}
			break;

		case SE_IO_MESGBOX_END :
			if (fSave) {
				if ( fEndKey == TRUE ) {
					EQS_END_TO(EQS_POP_TO_ROOT);
				}
				else EQS_END();
			} 

			break;

		case SE_IO_MESGBOX_NO :
			if( fEndKey == TRUE )
			{
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			else EQS_END();

			break;

		case SE_IO_MESGBOX_YES : 
			if(cardInputStatus == CARD_FIELD) {
					MW_write_card();
					fSave = TRUE;
					hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ON_SAVING_MSG), 2, MBT_NONE,MBT_HELP);
			} else if(!(cardInputStatus & CARD_NUMBER )) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_CARD), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(cardInputStatus & CARD_PASSWD)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PWD), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(cardInputStatus & CARD_VALID)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_VALID), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(cardInputStatus & CARD_RNO)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_RNO), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}

			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWOCBInfo(SKY_EVENT_T *pEvent)
{
	INT8		nIndex,len,nCtrl;
	HCONTROL	hControl;
	static	BOOL	fEndKey,fSave;
	
	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_SAVE,
						  SKY_GET_TOKEN(TKN_MW_OCB_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);
			
			MW_init_ocb();

			index = WhichOne - MOBILE_WALLET_OCB;
			fEndKey = FALSE;fSave = FALSE; len = 0;
			nIndex = 0; ocbInputStatus = 0;

			sAddInputControl(nIndex,1,ICT_P_NUMBER_E,TRUE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_P_PWD_E,FALSE);
			nIndex += 2;

			EQS_StartFormatter();
	
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					EQC_GET_X(EQS_GetNthIOC(nCtrl)) += (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();

			EQS_SetIOCFocus( EQS_GetNthIOC(1) );

			if ( SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
				MW_read_ocb();
				
				EQC_SetNumberEditValue(EQS_GetNthIOC(1),&ocbinfo.pointNo);
				EQC_SetNumberEditValue(EQS_GetNthIOC(3),&ocbinfo.pointPwd);
			}

			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_NEDIT_DONE :
			if (pEvent->wParam == EQS_GetNthIOC(1)) {
				MSG_HIGH("��ȣ �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(1),ocbinfo.pointNo);
				//if (len == POINT_INFO_NUMBER) {
				if (len > 0) {
					ocbInputStatus |= POINT_NUM;
				} else { 
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					ocbInputStatus &= (~POINT_NUM);
				}
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(3)) {
				MSG_HIGH("��ȣ �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(3),ocbinfo.pointPwd);
				if (len == POINT_INFO_PWD) {
					ocbInputStatus |= POINT_PWD;
				} else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					ocbInputStatus &= (~POINT_PWD);
				}
				len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}			
			break;

		case SE_SOFTKEY_SAVE :
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
			break;

		case SE_KEY_END : 
			if(EQS_IsControlDataChanged() == FALSE) EQS_END_TO(EQS_POP_TO_ROOT); 
			else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
				fEndKey = TRUE;
			}

			break;

		case SE_KEY_CLEAR : 
			if(EQS_IsControlDataChanged() == FALSE) EQS_END(); 
			else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
			}
			break;

		case SE_IO_MESGBOX_END :
			if (fSave) {
				if ( fEndKey == TRUE ) {
					EQS_END_TO(EQS_POP_TO_ROOT);
				} else EQS_END();
			} 

			break;

		case SE_IO_MESGBOX_NO :
			if( fEndKey == TRUE )
			{
				EQS_END_TO(EQS_POP_TO_ROOT);
			} else EQS_END();

			break;

		case SE_IO_MESGBOX_YES : 
			if(ocbInputStatus == POINT_FIELD) {
					MW_write_ocb();
					fSave = TRUE;
					hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ON_SAVING_MSG), 2, MBT_NONE,MBT_HELP);
			} else if(!(ocbInputStatus & POINT_NUM )) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_CARDNUM), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(ocbInputStatus & POINT_PWD )) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PWD), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} 

			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

/*
LOCAL void SKY_API sSKA_MWDeliveryInfo(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_START_CHILD_EVENT(sSKA_MWDeliverySelect,SE_APP_START,TRUE,FALSE);
        break;

    case SE_APP_END:
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}	


LOCAL void SKY_API sSKA_MWDeliverySelect(SKY_EVENT_T *pEvent)
{
    UINT8               i,j,unIdx, unIdxOfNew, AddrOfDelivery,idx,myidx,myidx3;
	static  BOOL		ADD_ST;
    static  UINT8       hList, TotalDelivery;
    static  BYTE*       pDeliveryList[2]; 

	static  BYTE		StatusOfDel[2];
	BOOL	MakeSibling = FALSE;		

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X, SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, SKY_DEF_WIN_DY, 
                        CL_DELETE_CONFIRM_NEWMEMO,   // �켱�� �˸��� command line�� �����ϱ�..
                        SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO),
                        WA_SHOW_CML | WA_SHOW_HEADER);

        unIdx = 0; ADD_ST = FALSE;

        for( i=0; i < MAX_DELIVERY_INFO; i++ )
        {
            pDeliveryList[i] = (BYTE *)SKY_Malloc( (SIZE_T)16 );
			StatusOfDel[i] = SKY_GET_MW_WALLET_DATA_SAVED(i+MOBILE_WALLET_DELIVERY1);
		}

		// ����� Delivery info�� ���� ���  
        if(StatusOfDel[0] >= 1 || StatusOfDel[1] >= 1 )
        {           
            hList = CreateList( MAX_DELIVERY_INFO );
            for( i=0; i<MAX_DELIVERY_INFO; i++ )
            {
				idx = StatusOfDel[i];
				if(idx > 0){
					WhichOne = i+MOBILE_WALLET_DELIVERY1;
					MW_read_delivery();
#ifdef BLOCK
					for(j=0;j<16;j++) pDeliveryList[idx-1][j] = deliveryinfo[i].dvTitle[j];
#endif //#ifdef BLOCK
					//for(j=0;j<16;j++) pDeliveryList[idx-1][j] = 'T';
					pDeliveryList[idx-1] = "����� ����";
					AddList( hList, (INT16)(idx-1), FALSE );
					unIdx++;
				} 
            }
            TotalDelivery = unIdx;
            EQC_ScrollList(EQS_NA, EQS_NA, EQS_NA, 
                           EQC_GetScrollDisplayLine(), 
                           pDeliveryList, 
                           TotalDelivery, 
                           TotalDelivery, 
                           0);
            EQC_SetScrollListHandle( EQS_GetNthIOC(0), hList );
            EQC_SET_STYLE(EQS_GetNthIOC(0), (EQC_GET_STYLE(EQS_GetNthIOC(0)) |
                                            CS_SCROLL_LIST_MAXIMIZE | 
                                            CS_SCROLL_LIST_ENUMBERATED));
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        }
        else
        {
            TotalDelivery = unIdx;
            EQS_SetWindowCML(CL_NONE_NONE_NEWMEMO);			
        }
        break;

    case SE_APP_END:        
        for( i=0; i<TotalDelivery; i++ )
		//for(i=0;i<MAX_DELIVERY_INFO;i++)
		{
            SKY_Free( pDeliveryList[i] );
			SKY_SET_MW_WALLET_DATA_SAVED(i+1,StatusOfDel[i]);
		}
        
        break;

    case SE_APP_RESUME:
		if( TotalDelivery )
		{
			if(!(ADD_ST && TotalDelivery == 1))
				EQS_DeleteControl(EQS_GetNthIOC(0));
		}

        if(StatusOfDel[0] >= 1 || StatusOfDel[1] >= 1 )
        {   
            unIdx = 0;
            hList = CreateList(MAX_DELIVERY_INFO);
            for( i=0; i<MAX_DELIVERY_INFO; i++ )
            {
				idx = StatusOfDel[i];
				if(idx){
					WhichOne = i+MOBILE_WALLET_DELIVERY1;
					MW_read_delivery();
					//for(j=0;j<16;j++) pDeliveryList[idx-1][j] = 'J'; //deliveryinfo[i].dvTitle[j];					
					pDeliveryList[idx-1] = "����� ����";
					AddList( hList, (INT16)(idx-1), FALSE );
					unIdx++;

				} 
            }
  
            TotalDelivery = unIdx;
            EQC_ScrollList(EQS_NA,
                           EQS_NA, 
                           EQS_NA,
                           EQC_GetScrollDisplayLine(), 
                           pDeliveryList, 
                           TotalDelivery, 
                           TotalDelivery,
                           0);
            EQC_SetScrollListHandle(EQS_GetNthIOC(0), hList);
            EQC_SET_STYLE(EQS_GetNthIOC(0), (EQC_GET_STYLE(EQS_GetNthIOC(0)) |
                                            CS_SCROLL_LIST_MAXIMIZE |
                                            CS_SCROLL_LIST_ENUMBERATED));
            EQS_SetIOCActive(EQS_GetNthIOC(0));
			EQS_SetWindowCML(CL_DELETE_CONFIRM_NEWMEMO);
        }     
        break;

    case SE_SOFTKEY_DELETE:
        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_Q_DELETE),
                       MESSAGE_DEFAULT_TIME, 
                       MBT_YESNO,
                       MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES: 
        myidx3 = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
		AddrOfDelivery = (UINT8)GetListValue(hList, myidx3);

		for(i = 0;i < MAX_DELIVERY_INFO;i++) {
			myidx = StatusOfDel[i];
			if ( myidx == myidx3+1){
				SKY_SET_MW_WALLET_DELIVERY(i,NULL,MW_WALLET_DELIVERY_LENGTH); 
				StatusOfDel[i] = 0;
				break;
			}
		}
		for(i = 0;i < MAX_DELIVERY_INFO;i++)
			if(StatusOfDel[i] > myidx && MAX_DELIVERY_INFO+1 > StatusOfDel[i]) {
				StatusOfDel[i] = StatusOfDel[i] - 1;
			}

        EQC_DeleteScrollData(EQS_GetNthIOC(0), unIdx);
        RemoveListItem(hList, unIdx);
        TotalDelivery = TotalDelivery - 1;

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_CONFIRM_DELETE),
                       MESSAGE_DEFAULT_TIME, 
                       MBT_NONE,
                       MBT_SUCCESS);
        break;

    case SE_IO_MESGBOX_NO:
        break;

    case SE_SOFTKEY_ADD: //new
		myidx = 0; ADD_ST = TRUE;
        unIdxOfNew = MAX_DELIVERY_INFO+1;
		for(i=0;i<MAX_DELIVERY_INFO;i++){
			if(StatusOfDel[i] == 0)
            {
                unIdxOfNew = i;
                break;
            }
		}
		if(unIdxOfNew < MAX_DELIVERY_INFO) {
			for(i=0;i<MAX_DELIVERY_INFO;i++){
				idx = StatusOfDel[i];
				if( idx > 0 ) myidx ++;
			}
			TotalDelivery = myidx + 1;
			StatusOfDel[unIdxOfNew] = TotalDelivery;
			WhichOne = unIdxOfNew + MOBILE_WALLET_DELIVERY1;
			//if(MakeSibling)
			//	EQS_START_SIBLING_EVENT(sSKA_EditDelivery, SE_APP_START, 0, WhichOne,FALSE);
			//else 
				EQS_START_CHILD_EVENT(sSKA_EditDelivery, SE_APP_START, 0, WhichOne);
		} 
		else 
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_FULL), 
                           MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL );
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        unIdx = EQC_GetScrollDataLine(EQS_GetNthIOC(0)); 
		ADD_ST = FALSE;
        if(unIdx > 0)
        {
			unIdx = (UINT8)EQC_GetScrollLogicalIndex( EQS_GetNthIOC(0) );
			for(i=0;i<MAX_DELIVERY_INFO;i++) {
				if(StatusOfDel[i] == unIdx + 1) break;
			}
			SKY_SET_MW_WALLET_DATA_SAVED(i+MOBILE_WALLET_DELIVERY1,StatusOfDel[i]);
	        AddrOfDelivery = (UINT8)GetListValue( hList, unIdx ); //get the address
			WhichOne = i+MOBILE_WALLET_DELIVERY1;
			if(MakeSibling) {
				deliverySel = WhichOne - MOBILE_WALLET_DELIVERY1;
				MakeSibling = FALSE;
				EQS_START_SIBLING_EVENT(sSKA_EditDelivery, SE_APP_START, 1, WhichOne,FALSE);
			} else
				EQS_START_CHILD_EVENT(sSKA_EditDelivery, SE_APP_START, 1, WhichOne);
        }
        else
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_NO_CONFIRM), 
                           MESSAGE_DEFAULT_TIME, 
                           MBT_NONE, 
                           MBT_HELP );
        }
        break;

    case EQE_IO_SCROLL_LIST_CANCEL:
		deliverySel = 0;
        EQS_END();
        break;

    case SE_IO_MESGBOX_END:
        if(TotalDelivery)
		{
            EQS_SetIOCActive(EQS_GetNthIOC(0));
		}
        else
		{
			EQS_DeleteControl(EQS_GetNthIOC(0));
			EQS_SetWindowCML(CL_NONE_NONE_NEWMEMO);
		}       
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }    
}

LOCAL void SKY_API sSKA_EditDelivery(SKY_EVENT_T *pEvent)
{
	INT8			nIndex, len;
	HCONTROL		hControl;
	static	BOOL	fEndKey;
	static  BOOL	fSave;
	static  BYTE	Valid;

	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_SAVE,
						  SKY_GET_TOKEN(TKN_MW_DELIVERY1_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);

			MW_init_delivery(MOBILE_WALLET_DELIVERY1-1);
			MW_init_delivery(MOBILE_WALLET_DELIVERY2-1);

			index = WhichOne - MOBILE_WALLET_DELIVERY1;
			fEndKey = FALSE; fSave = FALSE; len = 0;
			nIndex = 0; deliveryInputStatus = 0;
			//sAddInputControl(nIndex,1,ICT_D_TITLE_E,TRUE);
			//nIndex += 2;
			sAddInputControl(nIndex,1,ICT_D_ZIPCODE_E,TRUE);
			nIndex += 2;
			sAddInputControl(nIndex,5,ICT_D_ADDRESS_E,FALSE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_D_NAME_E,FALSE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_D_PHONE_E,FALSE);
			nIndex += 2;

			Valid = pEvent->wParam;
			if ( Valid == 1 ) {
				MW_read_delivery();
				
				//EQC_SetTextEditData(EQS_GetNthIOC(1),&deliveryinfo[index].dvTitle);
				EQC_SetNumberEditValue(EQS_GetNthIOC(1),&deliveryinfo[index].dvZip);
				EQC_SetTextEditData(EQS_GetNthIOC(3),&deliveryinfo[index].dvAddr);
				EQC_SetTextEditData(EQS_GetNthIOC(5),&deliveryinfo[index].dvName);
				EQC_SetNumberEditValue(EQS_GetNthIOC(7),&deliveryinfo[index].dvPhone);
			}

			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: 
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_TEDIT_DONE :
//			if (pEvent->wParam == EQS_GetNthIOC(1)) {
//				MSG_HIGH("Ÿ��Ʋ �Է� ",0,0,0);
//				len = EQC_GetTextEditData(EQS_GetNthIOC(1),deliveryinfo[index].dvTitle);
//				//if (len) deliveryInputStatus |= DELIVERY_TITLE; 
//				//else deliveryInputStatus &= (~DELIVERY_TITLE);
//				//len = 0;
//			} else 
			if (pEvent->wParam == EQS_GetNthIOC(3)) {
				MSG_HIGH("�ּ� �Է� ",0,0,0);
				len = EQC_GetTextEditData(EQS_GetNthIOC(3),deliveryinfo[index].dvAddr);
				if (len) deliveryInputStatus |= DELIVERY_ADDRESS;
				else deliveryInputStatus &= (~DELIVERY_ADDRESS);
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(5)) {
				MSG_HIGH("�̸� �Է� ",0,0,0);
				len = EQC_GetTextEditData(EQS_GetNthIOC(5),deliveryinfo[index].dvName);
				if (len) deliveryInputStatus |= DELIVERY_NAME; 
				else deliveryInputStatus &= (~DELIVERY_NAME);
				len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}
			break;

		case SE_IO_NEDIT_DONE :
			if (pEvent->wParam == EQS_GetNthIOC(1)) {
				MSG_HIGH("�����ȣ �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(1),deliveryinfo[index].dvZip);
				if (len == DELIVERY_INFO_ZIP) deliveryInputStatus |= DELIVERY_ZIPCODE;
				else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
					MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					deliveryInputStatus &= (~DELIVERY_ZIPCODE);
				}
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(7)) {
				MSG_HIGH("��ȭ��ȣ �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(7),deliveryinfo[index].dvPhone);
				if (len >= 6 && len <= 12) deliveryInputStatus |= DELIVERY_PHONE;
				else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					deliveryInputStatus &= (~DELIVERY_PHONE);
				}
				len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}			
			break;

		case SE_SOFTKEY_SAVE :
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
										10, MBT_YESNO, MBT_CONFIRM);
			break;

		case SE_KEY_END : 
			if(EQS_IsControlDataChanged() == FALSE) EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
			else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
				fEndKey = TRUE;
			}

			break;

		case SE_KEY_CLEAR : 
			if(EQS_IsControlDataChanged() == FALSE) EQS_END(); 
			else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
			}
			break;

		case SE_IO_MESGBOX_END :
			if (fSave) {
				if ( fEndKey == TRUE ) {
					EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
				}
				else EQS_END();
			} 

			break;

		case SE_IO_MESGBOX_NO :
			if( fEndKey == TRUE )
			{
				EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
			}
			else EQS_END();

			break;

		case SE_IO_MESGBOX_YES : 
			if(deliveryInputStatus == DELIVERY_FIELD) {
					MW_write_delivery();
					fSave = TRUE;
					hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ON_SAVING_MSG), 2, MBT_NONE,MBT_HELP);
					//SKY_SET_MW_WALLET_DATA_SAVED(WhichOne,SKY_GET_MW_WALLET_DATA_SAVED(WhichOne));
			} 
//			else if(!(deliveryInputStatus & DELIVERY_TITLE )) {
//				fSave = FALSE;
//				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_CARD), 
//							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
//			} 
			else if(!(deliveryInputStatus & DELIVERY_ZIPCODE)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ZIPCODE), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(deliveryInputStatus & DELIVERY_ADDRESS)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ADDR), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(deliveryInputStatus & DELIVERY_NAME)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_R_NAME), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(deliveryInputStatus & DELIVERY_PHONE)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PHONE), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}

			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}		
	
}
*/

LOCAL void SKY_API sSKA_MWDeliveryInfo(SKY_EVENT_T *pEvent)
{
	HCONTROL hControl;
	WORD MakeSibling = 0;
	BYTE* pToken[2];

	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_NONE,
						  SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);


			MakeSibling = pEvent->wParam;
			pToken[0] = SKY_GET_TOKEN(TKN_MW_DELIVERY1_INFO);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_DELIVERY2_INFO);   
			
			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,2,2,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END :
			break;

		case SE_APP_RESUME: 
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				WhichOne = MOBILE_WALLET_DELIVERY1;
				if (MakeSibling == 1){
					//EQS_START_SIBLING(sSKA_MWDeliverySelect,FALSE);
					EQS_END();
				} else {
					EQS_START_CHILD(sSKA_MWDeliverySelect);
				}
			    break;
			case 1 :  
				WhichOne = MOBILE_WALLET_DELIVERY2;
				if(MakeSibling == 1) {
					//EQS_START_SIBLING(sSKA_MWDeliverySelect,FALSE);
					EQS_END();
				} else {
					EQS_START_CHILD(sSKA_MWDeliverySelect);
				}
				break;
			default:
				break;
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END();
			break;

		case SE_KEY_END:	
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   
	
		case SE_KEY_PHONE_CLOSE:
			EQS_END_CURRENT_EVENT();
			break;

		case SE_IO_MESGBOX_END:
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWDeliverySelect(SKY_EVENT_T *pEvent)
{
	INT8			nIndex, len, nCtrl;
	HCONTROL		hControl;
	static	BOOL	fEndKey;
	static  BOOL	fSave;
	
	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_SAVE,
						  SKY_GET_TOKEN(TKN_MW_DELIVERY1_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);

			MW_init_delivery();

			index = WhichOne - MOBILE_WALLET_DELIVERY1;
			fEndKey = FALSE; fSave = FALSE; len = 0;
			nIndex = 0; deliveryInputStatus = 0;
//			sAddInputControl(nIndex,1,ICT_D_TITLE_E,TRUE);
//			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_D_ZIPCODE_E,TRUE);
			nIndex += 2;
			sAddInputControl(nIndex,4,ICT_D_ADDRESS_E,FALSE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_D_NAME_E,FALSE);
			nIndex += 2;
			sAddInputControl(nIndex,1,ICT_D_PHONE_E,FALSE);
			nIndex += 2;

			EQS_StartFormatter();
	
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();

			EQS_SetIOCFocus( EQS_GetNthIOC(1) );

			if ( SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
				MW_read_delivery();
				DelInfo = &deliveryinfo;
				
//				EQC_SetTextEditData(EQS_GetNthIOC(1),&deliveryinfo[index].dvTitle);
				EQC_SetNumberEditValue(EQS_GetNthIOC(1),&deliveryinfo.dvZip);
				EQC_SetTextEditData(EQS_GetNthIOC(3),&deliveryinfo.dvAddr);
				EQC_SetTextEditData(EQS_GetNthIOC(5),&deliveryinfo.dvName);
				EQC_SetNumberEditValue(EQS_GetNthIOC(7),&deliveryinfo.dvPhone);
			}

			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: 
			EQS_SetIOCActive(EQS_GetNthIOC(1));
			break;

		case SE_IO_TEDIT_DONE :
//			if (pEvent->wParam == EQS_GetNthIOC(1)) {
//				MSG_HIGH("Ÿ��Ʋ �Է� ",0,0,0);
//				len = EQC_GetTextEditData(EQS_GetNthIOC(1),deliveryinfo.dvTitle);
				//if (len) deliveryInputStatus |= DELIVERY_TITLE; 
				//else deliveryInputStatus &= (~DELIVERY_TITLE);
				//len = 0;
//			} else
			if (pEvent->wParam == EQS_GetNthIOC(3)) {
				MSG_HIGH("�ּ� �Է� ",0,0,0);
				len = EQC_GetTextEditData(EQS_GetNthIOC(3),deliveryinfo.dvAddr);
				if (len) {
					deliveryInputStatus |= DELIVERY_ADDRESS;
				} else {
					deliveryInputStatus &= (~DELIVERY_ADDRESS);
				}
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(5)) {
				MSG_HIGH("�̸� �Է� ",0,0,0);
				len = EQC_GetTextEditData(EQS_GetNthIOC(5),deliveryinfo.dvName);
				if (len) {
					deliveryInputStatus |= DELIVERY_NAME; 
				} else {
					deliveryInputStatus &= (~DELIVERY_NAME);
				}
				len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}
			break;

		case SE_IO_NEDIT_DONE :
			if (pEvent->wParam == EQS_GetNthIOC(1)) {
				MSG_HIGH("�����ȣ �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(1),deliveryinfo.dvZip);
				if (len == DELIVERY_INFO_ZIP) {
					deliveryInputStatus |= DELIVERY_ZIPCODE;
				} else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
					MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					deliveryInputStatus &= (~DELIVERY_ZIPCODE);
				}
				len = 0;
			} else if (pEvent->wParam == EQS_GetNthIOC(7)) {
				MSG_HIGH("��ȭ��ȣ �Է� ",0,0,0);
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(7),deliveryinfo.dvPhone);
				if (len >= 6 && len <= 12) {
					deliveryInputStatus |= DELIVERY_PHONE;
				} else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					deliveryInputStatus &= (~DELIVERY_PHONE);
				}
				len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}			
			break;

		case SE_SOFTKEY_SAVE :
			hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
										10, MBT_YESNO, MBT_CONFIRM);
			break;

		case SE_KEY_END : 
			if(EQS_IsControlDataChanged() == FALSE) EQS_END_TO(EQS_POP_TO_ROOT);
			else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
				fEndKey = TRUE;
			}

			break;

		case SE_KEY_CLEAR : 
			if(EQS_IsControlDataChanged() == FALSE) EQS_END(); 
			else {
				hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ASK_INFO_SAVING_MSG), \
											10, MBT_YESNO, MBT_CONFIRM);
			}
			break;

		case SE_IO_MESGBOX_END :
			if (fSave) {
				if ( fEndKey == TRUE ) {
					EQS_END_TO(EQS_POP_TO_ROOT);
				}
				else EQS_END();
			} 

			break;

		case SE_IO_MESGBOX_NO :
			if( fEndKey == TRUE )
			{
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			else EQS_END();

			break;

		case SE_IO_MESGBOX_YES : 
			if(deliveryInputStatus == DELIVERY_FIELD) {
					MW_write_delivery();
					fSave = TRUE;
					hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_MW_ON_SAVING_MSG), 2, MBT_NONE,MBT_HELP);
			} 
//			else if(!(deliveryInputStatus & DELIVERY_TITLE )) {
//				fSave = FALSE;
//				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_CARD), 
//							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
//			} 
			else if(!(deliveryInputStatus & DELIVERY_ZIPCODE)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ZIPCODE), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(deliveryInputStatus & DELIVERY_ADDRESS)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ADDR), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(deliveryInputStatus & DELIVERY_NAME)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_R_NAME), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			} else if(!(deliveryInputStatus & DELIVERY_PHONE)) {
				fSave = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PHONE), \
						MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}

			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}		

}

LOCAL void SKY_API sSKA_MWPasswdChange(SKY_EVENT_T *pEvent)
{
    BYTE	sPasswd[5];
    HCONTROL sNumHandle;
	static BOOL IsEnd;
	static UINT		checklock;


	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_NONE_CANCEL,
						  SKY_GET_TOKEN(TKN_MW_PASSWD_CHANGE),
						  WA_SHOW_HEADER | WA_SHOW_CML);
            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 

            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_FitCanvasSize();
            EQS_SetIOCActive(sNumHandle);
	        MSG_HIGH("�������� ���� �����޴� ",0,0,0);
			break;

		case SE_IO_NEDIT_CANCEL:
	        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
		    break;

		case SE_IO_NEDIT_DONE:
			EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
			break;
        
		case SE_IO_MESGBOX_END:
			if (IsEnd ) {
				EQS_END(); 
			}
			else {
				EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
				EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
			}
		    break; 

		case EQE_IO_NEDIT_MATCH_LENGTH:                        
			EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);

			// ��й�ȣ �Է� �õ� Ƚ��
			checklock = SKY_GET_MW_WALLET_TRIAL_NUM();

			// NV�� ����� PIN���� �о�´�. 
			memcpy((const char *)gHashedPIN, SKY_GET_MW_WALLET_HASHEDPIN(),MW_WALLET_HASHEDPIN_LENGTH);  
			
			// ����ڰ� �Է��� PIN�� �����ϴ� �κ� 
			if (!wwVerifyPIN(sPasswd)) {
				memcpy(currentPIN,(const char *)sPasswd, 4);
				pinTrialCounter = INIT_TRIAL_NUM;
				SKY_SET_MW_WALLET_TRIAL_NUM(INIT_TRIAL_NUM);
				SKY_SET_MW_WALLET();
				EQS_START_SIBLING(sSKA_MWPasswordChange, FALSE);
			}
			else {
				pinTrialCounter++; 
				// password �Է� �õ�Ƚ��(3)���� ������� ���� ��� ���� �޴��� �̵�
				if (pinTrialCounter > MAX_PASSWD_TRIAL) { 
					pinTrialCounter = INIT_TRIAL_NUM; checklock ++;
					SKY_SET_MW_WALLET_TRIAL_NUM(checklock);
					SKY_SET_MW_WALLET();
					MSG_HIGH("�޸��� ��й�ȣ ���� : ##%d##",SKY_GET_MW_WALLET_TRIAL_NUM(),0,0);
					// ��й�ȣ 3�� �Է½��а� ���� ���������� �߻��� ���
					if (checklock > MAX_PASSWD_TRIAL) {
						pinTrialCounter = INIT_TRIAL_NUM; checklock = INIT_TRIAL_NUM;
						MSG_HIGH("�޸� �ʱ�ȭ�� �����մϴ�. ",0,0,0);
						IsEnd = TRUE;
						// �������� ���� �ʱ�ȭ
						EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INITIALIZE_INFORM_MSG), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						Initialize_MW();
					}
					// ���� �޴��� �̵�
					else EQS_END();
				}
				else {  // �н����� ���Է��� �䱸 "��й�ȣ�� Ʋ���ϴ�."
					IsEnd = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
			} 
			break;

		case SE_IO_NEDIT_NODIGIT:
			EQS_End();
			break;

		case SE_APP_END :   /* The very last event */ 
			break;
        
		case SE_APP_SUSPEND: /* child applet is started */
			break;
        
		case SE_APP_RESUME: /* child applet is ended */
			break;

        case SE_APP_DRAW:
            TextOutInputCodeMessage();
            break;
        
		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}		
}

LOCAL void SKY_API sSKA_MWPasswordChange(SKY_EVENT_T *pEvent)
{
    STATIC BYTE     sPasswd[5], sAgnPasswd[5];
    STATIC HCONTROL s_hControl, s_hMsgControl, s_EndControl;
    STATIC BOOL s_Again = FALSE;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
   
		EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				      SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
					  CL_NONE_NONE_CANCEL,
					  SKY_GET_TOKEN(TKN_MW_PASSWD_CHANGE),
					  WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecretnew120x93x256c
               (BITMAP*)&g_BmpLock
			  );

        s_hControl  = EQC_NumberEdit(PASS_X, PASS_Y+2 , 4, 1, 4,(BYTE *)"");

        EQC_SET_STYLE(s_hControl, SECRET_NUMBER_EDITOR_DEFAULT);
        EQC_SetNumberEditBigFont(s_hControl, TRUE);   // Set Big Font
        EQC_NotifyNumberEditMatchLen(s_hControl, 4);
		EQS_FitCanvasSize();
        EQS_SetIOCActive(s_hControl);

        break;
      
    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;
        
    case SE_IO_NEDIT_CANCEL:
        break;

    case SE_APP_END :   /* The very last event */ 
        s_Again = FALSE;
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            EQC_SetNumberEditValue(s_hControl, (BYTE*)"");
            EQS_SetIOCActive(s_hControl);
        }else if(pEvent->wParam == s_EndControl)
        {
            EQS_End();
        }
        break;

    case SE_IO_NEDIT_MATCH_LENGTH://SE_IO_NEDIT_DONE:
		if(!s_Again)
		{
			s_Again = TRUE;
			EQC_GetNumberEditValue(s_hControl, sPasswd);
			EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_CONFIRM_PASSWORD));
			s_hMsgControl =
			EQC_MessageBox(SKY_GET_TOKEN(TKN_ONE_MORE_PUT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
		}else
		{
			EQC_GetNumberEditValue(s_hControl, sAgnPasswd);
			if(sAgnPasswd[0] == sPasswd[0] && sAgnPasswd[1] == sPasswd[1] &&
				sAgnPasswd[2] == sPasswd[2] && sAgnPasswd[3] == sPasswd[3])//!STRCMP(sAgnPasswd, sPasswd))
			{
				memcpy((const char *)gHashedPIN, SKY_GET_MW_WALLET_HASHEDPIN(),MW_WALLET_HASHEDPIN_LENGTH);  
				if(!wwChangePIN((char*)sPasswd, currentPIN)) {
					SKY_SET_MW_WALLET_HASHEDPIN(gHashedPIN,MW_WALLET_HASHEDPIN_LENGTH);
					SKY_SET_MW_WALLET();
					memcpy(currentPIN, (const char *)sPasswd, 5); currentPIN[4] = NULL;
					s_EndControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				}
				
			}
			else
			{
				s_hMsgControl =
					EQC_MessageBox(SKY_GET_TOKEN(TKN_NOT_IDENTITY), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			}
		}
		break;

    case SE_IO_NEDIT_DONE:
        break;

    case SE_APP_IO_MODE:
        if(pEvent->wParam == s_hControl)
            EQC_SetNumberEditValue(s_hControl, (BYTE *)"");
        break;
     
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputNewCodeMessage();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }

}

LOCAL void SKY_API sSKA_MWCertificationInfo(SKY_EVENT_T *pEvent)
{
	HCONTROL hControl;
	BYTE* pToken[2];

	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_NONE,
						  SKY_GET_TOKEN(TKN_MW_CERTIFICATION_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);

			pToken[0] = SKY_GET_TOKEN(TKN_MW_CA_INFO);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_PRI_INFO);   
			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,2,2,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END :
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				EQS_START_CHILD(sSKA_MWTCACertInfo);
			    break;
			case 1 :  
				EQS_START_CHILD(sSKA_MWPRIcertInfo);
				break;
			default:
				break;
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END();
			break;

		case SE_KEY_END:	
			//EQS_END_CURRENT_EVENT();
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   
	
		case SE_KEY_PHONE_CLOSE:
			EQS_END_CURRENT_EVENT();
			break;

		case SE_IO_MESGBOX_END:
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}		

}

LOCAL void SKY_API sSKA_MWTCACertInfo(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_START_CHILD(sSKA_MWTCACertSelect);
        break;

    case SE_APP_END:
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}	

LOCAL void SKY_API sSKA_MWTCACertSelect(SKY_EVENT_T *pEvent)
{
	UINT8	i,j,unIdx,AddrOfTCA;
	static  BYTE*	TCACertInfo[MAX_TCACERT_INFO];
	static  BYTE	Temp[30];
	static  BYTE	StatusOfTCA[MAX_TCACERT_INFO];
    static  UINT8   hList, TotalTCACert;
	static  WORD  OnLine;

	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_NONE,
						  SKY_GET_TOKEN(TKN_MW_CA_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);

	        unIdx = 0; 
			OnLine = pEvent->wParam;

			// �켱 CACertInfo[0]/CACertInfo[1]/CACertInfo[2]�� �׻� �����Ѵ�. 
			//�ֳ��ϸ� ��� �ÿ� �־���� �������� ���ϵ��� �ҰŴϱ�..
			for( i=0; i < MAX_TCACERT_INFO; i++ )
			{
				// �켱�� �ӽ÷� ������ �� ���... ũ�⵵ �ӽ÷� 20
				TCACertInfo[i] = (BYTE *)SKY_Malloc( (SIZE_T) 20);   
				StatusOfTCA[i] = SKY_GET_MW_TCACERT_DATA_SAVED(i);
			}

			hList = CreateList( MAX_TCACERT_INFO );
			for( i=0; i<MAX_TCACERT_INFO; i++ )
			{
				if(StatusOfTCA[i] > 0){
					WhichOne = i;
					memset((unsigned char *)Temp,NULL,30);
					memcpy((unsigned char *)Temp,SKY_GET_MW_TCACERT_DISPLAY(i),30);
					for(j=0;j<19;j++) { if(Temp[j] != ';' ) TCACertInfo[i][j] = Temp[j]; 
										else TCACertInfo[i][j] = NULL; }
					//AddList( hList, (INT16)(i), FALSE );
					// i : ���� EFS�� �ε��� 
					// StatusOfTCA[i] : �����Ͱ� ����Ʈ�� �������� ����(�����Ͱ� ����Ǿ� �ִ����� �˷���.)
					AddList(hList,(INT16)(StatusOfTCA[i]-1),FALSE);
					unIdx++;
				} 
			}
			TotalTCACert = unIdx;
			EQC_ScrollList(EQS_NA, EQS_NA, EQS_NA, 
						   EQC_GetScrollDisplayLine(), 
						   TCACertInfo, 
						   TotalTCACert, 
						   TotalTCACert, 
						   0);
			EQC_SetScrollListHandle( EQS_GetNthIOC(0), hList );
			EQC_SET_STYLE(EQS_GetNthIOC(0), (EQC_GET_STYLE(EQS_GetNthIOC(0)) |
											CS_SCROLL_LIST_MAXIMIZE | 
											CS_SCROLL_LIST_ENUMBERATED));
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_APP_END :
			/* dealloc. */
			for( i=0; i<TotalTCACert; i++ )
			//for(i =0; i<MAX_TCACERT_INFO;i++)
			{
				SKY_Free( TCACertInfo[i] );
				if(i != 0) { 
					SKY_SET_MW_TCACERT_DATA_SAVED(i,StatusOfTCA[i]);
					SKY_SET_MW_WALLET();
				}
			}
			break;

		case SE_APP_RESUME: //  resume�Ǿ ����Ǵ� �κ��� �����ϱ� �����ص� ���� ������..
/*			if( TotalTCACert )
//			{
//				if(!(ADD_ST && TotalDelivery == 1))
				EQS_DeleteControl(EQS_GetNthIOC(0));
//			}

			unIdx = 0;
			hList = CreateList(MAX_TCACERT_INFO);
			for( i=0; i<MAX_TCACERT_INFO; i++ )
			{
				if(StatusOfTCA[i]){
					WhichOne = i;
//					MW_read_delivery();
					for(j=0;j<20;j++) TCACertInfo[i][j] = Temp[j];				
					AddList( hList, (INT16)(i), FALSE );
					unIdx++;
				} 
			}

			TotalTCACert = unIdx;

			EQC_ScrollList(EQS_NA,
						   EQS_NA, 
						   EQS_NA,
						   EQC_GetScrollDisplayLine(), 
						   TCACertInfo, 
						   TotalTCACert, 
						   TotalTCACert,
						   0);

			EQC_SetScrollListHandle(EQS_GetNthIOC(0), hList);
			EQC_SET_STYLE(EQS_GetNthIOC(0), (EQC_GET_STYLE(EQS_GetNthIOC(0)) |
											CS_SCROLL_LIST_MAXIMIZE |
											CS_SCROLL_LIST_ENUMBERATED));
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			EQS_SetWindowCML(CL_NONE_SELECT_NONE); */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_SCROLL_LIST_SELECT:
			if( EQC_GetScrollDataLine(EQS_GetNthIOC(0)) > 0 )
			{
				unIdx = (UINT8)EQC_GetScrollLogicalIndex( EQS_GetNthIOC(0) );
				for(i=0;i<MAX_TCACERT_INFO;i++) {
					if(StatusOfTCA[i] == unIdx+1) break;  
				}
				SKY_SET_MW_TCACERT_DATA_SAVED(i,StatusOfTCA[i]); 
				SKY_SET_MW_WALLET();
				AddrOfTCA = (UINT8)GetListValue( hList, unIdx ); //get the address
				//WhichOne = i;

				// offline�� ��쿡�� �ش� �������� ������ ����������, 
				// online�� ��쿡�� �ش� �������� �����Ѵ�.
				if(!OnLine) {
					EQS_START_CHILD_EVENT(sSKA_MWShowTCACertInfo, SE_APP_START, 1, AddrOfTCA/*WhichOne*/);
				} else {
					// ����
					//MW_DeleteTCACert(i);					
					InitMWTCACert(i);
					// EFS index�� ����..
					EQS_END_PARAM(i,FALSE);
				}
			}
			else
			{
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_NO_CONFIRM), 
							   MESSAGE_DEFAULT_TIME, 
							   MBT_NONE, 
							   MBT_HELP );
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END();
			break;

		case SE_KEY_CLEAR :
			EQS_END();
			break;

		case SE_KEY_END:	
			//EQS_END_CURRENT_EVENT();
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   
	
		default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}	
		
}

LOCAL void SKY_API sSKA_MWShowTCACertInfo(SKY_EVENT_T *pEvent)
{
	HCONTROL		hControl;
	int		nCtrl,linenum,showopt;

	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_CA_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);

			showopt = MW_parse_display(pEvent->dwParam,0);

			hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
            sprintf(texttmp, "%s %s",SF_MW_CERT_ORG,SKY_GET_TOKEN(TKN_MW_CA_DISPLAY_ID));
            EQC_SetStaticText(hControl, texttmp);
			//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCertName);
			// Display.IssuerName
			linenum = strlen(Display.IssuerName);
			if(linenum % MAX_WIDTH_LEN != 0) {
				linenum = (linenum / MAX_WIDTH_LEN) + 1;
			} else {
				linenum = linenum / MAX_WIDTH_LEN;
			}
	        hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER, strlen(Display.IssuerName),&(Display.IssuerName), 0);
			EQC_SET_ENABLE(hControl, FALSE);
			EQC_SET_FOCUS(hControl,FALSE);

			hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
            sprintf(texttmp, "%s %s",SF_MW_CERT_VALID,SKY_GET_TOKEN(TKN_MW_CA_DISPLAY_DATE));
            EQC_SetStaticText(hControl, texttmp);
			//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCertTermOfValidity);
			// Display.NotAfter
			linenum = strlen(Display.NotAfter);
			if(linenum % MAX_WIDTH_LEN != 0) {
				linenum = (linenum / MAX_WIDTH_LEN) + 1;
			} else {
				linenum = linenum / MAX_WIDTH_LEN;
			}
	        hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER, strlen(Display.NotAfter),&(Display.NotAfter), 0);
			EQC_SET_ENABLE(hControl, FALSE);
			EQC_SET_FOCUS(hControl,FALSE);

			hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
            sprintf(texttmp, "%s %s",SF_MW_CERT_CERT,SKY_GET_TOKEN(TKN_MW_CA_DISPLAY_USAGE));
            EQC_SetStaticText(hControl, texttmp);
			//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCertUsage);
			// Display.CertificateUsage
			linenum = strlen(Display.CertificateUsage);
			if(linenum % MAX_WIDTH_LEN != 0) {
				linenum = (linenum / MAX_WIDTH_LEN) + 1;
			} else {
				linenum = linenum / MAX_WIDTH_LEN;
			}
	        hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER, strlen(Display.NotAfter),&Display.CertificateUsage, 0);
			EQC_SET_ENABLE(hControl, FALSE);
			EQC_SET_FOCUS(hControl,FALSE);

			if (showopt == 1) {
				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_CERT_ETC,SKY_GET_TOKEN(TKN_MW_CA_DISPLAY_OPT));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCertEtc);
				// Display.Option
				linenum = strlen(Display.Option);
				if(linenum % MAX_WIDTH_LEN != 0) {
					linenum = (linenum / MAX_WIDTH_LEN) + 1;
				} else {
					linenum = linenum / MAX_WIDTH_LEN;
				}
				hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER,strlen(Display.Option),&Display.Option, 0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl,FALSE);
			}

			EQS_StartFormatter();
			
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();

			break;

		case SE_KEY_END:	
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   

		case SE_KEY_CLEAR : 
			EQS_END();
			break;

		case SE_SOFTKEY_CONFIRM : 
			EQS_END();
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END();
			break;

		default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

int MW_parse_display(DWORD ind,int mycert)
{
	int i,j;
	BYTE tmpbuf[MW_TCACERT_DISPLAY];

	j = 0; i = 0;

	memset(Display.IssuerName,NULL,30);
	memset(Display.CertificateUsage,NULL,30);
	memset(Display.NotAfter,NULL,30);
	memset(Display.Option,NULL,200);

	if(mycert) {
		memset(tmpbuf,NULL,MW_MYCERT_DISPLAY);
		memcpy(tmpbuf,SKY_GET_MW_MYCERT_DISPLAY(ind),MW_MYCERT_DISPLAY);
	} else {
		memset(tmpbuf,NULL,MW_TCACERT_DISPLAY);
		memcpy(tmpbuf,SKY_GET_MW_TCACERT_DISPLAY(ind),MW_TCACERT_DISPLAY);
	}

	while(tmpbuf[i] != ';') Display.IssuerName[j++] = tmpbuf[i++];
	Display.IssuerName[j] = NULL; i++; j = 0;
	while(tmpbuf[i] != ';') Display.NotAfter[j++] = tmpbuf[i++];
	Display.NotAfter[j] = NULL; i++; j = 0;
	while(tmpbuf[i] != ';' && tmpbuf[i] != NULL) Display.CertificateUsage[j++] = tmpbuf[i++];
	Display.CertificateUsage[j] = NULL; j = 0;

	if(tmpbuf[i] == ';') {
		i++;
		while(tmpbuf[i] != NULL) {
			if(tmpbuf[i] == ';') {
				Display.Option[j++] = '\n';
			} else {
				Display.Option[j++] = tmpbuf[i++];
			}
		}
		Display.Option[j] = NULL;
		return 1;
	} else {
		memset(Display.Option,NULL,200);
		return 0;
	}
}

LOCAL void SKY_API sSKA_MWPRIcertInfo(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_START_CHILD(sSKA_MWPRICertSelect);
        break;

    case SE_APP_END:
        break;

	case SE_KEY_END : 
		EQS_END_TO(EQS_POP_TO_ROOT);
		break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSKA_MWPRICertSelect(SKY_EVENT_T *pEvent)
{
	UINT8	i,j,unIdx; //,AddrOfMy;
	static  BYTE*	MYCertInfo[MAX_MYCERT_INFO];
	static  BYTE	Temp[30];
	static  BYTE	StatusOfMY[MAX_MYCERT_INFO];
    static  UINT8   hList, TotalMYCert;
	static  WORD  OnLine1;
	static  DWORD	OnLine2;
	HCONTROL	hControl2;
	int len,linenum;

	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
	
			unIdx = 0; 
			OnLine1 = pEvent->wParam;  // ������ ���� ��  1�� ���� 
			OnLine2 = pEvent->dwParam; // signText 1�� ����
			
			if(OnLine2 == 1) EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_NONE,
						  SKY_GET_TOKEN(TKN_MW_PRI_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);

			for( i=0; i < MAX_MYCERT_INFO; i++ )
			{
				/* alloc. */
				// �켱�� �ӽ÷� ������ �� ���... ũ�⵵ �ӽ÷� 20
				MYCertInfo[i] = (BYTE *)SKY_Malloc( (SIZE_T) 20);   
				StatusOfMY[i] = SKY_GET_MW_MYCERT_DATA_SAVED(i);
			}

			hList = CreateList( MAX_MYCERT_INFO );
			for( i=0; i<MAX_MYCERT_INFO; i++ )
			{
				if(StatusOfMY[i] > 0){
					WhichOne = i;
					// SUBJECT_DN�� �о�´�. �׷���, ũ�Ⱑ 400byte�ΰ�...
					// ��.. �켱�� �߶� �����ְ�, Ư���κ��� searching�ؾ� �ϴ��� 
					// ã�ƺ���...
					memset((unsigned char *)Temp,NULL,30);
					memcpy((unsigned char *)Temp,SKY_GET_MW_MYCERT_DISPLAY(i),30);
					for(j=0;j<19;j++) { if(Temp[j] != ';' ) MYCertInfo[i][j] = Temp[j]; 
										else MYCertInfo[i][j] = NULL; }
					//AddList( hList, (INT16)(i), FALSE );
					// i : ���� EFS�� �ε��� 
					// StatusOfMY[i] : �����Ͱ� ����Ʈ�� �������� ����(�����Ͱ� ����Ǿ� �ִ����� �˷���.)
					AddList(hList,(INT16)(StatusOfMY[i]-1),FALSE);
					unIdx++;
				} 
			}
			TotalMYCert = unIdx;

			if(!OnLine2) { //  ������ ����
				EQC_ScrollList(EQS_NA, EQS_NA, EQS_NA, 
						   EQC_GetScrollDisplayLine(), 
						   MYCertInfo, 
						   TotalMYCert, 
						   TotalMYCert, 
						   0);

				EQC_SetScrollListHandle( EQS_GetNthIOC(0), hList );
				EQC_SET_STYLE(EQS_GetNthIOC(0), (EQC_GET_STYLE(EQS_GetNthIOC(0)) |
												CS_SCROLL_LIST_MAXIMIZE | 
												CS_SCROLL_LIST_ENUMBERATED));
				EQS_SetIOCActive(EQS_GetNthIOC(0));
			}
			else {  // online2 == 1  ���ڼ����
				len =  strlen(SKY_GET_TOKEN(TKN_MW_SELECT_PRICERT_SIGNING));
				linenum = len / MAX_WIDTH_LEN;
				memset(texttmp,NULL,100);
				memcpy(texttmp,SKY_GET_TOKEN(TKN_MW_SELECT_PRICERT_SIGNING),len);
				if(strlen(SKY_GET_TOKEN(TKN_MW_SELECT_PRICERT_SIGNING)) % MAX_WIDTH_LEN > 0 ) linenum ++;
				hControl2 = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_KOREAN,len,texttmp,0);
				EQC_SET_STYLE(hControl2,CS_TEXT_EDIT_READONLY);
				EQC_SET_FOCUS(hControl2,FALSE);
		        hControl2 = EQC_ScrollList(EQS_NA, EQS_NA, MAX_WIDTH_LEN, TotalMYCert, 
													MYCertInfo, TotalMYCert, TotalMYCert, 0);
				EQC_SET_STYLE(hControl2, CS_SCROLL_LIST_ENUMBERATED);

				EQS_StartFormatter();
				
				for(i = 0; i < EQS_GetIOCNum(); i++)
				{   
						EQS_SetControlPosition(EQS_GetNthIOC(i), 
							ES_AUTO, 0,	 EHA_CENTER, EVA_CENTER, FALSE);
						//EQC_GET_X(EQS_GetNthIOC(i)) -= (R_SCROLLBAR_SIZE)/2;
						EQS_NewLine();
				}

				EQS_EndFormatter();
				EQS_FitCanvasSize();
				EQS_SetIOCActive(EQS_GetNthIOC(1));
			}   
			break;

		case SE_APP_END :
			/* dealloc. */
			if(OnLine2 == 1) EQS_SetDirectDraw(TRUE);
			for( i=0; i<TotalMYCert; i++ )
			//for(i =0; i<MAX_MYCERT_INFO;i++)
			{
				SKY_Free( MYCertInfo[i] );
				if(i != 0)	{
					SKY_SET_MW_MYCERT_DATA_SAVED(i,StatusOfMY[i]);
					SKY_SET_MW_WALLET();
				}
			}
			break;

		case SE_APP_RESUME: //  resume�Ǿ ����Ǵ� �κ��� �����ϱ� �����ص� ���� ������..
			for( i=0; i < MAX_MYCERT_INFO; i++ )
				StatusOfMY[i] = SKY_GET_MW_MYCERT_DATA_SAVED(i);
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_SCROLL_LIST_SELECT:
			if(!OnLine2) {
				if( EQC_GetScrollDataLine(EQS_GetNthIOC(0)) > 0 )
				{
					unIdx = (UINT8)EQC_GetScrollLogicalIndex( EQS_GetNthIOC(0) );
					for(i=0;i<MAX_MYCERT_INFO;i++) {
						if(StatusOfMY[i] == unIdx+1) break;  // ����� +1 ��� ����.
					}
					//SKY_SET_MW_MYCERT_DATA_SAVED(i,StatusOfMY[i]); 
					//SKY_SET_MW_WALLET(); // ���� �̰� �� ������??
					//AddrOfMy = (UINT8)GetListValue( hList, unIdx ); //get the address

					// offline�� ��쿡�� �ش� �������� ������ ����������, 
					// online�� ��쿡�� �ش� �������� �����Ѵ�.
					if(!OnLine1) {
						EQS_START_CHILD_EVENT(sSKA_MWShowMYCertInfo, SE_APP_START, 0, i /*AddrOfMy*//*WhichOne*/);
					} else {
						// ����
						// MW_DeleteMYCert(i);					
						InitMWMyCert(i);
						// EFS index�� ����..
						EQS_END_PARAM(i,FALSE);
					}
				}
				else 
				{
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_NO_CONFIRM), 
								   MESSAGE_DEFAULT_TIME, 
								   MBT_NONE, 
								   MBT_HELP );
				}
			} else { // signText�� ������ ����
				if( EQC_GetScrollDataLine(EQS_GetNthIOC(1)) > 0 )
				{
					unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(1));
					for(i=0;i<MAX_MYCERT_INFO;i++) {
						if(StatusOfMY[i] == unIdx+1) break;  
					}
					// ������ �������� �ε����� ��򰡿� �����ϰ� �ش� ���Է��� �޴´�.
					// �ε��� ������ dwParam��...
					// �� �Է¹޴� ȭ������ ��ȯ....
					EQS_START_SIBLING_EVENT(SKA_CertPINInput,SE_APP_START,3,i,FALSE);

				} else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_NO_CONFIRM), 
								   MESSAGE_DEFAULT_TIME, 
								   MBT_NONE, 
								   MBT_HELP );
					EQS_END();  // �������� ���� �ÿ��� ���⼭ � Ư�� ���� �� Ȥ�� �̺�Ʈ�� ������ �Ѵ�.  EQS_END_PARAM(w,dw); EQS_END_EVENT(e,d,dw);					
				}
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END();
			break;

		case SE_KEY_CLEAR :
			EQS_END();
			break;

		case SE_KEY_END:	
			//EQS_END_CURRENT_EVENT();
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   
	
		default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}	
}

LOCAL void SKY_API sSKA_MWShowMYCertInfo(SKY_EVENT_T *pEvent)
{
	HCONTROL		hControl;
	int linenum,showopt,nCtrl;
	static WORD sel;
	static DWORD index;

	switch ( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_PRI_INFO),
						  WA_SHOW_HEADER | WA_SHOW_CML);

			sel = pEvent->wParam;
			index = pEvent->dwParam;
			showopt = MW_parse_display(index,1);

			hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
            sprintf(texttmp, "%s %s",SF_MW_CERT_ORG,SKY_GET_TOKEN(TKN_MW_CA_DISPLAY_ID));
            EQC_SetStaticText(hControl, texttmp);
			//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCertName);
			// Display.IssuerName
			linenum = strlen(Display.IssuerName);
			if(linenum % MAX_WIDTH_LEN != 0) {
				linenum = (linenum / MAX_WIDTH_LEN) + 1;
			} else {
				linenum = linenum / MAX_WIDTH_LEN;
			}
	        hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER, strlen(Display.IssuerName),&(Display.IssuerName), 0);
			EQC_SET_ENABLE(hControl, FALSE);
			EQC_SET_FOCUS(hControl,FALSE);

			hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
            sprintf(texttmp, "%s %s",SF_MW_CERT_VALID,SKY_GET_TOKEN(TKN_MW_CA_DISPLAY_DATE));
            EQC_SetStaticText(hControl, texttmp);
			//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCertTermOfValidity);
			// Display.NotAfter
			linenum = strlen(Display.NotAfter);
			if(linenum % MAX_WIDTH_LEN != 0) {
				linenum = (linenum / MAX_WIDTH_LEN) + 1;
			} else {
				linenum = linenum / MAX_WIDTH_LEN;
			}
	        hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER, strlen(Display.NotAfter),&(Display.NotAfter), 0);
			EQC_SET_ENABLE(hControl, FALSE);
			EQC_SET_FOCUS(hControl,FALSE);

			hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
            sprintf(texttmp, "%s %s",SF_MW_CERT_CERT,SKY_GET_TOKEN(TKN_MW_CA_DISPLAY_USAGE));
            EQC_SetStaticText(hControl, texttmp);
			//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCertUsage);
			// Display.CertificateUsage
			linenum = strlen(Display.CertificateUsage);
			if(linenum % MAX_WIDTH_LEN != 0) {
				linenum = (linenum / MAX_WIDTH_LEN) + 1;
			} else {
				linenum = linenum / MAX_WIDTH_LEN;
			}
	        hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER, strlen(Display.NotAfter),&Display.CertificateUsage, 0);
			EQC_SET_ENABLE(hControl, FALSE);
			EQC_SET_FOCUS(hControl,FALSE);

			if (showopt == 1) {
				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_CERT_ETC,SKY_GET_TOKEN(TKN_MW_CA_DISPLAY_OPT));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCertEtc);
				// Display.Option
				linenum = strlen(Display.Option);
				if(linenum % MAX_WIDTH_LEN != 0) {
					linenum = (linenum / MAX_WIDTH_LEN) + 1;
				} else {
					linenum = linenum / MAX_WIDTH_LEN;
				}
				hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER,strlen(Display.Option),&Display.Option, 0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl,FALSE);
			}

			EQS_StartFormatter();

			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();
			break;

		case SE_KEY_END:	
			//EQS_END_CURRENT_EVENT();
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;   

		case SE_KEY_CENTER :
			EQS_END();
			break;

		case SE_SOFTKEY_CONFIRM : // ����Ű�� ���� ���� �Ҽ��� ����. MW���� 
			if(sel == 1) {
				EQS_START_SIBLING_EVENT(SKA_CertPINInput,SE_APP_START,2,index,FALSE);
			} else {
				EQS_END();
			}
			break;

		case SE_KEY_CLEAR : 
			EQS_END();
			break;

		default :
	        EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWInitialize(SKY_EVENT_T *pEvent)
{
    BYTE	sPasswd[5];
	static BOOL IsEnd;
    HCONTROL sNumHandle;
	static UINT		checklock;
	
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_MW_PASSWD_CHECK), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
        
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT); 
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_FitCanvasSize();
            EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
		if (IsEnd ) {
			// �����޴��� �̵� 
			EQS_END(); //EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
		}
		else {
			EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
			EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
		}
        break; 

    case EQE_IO_NEDIT_MATCH_LENGTH:                        
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);

		// ��й�ȣ �Է� �õ� Ƚ��
		checklock = SKY_GET_MW_WALLET_TRIAL_NUM();

        MSG_HIGH("��й�ȣ �Էµ���.. �޸� : #%d#",checklock,0,0);

		// NV�� ����� PIN���� �о�´�. 
		memcpy((const char *)gHashedPIN, SKY_GET_MW_WALLET_HASHEDPIN(),MW_WALLET_HASHEDPIN_LENGTH);  

		// ����ڰ� �Է��� PIN�� �����ϴ� �κ� 
		if (!wwVerifyPIN(sPasswd)) {
			memcpy(currentPIN,(const char *)sPasswd, 4);
	        MSG_HIGH("�޸��� ��й�ȣ : ##%s##",(CHAR *)SKY_GET_MW_WALLET_PIN(),0,0);
			pinTrialCounter = INIT_TRIAL_NUM; IsEnd = TRUE;
			EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INITIALIZE_INFORM_MSG), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			Initialize_MW();
		}
        else {
			pinTrialCounter++; 
			// password �Է� �õ�Ƚ��(3)���� ������� ���� ��� ���� �޴��� �̵�
			if (pinTrialCounter > MAX_PASSWD_TRIAL) { 
				pinTrialCounter = INIT_TRIAL_NUM; checklock ++;
				SKY_SET_MW_WALLET_TRIAL_NUM(checklock);
				MSG_HIGH("�޸��� ��й�ȣ ���� : ##%d##",SKY_GET_MW_WALLET_TRIAL_NUM(),0,0);
				// ��й�ȣ 3�� �Է½��а� ���� ���������� �߻��� ���
				if (checklock > MAX_PASSWD_TRIAL) {
					pinTrialCounter = INIT_TRIAL_NUM; checklock = INIT_TRIAL_NUM;
			        MSG_HIGH("�޸� �ʱ�ȭ�� �����մϴ�. ",0,0,0);
					IsEnd = TRUE;
					// �������� ���� �ʱ�ȭ
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INITIALIZE_INFORM_MSG), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					Initialize_MW();
				}
				// ���� �޴��� �̵�
				else EQS_END();//EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
			}
			else {  // �н����� ���Է��� �䱸 "��й�ȣ�� Ʋ���ϴ�."
				IsEnd = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}
		} 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        break;

	case SE_KEY_CLEAR : 
		EQS_END();
		break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL HCONTROL sAddInputControl(UINT8 nInsertPosition, UINT8 nViewLine, MW_CONTROL_E Type ,BOOL fFocus)
{
	HCONTROL	hControl1, hControl2; 
	BYTE		*pToken[2];

    switch( Type ) 
    {
	case ICT_U_TITLE_E :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_MW_USER_INFO);
		break;

    case ICT_U_NAME_E :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_U_NAME,SKY_GET_TOKEN(TKN_MW_USER_INFO_NAME));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelName); 
        //hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_MW_USER_INFO_NAME);
        hControl2 = EQC_TextEdit(EQS_NA,EQS_NA, MAX_WIDTH_LEN, nViewLine, \
                                TEM_KOREAN, USER_INFO_NAME,(BYTE*)"", 0);

        break;

	case ICT_U_PHONE_E:
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_U_PHONE,SKY_GET_TOKEN(TKN_MW_USER_INFO_PHONE));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelPhoneNumber);
		//hControl1 = EQC_StaticText(EQS_NA,EQS_NA , TKN_MW_USER_INFO_PHONE);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA ,MAX_WIDTH_LEN, nViewLine, \
			                      USER_INFO_PHONE, (BYTE*)"" );
		strcpy(&userinfo.userPhone,sGetMyNumber());
		EQC_SetNumberEditValue(hControl2, &userinfo.userPhone);           // ��ȭ��ȣ�� disable ���·� ��� 
		EQC_SET_ENABLE(hControl2, FALSE);                           // ska_call.c�� GetMyNumber�Լ� ���. 
            
        break;

	case ICT_U_EMAIL_E:    
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_U_EMAIL,SKY_GET_TOKEN(TKN_MW_USER_INFO_EMAIL));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelEMail);
        //hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_MW_USER_INFO_EMAIL);
        hControl2 = EQC_TextEdit(EQS_NA,EQS_NA , MAX_WIDTH_LEN, nViewLine, \
                                TEM_ENGLISH_LOWER, USER_INFO_EMAIL,(BYTE*)"", 0);
        break;

/*	case ICT_D_TITLE_E : 
        hControl1 = EQC_StaticText(Margin.xLeft,yPos,TKN_MW_DELIVERY_INFO_TITLE);
        hControl2 = EQC_TextEdit(Margin.xLeft+xSpace,yPos, DELIVERY_INFO_TITLE, nViewLine, \
                                TEM_KOREAN, DELIVERY_INFO_TITLE,(BYTE*)"", 0);
        break; */

	case ICT_D_TITLE_E :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_MW_DELIVERY_INFO);
		break;

	case ICT_D_NAME_E : 
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_D_RNAME,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_NAME));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelReceiverName);
        //hControl1 = EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_DELIVERY_INFO_NAME);
        hControl2 = EQC_TextEdit(EQS_NA,EQS_NA , MAX_WIDTH_LEN, nViewLine, \
                                TEM_KOREAN, DELIVERY_INFO_NAME,(BYTE*)"", 0);
        break;

	case ICT_D_ZIPCODE_E :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_D_ZIPCODE,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_ZIP));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelZipCode);
		//hControl1 = EQC_StaticText(EQS_NA,EQS_NA , TKN_MW_DELIVERY_INFO_ZIP);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA ,MAX_WIDTH_LEN, nViewLine, \
			                      DELIVERY_INFO_ZIP, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
        break;

	case ICT_D_ADDRESS_E :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_D_ADDR,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_ADDR));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelAddress);
        //hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_MW_DELIVERY_INFO_ADDR);
        hControl2 = EQC_TextEdit(EQS_NA,EQS_NA , MAX_WIDTH_LEN, nViewLine, \
                                TEM_KOREAN, DELIVERY_INFO_ADDR,(BYTE*)"", 0);
        break;

	case ICT_D_PHONE_E : 
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_D_RPHONE,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_PHONE));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelReceiverPhone);
		//hControl1 = EQC_StaticText(EQS_NA,EQS_NA , TKN_MW_DELIVERY_INFO_PHONE);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA ,MAX_WIDTH_LEN, nViewLine, \
			                      DELIVERY_INFO_PHONE, (BYTE*)"" );
             
        break;

	case ICT_C_TITLE_E :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_MW_CARD_INFO);
		break;

	case ICT_C_NUMBER_E : 
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_C_NUM,SKY_GET_TOKEN(TKN_MW_CARD_INFO_NUM));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelCreditCardNum);
		//hControl1 = EQC_StaticText(EQS_NA,EQS_NA , TKN_MW_CARD_INFO_NUM);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA ,MAX_WIDTH_LEN, nViewLine, \
			                      CARD_INFO_NUMBER, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
        break;

	case ICT_C_PWD_E : 
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_C_PASSWD,SKY_GET_TOKEN(TKN_MW_CARD_INFO_PWD));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelPassWord);
		//hControl1 = EQC_StaticText(EQS_NA,EQS_NA , TKN_MW_CARD_INFO_PWD);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA ,MAX_WIDTH_LEN, nViewLine, \
			                      CARD_INFO_PWD, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE | CS_NUMBER_EDIT_PASSWD);
        break;

	case ICT_C_VALID_E : 
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_C_VALID,SKY_GET_TOKEN(TKN_MW_CARD_INFO_VALID));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelTermOfValidity);
		//hControl1 = EQC_StaticText(EQS_NA,EQS_NA , TKN_MW_CARD_INFO_VALID);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA ,MAX_WIDTH_LEN, nViewLine, \
			                      CARD_INFO_VALID, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
        break;

	case ICT_C_RNO_E :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_C_RNO,SKY_GET_TOKEN(TKN_MW_CARD_INFO_RNO));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelResidentRegistrationNumber);
        //hControl1 = EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_CARD_INFO_RNO);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA ,MAX_WIDTH_LEN, nViewLine, \
			                      CARD_INFO_RNO, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
        break;

	case ICT_P_TITLE_E :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_MW_OCB_INFO);
		break;

	case ICT_P_NUMBER_E : 
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_P_NUM,SKY_GET_TOKEN(TKN_MW_OCB_INFO_NUM));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA ,&g_BmpMWalletPannelOKCardNumber);
		//hControl1 = EQC_StaticText(EQS_NA,EQS_NA , TKN_MW_OCB_INFO_NUM);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN, nViewLine, \
			                      POINT_INFO_NUMBER, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
              
        break;

    case ICT_P_PWD_E : 
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
		sprintf(texttmp, "%s %s",SF_MW_P_PASSWD,SKY_GET_TOKEN(TKN_MW_OCB_INFO_PWD));
		EQC_SetStaticText(hControl1, texttmp);
		//hControl1 = EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelOKCashbagPassWord);
		//hControl1 = EQC_StaticText(EQS_NA,EQS_NA , TKN_MW_OCB_INFO_PWD);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA ,MAX_WIDTH_LEN, nViewLine, \
			                      POINT_INFO_PWD, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE | CS_NUMBER_EDIT_PASSWD);
        break;

	case ICT_S_CARDONLY_E :
		pToken[0] = SKY_GET_TOKEN(TKN_MW_CARD_CHANGE);
		hControl1 = EQC_ScrollList(EQS_NA,EQS_NA,MAX_WIDTH_LEN,1,pToken,1,1,0);
		EQC_SET_STYLE( hControl1, /*CS_SCROLL_LIST_BORDER |*/ CS_SCROLL_LIST_ENUMBERATED);
		break;

	case ICT_S_DELIVONLY_E :
		pToken[0] = SKY_GET_TOKEN(TKN_MW_DELIVERY_CHANGE);
		hControl1 = EQC_ScrollList(EQS_NA,EQS_NA,MAX_WIDTH_LEN,1,pToken,1,1,0);
		EQC_SET_STYLE( hControl1, /*CS_SCROLL_LIST_BORDER |*/ CS_SCROLL_LIST_ENUMBERATED);
		break;

	case ICT_S_ALL_E :
		pToken[0] = SKY_GET_TOKEN(TKN_MW_CARD_CHANGE);    
		pToken[1] = SKY_GET_TOKEN(TKN_MW_DELIVERY_CHANGE);
		hControl1 = EQC_ScrollList(EQS_NA,EQS_NA ,MAX_WIDTH_LEN,2,pToken,2,2,0);
		EQC_SET_STYLE( hControl1, /*CS_SCROLL_LIST_BORDER |*/ CS_SCROLL_LIST_ENUMBERATED);
		break;

	case ICT_S_SIGNING_E :
		EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_ASK_FOR_SIGNING);
		break;

	case ICT_C_DELETE_OR_CANCEL_E :
		pToken[0] = SKY_GET_TOKEN(TKN_MW_DELETE_CERT);    
		pToken[1] = SKY_GET_TOKEN(TKN_MW_CANCEL_CERT);
		hControl1 = EQC_ScrollList(EQS_NA,EQS_NA ,MAX_WIDTH_LEN,2,pToken,2,2,0);
		EQC_SET_STYLE( hControl1, CS_SCROLL_LIST_ENUMBERATED);
		break;

	case ICT_S_DOWNLOAD :
		hControl1 = EQC_TextEdit(EQS_NA,EQS_NA , MAX_WIDTH_LEN, nViewLine, \
                                TEM_KOREAN, 100,(BYTE*)(SKY_GET_TOKEN(TKN_MW_CERT_DOWNLOAD_SUCCESS)), 0);
		EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
		EQC_SET_FOCUS(hControl1,FALSE);
		break;

	case ICT_S_SUCCESS :
		hControl1 = EQC_TextEdit(EQS_NA,EQS_NA, MAX_WIDTH_LEN, nViewLine, \
                        TEM_KOREAN, 100,(BYTE*)(SKY_GET_TOKEN(TKN_MW_CERT_SUCCESS)), 0);
		EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
		EQC_SET_FOCUS(hControl1,FALSE);
		break;

	case ICT_E_FORMAT :
		hControl1 = EQC_TextEdit(EQS_NA,EQS_NA, MAX_WIDTH_LEN, nViewLine, \
                        TEM_KOREAN, 100,(BYTE*)(SKY_GET_TOKEN(TKN_MW_CERT_FORMAT_ERROR)), 0);
		EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
		EQC_SET_FOCUS(hControl1,FALSE);
		break;

	case ICT_E_DOWNLOAD :
		hControl1 = EQC_TextEdit(EQS_NA,EQS_NA , MAX_WIDTH_LEN, nViewLine, \
                        TEM_KOREAN, 100,(BYTE*)(SKY_GET_TOKEN(TKN_MW_CERT_DOWNLOAD_FAIL)), 0);
		EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
		EQC_SET_FOCUS(hControl1,FALSE);
		break;

	case ICT_E_NOSPACE :
		hControl1 = EQC_TextEdit(EQS_NA,EQS_NA , MAX_WIDTH_LEN, nViewLine, \
                        TEM_KOREAN, 100,(BYTE*)(SKY_GET_TOKEN(TKN_MW_ASK_FOR_DELETE_CERT)), 0);
		EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
		EQC_SET_FOCUS(hControl1,FALSE);
		break;

	case ICT_E_FAIL :
		hControl1 = EQC_TextEdit(EQS_NA,EQS_NA , MAX_WIDTH_LEN, nViewLine, \
                        TEM_KOREAN, 100,(BYTE*)(SKY_GET_TOKEN(TKN_MW_CERT_ERROR)), 0);
		EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
		EQC_SET_FOCUS(hControl1,FALSE);
		break;


	case ICT_T_INPUT1 :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_TEST_SIGNINFO);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA,3, nViewLine, 3, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
		break;

	case ICT_T_INPUT2 :
		hControl1 = EQC_StaticText(EQS_NA,EQS_NA ,TKN_MW_TEST_ENCRYPTINFO);
        hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA,3, nViewLine, 3, (BYTE*)"" );
		EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
						CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
		break;

    default:    

        return INVALID_CONTROL_HANDLE;
    
		break;
    }

/*	
	EQS_StartFormatter();
	
	for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
	{   
			EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
				ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
			EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
			EQS_NewLine();
	}

	EQS_EndFormatter();
	EQS_FitCanvasSize();
*/

    if( fFocus == TRUE )
        EQS_SetIOCFocus( hControl2 );

    return hControl2;    
}

/* ==================================================================
       �ʱ�ȭ �Լ��� 
	 ================================================================== */
void Initialize_MW(void)
{
	userInputStatus = 0;
	cardInputStatus = 0;
	ocbInputStatus = 0;
	deliveryInputStatus = 0;

	InitMWallet(0);
}

void MW_init_user(void)
{
    memset((void *)userinfo.userName,  NULL, 11);
    memset((void *)userinfo.userPhone, NULL, 13);
    memset((void *)userinfo.userEmail, NULL, 25);
}

void MW_init_card(void)
{
	memset((void *)cardinfo.cardNO,   NULL, 19);
	memset((void *)cardinfo.cardPwd,  NULL, 5);
	memset((void *)cardinfo.cardValid,  NULL, 5);
	memset((void *)cardinfo.cardRno,  NULL, 14);
}

void MW_init_ocb(void)
{
	memset((void *)ocbinfo.pointNo,   NULL, 17);
	memset((void *)ocbinfo.pointPwd,  NULL, 5);
}

void MW_init_delivery(void)
{
	//    memset((void *)deliveryinfo.dvTitle,   NULL, 17);
		memset((void *)deliveryinfo.dvName,  NULL, 11);
		memset((void *)deliveryinfo.dvPhone, NULL, 13);
		memset((void *)deliveryinfo.dvZip,   NULL, 7);
		memset((void *)deliveryinfo.dvAddr,  NULL, 81);
}

/* ==================================================================
     �������� Read/Write�Լ�
	 ==================================================================  */
int MW_read_user(void)
{
    memcpy(encPerson, (void *)SKY_GET_MW_WALLET_USER(), MW_WALLET_USER_LENGTH);
    
    pUserInfo = (WWPersonalInfo *)wwGetData(currentPIN, MOBILE_WALLET_USER);

    //wwGetData returns a pointer to structure WWPersonalInfo which has decrypted user data
	//string length + 1 for NULL 
    memcpy((void *)userinfo.userName,     (void *)pUserInfo->userName,  USER_INFO_NAME+1);
    memcpy((void *)userinfo.userPhone,    (void *)pUserInfo->userPhone, USER_INFO_PHONE+1);
    memcpy((void *)userinfo.userEmail,    (void *)pUserInfo->userEmail, USER_INFO_EMAIL+1);

    //Only if there is something on NV for user info, ui_wallet_check_init() returns true.
    //So we can add that mandatory user infomations are ready. Go on saving
    MW_add_input_status(MOBILE_WALLET_USER, USER_FIELD);
	return 1;
}

int MW_read_card(void)
{
//	index = WhichOne - MOBILE_WALLET_CARD1;

	memcpy((unsigned char *)gHashedPIN, (void *)SKY_GET_MW_WALLET_HASHEDPIN(), MW_WALLET_HASHEDPIN_LENGTH);

    //Copy user data to global encrypted user data
    //encPerson will be used as an argument of decryption function "wwGetPerInfo"
/*	if(WhichOne == MOBILE_WALLET_CARD1)
		memcpy(encCCard1,  (void *)SKY_GET_MW_WALLET_CARD(index), MW_WALLET_CARD_LENGTH);
	else if(WhichOne == MOBILE_WALLET_CARD2)
		memcpy(encCCard2,  (void *)SKY_GET_MW_WALLET_CARD(index), MW_WALLET_CARD_LENGTH);
	else if(WhichOne == MOBILE_WALLET_CARD3)
		memcpy(encCCard3,  (void *)SKY_GET_MW_WALLET_CARD(index), MW_WALLET_CARD_LENGTH); */

    pCardInfo = (WWCreditCard *)wwGetData(currentPIN, WhichOne);

    //wwGetData returns a pointer to structure WWPersonalInfo which has decrypted user data
    memcpy((void *)cardinfo.cardNO,(void *)pCardInfo->cardNO,CARD_INFO_NUMBER+1);
    memcpy((void *)cardinfo.cardPwd,(void *)pCardInfo->cardPwd,CARD_INFO_PWD+1);
    memcpy((void *)cardinfo.cardValid,(void *)pCardInfo->cardValid,CARD_INFO_VALID+1);
    memcpy((void *)cardinfo.cardRno,(void *)pCardInfo->cardRno,CARD_INFO_RNO+1);

    //Only if there is something on NV for user info, ui_wallet_check_init() returns true.
    //So we can add that mandatory user infomations are ready. Go on saving
	MW_add_input_status(WhichOne, CARD_FIELD);  

	return 1;
}

int MW_read_ocb(void)
{
	memcpy((unsigned char *)gHashedPIN, (void *)SKY_GET_MW_WALLET_HASHEDPIN(), MW_WALLET_HASHEDPIN_LENGTH);

    //Copy user data to global encrypted user data
    //encPerson will be used as an argument of decryption function "wwGetPerInfo"
//	memcpy(encOCB1,  (void *)SKY_GET_MW_WALLET_OCB(), MW_WALLET_OCB_LENGTH);

    pOCBInfo = (WWCardExtInfo *)wwGetData(currentPIN, WhichOne);

    //wwGetData returns a pointer to structure WWPersonalInfo which has decrypted user data
    memcpy((void *)ocbinfo.pointNo,(void *)pOCBInfo->pointNo,POINT_INFO_NUMBER+1);
    memcpy((void *)ocbinfo.pointPwd,(void *)pOCBInfo->pointPwd,POINT_INFO_PWD+1);

    //Only if there is something on NV for user info, ui_wallet_check_init() returns true.
    //So we can add that mandatory user infomations are ready. Go on saving
	MW_add_input_status(WhichOne, POINT_FIELD);

	return 1;
}

int MW_read_delivery(void)
{
	index = WhichOne - MOBILE_WALLET_DELIVERY1;

	if(WhichOne == MOBILE_WALLET_DELIVERY1)
		memcpy(encDel1,  (void *)SKY_GET_MW_WALLET_DELIVERY(index), MW_WALLET_DELIVERY_LENGTH);
	else if(WhichOne == MOBILE_WALLET_DELIVERY2)
		memcpy(encDel2,  (void *)SKY_GET_MW_WALLET_DELIVERY(index), MW_WALLET_DELIVERY_LENGTH);

    //memcpy(dOCBiv, (void *)SKY_GET_MW_WALLET_DDELIV(), MW_WALLET_DOCBIV_LENGTH);
    //dDellen = SKY_GET_MW_WALLET_DELIVERY_LENGTH();

    pDeliveryInfo = (WWDeliveryInfo *)wwGetData(currentPIN, WhichOne);

    //wwGetData returns a pointer to structure WWPersonalInfo which has decrypted user data
    memcpy((void *)deliveryinfo.dvName,(void *)pDeliveryInfo->dvName,DELIVERY_INFO_NAME+1);
    memcpy((void *)deliveryinfo.dvPhone,(void *)pDeliveryInfo->dvPhone,DELIVERY_INFO_PHONE+1);
    memcpy((void *)deliveryinfo.dvZip,(void *)pDeliveryInfo->dvZip,DELIVERY_INFO_ZIP+1);
    memcpy((void *)deliveryinfo.dvAddr,(void *)pDeliveryInfo->dvAddr,DELIVERY_INFO_ADDR+1);

    //Only if there is something on NV for user info, ui_wallet_check_init() returns true.
    //So we can add that mandatory user infomations are ready. Go on saving
	MW_add_input_status(WhichOne, DELIVERY_FIELD);
	return 1;
}

void MW_write_user(void)
{
	wwSetData(&userinfo, currentPIN, MOBILE_WALLET_USER);

	SKY_SET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_USER,1);
//	SKY_SET_MW_WALLET_USER_LENGTH(dPerlen);
    SKY_SET_MW_WALLET_USER(encPerson,MW_WALLET_USER_LENGTH);

	SKY_SET_MW_WALLET();
}

void MW_write_card(void)
{
//	index = WhichOne - MOBILE_WALLET_CARD1;

	memcpy((unsigned char *)gHashedPIN, (void *)SKY_GET_MW_WALLET_HASHEDPIN(), MW_WALLET_HASHEDPIN_LENGTH);

	wwSetData(&cardinfo, currentPIN, WhichOne);

/*	if(WhichOne == MOBILE_WALLET_CARD1) {
		SKY_SET_MW_WALLET_CARD(index,encCCard1,MW_WALLET_CARD_LENGTH);
		SKY_SET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_CARD1,1);
	} else if(WhichOne == MOBILE_WALLET_CARD2) {
		SKY_SET_MW_WALLET_CARD(index,encCCard2,MW_WALLET_CARD_LENGTH);
		SKY_SET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_CARD2,1);
	} else if(WhichOne == MOBILE_WALLET_CARD3) {
		SKY_SET_MW_WALLET_CARD(index,encCCard3,MW_WALLET_CARD_LENGTH);
		SKY_SET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_CARD3,1);
    }
	
	SKY_SET_MW_WALLET();   
*/
}

void MW_write_ocb(void)
{
	memcpy((unsigned char *)gHashedPIN, (void *)SKY_GET_MW_WALLET_HASHEDPIN(), MW_WALLET_HASHEDPIN_LENGTH);

	wwSetData(&ocbinfo, currentPIN, MOBILE_WALLET_OCB);

	//SKY_SET_MW_WALLET_OCB_LENGTH(dOCBlen);
/*    SKY_SET_MW_WALLET_OCB(encOCB1,MW_WALLET_OCB_LENGTH);
	SKY_SET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_OCB,1);   

	SKY_SET_MW_WALLET();  */
}

void MW_write_delivery(void)
{
	index = WhichOne - MOBILE_WALLET_DELIVERY1;

    wwSetData(&deliveryinfo, currentPIN, WhichOne);

	if(WhichOne == MOBILE_WALLET_DELIVERY1) {
		SKY_SET_MW_WALLET_DELIVERY(index,encDel1,MW_WALLET_DELIVERY_LENGTH);
		SKY_SET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_DELIVERY1,1);
	} else if(WhichOne == MOBILE_WALLET_DELIVERY2) {
		SKY_SET_MW_WALLET_DELIVERY(index,encDel2,MW_WALLET_DELIVERY_LENGTH);
		SKY_SET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_DELIVERY2,1);
	}

	SKY_SET_MW_WALLET();
}

void MW_add_input_status(int type, int member)
{
//At first, we should look up each items by selecting menu and press OK for
//marking its input status
    switch (type)
    {
		case 0 :
			userInputStatus |= member;
			break;
		case 1 :
		case 2 :
			deliveryInputStatus |= member;
			break;
		case 3 :
		case 4 :
		case 5 :
			cardInputStatus |= member;
			break;
		case 6 :
		case 7 :
		case 8 :
			ocbInputStatus |= member;
			break;

		default:
			break;
    }
}

/******************************************************************************************
 **    Online Menu  
 ******************************************************************************************/

// ���ڼ����� ����  Ű �Է� �޴� �Լ�  
void SKY_API SKA_CertPINInput(SKY_EVENT_T *pEvent)
{
	int len,linenum,nCtrl;
    static HCONTROL sNumHandle, hControl;
	static BOOL once,PINerror = 0;
	static int index;
	BYTE buf[20];

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        {
			EQS_SetDirectDraw(FALSE);
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_CONFIRM_NONE, 
                SKY_GET_TOKEN(TKN_MW_SIGN_PASSWD_CHECK), 
                WA_SHOW_HEADER| WA_SHOW_CML);

/*            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
*/
			once  = pEvent->wParam;
			index = pEvent->dwParam;

			len =  strlen(SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_PASSWD_INFO));
			linenum = len / MAX_WIDTH_LEN;
			memset(texttmp,NULL,100);
			memcpy(texttmp,SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_PASSWD_INFO),len);
			if(strlen(SKY_GET_TOKEN(TKN_MW_SELECT_TEMPCERT_DELETED)) % MAX_WIDTH_LEN > 0 ) linenum ++;
			hControl = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_KOREAN,len,texttmp,0);
			//EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 16, (BYTE*)"",0);
			EQC_SET_STYLE(hControl,CS_TEXT_EDIT_READONLY);
			EQC_SET_FOCUS(hControl,FALSE);

			EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_SIGN_PASSWD_CHECK);

            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            //sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 8, 1, 8,(BYTE *)"");
			sNumHandle = EQC_NumberEdit(EQS_NA, EQS_NA, 8, 1, 8,(BYTE *)"");

            EQC_SET_STYLE(sNumHandle,SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQS_SetIOCActive(sNumHandle);

			EQS_StartFormatter();
			
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_CENTER, EVA_CENTER, FALSE);
					EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();

        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(sNumHandle));
        break;

    case SE_IO_NEDIT_DONE:
		memset(certPIN,NULL,9);
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, certPIN);
		switch(once) {
		case 0 :
			{
				if (strlen(certPIN) > 3) {
					EQC_SetNumberEditValue(sNumHandle, (BYTE*)"");
					EQS_SetIOCActive(sNumHandle);
					EQS_START_CHILD(sSKA_CertPINInputAgain);  
				} else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_PASSWD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
			} break;
		case 1 :  
			{
				if(strlen(certPIN) > 3) {
					// during getMWdata process.. 
					EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, MMI_NO_ERROR);
				} else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_PASSWD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				}
			} break;
		case 2 :
			{
				if(strlen(certPIN) > 3) {
					// during cert revocation process.. 
					// compare PIN and remove the MyCert
					SHA1((const unsigned char *)certPIN, strlen(certPIN), buf);
					if(!strcmp(buf,SKY_GET_MW_MYCERT_PIN_HASH(index))) {
						// delete the MyCert
						InitMWMyCert(index);
						SKY_SET_MW_MYCERT(index);
						SKY_SET_MW_MYCERT_DATA_SAVED(index,0);
						SKY_SET_MW_WALLET();

						// ������ ������ ���÷���?? MW����

						EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, REQUEST_CERT, MMI_NO_ERROR);
					} else {
						// ������ ������ ����..���÷���....�ʿ��ҵ�..
						EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_WRONG_PASSWD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
						//EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, REQUEST_CERT, USER_CANCEL);
					}
				} else {
					PINerror = 1;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_PASSWD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				}
			} break;
		case 3 : 
			{
				if(strlen(certPIN) > 3) {
					// during SignText process 
					EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, SIGN_TEXT, MMI_NO_ERROR);
				} else {
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_PASSWD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
				}

			} break;
		}
        break;
        
    case SE_IO_NEDIT_NODIGIT:
        break;

	case SE_IO_MESGBOX_END : 
		if (PINerror) {
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, REQUEST_CERT, USER_CANCEL);
		} else {
			EQC_SetNumberEditValue(sNumHandle, (BYTE*)"");
			EQS_SetIOCActive(sNumHandle);
		}
		break;

    case SE_APP_END :   /* The very last event */
		EQS_SetDirectDraw(TRUE);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
		EQS_SetDirectDraw(FALSE);
		if(pEvent->wParam == 1) { 
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, REQUEST_CERT, MMI_NO_ERROR); 
		} else {
			EQC_SetNumberEditValue(sNumHandle, (BYTE*)"");
			EQS_SetIOCActive(sNumHandle);
		}
        break;

    case SE_APP_DRAW:
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSKA_CertPINInputAgain(SKY_EVENT_T *pEvent)
{
	int len,linenum,nCtrl;
	char certPINtmp[9];
    static HCONTROL sNumHandle, hControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        {
			EQS_SetDirectDraw(FALSE);
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_CONFIRM_CANCEL, 
                SKY_GET_TOKEN(TKN_MW_SIGN_PASSWD_CHECK), 
                WA_SHOW_HEADER| WA_SHOW_CML);

			len =  strlen(SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_PASSWD_INFO_AGAIN));
			linenum = len / MAX_WIDTH_LEN;
			memset(texttmp,NULL,100);
			memcpy(texttmp,SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_PASSWD_INFO_AGAIN),len);
			if(strlen(SKY_GET_TOKEN(TKN_MW_SELECT_TEMPCERT_DELETED)) % MAX_WIDTH_LEN > 0 ) linenum ++;
			hControl = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_KOREAN,len,texttmp,0);
			EQC_SET_STYLE(hControl,CS_TEXT_EDIT_READONLY);
			EQC_SET_FOCUS(hControl,FALSE);

			EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_SIGN_PASSWD_CHECK);

			sNumHandle = EQC_NumberEdit(EQS_NA, EQS_NA, 8, 1, 8,(BYTE *)"");

            EQC_SET_STYLE(sNumHandle, CS_NUMBER_EDIT_PASSWD | \
							CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_NO_CML |\
                            CS_NUMBER_EDIT_BORDER);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
			EQS_FitCanvasSize();
            EQS_SetIOCActive(sNumHandle);

			EQS_StartFormatter();
			
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_CENTER, EVA_CENTER, FALSE);
					EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();

        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(sNumHandle));//EQS_End();
        break;

    case SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, certPINtmp);
		if(!strcmp(certPINtmp,certPIN)){
			EQS_END_PARAM(1,FALSE);
		} else {
			EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_CERT_SIGNING_WRONG_PASSWD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 			
		}
        break;

	case SE_SOFTKEY_CANCEL :
		EQS_END_EVENT(SE_APP_END,EQS_NA,EQS_NA);
		break;
        
    case SE_IO_NEDIT_NODIGIT:
        break;

	case SE_IO_MESGBOX_END : 
		EQS_END();
		break;

    case SE_APP_END :   /* The very last event */
		EQS_SetDirectDraw(TRUE);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

// ---------------------------------------------------------------------------------- GetMWData �Լ� ���� �ϳ�.. 
// SignInfo�� EncryptInfo�� ���� ����ڿ��� � ������ ���÷��� ���� �����Ѵ�.
void SKY_API SKA_process_GetMWData(SKY_EVENT_T *pEvent)
{

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE); 

			if(clnt_get_cert_index(getMobileWalletData.keyIdType,getMobileWalletData.keyId) == -1){
				EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, NO_CERTIFICATE); 
			} 
			// appendstring�� �ִ��� ������ üũ�� �ʿ�...


			if( ((getMobileWalletData.setStringEnc & appendInfo) == INFO_RESET) && \
				((getMobileWalletData.setStringSign & appendInfo) == INFO_RESET) ) {
				// �Ѵ� reset�Ǿ� �ִ� ���.. ȭ�鿡 appendString�� ����Ѵ�.
				if(strlen(getMobileWalletData.appendString) > 0) {  // appendString�� �ִ� ��쿡��.
					EQS_START_SIBLING(sSKA_show_appendStringToMWdata,FALSE);
				} else {
					if( (getMobileWalletData.setStringEnc & \
						(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo) ) != INFO_RESET) {
						if( ((getMobileWalletData.setStringEnc & cardInfo) == cardInfo) ||
							((getMobileWalletData.setStringEnc & cardextInfo) == cardextInfo) ) {
							// encrypt�� �����Ͱ� ������ �������� pin�Է�ȭ������...
							EQS_START_SIBLING(sSKA_PasswdforEncrypt,FALSE);
						} else {
							// encrypt�� �����Ͱ� ������ encrypt ������ ǥ�� ȭ������...
							EQS_START_SIBLING(sSKA_DisplayEncryptData,FALSE);
						}
					} else {
						if( (getMobileWalletData.setStringSign & \
							(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo )) != INFO_RESET ) {
								EQS_START_SIBLING(sSKA_CheckForSign,FALSE);
						} else {
							EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, MMI_NO_ERROR);
						}
					}
				}
			} else if( ((getMobileWalletData.setStringEnc & appendInfo) == appendInfo) && \
				((getMobileWalletData.setStringSign & appendInfo) == INFO_RESET) ) {
				// FieldToEncrpt == 1 && FieldToSign == 0  ȭ�鿡 ��� 
				if(strlen(getMobileWalletData.appendString) > 0) {  // appendString�� �ִ� ��쿡��.
					EQS_START_SIBLING(sSKA_show_appendStringToMWdata,FALSE);
				} else {
					if( (getMobileWalletData.setStringEnc & \
						(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo) ) != INFO_RESET) {
						if( ((getMobileWalletData.setStringEnc & cardInfo) == cardInfo) ||
							((getMobileWalletData.setStringEnc & cardextInfo) == cardextInfo) ) {
							// encrypt�� �����Ͱ� ������ �������� pin�Է�ȭ������...
							EQS_START_SIBLING(sSKA_PasswdforEncrypt,FALSE);
						} else {
							// encrypt�� �����Ͱ� ������ encrypt ������ ǥ�� ȭ������...
							EQS_START_SIBLING(sSKA_DisplayEncryptData,FALSE);
						}
					} else {
						if( (getMobileWalletData.setStringSign & \
							(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo )) != INFO_RESET ) {
								EQS_START_SIBLING(sSKA_CheckForSign,FALSE);
						} else {
							EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, MMI_NO_ERROR);
						}
					}
				}
			} else if( (getMobileWalletData.setStringSign & appendInfo) == appendInfo ) {
				// FieldToSign�� appendinfo�� set�Ǿ� �ִ� ��� ȭ�鿡 ������� �ʴ´�.
				// ����, �������� �Ѿ ȭ���� �����Ѵ�.
				if( (getMobileWalletData.setStringEnc & \
					(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo) ) != INFO_RESET) {
					if( ((getMobileWalletData.setStringEnc & cardInfo) == cardInfo) ||
						((getMobileWalletData.setStringEnc & cardextInfo) == cardextInfo) ) {
						// encrypt�� �����Ͱ� ������ �������� pin�Է�ȭ������...
						EQS_START_SIBLING(sSKA_PasswdforEncrypt,FALSE);
					} else {
						// encrypt�� �����Ͱ� ������ encrypt ������ ǥ�� ȭ������...
						EQS_START_SIBLING(sSKA_DisplayEncryptData,FALSE);
					}
				} else {
					if( (getMobileWalletData.setStringSign & \
						(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo )) != INFO_RESET ) {
							EQS_START_SIBLING(sSKA_CheckForSign,FALSE);
					} else {
						EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, MMI_NO_ERROR);
					}
				}
			}

			break;  

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_APP_RESUME: /* child applet is ended */
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}	

}

// appendStringInfo�� �Ѵ� Reset�Ǿ� ���� ���, Ȯ�λ���(appendString)�� ���÷����Ѵ�.
LOCAL void SKY_API sSKA_show_appendStringToMWdata(SKY_EVENT_T *pEvent)
{
	HCONTROL	hControl;
	UINT8    nCtrl,len,linenum;
	
	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_APPEND_INFO),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			
			EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_APPEND_INFO);

			len =  strlen(getMobileWalletData.appendString);
			linenum = len / MAX_WIDTH_LEN;
			if(len % MAX_WIDTH_LEN > 0 ) linenum ++;
			hControl = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_ENGLISH_LOWER,len,getMobileWalletData.appendString,0);
			EQC_SET_STYLE(hControl,CS_TEXT_EDIT_READONLY);
			EQC_SET_FOCUS(hControl,FALSE);

			EQS_StartFormatter();
			for(nCtrl = 0; nCtrl <2; nCtrl ++)   // �ѻ��� �����ϱ� �켱..
			{
				EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
				EQS_NewLine();           
			}
			EQS_EndFormatter();			
			
			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_APP_RESUME: /* child applet is ended */
			break;

		case SE_IO_TEDIT_DONE : 
		case SE_SOFTKEY_CONFIRM :
			if( (getMobileWalletData.setStringEnc & \
				(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo) ) != INFO_RESET) {
				if( ((getMobileWalletData.setStringEnc & cardInfo) == cardInfo) ||
					((getMobileWalletData.setStringEnc & cardextInfo) == cardextInfo) ) {
					EQS_START_SIBLING(sSKA_PasswdforEncrypt,FALSE);
				} else {
					EQS_START_SIBLING(sSKA_DisplayEncryptData,FALSE);
				}
			} else {
				if( (getMobileWalletData.setStringSign & \
					(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo )) != INFO_RESET ) {
						EQS_START_SIBLING(sSKA_CheckForSign,FALSE);
				} else {
					EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, MMI_NO_ERROR);
				}
			}

			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_PasswdforEncrypt(SKY_EVENT_T *pEvent)
{
    HCONTROL sNumHandle;
	int checklock,IsEnd=0;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        {
			EQS_SetDirectDraw(FALSE);
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_MW_PASSWD_CHECK), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
        
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_FitCanvasSize();
            EQS_SetIOCActive(sNumHandle);

        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(sNumHandle);
		//EQS_End();
        break;

	case EQE_IO_NEDIT_MATCH_LENGTH : 
		EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, currentPIN);

		// ��й�ȣ �Է� �õ� Ƚ��
		checklock = SKY_GET_MW_WALLET_TRIAL_NUM();

		// NV�� ����� PIN���� �о�´�. 
		memcpy((const char *)gHashedPIN, SKY_GET_MW_WALLET_HASHEDPIN(),MW_WALLET_HASHEDPIN_LENGTH);  

		// ����ڰ� �Է��� PIN�� �����ϴ� �κ� 
		if (!wwVerifyPIN(currentPIN)) {
			pinTrialCounter = INIT_TRIAL_NUM;
			SKY_SET_MW_WALLET_TRIAL_NUM(INIT_TRIAL_NUM);
			SKY_SET_MW_WALLET();
			EQS_START_SIBLING(sSKA_DisplayEncryptData,FALSE);
		}
        else {
			pinTrialCounter++; 
			// password �Է� �õ�Ƚ��(3)���� ������� ���� ��� ���� �޴��� �̵�
			if (pinTrialCounter > MAX_PASSWD_TRIAL) { 
				pinTrialCounter = INIT_TRIAL_NUM; checklock ++;
				SKY_SET_MW_WALLET_TRIAL_NUM(checklock); 
				SKY_SET_MW_WALLET();
				// ��й�ȣ 3�� �Է½��а� ���� ���������� �߻��� ���
				if (checklock > MAX_PASSWD_TRIAL) {
					pinTrialCounter = INIT_TRIAL_NUM; checklock = INIT_TRIAL_NUM;
					IsEnd = TRUE;
					// �������� ���� �ʱ�ȭ
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INITIALIZE_INFORM_MSG), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					Initialize_MW();
				} else {
					EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL);
				}
			}
			else {  // �н����� ���Է��� �䱸 "��й�ȣ�� Ʋ���ϴ�."
				IsEnd = FALSE;
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}
		} 

		break;

    case SE_IO_MESGBOX_END:
		if (IsEnd ) {
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL); 
		}
		else {
			EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
			EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
		}
        break; 

    case SE_IO_NEDIT_NODIGIT: 
        //EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
		EQS_SetDirectDraw(TRUE);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSKA_DisplayEncryptData(SKY_EVENT_T *pEvent)
{
	int nCtrl,i,len;
	BYTE *pToken1[3];
	static BYTE IOCtrlOrder[30];
	static int order,nTemp;
	static BOOL	IsCardOnly = FALSE,user,card,ocb,all;
	static int cardnum,deliverynum;
	static int tmpcardinputstatus[3] = { 0, 0, 0 } , tmpdeliveryinputstatus[2] = { 0, 0 };
	HCONTROL hControl1,hControl2,hControl3,hControl4,hControl5, hControl6,hControl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX, 800, //SKY_DEF_WIN_DY,
						  CL_CANCEL_CONFIRM_NEXT,   
						  NULL, //SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);

			IsCardOnly = FALSE; user = FALSE; card = FALSE; ocb = FALSE; order = 0;
			all = TRUE;
			// ���� ����
			if((getMobileWalletData.setStringEnc & personInfo) == personInfo) {
				MW_init_user(); // ����� ���� �ʱ�ȭ
				WhichOne = MOBILE_WALLET_USER;	
				user = TRUE;  updateInfo[0] = 1;
				IOCtrlOrder[order++] = ICT_U_TITLE_E;
				IOCtrlOrder[order++] = ICT_U_TITLE_E;
				IOCtrlOrder[order++] = ICT_U_NAME_E;
				IOCtrlOrder[order++] = ICT_U_TITLE_E;
				IOCtrlOrder[order++] = ICT_U_PHONE_E;
//				IOCtrlOrder[order++] = ICT_U_EMAIL_E;
//				IOCtrlOrder[order++] = ICT_U_EMAIL_E;

				EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_WAP_USER_INFO);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_U_NAME,SKY_GET_TOKEN(TKN_MW_USER_INFO_NAME));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_USER_INFO_NAME);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelName);
				hControl1 = EQC_TextEdit(EQS_NA,EQS_NA, MAX_WIDTH_LEN, 1, \
										TEM_KOREAN, USER_INFO_NAME,(BYTE*)"", 0);
				//EQC_SET_FOCUS(hControl1,FALSE);  EQC_SET_ENABLE(hControl1, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_U_PHONE,SKY_GET_TOKEN(TKN_MW_USER_INFO_PHONE));
				EQC_SetStaticText(hControl, texttmp);
			    //EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_USER_INFO_PHONE);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelPhoneNumber);
				hControl2 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  USER_INFO_PHONE, (BYTE*)"" );
				strcpy(&userinfo.userPhone,sGetMyNumber());
				EQC_SetNumberEditValue(hControl2,&userinfo.userPhone);           // ��ȭ��ȣ�� disable ���·� ��� 
				EQC_SET_ENABLE(hControl2, FALSE);                           // ska_call.c�� GetMyNumber�Լ� ���. 

/*			  // ���õ�  Wallet�ʵ��� �⺻���� ǥ���Ѵ�. ����, �⺻���� �ƴ� e-mail�ʵ�� �����Ѵ�.
				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_USER_INFO_EMAIL);
				EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelEMail);
				hControl3 = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, 2, \
										TEM_ENGLISH_LOWER, USER_INFO_EMAIL,(BYTE*)"", 0);
*/
				if ( SKY_GET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_USER) == 1) {
					MW_read_user();
					MW_add_input_status(MOBILE_WALLET_USER, USER_FIELD);
					EQC_SetTextEditData(hControl1,&userinfo.userName);
					//EQC_SetNumberEditValue(hControl2,&userinfo.userPhone);
					//EQC_SetTextEditData(hControl3,&userinfo.userEmail);
				} else {
					userInputStatus = 0x00;
				}  
			} 

		// ī�� ����
			if((getMobileWalletData.setStringEnc & cardInfo) == cardInfo) { 
				card = TRUE; updateInfo[1] = 1;
				IOCtrlOrder[order++] = ICT_C_TITLE_E;
				IOCtrlOrder[order++] = ICT_C_TITLE_E;
				IOCtrlOrder[order++] = ICT_C_NUMBER_E;
				IOCtrlOrder[order++] = ICT_C_TITLE_E;
				IOCtrlOrder[order++] = ICT_C_PWD_E;
				IOCtrlOrder[order++] = ICT_C_TITLE_E;
				IOCtrlOrder[order++] = ICT_C_VALID_E;
				IOCtrlOrder[order++] = ICT_C_TITLE_E;
				IOCtrlOrder[order++] = ICT_C_RNO_E;

				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_WAP_CARD_INFO);
				pToken1[0] = SKY_GET_TOKEN(TKN_MW_CARD1_INFO);
				pToken1[1] = SKY_GET_TOKEN(TKN_MW_CARD2_INFO);
				pToken1[2] = SKY_GET_TOKEN(TKN_MW_CARD3_INFO);
				hControl5 = EQC_ScrollList(EQS_NA, EQS_NA, MAX_WIDTH_LEN, 1, pToken1, 3,3,0);
				EQC_SET_STYLE(hControl5, EQC_GET_STYLE(hControl5) |\
							            CS_SCROLL_LIST_COMBO );
//				EQC_SetShowScrollBar( hControl5, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_NUM,SKY_GET_TOKEN(TKN_MW_CARD_INFO_NUM));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_CARD_INFO_NUM);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCreditCardNum);
				hControl1 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 2, \
										  CARD_INFO_NUMBER, (BYTE*)"" );
				EQC_SET_STYLE(hControl1, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_PASSWD,SKY_GET_TOKEN(TKN_MW_CARD_INFO_PWD));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_CARD_INFO_PWD);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelPassWord);
				hControl2 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  CARD_INFO_PWD, (BYTE*)"" );
				EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE | CS_NUMBER_EDIT_PASSWD);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_VALID,SKY_GET_TOKEN(TKN_MW_CARD_INFO_VALID));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_CARD_INFO_VALID);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelTermOfValidity);
				hControl3 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  CARD_INFO_VALID, (BYTE*)"" );
				EQC_SET_STYLE(hControl3, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_RNO,SKY_GET_TOKEN(TKN_MW_CARD_INFO_RNO));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_CARD_INFO_RNO);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelResidentRegistrationNumber);
				hControl4 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  CARD_INFO_RNO, (BYTE*)"" );
				EQC_SET_STYLE(hControl4, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);

				// �켱 ��� ī�� �����͸� �ҷ��鿩�� temp������ ������ ���´�.
				// cardinfo�� �����ϴ� ���� ���� ȭ�鿡 ������ ���̰�.. ������ ������ temp������
				// �����ϰ� �ִٰ� �ε����� �ٲ𶧸��� �����ϰ� cardinfo�� �������ָ� �ȴ�.

				WhichOne = MOBILE_WALLET_CARD2;
				if ( SKY_GET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_CARD2) == 1) {
					MW_init_card();	
					MW_read_card();
					tmpcardinputstatus[1] = CARD_FIELD;
					strcpy(tmpcardinfo[1].cardNO,&cardinfo.cardNO);
					strcpy(tmpcardinfo[1].cardPwd,&cardinfo.cardPwd);
					strcpy(tmpcardinfo[1].cardValid,&cardinfo.cardValid);
					strcpy(tmpcardinfo[1].cardRno,&cardinfo.cardRno);
				} 
				WhichOne = MOBILE_WALLET_CARD3;
				if ( SKY_GET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_CARD3) == 1) {
					MW_init_card();
					MW_read_card();
					tmpcardinputstatus[2] = CARD_FIELD;
					strcpy(tmpcardinfo[2].cardNO,&cardinfo.cardNO);
					strcpy(tmpcardinfo[2].cardPwd,&cardinfo.cardPwd);
					strcpy(tmpcardinfo[2].cardValid,&cardinfo.cardValid);
					strcpy(tmpcardinfo[2].cardRno,&cardinfo.cardRno);
				}

				WhichOne = MOBILE_WALLET_CARD1; cardnum = 0;
				if ( SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
					MW_init_card();
					MW_read_card();
					tmpcardinputstatus[0] = CARD_FIELD;
					//MW_add_input_status(MOBILE_WALLET_CARD1, CARD_FIELD);
					
					EQC_SetNumberEditValue(hControl1,&cardinfo.cardNO);
					EQC_SetNumberEditValue(hControl2,&cardinfo.cardPwd);
					EQC_SetNumberEditValue(hControl3,&cardinfo.cardValid);
					EQC_SetNumberEditValue(hControl4,&cardinfo.cardRno);

					strcpy(tmpcardinfo[0].cardNO,&cardinfo.cardNO);
					strcpy(tmpcardinfo[0].cardPwd,&cardinfo.cardPwd);
					strcpy(tmpcardinfo[0].cardValid,&cardinfo.cardValid);
					strcpy(tmpcardinfo[0].cardRno,&cardinfo.cardRno);
				}
				else {
					cardInputStatus = 0x00;
				}
			} 

			// OCB ����
			if((getMobileWalletData.setStringEnc & cardextInfo) == cardextInfo) { 
				MW_init_ocb();
				WhichOne =  MOBILE_WALLET_OCB; 
				ocb = TRUE;  updateInfo[2] = 1;
				IOCtrlOrder[order++] = ICT_P_TITLE_E;
				IOCtrlOrder[order++] = ICT_P_TITLE_E;
				IOCtrlOrder[order++] = ICT_P_NUMBER_E;
				IOCtrlOrder[order++] = ICT_P_TITLE_E;
				IOCtrlOrder[order++] = ICT_P_PWD_E;

				EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_WAP_OCB_INFO);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_P_NUM,SKY_GET_TOKEN(TKN_MW_OCB_INFO_NUM));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_OCB_INFO_NUM);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelOKCardNumber);
				hControl1 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  POINT_INFO_NUMBER, (BYTE*)"" );
				EQC_SET_STYLE(hControl1, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_PASSWD,SKY_GET_TOKEN(TKN_MW_CARD_INFO_PWD));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_OCB_INFO_PWD);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelOKCashbagPassWord);
				hControl2 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  POINT_INFO_PWD, (BYTE*)"" );
				EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE | CS_NUMBER_EDIT_PASSWD);

				if ( SKY_GET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_OCB) == 1) {
					MW_read_ocb();
					MW_add_input_status(MOBILE_WALLET_OCB, POINT_FIELD);					

					EQC_SetNumberEditValue(hControl1,&ocbinfo.pointNo);
					EQC_SetNumberEditValue(hControl2,&ocbinfo.pointPwd);
				} else  {
					ocbInputStatus = 0x00;
				}
			} 

			// ����� ����
			if((getMobileWalletData.setStringEnc & deliveryInfo) == deliveryInfo) { 
				updateInfo[3] = 1;
				IOCtrlOrder[order++] = ICT_D_TITLE_E;
				IOCtrlOrder[order++] = ICT_D_TITLE_E;
				IOCtrlOrder[order++] = ICT_D_ZIPCODE_E;
				IOCtrlOrder[order++] = ICT_D_TITLE_E;
				IOCtrlOrder[order++] = ICT_D_ADDRESS_E;
				IOCtrlOrder[order++] = ICT_D_TITLE_E;
				IOCtrlOrder[order++] = ICT_D_NAME_E;
				IOCtrlOrder[order++] = ICT_D_TITLE_E;
				IOCtrlOrder[order++] = ICT_D_PHONE_E;
				
//				EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_WAP_DELIVERY_INFO);
				pToken1[0] = SKY_GET_TOKEN(TKN_MW_DELIVERY1_INFO);
				pToken1[1] = SKY_GET_TOKEN(TKN_MW_DELIVERY2_INFO);
				hControl6 = EQC_ScrollList(EQS_NA, EQS_NA, MAX_WIDTH_LEN, 1, pToken1, 2,2,0);
				EQC_SET_STYLE(hControl6, EQC_GET_STYLE(hControl6) |\
							            CS_SCROLL_LIST_COMBO );

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_D_ZIPCODE,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_ZIP));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_DELIVERY_INFO_ZIP);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelZipCode);
				hControl1 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  DELIVERY_INFO_ZIP, (BYTE*)"" );
				EQC_SET_STYLE(hControl1, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_D_ADDR,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_ADDR));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_DELIVERY_INFO_ADDR);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelAddress);
				hControl2 = EQC_TextEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 4, \
										TEM_KOREAN, DELIVERY_INFO_ADDR,(BYTE*)"", 0);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_D_RNAME,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_NAME));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_DELIVERY_INFO_NAME);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelReceiverName);
				hControl3 = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, 1, \
										TEM_KOREAN, DELIVERY_INFO_NAME,(BYTE*)"", 0);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_D_RPHONE,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_PHONE));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_DELIVERY_INFO_PHONE);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelReceiverPhone);
				hControl4 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  DELIVERY_INFO_PHONE+2, (BYTE*)"" );

				// ī�������� ���������� ó���� temp�������� ��� �ҷ����δ�.
				WhichOne = MOBILE_WALLET_DELIVERY2;
				if(SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
					MW_init_delivery();
					MW_read_delivery();
					tmpdeliveryinputstatus[1] = DELIVERY_FIELD;

					strcpy(tmpdeliveryinfo[1].dvZip,&deliveryinfo.dvZip);
					strcpy(tmpdeliveryinfo[1].dvAddr,&deliveryinfo.dvAddr);
					strcpy(tmpdeliveryinfo[1].dvName,&deliveryinfo.dvName);
					strcpy(tmpdeliveryinfo[1].dvPhone,&deliveryinfo.dvPhone);
				}

				WhichOne = MOBILE_WALLET_DELIVERY1; deliverynum = 0;
				if (SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
					MW_init_delivery();
					MW_read_delivery();
					tmpdeliveryinputstatus[0] = DELIVERY_FIELD;
					//MW_add_input_status(MOBILE_WALLET_DELIVERY1, DELIVERY_FIELD);
					
					EQC_SetNumberEditValue(hControl1,&deliveryinfo.dvZip);
					EQC_SetTextEditData(hControl2,&deliveryinfo.dvAddr);
					EQC_SetTextEditData(hControl3,&deliveryinfo.dvName);
					EQC_SetNumberEditValue(hControl4,&deliveryinfo.dvPhone);

					strcpy(tmpdeliveryinfo[0].dvZip,&deliveryinfo.dvZip);
					strcpy(tmpdeliveryinfo[0].dvAddr,&deliveryinfo.dvAddr);
					strcpy(tmpdeliveryinfo[0].dvName,&deliveryinfo.dvName);
					strcpy(tmpdeliveryinfo[0].dvPhone,&deliveryinfo.dvPhone);
				} else {
					deliveryInputStatus = 0x00;
				}
			}

/*			if( ((getMobileWalletData.setStringEnc & cardInfo) == cardInfo) && \
				((getMobileWalletData.setStringEnc & deliveryInfo) != deliveryInfo) ) {
				IsCardOnly = TRUE;
				pToken[0] = SKY_GET_TOKEN(TKN_MW_CARD_CHANGE);
				hControl1 = EQC_ScrollList(EQS_NA,EQS_NA,14,1,pToken,1,1,0);
				EQC_SET_STYLE( hControl1, CS_SCROLL_LIST_ENUMBERATED);
			}
			if(((getMobileWalletData.setStringEnc & cardInfo) != cardInfo) && \
				((getMobileWalletData.setStringEnc & deliveryInfo) == deliveryInfo) ) {
				pToken[0] = SKY_GET_TOKEN(TKN_MW_DELIVERY_CHANGE);
				hControl1 = EQC_ScrollList(EQS_NA,EQS_NA,14,1,pToken,1,1,0);
				EQC_SET_STYLE( hControl1, CS_SCROLL_LIST_ENUMBERATED);
			}
			if(((getMobileWalletData.setStringEnc & cardInfo) == cardInfo) && \
				((getMobileWalletData.setStringEnc & deliveryInfo) == deliveryInfo) ) {
				IsCardOnly = TRUE;
				pToken[0] = SKY_GET_TOKEN(TKN_MW_CARD_CHANGE);    
				pToken[1] = SKY_GET_TOKEN(TKN_MW_DELIVERY_CHANGE);
				hControl1 = EQC_ScrollList(EQS_NA,EQS_NA,14,2,pToken,2,2,0);
				EQC_SET_STYLE( hControl1, CS_SCROLL_LIST_ENUMBERATED);
			}  */

			EQS_StartFormatter();
			
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}
			EQS_EndFormatter();			
			EQS_FitCanvasSize();

			break;

		case SE_IO_SCROLL_LIST_SELECT :
		case SE_IO_SCROLL_LIST_CHG :
//		case SE_IO_SCROLL_LIST_LEFT :
//		case SE_IO_SCROLL_LIST_RIGHT :
			for(i = 0 ; i < order ; i++) {
				if(pEvent->wParam == EQS_GetNthIOC(i)) break;
			}

			if (IOCtrlOrder[i] == ICT_C_TITLE_E) {
				nTemp = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(i));
				switch(nTemp) {
				case 0 : 
					WhichOne = MOBILE_WALLET_CARD1;
					cardnum = 0;
					break;
				case 1 : 
					WhichOne = MOBILE_WALLET_CARD2;
					cardnum = 1;
					break;
				case 2 : 
					WhichOne = MOBILE_WALLET_CARD3;
					cardnum = 2;
					break;
				}

				//MW_init_card();
				//if ( SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
					//MW_read_card();
					//MW_add_input_status(MOBILE_WALLET_CARD1, CARD_FIELD);
				//}
				//else {
				//	cardInputStatus = 0x00;
				//}

				EQC_SetNumberEditValue(EQS_GetNthIOC(i+2),&tmpcardinfo[cardnum].cardNO);
				EQC_SetNumberEditValue(EQS_GetNthIOC(i+4),&tmpcardinfo[cardnum].cardPwd);
				EQC_SetNumberEditValue(EQS_GetNthIOC(i+6),&tmpcardinfo[cardnum].cardValid);
				EQC_SetNumberEditValue(EQS_GetNthIOC(i+8),&tmpcardinfo[cardnum].cardRno);

			} else if (IOCtrlOrder[i] == ICT_D_TITLE_E) {
				nTemp = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(i));
				switch(nTemp) {
				case 0 : 
					WhichOne = MOBILE_WALLET_DELIVERY1;
					deliverynum = 0;
					break;
				case 1 : 
					WhichOne = MOBILE_WALLET_DELIVERY2;
					deliverynum = 1;
					break;
				}

				//MW_init_delivery();
				//if (SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
					//MW_read_delivery();
				//	MW_add_input_status(MOBILE_WALLET_DELIVERY1, DELIVERY_FIELD);
				//} else {
				//	deliveryInputStatus = 0x00;
				//}
				EQC_SetNumberEditValue(EQS_GetNthIOC(i+2),&tmpdeliveryinfo[deliverynum].dvZip);
				EQC_SetTextEditData(EQS_GetNthIOC(i+4),&tmpdeliveryinfo[deliverynum].dvAddr);
				EQC_SetTextEditData(EQS_GetNthIOC(i+6),&tmpdeliveryinfo[deliverynum].dvName);
				EQC_SetNumberEditValue(EQS_GetNthIOC(i+8),&tmpdeliveryinfo[deliverynum].dvPhone);

			} else {
				ERR_FATAL("Wrong parameter",0,0,0);
			}

			EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);

			break;

		case SE_APP_DRAW :
			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_APP_RESUME: 
			break;

		case SE_SOFTKEY_CONFIRM :
			break;

		case SE_IO_TEDIT_DONE :
			for(i = 0;i < order;i++) {
				if(pEvent->wParam == EQS_GetNthIOC(i)) break;
			}

			if (IOCtrlOrder[i] == ICT_U_NAME_E) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(i),userinfo.userName);
				if (len) { 
					userInputStatus |= USER_NAME; 
				} else {
					userInputStatus &= (~USER_NAME);
				}
				len = 0;
			} /* else if (IOCtrlOrder[i] == ICT_U_EMAIL_E) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(i),userinfo.userEmail);
				if (len) { 
					userInputStatus |= USER_EMAIL; 
				} else { 
					userInputStatus &= (~USER_EMAIL); 
				}
				len = 0;
			} */else if (IOCtrlOrder[i] == ICT_D_ADDRESS_E) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(i),tmpdeliveryinfo[deliverynum].dvAddr);
				if (len) { 
					tmpdeliveryinputstatus[deliverynum] |= DELIVERY_ADDRESS; 
				} else {
					tmpdeliveryinputstatus[deliverynum] &= (~DELIVERY_ADDRESS);
				}
				len = 0;
			} else if (IOCtrlOrder[i] == ICT_D_NAME_E) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(i),tmpdeliveryinfo[deliverynum].dvName);
				if (len) { 
					tmpdeliveryinputstatus[deliverynum] |= DELIVERY_NAME; 
				} else { 
					tmpdeliveryinputstatus[deliverynum] &= (~DELIVERY_NAME); 
				}
				len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}
			break;

		case SE_IO_NEDIT_DONE :
			for(i = 0; i < order ; i++) {
				if(pEvent->wParam == EQS_GetNthIOC(i)) break;
			}
/*			if (IOCtrlOrder[i] == ICT_U_PHONE_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),userinfo.userPhone);
				if (len) {
					userInputStatus |= USER_PHONE;
				} else {
					userInputStatus &= (~USER_PHONE);
				}
				len = 0;
			} else*/ if (IOCtrlOrder[i] == ICT_D_ZIPCODE_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),tmpdeliveryinfo[deliverynum].dvZip);
				if (len == DELIVERY_INFO_ZIP ) {
					tmpdeliveryinputstatus[deliverynum] |= DELIVERY_ZIPCODE;
				} else {
					tmpdeliveryinputstatus[deliverynum] &= (~DELIVERY_ZIPCODE);
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
				len = 0;
			} else if (IOCtrlOrder[i] == ICT_D_PHONE_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),tmpdeliveryinfo[deliverynum].dvPhone);
				if (len >= 5) {
					tmpdeliveryinputstatus[deliverynum] |= DELIVERY_PHONE;
				} else {
					tmpdeliveryinputstatus[deliverynum] &= (~DELIVERY_PHONE);
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
				len = 0;

			} else if (IOCtrlOrder[i] == ICT_C_NUMBER_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),tmpcardinfo[cardnum].cardNO);
				if (len) {
					tmpcardinputstatus[cardnum] |= CARD_NUMBER;
				} else {
					tmpcardinputstatus[cardnum] &= (~CARD_NUMBER);
				}
				len = 0;
			} else if (IOCtrlOrder[i] == ICT_C_PWD_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),tmpcardinfo[cardnum].cardPwd);
				if (len == CARD_INFO_PWD) {
					tmpcardinputstatus[cardnum] |= CARD_PASSWD;
				} else {
					tmpcardinputstatus[cardnum] &= (~CARD_PASSWD);
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
												MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
				len = 0;
			} else if (IOCtrlOrder[i] == ICT_C_VALID_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),tmpcardinfo[cardnum].cardValid);
				if (len == CARD_INFO_VALID) {
					// ��ȿ�Ⱓ�� ����� �ԷµǾ����� üũ..MMYY (MM : 01 ~ 12)
					if (tmpcardinfo[cardnum].cardValid[0] == '0') {
						if(tmpcardinfo[cardnum].cardValid[1] != '0') {
							tmpcardinputstatus[cardnum] |= CARD_VALID;
						} else {
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_WRONG_VALID), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
							memset(tmpcardinfo[cardnum].cardValid,NULL,5);
							EQC_SetNumberEditValue(EQS_GetNthIOC(i),tmpcardinfo[cardnum].cardValid);
						}
					}
					else if (tmpcardinfo[cardnum].cardValid[0] == '1') {
						if (tmpcardinfo[cardnum].cardValid[1] >= '0' && tmpcardinfo[cardnum].cardValid[1] <= '2') {
							tmpcardinputstatus[cardnum] |= CARD_VALID;
						} else {
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_WRONG_VALID), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
							memset(tmpcardinfo[cardnum].cardValid,NULL,5);
							EQC_SetNumberEditValue(EQS_GetNthIOC(i),tmpcardinfo[cardnum].cardValid);
							tmpcardinputstatus[cardnum] &= (~CARD_VALID);
						}
					}
					else {
						EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_WRONG_VALID), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						memset(tmpcardinfo[cardnum].cardValid,NULL,5);
						EQC_SetNumberEditValue(EQS_GetNthIOC(i),tmpcardinfo[cardnum].cardValid);
						tmpcardinputstatus[cardnum] &= (~CARD_VALID);
					}

				} else {
					tmpcardinputstatus[cardnum] &= (~CARD_VALID);
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
						
				len = 0;
			} else if (IOCtrlOrder[i] == ICT_C_RNO_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),tmpcardinfo[cardnum].cardRno);
				if (len) {
					tmpcardinputstatus[cardnum] |= CARD_RNO;
				} else {
					tmpcardinputstatus[cardnum] &= (~CARD_RNO);
				}
				len = 0;
			} else if (IOCtrlOrder[i] == ICT_P_NUMBER_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),ocbinfo.pointNo);
				if (len) {
					ocbInputStatus |= POINT_NUM;
				} else {
					ocbInputStatus &= (~POINT_NUM);
				}
				len = 0;
			} else if (IOCtrlOrder[i] == ICT_P_PWD_E) {
				len = EQC_GetNumberEditValue(EQS_GetNthIOC(i),ocbinfo.pointPwd);
				if (len == POINT_INFO_PWD) {
					ocbInputStatus |= POINT_PWD;
				} else {
					ocbInputStatus &= (~POINT_PWD);
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_MORE), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
				len = 0;
			} else {
					MSG_HIGH("���⼭ ����Ʈ�� ������ �ȵ���....",0,0,0);
			}			
			break;


		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL); 
			break;

		case SE_SOFTKEY_NEXT :
// �� �ʵ尡 ��� ä���������� Ȯ���� �ʿ�.... 
/*
			all = TRUE;
			for(i = 0; i < order ; i++) {
//				if(EQC_GET_DIRTY(EQS_GetNthIOC(i) == TRUE) {
					if (IOCtrlOrder[i] == ICT_U_NAME_E) {
						len = strlen(userinfo.userName);
						if(len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_NAME), \
											MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						}
					} 
					else if (IOCtrlOrder[i] == ICT_D_ADDRESS_E) {
						len = strlen(deliveryinfo.dvAddr);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ADDR), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						} 
					}
					else if (IOCtrlOrder[i] == ICT_D_NAME_E) {
						len = strlen(deliveryinfo.dvName);
						if (len <= 0) { 
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_R_NAME), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						} 
					}

					else if (IOCtrlOrder[i] == ICT_D_ZIPCODE_E) {
						len = strlen(deliveryinfo.dvZip);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ZIPCODE), \
										MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						} 
					}
					else if (IOCtrlOrder[i] == ICT_D_PHONE_E) {
						len = strlen(deliveryinfo.dvPhone);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PHONE), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						} 
					} 
					else if (IOCtrlOrder[i] == ICT_C_NUMBER_E) {
						len = strlen(cardinfo.cardNO);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_CARD), \
										MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						}
					}
					else if (IOCtrlOrder[i] == ICT_C_PWD_E) {
						len = strlen(cardinfo.cardPwd);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PWD), \
										MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						}
					} 
					else if (IOCtrlOrder[i] == ICT_C_VALID_E) {
						len = strlen(cardinfo.cardValid);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_VALID), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						} 
					} 
					else if (IOCtrlOrder[i] == ICT_C_RNO_E) {
						len = strlen(cardinfo.cardRno);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_RNO), \
									MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						}
					} 
					else if (IOCtrlOrder[i] == ICT_P_NUMBER_E) {
						len = strlen(ocbinfo.pointNo);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_CARDNUM), \
										MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						}
					}
					else if (IOCtrlOrder[i] == ICT_P_PWD_E) {
						len = strlen(ocbinfo.pointPwd);
						if (len <= 0) {
							all = FALSE;
							EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PWD), \
										MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
						} 
					}
//				} else {

//				}
			}

*/
//			if (EQC_GET_DIRTY(EQS_GetFocusedIOC()) == TRUE) {
				
//			}

			all = TRUE;
			if((getMobileWalletData.setStringEnc & personInfo) == personInfo) {
				if(!(userInputStatus & USER_NAME )) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_NAME), \
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				} 
			} 
			if ((getMobileWalletData.setStringEnc & cardInfo ) == cardInfo) {
				if(!(tmpcardinputstatus[cardnum] & CARD_NUMBER )) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_CARD), \
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				} 
				if(!(tmpcardinputstatus[cardnum] & CARD_PASSWD)) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PWD), \
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				} 
				if(!(tmpcardinputstatus[cardnum] & CARD_VALID)) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_VALID), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				} 
				if(!(tmpcardinputstatus[cardnum] & CARD_RNO)) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_RNO), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
			} 
			if ((getMobileWalletData.setStringEnc & cardextInfo) == cardextInfo) {
				if(!(ocbInputStatus & POINT_NUM )) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_CARDNUM), \
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				} 
				if(!(ocbInputStatus & POINT_PWD )) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PWD), \
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
			} 
			if ((getMobileWalletData.setStringEnc & deliveryInfo) == deliveryInfo) {
				if(!(tmpdeliveryinputstatus[deliverynum] & DELIVERY_ZIPCODE)) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ZIPCODE), \
								MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				} 
				if(!(tmpdeliveryinputstatus[deliverynum] & DELIVERY_ADDRESS)) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ADDR), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				} 
				if(!(tmpdeliveryinputstatus[deliverynum] & DELIVERY_NAME)) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_R_NAME), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				} 
				if(!(tmpdeliveryinputstatus[deliverynum] & DELIVERY_PHONE)) {
					all = FALSE;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_PHONE), \
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
				}
			} 
			// ��� �ʵ尡 �� ä���� ������..... 
			if(all) {
				if( (getMobileWalletData.setStringSign & \
					(personInfo | deliveryInfo | cardInfo | accountInfo | cardextInfo )) != INFO_RESET ) {  
					updateInfo[1] = cardnum;
					updateInfo[3] = deliverynum;
					EQS_START_SIBLING(sSKA_CheckForSign,FALSE);
				} else {
					EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, MMI_NO_ERROR);
				}
			}

			break;

		case SE_SOFTKEY_CANCEL :
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL);
			break;

		default : 
			break;
	}
}

LOCAL void SKY_API sSKA_CheckForSign(SKY_EVENT_T *pEvent)
{
	int len,linenum,nCtrl;
	static int cardnum,deliverynum;
	HCONTROL hControl1,hControl2,hControl3,hControl4,hControl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,800,//SKY_DEF_WIN_DY,
						  CL_CANCEL_SIGN_NONE,
						  NULL, //SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);

			if(strlen(getMobileWalletData.appendString) > 0) {
				EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_APPEND_INFO);

				len =  strlen(getMobileWalletData.appendString);
				linenum = len / MAX_WIDTH_LEN;
				if(len % MAX_WIDTH_LEN > 0 ) linenum ++;
				hControl1 = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_ENGLISH_LOWER,len,getMobileWalletData.appendString,0);
				EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
				EQC_SET_FOCUS(hControl1,FALSE);
			}

			if((getMobileWalletData.setStringSign & personInfo) == personInfo) {
				WhichOne = MOBILE_WALLET_USER;
				if(updateInfo[0] != 1) {
					MW_init_user(); // ����� ���� �ʱ�ȭ
					if ( SKY_GET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_USER) == 1) {
						MW_read_user();
					} 
				}  
				
				EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_WAP_USER_INFO);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_U_NAME,SKY_GET_TOKEN(TKN_MW_USER_INFO_NAME));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_USER_INFO_NAME);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelName);
				hControl1 = EQC_TextEdit(EQS_NA,EQS_NA, MAX_WIDTH_LEN, 1, \
										TEM_KOREAN, USER_INFO_NAME,(BYTE*)"", 0);
				EQC_SET_FOCUS(hControl1,FALSE);  EQC_SET_ENABLE(hControl1, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_U_PHONE,SKY_GET_TOKEN(TKN_MW_USER_INFO_PHONE));
				EQC_SetStaticText(hControl, texttmp);
			    //EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_USER_INFO_PHONE);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelPhoneNumber);
				hControl2 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  USER_INFO_PHONE, (BYTE*)"" );
				//strcpy(&userinfo.userPhone,sGetMyNumber());
				EQC_SetNumberEditValue(hControl2, &userinfo.userPhone);           // ��ȭ��ȣ�� disable ���·� ��� 
				EQC_SET_FOCUS(hControl2,FALSE);  EQC_SET_ENABLE(hControl2, FALSE);

/*				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_USER_INFO_EMAIL);
				EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelEMail);
				hControl3 = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, 2, \
										TEM_ENGLISH_LOWER, USER_INFO_EMAIL,(BYTE*)"", 0);
				EQC_SET_FOCUS(hControl3,FALSE);  EQC_SET_ENABLE(hControl3, FALSE);
*/
				EQC_SetTextEditData(hControl1,&userinfo.userName);
				//EQC_SetNumberEditValue(hControl2,&userinfo.userPhone);
//				EQC_SetTextEditData(hControl3,&userinfo.userEmail);
			} 

			if((getMobileWalletData.setStringSign & cardInfo) == cardInfo) { 
				//WhichOne = MOBILE_WALLET_CARD1;
				//if( updateInfo[1] != 1) {
				//	MW_init_card();
				//	if ( SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
				//		MW_read_card();
				//	}
				//}
				cardnum = updateInfo[1];

				EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_WAP_CARD_INFO);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_NUM,SKY_GET_TOKEN(TKN_MW_CARD_INFO_NUM));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_CARD_INFO_NUM);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelCreditCardNum);
				len = strlen(tmpcardinfo[cardnum].cardNO); 
				linenum = len / MAX_WIDTH_LEN; 
				if(len % MAX_WIDTH_LEN > 0 ) linenum++;
				hControl1 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, linenum, \
										  CARD_INFO_NUMBER, (BYTE*)"" );
				EQC_SET_STYLE(hControl1, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
				EQC_SET_FOCUS(hControl1,FALSE);  EQC_SET_ENABLE(hControl1, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_PASSWD,SKY_GET_TOKEN(TKN_MW_CARD_INFO_PWD));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_CARD_INFO_PWD);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelPassWord);
				hControl2 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  CARD_INFO_PWD, (BYTE*)"" );
				EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE | CS_NUMBER_EDIT_PASSWD);
				EQC_SET_FOCUS(hControl2,FALSE);  EQC_SET_ENABLE(hControl2, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_VALID,SKY_GET_TOKEN(TKN_MW_CARD_INFO_VALID));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_CARD_INFO_VALID);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelTermOfValidity);
				hControl3 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  CARD_INFO_VALID, (BYTE*)"" );
				EQC_SET_STYLE(hControl3, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
				EQC_SET_FOCUS(hControl3,FALSE);  EQC_SET_ENABLE(hControl3, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_RNO,SKY_GET_TOKEN(TKN_MW_CARD_INFO_RNO));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_CARD_INFO_RNO);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelResidentRegistrationNumber);
				hControl4 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  CARD_INFO_RNO, (BYTE*)"" );
				EQC_SET_STYLE(hControl4, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
				EQC_SET_FOCUS(hControl4,FALSE);  EQC_SET_ENABLE(hControl4, FALSE);

				EQC_SetNumberEditValue(hControl1,&tmpcardinfo[cardnum].cardNO);
				EQC_SetNumberEditValue(hControl2,&tmpcardinfo[cardnum].cardPwd);
				EQC_SetNumberEditValue(hControl3,&tmpcardinfo[cardnum].cardValid);
				EQC_SetNumberEditValue(hControl4,&tmpcardinfo[cardnum].cardRno);

				strcpy(cardinfo.cardNO,tmpcardinfo[cardnum].cardNO);
				strcpy(cardinfo.cardPwd,tmpcardinfo[cardnum].cardPwd);
				strcpy(cardinfo.cardValid,tmpcardinfo[cardnum].cardValid);
				strcpy(cardinfo.cardRno,tmpcardinfo[cardnum].cardRno);
			} 

			if((getMobileWalletData.setStringSign & cardextInfo) == cardextInfo) { 
				WhichOne = MOBILE_WALLET_OCB;
				if(updateInfo[2] != 1) {
					MW_init_ocb();
					if ( SKY_GET_MW_WALLET_DATA_SAVED(MOBILE_WALLET_OCB) == 1) {
						MW_read_ocb();
					}
				}

				EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_WAP_OCB_INFO);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_P_NUM,SKY_GET_TOKEN(TKN_MW_OCB_INFO_NUM));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_OCB_INFO_NUM);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelOKCardNumber);
				hControl1 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  POINT_INFO_NUMBER, (BYTE*)"" );
				EQC_SET_STYLE(hControl1, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
				EQC_SET_FOCUS(hControl1,FALSE);  EQC_SET_ENABLE(hControl1, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_C_PASSWD,SKY_GET_TOKEN(TKN_MW_CARD_INFO_PWD));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_OCB_INFO_PWD);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelOKCashbagPassWord);
				hControl2 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  POINT_INFO_PWD, (BYTE*)"" );
				EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE | CS_NUMBER_EDIT_PASSWD);
				EQC_SET_FOCUS(hControl2,FALSE);  EQC_SET_ENABLE(hControl2, FALSE);

				EQC_SetNumberEditValue(hControl1,&ocbinfo.pointNo);
				EQC_SetNumberEditValue(hControl2,&ocbinfo.pointPwd);
			} 

			if((getMobileWalletData.setStringSign & deliveryInfo) == deliveryInfo) { 
				//WhichOne = MOBILE_WALLET_DELIVERY1;
				//if(updateInfo[3] != 1) {
				//	MW_init_delivery();
				//	if (SKY_GET_MW_WALLET_DATA_SAVED(WhichOne) == 1) {
				//		MW_read_delivery();
				//	}
				//}
				deliverynum = updateInfo[3];

				EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_WAP_DELIVERY_INFO);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_D_ZIPCODE,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_ZIP));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_DELIVERY_INFO_ZIP);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelZipCode);
				hControl1 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  DELIVERY_INFO_ZIP, (BYTE*)"" );
				EQC_SET_STYLE(hControl1, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
								CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
				EQC_SET_FOCUS(hControl1,FALSE);  EQC_SET_ENABLE(hControl1, FALSE);
              
				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_D_ADDR,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_ADDR));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_DELIVERY_INFO_ADDR);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelAddress);
				len = strlen(tmpdeliveryinfo[deliverynum].dvAddr);
				linenum = len / MAX_WIDTH_LEN;
				if (len % MAX_WIDTH_LEN > 0) linenum ++;
				hControl2 = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, \
										TEM_KOREAN, DELIVERY_INFO_ADDR,(BYTE*)"", 0);
				EQC_SET_FOCUS(hControl2,FALSE);  EQC_SET_ENABLE(hControl2, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_D_RNAME,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_NAME));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_DELIVERY_INFO_NAME);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelReceiverName);
				hControl3 = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, 1, \
										TEM_KOREAN, DELIVERY_INFO_NAME,(BYTE*)"", 0);
				EQC_SET_FOCUS(hControl3,FALSE);  EQC_SET_ENABLE(hControl3, FALSE);

				hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); 
				sprintf(texttmp, "%s %s",SF_MW_D_RPHONE,SKY_GET_TOKEN(TKN_MW_DELIVERY_INFO_PHONE));
				EQC_SetStaticText(hControl, texttmp);
				//EQC_StaticText(EQS_NA, EQS_NA, TKN_MW_DELIVERY_INFO_PHONE);
				//EQC_Image(EQS_NA,EQS_NA,&g_BmpMWalletPannelReceiverPhone);
				hControl4 = EQC_NumberEdit(EQS_NA, EQS_NA,MAX_WIDTH_LEN, 1, \
										  DELIVERY_INFO_PHONE+2, (BYTE*)"" );
				EQC_SET_FOCUS(hControl4,FALSE);  EQC_SET_ENABLE(hControl4, FALSE);
              
				EQC_SetNumberEditValue(hControl1,&tmpdeliveryinfo[deliverynum].dvZip);
				EQC_SetTextEditData(hControl2,&tmpdeliveryinfo[deliverynum].dvAddr);
				EQC_SetTextEditData(hControl3,&tmpdeliveryinfo[deliverynum].dvName);
				EQC_SetNumberEditValue(hControl4,&tmpdeliveryinfo[deliverynum].dvPhone);

				strcpy(deliveryinfo.dvZip,tmpdeliveryinfo[deliverynum].dvZip);
				strcpy(deliveryinfo.dvAddr,tmpdeliveryinfo[deliverynum].dvAddr);
				strcpy(deliveryinfo.dvName,tmpdeliveryinfo[deliverynum].dvName);
				strcpy(deliveryinfo.dvPhone,tmpdeliveryinfo[deliverynum].dvPhone);
			}

			/// ���ڼ����Ͻðڽ��ϱ�?
			len =  strlen((char *)SKY_GET_TOKEN(TKN_MW_ASK_FOR_SIGNING));
			linenum = len / MAX_WIDTH_LEN;
			if(len % MAX_WIDTH_LEN > 0 ) linenum ++;
			hControl1 = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_ENGLISH_LOWER,len,SKY_GET_TOKEN(TKN_MW_ASK_FOR_SIGNING),0);
			EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
			EQC_SET_FOCUS(hControl1,FALSE);
			//EQC_StaticText(EQS_NA,EQS_NA,TKN_MW_ASK_FOR_SIGNING);

/*			pToken[0] = SKY_GET_TOKEN(TKN_MW_CONFIRM);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_CANCEL);
			hControl1 = EQC_ScrollList(EQS_NA,EQS_NA,MAX_WIDTH_LEN,2,pToken,2,2,0);
			EQC_SET_STYLE( hControl1, CS_SCROLL_LIST_ENUMBERATED);  */

			EQS_StartFormatter();
			
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}
			EQS_EndFormatter();			

			EQS_FitCanvasSize();
			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_IO_TEDIT_DONE :
		case SE_SOFTKEY_MENU :
			// ���������� �߿��� ���� ����� �������� �����ִ� ȭ������ ��ȯ�Ѵ�.
			EQS_START_SIBLING_EVENT(SKA_ShowCertForSigning,SE_APP_START,FALSE,FALSE,FALSE);
			// ���ڼ���� ���Է��� �޾Ƶ��̴� ȭ������ ��ȯ�Ѵ�.
			//EQS_START_SIBLING_EVENT(SKA_CertPINInput,SE_APP_START,1,FALSE,FALSE);
			break;

		case SE_SOFTKEY_CANCEL :
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL);
			break;

/*		case SE_IO_SCROLL_LIST_SELECT:
			switch(pEvent->dwParam) {
				case 0 : 
					// ���ڼ���� ���Է��� �޾Ƶ��̴� ȭ������ ��ȯ�Ѵ�.
					EQS_START_SIBLING_EVENT(SKA_CertPINInput,SE_APP_START,1,FALSE,FALSE);
					break;
				case 1 :
					EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL);
					break;
				default :
					break;
			}
			break;
*/
		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, USER_CANCEL); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}	
}


// EFS�� �������� ������ ��ġ�� �˷��ش�. (index��)
// Input Parameter : 
//              1  :  MyCert
//              0  :  TCACert
// return : 
//				-1 : no place to save any more
//				1  : okay!! we found free space					
//				������ ���� EFS �ε���
int MW_GetSavePosition(BYTE MyCert, int *efs_pos, int *log_pos)
{
	BYTE	i; //, IdxOfNew, IdxOfList;
	static  BYTE	StatusOfCert[MAX_TCACERT_INFO];  // MAX_MYCERT_INFO���� ũ�ϱ�..

	*log_pos = 1;
	
	if ( MyCert ) {
		// ���� ����� ����Ǿ� �ִ����� �˱� ���ؼ�..
		// log_pos�� ����Ʈ�� ������ ������ ��ġ..
		for( i=0; i < MAX_MYCERT_INFO; i++ )
		{
			StatusOfCert[i] = SKY_GET_MW_MYCERT_DATA_SAVED(i);
			if ( StatusOfCert[i] > 0) *log_pos += 1;
		}

		// ���� ������ ���� EFS index�� �˾Ƴ���. : efs_pos
		*efs_pos = MAX_MYCERT_INFO + 1;
		for(i=0 ; i<MAX_MYCERT_INFO ; i++){
			if(StatusOfCert[i] == 0)
			{
				*efs_pos = i;
				break;
			}
		}

		// ������ ������ �ִ��� �˻�..
		if(*efs_pos == MAX_MYCERT_INFO + 1) 
		{
			// error : no place to save
			return -1;
		}

		StatusOfCert[*efs_pos] = *log_pos;
		//SKY_SET_MW_MYCERT_DATA_SAVED(efs_pos,log_pos);
		//SKY_SET_MW_MYCERT();

	} else {
		// ���� ����� ����Ǿ� �ִ����� �˱� ���ؼ�..
		// log_pos�� ����Ʈ�� ������ ������ ��ġ..
		for( i=0; i < MAX_TCACERT_INFO; i++ )
		{
			StatusOfCert[i] = SKY_GET_MW_TCACERT_DATA_SAVED(i);
			if ( StatusOfCert[i] > 0) *log_pos += 1;
		}

		// ���� ������ ���� EFS index�� �˾Ƴ���. : efs_pos
		*efs_pos = MAX_TCACERT_INFO + 1;
		for(i=0 ; i<MAX_TCACERT_INFO ; i++){
			if(StatusOfCert[i] == 0)
			{
				*efs_pos = i;
				break;
			}
		}

		// ������ ������ �ִ��� �˻�..
		if(*efs_pos == MAX_TCACERT_INFO + 1) 
		{
			// error : no place to save
			return -1;
		}

		StatusOfCert[*efs_pos] = *log_pos;
		//SKY_SET_MW_TCACERT_DATA_SAVED(efs_pos,log_pos);
		//SKY_SET_MW_TCACERT();
	}

	return 1;
}


// ����ڿ��� ��� Cert�� ��������� ���� 
// ������� ������ Cert�� ������, ���� ��ġ(EFS index)�� �����Ѵ�.	
// pEvent->wParam�� ������ ���� ���� �����ؼ� ȣ���ؾ� �Ѵ�.
//			1				:  TCACert
//			2				:  MYCert
// ���ϰ� :
// ����ڰ� ������ ���� �ʰڴٰ� ��Ҹ� ������ ��쿡�� parent���� 
// pEvent->wParam�� 0�� �����Ͽ� ������. 
// ����ڰ� ������ �ϴ� ���, ������ ���� EFS index�� �����Ѵ�.
VOID SKY_API SKA_AskForCertDelete(SKY_EVENT_T *pEvent)
{
	INT8			nIndex, nCtrl;
	static WORD		KindOfCert;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_NONE,
						  SKY_GET_TOKEN(TKN_MW_CERTIFICATION_INFO), 
						  WA_SHOW_HEADER | WA_SHOW_CML);
	
			KindOfCert = pEvent->wParam;
			nIndex = 0; 
			sAddInputControl(nIndex,1,ICT_E_NOSPACE,FALSE);
			nIndex += 1;
			sAddInputControl(nIndex,1,ICT_C_DELETE_OR_CANCEL_E,FALSE);
			nIndex += 1;

			EQS_StartFormatter();
			
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_CENTER, EVA_CENTER, FALSE);
					EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();		

			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
				case 0 :  
					if( KindOfCert == 1 ) { // TCACert
						EQS_START_SIBLING_EVENT(sSKA_MWTCACertSelect,SE_APP_START,1,FALSE,FALSE);
					} else if (KindOfCert == 2) {
						EQS_START_SIBLING_EVENT(sSKA_MWPRICertSelect,SE_APP_START,1,FALSE,FALSE);
					}
					break;
				case 1 :  
					EQS_END_PARAM(0,FALSE);  // ����ڰ� ��Ҹ� ������ ���..
					break;
			}
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}	
}

// ���� ����� ��� �������� ����Ұ����� ����ڿ��� �˸���.
LOCAL void SKY_API SKA_ShowCertForSigning(SKY_EVENT_T *pEvent)  
{
	int certIndex;
	int opt,linenum,nCtrl;
	HCONTROL hControl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_CERTIFICATION_INFO), 
						  WA_SHOW_HEADER | WA_SHOW_CML);
	
			certIndex = getEnumByID(getMobileWalletData.keyIdType,getMobileWalletData.keyId);

			if (certIndex == -1) {
				EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, GET_MW_DATA, NO_CERTIFICATE);
			} else {
				opt = MW_parse_display(certIndex,1);

				linenum = strlen(Display.IssuerName);
				if(linenum % MAX_WIDTH_LEN != 0) {
					linenum = (linenum / MAX_WIDTH_LEN) + 1;
				} else {
					linenum = linenum / MAX_WIDTH_LEN;
				}
				hControl = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN, linenum, TEM_ENGLISH_LOWER, strlen(Display.IssuerName),&(Display.IssuerName), 0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SET_FOCUS(hControl,FALSE);
				
				EQS_StartFormatter();
				
				for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
				{   
						EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
							ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
						//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
						EQS_NewLine();
				}

				EQS_EndFormatter();
				EQS_FitCanvasSize();
			}
			
			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_APP_RESUME: /* child applet is ended */
			break;

		case SE_SOFTKEY_CONFIRM :
	        // ���ڼ���� ���Է��� �޾Ƶ��̴� ȭ������ ��ȯ�Ѵ�.
			EQS_START_SIBLING_EVENT(SKA_CertPINInput,SE_APP_START,1,FALSE,FALSE);
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}	

}

VOID SKY_API SKA_MWCertResponse(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
		case SE_APP_START:
			EQS_START_SIBLING_EVENT(SKA_MWCheckMyCert,SE_APP_START,FALSE,FALSE,FALSE);
			break;

		case SE_APP_END:
			break;

		default:
			EQS_DEF_EVENT_PROC(pEvent);
			break;
    }
}

VOID SKY_API SKA_MWProcessCertResponse(SKY_EVENT_T *pEvent)
{
	CERT_RESPONSE *pCertResponse;
	static int efs_pos, log_pos;
	int error;
	
	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			efs_pos = pEvent->wParam;
			log_pos = pEvent->dwParam;
			pCertResponse = clnt_process_user_cert(CertMIME.data, CertMIME.length, &error);

			//space = MW_GetSavePosition(1, &efs_pos, &log_pos);

			if(clnt_write_user_cert(pCertResponse, efs_pos, log_pos))
			{
				EQS_START_CHILD_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_S_DOWNLOAD,FALSE);//Success MMI
			}
			else
			{
				EQS_START_CHILD_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_E_FAIL,FALSE);//Failure MMI
			}
			break;
		
		case SE_APP_END :
			EQS_SetDirectDraw(FALSE);
			break;

		case SE_APP_RESUME :
			EQS_END();
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

// MyCert���� 5�� �̻��� MyCert�� ����Ǿ� �ִ��� Ȯ���ϰ� 
// 5�� �̻� ����Ǿ� �ִµ� �� �ϳ��� ������ ���, 
// ����Ǿ� �ִ� 5���� MyCert�߿��� ����ڰ� �ϳ��� ������ �� �ֵ��� 
// MyCert ����Ʈ�� ����.
// ����ڰ� ������ MyCert�� �����.
void SKY_API SKA_MWCheckMyCert(SKY_EVENT_T *pEvent)
{
	int nCtrl,linenum,len,i,j;
	static int tcert,efs_pos,log_pos,check = 0;
	BYTE *pToken[5];
	HCONTROL hControl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_DELETE_MYCERT), 
						  WA_SHOW_HEADER | WA_SHOW_CML);

			if( MW_GetSavePosition(1,&efs_pos,&log_pos) == -1 ) {  // no space to save
				// show the MyCert list so that user can choose one
				len =  strlen(SKY_GET_TOKEN(TKN_MW_SELECT_MYCERT_DELETED));
				linenum = len / MAX_WIDTH_LEN;
				memset(texttmp,NULL,100);
				if(strlen(SKY_GET_TOKEN(TKN_MW_SELECT_MYCERT_DELETED)) % MAX_WIDTH_LEN > 0 ) linenum ++;
				hControl = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_ENGLISH_LOWER,len,\
														SKY_GET_TOKEN(TKN_MW_SELECT_MYCERT_DELETED),0);
				EQC_SET_STYLE(hControl,CS_TEXT_EDIT_READONLY);
				EQC_SET_FOCUS(hControl,FALSE);
				
				for(i = 0; i < MW_MYCERT_NUM; i++) {
					j = 0; check =1;
					pToken[i] = (BYTE *)SKY_Malloc( (SIZE_T) 100);
					memcpy(pToken[i],SKY_GET_MW_MYCERT_DISPLAY(i),100);
					while(pToken[i][j] != ';') j++;
					pToken[i][j] = NULL;
				}

				hControl = EQC_ScrollList(EQS_NA,EQS_NA,MAX_WIDTH_LEN,5,pToken,5,5,0);
				EQC_SET_STYLE( hControl, CS_SCROLL_LIST_ENUMBERATED );


				EQS_StartFormatter();
				
				for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
				{   
						EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
							ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
						//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
						EQS_NewLine();
				}

				EQS_EndFormatter();
				EQS_FitCanvasSize();
				EQS_SetIOCActive(hControl); 
		
			} else {
				// ���� ������ �ִ� ��쿡�� �ش� �ε����� �����Ѵ�.
				EQS_START_SIBLING_EVENT(SKA_MWProcessCertResponse,SE_APP_START,efs_pos,log_pos,FALSE);  
			}
			break;

		case SE_IO_SCROLL_LIST_SELECT:
			tcert = pEvent->dwParam;
			EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INFO_TEMPCERT_DELETE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			break;

		case SE_IO_SCROLL_LIST_CANCEL:
			EQS_END();
			break;

		case SE_IO_MESGBOX_END :
			for(i = 0; i<MW_MYCERT_NUM;i++) {
				if (SKY_GET_MW_MYCERT_DATA_SAVED(i) == tcert +1) break;
			}
			efs_pos = i;  log_pos = tcert +1;
			
			// �ش� ������ ����
			InitMWMyCert(i);
			SKY_SET_MW_MYCERT(i);

			EQS_START_SIBLING_EVENT(SKA_MWProcessCertResponse,SE_APP_START,efs_pos,log_pos,FALSE);  

			break;

		case SE_APP_END : 
			if (check) {
				for(i = 0; i < MW_MYCERT_NUM; i++) 
					SKY_Free(pToken[i]);
			}

			EQS_SetDirectDraw(TRUE);
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}		
}


//Start to handle downloaded Hashed MIME
VOID SKY_API SKA_MWHashedCertResponse(SKY_EVENT_T *pEvent)
{
	switch(pEvent->EventID)
	{
	case SE_APP_START :
		EQS_START_SIBLING_EVENT(SKA_MWCheckTCACert,SE_APP_START,0,FALSE,FALSE);
		break;

	case SE_APP_END :
		break;

	case SE_KEY_END : 
		break;

	case SE_KEY_CLEAR : 
		break;

	default :
		break;
	}
}

LOCAL VOID SKY_API SKA_MWCheckTCACert(SKY_EVENT_T *pEvent)
{
	int nCtrl,linenum,len,i,j;
	static int tcert,efs_pos,log_pos,select,check = 0;
	static BYTE *pToken[10];
	BYTE title[100];
	static HCONTROL hControl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_DELETE_TCACERT), 
						  WA_SHOW_HEADER | WA_SHOW_CML);

			select = pEvent->wParam;

			if( MW_GetSavePosition(0,&efs_pos,&log_pos) == -1 ) {  // no space to save
				// show the TCACert list so that user can choose one
				len =  strlen(SKY_GET_TOKEN(TKN_MW_SELECT_TCACERT_DELETED));
				linenum = len / MAX_WIDTH_LEN;
				if(strlen(SKY_GET_TOKEN(TKN_MW_SELECT_TCACERT_DELETED)) % MAX_WIDTH_LEN > 0 ) linenum ++;
				hControl = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_ENGLISH_LOWER, \
														len,SKY_GET_TOKEN(TKN_MW_SELECT_TCACERT_DELETED),0);
				EQC_SET_STYLE(hControl,CS_TEXT_EDIT_READONLY);
				EQC_SET_FOCUS(hControl,FALSE);
				
				for(i = 0; i < MW_TCACERT_NUM; i++) {
					if( SKY_GET_MW_TCACERT_DATA_SAVED(i) > 0 ) {
						j = 0; check = 1;
						pToken[i] = (BYTE *)SKY_Malloc( (SIZE_T) 50);
						memcpy(title,SKY_GET_MW_TCACERT_DISPLAY(i),50);
						while(title[j] != ';') { pToken[i][j] = title[j]; j++; }
						pToken[i][j] = NULL;
					}
				}

				hControl = EQC_ScrollList(EQS_NA,EQS_NA,MAX_WIDTH_LEN,6,pToken,MW_TCACERT_NUM,MW_TCACERT_NUM,0);
				EQC_SET_STYLE( hControl, CS_SCROLL_LIST_ENUMBERATED );


				EQS_StartFormatter();
				
				for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
				{   
						EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
							ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
						//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
						EQS_NewLine();
				}

				EQS_EndFormatter();
				EQS_FitCanvasSize();
				EQS_SetIOCActive(hControl); 
		
			} else {
				// ���� ������ �ִ� ��쿡�� �ش� �ε����� �����Ѵ�.
				if(select == 0) {
					EQS_START_SIBLING_EVENT(SKA_MWProcessHashedCert,SE_APP_START,efs_pos,log_pos,FALSE);  
				} else if (select == 1) {
					EQS_START_SIBLING_EVENT(SKA_MWProcessSignedCert,SE_APP_START,efs_pos,log_pos,FALSE);  
				} else { 
					ERR_FATAL("Wrong parameter : There is no routine for this value",0,0,0);
				}
			}
			break;

		case SE_IO_SCROLL_LIST_SELECT:
			tcert = pEvent->dwParam;
			if(tcert == 0 || tcert == 1 || tcert == 2) {
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INFO_NOT_DELETE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);	
			} else {
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INFO_TEMPCERT_DELETE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL:
			EQS_END();
			break;

		case SE_IO_MESGBOX_END :
			if (tcert > 2) {
				for(i = 0; i<MW_TCACERT_NUM;i++) {
					if (SKY_GET_MW_TCACERT_DATA_SAVED(i) == tcert +1) {
						efs_pos = i;  log_pos = tcert +1;
						break;
					}
				}
							
				// �ش� ������ ����
				InitMWTCACert(efs_pos);
				SKY_SET_MW_TCACERT(efs_pos);
				SKY_SET_MW_TCACERT_DATA_SAVED(efs_pos,0);

				if(select == 0) {
					EQS_START_SIBLING_EVENT(SKA_MWProcessHashedCert,SE_APP_START,efs_pos,log_pos,FALSE);  
				} else if (select == 1) {
					EQS_START_SIBLING_EVENT(SKA_MWProcessSignedCert,SE_APP_START,efs_pos,log_pos,FALSE);  
				} else { 
					ERR_FATAL("Wrong parameter : There is no routine for this value",0,0,0);
				}
			} else {
				EQS_SetIOCActive(hControl); 
			}

			break;

		case SE_APP_END : 
			if( check ) {
				for(i = 0; i < MW_TCACERT_NUM; i++)
					SKY_Free(pToken[i]);
			}
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}		
}

LOCAL VOID SKY_API SKA_MWProcessHashedCert(SKY_EVENT_T *pEvent)
{
	int verification_result,nCtrl;
	BYTE OutOfBandChannel[31];
	static int efs_pos, log_pos;
	HCONTROL hControl1, hControl2;
	
	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
						  SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_CERTIFICATION_INFO), 
						  WA_SHOW_HEADER | WA_SHOW_CML);

			efs_pos = pEvent->wParam;
			log_pos = pEvent->dwParam;

			hControl1 = EQC_TextEdit(EQS_NA, EQS_NA, MAX_WIDTH_LEN,3,\
									TEM_KOREAN,100, (BYTE *)(SKY_GET_TOKEN(TKN_MW_CERT_INPUT_FOR_HASH)),0);
			EQC_SET_STYLE(hControl1,CS_TEXT_EDIT_READONLY);
			EQC_SET_FOCUS(hControl1,FALSE);

			hControl2 = EQC_NumberEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN, 2, 30, (BYTE*)"" );
			EQC_SET_STYLE(hControl2, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER | \
							CS_NUMBER_EDIT_CURSOR | CS_NUMBER_EDIT_TP_PAUSE);
			EQC_SetNumberEditValue(hControl2,(BYTE *)"632331616805265561218933388389");

			EQS_StartFormatter();
				
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();
			EQS_SetIOCActive(hControl2); 

			break;

		case SE_IO_NEDIT_DONE :
			if (pEvent->wParam == EQS_GetNthIOC(1)) { 
				EQC_GetNumberEditValue(EQS_GetNthIOC(1),OutOfBandChannel);
				verification_result = verifyHashedCertificate(CertMIME.data, CertMIME.length, (char *)OutOfBandChannel, efs_pos, log_pos);

				if(!verification_result)
				{
					EQS_START_SIBLING_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_S_DOWNLOAD,FALSE,FALSE);//Success MMI
				}
				else
				{
					EQS_START_SIBLING_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_E_FAIL,FALSE,FALSE);//Failure MMI
				}

			}

			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_APP_RESUME : 
//			efs_pos = pEvent->wParam;
//			log_pos = pEvent->dwParam;
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		case SE_SOFTKEY_CONFIRM :
			EQS_END();
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

VOID SKY_API SKA_MWSignedCertResponse(SKY_EVENT_T *pEvent)
{
	switch(pEvent->EventID)
	{
	case SE_APP_START :
		EQS_START_SIBLING_EVENT(SKA_MWCheckTCACert,SE_APP_START,1,FALSE,FALSE);
		break;

	case SE_APP_END :
		break;

	case SE_KEY_END : 
		break;

	case SE_KEY_CLEAR : 
		break;

	default :
		break;
	}
}

LOCAL VOID SKY_API SKA_MWProcessSignedCert(SKY_EVENT_T *pEvent)
{
	int verification_result;
	static int efs_pos, log_pos;
	
	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
						  SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_CERTIFICATION_INFO), 
						  WA_SHOW_HEADER | WA_SHOW_CML);

			efs_pos = pEvent->wParam;
			log_pos = pEvent->dwParam;

			verification_result = verifySignedCertificate(CertMIME.data, CertMIME.length, efs_pos, log_pos);
			
			if(!verification_result)
			{
				EQS_START_SIBLING_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_S_DOWNLOAD,FALSE,FALSE);//Success MMI
			}
			else
			{
				EQS_START_SIBLING_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_E_FAIL,FALSE,FALSE);//Failure MMI
			}

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		case SE_SOFTKEY_CONFIRM : 
			EQS_END();
			break;

		default :
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

VOID SKY_API SKA_MWDeleteCert(SKY_EVENT_T *pEvent)
{
	int index;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_REVOKE_CERT),  
						  WA_SHOW_HEADER | WA_SHOW_CML);
			
			// ���⼭ search�� �����Ѵ�.
			if(!clnt_search_user_cert_dn((char *)CSRData.strSubject, &index))
			{
				CLNTa_log(1,0,"���������� ��Ͼ���.\n");
				// ���� ����... �޽��� ����.."������ �������� �����ϴ�."
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_DEBUG_NODATA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);				
			} else {
				EQS_START_CHILD_EVENT(sSKA_MWShowMYCertInfo, SE_APP_START, 1, index);
			}

			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_APP_RESUME: /* child applet is ended */
			break;

		case SE_IO_MESGBOX_END :
			EQS_END_EVENT(SE_WAP_SCRIPT_RESULT, REQUEST_CERT, NO_CERTIFICATE);
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END();
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}	
}

// ����ڷ� �Ͽ��� �������� �����Ҽ� �ֵ��� 
// ��ũ�Ѹ���Ʈ ��� --> �� �Է��� ����.
VOID SKY_API SKA_MWSignText(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
		case SE_APP_START:
			// ����ڰ� ������ �� �ֵ��� ������ ����Ʈ�� ����. pEvent->dwParam(online)���� �˸�.
			EQS_START_SIBLING_EVENT(sSKA_MWPRICertSelect,SE_APP_START,FALSE,online,FALSE);
			break;

		case SE_APP_END:
			break;

		case SE_APP_RESUME:
			EQS_END();
			break;

		default:
			break;
    }
}

VOID SKY_API SKA_MWEncryptText(SKY_EVENT_T *pEvent)
{
	int i = 0, j=0, len = 0, linenum = 0, sum = 0; 
	HCONTROL hControl;
	int nCtrl;
	static BYTE CtrlIndex[5] = {0,0,0,0,0}, CtrlInd[5] = {0,0,0,0,0};
	static char TmpUserPromptData[5][50];

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_SELECT_SAVE,
						  SKY_GET_TOKEN(TKN_MW_TEST_ENCRYPTINFO), 
						  WA_SHOW_HEADER | WA_SHOW_CML);
	
			// userPrompt�� ���� parsing�� �Ͽ� � control���� ��µɰ������� 
			// EncryptTxt����ü�� �����Ѵ�.
			MW_parse_userPrompt();

			for(i = 0; i< 5;i++) {
				if (EncryptTxt[i].datasaved) {
					// Substring���
					len =  strlen(EncryptTxt[i].Substring);
					linenum = len / MAX_WIDTH_LEN;
					memset(texttmp,NULL,100);
					memcpy(texttmp,EncryptTxt[i].Substring,len);
					if(len % MAX_WIDTH_LEN > 0 ) linenum ++;
					hControl = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_ENGLISH_LOWER,len,texttmp,0);
					EQC_SET_STYLE(hControl,CS_TEXT_EDIT_READONLY);
					EQC_SET_FOCUS(hControl,FALSE);

					if (EncryptTxt[i].format) {
						CtrlIndex[i] = 1;
  					    // userPrompt�� ����ϴ� ���..
						linenum = 4; len = 50;  // MW���� �Է������� ������??????
						if (EncryptTxt[i].numlimit) {  // �Է� ���� ���� ����.
							if(EncryptTxt[i].Digit[1] >= '0' && EncryptTxt[i].Digit[1] <= '9') {
								len = (EncryptTxt[i].Digit[0] - 0x30) * 10 + (EncryptTxt[i].Digit[1] - 0x30);
							} else {
								len = (EncryptTxt[i].Digit[0] - 0x30);
							}
							linenum = len / MAX_WIDTH_LEN;
							if( len % MAX_WIDTH_LEN > 0) linenum++;
						} 
						hControl = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_ENGLISH_LOWER,len,(BYTE *)"",0);

						switch(EncryptTxt[i].FormatChar) {
							case 'A' : EQC_SetTextEditModeSequence(hControl, MW_AMode);
								break;

							case 'a' : EQC_SetTextEditModeSequence(hControl, MW_aMode);
								break;

							case 'N' :	EQC_SetTextEditModeSequence(hControl, MW_NMode);
								break;

							case 'n' : EQC_SetTextEditModeSequence(hControl, MW_nMode);
								break;

							case 'X' : EQC_SetTextEditModeSequence(hControl, MW_XMode);
								break;

							case 'x' : EQC_SetTextEditModeSequence(hControl, MW_xMode);
								break;

							case 'M' : EQC_SetTextEditModeSequence(hControl, MW_MMode);
								break;

							case 'm' : EQC_SetTextEditModeSequence(hControl, MW_mMode);
								break;

							default :
								break;
						}

						// hidden�Ӽ��� �־����� ���..
						if(EncryptTxt[i].hidden) {
							EQC_SET_STYLE(hControl, CS_TEXT_EDIT_DEFAULT|CS_TEXT_EDIT_PASSWORD);
						} else {
							EQC_SET_STYLE(hControl, CS_TEXT_EDIT_DEFAULT);
						}

					} else {
						;   // Substring�� �ִ� ���...
					}
				}
			}
		
			EQS_StartFormatter();
			
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
					//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();
			EQS_SetIOCActive(EQS_GetNthIOC(1)); 

			sum = 0; 
			for(j = 0; j< 5; j++) {
				if (CtrlIndex[j] == 1) {
					CtrlInd[sum] = CtrlIndex[j] + j + sum; 
					sum++;
				} 
			}

			break;

		case SE_IO_TEDIT_DONE :
			if (CtrlInd[0] != 0 && pEvent->wParam == EQS_GetNthIOC(CtrlInd[0])) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(CtrlInd[0]),&TmpUserPromptData[0]);				
			} else if (CtrlInd[1] != 0 && pEvent->wParam == EQS_GetNthIOC(CtrlInd[1])) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(CtrlInd[1]),&TmpUserPromptData[1]);
			} else if (CtrlInd[2] != 0 && pEvent->wParam == EQS_GetNthIOC(CtrlInd[2])) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(CtrlInd[2]),&TmpUserPromptData[2]);
			} else if (CtrlInd[3] != 0 && pEvent->wParam == EQS_GetNthIOC(CtrlInd[3])) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(CtrlInd[3]),&TmpUserPromptData[3]);
			} else if (CtrlInd[4] != 0 && pEvent->wParam == EQS_GetNthIOC(CtrlInd[4])) {
				len = EQC_GetTextEditData(EQS_GetNthIOC(CtrlInd[4]),&TmpUserPromptData[4]);
			} else {
				ERR_FATAL("Can't be happen",0,0,0);
			}
			break;

		case SE_SOFTKEY_SAVE :
			for(i =0; i< 5; i++) {
				if (CtrlInd[i] != 0) {
					len = strlen(&TmpUserPromptData[i]);
					if(len == 0 || len < 0) {
						len = -1;
						break;
					}
				}
			}

			if(len == -1) {
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_ASK_INPUT_ALL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 						
			} else {
				j =0;
				for(i =0; i< 5; i++) {
					if (CtrlInd[i] != 0) {
						len = strlen(&TmpUserPromptData[i]);
						userPromptData[j] = len; j++;
						for(linenum = 0; linenum < len; linenum++)
							userPromptData[j + linenum] = TmpUserPromptData[i][linenum];
						j = j + len;
					}
				}
				PromptLen = j;
				EQS_END_EVENT(SE_WAP_SCRIPT_RESULT,ENCRYPT_TEXT,MMI_NO_ERROR);
			}

			break;

		case SE_IO_MESGBOX_END :
			break;

		case SE_SOFTKEY_CONFIRM :
			break;

		case SE_APP_DRAW :
			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}		
}

void MW_parse_userPrompt(void) 
{
	int mainIndex, subIndex,Index;
	int i,j,k;
	static int end = 0;
	BYTE userPrompt[1024];       // MW ����

	mainIndex = 0; subIndex = 0; Index = 0;

	InitEncryptText();

	while( mainIndex < 1024 && !end ) 
	{ 
		if (EncryptTextEx.userPrompt[mainIndex] != ';' && EncryptTextEx.userPrompt[mainIndex] != NULL)
		{
			userPrompt[subIndex] = EncryptTextEx.userPrompt[mainIndex]; 
			mainIndex++; subIndex++;		
		} else { // ';' Ȥ�� NULL�� ������, ��, �� subUserPrompt�� �����ϰ� �Ǹ�..

			// ';;'�� ���� ��쿡�� ';'�� ����ϴ� ���ڷ� �ν��Ѵ�.
			if(EncryptTextEx.userPrompt[mainIndex + 1] == ';') {
				userPrompt[subIndex] = EncryptTextEx.userPrompt[mainIndex];  // ù��° ';'
				mainIndex++; subIndex++;
				userPrompt[subIndex] = EncryptTextEx.userPrompt[mainIndex]; // �ι�° ';'
				mainIndex++; subIndex++;
			} else {
				
				if (EncryptTextEx.userPrompt[mainIndex] == NULL) end = 1;
				// ������ �������ش�.
				userPrompt[subIndex] = NULL;
				
				i = 0; j = 0; k = 0;
				
				do {
					while( userPrompt[j] != '%'  && j < subIndex ) {
							EncryptTxt[Index].Substring[i] = userPrompt[j];
							i++; j++;
					} 
					// '%%'�� �� ���...
					if(userPrompt[j +1] == '%') {
						EncryptTxt[Index].Substring[i] = userPrompt[j];  // ù��° '%'
						i++; j++;
						EncryptTxt[Index].Substring[i] = userPrompt[j];  // �ι�° '%'
						i++; j++;
					} else {
						k = 1;
					}
				} while(!k);

				// format������ �ؾ� �ϴ� ���.
				if( userPrompt[j] == '%') {
					EncryptTxt[Index].format = 1;
					j++;   // hidden �Ӽ� �˻�
					if(userPrompt[j] == '-') {
						EncryptTxt[Index].hidden = 1;
						j++;
					} else {
						EncryptTxt[Index].hidden = 0;
					}
					//j++;    // numlimit �˻�
					if(userPrompt[j] == '*') {
						EncryptTxt[Index].numlimit = 0;
						j++;
					} else {
						EncryptTxt[Index].numlimit = 1;
						//j++;    // digit�� �����Ѵ�.
						k = 0;
						while(userPrompt[j] >= '0' && userPrompt[j] <= '9')
							EncryptTxt[Index].Digit[k++] = userPrompt[j++];
						//EncryptTxt[Index].Digit[1] = userPrompt[j];
					}
					//j++;     // format-char�� �����Ѵ�.
					EncryptTxt[Index].FormatChar = userPrompt[j];
				} else {  // format������ �� �ʿ䰡 ���� ���.
					EncryptTxt[Index].format = 0;
				}

				EncryptTxt[Index].datasaved = 1;
						
				// ���� ���� subUserPrompt�� ���ؼ� �������� ������ �ش�.
				subIndex = 0; // �ٽ� ó������ ���� �� �ֵ��� 0���� �����Ѵ�.
				Index++;
				mainIndex++;
			}
		}

	}
}

void InitEncryptText(void)
{
	int i = 0;

	for(i = 0; i < 5 ;i++) {
		memset(EncryptTxt[i].Digit,NULL,2);
		EncryptTxt[i].format = 0;
		EncryptTxt[i].FormatChar = 0;
		EncryptTxt[i].hidden = 0;
		EncryptTxt[i].numlimit = 0;
		memset(EncryptTxt[i].Substring,NULL,50);
		EncryptTxt[i].datasaved = 0;
	}
}

VOID SKY_API SKA_MWTestShow(SKY_EVENT_T *pEvent)
{
	HCONTROL	hControl;
	BYTE	*pToken[5];

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			

			pToken[0] = SKY_GET_TOKEN(TKN_MW_CERT_DOWNLOAD_SUCCESS);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_CERT_SUCCESS);   
			pToken[2] = SKY_GET_TOKEN(TKN_MW_CERT_FORMAT_ERROR);
			pToken[3] = SKY_GET_TOKEN(TKN_MW_CERT_DOWNLOAD_FAIL);    
			pToken[4] = SKY_GET_TOKEN(TKN_MW_CERT_ERROR);   
			
			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,5,5,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END : 
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				EQS_START_CHILD_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_S_DOWNLOAD,FALSE);
			    break;
			case 1 :  
				EQS_START_CHILD_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_S_SUCCESS,FALSE);
				break;
			case 2 :  
				EQS_START_CHILD_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_E_FORMAT,FALSE);
				break;
			case 3 :  
				EQS_START_CHILD_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_E_DOWNLOAD,FALSE);
			    break;
//			case 4 :  
//				EQS_START_CHILD_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_E_NOSPACE,FALSE);
//				break;
			case 4 :  
				EQS_START_CHILD_EVENT(SKA_MWShowCertResult,SE_APP_START,ICT_E_FAIL,FALSE);
				break;
			default :  
	            break;
			}
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

//=============================================================
//  pEvent->wParam���� �Ѿ�� ���� ���� �ش� �޽����� ����Ѵ�.
//	ICT_S_DOWNLOAD   :    "�������� ���������� �ٿ�ε��Ͽ����ϴ�"
//	ICT_S_SUCCESS		:	 "�����Ͽ����ϴ�"
//	ICT_E_FORMAT		:	 "������ ���Ŀ� ������ �ֽ��ϴ�"
//	ICT_E_DOWNLOAD	:	  "������ �ٿ�ε忡 �����Ͽ����ϴ�"
//............��� ����.	ICT_E_NOSPACE		:	"������ ������ �����ϴ�. ����� �������� �����ұ��?"
//	ICT_E_FAIL				:	 "�����Ͽ����ϴ�"   
//==============================================================
VOID SKY_API SKA_MWShowCertResult(SKY_EVENT_T *pEvent)
{
	int nIndex,nCtrl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_CERTIFICATION_INFO), 
						  WA_SHOW_HEADER | WA_SHOW_CML);
	
			nIndex = 0; 
			sAddInputControl(nIndex,7,(MW_CONTROL_E)pEvent->wParam,FALSE);

			EQS_StartFormatter();
			
			for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
			{   
					EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
						ES_AUTO, 0,	 EHA_CENTER, EVA_CENTER, FALSE);
					EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
					EQS_NewLine();
			}

			EQS_EndFormatter();
			EQS_FitCanvasSize();		

			break;

		case SE_SOFTKEY_CONFIRM :
			EQS_END();
			break;

		case SE_APP_DRAW :
			
			//EQS_TextOutF(EQS_NA,EQS_NA,TKN_MW_CERT_DOWNLOAD_SUCCESS);
			
			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}	
}


/*
BYTE* MW_parse(BYTE* srcbuf)
{
	unsigned int buf_size, right_offset;
    unsigned int m=0, n=0;

#define LCD_LINE_SIZE 16
//    buf_size=strlen(getMobileWalletData.appendString);
	buf_size = strlen(srcbuf);

	while(buf_size--)
	{   
		// right_offset indicates the rest size of right part 
		right_offset = LCD_LINE_SIZE - n%LCD_LINE_SIZE;

		switch (getMobileWalletData.appendString[m]) 
        {
		case HORIZONTAL_TAB:
			if ( right_offset < LCD_HT_SIZE ) {
				memset(&shopping_list[n], ' ', right_offset);
				n += right_offset;
			}
			else {
				memset(&shopping_list[n], ' ', LCD_HT_SIZE);
				n += LCD_HT_SIZE;
			}
			break;
		case NEW_LINE:
		case CARRIAGE_RETURN:
		case ORDER_INFO_DELIMITER:
			if ( right_offset < LCD_NL_SIZE ) {
				memset(&shopping_list[n], ' ', right_offset);
				n += right_offset;
			}
			else {
				memset(&shopping_list[n], ' ', LCD_NL_SIZE);
				n += LCD_NL_SIZE;
			}
			break;
		default:
			shopping_list[n++] = getMobileWalletData.appendString[m++];
			break;
		} // end of switch
	} // end of while loop
}
*/

//==========================================================
// TempCert���� 2�� �̻��� TempCert�� ����Ǿ� �ִ��� Ȯ���ϰ� 
// 2�� �̻� ����Ǿ� �ִµ� �� �ϳ��� MIME�� ������ ���, 
// ����Ǿ� �ִ� 2���� TempCert�߿��� ����ڰ� �ϳ��� ������ �� �ֵ��� 
// TempCert ����Ʈ�� ����.
// ����ڰ� ������ TempCert�� �����.
// pEvent->wParam == 1 �̸� parent applet�� ���ο� mime�� tempcert�� ������ �� �ִ�. 
// ===========================================================
VOID SKY_API SKA_MWCheckTempCert(SKY_EVENT_T *pEvent)
{
	int nCtrl,linenum,len;
	static int tcert;
//	static char dn[2][50];
	HCONTROL hControl;
	BYTE *pToken[2];

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 

			EQS_SetDirectDraw(FALSE);
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_MW_DELETE_TEMPCERT), 
						  WA_SHOW_HEADER | WA_SHOW_CML);
	
			if(SKY_GET_MW_TEMPCERT_DATA_SAVED(0) == 1 && \
				SKY_GET_MW_TEMPCERT_DATA_SAVED(1) == 1 ) {    // no space to save
				// show the TempCert list so that user can choose one
				len =  strlen(SKY_GET_TOKEN(TKN_MW_SELECT_TEMPCERT_DELETED));
				linenum = len / MAX_WIDTH_LEN;
				memset(texttmp,NULL,100);
				memcpy(texttmp,SKY_GET_TOKEN(TKN_MW_SELECT_TEMPCERT_DELETED),len);
				if(strlen(SKY_GET_TOKEN(TKN_MW_SELECT_TEMPCERT_DELETED)) % 16 > 0 ) linenum ++;
				hControl = EQC_TextEdit(EQS_NA,EQS_NA,MAX_WIDTH_LEN,linenum,TEM_ENGLISH_LOWER,len,texttmp,0);
				//EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 16, (BYTE*)"",0);
				EQC_SET_STYLE(hControl,CS_TEXT_EDIT_READONLY);
				EQC_SET_FOCUS(hControl,FALSE);
				
				MW_parse_SubjectDN(&dn[0], 0);
				pToken[0] = &dn[0];
				MW_parse_SubjectDN(&dn[1], 1);
				pToken[1] = &dn[1];
				
				hControl = EQC_ScrollList(EQS_NA,EQS_NA,MAX_WIDTH_LEN,2,pToken,2,2,0);
				EQC_SET_STYLE( hControl, CS_SCROLL_LIST_ENUMBERATED );


				EQS_StartFormatter();
				
				for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
				{   
						EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
							ES_AUTO, 0,	 EHA_LEFT, EVA_CENTER, FALSE);
						//EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
						EQS_NewLine();
				}

				EQS_EndFormatter();
				EQS_FitCanvasSize();
				EQS_SetIOCActive(hControl); 
		
			} else {
				EQS_START_SIBLING(SKA_CertPINInput,FALSE);
				//EQS_START_SIBLING(SKA_MWCertResponse,FALSE);
				//EQS_END_PARAM(1,FALSE);
			}
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
				case 0 :  
					tcert = 0;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INFO_TEMPCERT_DELETE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					break;
				case 1 :  
					tcert = 1;
					EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_INFO_TEMPCERT_DELETE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
					break;
				default :  
					break;
			}
			break;

		case SE_IO_SCROLL_LIST_CANCEL:
			EQS_END();
			break;

		case SE_IO_MESGBOX_END :
			if(tcert == 0) {
				SKY_SET_MW_TEMPCERT_DATA_SAVED(0,0);
				SKY_SET_MW_WALLET();
				InitMWTempCert(0);
				SKY_SET_MW_TEMPCERT(0);
			} else if (tcert == 1) {
				SKY_SET_MW_TEMPCERT_DATA_SAVED(1,0);
				SKY_SET_MW_WALLET();
				InitMWTempCert(1);
				SKY_SET_MW_TEMPCERT(1);
			}
			EQS_START_SIBLING(SKA_CertPINInput,FALSE);
			//EQS_END_PARAM(1,FALSE);
			break;

		case SE_APP_END : 
			EQS_SetDirectDraw(TRUE);
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}		
}

LOCAL void MW_parse_SubjectDN(char *dn, int ind)
{
	int i,j;
	int find = 0;
	BYTE tmpbuf[MW_TEMPCERT_SUBJECT_DN];

	i = 0; j = 0;
	memset(tmpbuf,NULL,MW_TEMPCERT_SUBJECT_DN);
	memcpy(tmpbuf,SKY_GET_MW_TEMPCERT_SUBJECT_DN(ind),MW_TEMPCERT_SUBJECT_DN);

	while(!find && i < MW_TEMPCERT_SUBJECT_DN) {
		while(tmpbuf[i] != '/') i++;
		i++;
		if(tmpbuf[i] == 'O') {
			find = 1;
			i = i+2;
		}
	}

	if (i < MW_TEMPCERT_SUBJECT_DN) {
		while(tmpbuf[i] != '/') { 
			*(dn + j) = tmpbuf[i]; 
			i++; j++; 
		}
		*(dn + j) = NULL;
	} else {
		strcpy(dn,"TempCert");
	}
}


/******************************************************************** online menu end ************/

LOCAL void SKY_API sSKA_MWTestInfo(SKY_EVENT_T *pEvent)
{
	HCONTROL	hControl;
	BYTE	*pToken[3];

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			

			pToken[0] = SKY_GET_TOKEN(TKN_MW_DEBUG_TEMPCERT);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_DEBUG_MYCERT);   
			pToken[2] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT);   
			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,3,3,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END : 
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				EQS_START_CHILD(sSKA_MWTestTempCert);
			    break;
			case 1 :  
				EQS_START_CHILD(sSKA_MWTestMyCert);
				break;
			case 2 :  
				EQS_START_CHILD(sSKA_MWTestCACert);
				break;
			default :  
	            break;
			}
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWTestTempCert(SKY_EVENT_T *pEvent)
{
	HCONTROL	hControl;
	BYTE	*pToken[2];

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			

			pToken[0] = SKY_GET_TOKEN(TKN_MW_DEBUG_TEMPCERT1);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_DEBUG_TEMPCERT2);   
			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,2,2,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END : 
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestTempCertInfo,SE_APP_START,0,FALSE);
			    break;
			case 1 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestTempCertInfo,SE_APP_START,1,FALSE);
				break;
			default :  
	            break;
			}
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWTestTempCertInfo(SKY_EVENT_T *pEvent)
{
	int nCtrl,linenum,len, index=0;
	BYTE buf[500];
	HCONTROL	hControl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			
			if(pEvent->wParam == 0 ) index = 0;
			else if(pEvent->wParam == 1) index = 1;

			if (SKY_GET_MW_TEMPCERT_DATA_SAVED(index) != 0) {  // ����� �����Ͱ� �ִ� ���..
				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TEMP_LENGTH);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 16, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetTextEditData(hControl,char2bin(SKY_GET_MW_TEMPCERT_LENGTH(index)));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TEMP_KeyUsage);  
				hControl = EQC_NumberEdit(EQS_NA,EQS_NA, 16, 1, 16,(BYTE *)"");
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetNumberEditValue(hControl,SKY_GET_MW_TEMPCERT_KEY_USAGE(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TEMP_PINHASH);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, 2,TEM_ENGLISH_LOWER, 20, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetTextEditData(hControl,SKY_GET_MW_TEMPCERT_PIN_HASH(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TEMP_PRVKEY);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_TEMPCERT_PRV_KEY(index));
				len = strlen(buf);
				if(len % MAX_WIDTH_LEN == 0) {
					linenum = len / MAX_WIDTH_LEN;
				} else {
					linenum = (len / MAX_WIDTH_LEN) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, MAX_WIDTH_LEN, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_TEMPCERT_PRV_KEY(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TEMP_SKID);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, MAX_WIDTH_LEN, 2,TEM_ENGLISH_LOWER, 20, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetTextEditData(hControl,SKY_GET_MW_TEMPCERT_SKID(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TEMP_RequestDate);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 16, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetTextEditData(hControl,SKY_GET_MW_TEMPCERT_REQUESTDATE(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TEMP_SubjectDN);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_TEMPCERT_SUBJECT_DN(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetTextEditData(hControl,SKY_GET_MW_TEMPCERT_SUBJECT_DN(index));


				EQS_StartFormatter();
				
				for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
				{   
						EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
							ES_AUTO, 0,	 EHA_CENTER, EVA_CENTER, FALSE);
						EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
						EQS_NewLine();
				}

				EQS_EndFormatter();
				EQS_FitCanvasSize();
        
			} else {
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_DEBUG_NODATA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}
			
			break;

	    case SE_IO_MESGBOX_END:
			EQS_END();
			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_SOFTKEY_CONFIRM :
			EQS_END();
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWTestMyCert(SKY_EVENT_T *pEvent)
{
	HCONTROL	hControl;
	BYTE	*pToken[5];

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			

			pToken[0] = SKY_GET_TOKEN(TKN_MW_DEBUG_MYCERT1);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_DEBUG_MYCERT2);   
			pToken[2] = SKY_GET_TOKEN(TKN_MW_DEBUG_MYCERT3);    
			pToken[3] = SKY_GET_TOKEN(TKN_MW_DEBUG_MYCERT4);   
			pToken[4] = SKY_GET_TOKEN(TKN_MW_DEBUG_MYCERT5);    

			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,5,5,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END : 
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestMyCertInfo,SE_APP_START,0,FALSE);
			    break;
			case 1 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestMyCertInfo,SE_APP_START,1,FALSE);
				break;
			case 2 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestMyCertInfo,SE_APP_START,2,FALSE);
			    break;
			case 3 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestMyCertInfo,SE_APP_START,3,FALSE);
				break;
			case 4 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestMyCertInfo,SE_APP_START,4,FALSE);
			    break;
			default :  
	            break;
			}
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWTestMyCertInfo(SKY_EVENT_T *pEvent)
{
	int index,nCtrl,linenum,len;
	BYTE buf[500];
	HCONTROL	hControl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			
			index = pEvent->wParam;

			if (SKY_GET_MW_MYCERT_DATA_SAVED(index) != 0) {  // ����� �����Ͱ� �ִ� ���..
				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_LENGTH);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 16, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_LENGTH(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_CheckField);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 16, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_CHECKFIELD(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_PINHASH);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_MYCERT_PIN_HASH(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_PIN_HASH(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_PRVKEY);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_MYCERT_PRV_KEY(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_PRV_KEY(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_DISPLAY);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_MYCERT_DISPLAY(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_DISPLAY(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_SubjectKID);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_MYCERT_SUBJECT_KID(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_SUBJECT_KID(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_IssuerKID);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_MYCERT_ISSUER_KID(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_ISSUER_KID(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_SubjectDN);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_MYCERT_SUBJECT_DN(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_SUBJECT_DN(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_MY_CertURL);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_MYCERT_CERT_URL(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_MYCERT_CERT_URL(index));


				EQS_StartFormatter();
				
				for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
				{   
						EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
							ES_AUTO, 0,	 EHA_CENTER, EVA_CENTER, FALSE);
						EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
						EQS_NewLine();
				}

				EQS_EndFormatter();
				EQS_FitCanvasSize();
        
			} else {
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_DEBUG_NODATA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}
			
			break;

	    case SE_IO_MESGBOX_END:
			EQS_END();
			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_SOFTKEY_CONFIRM :
			EQS_END();
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWTestCACert(SKY_EVENT_T *pEvent)
{
	HCONTROL	hControl;
	BYTE	*pToken[10];

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			

			pToken[0] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT1);    
			pToken[1] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT2);   
			pToken[2] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT3);    
			pToken[3] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT4);   
			pToken[4] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT5);    
			pToken[5] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT6);   
			pToken[6] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT7);    
			pToken[7] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT8);   
			pToken[8] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT9);    
			pToken[9] = SKY_GET_TOKEN(TKN_MW_DEBUG_CACERT10);   

			hControl = EQC_ScrollList(EQS_NA,EQS_NA,EQS_NA,EQS_NA,pToken,10,10,0);
			EQC_SET_STYLE( hControl, CS_SCROLL_LIST_BORDER      |\
									CS_SCROLL_LIST_ENUMBERATED |\
									CS_SCROLL_LIST_MAXIMIZE);

			EQS_SetIOCActive(hControl); 
			break;

		case SE_APP_END : 
			break;

		case SE_IO_SCROLL_LIST_SELECT:
	        switch(pEvent->dwParam)
		    {
			case 0 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,0,FALSE);
			    break;
			case 1 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,1,FALSE);
				break;
			case 2 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,2,FALSE);
			    break;
			case 3 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,3,FALSE);
				break;
			case 4 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,4,FALSE);
			    break;
			case 5 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,5,FALSE);
				break;
			case 6 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,6,FALSE);
			    break;
			case 7 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,7,FALSE);
				break;
			case 8 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,8,FALSE);
			    break;
			case 9 :  
				EQS_START_CHILD_EVENT(sSKA_MWTestCACertInfo,SE_APP_START,9,FALSE);
				break;

			default :  
	            break;
			}
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_IO_SCROLL_LIST_CANCEL : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}

LOCAL void SKY_API sSKA_MWTestCACertInfo(SKY_EVENT_T *pEvent)
{
	int index,nCtrl,linenum,len;
	DWORD temp;
	BYTE temp2[6];
	BYTE buf[500];
	HCONTROL	hControl;

	switch( pEvent->EventID ) 
	{
		case SE_APP_START : 
			EQS_SetWindow(SKY_DEF_WIN_X,SKY_DEF_WIN_Y,
				          SKY_DEF_WIN_DX,SKY_DEF_WIN_DY,
						  CL_NONE_CONFIRM_NONE,
						  SKY_GET_TOKEN(TKN_TEST_MENU),  // ��ū �̸� �ٲ����..
						  WA_SHOW_HEADER | WA_SHOW_CML);
			
			index = pEvent->wParam;
			
			if (SKY_GET_MW_TCACERT_DATA_SAVED(index) != 0) {  // ����� �����Ͱ� �ִ� ���..
				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_LENGTH);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 16, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_LENGTH(index));
				EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_LENGTH(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_NOTAFTER);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 16, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				temp = SKY_GET_MW_TCACERT_NOT_AFTER(index);
				temp2[1] = temp >> 3*8;
				temp2[2] = temp >> 2*8;
				temp2[3] = temp >> 1*8;
				temp2[4] = temp; temp2[5] = NULL;
				EQC_SetTextEditData(hControl,temp2);

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_CheckField);  
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, 1,TEM_ENGLISH_LOWER, 2, (BYTE*)"",0);
				EQC_SET_ENABLE(hControl, FALSE);
				EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_CHECKFIELD(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_DISPLAY);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_TCACERT_DISPLAY(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_DISPLAY(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_SubjectKID);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_TCACERT_SUBJECT_KID(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_SUBJECT_KID(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_IssuerKID);
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_TCACERT_ISSUER_KID(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_ISSUER_KID(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_CAINFOURL);  
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_TCACERT_CA_INFO_URL(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_CA_INFO_URL(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_SubjectDN); 
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_TCACERT_SUBJECT_DN(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_SUBJECT_DN(index));

				EQC_StaticText(EQS_NA,EQS_NA, TKN_MW_DEBUG_TCA_CERT); 
				memset(buf,NULL,500);
				strcpy(buf,SKY_GET_MW_TCACERT_CERT(index));
				len = strlen(buf);
				if(len % 16 == 0) {
					linenum = len / 16;
				} else {
					linenum = (len / 16) + 1;
				}
				hControl = EQC_TextEdit(EQS_NA,EQS_NA, 16, linenum,TEM_ENGLISH_LOWER, len, (BYTE*)buf,0);
				EQC_SET_ENABLE(hControl, FALSE);
				//EQC_SetTextEditData(hControl,SKY_GET_MW_TCACERT_CERT(index));


				EQS_StartFormatter();
				
				for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
				{   
						EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
							ES_AUTO, 0,	 EHA_CENTER, EVA_CENTER, FALSE);
						EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
						EQS_NewLine();
				}

				EQS_EndFormatter();
				EQS_FitCanvasSize();
        
			} else {
				EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MW_DEBUG_NODATA), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
			}
			
			break;

	    case SE_IO_MESGBOX_END:
			EQS_END();
			break;

		case SE_APP_END : 
			break;

		case SE_APP_RESUME: /* child applet is ended */
			EQS_SetIOCActive(EQS_GetNthIOC(0));
			break;

		case SE_KEY_END : 
			EQS_END_TO(EQS_POP_TO_ROOT);
			break;

		case SE_SOFTKEY_CONFIRM :
			EQS_END();
			break;

		case SE_KEY_CLEAR : 
			EQS_END(); 
			break;

		default : 
			EQS_DEF_EVENT_PROC(pEvent);
			break;
	}
}


BYTE* sGetMyNumber(void)
{
   nv_dir_number_type  nv_dir_num;
   UINT8               iLoop = 0, jLoop;

   nv_dir_num.nam = SKY_GET_CURRENT_NAM();
   (void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );

   if( nv_dir_num.dir_number[0] != '0' ) 
   { // 4�ڸ� ���� 011-1234-5678 
     MyNumber[0] = '0';
     iLoop = 1;

     for(jLoop = 0; jLoop < NV_DIR_NUMB_SIZ; iLoop++,jLoop++)
     {
       MyNumber[iLoop] = nv_dir_num.dir_number[jLoop];
     }
   }
   else
   {
     for(iLoop = 0; iLoop < NV_DIR_NUMB_SIZ; iLoop++)
     {
       MyNumber[iLoop] = nv_dir_num.dir_number[iLoop];
     }
   }

   return MyNumber;
} 

char *char2bin(char* w)
{
	int i;
	char temp[400];

	i = 0;
	while(w[i] != NULL) {
		sprintf((char*)temp[i],"%x",w[i]);
		i++;
	}
	temp[i] = NULL;

	return (temp);
}
