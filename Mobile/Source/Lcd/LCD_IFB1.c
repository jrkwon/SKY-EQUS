
/*****************************************************************************
 *
 *
 *		LCD BIOS module
 *
 *
 *		Copyright (C) 1998, 1999, 2000 SK Teletech
 *
 *
 *****************************************************************************
 *
 *
 *		FileName : LCD_IF.c
 *		Revision : 0.00.001
 *		 Created : 00.03.02
 *
 *	    Modified : 00.07.11,tasik
 *		
 *
 *****************************************************************************
 *
 *
 *		Designed by : chos
 *		   Coded by : chos
 *		  Tested by : chos
 *
 *
 *****************************************************************************
 *
 *			
 *		Module description
 *		------------------
 *
 *			Qualcomm Source와 LCD driver를 interface하기위해
 *			 'hsg.c'의 LCD display 관련 routine만 'LCD_IF.c'로
 *			 옮겨 function 수정
 *
 *
 *****************************************************************************
 *
 *
 *		Revision detail
 *		---------------
 *
 *			Rev	0.00.001	00.03.02 13:00	chos
 *				created this module
 *		
 *
 *****************************************************************************/

#define __LCD_IF_MODULE


/*****************************************************************************
 *
 *		Include external header files
 *
 *****************************************************************************/

#include "target.h"     /* Target specific definitions             */
#include "comdef.h"     /* Definitions for byte, word, etc.        */
#include "LCD_IFB1.h"
#include "LCD_BIOSB1.h"
#include "..\Font\DrawText.h"
#include "hs.h"
#include <string.h>

#ifdef FEATURE_NUI_D1
#include "nui.h"
#endif /* FEATURE_NUI_D1 */
/*===========================================================================

                                 C O D E

===========================================================================*/

/* jrkwon 2000-05-12 --> @why, what */
#ifdef FEATURE_NUI_D1

/*
** ***************************************************************************
**
**
**
**
**
**
**
**
**
**
**   LCD Routine for D1 by jrkwon
**
**
**                  import from IM1200N
**
**
**
**
**
**
**
**
**
** ***************************************************************************
*/

extern	hs_scrn_type hs_lcd_screen;     /* LCD screen contents            */
extern	byte *hs_crnt_screen_ptr;


/* 2000/4/22 for DI_DRAW  by reo  {--*/  
extern  hs_scrn_type hs_lcd_mode_screen;
//extern  byte *hs_crnt_mode_ptr;
extern  hs_scrn_type *hs_crnt_mode_ptr;
/* 2000/4/22 for DI_DRAW  by reo  --}*/
extern  byte *hs_crnt_rssi_ptr;         /* Current RSSI level             */
extern  byte *hs_crnt_batt_ptr;         /* Current Battery level          */
extern  word *hs_crnt_annun_ptr;        /* Current annunciators           */



LOCAL NU_Int s_SetFont(NU_UChar ucFontA)
{
    NU_Int nFont; // previous font before SetASCIIFont()
    if( ucFontA & NU_DRAW_BIG_ASCII_FONT )
    {
        nFont = SetASCIIFont(BIG_SIZE_ASCII_FONT);
    }
    else if ( ucFontA & NU_DRAW_SMALL_ASCII_FONT )
    {
        nFont = SetASCIIFont(SMALL_SIZE_ASCII_FONT);
    }
    else if ( ucFontA & NU_DRAW_CALENDAR_FONT )
    {
        nFont = SetASCIIFont(CALENDAR_FONT);
    }
    else
    {
        nFont = SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
    }
    return nFont;
}

LOCAL NU_Bool s_IsBigAsciiRow(NU_UInt unRowA)
{
    NU_UInt unCol;
    NU_UInt unIndex;

    for(unCol = 0; unCol < NU_SCREEN_WIDTH; unCol++)
    {
        unIndex = unCol + unRowA * NU_SCREEN_WIDTH;
        if(hs_lcd_mode_screen[unIndex] & NU_DRAW_BIG_ASCII_FONT)
            return NU_TRUE;
    }
    return NU_FALSE;
}

