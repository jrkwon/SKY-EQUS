//////////////////////////////////////////////////////////////////////////////
//																			//
//							 WML Broswer									//
//																			//
//////////////////////////////////////////////////////////////////////////////

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

#include "aapimem.h"
#include "capiclnt.h"
#include "capimem.h"
#include "capimmi.h"
#include "cmmnrsrc.h"

#include "GenDsp.h"

#include "mmi_data.h"
#include "aapiclnt.h"//for log
#include "SISdecoder.h"

//////////////////////////////////////////////////////////////////////////////
#include "clnt.h"

#if MMI_TYPE == SKY_UI
    #include "comdef.h"    
#else
    #include "..\..\..\MMI_A10\mmedia\MMedia.h"
    #include "..\..\..\MMI_A10\nvA10.h"
    #include "..\Lcd\LCD_A10.h"
    #include "..\Lcd\lcddef_a10.h"
#endif

#include "sky.h"    //yjahn96 01.02.02
#include "eqc.h"    //yjahn96 01.03.02

// ----------------------------------------------------------------------------------------
//yjahn96 00.12.28 -----------------------
//BYTE abMainScreenBuffer[(SKY_SCREEN_BUFFER_WIDTH/8)*SKY_SCREEN_BUFFER_HEIGHT*SKY_COLOR_SPACE];
extern byte* /*EQS_API*/ GetScreenBuffer(void);	//BYTE* EQS_API GetScreenBuffer(VOID)

static int	WAP_ASCIIFontWidth = WAP_ASCII_FONT_WIDTH;
static int	WAP_ASCIIFontHeight = WAP_ASCII_FONT_HEIGHT;
#if !((defined WAP_B1) || (defined WAP_B2))
static int	WAP_ASCIIFontBufferSizePerChar = WAP_ASCII_FONT_BUFFER_SIZE_PER_CHAR;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

static int	WAP_HanFontWidth = WAP_HAN_FONT_WIDTH;
static int	WAP_HanFontHeight = WAP_HAN_FONT_HEIGHT;
#if !((defined WAP_B1) || (defined WAP_B2))
static int	WAP_HanFontBufferSizePerChar = WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

//------------------------------------------------------------------------------------------------
int WAP_TextOutXYnChar(BYTE *strText, int nPosX, int nPosY, int nChar, int nDrawMode);
void WAP_TextOutXYnCharCursor(BYTE *strText, int nPosX, int nPosY, int nChar, BYTE cursX, int nDrawMode);
WORD WAP_OutWordCharLCDBuffer(BYTE *pFont, int nPosX, int nPosY, int nFontWidth, int nFontHeight, int nDrawMode);
WORD WAP_OutWordCharLCDBufferHan(BYTE *pFont, int nPosX, int nPosY, int nFontWidth, int nFontHeight, int nDrawMode);

#define WAP_HS_LCD_LOW_DATA(addr)	(addr & 0x00ff)			// Low Byte of Lcd Word Command 
#define WAP_HS_LCD_HIGH_DATA(addr)	((addr >> 8) & 0x00ff)	// High Byte of Lcd Word Command 

extern void *wip_malloc (UINT32 size);
extern void wip_free (void* mem);
extern BOOL WAP_IsValidFile(SKY_DATA_FILE_E SkyDataFile, CONST BYTE* pszFileName);

ROM INT g_WapLoadingBarColor[6][6] = {
    { 10, 15, 27, 95, 159, 223},      //�Ķ���
    { 8, 12, 56, 156, 222, 255},      //�ʷϻ�
    { 9, 13, 56, 157, 222, 255},      //û�ϻ�
    { 128, 193, 234, 239, 251, 255},  //�����  //{ 66, 130, 163, 203, 211, 219},
    { 96, 160, 233, 246, 254, 255},//{ 128, 192, 224, 246, 254, 255}   //ī�λ�
    { 104, 172, 212, 253, 253, 255}   //�����
};

//////////////////////////////////////////////////////////////////////////////
//																			//
//						 General graphic library							//
//																			//
//////////////////////////////////////////////////////////////////////////////

/*==========================================================================+
 |																			|
 |			Byte/Bit Operations :											|
 |																			|
 |					1. gen_setbit / gen_getbit								|
 |					2. ByteRotate											|
 |																			|
 +==========================================================================*/

#define set_picture_mate_bit(byte,bitno,bit)  \
		  ( (bit)? ((byte)|=(1<<(bitno))) : ((byte)&=(~(1<<(bitno)))) )

#define color_setbit(byte,bitno,color)  \
			(byte |= ((color << 7) >> bitno))

#define gen_setbit(byte,bitno,color)  \
			(byte |= ((color << 6) >> (bitno * 2)))

#define gen_getbit(byte,bitno)   \
		  ( ((byte)&(1<<(bitno)))? 1 : 0 )


int ChangeBitPosition(int bitno)
{
	if (bitno == 0) bitno = 7;
	else if (bitno == 1) bitno = 6;
	else if (bitno == 2) bitno = 5;
	else if (bitno == 3) bitno = 4;
	else if (bitno == 4) bitno = 3;
	else if (bitno == 5) bitno = 2;
	else if (bitno == 6) bitno = 1;
	else if (bitno == 7) bitno = 0;

	return bitno;
}

BYTE ByteRotate(BYTE a)
{
	BYTE b;
	int i;
	
	b=0;
	for(i=0; i<8; i++)
	{		
		b |= ((a&(1<<(i%8)))? 1:0)<<(7-(i%8)) ;
	}
	return b;
}

/*==========================================================================+
 |																			|
 |			Graphic Primitives :											|
 |																			|
 |					1. Point												|
 |					2. Line													|
 |					3. Rectangle											|
 |																			|
 +==========================================================================*/

