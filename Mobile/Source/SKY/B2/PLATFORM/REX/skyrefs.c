/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     skyqefs.c
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
** 2000-11-30 redstar    add file utility function
** 2000-09-26 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "fs.h"
#include "sky.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
//#define FILE_EXT_NAME_SIZE      SKY_FILE_EXT_NAME_MAX_LENGTH //FS_FILENAME_MAX_LENGTH
//#define MAX_NUM_FILES           SKY_MAX_NUM_FILES

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

/* ***************************************************************************
** use some types defined in fs.h
** we just redefine the types to EQS style
** All of file operations is executed only in synchrnonous mode
*/

/* convet open mode from the EQUS FS_ACCESS_MODE to the EFS */
LOCAL fs_access_option_type SKY_API sOpenFlag(FS_ACCESS_MODE_E Mode)
{
    fs_access_option_type anMode[] = {
        FS_OA_CREATE,           /* FS_AM_CREATE    */
        FS_OA_APPEND,           /* FS_AM_APPEND    */
        FS_OA_READONLY,         /* FS_AM_READONLY  */
        FS_OA_READWRITE,        /* FS_AM_READWRITE */
        FS_OA_TRUNCATE,         /* FS_AM_TRUNCATE  */
        FS_OA_TEST              /* FS_AM_TEST      */
    };

    return anMode[Mode];
}

/* convet open mode from EQUS FS_SEEK_ORIGIN to EFS */
LOCAL fs_seek_origin_type SKY_API sSeekOrigin(FS_SEEK_ORIGIN_E Origin)
{
    fs_seek_origin_type anSeekOrigin[] = {
        FS_SEEK_SET,        /* FS_SO_SET */
        FS_SEEK_CURRENT,    /* FS_SO_CURRENT */
        FS_SEEK_EOF         /* FS_SO_EOF */
    };

    return anSeekOrigin[Origin];
}

LOCAL fs_cleanup_option_type SKY_API sCleanUp(FS_CLEANUP_OPTION_E CleanUpOption)
{
    fs_cleanup_option_type aCleanUpOption[] = {
        FS_OC_CLOSE,        /* FS_CO_CLOSE */
        FS_OC_DISCARD,      /* FS_CO_DISCARD */
        FS_OC_TRUNCATE,     /* FS_CO_TRUNCATE */
        FS_OC_REVERT        /* FS_CO_REVERT */
    };

    return aCleanUpOption[CleanUpOption];
}

HFILE SKY_API SKY_OpenFile(CONST BYTE* pszFileName, FS_ACCESS_MODE_E Mode)
{
    fs_open_xparms_type open_options;
    fs_rsp_msg_type rsp_msg;
    
    open_options.create.cleanup_option = FS_OC_CLOSE;
    open_options.create.buffering_option = FS_OB_ALLOW;
    open_options.create.attribute_mask = FS_FA_UNRESTRICTED;

    fs_open((const char*)pszFileName, sOpenFlag(Mode), &open_options, NULL, &rsp_msg);

    return (rsp_msg.open.handle == FS_NULL_HANDLE)
           ? SKY_INVALID_FILE_HANDLE : rsp_msg.open.handle;
}

/* ---------------------------------------------------------------------------
** melody, image, GVM data file download
*/
HFILE SKY_API SKY_OpenFileEx(CONST BYTE* pszFileName, FS_ACCESS_MODE_E Mode,
                             FS_CLEANUP_OPTION_E CleanUpOption)
{
    fs_open_xparms_type open_options;
    fs_rsp_msg_type rsp_msg;
    
    open_options.create.cleanup_option = sCleanUp(CleanUpOption);
    open_options.create.buffering_option = FS_OB_ALLOW;
    open_options.create.attribute_mask = FS_FA_UNRESTRICTED;

    fs_open((const char*)pszFileName, sOpenFlag(Mode), &open_options, NULL, &rsp_msg);

    return (rsp_msg.open.handle == FS_NULL_HANDLE)
           ? SKY_INVALID_FILE_HANDLE : rsp_msg.open.handle;
}

