/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


							WAP PhotoMail Main Module

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS
  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/17/2001 jmpark  Create WAP address initialize for SIS, PhotoMail
===========================================================================*/
/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"
#include "msg.h"
#include "err.h"
#include "wapadapter.h"
#include "wapconnector.h"
#include "clnt.h"
#include "confvars.h"
#include <memory.h>

#include "aapimem.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "capimem.h"
#include "capimmi.h"
#include "netuser.h"
#include "tapicmmn.h"

#include "sisdecoder.h"
#if MMI_TYPE == SKY_UI
#include "sky.h"
#endif //#if MMI_TYPE == SKY_UI

#define TEST_FLAG
#undef  WIN32

//////////////////////////////////////////////////////////////
enum {
	MIN   = 0x01,
	TEXT  = 0x02,
	IMAGE = 0x03
};

#define GUBUN  0x1B
#define AddGubun(p)   *p = GUBUN, \
					  len += 1

//////////////////////////////////////////////////////////////
#define LCD_COLUME_SIZE	 18

#define PM_MAX_SIZE_TEXT 80
#define PM_MAX_NUM_MIN   5
#define PM_MAX_SIZE_MIN  10

//#define FRAME_SIZE_TEST

#ifdef FRAME_SIZE_TEST
	int     FRAME_SIZE = 0; 
#else
	#define FRAME_SIZE 700  
#endif//1

#define MAX_SHARED_SIZE  10*1000
#define MAX_FILE_SIZE    MAX_SHARED_SIZE
#define MAX_CALLBUF_SIZE 60
#define HEADER_SIZE      2

typedef  struct  {  
    dword	PMPhoto;
    dword   Save_time;
    dword	Send_time;
    boolean Sent;
	byte	MIN_NUM;
    byte    DecoNum;
    BYTE    PMTitle[LCD_COLUME_SIZE];
    byte	PMText[PM_MAX_SIZE_TEXT+2];
	byte	MIN[PM_MAX_NUM_MIN][PM_MAX_SIZE_MIN+1];
	word    lenImage;
} PM_RECORD_T;

#ifdef TEST_FLAG
static PM_RECORD_T test_pm;
#endif//test

typedef struct 
{
	unsigned char numFrame;
	unsigned char totFrame;
} HEADER;

#ifdef TEST_FLAG
unsigned char TEXT_DATA[] = "abcdefghij" ; //hijklmnopqrstuvwxyz0123456789!@#$%^&*()";
unsigned char test_min1[] = "0123456789" ;
unsigned char test_min2[] = "9876543210" ;
unsigned char test_min3[] = "0123456789" ;
unsigned char test_min4[] = "9876543210" ;
unsigned char test_min5[] = "0123456789" ;
#endif//test_flag
////////////////////////////////////////////////////////////////
extern unsigned char *pStartPM, *pDevCallBuffer;  //Photo Mail
////////////////////////////////////////////////////////////////

extern void Initialize_PhotoMail(void);

int ReadMinNumber(unsigned char* phoneNumber, int lenMin)
{
	unsigned char* ptell, *psrc;
	int i=0 ;

	memset((void*)pDevCallBuffer, 0, MAX_CALLBUF_SIZE );

	ptell = pDevCallBuffer;
	psrc  = phoneNumber;	

	while(lenMin--)
	{		
		psrc--; //to be referenced.
		do 
			if((psrc = (unsigned char*)strpbrk((const char*)(psrc+1), "1234567890"))!= '\0') 
				*ptell++ = *psrc;
		while( psrc != '\0');
		*ptell= ',';

		if(lenMin)
		{
			psrc = &phoneNumber[(++i)*(PM_MAX_SIZE_MIN+1)] ;
			ptell++;
		}
	}

	*ptell = '\0';
#ifdef WIN32
	printf("\nRetrieved phone number [%s]",pDevCallBuffer);
#else
	CLNTa_log(1,0,"\nRetrieved phone number [%s]",pDevCallBuffer);
#endif//win32
	return strlen((const char*) pDevCallBuffer);
}

