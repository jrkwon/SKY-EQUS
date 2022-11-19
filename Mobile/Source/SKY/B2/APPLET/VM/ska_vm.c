/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_vm.c
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
** 2001-03-19 bbyes       Created.
**  
** ===========================================================================
*/
//#ifdef FEATURE_SKTT_PM
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include <stdlib.h>
#include "ska.h"
#include "uisockapi.h"
#include "dsm.h"

enum {
	BMP = 1,    //BMP_DATA
	CNT = 2,    //CONTENT_DATA
	TRS = 3     //TRANS_DATA
};



/* 
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
//#define deco_photo
#define BMP_256COLOR
//#define BMP_24BIT

#define SKY_GET_HOST()           "mms.nate.com"
#define HTTP_PORT                80

#define PM_EDIT_SIZE             16
#define PM_MESSAGE_DEFAULT_TIME  2

//#define sMAX_PHOTO_COUNT         50

#define EMAIL_FRAME_X   4
#define EMAIL_FRAME_Y   94

#define PM_ANI_X                 29
#define PM_ANI_Y                 39

#define PM_PANNEL_X              4
#define PM_PANNEL_Y              94

#define PM_STATUS_X              8
#define PM_STATUS_Y              109

#define ONEPACK_SIZE             528
#define ONEPACK_SEND_TIME        250
#define PM_WAIT_TIME             20000 //20sec   

#define BOUND_HEAD              "SKTT_BOUND\r\n"
#define BOUND_BODY               "--SKTT_BOUND\r\n" 
#define BOUND_END               "--SKTT_BOUND--\r\n"
#define HTTP_USER_AGENT         "SKTeletech/1.0 IM-3100"

#ifdef BMP_256COLOR
    #define BMP_HEADER_SIZE         (sizeof(BMPFILEHEADER)+sizeof(BMPINFOHEADER)+(sizeof(RGBQUAD)*256))
#else   //BMP_24BIT
    #define BMP_HEADER_SIZE         (sizeof(BMPFILEHEADER)+sizeof(BMPINFOHEADER))
#endif

#define PHOTO_TOTAL_SIZE                10800*2//6912
#define COLOR_IDLE_IMAGE_BPP            16      
#define NEED_MIN_SPACE                  sizeof(DWORD) + SKY_MAX_CAMERA_IMAGE_FILE_LEN + PHOTO_TOTAL_SIZE + sizeof(BitmapCameraInfo.Header)
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
PM_IMI_RECORD_T         g_PMList[PM_MAX_NUM_PM];
STATIC UINT             g_NumOfPMList;
STATIC UINT		        g_unNumOfPM;
STATIC DWORD            g_SendPM;
STATIC BYTE             g_SendState;
STATIC INT8             g_MenuSize;
STATIC HCONTROL         g_hHnd;
STATIC DWORD            g_filename;
STATIC BOOL             g_InPhotoName;
PM_RECORD_T             PMMsg;
BYTE                    FILENAMEPM[SKY_FILE_EXT_NAME_MAX_LENGTH + 1];
BYTE *                  l_paViewBufPM[PM_MAX_NUM_PM+1];
STATIC BYTE             l_PhotoName[SKY_FILE_EXT_NAME_MAX_LENGTH+1];    //SKY_FILE_EXT_NAME_MAX_LENGTH = 18
STATIC BYTE*            pTRANS_DATA = NULL;
BYTE                    g_PhoneNumBuf[MAX_DIAL_DIGIT_LENGTH+1];
DWORD                   l_PMStartTime;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
enum tag_SKA_EDIT_PM_E {
    EP_PHOTO=0,
    EP_STATIC_TEXT_1,
    EP_TEXT_EDIT_1,
    EP_STATIC_TEXT_2,
    EP_NUM_EDIT_1,
    EP_NUM_EDIT_2,
    EP_NUM_EDIT_3,
    EP_NUM_EDIT_4,
    EP_NUM_EDIT_5,
    EP_EDIT_MAX
};

enum tag_SKA_VP_PM_E {
    VP_PHOTO=0,
    VP_STATIC_TIME_1,
    VP_STATIC_TIME_2,
    VP_STATIC_TEXT_1,
    VP_TEXT_EDIT_1,
    VP_STATIC_TEXT_2,
    VP_NUM_EDIT_1,
    VP_NUM_EDIT_2,
    VP_NUM_EDIT_3,
    VP_NUM_EDIT_4,
    VP_NUM_EDIT_5,
    VP_EDIT_MAX
};
enum tag_SKA_RP_PM_E {
    RP_PHOTO = 0,
    RP_STATIC_TEXT_1,
    RP_TEXT_EDIT_1,
    RP_STATIC_TEXT_2,
    RP_NUM_EDIT_1,
    RP_NUM_EDIT_2,
    RP_NUM_EDIT_3,
    RP_NUM_EDIT_4,
    RP_NUM_EDIT_5,
    RP_EDIT_MAX
};

enum tagEP_MESSAGE_E{
    EP_MES_E_1=1,           
    EP_MES_E_2,             
    EP_MES_E_3,             
    EP_MES_E_4,             
    EP_MES_E_5,             
    EP_MES_E_6,             
    EP_MES_E_7,             
    EP_MES_E_8,             
    EP_MES_E_10,            
    EP_MES_E_12,            
    EP_MES_E_13,            
    EP_MES_E_14,             
    EP_MES_E_15
};
enum tagRP_MESSAGE_E{
    RP_MES_E_1=1,          
    RP_MES_E_2,            
    RP_MES_E_3,             
    RP_MES_E_4,              
    RP_MES_E_5,              
    RP_MES_E_7,
    RP_MES_E_8,
    RP_MES_E_9,
    RP_MES_E_10,
    RP_MES_E_11,
    RP_MES_E_12
};

enum tagVP_MESSAGE_E{
    VP_MES_E_1=1,         
    VP_MES_E_2,            
    VP_MES_E_3,            
    VP_MES_E_4,          
    VP_MES_E_5,
    VP_MES_E_6,
    VP_MES_E_7
};

enum tagENP_MESSAGE_E{
    ENP_MES_E_1=1,
    ENP_MES_E_2=2
};

enum tagPNS_MESSAGE_E{
    PNS_MES_E_1=1,
    PNS_MES_E_2
};

enum tagMP_MESSAGE_E{
    MP_MES_E_1=1,
    MP_MES_E_2,
    MP_MES_E_4,
    MP_MES_E_5,
    MP_MES_E_6,
    MP_MES_E_7,
    MP_MES_E_8
};

enum tagPM_MSG_E{
    PM_SUCCESS=1,
    PM_FAIL,
    PM_CONNECT_FAIL,
    PM_SYSTEM_TROUBLE,
    PM_SERVER_TROUBLE,
    PM_TRANS_CANCEL
};

enum tagPM_STATE_MEG_E{
    PM_NOSERVICE=1,
    PM_MAINTREQ,
    PM_FAIL_ORIGINATED,
    PM_SUCC_ORIGINATED
};
#ifdef email_version
#ifdef deco_photo
STATIC BOOL pabRedactPMMenu[6] = { FALSE,FALSE, FALSE, FALSE, FALSE ,FALSE}; 

SKY_TOKEN paRedactPMMenu[6] =
{   
    TKN_PM_SAVE_PM,             //포토메일저장
    TKN_PM_DECO_PHOTO,          //사진꾸미기
    TKN_PM_PBOOK_SEARCH,		//전화부찾기
    TKN_PM_CALLHISTORY_CALLTO,  //발신이력찾기
    TKN_PM_CALLHISTORY_CALLED,   //착신이력찾기
    TKN_PM_PB_EMAIL_SEARCH      //상대주소 찾기
};

STATIC BOOL pabEditPMMenu[7] = { FALSE,FALSE, FALSE, FALSE, FALSE, FALSE,FALSE }; 

SKY_TOKEN paEditPMMenu[7] =
{   
    TKN_PM_SAVE_PM,             //포토메일저장
    TKN_PM_DECO_PHOTO,          //사진꾸미기
    TKN_PM_INPUT_PHOTO_NAME,    //사진이름지정
    TKN_PM_PBOOK_SEARCH,		//전화부찾기
    TKN_PM_CALLHISTORY_CALLTO,  //발신이력찾기
    TKN_PM_CALLHISTORY_CALLED,   //착신이력찾기
    TKN_PM_PB_EMAIL_SEARCH      //상대주소 찾기
};
#else //deco_photo
STATIC BOOL pabRedactPMMenu[5] = { FALSE,FALSE, FALSE, FALSE ,FALSE}; 

SKY_TOKEN paRedactPMMenu[5] =
{   
    TKN_PM_SAVE_PM,             //포토메일저장
    TKN_PM_PBOOK_SEARCH,		//전화부찾기
    TKN_PM_CALLHISTORY_CALLTO,  //발신이력찾기
    TKN_PM_CALLHISTORY_CALLED,   //착신이력찾기
    TKN_PM_PB_EMAIL_SEARCH      //상대주소 찾기
};

STATIC BOOL pabEditPMMenu[6] = { FALSE,FALSE, FALSE, FALSE, FALSE,FALSE }; 

SKY_TOKEN paEditPMMenu[6] =
{   
    TKN_PM_SAVE_PM,             //포토메일저장
    TKN_PM_INPUT_PHOTO_NAME,    //사진이름지정
    TKN_PM_PBOOK_SEARCH,		//전화부찾기
    TKN_PM_CALLHISTORY_CALLTO,  //발신이력찾기
    TKN_PM_CALLHISTORY_CALLED,   //착신이력찾기
    TKN_PM_PB_EMAIL_SEARCH      //상대주소 찾기
};
#endif//deco_photo
#else //email_version
#ifdef deco_photo
STATIC BOOL pabRedactPMMenu[5] = { FALSE,FALSE, FALSE, FALSE, FALSE }; 

SKY_TOKEN paRedactPMMenu[5] =
{   
    TKN_PM_SAVE_PM,             //포토메일저장
    TKN_PM_DECO_PHOTO,          //사진꾸미기
    TKN_PM_PBOOK_SEARCH,		//전화부찾기
    TKN_PM_CALLHISTORY_CALLTO,  //발신이력찾기
    TKN_PM_CALLHISTORY_CALLED   //착신이력찾기
};

STATIC BOOL pabEditPMMenu[6] = { FALSE,FALSE, FALSE, FALSE, FALSE, FALSE }; 

SKY_TOKEN paEditPMMenu[6] =
{   
    TKN_PM_SAVE_PM,             //포토메일저장
    TKN_PM_DECO_PHOTO,          //사진꾸미기
    TKN_PM_INPUT_PHOTO_NAME,    //사진이름지정
    TKN_PM_PBOOK_SEARCH,		//전화부찾기
    TKN_PM_CALLHISTORY_CALLTO,  //발신이력찾기
    TKN_PM_CALLHISTORY_CALLED   //착신이력찾기
};
#else //deco_photo
STATIC BOOL pabRedactPMMenu[4] = { FALSE,FALSE, FALSE, FALSE }; 

SKY_TOKEN paRedactPMMenu[4] =
{   
    TKN_PM_SAVE_PM,             //포토메일저장
    TKN_PM_PBOOK_SEARCH,		//전화부찾기
    TKN_PM_CALLHISTORY_CALLTO,  //발신이력찾기
    TKN_PM_CALLHISTORY_CALLED   //착신이력찾기
};

STATIC BOOL pabEditPMMenu[5] = { FALSE,FALSE, FALSE, FALSE, FALSE }; 

SKY_TOKEN paEditPMMenu[5] =
{   
    TKN_PM_SAVE_PM,             //포토메일저장
    TKN_PM_INPUT_PHOTO_NAME,    //사진이름지정
    TKN_PM_PBOOK_SEARCH,		//전화부찾기
    TKN_PM_CALLHISTORY_CALLTO,  //발신이력찾기
    TKN_PM_CALLHISTORY_CALLED   //착신이력찾기
};
#endif//deco_photo
#endif//email_version


SKY_TOKEN paMainPMMenu[2] =
{
    TKN_PM_DELETE,	        	//삭제
	TKN_PM_DEL_ALL_PM           //전부삭제
};

LOCAL BITMAP BitmapCameraInfo = {
    { EQUS_IMAGE_ID, { PHOTO_H_SIZE, PHOTO_V_SIZE}, 
      PHOTO_H_SIZE, SKY_COLOR_SPACE, FALSE, MAX_COLOR, 0}
};

#ifdef WIN32
LOCAL BYTE s_aCameraRevData[PHOTO_TOTAL_SIZE/2];
#else
LOCAL BYTE *s_aCameraRevData;
#endif

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
extern BYTE* SeekWorkingArea(INT);
#ifndef WIN32
extern VOID AddressAssign(VOID);
#endif

VOID SKY_InitPMMsg(PM_RECORD_T*);
VOID SKY_InitPM(VOID);
VOID SKY_DeletePM(UINT16);
VOID SKY_SavePM (VOID);
VOID SKY_ReadPM(UINT16); 
VOID SKY_DeleteAllPM(VOID);
VOID SKY_ReadNWritePM(UINT16);
BYTE** SKY_MakeScrollListPM(VOID);
VOID SKY_FreePMScrollList(VOID);
VOID SKY_DeletePMPhoto(DWORD);
HFILE SKY_MakePMFile(VOID);
VOID SKY_PMListUP(VOID);

VOID SKY_API SKA_RedactPM(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_ViewPM(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_PhotoNameSave(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_DecoPhoto(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_PMOriginated(SKY_EVENT_T *pInEvent);
LOCAL VOID SKY_API sSKA_PMHTTPInProg(SKY_EVENT_T *pEvent);

WORD SKY_PrepareTran(DWORD,UINT8);
UINT16 SKY_SendPM(BYTE*,UINT16);
BOOL SKY_ReceiveHTTPReply(VOID);
VOID SKY_ConnectHTTPServer(VOID);
VOID SKY_DisconnectHTTPServer(VOID);

VOID SKY_InitPMMsg(PM_RECORD_T* initPM){

        initPM->Sent=FALSE;
		initPM->MIN_NUM = 0;
        initPM->DecoNum = 0;
		initPM->PMPhoto = 0;
        initPM->Send_time = 0;
        initPM->Save_time =0;
		memset(initPM->PMText, NULL, PM_MAX_SIZE_TEXT+2);
		memset(initPM->MIN, NULL, PM_MAX_NUM_MIN*(PM_MAX_SIZE_MIN+1));		
		
}

HFILE SKY_MakePMFile(VOID)
{
    BYTE  szFileNumber[SKY_MAX_CAMERA_IMAGE_FILE_LEN+1];//6]; 
    HFILE hFile;
    SKY_DATA_FILE_RESULT_E  Result;
    
    memset(szFileNumber,NULL,SKY_MAX_CAMERA_IMAGE_FILE_LEN+1);   
    sprintf((BYTE *)szFileNumber,"%lu",SKY_GET_PM_COUNT());
    Result = SKY_CreateDataFile(SDF_VM, szFileNumber, &hFile);
    if(Result == SDR_OK)
    {   
         g_filename = SKY_GET_PM_COUNT();
           
        if(SKY_GET_PM_COUNT()==PM_MAX_NUM_PM)
	    {
		    SKY_INIT_PM_COUNT(1);
        }
        else
	    {
            SKY_SET_PM_COUNT();
	    }		
        return hFile;
    }else if(Result == SDR_FILE_EXIST)
    {
        do
        {   
	        if(SKY_GET_PM_COUNT()==PM_MAX_NUM_PM)
	        {
		        SKY_INIT_PM_COUNT(1);
            }
            else
	        {
                SKY_SET_PM_COUNT();
	        }
            memset(szFileNumber,NULL,SKY_MAX_CAMERA_IMAGE_FILE_LEN+1); 
            sprintf((BYTE *)szFileNumber,"%lu",SKY_GET_PM_COUNT());
            Result = SKY_CreateDataFile(SDF_VM, szFileNumber, &hFile);
        }while(Result == SDR_FILE_EXIST);
        
        if(Result == SDR_OK)
        {   
            g_filename = SKY_GET_PM_COUNT();
                   
            if(SKY_GET_PM_COUNT()==PM_MAX_NUM_PM)
	        {
		        SKY_INIT_PM_COUNT(1);
            }
            else
	        {
                SKY_SET_PM_COUNT();
	        }		
            return hFile;
        }
        else
            return SKY_INVALID_FILE_HANDLE;
    }
    else
        return SKY_INVALID_FILE_HANDLE;

}


VOID SKY_SavePM(VOID){
	
    HFILE	hFile;
    UINT    i;
    BYTE    szFileNumber[SKY_MAX_CAMERA_IMAGE_FILE_LEN+1];
       
    if(g_unNumOfPM < PM_MAX_NUM_PM) 
	{
        hFile = SKY_MakePMFile();
        if(hFile != SKY_INVALID_FILE_HANDLE)
        {
            SKY_WriteDataBlock(hFile, (VOID *)&PMMsg, (DWORD)(sizeof(PM_RECORD_T)));
            SKY_CloseDataFile(hFile);
            //SKY_InitPM();
        }
        // Change List
        for(i=0 ; i<PM_MAX_NUM_PM;i++)
        { 
            if(g_PMList[i].fExist != TRUE)
            {
                g_PMList[i].Save_time = PMMsg.Save_time;
                g_PMList[i].Sent = PMMsg.Sent;
                g_PMList[i].fExist = TRUE;
                memset(szFileNumber,NULL,SKY_MAX_CAMERA_IMAGE_FILE_LEN+1);
                sprintf((BYTE *)szFileNumber,"%lu",g_filename);
                STRCPY(g_PMList[i].nAddr,szFileNumber);
                STRCPY(g_PMList[i].PMTitle, PMMsg.PMTitle);

                i = PM_MAX_NUM_PM;
            }
        }       
        hFile = SKY_OpenFile(FILE_NAME_PM, FS_AM_READWRITE);
        SKY_WriteFile(hFile, (CONST void *)g_PMList, 
                (DWORD)(sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM));
        SKY_CloseFile(hFile);
    }

}

VOID SKY_DeletePM(UINT16 delIndex){

    HFILE	hFile;

    STRCPY((BYTE*)FILENAMEPM, (BYTE*)g_PMList[delIndex].nAddr);

    if(g_unNumOfPM >0 )
    {
        if(SKY_IsDataFile(SDF_VM, (BYTE*)FILENAMEPM ))
        {
            SKY_DeleteDataFile(SDF_VM,(BYTE*)FILENAMEPM);
            //SKY_InitPM();
        }
    }
    // Change List
    if(delIndex !=29)
    {
        g_PMList[delIndex] = g_PMList[g_unNumOfPM-1];
        memset(&g_PMList[g_unNumOfPM-1],NULL,sizeof(PM_IMI_RECORD_T));
    }
    else
    {
        memset(&g_PMList[delIndex],NULL,sizeof(PM_IMI_RECORD_T));
    }

    hFile = SKY_OpenFile(FILE_NAME_PM, FS_AM_READWRITE);
    SKY_WriteFile(hFile, (CONST void *)g_PMList, 
            (DWORD)(sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM));
    SKY_CloseFile(hFile);
 
}

VOID SKY_DeleteAllPM(VOID){
    
    HFILE	hFile;

    SKY_DeleteDataFiles(SDF_VM);
    SKY_INIT_PM_COUNT(1);

    hFile = SKY_OpenFile(FILE_NAME_PM, FS_AM_READWRITE);

    memset(g_PMList, NULL, sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM);
    
    SKY_WriteFile(hFile, (CONST void *)g_PMList , 
            (DWORD)(sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM));

    SKY_CloseFile(hFile);
   
}


VOID SKY_ReadPM(UINT16 index){
	
    HFILE	hFile;

    SKY_InitPMMsg(&PMMsg);
    
    STRCPY((BYTE*)FILENAMEPM, (BYTE*)g_PMList[index].nAddr);
 
    if(SKY_IsDataFile(SDF_VM, (BYTE*)FILENAMEPM))
    {
        SKY_OpenDataFile(SDF_VM, (BYTE*)FILENAMEPM, &hFile, SFT_READ);
        SKY_ReadDataBlock(hFile, (VOID *)&PMMsg, (DWORD)(sizeof(PM_RECORD_T)));
        SKY_CloseDataFile(hFile);
    }
   
}
VOID SKY_ReadNWritePM(UINT16 index){
	
    HFILE	hFile;
   
    SKY_InitPMMsg(&PMMsg);
    
    STRCPY((BYTE*)FILENAMEPM, (BYTE*)g_PMList[index].nAddr);

    if(SKY_IsDataFile(SDF_VM, (BYTE*)FILENAMEPM))
    {
        SKY_OpenDataFile(SDF_VM, (BYTE*)FILENAMEPM, &hFile, SFT_WRITE);
        SKY_ReadDataBlock(hFile, (VOID *)&PMMsg, (DWORD)(sizeof(PM_RECORD_T)));
        if(!PMMsg.Sent)
            PMMsg.Sent=TRUE;
        PMMsg.Send_time=SKY_GetLocalTime();
        PMMsg.Save_time=SKY_GetLocalTime();
        
        SKY_SeekFile(hFile, FS_SO_SET, 0);
        SKY_WriteDataBlock(hFile, (VOID *)&PMMsg, (DWORD)(sizeof(PM_RECORD_T)));
        SKY_CloseDataFile(hFile);
    }

    // Change List
    
    g_PMList[index].Save_time = PMMsg.Save_time;
    g_PMList[index].Sent = PMMsg.Sent;
    g_PMList[index].fExist = TRUE;

    hFile = SKY_OpenFile(FILE_NAME_PM, FS_AM_READWRITE);
    SKY_WriteFile(hFile, (CONST void *)g_PMList, 
                (DWORD)(sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM));
    SKY_CloseFile(hFile);
    
}

VOID SKY_InitPM(VOID)
{   
    HFILE   hFile,nFiles;
    DWORD   AvailMem;
    UINT8   i,j;
    PM_IMI_RECORD_T         PMList[PM_MAX_NUM_PM];
        
    g_unNumOfPM = 0;

    if(!SKY_IsFileOrDirExist(FILE_NAME_PM))
    {   
              
        // Create file
        hFile = SKY_OpenFile(FILE_NAME_PM, FS_AM_CREATE);

        memset(g_PMList, NULL, sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM);
        
        SKY_WriteFile(hFile, (CONST void *)g_PMList , 
                (DWORD)(sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM));

        SKY_CloseFile(hFile);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_PM, FS_AM_READWRITE);

        SKY_ReadFile(hFile, (void *)PMList, 
                (DWORD)(sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM));
        
        g_unNumOfPM =0 ;
        memset(g_PMList, NULL, sizeof(PM_IMI_RECORD_T)*PM_MAX_NUM_PM);
        
        for(i = 0,j = 0; i < PM_MAX_NUM_PM; i++)
        {
            if(PMList[i].fExist == TRUE)
            {  
                g_PMList[j]=PMList[i];
                g_unNumOfPM++;
                j++;
            }
        }

        SKY_CloseFile(hFile);
    }
}

BYTE** SKY_MakeScrollListPM()
{
    
    UINT        i,j;
    PM_IMI_RECORD_T  Temp;
    
    SKY_InitPM();
 
    //Sorting
    if(g_unNumOfPM != 0)
    {
	    for (i = 0; i < g_unNumOfPM - 1; i++) 
        {
		    for (j = i + 1; j < g_unNumOfPM; j++) 
            {
			    if (g_PMList[i].Save_time < g_PMList[j].Save_time) 
                {   
                    Temp = g_PMList[i];
                    g_PMList[i] = g_PMList[j];
                    g_PMList[j] = Temp;
                    
			    }
		    }
        } 
    }      
    
    for( i = 0 ; i < g_unNumOfPM; i++)
    {
         l_paViewBufPM[i] = (BYTE *)SKY_Calloc(LCD_COLUME_SIZE-1, sizeof(BYTE)); 
         if(g_PMList[i].Sent==TRUE)
         {
            STRCPY(l_paViewBufPM[i],(BYTE*)SKY_GET_TOKEN(TKN_PM_SENT_IMAGE));
            STRCAT(l_paViewBufPM[i], g_PMList[i].PMTitle);
         }
         else
         {
            STRCPY(l_paViewBufPM[i],(BYTE*)SKY_GET_TOKEN(TKN_PM_SAVE_IMAGE));
            STRCAT(l_paViewBufPM[i], g_PMList[i].PMTitle);
         }
    }
    g_NumOfPMList=g_unNumOfPM;
          
    return l_paViewBufPM;        
}

VOID SKY_FreePMScrollList(VOID)
{
    UINT   i;
    for( i = 0; i < g_NumOfPMList; i++ )
    {
        SKY_Free(l_paViewBufPM[i]); 
    }        
}

VOID SKY_DeletePMPhoto(DWORD dwIndex)
{	
	HFILE	hFile;
    PM_RECORD_T  pTemp;
	BYTE        i;

	for (i = 1; i <= SKY_GET_PM_COUNT() ; i++) 
    {
        STRCPY((BYTE *)FILENAMEPM,(BYTE*)&i);
        if(SKY_IsDataFile(SDF_VM, (BYTE*)FILENAMEPM))
        {   
            SKY_OpenDataFile(SDF_VM, (BYTE*)FILENAMEPM, &hFile, SFT_WRITE);
            SKY_ReadDataBlock(hFile, (VOID *)&pTemp, (DWORD)(sizeof(PM_RECORD_T)));
			if(pTemp.PMPhoto == dwIndex)
			{	
				pTemp.PMPhoto = 0;		//포토메일에 기억된 사진정보를 지움
				SKY_SeekFile(hFile, FS_SO_SET, 0);
				SKY_WriteDataBlock(hFile, (VOID *)&pTemp, (DWORD)(sizeof(PM_RECORD_T)));
			}
			SKY_CloseDataFile(hFile);
        }
    }
}


WORD SKY_PrepareTran(DWORD index,  UINT8 decoNum)
{
    BMPINFO bmp;
    BMPFILEHEADER bmpFH;
     
    HFILE	hFile;
    BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
    DWORD   dwTimeInfo;
    BYTE * sGarbage;
    INT i,j;
    
    UINT16 TRAN_DATA_SIZE=0;
    UINT16 CONTENT_SIZE=0;
    UINT16 BMP_SIZE=0;

    BYTE* pBMP= NULL;
    BYTE* pCONTENT_DATA = NULL;

    WORD tmpEbi1Pixel;
    BYTE tmp24Bmp1Pixel[3];
#ifdef BMP_256COLOR
    BYTE tmp256Bmp;
#endif
#ifdef BMP_24BIT
    INT k;
#endif
   
	//ebi 파일 읽기
    sGarbage = (BYTE *)SKY_Calloc(1,CAMERA_BUF_SPACE);
	ConvertCameraImageFileName(s_psFileName, index); 
        if(SKY_OpenDataFile(SDF_PHOTO, s_psFileName, &hFile, SFT_READ) == SDR_OK)
        {
			SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
            SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
            SKY_ReadDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header)); 
            //SKY_ReadDataBlock(hFile, sGarbage, CAMERA_BUF_SPACE);    
            SKY_ReadDataBlock(hFile, g_abBmpBuf, PHOTO_TOTAL_SIZE);   
            //BitmapCameraInfo.pbData = g_abBmpBuf;  // woodstok 01.4.8
            SKY_CloseDataFile(hFile);
		}
    SKY_Free(sGarbage);

    
//BMP Header 채우기
#ifdef BMP_256COLOR

    bmpFH.bfType=0x4D42;//19778
    bmpFH.bfSize=BMP_HEADER_SIZE+PHOTO_TOTAL_SIZE/2+(360*sizeof(BYTE));
	bmpFH.bfReserved1 =0;
	bmpFH.bfReserved2=0;
    bmpFH.bfOffBits= BMP_HEADER_SIZE;
    
    bmp.bmiHeader.biSize	= 40;
    bmp.bmiHeader.biWidth	= PHOTO_H_SIZE;
    bmp.bmiHeader.biHeight	= PHOTO_V_SIZE+6;
	bmp.bmiHeader.biplanes	= 1;
	bmp.bmiHeader.biBitCount = 8;   //256color
	bmp.bmiHeader.biCompression = 0;
    bmp.bmiHeader.biSizeImage = PHOTO_TOTAL_SIZE/2+(360*sizeof(BYTE));
    bmp.bmiHeader.biXPelsPerMeter = 3780;
    bmp.bmiHeader.biYPelsPerMeter= 3780;
    bmp.bmiHeader.biClrUsed = 0;
	bmp.bmiHeader.biClrImportant = 0;

    for(i=0;i<256;i++)
    {   
        bmp.bmiColors[i].rgbRed=(i>>5)*32;
        bmp.bmiColors[i].rgbGleen=(i>>2)*32;
        bmp.bmiColors[i].rgbBlue=i*64;
        bmp.bmiColors[i].rgbReserved =0;
    }

#else   //BMP_24BIT

    bmpFH.bfType=0x4D42;//19778
    bmpFH.bfSize=BMP_HEADER_SIZE+PHOTO_TOTAL_SIZE+(360*sizeof(WORD));
	bmpFH.bfReserved1 =0;
	bmpFH.bfReserved2=0;
    bmpFH.bfOffBits= BMP_HEADER_SIZE;
    
    bmp.bmiHeader.biSize	= 40;
    bmp.bmiHeader.biWidth	= PHOTO_H_SIZE;
    bmp.bmiHeader.biHeight	= PHOTO_V_SIZE+6;
	bmp.bmiHeader.biplanes	= 1;
	bmp.bmiHeader.biBitCount = 24;  //24bit
	bmp.bmiHeader.biCompression = 0;
    bmp.bmiHeader.biSizeImage = PHOTO_TOTAL_SIZE+(360*sizeof(WORD));
    bmp.bmiHeader.biXPelsPerMeter = 3780;
    bmp.bmiHeader.biYPelsPerMeter= 3780;
    bmp.bmiHeader.biClrUsed = 0;
	bmp.bmiHeader.biClrImportant = 0;


#endif
#ifndef WIN32
    AddressAssign();
#endif
    pBMP = SeekWorkingArea(BMP);
	
    memcpy((BYTE*)pBMP, (void *)&bmpFH, (DWORD) sizeof(BMPFILEHEADER) );
	BMP_SIZE+= sizeof(BMPFILEHEADER);
    memcpy( (BYTE*)pBMP+BMP_SIZE, (void *)&bmp.bmiHeader, (DWORD)sizeof(BMPINFOHEADER) );
	BMP_SIZE+= sizeof(BMPINFOHEADER);
#ifdef BMP_256COLOR
    memcpy( (BYTE*)pBMP+BMP_SIZE, (void *)&bmp.bmiColors, (DWORD)(sizeof(RGBQUAD)*256) );
	BMP_SIZE+= sizeof(RGBQUAD)*256;

    for(i=0;i<360;i++)          
    {
        memset((BYTE*)pBMP+BMP_SIZE,0xFF,sizeof(BYTE));
        BMP_SIZE+=sizeof(BYTE);
    }
#else   //BMP_24BIT
    for(i=0;i<360;i++)          
    {
        memset((BYTE*)pBMP+BMP_SIZE,0xFFFFFF,sizeof(BYTE)*3);
        BMP_SIZE+=(sizeof(BYTE)*3);
    }
#endif

#ifdef BMP_256COLOR
#ifdef deco_photo
    switch(decoNum)
    {   
        case 0:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 1:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpFlheart120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpFlheart120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 2:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpBear120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpBear120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 3:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpCat120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpCat120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 4:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpElle120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpElle120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 5:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpFlfarm120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpFlfarm120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 6:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpFlower120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpFlower120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 7:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpLove120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpLove120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 8:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpMonst120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpMonst120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 9:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpRabbit120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpRabbit120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
        case 10:
            for(i=0;i<PHOTO_V_SIZE;i++)
            {
	            for(j=0;j<PHOTO_H_SIZE;j++)
                {    
                    if(g_BmpTv120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j]==0x01) 
                        memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    else
                        memcpy((BYTE*)pBMP+BMP_SIZE,(BYTE*)&g_BmpTv120x90x256c.pbData[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                    BMP_SIZE+=sizeof(BYTE);
                }
            }
            break;
    }
#else //deco_photo
    
    for(i=0;i<PHOTO_V_SIZE;i++)
    {
	    for(j=0;j<PHOTO_H_SIZE;j++)
        {   
            if(BitmapCameraInfo.Header.nBitsPerPixel == 16)
            {
                memcpy(&tmpEbi1Pixel,&g_abBmpBuf[(((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j)*2],sizeof(WORD));
            
                tmp24Bmp1Pixel[0] = (tmpEbi1Pixel>>11)*8;
                tmp24Bmp1Pixel[1] = (tmpEbi1Pixel>>5)*4;
                tmp24Bmp1Pixel[2] = tmpEbi1Pixel*8;

                tmp256Bmp = RGB332(tmp24Bmp1Pixel[0],tmp24Bmp1Pixel[1],tmp24Bmp1Pixel[2]);

                memcpy((BYTE*)pBMP+BMP_SIZE,&tmp256Bmp,sizeof(BYTE));
                BMP_SIZE+=sizeof(BYTE);
            }
            else if(BitmapCameraInfo.Header.nBitsPerPixel == 8)
            {
                memcpy((BYTE*)pBMP+BMP_SIZE,&g_abBmpBuf[((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j],sizeof(BYTE));
                BMP_SIZE+=sizeof(BYTE);
            }
        }
    }
    
#endif //deco_photo
    for(i=0;i<360;i++)
    {
        memset((BYTE*)pBMP+BMP_SIZE,0xFF,sizeof(BYTE));
        BMP_SIZE+=sizeof(BYTE);
    }
#else   //BMP_24BIT

    for(i=0;i<PHOTO_V_SIZE;i++)
    {
	    for(j=0;j<PHOTO_H_SIZE;j++)
        {   
            memcpy(&tmpEbi1Pixel,&g_abBmpBuf[(((PHOTO_V_SIZE-1)-i)*PHOTO_H_SIZE+j)*2],sizeof(WORD));

            tmp24Bmp1Pixel[0] = (tmpEbi1Pixel>>11)*8;
            tmp24Bmp1Pixel[1] = (tmpEbi1Pixel>>5)*4;
            tmp24Bmp1Pixel[2] = tmpEbi1Pixel*8;
            
            for(k=0;k<3;k++)
            {
                memcpy((BYTE*)pBMP+BMP_SIZE,&tmp24Bmp1Pixel[k],sizeof(BYTE));
            }
            BMP_SIZE+=(sizeof(BYTE)*3);
        }
    }

    for(i=0;i<360;i++)
    {
        memset((BYTE*)pBMP+BMP_SIZE,0xFFFFFF,sizeof(BYTE)*3);
        BMP_SIZE+=(sizeof(BYTE)*3);
    }
#endif
   
    pCONTENT_DATA = SeekWorkingArea(CNT);
        
    CONTENT_SIZE += sprintf(pCONTENT_DATA, BOUND_BODY); //boundary_body
    
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "Content-Disposition: form-data; name=\"min\"\r\n");
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "\r\n");

    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, GetMyNumber()); //포토메일 발신자 번호(자기번호)
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, ";");

    for(i=0;i<PMMsg.MIN_NUM;i++)
    {
        CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, PMMsg.MIN[i]);
        if(i==(PMMsg.MIN_NUM-1))
        {
            i=PMMsg.MIN_NUM;
        }
        else
        {
            CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, ";");
        }
        
    }
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "\r\n");

    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, BOUND_BODY); //boundary_body
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "Content-Disposition: form-data; name=\"message\"\r\n");
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "\r\n");
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, PMMsg.PMText);
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "\r\n");
    
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, BOUND_BODY); //boundary_body
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "Content-Disposition: form-data; name=\"upfile\"; ");
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "filename=\"%s.bmp\"\r\n",&s_psFileName);
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "Content-Type: image/bmp\r\n");
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "\r\n");
       
    memcpy(pCONTENT_DATA + CONTENT_SIZE ,pBMP, BMP_SIZE);
    CONTENT_SIZE+= BMP_SIZE;

    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, "\r\n");
    CONTENT_SIZE += sprintf(pCONTENT_DATA + CONTENT_SIZE, BOUND_END); //boundary_end
   
    pTRANS_DATA = SeekWorkingArea(TRS);
    
    TRAN_DATA_SIZE += sprintf(pTRANS_DATA, "POST %s HTTP/1.0\r\n", "/pmsu");
    //TRAN_DATA_SIZE += sprintf(pTRANS_DATA, "POST %s HTTP/1.0\r\n", "/cgi-bin/skcam_http_web_noqdecoder.cgi");
    TRAN_DATA_SIZE += sprintf(pTRANS_DATA + TRAN_DATA_SIZE, "Accept-Language: ko\r\n");
    TRAN_DATA_SIZE += sprintf(pTRANS_DATA + TRAN_DATA_SIZE, "Content-Type: multipart/form-data; ");
    TRAN_DATA_SIZE += sprintf(pTRANS_DATA + TRAN_DATA_SIZE, "boundary=%s",BOUND_HEAD); //boundary_head
    TRAN_DATA_SIZE += sprintf(pTRANS_DATA + TRAN_DATA_SIZE, "User-Agent: %s\r\n", HTTP_USER_AGENT);
    TRAN_DATA_SIZE += sprintf(pTRANS_DATA + TRAN_DATA_SIZE, "Content-length: %hu\r\n",CONTENT_SIZE);
    TRAN_DATA_SIZE += sprintf(pTRANS_DATA + TRAN_DATA_SIZE, "Connection: Keep_Alive\r\n");
    TRAN_DATA_SIZE += sprintf(pTRANS_DATA + TRAN_DATA_SIZE, "\r\n");

    memcpy(pTRANS_DATA + TRAN_DATA_SIZE, pCONTENT_DATA,CONTENT_SIZE);

    return TRAN_DATA_SIZE+CONTENT_SIZE;
    
}

VOID SKY_PMListUP(VOID)
{	
	INT8 sWindowSize;
	BYTE**  ppScrollList;
    	
	if(g_MenuSize!=0)
    {
        EQS_DeleteControl(g_hHnd);
        SKY_FreePMScrollList();
    }

    g_unNumOfPM= (UINT)SKY_GetNumFile(SKY_VM_DIR);

    if(g_unNumOfPM ==0)
    {
         SKY_InitPM();
    }
    
    g_MenuSize = g_unNumOfPM;

	if(g_MenuSize>0)
    {			
		ppScrollList = SKY_MakeScrollListPM();
		sWindowSize = ( g_MenuSize > 6 ) ? 6 : g_MenuSize;
		g_hHnd = EQC_ScrollList(5, 5, 18, sWindowSize,   ppScrollList   , g_MenuSize, g_MenuSize, 0); 
        EQC_SET_STYLE( g_hHnd, EQC_GET_STYLE(g_hHnd) |
                             CS_SCROLL_LIST_ENUMBERATED |
                             CS_SCROLL_LIST_MAXIMIZE );
		//EQC_SET_STYLE( hHnd, (EQC_GET_STYLE(hHnd) | CS_SCROLL_LIST_MAXIMIZE) );
		EQS_SetIOCActive( g_hHnd );

        EQS_SetWindowCML(CL_MENU_SELECT_NEWMAIL);
    }
	else
    {   
        g_NumOfPMList=g_unNumOfPM;
        SKY_INIT_PM_COUNT(1);
		EQS_SetWindowCML(CL_NONE_NONE_NEWMAIL);
    }
	
}

/*			
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_DecoPhoto()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_DecoPhoto(SKY_EVENT_T *pEvent)
{
    STATIC DWORD sImageNum;
    STATIC DWORD sImageIndex;
    STATIC DWORD sEditIndex;
    STATIC DWORD sViewIndex;
#ifdef deco_photo
    DRAW_MODE DrawMode;
#endif

    switch(pEvent->EventID)
    {
        case SE_APP_START : /* The very first event */
            //EQS_GetDC(TRUE);
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_MOVE_SELECT,
                SKY_GET_TOKEN(TKN_PM_DECO_PHOTO), //사진꾸미기 
                WA_SHOW_HEADER | WA_SHOW_CML);
      
            sViewIndex=pEvent->dwParam;
            if(pEvent->dwParam>0)
                sEditIndex=pEvent->dwParam;
            else
                sEditIndex=1;

            sImageIndex=0;
            EQC_Image(0,1,&BitmapCameraInfo);
            break;

        case SE_APP_END :   /* The very last event */ 
            //EQS_ReleaseDC();
		    break;

        case SE_APP_SUSPEND: /* child applet is started */
            break;

        case SE_APP_RESUME: /* child applet is ended */
            break;

        case SE_APP_DRAW:
#ifdef deco_photo
            if(EQS_GET_PARENT() == SKA_RedactPM)
            {   sImageNum = pEvent->dwParam+sViewIndex;}
            else
            {   sImageNum = pEvent->dwParam+sEditIndex;}
             
            if(sImageNum>10 || sImageNum<0)
            {   sImageNum-=11;}
           
            switch(sImageNum)
            {   
                case 0:
                    
                    break;
                case 1:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,1,&g_BmpFlheart120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 2:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpBear120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 3:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpCat120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 4:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpElle120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 5:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpFlfarm120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 6:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpFlower120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 7:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpLove120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 8:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpMonst120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 9:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpRabbit120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                case 10:
                    DrawMode  = EQS_GetDrawMode();
                    EQS_SetDrawMode(DM_OR);
                    EQS_PutBitmap(0,0,&g_BmpTv120x90x256c);
                    EQS_SetDrawMode(DrawMode);
                    break;
                
            }
#endif //deco_photo
            break;

        case SE_KEY_DOWN: 
                
                if(sImageIndex<10)
                    sImageIndex++;
                else
                    sImageIndex=0;
                EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA, sImageIndex);
            
            break;

        case SE_KEY_UP: 
                if(sImageIndex>0)         
                    sImageIndex--;
                else 
                    sImageIndex=10;
                EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA, sImageIndex);
            
            break;
        
        case SE_KEY_CLEAR:
            if(EQS_GET_PARENT() == SKA_RedactPM)
            {EQS_END_EVENT(SE_APP_RESUME, EQS_NA,sViewIndex);}
            else
            {EQS_END_EVENT(SE_APP_RESUME, PM_CLEAR_POPUP,EQS_NA);}
        break;

        case SE_SOFTKEY_SELECT:
           
            EQS_END_EVENT(SE_APP_RESUME,EQS_NA,sImageNum);

            break;

        default:
            EQS_DEF_EVENT_PROC(pEvent);
            break;

    }
}

