/*
** ============================================================================
**
** Project: 
**     Athens
**
** File:
**     eqa.h
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
** 2000-07-20 jrkwon     Created.
**
** ============================================================================
*/
//#ifdef FEATURE_SKTT_PM

#ifndef _SKA_VM_H_
#define _SKA_VM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define email_version
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define TO_CAMERA_FROM_VMAIL    1   //white 2001.03.25
#define PM_MAX_NUM_MIN			5	//송신번호 개수
#ifdef email_version
    #define PM_MAX_SIZE_MIN		30	
#else
    #define PM_MAX_SIZE_MIN		11	
#endif  //email_version
#define PM_MAX_SIZE_TEXT		80	//텍스트 크기
#define PM_MAX_NUM_PM			30	//총PM 개수
#define PM_MAX_BUF_SIZE         sizeof(BYTE)*IMAGE_VIEWER_TOTAL_SIZE + sizeof(PM_RECORD_T)
#define PM_MAX_BUF_SIZE1        sizeof(PM_RECORD_T)
#define PM_LIST_FILE_SIZE       sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM
#define PM_CLEAR_POPUP          7124
#define PM_END_POPUP            7115
#define PM_TRAN_POPUP           7114
#define PM_CLEAR1_POPUP         7113
//#define PM_SEND_EVENT           7112
 

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

struct tagPM_RECORD_T {
    
    DWORD	PMPhoto;
    DWORD   Save_time;
    DWORD	Send_time;
    BOOL    Sent;
	UINT8	MIN_NUM;
    UINT8   DecoNum;
    BYTE    PMTitle[LCD_COLUME_SIZE];
    BYTE	PMText[PM_MAX_SIZE_TEXT+2];
	BYTE	MIN[PM_MAX_NUM_MIN][PM_MAX_SIZE_MIN+1];

};
typedef struct  tagPM_RECORD_T  PM_RECORD_T;

struct tagPM_IMI_RECORD_T {

    DWORD Save_time;
    BYTE nAddr[SKY_FILE_EXT_NAME_MAX_LENGTH+1];  
    BYTE PMTitle[LCD_COLUME_SIZE];
    BOOL fExist;
    BOOL Sent;
};

typedef struct  tagPM_IMI_RECORD_T  PM_IMI_RECORD_T;

#ifdef WIN32
#include <pshpack2.h>
typedef struct tagBMPFILEHEADER{
	
	WORD		bfType;
	DWORD 	    bfSize;
	WORD 		bfReserved1;
	WORD 		bfReserved2;
	DWORD 	    bfOffBits;
} BMPFILEHEADER;
#include <poppack.h>


typedef struct tagBMPINFOHEADER{

	DWORD 	    biSize;
	LONG 		biWidth;
	LONG 		biHeight;
	WORD 		biplanes;
	WORD 		biBitCount;
	DWORD 	    biCompression;
	DWORD 	    biSizeImage;
	LONG 		biXPelsPerMeter;
	LONG 		biYPelsPerMeter;
	DWORD 	    biClrUsed;
	DWORD 	    biClrImportant;
}BMPINFOHEADER;

typedef struct tagRGBQUAD{

    BYTE 		rgbBlue;
	BYTE 		rgbGleen;
	BYTE 		rgbRed;
    BYTE 		rgbReserved;
} RGBQUAD;
typedef PACKED struct tagBMPINFO{

	BMPINFOHEADER	bmiHeader;
	RGBQUAD	    bmiColors[256];
} BMPINFO;
#else
typedef PACKED struct tagBMPFILEHEADER{
	
	WORD		bfType;
	DWORD 	    bfSize;
	WORD 		bfReserved1;
	WORD 		bfReserved2;
	DWORD 	    bfOffBits;
} BMPFILEHEADER;


typedef PACKED struct tagBMPINFOHEADER{

	DWORD 	    biSize;
	LONG 		biWidth;
	LONG 		biHeight;
	WORD 		biplanes;
	WORD 		biBitCount;
	DWORD 	    biCompression;
	DWORD 	    biSizeImage;
	LONG 		biXPelsPerMeter;
	LONG 		biYPelsPerMeter;
	DWORD 	    biClrUsed;
	DWORD 	    biClrImportant;
}BMPINFOHEADER;

typedef struct tagRGBQUAD{

    BYTE 		rgbBlue;
	BYTE 		rgbGleen;
	BYTE 		rgbRed;
    BYTE 		rgbReserved;
} RGBQUAD;
typedef PACKED struct tagBMPINFO{

	BMPINFOHEADER	bmiHeader;
	RGBQUAD	    bmiColors[256];
} BMPINFO;
#endif

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern BYTE g_PhoneNumBuf[MAX_DIAL_DIGIT_LENGTH+1];
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern VOID SKY_API SKA_MainPMList(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_EditPM(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_EditNewPM(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_RedactPM(SKY_EVENT_T *pEvent);
extern VOID SKY_DeletePMPhoto(DWORD dwIndex);
extern VOID SKY_InitPM(VOID);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* _SKA_VM_H_ */

//#endif /* FEATURE_SKTT_PM */