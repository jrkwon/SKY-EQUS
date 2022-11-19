#if MMI_TYPE == SKY_UI
#include "customer.h"
#include "target.h"             /* Target specific definitions          */
#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "err.h"
//#include "biog.h"
#include "bio.h"
#include "clk.h"

#include "..\modeldep\biogb0.h"

#include "tramp.h"              /* Trampoline typedefs and prototypes   */
#include "task.h"             /* Target specific definitions          */

#include "snd.h"
#include "sdb.h"
#include "sndi.h"               /* Sound internal typedefs                */
#include "sndihw.h"               /* Sound internal typedefs                */
#include "yamaha.h"             /* YAMAHA FM MUSIC synthesizer definition     */
#include "MelodyDb.h"             /* Music data base definition     */
#include "audtest.h"             /* Music data base definition     */
#include "..\MMI\sk\MMedia\MMedia.h"    /* YAMAHA FM MUSICesizer definition     */

#else

#include "..\Qualcomm\customer.h"
#include "..\Qualcomm\target.h"             /* Target specific definitions          */
#include "..\Qualcomm\comdef.h"             /* Definitions for byte, word, etc.     */
#include "..\Qualcomm\err.h"
#include "..\Qualcomm\biog.h"
#include "..\Qualcomm\bio.h"
#include "..\Qualcomm\clk.h"
#if MODEL_ID == MODEL_A10
#include "..\Qualcomm\gpio_int.h"              /* Trampoline typedefs and prototypes   */
#else
#include "..\Qualcomm\tramp.h"              /* Trampoline typedefs and prototypes   */
#endif
#include "..\Qualcomm\task.h"             /* Target specific definitions          */

#include "..\Qualcomm\snd.h"
#include "..\Qualcomm\sdb.h"
#include "..\Qualcomm\sndi.h"               /* Sound internal typedefs                */
#include "..\Qualcomm\sndihw.h"               /* Sound internal typedefs                */
#include "yamaha.h"             /* YAMAHA FM MUSIC synthesizer definition     */
#include "MelodyDb.h"             /* Music data base definition     */
#include "audtest.h"             /* Music data base definition     */
#include "..\MMI\sk\MMedia\MMedia.h"    /* YAMAHA FM MUSICesizer definition     */

#endif

#include <string.h>		// woodstok 00.4.27

const byte Tbl_RingerToNote_h[] = 
{
///Add New note in 2000.3.16
	0x44,	// NT_C3P,
	0x48,	// NT_D3	,
	0x4c,	// NT_D3P,
	0x54,	// NT_E3	,
	0x58,	// NT_F3	,
	0x5c,	// NT_F3P,

	0x64,	// NT_G3,
	0x68,	// NT_G3P,
	0x6c,	// NT_A3,
	0x74,	// NT_A3P,
	0x78,	// NT_B3,
	0x7c,	// NT_C4,
	0x84,	// NT_C4P,
	0x88,	// NT_D4	,
	0x8c,	// NT_D4P,
	0x94,	// NT_E4	,
	0x98,	// NT_F4	,
	0x9c,	// NT_F4P,
	0xa4,	// NT_G4	,
	0xa8,	// NT_G4P,
	0xac,	// NT_A4	,
	0xb4,	// NT_A4P,
	0xb8,	// NT_B4	,
	0xbc,	// NT_C5	,
	0xc4,	// NT_C5P,
	0xc8,	// NT_D5	,
	0xcc,	// NT_D5P,
	0xd4,	// NT_E5	,
	0xd8,	// NT_F5	,
	0xdc,	// NT_F5P,
	0xe4,	// NT_G5	,
	0xe8,	// NT_G5P,
	0xec,	// NT_A5	,
	0xf4,	// NT_A5P,
	0xf8,	// NT_B5	,
	0xfc,	// NT_C6	,
	0x85,	// NT_C6P,      
	0x89,	// NT_D6	
	0x8d,	// NT_D6P,
	0x95,	// NT_E6
	0x99,	// NT_F6
	0x9d,	// NT_F6P,
	0xa5,	// NT_G6
///Add New note in 2000.3.16
	0xa9,	// NT_G6P,
	0xad,	// NT_A6	,
	0xb5,	// NT_A6P,
	0xb9,	// NT_B6	,
	0xbd,	// NT_C7	,
	0xc5,	// NT_C7P, 
	0xc9,	// NT_D7
	0xcd,	// NT_D7P
	0xd5,	// NT_E7
	0xd9,	// NT_F7
	0xdd,	// NT_F7P
	0xe5,	// NT_G7
	0xe9,	// NT_G7P,
	0xed,	// NT_A7	,
	0xf5,	// NT_A7P,
	0xf9,	// NT_B7	,
	0xfd,	// NT_C8	,

};

const byte Tbl_RingerToNote_l[] = 
{
	0x0,	//NT_NULL,
	0x7f,	//NT_1,
	0x7f,	//NT_2D,
	0x7e,	//NT_2,
	0x74,	//NT_4D,
	0x69,	//NT_4,
	0x47,	//NT_8D,
	0x3e,	//NT_8	,
	0x35,	//NT_16D,
	0x23,	//NT_16	,
	0x11	//NT_32
};

const dword release_time_tbl[16] = 
{
	73234,	//infinite
	73234,	//73234.0
	36617,	//36617.0
	18309,	//18308.50
	9155,	//9154.25
	4578,	//4577.12
	2289,	//2288.56
	1145,	//1144.25
	573,	//572.14
	287,	//286.07
	144,	//143.04
	72,		//71.52
	36,		//35.76
	18,		//17.88
	9,		//8.94
	5		//4.47
};

const byte interval_time_tbl[16] = 
{
	0,
	2,
	3,
	4,
	6,
	8,
	9,
	12,
	18,
	24,
	48,
	0,
	16,
	24,
	36,
	48
};

const byte note_length_tbl[8][2] = 
{
	{1,15},
	{2,23},
	{3,29},
	{5,32},
	{7,35},
	{8,41},
	{11,47},
	{17,48}
};

static word MUSIC_data[MUSIC_REG_NUM] = {0,};

/* music Timer definition
*/
rex_timer_type snd_music_timer;    /* General music Timer               */
rex_timer_type snd_vRef_PDdelay_timer;    /* music Power down delayTimer               */

/*===========================================================================
FUNCTION SEND_CODEC_DATA

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
static dword bit_mask;

void send_addr(byte reg_addr){
//  dword bit_mask;

//  MSG_HIGH("reg:%x data:%x go_data:%x",reg_addr, data, go_data);


  for( bit_mask = MUSIC_ADDR_MSB; bit_mask != 0; bit_mask >>= 1 ) {
    if ( (reg_addr & bit_mask) != 0 ) {
      BIO_OUT( BIO_MUSIC_DATA_REG, BIO_MUSIC_DATA_M, BIO_MUSIC_DATA_HI_V );
    } else {
      BIO_OUT( BIO_MUSIC_DATA_REG, BIO_MUSIC_DATA_M, BIO_MUSIC_DATA_LO_V );
    }

    BIO_OUT( BIO_MUSIC_CLK_REG, BIO_MUSIC_CLK_M, BIO_MUSIC_CLK_HI_V );
	clk_busy_wait(1);
    BIO_OUT( BIO_MUSIC_CLK_REG, BIO_MUSIC_CLK_M, BIO_MUSIC_CLK_LO_V );
  } /* for */
}


void send_data(word data){
//  dword bit_mask;

//  MSG_HIGH("reg:%x data:%x go_data:%x",reg_addr, data, go_data);

  for( bit_mask = MUSIC_DATA_MSB; bit_mask != 0; bit_mask >>= 1 ) {

    if ( (data & bit_mask) != 0 ) {
	  BIO_OUT( BIO_MUSIC_DATA_REG, BIO_MUSIC_DATA_M, BIO_MUSIC_DATA_HI_V );
    } else {
      BIO_OUT( BIO_MUSIC_DATA_REG, BIO_MUSIC_DATA_M, BIO_MUSIC_DATA_LO_V );
    }
    BIO_OUT( BIO_MUSIC_CLK_REG, BIO_MUSIC_CLK_M, BIO_MUSIC_CLK_HI_V );
	clk_busy_wait(1);
    BIO_OUT( BIO_MUSIC_CLK_REG, BIO_MUSIC_CLK_M, BIO_MUSIC_CLK_LO_V );
  } /* for */
}
/*===========================================================================
FUNCTION FM_CMD

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
/* You must use for mask only defined value in yamaha.h, or ..  */

//static word isave;

void FM_cmd(byte reg, word mask, word val)
{
	word isave;

	MUSIC_data[reg] = (MUSIC_data[reg] & ~mask) | (val & mask);

	INTLOCK_SAV(isave);
	BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_LO_V );
	BIO_OUT( BIO_MUSIC_CLK_REG, BIO_MUSIC_CLK_M , BIO_MUSIC_CLK_LO_V );
	send_addr( reg);
	send_data( MUSIC_data[reg]);

	BIO_OUT( BIO_MUSIC_DATA_REG, BIO_MUSIC_DATA_M , BIO_MUSIC_DATA_LO_V );
	BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_HI_V );

	clk_busy_wait(1);

  INTFREE_SAV(isave);
}

/*void FM_note_cmd_start(byte reg)
{
	word isave;

	INTLOCK_SAV(isave);
	BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_LO_V );
	BIO_OUT( BIO_MUSIC_CLK_REG, BIO_MUSIC_CLK_M , BIO_MUSIC_CLK_LO_V );
	send_addr( reg);
	INTFREE_SAV(isave);
}*/