/*			
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_PhotoNameSave()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_PhotoNameSave(SKY_EVENT_T *pEvent)
{   
    BYTE pMsgStr[SKY_FILE_EXT_NAME_MAX_LENGTH+1];
    BYTE nCtrl;
    HCONTROL hControl;
        
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        //EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_CONFIRM_NONE,
            SKY_GET_TOKEN(TKN_PM_PHOTO_NAME), //사진이름 
            WA_SHOW_HEADER | WA_SHOW_CML);
    
           
        EQC_StaticText(EQS_NA, EQS_NA, TKN_PHOTO_FILE_NAME); //0
        hControl=EQC_TextEdit(EQS_NA, EQS_NA, 16, \
                    1, TEM_KOREAN, CAMERA_NAME_EDIT_MAX_LEN,(BYTE*)"" ,0); //1
        EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_TEXT_EDIT_ALONE);

        EQS_SetIOCActive(EQS_GetNthIOC(1));

        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < 2; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        break;

    case SE_APP_END :   /* The very last event */ 
        //EQS_ReleaseDC();
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_TEDIT_DONE:

        EQC_GetTextEditData(EQS_GetNthIOC(1), pMsgStr);
        memset(l_PhotoName,NULL,SKY_FILE_EXT_NAME_MAX_LENGTH + 1);
        STRNCPY(l_PhotoName,pMsgStr,CAMERA_NAME_EDIT_MAX_LEN); //CAMERA_NAME_EDIT_MAX_LEN=14
        if(l_PhotoName[0] == 0)
        {
            hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_NONE_PHOTO_NAME), PM_MESSAGE_DEFAULT_TIME, MBT_NONE,MBT_HELP);
            EQC_SET_USERDATA(hControl,PNS_MES_E_1);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |CS_MESSAGE_BOX_IGNORE_KEYS);             
        }
        else
        {
            g_InPhotoName=TRUE;
            hControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_CONFIRM_PHOTO_NAME),	//사진이름이 지정되었습니다.
				        PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hControl,PNS_MES_E_2);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        break;
    case SE_IO_MESGBOX_END: 
        if(pEvent->dwParam == PNS_MES_E_2)
        {
            EQS_End();
        }
        else if(pEvent->dwParam == PNS_MES_E_1)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(1));
        }

        break;

    case SE_IO_TEDIT_CANCEL:
        EQS_End();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;

    }
}
        
/*			
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_ViewPM()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API sSKA_ViewPM(SKY_EVENT_T *pEvent)
{ 
     
    STATIC BOOL existPHOTO;
    STATIC WORD readData;
    STATIC UINT8 decoNum;
    STATIC UINT16 nLen;
    STATIC UINT8 min_num;
    HCONTROL hVPControl;
    INT m;
    DWORD dwTimeInfo;
    //BYTE sGarbage[CAMERA_BUF_SPACE];
    BYTE sFNameGarbage[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
    HFILE	hFile;
    clk_julian_type julian;
	CHAR timeBuf[38],sAMPM[3];
    INT8    nLine,nCtrl;
    HCONTROL hControl;
    STATIC INT conNum;
#ifdef deco_photo
    DRAW_MODE DrawMode;
#endif
    BYTE    szWriteTime[PM_MAX_NUM_PM+1];
   	
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

		//EQS_GetDC(TRUE);	
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      350,              //SKY_DEF_WIN_DY,
                      CL_DELETE_SEND_EDIT,
                      SKY_GET_TOKEN(TKN_PM_PHOTO_MAIL),	//포토메일
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
                      
        SKY_InitPMMsg(&PMMsg);
        readData = pEvent->wParam;
        SKY_ReadPM(readData);

        //사진꾸미기
        decoNum = PMMsg.DecoNum;
        min_num = PMMsg.MIN_NUM;

        //사진
        
        ConvertCameraImageFileName(sFNameGarbage, PMMsg.PMPhoto); 
        if(SKY_OpenDataFile(SDF_PHOTO, sFNameGarbage, &hFile, SFT_READ) == SDR_OK)
        {
            SKY_ReadDataBlock(hFile, sFNameGarbage, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
            SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
            SKY_ReadDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));  
            //SKY_ReadDataBlock(hFile, sGarbage, CAMERA_BUF_SPACE); 
            SKY_ReadDataBlock(hFile, g_abBmpBuf, PHOTO_TOTAL_SIZE); 
            SKY_CloseDataFile(hFile); 
            BitmapCameraInfo.pbData = g_abBmpBuf;
            //BitmapCameraInfo.Header.yStart = 15;
            //BitmapCameraInfo.Header.PSize.yHeight =PHOTO_V_SIZE;
            
            existPHOTO = TRUE;
            
        }
        else
        {   
            existPHOTO=FALSE;
            EQS_SetWindowCML(CL_DELETE_NONE_NONE);
        }

        if(existPHOTO)
        {
            EQC_Image(EQS_NA,EQS_NA,&BitmapCameraInfo);
            conNum=0;
            EQC_SET_FOCUS(EQS_GetNthIOC(0),TRUE);

        }
        else 
        {
            conNum=1;
        }

        //시간정보
        
        clk_secs_to_julian(PMMsg.Save_time, &julian);
       
		if(julian.hour <13)
			STRCPY(sAMPM,"AM");
		else
		{
			julian.hour-=12;
			STRCPY(sAMPM,"PM");
		}

		sprintf(timeBuf,"%2d/%02d(%s)%2d:%02d%s",
                julian.month, julian.day, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
                julian.hour, julian.minute, sAMPM); 
        if(PMMsg.Sent)
        {
            STRCPY(szWriteTime,(BYTE*)SKY_GET_TOKEN(TKN_PM_SENT_IMAGE));
            STRCAT(szWriteTime, (BYTE*)SKY_GET_TOKEN(TKN_PM_WRITE_TIME));
            EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), szWriteTime); //작성 날짜
        }
        else
        {
            STRCPY(szWriteTime,(BYTE*)SKY_GET_TOKEN(TKN_PM_SAVE_IMAGE));
            STRCAT(szWriteTime, (BYTE*)SKY_GET_TOKEN(TKN_PM_WRITE_TIME));
            EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), szWriteTime); //작성 날짜
        }

        hControl = EQC_StaticText(EQS_NA,EQS_NA,TKN_NULL); //시간정보 
        EQC_SetStaticText(hControl,timeBuf);
 		
        EQC_StaticText(EQS_NA,EQS_NA,TKN_PM_MSG_CONTENT);   //메세지내용
        
        if (STRLEN(PMMsg.PMText) ==0)
        {
            nLine=1;
            EQC_TextEdit(EQS_NA,EQS_NA,PM_EDIT_SIZE,nLine,TEM_KOREAN,80, PMMsg.PMText ,0);
        }
        else
        {

            nLine=STRLEN(PMMsg.PMText)/PM_EDIT_SIZE ;
            if( (STRLEN(PMMsg.PMText)%PM_EDIT_SIZE) > 0)
            {    
                nLine++;
            }
            hControl = EQC_TextEdit(EQS_NA,EQS_NA,PM_EDIT_SIZE,nLine,TEM_KOREAN,80, PMMsg.PMText ,0);
            EQC_GetTextEditLine(hControl);
            EQC_FitTextEditLine(hControl);
        }

        
#ifdef email_version
        if(PMMsg.MIN_NUM)   //전화번호가 없을수없기땜시 IF문 필요없당.
        {   
            EQC_StaticText(EQS_NA,EQS_NA,TKN_PM_RECEIVER);      //받는 사람

		    for(m=0;m<PMMsg.MIN_NUM;m++)
            {   
                EQC_TextEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, TEM_NUMBER, 
                     PM_MAX_SIZE_MIN, PMMsg.MIN[m], 0);  
            
            }
        }
#else
		if(PMMsg.MIN_NUM)   //전화번호가 없을수없기땜시 IF문 필요없당.
        {   
            EQC_StaticText(EQS_NA,EQS_NA,TKN_PM_OTHER_PARTY_CALL_NUM);      //상대방 전화번호

		    for(m=0;m<PMMsg.MIN_NUM;m++)
            {   
                EQC_NumberEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, 11, PMMsg.MIN[m]);  
            
            }
        }
#endif //email_version
        
        EQS_StartFormatter();
        for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
        {
            if(nCtrl == VP_STATIC_TIME_1-conNum 
                || nCtrl ==VP_STATIC_TEXT_1-conNum ||nCtrl == VP_STATIC_TEXT_2 -conNum)
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            }
            else
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
                EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
            }
            EQS_NewLine();
        }

        EQS_EndFormatter();
        EQS_FitCanvasSize();
      
        for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
        {
            EQC_SET_FOCUS(EQS_GetNthIOC(nCtrl), FALSE);
        }
        
        if(!existPHOTO)
        {
            hVPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_NOT_EXIST_PHOTO),	//해당사진이 없습니다.
				        PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hVPControl,VP_MES_E_2);
            EQC_SET_STYLE(hVPControl, EQC_GET_STYLE(hVPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        EQS_StartTimer(EQS_TIMER_1, 500, TRUE);

        break;

    case SE_APP_END :   /* The very last event */
        SKY_AcceptKey(TRUE);
        EQS_StopTimer(EQS_TIMER_1);
        //SKY_DisconnectHTTPServer();
		SKY_SET_PM(FALSE);//jmpark
	//	EQS_ReleaseDC();
        break;
 
    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_KEY_CLEAR:
        EQS_StopTimer(EQS_TIMER_1);
        EQS_END_EVENT(SE_APP_RESUME, EQS_NA ,PM_CLEAR1_POPUP); 
        break;
    
    case SE_APP_DRAW:
        
#ifdef deco_photo   
         if(!existPHOTO)
             decoNum=0;
         switch(decoNum)
         {  
            case 0:
                
                break;
            case 1:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,1,&g_BmpFlheart120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 2:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpBear120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 3:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpCat120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 4:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpElle120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 5:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpFlfarm120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 6:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpFlower120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 7:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpLove120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 8:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpMonst120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 9:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpRabbit120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 10:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpTv120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
         }
#endif //deco_photo	         
          
   
         break;

    case SE_USER_1:
        SKY_ReadNWritePM(readData);

        //전송이벤트
#ifndef WIN32
        nLen = SKY_PrepareTran(PMMsg.PMPhoto,PMMsg.DecoNum);
        g_SendState = PM_FAIL_ORIGINATED;  

        if(SKY_IS_NOSERVICE())
        {   
            //SKY_AcceptKey(TRUE);
		  	g_SendState = PM_NOSERVICE;
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
			break;
        }
		else if(SKY_IS_MAINTREQ())
        {    
            //SKY_AcceptKey(TRUE);
            g_SendState = PM_MAINTREQ;
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
            break;
        }

        g_SendPM=PM_FAIL;
        SKY_ConnectHTTPServer();
        
            
        break;
#else       
           
        hVPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCCESS_SEND_PM),	//메일이 성공적으로 전송되었습니다.
						            PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        EQC_SET_STYLE(hVPControl, EQC_GET_STYLE(hVPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        
#endif  

        break;
               
    case SE_APP_RESUME: /* child applet is ended */
        if(EQS_GET_CHILD() == sSKA_PMOriginated||EQS_GET_CHILD() ==sSKA_PMHTTPInProg
            ||(EQS_GET_CHILD() == SKA_RedactPM && pEvent->wParam == PM_TRAN_POPUP))
        {   
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);    
        }
        else if(pEvent->dwParam ==PM_CLEAR1_POPUP)
            EQS_END_EVENT(SE_APP_RESUME, EQS_NA ,PM_CLEAR1_POPUP); 
        else               
            EQS_END();
       
        break;  

    case SE_TM_TIMER1:
        EQS_SendEvent((EQS_EVENT_ID)SE_KEY_DOWN, EQS_DRAW_LEVEL_APP, EQS_NA);
        break;

    case SE_KEY_UP:
        EQS_StopTimer(EQS_TIMER_1);
        EQS_PASS_CURRENT_EVENT();
        break;
        
    case SE_SOFTKEY_DELETE:
        EQS_StopTimer(EQS_TIMER_1);
        hVPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_QUS_DELETE_ITEM),		//해당항목을 삭제할까요? 
                                                PM_MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        EQC_SET_USERDATA(hVPControl,VP_MES_E_1);
        break;
    
    case SE_SOFTKEY_EDIT:
        EQS_StopTimer(EQS_TIMER_1);
        if(g_unNumOfPM>=30)
        {
            hVPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_TELL_MAXIMUM_PM), //최대 30개 저장가능
                           PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hVPControl,VP_MES_E_6);
            EQC_SET_STYLE(hVPControl, EQC_GET_STYLE(hVPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
            break;
        }
        EQS_START_CHILD_EVENT(SKA_RedactPM,SE_APP_START,readData,EQS_NA);
        break;

    case SE_SOFTKEY_SEND:
        SKY_AcceptKey(FALSE);   // Originated 를 받으면 TRUE로 됨
        EQS_StopTimer(EQS_TIMER_1);
        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(AB_ALWAYS);

        if(!PMMsg.Sent)
        {
           // EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, PM_SEND_EVENT);   
            for(nCtrl = EQS_GetIOCNum()-1; nCtrl >=0; nCtrl--)
            { 
                EQS_DeleteControl(EQS_GetNthIOC(nCtrl));
            }

            EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    300, 
                    CL_NONE, 
                    NULL, // show annunciators
                    WA_SHOW_HEADER);

            EQC_Image(PM_ANI_X, PM_ANI_Y, (BITMAP*)&g_BmpConnectDataService);
            //EQC_Image(PM_PANNEL_X,PM_PANNEL_Y, (BITMAP*)&g_BmpGeneralPannel);
            hControl = EQC_StaticText(PM_STATUS_X, PM_STATUS_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE);
            EQC_SET_STYLE(hControl,CS_STATIC_TEXT_ORING);
            EQS_FitCanvasSize();
            EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);

            EQS_SEND_EVENT(SE_USER_1,EQS_NA,EQS_NA);
        }
        else
        {

            //전송이벤트
#ifndef WIN32
            nLen = SKY_PrepareTran(PMMsg.PMPhoto,PMMsg.DecoNum);
            g_SendState = PM_FAIL_ORIGINATED;  
    
            if(SKY_IS_NOSERVICE())
            {   
                //SKY_AcceptKey(TRUE);
		  	    g_SendState = PM_NOSERVICE;
                EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
			    break;
            }
		    else if(SKY_IS_MAINTREQ())
            {    
                //SKY_AcceptKey(TRUE);
                g_SendState = PM_MAINTREQ;
                EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
                break;
            }
   
            g_SendPM=PM_FAIL;
            SKY_ConnectHTTPServer();
       
        break;
#else       
           
        hVPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCCESS_SEND_PM),	//메일이 성공적으로 전송되었습니다.
						            PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        EQC_SET_STYLE(hVPControl, EQC_GET_STYLE(hVPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        
#endif  
        }
        break;

    case SE_SOCK_RELEASED:
        //SKY_AcceptKey(TRUE);
        SKY_SET_ORIGINATE(FALSE);
        
        EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA); 
        
        break;

    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        SKY_SET_ORIGINATE(TRUE);    // ui.originate = TRUE; 
        g_SendState = PM_SUCC_ORIGINATED;        
        EQS_START_CHILD_EVENT(sSKA_PMOriginated,SE_APP_START,nLen,EQS_NA); 
       
        break;

    case SE_CALL_ENDED:
        g_SendPM=PM_CONNECT_FAIL;
        SKY_DisconnectHTTPServer();
        
       break;
    
    case SE_IO_MESGBOX_END:
        
        if(pEvent->dwParam == VP_MES_E_2 || pEvent->dwParam == VP_MES_E_6)
        { }
        else if(pEvent->dwParam == VP_MES_E_7)
        {
            SKY_DeletePM(readData);
            hVPControl= EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_CONFIRM_DEL_PM),	//포토메일이 삭제되었습니다. 
                               PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hVPControl,VP_MES_E_4);
            EQC_SET_STYLE(hVPControl, EQC_GET_STYLE(hVPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        else 
        {
               EQS_END();
        }
        break;

   case SE_IO_MESGBOX_YES:
       
        if (pEvent->dwParam == VP_MES_E_1)
        {  
            hVPControl= EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_DELETING_PM),	//삭제중입니다.
                               1, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hVPControl,VP_MES_E_7);
            EQC_SET_STYLE(hVPControl, EQC_GET_STYLE(hVPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
            
        }
       
		break;

	case SE_IO_MESGBOX_NO:
        if (pEvent->dwParam == VP_MES_E_1)
        { }
        else
        {
            EQS_END(); 
        }           
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;

    }
}

