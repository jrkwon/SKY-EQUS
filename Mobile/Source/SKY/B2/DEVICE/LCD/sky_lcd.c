/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     sky_lcd.c
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
** 2001-04-21  asdf		 Modify it fit for B2
** 2000.08.01  tasik     make it fit for B1
** 2000-07-18 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <string.h>
#include "target.h"
//#include "LCD_B1.h"
#include "clk.h" 
#include "eqs.h"
//#include "eqsqlcd_B1.h"

#include "sky.h"
#include "boothw.h"

/* 
**----------------------------------------------------------------------------
**  Definitions
**---------------------------------------------------------------------------- 
*/
#ifdef FEATURE_WAVELET // yoursun 01/08/23
#define LOBYTE(w)   ((BYTE) (w)) 
#define HIBYTE(w)   ((BYTE) (((WORD) (w) >> 8) & 0xFF)) 
#endif //FEATURE_WAVELET
/* 
**----------------------------------------------------------------------------
**  B1 Definitions
**---------------------------------------------------------------------------- 
*/
#ifdef LCD_CONVERT_TEMPORARY

#define NIBBLE_SIZE		4
#define BYTE_SIZE		8
#define UINT64_SIZE		64

#endif // #ifdef LCD_CONVERT_TEMPORARY

// by jnlee 00.12.20
// 아래의 define은 삭제 되었음. 최종은 이 define이 사용된 라인 삭제 요망됨 
#ifndef BIO_LCD_SEL_M		// 현재 biog에서 제외됐음
#define BIO_LCD_SEL_M 0
#endif	
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

/*
**---------------------------------------------------------------------------
**  Function Prototype Declarations
**---------------------------------------------------------------------------
*/
#if MODEL_ID == MODEL_B1 || (MODEL_ID == MODEL_B2)
LOCAL void sSetLCDAddress(BYTE bStartColumn, BYTE bEndColumn, 
                          BYTE bStartPage, BYTE bEndPage);

#ifdef OLD_LCD_ACCESS // 속도 개선 이전 
LOCAL void sLCDCommand(BYTE bLCDCommand);
LOCAL void sLCDData(BYTE bLCDData);
LOCAL void sLCDCommand(BYTE bLCDCommand)
{
	LCD_OUTP_CMD(bLCDCommand);
}

LOCAL void sLCDData(BYTE bLCDData)
{
	LCD_OUTP_DATA(bLCDData);
}
#else

//속도 개선을 위하여 함수를 define으로 변경 by jnlee 01.02.21 
#ifndef WIN32
#define sLCDCommand(bLCDCommand)	LCD_OUTP_CMD(bLCDCommand)
#define sLCDData(bLCDData)			LCD_OUTP_DATA(bLCDData)
#else
#define sLCDCommand(bLCDCommand)	
#define sLCDData(bLCDData)			
#endif
#endif

/* --------------------- */
/* for camera test only */
void SKY_API SKY_LCDCommand(BYTE bLCDCommand)
{
    sLCDCommand(bLCDCommand);
}

void SKY_API SKY_LCDData(BYTE bLCDData)
{
    sLCDData(bLCDData);
}
#endif //#if MODEL_ID == MODEL_B1 

/*
**--------------------------------------------------------------------------
	B2 64K COLOR LCD 
**--------------------------------------------------------------------------
*/
   
#ifdef FEATURE_64KCOLOR_LCD

LOCAL void sLCDDrg(BYTE bLCDDrg);
LOCAL void sLCDAdr(BYTE bLCDAdr);
LOCAL void sLCDCtl(BYTE bLCDCtl);
LOCAL void sLCDCnf(BYTE bLCDCnf);
LOCAL void sLCDMaster(BYTE bLCDMaster);
LOCAL void sLCDSlave(BYTE bLCDSlave);

void	aLCD_OUTP_DRG( byte Value )
{
	*(volatile word*)(LCD_DRG_ADDRESS) = (Value);
}
void	aLCD_OUTP_ADR( byte Value )
{
	*(volatile word*)(LCD_ADR_ADDRESS) = (Value);
}
void	aLCD_OUTP_CTL( byte Value )		
{
	*(volatile word*)(LCD_CTL_ADDRESS) = (Value);
}
void	aLCD_OUTP_CNF( byte Value )		
{
	*(volatile word*)(LCD_CNF_ADDRESS) = (Value);
}
void	aLCD_OUTP_MASTER( byte Value )
{
	*(volatile word*)(LCD_MASTER_ADDRESS) = (Value);
}
void	aLCD_OUTP_SLAVE( byte Value )
{
	*(volatile word*)(LCD_SLAVE_ADDRESS) = (Value);
}

#define sLCDDrg(bLCDDrg)		LCD_OUTP_DRG(bLCDDrg)
#define sLCDAdr(bLCDAdr)		LCD_OUTP_ADR(bLCDAdr)
#define sLCDCtl(bLCDCtl)		LCD_OUTP_CTL(bLCDCtl)
#define sLCDCnf(bLCDCnf)		LCD_OUTP_CNF(bLCDCnf)
#define sLCDMaster(bLCDMaster)	LCD_OUTP_MASTER(bLCDMaster)
#define sLCDSlave(bLCDSlave)	LCD_OUTP_SLAVE(bLCDSlave)
#define sLCDData1(bLCDData1)	LCD_OUTP_DATA1(bLCDData1)
#define sLCDData2(bLCDData2)	LCD_OUTP_DATA2(bLCDData2)


void SKY_API SKY_LCDOn(void)// ( void )
{
/* 임시로 막음	by jnlee 01.05.25
	sLCDMaster(POWER_SAVING_OFF);
	sLCDMaster(DISPLAY_ON);
	*/
}

void SKY_API SKY_LCDOff(void)//LCDDisplayOff ( void )
{
/* 임시로 막음	by jnlee 01.05.25
	sLCDMaster(DISPLAY_OFF);
	sLCDMaster(POWER_SAVING_ON);
	*/
}


const word cvt8b16b[]=
{
0x0000,0x0008,0x0010,0x0018,0x0100,0x0108,0x0110,0x0118,0x0200,0x0208,
0x0210,0x0218,0x0300,0x0308,0x0310,0x0318,0x0400,0x0408,0x0410,0x0418,
0x0500,0x0508,0x0510,0x0518,0x0600,0x0608,0x0610,0x0618,0x0700,0x0708,
0x0710,0x0718,0x2000,0x2008,0x2010,0x2018,0x2100,0x2108,0x2110,0x2118,
0x2200,0x2208,0x2210,0x2218,0x2300,0x2308,0x2310,0x2318,0x2400,0x2408,
0x2410,0x2418,0x2500,0x2508,0x2510,0x2518,0x2600,0x2608,0x2610,0x2618,
0x2700,0x2708,0x2710,0x2718,0x4000,0x4008,0x4010,0x4018,0x4100,0x4108,
0x4110,0x4118,0x4200,0x4208,0x4210,0x4218,0x4300,0x4308,0x4310,0x4318,
0x4400,0x4408,0x4410,0x4418,0x4500,0x4508,0x4510,0x4518,0x4600,0x4608,
0x4610,0x4618,0x4700,0x4708,0x4710,0x4718,0x6000,0x6008,0x6010,0x6018,
0x6100,0x6108,0x6110,0x6118,0x6200,0x6208,0x6210,0x6218,0x6300,0x6308,
0x6310,0x6318,0x6400,0x6408,0x6410,0x6418,0x6500,0x6508,0x6510,0x6518,
0x6600,0x6608,0x6610,0x6618,0x6700,0x6708,0x6710,0x6718,0x8000,0x8008,
0x8010,0x8018,0x8100,0x8108,0x8110,0x8118,0x8200,0x8208,0x8210,0x8218,
0x8300,0x8308,0x8310,0x8318,0x8400,0x8408,0x8410,0x8418,0x8500,0x8508,
0x8510,0x8518,0x8600,0x8608,0x8610,0x8618,0x8700,0x8708,0x8710,0x8718,
0xa000,0xa008,0xa010,0xa018,0xa100,0xa108,0xa110,0xa118,0xa200,0xa208,
0xa210,0xa218,0xa300,0xa308,0xa310,0xa318,0xa400,0xa408,0xa410,0xa418,
0xa500,0xa508,0xa510,0xa518,0xa600,0xa608,0xa610,0xa618,0xa700,0xa708,
0xa710,0xa718,0xc000,0xc008,0xc010,0xc018,0xc100,0xc108,0xc110,0xc118,
0xc200,0xc208,0xc210,0xc218,0xc300,0xc308,0xc310,0xc318,0xc400,0xc408,
0xc410,0xc418,0xc500,0xc508,0xc510,0xc518,0xc600,0xc608,0xc610,0xc618,
0xc700,0xc708,0xc710,0xc718,0xe000,0xe008,0xe010,0xe018,0xe100,0xe108,
0xe110,0xe118,0xe200,0xe208,0xe210,0xe218,0xe300,0xe308,0xe310,0xe318,
0xe400,0xe408,0xe410,0xe418,0xe500,0xe508,0xe510,0xe518,0xe600,0xe608,
0xe610,0xe618,0xe700,0xe708,0xe710,0xe718
};

/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *		InitialLCD 
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- 처음 전화기가 하드웨어 기본 세팅을 할 때 실행된다.
 *		    - 컬러 LCD의 동작을 위한 기본설정들, LCD 크기, 클럭, 색감 등을 정한다. 
 *
 *		Input parameters
 *		----------------
 *
 *		없음 
 *
 *
 *		Return
 *		------
 *
 *		없음 
 *
 *
 *
 *===========================================================================*/
byte clkk = 0x60;  		//external clk 과 internal clk 값	
byte xind = 0x40;
byte cont_v = 0x4d;		//contrast : electronic volume set

byte tempp[128*164*2];

