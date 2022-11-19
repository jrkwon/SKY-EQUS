
/*****************************************************************************
 *
 *
 *		LCD BIOS module for Color LCD
 *
 *
 *		Copyright (C) 1998, 1999, 2000 SK Teletech
 *
 *
 *****************************************************************************
 *
 *
 *		FileName : LCD_BIOSB1.c
 *		Revision : 0.00.004
 *		 Created : 00.06.26
 *
 *
 *		Module description
 *		------------------
 *
 *			LCD를 위한 buffer(like to video memory)를 관리
 *			LCD를 사용하기 위한 H/W control
 *
 *
 *****************************************************************************
 *
 *
 *		Global functions from this module
 *		---------------------------------
 *
 *		LCD buffer control routins
 *
 *			void DrawBitToLCDBuffer ( int indexX, int indexY,
 *					byte DrawPattern, byte MaskBitPattern,
 *					int nDrawMode );
 *				=> nDrawMode에 따라 LCD buffer에 DrawPattern을 저장
 *					- DrawText sub routene
 *					- 이 file에 둔 이유는 LCD의 H/W에 영향을 받기 때문
 *
 *			void DisplayLCDBufferAll ( void );
 *				=> LCD buffer의 내용을 LCD로 출력
 *
 *			void ClearLCDBufferAll ( void );
 *				=> LCD buffer의 내용을 모두 clear
 *					- 0 으로 채운다
 *			void ClearLCDDataBufferAll ( void );
 *				=> LCD Data buffer의 내용을 모두 clear
 *
 *			void FillLCDBufferAll ( void );
 *				=> LCD buffer의 내용을 모두 set
 *					- 1 로 채운다
 *
 *			void BeginDraw ( void );
 *				 ~~~~~~~~~~~~~~~~~~
 *			void EndDraw ( void );
 *				 ~~~~~~~~~~~~~~~~~
 *			void BeginDrawWait ( void );
 *				 ~~~~~~~~~~~~~~~~~~~~~~
 *				=> 여러 task에서 LCD buffer에 write하는 것을 방지
 *					- 다른 task에 현재 buffer가 사용 중임을 알린다
 *					- 다른 task의 buffer 사용이 끝날때까지 기다리려면
 *					   BeginDrawWait ()를 사용한다
 *					- 현재 일반 task들은 LCD에 write할 경우
 *					   mail을 LCD task로 보내고 LCD task만이 mail을
 *					   받아 buffer을 access 한다
 *
 *
  *****************************************************************************
 *
 *
 *		This module used external functions and constants and macros
 *		------------------------------------------------------------
 *
 *			from Common.h
 *				LCD_TYPE
 *
 *
 *****************************************************************************/
 
/* tasik 00.07.10, LCD control for B1 */

#define __LCD_BIOS_MODULE


/*****************************************************************************
 *
 *		Include external header files
 *
 *****************************************************************************/

#include "target.h"
#include <string.h>
#include "LCD_BIOSB1.h"


/*****************************************************************************
 *
 *		Predefine local functions prototype
 *
 *****************************************************************************/

// LCD blinking control routins
static int	bIsBlinkData ( void );
static void BlinkLCDDataBuffer ( void );
static int	bIsBufferReverse ( void );

// LCD data Convert/Reconvert to send LCD chip RAM
static uint64 LCDDataConvert(byte);
static byte LCDDataReconvert(uint64, int);

/*****************************************************************************
 *
 *
 *		LCD buffer control routins
 *
 *			ClearLCDBufferAll ()
 *			ClearLCDDataBufferAll ()
 *			FillLCDBufferAll ()
 *			DisplayLCDBufferAll ()
 *
 *
 *****************************************************************************/


/*****************************************************************************
 *
 *		Define local variables
 *
 *****************************************************************************/

/*
 *		LCD data buffer
 *		bit = 1 -> LCD bit on (black)
 */
byte	LCDDataBuffer
			[ LCD_TOTAL_PAGE_SIZE ][ LCD_TOTAL_COLUMN_SIZE ];
byte	LCDAniBuffer
			[ LCD_TOTAL_PAGE_SIZE ][ LCD_TOTAL_COLUMN_SIZE ];
byte	LCDIconBuffer[LCD_TOTAL_COLUMN_SIZE];

/*
 *		LCD data blink buffer
 */
