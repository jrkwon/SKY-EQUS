//************************************************************************
//
//          L C D   C O N T R O L L E R
//
//  File Name      : lcdann.c
//  Description    : These Functions provide an LCD interface between the
//                    handset(hsg.c) and the other software tasks,
//                    so displays information on the handset display.
//  LCD Controller : HD66750
//  Author       : SK Teletech Software Team
//
//************************************************************************
//
//          EDIT HISTORY FOR MODULE
//
//  This section contains comments describing changes made to this file.
//  Notice that changes are listed in reverse chronological order.
//
//
//  when       who      what, where, why
//--------     ---      ---------------------------------------------------
// 00.08.23  jwpark     add (MMI_TYPE == MMI_A10)
// --------    ---      ---------------------------------------------------
// 05/17/00    kmh      LCD 폭주로 인한 DATA 깨짐 현상을 방지하기 위해 
//                      Display시마다 Reset해줌
// 01/20/00    kmh      New Lcd Spec에 맞추어 hs_lcd_init() 수정함.
// 10/22/99    kmh      Create file for supporting A7 Project
//
//
//*************************************************************************
#include "customer.h"


#if (MMI_TYPE == MMI_A10) // jwpark 00.08.23

#include "lcd_a10.h"
#include "hs.h"
#include "hsig.h"
#include "target.h"
#include "err.h"
#include "bio.h"
#include "string.h"
#include "biog.h"
#include "nv.h"

#include "..\SMB\smb.h"
#include "..\SMB\smbdev.h"
#include "bq2018.h"

#include <uiDummy.h>

//#define  HS_AN_MELODY   0x0800   /* This must be moved to "hs.h" */

#define BLINK_LCDANN
#undef BLINK_LCDANN

/* 
*  ICON_PICTURE_TYPE (enum for DrawIcon())
*/
typedef enum {
  RSSI_5_BARS,
  RSSI_4_BARS,      
  RSSI_3_BARS,            
  RSSI_2_BARS,                  
  RSSI_1_BAR,                       
  RSSI_ANTENNA_ONLY,                              
  RSSI_NONE,                 
  ANN_ROAM_ON,
  ANN_ROAM_OFF,
  ANN_INUSE_SVC,
  ANN_NOTINUSE_SVC,
  ANN_NO_SVC,
#ifdef FEATURE_UI_DORMANT
  ANN_DORM_SVC,
#endif //#ifdef FEATURE_UI_DORMANT
  ANN_MELODY_ON,
  ANN_MELODY_OFF,
  ANN_VIB_ON,
  ANN_VIB_OFF,
  ANN_VMAIL_ON,
  ANN_VMAIL_OFF,
  ANN_ENVOLOPE_ON,
  ANN_ENVOLOPE_OFF,
  ANN_MSG_ON,
  ANN_MSG_OFF,
#ifdef FEATURE_ANN_ETIQUETTE
  ANN_ETIQ_ON,
#endif
  SMB_FULL_ON,
  SMB_DIGIT1_ON,
  SMB_DIGIT0_ON,
  SMB_DIGIT1_OFF,
  SMB_DIGIT0_OFF,
  SMB_PERCENT_ON,
  SMB_PERCENT_OFF,
  SMB_NOTKNOWN_ON,
  SMB_NOTKNOWN_OFF,
  NOSMB_BATT_3_BARS,
  NOSMB_BATT_2_BARS,
  NOSMB_BATT_1_BARS,
//  BATT_NOSMB,
  BATT_16_BARS,
  BATT_15_BARS,     
  BATT_14_BARS,
  BATT_13_BARS,
  BATT_12_BARS,     
  BATT_11_BARS,
  BATT_10_BARS,
  BATT_9_BARS,     
  BATT_8_BARS,
  BATT_7_BARS,
  BATT_6_BARS,     
  BATT_5_BARS,
  BATT_4_BARS,
  BATT_3_BARS,     
  BATT_2_BARS,
  BATT_1_BAR,
  BATT_BORDER_ONLY, /*empty charged*/
  BATT_NONE  
  } ICON_PICTURE_TYPE;

/*
* Coordinate  in IconBuffer (or IconBlinkBuffer) 
*/
#define RSSI_X           0//
#define RSSI_Y           0
#define RSSI_WIDE        22   
#define RSSI_HIGH        16
#define ANN_ROAM_X       82//
#define ANN_ROAM_Y       0
#define ANN_ROAM_WIDE    12   
#define ANN_ROAM_HIGH    16
#define ANN_SVC_X        23//
#define ANN_SVC_Y        0
#define ANN_SVC_WIDE     15
#define ANN_SVC_HIGH     16
#define ANN_MELODY_X     39//
#define ANN_MELODY_Y     0
#define ANN_MELODY_WIDE  9
#define ANN_MELODY_HIGH  16   
#define ANN_VIB_X        49//  
#define ANN_VIB_Y        0
#define ANN_VIB_WIDE     13
#define ANN_VIB_HIGH     16
#define ANN_VMAIL_X      79//
#define ANN_VMAIL_Y      0
#define ANN_VMAIL_WIDE   3
#define ANN_VMAIL_HIGH   16
#define ANN_ENV_X        63//
#define ANN_ENV_Y        0
#define ANN_ENV_WIDE     14
#define ANN_ENV_HIGH     15
#define ANN_MSG_X        95//
#define ANN_MSG_Y        0
#define ANN_MSG_WIDE     3
#define ANN_MSG_HIGH     16//
#ifdef FEATURE_ANN_ETIQUETTE
#define ANN_ETIQ_X       39
#define ANN_ETIQ_Y       32
#define ANN_ETIQ_WIDE    9
#define ANN_ETIQ_HIGH    16
#endif
#define SMB_FULL_X		 78	 
#define SMB_FULL_Y		 0
#define SMB_FULL_WIDE	 14	 
#define SMB_FULL_HIGH    16
#define SMB_DIGIT1_X     78 
#define SMB_DIGIT1_Y     0
#define SMB_DIGIT1_WIDE  6
#define SMB_DIGIT1_HIGH  16
#define SMB_DIGIT0_X     85 
#define SMB_DIGIT0_Y     0
#define SMB_DIGIT0_WIDE  8
#define SMB_DIGIT0_HIGH  16
#define SMB_NOTKNOWN_X     84 
#define SMB_NOTKNOWN_Y     0
#define SMB_NOTKNOWN_WIDE  8
#define SMB_NOTKNOWN_HIGH  16
#define SMB_PERCENT_X    92
#define SMB_PERCENT_Y    0
#define SMB_PERCENT_WIDE 6
#define SMB_PERCENT_HIGH 16
#define BATT_X           99//
#define BATT_Y           0
#define BATT_WIDE        21
#define BATT_HIGH        16

