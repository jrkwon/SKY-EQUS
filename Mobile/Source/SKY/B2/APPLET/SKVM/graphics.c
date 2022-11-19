/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File: 
*
*  graphics_p.c
**
** Description: 
**     @DesciptionAboutThisFileHere
*
*  Copyright (C) 2000  XCE, Inc. All rights reserved.
*
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-22 wshan      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "mdarm_def.h"
#include "graphics.h"
#include "eqs.h"

#include <stdio.h> 

#define ITYPE_GIF   1
#define	ITYPE_LBM	2
#define ITYPE_PNG	3 
#define ITYPE_EBI	4

#define PNG_SUPPORTED
#define LBM_SUPPORTED
#define EBI_SUPPORTED
#define INDEX_ON_IMG(x, y, xWidth) ((x) + ((y)*(xWidth)))

/* 색깔 변경..*/
#define GRAY1TOCLR32( GRAY, COL ) COL = ((GRAY) ? 0x00c0c0c0 : 0x00000000)
#define LCDGRAY1TOCLR32( GRAY, COL ) COL = ((GRAY) ? 0x00000000 : 0x00c0c0c0)

#define LCDGRAY2TOCLR32( GRAY, COL ) COL = ((GRAY)&0x2 ?  \
		((GRAY)&0x1 ? /*3*/0x00000000 : /*2*/0x00404040 ) : \
		((GRAY)&0x1 ? /*1*/0x00808080 : /*0*/0x00c0c0c0 ))

#define GRAY2TOCLR32( GRAY, COL ) COL = ((GRAY)&0x2 ?  \
		((GRAY)&0x1 ? /*3*/0x00c0c0c0 : /*2*/0x00808080 ) : \
		((GRAY)&0x1 ? /*1*/0x00404040 : /*0*/0x00000000 ))

#define LCDGRAY8TOCLR32( GRAY, COL ) do { \
    	COL = (~(GRAY))&0xff; \
		COL |= COL << 16 | COL << 8; \
    	} while (0)

#define CLR8TOCLR32(COL8, COL32) do{\
    	COL32 = (((COL8>>5)*32)<<16)| ((((COL8<<3)>>5)*32)<<8)| (((COL8<<6)>>5)*64) ;\
    	}while(0)

#define CLR8TOCLR16(COL8, COL16) do{\
		COL16 = ((COL8&0xE0)<<8)| ((COL8&0X1C)<<6)| ((COL8&0x3)<<3);\
		}while(0)

#define CLR8TOLCDGRAY2( COL, GRAY ) do{ \
		GRAY = (((COL)>>6)&0x03)+(((COL)>>2)&0x07)+(((COL)>>4)&0x01)+(((COL)>>1)&0x01);\
		GRAY = (~(GRAY >> 2)) & 0x3; \
		} while (0)
/*
#define CLR8TOLCDGRAY2( COL, GRAY ) do{ \
    	BYTE r, g, b; \
		r = (COL)>>6, g = (COL&0x18)>>3, b = (COL&0x03); \
		GRAY = MAX3(r, g, b); \
		GRAY = (~GRAY) & 0x3; \
		} while (0)
*/
#define CLR16TOCLR8(COL16, COL8) do{\
		COL8 = ((COL16&0xE000)>>8)| ((COL16&0X0700)>>6)| ((COL16&0x0018)>>3) ;\
		}while(0)
#define CLR16TOCLR32(COL16, COL32) do{\
    	COL32 =((COL16&0xf800)<<8)|((COL16&0X07E0)<<5)|((COL16&0x001F)<<3);\
    	}while(0)		
/*#define CLR16TOLCDGRAY2( COL, GRAY ) do{ \
    	BYTE r, g, b; \
		r = (COL)>>14, g = (COL&0x0600)>>9, b = (COL&0x0018)>>3; \
		GRAY = MAX3(r, g, b); \
		GRAY = (~GRAY) & 0x3; \
		} while (0)
*/
#define CLR16TOLCDGRAY2( COL, GRAY ) do{ \
		GRAY = (((COL)>>12)&0x0f)+(((COL)>>14)&0x03)+(((COL)>>6)&0x1f)+(((COL)>>9)&0x03)+(((COL)>>2)&0x07);\
		GRAY = (~(GRAY >> 4)) & 0x3; \
		} while (0)
#define CLR32TOGRAY2( COL, GRAY ) do { \
    	BYTE r, g, b; \
		r = (COL)>>16, g = (COL)>>8, b = (COL); \
		GRAY = MAX3(r, g, b); \
		GRAY = (GRAY >> 6) & 0x3; \
		} while (0)
#define CLR32TOCLR8(COL32, COL8) do{\
		COL8=((COL32&0xE00000)>>16)|((COL32&0x00E000)>>11)|((COL32&0x0000C0)>>6) ;\
   		}while(0)

#define CLR32TOCLR16(COL32, COL16) do{\
		COL16=((COL32&0xf80000)>>8)|((COL32&0x00fc00)>>5)|((COL32&0x0000f8)>>3) ;\
   		}while(0)

#define CLR32TOLCDGRAY2( COL, GRAY ) do{ \
    	BYTE r, g, b; \
		r = (COL)>>16, g = (COL)>>8, b = (COL); \
		GRAY = MAX3(r, g, b); \
		GRAY = (~(GRAY >> 6)) & 0x3; \
		} while (0)


#define	ROBUSTCLIP(IMG) do { \
    if ((IMG)->clipx1 < 0) (IMG)->clipx1 = 0; \
    if ((IMG)->clipy1 < 0) (IMG)->clipy1 = 0; \
    if ((IMG)->clipx2 >= (IMG)->width) (IMG)->clipx2 = (IMG)->width-1; \
    if ((IMG)->clipy2 >= (IMG)->height) (IMG)->clipy2 = (IMG)->height-1; \
    } while(0)

#define OUTCLIP( IMG, X, Y ) \
    ( ((X)<(IMG)->clipx1)|| \
    ((X)>(IMG)->clipx2)|| \
    ((Y)<(IMG)->clipy1)|| \
    ((Y)>(IMG)->clipy2) )

#define IMAGESETPIXEL(BUF,IMG , X, Y, COL, MODE) do{ \
	switch((MODE)){\
		case 0:(BUF)[(Y)*(IMG)->width + (X)] =  COL;break; \
		case 1:(BUF)[(Y)*(IMG)->width + (X)] &= COL;break;\
		case 2:(BUF)[(Y)*(IMG)->width + (X)] |= COL;break;\
		case 3:(BUF)[(Y)*(IMG)->width + (X)] ^= COL;break;\
	}\
}while(0)

#define IMAGESETPIXEL4G(IMG, X, Y, _GRAY)do{\
	INT16 wBytePos;\
	INT8  nBitPos;\
	BYTE  GRAY=_GRAY;\
	wBytePos = TOT_BYTE_POS_4G(X, Y,(IMG)->width);\
	nBitPos = BIT_POS_4G(X);\
	if(OUTCLIP(IMG,X,Y)) break;\
	(IMG)->buf[wBytePos] &= m_abNotMask4G[nBitPos];\
	(IMG)->buf[wBytePos] |= (GRAY<<nBitPos);\
}while(0)


#ifndef ABSO
#define ABSO( A )     ( (A)<0 ? -(A) : (A) )
#endif
#ifndef MIN
#define MIN(A, B)	((A) > (B) ? (B) : (A))
#endif

#ifndef MAX
#define MAX(A, B)	((A) > (B) ? (A) : (B))
#endif

#ifndef MAX3
#define MAX3(A, B, C)	MAX(MAX(A,B),C)
#endif

#ifndef MIN3
#define MIN3(A, B, C)	MIN(MIN(A,B),C)
#endif

static BYTE ROM bitMask[8]  = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40, 0x80};
static BYTE ROM nbitMask[8]  = { 0xFE,0x0FD,0xFB,0xF7,0xEF,0xDF,0xBF, 0x7F};
static BYTE ROM b4Gray[4]  =    
{
    0x00,
    0x55,
    0xAA,
    0xFF,
};
static BYTE ROM b4GTo256[4]  =    
{
 /*   0x00,
	0x49,
	0x92,
	0xFF,
*/	0xFF,
	0x92,
	0x49,
    0x00,
};

static UINT16 ROM b4GTo64K[4] = 
{
	0xFFFF,
	0x8410,
	0x4208,
    0x0000,
/*  0x0000,
	0x4208,
	0x8410,
	0xFFFF,
*/
};

ROM LOCAL BYTE m_abMask4G[] = { 0x03, 0, 0x0C, 0, 0x30, 0, 0xC0 ,0};
ROM LOCAL BYTE m_abNotMask4G[] = { 0xFC, 0, 0xF3, 0, 0xCF, 0, 0x3F ,0};
static int ROM tritable1024[19][2] = 
{
    {    1024,        0},
    {    1020,       89},
    {    1008,      178},
    {     989,      265},
    {     962,      350},
    {     928,      433},
    {     887,      512},
    {     839,      587},
    {     784,      658},
    {     724,      724},
    {     658,      784},
    {     587,      839},
    {     512,      887},
    {     433,      928},
    {     350,      962},
    {     265,      989},
    {     178,     1008},
    {      89,     1020},
    {       0,     1024}
};

typedef struct tagBITMAP_FONT           BITMAP_FONT;
struct tagBITMAP_FONT {
    SIZE Size;
    INT8 nBitsPerPixel;
    ROM BYTE* pbFontBuffer;
};

void PutBitmapFontToImg(Image *im, COORD x, COORD y, WORD wCode, 
                                 CONST BITMAP_FONT* pBitmapFont);
void PutBitmapToImg(Image *im, COORD x, COORD y, CONST BITMAP* pBitmap);

XImage *g_img;
extern int image_create_mode;
// for test
int		   prev_create_mode;
//

#define BYTE_POS_BW(nIndexOfBuffer)   ((nIndexOfBuffer) >> 3 )
#define BYTE_POS_4G(nIndexOfBuffer)   ((nIndexOfBuffer) >> 2 )
#define BYTE_POS_256C(nIndexOfBuffer) (nIndexOfBuffer)
#define BYTE_POS_64KC(nIndexOfBuffer)  ((nIndexOfBuffer) << 1)
#define BYTE_POS_MASK(nIndexOfBuffer)   BYTE_POS_BW(nIndexOfBuffer)

#define BIT_POS_BW(nIndexOfBuffer)    (((nIndexOfBuffer) % 8))
#define BIT_POS_4G(nIndexOfBuffer)    ((((nIndexOfBuffer) << 1) % 8))
#define BIT_POS_256C(nIndexOfBuffer)  (0)
#define BIT_POS_64KC(nIndexOfBuffer)  (0)
#define BIT_POS_MASK(nIndexOfBuffer)   (((nIndexOfBuffer) % 8))

#define TOT_BYTE_POS_4G(X, Y, W)    (((X)>>2)+((((W)+3)>>2)*(Y)))
#define TOT_BYTE_POS_BW(X, Y, W)	(((X)>>3)+((((W)+7)>>3)*(Y)))
#define TOT_BYTE_POS_MASK(X, Y, W)  TOT_BYTE_POS_BW(X, Y, W)

#define SET_COLOR_BIT_1ST_BYTE(bBuffer, bData, nBitPos) \
							(bBuffer) &= ~((0xFF)<< (nBitPos));\
							(bBuffer) |= ((bData)<< (nBitPos));
#define SET_COLOR_BIT_2ND_BYTE(bBuffer, bData, nBitPos) \
							(bBuffer) &= ~((0xFF)>> (8-(nBitPos)));\
							(bBuffer) |= ((bData)>> (8-(nBitPos)));

#define SETMASK_COLOR_BIT_1ST_BYTE(bBuffer, bData, bMaskData, nBitPos) \
							(bBuffer) &= (((bMaskData)<<(nBitPos))|(0xFF>>(8-(nBitPos)))) ;\
							(bBuffer) |= (((bData)&(~(bMaskData)))<<(nBitPos));

#define SETMASK_COLOR_BIT_2ND_BYTE(bBuffer, bData, bMaskData, nBitPos) \
							(bBuffer) &= ((bMaskData)>>(8-(nBitPos))|(0xFF<<(nBitPos)));\
							(bBuffer) |= (((bData)&(~(bMaskData)))>>(8-(nBitPos)));

#define CONVERT_1BIT_2_2BIT(bByte, b1st, b2nd) do{\
			b1st = b2nd = 0;\
			if(bByte&0x01) b1st|=0x03;\
			if(bByte&0x02) b1st|=0x0C;\
			if(bByte&0x04) b1st|=0x30;\
			if(bByte&0x08) b1st|=0xC0;\
			if(bByte&0x10) b2nd|=0x03;\
			if(bByte&0x20) b2nd|=0x0C;\
			if(bByte&0x40) b2nd|=0x30;\
			if(bByte&0x80) b2nd|=0xC0;\
		}while(0)

							
/*************************************************************
* function: imageCreateMain
* desc: 	create main screen image.
*		you can map it to screen buffer directly
*		without using imageCreate().
* input:	width - screen width
* 		height - screen height
* return:	Image *
*checked by tortol
&&&&&&&&&&&&&&&
**************************************************************/
Image *imageCreateMain( int width, int height )
{
	XImage *img = (XImage *)EQS_Malloc(sizeof(XImage));
	
	img->internal = NULL;
	img->width = SKY_SCREEN_WIDTH;
#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
	img->height = SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT;
#else	
	img->height = SKY_SCREEN_HEIGHT;
#endif
	img->clipx1 = 0;
	img->clipy1 = 0; 
	img->clipx2 = img->width-1;
	img->clipy2 = img->height-1;   
	
#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
	if(!SKY_GET_SKVMCOLORENV()){
		img->size= img->width * img->height;
		img->buf = GetScreenBuffer()+SKY_SCREEN_WIDTH*(SKY_ANNUNCIATORS_HEIGHT);
		img->transcolor = 0x01;
		img->type =IMAGE_TYPE_256C;
	}
	else{
		img->size= 2*img->width * img->height;
		img->buf = GetScreenBuffer()+2*SKY_SCREEN_WIDTH*(SKY_ANNUNCIATORS_HEIGHT);
		img->transcolor = 0x08;
		img->type =IMAGE_TYPE_64KC;
	}
#else
	if(!SKY_GET_SKVMCOLORENV()){
		img->size= img->width * img->height;
		img->buf = GetScreenBuffer();
		img->transcolor = 0x01;
		img->type =IMAGE_TYPE_256C;
	}
	else{
		img->size= 2*img->width * img->height;
		img->buf = GetScreenBuffer();
		img->transcolor = 0x08;
		img->type =IMAGE_TYPE_64KC;
	}
#endif
//#endif
	img->mask = 0;
	img->maskbuf = NULL;

	return (Image *)img;
}


/*************************************************************
* function: imageDestroyMain
* desc: 	if you create main image by imageCreate(),
*               you must call imageDestroy().
* input:	im
* return:	none
*checked by tortol
&&&&&&&&&
**************************************************************/
void imageDestroyMain(Image *im /* main Image */)
{
    EQS_Free((VOID**)&im);
}
/*************************************************************
* function: imageSize
* desc: 	size of a new image structure
*			set mask to 1 for transparency, this allocates an 
*			extra array for bitmap mask area
* input:	with, height, transparency
* return:	size of ...
*checked by tortol
**************************************************************/
int imageSize(int w, int h, int mask)
{
    int data_size, mask_size;
	if(image_create_mode==IMAGE_TYPE_4G){
		data_size = ((w+3)>>2)*h;
		mask_size = ((w+7)>>3)*h;
/*
		mask_size = (w*h)%8 ? (w*h>>3)+1 :(w*h>>3);
		data_size = (w*h)%4 ? (w*h>>2)+1 :(w*h>>2);
*/  return sizeof(XImage)+(mask? data_size+mask_size:data_size);
	}
	else if(image_create_mode==IMAGE_TYPE_256C)
		return sizeof(XImage)+h*w;
	else return sizeof(XImage)+2*h*w;
}

/*************************************************************
* function: imageSetup
* desc: 	setup extended date except Image structure
* input:	image, mask
* return:	none
*checked by tortol
**************************************************************/
void imageSetup(Image *im, int mask)
{                              
    if( im ) {
		register int i_temp;
		XImage *img = (XImage *)im; 
	    img->type = image_create_mode;
		i_temp = img->width*img->height;
		if (img->type == IMAGE_TYPE_4G) { /* 4 grey-scale */
			img->size= ((img->width+3)>>2)*img->height;
//			img->size = i_temp%4 ? (i_temp>>2)+1 :(i_temp>>2);
			img->buf = (BYTE *)(img + 1);
			img->mask = mask;
			img->maskbuf = mask? img->buf + img->size: NULL;
		}
		else if (img->type == IMAGE_TYPE_256C) { /* 256 */
				img->size=  i_temp;
				img->buf = (BYTE *)(img + 1);
				img->mask = mask;
		}
		else if (img->type == IMAGE_TYPE_64KC) { /* 64K*/
				img->size=  i_temp<<1;
				img->buf = (BYTE *)(img + 1);
				img->mask = mask;
		}
    }
}

/*****************************************************
* function  : refreshLCD
* desc  : update the specified area of the lcd device
*     width the given image 
*     THE IMAGE MUST HAVE THE SAME WIDTH AND HEIGHT
*     WITH THE LCD DEVICE
* parameter : image, rectagle(x, y, w, h)
* return  : nothing
******************************************************/
void refreshLCD(Image *im /*main image*/, int x, int y, int w, int h )
{
    int x2 = x+w-1;
    int y2 = y+h-1;
    UINT32 width,height,imagearea,lcdarea;
	g_img = (XImage*)im;

/*	if (SKVM_IS_FIRST_REFRESH())
	{
		if(!SKY_GET_SKVMCOLORENV()){  
			SKY_FillScreenImage();
//			SKY_FillScreenOneColor(0xFFFF);
			SKY_256DisplayStart();
		}
		DrawAnnunciator(FALSE);
		SKVM_SET_FIRST_REFRESH(FALSE);
	}
*/ 
	if (SKVM_IS_FIRST_REFRESH())
	{
		if(!SKY_GET_SKVMCOLORENV()){  
			SKY_FillScreenImage();
//			SKY_FillScreenOneColor(0xFFFF);
			SKY_256DisplayStart();
		}
		DrawAnnunciator(FALSE);
		SKVM_SET_FIRST_REFRESH(FALSE);
	}

    /* boundary */
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x2 >= g_img->width) x2 = g_img->width-1;
    if (y2 >= g_img->height) y2 = g_img->height-1;
    width = x2 - x + 1;
    height = y2 - y + 1;
    imagearea = width*height;
	lcdarea = g_img->width*g_img->height;
	if (imagearea) 
	{
//	    XSIG_SKT_SET(REFRESH_LCD_XSIG);
		if(!SKY_GET_SKVMCOLORENV()){  
			if (imagearea > (lcdarea/2)) 
				SKY_256ColorDisplay((BYTE *)GetScreenBuffer());
//				SKY_RefreshDisplay_256C();
			else
				SKY_256UpdatePartialDisplay((BYTE*)g_img->buf+g_img->width*y,
												y+SKY_ANNUNCIATORS_HEIGHT,
												y2+SKY_ANNUNCIATORS_HEIGHT);
//				SKY_RefreshPartialDisplay_256C((BYTE*)g_img->buf+g_img->width*y, y+SKY_ANNUNCIATORS_HEIGHT,
//												y2+SKY_ANNUNCIATORS_HEIGHT);
		}
		else {
			if (imagearea > (lcdarea/2)) 
				SKY_RefreshDisplay();
			else 
				if (width == g_img->width)
					SKY_RefreshDisplayLine((BYTE*)g_img->buf+(g_img->width*y+x)*2,y+SKY_ANNUNCIATORS_HEIGHT,y2+SKY_ANNUNCIATORS_HEIGHT);
			else 
				SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), x, y+SKY_ANNUNCIATORS_HEIGHT,
										x2, y2+SKY_ANNUNCIATORS_HEIGHT);
		}
	}
	return;

}

void DrawAnnunciator(BOOL f){
	COLOR OldBackColor,OldForeColor;
	if(!SKY_GET_SKVMCOLORENV()){  
		if(SKY_CheckAnnunciators()||SKVM_IS_FIRST_REFRESH()){
			OldBackColor = EQS_GetBackColor();
			OldForeColor = EQS_GetForeColor();
			EQS_SetBackColor(0xFFFF);
			EQS_SetForeColor(0x0000);
			SKY_Draw256Annunciators();
			if (f)
				SKY_RefreshPartialDisplay_256C((BYTE*)GetScreenBuffer(), 0, SKY_ANNUNCIATORS_HEIGHT-1);
		}
	}
	else {
		if(SKY_CheckAnnunciators()||SKVM_IS_FIRST_REFRESH()){
			SKY_DrawAnnunciators();
			if (f)
				SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), 0, 0,
										SKY_SCREEN_WIDTH-1, SKY_ANNUNCIATORS_HEIGHT-1);
		}	
	}
	return;
}

/*************************************************************
* function: imageClear
* desc:         fill the image with the given color
*                       image operation mode is given for special effects
*                       such as fade in/out 2000/12/3
* input:        image, color, mode 
* return:       nothing 
* checked by tortol...
**************************************************************/
void imageClear( Image *im, PIXEL col32, BYTE mode )
{
	/* 8bit color를 사용하는 경우 하나의 byte가 하나의 픽셀을 나타내게 되므로 
	색을 넣을때도 하나의 col8값을 그대로 써주면. 하나의 픽셀을 이용할 수 있다. 
	아래는 그러한 방식으로 작성하였다. 
	만일 24bit or 16bit의 color를 사용하게 되면 하나의 픽셀의 크기가 변하게 되므로 이를 
	보정해주어야 한다.
	해당 부분 
	*/
//	unsigned int i;
	register UINT32 i;
	register UINT16* buf16;
	XImage *img = (XImage *)im;
	BYTE col8;
	UINT16 col16;

	img->clipx1 = 0; 
	img->clipx2 = img->width-1; 
	img->clipy1 = 0; 
	img->clipy2 = img->height-1;

	// 32-> num of lcd color 
	if (img->type == IMAGE_TYPE_4G) { /* 4 grey-scale */
	/* 4gray인 경우 채워야 하는 2bit color로 변환후 1)
	이를 8bit 값에 모두 채운다. 2)
	그렇게 만들어진 8bit정보를 1byte 마다 채워 넣는다. 3)
	*/
		BYTE *data = img->buf;
		BYTE gray;
		CLR32TOLCDGRAY2(col32, gray);  //1)
		gray= b4Gray[gray]; //2)
		switch( mode ) {
		case IMG_COPY: //3)
			memset( img->buf, gray, img->size );
/*			if(img->mask) 
				memset(img->maskbuf, 0, img->size);
*/
			break;
		case IMG_AND: //3)
			for(i=0;i<img->size;i++) {
				data[i] &= gray;
			}
			break;
		case IMG_OR: //3)
			for(i=0;i<img->size;i++) {
				data[i] |= gray;
			}
			break;
		case IMG_XOR: //3)
			for(i=0;i<img->size;i++) {
				data[i] ^= gray;
			}
			break;
		}
	}
	else if (img->type == IMAGE_TYPE_256C) { /* 256 scale */
		CLR32TOCLR8(col32, col8);
		switch( mode )
		{
			case IMG_COPY:
				memset(img->buf, col8, img->size);
				break;
			case IMG_AND:
				for(i=0;i<img->size;i++) {
					// ********
					img->buf[i] &= col8;
				}
				break;
			case IMG_OR:
				for(i=0;i<img->size;i++) {
					// ********
					img->buf[i] |= col8;
				}
				break;
			case IMG_XOR:
				for(i=0;i<img->size;i++) {
					// ********
					img->buf[i] ^= col8;
				}
				break;
		}
	}
	else{
		CLR32TOCLR16(col32, col16);
		buf16 = (UINT16 *)img->buf;
		switch( mode )
		{
			case IMG_COPY:
				memset(buf16, col16, img->size);
				break;
			case IMG_AND:
				for(i=0;i<img->size>>1;i++) {
					// ********
					buf16[i] &= col16;
				}
				break	;
			case IMG_OR:	
				for(i=0;i<img->size>>1;i++) {
					// ********
					buf16[i] |= col16;
				}
				break;
			case IMG_XOR:
				for(i=0;i<img->size>>1;i++) {
					// ********
					buf16[i] ^= col16;
				}
				break;
		}
	}
	return ; 
}

