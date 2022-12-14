//////////////////////////////////////////////////////////////////////////////
//																			//
//							 WML Broswer									//
//																			//
//////////////////////////////////////////////////////////////////////////////

#include "aapimem.h"
#include "capiclnt.h"
#include "capimem.h"
#include "capimmi.h"
#include "cmmnrsrc.h"
#include "aapiclnt.h"//for log
#include "clnt.h"
#include "comdef.h"    

//////////////////////////////////////////////////////////////////////////////
#include "GenDsp.h"
#include "mmi_data.h"

//////////////////////////////////////////////////////////////////////////////
#include "sky.h"    
#include "eqc.h"  
#include "ska.h"

//#define MAX_PHOTO_COUNT        50  //for photo mail (bbyes 01.08.23)
#define NEED_MIN_SPACE          GET_IMAGE_BYTE_SIZE(BitmapCameraInfo.Header) + SKY_MAX_CAMERA_IMAGE_FILE_LEN + 4  //for photo mail (bbyes 01.08.23)
#define SKY_MAX_CAMERA_IMAGE_FILE_LEN   16      //for photo mail (bbyes 01.08.23)
#define PHOTO_TOTAL_SIZE                10800*2//6912  //for photo mail

LOCAL BITMAP BitmapCameraInfo = {
    { EQUS_IMAGE_ID, { 120, 120}, 
      120, 16, FALSE, MAX_COLOR, 0}
};  //for photo mail (bbyes 01.08.23)  

extern byte* GetScreenBuffer(void);	//BYTE* EQS_API GetScreenBuffer(VOID)

extern void *wip_malloc (UINT32 size);
extern void wip_free (void* mem);
extern BOOL WAP_IsValidFile(SKY_DATA_FILE_E SkyDataFile, CONST BYTE* pszFileName);
extern void CancelRLPTimeOut(void);

ROM INT g_WapLoadingBarColor[6][6] = { //밝은색 -> 어두운
    { EQS_RGB(208,218,248), EQS_RGB(149,188,253), EQS_RGB(126,170,242), EQS_RGB(49,116,227), EQS_RGB(45,86,196), EQS_RGB(28,62,118) },      //오션블루
    { EQS_RGB(255,204,201), EQS_RGB(255,134,129), EQS_RGB(251,103,69), EQS_RGB(252,64,20), EQS_RGB(217,40,0), EQS_RGB(131,9,0)  },      //토마토레드
    { EQS_RGB(177,255,173), EQS_RGB(122,233,117), EQS_RGB(70,236,62), EQS_RGB(63,209,56), EQS_RGB(8,175,0), EQS_RGB(19,122,0)  },      //올리브그린
    { EQS_RGB(252,204,245), EQS_RGB(243,136,226), EQS_RGB(251,101,228), EQS_RGB(252,20,215), EQS_RGB(194,2,165), EQS_RGB(117,1,112)  },  //와인퍼플
    { EQS_RGB(205,156,240), EQS_RGB(176,104,226), EQS_RGB(139,81,175), EQS_RGB(97,39,134), EQS_RGB(64,25,88), EQS_RGB(44,13,63)  },   //울트라 마린
    { EQS_RGB(252,233,214), EQS_RGB(255,200,147), EQS_RGB(255,178,106), EQS_RGB(255,144,37), EQS_RGB(232,112,0), EQS_RGB(140,51,0)  }   //쥬시오렌지
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

#define gen_setbit(byte,bitno,color)  \
			(byte |= ((color << 6) >> (bitno * 2)))

#define gen_getbit(byte,bitno)   \
		  ( ((byte)&(1<<(bitno)))? 1 : 0 )

#define gen_xor(byte) \
            ( (byte) = (byte)^(0xFF))


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
//		WAP 포인트
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Point(int x, int y, COLOR color)
{
	if(y >= 0)
	{
		//------------------------------------------------
		//y값을 WAP_HEAD_HEIGHT만큼 옮겨준다.
		y = y + WAP_HEAD_HEIGHT;	//yjahn96 00.12.28

		if((0 <= y && y < WAP_LCD_TOTAL_ADDRESS_Y) && (0 <= x && x < WAP_LCD_WIDTH))
		    EQS_SetPixel(x, y, color);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 라인
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Line(int x1, int y1, int x2, int y2, COLOR color)
{
    COLOR OldTextColor=WHITE;
    int x=0, y=0;

    OldTextColor = EQS_GetTextColor();

	if (x1 > x2) {		// x1은 시작점, x2는 끝점, 시작점이 끝점보다 크면 위치를 서로 바꿔주고 그린다. 
		x = x1; x1 = x2; x2 = x;
		y = y1; y1 = y2; y2 = y;
	}

	// -----------------------------------------
	// 한계값 체크 	
	if (x1 >= WAP_LCD_WIDTH) return;		// x1이 한계값보다 크면 리턴 
	if (x2 >= WAP_LCD_WIDTH) x2 = (WAP_LCD_WIDTH-1);

	if (y1 >= WAP_LCD_HEIGHT) return;	// y1이 한계값보다 크면 리턴 
	if (y2 >= WAP_LCD_HEIGHT) y2 = (WAP_LCD_HEIGHT-1);
    if (y1 < 0) y1 = 0;
    if (y2 < 0) y2 = 0;

	//------------------------------------------------
	//y값을 WAP_HEAD_HEIGHT만큼 옮겨준다.
	y1 = y1 + WAP_HEAD_HEIGHT;	y2 = y2 + WAP_HEAD_HEIGHT;	

    EQS_SetTextColor(color);
    EQS_Line(x1, y1, x2, y2);
    EQS_SetTextColor(OldTextColor);
}

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 사각형
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Rectangle(int x1, int y1, int x2, int y2, BOOL fill, COLOR color)
{
	int x, y;
    COLOR OldTextColor=WHITE;
    
	if (x1 > x2) {		// x1은 시작점, x2는 끝점, 시작점이 끝점보다 크면 위치를 서로 바꿔주고 그린다. 
		x = x1; x1 = x2; x2 = x;
	}

	if (y1 > y2) {
		y = y1; y1 = y2; y2 = y;
	}

	// 한계값 체크 	
	if (x1 >= WAP_LCD_WIDTH) return;		// x1이 한계값보다 크면 리턴 
	if (x2 >= WAP_LCD_WIDTH) x2 = (WAP_LCD_WIDTH-1);
	if (y1 >= WAP_LCD_HEIGHT) return;	// y1이 한계값보다 크면 리턴 
	if (y2 >= WAP_LCD_HEIGHT) y2 = (WAP_LCD_HEIGHT-1);
    if (x1 < 0) x1 = 0;    if (x2 < 0) x2 = 0;
    if (y1 < 0) y1 = 0;    if (y2 < 0) y2 = 0;

	//------------------------------------------------
	//y값을 WAP_HEAD_HEIGHT만큼 옮겨준다.
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
}


/////////////////////////////////////////////////////////////////////////////
//
//		WAP 원
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
		//line break 처리 (다음 문자 고려)
		/*=================================================*/
		if(((*pText & 0x80) && (nTextWidth+nHanFontWidth > WAP_LCD_WIDTH))//한글
		  ||(!(*pText & 0x80) && (nTextWidth+nAsciiFontWidth > WAP_LCD_WIDTH)))//아스키
		{
			return(nTextWidth - nOriginX);
		}

		if(*pText == 0) break;
		wChar = *pText;

		/*=================================================*/
		if ( wChar & 0x80 )              /* 한글 처리      */
		/*=================================================*/
		{	  
			pText++; i--;
			if (*pText == 0) break;

			wChar <<= 8; wChar += *pText;

			nCharWidth = nHanFontWidth;
		}
		/*=================================================*/
		else							/* 아스키 처리     */
		/*=================================================*/
		{
			nCharWidth = nAsciiFontWidth;
		}

		/*=================================================*/
		// 한 문자 표시 후의 처리
		/*=================================================*/
		nTextWidth += nCharWidth;
		nPosX += nCharWidth;
		pText++;
		
	}/* end of for */
	
	return( nTextWidth - nOriginX );
}



/////////////////////////////////////////////////////////////////////////////
//
//		WAP 텍스트
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_TextOut(int x, int y, char* str,
			 int font, int mode, BOOL underline, int align,
			 int nTableColumns, int nCurColumn)
{
    extern void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen);

#ifdef BOLD
    COLOR OldColor=WHITE;
#else	//#ifdef BOLD
    COLOR OldTextBackColor=WHITE;
    COLOR OldTextColor=WHITE;
#endif	//#ifdef BOLD
	int nLength=0, nLen=0, leng=0, nLimit=0;
	char Text[21];
    
	static int nPrev_X = 120, nHeight = WAP_ASCII_FONT_HEIGHT; //Mixfont시 line넘어가는 문제

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

        //한 line에 들어갈 글자(byte) 수
        nLineChars = WAP_SCREEN_WIDTH/nTableColumns;
    }