/*
* Coordinate  in  Icon Image File
*/
/* Rssi level */
#define  RSSI_5_BARS_X       0 //     
#define  RSSI_5_BARS_Y       0
#define  RSSI_4_BARS_X       0    
#define  RSSI_4_BARS_Y       16    
#define  RSSI_3_BARS_X       0      
#define  RSSI_3_BARS_Y       32      
#define  RSSI_2_BARS_X       0                
#define  RSSI_2_BARS_Y       48                
#define  RSSI_1_BAR_X        0                        
#define  RSSI_1_BAR_Y        64               
#define  RSSI_ANTENNA_ONLY_X 0                              
#define  RSSI_ANTENNA_ONLY_Y 80                         
#define  RSSI_NONE_X         0                        
#define  RSSI_NONE_Y         96               
/* ROAM */
#define ANN_ROAM_ON_X        82//
#define ANN_ROAM_ON_Y        0
#define ANN_ROAM_OFF_X       78 
#define ANN_ROAM_OFF_Y       88
/* CDMA & Service */
#define ANN_SVC_NOTINUSE_X   23//
#define ANN_SVC_NOTINUSE_Y   16
#define ANN_SVC_INUSE_X      23// 
#define ANN_SVC_INUSE_Y      32
#define ANN_SVC_NONE_X       39
#define ANN_SVC_NONE_Y       16
#define ANN_DORM_X			 14
#define ANN_DORM_Y			 98
/* MEMODY */
#define ANN_MELODY_ON_X      39//
#define ANN_MELODY_ON_Y      0
#define ANN_MELODY_OFF_X     39
#define ANN_MELODY_OFF_Y     16
/* VIB */
#define ANN_VIB_ON_X         49//
#define ANN_VIB_ON_Y         0
#define ANN_VIB_OFF_X        39
#define ANN_VIB_OFF_Y        16
/* ENVELOPE */
#define ANN_ENV_ON_X         63//
#define ANN_ENV_ON_Y         0
#define ANN_ENV_OFF_X        39
#define ANN_ENV_OFF_Y        16
/* VMAIL */
#define ANN_VMAIL_ON_X       79//
#define ANN_VMAIL_ON_Y       0
#define ANN_VMAIL_OFF_X      79
#define ANN_VMAIL_OFF_Y      16
/* TEXT MESSGE */
#define ANN_MSG_ON_X         95//
#define ANN_MSG_ON_Y         0
#define ANN_MSG_OFF_X        95
#define ANN_MSG_OFF_Y        16
/* Smart Battery */
#define SMB_FULL_ON_X			78
#define SMB_FULL_ON_Y			98
#define SMB_DIGIT1_ON_X         78//
#define SMB_DIGIT1_ON_Y         14
#define SMB_DIGIT1_OFF_X        39
#define SMB_DIGIT1_OFF_Y        16
#define SMB_DIGIT0_ON_X         85//
#define SMB_DIGIT0_ON_Y         14
#define SMB_DIGIT0_OFF_X        39
#define SMB_DIGIT0_OFF_Y        16
#define SMB_NOTKNOWN_ON_X		84
#define SMB_NOTKNOWN_ON_Y		84
#define SMB_NOTKNOWN_OFF_X		39
#define SMB_NOTKNOWN_OFF_Y		16
/* Smart Battery Percent */
#define SMB_PERCENT_ON_X         92//
#define SMB_PERCENT_ON_Y         14
#define SMB_PERCENT_OFF_X        92
#define SMB_PERCENT_OFF_Y        32
/* Battery Level */
#define  NOSMB_BATT_3_BARS_X	99
#define  NOSMB_BATT_3_BARS_Y	112
#define  NOSMB_BATT_2_BARS_X	74
#define  NOSMB_BATT_2_BARS_Y	112
#define  NOSMB_BATT_1_BARS_X	49
#define  NOSMB_BATT_1_BARS_Y	112
//#define  BATT_NOSMB_X		  32
//#define  BATT_NOSMB_Y		  98
#define  BATT_16_BARS_X       99//
#define  BATT_16_BARS_Y       0
#define  BATT_15_BARS_X       98//
#define  BATT_15_BARS_Y       14
#define  BATT_14_BARS_X       98//
#define  BATT_14_BARS_Y       28
#define  BATT_13_BARS_X       98//
#define  BATT_13_BARS_Y       42
#define  BATT_12_BARS_X       98//
#define  BATT_12_BARS_Y       56
#define  BATT_11_BARS_X       98//
#define  BATT_11_BARS_Y       70
#define  BATT_10_BARS_X       98//
#define  BATT_10_BARS_Y       84
#define  BATT_9_BARS_X       56//
#define  BATT_9_BARS_Y       14
#define  BATT_8_BARS_X       56//
#define  BATT_8_BARS_Y       28
#define  BATT_7_BARS_X       56//
#define  BATT_7_BARS_Y       42
#define  BATT_6_BARS_X       56//
#define  BATT_6_BARS_Y       56
#define  BATT_5_BARS_X       56//
#define  BATT_5_BARS_Y       70
#define  BATT_4_BARS_X       56//
#define  BATT_4_BARS_Y       84
#define  BATT_3_BARS_X       32//
#define  BATT_3_BARS_Y       56
#define  BATT_2_BARS_X       32
#define  BATT_2_BARS_Y       70
#define  BATT_1_BAR_X        32
#define  BATT_1_BAR_Y        84
#define  BATT_BODER_ONLY_X   56
#define  BATT_BODER_ONLY_Y   98
#define  BATT_NONE_X         99
#define  BATT_NONE_Y         98
///////////////////////////////////////////////////////////////////////////


word hs_annun = HS_AN_ALL_OFF;       /* non-blink annunciators         */
word hs_blink_annun;                 /* Blink annunciators             */

byte hs_rssi;                        /* Non-blink RSSI level           */
byte hs_blink_rssi;                  /* Blink RSSI level               */

byte hs_batt;                        /* Non-blink Battery level        */
byte hs_blink_batt;                  /* Blink Battery level            */

byte *hs_crnt_rssi_ptr = &hs_rssi;   /* Current RSSI level             */
byte *hs_crnt_batt_ptr = &hs_batt;   /* Current Battery level          */
word *hs_crnt_annun_ptr = &hs_annun; /* Current annunciators           */

byte hs_smb;
byte *hs_crnt_smb_ptr = &hs_smb;		 /* Current Smart Battery level    */

word hs_last_annun;
byte hs_last_rssi;
byte hs_last_batt;
byte hs_last_smb;

// for LCD test by msrock
extern byte LcdTestFlag;
		

#define SMB_OFFLINE		0xFF
#define SMB_FULL		100							 
#define SMB_HALF		10							 
#define SMB_LOW_BATT     2
/*
*   IconBuffer/IconBlinkBuffer 
*/
#define ICON_HIGH 16
byte IconBuffer[ICON_HIGH][LCD_TOTAL_ADDRESS_X];
byte IconBlinkBuffer[ICON_HIGH][LCD_TOTAL_ADDRESS_X]; 

/*
*  Icon Image Source File Include
*/
const byte srcIcon[128][LCD_TOTAL_ADDRESS_X] = {
  #include "Image\A7\annun_A7.def"   //2000.2.23
};

/*
*   Pointers
*/
byte* pIconPicture =(byte*) &srcIcon[0][0];
byte* pIconBuffer = &IconBuffer[0][0];



static void ClearIconBufferAll (void);
//static void DisplayIconBufferAll (void);
// tasik 00.11.22 self-check에서 쓰기 위해서.
void DisplayIconBufferAll (void);

static void lcd_AllPictographsOn (void);
static void lcd_AllPictographsOff (void);

extern void hs_lcd_word_dat_out(word lcd_data); // 16-bit LCD command to output to the LCD controller
extern void hs_lcd_onebit_dat_out(byte lcd_data);


/*
*  Draw function with IconBuffer[][]
*/
LOCAL void DrawIcon (ICON_PICTURE_TYPE picture);
LOCAL void CopyIconBlock(byte* pDest, byte dx, byte dy, byte* pSrc,  byte sx, byte sy, byte sw, byte sh);
LOCAL byte GetIconPicture(byte* data, byte x, byte y);
LOCAL void SetIconPicture(byte* data, byte x, byte y, byte color);

