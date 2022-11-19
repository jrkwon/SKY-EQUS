/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_sfil.c
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
** 00-12-19 rosa     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SCHL_VERSION_NUM                    2

/* Event */
#define INIT_SCHL_RECORD_TYPE                   SRT_NONE
#define INIT_SCHL_RECORD_START_TIME             0
#define INIT_SCHL_RECORD_END_TIME               0
#define INIT_SCHL_RECORD_DATA                   ""
#define INIT_SCHL_RECORD_RECURSIVE_TIME_TYPE    SERT_NONE
#define INIT_SCHL_RECORD_RECURSIVE_TIME         0
#define INIT_SCHL_RECORD_ALARM_TIME_TYPE        SATT_NONE
#define INIT_SCHL_RECORD_ALARM_TIME             0
#define INIT_SCHL_RECORD_ALARM_END_TIME         0

/* My Holiday */
#define INIT_SCHL_HOLIDAY_TYPE              SHT_NONE
#define INIT_SCHL_HOLIDAY_LUNAR             FALSE
#define INIT_SCHL_HOLIDAY_YEAR              0
#define INIT_SCHL_HOLIDAY_MONTH             0
#define INIT_SCHL_HOLIDAY_DAY               0
#define INIT_SCHL_HOLIDAY_CONSECUTIVE       FALSE
#define INIT_SCHL_HOLIDAY_TITLE             ""



/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

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
SCHL_PRE_CONFIG_T       g_SchlPreConfig[SCHL_MAX_RECORD_COUNT];
SCHL_RECORD_T           g_InitRecord;
SCHL_ALARM_TIME_TYPE_E  g_AlarmOfAnniv;
SCHL_HOLIDAY_CAL_T      g_InitHoliday;

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
static VOID SKY_API sInitScheduler( VOID );
static VOID SKY_API sInitMyHoliday( VOID );

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

VOID SKY_API SKY_InitScheduler(VOID)
{
    INT8            i;
    UINT8           unOldVersion;
    DWORD           dwCount;
    HFILE           hFile;
    SCHL_RECORD_T   l_SchlRecord;

    if(SKY_IsFileOrDirExist(SKY_SCHL_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_SCHL_DIR);
    }
    /*
    ** ***************************************************************************
    ** SCHEDULER
    ** ***************************************************************************
    */
    /* set init value */
    g_InitRecord.Type = INIT_SCHL_RECORD_TYPE;
    g_InitRecord.dwStartTime = INIT_SCHL_RECORD_START_TIME;
    g_InitRecord.dwEndTime = INIT_SCHL_RECORD_END_TIME;
    STRCPY( g_InitRecord.abData, INIT_SCHL_RECORD_DATA );
    g_InitRecord.Recursive.Type = INIT_SCHL_RECORD_RECURSIVE_TIME_TYPE;
    g_InitRecord.Recursive.dwEndTime = INIT_SCHL_RECORD_RECURSIVE_TIME;
    g_InitRecord.Alarm.Type = INIT_SCHL_RECORD_ALARM_TIME_TYPE;
    g_InitRecord.Alarm.dwTime = INIT_SCHL_RECORD_ALARM_TIME;
    g_InitRecord.Alarm.dwEndTime = INIT_SCHL_RECORD_ALARM_END_TIME;

    /* open the file */
    hFile = SKY_OpenFile(FILE_NAME_SCHL, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        /* set the initial value */
        sInitScheduler();
    }
    else
    {
        /* check VERSION NUMBER */
        SKY_ReadFile(hFile, &(unOldVersion), sizeof(UINT8));
        if(unOldVersion != SCHL_VERSION_NUM)
        {
            SKY_CloseFile(hFile);
            SKY_DeleteFile(FILE_NAME_SCHL);
            sInitScheduler();
            
        }
        else
        {
            /* if file is already exist, read the data into the global buffer */
            for(i=0; i<SCHL_MAX_RECORD_COUNT; i++)
            {
                dwCount = SKY_ReadFile(hFile, &(l_SchlRecord), sizeof(l_SchlRecord));
                g_SchlPreConfig[i].Type = l_SchlRecord.Type;
                g_SchlPreConfig[i].dwStartTime = l_SchlRecord.dwStartTime;
                g_SchlPreConfig[i].Recursive.Type = l_SchlRecord.Recursive.Type;
                g_SchlPreConfig[i].Recursive.dwEndTime = l_SchlRecord.Recursive.dwEndTime;
                g_SchlPreConfig[i].Alarm.Type = l_SchlRecord.Alarm.Type;
                g_SchlPreConfig[i].Alarm.dwTime = l_SchlRecord.Alarm.dwTime;
                g_SchlPreConfig[i].Alarm.dwEndTime = l_SchlRecord.Alarm.dwEndTime;

                g_AlarmOfAnniv = l_SchlRecord.Alarm.Type;
            }
            /* close the file */
            SKY_CloseFile(hFile);
        }

    }


    /*
    ** ***************************************************************************
    ** MY HOLIDAY
    ** ***************************************************************************
    */
    /* set init value */
    g_InitHoliday.Type = INIT_SCHL_HOLIDAY_TYPE;
    g_InitHoliday.fLunar = INIT_SCHL_HOLIDAY_LUNAR;
    g_InitHoliday.wYear = INIT_SCHL_HOLIDAY_YEAR;
    g_InitHoliday.unMonth = INIT_SCHL_HOLIDAY_MONTH;
    g_InitHoliday.unDay = INIT_SCHL_HOLIDAY_DAY;
    g_InitHoliday.fConsecutive = INIT_SCHL_HOLIDAY_CONSECUTIVE;
    STRCPY( g_InitHoliday.pszName, INIT_SCHL_HOLIDAY_TITLE );

    /* open the file */
    hFile = SKY_OpenFile(FILE_NAME_HLDY, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        /* set the initial value */
        sInitMyHoliday();
    }
    else
    {
        SKY_CloseFile(hFile);
    }

	Init_Schedule_Block(); // init block data

}

