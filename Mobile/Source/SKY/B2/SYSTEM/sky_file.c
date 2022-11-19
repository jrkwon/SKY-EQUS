/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     sky_file.c
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
** 2000-11-29 jrkwon     Created.
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>      /* sprintf */
#include <string.h>     /* strrchr */
#include <stdlib.h>     /* atoi */

#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* ----------------------------------------------------------------------------
** decide on cleanup operations to be executed on the file in case 
** an irregular power-down or file corruption due to some other cases.
** -- FS_CO_DISCARD --> delete file and remove it from dirctory
*/
#define DISCARD_CLEANUP_OPTION              FS_CO_DISCARD
#define	MM_MAX_BELL_BLOCK_SIZE	64
#define	BELL_FILE_CAHCE_SIZE	(MM_MAX_BELL_BLOCK_SIZE*15)

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
/* file version table */
ROM SKY_FILE_VERSION_T   g_aFileVersion[] = 
{
    /* extension info */
    {  1, 0  },
    /* system */
    {  4, 0  },
    /* misc */
    {  1, 0  },
    /* system application */
    {  3, 0  },
    /* GVM */
    {  1, 0  },
    /* phone book */
    {  1, 0  }
};

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
struct tagDATA_FILE_CONFIG_T {
    BYTE* pszDataDir;
    UINT8 nMaxCount;
}; 
typedef struct tagDATA_FILE_CONFIG_T    DATA_FILE_CONFIG;

#define sMAX_MELODY_COUNT       50
#define sMAX_IMAGE_COUNT        50
#ifdef FEATURE_CHARACBELL
#define sMAX_CHARACBELL_COUNT   5
#endif // FEATURE_CHARACBELL
#define sMAX_GVM_COUNT          16
//#define sMAX_PHOTO_COUNT        50
#define sMAX_MAIL_COUNT         43
#ifdef FEATURE_SKTT_PM
#define sMAX_PM_COUNT           40
#endif
#ifdef	FEATURE_WAVELET
#define sMAX_WAVELET_COUNT      50
#define sTEMP_WAVELET_COUNT		5
#endif //FEATURE_WAVELET

LOCAL DATA_FILE_CONFIG l_DataFileConfig[] = {
    { SKY_MELODY_DIR,   sMAX_MELODY_COUNT }, /* SDF_MELODY 멜로디 */
    { SKY_IMAGE_DIR,    sMAX_IMAGE_COUNT  }, /* SDF_IMAGE  이미지/애니메이션 */
#ifdef FEATURE_CHARACBELL
    { SKY_CHA_MELODY_DIR, sMAX_CHARACBELL_COUNT},
    { SKY_CHA_IMAGE_DIR, sMAX_CHARACBELL_COUNT},
#endif // FEATURE_CHARACBELL
    { SKY_GVM_DIR,      sMAX_GVM_COUNT    }, /* SDF_GVM    GVM 데이터 */
    { SKY_PHOTO_DIR,    MAX_PHOTO_COUNT  }, /* SDF_PHOTO  사진 이미지 */
    { SKY_MAIL_DIR,     sMAX_MAIL_COUNT   }, /* SDF_MAIL   MAIL데이터 */
#ifdef FEATURE_SKTT_PM
    { SKY_VM_DIR,       sMAX_PM_COUNT     }, /* SDF_VM     Photo Mail데이터 */
#endif
#ifdef	FEATURE_WAVELET	//	
    { SKY_WAVELET_DIR, sMAX_WAVELET_COUNT }, /* SDF_WAVELET */
    { SKY_TEMPWAV_DIR, sTEMP_WAVELET_COUNT }, /* SDF_WAVELET */
#endif	//	#ifdef	FEATURE_WAVELET
    { NULL, 0 }             /* SDF_MAX */
};

/* 
** the order must be same in SKY_DATA_FILE_E. use SKY_DATA_E as index of 
** this array
*/
//LOCAL BYTE *l_pszDataDir[] = {
//    SKY_MELODY_DIR,
//    SKY_IMAGE_DIR,
//    SKY_GVM_DIR,
//    SKY_PHOTO_DIR,
//    SKY_MAIL_DIR,           /* MAIL데이터 */
//#ifdef FEATURE_SKTT_PM
//	SKY_VM_DIR,
//#endif
//#ifdef	FEATURE_WAVELET_OFFLINE	//	khekim 01/01/16
//	SKY_WAVELET_DIR,
//#endif	//	#ifdef	FEATURE_WAVELET_OFFLINE
//    NULL
//};

LOCAL SKY_MMFILE_HANDLE_T   mm_BellFile = {"\0", 0,0,0, NULL};
LOCAL BYTE					mm_BellBlock[MM_MAX_BELL_BLOCK_SIZE];
LOCAL BYTE					*mm_BellBlockCache;
LOCAL DWORD 				mm_CacheSPos, mm_CacheEPos, mm_SeekPos;