#ifdef FEATURE_NETWORK
#ifdef RES_KOKR
	//UP 한글입력 모드 확인용
	extern boolean g_bKorEntryMode;
	extern boolean g_bDrawKorLabel;
#endif//RES_KOKR
#endif//FEATURE_NETWORK
	
#ifdef BLINK_LCDANN
/*
*  Current blink mode  
*/
boolean bBlinkMode=FALSE;
/*
*  Blink mode checking & setting functions  
*/
LOCAL void SetRssiBlink(void);
LOCAL void SetBattBlink(void);
LOCAL void SetAnnunBlink(word lcd_annun, word lcd_blink_annun);

/*
*  Draw function with IconBlinkBuffer[][]    //2000.2.23
*/
LOCAL void DrawBlinkIcon(byte x1, byte y1, byte dx, byte dy);
LOCAL byte GetBufPixel(byte* data, byte x, byte y);
LOCAL void SetBufPixel(byte* data, byte x, byte y, byte color);

/*
*  Invert IconBuffer[][] according to IconBlinkBuffer[][]  //2000.2.23
*/
LOCAL void BlinkIconBuffer(void);

/*
*  hs_icon_timer function (called every 300 ms by HS_TASK())
*  ---> DisplayIconBufferAll() is called by only this function 
*/
void lcd_icon_display(void);   

#endif //BLINK_LCDANN

/*=======================================================================
  Rssi Level Count Example  

          RSSI_VAL5 <= lcd_rssi                   //5 bars
          RSSI_VAL4 <= lcd_rssi < RSSI_VAL5       //4 bars
          RSSI_VAL3 <= lcd_rssi < RSSI_VAL4       //3 bars     
          RSSI_VAL2 <= lcd_rssi < RSSI_VAL3       //2 bars 
          RSSI_VAL1 <= lcd_rssi < RSSI_VAL2       //1 bar
                       lcd_rssi < RSSI_VAL1       //Antenna only
========================================================================*/
#define RSSI_VAL5       5
#define RSSI_VAL4       4  
#define RSSI_VAL3       3
#define RSSI_VAL2       2
#define RSSI_VAL1       1    
/*=======================================================================
  Battery Level Count Example 

          BATT_VAL3 <= lcd_batt                   //3 bars     
          BATT_VAL2 <= lcd_batt < BATT_VAL3       //2 bars 
          BATT_VAL1 <= lcd_batt < BATT_VAL2       //1 bar
					   lcd_batt < BATT_VAL1       //Low Batt. mode // girin. add. 2000.4.24.

========================================================================*/

// All these 
#define NOSMB_BATT_VAL1  31 
#define NOSMB_BATT_VAL2  32 
#define NOSMB_BATT_VAL3  33 

//#define BATT_VAL_NOSMB   20
#define BATT_VAL17       18
#define BATT_VAL16       17
#define BATT_VAL15       16
#define BATT_VAL14       15
#define BATT_VAL13       14
#define BATT_VAL12       13
#define BATT_VAL11       12
#define BATT_VAL10       11
#define BATT_VAL9       10
#define BATT_VAL8       9
#define BATT_VAL7       8
#define BATT_VAL6       7
#define BATT_VAL5       6
#define BATT_VAL4       5
#define BATT_VAL3       4
#define BATT_VAL2       3
#define BATT_VAL1       2    
#define BATT_VAL0		1 // girin. add. 2000.4.24 

// girin. comment. 2000.4.24
// All the #define's above should have the same values as those of #define BARS_*'s
// defined in UISTATE.C  

#ifdef SW_RESET_ENABLE
// chojw 01.02.15
// SW reset후에 In svc로 Ann에 표시되는 현상 방지
void nv_PutAnnValue(void)
{
    word *pAnnun;
    byte *pRssi;
    byte *pBatt;
    byte *pSmb;

    pAnnun = NV_SW_RESET_CHECK_POS - 10; 
	*pAnnun  = hs_last_annun;

    pRssi = NV_SW_RESET_CHECK_POS - 12; 
	*pRssi  = hs_last_rssi;

    pBatt = NV_SW_RESET_CHECK_POS - 14; 
	*pBatt  = hs_last_batt;

    pSmb = NV_SW_RESET_CHECK_POS - 16; 
	*pSmb  = hs_last_smb;
}

void nv_GetAnnValue(word *pHs_annun, byte *pHs_rssi, byte *pHs_batt, byte *pHs_smb)
{
    word mask;
    word *pAnnun;
    byte *pRssi;
    byte *pBatt;
    byte *pSmb;

    mask = HS_AN_DORM | HS_AN_INUSE | HS_AN_ALL_ON | HS_AN_ALL_OFF;
    pAnnun = NV_SW_RESET_CHECK_POS - 10; 
	*pHs_annun = *pAnnun & ~mask;
//    *pHs_annun |= HS_AN_CDMA;

    pRssi = NV_SW_RESET_CHECK_POS - 12; 
	*pHs_rssi = *pRssi;

    pBatt = NV_SW_RESET_CHECK_POS - 14; 
	*pHs_batt = *pBatt;

    pSmb = NV_SW_RESET_CHECK_POS - 16; 
	*pHs_smb = *pSmb;
}
#endif // SW_RESET_ENABLE

/*===========================================================================

EXTERN FUNCTION HS_UPDATE_ANNUN   // kmh 99.10.22

DESCRIPTION
  This procedure updates the annunciators, both LED and LCD.

DEPENDENCIES
  Not called from an ISR

RETURN VALUE   
  None

SIDE EFFECTS
  None

===========================================================================*/
///////////////////////////
// girin. add. 2000.1.30 //
// MODA  2000.1.25.    //
///////////////////////////