#ifdef BOLD
    if(mode == WAP_DRAW_REVERSE)
    {
        OldColor = EQS_GetTextBackColor();
        EQS_SetTextBackColor(EQS_GetBackColor());
    }
#else	//#ifdef BOLD

	//색 지정
    OldTextBackColor = EQS_GetTextBackColor();
    OldTextColor = EQS_GetTextColor();

    if(mode == WAP_DRAW_REVERSE)
    {
        EQS_SetTextBackColor((COLOR)WAP_SELECT_BACK_COLOR);//RED);
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
    // (x!=0)일 경우 
	// 1. Text element가 연속으로 와서 각 element끼리 연속해서 display되야 할때....
	// 2. Table내의 Text일 경우 
    if(x !=0 )
    {
       int nChar = (nLineWidth - (x - nOffset))/nAsciiFontWidth;
	   
       memset(Text, NULL, 21);
	   WAP_HanCopy((char *)Text, pStr, nChar);
	   nLen = strlen((char *)Text);
 
		//if(y >= 0)//화면에 보여야 할 부분만...
		{	   
			if(x <= nPrev_X)	//새 line에 쓸때
			{
				nHeight = nAsciiFontHeight;
			}
			else					//같은 line에 쓸때
			{
				if(nHeight < nAsciiFontHeight)
					nHeight = nAsciiFontHeight;
			}
			nPrev_X = x;
			
			if(y >= 0)//화면에 보여야 할 부분만...
			{
#ifdef BOLD
			if(mode == WAP_DRAW_REVERSE)
			{
                if(y < WAP_DOC_HEIGHT)
			        EQS_TextOutStrLenTDM(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nLen, TDM_BOLD);
			}
		    else
#endif  //#ifdef BOLD
			{
                if(y < WAP_DOC_HEIGHT)
    				EQS_TextOutStrLen(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nLen);//nChar);//nLineChars); 
			}
		   
		   if(underline && (y < WAP_DOC_HEIGHT))
			   EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
						x+nLen*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);            
			}
		} 
		y += nHeight;//nAsciiFontHeight;

        pStr = pStr + nLen;
    }

	nLength = strlen(pStr);

    if(nLength <= 0) return;    //string이 없는 경우 불필요한 Textout을 안하도록 함 - for table

	if(nLength > nLineChars)
	{
		do{

			memset(Text, NULL, 21);
			pTemp = pStr + nLimit;
			WAP_HanCopy((char *)Text, pTemp, nLineChars);
            leng = strlen((char *)Text);
			nLimit = nLimit + leng;//nLen = nLen + leng;

            //if(y >= 0)//화면에 보여야 할 부분만...
            {
                if(leng >= nLineChars)    //align의미 없음, 무조건 left
                {
					if(nOffset <= nPrev_X)	//새 line에 쓸때
					{
						nHeight = nAsciiFontHeight;
					}
					else					//같은 line에 쓸때
					{
						if(nHeight < nAsciiFontHeight)
							nHeight = nAsciiFontHeight;
					}
					nPrev_X = nOffset;

					if(y >= 0)//화면에 보여야 할 부분만...
					{
#ifdef BOLD
				   if(mode == WAP_DRAW_REVERSE)
				   {

                       if(y < WAP_DOC_HEIGHT)
    					   EQS_TextOutStrLenTDM(nOffset, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng, TDM_BOLD);//nLineChars);  
				   }
				   else
#endif  //#ifdef BOLD
				   {
                        if(y < WAP_DOC_HEIGHT)
		    				EQS_TextOutStrLen(nOffset, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng);//nLineChars);   
				   }
                    if(underline && (y < WAP_DOC_HEIGHT))
                        EQS_Line(nOffset, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
							nOffset+leng*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);
					}
                }
                else    //align적용
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

					if(x <= nPrev_X)	//새 line에 쓸때
					{
						nHeight = nAsciiFontHeight;
					}
					else					//같은 line에 쓸때
					{
						if(nHeight < nAsciiFontHeight)
							nHeight = nAsciiFontHeight;
					}
					nPrev_X = x;

					if(y >= 0)//화면에 보여야 할 부분만...
					{
#ifdef BOLD
					if(mode == WAP_DRAW_REVERSE)
					{
                        if(y < WAP_DOC_HEIGHT)
    					   EQS_TextOutStrLenTDM(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng, TDM_BOLD);//nLineChars);  
					}
				    else
#endif  //#ifdef BOLD
					{
                        if(y < WAP_DOC_HEIGHT)
                            EQS_TextOutStrLen(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng);   
					}

                    if(underline && (y < WAP_DOC_HEIGHT))
                        EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                                 x+leng*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);            
					}
                }
            }

            y += nHeight;//nAsciiFontHeight;
            if(y > WAP_DOC_HEIGHT-1)    break;  //LCD범위를 지나면 나온다..
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

			if(x <= nPrev_X)	//새 line에 쓸때
			{
				nHeight = nAsciiFontHeight;
			}
			else					//같은 line에 쓸때
			{
				if(nHeight < nAsciiFontHeight)
					nHeight = nAsciiFontHeight;
			}
			nPrev_X = x;

#ifdef BOLD
			if(mode == WAP_DRAW_REVERSE)
			{
                if(y < WAP_DOC_HEIGHT)
       	            EQS_TextOutStrLenTDM(x, y+WAP_HEAD_HEIGHT, (BYTE*)Text, nLength, TDM_BOLD);//nLineChars);  
  
			}
		    else
#endif  //#ifdef BOLD
			{
                if(y < WAP_DOC_HEIGHT)
    	            EQS_TextOutStrLen(x, y+WAP_HEAD_HEIGHT, (BYTE*)Text, nLength);//nLineChars);  
			}
            if(underline && (y < WAP_DOC_HEIGHT))
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

        //한 line에 들어갈 글자(byte) 수
        nLineChars = WAP_SCREEN_WIDTH/nTableColumns;
    }

#ifdef BOLD
    if(mode == WAP_DRAW_REVERSE)
    {
        OldColor = EQS_GetTextBackColor();
        EQS_SetTextBackColor(EQS_GetBackColor());
    }