/*			
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_RedactPM()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_RedactPM(SKY_EVENT_T *pEvent)
{ 
     
    STATIC DWORD dwTemp;
    STATIC BOOL DataChanged;
    STATIC WORD readData;
    STATIC UINT8 decoNum;
    STATIC UINT8 oldDecoNum;
    STATIC UINT16 nLen;
    STATIC HCONTROL hFocusedHnd;
    //STATIC BOOL bInputReceiver = FALSE;      
    HCONTROL hRPControl;
    pb_number_info_type sNumberInfo;
    BYTE  getValue[PM_MAX_SIZE_MIN+1];
    INT m;
    DWORD dwTimeInfo;
    DWORD AvailMem;
    //BYTE sGarbage[CAMERA_BUF_SPACE];
    BYTE sFNameGarbage[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
    HFILE	hFile,nFiles;
    INT8    nCtrl;
    BYTE sTempValue[5][PM_MAX_SIZE_MIN+1];
    HCONTROL hControl;
    CALLSTACK_T*                pCallStack;
#ifdef deco_photo
    DRAW_MODE DrawMode;
#endif
#ifdef email_version
    pb_email_info_type email_info;
#endif //email_version
   
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

		//EQS_GetDC(TRUE);	
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      310,//SKY_DEF_WIN_DY,
                      CL_MENU_NONE_SEND,
                      SKY_GET_TOKEN(TKN_PM_PHOTO_MAIL),	//포토메일
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        DataChanged = FALSE;
                
        SKY_InitPMMsg(&PMMsg);
        readData = pEvent->wParam;
        SKY_ReadPM(readData);

        //사진꾸미기
        oldDecoNum = PMMsg.DecoNum;
        decoNum = PMMsg.DecoNum;

        //사진
        dwTemp=PMMsg.PMPhoto;
        ConvertCameraImageFileName(sFNameGarbage, PMMsg.PMPhoto); 
        if(SKY_OpenDataFile(SDF_PHOTO, sFNameGarbage, &hFile, SFT_READ) == SDR_OK)
        {
            SKY_ReadDataBlock(hFile, sFNameGarbage, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
            SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
            SKY_ReadDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));  
            //SKY_ReadDataBlock(hFile, sGarbage, CAMERA_BUF_SPACE); 
            SKY_ReadDataBlock(hFile, g_abBmpBuf, GET_IMAGE_BYTE_SIZE(BitmapCameraInfo.Header)); 
            SKY_CloseDataFile(hFile); 
            BitmapCameraInfo.pbData = g_abBmpBuf;
            //BitmapCameraInfo.Header.yStart = 15;
            //BitmapCameraInfo.Header.Size.yHeight =PHOTO_V_SIZE;
                                   
        }
                
        EQC_Image(EQS_NA,EQS_NA,&BitmapCameraInfo);
        EQC_SET_FOCUS(EQS_GetNthIOC(RP_PHOTO),TRUE);

        
        EQC_StaticText(EQS_NA,EQS_NA,TKN_PM_MSG_CONTENT);   //메세지내용
        
        EQC_TextEdit(EQS_NA,EQS_NA,PM_EDIT_SIZE,5,TEM_KOREAN,80, PMMsg.PMText ,0);
        
#ifdef email_version
        EQC_StaticText(EQS_NA,EQS_NA,TKN_PM_RECEIVER);      //받는 사람

		for(m=0;m<PMMsg.MIN_NUM;m++)
        {   
           EQC_TextEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, TEM_NUMBER, 
                     PM_MAX_SIZE_MIN, PMMsg.MIN[m], 0);   
        }
          
        if(PMMsg.MIN_NUM<5)
        {
            for(m=PMMsg.MIN_NUM;m<5;m++)
            {
                EQC_TextEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, TEM_NUMBER, 
                     PM_MAX_SIZE_MIN, (BYTE*)"", 0); 
            }
        }
#else
		EQC_StaticText(EQS_NA,EQS_NA,TKN_PM_OTHER_PARTY_CALL_NUM);      //상대방 전화번호

		for(m=0;m<PMMsg.MIN_NUM;m++)
        {   
           EQC_NumberEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, 11, PMMsg.MIN[m]);  
        }
          
        if(PMMsg.MIN_NUM<5)
        {
            for(m=PMMsg.MIN_NUM;m<5;m++)
            {
                EQC_NumberEdit(EQS_NA,EQS_NA, PM_EDIT_SIZE, 1, 11, (BYTE*)""); 
            }
        }
#endif //email_version
        
        EQS_StartFormatter();
        for(nCtrl = 0; nCtrl < EQS_GetIOCNum() ;nCtrl++)
        {
            if(nCtrl == RP_STATIC_TEXT_1 || nCtrl == RP_STATIC_TEXT_2)
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            }
            else
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
                EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
            }
            EQS_NewLine();
        }

        EQS_EndFormatter();
        EQS_FitCanvasSize();
        
        EQC_SET_ENABLE(EQS_GetNthIOC(RP_PHOTO), FALSE);
                     
        break;

    case SE_APP_END :   /* The very last event */ 
        SKY_AcceptKey(TRUE);
        //SKY_DisconnectHTTPServer();
		SKY_SET_PM(FALSE); //jmpark
	//	EQS_ReleaseDC();
        break;
 
    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_KEY_CLEAR:
        hRPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_EDIT_CANCEL),	//포토메일 작성을 취소할까요?
							 MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, 
							 MBT_CONFIRM);
        EQC_SET_USERDATA(hRPControl,RP_MES_E_7);
        break;
 
    case SE_APP_DRAW:
        
#ifdef deco_photo   
     
         switch(decoNum)
         {  
            case 0:
                
                break;
            case 1:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,1,&g_BmpFlheart120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 2:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpBear120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 3:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpCat120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 4:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpElle120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 5:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpFlfarm120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 6:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpFlower120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 7:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpLove120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 8:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpMonst120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 9:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpRabbit120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 10:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpTv120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
        
         }   
#endif //deco_photo
        
         break;
/* 
    case SE_IO_TEDIT_DONE:

        memset(sTempValue,NULL,PM_MAX_SIZE_MIN+1);
#ifdef email_version
        EQC_GetTextEditData(pEvent->wParam, (BYTE*)sTempValue);
#else
		EQC_GetNumberEditValue(pEvent->wParam, (BYTE*)sTempValue);
#endif //email_version
        if(sTempValue[0]!=NULL)
           bInputReceiver = TRUE;
        
        break;
*/
    case SE_USER_1:
        SKY_ReadNWritePM(readData);

        //전송이벤트
          
        nLen = SKY_PrepareTran(PMMsg.PMPhoto,PMMsg.DecoNum);
        g_SendState = PM_FAIL_ORIGINATED;  

        if(SKY_IS_NOSERVICE())
        {
            //SKY_AcceptKey(TRUE);
		  	g_SendState = PM_NOSERVICE;
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
			break;
        }
		else if(SKY_IS_MAINTREQ())
        {   
            //SKY_AcceptKey(TRUE);
            g_SendState = PM_MAINTREQ;
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
            break;
        }
        
        g_SendPM=PM_FAIL;
        SKY_ConnectHTTPServer();

        break;

    case SE_USER_2:
        //전송이벤트
          
        nLen = SKY_PrepareTran(PMMsg.PMPhoto,PMMsg.DecoNum);
        g_SendState = PM_FAIL_ORIGINATED;  

        if(SKY_IS_NOSERVICE())
        {
            //SKY_AcceptKey(TRUE);
		  	g_SendState = PM_NOSERVICE;
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
			break;
        }
		else if(SKY_IS_MAINTREQ())
        {   
            //SKY_AcceptKey(TRUE);
            g_SendState = PM_MAINTREQ;
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
            break;
        }
        
        g_SendPM=PM_FAIL;
        SKY_ConnectHTTPServer();
        break;
       
    case SE_APP_RESUME: /* child applet is ended */
        if(EQS_GET_CHILD() == SKA_SpeedSearch)
        {
            if( pEvent->dwParam )
            {// 값을 선택 
                sNumberInfo.address = pEvent->dwParam;
        
                if( ReadNumber(&sNumberInfo) == TRUE )
                {
#ifdef email_version
                    EQC_SetTextEditData(hFocusedHnd, sNumberInfo.digits);
#else
				    EQC_SetNumberEditValue(hFocusedHnd, sNumberInfo.digits);
#endif //email_version
                    EQC_SET_DIRTY(hFocusedHnd, TRUE);
                }
                EQS_SetIOCFocus(EQS_GetNthIOC(EQS_GetIOCOrderly(hFocusedHnd)+1));
            }
            else
            {  //취소 

            }
        }
        else if(EQS_GET_CHILD() == SKA_MainCallList)
        {
            if(GetCallStackIndex() != 255)
            { 
                pCallStack = GetCSKData();
                
				//EQC_SetNumberEditValue(EQS_GetNthIOC(m), pCallStack->Stack[GetCallStackIndex()].aDigits);
#ifdef email_version
                    EQC_SetTextEditData(hFocusedHnd, g_PhoneNumBuf);
#else
				    EQC_SetNumberEditValue(hFocusedHnd, g_PhoneNumBuf);
#endif //email_version                
                
                EQC_SET_DIRTY(hFocusedHnd, TRUE);
                
                SetCallStackIndex(255);
            }
            else
            {
            }
        }
#ifdef email_version
        else if(EQS_GET_CHILD() == SKA_ViewEmailList)
        {
            if(pEvent->dwParam != NULL)
            {
                email_info.address = pEvent->dwParam;
                ReadEmail(&email_info);

				EQC_SetTextEditData(hFocusedHnd, email_info.szMailID);
                EQC_SET_DIRTY(hFocusedHnd, TRUE);
               
                EQS_SetIOCFocus(EQS_GetNthIOC(EQS_GetIOCOrderly(hFocusedHnd)+1));
            
                
            }
            else
            {
            }
        }
#endif //email_version
        else if(EQS_GET_CHILD() == SKA_MainStackCode)
            break;

        else if(EQS_GET_CHILD() == sSKA_DecoPhoto )
        {   
#ifdef deco_photo  
            decoNum = (UINT8)pEvent->dwParam;
#endif //deco_photo   
        }
        else if(EQS_GET_CHILD() == sSKA_PMOriginated||EQS_GET_CHILD() ==sSKA_PMHTTPInProg)
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);      
        }
        else if(EQS_GET_CHILD() == SKA_MainPbookCode)
        {
            break;
        }

        else 
        {
           EQS_END();
        }
        break;  
        
   
     case SE_SOFTKEY_MENU:
        hFocusedHnd = EQS_GetFocusedIOC();
        if(EQS_IsControlDataChanged()|| decoNum!=oldDecoNum)
        {    
            pabRedactPMMenu[0] = TRUE;
        }
        else
        {
            pabRedactPMMenu[0] = FALSE;
        }
#ifdef email_version
#ifdef deco_photo
        
        if(hFocusedHnd == EQS_GetNthIOC(RP_PHOTO))
        {   
            pabRedactPMMenu[1] = TRUE;
            pabRedactPMMenu[2] = FALSE;
            pabRedactPMMenu[3] = FALSE;
            pabRedactPMMenu[4] = FALSE;
            pabRedactPMMenu[5] = FALSE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,6,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
         }
   

        if(hFocusedHnd >= EQS_GetNthIOC(RP_NUM_EDIT_1))
        { 
            
            pabRedactPMMenu[1] = FALSE;
            pabRedactPMMenu[2] = TRUE;
            pabRedactPMMenu[3] = TRUE;
            pabRedactPMMenu[4] = TRUE;
            pabRedactPMMenu[5] = TRUE;
        
            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,6,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        
            
        }
        else if (hFocusedHnd >= EQS_GetNthIOC(RP_TEXT_EDIT_1)) 
        {   
            pabRedactPMMenu[1] = FALSE;
            pabRedactPMMenu[2] = FALSE;
            pabRedactPMMenu[3] = FALSE;
            pabRedactPMMenu[4] = FALSE;
            pabRedactPMMenu[5] = FALSE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,6,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
#else//deco_photo
        
        if(hFocusedHnd >= EQS_GetNthIOC(RP_NUM_EDIT_1))
        {   
            
            pabRedactPMMenu[1] = TRUE;
            pabRedactPMMenu[2] = TRUE;
            pabRedactPMMenu[3] = TRUE;
            pabRedactPMMenu[4] = TRUE;
        
            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,5,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
         
        }
        else 
        {   
            pabRedactPMMenu[1] = FALSE;
            pabRedactPMMenu[2] = FALSE;
            pabRedactPMMenu[3] = FALSE;
            pabRedactPMMenu[4] = FALSE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,5,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
#endif //deco_photo
#else //email_version
#ifdef deco_photo
        
        if(hFocusedHnd == EQS_GetNthIOC(RP_PHOTO))
        {   
            pabRedactPMMenu[1] = TRUE;
            pabRedactPMMenu[2] = FALSE;
            pabRedactPMMenu[3] = FALSE;
            pabRedactPMMenu[4] = FALSE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,5,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
         }
   

        if(hFocusedHnd >= EQS_GetNthIOC(RP_NUM_EDIT_1))
        {  
            
            pabRedactPMMenu[1] = FALSE;
            pabRedactPMMenu[2] = TRUE;
            pabRedactPMMenu[3] = TRUE;
            pabRedactPMMenu[4] = TRUE;
        
            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,5,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
            
        }
        else if (hFocusedHnd >= EQS_GetNthIOC(RP_TEXT_EDIT_1)) 
        {   
            pabRedactPMMenu[1] = FALSE;
            pabRedactPMMenu[2] = FALSE;
            pabRedactPMMenu[3] = FALSE;
            pabRedactPMMenu[4] = FALSE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,5,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
#else//deco_photo
        
        if(hFocusedHnd >= EQS_GetNthIOC(RP_NUM_EDIT_1))
        { 
            
            pabRedactPMMenu[1] = TRUE;
            pabRedactPMMenu[2] = TRUE;
            pabRedactPMMenu[3] = TRUE;
        
            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,4,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
            
        }
        else 
        {   
            pabRedactPMMenu[1] = FALSE;
            pabRedactPMMenu[2] = FALSE;
            pabRedactPMMenu[3] = FALSE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paRedactPMMenu,4,0,pabRedactPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
#endif //deco_photo
#endif //email_version
        
       break;

    case SE_SOFTKEY_SEND:
/*        if((!EQS_IsControlDataChanged())|| EQC_GET_DIRTY(EQS_GetNthIOC(RP_NUM_EDIT_1))
                                        || EQC_GET_DIRTY(EQS_GetNthIOC(RP_NUM_EDIT_2))
                                        || EQC_GET_DIRTY(EQS_GetNthIOC(RP_NUM_EDIT_3))
                                        || EQC_GET_DIRTY(EQS_GetNthIOC(RP_NUM_EDIT_4))
                                        || EQC_GET_DIRTY(EQS_GetNthIOC(RP_NUM_EDIT_5)))
        {
            bInputReceiver = TRUE;
        }
        else*/ if(EQS_IsControlDataChanged())
        {   
            DataChanged=TRUE;
        }
        //전화번호 유무확인
         for(m=RP_NUM_EDIT_1;m<RP_EDIT_MAX;m++)
         {	
	  	     sTempValue[m-(RP_NUM_EDIT_1)][m]=0;
#ifdef email_version
             EQC_GetTextEditData(EQS_GetNthIOC(m), sTempValue[m-(RP_NUM_EDIT_1)]);
#else
             EQC_GetNumberEditValue(EQS_GetNthIOC(m), sTempValue[m-(RP_NUM_EDIT_1)]);
#endif //email_version
         }
         if(sTempValue[0][0]==0 && sTempValue[1][0]==0 && sTempValue[2][0]==0 &&
                           sTempValue[3][0]==0 && sTempValue[4][0]==0)
         {
#ifdef email_version
             hRPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_INPUT_RECEIVER), //받는 사람을 입력하세요.
                                 PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
             EQC_SET_USERDATA(hRPControl,RP_MES_E_5);
#else
             hRPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_INPUT_PHONE_NUM), //전화번호를 입력하세요.
                                 PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
             EQC_SET_USERDATA(hRPControl,RP_MES_E_5);
#endif //email_version
             break;
        }

        hRPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_QUS_SEND_PM),	//지금 전송하시겠습니까?
							 MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, 
							 MBT_CONFIRM);
        EQC_SET_USERDATA(hRPControl,RP_MES_E_8);
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == RP_MES_E_1)
        {   
             SKY_InitPMMsg(&PMMsg);

            PMMsg.Sent=TRUE;
            PMMsg.MIN_NUM = 0;
            PMMsg.DecoNum = decoNum;
            PMMsg.PMPhoto=dwTemp;
            PMMsg.Save_time = SKY_GetLocalTime();
            PMMsg.Send_time = SKY_GetLocalTime();
            EQC_GetTextEditData(EQS_GetNthIOC(RP_TEXT_EDIT_1), PMMsg.PMText);
            //EQC_GetTextEditData(EQS_GetNthIOC(EP_TEXT_EDIT_1), PMMsg.PMText);

            if (STRLEN(PMMsg.PMText) ==0)
                STRCPY(PMMsg.PMTitle, "(내용없음)");
            else if(STRLEN(PMMsg.PMText) <= LCD_COLUME_SIZE-2)
                STRCPY(PMMsg.PMTitle, PMMsg.PMText);
            else  
                STRNCPY(PMMsg.PMTitle, PMMsg.PMText, LCD_COLUME_SIZE-2);

            for(m=RP_NUM_EDIT_1;m<RP_EDIT_MAX;m++)//for(m=EP_NUM_EDIT_1;m<EP_EDIT_MAX;m++)
            {
#ifdef email_version
                EQC_GetTextEditData(EQS_GetNthIOC(m), getValue);
#else
	            EQC_GetNumberEditValue(EQS_GetNthIOC(m), getValue);
#endif //email_version
	            if(getValue[0])
                {   
                    STRCPY(PMMsg.MIN[PMMsg.MIN_NUM],getValue);
		            PMMsg.MIN_NUM++;
	            }		
            }
            hRPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SAVING_PM),	//저장중입니다.
		    		            	1, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hRPControl,RP_MES_E_11);
            EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        else if(pEvent->dwParam == RP_MES_E_11)
        {
           if(DataChanged || PMMsg.DecoNum!=oldDecoNum)
           { 
                SKY_SavePM();
           }
           else
           {
                SKY_ReadNWritePM(readData);
               
           }
            
            if(DataChanged || decoNum!=oldDecoNum)
            {
            
            hRPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCC_STORAGE),	//저장되었습니다.
		    		            	PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hRPControl,RP_MES_E_2);
            EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
            }
            
        }
        else if(pEvent->dwParam == RP_MES_E_2)
        {
            //전송이벤트
#ifndef WIN32
            nLen = SKY_PrepareTran(PMMsg.PMPhoto,PMMsg.DecoNum);
            g_SendState = PM_FAIL_ORIGINATED;  
        
             if(SKY_IS_NOSERVICE())
            {
		  	    g_SendState = PM_NOSERVICE;
                EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
			    break;
            }
		    else if(SKY_IS_MAINTREQ())
            {    
                g_SendState = PM_MAINTREQ;
                EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
                break;
            }
            
            for(nCtrl = EQS_GetIOCNum()-1; nCtrl >=0; nCtrl--)
            { 
                EQS_DeleteControl(EQS_GetNthIOC(nCtrl));
            }
            EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);
                      
            EQC_Image(PM_ANI_X, PM_ANI_Y, (BITMAP*)&g_BmpConnectDataService);
            //EQC_Image(PM_PANNEL_X,PM_PANNEL_Y, (BITMAP*)&g_BmpGeneralPannel);
            hControl = EQC_StaticText(PM_STATUS_X, PM_STATUS_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE);
            EQC_SET_STYLE(hControl,CS_STATIC_TEXT_ORING);

            g_SendPM=PM_FAIL;
            SKY_AcceptKey(FALSE);   // Originated 를 받으면 TRUE로 됨
		    SKY_ConnectHTTPServer();
            
            break;
            
#else       
            
            hRPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCCESS_SEND_PM),	//메일이 성공적으로 전송되었습니다.
						                PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hRPControl,RP_MES_E_3);
            EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        