void hs_update_annun( void )
{

  word lcd_annun;
  byte lcd_rssi;
  byte lcd_batt;
  byte lcd_smb;

  static word lcd_blink_annun = 0;
  
#ifdef BLINK_LCDANN  
  word lcd_blink_annun;
  byte lcd_blink_rssi;
  byte lcd_blink_batt;  
#endif //BLINK_LCDANN  
  

// /****************************************************/
// /* 임시로 아이콘 모양을 테스트하기 위해 값을 지정함*/    
//  /* 2000.2.24*/
  //hs_rssi=7;
  //hs_batt = 5;  
  //hs_annun |= HS_AN_ROAM;
  //hs_annun |= HS_AN_CDMA;   
  //hs_annun |= HS_AN_VIB;
  //hs_annun |= HS_AN_ENVOLOPE;
  //hs_annun |= HS_AN_MSG;
  //hs_annun |= HS_AN_VMAIL;
  //hs_annun |= HS_AN_MELODY;
//  hs_blink_batt=1;
//  hs_blink_rssi=1;    
//  hs_blink_annun|=HS_AN_ENVOLOPE;
//  hs_blink_annun|=HS_AN_MSG;
//  hs_blink_annun|=HS_AN_VMAIL; 
  /* 임시로 아이콘 모양을 테스트하기 위해 값을 지정함*/     
  /**************************************************/

  
  /*Because of Bad RSSI Blinking ??? */
  lcd_rssi = hs_rssi;
  lcd_annun = hs_annun;
  lcd_batt = hs_batt;
  lcd_smb = hs_smb;

#ifdef BLINK_LCDANN  
  lcd_blink_annun=hs_blink_annun;
  lcd_blink_rssi=hs_blink_rssi;
  lcd_blink_batt=hs_blink_batt;  
#endif //BLINK_LCDANN  


  // chojw 01.01.12 - SW reset 후 SMS blink를 유지하기 위하여 blink check만을 
  //                  sw reset전으로 옮김
#ifdef BLINK_LCDANN
/* Enter to Set IconBlinkBuffer[][] */      
if (lcd_blink_rssi||lcd_blink_batt||lcd_blink_annun) 
{

  if (lcd_blink_rssi)
    SetRssiBlink();
    
  if (lcd_blink_annun)
    SetAnnunBlink(lcd_annun, lcd_blink_annun);
    
  if (lcd_blink_batt)
    SetBattBlink();    
    
  bBlinkMode = TRUE;  
} 
else
{
  bBlinkMode = FALSE;  
}

#else 

	// girin. add. 2000.3.20
	// Hardware blinking.
	#define BlinkENVOLOPE()	hs_lcd_blink(ANN_ENV_X, ANN_ENV_Y, ANN_ENV_WIDE, ANN_ENV_HIGH, HS_LCD_WHITE_BLINK)
	#define BlinkOFF()		hs_lcd_blink(0,0,0,0,HS_LCD_BLINK_OFF)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef FEATURE_NETWORK
#ifdef RES_KOKR
	// UP의 한글입력 모드 중이면서, KorLabel이 표시되는 중이면,
	// Blink를 멈춘다. (입력종료 후 다시 Blink하도록 한다)
	if(g_bKorEntryMode && g_bDrawKorLabel) BlinkOFF();
	else
#endif//RES_KOKR
#endif//FEATURE_NETWORK
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if (lcd_blink_annun != (hs_blink_annun & (HS_AN_VMAIL|HS_AN_MSG)))
	{
		lcd_blink_annun = hs_blink_annun & (HS_AN_VMAIL|HS_AN_MSG);

		if ((lcd_blink_annun & (HS_AN_VMAIL|HS_AN_MSG)) == (HS_AN_VMAIL|HS_AN_MSG))
			BlinkENVOLOPE();
		else if (lcd_blink_annun & HS_AN_VMAIL)
			BlinkENVOLOPE();
		else if (lcd_blink_annun & HS_AN_MSG)
			BlinkENVOLOPE();
		else
			BlinkOFF();
	}
	// girin. end.
#endif    


#ifdef SW_RESET_ENABLE
  if(mc_bSwReset)
  {
      if (lcd_annun & HS_AN_ALL_OFF)
      {
        lcd_AllPictographsOff();
        nv_GetAnnValue(&lcd_annun, &lcd_rssi, &lcd_batt, &lcd_smb);
      }
      else
      {
          return;
      }
  }
  else
  {
      hs_last_annun = hs_annun;
      hs_last_rssi = hs_rssi;
      hs_last_batt = hs_batt;
      hs_last_smb = hs_smb;
  }
#endif

  if(lcd_annun & HS_AN_ALL_ON)
  {
    lcd_AllPictographsOn();
    return;
  }
  else if(lcd_annun & HS_AN_ALL_OFF)
  {
    lcd_AllPictographsOff();
    return;
  }
  else
  {
    /* RSSI Indicator */
    if (lcd_rssi >= RSSI_VAL5)   
    {
      DrawIcon(RSSI_5_BARS);
    }
    else if( lcd_rssi >= RSSI_VAL4)  
    {
      DrawIcon(RSSI_4_BARS);      
    }
    else if( lcd_rssi >= RSSI_VAL3)  
    {
      DrawIcon(RSSI_3_BARS);            
    }
    else if( lcd_rssi >= RSSI_VAL2)  
    {
      DrawIcon(RSSI_2_BARS);                  
    }
    else if( lcd_rssi >= RSSI_VAL1)  
    {
      DrawIcon(RSSI_1_BAR);                        
    }
    else                  
    {
      DrawIcon(RSSI_ANTENNA_ONLY);                              
    }                     

    /* Roam Indicator */
    if (lcd_annun & HS_AN_ROAM)
    {
	  DrawIcon(SMB_DIGIT1_OFF);
	  DrawIcon(SMB_DIGIT0_OFF);
	  DrawIcon(SMB_PERCENT_OFF);
	  DrawIcon(SMB_NOTKNOWN_OFF);
      DrawIcon(ANN_ROAM_ON);
    }
    else
    {
      DrawIcon(ANN_ROAM_OFF);
	  
	  if(lcd_smb == SMB_OFFLINE)
	  {
		  DrawIcon(SMB_DIGIT1_OFF);
		  DrawIcon(SMB_DIGIT0_OFF);
		  DrawIcon(SMB_PERCENT_OFF);
		  DrawIcon(SMB_NOTKNOWN_OFF);
	  }
	  else if(lcd_smb == SMB_FULL)
	  {
		  DrawIcon(SMB_FULL_ON);
	    DrawIcon(SMB_PERCENT_ON);
	  }
	  else if(lcd_smb < SMB_HALF)
	  {
	    if(lcd_smb <= SMB_LOW_BATT )
		{
			DrawIcon(SMB_DIGIT1_OFF);
			DrawIcon(SMB_DIGIT0_OFF);
			DrawIcon(SMB_PERCENT_OFF);
			DrawIcon(SMB_NOTKNOWN_OFF);
		}
		else
		{
			DrawIcon(SMB_DIGIT1_OFF);
			DrawIcon(SMB_DIGIT0_ON);
			DrawIcon(SMB_PERCENT_ON);
		}
	  }
	  else
	  {
	 	DrawIcon(SMB_DIGIT1_ON);
	 	DrawIcon(SMB_DIGIT0_ON);
	    DrawIcon(SMB_PERCENT_ON);
	  }

    }

    /* Call Indicator */
    if (lcd_annun & HS_AN_CDMA)
    {
#ifdef FEATURE_UI_DORMANT
	  if (lcd_annun & HS_AN_DORM)
	  {
        DrawIcon(ANN_DORM_SVC);
	  }
	  else
#endif //#ifdef FEATURE_UI_DORMANT
      if(lcd_annun & HS_AN_INUSE)
      {
        DrawIcon(ANN_INUSE_SVC);
      }
      else
      {
        DrawIcon(ANN_NOTINUSE_SVC);
      }
    }
    else
    {
      DrawIcon(ANN_NO_SVC);
    }

    /* MELODY Indicator */    
    if (lcd_annun & HS_AN_MELODY)
    {
      DrawIcon(ANN_MELODY_ON);
    }
    else
    {
      DrawIcon(ANN_MELODY_OFF);
    }

#ifdef FEATURE_ANN_ETIQUETTE
    /* Etiquette Mode Indicator */    
    if (lcd_annun & HS_AN_ETIQUETTE)
    {
      DrawIcon(ANN_ETIQ_ON);
    }
#endif

    /* Vibrator Indicator */
    if (lcd_annun & HS_AN_VIB)
    {
      DrawIcon(ANN_VIB_ON);
    }
    else
    {
      DrawIcon(ANN_VIB_OFF);
    }


    /* VMS/SMS Indicator */ 
    if((lcd_annun & HS_AN_VMAIL) || (lcd_annun & HS_AN_MSG)) 
    {
      DrawIcon(ANN_ENVOLOPE_ON);    

	  /*
      if (lcd_annun & HS_AN_VMAIL)
      {
        DrawIcon(ANN_VMAIL_ON);       
      }
      else
      {
        DrawIcon(ANN_VMAIL_OFF);
      }
      
      if (lcd_annun & HS_AN_MSG)
      {
        DrawIcon(ANN_MSG_ON);          
      }
      else
      {
        DrawIcon(ANN_MSG_OFF);
      } 
	  */
    }
    else
    {
      DrawIcon(ANN_ENVOLOPE_OFF);
      //DrawIcon(ANN_VMAIL_OFF);
      //DrawIcon(ANN_MSG_OFF);    
    }

    /* Battery Indicator */
    if(lcd_batt >= NOSMB_BATT_VAL3)
	{
      DrawIcon(NOSMB_BATT_3_BARS);
	}
    else if(lcd_batt >= NOSMB_BATT_VAL2)
	{
      DrawIcon(NOSMB_BATT_2_BARS);
	}
    else if(lcd_batt >= NOSMB_BATT_VAL1)
	{
      DrawIcon(NOSMB_BATT_1_BARS);
	}
//    else
//	if(lcd_batt == BATT_VAL_NOSMB)
//	{
//      DrawIcon(BATT_NOSMB);
//	}
	else if(lcd_batt >= BATT_VAL16 && lcd_batt <= BATT_VAL17)
    {
      DrawIcon(BATT_16_BARS);
    }
    else if(lcd_batt >= BATT_VAL16)
    {
      DrawIcon(BATT_15_BARS);
    }
    else if(lcd_batt >= BATT_VAL14)
    {
      DrawIcon(BATT_14_BARS);
    }
    else if(lcd_batt >= BATT_VAL13)
    {
      DrawIcon(BATT_13_BARS);
    }
    else if(lcd_batt >= BATT_VAL12)
    {
      DrawIcon(BATT_12_BARS);
    }
    else if(lcd_batt >= BATT_VAL11)
    {
      DrawIcon(BATT_11_BARS);
    }
    else if(lcd_batt >= BATT_VAL10)
    {
      DrawIcon(BATT_10_BARS);
    }
    else if(lcd_batt >= BATT_VAL9)
    {
      DrawIcon(BATT_9_BARS);
    }
    else if(lcd_batt >= BATT_VAL8)
    {
      DrawIcon(BATT_8_BARS);
    }
    else if(lcd_batt >= BATT_VAL7)
    {
      DrawIcon(BATT_7_BARS);
    }
    else if(lcd_batt >= BATT_VAL6)
    {
      DrawIcon(BATT_6_BARS);
    }
    else if(lcd_batt >= BATT_VAL5)
    {
      DrawIcon(BATT_5_BARS);
    }
    else if(lcd_batt >= BATT_VAL4)
    {
      DrawIcon(BATT_4_BARS);
    }
    else if(lcd_batt >= BATT_VAL3)
    {
      DrawIcon(BATT_3_BARS);
    }
    else if(lcd_batt >= BATT_VAL2)
    {
      DrawIcon(BATT_2_BARS);
    }
    else if(lcd_batt >= BATT_VAL1)
    {
      DrawIcon(BATT_1_BAR);
    }
	// Comment-out below 'else if clause' cause only four batt. level used. 
    //else if(lcd_batt >= BATT_VAL0)
    //{
    //  DrawIcon(BATT_BORDER_ONLY);    
    //}
	else // girin. add. 2000.4.24
	{
      DrawIcon(BATT_NONE);    
    }

  } /*  if(lcd_annun & HS_AN_ALL_ON)*/

  DisplayIconBufferAll();

} /* end of hs_update_annun */