VOID SKY_API sInitScheduler( VOID )
{
    UINT8           unVersionNum;
    INT8            i;
    HFILE           hFile;
    DWORD           dwCount;
    DWORD           dwSetTime;
    clk_julian_type cjSetTime;
    SCHL_RECORD_T   l_SchlRecord;

    /* there is not the file, create the file */
    hFile = SKY_OpenFile(FILE_NAME_SCHL, FS_AM_CREATE);

    /* save VERSION NUMBER */
    unVersionNum = SCHL_VERSION_NUM;
    SKY_WriteFile(hFile, &unVersionNum, sizeof(UINT8));

    /* save the init record */
    for(i=0; i<SCHL_MAX_RECORD_COUNT; i++) //101
    {
        if( i == SCHL_IDX_ANNIV_ALARM )
        {
            dwSetTime = SKY_GetLocalTime();
            clk_secs_to_julian( dwSetTime, &cjSetTime );
            cjSetTime.hour = 9;
            cjSetTime.minute = 0;
            cjSetTime.second = 0;
            dwSetTime = clk_julian_to_secs( &cjSetTime );
            
            /* set init value for ANNIV */
            l_SchlRecord.Type = SRT_ANNIV;
            l_SchlRecord.dwStartTime = INIT_SCHL_RECORD_START_TIME;
            l_SchlRecord.dwEndTime = INIT_SCHL_RECORD_END_TIME;
            STRCPY( l_SchlRecord.abData, INIT_SCHL_RECORD_DATA );
            l_SchlRecord.Recursive.Type = INIT_SCHL_RECORD_RECURSIVE_TIME_TYPE;
            l_SchlRecord.Recursive.dwEndTime = INIT_SCHL_RECORD_RECURSIVE_TIME;
            l_SchlRecord.Alarm.Type = SATT_SET_ANYTIME; /* default value */
            l_SchlRecord.Alarm.dwTime = dwSetTime;
            l_SchlRecord.Alarm.dwEndTime = dwSetTime;

            /* set file */
            dwCount = SKY_WriteFile(hFile, &(l_SchlRecord), sizeof(SCHL_RECORD_T));

            /* set PreConfig */
            g_SchlPreConfig[i].Type = l_SchlRecord.Type;
            g_SchlPreConfig[i].dwStartTime = l_SchlRecord.dwStartTime;
            g_SchlPreConfig[i].Recursive.Type = l_SchlRecord.Recursive.Type;
            g_SchlPreConfig[i].Recursive.dwEndTime = l_SchlRecord.Recursive.dwEndTime;
            g_SchlPreConfig[i].Alarm.Type = l_SchlRecord.Alarm.Type;
            g_SchlPreConfig[i].Alarm.dwTime = l_SchlRecord.Alarm.dwTime;
            g_SchlPreConfig[i].Alarm.dwEndTime = l_SchlRecord.Alarm.dwEndTime;

            g_AlarmOfAnniv = l_SchlRecord.Alarm.Type;
        }
        else
        {
            /* set file */
            dwCount = SKY_WriteFile(hFile, &(g_InitRecord), sizeof(SCHL_RECORD_T));
    
            /* set PreConfig */
            g_SchlPreConfig[i].Type = g_InitRecord.Type;
            g_SchlPreConfig[i].dwStartTime = g_InitRecord.dwStartTime;
            g_SchlPreConfig[i].Recursive.Type = g_InitRecord.Recursive.Type;
            g_SchlPreConfig[i].Recursive.dwEndTime = g_InitRecord.Recursive.dwEndTime;
            g_SchlPreConfig[i].Alarm.Type = g_InitRecord.Alarm.Type;
            g_SchlPreConfig[i].Alarm.dwTime = g_InitRecord.Alarm.dwTime;
            g_SchlPreConfig[i].Alarm.dwEndTime = g_InitRecord.Alarm.dwEndTime;
        }
    }
    
    SKY_CloseFile(hFile);
}