/////////////////////////////////////////////////////////////////////////////
//
//		WAP ����Ʈ
//
/////////////////////////////////////////////////////////////////////////////
#ifdef WAP_B2
void WAP_Display_Point(int x, int y, COLOR color)
#else   //#ifdef WAP_B2
void WAP_Display_Point(int x, int y, BYTE color)
#endif  //#ifdef WAP_B2
{
#ifndef WAP_B2    
    byte* LCDWordAniBuffer = (byte* )GetScreenBuffer();		//yjahn96 00.12.28
#endif  //#ifdef WAP_B2

	//------------------------------------------------
	//Boundary Ȯ��
#if !((defined WAP_B1) || (defined WAP_B2))
    int nX = (int)(x/8*2);
	if (nX >= 0 && nX < 15) 
	{
		int nbit = x%8;
		if (nbit > 3) 
		{
			nX++;
			nbit = nbit - 4;
		}

		//------------------------------------------------
		//y���� WAP_HEAD_HEIGHT��ŭ �Ű��ش�.
		y = y + WAP_HEAD_HEIGHT;	//yjahn96 00.12.28

		if((0 <= y && y < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
			gen_setbit(*(LCDWordAniBuffer+y*WAP_LCD_TOTAL_ADDRESS_X+nX), nbit, color);	//yjahn96 00.12.28
	}
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	if(y >= 0)
	{
		//------------------------------------------------
		//y���� WAP_HEAD_HEIGHT��ŭ �Ű��ش�.
		y = y + WAP_HEAD_HEIGHT;	//yjahn96 00.12.28

		if((0 <= y && y < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= x && x < WAP_LCD_WIDTH))
		    EQS_SetPixel(x, y, color);
	}
#endif //WAP_B1

}

/////////////////////////////////////////////////////////////////////////////
//
//		WAP ����
//
/////////////////////////////////////////////////////////////////////////////
#ifdef WAP_B2
void WAP_Display_Line(int x1, int y1, int x2, int y2, COLOR color)
#else   //#ifdef WAP_B2
void WAP_Display_Line(int x1, int y1, int x2, int y2, BYTE color)
#endif  //#ifdef WAP_B2
{
#ifndef WAP_B2
    byte* LCDWordAniBuffer = (byte* )GetScreenBuffer();	//yjahn96 00.12.28
#endif  //#ifdef WAP_B2

	int x=0, y=0;

#if !((defined WAP_B1) || (defined WAP_B2))	//yjahn96 01.03.21
	//------------------------------------------------
	//y���� WAP_HEAD_HEIGHT��ŭ �Ű��ش�.
	y1 = y1 + WAP_HEAD_HEIGHT;	//yjahn96 00.12.28
	y2 = y2 + WAP_HEAD_HEIGHT;	//yjahn96 00.12.28

	if (x1 > x2) {		// x1�� ������, x2�� ����, �������� �������� ũ�� ��ġ�� ���� �ٲ��ְ� �׸���. 
		x = x1; x1 = x2; x2 = x;
		y = y1; y1 = y2; y2 = y;
	}

//	if (y1 > y2) {		// y1�� ������, y2�� ����, �������� �������� ũ�� ��ġ�� ���� �ٲ��ְ� �׸���. 
//		y = y1; y1 = y2; y2 = y;
//		x = x1; x1 = x2; x2 = x;
//	}

	// -----------------------------------------
	// �Ѱ谪 üũ 	
	if (x1 >= WAP_LCD_WIDTH) return;		// x1�� �Ѱ谪���� ũ�� ���� 
	if (x2 >= WAP_LCD_WIDTH) x2 = (WAP_LCD_WIDTH-1);

	if (y1 >= WAP_LCD_HEIGHT) return;	// y1�� �Ѱ谪���� ũ�� ���� 
	if (y2 >= WAP_LCD_HEIGHT) y2 = (WAP_LCD_HEIGHT-1);


	// -----------------------------------------
	// ���� ���� 
	// ���μ����� ���μ����� �ľ��Ѵ�. 
	if (y1 == y2)						// ���μ� 
	{
		for (x = x1 ; x <= x2 ; x++)
		{
            int nX = (int)(x/8*2);
			int nbit = x%8;

			if (nbit > 3) 
			{
				nX++;
				nbit = nbit - 4;
			}
			if (0 <= y1 && y1 < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X)
				gen_setbit(*(LCDWordAniBuffer+y1*WAP_LCD_TOTAL_ADDRESS_X+nX), nbit, color);	//yjahn96 00.12.28
		}

	}
	else								// ���μ� 
	{
		for (y = y1 ; y <= y2 ; y++)
		{
            int nX = (int)(x1/8*2);
			int nbit = x1%8;
			if (nbit > 3)
			{
				nX++;
				nbit = nbit - 4;
			}

			if (0 <= y && y < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X)
				gen_setbit(*(LCDWordAniBuffer+y*WAP_LCD_TOTAL_ADDRESS_X+nX), nbit, color);	//yjahn96 00.12.28

		}
	}	// end of if (y1 == y2)

#else	//#if !((defined WAP_B1) || (defined WAP_B2))
    COLOR OldTextColor=WHITE;
    
    OldTextColor = EQS_GetTextColor();

	if (x1 > x2) {		// x1�� ������, x2�� ����, �������� �������� ũ�� ��ġ�� ���� �ٲ��ְ� �׸���. 
		x = x1; x1 = x2; x2 = x;
		y = y1; y1 = y2; y2 = y;
	}

	// -----------------------------------------
	// �Ѱ谪 üũ 	
	if (x1 >= WAP_LCD_WIDTH) return;		// x1�� �Ѱ谪���� ũ�� ���� 
	if (x2 >= WAP_LCD_WIDTH) x2 = (WAP_LCD_WIDTH-1);

	if (y1 >= WAP_LCD_HEIGHT) return;	// y1�� �Ѱ谪���� ũ�� ���� 
	if (y2 >= WAP_LCD_HEIGHT) y2 = (WAP_LCD_HEIGHT-1);
    if (y1 < 0) y1 = 0;
    if (y2 < 0) y2 = 0;

	//------------------------------------------------
	//y���� WAP_HEAD_HEIGHT��ŭ �Ű��ش�.
	y1 = y1 + WAP_HEAD_HEIGHT;	y2 = y2 + WAP_HEAD_HEIGHT;	

    EQS_SetTextColor(color);
    EQS_Line(x1, y1, x2, y2);
    EQS_SetTextColor(OldTextColor);

#endif	//#if !((defined WAP_B1) || (defined WAP_B2))
}

/////////////////////////////////////////////////////////////////////////////
//
//		WAP �簢��
//
/////////////////////////////////////////////////////////////////////////////
#ifdef WAP_B2
void WAP_Display_Rectangle(int x1, int y1, int x2, int y2, BOOL fill, COLOR color)
#else   //#ifdef WAP_B2
void WAP_Display_Rectangle(int x1, int y1, int x2, int y2, BOOL fill, BYTE color)
#endif  //#ifdef WAP_B2
{
	int x, y;
    COLOR OldTextColor=WHITE;
    
	if (x1 > x2) {		// x1�� ������, x2�� ����, �������� �������� ũ�� ��ġ�� ���� �ٲ��ְ� �׸���. 
		x = x1; x1 = x2; x2 = x;
	}

	if (y1 > y2) {
		y = y1; y1 = y2; y2 = y;
	}

	// �Ѱ谪 üũ 	
	if (x1 >= WAP_LCD_WIDTH) return;		// x1�� �Ѱ谪���� ũ�� ���� 
	if (x2 >= WAP_LCD_WIDTH) x2 = (WAP_LCD_WIDTH-1);
	if (y1 >= WAP_LCD_HEIGHT) return;	// y1�� �Ѱ谪���� ũ�� ���� 
	if (y2 >= WAP_LCD_HEIGHT) y2 = (WAP_LCD_HEIGHT-1);
    if (x1 < 0) x1 = 0;    if (x2 < 0) x2 = 0;
    if (y1 < 0) y1 = 0;    if (y2 < 0) y2 = 0;

	//------------------------------------------------
	//y���� WAP_HEAD_HEIGHT��ŭ �Ű��ش�.
	y1 = y1 + WAP_HEAD_HEIGHT;	y2 = y2 + WAP_HEAD_HEIGHT;	

    if(fill)
    {
        EQS_FillRectangle(x1, y1, x2, y2, color);    
    }
    else
    {
        OldTextColor = EQS_GetTextColor();
        EQS_SetTextColor(color);
        EQS_Rectangle(x1, y1, x2, y2);
        EQS_SetTextColor(OldTextColor);
    }

/*	byte* LCDWordAniBuffer = (byte* )GetScreenBuffer();	// yjahn96 00.12.28

	int x, y;

	if (x1 > x2) {		// x1�� ������, x2�� ����, �������� �������� ũ�� ��ġ�� ���� �ٲ��ְ� �׸���. 
		x = x1; x1 = x2; x2 = x;
	}

	if (y1 > y2) {
		y = y1; y1 = y2; y2 = y;
	}

	// �Ѱ谪 üũ 	
	if (x1 >= WAP_LCD_WIDTH) return;		// x1�� �Ѱ谪���� ũ�� ���� 
	if (x2 >= WAP_LCD_WIDTH) x2 = (WAP_LCD_WIDTH-1);
	if (y1 >= WAP_LCD_HEIGHT) return;	// y1�� �Ѱ谪���� ũ�� ���� 
	if (y2 >= WAP_LCD_HEIGHT) y2 = (WAP_LCD_HEIGHT-1);

	WAP_Display_Line(x1, y1, x2, y1, color);
	WAP_Display_Line(x1, y2, x2, y2, color);
	WAP_Display_Line(x1, y1, x1, y2, color);
	WAP_Display_Line(x2, y1, x2, y2, color);


	// �簢�� ä��� ---------------------------- ������ ä��� ������ ä���. 

	if (!fill) return;

#if !((defined WAP_B1) || (defined WAP_B2))	//yjahn96 01.03.21

	for (y=y1+1 ; y<=y2-1 ; y++) {
		
		for (x=x1+1 ; x<=x2-1 ; x++) 
        {
			int nX = (int)(x/8*2);
			int nbit = x%8;
			if (nbit > 3)
			{
				nX++;
				nbit = nbit - 4;
			}
			if (0 <= y && y < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X)
				gen_setbit(*(LCDWordAniBuffer+y*WAP_LCD_TOTAL_ADDRESS_X+nX), nbit, color);	//yjahn96 00.12.28
		} 

	}	// end of fill - �簢�� ä��� 

#else	//#if !((defined WAP_B1) || (defined WAP_B2))

	//------------------------------------------------
	//y���� WAP_HEAD_HEIGHT��ŭ �Ű��ش�.
	if(y1 < 0 && y2 >= 0)
	{
		y1 = WAP_HEAD_HEIGHT;	y2 = y2 + WAP_HEAD_HEIGHT;	
		for (y=y1+1 ; y<=y2-1 ; y++) {
			
			for (x=x1+1 ; x<=x2-1 ; x++) 
			{
				if (0 <= y && y < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= x && x < WAP_LCD_TOTAL_ADDRESS_X)
					*(LCDWordAniBuffer+y*WAP_LCD_TOTAL_ADDRESS_X+x) = color;	
			} 

		}	// end of fill - �簢�� ä��� 
	}
	else if(y1 >= 0 && y2 <0)
	{
		y1 = y1 + WAP_HEAD_HEIGHT;	y2 = WAP_HEAD_HEIGHT;	
		for (y=y1+1 ; y<=y2-1 ; y++) {
			
			for (x=x1+1 ; x<=x2-1 ; x++) 
			{
				if (0 <= y && y < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= x && x < WAP_LCD_TOTAL_ADDRESS_X)
					*(LCDWordAniBuffer+y*WAP_LCD_TOTAL_ADDRESS_X+x) = color;	
			} 

		}	// end of fill - �簢�� ä��� 
	}
	else if(y1 >= 0 && y2 >= 0)
	{
		y1 = y1 + WAP_HEAD_HEIGHT;	y2 = y2 + WAP_HEAD_HEIGHT;	
		for (y=y1+1 ; y<=y2-1 ; y++) {
			
			for (x=x1+1 ; x<=x2-1 ; x++) 
			{
				if (0 <= y && y < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= x && x < WAP_LCD_TOTAL_ADDRESS_X)
					*(LCDWordAniBuffer+y*WAP_LCD_TOTAL_ADDRESS_X+x) = color;	
			} 

		}	// end of fill - �簢�� ä��� 
	}
#endif	//#if !((defined WAP_B1) || (defined WAP_B2))
*/
}


/////////////////////////////////////////////////////////////////////////////
//
//		WAP ��
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Circle(int x, int y, int radius, BOOL fill, COLOR color)
{
	int i;
	int a = radius/2;
	for(i=(-1*a); i<=a; i++)
	{
		WAP_Display_Point(x+i, y-radius, color);
		WAP_Display_Point(x+i, y+radius, color);

		WAP_Display_Point(x-radius, y+i, color);
		WAP_Display_Point(x+radius, y+i, color);
	}
	
	WAP_Display_Line(x-a, y-radius, x-radius, y-a, color);
	WAP_Display_Line(x+a, y+radius, x+radius, y+a, color);
	WAP_Display_Line(x-radius, y+a, x-a, y+radius, color);
	WAP_Display_Line(x+radius, y-a, x+a, y-radius, color);
}

/*==========================================================================+
 |																			|
 |			Text, Font :													|
 |																			|
 |					1. WAP_GetHanFontBuffer() / WAP_GetASCIIFontBuffer()	|
 |					2. WAP_DrawBitToLCDBuffer()								|
 |					3. WAP_OutCharLCDBuffer()								|
 |					4. WAP_NormalTextOutXY() / WAP_SmallTextOutXY()			|
 |																			|
 +==========================================================================*/

/*INLINE*/ BYTE *Widening(BYTE *pFont, BYTE nBytePerChar)
{
	static BYTE WordOneCharBuffer[WAP_BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR];
	BYTE i, j, indx;
	WORD dataBuffer;

	for (indx = 0; indx < nBytePerChar; indx++)
	{
		if (*pFont)
		{
			for (i=0, j=0, dataBuffer = 0; i < 8; i++, j+= 2)
			{ 
				if (*pFont & (WAP_HS_LCD_CHECK_BIT >> i)) 	// #define HS_LCD_CHECK_BIT 0x80
					dataBuffer |= (WAP_HS_LCD_MASK_BIT >> j);	// #define HS_LCD_MASK_BIT	0xc000
				else 
					dataBuffer &= ~(WAP_HS_LCD_MASK_BIT >> j);
			}
		}	
		else	 dataBuffer = 0;		

		// A7 Proejct Only: Because Hangul and ASCII font uses the .def files from B5 project and those files are not 
		// arranged to fit in little-endian by FontTool.exe of earlier version, the conversion process from 
		// big-endian to little endian is done here. 
		// If those .def files are to be arranged someday, switch the assigning order like below example :
		// 
		// WordOneCharBuffer[ indx*2 ]     = (byte)HS_LCD_LOW_DATA(dataBuffer);
		// WordOneCharBuffer[ indx*2 + 1 ] = (byte)HS_LCD_HIGH_DATA(dataBuffer);

		WordOneCharBuffer[ indx*2 ] = (BYTE)WAP_HS_LCD_HIGH_DATA(dataBuffer);
		WordOneCharBuffer[ indx*2 + 1 ] = (BYTE)WAP_HS_LCD_LOW_DATA(dataBuffer);

		pFont++;
	}

	return WordOneCharBuffer;
}


INLINE BYTE *Widening4Gray(BYTE *pFont, BYTE nBytePerChar, BYTE by4Gray)
{
	static BYTE WordOneCharBuffer[WAP_BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR*2];
	BYTE i, j, indx;
	WORD dataBuffer;

	for (indx = 0; indx < nBytePerChar; indx++)
	{
		dataBuffer = 0;

		if (*pFont)
		{
			for (i=0, j=0, dataBuffer = 0; i < 8; i++, j+= 2)
			{ 
				if (*pFont & (WAP_HS_LCD_CHECK_BIT >> i)) 	// #define HS_LCD_CHECK_BIT 0x80
				{
					switch (by4Gray)
					{
#if !((defined WAP_B1) || (defined WAP_B2))
					case WAP_BLACKGRAY:
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
					case WAP_LIGHTGRAY:
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
						dataBuffer |= (0x8000 >> j);	// #define HS_LCD_MASK_BIT	0xc000
						break;
					default:
						dataBuffer |= (WAP_HS_LCD_MASK_BIT >> j);
						break;
					}
				}
				else 
					dataBuffer &= ~(WAP_HS_LCD_MASK_BIT >> j);
			}
		}	
		else	 dataBuffer = 0;		

		// A7 Proejct Only: Because Hangul and ASCII font uses the .def files from B5 project and those files are not 
		// arranged to fit in little-endian by FontTool.exe of earlier version, the conversion process from 
		// big-endian to little endian is done here. 
		// If those .def files are to be arranged someday, switch the assigning order like below example :
		// 
		// WordOneCharBuffer[ indx*2 ]     = (byte)HS_LCD_LOW_DATA(dataBuffer);
		// WordOneCharBuffer[ indx*2 + 1 ] = (byte)HS_LCD_HIGH_DATA(dataBuffer);

		WordOneCharBuffer[ indx*2 ] = (BYTE)WAP_HS_LCD_HIGH_DATA(dataBuffer);
		WordOneCharBuffer[ indx*2 + 1 ] = (BYTE)WAP_HS_LCD_LOW_DATA(dataBuffer);

		pFont++;
	}

	return WordOneCharBuffer;
}


/////////////////////////////////////////////////////////////////////////////
//
//		WAP �ؽ�Ʈ for Med Font
//
/////////////////////////////////////////////////////////////////////////////
int WAP_MediumTextOutXY(BYTE *strText, int nPosX, int nPosY, int nDrawMode,	   
						int nTableColumns, int nCurColumn)
{
	BYTE *pFont;
	WORD wChar;
	WORD nTextWidth = 0, nCharWidth;
	int i;
	BYTE *pWordFont;
	
	int nCurChar = 0;

	extern /*ROM*/ BYTE* /*EQS_API*/ GetBitmapFontBufferByCode(WORD wCode);	//yjahn96 00.12.28

	BYTE *pText = (BYTE *)strText;//original data

	//for Table data
	int nTableX;
	if(nTableColumns > 1) nPosX = 0;

	//================================================================
	for(i=0; i<WAP_MED_SCREEN_WIDTH; i++)//20ȸ
	{
		if(*pText == 0) break;
		wChar = *pText;

		//------------------------------------------------------------------
		// Table data ���� x ��ǥ ó��
		if(nTableColumns > 1)//Table data
			nTableX = (WAP_LCD_WIDTH*2)/nTableColumns * (nCurColumn-1);
		else
			nTableX = 0;

		//------------------------------------------------------------------
		if ( wChar & 0x80 )            
		{	  
			/*=================================================*/
			//line break ó�� (���� ���� ���)
			if(nPosX + WAP_MED_HAN_FONT_WIDTH*2 > (WAP_LCD_WIDTH/nTableColumns)*2)
				return nCurChar;
			/*=================================================*/

			pText++; i++;
			if (*pText == 0) break;
			wChar <<= 8; wChar += *pText;

			// skip
			if(nPosY < 0 || nPosY > WAP_DOC_HEIGHT)
			{
				nCharWidth = WAP_MED_HAN_FONT_WIDTH * 2;
			}
			else
			{
				pFont = GetBitmapFontBufferByCode( wChar );
				pWordFont = Widening(pFont, WAP_MED_HAN_FONT_BUFFER_SIZE_PER_CHAR);
				nCharWidth = WAP_OutWordCharLCDBuffer(pWordFont,nPosX+nTableX,nPosY,WAP_MED_HAN_FONT_WIDTH,WAP_MED_HAN_FONT_HEIGHT,nDrawMode);
			}

			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			pText++;

			nCurChar += 2;//�ѱ�
		}
		else		
		{
			/*=================================================*/
			//line break ó�� (���� ���� ���)
			if(nPosX + WAP_MED_ASCII_FONT_WIDTH*2 > (WAP_LCD_WIDTH/nTableColumns)*2)
				return nCurChar;
			/*=================================================*/

			// skip
			if(nPosY < 0 || nPosY > WAP_DOC_HEIGHT)
			{
				nCharWidth  = WAP_MED_ASCII_FONT_WIDTH * 2;
			}
			else
			{
				pFont = GetBitmapFontBufferByCode( wChar );
				pWordFont = Widening(pFont, WAP_MED_ASCII_FONT_BUFFER_SIZE_PER_CHAR);
				nCharWidth = WAP_OutWordCharLCDBuffer(pWordFont, nPosX+nTableX, nPosY, WAP_MED_ASCII_FONT_WIDTH, WAP_MED_ASCII_FONT_HEIGHT,nDrawMode);
			}
			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			pText++;

			nCurChar += 1;//ascii
		}
	}
	return nCurChar;
} 

/////////////////////////////////////////////////////////////////////////////
//
//		WAP �ؽ�Ʈ for Normal Font
//
/////////////////////////////////////////////////////////////////////////////

int WAP_NormalTextOutXY(BYTE *strText, int nPosX, int nPosY, int nDrawMode,
				  int nTableColumns, int nCurColumn)//for Table Data
{
	BYTE *pFont;
	WORD wChar;
	WORD nTextWidth = 0, nCharWidth;
	int i;
#if !((defined WAP_B1) || (defined WAP_B2))
	BYTE *pWordFont;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	int nCurChar = 0;

	extern /*ROM*/ BYTE* /*EQS_API*/ GetBitmapFontBufferByCode(WORD wCode);

	BYTE *pText = (BYTE *)strText;//original data

	//for Table data
	int nTableX;
	if(nTableColumns > 1) nPosX = 0; 
		
	//================================================================
    for(i=0; i<WAP_SCREEN_WIDTH; i++)//20ȸ
	{
		if(*pText == 0) break;
		wChar = *pText;

		//------------------------------------------------------------------
		// Table data ���� x ��ǥ ó��
		if(nTableColumns > 1)//Table data
#if !((defined WAP_B1) || (defined WAP_B2))
			nTableX = (WAP_LCD_WIDTH*2)/nTableColumns * (nCurColumn-1);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
			nTableX = (WAP_LCD_WIDTH)/nTableColumns * (nCurColumn-1);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
		else
			nTableX = 0;

		// �ѱ� ó��
		if ( wChar & 0x80 )
		{	
			/*=================================================*/
			//line break ó�� (���� ���� ���)
#if !((defined WAP_B1) || (defined WAP_B2))
			if(nPosX + WAP_HAN_FONT_WIDTH*2 > (WAP_LCD_WIDTH/nTableColumns)*2)
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
			if(nPosX + WAP_HAN_FONT_WIDTH > (WAP_LCD_WIDTH/nTableColumns))
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
				return nCurChar;
			/*=================================================*/

			pText++; i++;
			if (*pText == 0) break;
			wChar <<= 8; wChar += *pText;

			// skip
			if(nPosY < 0 || nPosY > WAP_DOC_HEIGHT)
			{
#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth  = WAP_HAN_FONT_WIDTH * 2;
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth  = WAP_HAN_FONT_WIDTH ;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
            }
			// Hangul
			else if(wChar >= 0xB0A1 && wChar <= 0xC9AA) 
			{
				pFont = GetBitmapFontBufferByCode(wChar);
#if !((defined WAP_B1) || (defined WAP_B2))
				pWordFont = Widening(pFont, WAP_HanFontBufferSizePerChar);
				nCharWidth = WAP_OutWordCharLCDBufferHan(pWordFont,nPosX+nTableX,nPosY,WAP_HanFontWidth,WAP_HanFontHeight,nDrawMode);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth = WAP_OutWordCharLCDBufferHan(pFont,nPosX+nTableX,nPosY,WAP_HanFontWidth,WAP_HanFontHeight,nDrawMode);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
            }
			// KS5601 special font
			else if(wChar >= 0xA1A1 && wChar <= 0xACFE) 
			{
				pFont = GetBitmapFontBufferByCode(wChar);
#if !((defined WAP_B1) || (defined WAP_B2))
				pWordFont = Widening(pFont, WAP_HanFontBufferSizePerChar);
				nCharWidth = WAP_OutWordCharLCDBufferHan(pWordFont,nPosX+nTableX,nPosY,WAP_HanFontWidth,WAP_HanFontHeight,nDrawMode);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth = WAP_OutWordCharLCDBufferHan(pFont,nPosX+nTableX,nPosY,WAP_HanFontWidth,WAP_HanFontHeight,nDrawMode);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
			}
			// SK specified font
			else if(wChar >= 0x81A0 && wChar <= 0x8aFF) 
			{
				pFont = GetBitmapFontBufferByCode(wChar);
#if !((defined WAP_B1) || (defined WAP_B2))
				pWordFont = Widening(pFont, WAP_HanFontBufferSizePerChar);
				nCharWidth = WAP_OutWordCharLCDBufferHan(pWordFont,nPosX+nTableX,nPosY,WAP_HanFontWidth,WAP_HanFontHeight,nDrawMode);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth = WAP_OutWordCharLCDBufferHan(pFont,nPosX+nTableX,nPosY,WAP_HanFontWidth,WAP_HanFontHeight,nDrawMode);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
            }
			// Error. set default (2-byte space)
			else
			{
				pFont = GetBitmapFontBufferByCode(wChar);
#if !((defined WAP_B1) || (defined WAP_B2))
                pWordFont = Widening(pFont, WAP_HanFontBufferSizePerChar);
				nCharWidth = WAP_OutWordCharLCDBufferHan(pWordFont,nPosX+nTableX,nPosY,WAP_HanFontWidth,WAP_HanFontHeight,nDrawMode);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth = WAP_OutWordCharLCDBufferHan(pFont,nPosX+nTableX,nPosY,WAP_HanFontWidth,WAP_HanFontHeight,nDrawMode);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
            }

			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			pText++;

			nCurChar = nCurChar + 2;//�ѱ�
		} 
		// ASCII
		else 
		{
			/*=================================================*/
			//line break ó�� (���� ���� ���)
#if !((defined WAP_B1) || (defined WAP_B2))
			if(nPosX + WAP_ASCII_FONT_WIDTH*2 > (WAP_LCD_WIDTH/nTableColumns)*2)
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
			if(nPosX + WAP_ASCII_FONT_WIDTH > (WAP_LCD_WIDTH/nTableColumns))
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
				return nCurChar;
			/*=================================================*/
			
			// skip
			if(nPosY < 0 || nPosY > WAP_DOC_HEIGHT)
			{
#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth  = WAP_ASCII_FONT_WIDTH * 2;
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth  = WAP_ASCII_FONT_WIDTH;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
			}
			// Other ASCII characters are simply widen.
			else
			{
				pFont = GetBitmapFontBufferByCode( wChar );
#if !((defined WAP_B1) || (defined WAP_B2))
                pWordFont = Widening(pFont, WAP_ASCIIFontBufferSizePerChar);
				nCharWidth = WAP_OutWordCharLCDBuffer(pWordFont, nPosX+nTableX, nPosY, WAP_ASCIIFontWidth, WAP_ASCIIFontHeight,nDrawMode);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				nCharWidth = WAP_OutWordCharLCDBuffer(pFont, nPosX+nTableX, nPosY, WAP_ASCIIFontWidth, WAP_ASCIIFontHeight,nDrawMode);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
            }

			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			pText++;

			nCurChar += 1;//ascii
		}
	}
	return nCurChar;
}

void WAP_DrawWordBitToLCDBuffer ( int nIndexX, int nIndexY,	
						  BYTE DrawPattern, BYTE MaskBitPattern,
						  int nDrawMode )
{
	byte* LCDWordAniBuffer = (byte* )GetScreenBuffer();	//yjahn96 00.12.28

	nIndexY = nIndexY + WAP_HEAD_HEIGHT;	//head��ŭ �Ű���	yjahn96 00.12.28

	switch	( nDrawMode )
	{
    case WAP_DRAW_REVERSE :
		  //yjahn96 00.12.28 -------------------
		  //LCDWordAniBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		  //LCDWordAniBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
#if !((defined WAP_B1) || (defined WAP_B2))
		  *(LCDWordAniBuffer + nIndexY*WAP_LCD_TOTAL_ADDRESS_X + nIndexX) |= MaskBitPattern;
		  *(LCDWordAniBuffer + nIndexY*WAP_LCD_TOTAL_ADDRESS_X + nIndexX) ^= DrawPattern;
#else  //#if !((defined WAP_B1) || (defined WAP_B2))
		  *(LCDWordAniBuffer + nIndexY*WAP_LCD_TOTAL_ADDRESS_X + nIndexX) = DrawPattern;
		  *(LCDWordAniBuffer + nIndexY*WAP_LCD_TOTAL_ADDRESS_X + nIndexX) ^= MaskBitPattern;
#endif //#if !((defined WAP_B1) || (defined WAP_B2))
		break;

    case WAP_DRAW_NORMAL :
	  default :
		  //yjahn96 00.12.28 -------------------
		  //LCDWordAniBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
		  //LCDWordAniBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
#if !((defined WAP_B1) || (defined WAP_B2))
		  *(LCDWordAniBuffer + nIndexY*WAP_LCD_TOTAL_ADDRESS_X + nIndexX) &= ~MaskBitPattern;
		  *(LCDWordAniBuffer + nIndexY*WAP_LCD_TOTAL_ADDRESS_X + nIndexX) |= DrawPattern;
#else  //#if !((defined WAP_B1) || (defined WAP_B2))
		  *(LCDWordAniBuffer + nIndexY*WAP_LCD_TOTAL_ADDRESS_X + nIndexX) = DrawPattern;
		  *(LCDWordAniBuffer + nIndexY*WAP_LCD_TOTAL_ADDRESS_X + nIndexX) &= MaskBitPattern;
#endif //#if !((defined WAP_B1) || (defined WAP_B2))
		break;
  } // end of switch loop 
}// end of WAP_DrawWordBitToLCDBuffer


WORD WAP_OutWordCharLCDBuffer
( 
  BYTE *pFont, 
  int nPosX, 
  int nPosY,
  int nFontWidth, 
  int nFontHeight, 
  int nDrawMode 
)
{
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_DWORD_HWORD	BitPattern, BitMask;

	int		nIndexX, nIndexY, nBitPosX;
	WORD	i;	
    WORD    ret_width;

	BYTE BitMaskRear, BitPatternRear;

	//====================
	//for double buffering
	nFontWidth *= 2;
	//====================

	//------------------------------------------------
	//y���� WAP_HEAD_HEIGHT��ŭ �Ű��ش�.
//	nPosY = nPosY + WAP_HEAD_HEIGHT;	//yjahn96 00.12.28

	if( nPosX <= (WAP_LCD_WIDTH * 2)- nFontWidth) 
		ret_width = nFontWidth;
	else
		ret_width = 0;

	nIndexX = nPosX / 8;
	nBitPosX = (nPosX % 8);
	nIndexY = nPosY;
	if ( nBitPosX < 0 )
	{
		nIndexX --;
		nBitPosX = 8 + nBitPosX;
	}

	BitMask.DWord = ~ ( (DWORD)0xffffffff >> nFontWidth );
	BitMask.DWord >>= nBitPosX;

	// girin. 2000.2.21
	// If a font is splitted between two 'nIndexX', the rear part would be drawn
	// after the front part done.
	if (nBitPosX + nFontWidth > 32)
	{
		BitMaskRear = ~ ((byte)0xff >> (nBitPosX + nFontWidth - 32)) ;
	}
	else
	{
		BitMaskRear = 0;
	}

	for ( i = 0;
		( i < nFontHeight ) && ( nIndexY < WAP_LCD_TOTAL_ADDRESS_Y );
		i ++, nIndexY ++, pFont +=2 )
	{
		if ( nIndexX >= -4)
		{
			BitPattern.DWord = 0;	

			BitPattern.HWord.high.Byte.high = *pFont;
			if ( nFontWidth > 8)		
				BitPattern.HWord.high.Byte.low = *( pFont + 1 );
			if ( nFontWidth > 16) 	
				BitPattern.HWord.low.Byte.high = *( pFont + nFontHeight*2);
			if ( nFontWidth > 24 )
				BitPattern.HWord.low.Byte.low = *( pFont + 1 + nFontHeight*2);

			if (BitMaskRear != 0)
			{
				BitPatternRear = BitPattern.HWord.low.Byte.low << (8 - nBitPosX);
			}
			// To pacify the compiler
			else
			{
				BitPatternRear = 0;
			}
	
			BitPattern.DWord >>= nBitPosX;
		
			if ( ( nIndexX >= 0 ) && ( nIndexX < (WAP_LCD_TOTAL_ADDRESS_X * 2) ) )
			{
				WAP_DrawWordBitToLCDBuffer ( nIndexX, nIndexY,
					BitPattern.HWord.high.Byte.high,
					BitMask.HWord.high.Byte.high,
					nDrawMode );
			}
		
			if ( ( nIndexX + 1 >= 0 ) &&
				( nIndexX + 1 < WAP_LCD_TOTAL_ADDRESS_X  * 2) )
			{
				WAP_DrawWordBitToLCDBuffer ( nIndexX + 1, nIndexY,
					BitPattern.HWord.high.Byte.low,
					BitMask.HWord.high.Byte.low,
					nDrawMode );
			}
		
			if ( ( nIndexX + 2 >= 0 ) &&
				( nIndexX + 2 < WAP_LCD_TOTAL_ADDRESS_X * 2) &&
				( nFontWidth > 9))
			{
				WAP_DrawWordBitToLCDBuffer ( nIndexX + 2, nIndexY,
					BitPattern.HWord.low.Byte.high,
					BitMask.HWord.low.Byte.high,
					nDrawMode );
			}
	
			if ( ( nIndexX + 3 >= 0 ) &&
				( nIndexX + 3 < WAP_LCD_TOTAL_ADDRESS_X * 2) &&
				( nFontWidth > 17 ))
			{
				WAP_DrawWordBitToLCDBuffer ( nIndexX + 3, nIndexY,
					BitPattern.HWord.low.Byte.low,
					BitMask.HWord.low.Byte.low,
					nDrawMode );

				///////////////////////
				// Rear Part Drawing //
				///////////////////////

				if ((BitMaskRear) && (nIndexX + 4 < WAP_LCD_TOTAL_ADDRESS_X * 2))
				{
					WAP_DrawWordBitToLCDBuffer ( nIndexX + 4, nIndexY,
											BitPatternRear,
											BitMaskRear,
											nDrawMode );
				}
	
			}
		}
	}
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	int		nIndexX, nIndexY, nInterval;
    WORD	i, j;
    WORD    ret_width;

    BYTE BitMaskRear, BitPatternRear=0x00;

    BYTE bTextColor = EQS_GetTextColor();
    BYTE bTextBackColor = EQS_GetTextBackColor();

	if( nPosX <= WAP_LCD_WIDTH- nFontWidth) 
		ret_width = nFontWidth;
	else
		ret_width = 0;

    nIndexX = nPosX;
	nIndexY = nPosY;
    nInterval = nFontWidth/8 + 1;

	for ( i = 0;
		( i < nFontHeight ) && ( nIndexY < WAP_LCD_TOTAL_ADDRESS_Y );
		i ++, nIndexY ++, pFont += nInterval )
	{

        for(j = 0; j< nFontWidth ; j++)
        {
            BitPatternRear = gen_getbit(*(pFont + j/8), ChangeBitPosition(j%8));

            if (BitPatternRear == 0x01) BitPatternRear = bTextColor;//WAP_WHITE;
			else BitPatternRear = bTextBackColor;//WAP_BLACK;

            BitMaskRear = WAP_WHITE;

            WAP_DrawWordBitToLCDBuffer ( nIndexX + j, nIndexY,
					BitPatternRear,
					BitMaskRear,
					nDrawMode );

        }
    }
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	return ( ret_width );	
}

WORD WAP_OutWordCharLCDBufferHan
( 
  BYTE *pFont, 
  int nPosX, 
  int nPosY,
  int nFontWidth, 
  int nFontHeight, 
  int nDrawMode 
)
{
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_DWORD_HWORD	BitPattern, BitMask;

	int		nIndexX, nIndexY, nBitPosX;
	WORD	i;	
    WORD    ret_width;

	BYTE BitMaskRear, BitPatternRear;

	//====================
	//for double buffering
	nFontWidth *= 2;
	//====================

	if( nPosX <= (WAP_LCD_WIDTH * 2)- nFontWidth) 
		ret_width = nFontWidth;
	else
		ret_width = 0;

	nIndexX = nPosX / 8;
	nBitPosX = (nPosX % 8);
	nIndexY = nPosY;
	if ( nBitPosX < 0 )
	{
		nIndexX --;
		nBitPosX = 8 + nBitPosX;
	}

	BitMask.DWord = ~ ( (DWORD)0xffffffff >> nFontWidth );
	BitMask.DWord >>= nBitPosX;

	// girin. 2000.2.21
	// If a font is splitted between two 'nIndexX', the rear part would be drawn
	// after the front part done.
	if (nBitPosX + nFontWidth > 32)
	{
		BitMaskRear = ~ ((byte)0xff >> (nBitPosX + nFontWidth - 32)) ;
	}
	else
	{
		BitMaskRear = 0;
	}

	for ( i = 0;
		( i < nFontHeight ) && ( nIndexY < WAP_LCD_TOTAL_ADDRESS_Y );
		i ++, nIndexY ++, pFont +=4 )
	{
		if ( nIndexX >= -4)
		{
			BitPattern.DWord = 0;	

			BitPattern.HWord.high.Byte.high = *pFont;
			if ( nFontWidth > 8)		
				BitPattern.HWord.high.Byte.low = *( pFont + 1 );
			if ( nFontWidth > 16) 	
				BitPattern.HWord.low.Byte.high = *( pFont + 2);//nFontHeight*2);
			if ( nFontWidth > 24 )
				BitPattern.HWord.low.Byte.low = *( pFont + 1 + 2);//nFontHeight*2);

			if (BitMaskRear != 0)
			{
				BitPatternRear = BitPattern.HWord.low.Byte.low << (8 - nBitPosX);
			}
			// To pacify the compiler
			else
			{
				BitPatternRear = 0;
			}
	
			BitPattern.DWord >>= nBitPosX;
		
			if ( ( nIndexX >= 0 ) && ( nIndexX < (WAP_LCD_TOTAL_ADDRESS_X * 2) ) )
			{
				WAP_DrawWordBitToLCDBuffer ( nIndexX, nIndexY,
					BitPattern.HWord.high.Byte.high,
					BitMask.HWord.high.Byte.high,
					nDrawMode );
			}
		
			if ( ( nIndexX + 1 >= 0 ) &&
				( nIndexX + 1 < WAP_LCD_TOTAL_ADDRESS_X  * 2) )
			{
				WAP_DrawWordBitToLCDBuffer ( nIndexX + 1, nIndexY,
					BitPattern.HWord.high.Byte.low,
					BitMask.HWord.high.Byte.low,
					nDrawMode );
			}
		
			if ( ( nIndexX + 2 >= 0 ) &&
				( nIndexX + 2 < WAP_LCD_TOTAL_ADDRESS_X * 2) &&
				( nFontWidth > 9))
			{
				WAP_DrawWordBitToLCDBuffer ( nIndexX + 2, nIndexY,
					BitPattern.HWord.low.Byte.high,
					BitMask.HWord.low.Byte.high,
					nDrawMode );
			}
	
			if ( ( nIndexX + 3 >= 0 ) &&
				( nIndexX + 3 < WAP_LCD_TOTAL_ADDRESS_X * 2) &&
				( nFontWidth > 17 ))
			{
				WAP_DrawWordBitToLCDBuffer ( nIndexX + 3, nIndexY,
					BitPattern.HWord.low.Byte.low,
					BitMask.HWord.low.Byte.low,
					nDrawMode );

				///////////////////////
				// Rear Part Drawing //
				///////////////////////

				if ((BitMaskRear) && (nIndexX + 4 < WAP_LCD_TOTAL_ADDRESS_X * 2))
				{
					WAP_DrawWordBitToLCDBuffer ( nIndexX + 4, nIndexY,
											BitPatternRear,
											BitMaskRear,
											nDrawMode );
				}
	
			}
		}
	}
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	int		nIndexX, nIndexY, nInterval;
    WORD	i, j;
    WORD    ret_width;

    BYTE BitMaskRear, BitPatternRear=0x00;

    BYTE bTextColor = EQS_GetTextColor();
    BYTE bTextBackColor = EQS_GetTextBackColor();

	if( nPosX <= WAP_LCD_WIDTH- nFontWidth) 
		ret_width = nFontWidth;
	else
		ret_width = 0;

    nIndexX = nPosX;
	nIndexY = nPosY;
    nInterval = nFontWidth/8 + 1;

	for ( i = 0;
		( i < nFontHeight ) && ( nIndexY < WAP_LCD_TOTAL_ADDRESS_Y );
		i ++, nIndexY ++, pFont += nInterval )
	{

        for(j = 0; j< nFontWidth ; j++)
        {
            BitPatternRear = gen_getbit(*(pFont + j/8), ChangeBitPosition(j%8));

            if (BitPatternRear == 0x01) BitPatternRear = bTextColor;//WAP_WHITE;
			else BitPatternRear = bTextBackColor;//WAP_BLACK;

            BitMaskRear = WAP_WHITE;

            WAP_DrawWordBitToLCDBuffer ( nIndexX + j, nIndexY,
					BitPatternRear,
					BitMaskRear,
					nDrawMode );

        }
    }
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	return ( ret_width );	
}

int WAP_TextAlign(char *strText, int nPosX, int nFontType)
{
	int wChar;
	int nCharWidth;

	int nOriginX = nPosX;
	int nTextWidth = nPosX;
	char *pText = strText;

	int i;

	//--------------------------------------------------------
	int nLoopCount;
	int nHanFontWidth, nAsciiFontWidth;
	if(nFontType == WAP_FONT_MEDIUM)
	{
		nLoopCount = 20;//20
		nHanFontWidth	= WAP_MED_HAN_FONT_WIDTH;
		nAsciiFontWidth	= WAP_MED_ASCII_FONT_WIDTH;
	}
	else
	{
		nLoopCount = 20;//16;//16
		nHanFontWidth	= WAP_HAN_FONT_WIDTH;
		nAsciiFontWidth	= WAP_ASCII_FONT_WIDTH;
	}

	for(i=nLoopCount-nPosX/nAsciiFontWidth; i>0; i--)
	{
		/*=================================================*/
		//line break ó�� (���� ���� ���)
		/*=================================================*/
		if(((*pText & 0x80) && (nTextWidth+nHanFontWidth > WAP_LCD_WIDTH))//�ѱ�
		  ||(!(*pText & 0x80) && (nTextWidth+nAsciiFontWidth > WAP_LCD_WIDTH)))//�ƽ�Ű
		{
			return(nTextWidth - nOriginX);
		}

		if(*pText == 0) break;
		wChar = *pText;

		/*=================================================*/
		if ( wChar & 0x80 )              /* �ѱ� ó��      */
		/*=================================================*/
		{	  
			pText++; i--;
			if (*pText == 0) break;

			wChar <<= 8; wChar += *pText;

			nCharWidth = nHanFontWidth;
		}
		/*=================================================*/
		else							/* �ƽ�Ű ó��     */
		/*=================================================*/
		{
			nCharWidth = nAsciiFontWidth;
		}

		/*=================================================*/
		// �� ���� ǥ�� ���� ó��
		/*=================================================*/
		nTextWidth += nCharWidth;
		nPosX += nCharWidth;
		pText++;
		
	}/* end of for */
	
	return( nTextWidth - nOriginX );
}



/////////////////////////////////////////////////////////////////////////////
//
//		WAP �ؽ�Ʈ
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_TextOut(int x, int y, char* str,
			 int font, int mode, BOOL underline, int align,
			 int nTableColumns, int nCurColumn)
{
#if (defined WAP_B1) || (defined WAP_B2)
    extern void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen);

#ifdef BOLD
    COLOR OldColor=WHITE;
#else	//#ifdef BOLD
    COLOR OldTextBackColor=WHITE;
    COLOR OldTextColor=WHITE;
#endif	//#ifdef BOLD
	int nLength=0, nLen=0, leng=0, nLimit=0;
	char Text[21];
    
	static int nPrev_X = 120, nHeight = WAP_ASCII_FONT_HEIGHT; //Mixfont�� line�Ѿ�� ����

    char *pStr = str;
	char *pTemp;

	int nAsciiFontWidth=0, nAsciiFontHeight=0;
    int nLineChars=0, nLineWidth=0, nOffset=0;

    if(font == WAP_FONT_MEDIUM)
    {
        EQS_SetFontType(FT_MED);
		nAsciiFontWidth	= WAP_MED_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_MED_ASCII_FONT_HEIGHT;

        nLineChars = WAP_MED_SCREEN_WIDTH/nTableColumns;
    }
    else//if(font == WAP_FONT_NORMAL)
    {
        EQS_SetFontType(FT_BIG);
		nAsciiFontWidth	= WAP_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_ASCII_FONT_HEIGHT;

        //�� line�� �� ����(byte) ��
        nLineChars = WAP_SCREEN_WIDTH/nTableColumns;
    }

#ifdef BOLD
    if(mode == WAP_DRAW_REVERSE)
    {
        OldColor = EQS_GetTextBackColor();
        EQS_SetTextBackColor(EQS_GetBackColor());
    }
#else	//#ifdef BOLD

	//�� ����
    OldTextBackColor = EQS_GetTextBackColor();
    OldTextColor = EQS_GetTextColor();

    if(mode == WAP_DRAW_REVERSE)
    {
        EQS_SetTextBackColor((COLOR)WAP_SELECT_BACK_COLOR);
        EQS_SetTextColor((COLOR)WAP_SELECT_TEXT_COLOR);
    }
	else
	{
        EQS_SetTextBackColor((COLOR)WAP_BASIC_BACK_COLOR);
        EQS_SetTextColor((COLOR)WAP_BASIC_TEXT_COLOR);
	}
#endif	//#ifdef BOLD

    nLineWidth = WAP_LCD_WIDTH/nTableColumns;
   
    if(nCurColumn == 0) nCurColumn = 1;
    nOffset = nLineWidth*(nCurColumn-1);    //for Table

	//-------------------------------------------------------
    // (x!=0)�� ��� 
	// 1. Text element�� �������� �ͼ� �� element���� �����ؼ� display�Ǿ� �Ҷ�....
	// 2. Table���� Text�� ��� 
    if(x !=0 )
    {
       int nChar = (nLineWidth - (x - nOffset))/nAsciiFontWidth;
	   
       memset(Text, NULL, 21);
	   WAP_HanCopy((char *)Text, pStr, nChar);
	   nLen = strlen((char *)Text);
 
		if(y >= 0)//ȭ�鿡 ������ �� �κи�...
		{	   
			if(x <= nPrev_X)	//�� line�� ����
			{
				nHeight = nAsciiFontHeight;
			}
			else					//���� line�� ����
			{
				if(nHeight < nAsciiFontHeight)
					nHeight = nAsciiFontHeight;
			}
			nPrev_X = x;

#ifdef BOLD
			if(mode == WAP_DRAW_REVERSE)
			{
			   EQS_TextOutStrLenTDM(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nLen, TDM_BOLD);
			}
		    else
#endif  //#ifdef BOLD
			{
				EQS_TextOutStrLen(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nLen);//nChar);//nLineChars); 
			}
		   
		   if(underline)
			   EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
						x+nLen*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);            
		} 
		y += nHeight;//nAsciiFontHeight;

        pStr = pStr + nLen;
    }

	nLength = strlen(pStr);

	if(nLength > nLineChars)
	{
		do{

			memset(Text, NULL, 21);
			pTemp = pStr + nLimit;
			WAP_HanCopy((char *)Text, pTemp, nLineChars);
            leng = strlen((char *)Text);
			nLimit = nLimit + leng;//nLen = nLen + leng;

            if(y >= 0)//ȭ�鿡 ������ �� �κи�...
            {
                if(leng >= nLineChars)    //align�ǹ� ����, ������ left
                {
					if(nOffset <= nPrev_X)	//�� line�� ����
					{
						nHeight = nAsciiFontHeight;
					}
					else					//���� line�� ����
					{
						if(nHeight < nAsciiFontHeight)
							nHeight = nAsciiFontHeight;
					}
					nPrev_X = nOffset;

#ifdef BOLD
				   if(mode == WAP_DRAW_REVERSE)
				   {
					   EQS_TextOutStrLenTDM(nOffset, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng, TDM_BOLD);//nLineChars);  
				   }
				   else
#endif  //#ifdef BOLD
				   {
						EQS_TextOutStrLen(nOffset, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng);//nLineChars);   
				   }
                    if(underline)
                        EQS_Line(nOffset, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                                 nOffset+leng*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);
                }
                else    //align����
                {
					//align
					if(align == WAP_TEXT_ALIGN_LEFT)
					{
						x = 0 + nOffset;
					}
					else if(align == WAP_TEXT_ALIGN_CENTER)
					{
						x = ((nLineWidth - (leng*nAsciiFontWidth))/2) + nOffset;
					}
					else if(align == WAP_TEXT_ALIGN_RIGHT)
					{
						x = (nLineWidth - (leng*nAsciiFontWidth)) + nOffset;
					}

					if(x <= nPrev_X)	//�� line�� ����
					{
						nHeight = nAsciiFontHeight;
					}
					else					//���� line�� ����
					{
						if(nHeight < nAsciiFontHeight)
							nHeight = nAsciiFontHeight;
					}
					nPrev_X = x;

#ifdef BOLD
					if(mode == WAP_DRAW_REVERSE)
					{
					   EQS_TextOutStrLenTDM(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng, TDM_BOLD);//nLineChars);  
					}
				    else
#endif  //#ifdef BOLD
					{
						EQS_TextOutStrLen(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng);   
					}

                    if(underline)
                        EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                                 x+leng*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);            
                }
            }

            y += nHeight;//nAsciiFontHeight;
            if(y > WAP_DOC_HEIGHT-1)    break;  //LCD������ ������ ���´�..
		}
		while(nLimit/*nLen*/ != nLength);
	}
	else
	{
		//align
		if(align == WAP_TEXT_ALIGN_LEFT)
		{
			x = 0 + nOffset;
		}
		else if(align == WAP_TEXT_ALIGN_CENTER)
		{
			x = ((nLineWidth - (nLength*nAsciiFontWidth))/2) + nOffset;
		}
		else if(align == WAP_TEXT_ALIGN_RIGHT)
		{
			x = (nLineWidth - (nLength*nAsciiFontWidth)) + nOffset;
		}

        if(y >= 0)
        {
            memset(Text, NULL, 21);
		    memcpy(Text, pStr, nLength);

			if(x <= nPrev_X)	//�� line�� ����
			{
				nHeight = nAsciiFontHeight;
			}
			else					//���� line�� ����
			{
				if(nHeight < nAsciiFontHeight)
					nHeight = nAsciiFontHeight;
			}
			nPrev_X = x;

#ifdef BOLD
			if(mode == WAP_DRAW_REVERSE)
			{
   	            EQS_TextOutStrLenTDM(x, y+WAP_HEAD_HEIGHT, (BYTE*)Text, nLength, TDM_BOLD);//nLineChars);  
  
			}
		    else
#endif  //#ifdef BOLD
			{
	            EQS_TextOutStrLen(x, y+WAP_HEAD_HEIGHT, (BYTE*)Text, nLength);//nLineChars);  
			}
            if(underline)
                EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                        x+nLength*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);
        }
	}

