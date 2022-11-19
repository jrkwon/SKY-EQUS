/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     
**
** Description: 
**     <@Desciption about this file here>
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-07-21   woodstok   Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "target.h"
#include "customer.h"
#include "eqs.h"
#include "adc.h"
#include "sky.h"
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#define CAMERA_H_SZ		120		// 가로 dot size
#define CAMERA_V_SZ		 90		// 세로 dot size
#define CAMERA_RGB_SZ	(CAMERA_H_SZ * CAMERA_V_SZ)

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/


/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
//byte YUV[14000];	//header 32byte data 6912*2 end2byte = 13858
// ska_cam.c에서 선언했음 01.06.19
extern byte YUV[21700];	//header 32byte data 120*90*2 end2byte = 21600+34
extern byte rgb[21600];
//extern byte rgb[CAMERA_RGB_SZ];

#define CAMERA_120BY90

#ifdef CAMERA_120BY90
ROM byte CameraHeader[]={0xaa,0x55,0xff,0xd8,0x1c,0xf0 ,0xf1,0x00,0x78,0x00,
		       0x5a,0x81,0x41,0x20,0x4b,0x43,0x23,0x38,0x42,0x50,
			   0x2e,0x2e,0,   0,   0,   0,   0,   0,   0,   0,
			   0xaa,0x55};
#else
ROM byte CameraHeader[]={0xaa,0x55,0xff,0xd8,0x1c,0xf0,0xf1,0x00,0x60,0x00,
		       0x48,0x81,0x31,0x20,0x4b,0x43,0x23,0x38,0x42,0x50,
			   0x2e,0x2e,0,   0,   0,   0,   0,   0,   0,   0,
			   0xaa,0x55};
#endif
ROM byte CameraEndmark[] = {0xff, 0xd9};

byte bReverse = 0;



void yamn_clk_off(void)
{
	// yamn clock off 
	DMOD_OUT( DMOD_FM_CLK_MDIV_0, 0);
	DMOD_OUT( DMOD_FM_CLK_MDIV_1, 0);
	DMOD_OUT( DMOD_FM_CLK_NDIV_0, 0 );
	DMOD_OUT( DMOD_FM_CLK_NDIV_1, 0x10 );
	DMOD_OUT( DMOD_FM_CLK_DUTY_0, 0xff);
	DMOD_OUT( DMOD_FM_CLK_DUTY_1, 0x1f);
}

void yamn_clk_on(void)
{
	DMOD_OUT( DMOD_FM_CLK_MDIV_0, 1);
	DMOD_OUT( DMOD_FM_CLK_MDIV_1, 0);
	DMOD_OUT( DMOD_FM_CLK_NDIV_0, 0xfe );
	DMOD_OUT( DMOD_FM_CLK_NDIV_1, 0x1f );
	DMOD_OUT( DMOD_FM_CLK_DUTY_0, 1);
	DMOD_OUT( DMOD_FM_CLK_DUTY_1, 0);
}
/*---------------------------------------------------------------------------
FUNCTION InitCamera

DESCRIPTION
	initialize for camera module
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

---------------------------------------------------------------------------*/

void InitCamera (void)
{
	word i;

	yamn_clk_off();
//	BIO_OUT (DEC_GPIO_0, BIO_MOD_EN_M , BIO_MOD_EN_LO_V );	// mod_en pin low -> camera standby

  #if MODEL_ID==MODEL_B1 || (MODEL_ID == MODEL_B2)	// by jnlee 01.04.23
	// cam working하기 위하여 audio_en전원 사용 
	AUDIO_EN_ON();
	fAudioPwrOff |= AUDIO_EN_BY_CAM_WRK;
  #endif

#if MODEL_ID == MODEL_B2	// by jnlee 01.10.17
	for(i = 0 ; i <CAMERA_RGB_SZ*2 ; i++)
		rgb[i] = 0xff;	// 120*90*2byte
#else
	// all rgb data clear
	for(i = 0 ; i <CAMERA_RGB_SZ ; i++)
		rgb[i] = 0;
#endif

}