/*************************************************************
* function: imageInvert
* desc:      invert color of the specified area of the given image
*    		CLIPPING VALUES OF THE IMAGE MUST BE CONSIDERED 
* input:    image, rect( x, y, width, height ) 
* return:    nothing	
**************************************************************/
void 	imageInvert( Image *im, int x, int y, int w, int h  )
{
	int yOffset,i, j, x2, y2;
    XImage *img = (XImage *)im;
	UINT16 *buf16 = (UINT16 *)img->buf;
	LOCAL INT16 wBytePos , wByteEPos;
	LOCAL INT8 nBitPos, nBitEPos;
	BYTE ucResult ;

  	if (w<= 0 || h<= 0) return;
    x2 = x+w-1;
    y2 = y+h-1;

    ROBUSTCLIP(img);
    if (x < img->clipx1) x = img->clipx1;
    if (y < img->clipy1) y = img->clipy1;
    if (x2 > img->clipx2) x2 = img->clipx2;
    if (y2 > img->clipy2) y2 = img->clipy2;
    if (x2 - x + 1<= 0 || y2 - y + 1<= 0) return;
	if (img->type == IMAGE_TYPE_4G) { /* 4 grey-scale */
		/* 
			이미지를 뒤집는 것.. 
			해당 이미지의 위치를 계산 
			byte의 시작이 아니면 우선 첫 바이트를 뒤집은 후 시작 비트만큼 >> 하고 다시 << 하여 
			bit의 시작이 될때 까지는 해당 값을 & 연산 하여 값을 뽑아 내고
			bit의 시작부터  byte의 끝까지는 해당 값을 XOR 한후 & 하여 값을 뽑아 낸다. 
			BYTE내에 서는 직접 ~ 연산을 수행하고 
			마지막 BYTE는 마지막 bit가 byte의 끝일 때는  직접 ~ 현산 을 수행하고
			              끝이 아닐 때는 0부터 마지막 bit까지는 해당 값을 xor 후 & 
						  마지막 bit 다음 부터 byte의 끝까지는 & 연산으로 값을 뽑아낸다. 
		*/
		nBitPos = BIT_POS_4G(x);
		nBitEPos = BIT_POS_4G(x2);
	
		for(yOffset=y; yOffset<=y2; yOffset++){
			wBytePos = TOT_BYTE_POS_4G(x, yOffset, img->width);
			wByteEPos= TOT_BYTE_POS_4G(x2,yOffset, img->width);

			if(nBitPos){ // 시작 비트가 byte 중간 부터 시작할때. 
				ucResult = ~img->buf[wBytePos];
				ucResult = (ucResult>>nBitPos)<<nBitPos;
				img->buf[wBytePos] = (img->buf[wBytePos]<<(8-nBitPos))<<(8-nBitPos);
				img->buf[wBytePos] |= ucResult; 
			}
			else
				img->buf[wBytePos] = ~img->buf[wBytePos];

			// 상관없는 가운데 것들.. 
			for(i=wBytePos+1; i< wByteEPos; i++){
				img->buf[i] = ~img->buf[i];
			}
			
			// 마지막 바이트의 마지막 비트가 byte의 끝이 아닐때.. 
			if(nBitEPos!=6){// 마지막 바이트의 마지막 비트가 byte의 끝이 아닐때.. 
				ucResult = ~img->buf[wByteEPos];
				ucResult = (ucResult<<(6-nBitEPos))>>(6-nBitEPos);
				img->buf[wByteEPos] = (img->buf[wByteEPos]>>(2+nBitEPos))<<(2+nBitEPos);
				img->buf[wByteEPos] |= ucResult; 
			}
		}
	}
	else if (img->type == IMAGE_TYPE_256C) { // 256 scale 
		for(i=y; i<=y2; i++){
			for(j=x;j<=x2;j++){
				img->buf[img->width*i+j] = ~img->buf[img->width*i+j];
			}
		}
	}
	else {   // 64KC 인 경우 
			// lcd 가 64KC
		for(i=y; i<=y2; i++){
			for(j=x;j<=x2;j++)
					buf16[img->width*i+j] = ~buf16[img->width*i+j];
		}
	}
}

/*************************************************************
* function: imageGetPixel
* desc:  	return the pixel color value of the image at the
*			specified position	
* input:	image, pixel position( x, y ) 
* return:	pixel color	
 *checked by tortol 
**************************************************************/
PIXEL	imageGetPixel( Image *im, int x, int y )
{
	PIXEL	pix;
	XImage *img = (XImage *)im;	
	UINT16 *buf16 = (UINT16 *)img->buf;
	int	off = y*img->width + x;
	LOCAL INT16 wBytePos;
	LOCAL INT8 nBitPos;
	BYTE bResult;
	if (img->type == IMAGE_TYPE_4G) { /* 4 grey-scale */
		/*
		해당되는 위치의 bit를 찾고 그 비트 값만 읽어 낸후 
		그 비트값을 비트 위치만큼 오른쪽으로 시프트 한다. 
		그값을 32로 확장한값을 넘긴다. 
		*/
		wBytePos =TOT_BYTE_POS_4G(x,y,img->width);
		nBitPos = BIT_POS_4G(x);
		GRAY2TOCLR32(bResult = ((img->buf[wBytePos]&m_abMask4G[nBitPos])>>nBitPos),pix); 
	}
	else if (img->type == IMAGE_TYPE_256C){ // 256 scale
		CLR8TOCLR32(img->buf[off], pix);
	}
	else{  // 64KC
		CLR16TOCLR32(buf16[off], pix);
	}
  	return pix;
}

/*************************************************************
* function: imageGetPixelEx
* desc:         return the pixel color and mask value of the image
*                       at the specified position
* input:        image, pixel position( x, y ), mask position pointer
* return:       pixel color     
 *checked by tortol 
**************************************************************/
PIXEL imageGetPixelEx( Image *im, int x, int y, BYTE *mval )
{
	PIXEL	pix;
	XImage *img = (XImage *)im;	
	UINT16 *buf16 = (UINT16 *)img->buf;
	int	off = y*img->width + x;
	LOCAL INT16 wMaskBytePos, wBytePos;
	LOCAL INT8 nMaskBitPos, nBitPos;
	BYTE  bResult;
	if (img->type == IMAGE_TYPE_4G) { /* 4 grey-scale */
		/*
		해당되는 위치의 bit를 찾고 그 비트 값만 읽어 낸후 
		그 비트값을 비트 위치만큼 오른쪽으로 시프트 한다. 
		그값을 32로 확장한값을 넘긴다. 
		해당 mask bit도 적절히 값을 세팅 한다. 
		*/
		wBytePos =TOT_BYTE_POS_4G(x,y,img->width);
		nBitPos = BIT_POS_4G(x);
		GRAY2TOCLR32(bResult = ((img->buf[wBytePos]&m_abMask4G[nBitPos])>>nBitPos),pix); 
		wMaskBytePos = TOT_BYTE_POS_MASK(x,y,img->width);
		nMaskBitPos = BIT_POS_MASK(x);
		if(img->mask&&(img->maskbuf[wMaskBytePos]&bitMask[nMaskBitPos]))
			*mval = 0x1;
		else *mval = 0x0; 
	}
	else if (img->type == IMAGE_TYPE_256C) { /* 256 scale */
		CLR8TOCLR32(img->buf[off],pix);
		if(img->mask&&(img->buf[off]==img->transcolor))
			*mval = 0x1 ;
		else *mval = 0x0;
	}
	else { //64KC
		CLR16TOCLR32(buf16[off],pix);
		if(img->mask&&(buf16[off]==img->transcolor))
			*mval = 0x1 ;
		else *mval = 0x0;
	}
  	return pix;
}

/*************************************************************
* function: imageSetPixel
* desc:  	set the pixel color value of the image at the
*			specified position	
* input:	image, pixel position( x, y ), pixel color
* return: 	nothing	
* checked by tortol 
**************************************************************/
void	imageSetPixel( Image *im, int x, int y, PIXEL col32 ,BYTE mode)
{
	// 
	XImage *img = (XImage *)im;	
	int	off = y*img->width + x;
	BYTE col8, gray;
	UINT16 col16, *buf16 = (UINT16 *)img->buf;
	LOCAL INT16 wBytePos;
	LOCAL INT8 nBitPos;

	if(OUTCLIP(img,x, y)) return; 
	if (img->type == IMAGE_TYPE_4G) { /* 4 grey-scale */
		/*
		해당 컬러를 4gray로 바꾼 후 그 값을 해당 bit위치만큼 이동하여 
		알맞는 위치에 값을 넣는다. 
		*/
		CLR32TOLCDGRAY2(col32, gray);
		wBytePos = TOT_BYTE_POS_4G(x, y,img->width);
		nBitPos = BIT_POS_4G(x);

		switch( mode ) 
		{
		case IMG_COPY:
			img->buf[wBytePos] &= m_abNotMask4G[nBitPos];
			img->buf[wBytePos] |= (gray<<nBitPos);
			break;
		case IMG_AND:
			gray <<= nBitPos ;
			gray |= m_abNotMask4G[nBitPos] ;
			img->buf[wBytePos] &= gray;
			break;
		case IMG_OR:
			img->buf[wBytePos] |= gray<<nBitPos;
			break;
		case IMG_XOR:
			img->buf[wBytePos] ^= gray<<nBitPos;
			break;
		}
	}
	else if(img->type == IMAGE_TYPE_256C) { /* 256 scale */ 
		CLR32TOCLR8(col32, col8);
		switch( mode ) 
		{
		case IMG_COPY:
			img->buf[off] = col8;
			break;
		case IMG_AND:
			img->buf[off] &= col8;
			break;
		case IMG_OR:
			img->buf[off] |= col8;
			break;
		case IMG_XOR:
			img->buf[off] ^= col8;
			break;
		}
	}
	else { // 64K scale
		CLR32TOCLR16(col32, col16);
		switch( mode ) 
		{
		case IMG_COPY:
			buf16[off] = col16;
			break;
		case IMG_AND:
			buf16[off] &= col16;
			break;
		case IMG_OR:
			buf16[off] |= col16;
			break;
		case IMG_XOR:
			buf16[off] ^= col16;
			break;
		}
	}
  	return ;
}
/*************************************************************
* function: imageSetPixelMask(added 2000 01 16)
* desc:      set the pixel color value of the image at the
*               specified position      
* input:    image, pixel position( x, y ), mask bit
* return:     nothing   
**************************************************************/
void   imageSetPixelMask( Image *im, int x, int y, BYTE bmask )
{
  	int	off;
	LOCAL INT16 wMaskBytePos;
	LOCAL INT8 nMaskBitPos;
	XImage *img = (XImage *)im;	
	UINT16 *buf16=(UINT16 *)img->buf;
    off = y*img->width+x;

//	if(bmask){
	if (img->type == IMAGE_TYPE_4G) { /* 4 grey-scale */
		wMaskBytePos = TOT_BYTE_POS_MASK(x, y,img->width);
		nMaskBitPos =  BIT_POS_MASK(x);
/*		wMaskBytePos = BYTE_POS_BW(INDEX_ON_IMG(x, y,img->width));
		nMaskBitPos = BIT_POS_BW(INDEX_ON_IMG(x, y,img->width));
*/		if(img->mask){
			if(bmask)
				img->maskbuf[wMaskBytePos] |= bitMask[nMaskBitPos];
			else 
				img->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos];
		}
	}
	else if (img->type==IMAGE_TYPE_256C){ // 256 scale
		if(bmask) 
			img->buf[off] = img->transcolor;
	}
	else {   // 64KC
		if(bmask) buf16[off] = img->transcolor; 
	}
	return;  
}
/*************************************************************
* function: imageSetPixelEx
* desc:        set the pixel color value and bit mask of the image
*                       at the specified position      
*                       'mode' value is given for additional operation as
*                       follows:
*                       IMG_COPY : just overwrite the color value
*                       IMG_AND : bitwise and operation  
*                       IMG_OR  : bitwise or operation
*                       IMG_XOR : bitwise xor operation
* input:        image, pixel position( x, y ) 
* return:       pixel color     
 *checked by tortol 
**************************************************************/
void   imageSetPixelEx( Image *im, int x, int y, PIXEL col32, BYTE bmask, BYTE mode )
{
	int	off;
	XImage *img = (XImage *)im;	
	BYTE col8, gray;
	UINT16 col16, *buf16=(UINT16 *)img->buf;
	LOCAL INT16 wBytePos;
	LOCAL INT8 nBitPos;
	LOCAL INT16 wMaskBytePos;
	LOCAL INT8 nMaskBitPos;
	if(OUTCLIP(img,x, y)) return; 
	off	= y * img->width + x;

	if (img->type == IMAGE_TYPE_4G) { /* 4 grey-scale */

		CLR32TOLCDGRAY2(col32, gray);
		wBytePos =	   TOT_BYTE_POS_4G(x, y,img->width);
		nBitPos =	   BIT_POS_4G(x);
		wMaskBytePos = TOT_BYTE_POS_MASK(x, y,img->width);
		nMaskBitPos =  BIT_POS_MASK(x);
/*
		wBytePos = BYTE_POS_4G(INDEX_ON_IMG(x, y,img->width));
		nBitPos = BIT_POS_4G(INDEX_ON_IMG(x, y,img->width));
		wMaskBytePos = BYTE_POS_BW(INDEX_ON_IMG(x, y,img->width));
		nMaskBitPos = BIT_POS_BW(INDEX_ON_IMG(x, y,img->width));
*/		if(img->mask){
			if(bmask)
				img->maskbuf[wMaskBytePos] |= bitMask[nMaskBitPos];
			else	 
				img->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos];
		}
		switch( mode ) 
		{
		case IMG_COPY:
			img->buf[wBytePos] &= ~m_abMask4G[nBitPos];
			img->buf[wBytePos] |= (gray<<nBitPos);
			break;
		case IMG_AND:
			gray <<= nBitPos ;
			gray |= ~m_abMask4G[nBitPos] ;
			img->buf[wBytePos] &= gray;
			break;
		case IMG_OR:
			img->buf[wBytePos] |= gray<<nBitPos;
			break;
		case IMG_XOR:
			img->buf[wBytePos] ^= gray<<nBitPos;
			break;
		}
	}
	else if(img->type == IMAGE_TYPE_256C){   // 256 scale 
		CLR32TOCLR8(col32, col8);
		switch( mode )
		{
			case IMG_COPY:
				img->buf[off] = col8;
				break;
			case IMG_AND:
				img->buf[off] &= col8;
				break;
			case IMG_OR:
				img->buf[off] |= col8;
				break;
			case IMG_XOR:
				img->buf[off] ^= col8;
				break;
		}
	}
	else{			// 64KC
		CLR32TOCLR16(col32, col16); 
		switch( mode )
		{
			case IMG_COPY:
				buf16[off] = col16;
				break;
			case IMG_AND:
				buf16[off] &= col16;
				break;
			case IMG_OR:
				buf16[off] |= col16;
				break;
			case IMG_XOR:
				buf16[off] ^= col16;
				break;
		}
	}
	return ;

}
/*************************************************************
* function: imageDrawLine
* desc:  	draw a line in the image from (x1,y1) to (x2,y2)
*			the line includes vertical and horizontal lines.
*			BRESENHAM midpoint algorithm is suggested for
*			implementation. 'imageSetPixel' is used in the 
*			drawing loop. 
*			CLIPPING VALUES OF THE IMAGE MUST BE CONSIDERED 
* input:	image, x1, y1, x2, y2, color
* return: 	nothing	
 *checked by tortol 
**************************************************************/
void	imageDrawLine( Image *im, int xStart, int yStart, int xEnd, int yEnd, PIXEL col32 )
{
/*************************************/
    COORD dx, dy;
    INT16 nDy2, nDyDx2;
    COORD x, y, xEndPos;
    //COORD xTemp, yTemp;
    INT16 nPos, j;
    /* 모든 사선을 그리기 위해 추가시킨 변수들 - Sung-ryong */
    INT16 nDx2, nDxDy2;				
    INT16 nPos1;					
    COORD yEndPos;
	BYTE col8, gray4;
	XImage *img = (XImage *)im;
	UINT16 col16, *buf16 = (UINT16 *)img->buf;
	BYTE trans8, val8, *buf8 = img->buf ;
	LOCAL UINT16 wBytePos, wByteEndPos;
	LOCAL UINT8 nBitPos, nBitEndPos;

	if (img->type == IMAGE_TYPE_4G)
		CLR32TOLCDGRAY2(col32,gray4);
	else if(img->type == IMAGE_TYPE_256C)
		CLR32TOCLR8(col32,col8);
	else CLR32TOCLR16(col32,col16);

	
    /* 여기까지 */

    dx = ABSO(xStart - xEnd);
    dy = ABSO(yStart - yEnd);
	
	if (xStart < im->clipx1) xStart = im->clipx1;
	if (yStart < im->clipy1) yStart = im->clipy1;
	if (xEnd > im->clipx2) xEnd = im->clipx2;
	if (yEnd > im->clipy2) yEnd = im->clipy2;
		
    if(dx == 0) /* vertical line */
    {
//		UINT16	l_uMaskByteWidth = BYTE_POS_MASK(img->width+7);
		y = (yStart - yEnd) > 0 ? yEnd : yStart;
        yEnd = (yStart - yEnd) > 0 ? yStart : yEnd;
		if (img->type == IMAGE_TYPE_4G){ // 4 grey-scale 
	   		UINT16	l_uImgByteWidth = BYTE_POS_4G(img->width+3);
			buf8 = img->buf + TOT_BYTE_POS_4G(xStart,y,img->width);
			nBitPos= BIT_POS_4G(xStart);
			while(y <= yEnd){
				if(!OUTCLIP(img, xStart,y)){
					*buf8 &= m_abNotMask4G[nBitPos];
					*buf8 |= (gray4<<nBitPos);
				}
				buf8 += l_uImgByteWidth;
				y++;
			}
		}
		else if(img->type == IMAGE_TYPE_256C){
			buf8 = img->buf + y*img->width + xStart;
			while(y <= yEnd){
				if(!OUTCLIP(img, xStart,y)){
					*buf8 = col8; 
					buf8 += img->width;
				}
				y++;
			}
		}
		else {
			buf16 = (UINT16 *)img->buf + y*img->width + xStart;
			while(y <= yEnd){
				if(!OUTCLIP(img, xStart,y)){
					*buf16 = col16; 
					buf16 += img->width;
				}
				y++;
			}
		}
    }
    else if(dy == 0) /* horizontal line */
    {
        x = (xStart - xEnd) > 0 ? xEnd : xStart;
        xEnd = (xStart - xEnd) > 0 ? xStart : xEnd;
		if (x < img->clipx1) x = img->clipx1;
		if (xEnd > img->clipx2) xEnd = img->clipy1;

		if (img->type == IMAGE_TYPE_4G){
			wBytePos = TOT_BYTE_POS_4G(x, yStart,img->width);
			nBitPos = BIT_POS_4G(x);
			wByteEndPos = TOT_BYTE_POS_4G(xEnd, yStart,img->width);
			nBitEndPos = BIT_POS_4G(xEnd);
			buf8=img->buf+wBytePos;
			trans8 = 0;
			val8=0;
			col8= b4Gray[gray4];
			if((8-nBitPos)<((xEnd-x+1)<<1)){
				*buf8 &= ~(0XFF<<nBitPos);
				*buf8 |= (col8&(0xFF<<nBitPos));
				buf8++;
			}
			else {
				*buf8 &= ~((0xFF<<nBitPos)& (0xFF>>(6-nBitEndPos)));
				*buf8 |= (col8&((0xFF<<nBitPos)& (0xFF>>(6-nBitEndPos))));
				buf8++;
				wByteEndPos = 0;
			}

			// 중간 픽셀

			for(j=wBytePos+1;j<wByteEndPos;j++){
				*buf8 = col8;
				buf8++;
			}
			if(wByteEndPos){
				*buf8 &= ~(0xFF>>(6-nBitEndPos));
				*buf8 |= (col8>>(6-nBitEndPos));
			}
		}
		else if(img->type == IMAGE_TYPE_256C){
    		while(x <= xEnd){
				if(!OUTCLIP(img, x,yStart))
					IMAGESETPIXEL(img->buf,img,x,yStart,col8,IMG_COPY);
				x++;
			}
		}
		else{
    		while(x <= xEnd){
				if(!OUTCLIP(img, x,yStart))
						IMAGESETPIXEL(buf16,img,x,yStart,col16,IMG_COPY);
				x++;
			}
		}
    }
    else /* slanted line */
    {
        nDy2 = dy * 2;
        nDyDx2 = (dy - dx) * 2;
        nPos = (2 * dy) - dx;

        /* 완벽한 라인을 그리기 위한 변형 - Sung-ryong */
		nDx2 = dx * 2;
		nDxDy2 = (dx - dy) * 2;
		nPos1 = (2 * dx) - dy;
        
        
        /* 변경시킨 라인 그리는 부분. 제대로 된 그림이 그려진다. - Sung-ryong */
		if (dx >= dy)
		{
			/* Determine which point to use as start, which as end */
			if (xStart > xEnd)
				x = xEnd, y = yEnd, xEndPos = xStart;
			else
				x= xStart, y = yStart, xEndPos = xEnd;

			if(!OUTCLIP(img, x,y)){
				if (img->type == IMAGE_TYPE_4G){ /* 4 grey-scale */
					wBytePos = TOT_BYTE_POS_4G(x, y,img->width);
					nBitPos = BIT_POS_4G(x);
					img->buf[wBytePos] &= m_abNotMask4G[nBitPos];
					img->buf[wBytePos] |= (gray4<<nBitPos);

				//	IMAGESETPIXEL4G(img,x,y,gray4,IMG_COPY);
				}
				else if(img->type == IMAGE_TYPE_256C)
					IMAGESETPIXEL(img->buf,img,x,y,col8,IMG_COPY);
				else 
					IMAGESETPIXEL(buf16,img,x,y,col16,IMG_COPY);
			}

			if (img->type==IMAGE_TYPE_4G){
				while(x < xEndPos){
					x++;
					if(nPos < 0){
						nPos += nDy2;
					}
					else{
						if (xStart > xEnd){
							if((yEnd - yStart) > 0)
								y--;
							else
								y++;
						}
						else{
							if((yEnd - yStart) > 0)
								y++;
							else
								y--;
						}
						nPos += nDyDx2;
					}
					if(!OUTCLIP(img, x,y)){
						wBytePos = TOT_BYTE_POS_4G(x, y,img->width);
						nBitPos = BIT_POS_4G(x);
						img->buf[wBytePos] &= m_abNotMask4G[nBitPos];
						img->buf[wBytePos] |= (gray4<<nBitPos);
					}
				}
			}
			else if(img->type == IMAGE_TYPE_256C){
				while(x < xEndPos)
				{
					x++;
					if(nPos < 0){
						nPos += nDy2;
					}
					else{
						if (xStart > xEnd){
							if((yEnd - yStart) > 0)
								y--;
							else
								y++;
						}
						else{
							if((yEnd - yStart) > 0)
								y++;
							else
								y--;
						}
						nPos += nDyDx2;
					}
					if(!OUTCLIP(img, x,y))
						IMAGESETPIXEL(img->buf,img, x,y,col8,IMG_COPY);
				}
			}
			else{
				while(x < xEndPos)
				{
					x++;
					if(nPos < 0){
						nPos += nDy2;
					}
					else{
						if (xStart > xEnd){
							if((yEnd - yStart) > 0)
								y--;
							else
								y++;
						}
						else{
							if((yEnd - yStart) > 0)
								y++;
							else
								y--;
						}
						nPos += nDyDx2;
					}
					if(!OUTCLIP(img, x,y))
						IMAGESETPIXEL(buf16,img,x,y,col16,IMG_COPY);
				}
			}

		}
		else
		{
			/* Determine which point to use as start, which as end */
			if (yStart > yEnd)	y = yEnd, x = xEnd, yEndPos = yStart;
			else y = yStart, x = xStart, yEndPos = yEnd;
			if(!OUTCLIP(img, x,y)){
				if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				{ /* 4 grey-scale */
					wBytePos = BYTE_POS_4G(INDEX_ON_IMG(x, y,img->width));
					nBitPos = BIT_POS_4G(INDEX_ON_IMG(x, y,img->width));
					img->buf[wBytePos] &= m_abNotMask4G[nBitPos];
					img->buf[wBytePos] |= (gray4<<nBitPos);
				}
				else if(img->type == IMAGE_TYPE_256C)
					IMAGESETPIXEL(img->buf, img, x,y,col8,IMG_COPY);
				else 
					IMAGESETPIXEL(buf16,img, x,y,col16,IMG_COPY);
			}
			if (img->type == IMAGE_TYPE_4G){
					while(y < yEndPos){
						y++;
						if(nPos1 < 0){
							nPos1 += nDx2;
						}
						else{
							if (yStart > yEnd){
								if((xEnd - xStart) > 0)
									x--;
								else
									x++;
							}
							else{
								if((xEnd - xStart) > 0)
									x++;
								else
									x--;
							}
							nPos1 += nDxDy2;
						}
						//DevSetPixel(x, y, GET_FORE_COLOR());
						if(!OUTCLIP(img, x,y)){
							wBytePos = BYTE_POS_4G(INDEX_ON_IMG(x, y,img->width));
							nBitPos = BIT_POS_4G(INDEX_ON_IMG(x, y,img->width));
							img->buf[wBytePos] &= m_abNotMask4G[nBitPos];
							img->buf[wBytePos] |= (gray4<<nBitPos);
						}
					}
			}
			else if(img->type == IMAGE_TYPE_256C){
				while(y < yEndPos){
					y++;
					if(nPos1 < 0){
						nPos1 += nDx2;
					}
					else{
						if (yStart > yEnd){
							if((xEnd - xStart) > 0)
								x--;
							else
								x++;
						}
						else{
							if((xEnd - xStart) > 0)
								x++;
							else
								x--;
						}
						nPos1 += nDxDy2;
					}
					//DevSetPixel(x, y, GET_FORE_COLOR());
					if(!OUTCLIP(img, x,y))
						IMAGESETPIXEL(img->buf,img,x,y,col8,IMG_COPY);
				}
			}
			else{
				while(y < yEndPos){
					y++;
					if(nPos1 < 0){
						nPos1 += nDx2;
					}
					else{
						if (yStart > yEnd){
							if((xEnd - xStart) > 0)
								x--;
							else
								x++;
						}
						else{
							if((xEnd - xStart) > 0)
								x++;
							else
								x--;
						}
						nPos1 += nDxDy2;
					}
					//DevSetPixel(x, y, GET_FORE_COLOR());
					if(!OUTCLIP(img, x,y))
						IMAGESETPIXEL(buf16,img,x,y,col16,IMG_COPY);
				}
			}
		}
    }
	return ; 
} 

