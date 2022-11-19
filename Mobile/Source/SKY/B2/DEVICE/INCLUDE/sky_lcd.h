/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     sky_lcd.h
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
** 2001-04-21 asdf			Modify it fit for B2
** 2000-12-28 woodstok     Created.
**
** ============================================================================
*/

#ifndef _SKY_LCD_H_
#define _SKY_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "bio.h"
#include "biog.h"
#include "comdef.h"


/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
//#define HS_RGBLED(bred,bgreen,bblue) FALSE

#define LCD_COMMAND_ADDRESS			0x2000000    /* A1 ADDR : LCD Command 출력 */
#define	LCD_DATA_ADDRESS			0x2000002    /* A1 ADDR : LCD Data    출력 */

#define	LCD_OUTP_CMD( Value )	outp ( LCD_COMMAND_ADDRESS , (Value) );								
#define	LCD_OUTP_DATA( Value )	outp ( LCD_DATA_ADDRESS , (Value) );

/* 
-----------------------------------------------------------------------
					B2 64K Color LCD Command & Data Address, MACRO
-----------------------------------------------------------------------
*/

#ifdef FEATURE_64KCOLOR_LCD

#define LCD_DRG_ADDRESS				0x2000000    
#define	LCD_ADR_ADDRESS				0x2000002    
#define	LCD_CTL_ADDRESS				0x2000004    
#define	LCD_CNF_ADDRESS				0x2000006
#define	LCD_DATA1_ADDRESS			0x2000008
#define	LCD_MASTER_ADDRESS			0x200000a
#define	LCD_DATA2_ADDRESS			0x200000c
#define	LCD_SLAVE_ADDRESS			0x200000e


#define	LCD_OUTP_DRG( Value )		*(volatile word*)(LCD_DRG_ADDRESS) = (Value)
#define	LCD_OUTP_ADR( Value )		*(volatile word*)(LCD_ADR_ADDRESS) = (Value)
#define	LCD_OUTP_CTL( Value )		*(volatile word*)(LCD_CTL_ADDRESS) = (Value)
#define	LCD_OUTP_CNF( Value )		*(volatile word*)(LCD_CNF_ADDRESS) = (Value)
#define	LCD_OUTP_MASTER( Value )	*(volatile word*)(LCD_MASTER_ADDRESS) = (Value)
#define	LCD_OUTP_SLAVE( Value )		*(volatile word*)(LCD_SLAVE_ADDRESS) = (Value)
#define	LCD_OUTP_DATA1( Value )		*(volatile word*)(LCD_DATA1_ADDRESS) = (Value)
#define	LCD_OUTP_DATA2( Value )		*(volatile word*)(LCD_DATA2_ADDRESS) = (Value)


#define LCD_ORIGIN					0
#define	LCD_TOTAL_ROW_SIZE			160			// Y
#define LCD_TOTAL_COLUMN_SIZE		128			// X
#define LCD_MASTER_END				80
#define LCD_SLAVE_START				82

/*
**------------------------------------------------------------------------
		B2 LCD Command 
**------------------------------------------------------------------------
*/

//BANK Select
#define BANK_0							0xf0	//X, Y Address and Display Control 
#define BANK_1							0xf1	//Gradation Pallet
#define BANK_2							0xf2	//Gradation Pallet
#define BANK_3							0xf3	//Gradation Pallet
#define BANK_4							0xf4	//Gradation Pallet & others
#define BANK_5							0xf5	//window X, Y and Start/End Address

//BANK 0
#define DISPLAY_ON						0x81
#define DISPLAY_OFF						0x80
#define ALL_DISPLAY__ON					0x83
#define DISPLAY_MONOCHROME				0x85
#define DISPLAY_SHIFT					0x89
#define SEGMENT_NORMAL					0x90
#define SEGMENT_REVERSE					0x91
#define DISPLAY_SWAP					0x92
#define LINE_REVERSE					0x94
#define DISPLAY_REVERSE					0x98
#define WINDOW_FUNCTION_ACCESS			0xa8
#define NORMAL_RAM_ACCESS				0xa0 
#define X_INCREMENT						0xa1
#define Y_INCREMENT						0xa2
#define X_Y_INCREMENT					0xa3
#define INTERNAL_AMP_ON					0xb8
#define POWER_SAVING_ON					0xb4
#define POWER_SAVING_OFF				0xb0
#define BOOSTING_CIRCUIT_ON				0xb2