/*void FM_note_cmd(byte reg, word mask, word val)
{
	word isave;

	MUSIC_data[reg] = (MUSIC_data[reg] & ~mask) | (val & mask);

	INTLOCK_SAV(isave);
  BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_LO_V );
  BIO_OUT( BIO_MUSIC_CLK_REG, BIO_MUSIC_CLK_M , BIO_MUSIC_CLK_LO_V );

	send_addr( reg);
	send_data( MUSIC_data[reg]);

  BIO_OUT( BIO_MUSIC_DATA_REG, BIO_MUSIC_DATA_M , BIO_MUSIC_DATA_LO_V );

  BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_HI_V );
	clk_busy_wait(1);

  INTFREE_SAV(isave);
*/
/*	word isave;
	MUSIC_data[reg] = (MUSIC_data[reg] & ~mask) | (val & mask);

	INTLOCK_SAV(isave);
	send_data( MUSIC_data[reg]);
	  BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_LO_V );
		clk_busy_wait(1);
	  BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_HI_V );
	   INTFREE_SAV(isave);
}*/

/*void FM_note_cmd_stop(void)
{
	word isave;

	INTLOCK_SAV(isave);
  BIO_OUT( BIO_MUSIC_DATA_REG, BIO_MUSIC_DATA_M , BIO_MUSIC_DATA_LO_V );

//  BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_LO_V );
//	clk_busy_wait(1);
//  BIO_OUT( BIO_MUSIC_SYNC_REG, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_HI_V );
  INTFREE_SAV(isave);
}*/

/*===========================================================================
FUNCTION MUSIC_bio_init

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_bio_init(void)
{
	//  Serial I/F initialization
	BIO_TRISTATE(BIO_MUSIC_CLK_REG,BIO_MUSIC_CLK_M,BIO_OUTPUT_ENA_V);
	BIO_TRISTATE(BIO_MUSIC_DATA_REG,BIO_MUSIC_DATA_M,BIO_OUTPUT_ENA_V);
	BIO_TRISTATE(BIO_AUDIO_REG_REG,BIO_AUDIO_REG_M,BIO_OUTPUT_ENA_V);
	BIO_TRISTATE(BIO_MUSIC_SYNC_REG,BIO_MUSIC_SYNC_M,BIO_OUTPUT_ENA_V);

	BIO_TRISTATE(BIO_MUSIC_FIFO_EMPTY_IRQ_REG,BIO_MUSIC_FIFO_EMPTY_IRQ_M,BIO_OUTPUT_DIS_V);
}


void MUSIC_audio_en(boolean en)
{
	if(en)
	{
		BIO_OUT( BIO_AUDIO_REG_REG, BIO_AUDIO_REG_M , BIO_AUDIO_REG_ENA_V );
	}
	else
	{
		BIO_OUT( BIO_AUDIO_REG_REG, BIO_AUDIO_REG_M , BIO_AUDIO_REG_DIS_V );
	}
}

/*===========================================================================
FUNCTION MUSIC_init

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_hw_init(void)
{

	rex_def_timer( &snd_music_timer, &snd_tcb, SND_MUSIC_TIMER_SIG );
	rex_def_timer( &snd_vRef_PDdelay_timer, &snd_tcb, SND_VREF_TIMER_SIG );

	// Set master clock 
	FM_cmd(MUSIC_CLOCK_I_INDEX
			, MUSIC_CLOCK_I_SELECT_MASK 
			, CLOCK_I_19_680 );

	// reset S/W & resetting all register
	FM_cmd(MUSIC_FM_CONTROL_INDEX
			, FM_CONTOL_START_STOP_MASK | FM_CONTOL_RESET_MASK
			, FM_CONTOL_STOP | FM_CONTOL_RESET);

	// wait during reset procedure : min 6 usec
	clk_busy_wait(20);

	FM_cmd(MUSIC_FM_CONTROL_INDEX
			, FM_CONTOL_START_STOP_MASK | FM_CONTOL_RESET_MASK
			, FM_CONTOL_STOP );

	
	FM_cmd(MUSIC_POWER_MANAGE_INDEX, VREF_POWER_REDUCE_MASK, VREF_POWER_REDUCE_ON);

	FM_cmd(MUSIC_POWER_MANAGE_INDEX, VOLUME_AMP_REDUCE_MASK, VOLUME_AMP_REDUCE_ON);

	FM_cmd(MUSIC_POWER_MANAGE_INDEX, SPKOUT_REDUCE_MASK, SPKOUT_REDUCE_ON);

	FM_cmd(MUSIC_POWER_MANAGE_INDEX, EAR_DAC_REDUCE_MASK, EAR_DAC_REDUCE_ON);

	///Audio enable register Off (GPIO24)   2000.1.12
	MUSIC_audio_en(FALSE);
}

/*===========================================================================
FUNCTION MUSIC_power_on

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
byte MUSIC_power_status = 0;
byte MUSIC_power_real_status = 0;
byte MUSIC_power_sleep_status = 0;
byte MUSIC_power_mask = 0;
boolean MUSIC_power_cur_status[4] = {0,};
uint16 Cur_FM_vol, Cur_SPK_vol, Cur_HPOUT_vol;
//boolean IsVrefPD_delay = FALSE;

void MUSIC_power_on(
	mus_acquire_type  key,         /* What entity wants to acquire the Vocoder */
	boolean			  Dpchan_en,    /* DPD dose not change : data R/W enable
							        & stored FIFO data is available        */
	boolean			  Apchan_en    /* APD dose not change : Amp, Volume   */
)
{


//	MSG_ERROR("Func MUSIC_power_on = %d",key,0,0);
	/* Check for a legal user key */
	if(key  == MUS_NONE	)
	{
		return;
	}
	else if (key < MUS_MELODY || key > MUS_SPK) {
		ERR_FATAL("Invalid key %x", key, 0, 0);
	}

	///	Already power on in required key
	if( (MUSIC_power_status & key) != 0 &&
		(MUSIC_power_sleep_status & key) != 0 )
	{
//		MSG_ERROR("Cancel MUSIC_power_on",0,0,0);
		return;
	}

	///Power mask bit update
	if(key == MUS_MELODY)
		MUSIC_power_mask |= MELODY_POWER_MASK;
	else if(key == MUS_EAR_MELODY)
		MUSIC_power_mask |= EAR_MELODY_POWER_MASK;
	else if( key == MUS_SPK)
		MUSIC_power_mask |= SPK_POWER_MASK;

	///Digital Power management
	if(Dpchan_en && (MUSIC_power_mask & DP_ON))
	{
		FM_cmd(MUSIC_CLOCK_I_INDEX
			, MUSIC_CLOCK_I_SELECT_MASK 
			, CLOCK_I_19_680 );						//// Power On

		FM_cmd(MUSIC_POWER_MANAGE_INDEX, ENTIRE_POWER_REDUCE_MASK, ENTIRE_POWER_REDUCE_OFF); //// reset DP

		MUSIC_power_sleep_status |= key;  //Digital power
//		MSG_ERROR("DP on",0,0,0);
	}


	///Analog Power management
	if(Apchan_en)
	{

		if(MUSIC_power_mask & ANALOG_POWER_MASK)
		{
			///Audio enable register On (GPIO24)  2000.1.12
			MUSIC_audio_en(TRUE);
//			MSG_ERROR("MUSIC_audio_en",0,0,0);
		}

		if((MUSIC_power_mask & AP1_ON) && !MUSIC_power_cur_status[0])
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, VREF_POWER_REDUCE_MASK, VREF_POWER_REDUCE_OFF);    //// reset AP1
//			MSG_ERROR("VREF on",0,0,0);

			clk_busy_wait(150000);
			MUSIC_power_cur_status[0] = TRUE;
		}

		if((MUSIC_power_mask & AP2_ON) && !MUSIC_power_cur_status[1])
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, VOLUME_AMP_REDUCE_MASK, VOLUME_AMP_REDUCE_OFF); //// reset AP2
			clk_busy_wait(20);
			MUSIC_power_cur_status[1] = TRUE;
		}

		if((MUSIC_power_mask & AP3_ON) && !MUSIC_power_cur_status[2])
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, SPKOUT_REDUCE_MASK, SPKOUT_REDUCE_OFF); //// reset AP3`
			MUSIC_power_cur_status[2] = TRUE;
			//MSG_ERROR("AMP on",0,0,0);
		}

		if((MUSIC_power_mask & AP4_ON) && !MUSIC_power_cur_status[3])
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, EAR_DAC_REDUCE_MASK, EAR_DAC_REDUCE_OFF); //// reset AP4
//			MSG_ERROR("EAR on",0,0,0);
			MUSIC_power_cur_status[3] = TRUE;
		}

		MUSIC_power_status |= key;      //Analog power
	}

	if(MUSIC_power_status != 0 || MUSIC_power_sleep_status != 0) 
	{
		SND_SLEEP_FORBID();
	    MSG_HIGH("status:0x%x  slstatus:0x%x",MUSIC_power_status,MUSIC_power_sleep_status,0);
	}

	MUSIC_power_real_status = MUSIC_power_status;