#define _3D_SUPPORTED
#ifdef _3D_SUPPORTED
/*
 *	Routines Related to 3D Graphics
 */
extern	unsigned char*			zbuf;
extern	int zbufSize, zbufWidth, zbufHeight;
#define	ZINDEX(x,y)	((y)*zbufWidth + (x))
#define	PT3_SWAP(a,b,c,x,y,z,t)	t[0]=a,t[1]=b,t[2]=c,a=x,b=y,c=z,x=t[0],y=t[1],z=t[2]

/*************************************************************
* function: imageDrawLine3D
* desc:      draw a line in the image from (x1,y1,z1) to (x2,y2,z2)
*    		the line includes vertical and horizontal lines.
*    		BRESENHAM midpoint algorithm is suggested for
*    		implementation. 'imageSetPixel' is used in the 
*    		drawing loop.
*    		CLIPPING VALUES OF THE IMAGE AND Z-BUFFER MUST BE CONSIDERED 
* input:    image, x1, y1, x2, y2, color
* return:     nothing	
**************************************************************/
void    imageDrawLine3D( Image *im, int x1, int y1, int z1, int x2, int y2, int z2, PIXEL col32, int useZbuf )
{
    XImage	*img = (XImage*)im;
	UINT16 *orgbuf16 = (UINT16 *)img->buf;
    int		pixel_idx;
    int		set_pixel = 0, set_pixel_prev;
	BYTE col8, *buf8, gray4;
	UINT16 col16,*buf16;
	int w, h,off;
	int t[3],temp; // SWAP
	int dX, dY ,dZ, dX2, dY2, dZ2,  incrE, incrNE, Zincr, Zskip, D2, lbgn;
	int dY2_dX2, dZ2_dX2,dX2_dY2, dZ2_dY2, PX, PY, PZ, sx, sy, sz;
	
	int	z1inc;
    	
    	//--------------------------------------------------------
    	// DETERMINE "DIRECTIONS" TO INCREMENT
    	// X AND Y (REGARDLESS OF DECISION)
    	//---------------------------------------------------------
	int		IncrXY, IncrZ;


	if (img->type == IMAGE_TYPE_4G)
		CLR32TOLCDGRAY2(col32,gray4);
	else if(img->type == IMAGE_TYPE_256C)
		CLR32TOCLR8(col32,col8);
	else CLR32TOCLR16(col32,col16);
	  
    if ( x1 == x2 )
    {
    	ROBUSTCLIP(img);
    	/* clip x */
    	if (x1 < img->clipx1 || x1 > img->clipx2)
			return;

		if (y1 > y2) {
			temp = y1;
			y1 = y2;
			y2 = temp;
			temp = z1;
			z1 = z2;
			z2 = temp;
		}

		/* clip y */
		if (y1 == y2) {
			if (y1 < img->clipy1 || y1 > img->clipy2)
				return;
		}
		else if (y1 < img->clipy1 && y2 > img->clipy2) {
			z1inc = (img->clipy1 - y1) * (z2 - z1) / (y2 - y1);
			z2 = z1 + (img->clipy2 - y1) * (z2 - z1) / (y2 - y1);
			z1 += z1inc;
			y1 = img->clipy1;
			y2 = img->clipy2;
		}
		else if (y1 < img->clipy1) {
			z1 += (img->clipy1 - y1) * (z2 - z1) / (y2 - y1);
			y1 = img->clipy1;
		}
		else if (y2 > img->clipy2) {
			z2 = z1 + (img->clipy2 - y1) * (z2 - z1) / (y2 - y1);
			y2 = img->clipy2;
		}
		h = y2 - y1 + 1;
		if (h <= 0)
			return;

		if (y1 == y2) {	/* a point */
			w = img->width;
			off = y1*w+x1;
			buf8 = img->buf + off;
			buf16 = orgbuf16 + off;

			if (useZbuf) {
				pixel_idx = ZINDEX(x1, y1);
				if (z1 > z2)
					z2 = z1;
				if (z2 <= (int) zbuf[pixel_idx] || z2 > 255)
					return;
				zbuf[pixel_idx] = z2;
			}
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, x1,y1,gray4);
			else if(img->type == IMAGE_TYPE_256C) // 256 scale
				*buf8 = col8;
			else *buf16 = col16; 
			return;
		}

		if (useZbuf) {
			// line from (x1, y1, z1) to (x1, y1+h, z2)
		
			if (z1 > z2)
				Zincr = -1;
			else
				Zincr = 1;
			dY = h-1;
			dY2 = dY << 1;
			dZ = ABS(z2-z1);
			incrE = dZ << 1;
			Zskip = 0;
			if (dY > 0 && incrE > dY2) {
				Zskip = incrE / dY2;
				incrE %= dY2;
				dZ -= Zskip * dY;
				if (Zincr < 0)
					Zskip = -Zskip;
			}
			incrNE = incrE - dY2;
			D2 = incrE - dY;

			set_pixel_prev = 1;
			pixel_idx = ZINDEX(x1, y1);
			if (z1 <= (int) zbuf[pixel_idx] || z1 > 255)
				set_pixel_prev = 0;
			else
				zbuf[pixel_idx] = z1;
			lbgn = y1;

			while (dY--) {
				y1++;
				z1 += Zskip;
				if (D2 > 0) {
					z1 += Zincr;
					D2 += incrNE;
				}
				else
					D2 += incrE;

				set_pixel = 1;
				pixel_idx = ZINDEX(x1, y1);
				if (z1 <= (int) zbuf[pixel_idx] || z1 > 255)
					set_pixel = 0;
				else
					zbuf[pixel_idx] = z1;

				if (set_pixel != set_pixel_prev) {
					if (set_pixel)
						lbgn = y1;
					else {
						h = y1 - lbgn;
					//	_imageDrawVLine(img,x1,y1, h, col32);
						imageDrawLine((Image *)img,x1,lbgn,x1,lbgn+h, col32);
					}
					set_pixel_prev = set_pixel;
				}
			}
			if (set_pixel) {
				h = y1 - lbgn + 1;
				//_imageDrawVLine(img, x1, lbgn, h, col);
				imageDrawLine((Image *)img,x1,lbgn,x1,lbgn+h,col32);
			}
		}		
		else {
//			_jlog("not useZbuf : _imageDrawVLine");
			//_imageDrawVLine(img, x1, y1, h, col);
			imageDrawLine((Image *)img,x1,y1,x1,y1+h, col32);
			
		}
    }
    else if ( y1 == y2 )
    {
		ROBUSTCLIP(img);
		/* clip y */
    	if ( y1 < img->clipy1 || y1 > img->clipy2)
			return;

		if (x1 > x2) {
			temp = x1;
			x1 = x2;
			x2 = temp;
			temp = z1;
			z1 = z2;
			z2 = temp;
		}

		/* clip x */
		if (x1 < img->clipx1 && x2 > img->clipx2) {
			z1inc = (img->clipx1 - x1) * (z2 - z1) / (x2 - x1);
			z2 = z1 + (img->clipx2 - x1) * (z2 - z1) / (x2 - x1);
			z1 += z1inc;
			x1 = img->clipx1;
			x2 = img->clipx2;
		}
		else if (x1 < img->clipx1) {
			z1 += (img->clipx1 - x1) * (z2 - z1) / (x2 - x1);
			x1 = img->clipx1;
		}
		else if (x2 > img->clipx2) {
			z2 = z1 + (img->clipx2 - x1) * (z2 - z1) / (x2 - x1);
			x2 = img->clipx2;
		}
		w = x2 - x1 + 1;
		if (w <= 0)
			return;

		if (useZbuf) {
			// line from (x1, y1, z1) to (x1+w, y1, z2)
			if (z1 > z2)
				Zincr = -1;
			else
				Zincr = 1;
			dX = w-1;
			dX2 = dX << 1;
			dZ = ABS(z2-z1);
			incrE = dZ << 1;
			Zskip = 0;
			if (dX > 0 && incrE > dX2) {
				Zskip = incrE / dX2;
				incrE %= dX2;
				dZ -= Zskip * dX;
				if (Zincr < 0)
					Zskip = -Zskip;
			}
			incrNE = incrE - dX2;
			D2 = incrE - dX;

			set_pixel_prev = 1;
			pixel_idx = ZINDEX(x1, y1);
			if (z1 <= (int) zbuf[pixel_idx] || z1 > 255)
				set_pixel_prev = 0;
			else
				zbuf[pixel_idx] = z1;
			lbgn = x1;

			while (dX--) {
				x1++;
				z1 += Zskip;
				if (D2 > 0) {
					z1 += Zincr;
					D2 += incrNE;
				}
				else
					D2 += incrE;

				set_pixel = 1;
				pixel_idx = ZINDEX(x1, y1);
				if (z1 <= (int) zbuf[pixel_idx] || z1 > 255)
					set_pixel = 0;
				else
					zbuf[pixel_idx] = z1;

				if (set_pixel != set_pixel_prev) {
					if (set_pixel)
						lbgn = x1;
					else {
						w = x1 - lbgn;
						//_imageDrawHLine(img, x1, lbgn, w, col);
						imageDrawLine((Image *)img, x1, lbgn, x1+w,lbgn, col32);
					}
					set_pixel_prev = set_pixel;
				}
			}

			if (set_pixel) {
				w = x1 - lbgn + 1;
				//_imageDrawHLine(img, x1, lbgn, w, col);
				imageDrawLine((Image *)img, x1, lbgn, x1+w, lbgn,col32);
			}
		}
		else {
		//	_jlog("not useZbuf : _imageDrawHLine");
			//_imageDrawHLine(img, x1, y1, w, col);
			imageDrawLine((Image *)img,x1, y1, x1+2, y1, col32);
		}
    }
    else
    {
		dX = ABS(x2-x1);
		dY = ABS(y2-y1);
		dZ = ABS(z2-z1);
		dX2 = dX<<1;
		dY2 = dY<<1;
		dZ2 = dZ<<1;
		ROBUSTCLIP(img);
    	//----------------------------------------------------------
    	// DETERMINE INDEPENDENT VARIABLE
    	// (ONE THAT ALWAYS INCREMENTS BY 1 (OR -1) )
    	// AND INITIATE APPROPRIATE LINE DRAWING ROUTINE
    	// (BASED ON FIRST OCTANT ALWAYS).
    	// THE X AND Y'S MAY BE FLIPPED IF Y IS THE
    	// INDEPENDENT VARIABLE.
    	//----------------------------------------------------------
    	if (dX >= dY)
    	{	   

			dY2_dX2 = dY2 - dX2;// increment decision (for up)
			dZ2_dX2 = dZ2 - dX2;// increment decision (for up)
    		// decision variable start value
    		
			if (x1 > x2)
				PT3_SWAP(x1, y1, z1, x2, y2, z2, t);
			sx = x1;
			sy = y1;
			sz = z1;

			if (x1 > img->clipx2 || x2 < img->clipx1 )
				return;
			if (y1 > y2) {
				if (y2 > img->clipy2 || y1 < img->clipy1 )
					return;
				IncrXY = -1;
			}
			else {
				if (y1 > img->clipy2 || y2 < img->clipy1 )
					return;
				IncrXY = 1;
			}

			IncrZ = (z1 > z2) ? -1 : 1;

			if (x1 < img->clipx1) {
				x1 = img->clipx1;
				if (IncrXY > 0)
					y1 = sy + (dY2*(x1 - sx) + dX-1) / dX2;
				else
					y1 = sy - (dY2*(x1 - sx) + dX-1) / dX2;
				if (IncrZ > 0)
					z1 = sz + (dZ2*(x1 - sx) + dX-1) / dX2;
				else
					z1 = sz - (dZ2*(x1 - sx) + dX-1) / dX2;
			}
			if (x2 > img->clipx2) {
				x2 = img->clipx2;
				if (IncrXY > 0)
					y2 = sy + (dY2*(x2 - sx) + dX-1) / dX2;
				else
					y2 = sy - (dY2*(x2 - sx) + dX-1) / dX2;
				if (IncrZ > 0)
					z2 = sz + (dZ2*(x2 - sx) + dX-1) / dX2;
				else
					z2 = sz - (dZ2*(x2 - sx) + dX-1) / dX2;
			}
			if (y1 > y2) {
				if (y2 > img->clipy2 || y1 < img->clipy1 )
					return;
				if (y1 > img->clipy2) {
					y1 = img->clipy2;
					x1 = sx + (dX2*(sy - y1) + dY-1) / dY2;
					z1 = sz + (dZ2*(sy - y1) + dY-1) / dY2;
				}
				if (y2 < img->clipy1) {
					y2 = img->clipy1;
					x2 = sx + (dX2*(sy - y2) + dY-1) / dY2;
					z2 = sz + (dZ2*(sy - y2) + dY-1) / dY2;
				}
			}
			else {
				if (y1 > img->clipy2 || y2 < img->clipy1 )
					return;
				if (y1 < img->clipy1) {
					y1 = img->clipy1;
					x1 = sx + (dX2*(y1 - sy) + dY-1) / dY2;
					z1 = sz + (dZ2*(y1 - sy) + dY-1) / dY2;
				}
				if (y2 > img->clipy2) {
					y2 = img->clipy2;
					x2 = sx + (dX2*(y2 - sy) + dY-1) / dY2;
					z2 = sz + (dZ2*(y2 - sy) + dY-1) / dY2;
				}
			}

			PY = (x1 - sx + 1) * dY2 - dX - ABS(y1 - sy) * dX2;
			PZ = (x1 - sx + 1) * dZ2 - dX - ABS(z1 - sz) * dX2;

			w = img->width;
			off = y1*w +x1;
			buf8 = img->buf + off;
			buf16 = orgbuf16 + off;

			set_pixel = 1;
			if (useZbuf) {
				pixel_idx = ZINDEX(x1, y1);
				if (z1 <= (int) zbuf[pixel_idx] || z1 > 255)
					set_pixel = 0;
				else
					zbuf[pixel_idx] = z1;
			}
			if (set_pixel) {
				if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
					IMAGESETPIXEL4G(img, x1,y1,gray4);
				else if(img->type == IMAGE_TYPE_256C) // 256 scale
					*buf8 = col8;
				else *buf16 = col16; 
			}
			while(x1 < x2) {
				x1++;
				buf8++;
				buf16++ ;
    				if (PY > 0) // is the pixel going right AND up?
    				{
    					// increment dependent variable
    					y1+=IncrXY;

						if (IncrXY > 0){
							buf8+=w;
							buf16+=w ;
						}
						else{
							buf8-=w;
							buf16-=w ;
						}
						PY+=dY2_dX2;// increment decision (for up)
    				}
    				else      // is the pixel just going right?
    				{
    					// increment decision (for right)
    					PY+=dY2;
    				}

					if (PZ > 0) {
						z1 += IncrZ;
						PZ += dZ2_dX2;
					}
					else
						PZ += dZ2;
					// plot the pixel

					set_pixel = 1;
					if (useZbuf) {
						pixel_idx = ZINDEX(x1, y1);
						if (z1 <= (int) zbuf[pixel_idx] || z1 > 255)
							set_pixel = 0;
						else
							zbuf[pixel_idx] = z1;
					}
					if (set_pixel) {
						if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
							IMAGESETPIXEL4G(img, x1,y1,gray4);
						else if(img->type == IMAGE_TYPE_256C) // 256 scale
							*buf8 = col8;
						else *buf16 = col16; 
					}
    			}		
    		}
   		else
   		{
    			// amount to increment decision if up is chosen
   			dX2_dY2 	= dX2 - dY2;
   			dZ2_dY2 	= dZ2 - dY2;
    			
			if (y1 > y2) PT3_SWAP(x1, y1, z1, x2, y2, z2, t);
			sx = x1; sy = y1; sz = z1;

			if (y1 > img->clipy2 || y2 < img->clipy1 ) return;
			if (x1 > x2) {
				if (x2 > img->clipx2 || x1 < img->clipx1 ) return;
				IncrXY=-1;
			} else {
				if (x1 > img->clipx2 || x2 < img->clipx1 ) return;
				IncrXY=1;
			}
			if (z1 > z2)
				IncrZ = -1;
			else
				IncrZ = 1;

			if (y1 < img->clipy1) {
				y1 = img->clipy1;
				if (IncrXY > 0)
					x1 = sx + (dX2*(y1 - sy) + dY-1) / dY2;
				else
					x1 = sx - (dX2*(y1 - sy) + dY-1) / dY2;
				if (IncrZ > 0)
					z1 = sz + (dZ2*(y1 - sy) + dY-1) / dY2;
				else
					z1 = sz - (dZ2*(y1 - sy) + dY-1) / dY2;
			}
			if (y2 > img->clipy2) {
				y2 = img->clipy2;
				if (IncrXY > 0)
					x2 = sx + (dX2*(y2 - sy) + dY-1) / dY2;
				else
					x2 = sx - (dX2*(y2 - sy) + dY-1) / dY2;
				if (IncrZ > 0)
					z2 = sz + (dZ2*(y2 - sy) + dY-1) / dY2;
				else
					z2 = sz - (dZ2*(y2 - sy) + dY-1) / dY2;
			}
			if (x1 > x2) {
				if (x2 > img->clipx2 || x1 < img->clipx1 ) return;
				if (x1 > img->clipx2) {
					x1 = img->clipx2;
					y1 = sy + (dY2*(sx - x1) + dX-1) / dX2;
					z1 = sz + (dZ2*(sx - x1) + dX-1) / dX2;
				}
				if (x2 < img->clipx1) {
					x2 = img->clipx1;
					y2 = sy + (dY2*(sx - x2) + dX-1) / dX2;
					z2 = sz + (dZ2*(sx - x2) + dX-1) / dX2;
				}
			} else {
				if (x1 > img->clipx2 || x2 < img->clipx1 ) return;
				if (x1 < img->clipx1) {
					x1 = img->clipx1;
					y1 = sy + (dY2*(x1 - sx) + dX-1) / dX2;
					z1 = sz + (dZ2*(x1 - sx) + dX-1) / dX2;
				}
				if (x2 > img->clipx2) {
					x2 = img->clipx2;
					y2 = sy + (dY2*(x2 - sx) + dX-1) / dX2;
					z2 = sz + (dZ2*(x2 - sx) + dX-1) / dX2;
				}
			}

			PX = (y1 - sy + 1) * dX2 - dY - ABS(x1 - sx) * dY2;
			PZ = (y1 - sy + 1) * dZ2 - dY - ABS(z1 - sz) * dY2;

			w = img->width;
			off = y1*w + x1;
			buf8 = img->buf + off;
			buf16 = orgbuf16 + off ;
 
			set_pixel = 1;

			if (useZbuf) {
				pixel_idx = ZINDEX(x1, y1);
				if (z1 <= (int) zbuf[pixel_idx])
					set_pixel = 0;
				else
					zbuf[pixel_idx] = z1;
			}
			if(set_pixel){
				if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
					IMAGESETPIXEL4G(img, x1,y1,gray4);
				else if(img->type == IMAGE_TYPE_256C) // 256 scale
					*buf8 = col8;
				else *buf16 = col16; 
			}
			while(y1 < y2) {
				y1++;
				buf8+=w;
				buf16+=w ;
	   			if (PX > 0) // is the pixel going up AND right?
    			{ 
    				// increment dependent variable
    				x1+=IncrXY;
					buf8+=IncrXY;
					buf16+=IncrXY ;
					// increment decision (for up)
    				PX+=dX2_dY2;
    			}
    			else    // is the pixel just going up?
    			{
    				// increment decision (for right)
    				PX+=dX2;
    			}

				if (PZ > 0) {
					z1 += IncrZ;
					PZ += dZ2_dY2;
				}
				else
					PZ += dZ2;
    			// plot the pixel
    			//imageSetPixel((Image*)img, x1, y1, col, IMG_COPY);

				set_pixel = 1;
				if (useZbuf) {
					pixel_idx = ZINDEX(x1, y1);
					if (z1 <= (int) zbuf[pixel_idx])
						set_pixel = 0;
					else
						zbuf[pixel_idx] = z1;
				}
				if (set_pixel){
					if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
						IMAGESETPIXEL4G(img, x1,y1,gray4);
					else if(img->type == IMAGE_TYPE_256C) // 256 scale
						*buf8 = col8;
					else *buf16 = col16; 
				}
			}		
		}
	}
}
#endif
	
/*    		CLIPPING VALUES OF THE IMAGE MUST BE CONSIDERED */
static void imageCirclePoints( XImage *img, int cx, int cy, int x, int y, BYTE filled, PIXEL col32 )
{
	BYTE col8,gray4;
	UINT16 col16, *buf16 = (UINT16 *)img->buf;
	CLR32TOCLR8(col32,col8);
	CLR32TOCLR16(col32,col16);
	CLR32TOLCDGRAY2(col32,gray4);
	if ( !filled )
    {
		if(!OUTCLIP(img, cx + x, cy + y)){
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, cx + x, cy + y,gray4);
			else if (img->type == IMAGE_TYPE_256C) /* 256 scale */
				IMAGESETPIXEL(img->buf,img, cx + x, cy + y,col8,IMG_COPY);
			else IMAGESETPIXEL(buf16,img, cx + x, cy + y,col16,IMG_COPY);
		}
		if(!OUTCLIP(img, cx + y, cy + x)){
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, cx + y, cy + x,gray4);
			else if (img->type == IMAGE_TYPE_256C) /* 256 scale */
				IMAGESETPIXEL(img->buf,img, cx + y, cy + x,col8,IMG_COPY);
			else 
				IMAGESETPIXEL(buf16,img, cx + y, cy + x,col16,IMG_COPY);
		}
		if(!OUTCLIP(img, cx + y, cy - x)){
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, cx + y, cy - x,gray4);
			else if (img->type == IMAGE_TYPE_256C) /* 256 scale */
				IMAGESETPIXEL(img->buf,img, cx + y, cy - x,col8,IMG_COPY);
			else IMAGESETPIXEL(buf16,img, cx + y, cy - x,col16,IMG_COPY);
		}
		if(!OUTCLIP(img, cx + x, cy - y)){
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, cx + x, cy - y,gray4);
			else if (img->type == IMAGE_TYPE_256C) /* 256 scale */
				IMAGESETPIXEL(img->buf,img,  cx + x, cy - y,col8,IMG_COPY);
			else IMAGESETPIXEL(buf16,img,  cx + x, cy - y,col16,IMG_COPY);
		}
		if(!OUTCLIP(img, cx - x, cy - y)){
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, cx -x, cy - y,gray4);
			else if (img->type == IMAGE_TYPE_256C) /* 256 scale */
				IMAGESETPIXEL(img->buf,img,  cx - x, cy - y,col8,IMG_COPY);
			else IMAGESETPIXEL(buf16,img,   cx - x, cy - y,col16,IMG_COPY);
		}

		if(!OUTCLIP(img, cx - y, cy - x)){
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, cx -y, cy - x,gray4);
			else if (img->type == IMAGE_TYPE_256C) /* 256 scale */
				IMAGESETPIXEL(img->buf,img,  cx -y, cy - x,col8,IMG_COPY);
			else IMAGESETPIXEL(buf16,img,  cx -y, cy - x,col16,IMG_COPY);
		}
    	if(!OUTCLIP(img, cx - y, cy + x)){
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, cx -y, cy + x,gray4);
			else if (img->type == IMAGE_TYPE_256C) /* 256 scale */
				IMAGESETPIXEL(img->buf,img,  cx -y, cy + x,col8,IMG_COPY);
			else IMAGESETPIXEL(buf16,img,  cx -y, cy + x, col16,IMG_COPY);
		}
    	if(!OUTCLIP(img, cx - x, cy + y)){
			if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
				IMAGESETPIXEL4G(img, cx -x, cy + y,gray4);
			else if (img->type == IMAGE_TYPE_256C) /* 256 scale */
				IMAGESETPIXEL(img->buf,img,  cx - x, cy + y,col8,IMG_COPY);
			else IMAGESETPIXEL(buf16,img,  cx - x, cy + y, col16,IMG_COPY);
		}
    }
    else
    {
		// (x, y) ~ (x, -y) 
    	imageDrawLine((Image *)img, cx + x, cy - y, cx + x, cy + y, col32);
		// (-x, -y) ~ (-x, y) 
		if (x)
			 imageDrawLine((Image *)img, cx - x, cy - y, cx - x, cy + y, col32);
		// (y, -x) ~ (y, x) 
		if (x != y) {
			imageDrawLine((Image *)img, cx + y, cy - x, cx + y, cy + x, col32);
	    // (-y, -x) ~ (-y, x) 
			if (y)
			imageDrawLine((Image *)img, cx - y, cy - x, cx - y, cy + x, col32);
		}
    }
}
/*************************************************************
* function: imageDrawCircle
* desc:      draw a circle in the image at (cx, cy) with radius 
*    		filled disc is drawn when 'filled' is set to 1
*    		'imageSetPixel' is used in the drawing loop. 
*    		CLIPPING VALUES OF THE IMAGE MUST BE CONSIDERED 
* input:    image, cx, cy, filled(0 or 1), radius, color 
* return:     nothing	
**************************************************************/
void    imageDrawCircle( Image *im, int cx, int cy, BYTE filled, int radius, PIXEL col32 )
{	//

    XImage *img = (XImage*)im;
    int	x;
    int y;
    int d;
    int deltaE;
    int deltaSE;

	x = 0;
    y = radius;
    d = 1 - radius;
    deltaE = 3;
    deltaSE = -(radius<<1) + 5;
    imageCirclePoints( img, cx, cy, x,  y, filled, col32 );
    while( y > x) {
    	if ( d < 0 )
    	{
    		d += deltaE;
    		deltaE += 2;
    		deltaSE += 2;
    		x++;			 
    	}
    	else
    	{
    		d += deltaSE;
    		deltaE += 2;
    		deltaSE += 4;
    		x++;
    		y--;
    	}
	imageCirclePoints( img, cx, cy, x,  y, filled, col32 );
    }
}
//////////////////////////////////////////////////////////////

