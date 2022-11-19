/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     idlcd_b1.c
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
** 2000-08-17 youandi     Created.
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
#include "clk.h"
#include "eqs.h"
#include "hs.h"
#include "ID_LCD_B1.h"
#include "sky_idlcd.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
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
void hs_idlcd_cmd(byte reg);
void hs_idlcd_data(byte data);
void hs_idlcd_regdata(byte reg, byte data);

void InitialidLCD ( void );
void idLCDDisplayOn ( void );
void idLCDDisplayOff ( void );

 
void hs_idlcd_cmd(byte reg)
{
	IDLCD_OUTP_CMD(reg);
}

void hs_idlcd_data(byte data)
{
	IDLCD_OUTP_DATA(data);
}

void hs_idlcd_regdata(byte reg, byte data)
{
	hs_idlcd_cmd(reg);
	hs_idlcd_data(data);
}

void	InitialidLCD ( void )
{
	hs_idlcd_cmd(DISPLAY_START_LINE);	
    hs_idlcd_cmd(0xd3);     // Set Display Offest			    Extended CMD05.
    hs_idlcd_cmd(0x11);     // Set Display Offest			    Extended CMD05.
	hs_idlcd_cmd(0xa0);		// Set Segment Remap			07.
	hs_idlcd_cmd(0xa3);		// Set LCD Bias					08.
	hs_idlcd_cmd(0xc8);		// Set COM Output Scan Direction13.
	hs_idlcd_cmd(0x2f);		// Set Set Power Control Resgister4.
    hs_idlcd_cmd(0x22);		// Set Internal RRR				03.
    hs_idlcd_cmd(0x81);		// Set Contrast (CMD)
    hs_idlcd_cmd(0x2a);		// Set Contrast (DAT)
    hs_idlcd_cmd(0xa8);		// Set Duty Ratio (CMD)				Extended CMD01.
    hs_idlcd_cmd(0x1c);		// Set Duty Ratio (DAT)				Extended CMD01.	---- 32
	hs_idlcd_cmd(0xaf);		// Set Display On				11.
}
void	ReInitialidLCD (byte IRR )
{
	hs_idlcd_cmd(DISPLAY_START_LINE);	
    hs_idlcd_cmd(0xd3);     // Set Display Offest			    Extended CMD05.
    hs_idlcd_cmd(0x11);     // Set Display Offest			    Extended CMD05.
	hs_idlcd_cmd(0xa0);		// Set Segment Remap			07.
	hs_idlcd_cmd(0xa3);		// Set LCD Bias					08.
	hs_idlcd_cmd(0xc8);		// Set COM Output Scan Direction13.
	hs_idlcd_cmd(0x2f);		// Set Set Power Control Resgister4.
//    hs_idlcd_cmd(0x22);		// Set Internal RRR				03.
    hs_idlcd_cmd((byte)(0x20+IRR));		// Set Internal RRR				03.
    hs_idlcd_cmd(0x81);		// Set Contrast (CMD)
    hs_idlcd_cmd(0x2a);		// Set Contrast (DAT)
    hs_idlcd_cmd(0xa8);		// Set Duty Ratio (CMD)				Extended CMD01.
    hs_idlcd_cmd(0x1c);		// Set Duty Ratio (DAT)				Extended CMD01.	---- 32
	hs_idlcd_cmd(0xaf);		// Set Display On				11.
}
void	idLCDDisplayOn ( void )
{

}

void	idLCDDisplayOff ( void )
{

}
/*===========================================================================

LOCAL FUNCTION HS_IDLCD_ON

DESCRIPTION
  This function controls the on/off state of the display.

DEPENDENCIES
  Nonec

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hs_idlcd_on
(
  boolean on
    /* Whether the display is to be 'on' (True) or 'off' (False) */
)
{
	/* Update the current state
	*/
	if ( on )
	{
		idLCDDisplayOn ();
	}
	else
	{
		idLCDDisplayOff ();
	}
} /* end of hs_idlcd_on */


