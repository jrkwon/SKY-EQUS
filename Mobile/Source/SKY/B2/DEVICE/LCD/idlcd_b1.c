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
#include "idlcd_b1.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define IDLCD_COMMAND_ADDRESS		0x2000004    /* A1 ADDR : LCD Command 출력 */
#define	IDLCD_DATA_ADDRESS			0x2000006    /* A1 ADDR : LCD Data    출력 */

#define	IDLCD_OUTP_CMD( Value )		outp ( IDLCD_COMMAND_ADDRESS , (Value) );								
#define	IDLCD_OUTP_DATA( Value )	outp ( IDLCD_DATA_ADDRESS , (Value) );								
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
void hs_lcd_reg(word reg);
void hs_idlcd_data(word data);
void hs_lcd_regdata(word reg, word data);

void InitialidLCD ( void );
void hs_idlcd_init( void );

void idLCDDisplayOn ( void );
void idLCDDisplayOff ( void );

void idlcd_message(void)
{}

#define hs_lcd_reg(reg)\
	IDLCD_OUTP_CMD((HS_LCD_MASK & (byte)HS_LCD_HIGH_DATA(reg)));\
	IDLCD_OUTP_CMD((HS_LCD_MASK & (byte)HS_LCD_LOW_DATA(reg)))

#define hs_idlcd_data(data)\
	IDLCD_OUTP_DATA((HS_LCD_MASK & (byte)HS_LCD_HIGH_DATA(data)));\
	IDLCD_OUTP_DATA((HS_LCD_MASK & (byte)HS_LCD_LOW_DATA(data)));

void hs_lcd_regdata(word reg, word data)
{
	hs_lcd_reg(reg);
	hs_idlcd_data(data);
}

void	InitialidLCD ( void )
{
	int		nIndex;

    hs_lcd_regdata(HS_LCD_R00, 0x0001);
    hs_lcd_regdata(HS_LCD_R01, 0x024b);
    hs_lcd_regdata(HS_LCD_R02, 0x005c);
    hs_lcd_regdata(HS_LCD_R03, 0x0918);
    hs_lcd_regdata(HS_LCD_R04, HS_LCD_CONTRAST_D);
    hs_lcd_regdata(HS_LCD_R05, 0x0000);
    hs_lcd_regdata(HS_LCD_R06, 0x0000);
    hs_lcd_regdata(HS_LCD_R07, 0x0000);
    hs_lcd_regdata(HS_LCD_R08, 0x0000);
    hs_lcd_regdata(HS_LCD_R09, 0x0000);
    hs_lcd_regdata(HS_LCD_R0A, 0x0020);
    hs_lcd_regdata(HS_LCD_R0B, 0x0000);
    hs_lcd_regdata(HS_LCD_R0C, 0x0000);
    hs_lcd_regdata(HS_LCD_R10, 0x0000);

	hs_lcd_reg(0x0000);
	hs_lcd_reg(0x0000);
	hs_lcd_reg(0x0000);
	hs_lcd_reg(0x0000);
	{
		word tempw;
		tempw = -1;
        hs_lcd_regdata(HS_LCD_R11, tempw);
	}
	hs_idlcd_on(TRUE);

	hs_lcd_reg(0x0000);
	hs_lcd_reg(0x0000);
	hs_lcd_reg(0x0000);
	hs_lcd_reg(0x0000);
    hs_lcd_reg(HS_LCD_R12);
	{
		word tempw;
		tempw = 0x5555;
	for (nIndex = 0; nIndex < (LCD_TOTAL_ADDRESS_Y * LCD_TOTAL_ADDRESS_X + 1); nIndex++) {
		hs_idlcd_data(tempw);
	}
	}
	//hs_BootLogoDisplay();
}