/*---------------------------------------------------------------------------
FUNCTION StopCamera

DESCRIPTION
	End for camera module
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

---------------------------------------------------------------------------*/
void StopCamera (void)
{
	yamn_clk_off();

	//by jnlee for b2 01.06.12에서는 default상태가 low임
//	BIO_OUT (DEC_GPIO_0, BIO_MOD_EN_M  , BIO_MOD_EN_HI_V );	// mod_en pin high

  #if MODEL_ID==MODEL_B1 || (MODEL_ID == MODEL_B2)	// by jnlee 01.04.23
	// cam working하기 위하여 audio_en전원 사용 
	fAudioPwrOff &= ~AUDIO_EN_BY_CAM_WRK;
	AUDIO_EN_OFF();
  #endif

}

/*---------------------------------------------------------------------------
FUNCTION CameraCapturedRGB

DESCRIPTION
	Acquistion from camera module and convert YUV to RGB
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

---------------------------------------------------------------------------*/
//#define CAMERA_AVERAGE
long YColorDensity =  8;		// color세기를 정하는 변수 
long YColorAdd		= 30;		// color세기를 정하는 변수 
long RColorDensity =  2;		// color세기를 정하는 변수 
long GColorDensity =  2;		// color세기를 정하는 변수 
long BColorDensity =  2;		// color세기를 정하는 변수 

long Rmul = 8;
long Gmul = 8;
long Bmul = 8;

long RAdd = 0;
long GAdd = 0;
long BAdd = 0;

word DLY_1DATA=12;	// 1 byte읽은 후 delay

#ifdef CAMERA_120BY90
	#define YUV_READ_MAX	21634
	#define RGB_CNV_COUNT	10800
#else
	#define YUV_READ_MAX	13859
	#define RGB_CNV_COUNT	6912
#endif

#ifdef FEATURE_64KCOLOR_LCD
extern byte tempp[128*164*2];
#endif

unsigned long test1=30000;
unsigned long test2=30000;