#ifdef BOLD
    if(mode == WAP_DRAW_REVERSE)
    {
        EQS_SetTextBackColor(OldColor);
    }
#else	//#ifdef BOLD
    EQS_SetTextBackColor(OldTextBackColor);
    EQS_SetTextColor(OldTextColor);
#endif	//#ifdef BOLD

#else   //#if (defined WAP_B1) || (defined WAP_B2)
	int j, lineCount, nAlignX;
	int curWordCount = 0;
	BYTE *pText = (BYTE *)str;

	int nAsciiFontWidth, nAsciiFontHeight;
	if(font == WAP_FONT_MEDIUM)
	{
		nAsciiFontWidth	= WAP_MED_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_MED_ASCII_FONT_HEIGHT;
	}
	else
	{
		nAsciiFontWidth	= WAP_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_ASCII_FONT_HEIGHT;
	}
	//================================================================
	// �� ������ ���� y ���� ���Ͽ� "����� ũ��" �����Ͽ� loop�� ����
	// loop�� ���� ���÷����� ���ڰ� ���� ��� ������ �� �ִ�
	// ���� ���� �ִ밪�� HS_SCRN_HIGH(=8)�̴�
	lineCount = ((int)((WAP_DOC_HEIGHT-y)/nAsciiFontHeight)) + 1;
	for(j=0; j<lineCount; j++)
	{
		//================================================================
		// �� ���� ó�� �� ����
		if(*pText == 0) break;
		
		if(j!=0) x=0;//not frist line
	
		//================================================================
		// Text Alignment
		nAlignX = 0;
		if(align != WAP_TEXT_ALIGN_LEFT)
		{
			char *temp = (char*)pText;
			nAlignX = WAP_TextAlign(temp, x, font);

			if(align == WAP_TEXT_ALIGN_CENTER)		nAlignX = (WAP_LCD_WIDTH-nAlignX)/2;//center
			else if(align == WAP_TEXT_ALIGN_RIGHT)	nAlignX = WAP_LCD_WIDTH-nAlignX;//right
			else									nAlignX = 0;//left
		}
#if !((defined WAP_B1) || (defined WAP_B2))
		nAlignX *= 2;//for double buffering
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

		//================================================================
		// �� ������ ó���Ѵ� (multi font ������ ���)
		if(font == WAP_FONT_MEDIUM)
		{
			// Medium Font Output
			curWordCount = WAP_MediumTextOutXY((BYTE *)pText, x+nAlignX, y, mode, nTableColumns, nCurColumn);
		}
		else//if(font == WAP_FONT_NORMAL)
		{
			// Normal Font Output
			curWordCount = WAP_NormalTextOutXY((BYTE *)pText, x+nAlignX, y, mode, nTableColumns, nCurColumn);
		}

		if(underline)
		{
			int nNewX;
			//------------------------------------------------------------------
			// Table data ���� x ��ǥ ó��
			if(nTableColumns > 1)//Table data
#if !((defined WAP_B1) || (defined WAP_B2))
				nNewX = WAP_LCD_WIDTH/nTableColumns * (nCurColumn-1) * 2;//for double buffering
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				nNewX = WAP_LCD_WIDTH/nTableColumns * (nCurColumn-1);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
			else
				nNewX = x;

			if(mode == WAP_DRAW_NORMAL)
			{
#if !((defined WAP_B1) || (defined WAP_B2))
				WAP_Display_Line(	(nNewX+nAlignX)/2,//for double buffering
									y+nAsciiFontHeight-1,
									(nNewX+nAlignX)/2+(curWordCount*nAsciiFontWidth),//for double buffering
									y+nAsciiFontHeight-1,
									WAP_BLACK);//������ ����
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				WAP_Display_Line(	(nNewX+nAlignX),
									y+nAsciiFontHeight-1,
									(nNewX+nAlignX)+(curWordCount*nAsciiFontWidth),
									y+nAsciiFontHeight-1,
									WAP_RED);//WAP_WHITE);//�Ͼ�� ����
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
			}
			else
			{
#if !((defined WAP_B1) || (defined WAP_B2))
				WAP_Display_Line(	(nNewX+nAlignX)/2,//for double buffering
									y+nAsciiFontHeight-1,
									(nNewX+nAlignX)/2+(curWordCount*nAsciiFontWidth),//for double buffering
									y+nAsciiFontHeight-1,
									WAP_WHITEGRAY);//�Ͼ�� ����
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				WAP_Display_Line(	(nNewX+nAlignX),
									y+nAsciiFontHeight-1,
									(nNewX+nAlignX)+(curWordCount*nAsciiFontWidth),
									y+nAsciiFontHeight-1,
									WAP_LIGHTRED);//WAP_GRAY);//������ ����

#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
			}
		}
		
		//================================================================
		// �� ���� ó�� �� ����
		y += nAsciiFontHeight;//y �� ����

		pText = pText + curWordCount; //  /nAsciiFontWidth;

	}	//end of for
#endif //#if (defined WAP_B1) || (defined WAP_B2)
}


void WAP_Display_TextOut_Focus(int x, int y, char* str,
			 int font, int mode, BOOL underline, int align,
			 int nTableColumns, int nCurColumn)
{
    extern void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen);

#ifdef BOLD
    COLOR OldColor=WHITE;
#else //#ifdef BOLD
    COLOR OldTextBackColor=WHITE;
    COLOR OldTextColor=WHITE;
#endif	//#ifdef BOLD

	int nLength=0, nLen=0, leng=0, nLimit=0;
	char Text[21];
    
    char *pStr = str;
	char *pTemp;

	int nAsciiFontWidth=0, nAsciiFontHeight=0;
    int nLineChars=0, nLineWidth=0, nOffset=0;

    //SetScreenChangable(FALSE);

    if(font == WAP_FONT_MEDIUM)
    {
        EQS_SetFontType(FT_MED);
		nAsciiFontWidth	= WAP_MED_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_MED_ASCII_FONT_HEIGHT;

        nLineChars = WAP_MED_SCREEN_WIDTH/nTableColumns;
    }
    else//if(font == WAP_FONT_NORMAL)
    {
        EQS_SetFontType(FT_BIG);
		nAsciiFontWidth	= WAP_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_ASCII_FONT_HEIGHT;

        //�� line�� �� ����(byte) ��
        nLineChars = WAP_SCREEN_WIDTH/nTableColumns;
    }

#ifdef BOLD
    if(mode == WAP_DRAW_REVERSE)
    {
        OldColor = EQS_GetTextBackColor();
        EQS_SetTextBackColor(EQS_GetBackColor());
    }
#else //#ifdef BOLD
	//�� ����
    OldTextBackColor = EQS_GetTextBackColor();
    OldTextColor = EQS_GetTextColor();

    if(mode == WAP_DRAW_REVERSE)
    {
        EQS_SetTextBackColor((COLOR)WAP_SELECT_BACK_COLOR);
        EQS_SetTextColor((COLOR)WAP_SELECT_TEXT_COLOR);
    }
	else
	{
        EQS_SetTextBackColor((COLOR)WAP_BASIC_BACK_COLOR);
        EQS_SetTextColor((COLOR)WAP_BASIC_TEXT_COLOR);
	}

#endif	//#ifdef BOLD

    nLineWidth = WAP_LCD_WIDTH/nTableColumns;
   
    if(nCurColumn == 0) nCurColumn = 1;
    nOffset = nLineWidth*(nCurColumn-1);    //for Table

	//-------------------------------------------------------
    // (x!=0)�� ��� 
	// 1. Text element�� �������� �ͼ� �� element���� �����ؼ� display�Ǿ� �Ҷ�....
	// 2. Table���� Text�� ��� 
    if(x !=0 )
    {
       int nChar = (nLineWidth - (x - nOffset))/nAsciiFontWidth;
	   
       memset(Text, NULL, 21);
	   WAP_HanCopy((char *)Text, pStr, nChar);
	   nLen = strlen((char *)Text);
 
		if(y >= 0)//ȭ�鿡 ������ �� �κи�...
		{	   
#ifdef BOLD
		   if(mode == WAP_DRAW_REVERSE)
		   {
			   EQS_TextOutStrLenTDM(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nChar, TDM_BOLD);//nLineChars);
		   }
		   else
#endif  //#ifdef BOLD
		   {
			   EQS_TextOutStrLen(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nChar);//nLineChars);   
		   }
		   if(underline)
			   EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
						x+nLen*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);            
		} 
		y += nAsciiFontHeight;

        pStr = pStr + nLen;
    }

	nLength = strlen(pStr);

	if(nLength > nLineChars)
	{
		//nLen = 0;	//init
		do{

			memset(Text, NULL, 21);
			pTemp = pStr + nLimit;
			WAP_HanCopy((char *)Text, pTemp, nLineChars);//WAP_HanCopy((char *)Text, pStr + nLimit/*nLen*/, nLineChars);
            leng = strlen((char *)Text);
			nLimit = nLimit + leng;//nLen = nLen + leng;

            if(y >= 0)//ȭ�鿡 ������ �� �κи�...
            {
                if(leng >= nLineChars)    //align�ǹ� ����, ������ left
                {
#ifdef BOLD
					if(mode == WAP_DRAW_REVERSE)
				   {
						EQS_TextOutStrLenTDM(nOffset, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nLineChars, TDM_BOLD);//leng);
				   }
				   else
#endif  //#ifdef BOLD
				   {
						EQS_TextOutStrLen(nOffset, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nLineChars);//leng);   
				   }
                   if(underline)
                        EQS_Line(nOffset, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                                 nOffset+leng*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);
                }
                else    //align����
                {
					//align
					if(align == WAP_TEXT_ALIGN_LEFT)
					{
						x = 0 + nOffset;
					}
					else if(align == WAP_TEXT_ALIGN_CENTER)
					{
						x = ((nLineWidth - (leng*nAsciiFontWidth))/2) + nOffset;
					}
					else if(align == WAP_TEXT_ALIGN_RIGHT)
					{
						x = (nLineWidth - (leng*nAsciiFontWidth)) + nOffset;
					}

#ifdef BOLD
					if(mode == WAP_DRAW_REVERSE)
					{
						EQS_TextOutStrLenTDM(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng, TDM_BOLD);   
					}
					else
#endif  //#ifdef BOLD
					{
						EQS_TextOutStrLen(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng);   
					}
                    if(underline)
                        EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                                 x+leng*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);            
                }
            }

            y += nAsciiFontHeight;
			if(y > WAP_DOC_HEIGHT-1)    break;  //LCD������ ������ ���´�..
		}
		while(nLimit/*nLen*/ != nLength);
	}
	else
	{
		//align
		if(align == WAP_TEXT_ALIGN_LEFT)
		{
			x = 0 + nOffset;
		}
		else if(align == WAP_TEXT_ALIGN_CENTER)
		{
			x = ((nLineWidth - (nLength*nAsciiFontWidth))/2) + nOffset;
		}
		else if(align == WAP_TEXT_ALIGN_RIGHT)
		{
			x = (nLineWidth - (nLength*nAsciiFontWidth)) + nOffset;
		}

        if(y >= 0)
        {
            memset(Text, NULL, 21);
		    memcpy(Text, pStr, nLength);

#ifdef BOLD
            if(mode == WAP_DRAW_REVERSE)
			{
				EQS_TextOutStrLenTDM(x, y+WAP_HEAD_HEIGHT, (BYTE*)Text, nLineChars, TDM_BOLD);//nLength);  
			}
			else
#endif  //#ifdef BOLD
			{
				EQS_TextOutStrLen(x, y+WAP_HEAD_HEIGHT, (BYTE*)Text, nLineChars);//nLength);  
			}
            if(underline)
                EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                        x+nLength*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);
        }
	}

#ifdef BOLD
    if(mode == WAP_DRAW_REVERSE)
    {
        EQS_SetTextBackColor(OldColor);
    }
#else	//#ifdef BOLD
     EQS_SetTextBackColor(OldTextBackColor);
     EQS_SetTextColor(OldTextColor);
#endif	//#ifdef BOLD

    //SetScreenChangable(TRUE);
}


