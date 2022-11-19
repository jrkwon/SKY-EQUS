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
#include "hsig.h"
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
extern VOID EQA_VersionTop(EQS_EVENT *pInEvent);
extern VOID EQA_MelodyTop(EQS_EVENT *pInEvent);
extern VOID EQA_VolumeTop(EQS_EVENT *pInEvent);
extern VOID EQA_AudioTop(EQS_EVENT *pInEvent); 
extern VOID EQA_McTop(EQS_EVENT *pInEvent);
extern VOID EQA_VolumeUpDTop(EQS_EVENT *pInEvent);
// tasik 00.08.01
#if (MODEL_ID != MODEL_B1) && (MODEL_ID != MODEL_B2)
extern VOID EQA_BatteryTop(EQS_EVENT *pInEvent);
#endif // #if (MODEL_ID != MODEL_B1) 
//extern VOID EQA_IrDATop(EQS_EVENT *pInEvent);
extern VOID EQA_RSSITop(EQS_EVENT *pInEvent);
extern VOID EQA_LEDTop(EQS_EVENT *pInEvent);
extern VOID EQA_RingTop(EQS_EVENT *pInEvent);
extern VOID EQA_BLightTop(EQS_EVENT *pInEvent);
// tasik 00.08.01
#if (MODEL_ID != MODEL_B1) && (MODEL_ID != MODEL_B2)
extern VOID EQA_LcdContrastTop(EQS_EVENT *pInEvent);
#endif //#if (MODEL_ID != MODEL_B1)
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
void EQA_LcdContrastTop(EQS_EVENT *pInEvent);
void EQA_MelodyTop(EQS_EVENT *pInEvent);
void EQA_VolumeTop(EQS_EVENT *pInEvent);
void EQA_BatteryTop(EQS_EVENT *pInEvent);
void EQA_IrDATop(EQS_EVENT *pInEvent);
void EQA_RSSITop(EQS_EVENT *pInEvent);
void EQA_LEDTop(EQS_EVENT *pInEvent);
void EQA_RingTop(EQS_EVENT *pInEvent);
void EQA_BLightTop(EQS_EVENT *pInEvent);
void EQA_VersionTop(EQS_EVENT *pInEvent);


CONST CHAR* CONST TestMenu[] = {
					SF_BLACK_NUM1"화면밝기조정",
					SF_BLACK_NUM2"Audio Gain",
					SF_BLACK_NUM3"음량 및 환경설정",
					SF_BLACK_NUM4"Smart Battery",
					SF_BLACK_NUM5"적외선통신",
					SF_BLACK_NUM6"RSSI Display",
					SF_BLACK_NUM7"LED시험",
					SF_BLACK_NUM8"Ring Indicator",
					SF_BLACK_NUM9"Backlight",
					SF_BLACK_STAR"착신벨/진동",
					SF_BLACK_POUND"S/W 버전"
};

EQS_Applet TestApplet[] = {	EQA_LcdContrastTop,//1
							EQA_VolumeTop, //2
							EQA_VolumeUpDTop,//3
							EQA_BatteryTop, //4
							EQA_IrDATop,//5
							EQA_RSSITop,//6
							EQA_LEDTop,//7
							EQA_RingTop,//8
							EQA_BLightTop,//9
							EQA_MelodyTop, //*
							EQA_VersionTop//#
};




/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

void EQS_API sDraw4GImage(void);
void EQS_API sDraw256ImageRoses(void);
void EQS_API sDraw256ImageRock(void);
void EQS_API sDraw256ImageT(void);
void EQS_API sDraw256ImageT1(void);
void EQS_API sDraw256ImageT2(void);

BYTE TestI[120*120];
BITMAP TestImage = {
    { { 120, 120 }, 120, 8 }, TestI, FALSE
}; 

void EQS_API ImageColor(byte color)
{
	int i;
	for (i=0; i<(120*120);i++)
		TestI[i] = color;

    EQS_GetDC(TRUE);
    {
        EQS_PutBitmap(0, 0, &TestImage);
    }
    EQS_ReleaseDC();
}