unsigned char ComputeTotalFrame(int sizeFrame)
{
	int lenMin = 0, lenText = 0, lenImage = 0 ;
	unsigned char total = 0;


    PM_RECORD_T    *pm  = (PM_RECORD_T *)pStartPM;

	lenMin   = ReadMinNumber((unsigned char*)pm->MIN, (int) pm->MIN_NUM);
	lenText  = strlen((const char*)pm->PMText); //getlenth??
	lenImage = pm->lenImage ;

	total  = ( (lenMin + lenText + lenImage ) / sizeFrame) ;
	//even though total dosen't has remain, add total frame. 
	//Not Good,but simple !
	total++;  

#ifndef WIN32
	MSG_HIGH("lenText[%d], lenImage[%d] totalF[%d]", lenText, lenImage, total);
#endif
	return total;
}



//To write a photo mail to shared ram area.
int WritePhotoMail(unsigned char* PM, unsigned char* imageData, int size)
{
	extern void AddressAssign(void);
	unsigned char* pDest  = pStartPM;
	PM_RECORD_T*   pm ;

	AddressAssign();
	pDest = pStartPM;
	pm = (PM_RECORD_T*)pDest;

	Initialize_PhotoMail();

	if( size == 0 || size >= 50*1000 )	
	{
#ifndef WIN32
		MSG_ERROR("lenth of Image Data is invalid [%d]",size,0,0);
#endif//win32
		return 0;
	}
	memset(pStartPM, 0, MAX_OBJBUF_SIZE);
	memcpy((unsigned char*)pDest, PM, sizeof(PM_RECORD_T) );
	pm->lenImage = size;

	pDest += sizeof(PM_RECORD_T);
	memcpy((unsigned char*)pDest, imageData, size);
	return size;
}


//To read a photo mail data from shared ram area.
int ReadPhotoMail(unsigned char* pDest, int position, int size)
{
	unsigned char* pSrc = (unsigned char*)pStartPM;

	if( position == 0 || size == 0)
	{
#ifndef WIN32
		MSG_ERROR("Invaild argument[s]",0,0,0);
#endif//win32
		return -1;
	}	
	memcpy((unsigned char*)pDest, (const unsigned char*)&pSrc[position], size);
	return size;
}
/*
int ReadPhotoMail(unsigned char* pDest, int position, int size)
{
	unsigned char* pSrc = (unsigned char*)pStartPM;
	int len = 0, esc = 0;

	if( position == 0 || size == 0)
	{
#ifndef WIN32
		MSG_ERROR("Invaild argument[s]",0,0,0);
#endif//win32
		return -1;
	}	
	len = 0;
	pSrc += position;
	while(len < size)
	{
		if(*pSrc == GUBUN)
		{
		    *pDest++ = GUBUN;
			len++;
			esc++;
		}
		*pDest++ = *pSrc++;
		len++;
	}
	if( *(--pSrc) == GUBUN && esc != 0 ) esc--;
	return size-esc;
}
*/

#ifdef TEST_FLAG_not
void check_overflow(void)
{
	unsigned char last_byte;

	last_byte = data_buf[BUF_SIZE];
	if(last_byte != '.')
	{
#ifdef WIN32
		printf("\nYOU ARE OVERFLOW[%d]",data_buf[BUF_SIZE]);
#else
		MSG_HIGH("YOU ARE OVERFLOW",0,0,0);
#endif//win32
	}
}
#endif//test_flag

static int curFrame = 1;
static int offset   = 0;
static int totFrame = 0;

/*
void static nop(void)
{
	return;
}

int test_flag = 0;
*/

void Initialize_PhotoMail(void)
{
	curFrame = 1;
	offset   = 0;
	totFrame = 0;
}