byte *CameraCapturedRGB (void)
{
    word i,j,read_count=0;
	long RRR[2],GGG[2],BBB[2],YY[2],UU,VV;
	byte check_count=0,check_count2=0;
//	register word add_temp;
	long blue_thres[] = {0,32,128,224};

#ifdef FEATURE_64KCOLOR_LCD
	register byte *ctemp;
	word cconv;
	ctemp = rgb;
#endif


//	if(g_SysState.fCamera)
	{	// camera삽입 state인 경우에 mod_en을 low (active) 로 함
	//by jnlee for b2 01.06.11에서는 default상태가 low임
//		BIO_OUT (DEC_GPIO_0, BIO_MOD_EN_M , BIO_MOD_EN_LO_V );	// mod_en pin low -> camera standby
	}

  while(1)
  {
	dog_kick();
	// clock발생 
	yamn_clk_on();

	for(i=0;i< 10 ; i++);	// 20us delay

	/******** capture start*******/
	INTLOCK();
	dog_kick();			
	for(i=0	; i<700 ; i++)
	{
		if(read_count >= YUV_READ_MAX)
			break;
		for(j=0;j<DLY_1DATA;j++);
//		YUV[read_count] = *(byte *)0x2000000;
		YUV[read_count] = *(byte *)0x800000;	//ROM_CS2이용 for B2 by jnlee 01.07.02
		read_count++;
	}
	INTFREE();
	/******** capture end ********/

	// YAMN clock -> low
	yamn_clk_off();

	if(read_count >= YUV_READ_MAX)
		break;
  }


#if MODEL_ID == MODEL_B1
	/******** camera 탈착 확인 ***/
	for(i = 0 ; i < 5 ; i++)
	{
		if(YUV[i] == 0xff)
			check_count++;
		if(YUV[i] == 0)
			check_count2++;
	}
	if((check_count == 5)||(check_count2==5))
	{
		// 데이터가 0xff이면, 카메라가 빠진 것임 
	//by jnlee for b2 01.06.12에서는 default상태가 low임
//		BIO_OUT (DEC_GPIO_0, BIO_MOD_EN_M  , BIO_MOD_EN_HI_V );	// mod_en pin high
	}
#endif


	if(YUV[2] == CameraHeader[2])
	{	//정상적인 경우 
		/********* header compare *****************/
		for( i = 1, check_count = 1 ; i < sizeof(CameraHeader); i++)
		{	// 처음 데이터는 오류가 있어서 비교에 제외함 
			if(YUV[i] == CameraHeader[i])
				check_count++;
		}
		/********* endmarker compare *****************/
		for( i = 0; i < sizeof(CameraEndmark); i++)
		{
			if(YUV[i+YUV_READ_MAX-2]==CameraEndmark[i])
				check_count++;
		}
		j=32;
	}
	else
	{	// 비정상적으로 한 바이트 밀려 들어오는 경우 by jnlee 01.05.07
		/********* header compare *****************/
		for( i = 1, check_count = 1 ; i < sizeof(CameraHeader); i++)
		{	 
			if(YUV[i+1] == CameraHeader[i])
				check_count++;
		}
		/********* endmarker compare *****************/
		// 이 경우 마지막에는 한 바이트 모자라므로 1바이트 만 비교함 
		if(YUV[YUV_READ_MAX-1]==CameraEndmark[0])
			//중간에 카메라가 빠지면 이후 데이터가 ff가 되는 경우가 있으므로 , 
			//end mark를 ff하나만 처리하면 부족함. 하나앞의 데이터도 ff이면 에러 처리함 
			if(YUV[YUV_READ_MAX-2] != 0xff)			// by jnlee 01.09.18
				check_count+=sizeof(CameraEndmark);
		j=33;
	}


  if(check_count == ( sizeof(CameraHeader)+sizeof(CameraEndmark)) )		// OK
//  if(check_count >= sizeof(CameraHeader) )		//
  {
	/******** start convert YUV to RGB ********/
	dog_kick();

	for (i=0; i<RGB_CNV_COUNT; i+=2)
	{
#ifdef CAMERA_120BY90
		YY[0] = YUV[j++]+YColorAdd;
		UU    = YUV[j++]-128;
		YY[1] = YUV[j++]+YColorAdd;
		VV    = YUV[j++]-128;
#else
		UU    = YUV[j++]-128;
		YY[0] = YUV[j++]+YColorAdd;
		VV    = YUV[j++]-128;
		YY[1] = YUV[j++]+YColorAdd;
#endif
		YY[0] = YY[0]-16;
		YY[1] = YY[1]-16;
				
		YY[0] = (YY[0]<0)?0:YY[0];
		YY[1] = (YY[1]<0)?0:YY[1];
		YY[0] *= YColorDensity;
		YY[0] >>= 3;
		YY[1] *= YColorDensity;
		YY[1] >>= 3;

		RRR[0] = (178 * VV) >> 7; 
		RRR[0] *= RColorDensity;
		RRR[1] = RRR[0] + YY[1];
		RRR[0] = RRR[0] + YY[0];

		// original formula
		GGG[0] = (21 * UU + 48 * VV) >> 6;
		GGG[0] *= GColorDensity;
		GGG[1] = YY[1] - GGG[0] ;
		GGG[0] = YY[0] - GGG[0] ;

		BBB[0] = (7 * UU) >> 2;
		BBB[0] *= BColorDensity;
		BBB[1] = BBB[0] + YY[1];
		BBB[0] = BBB[0] + YY[0];


		RRR[0] = (RRR[0]*Rmul)>>3;
		RRR[1] = (RRR[1]*Rmul)>>3;
		GGG[0] = (GGG[0]*Gmul)>>3;
		GGG[1] = (GGG[1]*Gmul)>>3;
		BBB[0] = (BBB[0]*Bmul)>>3;
		BBB[1] = (BBB[1]*Bmul)>>3;

		RRR[0] += RAdd;
		RRR[1] += RAdd;
		GGG[0] += GAdd;
		GGG[1] += GAdd;
		BBB[0] += BAdd;
		BBB[1] += BAdd;

		if(RRR[0]<0) RRR[0] = 0;				
		if(RRR[1]<0) RRR[1] = 0;				
		if(GGG[0]<0) GGG[0] = 0;				
		if(GGG[1]<0) GGG[1] = 0;				
		if(BBB[0]<0) BBB[0] = 0;				
		if(BBB[1]<0) BBB[1] = 0;				

		if(RRR[0]>0xff) RRR[0] = 0xff;
		if(RRR[1]>0xff) RRR[1] = 0xff;
		if(GGG[0]>0xff) GGG[0] = 0xff;
		if(GGG[1]>0xff) GGG[1] = 0xff;
		if(BBB[0]>0xff) BBB[0] = 0xff;
		if(BBB[1]>0xff) BBB[1] = 0xff;

#ifdef FEATURE_64KCOLOR_LCD
		cconv = (word)(RRR[0]&0xf8);
		cconv<<=5;
		cconv |= (word)(GGG[0]&0xfc);
		cconv<<=3;
		cconv |= (word)((BBB[0]>>3)&0x1f);
		*ctemp = (byte)(cconv);
		ctemp++;
		*ctemp = (byte)(cconv>>8);
		ctemp++;

		cconv = (word)(RRR[1]&0xf8);
		cconv<<=5;
		cconv |= (word)(GGG[1]&0xfc);
		cconv<<=3;
		cconv |= (word)((BBB[1]>>3)&0x1f);
		*ctemp = (byte)(cconv);
		ctemp++;
		*ctemp = (byte)(cconv>>8);
		ctemp++;
#else
		rgb[i]  = ( RRR[0] & 0xe0 );
		rgb[i] |= ( GGG[0] >> 3 ) & 0x1c ;
//		rgb[i] |= ( BBB[0] >> 6 ) & 0x3;
		if(BBB[0]>blue_thres[3])
			rgb[i] |= 0x3;
		else if(BBB[0]>blue_thres[2])
		rgb[i] |= 0x2;
		else if(BBB[0]>blue_thres[1])
		rgb[i] |= 0x1;


		rgb[i+1]  = ( RRR[1] & 0xe0 );
		rgb[i+1] |= ( GGG[1] >> 3 ) & 0x1c ;
//		rgb[i+1] |= ( BBB[1] >> 6 ) & 0x3;
		if(BBB[1]>blue_thres[3])
			rgb[i+1] |= 0x3;
		else if(BBB[1]>blue_thres[2])
		rgb[i+1] |= 0x2;
		else if(BBB[1]>blue_thres[1])
		rgb[i+1] |= 0x1;
#endif
		if((i%1000)==0)
			dog_kick();
	}
	/******** end convert YUV to RGB ********/
  }
  else
  {
	// data가 오류인 경우 
	// 이전 데이터 그냥 사용하도록 함 
/*
	for(i=0;i<6912;i++)
		rgb[i] = 0xff;
*/
	#if MODEL_ID==MODEL_B2	// by jnlee 01.09.07
	  {
		unsigned long cnt;
		// cam working하기 위하여 audio_en전원 사용 
		INTLOCK();
		fAudioPwrOff &= ~AUDIO_EN_BY_CAM_WRK;
		AUDIO_EN_OFF();
		INTFREE();

		for(cnt=0;cnt<test1;cnt++)
			dog_kick();

		// cam working하기 위하여 audio_en전원 사용 
		INTLOCK();
		AUDIO_EN_ON();
		fAudioPwrOff |= AUDIO_EN_BY_CAM_WRK;
		INTFREE();

  		for(cnt=0;cnt<test2;cnt++)
			dog_kick();
	  }
	#endif

  }
	return ( rgb );
}