byte	LCDDataBlinkBuffer
			[ LCD_TOTAL_PAGE_SIZE ][ LCD_TOTAL_COLUMN_SIZE ];
byte	LCDAniBlinkBuffer
			[ LCD_TOTAL_PAGE_SIZE ][ LCD_TOTAL_COLUMN_SIZE ];
byte    LCDIconBlinkBuffer[ LCD_TOTAL_COLUMN_SIZE ];

/*
 *		LCD buffer를 사용하고 있음을 알리는 flag
 *		이 flag를 이용하여 다른 task와 동시에 LCD buffer를
 *		access(write)하는것을 방지함
 */
static int UsingLCDBuffer = 0;


/*****************************************************************************
 *
 *		Functions
 *
 *****************************************************************************/
/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			LCDDataConvert 
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD buffer(LCDDataBuffer) 의 1 byte data를 8 byte로 변환 
 *			- 이 함수는 한시적인 함수이다. 컬러 데이터에 대한 처리는 차후에 고려 
 *			- 여기서 컬러에 대한 처리를 할 수도 있다.			
 *
 *			- 변환 규칙은 0 -> 0x00, 1->0xFF(black)로 변환한다. 
 *
 *		Input parameters
 *		----------------
 *
 *			1 byte LCD data
 *
 *
 *		Return
 *		------
 *
 *			8 byte Color LCD data
 *
 *
 *
 *===========================================================================*/
#ifdef LCD_CONVERT_BIT_TO_BYTE
uint64 LCDDataConvert(byte data)
{
	
	uint64		output=0x0000;
	int			tmp, index;
	byte		extract_LSB=0x01;

	for (index=0 ; index<sizeof(byte) ; index++)
	{
		tmp = data >> index;
		tmp = tmp & extract_LSB;
		
		if(tmp==1)
		{
			tmp=0xff;
		}
		else
		{
			tmp=0x00;
		}
		
		output=output|(tmp<<(index*sizeof(byte)));
	}

	return output;

}
/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			LCDDataReconvert 
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD buffer(LCDDataBuffer) 의 8 byte data를 1 byte로 변환 
 *			- 이 함수는 반복구문과 같이 쓰여야 한다. 8번을 행해야 된다. 	
 *			- 이 함수는 한시적인 함수이다. 컬러 데이터에 대한 처리는 차후에 고려 
 *							
 *
 *
 *		Input parameters
 *		----------------
 *
 *			8 byte Color LCD data
 *
 *
 *		Return
 *		------
 *
 *			1 byte Color LCD data
 *
 *
 *
 *===========================================================================*/
byte LCDDataReconvert(uint64 data, int index)
{
	uint64 extract_LSByte=0x000f;
	byte output=0x00;

	output = (byte)( (data >> index) & extract_LSByte );
	
	return output;
}

#endif // #ifdef LCD_CONVERT_BIT_TO_BYTE


/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			DisplayLCDBufferAll
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD buffer(LCDDataBuffer) 의 data를 LCD로 출력
 *			- buffer 전체의 data를 출력
 *
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *
 *===========================================================================*/