#define	getTableIndex(angle) ((angle+3)/5) // 90/5 = 18
static void getCosSin1024( int degree, int *cosval, int *sinval )
{
    int	idx;

    while ( degree < 0 ) degree += 360;
    while ( degree > 360 ) degree -= 360;

    if ( degree <= 90 ) 
    {
    	idx = getTableIndex( degree );
    	*cosval = tritable1024[idx][0];
    	*sinval = tritable1024[idx][1];
    }
    else if ( degree <= 180 )
    {
    	idx = getTableIndex( 180 - degree );
    	*cosval = -tritable1024[idx][0];  
    	*sinval = tritable1024[idx][1];
    }
    else if ( degree <= 270 )
    {
    	idx = getTableIndex( degree - 180 );
    	*cosval = -tritable1024[idx][0];
    	*sinval = -tritable1024[idx][1];
    } 
    else 
    {
    	idx = getTableIndex( 360 - degree ); 
    	*cosval = tritable1024[idx][0];
    	*sinval = -tritable1024[idx][1];;
    }
}

/*************************************************************
* function: imageDrawArc
* desc:      draw an arc in a rectangle(x,y,w,h)
*    		drawing angle is specified by the start angle and
*    		arc angle, which is counterclockwise.
*    		filled arc is drawn when 'filled' is set to 1
*    		'imageSetPixel' is used in the drawing loop. 
*    		CLIPPING VALUES OF THE IMAGE MUST BE CONSIDERED 
* input:    image, cx, cy, rectangle(x,y,w,h), 
*    		angle(start angle, arcangle), filled(0 or 1), color 
* return:     nothing	
**************************************************************/
#define	getVal1024(x)		(((x)+512)>>10)
void     imageDrawArc( Image *im, int x, int y, int w, int h, 
    		int sAngle, int arcAngle, BYTE filled, PIXEL col32 )
{
	XImage *img = (XImage *)im;	
    int cx;
    int cy;
	BYTE col8,gray4;
	UINT16 col16 , *buf16 = (UINT16 *)img->buf;
	

// for test
//	filled = 0;
// test
	if (arcAngle < 0) {
	sAngle += arcAngle;
	arcAngle = -arcAngle;
    }
	
    if (w == 0 || h == 0) {
	imageDrawLine(im, x, y, x+w, y+h, col32);
	return;
    }

    if (w <= 0 || h <= 0 || arcAngle <= 0) return;
    if ( x>= img->width || y>= img->height || x+w < 1 || y+w < 1 ) return;

    cx = x + (w>>1);
    cy = y + (h>>1);
	
	if (img->type == IMAGE_TYPE_4G)
		CLR32TOLCDGRAY2(col32,gray4);
	else if(img->type == IMAGE_TYPE_256C)
		CLR32TOCLR8(col32,col8);
	else CLR32TOCLR16(col32,col16);
	
	if ( !(arcAngle%360) && (w == h) )
    {
    	imageDrawCircle( (Image*)img, cx, cy, filled, w>>1, col32 );
    }
    else {
    	int angle;
    	int step = 8 + (int)((w+h)>>5);
    	int da;
    	int cs;
    	int sn;
    	long a = w>>1;
    	long b = h>>1;
		long val;
		int sx, sy, ex, ey;
		int eAngle = sAngle + arcAngle;
		int i, j, result,subresult, rightX, leftX, topY, bottomY; 
		char fAlready;

     	da = arcAngle/(step+1);
		if (da <= 0) da = 1;
		getCosSin1024( sAngle, &cs, &sn );
		val = a*cs;
		sx = (int)(getVal1024(val));
		val = b*sn;
		sy = (int)(getVal1024(val));
    	for( angle = sAngle + da; angle < eAngle; angle += da ) {
			getCosSin1024( angle, &cs, &sn );
			val = a*cs;
			ex = (int)(getVal1024(val));
			val = b*sn;
			ey = (int)(getVal1024(val));

			result = ABSO((cx+ex)*(cy-sy)+(cx+sx)*(y+b)+(x+a)*(cy-ey)
						 -(cx+sx)*(cy-ey)-(x+a)*(cy-sy)-(cx+ex)*(y+b));
			rightX = MAX3(cx+sx, cx+ex, x+w/2);
			leftX =  MIN3(cx+sx, cx+ex, x+w/2);
			topY =   MIN3(cy-sy, cy-ey, y+h/2);
			bottomY = MAX3(cy-sy, cy-ey, y+h/2);
			if (filled){
				imageDrawLine(im, cx + sx, cy - sy, cx + ex, cy - ey, col32);

				for(j=topY; j <=bottomY; j++){
					fAlready = 0;
					for(i=leftX; i<=rightX; i++){
						subresult = ABSO(
							           (cx+ex)*(cy-sy)+(cx+sx)*j+i*(cy-ey)
									   -(cx+sx)*(cy-ey)-i*(cy-sy)-(cx+ex)*j
									   )+ 
									ABSO(
									   (cx+sx)*(y+b)+(x+a)*j+i*(cy-sy)
									   -(x+a)*(cy-sy)-i*(y+b)-(cx+sx)*j
									   )+
									ABSO(
									   (cx+ex)*(y+b)+(x+a)*j+i*(cy-ey)
									   -(x+a)*(cy-ey)-i*(y+b)-(cx+ex)*j
									   );
						if(result == subresult)	
						{
							if(!fAlready) fAlready =1;
							if(!OUTCLIP(img, cx + x, cy - y)){
								if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
									IMAGESETPIXEL4G(img, i, j,gray4);
								if (img->type == IMAGE_TYPE_256C) /* 256 scale */
									IMAGESETPIXEL(img->buf,img,  i, j, col8,IMG_COPY);
								else IMAGESETPIXEL(buf16,img,  i, j, col16,IMG_COPY);
							}
						}else if(fAlready) break;
					}
				}
			}
			else
				imageDrawLine(im, cx + sx, cy - sy, cx + ex, cy - ey, col32);
			sx = ex;
			sy = ey;
    	}
		getCosSin1024( eAngle, &cs, &sn );
		ex = (int)(getVal1024(a*cs));
		ey = (int)(getVal1024(b*sn));
		result = ABSO((cx+ex)*(cy-sy)+(cx+sx)*(y+b)+(x+a)*(cy-ey)
					 -(cx+sx)*(cy-ey)-(x+a)*(cy-sy)-(cx+ex)*(y+b));
			rightX = MAX3(cx+sx, cx+ex, x+a);
			leftX =  MIN3(cx+sx, cx+ex, x+a);
			topY =   MIN3(cy-sy, cy-ey, y+b);
			bottomY = MAX3(cy-sy, cy-ey,y+b);
			if (filled){
				imageDrawLine(im, cx + sx, cy - sy, cx + ex, cy - ey, col32);
				for(j=topY; j <=bottomY; j++){
					fAlready = 0;
					for(i=leftX; i<=rightX; i++){
						subresult = ABSO(
							           (cx+ex)*(cy-sy)+(cx+sx)*j+i*(cy-ey)
									   -(cx+sx)*(cy-ey)-i*(cy-sy)-(cx+ex)*j
									   )+ 
									ABSO(
									   (cx+sx)*(y+b)+(x+a)*j+i*(cy-sy)
									   -(x+a)*(cy-sy)-i*(y+b)-(cx+sx)*j
									   )+
									ABSO(
									   (cx+ex)*(y+b)+(x+a)*j+i*(cy-ey)
									   -(x+a)*(cy-ey)-i*(y+b)-(cx+ex)*j
									   );
					if(result == subresult)	
						{
							if(!fAlready) fAlready =1;
							if(!OUTCLIP(img, cx + x, cy - y)){
								if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
									IMAGESETPIXEL4G(img, i, j,gray4);
								if (img->type == IMAGE_TYPE_256C) /* 256 scale */
									IMAGESETPIXEL(img->buf,img,  i, j, col8,IMG_COPY);
								else IMAGESETPIXEL(buf16,img,  i, j, col16,IMG_COPY);
							}
						}else if(fAlready) break;
					}
				}
		}
		else
		    imageDrawLine(im, cx + sx, cy - sy, cx + ex, cy - ey, col32);
	}
	
}


/*************************************************************
* function: imageDrawRect
* desc:  	draw a rectangle in the image
*			filled rect is drawn when 'filled' is set to 1
*			'radius' is given for the corner rounding
*			'imageSetPixel' is used in the drawing loop. 
*			CLIPPING VALUES OF THE IMAGE MUST BE CONSIDERED 
* input:	image, rectangle(x,y,width,height), 
*			filled(0 or 1), radius, color 
* return: 	nothing	
**************************************************************/
void 	imageDrawRect( Image *im, int x, int y, int width, 
			int height, BYTE filled, PIXEL col32 )
{
	XImage *img = (XImage *)im;	
	COORD i, StartY, EndY,StartX, EndX;
	
	StartY = y;
	EndY = y+height -1;
	StartX = x;
	EndX = x+width -1;

	if(filled){
		if(y < img-> clipy1) StartY = img-> clipy1;
		if(y+height-1 > img->clipy2) EndY = img->clipy2;
		if(x < img-> clipx1) StartX = img->clipx1;
		if(x+width-1 > img-> clipx2) EndX = img->clipx2;
		for(i = StartY; i <= EndY ; i++)
			imageDrawLine((Image *)img, StartX, i, EndX,i,col32);
	}	
	else{
		// 빈 사각형
		if(StartX >= img->clipx1){
			if(y < img-> clipy1) StartY = img-> clipy1;
			if(y+height-1 > img->clipy2) EndY = img->clipy2;
			imageDrawLine((Image *)img, StartX, StartY, StartX, EndY,col32);
		}
		if(EndX <= img->clipx2){
			if(y < img-> clipy1) StartY = img-> clipy1;
			if(y+height-1 > img->clipy2) EndY = img->clipy2;
			imageDrawLine((Image *)img, EndX, StartY, EndX, EndY,col32);
		}
		if(StartY >= img->clipy1){
			if(x < img-> clipx1) StartX = img->clipx1;
			if(x+width-1 > img-> clipx2) EndX = img->clipx2;
			imageDrawLine((Image *)img, StartX, StartY, EndX, StartY,col32);
		}
		if(EndY <= img->clipy2){
			if(x < img-> clipx1) StartX = img->clipx1;
			if(x+width-1 > img-> clipx2) EndX = img->clipx2;
			imageDrawLine((Image *)img, StartX, EndY, EndX, EndY,col32);
		}
	}
}

/*************************************************************
* function: imageDrawImage
* desc:  	draw a part of the source image(sx,sy,w,h) to a
* 			target image at the given position( tx,ty)

  			'mode' value is given for additional operation as
*			follows:
*			IMG_COPY : just overwrite the color value
*			IMG_AND	: bitwise and operation  
*			IMG_OR	: bitwise or operation
*			IMG_XOR	: bitwise xor operation
*			CLIPPING VALUES OF THE TARGET IMAGE MUST BE CONSIDERED 
* input:	target image(tgt), target position(tx, ty), 
*			source image(src), source position(sx, sy),
*			width, height, mode	
* return: 	nothing	
**************************************************************/
void 	imageDrawImage( Image *tim, int tx, int ty, Image *sim, int sx, int sy, int w, int h, BYTE mode )
{	
    XImage *tgt = (XImage*)tim;
    XImage *src = (XImage*)sim;
	register BYTE *ptgt8,*psrc8, *psrcMask,*pOrgSrcData,*pOrgtgtData;
	register UINT16* ptgt16,*psrc16;
	register UINT32 i,j,k;
	int dx, dy, x2, y2;
	LOCAL INT16 wBytePos, wMaskBytePos ;
	BYTE b1stMaskData, b2ndMaskData, b1stData, b2ndData;
	INT8 nBitPos,nSourceBitPos, nTargetBitPos,nOrgsrcBitPos,nTargetOrgBitPos;
	INT8 nSourceMaskBitPos, nMaskBitPos;
	BYTE bResult;

	
    if (w <= 0 || h <= 0) return;

    dx = tx - sx;
    dy = ty - sy;
    x2 = sx + w - 1;
    y2 = sy + h - 1;

    /* boundary src */
    if (sx < 0) sx = 0;
    if (sy < 0) sy = 0;
    if (x2 >= src->width) x2 = src->width-1;
    if (y2 >= src->height) y2 = src->height-1;

    w = x2 - sx + 1;
    h = y2 - sy + 1;
    if (w <= 0 || h <= 0) return;
    tx = sx + dx;
    ty = sy + dy;
    x2 = tx + w - 1;
    y2 = ty + h - 1;
	
    ROBUSTCLIP(tgt);
    /* clip tgt */
    if ( tx < tgt->clipx1 )	tx = tgt->clipx1;
	if ( ty < tgt->clipy1 )	ty = tgt->clipy1;
	if ( x2 > tgt->clipx2 ) x2 = tgt->clipx2;
    if ( y2 > tgt->clipy2 ) y2 = tgt->clipy2;

    w = x2 - tx + 1;
    h = y2 - ty + 1;
    if (w <= 0 || h <= 0) return;
    sx = tx - dx;
    sy = ty - dy;

	psrc8 = src->buf;
	ptgt8 = tgt->buf;
	psrc16 = (UINT16 *)src->buf;
	ptgt16 = (UINT16 *)tgt->buf;

	if(src->type ==IMAGE_TYPE_4G){		// src .. //   4gray  -->    ??
		switch(tgt->type){
			case IMAGE_TYPE_4G:          // 4gray -> 4gray   
				/* ver 2 */
				if(src->mask){
					BYTE *pOrgsrcMask;
					UINT16 l_srcwidth, l_tgtwidth,l_srcmaskwidth;

					nSourceBitPos = nOrgsrcBitPos = BIT_POS_4G(sx);
					nTargetBitPos = nTargetOrgBitPos =  BIT_POS_4G(tx);
					nSourceMaskBitPos = nMaskBitPos = BIT_POS_MASK(sx);

					psrcMask = pOrgsrcMask =   src->maskbuf + TOT_BYTE_POS_MASK(sx, sy,(src->width));
					psrc8 = pOrgSrcData=  src->buf + TOT_BYTE_POS_4G(sx, sy,(src->width));
					ptgt8 = pOrgtgtData = tgt->buf + TOT_BYTE_POS_4G(tx, ty,(tgt->width));

					l_srcwidth = BYTE_POS_4G((src->width+3));
					l_tgtwidth = BYTE_POS_4G((tgt->width+3));
					l_srcmaskwidth = BYTE_POS_MASK((src->width+7));

					for(i=0; i<h; i++){//mask를 2배로 튀겨서 쓰기 때문에 mask의 시작 비트는 data와 같다. 

						// 첫번째 바이트 처리 
						CONVERT_1BIT_2_2BIT( (*psrcMask) , b1stMaskData, b2ndMaskData);

						if(nSourceMaskBitPos<4){ // mask bit가 4보다 작으면 2byte를 읽는다. 
							b1stData = *psrc8;
							psrc8++;
							b2ndData = *psrc8;
							psrc8++;
							psrcMask++;
							for(j=nSourceBitPos, k=0; (j<8)&& (k<w);j+=2, k++){
								if(!(b1stMaskData&m_abMask4G[j])){
									bResult = ((b1stData&m_abMask4G[j])>>j); 
									(*ptgt8) &= m_abNotMask4G[nTargetBitPos];
									(*ptgt8) |= (bResult<<nTargetBitPos);
								}
								nTargetBitPos+=2;
								if(nTargetBitPos==8){
									ptgt8++;
									nTargetBitPos = 0;
								}
							}
							if(k==w){
								nSourceBitPos = nOrgsrcBitPos;
								nTargetBitPos = nTargetOrgBitPos;
								nSourceMaskBitPos = nMaskBitPos;

								pOrgSrcData = psrc8  = pOrgSrcData+ l_srcwidth;
								pOrgtgtData = ptgt8 = pOrgtgtData+l_tgtwidth;
								psrcMask = pOrgsrcMask = pOrgsrcMask + l_srcmaskwidth;
								continue;
							}
							for(j=0;j<8 && k<w; j+=2, k++){
								if(!(b2ndMaskData&m_abMask4G[j])){
									bResult = (b2ndData&m_abMask4G[j])>>j; 
									(*ptgt8) &= m_abNotMask4G[nTargetBitPos];
									(*ptgt8) |=(bResult<<nTargetBitPos);
								}
								nTargetBitPos+=2;
								if(nTargetBitPos==8){
									ptgt8++;
									nTargetBitPos = 0;
								}
							}
							if(k==w){
								nSourceBitPos = nOrgsrcBitPos;
								nTargetBitPos = nTargetOrgBitPos;
								nSourceMaskBitPos = nMaskBitPos;

								pOrgSrcData = psrc8  = pOrgSrcData+ l_srcwidth;
								pOrgtgtData = ptgt8 = pOrgtgtData+l_tgtwidth;
								pOrgsrcMask = psrcMask = pOrgsrcMask + l_srcmaskwidth;
								continue;
							}
						}
						else{    // mask bit가 4보다 크면 1byte만 읽는다. 
							b1stData = *psrc8;
							psrc8++;
							psrcMask++;
							for(j=nSourceBitPos, k=0; (j<8)&& (k<w);j+=2, k++){
								if(!(b2ndMaskData&m_abMask4G[j])){
									bResult = (b1stData&m_abMask4G[j])>>j; 
									(*ptgt8) &= m_abNotMask4G[nTargetBitPos];
									(*ptgt8) |= (bResult<<nTargetBitPos);
								}
								nTargetBitPos+=2;
								if(nTargetBitPos==8){
									ptgt8++;
									nTargetBitPos = 0;
								}
							}
							if(k==w){
								nSourceBitPos = nOrgsrcBitPos;
								nTargetBitPos = nTargetOrgBitPos;
								nSourceMaskBitPos = nMaskBitPos;

								pOrgSrcData = psrc8  = pOrgSrcData+ l_srcwidth;
								pOrgtgtData = ptgt8 = pOrgtgtData+l_tgtwidth;
								psrcMask = pOrgsrcMask = pOrgsrcMask + l_srcmaskwidth;
								continue;
							}
						}

						// 중간 바이트 처리는 동일.. 

  						if(nTargetBitPos){
							for(j=8-nOrgsrcBitPos;j<w;j+=8){
								if(j+8>=w) break; // 맨 마지막 바이트 인가 검사..
								
								CONVERT_1BIT_2_2BIT((*psrcMask), b1stMaskData, b2ndMaskData);
								b1stData = *psrc8;
								psrc8++;
								b2ndData = *psrc8;
								psrc8++; psrcMask++;

								SETMASK_COLOR_BIT_1ST_BYTE((*ptgt8), b1stData, b1stMaskData,nTargetBitPos);
								ptgt8++;
								SETMASK_COLOR_BIT_2ND_BYTE((*ptgt8), b1stData, b1stMaskData,nTargetBitPos);
								SETMASK_COLOR_BIT_1ST_BYTE((*ptgt8), b2ndData, b2ndMaskData,nTargetBitPos);
								ptgt8++;
								SETMASK_COLOR_BIT_2ND_BYTE((*ptgt8), b2ndData, b2ndMaskData,nTargetBitPos);
							}
						}
						else{
							for(j=8-nOrgsrcBitPos;j<w;j+=8){
								if(j+8>=w) break; // 맨 마지막 바이트 인가 검사..
								CONVERT_1BIT_2_2BIT((*psrcMask), b1stMaskData, b2ndMaskData);
								b1stData = *psrc8;
								psrc8++;
								b2ndData = *psrc8;
								psrc8++; psrcMask++;

								SETMASK_COLOR_BIT_1ST_BYTE((*ptgt8), b1stData, b1stMaskData,nTargetBitPos);
								ptgt8++;
								SETMASK_COLOR_BIT_1ST_BYTE((*ptgt8), b2ndData, b2ndMaskData,nTargetBitPos);
								ptgt8++;
							}
						}

						/// 마지막 바이트 처리.. 
						nSourceBitPos =nSourceMaskBitPos = 0;
						for(;j<w;j++){
							if(!((*psrcMask)&bitMask[nSourceMaskBitPos])){ //1)
								bResult = ((*psrc8)& m_abMask4G[nSourceBitPos])>> nSourceBitPos; //3)
								(*ptgt8) &= m_abNotMask4G[nTargetBitPos];
								(*ptgt8) |= (bResult<<nTargetBitPos);
							}
							nSourceMaskBitPos++;
							nSourceBitPos+=2;
							nTargetBitPos += 2;
							if(nSourceBitPos==8){
								psrc8++;
								nSourceBitPos = 0;
							}
							if(nTargetBitPos==8){
								ptgt8++;
								nTargetBitPos =0;
							}
						}
						nSourceBitPos = nOrgsrcBitPos;
						nTargetBitPos = nTargetOrgBitPos;
						nSourceMaskBitPos = nMaskBitPos;
						
						pOrgSrcData = psrc8  = pOrgSrcData+ l_srcwidth;
						pOrgtgtData = ptgt8 = pOrgtgtData+l_tgtwidth;
						psrcMask = pOrgsrcMask = pOrgsrcMask + l_srcmaskwidth;

					}


				}
				else{
					UINT16 l_srcwidth, l_tgtwidth;

					nSourceBitPos = nOrgsrcBitPos = BIT_POS_4G(sx);
					nTargetBitPos = nTargetOrgBitPos =  BIT_POS_4G(tx);

					psrc8 = pOrgSrcData=  src->buf + TOT_BYTE_POS_4G(sx, sy,(src->width));
					ptgt8 = pOrgtgtData = tgt->buf + TOT_BYTE_POS_4G(tx, ty,(tgt->width));

					l_srcwidth = BYTE_POS_4G((src->width+3));
					l_tgtwidth = BYTE_POS_4G((tgt->width+3));

					
					for(i=0; i<h; i++){

						// 첫번째 바이트처리..
						b1stData = *psrc8;
						psrc8++;
						for(j=nSourceBitPos, k = 0; (j<8)&& (k<w); j+=2, k++){
							bResult = (b1stData&m_abMask4G[j])>>j; 
							(*ptgt8) &= m_abNotMask4G[nTargetBitPos];
							(*ptgt8) |= (bResult<<nTargetBitPos);
							nTargetBitPos+=2;
							if(nTargetBitPos==8){
								ptgt8++;
								nTargetBitPos = 0;
							}
						}
						if(k == w){
							pOrgSrcData = psrc8  = pOrgSrcData+ l_srcwidth;
							pOrgtgtData = ptgt8 = pOrgtgtData+l_tgtwidth;
							continue;
						}

						// 중간 바이트 처리는 동일.. 
						if(nTargetBitPos){
							for(j= 4-(nSourceBitPos>>1);j+4<w;j+=4){
//								if(j+8>=w) break; // 맨 마지막 바이트 인가 검사..
								b1stData = *psrc8;
								psrc8++;
								SET_COLOR_BIT_1ST_BYTE(*(ptgt8), b1stData, nTargetBitPos);
								ptgt8++;
								SET_COLOR_BIT_2ND_BYTE(*(ptgt8), b1stData, nTargetBitPos);
							}
						}
						else{
							for(j=(4-(nSourceBitPos>>1));j+4<w;j+=4){
//								if(j+8>=w) break; // 맨 마지막 바이트 인가 검사..
								*ptgt8 = *psrc8;
								psrc8++; ptgt8++;
							}
						}
						// 마지막 바이트 처리 
						nSourceBitPos = 0;
						for(;j<w;j++){
							bResult = ((*psrc8)& m_abMask4G[nSourceBitPos])>> nSourceBitPos; //3)
							*ptgt8 &= m_abNotMask4G[nTargetBitPos];
							*ptgt8 |= (bResult<<nTargetBitPos);
							nSourceBitPos +=2;
							nTargetBitPos +=2;
							if(nTargetBitPos==8){
								ptgt8++;
								nTargetBitPos = 0;
							}
						}
						pOrgSrcData = psrc8  = pOrgSrcData+  l_srcwidth;
						pOrgtgtData = ptgt8 = pOrgtgtData + l_tgtwidth;
						nSourceBitPos = nOrgsrcBitPos;
						nTargetBitPos = nTargetOrgBitPos;
					}
				}

				// 이상 4gray -> 4gray
				break;

			case IMAGE_TYPE_256C:         // 4gray -> 256 C 
			/*  4gray 에서 한점의 투명 mask의 buf가 set 되어 있고 mask 가 set 되어 있으면 건너 뛴다. 1) 
			   한점의 투명 mask가 set되어 있지 않거나 mask 가 set 되어 있지 않으면 2)
				소스의 해당 위치를 찾아서 3) 그 값을 읽어 4그레이 값으로 만든 후  4) 
				 8bit color로 변환 한다 5) 
			 그값을 타겟의 위치에 넣는다. 6)  
			*/
				if(src->mask){
					UINT16 l_width, l_maskwidth, tempwidth;
					BYTE *ptgtstart8, *psrcstart8,*psrcStartMask, nMasktemp, nTemp;
					l_width = (src->width+3)>>2;
					l_maskwidth = (src->width+7)>>3;
					ptgtstart8 = ptgt8 = tgt->buf+ tgt->width*ty+tx;  //6)
					psrcstart8 = psrc8 = src->buf + (sx>>2) + sy*l_width; 
					psrcStartMask = psrcMask = src->maskbuf+BYTE_POS_MASK(sx)+sy*l_maskwidth; 
					switch (mode) {
						case IMG_COPY:
							nMaskBitPos = BIT_POS_MASK(sx);
							nBitPos = BIT_POS_4G(sx);    //3)
							if (w>8)
								tempwidth = w-8;
							else tempwidth = 0;

							for(i=0; i<h; i++){
								if(nMaskBitPos>=4){		
									switch(nBitPos){
										case 0:
											if(!(*psrcMask & 0x10))
												*(ptgt8++) = b4GTo256[(*psrc8 & 0x03)];
											else ptgt8++;
										case 2:
											if(!(*psrcMask & 0x20))
												*(ptgt8++) = b4GTo256[(*psrc8 & 0x0C)>>2];
											else ptgt8++;
										case 4:
											if(!(*psrcMask & 0x40))
												*(ptgt8++) = b4GTo256[(*psrc8 & 0x30)>>4];
											else ptgt8++;
										case 6:
											if(!(*psrcMask & 0x80))
												*(ptgt8++) = b4GTo256[(*psrc8 & 0xC0)>>6];
											else ptgt8++;
										psrc8++;
										psrcMask++;
										break;
									}
								}

								else{					
									switch(nBitPos){
									case 0:
										if(!(*psrcMask & 0x01))
											*(ptgt8++) = b4GTo256[(*psrc8 & 0x03)];
										else ptgt8++;
									case 2:
										if(!(*psrcMask & 0x02))
											*(ptgt8++) = b4GTo256[(*psrc8 & 0x0C)>>2];
										else ptgt8++;
									case 4:
										if(!(*psrcMask & 0x04))
											*(ptgt8++) = b4GTo256[(*psrc8 & 0x30)>>4];
										else ptgt8++;
									case 6:
										if(!(*psrcMask & 0x08))
											*(ptgt8++) = b4GTo256[(*psrc8 & 0xC0)>>6];
										else ptgt8++;
										if(w<5) break;
										psrc8++;
										if (!(*psrcMask & 0x10))
											*(ptgt8++) = b4GTo256[*psrc8 & 0x03]; 
										else ptgt8++;
										if(w<6) break;
										if (!(*psrcMask & 0x20))
											*(ptgt8++) = b4GTo256[(*psrc8 & 0x0c) >> 2]; 
										else ptgt8++;
										if(w<7) break;
										if (!(*psrcMask & 0x40))
											*(ptgt8++) = b4GTo256[(*psrc8 & 0x30) >> 4];
										else ptgt8++;
										if(w<8) break;	
										if (!(*psrcMask & 0x80))
											*(ptgt8++) = b4GTo256[(*psrc8 & 0xc0) >> 6];
										else ptgt8++;

										psrc8++;
										psrcMask++;

										break;
									}
									if(w<(8-nMaskBitPos)){
										ptgt8 = ptgtstart8 += tgt->width;  //6)
										psrc8 = psrcstart8 += l_width; 
										psrcMask = psrcStartMask += l_maskwidth; 
										continue;							
									}

								}
								for(j=8-nMaskBitPos;j<tempwidth;j+=8){
//									if(j+8<w) break;
									if (!(*psrcMask & 0x01))
										*(ptgt8++) = b4GTo256[*psrc8 & 0x03]; 
									else ptgt8++;

									if (!(*psrcMask & 0x02))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x0c) >> 2]; 
									else ptgt8++;

									if (!(*psrcMask & 0x04))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x30) >> 4];
									else ptgt8++;
									
									if (!(*psrcMask & 0x08))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0xc0) >> 6];
									else ptgt8++;

									psrc8++;
									if (!(*psrcMask & 0x10))
										*(ptgt8++) = b4GTo256[*psrc8 & 0x03]; 
									else ptgt8++;
									if (!(*psrcMask & 0x20))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x0c) >> 2]; 
									else ptgt8++;

									if (!(*psrcMask & 0x40))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x30) >> 4];
									else ptgt8++;
									if (!(*psrcMask & 0x80))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0xc0) >> 6];
									else ptgt8++;
									psrc8++;
									psrcMask++;
								}
								nTemp = 0;
								if(w-j>=4){
									if (!(*psrcMask & 0x01))
										*(ptgt8++) = b4GTo256[*psrc8 & 0x03]; 
									else ptgt8++;

									if (!(*psrcMask & 0x02))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x0c) >> 2]; 
									else ptgt8++;

									if (!(*psrcMask & 0x04))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x30) >> 4];
									else ptgt8++;
									
									if (!(*psrcMask & 0x08))
										*(ptgt8++) = b4GTo256[(*psrc8 & 0xc0) >> 6];
									else ptgt8++;

									psrc8++;
							
									nMasktemp = 4;
									for(j+=4;j<w;j++){
										if (!(*psrcMask & bitMask[nMasktemp]))
											*(ptgt8++) = b4GTo256[(*psrc8&m_abMask4G[nTemp])>>nTemp]; 
										else ptgt8++;
										nMasktemp++;
										nTemp+=2;
									}
								}
								else{
									nMasktemp = 0;
									for(;j<w;j++){
										if (!(*psrcMask & bitMask[nMasktemp]))
											*(ptgt8++) = b4GTo256[(*psrc8&m_abMask4G[nTemp])>>nTemp]; 
										else ptgt8++;
										nMasktemp++;
										nTemp +=2;
									}
								}
								ptgt8 = ptgtstart8 += tgt->width;  //6)
								psrc8 = psrcstart8 += l_width; 
								psrcMask = psrcStartMask += l_maskwidth; 
							}
							break;
						case IMG_AND: 