#else //#ifdef BOLD
	//색 지정
    OldTextBackColor = EQS_GetTextBackColor();
    OldTextColor = EQS_GetTextColor();

    if(mode == WAP_DRAW_REVERSE)
    {
        EQS_SetTextBackColor((COLOR)WAP_SELECT_BACK_COLOR);//RED);
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
    // (x!=0)일 경우 
	// 1. Text element가 연속으로 와서 각 element끼리 연속해서 display되야 할때....
	// 2. Table내의 Text일 경우 
    if(x !=0 )
    {
       int nChar = (nLineWidth - (x - nOffset))/nAsciiFontWidth;
	   
       memset(Text, NULL, 21);
	   WAP_HanCopy((char *)Text, pStr, nChar);
	   nLen = strlen((char *)Text);
 
		if(y >= 0)//화면에 보여야 할 부분만...
		{	   
#ifdef BOLD
		   if(mode == WAP_DRAW_REVERSE)
		   {
               if(y < WAP_DOC_HEIGHT)
			        EQS_TextOutStrLenTDM(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nChar, TDM_BOLD);//nLineChars);
		   }
		   else
#endif  //#ifdef BOLD
		   {
			   if(y < WAP_DOC_HEIGHT)
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

    if(nLength <= 0) return;    //string이 없는 경우 불필요한 Textout을 안하도록 함 - for table

	if(nLength > nLineChars)
	{
		//nLen = 0;	//init
		do{

			memset(Text, NULL, 21);
			pTemp = pStr + nLimit;
			WAP_HanCopy((char *)Text, pTemp, nLineChars);//WAP_HanCopy((char *)Text, pStr + nLimit/*nLen*/, nLineChars);
            leng = strlen((char *)Text);
			nLimit = nLimit + leng;//nLen = nLen + leng;

            if(y >= 0)//화면에 보여야 할 부분만...
            {
                if(leng >= nLineChars)    //align의미 없음, 무조건 left
                {
#ifdef BOLD
					if(mode == WAP_DRAW_REVERSE)
				   {
                        if(y < WAP_DOC_HEIGHT)
						    EQS_TextOutStrLenTDM(nOffset, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nLineChars, TDM_BOLD);//leng);
				   }
				   else
#endif  //#ifdef BOLD
				   {
                       if(y < WAP_DOC_HEIGHT)
						    EQS_TextOutStrLen(nOffset, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, nLineChars);//leng);   
				   }
                   if(underline)
                        EQS_Line(nOffset, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                                 nOffset+leng*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);
                }
                else    //align적용
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
						if(y < WAP_DOC_HEIGHT)
                            EQS_TextOutStrLenTDM(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng, TDM_BOLD);   
					}
					else
#endif  //#ifdef BOLD
					{
                        if(y < WAP_DOC_HEIGHT)
						    EQS_TextOutStrLen(x, y+ WAP_HEAD_HEIGHT , (BYTE*)Text, leng);   
					}
                    if(underline)
                        EQS_Line(x, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1, 
                                 x+leng*nAsciiFontWidth-1, (y+WAP_HEAD_HEIGHT)+nAsciiFontHeight-1);            
                }
            }

            y += nAsciiFontHeight;
			if(y > WAP_DOC_HEIGHT-1)    break;  //LCD범위를 지나면 나온다..
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
                if(y < WAP_DOC_HEIGHT)
				    EQS_TextOutStrLenTDM(x, y+WAP_HEAD_HEIGHT, (BYTE*)Text, nLineChars, TDM_BOLD);//nLength);  
			}
			else
#endif  //#ifdef BOLD
			{
                if(y < WAP_DOC_HEIGHT)
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

        //한 line에 들어갈 글자(byte) 수
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
        x = WAP_LCD_WIDTH-1;
        EQS_SetTextAlignHoriz(TAH_RIGHT);
    }


    //-----------------------------------------------------
    // Title draw
    EQS_FillRectangle(0, y, WAP_LCD_WIDTH -1, y+WAP_ASCII_FONT_HEIGHT-1, (COLOR)WAP_TITLE_BACK_COLOR);
    OldColor = EQS_GetTextColor();
    EQS_SetTextColor((COLOR)WAP_TITLE_TEXT_COLOR);//WAP_SELECT_TEXT_COLOR);
    EQS_SetDrawMode(DM_OR);
    EQS_TextOutStr(x, y, (BYTE*)Text);//(BYTE*)str);
    EQS_SetDrawMode(DM_COPY);
    EQS_SetTextColor(OldColor);

    EQS_SetTextAlignHoriz(TextAlign.Horiz);

    EQS_SetTextWrap(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//
//		WAP 이미지
//
/////////////////////////////////////////////////////////////////////////////
void WAP_Display_Image_CML(int kind_of_cml, int image ,int xPos, int yPos)
{
    //BITMAP *pImage = NULL;
    BITMAP Bitmap;

    if(xPos > WAP_LCD_WIDTH) return;		// 이미지 X 시작 위치가 한계를 넘으면 리턴 
	if(yPos > WAP_LCD_HEIGHT) return;		// 이미지 Y 시작 위치가 한계를 넘으면 리턴 

    if(kind_of_cml == WAP_LEFT_CML)     Bitmap = g_BmpWapCommandLineLeft;//&g_BmpNewWAPLeftCml40x13x4G;
    else if(kind_of_cml == WAP_UPDOWN_CML)   Bitmap = g_BmpWapCommandLineCenterArrow;//&g_BmpNewWAPUpDown10x13x4G;
    else if(kind_of_cml == WAP_OK_CML)       Bitmap = g_BmpWapCommandLineCenterOK;//&g_BmpNewWAPOK9x13x4G;
    else if(kind_of_cml == WAP_CENTER_CML)   Bitmap = g_BmpWapCommandLineCenter;//&g_BmpNewWAPCenter21x13x4G;
    else if(kind_of_cml == WAP_RIGHT_CML)    Bitmap = g_BmpWapCommandLineRight;//&g_BmpNewWAPRightCml40x13x4G;

    //Bitmap.Header = pImage->Header;
    Bitmap.pbData = Bitmap.pbData +  EQC_GET_FRAME_SIZE(Bitmap.Header) * image;
    
    EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);
}

void WAP_4GrayBMPTo2BITData(char* pDestBuf, char* imageData, int width, int height, int bwidth)
{
	int nX, nY, nbit; 
	byte imgBit = 0x00;

	char * pt = imageData;

	BYTE inbuffer[120];
	memset(inbuffer, 0x00, 120);//init

	// ------------------------------------------------
	// 실제 이미지를 그린다. 
	// height를 하나씩 늘려가면서 그린다. 
	//for(nY = height-1 ; nY >= 0 ; nY--)
	for(nY = 0 ; nY < height ; nY++)
	{				
		// 이미지 포인터를 이용해서 데이터를 읽어들인다. 
		memset(inbuffer, 0x00, 120);
		memcpy(inbuffer, pt, (width+3)/4*2);
		pt += (width+3)/4*2;	//seek

		for (nX = 0 ; nX <= width ; nX++)
		{
            int xIndex;
		
			imgBit = inbuffer[nX/2];
			if (nX%2 == 0)			// inbuffer[] 내의 상위 bit 처리 
			{
				imgBit &= 0xf0;

                if (imgBit <= 0x60) imgBit = 0x00;//0x03;//BLACK;
				else if (imgBit == 0x70) imgBit = 0x02;//0x01;//LIGHTGRAY;
				else if (imgBit == 0x80) imgBit = 0x01;//0x02;//GRAY;
				else imgBit = 0x03;//0x00;//WHITE;
			}						
			else					// inbuffer[] 내의 하위 bit 처리 
			{
				imgBit &= 0x0f;

                if (imgBit <= 0x06) imgBit = 0x00;//0x03;//BLACK;
				else if (imgBit == 0x07) imgBit = 0x02;//0x01;//LIGHTGRAY;
				else if (imgBit == 0x08) imgBit = 0x01;//0x02;//GRAY;
				else imgBit = 0x03;//0x00;//WHITE;
			}

			xIndex = (int)(nX/4);
			nbit = nX%4;

			gen_setbit(*(pDestBuf + (nY*bwidth) + xIndex), nbit, imgBit);

		} 
	} 
}

void WAP_Display_Image_BMP(char* pDestBuf, char* imageData, int width, int height,
						   int xPos, int yPos, int mode, int align,
						   int nBitCount)
//	이미지 데이터가 저장될 버퍼, 실제 이미지 데이터, 가로, 세로, 시작위치 가로, 시작위치 세로, 이미지 정렬, 이미지 칼라수 
{
    BITMAP Bitmap;
    int nImageSize=0;

    BYTE    *pTempData = NULL;
    int     bwidth=0, i, j;
    int     imageWidth=0, imageHeight;    //조정후의 이미지 width 바이트수, 이미지 height

	if (xPos % 2 == 1)			// 이미지 사이즈가 짝수로 떨어지지 않고 홀수인 경우를 위해 
		xPos -= 1;

    if(xPos > WAP_LCD_WIDTH) return;		// 이미지 X 시작 위치가 한계를 넘으면 리턴 
	if(yPos > WAP_DOC_HEIGHT) return;		// 이미지 Y 시작 위치가 한계를 넘으면 리턴

    //--------------------------------------------------------------------
    // 이미지 width, height가 LCD size보다 큰 경우, LCD에 맞게 이미지 조정
    //
    // 1. 실제 이미지 한 라인의 byte값 계산 - 이 값만큼 이미지 포인터를 이동
    if(nBitCount == 1)         bwidth = (width+31)/32*4;
    else if(nBitCount == 4)    bwidth = (width+7)/8*4;
    else if(nBitCount == 8)    bwidth = width;
    imageHeight = height;

    // 2. 이미지 width, height값 조정
    if(width > WAP_LCD_WIDTH) width = WAP_LCD_WIDTH;      //이미지 width가 LCD width보다 큰 경우
    if(height > WAP_DOC_HEIGHT) imageHeight = WAP_DOC_HEIGHT;  //이미지 height가 LCD height보다 큰 경우

    // 3. 조정후 이미지 한 라인의 byte값 계산 - 이 값만큼 이미지 copy
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
        Bitmap.Header.TransparentColor = MAX_COLOR_2; 

	    if(yPos < 0)
	    {
            Bitmap.Header.yStart = yPos * (-1);		    
		    yPos = 0;
	    }
        else
        {
            Bitmap.Header.yStart = 0;
        }

        //if(Bitmap.pbData)
        //{
		//    wip_free((void*)Bitmap.pbData);
		//    Bitmap.pbData =NULL;
        //}

        nImageSize = (Bitmap.Header.Size.xWidth/8) * Bitmap.Header.Size.yHeight;
        memset((char *)g_abBmpBuf, 0x00, nImageSize);
		Bitmap.pbData = g_abBmpBuf;
		//Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
		//memset((char *)Bitmap.pbData, 0x00, nImageSize);

        //4. Data를 역으로 바꿔줌..
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy((BYTE*)Bitmap.pbData + (imageWidth * j) , imageData + (bwidth * i), imageWidth);
        }

        for( i = 0; i < nImageSize; i++)
        {
            gen_xor(*((BYTE *)Bitmap.pbData + i));
        }

        if(WAP_DOC_HEIGHT - yPos < height) // 이미지가 화면아래에서 잘리는 경우
        {
            Bitmap.Header.Size.yHeight = WAP_DOC_HEIGHT - yPos; 
        }
        else
        {
            //이미지 미리보기시 scroll할 때 화면이 지저분한 현상 수정 - yjahn96 01.09.07
            Bitmap.Header.Size.yHeight = height - Bitmap.Header.yStart;
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
        Bitmap.Header.TransparentColor = MAX_COLOR_4;

	    if(yPos < 0)
	    {
            Bitmap.Header.yStart = yPos * (-1);		    
		    yPos = 0;
	    }
        else
        {
            Bitmap.Header.yStart = 0;
        }

        //if(Bitmap.pbData)
        //{
		//    wip_free((void*)Bitmap.pbData);
		//    Bitmap.pbData =NULL;
        //}

        if(pTempData)
        {
		    wip_free((void*)pTempData);
		    pTempData =NULL;
        }

        nImageSize = (Bitmap.Header.Size.xWidth/4) * Bitmap.Header.Size.yHeight;
        memset((char *)g_abBmpBuf, 0x00, nImageSize);
		Bitmap.pbData = g_abBmpBuf;
        //Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
        //memset((BYTE *)Bitmap.pbData, 0x00, nImageSize);

        //4. Data를 역으로 바꿔줌
        pTempData = (BYTE*)wip_malloc(imageWidth * Bitmap.Header.Size.yHeight);
        memset((BYTE *)pTempData, 0x00, imageWidth * Bitmap.Header.Size.yHeight);
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy(pTempData + (imageWidth * j) , imageData + (bwidth * i), imageWidth);
        }

        //5. 4bit인 4Gray BMP를 2bit으로 바꿔줌
        WAP_4GrayBMPTo2BITData((char*)Bitmap.pbData, (char*)pTempData, width, height, Bitmap.Header.Size.xWidth/4);

        if(WAP_DOC_HEIGHT - yPos < height) // 이미지가 화면아래에서 잘리는 경우
        {
            Bitmap.Header.Size.yHeight = WAP_DOC_HEIGHT - yPos; 
        }
        else
        {
            //이미지 미리보기시 scroll할 때 화면이 지저분한 현상 수정 - yjahn96 01.09.07
            Bitmap.Header.Size.yHeight = height - Bitmap.Header.yStart;
        }

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
        Bitmap.Header.TransparentColor = MAX_COLOR_256;

	    if(yPos < 0)
	    {
            Bitmap.Header.yStart = yPos * (-1);		    
		    yPos = 0;
	    }
        else
        {
            Bitmap.Header.yStart = 0;
        }

        //if(Bitmap.pbData)
        //{
		//    wip_free((void*)Bitmap.pbData);
		//    Bitmap.pbData =NULL;
        //}

        nImageSize = Bitmap.Header.Size.xWidth * Bitmap.Header.Size.yHeight;
        memset((char *)g_abBmpBuf, 0x00, nImageSize);
		Bitmap.pbData = g_abBmpBuf;
        //Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
        //memset((char *)Bitmap.pbData, 0x00, nImageSize);

        //4. Data를 역으로 바꿔줌
        for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
        {
            memcpy((BYTE *)Bitmap.pbData + (imageWidth * j) , imageData + (bwidth * i), imageWidth);
        }

        if(WAP_DOC_HEIGHT - yPos < height) // 이미지가 화면아래에서 잘리는 경우
        {
            Bitmap.Header.Size.yHeight = WAP_DOC_HEIGHT - yPos; 
        }
        else
        {
            //이미지 미리보기시 scroll할 때 화면이 지저분한 현상 수정 - yjahn96 01.09.07
            Bitmap.Header.Size.yHeight = height - Bitmap.Header.yStart;
        }

        EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);
    }
}