void sendData(int svcID, int isAll, char* Result)
{
	int len = 0;
	int temp_len  = 0;

#ifndef WIN32
    ui_cmd_type	    *ui_cmd_ptr;
#endif//win32

	HEADER *pHeader = (HEADER*) Result;
	PM_RECORD_T *pm = (PM_RECORD_T*) pStartPM;
#ifdef FRAME_SIZE_TEST
	{
		extern BOOL fTest_flag1;

		if(fTest_flag1)
			FRAME_SIZE = 800;
		else
			FRAME_SIZE = 900;
	}
#endif//test
	if(isAll == -1)
	{
		curFrame  = 1;
		Result[0] = '0';

#ifndef WIN32
		if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
		{
			ERR_FATAL( "No UI cmd buffers",0,0,0);
			return;
		}
		else		
		{
			ui_cmd_ptr->hdr.cmd        = UI_PHOTOMAIL_F;
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
		}	
	    CLNTa_log(1,0,"++++ sendData is completed ++++",len);
#endif
		return;
	}
////////////////////////////////////////////////////////////
//  START OF HEADER
////////////////////////////////////////////////////////////
	pHeader->numFrame   = curFrame++;
	if(pHeader->numFrame == 1)
	{
		pHeader->totFrame = totFrame = ComputeTotalFrame(FRAME_SIZE-HEADER_SIZE);
	}
	else
	{
		pHeader->totFrame = totFrame;
		if(pHeader->numFrame > totFrame)
		{
			ERR_FATAL("Dont' exceed totFrame",0,0,0);
			return;
		}
	}	
	

////////////////////////////////////////////////////////////
//  START OF BODY
////////////////////////////////////////////////////////////
	len = 2;  

	if(pHeader->numFrame == 1)
	{
		offset = 0;
		//MIN
		Result[len++] = MIN;  
		temp_len = strlen((const char*)pDevCallBuffer) ; //sublength of MIN
		memcpy((void*)&Result[len], (const void*)pDevCallBuffer, temp_len );
		len += temp_len;

		//TEXT
		temp_len = strlen((const char*)pm->PMText);
		if( temp_len )
		{
			AddGubun(&Result[len]);
			Result[len++] = TEXT;
			memcpy(&Result[len], pm->PMText, temp_len );
			len += temp_len;
		}

		//IMAGE
		AddGubun(&Result[len]);
		Result[len++] = IMAGE;
		temp_len = FRAME_SIZE - len -1;  //sublength  of IMAGE
		offset += sizeof(PM_RECORD_T);  
		offset += ReadPhotoMail((unsigned char*)&Result[len], offset, temp_len ); 
		len    += temp_len;		
	}else
	{
		//IMAGE
		Result[len++] = IMAGE;
		temp_len  = FRAME_SIZE - len -1;  //sublength  of IMAGE
		offset += ReadPhotoMail((unsigned char*)&Result[len], offset, temp_len );
		len    += temp_len;
	}

#ifdef WIN32
	printf("\nResult [%s]", Result);
#else
	CLNTa_log(1,0,"++++ sendData is called len[%d] ++++",len);
#endif

#ifdef TEST_FLAG_not
	check_overflow();
#endif//test

}