/*
	///Digital Power management
	if(MUSIC_power_sleep_status == 0 && Dpchan_en)
	{

		FM_cmd(MUSIC_CLOCK_I_INDEX
			, MUSIC_CLOCK_I_SELECT_MASK 
			, CLOCK_I_19_680 );						//// Power On

		FM_cmd(MUSIC_POWER_MANAGE_INDEX, ENTIRE_POWER_REDUCE_MASK, ENTIRE_POWER_REDUCE_OFF); //// reset DP

		MUSIC_power_sleep_status |= (1 << key);  //Digital power
		MSG_ERROR("DP on",0,0,0);
	}

	///Analog Power management
	if(Apchan_en)
	{

		///Audio enable register On (GPIO24)  2000.1.12
		MUSIC_audio_en(TRUE);
		MSG_ERROR("MUSIC_audio_en",0,0,0);

	//	clk_busy_wait(10000);

		if(MUSIC_power_status == 0)
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, VREF_POWER_REDUCE_MASK, VREF_POWER_REDUCE_OFF);    //// reset AP1
			MSG_ERROR("VREF on",0,0,0);

			clk_busy_wait(150000);
		}

		if((key == MUS_MELODY )	|| (key == MUS_SPK) )
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, VOLUME_AMP_REDUCE_MASK, VOLUME_AMP_REDUCE_OFF); //// reset AP2
			clk_busy_wait(20);

			FM_cmd(MUSIC_POWER_MANAGE_INDEX, SPKOUT_REDUCE_MASK, SPKOUT_REDUCE_OFF); //// reset AP3`
			MSG_ERROR("AMP on",0,0,0);
		}

		if((key == MUS_MELODY )	|| (key == MUS_EAR_MELODY))
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, EAR_DAC_REDUCE_MASK, EAR_DAC_REDUCE_OFF); //// reset AP4
			MSG_ERROR("EAR on",0,0,0);
		}

		MUSIC_power_status |= (1 << key);      //Analog power
	}*/
}

/*===========================================================================
FUNCTION MUSIC_power_off

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_power_off(	
	mus_acquire_type  key,         /* What entity wants to acquire the Vocoder */
	boolean			  Dpchan_en,    /* DPD dose not change : data R/W enable
							        & stored FIFO data is available        */
	boolean			  Apchan_en    /* APD dose not change : Amp, Volume   */
)
{
//	MSG_ERROR("Func MUSIC_power_off = %d",key,0,0);
	/* Check for a legal user key */
	if(key  == MUS_NONE	)
	{
		return;
	}
	else if (key < MUS_MELODY || key > MUS_SPK) {
		ERR_FATAL("Invalid key %x", key, 0, 0);
	}
	

	///	Already power on in required key
	if( (MUSIC_power_status & key) == 0 &&		//Analog Power
		(MUSIC_power_sleep_status & key) == 0)   // Digital power
	{
//		MSG_ERROR("Cancel MUSIC_power_off",0,0,0);
		return;
	}


	///Power mask bit update
	MUSIC_power_mask = 0;

	if(Dpchan_en)
		MUSIC_power_sleep_status &= ~key;

/*	if(MUSIC_power_sleep_status & MUS_MELODY)
		MUSIC_power_mask |= (MELODY_POWER_MASK & DP_ON);
	else if(MUSIC_power_sleep_status & MUS_EAR_MELODY)
		MUSIC_power_mask |= (EAR_MELODY_POWER_MASK & DP_ON);
	else
		MUSIC_power_mask |= (SPK_POWER_MASK & DP_ON);
*/

	if(Apchan_en)
		MUSIC_power_status &= ~key;

	(void) rex_set_timer( &snd_vRef_PDdelay_timer , 3000);
	if(MUSIC_power_status & MUS_MELODY)
		MUSIC_power_mask |= (MELODY_POWER_MASK & ANALOG_POWER_MASK );
	else if(MUSIC_power_status & MUS_EAR_MELODY)
		MUSIC_power_mask |= (EAR_MELODY_POWER_MASK & ANALOG_POWER_MASK );
	else if(MUSIC_power_status & MUS_SPK)
		MUSIC_power_mask |= (SPK_POWER_MASK & ANALOG_POWER_MASK );

	///Digital power management
/*	if(!(MUSIC_power_mask & DP_ON) && Dpchan_en)
	{
		FM_cmd(MUSIC_POWER_MANAGE_INDEX, ENTIRE_POWER_REDUCE_MASK, ENTIRE_POWER_REDUCE_ON);
		MSG_ERROR("DP off",0,0,0);
	}

	///Analog power management
	if(Apchan_en)
	{

		if(!(MUSIC_power_mask & AP1_ON))
		{
//			FM_cmd(MUSIC_POWER_MANAGE_INDEX, VREF_POWER_REDUCE_MASK, VREF_POWER_REDUCE_ON);
			(void) rex_set_timer( &snd_vRef_PDdelay_timer , 3000);
			IsVrefPD_delay = TRUE;
			MSG_ERROR("VREF off",0,0,0);
		}*/

		if(!(MUSIC_power_mask & AP2_ON))
		{
			FM_cmd(MUSIC_FM_VOLUME_INDEX, MUSIC_FM_VOLUME_MASK, 0);
			FM_cmd(MUSIC_SPK_VOLUME_INDEX, MUSIC_SPK_VOLUME_MASK, 0);
//			FM_cmd(MUSIC_POWER_MANAGE_INDEX, VOLUME_AMP_REDUCE_MASK, VOLUME_AMP_REDUCE_ON); 
		}

/*		if(!(MUSIC_power_mask & AP3_ON))
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, SPKOUT_REDUCE_MASK, SPKOUT_REDUCE_ON); 
			MSG_ERROR("AMP off",0,0,0);
		}
	*/		
		if(!(MUSIC_power_mask & AP4_ON))
		{
			FM_cmd(MUSIC_HPOUT_VOLUME_INDEX, MUSIC_HPOUT_VOLUME_MASK, 0);
//			FM_cmd(MUSIC_POWER_MANAGE_INDEX, EAR_DAC_REDUCE_MASK, EAR_DAC_REDUCE_ON); 
//			MSG_ERROR("EAR off",0,0,0);
		}

/*		if(!(MUSIC_power_mask & ANALOG_POWER_MASK))
		{
			MUSIC_audio_en(FALSE);
			MSG_ERROR("Audio off",0,0,0);
		}
	}
*/
	/// 멜로디를 다 연주한 뒤에는 Sleep에 들어간다.
//	if (sndhw_allow_sleeping()) {     /* Check ALL the reasons for sleeping   */
//		SND_SLEEP_ALLOW();
//	}

/*	///Digital power management
	if(Dpchan_en)
	{
		MUSIC_power_sleep_status &= ~(1 << key);

		if( !MUSIC_POWER_MASK(MUSIC_power_sleep_status, MUS_MELODY)
			&&  !MUSIC_POWER_MASK(MUSIC_power_sleep_status, MUS_EAR_MELODY))
	//		(MUSIC_power_status & ((1 << MUS_EAR_MELODY) | (1 << MUS_MELODY))) == 0)
		{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, ENTIRE_POWER_REDUCE_MASK, ENTIRE_POWER_REDUCE_ON);
			MSG_ERROR("DP off",0,0,0);
		}
	}

	///Analog power management
	if(Apchan_en)
	{
		MUSIC_power_status &= ~(1 << key);

		if( !MUSIC_POWER_MASK(MUSIC_power_status, MUS_MELODY))
		{

			if( !MUSIC_POWER_MASK(MUSIC_power_status, MUS_EAR_MELODY)
				&& !MUSIC_POWER_MASK(MUSIC_power_status, MUS_SPK))
	//	if( (MUSIC_power_status & ((1 << MUS_EAR_MELODY) | (1 << MUS_MELODY)| (1 << MUS_SPK ))) == 0)
			{
				FM_cmd(MUSIC_POWER_MANAGE_INDEX, VREF_POWER_REDUCE_MASK, VREF_POWER_REDUCE_ON);
				MSG_ERROR("VREF off",0,0,0);
			}

	//	if( (MUSIC_power_status & ((1 << MUS_SPK )| (1 << MUS_MELODY))) == 0)
			if( !MUSIC_POWER_MASK(MUSIC_power_status, MUS_SPK))
			{
				FM_cmd(MUSIC_POWER_MANAGE_INDEX, VOLUME_AMP_REDUCE_MASK, VOLUME_AMP_REDUCE_ON); 

				FM_cmd(MUSIC_POWER_MANAGE_INDEX, SPKOUT_REDUCE_MASK, SPKOUT_REDUCE_ON); 
				MSG_ERROR("AMP off",0,0,0);
			}

			if( !MUSIC_POWER_MASK(MUSIC_power_status, MUS_EAR_MELODY))
		//	if((MUSIC_power_status & ((1 << MUS_EAR_MELODY )| (1 << MUS_MELODY))) == 0)
			{
				FM_cmd(MUSIC_POWER_MANAGE_INDEX, EAR_DAC_REDUCE_MASK, EAR_DAC_REDUCE_ON); 
				MSG_ERROR("EAR off",0,0,0);
			}
		}

		///Audio enable register Off (GPIO24) 2000.1.12
		if(MUSIC_power_status == 0 )
		{
			MUSIC_audio_en(FALSE);
			MSG_ERROR("Audio off",0,0,0);
		}
	}*/
}

