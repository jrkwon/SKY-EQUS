
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
 *		FileName : LCD_BIOS.c
 *		Revision : 0.00.004
 *		 Created : 98.03.09
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
 *			void DrawBitToLCDBuffer ( int nIndexX, int nIndexY,
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
 *			void ClearLCDIconBufferAll ( void );
 *				=> LCD ICON buffer�� ������ ��� clear
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
 *		LCD blinking control routins
 *
 *			void LCDBlinkON ( void );
 *			void LCDBlinkOFF ( void );
 *				=> LCD�� blink mode�� �����ϱ� ���� timer�� ����
 *					- LCD init routine�� LCDBlinkON() �߰� �Ͽ���
 *					- blink mode �̻��� LCDBlinkOFF () ��� ����
 *
 *			byte GetBlinkTimerID ( void );
 *				=> ���� �������� LCD blink�� timer�� ID�� ����
 *					- timer�� disable�̶�� �� ���� 0
 *					- tim_cmd() function�� �߰� �Ͽ���
 *
 *			void BlinkProcess ( void );
 *				=> LCD blink routine
 *					- tim_cmd() function�� �߰� �Ͽ���
 *					- blink timer�� on�Ǹ� blink routine�� ����
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
 *****************************************************************************
 *
 *
 *		Revision detail
 *		---------------
 *
 *			Rev	0.00.004	98.04.14 10:00	chos
 *				added icon�� functions
 *
 *			Rev	0.00.003	98.04.13 11:30	chos
 *				added SDP700�� functions
 *
 *			Rev	0.00.002	98.03.18 15:00	chos
 *				added NEC LCD control�� functions
 *
 *			Rev	0.00.001	98.03.09 16:25	chos
 *				created this module
 *
 *
 *****************************************************************************/

#define __LCD_BIOS_MODULE


/*****************************************************************************
 *
 *		Include external header files
 *
 *****************************************************************************/

#include "target.h"
#include <string.h>
#include "LCD_BIOS.h"
#include "clk.h"

#if (MODEL_ID==MODEL_A10)
#include "lcd_biosa10.c"
#else
/*****************************************************************************
 *
 *		Predefine local functions prototype
 *
 *****************************************************************************/

// LCD blinking control routins
static int	bIsBlinkData ( void );
static void BlinkLCDDataBuffer ( void );
static int	bIsBufferReverse ( void );

// LCD control routins
static void	LCDCommandDataEnable ( void );
static void	LCDCommandDataDisable ( void );

static void	SetLCDAddress ( int nAddressX, int nAddressY );
static void	SetLCDIconAddress ( int nSeg );
static void	SetLCDIconBlinkAddress ( int nSeg );


/*****************************************************************************
 *
 *
 *		LCD buffer control routins
 *
 *			ClearLCDBufferAll ()
 *			ClearLCDDataBufferAll ()
 *			ClearLCDIconBufferAll ()
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
 *	LCD�� ����� buffer ����
 *		LCD data buffer
 *		bit �� 1 �̸� LCD bit on (black)
 */
byte	LCDDataBuffer
			[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ];

////////////////////////////////////////////////////////////////
//	Insert by shinji 98/07/10	Animation update directly
byte	LCDAniBuffer
			[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ];
byte	LCDAniBlinkBuffer
			[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ];

byte	LCDIconBuffer[ LCD_TOTAL_ADDRESS_X ];

/*
 *		LCD data blink buffer
 *		bit �� 1 �̸� LCD bit blink
 */
byte	LCDDataBlinkBuffer
	[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ];

byte	LCDIconBlinkBuffer[ LCD_TOTAL_ADDRESS_X ];

/*
 *		LCD buffer�� ����ϰ� ������ �˸��� flag
 *		�� flag�� �̿��Ͽ� �ٸ� task�� ���ÿ� LCD buffer��
 *		access(write)�ϴ°��� ������
 */
static int nUsingLCDBuffer = 0;


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

/*
 *	LCD buffer(LCDDataBuffer) �� data�� LCD�� ���
 *	buffer ��ü�� data�� ���
 */
void DisplayLCDBufferAll ( void )
{
//	long	nInterruptStatus;
	int		nIndex;

//	// interrupt disable
//	nInterruptStatus = NU_Control_Interrupts ( SY_ALL_DIS );

	//
	//	draw data area
	//

	// set LCD address upper left postion
	SetLCDAddress ( 0, 0 );

	LCDCommandDataEnable ();

	// set write mode
	LCD_DATA_ADDRESS = LCD_COMMAND_WRITE_MODE;
	// LCD buffer�� ���� LCD�� �����ϱ� ���� loop
	for ( nIndex = 0; nIndex < sizeof ( LCDDataBuffer ) ; nIndex ++ )
	{
		// LCD buffer�� ���� LCD�� ����
		LCD_DATA_ADDRESS = *( (byte*)LCDDataBuffer + nIndex );
	}

	LCDCommandDataDisable ();


    /* jrkwon 2000-05-16 --> @why, what */
#ifdef FEATURE_NUI_D1 
    /* do nothing */
#else
    //
	//	draw icon area
	//

	// set LCD icon address
	SetLCDIconAddress ( 0 );

	LCDCommandDataEnable ();
	// set write mode
	LCD_DATA_ADDRESS = LCD_COMMAND_WRITE_MODE;
	// LCD buffer�� ���� LCD�� �����ϱ� ���� loop
	for ( nIndex = 0; nIndex < sizeof ( LCDIconBuffer ) ; nIndex ++ )
	{
		// LCD buffer�� ���� LCD�� ����
		LCD_DATA_ADDRESS = *( (byte*)LCDIconBuffer + nIndex );
	}
	LCDCommandDataDisable ();
#endif /* FEATURE_NUI_D1 */
    /* <-- */

//	NU_Control_Interrupts ( nInterruptStatus );

	if ( bIsBlinkData () )
		LCDBlinkTimerStart ();
	else
		LCDBlinkTimerStop ();
}
#ifdef FEATURE_NUI_D1
////////////////////////////////////////////////////////////////
//	Insert by shinji 98/07/10	Copy animation buffer directly
void CopyLCDBufferAll (void)
{
	//BeginDraw ();	// occupy LCD buffer
	memcpy ( (byte *)LCDDataBlinkBuffer, (byte *)LCDAniBlinkBuffer, sizeof ( LCDDataBuffer ) );
	memcpy ( (byte *)LCDDataBuffer,      (byte *)LCDAniBuffer,      sizeof ( LCDDataBuffer ) );
	//EndDraw ();		// free LCD buffer

	DisplayLCDBufferAll ();
	LCDBlinkTimerStart();
}
#endif
/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			ClearLCDBufferAll
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

void ClearLCDBufferAll ( void )
{
	ClearLCDDataBufferAll ();
	ClearLCDIconBufferAll ();
}

void ClearLCDDataBufferAll ( void )
{
	BeginDraw ();	// occupy LCD buffer

	// clear LCD data buffer
	memset ( (byte*)LCDDataBuffer, 0x00, sizeof ( LCDDataBuffer ) );
	// clear LCD blink buffer
	memset ( (byte*)LCDDataBlinkBuffer, 0x00, sizeof ( LCDDataBlinkBuffer ) );

	EndDraw ();		// free LCD buffer
}

void ClearLCDIconBufferAll ( void )
{
	BeginDraw ();	// occupy LCD buffer

	// clear LCD icon buffer
	memset ( (byte*)LCDIconBuffer, 0x00, sizeof ( LCDIconBuffer ) );
	// clear LCD icon blink buffer
	memset ( (byte*)LCDIconBlinkBuffer, 0x00, sizeof ( LCDIconBlinkBuffer ) );

	EndDraw ();		// free LCD buffer
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
	nUsingLCDBuffer ++;
}

void EndDraw ( void )
{
	if ( nUsingLCDBuffer > 0 ) 
		nUsingLCDBuffer --;
}

void BeginDrawWait ( void )
{
	while ( nUsingLCDBuffer > 0 )
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
 *			- nIndexX, nIndexY�� LCD buffer������ ��ġ
 *
 *		Input parameters
 *		----------------
 *
 *			int nIndexX, nIndexY
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
 *				|      |		<- nIndexX, nIndexY�� ����Ű�� byte ����
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

void	DrawBitToLCDBuffer ( int nIndexX, int nIndexY,
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
		LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
		LCDDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;

		// draw blink buffer (clear)
		// none blink
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
		break;

	case	DRAW_REVERSE :
		// draw data buffer
		LCDDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		LCDDataBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;

		// draw blink buffer (clear)
		// none blink
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
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
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		}
		else							// reverse ����
		{
			// ���� blink�Ͽ� reverse ���¸� data buffer clear
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
		}

		// draw blink buffer (draw normal)
		// ���ڰ� blink�ϹǷ� blink buffer���� pattern�� draw
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		break;

	case DRAW_BACKGROUND_BLINK :
		if ( ! bIsBufferReverse () )	// normal ����
		{
			// ������ blink�ϹǷ� normal mode�� ����
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		}
		else							// reverse ����
		{
			// ���� blink�Ͽ� reverse ���¸� data buffer fill
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		}

		// draw blink buffer (draw reverse)
		// ������ blink�ϹǷ� blink buffer���� pattern�� reverse
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
		break;

	case	DRAW_NORMAN_BLINK :
		// draw data buffer
		if ( ! bIsBufferReverse () )	// normal ����
		{
			// normal mode�� ����
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		}
		else							// reverse ����
		{
			// ���� blink�Ͽ� reverse ���¸� reverse mode�� ����
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
		}

		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		break;

	case	DRAW_REVERSE_BLINK :
		if ( ! bIsBufferReverse () )	// normal ����
		{
			// reverse mode�� ����
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
		}
		else							// reverse ����
		{
			// ���� blink�Ͽ� reverse ���¸� normal mode�� ����
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		}

		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
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

/*
 *	blink�� timer�� ID
 *		0 : ���� LCD blink mode OFF
 *		1 : LCD blink mode ON
 */
#define	SY_TIMER_NULL	0	// ???
static byte	nBlinkTimerID = SY_TIMER_NULL;


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
	int		nIndex;

	// blink buffer�� data�� �˻��ϱ� ���� loop ( data buffer )
	for ( nIndex = 0; nIndex < sizeof ( LCDDataBuffer ) ; nIndex ++ )
	{
		if ( *( (byte*)LCDDataBlinkBuffer + nIndex ) != 0x00 )
		{
			// LCDDataBlinkBuffer�� data�� 0x00�� �ƴϸ�(blink data ����)
			//  1(TRUE) return
			bReturn = 1;
			break;
		}
	}

	// blink buffer�� data�� �˻��ϱ� ���� loop ( icon buffer )
	if ( bReturn == 0 )
	{
		for ( nIndex = 0; nIndex < sizeof ( LCDIconBuffer ) ; nIndex ++ )
		{
			if ( *( (byte*)LCDIconBlinkBuffer + nIndex ) != 0x00 )
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
 *			LCDBlinkON
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- blink�� timer(os timer) �⵿
 *			- timer task�� ���� 100 msec���� mail�� ������ �����ϰ�
 *				blink time ������ BlinkProcess () ����
 *				BLINK_REVERSE_TIME���� ����
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

void LCDBlinkTimerStart ( void )
{

    // �̹� timer�� ���� �� �̶�� start�� �ʿ� ����
	if ( nBlinkTimerID == SY_TIMER_NULL )
	{
// ???		nBlinkTimerID = kc_tmr_start (
//			SY_TIMER_ID_0,		/* TIMER ID */
//			BLINK_TIME,		/* time out value (100 msec) */
//			SY_TIMER_CONT,		/* continue	timer */
//			SY_LCD_ID );		/* task identification */
	}
 
}

void LCDBlinkON ( void )
{
	LCDBlinkTimerStart ();
}

/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			LCDBlinkOFF
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- blink�� timer(os timer) ����
 *			- ���� blink on�ø� ���� blink buffer clear
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

void LCDBlinkTimerStop ( void )
{
	// timer�� ���� ���� �ƴ϶�� stop�� �ʿ� ����
	if ( nBlinkTimerID != SY_TIMER_NULL )
	{
		// timer stop(kill)
// ???		kc_tmr_end ( nBlinkTimerID, SY_LCD_ID );
//		nBlinkTimerID = SY_TIMER_NULL;
//		DebugMessageA ( "LCDBlinkTimerStop ()\n" );
	}
}

void LCDBlinkOFF ( void )
{
	// timer�� ���� ���� �ƴ϶�� stop�� �ʿ� ����
	if ( nBlinkTimerID != SY_TIMER_NULL )
	{
		// timer stop(kill)
		LCDBlinkTimerStop ();

		// blink buffer clear
		memset ( (byte*)LCDDataBlinkBuffer, 0x00, sizeof ( LCDDataBlinkBuffer ) );
		memset ( (byte*)LCDIconBlinkBuffer, 0x00, sizeof ( LCDIconBlinkBuffer ) );
		bIsBufferReverseFlag = 0;
	}
}


/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			GetBlinkTimerID
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- ���� blink�� timer�� ID�� return
 *			- timer�� ���� �������� �˱� ���Ҷ�
 *			- timer task�κ��� ���� ���� timeout mail�߿���
 *			   blink�� timer�� �����Ҷ�
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

byte GetBlinkTimerID ( void )
{
	return ( nBlinkTimerID );
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
	int	nIndex;

	BeginDraw ();	// occupy LCD buffer

	// LCD data buffer length ��ŭ loop
	for ( nIndex = 0; nIndex < sizeof ( LCDDataBlinkBuffer ) ; nIndex ++ )
	{
		// blink bit �� �ִ��� Ȯ��
		if ( *( (byte*)LCDDataBlinkBuffer + nIndex ) != 0x00 )
		{
			// blink bit position�� data bit�� ����
			*( (byte*)LCDDataBuffer + nIndex ) ^=
				*( (byte*)LCDDataBlinkBuffer + nIndex );
		}
	}
	// LCD Icon buffer length ��ŭ loop
	for ( nIndex = 0; nIndex < sizeof ( LCDIconBlinkBuffer ) ; nIndex ++ )
	{
		// blink bit �� �ִ��� Ȯ��
		if ( *( (byte*)LCDIconBlinkBuffer + nIndex ) != 0x00 )
		{
			// blink bit position�� data bit�� ����
			*( (byte*)LCDIconBuffer + nIndex ) ^=
				*( (byte*)LCDIconBlinkBuffer + nIndex );
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
	static int nCount = 0;
	// ���� LCD data buffer�� ���´� bIsBufferReverseFlag�̰�
	//  �� flag�� �Լ� ���������� ���
	// blink time ������ �ٸ� task�� LCD buffer�� ����� �̶��
	//  blink�ֱ⸦ �����ϰ� ���� �ϱ� ���� data buffer�� �������� �ʴ´�
	static int bIsReverse = 0;

	nCount ++;
	nCount &= 0x07;		// 3bits�� mask�ϸ� 0x00 ~ 0x07�� ���� ��´�

	// normal display time = timeout��(100 msec) * BLINK_REVERSE_TIME
	// reverse display time = blink cycle(800 msec) - normal display time
	// �����	normal display time = 600 msec
	//			reverse display time = 200 msec
	if ( nCount == 0 )
		bIsReverse = 0;		// normal display
	else if ( nCount == BLINK_REVERSE_TIME )
		bIsReverse = 1;		// reverse display

	if ( nUsingLCDBuffer == 0 )
	// LCD bufer�� ������� �ƴ� ����
	{
		// blink time ������ �ٸ� task�� LCD buffer�� ����� �̶��
		//  blink�ֱ⸦ �����ϰ� ���� �ϱ� ���� data buffer�� �������� �ʴ´�
		// LCDDataBlinkBuffer, LCDIconBlinkBuffer�� data�� ������ ���� ����
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



/*************************************************************************
 *
 *
 *		LCD (HW, NEC controler) control routins
 *		
 *			NEC controler mode
 *				LCDCommandDataEnable
 *				LCDCommandDataDisable
 *				LCDDisplayOn
 *				LCDDisplayOff
 *				InitialLCD
 *				SetLCDAddress
 *
 *************************************************************************/


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
 *			NEC controler mode
 *				LCDCommandDataEnable
 *				LCDCommandDataDisable
 *
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- set command,data enable/disable
 *			- NEC controler�� command�� data�� ���� ����
 *			- STB pin�� low�϶� command�� data�� ������ ������ �Ѵ�
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

void	LCDCommandDataEnable ( void )
{
	// STB low
	PORT_LOW ( LCD_STB_PORT, LCD_STB_BIT_MASK );
}

void	LCDCommandDataDisable ( void )
{
	// STB high
	PORT_HIGH ( LCD_STB_PORT, LCD_STB_BIT_MASK );
}


/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			NEC controler mode
 *				LCDDisplayOn
 *				LCDDisplayOff
 *
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- LCD diaplay enable/disable
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
	LCDCommandDataEnable ();
	LCD_DATA_ADDRESS = LCD_COMMAND_DISPLAY_ON;
	LCDCommandDataDisable ();
}

void	LCDDisplayOff ( void )
{
	LCDCommandDataEnable ();
	LCD_DATA_ADDRESS = LCD_COMMAND_DISPLAY_OFF;
	LCDCommandDataDisable ();
}


/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			NEC controler mode
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
 *			- LCD buffer clear
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
	// enable output mode LCD_ON, LCD_STB
	PORT_OUTPUT_MODE ( LCD_ON_PORT, LCD_ON_BIT_MASK );
	PORT_OUTPUT_MODE ( LCD_STB_PORT, LCD_STB_BIT_MASK );

	// LCD not select
	LCDCommandDataDisable ();

	// LCD H/W reset and on
	PORT_LOW ( LCD_ON_PORT, LCD_ON_BIT_MASK );
	// Wait for above 50 micro sec
	clk_busy_wait ( 50 );
	PORT_HIGH ( LCD_ON_PORT, LCD_ON_BIT_MASK );

	// LCD S/W reset
	LCDCommandDataEnable ();
	LCD_DATA_ADDRESS = LCD_COMMAND_RESET;
	LCDCommandDataDisable ();

	// LCD duty set
	LCDCommandDataEnable ();
	LCD_DATA_ADDRESS = LCD_COMMAND_DUTY_51;
	LCDCommandDataDisable ();

	// LCD DAC set
	LCDCommandDataEnable ();
	LCD_DATA_ADDRESS = LCD_COMMAND_DAC_SET;
	LCD_DATA_ADDRESS = LCD_DAC_DEFAULT;
	LCDCommandDataDisable ();

	ClearLCDBufferAll ();
	DisplayLCDBufferAll ();

	// LCD display on
	LCDDisplayOn ();
	// clear LCD data, icon
}


/*===========================================================================*
 *
 *		Function name
 *		-------------
 *
 *			SetLCDAddress
 *			SetLCDIconAddress
 *			SetLCDIconBlinkAddress
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- data�� ����� LCD�� address�� set
 *			- NEC controler type
 *
 *		Input parameters
 *		----------------
 *
 *			int nAddressX
 *				- X position(byte ����) select
 *
 *			int nAddressY
 *				- Y position(bit ����) select
 *
 *		Return
 *		------
 *
 *			NONE
 *
 *
 *===========================================================================*/

void	SetLCDAddress ( int nAddressX, int nAddressY )
{
	
	LCDCommandDataEnable ();

	// send address command
	LCD_DATA_ADDRESS = LCD_COMMAND_ADDRESS_SET;
	// send X address
	LCD_DATA_ADDRESS = nAddressX;
	// send Y address
	LCD_DATA_ADDRESS = nAddressY;

	LCDCommandDataDisable ();
}

void	SetLCDIconAddress ( int nSeg )
{
	
	LCDCommandDataEnable ();

	// send address command
	LCD_DATA_ADDRESS = LCD_COMMAND_ICON_ADDRESS_SET;
	// send X address
	LCD_DATA_ADDRESS = nSeg / 8;
	// send Y address
	LCD_DATA_ADDRESS = 0;

	LCDCommandDataDisable ();
}

void	SetLCDIconBlinkAddress ( int nSeg )
{
	
	LCDCommandDataEnable ();

	// send address command
	LCD_DATA_ADDRESS = LCD_COMMAND_ICON_BLINK_ADDRESS_SET;
	// send X address
	LCD_DATA_ADDRESS = nSeg / 8;
	// send Y address
	LCD_DATA_ADDRESS = 0;

	LCDCommandDataDisable ();
}
#endif