LOCAL UINT16				nCache_Size;
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void SKY_API sGetFileExtNameFromPathName(const BYTE* pszPathName, BYTE* pszFileName);
LOCAL void SKY_API sGetFileNameFromPathName(const BYTE* pszPathName, BYTE* pszFileName);

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
FILENAME_VALID_E SKY_API SKY_IsValidFileName(const BYTE* pszFileName)
{
    BYTE bDirChar = SKY_DIR_CHAR[0];

    /* null file name */
    if(pszFileName == NULL || pszFileName[0] == NULL)
        return FV_NULL;

    if(STRLEN(pszFileName) >= SKY_FILE_EXT_NAME_MAX_LENGTH)
        return FV_2LONG;

    /* directory char */
    if(strchr((char*)pszFileName, bDirChar))
        return FV_DIRCHAR;

    return FV_OK;
}

/* get the file name and ext name from the complete path name */
LOCAL void SKY_API sGetFileExtNameFromPathName(const BYTE* pszPathName, BYTE* pszFileName)
{
    BYTE *pszPosDirChar;
    BYTE bDirChar = SKY_DIR_CHAR[0];

    /* search backward SKY_DIR_CHAR */
    pszPosDirChar = (BYTE*)strrchr((const char*)pszPathName, (int)bDirChar);
    /* if the char found */
    if(pszPosDirChar)
    {
        STRCPY(pszFileName, pszPosDirChar + 1);
    }
    else
    {
        STRCPY(pszFileName, pszPathName);
    }
}

/* get file name(but extension name) form the complete path name */
LOCAL void SKY_API sGetFileNameFromPathName(const BYTE* pszPathName, BYTE* pszFileName)
{
    sGetFileExtNameFromPathName(pszPathName, pszFileName);
    if(pszFileName[0])
    {
        BYTE *pszPosExtChar;

        pszPosExtChar = (BYTE*)strrchr((char*)pszFileName, (int)'.');
        if(pszPosExtChar)
            *pszPosExtChar = '\0';
    }
}

void SKY_API SKY_InitDataDir(void)
{
    /* is melody directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_MELODY_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_MELODY_DIR);
    }
    /* is image directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_IMAGE_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_IMAGE_DIR);
    }
#ifdef FEATURE_CHARACBELL
    /* is characbell directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_CHARACBELL_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_CHARACBELL_DIR);
    }
    /* is characbell melody directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_CHA_MELODY_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_CHA_MELODY_DIR);
    }
    /* is characbell image directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_CHA_IMAGE_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_CHA_IMAGE_DIR);
    }
#endif // FEATURE_CHARACBELL
    /* is GVM directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_GVM_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_GVM_DIR);
    }
    /* is PHOTO directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_PHOTO_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_PHOTO_DIR);    
    }
//FEATURE_WAPMAIL
    /* is MAIL directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_MAIL_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_MAIL_DIR);    
    }
#ifdef FEATURE_SKTT_PM
	/* is VIDEO MAIL directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_VM_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_VM_DIR);    
    }
#endif

#ifdef	FEATURE_WAVELET	//	
    /* is WAVELET directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_WAVELET_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_WAVELET_DIR);    
    }
    /* is WAVELET directory exist ? */
    if(!SKY_IsFileOrDirExist(SKY_TEMPWAV_DIR))
    {
        /* make the directory */
        SKY_MakeDir(SKY_TEMPWAV_DIR);    
    }

#endif	//	#ifdef	FEATURE_WAVELET

}

void SKY_API SKY_RenameDataFile(SKY_DATA_FILE_E SkyDataFile, BYTE *pszOldName, BYTE *pszNewName)
{
    BYTE szPathOld[SKY_PATH_NAME_MAX_LENGTH];
    BYTE szPathNew[SKY_PATH_NAME_MAX_LENGTH];

    sprintf((char*)szPathOld, "%s%s%s", 
            l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, SKY_DIR_CHAR, pszOldName);

    sprintf((char*)szPathNew, "%s%s%s", 
            l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, SKY_DIR_CHAR, pszNewName);

    SKY_RenameFile(szPathOld, szPathNew);
}


SKY_DATA_FILE_RESULT_E SKY_API SKY_DeleteDataFile(SKY_DATA_FILE_E SkyDataFile, BYTE *pszFileName)
{
    BYTE szPath[SKY_PATH_NAME_MAX_LENGTH];

    sprintf((char*)szPath, "%s%s%s", 
            l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, SKY_DIR_CHAR, pszFileName);

    if(SKY_IsFileOrDirExist(szPath) == FALSE)
        return SDR_FILE_EXIST;

    SKY_DeleteFile(szPath);
    return SDR_OK;
}

