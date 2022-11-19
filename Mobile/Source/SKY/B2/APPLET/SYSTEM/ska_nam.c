/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_nam.c
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
** 2000-12-10   hjcho      Created.
**
** ===========================================================================
*/



/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/ 
#include "sky.h"
#include "ska.h"


#include "customer.h"
#include "target.h"
#include "comdef.h"



//#include "hs.h"
#include "nv.h"
#include "msg.h"
#include "err.h"
#include "mc.h"
#include "db.h"
#include "mccreg.h"
#include "mccdma.h"



/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define INPUT_FROM_SIMPLE_NAM             100

#define NAM_STRD_INPUT_CONTROL_WIDTH      15
#define NAM_SHORT_INPUT_CONTROL_WIDTH      7

/*
**      SCI SCM display position
*/
#define NAM_SCISCM_XPOS                    0                
#define NAM_SCISCM_YPOS                   60


/*
**     the kind of NAM
*/
#define NAM1      0
#define NAM2      1
#define NAM3      2
#define NAM4      3
#define NAM5      4


/*
**     A Only
**     A Prefered
**     B Only
**     B Prefered
*/
#define NUM_NAM_PREF_SYS                   4
#define SIZE_PREF_SYS                     15


/*
**     Yes
**     No
*/
#define NUM_YESNO_LIST                    2


/*
**     # of SIDNID pair in Scroll list
**
*/
#define NUM_SIDNID_LIST                   7


/*
**     EQUS NAM buffer size
*/
#define EQS_SIZE_NAM_BUFFER               20

/*
**     EQUS MAX MIN SIZE
*/
#define EQS_MAX_MIN_SIZE                  10

/*
**     EQUS MAX AREA CODE
*/
#define EQS_MAX_TMP_BUF                   20

/*
**     EQUS Max phone number
*/
#define EQS_MIN_PHONE_NUM                 10


/*
**     EQS SID NID CHANGE
*/
#define NO_CHANGE_SIDNID                -255



/*
**  Maximum SID MID range
**
*/
#define MAX_SID_RANGE       32767      /*  Maximum SID value.                */
#define MAX_NID_RANGE       65535      /*  Maximum NID value.                */
#define MAX_CHANNEL_RANGE   0x3FF      /*  Maximum Channel range             */


/*
** Temp MAX_NAME_LTRS
*/ 
#define  EQS_NAME_LTRS         16

/*
** Temporal Define
*/  
#define  EQS_SPACES_TXT	       "                0"

/*
** # of MSM3100 NAM Programming items
*/
#define  EQS_NUM_NAMITEM_MSM3100       7

/*
** # of MSM5000 NAM Programming items
*/
#define  EQS_NUM_NAMITEM_MSM5000      10

/*
** # of SubMenu
*/
/* hjcho 2000-11-28 --> @why, what */
#if   (MODEL_ID == MODEL_B0)
#define  EQS_NUM_SUBMENU      4
#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
#define  EQS_NUM_SUBMENU      5
#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)*/
/* <-- */

#define  UI_WIDE               17  // it will be changed!!
#define  SKY_SPACE             ' ' // it will be changed!!

#define  SKY_NAM_ONESEC        1
#define  SKY_NAM_TWOSEC        2
#define  SKY_NAM_THREESEC      3

#define  NAM_MESG_START_XPOS   2

#define PCDMACH_NAM2_DEFAULT	568
#define PCDMACH_NAM2_DEFAULT_B	486
#define SCDMACH_NAM2_DEFAULT	738
#define SCDMACH_NAM2_DEFAULT_B	779

#define PCDMACH_NAM3_DEFAULT	283
#define PCDMACH_NAM3_DEFAULT_B	384
#define SCDMACH_NAM3_DEFAULT	691
#define SCDMACH_NAM3_DEFAULT_B	777

#define PCDMACH_NAM4_DEFAULT	283
#define PCDMACH_NAM4_DEFAULT_B	283
#define SCDMACH_NAM4_DEFAULT	691
#define SCDMACH_NAM4_DEFAULT_B	691

#define PCDMACH_NAM5_DEFAULT	384
#define PCDMACH_NAM5_DEFAULT_B	384
#define SCDMACH_NAM5_DEFAULT	777
#define SCDMACH_NAM5_DEFAULT_B	777




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
//STATIC nv_min1_type min1;         /* hold min1s */
//STATIC nv_min2_type min2;         /* hold min2s */


enum tag_NAM_ITEM_E{
  ESN_I = 0,        /* ESN */
  AKEY_I,       /* Press [AKEY] to continue */
  NAM_I,        /* NAM ? SETTINGS */

  PHONENO_I,    /* NAM ? PHONE NUMBER */
  HOME_I,       /* NAM ? HOME SID */
  NAMNAME_I,    /* NAM ? NAME */

  EXIT_MORE_I,  /* more or exit */

  SECURITY_I,   /* SERVICE SEC. CODE */

  CDMAMCC_I,    /* NAM ? MOBILE COUNTRY CODE default is 310 */
  NMSID_I,      /* NATIONAL MOBILE STATION ID */

  /* hjcho 2000-11-18 --> @why, what */
#if (MODEL_ID == MODEL_B0) 
#ifdef FEATURE_IS95B_TRUE_IMSI
#error code not present
#endif 
#else  //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
#ifdef FEATURE_IS95B_TRUE_IMSI
  TCDMAMCC_I,    /* NAM ? True IMSI MCC default is 310 */
  TNMSID_I,      /* True NATIONAL MOBILE STATION ID */
#endif
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
  /* <-- */



#if defined(FEATURE_CDMA_800)  /* Dual mode phone */

#ifdef FEATURE_SSPR_ENHANCEMENTS
  PRL_ENABLED_I, /* NAM ? PRL Enabled ? */
#endif
#endif /* (FEATURE_CDMA_800)  */

#if defined(FEATURE_CDMA_800) || (defined(FEATURE_CDMA_1900) && defined(FEATURE_ACP)) /* Dual mode or dual band phone */
  CDMADIRNUM_I, /* NAM ? CDMA PHONE NUMBER */
#endif /* (FEATURE_ACP) */


#if defined(FEATURE_CDMA_800)  /* Dual mode phone */
  CDMAHOMESIDNID_I,   /* NAM ? CDMA HOME SID/NID */
  CDMA_PRICHNA_I,     /* NAM ? CDMA Primary Chn A */
  CDMA_SECCHNA_I,     /* NAM ? CDMA Sec Chn A */
  CDMA_PRICHNB_I,     /* NAM ? CDMA Primary Chn B */
  CDMA_SECCHNB_I,     /* NAM ? CDMA Sec Chn B */
  LOCK_I,             /* NAM ? LOCK OUT SYSTEM, writes to both CDMA & Analog */

#elif defined(FEATURE_CDMA_1900) && defined(FEATURE_ACP) /* Dual band phone */
  CDMAHOMENID_I,     /* NAM ? CDMA HOME NID */
#endif /* (FEATURE_CDMA_800)  */

  CDMA_HOMESYS_I,     /* NAM ? Home Sys Reg */
  CDMA_FORGNSID_I,    /* NAM ? Forgn SID Reg */
  CDMA_FORGNNID_I,    /* NAM ? Forgn NID Reg */

#ifdef FEATURE_ACP
  AMPSPHONE_I,  /* NAM ? AMPS PHONE NUMBER */
  AMPSHOMESID_I,   /* NAM ? AMPS HOME SID */
  AMPS_1PGCHNL_I,  /* NAM ? AMPS 1st Paging Channel */
  AMPS_AUTOREG_I,  /* NAM ? AMPS AutoReg */
#endif /* FEATURE_ACP */

  OVERLOAD_I,   /* NAM ? Access Overload Class */
  SMS_UTC_I,    /* SMS UTC time */
  MODEL_I,      /* PHONE MODEL */
  SLOTIDX_I,    /* SLOT CYCLE INDEX */
  SCM_I,        /* STATION CLASS MARk */
  /* hjcho 2000-11-26 --> @why, what */

#if (MODEL_ID == MODEL_B0)
/* It is only used for MSM3100 */
  CDMAHOMESID_I,   /* NAM ? CDMA HOME SID */    
#endif /* (MODEL_ID == MODEL_B0) */
/* <-- */

/* hjcho 2000-11-26 --> @why, what */
/* 
   Prefered system, Preferted System Select application
   is implemented by scroll list 
*/
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
 PREFSYS_I,
 CDMAMNC_I,
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
/* <-- */
  ENDITEM_I,
  LAST_I
};

typedef enum tag_NAM_ITEM_E  NAM_ITEM_E;


enum tagNAM_PHONENUMBER_E
{
//  NP_TITLE             = 0,
  NP_PHONENUMBER_TITLE = 0,
  NP_PHONENUMBER       ,
  NP_MAX               
};

enum tagNAM_CHANNEL_E
{
  /* hjcho 2000-11-28 --> @why, what */
#if (MODEL_ID == MODEL_B0)
  NC_PRIMARYCHANNEL_TITLE   = 0,
  NC_PRIMARYCHANNEL         ,
  NC_SECONDARYCHANNEL_TITLE ,
  NC_SECONDARYCHANNEL       ,
  NC_MAX                      
#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
  NC_FIRSTCHANNELA_TITLE    = 0,
  NC_FIRSTCHANNELA          ,
  NC_SECONDCHANNELA_TITLE   ,
  NC_SECONDCHANNELA         ,
  NC_FIRSTCHANNELB_TITLE    ,
  NC_FIRSTCHANNELB          ,
  NC_SECONDCHANNELB_TITLE   ,
  NC_SECONDCHANNELB         ,
  NC_MAX                    
#endif  /*(MODEL_ID == MODEL_B0)*/
  /* <-- */
};


enum tagNAM_SIDNID_E
{
  /* hjcho 2000-11-30 --> @why, what */
#if (MODEL_ID == MODEL_B0)
  NS_SIDNID_TITLE     ,
  NS_SID_TITLE        ,
  NS_SID              ,
  NS_NID_TITLE        ,
  NS_NID              ,
  NS_MAX              
#elif(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
  NS_SIDNID       = 0 ,
  NS_MAX                
#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)*/
  /* <-- */
};

enum tagNAM_EDITSIDNID_E
{
  NE_TITLE            = 0,
  NE_SID_TITLE        ,
  NE_SID              ,
  NE_NID_TITLE        ,
  NE_NID              ,
  NE_MAX              
};


enum tagNAM_PREFSYS_E
{
  NPS_PREFSYS        = 0,
  NPS_MAX                 
};

enum tagNAM_MCC_E
{
  NM_MCC_TITLE       = 0,
  NM_MCC             ,
  NM_MAX                  
};

enum tagNAM_MNC_E
{
  NMN_MNC_TITLE           ,
  NMN_MNC                 ,
  NMN_MAX                 
};


enum tagNAM_PRL_E
{
  NPE_PRL               ,
  NPE_MAX                 
};    


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
STATIC BYTE*         pszPreferedSys[NUM_NAM_PREF_SYS];
STATIC BYTE*         pszYesNo[NUM_YESNO_LIST];
STATIC BYTE*         pszSIDNIDPair[NUM_SIDNID_LIST+1];
BYTE                 pszNAMInfo[EQS_SIZE_NAM_BUFFER];