void WAP_Display_TextOut_Title(int x, int y, char* str,
			 int font, int mode, BOOL underline, int align,
			 int nTableColumns, int nCurColumn)
{
    COLOR OldColor;
    TEXT_ALIGN TextAlign;

	char Text[21];

	int nAsciiFontWidth=0, nAsciiFontHeight=0;
    int nLineChars=0;

    y = y + WAP_HEAD_HEIGHT;

    //-----------------------------------------------------
    // Font Size
    if(font == WAP_FONT_MEDIUM)
    {
        EQS_SetFontType(FT_MED);
		nAsciiFontWidth	= WAP_MED_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_MED_ASCII_FONT_HEIGHT;

        nLineChars = WAP_MED_SCREEN_WIDTH/nTableColumns;
    }
    else//if(font == WAP_FONT_NORMAL)
    {
        EQS_SetFontType(FT_BIG);
		nAsciiFontWidth	= WAP_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_ASCII_FONT_HEIGHT;

        //�� line�� �� ����(byte) ��
        nLineChars = WAP_SCREEN_WIDTH/nTableColumns;
    }

    //-----------------------------------------------------
    // Text align
	memset((char*)Text, NULL, 21);
	strncpy((char*)Text, str, nLineChars);

    EQS_GetTextAlign(&TextAlign);
    
    if(align == WAP_TEXT_ALIGN_CENTER)
    {
        EQS_SetTextAlignHoriz(TAH_CENTER);
        x = WAP_LCD_WIDTH/2;
    }
    else if(align == WAP_TEXT_ALIGN_RIGHT)
    {
        x = WAP_LCD_WIDTH -1;
        EQS_SetTextAlignHoriz(TAH_RIGHT);
    }

    //-----------------------------------------------------
    // Title draw
    //EQS_FillRectangle(0, y, WAP_LCD_TOTAL_ADDRESS_X-1, y, 223); //w-line    
    EQS_FillRectangle(0, y, WAP_LCD_WIDTH -1, y+WAP_ASCII_FONT_HEIGHT-1, (COLOR)WAP_TITLE_BACK_COLOR);
    //EQS_FillRectangle(0, y+WAP_ASCII_FONT_HEIGHT-1, WAP_LCD_TOTAL_ADDRESS_X-1, y+WAP_ASCII_FONT_HEIGHT-1, 14); //b-line
    OldColor = EQS_GetTextColor();
    EQS_SetTextColor((COLOR)WAP_SELECT_TEXT_COLOR);
    EQS_SetDrawMode(DM_OR);
    EQS_TextOutStr(x, y, (BYTE*)Text);//(BYTE*)str);
//    EQS_TextOutLenTDM(x, y, (BYTE*)str, WAP_LCD_TOTAL_ADDRESS_X-1 ,underline ? TDM_UNDERLINE : TDM_NORMAL);
    EQS_SetDrawMode(DM_COPY);
    EQS_SetTextColor(OldColor);

    EQS_SetTextAlignHoriz(TextAlign.Horiz);

    EQS_SetTextWrap(FALSE);

    //SetScreenChangable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//
//		WAP �̹���
//
/////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------
// yjahn96 01.03.04
// Display command line image
void WAP_Display_Image_CML(int kind_of_cml, int image ,int xPos, int yPos)
{
    //BITMAP *pImage = NULL;
    BITMAP Bitmap;

    if(xPos > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	if(yPos > WAP_LCD_HEIGHT) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

    if(kind_of_cml == WAP_LEFT_CML)     Bitmap = g_BmpWapCommandLineLeft;//&g_BmpNewWAPLeftCml40x13x4G;
    else if(kind_of_cml == WAP_UPDOWN_CML)   Bitmap = g_BmpWapCommandLineCenterArrow;//&g_BmpNewWAPUpDown10x13x4G;
    else if(kind_of_cml == WAP_OK_CML)       Bitmap = g_BmpWapCommandLineCenterOK;//&g_BmpNewWAPOK9x13x4G;
    else if(kind_of_cml == WAP_CENTER_CML)   Bitmap = g_BmpWapCommandLineCenter;//&g_BmpNewWAPCenter21x13x4G;
    else if(kind_of_cml == WAP_RIGHT_CML)    Bitmap = g_BmpWapCommandLineRight;//&g_BmpNewWAPRightCml40x13x4G;

    //Bitmap.Header = pImage->Header;
    Bitmap.pbData = Bitmap.pbData +  EQC_GET_FRAME_SIZE(Bitmap.Header) * image;
    
    EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);
	/*
    BITMAP *pImage = NULL;
    BITMAP Bitmap;

    if(xPos > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	if(yPos > WAP_LCD_HEIGHT) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

    if(kind_of_cml == WAP_LEFT_CML)     pImage = &g_BmpNewWAPLeftCml40x13x4G;
    else if(kind_of_cml == WAP_UPDOWN_CML)   pImage = &g_BmpNewWAPUpDown10x13x4G;
    else if(kind_of_cml == WAP_OK_CML)       pImage = &g_BmpNewWAPOK9x13x4G;
    else if(kind_of_cml == WAP_CENTER_CML)   pImage = &g_BmpNewWAPCenter21x13x4G;
    else if(kind_of_cml == WAP_RIGHT_CML)    pImage = &g_BmpNewWAPRightCml40x13x4G;

    Bitmap.Header = pImage->Header;
    Bitmap.pbData = pImage->pbData +  EQC_GET_FRAME_SIZE(Bitmap.Header) * image;
    
    EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);
    */
	  /*
    byte* LCDWordAniBuffer = (byte* )GetScreenBuffer();		//yjahn96 00.12.28

    BITMAP *pImage;
    BYTE *pt;
    BYTE inbuffer[30];//BYTE *inbuffer;
    BYTE imgBit = 0x00;

    int size=0, offset, order;
    int nX, nY;
    int width= 0, height = 0, Iwidth, Iheight, bwidth = 0;

    if(kind_of_cml == WAP_LEFT_CML)     pImage = &g_BmpNewWAPLeftCml40x13x4G;
    else if(kind_of_cml == WAP_UPDOWN_CML)   pImage = &g_BmpNewWAPUpDown10x13x4G;
    else if(kind_of_cml == WAP_OK_CML)       pImage = &g_BmpNewWAPOK9x13x4G;
    else if(kind_of_cml == WAP_CENTER_CML)   pImage = &g_BmpNewWAPCenter21x13x4G;
    else if(kind_of_cml == WAP_RIGHT_CML)    pImage = &g_BmpNewWAPRightCml40x13x4G;

    if((kind_of_cml == WAP_LEFT_CML) || (kind_of_cml == WAP_RIGHT_CML))
    {
        width = WAP_LEFTRIGHT_CML_WIDTH;
        height = WAP_CML_HEIGHT;
        size = WAP_LEFTRIGHT_CML_SIZE;
    }
    else if(kind_of_cml == WAP_UPDOWN_CML)
    {
        width = WAP_UPDOWN_CML_WIDTH;
        height = WAP_CML_HEIGHT;
        size = WAP_UPDOWN_CML_SIZE;
    }
    else if(kind_of_cml == WAP_OK_CML)
    {
        width = WAP_OK_CML_WIDTH;
        height = WAP_CML_HEIGHT;
        size = WAP_OK_CML_SIZE;
    }
    else if(kind_of_cml == WAP_CENTER_CML)
    {
        width = WAP_CENTER_CML_WIDTH;
        height = WAP_CML_HEIGHT;
        size = WAP_CENTER_CML_SIZE;
    }

    order = image;
    offset = order * size;
    pt = (byte *)pImage->pbData + offset;
    bwidth = (width+3)/4;

	Iwidth = pos_x + width - 1;				// �̹����� ���� �Ѱ��� 
	Iheight = pos_y + height - 1;			// �̹����� ���� �Ѱ��� 

    if(pos_x > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	if(pos_y > WAP_LCD_HEIGHT) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

    if(Iwidth > WAP_LCD_WIDTH) Iwidth = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
    if(Iheight > WAP_LCD_HEIGHT) Iheight = WAP_LCD_HEIGHT;  //�̹��� height�� LCD height���� ū ���

	if (Iwidth == WAP_LCD_WIDTH) Iwidth--;
	if (Iheight == WAP_LCD_HEIGHT) Iheight--;

	// ------------------------------------------------
	// ���� �̹����� �׸���. 
	// height�� �ϳ��� �÷����鼭 �׸���. 
    for(nY = pos_y ; nY <= Iheight ; nY++)//for(nY = Iheight ; nY >= pos_y ; nY--)
	{				
		// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
		memcpy(inbuffer, pt, bwidth);
		pt += bwidth;	//seek

 		if ((pos_y <= nY) && (nY <= (WAP_LCD_HEIGHT-1)))
		{
			// �� ������ width ��Ʈ�� �׸���. 
			for(nX=0; nX<=Iwidth; nX++)
			{
				// x �� boundary check 
				if((nX/8) > 14) break;
			
				if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) //&& (pos_y<=nY && nY<=WAP_LCD_HEIGHT))
				{
                    imgBit = inbuffer[(nX-pos_x)/4];

                    //�ֻ���2bit-����2bit-����2bit-������2bit 
                    if((nX-pos_x)%4 == 0)      // inbuffer[] ���� �ֻ��� 2bit ó��
                    {
                        imgBit &= 0xc0;

                        if(pImage->Header.fPalette)
                        {
                            if(imgBit == 0x00) imgBit = pImage->pbPalette[0];
                            else if(imgBit == 0x40) imgBit = pImage->pbPalette[1];
                            else if(imgBit == 0x80) imgBit = pImage->pbPalette[2];
                            else if(imgBit == 0xc0) imgBit = pImage->pbPalette[3];
                        }
                        else
                        {
                            if(imgBit == 0x00) imgBit = WAP_WHITE;
                            else if(imgBit == 0x40) imgBit = WAP_LIGHTGRAY;
                            else if(imgBit == 0x80) imgBit = WAP_GRAY;
                            else if(imgBit == 0xc0) imgBit = WAP_BLACK;
                        }
                    }
                    else if((nX-pos_x)%4 == 1)   // inbuffer[] ���� ���� 2bit ó��
                    {
                        imgBit &= 0x30;
                    
                        if(pImage->Header.fPalette)
                        {
                            if(imgBit == 0x00) imgBit = pImage->pbPalette[0];
                            else if(imgBit == 0x10) imgBit = pImage->pbPalette[1];
                            else if(imgBit == 0x20) imgBit = pImage->pbPalette[2];
                            else if(imgBit == 0x30) imgBit = pImage->pbPalette[3];
                        }
                        else
                        {
                            if(imgBit == 0x00) imgBit = WAP_WHITE;
                            else if(imgBit == 0x10) imgBit = WAP_LIGHTGRAY;
                            else if(imgBit == 0x20) imgBit = WAP_GRAY;
                            else if(imgBit == 0x30) imgBit = WAP_BLACK;
                        }
                    }
                    else if((nX-pos_x)%4 == 2)   // inbuffer[] ���� ���� 2bit ó��
                    {
                        imgBit &= 0x0c;

                        if(pImage->Header.fPalette)
                        {
                            if(imgBit == 0x00) imgBit = pImage->pbPalette[0];//WAP_WHITE;
                            else if(imgBit == 0x04) imgBit = pImage->pbPalette[1];
                            else if(imgBit == 0x08) imgBit = pImage->pbPalette[2];
                            else if(imgBit == 0x0c) imgBit = pImage->pbPalette[3];
                        }
                        else
                        {
                            if(imgBit == 0x00) imgBit = WAP_WHITE;
                            else if(imgBit == 0x04) imgBit = WAP_LIGHTGRAY;
                            else if(imgBit == 0x08) imgBit = WAP_GRAY;
                            else if(imgBit == 0x0c) imgBit = WAP_BLACK;
                        }
                    }
                    else					     // inbuffer[] ���� ������ 2bit ó��
                    {
                        imgBit &= 0x03;

                        if(pImage->Header.fPalette)
                        {
                            if(imgBit == 0x00) imgBit = pImage->pbPalette[0];
                            else if(imgBit == 0x01) imgBit = pImage->pbPalette[1];
                            else if(imgBit == 0x02) imgBit = pImage->pbPalette[2];
                            else if(imgBit == 0x03) imgBit = pImage->pbPalette[3];
                        }
                        else
                        {
                            if(imgBit == 0x00) imgBit = WAP_WHITE;
                            else if(imgBit == 0x01) imgBit = WAP_LIGHTGRAY;
                            else if(imgBit == 0x02) imgBit = WAP_GRAY;
                            else if(imgBit == 0x03) imgBit = WAP_BLACK;
                        
                        }
                    
                    }
        
                    *(LCDWordAniBuffer + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;
				}
            }
        }

    }
*/
}

void WAP_4GrayBMPTo2BITData(char* pDestBuf, char* imageData, int width, int height, int bwidth)
{
	int nX, nY, nbit; 
	byte imgBit = 0x00;

	char * pt = imageData;

	BYTE inbuffer[120];
	memset(inbuffer, 0x00, 120);//init

	// ------------------------------------------------
	// ���� �̹����� �׸���. 
	// height�� �ϳ��� �÷����鼭 �׸���. 
	//for(nY = height-1 ; nY >= 0 ; nY--)
	for(nY = 0 ; nY < height ; nY++)
	{				
		// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 

		memset(inbuffer, 0x00, 120);//init
		memcpy(inbuffer, pt, (width+3)/4*2);//(width+7)/8*4);
		pt += (width+3)/4*2;//(width+7)/8*4;	//seek

		for (nX = 0 ; nX <= width ; nX++)
		{
            int xIndex;
		
			imgBit = inbuffer[nX/2];
			if (nX%2 == 0)			// inbuffer[] ���� ���� bit ó�� 
			{
				imgBit &= 0xf0;

                if (imgBit <= 0x60) imgBit = 0x03;//0x00;//WAP_BLACK;
				else if (imgBit == 0x70) imgBit = 0x01;//WAP_LIGHTGRAY;
				else if (imgBit == 0x80) imgBit = 0x02;//WAP_GRAY;
				else imgBit = 0x00;//WAP_WHITE;
			}						
			else					// inbuffer[] ���� ���� bit ó�� 
			{
				imgBit &= 0x0f;

                if (imgBit <= 0x06) imgBit = 0x03;//WAP_BLACK;
				else if (imgBit == 0x07) imgBit = 0x01;//WAP_LIGHTGRAY;
				else if (imgBit == 0x08) imgBit = 0x02;//WAP_GRAY;
				else imgBit = 0x00;//WAP_WHITE;
			}

			xIndex = (int)(nX/4);
			nbit = nX%4;

			gen_setbit(*(pDestBuf + (nY*bwidth) + xIndex), nbit, imgBit);

		} // end of for (nX = 0 ; nX <= Iwidth ; nX = nX+2)
	}// end of for - Image setting 
}

void WAP_Display_Image_BMP(char* pDestBuf, char* imageData, int width, int height,
						   int xPos, int yPos, int mode, int align,
						   int nBitCount)
//	�̹��� �����Ͱ� ����� ����, ���� �̹��� ������, ����, ����, ������ġ ����, ������ġ ����, �̹��� ����, �̹��� Į��� 
{
    BITMAP Bitmap;
    int nImageSize=0;

    BYTE    *pTempData = NULL;
    int     bwidth=0, i, j;
    int     imageWidth=0, imageHeight;    //�������� �̹��� width ����Ʈ��, �̹��� height

	if (xPos % 2 == 1)			// �̹��� ����� ¦���� �������� �ʰ� Ȧ���� ��츦 ���� 
		xPos -= 1;

    if(xPos > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	if(yPos > WAP_DOC_HEIGHT) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ����

    //--------------------------------------------------------------------
    // �̹��� width, height�� LCD size���� ū ���, LCD�� �°� �̹��� ����
    //
    // 1. ���� �̹��� �� ������ byte�� ��� - �� ����ŭ �̹��� �����͸� �̵�
    if(nBitCount == 1)         bwidth = (width+31)/32*4;
    else if(nBitCount == 4)    bwidth = (width+7)/8*4;
    else if(nBitCount == 8)    bwidth = width;
    imageHeight = height;

    // 2. �̹��� width, height�� ����
    if(width > WAP_LCD_WIDTH) width = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
    if(height > WAP_DOC_HEIGHT) imageHeight = WAP_DOC_HEIGHT;  //�̹��� height�� LCD height���� ū ���

    // 3. ������ �̹��� �� ������ byte�� ��� - �� ����ŭ �̹��� copy
    if(nBitCount == 1)         imageWidth = (width+7)/8;//(width+31)/32*4;
    else if(nBitCount == 4)    imageWidth = ((width+3)/4)*2;//(width+7)/8*4;
    else if(nBitCount == 8)    imageWidth = width;

    if(nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = imageWidth*8;//((width+7)/8)*8;//width + width%8;
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = width;
        Bitmap.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR; 

	    if(yPos < 0)
	    {
            Bitmap.Header.yStart = yPos * (-1);		    
		    yPos = 0;
	    }
        else
        {
            Bitmap.Header.yStart = 0;
        }

        nImageSize = (Bitmap.Header.Size.xWidth/8) * Bitmap.Header.Size.yHeight;
        Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
        memset((char *)Bitmap.pbData, 0x00, nImageSize);

        //4. Data�� ������ �ٲ���..
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy((BYTE*)Bitmap.pbData + (imageWidth * j) , imageData + (bwidth * i), imageWidth);
        }

        EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);


   }
    else if(nBitCount == WAP_4GRAY_BITS_PER_PIXEL * 2)  //4 Gray
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = (imageWidth/2)*4;//((width+3)/4)*4;//width + (width%4);
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = width;
        Bitmap.Header.nBitsPerPixel = WAP_4GRAY_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR;

	    if(yPos < 0)
	    {
            Bitmap.Header.yStart = yPos * (-1);		    
		    yPos = 0;
	    }
        else
        {
            Bitmap.Header.yStart = 0;
        }

        if(Bitmap.pbData)
        {
		    wip_free((void*)Bitmap.pbData);
		    Bitmap.pbData =NULL;
        }

        nImageSize = (Bitmap.Header.Size.xWidth/4) * Bitmap.Header.Size.yHeight;
        Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
        memset((BYTE *)Bitmap.pbData, 0x00, nImageSize);

        //4. Data�� ������ �ٲ���
        pTempData = (BYTE*)wip_malloc(imageWidth * Bitmap.Header.Size.yHeight);
        memset((BYTE *)pTempData, 0x00, imageWidth * Bitmap.Header.Size.yHeight);
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy(pTempData + (imageWidth * j) , imageData + (bwidth * i), imageWidth);
        }

        //5. 4bit�� 4Gray BMP�� 2bit���� �ٲ���
        WAP_4GrayBMPTo2BITData((char*)Bitmap.pbData, (char*)pTempData, width, height, Bitmap.Header.Size.xWidth/4);

        //Bitmap.pbData = (BYTE *)pImgData;
        EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);

    }
    else if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = width;//MAX_SKY_IMAGE_WIDTH;//120
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = width;//MAX_SKY_IMAGE_WIDTH;//width;
        Bitmap.Header.nBitsPerPixel = WAP_256COLOR_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR;

	    if(yPos < 0)
	    {
            Bitmap.Header.yStart = yPos * (-1);		    
		    yPos = 0;
	    }
        else
        {
            Bitmap.Header.yStart = 0;
        }

        nImageSize = Bitmap.Header.Size.xWidth * Bitmap.Header.Size.yHeight;
        Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
        memset((char *)Bitmap.pbData, 0x00, nImageSize);

        //4. Data�� ������ �ٲ���
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy((BYTE *)Bitmap.pbData + (imageWidth * j) , imageData + (bwidth * i), imageWidth);
        }

        EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);
    }


/*
    BITMAP Bitmap;
    int nImageSize = 0, bwidth = 0;

    if(nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = ((width+7)/8)*8;//width + width%8;
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = width;
        Bitmap.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR; 

        bwidth = (Bitmap.Header.Size.xWidth/8);
    }
    else if(nBitCount == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = ((width+3)/4)*4;//width + (width%4);
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = width;
        Bitmap.Header.nBitsPerPixel = WAP_4GRAY_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR;

        bwidth = (Bitmap.Header.Size.xWidth/4);       
    }
    else if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = MAX_SKY_IMAGE_WIDTH;//120
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = MAX_SKY_IMAGE_WIDTH;//width;
        Bitmap.Header.nBitsPerPixel = WAP_256COLOR_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR;
    
        bwidth = Bitmap.Header.Size.xWidth;
    }

	if(yPos < 0)
	{
        Bitmap.Header.yStart = yPos * (-1);		    
		yPos = 0;
	}
    else
    {
        Bitmap.Header.yStart = 0;
    }

    nImageSize = bwidth * Bitmap.Header.Size.yHeight;

    if(Bitmap.pbData)
    {
		wip_free((void*)Bitmap.pbData);
		Bitmap.pbData =NULL;
    }
    Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);

    memcpy((VOID*)Image.pbData, (char *)pWBMPData+data_index, nImageSize);  //remove header
    Bitmap.pbData = (BYTE *)pImgData;
    EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);
*/
    /*

	int Iwidth, Iheight, nX, nY, nbit; 
	byte imgBit = 0x00;

    BYTE bTextColor = EQS_GetTextColor();
    BYTE bTextBackColor = EQS_GetBackColor();//EQS_GetTextBackColor();


	char * pt = imageData;

	BYTE inbuffer[120];
	memset(inbuffer, 0x00, 120);//init


	if (pos_x % 2 == 1)			// �̹��� ����� ¦���� �������� �ʰ� Ȧ���� ��츦 ���� 
		pos_x -= 1;

	//--------------------------------------------
	// ȭ�鿡 ������ �ʴ� ������ ����Ÿ�� �����͸� ������Ű��,
	// ���Ŀ� �̹��� ���÷��̸� �����ϵ��� �Ѵ�
	if(pos_y < 0)
	{
		height = height - (pos_y * (-1));
		
		pos_y = 0;
	}
    


	Iwidth = pos_x + width - 1;				// �̹����� ���� �Ѱ��� 
	Iheight = pos_y + height - 1;			// �̹����� ���� �Ѱ��� 

    if (HowManyColor == 2 || HowManyColor == 5 || HowManyColor == 9) //������ ���� 120*120���� �����ϱ� ����..
    {
	    if(pos_x > WAP_LCD_TOTAL_ADDRESS_X) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_LCD_TOTAL_ADDRESS_Y) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_TOTAL_ADDRESS_X) Iwidth = WAP_LCD_TOTAL_ADDRESS_X;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_LCD_TOTAL_ADDRESS_Y) Iheight = WAP_LCD_TOTAL_ADDRESS_Y;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_TOTAL_ADDRESS_X) Iwidth--;
	    if (Iheight == WAP_LCD_TOTAL_ADDRESS_Y) Iheight--;
    }
    else if(HowManyColor == 1 || HowManyColor == 4 || HowManyColor == 8)
    {
	    if(pos_x > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_DOC_HEIGHT) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_WIDTH) Iwidth = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_DOC_HEIGHT) Iheight = WAP_DOC_HEIGHT;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_WIDTH) Iwidth--;
	    if (Iheight == WAP_DOC_HEIGHT) Iheight--;
    }

	// ------------------------------------------------
	// ���� �̹����� �׸���. 
	// height�� �ϳ��� �÷����鼭 �׸���. 
	for(nY = Iheight ; nY >= pos_y ; nY--)
	{				
		if (HowManyColor == 1)				// ��� ��Ʈ�� 
		{
			// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
			memcpy(inbuffer, pt, (width+31)/32*4);
			pt += (width+31)/32*4;	//seek

			if ((pos_y<=nY) && (nY<=(WAP_LCD_HEIGHT-1)))
			{
				// �� ������ width ��Ʈ�� �׸���. 
				for(nX=0 ; nX <= Iwidth ; nX++)
				{
					// x �� boundary check 
					if((nX/8) > 14) break;
				

					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) // && (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
					{
						// ���ۿ� �̹��� ������ �����Ѵ�. 
						imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);
						if (imgBit == 0x01) imgBit = bTextColor;//WAP_BLACK;
						else imgBit = bTextBackColor;//WAP_WHITE;
					
		                if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
			                *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;
					}	// end of if((pos_x<=nX && nX<=WAP_LCD_WIDTH) && (pos_y<=nY && nY<=WAP_LCD_HEIGHT))

				}	// end of for - Image width 

			}	// end of if

		}
		else if (HowManyColor == 2)				// Picture Mate - Black & White �̹��� ������ ���� ��ƾ 
		{
			// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
			memcpy(inbuffer, pt, (width+31)/32*4);
			pt += (width+31)/32*4;	//seek

			if (pos_y <= nY && nY <= (WAP_LCD_HEIGHT-1))
			{
				// �� ������ width ��Ʈ�� �׸���. 
				for(nX=0 ; nX <= Iwidth ; nX++)
				{
					// x �� boundary check 
					if((nX/8) > 14) break;
				
					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) //&& (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))  // WIDTH 120, HEIGHT 120
					{
						// ���ۿ� �̹��� ������ �����Ѵ�. 
						imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);

                        //B/W�� set
                        if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X) 
                            color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/8)) + (nX/8)), (nX%8), imgBit);
					}// end of if((pos_x<=nX && nX<=WAP_LCD_WIDTH) && (pos_y<=nY && nY<=WAP_LCD_HEIGHT))

				}// end of for - Image width 
			
			}// end of if

		}
		else if (HowManyColor == 4 || HowManyColor == 5)	// 16Color Bitmap -> Phone������ 4Gray ��°� ���� ��ƾ 
		{
			// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 

			memset(inbuffer, 0x00, 120);//init
			memcpy(inbuffer, pt, (width+7)/8*4);
			pt += (width+7)/8*4;	//seek

			if(pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)) //&& ((pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)))
			{
				for (nX = 0 ; nX <= Iwidth ; nX++)
				{
					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) // && (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
					{

                        int xIndex;
					
						imgBit = inbuffer[(nX-pos_x)/2];

                        if (HowManyColor == 4)
                        {
						    if (nX%2 == 0)			// inbuffer[] ���� ���� bit ó�� 
						    {
							    imgBit &= 0xf0;

                                if (imgBit <= 0x60) imgBit = bTextColor;//WAP_BLACK;
							    else if (imgBit == 0x70) imgBit = WAP_LIGHTGRAY;
							    else if (imgBit == 0x80) imgBit = WAP_GRAY;
							    else imgBit = bTextBackColor;//WAP_WHITE;

						    }						
						    else					// inbuffer[] ���� ���� bit ó�� 
						    {
							    imgBit &= 0x0f;

                                if (imgBit <= 0x06) imgBit = bTextColor;//WAP_BLACK;
							    else if (imgBit == 0x07) imgBit = WAP_LIGHTGRAY;
							    else if (imgBit == 0x08) imgBit = WAP_GRAY;
							    else imgBit = bTextBackColor;//WAP_WHITE;

						    }

						if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
                            *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;					
                        }
                        else if (HowManyColor == 5)
                        {
						    if (nX%2 == 0)			// inbuffer[] ���� ���� bit ó�� 
						    {
							    imgBit &= 0xf0;

                                if (imgBit <= 0x60) imgBit = 0x03;//0x00;//WAP_BLACK;
							    else if (imgBit == 0x70) imgBit = 0x01;//WAP_LIGHTGRAY;
							    else if (imgBit == 0x80) imgBit = 0x02;//WAP_GRAY;
							    else imgBit = 0x00;//WAP_WHITE;
						    }						
						    else					// inbuffer[] ���� ���� bit ó�� 
						    {
							    imgBit &= 0x0f;

                                if (imgBit <= 0x06) imgBit = 0x03;//WAP_BLACK;
							    else if (imgBit == 0x07) imgBit = 0x01;//WAP_LIGHTGRAY;
							    else if (imgBit == 0x08) imgBit = 0x02;//WAP_GRAY;
							    else imgBit = 0x00;//WAP_WHITE;
						    }

						    xIndex = (int)(nX/4);
						    nbit = nX%4;

							gen_setbit(*(pDestBuf + (nY*30) + xIndex), nbit, imgBit);
                        }
    				}

				} // end of for (nX = 0 ; nX <= Iwidth ; nX = nX+2)

			}

		}

		else if (HowManyColor == 8 || HowManyColor == 9)				//  256Color Bitmap 
		{
			// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�.
			memset(inbuffer, 0x00, 120);//init			
			if(width > 120)
			{
				memcpy(inbuffer, pt, 120);//width);
				pt = pt + width; // + 2;					//seek
			}
			else
			{
				memcpy(inbuffer, pt, width);//width);
				pt = pt + width; // + 2;					//seek
			}

			if(HowManyColor == 8)
			{
				if ((pos_y<=nY) && (nY<=(WAP_LCD_HEIGHT-1)))
				{
					// �� ������ width ��Ʈ�� �׸���. 
					for(nX=0 ; nX <= Iwidth ; nX++)
					{
						// x �� boundary check 
						if(nX > 120) break;

						if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) // && (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
						{
							imgBit = inbuffer[nX-pos_x];    
			
							if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
								*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;

						}	// end of if((pos_x<=nX && nX<=WAP_LCD_WIDTH) && (pos_y<=nY && nY<=WAP_LCD_HEIGHT))

					}	// end of for - Image width 

				}	// end of if
			}
			else if(HowManyColor == 9)
			{
				if ((pos_y<=nY) && (nY<=(WAP_LCD_TOTAL_ADDRESS_Y-1)))
				{
					// �� ������ width ��Ʈ�� �׸���. 
					for(nX=0 ; nX <= Iwidth ; nX++)
					{
						// x �� boundary check 
						if(nX > 120) break;

						if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) // && (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
						{
							imgBit = inbuffer[nX-pos_x];    
			
							if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
								*(pDestBuf + (nY * WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;

						}	// end of if((pos_x<=nX && nX<=WAP_LCD_WIDTH) && (pos_y<=nY && nY<=WAP_LCD_HEIGHT))

					}	// end of for - Image width 

				}	// end of if
			}
		}
		else									// 24bit Bitmap
		{
			// SKT�� �̹��� Ÿ�� ���� �� make
		}

	}// end of for - Image setting 
    */
}