//							*ptgt8 &= b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
						case IMG_OR	:
//							*ptgt8 |=  b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
						case IMG_XOR : 
//							*ptgt8 ^=  b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
					}

////////////////////

				}
				else{
					UINT16 l_width, tempwidth;
					BYTE *ptgtstart8, *psrcstart8,nTemp;
					l_width = BYTE_POS_4G((src->width+3));
					ptgt8 = ptgtstart8 = tgt->buf+ tgt->width*ty+tx;  //6)
					psrc8 = psrcstart8 = src->buf + BYTE_POS_4G(sx) + sy*l_width; 

					switch (mode) {
						case IMG_COPY:
							nBitPos = BIT_POS_4G(sx);    //3)
							if (w>4)
								tempwidth = w-4;
							else tempwidth = 0;
							for(i=0; i<h; i++){
								switch(nBitPos){
									case 0:
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x03)];
									case 2:
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x0C)>>2];
									case 4:
										*(ptgt8++) = b4GTo256[(*psrc8 & 0x30)>>4];
									case 6:
										*(ptgt8++) = b4GTo256[(*psrc8 & 0xC0)>>6];
									psrc8++;
									psrcMask++;
									break;
								}
								for(j=4-(nBitPos>>1);j<=tempwidth;j+=4){
									*(ptgt8++) = b4GTo256[*psrc8 & 0x03]; 
									*(ptgt8++) = b4GTo256[(*psrc8 & 0x0c) >> 2]; 
									*(ptgt8++) = b4GTo256[(*psrc8 & 0x30) >> 4];
									*(ptgt8++) = b4GTo256[(*psrc8 & 0xc0) >> 6];
									psrc8++;
								}
								nTemp = 0;
								for(;j<w;j++){
									*(ptgt8++) = b4GTo256[(*psrc8&m_abMask4G[nTemp])>>nTemp]; 
									nTemp+=2;
								}

								ptgt8 = ptgtstart8 += tgt->width;  //6)
								psrc8 = psrcstart8 += l_width; 
							}
							break;
						case IMG_AND: 
//							*ptgt8 &= b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
						case IMG_OR	:
//							*ptgt8 |=  b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
						case IMG_XOR : 
//							*ptgt8 ^=  b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
					}
				}
				break;
				// 이상 4gray -> 256C 


			case IMAGE_TYPE_64KC:         // 4gray -> 64KC
				if(src->mask){
					UINT16 l_width, l_maskwidth, tempwidth, *ptgtstart16;
					BYTE  *psrcstart8,*psrcStartMask, nMasktemp, nTemp;
					l_width = (src->width+3)>>2;
					l_maskwidth = (src->width+7)>>3;
					ptgtstart16 = ptgt16= (UINT16 *)(tgt->buf+ (tgt->width*ty+tx)*2);  //6)
					psrcstart8 = psrc8 = src->buf + (sx>>2) + sy*l_width; 
					psrcStartMask = psrcMask = src->maskbuf+BYTE_POS_MASK(sx)+sy*l_maskwidth; 
					switch (mode) {
						case IMG_COPY:
							nMaskBitPos = BIT_POS_MASK(sx);
							nBitPos = BIT_POS_4G(sx);    //3)
							if (w>8)
								tempwidth = w-8;
							else tempwidth = 0;
							for(i=0; i<h; i++){
								if(nMaskBitPos>=4){		
									switch(nBitPos){
										case 0:
											if(!(*psrcMask & 0x10))
												*(ptgt16++) = b4GTo64K[(*psrc8 & 0x03)];
											else ptgt16++;
										case 2:
											if(!(*psrcMask & 0x20))
												*(ptgt16++) = b4GTo64K[(*psrc8 & 0x0C)>>2];
											else ptgt16++;
										case 4:
											if(!(*psrcMask & 0x40))
												*(ptgt16++) = b4GTo64K[(*psrc8 & 0x30)>>4];
											else ptgt16++;
										case 6:
											if(!(*psrcMask & 0x80))
												*(ptgt16++) = b4GTo64K[(*psrc8 & 0xC0)>>6];
											else ptgt16++;
										psrc8++;
										psrcMask++;
										break;
									}
								}

								else{					
									switch(nBitPos){
									case 0:
										if(!(*psrcMask & 0x01))
											*(ptgt16++) = b4GTo64K[(*psrc8 & 0x03)];
										else ptgt16++;
									case 2:
										if(!(*psrcMask & 0x02))
											*(ptgt16++) = b4GTo64K[(*psrc8 & 0x0C)>>2];
										else ptgt16++;
									case 4:
										if(!(*psrcMask & 0x04))
											*(ptgt16++) = b4GTo64K[(*psrc8 & 0x30)>>4];
										else ptgt16++;
									case 6:
										if(!(*psrcMask & 0x08))
											*(ptgt16++) = b4GTo64K[(*psrc8 & 0xC0)>>6];
										else ptgt16++;
										if(w<5) break;
										psrc8++;
										if (!(*psrcMask & 0x10))
											*(ptgt16++) = b4GTo64K[*psrc8 & 0x03]; 
										else ptgt16++;
										if(w<6) break;
										if (!(*psrcMask & 0x20))
											*(ptgt16++) = b4GTo64K[(*psrc8 & 0x0c) >> 2]; 
										else ptgt16++;
										if(w<7) break;
										if (!(*psrcMask & 0x40))
											*(ptgt16++) = b4GTo64K[(*psrc8 & 0x30) >> 4];
										else ptgt16++;
										if(w<8) break;	
										if (!(*psrcMask & 0x80))
											*(ptgt16++) = b4GTo64K[(*psrc8 & 0xc0) >> 6];
										else ptgt16++;

										psrc8++;
										psrcMask++;

										break;
									}
									if(w<(8-nMaskBitPos)){
										ptgt16 = ptgtstart16 += tgt->width;  //6)
										psrc8 = psrcstart8 += l_width; 
										psrcMask = psrcStartMask += l_maskwidth; 
										continue;							
									}

								}
								for(j=8-nMaskBitPos;j<tempwidth;j+=8){
//									if(j+8<w) break;
									if (!(*psrcMask & 0x01))
										*(ptgt16++) = b4GTo64K[*psrc8 & 0x03]; 
									else ptgt16++;

									if (!(*psrcMask & 0x02))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x0c) >> 2]; 
									else ptgt16++;

									if (!(*psrcMask & 0x04))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x30) >> 4];
									else ptgt16++;
									
									if (!(*psrcMask & 0x08))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0xc0) >> 6];
									else ptgt16++;

									psrc8++;
									if (!(*psrcMask & 0x10))
										*(ptgt16++) = b4GTo64K[*psrc8 & 0x03]; 
									else ptgt16++;
									if (!(*psrcMask & 0x20))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x0c) >> 2]; 
									else ptgt16++;

									if (!(*psrcMask & 0x40))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x30) >> 4];
									else ptgt16++;
									if (!(*psrcMask & 0x80))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0xc0) >> 6];
									else ptgt16++;
									psrc8++;
									psrcMask++;
								}
								nTemp = 0;
								if(w-j>=4){
									if (!(*psrcMask & 0x01))
										*(ptgt16++) = b4GTo64K[*psrc8 & 0x03]; 
									else ptgt16++;

									if (!(*psrcMask & 0x02))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x0c) >> 2]; 
									else ptgt16++;

									if (!(*psrcMask & 0x04))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x30) >> 4];
									else ptgt16++;
									
									if (!(*psrcMask & 0x08))
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0xc0) >> 6];
									else ptgt16++;

									psrc8++;
							
									nMasktemp = 4;
									for(j+=4;j<w;j++){
										if (!(*psrcMask & bitMask[nMasktemp]))
											*(ptgt16++) = b4GTo64K[(*psrc8&m_abMask4G[nTemp])>>nTemp]; 
										else ptgt16++;
										nMasktemp++;
										nTemp+=2;
									}
								}
								else{
									nMasktemp = 0;
									for(;j<w;j++){
										if (!(*psrcMask & bitMask[nMasktemp]))
											*(ptgt16++) = b4GTo64K[(*psrc8&m_abMask4G[nTemp])>>nTemp]; 
										else ptgt16++;
										nMasktemp++;
										nTemp +=2;
									}
								}
								ptgt16 = ptgtstart16 += tgt->width;  //6)
								psrc8 = psrcstart8 += l_width; 
								psrcMask = psrcStartMask += l_maskwidth; 
							}
							break;
						case IMG_AND: 
//							*ptgt8 &= b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
						case IMG_OR	:
//							*ptgt8 |=  b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
						case IMG_XOR : 
//							*ptgt8 ^=  b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
					}
				}
				else{
					UINT16 l_width, tempwidth, *ptgtstart16;
					BYTE *psrcstart8,nTemp;
					l_width = BYTE_POS_4G((src->width+3));
					ptgt16 = ptgtstart16 = (UINT16 *)tgt->buf+ tgt->width*ty+tx;  //6)
					psrc8 = psrcstart8 = src->buf + BYTE_POS_4G(sx) + sy*l_width; 

					switch (mode) {
						case IMG_COPY:
							nBitPos = BIT_POS_4G(sx);    //3)
							if (w>4)
								tempwidth = w-4;
							else tempwidth = 0;
							for(i=0; i<h; i++){
								switch(nBitPos){
									case 0:
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x03)];
									case 2:
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x0C)>>2];
									case 4:
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0x30)>>4];
									case 6:
										*(ptgt16++) = b4GTo64K[(*psrc8 & 0xC0)>>6];
									psrc8++;
									break;
								}
								for(j=4-(nBitPos>>1);j<=tempwidth;j+=4){
									*(ptgt16++) = b4GTo64K[*psrc8 & 0x03]; 
									*(ptgt16++) = b4GTo64K[(*psrc8 & 0x0c) >> 2]; 
									*(ptgt16++) = b4GTo64K[(*psrc8 & 0x30) >> 4];
									*(ptgt16++) = b4GTo64K[(*psrc8 & 0xc0) >> 6];
									psrc8++;
								}
								nTemp = 0;
								for(;j<w;j++){
									*(ptgt16++) = b4GTo64K[(*psrc8&m_abMask4G[nTemp])>>nTemp]; 
									nTemp+=2;
								}

								ptgt16 = ptgtstart16 += tgt->width;  //6)
								psrc8 = psrcstart8 += l_width; 
							}
							break;
						case IMG_AND: 
//							*ptgt8 &= b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
						case IMG_OR	:
//							*ptgt8 |=  b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
						case IMG_XOR : 