LOCAL void s_FillBigAsciiDrawMode(NU_UInt unRowA)
{
    NU_UInt unCol;
    NU_UInt unIndex;

    for(unCol = 0; unCol < NU_SCREEN_WIDTH; unCol++)
    {
        unIndex = unCol + unRowA * NU_SCREEN_WIDTH;
        hs_lcd_mode_screen[unIndex] |= NU_DRAW_BIG_ASCII_FONT;
    }
}





/*===========================================================================

LOCAL FUNCTION     HS_LCD_CMD_OUT
                   HS_LCD_DAT_OUT

DESCRIPTION
  This function sends the command/data out.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hs_lcd_cmd_out
(
  byte lcd_command
    /* 8-bit LCD command to output to the LCD controller */
)
{
    LCD_OUTP_CMD(lcd_command);
} /* end of hs_lcd_cmd_out */

LOCAL void hs_lcd_dat_out
(
	byte lcd_data
 )
{
	
	LCD_OUTP_DATA(lcd_data);

}/* end of hs_lcd_dat_out*/


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
LOCAL void hs_lcd_on
(
  boolean on
    /* Whether the display is to be 'on' (True) or 'off' (False) */
)
{
	/* Update the current state
	*/
	if ( on )
	{
		LCDDisplayOn ();
	}
	else
	{
		LCDDisplayOff ();
	}
} /* end of hs_lcd_on */


/*===========================================================================

FUNCTION HS_SET_GLYPH

DESCRIPTION
  Set the bitmap of a specified character.  The data format of a bitmap
  is a map of each row of the character, 5 bits wide (LSB justified) and
  7 rows tall.  For example:
  
    0x04, 0x0e, 0x1f, 0x03, 0x03, 0x18, 0x18 is

        ..X..
        .XXX.
        XXXXX
        ...XX
        ...XX
        XX...
        XX...

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  By changing the bitmap of the character, any characters of that type
  currently being displayed will change to the new character.

===========================================================================*/
LOCAL void hs_set_glyph
(
  byte index,           /* Index of the character to set the bitmap of */
  byte *bitmap_ptr      /* Pointer to 7 bytes defining the bitmap      */
)
{
} /* end of hs_set_glyph */