/*===========================================================================
FUNCTION MUSIC_power_real_off

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_power_real_off(void)
{

      (void) rex_clr_timer( &snd_vRef_PDdelay_timer );
	///Power mask bit update
	MUSIC_power_mask = 0;

	if(MUSIC_power_sleep_status & MUS_MELODY)
		MUSIC_power_mask |= (MELODY_POWER_MASK & DP_ON);
	else if(MUSIC_power_sleep_status & MUS_EAR_MELODY)
		MUSIC_power_mask |= (EAR_MELODY_POWER_MASK & DP_ON);
	else if(MUSIC_power_status & MUS_SPK)
		MUSIC_power_mask |= (SPK_POWER_MASK & DP_ON);


	if(MUSIC_power_status & MUS_MELODY)
		MUSIC_power_mask |= (MELODY_POWER_MASK & ANALOG_POWER_MASK );
	else if(MUSIC_power_status & MUS_EAR_MELODY)
		MUSIC_power_mask |= (EAR_MELODY_POWER_MASK & ANALOG_POWER_MASK );
	else if(MUSIC_power_status & MUS_SPK)
		MUSIC_power_mask |= (SPK_POWER_MASK & ANALOG_POWER_MASK );

	if(MUSIC_power_mask == 0x1F)
		return;

	///Digital power management
	if(!(MUSIC_power_mask & DP_ON))
	{
		FM_cmd(MUSIC_POWER_MANAGE_INDEX, ENTIRE_POWER_REDUCE_MASK, ENTIRE_POWER_REDUCE_ON);
//		MSG_ERROR("DP off",0,0,0);
	}

	///Analog power management
	if(!(MUSIC_power_mask & AP1_ON) && MUSIC_power_cur_status[0])
	{
			FM_cmd(MUSIC_POWER_MANAGE_INDEX, VREF_POWER_REDUCE_MASK, VREF_POWER_REDUCE_ON);
//		(void) rex_set_timer( &snd_vRef_PDdelay_timer , 3000);
//		IsVrefPD_delay = TRUE;
			MUSIC_power_cur_status[0] = FALSE;
//		MSG_ERROR("VREF off",0,0,0);
	}

	if(!(MUSIC_power_mask & AP2_ON)&& MUSIC_power_cur_status[1])
	{
		FM_cmd(MUSIC_POWER_MANAGE_INDEX, VOLUME_AMP_REDUCE_MASK, VOLUME_AMP_REDUCE_ON); 
		MUSIC_power_cur_status[1] = FALSE;
	}

	if(!(MUSIC_power_mask & AP3_ON)&& MUSIC_power_cur_status[2])
	{
		FM_cmd(MUSIC_POWER_MANAGE_INDEX, SPKOUT_REDUCE_MASK, SPKOUT_REDUCE_ON); 
		MUSIC_power_cur_status[2] = FALSE;
//		MSG_ERROR("AMP off",0,0,0);
	}
		
	if(!(MUSIC_power_mask & AP4_ON)&& MUSIC_power_cur_status[3])
	{
		FM_cmd(MUSIC_POWER_MANAGE_INDEX, EAR_DAC_REDUCE_MASK, EAR_DAC_REDUCE_ON); 
		MUSIC_power_cur_status[3] = FALSE;
//		MSG_ERROR("EAR off",0,0,0);
	}

	if(!(MUSIC_power_mask & ANALOG_POWER_MASK))
	{
		MUSIC_audio_en(FALSE);
//		MSG_ERROR("Audio off",0,0,0);
	}

	MUSIC_power_real_status = MUSIC_power_status;

	/// 멜로디를 다 연주한 뒤에는 Sleep에 들어간다.
	if (sndhw_allow_sleeping()) {     /* Check ALL the reasons for sleeping   */
		SND_SLEEP_ALLOW();
  MSG_HIGH("snd_sleep_allow", 0, 0,0);
	}

}

/*===========================================================================
FUNCTION MUSIC_set_volume
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_set_volume(void)	
{
	byte MUSIC_vol_mask = 0;

	///Mask bit update
	if(MUSIC_power_status & MUS_MELODY)
	{
		MUSIC_vol_mask |= MELODY_VOL_MASK;
	}

	if(MUSIC_power_status & MUS_EAR_MELODY)
	{
		MUSIC_vol_mask |= EAR_MELODY_VOL_MASK;
	}

	if(MUSIC_power_status & SPK_VOL_MASK)
	{
		MUSIC_vol_mask |= MELODY_VOL_MASK;
	}


	/// Set FM volume
	if(MUSIC_vol_mask & MUS_FM_VOL)
	{
		FM_cmd(MUSIC_FM_VOLUME_INDEX, MUSIC_FM_VOLUME_MASK, Cur_FM_vol);
	}
//	else
//	{
//		FM_cmd(MUSIC_FM_VOLUME_INDEX, MUSIC_FM_VOLUME_MASK, 0);
//	}
	
	/// Set Spk volume
	if(MUSIC_vol_mask & MUS_SPK_VOL)
	{
		FM_cmd(MUSIC_SPK_VOLUME_INDEX, MUSIC_SPK_VOLUME_MASK, Cur_SPK_vol);
	}
//	else
//	{
//		FM_cmd(MUSIC_SPK_VOLUME_INDEX, MUSIC_SPK_VOLUME_MASK, 0);
//	}
	
	/// Set HPOUT volume
	if(MUSIC_vol_mask & MUS_HPOUT_VOL)
	{
		FM_cmd(MUSIC_HPOUT_VOLUME_INDEX, MUSIC_HPOUT_VOLUME_MASK, Cur_HPOUT_vol);
	}
//	else
//	{
//		FM_cmd(MUSIC_HPOUT_VOLUME_INDEX, MUSIC_HPOUT_VOLUME_MASK, 0);
//	}

}


const snd_sound_type	*Cur_TimbreTbl;       /* Pointer to array of tones to play    */
//static word *Cur_TimbreTbl;
static boolean Melody_repeat_ena;

static word Total_number_Note;    /// the number of Notes to play.  For only SND_COMPACT_SOUND
static word WriteToFIFO_NoteData_p;  /// current Data pointer to Write FIFO
//static word UseNoteDataFromFIFO_p;
static byte refill_alert_num ;       /// Notes played to refill FIFO
static word Melody_repeat_num ;       /// number of Melody Repeating

static word Opt_tempo;

boolean MUSIC_sound_stop_en = FALSE;

//// repeat from SND_LABEL to SND_BACK_TO_LABEL. For only SND_FLEXIBLE_SOUND
static byte labal_stack_p;				// SND_LABEL point
static word label_location[10];			// interval between LABEL

//// Calcurated release time(msec)
static dword release_time[8];
static byte assign_table[4];

//// Melody File play
static dword Cur_File_size;
static word *Cur_File_bufp;
static byte *b_Cur_File_bufp;
static boolean Is_FilePlaying = FALSE;
static int nNext_data_load;
static boolean ReadNextMelodyReadOK;