void EQS_API redred(void)
{
	ImageColor(0xe0);
}

void EQS_API greengreen(void)
{
	ImageColor(0x1c);
}
void EQS_API blueblue(void)
{
	ImageColor(0x3);
}
void EQS_API whitewhite(void)
{
	ImageColor(0xff);
}

void EQS_API c4color(void)
{
	int i;

	for (i=0; i<(120*120/4);i++)
		TestI[i] = 0xe0;
	for (; i<(120*120/2);i++)
		TestI[i] = 0x1c;
	for (; i<(120*120/4*3);i++)
		TestI[i] = 0x3;
	for (; i<(120*120);i++)
		TestI[i] = 0xff;

    EQS_GetDC(TRUE);
    {
        EQS_PutBitmap(0, 0, &TestImage);
    }
    EQS_ReleaseDC();
}

byte csel;

void EQA_LcdContrastTop(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
    void EQS_API (*pfnDraw[11])(void) = {
        sDraw256ImageRoses,
        sDraw256ImageRock,
        sDraw4GImage,
        sDraw256ImageT,
        sDraw256ImageT1,
        sDraw256ImageT2,
		redred,
		greengreen,
		blueblue,
		whitewhite,
		c4color

    };
    STATIC INT8 i = 0;
	UINT8	iKey;
	int idcont[10] = {0,7,14,21,28,35,42,49,56,63};
	

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
		csel = 8;
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0xff);			// id lcd select
		hs_idlcd_contrast(idcont[csel]);
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0);
        EQS_GetDC(TRUE);
		//hHnd = EQC_StaticText("LCD TEST", 0, 0, FT_MED, 0);
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_UP:
	case EQE_KEY_1:
		/* */

		hs_lcd_contrast (0x21);

		if(csel==9)
			;
		else
			csel++;
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0xff);			// id lcd select
		hs_idlcd_contrast(idcont[csel]);
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0);
		break;

	case EQE_KEY_DOWN:
	case EQE_KEY_2:
		/* */

		hs_lcd_contrast (1);

		if(csel==0)
			;
		else
			csel--;
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0xff);			// id lcd select
		hs_idlcd_contrast(idcont[csel]);
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0);
		break;

	case EQE_KEY_4:
		HS_BACKLIGHT_ON();
		break;
	case EQE_KEY_5:
		HS_BACKLIGHT_OFF();
		break;

	case EQE_KEY_6:
		BIO_OUT( ENC_GPIO_1, BIO_LED_BLUE_M,  BIO_LED_BLUE_LO_V); 
		BIO_OUT( ENC_GPIO_1, BIO_LED_RED_M,   BIO_LED_RED_LO_V); 
		BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V); 
		break;

	case EQE_KEY_7:
		BIO_OUT( ENC_GPIO_1, BIO_LED_BLUE_M,  BIO_LED_BLUE_HI_V); 
		break;

	case EQE_KEY_8:
		BIO_OUT( ENC_GPIO_1, BIO_LED_RED_M,   BIO_LED_RED_HI_V); 
		break;

	case EQE_KEY_9:
		BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_HI_V); 
		break;

	case EQE_KEY_F1:
        if(i > 0)
            i--;
		else
			i = 10;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	case EQE_KEY_F2:
        if(i < 10)
            i++;
		else
			i = 0;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}

}

void EQA_MelodyTop(EQS_EVENT *pInEvent)
{
	EQS_End();
}

void EQA_VolumeTop(EQS_EVENT *pInEvent)
{
	EQS_End();
}

void EQA_VolumeUpDTop(EQS_EVENT *pInEvent)
{
	EQS_End();
}

void EQA_BatteryTop(EQS_EVENT *pInEvent)
{
	EQS_End();
}

void EQA_IrDATop(EQS_EVENT *pInEvent)
{
	EQS_End();
}