void SKY_API SKY_InitLCD(void)//void	InitialLCD ( void )
{
	word wordtemp=0;
	byte cont_a, cont_b;	//contrast 조절용 
	unsigned int i, j = 0;
 	const word FGCTBL[]={
	0x8000, 0x84d9, 0x86db, 0x850c, 0x82bb, 0x871c, 0x8914, 0x836a, 0x8ee3, 0x8924, 0x8ed5, 0x8af3, 0x879e, 0x8926, 0x8f17, 0x8b2c, 
	0x8aed, 0x8f2c, 0x89a7, 0x8b6c, 0x87b7, 0x897d, 0x8977, 0x8b6d, 0x89ff, 0x8b6f, 0x8bae, 0x8bef, 0x8bb7, 0x8db6, 0x8dbf, 0x8fff,

	0x8000, 0x8492, 0x84d9, 0x8859, 0x86db, 0x82aa, 0x850c, 0x88a2, 0x82bb, 0x869d, 0x871c, 0x8ca1, 0x8914, 0x885f, 0x836a, 0x8929, 
	0x8356, 0x8ee3, 0x8924, 0x8cb2, 0x8ed5, 0x8d1c, 0x8af3, 0x8eeb, 0x879e, 0x8efb, 0x8926, 0x8f94, 0x8f17, 0x8ce6, 0x8d1f, 0x8b2c,

	0x8aed, 0x8d2c, 0x8f2c, 0x8d34, 0x89a7, 0x89e7, 0x8f5e, 0x8b6c, 0x87b6, 0x87b7, 0x896e, 0x897d, 0x8976, 0x8977, 0x897f, 0x89b6, 
	0x8b6d, 0x89f7, 0x89ff, 0x8b6e, 0x8b6f, 0x8bae, 0x8baf, 0x8bef, 0x8bb6, 0x8bb7, 0x8bbf, 0x8db6, 0x8db7, 0x8dbf, 0x8dff, 0x8fff,

	0x8000, 0x8492, 0x8110, 0x8890, 0x8582, 0x8496, 0x8820, 0x8504, 0x88b0, 0x8514, 0x8d82, 0x8ca2, 0x8cb2, 0x8904, 0x8930, 0x8524, 
	0x8d84, 0x8d14, 0x8d86, 0x8d94, 0x8d94, 0x85b6, 0x8924, 0x8924, 0x8926, 0x8926, 0x89a6, 0x89a6, 0x89b6, 0x89b6, 0x8db6, 0x8db6
	};

    byte* ctemp =  (byte*)g_BmpBootLogoMain.pbData;	//initLCD에서 부트 로고를 미리 넣어둠 
    register byte* pbBuffer;
	pbBuffer = ctemp;


//	BIO_TRISTATE(DEC_GPIO_1, BIO_DC_ON_M,0xFF);      
//	BIO_OUT(DEC_GPIO_1, BIO_DC_ON_M, BIO_DC_ON_HI_V);
//	BIO_TRISTATE(DEC_GPIO_0, BIO_LCD_LED_M,0xFF);      
//	BIO_OUT(DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_M);

//	BIO_TRISTATE(ENC_GPIO_1, BIO_64KCOLOR_LCD_M,0xFF);      // mc_device_init으로 옮김
//	BIO_OUT(ENC_GPIO_1, BIO_64KCOLOR_LCD_M, BIO_64KCOLOR_LCD_HI_V);	// mc_device_init으로 옮김


	clk_busy_wait(5);	//2000.05.10 5 us wait
	
/*
---------------------------------------------------------------------------------------------
	CFG register Set 
---------------------------------------------------------------------------------------------
*/ 
	sLCDCnf(xind);	//register set  WDEL = '001', FORCE = 'H', 8bit I/F, XIND = 'H', NUMWR = 'H'
	sLCDCnf(0x00);

/*
---------------------------------------------------------------------------------------------
	DEN bit Reset 
---------------------------------------------------------------------------------------------
*/
	sLCDCtl(0x24);	//register set  DEN ='L', EDIV(1/7) = '00111', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);

/*
---------------------------------------------------------------------------------------------
	Table Write : FGC Table write          
---------------------------------------------------------------------------------------------
*/
	sLCDCnf(0x00);	//4000
	sLCDCnf(0x40);
		for(i=0; i<0x80; ++i){
			wordtemp=(FGCTBL[i]&0x00ff);
			sLCDCnf((BYTE)(wordtemp));
			wordtemp=(FGCTBL[i]&0xff00);
			wordtemp>>=8;
			sLCDCnf((BYTE)(wordtemp));
		}
/*
---------------------------------------------------------------------------------------------
	파워회로 초기화 : Master
---------------------------------------------------------------------------------------------
*/	
	sLCDCtl(0x2c);		//DEN ='L', BOTH = 'H'
	sLCDCtl(clkk);
		
	sLCDMaster(0xf4);	//Master Side  Bank4	
	sLCDMaster(0xb4);	//Master Side  Electronic Volume(upper nibble)DV[6:4] = "101"
	sLCDMaster(0xad);	//Master Side  Electronic Volume(lower nibble)DV[3:0] = "1001"	
	sLCDMaster(0xd0);	//Master Side  Select Rf	
	sLCDMaster(0xf0);	//Master Side  Bank0
//	sLCDMaster(0xe2);	//Master Side  1/7bias	
	sLCDMaster(0xe3);	//Master Side  1/6bias
	sLCDMaster(0xd4);	//Master Side  5 times Booster set	
	sLCDMaster(0x66);	//Master Side  n-line alternative N3?N0 = '0110'  7-lines
	sLCDMaster(0x70);	//Master Side  n-line alternative N6?N4 = '000'
	sLCDMaster(0xba);	//power circuit on Master Side  Power control : Booster ON DCON ='H',internal AMPON AMPON='H'
	sLCDMaster(0xf5);
	sLCDMaster(0xa2);	//PWM setting
	
	
/* 
---------------------------------------------------------------------------------------------
	Gradation palette Set : Master And Slave	
---------------------------------------------------------------------------------------------
*/
 	sLCDCtl(0x2c);		//2000.05.10 BOTH = 'H', EDIV(1/6) = '00111', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);

	clk_busy_wait(50000);	//2000.05.10 50ms wait
	
	//master palette
	sLCDMaster(0xf1);	//Master Side Common Bank1 set
	sLCDMaster(0xf1);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDMaster(0x46);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDMaster(0x60);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDMaster(0x8a);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDMaster(0xa0);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDMaster(0xcf);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDMaster(0xf2);	//Master Side Common Bank2 set
	sLCDMaster(0xf2);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDMaster(0x41);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDMaster(0x64);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDMaster(0x88);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDMaster(0xa0);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDMaster(0xca);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDMaster(0xf3);	//Master Side Common Bank3 set
	sLCDMaster(0xf3);	//dummy

	sLCDMaster(0x0d);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDMaster(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDMaster(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDMaster(0x61);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDMaster(0x84);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDMaster(0xa8);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDMaster(0xb0);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDMaster(0xc0);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDMaster(0xf4);	//Master Side Common Bank4 set
	sLCDMaster(0xf4);	//dummy

	sLCDMaster(0x0a);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDMaster(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDMaster(0x2d);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDMaster(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette C7  set(upper nibble)

	//slave palette
	sLCDSlave(0xf1);	//Master Side Common Bank1 set
	sLCDSlave(0xf1);	//dummy

	sLCDSlave(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDSlave(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDSlave(0x46);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDSlave(0x60);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDSlave(0x8a);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDSlave(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDSlave(0xa0);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDSlave(0xcf);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDSlave(0xf2);	//Master Side Common Bank2 set
	sLCDSlave(0xf2);	//dummy

	sLCDSlave(0x00);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDSlave(0x10);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDSlave(0x41);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDSlave(0x64);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDSlave(0x88);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDSlave(0x90);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDSlave(0xa0);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDSlave(0xca);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDSlave(0xf3);	//Master Side Common Bank3 set
	sLCDSlave(0xf3);	//dummy

	sLCDSlave(0x0d);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDSlave(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDSlave(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDSlave(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDSlave(0x61);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDSlave(0x84);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDSlave(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDSlave(0xa8);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDSlave(0xb0);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDSlave(0xc0);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDSlave(0xf4);	//Master Side Common Bank4 set
	sLCDSlave(0xf4);	//dummy

	sLCDSlave(0x0a);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDSlave(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDSlave(0x2d);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDSlave(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDSlave(0x51);	//Master Side Gradation palette C7  set(upper nibble)

/*
------------------------------------------------------------------------------
	BOTH bit set 
------------------------------------------------------------------------------
*/
	sLCDCtl(0x2c);		//WB = 'L' BOTH = 'H', EDIV(1/6) = '00111', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H' 
	sLCDCtl(clkk);

	sLCDMaster(0xf4);	//Common Bank4 set	
	sLCDMaster(0x90);	//Common  RAM data length 8bit
	sLCDMaster(0xf0);	//Common Bank0 set
	sLCDMaster(0xa3);	//Common  AY,AX auto increment
	sLCDMaster(0x00);	//Common  Xaddress lower nibble
	sLCDMaster(0x10);	//Common  Xaddress upper nibble
	sLCDMaster(0x20);	//Common  Yaddress lower nibble
	sLCDMaster(0x30);	//Common  Yaddress upper nibble
	sLCDMaster(0xf4);	//Common Bank4 set
	sLCDMaster(0x60);	//Common Display start common
	sLCDMaster(0xf0);	//Common Bank0 set
	sLCDMaster(0x40);	//Common  Display start address lower nibble
	sLCDMaster(0x50);	//Common  Display start address upper nibble
	sLCDMaster(0xc0);	//Common  Duty ratio  1/82Duty
	sLCDMaster(0xf4);	//Common Bank4 set
	sLCDMaster(0x80);	//GLSB = 'L'	2001.05.10
	sLCDMaster(0xf5);
	sLCDMaster(0xa2);	//PWM setting

/*
------------------------------------------------------------------------------
	initial controller and driver
------------------------------------------------------------------------------
*/

	for(i=0; i<128*164; i++)
	{		
		sLCDData1(0x00);	//driver master side init
		sLCDData2(0x00);	//driver slave side init 	
	}

	sLCDAdr(0x00);
	sLCDAdr(0x00);

	for(i = 0; i<128*80; i++)
	{
		sLCDDrg(*(pbBuffer++));		//controller init
		sLCDDrg(*(pbBuffer++));		//controller init
	}

	sLCDAdr(0x00);				//slave start address
	sLCDAdr(0x52);

	for(i = 0; i<128*80; i++)
	{
		sLCDDrg(*(pbBuffer++));		//controller init
		sLCDDrg(*(pbBuffer++));		//controller init
	}

/*
------------------------------------------------------------------------------
	BOTH bit Reset 
------------------------------------------------------------------------------
*/
	sLCDCnf(xind);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'
	sLCDCnf(0x00);						

	sLCDCtl(0x24);		//2000.05.10 BOTH = 'L', EDIV = '00110', MDIV = '000', AAC = 'H', CKCTL = 'H' DEN = "L"
	sLCDCtl(clkk);

	sLCDMaster(0xf0);	//Common Bank0 set	
	sLCDMaster(0x97);	//Master Side  NLIN = 'H', SWAP = 'H', REF = 'H'
	sLCDMaster(0x81);	//Master Side  DispON, SHIFT = 'L'
	sLCDSlave(0xf0);	//Common Bank0 set
	sLCDSlave(0x96);	//Slave Side  NLIN = 'H', SWAP = 'H', REF = 'L'
	sLCDSlave(0x89);	//Slave Side  DispON, SHIFT = 'H'

	clk_busy_wait(10);

	sLCDAdr(0x00);		//start address : controller 
	sLCDAdr(0x00);

/*
------------------------------------------------------------------------------
	65K-color Display 
------------------------------------------------------------------------------
*/
	sLCDCtl(0x25); //DEN = '1', WB = '0'
	sLCDCtl(clkk);

/*
----------------------------------------------------------------------------------
	 clear screen 
----------------------------------------------------------------------------------
*/
/*
//display data C
	sLCDCnf(0xc0);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'(1/2)
	//sLCDCnf(0x40);		//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'L'
	sLCDCnf(0x00);

	sLCDCtl(0x25);		//BOTH = 'L', EDIV(1/5) = '00101', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);	
	
	
	for(j=0; j<128*164; j++)
	{
		sLCDDrg(0x0);
		sLCDDrg(0xf8);
	}

	for(j=0;j<65535;j++)
		dog_kick();
	for(j=0;j<65535;j++)
		dog_kick();


//display data A
	sLCDCnf(0xc0);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'(1/2)
	//sLCDCnf(0x40);		//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'L'
	sLCDCnf(0x00);

	sLCDCtl(0x25);		//BOTH = 'L', EDIV(1/5) = '00101', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);

	for(j=0; j<128*164; j++)
	{
		sLCDDrg(0xe0);
		sLCDDrg(0x07);
	}

	for(j=0;j<65535;j++)
		dog_kick();
	for(j=0;j<65535;j++)
		dog_kick();

//display data B
	sLCDCnf(0xc0);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'(1/2)
	//sLCDCnf(0x40);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'L'
	sLCDCnf(0x00);

	sLCDCtl(0x25);		//BOTH = 'L', EDIV = '00101', MDIV = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);

	for(j=0; j<128*164; j++)
	{
		sLCDDrg(0x1f);
		sLCDDrg(0x00);
	}

	for(j=0;j<65535;j++)
		dog_kick();
	for(j=0;j<65535;j++)
		dog_kick();
*/

}

//extern BYTE abContrast[HCL_MAX+1];
extern BYTE ContrastLevel;
void SKY_API SKY_ReInitLCD(void)
{
	word wordtemp=0;
	byte cont_a, cont_b;	//contrast 조절용 
	unsigned int i, j = 0;
 	const word FGCTBL[]={
	0x8000, 0x84d9, 0x86db, 0x850c, 0x82bb, 0x871c, 0x8914, 0x836a, 0x8ee3, 0x8924, 0x8ed5, 0x8af3, 0x879e, 0x8926, 0x8f17, 0x8b2c, 
	0x8aed, 0x8f2c, 0x89a7, 0x8b6c, 0x87b7, 0x897d, 0x8977, 0x8b6d, 0x89ff, 0x8b6f, 0x8bae, 0x8bef, 0x8bb7, 0x8db6, 0x8dbf, 0x8fff,

	0x8000, 0x8492, 0x84d9, 0x8859, 0x86db, 0x82aa, 0x850c, 0x88a2, 0x82bb, 0x869d, 0x871c, 0x8ca1, 0x8914, 0x885f, 0x836a, 0x8929, 
	0x8356, 0x8ee3, 0x8924, 0x8cb2, 0x8ed5, 0x8d1c, 0x8af3, 0x8eeb, 0x879e, 0x8efb, 0x8926, 0x8f94, 0x8f17, 0x8ce6, 0x8d1f, 0x8b2c,

	0x8aed, 0x8d2c, 0x8f2c, 0x8d34, 0x89a7, 0x89e7, 0x8f5e, 0x8b6c, 0x87b6, 0x87b7, 0x896e, 0x897d, 0x8976, 0x8977, 0x897f, 0x89b6, 
	0x8b6d, 0x89f7, 0x89ff, 0x8b6e, 0x8b6f, 0x8bae, 0x8baf, 0x8bef, 0x8bb6, 0x8bb7, 0x8bbf, 0x8db6, 0x8db7, 0x8dbf, 0x8dff, 0x8fff,

	0x8000, 0x8492, 0x8110, 0x8890, 0x8582, 0x8496, 0x8820, 0x8504, 0x88b0, 0x8514, 0x8d82, 0x8ca2, 0x8cb2, 0x8904, 0x8930, 0x8524, 
	0x8d84, 0x8d14, 0x8d86, 0x8d94, 0x8d94, 0x85b6, 0x8924, 0x8924, 0x8926, 0x8926, 0x89a6, 0x89a6, 0x89b6, 0x89b6, 0x8db6, 0x8db6
	};
	
/*
---------------------------------------------------------------------------------------------
	CFG register Set 
---------------------------------------------------------------------------------------------
*/ 
	sLCDCnf(xind);	//register set  WDEL = '001', FORCE = 'H', 8bit I/F, XIND = 'H', NUMWR = 'H'
	sLCDCnf(0x00);

/*
---------------------------------------------------------------------------------------------
	DEN bit Reset 
---------------------------------------------------------------------------------------------
*/
	sLCDCtl(0x24);	//register set  DEN ='L', EDIV(1/7) = '00111', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);

/*
---------------------------------------------------------------------------------------------
	Table Write : FGC Table write          
---------------------------------------------------------------------------------------------
*/
	sLCDCnf(0x00);	//4000
	sLCDCnf(0x40);
		for(i=0; i<0x80; ++i){
			wordtemp=(FGCTBL[i]&0x00ff);
			sLCDCnf((BYTE)(wordtemp));
			wordtemp=(FGCTBL[i]&0xff00);
			wordtemp>>=8;
			sLCDCnf((BYTE)(wordtemp));
		}
/*
---------------------------------------------------------------------------------------------
	파워회로 초기화 : Master
---------------------------------------------------------------------------------------------
*/	
	sLCDCtl(0x2c);		//DEN ='L', BOTH = 'H'
	sLCDCtl(clkk);
		
	sLCDMaster(0xf4);	//Master Side  Bank4	
	sLCDMaster(0xb4);	//Master Side  Electronic Volume(upper nibble)DV[6:4] = "101"
	sLCDMaster(0xad);	//Master Side  Electronic Volume(lower nibble)DV[3:0] = "1001"	
	sLCDMaster(0xd0);	//Master Side  Select Rf	
	sLCDMaster(0xf0);	//Master Side  Bank0
//	sLCDMaster(0xe2);	//Master Side  1/7bias	
	sLCDMaster(0xe3);	//Master Side  1/6bias
	sLCDMaster(0xd4);	//Master Side  5 times Booster set	
	sLCDMaster(0x66);	//Master Side  n-line alternative N3?N0 = '0110'  7-lines
	sLCDMaster(0x70);	//Master Side  n-line alternative N6?N4 = '000'
	sLCDMaster(0xba);	//power circuit on Master Side  Power control : Booster ON DCON ='H',internal AMPON AMPON='H'
	sLCDMaster(0xf5);
	sLCDMaster(0xa2);	//PWM setting

	//SKY_GetNV( NV_LCD_I, (nv_item_type *)&(abContrast[HCL_D]));
	//SKY_SetDisplayContrast((HS_CONTRAST_E)SKY_GET_CONTRAST(), abContrast[HCL_D]);	
	SKY_SetDisplayContrast((HS_CONTRAST_E)SKY_GET_CONTRAST(), ContrastLevel);	
	
/* 
---------------------------------------------------------------------------------------------
	Gradation palette Set : Master And Slave	
---------------------------------------------------------------------------------------------
*/
 	sLCDCtl(0x2c);		//2000.05.10 BOTH = 'H', EDIV(1/6) = '00111', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);

	clk_busy_wait(50000);	//2000.05.10 50ms wait
	
	//master palette
	sLCDMaster(0xf1);	//Master Side Common Bank1 set
	sLCDMaster(0xf1);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDMaster(0x46);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDMaster(0x60);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDMaster(0x8a);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDMaster(0xa0);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDMaster(0xcf);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDMaster(0xf2);	//Master Side Common Bank2 set
	sLCDMaster(0xf2);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDMaster(0x41);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDMaster(0x64);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDMaster(0x88);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDMaster(0xa0);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDMaster(0xca);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDMaster(0xf3);	//Master Side Common Bank3 set
	sLCDMaster(0xf3);	//dummy

	sLCDMaster(0x0d);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDMaster(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDMaster(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDMaster(0x61);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDMaster(0x84);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDMaster(0xa8);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDMaster(0xb0);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDMaster(0xc0);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDMaster(0xf4);	//Master Side Common Bank4 set
	sLCDMaster(0xf4);	//dummy

	sLCDMaster(0x0a);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDMaster(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDMaster(0x2d);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDMaster(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette C7  set(upper nibble)

	//slave palette
	sLCDSlave(0xf1);	//Master Side Common Bank1 set
	sLCDSlave(0xf1);	//dummy

	sLCDSlave(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDSlave(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDSlave(0x46);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDSlave(0x60);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDSlave(0x8a);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDSlave(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDSlave(0xa0);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDSlave(0xcf);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDSlave(0xf2);	//Master Side Common Bank2 set
	sLCDSlave(0xf2);	//dummy

	sLCDSlave(0x00);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDSlave(0x10);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDSlave(0x41);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDSlave(0x64);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDSlave(0x88);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDSlave(0x90);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDSlave(0xa0);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDSlave(0xca);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDSlave(0xf3);	//Master Side Common Bank3 set
	sLCDSlave(0xf3);	//dummy

	sLCDSlave(0x0d);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDSlave(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDSlave(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDSlave(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDSlave(0x61);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDSlave(0x84);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDSlave(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDSlave(0xa8);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDSlave(0xb0);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDSlave(0xc0);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDSlave(0xf4);	//Master Side Common Bank4 set
	sLCDSlave(0xf4);	//dummy

	sLCDSlave(0x0a);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDSlave(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDSlave(0x2d);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDSlave(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDSlave(0x51);	//Master Side Gradation palette C7  set(upper nibble)

/*
------------------------------------------------------------------------------
	BOTH bit set 
------------------------------------------------------------------------------
*/
	sLCDCtl(0x2c);		//WB = 'L' BOTH = 'H', EDIV(1/6) = '00111', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H' 
	sLCDCtl(clkk);

	sLCDMaster(0xf4);	//Common Bank4 set	
	sLCDMaster(0x90);	//Common  RAM data length 8bit
	sLCDMaster(0xf0);	//Common Bank0 set
	sLCDMaster(0xa3);	//Common  AY,AX auto increment
	sLCDMaster(0x00);	//Common  Xaddress lower nibble
	sLCDMaster(0x10);	//Common  Xaddress upper nibble
	sLCDMaster(0x20);	//Common  Yaddress lower nibble
	sLCDMaster(0x30);	//Common  Yaddress upper nibble
	sLCDMaster(0xf4);	//Common Bank4 set
	sLCDMaster(0x60);	//Common Display start common
	sLCDMaster(0xf0);	//Common Bank0 set
	sLCDMaster(0x40);	//Common  Display start address lower nibble
	sLCDMaster(0x50);	//Common  Display start address upper nibble
	sLCDMaster(0xc0);	//Common  Duty ratio  1/82Duty
	sLCDMaster(0xf4);	//Common Bank4 set
	sLCDMaster(0x80);	//GLSB = 'L'	2001.05.10
	sLCDMaster(0xf5);
	sLCDMaster(0xa2);	//PWM setting

/*
------------------------------------------------------------------------------
	BOTH bit Reset 
------------------------------------------------------------------------------
*/
	sLCDCnf(xind);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'
	sLCDCnf(0x00);						

	sLCDCtl(0x24);		//2000.05.10 BOTH = 'L', EDIV = '00110', MDIV = '000', AAC = 'H', CKCTL = 'H' DEN = "L"
	sLCDCtl(clkk);

	sLCDMaster(0xf0);	//Common Bank0 set	
	sLCDMaster(0x97);	//Master Side  NLIN = 'H', SWAP = 'H', REF = 'H'
	sLCDMaster(0x81);	//Master Side  DispON, SHIFT = 'L'
	sLCDSlave(0xf0);	//Common Bank0 set
	sLCDSlave(0x96);	//Slave Side  NLIN = 'H', SWAP = 'H', REF = 'L'
	sLCDSlave(0x89);	//Slave Side  DispON, SHIFT = 'H'

	clk_busy_wait(10);

	sLCDAdr(0x00);		//start address : controller 
	sLCDAdr(0x00);

/*
------------------------------------------------------------------------------
	65K-color Display 
------------------------------------------------------------------------------
*/
	sLCDCtl(0x25); //DEN = '1', WB = '0'
	sLCDCtl(clkk);
}

void SKY_API SKY_PartialDisplay_ON(void)
{
	int i, j;

#ifdef WIN32
    return;
#endif
/*
	sLCDAdr(0x00);				//16번째 줄부터 검은색으로 채운다. 
	sLCDAdr(0x0f);

	for(i = 0; i < (128*164) - (128*15); i++)
	{
		sLCDDrg(0x00);
		sLCDDrg(0x00);
	}

    sLCDCnf(0x00);	//2001.08.16 NUMWR = 'L'
	sLCDCnf(0x00);
*/
	sLCDCtl(0x34);	//2001.07.18
	sLCDCtl(clkk);
	
	clk_busy_wait(30000);

	sLCDMaster(0xf4);	
	sLCDMaster(0x90);	

	sLCDCtl(0x34);	//2001.07.18
	sLCDCtl(0x00);

	sLCDCtl(0x26);	//CKCTL = '1' use internal clock
	sLCDCtl(0x00);

	clk_busy_wait(30000);	//30ms wait add 01.07.10

/*
---------------------------------------------------------------------------------------------
	1. Display OFF
---------------------------------------------------------------------------------------------
*/
	sLCDMaster(0xf0);	//Master Side  Bank0
	sLCDMaster(0x80);	//master : DispOFF, SHIFT = 'L'

	sLCDSlave(0xf0);	//slave Side  Bank0
	sLCDSlave(0x88);	//slave : DispOFF, SHIFT = 'H'

/*
---------------------------------------------------------------------------------------------
	2. Power-circuit OFF
---------------------------------------------------------------------------------------------
*/
	sLCDMaster(0xf0);	//Master Side  Bank0
	sLCDMaster(0xb4);	//change 01.07.10 DCON = '0', AMPON = '0'
	sLCDMaster(0xf4);	//Master Side  Bank4
	sLCDMaster(0xe1);	//Discharg ON

	clk_busy_wait(30000);	//30ms wait

/*
---------------------------------------------------------------------------------------------
	3. Power-circuit re-initialization 
---------------------------------------------------------------------------------------------
*/
//	sLCDMaster(0xf4);	//Master Side  Bank4		
//	sLCDMaster(0xb4);	//Master Side  Electronic Volume(lower nibble)DV[3:0] = "1001"
//	sLCDMaster(0xad);	//Master Side  Electronic Volume(upper nibble)DV[6:4] = "101"	
	sLCDMaster(0xf0);	//Master Side  Bank0
	sLCDMaster(0xe4);	//Master Side  1/5bias	
	sLCDMaster(0xd2);	//Master Side  3 times Booster set	
	sLCDMaster(0x61);	//Master Side  n-line alternative N3?N0 = '0110'  2-lines
	sLCDMaster(0x70);	//Master Side  n-line alternative N6?N4 = '000'

/*
---------------------------------------------------------------------------------------------
	4. FGC HALT	DEN bit Reset, HALT Set And 256 color palette
---------------------------------------------------------------------------------------------
*/
/*
	sLCDCtl(0x3c); 		//256 color WB = '1', HALT = '1', DEN = '0'
	sLCDCtl(clkk);
	
	clk_busy_wait(50000);	//50ms wait

	sLCDMaster(0xf1);	//Master Side Common Bank1 set
	sLCDMaster(0xf1);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDMaster(0x41);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDMaster(0x61);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDMaster(0x71);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDMaster(0x86);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDMaster(0xa6);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDMaster(0xce);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDMaster(0xf2);	//Master Side Common Bank2 set
	sLCDMaster(0xf2);	//dummy

	sLCDMaster(0x0e);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDMaster(0x49);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDMaster(0x6f);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDMaster(0x82);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDMaster(0xa5);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDMaster(0xc7);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDMaster(0xf3);	//Master Side Common Bank3 set
	sLCDMaster(0xf3);	//dummy

	sLCDMaster(0x09);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDMaster(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDMaster(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDMaster(0x69);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDMaster(0x8e);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDMaster(0xa2);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDMaster(0xc5);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDMaster(0xf4);	//Master Side Common Bank4 set
	sLCDMaster(0xf4);	//dummy

	sLCDMaster(0x07);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDMaster(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDMaster(0x2a);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDMaster(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette C7  set(upper nibble)

/*
---------------------------------------------------------------------------------------------
	5. Power-circuit ON 
---------------------------------------------------------------------------------------------
*/
	
	sLCDMaster(0xf4);	//DCON = '1', AMPON = '1'
	sLCDMaster(0xe0);	//DCON = '1', AMPON = '1'

	sLCDMaster(0xf0);	//DCON = '1', AMPON = '1'
	sLCDMaster(0xba);	//DCON = '1', AMPON = '1'

	clk_busy_wait(50000);	//50ms wait

/*
------------------------------------------------------------------------------
	6. BOTH bit set 
------------------------------------------------------------------------------
*/
	sLCDMaster(0xf4);	//Common Bank4 set
	sLCDMaster(0x60);	//Display start command 
	sLCDMaster(0xf0);	//Common Bank0 set
	sLCDMaster(0x40);	//Common  Display start address lower nibble : Line Address 0
	sLCDMaster(0x50);	//Common  Display start address upper nibble
	sLCDMaster(0xc5);	//Common  Duty ratio  1/17Duty
//	sLCDMaster(0xf4);	//Common Bank4 set
//	sLCDMaster(0x88);	//Display select control GLSB = '0', PWM = '1'

	sLCDSlave(0xf4);	//Common Bank4 set
	sLCDSlave(0x60);	//Display start command 
	sLCDSlave(0xf0);	//Common Bank0 set
	sLCDSlave(0x40);	//Common  Display start address lower nibble : Line Address 0
	sLCDSlave(0x50);	//Common  Display start address upper nibble
	sLCDSlave(0xc5);	//Common  Duty ratio  1/17Duty
//	sLCDSlave(0xf4);	//Common Bank4 set
//	sLCDSlave(0x88);	//Display select control GLSB = '0', PWM = '1'

/*
---------------------------------------------------------------------------------------------
	FGC HALT	DEN bit Reset, HALT Set And 256 color palette
---------------------------------------------------------------------------------------------
*/
	/*
	Master palette setting
	*/
/*	sLCDMaster(0xf1);	//Master Side Common Bank1 set
	sLCDMaster(0xf1);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDMaster(0x41);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDMaster(0x61);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDMaster(0x71);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDMaster(0x86);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDMaster(0xa6);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDMaster(0xce);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDMaster(0xf2);	//Master Side Common Bank2 set
	sLCDMaster(0xf2);	//dummy

	sLCDMaster(0x0e);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDMaster(0x49);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDMaster(0x6f);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDMaster(0x82);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDMaster(0xa5);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDMaster(0xc7);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDMaster(0xf3);	//Master Side Common Bank3 set
	sLCDMaster(0xf3);	//dummy

	sLCDMaster(0x09);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDMaster(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDMaster(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDMaster(0x69);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDMaster(0x8e);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDMaster(0xa2);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDMaster(0xc5);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDMaster(0xf4);	//Master Side Common Bank4 set
	sLCDMaster(0xf4);	//dummy

	sLCDMaster(0x07);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDMaster(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDMaster(0x2a);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDMaster(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette C7  set(upper nibble)

	/*
	slave palette setting
	*/

/*	sLCDSlave(0xf1);	//Master Side Common Bank1 set
	sLCDSlave(0xf1);	//dummy

	sLCDSlave(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDSlave(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDSlave(0x41);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDSlave(0x51);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDSlave(0x61);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDSlave(0x71);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDSlave(0x86);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDSlave(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDSlave(0xa6);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDSlave(0xce);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDSlave(0xf2);	//Master Side Common Bank2 set
	sLCDSlave(0xf2);	//dummy

	sLCDSlave(0x0e);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDSlave(0x11);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDSlave(0x49);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDSlave(0x6f);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDSlave(0x82);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDSlave(0x91);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDSlave(0xa5);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDSlave(0xc7);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDSlave(0xf3);	//Master Side Common Bank3 set
	sLCDSlave(0xf3);	//dummy

	sLCDSlave(0x09);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDSlave(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDSlave(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDSlave(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDSlave(0x69);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDSlave(0x8e);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDSlave(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDSlave(0xa2);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDSlave(0xc5);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDSlave(0xf4);	//Master Side Common Bank4 set
	sLCDSlave(0xf4);	//dummy

	sLCDSlave(0x07);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDSlave(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDSlave(0x2a);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDSlave(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDSlave(0x51);	//Master Side Gradation palette C7  set(upper nibble)
*/



/*
------------------------------------------------------------------------------
	7. Display data write
------------------------------------------------------------------------------
*/
/*
	sLCDMaster(0xf0);		//master start address setting		
	sLCDMaster(0x00);
	sLCDMaster(0x10);
	sLCDMaster(0x20);
	sLCDMaster(0x30);

	for(i = 0; i < 128 * 15; i++)
		sLCDData1(0xff);
*/
	sLCDSlave(0xf0);		//slave start address setting
	sLCDSlave(0x00);
	sLCDSlave(0x10);
	sLCDSlave(0x20);
	sLCDSlave(0x30);

	for(i = 0; i < 128 * 15; i++)
		sLCDData2(0);

/*
------------------------------------------------------------------------------
	8. BOTH bit Reset 
------------------------------------------------------------------------------
*/
//	sLCDCnf(xind);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'
//	sLCDCnf(0x00);
	
//	sLCDCtl(0x24);		//2001.07.18 hynix driver
//	sLCDCtl(clkk);

	sLCDMaster(0xf0);	//Common Bank0 set
	sLCDMaster(0xf0);	//dummy
	sLCDMaster(0x97);	//Master Side  NLIN = 'H', SWAP = 'H', REF = 'H'
	sLCDMaster(0x81);	//Master Side  DispON, SHIFT = 'L'
	
	sLCDSlave(0xf0);	//Common Bank0 set
	sLCDSlave(0xf0);	//dummy
	sLCDSlave(0x96);	//Slave Side  NLIN = 'H', SWAP = 'H', REF = 'L'
	sLCDSlave(0x89);	//Slave Side  DispON, SHIFT = 'H'

	clk_busy_wait(10);

/*
------------------------------------------------------------------------------
	9. Partial Display
------------------------------------------------------------------------------
*/
	
}

// by jnlee 01.10.09
byte bPartial_Off_Flag = 0xff;	//0 : rest이용 , 1:read이용 
void SKY_LCDReset(void)
{
	word *cs;
	volatile word read;

	//LCD wait reset timing 을 늘리기 위해서
	outpw( LCD_CTL_WH,		  LCD_CTL_WH__LCD_E_HIGH_8CLK |
                                LCD_CTL_WH__LCD_E_SETUP_7CLK |
                                LCD_CTL_WH__LCD_WAIT_1fCLK );
	clk_busy_wait(5000);
	//reset
	cs = (word *)0x2000000;
	read = *cs;
	clk_busy_wait(5000);
	//LCD wait 복귀 
	outpw( LCD_CTL_WH,      LCD_CTL_WH__LCD_E_HIGH_2CLK |
                                LCD_CTL_WH__LCD_E_SETUP_1CLK |
                                LCD_CTL_WH__LCD_WAIT_3CLK );
	clk_busy_wait(5000);
}

void SKY_API SKY_PartialDisplay_OFF(void)
{
    int i = 0;
	byte cont_a, cont_b;	//adjustment contrast 
	word wordtemp=0;		//for 65k color table write
	word *cs;
	volatile word read;			//for reset signal 
	
#ifdef WIN32
    return;
#endif
/*
	if(bPartial_Off_Flag == 0xff)	// 처음 들어온 경우 
	{
		if(NV_DONE_S != SKY_GetNV (NV_PA_RANGE_STEP_CAL_I, (nv_item_type *)&bPartial_Off_Flag))
			bPartial_Off_Flag = 0;
	}
*/	
//#ifdef FEATURE_LCD_RESET
  if(bPartial_Off_Flag == 0)		// reset이용 partial out   by jnlee 01.10.09
  { 
	SKY_LCDReset();
	SKY_ReInitLCD();


//#else
  }
  else
  {		// read이용 partial out!
	
//	sLCDCnf(xind);		//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'
//	sLCDCnf(0x00);

//	sLCDCtl(0x24);		//2001.07.18 hynix driver
//	sLCDCtl(clkk);

/*
---------------------------------------------------------------------------------------------
	1. Display OFF
---------------------------------------------------------------------------------------------
*/
	sLCDMaster(0xf0);	//Master Side  Bank0
	sLCDMaster(0x80);	//master : DispOFF, SHIFT = 'L'

	sLCDSlave(0xf0);	//slave Side  Bank0
	sLCDSlave(0x88);	//slave : DispOFF, SHIFT = 'H'

/*
---------------------------------------------------------------------------------------------
	2. Power-circuit OFF
---------------------------------------------------------------------------------------------
*/
	sLCDMaster(0xf0);	//Master Side  Bank0
	sLCDMaster(0xb0);	//DCON = '0', AMPON = '0', HALT = '1'
	sLCDMaster(0xf4);	//Master Side  Bank4
	sLCDMaster(0xe1);	//Discharg ON

	clk_busy_wait(30000);	//30ms wait

/*
---------------------------------------------------------------------------------------------
	3. Reset
---------------------------------------------------------------------------------------------
*/
/*	cs = (word *)0x2000000;
	read = *cs;

	clk_busy_wait(5);		//5 us wait
*/


/*
---------------------------------------------------------------------------------------------
	4. Power-circuit re-initialization 
---------------------------------------------------------------------------------------------
*/

//	sLCDMaster(0xf4);	//Master Side  Bank4		
//	sLCDMaster(0xb4);	//Master Side  Electronic Volume(upper nibble)DV[6:4] = "101"
//	sLCDMaster(0xad);	//Master Side  Electronic Volume(lower nibble)DV[3:0] = "1001"
	sLCDMaster(0xf0);	//Master Side  Bank0
//	sLCDMaster(0xe2);	//Master Side  1/7bias	
	sLCDMaster(0xe3);	//Master Side  1/6bias
	sLCDMaster(0xd4);	//Master Side  5 times Booster set	
	sLCDMaster(0x6c);	//Master Side  n-line alternative N3?N0 = '0110'  7-lines
	sLCDMaster(0x70);	//Master Side  n-line alternative N6?N4 = '000'

/*
---------------------------------------------------------------------------------------------
	5. Power-circuit ON 
---------------------------------------------------------------------------------------------
*/
	sLCDMaster(0xf4);	//Master Side  Bank4
	sLCDMaster(0xe0);	//DCON = '1', AMPON = '1'
	sLCDMaster(0xf0);	//Master Side  Bank0
	sLCDMaster(0xba);	//DCON = '1', AMPON = '1'
	sLCDMaster(0xf4);	//Common Bank4 set
	sLCDMaster(0x60);	//Common  RAM data length 8bit
	sLCDMaster(0xf0);	//Common Bank0 set
	sLCDMaster(0x40);	//Common  Display start address lower nibble
	sLCDMaster(0x50);	//Common  Display start address upper nibble
	sLCDMaster(0xc0);	//Common  Duty ratio  1/82Duty
	sLCDMaster(0xf4);	//Common Bank4 set
	sLCDMaster(0x80);	//Display select control GLSB = '0', PWM = '0'

	sLCDSlave(0xf4);	//Master Side  Bank4
	sLCDSlave(0xe0);	//DCON = '1', AMPON = '1'
	sLCDSlave(0xf0);	//Master Side  Bank0
	sLCDSlave(0xba);	//DCON = '1', AMPON = '1'
	sLCDSlave(0xf4);	//Common Bank4 set
	sLCDSlave(0x60);	//Common  RAM data length 8bit
	sLCDSlave(0xf0);	//Common Bank0 set
	sLCDSlave(0x40);	//Common  Display start address lower nibble
	sLCDSlave(0x50);	//Common  Display start address upper nibble
	sLCDSlave(0xc0);	//Common  Duty ratio  1/82Duty
	sLCDSlave(0xf4);	//Common Bank4 set
	sLCDSlave(0x80);	//Display select control GLSB = '0', PWM = '0'
/* 
---------------------------------------------------------------------------------------------
	Gradation palette Set : 65K palette setting Master And Slave	
---------------------------------------------------------------------------------------------
*/
 	sLCDCtl(0x2c);		//2000.05.10 BOTH = 'H', EDIV(1/6) = '00111', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);

	clk_busy_wait(50000);	//2000.05.10 50ms wait

	sLCDMaster(0xf1);	//Master Side Common Bank1 set
	sLCDMaster(0xf1);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDMaster(0x46);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDMaster(0x60);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDMaster(0x8a);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDMaster(0xa0);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDMaster(0xcf);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDMaster(0xf2);	//Master Side Common Bank2 set
	sLCDMaster(0xf2);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDMaster(0x41);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDMaster(0x64);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDMaster(0x88);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDMaster(0xa0);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDMaster(0xca);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDMaster(0xf3);	//Master Side Common Bank3 set
	sLCDMaster(0xf3);	//dummy

	sLCDMaster(0x0d);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDMaster(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDMaster(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDMaster(0x61);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDMaster(0x84);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDMaster(0xa8);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDMaster(0xb0);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDMaster(0xc0);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDMaster(0xf4);	//Master Side Common Bank4 set
	sLCDMaster(0xf4);	//dummy

	sLCDMaster(0x0a);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDMaster(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDMaster(0x2d);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDMaster(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette C7  set(upper nibble)

//slave palette
	sLCDSlave(0xf1);	//Master Side Common Bank1 set
	sLCDSlave(0xf1);	//dummy

	sLCDSlave(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDSlave(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDSlave(0x46);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDSlave(0x60);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDSlave(0x8a);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDSlave(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDSlave(0xa0);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDSlave(0xcf);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDSlave(0xf2);	//Master Side Common Bank2 set
	sLCDSlave(0xf2);	//dummy

	sLCDSlave(0x00);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDSlave(0x10);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDSlave(0x41);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDSlave(0x64);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDSlave(0x88);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDSlave(0x90);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDSlave(0xa0);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDSlave(0xca);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDSlave(0xf3);	//Master Side Common Bank3 set
	sLCDSlave(0xf3);	//dummy

	sLCDSlave(0x0d);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDSlave(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDSlave(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDSlave(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDSlave(0x61);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDSlave(0x84);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDSlave(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDSlave(0xa8);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDSlave(0xb0);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDSlave(0xc0);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDSlave(0xf4);	//Master Side Common Bank4 set
	sLCDSlave(0xf4);	//dummy

	sLCDSlave(0x0a);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDSlave(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDSlave(0x2d);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDSlave(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDSlave(0x51);	//Master Side Gradation palette C7  set(upper nibble)
/*
------------------------------------------------------------------------------
	6. Display ON
------------------------------------------------------------------------------
*/
	sLCDCnf(xind);	//2001.08.16 WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'
	sLCDCnf(0x00);

	sLCDCtl(0x24);		
	sLCDCtl(clkk);

	sLCDMaster(0xf0);	//Common Bank0 set
	sLCDMaster(0xf0);	//dummy
	sLCDMaster(0x97);	//Master Side  NLIN = 'H', SWAP = 'H', REF = 'H'
	sLCDMaster(0x81);	//Master Side  DispON, SHIFT = 'L'
		
	sLCDSlave(0xf0);	//Common Bank0 set
	sLCDSlave(0xf0);	//dummy
	sLCDSlave(0x96);	//Slave Side  NLIN = 'H', SWAP = 'H', REF = 'L'
	sLCDSlave(0x89);	//Slave Side  DispON, SHIFT = 'H'

	//read
	cs = (word *)0x2000000;
	read = *cs;

	clk_busy_wait(10);		

	sLCDCtl(0x24);		
	sLCDCtl(clkk);

	sLCDCtl(0x24);		//2001.07.18 hynix driver
	sLCDCtl(clkk);

	sLCDMaster(0xf4);	
	sLCDMaster(0x92);	

	sLCDAdr(0x00);
	sLCDAdr(0x00);
	
/*
------------------------------------------------------------------------------
	7. 65K-color Display 
------------------------------------------------------------------------------
*/
	sLCDCtl(0x25);		//CKCTL = '1', DEN = '1', AAC = '1'		
	sLCDCtl(clkk);

//#endif
  }
}

/* folder를 열때 사용하는 initialize	01.06.28 by jnlee */
void SKY_API folder_SKY_InitLCD(void)
{
#if 0 // 0000000000000000000000000000000000000000000000000000000000000000000000000
	sLCDCommand(DISPLAY_CONTROL);		// 
	sLCDData(F12_OP_PATTERN_8H|DIVIDED_PERIOD_2|F12_NORMAL_MODE|    \
		          FRC_NORMAL_MODE|FRC_FORWARD_FLOW|FRC_FRAME_FLOW);
	sLCDData(LCD_DUTY_1_120);
	sLCDData(INVERTED_LINE_12);

	sLCDCommand(DATA_CONTROL);		 
	sLCDData(LOCATION_0_PAGE_LN1|LOCATION_119_COLUMN_SEG123|COLUMN_SCAN_DIRECTION |  \
				  CONVERT_01_TO_001|CONVERT_10_TO_100);
	sLCDData(BGR_BGR_1_ARRANGEMENT);

	// column/page address set
	sSetLCDAddress(LCD_ORIGIN,LCD_TOTAL_COLUMN_SIZE-1,LCD_ORIGIN,LCD_TOTAL_PAGE_SIZE-1);
	
	// The default display is inverted one so that 
	sLCDCommand(DISPLAY_INVERSE);
#endif // 0000000000000000000000000000000000000000000000000000000000000000000000000
}



#ifdef FEATURE_64KCOLOR_LCD	// by jnlee 01.05.28
void fill_buff(word color)
{
	word i;
	byte hi,lo;
	register byte *ctemp;
	ctemp = tempp;

	hi = (byte)(color>>8);
	lo = (byte)(color&0xff);
	for(i=0; i<128*164; i++)
	{
		*ctemp=lo;
		ctemp++;
		*ctemp=hi;
		ctemp++;
	}
}

void fill_france(void)
{
	word i;
	register byte *ctemp;
	ctemp = tempp;

	for(i=0; i<64*164/2; i++)
	{
		*ctemp=0;
		ctemp++;
		*ctemp=0xf8;
		ctemp++;
	}
	for(i=0; i<128*164/4; i++)
	{
		*ctemp=0xe0;
		ctemp++;
		*ctemp=0x07;
		ctemp++;
	}
	for(i=0; i<128*164/4; i++)
	{
		*ctemp=0x1f;
		ctemp++;
		*ctemp=0x00;
		ctemp++;
	}
	for(i=0; i<128*164/4; i++)
	{
		*ctemp=0xff;
		ctemp++;
		*ctemp=0xff;
		ctemp++;
	}
}

void fill_lcd(byte *input_ptr)
{
	register byte *pbBuffer;
	register unsigned int i,j,k;

	pbBuffer = input_ptr;

	sLCDCnf(xind);	
	sLCDCnf(0x00);

	sLCDAdr(0x00);
	sLCDAdr(0x00);

	sLCDCtl(0x25);		
	sLCDCtl(clkk);

	//display data write
	for (i = 0 ; i < 160 ; i++)				//write data to master
	{
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
	}
	
	sLCDAdr(0x00);				//slave start address
	sLCDAdr(0x52);
		
	for (i = 0; i < 160; i++)	//write data to slave
	{
		
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));

		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
		sLCDDrg(*(pbBuffer++));
	}
}
void SKY_FillScreenImage(void){
	fill_lcd ((byte *)g_BmpSandglass.pbData);		//모래시계
}

void SKY_FillScreenOneColor(WORD c)
{
//	fill_buff(c);
	memset((WORD*)tempp,c,sizeof(tempp));
	fill_lcd (tempp);
}
extern void SKY_256ColorDisplay(BYTE* directbuffer);  
void SKY_256FillScreenOneColor(BYTE c)
{
	memset((BYTE*)tempp,c,sizeof(tempp));
	SKY_256ColorDisplay(tempp);
}

#endif	//#ifdef FEATURE_64KCOLOR_LCD


void SKY_BootLogoDisplay(void)
{
    const byte* pbBuffer =  g_BmpBootLogoMain.pbData;
#ifdef FEATURE_64KCOLOR_LCD

#ifdef WIN32
    return;
#endif

//	fill_lcd ((byte *)pbBuffer);		// Sky_initLCD에서 넣어줌 

#else
	BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0); //color LCD select
	/* 사용가능한 주소를 모두 사용한다. 이는 초기화과정에서 이미 설정되어 있다.*/
	sSetLCDAddress(LCD_ORIGIN, LCD_WIDTH, LCD_ORIGIN, LCD_HEIGHT);
	
	sLCDCommand(RAM_WRITE);
    
	nTotalLCDSize = SKY_SCREEN_BUFFER_WIDTH * SKY_SCREEN_BUFFER_HEIGHT;
    for (i = 0 ; i < nTotalLCDSize; i++) 
	{
        sLCDData(*(pbBuffer+i));
    }
#endif
}


boolean Display64K = TRUE;	
void SKY_UpdateScreen()
{	
	register byte *pbBuffer;		
	register unsigned int i,j;
	
	//use 256 display mode
	byte converted_temp;
	word R_data, G_data, B_data;
	word converted_base;

#ifdef WIN32
    return;
#endif

	pbBuffer = GetScreenBuffer();

	//display data write
	if(Display64K == TRUE)					
	{
		fill_lcd (pbBuffer);
	}		
	else if(Display64K == FALSE)		//256 display mode 
	{
		sLCDMaster(0xf0);				//master start address setting		
		sLCDMaster(0x00);
		sLCDMaster(0x10);
		sLCDMaster(0x20);
		sLCDMaster(0x30);

		for (i = 0 ; i < 80 ; i++)		//write data to master
		{
			for (j = 0; j< 128 ; j++)
			{		
				converted_base = (word)(*(pbBuffer++));
				converted_base |= (word)(*(pbBuffer++) << 8);
				
				R_data = ((converted_base) & 0xe000);
				G_data = ((converted_base) & 0x0700);
				B_data = ((converted_base) & 0x0018);

				converted_temp = (byte)(B_data >> 3);
				converted_temp |= (byte)(G_data >> 6);
				converted_temp |= (byte)(R_data >> 8);
				
				sLCDData1(converted_temp);		
			}

		}

		sLCDSlave(0xf0);				//slave start address setting
		sLCDSlave(0x00);
		sLCDSlave(0x10);
		sLCDSlave(0x20);
		sLCDSlave(0x30);

		for (i = 0 ; i < 80 ; i++)		//write data to master
		{
			for (j = 0; j< 128 ; j++)
			{
				converted_base = (word)(*(pbBuffer++));
				converted_base |= (word)(*(pbBuffer++) << 8);

				R_data = ((converted_base) & 0xe000);
				G_data = ((converted_base) & 0x0700);
				B_data = ((converted_base) & 0x0018);

				converted_temp = (byte)(B_data >> 3);
				converted_temp |= (byte)(G_data >> 6);
				converted_temp |= (byte)(R_data >> 8);

				sLCDData2(converted_temp);		
			}

		}
	}
}

//256 color display를 하기전에 반드시 SKY_256DisplayStart()함수를 호출한다. 
//256 color display가 끝난 다음에는 반드시 SKY_256DisplayEnd()를 호출한다. 
void SKY_256DisplayStart(void)		
{
	int j;

	sLCDCnf(xind);
	sLCDCnf(0xff);

	sLCDCtl(0x25);	
	sLCDCtl(clkk);
	
	sLCDCtl(0x34);	
	sLCDCtl(clkk);

	clk_busy_wait(30000);	

	sLCDMaster(0xf4);	
	sLCDMaster(0x90);	

	sLCDCtl(0x26);
	sLCDCtl(0x00);

	sLCDCtl(0x34);	
	sLCDCtl(0x00);

	clk_busy_wait(50000);

/*
---------------------------------------------------------------------------------------------
	FGC HALT	DEN bit Reset, HALT Set And 256 color palette
---------------------------------------------------------------------------------------------
*/
	/*
	Master palette setting
	*/
	sLCDMaster(0xf1);	//Master Side Common Bank1 set
	sLCDMaster(0xf1);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDMaster(0x41);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDMaster(0x61);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDMaster(0x71);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDMaster(0x86);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDMaster(0xa6);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDMaster(0xce);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDMaster(0xf2);	//Master Side Common Bank2 set
	sLCDMaster(0xf2);	//dummy

	sLCDMaster(0x0e);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDMaster(0x49);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDMaster(0x6f);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDMaster(0x82);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDMaster(0xa5);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDMaster(0xc7);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDMaster(0xf3);	//Master Side Common Bank3 set
	sLCDMaster(0xf3);	//dummy

	sLCDMaster(0x09);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDMaster(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDMaster(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDMaster(0x69);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDMaster(0x8e);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDMaster(0xa2);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDMaster(0xc5);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDMaster(0xf4);	//Master Side Common Bank4 set
	sLCDMaster(0xf4);	//dummy

	sLCDMaster(0x07);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDMaster(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDMaster(0x2a);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDMaster(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette C7  set(upper nibble)

	/*
	slave palette setting
	*/

	sLCDSlave(0xf1);	//Master Side Common Bank1 set
	sLCDSlave(0xf1);	//dummy

	sLCDSlave(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDSlave(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDSlave(0x41);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDSlave(0x51);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDSlave(0x61);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDSlave(0x71);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDSlave(0x86);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDSlave(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDSlave(0xa6);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDSlave(0xce);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDSlave(0xf2);	//Master Side Common Bank2 set
	sLCDSlave(0xf2);	//dummy

	sLCDSlave(0x0e);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDSlave(0x11);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDSlave(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDSlave(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDSlave(0x49);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDSlave(0x6f);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDSlave(0x82);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDSlave(0x91);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDSlave(0xa5);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDSlave(0xc7);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDSlave(0xf3);	//Master Side Common Bank3 set
	sLCDSlave(0xf3);	//dummy

	sLCDSlave(0x09);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDSlave(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDSlave(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDSlave(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDSlave(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDSlave(0x69);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDSlave(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDSlave(0x8e);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDSlave(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDSlave(0xa2);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDSlave(0xb1);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDSlave(0xc5);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDSlave(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDSlave(0xf4);	//Master Side Common Bank4 set
	sLCDSlave(0xf4);	//dummy

	sLCDSlave(0x07);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDSlave(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDSlave(0x2a);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDSlave(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDSlave(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDSlave(0x51);	//Master Side Gradation palette C7  set(upper nibble)

	sLCDCtl(0x36);	
	sLCDCtl(0x00);

//	BIO_OUT(ENC_GPIO_1, BIO_64KCOLOR_LCD_M, BIO_64KCOLOR_LCD_LO_V);

//	for(j=0;j<65535;j++)
//		dog_kick(); 	

}


void SKY_256DisplayEnd(void)		//256 dispaly를 끝낸후에 반드시 이 함수를 호출한다. 
{
	word *cs;
	volatile word read;			//for reset signal 

//	BIO_OUT(ENC_GPIO_1, BIO_64KCOLOR_LCD_M, BIO_64KCOLOR_LCD_HI_V);

//	clk_busy_wait(30000);
/*	
	if(bPartial_Off_Flag == 0xff)	// 처음 들어온 경우 
		{
			if(NV_DONE_S != SKY_GetNV (NV_PA_RANGE_STEP_CAL_I, (nv_item_type *)&bPartial_Off_Flag))
				bPartial_Off_Flag = 0;
		}
*/		
	  if(bPartial_Off_Flag == 0)		// reset이용 partial out   by jnlee 01.10.09
	  {
		SKY_LCDReset();
		SKY_ReInitLCD();
	  }
	else{
	cs = (word *)0x2000000;
	read = *cs;
	clk_busy_wait(10);	 	

	sLCDCtl(0x24);		
	sLCDCtl(clkk);

	sLCDCtl(0x24);		
	sLCDCtl(clkk);

	sLCDMaster(0xf4);	
	sLCDMaster(0x92);
	clk_busy_wait(10);

	sLCDCnf(xind);
	sLCDCnf(0x00);

/* 
---------------------------------------------------------------------------------------------
	Gradation palette Set : 65K palette setting Master And Slave	
---------------------------------------------------------------------------------------------
*/
 	sLCDCtl(0x2c);		//2000.05.10 BOTH = 'H', EDIV(1/6) = '00111', MDIV(1/1) = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);

	clk_busy_wait(50000);	//2000.05.10 50ms wait

	sLCDMaster(0xf1);	//Master Side Common Bank1 set
	sLCDMaster(0xf1);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A0 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A0 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette A1 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette A1 set(upper nibble)
	sLCDMaster(0x46);	//Master Side Gradation palette A2 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette A2 set(upper nibble)
	sLCDMaster(0x60);	//Master Side Gradation palette A3 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette A3 set(upper nibble)
	sLCDMaster(0x8a);	//Master Side Gradation palette A4 set(lower nibble)
	sLCDMaster(0x91);	//Master Side Gradation palette A4 set(upper nibble)
	sLCDMaster(0xa0);	//Master Side Gradation palette A5 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette A5 set(upper nibble)
	sLCDMaster(0xcf);	//Master Side Gradation palette A6 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette A6 set(upper nibble)

	sLCDMaster(0xf2);	//Master Side Common Bank2 set
	sLCDMaster(0xf2);	//dummy

	sLCDMaster(0x00);	//Master Side Gradation palette A7 set(lower nibble)
	sLCDMaster(0x10);	//Master Side Gradation palette A7 set(upper nibble)
	sLCDMaster(0x20);	//Master Side Gradation palette B0 set(lower nibble)
	sLCDMaster(0x30);	//Master Side Gradation palette B0 set(upper nibble)
	sLCDMaster(0x41);	//Master Side Gradation palette B1 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette B1 set(upper nibble)
	sLCDMaster(0x64);	//Master Side Gradation palette B2 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette B2 set(upper nibble)
	sLCDMaster(0x88);	//Master Side Gradation palette B3 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette B3 set(upper nibble)
	sLCDMaster(0xa0);	//Master Side Gradation palette B4 set(lower nibble)
	sLCDMaster(0xb1);	//Master Side Gradation palette B4 set(upper nibble)
	sLCDMaster(0xca);	//Master Side Gradation palette B5 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette B5 set(upper nibble)
	
	sLCDMaster(0xf3);	//Master Side Common Bank3 set
	sLCDMaster(0xf3);	//dummy

	sLCDMaster(0x0d);	//Master Side Gradation palette B6 set(lower nibble)
	sLCDMaster(0x11);	//Master Side Gradation palette B6 set(upper nibble)
	sLCDMaster(0x2f);	//Master Side Gradation palette B7 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette B7  set(upper nibble)
	sLCDMaster(0x40);	//Master Side Gradation palette C0 set(lower nibble)
	sLCDMaster(0x50);	//Master Side Gradation palette C0  set(upper nibble)
	sLCDMaster(0x61);	//Master Side Gradation palette C1 set(lower nibble)
	sLCDMaster(0x70);	//Master Side Gradation palette C1  set(upper nibble)
	sLCDMaster(0x84);	//Master Side Gradation palette C2 set(lower nibble)
	sLCDMaster(0x90);	//Master Side Gradation palette C2  set(upper nibble)
	sLCDMaster(0xa8);	//Master Side Gradation palette C3 set(lower nibble)
	sLCDMaster(0xb0);	//Master Side Gradation palette C3  set(upper nibble)
	sLCDMaster(0xc0);	//Master Side Gradation palette C4 set(lower nibble)
	sLCDMaster(0xd1);	//Master Side Gradation palette C4  set(upper nibble)
	
	sLCDMaster(0xf4);	//Master Side Common Bank4 set
	sLCDMaster(0xf4);	//dummy

	sLCDMaster(0x0a);	//Master Side Gradation palette C5 set(lower nibble)	
	sLCDMaster(0x11);	//Master Side Gradation palette C5  set(upper nibble)
	sLCDMaster(0x2d);	//Master Side Gradation palette C6 set(lower nibble)
	sLCDMaster(0x31);	//Master Side Gradation palette C6  set(upper nibble)
	sLCDMaster(0x4f);	//Master Side Gradation palette C7 set(lower nibble)
	sLCDMaster(0x51);	//Master Side Gradation palette C7  set(upper nibble)

//65k dispaly 
	sLCDCtl(0x25);	
	sLCDCtl(clkk);
	}
}

//256 color display를 하기전에 반드시 SKY_256DisplayStart()함수를 호출한다. 
//256 color display가 끝난 다음에는 반드시 SKY_256DisplayEnd()를 호출한다. 
void SKY_256ColorDisplay(BYTE* directbuffer)  
{	
	register unsigned int i,j;

#ifdef WIN32
    return;
#endif
	
	sLCDMaster(0xf0);		
	sLCDMaster(0x00);
	sLCDMaster(0x10);
	sLCDMaster(0x20);
	sLCDMaster(0x30);

	for (i = 0 ; i < 80 ; i++)				//write data to master
	{
		for (j = 0; j< 4 ; j++)
		{		
			sLCDData1(*(directbuffer++));		
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));

			sLCDData1(*(directbuffer++));		
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));

			sLCDData1(*(directbuffer++));		
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));

			sLCDData1(*(directbuffer++));
			sLCDData1(*(directbuffer++));
		}

	}

	sLCDSlave(0xf0);		
	sLCDSlave(0x00);
	sLCDSlave(0x10);
	sLCDSlave(0x20);
	sLCDSlave(0x30);

	for (i = 0 ; i < 80 ; i++)				//write data to master
	{
		for (j = 0; j < 4; j++)
		{
			sLCDData2(*(directbuffer++));		
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));

			sLCDData2(*(directbuffer++));		
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));

			sLCDData2(*(directbuffer++));		
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));

			sLCDData2(*(directbuffer++));
			sLCDData2(*(directbuffer++));
		}

	}
}

void lcd_message(char *msg)
{
    extern void dog_kick(void);


#ifdef SW_RESET_ENABLE // by jnlee 01.08.22
  SW_Reset();
#endif

    _disable(); 

    SetTextWrap(TRUE);
    TextOutStrTDM(0, 0, (const unsigned char*)msg, TDM_REVERSE);
    SKY_UpdateScreen();
    /* infinite loop */
    while (1)
    {
        dog_kick();
    }
}
#endif

/* -------------------- */

LOCAL void sSetLCDAddress(BYTE bStartColumn, BYTE bEndColumn, 
                          BYTE bStartPage, BYTE bEndPage)
{
#ifndef FEATURE_64KCOLOR_LCD
	/*
		column = X,	page = line, Y
		
        이 함수는 LCD 화면중 원하는 부분의 주소를 설정한다.  

	*/
	BIO_OUT ( DEC_GPIO_0, BIO_LCD_SEL_M, 0);	//color LCD select
	
	// column address set
	sLCDCommand(COLUMN_ADDRESS_SET);
#ifndef LCD_CONVERT_TEMPORARY
	sLCDData(0);	
	sLCDData(119);	
#else
	sLCDData(StartColumn);
	sLCDData(EndColumn);	
#endif


	// page address set
	sLCDCommand(PAGE_ADDRESS_SET);
#ifndef LCD_CONVERT_TEMPORARY
	sLCDData(0);	
	sLCDData(119);	
#else
	sLCDData(StartPage);
	sLCDData(EndPage);
#endif
#endif
}

#ifdef FEATURE_PARTIAL_DISPLAY
//buffer가 x=0 ~ x=128까지의 값을 가질때 
void SKY_UpdatePartialScreen(BYTE *pbBuffer, BYTE nXpos, BYTE nYpos, BYTE nXendpos, BYTE nYendpos)
{
	int	x, y;
	
#ifdef FEATURE_WAVELET // yoursun 01/08/23
	BYTE bR, bG, bB;
    WORD wColor;
	extern BYTE g_bStride;
#endif//FEATURE_WAVELET

	//1. y의 시작과 끝점 모두 master에 있는 경우
	if(nYpos < 80 && nYendpos < 80 )		
	{
		for (y=nYpos; y<= nYendpos; y++) 
		{
			sLCDAdr(nXpos);			//start address x-axis
			sLCDAdr(y);				//start address y-axis

			if (y!=nYpos)
			{
#ifdef FEATURE_WAVELET
//				if (SKY_IS_IN_WAVELET() == TRUE) pbBuffer += nXpos;
//				else pbBuffer+= nXpos*2;
				if (SKY_IS_IN_WAVELET() == FALSE) pbBuffer += nXpos*2;
				else pbBuffer += g_bStride *2;
#else //FEATURE_WAVELET
				pbBuffer+= nXpos*2;
#endif//FEATURE_WAVELET
			}
#ifdef FEATURE_WAVELET				
			if (SKY_IS_IN_WAVELET())
			{
				for(x = nXpos; x < nXendpos ; x++)
				{
					// yoursun 01/08/23				
					bR = (*pbBuffer >> 5) << 5;//* 32;
					bG = ((*pbBuffer >> 2) & 0x07) << 5;//* 32;
					bB = (*pbBuffer & 0x03) << 6;//* 64;

					wColor = RGB565(bR, bG, bB);
					sLCDDrg(LOBYTE(wColor));
					sLCDDrg(HIBYTE(wColor));
					pbBuffer++;
/*					sLCDDrg((BYTE)0);
					sLCDDrg(*(pbBuffer++));
*/
				}
			}
			else 
			{
				for(x = nXpos; x <= nXendpos ; x++)
				{
					sLCDDrg(*(pbBuffer++));
					sLCDDrg(*(pbBuffer++));
				}
			}
#else //FEATURE_WAVELET
			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));
			}
#endif//FEATURE_WAVELET

#ifdef FEATURE_WAVELET
//			if (SKY_IS_IN_WAVELET()) pbBuffer+=(119-nXendpos);
//			else pbBuffer+=(127-nXendpos)*2;
			if (SKY_IS_IN_WAVELET()==FALSE) pbBuffer+=(127-nXendpos)*2;
#else //FEATURE_WAVELET
			pbBuffer+=(127-nXendpos)*2;
#endif//FEATURE_WAVELET
		}
	}

	//2. y시작점은 master에 있고 y끝점은 slave에 있는 경우
	else if(nYpos < 80 && nYendpos >= 80)		
	{
		for (y=nYpos; y< 80; y++)	//display data to master 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			if (y!=nYpos)
			{
#ifdef FEATURE_WAVELET
				if (SKY_IS_IN_WAVELET() == FALSE) pbBuffer += nXpos*2;
				else pbBuffer += g_bStride *2;
#else //FEATURE_WAVELET
				pbBuffer+= nXpos*2;
#endif//FEATURE_WAVELET
			}
#ifdef FEATURE_WAVELET				
			if (SKY_IS_IN_WAVELET())
			{
				for(x = nXpos; x < nXendpos ; x++)
				{
					// yoursun 01/08/23				
					bR = (*pbBuffer >> 5) << 5;//* 32;
					bG = ((*pbBuffer >> 2) & 0x07) << 5;//* 32;
					bB = (*pbBuffer & 0x03) << 6;//* 64;

					wColor = RGB565(bR, bG, bB);
					sLCDDrg(LOBYTE(wColor));
					sLCDDrg(HIBYTE(wColor));
					pbBuffer++;
/*					sLCDDrg((BYTE)0);
					sLCDDrg(*(pbBuffer++));
*/
				}
			}
			else 
			{
				for(x = nXpos; x <= nXendpos ; x++)
				{
					sLCDDrg(*(pbBuffer++));
					sLCDDrg(*(pbBuffer++));
				}
			}
#else //FEATURE_WAVELET
			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));
			}
#endif//FEATURE_WAVELET

#ifdef FEATURE_WAVELET
			if (SKY_IS_IN_WAVELET()==FALSE) pbBuffer+=(127-nXendpos)*2;
#else //FEATURE_WAVELET
			pbBuffer+=(127-nXendpos)*2;
#endif//FEATURE_WAVELET
		}
		
		for (y=82; y<= nYendpos+2; y++)	//display data to slave
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

//			if (y!=82)
//			{
#ifdef FEATURE_WAVELET
				if (SKY_IS_IN_WAVELET() == FALSE) pbBuffer += nXpos*2;
				else pbBuffer += g_bStride *2;
#else //FEATURE_WAVELET
				pbBuffer+= nXpos*2;
#endif//FEATURE_WAVELET
//			}
#ifdef FEATURE_WAVELET				
			if (SKY_IS_IN_WAVELET())
			{
				for(x = nXpos; x < nXendpos ; x++)
				{
					// yoursun 01/08/23				
					bR = (*pbBuffer >> 5) << 5;//* 32;
					bG = ((*pbBuffer >> 2) & 0x07) << 5;//* 32;
					bB = (*pbBuffer & 0x03) << 6;//* 64;

					wColor = RGB565(bR, bG, bB);
					sLCDDrg(LOBYTE(wColor));
					sLCDDrg(HIBYTE(wColor));
					pbBuffer++;
/*					sLCDDrg((BYTE)0);
					sLCDDrg(*(pbBuffer++));
*/
				}
			}
			else 
			{
				for(x = nXpos; x <= nXendpos ; x++)
				{
					sLCDDrg(*(pbBuffer++));
					sLCDDrg(*(pbBuffer++));
				}
			}
#else //FEATURE_WAVELET
			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));
			}
#endif//FEATURE_WAVELET

#ifdef FEATURE_WAVELET
			if (SKY_IS_IN_WAVELET()==FALSE) pbBuffer+=(127-nXendpos)*2;
#else //FEATURE_WAVELET
			pbBuffer+=(127-nXendpos)*2;
#endif//FEATURE_WAVELET
		}
	}

	//3. y시작점과 끝점 모두 slave에 있는 경우
	else if(nYpos >= 80 && nYendpos >= 80)		
	{
		for (y=nYpos+2; y<= nYendpos+2; y++) 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			if (y!=nYpos+2)
			{
#ifdef FEATURE_WAVELET
				if (SKY_IS_IN_WAVELET() == FALSE) pbBuffer += nXpos*2;
				else pbBuffer += g_bStride *2;
#else //FEATURE_WAVELET
				pbBuffer+= nXpos*2;
#endif//FEATURE_WAVELET
			}
#ifdef FEATURE_WAVELET				
			if (SKY_IS_IN_WAVELET())
			{
				for(x = nXpos; x < nXendpos ; x++)
				{
					// yoursun 01/08/23				
					bR = (*pbBuffer >> 5) << 5;//* 32;
					bG = ((*pbBuffer >> 2) & 0x07) << 5;//* 32;
					bB = (*pbBuffer & 0x03) << 6;//* 64;

					wColor = RGB565(bR, bG, bB);
					sLCDDrg(LOBYTE(wColor));
					sLCDDrg(HIBYTE(wColor));
					pbBuffer++;
/*					sLCDDrg((BYTE)0);
					sLCDDrg(*(pbBuffer++));
*/
				}
			}
			else 
			{
				for(x = nXpos; x <= nXendpos ; x++)
				{
					sLCDDrg(*(pbBuffer++));
					sLCDDrg(*(pbBuffer++));
				}
			}
#else //FEATURE_WAVELET
			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));
			}
#endif//FEATURE_WAVELET

#ifdef FEATURE_WAVELET
			if (SKY_IS_IN_WAVELET()==FALSE) pbBuffer+=(127-nXendpos)*2;
#else //FEATURE_WAVELET
			pbBuffer+=(127-nXendpos)*2;
#endif//FEATURE_WAVELET
		}
	}
}

//buffer가 updatescreen 할 만큼만의 x값을 가질때  
void SKY_AdjustUpdatePartialScreen(BYTE *pbBuffer, BYTE nXpos, BYTE nYpos, BYTE nXendpos, BYTE nYendpos)
{
	int	x, y;
	
	//1. y의 시작과 끝점 모두 master에 있는 경우
	if(nYpos < 80 && nYendpos < 80 )		
	{
		for (y=nYpos; y<= nYendpos; y++) 
		{
			sLCDAdr(nXpos);			//start address x-axis
			sLCDAdr(y);				//start address y-axis

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));
			}
		}
	}

	//2. y시작점은 master에 있고 y끝점은 slave에 있는 경우
	else if(nYpos < 80 && nYendpos >= 80)		
	{
		for (y=nYpos; y< 80; y++)	//display data to master 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));
			}
		}
		
		for (y=82; y<= nYendpos+2; y++)	//display data to slave
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));
			}
		}
	}

	//3. y시작점과 끝점 모두 slave에 있는 경우
	else if(nYpos >= 80 && nYendpos >= 80)		
	{
		for (y=nYpos+2; y<= nYendpos+2; y++) 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));
			}
		}
	}

}

void SKY_UpdatePartialScreenLine(BYTE *pbBuffer, BYTE nYpos, BYTE nYendpos)
{
	int	x, y;
	
	//1. y의 시작과 끝점 모두 master에 있는 경우
	if(nYpos < 80 && nYendpos < 80 )		
	{
		for (y=nYpos; y<= nYendpos; y++) 
		{
			sLCDAdr(0x00);			
			sLCDAdr(y);

			for(x=0; x<2; x++)
			{			
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));	
			}
		}
	}

	//2. y시작점은 master에 있고 y끝점은 slave에 있는 경우
	else if(nYpos < 80 && nYendpos >= 80)		
	{
		for (y=nYpos; y< 80; y++)	//display data to master 
		{
			sLCDAdr(0x00);
			sLCDAdr(y);		

			for(x=0; x<2; x++)
			{
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			}
		}
		
		for (y=82; y<= nYendpos+2; y++)	//display data to slave
		{
			sLCDAdr(0x00);
			sLCDAdr(y);	

			for(x=0; x<2; x++)
			{
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));	
			}
		}
	}

	//3. y시작점과 끝점 모두 slave에 있는 경우
	else if(nYpos >= 80 && nYendpos >= 80)		
	{
		for (y=nYpos+2; y<= nYendpos+2; y++) 
		{
			sLCDAdr(0x00);
			sLCDAdr(y);	

			for(x=0; x<2; x++)
			{					
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));

				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));	
			}
		}
	}

}

void SKY_UpdatePartialDisplayRect(BYTE *pbBuffer, UINT8 nXpos, UINT8 nYpos, UINT8 nXendpos, UINT8 nYendpos)
{
	int	x, y;

	BYTE* pbSrc;

	sLCDCnf(xind);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'(1/2)
	sLCDCnf(0x00);

	sLCDAdr(0x00);
	sLCDAdr(0x00);

	sLCDCtl(0x25);		//BOTH = 'L', EDIV = '00101', MDIV = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);
		
	//1. y의 시작과 끝점 모두 master에 있는 경우
	if(nYpos < 80 && nYendpos < 80 )		
	{		
		for (y=nYpos; y<= nYendpos; y++) 
		{
			sLCDAdr(nXpos);			//start address x-axis
			sLCDAdr(y);				//start address y-axis

            pbSrc = pbBuffer + 128 *2 *y + nXpos * 2;
			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbSrc++));
				sLCDDrg(*(pbSrc++));
			}
		    
		}
	}

	//2. y시작점은 master에 있고 y끝점은 slave에 있는 경우
	else if(nYpos < 80 && nYendpos >= 80)		
	{
		for (y=nYpos; y< 80; y++)	//display data to master 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			pbSrc = pbBuffer + 128 *2 *y + nXpos * 2;
			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbSrc++));
				sLCDDrg(*(pbSrc++));
			}		

		}
		for (y=80; y< 82; y++)		//dummy data
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(0xff);
				sLCDDrg(0xff);
			}
		}
		for (y=82; y<= nYendpos+2; y++)	//display data to slave
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			pbSrc = pbBuffer + 128 *2 *(y-2) + nXpos * 2;

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbSrc++));
				sLCDDrg(*(pbSrc++));
			}			
		}
	}

	//3. y시작점과 끝점 모두 slave에 있는 경우
	else if(nYpos >= 80 && nYendpos >= 80)		
	{
		for (y=nYpos+2; y<= nYendpos+2; y++) 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			pbSrc = pbBuffer + 128 *2 *(y-2) + nXpos * 2;
			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbSrc++));
				sLCDDrg(*(pbSrc++));
			}			
		}
	}
}