STATIC nv_item_type  nvi;
STATIC CONST CHAR    aMINTable1[EQS_MAX_MIN_SIZE] = 
{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

STATIC CONST WORD    aMINTable2[EQS_MAX_MIN_SIZE] =
{ 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

#ifdef FEATURE_CDMA_800
/* SID/NID edit substate */
//STATIC BYTE gbHOME_SIDNID_INDEX    = 0;     //home_sidnid_indx
//STATIC BYTE gbLOCKOUT_SIDNID_INDEX = 0;     //lockout_sidnid_indx
#endif
// mcc_changed
STATIC BOOL sfMCCChanged = FALSE;/* falg indicating that mcc was changed */

#ifdef FEATURE_IS95B_TRUE_IMSI
STATIC BOOL fTMCCChanged  =   FALSE;              // tmcc_changed
STATIC nv_min1_type           lIMSI_T_S1;         // nvisrv_imsi_t_s1
STATIC nv_min2_type           lIMSI_T_S2;         // nvisrv_imsi_t_s2
STATIC nv_imsi_mcc_type       lIMSI_T_MCC;        // nvisrv_imsi_t_mcc
STATIC nv_imsi_11_12_type     lIMSI_T_11_12;      // nvisrv_imsi_t_11_12
STATIC nv_imsi_addr_num_type  lIMSI_T_ADDR_NUM;   // nvisrv_imsi_t_addr_num
STATIC BOOL                   fIMSIInActive = FALSE;      // imsi_t_is_inactive
#endif /* FEATURE_IS95B_TRUE_IMSI */

STATIC UINT nSIDNID;


/* 
   hjcho 2000-11-26 --> This list includes items which are used in
   MSM3100 NAM programming. 
*/
#if (MODEL_ID == MODEL_B0)
STATIC NAM_ITEM_E  sMSM3100ITEMLST[EQS_NUM_NAMITEM_MSM3100]
= {
   PHONENO_I, 
   CDMA_PRICHNB_I, 
   CDMA_SECCHNB_I,                      
   SLOTIDX_I,
   SCM_I,
   CDMAHOMESID_I,
   PREFSYS_I
};
#endif /* (MODEL_ID == MODEL_B0) */
/* <-- */


/* 
   hjcho 2000-11-26 --> This list includes items which are used in
   MSM5000 NAM programming. 
*/
//#if (MODEL_ID == MODEL_B1)
//STATIC NAM_ITEM_E  sMSM5000ITEMLST[EQS_NUM_NAMITEM_MSM5000]
//= {
//   PHONENO_I,
//   CDMAHOMESIDNID_I,
//   CDMA_PRICHNA_I,
//   CDMA_PRICHNB_I,
//   CDMA_SECCHNA_I,
//   CDMA_SECCHNB_I,
//   PREFSYS_I,
//   CDMAMCC_I,
//   CDMAMNC_I,
//   PRL_ENABLED_I   
//};
//#endif /* (MODEL_ID == MODEL_B1) */
/* <-- */






/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
STATIC nv_item_type nvi,nvi2;      /* nv data */


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
STATIC  VOID sConvertMIN1_ASC(DWORD dwPhoneNum, CHAR* pszTXT);
STATIC  VOID sConvertMIN2_ASC(WORD wAreaCode, CHAR* pszTXT);
LOCAL   BOOL sWriteMIN1_MIN2_ACCOLCNumber(UINT nNAM, CHAR* pszNAMInfo);
VOID    sConvertMCC2ASC(WORD wMCC, CHAR* pszNAMInfo);
STATIC  WORD sConvertASC2MCC(CHAR* pszNAMInfo);
STATIC  DWORD sConvertASC2MIN1(CHAR* pszNAMInfo);
LOCAL   BOOL sIsValidData(BYTE pszBuff[]);



#ifdef FEATURE_MULTI_SID
/*
** ---------------------------------------------------------------------------
**     IRM2.0 spec.에 따라 사용자가 NAM3를 선택할 경우에는 다음과 같은 순서에 
**     따라 시스템을 선택한다.
**     - 단말기는 Acquisition Table에 따라 시스템을 탐색해서 PRL Table상에서 
**       SID가 0이 아닌 시스템을 1차적으로 탐색해서 시스템 접속을 시도한다.
**     - 1차 탐색에서 실패한 경우 단말기는 PRL Table의 SID가 0(wild SID)인 
**       시스템과 일치하는 시스템을 찾고 탐색한 결과가 1개 시스템인 경우에는
**       해당시스템으로 접속을 하고, 2개 이상인 경우에는 사용자가 특정 SID를
**       선택하는 메뉴를 제공해야 한다.
**     - 국제자동로밍용 NAM (NAM3)이 선택된 경우에 단말기는 메뉴상에 시스템 
**       탐색(혹은 SID탐색, 로밍사업자 선택) 메뉴를 추가해서 위의 절차를 사용자
**       선택에 의해서 반복적으로 수행할 수 있도록 해야 한다. 
** ----------------------------------------------------------------------------
*/


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sMakeMultiSIDList()
**
** Description: 
**     사용자가 NAM3을 선택했을 때 IRM2.0 spec.에 따라 메뉴상에 시스템 탐색
**     메뉴를 추가하기 위해 메뉴의 문자열을 생성하는 함수
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL BYTE** sMakeMultiSIDList(nv_multi_sid_type* pMultiSIDList)
{
    STATIC BYTE*    paSIDList[NV_MAX_CH_LIST_SIZE];
    STATIC BYTE     baSIDBuffer[NV_MAX_CH_LIST_SIZE][6];

    INT8            i;

    memset(paSIDList, NULL, sizeof(paSIDList));
    memset(baSIDBuffer, NULL, sizeof(baSIDBuffer));
    for( i = 0; i < pMultiSIDList->nSIDNum; i++)
    {
        sprintf((CHAR *)&baSIDBuffer[i][0], "%d", pMultiSIDList->awSIDList[i]); 
        paSIDList[i] = &baSIDBuffer[i][0];
    }                
    
    
    return (BYTE**)paSIDList;
}

        
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_ReceiveMulitSID()
**
** Description: 
**     Entrace
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_ReceiveMultiSID(SKY_EVENT_T *pEvent)
{
    STATIC nv_multi_sid_type    sMultiSIDBuf;
    HCONTROL                    hControl;
    BYTE**                      ppSIDList;

    SKY_TOKEN paPsedoPopMenu[]=
    {
        TKN_SID_1,  
        TKN_SID_2,  
        TKN_SID_3,
        TKN_SID_4
    };

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        SKY_GetNV(NV_MULTI_SID_I, (nv_item_type *)&sMultiSIDBuf);

        if(sMultiSIDBuf.nSIDNum == 0)
        {
            SKY_ERROR("Not Necessary come to MultiSID");
            EQS_END();
            break;
        }        

        ppSIDList = sMakeMultiSIDList(&sMultiSIDBuf);

        hControl = EQC_PopUpMenu((EQS_TOKEN_EX *)paPsedoPopMenu, sMultiSIDBuf.nSIDNum, 0);

        EQC_SetPopupMenuTitleString(hControl, (BYTE*)SKY_GET_TOKEN(TKN_RECEIVE_MULTI_SID));
        EQC_SetPopupStringItem(hControl, ppSIDList);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_POPUP_SELECT:
        sMultiSIDBuf.nSelIndex = pEvent->dwParam;
        SKY_PutNV(NV_MULTI_SID_I, (nv_item_type *)&sMultiSIDBuf);
        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SID_SAVED),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_POPUP_CANCEL:       

        ppSIDList = sMakeMultiSIDList(&sMultiSIDBuf);

        hControl = EQC_PopUpMenu((EQS_TOKEN_EX *)paPsedoPopMenu, sMultiSIDBuf.nSIDNum, 0);

        EQC_SetPopupMenuTitleString(hControl, (BYTE*)SKY_GET_TOKEN(TKN_RECEIVE_MULTI_SID));
        EQC_SetPopupStringItem(hControl, ppSIDList);
        break;

    case SE_IO_MESGBOX_END:
        ui_send_mc_cmd(MC_SELECTED_SID_F);
        EQS_END();

        /* jrkwon 2001.11.20 */
        if(SKY_IS_MAINTREQ() && (SKY_GET_PHONE_INFO_CURRENT_NAME() == CM_NAM_3))
            SKY_PowerOff(POM_RESET);
        break;

    case SE_KEY_END:
    case SE_KEY_PHONE_CLOSE:
    case SE_SOFTKEY_SMS:
        break;
   
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif //FEATURE_MULTI_SID





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     sMakeString
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sMakeString(CHAR* pcTarget, UINT nTotalStringLen, UINT nUsedLen)
{
   CHAR aszTmpBuf[EQS_MAX_TMP_BUF];
   UINT iLoop;
   UINT nOffset = nTotalStringLen - nUsedLen;

   for(iLoop = 0; iLoop < nUsedLen ; iLoop++)
   {
      aszTmpBuf[iLoop] = pcTarget[nOffset+iLoop];
   }
   aszTmpBuf[iLoop] = '\0';
   pcTarget[0]      = '\0';
   
   STRCPY((BYTE*) pcTarget, (BYTE*) aszTmpBuf);
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertASC2MIN1(CHAR* pazNAMInfo)
**
** Description: 
**     Convert 7 ASCII digits to a MIN1 type
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
STATIC DWORD sConvertASC2MIN1(CHAR* pszNAMInfo)
{
  DWORD dwPhoneNumber;
  INT   iLoop;
  
  for( iLoop=0; iLoop<7; iLoop++ )
  {
    if( pszNAMInfo[iLoop] == ' ' )
    {
      pszNAMInfo[iLoop] = '0';
    }
  }

  dwPhoneNumber = (DWORD) aMINTable2[pszNAMInfo[0] - '0']*100 +
                  (DWORD) aMINTable2[pszNAMInfo[1] - '0']*10 +
                  (DWORD) aMINTable2[pszNAMInfo[2] - '0'];
  dwPhoneNumber <<= 14;
  if( pszNAMInfo[3] == '0' )
  {
    dwPhoneNumber |= 10 << 10;
  }
  else
  {
    dwPhoneNumber |= ( (DWORD) ( (INT) pszNAMInfo[3]-'0' ) ) << 10;
  }
  dwPhoneNumber |= (DWORD) aMINTable2[pszNAMInfo[4] - '0' ]*100 +
                   (DWORD) aMINTable2[pszNAMInfo[5] - '0' ]*10 +
                   (DWORD) aMINTable2[pszNAMInfo[6] - '0' ];

  return dwPhoneNumber;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetDefaultIMSI_S1(VOID)                        //ui_default_imsi_s1()
**
** Description: 
**  This function computes the default IMSI_S1 value as per IS-683 section 3.1
**
** Input:
**     NONE  
**
** Output:
**     NONE
**
** Return value:
**     This function returns the encoded value of IMSI_S1 with the four least
**    significant digits set to ESNp, converted directly from binary to decimal,
**    modulo 10000.  The other digits are set to zero.
**
** Side effects:
**      Reading ESN from NV will change structure nvi.
**
** ---------------------------------------------------------------------------
*/
DWORD sSetDefaultIMSI_S1(VOID)
{
  WORD wZero;         /* Encoding of three zero digits */
  WORD wFourth;       /* Fourth from last decimal digit of the ESN */
  WORD wLast3Digit;   /* Last three decimal digits of the ESN */

  (VOID) SKY_GetNV( NV_ESN_I, &nvi );
  /* Encode digits as per JSTD-008 section 2.3.1.1 */
  wZero = 1110 - 111;
  wLast3Digit = (nvi.esn.esn % 1000);
  wLast3Digit += ((wLast3Digit / 100) == 0) ? 1000 : 0;
  wLast3Digit += (((wLast3Digit % 100) / 10) == 0) ? 100 : 0;
  wLast3Digit += ((wLast3Digit % 10) == 0) ? 10 : 0;
  wLast3Digit -= 111;
  wFourth = ((nvi.esn.esn % 10000) / 1000); /* In range 0-9 */
  if (wFourth == 0)
  {
    wFourth = 10;
  }

#ifdef FEATURE_UI_IMSI_ALL_0_OK
#error code not present
#else
  /* Concatenate results and return 24 bit value for imsi_s1 */
  /* Example: esn = 120406
  **      imsi_s1 = 000  0  406
  **  encodes to -> 999  10 395
  **       in hex = 3e7  a  18b
  **    in binary = 1111100111 1010 0110001011
  */
  return (((DWORD) wZero << 14) | (wFourth << 10) | wLast3Digit);
#endif
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     gInitializeIMSI()                    //ui_initialize_imsi_components
**
** Description: 
**  This function computes the default for all IMSI components, and then
**  writes them to NV. ACCOLC is also drived from the last digit of the
**  IMSI.
**    
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID gInitializeIMSI(UINT8 nNAM)
{
  BYTE bLastESNDigit;
  
  /* Default MCC is 000 in ASCII and 999 in min */
  nvi.imsi_mcc.nam = nNAM;
  nvi.imsi_mcc.imsi_mcc = IMSI_MCC_0;      /* default to '000' */
  (void) SKY_PutNV( NV_IMSI_MCC_I, &nvi );

  /* Default MNC is 00 in ASCII and 99 in min */
  nvi.imsi_11_12.nam = nNAM;
  nvi.imsi_11_12.imsi_11_12 = IMSI_11_12_0;  /* default to '00' */
  (void) SKY_PutNV( NV_IMSI_11_12_I, &nvi );

  nvi.imsi_addr_num.nam = nNAM;
  nvi.imsi_addr_num.num = IMSI_CLASS0_ADDR_NUM ;
  (void) SKY_PutNV( NV_IMSI_ADDR_NUM_I, &nvi ) ;

  nvi.min2.nam = nNAM;
  nvi.min2.min2[FMMIN] = IMSI_S2_0 ;
  nvi.min2.min2[CDMAMIN] = IMSI_S2_0 ;
  SKY_PutNV( NV_MIN2_I, &nvi ) ;

  nvi.min1.min1[CDMAMIN] =  sSetDefaultIMSI_S1();
  /* The following assignment must not be done, before the function above */
  /* because that function will read ESN which will corrupt the min1.nam. */
  nvi.min1.nam = nNAM;
  nvi.min1.min1[FMMIN] = IMSI_S1_0 ;
  SKY_PutNV( NV_MIN1_I, &nvi ) ;


  (void) SKY_GetNV( NV_ESN_I, &nvi ) ;
  bLastESNDigit  = (BYTE) (nvi.esn.esn % 10) ;
  nvi.accolc.nam = nNAM ;
  nvi.accolc.class[CDMAMIN] = bLastESNDigit;
  nvi.accolc.class[FMMIN] = bLastESNDigit;
  (void) SKY_PutNV( NV_ACCOLC_I, &nvi ) ;
}
  



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertASC2MNC()
**
** Description: 
**     Convert 2 ASCII digits to a MNC type
**
** Input:
**     CHAR* pszNAMInfo : Source Buffer 
**
** Output:
**     NONE
**
** Return value:
**     MNC format word number
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
WORD sConvertASC2MNC(CHAR* pszNAMInfo)
{
  WORD wMNC;
  UINT iLoop;

  for(iLoop = 0; iLoop < 2; iLoop++)
  {
    if(pszNAMInfo[iLoop] == ' ')
    {
      pszNAMInfo[iLoop] = '0';
    }
  }

  wMNC = aMINTable2[pszNAMInfo[0] - '0']*10 +
         aMINTable2[pszNAMInfo[1] - '0'];
  
  return wMNC;
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWriteNMSIDNumber(CHAR* pszNAMInfo)           //ui_write_NMSID_number
**
** Description:
**  This function encodes and writes the NMSID (National Mobile Station Identity)
**  number that is currently stored in serv3_txt_pointer to NV. 
**     
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     TRUE
**     FALSE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
BOOL sWriteNMSIDNumber(UINT nNAM, CHAR* pszNAMInfo)
{
  UINT  iLoop, nNumDigits;
  CHAR  szBuf[3];
  
#ifdef FEATURE_OTASP
  BOOL  fAllzeros ;
#endif

  nNumDigits = STRLEN((BYTE*)pszNAMInfo);

  if(nNumDigits < 4) return FALSE;

#ifdef FEATURE_OTASP
  fAllzeros = TRUE;
  for(iLoop = 1; iLoop <= nNumDigits; iLoop++)
  {
    if (pszNAMInfo[UI_WIDE - iLoop] != '0')
    {
      fAllzeros = FALSE;
      break;
    }
  }
#endif //FEATURE_OTASP
  
  /* get MCC from NV -- needed in order to compute min1, min2, imsi_11_12 */
  nvi.imsi_mcc.nam = nNAM;
  SKY_GetNV(NV_IMSI_MCC_I, &nvi);  


#ifdef FEATURE_OTASP
  /* If NMSID is all zeros and if MCC is all zeros, then we need to init */
  /* all IMSI components, using the special function which initialzes    */
  /* all components to zeros, except the last 4 digits of MIN1 are taken */
  /* from the last 4 digits of the ESN.                                  */
  if(fAllzeros == TRUE)
  {
     if(nvi.imsi_mcc.imsi_mcc == IMSI_MCC_0)
     {
       gInitializeIMSI(nNAM);              
       return TRUE;
     }
  }
#endif //FEATURE_OTASP

  sConvertMCC2ASC(nvi.imsi_mcc.imsi_mcc, szBuf);
  
  for(iLoop=1; iLoop<4 && iLoop+nNumDigits<= 12; iLoop++)
  {
    pszNAMInfo[UI_WIDE - nNumDigits -iLoop]  = szBuf[3-iLoop];    
  }


  /* compute and write imsi_addr_num */
  nvi.imsi_addr_num.nam = nNAM;
  nvi.imsi_addr_num.num = (nNumDigits == 12 ? IMSI_CLASS0_ADDR_NUM : nNumDigits - 4);
  if( SKY_PutNV( NV_IMSI_ADDR_NUM_I, &nvi ) != NV_DONE_S )
  {
    return FALSE;
  }
    
  /* compute and write min1,min2, ACCOLC */
  if (sWriteMIN1_MIN2_ACCOLCNumber(nNAM, &pszNAMInfo[UI_WIDE-10]) != TRUE)
  {
    return FALSE;
  }

  /* compute and write imsi_11_12 */
  nvi.imsi_11_12.nam = nNAM;
  nvi.imsi_11_12.imsi_11_12 = sConvertASC2MNC( (CHAR*)&pszNAMInfo[UI_WIDE-12] );
  if( SKY_PutNV( NV_IMSI_11_12_I, &nvi ) != NV_DONE_S )
  {
    return FALSE;
  }

  /* return 0, indicating that NMSID was written successfuly to NV. */
  return TRUE;
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWriteLockCode(CHAR* pszPhoneNumber)
**
** Description: 
**     Lock code is written from the last four digits of phone number
**
** Input:
**     pszPhoneNumber : Phone number string!!
**
** Output:
**     NONE
**
** Return value:
**     TRUE : Writing LOCK code success!!
**     FALSE: Writing LOCK code fail!!
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
BOOL sWriteLockCode(CHAR* pszPhoneNumber)
{
  INT  iLoop;
  UINT nStrLen = STRLEN((BYTE*)pszPhoneNumber);

  for(iLoop = 0; iLoop <= NV_LOCK_CODE_SIZE; iLoop++)
  {
    nvi.lock_code.digits[NV_LOCK_CODE_SIZE-iLoop] = 
                                         (BYTE) pszPhoneNumber[nStrLen-iLoop];
  }

  if(SKY_PutNV(NV_LOCK_CODE_I, &nvi) != NV_DONE_S)
  {
    return FALSE;
  }

  return TRUE;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL sGetValueFromDisp(CHAR* pszNAMInfo)
{
  if(STRCMP((BYTE*)pszNAMInfo, (BYTE*)"Yes"))      return TRUE;
  else if(STRCMP((BYTE*)pszNAMInfo, (BYTE*)"No"))  return FALSE;
  else                                             return FALSE;
}






/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSet017DefaultValue(UINT nNAM)
**
** Description: 
**     set default value of SID-NID, CDMA primary/secondary channel, 
**     preferred system, NAM name, Lock-Out SID, MCC, NMC, SCI, SCM
**
** Input:
**     nNAM : NAM identification number
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
#define SID_017_DEFAULT0		1700
#define SID_017_DEFAULT1        2222
#define SID_017_DEFAULT2        2236
#define SID_017_DEFAULT3        2221
#define SID_017_DEFAULT4        2220
#define SID_017_DEFAULT5        2226
#define SID_017_DEFAULT6        2238   
#define NID_017_DEFAULT			65535
#define MCC_017_DEFAULT			"450"
#define MNC_017_DEFAULT			"22"
#define SCI_017_DEFAULT			2
#define SCM_017_DEFAULT			42 // 0x2A
#define NAME_017_DEFAULT		"    POWER 017   "


LOCAL BOOL SKY_API sSet017DefaultValue(UINT nNAM)
{
	UINT iLoop;


    // Primary CDMA Channel
    nvi.pcdmach.channel_a = (WORD) PCDMACH_NAM2_DEFAULT;    /* 568 */
	nvi.pcdmach.channel_b = (WORD) PCDMACH_NAM2_DEFAULT_B;  /* 486 */
    if( SKY_PutNV( NV_PCDMACH_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }

    // Secondary CDMA Channel
    nvi.scdmach.channel_a = (WORD) SCDMACH_NAM2_DEFAULT;    /* 738 */
	nvi.scdmach.channel_b = (WORD) SCDMACH_NAM2_DEFAULT_B;  /* 779 */
    if( SKY_PutNV( NV_SCDMACH_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }

	// MCC
    nvi.imsi_mcc.imsi_mcc = sConvertASC2MCC((CHAR *)MCC_017_DEFAULT);
    //nvi.imsi_mcc.nam = nam;
    if( SKY_PutNV( NV_IMSI_MCC_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }

    // Mobile Station Network Code (MNC)
    nvi.imsi_11_12.imsi_11_12 = (BYTE)sConvertASC2MNC((CHAR *)MNC_017_DEFAULT);
    //nvi.imsi_11_12.nam = nam;
    if( SKY_PutNV( NV_IMSI_11_12_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }

	// SCM
    nvi.scm = (BYTE) SCM_017_DEFAULT;
    if( SKY_PutNV( NV_SCM_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }
   
	// SCI
    mccreg_clear_reg();   /* Clear out the reg sid nid */   
    nvi.slot_cycle_index = SCI_017_DEFAULT;
    if( SKY_PutNV( NV_SLOT_CYCLE_INDEX_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }


	// SID lock list, both CDMA and Analog
    for ( iLoop = 0; iLoop<NV_MAX_SID_LOCK; iLoop++ ) 
	{
		nvi.cdma_sid_lock.sid[iLoop] = 0 ;
    }

	// Preferred System 
	nvi.system_pref.nam = nNAM;
	nvi.system_pref.sys = NV_SP_STANDARD;
    ( void )SKY_PutNV( NV_SYSTEM_PREF_I, &nvi ); 
 	
	// NAM name
	nvi.name_nam.nam = nNAM;
	(void) memcpy((CHAR *)nvi.name_nam.name, (CHAR *)NAME_017_DEFAULT, EQS_NAME_LTRS);
	if (SKY_PutNV( NV_NAME_NAM_I, &nvi ) != NV_DONE_S )
	{
		return FALSE;
	}

    return TRUE;
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSet011DefaultValue(UINT nNAM)
**
** Description: 
**     set default value of SID-NID, CDMA primary/secondary channel, 
**     preferred system, NAM name, Lock-Out SID, MCC, NMC, SCI, SCM
**
** Input:
**     nNAM : NAM identification number
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
#define SID_011_DEFAULT0		2222
#define SID_011_DEFAULT1		2236
#define SID_011_DEFAULT2		2220
#define SID_011_DEFAULT3		2221
#define SID_011_DEFAULT4		2226
#define SID_011_DEFAULT5		2238
#define NID_011_DEFAULT			65535
#define PCDMACH_011_DEFAULT		779
#define PCDMACH_011_DEFAULT_B	779
#define SCDMACH_011_DEFAULT		738
#define SCDMACH_011_DEFAULT_B	738
#define MCC_011_DEFAULT			"450"
#define MNC_011_DEFAULT			"00"
#define SCI_011_DEFAULT			2
#define SCM_011_DEFAULT			42 // 0x2A
#define NAME_011_DEFAULT		"    SPEED 011   "



STATIC BOOL sSet011DefaultValue(UINT nNAM)
{
	UINT iLoop;

	nvi.name_nam.nam = nNAM;

    // Primary CDMA Channel
    nvi.pcdmach.channel_a = (WORD) PCDMACH_011_DEFAULT;
    nvi.pcdmach.channel_b = (WORD) PCDMACH_011_DEFAULT_B;
    if( SKY_PutNV( NV_PCDMACH_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }

    // Secondary CDMA Channel
    nvi.scdmach.channel_a =(WORD) SCDMACH_011_DEFAULT;
    nvi.scdmach.channel_b =(WORD) SCDMACH_011_DEFAULT_B;
    if( SKY_PutNV( NV_SCDMACH_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }

	// MCC
    nvi.imsi_mcc.imsi_mcc = sConvertASC2MCC((CHAR*)MCC_011_DEFAULT);
    
    if( SKY_PutNV( NV_IMSI_MCC_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }

    // Mobile Station Network Code
    nvi.imsi_11_12.imsi_11_12 = (BYTE)sConvertASC2MNC((CHAR *)MNC_011_DEFAULT);
    if( SKY_PutNV( NV_IMSI_11_12_I, &nvi ) != NV_DONE_S ) 
	{
	    return FALSE;
    }

	// SCM
    nvi.scm = (BYTE) SCM_011_DEFAULT;
    if( SKY_PutNV( NV_SCM_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }
   
	// SCI
    mccreg_clear_reg();   /* Clear out the reg sid nid */   
    nvi.slot_cycle_index = SCI_011_DEFAULT;
    if( SKY_PutNV( NV_SLOT_CYCLE_INDEX_I, &nvi ) != NV_DONE_S ) 
	{
		return FALSE;
    }


	// SID lock list, both CDMA and Analog
    for ( iLoop = 0; iLoop<NV_MAX_SID_LOCK; iLoop++ ) 
	{
		nvi.cdma_sid_lock.sid[iLoop] = 0 ;
    }

	// Preferred System 
	nvi.system_pref.nam = nNAM;
	nvi.system_pref.sys = NV_SP_STANDARD;
    ( void )SKY_PutNV( NV_SYSTEM_PREF_I, &nvi ); // preferred system isn't used
	// actually by SK Telecom. So, don't check the return value from SKY_PutNV()
 	
	// NAM name
	nvi.name_nam.nam = nNAM;
	(void) memcpy((CHAR *)nvi.name_nam.name, (CHAR *)NAME_011_DEFAULT, EQS_NAME_LTRS);
	if (SKY_PutNV( NV_NAME_NAM_I, &nvi ) != NV_DONE_S )
	{
		return FALSE;
	}

    return TRUE;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetNAMDefaultsValue(UINT8 nNAM)
**
** Description: 
**     NAM item(NAM3 ~ NAM5)들에 대한 값을 설정한다.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sSetNAMDefaultsValue(UINT8 nNAM)
{
	byte i;

	nvi.name_nam.nam = nNAM;


	// HOME SID - Store in both CDMA and AMPS */
    // Primary CDMA Channel
    switch(nNAM)
    {
        case NAM3:
            nvi.pcdmach.channel_a = (WORD) PCDMACH_NAM3_DEFAULT;
            nvi.pcdmach.channel_b = (WORD) PCDMACH_NAM3_DEFAULT_B;
            break;

        case NAM4:
            nvi.pcdmach.channel_a = (WORD) PCDMACH_NAM4_DEFAULT;
            nvi.pcdmach.channel_b = (WORD) PCDMACH_NAM4_DEFAULT_B;
            break;

        case NAM5:
            nvi.pcdmach.channel_a = (WORD) PCDMACH_NAM5_DEFAULT;
            nvi.pcdmach.channel_b = (WORD) PCDMACH_NAM5_DEFAULT_B;
            break;

        default:
            nvi.pcdmach.channel_a = (WORD) PCDMACH_011_DEFAULT;
            nvi.pcdmach.channel_b = (WORD) PCDMACH_011_DEFAULT_B;
            break;
    }

    if( SKY_PutNV(NV_PCDMACH_I, &nvi) != NV_DONE_S)
    {
        return;
    }

    
    // Secondary CDMA Channel
    switch(nNAM)
    {
        case NAM3:
            nvi.scdmach.channel_a =(WORD) SCDMACH_NAM3_DEFAULT;
            nvi.scdmach.channel_b =(WORD) SCDMACH_NAM3_DEFAULT_B;
            break;

        case NAM4:
            nvi.scdmach.channel_a =(WORD) SCDMACH_NAM4_DEFAULT;
            nvi.scdmach.channel_b =(WORD) SCDMACH_NAM4_DEFAULT_B;
            break;

        case NAM5:
            nvi.scdmach.channel_a =(WORD) SCDMACH_NAM5_DEFAULT;
            nvi.scdmach.channel_b =(WORD) SCDMACH_NAM5_DEFAULT_B;
            break;

        default:
            nvi.scdmach.channel_a =(WORD) SCDMACH_011_DEFAULT;
            nvi.scdmach.channel_b =(WORD) SCDMACH_011_DEFAULT_B;
            break;
    }

    if( SKY_PutNV( NV_SCDMACH_I, &nvi ) != NV_DONE_S ) 
	{
		return;
    }

	// MCC
    nvi.imsi_mcc.imsi_mcc = sConvertASC2MCC((CHAR*) MCC_011_DEFAULT);

    if( SKY_PutNV( NV_IMSI_MCC_I, &nvi ) != NV_DONE_S ) 
	{
		return;
    }

    // Mobile Station Network Code (MNC)
    switch(nNAM)
    {
        case NAM3:
        case NAM4:
        case NAM5:
        default:
            nvi.imsi_11_12.imsi_11_12 = (byte)sConvertASC2MNC((CHAR *)MNC_011_DEFAULT);
        break;
    }
   
    if( SKY_PutNV( NV_IMSI_11_12_I, &nvi ) != NV_DONE_S ) 
	{
		return;
    }

	// SCM
    nvi.scm = (byte) SCM_011_DEFAULT;
    if( SKY_PutNV( NV_SCM_I, &nvi ) != NV_DONE_S ) 
	{
		return;
    }
   
	// SCI
    mccreg_clear_reg();   /* Clear out the reg sid nid */
    nvi.slot_cycle_index = SCI_011_DEFAULT;
    if( SKY_PutNV( NV_SLOT_CYCLE_INDEX_I, &nvi ) != NV_DONE_S ) 
	{
		return;
    }

	// SID lock list, both CDMA and Analog
    for ( i = 0; i<NV_MAX_SID_LOCK; i++ ) 
	{
		nvi.cdma_sid_lock.sid[i] = 0 ;
    }

    // Preferred System 
	nvi.system_pref.nam = nNAM;
    switch(nNAM)
    {
        case NAM3:
            nvi.system_pref.sys = NV_SP_STANDARD;
            break;

        case NAM4:
            nvi.system_pref.sys = NV_SP_A_ONLY;
            break;

        case NAM5:
            nvi.system_pref.sys = NV_SP_B_ONLY;
            break;

        default:
            nvi.system_pref.sys = NV_SP_STANDARD;
            break;
    }
    ( void )SKY_PutNV( NV_SYSTEM_PREF_I, &nvi ); 

	// NAM name
	nvi.name_nam.nam = nNAM;
	(void) memcpy((char *)nvi.name_nam.name, (char *)NAME_011_DEFAULT, EQS_NAME_LTRS);
	if (SKY_PutNV( NV_NAME_NAM_I, &nvi ) != NV_DONE_S )
	{
		return;
	}
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetDefaultValue(UINT nNAM)
**
** Description: 
**     set default value of SID-NID, CDMA primary/secondary channel, 
**     preferred system, NAM name, Lock-Out SID, MCC, NMC, SCI, SCM
**
** Input:
**     nNAM : NAM identification number
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
LOCAL BOOL SKY_API sSetDefaultValue(UINT8 nNAM)
{
    UINT iLoop;
    CHAR pszTmpBuf[EQS_SIZE_NAM_BUFFER];

    /* get min2 */
	nvi.min2.nam = nNAM;
	(void) SKY_GetNV( NV_MIN2_I, &nvi );
	sConvertMIN2_ASC(nvi.min2.min2[CDMAMIN], pszTmpBuf);

    switch(nNAM)
    {
      case NAM1:
      case NAM2:
	    if((0 == STRNCMP((BYTE*)"011", (BYTE*)pszTmpBuf, 3)) || 
           (0 == STRNCMP((BYTE*)"11",  (BYTE*)pszTmpBuf, 2)))
        {
            sSet011DefaultValue(nNAM);
        }	
		else if(0 == STRNCMP((BYTE*)"017", (BYTE*)pszTmpBuf, 3)  || 
           (0 == STRNCMP((BYTE*)"17", (BYTE*)pszTmpBuf, 2)))
        {
	        sSet017DefaultValue(nNAM);
        }
        else
        {
            sSetNAMDefaultsValue(nNAM);
        }
        break;
	
      case NAM3:
      case NAM4:
      case NAM5:    
	  default:
		  sSetNAMDefaultsValue(nNAM);
          /* NAM3,4,5에 입력되어 있는 번호가 017일 경우에는 MNC를 22로 설정한다. */
          if((0 == STRNCMP((BYTE*)"017", (BYTE*)pszTmpBuf, 3))  || 
             (0 == STRNCMP((BYTE*)"17", (BYTE*)pszTmpBuf, 2)))
          {
              nvi.imsi_11_12.imsi_11_12 = (byte)sConvertASC2MNC((CHAR *)MNC_017_DEFAULT);
              SKY_PutNV( NV_IMSI_11_12_I, &nvi );
          }
        break;
    }

    nvi.mob_term_home.nam = nNAM;
    (void) SKY_GetNV( NV_MOB_TERM_HOME_I, &nvi );
    for( iLoop=0; iLoop<NV_MAX_MINS; iLoop++ ) 
    {
        /* default for Home System Registration is yes */
        nvi.mob_term_home.enabled[iLoop] = TRUE;
    }
    (void) SKY_PutNV( NV_MOB_TERM_HOME_I, &nvi );

    nvi.mob_term_for_sid.nam = nNAM;
    (void) SKY_GetNV( NV_MOB_TERM_FOR_SID_I, &nvi );
    for( iLoop=0; iLoop<NV_MAX_MINS; iLoop++ ) 
    {
        /* default for Foreign SID Registration is yes */
        nvi.mob_term_for_sid.enabled[iLoop] = TRUE;
    }
    (void) SKY_PutNV( NV_MOB_TERM_FOR_SID_I, &nvi );

    nvi.mob_term_for_nid.nam = nNAM;
    (void) SKY_GetNV( NV_MOB_TERM_FOR_NID_I, &nvi );
    for( iLoop=0; iLoop<NV_MAX_MINS; iLoop++ ) 
    {
        /* default for Foreign NID Registration is yes */
        nvi.mob_term_for_nid.enabled[iLoop] = TRUE;
    }
    (void) SKY_PutNV( NV_MOB_TERM_FOR_NID_I, &nvi );

    nvi.home_sid_nid.nam = nNAM;
    (void) SKY_GetNV( NV_HOME_SID_NID_I, &nvi );
    
    /*  SID, NID default */

    if (nNAM == NAM1)
    {
        for (iLoop = 0; iLoop < NV_MAX_HOME_SID_NID; iLoop++)
        {
            nvi.home_sid_nid.pair[iLoop].sid = SID_011_DEFAULT0;
            nvi.home_sid_nid.pair[iLoop].nid = NID_011_DEFAULT;
        }

        nvi.home_sid_nid.pair[0].sid = SID_011_DEFAULT0;
        nvi.home_sid_nid.pair[0].nid = NID_011_DEFAULT;
        nvi.home_sid_nid.pair[1].sid = SID_011_DEFAULT1;
        nvi.home_sid_nid.pair[1].nid = NID_011_DEFAULT;
        nvi.home_sid_nid.pair[2].sid = SID_011_DEFAULT2;
        nvi.home_sid_nid.pair[2].nid = NID_011_DEFAULT;
        nvi.home_sid_nid.pair[3].sid = SID_011_DEFAULT3;
        nvi.home_sid_nid.pair[3].nid = NID_011_DEFAULT;
        nvi.home_sid_nid.pair[4].sid = SID_011_DEFAULT4;
        nvi.home_sid_nid.pair[4].nid = NID_011_DEFAULT;
        nvi.home_sid_nid.pair[5].sid = SID_011_DEFAULT5;
        nvi.home_sid_nid.pair[5].nid = NID_011_DEFAULT;
        nvi.home_sid_nid.pair[6].sid = SID_017_DEFAULT6;
        nvi.home_sid_nid.pair[6].nid = NID_017_DEFAULT;
    }
    else if (nNAM == NAM2)
	{
        for (iLoop = 0; iLoop < NV_MAX_HOME_SID_NID; iLoop++)
        {
            nvi.home_sid_nid.pair[iLoop].sid = SID_017_DEFAULT0;
            nvi.home_sid_nid.pair[iLoop].nid = NID_017_DEFAULT;
        }

        nvi.home_sid_nid.pair[0].sid = SID_017_DEFAULT0;
        nvi.home_sid_nid.pair[0].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[1].sid = SID_017_DEFAULT1;
        nvi.home_sid_nid.pair[1].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[2].sid = SID_017_DEFAULT2;
        nvi.home_sid_nid.pair[2].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[3].sid = SID_017_DEFAULT3;
        nvi.home_sid_nid.pair[3].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[4].sid = SID_017_DEFAULT4;
        nvi.home_sid_nid.pair[4].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[5].sid = SID_017_DEFAULT5;
        nvi.home_sid_nid.pair[5].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[6].sid = SID_017_DEFAULT6;
        nvi.home_sid_nid.pair[6].nid = NID_017_DEFAULT;
    }
	else if (nNAM == NAM3)
	{
        for (iLoop = 0; iLoop < NV_MAX_HOME_SID_NID; iLoop++)
        {
            nvi.home_sid_nid.pair[iLoop].sid = SID_017_DEFAULT0;
            nvi.home_sid_nid.pair[iLoop].nid = NID_017_DEFAULT;
        }

        nvi.home_sid_nid.pair[0].sid = SID_017_DEFAULT1; /* 2222 */
        nvi.home_sid_nid.pair[0].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[1].sid = SID_017_DEFAULT2; /* 2236 */
        nvi.home_sid_nid.pair[1].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[2].sid = SID_017_DEFAULT0; /* 1700 */
        nvi.home_sid_nid.pair[2].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[3].sid = SID_017_DEFAULT3; /* 2221 */
        nvi.home_sid_nid.pair[3].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[4].sid = SID_017_DEFAULT4; /* 2220 */
        nvi.home_sid_nid.pair[4].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[5].sid = SID_017_DEFAULT5; /* 2226 */
        nvi.home_sid_nid.pair[5].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[6].sid = SID_017_DEFAULT6; /* 2238 */
        nvi.home_sid_nid.pair[6].nid = NID_017_DEFAULT;
    }    
	else if (nNAM == NAM4)
	{
        for (iLoop = 0; iLoop < NV_MAX_HOME_SID_NID; iLoop++)
        {
            nvi.home_sid_nid.pair[iLoop].sid = SID_017_DEFAULT0;
            nvi.home_sid_nid.pair[iLoop].nid = NID_017_DEFAULT;
        }

        nvi.home_sid_nid.pair[0].sid = SID_017_DEFAULT1;
        nvi.home_sid_nid.pair[0].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[1].sid = SID_017_DEFAULT2;
        nvi.home_sid_nid.pair[1].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[2].sid = SID_017_DEFAULT0;
        nvi.home_sid_nid.pair[2].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[3].sid = SID_017_DEFAULT3;
        nvi.home_sid_nid.pair[3].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[4].sid = SID_017_DEFAULT4;
        nvi.home_sid_nid.pair[4].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[5].sid = SID_017_DEFAULT5;
        nvi.home_sid_nid.pair[5].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[6].sid = SID_017_DEFAULT6; /* 2238 */
        nvi.home_sid_nid.pair[6].nid = NID_017_DEFAULT;
	}
	else if (nNAM == NAM5)
	{
        for (iLoop = 0; iLoop < NV_MAX_HOME_SID_NID; iLoop++)
        {
            nvi.home_sid_nid.pair[iLoop].sid = SID_017_DEFAULT0;
            nvi.home_sid_nid.pair[iLoop].nid = NID_017_DEFAULT;
        }

        nvi.home_sid_nid.pair[0].sid = SID_017_DEFAULT1;
        nvi.home_sid_nid.pair[0].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[1].sid = SID_017_DEFAULT3;
        nvi.home_sid_nid.pair[1].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[2].sid = SID_017_DEFAULT2;
        nvi.home_sid_nid.pair[2].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[3].sid = SID_017_DEFAULT0;
        nvi.home_sid_nid.pair[3].nid = NID_017_DEFAULT ;
        nvi.home_sid_nid.pair[4].sid = SID_017_DEFAULT4;
        nvi.home_sid_nid.pair[4].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[5].sid = SID_017_DEFAULT5;
        nvi.home_sid_nid.pair[5].nid = NID_017_DEFAULT;
        nvi.home_sid_nid.pair[6].sid = SID_017_DEFAULT6; /* 2238 */
        nvi.home_sid_nid.pair[6].nid = NID_017_DEFAULT;
    }
    if(SKY_PutNV(NV_HOME_SID_NID_I, &nvi) != NV_DONE_S)
    {
        SKY_PutNV(NV_HOME_SID_NID_I, &nvi);
    }

    switch(nNAM)
    {
        case NAM1:
        case NAM2:
        case NAM3:
	        nvi.prl_enabled.enabled = TRUE;
            break;

        case NAM4:
        case NAM5:
        default:
            nvi.prl_enabled.enabled = FALSE;
            break;
    }
    (void) SKY_PutNV( NV_PRL_ENABLED_I, &nvi );

	return TRUE;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertASC2MIN2
**
** Description: 
**     Convert 3 ASCII type string to a MIN2 type 
**
** Input:
**     pszPhoneNumber : It has a phone number as ASCII type.
**
** Output:
**     NONE
**
** Return value:
**     A value which is converted from input string.
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
WORD sConvertASC2MIN2(CHAR* pszPhoneNumber)
{
  INT  iLoop;
  WORD wMIN2;

  for(iLoop = 0; iLoop < 3; iLoop++)
  {
	  if(pszPhoneNumber[iLoop] == ' ')
	  {
		  pszPhoneNumber[iLoop] = 0;
	  }
  }

  wMIN2 = aMINTable2[pszPhoneNumber[0] - '0']*100 +
	      aMINTable2[pszPhoneNumber[1] - '0']*10  +
		  aMINTable2[pszPhoneNumber[2] - '0'];

  return wMIN2;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWriteMIN1_MIN2_ACCOLCNumber(UINT nNAM, CHAR* pszNAMInfo)
**
** Description: 
**     This function encodes and writes the MIN1, MIN2, and ACCOLC numbers
**     that are currently stored txt_ptr[] to NV.
**
** Input:
**     nNAM : the number of NAM
**     pszNAMInfo : a string which has NAM information.
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If succeed in writing all MIN1, MIN2, ACCOLC numbers.
**     FALSE: If fail in writing all MIN1, MIN2, ACCOLC numbers.
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
LOCAL BOOL sWriteMIN1_MIN2_ACCOLCNumber(UINT nNAM, CHAR* pszNAMInfo)
{
  /* compute and write min2 */
  /* start by reading FMMIN because it will be over written when CDMAMIN
  ** is written to NV */	
  nvi.min2.nam = nNAM;
  (void) SKY_GetNV(NV_MIN2_I, &nvi);
  nvi.min2.min2[CDMAMIN] = sConvertASC2MIN2(pszNAMInfo);
  if(SKY_PutNV(NV_MIN2_I, &nvi) != NV_DONE_S)
  {
	  if(SKY_PutNV(NV_MIN2_I, &nvi) != NV_DONE_S)
      {
        return FALSE;
      }
  }

  /* compute and write min1 */
  /* start by reading FMMIN because it will be over written when CDMAMIN
  ** is written to NV */
  nvi.min1.nam = nNAM;
  (void) SKY_GetNV(NV_MIN1_I, &nvi);
  nvi.min1.min1[CDMAMIN] = sConvertASC2MIN1(&(pszNAMInfo[3]));
  if(SKY_PutNV(NV_MIN1_I, &nvi) != NV_DONE_S)
  {
	  if(SKY_PutNV(NV_MIN1_I, &nvi) != NV_DONE_S)
      {
	    return FALSE;
      }
  }

  /* overwrite ACCOLC (Access Overload Class) with LSD digit only
  ** if ACCOLC is less than 10 ( 0x A ) or greater than 15 ( 0x F ) */
  nvi.accolc.nam = nNAM;
  (void) SKY_GetNV(NV_ACCOLC_I, &nvi); /* Read Analog ACCOLC */

  if(nvi.accolc.class[CDMAMIN] < 10 || nvi.accolc.class[CDMAMIN] > 15)
  {
    nvi.accolc.class[CDMAMIN] = pszNAMInfo[9] - '0';
	if(SKY_PutNV(NV_ACCOLC_I, &nvi) != NV_DONE_S)
	{
      if(SKY_PutNV(NV_ACCOLC_I, &nvi) != NV_DONE_S)
      {
        return FALSE;
      }
	}
  }

  return TRUE;
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWriteStationID(UINT nNAM, CHAR* pszNAMInfo) //UI_WRITE_STATION_ID
**
** Description: 
**     Write station ID to NV
**
** Input:
**     nNAM : NAM identifier
**     pszNAMInfo : a string which has NAM information string!!
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If writing succeed.
**     FALSE: If writing fail.
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
LOCAL BOOL sWriteStationID(UINT nNAM, CHAR* pszNAMInfo)
{
  if(STRLEN((BYTE*) pszNAMInfo) == 10)
  {
	  if(sWriteMIN1_MIN2_ACCOLCNumber(nNAM, pszNAMInfo))
	  {
		  return TRUE;
	  }
      else 
		  return FALSE;
  }
  else
  {
	  return FALSE;
  }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWriteCDMANumber(CHAR* pszPhoneNumber, UINT8 nNAM)
**
** Description: 
**     Writes the CDMA number, using the number in pszPhoneNumber CHAR pointer.
**
** Input:
**     pszPhoneNumber : 
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If succeed in writing phone number.
**     FALSE: If fail in writing phone number.
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
LOCAL BOOL SKY_API sWriteCDMANumber(CHAR* pszPhoneNumber, UINT8 nNAM)
{
  BYTE     pszTmpBuf[11];
  UINT8    iLoop;
    
  nvi.dir_number.nam = nNAM;
  if(STRLEN((BYTE*)pszPhoneNumber) == 10)     /* 입력된 전화번호가 10자리일 경우 */
  {                                           /*      011XXXYYYY, 11XXXXYYYY     */
      (void) memcpy((void*) nvi.dir_number.dir_number,
		                                   pszPhoneNumber, 
										   NV_DIR_NUMB_SIZ);
	  if(SKY_PutNV(NV_DIR_NUMBER_I, &nvi) != NV_DONE_S)
	  {
		  if(SKY_PutNV(NV_DIR_NUMBER_I, &nvi) != NV_DONE_S)
          {
            return FALSE;
          }
	  }
	  else
	  {
		  return TRUE;
	  }
  }
  else if(STRLEN((BYTE*)pszPhoneNumber) == 11) /* 입력된 전화번호가 11자리일 경우 */
  {                                            /*     011XXXXYYYY, 017XXXXYYYY    */
	  for(iLoop = 0; pszPhoneNumber[iLoop+1] != '\0'; iLoop++)
      {
          pszTmpBuf[iLoop] = pszPhoneNumber[iLoop+1];
      }
      pszTmpBuf[iLoop] = '\0';

      (void) memcpy((void*) nvi.dir_number.dir_number, pszTmpBuf, NV_DIR_NUMB_SIZ);
      (void) STRCPY((BYTE*)pszPhoneNumber, (BYTE*)pszTmpBuf);

      if(SKY_PutNV(NV_DIR_NUMBER_I, &nvi) != NV_DONE_S)
      {
         if(SKY_PutNV(NV_DIR_NUMBER_I, &nvi) != NV_DONE_S)
         {
            return FALSE;
         }
      }
      else
      {
          return TRUE;
      }
  }
  else
  {
      return FALSE;
  }
} 




//#ifdef T_ACP 
#ifdef FEATURE_ACP
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWriteAMPSNumber(UINT nNAM, CHAR* pszPhoneNumber)
**
** Description: 
**     Write pszPhoneNumber to AMPS NV
**
** Input:
**     nNAM : NAM identification number
**     pszPhoneNumber : A string which has phone number.
**
** Output:
**     NONE
**
** Return value:
**     TRUE  : If writing succeed.
**     FALSE : If writing false.
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
STATIC BOOL sWriteAMPSNumber(UINT nNAM, CHAR* pszPhoneNumber)
{
  STATIC WORD  wValue;
  STATIC DWORD dwValue;

  if(STRLEN((BYTE*)pszPhoneNumber) >= EQS_MIN_PHONE_NUM)
  {
	/* read CDMAMIN because it will be written together with FMMIN */
    nvi.min2.nam = nNAM;
    (void) SKY_GetNV( NV_MIN2_I, &nvi );
    /* first take the area code */
	wValue = sConvertASC2MIN2(pszPhoneNumber); 

	nvi.min2.min2[FMMIN] = min2.min2[FMMIN] = wValue;
    if( SKY_PutNV( NV_MIN2_I, &nvi ) != NV_DONE_S )
	{
      return FALSE;
    }

    nvi.min1.nam = nNAM;
	(void) SKY_GetNV( NV_MIN1_I, &nvi );

	dwValue = sConvertASC2MIN1(pszPhoneNumber);
	nvi.min1.min1[FMMIN] = min1.min1[FMMIN] = dwValue;

	if( SKY_PutNV( NV_MIN1_I, &nvi ) != NV_DONE_S ) 
	{
      return FALSE;
    }
	else
	{
//      I don't know what to do only with this code!!
//      I must repair this part some times!!!
//      Set AMPS number as CDMA number.
//
//      nvi.accolc.nam = nNAM;
//      /* Both ACCOLC's will be written, so read the CDMA value */
//      (void) SKY_GetNV( NV_ACCOLC_I, &nvi );
//      nvi.accolc.class[FMMIN] = pszPhoneNumber
//#ifdef TREX_UI
//                (byte) ui_svc_screen[UI_SERV3_I][UI_WIDE-2] - '0';
//#else
//				(byte) ui_svc_screen[UI_SERV3_I][UI_WIDE-1] - '0';
//#endif
//            if( SKY_PutNV( NV_ACCOLC_I, &nvi ) != NV_DONE_S ) {
//              error_code = 99;
     }
    return TRUE;
  }
  else return FALSE;
}
#endif// FEATURE_ACP
//#endif /* ifdef T_ACP */




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWritePhoneNumberAMPSandCDMA(UINT nNAM, CHAR* pszPhoneNumber)
**
** Description: 
**     Write pszPhoneNumber to AMPS and CDMA phone number NV items
**
** Input:
**     nNAM : NAM identification number.
**     pszPhoneNumber : A Phone number which is to be saved in AMPS and CDMA.
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If succeed in writing AMPS and CDMA phone number.
**     FALSE: If do not succeed in writing AMPS and CDMA phoen number.
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
STATIC BOOL sWritePhoneNumberAMPSandCDMA(UINT nNAM, CHAR* pszPhoneNumber)
{
#ifdef FEATURE_ACP
	if(sWriteAMPSNumber(nNAM, pszPhoneNumber) == TRUE)
	{
		if(sWriteCDMANumber(pszPhoneNumber, nNAM) == TRUE)  
			return TRUE;    // succeed in writing both AMPS and CDMA number
		else
			return FALSE;   // fail in writing CDMA number
	}
	else
	{
		return FALSE;       // Error in writing AMPS number
	}
#else  // FEATURE_ACP
    return TRUE;
#endif // FEATURE_ACP
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sIsDigit(CHAR cDigit)
**
** Description: 
**     Return TRUE if given character is decimal digit.
**
** Input:
**     cDigit : Phonenumber string.
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If the given character is decimal digit.
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
STATIC BOOL sIsDigit(CHAR cDigit)
{
	if((cDigit >= '0') && (cDigit <= '9'))
		return TRUE;
	else
		return FALSE;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sIsPhoneNumber(CHAR* pszPhoneNumber)
**
** Description: 
**     Retrun TRUE, if a string pszPhoneNumber has a valid phone number string.
**
** Input:
**     CHAR* pszPhoneNumber : phone number string
**
** Output:
**     NONE
**
** Return value:
**    TRUE : If the input string has a valid phone number.
**    FALSE: If the input string doesn't have a valid phone number.
**
** Side effects:
**    NONE
**
** ---------------------------------------------------------------------------
*/
STATIC BOOL sIsPhoneNumber(CHAR* pszPhoneNumber)
{
  UINT iLoop;

  for(iLoop = 0; iLoop < 10; iLoop++)
  {
	  if(!sIsDigit(pszPhoneNumber[iLoop])) return FALSE;
  }

  return TRUE;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sIsValidMIN(UINT nDigits, CHAR* pszPhoneNumber)
**
** Description: 
**     Return FALSE if the given string is valid directory phone number and
**     the format of 011xxxxxxx, 11xxxxxxxx
**
** Input:
**     nDigits : # of phone number digits
**     pszPhoneNumber : a string which contains phone number.
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If the phone number which is in pszPhoneNumber string is 011
**     FALSE: If the phone number which is in pszPhoneNumber string is not 011
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
#define sMIN_LENGTH      10

STATIC BOOL sIsValidMIN(UINT nDigits, CHAR* pszPhoneNumber)
{  
  BYTE l_TmpPhoneNumber[12];

  if(nDigits == 10)
  {
      memcpy(l_TmpPhoneNumber, &(pszPhoneNumber[0]), sMIN_LENGTH);
      l_TmpPhoneNumber[nDigits] = '\0';
  }
  else if(nDigits == 11)
  {
      memcpy(l_TmpPhoneNumber, &(pszPhoneNumber[1]), sMIN_LENGTH);
      l_TmpPhoneNumber[(nDigits-1)] = '\0';
  }
  else
  {
      return FALSE;
  }
  
  if(sIsPhoneNumber((CHAR*)l_TmpPhoneNumber) == FALSE) return FALSE;


  return TRUE;
  

//  if(l_TmpPhoneNumber[0] == '0') 
//  {
//      return TRUE;
//  }
//  else if(l_TmpPhoneNumber[0] == '1')
//  {
//      return TRUE;
//  }
//  else
//  {
//      return FALSE;
//  }    

#if 0//00000000000000000000000000000000000000000000000000000
//  if(sIsPhoneNumber(pszPhoneNumber) == FALSE) return FALSE;
//	
//  if(nDigits == 11)      /* 011-xxxx-yyyy */
//  {
//     if(pszPhoneNumber[1] == '1')   //(pszPhoneNumber[0] == '0') && (pszPhoneNumber[1] == '1'))
//                                    //&& (pszPhoneNumber[2] == '1'))
//		 return TRUE;
//	 else
//		 return FALSE;
//  }
//  else if(nDigits == 10) /* 011-xxx-yyyy or 11xxxxyyyy*/
//  {
//	  if((pszPhoneNumber[0] == '0') //&& (pszPhoneNumber[1] == '1')
//      {	                            //&& (pszPhoneNumber[2] == '1'))
//	     return TRUE;
//      }
//      else if(pszPhoneNumber[0] == '1') //&& (pszPhoneNumber[1] == '1'))
//      {
//         return TRUE;
//      } 
//	  else
//      {
//		 return FALSE;  
//      }
//  }
//  else return FALSE;       //Wrong Digit !!!
#endif //00000000000000000000000000000000000000000000000000000
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sIs011(UINT nDigits, CHAR* pszPhoneNumber)
**
** Description: 
**     Return FALSE if the given string is valid directory phone number and
**     the format of 011xxxxxxx, 11xxxxxxxx
**
** Input:
**     nDigits : # of phone number digits
**     pszPhoneNumber : a string which contains phone number.
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If the phone number which is in pszPhoneNumber string is 011
**     FALSE: If the phone number which is in pszPhoneNumber string is not 011
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
STATIC BOOL sIs011(UINT nDigits, CHAR* pszPhoneNumber)
{
  
  if(sIsPhoneNumber(pszPhoneNumber) == FALSE) return FALSE;
	
  if(nDigits == 11)      /* 011-xxxx-yyyy */
  {
     if((pszPhoneNumber[0] == '0') && (pszPhoneNumber[1] == '1')
                                   && (pszPhoneNumber[2] == '1'))
		 return TRUE;
	 else
		 return FALSE;
  }
  else if(nDigits == 10) /* 011-xxx-yyyy or 11xxxxyyyy*/
  {
	  if((pszPhoneNumber[0] == '0') && (pszPhoneNumber[1] == '1')
		                            && (pszPhoneNumber[2] == '1'))
	     return TRUE;

      else if((pszPhoneNumber[0] == '1') && (pszPhoneNumber[1] == '1'))
         return TRUE;
 
	  else
		 return FALSE;  
  }
  else return FALSE;       //Wrong Digit !!!
}


//#ifdef FEATURE_017_NAM
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sIs017(UINT nDigits, CHAR* pszPhoneNumber)
**
** Description: 
**     Return FALSE if the given string is valid directory phone number and
**     the format of 017xxxxxxx, 17xxxxxxxx
**
** Input:
**     nDigits : # of phone number digits
**     pszPhoneNumber : a string which contains phone number.
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If the phone number which is in pszPhoneNumber string is 017
**     FALSE: If the phone number which is in pszPhoneNumber string is not 017
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
STATIC BOOL sIs017(UINT nDigits, CHAR* pszPhoneNumber)
{
  
  if(sIsPhoneNumber(pszPhoneNumber) == FALSE) return FALSE;
	
  if(nDigits == 11)
  {
     if( (pszPhoneNumber[0] == '0') && 
         (pszPhoneNumber[1] == '1') && 
         (pszPhoneNumber[2] == '7'))
		 return TRUE;
	 else
		 return FALSE;
  }
  else if(nDigits == 10)
  {
	  if((pszPhoneNumber[0] == '0') && 
         (pszPhoneNumber[1] == '1') && 
         (pszPhoneNumber[2] == '7'))
	     return TRUE;      
      else if((pszPhoneNumber[0] == '1') && (pszPhoneNumber[1] == '7'))
         return TRUE;
	  else
		 return FALSE;  
  }
  else return FALSE;       //Wrong Digit !!!
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sIs010(UINT nDigits, CHAR* pszPhoneNumber)
**
** Description: 
**     Return FALSE if the given string is valid directory phone number and
**     the format of 017xxxxxxx, 17xxxxxxxx
**
** Input:
**     nDigits : # of phone number digits
**     pszPhoneNumber : a string which contains phone number.
**
** Output:
**     NONE
**
** Return value:
**     TRUE : If the phone number which is in pszPhoneNumber string is 017
**     FALSE: If the phone number which is in pszPhoneNumber string is not 017
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
STATIC BOOL sIs010(UINT nDigits, CHAR* pszPhoneNumber)
{
  
  if(sIsPhoneNumber(pszPhoneNumber) == FALSE) return FALSE;
	
  if(nDigits == 11)
  {
     if( (pszPhoneNumber[0] == '0') && 
         (pszPhoneNumber[1] == '1') && 
         (pszPhoneNumber[2] == '0'))
		 return TRUE;
	 else
		 return FALSE;
  }
  else if(nDigits == 10)
  {
	  if((pszPhoneNumber[0] == '0') && 
         (pszPhoneNumber[1] == '1') && 
         (pszPhoneNumber[2] == '0'))
	     return TRUE;      
      else if((pszPhoneNumber[0] == '1') && (pszPhoneNumber[1] == '0'))
         return TRUE;
	  else
		 return FALSE;  
  }
  else return FALSE;       //Wrong Digit !!!
}




void sInitBuf(CHAR *pszNAMInfo, UINT8 nSize)
{
   memset(pszNAMInfo,' ',nSize);
   pszNAMInfo[0] ='\0';  
}
      

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertASC2DEC()                             //fromval()
**
** Description: 
**     Return the numerical value of whatever is in a given argument string
**
** Input:
**     pszNAMInfo
**
** Output:
**     NONE
**
** Return value:
**     DWORD value which is converted from a input argument string.
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
STATIC DWORD sConvertASC2DEC(CHAR* pszNAMInfo)
{
  DWORD dwReturnVal;
  INT   iLoop;

  dwReturnVal = 0;

  for( iLoop = 0; pszNAMInfo[iLoop] != '\0'; iLoop++)
  {
    if(pszNAMInfo[iLoop] != '\0')
	{
		dwReturnVal = dwReturnVal*10 + (DWORD)(pszNAMInfo[iLoop] - '0');
	}
  }

  return dwReturnVal;
}






/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertMNC2ASC()                                   mnc_asc
**
** Description: 
**     Convert a MNC type number to ASCII     
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sConvertMNC2ASC(WORD wMNC, CHAR* pszNAMInfo)
{
  *pszNAMInfo++ = aMINTable1[wMNC/10];
  *pszNAMInfo++ = aMINTable1[wMNC%10];
  *pszNAMInfo   = '\0';
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()   //get_nmsid_into_temp_buffer
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID sGetNMSIDintoTmpBuf(BYTE nNAM, CHAR* pszTmpBuf)
{
  CHAR   aBuf[3];
  
//#if (defined(FEATURE_CDMA_1900) || defined(FEATURE_IS95B_TRUE_IMSI))  
  UINT8  iLoop;
//#endif
  
   /* get min2 */
  nvi.min2.nam = nNAM;
  (VOID) SKY_GetNV(NV_MIN2_I, &nvi);
  sConvertMIN2_ASC(nvi.min2.min2[CDMAMIN], (CHAR*)&pszTmpBuf[UI_WIDE-10]);

  /* get min1 */
  nvi.min1.nam = nNAM;
  (VOID) SKY_GetNV(NV_MIN1_I, &nvi);
  if(nvi.min1.min1[CDMAMIN] == 0)
  {
    (VOID) memcpy(pszTmpBuf, " ", UI_WIDE);
    return;
  }

  sConvertMIN1_ASC(nvi.min1.min1[CDMAMIN], (CHAR*)&pszTmpBuf[UI_WIDE-7]);

  /* get imsi_11_12 */
  nvi.imsi_11_12.nam = nNAM;
  SKY_GetNV(NV_IMSI_11_12_I, &nvi);
  sConvertMNC2ASC(nvi.imsi_11_12.imsi_11_12, (CHAR*)pszTmpBuf);

  /* get mcc */
  nvi.imsi_mcc.nam = nNAM;
  SKY_GetNV(NV_IMSI_MCC_I, &nvi);
  sConvertMCC2ASC(nvi.imsi_mcc.imsi_mcc, aBuf);

//HDTEMPO#if (defined(FEATURE_CDMA_1900) || defined(FEATURE_IS95B_TRUE_IMSI))
  /* get addr_num ( = NMSID length - 4) */
  nvi.imsi_addr_num.nam = nNAM;
  (VOID) SKY_GetNV(NV_IMSI_ADDR_NUM_I, &nvi);

  /* if IMSI is class 0 return now */
  if ( nvi.imsi_addr_num.num == IMSI_CLASS0_ADDR_NUM )
  {
    return;
  }

  /* IMSI is class 1, so we have to overwrite leading 0's to ' ' */
  for(iLoop = nvi.imsi_addr_num.num + 5; iLoop <= 12; iLoop++)
  {
    pszTmpBuf[UI_WIDE - iLoop] = ' ';
  }

  sMakeString(pszTmpBuf, UI_WIDE, 12);
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertASC2MCC()                        //asc_mcc
**
** Description: 
**     Convert ASCII format number to MCC format.
**
** Input:
**     CHAR* pszNAMInfo : 
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
STATIC WORD sConvertASC2MCC(CHAR* pszNAMInfo)
{
  WORD wMCC;
  UINT iLoop;

  for(iLoop = 0; iLoop<3; iLoop++)
  {
    if(pszNAMInfo[iLoop] == ' ')
    {
      pszNAMInfo[iLoop] = '0';
    }
  }

  wMCC = aMINTable2[pszNAMInfo[0] - '0']*100 +
         aMINTable2[pszNAMInfo[1] - '0']*10  +
         aMINTable2[pszNAMInfo[2] - '0'];

  return wMCC;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertMCC2ASC()                          //mcc_asc
**
** Description: 
**     
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sConvertMCC2ASC(WORD wMCC, CHAR* pszNAMInfo)
{
   *pszNAMInfo++ = aMINTable1[wMCC/100];
   wMCC         %= 100;
   *pszNAMInfo++ = aMINTable1[wMCC/10];
   *pszNAMInfo++ = aMINTable1[wMCC%10];
   *pszNAMInfo   = '\0';
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetDefaultIMSI_S1()   UI_DEFAULT_IMSI_S1
**
** Description: 
**   This function computes the default IMSI_T_S1 value as per IS-683 section 3.1.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**  This function returns the encoded value of IMSI_T_S1 with the four least
**  significant digits set to ESNp, converted directly from binary to decimal,
**  modulo 10000.  The other digits are set to zero.
**
** Side effects:
**  Reading ESN from NV will change structure nvi.
**
** ---------------------------------------------------------------------------
*/
//DWORD sSetDefaultIMSI_S1()
//{
//  WORD wZero;             /* Encoding of three zero digits */
//  WORD wFourth;           /* Fourth from last decimal digit of the ESN */
//  WORD wLast3Digits;      /* Last three decimal digits of the ESN */
//  
//  
//  (void) SKY_GetNV( NV_ESN_I, &nvi );
//  /* Encode digits as per JSTD-008 section 2.3.1.1 */
//  wZero = 1110 - 111;
//  wLast3Digits = (nvi.esn.esn % 1000);
//  wLast3Digits += ((wLast3Digits / 100) == 0) ? 1000 : 0;
//  wLast3Digits += (((wLast3Digits % 100) / 10) == 0) ? 100 : 0;
//  wLast3Digits += ((wLast3Digits % 10) == 0) ? 10 : 0;
//  wLast3Digits -= 111;
//  wFourth = ((nvi.esn.esn % 10000) / 1000); /* In range 0-9 */
//  if (wFourth == 0)
//  {
//    wFourth = 10;
//  }
//
//  /* Concatenate results and return 24 bit value for imsi_s1 */
//  /* Example: esn = 120406
//  **      imsi_s1 = 000  0  406
//  **  encodes to -> 999  10 395
//  **       in hex = 3e7  a  18b
//  **    in binary = 1111100111 1010 0110001011
//  */
//  return (((DWORD) wZero << 14) | (wFourth << 10) | wLast3Digits);
//
//} /* ui_default_imsi_s1() */




#ifdef FEATURE_IS95B_TRUE_IMSI
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetDefaultIMSIT_S1()   UI_DEFAULT_IMSI_T_S1
**
** Description: 
**   This function computes the default IMSI_T_S1 value as per IS-683 section 3.1.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**  This function returns the encoded value of IMSI_T_S1 with the four least
**  significant digits set to ESNp, converted directly from binary to decimal,
**  modulo 10000.  The other digits are set to zero.
**
** Side effects:
**  Reading ESN from NV will change structure nvi.
**
** ---------------------------------------------------------------------------
*/
DWORD sSetDefaultIMSIT_S1()
{
  WORD wZero;             /* Encoding of three zero digits */
  WORD wFourth;           /* Fourth from last decimal digit of the ESN */
  WORD wLast3Digits;      /* Last three decimal digits of the ESN */
  
  
  (void) SKY_GetNV( NV_ESN_I, &nvi );
  /* Encode digits as per JSTD-008 section 2.3.1.1 */
  wZero = 1110 - 111;
  wLast3Digits = (nvi.esn.esn % 1000);
  wLast3Digits += ((wLast3Digits / 100) == 0) ? 1000 : 0;
  wLast3Digits += (((wLast3Digits % 100) / 10) == 0) ? 100 : 0;
  wLast3Digits += ((wLast3Digits % 10) == 0) ? 10 : 0;
  wLast3Digits -= 111;
  wFourth = ((nvi.esn.esn % 10000) / 1000); /* In range 0-9 */
  if (wFourth == 0)
  {
    wFourth = 10;
  }

  /* Concatenate results and return 24 bit value for imsi_s1 */
  /* Example: esn = 120406
  **      imsi_s1 = 000  0  406
  **  encodes to -> 999  10 395
  **       in hex = 3e7  a  18b
  **    in binary = 1111100111 1010 0110001011
  */
  return (((DWORD) wZero << 14) | (wFourth << 10) | wLast3Digits);

} /* ui_default_imsi_t_s1() */



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     gInitializeIMSIT()                 //UI_INITIALIZE_IMSI_T_COMPONENTS
**
** Description:
**  This function computes the default for all TRUE IMSI components, and then
**  writes them to NV. 
**     
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID gInitializeIMSIT(UINT8 nNAM)
{
  
  /* Default MCC is 000 in ASCII and 999 in min */
  nvi.imsi_t_mcc.nam = nNAM;
  nvi.imsi_t_mcc.imsi_mcc = IMSI_MCC_0;      /* default to '000' */
  (void) SKY_PutNV( NV_IMSI_T_MCC_I, &nvi );

  /* Default MNC is 00 in ASCII and 99 in min */
  nvi.imsi_t_11_12.nam = nNAM;
  nvi.imsi_t_11_12.imsi_11_12 = IMSI_11_12_0;  /* default to '00' */
  (void) SKY_PutNV( NV_IMSI_T_11_12_I, &nvi );

  nvi.imsi_t_addr_num.nam = nNAM;
  nvi.imsi_t_addr_num.num = IMSI_CLASS0_ADDR_NUM ;
  SKY_PutNV( NV_IMSI_T_ADDR_NUM_I, &nvi ) ;

  /* Default S2 is 000 in ASCII and 999 in min */
  nvi.imsi_t_s2.nam = nNAM;
  nvi.imsi_t_s2.min2[FMMIN] = IMSI_S2_0 ;
  nvi.imsi_t_s2.min2[CDMAMIN] = IMSI_S2_0 ;
  SKY_PutNV( NV_IMSI_T_S2_I, &nvi ) ;

  nvi.imsi_t_s1.min1[CDMAMIN] =  sSetDefaultIMSIT_S1() ;
  /* The following assignment must not be done, before the function above */
  /* because that function will read ESN which will corrupt the min1.nam. */
  nvi.imsi_t_s1.nam = nNAM;
  nvi.imsi_t_s1.min1[FMMIN] = IMSI_S1_0 ;
  SKY_PutNV( NV_IMSI_T_S1_I, &nvi ) ;

}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**  sSetDefaultIMSITS1()
**
** Description: 
**  This function computes the default IMSI_T_S1 value as per IS-683 section 3.1.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**  This function returns the encoded value of IMSI_T_S1 with the four least
**  significant digits set to ESNp, converted directly from binary to decimal,
**  modulo 10000.  The other digits are set to zero.   
**
** Side effects:
**  Reading ESN from NV will change structure nvi.
**
** ---------------------------------------------------------------------------
*/
DWORD sSetDefaultIMSITS1()
{
  WORD wZero;          /* Encoding of three zero digits */                 
  WORD wFourthDigit;   /* Fourth from last decimal digit of the ESN */    
  WORD wLast3Digit;    /* Last three decimal digits of the ESN */         

  (void) SKY_GetNV( NV_ESN_I, &nvi );
  /* Encode digits as per JSTD-008 section 2.3.1.1 */

  wZero = 1110 - 111;
  wLast3Digit = (nvi.esn.esn % 1000);
  wLast3Digit += ((wLast3Digit / 100) == 0) ? 1000 : 0;
  wLast3Digit += (((wLast3Digit % 100) / 10) == 0) ? 100 : 0;
  wLast3Digit += ((wLast3Digit % 10) == 0) ? 10 : 0;
  wLast3Digit -= 111;
  wFourthDigit = ((nvi.esn.esn % 10000) / 1000); /* In range 0-9 */
  if (wFourthDigit == 0)
  {
    wFourthDigit = 10;
  }

  /* Concatenate results and return 24 bit value for imsi_s1 */
  /* Example: esn = 120406
  **      imsi_s1 = 000  0  406
  **  encodes to -> 999  10 395
  **       in hex = 3e7  a  18b
  **    in binary = 1111100111 1010 0110001011
  */
  return (((DWORD) wZero << 14) | (wFourthDigit << 10) | wLast3Digit);

} /* ui_default_imsi_t_s1() */




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetIMSITInactive()
**
** Description: 
**  Set imsi_t_is_inactive to TRUE if TRUE IMSI has default values, particularly
**  if the S1 contains the last ESN digits, FALSE otherwise.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sSetIMSITInactive()
{
   if ((lIMSI_T_11_12.imsi_11_12 == IMSI_11_12_0) && 
      (lIMSI_T_ADDR_NUM.num == IMSI_CLASS0_ADDR_NUM) &&
      (lIMSI_T_S1.min1[NV_CDMA_MIN_INDEX] == sSetDefaultIMSITS1()) &&
      (lIMSI_T_S2.min2[NV_CDMA_MIN_INDEX] == IMSI_S2_0)
     )
  {
    fIMSIInActive = TRUE;
  }
  else
  {
    fIMSIInActive = FALSE;
  }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sLoadCheckIMSITActive()           //load_check_imsi_t_active
**
** Description: 
**     It gets TRUE IMSI data and check whether the IMSI is active or not.
**
** Input:
**     nNAM : NAM identifier
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
VOID sLoadCheckIMSITActive(UINT nNAM)
{

  /* Read in all the IMSI data from NV */
  /* get min2 */
  nvi.imsi_t_s2.nam = nNAM;
  (void) SKY_GetNV( NV_IMSI_T_S2_I, &nvi );
  lIMSI_T_S2.min2[CDMAMIN] = nvi.imsi_t_s2.min2[CDMAMIN];


  /* get min1 */
  nvi.imsi_t_s1.nam = nNAM;
  (void) SKY_GetNV( NV_IMSI_T_S1_I, &nvi );
  lIMSI_T_S1.min1[CDMAMIN] = nvi.imsi_t_s1.min1[CDMAMIN];

  /* get imsi_11_12 */
  nvi.imsi_t_11_12.nam = nNAM;
  SKY_GetNV( NV_IMSI_T_11_12_I, &nvi );
  lIMSI_T_11_12.imsi_11_12 = nvi.imsi_t_11_12.imsi_11_12;

  /* get mcc */
  nvi.imsi_t_mcc.nam = nNAM;
  SKY_GetNV( NV_IMSI_T_MCC_I, &nvi );
  lIMSI_T_MCC.imsi_mcc = nvi.imsi_t_mcc.imsi_mcc;

  /* get addr_num ( = NMSID length - 4) */
  nvi.imsi_t_addr_num.nam = nNAM;
  SKY_GetNV( NV_IMSI_T_ADDR_NUM_I, &nvi );
  lIMSI_T_ADDR_NUM.num = nvi.imsi_t_addr_num.num;


  /* Check whether IMSI is active */
  sSetIMSITInactive();  
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sGetTMCCintoNAMInfo()          //get_tmcc_into_temp_buffer
**
** Description: 
**     It gets True Mobile Country Code from NV and writes it to temp_buffer 
**     variable.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sGetTMCCintoTmpBuf(UINT8 nNAM, CHAR* pszTmpBuf)
{
  /* Load and check IMSI active status */
  sLoadCheckIMSITActive(nNAM);

  if (fIMSIInActive)
  {
    /* Copy "-Inactive-" string for Display */
    memcpy(pszTmpBuf, "-Inactive-", UI_WIDE);
  }
  else
  {
    /* Display the the TRUE IMSI MCC */
    sConvertMCC2ASC(lIMSI_T_MCC.imsi_mcc, (CHAR*)&pszTmpBuf[UI_WIDE-3]);    
  }

  sMakeString(pszTmpBuf, UI_WIDE, 3);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sGetTNMSIDintoTmpBuf(UINT8 nNAM, CHAR* pszTmpBuf)    
**     //get_tnmsid_into_temp_buffer
**
** Description:
**     This function gets the TNMSID (True National Mobile Station Identityl) 
**   number from NV into the static buffer temp_buffer. 
**     
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sGetTNMSIDintoTmpBuf(UINT8 nNAM, CHAR pszTmpBuf[])
{
  UINT8 iLoop;                //i
  CHAR  szBuf[3];             //buf[3]

  /* initialize temp_buffer to all spaces (i.e ' ') */
  (VOID) memset(pszTmpBuf, SKY_SPACE, UI_WIDE );
  
  
  /* Load and check IMSI active status */
  sLoadCheckIMSITActive(nNAM);

  if (fIMSIInActive)
  {
    /* Copy "-Inactive-" string for Display */
    memcpy(pszTmpBuf, "-Inactive-", UI_WIDE);    
  }
  else
  {
    sConvertMIN2_ASC( lIMSI_T_S2.min2[CDMAMIN], (CHAR *)&pszTmpBuf[UI_WIDE-10] );
    
    if( lIMSI_T_S1.min1[CDMAMIN] == 0 )
    {
      /* min1 == 0 indicates that no NMSID was ever stored in NV */
      memcpy( pszTmpBuf, "Never Stored", UI_WIDE);
      return;
    }
    sConvertMIN1_ASC(lIMSI_T_S1.min1[CDMAMIN], (CHAR*)(&pszTmpBuf[UI_WIDE-7]));
    sConvertMNC2ASC(lIMSI_T_11_12.imsi_11_12, (CHAR*)(&pszTmpBuf[UI_WIDE-12]));
    sConvertMCC2ASC(lIMSI_T_MCC.imsi_mcc, szBuf);
    
    /* if IMSI is class 0 return now */
    if ( lIMSI_T_ADDR_NUM.num == IMSI_CLASS0_ADDR_NUM )
    {
      return;
    }

    /* IMSI is class 1, so we have to overwrite leading 0's to ' ' */
    for( iLoop=nvi.imsi_t_addr_num.num + 5; iLoop<=12; iLoop++ )
    {
      pszTmpBuf[UI_WIDE - iLoop] = ' ';
    }
    sMakeString(pszTmpBuf, UI_WIDE, 12);
  }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWriteTNMSIDNumber(UINT8 nNAM, CHAR* pszNAMInfo)
**
** Description: 
**  This function encodes and writes the TNMSID (True National Mobile Station Identity)
**  number that is currently stored in serv3_txt_pointer to NV.
**
**
** Input:
**    UINT8 nNAM : NAM identification number
**    pszNAMInfo : NAM information buffer
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**    Returns an error code. See global errs structure for error codes.     
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL sWriteTNMSIDNumber(UINT8 nNAM, CHAR pszNAMInfo[])
{
  UINT iLoop, nNumDigits;
  CHAR szBuf[3];  
#ifdef FEATURE_OTASP
  BOOL fAllzeros;
#endif

  nNumDigits = STRLEN((BYTE*)pszNAMInfo);

#ifdef FEATURE_OTASP
  fAllzeros = TRUE ; /* assume NMSID entered is all zeros */
  for ( iLoop=1; iLoop <= nNumDigits ; iLoop++ )
  {
    if ( pszNAMInfo[UI_WIDE - iLoop] != '0' )
    {
      fAllzeros = FALSE ;
      break ;
    }
  }
#endif

    /* get MCC from NV -- needed in order to compute min1, min2, imsi_11_12 */
  nvi.imsi_t_mcc.nam = nNAM;
  SKY_GetNV( NV_IMSI_T_MCC_I, &nvi );
  lIMSI_T_MCC.imsi_mcc = nvi.imsi_t_mcc.imsi_mcc;

#ifdef FEATURE_OTASP
  /* If NMSID is all zeros and if MCC is all zeros, then we need to init */
  /* all IMSI components, using the special function which initialzes    */
  /* all components to zeros, except the last 4 digits of MIN1 are taken */
  /* from the last 4 digits of the ESN.                                  */
  if ( fAllzeros == TRUE )
  {
    if (nvi.imsi_t_mcc.imsi_mcc == IMSI_MCC_0 )
    {
      gInitializeIMSIT(nNAM) ;
      return TRUE;
    }
  }
#endif

  /* mcc is in the nvi structure already */
  sConvertMCC2ASC(nvi.imsi_t_mcc.imsi_mcc, szBuf);
  
  for(iLoop=1; iLoop<4 && iLoop + nNumDigits<=12; iLoop++)
  {
    pszNAMInfo[UI_WIDE - nNumDigits - iLoop] = szBuf[3 - iLoop];
  }

  /* compute and write imsi_addr_num */
  nvi.imsi_t_addr_num.nam = nNAM;
  nvi.imsi_t_addr_num.num = (nNumDigits == 12 ? IMSI_CLASS0_ADDR_NUM : nNumDigits - 4);
  lIMSI_T_ADDR_NUM.num = nvi.imsi_t_addr_num.num;

  if( SKY_PutNV( NV_IMSI_T_ADDR_NUM_I, &nvi ) != NV_DONE_S )
  {
    return FALSE;
  }

  if( SKY_PutNV( NV_IMSI_T_ADDR_NUM_I, &nvi ) != NV_DONE_S )
  {
    return FALSE;
  }

  /* compute and write min1/S1,min2/S2, ACCOLC */  
  if (!sWriteMIN1_MIN2_ACCOLCNumber(nNAM, pszNAMInfo))
  {
    return FALSE;
  }
  
  /* compute and write imsi_11_12 */
  nvi.imsi_t_11_12.nam = nNAM;
  nvi.imsi_t_11_12.imsi_11_12 = sConvertASC2MNC((CHAR *)&pszNAMInfo[UI_WIDE-12] );
  lIMSI_T_11_12.imsi_11_12 = nvi.imsi_t_11_12.imsi_11_12;

  if( SKY_PutNV( NV_IMSI_T_11_12_I, &nvi ) != NV_DONE_S )
  {
    return FALSE;
  }

  /* 
  ** Check whether the IMSI is active or not.
  */
  sSetIMSITInactive();

  /* return 0, indicating that NMSID was written successfuly to NV. */
  return TRUE;
}
#endif //FEATURE_IS95B_TRUE_IMSI



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sGetMCCintoNAMInfo()                       get_mcc_into_temp_buffer
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sGetMCCintoNAMInfo(UINT8 nNAM, CHAR* pszNAMInfo)
{
  nvi.imsi_mcc.nam = nNAM;
  (void) SKY_GetNV( NV_IMSI_MCC_I, &nvi );
  sConvertMCC2ASC( nvi.imsi_mcc.imsi_mcc, (CHAR *)pszNAMInfo);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sGetMNCintoNAMInfo(UINT8 nNAM, CHAR* pszNAMInfo)
{
  nvi.imsi_11_12.nam = nNAM;
  (void) SKY_GetNV( NV_IMSI_11_12_I, &nvi );
  sConvertMNC2ASC(nvi.imsi_11_12.imsi_11_12, (CHAR *)pszNAMInfo);
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sDispYesNo
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sDispYesNo(BOOL fBool, CHAR* pszNAMInfo)
{
   if(nvi.prl_enabled.enabled == TRUE)
      STRCPY((BYTE*)pszNAMInfo, (BYTE*)"YES");
   else
      STRCPY((BYTE*)pszNAMInfo, (BYTE*)"NO"); 
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertPCSNUM2ASC()
**
** Description: 
**     Translate Mobile directory number digit format to ASCII text format.
**     Mobile directory number ranges from 1 to 10 decimal, which is translated
**     to '1' .. '9', '0' ASCII text.
**     
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sConvertPCSNUM2ASC(nv_mob_dir_data_type* pSrc, CHAR* pszDest)
{
  INT nLoop;
  INT nLength = pSrc->n_digits;

  for(nLoop=0; nLoop<nLength; nLoop++)
  {
    pszDest[nLoop] = (pSrc->digitn[nLoop] == 10) ? (CHAR)'0' : (CHAR)pSrc->digitn[nLoop]+'0';
  }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertInt2ASC
**
** Description: 
** Converts an integer into a NULL terminated ASCII string.
**
** If lead_zero is true, leading zeroes are added so that the number of digits
** is equal to field_width. If you don't want leading zeroes, field_width
** is ignored but can be specified as 0.
**
** Input:
**     @ArgumentsList
**
** Output:
**     The formatted string is returned in dest_str
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
VOID sConvertInt2ASC(DWORD dwSource, CHAR* pszDestStr, BOOL fLeadZero, BYTE bFieldWidth)
{
  CHAR pszTmp[33];
  INT  nLoop1 = 0, nLoop2 = 0;

  do{
    pszTmp[nLoop1++] = (dwSource % 10) + '0';
    dwSource /= 10;
  }while(dwSource > 0);

  if(fLeadZero)
  {
    while(nLoop1 < bFieldWidth)
    {
      pszTmp[nLoop1++] = '0';
    }
  }

  while(nLoop1)
  {
    pszDestStr[nLoop2++] = pszTmp[--nLoop1];
  }
  pszDestStr[nLoop2] = 0;
} /* End of ui_itoa */



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvert2SIDNIDFormat()
**
** Description:
**     Formats a text string to display the SID/NID pair in case of Home SID/NID
**     display, Lockout SID/NID display.
**
**     e.g.
**
**     03=4/65535
**
**     dest_str is assumed to be of length UI_WIDE. If the SID/NID pair takes more
**     than 12 characters, then it is truncated and the last 3 characters is the
**     ellipsis. 
**     
**
** Input:
**     Pair_no is less than 100.
**
** Output:
**     
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
//VOID sConvert2SIDNIDFormat(CHAR* pszDestStr, BYTE bPairNO, DWORD dwSID, DWORD dwNID)
//{
//  CHAR pszLocStr[3];
//  CHAR pszTmpStr[5];
//  INT  nCurrLength, nNIDLength;
//  INT  nLoop = 0;
//
//  memset(pszDestStr, 0, UI_WIDE);   /*  Fill the display text with NULLs */
//  /* format the index to fill 2 characters with leading zeroes if necessary */
//  sConvertInt2ASC(bPairNO, pszLocStr, TRUE, 2);
//
//  pszLocStr[2] = 0; /* Null terminate */
//  strcpy((CHAR *) pszDestStr, (CHAR *) pszLocStr);
//  strcat((CHAR *) pszDestStr, "=");
//  sConvertInt2ASC(dwSID, pszTmpStr, FALSE, 0);
//
//  /* SID can be a max of 5 digits - no bounds check reqd */
//  /* Definitely not the right way of doing it */
//  strcat((CHAR*) pszDestStr, (CHAR*) pszTmpStr);
//  strcat((CHAR*) pszDestStr, (CHAR*) "/");
//
//  /* Now we the length of our dest_str is somewhere between 5 and 9 characters */
//  /* We have not overflown our buffer yet but we may once we try to concatenate */
//  /* the NID - so take the necessary precautions here */
//
//  nCurrLength = strlen((char *) pszDestStr);
//  sConvertInt2ASC(dwNID, pszTmpStr, FALSE, 0);
//  nNIDLength  = strlen((char *) pszTmpStr);
//
//  if((nCurrLength + nNIDLength) <= 12)
//  {
//    (VOID) strlen((CHAR *) pszDestStr);
//
//    /*  CR 6929: Fill with trailing spaces */
//    nLoop = (INT) strlen((CHAR *) pszDestStr);
//
//    while(nLoop < UI_WIDE)
//    {
//      pszDestStr[nLoop++] = SKY_SPACE;
//    }
//    pszDestStr[UI_WIDE] = 0;
//  }
//  else
//  {
//    nLoop = 0;   /* chop-off index */
//                 /* The magic no 4 is for '...' */
//    while(nCurrLength <= UI_WIDE - 4)
//    {
//      pszDestStr[nCurrLength++] = pszTmpStr[nLoop++];
//    }
//    (VOID) strcat((CHAR *) pszDestStr, (CHAR *) "...");
//  }
//} /* End of uiserv_format_sid_nid */




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvert2Dec()     toval
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sConvert2Dec(DWORD dwSource, CHAR* pszNAMInfo)
{
  UINT8 iLoop;
  DWORD dwTmp = dwSource;
  DWORD dwDenominator = 1;


  for(iLoop = 0; iLoop<EQS_SIZE_NAM_BUFFER; iLoop++)
  {
    dwTmp /= 10;
    if(dwTmp != 0) 
    {
      dwDenominator *= 10;
    }
    else           break;
  }

  dwTmp = dwSource;

  for(iLoop = 0; iLoop<EQS_SIZE_NAM_BUFFER; iLoop++)
  {
    dwSource /= dwDenominator;
    dwTmp    %= dwDenominator;

    if((dwTmp != 0) || (dwDenominator != 1))
    {
      pszNAMInfo[iLoop] = (CHAR)('0'+dwSource);
      dwSource          = dwTmp;
      dwDenominator    /= 10; 
    }
    else
    {
      pszNAMInfo[iLoop] = (CHAR)('0'+dwSource);
      break;
    }
  }

  pszNAMInfo[iLoop+1] = '\0';
//  sMakeString(pszNAMInfo, UI_WIDE, nDigit);
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sComp2CDMADialNumber(pszNAMInfo, nNAM))
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     BOOL TRUE, FALSE
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL sComp2CDMADialNumber(CHAR* pszNAMInfo, UINT8 nNAM)
{
   CHAR   pszTmp[20];
   UINT8  i; 
   
   /* get CDMA to compare */
   nvi.dir_number.nam = nNAM;
   (void) SKY_GetNV( NV_DIR_NUMBER_I, &nvi );

   /* is CDMA number same as AMPS number ? */
   if(0 == memcmp(pszNAMInfo, (void*)nvi.dir_number.dir_number, NV_DIR_NUMB_SIZ))
   {
     if((pszNAMInfo[0] == '1') && (pszNAMInfo[1] == '1') && (STRLEN((BYTE*)pszNAMInfo) == 10))
     {
       pszTmp[0] = '0';
       for(i=0; pszNAMInfo[i] != '\0'; i++)
       {
         pszTmp[i+1] = pszNAMInfo[i];
       }
       pszTmp[i+1] = '\0';
       STRCPY((BYTE*)pszNAMInfo, (BYTE*)pszTmp);            
     }
     return TRUE;
   }
   else
   {
     return FALSE;
   }

}   



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertPCSDialNum_ASC()
**
** Description: 
**     Convert PCS Dial Number to ASCII code
**
** Input:
**     nv_mob_dir_data_type* pSrc   : PCS dial number
**     char*                 pszTXT : a buffer which will have ASCII PCS NUM.
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
VOID sConvertPCSDialNum_ASC(nv_mob_dir_data_type* pSrc, CHAR* pszTXT, UINT8 nNAM)
{
  UINT iLoop;
  UINT nLength = pSrc->n_digits;

  /* CDMA Directory number */
  nvi.mob_dir_number.nam = nNAM;
  (VOID) SKY_GetNV( NV_DIR_NUMBER_PCS_I, &nvi );
  /* Display the Directory number */


  for(iLoop=0; iLoop<nLength; iLoop++)
  {
	pszTXT[iLoop] = (pSrc->digitn[iLoop]==10) ? (CHAR)'0' : (CHAR)pSrc->digitn[iLoop]+'0';
  }

}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertMIN2_ASC                                    //min2_asc
**
** Description: 
**     Convert a MIN2 type number to ASCII
**
** Input:
**     wAreaCode : area code
**     pszTXT    : a buffer which will have ASCII code area code
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
STATIC VOID sConvertMIN2_ASC(WORD wAreaCode, CHAR* pszTXT)
{
	*pszTXT++  = aMINTable1[wAreaCode/100];
	wAreaCode %= 100;
	*pszTXT++  = aMINTable1[wAreaCode/10];
	*pszTXT++  = aMINTable1[wAreaCode%10];
	*pszTXT    = '\0';
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertMIN1_ASC                             //min1_asc
**
** Description: 
**     convert a MIN1 type number to ASCII
**
** Input:
**     DWORD dwPhoneNum :  phone number
**     CHAR* pszTXT     :  a buffer which will have ASCII code area code
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
STATIC VOID sConvertMIN1_ASC(DWORD dwPhoneNum, CHAR* pszTXT)
{
  UINT iLoop;

  if( dwPhoneNum == 0 ) {
    for( iLoop=0; iLoop<7; iLoop++ ) {
      *pszTXT++ = '0';
    }
  } else {
    iLoop = (WORD) (dwPhoneNum>>14);
    *pszTXT++ = aMINTable1[iLoop/100];
    iLoop %= 100;
    *pszTXT++ = aMINTable1[iLoop/10];
    *pszTXT++ = aMINTable1[iLoop%10];
    dwPhoneNum &= 0x3FFFL;                /* get bottom 14 bits */
    /* next digit is top 4 bits */
    iLoop = (WORD) (( dwPhoneNum >> 10 ) & 0xF );
    *pszTXT++ = (CHAR) (((iLoop == 10 ) ? 0 : iLoop) + '0' );
    iLoop = (WORD) (dwPhoneNum & 0x3FF);  /* get bottom 10 bits */
    *pszTXT++ = aMINTable1[iLoop/100];
    iLoop %= 100;
    *pszTXT++ = aMINTable1[iLoop/10];
    *pszTXT++ = aMINTable1[iLoop%10];
  }
  *pszTXT = '\0';
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sMakeDialNumber(CHAR* pszDialNum, UINT nNAM)
**
** Description: 
**     Make a complete dial number with values of MIN1 and MIN2
**
** Input:
**     CHAR* pszDialNum : A buffer which will has ASCII code dial number
**     UINT  nNAM       : NAM number(ex. NAM1, NAM2, NAM3, NAM4)
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sMakeDialNumber(CHAR pszDialNum[], UINT nNAM)
{
  CHAR        pszTmpBuf[EQS_MAX_TMP_BUF];
	
  /* get the area code first */
  nvi.min2.nam = nNAM;
  (void) SKY_GetNV( NV_MIN2_I, &nvi );
  /* display the area code */
  sConvertMIN2_ASC(nvi.min2.min2[CDMAMIN], pszDialNum);
			
  /* get the rest of the number */
  nvi.min1.nam = nNAM;
  (void) SKY_GetNV( NV_MIN1_I, &nvi );

  /* and display the 7 digits at the end */
  sConvertMIN1_ASC(nvi.min1.min1[CDMAMIN], pszTmpBuf);
			
  /* string concatenate to make a complete dial number */
  strcat(pszDialNum, pszTmpBuf);

/*
  if((pszDialNum[0] == '1') && (pszDialNum[1] == '1'))
  {
    pszTmpBuf[0] = '0';
    
    for(nLoop = 0 ; pszDialNum[nLoop] != '\0'; nLoop++)
    {
        pszTmpBuf[nLoop + 1] = pszDialNum[nLoop];
    }
    STRCPY((BYTE*)pszDialNum, (BYTE*)pszTmpBuf);
  }     
*/
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sConvertHEX(DWORD dwSource, CHAR* pcTARGET)
**
** Description: 
**     Convert dwSource Value to pcTARGET HEX value
**
** Input:
**    DWORD  dwSource - A source value
**    CHAR*  pcTARGET - A target value
**
** Output:
**    NONE
**
** Return value:
**    NONE
**
** Side effects:
**    NONE
**
** ---------------------------------------------------------------------------
*/
VOID sConvert2HEX(DWORD dwSource, CHAR* pcTarget)
{
  UINT iLoop;
  BYTE bHEX;
  UINT nDigit = 0;

  for(iLoop = 0; iLoop < UI_WIDE - 1; iLoop++)
  {
     nDigit++;
     bHEX = dwSource % 16;
     pcTarget[UI_WIDE-2-iLoop] = (bHEX<10) ? (BYTE) ('0'+bHEX) : (BYTE) ('A'+bHEX-10);
     dwSource /= 16;
     if(dwSource == 0) break;	 
  }

  sMakeString(pcTarget, UI_WIDE, nDigit);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sNeed2ConvertMIN2IRM(CHAR pszPhoneNumber[])
**
** Description: 
**     입력된 전화번호를 보고 MIN을 IRM2.0 spec.에 맞게 변경할 지를 판단한다
**     
**
** Input:
**    
**
** Output:
**    NONE
**
** Return value:
**    NONE
**
** Side effects:
**    NONE
**
** ---------------------------------------------------------------------------
*/
LOCAL BOOL SKY_API sNeed2ConvertMIN2IRM(CHAR pszPhoneNumber[])
{
  BYTE     pszTmpBuf[11];
  UINT8    iLoop;
    
  if(STRLEN((BYTE*)pszPhoneNumber) == 10)     /* 입력된 전화번호가 10자리일 경우 */
  {                                           /*      011XXXYYYY, 11XXXXYYYY     */
      if(((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '1')) ||
         ((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '2')) ||
         ((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '3')) ||
         ((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '4')) ||
         ((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '9')))
      {
          return TRUE;
      }
      else
      {
          return FALSE;
      }
  }
  else if(STRLEN((BYTE*)pszPhoneNumber) == 11) /* 입력된 전화번호가 11자리일 경우 */
  {                                            /*     011XXXXYYYY                 */
	  for(iLoop = 0; pszPhoneNumber[iLoop+1] != '\0'; iLoop++)
      {
          pszTmpBuf[iLoop] = pszPhoneNumber[iLoop+1];
      }
      pszTmpBuf[iLoop] = '\0';

      (void) STRCPY((BYTE*)pszPhoneNumber, (BYTE*)pszTmpBuf);

      if(((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '1')) ||
         ((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '2')) ||
         ((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '3')) ||
         ((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '4')) ||
         ((pszPhoneNumber[2] == '9') && (pszPhoneNumber[3] == '9')))
      {
          return TRUE;
      }
      else
      {
          return FALSE;
      }      
  }
  else
  {
      return FALSE;
  }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSaveItems2NV()
**
** Description: 
**     Save some edited NAM items to NV.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sSaveNAMItems2NV(UINT8 nNAM, NAM_ITEM_E NAM_ITEM, CHAR* pszNAMInfo)
{
  STATIC DWORD dwValue;
  STATIC DWORD dwBackUp;
  STATIC CHAR  tmpBuff[10];
  STATIC INT   iIndex;
  STATIC WORD  wValue;
  UINT8        nLoop;
  
  switch(NAM_ITEM)
  {  
    case  ESN_I:            /* ESN */
      break;

    case  AKEY_I:           /* Press [AKEY] to continue */
      break;

    case  NAM_I:            /* NAM ? SETTINGS */
      break;
    
	case  PHONENO_I:        /* NAM ? PHONE NUMBER */
        if(nNAM == NAM1)    /* SKT NAM설정 */ 
	    {
	      if(sNeed2ConvertMIN2IRM(pszNAMInfo) == FALSE)
          {            
              for(nLoop = 0; nLoop < NV_MAX_NAMS; nLoop++)
	          {            
                /* NAM2는 초기화에서 예외임 */  
                if(nLoop != NAM2)
                {
            #ifdef FEATURE_ACP
                    /* Write both AMPS and CDMA phone numbers */
                    if(sWritePhoneNumberAMPSandCDMA(nLoop, pszNAMInfo))
            #else /* !FEATURE_ACP */
                    /* Write the CDMA phone number */
                    if(sWriteCDMANumber(pszNAMInfo, nLoop))
            #endif /* FEATURE_ACP */
			        { 
                      if(sWriteStationID(nLoop, pszNAMInfo))
			          {
		                if(sSetDefaultValue(nLoop)!= TRUE)
				        {
		                   sprintf((CHAR *)tmpBuff, "%s", "Error");   //Test code!!
				        }
			          }
			        }
                }
	          }
          }
          else /* MIN을 IRM2.0 spec.에 따라 변경할 필요가 있을 경우 */
          {
              for(nLoop = 0; nLoop < NV_MAX_NAMS; nLoop++)
	          {            
                /* NAM2는 초기화에서 예외임 */  
                if(nLoop != NAM2)
                {
                    if(nLoop > NAM2)
                    {
                        pszNAMInfo[0] = '0';
                        pszNAMInfo[1] = '1';
                        pszNAMInfo[2] = '0';
                    }
            #ifdef FEATURE_ACP
                    /* Write both AMPS and CDMA phone numbers */
                    if(sWritePhoneNumberAMPSandCDMA(nLoop, pszNAMInfo))
            #else /* !FEATURE_ACP */
                    /* Write the CDMA phone number */
                    if(sWriteCDMANumber(pszNAMInfo, nLoop))
            #endif /* FEATURE_ACP */
			        { 
                      if(sWriteStationID(nLoop, pszNAMInfo))
			          {
		                if(sSetDefaultValue(nLoop)!= TRUE)
				        {
		                   sprintf((CHAR *)tmpBuff, "%s", "Error");   //Test code!!
				        }
			          }
			        }
                }
	          }
          }
	      nvi.dir_number.nam = NAM1;
	    }
        else if(nNAM == NAM2) /* SHINSEGI NAM설정 */ 
        {
            /* NAM1은 초기화에서 예외임 */
            for(nLoop = NAM2; nLoop < NV_MAX_NAMS; nLoop++)
            {
            #ifdef FEATURE_ACP
                /* Write both AMPS and CDMA phone numbers */
                if(sWritePhoneNumberAMPSandCDMA(nLoop, pszNAMInfo))
            #else /* !FEATURE_ACP */
                /* Write the CDMA phone number */
                if(sWriteCDMANumber(pszNAMInfo, nLoop))
            #endif /* FEATURE_ACP */
                { 
                    if(sWriteStationID(nLoop, pszNAMInfo))
                    {
                        if(sSetDefaultValue(nLoop)!= TRUE)
                        {
                            sprintf((CHAR *)tmpBuff, "%s", "Error");   //Test code!!
                        }
                    }
                }
            }
	        nvi.dir_number.nam = NAM2;
        }
	    else /* NAM3 - NAM5에 번호를 전화번호를 입력하는 경우 */
	    {
    #ifdef FEATURE_ACP
          /* Write both AMPS and CDMA phone numbers */
          if(sWritePhoneNumberAMPSandCDMA(nNAM, pszNAMInfo))
    #else /* !FEATURE_ACP */
          /* Write the CDMA phone number */
          if(sWriteCDMANumber(pszNAMInfo, nNAM))
    #endif /* FEATURE_ACP */
	      { 
            if(sWriteStationID(nNAM, pszNAMInfo))
		    {
		      if(sSetDefaultValue(nNAM)!= TRUE)
		      {
		         sprintf((CHAR *)tmpBuff, "%s", "Error");   //Test code!!
		      }
		    }
	      }
	    }
      break;

    case  HOME_I:       /* NAM ? HOME SID */
/* TriMode - Same as 800 */
#if defined (FEATURE_CDMA_800) 
    case CDMAHOMESIDNID_I: /* NAM ? CDMA HOME SID */
#endif /* (FEATURE_CDMA_800)  */
      dwValue = sConvertASC2DEC(pszNAMInfo);

      if( dwValue > MAX_SID_RANGE ) 
	  {
       /*
		   Error!!
		*/
      }      
      else 
	  {
#ifdef FEATURE_SSPR_ENHANCEMENTS
        iIndex = 0;

        nvi.home_sid_nid.pair[0].sid = (WORD) dwValue;
        nvi.home_sid_nid.pair[0].nid = NID_DEFAULTS;
 
		for (iIndex = 1; iIndex < NV_MAX_HOME_SID_NID; iIndex++) 
		{
           nvi.home_sid_nid.pair[iIndex].sid = 0;
           nvi.home_sid_nid.pair[iIndex].nid = NID_DEFAULTS;
        }
              
		if (SKY_PutNV( NV_HOME_SID_NID_I, &nvi ) != NV_DONE_S) 
		{
          /*
          **   Error!!
          */           
        }
#else
        {
           for (iIndex = 0; iIndex < NV_MAX_NAMS; iIndex++) 
		   {
              nvi.sid_nid.pair[CDMAMIN][iIndex].sid = 0;
              nvi.sid_nid.pair[CDMAMIN][iIndex].nid = NID_DEFAULTS;                
		   }
        }
        nvi.sid_nid.pair[CDMAMIN][nSIDNID].sid = (WORD) dwValue;
        if (SKY_PutNV( NV_SID_NID_I, &nvi ) != NV_DONE_S) 
		{
           /*
           **   Error!!
           */
        }
#endif /* FEATURE_SSPR_ENHANCEMENTS */
            }
/* TriMode - Same as 800 */
#if defined (FEATURE_CDMA_800) 
//        if ( item == CDMAHOMESIDNID_I )    // hjcho 00.11.21 Why?....
//		{
//              break; /* only change CDMA */
//      }
            break;
#else /* (FEATURE_CDMA_800)  */
            /* If this is a PCS phone, only write the CDMA SID and exit */
            break;
#endif /* (FEATURE_CDMA_800) */


    case  NAMNAME_I:    /* NAM ? NAME */
      nvi.name_nam.nam = nNAM;
      (void) memcpy( (void *)nvi.name_nam.name, pszNAMInfo, EQS_NAME_LTRS );
      if( SKY_PutNV( NV_NAME_NAM_I, &nvi ) != NV_DONE_S ) 
	  {
            /*
			    Error 처리
			*/
      }
      break;

    case  EXIT_MORE_I:  /* more or exit */
      break;

//  case  SECURITY_I:   /* SERVICE SEC. CODE */
//
//    SECURITY_I is not used in EQUS project!!
//
//    break;

    case  CDMAMCC_I:    /* NAM ? MOBILE COUNTRY CODE default is 310 */
       nvi.imsi_mcc.nam = nNAM;
       nvi.imsi_mcc.imsi_mcc = sConvertASC2MCC(pszNAMInfo);
       if( SKY_PutNV( NV_IMSI_MCC_I, &nvi ) != NV_DONE_S )
       {
         /*
           Error!!
          */
       }
       else
       {
          sfMCCChanged = TRUE;
       }
      break;

    case  NMSID_I:      /* NATIONAL MOBILE STATION ID */
      if(sWriteNMSIDNumber(nNAM, pszNAMInfo)==TRUE)
      {
      /* change mcc_changed to FALSE only if NMSID
      ** was modified successfully */
        sfMCCChanged = FALSE;
      }
      break;

      /* hjcho 2000-11-20 --> @why, what */
#if (MODEL_ID == MODEL_B0) 
#ifdef FEATURE_IS95B_TRUE_IMSI
#error code not present
#endif      
#else
    case TCDMAMCC_I:    /* NAM ? True IMSI MCC default is 310 */
        
        wValue = sConvertASC2MCC(pszNAMInfo);

        lIMSI_T_MCC.imsi_mcc = nvi.imsi_t_mcc.imsi_mcc = wValue;
        nvi.imsi_t_mcc.nam = nNAM;        
        if( SKY_PutNV( NV_IMSI_T_MCC_I, &nvi ) != NV_DONE_S )
        {
           /*
              Error 처리
            */
        }
/* TriMode - Same as 1900 */
#ifdef FEATURE_CDMA_1900
        else
        {
          fTMCCChanged = TRUE;
        }
#endif
      break;
    
    case TNMSID_I:      /* True NATIONAL MOBILE STATION ID */
        if(sWriteTNMSIDNumber(nNAM, pszNAMInfo))
        {
        /* change mcc_changed to FALSE only if NMSID
        ** was modified successfully */
          fTMCCChanged = FALSE;
        }
      break;
#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)*/
      /* <-- */

#if defined(FEATURE_CDMA_800)  /* Dual mode phone */

#ifdef FEATURE_SSPR_ENHANCEMENTS
    case PRL_ENABLED_I: /* NAM ? PRL Enabled ? */
        nvi.prl_enabled.nam = nNAM;
        nvi.prl_enabled.enabled = sGetValueFromDisp(pszNAMInfo);
        
        /* Write CDMA Home System Registration value into NV */
        if (SKY_PutNV(NV_PRL_ENABLED_I,&nvi) != NV_DONE_S) 
        {
         /*
           Error 처리!!
          */
        }
      break;
#endif
#endif /* (FEATURE_CDMA_800)  */

#if defined(FEATURE_CDMA_800) || (defined(FEATURE_CDMA_1900) && defined(FEATURE_ACP)) /* Dual mode or dual band phone */
    case CDMADIRNUM_I: /* NAM ? CDMA PHONE NUMBER */
       if( sWriteCDMANumber(pszNAMInfo, nNAM) == TRUE )
       {
         if( sWriteLockCode(pszNAMInfo) == TRUE ) /* lock code from last four digits */
         {
            /* Set Mobile Station ID Number to the Phone number */            
             if( sWriteStationID(nNAM, pszNAMInfo) != TRUE)
             {
               /*
                  Error 처리
               */
             }
       }
      break;
#endif /* (FEATURE_ACP) */


#if defined(FEATURE_CDMA_800)  /* Dual mode phone */
    case CDMA_PRICHNA_I:     /* NAM ? CDMA Primary Chn A */
       dwValue = sConvertASC2DEC(pszNAMInfo);
       
       if(dwValue <= MAX_CHANNEL_RANGE)
       {
         /* hjcho 2000-12-06 --> @why, what */
#if (MODEL_ID == MODEL_B0) 
         nvi.pcdmach.nam = nNAM;
         nvi.pcdmach.channel_a = nvi.pcdmach.channel_b = dwValue;         
#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
         /*  NV_PCDMACH_I는 구조체 pcdmach단위로 읽고 쓰기를 한다.  */
         /*  따라서 하나의 필드 즉 channel_a나 channel_b에 대해서만 */
         /*  새로운 정보를 쓰고 SKY_PutNV를 할 경우에는 나머지 다른 */
         /*  하나의 필드에는 garbage가 씌어지게 된다. 이를 방지하기 */
         /*  위해 이전 값을 dwBackUp이라는 변수에 임시로 저장한뒤   */
         /*  SKY_PutNV를 수행하기 전에 이를 이용해 적절한 필드의    */
         /*  값을 설정한다.                                         */
         nvi.pcdmach.nam = nNAM;
         SKY_GetNV(NV_PCDMACH_I, &nvi);
         dwBackUp = nvi.pcdmach.channel_b;         
         
         nvi.pcdmach.channel_a = dwValue;
         nvi.pcdmach.channel_b = dwBackUp;         
#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)*/
         /* <-- */
         if(SKY_PutNV(NV_PCDMACH_I, &nvi) != NV_DONE_S)
         {
           /*
              Error 처리
            */
         }
       }
       else
       {
           /*
              Error 처리
            */
       }
      break;

    case CDMA_SECCHNA_I:     /* NAM ? CDMA Sec Chn A */
       dwValue = sConvertASC2DEC(pszNAMInfo);
       if (dwValue <= MAX_CHANNEL_RANGE) 
       {
         /* hjcho 2000-12-06 --> @why, what */
#if (MODEL_ID == MODEL_B0) 
         nvi.scdmach.nam = nNAM;
         nvi.scdmach.channel_a = nvi.scdmach.channel_b = dwValue;                  
#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
         /*  NV_SCDMACH_I는 구조체 scdmach단위로 읽고 쓰기를 한다.  */
         /*  따라서 하나의 필드 즉 channel_a나 channel_b에 대해서만 */
         /*  새로운 정보를 쓰고 SKY_PutNV를 할 경우에는 나머지 다른 */
         /*  하나의 필드에는 garbage가 씌어지게 된다. 이를 방지하기 */
         /*  위해 이전 값을 dwBackUp이라는 변수에 임시로 저장한뒤   */
         /*  SKY_PutNV를 수행하기 전에 이를 이용해 적절한 필드의    */
         /*  값을 설정한다.                                         */
         nvi.scdmach.nam = nNAM;
         SKY_GetNV(NV_SCDMACH_I, &nvi);
         dwBackUp = nvi.scdmach.channel_b;

         nvi.scdmach.channel_a = dwValue;         
         nvi.scdmach.channel_b = dwBackUp;
#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)*/
         /* <-- */
         if (SKY_PutNV( NV_SCDMACH_I, &nvi ) != NV_DONE_S) 
         {
            SKY_PutNV( NV_SCDMACH_I, &nvi );
         }
       }
       else 
       {
            /*
             Error 처리
             */
       }
      break;

    case CDMA_PRICHNB_I:     /* NAM ? CDMA Primary Chn B */
       dwValue = sConvertASC2DEC(pszNAMInfo);

       if (dwValue <= MAX_CHANNEL_RANGE) 
       {
         /*  NV_PCDMACH_I는 구조체 pcdmach단위로 읽고 쓰기를 한다.  */
         /*  따라서 하나의 필드 즉 channel_a나 channel_b에 대해서만 */
         /*  새로운 정보를 쓰고 SKY_PutNV를 할 경우에는 나머지 다른 */
         /*  하나의 필드에는 garbage가 씌어지게 된다. 이를 방지하기 */
         /*  위해 이전 값을 dwBackUp이라는 변수에 임시로 저장한뒤   */
         /*  SKY_PutNV를 수행하기 전에 이를 이용해 적절한 필드의    */
         /*  값을 설정한다.                                         */
         nvi.pcdmach.nam = nNAM;
         SKY_GetNV(NV_PCDMACH_I, &nvi);
         dwBackUp = nvi.pcdmach.channel_a;

         nvi.pcdmach.channel_a = dwBackUp;
         nvi.pcdmach.channel_b = dwValue;         
         if (SKY_PutNV( NV_PCDMACH_I, &nvi ) != NV_DONE_S) 
         {
             SKY_PutNV( NV_PCDMACH_I, &nvi );
         }
       }
       else 
       {
             /*
              Error 처리
              */
       }
      break;

    case CDMA_SECCHNB_I:     /* NAM ? CDMA Sec Chn B */
      dwValue = sConvertASC2DEC(pszNAMInfo);      
      if (dwValue <= MAX_CHANNEL_RANGE) 
      {
         /*  NV_SCDMACH_I는 구조체 scdmach단위로 읽고 쓰기를 한다.  */
         /*  따라서 하나의 필드 즉 channel_a나 channel_b에 대해서만 */
         /*  새로운 정보를 쓰고 SKY_PutNV를 할 경우에는 나머지 다른 */
         /*  하나의 필드에는 garbage가 씌어지게 된다. 이를 방지하기 */
         /*  위해 이전 값을 dwBackUp이라는 변수에 임시로 저장한뒤   */
         /*  SKY_PutNV를 수행하기 전에 이를 이용해 적절한 필드의    */
         /*  값을 설정한다.                                         */  
         nvi.scdmach.nam = nNAM;
         SKY_GetNV(NV_SCDMACH_I, &nvi);
         dwBackUp = nvi.scdmach.channel_a;

         nvi.scdmach.channel_a = dwBackUp;
         nvi.scdmach.channel_b = dwValue;
         if(SKY_PutNV( NV_SCDMACH_I, &nvi ) != NV_DONE_S) 
         {
           SKY_PutNV( NV_SCDMACH_I, &nvi );
         }
      }
      else 
      {
         /*
          Error 처리
          */
      }      
      break;

    case LOCK_I:         /* NAM ? LOCK OUT SYSTEM, writes to both CDMA & Analog */
      break;

#elif defined(FEATURE_CDMA_1900) && defined(FEATURE_ACP) /* Dual band phone */
    case CDMAHOMENID_I:     /* NAM ? CDMA HOME NID */
       dwValue = sConvertASC2DEC(pszNAMInfo);
       if( dwValue > MAX_NID_RANGE ) 
       {
         /*
             Error 처리
          */
       }
       else 
       {
         nvi.sid_nid.pair[CDMAMIN][nSIDNID].nid = (WORD) dwValue;
         if (SKY_PutNV( NV_SID_NID_I, &nvi ) != NV_DONE_S) 
         {
           /*
             Error 처리
            */
         }
        mccreg_clear_reg();   /* Clear out the reg sid nid */  
      }
      break;
#endif /* (FEATURE_CDMA_800)  */

    case CDMA_HOMESYS_I:     /* NAM ? Home Sys Reg */
       nvi.mob_term_home.nam = nNAM;
       nvi.mob_term_home.enabled[CDMAMIN] = sGetValueFromDisp(pszNAMInfo);
       /* Write CDMA Home System Registration value into NV */
       if (SKY_PutNV(NV_MOB_TERM_HOME_I,&nvi) != NV_DONE_S) 
       {
         /*
           ERROR 처리!!
          */
       }
       mccreg_clear_reg();   /* Clear out the reg sid nid */ 
      break;

    case CDMA_FORGNSID_I:    /* NAM ? Forgn SID Reg */
       nvi.mob_term_for_sid.nam = nNAM;
       nvi.mob_term_for_sid.enabled[CDMAMIN] = sGetValueFromDisp(pszNAMInfo);
       /* Write CDMA Home System Registration value into NV */
       if (SKY_PutNV(NV_MOB_TERM_FOR_SID_I,&nvi) != NV_DONE_S) 
       {
         /*
           Error 처리!!
          */
       }
       mccreg_clear_reg();   /* Clear out the reg sid nid */ 
      break;

    case CDMA_FORGNNID_I:    /* NAM ? Forgn NID Reg */
        nvi.mob_term_for_nid.nam = nNAM;
        nvi.mob_term_for_nid.enabled[CDMAMIN] = sGetValueFromDisp(pszNAMInfo);
        /* Write CDMA Home System Registration value into NV */
        if (SKY_PutNV(NV_MOB_TERM_FOR_NID_I,&nvi) != NV_DONE_S) 
        {
          /*
            Error 처리!!
           */
        }
//        mccreg_clear_reg();   /* Clear out the reg sid nid */ tmp
      break;

#ifdef FEATURE_ACP
    case AMPSPHONE_I:     /* NAM ? AMPS PHONE NUMBER */
       sWriteAMPSNumber(nNAM, pszNAMInfo);	   
      break;

    case AMPSHOMESID_I:   /* NAM ? AMPS HOME SID */
       dwValue = sConvertASC2DEC(pszNAMInfo);
       if( dwValue > MAX_SID_RANGE ) 
       {
           /*
              Error 처리
           */
       } 
       else 
       {
        /* Default AMPS first paging channel if
        ** the AMPS home SID is modified.
        */
          if (nvi.analog_home_sid.sid != (word) dwValue) 
          {
            nvi.analog_firstchp.nam = nNAM;
             /* Odd SID - System A */
            if (dwValue%2) 
            {
              nvi.analog_firstchp.channel = (WORD) SYSA_1ST_DED_CTL_CHAN;
            }
            else 
            {
             /* Even SID - System B */
              nvi.analog_firstchp.channel = (WORD) SYSB_1ST_DED_CTL_CHAN;
            }
          
            if( SKY_PutNV( NV_ANALOG_FIRSTCHP_I, &nvi ) != NV_DONE_S ) 
            {
               /*
                  Error 처리
                */
            }
          }

          nvi.analog_home_sid.nam = nNAM;
          nvi.analog_home_sid.sid = (WORD) dwValue;
          if( SKY_PutNV( NV_ANALOG_HOME_SID_I, &nvi ) != NV_DONE_S ) 
          {
               /*
                 Error 처리
               */
          }
         }
      break;

    case AMPS_1PGCHNL_I:  /* NAM ? AMPS 1st Paging Channel */
       
       dwValue = sConvertASC2DEC(pszNAMInfo);
       
       if ((dwVal > 0) && (dwVal < 1000)) 
       {
          nvi.analog_firstchp.nam = nNAM;
          nvi.analog_firstchp.channel = (WORD) dwValue;
          if( SKY_PutNV( NV_ANALOG_FIRSTCHP_I, &nvi ) != NV_DONE_S ) 
          {
                /*
                   Error!!
                 */
          }
       }
       else 
       {
                /*
                   Error!!
                 */
       }
      break;

    case AMPS_AUTOREG_I:  /* NAM ? AMPS AutoReg */
        nvi.analog_reg.nam =  nNAM;
        nvi.analog_reg.kind = (nv_analog_reg_enum_type) sGetValueFromDisp(pszNAMInfo);
        /* Write CDMA Home System Registration value into NV */
        if (SKY_PutNV(NV_ANALOG_REG_I,&nvi) != NV_DONE_S) 
        {
            /*
               Error 처리
             */
        }      
      break;

#endif /* FEATURE_ACP */

    case OVERLOAD_I:   /* NAM ? Access Overload Class */
      break;

    case SMS_UTC_I:    /* SMS UTC time */
      break;

    case MODEL_I:      /* PHONE MODEL */
      break;
    
	case SLOTIDX_I:    /* SLOT CYCLE INDEX */
      dwValue = sConvertASC2DEC(pszNAMInfo);
      if(dwValue >= 8)
	  {
           /* Error 처리 */
	  }
	  else
	  {
		if(nvi.slot_cycle_index != (BYTE) dwValue)
		{
           mccreg_clear_reg();  
		}
        nvi.slot_cycle_index = (BYTE) dwValue;
		if(SKY_PutNV(NV_SLOT_CYCLE_INDEX_I, &nvi) != NV_DONE_S)
		{
			/* Error 처리 */
		}
	  }
      break;

    case SCM_I:        /* STATION CLASS MARk */
      dwValue = sConvertASC2DEC(pszNAMInfo);
	  if(dwValue > 999)
	  {
             /* Error 처리 */
	  }
      else
	  {
		  nvi.scm = (BYTE) dwValue;
		  if(SKY_PutNV(NV_SCM_I, &nvi) != NV_DONE_S)
		  {
		     /*  Error 처리 */
		  }
	  }		
      break;
      /* hjcho 2000-11-26 --> @why, what */
#if (MODEL_ID == MODEL_B0)
/* It is only used for MSM3100 */
    case CDMAHOMESID_I:
      break;      
#endif /* (MODEL_ID == MODEL_B0) */
      /* <-- */
      /* hjcho 2000-11-26 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
//    case PREFSYS_I:
//      sConvertASC2DEC(pszNAMInfo);
//      nvi.system_pref.nam = nNAM;
//      nvi.system_pref.sys = sConvertASC2DEC(pszNAMInfo);
//      (void) SKY_PutNV( NV_SYSTEM_PREF_I, &nvi );
      break;
    
    case CDMAMNC_I:
      nvi.imsi_11_12.imsi_11_12 = sConvertASC2MNC(pszNAMInfo);
      nvi.imsi_11_12.nam        = nNAM;
      (void)SKY_PutNV(NV_IMSI_11_12_I, &nvi);
      break;
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)*/
      /* <-- */
   }   
  }  
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sReadNAMItemsFromNV()
**
** Description: 
**     Read NAM Prog Items from NV.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sReadNAMItemsFromNV(NAM_ITEM_E NAM_ITEM, UINT8 nNAM, CHAR* pszNAMInfo)
{
  CHAR        pszTmpBuf[EQS_MAX_TMP_BUF];
      
  switch(NAM_ITEM)
  {  
    case  ESN_I:        /* ESN */
      (void) SKY_GetNV( NV_ESN_CHKSUM_I, &nvi );
      if( nvi.esn_chksum.chksum != NV_VALID_ESN_CHKSUM ) 
      {
          sprintf((CHAR *)pszNAMInfo, "%s", "Invalid ESN CHKSUM");
      }
	  else
	  {
          (void) SKY_GetNV( NV_ESN_I, &nvi );
		  sConvert2HEX((dword)nvi.esn.esn, pszNAMInfo);
	  }
      break;

    case  AKEY_I:       /* Press [AKEY] to continue */	  
      break;
    case  NAM_I:        /* NAM ? SETTINGS */
      break;
    case  PHONENO_I:    /* NAM ? PHONE NUMBER */		    
#ifdef FEATURE_ACP
            /* get AMPS number  */
          sMakeDialNumber(pszNAMInfo, nNAM);
		  if(sComp2CDMADialNumber(pszNAMInfo, nNAM) == FALSE)
          {
             STRCPY((BYTE*)pszNAMInfo, "-NOT COMMON-");
          }
#else /* PCS - No AMPS */
          sMakeDialNumber(pszNAMInfo, nNAM);
          if(sComp2CDMADialNumber(pszNAMInfo, nNAM) == FALSE)
          {
            STRCPY((BYTE*)pszNAMInfo, (BYTE*)"-NOT COMMON-");
          }          
#endif /* FEATURE_ACP */
      break;

    case  HOME_I:       /* NAM ? HOME SID */

#ifdef FEATURE_ACP
         WORD  wTmpVal;

         /* Get the Analog SID */
         nvi.analog_home_sid.nam = nNAM;
         (void) SKY_GetNV( NV_ANALOG_HOME_SID_I, &nvi );
         wTmpVal = nvi.analog_home_sid.sid ;

         /* Display the SID */
         sConvert2Dec((DWORD) nvi.analog_home_sid.sid, pszNAMInfo);

#ifdef FEATURE_SSPR_ENHANCEMENTS
            /* is it the same as CDMA HOME SID */
            /* If not, overwrite the SID with the Not Common text */
         nSIDNID = 0;
         nvi.home_sid_nid.nam = nNAM;
         (void) SKY_GetNV( NV_HOME_SID_NID_I, &nvi );

         if ( wTmpVal != nvi.home_sid_nid.pair[nSIDNID].sid ) 
         {
            /* no the numbers are not the same */
            STRCPY(pszNAMInfo, "-NOT COMMON-");                
         }

#else /* !FEATURE_SSPR_ENHANCEMENTS */

         /* is it the same as CDMA HOME SID */
         /* If not, overwrite the SID with the Not Common text */
         nSIDNID = 0;
         nvi.sid_nid.nam = nNAM;
         (void) SKY_GetNV( NV_SID_NID_I, &nvi );

         if ( wTmpVal != nvi.sid_nid.pair[CDMAMIN][nSIDNID].sid ) 
         {
            /* no the numbers are not the same */
            STRCPY(pszNAMInfo, "-NOT COMMON-");                
         }

#endif /* FEATURE_SSPR_ENHANCEMENTS */

#else /* PCS - which is CDMA only */

         /* Get the PCS SID */
         nSIDNID = 0;
         /* Display the SID */
#ifdef FEATURE_SSPR_ENHANCEMENTS
         nvi.home_sid_nid.nam = nNAM;
         (void) SKY_GetNV( NV_HOME_SID_NID_I, &nvi );
         sConvert2Dec((DWORD) nvi.home_sid_nid.pair[nSIDNID].sid, pszNAMInfo);
#else
         nvi.sid_nid.nam = nNAM;
         (void) SKY_GetNV( NV_SID_NID_I, &nvi );
         sConvert2Dec((DWORD) nvi.sid_nid.pair[CDMAMIN][nSIDNID].sid, pszNAMInfo);            
#endif /* FEATURE_SSPR_ENHANCEMENTS */

#endif /* FEATURE_ACP */
      break;

    case  NAMNAME_I:    /* NAM ? NAME */
         nvi.name_nam.nam = nNAM;
         (void) SKY_GetNV( NV_NAME_NAM_I, &nvi );
         STRCPY((BYTE*)pszNAMInfo, (BYTE*) nvi.name_nam.name );
      break;

    case  EXIT_MORE_I:  /* more or exit */
                        /* BASIC NAM _ PROGRAMMING IS COMPLETE. EXIT MORE */
        /*
        **  Use message box
        */
      break;
    
    
    case  SECURITY_I:   /* SERVICE SEC. CODE */
        /*
        **  We will never enter this case statement in EQUS Project.
        **  Because EQUS process security code in Nmber editor.
        */
      break;

    case  CDMAMCC_I:    /* NAM ? MOBILE COUNTRY CODE default is 310 */
       /* Mobile Station Country Code */
       sGetMCCintoNAMInfo(nNAM, pszNAMInfo);
       break;


    case  NMSID_I:      /* NATIONAL MOBILE STATION ID */
                        /* National Mobile Station Identity */
       if (sfMCCChanged)
       {
         /* mcc was changed, so we must enter NMSID number as well, */
         /* so we go directly to digit editing state */
       }
       else
       {
         /* mcc was not changed, so we bring up the current NMSID */
          sGetNMSIDintoTmpBuf(nNAM, pszTmpBuf);
          (VOID) memcpy(pszNAMInfo, pszTmpBuf, UI_WIDE);          
        }
      break;

      /* hjcho 2000-11-20 --> @why, what */
#if (MODEL_ID == MODEL_B0) 
#ifdef FEATURE_IS95B_TRUE_IMSI
#error code not present
#endif      
#else
    case TCDMAMCC_I:    /* NAM ? True IMSI MCC default is 310 */
          /* Mobile Station Country Code */
        sGetTMCCintoTmpBuf(nNAM, pszTmpBuf);

        if (fIMSIInActive)
        {
          (VOID) memcpy(pszNAMInfo, pszTmpBuf, UI_WIDE );
        }
        else
        {
          (VOID) memcpy((CHAR*)&pszNAMInfo[UI_WIDE-3], (CHAR*)&pszTmpBuf[UI_WIDE-3], 3);
        }
      break;

    case TNMSID_I:      /* True NATIONAL MOBILE STATION ID */
         /* National Mobile Station Identity */
         if (fTMCCChanged)
         {
           /* mcc was changed, so we must enter NMSID number as well, */
           /* so we go directly to digit editing state */
         }
         else
         {
           /* mcc was not changed, so we bring up the current NMSID */
           sGetTNMSIDintoTmpBuf(nNAM, pszTmpBuf);
           (VOID)STRCPY((BYTE*)pszNAMInfo, (BYTE*)pszTmpBuf);
         }        
      break;
#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)*/
      /* <-- */

#if defined(FEATURE_CDMA_800)  /* Dual mode phone */
#ifdef FEATURE_SSPR_ENHANCEMENTS
    case PRL_ENABLED_I: /* NAM ? PRL Enabled ? */
         nvi.prl_enabled.nam = nNAM;
         (void) SKY_GetNV( NV_PRL_ENABLED_I, &nvi );
         sDispYesNo(nvi.prl_enabled.enabled, pszNAMInfo);         
      break;
#endif
#endif /* (FEATURE_CDMA_800)  */

#if defined(FEATURE_CDMA_800) || (defined(FEATURE_CDMA_1900) && defined(FEATURE_ACP)) /* Dual mode or dual band phone */
    case CDMADIRNUM_I: /* NAM ? CDMA PHONE NUMBER */
       /* Mobile Directory Number */
       {
//         /* temp array for phone# */
//         CHAR sTmpPhoneNum[UI_WIDE*2];
//
//         /* CDMA Directory number */
//         nvi.mob_dir_number.nam = nNAM;
//         (void) SKY_GetNV( NV_DIR_NUMBER_PCS_I, &nvi );
//
//         /* Display the Directory number */
//         memset(sTmpPhoneNum, ' ', UI_WIDE*2);
//         sConvertPCSNUM2ASC(
//                &(nvi.mob_dir_number),
//                &sTmpPhoneNum[UI_WIDE*2-nvi.mob_dir_number.n_digits]);                
//         memcpy(pszNAMInfo, sTmpPhoneNum, EQS_SIZE_NAM_BUFFER);
//         pszNAMInfo[EQS_SIZE_NAM_BUFFER-1] = '\0';
           nvi.dir_number.nam = nNAM;
           (VOID) SKY_GetNV( NV_DIR_NUMBER_I, &nvi );
           (VOID) memcpy( pszNAMInfo, EQS_SPACES_TXT, UI_WIDE);
           (VOID) memcpy( &pszNAMInfo[UI_WIDE - 1 - NV_DIR_NUMB_SIZ],
                                (void *)nvi.dir_number.dir_number, NV_DIR_NUMB_SIZ );
           sMakeString(pszNAMInfo, UI_WIDE, NV_DIR_NUMB_SIZ);
        }
      break;
#endif /* (FEATURE_ACP) */


#if defined(FEATURE_CDMA_800)  /* Dual mode phone */
//    case CDMAHOMESIDNID_I:   /* NAM ? CDMA HOME SID/NID */
//         gbHOME_SIDNID_INDEX = 0;
//#ifdef FEATURE_SSPR_ENHANCEMENTS
//         nvi.home_sid_nid.nam = nNAM;
//         (void) SKY_GetNV(NV_HOME_SID_NID_I, &nvi);
//         sConvert2SIDNIDFormat((CHAR*)pszTmpBuf, (byte)(gbHOME_SIDNID_INDEX+1),
//                      (dword) nvi.home_sid_nid.pair[gbHOME_SIDNID_INDEX].sid,
//                      (dword) nvi.home_sid_nid.pair[gbHOME_SIDNID_INDEX].nid);
//#else
//         nvi.sid_nid.nam = nNAM;
//         (void) SKY_GetNV( NV_SID_NID_I, &nvi );
//         sConvert2SIDNIDFormat((CHAR*)pszTmpBuf, (byte)(gbHOME_SIDNID_INDEX+1),
//                      (dword) nvi.sid_nid.pair[CDMAMIN][gbHOME_SIDNID_INDEX].sid,
//                      (dword) nvi.sid_nid.pair[CDMAMIN][gbHOME_SIDNID_INDEX].nid);
//#endif
//         (void) memcpy(pszNAMInfo, pszTmpBuf, UI_WIDE);
//      break;

    case CDMA_PRICHNA_I:     /* NAM ? CDMA Primary Chn A */
         nvi.pcdmach.nam = nNAM;
         (void) SKY_GetNV( NV_PCDMACH_I, &nvi );
         sConvert2Dec((DWORD) nvi.pcdmach.channel_a, pszNAMInfo);
//         sConvert2Dec((DWORD) 800, pszNAMInfo);
//         sConvert2Dec((DWORD) 801, pszNAMInfo);
//         sConvert2Dec((DWORD) 733, pszNAMInfo);
       break;

    case CDMA_SECCHNA_I:     /* NAM ? CDMA Sec Chn A */
         nvi.scdmach.nam = nNAM;
         (void) SKY_GetNV( NV_SCDMACH_I, &nvi );
         sConvert2Dec((DWORD) nvi.scdmach.channel_a, pszNAMInfo);         
      break;

    case CDMA_PRICHNB_I:     /* NAM ? CDMA Primary Chn B */
         nvi.pcdmach.nam = nNAM;
         (void) SKY_GetNV( NV_PCDMACH_I, &nvi );
         sConvert2Dec((DWORD) nvi.pcdmach.channel_b, pszNAMInfo);
      break;

    case CDMA_SECCHNB_I:     /* NAM ? CDMA Sec Chn B */
         nvi.scdmach.nam = nNAM;
         (void) SKY_GetNV( NV_SCDMACH_I, &nvi );
         sConvert2Dec((DWORD) nvi.scdmach.channel_b, pszNAMInfo);            
      break;

    case LOCK_I:             /* NAM ? LOCK OUT SYSTEM, writes to both CDMA & Analog */
                             /* but write to both CDMA and Analog */
//      gbLOCKOUT_SIDNID_INDEX = 0;
//
//#ifdef FEATURE_SSPR_ENHANCEMENTS
//      nvi.sid_nid_lock.nam = nNAM;
//      (void) SKY_GetNV(NV_SID_NID_LOCK_I, &nvi);
//      sConvert2SIDNIDFormat((CHAR*)pszTmpBuf, (BYTE)(gbLOCKOUT_SIDNID_INDEX+1),
//                      (dword) nvi.sid_nid_lock.pair[gbLOCKOUT_SIDNID_INDEX].sid,
//                      (dword) nvi.sid_nid_lock.pair[gbLOCKOUT_SIDNID_INDEX].nid);
//#else
//      nvi.cdma_sid_lock.nam = nam;
//      (void) SKY_GetNV( NV_CDMA_SID_LOCK_I, &nvi );
//      sConvert2SIDNIDFormat((char*)pszTmpBuf, (byte)(gbLOCKOUT_SIDNID_INDEX+1),
//                      (dword) nvi.cdma_sid_lock.sid[gbLOCKOUT_SIDNID_INDEX],
//                      (dword) nvi.cdma_sid_lock.sid[gbLOCKOUT_SIDNID_INDEX]);
//#endif /* FEATURE_SSPR_ENHANCEMENTS */
//      (void) memcpy ( pszNAMInfo, pszTmpBuf, UI_WIDE);
      break;

#elif defined(FEATURE_CDMA_1900) && defined(FEATURE_ACP) /* Dual band phone */
    case CDMAHOMENID_I:     /* NAM ? CDMA HOME NID */
       nSIDNID = 0;
       nvi.sid_nid.nam = nNAM;
       (void) SKY_GetNV( NV_SID_NID_I, &nvi );
       sConvert2Dec(nvi.sid_nid.pair[CDMAMIN][0].nid, pszNAMInfo);       
      break;

#endif /* (FEATURE_CDMA_800)  */

    case CDMA_HOMESYS_I:     /* NAM ? Home Sys Reg */
       nvi.mob_term_home.nam = nNAM;
       /* Read CDMA Home System Registration value from NV */
       (void) SKY_GetNV(NV_MOB_TERM_HOME_I,&nvi);
       sDispYesNo(nvi.mob_term_home.enabled[CDMAMIN], pszNAMInfo);       
      break;

    case CDMA_FORGNSID_I:    /* NAM ? Forgn SID Reg */
       nvi.mob_term_for_sid.nam = nNAM;
       /* Read CDMA Home System Registration value from NV */
       (void) SKY_GetNV(NV_MOB_TERM_FOR_SID_I,&nvi);
       sDispYesNo(nvi.mob_term_for_sid.enabled[CDMAMIN], pszNAMInfo);
       break;

    case CDMA_FORGNNID_I:    /* NAM ? Forgn NID Reg */
       nvi.mob_term_for_nid.nam = nNAM;
       /* Read CDMA Home System Registration value from NV */
       (void) SKY_GetNV(NV_MOB_TERM_FOR_NID_I,&nvi);
       sDispYesNo(nvi.mob_term_for_nid.enabled[CDMAMIN], pszNAMInfo);        
      break;


#ifdef FEATURE_ACP
    case AMPSPHONE_I:     /* NAM ? AMPS PHONE NUMBER */
      /* get AMPS number  */
      /* get the area code first */
       nvi.min2.nam = nNAM;
       (void) SKY_GetNV( NV_MIN2_I, &nvi );
      /* display the area code */
       sConvertMIN2_ASC(nvi.min2.min2[FMMIN], (CHAR*) &pszTmpBuf[2]);
      /* get the rest of the number */         
       nvi.min1.nam = nNAM;
       (void) SKY_GetNV( NV_MIN1_I, &nvi );
       /* and display the 7 digits at the end */
       sConvertMIN1_ASC(nvi.min1.min1[FMMIN], (CHAR*) &pszTmpBuf[UI_WIDE-7]);
       sMakeString(pszTmpBuf, UI_WIDE, 12);
       STRCPY(pszNAMInfo, pszTmpBuf);
      break;

    case AMPSHOMESID_I:   /* NAM ? AMPS HOME SID */
       nvi.analog_home_sid.nam = nNAM;
       (void) SKY_GetNV( NV_ANALOG_HOME_SID_I, &nvi );
       sConvert2Dec((DWORD) nvi.analog_home_sid.sid, pszNAMInfo);
      break;

    case AMPS_1PGCHNL_I:  /* NAM ? AMPS 1st Paging Channel */
       nvi.analog_firstchp.nam = nNAM;
       (void) SKY_GetNV( NV_ANALOG_FIRSTCHP_I, &nvi );
       sConvert2Dec((DWORD) nvi.analog_firstchp.channel, pszNAMInfo);
       break;

    case AMPS_AUTOREG_I:  /* NAM ? AMPS AutoReg */
       nvi.analog_reg.nam = nNAM;
       /* Read AMPS auto registration value from NV and display.  */
       (void) SKY_GetNV(NV_ANALOG_REG_I,&nvi);
       sDispYesNo(nvi.analog_reg.kind, pszNAMInfo);
       break;
#endif /* FEATURE_ACP */

    case OVERLOAD_I:   /* NAM ? Access Overload Class */
       nvi.accolc.nam = nNAM;
       (void) SKY_GetNV( NV_ACCOLC_I, &nvi );
       sConvert2Dec((DWORD) nvi.accolc.class[CDMAMIN], pszNAMInfo);       
      break;

    case SMS_UTC_I:    /* SMS UTC time */
//       (void) SKY_GetNV( NV_SMS_UTC_I, &nvi);
//       ui.sms_as_is_time = nvi.sms_utc;
//       sms_utc[ui.sms_as_is_time][0] = UI_CHECK_MARK;
//       (void) uiscrn_bind_field ( UI_SERV3_F, (char *)&sms_utc[ui.sms_as_is_time] );
      break;

    case MODEL_I:      /* PHONE MODEL */
//       Do not use it in EQUS project!!
//
        (VOID) SKY_GetNV( NV_MOB_MODEL_I, &nvi );
        sConvert2Dec((DWORD) nvi.mob_model, pszNAMInfo);
//
//        (void) SKY_GetNV( NV_WDC_I, &nvi );
//        /* shift over model number to fit Warranty Date Code info */
//
//        pszNAMInfo[ ((UI_WIDE - 1) - (NV_WDC_LTRS + 1)) - 2 ] =
//               pszNAMInfo[ (UI_WIDE - 1) - 2 ];
//        pszNAMInfo[ ((UI_WIDE - 1) - (NV_WDC_LTRS + 1)) - 1 ] =
//               pszNAMInfo[ (UI_WIDE - 1) - 1 ];
//        pszNAMInfo[ ((UI_WIDE - 1) - (NV_WDC_LTRS + 1)) ] =
//               pszNAMInfo[ (UI_WIDE - 1) ];
//
//        (VOID) memcpy( &pszNAMInfo[ (UI_WIDE - NV_WDC_LTRS) ],
//                                  (BYTE *) nvi.wdc.wdc , NV_WDC_LTRS );      
      break;

    case SLOTIDX_I:    /* SLOT CYCLE INDEX */
        (void) SKY_GetNV( NV_SLOT_CYCLE_INDEX_I, &nvi );
        sConvert2Dec((DWORD) nvi.slot_cycle_index, pszNAMInfo);
      break;

    case SCM_I:        /* STATION CLASS MARk */
        (void) SKY_GetNV( NV_SCM_I, &nvi );
        sConvert2Dec((DWORD) nvi.scm, pszNAMInfo);        
      break;    
        

      /* hjcho 2000-11-26 --> @why, what */
#if (MODEL_ID == MODEL_B0)
/* It is only used for MSM3100 */
    case CDMAHOMESID_I:
      break;      
#endif /* (MODEL_ID == MODEL_B0) */
      /* <-- */
      
/* hjcho 2000-11-26 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
    case PREFSYS_I:
      break;

    case CDMAMNC_I:
       sGetMNCintoNAMInfo(nNAM, pszNAMInfo);
       break;
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)*/
/* <-- */  
    default:
      sprintf((CHAR *)pszNAMInfo, "%s", "INVALID ITEM");
      break;
  }  
}
  



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sCheckUninitializedItems()
**
** Description: 
**     Check uninitialized items.
**     If there are uninitialized items then set some appropriate values.
** 
** Input:
**     NONE
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     uninitialized items are to be initialized with given value.
**
** ---------------------------------------------------------------------------
*/
void sCheckUninitializedItems(void)
{
  CHAR  min;
  BYTE  bNAM;    //nam;
  UINT  nLoop;   //j;

#ifdef FEATURE_ACP
  nvi.nxtreg = 0;
  (void) SKY_PutNV( NV_NXTREG_I, &nvi );
  nvi.lstsid = 0;
  (void) SKY_PutNV( NV_LSTSID_I, &nvi );
  nvi.locaid = 0;
  (void) SKY_PutNV( NV_LOCAID_I, &nvi );
  nvi.pureg = FALSE;
  (void) SKY_PutNV( NV_PUREG_I, &nvi );
#endif /* FEATURE_ACP */
  
  nvi.zone_list.reg_zone = 0;
  nvi.zone_list.sid = 0;
  nvi.zone_list.nid = 0;
  (void) SKY_PutNV( NV_ZONE_LIST_I, &nvi );
  nvi.dist_reg.base_lat_reg = 0;
  nvi.dist_reg.base_long_reg = 0;
  nvi.dist_reg.reg_dist_reg = 0;
  (void) SKY_PutNV( NV_DIST_REG_I, &nvi );
  for( bNAM=0; bNAM<NV_MAX_NAMS; bNAM++ ) 
  {
    nvi.last_cdmach.nam = bNAM;
    nvi.last_cdmach.channel_a = 0;
    nvi.last_cdmach.channel_b = 0;
    (void) SKY_PutNV( NV_LAST_CDMACH_I, &nvi );
  }

  for( bNAM=0; bNAM<NV_MAX_NAMS; bNAM++ ) 
  {
    nvi.nam_chksum.nam = bNAM;
    /* NV initializes NV_NAM_CHKSUM_I to 0, so it is active for sure */
    (void) SKY_GetNV( NV_NAM_CHKSUM_I, &nvi );
    if( nvi.nam_chksum.chksum != 0xFFFF ) 
    {
       nvi.curr_nam = 0;
      (void) SKY_PutNV( NV_CURR_NAM_I, &nvi );
      nvi.auto_nam = FALSE;
      (void) SKY_PutNV( NV_AUTO_NAM_I, &nvi );

      nvi.name_nam.nam = bNAM;

      (void) memcpy( (void *) nvi.name_nam.name, (const void *)"스카이", UI_WIDE );
      (void) SKY_PutNV( NV_NAME_NAM_I, &nvi );

      nvi.cdma_sid_lock.nam = bNAM;

#if defined(FEATURE_SSPR_ENHANCEMENTS)
      for (nLoop = 0; nLoop<NV_MAX_SID_NID_LOCK; nLoop++) 
      {
        nvi.sid_nid_lock.pair[nLoop].sid = 0;
        nvi.sid_nid_lock.pair[nLoop].nid = 0;
      }
      (void) SKY_PutNV( NV_SID_NID_LOCK_I, &nvi );
#elif defined(FEATURE_CDMA_800)
      for( nLoop = 0; j< NV_MAX_SID_LOCK; nLoop++ ) 
      {
        nvi.cdma_sid_lock.sid[nLoop] = 0;
      }
      (void) SKY_PutNV( NV_CDMA_SID_LOCK_I, &nvi );
#endif /* FEATURE_CDMA_800 */

/* Home SID/NID is not per MIN */
/* for non-SSPR targets see towards end of function */
#ifdef FEATURE_SSPR_ENHANCEMENTS
//    nvi.home_sid_nid.nam = nLoop;
//    (void) SKY_GetNV( NV_HOME_SID_NID_I, &nvi );
//    for( nLoop=0; nLoop<NV_MAX_SID_NID; nLoop++) 
//    {
//      /*  NID default */
//      nvi.home_sid_nid.pair[nLoop].sid = 0;
//      nvi.home_sid_nid.pair[nLoop].nid = NID_DEFAULTS ;
//    }
//    (void) SKY_PutNV( NV_HOME_SID_NID_I, &nvi );
#endif

#ifdef FEATURE_ACP
      /* Also clear the Analog SID lock list.  The NV item doesn't
         need to be set to all zeros, cuz it's using the CDMA Lock
         list set to zeros above.  This is possible, cuz the structures
         are exactly the same. */

      (void) SKY_PutNV( NV_ANALOG_SID_LOCK_I, &nvi );

      nvi.analog_firstchp.nam = bNAM;
      /* default for 1st Paging Channel is 333 if system A */
      /* and is 334 if system B                               */
      nvi.analog_firstchp.channel = 0;
      (void) SKY_PutNV( NV_ANALOG_FIRSTCHP_I, &nvi );


      nvi.analog_home_sid.nam = bNAM;
      nvi.analog_home_sid.sid = 0;
      (void) SKY_PutNV( NV_ANALOG_HOME_SID_I, &nvi );

      nvi.analog_reg.nam = bNAM;
      /* Auto registration default is yes */
      nvi.analog_reg.kind = NV_WHEREABOUTS_KNOWN;
      (void) SKY_PutNV( NV_ANALOG_REG_I, &nvi );

      nvi.analog_pref_serv.nam = bNAM;
      nvi.analog_pref_serv.sys = NV_SYS_A_PREF;
      (void) SKY_PutNV( NV_ANALOG_PREF_SERV_I, &nvi );


#endif /* FEATURE_ACP */

#ifdef FEATURE_CDMA_800
/*
      nvi.pcdmach.nam = bNAM;
      nvi.pcdmach.channel_a = PCH_A_DEF;
      nvi.pcdmach.channel_b = PCH_B_DEF;
      (void) SKY_PutNV( NV_PCDMACH_I, &nvi );

      nvi.scdmach.nam = bNAM;
      nvi.scdmach.channel_a = SCH_A_DEF;
      nvi.scdmach.channel_b = SCH_B_DEF;
      (void) SKY_PutNV( NV_SCDMACH_I, &nvi );

      nvi.pref_mode.nam = bNAM;
      nvi.pref_mode.mode = NV_MODE_DIGITAL_PREF;
      (void) SKY_PutNV( NV_PREF_MODE_I, &nvi );

      nvi.cdma_pref_serv.nam = bNAM;
      nvi.cdma_pref_serv.sys = NV_SYS_A_PREF;
      (void) SKY_PutNV( NV_CDMA_PREF_SERV_I, &nvi );

      nvi.ppcnch.nam = bNAM;
      nvi.ppcnch.channel_a = 0;
      nvi.ppcnch.channel_b = 0;
      (void) SKY_PutNV( NV_PPCNCH_I, &nvi );

      nvi.spcnch.nam = bNAM;
      nvi.spcnch.channel_a = 0;
      nvi.spcnch.channel_b = 0;
      (void) SKY_PutNV( NV_SPCNCH_I, &nvi );
*/
#endif /* not FEATURE_CDMA_1900 */

      nvi.count.nam = bNAM;
      nvi.count.count = 0;
      (void) SKY_PutNV( NV_COUNT_I, &nvi );

      nvi.air_timer.nam = bNAM;
      nvi.air_timer.time = 0;
      (void) SKY_PutNV( NV_AIR_TIMER_I, &nvi );

      nvi.roam_timer.nam = bNAM;
      nvi.roam_timer.time = 0;
      (void) SKY_PutNV( NV_ROAM_TIMER_I, &nvi );

      nvi.life_timer_g.time = 0;
      (void) SKY_PutNV( NV_LIFE_TIMER_G_I, &nvi );

      nvi.nam_chksum.nam = bNAM;
      nvi.nam_chksum.chksum = 0xffff;
      (void) SKY_PutNV( NV_NAM_CHKSUM_I, &nvi );

      /* Now set MCC and MNC (IMSI_11_12) to default values */
#ifndef FEATURE_OTASP
      /* Already initialized these during start up */
      nvi.imsi_mcc.nam = bNAM;
      /* Default MCC is 000 in ASCII and 999 in min */
      nvi.imsi_mcc.imsi_mcc = IMSI_MCC_0;      /* default to '000' */
      (void) SKY_PutNV( NV_IMSI_MCC_I, &nvi );

      nvi.imsi_11_12.nam = bNAM;
      nvi.imsi_11_12.imsi_11_12 = IMSI_11_12_0;  /* default to '00' */
      (void) SKY_PutNV( NV_IMSI_11_12_I, &nvi );
#endif
    }  /*  if( nvi.nam_chksum.chksum != 0xFFFF ) */

    nvi2.min_chksum.nam = bNAM;
    /* NV initializes NV_MIN_CHKSUM_I to 0, so it is active for sure */
    (void) SKY_GetNV( NV_MIN_CHKSUM_I, &nvi2 );
    for( min=0; min<NV_MAX_MINS; min++ ) {
      if( nvi2.min_chksum.chksum[min] != 0xffff ) {
        //MSG_HIGH("NAM %d  MIN %d bad - building", nam, min, 0 );
      }
    }

#ifndef FEATURE_OTASP
    /* already initialized during start up */
    nvi.min1.nam = bNAM;
    (void) SKY_GetNV( NV_MIN1_I, &nvi );

    for( min=0; min<NV_MAX_MINS; min++ ) {
      if( nvi2.min_chksum.chksum[min] != 0xffff ) {
        nvi.min1.min1[min] = 0;
      }
    }
    (void) SKY_PutNV( NV_MIN1_I, &nvi );

    nvi.min2.nam = bNAM;
    (void) SKY_GetNV( NV_MIN2_I, &nvi );
    for( min=0; min<NV_MAX_MINS; min++ ) {
      if( nvi2.min_chksum.chksum[min] != 0xffff ) {
        nvi.min2.min2[min] = 0;
      }
    }
    (void) SKY_PutNV( NV_MIN2_I, &nvi );
#endif

//#if 0
//    nvi.mob_term_home.nam = bNAM;
//    (void) SKY_GetNV( NV_MOB_TERM_HOME_I, &nvi );
//    for( min=0; min<NV_MAX_MINS; min++ ) {
//      if( nvi2.min_chksum.chksum[min] != 0xffff ) {
//        /* default for Home System Registration is yes */
//        nvi.mob_term_home.enabled[min] = FALSE;
//      }
//    }
//    (void) SKY_PutNV( NV_MOB_TERM_HOME_I, &nvi );
//
//    nvi.mob_term_for_sid.nam = bNAM;
//    (void) SKY_GetNV( NV_MOB_TERM_FOR_SID_I, &nvi );
//    for( min=0; min<NV_MAX_MINS; min++ ) {
//      if( nvi2.min_chksum.chksum[min] != 0xffff ) {
//        /* default for Foreign SID Registration is yes */
//        nvi.mob_term_for_sid.enabled[min] = FALSE;
//      }
//    }
//    (void) SKY_PutNV( NV_MOB_TERM_FOR_SID_I, &nvi );
//
//    nvi.mob_term_for_nid.nam = bNAM;
//    (void) SKY_GetNV( NV_MOB_TERM_FOR_NID_I, &nvi );
//    for( min=0; min<NV_MAX_MINS; min++ ) {
//      if( nvi2.min_chksum.chksum[min] != 0xffff ) {
//        /* default for Foreign NID Registration is yes */
//        nvi.mob_term_for_nid.enabled[min] = FALSE;
//      }
//    }
//    (void) SKY_PutNV( NV_MOB_TERM_FOR_NID_I, &nvi );
//#endif //0

    mccreg_clear_reg();   /* Clear out the reg sid nid */  

#ifndef FEATURE_OTASP
    {
      byte esn_last_digit;
      (void) SKY_GetNV( NV_ESN_I, &nvi );
      esn_last_digit = (byte) ( nvi.esn.esn % 10 );
      nvi.accolc.nam = bNAM;
      (void) SKY_GetNV( NV_ACCOLC_I, &nvi );
      for( min=0; min<NV_MAX_MINS; min++ ) {
        if( nvi2.min_chksum.chksum[min] != 0xffff ) {
          nvi.accolc.class[min] = esn_last_digit;
        }
      }
    }
    (void) SKY_PutNV( NV_ACCOLC_I, &nvi );
#endif

#ifndef FEATURE_SSPR_ENHANCEMENTS
    nvi.sid_nid.nam = nam;
    (void) SKY_GetNV( NV_SID_NID_I, &nvi );
    for( min=0; min<NV_MAX_MINS; min++ ) {
      if( nvi2.min_chksum.chksum[min] != 0xffff ) {
        for( nLoop=0; j<NV_MAX_SID_NID; nLoop++) {
          /*  NID default */
          nvi.sid_nid.pair[min][nLoop].sid = 0;
          nvi.sid_nid.pair[min][nLoop].nid = NID_DEFAULTS ;
        }
      }
    }
    (void) SKY_PutNV( NV_SID_NID_I, &nvi );
#endif /* !FEATURE_SSPR_ENHANCEMENTS */

    for( min=0; min<NV_MAX_MINS; min++ ) {
      if( nvi2.min_chksum.chksum[min] != 0xffff ) {
        nvi2.min_chksum.chksum[min] = 0xffff;
      }
    }
    (void) SKY_PutNV( NV_MIN_CHKSUM_I, &nvi2 );
  } /* for nam */
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sGoOutOffline()
{
  if(!SKY_IS_OFFLINE())   
  {
    ui_set_ph_oprt_mode(CM_OPRT_MODE_OFFLINE);  
    SKY_SET_OFFLINE(TRUE);
  }    
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitNAMProgMode()
**
** Description: 
**     
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID sInitNAMProgMode()
{
  UINT8 nNam, nLoop;

#ifdef FEATURE_UI_OTKSL
#error code not present
#endif
  
  for(nNam = 0; nNam < NV_MAX_NAMS; nNam++)
  {
    nvi.nam_chksum.nam = nNam;
    /* NV initializes NV_NAM_CHKSUM_I to 0, so it is active for sure */
    (VOID) SKY_GetNV(NV_NAM_CHKSUM_I, &nvi);

    /* Not the first entrance in NAM Prog Mode */
    if(nvi.nam_chksum.chksum != 0xFFFF)
    {
      for(nLoop = 0; nLoop<NV_SEC_CODE_SIZE; nLoop++)
      {
        nvi.sec_code.digits[nLoop] = '0';
      }
      (VOID) SKY_PutNV(NV_SEC_CODE_I, &nvi);

      for(nLoop = 0; nLoop<NV_LOCK_CODE_SIZE; nLoop++)
      {
        nvi.lock_code.digits[nLoop] = '0';
      }
      (VOID) SKY_PutNV(NV_LOCK_CODE_I, &nvi);
      break;
    }
  }

#ifdef FEATURE_UI_OTKSL
#error code not present
#endif
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sReadPrefSysInfo(UINT8 nNAM)
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     Current prefered system which is converted to UINT8
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
UINT8 sReadPrefSysInfo(UINT8 nNAM)
{
  UINT8 currIndex;

  /* Read Prefered Sys from NV */
   nvi.system_pref.nam = nNAM ;
   /* is it active and in valid range ? */
   if ( NV_NOTACTIVE_S == SKY_GetNV( NV_SYSTEM_PREF_I, &nvi ))
   {
     /* no, so default it */
     nvi.system_pref.nam = nNAM;
     currIndex = nvi.system_pref.sys = NV_SP_B_ONLY;
     (void) SKY_PutNV( NV_SYSTEM_PREF_I, &nvi );
   }
   else
	 currIndex = nvi.system_pref.sys;

   return currIndex;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sinitPrefSysMenu()
**
** Description: 
**     Initialize Prefered System Menu with I18N scroll list.
**
** Input:
**     NONE
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
VOID sinitPrefSysMenu()
{
   pszPreferedSys[0] = (BYTE*) SKY_GET_TOKEN(TKN_NAM_AONLY);
   pszPreferedSys[1] = (BYTE*) SKY_GET_TOKEN(TKN_NAM_BONLY);
   pszPreferedSys[2] = (BYTE*) SKY_GET_TOKEN(TKN_NAM_HOMEONLY);
   pszPreferedSys[3] = (BYTE*) SKY_GET_TOKEN(TKN_NAM_STANDARD);   
} 


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sinitYesNoList()
{
   pszYesNo[0] = (BYTE*) SKY_GET_TOKEN(TKN_NAM_YES); 
   pszYesNo[1] = (BYTE*) SKY_GET_TOKEN(TKN_NAM_NO);
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

//typedef PACKED struct {
//  word      sid;
//  word      nid;
//} nv_sid_nid_pair_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SID+NID pairs for CDMA acquisition, along with NAM id      */
/* NID is 16 bits, per CAI section 2.3.10.3.  There are up to 4 SID+NID    */
/* pairs, in descending preferrence (0=first, 3=last).                     */

//typedef PACKED struct {
//  byte                 nam;                               /* NAM id 0-N */
//  nv_sid_nid_pair_type pair[NV_MAX_MINS][NV_MAX_SID_NID]; /* SID+NID Pair */
//} nv_sid_nid_type;
//#define NV_MAX_HOME_SID_NID 20



BYTE szSIDNID[NUM_SIDNID_LIST+1][20];
void sInitSIDNIDList(UINT8 nNAM)
{ 
  UINT8   i;

  nvi.home_sid_nid.nam = nNAM;      
  SKY_GetNV(NV_HOME_SID_NID_I, &nvi);
  
  for(i = 0; i <= NUM_SIDNID_LIST; i++)
  {
    sprintf((CHAR *)szSIDNID[i], "%ld/%ld", (DWORD)nvi.home_sid_nid.pair[i].sid, 
                                  (DWORD)nvi.home_sid_nid.pair[i].nid);                      
    pszSIDNIDPair[i] = szSIDNID[i];
  }
}







/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sUpdateSIDNIDList(UINT8 nNAM, BYTE* pszBuffer, WORD nIndex)
{
  nvi.home_sid_nid.nam = nNAM;
  SKY_GetNV(NV_HOME_SID_NID_I, &nvi);

  sprintf((CHAR*)pszBuffer, "%d/%d", nvi.home_sid_nid.pair[nIndex].sid,
                                     nvi.home_sid_nid.pair[nIndex].nid);
}


//#if (MODEL_ID == MODEL_B0)
//BOOL sInitInputCtrlInNAMMSM3100(UINT8 nNAM)
//{
//   STATIC HCONTROL  hControl;
//   STATIC CHAR      pszNAMInfo[EQS_SIZE_NAM_BUFFER];
//   UINT8            nCtrl;
//   UINT8            nInitLine, currIndex;
//   WORD             nLogicalLine;
//
//   nInitLine      =   NUM_NAM_PREF_SYS;
//   nLogicalLine   =   NUM_NAM_PREF_SYS;
//   currIndex      =   0;          // current prefered system 
//                                  // this value will be read from NV
//   sinitPrefSysMenu();       
//     
//   switch (nNAM)
//   {
//    case NAM1:
//       hControl = EQC_StaticText(0, 0,TKN_NAM_1);
//        break;
//    case NAM2:
//       hControl = EQC_StaticText(0, 0,TKN_NAM_2);
//        break;
//    case NAM3:
//       hControl = EQC_StaticText(0, 0,(BYTE*)EQS_GET_TOKEN(TKN_NAM_3));
//        break;
//    case NAM4:
//       hControl = EQC_StaticText(0, 0,(BYTE*)EQS_GET_TOKEN(TKN_NAM_4));
//        break;
//   }
//
//   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_CENTER | CS_STATIC_TEXT_CAPTION );
//   EQC_SET_DX( hControl, EQS_GetMaxWidth() );
//
//   /* Read phone number from NV */
//   EQC_StaticText(0, 0, (BYTE*)EQS_GET_TOKEN(TKN_NAM_PHONENUMBER));
//   sReadNAMItemsFromNV(PHONENO_I, nNAM, pszNAMInfo);
//   EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , (BYTE *)pszNAMInfo);
//
//   /* Read CDMA 1st Channel from NV */
//   EQC_StaticText(0, 0, (BYTE*)EQS_GET_TOKEN(TKN_NAM_CDMA_1STCHANN));  
//   sReadNAMItemsFromNV(CDMA_PRICHNB_I, nNAM, pszNAMInfo);
//   EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , (BYTE*)pszNAMInfo);     
//
//   /* Read CDMA 2nd Channel from NV */
//   EQC_StaticText(0, 0, (BYTE*)EQS_GET_TOKEN(TKN_NAM_CDMA_2NDCHANN));  
//   sReadNAMItemsFromNV(CDMA_SECCHNB_I, nNAM, pszNAMInfo);
//   EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , (BYTE*)pszNAMInfo );
// 
//   /* Read SID from NV */  
//   EQC_StaticText(0, 0,(BYTE*)EQS_GET_TOKEN(TKN_NAM_SID));
//   sReadNAMItemsFromNV(CDMA_HOMESID_I, nNAM, pszNAMInfo);
//   EQC_NumberEdit(0, 0, NAM_SHORT_INPUT_CONTROL_WIDTH, 1 , (BYTE*)pszNAMInfo );
//
//   /* Read NID from NV */
//   EQC_StaticText(0, 0,(BYTE*)EQS_GET_TOKEN(TKN_NAM_NID));  
//   sReadNAMItemsFromNV(CDMA_HOMENID_I, nNAM, pszNAMInfo);
//   EQC_NumberEdit(0, 0, NAM_SHORT_INPUT_CONTROL_WIDTH, 1 , (BYTE*)pszNAMInfo );
//   
//   
//   currIndex = sReadPrefSysInfo(nNAM);
//   hControl = EQC_ScrollList(0, 0, 16, VIEW_HEIGHT-1, pszPreferedSys, 
//                                           nInitLine, nLogicalLine, currIndex);
//   EQC_SetScrollTitle(hControl, (BYTE*)EQS_GET_TOKEN(TKN_NAM_PREF_SYS_TITLE));
//   EQC_ShowScrollTitle(hControl, TRUE);
//
//
//   EQS_StartFormatter();
//   for(nCtrl = 0; nCtrl < NAE_MAX_CONTROL; nCtrl ++)
//    {
//       EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
//       
//       if((nCtrl != NAE_SID_TITLE) && (nCtrl != NAE_NID_TITLE))
//       {
//        EQS_ENDLine();
//       }
//    }
//   EQS_ENDFormatter();
//
//
//   return TRUE;
//}
//#endif /* (MODEL_ID == MODEL_B0) */
///* <-- */



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitInputControlNAM()
**
** Description: 
**     Initialize input controls
**     Phone Number, CDMA SID_NID, CDMA 1st Channel, CDMA 2nd Channel,
**     System Select
**
** Input:
**     NONE
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
//BOOL sInitInputCtrlInNAM(UINT8 nNAM, UINT8 nITEM)
//{
//   HCONTROL         hControl;
//   STATIC CHAR      pszNAMInfo[EQS_SIZE_NAM_BUFFER];
//   UINT8            nCtrl;
//   UINT8            currIndex, nMAXCtrl;
//
//   currIndex      =   0;          // current prefered system 
//                                  // this value will be read from NV
//   sInitBuf(pszNAMInfo, EQS_SIZE_NAM_BUFFER);
//
//   /* Read phone number from NV */
//
//   switch (nITEM)
//   {
//   case PHONENO_I:
//     EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_PHONENUMBER);
//     sReadNAMItemsFromNV(PHONENO_I, nNAM, pszNAMInfo);
//	 hControl  = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
//                                DIAL_DIGIT_POS_Y, 
//                                DIAL_DIGIT_LENGTH_PER_LINE, 
//                                1, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT | CS_NUMBER_EDIT_BORDER);
//     EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
//     EQS_SetIOCActive(hControl);
//     nMAXCtrl = NP_MAX;
//     break;

//   case CDMA_PRICHNA_I:
//     /* hjcho 2000-11-28 --> @why, what */
//#if (MODEL_ID == MODEL_B0) 
//     /* MSM3100을 사용하는 모델에서는 각 하나씩의 Primary Channel과    */
//     /* Secondary Channel을 가진다.                                    */
//     /* 각 항목에 대한 enum type으로 CDMA_PRICHNA_I,CDMA_SECCHNA_I를   */
//     /* 사용한다.                                                      */
//     /* Read CDMA 1st Channel from NV */
//     EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_CDMA_1STCHANN);  
//     sReadNAMItemsFromNV(CDMA_PRICHNA_I, nNAM, pszNAMInfo);
//     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)pszNAMInfo);     
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);
//     sInitBuf(pszNAMInfo, EQS_SIZE_NAM_BUFFER);
//
//     /* Read CDMA 2nd Channel from NV */
//     EQC_StaticText(0, 0, TKN_NAM_CDMA_2NDCHANN);  
//     sReadNAMItemsFromNV(CDMA_SECCHNA_I, nNAM, pszNAMInfo);
//     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)pszNAMInfo);   
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);
//#elif (MODEL_ID == MODEL_B1)
//     /* Read CDMA 1st Channel A from NV */
//     EQC_StaticText(0, 0, TKN_NAM_CDMA_1STCHANNA);
//     sReadNAMItemsFromNV(CDMA_PRICHNA_I, nNAM, pszNAMInfo);
//     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)pszNAMInfo);
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);
//     sInitBuf(pszNAMInfo, EQS_SIZE_NAM_BUFFER);
//
//     /* Read CDMA 2nd Channel A from NV */
//     EQC_StaticText(0, 0,TKN_NAM_CDMA_2NDCHANNA);
//     sReadNAMItemsFromNV(CDMA_SECCHNA_I, nNAM, pszNAMInfo);
//     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)pszNAMInfo);
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);
//     sInitBuf(pszNAMInfo, EQS_SIZE_NAM_BUFFER);
//
//     /* Read CDMA 1st Channel B from NV */
//     EQC_StaticText(0, 0, TKN_NAM_CDMA_1STCHANNB);
//     sReadNAMItemsFromNV(CDMA_PRICHNB_I, nNAM, pszNAMInfo);
//     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)pszNAMInfo);
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);
//     sInitBuf(pszNAMInfo, EQS_SIZE_NAM_BUFFER);
//
//     /* Read CDMA 2nd Channel B from NV */
//     EQC_StaticText(0, 0, TKN_NAM_CDMA_2NDCHANNB);
//     sReadNAMItemsFromNV(CDMA_SECCHNB_I, nNAM, pszNAMInfo);
//     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)pszNAMInfo);     
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);
//#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)*/
//     /* <-- */
//     nMAXCtrl = NC_MAX;
//     break;

//   case CDMAHOMESIDNID_I:
//     /*               Read SID NID set from NV             */
//     /*               Implemented by Scroll list           */   
//     /* hjcho 2000-11-28 --> @why, what */
//#if (MODEL_ID == MODEL_B0) 
//     EQC_StaticText(0, 0, TKN_NAM_SIDNID);
//     EQC_StaticText(0, 0, TKN_NAM_SID);
//     EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1, 34, (BYTE*)"2222");
//     EQC_StaticText(0, 0, TKN_NAM_NID);
//     EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1, 34, (BYTE*)"65535");
//#elif (MODEL_ID == MODEL_B1)
//     /* NV_HOME_SID_NID_I 이용    */
//     /* 20개의 쌍을 보여주어야 함 */
//     sInitSIDNIDList(nNAM);
//     nInitLine     =  NUM_SIDNID_LIST + 1;
//     nLogicalLine  =  NV_MAX_HOME_SID_NID;
//     currIndex = 0;
//     hControl  = EQC_ScrollList(EQS_NA, EQS_NA, 16, NUM_SIDNID_LIST, pszSIDNIDPair,
//                                nInitLine, nLogicalLine, currIndex);
//     EQC_SetLogicalList(hControl, TRUE); // IO 이벤트를 받을 수 있다.
//     EQC_SET_STYLE(hControl, 
//		           CS_SCROLL_LIST_BORDER | 
//				   CS_SCROLL_LIST_ENUMBERATED | 
//				   CS_SCROLL_LIST_HMAXIMIZE);     
//     EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_NAM_HOMESIDNID));
//     EQC_ShowScrollTitle(hControl, TRUE);
//     EQS_SetIOCActive(hControl);
//#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)*/
//     /* <-- */
//     nMAXCtrl = NS_MAX;
//     break;

//   case PREFSYS_I:     
//      sinitPrefSysMenu();
//      currIndex = sReadPrefSysInfo(nNAM);
//      hControl  = EQC_ScrollList(EQS_NA, EQS_NA, 16, VIEW_HEIGHT, pszPreferedSys, 
//                                           NUM_NAM_PREF_SYS, NUM_NAM_PREF_SYS, currIndex);
//	  EQC_SET_STYLE(hControl, CS_SCROLL_LIST_HMAXIMIZE);
//      EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_NAM_PREF_SYS_TITLE));
//      EQC_ShowScrollTitle(hControl, TRUE);
//      EQS_SetIOCActive(hControl);
//      nMAXCtrl = NPS_MAX;
//     break;

//   case CDMAMCC_I:
//      /* Read MCC from NV  implemented by Number Edit */
//      EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_MCC);
//      sReadNAMItemsFromNV(CDMAMCC_I, nNAM, pszNAMInfo);
//      hControl  = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
//                                 DIAL_DIGIT_POS_Y, 
//                                 DIAL_DIGIT_LENGTH_PER_LINE, 
//                                 1, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
//      EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT | CS_NUMBER_EDIT_BORDER);
//      EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
//      EQS_SetIOCActive(hControl);
//      nMAXCtrl = NM_MAX;
//     break;
//   
//   
//   case CDMAMNC_I:
//     /* Read MNC from NV  implemented by Number Edit */
//      EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_MNC);
//      sReadNAMItemsFromNV(CDMAMNC_I, nNAM, pszNAMInfo);
//      hControl  = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
//                                 DIAL_DIGIT_POS_Y, 
//                                 DIAL_DIGIT_LENGTH_PER_LINE, 
//                                 1, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
//      EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT | CS_NUMBER_EDIT_BORDER);
//      EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
//      EQS_SetIOCActive(hControl);      
//      nMAXCtrl = NMN_MAX;
//     break;

//   case PRL_ENABLED_I:
//   /* Read PRL from NV  implemented by Combo Type Scroll List   */
//      nvi.prl_enabled.nam = nNAM;
//      SKY_GetNV(NV_PRL_ENABLED_I, &nvi);
//
//      if(nvi.prl_enabled.enabled == TRUE) currIndex = 0;
//      else                                currIndex = 1;
//      
//      sinitYesNoList();
//      hControl = EQC_ScrollList(0, 0, 16, VIEW_HEIGHT-2, pszYesNo,
//                                          NUM_YESNO_LIST, NUM_YESNO_LIST, currIndex);
//      EQC_SetScrollTitle(hControl, (BYTE*) SKY_GET_TOKEN(TKN_NAM_PRL));
//      EQC_ShowScrollTitle(hControl, TRUE);
//      EQS_SetIOCActive(hControl);
//      nMAXCtrl = NPE_MAX;
//      break;

//   default : 
//       nMAXCtrl = 0;
//   }
//
//   EQS_StartFormatter();
//   for(nCtrl = 0; nCtrl <nMAXCtrl  ; nCtrl ++)
//   {
//     EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
//     EQS_NewLine();
//   }
//   EQS_EndFormatter();
//   sInitBuf(pszNAMInfo, EQS_SIZE_NAM_BUFFER);
//
//   return TRUE;
//}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_SCI()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sMakeTitle(UINT8 nNAM, BYTE pszNAMInfo[])
{
  switch (nNAM)
  {
    case NAM1:
	   sprintf((CHAR *)pszNAMInfo, "%s", (CHAR *)SKY_GET_TOKEN(TKN_NAM_1));
        break;
    case NAM2:
	   sprintf((CHAR *)pszNAMInfo, "%s", (CHAR *)SKY_GET_TOKEN(TKN_NAM_2));
        break;
    case NAM3:
	   sprintf((CHAR *)pszNAMInfo, "%s", (CHAR *)SKY_GET_TOKEN(TKN_NAM_3));
        break;
    case NAM4:
	   sprintf((CHAR *)pszNAMInfo, "%s", (CHAR *)SKY_GET_TOKEN(TKN_NAM_4));
        break;
	case NAM5:
	   sprintf((CHAR *)pszNAMInfo, "%s", (CHAR *)SKY_GET_TOKEN(TKN_NAM_5));
	   break;
   }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_SCI()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API SKA_SCI(SKY_EVENT_T* pEvent)
{
    STATIC HCONTROL  hControl;
	BYTE             spszNAMInfo[EQS_SIZE_NAM_BUFFER];
    STATIC HCONTROL  hCtrlConfirm, hCtrlFail;

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               SKY_GET_TOKEN(TKN_NAM_SCI), 
               WA_SHOW_CML|WA_SHOW_HEADER);

        sReadNAMItemsFromNV(SLOTIDX_I, EQS_NA, (CHAR*)spszNAMInfo);
	    hControl  = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                                DIAL_DIGIT_POS_Y, 
                                DIAL_DIGIT_LENGTH_PER_LINE, 
                                1, MAX_DIAL_DIGIT_LENGTH, (BYTE*)spszNAMInfo);
        EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT|CS_NUMBER_EDIT_BORDER|
                                CS_NUMBER_EDIT_ALONE | CS_NUMBER_EDIT_CURSOR);
        EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
        EQS_SetIOCActive(hControl);
//        EQS_FitCanvasSize();
        break;

    case SE_APP_END : /* The very last event */
        EQS_ReleaseDC();
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;
    case SE_APP_DRAW :  /* draw */
        break;

    case SE_KEY_END:
    case SE_IO_NEDIT_CANCEL :
    case SE_SOFTKEY_CANCEL  :
        /* restore original number */
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_End();
        }
        break;


    case SE_IO_NEDIT_DONE :
    case SE_SOFTKEY_SAVE :
        EQC_GetNumberEditValue(EQS_GetNthIOC(0), (BYTE*)spszNAMInfo);

        if(sIsValidData(spszNAMInfo))
        {
            sSaveNAMItems2NV(EQS_NA, SLOTIDX_I, (CHAR*)spszNAMInfo);
            hControl = EQC_MessageBox((BYTE*) "저장중 입니다.", MESSAGE_DEFAULT_TIME, 
                                               MBT_NONE, MBT_SUCCESS);
        }
        else
        {
            hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_WRONG_INPUT), MESSAGE_DEFAULT_TIME, 
                                                 MBT_NONE, MBT_FAIL);
        }
        //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
        break;


    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(0));
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }
        break;

    case SE_IO_MESGBOX_NO:
//        if(fEndToRoot == FALSE)
//        {
            EQS_END();
//        }
//        else
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
        break;

    case SE_IO_MESGBOX_END:  
        if(pEvent->wParam == hControl)
        {
//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
        else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        }
       break;


    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Rturn value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_SCM(SKY_EVENT_T* pEvent)
{
   STATIC BYTE        spszNAMInfo[EQS_SIZE_NAM_BUFFER];
   STATIC HCONTROL    hControl;
   STATIC HCONTROL    hCtrlConfirm, hCtrlFail;
  
  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               SKY_GET_TOKEN(TKN_NAM_SCM), 
               WA_SHOW_CML|WA_SHOW_HEADER);

        sReadNAMItemsFromNV(SCM_I, EQS_NA, (CHAR*)spszNAMInfo);
	    hControl  = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                                DIAL_DIGIT_POS_Y, 
                                DIAL_DIGIT_LENGTH_PER_LINE, 
                                1, MAX_DIAL_DIGIT_LENGTH, (BYTE*)spszNAMInfo);
        EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT|CS_NUMBER_EDIT_BORDER|
                                CS_NUMBER_EDIT_ALONE|CS_NUMBER_EDIT_CURSOR);
        EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
        EQS_SetIOCActive(hControl);
//        EQS_FitCanvasSize();
        break;

    case SE_APP_END : /* The very last event */
        EQS_ReleaseDC();
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;
    case SE_APP_DRAW :    /* draw */
        break;

    case SE_KEY_END:
    case SE_IO_NEDIT_CANCEL :
    case SE_SOFTKEY_CANCEL  :
        /* restore original number */
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_End();
        }
        break;

    case SE_IO_NEDIT_DONE :
    case SE_SOFTKEY_SAVE :
        EQC_GetNumberEditValue(EQS_GetNthIOC(0), (BYTE*)spszNAMInfo);
        if(sIsValidData(spszNAMInfo))
        {
    		sSaveNAMItems2NV(EQS_NA, SCM_I, (CHAR*)spszNAMInfo);
            hControl = EQC_MessageBox((BYTE*) "저장중 입니다.", MESSAGE_DEFAULT_TIME, 
                                               MBT_NONE, MBT_SUCCESS);
        }
        else
        {
             hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_WRONG_INPUT), MESSAGE_DEFAULT_TIME, 
                                                 MBT_NONE, MBT_FAIL);
        }
        //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(0));
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }
        break;

    case SE_IO_MESGBOX_NO:
//        if(fEndToRoot == FALSE)
//        {
            EQS_END();
//        }
//        else
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
        break;

    case SE_IO_MESGBOX_END:  
        if(pEvent->wParam == hControl)
        {
//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
        else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(0));
        }
       break;


    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


#ifdef FEATURE_Q_PAGE
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define QPAGE_MAX_MENU_LENGTH   15
#define QPAGE_MAX_MENU_LINE     2

enum tagQPAGE_SUPPORTED_E    /*시스템선택*/
{
  QS_TITLE = 0,           
  QS_MENU,     
  QS_MAX
};



LOCAL void sInitQPageSupportedDisplay(void)
{
   UINT8    nCurrIndex;
   HCONTROL hControl;
   UINT8    nCtrl;

   /* Read PRL from NV  implemented by Combo Type Scroll List   */
   //nvi.prl_enabled.nam = nNAM;
   //SKY_GetNV(NV_PRL_ENABLED_I, &nvi);

   //if(nvi.prl_enabled.enabled == TRUE) nCurrIndex = 0;
   //else                                nCurrIndex = 1;
      
   EQC_StaticText(EQS_NA, EQS_NA, TKN_Q_PAGE);
   sinitYesNoList();

   hControl = EQC_RadioButton(EQS_NA, EQS_NA, QPAGE_MAX_MENU_LENGTH,
	                          QPAGE_MAX_MENU_LINE, (BYTE **)pszYesNo, nCurrIndex);
   EQC_SET_STYLE(hControl, 
                (EQC_GET_STYLE(hControl) & ~CS_RADIO_BUTTON_BORDER));
   EQS_SetIOCActive(hControl);
   
   EQS_StartFormatter();

   for(nCtrl = 0; nCtrl < QS_MAX; nCtrl ++)
   {
     EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);              
     EQS_NewLine();
   }
   EQS_EndFormatter();
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     VOID SKY_API SKA_QPAGE(SKY_EVENT_T* pEvent)
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_QPAGE(SKY_EVENT_T* pEvent)
{
    STATIC UINT8 nNAM;
    HCONTROL  hControl;

    
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_SELECT_NONE, 
               pszNAMInfo, 
               (WA_SHOW_CML|WA_SHOW_HEADER));
 	  sInitQPageSupportedDisplay();
        break;

    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;
    case SE_APP_DRAW :  /* draw */
        break;
    
    case SE_IO_RADIO_BUTTON_SELECT:
        //nvi.prl_enabled.nam = nNAM;
        if(EQC_GetRadioButton(EQS_GetNthIOC(QS_MENU)) == 0)
        {
            /*RADIO Button으로부터 입력된 값을 읽어서 변수에 저장 */
        }
        else
        {
            /*RADIO Button으로부터 입력된 값을 읽어서 변수에 저장 */
        }

		EQS_SetIOCActive(EQS_GetNthIOC(QS_MENU));

        /* 변수로 옮겨진 값을 NV item에 저장한다 */
        /* SKY_PutNV(NV_PRL_ENABLED_I,&nvi);     */
        hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NAM_SAVE_END), MESSAGE_DEFAULT_TIME, 
                                   MBT_NONE, MBT_SUCCESS);
        break;


	case SE_KEY_END:
        if(EQS_IsControlDataChanged())
		{
		  hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NAM_SAVE_CONFIRM),
		 	                        MESSAGE_DEFAULT_TIME,
								    MBT_YESNO, MBT_CONFIRM);		}
		else
		{
		  EQS_END();
		}
		break;

		
    case SE_IO_RADIO_BUTTON_CANCEL:
    	if(EQS_IsControlDataChanged())
		{
  		  hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NAM_SAVE_CONFIRM),
		 	                        MESSAGE_DEFAULT_TIME,
								    MBT_YESNO, MBT_CONFIRM);
		  //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
		}
		else
		{
		  EQS_END();
		}
        break;


    case SE_IO_MESGBOX_YES:
		EQS_SEND_EVENT(SE_IO_RADIO_BUTTON_SELECT, EQS_NA, EQS_NA);
		break;

    case SE_IO_MESGBOX_END:
    case SE_IO_MESGBOX_NO:
		 EQS_END();
		break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif// FEATURE_Q_PAGE




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainNAM()
**
** Description: 
**     
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API sSKA_EnterNAM(SKY_EVENT_T* pEvent)
{

   switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_SetIOCActive(EQC_MenuList(EQS_NA, EQS_NA, EQS_NA, EQS_NA, &mnNAMProgTop, 0, SKY_GET_SEL_MENU_MODE()));
        break;

    case SE_APP_END :     /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_APP_DRAW :    /* draw */
        break;
 
    case SE_KEY_END :
       EQS_END();
      // Reset 되고 저장되지 않은 NAM data는 
      // 기존값으로 Setting되어야 한다.
      break;
    
    case SE_IO_MENU_LIST_SELECT:
      EQS_START_CHILD((EQS_APPLET*)pEvent->dwParam);
      break;

    case SE_IO_MENU_LIST_CANCEL:
       EQS_END();
      break;
    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_EnterSimpleNAM()
**
** Description: 
**     
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API sSKA_EnterSimpleNAM(SKY_EVENT_T* pEvent)
{
   HCONTROL hControl;

   switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        //hControl = EQC_MenuList(EQS_NA, EQS_NA, EQS_NA, EQS_NA, &mnSimpleNAMProgTop, 0,SKY_GET_SEL_MENU_MODE());
        //EQS_SetIOCActive(hControl);
        EQS_START_CHILD_EVENT(SKA_PhoneNumber, SE_APP_START, EQS_NA, INPUT_FROM_SIMPLE_NAM);
        break;

    case SE_APP_END :     /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        //EQS_SetIOCActive(EQS_GetNthIOC(0));
        EQS_END();
        break;

    case SE_APP_DRAW :    /* draw */
        break;
 
    case SE_KEY_END :
      break;
    

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}






/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_InitNAM()
**
** Description: 
**     Initialize some parameters to enter NAM Prog Mode
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_InitNAM(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);

        break;
    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        EQS_END();
        break;
    case SE_APP_DRAW :  /* draw */
        break;

    case SE_IO_MESGBOX_END :
        sInitNAMProgMode();
        SKY_StopTone();
        sCheckUninitializedItems(); /* check for uninitizalized stuff */
        EQS_START_CHILD((EQS_APPLET*)sSKA_EnterNAM);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_InitNAM()
**
** Description: 
**     Initialize some parameters to enter NAM Prog Mode
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_InitSimpleNAM(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);

        break;
    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        EQS_END();
        break;
    case SE_APP_DRAW :  /* draw */
        break;

    case SE_IO_MESGBOX_END :
        sInitNAMProgMode();
        SKY_StopTone();
        sCheckUninitializedItems(); /* check for uninitizalized stuff */
        EQS_START_CHILD((EQS_APPLET*)sSKA_EnterSimpleNAM);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_ReturnMainNAM(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
         EQS_END();
        break;
    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;
    case SE_APP_DRAW :  /* draw */
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}






/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKAMainNAM
**
** Description: 
**     Initialize to enter NAM program mode
**
** Input:
**     NONE
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_MainNAM(SKY_EVENT_T* pEvent)
{
  STATIC HCONTROL hCtrlSuccess, hCtrlFail;
  switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START :  /* The very first event */
        EQS_GetDC(TRUE);
        sGoOutOffline(); /* Go out Offline State */
        hCtrlSuccess = EQC_MessageBox((BYTE*)"Please Wait Service Program", 
			                                          MESSAGE_DEFAULT_TIME, 
													  MBT_NONE, MBT_HELP);
        break;

    case SE_APP_END :     /* The very last event */
         EQS_ReleaseDC();
        break;
 
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
		if(!SKY_IS_OFFLINE())
		{
		  hCtrlFail = EQC_MessageBox((BYTE*) "Fail to go offline state",
			                                 MESSAGE_DEFAULT_TIME,
											 MBT_NONE, MBT_HELP);
		}
		else
		{
		  EQS_END();
		}
        break;

    case SE_SYS_TO_IDLE:  /* Ignore this event */
        break;

    case SE_IO_MESGBOX_END:
        if(SKY_IS_OFFLINE() && (pEvent->wParam == hCtrlSuccess))
        {
          //EQS_START_SIBLING_CURRENT_EVENT((EQS_APPLET*)sSKA_InitNAM, TRUE);
		  EQS_START_CHILD_CURRENT_EVENT((EQS_APPLET*)sSKA_InitNAM);
        }
        else if(!SKY_IS_OFFLINE() && (pEvent->wParam == hCtrlSuccess))
        {          
          //EQS_START_SIBLING_CURRENT_EVENT(sSKA_ReturnMainNAM, TRUE);
          EQS_START_CHILD_CURRENT_EVENT(sSKA_ReturnMainNAM);
        }
		else if(pEvent->wParam == hCtrlFail)
		{
		  EQS_END();
		}
        break;  

    case SE_KEY_END :
        EQS_END();
        break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKAMainNAM
**
** Description: 
**     Initialize to enter NAM program mode
**
** Input:
**     NONE
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_MainSimpleNAM(SKY_EVENT_T* pEvent)
{
  STATIC HCONTROL hCtrlSuccess, hCtrlFail;
  switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START :  /* The very first event */
        EQS_GetDC(TRUE);
        sGoOutOffline(); /* Go out Offline State */
        hCtrlSuccess = EQC_MessageBox((BYTE*)"Please Wait Service Program", 
			                          MESSAGE_DEFAULT_TIME, 
									  MBT_NONE, MBT_HELP);
        break;

    case SE_APP_END :     /* The very last event */
         EQS_ReleaseDC();
        break;
 
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
		if(!SKY_IS_OFFLINE())
		{
		  hCtrlFail = EQC_MessageBox((BYTE*) "Fail to go offline state",
			                                 MESSAGE_DEFAULT_TIME,
											 MBT_NONE, MBT_HELP);
		}
		else
		{
		  EQS_END();
		}
        break;

    case SE_SYS_TO_IDLE:  /* Ignore this event */
        break;

    case SE_IO_MESGBOX_END:
        if(SKY_IS_OFFLINE() && (pEvent->wParam == hCtrlSuccess))
        {
          //EQS_START_SIBLING_CURRENT_EVENT((EQS_APPLET*)sSKA_InitNAM, TRUE);
		  EQS_START_CHILD_CURRENT_EVENT((EQS_APPLET*)sSKA_InitSimpleNAM);
        }
        else if(!SKY_IS_OFFLINE() && (pEvent->wParam == hCtrlSuccess))
        {          
          //EQS_START_SIBLING_CURRENT_EVENT(sSKA_ReturnMainNAM, TRUE);
          EQS_START_CHILD_CURRENT_EVENT(sSKA_ReturnMainNAM);
        }
		else if(pEvent->wParam == hCtrlFail)
		{
		  EQS_END();
		}
        break;  

    case SE_KEY_END :
        EQS_END();
        break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sInitPhoneNumberDisplay(UINT8 nNAM)
{
   HCONTROL hControl;
   BYTE spszNAMInfo[EQS_SIZE_NAM_BUFFER];

   EQC_StaticText(NAM_MESG_START_XPOS, 1, TKN_NAM_PHONENUMBER);
   sReadNAMItemsFromNV(PHONENO_I, nNAM, (CHAR*)spszNAMInfo);
   hControl  = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              1, MAX_DIAL_DIGIT_LENGTH,(BYTE*)spszNAMInfo);
   EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT|CS_NUMBER_EDIT_BORDER|
                           CS_NUMBER_EDIT_ALONE|CS_NUMBER_EDIT_CURSOR);
   EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
   EQS_SetIOCActive(hControl);
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API SKA_PhoneNumber(SKY_EVENT_T* pEvent)
{
    
    STATIC  UINT8       nNAM;
    STATIC  HCONTROL    hCtrlSuccess, hCtrlFail, hCtrlConfirm;
    STATIC  BOOL        fFromSimpleNAM/*,fEndToRoot*/;
    STATIC  BYTE        pszTitleName[EQS_SIZE_NAM_BUFFER];
    
  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      if(pEvent->dwParam == INPUT_FROM_SIMPLE_NAM)
      {
          fFromSimpleNAM = TRUE;
          sprintf((CHAR *)pszTitleName, "%s", "Simple Mode");    
          nNAM = (BYTE)SKY_GET_CURRENT_NAM();
          if(nNAM >= NV_MAX_NAMS) nNAM = NAM1;
      }
      else
      {
          fFromSimpleNAM = FALSE;
          nNAM = pEvent->wParam;
          sMakeTitle(nNAM, pszTitleName);
      }   

      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               pszTitleName, 
               WA_SHOW_CML|WA_SHOW_HEADER);
      
      //fEndToRoot = FALSE;
         
      sInitPhoneNumberDisplay(nNAM);
        break;

    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;
    case SE_APP_DRAW :  /* draw */
        break;
    
    case SE_IO_NEDIT_CANCEL :
    case SE_SOFTKEY_CANCEL  :
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_End();
        }
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(NP_PHONENUMBER));
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }
        break;

    case SE_IO_MESGBOX_NO:
//        if(fEndToRoot == FALSE)
//        {
            EQS_END();
//        }
//        else
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
        break;

    case SE_IO_NEDIT_DONE :	
    case SE_SOFTKEY_SAVE :
       if(fFromSimpleNAM == FALSE)
       {
            EQC_GetNumberEditValue(EQS_GetNthIOC(NP_PHONENUMBER), (BYTE*)pszNAMInfo);
            if(sIsValidMIN((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo))
            {
                hCtrlSuccess = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_SAVING), 0, 
                                                         MBT_NONE, MBT_SUCCESS);
            }
            else
            {
                hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_TRY_AGAIN), MESSAGE_DEFAULT_TIME, 
                                                 MBT_NONE, MBT_FAIL);                   
            }
       }
       else if(fFromSimpleNAM == TRUE)
       {
           /* 단순모드 번호 입력시 NAM1에는 STI번호가, NAM2에는 SKT번호가 입력되면 안된다 */
            EQC_GetNumberEditValue(EQS_GetNthIOC(NP_PHONENUMBER), (BYTE*)pszNAMInfo);

           if( sIsValidMIN((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo) && 
               sIs011((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo))
           {
              hCtrlSuccess = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_SAVING), 0, 
                                                    MBT_NONE, MBT_SUCCESS);
              nNAM = NAM1;
              break;
           }
           else if(sIsValidMIN((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo) && 
                   sIs017((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo))
           {
              hCtrlSuccess = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_SAVING), 0, 
                                                    MBT_NONE, MBT_SUCCESS);
              nNAM = NAM2;
              break;
           }
           else if(sIsValidMIN((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo) && 
                   sIs010((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo))
           {
              hCtrlSuccess = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_SAVING), 0, 
                                                    MBT_NONE, MBT_SUCCESS);
              nNAM = SKY_GET_CURRENT_NAM();
              break;
           }
           else
           {
               hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_TRY_AGAIN), MESSAGE_DEFAULT_TIME, 
                                                 MBT_NONE, MBT_FAIL);
           }
       }
       //if((nNAM == NAM1) && sIsValidMIN((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo))
       //{
       //     /* 011번호가 들어왔을 경우 */
       //     hCtrlSuccess = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_SAVING), 0, 
       //                                          MBT_NONE, MBT_SUCCESS);
       //     break;
       //}
       //else if((nNAM == NAM2) && sIsValidMIN((UINT)STRLEN((BYTE*)pszNAMInfo), (CHAR*)pszNAMInfo))
       //{
       //     /* 017번호가 들어왔을 경우 */
       //     hCtrlSuccess = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_SAVING), 0, 
       //                                          MBT_NONE, MBT_SUCCESS);
       //     break;
       //}
       //else if((nNAM == NAM1) || (nNAM == NAM2))
       //{
       //     /* NAM1에 017이 아닌 번호가 들어 왔을 경우 또는 NAM2에 011이  */
       //     /* 입력되었을 경우                                            */
       //     hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_TRY_AGAIN), 0, 
       //                                          MBT_NONE, MBT_FAIL);            
       //     break;
       //}
       //else
       //{
       //     hCtrlSuccess = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_SAVING), 0, 
       //                                          MBT_NONE, MBT_SUCCESS);
       //     break;
       //}
       break;


    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hCtrlSuccess)
        {
            /* 적절한 번호가 입력된 경우 */
            EQC_GetNumberEditValue(EQS_GetNthIOC(NP_PHONENUMBER), (BYTE*)pszNAMInfo);
            sSaveNAMItems2NV(nNAM, PHONENO_I, (CHAR*)pszNAMInfo);

            /* 단순모드에서 전화번호를 입력했을 경우, NAM1과 NAM2에 한 해 자동적으로 */
            /* Current NAM에 대한 설정이 바뀐다.                                     */
            if(fFromSimpleNAM == TRUE)
            {
                if((nNAM == NAM1) || (nNAM == NAM2))
                {
                    SKY_SET_CURRENT_NAM(nNAM);
                    nvi.curr_nam = (BYTE)SKY_GET_CURRENT_NAM();
                    SKY_PutNV(NV_CURR_NAM_I, &nvi);
                }
            }

//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
        else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(NP_PHONENUMBER));
        }
        break;

    case SE_KEY_END :
//        fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_END();
        }
       break;
    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sInitChannelDisplay(UINT8 nNAM)
{
	UINT8    nMAXCtrl, nCtrl;
	HCONTROL hControl;
	BYTE     spszNAMInfo[EQS_SIZE_NAM_BUFFER];

//#if (MODEL_ID == MODEL_B0) 
//     /* MSM3100을 사용하는 모델에서는 각 하나씩의 Primary Channel과    */
//     /* Secondary Channel을 가진다.                                    */
//     /* 각 항목에 대한 enum type으로 CDMA_PRICHNA_I,CDMA_SECCHNA_I를   */
//     /* 사용한다.                                                      */
//     /* Read CDMA 1st Channel from NV */
//     EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_CDMA_1STCHANN);  
//     sReadNAMItemsFromNV(CDMA_PRICHNA_I, nNAM, spszNAMInfo);
//     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)spszNAMInfo);     
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_CURSOR);
//     sInitBuf(spszNAMInfo, EQS_SIZE_NAM_BUFFER);
//
//     /* Read CDMA 2nd Channel from NV */
//     EQC_StaticText(0, 0, TKN_NAM_CDMA_2NDCHANN);  
//     sReadNAMItemsFromNV(CDMA_SECCHNA_I, nNAM, spszNAMInfo);
//     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)spszNAMInfo);   
//     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_CURSOR);
//#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
     /* Read CDMA 1st Channel A from NV */
     EQC_StaticText(0, 0, TKN_NAM_CDMA_1STCHANNA);
     sReadNAMItemsFromNV(CDMA_PRICHNA_I, nNAM, (CHAR*)spszNAMInfo);
     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)spszNAMInfo);
     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_CURSOR);
     sInitBuf((CHAR*)spszNAMInfo, EQS_SIZE_NAM_BUFFER);

     /* Read CDMA 2nd Channel A from NV */
     EQC_StaticText(0, 0,TKN_NAM_CDMA_2NDCHANNA);
     sReadNAMItemsFromNV(CDMA_SECCHNA_I, nNAM, (CHAR*)spszNAMInfo);
     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)spszNAMInfo);
     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_CURSOR);
     sInitBuf((CHAR*)spszNAMInfo, EQS_SIZE_NAM_BUFFER);

     /* Read CDMA 1st Channel B from NV */
     EQC_StaticText(0, 0, TKN_NAM_CDMA_1STCHANNB);
     sReadNAMItemsFromNV(CDMA_PRICHNB_I, nNAM, (CHAR*)spszNAMInfo);
     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)spszNAMInfo);
     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_CURSOR);
     sInitBuf((CHAR*)spszNAMInfo, EQS_SIZE_NAM_BUFFER);

     /* Read CDMA 2nd Channel B from NV */
     EQC_StaticText(0, 0, TKN_NAM_CDMA_2NDCHANNB);
     sReadNAMItemsFromNV(CDMA_SECCHNB_I, nNAM, (CHAR*)spszNAMInfo);
     hControl = EQC_NumberEdit(0, 0, NAM_STRD_INPUT_CONTROL_WIDTH, 1 , 34, (BYTE*)spszNAMInfo);     
     EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_CURSOR);
//#endif  /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)*/
     /* <-- */
     nMAXCtrl = NC_MAX;


     EQS_StartFormatter();
     for(nCtrl = 0; nCtrl <nMAXCtrl  ; nCtrl ++)
	 {
       EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
       EQS_NewLine();
	 }
     EQS_EndFormatter();
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_Channel(SKY_EVENT_T* pEvent)
{
    STATIC  UINT8       nNAM;
//    STATIC  BOOL        fEndToRoot;    
    STATIC  HCONTROL    hControl, hCtrlFail;
    STATIC  HCONTROL    hCtrlConfirm;
    BOOL    fValidData;
      
    
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      nNAM = pEvent->wParam;
      sMakeTitle(nNAM, pszNAMInfo);
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               pszNAMInfo, 
               WA_SHOW_CML|WA_SHOW_HEADER);

//      fEndToRoot = FALSE;
	  sInitChannelDisplay(nNAM);
        break;    
  
    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;
    case SE_APP_DRAW :  /* draw */
        break;

    case SE_KEY_END :
//        fEndToRoot = TRUE;
    case SE_IO_NEDIT_CANCEL :
    case SE_SOFTKEY_CANCEL  :
        /* restore original number */
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_End();
        }
      break;

    case SE_IO_NEDIT_DONE :
      break;
        
    case SE_SOFTKEY_SAVE :
      sInitBuf((CHAR*)pszNAMInfo, EQS_SIZE_NAM_BUFFER);
      fValidData = TRUE;

      EQC_GetNumberEditValue(EQS_GetNthIOC(NC_FIRSTCHANNELA), (BYTE*)pszNAMInfo);
      if(sIsValidData(pszNAMInfo) == TRUE)
      {
        sSaveNAMItems2NV(nNAM, CDMA_PRICHNA_I, (CHAR*)pszNAMInfo);
      }
      else
      {
          fValidData = FALSE;
      }


      EQC_GetNumberEditValue(EQS_GetNthIOC(NC_SECONDCHANNELA), (BYTE*)pszNAMInfo);
      if(sIsValidData(pszNAMInfo) == TRUE)
      {
        sSaveNAMItems2NV(nNAM, CDMA_SECCHNA_I, (CHAR*)pszNAMInfo);
      }
      else
      {
          fValidData = FALSE;
      }

      EQC_GetNumberEditValue(EQS_GetNthIOC(NC_FIRSTCHANNELB), (BYTE*)pszNAMInfo);
      if(sIsValidData(pszNAMInfo) == TRUE)
      {
          sSaveNAMItems2NV(nNAM, CDMA_PRICHNB_I, (CHAR*)pszNAMInfo);
      }
      else
      {
          fValidData = FALSE;
      }

      EQC_GetNumberEditValue(EQS_GetNthIOC(NC_SECONDCHANNELB), (BYTE*)pszNAMInfo);

      if(sIsValidData(pszNAMInfo) == TRUE)
      {
        sSaveNAMItems2NV(nNAM, CDMA_SECCHNB_I, (CHAR*)pszNAMInfo);
      }
      else
      {
          fValidData = FALSE;
      }

      if(fValidData == TRUE)
      {
          hControl = EQC_MessageBox((BYTE*) "저장중 입니다.", MESSAGE_DEFAULT_TIME, 
                                             MBT_NONE, MBT_SUCCESS);
      }
      else
      {
          hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_WRONG_INPUT), MESSAGE_DEFAULT_TIME, 
                                                 MBT_NONE, MBT_FAIL);
      }
       //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
       break;    

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }
        break;


    case SE_IO_MESGBOX_NO:
//        if(fEndToRoot == FALSE)
//        {
            EQS_END();
//        }
//        else
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
        break;


    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hControl)
        {
//            if(fEndToRoot == FALSE)
//            {        
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
      break;
      
      
    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sInitSIDNIDEditBox(WORD wIndex, DWORD nNAM)
{
   HCONTROL   hControl;
   BYTE       pszTmpBuf[EQS_MAX_TMP_BUF];
   UINT8      nCtrl;
   UINT8      nMAXCtrl = NE_MAX;

   hControl = EQC_StaticText(0, 0, TKN_BLANK);
   sprintf((CHAR *)pszTmpBuf, "[%02d]",wIndex+1);
   EQC_SetStaticText(hControl, (BYTE*) pszTmpBuf); 

   nvi.home_sid_nid.nam = nNAM;      
   SKY_GetNV(NV_HOME_SID_NID_I, &nvi);

   /* wIndex를 이용해서 적절한 SIDNID pair를 get nv를 해야한다. */
   EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_SID);
   sprintf((CHAR *)pszTmpBuf, "%d", nvi.home_sid_nid.pair[wIndex].sid);
   hControl = EQC_NumberEdit(EQS_NA, EQS_NA, NAM_STRD_INPUT_CONTROL_WIDTH, 1, 34, pszTmpBuf);
   EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM |CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_CURSOR);


   EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_NID);
   sprintf((CHAR *)pszTmpBuf, "%d", nvi.home_sid_nid.pair[wIndex].nid);
   hControl = EQC_NumberEdit(EQS_NA, EQS_NA, NAM_STRD_INPUT_CONTROL_WIDTH, 1, 34, pszTmpBuf);
   EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_PLAIN_NUM |CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_CURSOR);


   EQS_StartFormatter();
   for(nCtrl = 0; nCtrl <nMAXCtrl  ; nCtrl ++)
    {
       EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
       
       EQS_NewLine();
    }
   EQS_EndFormatter();

   EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0);
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sSKA_EditSIDNID(SKY_EVENT_T* pEvent)
{
    STATIC WORD         wIndex;
    STATIC UINT8        nNAM;
    WORD                wValue;
    BYTE                pszSID[EQS_SIZE_NAM_BUFFER], pszNID[EQS_SIZE_NAM_BUFFER];
    STATIC HCONTROL     hControl, hCtrlConfirm, hCtrlFail;
//    STATIC BOOL         fEndToRoot;;

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               (BYTE*)"EDIT SID-NID", 
               (WA_SHOW_CML | WA_SHOW_HEADER));      
      wIndex = pEvent->wParam;
      nNAM   = pEvent->dwParam;
//      fEndToRoot = FALSE;
      sInitSIDNIDEditBox(wIndex, nNAM);      
//      EQS_FitCanvasSize();
        break;
    
    case SE_APP_END :     /* The very last event */      
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;

    case SE_KEY_END:
//        fEndToRoot = TRUE;
	    if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, 
                                          MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_END();
        }
        break;

    case SE_IO_NEDIT_CANCEL :
    case SE_SOFTKEY_CANCEL  :
    case SE_KEY_CLEAR:
        /* restore original number */
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_End();
        }
        break;

    case SE_SOFTKEY_SAVE :        
        EQC_GetNumberEditValue(EQS_GetNthIOC(NE_SID), pszSID);        
        EQC_GetNumberEditValue(EQS_GetNthIOC(NE_NID), pszNID);

        if(sIsValidData(pszSID) && sIsValidData(pszNID))
        {
            /* SID 저장 */        
            //EQC_GetNumberEditValue(EQS_GetNthIOC(NE_SID), pszSID);
            wValue = sConvertASC2DEC((CHAR*)pszSID);
            nvi.home_sid_nid.nam = nNAM;      
            SKY_GetNV(NV_HOME_SID_NID_I, &nvi);
            nvi.home_sid_nid.pair[wIndex].sid = wValue;
            if(SKY_PutNV(NV_HOME_SID_NID_I, &nvi) != NV_DONE_S)
            {
                SKY_PutNV(NV_HOME_SID_NID_I, &nvi);
            }
      
            /* NID 저장 */        
            //EQC_GetNumberEditValue(EQS_GetNthIOC(NE_NID), pszNID);
            wValue = sConvertASC2DEC((CHAR*)pszNID);
            nvi.home_sid_nid.nam = nNAM;
            SKY_GetNV(NV_HOME_SID_NID_I, &nvi);
            nvi.home_sid_nid.pair[wIndex].nid = wValue;
            if(SKY_PutNV(NV_HOME_SID_NID_I, &nvi) != NV_DONE_S)
            {
                SKY_PutNV(NV_HOME_SID_NID_I, &nvi);
            }

            hControl = EQC_MessageBox((BYTE*) "저장중 입니다.", MESSAGE_DEFAULT_TIME, 
                                       MBT_NONE, MBT_SUCCESS);
        }
        else
        {
             hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_WRONG_INPUT), MESSAGE_DEFAULT_TIME, 
                                                 MBT_NONE, MBT_FAIL);
        }
        //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
       break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(pEvent->wParam == hCtrlConfirm)
        {
//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
        break;

    case SE_IO_MESGBOX_END:  
        if(pEvent->wParam == hControl)
        {
//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
       break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sInitSIDNIDDisplay(UINT8 nNAM)
{
     UINT8 nInitLine, nLogicalLine, nCurrIndex;
     HCONTROL hControl;
 
     /* NV_HOME_SID_NID_I 이용    */
     /* 20개의 쌍을 보여주어야 함 */
     sInitSIDNIDList(nNAM);
     nInitLine     =  NUM_SIDNID_LIST + 1;
     nLogicalLine  =  NV_MAX_HOME_SID_NID;
     nCurrIndex = 0;
     hControl  = EQC_ScrollList(0, 0, 17, NUM_SIDNID_LIST, pszSIDNIDPair,
                                nInitLine, nLogicalLine, nCurrIndex);
     EQC_SetLogicalList(hControl, TRUE); // IO 이벤트를 받을 수 있다.
	 EQC_ShowScrollTitle(hControl, TRUE);
     EQC_SET_STYLE(hControl, 
				   CS_SCROLL_LIST_ENUMBERATED | 
				   CS_SCROLL_LIST_HMAXIMIZE |//);
                   CS_SCROLL_LIST_BORDER );     
     EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_NAM_HOMESIDNID));     
     EQS_SetIOCActive(hControl);
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_SIDNID(SKY_EVENT_T* pEvent)
{
   HCONTROL         hControl;
   UINT8            nCurrIndex, i;
   STATIC UINT8     nNAM;
   STATIC DWORD     dwLogicalIndex;
  
  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START :   /* The very first event */
      nNAM = pEvent->wParam;
	  sMakeTitle(nNAM, pszNAMInfo);
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_SELECT_NONE, 
               pszNAMInfo, 
               WA_SHOW_CML|WA_SHOW_HEADER);  
      
	  sInitSIDNIDDisplay(nNAM);
        break;
    
    case SE_APP_END :     /* The very last event */
        break;

    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
       hControl = EQS_GetNthIOC(NS_SIDNID);
       sUpdateSIDNIDList(nNAM, EQC_GetScrollData(hControl, EQC_GetScrollPhysicalIndex(hControl)),\
                               dwLogicalIndex);
       EQS_SEND_EVENT(SE_APP_DRAW, (WORD)EQS_GetNthIOC(NS_SIDNID), 0);
       EQS_SetIOCActive(EQS_GetNthIOC(NS_SIDNID));
        break;

    case SE_APP_DRAW :    /* draw */
        break;

    case SE_KEY_END :
         EQS_END();
        break;

    case SE_IO_SCROLL_LIST_UP :    /* 화면의 갱신이 필요할 경우에만 발생하는 이벤트 */
    case SE_IO_SCROLL_LIST_DOWN :
        hControl    = pEvent->wParam;   
        nCurrIndex  = pEvent->dwParam;  /* Logical Index */
        sprintf((char*)EQC_GetScrollData(hControl, EQC_GetScrollPhysicalIndex(hControl)), \
                         "%ld/%ld", (DWORD)nvi.home_sid_nid.pair[nCurrIndex].sid, \
                                  (DWORD)nvi.home_sid_nid.pair[nCurrIndex].nid);                      
        break;

    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        hControl   = pEvent->wParam;
        nCurrIndex = pEvent->dwParam;    /* 화면에 처음 뿌려질 데이터의 인덱스 */
        for( i=0; i< NUM_SIDNID_LIST; i++)
        {
          sprintf((char*)EQC_GetScrollData(hControl, i/*EQC_GetScrollPhysicalIndex(hControl)*/), \
                         "%ld/%ld", (DWORD)nvi.home_sid_nid.pair[nCurrIndex+i].sid, \
                                  (DWORD)nvi.home_sid_nid.pair[nCurrIndex+i].nid);
        }
        break;

    case SE_IO_SCROLL_LIST_SELECT :
    case SE_SOFTKEY_SELECT:
        dwLogicalIndex = pEvent->dwParam;
        EQS_START_CHILD_EVENT(sSKA_EditSIDNID, SE_APP_START,(WORD) dwLogicalIndex, nNAM);
        break;

    case SE_IO_SCROLL_LIST_CANCEL :
    case SE_SOFTKEY_CANCEL :
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define PREF_MAX_MENU_LENGTH   15
#define PREF_MAX_MENU_LINE      4

enum tagSelectPrefSys    /*시스템선택*/
{
  SP_TITLE = 0,           
  SP_MENU,     
  SP_MAX
};


void sInitPrefSystemDisplay(UINT8 nNAM)
{
  UINT8 nCurrIndex, nCtrl;
  HCONTROL hControl;

  sinitPrefSysMenu();
  nCurrIndex = sReadPrefSysInfo(nNAM);

  hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_PREF_SYS_TITLE);

  hControl = EQC_RadioButton(EQS_NA, EQS_NA, PREF_MAX_MENU_LENGTH,
	                         PREF_MAX_MENU_LINE, (BYTE **)pszPreferedSys, nCurrIndex);
  EQC_SET_STYLE( EQS_GetNthIOC(SP_MENU), 
                (EQC_GET_STYLE(EQS_GetNthIOC(SP_MENU)) & ~CS_RADIO_BUTTON_BORDER)); 
  
  EQS_SetIOCActive(hControl);


  EQS_StartFormatter();
  for(nCtrl = 0; nCtrl <SP_MAX  ; nCtrl ++)
  {
    EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);              
    EQS_NewLine();
  }
   EQS_EndFormatter();
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API SKA_PrefSys(SKY_EVENT_T* pEvent)
{
  STATIC UINT8             nNAM;
  STATIC UINT8             nRadioIndex;
  STATIC HCONTROL          hControl, hCtrlConfirm;
//  STATIC  BOOL             fEndToRoot;

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      nNAM = pEvent->wParam;
	  sMakeTitle(nNAM, pszNAMInfo);
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_SELECT_NONE, 
               pszNAMInfo, 
               (WA_SHOW_CML|WA_SHOW_HEADER)); 
	  sInitPrefSystemDisplay(nNAM);