void EQA_RSSITop(EQS_EVENT *pInEvent)
{
	EQS_End();
}


byte YUV[14000];	//header 32byte data 6912*2 end2byte = 13858
byte rgb[6912];
byte bw[6912];

void EQA_LEDTop(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
    word i,j,k,l,m,n;
	dword dw;
	UINT8	iKey;
	long RRR[2],GGG[2],BBB[2],YY[2],UU,VV;
	byte header[]={0xaa,0x55,0xff,0xd8,0x1c,0xf0,0xf1,0x00,0x60,0x00,
		           0x48,0x81,0x31,0x20,0x4b,0x43,0x23,0x38,0x42,0x50,
				   0x2e,0x2e,0,   0,   0,   0,   0,   0,   0,   0,
				   0xaa,0x55};
	byte endmark[] = { 0xff, 0xd9};
	byte Rd;
	word read_max = 13860;
	byte gpio_save, gpio_hi, gpio_lo;

	BITMAP CamImage = {
    { { 96, 72 }, 96, 8 }, NULL, FALSE
	}; 

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
        EQS_ClearScreen();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:

	for(n=0;n<100;n++)
	{
		/* capture start*/
		INTLOCK();
		read_max = 13858;
		k = 10;
 
		gpio_save = ENC_GPIO_0_BUF;
		gpio_hi = gpio_save | 0x80;
		gpio_lo = gpio_save & 0x7f;

		outp(ENC_GPIO_0_OUT, gpio_lo);
		for(i=0	; i<(read_max-1) ; i++)
		{
			if((i%5000) == 0) dog_kick();
			for(j=0;j<k;j++);
			outp(ENC_GPIO_0_OUT, gpio_hi);
			YUV[i] = *(byte *)0x2000000;
			outp(ENC_GPIO_0_OUT, gpio_lo);
		}
		if((i%5000) == 0) dog_kick();
		for(j=0;j<k;j++);
		outp(ENC_GPIO_0_OUT, gpio_hi);
		YUV[i] = *(byte *)0x2000000;

		ENC_GPIO_0_BUF = gpio_hi;
		INTFREE();

		dog_kick();
		EQS_TextOutStr(0,0,(BYTE*)"Read OK!");

		j=32;
		for (i=0; i<6912; i+=2)
		{
			UU    = YUV[j++]-128;
			YY[0] = YUV[j++]-16;
			VV    = YUV[j++]-128;
			YY[1] = YUV[j++]-16;

//			RRR[0] = 139 * VV / 100; 
			RRR[0] = (178 * VV) >> 7; 
			RRR[1] = RRR[0] + YY[1];
			RRR[0] = RRR[0] + YY[0];
				
//			GGG[0] = (33 * UU + 74 * VV)/100;
			GGG[0] = (21 * UU + 48 * VV) >> 6;
			GGG[1] = YY[1] - GGG[0] ;
			GGG[0] = YY[0] - GGG[0] ;

//			BBB[0] = (176 * UU) /100;
			BBB[0] = (7 * UU) >> 2;
			BBB[1] = BBB[0] + YY[1];
			BBB[0] = BBB[0] + YY[0];

			if(RRR[0]<0) RRR[0] = 0;				
			if(RRR[1]<0) RRR[1] = 0;				
			if(GGG[0]<0) GGG[0] = 0;				
			if(GGG[1]<0) GGG[1] = 0;				
			if(BBB[0]<0) BBB[0] = 0;				
			if(BBB[1]<0) BBB[1] = 0;				

			rgb[i]  = ( RRR[0] & 0xe0 );
			rgb[i] |= ( GGG[0] >> 3 ) & 0x1c ;
			rgb[i] |= ( BBB[0] >> 6 ) & 0x3;

			rgb[i+1]  = ( RRR[1] & 0xe0 );
			rgb[i+1] |= ( GGG[1] >> 3 ) & 0x1c ;
			rgb[i+1] |= ( BBB[1] >> 6 ) & 0x3;
			if((i%1000)==0)
				dog_kick();
		}
	/*****************************************************/
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0); //color LCD select

		hs_lcd_cmd(0x75);		// PASET
		hs_lcd_data(0x00);	// start page
		hs_lcd_data(71);	// end page

		hs_lcd_cmd(0x15);		// CASET
		hs_lcd_data(0x00);	// start column address
		hs_lcd_data(95);	// end col addr

		hs_lcd_cmd(0x5c);	
		for(i=0;i<(96*72);i++)
		{
			hs_lcd_data (rgb[i]);	
		}
		hs_lcd_cmd(0xAF);		// DISPON 표시부 on 
	}