void SKY_UpdatePartialDisplayRectByRegion(BYTE *pbBuffer, UINT8 nXpos, UINT8 nYpos, UINT8 nXendpos, UINT8 nYendpos)
{
	int	x, y;

	BYTE* pbSrc;

	sLCDCnf(xind);	//WDEL = '000', FORCE = 'L', 8bit I/F, XIND = 'H'(1/2)
	sLCDCnf(0x00);

	sLCDAdr(0x00);
	sLCDAdr(0x00);

	sLCDCtl(0x25);		//BOTH = 'L', EDIV = '00101', MDIV = '000', AAC = 'H', CKCTL = 'H'
	sLCDCtl(clkk);


	pbSrc = pbBuffer;
		
	//1. y의 시작과 끝점 모두 master에 있는 경우
	if(nYpos < 80 && nYendpos < 80 )		
	{		
		for (y=nYpos; y<= nYendpos; y++) 
		{
			sLCDAdr(nXpos);			//start address x-axis
			sLCDAdr(y);				//start address y-axis

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbSrc++));
				sLCDDrg(*(pbSrc++));
			}
		    
		}
	}

	//2. y시작점은 master에 있고 y끝점은 slave에 있는 경우
	else if(nYpos < 80 && nYendpos >= 80)		
	{
		for (y=nYpos; y< 80; y++)	//display data to master 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbSrc++));
				sLCDDrg(*(pbSrc++));
			}		

		}