//	  fEndToRoot = FALSE;
        break;

    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;
    case SE_APP_DRAW :  /* draw */
        break;

    case SE_KEY_END :
//        fEndToRoot = TRUE;
	    if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, 
                                          MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_END();
        }
		break;

		
    case SE_IO_RADIO_BUTTON_CANCEL:
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_End();
        }
        break;

    case SE_IO_MESGBOX_NO:
//        if(fEndToRoot == FALSE)
//        {
            EQS_END();
//        }
//        else
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hControl)
        {           
//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
		break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {            
            EQS_SEND_EVENT(SE_IO_RADIO_BUTTON_SELECT, EQS_NA, EQS_NA);
        }
        break;
    
    case SE_SOFTKEY_SAVE:
    case SE_IO_RADIO_BUTTON_SELECT:
		nRadioIndex = EQC_GetRadioButton(EQS_GetNthIOC(SP_MENU));
        nvi.system_pref.nam = nNAM;
        if(nRadioIndex == 0)
           nvi.system_pref.sys = (nv_sys_pref_enum_type) 0;
        else if(nRadioIndex == 1)
           nvi.system_pref.sys = (nv_sys_pref_enum_type) 1;
        else if(nRadioIndex == 2)
           nvi.system_pref.sys = (nv_sys_pref_enum_type) 2;
        else if(nRadioIndex == 3)
           nvi.system_pref.sys = (nv_sys_pref_enum_type) 3;

        (void) SKY_PutNV( NV_SYSTEM_PREF_I, &nvi );

		EQS_SetIOCActive(EQS_GetNthIOC(SP_MENU));

        hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NAM_SAVE_END), MESSAGE_DEFAULT_TIME, 
                                   MBT_NONE, MBT_SUCCESS);
        //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
        break;

   
    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sInitMCCDisplay(UINT8 nNAM)
{
   HCONTROL hControl;
   BYTE     spszNAMInfo[EQS_SIZE_NAM_BUFFER];
     /* Read MCC from NV  implemented by Number Edit */
   EQC_StaticText(NAM_MESG_START_XPOS, 0, TKN_NAM_MCC);
   sReadNAMItemsFromNV(CDMAMCC_I, nNAM, (CHAR*)spszNAMInfo);
   hControl  = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              1, MAX_DIAL_DIGIT_LENGTH,(BYTE*)spszNAMInfo);
   EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT|CS_NUMBER_EDIT_BORDER|
                           CS_NUMBER_EDIT_ALONE|CS_NUMBER_EDIT_CURSOR);
   EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
   EQS_SetIOCActive(hControl);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API SKA_MCC(SKY_EVENT_T* pEvent)
{
    STATIC UINT8 nNAM;
    STATIC HCONTROL     hControl, hCtrlConfirm, hCtrlFail;
//    STATIC BOOL         fFromSimpleNAM, fEndToRoot;
    
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      nNAM = pEvent->wParam;
	  sMakeTitle(nNAM, pszNAMInfo);
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               pszNAMInfo, 
               (WA_SHOW_CML|WA_SHOW_HEADER));   
      