/*****************************************************/
		break;
	case EQE_KEY_2:
		/* */
        EQS_ClearScreen();
		BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0); //color LCD select
		hs_lcd_cmd(0x75);		// PASET
		hs_lcd_data(0x00);	// start page
		hs_lcd_data(119);	// end page

		hs_lcd_cmd(0x15);		// CASET
		hs_lcd_data(0x00);	// start column address
		hs_lcd_data(119);	// end col addr

		hs_lcd_cmd(0x5c);	

		k=0;
		for(i=0;i<60;i++)
		{
			for(j=0;j<60 ;j++)
			{
				hs_lcd_data (rgb[k]);	
				hs_lcd_data (rgb[k]);	
				k++;
			}
			k-=60;
			for(j=0;j<60 ;j++)
			{
				hs_lcd_data (rgb[k]);	
				hs_lcd_data (rgb[k]);	
				k++;
			}
			k+=36;
		}
		hs_lcd_cmd(0xAF);		// DISPON 표시부 on 
		break;


/******************************************************/

	case EQE_KEY_3:	// GPIO READ!!!	// COLOR
		/* capture start*/
	{
		byte Sft;
		word lp ;
/*
		byte header[]={0xaa,0x55,0xff,0xd8,0x1c,0xf0,0xf1,0x00,0x60,0x00,
			           0x48,0x81,0x31,0x20,0x4b,0x43,0x23,0x38,0x42,0x50,
					   0x2e,0x2e,0,   0,   0,   0,   0,   0,   0,   0,
					   0xaa,0x55};
		byte endmark[] = { 0xff, 0xd9};
*/
		BIO_TRISTATE ( ENC_GPIO_0, 0x80, 0x80);	// CLK
		
		for ( lp = 0 ; lp < 100 ; lp++)
		{
			INTLOCK();
			gpio_save = ENC_GPIO_0_BUF;
			gpio_hi = gpio_save | 0x80;
			gpio_lo = gpio_save & 0x7f;

			read_max = 13858;
			for (i = 0 ; i<read_max ; i++)
			{
				for (j = 0 ; j<8 ;j++)
				{
					Sft<<=1;
					outp(ENC_GPIO_0_OUT, gpio_hi);

					if (inp(DMOD_GPIO_INT_IN)&2)
						Sft|=1;

					outp(ENC_GPIO_0_OUT, gpio_lo);
				}
				YUV[i] = Sft;
				if((i%100)==0)
					dog_kick();
			}

			outp(ENC_GPIO_0_OUT, gpio_save);
			INTFREE();


			EQS_TextOutStr(0,0,(BYTE*)"Read OK!");

			j=32;
			for (i=0; i<6912; i+=2)
			{
				/*
				r = 104635 * v;
				g = -25690 * u + -53294 * v;
				b = 132278 * u;
				y*=76310; y1*=76310;

				r = (139 * v )/100;
				g = (-33 * u + -74 * v)/100;
				b = (176 * u)/100;
				*/
#if FALSE
				UU    = YUV[j++]-128;
				if(UU<0) UU = 0;
				YY[0] = YUV[j++]-16;
				if(YY[0]<0) YY[0] = 0;
				VV    = YUV[j++]-128;
				if(VV<0) VV = 0;
				YY[1] = YUV[j++]-16;
				if(YY[1]<0) YY[1] = 0;

#define limit(x) (((x>0xffffff)?0xff0000:((x<=0xffff)?0:x&0xff0000))>>16)

				YY[0] *= 76310;
				YY[1] *= 76310;

				RRR[0] = 104635 * VV;
				RRR[1] = RRR[0] + YY[1];
				RRR[0] = RRR[0] + YY[0];
				
				GGG[0] = 25690 * UU + 53294 * VV;
				GGG[1] = YY[1] - GGG[0] ;
				GGG[0] = YY[0] - GGG[0] ;

				BBB[0] = 132278 * UU;
				BBB[1] = BBB[0] + YY[1];
				BBB[0] = BBB[0] + YY[0];

				RRR[0] = limit(RRR[0]);
				RRR[1] = limit(RRR[1]);
				GGG[0] = limit(GGG[0]);
				GGG[1] = limit(GGG[1]);
				BBB[0] = limit(BBB[0]);
				BBB[1] = limit(BBB[1]);
#else
				UU    = YUV[j++]-128;
				YY[0] = YUV[j++]-16;
//				if(YY[0]<0) YY[0] = 0;
				VV    = YUV[j++]-128;
				YY[1] = YUV[j++]-16;
//				if(YY[1]<0) YY[1] = 0;

//				RRR[0] = 139 * VV / 100; 
				RRR[0] = (178 * VV) >> 7; 
				RRR[1] = RRR[0] + YY[1];
				RRR[0] = RRR[0] + YY[0];
				
//				GGG[0] = (33 * UU + 74 * VV)/100;
				GGG[0] = (21 * UU + 48 * VV) >> 6;
				GGG[1] = YY[1] - GGG[0] ;
				GGG[0] = YY[0] - GGG[0] ;

//				BBB[0] = (176 * UU) /100;
				BBB[0] = (7 * UU) >> 2;
				BBB[1] = BBB[0] + YY[1];
				BBB[0] = BBB[0] + YY[0];

				if(RRR[0]<0) RRR[0] = 0;				
				if(RRR[1]<0) RRR[1] = 0;				
				if(GGG[0]<0) GGG[0] = 0;				
				if(GGG[1]<0) GGG[1] = 0;				
				if(BBB[0]<0) BBB[0] = 0;				
				if(BBB[1]<0) BBB[1] = 0;				

#endif
				rgb[i]  = ( RRR[0] & 0xe0 );
				rgb[i] |= ( GGG[0] >> 3 ) & 0x1c ;
				rgb[i] |= ( BBB[0] >> 6 ) & 0x3;

				rgb[i+1]  = ( RRR[1] & 0xe0 );
				rgb[i+1] |= ( GGG[1] >> 3 ) & 0x1c ;
				rgb[i+1] |= ( BBB[1] >> 6 ) & 0x3;


				if((i%100)==0)
					dog_kick();

			}

			EQS_TextOutStr(30,30,(BYTE*)"conversion OK!");
			EQS_UpdateDisplay();

	/*****************************************************/
			BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0); //color LCD select

			hs_lcd_cmd(0x75);		// PASET
			hs_lcd_data(0x00);	// start page
			hs_lcd_data(71);	// end page

			hs_lcd_cmd(0x15);		// CASET
			hs_lcd_data(0x00);	// start column address
			hs_lcd_data(95);	// end col addr

			hs_lcd_cmd(0x5c);	
			for(i=0;i<(96*72);i++)
			{
				hs_lcd_data (rgb[i]);	
			}
			hs_lcd_cmd(0xAF);		// DISPON 표시부 on 
		}
	}	
	break;