/////////////////////////////////////////////////////////////////////////////////
//
//  SKT와 이미지 사용 사항 결정 후 수정 요...... 일단은 흑백 WBMP 사용 
//
/////////////////////////////////////////////////////////////////////////////////
void WAP_Display_Image_WBMP(char* pDestBuf, char* pImgData,
							int width, int height, int xPos, int yPos,
							int mode, int align)
{
    BITMAP Bitmap;
    int i, nImageSize=0;

    //Bitmap Header
    memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
    Bitmap.Header.Size.xWidth = ((width+7)/8)*8;//width + width%8;
    Bitmap.Header.Size.yHeight = height;
    Bitmap.Header.xWidth = width;
    Bitmap.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
    Bitmap.Header.fPalette = FALSE;
    Bitmap.Header.TransparentColor = MAX_COLOR_2; 

    if(yPos < 0)
    {
        Bitmap.Header.yStart = yPos * (-1);		    
	    yPos = 0;
    }
    else
    {
        Bitmap.Header.yStart = 0;
    }

    //if(Bitmap.pbData)
    //{
	//	wip_free((void*)Bitmap.pbData);
	//	Bitmap.pbData =NULL;
    //}

    nImageSize = (Bitmap.Header.Size.xWidth/8) * Bitmap.Header.Size.yHeight;

    //copy data
    //Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
    //memset((char *)Bitmap.pbData, 0x00, nImageSize);
    //memcpy((char *)Bitmap.pbData, (BYTE *)pImgData, nImageSize);
    memset((char *)g_abBmpBuf, 0x00, nImageSize);
    memcpy((char *)g_abBmpBuf, (BYTE *)pImgData, nImageSize);
	Bitmap.pbData = g_abBmpBuf;    

    for( i = 0; i < nImageSize; i++)
    {
        gen_xor(*((BYTE *)Bitmap.pbData + i));
    }

    if(WAP_DOC_HEIGHT - yPos < height) // 이미지가 화면아래에서 잘리는 경우
    {
        Bitmap.Header.Size.yHeight = WAP_DOC_HEIGHT - yPos; 
    }
    else
    {
        //이미지 미리보기시 scroll할 때 화면이 지저분한 현상 수정 - yjahn96 01.09.07
        Bitmap.Header.Size.yHeight = height - Bitmap.Header.yStart;
    }

    EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);
}