void DisplayLCDBufferAll ( void )
{
	int index;
#ifdef LCD_CONVERT_BIT_TO_BYTE
	uint64	converted_data;
	int		byte_shift;
	int		uint64_bit_number = 64, byte_bit_number=4;
	byte    color_data;
#endif
	// data area //

	SetLCDAddress(LCD_ORIGIN, LCD_WIDTH, LCD_ORIGIN, LCD_HEIGHT);
	// HOWEVER, above setting is not necessary.
	// Because default address setting is done at InitialLCD.

#ifdef LCD_CONVERT_BIT_TO_BYTE
	// entry of write mode
	LCD_OUTP_CMD(RAM_WRITE);
	for (index=0 ; index < sizeof(LCDDataBuffer) ; index++)
	{
		converted_data = LCDDataConvert(*((byte*) LCDDataBuffer + index));

		byte_shift=0;
		while (byte_shift < uint64_bit_number)
		{
			color_data = LCDDataReconvert(converted_data,byte_shift);
			LCD_OUTP_DATA(color_data);

			byte_shift=byte_shift+byte_bit_number;
		}
	}

#else

	// entry of write mode
	LCD_OUTP_CMD(RAM_WRITE);

	for ( index = 0; index < sizeof ( LCDDataBuffer ) ; index++ )
	{
		// LCD buffer -> LCD RAM transfer
		LCD_OUTP_DATA(*( (byte *) LCDDataBuffer + index)); 
	}
#endif // #ifdef LCD_CONVERT_BIT_TO_BYTE


#ifdef FEATURE_NUI_D1
	/* Do nothing */
#else

	// icon area //

	SetLCDIconAddress(LCD_ORIGIN,LCD_ICON_HEIGHT);

#ifdef LCD_CONVERT_BIT_TO_BYTE

	// entry of write mode
	// Icon은 다른 변환 함수를 거칠 수 있지만 여기선 같은 함수를 거치는 것으로 한다.
	
	LCD_OUT_CMD(RAM_WRITE);
	for (index=0 ; index < sizeof(LCDIconBuffer) ; index++)
	{
		converted_data = LCDDataConvert(*((byte*) LCDIconBuffer + index));
	
		for (byte_shift=0 ; byte_shift < sizeof(uint64) ; byte_shift+sizeof(byte))
		{
			color_data = LCDDataReconvert(converted_data,byte_shift);
			LCD_OUTP_DATA(color_data);
		}
	}

#else // #ifdef LCD_CONVERT_BIT_TO_BYTE
	
	LCD_OUTP_CMD(RAM_WRITE);
	for ( index = 0; index < sizeof ( LCDIconBuffer ) ; index++ )
	{
		// LCD buffer -> LCD RAM transfer
		LCD_OUTP_DATA(*( (byte *) LCDIconBuffer + index)); 
	}

#endif // #ifdef LCD_CONVERT_BIT_TO_BYTE

#endif // #ifdef FEATURE_NUI_D1
}

void CopyLCDBufferAll (void)
{
	BeginDraw ();	// occupy LCD buffer
	memcpy ( (byte *)LCDDataBlinkBuffer, (byte *)LCDAniBlinkBuffer, sizeof ( LCDDataBuffer ) );
	memcpy ( (byte *)LCDDataBuffer,      (byte *)LCDAniBuffer,      sizeof ( LCDDataBuffer ) );
	EndDraw ();		// free LCD buffer

	DisplayLCDBufferAll ();
}

/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			ClearLCDDataBufferAll
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD data, icon buffer를 clear(0으로 채움)
 *			- blink buffer도 clear
 *
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *
 *===========================================================================*/
void ClearLCDBufferAll(void)
{
	ClearLCDDataBufferAll();
	ClearLCDIconBufferAll();
}


void ClearLCDDataBufferAll ( void )
{
	BeginDraw ();	// occupy LCD buffer

	// clear LCD data/blink buffer
	memset ( (byte*)LCDDataBuffer, 0x00, sizeof ( LCDDataBuffer ) );
	memset ( (byte*)LCDDataBlinkBuffer, 0x00, sizeof ( LCDDataBlinkBuffer ) );

	EndDraw ();		// free LCD buffer
}

void ClearLCDIconBufferAll(void) //
{
	BeginDraw();

	//clear LCD icon data/blink buffer

	memset((byte*) LCDIconBuffer, 0x00 , sizeof(LCDIconBuffer));
	memset((byte*) LCDIconBlinkBuffer, 0x00 , sizeof(LCDIconBuffer));
	
	EndDraw();
}

/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			FillLCDBufferAll
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD data, icon buffer를 fill(1로 채움)
 *			- blink buffer는 clear
 *
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *
 *===========================================================================*/

void FillLCDBufferAll ( void )
{
	BeginDraw ();	// occupy LCD buffer

	// set LCD data buffer
	memset ( (byte*)LCDDataBuffer, 0xff, sizeof ( LCDDataBuffer ) );
	// clear LCD blink buffer, all blink off
	memset ( (byte*)LCDDataBlinkBuffer, 0x00, sizeof ( LCDDataBlinkBuffer ) );

	// set LCD icon buffer
	memset ( (byte*)LCDIconBuffer, 0xff, sizeof ( LCDIconBuffer ) );
	// clear LCD icon blink buffer, all blink off
	memset ( (byte*)LCDIconBlinkBuffer, 0x00, sizeof ( LCDIconBlinkBuffer ) );

	EndDraw ();		// free LCD buffer
}