//							*ptgt8 ^=  b4GTo256[(*psrc8&m_abMask4G[nBitPos])>>nBitPos]; //6)
							break;
					}
				}
			break;
				// 이상 4gray -> 64KC
		}
	}				// 이상 4gray -> ??? 
	else if(src->type == IMAGE_TYPE_256C){
		switch(tgt->type){
			case IMAGE_TYPE_4G:             //  256 ->    4gray
				switch(mode){
					case IMG_COPY:
						for(i=0; i<h;i++){
							psrc8 = src->buf + src->width*(sy+i)+sx;
							wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);
							wMaskBytePos = TOT_BYTE_POS_MASK(tx, ty+i,tgt->width);
							for(j=0;j<w;j++){
								BYTE lcdgray;
								nBitPos = BIT_POS_4G(tx+j);
								nMaskBitPos =  BIT_POS_MASK (tx+j);
								if(!src->mask || *psrc8!=src->transcolor){
									if(tgt->mask){
										tgt->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos]; // 5)
									}	
									CLR8TOLCDGRAY2(*psrc8,lcdgray);
									tgt->buf[wBytePos] &= ~m_abMask4G[nBitPos];
									tgt->buf[wBytePos] |= lcdgray<<nBitPos;
								}
								psrc8++;
								if(nBitPos==6)	wBytePos ++;
								if(nMaskBitPos == 7) wMaskBytePos++;
							}
						}
						break;
							
					case IMG_AND: //6)
						for(i=0; i<h;i++){
							psrc8 = src->buf + src->width*(sy+i)+sx;
							wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);
							wMaskBytePos = TOT_BYTE_POS_MASK(tx, ty+i,tgt->width);
							for(j=0;j<w;j++){
								BYTE lcdgray, temp ;
								nBitPos = BIT_POS_4G (tx+j);
								nMaskBitPos =  BIT_POS_MASK(tx+j);
								if(!src->mask ||*psrc8!=src->transcolor){
									if(tgt->mask){
										tgt->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos]; // 5)
									}
									CLR8TOLCDGRAY2(*psrc8,lcdgray);
									temp = ~m_abMask4G[nBitPos];
									temp |= lcdgray<<nBitPos;
									tgt->buf[wBytePos] &= temp;
								}
								psrc8++;
								if(nBitPos==6) wBytePos ++;
								if(nMaskBitPos == 7) wMaskBytePos++;
							}
						}
						break;
					case IMG_OR	:
						for(i=0; i<h; i++){
							psrc8 = src->buf + src->width*(sy+i)+sx;
							wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);
							wMaskBytePos = TOT_BYTE_POS_MASK(tx, ty+i,tgt->width);
							for(j=0;j<w;j++){
								BYTE lcdgray;
								nBitPos = BIT_POS_4G (tx+j);
								nMaskBitPos =  BIT_POS_MASK(tx+j);
								if(!src->mask ||*psrc8!=src->transcolor){
									if(tgt->mask){
										tgt->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos]; // 5)
									}
									CLR8TOLCDGRAY2(*psrc8,lcdgray);
									tgt->buf[wBytePos]|= (lcdgray<<nBitPos);
								}
								psrc8++;
								if(nBitPos==6) wBytePos ++;
								if(nMaskBitPos == 7) wMaskBytePos++;
							}
						}
						break;
					case IMG_XOR : 
						for(i=0; i<h; i++){
							psrc8 = src->buf + src->width*(sy+i)+sx;
							wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);
							wMaskBytePos = TOT_BYTE_POS_MASK(tx, ty+i,tgt->width);
							for(j=0;j<w;j++){
								BYTE lcdgray;
								nBitPos = BIT_POS_4G (tx+j);
								nMaskBitPos =  BIT_POS_MASK(tx+j);
								if(!src->mask ||*psrc8!=src->transcolor){
									if(tgt->mask){
										tgt->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos]; // 5)
									}
									CLR8TOLCDGRAY2(*psrc8,lcdgray);
									tgt->buf[wBytePos]^= (lcdgray<<nBitPos);
								}
								psrc8++;
								if(nBitPos==6)wBytePos ++;
								if(nMaskBitPos == 7) wMaskBytePos++;
							}
						}	
						break;
					}
				break;
				// 이상 256C -> 4gray 
			
			case IMAGE_TYPE_256C:             //    256   -> 256 
				switch (mode) {
					case IMG_COPY:
						if(src->mask)
						{
							BYTE *psrcorg8, *ptgtorg8;
							psrcorg8 = psrc8=src->buf+src->width*sy+sx;
							ptgtorg8 = ptgt8=tgt->buf+tgt->width*ty+tx;
							for(i=0; i<h; i++){
								for(j=0;j<w;j++){
									if(*psrc8 != src->transcolor)
										*ptgt8 = *psrc8;
									psrc8++; ptgt8++;
								}
								psrc8 = psrcorg8 += src-> width;
								ptgt8 = ptgtorg8 += tgt-> width;
							}
						}
						else{
							BYTE *psrcorg8, *ptgtorg8;
							psrcorg8 = psrc8=src->buf+src->width*sy+sx;
							ptgtorg8 = ptgt8=tgt->buf+tgt->width*ty+tx;
							for(i=0; i<h; i++){
//								memcpy(ptgt8,psrc8,w);
								for(j=0;j<w;j++){
									*ptgt8 = *psrc8;
									psrc8++; ptgt8++;
								}
								psrc8 = psrcorg8 += src-> width;
								ptgt8 = ptgtorg8 += tgt-> width;
							}
						}
						break;
					case IMG_AND: 
						for(i=0; i<h; i++){
							psrc8=src->buf+src->width*(sy+i)+sx;
							ptgt8=tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask|| *psrc8!=src->transcolor)
									*ptgt8 &= *psrc8;
								psrc8++; ptgt8++;
							}
						}
						break;
					case IMG_OR	:
						for(i=0; i<h; i++){
							psrc8=src->buf+src->width*(sy+i)+sx;
							ptgt8=tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask|| *psrc8!=src->transcolor)
									*ptgt8 |= *psrc8;
								psrc8++; ptgt8++;
							}
						}
						break;
					case IMG_XOR : 
						for(i=0; i<h; i++){
							psrc8=src->buf+src->width*(sy+i)+sx;
							ptgt8=tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask|| *psrc8!=src->transcolor)
									*ptgt8 ^= *psrc8;
								psrc8++; ptgt8++;
							}
						}	
						break;
				}
				break;
				// 이상 256C -> 256 C

			case IMAGE_TYPE_64KC:             //  256 ->    64KC
				switch (mode) {
					case IMG_COPY:
						if(src->mask){
							BYTE *psrcorg8;
							UINT16 *ptgtorg16;
							psrc8= psrcorg8 = src->buf+src->width*(sy+i)+sx;
							ptgt16= ptgtorg16 = (UINT16 *)tgt->buf+tgt->width*(ty+i)+tx;
							for(i=0; i<h; i++){
								for(j=0;j<w;j++){
									if(*psrc8!=src->transcolor)
										*ptgt16= cvt8b16b[*psrc8];
									psrc8++; ptgt16++;
								}
								psrc8 = psrcorg8 += src-> width;
								ptgt16 = ptgtorg16 += tgt-> width;
							}

						}
						else {
							BYTE *psrcorg8;
							UINT16 *ptgtorg16;
							psrc8= psrcorg8 = src->buf+src->width*(sy+i)+sx;
							ptgt16= ptgtorg16 = (UINT16 *)tgt->buf+tgt->width*(ty+i)+tx;
							for(i=0; i<h; i++){
								for(j=0;j<w;j++){
									*ptgt16= cvt8b16b[*psrc8];
									psrc8++; ptgt16++;
								}
								psrc8 = psrcorg8 += src-> width;
								ptgt16 = ptgtorg16 += tgt-> width;
							}
						}
						break;
					case IMG_AND: 
						for(i=0; i<h; i++){
							psrc8=src->buf+src->width*(sy+i)+sx;
							ptgt16=(UINT16 *)tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask||*psrc8!=src->transcolor)
									*ptgt16 = cvt8b16b[*psrc8];
								psrc8++; ptgt16++;
							}
						}
						break;
					case IMG_OR	:
						for(i=0; i<h; i++){
							psrc8=src->buf+src->width*(sy+i)+sx;
							ptgt16=(UINT16 *)tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask||*psrc8!=src->transcolor)
									*ptgt16|= cvt8b16b[*psrc8];
								psrc8++; ptgt16++;
							}
						}
						break;
					case IMG_XOR : 
						for(i=0; i<h; i++){
							psrc8=src->buf+src->width*(sy+i)+sx;
							ptgt16=(UINT16 *)tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask||*psrc8!=src->transcolor)
									*ptgt16^= cvt8b16b[*psrc8];
								psrc8++; ptgt16++;
							}
						}
						break;
				}
				break;
		}		
	}			// 이상 256c-> ??? 
	else if(src->type == IMAGE_TYPE_64KC){   //   64KC  -> ??
		switch(tgt->type){
			case IMAGE_TYPE_4G:               //   64K  -> 4gray
				switch(mode){
					BYTE lcdgray,temp;

					case IMG_COPY:
						if(src->mask){
							for(i=0; i<h;i++){
								psrc16 = (UINT16 *)src->buf+ src->width*(sy+i)+sx;
								wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);
								if(tgt->mask)
									wMaskBytePos = TOT_BYTE_POS_MASK(tx, ty+i,tgt->width);

								for(j=0;j<w;j++){
									nBitPos = BIT_POS_4G(tx+j);
									if(*psrc16 !=src->transcolor){
										CLR16TOLCDGRAY2(*psrc16,lcdgray);
										tgt->buf[wBytePos] &= ~m_abMask4G[nBitPos];
										tgt->buf[wBytePos] |= lcdgray<<nBitPos;
									}
									psrc16++;
									if(nBitPos==6){
										wBytePos ++;
									}
									if(nMaskBitPos == 7){
										wMaskBytePos++;
									}
								}
							}
						}
						else{
							for(i=0; i<h;i++){
								psrc16 = (UINT16 *)src->buf+ src->width*(sy+i)+sx;
								wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);

								for(j=0;j<w;j++){
									nBitPos = BIT_POS_4G(tx+j);
									nMaskBitPos =  BIT_POS_MASK (tx+j);

									CLR16TOLCDGRAY2(*psrc16,lcdgray);
									tgt->buf[wBytePos] &= ~m_abMask4G[nBitPos];
									tgt->buf[wBytePos] |= lcdgray<<nBitPos;

									psrc16++;
									if(nBitPos==6){
										wBytePos ++;
									}
									if(nMaskBitPos == 7){
										wMaskBytePos++;
									}
								}
							}
						}
						break;
							
					case IMG_AND: //6)
						for(i=0; i<h;i++){
							psrc16 = (UINT16 *)src->buf+ src->width*(sy+i)+sx;
							wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);
							wMaskBytePos = TOT_BYTE_POS_MASK(tx, ty+i,tgt->width);

							for(j=0;j<w;j++){
								nBitPos = BIT_POS_4G(tx+j);
								nMaskBitPos =  BIT_POS_MASK (tx+j);
								if(!src->mask ||	*psrc16!=src->transcolor){
									if(tgt->mask){
										tgt->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos]; // 5)
									}	
									CLR16TOLCDGRAY2(*psrc16,lcdgray);
									temp = ~m_abMask4G[nBitPos];
									temp |= lcdgray<<nBitPos;
									tgt->buf[wBytePos] &= temp;
								}
								psrc16++;
								if(nBitPos==6){
									wBytePos ++;
								}
								if(nMaskBitPos == 7){
									wMaskBytePos++;
								}
							}
						}
						break;
					case IMG_OR	:
						for(i=0; i<h; i++){
							psrc16 = (UINT16 *)src->buf+ src->width*(sy+i)+sx;
							wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);
							wMaskBytePos = TOT_BYTE_POS_MASK(tx, ty+i,tgt->width);

							for(j=0;j<w;j++){
								nBitPos = BIT_POS_4G(tx+j);
								nMaskBitPos =  BIT_POS_MASK (tx+j);
								if(!src->mask ||	*psrc16!=src->transcolor){
									if(tgt->mask){
										tgt->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos]; // 5)
									}	
									CLR16TOLCDGRAY2(*psrc16,lcdgray);
									tgt->buf[wBytePos]|= (lcdgray<<nBitPos);
								}
								psrc16++;
								if(nBitPos==6){
									wBytePos ++;
								}
								if(nMaskBitPos == 7){
									wMaskBytePos++;
								}
							}
						}
						break;
					case IMG_XOR : 
						for(i=0; i<h; i++){
							psrc16 = (UINT16 *)src->buf+ src->width*(sy+i)+sx;
							wBytePos = TOT_BYTE_POS_4G(tx, ty+i,tgt->width);
							wMaskBytePos = TOT_BYTE_POS_MASK(tx, ty+i,tgt->width);

							for(j=0;j<w;j++){
								nBitPos = BIT_POS_4G(tx+j);
								nMaskBitPos =  BIT_POS_MASK (tx+j);
								if(!src->mask ||	*psrc16!=src->transcolor){
									if(tgt->mask){
										tgt->maskbuf[wMaskBytePos] &= ~bitMask[nMaskBitPos]; // 5)
									}	
									CLR16TOLCDGRAY2(*psrc16,lcdgray);
									tgt->buf[wBytePos]^= (lcdgray<<nBitPos);
								}
								psrc16++;
								if(nBitPos==6){
									wBytePos ++;
								}
								if(nMaskBitPos == 7){
									wMaskBytePos++;
								}
							}
						}	
						break;
				}
				// 이상 64KC -> 4gray 
			case IMAGE_TYPE_256C:             //   64K  -> 256 
				switch (mode) {
					BYTE temp;
					case IMG_COPY:
						if(src->mask){
							for(i=0; i<h; i++){
								psrc16=(UINT16*)src->buf+src->width*(sy+i)+sx;
								ptgt8=tgt->buf+tgt->width*(ty+i)+tx;
								for(j=0;j<w;j++){
									if(*psrc16!=src->transcolor)
										CLR16TOCLR8(*psrc16,*ptgt8);
									psrc16++;
									ptgt8++;
								}
							}
						}
						else{
							for(i=0; i<h; i++){
								psrc16=(UINT16*)src->buf+src->width*(sy+i)+sx;
								ptgt8=tgt->buf+tgt->width*(ty+i)+tx;
								for(j=0;j<w;j++){
									CLR16TOCLR8(*psrc16,*ptgt8);
									psrc16++;
									ptgt8++;
								}
							}
						}
						break;
					case IMG_AND: 
						for(i=0; i<h; i++){
							psrc16=(UINT16*)src->buf+src->width*(sy+i)+sx;
							ptgt8=tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask|| *psrc16!=src->transcolor){
									CLR16TOCLR8(*psrc16,temp);
									*ptgt8 &=temp;
								}
								psrc16++;
								ptgt8++;
							}
						}
						break;
					case IMG_OR	:
						for(i=0; i<h; i++){
							psrc16=(UINT16*)src->buf+src->width*(sy+i)+sx;
							ptgt8=tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask|| *psrc16!=src->transcolor){
									CLR16TOCLR8(*psrc16,temp);
									*ptgt8 |=temp;
								}
								psrc16++;
								ptgt8++;
							}
						}
						break;
					case IMG_XOR : 
						for(i=0; i<h; i++){
							psrc16=(UINT16*)src->buf+src->width*(sy+i)+sx;
							ptgt8=tgt->buf+tgt->width*(ty+i)+tx;
							for(j=0;j<w;j++){
								if(!src->mask|| *psrc16!=src->transcolor){
									CLR16TOCLR8(*psrc16,temp);
									*ptgt8 ^=temp;
								}
								psrc16++;
								ptgt8++;
							}
						}
						break;
					}
				break;
				// 이상 64KC -> 256C
		case IMAGE_TYPE_64KC:             //   64K  -> 64KC
			switch (mode) {
				case IMG_COPY:
					if(src->mask){
						UINT16 *psrcorg16, *ptgtorg16;
						psrcorg16 = psrc16=(UINT16 *)src->buf+src->width*(sy)+sx;
						ptgtorg16 = ptgt16=(UINT16 *)tgt->buf+tgt->width*(ty)+tx;

						for(i=0; i<h; i++){
							for(j=0;j<w;j++){
								if(*psrc16!=src->transcolor)
									*ptgt16 = *psrc16;
								psrc16++;ptgt16++;
							}
							psrc16=psrcorg16+=src->width;
							ptgt16=ptgtorg16+=tgt->width;

						}

					}
					else{
						UINT16 *psrcorg16, *ptgtorg16;
						psrcorg16 = psrc16=(UINT16 *)src->buf+src->width*(sy)+sx;
						ptgtorg16 = ptgt16=(UINT16 *)tgt->buf+tgt->width*(ty)+tx;
						for(i=0; i<h; i++){
//							memcpy(ptgt16,psrc16,w*2);
							for(j=0;j<w;j++){
								*ptgt16 = *psrc16;
								psrc16++;ptgt16++;
							}
							psrc16=psrcorg16+=src->width;
							ptgt16=ptgtorg16+=tgt->width;

						}

					}
					break;

				case IMG_AND: 
					for(i=0; i<h; i++){
						psrc16=(UINT16 *)src->buf+src->width*(sy+i)+sx;
						ptgt16=(UINT16 *)tgt->buf+tgt->width*(ty+i)+tx;
						for(j=0;j<w;j++){
							if(!src->mask||*psrc16!=src->transcolor)
								*ptgt16 &= *psrc16;
							psrc16++;ptgt16++;
						}
					}
					break;
				case IMG_OR	:
					for(i=0; i<h; i++){
						psrc16=(UINT16 *)src->buf+src->width*(sy+i)+sx;
						ptgt16=(UINT16 *)tgt->buf+tgt->width*(ty+i)+tx;
						for(j=0;j<w;j++){
							if(!src->mask||*psrc16!=src->transcolor)
								*ptgt16 |= *psrc16;
							psrc16++;ptgt16++;
						}
					}
					break;
				case IMG_XOR : 
					for(i=0; i<h; i++){
						psrc16=(UINT16 *)src->buf+src->width*(sy+i)+sx;
						ptgt16=(UINT16 *)tgt->buf+tgt->width*(ty+i)+tx;
						for(j=0;j<w;j++){
							if(!src->mask||*psrc16!=src->transcolor)
								*ptgt16 ^= *psrc16;
							psrc16++;ptgt16++;
						}
					}
					break;
			}
			break;
		}
			// 이상 64KC ->64KC
	// 이상 64KC -> ??
	}
	return;
}


// for imagedrawtext 를 위한 함수들. 

extern BYTE* EQS_API EQS_SetSKVMTextDrawMode(CONST BYTE* pSrc, INT8 nWidthByte, INT8 nHeight);
extern CONST BYTE*  EQS_API EQS_GetFontBuffer(WORD wCode, CONST BITMAP_FONT *pBitmapFont);
extern CONST BITMAP_FONT* EQS_API EQS_GetBitmapFontByCode(WORD wCode);

LOCAL VOID PutBitmapToImg(Image *im, COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth, yHeight;
	XImage *img = (XImage *) im ; 
    COORD yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    BYTE bImageData, gray4, nBitPos;
    int	i,j, tempi;
	int xgap=0, ygap=0, xlimit;

    BYTE *pbBaseData8, l_ForeColor;
	UINT16 * pbBaseData16;
    BYTE* pbFontImageData;

    ROM BYTE abBWMask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
 
    COLOR ForeColor = EQS_GetTextColor();
	if (img->type == IMAGE_TYPE_4G) /* 4 grey-scale */
		CLR16TOLCDGRAY2(ForeColor, gray4);
	else if(img->type == IMAGE_TYPE_256C)
		CLR16TOCLR8(ForeColor, l_ForeColor);
	
	if(x > img->width-1)
        return;

	xWidth = pBitmap->Header.xWidth; 
    yHeight = pBitmap->Header.Size.yHeight;
 
	ROBUSTCLIP(img);
	if(x<img->clipx1){
		if(x+xWidth-1>img->clipx2){
			xgap = img->clipx1 -x;
			x = img-> clipx1;
			xWidth = img-> clipx2 - img->clipx1;
		}
		else{
			xgap = img->clipx1 - x;
			x = img-> clipx1;
			xWidth = xWidth - xgap;
		}
	}
	else {
		if(x+xWidth -1>img->clipx2){
			xWidth = img-> clipx2 -x;
		}
	}
	if(y<img->clipy1){
		if(y+yHeight-1>img->clipy2){
			ygap = img->clipy1 -y ;
			y = img-> clipy1;
			yHeight = img-> clipy2 - img->clipy1;
		}
		else{
			ygap = img->clipy1 - y;
			y = img-> clipy1;
			yHeight = yHeight - ygap;
		}
	}
	else {
		if(y+yHeight-1>img->clipy2){
			yHeight = img-> clipy2 -y;
		}
	}

   if (xWidth <= 0 || yHeight <= 0) return;
	xlimit = xWidth+xgap ;
	if (img->type == IMAGE_TYPE_4G){ // 4 grey-scale 
		for(yOffset = 0; yOffset < yHeight; yOffset++)
		{
			if((y < 0) || ((y + yOffset) > img->height-1))
				break;//continue;
			nBytePos = INDEX_ON_IMG(x, y+yOffset, img->width);
			pbBaseData8 = img->buf + TOT_BYTE_POS_4G(x,y+yOffset, img->width);
			nBitPos = BIT_POS_4G(x);

			pbFontImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yOffset+ygap);

			for(i = 0; (tempi=(i<<3))<xlimit; i++){
				// image data 
				bImageData = *(pbFontImageData + i);
				if(i){
					for(j = 0; (tempi+j) < xlimit && j < 8; j++){
						if(bImageData & abBWMask[j]){
							*pbBaseData8 &= m_abNotMask4G[nBitPos];
							*pbBaseData8 |= gray4<< nBitPos;
						}
						nBitPos += 2;
						if(nBitPos==8){
							nBitPos =0;
							pbBaseData8++;
						}
					}
				}
				else{
					for(j = xgap; j < xlimit && j <8; j++) {
						if(bImageData & abBWMask[j]){
							*pbBaseData8 &= m_abNotMask4G[nBitPos];
							*pbBaseData8 |= gray4<< nBitPos;
						}
						nBitPos += 2;
						if(nBitPos==8){
							nBitPos =0;
							pbBaseData8++;
						}
					}
				}
			}
		}
	}
	else if(img->type == IMAGE_TYPE_256C){
		for(yOffset = 0; yOffset < yHeight; yOffset++)
		{
			if((y < 0) || ((y + yOffset) > img->height-1))
				break;//continue;

			nBytePos = INDEX_ON_IMG(x, y+yOffset, img->width);
			pbBaseData8 = img->buf + nBytePos;
			pbFontImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yOffset+ygap);

			for(i = 0; (tempi =(i<<3))<xlimit; i++){
				// image data 
				bImageData = *(pbFontImageData + i);
				if(i){
					for(j = 0; (tempi+j) < xlimit && j < 8; j++){
						if(bImageData & abBWMask[j])
							*(pbBaseData8++) = l_ForeColor;
						else pbBaseData8++;
					}
				}
				else{
					for(j = xgap; j <xlimit && j < 8; j++) {
						if(bImageData & abBWMask[j])
							*(pbBaseData8++) = l_ForeColor;
						else pbBaseData8++;
					}
				}
			}
		}
	}
	else{
		for(yOffset = 0; yOffset < yHeight; yOffset++)
		{
			if((y < 0) || ((y + yOffset) > img->height-1))
				break;//continue;

			nBytePos = INDEX_ON_IMG(x, y+yOffset, img->width);
			pbBaseData16 = (UINT16 *)img->buf + nBytePos;
		
			pbFontImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yOffset+ygap);

			for(i = 0; (tempi = (i<<3))<xlimit; i++){
				// image data 
				bImageData = *(pbFontImageData + i);
				if(i){
					for(j = 0; (tempi+j) < xlimit && j < 8; j++){
						if(bImageData & abBWMask[j])
							*(pbBaseData16++) = ForeColor;
						else
							pbBaseData16++;
					}
				}
				else{
					for(j = xgap; j < xlimit && j < 8; j++) {
						if(bImageData & abBWMask[j])
							*(pbBaseData16++) = ForeColor;
						else pbBaseData16++;
					}
				}
			}
		}
    }
} 

void PutBitmapFontToImg(Image *im, COORD x, COORD y, WORD wCode, 
                                 CONST BITMAP_FONT* pBitmapFont)
{
    BITMAP Bitmap;
	LOCAL BOOL l_fPutFont;

    /* make a bitmap from a bitmap font buffer */
    //Bitmap.Header.Type = BT_FONT;
    Bitmap.Header.nBitsPerPixel = pBitmapFont->nBitsPerPixel;
    Bitmap.Header.Size.xWidth = PIXEL2BYTES(pBitmapFont->Size.xWidth) * 8;
    Bitmap.Header.Size.yHeight = pBitmapFont->Size.yHeight;
    Bitmap.Header.xWidth = pBitmapFont->Size.xWidth;
    /* get the start pointer of the font data of wCode */
    /* we can only set text draw mode about the black and white font */
    if(pBitmapFont->nBitsPerPixel == 1) 
    {
        //sSetTextDrawMode(&(Bitmap.pbData), sGetFontBuffer(wCode, pBitmapFont), 
        Bitmap.pbData = EQS_SetSKVMTextDrawMode(EQS_GetFontBuffer(wCode, pBitmapFont), 
                        (INT8)PIXEL2BYTES(pBitmapFont->Size.xWidth),
                        (INT8)pBitmapFont->Size.yHeight);
        /* --------------------------------------------
        ** convert bw font image into 4 gray font image 
        ** for accelerating display speed 
        */
    }
    else
    {
        Bitmap.pbData = EQS_GetFontBuffer(wCode, pBitmapFont);
    }
	l_fPutFont = TRUE;
    PutBitmapToImg(im, x, y, &Bitmap);
    l_fPutFont = FALSE;
}


void DrawTextToImg( Image *im, int x, int y, char *text, 
			    int offset, int len, PIXEL col32)
{
	UINT16 col16;
    CONST BITMAP_FONT* pBitmapFont;
    BYTE bChar;
    WORD wCode;
    BYTE *pszString = (BYTE *)(text+offset);
    int count = len;
    XImage *img = (XImage *)im;	

	CLR32TOCLR16(col32, col16);
	EQS_SetTextColor(col16);

    while((BOOL)(bChar = *pszString++) &&count--)
    {
        if(bChar & 0x80) /* multibyte character */
        {
            /* check valid multibyte character */
            //ASSERT(*pszString & 0x80);
            wCode = (bChar << 8) | *(pszString++);
            pBitmapFont = EQS_GetBitmapFontByCode(wCode);
            PutBitmapFontToImg((Image *)img, x, y, wCode, pBitmapFont);
            x += pBitmapFont->Size.xWidth;
        }
        else
        {
            pBitmapFont = EQS_GetBitmapFontByCode((WORD)bChar);
            PutBitmapFontToImg((Image *)img,x, y, (WORD)bChar, pBitmapFont);
            x += pBitmapFont->Size.xWidth;
        }
    }
}

/*************************************************************
* function: imageDrawTEXT
* desc:  	draw a string in the image at the specified position 
*			the string is given as a KSC5601 encoded string
*			font size is given as follows: 
*			( FONT_XLARGE, FONT_LARGE, FONT_MEDIUM, FONT_SMALL )
*			CLIPPING VALUES OF THE TARGET IMAGE MUST BE CONSIDERED 
* input:	image(img), string position(x,y), string(text) 
*			string offset, string length, font size, color	
* return: 	nothing	
**************************************************************/
void	imageDrawText( Image *im, int x, int y, char *text, 
			int offset, int len, BYTE fsize, PIXEL col32  )
{
    // set the properties by the current DC's properties 
    // 여기서는 Text를 뿌릴때 한줄 단위로 뿌리고 있습니다.
    // 그래서 한줄에 들어갈 수 있는 글자수를 최대 30이라고 잡았음..
    static char temp [30];
    int i;
	// FONT 크기를 정한다. 
	switch(fsize){
	case  FONT_LARGE:
			SetFontType(FT_BIG);
		break;
    case FONT_SMALL:
			SetFontType(FT_SMALL);
    	break;
    case FONT_XLARGE:
			SetFontType(FT_BIGBIG);
    	break;
    case FONT_MEDIUM:
    default:
			SetFontType(FT_MED);
    	break;
	}	

    memset(temp, 0x00, 30);
    for(i = 0; i < len; i++) {
        WORD ch = (BYTE)text[i];
        if (ch >= 0x20) {
            temp[i] = text[i];
        }
        else
            temp[i] = 0x20;
    }
    DrawTextToImg(im,x,y,temp,offset,i,col32);
	//EQS_DrawTextToImg(im,x,y,text,offset,len,col);
}
/*************************************************************
* function: getTextWidth
* desc:  	return the pixel width of the string when drawn
*			the string is given as a KSC5601 encoded string
*			font size is given as follows: 
*			( FONT_XLARGE, FONT_LARGE, FONT_MEDIUM, FONT_SMALL )
* input:	string(text), string offset, string length, font size
* return: 	nothing	
*   10 x 21 : BIG BIG FONT (ascii only)
**   14 x 16 : BIG FONT (hangul, special, sky symbol)
**    7 x 16 : BIG FONT (ascii)
**   12 x 13 : SYSTEM FONT (hangul, special, sky symbol)
**    6 x 13 : SYSTEM FONT (ascii)
* 
*checked  by tortol
**************************************************************/
int getTextWidth( char *text, int offset, int length, int fsize )
{
	int width = 0;
	int i;
  
	for( i=0; i<length; i++)
	{
		char c = text[offset+i]; 
		switch( fsize )
		{
		case FONT_LARGE:
			//FONT_14x16;
			if ( c & 0x80 )
			{
				i++;
				width += 14;
			}
			else
				width += 7;
			break;
		case FONT_SMALL:
			//FONT_6x8;
			width += 6;
			break;
		case FONT_XLARGE:
			//FONT_10x21;
			width += 10;
			break;
		case FONT_MEDIUM:
		default:
			//FONT_12x13;
			if ( c & 0x80 )
			{
				i++;
				width += 12;
			}
			else {
				width += 6;
			}
			break;
		}
	}
	return width;
} 

/*************************************************************
* function: getFontHeight
* desc:  	return the pixel height of the given font 
*			font size is given as follows: 
*			( FONT_XLARGE, FONT_LARGE, FONT_MEDIUM, FONT_SMALL )
* input:	font size
* return: 	nothing	
*checked 
*    10 x 21 : BIG BIG FONT (ascii only)
**   14 x 16 : BIG FONT (hangul, special, sky symbol)
**    7 x 16 : BIG FONT (ascii)
**   12 x 13 : SYSTEM FONT (hangul, special, sky symbol)
**    6 x 13 : SYSTEM FONT (ascii)
***************************************************************/
int getFontHeight( int fsize )
{
		    
	switch( fsize )
	{
	case FONT_LARGE:
		//FONT_14x16;
		return 16;
	case FONT_SMALL:
		//FONT_6x9;
		return 9;
	case FONT_XLARGE:
		//FONT_10x21;
		return 21;
	case FONT_MEDIUM:
	default:
		//FONT_12x13;
		return 13;
	}
}