void WAP_Display_Image_BMP_Save(char* pDestBuf, char* imageData, int width, int height,
						   int pos_x, int pos_y, int mode, int align,
						   int HowManyColor)
//	�̹��� �����Ͱ� ����� ����, ���� �̹��� ������, ����, ����, ������ġ ����, ������ġ ����, �̹��� ����, �̹��� Į��� 
{
	int Iwidth, Iheight, nX, nY, nbit; 
	byte imgBit = 0x00;

    BYTE bTextColor = EQS_GetTextColor();
    BYTE bTextBackColor = EQS_GetBackColor();//EQS_GetTextBackColor();

#if !((defined WAP_B1) || (defined WAP_B2))
	int bwidth = (width+7)/8;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	int bheight = (height+7)/8;

	char * pt = imageData;

#if !((defined WAP_B1) || (defined WAP_B2))
	char *inbuffer = (char *) wip_malloc(bwidth);
#else   //#ifndef  WAP_B1
	BYTE inbuffer[120];
#endif  //#ifndef  WAP_B1
	memset(inbuffer, 0x00, 120);//init


	if (pos_x % 2 == 1)			// �̹��� ����� ¦���� �������� �ʰ� Ȧ���� ��츦 ���� 
		pos_x -= 1;

	//--------------------------------------------
	// ȭ�鿡 ������ �ʴ� ������ ����Ÿ�� �����͸� ������Ű��,
	// ���Ŀ� �̹��� ���÷��̸� �����ϵ��� �Ѵ�
	if(pos_y < 0)
	{
#if !((defined WAP_B1) || (defined WAP_B2))
		pt = pt + (bwidth * pos_y * (-1));
#endif  //#ifdef  WAP_B1
		height = height - (pos_y * (-1));
		
		pos_y = 0;
		bheight = (height+7)/8;
	}
    


	Iwidth = pos_x + width - 1;				// �̹����� ���� �Ѱ��� 
	Iheight = pos_y + height - 1;			// �̹����� ���� �Ѱ��� 

    if (HowManyColor == 2 || HowManyColor == 5 || HowManyColor == 9) //������ ���� 120*120���� �����ϱ� ����..
    {
	    if(pos_x > WAP_LCD_TOTAL_ADDRESS_X) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_LCD_TOTAL_ADDRESS_Y) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_TOTAL_ADDRESS_X) Iwidth = WAP_LCD_TOTAL_ADDRESS_X;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_LCD_TOTAL_ADDRESS_Y) Iheight = WAP_LCD_TOTAL_ADDRESS_Y;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_TOTAL_ADDRESS_X) Iwidth--;
	    if (Iheight == WAP_LCD_TOTAL_ADDRESS_Y) Iheight--;
    }
    else if(HowManyColor == 1 || HowManyColor == 4 || HowManyColor == 8)
    {
	    if(pos_x > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_WIDTH) Iwidth = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) Iheight = WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_WIDTH) Iwidth--;
	    if (Iheight == WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) Iheight--;
    }

	// ------------------------------------------------
	// ���� �̹����� �׸���. 
	// height�� �ϳ��� �÷����鼭 �׸���. 
	for(nY = Iheight ; nY >= pos_y ; nY--)
	{				
		if (HowManyColor == 1)				// ��� ��Ʈ�� 
		{
			// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
			memcpy(inbuffer, pt, (width+31)/32*4);
			pt += (width+31)/32*4;	//seek

			if ((pos_y<=nY) && (nY<=(WAP_LCD_HEIGHT-1)))
			{
				// �� ������ width ��Ʈ�� �׸���. 
				for(nX=0 ; nX <= Iwidth ; nX++)
				{
					// x �� boundary check 
					if((nX/8) > 14) break;
				

					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) // && (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
					{
#if !((defined WAP_B1) || (defined WAP_B2))
                        int xIndex;

						// ���ۿ� �̹��� ������ �����Ѵ�. 
						imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);
						if (imgBit == 0x01) imgBit = WAP_BLACK;
						else imgBit = WAP_WHITE;
						
						xIndex = (int)(nX/8*2);
						nbit = nX%8;
						if (nbit > 3)
						{
							xIndex++;	nbit = nbit - 4;
						}
	//					gen_setbit(LCDWordAniBuffer[nY][xIndex], nbit, imgBit);
						if(0<=nY && nY<80 && 0<=xIndex && xIndex<30)
//							gen_setbit(pDestBuf[(nY*WAP_LCD_TOTAL_ADDRESS_X) + xIndex], nbit, imgBit);
							gen_setbit(*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + xIndex), nbit, imgBit);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
						// ���ۿ� �̹��� ������ �����Ѵ�. 
						imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);
						if (imgBit == 0x01) imgBit = bTextColor;//WAP_BLACK;
						else imgBit = bTextBackColor;//WAP_WHITE;
					
		                if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
			                *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
					}	// end of if((pos_x<=nX && nX<=WAP_LCD_WIDTH) && (pos_y<=nY && nY<=WAP_LCD_HEIGHT))

				}	// end of for - Image width 

			}	// end of if

		}
		else if (HowManyColor == 2)				// Picture Mate - Black & White �̹��� ������ ���� ��ƾ 
		{
			// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
			memcpy(inbuffer, pt, (width+31)/32*4);
			pt += (width+31)/32*4;	//seek

			if (pos_y <= nY && nY <= (WAP_LCD_HEIGHT-1))
			{
				// �� ������ width ��Ʈ�� �׸���. 
				for(nX=0 ; nX <= Iwidth ; nX++)
				{
					// x �� boundary check 
					if((nX/8) > 14) break;
				
					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) //&& (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))  // WIDTH 120, HEIGHT 120
					{
						// ���ۿ� �̹��� ������ �����Ѵ�. 
						imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);

#if (defined WAP_B1) || (defined WAP_B2)
/*                        //color�� set
						if (imgBit == 0x01) imgBit = WAP_WHITE;//WAP_BLACK;
						else imgBit = WAP_BLACK;//WAP_WHITE;
						
		                if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
			                *(pDestBuf + (nY*WAP_LCD_TOTAL_ADDRESS_X) + nX) |= imgBit;
*/
                        //B/W�� set
                        if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X) 
                            color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/8)) + (nX/8)), (nX%8), imgBit);
#else   //#if (defined WAP_B1) || (defined WAP_B2)
                        if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X) 
							set_picture_mate_bit(*(pDestBuf + (nY*(WAP_LCD_TOTAL_ADDRESS_X/8)) + (nX/8)), ChangeBitPosition(nX%8), imgBit);
#endif  //#if (defined WAP_B1) || (defined WAP_B2)

					}// end of if((pos_x<=nX && nX<=WAP_LCD_WIDTH) && (pos_y<=nY && nY<=WAP_LCD_HEIGHT))

				}// end of for - Image width 
			
			}// end of if

		}
/*
		else if (HowManyColor == 3)				// Picture Mate - 4 Gray �̹��� ������ ���� ��ƾ  
		{
			// SKT�� �̹��� Ÿ�� ���� �� make
			memset(inbuffer, 0x00, 120);
			memcpy(inbuffer, pt, width);
			pt = pt + width; // + 2;					//seek
			
			if (pos_y <= nY && nY <= (WAP_LCD_HEIGHT-1))
			{
				for (nX = 0 ; nX <= Iwidth ; nX++)
				{
					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1))  //&& (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
					{
						int xIndex;
						imgBit = inbuffer[nX-pos_x];
						
						if (imgBit == 0xff) imgBit = WAP_WHITE;
						else if (imgBit == 0xC0) imgBit = WAP_WHITEGRAY;
						else if (imgBit == 0x80) imgBit = WAP_BLACKGRAY;
						else imgBit = WAP_BLACK;

						xIndex = (int)(nX/8*2);
						nbit = nX%8;
						if (nbit > 3)
						{
							xIndex++; nbit = nbit - 4;
						}
						gen_setbit(pDestBuf[(nY*30) + xIndex], nbit, imgBit);					
					}

				} // end of for (nX = 0 ; nX <= Iwidth ; nX = nX+2)

			}

		}
*/

		else if (HowManyColor == 4 || HowManyColor == 5)	// 16Color Bitmap -> Phone������ 4Gray ��°� ���� ��ƾ 
		{
			// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 

			memset(inbuffer, 0x00, 120);//init
			memcpy(inbuffer, pt, (width+7)/8*4);
			pt += (width+7)/8*4;	//seek

			if(pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)) //&& ((pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)))
			{
				for (nX = 0 ; nX <= Iwidth ; nX++)
				{
					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) // && (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
					{

                        int xIndex;
					
						imgBit = inbuffer[(nX-pos_x)/2];
#if !((defined WAP_B1) || (defined WAP_B2))
						if (nX%2 == 0)			// inbuffer[] ���� ���� bit ó�� 
						{
							imgBit &= 0xf0;
							if (imgBit <= 0x60) imgBit = WAP_BLACK;
							else if (imgBit == 0x70) imgBit = WAP_WHITEGRAY;
							else if (imgBit == 0x80) imgBit = WAP_BLACKGRAY;
							else imgBit = WAP_WHITE;
						}						
						else					// inbuffer[] ���� ���� bit ó�� 
						{
							imgBit &= 0x0f;
							if (imgBit <= 0x06) imgBit = WAP_BLACK;
							else if (imgBit == 0x07) imgBit = WAP_WHITEGRAY;
							else if (imgBit == 0x08) imgBit = WAP_BLACKGRAY;
							else imgBit = WAP_WHITE;
						}

						xIndex = (int)(nX/4);
						nbit = nX%4;
						//if(0<=nY && nY<80 && 0<=xIndex && xIndex<30) 
//						{
							if (HowManyColor == 4)
//								gen_setbit(pDestBuf[(nY*WAP_LCD_TOTAL_ADDRESS_X) + xIndex], nbit, imgBit);					
								gen_setbit(*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + xIndex), nbit, imgBit);
							else if (HowManyColor == 5)
//								gen_setbit(pDestBuf[(nY*30) + xIndex], nbit, imgBit);
								gen_setbit(*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*30) + xIndex), nbit, imgBit);
//						}
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
                            if (HowManyColor == 4)
                            {
						        if (nX%2 == 0)			// inbuffer[] ���� ���� bit ó�� 
						        {
							        imgBit &= 0xf0;

                                    if (imgBit <= 0x60) imgBit = bTextColor;//WAP_BLACK;
							        else if (imgBit == 0x70) imgBit = WAP_LIGHTGRAY;
							        else if (imgBit == 0x80) imgBit = WAP_GRAY;
							        else imgBit = bTextBackColor;//WAP_WHITE;

						        }						
						        else					// inbuffer[] ���� ���� bit ó�� 
						        {
							        imgBit &= 0x0f;

                                    if (imgBit <= 0x06) imgBit = bTextColor;//WAP_BLACK;
							        else if (imgBit == 0x07) imgBit = WAP_LIGHTGRAY;
							        else if (imgBit == 0x08) imgBit = WAP_GRAY;
							        else imgBit = bTextBackColor;//WAP_WHITE;

						        }

							if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
                                *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;					
                            }
                            else if (HowManyColor == 5)
                            {
						        if (nX%2 == 0)			// inbuffer[] ���� ���� bit ó�� 
						        {
							        imgBit &= 0xf0;

                                    if (imgBit <= 0x60) imgBit = 0x03;//0x00;//WAP_BLACK;
							        else if (imgBit == 0x70) imgBit = 0x01;//WAP_LIGHTGRAY;
							        else if (imgBit == 0x80) imgBit = 0x02;//WAP_GRAY;
							        else imgBit = 0x00;//WAP_WHITE;
						        }						
						        else					// inbuffer[] ���� ���� bit ó�� 
						        {
							        imgBit &= 0x0f;

                                    if (imgBit <= 0x06) imgBit = 0x03;//WAP_BLACK;
							        else if (imgBit == 0x07) imgBit = 0x01;//WAP_LIGHTGRAY;
							        else if (imgBit == 0x08) imgBit = 0x02;//WAP_GRAY;
							        else imgBit = 0x00;//WAP_WHITE;
						        }

						        xIndex = (int)(nX/4);
						        nbit = nX%4;

								gen_setbit(*(pDestBuf + (nY*30) + xIndex), nbit, imgBit);
                            }
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
/*						if (nX%2 == 0)			// inbuffer[] ���� ���� bit ó�� 
						{
							imgBit &= 0xf0;
#if !((defined WAP_B1) || (defined WAP_B2))
							if (imgBit <= 0x60) imgBit = WAP_BLACK;
							else if (imgBit == 0x70) imgBit = WAP_WHITEGRAY;
							else if (imgBit == 0x80) imgBit = WAP_BLACKGRAY;
							else imgBit = WAP_WHITE;
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
							if (imgBit <= 0x60) imgBit = WAP_WHITE;
							else if (imgBit == 0x70) imgBit = WAP_GRAY;
							else if (imgBit == 0x80) imgBit = WAP_LIGHTGRAY;
							else imgBit = WAP_BLACK;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
						}						
						else					// inbuffer[] ���� ���� bit ó�� 
						{
							imgBit &= 0x0f;
#if !((defined WAP_B1) || (defined WAP_B2))
							if (imgBit <= 0x06) imgBit = WAP_BLACK;
							else if (imgBit == 0x07) imgBit = WAP_WHITEGRAY;
							else if (imgBit == 0x08) imgBit = WAP_BLACKGRAY;
							else imgBit = WAP_WHITE;
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
							if (imgBit <= 0x06) imgBit = WAP_WHITE;
							else if (imgBit == 0x07) imgBit = WAP_GRAY;
							else if (imgBit == 0x08) imgBit = WAP_LIGHTGRAY;
							else imgBit = WAP_BLACK;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
						}

						xIndex = (int)(nX/4);
						nbit = nX%4;
#if !((defined WAP_B1) || (defined WAP_B2))
						//if(0<=nY && nY<80 && 0<=xIndex && xIndex<30) 
//						{
							if (HowManyColor == 4)
//								gen_setbit(pDestBuf[(nY*WAP_LCD_TOTAL_ADDRESS_X) + xIndex], nbit, imgBit);					
								gen_setbit(*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + xIndex), nbit, imgBit);
							else if (HowManyColor == 5)
//								gen_setbit(pDestBuf[(nY*30) + xIndex], nbit, imgBit);
								gen_setbit(*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*30) + xIndex), nbit, imgBit);
//						}
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
                            if (HowManyColor == 4)
                                *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) |= imgBit;					
                            else if (HowManyColor == 5)
								gen_setbit(*(pDestBuf + (nY*30) + xIndex), nbit, imgBit);
								//gen_setbit(*(pDestBuf + (nY*30) + xIndex), (3-nbit), imgBit); //bit ordering�� �ٸ��ٸ�..�̰ɷ�
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
*/					}

				} // end of for (nX = 0 ; nX <= Iwidth ; nX = nX+2)

			}

		}

		else if (HowManyColor == 8 || HowManyColor == 9)				//  256Color Bitmap 
		{
			// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�.
			memset(inbuffer, 0x00, 120);//init			
			if(width > 120)
			{
				memcpy(inbuffer, pt, 120);//width);
				pt = pt + width; // + 2;					//seek
			}
			else
			{
				memcpy(inbuffer, pt, width);//width);
				pt = pt + width; // + 2;					//seek
			}

			if(HowManyColor == 8)
			{
				if ((pos_y<=nY) && (nY<=(WAP_LCD_HEIGHT-1)))
				{
					// �� ������ width ��Ʈ�� �׸���. 
					for(nX=0 ; nX <= Iwidth ; nX++)
					{
						// x �� boundary check 
						if(nX > 120) break;

						if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) // && (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
						{
							imgBit = inbuffer[nX-pos_x];    
			
							if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
								*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;

						}	// end of if((pos_x<=nX && nX<=WAP_LCD_WIDTH) && (pos_y<=nY && nY<=WAP_LCD_HEIGHT))

					}	// end of for - Image width 

				}	// end of if
			}
			else if(HowManyColor == 9)
			{
				if ((pos_y<=nY) && (nY<=(WAP_LCD_TOTAL_ADDRESS_Y-1)))
				{
					// �� ������ width ��Ʈ�� �׸���. 
					for(nX=0 ; nX <= Iwidth ; nX++)
					{
						// x �� boundary check 
						if(nX > 120) break;

						if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) // && (pos_y<=nY && nY<=(WAP_LCD_HEIGHT-1)))
						{
							imgBit = inbuffer[nX-pos_x];    
			
							if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
								*(pDestBuf + (nY * WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;

						}	// end of if((pos_x<=nX && nX<=WAP_LCD_WIDTH) && (pos_y<=nY && nY<=WAP_LCD_HEIGHT))

					}	// end of for - Image width 

				}	// end of if
			}
		}
		else									// 24bit Bitmap
		{
			// SKT�� �̹��� Ÿ�� ���� �� make
		}

	}// end of for - Image setting 
#ifndef  WAP_B1	
	if(inbuffer) wip_free((void *)inbuffer);
#endif  //#ifndef  WAP_B1
}	



/////////////////////////////////////////////////////////////////////////////////
//
//  SKT�� �̹��� ��� ���� ���� �� ���� ��...... �ϴ��� ��� WBMP ��� 
//
/////////////////////////////////////////////////////////////////////////////////
void WAP_Display_Image_WBMP(char* pDestBuf, char* pImgData,
							int width, int height, int xPos, int yPos,
							int mode, int align)
{
    BITMAP Bitmap;

    //Bitmap Header
    memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
    Bitmap.Header.Size.xWidth = ((width+7)/8)*8;//width + width%8;
    Bitmap.Header.Size.yHeight = height;
    Bitmap.Header.xWidth = width;
    Bitmap.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
    Bitmap.Header.fPalette = FALSE;
    Bitmap.Header.TransparentColor = MAX_COLOR; 

    if(yPos < 0)
    {
        Bitmap.Header.yStart = yPos * (-1);		    
	    yPos = 0;
    }
    else
    {
        Bitmap.Header.yStart = 0;
    }

    //copy data
    Bitmap.pbData = (BYTE *)pImgData;
    EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);

    /*
	BYTE imgBit;
	int Iwidth, Iheight, nX, nY; 

	int bwidth = (width+7)/8;
	int bheight = (height+7)/8;

	char *pt = pImgData;

    BYTE bTextColor = EQS_GetTextColor();
    BYTE bTextBackColor = EQS_GetBackColor();//EQS_GetTextBackColor();	

	BYTE inbuffer[120]; //char *inbuffer = (char *) wip_malloc(bwidth);
	memset(inbuffer, 0x00, 120);//init

	//--------------------------------------------
	// ȭ�鿡 ������ �ʴ� ������ ����Ÿ�� �����͸� ������Ű��,
	// ���Ŀ� �̹��� ���÷��̸� �����ϵ��� �Ѵ�
	if(pos_y < 0)
	{
		pt = pt + (bwidth * pos_y * (-1));
		height = height - (pos_y * (-1));
		
		pos_y = 0;
		bheight = (height+7)/8;
	}

	//------------------------------------------------
	Iwidth = pos_x + width - 1;				// �̹����� ���� �Ѱ��� 
	Iheight = pos_y + height - 1;			// �̹����� ���� �Ѱ��� 

    if (align == WAP_DRAW_PICTURE_MATE) //������ ���� 120*120���� �����ϱ� ����..
    {
	    if(pos_x > WAP_LCD_TOTAL_ADDRESS_X) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_LCD_TOTAL_ADDRESS_Y) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_TOTAL_ADDRESS_X) Iwidth = WAP_LCD_TOTAL_ADDRESS_X;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_LCD_TOTAL_ADDRESS_Y) Iheight = WAP_LCD_TOTAL_ADDRESS_Y;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_TOTAL_ADDRESS_X) Iwidth--;
	    if (Iheight == WAP_LCD_TOTAL_ADDRESS_Y) Iheight--;
    }
    else
    {
	    if(pos_x > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_DOC_HEIGHT) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_WIDTH) Iwidth = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_DOC_HEIGHT) Iheight = WAP_DOC_HEIGHT;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_WIDTH) Iwidth--;
	    if (Iheight == WAP_DOC_HEIGHT) Iheight--;
    }
	
	//------------------------------------------------
	// ���� �̹����� �׸���. 
	// height�� �ϳ��� �÷����鼭 �׸���. 
//	for(nY=0; nY<=Iheight; nY++)
	for(nY = pos_y ; nY <= Iheight ; nY++)
	{						
		// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
		memcpy(inbuffer, pt, bwidth);
		pt += bwidth;	//seek

		if ((pos_y <= nY) && (nY <= (WAP_LCD_HEIGHT-1)))
		{
			// �� ������ width ��Ʈ�� �׸���. 
			for(nX=0; nX<=Iwidth; nX++)
			{
				// x �� boundary check 
				if((nX/8) > 14) break;
			
				if((pos_x<=nX) && (nX<=(WAP_LCD_WIDTH-1))) //&& (pos_y<=nY && nY<=WAP_LCD_HEIGHT))
				{
					if(mode == WAP_DRAW_NORMAL)
						imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);
					else
						imgBit = gen_getbit(ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);

					if (align == WAP_DRAW_PICTURE_MATE)	// picture mate �� ��� 
					{
                        if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X) 
                            color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/8)) + (nX/8)), (nX%8), imgBit);
                    }
					else // picture mate�� �ƴ� ��� 
					{
						if (imgBit == 0x01) imgBit = bTextColor;//WAP_BLACK;
						else imgBit = bTextBackColor;//WAP_WHITE;

                        if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
			                *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;						

					}	// end of if (mode == WAP_DRAW_PICTURE_MATE)

				}	// end of if((pos_x<=nX) && (nX<=(WAP_LCD_WIDTH-1)))

			}// end of for - Image width 
		}
	}// end of for - Image setting 

	//if(inbuffer) wip_free((void *)inbuffer);
    */
}