/*****************************************************/

	case EQE_KEY_4:	// GPIO READ!!!	MONO!!
		/* capture start*/
	{
		byte Sft,gpio_save, gpio_hi, gpio_lo;
		word lp;
/*
		byte header[]={0xaa,0x55,0xff,0xd8,0x1c,0xf0,0xf1,0x00,0x60,0x00,
			           0x48,0x81,0x31,0x20,0x4b,0x43,0x23,0x38,0x42,0x50,
					   0x2e,0x2e,0,   0,   0,   0,   0,   0,   0,   0,
					   0xaa,0x55};
		byte endmark[] = { 0xff, 0xd9};
*/
		BIO_TRISTATE ( ENC_GPIO_0, 0x80, 0x80);	// CLK
		
		for (lp = 0 ; lp<100 ; lp ++)
		{

			INTLOCK();
			gpio_save = ENC_GPIO_0_BUF;
			gpio_hi = gpio_save | 0x80;
			gpio_lo = gpio_save & 0x7f;

			read_max = 13858;
			for (i = 0 ; i<read_max ; i++)
			{
				for (j = 0 ; j<8 ;j++)
				{
					Sft<<=1;
					outp(ENC_GPIO_0_OUT, gpio_hi);

					if (inp(DMOD_GPIO_INT_IN)&2)
						Sft|=1;

					outp(ENC_GPIO_0_OUT, gpio_lo);
				}
				YUV[i] = Sft;
				if((i%100)==0)
					dog_kick();
			}

			outp(ENC_GPIO_0_OUT, gpio_save);
			INTFREE();


			EQS_TextOutStr(0,0,(BYTE*)"Read OK!");

			j=32;
			for (i=0; i<6912; i+=2)
			{
				j++;
				YY[0] = YUV[j++]-16;
				j++;
				YY[1] = YUV[j++]-16;
				bw[i] = (YY[0]>>3 & 0x1c) | 0xe3;
				bw[i+1] = (YY[1]>>3 & 0x1c) | 0xe3;
				if((i%100)==0)
					dog_kick();
			}
			EQS_TextOutStr(30,30,(BYTE*)"conversion OK!");
			EQS_UpdateDisplay();

			BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0); //color LCD select
			hs_lcd_cmd(0x75);		// PASET
			hs_lcd_data(0x00);	// start page
			hs_lcd_data(71);	// end page

			hs_lcd_cmd(0x15);		// CASET
			hs_lcd_data(0x00);	// start column address
			hs_lcd_data(95);	// end col addr

			hs_lcd_cmd(0x5c);	
			for(i=0;i<(96*72);i++)
			{
				hs_lcd_data (bw[i]);	
			}
			hs_lcd_cmd(0xAF);		// DISPON 표시부 on 
		}
	}
