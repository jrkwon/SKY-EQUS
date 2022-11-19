#if 1

// MAX_TOTSHARE_SIZE should be larger than MAX_SIS_SIZE
#define MAX_NTOP_SIZE       (75*1000)
#define MAX_RXBUF_SIZE      (11*1000) 
#define	MAX_SISRAM_START    (MAX_NTOP_SIZE + MAX_RXBUF_SIZE)
#define MAX_OBJBUF_SIZE     (64*1024)  // 65536

#ifdef FEATURE_MLZ
#define	MAX_CODEBUF_SIZE    (25*1024)  // 25600
#else
#define	MAX_CODEBUF_SIZE    (34*1000)
#endif // FEATURE_MLZ

#define MAX_IMAGEBUF_SIZE   (10*1024) 
#define MAX_UNCOMPBUF_SIZE  (15*1024)  //128*128=16,384
 
#define	MAX_SIS_SIZE        ( MAX_SISRAM_START + MAX_OBJBUF_SIZE \
	                     + MAX_CODEBUF_SIZE + MAX_IMAGEBUF_SIZE  \
						 + MAX_UNCOMPBUF_SIZE + MAX_RXBUF_SIZE )
 
//#define	MAX_TOTSHARE_SIZE   (0x0127FFF0 - 0x124F000)
#define	MAX_TOTSHARE_SIZE   (0x012FFFE0 - 0x1280000)

extern unsigned char *m_gObjBuf, *m_gCodeBuf, *m_gImageBuf, *m_gUncompBuf;

#define	MAX_SKY_IMAGE_WIDTH	120//128	/* maximum width of a image frame (pixels) */
#define	MAX_SKY_IMAGE_HEIGHT 120//128	/* maximum heigth of a image frame (pixels) */

/////////////////////////////////////////////////////////

#define	MAX_IMAGE_WIDTH	256	/* maximum width of a image frame (pixels) */
#define	MAX_IMAGE_HEIGHT 256	/* maximum heigth of a image frame (pixels) */
#define	MAX_FRAMES 128	/* maximum number of animation frames */

/* Get SIS Information function*/
/*   - parameter 1 : pointer to encoded (compressed) binary data  */
/*   - parameter 2 : pointer to register for number of frames of image data	 */
/*   - parameter 3 : pointer to register for number of pixels in x axis */
/*   - parameter 4 : pointer to register for number of pixels in y axis */
/*   - parameter 5 : pointer to register flag for variable delay or not	 */
/*   - parameter 6 : pointer to register values of variable delay if variableDelayorNot is set to 1 */
/*   - parameter 7 : pointer to register for best frame number */
/*   - parameter 8 : pointer to register for image levelype */
/*   - parameter 9 : pointer to register for loop-playing or not  */
/*   - output	: 0 - Error, 1-No Error  */
extern int getSISInfo(
						unsigned char* startOfImageData, 
						int* numOfFrames, 
						int* xSize, 
						int* ySize, 
						int* variableDelayorNot,
						int delay[], 
						int* best, 
						int* imagelevel, 
						int* Looped_or_not 
);
#ifdef FEATURE_MLZ
/* SIS (Simple Image Service) image decoding function 				*/
/*   - parameter 1 : pointer to encoded (compressed) binary data 	*/
/*   - parameter 2 : pointer to (will-be) decoded (uncompressed) binary data array	*/
/*   - parameter 3 : 												*/
/* if     -1 :  decodes the whole frames							*/ 
/*		 else     :  decodes the "parameter 3" frame ....  			*/
/*   - parameter 4 : 												*/
/*		 															*/
/*   - output 	     0        : Error, 								*/	
/*                     total frame number - No Error 				*/
extern int sisDecode(unsigned char*, unsigned char*, int,int);


/* SIS Global Initialization function */
/* User should provide the required memory to the SIS Library through this function */
/* size_globalObjectBuffer should be 65536 bytes, word aligned */
/* size_coderBuf should be 25600 bytes, word aligned */
/* xsize_LCD should be the working horizontal size of LCD, in pixel */
/* ysize_LCD should be the working vertical size of LCD, in pixel */
extern int sisInitialize(
			unsigned char* globalObjectBuf, 
			int size_globalObjectBuf, 
			unsigned char* coderBuf, 
			int size_coderBuf, 
			int xsize_LCD, 
			int ysize_LCD
);
#else
/* SIS (Simple Image Service) image decoding function */
/*   - parameter 1 : pointer to encoded (compressed) binary data 	*/
/*   - parameter 2 : pointer to (will-be) decoded (uncompressed) binary data array	*/
/*   - parameter 3 : 
 if     -1 :  decodes the whole frames 
		 else     :  decodes the "parameter 3" frame ....  */
/*   - output 	     0        : Error, 
                     total frame number - No Error */
extern int sisDecode(unsigned char*, unsigned char*, int);


/* SIS Global Initialization function */
/* User should provide the required memory to the SIS Library through this function */
/* size_globalObjectBuffer should be 65536 bytes, word aligned */
/* size_coderBuf should be 34000 bytes, word aligned */
/* xsize_LCD should be the working horizontal size of LCD, in pixel */
/* ysize_LCD should be the working vertical size of LCD, in pixel */
extern int sisInitialize(
			unsigned char* globalObjectBuf, 
			int size_globalObjectBuf, 
			unsigned char* coderBuf, 
			int size_coderBuf, 
			int xsize_LCD, 
			int ysize_LCD
);
#endif // FEATURE_MLZ

extern unsigned char* getSIS_pal_Info(int* paletteType);
extern void set_pal_process_mode(int mode,unsigned char* taget_palette);

extern int sis_prepare_images(unsigned char* startOfImageData);

extern unsigned char* SeekWorkingArea(int work);

#else

#define	MAX_IMAGE_WIDTH	128	/* maximum width of a image frame (pixels) */
#define	MAX_IMAGE_HEIGHT	64	/* maximum heigth of a image frame (pixels) */
#define	MAX_FRAMES	20	/* maximum number of animation frames */

/* Get image size function*/
/*   - parameter 1 : pointer to encoded (compressed) binary data 	*/
/*   - parameter 2 : pointer to register for number of frames of image data	*/
/*   - parameter 3 : pointer to register for number of pixels in x axis	*/
/*   - parameter 4 : pointer to register for number of pixels in y axis	*/
/*   - parameter 5 : pointer to register for speed	*/
/*   - parameter 6 : pointer to register for best frame number	*/
/*   - output	: 0 - Error, 1-No Error */
extern int getImageSize(unsigned char*, int*, int*, int*, int*, int*);

/* SIS (Simple Image Service) image decoding function */
/*   - parameter 1 : pointer to encoded (compressed) binary data 	*/
/*   - parameter 2 : pointer to (will-be) decoded (uncompressed) binary data array	*/
/*   - parameter 3 : 
		if -1 : decode the whole frames 
		else  : decode the parameter 3 frame .... */
/*   - output	: 0 - Error,  total frame numbrt - No Error */
extern int sisDecode(unsigned char* , unsigned char*, int);

#endif//1