void SKY_API SKY_DeleteDataFiles(SKY_DATA_FILE_E SkyDataFile)
{
    SKY_DeleteAllFile(l_DataFileConfig[(INT8)SkyDataFile].pszDataDir);
}

void  SKY_API SKY_DeleteAllDataFile(void)
{
    SKY_DeleteDataFiles(SDF_MELODY);
    SKY_DeleteDataFiles(SDF_IMAGE);
    SKY_DeleteDataFiles(SDF_GVM);
    SKY_DeleteDataFiles(SDF_PHOTO);
// FEATURE_WAPMAIL
    SKY_DeleteDataFiles(SDF_MAIL);
#ifdef FEATURE_SKTT_PM
	SKY_DeleteDataFiles(SDF_VM);
#endif
#ifdef	FEATURE_WAVELET	//	
    SKY_DeleteDataFiles(SDF_WAVELET);
    SKY_DeleteDataFiles(SDF_TEMPWAV);
#endif	//	#ifdef	FEATURE_WAVELET
}

#ifdef FEATURE_SKVM
void SKY_RemoveSKVMAllData(CONST BYTE* pszDirName)
{

	// sky 내에서는 50개가 화일 크기 한계이다. 
	BYTE szDirNum, tempNum;
	BYTE szDirNameList[SKY_PATH_NAME_MAX_LENGTH * SKY_MAX_NUM_FILES];
	BYTE aszDirNameList[SKY_MAX_NUM_FILES][SKY_PATH_NAME_MAX_LENGTH];
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];
	BYTE *pszPathName;
	BYTE aszRSPathName[SKY_PATH_NAME_MAX_LENGTH];
	BYTE *temp;
	int i;

	/* 
	1. xce 밑의 모든 디렉토리의 갯수와 이름을 읽어 온다. 
	2. 갯수 만큼의 loop을 돌면서 각 directory내의 화일을 지운다. 
	   화일을 다지우면 해당 디렉토리를 지운다. 
	3. Root디렉토리 지운다.  
	*/
   if(!SKY_IsFileOrDirExist(pszDirName))
       return;

	// 1 
	szDirNum = SKY_GetDirNameList(pszDirName, szDirNameList);
	pszPathName = szDirNameList;
    for(i = 0; i < szDirNum; i++)
    {

		STRCPY(aszDirNameList[i], pszDirName);
		aszDirNameList[i][STRLEN(pszDirName)]='/';
        STRCPY(&aszDirNameList[i][STRLEN(pszDirName)+1],pszPathName);
        pszPathName += (STRLEN(pszPathName) + 1);
    }

	// 2
	for(i=0; i<szDirNum; i++){
		STRCPY(aszRSPathName,aszDirNameList[i]); 
		STRCAT(aszRSPathName,(BYTE*)"/rs");
		if(SKY_IsFileOrDirExist(aszRSPathName)){
			SKY_DeleteAllFile(aszRSPathName);
			SKY_RemoveDir(aszRSPathName);
		}
		SKY_DeleteAllFile(aszDirNameList[i]);
		SKY_RemoveDir(aszDirNameList[i]);
	}
	//3
	SKY_RemoveDir(SKY_SKVM_DIR);
}

#endif
/*
** SKY_GetFileNameList returns the char pointer of file name list like this
** "SKY/SYS/AAA.DAT\0SKY/SYS/BBB.DAT\0SKY/SYS/CCC.DAT\0\0"
** convert this data to char pointer array like this..
** "AAA.DAT", "BBB.DAT", "CCC.DAT"
*/
UINT8 SKY_API SKY_GetFileExtList(const BYTE *pszDirName, 
                                 //BYTE *pszFileNameList,
                                 BYTE *paszFileExtNameList[])
{
    BYTE szFileNameList[SKY_PATH_NAME_MAX_LENGTH * SKY_MAX_NUM_FILES];
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];
    BYTE *pszPathName;
    WORD wFileCount;
    WORD i;

    /* get the file list */
    wFileCount = SKY_GetFileNameList(pszDirName, szFileNameList);

    pszPathName = szFileNameList;
    for(i = 0; i < wFileCount; i++)
    {
        STRCPY(szPathName, pszPathName);
        sGetFileExtNameFromPathName(szPathName, paszFileExtNameList[i]);
        pszPathName += (STRLEN(pszPathName) + 1);
    }

    return (UINT8)wFileCount;
}

