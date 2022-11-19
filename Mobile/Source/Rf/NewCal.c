
/*****************************************************************************
 *
 *		RF calibration support functions for SKTT project
 *
 *		Copyright (C) 1999, 2000 SK Telecom
 *
 *		'Dooly' project 
 *
 *****************************************************************************
 *
 *		FileName : NewCal.c
 *		Revision : 0.00.001
 *		 Created : 00.08.08		by chos
 *
 *****************************************************************************
 *
 *		Revision detail
 *		---------------
 *    
 *                      00.09.14        jwpark
 *          porting to A10(MSM5000) from A7
 *
 *		Rev	0.00.001	00.08.08 09:00	chos
 *			created this module
 *
 *****************************************************************************/

#include "target.h"
#include "customer.h"

#ifdef	FEATURE_SKTT_NEW_CAL

#include	"rf.h"
#include	"cagc.h"
#include	"enc.h"
#include	"rfnv.h"
#include	"NewCal.h"
#include    "bio.h"


// from rfmsm.c
//			-------------------------------------------->
/* Constant:  microseconds per millisecond
*/
#define RF_US_PER_MS ((word)1000)

/* Microseconds needed for RF synthesizers to settle before testing ALL_LOCK
** Use the millisecond value defined in rf.h, subtract the clock tick bias,
** and expand it to microseconds.
*/
// jwpark 00.09.14 : MSM5000 source에 맞추어 값을 변경한다.
//
#define RF_TUNE_CHAN_WAIT_US \
  ((word)(( (word)RF_TUNE_CHAN_WAIT - (word)CLK_MS_PER_TICK ) * RF_US_PER_MS ))

#define RF_ENABLE_PWR_WAIT_US \
  ((word)(( RF_ENABLE_PWR_WAIT - CLK_MS_PER_TICK ) * RF_US_PER_MS ))


/* Pointer to NV digital items/tables
*/
extern rfnv_digital_items_type *rf_nv_dig_items_ptr;
extern byte    rf_freq_index;

//			<--------------------------------------------


void	rf_DigitalTxOn ( void )
{
    qword high_bit;

	// jwpark 00.09.27
	// MSM5000에서는 access channel을 사용하기 위해서는
	// 추가적을 설정해 주어야 하는 것들이 txc_access_init()안에
	// 들어 있다.
	{ // from txc.c
		typedef enum {
		  TXC_CDMA_S,           // Initializing CDMA
		  TXC_ACC_IDLE_S,       // Idle on Paging channel
		  TXC_ACC_JUMP_S,       // Performing jump to hyperspace
		  TXC_ACC_ATT_S,        // Performing an access attempt
		  TXC_TRAFFIC_S,        // Traffic channel
		  TXC_EXIT_S            // Stop state
		}txc_state_type;
		extern void txc_access_init(txc_state_type state);
		txc_access_init(TXC_CDMA_S);
	}

	//--------------------------------------------------
	//	this routine from txc_cdma_enc_setup() in txc.c
	//--------------------------------------------------
	//		---------------------------------------------------->
    enc_tx_clk_enable(TRUE);

    enc_iq_pn_state(0x4000, 0x4000); /* Load I and Q with MSB 1      */

    qw_set(high_bit, 0x200L, 0L);    /* Init quad word, set bit 41 [0..41] */
    enc_user_pn_state(high_bit);     /* Load User PN state */
    enc_user_pn_mask(high_bit);      /* Load User PN mask  */

    /* -------------------------------------------
    ** Power up and program the VHF TX synthesizer
    ** ------------------------------------------- */
    rf_enable_tx_pwr();
	clk_busy_wait (RF_ENABLE_PWR_WAIT_US); // jwpark 00.09.14
    rf_tx_enable();

    enc_tx_enable(TRUE);
	//		<----------------------------------------------------
}

void	rf_DigitalTxOff ( void )
{
	enc_tx_enable ( FALSE );

	/* -------------------------------
	** Turn off the transmit circuitry
	** ------------------------------- */
	rf_tx_disable ();
}

void	rf_Set_TX_AGC_ADJ ( int2 nValue )
{
	nValue &= 0x1ff;	// mask 9 bits

	CAGC_OUTM ( CAGC_DATA_3, CAGC_TX_AGC_M,
		WORD_LO ( nValue ) << CAGC_TX_AGC_LSB_SHIFT );
	CAGC_OUT ( CAGC_DATA_2, WORD_LO ( nValue >> 1 ) );
}

void	rf_TxAGCManualMode ( void )
{
	//--------------------------------------------------
	//	set TxAGC for max power
	//--------------------------------------------------

	// set TxAGC manual mode (CPU mode)
	CAGC_OUTM ( CAGC_OUT_CTL, CAGC_TX_AGC_CTL_M, CAGC_TX_AGC_CTL_CPU_V );

	// get AGC value for 23.8dBm
	//	AGC value는 rf_nv_dig_items_ptr->rfnv_cdma_tx_lin_master_off[ 2 ]
	// AGC value 보정 약 24.2dBm ( + 0.4dBm )
	//	5.6 dBm 변할때 AGC 값은 약 0x17변함 (0.4dBm은 약1.6)
	//	0.4 dBm을 크게하기 위해 rfnv_cdma_tx_lin_master_off[ 2 ] - 2

	// set TX_AGC_ADJ_WR
	// jwpark 00.09.14
	rf_Set_TX_AGC_ADJ (
		(int2)(rf_nv_dig_items_ptr->rfnv_cdma_tx_lin_master_off[0][2] - 2));
}

byte	rf_GetCurrentFreqIndex ( void )
{
	return ( rf_freq_index );
}

int1	rf_GetTxMaxPowerLimit ( byte nFreqIndex )
{
	return ( rf_nv_dig_items_ptr->rfnv_cdma_tx_lim_vs_freq[ nFreqIndex ] );
}

void	rf_SetTxMaxPowerLimit ( byte nFreqIndex, int1 nTxLimitVsFreq )
{
	// set current channel max power limit value
	rf_nv_dig_items_ptr->rfnv_cdma_tx_lim_vs_freq[ nFreqIndex ] =
		nTxLimitVsFreq;
}

#endif	//#ifdef	FEATURE_SKTT_NEW_CAL
