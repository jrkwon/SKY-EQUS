#ifndef WAP_GENERAL_DISPLAY_H
#define WAP_GENERAL_DISPLAY_H

//////////////////////////////////////////////////////////////////////////////
//																			//
//						 General graphic library							//
//																			//
//////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------
// WAP B0
#if !((defined WAP_B1) || (defined WAP_B2))
//-------------------------------------------------------------
#define WAP_LCD_PIXELS						16256//3072

#define WAP_SOFTKEY_HEIGHT					13//WAP_ASCII_FONT_HEIGHT
#define WAP_DOC_HEIGHT						91//104//13*8//WAP_LCD_HEIGHT - WAP_ASCII_FONT_HEIGHT
#define WAP_SOFTKEY_Y_POS					91//114(127-13) -16 //WAP_LCD_HEIGHT - WAP_ASCII_FONT_HEIGHT
#define WAP_HEAD_HEIGHT						16

#define WAP_TEXT_ALIGN_LEFT                  0
#define WAP_TEXT_ALIGN_CENTER                1
#define WAP_TEXT_ALIGN_RIGHT                 2 

// -------------------------------------------------------------
// FONT SIZE DEFINITION
#define	WAP_FONT_NORMAL					0
#define WAP_FONT_MEDIUM					1
#define	WAP_FONT_BIG					2
#define	WAP_FONT_SMALL					3
#define	WAP_FONT_CALENDAR				4

// ------------------------------------------------------------
#define WAP_WHITE			0x00
#define WAP_WHITEGRAY		0x01
#define WAP_BLACKGRAY		0x02
#define WAP_BLACK			0x03

// ---------------------------------------------------------------
#define WAP_HS_SCRN_HIGH  8       /* Screen height in characters */
#define WAP_HS_SCRN_WIDE  21      /* Screen width in characters  */

#define WAP_HS_EMPTY_BOX    ((byte) 0xDC)   /* Empty box            */
#define WAP_HS_FILL_BOX     ((byte) 0x8F)   /* Filled box           */

#define WAP_LCD_WIDTH					120		
#define WAP_LCD_HEIGHT					104		//127-16-7	

#define WAP_LCD_TOTAL_ADDRESS_X			16*2	
#define WAP_LCD_TOTAL_ADDRESS_Y			127	

#define WAP_MAX_TEXT						255

//-------------------------------------------------------------
// WAP B1
#else//#ifndef WAP_B1
//-------------------------------------------------------------
#define WAP_LCD_PIXELS						128*160//16256//3072

#define WAP_SOFTKEY_HEIGHT					SKY_COMMANDLINE_HEIGHT//16
#define WAP_DOC_HEIGHT						120//15*8
#define WAP_SOFTKEY_Y_POS					123//WAP_LCD_HEIGHT - (WAP_SOFTKEY_HEIGHT + WAP_HEAD_HEIGHT)
#define WAP_HEAD_HEIGHT						SKY_ANNUNCIATORS_HEIGHT + 2//19 + 2

#define WAP_TEXT_ALIGN_LEFT                  0
#define WAP_TEXT_ALIGN_CENTER                1
#define WAP_TEXT_ALIGN_RIGHT                 2 

// -------------------------------------------------------------
// FONT SIZE DEFINITION
#define	WAP_FONT_NORMAL					0
#define WAP_FONT_MEDIUM					1
#define	WAP_FONT_BIG					2
#define	WAP_FONT_SMALL					3
#define	WAP_FONT_CALENDAR				4

// ------------------------------------------------------------
#define RGB332(r,g,b)       ((((r)/32) << 5) | (((g)/32) << 2) | ((b)/64))