static  VOID SKY_API sInitMyHoliday( VOID )
{
    INT8 i;
    HFILE hFile;
    DWORD dwCount;

    hFile = SKY_OpenFile(FILE_NAME_HLDY, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_HLDY, FS_AM_CREATE);
    }    

    for( i=0; i<SCHL_MAX_COUNT_MY_HOLIDAY; i++) //50
    {
        dwCount = SKY_WriteFile(hFile, &(g_InitHoliday), sizeof(SCHL_HOLIDAY_CAL_T));
    }    
    SKY_CloseFile(hFile);
}

VOID SKY_SeekReadSchlT( UINT8 unIdxOfSchl, SCHL_RECORD_T* pReadRecord )
{
    HFILE   hFile;
    DWORD   dwCount;

    hFile = SKY_OpenFile(FILE_NAME_SCHL, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    dwCount = SKY_SeekReadFile( hFile, 
                                FS_SO_SET, 
                                sizeof(UINT8)+(unIdxOfSchl*sizeof(SCHL_RECORD_T)), 
                                pReadRecord, 
                                sizeof(SCHL_RECORD_T) );
    SKY_CloseFile( hFile );
}

VOID SKY_SeekWriteSchlT( UINT8 unIdxOfSchl, SCHL_RECORD_T* pWriteRecord )
{
    HFILE hFile;
    DWORD dwCount;

    hFile = SKY_OpenFile(FILE_NAME_SCHL, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        hFile = SKY_OpenFile(FILE_NAME_SCHL, FS_AM_CREATE);
    } 
    dwCount = SKY_SeekWriteFile( hFile, 
                                 FS_SO_SET, 
                                 sizeof(UINT8)+(unIdxOfSchl*sizeof(SCHL_RECORD_T)),
                                 pWriteRecord, 
                                 sizeof(SCHL_RECORD_T) );
    SKY_CloseFile( hFile );
}



#if 0 /* 00000000000000000000000000000000000000000000000000000000000000000 */
VOID SKY_API SKY_WriteScheduler( VOID )
{
    /*
    HFILE hFile;
    DWORD dwCount;

    hFile = SKY_OpenFile(FILE_NAME_SCHL, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        hFile = SKY_OpenFile(FILE_NAME_SCHL, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    
    dwCount = SKY_WriteFile(hFile, &(l_SchlRecord), 
                            sizeof(l_SchlRecord));
    
    SKY_ASSERT(dwCount == sizeof(l_SchlRecord));
    
    SKY_CloseFile(hFile);
    */
}
#endif /* 00000000000000000000000000000000000000000000000000000000000000000 */