/*===========================================================================

LOCAL FUNCTION HS_IDLCD_INIT

DESCRIPTION
  This function performs power up initialization on the SSD1815 LCD
  controller. 

DEPENDENCIES
  Must be called at least 15ms after power is applied to the LCD.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hs_idlcd_init( void )
{
	InitialidLCD ();
} /* end of hs_idlcd_init */




byte IDLCDDataConvert(byte data0, byte data1,byte data2, byte data3,byte data4, byte data5,byte data6, byte data7)
{
	byte 		output=0x00;
	byte		tmp, index, i;
	byte		extract_LSB=0xff;
	byte		tmp_map[256] = 
	{ 
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, \
	0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, \
	0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, \
	0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, \
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, \
	0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, \
	0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, \
	0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01 
	};

	tmp = data0;
	tmp = tmp & extract_LSB;
	tmp = tmp_map[tmp];
	output = output|tmp;

	tmp = data1;
	tmp = tmp & extract_LSB;
	tmp = tmp_map[tmp];
	output=output|(tmp<<1);
	
	tmp = data2;
	tmp = tmp & extract_LSB;
	tmp = tmp_map[tmp];
	output=output|(tmp<<2);

	tmp = data3;
	tmp = tmp & extract_LSB;
	tmp = tmp_map[tmp];
	output=output|(tmp<<3);

	tmp = data4;
	tmp = tmp & extract_LSB;
	tmp = tmp_map[tmp];
	output=output|(tmp<<4);

	tmp = data5;
	tmp = tmp & extract_LSB;
	tmp = tmp_map[tmp];
	output=output|(tmp<<5);

	tmp = data6;
	tmp = tmp & extract_LSB;
	tmp = tmp_map[tmp];
	output=output|(tmp<<6);

	tmp = data7;
	tmp = tmp & extract_LSB;
	tmp = tmp_map[tmp];
	output=output|(tmp<<7);

	return output;
}
/*===========================================================================

EXTERN FUNCTION EQS_UPDATE_IDLCD_SCREEN

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None    

SIDE EFFECTS


===========================================================================*/
void EQS_hs_update_IDLCD_screen(void)
{
    BYTE* pbBuffer;
    SIZE* pSize;
    INT8 nBitsPerPixel;
    INT nTotalByteSize;

	uint64 converted_data;
	INT i, j, byte_shift;

    pbBuffer = GetScreenBuffer();
    pSize = GetScreenSize();
    nBitsPerPixel = GetScreenBitsPerPixel();
    nTotalByteSize = (pSize->xWidth * pSize->yHeight);

	for( j=0; j<4; ++j){		// 4 : number of page ( 0,1,2,3 Page )
    
		hs_idlcd_cmd(0xb0+j);	// Set Page	
		hs_idlcd_cmd(0x11);		// Set Start Column Address	
		hs_idlcd_cmd(0x08);		// Set Start Column Address	
		for ( i=0; i<84; ++i )  // 84 : ID LCD width 
		{  
		    converted_data = IDLCDDataConvert(
			*(pbBuffer + i       +960*2 + 960*j), \
			*(pbBuffer + i + 120 +960*2 + 960*j), \
			*(pbBuffer + i + 240 +960*2 + 960*j), \
			*(pbBuffer + i + 360 +960*2 + 960*j), \
			*(pbBuffer + i + 480 +960*2 + 960*j), \
			*(pbBuffer + i + 600 +960*2 + 960*j), \
			*(pbBuffer + i + 720 +960*2 + 960*j), \
			*(pbBuffer + i + 840 +960*2 + 960*j)  );

			hs_idlcd_data( (byte)(converted_data) );
		}
	}
}

/*===========================================================================

EXTERN FUNCTION HS_IDLCD_CONTRAST

DESCRIPTION
  This function controls the contrast state of the display.

DEPENDENCIES
  None
--
RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
void hs_idlcd_contrast
(
  word contrast
    /* Setting the value of LCD Contrast */
)
{	
	
	if(contrast >= 0 && contrast <=63){
		hs_idlcd_cmd(0x81);		
		hs_idlcd_cmd((byte)contrast);		
	}
} /* end of hs_idlcd_contrast */