extern boolean Music_active;
/*===========================================================================
FUNCTION MUSIC_CompressTimbre
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

LOCAL dword MUSIC_CompressTimbre(basic_Timbre_data td  )
{

	dword timbre_data ;

	timbre_data = (MUSIC_TIMBRE_MULTIPLE_MASK	& (  (dword)td.multi  << MUSIC_TIMBRE_MULTIPLE_SHIFT		))
				| (MUSIC_TIMBRE_VIBRATO_MASK	& (  (dword)td.vib  << MUSIC_TIMBRE_VIBRATO_SHIFT		))
				| (MUSIC_TIMBRE_ENV_TYPE_MASK	& (  (dword)td.egt  << MUSIC_TIMBRE_ENV_TYPE_SHIFT		))	
				| (MUSIC_TIMBRE_SUSTAIN_ONOFF_MASK &((dword)td.sus <<MUSIC_TIMBRE_SUSTAIN_ONOFF_SHIFT ))
				| (MUSIC_TIMBRE_RELEASE_L_MASK	& (  (dword)td.rr  << MUSIC_TIMBRE_RELEASE_L_SHIFT	))
				| (MUSIC_TIMBRE_DECAY_L_MASK	& (  (dword)td.dr  << MUSIC_TIMBRE_DECAY_L_SHIFT	))	
				| (MUSIC_TIMBRE_ATTACK_L_MASK	& (  (dword)td.ar  << MUSIC_TIMBRE_ATTACK_L_SHIFT	))	
				| (MUSIC_TIMBRE_SUSTAIN_L_MASK	& (  (dword)td.sl  << MUSIC_TIMBRE_SUSTAIN_L_SHIFT	))
				| (MUSIC_TIMBRE_TOTAL_L_MASK	& (  (dword)td.tl  << MUSIC_TIMBRE_TOTAL_L_SHIFT	))	
				| (MUSIC_TIMBRE_WAVEFORM_MASK	& (  (dword)td.wave  << MUSIC_TIMBRE_WAVEFORM_SHIFT	))	
				| (MUSIC_TIMBRE_FEEDBACK_MASK	& (  (dword)td.fl << MUSIC_TIMBRE_FEEDBACK_SHIFT	));

	return timbre_data;
}

/*===========================================================================
FUNCTION MUSIC_MakeTimbreData
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

LOCAL void MUSIC_MakeTimbreData( uint16 voiceType , word Bin_timbre[])
{

	const Timbre_data_type	*UnCompressedTimbre;
	dword				CompressedTimbre_data;

	
	UnCompressedTimbre = (Timbre_data_type *)&GMIDI_timbre[voiceType];

	CompressedTimbre_data = MUSIC_CompressTimbre( UnCompressedTimbre->Mod_T );

	Bin_timbre[0] = (word)((CompressedTimbre_data >> 16) & MUSIC_TIMBRE_MODUL_MASK);
	Bin_timbre[1] = (word)(CompressedTimbre_data & MUSIC_TIMBRE_MODUL_MASK);

	CompressedTimbre_data = MUSIC_CompressTimbre( UnCompressedTimbre->Car_T );

	Bin_timbre[2] = (word)((CompressedTimbre_data >> 16) & MUSIC_TIMBRE_MODUL_MASK);
	Bin_timbre[3] = (word)(CompressedTimbre_data & MUSIC_TIMBRE_MODUL_MASK);
}

/*===========================================================================
FUNCTION MUSIC_make_release_table
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

LOCAL void MUSIC_make_release_table(int timbre_no, word timbre_ptr[])
{
	byte EGT_v, SUS_v, RR_v;

	EGT_v = (byte)(timbre_ptr[2] & (MUSIC_TIMBRE_ENV_TYPE_MASK >> 16)) >> MUSIC_TIMBRE_ENV_TYPE_SHIFT;
	SUS_v = (byte)(timbre_ptr[2] & (MUSIC_TIMBRE_SUSTAIN_ONOFF_MASK >> 16)) >> MUSIC_TIMBRE_SUSTAIN_ONOFF_SHIFT;
	RR_v = (byte)(timbre_ptr[2] & (MUSIC_TIMBRE_RELEASE_L_MASK >> 16)) >> MUSIC_TIMBRE_RELEASE_L_SHIFT;
	
//	ERR("EGT_v : %d SUS_v : %d RR_v : %d", EGT_v ,SUS_v , RR_v);
	if(EGT_v !=0 )
	{
		release_time[timbre_no] = release_time_tbl[RR_v];
	}
	else
	{
		if(SUS_v != 0)
		{
			release_time[timbre_no] = release_time_tbl[6];
		}
		else
		{
			release_time[timbre_no] = release_time_tbl[9];
		}
	}
//	ERR("release_time %d , timbre_ptr %x", release_time[timbre_no] , &timbre_ptr[2],0);
}

/*===========================================================================
FUNCTION MUSIC_make_assign_table
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

LOCAL void MUSIC_make_assign_table(word assign)
{
	assign_table[0] = assign & ALLOT_CH_0_MASK;
	assign_table[1] = (assign & ALLOT_CH_1_MASK) >> 4;
	assign_table[2] = (assign & ALLOT_CH_2_MASK) >> 8;
	assign_table[3] = (assign & ALLOT_CH_3_MASK) >> 12;
}

/*===========================================================================
FUNCTION MUSIC_update_assign_table
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

LOCAL void MUSIC_update_assign_table(word val)
{
	word ch;

	if(val & MUSIC_TIMBER_CE_M)
	{
		 ch = (val & MUSIC_PART_SET_M) >> 8;
		assign_table[ch] = val & MUSIC_TIMBER_NO_M;
	}
}

/*===========================================================================
FUNCTION Convert_RingerToNote
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/


word Convert_RingerToNote(snd_tone_type tone, uint16 dur)
{

	byte Cur_note_height=0 ;
	byte Cur_note_length=0 ;
	word Cur_note;

	///When note data entered
	if(tone >= NT_C3P && tone <=  NT_B7)
	{
		Cur_note_height = Tbl_RingerToNote_h[tone - NT_C3P];
		if( dur <= NT_32)
		{
			Cur_note_length = Tbl_RingerToNote_l[dur - NT_NULL];
		}

	}
	else if(tone == SND_SILENCE)
	{
		Cur_note_height = 0x30;
		if(dur <= NT_32)
		{
			Cur_note_length = Tbl_RingerToNote_l[dur - NT_NULL] & 0x78;
		}
	}

	Cur_note = ((((word)Cur_note_height << 8) & 0xFF00) | (Cur_note_length & 0x00FF));

	return Cur_note;
}

/*===========================================================================
FUNCTION MUSIC_soundstart
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_soundstart(void)
{
	/// Start Melody
	FM_cmd(MUSIC_FM_CONTROL_INDEX, FM_CONTOL_START_STOP_MASK, FM_CONTOL_START); 
	Music_active = TRUE;
	(void) rex_clr_timer( &snd_music_timer);
}

/*===========================================================================
FUNCTION MUSIC_soundstop
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_soundstop(void)
{
	/// Start Melody
	FM_cmd(MUSIC_FM_CONTROL_INDEX, FM_CONTOL_START_STOP_MASK, FM_CONTOL_STOP); 
	Music_active = FALSE;
	 if(Is_FilePlaying)
	 {
		mm_BellFileLoadClose();
		Is_FilePlaying = FALSE;
	 }
}

/*===========================================================================
FUNCTION MUSIC_lastsoundtone
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_lastsoundtone(word last_tone)
{
	byte chan_v , inter_l, note_l; 
	dword PowerDn_delay_t;

	if(last_tone & MUSIC_OCT_BLK_SET_M != 0)
	{
		//// note data
		chan_v = assign_table[((last_tone & MUSIC_PART_SET_M) >> 8)];
		inter_l = interval_time_tbl[((last_tone & MUSIC_INTERVAL_SET_M) >> 3)];

		if(inter_l <= 0x0A)
		{
			note_l = note_length_tbl[(last_tone & MUSIC_NOTE_LENGTH_M)][0];
		}
		else
		{
			note_l =  note_length_tbl[last_tone & MUSIC_NOTE_LENGTH_M][1];
		}

		//ERR("chan %x inter %d note %d",chan_v , inter_l , note_l );
/*		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 
		if(chan_v ==0)
		{
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078 ); 
		}
		else if(chan_v ==1)
		{
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3178 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3178 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3178 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3178 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3178 ); 
		}
		else if(chan_v ==2)
		{
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3278 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3278 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3278 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3278 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3278 ); 
		}
		else
		{
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3378 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3378 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3378 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3378 ); 
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3378 ); 
		}

		/// Enable IRQ for FIFO empty
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 
*/
//		PowerDn_delay_t = 100;
//		PowerDn_delay_t = ((dword)note_l * 1091 * Opt_tempo / 90000)// (60000(msec) * note_l /12) / (8728/Opt_tempo)
//			+ release_time[chan_v ];
		PowerDn_delay_t = ((dword)Opt_tempo * 30000 / 1091);// 60000(msec) * (8728/Opt_tempo) /4

	}
	else
	{
		/// rest data
		PowerDn_delay_t = 10;
	}
	//ERR("last tone %x timer %d",last_tone, PowerDn_delay_t, 0);
	//// 마지막 톤을 다 재생한 후에 멜로디 칩의 파워를 꺼라.
	(void) rex_set_timer( &snd_music_timer , PowerDn_delay_t);
}

/*===========================================================================
FUNCTION MUSIC_music_id
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_music_id(word Melody_I, word Repeat_num)
{
//	int volume;

	if(Repeat_num == 0)
		Melody_repeat_ena = TRUE;
	else
	{
		Melody_repeat_ena = FALSE;
		Melody_repeat_num = Repeat_num;
	}

	MUSIC_sound_stop_en = FALSE;
	if(sdb_get_music( Melody_I, &Cur_TimbreTbl ) == SDB_FAIL)
	{
		return;
	}
//	Cur_TimbreTbl = (word *)(melody_fn_p[Melody_I]);

//	Repeat_num =1;
//	Melody_repeat_ena = FALSE;
  /* While we are generating a Tone (with ANY generator) we will prevent
  ** the Phone from Sleeping.  Note that there are several reason we might
  ** want the phone to stay awake.
  */
//  SND_SLEEP_FORBID();

    MUSIC_ReadTimbreData();

	/// Set Spk volume
//	FM_cmd(MUSIC_SPK_VOLUME_INDEX, MUSIC_SPK_VOLUME_MASK, FM_vol ); /// Sound Start

	/// Set FM volume
//	FM_cmd(MUSIC_FM_VOLUME_INDEX, MUSIC_FM_VOLUME_MASK, FM_vol ); /// Sound Start
//	FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, MUSIC_REFILL_REMEAINED); 
	MUSIC_FillfirstData();

#if (MODEL_ID !=MODEL_A10)
	tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  MUSIC_ReFillData );
#else
	gpio_int_set_handler ( GPIO_INT_7, ACTIVE_LOW, &MUSIC_ReFillData);
#endif


}
/*===========================================================================
FUNCTION MUSIC_music
DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
static word *inBuffer;

static mus_compact_melody_type inBuffer_music = {
  SND_COMPACT_SOUND,
	0,
  (mus_compact_tone_type *) &inBuffer
};

void MUSIC_music(word *Melody_bp, word Repeat_num)
{
//	int volume;

	if(Repeat_num == 0)
		Melody_repeat_ena = TRUE;
	else
	{
		Melody_repeat_ena = FALSE;
		Melody_repeat_num = Repeat_num;
	}

	MUSIC_sound_stop_en = FALSE;

	if(Melody_bp == NULL)
		return;
/*	if(sdb_get_music( Melody_I, &Cur_TimbreTbl ) == SDB_FAIL)
	{
		return;
	}*/
	inBuffer_music.tone_array = (mus_compact_tone_type *)Melody_bp;
	Cur_TimbreTbl = (snd_sound_type *)&inBuffer_music;

  /* While we are generating a Tone (with ANY generator) we will prevent
  ** the Phone from Sleeping.  Note that there are several reason we might
  ** want the phone to stay awake.
  */
//  SND_SLEEP_FORBID();
    MUSIC_ReadTimbreData();
	/// Set Spk volume
//	FM_cmd(MUSIC_SPK_VOLUME_INDEX, MUSIC_SPK_VOLUME_MASK, FM_vol ); /// Sound Start

	/// Set FM volume
//	FM_cmd(MUSIC_FM_VOLUME_INDEX, MUSIC_FM_VOLUME_MASK, FM_vol ); /// Sound Start
//	FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, MUSIC_REFILL_REMEAINED); 
	MUSIC_FillfirstData();

#if (MODEL_ID!=MODEL_A10)
	tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  MUSIC_ReFillData );
#else
	gpio_int_set_handler ( GPIO_INT_7, ACTIVE_LOW, &MUSIC_ReFillData);
