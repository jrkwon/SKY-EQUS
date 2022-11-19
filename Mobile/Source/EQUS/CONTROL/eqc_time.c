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
** 00-12-21   redstar      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include "eqs.h"



/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define MAX_YEAR          9999
#define MIN_YEAR             0
#define MAX_MONTH           12
#define MIN_MONTH            1
#define MAX_DAY             31
#define MIN_DAY              1
#define MAX_HOUR            24
#define MIN_HOUR             1
#define MAX_MINUTE          59
#define MIN_MINUTE           0

#define DRAW_CURSOR       0xffff0000

#define END_TIME_CONTROL  0xffffffff
#define UP_TIME_CONTROL   0xfffffff0
#define DOWN_TIME_CONTROL 0xfffffff1

#define MAX_TIME_STRING   20

#define MAX_YEAR_POS       3   //(0 ~ 3)
#define MAX_MONTH_POS      1   //(0 ~ 1)
#define MAX_DAY_POS        1   //(0 ~ 1)
#define MAX_WEEK_POS       1   //(0)
#define MAX_TIME_POS       5   //(0 ~ 4)

#define TIME_DEL_POS       2  
#define TIME_AMPM_POS      5
#define TIME_NEXT_POS      7
#define TIME2_DEL_POS      9
#define TIME2_AMPM_POS    12


#define TIME1_HOUR_POS     0
#define TIME1_HOUR_DEL     2
#define TIME1_MINUTE_POS   3
#define TIME1_AM_POS       5
#define TIME1_TILD         7

#define TIME2_HOUR_POS     8
#define TIME2_HOUR_DEL    10
#define TIME2_MINUTE_POS  11
#define TIME2_AM_POS      13
#define TIME2_TILD        15


#define TIME_YEAR_DEL      4
#define TIME_MONTH_DEL     7
#define TIME_DAY_DEL       10
#define TIME_LUNAR_DEL     11

#define TIME_HOUR_DEL      13
#define TIME_MINUTE_DEL    16
#define TIME_AM_DEL        17

#define TIME_MINUTE_HOUR_POS  4
#define TIME_MINUTE_HOUR_DEL  6
#define TIME_MINUTE_MIN_POS   8
#define TIME_MINUTE_MIN_DEL   10

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagTIME_CONTROL_T  TIME_CONTROL_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagTIME_CONTROL_T
{
    /* control header */
    CONTROL_HEADER_T Header;   
    /* <-- Add here --> the information about this control */
    TIME_CONTROL_E   TimeType;
    BYTE             *pszStr;
    BOOL             fShow;
    UINT8            nPos;   
    BOOL             fCursor;   
    WORD             Year;
    BOOL             fLunar;
	BOOL             fEdit;
    UINT8            Month;
    UINT8            Week;
    UINT8            Day;
    UINT8            Hour;
    UINT8            Minute;
    BOOL             fPM;
    WORD             wMaxYear;
    WORD             wMinYear;
	UINT8            nMaxMonth;
	UINT8            nMinMonth;
	UINT8            nMaxDay;
	UINT8            nMinDay;
	UINT8            nMaxHour;
	UINT8            nMinHour;
	UINT8            nMaxMinute;
	UINT8            nMinMinute;
};


LOCAL WORD             l_Year;
LOCAL BOOL             l_fLunar;
LOCAL UINT8            l_Month;
LOCAL UINT8            l_Week;
LOCAL UINT8            l_Day;
LOCAL UINT8            l_Hour;
LOCAL UINT8            l_Minute;
LOCAL BOOL             l_fPM;
LOCAL BOOL             l_fDirty;

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


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

LOCAL void* EQS_API sAllocInstance(DWORD nInitTime,  TIME_CONTROL_E WhichTime);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL void  EQS_API sDrawInstance(HCONTROL hControl);
LOCAL void  EQS_API sDrawTextBlink(HCONTROL hControl);

            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

LOCAL void* EQS_API sAllocInstance(DWORD nInitTime, TIME_CONTROL_E WhichTime)
{

    TIME_CONTROL_T *pTimeControl;
    BYTE* pszToken = NULL;

    /* allocate memory for the instance itself */
	pTimeControl = (TIME_CONTROL_T *)EQS_Malloc(sizeof(TIME_CONTROL_T));
    
    /* --> Add here <-- allocate memory for this instance information */

    pTimeControl->TimeType     = WhichTime;
	pTimeControl->fEdit        = TRUE;
    pTimeControl->Year         = 2001;
    pTimeControl->Month        = 1;
    pTimeControl->Week         = 0;
    pTimeControl->Day          = 1;
    pTimeControl->Hour         = 0;
    pTimeControl->Minute       = 0;
    pTimeControl->fLunar       = FALSE;
    pTimeControl->fPM          = FALSE;
    pTimeControl->fShow        = TRUE;
    pTimeControl->nPos         = 0;
    pTimeControl->fCursor      = FALSE;
    pTimeControl->wMaxYear     = MAX_YEAR;
    pTimeControl->wMinYear     = MIN_YEAR;
	pTimeControl->nMaxMonth    = MAX_MONTH;
	pTimeControl->nMinMonth    = MIN_MONTH;
	pTimeControl->nMaxDay      = MAX_DAY;
	pTimeControl->nMinDay      = MIN_DAY;
	pTimeControl->nMaxHour     = MAX_HOUR;
	pTimeControl->nMinHour     = MIN_HOUR;
	pTimeControl->nMaxMinute   = MAX_MINUTE;
	pTimeControl->nMinMinute   = MIN_MINUTE;
    
    switch( WhichTime )
    {
    case YEAR_TIME_CONTROL:
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_YEAR);
        if( nInitTime >= pTimeControl->wMinYear && nInitTime <= pTimeControl->wMaxYear )
        {
            pTimeControl->Year = nInitTime;
        }  
        else 
            pTimeControl->Year = MIN_YEAR;
        
        break;
    case MON_TIME_CONTROL:
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_MONTH);
		if( nInitTime > 0 && nInitTime < 13 )
			pTimeControl->Month = nInitTime;
		else
			pTimeControl->Month = 1;
        break;

    case WEEK_TIME_CONTROL:
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_DAYOFWEEK);
		if( nInitTime < 7 )
			pTimeControl->Week = nInitTime;
		else
			pTimeControl->Week = 0;
        break;
    case DAY_TIME_CONTROL:
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_DAY);
		if( nInitTime > 0 && nInitTime < GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month)) //31 )
			pTimeControl->Day = nInitTime;
		else
			pTimeControl->Day = 1;
        break;
    case TIME_TIME_CONTROL:
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_BLANK);
        pTimeControl->Hour   = nInitTime/60;
        pTimeControl->Minute = nInitTime%60;
        if( pTimeControl->Hour > 12 )
        {
            pTimeControl->Hour -= 12;
            pTimeControl->fPM   = TRUE;
            pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM);
        }
        else
        {
            pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM);
        }
        break;
    case TIME_TO_TIME_CONTROL:
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_BLANK);
        pTimeControl->Hour   = nInitTime/60;
        pTimeControl->Minute = nInitTime%60;
        if( pTimeControl->Hour > 12 )
        {
            pTimeControl->Hour -= 12;
            pTimeControl->fPM   = TRUE;
            pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM);
        }
        else
        {
            pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM);
        }
		pTimeControl->Month = 0;
		pTimeControl->Day   = 0;
		pTimeControl->fLunar = FALSE;
	
        break;

		break;
	case TIME_MINUTE_CONTROL:
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_BLANK);
        pTimeControl->Hour   = nInitTime/60;
        pTimeControl->Minute = nInitTime%60;
        if( pTimeControl->Hour > 12 )
        {
            pTimeControl->Hour -= 12;
            pTimeControl->fPM   = TRUE;
            pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM);
        }
        else
        {
            pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM);
        }
        break;
	case MONTH_DAY_CONTROL:
		pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_BLANK);
		break;
    case YEAR_MONTH_CONTROL:
    case CAL_TIME_CONTROL:       
    case INFORM_TIME_CONTROL:  
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_SOLAR);
        break;
    case ENTIRE_TIME_CONTROL:
    case ALL_TIME_CONTROL:        
        pszToken = (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM);
        break;

    default:
        break;
    }
	pTimeControl->pszStr   = (BYTE *)EQS_Malloc((SIZE_T)(MAX_TIME_STRING+1));    
    STRCPY(pTimeControl->pszStr, pszToken);

	return (void *)pTimeControl;
}


LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
   
	TIME_CONTROL_T *pTimeControl;
    BYTE            szTemp[10], szDigit[10];
    EQS_EVENT       PosEvent;
    WORD            wValue, wTime;

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);
   

    switch( pEvent->EventID  )
    {
	case EQE_APP_IO_MODE:

		l_Year   = pTimeControl->Year;
		l_fLunar = pTimeControl->fLunar;
		l_Month  = pTimeControl->Month;
		l_Week   = pTimeControl->Week;
		l_Day    = pTimeControl->Day;
		l_Hour   = pTimeControl->Hour;
		l_Minute = pTimeControl->Minute;
		l_fPM    = pTimeControl->fPM;
		l_fDirty = EQC_GET_DIRTY(hControl);
		EQC_SET_DIRTY(hControl, TRUE);
		
		break;
    case EQE_APP_DRAW:
        if( pEvent->dwParam == END_TIME_CONTROL )
        {
            pTimeControl->fCursor = FALSE;
            sDrawInstance( hControl );
            pTimeControl->nPos = 0;            
            EQS_EndIO();
        }
        else if( pEvent->dwParam == UP_TIME_CONTROL )
        {
            pTimeControl->fCursor = FALSE;
            sDrawInstance( hControl );
            pTimeControl->nPos = 0;            
            EQS_PassIO(-1);
        }
        else if( pEvent->dwParam == DOWN_TIME_CONTROL )
        {
            pTimeControl->fCursor = FALSE;
            sDrawInstance( hControl );
            pTimeControl->nPos = 0;            
            EQS_PassIO(1);
        }
        else
        {        
            if( EQS_IsActive(hControl) == TRUE )
            {
                EQS_StartControlTimer(hControl, EQS_TIMER_1, 300, FALSE );
            }
            sDrawInstance( hControl );
        }
        //EQS_UpdateDisplay(); 
		EQS_UpdateControlDisplay();
        break;
    case EQE_IO_DELETE:
		if( pEvent->wParam == hControl )
		{
			EQS_SendEvent(EQE_IO_TIME_CANCEL, (WORD)hControl, (DWORD)EQC_DELETED);
		}
        sFreeInstance(hControl);
        break;

	case EQE_KEY_F1:
		if( pTimeControl->TimeType == WEEK_TIME_CONTROL )
		{
			pTimeControl->Week = (pTimeControl->Week+1)%7;
			EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
			break;
		}
		else if( pTimeControl->TimeType == TIME_TIME_CONTROL )
		{
           if( pTimeControl->nPos == TIME1_AM_POS )
		   {
			   pTimeControl->fPM = !pTimeControl->fPM;
		   }
		   else
		   {
			   break;
		   }
		}
		else if( pTimeControl->TimeType == TIME_TO_TIME_CONTROL )
		{
		   UINT8 nHour1, nHour2, nMinute1, nMinute2;

           if( pTimeControl->nPos == TIME1_AM_POS )
		   {			   			   
			   if( pTimeControl->fPM )
			   {
				   pTimeControl->fPM = FALSE;
			   }
			   else
			   {
				   pTimeControl->fPM = TRUE;
				   if( pTimeControl->fLunar == FALSE )
				   {
					   pTimeControl->fLunar = TRUE;
				   }
				   nHour1 = 12 + pTimeControl->Hour;
				   nHour2 = 12 + pTimeControl->Month;

				   if( nHour1 > nHour2 )
				   {
					   pTimeControl->Month = pTimeControl->Hour;
					   if( pTimeControl->Minute > pTimeControl->Day )
					   {
						   pTimeControl->Day = pTimeControl->Minute;
					   }
				   }
				   else if( nHour1 == nHour2 )
				   {
					   if( pTimeControl->Minute > pTimeControl->Day )
					   {
						   pTimeControl->Day = pTimeControl->Minute;
					   }
				   }
				   else ;
			   }
		   }
           else if( pTimeControl->nPos == TIME2_AM_POS )
		   {
			   pTimeControl->fLunar = !pTimeControl->fLunar;
			   if( pTimeControl->fPM )
			   {
				  nHour1 = 12 + pTimeControl->Hour;
			   }
			   else
			   {
				  nHour1 = pTimeControl->Hour;
			   }

			   if( pTimeControl->fLunar )
			   {
					nHour2 = 12 + pTimeControl->Month;
			   }
			   else
			   {
			    	nHour2 = pTimeControl->Month;
			   }

			   if( nHour1 > nHour2 )
			   {
				   if( pTimeControl->fPM != pTimeControl->fLunar )
				   {
					   pTimeControl->fLunar = !pTimeControl->fLunar;
					   break;
				   }
				   else
				   {
					  pTimeControl->Month = pTimeControl->Hour;
					  if( pTimeControl->Minute > pTimeControl->Day )
					  {
					    pTimeControl->Day = pTimeControl->Minute;
					  }
				   }
			   }
			   else if( nHour1 == nHour2 )
			   {
				   if( pTimeControl->Minute > pTimeControl->Day )
				   {
					   pTimeControl->Day = pTimeControl->Minute;
				   }
			   }
			   else ;

		   }
		   else
		   {
			   break;
		   }
		}
		else if( pTimeControl->TimeType == TIME_MINUTE_CONTROL )
		{						
			if( pTimeControl->nPos < TIME_MINUTE_HOUR_POS )
			{
				pTimeControl->fPM = !pTimeControl->fPM;
			}
			else
			{
				break;
			}
		}
		else if( pTimeControl->TimeType == INFORM_TIME_CONTROL )
		{
			if( pTimeControl->nPos == TIME_LUNAR_DEL -1 )
			{
				pTimeControl->fLunar = !pTimeControl->fLunar;
			}
			else
			{
				break;
			}
		}
		else if( pTimeControl->TimeType == ALL_TIME_CONTROL )
		{
			if( pTimeControl->nPos == TIME_AM_DEL -1 )
			{
				pTimeControl->fPM = !pTimeControl->fPM;
			}
			else
			{
				break;
			}
		}
		else if( pTimeControl->TimeType == ENTIRE_TIME_CONTROL )
		{
			if( pTimeControl->nPos == TIME_AM_DEL -1 )
			{
				pTimeControl->fPM = !pTimeControl->fPM;
			}
			else
			{
				break;
			}
		}
		else 
		{
			break;
		}
		//EQS_SendEvent( EQE_APP_DRAW, hControl, EQS_NA);
	   PosEvent.EventID = EQE_KEY_RIGHT;
	   sHandleEvent(hControl, &PosEvent);  
		break;
    case EQE_KEY_UP:
		EQC_SET_DIRTY(hControl, TRUE);
		EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, UP_TIME_CONTROL);
        EQS_SendEvent(EQE_IO_TIME_SELECT, (WORD)hControl, EQS_NA);
        break;
    case EQE_KEY_DOWN:
		EQC_SET_DIRTY(hControl, TRUE);
		EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, DOWN_TIME_CONTROL);
        EQS_SendEvent(EQE_IO_TIME_SELECT, (WORD)hControl, EQS_NA);
        break;
    case EQE_KEY_LEFT:
    case EQE_KEY_STAR:
        if( pTimeControl->nPos > 0 )
        {  
            pTimeControl->nPos--;  
            if( pTimeControl->TimeType == MONTH_DAY_CONTROL )
            {                              
                if( pTimeControl->nPos == TIME_DEL_POS )           
                {
                    pTimeControl->nPos--;
                }
                else if( pTimeControl->nPos == TIME_AMPM_POS  )
                {
                    pTimeControl->nPos--;
                }
                else ;
            }
			else if( pTimeControl->TimeType == TIME_TIME_CONTROL )
			{
				if( pTimeControl->nPos == TIME1_HOUR_DEL )
				{
					pTimeControl->nPos--;
				}
				else ;

			}
			else if(pTimeControl->TimeType == TIME_TO_TIME_CONTROL )
			{
                if( pTimeControl->nPos == TIME1_HOUR_DEL )
				{
					pTimeControl->nPos--;
				}
				else if( pTimeControl->nPos == TIME1_TILD )
				{
					pTimeControl->nPos = TIME1_AM_POS; 
				}
                else if( pTimeControl->nPos == TIME2_HOUR_DEL )
				{
					pTimeControl->nPos--;
				}				
				else
				{
					;
				}
			}
			else if(pTimeControl->TimeType == TIME_MINUTE_CONTROL )
			{
				if( pTimeControl->nPos == (TIME_MINUTE_HOUR_DEL+1) )
				{
					pTimeControl->nPos = TIME_MINUTE_HOUR_DEL - 1;
				}
				
			}
            else if( pTimeControl->TimeType == YEAR_MONTH_CONTROL )
            {
                if( pTimeControl->nPos == TIME_YEAR_DEL )
                    pTimeControl->nPos--;
            }
            else if( pTimeControl->TimeType == INFORM_TIME_CONTROL ||\
                     pTimeControl->TimeType == CAL_TIME_CONTROL)
            {           
                if( pTimeControl->nPos == TIME_MONTH_DEL ||\
                    pTimeControl->nPos == TIME_YEAR_DEL )
                    pTimeControl->nPos--;

            }
            else if( pTimeControl->TimeType == ALL_TIME_CONTROL )
            {
                if( pTimeControl->nPos == TIME_YEAR_DEL   ||  \
                    pTimeControl->nPos == TIME_MONTH_DEL  ||  \
                    pTimeControl->nPos == TIME_DAY_DEL    ||  \
                    pTimeControl->nPos == TIME_HOUR_DEL   )
                {
                    pTimeControl->nPos--;
                }

            }
            else if( pTimeControl->TimeType == ENTIRE_TIME_CONTROL)
            {                    
                if( pTimeControl->nPos == TIME_YEAR_DEL   ||  \
                    pTimeControl->nPos == TIME_MONTH_DEL  ||  \
                    pTimeControl->nPos == TIME_DAY_DEL    ||  \
                    pTimeControl->nPos == TIME_HOUR_DEL )
                {
                    pTimeControl->nPos--;
                }                
            }
            else ;
            
            EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);     
        }
        break;

    case EQE_KEY_RIGHT:
    case EQE_KEY_POUND:
        
        switch( pTimeControl->TimeType)
        {
        case YEAR_TIME_CONTROL:
            if( pTimeControl->nPos < MAX_YEAR_POS )
            {                
                pTimeControl->nPos++;   
                pTimeControl->fCursor = TRUE;
            } 
            break;
        case MON_TIME_CONTROL:      
            if( pTimeControl->nPos < MAX_MONTH_POS )
            {
                pTimeControl->nPos++;
            }   
            break;

        case WEEK_TIME_CONTROL:
            if( pTimeControl->nPos < MAX_WEEK_POS )
            {
                pTimeControl->nPos++;
            }    
            break;
        case DAY_TIME_CONTROL:
            if( pTimeControl->nPos < MAX_DAY_POS )
            {
                pTimeControl->nPos++;
            } 

            break;
        case TIME_TIME_CONTROL:
            if( pTimeControl->nPos < TIME1_AM_POS )
            {
                pTimeControl->nPos++;
                if( pTimeControl->nPos == TIME1_HOUR_DEL )
				{
					pTimeControl->nPos++;
				}
				else ;                              
            } 
            break;
		case TIME_TO_TIME_CONTROL:
            if( pTimeControl->nPos < TIME2_AM_POS)
            {
                pTimeControl->nPos++;
                if( pTimeControl->nPos == TIME1_HOUR_DEL )
				{
					pTimeControl->nPos++;
				}
				else if( pTimeControl->nPos == TIME1_AM_POS + 1 )
				{
					pTimeControl->nPos = TIME2_HOUR_POS;
				}
                else if( pTimeControl->nPos == TIME2_HOUR_DEL )
				{
					pTimeControl->nPos++;
				}
				else 
					;				
            }
			break;
		case TIME_MINUTE_CONTROL:      
			if( pTimeControl->nPos < TIME_MINUTE_HOUR_POS )
			{
				pTimeControl->nPos = TIME_MINUTE_HOUR_POS;
			}
			else if( pTimeControl->nPos == (TIME_MINUTE_HOUR_DEL-1) )
			{
				pTimeControl->nPos = TIME_MINUTE_HOUR_DEL + 2;
			}
			else if( pTimeControl->nPos >= (TIME_MINUTE_MIN_DEL-1) )
			{
				break;
			}
			else
			{
				pTimeControl->nPos++;
			}	
			break;
		case MONTH_DAY_CONTROL:
            if( pTimeControl->nPos < (MAX_TIME_POS-1) )
            {
                pTimeControl->nPos++;
                if( pTimeControl->nPos == TIME_DEL_POS )
                    pTimeControl->nPos++;
                else ;                                
            } 
            break;
        case YEAR_MONTH_CONTROL:
            if( pTimeControl->nPos >= (TIME_MONTH_DEL-1) )
                break;
        case CAL_TIME_CONTROL:
            if( pTimeControl->nPos >= (TIME_DAY_DEL-1) )
                break;
        case INFORM_TIME_CONTROL:
            
            if( pTimeControl->nPos < TIME_DAY_DEL )
            {
                pTimeControl->nPos++;
                if( pTimeControl->nPos == TIME_YEAR_DEL ||\
                    pTimeControl->nPos == TIME_MONTH_DEL )
                {
                    pTimeControl->nPos++;
                }
            }

            break;
        case ALL_TIME_CONTROL:
            if( pTimeControl->nPos < (TIME_AM_DEL-1) )
            {
                pTimeControl->nPos++;
                if( pTimeControl->nPos == TIME_YEAR_DEL   ||  \
                    pTimeControl->nPos == TIME_MONTH_DEL  ||  \
                    pTimeControl->nPos == TIME_DAY_DEL    ||  \
                    pTimeControl->nPos == TIME_HOUR_DEL )
                {
                    pTimeControl->nPos++;
                }

            } 
            break;
        case ENTIRE_TIME_CONTROL:
            if( pTimeControl->nPos < TIME_AM_DEL )
            {
                pTimeControl->nPos++;
                if( pTimeControl->nPos == TIME_YEAR_DEL   ||  \
                    pTimeControl->nPos == TIME_MONTH_DEL  ||  \
                    pTimeControl->nPos == TIME_DAY_DEL    ||  \
                    pTimeControl->nPos == TIME_HOUR_DEL)
                    //pTimeControl->nPos == TIME_MINUTE_DEL )
                {
                    pTimeControl->nPos++;
                }

            } 
            break;      

        default:
            break;
        }      
        EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);  
        break;
    case EQE_KEY_CENTER:       
		EQC_SET_DIRTY(hControl, TRUE);
        EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, END_TIME_CONTROL);
        EQS_SendEvent( EQE_IO_TIME_SELECT , (WORD)hControl, EQS_NA);        
        break;
    
    case EQE_KEY_0:
    case EQE_KEY_1:
    case EQE_KEY_2:
    case EQE_KEY_3:
    case EQE_KEY_4:
    case EQE_KEY_5:
    case EQE_KEY_6:
    case EQE_KEY_7:
    case EQE_KEY_8:
    case EQE_KEY_9:
        switch( pTimeControl->TimeType)
        {
        case YEAR_TIME_CONTROL:
            sprintf((CHAR*)szTemp,"%04d",pTimeControl->Year);
            szTemp[pTimeControl->nPos] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
            
			STRCPY(szDigit, szTemp + pTimeControl->nPos +1);
            wValue = (WORD) EQS_atof((CHAR*)szTemp);
			if( STRLEN(szDigit) )
			{
				wValue -= EQS_atof((CHAR*)szDigit);
			}

			if( wValue <= pTimeControl->wMinYear )
			{
				pTimeControl->Year = pTimeControl->wMinYear;
			}
			else if( wValue >= pTimeControl->wMaxYear )
			{
				pTimeControl->Year = pTimeControl->wMaxYear;
			}
			else
            {
                pTimeControl->Year = wValue;
            }
            //EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
            PosEvent.EventID = EQE_KEY_RIGHT;
            sHandleEvent(hControl, &PosEvent);

            break;
        case MON_TIME_CONTROL:   
            wValue = pTimeControl->Month;

            szTemp[0] = wValue / 10 +'0';
            szTemp[1] = wValue % 10 +'0';
            szTemp[2] = 0;
                           
            szTemp[pTimeControl->nPos] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
			STRCPY(szDigit, szTemp + pTimeControl->nPos +1);
            wValue = (WORD) EQS_atof((CHAR*)szTemp);
			if( pTimeControl->nPos == 0 )
			{
				if( wValue > 12 )
				{
					wValue -= wValue %10;
				}
			}

            if( wValue > 12 )            
                break;
            
            if( wValue == 0 && pTimeControl->nPos != 0 )
				break;

            if(  wValue >= pTimeControl->nMinMonth && wValue <= pTimeControl->nMaxMonth )
			{
				pTimeControl->Month = wValue;
				//EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
			}
			else if( wValue < pTimeControl->nMinMonth )
			{
				pTimeControl->Month = pTimeControl->nMinMonth;
			}
			else 
			{
				pTimeControl->Month = pTimeControl->nMaxMonth;
			}
			PosEvent.EventID = EQE_KEY_RIGHT;
			sHandleEvent(hControl, &PosEvent);

            break;
        case WEEK_TIME_CONTROL:
/*
			if( pEvent->EventID != EQE_KEY_0 )
			{
				pTimeControl->Week = pEvent->EventID - EQE_KEY_1;
				pTimeControl->Week %=7;
				pTimeControl->nPos = 0;
			}
*/

            break;
        case DAY_TIME_CONTROL:
            wValue = pTimeControl->Day;

            szTemp[0] = wValue / 10 +'0';
            szTemp[1] = wValue % 10 +'0';
            szTemp[2] = 0;
                           
            szTemp[pTimeControl->nPos] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
            wValue = (WORD) EQS_atof((CHAR*)szTemp);
			if( pTimeControl->nPos == 0 )
			{
				if( wValue > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month)) //31 )
				{
					wValue -= wValue %10;
				}
			}
             
            if( wValue >= 40 )
                break;
			if( wValue > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month)) //31 )
			{
				wValue = GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month); //30;
			}				
            if( wValue == 0 && pTimeControl->nPos != 0 )
				break;

			if(  wValue >= pTimeControl->nMinDay && wValue <= pTimeControl->nMaxDay )
			{
				pTimeControl->Day = wValue;
				//EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);     
			}
			else if( wValue < pTimeControl->nMinDay )
			{
				pTimeControl->Day = pTimeControl->nMinDay;
			}
			else 
			{
				pTimeControl->Day = pTimeControl->nMaxDay;
			}
							
			PosEvent.EventID = EQE_KEY_RIGHT;				
			sHandleEvent(hControl, &PosEvent);            
            break;
			
		case TIME_MINUTE_CONTROL:	
			if( pTimeControl->nPos < TIME_MINUTE_HOUR_POS ) // am or pm
			{
				break;
			}
            else if( pTimeControl->nPos < TIME_MINUTE_HOUR_DEL )  // Hour
            {
                szTemp[0] = pTimeControl->Hour /10 +'0';
                szTemp[1] = pTimeControl->Hour %10 +'0';
                szTemp[2] = 0;
                szTemp[pTimeControl->nPos-TIME_MINUTE_HOUR_POS] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
                wTime = (WORD) EQS_atof((CHAR*)szTemp);
				if( pTimeControl->nPos == TIME_MINUTE_HOUR_POS )
				{
					wTime -= wTime%10;
					if( wTime == 0 && pTimeControl->fPM )
						wTime = 1;
				}

                if( wTime > 12 )
                    break;
				else if( wTime == 12 )
				{
					if( pTimeControl->fPM )
						wTime = 0;
					else
						break;
				}
				else ;

                pTimeControl->Hour = wTime;
            }
            else if( pTimeControl->nPos < TIME_MINUTE_MIN_DEL )   // Min
            {
                szTemp[0] = pTimeControl->Minute /10 +'0';
                szTemp[1] = pTimeControl->Minute %10 +'0';
                szTemp[2] = 0;
                szTemp[pTimeControl->nPos-TIME_MINUTE_MIN_POS] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
                wTime = (WORD) EQS_atof((CHAR*)szTemp);
				if( pTimeControl->nPos == TIME_MINUTE_MIN_POS )
					wTime -= wTime%10;
                
                if( wTime >= 60 )
                    break;
                pTimeControl->Minute = wTime;
            }
            else 
                return ;
            //EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
            PosEvent.EventID = EQE_KEY_RIGHT;
            sHandleEvent(hControl, &PosEvent);

            break;
		case TIME_TIME_CONTROL: 
            if( pTimeControl->nPos < TIME1_HOUR_DEL )  // Hour
            {
                szTemp[0] = pTimeControl->Hour /10 +'0';
                szTemp[1] = pTimeControl->Hour %10 +'0';
                szTemp[2] = 0;
                szTemp[pTimeControl->nPos-TIME1_HOUR_POS] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
                wTime = (WORD) EQS_atof((CHAR*)szTemp);
				if( pTimeControl->nPos == TIME1_HOUR_POS )
				{
					wTime -= wTime%10;										
					if( wTime == 0 && pTimeControl->fPM )
						wTime = 1;
				}

                if( wTime > 12 )
                    break;
				else if( wTime == 12 )
				{
					if( pTimeControl->fPM )
						wTime = 0;
					else
						break;
				}
				else ;

                pTimeControl->Hour = wTime;
            }
            else if( pTimeControl->nPos < TIME1_TILD )   // Min
            {
                szTemp[0] = pTimeControl->Minute /10 +'0';
                szTemp[1] = pTimeControl->Minute %10 +'0';
                szTemp[2] = 0;
                szTemp[pTimeControl->nPos-TIME1_MINUTE_POS] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
                wTime = (WORD) EQS_atof((CHAR*)szTemp);
				if( pTimeControl->nPos == TIME1_MINUTE_POS )
					wTime -= wTime%10;
                
                if( wTime >= 60 )
                    break;
                pTimeControl->Minute = wTime;
            }
            else 
                return ;
            //EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
            PosEvent.EventID = EQE_KEY_RIGHT;
            sHandleEvent(hControl, &PosEvent);               
            break;	
		case TIME_TO_TIME_CONTROL:
			{
				UINT8 nHour1, nHour2;

				if( pTimeControl->nPos < TIME1_HOUR_DEL )  // Hour
				{
					szTemp[0] = pTimeControl->Hour /10 +'0';
					szTemp[1] = pTimeControl->Hour %10 +'0';
					szTemp[2] = 0;
					szTemp[pTimeControl->nPos-TIME1_HOUR_POS] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
					wTime = (WORD) EQS_atof((CHAR*)szTemp);
					if( pTimeControl->nPos == TIME1_HOUR_POS )
					{
						wTime -= wTime%10;
						if( wTime == 0 && pTimeControl->fPM )
							wTime = 1;
					}

					if( wTime > 12 )
						break;
					else if( wTime == 12 )
					{
						if( pTimeControl->fPM )
							wTime = 0;
						else
							break;
					}
					else ;

					pTimeControl->Hour = wTime;

					if( pTimeControl->fPM )
					{
						nHour1 = 12 + pTimeControl->Hour;
					}
					else
					{
						nHour1 = pTimeControl->Hour;
					}

					if( pTimeControl->fLunar )
					{
						nHour2 = 12 + pTimeControl->Month;
					}
					else
					{
						nHour2 = pTimeControl->Month;
					}

					if( nHour1 > nHour2 )
					{
					   pTimeControl->Month = pTimeControl->Hour;
						if( pTimeControl->Minute > pTimeControl->Day )
					   {
						   pTimeControl->Day = pTimeControl->Minute;
					   }
					}
				    else if( nHour1 == nHour2 )
					{
					   if( pTimeControl->Minute > pTimeControl->Day )
					   {							   
						   pTimeControl->Day = pTimeControl->Minute;						   
					   }
					}
				    else if( nHour1 == nHour2 )
					{
					   if( pTimeControl->Minute > pTimeControl->Day )
					   {
						   pTimeControl->Day = pTimeControl->Minute;
					   }
					}
					else ;
					
				}
				else if( pTimeControl->nPos < TIME1_TILD )   // Min
				{
					szTemp[0] = pTimeControl->Minute /10 +'0';
					szTemp[1] = pTimeControl->Minute %10 +'0';
					szTemp[2] = 0;
					szTemp[pTimeControl->nPos-TIME1_MINUTE_POS] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
					wTime = (WORD) EQS_atof((CHAR*)szTemp);
					if( pTimeControl->nPos == TIME1_MINUTE_POS )
					{
						wTime -= wTime%10;
						if( wTime == 0 && pTimeControl->fLunar )
							wTime = 1;
					}
                
					if( wTime >= 60 )
						break;
					pTimeControl->Minute = wTime;

					if( pTimeControl->fPM )
					{
						nHour1 = 12 + pTimeControl->Hour;
					}
					else
					{
						nHour1 = pTimeControl->Hour;
					}

					if( pTimeControl->fLunar )
					{
						nHour2 = 12 + pTimeControl->Month;
					}
					else
					{
						nHour2 = pTimeControl->Month;
					}

					if( nHour1 > nHour2 )
					{
					   pTimeControl->Month = pTimeControl->Hour;
						if( pTimeControl->Minute > pTimeControl->Day )
					   {
						   pTimeControl->Day = pTimeControl->Minute;
					   }
					}
				    else if( nHour1 == nHour2 )
					{
					   if( pTimeControl->Minute > pTimeControl->Day )
					   {
						   pTimeControl->Day = pTimeControl->Minute;
					   }
					}
					else ;
				}
				else if( pTimeControl->nPos <  TIME2_HOUR_POS )
				{
					break;// pTimeControl->fLunar = !pTimeControl->fLunar;
				}
				else if( pTimeControl->nPos <  TIME2_HOUR_DEL )  // Hour
				{
					UINT8 nHour1, nHour2;
					szTemp[0] = pTimeControl->Month /10 +'0';
					szTemp[1] = pTimeControl->Month %10 +'0';
					szTemp[2] = 0;
					szTemp[pTimeControl->nPos-TIME2_HOUR_POS] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
					wTime = (WORD) EQS_atof((CHAR*)szTemp);
					if( pTimeControl->nPos == TIME2_HOUR_POS )
					{
						wTime -= wTime%10;						
					}

					if( wTime > 12 )
						break;
					else if( wTime == 12 )
					{
						if( pTimeControl->fLunar )
							wTime = 0;
						else
							break;
					}
					else ;

					nHour1 = pTimeControl->Hour;
					if( pTimeControl->fPM )
						nHour1 += 12;

					nHour2 = wTime;
					if( pTimeControl->fLunar )
						nHour2 += 12;

					if( nHour1 > nHour2 )
					{
						if( pTimeControl->nPos == TIME2_HOUR_POS )
						{
							pTimeControl->Month = pTimeControl->Hour;
						}
						else
						{
							break;
						}
					}
				    else if( nHour1 == nHour2 )
					{
                       pTimeControl->Month = wTime;
					   if( pTimeControl->Minute > pTimeControl->Day )
					   {							   
						   pTimeControl->Day = pTimeControl->Minute;						   
					   }
					}
					else
					{
						pTimeControl->Month = wTime;
					}
				}
				else if( pTimeControl->nPos < TIME2_TILD)   // Min
				{
					szTemp[0] = pTimeControl->Day /10 +'0';
					szTemp[1] = pTimeControl->Day %10 +'0';
					szTemp[2] = 0;
					szTemp[pTimeControl->nPos-TIME2_MINUTE_POS] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
					wTime = (WORD) EQS_atof((CHAR*)szTemp);
					if( pTimeControl->nPos == TIME2_MINUTE_POS )
						wTime -= wTime%10;
                
					if( wTime >= 60 )
						break;
					
					if( pTimeControl->fPM )
					{
						nHour1 = 12 + pTimeControl->Hour;
					}
					else
					{
						nHour1 = pTimeControl->Hour;
					}

					if( pTimeControl->fLunar )
					{
						nHour2 = 12 + pTimeControl->Month;
					}
					else
					{
						nHour2 = pTimeControl->Month;
					}

				    if( nHour1 == nHour2 )
					{
					   if( pTimeControl->Minute > wTime )
					   {							   
						   wTime = pTimeControl->Minute;
					   }
					}
					else ;

					pTimeControl->Day = wTime;

				}
				else 
					return ;
				//EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
				PosEvent.EventID = EQE_KEY_RIGHT;
				sHandleEvent(hControl, &PosEvent);
			}

			break;
		case MONTH_DAY_CONTROL:
            if( pTimeControl->nPos < TIME_DEL_POS )  // MONTH
            {
				wValue = pTimeControl->Month;

				szTemp[0] = wValue / 10 +'0';
				szTemp[1] = wValue % 10 +'0';
				szTemp[2] = 0;
                           
				szTemp[pTimeControl->nPos] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
				STRCPY(szDigit, szTemp + pTimeControl->nPos +1);
				wValue = (WORD) EQS_atof((CHAR*)szTemp);
				if( pTimeControl->nPos == 0 )
				{
					if( wValue > 12 )
					{
						wValue -= wValue %10;
					}
				}

				if( wValue > 12 )            
					break;
            
				if( wValue == 0 && pTimeControl->nPos != 0 )
					break;

				if( wValue <= pTimeControl->nMinMonth )
				{
					wValue = pTimeControl->nMinMonth;
					if( pTimeControl->Day <  pTimeControl->nMinDay )
						pTimeControl->Day = pTimeControl->nMinDay;
				}
				else if( wValue > pTimeControl->nMaxMonth )
				{
					wValue = pTimeControl->nMaxMonth;
				}
				else ;

				if( wValue != 0  )
				{				
					pTimeControl->Month = wValue;
				}
				else
				{
					pTimeControl->Month = 1;
				}
            }
            else if( pTimeControl->nPos < TIME_AMPM_POS )   // DAY
            {
				wValue = pTimeControl->Day;

				szTemp[0] = wValue / 10 +'0';
				szTemp[1] = wValue % 10 +'0';
				szTemp[2] = 0;
                           
				szTemp[pTimeControl->nPos-TIME_DEL_POS-1] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
				wValue = (WORD) EQS_atof((CHAR*)szTemp);
				if( pTimeControl->nPos == 0 )
				{
					if( wValue > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month)) //31 )
					{
						wValue -= wValue %10;
					}
				}

				if( wValue >= 40 )
					break;
				if( wValue > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month)) //31 )
				{
					wValue = GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month); //30;
				}

				if( wValue == 0 && pTimeControl->nPos != 0 )
					break; 
				
				if( (pTimeControl->Month == pTimeControl->nMinMonth && wValue < pTimeControl->nMinDay) )
				{
					pTimeControl->Day = pTimeControl->nMinDay;
				}
				else if(pTimeControl->Month == pTimeControl->nMaxMonth && wValue > pTimeControl->nMaxDay)   
				{
					pTimeControl->Day = pTimeControl->nMaxDay;
				}
				else
				{
					if( wValue != 0 )
					{
						pTimeControl->Day = wValue;
					}
					else
					{
						pTimeControl->Day = 1;
					}					
				}
            }
            else 
                return ;
            //EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);			
            PosEvent.EventID = EQE_KEY_RIGHT;
            sHandleEvent(hControl, &PosEvent);     
			break;
        case YEAR_MONTH_CONTROL:
        case CAL_TIME_CONTROL:         
        case INFORM_TIME_CONTROL:
            if( pTimeControl->nPos == TIME_DAY_DEL )
                break;        
        case ENTIRE_TIME_CONTROL:
        case ALL_TIME_CONTROL:
            if( pTimeControl->nPos < TIME_YEAR_DEL )
            {
                sprintf((CHAR*)szTemp,"%04d",pTimeControl->Year);
                szTemp[pTimeControl->nPos] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
              
				STRCPY(szDigit, szTemp + pTimeControl->nPos +1);
				wValue = (WORD) EQS_atof((CHAR*)szTemp);
				if( STRLEN(szDigit) )
					wValue -= EQS_atof((CHAR*)szDigit);       

                if( wValue < pTimeControl->wMinYear )
				{
					pTimeControl->Year = pTimeControl->wMinYear;
				}
				else if( wValue > pTimeControl->wMaxYear )
				{
					pTimeControl->Year = pTimeControl->wMaxYear ;
				}
				else if( wValue == pTimeControl->wMinYear )
				{
					pTimeControl->Year = pTimeControl->wMinYear;
					if( pTimeControl->Month <= pTimeControl->nMinMonth )
					{
						pTimeControl->Month = pTimeControl->nMinMonth;
						if( pTimeControl->Day <= pTimeControl->nMinDay )
						{							 
							pTimeControl->Day = pTimeControl->nMinDay;
						}
					}
				}									
				else if(  wValue == pTimeControl->wMaxYear )
				{
					pTimeControl->Year = pTimeControl->wMaxYear;
					if( pTimeControl->Month >= pTimeControl->nMaxMonth )
					{
						pTimeControl->Month = pTimeControl->nMaxMonth;
						if( pTimeControl->Day >= pTimeControl->nMaxDay )
						{							 
							pTimeControl->Day = pTimeControl->nMaxDay;
						}
					}

				}
                else
                {
                    pTimeControl->Year = wValue;
                } 

                // jrkwon 2001.08.19 --> 
                //if(pTimeControl->Day > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month))
                //    pTimeControl->Day = GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month);
                // <--
            }
            else if( pTimeControl->nPos < TIME_MONTH_DEL )
            {
                wValue = pTimeControl->Month;

                szTemp[0] = wValue / 10 +'0';
                szTemp[1] = wValue % 10 +'0';
                szTemp[2] = 0;
                           
                szTemp[pTimeControl->nPos-TIME_YEAR_DEL-1] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
                wValue = (WORD) EQS_atof((CHAR*)szTemp);

				if( pTimeControl->nPos == (TIME_YEAR_DEL+1) )
				{
					if( wValue > 12 )
					{
						wValue -= wValue%10;
					}
				}

                if( wValue > 12 )
                    break;

				if( wValue == 0 && pTimeControl->nPos != (TIME_YEAR_DEL+1) )
					break;

                pTimeControl->Month = wValue;

				if( pTimeControl->Year == pTimeControl->wMinYear )
				{
					if( pTimeControl->Month <= pTimeControl->nMinMonth )
					{
						pTimeControl->Month = pTimeControl->nMinMonth;
						if( pTimeControl->Day <= pTimeControl->nMinDay )
						{							 
							pTimeControl->Day = pTimeControl->nMinDay;
						}
					}
				}									
				else if( pTimeControl->Year == pTimeControl->wMaxYear )
				{
					if( pTimeControl->Month >= pTimeControl->nMaxMonth )
					{
						pTimeControl->Month = pTimeControl->nMaxMonth;
						if( pTimeControl->Day >= pTimeControl->nMaxDay )
						{							 
							pTimeControl->Day = pTimeControl->nMaxDay;
						}
					}
				}
				else 
					;
				
				if( pTimeControl->Month == 0 )
				{
					pTimeControl->Month = 1;
				}

                // jrkwon 2001.08.19 --> 
                //if(pTimeControl->Day > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month))
                //    pTimeControl->Day = GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month);
                // <--
				
            }
            else if( pTimeControl->nPos < TIME_DAY_DEL )
            {
                wValue = pTimeControl->Day;

                szTemp[0] = wValue / 10 +'0';
                szTemp[1] = wValue % 10 +'0';
                szTemp[2] = 0;
                           
                szTemp[pTimeControl->nPos-TIME_MONTH_DEL-1] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
                wValue = (WORD) EQS_atof((CHAR*)szTemp);
                
				if( pTimeControl->nPos == (TIME_MONTH_DEL+1) )
				{
					if( wValue > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month)) //31 )
					{
						wValue -= wValue%10;
					}
				}
                if( wValue >= 40 )
                    break;
				if( wValue > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month)) //31 )
				{
					wValue = GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month); //30;
				}
				if( wValue == 0 && pTimeControl->nPos != (TIME_MONTH_DEL+1) )
					break;

				if( wValue != 0 )
				{
					pTimeControl->Day = wValue;
				}
				else
				{
					pTimeControl->Day = 1;
				}

				if( pTimeControl->Year == pTimeControl->wMinYear )
				{
					if( pTimeControl->Month == pTimeControl->nMinMonth )
					{
						if( pTimeControl->Day <= pTimeControl->nMinDay )
						{							 
							pTimeControl->Day = pTimeControl->nMinDay;
						}
					}
				}									
				else if( pTimeControl->Year == pTimeControl->wMaxYear )
				{
					if( pTimeControl->Month == pTimeControl->nMaxMonth )
					{
						if( pTimeControl->Day >= pTimeControl->nMaxDay )
						{							 
							pTimeControl->Day = pTimeControl->nMaxDay;
						}
					}
				}
				else ;				

            }
            else if( pTimeControl->nPos < TIME_HOUR_DEL )
            {
                szTemp[0] = pTimeControl->Hour /10 +'0';
                szTemp[1] = pTimeControl->Hour %10 +'0';
                szTemp[2] = 0;
                szTemp[pTimeControl->nPos-TIME_DAY_DEL-1] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
                wTime = (WORD) EQS_atof((CHAR*)szTemp);
				
				if( pTimeControl->nPos == (TIME_DAY_DEL+1) )
					wTime -= wTime%10;
				if( wTime >= 24 )
					break;

				if( pTimeControl->nPos > (TIME_DAY_DEL+1 ) )
				{
					if( wTime > 12 )
					{
						if( wTime == 24 )
						{
							wTime = 0;
							pTimeControl->fPM = FALSE;
						}
						else
						{
							wTime -=12;
							pTimeControl->fPM = TRUE;
						}
					}
					else
					{
						pTimeControl->fPM = FALSE;
					}
				}
                pTimeControl->Hour = wTime;

            }
            else if( pTimeControl->nPos < TIME_MINUTE_DEL )
            {
                szTemp[0] = pTimeControl->Minute /10 +'0';
                szTemp[1] = pTimeControl->Minute %10 +'0';
                szTemp[2] = 0;
                szTemp[pTimeControl->nPos-TIME_HOUR_DEL-1] = (BYTE)((pEvent->EventID - EQE_KEY_0)+'0');
                wTime = (WORD) EQS_atof((CHAR*)szTemp);

				if( pTimeControl->nPos == (TIME_HOUR_DEL+1) )
					wTime -= wTime%10;
                if( wTime >= 60 )
                    break;
                pTimeControl->Minute = wTime;

            }
            else  
            {
                pTimeControl->Week = pEvent->EventID - EQE_KEY_0;
                pTimeControl->Week %=7;
            }      
            //EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);  
            PosEvent.EventID = EQE_KEY_RIGHT;
            sHandleEvent(hControl, &PosEvent);
            break;      
        default:
            break;
        }

        // jrkwon 2001.08.19 --> 
        if(pTimeControl->Day > GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month))
            pTimeControl->Day = GetDaysOfMonth(pTimeControl->Year, pTimeControl->Month);
        // <--

        EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);  
        break;

    case EQE_KEY_CLEAR:        
		pTimeControl->Year = l_Year;
		pTimeControl->fLunar = l_fLunar;
		pTimeControl->Month = l_Month;
		pTimeControl->Week  = l_Week;
		pTimeControl->Day = l_Day;
		pTimeControl->Hour = l_Hour;
		pTimeControl->Minute = l_Minute;
		pTimeControl->fPM = l_fPM;
		EQC_SET_DIRTY(hControl, l_fDirty);
        EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, END_TIME_CONTROL);
        EQS_SendEvent( EQE_IO_TIME_CANCEL , (WORD)hControl, EQS_NA);      
        break;

    case EQE_TM_TIMER1:        
        if( EQS_IsActive(hControl) == TRUE )
        {
            pTimeControl->fCursor = !pTimeControl->fCursor;
            EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, DRAW_CURSOR); 
            EQS_StartControlTimer(hControl, EQS_TIMER_1, 300, FALSE );
        }
        break;
    default:
        break;	
    }
}

