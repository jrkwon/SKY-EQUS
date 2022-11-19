
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
 *			LCD�� ���� buffer(like to video memory)�� ����
 *			LCD�� ����ϱ� ���� H/W control
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
 *				=> nDrawMode�� ���� LCD buffer�� DrawPattern�� ����
 *					- DrawText sub routene
 *					- �� file�� �� ������ LCD�� H/W�� ������ �ޱ� ����
 *
 *			void DisplayLCDBufferAll ( void );
 *				=> LCD buffer�� ������ LCD�� ���
 *
 *			void ClearLCDBufferAll ( void );
 *				=> LCD buffer�� ������ ��� clear
 *					- 0 ���� ä���
 *			void ClearLCDDataBufferAll ( void );
 *				=> LCD Data buffer�� ������ ��� clear
 *
 *			void FillLCDBufferAll ( void );
 *				=> LCD buffer�� ������ ��� set
 *					- 1 �� ä���
 *
 *			void BeginDraw ( void );
 *				 ~~~~~~~~~~~~~~~~~~
 *			void EndDraw ( void );
 *				 ~~~~~~~~~~~~~~~~~
 *			void BeginDrawWait ( void );
 *				 ~~~~~~~~~~~~~~~~~~~~~~
 *				=> ���� task���� LCD buffer�� write�ϴ� ���� ����
 *					- �ٸ� task�� ���� buffer�� ��� ������ �˸���
 *					- �ٸ� task�� buffer ����� ���������� ��ٸ�����
 *					   BeginDrawWait ()�� ����Ѵ�
 *					- ���� �Ϲ� task���� LCD�� write�� ���
 *					   mail�� LCD task�� ������ LCD task���� mail��
 *					   �޾� buffer�� access �Ѵ�
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
 *		LCD buffer�� ����ϰ� ������ �˸��� flag
 *		�� flag�� �̿��Ͽ� �ٸ� task�� ���ÿ� LCD buffer��
 *		access(write)�ϴ°��� ������
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
 *			- LCD buffer(LCDDataBuffer) �� 1 byte data�� 8 byte�� ��ȯ 
 *			- �� �Լ��� �ѽ����� �Լ��̴�. �÷� �����Ϳ� ���� ó���� ���Ŀ� ��� 
 *			- ���⼭ �÷��� ���� ó���� �� ���� �ִ�.			
 *
 *			- ��ȯ ��Ģ�� 0 -> 0x00, 1->0xFF(black)�� ��ȯ�Ѵ�. 
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
 *			- LCD buffer(LCDDataBuffer) �� 8 byte data�� 1 byte�� ��ȯ 
 *			- �� �Լ��� �ݺ������� ���� ������ �Ѵ�. 8���� ���ؾ� �ȴ�. 	
 *			- �� �Լ��� �ѽ����� �Լ��̴�. �÷� �����Ϳ� ���� ó���� ���Ŀ� ��� 
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
 *			- LCD buffer(LCDDataBuffer) �� data�� LCD�� ���
 *			- buffer ��ü�� data�� ���
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
	// Icon�� �ٸ� ��ȯ �Լ��� ��ĥ �� ������ ���⼱ ���� �Լ��� ��ġ�� ������ �Ѵ�.
	
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
 *			- LCD data, icon buffer�� clear(0���� ä��)
 *			- blink buffer�� clear
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
 *			- LCD data, icon buffer�� fill(1�� ä��)
 *			- blink buffer�� clear
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
 *			- ���ÿ� �� task �̻󿡼� LCD buffer�� write �ϴ� ���� ����
 *			- BeginDraw ()�� �ѹ� call�� ������ nUsingLCDBuffer�� 1 �����ϰ�
 *				EndDraw ()�� call�Ǹ� 1 �����ϹǷ� �� flag�� 0�� �Ǹ�
 *				���� buffer�� �ƹ��� ������� �ʴ´�
 *			- ���� �ٸ� task������ buffer ����� �������� ��ٸ�����
 *				BeginDrawWait ()�� ����Ѵ�
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
 *			- LCD buffer�� bits�� ���� (text font, bitmap�� �׸��� ���� ���)
 *			- IndexX, IndexY�� LCD buffer������ ��ġ
 *
 *		Input parameters
 *		----------------
 *
 *			int IndexX, IndexY
 *				- buffer������ ��ġ
 *			byte DrawPattern
 *				- buffer�� ������ data
 *			byte MaskBitPattern
 *				-DrawPattern���� �� data�� position�� bit length
 *
 *				 |    |			<- data ����
 *				..*.*...		<- DrawPattern(���� ������ bits)
 *				.******.		<- MaskBitPattern(mask bits)
 *
 *			****************	<- draw �� buffer
 *			*****.*.*..*****	<- draw �� buffer
 *				|      |		<- indexX, indexY�� ����Ű�� byte ����
 *
 *			int nDrawMode
 *				- draw mode
 *				- DRAW_NORMAL, DRAW_REVERSE, DRAW_BLINK ...
 *					(define in LCD_BIOS.h)
 *				- BLINK �迭�� mode�� ���� ����
 *					LCDBlinkON ()�� ����Ͽ����Ѵ�(blink�� timer �⵿)
 *					BLINK�� �ʿ� ������ �ݵ�� LCDBlinkOFF () call
 *					(blink�� timer ����)
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
	// nDrawMode�� ���� buffer�� write�ϴ� ����� �ٸ���
	switch	( nDrawMode )
	{
	// DRAW_NORMAL, DRAW_REVERSE�϶��� mask pattern ������ buffer�� clear�ϰ�
	//  draw pattern�� mode�� ���� draw
	// blink�� �����Ƿ� blink buffer�� clear
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

	// blink �迭�� mode���� ���� blink ����(�ֱ������� buffer�� blink
	//  ������ ���� �ϹǷ� 2���� ���°� �ִ�)�� ���� pattern�� draw�ϴ�
	//  ����� 2������ �ִ�
	// blink buffer���� �� draw mode�� ���� draw����� �ٸ���
	case DRAW_FOREGROUND_BLINK :
		// draw data buffer
		if ( ! bIsBufferReverse () )	// normal ����
		{
			// ���ڰ� blink�ϹǷ� normal mode�� ����
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] |= DrawPattern;
		}
		else							// reverse ����
		{
			// ���� blink�Ͽ� reverse ���¸� data buffer clear
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
		}

		// draw blink buffer (draw normal)
		// ���ڰ� blink�ϹǷ� blink buffer���� pattern�� draw
		LCDDataBlinkBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
		LCDDataBlinkBuffer[ indexY ][ indexX ] |= DrawPattern;
		break;

	case DRAW_BACKGROUND_BLINK :
		if ( ! bIsBufferReverse () )	// normal ����
		{
			// ������ blink�ϹǷ� normal mode�� ����
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] |= DrawPattern;
		}
		else							// reverse ����
		{
			// ���� blink�Ͽ� reverse ���¸� data buffer fill
			LCDDataBuffer[ indexY ][ indexX ] |= MaskBitPattern;
		}

		// draw blink buffer (draw reverse)
		// ������ blink�ϹǷ� blink buffer���� pattern�� reverse
		LCDDataBlinkBuffer[ indexY ][ indexX ] |= MaskBitPattern;
		LCDDataBlinkBuffer[ indexY ][ indexX ] ^= DrawPattern;
		break;

	case	DRAW_NORMAN_BLINK :
		// draw data buffer
		if ( ! bIsBufferReverse () )	// normal ����
		{
			// normal mode�� ����
			LCDDataBuffer[ indexY ][ indexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] |= DrawPattern;
		}
		else							// reverse ����
		{
			// ���� blink�Ͽ� reverse ���¸� reverse mode�� ����
			LCDDataBuffer[ indexY ][ indexX ] |= MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] ^= DrawPattern;
		}

		LCDDataBlinkBuffer[ indexY ][ indexX ] |= MaskBitPattern;
		break;

	case	DRAW_REVERSE_BLINK :
		if ( ! bIsBufferReverse () )	// normal ����
		{
			// reverse mode�� ����
			LCDDataBuffer[ indexY ][ indexX ] |= MaskBitPattern;
			LCDDataBuffer[ indexY ][ indexX ] ^= DrawPattern;
		}
		else							// reverse ����
		{
			// ���� blink�Ͽ� reverse ���¸� normal mode�� ����
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
 *	������ LCD data buffer blink ���¸� ��Ÿ���� 
 *		0 : normal ����
 *		1 : ���� ����(reverse)�� ����
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
 *			- ���� blink�� data�� �ִ°��� ����
 *			- LCDDataBlinkBuffer�� ������ 0�� �ƴϸ� TRUE
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
 *				- 1(TRUE)  : ���� blink�� data�� �ִ�
 *				  0(FALSE) : ���� blink�� data�� ����
 *
 *
 *===========================================================================*/

int bIsBlinkData ( void )
{
	int		bReturn = 0;		// �ʱⰪ (blink data�� ���ٰ� ����)
	int		index;

	// blink buffer�� data�� �˻��ϱ� ���� loop ( data buffer )
	for ( index = 0; index < sizeof ( LCDDataBuffer ) ; index ++ )
	{
		if ( *( (byte*)LCDDataBlinkBuffer + index ) != 0x00 )
		{
			bReturn = 1; // there is blink data in buffer.
			break;
		}
	}

		// blink buffer�� data�� �˻��ϱ� ���� loop ( icon buffer )
	if ( bReturn == 0 )
	{
		for ( index = 0; index < sizeof ( LCDIconBuffer ) ; index ++ )
		{
			if ( *( (byte*)LCDIconBlinkBuffer + index ) != 0x00 )
			{
				// LCDIconBlinkBuffer�� data�� 0x00�� �ƴϸ�(blink data ����)
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
 *			- LCD data buffer�� blink
 *			- LCDDataBlinkBuffer�� data�� set�Ǿ� �ִ� bit�� �ش��ϴ�
 *				LCDDataBuffer�� data�� ����
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

	// LCD data blink buffer length ��ŭ loop
	for ( index = 0; index < sizeof ( LCDDataBlinkBuffer ) ; index++ )
	{
		// blink bit �� �ִ��� Ȯ��
		if ( *( (byte*)LCDDataBlinkBuffer + index ) != 0x00 )
		{
			// blink bit position�� data bit�� ����
			*( (byte*)LCDDataBuffer + index ) ^=
				*( (byte*)LCDDataBlinkBuffer + index );
		}
	}

		// LCD Icon buffer length ��ŭ loop
	for ( index = 0; index < sizeof ( LCDIconBlinkBuffer ) ; index ++ )
	{
		// blink bit �� �ִ��� Ȯ��
		if ( *( (byte*)LCDIconBlinkBuffer + index ) != 0x00 )
		{
			// blink bit position�� data bit�� ����
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
 *			- blink mode�� ������ data buffer�� ���� ��������
 *				���� �������� ����
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
	// bIsBufferReverseFlag�� BlinkProcess () ���� blink�� ���� ����
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
 *			- LCD main task�� timeout mail�� ������ blink�� timer����
 *				�Ǵ��Ͽ� BlinkProcess ()�� call�Ѵ�
 *			- ���� timer�� 100 msec���� timeout ������ blink �ֱ��
 *				0.8 sec (100 msec * 8) �̴�
 *			- blink 1 �ֱ� ���� normal display�� reverse display��
 *				��� BLINK_REVERSE_TIME�� ���Ѵ�
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
	// blink�� normal display�� reverse display�� �� ����ϱ�����
	//	����ϴ� counter
	static int Count = 0;

	// ���� LCD data buffer�� ���´� bIsBufferReverseFlag�̰�
	//  �Ʒ��� flag�� �Լ� ���������θ� ���
	// blink time ������ �ٸ� task�� LCD buffer�� ����� �̶��
	//  blink�ֱ⸦ �����ϰ� ���� �ϱ� ���� data buffer�� �������� �ʴ´�
	static int bIsReverse = 0;

	Count ++;
	Count &= 0x07;		// 3bits�� mask�ϸ� 0x00 ~ 0x07�� ���� ��´�

	// normal display time = timeout��(100 msec) * BLINK_REVERSE_TIME
	// reverse display time = blink cycle(800 msec) - normal display time
	// �����	normal display time = 600 msec
	//			reverse display time = 200 msec
	if ( Count == 0 )
		bIsReverse = 0;		// normal display
	else if ( Count == BLINK_REVERSE_TIME )
		bIsReverse = 1;		// reverse display

	if ( UsingLCDBuffer == 0 )
	// LCD bufer�� ������� �ƴ� ����
	{
		// blink time ������ �ٸ� task�� LCD buffer�� ����� �̶��
		//  blink�ֱ⸦ �����ϰ� ���� �ϱ� ���� data buffer�� �������� �ʴ´�
		// LCDDataBlinkBuffer�� data�� ������ ���� ����
		if ( ( bIsReverse != bIsBufferReverseFlag ) &&
			( bIsBlinkData () ) )
		{
			// blink LCD data buffer
			BlinkLCDDataBuffer ();
			bIsBufferReverseFlag = bIsReverse;

			// ������ LCD data buffer�� data�� LCD�� ����
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
 *			- data�� ����� LCD�� address�� set
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
 *			- data�� ����� LCD�� icon address�� set
 *			- [nColumn] X [LCD_HEIGHT] matrix�� icon�� �ִ´�.
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
 *			- H/W ���� ��� set
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
	    2����, 8H,Normal Operation, �����, FRC
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


