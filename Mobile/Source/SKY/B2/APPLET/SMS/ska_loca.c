/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_loca.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-25 reo     Created.
**
** ===========================================================================
*/

//#define TEL_NUM_POSITION 12
//#define TEL_NUM_LENGTH   10
/*
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

#include "ska.h"

#ifndef WIN32
// for 위치정보 추적 
#ifdef FEATURE_SKTT_EQUS_LOCATION
#include "caii.h"
#include "srchi.h"
#include "mccdma.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define TEL_NUM_POSITION	12
#define TEL_NUM_LENGTH		10
#define PERIOD_LEN			6
		
#define	ITOA10(i)		(((i) < 0x0A) ? '0' + (i) : '0')

#define PTIMEC(i)		(((i) >= 13 ) ? (i) - 12  : i)

#define LOCATION_INFORM_TO_NUM          "1571809000"
#define LOCATION_ON                     1
#define LOCATION_OFF                    2
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagLOCATION_VIEW_E {  //SEM 
    LV_STATIC_TEXT_1,
    LV_TEXT_EDIT_1,        
    LV_STATIC_TEXT_2,	// 위치 확인자 id
    LV_TEXT_EDIT_2,
    LV_STATIC_TEXT_3,	// 위치 통보 start time
	LV_TEXT_EDIT_3,
	LV_STATIC_TEXT_4,	// 위치 통보 end time
	LV_TEXT_EDIT_4,
	LV_STATIC_TEXT_5,	// 위치 통보 주기 
	LV_TEXT_EDIT_5,  
    LV_MAX
};
/*
**----------------------------------------------------------------------------
**  General Declarations
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
LOCAL UINT8	l_szConfirmID[TEL_NUM_LENGTH + 1] = "0000000000";
LOCAL CHAR	l_szStartTime[LCD_COLUME_SIZE];
LOCAL CHAR	l_szEndTime[LCD_COLUME_SIZE];
LOCAL CHAR	l_szPeriod[PERIOD_LEN];

extern srch_sect_struct_type  srch_aset[];    /* Active set */
extern srch_sect_struct_type  srch_cset[];    /* Candidate set */  
extern srch_sect_struct_type  srch_nset[];    /* Neighbor set */
extern UINT16  srch_aset_cnt;    /* Active set size */
extern UINT16  srch_cset_cnt;    /* Candidate set size */
extern UINT16  srch_nset_cnt;    /* Neighbor set size */
extern UINT16  srch_ref_inx;
extern UINT16 *srch_ANCR_sets_ecio_tab_ptr;
extern BYTE  srch_eng_to_ecio(UINT16 eng, const UINT16 ecio_tab[]);


VOID SaveLocationReport(LOCATION_INFO_T* pGetLocation)
{
	caii_sysparm_type *cap_ptr;

	SKY_TRACE(TL_HIGH, "SaveLocationReport");

	if ((cap_ptr = (caii_sysparm_type *) mcc_get_msg( CAI_SYS_PARM_MSG )) != NULL)
	{
		pGetLocation->lastsid = cap_ptr->sid;
		pGetLocation->lastnid = cap_ptr->nid;
		pGetLocation->lastzone = cap_ptr->reg_zone;
		pGetLocation->lastbaseid = cap_ptr->base_id;
		pGetLocation->lastpn = (UINT16)srch_aset[srch_ref_inx].pilot;
		pGetLocation->lastbaselat = cap_ptr->base_lat;
		pGetLocation->lastbaselong = cap_ptr->base_long;
		pGetLocation->reportwait = 0;
	}
	
	SKY_SET_LOCATION_INFO(*pGetLocation);
}

BYTE ConvertDtmf(BYTE ch)
{
	BYTE temp = ch;

	if( 0 == (temp & 0xF0) )
		temp |= 0xA0;

	if( 0 == (temp & 0x0F ) )
		temp |= 0x0A;

	return(temp);
}