LOCAL void EQS_API sFreeInstance(HCONTROL hControl)
{
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);
    /* free the memory that was allocated */
	if ( ((TIME_CONTROL_T *)EQC_GET_INSTANCE(hControl))->pszStr != 0 )
		EQS_Free((void**)&((TIME_CONTROL_T *)EQC_GET_INSTANCE(hControl))->pszStr);

    /* free the instance itself */
    EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

LOCAL void EQS_API sDrawInstance(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    COORD          StartX;
    COORD          StartY;
 
    UINT8          nWidth, nHeight;
    BOOL           fActive;


	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    EQS_GetDC(TRUE);

#if 0
	if( EQC_GET_FOCUS(hControl) == FALSE )
	{
		EQS_SetTextColor(CTRL_DEACTIVE_COLOR);
	}
#endif


    StartX = EQC_GET_X(hControl);
    StartY = EQC_GET_Y(hControl);

    if( pTimeControl->fShow == FALSE )
        return ;

    nWidth  = EQS_CharWidth((BYTE)' ');
    nHeight = EQS_CharHeight((BYTE)' ');
    
    fActive = EQS_IsActive(hControl);

    switch( pTimeControl->TimeType)
    {
    case YEAR_TIME_CONTROL:
        sprintf((CHAR*)pTimeControl->pszStr,"%04d%s",pTimeControl->Year,(BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_YEAR) );
        break;
    case MON_TIME_CONTROL:   
        sprintf((CHAR*)pTimeControl->pszStr,"%02d%s",\
											pTimeControl->Month,
											(BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_MONTH));

        break;

    case WEEK_TIME_CONTROL:
        STRCPY(pTimeControl->pszStr, EQS_GET_TOKEN(TKN_TIME_CONTROL_MON_DAY + pTimeControl->Week));
        break;
        
    case DAY_TIME_CONTROL:

        sprintf((CHAR*)pTimeControl->pszStr,"%02d%s",pTimeControl->Day,
													 EQS_GET_TOKEN(TKN_TIME_CONTROL_DAY));
        break;
    case TIME_TIME_CONTROL:
		if( pTimeControl->fEdit == FALSE )
		{			
			STRCPY( pTimeControl->pszStr, (BYTE*)"--:--");			
			STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM));
		}
		else
		{
			UINT8 nHour;


			nHour = pTimeControl->Hour;
			if( pTimeControl->fPM && nHour == 0 )
			{
				nHour = 12;
			}

			sprintf((CHAR*)pTimeControl->pszStr,"%02d:%02d", nHour,
												 pTimeControl->Minute);
			if( pTimeControl->fPM )
			{
				STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM));
			}
			else
			{
				STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM));
			}


		}
        break;
	case TIME_TO_TIME_CONTROL:
		if( pTimeControl->fEdit == FALSE )
		{
			sprintf((CHAR*) pTimeControl->pszStr,"--:--%s~--:--%s",\
				                                 (CHAR*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM),\
												 (CHAR*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM));
		}
		else
		{
			UINT8 nHour;
			nHour = pTimeControl->Hour;
			if( pTimeControl->fPM && nHour == 0 )
			{
				nHour = 12;
			}
			sprintf((CHAR*)pTimeControl->pszStr,"%02d:%02d", nHour,
												 pTimeControl->Minute);

			if( pTimeControl->fPM )
			{
				STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM));
			}
			else
			{
				STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM));
			}

			STRCAT(pTimeControl->pszStr,(BYTE*)"~");

			nHour = pTimeControl->Month;
			if( pTimeControl->fLunar && nHour == 0 )
			{
				nHour = 12;
			}

			sprintf((CHAR*)pTimeControl->pszStr + STRLEN(pTimeControl->pszStr),"%02d:%02d",\
				                                          nHour, pTimeControl->Day);
			if( pTimeControl->fLunar )
			{
				STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM));
			}
			else
			{
				STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM));
			}			

		}
		break;
    case TIME_MINUTE_CONTROL:
		if(pTimeControl->fEdit == FALSE )
		{
			sprintf((CHAR*)pTimeControl->pszStr,"%s--%s--%s",\
								   (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HAM),\
									(BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HTIME),\
									(BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HMINUTE));
		}
		else
		{
			UINT8 nHour;
			nHour = pTimeControl->Hour;
			if( pTimeControl->fPM )
			{
				if( nHour == 0 )
					nHour = 12;
				sprintf((CHAR*)pTimeControl->pszStr,"%s%02d%s%02d%s",\
									   (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HPM),\
													 nHour,
										(BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HTIME),\
													 pTimeControl->Minute,
										(BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HMINUTE));
			}
			else
			{
				sprintf((CHAR*)pTimeControl->pszStr,"%s%02d%s%02d%s",\
									   (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HAM),\
													 nHour,
										(BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HTIME),\
													 pTimeControl->Minute,
										(BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_HMINUTE));
			} 

		}
		break;
	case MONTH_DAY_CONTROL:
		if( pTimeControl->fEdit == FALSE )
		{
			sprintf((CHAR*)pTimeControl->pszStr,"--%s--%s",\
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_MONTH),\
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_DAY));
		}
		else
		{

			sprintf((CHAR*)pTimeControl->pszStr,"%02d%s%02d%s",\
															   pTimeControl->Month,
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_MONTH),\
															   pTimeControl->Day,
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_DAY));
		}
		break;
    case YEAR_MONTH_CONTROL:
		if( pTimeControl->fEdit == FALSE )
		{
			sprintf((CHAR*)pTimeControl->pszStr,"----%s--%s",\
												(BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_YEAR),\
												(BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_MONTH));

		}
		else
		{
			sprintf((CHAR*)pTimeControl->pszStr,"%04d%s%02d%s",pTimeControl->Year,\
															(BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_YEAR),\
															pTimeControl->Month,
															(BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_MONTH));
		}

        break;
    
    case CAL_TIME_CONTROL:
		if( pTimeControl->fEdit == FALSE )
		{
			sprintf((CHAR*)pTimeControl->pszStr,"----%s--%s--%s",\
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_YEAR),\
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_MONTH),\
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_DAY));

		}
		else
		{
			sprintf((CHAR*)pTimeControl->pszStr,"%04d%s%02d%s%02d%s",pTimeControl->Year,\
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_YEAR),\
															   pTimeControl->Month,
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_MONTH),\
															   pTimeControl->Day,
										 (BYTE*) EQS_GET_TOKEN(TKN_TIME_CONTROL_DAY));
		}
		break;
	case INFORM_TIME_CONTROL:
		if( pTimeControl->fEdit == FALSE )
		{
			STRCPY(pTimeControl->pszStr,(BYTE*)"----.--.--");
		}
		else
		{
			if( pTimeControl->Year == 0 )
			{
				sprintf((CHAR*)pTimeControl->pszStr,"----.%02d.%02d",\
												   pTimeControl->Month,
												    pTimeControl->Day);
			}
			else
			{
				sprintf((CHAR*)pTimeControl->pszStr,"%04d.%02d.%02d",pTimeControl->Year,\
																   pTimeControl->Month,
																   pTimeControl->Day);
			}
		}
        if( pTimeControl->fLunar == TRUE )
            STRCAT(pTimeControl->pszStr, EQS_GET_TOKEN(TKN_TIME_CONTROL_LUNAR));
        else
            STRCAT(pTimeControl->pszStr, EQS_GET_TOKEN(TKN_TIME_CONTROL_SOLAR));

        break;

    case ENTIRE_TIME_CONTROL:
    case ALL_TIME_CONTROL:
		if( pTimeControl->fEdit == FALSE )
		{
			STRCPY(pTimeControl->pszStr,(BYTE*)"----.--.--.--:--");

		}
		else
		{
			sprintf((CHAR*)pTimeControl->pszStr,"%04d.%02d.%02d.%02d:%02d",\
												  pTimeControl->Year,     \
												  pTimeControl->Month,
												  pTimeControl->Day,
												  pTimeControl->Hour,
												  pTimeControl->Minute);
		}
        if( pTimeControl->fPM )
        {
            STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_PM));
        }
        else
        {
            STRCAT( pTimeControl->pszStr, (BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_AM));
        }
        if( pTimeControl->TimeType == ENTIRE_TIME_CONTROL )
        {
            STRCAT(pTimeControl->pszStr, EQS_GET_TOKEN(TKN_TIME_CONTROL_MON + pTimeControl->Week));
        }
        break;                 
    default:
        break;
    }
    
    if( pTimeControl->fCursor )
        sDrawTextBlink(hControl);
    else
	{
		EQS_FillRectangle(EQC_GET_X(hControl),\
			              EQC_GET_Y(hControl),\
						  EQC_GET_X(hControl)+EQC_GET_DX(hControl)-1,\
						  EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-1,\
						  EQS_GetTextBackColor());
        EQS_TextOutStrLen(StartX, StartY,\
                          pTimeControl->pszStr,\
                          STRLEN(pTimeControl->pszStr));
	}

    if( EQC_GET_STYLE(hControl) & CS_TIME_BORDER )
    {
        EQC_DrawBorder( hControl,\
			            EQC_GET_X(hControl) , EQC_GET_Y(hControl),
                       (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1), 
                       (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)-1));

    }

	if( EQS_IsActive(hControl) )
	{
		EQS_EVENT CurrentEvent;

		switch( pTimeControl->TimeType )
		{
		case WEEK_TIME_CONTROL:
			EQS_DrawIOCCML(IOC_CML_NEXT_LEFT);
			break;
		case TIME_TIME_CONTROL:
			if( pTimeControl->nPos == TIME1_AM_POS )
			{
			   if( pTimeControl->fPM )
			   {
				   EQS_DrawIOCCML(IOC_CML_AM_LEFT);
			   }
			   else
			   {
				   EQS_DrawIOCCML(IOC_CML_PM_LEFT);
			   }
			}
			else
			{				
				EQS_DrawIOCCML(IOC_CML_BLANK_LEFT);
			}
			break;
		case TIME_TO_TIME_CONTROL:
			if( pTimeControl->nPos == TIME1_AM_POS )
			{
			   if( pTimeControl->fPM )
			   {
				   EQS_DrawIOCCML(IOC_CML_AM_LEFT);
			   }
			   else
			   {
				   EQS_DrawIOCCML(IOC_CML_PM_LEFT);
			   }
			}
			else if( pTimeControl->nPos == TIME2_AM_POS )
			{
			   if( pTimeControl->fLunar )
			   {
				   EQS_DrawIOCCML(IOC_CML_AM_LEFT);
			   }
			   else
			   {
				   EQS_DrawIOCCML(IOC_CML_PM_LEFT);
			   }
			}
			else
			{				
				EQS_DrawIOCCML(IOC_CML_BLANK_LEFT);
			}
			break;

			break;
		case TIME_MINUTE_CONTROL:
			if( pTimeControl->nPos < TIME_MINUTE_HOUR_POS )
			{
			   if( pTimeControl->fPM )
			   {
				   EQS_DrawIOCCML(IOC_CML_KOREAN_AM_LEFT);
			   }
			   else
			   {
				   EQS_DrawIOCCML(IOC_CML_KOREAN_PM_LEFT);
			   }
			}
			else
			{				
				EQS_DrawIOCCML(IOC_CML_BLANK_LEFT);
			}
			break;
		case INFORM_TIME_CONTROL:
            if( pTimeControl->nPos == TIME_LUNAR_DEL -1 )
			{
				if( pTimeControl->fLunar )
				{
					EQS_DrawIOCCML(IOC_CML_SUN_LEFT);
				}
				else
				{
					EQS_DrawIOCCML(IOC_CML_LUNAR_LEFT);
				}
			}
			else
			{				
				EQS_DrawIOCCML(IOC_CML_BLANK_LEFT);
			}
			break;
		case ALL_TIME_CONTROL:	
		case ENTIRE_TIME_CONTROL:
			if( pTimeControl->nPos == TIME_AM_DEL -1 )
			{
			   if( pTimeControl->fPM )
			   {
				   EQS_DrawIOCCML(IOC_CML_AM_LEFT);
			   }
			   else
			   {
				   EQS_DrawIOCCML(IOC_CML_PM_LEFT);
			   }
			}
			else
			{
				EQS_DrawIOCCML(IOC_CML_BLANK_LEFT);
			}
			break;
		default:			
			EQS_DrawIOCCML(IOC_CML_BLANK_LEFT);	
			break;

		}
		EQS_DrawIOCCML(IOC_CML_DONE_CENTER);

		CurrentEvent = EQS_GetEvent();
		if( CurrentEvent.wParam == hControl )
		{
			EQS_UpdateIOCCML();
		}
	}

    EQS_ReleaseDC();   
    
}