/*		for (y=80; y< 82; y++)		//dummy data
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(0xff);
				sLCDDrg(0xff);
			}
		}
*/
		for (y=82; y<= nYendpos+2; y++)	//display data to slave
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbSrc++));
				sLCDDrg(*(pbSrc++));
			}			
		}
	}

	//3. y시작점과 끝점 모두 slave에 있는 경우
	else if(nYpos >= 80 && nYendpos >= 80)		
	{
		for (y=nYpos+2; y<= nYendpos+2; y++) 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			for(x = nXpos; x <= nXendpos ; x++)
			{
				sLCDDrg(*(pbSrc++));
				sLCDDrg(*(pbSrc++));
			}			
		}
	}
}
 
//256 color display를 하기전에 반드시 SKY_256DisplayStart()함수를 호출한다. 
//256 color display가 끝난 다음에는 반드시 SKY_256DisplayEnd()를 호출한다. 
void SKY_256UpdatePartialDisplay(BYTE *pbBuffer, UINT8 nYpos, UINT8 nYendpos)	
{	
	register unsigned int x, y;
	byte x_lower, x_upper, y_lower, y_upper;

#ifdef WIN32
    return;
#endif
	
	//1. y의 시작과 끝점 모두 master에 있는 경우
	
	if(nYpos < 80 && nYendpos < 80 )		
	{		
		y_lower = nYpos & 0x0f | 0x20;
		y_upper = nYpos >> 4 | 0x30;

		sLCDMaster(0xf0);
		sLCDMaster(0xf0);
		sLCDMaster(0x00);				//start address x-axis
		sLCDMaster(0x10);
		sLCDMaster(y_lower);			//start address y-axis
		sLCDMaster(y_upper);

		for (y=nYpos; y<= nYendpos; y++) 
		{		
            sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));						    
		}
	}

	//2. y시작점은 master에 있고 y끝점은 slave에 있는 경우
	else if(nYpos < 80 && nYendpos >= 80)		
	{
		y_lower = nYpos & 0x0f | 0x20;
		y_upper = nYpos >> 4 | 0x30;

		sLCDMaster(0xf0);
		sLCDMaster(0xf0);
		sLCDMaster(0x00);				//start address x-axis
		sLCDMaster(0x10);
		sLCDMaster(y_lower);			//start address y-axis
		sLCDMaster(y_upper);

		for (y=nYpos; y< 80; y++)		//display data to master 
		{				
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));	

		}
		
		sLCDSlave(0xf0);
		sLCDSlave(0xf0);
		sLCDSlave(0x00);				//start address x-axis
		sLCDSlave(0x10);
		sLCDSlave(0x20);				//start address y-axis
		sLCDSlave(0x30);	

		for (y=80; y<= nYendpos; y++)	//display data to slave
		{
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				
		
		}
	}

	//3. y시작점과 끝점 모두 slave에 있는 경우
	else if(nYpos >= 80 && nYendpos >= 80)		
	{
		y_lower = (nYpos-80) & 0x0f | 0x20;
		y_upper = (nYpos-80) >> 4 | 0x30;

		sLCDSlave(0xf0);
		sLCDSlave(0xf0);
		sLCDSlave(0x00);				//start address x-axis
		sLCDSlave(0x10);
		sLCDSlave(y_lower);			//start address y-axis
		sLCDSlave(y_upper);

		for (y=nYpos; y<= nYendpos; y++) 
		{
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
		
		}
	}		
}
#endif