/*
** SKY_GetFileNameList returns the char pointer of file name list like this
** "SKY/SYS/AAA.DAT\0SKY/SYS/BBB.DAT\0SKY/SYS/CCC.DAT\0\0"
** convert this data to char pointer array like this..
** "AAA", "BBB", "CCC"
*/
UINT8 SKY_API SKY_GetFileList(const BYTE *pszDirName, 
                              //BYTE *pszFileNameList,
                              BYTE *paszFileNameList[])
{
    BYTE szFileNameList[SKY_PATH_NAME_MAX_LENGTH * SKY_MAX_NUM_FILES];
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];
    BYTE *pszPathName;
    WORD wFileCount;
    WORD i;

    /* get the file list */
    wFileCount = SKY_GetFileNameList(pszDirName, szFileNameList);

    pszPathName = szFileNameList;
    for(i = 0; i < wFileCount; i++)
    {
        STRCPY(szPathName, pszPathName);

        sGetFileNameFromPathName(szPathName, paszFileNameList[i]);
        pszPathName += (STRLEN(pszPathName) + 1);
    }
    return (UINT8)wFileCount;
}

/* ---------------------------------------------------------------------------
**  get the used space
*/
DWORD SKY_API SKY_GetUsedSpaceInDataFile(SKY_DATA_FILE_E SkyDataFile)
{
    return SKY_GetDirSize(l_DataFileConfig[(INT8)SkyDataFile].pszDataDir);
}

/* ---------------------------------------------------------------------------
**  get the free space in the specified size in SKY system (SKY_MAX_EFS_SIZE)
*/
DWORD SKY_API SKY_GetFreeSpaceInDataFile(void)
{
    DWORD dwSize;
    INT8 i;
#ifdef FEATURE_WAVELET
	DWORD sUsedTtempWaveletSpace = SKY_GetUsedSpaceInDataFile(SDF_TEMPWAV);
#endif //FEATURE_WAVELET

    dwSize = 0;

    /* jrkwon 2001.12.05 */
    for(i = 0; i < SDF_MAX; i++)
        dwSize += SKY_GetUsedSpaceInDataFile((SKY_DATA_FILE_E)i);

#ifdef FEATURE_WAVELET
	dwSize -= sUsedTtempWaveletSpace;
#endif //FEATURE_WAVELET
    
    /* jrkwon 2001.11.21 -> */
    /*
    ** If the size of sub directories is larger than SKY_MAX_EFS_SIZE
    ** Um.. something wrong..
    **/
    if(dwSize > SKY_MAX_EFS_SIZE) 
        return 0;
    /* <-- */
    else
        return MIN(SKY_MAX_EFS_SIZE - dwSize, SKY_GetFreeSpace());
}

/* ---------------------------------------------------------------------------
** get the number of files
*/
WORD SKY_API SKY_GetFileNumberInDataFile(SKY_DATA_FILE_E SkyDataFile)
{
    return SKY_GetNumFile(l_DataFileConfig[(INT8)SkyDataFile].pszDataDir);
}

WORD SKY_API SKY_GetFileNumberInAllDataFile(void)
{
    WORD nDataFile;

    nDataFile = SKY_GetNumFile(l_DataFileConfig[(INT8)SDF_MELODY].pszDataDir);
    nDataFile += SKY_GetNumFile(l_DataFileConfig[(INT8)SDF_PHOTO].pszDataDir);
    nDataFile += SKY_GetNumFile(l_DataFileConfig[(INT8)SDF_IMAGE].pszDataDir);
    nDataFile += SKY_GetNumFile(l_DataFileConfig[(INT8)SDF_GVM].pszDataDir);
#ifdef FEATURE_SKTT_PM
	nDataFile += SKY_GetNumFile(l_DataFileConfig[(INT8)SDF_VM].pszDataDir);
#endif
#ifdef	FEATURE_WAVELET	//	
    nDataFile +=( SKY_GetNumFile(l_DataFileConfig[(INT8)SDF_WAVELET].pszDataDir)
				+ SKY_GetNumFile(l_DataFileConfig[(INT8)SDF_TEMPWAV].pszDataDir));
#endif	//FEATURE_WAVELET
    return nDataFile;
}
/* ---------------------------------------------------------------------------
** Write a block as a file
*/
SKY_DATA_FILE_RESULT_E SKY_API SKY_WriteDataFile(SKY_DATA_FILE_E SkyDataFile,
                                                 CONST BYTE* pszFileName, 
                                                 DWORD dwBlockSize,
                                                 CONST void* pBlockData)
{
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];
    FS_ACCESS_MODE_E AccessMode;
    HFILE hFile;

    /* check free space and file count */
    if(dwBlockSize > SKY_GetFreeSpaceInDataFile())
        return SDR_OUT_OF_SPACE;

    /* make complete path */
    sprintf((char*)szPathName, "%s%s%s", l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, 
                                         SKY_DIR_CHAR,
                                         pszFileName);
    AccessMode = FS_AM_APPEND;
    /* is there this file ? */
    if(SKY_IsFileOrDirExist(szPathName) == FALSE)
    {
        /* check the maximum number of files */
        if(SKY_GetFileNumberInDataFile(SkyDataFile) > l_DataFileConfig[(INT8)SkyDataFile].nMaxCount)//SKY_MAX_NUM_FILES)
            return SDR_TOO_MANY_FILES;
        /* no file, then create it or just open it */
        AccessMode = FS_AM_CREATE;
    }
    hFile = SKY_OpenFileEx(szPathName, AccessMode, DISCARD_CLEANUP_OPTION);

    if(hFile == SKY_INVALID_FILE_HANDLE)
        return SDR_OPEN_ERROR;

    SKY_SET_OPEN_DATA_FILE_NAME(szPathName);