LOCAL void  EQS_API sDrawTextBlink(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    COORD          StartX;
    COORD          StartY;
    UINT8          nPos, nLen;
    BYTE*          pszStr;


	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    StartX = EQC_GET_X(hControl);
    StartY = EQC_GET_Y(hControl);
    nPos   = pTimeControl->nPos;
    pszStr = pTimeControl->pszStr;
    nLen = STRLEN(pszStr);


    switch( pTimeControl->TimeType)
    {
    case YEAR_TIME_CONTROL:
    case MON_TIME_CONTROL:
    case DAY_TIME_CONTROL:
        if( pTimeControl->nPos > 0 )
        {
            EQS_TextOutStrLen(StartX, StartY,pszStr, nPos);
            StartX += EQS_CharWidth((BYTE)' ')*nPos;
        }
        EQS_TextOutStrLenTDM(StartX, StartY, pszStr+nPos, 1, TDM_REVERSE);
        StartX += EQS_CharWidth((BYTE)' ');
        EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+1, nLen-nPos-1);
        break;
    case WEEK_TIME_CONTROL:
        EQS_TextOutStrTDM(StartX, StartY, pszStr, TDM_REVERSE); 
        break;
           
    case TIME_TIME_CONTROL:
        if( pTimeControl->nPos == TIME1_AM_POS )
        {  
			EQS_TextOutStrLen(StartX, StartY,pszStr,nLen-2);
			StartX += EQS_CharWidth((BYTE)' ')*(nLen-2);
			EQS_TextOutStrLenTDM(StartX, StartY,pszStr+nLen-2,2,TDM_REVERSE);                        
        }
        else
        {
            EQS_TextOutStrLen(StartX, StartY,pszStr, nPos);
            StartX += EQS_CharWidth((BYTE)' ')*nPos;            
            EQS_TextOutStrLenTDM(StartX, StartY, pszStr+nPos, 1, TDM_REVERSE);
            StartX += EQS_CharWidth((BYTE)' ');
            EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+1,STRLEN(pszStr)-nPos-1);
        } 
        break;
	case TIME_TO_TIME_CONTROL:
        if( pTimeControl->nPos == TIME2_AM_POS )
        {  
			EQS_TextOutStrLen(StartX, StartY,pszStr,nLen-2);
			StartX += EQS_CharWidth((BYTE)' ')*(nLen-2);
			EQS_TextOutStrLenTDM(StartX, StartY,pszStr+nLen-2,2,TDM_REVERSE);
        }
		else if( pTimeControl->nPos == TIME1_AM_POS   )
		{
            EQS_TextOutStrLen(StartX, StartY,pszStr,nPos);
            StartX += EQS_CharWidth((BYTE)' ') *nPos;
            EQS_TextOutStrLenTDM(StartX, StartY,pszStr+nPos,2,TDM_REVERSE);
			StartX += EQS_CharWidth((BYTE)' ') * 2;
			EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+2,STRLEN(pszStr)-nPos-2);
		}
        else
        {
            EQS_TextOutStrLen(StartX, StartY,pszStr, nPos);
            StartX += EQS_CharWidth((BYTE)' ')*nPos;
            
            EQS_TextOutStrLenTDM(StartX, StartY, pszStr+nPos, 1, TDM_REVERSE);
            StartX += EQS_CharWidth((BYTE)' ');
            EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+1,STRLEN(pszStr)-nPos-1);
        } 
		break;
	case TIME_MINUTE_CONTROL:
		if( pTimeControl->nPos < TIME_MINUTE_HOUR_POS )
		{
			EQS_TextOutStrLenTDM(StartX, StartY, pszStr, 4, TDM_REVERSE);
			StartX += (EQS_CharWidth((BYTE)' ') * 4);
			EQS_TextOutStrTDM(StartX, StartY,pszStr+TIME_MINUTE_HOUR_POS, TDM_NORMAL);
		}
		else 
		{
			EQS_TextOutStrLenTDM(StartX, StartY, pszStr, pTimeControl->nPos, TDM_NORMAL);
			StartX += (EQS_CharWidth((BYTE)' ') * pTimeControl->nPos) ;			
			EQS_TextOutStrLenTDM(StartX, StartY, pszStr+pTimeControl->nPos,1, TDM_REVERSE);
			StartX += EQS_CharWidth((BYTE)' ');
			EQS_TextOutStrTDM(StartX, StartY,pszStr+pTimeControl->nPos+1,TDM_NORMAL);
		}			
		break;
	case MONTH_DAY_CONTROL:
        if( pTimeControl->nPos > 0 )
        {
			if( pTimeControl->nPos > TIME_DEL_POS )
				nPos++;
            EQS_TextOutStrLen(StartX, StartY,pszStr, nPos);
            StartX += EQS_CharWidth((BYTE)' ')*nPos;
        }
        EQS_TextOutStrLenTDM(StartX, StartY, pszStr+nPos, 1, TDM_REVERSE);
        StartX += EQS_CharWidth((BYTE)' ');
        EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+1,STRLEN(pszStr)-nPos-1);
		break;
	case INFORM_TIME_CONTROL:
        if(pTimeControl->nPos == TIME_DAY_DEL )
        {
            EQS_TextOutStrLen(StartX, StartY,pszStr,nLen-2);
            StartX += EQS_CharWidth((BYTE)' ') *(nLen-2);
            EQS_TextOutStrLenTDM(StartX, StartY,pszStr+nLen-2,2,TDM_REVERSE);            
        }
        else
        {
            if( pTimeControl->nPos > 0 )
            {
                EQS_TextOutStrLen(StartX, StartY,pszStr, nPos);
                StartX += EQS_CharWidth((BYTE)' ')*nPos;
            }
            EQS_TextOutStrLenTDM(StartX, StartY, pszStr+nPos, 1, TDM_REVERSE);
            StartX += EQS_CharWidth((BYTE)' ');
            EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+1,STRLEN(pszStr)-nPos-1);
        } 
		break;
    case YEAR_MONTH_CONTROL:    
    case CAL_TIME_CONTROL:

        if( pTimeControl->nPos > 0 )
        {
			if( pTimeControl->nPos > TIME_YEAR_DEL )
				nPos++;
			if( pTimeControl->nPos > TIME_MONTH_DEL )
				nPos++;
			//if( pTimeControl->nPos > TIME_DAY_DEL )
			//	nPos++;
            EQS_TextOutStrLen(StartX, StartY,pszStr, nPos);
            StartX += EQS_CharWidth((BYTE)' ')*nPos;
        }
        EQS_TextOutStrLenTDM(StartX, StartY, pszStr+nPos, 1, TDM_REVERSE);
        StartX += EQS_CharWidth((BYTE)' ');
        EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+1,STRLEN(pszStr)-nPos-1);
        
        break;

    case ENTIRE_TIME_CONTROL:
        if( pTimeControl->nPos == (TIME_AM_DEL-1) )
        {
            EQS_TextOutStrLen(StartX, StartY,pszStr,nLen-4);
            StartX += EQS_CharWidth((BYTE)' ') *(nLen-4);
            EQS_TextOutStrLenTDM(StartX, StartY,pszStr+nLen-4,2,TDM_REVERSE);
            StartX += EQS_CharWidth((BYTE)' ') *2;
            EQS_TextOutStrLen(StartX, StartY,pszStr+nLen-2,2);
        }
        else if( pTimeControl->nPos == TIME_AM_DEL )
        {
            EQS_TextOutStrLen(StartX, StartY,pszStr,nLen-2);
            StartX += EQS_CharWidth((BYTE)' ') *(nLen-2);
            EQS_TextOutStrLenTDM(StartX, StartY,pszStr+nLen-2,2,TDM_REVERSE);
        }
        else
        {
            if( pTimeControl->nPos > 0 )
            {
                EQS_TextOutStrLen(StartX, StartY,pszStr, nPos);
                StartX += EQS_CharWidth((BYTE)' ')*nPos;
            }
            EQS_TextOutStrLenTDM(StartX, StartY, pszStr+nPos, 1, TDM_REVERSE);
            StartX += EQS_CharWidth((BYTE)' ');
            EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+1,STRLEN(pszStr)-nPos-1);
        } 
        break;

    case ALL_TIME_CONTROL:
        if( pTimeControl->nPos == (TIME_AM_DEL-1) )
        {
            EQS_TextOutStrLen(StartX, StartY,pszStr,nLen-2);
            StartX += EQS_CharWidth((BYTE)' ') *(nLen-2);
            EQS_TextOutStrLenTDM(StartX, StartY,pszStr+nLen-2,2,TDM_REVERSE);
        }
        else
        {

            if( pTimeControl->nPos > 0 )
            {
                EQS_TextOutStrLen(StartX, StartY,pszStr, nPos);
                StartX += EQS_CharWidth((BYTE)' ')*nPos;
            }
            EQS_TextOutStrLenTDM(StartX, StartY, pszStr+nPos, 1, TDM_REVERSE);
            StartX += EQS_CharWidth((BYTE)' ');
            EQS_TextOutStrLen(StartX, StartY,pszStr+nPos+1,STRLEN(pszStr)-nPos-1);
        } 
        break;
    default:
        break;
    }   
}