#endif
		               
        }
        else if (pEvent->dwParam == RP_MES_E_5)
        {
             for(m=RP_NUM_EDIT_1;m<RP_EDIT_MAX;m++)
			{	
			    getValue[0]=0;
#ifdef email_version
                EQC_GetTextEditData(EQS_GetNthIOC(m), (BYTE*)getValue);
#else
			    EQC_GetNumberEditValue(EQS_GetNthIOC(m), (BYTE*)getValue);
#endif //email_version
						
                if (getValue[0]==0)
                {   
                    EQS_SetIOCFocus(EQS_GetNthIOC(m));
				    m = RP_EDIT_MAX;              
			    }
 		    }
        }
        else if(pEvent->dwParam == RP_MES_E_9)
        {   
            SKY_InitPMMsg(&PMMsg);
    
            PMMsg.Sent=FALSE;
    
            PMMsg.MIN_NUM = 0;
            PMMsg.DecoNum = decoNum;
            PMMsg.PMPhoto=dwTemp;
            PMMsg.Save_time = SKY_GetLocalTime();
            PMMsg.Send_time = SKY_GetLocalTime();
            EQC_GetTextEditData(EQS_GetNthIOC(RP_TEXT_EDIT_1), PMMsg.PMText);
            //EQC_GetTextEditData(EQS_GetNthIOC(EP_TEXT_EDIT_1), PMMsg.PMText);

            if (STRLEN(PMMsg.PMText) ==0)
                STRCPY(PMMsg.PMTitle, "(내용없음)");
            else if(STRLEN(PMMsg.PMText) <= LCD_COLUME_SIZE-2)
                STRCPY(PMMsg.PMTitle, PMMsg.PMText);
            else  
                STRNCPY(PMMsg.PMTitle, PMMsg.PMText, LCD_COLUME_SIZE-2);

            for(m=RP_NUM_EDIT_1;m<RP_EDIT_MAX;m++)//for(m=EP_NUM_EDIT_1;m<EP_EDIT_MAX;m++)
            {
#ifdef email_version
                EQC_GetTextEditData(EQS_GetNthIOC(m), getValue);
#else
                EQC_GetNumberEditValue(EQS_GetNthIOC(m), getValue);
#endif //email_version
	            if(getValue[0])
                {   
                    STRCPY(PMMsg.MIN[PMMsg.MIN_NUM],getValue);
		            PMMsg.MIN_NUM++;
	            }		
            }
            hRPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SAVING_PM),	//저장중입니다.
		    		            	1, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hRPControl,RP_MES_E_12);
            EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        else if(pEvent->dwParam == RP_MES_E_12)
        {
            

            SKY_SavePM();
           

            hRPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCC_STORAGE),	//저장되었습니다.
		    		            	PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hRPControl,RP_MES_E_4);
            EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        else if(pEvent->dwParam == RP_MES_E_3||pEvent->dwParam == RP_MES_E_4
                                             ||pEvent->dwParam ==   RP_MES_E_10)
        {   
            EQS_END();
        }
        break;

    case SE_SOCK_RELEASED:
        //SKY_AcceptKey(TRUE);
        SKY_SET_ORIGINATE(FALSE);
        
        EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA); 
        
        break;


    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        
        SKY_SET_ORIGINATE(TRUE);    // ui.originate = TRUE; 
        g_SendState = PM_SUCC_ORIGINATED;   
        EQS_START_CHILD_EVENT(sSKA_PMOriginated,SE_APP_START,nLen,EQS_NA);  
            
        break;

    case SE_CALL_ENDED:
        g_SendPM=PM_CONNECT_FAIL;
        SKY_DisconnectHTTPServer();
        
        break;

   case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == RP_MES_E_8)
        { 
            SKY_SET_NO_OFF_BACKLIGHT(TRUE);
            SKY_BacklightOn(AB_ALWAYS);

            if(DataChanged || decoNum!=oldDecoNum)
            {
                
                //유용메모리 확인
                AvailMem = SKY_GetFreeSpaceInDataFile();
               
                if( PM_MAX_BUF_SIZE1 > AvailMem ) 
                {
                    hRPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_STORAGE_SCARCITY), //저장공간이 부족합니다.
				        PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    EQC_SET_USERDATA(hRPControl,RP_MES_E_10);
       
                    break;
                }
                         
                hRPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_MSG_STORAGE), //사진은 나만의 사진에, 메세지는 포토메일에 저장됩니다. 
                               1, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hRPControl,RP_MES_E_1);
                EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) | CS_MESSAGE_BOX_IGNORE_KEYS);
            }
            else
            {   
                SKY_AcceptKey(FALSE);   // Originated 를 받으면 TRUE로 됨
                
                if(!PMMsg.Sent)
                {   
                    for(nCtrl = EQS_GetIOCNum()-1; nCtrl >=0; nCtrl--)
                    { 
                        EQS_DeleteControl(EQS_GetNthIOC(nCtrl));
                    }
                    EQS_SetWindow(SKY_DEF_WIN_X,
                            SKY_DEF_WIN_Y, 
                            SKY_DEF_WIN_DX, 
                            SKY_DEF_WIN_DY, 
                            CL_NONE, 
                            NULL, // show annunciators
                            WA_SHOW_HEADER);

                    EQC_Image(PM_ANI_X, PM_ANI_Y, (BITMAP*)&g_BmpConnectDataService);
                    //EQC_Image(PM_PANNEL_X,PM_PANNEL_Y, (BITMAP*)&g_BmpGeneralPannel);
                    hControl = EQC_StaticText(PM_STATUS_X, PM_STATUS_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE);
                    EQC_SET_STYLE(hControl,CS_STATIC_TEXT_ORING);
                    EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);
                    EQS_SEND_EVENT(SE_USER_1,EQS_NA,EQS_NA);
                    break;
                }
                else
                {
                    //전송이벤트
#ifndef WIN32
           
                    for(nCtrl = EQS_GetIOCNum()-1; nCtrl >=0; nCtrl--)
                    { 
                        EQS_DeleteControl(EQS_GetNthIOC(nCtrl));
                    }
                    EQS_SetWindow(SKY_DEF_WIN_X,
                            SKY_DEF_WIN_Y, 
                            SKY_DEF_WIN_DX, 
                            SKY_DEF_WIN_DY, 
                            CL_NONE, 
                            NULL, // show annunciators
                            WA_SHOW_HEADER);

                    EQC_Image(PM_ANI_X, PM_ANI_Y, (BITMAP*)&g_BmpConnectDataService);
                    //EQC_Image(PM_PANNEL_X,PM_PANNEL_Y, (BITMAP*)&g_BmpGeneralPannel);
                    hControl = EQC_StaticText(PM_STATUS_X, PM_STATUS_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE);
                    EQC_SET_STYLE(hControl,CS_STATIC_TEXT_ORING);
                    EQS_SEND_EVENT(SE_APP_DRAW,EQS_NA,EQS_NA);
                    EQS_SEND_EVENT(SE_USER_2,EQS_NA,EQS_NA);
               
#else       
                
                hRPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCCESS_SEND_PM),	//메일이 성공적으로 전송되었습니다.
						                    PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                EQC_SET_USERDATA(hRPControl,RP_MES_E_3);
                EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
#endif
                }
            }
          
        }
        else if(pEvent->dwParam == RP_MES_E_7)
        {
            EQS_END_EVENT(SE_APP_RESUME, EQS_NA ,PM_CLEAR1_POPUP); 
        }
		break;

	case SE_IO_MESGBOX_NO:
        if(pEvent->dwParam == RP_MES_E_8)
        {            
            if(DataChanged || decoNum!=oldDecoNum)
            {
                //유용메모리 확인
                AvailMem = SKY_GetFreeSpaceInDataFile();
                
                if( PM_MAX_BUF_SIZE1 > AvailMem )
                {
                    hRPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_STORAGE_SCARCITY), //저장공간이 부족합니다.
				        PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    EQC_SET_USERDATA(hRPControl,RP_MES_E_10);
       
                    break;
                }
                
                hRPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_MSG_STORAGE), //사진은 나만의 사진에, 메세지는 포토메일에 저장됩니다. 
                               1, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hRPControl,RP_MES_E_9);
                EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) | CS_MESSAGE_BOX_IGNORE_KEYS);  
            }
            else
            {
                EQS_END();
            }   
        
        }
        else if(pEvent->dwParam == RP_MES_E_7)
        {}
        break;

    case SE_IO_FOCUS_CHG:

        if(EQS_GetIOCOrderly(pEvent->wParam)==0)
            EQS_SetWindowCML(CL_MENU_NONE_SEND);
        else
            EQS_SetWindowCML(CL_MENU_INPUT_SEND);

                
        break;

     case SE_IO_POPUP_SELECT:
			
		switch(pEvent->dwParam)
        {   
            case TKN_PM_SAVE_PM:    //포토메일저장
                if(EQS_IsControlDataChanged()|| decoNum!=oldDecoNum)
                {
                    DataChanged=TRUE;
                
                    //전화번호 유무확인
                     for(m=RP_NUM_EDIT_1;m<RP_EDIT_MAX;m++)
                    {	
	  		             sTempValue[m-(RP_NUM_EDIT_1)][m]=0;
#ifdef email_version
                         EQC_GetTextEditData(EQS_GetNthIOC(m), sTempValue[m-(RP_NUM_EDIT_1)]);
#else
                         EQC_GetNumberEditValue(EQS_GetNthIOC(m), sTempValue[m-(RP_NUM_EDIT_1)]);
#endif //email_version
                        }
                        if(sTempValue[0][0]==0 && sTempValue[1][0]==0 && sTempValue[2][0]==0 &&
                           sTempValue[3][0]==0 && sTempValue[4][0]==0)
                        {
#ifdef email_version
                       hRPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_INPUT_RECEIVER), //받는 사람을 입력하세요.
                                           PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                       EQC_SET_USERDATA(hRPControl,RP_MES_E_5);
#else
                       hRPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_INPUT_PHONE_NUM), //전화번호를 입력하세요.
                                           PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                       EQC_SET_USERDATA(hRPControl,RP_MES_E_5);
#endif //email_version
                         break;
                    }
                    
                    //유용메모리 확인
                    AvailMem = SKY_GetFreeSpaceInDataFile();
                   
                    if( PM_MAX_BUF_SIZE1 > AvailMem )
                    {
                        hRPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_STORAGE_SCARCITY), //저장공간이 부족합니다.
				            PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                        EQC_SET_USERDATA(hRPControl,RP_MES_E_10);
   
                        break;
                    }
                    hRPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_MSG_STORAGE), //사진은 나만의 사진에, 메세지는 포토메일에 저장됩니다. 
                                   1, MBT_NONE, MBT_HELP);
                    EQC_SET_USERDATA(hRPControl,RP_MES_E_9);
                    EQC_SET_STYLE(hRPControl, EQC_GET_STYLE(hRPControl) | CS_MESSAGE_BOX_IGNORE_KEYS);  
                }
                else
                {
                    EQS_END();
                }

                break;
            case TKN_PM_PBOOK_SEARCH:  //전화부찾기
/*                if(pEvent->wParam == BOOKCODE_IN_PHOTOMAIL)
                {
                    EQS_START_CHILD(SKA_SearchBookNumber);
                    break;
                }
*/
                if(!SKY_IS_PHONE_BOOK_LOCK())
				{
                    //EQS_START_CHILD(SKA_SearchBookNumber);
					EQS_START_CHILD_EVENT( SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_SEARCH_BOOK_E);
				}
                else
                {
                    EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, \
                        BOOKCODE_IN_PHOTOMAIL, TKN_PM_PBOOK_SEARCH);
                } 
                break;
                
            case TKN_PM_CALLHISTORY_CALLTO:		//발신이력찾기
#ifdef FEATURE_CALL_STACK_SEPERATED
                SetReceivedCallStack(FALSE);
#endif//FEATURE_CALL_STACK_SEPERATED 
				EQS_START_CHILD(SKA_MainStackCode);
                break;

            case TKN_PM_CALLHISTORY_CALLED:		//착신이력찾기
#ifdef FEATURE_CALL_STACK_SEPERATED
                SetReceivedCallStack(TRUE);
#endif//FEATURE_CALL_STACK_SEPERATED
				EQS_START_CHILD(SKA_MainStackCode);
                break;
#ifdef email_version
            case TKN_PM_PB_EMAIL_SEARCH:        //상대주소 찾기
                if(SKY_IS_PHONE_BOOK_LOCK())
                    EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, BOOKCODE_IN_VM, EQS_NA);
                else
                    EQS_START_CHILD(SKA_ViewEmailList);
                break;
#endif //email_version

            case TKN_PM_DECO_PHOTO:         //사진꾸미기
#ifdef deco_photo
                EQS_START_CHILD_EVENT(sSKA_DecoPhoto,SE_APP_START,EQS_NA,(DWORD)decoNum);
#endif //deco_photo
                break;

        }
		break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;

    }
}
/*			
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_EditPM()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_EditPM(SKY_EVENT_T *pEvent)
{   
#ifndef WIN32
    extern BYTE	Image$$APP_SHARE_RAM$$Base[];
#endif

    
    STATIC DWORD dwTemp;
    STATIC BOOL sParentIsCamera;
//  STATIC INT defPhotoName =0;
    STATIC UINT8 decoNum;
    STATIC UINT16 nLen;
    STATIC HCONTROL hFocusedHnd;
    //STATIC BOOL bInputReceiver = FALSE;
    HCONTROL hEPControl;
    HCONTROL hControl;
    INT8 nCtrl;
    //BYTE sGarbage[CAMERA_BUF_SPACE];
    DWORD dwTimeInfo;
    BYTE sPhotoName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
    HFILE	hFile,nFiles;
    DWORD AvailMem ,sMaxSize;
    WORD        wMaxDataCount = 0, i, j;
    pb_number_info_type sNumberInfo;
    BYTE  getValue[PM_MAX_SIZE_MIN+1];
    BYTE sTempValue[5][PM_MAX_SIZE_MIN+1];
    BYTE sTempText[PM_MAX_SIZE_TEXT+2];
    INT m;
    CALLSTACK_T*                pCallStack;
    PHOTO_LIST_T l_PhotoList[MAX_PHOTO_COUNT];
#ifdef deco_photo
    DRAW_MODE DrawMode;
#endif
    UINT sHangleCount=0;
    UINT sMaxPhotoNameSize=0;
#ifdef email_version
    pb_email_info_type email_info;
#endif //email_version
   
#ifndef WIN32
    s_aCameraRevData = (byte*)Image$$APP_SHARE_RAM$$Base;
#endif


	switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

		//EQS_GetDC(TRUE);
        decoNum=0;
        if(EQS_GET_PARENT() == SKA_MainCamera )
        {
            EQS_SetWindow(SKY_DEF_WIN_X, 
                          SKY_DEF_WIN_Y, 
                          SKY_DEF_WIN_DX, 
                          310,//SKY_DEF_WIN_DY,
                          CL_MENU_NONE_SEND,
                          SKY_GET_TOKEN(TKN_PM_EDIT_PM),	//포토메일작성
                          WA_SHOW_CML | WA_SHOW_HEADER);
        
            sParentIsCamera=TRUE;
            g_InPhotoName=FALSE;
                                               
            BitmapCameraInfo.Header.abID[0] = 'I';
            BitmapCameraInfo.Header.abID[1] = 'M';
            BitmapCameraInfo.Header.abID[2] = 'G';
            BitmapCameraInfo.Header.Size.xWidth = PHOTO_H_SIZE;
            BitmapCameraInfo.Header.Size.yHeight = PHOTO_V_SIZE;
            BitmapCameraInfo.Header.xWidth = PHOTO_H_SIZE;//COLOR_IDLE_IMAGE_WIDTH;
            BitmapCameraInfo.Header.nBitsPerPixel = SKY_COLOR_SPACE;//COLOR_IDLE_IMAGE_BPP;
            BitmapCameraInfo.Header.fPalette = FALSE;
            BitmapCameraInfo.Header.TransparentColor = MAX_COLOR;
            BitmapCameraInfo.Header.yStart = 0;
            //BitmapCameraInfo.Header.Size.yHeight =PHOTO_V_SIZE;
            
             if(pEvent->wParam == CAMERA_TURN)
            {   
                wMaxDataCount =  PHOTO_TOTAL_SIZE/2;
                j = 0;
			    i = wMaxDataCount-1;
			    for(;;)
			    {
				    memcpy(&s_aCameraRevData[(j++)*2],&g_abBmpBuf[i*2],sizeof(WORD));	
				    if ( i == 0 )	break;
				    i--;
			    }   
                
                BitmapCameraInfo.pbData = s_aCameraRevData;
                
            }
            else
            {
                BitmapCameraInfo.pbData=g_abBmpBuf;
            }
        } 
        else if(EQS_GET_PARENT() == SKA_EditNewPM)
        {   
            EQS_SetWindow(SKY_DEF_WIN_X, 
                          SKY_DEF_WIN_Y, 
                          SKY_DEF_WIN_DX, 
                          SKY_DEF_WIN_DY,
                          CL_MENU_NONE_SEND,
                          SKY_GET_TOKEN(TKN_PM_EDIT_PM),	//포토메일작성
                          WA_SHOW_CML | WA_SHOW_HEADER);

            sParentIsCamera=FALSE;
                        
            dwTemp=pEvent->dwParam;
            ConvertCameraImageFileName(sPhotoName, pEvent->dwParam); 
            if(SKY_OpenDataFile(SDF_PHOTO, sPhotoName, &hFile, SFT_READ) == SDR_OK)
            {
                SKY_ReadDataBlock(hFile, sPhotoName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(DWORD));
                SKY_ReadDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));  
                //SKY_ReadDataBlock(hFile, sGarbage, CAMERA_BUF_SPACE); 
                SKY_ReadDataBlock(hFile, g_abBmpBuf, PHOTO_TOTAL_SIZE);
                BitmapCameraInfo.pbData= g_abBmpBuf;
                SKY_CloseDataFile(hFile); 
                //BitmapCameraInfo.Header.yStart = 15;
                //BitmapCameraInfo.Header.Size.yHeight =90;
                
            }
            else
            {
                hEPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_NOT_OPEN_PHOTO),	//사진을 열수 없습니다.
				            PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_CONFIRM);
                EQC_SET_USERDATA(hEPControl, EP_MES_E_12);
                EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
            }
        }
        hControl = EQC_Image(EQS_NA,EQS_NA,&BitmapCameraInfo);
        EQC_SET_FOCUS(hControl, TRUE);

        EQC_StaticText(EQS_NA,EQS_NA,  TKN_PM_MSG_CONTENT);	// 메세지내용
		EQC_TextEdit(EQS_NA,EQS_NA,  PM_EDIT_SIZE, 5, TEM_KOREAN, 80, (BYTE*)"", 0);
#ifdef email_version
        EQC_StaticText(EQS_NA,EQS_NA, TKN_PM_RECEIVER);      //받는 사람
        EQC_TextEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, TEM_NUMBER, 
                     PM_MAX_SIZE_MIN, (BYTE*)"", 0); 
        EQC_TextEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, TEM_NUMBER, 
                     PM_MAX_SIZE_MIN, (BYTE*)"", 0); 
        EQC_TextEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, TEM_NUMBER, 
                     PM_MAX_SIZE_MIN, (BYTE*)"", 0); 
        EQC_TextEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, TEM_NUMBER, 
                     PM_MAX_SIZE_MIN, (BYTE*)"", 0); 
        EQC_TextEdit(EQS_NA, EQS_NA, PM_EDIT_SIZE, 1, TEM_NUMBER, 
                     PM_MAX_SIZE_MIN, (BYTE*)"", 0); 
#else
        EQC_StaticText(EQS_NA,EQS_NA, TKN_PM_OTHER_PARTY_CALL_NUM); // 상대방 전화번호 
        EQC_NumberEdit(EQS_NA,EQS_NA, PM_EDIT_SIZE, 1, 11, (BYTE*)"");
        EQC_NumberEdit(EQS_NA,EQS_NA, PM_EDIT_SIZE, 1, 11, (BYTE*)"");
        EQC_NumberEdit(EQS_NA,EQS_NA, PM_EDIT_SIZE, 1, 11, (BYTE*)"");
        EQC_NumberEdit(EQS_NA,EQS_NA, PM_EDIT_SIZE, 1, 11, (BYTE*)"");
        EQC_NumberEdit(EQS_NA,EQS_NA, PM_EDIT_SIZE, 1, 11, (BYTE*)"");
#endif //email_version
        EQS_StartFormatter();
		
		for(nCtrl = 0; nCtrl < EQS_GetIOCNum(); nCtrl++)
        {   
            if(nCtrl == EP_STATIC_TEXT_1 || nCtrl== EP_STATIC_TEXT_2)
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            }
            else
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
                EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
            }
            EQS_NewLine();
        }

        EQS_EndFormatter();
        EQS_FitCanvasSize();
        
        EQC_SET_ENABLE(EQS_GetNthIOC(EP_PHOTO), FALSE);
        EQC_SET_FOCUS(EQS_GetNthIOC(EP_STATIC_TEXT_1),FALSE);
        EQC_SET_FOCUS(EQS_GetNthIOC(EP_STATIC_TEXT_2),FALSE);

        break;

    case SE_APP_END :   /* The very last event */ 
        //SKY_DisconnectHTTPServer();
        SKY_AcceptKey(TRUE);
        SKY_SET_PM(FALSE);//jmpark
		//EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_DRAW:
#ifdef deco_photo            
         switch(decoNum)
         {   
            case 0:
                    
                break;
            case 1:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,1,&g_BmpFlheart120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 2:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpBear120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 3:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpCat120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 4:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpElle120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 5:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpFlfarm120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 6:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpFlower120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 7:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpLove120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 8:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpMonst120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 9:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpRabbit120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;
            case 10:
                DrawMode  = EQS_GetDrawMode();
                EQS_SetDrawMode(DM_OR);
                EQS_PutBitmap(0,0,&g_BmpTv120x90x256c);
                EQS_SetDrawMode(DrawMode);
                break;         
         }
#endif //deco_photo         
        break;    
    case SE_APP_RESUME: /* child applet is ended */
        if(EQS_GET_CHILD() == SKA_SpeedSearch)
        {
            if( pEvent->dwParam )
            {// 값을 선택 
                sNumberInfo.address = pEvent->dwParam;
        
                if( ReadNumber(&sNumberInfo) == TRUE )
                {
#ifdef email_version
                    EQC_SetTextEditData(hFocusedHnd, sNumberInfo.digits);
#else
				    EQC_SetNumberEditValue(hFocusedHnd, sNumberInfo.digits);
#endif //email_version
                    EQC_SET_DIRTY(hFocusedHnd, TRUE);
                }
                EQS_SetIOCFocus(EQS_GetNthIOC(EQS_GetIOCOrderly(hFocusedHnd)+1));
            }
            else
            {  //취소 

            }
        }
        else if(EQS_GET_CHILD() == SKA_MainCallList)
        {
            if(GetCallStackIndex() != 255)
            { 
                pCallStack = GetCSKData();
                
				//EQC_SetNumberEditValue(EQS_GetNthIOC(m), pCallStack->Stack[GetCallStackIndex()].aDigits);
#ifdef email_version
                    EQC_SetTextEditData(hFocusedHnd, g_PhoneNumBuf);
#else
				    EQC_SetNumberEditValue(hFocusedHnd, g_PhoneNumBuf);
#endif //email_version                
                
                EQC_SET_DIRTY(hFocusedHnd, TRUE);
                
                SetCallStackIndex(255);
            }
            else
            {
            }
        }
#ifdef email_version
        else if(EQS_GET_CHILD() == SKA_ViewEmailList)
        {
            if(pEvent->dwParam != NULL)
            {
                email_info.address = pEvent->dwParam;
                ReadEmail(&email_info);

				EQC_SetTextEditData(hFocusedHnd, email_info.szMailID);
                EQC_SET_DIRTY(hFocusedHnd, TRUE);
               
                EQS_SetIOCFocus(EQS_GetNthIOC(EQS_GetIOCOrderly(hFocusedHnd)+1));
            
                
            }
            else
            {
            }
        }
#endif //email_version
        else if(EQS_GET_CHILD() == SKA_MainStackCode)
            break;

        else if(EQS_GET_CHILD() == sSKA_PhotoNameSave )
            break;

        else if(EQS_GET_CHILD() == sSKA_DecoPhoto )
        {
#ifdef deco_photo
            if(pEvent->wParam != PM_CLEAR_POPUP)
            {decoNum = (UINT8)pEvent->dwParam;}
#endif //deco_photo                   
        }
        else if(EQS_GET_CHILD() == sSKA_PMOriginated||EQS_GET_CHILD() ==sSKA_PMHTTPInProg)
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);        
        }
        else if(EQS_GET_CHILD() == SKA_MainPbookCode)
        {
            break;
        }

        else
        {   
            EQS_END_EVENT(SE_APP_RESUME, PM_END_POPUP, EQS_NA);            
        }
        break;   
 
    case SE_KEY_CLEAR:
        hEPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_EDIT_CANCEL),	//포토메일 작성을 취소할까요?
							 MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, 
							 MBT_CONFIRM);
        EQC_SET_USERDATA(hEPControl,EP_MES_E_15);
        
        break;
   
    case SE_SOFTKEY_MENU:
  
        hFocusedHnd = EQS_GetFocusedIOC(); 
#ifdef email_version
#ifdef deco_photo
        if(hFocusedHnd == EQS_GetNthIOC(EP_PHOTO))
        {   
            if(sParentIsCamera)
            {
                pabEditPMMenu[0] = TRUE;
                pabEditPMMenu[1] = TRUE;
                pabEditPMMenu[2] = TRUE;
                pabEditPMMenu[3] = FALSE;
                pabEditPMMenu[4] = FALSE;
                pabEditPMMenu[5] = FALSE;
            }
            else
            {   
                pabEditPMMenu[0] = TRUE;
                pabEditPMMenu[1] = TRUE;
                pabEditPMMenu[2] = FALSE;
                pabEditPMMenu[3] = FALSE;
                pabEditPMMenu[4] = FALSE;
                pabEditPMMenu[5] = FALSE;
                pabEditPMMenu[6] = FALSE;
            }

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,7,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
        else if(hFocusedHnd >= EQS_GetNthIOC(RP_NUM_EDIT_1))
        {   
            
            pabEditPMMenu[0] = TRUE;
            pabEditPMMenu[1] = FALSE;
            if(sParentIsCamera)
            {
                pabEditPMMenu[2] = TRUE;
            }
            else
            {
                pabEditPMMenu[2] = FALSE;
            }
            pabEditPMMenu[3] = TRUE;
            pabEditPMMenu[4] = TRUE;
            pabEditPMMenu[5] = TRUE;
            pabEditPMMenu[6] = TRUE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,7,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
   
        }
        else
        {
            pabEditPMMenu[0] = TRUE;
            pabEditPMMenu[1] = FALSE;
            if(sParentIsCamera)
            {
                pabEditPMMenu[2] = TRUE;
            }
            else
            {
                pabEditPMMenu[2] = FALSE;
            }
            pabEditPMMenu[3] = FALSE;
            pabEditPMMenu[4] = FALSE;
            pabEditPMMenu[5] = FALSE;
            pabEditPMMenu[6] = FALSE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,7,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
#else //deco_photo
        
        if(hFocusedHnd == EQS_GetNthIOC(EP_PHOTO))
        {   
            if(sParentIsCamera)
            {
                pabEditPMMenu[0] = TRUE;
                pabEditPMMenu[1] = TRUE;
                pabEditPMMenu[2] = FALSE;
                pabEditPMMenu[3] = FALSE;
                pabEditPMMenu[4] = FALSE;
                pabEditPMMenu[5] = FALSE;
            }
            else
            {   
                pabEditPMMenu[0] = TRUE;
                pabEditPMMenu[1] = FALSE;
                pabEditPMMenu[2] = FALSE;
                pabEditPMMenu[3] = FALSE;
                pabEditPMMenu[4] = FALSE;
                pabEditPMMenu[5] = FALSE;
            }
            
            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,6,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
        else if(hFocusedHnd >= EQS_GetNthIOC(RP_NUM_EDIT_1))
        {   
            
            pabEditPMMenu[0] = TRUE;
            if(sParentIsCamera)
            {
                pabEditPMMenu[1] = TRUE;
            }
            else
            {
                pabEditPMMenu[1] = FALSE;
            }
            pabEditPMMenu[2] = TRUE;
            pabEditPMMenu[3] = TRUE;
            pabEditPMMenu[4] = TRUE;
            pabEditPMMenu[5] = TRUE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,6,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        
        }
        else
        {   
            pabEditPMMenu[0] = TRUE;
            if(sParentIsCamera)
            {
                pabEditPMMenu[1] = TRUE;
            }
            else
            {
                pabEditPMMenu[1] = FALSE;
            }
            pabEditPMMenu[2] = FALSE;
            pabEditPMMenu[3] = FALSE;
            pabEditPMMenu[4] = FALSE;
            pabEditPMMenu[5] = FALSE;
            
            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,6,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
#endif //deco_photo
#else //email_version
#ifdef deco_photo
        if(hFocusedHnd == EQS_GetNthIOC(EP_PHOTO))
        {   
            if(sParentIsCamera)
            {
                pabEditPMMenu[0] = TRUE;
                pabEditPMMenu[1] = TRUE;
                pabEditPMMenu[2] = TRUE;
                pabEditPMMenu[3] = FALSE;
                pabEditPMMenu[4] = FALSE;
                pabEditPMMenu[5] = FALSE;
            }
            else
            {   
                pabEditPMMenu[0] = TRUE;
                pabEditPMMenu[1] = TRUE;
                pabEditPMMenu[2] = FALSE;
                pabEditPMMenu[3] = FALSE;
                pabEditPMMenu[4] = FALSE;
                pabEditPMMenu[5] = FALSE;
            }

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,6,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
        else if(hFocusedHnd >= EQS_GetNthIOC(RP_NUM_EDIT_1))
        {   
            
            pabEditPMMenu[0] = TRUE;
            pabEditPMMenu[1] = FALSE;
            if(sParentIsCamera)
            {
                pabEditPMMenu[2] = TRUE;
            }
            else
            {
                pabEditPMMenu[2] = FALSE;
            }
            pabEditPMMenu[3] = TRUE;
            pabEditPMMenu[4] = TRUE;
            pabEditPMMenu[5] = TRUE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,6,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        

        }
        else
        {
            pabEditPMMenu[0] = TRUE;
            pabEditPMMenu[1] = FALSE;
            if(sParentIsCamera)
            {
                pabEditPMMenu[2] = TRUE;
            }
            else
            {
                pabEditPMMenu[2] = FALSE;
            }
            pabEditPMMenu[3] = FALSE;
            pabEditPMMenu[4] = FALSE;
            pabEditPMMenu[5] = FALSE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,6,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
#else//deco_photo
        
        if(hFocusedHnd == EQS_GetNthIOC(EP_PHOTO))
        {   
            if(sParentIsCamera)
            {
                pabEditPMMenu[0] = TRUE;
                pabEditPMMenu[1] = TRUE;
                pabEditPMMenu[2] = FALSE;
                pabEditPMMenu[3] = FALSE;
                pabEditPMMenu[4] = FALSE;
            }
            else
            {   
                pabEditPMMenu[0] = TRUE;
                pabEditPMMenu[1] = FALSE;
                pabEditPMMenu[2] = FALSE;
                pabEditPMMenu[3] = FALSE;
                pabEditPMMenu[4] = FALSE;
            }
            
            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,5,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
        else if(hFocusedHnd >= EQS_GetNthIOC(RP_NUM_EDIT_1))
        {   
            
            pabEditPMMenu[0] = TRUE;
            if(sParentIsCamera)
            {
                pabEditPMMenu[1] = TRUE;
            }
            else
            {
                pabEditPMMenu[1] = FALSE;
            }
            pabEditPMMenu[2] = TRUE;
            pabEditPMMenu[3] = TRUE;
            pabEditPMMenu[4] = TRUE;

            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,5,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        
        }
        else
        {   
            pabEditPMMenu[0] = TRUE;
            if(sParentIsCamera)
            {
                pabEditPMMenu[1] = TRUE;
            }
            else
            {
                pabEditPMMenu[1] = FALSE;
            }
            pabEditPMMenu[2] = FALSE;
            pabEditPMMenu[3] = FALSE;
            pabEditPMMenu[4] = FALSE;
            
            hControl=EQC_ExceptPopUpMenu((EQS_TOKEN_EX*)paEditPMMenu,5,0,pabEditPMMenu);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_POPUP_RETURN_TOKEN );
        }
#endif //deco_photo
#endif //email_version
        break;

/*
    case SE_IO_TEDIT_DONE:

        memset(sTempValue,NULL,PM_MAX_SIZE_MIN+1);
#ifdef email_version
        EQC_GetTextEditData(pEvent->wParam, (BYTE*)sTempValue);
#else
		EQC_GetNumberEditValue(pEvent->wParam, (BYTE*)sTempValue);
#endif //email_version
        if(sTempValue[0]!=NULL)
           bInputReceiver = TRUE;
        
        break;
*/        
    case SE_SOFTKEY_SEND:
        //전화번호 유무확인 
        for(m=EP_NUM_EDIT_1;m<EP_EDIT_MAX;m++)
        {	
	  	    sTempValue[m-EP_NUM_EDIT_1][m]=0;
#ifdef email_version
            EQC_GetTextEditData(EQS_GetNthIOC(m), (BYTE*)sTempValue[m-EP_NUM_EDIT_1]);
#else
		 	EQC_GetNumberEditValue(EQS_GetNthIOC(m), (BYTE*)sTempValue[m-EP_NUM_EDIT_1]);
#endif //email_version
        }
        if(sTempValue[0][0]==0 && sTempValue[1][0]==0 && sTempValue[2][0]==0 &&
                                    sTempValue[3][0]==0 && sTempValue[4][0]==0)
        {
#ifdef email_version
             hEPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_INPUT_RECEIVER), //받는 사람을 입력하세요.
                                 PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
             EQC_SET_USERDATA(hEPControl,EP_MES_E_6);
#else
             hEPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_INPUT_PHONE_NUM), //전화번호를 입력하세요.
                                 PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
             EQC_SET_USERDATA(hEPControl,EP_MES_E_6);
#endif //email_version
             break;
        }
          
        hEPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_QUS_SEND_PM),	//지금 전송하시겠습니까?
							 MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, 
							 MBT_CONFIRM);
        EQC_SET_USERDATA(hEPControl,EP_MES_E_1);

        break;

    case SE_IO_MESGBOX_END:
        if (pEvent->dwParam == EP_MES_E_2 )
        {   
            SKY_InitPMMsg(&PMMsg);

            PMMsg.Sent=TRUE;
            PMMsg.MIN_NUM = 0;
            PMMsg.DecoNum = decoNum;
            PMMsg.PMPhoto=dwTemp;
            PMMsg.Save_time = SKY_GetLocalTime();
            PMMsg.Send_time = SKY_GetLocalTime();
            EQC_GetTextEditData(EQS_GetNthIOC(RP_TEXT_EDIT_1), PMMsg.PMText);
            //EQC_GetTextEditData(EQS_GetNthIOC(EP_TEXT_EDIT_1), PMMsg.PMText);

            if (STRLEN(PMMsg.PMText) ==0)
                STRCPY(PMMsg.PMTitle, "(내용없음)");
            else if(STRLEN(PMMsg.PMText) <= LCD_COLUME_SIZE-2)
                STRCPY(PMMsg.PMTitle, PMMsg.PMText);
            else  
                STRNCPY(PMMsg.PMTitle, PMMsg.PMText, LCD_COLUME_SIZE-2);

            for(m=RP_NUM_EDIT_1;m<RP_EDIT_MAX;m++)//for(m=EP_NUM_EDIT_1;m<EP_EDIT_MAX;m++)
            {	
#ifdef email_version
                EQC_GetTextEditData(EQS_GetNthIOC(m), getValue);
#else
	            EQC_GetNumberEditValue(EQS_GetNthIOC(m), getValue);
#endif //email_version
	            if(getValue[0])
                {   
                    STRCPY(PMMsg.MIN[PMMsg.MIN_NUM],getValue);
		            PMMsg.MIN_NUM++;
	            }		
            }
            hEPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SAVING_PM),	//저장중입니다.
		    		            	1, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hEPControl,EP_MES_E_13);
            EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        else if(pEvent->dwParam == EP_MES_E_13)
        {   
            if(sParentIsCamera)
            {   
                PMMsg.PMPhoto = SKY_GET_CAMERA_COUNT();
                memset(sPhotoName,NULL,SKY_FILE_EXT_NAME_MAX_LENGTH + 1);
                //memset(sTempText,NULL,PM_MAX_SIZE_TEXT+2);
                if(g_InPhotoName)
                    STRCPY(sPhotoName, l_PhotoName);
                else
                {
                    EQC_GetTextEditData(EQS_GetNthIOC(EP_TEXT_EDIT_1), sTempText);
                
                    if(STRLEN(sTempText) <= CAMERA_NAME_EDIT_MAX_LEN)  //SKY_MAX_CAMERA_IMAGE_FILE_LEN-2 = 16 
                    {
                        STRCPY(sPhotoName, sTempText);
                    }
                    else
                    {
                        while( sTempText[sMaxPhotoNameSize] != NULL )
                        {
                            if(sTempText[sMaxPhotoNameSize] >= 0x80)       // 한글
                            {
	                            sHangleCount ++;
                            }
                            sMaxPhotoNameSize ++;
                            if(sMaxPhotoNameSize == CAMERA_NAME_EDIT_MAX_LEN)
                              break;
                        }//while
                    
                        if(sHangleCount%2)
                        {
	                        memcpy(sPhotoName, sTempText, CAMERA_NAME_EDIT_MAX_LEN-1);
                        }else
                        { 
	                        memcpy(sPhotoName, sTempText, CAMERA_NAME_EDIT_MAX_LEN);
                        }
                        //STRNCPY(sPhotoName, sTempText, CAMERA_NAME_EDIT_MAX_LEN);
                    }

                }
                if(sPhotoName[0] == 0)
                {   
                    sprintf((char *)sPhotoName,"%s","사진");
                }

                dwTimeInfo = SKY_GetLocalTime();
                //memset((VOID*)g_abBmpBuf, 0xFF, CAMERA_BUF_SPACE);    //init
	
		        hFile = SKY_MakeCameraImageFile(SKY_GET_CAMERA_COUNT());
                if(hFile != SKY_INVALID_FILE_HANDLE)
                {   
                    //BitmapCameraInfo.Header.yStart = 0;
                    //BitmapCameraInfo.Header.Size.yHeight =PHOTO_V_SIZE;
                    SKY_WriteDataBlock(hFile, sPhotoName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);      //File Name
                    SKY_WriteDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));              //Time Info
                    SKY_WriteDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));
                    //SKY_WriteDataBlock(hFile, g_abBmpBuf, CAMERA_BUF_SPACE);
                    SKY_WriteDataBlock(hFile, BitmapCameraInfo.pbData, PHOTO_TOTAL_SIZE);
                    //SKY_WriteDataBlock(hFile, g_abBmpBuf, CAMERA_BUF_SPACE);
                    SKY_CloseDataFile(hFile);
                   
                }

                 // Change List
                hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
                SKY_ReadFile(hFile, (CONST void *)l_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                
                for(i=0 ; i<MAX_PHOTO_COUNT;i++)
                { 
                    if(l_PhotoList[i].PhotoList == NULL)
                    {                                              
                        l_PhotoList[i].PhotoList = SKY_GET_CAMERA_COUNT()-1;
                        STRCPY(l_PhotoList[i].PhotoName, sPhotoName);

                        i = MAX_PHOTO_COUNT;
                    }
                }   
                SKY_SeekFile(hFile, FS_SO_SET, 0);
                SKY_WriteFile(hFile, (CONST void *)l_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                SKY_CloseFile(hFile);

                
            }
            else
            {
                PMMsg.PMPhoto =dwTemp;
            }
            SKY_SavePM();

            hEPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCC_STORAGE),	//저장되었습니다.
		    		            	PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hEPControl,EP_MES_E_3);
            EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
            
        }
        else if(pEvent->dwParam == EP_MES_E_3)
        {
            //전송이벤트
#ifndef WIN32
            nLen = SKY_PrepareTran(PMMsg.PMPhoto,PMMsg.DecoNum);
            g_SendState = PM_FAIL_ORIGINATED;  
        
             if(SKY_IS_NOSERVICE())
            {
		  	    g_SendState = PM_NOSERVICE;
                EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
			    break;
            }
		    else if(SKY_IS_MAINTREQ())
            {    
                g_SendState = PM_MAINTREQ;
                EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA);  
                break;
            }
            for(nCtrl = EQS_GetIOCNum()-1; nCtrl >=0; nCtrl--)
            { 
                EQS_DeleteControl(EQS_GetNthIOC(nCtrl));
            }
            EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        NULL, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);
 
            EQC_Image(PM_ANI_X, PM_ANI_Y, (BITMAP*)&g_BmpConnectDataService);
            //EQC_Image(PM_PANNEL_X,PM_PANNEL_Y, (BITMAP*)&g_BmpGeneralPannel);
            hControl = EQC_StaticText(PM_STATUS_X, PM_STATUS_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE);
            EQC_SET_STYLE(hControl,CS_STATIC_TEXT_ORING);

            g_SendPM=PM_FAIL;
            SKY_AcceptKey(FALSE);   // Originated 를 받으면 TRUE로 됨
		    SKY_ConnectHTTPServer();
            
            break;