#define WAP_BLACK        RGB332(0,0,0)      //0x00       
#define WAP_BLUE         RGB332(0,0,255)    //0x03     
#define WAP_GREEN        RGB332(0,255,0)    //0x1C 
#define WAP_CYAN         RGB332(0,255,255)  //0x1F        
#define WAP_RED          RGB332(255,0,0)    //0xD0     
#define WAP_MAGENTA      RGB332(255,0,255)  //0xD3   
#define WAP_BROWN        RGB332(128,0,0)    //0x80     
#define WAP_GRAY         RGB332(128,128,128)//0x92 
#define WAP_LIGHTGRAY    RGB332(192,192,192)//0xDB 
#define WAP_LIGHTBLUE    RGB332(64,64,255)  //0x4B   
#define WAP_LIGHTGREEN   RGB332(64,255,64)  //0x5C   
#define WAP_LIGHTCYAN    RGB332(64,255,255) //0x5F  
#define WAP_LIGHTRED     RGB332(255,64,64)  //0xD9   
#define WAP_LIGHTMAGENTA RGB332(255,64,255) //0xDB
#define WAP_YELLOW       RGB332(255,255,0)  //0xFC   
#define WAP_WHITE        RGB332(255,255,255)//0xFF

//----------------------------------------------------------------
#define WAP_TITLE_BACK_COLOR    BLUE//83
#define WAP_SELECT_BACK_COLOR   BLACK//1
#define WAP_SELECT_TEXT_COLOR   WHITE//255   
#define WAP_BASIC_BACK_COLOR    WHITE//255
#define WAP_BASIC_TEXT_COLOR    BLACK//0

// ---------------------------------------------------------------
#define WAP_HS_SCRN_HIGH  9       /* Screen height in characters */
#define WAP_HS_SCRN_WIDE  18      /* Screen width in characters  */

#define WAP_HS_EMPTY_BOX    ((byte) 0xDC)   /* Empty box            */
#define WAP_HS_FILL_BOX     ((byte) 0x8F)   /* Filled box           */

#define WAP_LCD_WIDTH					SKY_SCREEN_WIDTH//128		
#define WAP_LCD_HEIGHT					SKY_SCREEN_HEIGHT - SKY_ANNUNCIATORS_HEIGHT//146		//160-14	

#define WAP_LCD_TOTAL_ADDRESS_X			SKY_SCREEN_WIDTH//128	
#define WAP_LCD_TOTAL_ADDRESS_Y			SKY_SCREEN_HEIGHT//160	

#define WAP_MAX_TEXT						255

//-------------------------------------------------------------
#endif//#ifndef WAP_B1
//-------------------------------------------------------------


/*--------------------------------------------------------------------------------*/ 	

#define WAP_CHAR_BIT_WIDTH							8

// The number of characters in case of medium font
#define WAP_MED_SCREEN_WIDTH						21		//--> 바꿔야함
// The number of characters in case of standard large font
#define WAP_SCREEN_WIDTH							WAP_HS_SCRN_WIDE
// The number of characters in case of medium font
#define WAP_BIG_SCREEN_WIDTH						16		

// ---------------------------------------------------------------------------------
// Medium font 12*13														//--> 바꿔야함
#define	WAP_MED_HAN_FONT_WIDTH						12 
#define	WAP_MED_HAN_FONT_HEIGHT						13 
#define	WAP_MED_HAN_FONT_BUFFER_SIZE_PER_CHAR		26 	/* byte */

#define WAP_MED_ASCII_FONT_WIDTH					6						//--> 바꿔야함  
#define WAP_MED_ASCII_FONT_HEIGHT					13 
#define WAP_MED_ASCII_FONT_BUFFER_SIZE_PER_CHAR		13 	/* byte */

// Standard font 12*13
#define	WAP_HAN_FONT_WIDTH							14//12 
#define	WAP_HAN_FONT_HEIGHT							15//13 
#define	WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR			26 	/* byte */

#define	WAP_ASCII_FONT_WIDTH						7//6
#define	WAP_ASCII_FONT_HEIGHT						15//13 
#define	WAP_ASCII_FONT_BUFFER_SIZE_PER_CHAR			13 	/* byte */

//Big font 14*16
#define	WAP_BIG_HAN_FONT_WIDTH						14 
#define	WAP_BIG_HAN_FONT_HEIGHT						16 
#define	WAP_BIG_HAN_FONT_BUFFER_SIZE_PER_CHAR		32 	/* byte */