VOID RptLocationData(LOCATION_INFO_T* pGetLocation)
{
	BYTE msg_type, ver, temp, i, j, swap_inx;
	BYTE cfrmid[5];
	
	UINT16 max_pilot[20];
	UINT16 temp_pilot;
	BYTE max_ecio[20];
	BYTE min_ecio;
	BYTE aset_ecio;
	BYTE temp_ecio;
	UINT16 max_pos[20];
	UINT16 temp_pos;
	UINT16 pn_phase;

	clk_julian_type	tm;
	DWORD			dwGetTime;
	caii_sysparm_type *cap_ptr;

	SMSMO_SEND_T*	pMsg;

	UCHAR ucNumber[TEL_NUM_POSITION] = {NULL, };

	SKY_TRACE(TL_HIGH,"RptLocationData ");

	pMsg = &g_UIMOBuffer;
    memset((SMSMO_SEND_T*)pMsg, NULL, sizeof(SMSMO_SEND_T));

	for(i = 0; i < srch_nset_cnt; i++)
	{
		max_ecio[i] = srch_eng_to_ecio ( srch_nset[i].trpt.eng, srch_ANCR_sets_ecio_tab_ptr );
		max_pilot[i] = (UINT16)srch_nset[i].pilot;
		max_pos[i] = srch_nset[i].trpt.pos;
//		MSG_HIGH("max_ecio[%d] = %d pn = %d",i,max_ecio[i],max_pilot[i]);
	}

	for(i=0; i<srch_nset_cnt-1; i++)
	{
		min_ecio = max_ecio[i];
		swap_inx = i;
		for(j=i+1; j< srch_nset_cnt;j++)
		{
			if(max_ecio[j] < min_ecio)
			{
				min_ecio = max_ecio[j];
				swap_inx = j;
			}
		}		

		if(swap_inx != i ) {
			temp_ecio = max_ecio[i];
			max_ecio[i] = max_ecio[swap_inx];
			max_ecio[swap_inx] = temp_ecio;

			temp_pilot = max_pilot[i];
			max_pilot[i] = max_pilot[swap_inx];
			max_pilot[swap_inx] = temp_pilot;
						
			temp_pos = max_pos[i];
			max_pos[i] = max_pos[swap_inx];
			max_pos[swap_inx] = temp_pos;
		}
	}

	/* time get */	
    if((dwGetTime = SKY_GetLocalTime())!= SKY_INVALID_SYS_TIME)
    {
		clk_secs_to_julian (dwGetTime, &tm);
		pMsg->dwMadeTime = dwGetTime;
	}

	pMsg->nMsgSize = (UINT8)0;

	msg_type = LOCATION_INFORM;
	pMsg->pszMsg[pMsg->nMsgSize++] = msg_type;
	pMsg->pszMsg[pMsg->nMsgSize++] = 0x03;
	
//	(void) ui_get_nv( NV_LOCATION_INFO_I, &nvi );
	ver = pGetLocation->lam3ver;
	
	for (i = 0; i < 5; i++)
		cfrmid[i] =  pGetLocation->cfrmid[i];

	pMsg->pszMsg[pMsg->nMsgSize++] = ver;

	temp = ( ((tm.month / 10) << 4) | ((tm.month % 10) & 0x0f) );
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;

	temp = ( ((tm.day / 10) << 4) | ((tm.day % 10) & 0x0f) );;
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;

	temp = ( ((tm.hour / 10) << 4) | ((tm.hour % 10) & 0x0f) );;
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	SKY_TRACE1(TL_HIGH, "hour = %x",pMsg->pszMsg[pMsg->nMsgSize]);
	pMsg->nMsgSize++;

	temp = ( ((tm.minute / 10) << 4) | ((tm.minute % 10) & 0x0f) );;
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;

	for (i = 0; i < 5; i++) 
	{
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(cfrmid[i]);
		pMsg->nMsgSize++;
	}

    STRCPY(ucNumber, GetMyNumber());
	if(STRLEN(ucNumber) == TEL_NUM_LENGTH+1)
    {
        for( i = 0; i < TEL_NUM_LENGTH+1; i++ )
        {
            ucNumber[i] = ucNumber[i+1];
        }
        ucNumber[TEL_NUM_LENGTH+1] = NULL;
    }

	for (i = 0; i < 10; ) 
	{
		temp = (((char)ucNumber[i] << 4) | ((char)ucNumber[i+1] & 0x0f));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
		i += 2;
	}

	if ((cap_ptr = (caii_sysparm_type *) mcc_get_msg( CAI_SYS_PARM_MSG )) != NULL)
	{
		temp = (cap_ptr->sid / 10000);
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp =  ((((cap_ptr->sid % 10000) / 1000) << 4) | ((cap_ptr->sid % 1000) / 100));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;	

		temp = cap_ptr->sid % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
		
		temp = (cap_ptr->nid / 10000);
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp =  (BYTE)((((cap_ptr->nid % 10000) / 1000) << 4 ) | ((cap_ptr->nid % 1000) / 100));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp = cap_ptr->nid % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp =  (BYTE)(((cap_ptr->reg_zone / 1000) << 4 ) | ((cap_ptr->reg_zone % 1000) / 100) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp = cap_ptr->reg_zone % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp = (cap_ptr->base_id / 10000);
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp =  ((((cap_ptr->base_id % 10000) / 1000) << 4) | ((cap_ptr->base_id % 1000) / 100));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;	

		temp = cap_ptr->base_id % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp =  (BYTE)(((srch_aset[srch_ref_inx].pilot / 1000) << 4 ) | ((srch_aset[srch_ref_inx].pilot % 1000) / 100) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp = srch_aset[srch_ref_inx].pilot % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp =  (BYTE)(((cap_ptr->base_lat / 10000000) << 4 ) | ((cap_ptr->base_lat % 10000000) / 1000000) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp =  (BYTE)((((cap_ptr->base_lat % 1000000) / 100000) << 4 ) | ((cap_ptr->base_lat % 100000) / 10000) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp =  (BYTE)((((cap_ptr->base_lat % 10000)/ 1000) << 4 ) | ((cap_ptr->base_lat % 1000) / 100) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp = cap_ptr->base_lat % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp =  (BYTE)(((cap_ptr->base_long / 10000000) << 4 ) | ((cap_ptr->base_long % 10000000) / 1000000) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp =  (BYTE)((((cap_ptr->base_long % 1000000) / 100000) << 4 ) | ((cap_ptr->base_long % 100000) / 10000) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp =  (BYTE)((((cap_ptr->base_long % 10000)/ 1000) << 4 ) | ((cap_ptr->base_long % 1000) / 100) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	
		temp = cap_ptr->base_long % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	}

	aset_ecio = srch_eng_to_ecio( srch_aset[srch_ref_inx].trpt.eng, srch_ANCR_sets_ecio_tab_ptr );
	temp = (BYTE)((((BYTE)aset_ecio / 10) << 4 ) | ((BYTE)aset_ecio % 10) );
	SKY_TRACE1(TL_HIGH,"ref ec/io = %d",temp);
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;								// REF PN Ec/Io

	for(i=0; i<8; i++)
	{
		if(max_pos[i] == 0)
		{
			pn_phase = (UINT16)max_pilot[i] * 64;
		}
		else
		{
			pn_phase = (UINT16)max_pilot[i] * 64 
				+(UINT16)((max_pos[i] - srch_aset[srch_ref_inx].trpt.pos + 4) / 8);

			MSG_HIGH("PN phase = %ld", pn_phase,0,0);
		}

		temp = (pn_phase / 10000);
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp =  ((((pn_phase % 10000) / 1000) << 4) | ((pn_phase % 1000) / 100));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;	

		temp = pn_phase % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp = (BYTE)((((BYTE)max_ecio[i] / 10) << 4 ) | ((BYTE)max_ecio[i] % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	}

	SKY_TRACE1(TL_HIGH, "pMsg->nMsgSize = %d",pMsg->nMsgSize);
	/* fill the buffer for SMS */

    STRCPY(pMsg->pszRcvNumber,(BYTE *)LOCATION_INFORM_TO_NUM);
    pMsg->nRcvNumberSize = STRLEN((BYTE *)LOCATION_INFORM_TO_NUM);

//	EQS_START_CHILD(SKA_LocationSendMsg);
}


VOID ReportCfrmOff(LOCATION_INFO_T* pGetLocation)
{
	SMSMO_SEND_T* pMsg;
	BYTE msg_type, temp, i;
	UCHAR	ucNumber[TEL_NUM_POSITION] = {NULL, };

	pMsg = &g_UIMOBuffer;
	/* 초기화 */
	memset((SMSMO_SEND_T*)pMsg, NULL, sizeof(SMSMO_SEND_T));
	
	pMsg->nMsgSize = (BYTE)0;

	msg_type = LOCATION_INFORM_OFF;
	pMsg->pszMsg[pMsg->nMsgSize++] = msg_type;

	STRCPY(ucNumber, GetMyNumber());
	if(STRLEN(ucNumber) == TEL_NUM_LENGTH+1)
    {
        for( i = 0; i < TEL_NUM_LENGTH+1; i++ )
        {
            ucNumber[i] = ucNumber[i+1];
        }
        ucNumber[TEL_NUM_LENGTH+1] = NULL;
    }

	for (i = 0; i < 10; ) {
		temp = (((char)ucNumber[i] << 4) | ((char)ucNumber[i+1] & 0x0f));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		SKY_TRACE1(TL_HIGH, "number = %x", ConvertDtmf(temp));
		pMsg->nMsgSize++;
		i += 2;
	}

    STRCPY(pMsg->pszRcvNumber,(BYTE *)LOCATION_INFORM_TO_NUM);
    pMsg->nRcvNumberSize = STRLEN((BYTE *)LOCATION_INFORM_TO_NUM);

    SKY_TRACE(TL_HIGH, "ReportCfrmOff");

//	EQS_START_CHILD(SKA_LocationSendMsg);
}

VOID SaveTrafficLocationData(LOCATION_INFO_T* pGetLocation)
{
	SMSMO_SEND_T*	pMsg;
	BYTE            msg_type, ver, temp, i, j, swap_inx;
	BYTE            cfrmid[5];

	DWORD           dwGetTime;
	clk_julian_type tm;
	UINT16          max_pilot[35];
	UINT16          temp_pilot;
	BYTE            max_ecio[35];
	BYTE            min_ecio;
	BYTE            aset_ecio;
	BYTE            temp_ecio;
	UINT16          max_pos[35];
	UINT16          temp_pos;
	UINT16          pn_phase;

	UCHAR	ucNumber[TEL_NUM_POSITION] = {NULL, };
	pMsg = &g_UIMOBuffer;		

	SKY_TRACE(TL_HIGH,"SaveTrafficLocationData ");	
	memset((SMSMO_SEND_T*)pMsg, NULL, sizeof(SMSMO_SEND_T));


	for(i=0; i<srch_aset_cnt; i++)
	{
		if((UINT16)srch_aset[srch_ref_inx].pilot != (UINT16)srch_aset[i].pilot) {
			max_ecio[i] = srch_eng_to_ecio ( srch_aset[i].trpt.eng, srch_ANCR_sets_ecio_tab_ptr );
			max_pilot[i] = (UINT16)srch_aset[i].pilot;
			max_pos[i] = srch_aset[i].trpt.pos;
		}
	}

	for(i=srch_aset_cnt-1; i<(srch_aset_cnt-1) + srch_cset_cnt; i++)
	{
		max_ecio[i] = srch_eng_to_ecio ( srch_cset[i].trpt.eng, srch_ANCR_sets_ecio_tab_ptr );
		max_pilot[i] = (UINT16)srch_cset[i].pilot;
		max_pos[i] = srch_cset[i].trpt.pos;
	}

	for(i=(srch_aset_cnt+srch_cset_cnt -1); i<(srch_aset_cnt+srch_cset_cnt -1) + srch_nset_cnt; i++)
	{
		max_ecio[i] = srch_eng_to_ecio ( srch_nset[i].trpt.eng, srch_ANCR_sets_ecio_tab_ptr );
		max_pilot[i] = (UINT16)srch_nset[i].pilot;
		max_pos[i] = srch_nset[i].trpt.pos;
	}

	for(i=0; i<srch_aset_cnt+srch_cset_cnt+srch_nset_cnt-2; i++)
	{
		min_ecio = max_ecio[i];
		swap_inx = i;

		for(j=i+1; j< srch_nset_cnt;j++)
		{
			if(max_ecio[j] < min_ecio)
			{
				min_ecio = max_ecio[j];
				swap_inx = j;
			}
		}		

		if(swap_inx != i ) {
			temp_ecio = max_ecio[i];
			max_ecio[i] = max_ecio[swap_inx];
			max_ecio[swap_inx] = temp_ecio;

			temp_pilot = max_pilot[i];
			max_pilot[i] = max_pilot[swap_inx];
			max_pilot[swap_inx] = temp_pilot;
						
			temp_pos = max_pos[i];
			max_pos[i] = max_pos[swap_inx];
			max_pos[swap_inx] = temp_pos;
		}
	}

	if ((dwGetTime =  SKY_GetLocalTime())!= SKY_INVALID_SYS_TIME) {
		clk_secs_to_julian (dwGetTime, &tm);
		pMsg->dwMadeTime = dwGetTime ; 
	}
	
	pMsg->nMsgSize = (BYTE)0;

	msg_type = LOCATION_INFORM_IN_CALL;  // 통화중 알림
	pMsg->pszMsg[pMsg->nMsgSize++] = msg_type;  // TYPE
	pMsg->pszMsg[pMsg->nMsgSize++] = 0x03;  // LAM_NUM

	ver = pGetLocation->lam3ver;
	
	for (i = 0; i < 5; i++)
		cfrmid[i] =  pGetLocation->cfrmid[i];

	pMsg->pszMsg[pMsg->nMsgSize++] = ver;   // VER

	// 위치확인자 ID
	for (i = 0; i < 5; i++) {
		temp = cfrmid[i];
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;
	}

	STRCPY(ucNumber, GetMyNumber());
	if(STRLEN(ucNumber) == TEL_NUM_LENGTH+1)
    {
        for( i = 0; i < TEL_NUM_LENGTH+1; i++ )
        {
            ucNumber[i] = ucNumber[i+1];
        }
        ucNumber[TEL_NUM_LENGTH+1] = NULL;
    }

	for (i = 0; i < 10; ) {
		temp = (((char)ucNumber[i] << 4) | ((char)ucNumber[i+1] & 0x0f));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
//		MSG_HIGH("number = %x", ConvertDtmf(temp),0,0);
		pMsg->nMsgSize++;
		i += 2;
	}

	temp = ( ((tm.month / 10) << 4) | ((tm.month % 10) & 0x0f) );
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;

	temp = ( ((tm.day / 10) << 4) | ((tm.day % 10) & 0x0f) );
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;                                            // DATE

	temp = ( ((tm.hour / 10) << 4) | ((tm.hour % 10) & 0x0f) );
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;

	temp = ( ((tm.minute / 10) << 4) | ((tm.minute % 10) & 0x0f) );
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;											// TIME
	
	temp = ((srch_aset[srch_ref_inx].pilot / 1000) << 4) | (((srch_aset[srch_ref_inx].pilot % 1000) / 100) & 0x0f);
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;

	temp = srch_aset[srch_ref_inx].pilot % 100;
	temp =  (BYTE)( ( (temp / 10) << 4 ) | (temp % 10) );
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;											// REF_PN

	aset_ecio = srch_eng_to_ecio( srch_aset[srch_ref_inx].trpt.eng, srch_ANCR_sets_ecio_tab_ptr );
	temp = (BYTE)((((BYTE)aset_ecio / 10) << 4 ) | ((BYTE)aset_ecio % 10) );
	SKY_TRACE1(TL_HIGH, "ref ec/io = %d",temp);
	pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
	pMsg->nMsgSize++;								// REF PN Ec/Io

	for(i=0; i<8; i++)
	{
		if(max_pos[i] == 0)
		{
			pn_phase = (UINT16)max_pilot[i] * 64;
		}
		else
		{
			pn_phase = (UINT16)max_pilot[i] * 64 
				+(UINT16)((max_pos[i] - srch_aset[srch_ref_inx].trpt.pos + 4) / 8);
			
			SKY_TRACE2(TL_HIGH, "PN phase = %ld Ec/Io = %d", pn_phase,max_ecio[i]);
		}
	
		temp = (pn_phase / 10000);
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp =  ((((pn_phase % 10000) / 1000) << 4) | ((pn_phase % 1000) / 100));
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;	

		temp = pn_phase % 100;
		temp =  (BYTE)(((temp / 10) << 4 ) | (temp % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;

		temp = (BYTE)((((BYTE)max_ecio[i] / 10) << 4 ) | ((BYTE)max_ecio[i] % 10) );
		pMsg->pszMsg[pMsg->nMsgSize] = ConvertDtmf(temp);
		pMsg->nMsgSize++;	

	}

    STRCPY(pMsg->pszRcvNumber,(BYTE *)LOCATION_INFORM_TO_NUM);
    pMsg->nRcvNumberSize = STRLEN((BYTE *)LOCATION_INFORM_TO_NUM);
//	ui_ShortMessageOrigination((SMS_MEM*)&pMsg, (char*)&DestAddr[0], TI_LOCATION_SMS, 	AfterTraffic);
//	EQS_START_CHILD(SKA_LocationSendMsg);
}

BYTE CompLocation(LOCATION_INFO_T* pGetLocation)
{			
	caii_sysparm_type *cap_ptr;
	
//	(void) ui_get_nv( NV_LOCATION_INFO_I, &nvi );	
	if ((cap_ptr = (caii_sysparm_type *) mcc_get_msg( CAI_SYS_PARM_MSG )) != NULL)
	{
		if( ( cap_ptr->sid != pGetLocation->lastsid )          ||
			( cap_ptr->nid != pGetLocation->lastnid )          ||
			( cap_ptr->reg_zone != pGetLocation->lastzone )    ||
			( cap_ptr->base_id != pGetLocation->lastbaseid )   ||
			( cap_ptr->base_lat != pGetLocation->lastbaselat ) ||
			( cap_ptr->base_long != pGetLocation->lastbaselong ) ) {
				return(0x01);
			}
		else 
		{
			SKY_TRACE(TL_HIGH,"Location not chg");
			return(0xff);
		}
	}
    else return(0x00);
}

BYTE CheckCurrentTime(VOID)
{
	int 	                CurTime,S_Time, E_Time;
	int                     temp;
	BYTE                    ret_state;
	DWORD                   dwGetTime;
	clk_julian_type	        tm;
	STATIC LOCATION_INFO_T*	s_pLocation;   //why static?
	
	ret_state = 0;


	s_pLocation = &SKY_GET_LOCATION_INFO();
	SKY_TRACE1(TL_HIGH, "on = %ld ", s_pLocation->onoffstate);

	if(s_pLocation->onoffstate)
	{
		if((dwGetTime = SKY_GetLocalTime())!= SKY_INVALID_SYS_TIME)
		{
			clk_secs_to_julian (dwGetTime, &tm);

			CurTime = tm.hour * 60 + tm.minute;
			S_Time = s_pLocation->starttime * 60 + s_pLocation->startmin;
			E_Time = s_pLocation->endtime * 60 + s_pLocation->endmin;

			if( ( CurTime >= S_Time ) && ( CurTime <= E_Time ) ) {

				SKY_TRACE1(TL_HIGH, "MAIN_STATE = %lx", cdma.curr_state);

				switch(MAIN_STATE( cdma.curr_state )) 
				{
					case CDMA_IDLE:
						temp = CompLocation(s_pLocation);
						if( (temp == 0x01) || (s_pLocation->reportwait) )
						{
							ret_state = 0x01;
							RptLocationData(s_pLocation);
                            s_pLocation->reportwait = 0;
                            SKY_SET_LOCATION_INFO(*s_pLocation);
                            EQS_START_CHILD(SKA_LocationSendMsg);
						}
						break;
					case CDMA_TC:
		//					(void) ui_get_nv( NV_LOCATION_INFO_I, &nvi );
						if( (UINT16)srch_aset[srch_ref_inx].pilot != (UINT16)s_pLocation->lastpn )					
						{
							ret_state = 0x01;
							SaveTrafficLocationData(s_pLocation);
                            EQS_START_CHILD(SKA_LocationSendMsg);
						}
						break;
					default:
						temp = CompLocation(s_pLocation);
						if( temp == 0x01 )
						{
							s_pLocation->reportwait = 0x01;
		//						memcpy( (BYTE *)&nvi.location_info, (BYTE *)&ui.location_info, sizeof( nv_location_info_type ));
		//						(void) ui_put_nv( NV_LOCATION_INFO_I, &nvi );
							SKY_SET_LOCATION_INFO(*s_pLocation);
						}
						break;
				}  /* end-switch */
			}
		} // valid system time
	}  /* end-if( location.onoffstate ) */

	return(ret_state);
}

VOID sFormatLocationInfo(LOCATION_INFO_T* pGetLocation)
{
	INT					i, j;
	BYTE				unHour;
	BOOL				fAM;

    unHour = 0;
	/* 위치 확인자 ID  : 수정 할 필요 있음 */
	for(j=0, i=0; i<10; i+=2, j++)
	{
		l_szConfirmID[i] = ITOA10(pGetLocation->cfrmid[j] >> 4);
		l_szConfirmID[i+1] = ITOA10(pGetLocation->cfrmid[j] & 0x0F);
	}
	/* 위치 통보 Start Time */
    if(pGetLocation->starttime < 12)
    {
        fAM = TRUE;
        unHour = pGetLocation->starttime;
    }
    else
    {
        fAM = FALSE;
        if(pGetLocation->starttime == 24)
            unHour = 0;
        else
            unHour = pGetLocation->starttime - 12;
    }

	memset(l_szStartTime, NULL, LCD_COLUME_SIZE); //오전/오후 00시 00분
	sprintf((CHAR*)l_szStartTime, (CHAR*)SKY_GET_TOKEN(TKN_LOCATION_FMT_TIME), 
		                   (fAM)? (BYTE *)SKY_GET_TOKEN(TKN_DAT_AM) : (BYTE *)SKY_GET_TOKEN(TKN_DAT_PM),
		                   unHour, pGetLocation->startmin);
	
	/* 위치 통보 End Time */
    if(pGetLocation->endtime < 12)
    {
        fAM = TRUE;
        unHour = pGetLocation->endtime;
    }
    else
    {
        fAM = FALSE;
        if(pGetLocation->endtime == 24)
            unHour = 0;
        else
            unHour = pGetLocation->endtime - 12;
    }

	memset(l_szEndTime, NULL, LCD_COLUME_SIZE); //오전/오후 00시 00분 
	sprintf((CHAR*)l_szEndTime, (CHAR*)SKY_GET_TOKEN(TKN_LOCATION_FMT_TIME), 
		                   (fAM)? (BYTE *)SKY_GET_TOKEN(TKN_DAT_AM) : (BYTE *)SKY_GET_TOKEN(TKN_DAT_PM),
		                   unHour, pGetLocation->endmin);

	/* Period */
	memset(l_szPeriod, NULL, PERIOD_LEN);
	sprintf((CHAR*)l_szPeriod, 
		(CHAR*)SKY_GET_TOKEN(TKN_LOCATION_FMT_PERIOD), pGetLocation->cycle);

}

VOID SKY_API SKA_LocationView(SKY_EVENT_T *pEvent)
{
	DWORD		                dwCurTime;
	UINT16		                unStart, unEnd, unCurrent;
	clk_julian_type		        tm;
    UINT8                       nCtrl;
    HCONTROL                    hLocationOFF, hLocationON;
    LOCATION_INFO_T*            pLocation;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_DISSOLVE_NONE,//CML_NONE, 
                      SKY_GET_TOKEN(TKN_LOCATION_MENU_VIEW), 
                      WA_SHOW_HEADER | WA_SHOW_CML );

       	pLocation = &SKY_GET_LOCATION_INFO();

		sFormatLocationInfo(pLocation);
		
        EQC_StaticText(EQS_NA, EQS_NA, TKN_LOCATION);
        if(pLocation->onoffstate)
        {            
            EQC_TextEdit(EQS_NA, EQS_NA, PERIOD_LEN, 1, TEM_KOREAN, PERIOD_LEN, 
                    (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_ON), 0);
            EQS_SetWindowCML(CL_NONE_DISSOLVE_NONE);
        }
        else
        {
            EQC_TextEdit(EQS_NA, EQS_NA, PERIOD_LEN, 1, TEM_KOREAN, PERIOD_LEN,
                    (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_OFF), 0);
            EQS_SetWindowCML(CL_NONE_SETUP_NONE);
        }

		EQC_StaticText(EQS_NA, EQS_NA, TKN_LOCATION_VIEW_FINDER_ID);
		EQC_TextEdit(EQS_NA, EQS_NA, LCD_COLUME_SIZE, 1, TEM_KOREAN, LCD_COLUME_SIZE,
					(BYTE *)l_szConfirmID, 0);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_LOCATION_VIEW_START_TIME);
		EQC_TextEdit(EQS_NA, EQS_NA, LCD_COLUME_SIZE, 1, TEM_KOREAN, LCD_COLUME_SIZE,
					(BYTE *)l_szStartTime, 0);


		EQC_StaticText(EQS_NA, EQS_NA, TKN_LOCATION_VIEW_END_TIME);
		EQC_TextEdit(EQS_NA, EQS_NA, LCD_COLUME_SIZE, 1, TEM_KOREAN, LCD_COLUME_SIZE,
					(BYTE *)l_szEndTime, 0);


		EQC_StaticText(EQS_NA, EQS_NA, TKN_LOCATION_VIEW_PERIOD);
		EQC_TextEdit(EQS_NA, EQS_NA, LCD_COLUME_SIZE, 1, TEM_KOREAN, LCD_COLUME_SIZE,
					(BYTE *)l_szPeriod, 0);
		        // Screen Formatting

        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < LV_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            if( (nCtrl == LV_TEXT_EDIT_1) ||
				(nCtrl == LV_TEXT_EDIT_2) ||
				(nCtrl == LV_TEXT_EDIT_3) ||
				(nCtrl == LV_TEXT_EDIT_4) ||
                (nCtrl == LV_TEXT_EDIT_5) )
			{
                EQC_SET_FOCUS(EQS_GetNthIOC(nCtrl), FALSE);
			}

            if(nCtrl != LV_STATIC_TEXT_1)
            {
                EQS_NewLine();
            }
        }

        EQS_EndFormatter();

        EQC_SET_STYLE(EQS_GetNthIOC(LV_TEXT_EDIT_1), 
                    CS_TEXT_EDIT_RIGHT_ALIGN | CS_TEXT_EDIT_READONLY |  CS_TEXT_EDIT_DEFAULT);

        EQC_SET_STYLE(EQS_GetNthIOC(LV_TEXT_EDIT_2), 
                    CS_TEXT_EDIT_RIGHT_ALIGN | CS_TEXT_EDIT_READONLY |  CS_TEXT_EDIT_DEFAULT);

        EQC_SET_STYLE(EQS_GetNthIOC(LV_TEXT_EDIT_3), 
                    CS_TEXT_EDIT_RIGHT_ALIGN | CS_TEXT_EDIT_READONLY |  CS_TEXT_EDIT_DEFAULT);

        EQC_SET_STYLE(EQS_GetNthIOC(LV_TEXT_EDIT_4), 
                    CS_TEXT_EDIT_RIGHT_ALIGN | CS_TEXT_EDIT_READONLY |  CS_TEXT_EDIT_DEFAULT);
        
        EQC_SET_STYLE(EQS_GetNthIOC(LV_TEXT_EDIT_5), 
                    CS_TEXT_EDIT_RIGHT_ALIGN | CS_TEXT_EDIT_READONLY |  CS_TEXT_EDIT_DEFAULT);

        EQS_FitCanvasSize();

        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
//        EQS_SEND_EVENT(SE_APP_START, EQS_NA, EQS_NA);
        if( EQS_GET_CHILD() == SKA_LocationNotif )         //2001.06.27추가    
        {
            EQS_END();
            break;
        }

        if (WhatLocationType() == LOCATION_DELETE_SETTING)
        {
            EQS_END();
        }
        else
        {
       	    pLocation = &SKY_GET_LOCATION_INFO();

            if( (pEvent->dwParam == SMS_RESULT_SUCCESS) 
                &&(EQS_GET_CHILD() == SKA_LocationSendMsg) )
            {
                if(pLocation->onoffstate == TRUE)
                {
                    // Timer ON
		            SKY_SetLocationTimer ((int4)(pLocation->cycle * 60 * 1000), TRUE);

		            hLocationON = EQC_MessageBox(
				            (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_ON_MSG),
				            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);


                    // 변경 내용 update
                    EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_1),
                                        (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_ON) );
                    EQS_SetWindowCML(CL_NONE_DISSOLVE_NONE);     

                }
                else
                {
		            SKY_SetLocationTimer((int4)0, TRUE);

		            hLocationOFF = EQC_MessageBox(
				            (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_OFF_MSG),
				            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

                    // 변경 내용 update
                    EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_1),
                                        (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_OFF) );
                    EQS_SetWindowCML(CL_NONE_SETUP_NONE); 
                }
            }
            else if( (pEvent->dwParam == SMS_RESULT_FAIL)
                        &&(EQS_GET_CHILD() == SKA_LocationSendMsg) )
            {
                if(pLocation->onoffstate == TRUE)
                {
                    pLocation->onoffstate = FALSE;
                    SKY_SET_LOCATION_INFO(*pLocation);
                }
                else
                {
                    pLocation->onoffstate = TRUE;
                    SKY_SET_LOCATION_INFO(*pLocation);
                }          
            }

		    sFormatLocationInfo(pLocation);
            if(pLocation->onoffstate)
            {            
                EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_1), 
                        (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_ON));
                EQS_SetWindowCML(CL_NONE_DISSOLVE_NONE);
            }
            else
            {
                EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_1),
                        (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_OFF));
                EQS_SetWindowCML(CL_NONE_SETUP_NONE);
            }

		    EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_2), (BYTE *)l_szConfirmID);

		    EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_3), (BYTE *)l_szStartTime);

		    EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_4), (BYTE *)l_szEndTime);

		    EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_5), (BYTE *)l_szPeriod);
            // Screen Formatting
        }
        break;
    
    case SE_SOFTKEY_DISSOLVE:        
		pLocation = &SKY_GET_LOCATION_INFO();
   		pLocation->onoffstate = FALSE;
		SKY_SET_LOCATION_INFO(*pLocation);

		ReportCfrmOff(pLocation);
        EQS_START_CHILD(SKA_LocationSendMsg);
        break;

    case SE_SOFTKEY_SETUP:
		pLocation = &SKY_GET_LOCATION_INFO();
   		pLocation->onoffstate = TRUE;
		SKY_SET_LOCATION_INFO(*pLocation);

        unStart = pLocation->starttime * 60 + pLocation->startmin;
		unEnd = pLocation->endtime * 60 + pLocation->endmin;

		dwCurTime = SKY_GetLocalTime();
		clk_secs_to_julian (dwCurTime, &tm);
		unCurrent = tm.hour * 60 + tm.minute;

		if( ( unCurrent >= unStart ) && ( unCurrent <= unEnd ) )
		{
			(VOID)RptLocationData(pLocation);
            EQS_START_CHILD(SKA_LocationSendMsg);
		}
        else
        {
            // 변경 내용 update
            EQC_SetTextEditData(EQS_GetNthIOC(LV_TEXT_EDIT_1),
                                (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_ON) );
            EQS_SetWindowCML(CL_NONE_DISSOLVE_NONE);     

		    hLocationON = EQC_MessageBox(
				    (BYTE *)SKY_GET_TOKEN(TKN_LOCATION_ON_MSG),
				    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        } 

        break;

    case SE_LOCATION_INFORM:
        EQS_START_CHILD(SKA_LocationNotif);
        EQS_PASS_CURRENT_EVENT();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

VOID SKY_API SKA_LocationNotif(SKY_EVENT_T* pEvent)      
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            160, //SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, 
            WA_SHOW_HEADER | WA_SHOW_CML);

        if(WhatLocationType() == LOCATION_REMOTE_SETTING)
        {
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

            EQC_NoBeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_LOCATION_NEW), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        else if(WhatLocationType() == LOCATION_DELETE_SETTING)
        {
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

            EQC_NoBeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_LOCATION_NOT_HERE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        else
        {
            //error
            EQS_END();
        }

        EQS_FitCanvasSize();

        break;

    case SE_APP_RESUME:    /* end child */
        break;

    case SE_APP_END :     /* The very last event */
        SKY_StopMusic();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_LOCATION_INFORM:
//        SKY_StopMusic();
//        SKY_DoSMSAlert();
//        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        EQS_START_SIBLING(SKA_LocationNotif, FALSE);
        EQS_PASS_CURRENT_EVENT();            
        break;

    case SE_IO_MESGBOX_END: /* The very first event */
        EQS_END();
        break;

    default :
	    SKY_DefEventProc(pEvent);
        break;
    }
}
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
#endif //WIN32