/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

HCONTROL EQS_API EQC_Time(COORD x, COORD y,  DWORD nInitTime, TIME_CONTROL_E WhichTime)
{
	
	TIME_CONTROL_T *pTimeControl;
    HCONTROL        hControl;
    COORD           dx, dy;
   

    /* allocation memory for this instance */
	pTimeControl = (TIME_CONTROL_T *)sAllocInstance(nInitTime, WhichTime);

    //dx = EQS_TextWidth(pTimeControl->pszStr); 
	dx = 0;
    dy = EQS_TextHeight(pTimeControl->pszStr); 
    switch( WhichTime )
    {
    case YEAR_TIME_CONTROL:
        dx += EQS_CharWidth((BYTE)' ') * 6;        
        break;
    case MON_TIME_CONTROL:
        dx += EQS_CharWidth((BYTE)' ') * 4;     
        break;
    case WEEK_TIME_CONTROL:
		dx = EQS_TextWidth((BYTE*)EQS_GET_TOKEN(TKN_TIME_CONTROL_MON_DAY));
        break;
    case DAY_TIME_CONTROL:
        dx += EQS_CharWidth((BYTE)' ') * (4);
        break;
    case TIME_TIME_CONTROL:
        dx += EQS_CharWidth((BYTE)' ') * 7;//5;
        break;
	case TIME_TO_TIME_CONTROL:
		dx += EQS_CharWidth((BYTE)' ')* 15;
		break;
	case TIME_MINUTE_CONTROL:
		dx += EQS_CharWidth((BYTE)' ') * 12;
		break;
	case MONTH_DAY_CONTROL:
		dx += EQS_CharWidth((BYTE)' ') * 8;
		break;
    case YEAR_MONTH_CONTROL:
        dx += EQS_CharWidth((BYTE)' ')*10;
        break;
    case CAL_TIME_CONTROL:
        dx += EQS_CharWidth((BYTE)' ')*14;
        break;
    case INFORM_TIME_CONTROL:
        dx = EQS_CharWidth((BYTE)' ')*12;
        break;
    case ALL_TIME_CONTROL:
        dx = EQS_CharWidth((BYTE)' ')*18;
        break;      
    case ENTIRE_TIME_CONTROL:
        dx = EQS_CharWidth((BYTE)' ')*20;
    default:        
        break;
    }
    /* get dx, dy */
    

/* Original - woodstok 01.1.8
	hControl = EQC_AllocControl(CT_TIME,  pTimeControl, x, y, dx, dy, 
                                sHandleEvent, sFreeInstance );
*/
	hControl = EQC_AllocControl(CT_TIME,  pTimeControl, x, y, dx, dy, 
                                sHandleEvent );

    ASSERT( hControl != INVALID_CONTROL_HANDLE);

    EQC_SET_STYLE(hControl, CS_TIME_DEFAULT);

    /* send draw event to the state machine engine */
	EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);    
    


    return hControl;
}