static void ClearIconBufferAll (void)
{
  memset(&IconBuffer,0x00,sizeof(IconBuffer));
  memset(&IconBlinkBuffer,0x00,sizeof(IconBuffer));

}/*ClearIconBufferAll*/


////////////////////////////////////////////////////////////////
//
//  Display IconBuffer to LCD Controller
//
//static void DisplayIconBufferAll (void)
// tasik 00.11.22 self-check에서 쓰기 위해서 
void DisplayIconBufferAll (void)
{
  int x,y;

  // for LCD test by msrock
  if (LcdTestFlag) return;

  if(!bInvisable_Charge)		// off 충전 모드가 아닌 경우에만 실행함 
  {  
	  /*----------------------------------------------------
	     00.05.15
	     LCD 폭주 현상이 일어날 경우를 대비하여 
	     LCD Display시마다 Initialize를 한다.
	  -----------------------------------------------------*/
	  hs_lcd_reset();
  }

  hs_lcd_reset_instruction();
  hs_lcd_cmd_out(HS_LCD_R17);
  hs_lcd_word_dat_out(HS_LCD_START_ADDR);
 
  hs_lcd_reset_instruction();
  hs_lcd_cmd_out(HS_LCD_R18);

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef FEATURE_NETWORK
#ifdef RES_KOKR
	// UP의 한글입력 모드 중이면서, KorLabel이 표시되는 중이면,
	// System Indicator 영역에 한글자판을 표시한다
	if(g_bKorEntryMode && g_bDrawKorLabel)
	{
		extern int g_nKorMode;
		extern void CopyKoreanKeypadIcon(int nKorMode);

		CopyKoreanKeypadIcon(g_nKorMode);
	}
#endif//RES_KOKR
#endif//FEATURE_NETWORK
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  for (y = 0 ; y < ICON_HIGH ; y++)
    for (x=0; x <LCD_TOTAL_ADDRESS_X ; x++)
      hs_lcd_onebit_dat_out(IconBuffer[y][x]);

}/* DisplayIconBufferAll() */

static void  lcd_AllPictographsOn(void)
{
  ClearIconBufferAll();
  CopyIconBlock(pIconBuffer,0,0,pIconPicture,0,0,
                    (LCD_TOTAL_ADDRESS_X-1)*8,ICON_HIGH);
  DisplayIconBufferAll();

} /*lcd_AllPictographsOn()*/

static void lcd_AllPictographsOff(void)
{
  ClearIconBufferAll();
  DisplayIconBufferAll();

} /*lcd_AllPictographsOff()*/