void SKY_256UpdatePartialScreenWithoutAnnu(BYTE *pbBuffer)
{
	register unsigned int y;

#ifdef WIN32
    return;
#endif
		sLCDMaster(0xf0);
		sLCDMaster(0xf0);
		sLCDMaster(0x00);				//start address x-axis
		sLCDMaster(0x10);
		sLCDMaster(0x20);				//start address y-axis
		sLCDMaster(0x31);

		for (y=16; y< 80; y++)		//display data to master 
		{				
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));

			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));
			sLCDData1(*(pbBuffer++));				sLCDData1(*(pbBuffer++));	

		}
		
		sLCDSlave(0xf0);
		sLCDSlave(0xf0);
		sLCDSlave(0x00);				//start address x-axis
		sLCDSlave(0x10);
		sLCDSlave(0x20);				//start address y-axis
		sLCDSlave(0x30);	

		for (y = 0; y < 80; y++)	//display data to slave
		{
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));

			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));
			sLCDData2(*(pbBuffer++));				sLCDData2(*(pbBuffer++));				
		
	}
}


#define PROGRESS_BAR_X  16
#define PROGRESS_BAR_Y  141
#define PROGRESS_UNIT_X 3
#define PROGRESS_UNIT_Y 5

void SKY_BootProgressDisplay(UINT8  nStep)  // Step : 0 ~ 33
{
    const byte* pbBuffer = g_BmpBootProgressBar.pbData;
    UINT8  i;

#ifdef SW_RESET_ENABLE
	if(mc_bSwReset)
	{
        return;
    }
#endif
    if(nStep == 0)  return;
    nStep --;

#ifdef FEATURE_PARTIAL_DISPLAY
	SKY_AdjustUpdatePartialScreen
		((BYTE *)pbBuffer, 
		PROGRESS_BAR_X + nStep * PROGRESS_UNIT_X, 
		PROGRESS_BAR_Y, 
		PROGRESS_BAR_X + (nStep+1) * PROGRESS_UNIT_X -1, 
		PROGRESS_BAR_Y+PROGRESS_UNIT_Y-1);
#endif	//#ifdef FEATURE_PARTIAL_DISPLAY

}