void WAP_Display_Image_WBMP_Save(char* pDestBuf, char* pImgData,
							int width, int height, int pos_x, int pos_y,
							int mode, int align)
{
	BYTE imgBit;
	int Iwidth, Iheight, nX, nY; 

	int bwidth = (width+7)/8;
	int bheight = (height+7)/8;

	char *pt = pImgData;

    BYTE bTextColor = EQS_GetTextColor();
    BYTE bTextBackColor = EQS_GetBackColor();//EQS_GetTextBackColor();	

	BYTE inbuffer[120]; //char *inbuffer = (char *) wip_malloc(bwidth);
	memset(inbuffer, 0x00, 120);//init

	//--------------------------------------------
	// ȭ�鿡 ������ �ʴ� ������ ����Ÿ�� �����͸� ������Ű��,
	// ���Ŀ� �̹��� ���÷��̸� �����ϵ��� �Ѵ�
	if(pos_y < 0)
	{
		pt = pt + (bwidth * pos_y * (-1));
		height = height - (pos_y * (-1));
		
		pos_y = 0;
		bheight = (height+7)/8;
	}

	//------------------------------------------------
	Iwidth = pos_x + width - 1;				// �̹����� ���� �Ѱ��� 
	Iheight = pos_y + height - 1;			// �̹����� ���� �Ѱ��� 

    if (align == WAP_DRAW_PICTURE_MATE) //������ ���� 120*120���� �����ϱ� ����..
    {
	    if(pos_x > WAP_LCD_TOTAL_ADDRESS_X) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_LCD_TOTAL_ADDRESS_Y) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_TOTAL_ADDRESS_X) Iwidth = WAP_LCD_TOTAL_ADDRESS_X;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_LCD_TOTAL_ADDRESS_Y) Iheight = WAP_LCD_TOTAL_ADDRESS_Y;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_TOTAL_ADDRESS_X) Iwidth--;
	    if (Iheight == WAP_LCD_TOTAL_ADDRESS_Y) Iheight--;
    }
    else
    {
	    if(pos_x > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_WIDTH) Iwidth = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) Iheight = WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_WIDTH) Iwidth--;
	    if (Iheight == WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) Iheight--;
    }
	
	//------------------------------------------------
	// ���� �̹����� �׸���. 
	// height�� �ϳ��� �÷����鼭 �׸���. 
//	for(nY=0; nY<=Iheight; nY++)
	for(nY = pos_y ; nY <= Iheight ; nY++)
	{						
		// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
		memcpy(inbuffer, pt, bwidth);
		pt += bwidth;	//seek

		if ((pos_y <= nY) && (nY <= (WAP_LCD_HEIGHT-1)))
		{
			// �� ������ width ��Ʈ�� �׸���. 
			for(nX=0; nX<=Iwidth; nX++)
			{
				// x �� boundary check 
				if((nX/8) > 14) break;
			
				if((pos_x<=nX) && (nX<=(WAP_LCD_WIDTH-1))) //&& (pos_y<=nY && nY<=WAP_LCD_HEIGHT))
				{
#if !((defined WAP_B1) || (defined WAP_B2))
					int xIndex, nbit;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
					if(mode == WAP_DRAW_NORMAL)
						imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);
					else
						imgBit = gen_getbit(ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);

					if (align == WAP_DRAW_PICTURE_MATE)	// picture mate �� ��� 
					{
#if (defined WAP_B1) || (defined WAP_B2)
/*                       //color�� set
						if (imgBit == 0x01) imgBit = WAP_WHITE;
						else imgBit = WAP_BLACK;

						if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X) 
			                *(pDestBuf + (nY*WAP_LCD_TOTAL_ADDRESS_X) + nX) |= imgBit;
*/
                        //B/W�� set
                        if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X) 
                            color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/8)) + (nX/8)), (nX%8), imgBit);

#else   //#if (defined WAP_B1) || (defined WAP_B2)
                        if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= (nX/8) && (nX/8) < (WAP_LCD_TOTAL_ADDRESS_X/8)) 
							set_picture_mate_bit(pDestBuf[(nY*15)+(nX/8)], ChangeBitPosition(nX%8), imgBit);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
                    }
					else // picture mate�� �ƴ� ��� 
					{
#if !((defined WAP_B1) || (defined WAP_B2))
						if (imgBit == 0x01) imgBit = WAP_BLACK;
						else imgBit = WAP_WHITE;

                        xIndex = (int)(nX/8*2);

                        nbit = nX%8;
						if (nbit > 3)
						{
							xIndex++;	nbit = nbit - 4;
						}

						if(0<=nY && nY<80 && 0<=xIndex && xIndex<30)
//							gen_setbit(pDestBuf[(nY*WAP_LCD_TOTAL_ADDRESS_X) + xIndex], nbit, imgBit);
							gen_setbit(*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + xIndex), nbit, imgBit);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
						if (imgBit == 0x01) imgBit = bTextColor;//WAP_BLACK;
						else imgBit = bTextBackColor;//WAP_WHITE;

                        if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
			                *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;						
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

					}	// end of if (mode == WAP_DRAW_PICTURE_MATE)


				}	// end of if((pos_x<=nX) && (nX<=(WAP_LCD_WIDTH-1)))

			}// end of for - Image width 
		}
	}// end of for - Image setting 

	//if(inbuffer) wip_free((void *)inbuffer);
}

/////////////////////////////////////////////////////////////////////////////////
//
//  SKT�� �̹��� ��� ���� ���� �� ���� ��...... 
//
/////////////////////////////////////////////////////////////////////////////////
#include "sisdecoder.h"
void WAP_Display_Image_SIS(char* pDestBuf, char* pImgData,
					   int width, int height, int xPos, int yPos,
					   int mode, int align, int nBitCount)
{
    BITMAP Bitmap;

    if(nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = ((width+7)/8)*8;//width + width%8;
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = width;
        Bitmap.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR; 

    }
    else if(nBitCount == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = ((width+3)/4)*4;//width + (width%4);
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = width;
        Bitmap.Header.nBitsPerPixel = WAP_4GRAY_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR;
      
    }
    else if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = MAX_SKY_IMAGE_WIDTH;//120
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = MAX_SKY_IMAGE_WIDTH;//width;
        Bitmap.Header.nBitsPerPixel = WAP_256COLOR_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR;
    }

	if(yPos < 0)
	{
        Bitmap.Header.yStart = yPos * (-1);		    
		yPos = 0;
	}
    else
    {
        Bitmap.Header.yStart = 0;
    }

    //copy data
    Bitmap.pbData = (BYTE *)pImgData;
    EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);



/*
    BYTE imgBit = 0x00;
	int bwidth = 0, Iwidth, Iheight, nX, nY; 

    BYTE bTextColor = EQS_GetTextColor();
    BYTE bTextBackColor = EQS_GetBackColor();//EQS_GetTextBackColor();
    
	char *pt = pImgData;
    char *inbuffer= NULL;
	
    if(nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        bwidth = (width+7)/8;

	    inbuffer = (char *) wip_malloc(bwidth);
		if(inbuffer == NULL)
		{
			CLNTa_log(1,0,"inbuffer(���) NULL");
			return;
		}
	    memset(inbuffer, 0x00, bwidth);//init
    }
    else if(nBitCount == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
    {
        bwidth = (width+3)/4;

        inbuffer = (char *) wip_malloc(bwidth);
		if(inbuffer == NULL)
		{
			CLNTa_log(1,0,"inbuffer(�׷���) NULL");
			return;
		}
	    memset(inbuffer, 0x00, bwidth);//init
    }
    else if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
    {
        bwidth = MAX_SKY_IMAGE_WIDTH;//width;

        inbuffer = (char *) wip_malloc(bwidth);
		if(inbuffer == NULL)
		{
			CLNTa_log(1,0,"inbuffer(�÷�) NULL");
			return;
		}
	    memset(inbuffer, 0x00, bwidth);//init
    }

	//--------------------------------------------
	// ȭ�鿡 ������ �ʴ� ������ ����Ÿ�� �����͸� ������Ű��,
	// ���Ŀ� �̹��� ���÷��̸� �����ϵ��� �Ѵ�
	if(pos_y < 0)
	{
		pt = pt + (bwidth * pos_y * (-1));
		height = height - (pos_y * (-1));
		
		pos_y = 0;
	}


	Iwidth = pos_x + width - 1;				// �̹����� ���� �Ѱ��� 
	Iheight = pos_y + height - 1;			// �̹����� ���� �Ѱ��� 
    
    if (align == WAP_DRAW_PICTURE_MATE) //������ ���� 120*120���� �����ϱ� ����..
    {
	    if(pos_x > WAP_LCD_TOTAL_ADDRESS_X) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_LCD_TOTAL_ADDRESS_Y) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_TOTAL_ADDRESS_X) Iwidth = WAP_LCD_TOTAL_ADDRESS_X;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_LCD_TOTAL_ADDRESS_Y) Iheight = WAP_LCD_TOTAL_ADDRESS_Y;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_TOTAL_ADDRESS_X) Iwidth--;
	    if (Iheight == WAP_LCD_TOTAL_ADDRESS_Y) Iheight--;
    }
    else
    {
	    if(pos_x > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_DOC_HEIGHT) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_WIDTH) Iwidth = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_DOC_HEIGHT) Iheight = WAP_DOC_HEIGHT;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_WIDTH) Iwidth--;
	    if (Iheight == WAP_DOC_HEIGHT) Iheight--;
    }

	//------------------------------------------------
	// ���� �̹����� �׸���. 
	// height�� �ϳ��� �÷����鼭 �׸���. 
	for(nY = pos_y ; nY <= Iheight ; nY++)
	{						
		// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
		memcpy(inbuffer, pt, bwidth);
		pt += bwidth;	//seek

		if ((pos_y <= nY) && (nY <= (WAP_LCD_HEIGHT-1)))
		{
			if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
			{
				// �� ������ width ��Ʈ�� �׸���. 
				for(nX=0; nX<=Iwidth; nX++)
				{
					// x �� boundary check 
					if((nX/8) > 14) break;
				
					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) //&& (pos_y<=nY && nY<=WAP_LCD_HEIGHT))
					{
						if (align == WAP_DRAW_PICTURE_MATE)
							memcpy(&pDestBuf[(nY * WAP_LCD_TOTAL_ADDRESS_X) + nX], &inbuffer[nX-pos_x],Iwidth-nX);
						 else    
							memcpy(&pDestBuf[((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX], &inbuffer[nX-pos_x],Iwidth-nX);
					}
				}
			}
			else{

			    // �� ������ width ��Ʈ�� �׸���. 
			    for(nX=0; nX<=Iwidth; nX++)
			    {
				    // x �� boundary check 
				    if((nX/8) > 14) break;
			    
				    if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) //&& (pos_y<=nY && nY<=WAP_LCD_HEIGHT))
				    {
                        if(nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
                        {
					        if(mode == WAP_DRAW_NORMAL)
						        imgBit = gen_getbit(ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);
					        else
						        imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);

                            if (align == WAP_DRAW_PICTURE_MATE)
                            {
                                //B/W�� set
                                if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X) 
                                    color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/8)) + (nX/8)), (nX%8), imgBit);
                            }
                            else 
                            {
                                if (imgBit == 0x01) imgBit = bTextColor;//WAP_BLACK;
					            else imgBit = bTextBackColor;//WAP_WHITE;

                                if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
			                        *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;			
                            }
                        }
                        else if(nBitCount == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
                        {
						    imgBit = inbuffer[(nX-pos_x)/4];    

                            if (align == WAP_DRAW_PICTURE_MATE)
                            {
                                int nbit=0, xIndex=0;
                                //BYTE nbit1=0, nbit2=0;
                                //�ֻ���2bit-����2bit-����2bit-������2bit 
                                if((nX-pos_x)%4 == 0)      // inbuffer[] ���� �ֻ��� 2bit ó��
                                {
                                    imgBit &= 0xc0;

                                    if(imgBit == 0x00)      imgBit = 0x00;//{ nbit1 = 0; nbit2 = 0; }
                                    else if(imgBit == 0x40) imgBit = 0x01;//{ nbit1 = 0; nbit2 = 1; }
                                    else if(imgBit == 0x80) imgBit = 0x02;//{ nbit1 = 1; nbit2 = 0; }
                                    else if(imgBit == 0xc0) imgBit = 0x03;//{ nbit1 = 1; nbit2 = 1; }
                                }
                                else if((nX-pos_x)%4 == 1)   // inbuffer[] ���� ���� 2bit ó��
                                {
                                    imgBit &= 0x30;
                            
                                    if(imgBit == 0x00)      imgBit = 0x00;//{ nbit1 = 0; nbit2 = 0; }
                                    else if(imgBit == 0x10) imgBit = 0x01;//{ nbit1 = 0; nbit2 = 1; }
                                    else if(imgBit == 0x20) imgBit = 0x02;//{ nbit1 = 1; nbit2 = 0; }
                                    else if(imgBit == 0x30) imgBit = 0x03;//{ nbit1 = 1; nbit2 = 1; }
                                }
                                else if((nX-pos_x)%4 == 2)   // inbuffer[] ���� ���� 2bit ó��
                                {
                                    imgBit &= 0x0c;

                                    if(imgBit == 0x00)      imgBit = 0x00;//{ nbit1 = 0; nbit2 = 0; }
                                    else if(imgBit == 0x04) imgBit = 0x01;//{ nbit1 = 0; nbit2 = 1; }
                                    else if(imgBit == 0x08) imgBit = 0x02;//{ nbit1 = 1; nbit2 = 0; }
                                    else if(imgBit == 0x0c) imgBit = 0x03;//{ nbit1 = 1; nbit2 = 1; }
                                }
                                else					     // inbuffer[] ���� ������ 2bit ó��
                                {
                                    imgBit &= 0x03;

                                    if(imgBit == 0x00)      imgBit = 0x00;//{ nbit1 = 0; nbit2 = 0; }
                                    else if(imgBit == 0x01) imgBit = 0x01;//{ nbit1 = 0; nbit2 = 1; }
                                    else if(imgBit == 0x02) imgBit = 0x02;//{ nbit1 = 1; nbit2 = 0; }
                                    else if(imgBit == 0x03) imgBit = 0x03;//{ nbit1 = 1; nbit2 = 1; }
                            
                                }

						        xIndex = (int)(nX/4);
						        nbit = nX%4;

							    gen_setbit(*(pDestBuf + (nY*(WAP_LCD_TOTAL_ADDRESS_X/4)) + xIndex), nbit, imgBit);

    //                            color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/4)) + (nX/4)), (nX%4)*2, nbit1);
    //                            color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/4)) + (nX/4)), (nX%4)*2 +1, nbit2);
                                
                            }
                            else
                            {
                                //�ֻ���2bit-����2bit-����2bit-������2bit 
                                if((nX-pos_x)%4 == 0)      // inbuffer[] ���� �ֻ��� 2bit ó��
                                {
                                    imgBit &= 0xc0;

                                    if(imgBit == 0x00) imgBit = bTextBackColor;//WAP_WHITE;
                                    else if(imgBit == 0x40) imgBit = WAP_LIGHTGRAY;
                                    else if(imgBit == 0x80) imgBit = WAP_GRAY;
                                    else if(imgBit == 0xc0) imgBit = bTextColor;//WAP_BLACK;
                                }
                                else if((nX-pos_x)%4 == 1)   // inbuffer[] ���� ���� 2bit ó��
                                {
                                    imgBit &= 0x30;
                            
                                    if(imgBit == 0x00) imgBit = bTextBackColor;//WAP_WHITE;
                                    else if(imgBit == 0x10) imgBit = WAP_LIGHTGRAY;
                                    else if(imgBit == 0x20) imgBit = WAP_GRAY;
                                    else if(imgBit == 0x30) imgBit = bTextColor;//WAP_BLACK;
                                }
                                else if((nX-pos_x)%4 == 2)   // inbuffer[] ���� ���� 2bit ó��
                                {
                                    imgBit &= 0x0c;

                                    if(imgBit == 0x00) imgBit = bTextBackColor;//WAP_WHITE;
                                    else if(imgBit == 0x04) imgBit = WAP_LIGHTGRAY;
                                    else if(imgBit == 0x08) imgBit = WAP_GRAY;
                                    else if(imgBit == 0x0c) imgBit = WAP_BLACK;
                                }
                                else					     // inbuffer[] ���� ������ 2bit ó��
                                {
                                    imgBit &= 0x03;

                                    if(imgBit == 0x00) imgBit = bTextBackColor;//WAP_WHITE;
                                    else if(imgBit == 0x01) imgBit = WAP_LIGHTGRAY;
                                    else if(imgBit == 0x02) imgBit = WAP_GRAY;
                                    else if(imgBit == 0x03) imgBit = bTextColor;//WAP_BLACK;
                            
                                }
                
                                *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;
                            }                        
                        }
                        else if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
                        {
						    imgBit = inbuffer[nX-pos_x];    

                            if (align == WAP_DRAW_PICTURE_MATE)
                                *(pDestBuf + (nY * WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;
                            else    
                                *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;

                        }
                    }

    			}

			}// end of for - Image width 
		}
	}// end of for - Image setting 

	if(inbuffer) wip_free((void *)inbuffer);
*/
}



