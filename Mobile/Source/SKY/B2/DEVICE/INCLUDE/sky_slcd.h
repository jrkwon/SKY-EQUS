/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     sky_slcd.h
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
** 2000-12-25 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_SLCD_H_
#define _SKY_SLCD_H_

#ifdef __cplusplus
extern "C" {
#endif
    
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
/*------------------------------------------------------------------------*/
/* SUB LCD GPIO OUT PUT Mocros */
/*------------------------------------------------------------------------*/

/* LCD Application Specifications
** 1. LCD SIZE          : 84 x 30
** 2. Default Font Size : 6 * 13 (ASCII)
** 3. Total Charactoer  : 14 Char
** 4. Total Line        : 2 Line
*/

/*------------------------------------------------------------------------*/
/* SUB LCD Control data */
/*------------------------------------------------------------------------*/

#define LCD_SUB_DISPLAY_ON      0xAF    /* Display On */
#define LCD_SUB_DISPLAY_OFF     0xAE    /* Display Off */

#define LCD_SUB_INIT_DISP_CMD   0x40    /* Addr. Specify DDRAM line for COM0 */
#define LCD_SUB_INIT_DISP_M     0x3F    /* MASK. Specify DDRAM line for COM0 */

#define LCD_SUB_REFERENCE_VOL_MODE  0x81    /* Set reference voltage mode */

#define LCD_SUB_REFERENCE_VOL_CMD   0x80    /* Addr. Set reference voltage mode */
#define LCD_SUB_REFERENCE_VOL_M     0x1F    /* MASK. Set reference voltage mode */

#define LCD_SUB_SET_PAGE_ADDR_CMD   0xB0    /* Addr. Set page address */
#define LCD_SUB_SET_PAGE_ADDR_M     0x0F    /* Mask Set page address */

#define LCD_SUB_COLUMN_ADDR_MSB_CMD 0x10    /* Addr Set Column Address  */
#define LCD_SUB_COLUMN_ADDR_MSB_M   0x07    /* Mask Set Column Address  */
#define LCD_SUB_COLUMN_ADDR_LSB_CMD 0x00    /* Addr Set Column Address  */
#define LCD_SUB_COLUMN_ADDR_LSB_M   0x0F    /* Mask Set Column Address  */

#define LCD_SUB_SEG_DIR_NORMAL      0xA0    /* Set SEG output direction to normal */
#define LCD_SUB_SEG_DIR_REVERSE     0xA1    /* Set SEG output direction to reverse */
#define LCD_SUB_DISPLAY_NORMAL      0xA6    /* Select normal/reverse display */
#define LCD_SUB_DISPLAY_REVERSE     0Xa7    /* Select normal/reverse display */

#define LCD_SUB_ENTIRE_DISPLAY_NORMAL   0xA4    /* Select normal/entire display */
#define LCD_SUB_ENTIRE_DISPLAY_ENTIRE   0xA5    /* Select normal/entire display */

#define LCD_SUB_LCD_BIAS_1_5            0xA3    /* Select LCD bias 1/5 */
#define LCD_SUB_LCD_BIAS_1_6            0xA2    /* Select LCD bias 1/6 */

#define LCD_SUB_RESET                   0xE2    /* Initialize the internal functions */

#define LCD_SUB_COM_DIR_NORMAL          0xC0    /* Select COM output direction NORMAL  */
#define LCD_SUB_COM_DIR_REVERSE         0xC8    /* Select COM output direction REVERSE  */

#define LCD_SUB_PWR_CTRL_VC             0x2C    /* Control power circuit operation */
#define LCD_SUB_PWR_CTRL_VR             0x2A    /* Control power circuit operation */
#define LCD_SUB_PWR_CTRL_VF             0x29    /* Control power circuit operation */

#define LCD_SUB_STATIC_INDICATOR_ON     0xAD    /* Set static indicator register on */
#define LCD_SUB_STATIC_INDICATOR_OFF    0xAC    /* Set static indicator register off */

/* --
#define ID_LCD_W_ENA() \
  outpw(bio_gpio[MSM31_GPIO_1].out_addr, (bio_gpio[MSM31_GPIO_1].out_buf & ~BIO_ID_LCD_CS_HI_V))

#define ID_LCD_W_DIS()  \
  outpw(bio_gpio[MSM31_GPIO_1].out_addr, (bio_gpio[MSM31_GPIO_1].out_buf | BIO_ID_LCD_CS_HI_V))

#define ID_LCD_OUTP_CMD( Value )    outp ( 0x2000000 , (Value) );
#define ID_LCD_OUTP_DATA( Value )   outp ( 0x2000002 , (Value) );
-- */

/*------------------------------------------------------------------------*/
/* SUB LCD Use Defaines */
/*------------------------------------------------------------------------*/
/* SUB LCD Initial value */
//#define LCD_SUB_INIT_DISP_LINE      0x1C
//#define LCD_SUB_INIT_PAGE           6
/* SUB LCD delay time (ms) */
#define  LCD_SUB_CONTOFFSET_MAX			(63)
#define  SUBLCD_CONTRAST_DEFAULT		0x2a
#define  SLCD_CONT_STEP					3
/* data default */
//#define  LCD_SUB_CONTADJVAL_DEF   0x12

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
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API SKY_InitSubLCD(void);
extern void SKY_API SKY_ReInitSubLCD(BYTE bIRR);
extern void SKY_API SKY_SubLCDOn(void);
extern void SKY_API SKY_SubLCDOff(void);
extern void SKY_API SKY_SetSubLCDContrast(HS_CONTRAST_E wContrast, BYTE wContrastLevel);
extern void SKY_API SKY_UpdateSubScreen(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_SLCD_H_ */