HFILE SKY_API SKY_OpenTempFile(void)
{
    fs_open_xparms_type open_options;
    fs_rsp_msg_type rsp_msg;

    if(SKY_IsFileOrDirExist(SKY_TEMP_FILE_NAME))
    {
        if(SKY_GET_DM_TEMP_FILE() != SKY_INVALID_FILE_HANDLE)
            SKY_CloseFile(SKY_GET_DM_TEMP_FILE());
        
        SKY_SET_DM_TEMP_FILE(SKY_INVALID_FILE_HANDLE);
        SKY_DeleteFile(SKY_TEMP_FILE_NAME);
    }

    open_options.create.cleanup_option = FS_OC_CLOSE;
    open_options.create.buffering_option = FS_OB_ALLOW;
    open_options.create.attribute_mask = FS_FA_UNRESTRICTED;

    fs_open((const char*)SKY_TEMP_FILE_NAME, FS_OA_CREATE, &open_options, NULL, &rsp_msg);

    return (rsp_msg.open.handle == FS_NULL_HANDLE)
           ? SKY_INVALID_FILE_HANDLE : rsp_msg.open.handle;
}

void SKY_API SKY_CloseFile(HFILE hFile)
{
    fs_rsp_msg_type rsp_msg;

    fs_close(hFile, NULL, &rsp_msg);

    SKY_ASSERT_VOID(rsp_msg.close.status == FS_OKAY_S);
}

DWORD SKY_API SKY_ReadFile(HFILE hFile, void* pBuffer, DWORD dwCount)
{
    fs_rsp_msg_type rsp_msg;

    fs_read(hFile, pBuffer, dwCount, NULL, &rsp_msg);

    SKY_ASSERT_EX(rsp_msg.read.status == FS_OKAY_S, 0);

    return rsp_msg.read.count;
}

DWORD SKY_API SKY_WriteFile(HFILE hFile, const void* pBuffer, DWORD dwCount)
{
    fs_rsp_msg_type rsp_msg;

    fs_write(hFile, pBuffer, dwCount, NULL, &rsp_msg);

    SKY_ASSERT_EX(rsp_msg.write.status == FS_OKAY_S, 0);

    return rsp_msg.write.count;
}

void SKY_API SKY_SeekFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin, 
                          DWORD dwPosition)
{
    fs_rsp_msg_type rsp_msg;

    fs_seek(hFile, sSeekOrigin(SeekOrigin), dwPosition, NULL, &rsp_msg);
    /* 
    ** jrkwon 2001.06.28 실제 파일보다 큰 영역을 읽어오려고 하는 경우가 경우가 빈번해서 
    ** assert 삭제
    */
    SKY_ASSERT_VOID(rsp_msg.seek.status == FS_OKAY_S);
}

DWORD SKY_API SKY_SeekReadFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin,
                              DWORD dwPosition, 
                              void* pBuffer, DWORD dwCount)
{
    fs_rsp_msg_type rsp_msg;

    /* rosa 01-05-17 --> @why, what */
    if(dwCount == 0)
        return 0;

    fs_seek_read(hFile, sSeekOrigin(SeekOrigin), dwPosition, pBuffer, dwCount, 
                 NULL, &rsp_msg);

    /* 
    ** jrkwon 2001.06.28 실제 파일보다 큰 영역을 읽어오려고 하는 경우가 경우가 빈번해서 
    ** assert 삭제
    */
    SKY_ASSERT_EX(rsp_msg.seek_read.status == FS_OKAY_S, 0);

    return rsp_msg.seek_read.count;
}

DWORD SKY_API SKY_SeekWriteFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin,
                               DWORD dwPosition, 
                               void* pBuffer, DWORD dwCount)
{
    fs_rsp_msg_type rsp_msg;

    /* rosa 01-05-17 --> @why, what */
    if(dwCount == 0)
        return 0;

    fs_seek_write(hFile, sSeekOrigin(SeekOrigin), dwPosition, pBuffer, dwCount, 
                 NULL, &rsp_msg);

    /* 
    ** jrkwon 2001.06.28 실제 파일보다 큰 영역을 읽어오려고 하는 경우가 경우가 빈번해서 
    ** assert 삭제
    */
    SKY_ASSERT_EX(rsp_msg.seek_write.status == FS_OKAY_S, 0);

    return rsp_msg.seek_write.count;
}