#define WAP_BIG_ASCII_FONT_WIDTH					7						  
#define WAP_BIG_ASCII_FONT_HEIGHT					16 
#define WAP_BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR		16 	/* byte */


#define	WAP_BIG_BIG_ASCII_FONT_WIDTH					10					
#define	WAP_BIG_BIG_ASCII_FONT_HEIGHT					21
#define	WAP_BIG_BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR		42	/* byte */

#define	WAP_SMALL_ASCII_FONT_WIDTH					6						
#define	WAP_SMALL_ASCII_FONT_HEIGHT					8
#define	WAP_SMALL_ASCII_FONT_BUFFER_SIZE_PER_CHAR	6	/* byte */

#define	WAP_CALENDAR_FONT_WIDTH						5						//--> 바꿔야함
#define	WAP_CALENDAR_FONT_HEIGHT					6
#define	WAP_CALENDAR_FONT_BUFFER_SIZE_PER_CHAR		8	/* byte */


// The margin number of pixels when you use MED_SIZE_HAN_FONT font
#define WAP_LCD_LEFT_MARGIN							4 /* bits */
#define WAP_ICON_WIDTH								28//14*2
/*--------------------------------------------------------------------------------*/ 	

#define WAP_HS_LCD_GRAY_SCALE_ON				0x01
#define WAP_HS_LCD_CHECK_BIT					0x80
#define WAP_HS_LCD_MASK_BIT						0xc000

/*--------------------------------------------------------------------------------*/ 	

/* Define for Draw Mode */
#define	WAP_DRAW_NORMAL								0			/* Normal            */
#define	WAP_DRAW_FOREGROUND_BLINK					1			/* Character Blink   */
#define	WAP_DRAW_BACKGROUND_BLINK					2			/* Background Blink  */
#define	WAP_DRAW_REVERSE							3			/* Character Reverse */
#define	WAP_DRAW_NORMAL_BLINK						0x81		/* Normal Blink      */
#define	WAP_DRAW_REVERSE_BLINK						0x82		/* Reverse Blink     */
#define	WAP_DRAW_BLINK								WAP_DRAW_FOREGROUND_BLINK
#define WAP_DRAW_SCROLL								5
#define WAP_DRAW_PICTURE_MATE						6			// Picture mate mode
/*--------------------------------------------------------------------------------*/ 	

/* Bits per pixel */
#define WAP_BW_BITS_PER_PIXEL                       1
#define WAP_4GRAY_BITS_PER_PIXEL                    2
#define WAP_16COLOR_BITS_PER_PIXEL                  4
#define WAP_256COLOR_BITS_PER_PIXEL                 8           //1byte

/*--------------------------------------------------------------------------------*/
/* Image data size */
#define WAP_BW_IMAGE_SIZE                       1800    //(120/8)*120 bytes
#define WAP_4GRAY_IMAGE_SIZE                    3600    //(120/4)*120 bytes
#define WAP_256COLOR_IMAGE_SIZE                 14400   //120*120 bytes 


/*--------------------------------------------------------------------------------*/
/* WAP Loading Bar color */
extern ROM INT g_WapLoadingBarColor[6][6];

/*--------------------------------------------------------------------------------*/
/* WAP Command Line Image */
enum{          
    // THE KIND OF CMLS
    WAP_LEFT_CML     = 0,
    WAP_UPDOWN_CML   = 1,
    WAP_OK_CML       = 2,
    WAP_CENTER_CML   = 3,
    WAP_RIGHT_CML    = 4,

    WAP_LEFTRIGHT_CML_WIDTH   = 40,//36,  
    WAP_UPDOWN_CML_WIDTH      = 10,
    WAP_OK_CML_WIDTH          = 9,//WAP_UPDOWNOK_CML_WIDTH    = 9,
    WAP_CENTER_CML_WIDTH      = 21,//30,  
    WAP_CML_HEIGHT            = 13,