#else       
            
            hEPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCCESS_SEND_PM),	//메일이 성공적으로 전송되었습니다.
						PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hEPControl,EP_MES_E_5);
            EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
#endif       
          
        }
        else if (pEvent->dwParam == EP_MES_E_4)
        {
             SKY_InitPMMsg(&PMMsg);
    
            PMMsg.Sent=FALSE;
            PMMsg.MIN_NUM = 0;
            PMMsg.DecoNum = decoNum;
            PMMsg.PMPhoto=dwTemp;
            PMMsg.Save_time = SKY_GetLocalTime();
            PMMsg.Send_time = SKY_GetLocalTime();
            EQC_GetTextEditData(EQS_GetNthIOC(RP_TEXT_EDIT_1), PMMsg.PMText);
            //EQC_GetTextEditData(EQS_GetNthIOC(EP_TEXT_EDIT_1), PMMsg.PMText);

            if (STRLEN(PMMsg.PMText) ==0)
                STRCPY(PMMsg.PMTitle, "(내용없음)");
            else if(STRLEN(PMMsg.PMText) <= LCD_COLUME_SIZE-2)
                STRCPY(PMMsg.PMTitle, PMMsg.PMText);
            else  
                STRNCPY(PMMsg.PMTitle, PMMsg.PMText, LCD_COLUME_SIZE-2);

            for(m=RP_NUM_EDIT_1;m<RP_EDIT_MAX;m++)//for(m=EP_NUM_EDIT_1;m<EP_EDIT_MAX;m++)
            {	
#ifdef email_version
                EQC_GetTextEditData(EQS_GetNthIOC(m), getValue);
#else
	            EQC_GetNumberEditValue(EQS_GetNthIOC(m), getValue);
#endif //email_version
	            if(getValue[0])
                {   
                    STRCPY(PMMsg.MIN[PMMsg.MIN_NUM],getValue);
		            PMMsg.MIN_NUM++;
	            }		
            }
            hEPControl =EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SAVING_PM),	//저장중입니다.
		    		            	1, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hEPControl,EP_MES_E_14);
            EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        else if (pEvent->dwParam == EP_MES_E_14)
        {   
           
            if(sParentIsCamera)
            {
                PMMsg.PMPhoto = SKY_GET_CAMERA_COUNT();
                memset(sPhotoName,NULL,SKY_FILE_EXT_NAME_MAX_LENGTH + 1);
                //memset(sTempText,NULL,PM_MAX_SIZE_TEXT+2);
                if(g_InPhotoName)
                    STRCPY(sPhotoName, l_PhotoName);
                else
                {
                    EQC_GetTextEditData(EQS_GetNthIOC(EP_TEXT_EDIT_1), sTempText);
                
                    if(STRLEN(sTempText) <= CAMERA_NAME_EDIT_MAX_LEN) //CAMERA_NAME_EDIT_MAX_LEN=14
                    {
                        STRCPY(sPhotoName, sTempText);
                    }
                    else
                    {   
                        while( sTempText[sMaxPhotoNameSize] != NULL )
                        {
                            if(sTempText[sMaxPhotoNameSize] >= 0x80)       // 한글
                            {
	                            sHangleCount ++;
                            }
                            sMaxPhotoNameSize ++;
                            if(sMaxPhotoNameSize == CAMERA_NAME_EDIT_MAX_LEN)
                              break;
                        }//while
                    
                        if(sHangleCount%2)
                        {
	                        memcpy(sPhotoName, sTempText, CAMERA_NAME_EDIT_MAX_LEN-1); //CAMERA_NAME_EDIT_MAX_LEN(14byte)내에 한글이 홀수byte일 때
                        }else
                        { 
	                        memcpy(sPhotoName, sTempText, CAMERA_NAME_EDIT_MAX_LEN);   //CAMERA_NAME_EDIT_MAX_LEN(14byte)내에 한글이 짝수byte일 때
                        }
                        //STRNCPY(sPhotoName, sTempText, CAMERA_NAME_EDIT_MAX_LEN);
                    }

                }
                if(sPhotoName[0] == 0)
                {
                   sprintf((char *)sPhotoName,"%s","사진");
                }
                
                dwTimeInfo = SKY_GetLocalTime();
                //memset((VOID*)g_abBmpBuf, 0xFF, CAMERA_BUF_SPACE);    //init
	
		        hFile = SKY_MakeCameraImageFile(SKY_GET_CAMERA_COUNT());
                if(hFile != SKY_INVALID_FILE_HANDLE)
                {   
                    //BitmapCameraInfo.Header.yStart = 0;
                    //BitmapCameraInfo.Header.Size.yHeight =PHOTO_V_SIZE;
                    SKY_WriteDataBlock(hFile, sPhotoName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);      //File Name
                    SKY_WriteDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));              //Time Info
                    SKY_WriteDataBlock(hFile, &BitmapCameraInfo.Header, sizeof(BitmapCameraInfo.Header));
                    //SKY_WriteDataBlock(hFile, g_abBmpBuf, CAMERA_BUF_SPACE);
                    SKY_WriteDataBlock(hFile, BitmapCameraInfo.pbData, PHOTO_TOTAL_SIZE);
                    //SKY_WriteDataBlock(hFile, g_abBmpBuf, CAMERA_BUF_SPACE);
                    SKY_CloseDataFile(hFile);

                }
                // Change List
                hFile = SKY_OpenFile(FILE_NAME_CAMERA, FS_AM_READWRITE);
                SKY_ReadFile(hFile, (CONST void *)l_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);

                for(i=0 ; i<MAX_PHOTO_COUNT;i++)
                { 
                    if(l_PhotoList[i].PhotoList == NULL)
                    {
                        l_PhotoList[i].PhotoList = SKY_GET_CAMERA_COUNT()-1;
                        STRCPY(l_PhotoList[i].PhotoName, sPhotoName);

                        i = MAX_PHOTO_COUNT;
                    }
                }   
                SKY_SeekFile(hFile, FS_SO_SET, 0);
                SKY_WriteFile(hFile, (CONST void *)l_PhotoList,sizeof(PHOTO_LIST_T)*MAX_PHOTO_COUNT);
                SKY_CloseFile(hFile);

                              
            }
            else
            {
                PMMsg.PMPhoto = dwTemp;
            }

            SKY_SavePM();

            hEPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_SUCC_STORAGE),	//저장되었습니다.
					                	PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hEPControl,EP_MES_E_7);
            EQC_SET_STYLE(hEPControl,EQC_GET_STYLE(hEPControl) | CS_MESSAGE_BOX_IGNORE_KEYS);
                
        }
        else if ( pEvent->dwParam == EP_MES_E_6)
        {
            for(m=EP_NUM_EDIT_1;m<EP_EDIT_MAX;m++)
			{	
			    getValue[0]=0;
#ifdef email_version
                EQC_GetTextEditData(EQS_GetNthIOC(m), (BYTE*)getValue);
#else
			    EQC_GetNumberEditValue(EQS_GetNthIOC(m), (BYTE*)getValue);
#endif //email_version						
                if (getValue[0]==0)
                {   
                    EQS_SetIOCFocus(EQS_GetNthIOC(m));
				    m = EP_EDIT_MAX;
          
			    }
 		    }        
        }
        else if ( pEvent->dwParam == EP_MES_E_5 
                                        || pEvent->dwParam == EP_MES_E_7 
                                        || pEvent->dwParam ==EP_MES_E_8
                                        || pEvent->dwParam ==EP_MES_E_10
                                        || pEvent->dwParam ==EP_MES_E_12)
                                        
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_END_POPUP, EQS_NA);
        }
        break;

    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        SKY_SET_ORIGINATE(TRUE);    // ui.originate = TRUE; 
        g_SendState = PM_SUCC_ORIGINATED;     
        EQS_START_CHILD_EVENT(sSKA_PMOriginated,SE_APP_START,nLen,EQS_NA);  

        break;
    case SE_SOCK_RELEASED:
        //SKY_AcceptKey(TRUE);
        SKY_SET_ORIGINATE(FALSE);
        
        EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA); 
        
        break;

    case SE_CALL_ENDED:
        g_SendPM=PM_CONNECT_FAIL;
        SKY_DisconnectHTTPServer();
        
       break;

    case SE_IO_MESGBOX_YES:
        if (pEvent->dwParam == EP_MES_E_1)
        {
            SKY_SET_NO_OFF_BACKLIGHT(TRUE);
            SKY_BacklightOn(AB_ALWAYS);
            
            //유용메모리 확인
            AvailMem = SKY_GetFreeSpaceInDataFile();
            
            if(sParentIsCamera)
               
                sMaxSize=PM_MAX_BUF_SIZE1+NEED_MIN_SPACE;
            else
                sMaxSize=PM_MAX_BUF_SIZE1;

            if( sMaxSize > AvailMem ) 
            {
                hEPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_STORAGE_SCARCITY), //저장공간이 부족합니다.
				    PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hEPControl,EP_MES_E_8);
   
                break;
            }
            
         

            hEPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_MSG_STORAGE), //사진은 나만의 사진에, 메세지는 포토메일에 저장됩니다. 
                         PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hEPControl,EP_MES_E_2);
            EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        
        }
        else if(pEvent->dwParam == EP_MES_E_15)
        {
            EQS_END_EVENT(SE_APP_RESUME, PM_END_POPUP, PM_CLEAR1_POPUP);
        } 
        break;

	case SE_IO_MESGBOX_NO:
        if (pEvent->dwParam == EP_MES_E_1)
        {
            AvailMem = SKY_GetFreeSpaceInDataFile();
            nFiles = SKY_GetFileNumberInDataFile(SDF_VM);
            if(sParentIsCamera)
               
                sMaxSize=PM_MAX_BUF_SIZE1+GET_IMAGE_BYTE_SIZE(BitmapCameraInfo.Header);
            else
                sMaxSize=PM_MAX_BUF_SIZE1;


            if( ( sMaxSize > AvailMem ) || (nFiles == 0xFFFF) || (nFiles >= SKY_MAX_NUM_FILES))
            {
                hEPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_STORAGE_SCARCITY), //저장공간이 부족합니다.
				    PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hEPControl,EP_MES_E_10);

                break;
            }
                                          

            hEPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_MSG_STORAGE), //사진은 나만의 사진에, 메세지는 포토메일에 저장됩니다. 
                           PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hEPControl,EP_MES_E_4);
            EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
             
        }
        else if(pEvent->dwParam == EP_MES_E_15)
        {}  
 		break;

    case SE_IO_FOCUS_CHG:
        
        if(EQS_GetIOCOrderly(pEvent->wParam)==0)
            EQS_SetWindowCML(CL_MENU_NONE_SEND);
        else
            EQS_SetWindowCML(CL_MENU_INPUT_SEND);
        break;
    
    case SE_IO_POPUP_SELECT:
			
		switch(pEvent->dwParam)
        {   
            case TKN_PM_SAVE_PM:        //포토메일저장
                 for(m=EP_NUM_EDIT_1;m<EP_EDIT_MAX;m++)
                 {	
	  	  	         sTempValue[m-EP_NUM_EDIT_1][m]=0;
#ifdef email_version
                     EQC_GetTextEditData(EQS_GetNthIOC(m), (BYTE*)sTempValue[m-EP_NUM_EDIT_1]);
#else
		 	         EQC_GetNumberEditValue(EQS_GetNthIOC(m), (BYTE*)sTempValue[m-EP_NUM_EDIT_1]);
#endif //email_version
                 }
                 if(sTempValue[0][0]==0 && sTempValue[1][0]==0 && sTempValue[2][0]==0 &&
                                                 sTempValue[3][0]==0 && sTempValue[4][0]==0)
                 {
#ifdef email_version
                     hEPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_INPUT_RECEIVER), //받는 사람을 입력하세요.
                                         PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                     EQC_SET_USERDATA(hEPControl,EP_MES_E_6);
#else
                     hEPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_INPUT_PHONE_NUM), //전화번호를 입력하세요.
                                         PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                     EQC_SET_USERDATA(hEPControl,EP_MES_E_6);
#endif //email_version
                     break;
                     
                }
                
                AvailMem = SKY_GetFreeSpaceInDataFile();
                nFiles = SKY_GetFileNumberInDataFile(SDF_VM);
                if(sParentIsCamera)
                   
                    sMaxSize=PM_MAX_BUF_SIZE1+GET_IMAGE_BYTE_SIZE(BitmapCameraInfo.Header);
                else
                    sMaxSize=PM_MAX_BUF_SIZE1;

                if( ( sMaxSize > AvailMem ) || (nFiles == 0xFFFF) || (nFiles >= SKY_MAX_NUM_FILES))
                {
                    hEPControl=EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_STORAGE_SCARCITY), //저장공간이 부족합니다.
				        PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    EQC_SET_USERDATA(hEPControl,EP_MES_E_10);
                    EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);

                    break;
                }
                
                               

                hEPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_HELP_MSG_STORAGE), //사진은 나만의 사진에, 메세지는 포토메일에 저장됩니다. 
                               1, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hEPControl,EP_MES_E_4);
                EQC_SET_STYLE(hEPControl, EQC_GET_STYLE(hEPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                break;

            case TKN_PM_PBOOK_SEARCH:  //전화부찾기
/*                if(pEvent->wParam == BOOKCODE_IN_PHOTOMAIL)
                {
                    EQS_START_CHILD(SKA_SearchBookNumber);
                    break;
                }
*/
                if(!SKY_IS_PHONE_BOOK_LOCK())
				{
                    //EQS_START_CHILD(SKA_SearchBookNumber);
					EQS_START_CHILD_EVENT( SKA_SpeedSearch, SE_APP_START, EQS_NA, BFT_SEARCH_BOOK_E);
				}
                else
                {
                    EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, \
                        BOOKCODE_IN_PHOTOMAIL, TKN_PM_PBOOK_SEARCH);
                } 
                break;
                
            case TKN_PM_CALLHISTORY_CALLTO:		//발신이력찾기
#ifdef FEATURE_CALL_STACK_SEPERATED
                SetReceivedCallStack(FALSE);
#endif//FEATURE_CALL_STACK_SEPERATED 
				EQS_START_CHILD(SKA_MainStackCode);
                break;

            case TKN_PM_CALLHISTORY_CALLED:		//착신이력찾기
#ifdef FEATURE_CALL_STACK_SEPERATED
                SetReceivedCallStack(TRUE);
#endif//FEATURE_CALL_STACK_SEPERATED
				EQS_START_CHILD(SKA_MainStackCode);
                break;
#ifdef email_version
            case TKN_PM_PB_EMAIL_SEARCH:        //상대주소 찾기
                if(SKY_IS_PHONE_BOOK_LOCK())
                    EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, BOOKCODE_IN_VM, EQS_NA);
                else
                    EQS_START_CHILD(SKA_ViewEmailList);
                break;
#endif //email_version

            case TKN_PM_INPUT_PHOTO_NAME:     //사진이름지정
                EQS_START_CHILD(sSKA_PhotoNameSave);
                break;

            case TKN_PM_DECO_PHOTO:         //사진꾸미기
#ifdef deco_photo
                EQS_START_CHILD_EVENT(sSKA_DecoPhoto,SE_APP_START,EQS_NA,(DWORD)decoNum);
#endif //deco_photo
                break;

        }
		break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