//      fEndToRoot = FALSE;
	  sInitMCCDisplay(nNAM);
//      EQS_FitCanvasSize();
        break;

    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;    
    case SE_APP_DRAW:
        break;

    case SE_IO_NEDIT_CANCEL :
    case SE_SOFTKEY_CANCEL  :
        /* restore original number */
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_End();
        }
      break;

    case SE_IO_NEDIT_DONE :
    case SE_SOFTKEY_SAVE :       
        EQC_GetNumberEditValue(EQS_GetNthIOC(NM_MCC), (BYTE*)pszNAMInfo);

        if(sIsValidData(pszNAMInfo))
        {
            sSaveNAMItems2NV(nNAM, CDMAMCC_I, (CHAR*)pszNAMInfo);
            hControl = EQC_MessageBox((BYTE*) "저장중 입니다.", MESSAGE_DEFAULT_TIME, 
                                              MBT_NONE, MBT_SUCCESS);
        }
        else
        {
            hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_WRONG_INPUT), MESSAGE_DEFAULT_TIME, 
                                                 MBT_NONE, MBT_FAIL);
        }
       //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
       break;


    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(NM_MCC));
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }
        break;

    case SE_IO_MESGBOX_NO:
//        if(fEndToRoot == FALSE)
//        {
            EQS_END();