/* is the request size to write same with the return value */
    if(SKY_WriteFile(hFile, pBlockData, dwBlockSize) == dwBlockSize)
    {
        /* success */
        SKY_CloseFile(hFile);

        SKY_TRACE2(TL_HIGH, "%s - SUCCESS in writing (%ld)", 
                   szPathName, dwBlockSize);
    }
    else
    {
        /* error !! not enough space to save the block */
        SKY_CloseFile(hFile);
        /* erase this file */
        SKY_DeleteFile(szPathName);
        
        SKY_TRACE2(TL_HIGH, "%s - ERROR in writing (%ld). Close and Delete it", 
                            szPathName, dwBlockSize);
        return SDR_WRITE_ERROR;
    }

    SKY_CLEAR_OPEN_DATA_FILE_NAME();

    return SDR_OK;
}

/* ---------------------------------------------------------------------------
** read a block from a file 
*/
SKY_DATA_FILE_RESULT_E SKY_API SKY_ReadDataFile(SKY_DATA_FILE_E SkyDataFile,
                                                CONST BYTE* pszFileName,
                                                DWORD dwBlockSize,      
                                                void* pBlockDataToRead)
{
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];
    HFILE hFile;

    /* make complete path */
    sprintf((char*)szPathName, "%s%s%s", l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, 
                                         SKY_DIR_CHAR,
                                         pszFileName);
    /* -----------------------------------------
    ** is there this file ? no file, just return
    */
    if(SKY_IsFileOrDirExist(szPathName) == FALSE)
        return SDR_FILE_EXIST;

    hFile = SKY_OpenFileEx(szPathName, FS_AM_READONLY, 
                           DISCARD_CLEANUP_OPTION);

    if(hFile == SKY_INVALID_FILE_HANDLE)
        return SDR_OPEN_ERROR;

    /* is the request size to write same with the return value */
    if(SKY_ReadFile(hFile, pBlockDataToRead, dwBlockSize) == dwBlockSize)
    {
        /* success */
        SKY_CloseFile(hFile);

        SKY_TRACE2(TL_HIGH, "%s - SUCCESS in reading (%ld)", szPathName, dwBlockSize);
    }
    else
    {
        /* error !! not enough space to save the block */
        SKY_CloseFile(hFile);
        /* erase this file */
        SKY_DeleteFile(szPathName);

        SKY_TRACE2(TL_HIGH, "%s - ERROR in reading (%ld). Close and Delete it", 
                            szPathName, dwBlockSize);
        return SDR_READ_ERROR;
    }

    return SDR_OK;
}


/* ---------------------------------------------------------------------------
** create new data file
**
** phFile : file handle newly created
*/
SKY_DATA_FILE_RESULT_E SKY_API SKY_CreateDataFile(SKY_DATA_FILE_E SkyDataFile,
                                                  CONST BYTE* pszFileName,
                                                  HFILE *phFile) 
{
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];

    /* make complete path */
    sprintf((char*)szPathName, "%s%s%s", l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, 
                                         SKY_DIR_CHAR,
                                         pszFileName);
    /* is there this file ? */
    if(SKY_IsFileOrDirExist(szPathName))
        return SDR_FILE_EXIST;

    /* check the maximum number of files */
    if(SKY_GetFileNumberInDataFile(SkyDataFile) > l_DataFileConfig[(INT8)SkyDataFile].nMaxCount)//SKY_MAX_NUM_FILES)
        return SDR_TOO_MANY_FILES;

    *phFile = SKY_OpenFileEx(szPathName, FS_AM_CREATE, DISCARD_CLEANUP_OPTION);

    if(*phFile == SKY_INVALID_FILE_HANDLE)
        return SDR_OPEN_ERROR;

    SKY_TRACE1(TL_HIGH, "%s - Create file SUCCESS", szPathName);
        
    SKY_SET_OPEN_DATA_FILE_NAME(szPathName);

    return SDR_OK;
}


