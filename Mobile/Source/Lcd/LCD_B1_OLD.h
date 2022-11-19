
#include	"biog.h"

#define	LCD_TOTAL_ADDRESS_X		0x0d				// (14) LCD의 가로 byte수
#define	LCD_TOTAL_ADDRESS_Y		0x33				// (52) LCD의 세로 bit수

// 실제 LCD에 display되는 크기
#define	LCD_WIDTH				96					// LCD의 가로 bit수
#define	LCD_HEIGHT				LCD_TOTAL_ADDRESS_Y	// LCD의 세로 bit수

#define	LCD_COLUMN				16					// LCD의 가로 character수
#define	LCD_LINE				4

// LCD control port
#define	LCD_ON_PORT						DEC_GPIO_0
#define	LCD_ON_BIT_MASK					BIO_LCD_ON_M

#define	LCD_STB_PORT					DEC_GPIO_1
#define	LCD_STB_BIT_MASK				BIO_LCD_STB_M

#define	LCD_DATA_ADDRESS				( * (volatile byte*)( 0x2000000 ) )

// define LCD command
#define	LCD_COMMAND_RESET				0x27
#define	LCD_COMMAND_DUTY_SET			0x18
#define	LCD_COMMAND_DAC_SET				0x28		// must second data
	#define	LCD_DAC_DEFAULT				0x1e
#define	LCD_COMMAND_DISPLAY_ON			0x0f
#define	LCD_COMMAND_DISPLAY_OFF			0x08
#define	LCD_COMMAND_DUTY_51				0x18

#define	LCD_COMMAND_DOT_ADDRESS_SET		0xc0
#define	LCD_COMMAND_ADDRESS_SET			LCD_COMMAND_DOT_ADDRESS_SET
#define	LCD_COMMAND_ICON_ADDRESS_SET	0xd0
#define	LCD_COMMAND_ICON_BLINK_ADDRESS_SET	0xe0
#define	LCD_COMMAND_WRITE_MODE			0xb0
#define	LCD_COMMAND_READ_MODE			0xb5