void SKY_API SKY_DeleteFile(CONST BYTE* pszFileName)
{
    fs_rsp_msg_type rsp_msg;

    fs_test((const char*)pszFileName, NULL, &rsp_msg);
    SKY_ASSERT_VOID( rsp_msg.nametest.status == FS_OKAY_S );
    if(rsp_msg.nametest.name_found)
    {
        fs_remove((const char *)pszFileName, NULL, &rsp_msg);
        SKY_ASSERT_VOID(rsp_msg.rmfile.status == FS_OKAY_S);
    }
}

void SKY_API SKY_RenameFile(CONST BYTE* pszOldFileName, CONST BYTE* pszNewFileName)
{
    fs_rsp_msg_type rsp_msg;

    fs_rename((const char*)pszOldFileName, (const char *)pszNewFileName, NULL, &rsp_msg);
    SKY_ASSERT_VOID( rsp_msg.rename.status == FS_OKAY_S );
}

void SKY_API SKY_MakeDir(CONST BYTE* pszDirName)
{
    fs_rsp_msg_type rsp_msg;

    fs_mkdir((const char *)pszDirName, NULL, &rsp_msg);

    SKY_ASSERT_VOID(rsp_msg.mkdir.status == FS_OKAY_S);
}

void SKY_API SKY_RemoveDir(CONST BYTE* pszDirName)
{
    fs_rsp_msg_type rsp_msg;

    fs_test((const char*)pszDirName, NULL, &rsp_msg);
    SKY_ASSERT_VOID( rsp_msg.nametest.status == FS_OKAY_S );

    if(rsp_msg.nametest.name_found)
    {
        fs_rmdir((const char *)pszDirName, NULL, &rsp_msg);
        SKY_ASSERT_VOID(rsp_msg.rmdir.status == FS_OKAY_S);
    }
}

DWORD SKY_API SKY_GetFileSize(const BYTE* pszFileName)
{
	fs_rsp_msg_type rsp_msg;
	
	fs_file_size( (const char*) pszFileName, NULL, &rsp_msg );
	
	SKY_ASSERT_EX(rsp_msg.file_size.status == FS_OKAY_S, 0);
	
	return rsp_msg.file_size.size;
		
}

DWORD SKY_API SKY_GetFileSizeByHandle(HFILE hFile)
{
	fs_rsp_msg_type rsp_msg;
	
    fs_seek(hFile, FS_SEEK_EOF, 0, NULL, &rsp_msg);
	SKY_ASSERT_EX(rsp_msg.seek.status == FS_OKAY_S, 0);
    fs_tell(hFile, NULL, &rsp_msg);
	SKY_ASSERT_EX(rsp_msg.tell.status == FS_OKAY_S, 0);
	
	return rsp_msg.tell.position;
}

DWORD SKY_API SKY_GetDirSize(const BYTE*  pszDirName)
{
	DWORD dwDirSize;
	fs_enum_iterator_type Iterator;
    fs_enum_data_type     FileInfo;
	fs_rsp_msg_type	rsp_msg;

    if( SKY_GetNumFile(pszDirName) == 0 )
    {
    	return 0;
    }

    dwDirSize = 0;
    fs_enum_init((const char*)pszDirName, FS_ENUM_FILES, &Iterator, NULL, &rsp_msg);
    do 
    {
        fs_enum_next(&FileInfo, &Iterator, NULL, &rsp_msg);
        if(rsp_msg.enum_next.status != FS_ENUM_DONE_S)
            dwDirSize += FileInfo.logical_size;
    } while (rsp_msg.enum_next.status != FS_ENUM_DONE_S);
	
    return dwDirSize;				
}

DWORD SKY_API SKY_GetFreeSpace(void)
{
	return fs_space_available();	
}