void EQS_API EQC_ShowTime(HCONTROL hControl, BOOL fShow)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    pTimeControl->fShow  = fShow;
}


void EQS_API EQC_SetTime(HCONTROL hControl, WORD wYear,   \
                                            UINT8 uMonth, \
                                            UINT8 uWeek,  \
                                            UINT8 uDay,   \
                                            UINT8 uHour,  \
                                            UINT8 uMinute)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    if( wYear >= pTimeControl->wMinYear && wYear <= pTimeControl->wMaxYear )
        pTimeControl->Year   = wYear;

	if( uMonth < MIN_MONTH )
		pTimeControl->Month = MIN_MONTH;
	else if( uMonth > MAX_MONTH )
		pTimeControl->Month = MAX_MONTH;
	else
		pTimeControl->Month  = uMonth;
   

    pTimeControl->Week   = uWeek;
    if( uDay > 0 && uDay < 32 )
        pTimeControl->Day    = uDay;

    if( uHour > 12 )
    {
        pTimeControl->fPM  = TRUE;
        pTimeControl->Hour = uHour - 12;
    }
    else
    {
        pTimeControl->fPM  = FALSE;                
        pTimeControl->Hour   = uHour;
    }

    if( uMinute <= 60 )
        pTimeControl->Minute = uMinute;
	else
		pTimeControl->Minute = 0;
}