/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			BeginDraw
 *			EndDraw
 *			BeginDrawWait
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- 동시에 두 task 이상에서 LCD buffer에 write 하는 것을 방지
 *			- BeginDraw ()가 한번 call될 때마다 nUsingLCDBuffer를 1 증가하고
 *				EndDraw ()가 call되면 1 감소하므로 이 flag가 0이 되면
 *				현재 buffer는 아무도 사용하지 않는다
 *			- 실제 다른 task에서의 buffer 사용이 끝날때를 기다리려면
 *				BeginDrawWait ()를 사용한다
 *
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *
 *===========================================================================*/
void BeginDraw ( void )
{
	UsingLCDBuffer++;
}

void EndDraw ( void )
{
	if ( UsingLCDBuffer > 0 ) 
		UsingLCDBuffer--;
}

void BeginDrawWait ( void )
{
	while ( UsingLCDBuffer > 0 )
		;

	BeginDraw ();	// occupy LCD buffer
}



/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			DrawBitToLCDBuffer
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD buffer에 bits를 저장 (text font, bitmap을 그리기 위해 사용)
 *			- IndexX, IndexY는 LCD buffer에서의 위치
 *
 *		Input parameters
 *		----------------
 *
 *			int IndexX, IndexY
 *				- buffer에서의 위치
 *			byte DrawPattern
 *				- buffer에 저장할 data
 *			byte MaskBitPattern
 *				-DrawPattern에서 실 data의 position과 bit length
 *
 *				 |    |			<- data 영역
 *				..*.*...		<- DrawPattern(실재 저장할 bits)
 *				.******.		<- MaskBitPattern(mask bits)
 *
 *			****************	<- draw 전 buffer
 *			*****.*.*..*****	<- draw 후 buffer
 *				|      |		<- indexX, indexY가 가리키는 byte 영역
 *
 *			int nDrawMode
 *				- draw mode
 *				- DRAW_NORMAL, DRAW_REVERSE, DRAW_BLINK ...
 *					(define in LCD_BIOS.h)
 *				- BLINK 계열의 mode를 사용시 먼저
 *					LCDBlinkON ()을 사용하여야한다(blink용 timer 기동)
 *					BLINK가 필요 없을시 반드시 LCDBlinkOFF () call
 *					(blink용 timer 해제)
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/

void	DrawBitToLCDBuffer ( int indexX, int indexY,
				 byte DrawPattern, byte MaskBitPattern,
				 int nDrawMode )
{
	// nDrawMode에 따라 buffer에 write하는 방법이 다르다
	switch	( nDrawMode )
	{
	// DRAW_NORMAL, DRAW_REVERSE일때는 mask pattern 영역의 buffer를 clear하고
	//  draw pattern을 mode에 따라 draw
	// blink는 없으므로 blink buffer는 clear
	case	DRAW_NORMAL :
	default :
		// draw data buffer
		LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
		LCDDataBuffer[ indexY ][ indexX ] |= DrawPattern;

		// draw blink buffer (clear)
		// none blink
		LCDDataBlinkBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
		break;

	case	DRAW_REVERSE :
		// draw data buffer
		LCDDataBuffer[ indexY ][ indexX ] |= MaskBitPattern;
		LCDDataBuffer[ indexY ][ indexX ] ^= DrawPattern;

		// draw blink buffer (clear)
		// none blink
		LCDDataBlinkBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
		break;

	// blink 계열의 mode들은 현재 blink 상태(주기적으로 buffer의 blink
	//  영역을 반전 하므로 2가지 상태가 있다)에 따라 pattern을 draw하는
	//  방법이 2가지가 있다
	// blink buffer에도 각 draw mode에 따라 draw방법이 다르다
	case DRAW_FOREGROUND_BLINK :
		// draw data buffer
		if ( ! bIsBufferReverse () )	// normal 상태
		{
			// 글자가 blink하므로 normal mode와 같다
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] |= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 data buffer clear
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
		}

		// draw blink buffer (draw normal)
		// 글자가 blink하므로 blink buffer에는 pattern을 draw
		LCDDataBlinkBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
		LCDDataBlinkBuffer[ indexY ][ indexX ] |= DrawPattern;
		break;

	case DRAW_BACKGROUND_BLINK :
		if ( ! bIsBufferReverse () )	// normal 상태
		{
			// 바탕이 blink하므로 normal mode와 같다
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] |= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 data buffer fill
			LCDDataBuffer[ indexY ][ indexX ] |= MaskBitPattern;
		}

		// draw blink buffer (draw reverse)
		// 바탕이 blink하므로 blink buffer에는 pattern을 reverse
		LCDDataBlinkBuffer[ indexY ][ indexX ] |= MaskBitPattern;
		LCDDataBlinkBuffer[ indexY ][ indexX ] ^= DrawPattern;
		break;

	case	DRAW_NORMAN_BLINK :
		// draw data buffer
		if ( ! bIsBufferReverse () )	// normal 상태
		{
			// normal mode와 같다
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] |= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 reverse mode와 같다
			LCDDataBuffer[ indexY ][ indexX ] |= MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] ^= DrawPattern;
		}

		LCDDataBlinkBuffer[ indexY ][ indexX ] |= MaskBitPattern;
		break;

	case	DRAW_REVERSE_BLINK :
		if ( ! bIsBufferReverse () )	// normal 상태
		{
			// reverse mode와 같다
			LCDDataBuffer[ indexY ][ indexX ] |= MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] ^= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 normal mode와 같다
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] |= DrawPattern;
		}

		LCDDataBlinkBuffer[ indexY ][ indexX ] |= MaskBitPattern;
		break;
	}
}