void ColorDensityPrint(void)
{
	char str[]={0x30,0x30,' ',0x30,0x30,' ',0x30,0x30,' ',0x30,0x30,' ',0x30,0x30,0};
	str[0]+=Rmul/10;
	str[1]+=Rmul%10;

	str[3]+=RColorDensity/10;
	str[4]+=RColorDensity%10;

	str[6]+=GColorDensity/10;
	str[7]+=GColorDensity%10;

	str[9]+=BColorDensity/10;
	str[10]+=BColorDensity%10;

	str[12]+=RAdd/10;
	str[13]+=RAdd%10;

	EQS_TextOutStr(10,100,(CONST BYTE*)str);
}

/*---------------------------------------------------------------------------
FUNCTION EQA_CameraTestMenuTop

DESCRIPTION
	camera test menu
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

---------------------------------------------------------------------------*/
void EQA_CameraTestMenuTop(EQS_EVENT *pInEvent)
{
    word i;
	byte *rgbdata,dflag=0;
	word max_count;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
		InitCamera();
        EQS_GetDC(TRUE);
		break;

	case EQE_APP_DRAW:
        EQS_ClearScreen();
		EQS_TextOutStr(10,10,(CONST BYTE*)"Start:Center ");
		EQS_TextOutStr(10,25,(CONST BYTE*)"LCD 밝기:좌우키");
		EQS_TextOutStr(10,40,(CONST BYTE*)"Y: *,# ");
		EQS_TextOutStr(10,55,(CONST BYTE*)"R: 4,7 ");
		EQS_TextOutStr(10,70,(CONST BYTE*)"G: 5,8 ");
		EQS_TextOutStr(10,85,(CONST BYTE*)"B: 6.9 ");
		EQS_TextOutStr(10,100,(CONST BYTE*)"역상: 0 ");
		SKY_UpdateScreen();
		break;

	case EQE_APP_END:
		// low
		StopCamera();
        EQS_ReleaseDC();
		UI_ENABLE_SLEEP();
        break;

	case EQE_KEY_CENTER:
        EQS_ClearScreen();
        EQS_StartTimer(EQS_TIMER_1, 10, FALSE);
		break;

	case EQE_TM_TIMER1:
		UI_DISABLE_SLEEP();
		BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_HI_V);
	
		rgbdata = CameraCapturedRGB();

		if( (YUV[8] == 0x60) && (YUV[10] == 0x48))
			max_count = 6912;
		else if((YUV[8] == 0x78) && (YUV[10] == 0x5a))
			max_count = 10800;
		else
			max_count = 0;

		{
			extern void fill_lcd(byte*);
			//extern byte abMainScreenBuffer[];
			word j,tind,cind;
			word *dest,*sour;
#if 1
			dest = (word *)GetScreenBuffer();//abMainScreenBuffer;
			sour = (word *)rgb;

			if(bReverse)
			{

				for(i=0;i<90;i++)
				{
					for(j=0;j<120;j++)
						*(dest++)=*(sour++);
					for(;j<128;j++)
						*(dest++)=0;
				}
				for(i=0;i<3;i++)
				{
					for(j=0;j<128;j++)
						*(dest++)=0;
				}
			}
			else
			{
				sour += 120*90-1;

				for(i=0;i<90;i++)
				{
					for(j=0;j<120;j++)
						*(dest++)=*(sour--);
					for(;j<128;j++)
						*(dest++)=0;
				}
				for(i=0;i<3;i++)
				{
					for(j=0;j<128;j++)
						*(dest++)=0;
				}
			}
#else
			for(i=0;i<90;i++)
			{
				for(j=0;j<120*2;j++)
					tempp[tind++]=rgb[cind++];
				for(;j<128*2;j++)
					tempp[tind++]=0;
			}
			fill_lcd(tempp);
#endif
		}