/* only for LCD contrast test !!!!!!! do not use this function in UI */
void SKY_API SKY_SetLCDContrastTest(BYTE bContrast)
{
#ifdef FEATURE_64KCOLOR_LCD
	byte cont_a,cont_b;

	sLCDCtl(0x34);		//WB = 'H', DEN ='L', BOTH = 'H'
	sLCDCtl(clkk);

	clk_busy_wait(30000);

	cont_a = ((bContrast >> 4) & 0x0f)|0xb0;
	cont_b = (bContrast & 0x0f)|0xa0;
		
	sLCDMaster(0xf4);	//Master Side  Bank4	
	sLCDMaster(cont_a);	//Master Side  Electronic Volume(upper nibble)DV[6:4] = "101"
	sLCDMaster(cont_b);	//Master Side  Electronic Volume(lower nibble)DV[3:0] = "1001"
	sLCDMaster(0xf0);	//Master Side  Bank0
	
	sLCDCtl(0x25);		//WB = 'H', DEN ='L', BOTH = 'H'
	sLCDCtl(clkk);
	
#else
    sLCDCommand(VOLUME_CONTROL);
    sLCDData(bContrast);      /* step value */
#endif
}

LOCAL void SKY_API sSetLCDContrast(LCD_CONTRAST_UPDOWN_E UpDown, BYTE bValue)
{
#ifdef FEATURE_64KCOLOR_LCD
	byte cont_a,cont_b;

	sLCDCtl(0x34);		//DEN ='L', BOTH = 'H'
	sLCDCtl(clkk);

	cont_a = ((bValue >> 4) & 0x0f)|0xb0;
	cont_b = (bValue & 0x0f)|0xa0;
		
	sLCDMaster(0xf4);	//Master Side  Bank4	
	sLCDMaster(0xf4);	//Master Side  Bank4	
	sLCDMaster(cont_a);	//Master Side  Electronic Volume(upper nibble)DV[6:4] = "101"
	sLCDMaster(cont_b);	//Master Side  Electronic Volume(lower nibble)DV[3:0] = "1001"
	
#else
    bValue &= 0x1F; /* make value to 0 ~ 31 */
    sLCDCommand(VOLUME_CONTROL);
    sLCDData(UpDown | bValue);      /* step value */
#endif
} /* end of hs_lcd_contrast */