////////////////////////////////////////////////////////////////
//
//  Drawing handler for hs_update_ann()
//
LOCAL void DrawIcon(ICON_PICTURE_TYPE picture) {

  byte lcd_smb;
  byte smb_index;

  switch (picture) {

    /* Draw Rssi */                   
    case RSSI_5_BARS:    
      CopyIconBlock(pIconBuffer,RSSI_X,RSSI_Y,
                   pIconPicture,RSSI_5_BARS_X,RSSI_5_BARS_Y,
                   RSSI_WIDE,RSSI_HIGH); break;
    case RSSI_4_BARS:    
      CopyIconBlock(pIconBuffer,RSSI_X,RSSI_Y,
                   pIconPicture,RSSI_4_BARS_X,RSSI_4_BARS_Y,
                   RSSI_WIDE,RSSI_HIGH); break;
    case RSSI_3_BARS:    
      CopyIconBlock(pIconBuffer,RSSI_X,RSSI_Y,
                   pIconPicture,RSSI_3_BARS_X,RSSI_3_BARS_Y,
                   RSSI_WIDE,RSSI_HIGH); break;
    case RSSI_2_BARS:    
      CopyIconBlock(pIconBuffer,RSSI_X,RSSI_Y,
                   pIconPicture,RSSI_2_BARS_X,RSSI_2_BARS_Y,
                   RSSI_WIDE,RSSI_HIGH); break;
    case RSSI_1_BAR:    
      CopyIconBlock(pIconBuffer,RSSI_X,RSSI_Y,
                   pIconPicture,RSSI_1_BAR_X,RSSI_1_BAR_Y,
                   RSSI_WIDE,RSSI_HIGH); break;
    case RSSI_ANTENNA_ONLY:    
      CopyIconBlock(pIconBuffer,RSSI_X,RSSI_Y,
                   pIconPicture,RSSI_ANTENNA_ONLY_X,RSSI_ANTENNA_ONLY_Y,
                   RSSI_WIDE,RSSI_HIGH); break;
    case RSSI_NONE:    
      CopyIconBlock(pIconBuffer,RSSI_X,RSSI_Y,
                   pIconPicture,RSSI_NONE_X,RSSI_NONE_Y,
                   RSSI_WIDE,RSSI_HIGH); break;        

    /* Draw Annun */                   
    case ANN_ROAM_ON:
      CopyIconBlock(pIconBuffer,ANN_ROAM_X,ANN_ROAM_Y,
                   pIconPicture,ANN_ROAM_ON_X,ANN_ROAM_ON_Y,
                   ANN_ROAM_WIDE,ANN_ROAM_HIGH); break;

    case ANN_ROAM_OFF:
      CopyIconBlock(pIconBuffer,ANN_ROAM_X,ANN_ROAM_Y,
                   pIconPicture,ANN_ROAM_OFF_X,ANN_ROAM_OFF_Y,
                   ANN_ROAM_WIDE,ANN_ROAM_HIGH);break;

    case ANN_INUSE_SVC:
      CopyIconBlock(pIconBuffer,ANN_SVC_X,ANN_SVC_Y,
                   pIconPicture,ANN_SVC_INUSE_X,ANN_SVC_INUSE_Y,
                   ANN_SVC_WIDE,ANN_SVC_HIGH);break;

    case ANN_NOTINUSE_SVC:
      CopyIconBlock(pIconBuffer,ANN_SVC_X,ANN_SVC_Y,
                   pIconPicture,ANN_SVC_NOTINUSE_X,ANN_SVC_NOTINUSE_Y,
                   ANN_SVC_WIDE,ANN_SVC_HIGH);break;

    case ANN_NO_SVC:
      CopyIconBlock(pIconBuffer,ANN_SVC_X,ANN_SVC_Y,
                   pIconPicture,ANN_SVC_NONE_X,ANN_SVC_NONE_Y,
                   ANN_SVC_WIDE,ANN_SVC_HIGH); break;
#ifdef FEATURE_UI_DORMANT
	case ANN_DORM_SVC:
      CopyIconBlock(pIconBuffer,ANN_SVC_X,ANN_SVC_Y,
                   pIconPicture,ANN_DORM_X,ANN_DORM_Y,
                   ANN_SVC_WIDE,ANN_SVC_HIGH); break;
#endif //#ifdef FEATURE_UI_DORMANT

    case ANN_VIB_ON:
      CopyIconBlock(pIconBuffer,ANN_VIB_X,ANN_VIB_Y,
                   pIconPicture,ANN_VIB_ON_X,ANN_VIB_ON_Y,
                   ANN_VIB_WIDE,ANN_VIB_HIGH);break;

    case ANN_VIB_OFF:
      CopyIconBlock(pIconBuffer,ANN_VIB_X,ANN_VIB_Y,
                   pIconPicture,ANN_VIB_OFF_X,ANN_VIB_OFF_Y,
                   ANN_VIB_WIDE,ANN_VIB_HIGH); break;


    case ANN_ENVOLOPE_ON:
      CopyIconBlock(pIconBuffer,ANN_ENV_X,ANN_ENV_Y,
                   pIconPicture,ANN_ENV_ON_X,ANN_ENV_ON_Y,
                   ANN_ENV_WIDE,ANN_ENV_HIGH); break;

    case ANN_ENVOLOPE_OFF:
      CopyIconBlock(pIconBuffer,ANN_ENV_X,ANN_ENV_Y,
                   pIconPicture,ANN_ENV_OFF_X,ANN_ENV_OFF_Y,
                   ANN_ENV_WIDE,ANN_ENV_HIGH); break;


    case ANN_MSG_ON:
      CopyIconBlock(pIconBuffer,ANN_MSG_X,ANN_MSG_Y,
                   pIconPicture,ANN_MSG_ON_X,ANN_MSG_ON_Y,
                   ANN_MSG_WIDE,ANN_MSG_HIGH); break;

    case ANN_MSG_OFF:
      CopyIconBlock(pIconBuffer,ANN_MSG_X,ANN_MSG_Y,
                   pIconPicture,ANN_MSG_OFF_X,ANN_MSG_OFF_Y,
                   ANN_MSG_WIDE,ANN_MSG_HIGH); break;

    case ANN_VMAIL_ON:
      CopyIconBlock(pIconBuffer,ANN_VMAIL_X,ANN_VMAIL_Y,
                   pIconPicture,ANN_VMAIL_ON_X,ANN_VMAIL_ON_Y,
                   ANN_VMAIL_WIDE,ANN_VMAIL_HIGH); break;


    case ANN_VMAIL_OFF:
      CopyIconBlock(pIconBuffer,ANN_VMAIL_X,ANN_VMAIL_Y,
                   pIconPicture,ANN_VMAIL_OFF_X,ANN_VMAIL_OFF_Y,
                   ANN_VMAIL_WIDE,ANN_VMAIL_HIGH);break;


    case ANN_MELODY_ON:
      CopyIconBlock(pIconBuffer,ANN_MELODY_X,ANN_MELODY_Y,
                   pIconPicture,ANN_MELODY_ON_X,ANN_MELODY_ON_Y,
                   ANN_MELODY_WIDE,ANN_MELODY_HIGH); break;


    case ANN_MELODY_OFF:
      CopyIconBlock(pIconBuffer,ANN_MELODY_X,ANN_MELODY_Y,
                   pIconPicture,ANN_MELODY_OFF_X,ANN_MELODY_OFF_Y,
                   ANN_MELODY_WIDE,ANN_MELODY_HIGH); break;

#ifdef FEATURE_ANN_ETIQUETTE
    case ANN_ETIQ_ON:
      CopyIconBlock(pIconBuffer,ANN_MELODY_X,ANN_MELODY_Y,
                   pIconPicture,ANN_ETIQ_X,ANN_ETIQ_Y,
                   ANN_ETIQ_WIDE,ANN_ETIQ_HIGH); break;
#endif

	case SMB_FULL_ON:
      CopyIconBlock(pIconBuffer,SMB_FULL_X,SMB_FULL_Y,
                   pIconPicture,SMB_FULL_ON_X,SMB_FULL_ON_Y,
                   SMB_FULL_WIDE,SMB_FULL_HIGH); break;

    case SMB_DIGIT1_ON:
	  lcd_smb = hs_smb;
	  smb_index = lcd_smb/10;
	  if((smb_index%2) == 0)
	  {
		  smb_index = smb_index/2;
		  CopyIconBlock(pIconBuffer,SMB_DIGIT1_X,SMB_DIGIT1_Y,
					   pIconPicture,SMB_DIGIT1_ON_X,SMB_DIGIT1_ON_Y+smb_index*14,
					   SMB_DIGIT1_WIDE,SMB_DIGIT1_HIGH); break;
	  }
	  else
	  {
		  smb_index = smb_index/2;
		  CopyIconBlock(pIconBuffer,SMB_DIGIT1_X,SMB_DIGIT1_Y,
					   pIconPicture,SMB_DIGIT0_ON_X,SMB_DIGIT0_ON_Y+smb_index*14,
					   SMB_DIGIT1_WIDE,SMB_DIGIT1_HIGH); break;
	  }

    case SMB_DIGIT1_OFF:
      CopyIconBlock(pIconBuffer,SMB_DIGIT1_X,SMB_DIGIT1_Y,
                   pIconPicture,SMB_DIGIT1_OFF_X,SMB_DIGIT1_OFF_Y,
                   SMB_DIGIT1_WIDE,SMB_DIGIT1_HIGH);break;

    case SMB_DIGIT0_ON:
	  lcd_smb = hs_smb;
	  smb_index = lcd_smb%10;

	  if((smb_index%2) == 0)
	  {
		  smb_index = smb_index/2;
		  CopyIconBlock(pIconBuffer,SMB_DIGIT0_X,SMB_DIGIT0_Y,
					   pIconPicture,SMB_DIGIT1_ON_X,SMB_DIGIT1_ON_Y+smb_index*14,
					   SMB_DIGIT0_WIDE,SMB_DIGIT0_HIGH); break;
	  }
	  else
	  {
		  smb_index = smb_index/2;
		  CopyIconBlock(pIconBuffer,SMB_DIGIT0_X,SMB_DIGIT0_Y,
					   pIconPicture,SMB_DIGIT0_ON_X,SMB_DIGIT0_ON_Y+smb_index*14,
					   SMB_DIGIT0_WIDE,SMB_DIGIT0_HIGH); break;
	  }

    case SMB_DIGIT0_OFF:
      CopyIconBlock(pIconBuffer,SMB_DIGIT0_X,SMB_DIGIT0_Y,
                   pIconPicture,SMB_DIGIT0_OFF_X,SMB_DIGIT0_OFF_Y,
                   SMB_DIGIT0_WIDE,SMB_DIGIT0_HIGH);break;

    case SMB_NOTKNOWN_ON:
      CopyIconBlock(pIconBuffer,SMB_NOTKNOWN_X,SMB_NOTKNOWN_Y,
                   pIconPicture,SMB_NOTKNOWN_ON_X,SMB_NOTKNOWN_ON_Y,
                   SMB_NOTKNOWN_WIDE,SMB_NOTKNOWN_HIGH); break;

    case SMB_NOTKNOWN_OFF:
      CopyIconBlock(pIconBuffer,SMB_NOTKNOWN_X,SMB_NOTKNOWN_Y,
                   pIconPicture,SMB_NOTKNOWN_OFF_X,SMB_NOTKNOWN_OFF_Y,
                   SMB_NOTKNOWN_WIDE,SMB_NOTKNOWN_HIGH);break;

    case SMB_PERCENT_ON:
      CopyIconBlock(pIconBuffer,SMB_PERCENT_X,SMB_PERCENT_Y,
                   pIconPicture,SMB_PERCENT_ON_X,SMB_PERCENT_ON_Y,
                   SMB_PERCENT_WIDE,SMB_PERCENT_HIGH); break;

    case SMB_PERCENT_OFF:
      CopyIconBlock(pIconBuffer,SMB_PERCENT_X,SMB_PERCENT_Y,
                   pIconPicture,SMB_PERCENT_OFF_X,SMB_PERCENT_OFF_Y,
                   SMB_PERCENT_WIDE,SMB_PERCENT_HIGH);break;

    /* Draw Battery */
    case NOSMB_BATT_3_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,NOSMB_BATT_3_BARS_X,NOSMB_BATT_3_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case NOSMB_BATT_2_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,NOSMB_BATT_2_BARS_X,NOSMB_BATT_2_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case NOSMB_BATT_1_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,NOSMB_BATT_1_BARS_X,NOSMB_BATT_1_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;
/*
	case BATT_NOSMB:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_NOSMB_X,BATT_NOSMB_Y,
                   BATT_WIDE,BATT_HIGH); break;
*/
    case BATT_16_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_16_BARS_X,BATT_16_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_15_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_15_BARS_X,BATT_15_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_14_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_14_BARS_X,BATT_14_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_13_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_13_BARS_X,BATT_13_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_12_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_12_BARS_X,BATT_12_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_11_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_11_BARS_X,BATT_11_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_10_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_10_BARS_X,BATT_10_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_9_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_9_BARS_X,BATT_9_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_8_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_8_BARS_X,BATT_8_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_7_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                       pIconPicture,BATT_7_BARS_X,BATT_7_BARS_Y,
                       BATT_WIDE,BATT_HIGH); break;

    case BATT_6_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_6_BARS_X,BATT_6_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_5_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_5_BARS_X,BATT_5_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_4_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                       pIconPicture,BATT_4_BARS_X,BATT_4_BARS_Y,
                       BATT_WIDE,BATT_HIGH); break;

    case BATT_3_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_3_BARS_X,BATT_3_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_2_BARS:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_2_BARS_X,BATT_2_BARS_Y,
                   BATT_WIDE,BATT_HIGH); break;

    case BATT_1_BAR:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                       pIconPicture,BATT_1_BAR_X,BATT_1_BAR_Y,
                       BATT_WIDE,BATT_HIGH); break;

    case BATT_BORDER_ONLY:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                       pIconPicture,BATT_BODER_ONLY_X,BATT_BODER_ONLY_Y,
                       BATT_WIDE,BATT_HIGH); break;

    case BATT_NONE:
      CopyIconBlock(pIconBuffer,BATT_X,BATT_Y,
                   pIconPicture,BATT_NONE_X,BATT_NONE_Y,
                   BATT_WIDE,BATT_HIGH); break;
                       
                   
                       
    }//switch

}/* DrawIcon() */