    // THE SIZE OF A CM
    WAP_LEFTRIGHT_CML_SIZE    = 10*13,//40*13    
    WAP_UPDOWN_CML_SIZE       = 3*13,  //10*13    
    WAP_OK_CML_SIZE           = 3*13,  //9*13
    WAP_CENTER_CML_SIZE       = 6*13//273   //21*13    
};  

enum{                       
    WAP_LEFT_CML_X     = 0,
    WAP_UPDOWN_CML_X   = 40,//36,
    WAP_OK_CML_X       = 50,//45,
    WAP_CENTER_CML_X   = 59,//54,
    WAP_RIGHT_CML_X    = 80//84
};

enum{                               // WAP LEFT CML
    WAP_CML_LEFTEMPTY   = 0,
    WAP_CML_MENU        = 1,
    WAP_CML_LSELECT     = 2,
    WAP_CML_RECEIVING1  = 3,
    WAP_CML_RECEIVING2  = 4,
    WAP_CML_SECURITY1   = 5,
    WAP_CML_SECURITY2   = 6,
    WAP_CML_CONNECTING1 = 7,
    WAP_CML_CONNECTING2 = 8,
    WAP_CML_ADDBOOKMARK1= 9,
    WAP_CML_ADDBOOKMARK2= 10,
    WAP_CML_RESETUP1    = 11,
    WAP_CML_RESETUP2    = 12
};  

enum{                               // WAP UPDOWN CML
    WAP_CML_UPDOWNEMPTY = 0,
    WAP_CML_UPDOWN      = 1,
    WAP_CML_UP          = 2,
    WAP_CML_DOWN        = 3
};

enum{                               // WAP OK CML
    WAP_CML_OKEMPTY     = 0,
    WAP_CML_OK          = 1
};

enum{                               // WAP CENTER CML
    WAP_CML_CETEREMPTY  = 0,
    WAP_CML_KEY         = 1,
    WAP_CML_LINK        = 2,
    WAP_CML_INPUT       = 3,
    WAP_CML_CSELECT     = 4,
    WAP_CML_YES         = 5,
    WAP_CML_CONFIRM     = 6
};

enum{                               // WAP RIGHT CML
    WAP_CML_RIGHTEMPTY  = 0,
    WAP_CML_HIGH        = 1,
    WAP_CML_CANCEL      = 2,
    WAP_CML_NO          = 3
};
  
/*==========================================================================+
 |																			|
 |			Graphic Primitives :											|
 |																			|
 |					1. Point												|
 |					2. Line													|
 |					3. Rectangle											|
 |																			|
 +==========================================================================*/
#include "sky.h"
#include "eqs.h"
/////////////////////////////////////////////////////////////////////////////
//
//		WAP 포인트
//
/////////////////////////////////////////////////////////////////////////////
//void WAP_Display_Point(int x, int y, boolean color);
#ifdef WAP_B2
void WAP_Display_Point(int x, int y, COLOR color);
#else   //#ifdef WAP_B2
void WAP_Display_Point(int x, int y, BYTE color);
#endif  //#ifdef WAP_B2

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 라인
//
/////////////////////////////////////////////////////////////////////////////
//void WAP_Display_Line(int x1, int y1, int x2, int y2, boolean color);
#ifdef WAP_B2
void WAP_Display_Line(int x1, int y1, int x2, int y2, COLOR color);
#else   //#ifdef WAP_B2
void WAP_Display_Line(int x1, int y1, int x2, int y2, BYTE color);
#endif  //#ifdef WAP_B2


/////////////////////////////////////////////////////////////////////////////
//
//		WAP 사각형
//
/////////////////////////////////////////////////////////////////////////////
#ifdef WAP_B2
void WAP_Display_Rectangle(int x1, int y1, int x2, int y2, BOOL fill, COLOR color);
#else   //#ifdef WAP_B2
void WAP_Display_Rectangle(int x1, int y1, int x2, int y2, BOOL fill, BYTE color);
#endif  //#ifdef WAP_B2

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 원
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Circle(int x, int y, int radius, BOOL fill, COLOR color);