/*****************************************************
 * function  : getLCDInfo
 * desc  : get the width and height of lcd device  
 *       in pixel
 * parameter : integer pointer of width, height, color
 *               flag, depth of bitmap plane 
 *               width(lcd pixel width) 
 *               height(lcd pixel height) 
 *               isColor(0 for gray, 1 for color )
 *               depth(1 for BW, 2 for 4-gray, 4 for 16-gray or
 *                          16-color, 8 for 256-gray or 256-color)       
 * return  : nothing
 ******************************************************/
 
void getLCDInfo( int *width, int *height, int *isColor, int *depth )
{
	*width =  SKY_SCREEN_WIDTH;
#ifdef SUPPORT_ANNUNCIATORS_IN_SKVM
    *height = SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT;
#else
    *height = SKY_SCREEN_HEIGHT;
#endif

    *isColor = 1;

	if(!SKY_GET_SKVMCOLORENV()){  
	    *depth = 8; // 8bit
    }
	else{
		*depth = 16;	// 16bit color
	}
}
/*************************************************************
* function: imageFileCheck
* desc: 	check if the given array is a image file
* input:	image array
* return:	1(TRUE) or 0(FALSE)
**************************************************************/

BYTE imageFileCheck( BYTE *data )
{
	if(memcmp( data+1, "PNG", 3 ) == 0) 
		return ITYPE_PNG;
	else if (memcmp( data, "LBMP", 4 ) == 0)
		return ITYPE_LBM;
	else if (memcmp( data, "IMG", 3 ) == 0)
		return ITYPE_EBI;
	return 0;
}


/*************************************************************
* function: png2Image
* desc:		fill the image data with png information
* input:	img, png_data, width, height, trans_color, maskflag
* return:	none
**************************************************************/
typedef unsigned short PNGPixel;

void png2Image(Image *im, PNGPixel *png_data,
	int w, int h, int y, PNGPixel transparent, int maskflag)
{
    /* setup img with png information */
	// PNG pixel은 한 줄 씩 읽어서 png data 에 넣어 준다. 
	XImage *img = (XImage *)im;
	int i,j, off = w * y;
	BYTE trans8, val8, gray4, *buf8, *mask8;
	UINT16 *buf16 = (UINT16 *)img->buf;
	LOCAL INT16 wBytePos, wByteEndPos;
	LOCAL INT8 nMaskBitPos, nBitPos, nBitEndPos;
	
	if(image_create_mode==IMAGE_TYPE_4G){ /* 4 grey-scale */
		wBytePos = TOT_BYTE_POS_4G(0,y,w);
		nBitPos = BIT_POS_4G(0);
		wByteEndPos = TOT_BYTE_POS_4G(w-1, y, w);
		nBitEndPos = BIT_POS_4G(w-1);

		if(maskflag){
			// 시작 부터 마지막 바이트 앞까지.. 
			nMaskBitPos =0 ;
			buf8 = img->buf + ((w+3)>>2)*y;
			mask8 = img->maskbuf + ((w+7)>>3)*y;
			*mask8 = 0;
			for(j=wBytePos;j<wByteEndPos;j++){
				*buf8 = 0;
				if(*png_data == transparent){
					*mask8 |= bitMask[nMaskBitPos];
				}
				else{
					CLR16TOLCDGRAY2(*png_data,gray4);
					*buf8 &= m_abNotMask4G[0];
					*buf8 |= gray4<<0;
				}
				png_data++, nMaskBitPos++;

				if(*png_data == transparent){
					*mask8 |= bitMask[nMaskBitPos];
				}
				else{
					CLR16TOLCDGRAY2(*png_data,gray4);
					*buf8 &= m_abNotMask4G[2];
					*buf8 |= gray4<<2;
				}
				png_data++, nMaskBitPos++;

				if(*png_data == transparent){
					*mask8 |= bitMask[nMaskBitPos];
				}
				else{
					CLR16TOLCDGRAY2(*png_data,gray4);
					*buf8 &= m_abNotMask4G[4];
					*buf8 |= gray4<<4;
				}
				png_data++, nMaskBitPos++;

				if(*png_data == transparent){
					*mask8 |= bitMask[nMaskBitPos];
				}
				else{
					CLR16TOLCDGRAY2(*png_data,gray4);
					*buf8 &= m_abNotMask4G[6];
					*buf8 |= gray4<<6;
				}

				png_data++, nMaskBitPos++;
				if((nMaskBitPos) == 8){
					nMaskBitPos=0;
					mask8++;
					*mask8 = 0;
				}
				buf8++;
			}

			// 마지막 바이트 
			*buf8 =0;
			for(i=0;i<=nBitEndPos;i+=2){
				if(*png_data == transparent){
					// bw 포지션 찾아서.. 비트만 셋.. 
					*mask8 |= bitMask[nMaskBitPos];
					*buf8 &= m_abNotMask4G[i];
				}
				else{
					*mask8 &= nbitMask[nMaskBitPos];
					CLR16TOLCDGRAY2(*png_data,gray4);
					*buf8 &= m_abNotMask4G[i];
					*buf8 |= gray4<<i;
				}
				png_data++;
				if((++nMaskBitPos) == 8){
					nMaskBitPos=0;
					mask8++;
				}
			}
		}
		else{ // mask 없을 때 
			// 시작 부터 마지막 바로 앞까지..
			buf8=img->buf+wBytePos;
			for(j=wBytePos;j<wByteEndPos;j++){
				*buf8= 0;
				CLR16TOLCDGRAY2(*png_data,gray4);
				*buf8 |= gray4;
				png_data++;
				CLR16TOLCDGRAY2(*png_data,gray4);
				*buf8 |= gray4<<2;
				png_data++;
				CLR16TOLCDGRAY2(*png_data,gray4);
				*buf8 |= gray4<<4;
				png_data++;
				CLR16TOLCDGRAY2(*png_data,gray4);
				*buf8 |= gray4<<6;
				png_data++;
				buf8++;
			}
			val8=0;
			// 마지막 byte
			for(i=0;i<=nBitEndPos;i+=2){
				CLR16TOLCDGRAY2(*png_data,gray4);
				val8 |= gray4<<i;
				png_data++;
			}
			*buf8 |= val8;
		}
	}
	else if(image_create_mode==IMAGE_TYPE_256C){ /* 4 grey-scale */ 
		CLR16TOCLR8(transparent,trans8);
		img->transcolor = trans8;
		buf8 = img->buf + off;
		for (i = 0; i < w; i++) // 한 라인 안에 있는 모든 pixel에 대하여
			CLR16TOCLR8(png_data[i],*buf8++);
	}
	else {
		img->transcolor=transparent;
		memcpy(buf16+off, png_data, w *2);
	}
	return;
}
/*************************************************************
* function: imageFileDefaultExt
* desc: 	return default extenstion of system image file
* input:	none
* return:	extenstion
* checked 
**************************************************************/
char *imageFileDefaultExt(void)
{
	return ".EBI";
}



#define MAX_XVM_MSG_LEN		64
void console_init(void) { }
void console_print(char *s, int len) {
    static char msgbuf[MAX_XVM_MSG_LEN+1];
    static msg_store_type xx_store =
        {MSG_LVL_HIGH, __LINE__, __FILE__, (const char *)msgbuf};
    byte level = MSG_LVL_HIGH;	// FIXME: 적절한 레벨 설정
 
    if (len < 0) len = strlen(s);

    if ((!(level < msg_limit)) && (!(level < MSG_THRESHOLD))) {
	xx_store.level = level;
        while(len > 0) {
            memcpy(msgbuf, s, MAX_XVM_MSG_LEN);
            msgbuf[MAX_XVM_MSG_LEN] = '\0';
            msg_put(&xx_store, (dword)0, (dword)0, (dword)0);
            len -= MAX_XVM_MSG_LEN;
            s += MAX_XVM_MSG_LEN;
        }
		MSG_ERROR(msgbuf,0,0,0);
    }
}


#ifdef LBM_SUPPORTED    //     2bit연속 방식을 쓴경우 .. 
/*************************************************************
* function: imageLBMFileInfo
* desc:
* input:
* return:
**************************************************************/
int imageLBMDataOpen(BYTE *data, int datasize, void *sbuf, int bufsize,
    int *w, int *h, int *m)
{
    int width;
    int height;
    int mask;

    if ( memcmp( data,"LBMP", 4 ) != 0  )
    {
    	_jlog("invalid LBM file");
    	return -1; 
    }
    data += 8;
    memcpy( &width, data, 4 ); data += 4;
    if ( width < 0 )
    {
    	_jlog("invalid LBM width %d", width );
    	return -3;
    }

    memcpy( &height, data, 4 ); data += 4;
    if ( height < 0 )
    {
    	_jlog("invalid LBM height %d", height );
    	return -4;
    }

    data += 4; /* size */
    memcpy( &mask, data, 4 ); data += 4;
	
    *w = width;
    *h = height;
    *m = mask;


	return 0;
}