#if 1
		ColorDensityPrint();
		SKY_UpdateScreen();
#endif

		/**********  display end  **********/
        EQS_StartTimer(EQS_TIMER_1, 50, FALSE);
		break;


	case EQE_KEY_3:	
//		YColorAdd+=10;		// 색감 높이기 
		RAdd+=10;		// 색감 높이기 
		GAdd+=10;		// 색감 높이기 
		BAdd+=10;		// 색감 높이기 
		break;

	case EQE_KEY_1:	
/*
		if(YColorAdd >= 10)
			YColorAdd-=10;		// 색감 줄이기 
*/
		if(RAdd >= 10)
		{
			RAdd -= 10;
			GAdd -= 10;
			BAdd -= 10;
		}
		break;

	case EQE_KEY_4:	
		RColorDensity++;		// 색감 높이기 
		break;

	case EQE_KEY_5:	// 
		GColorDensity++;		// 색감 높이기 
		break;

	case EQE_KEY_6:	// 
		BColorDensity++;		// 색감 높이기 
		break;

	case EQE_KEY_7:	
		if(RColorDensity > 0)
			RColorDensity--;		// 색감 줄이기 
		break;

	case EQE_KEY_8:	// 
		if(GColorDensity > 0)
			GColorDensity--;		// 색감 줄이기 
		break;

	case EQE_KEY_9:	// 
		if(BColorDensity > 0)
			BColorDensity--;		// 색감 줄이기 
		break;

	case EQE_KEY_POUND:	// 