void EQS_API EQC_SetTimeYear(HCONTROL hControl, WORD wYear )
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    if( wYear >= pTimeControl->wMinYear && wYear <= pTimeControl->wMaxYear )
        pTimeControl->Year   = wYear;
}

void EQS_API EQC_SetTimeMonth(HCONTROL hControl, UINT8 uMonth)
{
	TIME_CONTROL_T *pTimeControl;
    

    if( uMonth == 0 || uMonth > 12 )
        return;
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    pTimeControl->Month  = uMonth;
}

void EQS_API EQC_SetTimeWeek(HCONTROL hControl, UINT8 uWeek)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    if( uWeek > 6 )
        return;

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    pTimeControl->Week  = uWeek;
}

void EQS_API EQC_SetTimeDay(HCONTROL hControl, UINT8 uDay)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    if( uDay > 0 && uDay < 32 )
        pTimeControl->Day    = uDay;
}

void EQS_API EQC_SetTimeHour(HCONTROL hControl, UINT8 uHour)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    if( uHour > 24 )
        return;

    if( uHour >= 12 )
    {
        pTimeControl->fPM  = TRUE;
        pTimeControl->Hour = uHour - 12;
    }
    else
    {
        pTimeControl->fPM  = FALSE;                
        pTimeControl->Hour   = uHour;
    }   
}