DWORD SKY_API SKY_GetUsedSpace(void)
{
	return fs_space_used();	
}	

WORD SKY_API SKY_GetNumFile(const BYTE* pszDirName)
{
	fs_rsp_msg_type rsp_msg;

 
	fs_num_files( (const char*)pszDirName, NULL, &rsp_msg);
	
	SKY_ASSERT_EX(rsp_msg.num_files.status == FS_OKAY_S, 0);
	
	return (WORD) rsp_msg.num_files.num_files;
	
}	

WORD SKY_API SKY_GetNumDir(const BYTE* pszDirName)
{
	fs_rsp_msg_type rsp_msg;
	
	fs_num_dirs( (const char*)pszDirName, NULL, &rsp_msg);
	
	SKY_ASSERT_EX(rsp_msg.num_dirs.status == FS_OKAY_S, 0);
	
	return (WORD) rsp_msg.num_dirs.num_dirs;
}

/* return value : the number of files */
WORD SKY_API SKY_GetFileNameList(const BYTE* pszPathName, BYTE* pszFileNameList)
//                                 BYTE* pszFileNameList, WORD nLine )
{
	fs_rsp_msg_type rsp_msg;
	
	fs_list_files((const char*) pszPathName, pszFileNameList, 
                   SKY_PATH_NAME_MAX_LENGTH*SKY_MAX_NUM_FILES,
	               NULL, &rsp_msg );
	SKY_ASSERT_EX( rsp_msg.list_files.status == FS_OKAY_S, 0);
	
	return SKY_GetNumFile(pszPathName);//rsp_msg.list_files.count;
		
}

/* return value : the number of dirs */
WORD SKY_API SKY_GetDirNameList(const BYTE* pszPathName, BYTE* pszDirNameList)
//                                WORD nLine )
{
	fs_rsp_msg_type rsp_msg;
	
	fs_list_dirs( (const char*) pszPathName, pszDirNameList, \
	              SKY_PATH_NAME_MAX_LENGTH*SKY_MAX_NUM_FILES, NULL, &rsp_msg);
	              
    SKY_ASSERT_EX( rsp_msg.list_dirs.status == FS_OKAY_S, 0 );
    
    return SKY_GetNumDir(pszPathName);//rsp_msg.list_dirs.count;	              
	
}

WORD SKY_API SKY_DeleteAllFile(const BYTE* pszPathName)
{
	WORD nFiles;
    WORD i;
	BYTE *pList;
	BYTE szFile[SKY_PATH_NAME_MAX_LENGTH];
	WORD nListPos, nFileNameSize;
	fs_rsp_msg_type	rsp_msg;

    nFiles = SKY_GetNumFile(pszPathName);
    if( nFiles == 0 )
    {
    	return 0;
    }
    pList = (BYTE *) SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH * nFiles);

	fs_list_files(
		(const char*)pszPathName,
		pList,
		SKY_PATH_NAME_MAX_LENGTH* nFiles,
		NULL, // Synchronous invocation
		&rsp_msg);	

	if( rsp_msg.list_files.status != FS_OKAY_S)	
	{
		SKY_Free( pList );
		return 0;
	}

	nListPos = 0;
	
	for(i = 0; i < nFiles; i++)
	{
		STRNCPY( szFile, pList + nListPos, SKY_PATH_NAME_MAX_LENGTH);
		nFileNameSize = STRLEN( szFile );
		if( nFileNameSize == 0 )
		    break;
		nListPos += (nFileNameSize+1);
		fs_remove((const char*)szFile , NULL , &rsp_msg);											
	}
	
	SKY_Free( pList );
	
	return nFiles;	
}

BOOL SKY_API SKY_IsFileOrDirExist(const BYTE* pszDirOrFileName)
{
	fs_rsp_msg_type	rsp_msg;

    fs_test((const char*)pszDirOrFileName, NULL, &rsp_msg);

    SKY_ASSERT( rsp_msg.nametest.status == FS_OKAY_S);

    return rsp_msg.nametest.name_found;
}
