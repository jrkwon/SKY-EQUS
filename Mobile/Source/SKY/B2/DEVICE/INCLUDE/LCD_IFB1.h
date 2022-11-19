#ifndef LCD_IFB1_H
#define LCD_IFB1_H

#include "target.h"

/* jrkwon 2000-03-30 --> @why, what */
#ifdef FEATURE_NUI_D1 

/* LCD type is NEC */
#define FONT_WIDTH                                 6   /* Col. Dot No. */
#define FONT_HEIGHT                                13  /* Row  Dot No. */

#define HAN_FONT_WIDTH                             12
#define HAN_FONT_HEIGHT                            13
#define HAN_FONT_BUFFER_SIZE_PER_CHAR              26  /* byte */

#define ASCII_FONT_WIDTH                           6
#define ASCII_FONT_HEIGHT                          13
#define ASCII_FONT_BUFFER_SIZE_PER_CHAR            13  /* byte */

#define BIG_ASCII_FONT_WIDTH                       8
#define BIG_ASCII_FONT_HEIGHT                      19
#define BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR        19  /* byte */

#define SMALL_ASCII_FONT_WIDTH                     6
#define SMALL_ASCII_FONT_HEIGHT                    8
#define SMALL_ASCII_FONT_BUFFER_SIZE_PER_CHAR      8  /* byte */

#define CALENDAR_FONT_WIDTH                        5
#define CALENDAR_FONT_HEIGHT                       6
#define CALENDAR_FONT_BUFFER_SIZE_PER_CHAR         8  /* byte */

#define NORMAL_SIZE_ASCII_FONT                     0
#define BIG_SIZE_ASCII_FONT                        1
#define SMALL_SIZE_ASCII_FONT                      2
#define CALENDAR_FONT                              3

/* Valiables for LCD
*/
#define LCD_BASE_ADDR                              0x2000010

#endif /* FEATURE_NUI_D1 */
/* <-- */

void hs_lcd_cmd_out ( byte lcd_command );
void hs_lcd_dat_out (byte lcd_data); 
void hs_lcd_on ( boolean on );
void hs_set_glyph ( byte index, byte *bitmap_ptr );
void hs_update_screen ( void );
void hs_update_annun ( void );
void hs_lcd_init ( void );

#endif
