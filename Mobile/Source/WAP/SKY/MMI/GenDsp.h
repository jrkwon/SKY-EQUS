#ifndef WAP_GENERAL_DISPLAY_H
#define WAP_GENERAL_DISPLAY_H

//////////////////////////////////////////////////////////////////////////////
//																			//
//						 General graphic library							//
//																			//
//////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------
// WAP B2
//-------------------------------------------------------------
#define WAP_LCD_PIXELS						128*160

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

//----------------------------------------------------------------
#define WAP_TITLE_BACK_COLOR    BLUE//83
#define WAP_SELECT_BACK_COLOR   BLACK//1
#define WAP_SELECT_TEXT_COLOR   WHITE//255   
#define WAP_BASIC_BACK_COLOR    WHITE//255
#define WAP_BASIC_TEXT_COLOR    BLACK//0

// ---------------------------------------------------------------
#define WAP_HS_SCRN_HIGH  9//8       /* Screen height in characters */
#define WAP_HS_SCRN_WIDE  18//21      /* Screen width in characters  */

#define WAP_LCD_WIDTH					SKY_SCREEN_WIDTH//128		
#define WAP_LCD_HEIGHT					SKY_SCREEN_HEIGHT - SKY_ANNUNCIATORS_HEIGHT//146		//160-14	

#define WAP_LCD_TOTAL_ADDRESS_X			SKY_SCREEN_WIDTH//128	
#define WAP_LCD_TOTAL_ADDRESS_Y			SKY_SCREEN_HEIGHT//160	

#define WAP_MAX_TEXT						255

/*--------------------------------------------------------------------------------*/ 	
// The number of characters in case of standard large font
#define WAP_SCREEN_WIDTH							18
// The number of characters in case of medium font
#define WAP_MED_SCREEN_WIDTH						21		

// ---------------------------------------------------------------------------------
// Medium font 12*13														//--> 바꿔야함
#define	WAP_MED_HAN_FONT_WIDTH						12 
#define	WAP_MED_HAN_FONT_HEIGHT						13 

#define WAP_MED_ASCII_FONT_WIDTH					6						//--> 바꿔야함  
#define WAP_MED_ASCII_FONT_HEIGHT					13 

// Standard font 12*13
#define	WAP_HAN_FONT_WIDTH							14//12 
#define	WAP_HAN_FONT_HEIGHT							15//13 

#define	WAP_ASCII_FONT_WIDTH						7//6
#define	WAP_ASCII_FONT_HEIGHT						15//13 

//Big font 14*16
#define	WAP_BIG_HAN_FONT_WIDTH						14 
#define	WAP_BIG_HAN_FONT_HEIGHT						16 

#define WAP_BIG_ASCII_FONT_WIDTH					7						  
#define WAP_BIG_ASCII_FONT_HEIGHT					16 


#define	WAP_BIG_BIG_ASCII_FONT_WIDTH					10					
#define	WAP_BIG_BIG_ASCII_FONT_HEIGHT					21

#define	WAP_SMALL_ASCII_FONT_WIDTH					6						
#define	WAP_SMALL_ASCII_FONT_HEIGHT					8

#define	WAP_CALENDAR_FONT_WIDTH						5						//--> 바꿔야함
#define	WAP_CALENDAR_FONT_HEIGHT					6

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
void WAP_Display_Point(int x, int y, COLOR color);

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 라인
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Line(int x1, int y1, int x2, int y2, COLOR color);

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 사각형
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Rectangle(int x1, int y1, int x2, int y2, BOOL fill, COLOR color);

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 원
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Circle(int x, int y, int radius, BOOL fill, COLOR color);

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


/////////////////////////////////////////////////////////////////////////////
//
//		WAP 이미지
//
/////////////////////////////////////////////////////////////////////////////

void WAP_Display_Image_CML(int kind_of_cml, int image ,int xPos, int yPos);
void WAP_Display_Image_BMP(char* pDestBuf, char* pImgData,
								  int width, int height, int xPos, int yPos,
								  int mode, int align, int nBitCount);
void WAP_Display_Image_WBMP(char* pDestBuf, char* pImgData,
								  int width, int height, int xPos, int yPos,
								  int mode, int align);
void WAP_Display_Image_SIS(char* pDestBuf, char* pImgData,
								  int width, int height, int xPos, int yPos,
								  int mode, int align, int nBitCount);

void Play_SIS_Animation(void);

#ifdef WAP_NOWRAP
void Support_NoWrapping(void); //yjahn96 00.10.27
#endif//#ifdef WAP_NOWRAP

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