/*****************************************************/
	break;

	case EQE_KEY_5:		// 1번 읽기 
		INTLOCK();
		k = 0;
		l = 100;
		m = 100;
		n = 100;
		BIO_OUT(ENC_GPIO_0, 0x80, 0);		//gpio13 low
		for(j=0;j<k;j++)	if((j%1000) == 0)	dog_kick();
		YUV[i] = *(byte *)0x2000000;
		for(j=0;j<l;j++)	if((j%1000) == 0)	dog_kick();
		BIO_OUT(ENC_GPIO_0, 0x80, 0x80);		//gpio13 low
		for(j=0;j<m;j++)	if((j%1000) == 0)	dog_kick();
		dog_kick();
		INTFREE();
		break;

	case EQE_KEY_6:		// 10번 읽기 
		INTLOCK();
		k = 0;
		l = 100;
		m = 100;
		n = 100;
		for(i=0	; i<10 ; i++)
		{
			BIO_OUT(ENC_GPIO_0, 0x80, 0);		//gpio13 low
			for(j=0;j<k;j++)	if((j%1000) == 0)	dog_kick();
			YUV[i] = *(byte *)0x2000000;
			for(j=0;j<l;j++)	if((j%1000) == 0)	dog_kick();
			BIO_OUT(ENC_GPIO_0, 0x80, 0x80);		//gpio13 low
			for(j=0;j<m;j++)	if((j%1000) == 0)	dog_kick();
			dog_kick();
		}

		INTFREE();
		break;

	case EQE_KEY_7:		// 20번 읽기 
		INTLOCK();
		k = 0;
		l = 100;
		m = 100;
		n = 100;
		for(i=0	; i<20 ; i++)
		{
			BIO_OUT(ENC_GPIO_0, 0x80, 0);		//gpio13 low
			for(j=0;j<k;j++)	if((j%1000) == 0)	dog_kick();
			YUV[i] = *(byte *)0x2000000;
			for(j=0;j<l;j++)	if((j%1000) == 0)	dog_kick();
			BIO_OUT(ENC_GPIO_0, 0x80, 0x80);		//gpio13 low
			for(j=0;j<m;j++)	if((j%1000) == 0)	dog_kick();
			dog_kick();
		}
		INTFREE();
		break;

	case EQE_KEY_F1:
        EQS_ClearScreen();