/*===========================================================================

LOCAL FUNCTION HS_UPDATE_SCREEN

DESCRIPTION
  This procedure updates the screen, making the screen appear the
  same as new_screen.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the display RAM address and contents.

===========================================================================*/
LOCAL void hs_update_screen( void )
{

    NU_UInt     unDrawMode;
    NU_Int      nCharWidth;
    NU_Int      nCharHeight;

    NU_UInt unCol, unRow;
    NU_UInt unIndex;
    NU_Bool bBigASCII = NU_FALSE; /* current row ascii mode */

    if ( (memcmp(hs_lcd_screen,hs_crnt_screen_ptr,sizeof(hs_lcd_screen)) == 0)
         && (memcmp(hs_lcd_mode_screen,hs_crnt_mode_ptr,sizeof(hs_lcd_mode_screen)) == 0) )  
    {
        return;
    }

    memcpy( hs_lcd_screen , hs_crnt_screen_ptr, sizeof(hs_lcd_screen) );    
    memcpy( hs_lcd_mode_screen, hs_crnt_mode_ptr, sizeof(hs_lcd_mode_screen) );


    for( unRow = 0; unRow < NU_SCREEN_HEIGHT; unRow++)
    {
        if(s_IsBigAsciiRow(unRow))
        {
            bBigASCII = NU_TRUE;
            s_FillBigAsciiDrawMode(unRow);
            s_SetFont(NU_DRAW_BIG_ASCII_FONT);
        }
        else
        {
            bBigASCII = NU_FALSE;
            s_SetFont(NU_DRAW_NORMAL);
        }
        nCharWidth = GetASCIIFontWidth();
        nCharHeight = GetASCIIFontHeight();

        /* clear 1.5 line */
        if( (unRow > 0) && bBigASCII && !s_IsBigAsciiRow(unRow-1) )
        {
            NU_UChar szSpaces[NU_SCREEN_WIDTH+1];

            memset((char*)szSpaces, ' ', NU_SCREEN_WIDTH);
            szSpaces[NU_SCREEN_WIDTH] = NULL;
            NU_DrawModeNUI2LCD(NU_DRAW_NORMAL, &unDrawMode );           

            TextOutXYN( szSpaces, NU_SCREEN_WIDTH, 
                        0, unRow*ASCII_FONT_HEIGHT, unDrawMode, NO_DISPLAY_LCD);     
        }

        for(unCol = 0; unCol < NU_SCREEN_WIDTH; unCol++)
        {
            unIndex = unCol + unRow * NU_SCREEN_WIDTH;

            NU_DrawModeNUI2LCD((NU_DrawMode)hs_lcd_mode_screen[unIndex], &unDrawMode );            
            TextOutXYN( &hs_lcd_screen[unIndex], 1, 
                        unCol*nCharWidth, unRow*nCharHeight, unDrawMode, NO_DISPLAY_LCD);     
            if(hs_lcd_screen[unIndex] & 0x80)
                unCol++;            
            
        }

        /* to avoid for default font covering the BIG_ASCII_FONT_HEIGHT */
        if(bBigASCII==NU_TRUE)
        {
            if(unRow < (NU_BIG_ASCII_SCREEN_HEIGHT-1))
            {
                if(!(s_IsBigAsciiRow(unRow+1)))
                {
                    unRow = unRow + BIG_ASCII_FONT_HEIGHT/ASCII_FONT_HEIGHT;
                }                    
            }
            else    unRow++; //before commandline:NU_SCREEN_HEIGHT-1            
        }            
    }

	DisplayLCDBufferAll();

} /* end of hs_update_screen */

void hs_lcd_icon(byte nColumn, byte data)
{
	/*
		nColumn : start column number at which the intended icon is displayed.
				  This number is multiple times of LCD_ICON_WIDTH.
				  However, the width of a single icon can't be determined here.
	
	*/

	SetLCDIconAddress(nColumn);

	hs_lcd_cmd_out(RAM_WRITE);
	hs_lcd_dat_out( data );
}

//void hs_icd_on_all()
//{
//    (void) memset((byte *)LCDDataBuffer, 0xff, sizeof(LCDDataBuffer));
//    hs_lcd_out(LCD_COMMAND_WRITE_MODE);
//}

// Added by chul


// tasik 00.07.11 //
/* following Address may be modified for B1 */
const unsigned char IconAddr[] = 
    { 5, 8, 11, 15, 18, 21, 27, 33, 41, 45, 39, 50, 62, 68, 67, 72, 80, 95, 94, 96 };

static void lcd_AllPictographsOn(void)
{
    short   n;

    for(n=0; n<sizeof(IconAddr); n++){
        hs_lcd_icon(IconAddr[n], 0xff);
    }
}

static void lcd_AllPictographsOff(void)
{
    short   n;
    
    for(n=0; n<sizeof(IconAddr); n++){
        hs_lcd_icon(IconAddr[n], 0x00);
    }
}