void WAP_Display_Image_SIS_Save(char* pDestBuf, char* pImgData,
					   int width, int height, int pos_x, int pos_y,
					   int mode, int align, int nBitCount)
{
#if !((defined WAP_B1) || (defined WAP_B2))
	BYTE imgBit;
	int Iwidth, Iheight, nX, nY; 
	
	int bwidth = (width+7)/8;
	int bheight = (height+7)/8;
	
	char *pt = pImgData;

	//BYTE inbuffer[100];
	char *inbuffer = (char *) wip_malloc(bwidth);
	memset(inbuffer, 0x00, bwidth);//init

#else   //#if !((defined WAP_B1) || (defined WAP_B2))
    BYTE imgBit = 0x00;
	int bwidth = 0, Iwidth, Iheight, nX, nY; 

    BYTE bTextColor = EQS_GetTextColor();
    BYTE bTextBackColor = EQS_GetBackColor();//EQS_GetTextBackColor();
    
	char *pt = pImgData;
    char *inbuffer= NULL;
	
    if(nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        bwidth = (width+7)/8;

	    inbuffer = (char *) wip_malloc(bwidth);
		if(inbuffer == NULL)
		{
			CLNTa_log(1,0,"inbuffer(���) NULL");
			return;
		}
	    memset(inbuffer, 0x00, bwidth);//init
    }
    else if(nBitCount == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
    {
        bwidth = (width+3)/4;

        inbuffer = (char *) wip_malloc(bwidth);
		if(inbuffer == NULL)
		{
			CLNTa_log(1,0,"inbuffer(�׷���) NULL");
			return;
		}
	    memset(inbuffer, 0x00, bwidth);//init
    }
    else if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
    {
        bwidth = MAX_SKY_IMAGE_WIDTH;//width;

        inbuffer = (char *) wip_malloc(bwidth);
		if(inbuffer == NULL)
		{
			CLNTa_log(1,0,"inbuffer(�÷�) NULL");
			return;
		}
	    memset(inbuffer, 0x00, bwidth);//init
    }
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	//--------------------------------------------
	// ȭ�鿡 ������ �ʴ� ������ ����Ÿ�� �����͸� ������Ű��,
	// ���Ŀ� �̹��� ���÷��̸� �����ϵ��� �Ѵ�
	if(pos_y < 0)
	{
		pt = pt + (bwidth * pos_y * (-1));
		height = height - (pos_y * (-1));
		
		pos_y = 0;
	}


	Iwidth = pos_x + width - 1;				// �̹����� ���� �Ѱ��� 
	Iheight = pos_y + height - 1;			// �̹����� ���� �Ѱ��� 
    
    if (align == WAP_DRAW_PICTURE_MATE) //������ ���� 120*120���� �����ϱ� ����..
    {
	    if(pos_x > WAP_LCD_TOTAL_ADDRESS_X) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_LCD_TOTAL_ADDRESS_Y) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_TOTAL_ADDRESS_X) Iwidth = WAP_LCD_TOTAL_ADDRESS_X;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_LCD_TOTAL_ADDRESS_Y) Iheight = WAP_LCD_TOTAL_ADDRESS_Y;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_TOTAL_ADDRESS_X) Iwidth--;
	    if (Iheight == WAP_LCD_TOTAL_ADDRESS_Y) Iheight--;
    }
    else
    {
	    if(pos_x > WAP_LCD_WIDTH) return;		// �̹��� X ���� ��ġ�� �Ѱ踦 ������ ���� 
	    if(pos_y > WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) return;		// �̹��� Y ���� ��ġ�� �Ѱ踦 ������ ���� 

        if(Iwidth > WAP_LCD_WIDTH) Iwidth = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
        if(Iheight > WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) Iheight = WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/;  //�̹��� height�� LCD height���� ū ���

	    if (Iwidth == WAP_LCD_WIDTH) Iwidth--;
	    if (Iheight == WAP_DOC_HEIGHT/*WAP_LCD_HEIGHT*/) Iheight--;
    }

	//------------------------------------------------
	// ���� �̹����� �׸���. 
	// height�� �ϳ��� �÷����鼭 �׸���. 
	for(nY = pos_y ; nY <= Iheight ; nY++)
	{						
		// �̹��� �����͸� �̿��ؼ� �����͸� �о���δ�. 
		memcpy(inbuffer, pt, bwidth);
		pt += bwidth;	//seek

		if ((pos_y <= nY) && (nY <= (WAP_LCD_HEIGHT-1)))
		{

#if (defined WAP_B1) || (defined WAP_B2)	
			if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
			{
				// �� ������ width ��Ʈ�� �׸���. 
				for(nX=0; nX<=Iwidth; nX++)
				{
					// x �� boundary check 
					if((nX/8) > 14) break;
				
					if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) //&& (pos_y<=nY && nY<=WAP_LCD_HEIGHT))
					{
						if (align == WAP_DRAW_PICTURE_MATE)
							memcpy(&pDestBuf[(nY * WAP_LCD_TOTAL_ADDRESS_X) + nX], &inbuffer[nX-pos_x],Iwidth-nX);
						 else    
							memcpy(&pDestBuf[((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX], &inbuffer[nX-pos_x],Iwidth-nX);
					}
				}
			}
			else{
#endif//#if (defined WAP_B1) || (defined WAP_B2)	

			// �� ������ width ��Ʈ�� �׸���. 
			for(nX=0; nX<=Iwidth; nX++)
			{
				// x �� boundary check 
				if((nX/8) > 14) break;
			
				if(pos_x<=nX && nX<=(WAP_LCD_WIDTH-1)) //&& (pos_y<=nY && nY<=WAP_LCD_HEIGHT))
				{
#if !((defined WAP_B1) || (defined WAP_B2))	
					int xIndex=0, nbit=0;

					if(mode == WAP_DRAW_NORMAL)
						imgBit = gen_getbit(ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);
					else
						imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);

                    if (imgBit == 0x01) imgBit = WAP_BLACK;
					else imgBit = WAP_WHITE;

					xIndex = (int)(nX/8*2);
					nbit = nX%8;
					if (nbit > 3)
					{
						xIndex++;	nbit = nbit - 4;
					}

					if (align == WAP_DRAW_PICTURE_MATE)
//						gen_setbit(pDestBuf[(nY*30) + xIndex], nbit, imgBit);
						gen_setbit(*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*30) + xIndex), nbit, imgBit);
					else 
//						gen_setbit(pDestBuf[(nY*WAP_LCD_TOTAL_ADDRESS_X) + xIndex], nbit, imgBit);
						gen_setbit(*(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + xIndex), nbit, imgBit);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
                    if(nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
                    {
					    if(mode == WAP_DRAW_NORMAL)
						    imgBit = gen_getbit(ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);
					    else
						    imgBit = gen_getbit(~ByteRotate(inbuffer[(nX-pos_x)/8]), (nX-pos_x)%8);

                        if (align == WAP_DRAW_PICTURE_MATE)
                        {
                            //B/W�� set
                            if (0<= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y && 0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X) 
                                color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/8)) + (nX/8)), (nX%8), imgBit);
                        }
                        else 
                        {
                            if (imgBit == 0x01) imgBit = bTextColor;//WAP_BLACK;
					        else imgBit = bTextBackColor;//WAP_WHITE;

                            if((0 <= nY && nY < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= nX && nX < WAP_LCD_TOTAL_ADDRESS_X))
			                    *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;			
                        }
                    }
                    else if(nBitCount == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
                    {
						imgBit = inbuffer[(nX-pos_x)/4];    

                        if (align == WAP_DRAW_PICTURE_MATE)
                        {
                            int nbit=0, xIndex=0;
                            //BYTE nbit1=0, nbit2=0;
                            //�ֻ���2bit-����2bit-����2bit-������2bit 
                            if((nX-pos_x)%4 == 0)      // inbuffer[] ���� �ֻ��� 2bit ó��
                            {
                                imgBit &= 0xc0;

                                if(imgBit == 0x00)      imgBit = 0x00;//{ nbit1 = 0; nbit2 = 0; }
                                else if(imgBit == 0x40) imgBit = 0x01;//{ nbit1 = 0; nbit2 = 1; }
                                else if(imgBit == 0x80) imgBit = 0x02;//{ nbit1 = 1; nbit2 = 0; }
                                else if(imgBit == 0xc0) imgBit = 0x03;//{ nbit1 = 1; nbit2 = 1; }
                            }
                            else if((nX-pos_x)%4 == 1)   // inbuffer[] ���� ���� 2bit ó��
                            {
                                imgBit &= 0x30;
                            
                                if(imgBit == 0x00)      imgBit = 0x00;//{ nbit1 = 0; nbit2 = 0; }
                                else if(imgBit == 0x10) imgBit = 0x01;//{ nbit1 = 0; nbit2 = 1; }
                                else if(imgBit == 0x20) imgBit = 0x02;//{ nbit1 = 1; nbit2 = 0; }
                                else if(imgBit == 0x30) imgBit = 0x03;//{ nbit1 = 1; nbit2 = 1; }
                            }
                            else if((nX-pos_x)%4 == 2)   // inbuffer[] ���� ���� 2bit ó��
                            {
                                imgBit &= 0x0c;

                                if(imgBit == 0x00)      imgBit = 0x00;//{ nbit1 = 0; nbit2 = 0; }
                                else if(imgBit == 0x04) imgBit = 0x01;//{ nbit1 = 0; nbit2 = 1; }
                                else if(imgBit == 0x08) imgBit = 0x02;//{ nbit1 = 1; nbit2 = 0; }
                                else if(imgBit == 0x0c) imgBit = 0x03;//{ nbit1 = 1; nbit2 = 1; }
                            }
                            else					     // inbuffer[] ���� ������ 2bit ó��
                            {
                                imgBit &= 0x03;

                                if(imgBit == 0x00)      imgBit = 0x00;//{ nbit1 = 0; nbit2 = 0; }
                                else if(imgBit == 0x01) imgBit = 0x01;//{ nbit1 = 0; nbit2 = 1; }
                                else if(imgBit == 0x02) imgBit = 0x02;//{ nbit1 = 1; nbit2 = 0; }
                                else if(imgBit == 0x03) imgBit = 0x03;//{ nbit1 = 1; nbit2 = 1; }
                            
                            }

						    xIndex = (int)(nX/4);
						    nbit = nX%4;

							gen_setbit(*(pDestBuf + (nY*(WAP_LCD_TOTAL_ADDRESS_X/4)) + xIndex), nbit, imgBit);

//                            color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/4)) + (nX/4)), (nX%4)*2, nbit1);
//                            color_setbit(*(pDestBuf+(nY*(WAP_LCD_TOTAL_ADDRESS_X/4)) + (nX/4)), (nX%4)*2 +1, nbit2);
                                
                        }
                        else
                        {
                            //�ֻ���2bit-����2bit-����2bit-������2bit 
                            if((nX-pos_x)%4 == 0)      // inbuffer[] ���� �ֻ��� 2bit ó��
                            {
                                imgBit &= 0xc0;

                                if(imgBit == 0x00) imgBit = bTextBackColor;//WAP_WHITE;
                                else if(imgBit == 0x40) imgBit = WAP_LIGHTGRAY;
                                else if(imgBit == 0x80) imgBit = WAP_GRAY;
                                else if(imgBit == 0xc0) imgBit = bTextColor;//WAP_BLACK;
                            }
                            else if((nX-pos_x)%4 == 1)   // inbuffer[] ���� ���� 2bit ó��
                            {
                                imgBit &= 0x30;
                            
                                if(imgBit == 0x00) imgBit = bTextBackColor;//WAP_WHITE;
                                else if(imgBit == 0x10) imgBit = WAP_LIGHTGRAY;
                                else if(imgBit == 0x20) imgBit = WAP_GRAY;
                                else if(imgBit == 0x30) imgBit = bTextColor;//WAP_BLACK;
                            }
                            else if((nX-pos_x)%4 == 2)   // inbuffer[] ���� ���� 2bit ó��
                            {
                                imgBit &= 0x0c;

                                if(imgBit == 0x00) imgBit = bTextBackColor;//WAP_WHITE;
                                else if(imgBit == 0x04) imgBit = WAP_LIGHTGRAY;
                                else if(imgBit == 0x08) imgBit = WAP_GRAY;
                                else if(imgBit == 0x0c) imgBit = WAP_BLACK;
                            }
                            else					     // inbuffer[] ���� ������ 2bit ó��
                            {
                                imgBit &= 0x03;

                                if(imgBit == 0x00) imgBit = bTextBackColor;//WAP_WHITE;
                                else if(imgBit == 0x01) imgBit = WAP_LIGHTGRAY;
                                else if(imgBit == 0x02) imgBit = WAP_GRAY;
                                else if(imgBit == 0x03) imgBit = bTextColor;//WAP_BLACK;
                            
                            }
                
                            *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;
                        }                        
                    }
                    else if(nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
                    {
						imgBit = inbuffer[nX-pos_x];    

                        if (align == WAP_DRAW_PICTURE_MATE)
                            *(pDestBuf + (nY * WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;
                        else    
                            *(pDestBuf + ((nY+WAP_HEAD_HEIGHT)*WAP_LCD_TOTAL_ADDRESS_X) + nX) = imgBit;

                    }
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
                }

#if (defined WAP_B1) || (defined WAP_B2)	
				}
#endif	//#if !((defined WAP_B1) || (defined WAP_B2))	

			}// end of for - Image width 
		}
	}// end of for - Image setting 

	if(inbuffer) wip_free((void *)inbuffer);
}

/////////////////////////////////////////////////////////////////////////////
//
//		Message Display
//
/////////////////////////////////////////////////////////////////////////////
int g_nAlign[8];
#define UI_WIDE 20//10

char g_strMessage0[UI_WIDE];
char g_strMessage1[UI_WIDE];
char g_strMessage2[UI_WIDE];
char g_strMessage3[UI_WIDE];
char g_strMessage4[UI_WIDE];
char g_strMessage5[UI_WIDE];
char g_strMessage6[UI_WIDE];
char g_strMessage7[UI_WIDE];

void WAP_ClearScreen()
{
	extern void WAP_Clear_LCDDataBuffer(void);

	//------------------------------------------------
	int i;
	for(i=0; i<8; i++) g_nAlign[i] = 0;

	//------------------------------------------------
	memset(g_strMessage0, '\0', UI_WIDE);
	memset(g_strMessage1, '\0', UI_WIDE);
	memset(g_strMessage2, '\0', UI_WIDE);
	memset(g_strMessage3, '\0', UI_WIDE);
	memset(g_strMessage4, '\0', UI_WIDE);
	memset(g_strMessage5, '\0', UI_WIDE);
	memset(g_strMessage6, '\0', UI_WIDE);
	memset(g_strMessage7, '\0', UI_WIDE);

	//------------------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();
}

void WAP_SetTextAtLine(int nLine, char* strText, int nAlign)
{
	char temp[WAP_MAX_TEXT];
	int nAlignX;

	//------------------------------------------------------------------
	// �޼��� ����
	memset(temp, '\0', WAP_MAX_TEXT);
	strcpy(temp, strText);
	
	if(strlen(strText) > UI_WIDE)	
	{
		extern void TruncateString(char *strSource, int nMaxWidth);
		TruncateString(temp, UI_WIDE);
	}

	//================================================================
	// Text Alignment
	nAlignX = 0;
	if(nAlign != WAP_TEXT_ALIGN_LEFT)
	{
		nAlignX = WAP_TextAlign(temp, 0, WAP_FONT_NORMAL);

		if(nAlign == WAP_TEXT_ALIGN_CENTER)		
			nAlignX = (WAP_LCD_WIDTH-nAlignX)/2;//center
		else if(nAlign == WAP_TEXT_ALIGN_RIGHT)
			nAlignX = WAP_LCD_WIDTH-nAlignX;//right
		else
			nAlignX = 0;//left
	}
	g_nAlign[nLine] = nAlignX;

	switch(nLine)
	{
		case 0: strcpy(g_strMessage0, temp); break;
		case 1: strcpy(g_strMessage1, temp); break;
		case 2: strcpy(g_strMessage2, temp); break;
		case 3: strcpy(g_strMessage3, temp); break;
		case 4: strcpy(g_strMessage4, temp); break;
		case 5: strcpy(g_strMessage5, temp); break;
		case 6: strcpy(g_strMessage6, temp); break;
		case 7: strcpy(g_strMessage7, temp); break;
	}
}

extern void WAP_Send_LCDBuffer_ToHS(void);
void WAP_DisplayMessage()
{
	int nLine;
	char temp[UI_WIDE];

	for(nLine=0; nLine<8; nLine++)
	{
		memset(temp, '\0', UI_WIDE);
		switch(nLine)
		{
			case 0: strcpy(temp, g_strMessage0); break;
			case 1: strcpy(temp, g_strMessage1); break;
			case 2: strcpy(temp, g_strMessage2); break;
			case 3: strcpy(temp, g_strMessage3); break;
			case 4: strcpy(temp, g_strMessage4); break;
			case 5: strcpy(temp, g_strMessage5); break;
			case 6: strcpy(temp, g_strMessage6); break;
			case 7: strcpy(temp, g_strMessage7); break;
		}
#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(g_nAlign[nLine]*2,//for double buffering
							nLine * WAP_ASCII_FONT_HEIGHT,
							temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(g_nAlign[nLine],
							nLine * WAP_ASCII_FONT_HEIGHT,
							temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}

	//------------------------------------------------------------------
	//handset task�� display�� ��û�Ѵ�
	WAP_Send_LCDBuffer_ToHS();
}

/////////////////////////////////////////////////////////////////////////////
//
//		WML Script�� Download�� Image�� ����
//
/////////////////////////////////////////////////////////////////////////////
#include "SISdecoder.h"

////////////////////////////////////////////////////////////////////////
//		
// Picture Mate �۾����� ���� ���� ��....	
//
////////////////////////////////////////////////////////////////////////
BOOL SaveWAP_BMPToFS(char *imgData, int leng, char *imgName)
{
    BITMAP  Image;  
    STATIC HFILE hFile;

    BYTE *pTempData = NULL;
    int bwidth=0, nImageSize=0, i, j;
    int imageWidth=0, imageHeight;    //�������� �̹��� width ����Ʈ��, �̹��� height

	int width, height, x, y;
	//------------------------------------------------------------------
	//#pragma pack (2)		���߿� Į��� �ľ��� ���� ��������� �м��Ѵ�. 
	char	*imgDataOrigin = imgData;
	typedef struct {
		unsigned char   fill1[2];
		unsigned long	bfsize;
		unsigned char   fill2[4];
		unsigned long	bfoffbits;
		unsigned char   fill3[4];
		long			biWidth;
		long			biHeight;
		unsigned char   fill4[2];
		short			biBitCount;
	}  BMPHeader;

	BMPHeader head;

	//unsigned char   fill1[2];
	head.fill1[0] = *imgData++;
	head.fill1[1] = *imgData++;
	//unsigned long	bfsize;
	head.bfsize = imgData[0] + (imgData[1]<<8) + (imgData[2]<<16) + (imgData[3]<<24);
	imgData += 4;
	//unsigned char   fill2[4];
	imgData += 4;
	//unsigned long	bfoffbits;
	head.bfoffbits = imgData[0] + (imgData[1]<<8) + (imgData[2]<<16) + (imgData[3]<<24);
	imgData += 4;
	//unsigned char   fill3[4];
	imgData += 4;
	//long			biWidth;
	head.biWidth = imgData[0] + (imgData[1]<<8) + (imgData[2]<<16) + (imgData[3]<<24);
	imgData += 4;
	//long			biHeight;
	head.biHeight = imgData[0] + (imgData[1]<<8) + (imgData[2]<<16) + (imgData[3]<<24);
	imgData += 4;
	//unsigned char   fill4[2];
	imgData += 2;
	//short				biBitCount;
	head.biBitCount = imgData[0] + (imgData[1]<<8);
	imgData += 2;

	// check file format (BMP)
	if (!(head.fill1[0] == 'B' && head.fill1[1] == 'M')) return FALSE;	
	// check file colors
	if(!(head.biBitCount == 1 || head.biBitCount == 4)) return FALSE;		
	

	//get image's information for center_align (120*120)
	width  = head.biWidth;
	if(width >= WAP_LCD_WIDTH)		x = 0;
	else							x = (WAP_LCD_TOTAL_ADDRESS_X-width)/2;//(WAP_LCD_WIDTH-width)/2;
	
	height = head.biHeight;
	if(height >= WAP_LCD_HEIGHT+WAP_HEAD_HEIGHT)	y = 0;
	else							y = (WAP_LCD_TOTAL_ADDRESS_Y-height)/2;//(WAP_LCD_HEIGHT-16-height)/2;

    //--------------------------------------------------------------------
    // �̹��� width, height�� LCD size���� ū ���, LCD�� �°� �̹��� ����
    //
    // 1. ���� �̹��� �� ������ byte�� ��� - �� ����ŭ �̹��� �����͸� �̵�
    if(head.biBitCount == 1)         bwidth = (width+31)/32*4;
    else if(head.biBitCount == 4)    bwidth = (width+7)/8*4;
    else if(head.biBitCount == 8)    bwidth = width;
    imageHeight = height;

    // 2. �̹��� width, height�� ����
    if(width > WAP_LCD_WIDTH) width = WAP_LCD_WIDTH;      //�̹��� width�� LCD width���� ū ���
    if(height > WAP_DOC_HEIGHT) imageHeight = WAP_DOC_HEIGHT;  //�̹��� height�� LCD height���� ū ���

    // 3. ������ �̹��� �� ������ byte�� ��� - �� ����ŭ �̹��� copy
    if(head.biBitCount == 1)         imageWidth = (width+7)/8;//(width+31)/32*4;
    else if(head.biBitCount == 4)    imageWidth = ((width+3)/4)*2;//(width+7)/8*4;
    else if(head.biBitCount == 8)    imageWidth = width;

    if(head.biBitCount == 1)
    {
        //Bitmap Header
        memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Image.Header.Size.xWidth = imageWidth*8;//((width+7)/8)*8;//120;//((width + 7)/8)*8;
        Image.Header.Size.yHeight = height;
        Image.Header.xWidth = width;
        Image.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
        Image.Header.fPalette = FALSE;
        Image.Header.TransparentColor = MAX_COLOR;
        Image.Header.yStart = 0;

        nImageSize = (Image.Header.Size.xWidth/8) * Image.Header.Size.yHeight;

        if(Image.pbData)
        {
		    wip_free((void*)Image.pbData);
		    Image.pbData =NULL;
        }        
        Image.pbData = (BYTE*)wip_malloc(nImageSize);
        memset((BYTE *)Image.pbData, 0x00, nImageSize);

        //4. Data�� ������ �ٲ���..
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy((BYTE*)Image.pbData + (imageWidth * j) , imgDataOrigin+head.bfoffbits + (bwidth * i), imageWidth);
        }
    }
    else if(head.biBitCount == 4)   //16color -> 4gray�� ���
    {
        //Bitmap Header
        memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Image.Header.Size.xWidth = (imageWidth/2)*4;//((width+3)/4)*4;   //���� �̹����� width�� 8�� ����� ���� ��
        Image.Header.Size.yHeight = height;
        Image.Header.xWidth = width;
        Image.Header.nBitsPerPixel = WAP_4GRAY_BITS_PER_PIXEL;     //HowManyColor
        Image.Header.fPalette = FALSE;     
        Image.Header.TransparentColor = MAX_COLOR;
        Image.Header.yStart = 0;

        nImageSize = (Image.Header.Size.xWidth/4) * Image.Header.Size.yHeight;

        //Data�� ������ �ٲ���..
        pTempData = (BYTE*)wip_malloc(imageWidth * Image.Header.Size.yHeight);
        memset((BYTE *)pTempData, 0x00, imageWidth * Image.Header.Size.yHeight);
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy(pTempData + (imageWidth * j) , imgDataOrigin+head.bfoffbits + (bwidth * i), imageWidth);
        }

        if(Image.pbData)
        {
		    wip_free((void*)Image.pbData);
		    Image.pbData =NULL;
        }        
        Image.pbData = (BYTE*)wip_malloc(nImageSize);
        memset((BYTE *)Image.pbData, 0x00, nImageSize);

        WAP_4GrayBMPTo2BITData((char*)Image.pbData, (char*)pTempData, width, height, Image.Header.Size.xWidth/4);

    }
    else if(head.biBitCount == 8)   //256color
    {
        //Bitmap Header
        memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Image.Header.Size.xWidth = width;
        Image.Header.Size.yHeight = height;
        Image.Header.xWidth = width;
        Image.Header.nBitsPerPixel = WAP_256COLOR_BITS_PER_PIXEL;     //HowManyColor
        Image.Header.fPalette = FALSE;     
        Image.Header.TransparentColor = MAX_COLOR;
        Image.Header.yStart = 0;

        nImageSize = Image.Header.Size.xWidth * Image.Header.Size.yHeight;

        if(Image.pbData)
        {
		    wip_free((void*)Image.pbData);
		    Image.pbData =NULL;
        }        
        Image.pbData = (BYTE*)wip_malloc(nImageSize);
        memset((BYTE *)Image.pbData, 0x00, nImageSize);

        //Data�� ������ �ٲ���..
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy((char *)Image.pbData + (imageWidth * j) , imgDataOrigin+head.bfoffbits + (bwidth * i), imageWidth);
        }
    }

    //----------------------------------------------------------------------------------------------------
	// ���� File System�� �����Ѵ�. 
    if(SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile) == SDR_OK)
    {
        SKY_WriteDataBlock(hFile, &Image.Header, sizeof(Image.Header));
        SKY_WriteDataBlock(hFile, Image.pbData, nImageSize);
        SKY_CloseDataFile(hFile);

        wip_free((VOID*)Image.pbData);
        if(pTempData)   wip_free((VOID*)pTempData);
		return TRUE;	// success
	}
    else
    {
        wip_free((VOID*)Image.pbData);
        if(pTempData)   wip_free((VOID*)pTempData);
    }

	return FALSE;		// fail
}


BOOL SaveWAP_WBMPToFS(char *pWBMPData, int leng, char* imgName)
{
    BITMAP  Image;  
    STATIC HFILE hFile;

    int     nImageSize = 0;

	int x, y;
	int image_width, image_height;
	int data_index;
	
	//check file format(WBMP)
	if(!(pWBMPData[0]==0 && pWBMPData[1]==0)) return FALSE;				
	
	//----------------------------------
	data_index = 2;//header Byte
	//----------------------------------
	image_width = 0;
	while(pWBMPData[data_index] > 127)
	{
		image_width += 127;
		data_index++;
	}
	image_width += pWBMPData[data_index++];
	//----------------------------------
	image_height = 0;
	while(pWBMPData[data_index] > 127)
	{
		image_height += 127;
		data_index++;
	}
	image_height += pWBMPData[data_index++];
	//----------------------------------

	//get image's information for center_align
	if(image_width >= WAP_LCD_WIDTH)x = 0;
	else							x = (WAP_LCD_TOTAL_ADDRESS_X-image_width)/2;
	
	if(image_height >= WAP_LCD_HEIGHT+WAP_HEAD_HEIGHT)y = 0;
	else							y = (WAP_LCD_TOTAL_ADDRESS_Y-image_height)/2;

    //Bitmap Header
    memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
    Image.Header.Size.xWidth = ((image_width + 7)/8)*8;
    Image.Header.Size.yHeight = image_height;
    Image.Header.xWidth = image_width;
    Image.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
    Image.Header.fPalette = FALSE;
    Image.Header.TransparentColor = MAX_COLOR;
    Image.Header.yStart = 0;

    nImageSize = (Image.Header.Size.xWidth/8) * Image.Header.Size.yHeight;
    Image.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_BW_IMAGE_SIZE);
    memset((BYTE *)Image.pbData, 0x00, nImageSize);

	memcpy((char *)Image.pbData, (char *)pWBMPData+data_index, nImageSize);  //remove header

	//----------------------------------------------------------------------------------------------------
	// ���� File System�� �����Ѵ�. 
    if(SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile) == SDR_OK)
    {
        SKY_WriteDataBlock(hFile, &Image.Header, sizeof(Image.Header));
        SKY_WriteDataBlock(hFile, Image.pbData, nImageSize);//WAP_BW_IMAGE_SIZE);
        SKY_CloseDataFile(hFile);

        wip_free((VOID*)Image.pbData);
		return TRUE;	// success
	}
    else wip_free((VOID*)Image.pbData);

	return FALSE;		// fail
}