/*************************************************************************
 *
 *
 *		LCD blinking control routins
 *
 *			bIsBlinkData ()
 *			BlinkLCDDataBuffer ()
 *
 *
 *************************************************************************/

/*****************************************************************************
 *
 *		Define local variables
 *
 *****************************************************************************/

/*
 *	현재의 LCD data buffer blink 상태를 나타낸다 
 *		0 : normal 상태
 *		1 : 현재 반전(reverse)된 상태
 */
static int	bIsBufferReverseFlag = 0;


/*****************************************************************************
 *
 *		Functions
 *
 *****************************************************************************/

/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			bIsBlinkData
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- 현재 blink할 data가 있는가를 판정
 *			- LCDDataBlinkBuffer의 내용이 0이 아니면 TRUE
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			int(boolean)
 *				- 1(TRUE)  : 현재 blink할 data가 있다
 *				  0(FALSE) : 현재 blink할 data가 없다
 *
 *
 *===========================================================================*/

int bIsBlinkData ( void )
{
	int		bReturn = 0;		// 초기값 (blink data가 없다고 가정)
	int		index;

	// blink buffer의 data를 검색하기 위한 loop ( data buffer )
	for ( index = 0; index < sizeof ( LCDDataBuffer ) ; index ++ )
	{
		if ( *( (byte*)LCDDataBlinkBuffer + index ) != 0x00 )
		{
			bReturn = 1; // there is blink data in buffer.
			break;
		}
	}

		// blink buffer의 data를 검색하기 위한 loop ( icon buffer )
	if ( bReturn == 0 )
	{
		for ( index = 0; index < sizeof ( LCDIconBuffer ) ; index ++ )
		{
			if ( *( (byte*)LCDIconBlinkBuffer + index ) != 0x00 )
			{
				// LCDIconBlinkBuffer의 data가 0x00이 아니면(blink data 존재)
				//  1(TRUE) return
				bReturn = 1;
				break;
			}
		}
	}


	return ( bReturn );
}

/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			BlinkLCDDataBuffer
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD data buffer를 blink
 *			- LCDDataBlinkBuffer의 data중 set되어 있는 bit에 해당하는
 *				LCDDataBuffer의 data를 반전
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/

void BlinkLCDDataBuffer ( void )
{
	int	index;

	BeginDraw ();	// occupy LCD buffer

	// LCD data blink buffer length 만큼 loop
	for ( index = 0; index < sizeof ( LCDDataBlinkBuffer ) ; index++ )
	{
		// blink bit 가 있는지 확인
		if ( *( (byte*)LCDDataBlinkBuffer + index ) != 0x00 )
		{
			// blink bit position의 data bit를 반전
			*( (byte*)LCDDataBuffer + index ) ^=
				*( (byte*)LCDDataBlinkBuffer + index );
		}
	}

		// LCD Icon buffer length 만큼 loop
	for ( index = 0; index < sizeof ( LCDIconBlinkBuffer ) ; index ++ )
	{
		// blink bit 가 있는지 확인
		if ( *( (byte*)LCDIconBlinkBuffer + index ) != 0x00 )
		{
			// blink bit position의 data bit를 반전
			*( (byte*)LCDIconBuffer + index ) ^=
				*( (byte*)LCDIconBlinkBuffer + index );
		}
	}

	EndDraw ();		// free LCD buffer
}