//		YColorDensity++;		// 색감 높이기 
		Rmul++;
		Gmul++;
		Bmul++;
		break;

	case EQE_KEY_STAR:	
/*		if(YColorDensity > 0)
			YColorDensity--;		// 색감 줄이기 
*/
		if(Rmul > 0)
		{
			Rmul--;
			Gmul--;
			Bmul--;
		}

		break;

	case EQE_KEY_0:		//  
		bReverse ^= 1;
		break;

	case EQE_KEY_LEFT:
		SKY_SetLCDContrastTest (1);	// 어둡게 
        break;

	case EQE_KEY_RIGHT:
		SKY_SetLCDContrastTest (0x21);	// 밝게 
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}

}


void SKY_RecoverPhotoList(void){

		HFILE   hFile,nFile;
		UINT8   index,s_wIdx;
		BYTE			 psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH+1]; 
		PHOTO_LIST_T	 psPhotoList[MAX_PHOTO_COUNT];
		BYTE			 *ppPhotoList[MAX_PHOTO_COUNT];
		BYTE			 *ppPhotoName[MAX_PHOTO_COUNT];
   
		memset(psPhotoList, NULL, sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);

		if(!SKY_IsFileOrDirExist(FILE_NAME_CAMERA))
		{   
			      
			// Create file
			hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_CREATE);
			       
			
		}
		else
		{
			hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
			
			if((s_wIdx = SKY_GetFileNumberInDataFile(SDF_PHOTO)) != 0)
			{
				for(index=0; index < s_wIdx ; index ++)
				{
					ppPhotoName[index] = (BYTE*)SKY_Malloc(SKY_FILE_EXT_NAME_MAX_LENGTH+1);
					ppPhotoList[index] = (BYTE*)SKY_Malloc(SKY_FILE_EXT_NAME_MAX_LENGTH+1);
				}

				SKY_GetFileList(SKY_PHOTO_DIR, ppPhotoList);

				for(index=0; index < s_wIdx ; index ++)
				{
					STRCPY(psFileName, ppPhotoList[index]);
					SKY_OpenDataFile(SDF_PHOTO, psFileName, &nFile, SFT_READ);
					SKY_ReadDataBlock(nFile, psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
					SKY_CloseDataFile(nFile);
					STRCPY(ppPhotoName[index], psFileName);
					
					psPhotoList[index].PhotoList = atol(ppPhotoList[index]);
					STRCPY(psPhotoList[index].PhotoName ,ppPhotoName[index]);
				}
			}
		}

		SKY_WriteFile(hFile, (CONST void *)psPhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                
		SKY_CloseFile(hFile);
	        
}

void SKA_RecoverPhotoList(SKY_EVENT_T *pEvent){

    switch(pEvent->EventID)
    {
    
	case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    CL_NONE,
					SKY_GET_TOKEN(TKN_CAMERA_TEST_RECOVER_PHOTOLIST), //사진리스트 복구
					WA_SHOW_HEADER | WA_SHOW_CML);
    
		EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_CAMERA_TEST_RECOVERING_PHOTOLIST), MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
		
		break;
	

	case SE_APP_END :   /* The very last event */ 
		break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME:
		break;

	case SE_IO_MESGBOX_END:

		SKY_RecoverPhotoList();
		EQS_END();

		break;

	default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    
	}
       
}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