BOOL SaveWAP_AnimationToFS(char *pSISData, int leng, char *imgName)
{
    ANIMATION_INFO_T  AnimationImage;
    STATIC HFILE hFile;
	
    int x, y, i, infoImage, result;

    int     nImageSize = 0;

	int		numberOfFrame, imageXSize, imageYSize, bestImage;
	int     varDelay, imageLevel, Loop; 
    int     delay[MAX_ANIMATION_FRAME-1]={0,};

    //����� image �ӽ� ����
    memset((char*)m_gImageBuf, 0, MAX_IMAGEBUF_SIZE);
	memcpy((char*)m_gImageBuf, (const char*)pSISData, leng);

	infoImage = getSISInfo((unsigned char*)m_gImageBuf, &numberOfFrame,
		                &imageXSize, &imageYSize, &varDelay, delay, &bestImage,
						&imageLevel, &Loop);

    if(infoImage == 0) //Error
        return FALSE;

	if(imageLevel == WAP_256COLOR_BITS_PER_PIXEL)
	{
		//���� X,Y size�� �Ʒ��� ����
		if(imageXSize == 0) imageXSize = 256;
		if(imageYSize == 0) imageYSize = 256;

		//�׷���, 128�� ������ ���Ƿ� 128�� �־���
		if(imageXSize > MAX_SKY_IMAGE_WIDTH) imageXSize = MAX_SKY_IMAGE_WIDTH;	//128
		if(imageYSize > MAX_SKY_IMAGE_HEIGHT) imageYSize = MAX_SKY_IMAGE_HEIGHT;	//128
	}

	//get image's information for center_align
	if(imageXSize >= WAP_LCD_WIDTH) x = 0;
	else							x = (WAP_LCD_TOTAL_ADDRESS_X-imageXSize)/2;
	
	if(imageYSize >= WAP_LCD_HEIGHT+WAP_HEAD_HEIGHT) y = 0;
	else							y = (WAP_LCD_TOTAL_ADDRESS_Y-imageYSize)/2;

    for(i = 0; i<MAX_ANIMATION_FRAME; i++)
        AnimationImage.AnimationHeader.awDelayTime[i] = 0;


    if(imageLevel == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        //Animation Header
        memcpy(AnimationImage.BitmapHeader.abID, EQUS_ANIMATION_ID_STR, EQUS_ID_LENGTH);
        AnimationImage.BitmapHeader.Size.xWidth = ((imageXSize+7)/8)*8;//imageXSize + (imageXSize%8);//120;
        AnimationImage.BitmapHeader.Size.yHeight = imageYSize;//120;
        AnimationImage.BitmapHeader.xWidth = imageXSize;//120;
        AnimationImage.BitmapHeader.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;
        AnimationImage.BitmapHeader.fPalette = FALSE;
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR;
        AnimationImage.BitmapHeader.yStart = 0;

        AnimationImage.AnimationHeader.nFrame = numberOfFrame;
        
        for(i = 0; i<numberOfFrame; i++)
        {
            if(varDelay == 0)
                AnimationImage.AnimationHeader.awDelayTime[i] = delay[0]*100 + 100;
            else
			{
				if(i < numberOfFrame-1)
	                AnimationImage.AnimationHeader.awDelayTime[i] = delay[i]*100 + 100;
				else
	                AnimationImage.AnimationHeader.awDelayTime[i] = delay[i-1]*100 + 100;
			}
        }
        
        nImageSize = (AnimationImage.BitmapHeader.Size.xWidth/8) * AnimationImage.BitmapHeader.Size.yHeight;
        AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_BW_IMAGE_SIZE);
    }
    else if(imageLevel == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
    {
        //Animation Header
        memcpy(AnimationImage.BitmapHeader.abID, EQUS_ANIMATION_ID_STR, EQUS_ID_LENGTH);
        AnimationImage.BitmapHeader.Size.xWidth = ((imageXSize+3)/4)*4;//imageXSize + (imageXSize%4);//120;
        AnimationImage.BitmapHeader.Size.yHeight = imageYSize;//120;
        AnimationImage.BitmapHeader.xWidth = imageXSize;//120;
        AnimationImage.BitmapHeader.nBitsPerPixel = WAP_4GRAY_BITS_PER_PIXEL;
        AnimationImage.BitmapHeader.fPalette = FALSE;
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR;
        AnimationImage.BitmapHeader.yStart = 0;

        AnimationImage.AnimationHeader.nFrame = numberOfFrame;
        
        for(i = 0; i<numberOfFrame; i++)
        {
            if(varDelay == 0)
                AnimationImage.AnimationHeader.awDelayTime[i] = delay[0]*100 + 100;
            else
			{
				if(i < numberOfFrame-1)
	                AnimationImage.AnimationHeader.awDelayTime[i] = delay[i]*100 + 100;
				else
	                AnimationImage.AnimationHeader.awDelayTime[i] = delay[i-1]*100 + 100;
			}
        } 

        nImageSize = (AnimationImage.BitmapHeader.Size.xWidth/4) * AnimationImage.BitmapHeader.Size.yHeight;
        AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_4GRAY_IMAGE_SIZE);
    }
    else if(imageLevel == WAP_256COLOR_BITS_PER_PIXEL)//256 color
    {
        //Animation Header
        memcpy(AnimationImage.BitmapHeader.abID, EQUS_ANIMATION_ID_STR, EQUS_ID_LENGTH);
        AnimationImage.BitmapHeader.Size.xWidth = imageXSize;//120;
        AnimationImage.BitmapHeader.Size.yHeight = imageYSize;
        AnimationImage.BitmapHeader.xWidth = imageXSize;
        AnimationImage.BitmapHeader.nBitsPerPixel = WAP_256COLOR_BITS_PER_PIXEL;
        AnimationImage.BitmapHeader.fPalette = FALSE;
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR;
        AnimationImage.BitmapHeader.yStart = 0;

        AnimationImage.AnimationHeader.nFrame = numberOfFrame;
        
        for(i = 0; i<numberOfFrame; i++)
        {
            if(varDelay == 0)
                AnimationImage.AnimationHeader.awDelayTime[i] = 900;//delay[0]*100 + 100;
            else
			{
				if(i < numberOfFrame-1)
	                AnimationImage.AnimationHeader.awDelayTime[i] = 900;//delay[i]*100 + 100;
				else
	                AnimationImage.AnimationHeader.awDelayTime[i] = 900;//delay[i-1]*100 + 100;
			}
        }

        nImageSize = AnimationImage.BitmapHeader.Size.xWidth * AnimationImage.BitmapHeader.Size.yHeight;
        AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_256COLOR_IMAGE_SIZE);
    }

	if(AnimationImage.pbData == NULL)
	{
		CLNTa_log(1,0,"wip_malloc() returns NULL");
		return FALSE;
	}

    //----------------------------------------------------------------------------------------------------
    // ���� File System�� �����Ѵ�. 
    if(SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile) == SDR_OK)
    {
        SKY_WriteDataBlock(hFile, &AnimationImage.BitmapHeader, sizeof(AnimationImage.BitmapHeader));
        SKY_WriteDataBlock(hFile, &AnimationImage.AnimationHeader, sizeof(AnimationImage.AnimationHeader));   

        for(i= 0; i<numberOfFrame; i++)
        {
            memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
            result = sisDecode( (unsigned char *)m_gImageBuf, (unsigned char *)m_gUncompBuf, i);
       
            if(result == 0)     //Error
            { 
                SKY_CloseDataFile(hFile);
                SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)imgName);
                wip_free((VOID*)AnimationImage.pbData);
                return FALSE;   
            }
               
            memcpy((VOID*)AnimationImage.pbData, (char*)m_gUncompBuf, nImageSize);
            SKY_WriteDataBlock(hFile, AnimationImage.pbData, nImageSize);//WAP_4GRAY_IMAGE_SIZE);

			//------------------------------------------------------
			// �̹��� ������ �ð��� �����ɸ��Ƿ� �ѹ��� kick�� ����
			dog_report(DOG_PS_RPT | DOG_DS_RPT | DOG_WAP_RPT);
        }
        SKY_CloseDataFile(hFile);

        if(AnimationImage.pbData) wip_free((VOID*)AnimationImage.pbData);
	    return TRUE;	// success
    }
    else
    {
        if(AnimationImage.pbData) wip_free((VOID*)AnimationImage.pbData);
    	return FALSE;
    }

    return FALSE;
}

BOOL SaveWAP_AnimationToFS_test(char *pSISData, int leng, char *imgName)
{
    ANIMATION_INFO_T  AnimationImage;
    STATIC HFILE hFile;
	
    int x, y, i, infoImage, result;

    int     nImageSize = 0;

	int		numberOfFrame, imageXSize, imageYSize, bestImage;
	int     varDelay, imageLevel, Loop; 
    int     delay[MAX_ANIMATION_FRAME-1]={0,};

    //����� image �ӽ� ����
    memset((char*)m_gImageBuf, 0, MAX_IMAGEBUF_SIZE);
	memcpy((char*)m_gImageBuf, (const char*)pSISData, leng);

	infoImage = getSISInfo((unsigned char*)m_gImageBuf, &numberOfFrame,
		                &imageXSize, &imageYSize, &varDelay, delay, &bestImage,
						&imageLevel, &Loop);

    if(infoImage == 0) //Error
        return FALSE;

	if(imageLevel == WAP_256COLOR_BITS_PER_PIXEL)
	{
		//���� X,Y size�� �Ʒ��� ����
		if(imageXSize == 0) imageXSize = 256;
		if(imageYSize == 0) imageYSize = 256;

		//�׷���, 128�� ������ ���Ƿ� 128�� �־���
		if(imageXSize > MAX_SKY_IMAGE_WIDTH) imageXSize = MAX_SKY_IMAGE_WIDTH;	//128
		if(imageYSize > MAX_SKY_IMAGE_HEIGHT) imageYSize = MAX_SKY_IMAGE_HEIGHT;	//128
	}

	//get image's information for center_align
	if(imageXSize >= WAP_LCD_WIDTH) x = 0;
	else							x = (WAP_LCD_TOTAL_ADDRESS_X-imageXSize)/2;
	
	if(imageYSize >= WAP_LCD_HEIGHT+WAP_HEAD_HEIGHT) y = 0;
	else							y = (WAP_LCD_TOTAL_ADDRESS_Y-imageYSize)/2;

    for(i = 0; i<MAX_ANIMATION_FRAME; i++)
        AnimationImage.AnimationHeader.awDelayTime[i] = 0;


    if(imageLevel == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        //Animation Header
        memcpy(AnimationImage.BitmapHeader.abID, EQUS_ANIMATION_ID_STR, EQUS_ID_LENGTH);
        AnimationImage.BitmapHeader.Size.xWidth = ((imageXSize+7)/8)*8;//imageXSize + (imageXSize%8);//120;
        AnimationImage.BitmapHeader.Size.yHeight = imageYSize;//120;
        AnimationImage.BitmapHeader.xWidth = imageXSize;//120;
        AnimationImage.BitmapHeader.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;
        AnimationImage.BitmapHeader.fPalette = FALSE;
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR;
        AnimationImage.BitmapHeader.yStart = 0;

        AnimationImage.AnimationHeader.nFrame = numberOfFrame;
        
        for(i = 0; i<numberOfFrame; i++)
        {
            if(varDelay == 0)
                AnimationImage.AnimationHeader.awDelayTime[i] = delay[0]*100 + 100;
            else
			{
				if(i < numberOfFrame-1)
	                AnimationImage.AnimationHeader.awDelayTime[i] = delay[i]*100 + 100;
				else
	                AnimationImage.AnimationHeader.awDelayTime[i] = delay[i-1]*100 + 100;
			}
        }
        
        nImageSize = (AnimationImage.BitmapHeader.Size.xWidth/8) * AnimationImage.BitmapHeader.Size.yHeight;
        AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_BW_IMAGE_SIZE);
 
        //----------------------------------------------------------------------------------------------------
        // ���� File System�� �����Ѵ�. 
        if(SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile) == SDR_OK)
        {
            SKY_WriteDataBlock(hFile, &AnimationImage.BitmapHeader, sizeof(AnimationImage.BitmapHeader));
            SKY_WriteDataBlock(hFile, &AnimationImage.AnimationHeader, sizeof(AnimationImage.AnimationHeader));   

            for(i= 0; i<numberOfFrame; i++)
            {
                memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
                result = sisDecode( (unsigned char *)m_gImageBuf, (unsigned char *)m_gUncompBuf, i);
           
                if(result == 0)     //Error
                { 
                    SKY_CloseDataFile(hFile);
                    SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)imgName);
                    wip_free((VOID*)AnimationImage.pbData);
                    return FALSE;   
                }
                //else if(result == 1)//Success
                {
//					memset((VOID*)AnimationImage.pbData, 0x00, WAP_BW_IMAGE_SIZE);    //init

//		            WAP_Display_Image_SIS((char*)AnimationImage.pbData, (char*)m_gUncompBuf, \
    			            imageXSize, imageYSize, x, y, 0, WAP_DRAW_PICTURE_MATE, WAP_BW_BITS_PER_PIXEL);
					memcpy((VOID*)AnimationImage.pbData, (char *)m_gUncompBuf, nImageSize);
                    SKY_WriteDataBlock(hFile, AnimationImage.pbData, nImageSize);//WAP_BW_IMAGE_SIZE);
                }

            }
            SKY_CloseDataFile(hFile);

            wip_free((VOID*)AnimationImage.pbData);
	        return TRUE;	// success
        }
        else    wip_free((VOID*)AnimationImage.pbData);
    }
    else if(imageLevel == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
    {
        //Animation Header
        memcpy(AnimationImage.BitmapHeader.abID, EQUS_ANIMATION_ID_STR, EQUS_ID_LENGTH);
        AnimationImage.BitmapHeader.Size.xWidth = ((imageXSize+3)/4)*4;//imageXSize + (imageXSize%4);//120;
        AnimationImage.BitmapHeader.Size.yHeight = imageYSize;//120;
        AnimationImage.BitmapHeader.xWidth = imageXSize;//120;
        AnimationImage.BitmapHeader.nBitsPerPixel = WAP_4GRAY_BITS_PER_PIXEL;
        AnimationImage.BitmapHeader.fPalette = FALSE;
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR;
        AnimationImage.BitmapHeader.yStart = 0;

        AnimationImage.AnimationHeader.nFrame = numberOfFrame;
        
        for(i = 0; i<numberOfFrame; i++)
        {
            if(varDelay == 0)
                AnimationImage.AnimationHeader.awDelayTime[i] = delay[0]*100 + 100;
            else
			{
				if(i < numberOfFrame-1)
	                AnimationImage.AnimationHeader.awDelayTime[i] = delay[i]*100 + 100;
				else
	                AnimationImage.AnimationHeader.awDelayTime[i] = delay[i-1]*100 + 100;
			}
        } 

        nImageSize = (AnimationImage.BitmapHeader.Size.xWidth/4) * AnimationImage.BitmapHeader.Size.yHeight;
        AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_4GRAY_IMAGE_SIZE);

        //----------------------------------------------------------------------------------------------------
        // ���� File System�� �����Ѵ�. 
        if(SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile) == SDR_OK)
        {
            SKY_WriteDataBlock(hFile, &AnimationImage.BitmapHeader, sizeof(AnimationImage.BitmapHeader));
            SKY_WriteDataBlock(hFile, &AnimationImage.AnimationHeader, sizeof(AnimationImage.AnimationHeader));   

            for(i= 0; i<numberOfFrame; i++)
            {
                memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
                result = sisDecode( (unsigned char *)m_gImageBuf, (unsigned char *)m_gUncompBuf, i);
           
                if(result == 0)     //Error
                { 
                    SKY_CloseDataFile(hFile);
                    SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)imgName);
                    wip_free((VOID*)AnimationImage.pbData);
                    return FALSE;   
                }
                //else if(result == 1)//Success
                {
//					memset((VOID*)AnimationImage.pbData, 0x00, WAP_4GRAY_IMAGE_SIZE);    //init

//		            WAP_Display_Image_SIS((char*)AnimationImage.pbData, (char*)m_gUncompBuf, \
    			            imageXSize, imageYSize, x, y, 0, WAP_DRAW_PICTURE_MATE, WAP_4GRAY_BITS_PER_PIXEL);
                    
                    memcpy((VOID*)AnimationImage.pbData, (char*)m_gUncompBuf, nImageSize);
                    SKY_WriteDataBlock(hFile, AnimationImage.pbData, nImageSize);//WAP_4GRAY_IMAGE_SIZE);
                }

				//------------------------------------------------------
				// �̹��� ������ �ð��� �����ɸ��Ƿ� �ѹ��� kick�� ����
				dog_report(DOG_PS_RPT | DOG_DS_RPT | DOG_WAP_RPT);
            }
            SKY_CloseDataFile(hFile);

            wip_free((VOID*)AnimationImage.pbData);
	        return TRUE;	// success
        }
        else    wip_free((VOID*)AnimationImage.pbData);
    }
    else if(imageLevel == WAP_256COLOR_BITS_PER_PIXEL)//256 color
    {
        //Animation Header
        memcpy(AnimationImage.BitmapHeader.abID, EQUS_ANIMATION_ID_STR, EQUS_ID_LENGTH);
        AnimationImage.BitmapHeader.Size.xWidth = imageXSize;//120;
        AnimationImage.BitmapHeader.Size.yHeight = imageYSize;
        AnimationImage.BitmapHeader.xWidth = imageXSize;
        AnimationImage.BitmapHeader.nBitsPerPixel = WAP_256COLOR_BITS_PER_PIXEL;
        AnimationImage.BitmapHeader.fPalette = FALSE;
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR;
        AnimationImage.BitmapHeader.yStart = 0;

        AnimationImage.AnimationHeader.nFrame = numberOfFrame;
        
        for(i = 0; i<numberOfFrame; i++)
        {
            if(varDelay == 0)
                AnimationImage.AnimationHeader.awDelayTime[i] = 900;//delay[0]*100 + 100;
            else
			{
				if(i < numberOfFrame-1)
	                AnimationImage.AnimationHeader.awDelayTime[i] = 900;//delay[i]*100 + 100;
				else
	                AnimationImage.AnimationHeader.awDelayTime[i] = 900;//delay[i-1]*100 + 100;
			}
        }

        nImageSize = AnimationImage.BitmapHeader.Size.xWidth * AnimationImage.BitmapHeader.Size.yHeight;
        AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_256COLOR_IMAGE_SIZE);
		
		if(AnimationImage.pbData == NULL)
		{
			CLNTa_log(1,0,"wip_malloc() returns NULL");
			return;
		}

        //----------------------------------------------------------------------------------------------------
        // ���� File System�� �����Ѵ�. 
        if(SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile) == SDR_OK)
        {
            SKY_WriteDataBlock(hFile, &AnimationImage.BitmapHeader, sizeof(AnimationImage.BitmapHeader));
            SKY_WriteDataBlock(hFile, &AnimationImage.AnimationHeader, sizeof(AnimationImage.AnimationHeader));   

            for(i= 0; i<numberOfFrame; i++)
            {
                memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
                result = sisDecode( (unsigned char *)m_gImageBuf, (unsigned char *)m_gUncompBuf, i);
           
                if(result == 0)     //Error
                {    
                    SKY_CloseDataFile(hFile);
                    SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)imgName);
                    wip_free((VOID*)AnimationImage.pbData);
                    return FALSE;
                }
                //else if(result == 1)//Success
                {
					memcpy((VOID*)AnimationImage.pbData, (char *)m_gUncompBuf, nImageSize);//WAP_256COLOR_IMAGE_SIZE);    //init

//		            WAP_Display_Image_SIS((char*)AnimationImage.pbData, (char*)m_gUncompBuf, \
    			            imageXSize, imageYSize, x, y, 0, WAP_DRAW_PICTURE_MATE, WAP_256COLOR_BITS_PER_PIXEL);

                    SKY_WriteDataBlock(hFile, AnimationImage.pbData, nImageSize);//WAP_256COLOR_IMAGE_SIZE);
                }
		
				//------------------------------------------------------
				// �̹��� ������ �ð��� �����ɸ��Ƿ� �ѹ��� kick�� ����
				dog_report(DOG_PS_RPT | DOG_DS_RPT | DOG_WAP_RPT);
            }
            SKY_CloseDataFile(hFile);

            wip_free((VOID*)AnimationImage.pbData);
	        return TRUE;	// success
        }
        else    wip_free((VOID*)AnimationImage.pbData);
    }

	return FALSE;

}
////////////////////////////////////////////////////////
//	Hangul copy routine
//	Inserted by shinji 98/09/24
void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen)
{
	while (nLen > 0) {
		if (*pSrc == '\0')
			break;

		if (*pSrc & 0x80) {		// �ѱ�
			if (nLen < 2)
				break;
			*pDest++ = *pSrc++;
			*pDest++ = *pSrc++;
			nLen -= 2;
		}
		else {					// ASCII
			*pDest++ = *pSrc++;
			nLen--;
		}
	}
	*pDest = '\0';
}

#if !((defined WAP_B1) || (defined WAP_B2))
extern nv_stat_enum_type wap_process_nv_item
(
  nv_items_enum_type  item_code,       /* Item to get                      */
  nv_item_type        *item_ptr,        /* Pointer where to put the item    */
  boolean              read			   /* read = TRUE; READ_F, read = FALSE;WRITE_F */
);
#endif //#if !((defined WAP_B1) || (defined WAP_B2))

int Process_DownloadedImage(char *data, int length,
							 char *url, char * title, int useImage, int imageType)
{
    PICTURE_TYPE_T      wapImage;
	PICTURE_IN_CASE_T   caseImage;
	boolean IsValid = FALSE;
    boolean IsDeleted = FALSE;

	char *imageTitle = title;

	//===================================================
	// 0. check disk space, when using broswer2501
	//===================================================

	if(BROWSER_VERSION == BROWSER_VERSION_2501)
	{
		extern BOOL CheckDiskSpaceForSetImage(void);
		if(!CheckDiskSpaceForSetImage()) return 1;//Error
	}

	//===================================================
	// 1. check image data
	//===================================================
	if(data == NULL) return 1;//Error

	//===================================================
	// 2. check image type & image name
	//
	//    - 0 : WBMP type
	//    - 1 : Animation type
	//    - 2 : BMP type
	//===================================================
	if(imageType > 2) return 1002;//Not_Supported_Format

	// image name check
	if (*title == NULL)
		imageTitle = "�̸�����";

	// if image name is over 14 then truncation...
	if (strlen(title) > SKY_FILE_NAME_MAX_LENGTH-1) 
		WAP_HanCopy(imageTitle, title, SKY_FILE_NAME_MAX_LENGTH-1);

	//Title�� ���� �����ϴ��� check!! -- �����ϸ� delete��, ���� 
	IsValid = WAP_IsValidFile(SDF_IMAGE, (BYTE *)imageTitle);
	if(IsValid)		//�����ϴ� ���
    {
        //�ϳ��� �׸��� ������쿡 �ߺ� ������ ���,
        //file�� delete�Ǹ� �������� ��� �⺻������ �ǹǷ�
        //�ٽ� �ٿ�޾��� ��, ������ �ѱ����� �Ǵ� ������������ backup 
        {
            caseImage.Idle = SKY_GET_IDLE_PICTURE(); 
            caseImage.IncomingCall = SKY_GET_INCOMING_PICTURE();
            caseImage.PowerOff = SKY_GET_POWER_OFF_PICTURE();
            caseImage.PowerOn = SKY_GET_POWER_ON_PICTURE(); 
            caseImage.WAPStart = SKY_GET_WAP_PICTURE();
        }

        IsDeleted = TRUE;
        SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)imageTitle);
    }


	/////////////////////////////////////////////////////////////
	// 3. ���� �̹����� �����Ѵ�.

	if (imageType == 2)			// BMP type
	{
		if(!SaveWAP_BMPToFS(data, length, imageTitle)) 
			return 1004;		// invalid image type
	} 
	else if (imageType == 0)	// WBMP type
	{
		if(!SaveWAP_WBMPToFS(data, length, imageTitle))
			return 1004;
	} 
	else if (imageType == 1)	// Animation type
	{
		if(!SaveWAP_AnimationToFS(data, length, imageTitle))
			return 1004;
	}

	//////////////////////////////////////////////////////////////
	// 4. ���ݱ��� �����Ǿ� �ִ� �̹��� ������ �ٽ� �����Ѵ�.
    if(IsDeleted)
    {
		SKY_SET_POWER_ON_PICTURE(caseImage.PowerOn);
		SKY_SET_POWER_OFF_PICTURE(caseImage.PowerOff);
		SKY_SET_IDLE_PICTURE(caseImage.Idle);
		SKY_SET_INCOMING_PICTURE(caseImage.IncomingCall);
		SKY_SET_WAP_PICTURE(caseImage.WAPStart);    
    }

	//////////////////////////////////////////////////////////////
	// 5. �ٿ���� �̹����� ���ϴ� ȭ�鿡 �������� �����Ѵ�.
    wapImage.wPictureID = sMY_PICTURE;
    strcpy((char*)wapImage.szFileName, imageTitle);//strncpy((char*)wapImage.szFileName, imageTitle, strlen(imageTitle));

    switch(useImage)
    {
    case 0:
		SKY_SET_POWER_ON_PICTURE(wapImage);
        break;
    case 1:
        SKY_SET_POWER_OFF_PICTURE(wapImage);
        break;
    case 2:
        SKY_SET_IDLE_PICTURE(wapImage);
		SKY_SET_IDLE_SCREEN(IS_PICTURE);
        break;
    case 3:
        SKY_SET_INCOMING_PICTURE(wapImage);
        break;
    case 4:
        SKY_SET_WAP_PICTURE(wapImage);
        break;
    default:
        SKY_SET_IDLE_PICTURE(wapImage);
        break;
    }

    return 0;//success

    MSG_HIGH("",0,0,0);
}