/* ---------------------------------------------------------------------------
** open data file
*/
SKY_DATA_FILE_RESULT_E SKY_API SKY_OpenDataFile(SKY_DATA_FILE_E SkyDataFile,
                                                CONST BYTE* pszFileName,
                                                HFILE *phFile,
                                                SKY_FILEOPEN_TYPE_E openType) 
{
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];

    /* make complete path */
    sprintf((char*)szPathName, "%s%s%s", l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, 
                                         SKY_DIR_CHAR,
                                         pszFileName);
    /* is there this file ? */
    if(SKY_IsFileOrDirExist(szPathName) == FALSE)
        return SDR_NO_FILE;

    //*phFile = SKY_OpenFileEx(szPathName, FS_AM_READONLY, DISCARD_CLEANUP_OPTION);
    *phFile = SKY_OpenFileEx(szPathName, 
            (openType == SFT_READ)?FS_AM_READONLY:FS_AM_READWRITE, 
            DISCARD_CLEANUP_OPTION);

    if(*phFile == SKY_INVALID_FILE_HANDLE)
        return SDR_OPEN_ERROR;

    if(openType == SFT_WRITE)
        SKY_SET_OPEN_DATA_FILE_NAME(szPathName);

    SKY_TRACE1(TL_HIGH, "%s - Open file SUCCESS", szPathName);
        
    return SDR_OK;
}

/* ---------------------------------------------------------------------------
** get file size
*/
DWORD SKY_API SKY_GetDataFileSize(SKY_DATA_FILE_E SkyDataFile,
                                  CONST BYTE* pszFileName)
{
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];

    /* make complete path */
    sprintf((char*)szPathName, "%s%s%s", l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, 
                                         SKY_DIR_CHAR,
                                         pszFileName);
    /* is there this file ? */
    if(SKY_IsFileOrDirExist(szPathName) == FALSE)
        return 0;

    return SKY_GetFileSize(szPathName);
}

/* ---------------------------------------------------------------------------
** get file size
*/
BOOL SKY_API SKY_IsDataFile(SKY_DATA_FILE_E SkyDataFile,
                                  CONST BYTE* pszFileName)
{
    BYTE szPathName[SKY_PATH_NAME_MAX_LENGTH];

    /* make complete path */
    sprintf((char*)szPathName, "%s%s%s", l_DataFileConfig[(INT8)SkyDataFile].pszDataDir, 
                                         SKY_DIR_CHAR,
                                         pszFileName);
    /* is there this file ? */
    return SKY_IsFileOrDirExist(szPathName);
}

/* ---------------------------------------------------------------------------
** close data file
*/
void SKY_API SKY_CloseDataFile(HFILE hFile) 
{
    SKY_CloseFile(hFile);

    SKY_CLEAR_OPEN_DATA_FILE_NAME();
}

/* ---------------------------------------------------------------------------
** read data file
*/
DWORD SKY_API SKY_ReadDataBlock(HFILE hFile, void *pBufferToRead, DWORD dwSize) 
{
    return SKY_ReadFile(hFile, pBufferToRead, dwSize);
}

/* ---------------------------------------------------------------------------
** write data file
*/
DWORD SKY_API SKY_WriteDataBlock(HFILE hFile, CONST void *pBufferToWrite, DWORD dwSize) 
{
    /* check free space and file count */
    //if(dwSize > SKY_GetFreeSpaceInDataFile())
    //    return SDR_OUT_OF_SPACE;

    return SKY_WriteFile(hFile, pBufferToWrite, dwSize);
}