/*			
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_EditNewPM()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_EditNewPM(SKY_EVENT_T *pEvent)
{ 
    BYTE * paEditNewPM[2];
    STATIC HCONTROL hHnd;
    HCONTROL hENPControl;
    HFILE	hFile;
    BYTE sFNameGarbage[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
    DWORD dwPhoto;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

		//EQS_GetDC(TRUE);	
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY,
                      CL_NONE,
                      SKY_GET_TOKEN(TKN_PM_EDIT_NEW_MSG),	//새포토메일작성
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        paEditNewPM[0]=(BYTE*)SKY_GET_TOKEN(TKN_PM_NEW_PHOTO);		//새 사진으로
		paEditNewPM[1]=(BYTE*)SKY_GET_TOKEN(TKN_PM_MY_PHOTO);		//나만의 사진으로

        hHnd = EQC_ScrollList(5,10,18, EQC_GetScrollDisplayLine(),paEditNewPM,2,2,0); 
		EQC_SET_STYLE( hHnd, EQC_GET_STYLE(hHnd) |
				                     CS_SCROLL_LIST_ENUMBERATED |
					                 CS_SCROLL_LIST_MAXIMIZE );


		EQS_SetIOCActive( hHnd );

        break;

    case SE_APP_END :   /* The very last event */ 
		//EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        if(EQS_GET_CHILD() == SKA_EditPM && pEvent->wParam != PM_END_POPUP && pEvent->wParam != PM_TRAN_POPUP )
        {	
            EQS_START_CHILD(SKA_PhotoListNView);
        }
        else if(EQS_GET_CHILD() == SKA_PhotoListNView || EQS_GET_CHILD() == SKA_PhotoViewer)
        {  
            if(pEvent->wParam == 2 )  //#define CLEAR_POPUP  2 //ska_cam.c
            {
                EQS_SetIOCActive( hHnd );
                              
                break;
            }
            else if(pEvent->wParam == PM_END_POPUP)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            else
            {
                ConvertCameraImageFileName(sFNameGarbage, pEvent->dwParam); 
                if(SKY_OpenDataFile(SDF_PHOTO, sFNameGarbage, &hFile, SFT_READ) == SDR_OK)
                {
                    SKY_CloseDataFile(hFile);
                    dwPhoto= pEvent->dwParam;
                    EQS_START_CHILD_EVENT(SKA_EditPM,SE_APP_START,EQS_NA,dwPhoto);
                }
                else
                {
                    hENPControl=EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_ASK_TAKING_PICTURE),	//새 사진을 찍으시겠습니까? 
				                PM_MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    EQC_SET_USERDATA(hENPControl,ENP_MES_E_1);
                    
                }
            }
        }
        else if(pEvent->wParam == PM_CLEAR_POPUP)
        {   
            EQS_SetIOCActive( hHnd );
            break;
            
        }
        else if(pEvent->wParam == PM_TRAN_POPUP)
        {   
            EQS_END_EVENT(SE_APP_RESUME, PM_TRAN_POPUP, EQS_NA); 
        }
        else if(pEvent->dwParam == PM_CLEAR1_POPUP)
        {
            EQS_END_EVENT(SE_APP_RESUME, EQS_NA ,PM_CLEAR1_POPUP); 
        }
        else
        {
            EQS_END();
        }
       

        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        
        EQS_END_EVENT(SE_APP_RESUME, EQS_NA ,PM_CLEAR1_POPUP);  
       
        break;

    case SE_IO_MESGBOX_YES:
        
        if(SKY_GetFileNumberInDataFile(SDF_PHOTO) < MAX_PHOTO_COUNT && \
           SKY_GetFreeSpaceInDataFile() >= NEED_MIN_SPACE)
        {
            EQS_START_CHILD(SKA_MainCamera);	//새 사진으로
        }
        else
        {
            hENPControl=EQC_MessageBox(SKY_GET_TOKEN(TKN_PM_TELL_MAXIMUM_PHOTO), PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hENPControl,ENP_MES_E_2);
            EQC_SET_STYLE(hENPControl, EQC_GET_STYLE(hENPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        break;

	case SE_IO_MESGBOX_NO:
    case SE_IO_MESGBOX_CANCEL:
    case SE_IO_MESGBOX_CLEAR:
    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive( hHnd );
        break;
    
    case SE_IO_SCROLL_LIST_SELECT:

        switch(pEvent->dwParam  )
        {
        case 0 :
            if(SKY_GetFileNumberInDataFile(SDF_PHOTO) < MAX_PHOTO_COUNT && \
             SKY_GetFreeSpaceInDataFile() >= NEED_MIN_SPACE)
            {
                EQS_START_CHILD(SKA_MainCamera);	//새 사진으로
            }
            else
            {
                hENPControl=EQC_MessageBox(SKY_GET_TOKEN(TKN_PM_TELL_MAXIMUM_PHOTO), PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hENPControl,ENP_MES_E_2);
                EQC_SET_STYLE(hENPControl, EQC_GET_STYLE(hENPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
            }
            break;
        case 1 :
            EQS_START_CHILD(SKA_PhotoListNView);				//나만의 사진으로
            break;
		}
        break;
    
     default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
/*			
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainPMList()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_MainPMList(SKY_EVENT_T *pEvent)
{   
    
    STATIC  UINT16 nDelIndex;
    HCONTROL hMPControl;
        
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

		//EQS_GetDC(TRUE);	
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY,
                      CL_MENU_SELECT_NEWMAIL,
                      SKY_GET_TOKEN(TKN_PM_PHOTO_MAIL),    //포토메일
                      WA_SHOW_CML | WA_SHOW_HEADER);
        g_MenuSize=0;
        SKY_PMListUP();
                   
        break;

    case SE_APP_END :   /* The very last event */ 
        SKY_FreePMScrollList();
		//EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */

        SKY_SET_NO_OFF_BACKLIGHT(FALSE);    
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

        if(pEvent->wParam == PM_TRAN_POPUP)
        {
            switch(g_SendState)
            {
                case PM_NOSERVICE:
                    hMPControl=EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NO_SERVICE), 
			                    PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    EQC_SET_USERDATA(hMPControl,MP_MES_E_5);
                    EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                      
                    break;
                case PM_MAINTREQ:
                    hMPControl=EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
					             PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
                    EQC_SET_USERDATA(hMPControl,MP_MES_E_5);
                    EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                    
                    break;
                case PM_FAIL_ORIGINATED:
                case PM_SUCC_ORIGINATED:
                    switch(g_SendPM)
                    {
                        case PM_SUCCESS:
                            hMPControl=EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_SUCC_TRANS_PM), 
                                            PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                            EQC_SET_USERDATA(hMPControl,MP_MES_E_5);
                            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                            
                            break;
                        case PM_FAIL:
                            hMPControl=EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_FAIL_TRANS_PM), 
									                PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                            EQC_SET_USERDATA(hMPControl,MP_MES_E_5);
                            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                            
                            break;
                        case PM_CONNECT_FAIL:
                            hMPControl=EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PM_CONNECT_FAIL), 
									                PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                            EQC_SET_USERDATA(hMPControl,MP_MES_E_5);
                            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                            
                            break;
                        case PM_SYSTEM_TROUBLE:
                            hMPControl=EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PM_SYSTEM_TROUBLE), 
									                PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                            EQC_SET_USERDATA(hMPControl,MP_MES_E_5);
                            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                            
                            break;
                        case PM_SERVER_TROUBLE:
                            hMPControl=EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PM_SERVER_TROUBLE), 
									                PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                            EQC_SET_USERDATA(hMPControl,MP_MES_E_5);
                            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                            
                            break;
                        case PM_TRANS_CANCEL:
                            hMPControl=EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PM_TRANS_CANCEL), 
									                PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                            EQC_SET_USERDATA(hMPControl,MP_MES_E_5);
                            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
                            
                            break;


                    }
                    break;
             }
             break;
            
        }

        if(pEvent->dwParam !=PM_CLEAR1_POPUP)
            SKY_PMListUP();
        else
            EQS_SetIOCActive( g_hHnd );
            
        break; 

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam == 0)
            EQS_END();
        break;

    case SE_SOFTKEY_ADD:		//작성
        if(g_unNumOfPM>=30)
        {
            hMPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_TELL_MAXIMUM_PM), //최대 30개 저장가능
                           PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMPControl,MP_MES_E_8);
            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
            break;
        }
		EQS_START_CHILD(SKA_EditNewPM);		//새메일작성
		break;
    
    
    case SE_SOFTKEY_MENU:     //삭제

        EQC_PopUpMenu((EQS_TOKEN_EX*)paMainPMMenu, 2, 0);
        
        break;

    case SE_IO_POPUP_SELECT:
                                                 	
        switch(pEvent->dwParam)
        {
            case 0:
                if(g_unNumOfPM == 0)
		        {
			        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_NOT_EXIST_DEL_ITEM),		//삭제할 항목이 없습니다. 
                        PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_CONFIRM);
		        }
		        else
                {
			        
                    nDelIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
                    hMPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_QUS_DELETE_ITEM),		//해당항목을 삭제할까요? 
                                                PM_MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    EQC_SET_USERDATA(hMPControl,MP_MES_E_1);
                }

                break;
            case 1:
                     hMPControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_QUS_DELETE_ALL_PM),		//전부 삭제할까요? 
                                                PM_MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                     EQC_SET_USERDATA(hMPControl,MP_MES_E_4);
                break;
        }
        break;

    case SE_IO_MESGBOX_YES:
        if (pEvent->dwParam == MP_MES_E_1 )
        {
            hMPControl= EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_DELETING_PM),	//삭제중입니다.
                               1, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMPControl,MP_MES_E_6);
            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);
        }
        else if(pEvent->dwParam == MP_MES_E_4)
        {
            hMPControl= EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_DELETING_PM),	//삭제중입니다.
                               1, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMPControl,MP_MES_E_7);
            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);

        }
        
        break;

    case SE_IO_MESGBOX_END:
        if (pEvent->dwParam == MP_MES_E_6 )
        {
            SKY_DeletePM(nDelIndex);
            hMPControl= EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_CONFIRM_DEL_PM),	//포토메일이 삭제되었습니다.  
                               PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMPControl,MP_MES_E_2);
            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);

        }
        else if(pEvent->dwParam == MP_MES_E_7)
        {
            SKY_DeleteAllPM();
            hMPControl= EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_PM_CONFIRM_DEL_PM),	//포토메일이 삭제되었습니다. 
                               PM_MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMPControl,MP_MES_E_2);
            EQC_SET_STYLE(hMPControl, EQC_GET_STYLE(hMPControl) |CS_MESSAGE_BOX_IGNORE_KEYS);

        }
        else if(pEvent->dwParam == MP_MES_E_8)
            EQS_SetIOCActive( g_hHnd );
        else
            SKY_PMListUP();
			if(g_unNumOfPM>0) 
			{
				if(nDelIndex > EQC_GetScrollMaxDataLine(g_hHnd) -1)
					nDelIndex = EQC_GetScrollMaxDataLine(g_hHnd) -1;
			
				EQC_SetScrollLogicalIndex(g_hHnd , nDelIndex);
				
			}	
			
		break;
    
    
	case SE_IO_MESGBOX_NO:
        
		break;

    case SE_IO_SCROLL_LIST_SELECT:
         
         EQS_START_CHILD_EVENT(sSKA_ViewPM,SE_APP_START,EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0)),EQS_NA);        
        
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;


    }
}
//#endif /*FEATURE_SKTT_PM*/

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////                                 ////////////////////////////////////////
/////////////////////////     전송(HTTP)                  ////////////////////////////////////////
/////////////////////////                                 ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

UINT16 SKY_SendPM(BYTE* pData, UINT16 nLen)
{       
    if (nLen>ONEPACK_SIZE)
    {
        SvrLinkSend(pData, ONEPACK_SIZE);
        return ONEPACK_SIZE;
    }
    else
    {
        SvrLinkSend(pData, nLen);
        return nLen;
    }
}



BOOL SKY_ReceiveHTTPReply(void){
   
    BYTE    aRecvBuf[DSM_DS_LARGE_ITEM_SIZ];
    WORD    nRecvLen;

    MSG_HIGH("HTTP reply received",0,0,0);
    memset(aRecvBuf, 0, sizeof(aRecvBuf));

    nRecvLen = SvrLinkRcv(aRecvBuf);

    MSG_HIGH("[%c][%c][%c]", aRecvBuf[9], aRecvBuf[10], aRecvBuf[11]);
    if (nRecvLen>=9)
    {
/*  
    ex) HTTP/1.0 200 OK
    2xx : Success - 성공적으로 수신되고 해독되고 처리된 경우. 
·	3xx : Redirection - 완전한 처리를 위해 추가적인 동작이 필요로 하는 경우. 
·	4xx : Client Error - 요구 메시지에 글자상의 문제가 있는 경우거나 메시지를 처리할 수 없을 때. 
·	5xx : Server Error - 서버가 요구 메시지를 처리하는 가운데 문제가 발생한 경우.  
*/
        if(aRecvBuf[9]=='2')
        {
            MSG_HIGH("Success in HTTP transmittion",0,0,0);
            return TRUE;    //success   
        }
    }
    MSG_HIGH("Fail in HTTP transmittion",0,0,0);
    return FALSE;
}

void SKY_DisconnectHTTPServer(void)
{   
    
    if (SKY_IS_SOCK_CONNECT())
    {
        SKY_SET_SOCK_CONNECT(FALSE);
    }
    if(SKY_IS_SOCK_MODE())
    {
        SvrLinkReleasebyUi();
    }
    SKY_SET_SOCK_MODE(FALSE);
    
}

void SKY_ConnectHTTPServer(void)
{
	SKY_SET_PM(TRUE);
    SKY_SET_SOCK_MODE(TRUE);

	if (SKY_IS_SOCK_CONNECT())
    {
        SvrLinkReconnect((char*)SKY_GET_HOST(), HTTP_PORT);
        
    }
    else
    {
        SvrLinkConnect((char*)SKY_GET_HOST(), HTTP_PORT);
    
    }
	
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainXxx()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL  void sSKA_PMOriginated(SKY_EVENT_T *pInEvent)
{
    STATIC UINT16 nLen;
    STATIC BOOL ToRoot=FALSE;
    STATIC BOOL fClearKey=FALSE;
    HCONTROL hControl;

    switch(pInEvent->EventID)
    {
        case SE_APP_START : /* The very first event */
            EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        NULL, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);
            
            nLen=pInEvent->wParam;
            
            /*hControl = EQC_Animation(PM_ANI_X, PM_ANI_Y, &g_AniData59x36x256C, NULL);
            EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);
            EQC_StartAnimation(hControl, TRUE);*/
            EQC_Image(PM_ANI_X, PM_ANI_Y, (BITMAP*)&g_BmpConnectDataService);
            //EQC_Image(PM_PANNEL_X,PM_PANNEL_Y, (BITMAP*)&g_BmpGeneralPannel);
        
            //EQC_Image(PM_FRAME_X, PM_FRAME_Y, (BITMAP*)&g_BmpEframe115x15x256c);

            hControl = EQC_StaticText(PM_STATUS_X, PM_STATUS_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE);
            EQC_SET_STYLE(hControl,CS_STATIC_TEXT_ORING);
            SKY_AcceptKey(TRUE);
            break;

        case SE_APP_END : /* The very last event */
            SKY_AcceptKey(TRUE);   // connect 시점에서 False를 해놨을므로 꼭 True로 돌려줘야 함.
            ToRoot=FALSE;
            SKY_SET_NO_OFF_BACKLIGHT(FALSE);    
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            break;    

        case SE_SOCK_RELEASED:
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_ORIGINATE(FALSE);
            if(!ToRoot)
                EQS_END();
            else
                EQS_END_TO(EQS_POP_TO_ROOT);
            break;

        case SE_APP_DRAW :    		
            break;

        case SE_KEY_0 ://HS_0_K:
        case SE_KEY_1 ://HS_1_K:
        case SE_KEY_2 ://HS_2_K:
        case SE_KEY_3 ://HS_3_K:
        case SE_KEY_4 ://HS_4_K:
        case SE_KEY_5 ://HS_5_K:
        case SE_KEY_6 ://HS_6_K:
        case SE_KEY_7 ://HS_7_K:
        case SE_KEY_8 ://HS_8_K:
        case SE_KEY_9 ://HS_9_K:
        case SE_KEY_STAR ://HS_STAR_K:
        case SE_KEY_POUND ://HS_POUND_K:
        case SE_KEY_UP ://HS_UP_K:
        case SE_KEY_DOWN ://HS_DOWN_K:
        // ignore all these events
            break;

        case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
            break;

        case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
            SKY_DisconnectHTTPServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            ToRoot=TRUE;
            break;

        case SE_SOFTKEY_LEFT ://HS_PF1_K:
        case SE_SOFTKEY_RIGHT ://HS_PF2_K:
        case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
            // ignore all these events
            break;

        case SE_KEY_CLEAR:
            fClearKey =TRUE;
            g_SendPM=PM_TRANS_CANCEL;
            SKY_DisconnectHTTPServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            break;

        case SE_KEY_END ://HS_END_K:
            SKY_DisconnectHTTPServer();
            SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
            ToRoot=TRUE;
            break;

        case SE_KEY_SEND ://HS_SEND_K:
        case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
        case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
        case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
        case SE_CALL_EXT_DISPLAY ://UI_CALL_EXT_DISPLAY_E:
        case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        case SE_CALL_FLASHED :   //UI_CALL_FLASHED_E:
        case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
            break;

        case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
            
            break;

        case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
            l_PMStartTime = clk_uptime();

            // 통화연결음
            if(SKY_IS_CONNECT_WARNING_SIGNAL())
            {
                if(!SKY_IS_ETIQUETTE())
                {
                    SKY_PlayMusicEx( MUS_DENWA_1, 1, 1);
                }
            }
            EQS_START_SIBLING_EVENT(sSKA_PMHTTPInProg,SE_APP_START,nLen,EQS_NA, FALSE );
            
            break;
        
        case SE_CALL_ENDED:
            if(!fClearKey)
            {
                g_SendPM=PM_SYSTEM_TROUBLE;
            }
            SKY_SET_ORIGINATE(FALSE);  
            SKY_DisconnectHTTPServer();
            
            break;

        case SE_TCPCONNECT_FAIL:
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_ORIGINATE(FALSE);
            g_SendPM=PM_SERVER_TROUBLE;
            EQS_End();
            break;


        default:
            EQS_DEF_EVENT_PROC(pInEvent);
            break;
        }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_PMHTTPInProg()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_PMHTTPInProg(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL    hControl;
    STATIC HCONTROL    hAniControl;
    STATIC UINT16 nLen;
    STATIC BYTE* pSendData;
    STATIC BOOL ToRoot=FALSE;
    UINT16 nSentLen;
    

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    CL_NONE, 
                    NULL, // show annunciators
                    WA_SHOW_HEADER);

        nLen =pEvent->wParam;

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(AB_ALWAYS);

        //EQC_Image(PM_ANI_BACK_X, PM_ANI_BACK_Y, (BITMAP*)&g_BmpEbase41x64x256c);
        //EQC_Image(PM_FRAME_X, PM_FRAME_Y, (BITMAP*)&g_BmpEframe115x15x256c);
        hAniControl = EQC_Animation(PM_ANI_X, PM_ANI_Y, 
                    &g_AniSendEMail, NULL);
        //EQC_Image(PM_PANNEL_X,PM_PANNEL_Y, (BITMAP*)&g_BmpGeneralPannel);
        EQC_SET_STYLE(hAniControl, CS_ANIMATION_REPEAT);
        EQC_StartAnimation(hAniControl, TRUE);

        hControl = EQC_StaticText(PM_STATUS_X, PM_STATUS_Y, TKN_PM_CONNECTING);
        //EQC_SET_STYLE(hControl,CS_STATIC_TEXT_ORING);
        //EQS_StartTimer(EQS_TIMER_1,10000, FALSE);
       
        break;
        
    case SE_APP_END :   /* The very last event */ 
        ToRoot=FALSE;
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);    
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        break;
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;   

    case SE_SOCK_RELEASED:
        SKY_SET_SOCK_CONNECT(FALSE);
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_ORIGINATE(FALSE);
        if(!ToRoot)
            EQS_END();
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_KEY_CLEAR:
        g_SendPM=PM_TRANS_CANCEL;
        SKY_DisconnectHTTPServer();
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        break;

    case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
    case SE_SYS_OFFLINE:
        SKY_DisconnectHTTPServer();
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        break;

    case SE_KEY_END ://HS_END_K:
    case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
        ToRoot= TRUE;
        SKY_DisconnectHTTPServer();
        SKY_SET_ORIGINATE(FALSE);    // ui.originate = TRUE; 
        break;

    case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
    case SE_CALL_INCOMING:/*UI_CALL_INCOMING_E:*/
        break;

    case SE_RECV_DATA:
    
        switch(SKY_ReceiveHTTPReply()){
        case 0: //fail

            SKY_DisconnectHTTPServer();
            SKY_SET_ORIGINATE(FALSE);
                     
            break;

        case 1: //success

            g_SendPM = PM_SUCCESS;
            SKY_DisconnectHTTPServer();
            SKY_SET_ORIGINATE(FALSE);
                          
            break;
        }
        break;

  
    case SE_TCPCONNECT_SUCCESS:
        SKY_SET_SOCK_CONNECT(TRUE);
        EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_PM_SENDING));
        //EQS_DeleteControl(hAniControl);   // Animation
        //EQS_StartTimer(EQS_TIMER_2, PM_WAIT_TIME, FALSE);

        if ((nSentLen=SKY_SendPM(pTRANS_DATA, nLen))<nLen)
        {
            pSendData = pTRANS_DATA+nSentLen;
            nLen -= nSentLen;
            EQS_StartTimer(EQS_TIMER_1, ONEPACK_SEND_TIME, FALSE);
        }
        break;
    
    case SE_TM_TIMER1:
        if ((nSentLen=SKY_SendPM(pSendData, nLen))<nLen)
        {
            pSendData = pSendData+nSentLen;
            nLen -= nSentLen;
            EQS_StartTimer(EQS_TIMER_1, ONEPACK_SEND_TIME, FALSE);
        }
        else
        {
            EQS_StartTimer(EQS_TIMER_2, PM_WAIT_TIME, FALSE);
        }
        break;

    case SE_TM_TIMER2:
        MSG_HIGH("time_out while waiting for result",0,0,0);
        SKY_DisconnectHTTPServer();
        SKY_SET_ORIGINATE(FALSE);
        break;

    case SE_TCPCONNECT_FAIL:
        SKY_SET_SOCK_CONNECT(FALSE);
        SKY_SET_SOCK_MODE(FALSE);
        SKY_SET_ORIGINATE(FALSE);
        g_SendPM=PM_SERVER_TROUBLE;
        EQS_End();
        break;

    case SE_CALL_ENDED:
        UpdateOriginatedDataCallInfo(clk_uptime() - l_PMStartTime);   // 데이터발신 통화시간
        SKY_SET_ORIGINATE(FALSE);
        SKY_DisconnectHTTPServer();
        
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////