//        }
//        else
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
        break;

    case SE_IO_MESGBOX_END:  
        if(pEvent->wParam == hControl)
        {
//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
        else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(NM_MCC));
        }
       break;


    case SE_KEY_END :
//        fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            //EQS_END_TO(EQS_POP_TO_ROOT);
            EQS_END();
        }
       break;
    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void sInitMNCDisplay(UINT8 nNAM)
{
   HCONTROL hControl;
   BYTE     spszNAMInfo[EQS_SIZE_NAM_BUFFER];

   /* Read MNC from NV  implemented by Number Edit */
   EQC_StaticText(NAM_MESG_START_XPOS, 0, TKN_NAM_MNC);
   sReadNAMItemsFromNV(CDMAMNC_I, nNAM, (CHAR*)spszNAMInfo);
   hControl  = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              1, MAX_DIAL_DIGIT_LENGTH, spszNAMInfo);
   EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT|CS_NUMBER_EDIT_BORDER|
                           CS_NUMBER_EDIT_ALONE|CS_NUMBER_EDIT_CURSOR);
   EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
   EQS_SetIOCActive(hControl);      
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL BOOL sIsValidData(BYTE pszBuff[])
{
    if(STRLEN(pszBuff) == 0) return FALSE;
    else                     return TRUE;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_MNC(SKY_EVENT_T* pEvent)
{
    STATIC UINT8 nNAM;
    STATIC HCONTROL     hControl, hCtrlConfirm, hCtrlFail;
//    STATIC BOOL         fEndToRoot;
    
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      nNAM = pEvent->wParam;
	  sMakeTitle(nNAM, pszNAMInfo);
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_NONE_SAVE, 
               pszNAMInfo, 
               (WA_SHOW_CML|WA_SHOW_HEADER));
      
//      fEndToRoot = FALSE;
      sInitMNCDisplay(nNAM);
//      EQS_FitCanvasSize();
        break;

    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;

    case SE_APP_DRAW :  /* draw */
        break;

    case SE_IO_NEDIT_CANCEL :
    case SE_SOFTKEY_CANCEL  :
        /* restore original number */
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_End();
        }
        break;

    case SE_IO_NEDIT_DONE :
    case SE_SOFTKEY_SAVE :
        EQC_GetNumberEditValue(EQS_GetNthIOC(NMN_MNC), (BYTE*)pszNAMInfo);
        
        if(sIsValidData(pszNAMInfo) == TRUE)
        {
            sSaveNAMItems2NV(nNAM, CDMAMNC_I, (CHAR*)pszNAMInfo);
            hControl = EQC_MessageBox((BYTE*) "저장중 입니다.", MESSAGE_DEFAULT_TIME, 
                                       MBT_NONE, MBT_SUCCESS);
        }
        else
        {
             hCtrlFail   = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NAM_WRONG_INPUT), MESSAGE_DEFAULT_TIME, 
                                                 MBT_NONE, MBT_FAIL);
        }
        //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
      break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(NMN_MNC));
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }
        break;

    case SE_IO_MESGBOX_NO:
//        if(fEndToRoot == FALSE)
//        {
            EQS_END();
//        }
//        else
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
        break;

    case SE_IO_MESGBOX_END:  
        if(pEvent->wParam == hControl)
        {
//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
        else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(NMN_MNC));
        }
       break;


    case SE_KEY_END:
//        fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }
        else
        {
            EQS_END();
        }
       break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define PRL_MAX_MENU_LENGTH   15
#define PRL_MAX_MENU_LINE     2

enum tagPRLENABLED_E    /*시스템선택*/
{
  PRLE_TITLE = 0,           
  PRLE_MENU,     
  PRLE_MAX
};



void sInitPRLEnabledDisplay(UINT8 nNAM)
{
   UINT8    nCurrIndex;
   HCONTROL hControl;
   UINT8    nCtrl;

   /* Read PRL from NV  implemented by Combo Type Scroll List   */
   nvi.prl_enabled.nam = nNAM;
   SKY_GetNV(NV_PRL_ENABLED_I, &nvi);

   if(nvi.prl_enabled.enabled == TRUE) nCurrIndex = 0;
   else                                nCurrIndex = 1;
      
   EQC_StaticText(EQS_NA, EQS_NA, TKN_NAM_PRL);
   sinitYesNoList();

   hControl = EQC_RadioButton(EQS_NA, EQS_NA, PRL_MAX_MENU_LENGTH,
	                         PRL_MAX_MENU_LINE, (BYTE **)pszYesNo, nCurrIndex);
   EQC_SET_STYLE(hControl, 
                (EQC_GET_STYLE(hControl) & ~CS_RADIO_BUTTON_BORDER));
   EQS_SetIOCActive(hControl);
   
   EQS_StartFormatter();

   for(nCtrl = 0; nCtrl <PRLE_MAX  ; nCtrl ++)
   {
     EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);              
     EQS_NewLine();
   }
   EQS_EndFormatter();
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_PRL(SKY_EVENT_T* pEvent)
{
    STATIC UINT8        nNAM;
    STATIC HCONTROL     hControl, hCtrlConfirm;
//    STATIC BOOL         fEndToRoot;
  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      nNAM = pEvent->wParam;
	  sMakeTitle(nNAM, pszNAMInfo);
      EQS_SetWindow(SKY_DEF_WIN_X,
               SKY_DEF_WIN_Y, 
               SKY_DEF_WIN_DX, 
               SKY_DEF_WIN_DY, 
               CL_NONE_SELECT_NONE, 
               pszNAMInfo, 
               (WA_SHOW_CML|WA_SHOW_HEADER));
	  sInitPRLEnabledDisplay(nNAM);
//      EQS_FitCanvasSize();
//	  fEndToRoot = FALSE;
        break;

    case SE_APP_END : /* The very last event */
        break;
    case SE_APP_SUSPEND : /* start child */
        break;
    case SE_APP_RESUME :  /* end child */
        break;
    case SE_APP_DRAW :  /* draw */
        break;
    
    case SE_IO_RADIO_BUTTON_SELECT:
        nvi.prl_enabled.nam = nNAM;
        if(EQC_GetRadioButton(EQS_GetNthIOC(PRLE_MENU)) == 0)
            nvi.prl_enabled.enabled = TRUE;
        else
            nvi.prl_enabled.enabled = FALSE;

		EQS_SetIOCActive(EQS_GetNthIOC(PRLE_MENU));

        SKY_PutNV(NV_PRL_ENABLED_I,&nvi);
        hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NAM_SAVE_END), MESSAGE_DEFAULT_TIME, 
                                   MBT_NONE, MBT_SUCCESS);
        //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
        break;


	case SE_KEY_END:
        if(EQS_IsControlDataChanged())
		{
  		   hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NAM_SAVE_CONFIRM),
		 	                        MESSAGE_DEFAULT_TIME,
								    MBT_YESNO, MBT_CONFIRM);
		  //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
		}
		else
		{
		  EQS_END();
		}
		break;

		
    case SE_IO_RADIO_BUTTON_CANCEL:
    	if(EQS_IsControlDataChanged())
		{
  		  hCtrlConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NAM_SAVE_CONFIRM),
		 	                             MESSAGE_DEFAULT_TIME,
								          MBT_YESNO, MBT_CONFIRM);
		  //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
		}
		else
		{
		  EQS_END();
		}
        break;


    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlConfirm)
        {
            EQS_SEND_EVENT(SE_IO_RADIO_BUTTON_SELECT, EQS_NA, EQS_NA);
        }
        break;

    case SE_IO_MESGBOX_NO:
//        if(fEndToRoot == FALSE)
//        {
            EQS_END();
//        }
//        else
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
        break;

    case SE_IO_MESGBOX_END:  
        if(pEvent->wParam == hControl)
        {
//            if(fEndToRoot == FALSE)
//            {
                EQS_END();
//            }
//            else
//            {
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
        }
        else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(PRLE_MENU));
        }
       break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_NAM1SubEntry(SKY_EVENT_T* pEvent)
{
  UINT8  xPos, yPos;

  switch(pEvent->EventID)
  {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);
        xPos = 10;                         
        yPos = 10;
        EQS_SetIOCActive(EQC_MenuList(xPos, yPos, 17, EQS_NUM_SUBMENU, &mnNAM1ProgSub, 0,SKY_GET_SEL_MENU_MODE()));
      break;

    case SE_APP_RESUME :  /* end child */
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_KEY_END:
    case SE_IO_MENU_LIST_CANCEL:
        EQS_END();
      break;
    
    case SE_IO_MENU_LIST_SELECT:
       EQS_START_CHILD_EVENT((EQS_APPLET*)pEvent->dwParam, SE_APP_START, NAM1, 0);              
      break;

    
    default:
      EQS_DEF_EVENT_PROC(pEvent);
      break;
  }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_NAM2SubEntry(SKY_EVENT_T* pEvent)
{
    UINT8  xPos, yPos;

  switch(pEvent->EventID)
  {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);
        xPos = 10;
        yPos = 10;
        EQS_SetIOCActive(EQC_MenuList(xPos, yPos, 17, EQS_NUM_SUBMENU, &mnNAM2ProgSub, 0,SKY_GET_SEL_MENU_MODE()));
      break;

    case SE_APP_RESUME :  /* end child */
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
 
    case SE_KEY_END :
       EQS_END();
      break;

    case SE_IO_MENU_LIST_SELECT:
       EQS_START_CHILD_EVENT((EQS_APPLET*)pEvent->dwParam, SE_APP_START, NAM2, 0);                    
      break;

    case SE_IO_MENU_LIST_CANCEL:
       EQS_END();
      break;

    default:
      EQS_DEF_EVENT_PROC(pEvent);
      break;
  }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_NAM3SubEntry(SKY_EVENT_T* pEvent)
{
  UINT8  xPos, yPos;

  switch(pEvent->EventID)
  {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);
        xPos = 10;
        yPos = 10;
        EQS_SetIOCActive(EQC_MenuList(xPos, yPos, 17, EQS_NUM_SUBMENU, &mnNAM3ProgSub, 0,SKY_GET_SEL_MENU_MODE()));
      break;

    case SE_APP_RESUME :  /* end child */
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
 
    case SE_KEY_END :
       EQS_END();
      // Reset 되고 저장되지 않은 NAM data는 
      // 기존값으로 Setting되어야 한다.
      break;

    case SE_IO_MENU_LIST_SELECT:
       EQS_START_CHILD_EVENT((EQS_APPLET*)pEvent->dwParam, SE_APP_START, NAM3, 0);                    
      break;

    case SE_IO_MENU_LIST_CANCEL:
       EQS_END();
      break;

    default:
      EQS_DEF_EVENT_PROC(pEvent);
      break;
  }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_NAM4SubEntry(SKY_EVENT_T* pEvent)
{
  UINT8 xPos, yPos;

  switch(pEvent->EventID)
  {
    case SE_APP_START : /* The very first event */
        xPos = 10;
        yPos = 10;
        EQS_SetIOCActive(EQC_MenuList(xPos, yPos, 17, EQS_NUM_SUBMENU, &mnNAM4ProgSub, 0,SKY_GET_SEL_MENU_MODE()));
      break;

    case SE_APP_RESUME :  /* end child */
        EQS_SetIOCActive(EQS_GetNthIOC(0));
      break;
 
    case SE_KEY_END :
       EQS_END();
      // Reset 되고 저장되지 않은 NAM data는 
      // 기존값으로 Setting되어야 한다.
      break;

    case SE_IO_MENU_LIST_SELECT:
       EQS_START_CHILD_EVENT((EQS_APPLET*)pEvent->dwParam, SE_APP_START, NAM4, 0);                    
      break;

    case SE_IO_MENU_LIST_CANCEL:
       EQS_END();
      break;

    default:
      EQS_DEF_EVENT_PROC(pEvent);
      break;
  }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_NAM5SubEntry(SKY_EVENT_T* pEvent)
{
  UINT8 xPos, yPos;

  switch(pEvent->EventID)
  {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);
        xPos = 10;
        yPos = 10;
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
        EQS_SetIOCActive(EQC_MenuList(xPos, yPos, 17, EQS_NUM_SUBMENU, &mnNAM5ProgSub, 0,SKY_GET_SEL_MENU_MODE()));
#endif
      break;

    case SE_APP_RESUME :  /* end child */
        EQS_SetIOCActive(EQS_GetNthIOC(0));
      break;
 
    case SE_KEY_END :
       EQS_END();
      // Reset 되고 저장되지 않은 NAM data는 
      // 기존값으로 Setting되어야 한다.
      break;

    case SE_IO_MENU_LIST_SELECT:
       EQS_START_CHILD_EVENT((EQS_APPLET*)pEvent->dwParam, SE_APP_START, NAM5, 0);                    
      break;

    case SE_IO_MENU_LIST_CANCEL:
       EQS_END();
      break;

    default:
      EQS_DEF_EVENT_PROC(pEvent);
      break;
  }
}