//        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	case EQE_KEY_F2:
        EQS_ClearScreen();
//        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}

}

void EQA_RingTop(EQS_EVENT *pInEvent)
{
	EQS_End();
}

extern byte hs_light_sense_value;
void EQA_BLightTop(EQS_EVENT *pInEvent)
{
	//static HCONTROL hHnd, hStatic2;
	UINT8	iKey;
	byte	sense[4]={0x30,0x30,0x30,0};
	word	ts;
	int i;


	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		//hHnd = EQC_StaticText("LIGHT SENSE TEST", 0, 0, FT_MED, 0);
		//hStatic2 = EQC_StaticText((char *)sense, 50, 50, FT_MED, 0);
        EQS_ClearScreen();
        EQS_UpdateDisplay();
		EQS_StartTimer(EQS_TIMER_1, 300, TRUE);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
		EQS_StopTimer(EQS_TIMER_1);
        break;

	case EQE_KEY_UP:
	case EQE_KEY_1:
	case EQE_TM_TIMER1:
		hs_light_sense_value = adc_read (ADC_LIGHT_SENSE);
		ts = hs_light_sense_value;
		ts =  (ts * 200 /256)+ 50;
		
		sense[0] = ((byte)ts)/100;
		sense[1] = (((byte)ts) %100 )/10;
		sense[2] = ((byte)ts)%10;

		for(i=0;i<3;i++)
		{
			if(sense[i] >= 10)
				sense[i]+=(0x40-9);
			else
				sense[i]+=0x30;
		}
		//EQC_SetSText(hStatic2, (char *)sense);
		EQS_TextOutStr(0,0,(BYTE*)"LIGHT SENSE TEST");
		EQS_TextOutStrLen(50,50,(BYTE*)sense,3);
		//hStatic2 = EQC_StaticText((char *)sense, 50, 50, FT_MED, 0);
        EQS_UpdateDisplay();
		/* */
		break;

	case EQE_KEY_DOWN:
	case EQE_KEY_2:
		/* */
		break;
	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

void EQA_VersionTop(EQS_EVENT *pInEvent)
{
	EQS_End();
}


VOID EQA_TestMenuTop(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	UINT8	iKey;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

		hHnd = EQC_ScrollList(TestMenu, 11, 11, 0, 0, 16, 5, 0, SCROLL_DEFAULTS);
//		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

    case EQE_APP_RESUME:
        EQS_SetIOCActive(hHnd);
		break;

	case EQE_KEY_1:
	case EQE_KEY_2:
	case EQE_KEY_3:
	case EQE_KEY_4:
	case EQE_KEY_5:
	case EQE_KEY_6:
	case EQE_KEY_7:
	case EQE_KEY_8:
	case EQE_KEY_9:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollIndex(hHnd, iKey);

	case EQE_KEY_CENTER:
		EQS_StartChild(TestApplet[EQC_GetScrollIndex(hHnd)]);
		break;

	case EQE_KEY_STAR:
		EQS_StartChild(TestApplet[9]);
		break;

	case EQE_KEY_POUND:
		EQS_StartChild(TestApplet[10]);
		break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}

}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
