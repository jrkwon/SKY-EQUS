
/*****************************************************************************
 *
 *		RF calibration support functions for SKTT project
 *
 *		Copyright (C) 1999, 2000 SK Telecom
 *
 *		'DOOLY' project 
 *
 *****************************************************************************
 *
 *		FileName : NewCal.h
 *		Revision : 0.00.001
 *		 Created : 00.08.08		by chos
 *
 *****************************************************************************
 *
 *		Revision detail
 *		---------------
 *
 *		Rev	0.00.001	00.08.08 09:00	chos
 *			created this module
 *
 *****************************************************************************/


#define	RF_CALIBRATION_INITIAL_CHANNEL			400

void	rf_DigitalTxOn ( void );
void	rf_DigitalTxOff ( void );
void	rf_Set_TX_AGC_ADJ ( int2 nValue );
void	rf_TxAGCManualMode ( void );

byte	rf_GetCurrentFreqIndex ( void );
int1	rf_GetTxMaxPowerLimit ( byte nFreqIndex );
void	rf_SetTxMaxPowerLimit ( byte nFreqIndex, int1 nTxLimitVsFreq );