//BANK4
#define	GLSB_SELECT 					0x84
#define DATA_LENGTH_8					0x90
#define DATA_LENGTH_16					0x91
//display select control

#endif

/*
**----------------------------------------------------------------------------
**  B1 Type Definitions
**----------------------------------------------------------------------------
*/
#define DISPLAY_NORMAL				0xA6	// display normal
#define DISPLAY_INVERSE				0xA7	// display inverted
#define DISPLAY_CONTROL				0xCA	// display control
#define SLEEP_IN					0x95	// sleep in
#define SLEEP_OUT					0x94	// sleep out
#define PAGE_ADDRESS_SET			0x75	// page address set
#define COLUMN_ADDRESS_SET			0x15	// column address set
#define DATA_CONTROL				0xBC	// data control
#define RAM_WRITE					0x5C	// RAM write
#define PARTIAL_DISPLAY_IN			0xA8	// partial display in
#define PARTIAL_DISPLAY_OUT			0xA9	// partial display out
#define READ_MODIFY_WRITE_IN		0xE0	// read modify write in
#define READ_MODIFY_WRITE_OUT		0xEE	// read modify write out
#define AREA_SCROLL_SET				0xAA	// area scroll set
#define SCROLL_START_BLOCK			0xAB	// scroll block start
#define VOLUME_CONTROL				0xC6	// volume control
#define NOP							0x25	// no operation
/* logical contrast level */
enum tagHS_CONTRAST_E 
{
    HCL_MIN,
    HCL_LLL,
    HCL_LL,
    HCL_L,
    HCL_D,
    HCL_H,
    HCL_HH,
    HCL_HHH,
    HCL_MAX
};
typedef enum tagHS_CONTRAST_E  HS_CONTRAST_E;

/*
** LCD contrast value : 0 ~ 31 (5 bits)
** flag for up or down (6th bit) : 
**       0 -> down
**       1 -> up
*/
enum tagLCD_CONTRAST_UPDOWN_E 
{
    LCU_DOWN = 0x00, 
    LCU_UP   = 0x20
};
typedef enum tagLCD_CONTRAST_UPDOWN_E  LCD_CONTRAST_UPDOWN_E;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
#define LCD_CONTRAST_MIN			0x30
#define LCD_CONTRAST_MAX			0x7F

#define LCD_CONTAST_STEP			0x03
#define LCD_CONTRAST_DEFAULT		0x4d

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern const word cvt8b16b[];
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API SKY_LCDOn(void);
extern void SKY_API SKY_LCDOff(void);
extern void SKY_API SKY_InitLCD(void);
extern void SKY_API SKY_UpdateScreen(void);
extern void SKY_API SKY_BootLogoDisplay(void);
extern void SKY_FillScreenOneColor(WORD c);
extern void SKY_FillScreenImage(void);
extern void SKY_LCDReset(void);

/* only for LCD contrast test !!!!!!! do not use this function in UI */
extern void SKY_API SKY_SetLCDContrastTest(BYTE bContrast);
extern void SKY_API SKY_SetLCDContrast(HS_CONTRAST_E Contrast, BYTE ContrastLevel);
//extern void SKY_API SKY_SetLCDContrast (HS_CONTRAST_E Contrast);
extern void SKY_UpdatePartialDisplayRect(BYTE *pbBuffer, UINT8 nXpos, UINT8 nYpos, UINT8 nXendpos, UINT8 nYendpos);
extern void SKY_UpdatePartialDisplayRectByRegion(BYTE *pbBuffer, UINT8 nXpos, UINT8 nYpos, UINT8 nXendpos, UINT8 nYendpos);

extern void SKY_UpdatePartialScreen(BYTE *pbBuffer, BYTE nXPos, BYTE nYPos, BYTE nXendPos, BYTE nYendPos);
extern void SKY_UpdatePartialScreenWithoutAnnu(BYTE *pbBuffer);

/* for camera test only ==> */
extern void SKY_API SKY_LCDCommand(BYTE bLCDCommand);
extern void SKY_API SKY_LCDData(BYTE bLCDData);
/* <-- */
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_LCD_H_ */