void EQS_API EQC_SetTimeMinute(HCONTROL hControl, UINT8 uMinute)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    if( uMinute < 60 )
        pTimeControl->Minute    = uMinute;
}



void EQS_API EQC_SetTimePM(HCONTROL hControl)

{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    pTimeControl->fPM = TRUE;

}

void EQS_API EQC_SetTimeAM(HCONTROL hControl)

{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    pTimeControl->fPM = FALSE;

}

void EQS_API EQC_SetTimeYearLimit(HCONTROL hControl, WORD wMinYear, WORD wMaxYear)

{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    pTimeControl->wMaxYear = wMaxYear;
    pTimeControl->wMinYear = wMinYear;

}

void EQS_API EQC_SetTimeLunar(HCONTROL hControl, BOOL fLunar)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);
    pTimeControl->fLunar = fLunar;
    EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, EQS_NA);
}


void EQS_API EQC_GetTime(HCONTROL hControl, WORD*  pwYear,   \
                                            UINT8* puMonth, \
                                            UINT8* puWeek,  \
                                            UINT8* puDay,   \
                                            UINT8* puHour,  \
                                            UINT8* puMinute)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    *pwYear   =  pTimeControl->Year;
    *puMonth  =  pTimeControl->Month;
    *puWeek   =  pTimeControl->Week;
    *puDay    =  pTimeControl->Day;

    if( pTimeControl->fPM == TRUE )
	{
        *puHour   =  pTimeControl->Hour + 12;
	}
    else
	{
        *puHour   =  pTimeControl->Hour;
	}
    *puMinute =  pTimeControl->Minute;
}
WORD  EQS_API EQC_GetTimeYear(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    return pTimeControl->Year;
}
UINT8 EQS_API EQC_GetTimeMonth(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    return pTimeControl->Month;
}
UINT8 EQS_API EQC_GetTimeWeek(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    return pTimeControl->Week;
}
UINT8 EQS_API EQC_GetTimeDay(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    return pTimeControl->Day;
}
UINT8 EQS_API EQC_GetTimeHour(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);
 
	if( pTimeControl->fPM == TRUE)
        return (pTimeControl->Hour + 12 );
    else 
        return pTimeControl->Hour;

}
UINT8 EQS_API EQC_GetTimeMinute(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    

	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);

    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

    return pTimeControl->Minute;
}

BOOL  EQS_API EQC_GetTimeLunar(HCONTROL   hControl)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);
    return pTimeControl->fLunar;
}


BOOL EQS_API EQC_GetTimeEditMode(HCONTROL hControl)
{
	
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);	
    return pTimeControl->fEdit;
}


void EQS_API EQC_SetTimeEditMode(HCONTROL hControl, BOOL fEdit)
{
	
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	EQC_SET_FOCUS(hControl, fEdit);
    pTimeControl->fEdit = fEdit;
}


BOOL EQS_API EQC_GetFirstTime(HCONTROL hControl, UINT8* nHour, UINT8* nMinute)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);


	if( pTimeControl->fPM )
	{
		*nHour   = pTimeControl->Hour + 12;
	}
	else
	{
		*nHour = pTimeControl->Hour;
	}
	
	*nMinute = pTimeControl->Minute;

	return TRUE;
}
BOOL EQS_API EQC_GetSecondTime(HCONTROL hControl, UINT8* nHour, UINT8* nMinute)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);


	if( pTimeControl->fLunar )
	{
		*nHour = pTimeControl->Month + 12;
	}
	else
	{
		*nHour   = pTimeControl->Month;
	}	
	*nMinute = pTimeControl->Day;

	return TRUE;
}

BOOL EQS_API EQC_SetFirstTime(HCONTROL hControl, UINT8  nHour, UINT8  nMinute)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	pTimeControl->Hour   = nHour;
	pTimeControl->Minute = nMinute;

	if( pTimeControl->Hour >= 12 )
	{
		pTimeControl->Hour -= 12;
		pTimeControl->fPM   = TRUE;
	}
	else
	{
		pTimeControl->fPM = FALSE;
	}
	return TRUE;
}
BOOL EQS_API EQC_SetSecondTime(HCONTROL hControl, UINT8  nHour, UINT8  nMinute)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	pTimeControl->Month = nHour;
	pTimeControl->Day   = nMinute;

    if( pTimeControl->Month >= 12 )
	{
		pTimeControl->Month -= 12;
		pTimeControl->fLunar = TRUE;
	}
	else
	{
		pTimeControl->fLunar = FALSE;
	}
	return TRUE;
}

void EQS_API EQC_SetTime2PM(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	pTimeControl->fLunar = TRUE;

}
void EQS_API EQC_SetTime2AM(HCONTROL hControl)
{

	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	pTimeControl->fLunar = FALSE;

}


BOOL  EQS_API EQC_IsTimePM(HCONTROL hControl)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	if( pTimeControl->fPM )
		return TRUE;
	else
		return FALSE;

}
BOOL  EQS_API EQC_IsTime2PM(HCONTROL hControl)
{

	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	if( pTimeControl->fLunar )
		return TRUE;
	else
		return FALSE;
}



void  EQS_API EQC_SetTimeLimit(HCONTROL hControl, BOOL fActive)
{

}
BOOL  EQS_API EQC_GetTimeLimit(HCONTROL hControl)
{
	return TRUE;
}
void  EQS_API EQC_SetTimeUpper(HCONTROL hControl, WORD wYear, UINT8 nMonth, UINT8 nDay, UINT8 nHour, UINT8 nMinute)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	pTimeControl->wMaxYear   = wYear;
	pTimeControl->nMaxMonth  = nMonth;
	pTimeControl->nMaxDay    = nDay;
	pTimeControl->nMaxHour   = nHour;
	pTimeControl->nMaxMinute = nMinute;
}
void  EQS_API EQC_SetTimeLower(HCONTROL hControl, WORD wYear, UINT8 nMonth, UINT8 nDay, UINT8 nHour, UINT8 nMinute)
{
	TIME_CONTROL_T *pTimeControl;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_TIME);
    pTimeControl = (TIME_CONTROL_T *) EQC_GET_INSTANCE(hControl);

	pTimeControl->wMinYear   = wYear;
	pTimeControl->nMinMonth  = nMonth;
	pTimeControl->nMinDay    = nDay;
	pTimeControl->nMinHour   = nHour;
	pTimeControl->nMinMinute = nMinute;
}