#endif

}
/*===========================================================================
FUNCTION MUSIC_music_fileopen

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/


void MUSIC_music_fileopen(char *Melody_fn, word Repeat_num )
{
	if(Repeat_num == 0)
		Melody_repeat_ena = TRUE;
	else
	{
		Melody_repeat_ena = FALSE;
		Melody_repeat_num = Repeat_num;
	}

    MUSIC_sound_stop_en = FALSE;
#if (MODEL_ID!=MODEL_A10)
	tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  MUSIC_ReFill_FileData );
#else
	gpio_int_set_handler ( GPIO_INT_7, ACTIVE_LOW, &MUSIC_ReFill_FileData);
#endif
    ///이전에 열려있는 파일이 있다면 그냥 닫는다.
    if(Is_FilePlaying)
    {
		mm_BellFileLoadClose();
		Is_FilePlaying = FALSE;
	}

    ///파일을 연다.
    if(mm_BellFileLoadOpen(Melody_fn, &Cur_File_size, &b_Cur_File_bufp) != MM_OK)
    {
	    ///파일을 열다가 실패했을때의 처리를 한다.
	return;
    }
    Is_FilePlaying = TRUE;

        Cur_File_bufp = (word *)b_Cur_File_bufp;

    MUSIC_ReadTimbreFileData();
	MUSIC_FillfirstFileData();
}
/*void MUSIC_music(word *Melody_ptr, boolean Repeat_en)
{

	Melody_repeat_ena = Repeat_en;
	MUSIC_power_on(MUS_MELODY);

    tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  MUSIC_ReFillData );

	Cur_TimbreTbl = Melody_ptr;

	MUSIC_ReadTimbreData();
	MUSIC_FillfirstData();

	MUSIC_soundstart();
}*/

/*===========================================================================
FUNCTION Fill_FIFO

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void Fill_FIFO(int note_num )
{
	int loop;
	word note_h, note_l, note_t;
	mus_flexible_tone_type flexible_tone;  /* temp flexible tone structure */

	loop = 0;
	while(loop < note_num )
	{
		flexible_tone = (((mus_flexible_melody_type *)Cur_TimbreTbl)->tone_array)[WriteToFIFO_NoteData_p];
		note_h = flexible_tone.height;
		note_l = flexible_tone.dur;
		
		switch(note_h)
		{
//		case SND_SILENCE:
//			note_t = Convert_RingerToNote(SND_SILENCE, note_l );
//			FM_note_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, note_t );
//			WriteToFIFO_NoteData_p++;
//			loop++;
//			break;
		case SND_LABEL:
			WriteToFIFO_NoteData_p++;
			label_location[labal_stack_p++] = WriteToFIFO_NoteData_p;
			break;
		case SND_BACK_TO_LABEL:
			if( labal_stack_p == 0)
				WriteToFIFO_NoteData_p ++;
			else
			{
				labal_stack_p-- ;
				WriteToFIFO_NoteData_p = label_location[labal_stack_p];
			}
			break;
		case SND_RPT:
			WriteToFIFO_NoteData_p = 0;
			break;
		default:
			if(note_l >= NT_1 && note_l <= NT_2D)
			{
				if(note_num - loop < 2)
				{
					return;
				}
				else
				{
					note_t = Convert_RingerToNote(note_h, note_l );

					FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, note_t );
					if(note_l == NT_1)
					{
						note_t = Convert_RingerToNote(note_h, NT_2 );
						FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, note_t );
					}
					else if(note_l == NT_2D)
					{
						note_t = Convert_RingerToNote(note_h, NT_4 );
						FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, note_t );
					}
					
					WriteToFIFO_NoteData_p++;
					loop+=2;
				}
			}
			else
			{
				note_t = Convert_RingerToNote(note_h, note_l );
				FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, note_t );
				WriteToFIFO_NoteData_p++;
				loop++;
			}
			break;
		}
	}
}

/*===========================================================================
FUNCTION MUSIC_ReadTimbreData

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_ReadTimbreData(void)
{
	int loop , j;

	if(((mus_compact_melody_type *)Cur_TimbreTbl)->type == SND_COMPACT_SOUND)
	{
		Total_number_Note = ((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[0]/2;
		WriteToFIFO_NoteData_p = 42 ;
//		UseNoteDataFromFIFO_p = 42;
		if(Total_number_Note <= WriteToFIFO_NoteData_p )
		{
			Total_number_Note = WriteToFIFO_NoteData_p;
		}

		for (loop=0; loop< 8 ; loop++)
		{
			for (j=0; j< 4 ; j++)
			{
				FM_cmd(MUSIC_1ST_TIMBRE_MODUL_INDEX1 + loop*4 + j, MUSIC_TIMBRE_MODUL_MASK, 
					((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[loop * 4 + j + 1]);
			}
//			ERR("ptr1 %x ptr2 %x",((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array, (((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array) + loop * 4 + j + 1,0);
			MUSIC_make_release_table(loop, 
				(((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array) + loop * 4 + j + 1);
		}

		FM_cmd(MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX, 
			ALLOT_CH_3_MASK | ALLOT_CH_2_MASK | ALLOT_CH_1_MASK | ALLOT_CH_0_MASK    /// Sound Start
			, ((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[33] ); // =110

		//// Make assign table
		MUSIC_make_assign_table( ((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[33] );

		//// Set Tempo data
		Opt_tempo  = ((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[34];
		FM_cmd(MUSIC_TEMPO_DATA_INDEX, MUSIC_TEMPO_DATA_MASK, Opt_tempo); // =110/// Sound Start
	}
	else if(((mus_flexible_melody_type *)Cur_TimbreTbl)->type == SND_FLEXIBLE_SOUND)
	{
		word Bin_timbre[4];

		///Load General MIDI Timbre data
		MUSIC_MakeTimbreData( 53 , Bin_timbre);

		/// 멜로디의 음역을 다 표현하기 위해 같은 음색으로 multiple = 4와 multiple = 1의 두가지를 load.
		/// 이렇게하면 각각이 3개의 octave를 가지고 전체적으로는 5 octave를 cover한다.
		Bin_timbre[2] = (Bin_timbre[2] & 0x1FFF) | 0x2000;  /// Multiple X1
		for (loop= 0 ; loop< 4 ; loop++)
		{
			FM_cmd(MUSIC_1ST_TIMBRE_MODUL_INDEX1 + loop , MUSIC_TIMBRE_MODUL_MASK, Bin_timbre[loop] );
		}

//		MUSIC_make_release_table(0, Bin_timbre);

		MUSIC_MakeTimbreData( 53 , Bin_timbre);

		Bin_timbre[2] = (Bin_timbre[2] & 0x1FFF) | 0x8000;/// Multiple X4

		for (loop= 0 ; loop< 4 ; loop++)
		{
			FM_cmd(MUSIC_8TH_TIMBRE_MODUL_INDEX1 + loop , MUSIC_TIMBRE_MODUL_MASK, Bin_timbre[loop] );
		}

//		MUSIC_make_release_table(1, Bin_timbre);

		/// Channel 0에는 낮은 음역을 Channel1에는 높은 음역을 올린다.
		FM_cmd(MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX, 
			ALLOT_CH_3_MASK | ALLOT_CH_2_MASK | ALLOT_CH_1_MASK | ALLOT_CH_0_MASK    /// Sound Start
			, 0x1070 ); // =110

		//// Make assign table
//		MUSIC_make_assign_table( 0x1010);

		/// Find optimized tempo value
		Opt_tempo  = (word)(8728 / ((mus_flexible_melody_type *)Cur_TimbreTbl)->tempo) / 2; 
		FM_cmd(MUSIC_TEMPO_DATA_INDEX, MUSIC_TEMPO_DATA_MASK, Opt_tempo); // =110/// Sound Start

		//변수 초기화
		labal_stack_p = 0;
		WriteToFIFO_NoteData_p = 0;
	}

}

/*===========================================================================
FUNCTION MUSIC_ReadTimbreFileData

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_ReadTimbreFileData(void)
{
	int loop , j;

	mm_BellFileBlockLoad(2);
	Total_number_Note = Cur_File_bufp[0]/2;

	WriteToFIFO_NoteData_p = 42 ;

		if(Total_number_Note <= WriteToFIFO_NoteData_p )
		{
			Total_number_Note = WriteToFIFO_NoteData_p;
		}

	for (loop=0; loop< 8 ; loop++)
	{
		mm_BellFileBlockLoad(8);
		for (j=0; j< 4 ; j++)
		{
			FM_cmd(MUSIC_1ST_TIMBRE_MODUL_INDEX1 + loop*4 + j, MUSIC_TIMBRE_MODUL_MASK, 
				Cur_File_bufp[j]);
		}
//			ERR("ptr1 %x ptr2 %x",((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array, (((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array) + loop * 4 + j + 1,0);
		MUSIC_make_release_table(loop, 
			Cur_File_bufp + j);
	}

	mm_BellFileBlockLoad(4);
	FM_cmd(MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX, 
		ALLOT_CH_3_MASK | ALLOT_CH_2_MASK | ALLOT_CH_1_MASK | ALLOT_CH_0_MASK    /// Sound Start
		, Cur_File_bufp[0] ); // =110

	//// Make assign table
	MUSIC_make_assign_table( Cur_File_bufp[0] );

	//// Set Tempo data
	Opt_tempo  = Cur_File_bufp[1];
	FM_cmd(MUSIC_TEMPO_DATA_INDEX, MUSIC_TEMPO_DATA_MASK, Opt_tempo); // =110/// Sound Start

	mm_BellFileBlockSeek(WriteToFIFO_NoteData_p * 2);

}

/*===========================================================================
FUNCTION MUSIC_FillfirstData

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_FillfirstData(void)
{
	int loop;
	int note_num ;
	int rest_num;


	if(((mus_compact_melody_type *)Cur_TimbreTbl)->type == SND_COMPACT_SOUND)
	{
		/// 지정된 횟수만큼 play할 경우에 첫번째 play이므로 하나 뺀다.
		if(Melody_repeat_num > 0)
		{
			Melody_repeat_num--;
		}

		rest_num = Total_number_Note - WriteToFIFO_NoteData_p;
		note_num = MIN( 32, rest_num);

		/// Disable IRQ for writing note data
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 
		
		for(loop = 0;loop < note_num ; loop++, WriteToFIFO_NoteData_p ++)
		{
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
				((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p] ); 

			MUSIC_update_assign_table(((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p]);
		}

	if(0x20 >  Total_number_Note - 42)
		refill_alert_num = 1;
	else
		refill_alert_num = MUSIC_REFILL_REMEAINED ;

//		refill_alert_num = MIN(MUSIC_REFILL_REMEAINED, note_num );
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, refill_alert_num ); 

		/// Enable IRQ for FIFO empty
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 
	}
	else if(((mus_flexible_melody_type *)Cur_TimbreTbl)->type == SND_FLEXIBLE_SOUND)
	{
//		note_num = MIN( 32, (Total_number_Note - WriteToFIFO_NoteData_p));

		/// Disable IRQ for writing note data
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 
		
		Fill_FIFO(32 );
		
/*		for(loop = 0;loop < note_num ; loop++, WriteToFIFO_NoteData_p ++)
		{
			FM_note_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
				Convert_RingerToNote(Cur_TimbreTbl->tone_array[WriteToFIFO_NoteData_p].heigt
				, Cur_TimbreTbl->tone_array[WriteToFIFO_NoteData_p].dur )); 
		}*/