/*************************************************************
* function: imageLBMFileRead
* desc:     read a byte array and create a new image from it
* input:    image array
* return:    new image or NULL when failed 
**************************************************************/
int imageLBMDataRead(BYTE *data, int datasize, void *sbuf, int bufsize,
    Image *im, int width, int height, int mask)
{
	register BYTE *pFirst, *pSecond, *pMask, *pTemp;
	register BYTE *start = data ;
    register unsigned int  i, j, k;
	int bsize, size, type;
	LOCAL INT8 nBitPos;
	LOCAL INT8 nMaskBitPos;
	UINT16 *buf16;
	XImage *img = (XImage *)im;
	
	if ( img == NULL )
	return -1;

    data += 4; // desc,
    memcpy( &type, data, 4 ); data += 12;//	type, width, height
    if ( type != 8 && type !=2 && type !=0 )
    {
    	_jlog("invalid LBM type %d", type );
    	return -2;	// currently invalid type
    }

    memcpy( &size, data, 4 ); data += 4;
    if ( size < 0 )
    {
    	_jlog("invalid LBM buf size %d", size );
    	return -2;	// to big size
    }

    data += 4; /* mask */
	if(image_create_mode==IMAGE_TYPE_4G){
		BYTE *l_abBuf[8], *l_abMaskBuf[8];
		UINT16 l_uImgByteWidth,l_uTempHeight, l_uMaskByteWidth;

		if(type==2 || type==0){
		/* 4 gray lbm data -> 4 gray internal data */
		    bsize = mask ? (size<<1)+size : size<<1;
			pFirst = data ;
			pSecond = data +size;
			pMask = mask? data+(size<<1):NULL;
			
			if ( (bsize+(data-start)) != datasize )
			{
    			_jlog("invalid check sum %d %d", bsize+(data-start), datasize );
    			return -3;	
			}

			if(mask){
/////////////
				l_uImgByteWidth = BYTE_POS_4G(width+3);
				l_uMaskByteWidth = BYTE_POS_MASK(width+7);

				l_abBuf[7] = img->buf;
				l_abMaskBuf[7] = img->maskbuf;
				for(k=0; k< (height>>3); k++){
					l_abBuf[0] = l_abBuf[7];
					*l_abBuf[0] = 0;
					l_abMaskBuf[0] = l_abMaskBuf[7];
				
					for(j=1;j<8;j++){
						l_abBuf[j] = l_abBuf[j-1]+l_uImgByteWidth;
						*l_abBuf[j] = 0;
						l_abMaskBuf[j] = l_abMaskBuf[j-1]+l_uMaskByteWidth;
						*l_abMaskBuf[j] = 0;
					}

					for(i=0; i< width; i++){
						nBitPos = BIT_POS_4G(i);
						nMaskBitPos = BIT_POS_MASK(i);

						if(*pMask&0x01)	*l_abMaskBuf[0] |= bitMask[nMaskBitPos];
						else if(*pFirst&0x01){
								if(*pSecond&0x01) *l_abBuf[0]|= 0x03<<nBitPos;
								else *l_abBuf[0]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x01) *l_abBuf[0]|= 0x01<<nBitPos;

						if(*pMask&0x02)	*l_abMaskBuf[1] |= bitMask[nMaskBitPos];
						else if(*pFirst&0x02){
								if(*pSecond&0x02) *l_abBuf[1]|= 0x03<<nBitPos;
								else *l_abBuf[1]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x02) *l_abBuf[1]|= 0x01<<nBitPos;

						if(*pMask&0x04)	*l_abMaskBuf[2] |= bitMask[nMaskBitPos];
						else if(*pFirst&0x04){
								if(*pSecond&0x04) *l_abBuf[2]|= 0x03<<nBitPos;
								else *l_abBuf[2]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x04) *l_abBuf[2]|= 0x01<<nBitPos;

						if(*pMask&0x08)	*l_abMaskBuf[3] |= bitMask[nMaskBitPos];
						else if(*pFirst&0x08){
								if(*pSecond&0x08) *l_abBuf[3]|= 0x03<<nBitPos;
								else *l_abBuf[3]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x08) *l_abBuf[3]|= 0x01<<nBitPos;

						if(*pMask&0x10)	*l_abMaskBuf[4] |= bitMask[nMaskBitPos];
						else if(*pFirst&0x10){
								if(*pSecond&0x10) *l_abBuf[4]|= 0x03<<nBitPos;
								else *l_abBuf[4]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x10) *l_abBuf[4]|= 0x01<<nBitPos;

						if(*pMask&0x20)	*l_abMaskBuf[5]|= bitMask[nMaskBitPos];
						else if(*pFirst&0x20){
								if(*pSecond&0x20) *l_abBuf[5]|= 0x03<<nBitPos;
								else *l_abBuf[5]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x20) *l_abBuf[5]|= 0x01<<nBitPos;

						if(*pMask&0x40)	*l_abMaskBuf[6] |= bitMask[nMaskBitPos];
						else if(*pFirst&0x40){
								if(*pSecond&0x40) *l_abBuf[6]|= 0x03<<nBitPos;
								else *l_abBuf[6]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x40) *l_abBuf[6]|= 0x01<<nBitPos;

						if(*pMask&0x80)	*l_abMaskBuf[7] |= bitMask[nMaskBitPos];
						else if(*pFirst&0x80){
								if(*pSecond&0x80) *l_abBuf[7]|= 0x03<<nBitPos;
								else *l_abBuf[7]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x80) *l_abBuf[7]|= 0x01<<nBitPos;

						
						if(nBitPos==6){
								l_abBuf[0]++; l_abBuf[1]++; l_abBuf[2]++; l_abBuf[3]++;
								l_abBuf[4]++; l_abBuf[5]++; l_abBuf[6]++; l_abBuf[7]++; 
						}
            			if(nMaskBitPos==7){
								l_abMaskBuf[0]++; l_abMaskBuf[1]++; l_abMaskBuf[2]++; l_abMaskBuf[3]++;
								l_abMaskBuf[4]++; l_abMaskBuf[5]++; l_abMaskBuf[6]++; l_abMaskBuf[7]++; 
						}
						pFirst++;
						pSecond++;
						pMask++;
					}
					if(nBitPos!=6){
						l_abBuf[7]++;
					}
					if(nMaskBitPos!=7){
						l_abMaskBuf[7]++;
					}
				}

				l_uTempHeight = (BYTE_POS_MASK(height)<<3);
				if(l_uTempHeight != height){
					l_abBuf[0] = l_abBuf[7];
					*l_abBuf[0] = 0;
					l_abMaskBuf[0] = l_abMaskBuf[7];

					for(j=1;j<(height-l_uTempHeight);j++){
						l_abBuf[j] = l_abBuf[j-1]+l_uImgByteWidth;
						*l_abBuf[j] = 0;
						l_abMaskBuf[j] = l_abMaskBuf[j-1]+l_uMaskByteWidth;
						*l_abMaskBuf[j] = 0;
					}
					
					for(i=0; i< width; i++){
						nBitPos = BIT_POS_4G(i);
						nMaskBitPos = BIT_POS_MASK(i);
						for(j=0;j<(height-l_uTempHeight); j++){
							if(*pMask&bitMask[j]){ // 마스크가 set 되어 있을 때.. 
								*l_abMaskBuf[j] |= bitMask[nMaskBitPos];
								continue;
							}	
							if(*pFirst&bitMask[j]){
								if(*pSecond&bitMask[j]){
									*l_abBuf[j]|= 0x03<<nBitPos;
								}
								else {
									*l_abBuf[j]|= 0x02<<nBitPos;
								}
							}
							else {
								if(*pSecond&bitMask[j])
									*l_abBuf[j]|= 0x01<<nBitPos;
							}
						}

						if(nBitPos==6){
							for(j=0;j<(height-l_uTempHeight); j++)
								l_abBuf[j]++;
						}
						if(nMaskBitPos==7){
							for(j=0; j<(height-l_uTempHeight) ; j++)
								l_abMaskBuf[j]++;
						}
						pFirst++;
						pSecond++;
						pMask++;
					}
				}
//////////////
			}
			else{
///////////////
				l_uImgByteWidth = BYTE_POS_4G(width+3);
				l_abBuf[7] = img->buf;
				for(k=0; k< (height>>3); k++){
					l_abBuf[0] = l_abBuf[7];
					*l_abBuf[0] = 0;

					for(j=1;j<8;j++){
						l_abBuf[j] = l_abBuf[j-1]+l_uImgByteWidth;
						*l_abBuf[j] = 0;
					}
					for(i=0; i< width; i++){
						nBitPos = BIT_POS_4G(i);

						if(*pFirst&0x01){
								if(*pSecond&0x01) *l_abBuf[0]|= 0x03<<nBitPos;
								else *l_abBuf[0]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x01) *l_abBuf[0]|= 0x01<<nBitPos;

						if(*pFirst&0x02){
								if(*pSecond&0x02) *l_abBuf[1]|= 0x03<<nBitPos;
								else *l_abBuf[1]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x02) *l_abBuf[1]|= 0x01<<nBitPos;

						if(*pFirst&0x04){
								if(*pSecond&0x04) *l_abBuf[2]|= 0x03<<nBitPos;
								else *l_abBuf[2]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x04) *l_abBuf[2]|= 0x01<<nBitPos;

						if(*pFirst&0x08){
								if(*pSecond&0x08) *l_abBuf[3]|= 0x03<<nBitPos;
								else *l_abBuf[3]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x08) *l_abBuf[3]|= 0x01<<nBitPos;

						if(*pFirst&0x10){
								if(*pSecond&0x10) *l_abBuf[4]|= 0x03<<nBitPos;
								else *l_abBuf[4]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x10) *l_abBuf[4]|= 0x01<<nBitPos;

						if(*pFirst&0x20){
								if(*pSecond&0x20) *l_abBuf[5]|= 0x03<<nBitPos;
								else *l_abBuf[5]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x20) *l_abBuf[5]|= 0x01<<nBitPos;

						if(*pFirst&0x40){
								if(*pSecond&0x40) *l_abBuf[6]|= 0x03<<nBitPos;
								else *l_abBuf[6]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x40) *l_abBuf[6]|= 0x01<<nBitPos;

						if(*pFirst&0x80){
								if(*pSecond&0x80) *l_abBuf[7]|= 0x03<<nBitPos;
								else *l_abBuf[7]|= 0x02<<nBitPos;
						}
						else if(*pSecond&0x80) *l_abBuf[7]|= 0x01<<nBitPos;
						if(nBitPos==6){
							for(j=0; j<8 ; j++)
								l_abBuf[j]++;
						}
						pFirst++;
						pSecond++;
					}
					if(nBitPos!=6)
						l_abBuf[7]++;
				}

				l_uTempHeight = (BYTE_POS_MASK(height)<<3);
				if(l_uTempHeight != height){
					l_abBuf[0] = l_abBuf[7];
					*l_abBuf[0] = 0;
					for(j=1;j<(height-l_uTempHeight);j++){
						l_abBuf[j] = l_abBuf[j-1]+l_uImgByteWidth;
						*l_abBuf[j] = 0;
					}
					
					for(i=0; i< width; i++){
						nBitPos = BIT_POS_4G(i);
						for(j=0;j<(height-l_uTempHeight); j++){
							if(*pFirst&bitMask[j]){
								if(*pSecond&bitMask[j]){
									*l_abBuf[j]|= 0x03<<nBitPos;
								}
								else {
									*l_abBuf[j]|= 0x02<<nBitPos;
								}
							}
							else {
								if(*pSecond&bitMask[j]){
									*l_abBuf[j]|= 0x01<<nBitPos;
								}
								else{ 
									*l_abBuf[j]|= 0x00<<nBitPos;
								}
							}
						}
						if(nBitPos==6){
							for(j=0;j<(height-l_uTempHeight); j++)
								l_abBuf[j]++;
						}
						pFirst++;
						pSecond++;
					}
				}
			}
		}
		else if(type==8)
		{   /*  256C lbm data -> 4gray internal data */  
			BYTE lcdgray, *l_abSrc[8];
			UINT16 l_uImgByteWidth,l_uTempHeight, l_uMaskByteWidth;

			if(mask)img->transcolor = 0x1;
			bsize = mask ? size+((height+7)>>3)*width : size;
			pMask = mask?data+size:NULL;
			if ( (bsize+(data-start)) != datasize )
			{
	    		_jlog("invalid check sum %d %d", bsize+(data-start), datasize );
				return -3;	
			}
			l_uImgByteWidth = BYTE_POS_4G(width+3);
			l_uMaskByteWidth = BYTE_POS_MASK(width+7);

			for(k=0; k< (height>>3); k++){  // 세로 8bit를 다 쓰는 것들.. 
				l_abBuf[0] = img->buf+TOT_BYTE_POS_4G(0,(k<<3),width);
				*l_abBuf[0] = 0;
				l_abSrc[0] = data + (k<<3)*width  ;

				if(mask)
					l_abMaskBuf[0] = img->maskbuf+((k<<3)*((width+7)>>3));
			
				for(j=1;j<8;j++){
					l_abBuf[j]= l_abBuf[j-1]+l_uImgByteWidth;
					l_abSrc[j]= l_abSrc[j-1] + width;
					*l_abBuf[j] = 0;
					if(mask)
						l_abMaskBuf[j] = l_abMaskBuf[j-1]+l_uMaskByteWidth;
				}

				for(i=0; i< width; i++){
					nBitPos = BIT_POS_4G(i);
					nMaskBitPos = BIT_POS_MASK(i);
					for(j=0; j<8 ; j++){
						if(mask){
							if(*pMask&bitMask[j]){
								*l_abMaskBuf[j] |= bitMask[nMaskBitPos];
								l_abSrc[j]++;
								continue;
							}
							else {
								*l_abMaskBuf[j] &= nbitMask[nMaskBitPos];
							}
						}	
						CLR8TOLCDGRAY2(*l_abSrc[j], lcdgray);
						*l_abBuf[j] |= lcdgray<<nBitPos;
						l_abSrc[j]++;
					}
					if(mask){
						pMask++;
						if(nMaskBitPos==7){
						for(j=0; j<8 ; j++)
							l_abMaskBuf[j]++;
						}
					}
					if(nBitPos==6){
						for(j=0; j<8 ; j++)
							l_abBuf[j]++;
					}
				}
			}
			l_uTempHeight = (BYTE_POS_MASK(height)<<3);
			if(l_uTempHeight != height){ // 마지막 세로 8bit를 다 쓰지 않는 것
				l_abBuf[0] = img->buf+TOT_BYTE_POS_4G(0,(k<<3),width);
				*l_abBuf[0] = 0;
				l_abSrc[0] = data + (k<<3)*width  ;

				if(mask)
					l_abMaskBuf[0] = img->maskbuf+((k<<3)*((width+7)>>3));
			
				for(j=1;j<(height-l_uTempHeight);j++){
					l_abBuf[j]= l_abBuf[j-1]+l_uImgByteWidth;
					l_abSrc[j]= l_abSrc[j-1] + width;
					*l_abBuf[j] = 0;
					if(mask)
						l_abMaskBuf[j] = l_abMaskBuf[j-1]+l_uMaskByteWidth;
				}
				for(i=0; i< width; i++){
					nBitPos = BIT_POS_4G(i);
					nMaskBitPos = BIT_POS_MASK(i);
					for(j=0; j<(height-l_uTempHeight); j++){
						if(mask){
							if(*pMask&bitMask[j]){
								*l_abMaskBuf[j] |= bitMask[nMaskBitPos];
								l_abSrc[j]++;
								continue;
							}
							else 
								*l_abMaskBuf[j] &= nbitMask[nMaskBitPos];
						}	
						CLR8TOLCDGRAY2(*l_abSrc[j], lcdgray);
						*l_abBuf[j] |= lcdgray<<nBitPos;
						l_abSrc[j]++;
					}
					if(mask){
						pMask++;
						if(nMaskBitPos==7){
						for(j=0; j<8 ; j++)
							l_abMaskBuf[j]++;
						}
					}
					if(nBitPos==6){
						for(j=0; j<8 ; j++)
							l_abBuf[j]++;
					}
				}
			}
		}
		else if(type==16)
		{   /* 64KC lbm data -> 4gray internal data */  
			// 구현 해야한다. 
		}
	}
	else if(image_create_mode== IMAGE_TYPE_256C){
		if(type==2 || type == 0){ // 4gray -> 256 C 
			BYTE *l_abBuf[8];
			UINT16 l_uTempHeight;

			if(mask)img->transcolor = 0x1;
		    bsize = mask ? (size<<1)+size : size<<1;
			pFirst = data ;
			pSecond = data +size;
			pMask = mask? data+(size<<1):NULL;
			if ( (bsize+(data-start)) != datasize )
			{
    			_jlog("invalid check sum %d %d", bsize+(data-start), datasize );
    			return -3;	
			}
			if(mask){
				for(k=0; k< (height>>3); k++){
					l_abBuf[0] = img->buf+INDEX_ON_IMG(0,(k<<3),width);
					for(j=1;j<8;j++){
						l_abBuf[j] = l_abBuf[j-1]+width;
					}
					for(i=0; i< width; i++){
						for(j=0; j<8 ; j++){
							if(*pMask&bitMask[j]){
								*l_abBuf[j] =(BYTE)img->transcolor; 
								l_abBuf[j]++;
								continue;
							}	
							else{
								if(*pFirst&bitMask[j]){
									if(*pSecond&bitMask[j])
									*l_abBuf[j]= 0X00;
								else 
									*l_abBuf[j]= 0x49;
								}
								else {
									if(*pSecond&bitMask[j])
										*l_abBuf[j]= 0x92;
									else 
										*l_abBuf[j]= 0xFF;
								}
							}
							l_abBuf[j]++;
						}
						pFirst++;
						pSecond++;
						pMask++;
					}
				}
			}
			else {
				for(k=0; k< (height>>3); k++){
					l_abBuf[0] = img->buf+INDEX_ON_IMG(0,(k<<3),width);
					for(j=1;j<8;j++){
						l_abBuf[j] = l_abBuf[j-1]+width;
					}
					for(i=0; i< width; i++){
						for(j=0; j<8 ; j++){
							if(*pFirst&bitMask[j]){
								if(*pSecond&bitMask[j])
									*l_abBuf[j]= 0X00;
								else 
									*l_abBuf[j]= 0x49;
							}
							else {
								if(*pSecond&bitMask[j])
									*l_abBuf[j]= 0x92;
								else 
									*l_abBuf[j]= 0xFF;
							}
							l_abBuf[j]++;
						}
						pFirst++;
						pSecond++;
					}
				}
			}
			l_uTempHeight = (BYTE_POS_MASK(height)<<3);
			if(l_uTempHeight != height){
				l_abBuf[0] = img->buf+INDEX_ON_IMG(0,(k<<3),width);
				for(j=1;j<(height-l_uTempHeight);j++){
					l_abBuf[j] = l_abBuf[j-1]+width;
				}
				if(mask){
					for(i=0; i< width; i++){
						for(j=0;j<(height-l_uTempHeight); j++){
							if(*pMask&bitMask[j]){ // 마스크가 set 되어 있을 때.. 
								*l_abBuf[j] =(BYTE)img->transcolor; 
								l_abBuf[j]++;
								continue;
							}	
							if(*pFirst&bitMask[j]){
								if(*pSecond&bitMask[j])
									*l_abBuf[j]= 0X00;
								else 
									*l_abBuf[j]= 0x49;
							}
							else {
								if(*pSecond&bitMask[j])
									*l_abBuf[j]= 0x92;
								else 
									*l_abBuf[j]= 0xFF;
							}
							l_abBuf[j]++;
						}
						pFirst++;
						pSecond++;
						pMask++;
					}
				}
				else{
					for(i=0; i< width; i++){
						for(j=0;j<(height-l_uTempHeight); j++){
							if(*pFirst&bitMask[j]){
								if(*pSecond&bitMask[j])
									*l_abBuf[j]= 0X00;
								else 
									*l_abBuf[j]= 0x49;
							}
							else {
								if(*pSecond&bitMask[j])
									*l_abBuf[j]= 0x92;
								else 
									*l_abBuf[j]= 0xFF;
							}
							l_abBuf[j]++;
						}
						pFirst++;
						pSecond++;
					}
				}
			}
		}
		else if (type==8){
			BYTE *l_abBuf[8]/*, *l_abMaskBuf[8]*/, *pBuf8;
			UINT16 l_uTempHeight;

			img->transcolor = 0x1;
			bsize = mask ? size+((height+7)>>3)*width : size;
			pFirst = data ;
			pTemp = pMask = mask?data+size:NULL;
			if ( (bsize+(data-start)) != datasize )
			{
	    		_jlog("invalid check sum %d %d", bsize+(data-start), datasize );
				return -3;	
			}
			// 이미지는 그냥 모조리 카피 하고 
			pBuf8 = img->buf;
			memcpy(pBuf8,pFirst, size);
			// mask가 있는 부분만 찾아서 transcolor로 바꿔준다. 
			if(mask){
				for(k=0; k< (height>>3); k++){
					l_abBuf[0] = img->buf+INDEX_ON_IMG(0,(k<<3),width);
					for(j=1;j<8;j++){
						l_abBuf[j] = l_abBuf[j-1]+width;
					}
					for(i=0; i< width; i++){
						if(*pMask&0x01) *l_abBuf[0] =(BYTE)img->transcolor; 
						l_abBuf[0]++;

						if(*pMask&0x02)	*l_abBuf[1] =(BYTE)img->transcolor; 
						l_abBuf[1]++;

						if(*pMask&0x04) *l_abBuf[2] =(BYTE)img->transcolor; 
						l_abBuf[2]++;

						if(*pMask&0x08) *l_abBuf[3] =(BYTE)img->transcolor; 
						l_abBuf[3]++;

						if(*pMask&0x10)	*l_abBuf[4] =(BYTE)img->transcolor; 
						l_abBuf[4]++;

						if(*pMask&0x20)	*l_abBuf[5] =(BYTE)img->transcolor; 
						l_abBuf[5]++;

						if(*pMask&0x40)	*l_abBuf[6] =(BYTE)img->transcolor; 
						l_abBuf[6]++;

						if(*pMask&0x80)	*l_abBuf[7] =(BYTE)img->transcolor; 

						l_abBuf[7]++;
						pMask++;
					}
				}

				l_uTempHeight = (BYTE_POS_MASK(height)<<3);
				if(l_uTempHeight != height){
					l_abBuf[0] = img->buf+INDEX_ON_IMG(0,(k<<3),width);
					for(j=1;j<(height-l_uTempHeight);j++){
						l_abBuf[j] = l_abBuf[j-1]+width;
					}
					for(i=0; i< width; i++){
						for(j=0;j<(height-l_uTempHeight); j++){
							if(*pMask&bitMask[j]){ // 마스크가 set 되어 있을 때.. 
								*l_abBuf[j] =(BYTE)img->transcolor; 
								l_abBuf[j]++;
								continue;
							}	
							l_abBuf[j]++;
						}
						pMask++;
					}
				}
			}
		}
		else if(type==16){
			// 구현 해야한다. 
		}
	}
	else if(image_create_mode== IMAGE_TYPE_64KC){
		if(type==2 || type == 0){
			UINT16 *l_awBuf[8];
			UINT16 l_uTempHeight;

			img->transcolor = 0x8;
		    bsize = mask ? (size<<1)+size : size<<1;
			pFirst = data ;
			pSecond = data +size;
			pMask = mask? data+(size<<1):NULL;
			buf16 = (UINT16 *)img->buf;
			if ( (bsize+(data-start)) != datasize )
			{
    			_jlog("invalid check sum %d %d", bsize+(data-start), datasize );
    			return -3;	
			}
			if(mask){
				for(k=0; k< (height>>3); k++){
					l_awBuf[0] = (UINT16 *)img->buf+INDEX_ON_IMG(0,(k<<3),width);
					for(j=1;j<8;j++){
						l_awBuf[j] = l_awBuf[j-1]+width;
					}
					for(i=0; i< width; i++){
						if(*pMask&0x01) *l_awBuf[0] =img->transcolor; 
						else if(*pFirst&0x01){
							if(*pSecond&0x01)	*l_awBuf[0]= 0X0000;
							else *l_awBuf[0]= 0x4208;
						}
						else{
							if(*pSecond&0x01) *l_awBuf[0]= 0x8410;
							else *l_awBuf[0]= 0xFFFF;
						}
						l_awBuf[0]++;

						if(*pMask&0x02) *l_awBuf[1] =img->transcolor; 
						else if(*pFirst&0x02){
							if(*pSecond&0x02)	*l_awBuf[1]= 0X0000;
							else *l_awBuf[1]= 0x4208;
						}
						else{
							if(*pSecond&0x02) *l_awBuf[1]= 0x8410;
							else *l_awBuf[1]= 0xFFFF;
						}
						l_awBuf[1]++;

						if(*pMask&0x04) *l_awBuf[2] =img->transcolor; 
						else if(*pFirst&0x04){
							if(*pSecond&0x04)	*l_awBuf[2]= 0X0000;
							else *l_awBuf[2]= 0x4208;
						}
						else{
							if(*pSecond&0x04) *l_awBuf[2]= 0x8410;
							else *l_awBuf[2]= 0xFFFF;
						}
						l_awBuf[2]++;

						if(*pMask&0x08) *l_awBuf[3] =img->transcolor; 
						else if(*pFirst&0x08){
							if(*pSecond&0x08)	*l_awBuf[3]= 0X0000;
							else *l_awBuf[3]= 0x4208;
						}
						else{
							if(*pSecond&0x08) *l_awBuf[3]= 0x8410;
							else *l_awBuf[3]= 0xFFFF;
						}
						l_awBuf[3]++;

						if(*pMask&0x10) *l_awBuf[4] =img->transcolor; 
						else if(*pFirst&0x10){
							if(*pSecond&0x10)	*l_awBuf[4]= 0X0000;
							else *l_awBuf[4]= 0x4208;
						}
						else{
							if(*pSecond&0x10) *l_awBuf[4]= 0x8410;
							else *l_awBuf[4]= 0xFFFF;
						}
						l_awBuf[4]++;

						if(*pMask&0x20) *l_awBuf[5] =img->transcolor; 
						else if(*pFirst&0x20){
							if(*pSecond&0x20)	*l_awBuf[5]= 0X0000;
							else *l_awBuf[5]= 0x4208;
						}
						else{
							if(*pSecond&0x20) *l_awBuf[5]= 0x8410;
							else *l_awBuf[5]= 0xFFFF;
						}
						l_awBuf[5]++;

						if(*pMask&0x40) *l_awBuf[6] =img->transcolor; 
						else if(*pFirst&0x40){
							if(*pSecond&0x40)	*l_awBuf[6]= 0X0000;
							else *l_awBuf[6]= 0x4208;
						}
						else{
							if(*pSecond&0x40) *l_awBuf[6]= 0x8410;
							else *l_awBuf[6]= 0xFFFF;
						}
						l_awBuf[6]++;

						if(*pMask&0x80) *l_awBuf[7] =img->transcolor; 
						else if(*pFirst&0x80){
							if(*pSecond&0x80)	*l_awBuf[7]= 0X0000;
							else *l_awBuf[7]= 0x4208;
						}
						else{
							if(*pSecond&0x80) *l_awBuf[7]= 0x8410;
							else *l_awBuf[7]= 0xFFFF;
						}
						l_awBuf[7]++;

						pFirst++;
						pSecond++;
						pMask++;
					}
				}

				l_uTempHeight = BYTE_POS_MASK(height)<<3;
				if(l_uTempHeight != height){
					l_awBuf[0] = (UINT16 *)img->buf+INDEX_ON_IMG(0,(k<<3),width);
					for(j=1;j<(height-l_uTempHeight);j++){
						l_awBuf[j] = l_awBuf[j-1]+width;
					}
					for(i=0; i< width; i++){
						for(j=0;j<(height-l_uTempHeight); j++){
							if(*pMask&bitMask[j]){ // 마스크가 set 되어 있을 때.. 
								*l_awBuf[j] =img->transcolor; 
								l_awBuf[j]++;
								continue;
							}	
							if(*pFirst&bitMask[j]){
								if(*pSecond&bitMask[j])
									*l_awBuf[j]= 0X0000;
								else 
									*l_awBuf[j]= 0x4208;
							}
							else {
								if(*pSecond&bitMask[j])
									*l_awBuf[j]= 0x8410;
								else 
									*l_awBuf[j]= 0xFFFF;
							}
							l_awBuf[j]++;
						}
						pFirst++;
						pSecond++;
						pMask++;
					}
				}
			}	
			else {
				for(k=0; k< (height>>3); k++){
					l_awBuf[0] = (UINT16 *)img->buf+INDEX_ON_IMG(0,(k<<3),width);
					
					for(j=1;j<8;j++)
						l_awBuf[j] = l_awBuf[j-1]+width;

					for(i=0; i< width; i++){
						if(*pFirst&0x01){
							if(*pSecond&0x01)	*l_awBuf[0]= 0X0000;
							else *l_awBuf[0]= 0x4208;
						}
						else{
							if(*pSecond&0x01) *l_awBuf[0]= 0x8410;
							else *l_awBuf[0]= 0xFFFF;
						}
						l_awBuf[0]++;

						if(*pFirst&0x02){
							if(*pSecond&0x02)	*l_awBuf[1]= 0X0000;
							else *l_awBuf[1]= 0x4208;
						}
						else{
							if(*pSecond&0x02) *l_awBuf[1]= 0x8410;
							else *l_awBuf[1]= 0xFFFF;
						}
						l_awBuf[1]++;

						if(*pFirst&0x04){
							if(*pSecond&0x04)	*l_awBuf[2]= 0X0000;
							else *l_awBuf[2]= 0x4208;
						}
						else{
							if(*pSecond&0x04) *l_awBuf[2]= 0x8410;
							else *l_awBuf[2]= 0xFFFF;
						}
						l_awBuf[2]++;

						if(*pFirst&0x08){
							if(*pSecond&0x08)	*l_awBuf[3]= 0X0000;
							else *l_awBuf[3]= 0x4208;
						}
						else{
							if(*pSecond&0x08) *l_awBuf[3]= 0x8410;
							else *l_awBuf[3]= 0xFFFF;
						}
						l_awBuf[3]++;

						if(*pFirst&0x10){
							if(*pSecond&0x10)	*l_awBuf[4]= 0X0000;
							else *l_awBuf[4]= 0x4208;
						}
						else{
							if(*pSecond&0x10) *l_awBuf[4]= 0x8410;
							else *l_awBuf[4]= 0xFFFF;
						}
						l_awBuf[4]++;

						if(*pFirst&0x20){
							if(*pSecond&0x20)	*l_awBuf[5]= 0X0000;
							else *l_awBuf[5]= 0x4208;
						}
						else{
							if(*pSecond&0x20) *l_awBuf[5]= 0x8410;
							else *l_awBuf[5]= 0xFFFF;
						}
						l_awBuf[5]++;

						if(*pFirst&0x40){
							if(*pSecond&0x40)	*l_awBuf[6]= 0X0000;
							else *l_awBuf[6]= 0x4208;
						}
						else{
							if(*pSecond&0x40) *l_awBuf[6]= 0x8410;
							else *l_awBuf[6]= 0xFFFF;
						}
						l_awBuf[6]++;

						if(*pFirst&0x80){
							if(*pSecond&0x80)	*l_awBuf[7]= 0X0000;
							else *l_awBuf[7]= 0x4208;
						}
						else{
							if(*pSecond&0x80) *l_awBuf[7]= 0x8410;
							else *l_awBuf[7]= 0xFFFF;
						}
						l_awBuf[7]++;

					}
				}

				l_uTempHeight = (BYTE_POS_MASK(height)<<3);
				if(l_uTempHeight != height){
					l_awBuf[0] = (UINT16 *)img->buf+INDEX_ON_IMG(0,(k<<3),width);
					for(j=1;j<(height-l_uTempHeight);j++)
						l_awBuf[j] = l_awBuf[j-1]+width;
					
					for(i=0; i< width; i++){
						for(j=0;j<(height-l_uTempHeight); j++){
							if(*pFirst&bitMask[j]){
								if(*pSecond&bitMask[j])
									*l_awBuf[j]= 0X0000;
								else 
									*l_awBuf[j]= 0x4208;
							}
							else {
								if(*pSecond&bitMask[j])
									*l_awBuf[j]= 0x8410;
								else 
									*l_awBuf[j]= 0xFFFF;
							}
							l_awBuf[j]++;
						}
						pFirst++;
						pSecond++;
					}
				}
			}
		}
		else if (type==8){
			UINT16 *l_awBuf[8];
			BYTE  *pFirst[8];
			UINT16 l_uTempHeight;

			if(mask)img->transcolor = 0x8;
			bsize = mask ? size+((height+7)>>3)*width : size;
			pTemp = pMask = mask?data+size:NULL;
			if ( (bsize+(data-start)) != datasize )
			{
	    		_jlog("invalid check sum %d %d", bsize+(data-start), datasize );
				return -3;	
			}
			// mask가 있는 부분만 찾아서 transcolor로 바꿔준다. 
			for(k=0; k< (height>>3); k++){
				pFirst[0] = data + (k<<3)*width;
				l_awBuf[0] = (UINT16 *)img->buf+INDEX_ON_IMG(0,(k<<3),width);
				for(j=1;j<8;j++){
					pFirst[j] = pFirst[j-1] + width;
					l_awBuf[j] = l_awBuf[j-1]+width;
				}
				for(i=0; i< width; i++){
					for(j=0; j<8 ; j++){
						if(mask){
							if(*pMask&bitMask[j]){
								*l_awBuf[j] =img->transcolor; 
								l_awBuf[j]++;
								pFirst[j]++;
								continue;
							}
						}
						*l_awBuf[j] = cvt8b16b[*pFirst[j]];
						pFirst[j]++;
						l_awBuf[j]++;
					}
					if(mask)
						pMask++;
				}
			}

			l_uTempHeight = (BYTE_POS_MASK(height)<<3);
			if(l_uTempHeight != height){
				pFirst[0] = pFirst[7];
				l_awBuf[0] = (UINT16 *)img->buf+INDEX_ON_IMG(0,(k<<3),width);
				for(j=1;j<(height-l_uTempHeight);j++){
					pFirst[j] = pFirst[j-1]+width;
					l_awBuf[j] = l_awBuf[j-1]+width;
				}
				for(i=0; i< width; i++){
					for(j=0;j<(height-l_uTempHeight); j++){
						if(mask){
							if(*pMask&bitMask[j]){ // 마스크가 set 되어 있을 때.. 
								*l_awBuf[j] =img->transcolor; 
								l_awBuf[j]++;
								pFirst[j]++;
								continue;
							}
						}	
						*l_awBuf[j] = cvt8b16b[*pFirst[j]];
						l_awBuf[j]++;
						pFirst[j]++;
					}
					if(mask)
						pMask++;
				}
			}
		}
		else if(type==16)
		{   /* 64KC lbm data -> 4gray internal data */  
			// 구현 해야한다. 
		}
	}
	return 0;
}

int imageLBMDataClose(BYTE *data, int datasize, void *sbuf, int bufsize,
    int w, int h, int mask)
{
	// test
    return 0;
}
#endif

/*************************************************************
* function: imageDataOpen
* desc: 	get image file information
*		calling sequence
*
*		if (imageDataOpen() < 0)
*			imageDataClose() and return
*		... if some error call imageDataClose() return ...
*		if (imageDataRead() < 0)
*			imageDataClose() and return
*		imageDataClose()
*
* input:	data - full file data
*		datasize - file length
*		sbuf - usable buffer
*		bufsize - sizeof(sbuf)
* output:
*		w - width
*		h - height
*		mask - transparency
* return:	imageType >= 0 or negative when failed 
**************************************************************/
int imageDataOpen(BYTE *data, int datasize, void *sbuf, int bufsize,
	int *w, int *h, int *mask)
{

    int ret = -1;
	INT16 width,height,m;
	//for test
	switch (imageFileCheck(data)){
	case ITYPE_PNG:
		if (imagePNGDataOpen(data, datasize, sbuf, bufsize, w, h, mask) >= 0)
			ret = ITYPE_PNG;
		break;
#ifdef LBM_SUPPORTED
	case ITYPE_LBM:
		//for test
		if (imageLBMDataOpen(data, datasize, sbuf, bufsize, w, h, mask) >= 0)
			ret = ITYPE_LBM;
		break;
#endif 

#ifdef EBI_SUPPORTED
	case ITYPE_EBI:
		if(memcmp( data, "IMG", 3 ) != 0)
			break;
		data += 4;
		memcpy(  &width, data, 2 ); data+=2;
		memcpy(  &height, data, 2 ); data +=10;
		memcpy(  &m, data,2);
		*w =(int) width;
		*h = (int) height;
		*mask = (int) m;
		ret = ITYPE_EBI;
		prev_create_mode = image_create_mode;
		if(!SKY_GET_SKVMCOLORENV()){  
			image_create_mode  = 1;
		}
		else{
			image_create_mode = 2;
		}
		break;
#endif
	}
	return ret;
}

/*************************************************************
* function: imageDataRead
* desc: 	read image
* input:	data, datasize, sbuf, bufsize - same with imageDataOpen()
*		im - image structure(you fill it with data)
*		imageType - return value from imageDataOpen()
*		w, h, mask - set value from imageDataOpen()
* return:	0 or negative when failed 
**************************************************************/
int imageDataRead(BYTE *data, int datasize, void *sbuf, int bufsize,
	Image *im, int imageType, int w, int h, int mask)
{	
    XImage *img = (XImage *)im;
	UINT16 *buf16 = (UINT16 *)img->buf;
	register UINT32 i, size;

    switch(imageType) {
   	case ITYPE_PNG:
        return imagePNGDataRead(data, datasize, sbuf, bufsize, im, w, h, mask);
#ifdef LBM_SUPPORTED
   	case ITYPE_LBM:
        return imageLBMDataRead(data, datasize, sbuf, bufsize, im, w, h, mask);
#endif

#ifdef EBI_SUPPORTED
	case ITYPE_EBI:
		if ( img == NULL )
			return -1;

		data += 12; /* desc  width, height, real_width,  bits per pixel, palette,  */
                //   3+1(공백)  2,      2,       2,         1,                1
		if(image_create_mode ==IMAGE_TYPE_4G){
			BYTE lcdgray, *buf8 , *maskbuf8, nBitPos, nMaskBitPos;
			int j, temp_width,temp_maskwidth; 
			memcpy(&img->transcolor,data,1);data+=40;
			temp_width  = ((img->width)+3)>>2;
			if(mask){
				temp_maskwidth = ((img->width)+7)>>3;
				for(i=0;i<h;i++){
					buf8 = img->buf + temp_width*i;
					maskbuf8 = img->maskbuf + temp_maskwidth*i;
					for(j=0; j<w;j++){
						nMaskBitPos = BIT_POS_BW(j);
						nBitPos = BIT_POS_4G (j);
						if(*data == img->transcolor){
							*maskbuf8 |= bitMask[nMaskBitPos];
							*buf8 &= m_abNotMask4G[nBitPos];
						}
						else {
							CLR8TOLCDGRAY2(*data, lcdgray);
							*buf8 &= m_abNotMask4G[nBitPos];
							*buf8 |= lcdgray<<nBitPos;
							*maskbuf8 &= nbitMask[nMaskBitPos];
						}
						if(nMaskBitPos==7)
							maskbuf8++;
						if(nBitPos ==6)
							buf8++;
						data++;
					}
				}
			}
			else{
				for(i=0;i<h;i++){
					buf8 = img->buf + temp_width*i;
					for(j=0; j<w;j++){
						nBitPos = BIT_POS_4G (j);
						CLR8TOLCDGRAY2(*data, lcdgray);
						*buf8 &= m_abNotMask4G[nBitPos];
						*buf8 |= lcdgray<<nBitPos;
						if(nBitPos ==6)
							buf8++;
						data++;
					}
				}
			}
		}
		else if(image_create_mode ==IMAGE_TYPE_256C){  
			memcpy(&img->transcolor,data,1);data+=40;
			memcpy(img->buf,data,w*h);
		}
		else{
			memcpy(&img->transcolor,data,1);data+=40;
			img->transcolor = cvt8b16b[img->transcolor];
			size = w*h;
			for(i=0; i<size;i++)
				*buf16++ = cvt8b16b[*data++];
		}
		return 0;
#endif 
	default:
		return -1; 
	}
}

/*************************************************************
* function: imageDataClose
* desc: 	finalize
* input:	data, datasize, sbuf, bufsize, imageType, w, h, mask
* return:	0 or negative when failed 
**************************************************************/
int imageDataClose(BYTE *data, int datasize, void *sbuf, int bufsize, 
				   int imageType, int w, int h, int mask)
{

	switch(imageType) {
	case ITYPE_PNG:
		return imagePNGDataClose(data, datasize, sbuf, bufsize, w, h, mask);//

#ifdef LBM_SUPPORTED
	case ITYPE_LBM:
		image_create_mode = prev_create_mode;
		return imageLBMDataClose(data, datasize, sbuf, bufsize, w, h, mask);//
#endif

#ifdef EBI_SUPPORTED
	case ITYPE_EBI:
		image_create_mode = prev_create_mode;
		return 0;
#endif
	default:
		return -1;
    }
}