/*==========================================================================+
 |																			|
 |			Text, Font :													|
 |																			|
 |																			|
 +==========================================================================*/
#define PACKED	__packed
typedef __packed union 
{ 
  __packed struct  { BYTE low, high; } Byte;
  WORD HWord;

} WAP_HWORD_BYTE;

typedef __packed union
{ 
  __packed struct  { WAP_HWORD_BYTE low, high; } HWord;
  DWORD DWord;
  
} WAP_DWORD_HWORD;

/*
BYTE *WAP_GetASCIIFontBuffer ( WORD nCode );
BYTE *WAP_GetHanFontBuffer ( WORD nCode );
BYTE *WAP_GetHangulFontBuffer ( WORD nCode );
BYTE *WAP_GetSKSymbolFontBuffer ( WORD nCode );
BYTE *WAP_GetKS5601SpecialFontBuffer ( WORD nCode );
BYTE *WAP_GetSpaceFontBuffer ( WORD nCode );
*/
///////////////////////////////////////////////////////////////////////////////
//
//  WAP Text for NormalFont
//
///////////////////////////////////////////////////////////////////////////////
void WAP_DrawWordBitToLCDBuffer (int nIndexX, int nIndexY, BYTE DrawPattern, BYTE MaskBitPattern, int nDrawMode);
WORD WAP_OutWordCharLCDBuffer( BYTE *pFont, int nPosX, int nPosY, int nFontWidth, int nFontHeight, int nDrawMode );
int WAP_NormalTextOutXY( BYTE *strText, int nPosX, int nPosY, int nDrawMode, int nTableColumns, int nCurColumn );
int WAP_MediumTextOutXY( BYTE *strText, int nPosX, int nPosY, int nDrawMode, int nTableColumns, int nCurColumn );

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 텍스트
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_TextOut(int x, int y, char* str,
						 int font, int mode, BOOL underline, int align,
						 int nTableColumns, int nCurColumn); //, int NormalFont);
void WAP_Display_TextOut_Focus(int x, int y, char* str,
			             int font, int mode, BOOL underline, int align,
			             int nTableColumns, int nCurColumn);
void WAP_Display_TextOut_Title(int x, int y, char* str,
			             int font, int mode, BOOL underline, int align,
			             int nTableColumns, int nCurColumn);

// MedFont, Normal Font 구별을 위해 NormalFont 변수를 사용한다. 

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 이미지
//
/////////////////////////////////////////////////////////////////////////////

void WAP_Display_Image_CML(int kind_of_cml, int image ,int xPos, int yPos);

// zeus add. A7에서 이걸로 바꿔서 사용해야 할 듯  일단은 test용 
//void WAP_Display_Image_BMP(char* imageData, int width, int height,
//						   int pos_x, int pos_y, int align, int HowManyColor);

void WAP_Display_Image_BMP(char* pDestBuf, char* pImgData,
								  int width, int height, int xPos, int yPos,
								  int mode, int align, int nBitCount);

void WAP_Display_Image_WBMP(char* pDestBuf, char* pImgData,
								  int width, int height, int xPos, int yPos,
								  int mode, int align);

void Play_SIS_Animation(void);

#ifdef WAP_NOWRAP
void Support_NoWrapping(void); //yjahn96 00.10.27
#endif//#ifdef WAP_NOWRAP

void WAP_Display_Image_SIS(char* pDestBuf, char* pImgData,
								  int width, int height, int xPos, int yPos,
								  int mode, int align, int nBitCount);

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 메세지
//
/////////////////////////////////////////////////////////////////////////////
void WAP_ClearScreen(void);
void WAP_SetTextAtLine(int nLine, char* strText, int nAlign);
void WAP_DisplayMessage(void);

/////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////
typedef struct
{
	UINT8 viewid;
	UINT8 id;
	UINT8 wmlkind;
	//1: KEY 2: OPTION 3: LINK
}storeWML;

#endif// WAP_GENERAL_DISPLAY_H