/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			bIsBufferReverse
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- blink mode중 현재의 data buffer가 정상 상태인지
 *				반전 상태인지 판정
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/

int	bIsBufferReverse ( void )
{
	// bIsBufferReverseFlag는 BlinkProcess () 에서 blink시 마다 반전
	return ( bIsBufferReverseFlag );
}


/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			BlinkProcess
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- blink timer ISR
 *			- LCD main task가 timeout mail을 받으면 blink용 timer인지
 *				판단하여 BlinkProcess ()를 call한다
 *			- 실제 timer는 100 msec마다 timeout 되지만 blink 주기는
 *				0.8 sec (100 msec * 8) 이다
 *			- blink 1 주기 에서 normal display와 reverse display의
 *				비는 BLINK_REVERSE_TIME로 정한다
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/

void BlinkProcess ( void )
{
	// blink시 normal display와 reverse display의 비를 계산하기위해
	//	사용하는 counter
	static int Count = 0;

	// 실제 LCD data buffer의 상태는 bIsBufferReverseFlag이고
	//  아래의 flag는 함수 내부적으로만 사용
	// blink time 에서도 다른 task가 LCD buffer를 사용중 이라면
	//  blink주기를 일정하게 유지 하기 위해 data buffer를 반전하지 않는다
	static int bIsReverse = 0;

	Count ++;
	Count &= 0x07;		// 3bits를 mask하면 0x00 ~ 0x07의 값을 얻는다

	// normal display time = timeout값(100 msec) * BLINK_REVERSE_TIME
	// reverse display time = blink cycle(800 msec) - normal display time
	// 현재는	normal display time = 600 msec
	//			reverse display time = 200 msec
	if ( Count == 0 )
		bIsReverse = 0;		// normal display
	else if ( Count == BLINK_REVERSE_TIME )
		bIsReverse = 1;		// reverse display

	if ( UsingLCDBuffer == 0 )
	// LCD bufer가 사용중이 아닐 때만
	{
		// blink time 에서도 다른 task가 LCD buffer를 사용중 이라면
		//  blink주기를 일정하게 유지 하기 위해 data buffer를 반전하지 않는다
		// LCDDataBlinkBuffer에 data가 존재할 때만 반전
		if ( ( bIsReverse != bIsBufferReverseFlag ) &&
			( bIsBlinkData () ) )
		{
			// blink LCD data buffer
			BlinkLCDDataBuffer ();
			bIsBufferReverseFlag = bIsReverse;

			// 실제로 LCD data buffer의 data를 LCD로 전송
			DisplayLCDBufferAll ();
		}
	}
}



/*****************************************************************************
 *
 *		Functions
 *
 *****************************************************************************/




/*===========================================================================*
 *
 *		Function name
 *		-------------
 *		1. LCDDisplayOn
 *		2. LCDDisplayOff
 *		3. SetLCDAddress
 *		4. SetLCDIconAddress
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *			1. LCD display at Power On
 *			2,3. LCD diaplay on/off at Normal Time
 *
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/
void	LCDDisplayOn ( void )
{
	/*
		This function is performed at power-on.
		Sleep_In is the default setting at power-on so that
		we should first take the LCD out of Sleep Mode.

		The operation of Display-on is done at the initial power-up and 
		at sleep-out. Thus, this module includes Sleep-out command.
		
		Display-on turns on the whole panel which range is determined by
		the address-set command.

	*/
	LCD_OUTP_CMD(SLEEP_OUT);
	LCD_OUTP_CMD(DISPLAY_ON);
}

void	LCDDisplayOff ( void )
{
	/*	
		This function includes powerdown mode. 
		Sleep_In should be done AFTER Display-Off.
	*/
	LCD_OUTP_CMD(DISPLAY_OFF);
	LCD_OUTP_CMD(SLEEP_IN);
}
/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			SetDisplayAddress
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- data를 출력할 LCD의 address를 set
 *			
 *
 *		Input parameters
 *		----------------
 *
 *			byte StartColumn
 *				- Start Column position (unit: byte ) 
 *				- Red (3bit) Green (3bit) Blue(2bit)
 *			byte EndColumn
 *				- End Column position (byte) 
 *				- Red (3bit) Green (3bit) Blue(2bit)
 *
 *			byte StartPage
 *				- Start Page position (a line)
 *				- 4 lines is 1 block
 *			byte EndPage
 *				- End Page position 
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/