///////////////////////////////////////////////////////////////////////////////
//
//  Copy to IconBuffer
//

LOCAL void  CopyIconBlock(byte* pDest, byte dx, byte dy,
             byte* pSrc,  byte sx, byte sy, byte sw, byte sh)
{
  byte  x, y;
  byte  color;
//  byte  mask = 0x80;	// woodstok 00.4.27 delete


  for (x = 0; x < sw; x++)
    for (y = 0; y < sh; y++) {
      color = GetIconPicture(pSrc, sx + x, sy + y);
      SetIconPicture(pDest, dx + x, dy + y, color);
   }
} /*CopyIconBlock()*/



///////////////////////////////////////////////////////////////////////////////
//
//  Set pixel to IconBuffer
//
LOCAL void  SetIconPicture(byte* data, byte x, byte y, byte color)
{

  byte  mask = 0x80;
  data += y*LCD_TOTAL_ADDRESS_X;
  data += x/8;
  mask >>= x%8;

  if (color)
    *data |= mask;
  else
    *data &= ~mask;
} /*SetIconPicture()*/

///////////////////////////////////////////////////////////////////////////////
//
//  Get pixel from an icon image (.def)
//
LOCAL byte  GetIconPicture(byte* data, byte x, byte y)
{

  byte  mask = 0x80;

  data += y*LCD_TOTAL_ADDRESS_X;
  data += x/8;
  mask >>= x%8;

  return (*data & mask) ? 0x01 : 0x00;

} /*GetIconPicture()*/

