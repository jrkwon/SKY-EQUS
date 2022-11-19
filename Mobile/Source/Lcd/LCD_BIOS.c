
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
 *			void DrawBitToLCDBuffer ( int nIndexX, int nIndexY,
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
 *			void ClearLCDIconBufferAll ( void );
 *				=> LCD ICON buffer의 내용을 모두 clear
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
 *		LCD blinking control routins
 *
 *			void LCDBlinkON ( void );
 *			void LCDBlinkOFF ( void );
 *				=> LCD의 blink mode를 지원하기 위해 timer를 가동
 *					- LCD init routine에 LCDBlinkON() 추가 하였음
 *					- blink mode 미사용시 LCDBlinkOFF () 사용 권장
 *
 *			byte GetBlinkTimerID ( void );
 *				=> 현재 가동중인 LCD blink용 timer의 ID를 얻음
 *					- timer가 disable이라면 이 값은 0
 *					- tim_cmd() function에 추가 하였음
 *
 *			void BlinkProcess ( void );
 *				=> LCD blink routine
 *					- tim_cmd() function에 추가 하였음
 *					- blink timer가 on되면 blink routine를 실행
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
 *				added icon용 functions
 *
 *			Rev	0.00.003	98.04.13 11:30	chos
 *				added SDP700용 functions
 *
 *			Rev	0.00.002	98.03.18 15:00	chos
 *				added NEC LCD control용 functions
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
 *	LCD로 출력할 buffer 선언
 *		LCD data buffer
 *		bit 가 1 이면 LCD bit on (black)
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
 *		bit 가 1 이면 LCD bit blink
 */
byte	LCDDataBlinkBuffer
	[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ];

byte	LCDIconBlinkBuffer[ LCD_TOTAL_ADDRESS_X ];

/*
 *		LCD buffer를 사용하고 있음을 알리는 flag
 *		이 flag를 이용하여 다른 task와 동시에 LCD buffer를
 *		access(write)하는것을 방지함
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

/*
 *	LCD buffer(LCDDataBuffer) 의 data를 LCD로 출력
 *	buffer 전체의 data를 출력
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
	// LCD buffer의 값을 LCD로 전송하기 위해 loop
	for ( nIndex = 0; nIndex < sizeof ( LCDDataBuffer ) ; nIndex ++ )
	{
		// LCD buffer의 값을 LCD로 전송
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
	// LCD buffer의 값을 LCD로 전송하기 위해 loop
	for ( nIndex = 0; nIndex < sizeof ( LCDIconBuffer ) ; nIndex ++ )
	{
		// LCD buffer의 값을 LCD로 전송
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
 *			- LCD buffer에 bits를 저장 (text font, bitmap을 그리기 위해 사용)
 *			- nIndexX, nIndexY는 LCD buffer에서의 위치
 *
 *		Input parameters
 *		----------------
 *
 *			int nIndexX, nIndexY
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
 *				|      |		<- nIndexX, nIndexY가 가리키는 byte 영역
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

void	DrawBitToLCDBuffer ( int nIndexX, int nIndexY,
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

	// blink 계열의 mode들은 현재 blink 상태(주기적으로 buffer의 blink
	//  영역을 반전 하므로 2가지 상태가 있다)에 따라 pattern을 draw하는
	//  방법이 2가지가 있다
	// blink buffer에도 각 draw mode에 따라 draw방법이 다르다
	case DRAW_FOREGROUND_BLINK :
		// draw data buffer
		if ( ! bIsBufferReverse () )	// normal 상태
		{
			// 글자가 blink하므로 normal mode와 같다
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 data buffer clear
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
		}

		// draw blink buffer (draw normal)
		// 글자가 blink하므로 blink buffer에는 pattern을 draw
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		break;

	case DRAW_BACKGROUND_BLINK :
		if ( ! bIsBufferReverse () )	// normal 상태
		{
			// 바탕이 blink하므로 normal mode와 같다
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 data buffer fill
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		}

		// draw blink buffer (draw reverse)
		// 바탕이 blink하므로 blink buffer에는 pattern을 reverse
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
		break;

	case	DRAW_NORMAN_BLINK :
		// draw data buffer
		if ( ! bIsBufferReverse () )	// normal 상태
		{
			// normal mode와 같다
			LCDDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 reverse mode와 같다
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
		}

		LCDDataBlinkBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		break;

	case	DRAW_REVERSE_BLINK :
		if ( ! bIsBufferReverse () )	// normal 상태
		{
			// reverse mode와 같다
			LCDDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
			LCDDataBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 normal mode와 같다
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
 *	현재의 LCD data buffer blink 상태를 나타낸다 
 *		0 : normal 상태
 *		1 : 현재 반전(reverse)된 상태
 */
static int	bIsBufferReverseFlag = 0;

/*
 *	blink용 timer의 ID
 *		0 : 현재 LCD blink mode OFF
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
	int		nIndex;

	// blink buffer의 data를 검색하기 위한 loop ( data buffer )
	for ( nIndex = 0; nIndex < sizeof ( LCDDataBuffer ) ; nIndex ++ )
	{
		if ( *( (byte*)LCDDataBlinkBuffer + nIndex ) != 0x00 )
		{
			// LCDDataBlinkBuffer의 data가 0x00이 아니면(blink data 존재)
			//  1(TRUE) return
			bReturn = 1;
			break;
		}
	}

	// blink buffer의 data를 검색하기 위한 loop ( icon buffer )
	if ( bReturn == 0 )
	{
		for ( nIndex = 0; nIndex < sizeof ( LCDIconBuffer ) ; nIndex ++ )
		{
			if ( *( (byte*)LCDIconBlinkBuffer + nIndex ) != 0x00 )
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
 *			LCDBlinkON
 *
 *===========================================================================*
 *
 *		Function description
 *		--------------------
 *
 *			- blink용 timer(os timer) 기동
 *			- timer task로 부터 100 msec마다 mail이 오도록 설정하고
 *				blink time 설정은 BlinkProcess () 에서
 *				BLINK_REVERSE_TIME으로 결정
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

    // 이미 timer가 실행 중 이라면 start할 필요 없다
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
 *			- blink용 timer(os timer) 해제
 *			- 다음 blink on시를 위해 blink buffer clear
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
	// timer가 실행 중이 아니라면 stop할 필요 없다
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
	// timer가 실행 중이 아니라면 stop할 필요 없다
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
 *			- 현재 blink용 timer의 ID를 return
 *			- timer가 실행 중인지를 알기 원할때
 *			- timer task로부터 오는 여러 timeout mail중에서
 *			   blink용 timer를 구별할때
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
	int	nIndex;

	BeginDraw ();	// occupy LCD buffer

	// LCD data buffer length 만큼 loop
	for ( nIndex = 0; nIndex < sizeof ( LCDDataBlinkBuffer ) ; nIndex ++ )
	{
		// blink bit 가 있는지 확인
		if ( *( (byte*)LCDDataBlinkBuffer + nIndex ) != 0x00 )
		{
			// blink bit position의 data bit를 반전
			*( (byte*)LCDDataBuffer + nIndex ) ^=
				*( (byte*)LCDDataBlinkBuffer + nIndex );
		}
	}
	// LCD Icon buffer length 만큼 loop
	for ( nIndex = 0; nIndex < sizeof ( LCDIconBlinkBuffer ) ; nIndex ++ )
	{
		// blink bit 가 있는지 확인
		if ( *( (byte*)LCDIconBlinkBuffer + nIndex ) != 0x00 )
		{
			// blink bit position의 data bit를 반전
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
	static int nCount = 0;
	// 실제 LCD data buffer의 상태는 bIsBufferReverseFlag이고
	//  이 flag는 함수 내부적으로 사용
	// blink time 에서도 다른 task가 LCD buffer를 사용중 이라면
	//  blink주기를 일정하게 유지 하기 위해 data buffer를 반전하지 않는다
	static int bIsReverse = 0;

	nCount ++;
	nCount &= 0x07;		// 3bits를 mask하면 0x00 ~ 0x07의 값을 얻는다

	// normal display time = timeout값(100 msec) * BLINK_REVERSE_TIME
	// reverse display time = blink cycle(800 msec) - normal display time
	// 현재는	normal display time = 600 msec
	//			reverse display time = 200 msec
	if ( nCount == 0 )
		bIsReverse = 0;		// normal display
	else if ( nCount == BLINK_REVERSE_TIME )
		bIsReverse = 1;		// reverse display

	if ( nUsingLCDBuffer == 0 )
	// LCD bufer가 사용중이 아닐 때만
	{
		// blink time 에서도 다른 task가 LCD buffer를 사용중 이라면
		//  blink주기를 일정하게 유지 하기 위해 data buffer를 반전하지 않는다
		// LCDDataBlinkBuffer, LCDIconBlinkBuffer에 data가 존재할 때만 반전
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
 *			- NEC controler은 command와 data를 구분 안함
 *			- STB pin이 low일때 command와 data의 조합을 보내야 한다
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
 *			- H/W 선택 사양 set
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
 *			- data를 출력할 LCD의 address를 set
 *			- NEC controler type
 *
 *		Input parameters
 *		----------------
 *
 *			int nAddressX
 *				- X position(byte 단위) select
 *
 *			int nAddressY
 *				- Y position(bit 단위) select
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