void hs_update_annun( void )
/*LOCAL void hs_update_annun( void )*/
{
    word lcd_annun;
    byte lcd_rssi;
    byte lcd_batt;
  
    lcd_annun = (word)*hs_crnt_annun_ptr;
    lcd_rssi = (byte)*hs_crnt_rssi_ptr;
    lcd_batt = (byte)*hs_crnt_batt_ptr;

    // chul.add
    // jrkwon if(mc_bSwReset) return;
    // chul.end

// Added by chul
    if(lcd_annun & HS_AN_ALL_ON)
    {
        lcd_AllPictographsOn();
        return;
    }
    else if(lcd_annun & HS_AN_ALL_OFF)
    {
        lcd_AllPictographsOff();
        return;
    }

// End of chul

    //lcd_rssi = 5;
    switch(lcd_rssi)
    {

/////////////////////////////////////////////////////
//       tasik 00.07.11							   //
//       following number should be modifed for B1 //
/////////////////////////////////////////////////////
	case 5:
            hs_lcd_icon(1,0x09);
            hs_lcd_icon(9,0x22);
            hs_lcd_icon(17,0x48);
            break;
        case 4:
            hs_lcd_icon(1,0x09);
            hs_lcd_icon(9,0x22);
            hs_lcd_icon(17,0x40);
            break;
        case 3:
            hs_lcd_icon(1,0x09);
            hs_lcd_icon(9,0x22);
            hs_lcd_icon(17,0x00);
            break;
        case 2:
            hs_lcd_icon(1,0x09);
            hs_lcd_icon(9,0x20);
            hs_lcd_icon(17,0x00);
            break;
        case 1:
            hs_lcd_icon(1,0x09);
            hs_lcd_icon(9,0x00);
            hs_lcd_icon(17,0x00);
            break;
        case 0:
        default:
            hs_lcd_icon(1,0x08);
            hs_lcd_icon(9,0x00);
            hs_lcd_icon(17,0x00);
            break;
    }

    //lcd_batt = 4;
    switch(lcd_batt)
    {
        case 4:
            hs_lcd_icon(73,0x01);
            hs_lcd_icon(89,0x07);
            break;
        case 3:
            hs_lcd_icon(73,0x01);
            hs_lcd_icon(89,0x06);
            break;
        case 2:
            hs_lcd_icon(73,0x01);
            hs_lcd_icon(89,0x02);
            break;
        case 1:
            hs_lcd_icon(73,0x01);
            hs_lcd_icon(89,0x00);
            break;
        case 0:
            hs_lcd_icon(73,0x00);
            hs_lcd_icon(89,0x00);
            break;
        default:
            hs_lcd_icon(73,0x01);
            hs_lcd_icon(89,0x00);
            break;
    }

	if ((lcd_annun & HS_AN_INUSE) != 0)
    {
        hs_lcd_icon(33,0x82);
        hs_lcd_icon(41,0x00);
    }
  	else
    {
        hs_lcd_icon(33,0x02);
        hs_lcd_icon(41,0x08);
    }

	if ((lcd_annun & HS_AN_ROAM) != 0)
        hs_lcd_icon(27,0x20);
	else
        hs_lcd_icon(27,0x00);

// SMS ICON huni가 수정 99.7.7

  	if ((lcd_annun & HS_AN_MSG) != 0)
    {
        hs_lcd_icon(65,0x11);
    }
  	else if((lcd_annun & HS_AN_VMAIL) != 0)
    {
        hs_lcd_icon(65,0x21);
    }
  	else
    {
        hs_lcd_icon(65,0x00);
    }

// SMS ICON huni가 수정 99.7.7
/*
  if ((lcd_annun & HS_AN_MSG) != 0)
	{
			hs_lcd_icon(65,0x11);
	}
  else if((lcd_annun & HS_AN_VMAIL) != 0)
	{
			hs_lcd_icon(65,0x21);
	}
  else
		{
			hs_lcd_icon(65,0x00);
	}
*/
    if ((lcd_annun & HS_AN_MSG) != 0) 
    {
        if ((lcd_annun & HS_AN_VMAIL) != 0)
            hs_lcd_icon(65,0x31);
        else 
            hs_lcd_icon(65,0x11);
    } 
    else if((lcd_annun & HS_AN_VMAIL) != 0)
    {
        hs_lcd_icon(65,0x21);
    }
    else 
    {
        hs_lcd_icon(65,0x00);
    }
// SMS ICON huni가 수정 99.7.7 end.

	if ((lcd_annun & HS_AN_VIB) != 0)
        hs_lcd_icon(50,0x40);
  	else
        hs_lcd_icon(50,0x00);


  	if ((lcd_annun & HS_AN_REC) != 0)
        hs_lcd_icon(62,0x04);
  	else
        hs_lcd_icon(62,0x00);

  	/* jrkwon
    if(sni_led_select == FALSE)
    {
  		if ((lcd_annun & HS_AN_SNI) != 0) {
    		HS_CALL_LED_ON();
  		} else {
    		HS_CALL_LED_OFF();
  		}
    }
    */

    // chul.add
    //jrkwon : hs_SetIconBlink();
    // chul.end

}


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
LOCAL void hs_lcd_init( void )
{

	InitialLCD ();

    hs_update_annun();
} /* end of hs_lcd_init */