//		refill_alert_num = MIN(MUSIC_REFILL_REMEAINED, note_num );
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, MUSIC_REFILL_REMEAINED ); 

		/// Enable IRQ for FIFO empty
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 
	}
}

/*===========================================================================
FUNCTION MUSIC_FillfirstFileData

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_FillfirstFileData(void)
{
	int loop;
	int note_num ;
	int rest_num, norm_note_num, rest_note_num ;

	/// 지정된 횟수만큼 play할 경우에 첫번째 play이므로 하나 뺀다.
	if(Melody_repeat_num > 0)
	{
		Melody_repeat_num--;
	}

	rest_num = Total_number_Note - WriteToFIFO_NoteData_p;
	note_num = MIN( 32, rest_num);

	mm_BellFileBlockLoad(note_num * 2);

	/// Disable IRQ for writing note data
	FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 
	
	for(loop = 0;loop < note_num ; loop++, WriteToFIFO_NoteData_p ++)
	{
		FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
			Cur_File_bufp[loop] ); 

		MUSIC_update_assign_table(Cur_File_bufp[loop]);
	}

	if(0x20 >  Total_number_Note - 42)
		refill_alert_num = 1;
	else
		refill_alert_num = MUSIC_REFILL_REMEAINED ;
//		refill_alert_num = MIN(MUSIC_REFILL_REMEAINED, note_num );
	FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, refill_alert_num ); 

	/// Enable IRQ for FIFO empty
	FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 

	///Next data Pre-load
	if(WriteToFIFO_NoteData_p >= Total_number_Note )
	{
		if(Melody_repeat_ena || Melody_repeat_num > 0)
		{
			if(Melody_repeat_num > 0)
			{
				Melody_repeat_num--;
			}

			WriteToFIFO_NoteData_p = 42 ;

			rest_note_num = Total_number_Note - WriteToFIFO_NoteData_p;
			norm_note_num = 32 - refill_alert_num;
			note_num = MIN( norm_note_num , rest_note_num);

			nNext_data_load = note_num;
			ReadNextMelodyReadOK = TRUE;

			if(refill_alert_num == MUSIC_REFILL_REMEAINED)
			{
				//음표가 32개 이상인 경우를 따로 함    2000.5.9
				mm_BellFileBlockSeek(WriteToFIFO_NoteData_p * 2);
				mm_BellFileBlockLoad(note_num * 2);
			}
			//음표가 32개 이하인 경우에는 더 이상의 file access를 하지 않음
		}
		else
		{
			if(MUSIC_sound_stop_en)
			{
				///멜로디를 완전히 끝내겠음
				///인터럽트 서비스 루틴을 호출하지 않도록 함
#if (MODEL_ID!=MODEL_A10)
				tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  NULL);
#else
				gpio_int_set_handler ( GPIO_INT_7, ACTIVE_LOW, NULL);
#endif
//				tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  NULL );

				///이전 톤이 마지막 톤이므로 
				WriteToFIFO_NoteData_p--;
				if(refill_alert_num == MUSIC_REFILL_REMEAINED)
				{
					//음표가 32개 이상인 경우를 따로 함    2000.5.9
					mm_BellFileBlockSeek(WriteToFIFO_NoteData_p * 2);
					mm_BellFileBlockLoad(2);
					MUSIC_lastsoundtone(
						Cur_File_bufp[0]);
				}
				else
				{
					//음표가 32개 이하인 경우에는 더 이상의 file access를 하지 않음
					MUSIC_lastsoundtone(
						Cur_File_bufp[WriteToFIFO_NoteData_p - 0x20]);
				}
//					MUSIC_soundstop();
				MUSIC_sound_stop_en = FALSE;
			}
			else
			{
				///멜로디를 완전히 끝내겠다면 FIFO interrupt의 level을 0으로 
				///맞추어야 함
				FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 
				FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
				FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078 ); 
				/// Enable IRQ for FIFO empty
				FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 

				MUSIC_sound_stop_en = TRUE;
			}
		}
	}
	else
	{
		//음표가 32개 이하인 경우에는 이쪽으로 오지 않음
		rest_note_num = Total_number_Note - WriteToFIFO_NoteData_p;
		norm_note_num = 32 - refill_alert_num;
		note_num = MIN( norm_note_num , rest_note_num);
		
		mm_BellFileBlockLoad(note_num * 2);
		nNext_data_load = note_num;
		ReadNextMelodyReadOK = TRUE;
	}
}

/*===========================================================================
FUNCTION MUSIC_ReFillData

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_ReFillData(void)
{
	int loop;
	int note_num;
	int rest_note_num, norm_note_num;

	if(((mus_compact_melody_type *)Cur_TimbreTbl)->type == SND_COMPACT_SOUND)
	{

		if(WriteToFIFO_NoteData_p >= Total_number_Note )
		{
/*			if(Melody_repeat_ena || Melody_repeat_num >0)
			{
				rest_note_num = MIN((32 - refill_alert_num - note_num), (Total_number_Note - 42)) ;

				WriteToFIFO_NoteData_p = 42 ;

				for(loop = 0;loop < rest_note_num ; loop++ )
				{
					FM_note_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
						((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p] ); 
					WriteToFIFO_NoteData_p++;
				}
			}
*/
			if(Melody_repeat_ena || Melody_repeat_num > 0)
			{
				if(Melody_repeat_num > 0)
				{
					Melody_repeat_num--;
				}

				WriteToFIFO_NoteData_p = 42 ;

				FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 

				rest_note_num = Total_number_Note - WriteToFIFO_NoteData_p;
				norm_note_num = 32 - refill_alert_num;
				note_num = MIN( norm_note_num , rest_note_num);
				
				for(loop = 0;loop < note_num ; loop++)
				{
					FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
						((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p] ); 

					MUSIC_update_assign_table(((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p]);

					WriteToFIFO_NoteData_p++;
				}
				
				FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 

			}
			else
			{
				if(MUSIC_sound_stop_en)
				{
					///멜로디를 완전히 끝내겠음
					///인터럽트 서비스 루틴을 호출하지 않도록 함
#if (MODEL_ID!=MODEL_A10)
				tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  NULL);
#else
				gpio_int_set_handler ( GPIO_INT_7, ACTIVE_LOW, NULL);
#endif
//					tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  NULL );

					///이전 톤이 마지막 톤이므로 
					WriteToFIFO_NoteData_p--;
					MUSIC_lastsoundtone(
						((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p]);
//					MUSIC_soundstop();
					MUSIC_sound_stop_en = FALSE;
				}
				else
				{
					///멜로디를 완전히 끝내겠다면 FIFO interrupt의 level을 0으로 
					///맞추어야 함
					FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 
					FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
					FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078 ); 
					/// Enable IRQ for FIFO empty
					FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 

					MUSIC_sound_stop_en = TRUE;
				}
			}
		}
		else
		{
	//		WriteToFIFO_NoteData_p--;

			/// Disable IRQ for writing note data
			FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 

			rest_note_num = Total_number_Note - WriteToFIFO_NoteData_p;
			norm_note_num = 32 - refill_alert_num;
			note_num = MIN( norm_note_num , rest_note_num);
			
			for(loop = 0;loop < note_num ; loop++)
			{
				FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
					((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p] ); 
				MUSIC_update_assign_table(((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p]);
				WriteToFIFO_NoteData_p++;
			}

//			if(WriteToFIFO_NoteData_p >= Total_number_Note && (Melody_repeat_ena || Melody_repeat_num >0))
//			{
//				rest_note_num = MIN((32 - refill_alert_num - note_num), (Total_number_Note - 42)) ;

//				WriteToFIFO_NoteData_p = 42 ;

//				for(loop = 0;loop < rest_note_num ; loop++ )
//				{
//					FM_note_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
//						((mus_compact_melody_type *)Cur_TimbreTbl)->tone_array[WriteToFIFO_NoteData_p] ); 
//					WriteToFIFO_NoteData_p++;
//				}

//			}

	//			FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
	//		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, MUSIC_REFILL_REMEAINED); 
			/// Enable IRQ for FIFO empty
			FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 
		}
	}
	else if(((mus_flexible_melody_type *)Cur_TimbreTbl)->type == SND_FLEXIBLE_SOUND)
	{
//		if(WriteToFIFO_NoteData_p >= Total_number_Note && !Melody_repeat_ena)
//		{
//			tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  NULL );
	//		if(MUSIC_sound_stop_en)
	//		{
	//			MUSIC_soundstop();
	//			MUSIC_sound_stop_en = FALSE;
	//		}
	//		else
	//		{
	//			FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
	//			MUSIC_sound_stop_en = TRUE;
	//		}
//		}
//		else
		{
	//		WriteToFIFO_NoteData_p--;

			/// Disable IRQ for writing note data
			FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 

//			note_num = MIN( (32 - refill_alert_num) , (Total_number_Note - WriteToFIFO_NoteData_p));
			
			Fill_FIFO(MUSIC_REFILL_REMEAINED );
/*			for(loop = 0;loop < note_num ; loop++)
			{
				FM_note_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
					Convert_RingerToNote(Cur_TimbreTbl->tone_array[WriteToFIFO_NoteData_p].heigt
					, Cur_TimbreTbl->tone_array[WriteToFIFO_NoteData_p].dur )); 

				 WriteToFIFO_NoteData_p++;
			}*/

/*			if(WriteToFIFO_NoteData_p >= Total_number_Note && (Melody_repeat_ena || Melody_repeat_num >0))
			{
				rest_note_num = MIN((32 - refill_alert_num - note_num), (Total_number_Note - 42)) ;

				WriteToFIFO_NoteData_p = 42 ;
				UseNoteDataFromFIFO_p = 42;

				Fill_FIFO(rest_note_num  );

				for(loop = 0;loop < rest_note_num ; loop++ )
				{
					FM_note_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
						Convert_RingerToNote(Cur_TimbreTbl->tone_array[WriteToFIFO_NoteData_p].heigt
						, Cur_TimbreTbl->tone_array[WriteToFIFO_NoteData_p].dur )); 

					WriteToFIFO_NoteData_p++;
				}
			}*/
	//			FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
	//		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, MUSIC_REFILL_REMEAINED); 
			/// Enable IRQ for FIFO empty
			FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 
		}
	}
}