void	SetLCDAddress ( byte StartColumn, byte EndColumn,byte StartPage, byte EndPage)
{

	/*
		column = X,	page = line, Y
		
		%% This function can set the specific addresses at which the display 
		starts to be on.
		
		%% The default end column/page (LCD_WIDTH,LCD_HEIGHT) is defined in LCD_B1B1.h	
	
		% nColumn = start column, nPage = start page to display

	*/

	// column address set
	LCD_OUTP_CMD(COLUMN_ADDRESS_SET);
	LCD_OUTP_DATA(StartColumn);
	LCD_OUTP_DATA(EndColumn);	
	// page address set
	LCD_OUTP_CMD(PAGE_ADDRESS_SET);
	LCD_OUTP_DATA(StartPage);
	LCD_OUTP_DATA(EndPage);
}

/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			SetLCDIconAddress
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- data를 출력할 LCD의 icon address를 set
 *			- [nColumn] X [LCD_HEIGHT] matrix에 icon을 넣는다.
 *			
 *
 *		Input parameters
 *		----------------
 *
 *			byte nColumn
 *		
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/

void	SetLCDIconAddress (byte nColumn)
{
	
	// column address set (X axis)
	LCD_OUTP_CMD(COLUMN_ADDRESS_SET);
	LCD_OUTP_DATA(nColumn);
	LCD_OUTP_DATA(LCD_ICON_WIDTH);	

	// page address set (Y axis)
	LCD_OUTP_CMD(PAGE_ADDRESS_SET);
	LCD_OUTP_DATA(LCD_ORIGIN);
	LCD_OUTP_DATA(LCD_ICON_HEIGHT);
}



/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *				InitialLCD
 *
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD initialize
 *			- H/W 선택 사양 set
 *			- LCD display on
 *
 *
 *		Input parameters
 *		----------------
 *
 *			NONE
 *
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/

void	InitialLCD ( void )
{
/*
	DISCTL: display control

    P1: 0x00
	    2분주, 8H,Normal Operation, 기순류, FRC
    P2: 0x1D
	    duty = 1/120
		block = 29
    P3: 0x0B
	    inverted line number = 12
*/
	LCD_OUTP_CMD(DISPLAY_CONTROL);		// 
	LCD_OUTP_DATA(F12_OP_PATTERN_8H|DIVIDED_PERIOD_2|F12_NORMAL_MODE|    \
		          FRC_NORMAL_MODE|FRC_FORWARD_FLOW|FRC_FRAME_FLOW);
	LCD_OUTP_DATA(LCD_DUTY_1_120);
	LCD_OUTP_DATA(INVERTED_LINE_12);
/* 
	VOLCTR : volumn control (contrast)
	P1: 0x36
		UP 
		22 step voltages
*/
	LCD_OUTP_CMD(VOLUMN_CONTROL);		 
	LCD_OUTP_DATA(STEP_VOLT_NUMBER_22|CONTRAST_UP_FROM_CURRENT);
/*
	DATCTL: data control
	P1: 0x02
		page 0 location = fist line
		
		120th(0th) column = SEG 1,2,3
		239th(119th) column = SEG 358,359,360

		scan direction = Column-ward
	P2: 0x01
		RGB arrangement = BGR BGR in this order
		-> because above setting produces more color-sensitive output.
*/
	LCD_OUTP_CMD(DATA_CONTROL);		 
	LCD_OUTP_DATA(LOCATION_0_PAGE_LN1|LOCATION_120_COLUMN_SEG123|COLUMN_SCAN_DIRECTION |  \
				  CONVERT_01_TO_001|CONVERT_10_TO_100);
	LCD_OUTP_DATA(BGR_BGR_1_ARRANGEMENT);

	// column/page address set
	SetLCDAddress(FIRST_COLUMN_ADDRESS,LAST_COLUMN_ADDRESS,FIRST_PAGE_ADDRESS,LAST_PAGE_ADDRESS);
	
	// The default display is inverted one so that 
	LCD_OUTP_CMD(DISPLAY_INVERSE);

	/* LCD display on at initial power on */
	LCDDisplayOn();
}