/////////////////////////////////////////////////////////////////////////////////
//
//  SKT와 이미지 사용 사항 결정 후 수정 요...... 
//
/////////////////////////////////////////////////////////////////////////////////
#include "sisdecoder.h"
void WAP_Display_Image_SIS(char* pDestBuf, char* pImgData,
					   int width, int height, int xPos, int yPos,
					   int mode, int align, int nBitCount)
{
    BITMAP Bitmap;
	int i = 0, nImageSize = 0;

    if(nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        //Bitmap Header
        memcpy(Bitmap.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
        Bitmap.Header.Size.xWidth = ((width+7)/8)*8;//width + width%8;
        Bitmap.Header.Size.yHeight = height;
        Bitmap.Header.xWidth = width;
        Bitmap.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.TransparentColor = MAX_COLOR_2; 

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
        Bitmap.Header.TransparentColor = MAX_COLOR_4;

        nImageSize = (Bitmap.Header.Size.xWidth/4) * Bitmap.Header.Size.yHeight;
        memset((char *)g_abBmpBuf, 0x00, nImageSize);
		memcpy((char *)g_abBmpBuf, (BYTE *)pImgData, nImageSize);

        for( i = 0; i < nImageSize; i++)
        {
            gen_xor(*((BYTE *)g_abBmpBuf + i));
        }	

		// yjahn96 01.10.19 -------------------------------------------------------------------------------
		// 4Gray의 경우 00/01/10/11 -> 11/10/01/00 의 값으로 변환해주어야함.. (EQUS의 putbitmap함우 때문)
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
        Bitmap.Header.TransparentColor = MAX_COLOR_256;
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

    if(WAP_DOC_HEIGHT - yPos < height) // 이미지가 화면아래에서 잘리는 경우
    {
        Bitmap.Header.Size.yHeight = WAP_DOC_HEIGHT - yPos; 
    }
    else
    {
        //이미지 미리보기시 scroll할 때 화면이 지저분한 현상 수정 - yjahn96 01.09.07
        Bitmap.Header.Size.yHeight = height - Bitmap.Header.yStart;
    }

    //copy data
	if(nBitCount == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
	{
		Bitmap.pbData = g_abBmpBuf;
	}
	else
	{
	    Bitmap.pbData = (BYTE *)pImgData;
	}

    EQS_PutBitmap(xPos, yPos + WAP_HEAD_HEIGHT, &Bitmap);
}

/////////////////////////////////////////////////////////////////////////////
//
//		Message Display
//
/////////////////////////////////////////////////////////////////////////////
int g_nAlign[8];
//#define UI_WIDE 20//10

char g_strMessage0[WAP_HS_SCRN_WIDE];
char g_strMessage1[WAP_HS_SCRN_WIDE];
char g_strMessage2[WAP_HS_SCRN_WIDE];
char g_strMessage3[WAP_HS_SCRN_WIDE];
char g_strMessage4[WAP_HS_SCRN_WIDE];
char g_strMessage5[WAP_HS_SCRN_WIDE];
char g_strMessage6[WAP_HS_SCRN_WIDE];
char g_strMessage7[WAP_HS_SCRN_WIDE];

void WAP_ClearScreen()
{
	extern void WAP_Clear_LCDDataBuffer(void);

	//------------------------------------------------
	int i;
	for(i=0; i<8; i++) g_nAlign[i] = 0;

	//------------------------------------------------
	memset(g_strMessage0, '\0', WAP_HS_SCRN_WIDE);
	memset(g_strMessage1, '\0', WAP_HS_SCRN_WIDE);
	memset(g_strMessage2, '\0', WAP_HS_SCRN_WIDE);
	memset(g_strMessage3, '\0', WAP_HS_SCRN_WIDE);
	memset(g_strMessage4, '\0', WAP_HS_SCRN_WIDE);
	memset(g_strMessage5, '\0', WAP_HS_SCRN_WIDE);
	memset(g_strMessage6, '\0', WAP_HS_SCRN_WIDE);
	memset(g_strMessage7, '\0', WAP_HS_SCRN_WIDE);

	//------------------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();
}

void WAP_SetTextAtLine(int nLine, char* strText, int nAlign)
{
	char temp[WAP_MAX_TEXT];
	int nAlignX;

	//------------------------------------------------------------------
	// 메세지 쓰기
	memset(temp, '\0', WAP_MAX_TEXT);
	strcpy(temp, strText);
	
	if(strlen(strText) > WAP_HS_SCRN_WIDE)	
	{
		extern void TruncateString(char *strSource, int nMaxWidth);
		TruncateString(temp, WAP_HS_SCRN_WIDE);
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
	char temp[WAP_HS_SCRN_WIDE];

	for(nLine=0; nLine<8; nLine++)
	{
		memset(temp, '\0', WAP_HS_SCRN_WIDE);
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
		WAP_Display_TextOut(g_nAlign[nLine],
							nLine * WAP_ASCII_FONT_HEIGHT,
							temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
	}

	//------------------------------------------------------------------
	//handset task에 display를 요청한다
	WAP_Send_LCDBuffer_ToHS();
}

/////////////////////////////////////////////////////////////////////////////
//
//		WML Script의 Download된 Image의 저장
//
/////////////////////////////////////////////////////////////////////////////
#include "SISdecoder.h"

////////////////////////////////////////////////////////////////////////
//		
// Picture Mate 작업으로 인해 수정 중....	
//
////////////////////////////////////////////////////////////////////////
BOOL SaveWAP_BMPToFS(char *imgData, int leng, char *imgName)
{
	extern WAP_Card *g_pCard;

    BITMAP  Image;  
    STATIC HFILE hFile;

    BYTE *pTempData = NULL;
    int bwidth=0, nImageSize=0, i, j;
    int imageWidth=0, imageHeight;    //조정후의 이미지 width 바이트수, 이미지 height

	int width, height, x, y;
    WORD    iResult;

	WAP_Node *pCurNode;
	WAP_Content_Image* pImage = NULL;

	if(strcmp(imgData, "NOT") == 0)// get_content()없이 즉시 call
	{
		pCurNode = g_pCard->pHeadContent;

		while(pCurNode != NULL)
		{
			if(pCurNode->type == _ELEMENT_TYPE_IMAGE
			|| pCurNode->type == _ELEMENT_TYPE_LINK_IMAGE)
			{
				pImage = (WAP_Content_Image *)pCurNode->pContent;
				break;
			}
			pCurNode = pCurNode->pNextNode;
		}//end of while	

		//--------------------------------------------------------------------
		// 이미지 width, height가 LCD size보다 큰 경우, LCD에 맞게 이미지 조정
		//
		// 1. 실제 이미지 한 라인의 byte값 계산 - 이 값만큼 이미지 포인터를 이동
		if(pImage->nBitCount == 1)         bwidth = (pImage->width+31)/32*4;
		else if(pImage->nBitCount == 4)    bwidth = (pImage->width+7)/8*4;
		else if(pImage->nBitCount == 8)    bwidth = pImage->width;
		imageHeight = pImage->height;//height;

		// 2. 이미지 width, height값 조정
		if(pImage->width > WAP_LCD_WIDTH) pImage->width = WAP_LCD_WIDTH;      //이미지 width가 LCD width보다 큰 경우
		if(pImage->height > WAP_DOC_HEIGHT) imageHeight = WAP_DOC_HEIGHT;  //이미지 height가 LCD height보다 큰 경우

		// 3. 조정후 이미지 한 라인의 byte값 계산 - 이 값만큼 이미지 copy
		if(pImage->nBitCount == 1)         imageWidth = (pImage->width+7)/8;//(width+31)/32*4;
		else if(pImage->nBitCount == 4)    imageWidth = ((pImage->width+3)/4)*2;//(width+7)/8*4;
		else if(pImage->nBitCount == 8)    imageWidth = pImage->width;

		if(pImage->nBitCount == WAP_BW_BITS_PER_PIXEL)          //B/W
		{
			//Bitmap Header
			memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
			Image.Header.Size.xWidth = imageWidth*8;//((width+7)/8)*8;//width + width%8;
			Image.Header.Size.yHeight = pImage->height;//height;
			Image.Header.xWidth = pImage->width;//width;
			Image.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
			Image.Header.fPalette = FALSE;
			Image.Header.TransparentColor = MAX_COLOR_2; 
			Image.Header.yStart = 0;


			nImageSize = (Image.Header.Size.xWidth/8) * Image.Header.Size.yHeight;
			memset((char *)g_abBmpBuf, 0x00, nImageSize);
			Image.pbData = g_abBmpBuf;
			//Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
			//memset((char *)Bitmap.pbData, 0x00, nImageSize);

			//4. Data를 역으로 바꿔줌..
			for( i = pImage->height-1, j=0; i >= (pImage->height - imageHeight); i--, j++)
			{
				memcpy((BYTE*)Image.pbData + (imageWidth * j) , pImage->imageData + (bwidth * i), imageWidth);
			}

			for( i = 0; i < nImageSize; i++)
			{
				gen_xor(*((BYTE *)Image.pbData + i));
			}
	   }
		else if(pImage->nBitCount == WAP_4GRAY_BITS_PER_PIXEL * 2)  //4 Gray
		{
			//Bitmap Header
			memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
			Image.Header.Size.xWidth = (imageWidth/2)*4;//((width+3)/4)*4;//width + (width%4);
			Image.Header.Size.yHeight = pImage->height;//height;
			Image.Header.xWidth = pImage->width;//width;
			Image.Header.nBitsPerPixel = WAP_4GRAY_BITS_PER_PIXEL;     //HowManyColor
			Image.Header.fPalette = FALSE;
			Image.Header.TransparentColor = MAX_COLOR_4;
			Image.Header.yStart = 0;

			if(pTempData)
			{
				wip_free((void*)pTempData);
				pTempData =NULL;
			}

			nImageSize = (Image.Header.Size.xWidth/4) * Image.Header.Size.yHeight;
			memset((char *)g_abBmpBuf, 0x00, nImageSize);
			Image.pbData = g_abBmpBuf;
			//Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
			//memset((BYTE *)Bitmap.pbData, 0x00, nImageSize);

			//4. Data를 역으로 바꿔줌
			pTempData = (BYTE*)wip_malloc(imageWidth * Image.Header.Size.yHeight);
			memset((BYTE *)pTempData, 0x00, imageWidth * Image.Header.Size.yHeight);
			for( i = pImage->height-1, j=0; i >= (pImage->height - imageHeight); i--, j++)
			{
				memcpy(pTempData + (imageWidth * j) , pImage->imageData + (bwidth * i), imageWidth);
			}

			//5. 4bit인 4Gray BMP를 2bit으로 바꿔줌
			WAP_4GrayBMPTo2BITData((char*)Image.pbData, (char*)pTempData, pImage->width, pImage->height, Image.Header.Size.xWidth/4);
		}
		else if(pImage->nBitCount == WAP_256COLOR_BITS_PER_PIXEL)//256 color
		{
			//Bitmap Header
			memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
			Image.Header.Size.xWidth = pImage->width;//width;//MAX_SKY_IMAGE_WIDTH;//120
			Image.Header.Size.yHeight = pImage->height;//height;
			Image.Header.xWidth = pImage->width;//width;//MAX_SKY_IMAGE_WIDTH;//width;
			Image.Header.nBitsPerPixel = WAP_256COLOR_BITS_PER_PIXEL;     //HowManyColor
			Image.Header.fPalette = FALSE;
			Image.Header.TransparentColor = MAX_COLOR_256;
			Image.Header.yStart = 0;

			nImageSize = Image.Header.Size.xWidth * Image.Header.Size.yHeight;
			memset((char *)g_abBmpBuf, 0x00, nImageSize);
			Image.pbData = g_abBmpBuf;
			//Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
			//memset((char *)Bitmap.pbData, 0x00, nImageSize);

			//4. Data를 역으로 바꿔줌
			for( i = pImage->height-1, j=0; i >= (pImage->height - imageHeight); i--, j++)
			{
				memcpy((BYTE *)Image.pbData + (imageWidth * j) , pImage->imageData + (bwidth * i), imageWidth);
			}
		}

	}
	else	//contentbuffer를 통해 들어올 때
	{
		//------------------------------------------------------------------
		//#pragma pack (2)		나중에 칼라수 파악을 위해 헤더정보를 분석한다. 
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
		// 이미지 width, height가 LCD size보다 큰 경우, LCD에 맞게 이미지 조정
		//
		// 1. 실제 이미지 한 라인의 byte값 계산 - 이 값만큼 이미지 포인터를 이동
		if(head.biBitCount == 1)         bwidth = (width+31)/32*4;
		else if(head.biBitCount == 4)    bwidth = (width+7)/8*4;
		else if(head.biBitCount == 8)    bwidth = width;
		imageHeight = height;

		// 2. 이미지 width, height값 조정
		if(width > WAP_LCD_WIDTH) width = WAP_LCD_WIDTH;      //이미지 width가 LCD width보다 큰 경우
		if(height > WAP_DOC_HEIGHT) imageHeight = WAP_DOC_HEIGHT;  //이미지 height가 LCD height보다 큰 경우

		// 3. 조정후 이미지 한 라인의 byte값 계산 - 이 값만큼 이미지 copy
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
			Image.Header.TransparentColor = MAX_COLOR_2;
			Image.Header.yStart = 0;

			nImageSize = (Image.Header.Size.xWidth/8) * Image.Header.Size.yHeight;

			//if(Image.pbData)
			//{
			//    wip_free((void*)Image.pbData);
			//    Image.pbData =NULL;
			//}        
			//Image.pbData = (BYTE*)wip_malloc(nImageSize);
			//memset((BYTE *)Image.pbData, 0x00, nImageSize);
			memset((char *)g_abBmpBuf, 0x00, nImageSize);
			Image.pbData = g_abBmpBuf;

			//4. Data를 역으로 바꿔줌..
			for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
			{
				memcpy((BYTE*)Image.pbData + (imageWidth * j) , imgDataOrigin+head.bfoffbits + (bwidth * i), imageWidth);
			}

			for( i = 0; i < nImageSize; i++)
			{
				gen_xor(*((BYTE *)Image.pbData + i));
			}
		}
		else if(head.biBitCount == 4)   //16color -> 4gray로 취급
		{
			//Bitmap Header
			memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
			Image.Header.Size.xWidth = (imageWidth/2)*4;//((width+3)/4)*4;   //실제 이미지의 width를 8의 배수로 만든 값
			Image.Header.Size.yHeight = height;
			Image.Header.xWidth = width;
			Image.Header.nBitsPerPixel = WAP_4GRAY_BITS_PER_PIXEL;     //HowManyColor
			Image.Header.fPalette = FALSE;     
			Image.Header.TransparentColor = MAX_COLOR_4;
			Image.Header.yStart = 0;

			nImageSize = (Image.Header.Size.xWidth/4) * Image.Header.Size.yHeight;

			if(pTempData)
			{
				wip_free((void*)pTempData);
				pTempData =NULL;
			}   

			//Data를 역으로 바꿔줌..
			pTempData = (BYTE*)wip_malloc(imageWidth * Image.Header.Size.yHeight);
			memset((BYTE *)pTempData, 0x00, imageWidth * Image.Header.Size.yHeight);
			for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
			{
				memcpy(pTempData + (imageWidth * j) , imgDataOrigin+head.bfoffbits + (bwidth * i), imageWidth);
			}

			//if(Image.pbData)
			//{
			//    wip_free((void*)Image.pbData);
			//    Image.pbData =NULL;
			//}        
			//Image.pbData = (BYTE*)wip_malloc(nImageSize);
			//memset((BYTE *)Image.pbData, 0x00, nImageSize);
			memset((char *)g_abBmpBuf, 0x00, nImageSize);
			Image.pbData = g_abBmpBuf;

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
			Image.Header.TransparentColor = MAX_COLOR_256;
			Image.Header.yStart = 0;

			nImageSize = Image.Header.Size.xWidth * Image.Header.Size.yHeight;

			//if(Image.pbData)
			//{
			//    wip_free((void*)Image.pbData);
			//    Image.pbData =NULL;
			//}        
			//Image.pbData = (BYTE*)wip_malloc(nImageSize);
			//memset((BYTE *)Image.pbData, 0x00, nImageSize);
			memset((char *)g_abBmpBuf, 0x00, nImageSize);
			Image.pbData = g_abBmpBuf;

			//Data를 역으로 바꿔줌..
			for( i = height-1, j=0; i >= (height - imageHeight); i--, j++)
			{
				memcpy((char *)Image.pbData + (imageWidth * j) , imgDataOrigin+head.bfoffbits + (bwidth * i), imageWidth);
			}
		}
	}

    //----------------------------------------------------------------------------------------------------
	// 이제 File System에 저장한다. 
#ifdef FEATURE_CHARACBELL
    if (GetCharacbellSelected()) {
        iResult = SKY_CreateDataFile(SDF_CHA_IMAGE, (BYTE*)imgName, &hFile);
        SetCharacbellSelected(FALSE);
    }
    else
        iResult = SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile);
#else
    iResult = SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile);
#endif // FEATURE_CHARACBELL
    if (iResult == SDR_OK) {   
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
	extern WAP_Card *g_pCard;

    BITMAP  Image;  
    STATIC HFILE hFile;

    int     nImageSize = 0;

	int x, y, i;
	int image_width, image_height;
	int data_index;
	WORD    iResult;

	WAP_Node *pCurNode;
	WAP_Content_Image* pImage = NULL;

	if(strcmp(pWBMPData, "NOT") == 0)// get_content()없이 즉시 call
	{
		pCurNode = g_pCard->pHeadContent;

		while(pCurNode != NULL)
		{
			if(pCurNode->type == _ELEMENT_TYPE_IMAGE
			|| pCurNode->type == _ELEMENT_TYPE_LINK_IMAGE)
			{
				pImage = (WAP_Content_Image *)pCurNode->pContent;
				break;
			}
			pCurNode = pCurNode->pNextNode;
		}//end of while	

		//Bitmap Header
		memcpy(Image.Header.abID, EQUS_IMAGE_ID_STR, EQUS_ID_LENGTH);
		Image.Header.Size.xWidth = ((pImage->width+7)/8)*8;//width + width%8;
		Image.Header.Size.yHeight = pImage->height;//height;
		Image.Header.xWidth = pImage->width;//width;
		Image.Header.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;     //HowManyColor
		Image.Header.fPalette = FALSE;
		Image.Header.TransparentColor = MAX_COLOR_2; 
        Image.Header.yStart = 0;

	    nImageSize = (Image.Header.Size.xWidth/8) * Image.Header.Size.yHeight;

		//copy data
		//Bitmap.pbData = (BYTE*)wip_malloc(nImageSize);
		//memset((char *)Bitmap.pbData, 0x00, nImageSize);
		//memcpy((char *)Bitmap.pbData, (BYTE *)pImgData, nImageSize);
		memset((char *)g_abBmpBuf, 0x00, nImageSize);
		memcpy((char *)g_abBmpBuf, (BYTE *)pImage->imageData, nImageSize);
		Image.pbData = g_abBmpBuf;    

		for( i = 0; i < nImageSize; i++)
		{
			gen_xor(*((BYTE *)Image.pbData + i));
		}
	}
	else	//contentbuffer사용시
	{
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
		Image.Header.TransparentColor = MAX_COLOR_2;
		Image.Header.yStart = 0;

		//if(Image.pbData)
		//{
		//	wip_free((void*)Image.pbData);
		//	Image.pbData =NULL;
		//}

		nImageSize = (Image.Header.Size.xWidth/8) * Image.Header.Size.yHeight;
		memset((char *)g_abBmpBuf, 0x00, nImageSize);
		Image.pbData = g_abBmpBuf;
		//Image.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_BW_IMAGE_SIZE);
		//memset((BYTE *)Image.pbData, 0x00, nImageSize);

		memcpy((char *)Image.pbData, (char *)pWBMPData+data_index, nImageSize);  //remove header

		for( i = 0; i < nImageSize; i++)
		{
			gen_xor(*((BYTE *)Image.pbData + i));
		}
	}
	//----------------------------------------------------------------------------------------------------
	// 이제 File System에 저장한다. 
#ifdef FEATURE_CHARACBELL
    if (GetCharacbellSelected()) {
        iResult = SKY_CreateDataFile(SDF_CHA_IMAGE, (BYTE*)imgName, &hFile);
        SetCharacbellSelected(FALSE);
    }
    else
        iResult = SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile);
#else
    iResult = SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile);
#endif // FEATURE_CHARACBELL
    if (iResult == SDR_OK) {   
        SKY_WriteDataBlock(hFile, &Image.Header, sizeof(Image.Header));
        SKY_WriteDataBlock(hFile, Image.pbData, nImageSize);//WAP_BW_IMAGE_SIZE);
        SKY_CloseDataFile(hFile);

        wip_free((VOID*)Image.pbData);
		return TRUE;	// success
	}
    else wip_free((VOID*)Image.pbData);

	return FALSE;		// fail
}

BOOL SaveWAP_AnimationToFS(char *pSISData, int leng, char *imgName, char *url)
{
    ANIMATION_INFO_T  AnimationImage;
    STATIC HFILE hFile;

    DWORD   dwTimeInfo;
    BYTE    pMsgStr[17];//SKY_MAX_CAMERA_IMAGE_FILE_LEN+1=17
    PHOTO_LIST_T l_PhotoList[MAX_PHOTO_COUNT];
    WORD    /*tmpPhotoData[4],*/ iResult;
	
    int x, y, i, j, infoImage, result;

    int     nImageSize = 0;

	int		numberOfFrame, imageXSize, imageYSize, bestImage;
	int     varDelay, imageLevel, Loop; 
    int     delay[MAX_ANIMATION_FRAME-1]={0,};

	if(strcmp(pSISData, "NOT") != 0)// get_content()없이 즉시 call아닌 경우, 즉 setimage()를 통한경우..
	{
		//압축된 image 임시 저장
	    memset((char*)m_gImageBuf, 0, MAX_IMAGEBUF_SIZE);
		memcpy((char*)m_gImageBuf, (const char*)pSISData, leng);
	}

	infoImage = getSISInfo((unsigned char*)m_gImageBuf, &numberOfFrame,
		                &imageXSize, &imageYSize, &varDelay, delay, &bestImage,
						&imageLevel, &Loop);

    if(infoImage == 0) //Error
        return FALSE;

	if(imageLevel == WAP_256COLOR_BITS_PER_PIXEL)
	{
		//실제 X,Y size는 아래와 같음
		if(imageXSize == 0) imageXSize = 256;
		if(imageYSize == 0) imageYSize = 256;

		//그러나, 128이 넘으면 임의로 128로 넣어줌
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

    //if(AnimationImage.pbData)
    //{
	//	wip_free((void*)AnimationImage.pbData);
	//	AnimationImage.pbData =NULL;
    //}

    if(imageLevel == WAP_BW_BITS_PER_PIXEL)          //B/W
    {
        //Animation Header
        memcpy(AnimationImage.BitmapHeader.abID, EQUS_ANIMATION_ID_STR, EQUS_ID_LENGTH);
        AnimationImage.BitmapHeader.Size.xWidth = ((imageXSize+7)/8)*8;//imageXSize + (imageXSize%8);//120;
        AnimationImage.BitmapHeader.Size.yHeight = imageYSize;//120;
        AnimationImage.BitmapHeader.xWidth = imageXSize;//120;
        AnimationImage.BitmapHeader.nBitsPerPixel = WAP_BW_BITS_PER_PIXEL;
        AnimationImage.BitmapHeader.fPalette = FALSE;
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR_2;
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
        //AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_BW_IMAGE_SIZE);
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
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR_4;
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
        //AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_4GRAY_IMAGE_SIZE);
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
        AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR_256;
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
        //AnimationImage.pbData = (BYTE*)wip_malloc(nImageSize);//WAP_256COLOR_IMAGE_SIZE);
    }

    memset((char *)g_abBmpBuf, 0x00, nImageSize);
	AnimationImage.pbData = g_abBmpBuf;

	//if(AnimationImage.pbData == NULL)
	//{
	//	CLNTa_log(1,0,"wip_malloc() returns NULL");
	//	return FALSE;
	//}

    //----------------------------------------------------------------------------------------------------
    // 이제 File System에 저장한다. 
    //for  Photo mail download 
        if((strstr(url, "mms.n-top.com/pmsd")) != NULL)
        {  
            hFile = SKY_MakeCameraImageFile(SKY_GET_CAMERA_COUNT());
            if(hFile != SKY_INVALID_FILE_HANDLE)
            {   
                memset(pMsgStr,NULL,17);
                strncpy((char *)pMsgStr,imgName,14);
                dwTimeInfo = SKY_GetLocalTime();
                //memset((VOID*)g_abBmpBuf, 0x00, 1800);    //init//1800==CAMERA_BUF_SPACE

                AnimationImage.BitmapHeader.abID[0] = 'I';
                AnimationImage.BitmapHeader.abID[1] = 'M';
                AnimationImage.BitmapHeader.abID[2] = 'G';
                AnimationImage.BitmapHeader.Size.xWidth = 120;
                AnimationImage.BitmapHeader.Size.yHeight = 90;
                AnimationImage.BitmapHeader.xWidth = 120;//COLOR_IDLE_IMAGE_WIDTH;
                AnimationImage.BitmapHeader.nBitsPerPixel = WAP_256COLOR_BITS_PER_PIXEL;
                AnimationImage.BitmapHeader.fPalette = FALSE;
                AnimationImage.BitmapHeader.TransparentColor = MAX_COLOR_256;
                AnimationImage.BitmapHeader.yStart = 0;
            
                SKY_WriteDataBlock(hFile, pMsgStr, 16);      //File Name//14==SKY_MAX_CAMERA_IMAGE_FILE_LEN
                SKY_WriteDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));              //Time Info
                SKY_WriteDataBlock(hFile, &AnimationImage.BitmapHeader, sizeof(AnimationImage.BitmapHeader));
                //SKY_WriteDataBlock(hFile, g_abBmpBuf, 1800);
                for(i= 0; i<numberOfFrame; i++)
                {
                    memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
#ifdef FEATURE_MLZ
                    result = sisDecode( (unsigned char *)m_gImageBuf, (unsigned char *)m_gUncompBuf, i, 0);
#else
                    result = sisDecode( (unsigned char *)m_gImageBuf, (unsigned char *)m_gUncompBuf, i); 
#endif // FEATURE_MLZ
                               
                    if(result == 0)     //Error
                    {    
                        SKY_CloseDataFile(hFile);
                        SKY_DeleteDataFile(SDF_PHOTO,(BYTE *)pMsgStr);
                        //wip_free((VOID*)AnimationImage.pbData);
                        return FALSE;
                    }
                    //else if(result == 1)//Success
                    {
					    //memset((VOID*)AnimationImage.pbData, 0xFF, WAP_256COLOR_IMAGE_SIZE);    //init
                        memcpy((VOID*)AnimationImage.pbData, (char*)m_gUncompBuf, nImageSize);
		                //WAP_Display_Image_SIS((char*)AnimationImage.pbData, (char*)m_gUncompBuf, 
    			        //        imageXSize, imageYSize, x, y, 0, WAP_DRAW_PICTURE_MATE, WAP_256COLOR_BITS_PER_PIXEL);
/*
                      for(i=360;i<360+PHOTO_TOTAL_SIZE;i++)
                        {
                            tmpPhotoData[0]=(AnimationImage.pbData[i]>>5)*13;
                            tmpPhotoData[1]=((AnimationImage.pbData[i] & 00011100)>>2)*13;
                            tmpPhotoData[2]=(AnimationImage.pbData[i] & 00000011)*14;
                            tmpPhotoData[3]=RGB565(tmpPhotoData[0],tmpPhotoData[1],tmpPhotoData[2]);
                            
                            memcpy(&g_abBmpBuf[(i-360)*2],&tmpPhotoData[3],sizeof(WORD));
                        }
                        SKY_WriteDataBlock(hFile, g_abBmpBuf, PHOTO_TOTAL_SIZE);
*/                      
                        SKY_WriteDataBlock(hFile, &AnimationImage.pbData[360], 120*90);
						dog_report(DOG_PS_RPT | DOG_DS_RPT);
						if(P_REV_IS_6_OR_GREATER) CancelRLPTimeOut();	
                    }
                   
                }
                SKY_CloseDataFile(hFile);
                //change list
                hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
                SKY_WriteFile(hFile, (CONST void *)l_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                for(i=0 ; i<MAX_PHOTO_COUNT;i++)
                { 
                    if(l_PhotoList[i].PhotoList == NULL)
                    {
                        l_PhotoList[i].PhotoList = SKY_GET_CAMERA_COUNT()-1;
                        STRCPY(l_PhotoList[i].PhotoName, pMsgStr);

                        i = MAX_PHOTO_COUNT;
                    }
                }   
                SKY_SeekFile(hFile, FS_SO_SET, 0);
                SKY_WriteFile(hFile, (CONST void *)l_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                SKY_CloseFile(hFile);

                //wip_free((VOID*)AnimationImage.pbData);
		        return TRUE;	// success
	        }
            else
            { 
                //wip_free((VOID*)AnimationImage.pbData);
        
	            return FALSE;		// fail
            }
        }
        else
        {
#ifdef FEATURE_CHARACBELL
            if (GetCharacbellSelected()) {
                iResult = SKY_CreateDataFile(SDF_CHA_IMAGE, (BYTE*)imgName, &hFile);
                SetCharacbellSelected(FALSE);
            }
            else
                iResult = SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile);
#else
            iResult = SKY_CreateDataFile(SDF_IMAGE, (BYTE*)imgName, &hFile);
#endif // FEATURE_CHARACBELL
            if (iResult == SDR_OK) {   
                SKY_WriteDataBlock(hFile, &AnimationImage.BitmapHeader, sizeof(AnimationImage.BitmapHeader));
                SKY_WriteDataBlock(hFile, &AnimationImage.AnimationHeader, sizeof(AnimationImage.AnimationHeader));   

                for(i= 0; i<numberOfFrame; i++)
                {
                    memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
#ifdef FEATURE_MLZ
                    result = sisDecode( (unsigned char *)m_gImageBuf, (unsigned char *)m_gUncompBuf, i, 0);
#else
                    result = sisDecode( (unsigned char *)m_gImageBuf, (unsigned char *)m_gUncompBuf, i);
#endif // FEATURE_MLZ
                    if(result == 0)     //Error
                    { 
                        SKY_CloseDataFile(hFile);
                        SKY_DeleteDataFile(SDF_IMAGE,(BYTE *)imgName);
                        //wip_free((VOID*)AnimationImage.pbData);
                        return FALSE;   
                    }
               
					// yjahn96 01.10.19 -------------------------------------------------------------------------------
					// 4Gray의 경우 00/01/10/11 -> 11/10/01/00 의 값으로 변환해주어야함.. (EQUS의 putbitmap함우 때문)
					if(imageLevel == WAP_4GRAY_BITS_PER_PIXEL)  //4 Gray
					{
						for( j = 0; j < nImageSize; j++)
						{
							gen_xor(*((BYTE *)m_gUncompBuf + j));
						}
					}

                   	memcpy((VOID*)AnimationImage.pbData, (char*)m_gUncompBuf, nImageSize);
                    SKY_WriteDataBlock(hFile, AnimationImage.pbData, nImageSize);//WAP_4GRAY_IMAGE_SIZE);

			        //------------------------------------------------------
			        // 이미지 저장중 시간이 오래걸리므로 한번씩 kick을 해줌
			        dog_report(DOG_PS_RPT | DOG_DS_RPT);
					if(P_REV_IS_6_OR_GREATER) CancelRLPTimeOut();
                }
                SKY_CloseDataFile(hFile);

                //if(AnimationImage.pbData) wip_free((VOID*)AnimationImage.pbData);
	            return TRUE;	// success
            }
            else
            {
                //if(AnimationImage.pbData) wip_free((VOID*)AnimationImage.pbData);
    	        return FALSE;
            }
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

		if (*pSrc & 0x80) {		// 한글
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
		if((strstr(url, "mms.n-top.com/pmsd")) != NULL) //for photo mail
        {
            if(SKY_GetFileNumberInDataFile(SDF_PHOTO) < MAX_PHOTO_COUNT && \
                SKY_GetFreeSpaceInDataFile() >= NEED_MIN_SPACE)
            {}
            else 
			{
				return 1006;
			}
        }
        else
        {
            extern BOOL CheckDiskSpaceForSetImage(void);
		    if(!CheckDiskSpaceForSetImage()) 
			{
				return 1006;
			}
        }
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
		imageTitle = "이름없음";

	// if image name is over 14 then truncation...
	if (strlen(title) > SKY_FILE_NAME_MAX_LENGTH-1) 
		WAP_HanCopy(imageTitle, title, SKY_FILE_NAME_MAX_LENGTH-1);

    if((strstr(url, "mms.n-top.com/pmsd")) == NULL)//for photo mail
    {
	    //Title이 현재 존재하는지 check!! -- 존재하면 delete후, 저장 
	    IsValid = WAP_IsValidFile(SDF_IMAGE, (BYTE *)imageTitle);
	    if(IsValid)		//존재하는 경우
        {
            //하나의 그림이 여러경우에 중복 설정된 경우,
            //file이 delete되면 설정값이 모두 기본값으로 되므로
            //다시 다운받았을 때, 설정이 한군데만 되는 문제막기위해 backup 
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
    }

	/////////////////////////////////////////////////////////////
	// 3. 실제 이미지를 저장한다.

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
		if(!SaveWAP_AnimationToFS(data, length, imageTitle,url))
			return 1004;
	}

	//////////////////////////////////////////////////////////////
	// 4. 지금까지 설정되어 있던 이미지 정보를 다시 세팅한다.
    if(IsDeleted)
    {
		SKY_SET_POWER_ON_PICTURE(caseImage.PowerOn);
		SKY_SET_POWER_OFF_PICTURE(caseImage.PowerOff);
		SKY_SET_IDLE_PICTURE(caseImage.Idle);
		SKY_SET_INCOMING_PICTURE(caseImage.IncomingCall);
		SKY_SET_WAP_PICTURE(caseImage.WAPStart);    
    }

	//////////////////////////////////////////////////////////////
	// 5. 다운받은 이미지를 원하는 화면에 나오도록 설정한다.
    if((strstr(url, "mms.n-top.com/pmsd")) != NULL) //for photo mail
    {   
        char  szFileNumber[17]; //SKY_MAX_CAMERA_IMAGE_FILE_LEN+1=17

        wapImage.wPictureID = sMY_PHOTO;    
        ConvertCameraImageFileName((BYTE*)szFileNumber, SKY_GET_CAMERA_COUNT()-1);
        strcpy((char*)wapImage.szFileName, szFileNumber);
    }
    else
    {
        wapImage.wPictureID = sMY_PICTURE;
        strcpy((char*)wapImage.szFileName, imageTitle);//strncpy((char*)wapImage.szFileName, imageTitle, strlen(imageTitle));
    }    

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
}








