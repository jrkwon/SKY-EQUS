/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_efs.c
**
** Description: 
**     Test for melody, image, GVM read and write.
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-15 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h> /* sprintf */
#include "sky.h"

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

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

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

void SKY_API SKA_MainDeleteAll(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        SKY_DeleteAllDataFile();

        EQS_END();
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainInitDir(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        SKY_InitDataDir();

        EQS_END();
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainImageList(SKY_EVENT_T *pEvent)
{
    STATIC BYTE *paszFileNameList[SKY_MAX_NUM_FILES];
    STATIC HCONTROL hControl;
    INT8 i;
    INT8 nFiles;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        for(i = 0; i < SKY_MAX_NUM_FILES; i++)
        {
            paszFileNameList[i] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);
        }
        nFiles = SKY_GetFileExtList(SKY_IMAGE_DIR, paszFileNameList);
        hControl = EQC_ScrollList(5, 5, 18, 5, paszFileNameList, nFiles, nFiles, 0);
        EQS_SetIOCActive(hControl);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(i = 0; i < SKY_MAX_NUM_FILES; i++)
        {
            SKY_Free(paszFileNameList[i]);
        }
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_F1 :
        EQS_END();
        break;

    case SE_KEY_F2 :
        EQS_DeleteControl(hControl);
        nFiles = SKY_GetFileList(SKY_MELODY_DIR, paszFileNameList);
        hControl = EQC_ScrollList(5, 5, 18, 5, paszFileNameList, nFiles, nFiles, 0);
        EQS_SetIOCActive(hControl);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainMelodyList(SKY_EVENT_T *pEvent)
{
    STATIC BYTE *paszFileNameList[SKY_MAX_NUM_FILES];
    STATIC HCONTROL hControl;
    INT8 i;
    INT8 nFiles;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        for(i = 0; i < SKY_MAX_NUM_FILES; i++)
        {
            paszFileNameList[i] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);
        }
        nFiles = SKY_GetFileExtList(SKY_MELODY_DIR, paszFileNameList);
        hControl = EQC_ScrollList(5, 5, 18, 5, paszFileNameList, nFiles, nFiles, 0);
        EQS_SetIOCActive(hControl);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(i = 0; i < SKY_MAX_NUM_FILES; i++)
        {
            SKY_Free(paszFileNameList[i]);
        }
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_KEY_F1 :
        EQS_END();
        break;

    case SE_KEY_F2 :
        EQS_DeleteControl(hControl);
        nFiles = SKY_GetFileList(SKY_MELODY_DIR, paszFileNameList);
        hControl = EQC_ScrollList(5, 5, 18, 5, paszFileNameList, nFiles, nFiles, 0);
        EQS_SetIOCActive(hControl);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainViewUsage(SKY_EVENT_T *pEvent)
{
    BYTE szInfo[250];

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        sprintf((char*)szInfo, "Total:%ld\nData:%ld\nImage:%ld\nMelody:%ld", 
                               SKY_GetFreeSpace(), SKY_GetFreeSpaceInDataFile(),
                               SKY_GetUsedSpaceInDataFile(SDF_IMAGE),
                               SKY_GetUsedSpaceInDataFile(SDF_MELODY));
        EQC_MessageBox(szInfo, EQS_NA, MBT_OK, EQS_NA);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_KEY_F1 :
        EQS_END();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainWriteFile(SKY_EVENT_T *pEvent)
{
    BYTE szInfo[250];
    BYTE abTest1Data[1024] = "TEST IMAGE DATA 1234567890";
    BYTE abTest2Data[2048] = "TEST MELODY DATA 1234567890";

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        SKY_WriteDataFile(SDF_IMAGE, (BYTE*)"Test1", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_MELODY, (BYTE*)"Test2", 2048, abTest2Data);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_KEY_F1 :
        EQS_END();
        break;

    case SE_KEY_F2 :
        sprintf((char*)szInfo, "TotalFree:%ld\nDataFree:%ld\nImage:%ld\nMelody:%ld", 
                               SKY_GetFreeSpace(), SKY_GetFreeSpaceInDataFile(),
                               SKY_GetUsedSpaceInDataFile(SDF_IMAGE),
                               SKY_GetUsedSpaceInDataFile(SDF_MELODY));
        EQC_MessageBox(szInfo, EQS_NA, MBT_OK, MBT_HELP);

        break;
    
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainReadFile(SKY_EVENT_T *pEvent)
{
    BYTE abTest1Data[1024] = "DUMMY IMAGE DATA";
    BYTE abTest2Data[2048] = "DUMMY MELODY DATA";

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        SKY_ReadDataFile(SDF_IMAGE, (BYTE*)"Test1", 1024, abTest1Data);
        SKY_ReadDataFile(SDF_MELODY, (BYTE*)"Test2", 2048, abTest2Data);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_KEY_F1 :
        EQS_END();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainWriteBlock(SKY_EVENT_T *pEvent)
{
    STATIC HFILE hFile;
    BYTE abTest1Data[1024] = "block1 block1 DUMMY IMAGE DATA";
    BYTE abTest2Data[1024] = "block2 block2 DUMMY IMAGE DATA";
    BYTE abTest3Data[1024] = "block3 block3 DUMMY IMAGE DATA";

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        if(SKY_CreateDataFile(SDF_IMAGE, (BYTE*)"TEST", &hFile) == SDR_OK)
        {
            SKY_WriteDataBlock(hFile, abTest1Data, 1024);
            SKY_WriteDataBlock(hFile, abTest2Data, 1024);
            SKY_WriteDataBlock(hFile, abTest3Data, 1024);

            SKY_CloseDataFile(hFile);
        }
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_KEY_F1 :
        EQS_END();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainReadBlock(SKY_EVENT_T *pEvent)
{
    STATIC HFILE hFile;
    BYTE abTest1Data[1024];
    BYTE abTest2Data[1024];
    BYTE abTest3Data[1024];

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        if(SKY_OpenDataFile(SDF_IMAGE, (BYTE*)"TEST", &hFile, SFT_READ) == SDR_OK)
        {
            SKY_ReadDataBlock(hFile, abTest1Data, 1024);
            SKY_ReadDataBlock(hFile, abTest2Data, 1024);
            SKY_ReadDataBlock(hFile, abTest3Data, 1024);

            SKY_CloseDataFile(hFile);
        }
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_MainMakeTestData(SKY_EVENT_T *pEvent)
{
    BYTE abTest1Data[1024];

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        SKY_WriteDataFile(SDF_IMAGE, (BYTE*)"Test-1", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_IMAGE, (BYTE*)"Test-2", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_IMAGE, (BYTE*)"Test-3", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_IMAGE, (BYTE*)"Test-4", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_IMAGE, (BYTE*)"Test-5", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_IMAGE, (BYTE*)"Test-6", 1024, abTest1Data);

        SKY_WriteDataFile(SDF_MELODY, (BYTE*)"Test-1", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_MELODY, (BYTE*)"Test-2", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_MELODY, (BYTE*)"Test-3", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_MELODY, (BYTE*)"Test-4", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_MELODY, (BYTE*)"Test-5", 1024, abTest1Data);
        SKY_WriteDataFile(SDF_MELODY, (BYTE*)"Test-6", 1024, abTest1Data);

        EQS_END();
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