/*===========================================================================

EXTERN FUNCTION HS_LCD_CONTRAST

DESCRIPTION
  This function controls the contrast state of the display.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  HS_LCD_INIT이 실행 되면 다시 처음의 Default value로 변환됨.

===========================================================================*/
void SKY_API SKY_SetLCDContrast(HS_CONTRAST_E Contrast, BYTE ContrastLevel)
{
#ifdef FEATURE_64KCOLOR_LCD
	byte cont,cont_a,cont_b;

	if(Contrast >= HCL_D)
	{
		cont = ContrastLevel + LCD_CONTAST_STEP * (Contrast-HCL_D);
	}
	else
	{
		cont = ContrastLevel - LCD_CONTAST_STEP * (HCL_D-Contrast);
	}

	sLCDCtl(0x34);		//WB = 'H', DEN ='L', BOTH = 'H'
	sLCDCtl(clkk);

	clk_busy_wait(30000);

	cont_a = ((cont >> 4) & 0x0f)|0xb0;
	cont_b = (cont & 0x0f)|0xa0;
		
	sLCDMaster(0xf4);	//Master Side  Bank4	
	sLCDMaster(cont_a);	//Master Side  Electronic Volume(upper nibble)DV[6:4] = "101"
	sLCDMaster(cont_b);	//Master Side  Electronic Volume(lower nibble)DV[3:0] = "1001"
	sLCDMaster(0xf0);	//Master Side  Bank0
	
	sLCDCtl(0x25);		//WB = 'H', DEN ='L', BOTH = 'H'
	sLCDCtl(clkk);
#else
    INT8 nCount;
    STATIC HS_CONTRAST_E l_CurrentContrast = INIT_CONTRAST;
	STATIC BYTE l_CurrentContrastLevel = LCD_CONTRAST_DEFAULT&0x1F;

    /* get the difference between the initial and saved contrast value */
    nCount = (Contrast - l_CurrentContrast)+ (ContrastLevel-l_CurrentContrastLevel);
    
    /* the value to set is same with default value, just do nothing */
//    if(nCount == 0)
//        return;

    if(nCount > 0)
    {
        /* up contrast level */
//        while(nCount--)
        sSetLCDContrast(LCU_UP, nCount);
    }
    else
    {
        /* down contast level */
//        while(nCount++)
        sSetLCDContrast(LCU_DOWN, ABS(nCount));
    }

    l_CurrentContrast = Contrast;
	l_CurrentContrastLevel = ContrastLevel;
#endif
} /* end of hs_lcd_contrast */

void SKY_UpdatePartialScreenWithoutAnnu(BYTE *pbBuffer)
{
	int	x, y;
	
		sLCDAdr(0x00);
		sLCDAdr(0x10);		

		for(y = 16; y < 80; y++)		//display data to master
		{
			for(x = 0; x < 2; x++ ){
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));}
		}

		sLCDAdr(0x00);				
		sLCDAdr(0x52);
		
		for (y = 0; y < 80*2; y++)	//display data to slave
		{
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));	
		}
}