/*
** ***************************************************************************
**
**
**
**
**
**
**
**
**
**
**   LCD Routine for D1 by LAYER 1
**
**
**
**
**
**
**
**
**
**
**
**
** ***************************************************************************
*/
#else /* FEATURE_NUI_D1 */

/*-----------------------------------------------------------------------
                         LCD Controller Routines
-----------------------------------------------------------------------*/

/*===========================================================================

LOCAL FUNCTION LCD_OUT

DESCRIPTION
  Output command to the LCD controller.

DEPENDENCIES
  Clock dependent delays which rely on a 13.5 MHz '186 in an MSM 2p.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hs_lcd_out
(
  word lcd_command
    /* 9-bit LCD command to output to the LCD controller */
)
{
} /* end of hs_lcd_out */


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
LOCAL void hs_lcd_on
(
  boolean on
    /* Whether the display is to be 'on' (True) or 'off' (False) */
)
{
	/* Update the current state
	*/
	if ( on )
	{
		LCDDisplayOn ();
	}
	else
	{
		LCDDisplayOff ();
	}
} /* end of hs_lcd_on */


/*===========================================================================

FUNCTION HS_SET_GLYPH

DESCRIPTION
  Set the bitmap of a specified character.  The data format of a bitmap
  is a map of each row of the character, 5 bits wide (LSB justified) and
  7 rows tall.  For example:
  
    0x04, 0x0e, 0x1f, 0x03, 0x03, 0x18, 0x18 is

        ..X..
        .XXX.
        XXXXX
        ...XX
        ...XX
        XX...
        XX...

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  By changing the bitmap of the character, any characters of that type
  currently being displayed will change to the new character.

===========================================================================*/
LOCAL void hs_set_glyph
(
  byte index,           /* Index of the character to set the bitmap of */
  byte *bitmap_ptr      /* Pointer to 7 bytes defining the bitmap      */
)
{
} /* end of hs_set_glyph */


/*===========================================================================

LOCAL FUNCTION HS_UPDATE_SCREEN

DESCRIPTION
  This procedure updates the screen, making the screen appear the
  same as new_screen.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the display RAM address and contents.

===========================================================================*/

extern	hs_scrn_type hs_lcd_screen;     /* LCD screen contents            */
extern	byte *hs_crnt_screen_ptr;

LOCAL void hs_update_screen( void )
{
	int		nRow;

	// compare new screen & backup screen
	//	backup screen과 같을 경우 draw 할 필요 없다
	if ( memcmp ( hs_lcd_screen, hs_crnt_screen_ptr,
		sizeof ( hs_lcd_screen ) ) != 0 )
	{
		// backup curent screen
		memcpy ( hs_lcd_screen, hs_crnt_screen_ptr, sizeof ( hs_lcd_screen ) );

		// draw current screen to LCD buffer
		for ( nRow = 0; nRow < HS_SCRN_HIGH; nRow ++ )
		{
			TextOutLineN ( hs_lcd_screen + ( HS_SCRN_WIDE * nRow ),
				HS_SCRN_WIDE, nRow,
				DRAW_NORMAL, NO_DISPLAY_LCD );
		}

		// draw current screen
		DisplayLCDBufferAll ();
	}

} /* end of hs_update_screen */


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

LOCAL void hs_update_annun( void )
{
} /* end of hs_update_annun */


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
LOCAL void hs_lcd_init( void )
{

	InitialLCD ();

} /* end of hs_lcd_init */

#endif /* FEATURE_NUI_D1 */
/* <-- */