/*===========================================================================

LOCAL FUNCTION HS_LCD_ON

DESCRIPTION
  This function controls the on/off state of the display.

DEPENDENCIES
  None

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

LOCAL FUNCTION HS_LCD_INIT

DESCRIPTION
  This function performs power up initialization on the HD44780 LCD
  controller. It sets up the controller in 8 bit mode, selects 2 line
  display mode. It also sets the LCD into a default mode and initializes
  the instruction states for all instructions which have bit fields which
  may vary. The default mode is to turn the display ON and set it up to
  increment the address counter when DDRAM data is written.  No other modes
  are enabled at this time.


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

void	idLCDDisplayOn ( void )
{
	hs_lcd_reg(HS_LCD_R07);
	hs_idlcd_data(HS_LCD_DISPLAY_ON);
}

void	idLCDDisplayOff ( void )
{
	hs_lcd_reg(HS_LCD_R07);
	hs_idlcd_data(HS_LCD_DISPLAY_OFF);
}
/*
// EQS_DisplayLCDBufferAll(VOID) 
VOID EQS_API EQS_RefreshDisplayDevice(VOID)
{
    hs_packets_type *hcmd;           
	// command to handset 

    if( ( hcmd = (hs_packets_type *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
      hcmd->hdr.cmd = HS_ALL_SCRN;
      hcmd->hdr.task_ptr = NULL;
      hcmd->scrna.scrn_ptr = NULL;
      hcmd->scrna.blink_scrn_ptr = NULL;
      hs_cmd( hcmd );
    }
}
*/

void EQS_hs_update_IDLCD_screen(void)
{
    BYTE* pbBuffer;
    INT8 nBitsPerPixel;
    SIZE* pSize;
    INT nTotalByteSize;
	INT i;

	hs_lcd_reg(HS_LCD_R00);
	hs_lcd_reg(HS_LCD_R11);
	hs_idlcd_data(0x07ff);
	hs_lcd_reg(HS_LCD_R00);
	hs_lcd_reg(HS_LCD_R12);

    pbBuffer = GetScreenBuffer();
    nBitsPerPixel = GetScreenBitsPerPixel();
    pSize = GetScreenSize();
    nTotalByteSize = (pSize->xWidth * nBitsPerPixel) / 8 * pSize->yHeight;

    for (i = 0 ; i < nTotalByteSize; i++) 
	{
        IDLCD_OUTP_DATA(*(pbBuffer+i));
    }

    //for (i = 0 ; i < 6; i++) 
	//{
    //  LCD_OUTP_DATA(0x55);
    //}

}

void hs_update_IDLCD_screen( void )
{
}

/*===========================================================================

LOCAL FUNCTION HS_UPDATE_ANNUN

DESCRIPTION
  This procedure updates the annunciators, both LED and LCD.

DEPENDENCIES
  Not called from an ISR

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern word *hs_crnt_annun_ptr;              /* Current annunciators           */
extern word hs_annun;                       /* non-blink annunciators         */

void hs_update_annun_IDLCD( void )
{
  word lcd_annun;

  /* Get the new values to update the display with
  */
  lcd_annun = *hs_crnt_annun_ptr;              /* Fetch new setting */
} /* end of hs_update_annun */


void hs_set_glyph_IDLCD
(
  byte index,           /* Index of the character to set the bitmap of */
  byte *bitmap_ptr      /* Pointer to 7 bytes defining the bitmap      */
)
{
}


void hs_idlcd_out
(
  word lcd_command
    /* 9-bit LCD command to output to the LCD controller */
)
{
}

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
void hs_idlcd_contrast
(
  word contrast
    /* Setting the value of LCD Contrast */
)
{	
	if(contrast > HS_LCD_CONTRAST_MAX)
		contrast = HS_LCD_CONTRAST_MAX;
	else if(contrast < HS_LCD_CONTRAST_MIN)
		contrast = HS_LCD_CONTRAST_MIN;

	hs_lcd_regdata(HS_LCD_R04, contrast);

} /* end of hs_idlcd_contrast */
