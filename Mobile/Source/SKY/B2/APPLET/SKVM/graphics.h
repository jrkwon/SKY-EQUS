#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "sky.h"
#include "ska.h"
#include "eqs.h"

#ifndef BYTE 
#define BYTE unsigned char
#endif

#ifndef WORD 
#define WORD unsigned short
#endif
 
/**********************************
* IMAGE OPERATINOS
**********************************/
#define IMG_COPY	0
#define IMG_AND		1
#define IMG_OR		2
#define IMG_XOR		3

/**********************************
* FONT SIZE 
**********************************/
#define FONT_XLARGE		21	
#define FONT_LARGE		16	
#define FONT_MEDIUM		13	
#define FONT_SMALL		8

/**********************************
* IMAGE COLOR DEF 
**********************************/
/* 
   PIXEL is for color value : 256 color is 8bit..
*/

typedef unsigned long PIXEL;

/* 
   IMAGE_DEPTH
   by tortol for customizing color depth
   1: B/W 
   2: 4 gray 
   4: 16 gray or color
   8: 256 gray or color 
   24 : 24bit true color
*/
#define	BW				1
#define GRAY_4			2
#define GRAY_16			4
#define GRAY_256		8
#define COLOR_256		8
#define COLOR_TURE		24
#define IMAGE_TYPE_4G   0
#define IMAGE_TYPE_256C 1
#define IMAGE_TYPE_64KC 2
#define IMAGE_DEPTH		COLOR_256	
#define WHITE	0
#define LGRAY	1
#define DGRAY	2
#define BLACK	3

#ifndef TEST_TORTOL
#define TEST_TORTOL
#endif 
/*************************************************************
* IMAGE STRUCTURE 
* CAUTION: allocate this structure only by imageCreate
* 1. 'data[0]' and 'data[1]' are for the two bitmap planes
*     specifying 4 gray level
* 2. 'mask' is used for transparency masking where 1 is for 
*    opaque and 0 for transparent
* 3. 'data[0]', 'data[1]' and 'mask' point at the position in
*    'buf' array. 'size' specifies the number of bytes in one
*    pixel plane. In default implementation, data[0] = buf,
*    data[1] = (buf + size) and mask = (buf + size*2)
* 4. 'internal' is used in java system, DO NOT CHANGE THIS VALUE
* DEPENDING ON THE LCD DEVICE, YOU MUST ORGANIZE THE 'buf' 
* STRUCTURE APPROPRIATELY.

  --> for our 8bit color system 
  we have to have 8 data bufs array each layer is for each bit 
                                                      --------
*************************************************************/

typedef struct _Image
{
	void *internal;			// internal use only, don't edit this
	int width;				// image width
	int height;				// image height
	int clipx1;				// clipping left position
	int clipx2;				// clipping right position
	int clipy1;				// clipping top position
	int clipy2;				// clipping bottom position
} Image; 

typedef	struct _XImage {
	///////////////////////////////////////////////////
	void *internal;			// internal use only, don't edit this
	int width;				// image width
	int height;				// image height
	int clipx1;				// clipping left position
	int clipx2;				// clipping right position
	int clipy1;				// clipping top position
	int clipy2;				// clipping bottom position
	////////////////////////////////////////////////////

	unsigned int  size;	    // image total size 
	BYTE	*buf;			// total buffer holder
	BYTE	mask;			// boolean transparency value
	BYTE	*maskbuf;	    // for using 4gray 
	UINT16	transcolor;		// transparence color

    BYTE	type;	// 1 for 256 color image and 0 for 4 gray-scale image
	
} XImage;
 

typedef struct ColorType 
{
    BYTE Red, Green, Blue;
} ColorType;

extern int imageSize(int w, int h, int mask);

extern void imageSetup(Image *im, int mask);

extern Image *imageCreateMain( int width, int height ); 
extern Image *simageCreate( int width, int height,int mask );

extern void imageDestroyMain(Image *im);

extern void refreshLCD( Image *img, int x, int y, int w, int h );

extern void imageClear( Image* img, PIXEL col, BYTE mode );

extern void imageInvert( Image *img, int x, int y, int w, int h );

extern PIXEL imageGetPixel( Image *tgt, int x, int y );

extern PIXEL imageGetPixelEx( Image *img, int x, int y, BYTE *mval );

extern void	imageSetPixel( Image *im, int x, int y, PIXEL col32 ,BYTE mode);

extern void imageSetPixelEx( Image *img, int x, int y, PIXEL col, BYTE bmask, BYTE mode);

extern void imageDrawLine( Image *tgt, int x1, int y1, int x2, int y2,
			PIXEL col );

extern void imageDrawArc( Image *img, int x, int y, int w, int h, 
			int sAngle, int arcAngle, BYTE filled, PIXEL col );

extern void 	imageDrawRect( Image *im, int x, int y, int width, 
			int height, BYTE filled, PIXEL col );
extern void imageDrawImage( Image *tgt, int tx, int ty, Image *src, 
			int sx, int sy, int w, int h, BYTE mode );
extern void imageDrawCircle( Image *im, int cx, int cy, BYTE filled, int radius, PIXEL col );
extern void imageDrawText( Image *img, int x, int y, char *text, 
			int offset, int len, BYTE fsize, PIXEL col  );

extern void  DrawTextToImg(Image *im, int x, int y, char *text, 
                                           int offset, int len,PIXEL col8);
extern int getTextWidth( char *text, int offset, int length, int fsize );

extern int getFontHeight( int fsize );

extern char *imageFileDefaultExt(void);

extern int imageDataOpen(BYTE *data, int datasize, void *sbuf, int bufsize,
	int *w, int *h, int *mask);

extern int imageDataRead(BYTE *data, int datasize, void *sbuf, int bufsize,
	Image *im, int imageType, int w, int h, int mask);

extern int imageDataClose(BYTE *data, int datasize, void *sbuf, int bufsize,
	int imageType, int w, int h, int mask);

// PNGLIB -> graphics
extern int imagePNGDataOpen(BYTE *data, int datasize, void *sbuf, int bufsize,
	        int *w, int *h, int *maskflag);
extern int imagePNGDataRead(BYTE *data, int datasize, void *sbuf, int bufsize,
	        Image *im, int w, int h, int maskflag);
//extern int imagePNGDataRead(BYTE *data, int datasize, void *sbuf, int bufsize,
//	        int w, int h, int maskflag);

extern int imagePNGDataClose(BYTE *data, int datasize, void *sbuf, int bufsize,
	        int w, int h, int maskflag);

// graphics -> PNGLIB
extern void png2Image(Image *img, unsigned short *png_data,
	int w, int h, int y, unsigned short trans_color, int maskflag);

extern void refreshLCD( Image *img, int x, int y, int w, int h );

// Call in the EQS function (EQS_DrawTextToImg)

extern void	DrawAnnunciator(BOOL f);
extern void SKY_DrawAnnunciators(void);
extern void SKY_Draw256Annunciators(void);
#endif