void SKY_UpdatePartialScreenWavelet(BYTE *pbBuffer, UINT8 nXpos, UINT8 nYpos, UINT8 nXendpos, UINT8 nYendpos)
{
	int	x, y;
	register BYTE* pbSrc;
	pbSrc = pbBuffer;

	for (y=nYpos; y< 80; y++)	//display data to master 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));
		}

	for (y=80; y<= nYendpos; y++)	//display data to slave
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));	
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));	
		}
}
void SKY_UpdatePartialScreenWaveletEx(BYTE *pbBuffer, UINT8 nXpos, UINT8 nYpos, UINT8 nXendpos, UINT8 nYendpos)
{
	int	x, y;
	register BYTE* pbSrc;
	pbSrc = pbBuffer;

	for (y=nYpos; y< 80; y++)	//display data to master 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));	
		}

	for (y=82; y<= nYendpos+2; y++)	//display data to slave
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));
			sLCDDrg(*(pbSrc++));

			sLCDDrg(*(pbSrc++));				sLCDDrg(*(pbSrc++));	
		}
}
void SKY_UpdatePartialScreenWaveletText(BYTE *pbBuffer, UINT8 nXpos, UINT8 nYpos, UINT8 nXendpos, UINT8 nYendpos)
{
	int	x, y;

	for (y=nYpos; y< nYendpos; y++)	//display data to master 
		{
			sLCDAdr(nXpos);
			sLCDAdr(y);

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));				sLCDDrg(*(pbBuffer++));
			sLCDDrg(*(pbBuffer++));

			pbBuffer+= 16;
		}
}
//LCD test pattern
void fill_rect(void)
{
	int i, j;		
	register byte *ctemp;
	ctemp = tempp;

	for (j = 0 ; j<32 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<32*2 ; j++)				
	{
		for (i = 0; i< 40 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 40*2 ; i++)
		{
			*ctemp = 0;
			ctemp++;
			*ctemp = 0;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<32*3 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<32*4 ; j++)				
	{
		for (i = 0; i< 40 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 40*2 ; i++)
		{
			*ctemp = 0xff;
			ctemp++;
			*ctemp = 0xff;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<164 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}
	ctemp = tempp;
}

void fill_rect_black(void)
{
	int i, j;		
	register byte *ctemp;
	ctemp = tempp;

	for (j = 0 ; j<40 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<120; j++)				
	{
		for (i = 0; i< 40 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 80 ; i++)
		{
			*ctemp = 0;
			ctemp++;
			*ctemp = 0;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<164 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}
	ctemp = tempp;
}

void fill_rect_round_black(void)
{
	int i, j;		
	register byte *ctemp;
	ctemp = tempp;

	for (j = 0 ; j<40 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<41; j++)				
	{
		for (i = 0; i< 41 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 79 ; i++)
		{
			*ctemp = 0;
			ctemp++;
			*ctemp = 0;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<120; j++)				
	{
		for (i = 0; i< 40 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 80 ; i++)
		{
			*ctemp = 0;
			ctemp++;
			*ctemp = 0;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<121; j++)				
	{
		for (i = 0; i< 41 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 79 ; i++)
		{
			*ctemp = 0;
			ctemp++;
			*ctemp = 0;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<164 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}
	ctemp = tempp;
}

void fill_rect_white(void)
{
	int i, j;		
	register byte *ctemp;
	ctemp = tempp;

	for (j = 0 ; j<40 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<120 ; j++)				
	{
		for (i = 0; i< 40 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 80 ; i++)
		{
			*ctemp = 0xff;
			ctemp++;
			*ctemp = 0xff;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<164 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}
	ctemp = tempp;
}

void fill_rect_round_white(void)
{
	int i, j;		
	register byte *ctemp;
	ctemp = tempp;

	for (j = 0 ; j<40 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<41 ; j++)				
	{
		for (i = 0; i< 41 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 79 ; i++)
		{
			*ctemp = 0xff;
			ctemp++;
			*ctemp = 0xff;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<120 ; j++)				
	{
		for (i = 0; i< 40 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 80 ; i++)
		{
			*ctemp = 0xff;
			ctemp++;
			*ctemp = 0xff;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<121 ; j++)				
	{
		for (i = 0; i< 41 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}

		for ( ; i< 79 ; i++)
		{
			*ctemp = 0xff;
			ctemp++;
			*ctemp = 0xff;
			ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}

	for ( ; j<164 ; j++)				
	{
		for (i = 0; i< 128 ; i++)
		{
			*ctemp = 0x10;
			ctemp++;
			*ctemp = 0x84;
			ctemp++;
		}
	}
	ctemp = tempp;
}

void fill_hori_red(void)
{
	int i, j;
	register byte *ctemp;
	byte k = 0;

	ctemp = tempp;

	for(i=0; i<32; i++)
	{
		for(j=0; j<128*5; j++)
		{
		*ctemp = 0;
		ctemp++;
		*ctemp = 0x00 + (k << 3);
		ctemp++;
			if(k > 0x1f)
			{
				*ctemp = 0;
				ctemp++;
				*ctemp = 0xf8;
				ctemp++;
			}
		}
		k++;
	}
	for(; i<36; i++)
	{
		for(j=0; j<128; j++)
		{
			*ctemp = 0;
			ctemp++;
			*ctemp = 0xf8;
			ctemp++;
		}
	}
	ctemp = tempp;
}

void fill_hori_green(void)
{
	int i, j;
	register byte *ctemp;
	word green_data = 0;
	word k = 0;
	ctemp = tempp;

	for(i=0; i<32; i++)
	{
		for(j=0; j<128*5; j++)
		{
		green_data = 0x0000 + (k<<5);
		*ctemp = (byte)(green_data & 0xff);
		ctemp++;
		*ctemp = (byte)(green_data >> 8);
		ctemp++;
			if(k > 0x2f)
			{
				*ctemp = 0xe0;
				ctemp++;
				*ctemp = 0x07;
				ctemp++;
			}
		}
		k++;
	}
	for(; i<36; i++)
	{
		for(j=0; j<128; j++)
		{
			*ctemp = 0xe0;
			ctemp++;
			*ctemp = 0x07;
			ctemp++;
		}
	}
	ctemp = tempp;
}

void fill_hori_blue(void)
{
	int i, j;
	register byte *ctemp;
	byte k = 0;
	ctemp = tempp;

	for(i=0; i<32; i++)
	{
		for(j=0; j<128*5; j++)
		{
		*ctemp = 0x00 + k;
		ctemp++;
		*ctemp = 0;
		ctemp++;
			if(k > 0x1f)
			{
				*ctemp = 0x1f;
				ctemp++;
				*ctemp = 0;
				ctemp++;
			}
		}
		k++;
	}
	for(; i<36; i++)
	{
		for(j=0; j<128; j++)
		{
			*ctemp = 0x1f;
			ctemp++;
			*ctemp = 0x00;
			ctemp++;
		}
	}
	ctemp = tempp;
}

void fill_vert_red(void)
{
	int i, j, l;
	register byte *ctemp;
	byte k = 0;
	ctemp = tempp;

	for(l=0; l<164; l++)
	{
		k = 0;
		for(i=0; i<128/4; i++)
		{
			for(j=0; j<4; j++)
			{
			*ctemp = 0;
			ctemp++;
			*ctemp = 0x00 + (k << 3);
			ctemp++;
				if(k > 0x1f)
				{
					*ctemp = 0;
					ctemp++;
					*ctemp = 0xf8;
					ctemp++;
				}
			}
			k++;
		}
	}
	ctemp = tempp;
}

void fill_vert_green(void)
{
	int i, j, l;
	register byte *ctemp;
	word green_data = 0;
	word k = 0;
	ctemp = tempp;

	for(l=0; l<164; l++)
	{
		k = 0;
		for(i=0; i<128/4; i++)
		{
			for(j=0; j<4; j++)
			{
			green_data = 0x0000 + (k<<5);
			*ctemp = (byte)(green_data & 0xff);
			ctemp++;
			*ctemp = (byte)(green_data >> 8);
			ctemp++;
				if(k > 0x2f)
				{
					*ctemp = 0xe0;
					ctemp++;
					*ctemp = 0x07;
					ctemp++;
				}
			}
			k++;
		}	
	}
	ctemp = tempp;
}

void fill_vert_blue(void)
{
	int i, j, l;
	register byte *ctemp;
	byte k = 0;
	ctemp = tempp;

	for(l=0; l<164; l++)
	{
		k = 0;
		for(i=0; i<128/4; i++)
		{
			for(j=0; j<4; j++)
			{
			*ctemp = 0x00 + k;
			ctemp++;
			*ctemp = 0;
			ctemp++;
				if(k > 0x1f)
				{
					*ctemp = 0x1f;
					ctemp++;
					*ctemp = 0;
					ctemp++;
				}
			}
			k++;
		}	
	}
	ctemp = tempp;
}

void fill_vert_color(void)
{
	int j, l;
	register byte *ctemp;
	ctemp = tempp;

	for(l=0; l<164; l++)
	{
		//Red
		for(j=0; j<16; j++)
		{
			*ctemp = 0x00;
			ctemp++;
			*ctemp = 0xf8;
			ctemp++;
		}	
		//green
		for(; j<16*2; j++)
		{
			*ctemp = 0xe0;
			ctemp++;
			*ctemp = 0x07;
			ctemp++;
		}
		//blue
		for(; j<16*3; j++)
		{
			*ctemp = 0x1f;
			ctemp++;
			*ctemp = 0x00;
			ctemp++;
		}
		//orange
		for(; j<16*4; j++)
			{
			*ctemp = 0xe0;
			ctemp++;
			*ctemp = 0xff;
			ctemp++;
			}
		//purpple
		for(; j<16*5; j++)
			{
			*ctemp = 0xf8;
			ctemp++;
			*ctemp = 0x1f;
			ctemp++;
			}
		//black
		for(; j<16*6; j++)
			{
			*ctemp = 0x00;
			ctemp++;
			*ctemp = 0x00;
			ctemp++;
			}
		//white
		for(; j<16*7; j++)
			{
			*ctemp = 0xff;
			ctemp++;
			*ctemp = 0xff;
			ctemp++;
			}
		//gray
		for(; j<16*8; j++)
			{
			*ctemp = 0x77;
			ctemp++;
			*ctemp = 0x77;
			ctemp++;
			}
	}
	ctemp = tempp;
}

void fill_vert_4color(void)
{
	int j, l;
	register byte *ctemp;
	ctemp = tempp;

	for(l=0; l<164; l++)
	{
		//Red
		for(j=0; j<32; j++)
		{
			*ctemp = 0x00;
			ctemp++;
			*ctemp = 0xf8;
			ctemp++;
		}	
		//green
		for(; j<32*2; j++)
		{
			*ctemp = 0xe0;
			ctemp++;
			*ctemp = 0x07;
			ctemp++;
		}
		//blue
		for(; j<32*3; j++)
		{
			*ctemp = 0x1f;
			ctemp++;
			*ctemp = 0x00;
			ctemp++;
		}
		//black
		for(; j<32*4; j++)
			{
			*ctemp = 0x00;
			ctemp++;
			*ctemp = 0x00;
			ctemp++;
			}
	}
	ctemp = tempp;
}
void vert_gray(void) 
{
	int j, l;
	register byte *ctemp;
	
	ctemp = tempp;

	for(l=0; l<164; l++)
	{
		//black
		for(j=0; j<16; j++)
		{
			*ctemp = 0x00;
			ctemp++;
			*ctemp = 0x00;
			ctemp++;
		}	
		
		for(; j<16*2; j++)
		{
			*ctemp = 0x22;
			ctemp++;
			*ctemp = 0x22;
			ctemp++;
		}
		
		for(; j<16*3; j++)
		{
			*ctemp = 0x44;
			ctemp++;
			*ctemp = 0x44;
			ctemp++;
		}
	
		for(; j<16*4; j++)
			{
			*ctemp = 0x66;
			ctemp++;
			*ctemp = 0x66;
			ctemp++;
			}
	
		for(; j<16*5; j++)
			{
			*ctemp = 0x88;
			ctemp++;
			*ctemp = 0x88;
			ctemp++;
			}
		
		for(; j<16*6; j++)
			{
			*ctemp = 0xaa;
			ctemp++;
			*ctemp = 0xaa;
			ctemp++;
			}
		
		for(; j<16*7; j++)
			{
			*ctemp = 0xcc;
			ctemp++;
			*ctemp = 0xcc;
			ctemp++;
			}
		//white
		for(; j<16*8; j++)
			{
			*ctemp = 0xff;
			ctemp++;
			*ctemp = 0xff;
			ctemp++;
			}
	}
	ctemp = tempp;
}


void round_white_gray(void)
{
	int i, j;
	register byte *ctemp;
	
	ctemp = tempp;


	for (j = 0 ; j< 40 ; j++)		//vacance				
	{
		for (i = 0; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 45 ; j++)			//step1				
	{
		for (i = 0; i< 55 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 73 ; i++)
		{
		*ctemp=0xff;	 
		ctemp++;
		*ctemp=0xff;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 50 ; j++)			//step2				
	{
		for (i = 0; i< 50 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 78 ; i++)
		{
		*ctemp=0xff;	 
		ctemp++;
		*ctemp=0xff;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 55 ; j++)			//step3				
	{
		for (i = 0; i< 45 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 83 ; i++)
		{
		*ctemp=0xff;	 
		ctemp++;
		*ctemp=0xff;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 105 ; j++)			//vertical color				
	{
		for (i = 0; i< 40 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 88 ; i++)
		{
		*ctemp=0xff;	 
		ctemp++;
		*ctemp=0xff;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 110 ; j++)				
	{
		for (i = 0; i< 45 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 83 ; i++)
		{
		*ctemp=0xff;	 
		ctemp++;
		*ctemp=0xff;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 115 ; j++)				
	{
		for (i = 0; i< 50 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 78 ; i++)
		{
		*ctemp=0xff;	 
		ctemp++;
		*ctemp=0xff;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}
	
	for ( ; j< 120 ; j++)				
	{
		for (i = 0; i< 55 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 73 ; i++)
		{
		*ctemp=0xff;	 
		ctemp++;
		*ctemp=0xff;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 164 ; j++)				//vacance				
	{
		for (i = 0; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}
	ctemp = tempp;
}

void round_black_gray(void)
{
	int i, j;
	register byte *ctemp;
	
	ctemp = tempp;


	for (j = 0 ; j< 40 ; j++)		//vacance				
	{
		for (i = 0; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 45 ; j++)			//step1				
	{
		for (i = 0; i< 55 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 73 ; i++)
		{
		*ctemp=0x00;	 
		ctemp++;
		*ctemp=0x00;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 50 ; j++)			//step2				
	{
		for (i = 0; i< 50 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 78 ; i++)
		{
		*ctemp=0x00;	 
		ctemp++;
		*ctemp=0x00;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 55 ; j++)			//step3				
	{
		for (i = 0; i< 45 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 83 ; i++)
		{
		*ctemp=0x00;	 
		ctemp++;
		*ctemp=0x00;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 105 ; j++)			//vertical color				
	{
		for (i = 0; i< 40 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 88 ; i++)
		{
		*ctemp=0x00;	 
		ctemp++;
		*ctemp=0x00;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 110 ; j++)				
	{
		for (i = 0; i< 45 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 83 ; i++)
		{
		*ctemp=0x00;	 
		ctemp++;
		*ctemp=0x00;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 115 ; j++)				
	{
		for (i = 0; i< 50 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 78 ; i++)
		{
		*ctemp=0x00;	 
		ctemp++;
		*ctemp=0x00;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}
	
	for ( ; j< 120 ; j++)				
	{
		for (i = 0; i< 55 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}

		for ( ; i< 73 ; i++)
		{
		*ctemp=0x00;	 
		ctemp++;
		*ctemp=0x00;	
		ctemp++;
		}

		for ( ; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}

	for ( ; j< 164 ; j++)				//vacance				
	{
		for (i = 0; i< 128 ; i++)
		{
		*ctemp=0x66;
		ctemp++;
		*ctemp=0x66;
		ctemp++;
		}
	}
	ctemp = tempp;
}

void LcdWindowDisplay(void)
{
	sLCDMaster(0xf0);
	sLCDMaster(0x04);
	sLCDMaster(0x10);
	sLCDMaster(0x22);
	sLCDMaster(0x30);

	sLCDMaster(0xf5);
	sLCDMaster(0x07);
	sLCDMaster(0x10);
	sLCDMaster(0x39);
	sLCDMaster(0x40);

	sLCDMaster(0xf0);
	sLCDMaster(0xab);

//data
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);

	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);
	sLCDMaster(0x1f);
	sLCDMaster(0xf8);

}