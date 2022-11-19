/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     
**
** Description: 
**     <@Desciption about this file here>
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-07-21   woodstok   Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

#include <stdio.h>
#include "uistate.h"
#include "eqs.h"
#include "eqa.h"

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
#include "eqs_vari.h"
#endif
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
BYTE	GetRssiValue(UINT16 val  ,BOOL blink)
{
	BYTE i;

	/*
	** RSSI thresholds arry for mapping real RSS values into bar levels 1
	** through 5, which corresponds to the RSSI bars that are displayed on
	** the screen. Note that the two first values (index 0, 1) are never used,
	** so they are DC. The threshold value in index 2 corresponds to 1 RSSI
	** bars, the threshold value in index 3 corresponds to 2 RSSI bars and
	** so on.
	*/
	static BYTE rssi_threshold_val[6] = { 0, 0, 105, 99, 96, 91 };  // -97 -92 -87 -82
	//  static byte rssi_threshold_val[6] = { 0, 0, 103, 99, 96, 95 };    // -96dBm -92dBm -86dBm -83dbm 

	/* Send it to the handset task after converting it into RSSI segments.*/

	// Digital Only
	if ( val != 0 )	{
		/* map real RSS value into RSSI bar level 1 through 5 */
		for( i = 5; i > 1; i-- )
		{
			if( val < rssi_threshold_val[i] )
				break;
		}
		
		return i;
	}

	return 0;
}

UINT16	UpdateRSSI(void)
{
	static db_items_value_type dbi; /* DB items */
    BOOL fCDMA;

	db_get( DB_NO_SVC, &dbi );  /* Read No Service status */
	if( dbi.no_svc ) 
	{
		return 0;		// HS_AN_NOSVC
	} 
	else 
	{     

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
        /* dbi.no_svc == FALSE */
        EQS_GetUi( ES_CM_CDMA_I, (void*)&fCDMA, TRUE);
		if( fCDMA )       
#else  /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
        /* dbi.no_svc == FALSE */
		if( ui.digital )     
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
		{
			/* update the CDMA RSSI */
			db_get( DB_CDMA_RSSI, &dbi );
			return dbi.cdma_rssi;
		}
		else 
		{
			/* update the Analog RSSI */    
			db_get( DB_RSSI, &dbi );
			if ( dbi.rssi == 0 ) 
			{
				return 1;
			}
			else 
			{
				return dbi.rssi;
			}
		}
	}

}
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

VOID EQA_RSSITop(EQS_EVENT *pInEvent)
{
	UINT16		iRssi_level=0;
	BYTE		iRssi_value=0;
	CHAR		szTmp[5];
	static HCONTROL hHnd;

#if ( defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) )
	static db_items_value_type dbi; /* DB items */
	boolean cdma_cellular = FALSE;
#endif

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
		EQC_StaticText("RSSI Value", 0, 0, FT_MED, STEXT_DEFAULTS);
		hHnd = NULL;

		// Timer Start (RSSI update timer )
		EQS_StartTimer(EQS_TIMER_1, 1000, TRUE);

		EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
		break;

	case EQE_TM_GENERAL:
		iRssi_level = UpdateRSSI();
		iRssi_value = GetRssiValue( (byte)iRssi_level, FALSE );

		sprintf((CHAR*)szTmp, "%d", iRssi_value);
		if(hHnd == NULL)
			hHnd = EQC_StaticText(szTmp, 10, 20, FT_MED, STEXT_DEFAULTS);
		else
			EQC_SetSText(hHnd, szTmp);
//		EQS_TextOut(10, 20, (BYTE*)szTmp);
//		EQS_UpdateDisplay();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