/*===========================================================================
FUNCTION MUSIC_ReFill_FileData

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_ReFill_FileData(void)
{
	int loop;
	int note_num;
	int rest_note_num, norm_note_num;

//	if(ReadNextMelodyReadOK)
	{
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 

		for(loop = 0;loop < nNext_data_load ; loop++)
		{
			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
				Cur_File_bufp[loop]); 

			MUSIC_update_assign_table(Cur_File_bufp[loop]);

			WriteToFIFO_NoteData_p++;
		}
		
		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 
	}

	if(WriteToFIFO_NoteData_p >= Total_number_Note )
	{
		if(Melody_repeat_ena || Melody_repeat_num > 0)
		{
			if(Melody_repeat_num > 0)
			{
				Melody_repeat_num--;
			}

			WriteToFIFO_NoteData_p = 42 ;

			rest_note_num = Total_number_Note - WriteToFIFO_NoteData_p;
			norm_note_num = 32 - refill_alert_num;
			note_num = MIN( norm_note_num , rest_note_num);
			nNext_data_load = note_num;
			if(refill_alert_num == MUSIC_REFILL_REMEAINED)
			{
					//음표가 32개 이상인 경우를 따로 함    2000.5.9
				mm_BellFileBlockSeek(WriteToFIFO_NoteData_p * 2);
				ReadNextMelodyReadOK = FALSE;
				(void) rex_set_sigs( &snd_tcb, SND_FILE_READ_SIG );
			}
			//음표가 32개 이하인 경우에는 더 이상의 file access를 하지 않음
//			mm_BellFileBlockLoad(note_num * 2);
		}
		else
		{
			if(MUSIC_sound_stop_en)
			{
				///멜로디를 완전히 끝내겠음
				///인터럽트 서비스 루틴을 호출하지 않도록 함
#if (MODEL_ID!=MODEL_A10)
				tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  NULL);
#else
				gpio_int_set_handler ( GPIO_INT_7, ACTIVE_LOW, NULL);
#endif
//				tramp_set_isr( TRAMP_GPIO_INT_0_ISR,  NULL );

				///이전 톤이 마지막 톤이므로 
				WriteToFIFO_NoteData_p--;

//				mm_BellFileBlockSeek(WriteToFIFO_NoteData_p * 2);
//				mm_BellFileBlockLoad(2);

				MUSIC_lastsoundtone(
					Cur_File_bufp[nNext_data_load -1]);
//					MUSIC_soundstop();
				MUSIC_sound_stop_en = FALSE;
			}
			else
			{
				///멜로디를 완전히 끝내겠다면 FIFO interrupt의 level을 0으로 
				///맞추어야 함
				FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 
				FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
				FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078 ); 
				FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 
				MUSIC_sound_stop_en = TRUE;
			}
		}
	}
	else
	{

		/// Disable IRQ for writing note data
//		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 
		//음표가 32개 이하인 경우에는 이쪽으로 들어오지 않음
		rest_note_num = Total_number_Note - WriteToFIFO_NoteData_p;
		norm_note_num = 32 - refill_alert_num;
		note_num = MIN( norm_note_num , rest_note_num);
		nNext_data_load = note_num;
		ReadNextMelodyReadOK = FALSE;
		
		(void) rex_set_sigs( &snd_tcb, SND_FILE_READ_SIG );

//		mm_BellFileBlockLoad(note_num * 2);


//		for(loop = 0;loop < note_num ; loop++)
//		{
//			FM_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 
//				Cur_File_bufp[loop] ); 
//			MUSIC_update_assign_table(Cur_File_bufp[loop]);
//			WriteToFIFO_NoteData_p++;
//		}

		/// Enable IRQ for FIFO empty
//		FM_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, WAIT_FIFO_EMPTY_INT); 
	}
}

/*===========================================================================
FUNCTION MUSIC_volcmd

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void MUSIC_volcmd(mus_volume_ctr_type voltype, uint16 vol )
{
#ifdef SAVE_AUDIO_TEST_DATA
	//This is a code for Audio test
	extern boolean Test_sound_on;
	extern audio_param_type TestVol;
#endif// SAVE_AUDIO_TEST_DATA
	uint16 avol;

	if(vol == 0 && MUSIC_power_status != 0)
		return;

	avol = vol;

	if(voltype == MUS_SPK_VOL)
	{
	/// Set Spk volume
#ifdef SAVE_AUDIO_TEST_DATA
		if(Test_sound_on)
			avol = TestVol.SPKvol;
#endif// SAVE_AUDIO_TEST_DATA

		FM_cmd(MUSIC_SPK_VOLUME_INDEX, MUSIC_SPK_VOLUME_MASK, avol); /// 
		if(	avol != 0)
			Cur_FM_vol = avol;
	}
	else if(voltype == MUS_FM_VOL)
	{
#ifdef SAVE_AUDIO_TEST_DATA
		if(Test_sound_on)
			avol = TestVol.FMvol;
#endif// SAVE_AUDIO_TEST_DATA

	/// Set FM volume
		FM_cmd(MUSIC_FM_VOLUME_INDEX, MUSIC_FM_VOLUME_MASK, avol); /// 
		if(	avol != 0)
			Cur_SPK_vol = avol;
	}
	else if(voltype == MUS_HPOUT_VOL)
	{
	/// Set HPOUT volume
#ifdef SAVE_AUDIO_TEST_DATA
		if(Test_sound_on)
			avol = TestVol.HPvol;
#endif// SAVE_AUDIO_TEST_DATA

		FM_cmd(MUSIC_HPOUT_VOLUME_INDEX, MUSIC_HPOUT_VOLUME_MASK, avol);
		if(	avol != 0)
			Cur_HPOUT_vol = avol;

	}
}

/*===========================================================================
FUNCTION ReadMelodyFromFlash

DESCRIPTION

DEPENDENCIES
	SND_FILE_READ_SIG

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/

void ReadMelodyFromFlash(void)
{
	if(ReadNextMelodyReadOK)
		return;

	if(mm_BellFileBlockLoad(nNext_data_load * 2) == MM_OK)
	{
		ReadNextMelodyReadOK = TRUE;
	}
}

void Make_MyMelody_Header(
	word *Hp,  /* preplay ram buffer   */
	byte timb    /* Timbre to load     */
)
{
	word Bin_timbre[4];

	///Load General MIDI Timbre data
	MUSIC_MakeTimbreData( timb , Bin_timbre);
	/// 멜로디의 음역을 다 표현하기 위해 같은 음색으로 multiple = 4와 multiple = 1의 두가지를 load.
	/// 이렇게하면 각각이 3개의 octave를 가지고 전체적으로는 5 octave를 cover한다.
	Bin_timbre[2] = (Bin_timbre[2] & 0x1FFF) | 0x2000;  /// Multiple X1
	memcpy(Hp, Bin_timbre, sizeof(word)* 4);

	MUSIC_MakeTimbreData( timb , Bin_timbre);
	Bin_timbre[2] = (Bin_timbre[2] & 0x1FFF) | 0x8000;/// Multiple X4
	memcpy(Hp + 28, Bin_timbre, sizeof(word)* 4);
	//		MUSIC_make_release_table(1, Bin_timbre);

	/// Channel 0에는 낮은 음역을 Channel1에는 높은 음역을 올린다.
	Hp[32] = (ALLOT_CH_3_MASK | ALLOT_CH_2_MASK | ALLOT_CH_1_MASK | ALLOT_CH_0_MASK)
		& 0x1070 ;

		//// Make assign table
//		MUSIC_make_assign_table( 0x1010);

		/// Find optimized tempo value
//		Opt_tempo  = (word)(8728 / ((mus_flexible_melody_type *)Cur_TimbreTbl)->tempo) / 2; 
//		FM_cmd(MUSIC_TEMPO_DATA_INDEX, MUSIC_TEMPO_DATA_MASK, Opt_tempo); // =110/// Sound Start

		//변수 초기화
//		labal_stack_p = 0;
//		WriteToFIFO_NoteData_p = 0;
}

///볼륨 옵셋 값을 읽음 in 2000.5.22
sint15 get_volume_offset(void)
{
	if(Cur_TimbreTbl != NULL)
		return ((mus_compact_melody_type *)Cur_TimbreTbl)->vol;
	else
		return 0;
}