#ifdef FEATURE_NETWORK
	/*
	*  Korean Keypad Image Source File Include
	*/
	#ifdef RES_KOKR

	const byte srcKorPadJaum1[ICON_HIGH][LCD_TOTAL_ADDRESS_X] = {
	  #include "..\UP\\TextEntry\KorJaum1.def"//자음1
	};
	const byte srcKorPadJaum2[ICON_HIGH][LCD_TOTAL_ADDRESS_X] = {
	  #include "..\UP\\TextEntry\KorJaum2.def"//자음2
	};
	const byte srcKorPadMoum1[ICON_HIGH][LCD_TOTAL_ADDRESS_X] = {
	  #include "..\UP\\TextEntry\KorMoum1.def"//모음1
	};
	const byte srcKorPadMoum2[ICON_HIGH][LCD_TOTAL_ADDRESS_X] = {
	  #include "..\UP\\TextEntry\KorMoum2.def"//모음2
	};
	void CopyKoreanKeypadIcon(int nKorMode)
	{
		byte *pIcon = NULL;
		switch(nKorMode)
		{
			case 0:	pIcon = (byte *) srcKorPadJaum1; break;//자음1
			case 1:	pIcon = (byte *) srcKorPadJaum2; break;//자음2
			case 2:	pIcon = (byte *) srcKorPadMoum1; break;//모음1
			case 3:	pIcon = (byte *) srcKorPadMoum2; break;//모음2
			default : return;
		}
		CopyIconBlock(	pIconBuffer,	0, 0,	//destination (x,y)
						pIcon,			0, 0,	//source	  (x,y)
						(LCD_TOTAL_ADDRESS_X-1)*8, ICON_HIGH);//width, height
	}

	void DrawKoreanKeypad(int nKorMode)
	{
		ClearIconBufferAll();

		if(3 < nKorMode || nKorMode < 0)
		{
			//Blink 여부를 확인하여, 이를 재설정한다
			if(hs_blink_annun & (HS_AN_VMAIL|HS_AN_MSG))
				hs_lcd_blink(ANN_ENV_X, ANN_ENV_Y, ANN_ENV_WIDE, ANN_ENV_HIGH, HS_LCD_WHITE_BLINK);

			//System Indicator 영역을 복원한다
			hs_update_annun();
		}
		else
		{
			//Blink를 멈춘다
			hs_lcd_blink(0,0,0,0,HS_LCD_BLINK_OFF);// Hardware blinking.

			//System Indicator 영역에 한글자판을 표시한다(for UP)
			CopyKoreanKeypadIcon(nKorMode);
			DisplayIconBufferAll();
		}
	}
	#endif// RES_KOKR
#endif//feature_network

#ifdef BLINK_LCDANN
///////////////////////////////////////////////////////////////////////////////
//
// Set Rssi Blink Coordinate
//
void SetRssiBlink(void) {

    DrawBlinkIcon(RSSI_X,RSSI_Y,RSSI_WIDE,RSSI_HIGH); 
    
} /*SetRssiBlink()*/

///////////////////////////////////////////////////////////////////////////////
//
// Set Batt Blink Coordinate
//
void SetBattBlink(void) 
{ 
    DrawBlinkIcon(BATT_X,BATT_Y,BATT_WIDE,BATT_HIGH); 

} /*SetBattBlink()*/

///////////////////////////////////////////////////////////////////////////////
//
// Set Annun Blink Coordinate
//
void SetAnnunBlink(word lcd_annun,word lcd_blink_annun) {


    /* SMS blink set*/
    if (((lcd_annun & HS_AN_MSG) && (lcd_blink_annun & HS_AN_MSG)) 
        && (lcd_annun & HS_AN_ENVOLOPE))
    {
      DrawBlinkIcon(ANN_MSG_X,ANN_MSG_Y,ANN_MSG_WIDE,ANN_MSG_HIGH);      
      DrawBlinkIcon(ANN_ENV_X,ANN_ENV_Y,ANN_ENV_WIDE,ANN_ENV_HIGH); 

    }

    /* VMS blink set*/
    if (((lcd_annun & HS_AN_VMAIL) && (lcd_blink_annun & HS_AN_VMAIL)) 
        && (lcd_annun & HS_AN_ENVOLOPE))

    {
      DrawBlinkIcon(ANN_VMAIL_X,ANN_VMAIL_Y,ANN_VMAIL_WIDE,ANN_VMAIL_HIGH); 
      DrawBlinkIcon(ANN_ENV_X,ANN_ENV_Y,ANN_ENV_WIDE,ANN_ENV_HIGH); 
    }
    


}/*SetAnnunBlink()*/

///////////////////////////////////////////////////////////////////////////////
//
// Get blinking information of specified coordinate from IconBuffer 
//  & Set that information to IconBlinkBuffer.
//
LOCAL void  DrawBlinkIcon(byte x1, byte y1, byte dx, byte dy)
{
  byte  x2 = x1 + dx, y2 = y1 + dy;
  byte  x, y;


  for (x = x1; x <= x2; x++)
    for (y = y1; y <= y2; y++)
      if (GetBufPixel((byte*)IconBuffer, x, y))
         SetBufPixel((byte*)IconBlinkBuffer, x, y, 1);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Get pixel from IconBuffer
//
LOCAL byte GetBufPixel(byte* data, byte x, byte y)
{
  byte  mask = 0x80;
  data += y*LCD_TOTAL_ADDRESS_X; 
  data += x/8;
  mask >>= x%8; 

  return (*data & mask) ? 0x01 : 0x00;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Set pixel to IconBlinkBuffer
//
void  SetBufPixel(byte* data, byte x, byte y, byte color)
{
  byte  mask = 0x80;
  data += y*LCD_TOTAL_ADDRESS_X; 
  data += x/8;
  mask >>= x%8;

  if (color)
    *data |= mask;
  else
    *data &= ~mask;
}
void BlinkIconBuffer ( void )
{
	int	nIndex;

	// LCD Icon buffer length 만큼 loop
	for ( nIndex = 0; nIndex < sizeof ( IconBlinkBuffer ) ; nIndex ++ )
	{
		// blink bit 가 있는지 확인
		if ( *( (byte *)IconBlinkBuffer + nIndex ) != 0x00 )
		{
			// blink bit position의 data bit를 반전
			*( (byte *)IconBuffer + nIndex ) ^=
				*( (byte *)IconBlinkBuffer + nIndex );
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
//
//  hs_icon_timer function 
//
static int blink_toggle=0;
void lcd_icon_display(void) { //every 300ms

  if ( bBlinkMode) {
    /* current pictograph state is blink mode */  
    
    if (blink_toggle) 
    {
      BlinkIconBuffer ();  /* Invert blink area of IconBuffer[][]*/      
      DisplayIconBufferAll();   
      blink_toggle=0;
    } 
    else
    {
      DisplayIconBufferAll();         
      blink_toggle=1;
    }      
  }   
  else     
  {
    /* current pictograph state is non-blink mode */  
    //memcpy(IconBuffer,IconBlinkBuffer,sizeof(IconBuffer)); 
    DisplayIconBufferAll();           
  } 
  
}/*lcd_icon_display()*/


#endif /*BLINK_LCDANN 2000.1.24*/
#endif // #if (MMI_TYPE == MMI_A10) // jwpark 00.08.23