#ifdef TEST_FLAG
unsigned char IMAGE_DATA[] = 
"\
0000>00000000000000 0001>11111111111111 0002>22222222222222 0003>33333333333333 \
0004>44444444444444 0005>55555555555555 0006>66666666666666 0007>77777777777777 \
0008>88888888888888 0009>99999999999999 0010>AAAAAAAAAAAAAA 0011>BBBBBBBBBBBBBB \
0012>CCCCCCCCCCCCCC 0013>DDDDDDDDDDDDDD 0014>EEEEEEEEEEEEEE 0015>FFFFFFFFFFFFFF \
0016>GGGGGGGGGGGGGG 0017>HHHHHHHHHHHHHH 0018>IIIIIIIIIIIIII 0019>JJJJJJJJJJJJJJ \
0020>KKKKKKKKKKKKKK 0021>LLLLLLLLLLLLLL 0022>MMMMMMMMMMMMMM 0023>NNNNNNNNNNNNNN \
0024>OOOOOOOOOOOOOO 0025>PPPPPPPPPPPPPP 0026>QQQQQQQQQQQQQQ 0027>RRRRRRRRRRRRRR \
0028>SSSSSSSSSSSSSS 0029>TTTTTTTTTTTTTT 0030>UUUUUUUUUUUUUU 0031>VVVVVVVVVVVVVV \
0032>WWWWWWWWWWWWWW 0033>XXXXXXXXXXXXXX 0034>YYYYYYYYYYYYYY 0035>ZZZZZZZZZZZZZZ \
0036>aaaaaaaaaaaaaa 0037>bbbbbbbbbbbbbb 0038>cccccccccccccc 0039>dddddddddddddd \
0040>eeeeeeeeeeeeee 0041>ffffffffffffff 0042>gggggggggggggg 0043>hhhhhhhhhhhhhh \
0044>iiiiiiiiiiiiii 0045>jjjjjjjjjjjjjj 0046>kkkkkkkkkkkkkk 0047>llllllllllllll \
0048>mmmmmmmmmmmmmm 0049>nnnnnnnnnnnnnn 0050>oooooooooooooo 0051>pppppppppppppp \
0052>qqqqqqqqqqqqqq 0053>rrrrrrrrrrrrrr 0054>ssssssssssssss 0055>tttttttttttttt \
0056>uuuuuuuuuuuuuu 0057>vvvvvvvvvvvvvv 0058>wwwwwwwwwwwwww 0059>xxxxxxxxxxxxxx \
0060>yyyyyyyyyyyyyy 0061>zzzzzzzzzzzzzz 0062>zzzzzzzzzzzzzz 0063>yyyyyyyyyyyyyy \
0064>xxxxxxxxxxxxxx 0065>wwwwwwwwwwwwww 0066>vvvvvvvvvvvvvv 0067>uuuuuuuuuuuuuu \
0068>tttttttttttttt 0069>ssssssssssssss 0070>rrrrrrrrrrrrrr 0071>qqqqqqqqqqqqqq \
0072>pppppppppppppp 0073>oooooooooooooo 0074>nnnnnnnnnnnnnn 0075>mmmmmmmmmmmmmm \
0076>llllllllllllll 0077>kkkkkkkkkkkkkk 0078>jjjjjjjjjjjjjj 0079>iiiiiiiiiiiiii \
0080>hhhhhhhhhhhhhh 0081>gggggggggggggg 0082>ffffffffffffff 0083>eeeeeeeeeeeeee \
0084>dddddddddddddd 0085>cccccccccccccc 0086>bbbbbbbbbbbbbb 0087>aaaaaaaaaaaaaa \
0088>ZZZZZZZZZZZZZZ 0089>YYYYYYYYYYYYYY 0090>XXXXXXXXXXXXXX 0091>WWWWWWWWWWWWWW \
0092>VVVVVVVVVVVVVV 0093>UUUUUUUUUUUUUU 0094>TTTTTTTTTTTTTT 0095>SSSSSSSSSSSSSS \
0096>RRRRRRRRRRRRRR 0097>QQQQQQQQQQQQQQ 0098>PPPPPPPPPPPPPP 0099>OOOOOOOOOOOOOO ";


void CopyImageData(void)
{
	extern void AddressAssign(void);
    unsigned char* pDest;//    = (unsigned char*)pStartPM;
	int            temp_len = 0;

	AddressAssign();
	pDest = pStartPM;

	memcpy((unsigned char*)pDest, (const unsigned char*)&test_pm, sizeof(test_pm));
	pDest += temp_len = sizeof(test_pm);
	memcpy((unsigned char*)pDest, IMAGE_DATA, sizeof(IMAGE_DATA) );

#ifdef ESC
	{
		*(pDest+26) = 0x1b;
		*(pDest+21*20+5) = 0x1b;
		*(pDest+5) = 0x1b;
	}
#endif//esc

}

void photomail_main(void)
{
	test_pm.Sent    = 1;
    test_pm.MIN_NUM = 5;
	memcpy((void*)test_pm.PMText, (const void*)TEXT_DATA, sizeof(TEXT_DATA) );
	memcpy((void*)&test_pm.MIN[0][0], (const void*)test_min1, sizeof(test_min1) );
	memcpy((void*)&test_pm.MIN[1][0], (const void*)test_min2, sizeof(test_min2) );
	memcpy((void*)&test_pm.MIN[2][0], (const void*)test_min3, sizeof(test_min3) );
	memcpy((void*)&test_pm.MIN[3][0], (const void*)test_min4, sizeof(test_min4) );
	memcpy((void*)&test_pm.MIN[4][0], (const void*)test_min5, sizeof(test_min5) );
	test_pm.lenImage = sizeof(IMAGE_DATA) ;

	CopyImageData();

	curFrame = 1;
}
#endif//test_flag