/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#if 0 //000000000000000000000000000000000000000000000000000000000000000000000000
//VOID SKY_API SKA_SimpleNAM1SubEntry(SKY_EVENT_T* pEvent)
//{
//  switch(pEvent->EventID)
//  {
//    case SE_APP_START : /* The very first event */
//        EQS_SetWindow(SKY_DEF_WIN_X,
//                SKY_DEF_WIN_Y, 
//                SKY_DEF_WIN_DX, 
//                SKY_DEF_WIN_DY, 
//                CL_NONE_SELECT_NONE, 
//                NULL, 
//                WA_SHOW_CML);
//        EQS_START_CHILD_EVENT(SKA_PhoneNumber, SE_APP_START, NAM1, INPUT_FROM_SIMPLE_NAM);
//      break;
//
//    case SE_APP_RESUME :  /* end child */
//        EQS_END();
//        break;
//
//    case SE_KEY_END:
//    case SE_IO_MENU_LIST_CANCEL:
//        EQS_END();
//      break;
//    
//    
//    default:
//      EQS_DEF_EVENT_PROC(pEvent);
//      break;
//  }
//}
#endif //000000000000000000000000000000000000000000000000000000000000000000000000


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#if 0 //0000000000000000000000000000000000000000000000000000000000000000000000
//VOID SKY_API SKA_SimpleNAM2SubEntry(SKY_EVENT_T* pEvent)
//{
//  switch(pEvent->EventID)
//  {
//    case SE_APP_START : /* The very first event */
//        EQS_SetWindow(SKY_DEF_WIN_X,
//                SKY_DEF_WIN_Y, 
//                SKY_DEF_WIN_DX, 
//                SKY_DEF_WIN_DY, 
//                CL_NONE_SELECT_NONE, 
//                NULL, 
//                WA_SHOW_CML);
//        EQS_START_CHILD_EVENT(SKA_PhoneNumber, SE_APP_START, NAM2, INPUT_FROM_SIMPLE_NAM);
//      break;
//
//    case SE_APP_RESUME :  /* end child */
//        EQS_END();
//        break;
//
//    case SE_KEY_END:
//    case SE_IO_MENU_LIST_CANCEL:
//        EQS_END();
//      break;
//    
//    
//    default:
//      EQS_DEF_EVENT_PROC(pEvent);
//      break;
//  }
//}
#endif //0000000000000000000000000000000000000000000000000000000000000000000000



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#if 0 //0000000000000000000000000000000000000000000000000000000000000000000000
//VOID SKY_API SKA_SimpleNAM3SubEntry(SKY_EVENT_T* pEvent)
//{
//  switch(pEvent->EventID)
//  {
//    case SE_APP_START : /* The very first event */
//        EQS_SetWindow(SKY_DEF_WIN_X,
//                SKY_DEF_WIN_Y, 
//                SKY_DEF_WIN_DX, 
//                SKY_DEF_WIN_DY, 
//                CL_NONE_SELECT_NONE, 
//                NULL, 
//                WA_SHOW_CML);
//        EQS_START_CHILD_EVENT(SKA_PhoneNumber, SE_APP_START, NAM3, INPUT_FROM_SIMPLE_NAM);
//      break;
//
//    case SE_APP_RESUME :  /* end child */
//        EQS_END();
//        break;
//
//    case SE_KEY_END:
//        EQS_END();
//      break;
//    
//    
//    default:
//      EQS_DEF_EVENT_PROC(pEvent);
//      break;
//  }
//}
#endif //0000000000000000000000000000000000000000000000000000000000000000000000


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#if 0 //0000000000000000000000000000000000000000000000000000000000000000000000
//VOID SKY_API SKA_SimpleNAM4SubEntry(SKY_EVENT_T* pEvent)
//{
//  switch(pEvent->EventID)
//  {
//    case SE_APP_START : /* The very first event */
//        EQS_SetWindow(SKY_DEF_WIN_X,
//               SKY_DEF_WIN_Y, 
//                SKY_DEF_WIN_DX, 
//                SKY_DEF_WIN_DY, 
//                CL_NONE_SELECT_NONE, 
//                NULL, 
//                WA_SHOW_CML);
//        EQS_START_CHILD_EVENT(SKA_PhoneNumber, SE_APP_START, NAM4, INPUT_FROM_SIMPLE_NAM);
//      break;
//
//    case SE_APP_RESUME :  /* end child */
//        EQS_END();
//        break;
//
//    case SE_KEY_END:
//        EQS_END();
//      break;    
//    
//    default:
//      EQS_DEF_EVENT_PROC(pEvent);
//      break;
//  }
//}
#endif //0000000000000000000000000000000000000000000000000000000000000000000000


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#if 0 //0000000000000000000000000000000000000000000000000000000000000000000000
//VOID SKY_API SKA_SimpleNAM5SubEntry(SKY_EVENT_T* pEvent)
//{
//  switch(pEvent->EventID)
//  {
//    case SE_APP_START : /* The very first event */
//       EQS_SetWindow(SKY_DEF_WIN_X,
//                SKY_DEF_WIN_Y, 
//                SKY_DEF_WIN_DX, 
//                SKY_DEF_WIN_DY, 
//                CL_NONE_SELECT_NONE, 
//               NULL, 
//                WA_SHOW_CML);
//        EQS_START_CHILD_EVENT(SKA_PhoneNumber, SE_APP_START, NAM5, INPUT_FROM_SIMPLE_NAM);
//      break;
//
//    case SE_APP_RESUME :  /* end child */
//        EQS_END();
//        break;
//
//    case SE_KEY_END:
//        EQS_END();
//      break;    
//    
//    default:
//      EQS_DEF_EVENT_PROC(pEvent);
//      break;
//  }
//}
#endif //0000000000000000000000000000000000000000000000000000000000000000000000