/* ---------------------------------------------------------------------------
** mm_Bell... functions (called by yamaha.c)
*/
boolean fBellCacheMalloc;
void SKY_API mm_BellFileLoadClose(void) 
{
    /* 
    ** jrkwon 2001.07.07 --> 
    ** If haven't open the bell file, just return
    */
    if(mm_BellFile.Handle == 0)
        return;

    SKY_CloseDataFile(mm_BellFile.Handle);
	mm_BellFile.Handle = 0;
}
void SKY_API mm_BellMemoryFree(void)
{
	//SKY_Free(mm_BellBlock);
	if(fBellCacheMalloc)
	{
	  SKY_Free(mm_BellBlockCache);
	  fBellCacheMalloc=FALSE;
	}
}
UINT8 SKY_API mm_BellFileLoadOpen(BYTE *pFileName, 
					             DWORD *pFileSize,
					             BYTE **pAllocBlock)
{
    //SKY_DATA_FILE_RESULT_E  Result;

    if(mm_BellFile.Handle)
        mm_BellFileLoadClose();

//	*pAllocBlock = NULL;
     //mm_BellBlock = (BYTE*)SKY_Malloc(MM_MAX_BELL_BLOCK_SIZE);
    mm_BellFile.pBlock = mm_BellBlock;

    STRNCPY(mm_BellFile.FPathName, pFileName, SKY_PATH_NAME_MAX_LENGTH);
#ifdef FEATURE_CHARACBELL
    if (GetCharacbellSelected()) {
        if(SKY_OpenDataFile(SDF_CHA_MELODY, mm_BellFile.FPathName, &mm_BellFile.Handle, SFT_READ)
        != SDR_OK)
            return 1;
        mm_BellFile.FileSize = SKY_GetDataFileSize(SDF_CHA_MELODY, mm_BellFile.FPathName);
        SetCharacbellSelected (FALSE);
    }
    else {
        if(SKY_OpenDataFile(SDF_MELODY, mm_BellFile.FPathName, &mm_BellFile.Handle, SFT_READ)
        != SDR_OK)
            return 1;   // MM_NG (MMedia.h)
        mm_BellFile.FileSize = SKY_GetDataFileSize(SDF_MELODY, mm_BellFile.FPathName);
    }
#else
    if(SKY_OpenDataFile(SDF_MELODY, mm_BellFile.FPathName, &mm_BellFile.Handle, SFT_READ)
        != SDR_OK)
        return 1;   // MM_NG (MMedia.h)

    mm_BellFile.FileSize = SKY_GetDataFileSize(SDF_MELODY, mm_BellFile.FPathName);
#endif // FEATURE_CHARACBELL

#if MODEL_ID == MODEL_B1 || (MODEL_ID == MODEL_B2)
	//nCache_Size = mm_BellFile.FileSize-4;-이런 ma1의 잔재가 남아 있다니. -happykid
	nCache_Size = mm_BellFile.FileSize;
//youandi 01.06.11
//멜로디 데이터의 최고 크기는 50k를 넘지 않는다.
	if(nCache_Size > 50000)
		return 1;
//youandi 01.06.11
#else
	nCache_Size = BELL_FILE_CAHCE_SIZE;
#endif
//youandi 01.06.11
//데이터를 저장하는 곳을 ska_cam.c에 만들었다. 섀어드 램을 사용하기 위해서 바꿈. 
//		mm_BellBlockCache = (BYTE*)SKY_Malloc(nCache_Size);
//youandi 01.06.11
#if MODEL_ID == MODEL_B1 || (MODEL_ID == MODEL_B2)
//youandi 01.06.11
//데이터를 저장하는 곳을 ska_cam.c에 만들었다. 섀어드 램을 사용하기 위해서 바꿈. 
	if( !SKY_IS_IN_GVM() && !g_fGVMPaused ){
		mm_BellBlockCache=*pAllocBlock;
	}else{
		*pAllocBlock = NULL;
		mm_BellBlockCache = (BYTE*)SKY_Malloc(nCache_Size);
		*pAllocBlock = mm_BellBlockCache;
		fBellCacheMalloc=TRUE;
	}
//youandi 01.06.11
#else
	*pAllocBlock = mm_BellBlock;
#endif
	*pFileSize = mm_BellFile.FileSize;      // value 0 : not used??
	mm_SeekPos = 0;
	mm_CacheSPos = mm_CacheEPos = 0;

	mm_BellFileBlockSeek(0);

    return 0;   // MM_OK (MMedia.h)
}

void mm_BellFileBlockSeek(DWORD Position)
{
	if(mm_BellFile.Handle)
    {
	    mm_SeekPos = Position;
        SKY_SeekFile(mm_BellFile.Handle, 
                     FS_SO_SET, 
                     Position + MM_NUM_FILE_TYPE);
    }
}

UINT8 mm_BellFileBlockLoad(WORD BlockSize)
{
	DWORD EndPos, Size;

	if(BlockSize > MM_MAX_BELL_BLOCK_SIZE)
		return 1;       // MM_NG

	if(mm_BellFile.Handle == 0)
		return 1;       // MM_NG

	// Required area is not in cached area
	if(!((mm_SeekPos >= mm_CacheSPos)
	    && (mm_SeekPos + BlockSize <= mm_CacheEPos)))
	{
		mm_BellFile.pBlock = mm_BellBlockCache;
		mm_BellFileBlockSeek(mm_SeekPos);
		EndPos = mm_BellFile.FileSize - mm_SeekPos - 1;
		if(EndPos > nCache_Size - 1)
			EndPos = nCache_Size - 1;

        Size = SKY_ReadDataBlock(mm_BellFile.Handle, mm_BellFile.pBlock, EndPos+1);

		mm_BellFile.pBlock = mm_BellBlock;
		if(Size > 0)
		{
			mm_CacheSPos = mm_SeekPos;
			mm_CacheEPos = EndPos+mm_CacheSPos;
		}
	}
	else
		Size = MM_MAX_BELL_BLOCK_SIZE;

	if(Size > 0)
	{
		memcpy(mm_BellFile.pBlock, &mm_BellBlockCache[mm_SeekPos - mm_CacheSPos], BlockSize);
		mm_SeekPos += BlockSize;
	}

    return 0;       // MM_OK
}

void ConvertCameraImageFileName(BYTE* pszFileName, DWORD dwIndex)
{
    //BYTE  szFileNumber[SKY_MAX_CAMERA_IMAGE_FILE_LEN+4]; 
    //BYTE  szNumber[SKY_MAX_CAMERA_IMAGE_FILE_LEN+1];
    //UINT8 nLen;    
    
    memset(pszFileName,NULL,SKY_MAX_CAMERA_IMAGE_FILE_LEN+1);   
    sprintf((char *)pszFileName,"%lu",dwIndex);
   
    
    //STRCAT(pszFileName, (BYTE*)".ebi");
}

HFILE SKY_MakeCameraImageFile(DWORD dwIndex)
{
    BYTE  szFileNumber[SKY_MAX_CAMERA_IMAGE_FILE_LEN+1];//6]; 
    HFILE hFile;
    SKY_DATA_FILE_RESULT_E  Result;
    
    ConvertCameraImageFileName(szFileNumber, dwIndex);
    Result = SKY_CreateDataFile(SDF_PHOTO, szFileNumber, &hFile);
    if(Result == SDR_OK)
    {
        SKY_ADD_CAMERA_COUNT();
        return hFile;
    }else if(Result == SDR_FILE_EXIST)
    {
        do
        {
            SKY_ADD_CAMERA_COUNT();
            ConvertCameraImageFileName(szFileNumber, SKY_GET_CAMERA_COUNT());
            Result = SKY_CreateDataFile(SDF_PHOTO, szFileNumber, &hFile);
        }while(Result == SDR_FILE_EXIST);
        
        if(Result == SDR_OK)
        {
            SKY_ADD_CAMERA_COUNT();
            return hFile;
        }
        else
            return SKY_INVALID_FILE_HANDLE;
    }
    else
        return SKY_INVALID_FILE_HANDLE;

}

VOID SKY_SetCameraCount()
{
    BYTE bTotal, index;
    BYTE *ppPhotoList[MAX_PHOTO_COUNT];
    DWORD   dwCount;

    bTotal = SKY_GetFileNumberInDataFile(SDF_PHOTO);

    if(bTotal)
    {
        for(index=0; index < bTotal ; index ++)
        {
            ppPhotoList[index] = (BYTE*)SKY_Malloc(SKY_FILE_EXT_NAME_MAX_LENGTH+1);
        }
        SKY_GetFileList(SKY_PHOTO_DIR, ppPhotoList);
    
        dwCount = atol((char*)ppPhotoList[bTotal-1]);
        //SKY_SET_CAMERA_COUNT(dwCount+1);

        for(index=0; index < bTotal ; index ++)
        {    
            SKY_Free(ppPhotoList[index]);
        }
    }
}

BOOL SKY_IsCameraData(CHAR* szName)
{
    STATIC WORD s_wIdx=0;
    WORD wIndex;
    BOOL    fReturn = FALSE;
    BYTE *ppPhotoList[MAX_PHOTO_COUNT];
    BYTE szTemp[SKY_MAX_CAMERA_IMAGE_FILE_LEN+1];
    HFILE   hFile;

    s_wIdx = SKY_GetFileNumberInDataFile(SDF_PHOTO);
    
    for(wIndex=0; wIndex < s_wIdx ; wIndex ++)
    {
        ppPhotoList[wIndex] = (BYTE*)SKY_Malloc(SKY_FILE_EXT_NAME_MAX_LENGTH+1);
    }

    SKY_GetFileList(SKY_PHOTO_DIR, ppPhotoList);

    for(wIndex=0; wIndex < s_wIdx ; wIndex ++)
    {
        SKY_OpenDataFile(SDF_PHOTO, ppPhotoList[wIndex], &hFile, SFT_READ);
        SKY_ReadDataBlock(hFile, szTemp, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
        SKY_CloseDataFile(hFile);
        if(!STRCMP((BYTE *)szTemp, (BYTE *)szName))
        {
            fReturn = TRUE;
            break;
        }
    }
    for(wIndex=0; wIndex < s_wIdx ; wIndex ++)
    {
        SKY_Free(ppPhotoList[wIndex]);
    }
            
    return fReturn;
}
