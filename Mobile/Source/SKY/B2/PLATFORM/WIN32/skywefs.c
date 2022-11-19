/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     skywefs.c
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
#include <winerror.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <time.h>


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
LOCAL SKY_API int sOpenFlag(FS_ACCESS_MODE_E Mode);
LOCAL SKY_API int sSeekOrigin(FS_SEEK_ORIGIN_E Origin);

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

/* convet open mode from EQUS FS_SEEK_ORIGIN to the PC file system */
LOCAL SKY_API int sOpenFlag(FS_ACCESS_MODE_E Mode)
{
    int anMode[] = {
        _O_BINARY | _O_CREAT | _O_RDWR ,  /* FS_AM_CREATE    */
        _O_BINARY | _O_APPEND,  /* FS_AM_APPEND    */
        _O_BINARY | _O_RDONLY,  /* FS_AM_READONLY  */
        _O_BINARY | _O_RDWR,    /* FS_AM_READWRITE */
        _O_BINARY | _O_TRUNC,   /* FS_AM_TRUNCATE  */
        _O_BINARY               /* FS_AM_TEST      */
    };

    return anMode[Mode];
}

/* convet open mode from EQUS FS_SEEK_ORIGIN to the PC file system */
LOCAL SKY_API int sSeekOrigin(FS_SEEK_ORIGIN_E Origin)
{
    int anSeekOrigin[] = {
        SEEK_SET,   /* FS_SO_SET */
        SEEK_CUR,   /* FS_SO_CURRENT */
        SEEK_END    /* FS_SO_EOF */
    };

    return anSeekOrigin[Origin];
}

HFILE SKY_API SKY_OpenFile(CONST BYTE* pszFileName, FS_ACCESS_MODE_E Mode)
{
    HFILE hFile;    
    hFile = _open((const char*)pszFileName, sOpenFlag(Mode), _S_IREAD | _S_IWRITE);

    //EQS_ASSERT(hFile != -1);
    if( hFile == -1 )
        return SKY_INVALID_FILE_HANDLE;

    return hFile;
}

HFILE SKY_API SKY_OpenFileEx(CONST BYTE* pszFileName, FS_ACCESS_MODE_E Mode,
                             FS_CLEANUP_OPTION_E CleanUpOption)
{
    UNUSED(CleanUpOption);

    return SKY_OpenFile(pszFileName, Mode);
}

HFILE SKY_API SKY_OpenTempFile(void)
{
    return SKY_OpenFile(SKY_TEMP_FILE_NAME, _O_BINARY | _O_CREAT | _O_RDWR);
}

void SKY_API SKY_CloseFile(HFILE hFile)
{
    _close(hFile);
}

DWORD SKY_API SKY_ReadFile(HFILE hFile, void* pBuffer, DWORD dwCount)
{
    return (DWORD)_read(hFile, pBuffer, (unsigned int)dwCount);
}

DWORD SKY_API SKY_WriteFile(HFILE hFile, const void* pBuffer, DWORD dwCount)
{
    return (DWORD)_write(hFile, pBuffer, (unsigned int)dwCount);
}

void SKY_API SKY_SeekFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin, 
                          DWORD dwPosition)
{
    _lseek(hFile, dwPosition, sSeekOrigin(SeekOrigin));
}

DWORD SKY_API SKY_SeekReadFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin,
                              DWORD dwPosition, 
                              void* pBuffer, DWORD dwCount)
{
    _lseek(hFile, dwPosition, sSeekOrigin(SeekOrigin));
    return (DWORD)_read(hFile, pBuffer, (unsigned int)dwCount);
}

DWORD SKY_API SKY_SeekWriteFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin,
                               DWORD dwPosition, 
                               void* pBuffer, DWORD dwCount)
{
    _lseek(hFile, dwPosition, sSeekOrigin(SeekOrigin));
    return (DWORD)_write(hFile, pBuffer, (unsigned int)dwCount);
}

void SKY_API SKY_DeleteFile(BYTE* pszFileName)
{
/*    extern unsigned char __cdecl DeleteFile(unsigned short *);

    DeleteFile((unsigned short*)pszFileName);
*/
    remove(pszFileName);
}

void SKY_API SKY_RenameFile(CONST BYTE* pszOldFileName, CONST BYTE* pszNewFileName)
{
    rename((const char*)pszOldFileName, (const char*)pszNewFileName);
}

void SKY_API SKY_MakeDir(BYTE* pszDirName)
{
    _mkdir((const char*)pszDirName);
}

void SKY_API SKY_RemoveDir(BYTE* pszDirName)
{
    _rmdir((const char*)pszDirName);
}

UINT SKY_API SKY_GetFileSize(const BYTE* pszFileName)
{   
	struct _finddata_t c_file;
	HFILE  hFile;
			
    if( (hFile = _findfirst((char*) pszFileName, &c_file )) == -1L )
    {
    	return 0;
    }
    return c_file.size;   
}

DWORD SKY_API SKY_GetFileSizeByHandle(HFILE hFile)
{   
    DWORD dwFileSize;

    _lseek(hFile, 0L, SEEK_END);
    dwFileSize = _tell(hFile);
    if(dwFileSize == -1) // error
        return 0;
    return dwFileSize;
}

DWORD SKY_API SKY_GetDirSize(const BYTE*  pszDirName)
{
	struct _finddata_t c_file;
	HFILE  hFile;		
	DWORD  dwDirSize = 0;
    BYTE szPathName[_MAX_PATH];

    sprintf((char*)szPathName, "%s\\*.*", pszDirName);

//    if( _chdir((char*) pszDirName) )
//        return 0;
        
    if( (hFile = _findfirst( szPathName, &c_file )) == -1L )
    {
    	return 0;
    }
    
    do
    {
    	if(!(c_file.attrib &  _A_SUBDIR))
    	{
    	    dwDirSize += c_file.size;
    	}
    } while( _findnext( hFile, &c_file) == 0 );
    
    return dwDirSize;   		
}

DWORD SKY_API SKY_GetFreeSpace(void)
{
	return 1024*1024;
}

DWORD SKY_API SKY_GetUsedSpace(void)
{
    return 1024;
}	

WORD SKY_API SKY_GetNumFile(const BYTE* pszDirName)
{
	struct _finddata_t c_file;
	HFILE  hFile;
	WORD   nFiles = 0;
    BYTE szPathName[_MAX_PATH];
		
    sprintf((char*)szPathName, "%s\\*.*", pszDirName);

//    if( _chdir((char*) pszDirName) )
//        return 0;
        		
    if( (hFile = _findfirst( szPathName, &c_file )) == -1L )
    {
    	return 0;
    }
    
    do 
    {
    	if( !(c_file.attrib &  _A_SUBDIR) )
    	   nFiles++;
    } while( _findnext( hFile, &c_file) == 0 );

    return nFiles;                  	
}	

WORD SKY_API SKY_GetNumDir(const BYTE* pszPathName)
{
	struct _finddata_t c_file;
	HFILE  hFile;
	WORD   nDir = 0;
    BYTE szPathName[_MAX_PATH];
    
    sprintf((char*)szPathName, "%s\\*.*", pszPathName);
//    if( _chdir((char*) pszPathName) )
//        return 0;		
    if( (hFile = _findfirst( szPathName, &c_file )) == -1L )
    {
    	return 0;
    }
    
/*    do
    {
    	if( c_file.attrib &  _A_SUBDIR )
    } while( _findnext( hFile, &c_file) == 0 );
*/    
    return nDir;      			
}

WORD SKY_API SKY_GetFileNameList(const BYTE* pszPathName, BYTE* pFileNameList )
{
	struct _finddata_t c_file;
	long  hFile;
    BYTE szPathName[_MAX_PATH];
	WORD nFiles = 0;
    BYTE *pList;

    sprintf((char*)szPathName, "%s\\*.*", pszPathName);

    pList = pFileNameList;
    /* Find first .c file in current directory */
    if( (hFile = _findfirst( szPathName, &c_file )) != -1L )
    {
        if(!(c_file.attrib & _A_SUBDIR))
        {
            STRCPY(pList, (BYTE*)c_file.name);
            pList += STRLEN((BYTE*)c_file.name);
            *pList = '\0';
            pList++;

            //DeleteFile(c_file.name);
            nFiles++;
        }
        /* Find the rest of the .c files */
        while( _findnext( hFile, &c_file ) == 0 )
        {
            if(!(c_file.attrib & _A_SUBDIR))
            {
                STRCPY(pList, (BYTE*)c_file.name);
                pList += STRLEN((BYTE*)c_file.name);
                *pList = '\0';
                pList++;

                //DeleteFile(c_file.name);
                nFiles++;
            }
        }
        *pList = '\0';

        _findclose( hFile );
    }

    return nFiles;	
}

WORD SKY_API SKY_GetDirNameList(const BYTE* pszPathName, BYTE* pDirNameList)
{
	struct _finddata_t c_file;
	long  hFile;
    BYTE szPathName[_MAX_PATH];
	WORD nDirs = 0;
    BYTE *pList;

    sprintf((char*)szPathName, "%s\\*.*", pszPathName);

    pList = pDirNameList;
    /* Find first .c file in current directory */
    if( (hFile = _findfirst( szPathName, &c_file )) != -1L )
    {
        if(c_file.attrib & _A_SUBDIR) 
        {
            STRCPY(pList, (BYTE*)c_file.name);
            pList += STRLEN((BYTE*)c_file.name);
            *pList = '\0';
            pList++;
    
            nDirs++;
        }

        /* Find the rest of the .c files */
        while( _findnext( hFile, &c_file ) == 0 )
        {
            if(c_file.attrib & _A_SUBDIR) 
            {
                STRCPY(pList, (BYTE*)c_file.name);
                pList += STRLEN((BYTE*)c_file.name);
                *pList = '\0';
                pList++;
    
                nDirs++;
            }
        }
        *pList = '\0';

        _findclose( hFile );
    }

    return nDirs;	
}

WORD SKY_API SKY_DeleteAllFile(const BYTE* pszPathName)
{
    struct _finddata_t c_file;
    long hFile;
    BYTE szPathName[_MAX_PATH];
    BYTE szDeleteName[_MAX_PATH];
	WORD nFiles = 0;

    sprintf((char*)szPathName, "%s\\*.*", pszPathName);

    /* Find first .c file in current directory */
    if( (hFile = _findfirst( szPathName, &c_file )) != -1L )
    {
        sprintf(szDeleteName, "%s\\%s", pszPathName, c_file.name);
        remove(szDeleteName);//c_file.name);
        //DeleteFile(c_file.name);
        nFiles++;
        /* Find the rest of the .c files */
        while( _findnext( hFile, &c_file ) == 0 )
        {
            sprintf(szDeleteName, "%s\\%s", pszPathName, c_file.name);
            remove(szDeleteName);//c_file.name);
            //DeleteFile(c_file.name);
            nFiles++;
        }

        _findclose( hFile );
    }

    return nFiles;	
}

BOOL SKY_API SKY_IsFileOrDirExist(const BYTE* pszDirOrFileName)
{
    struct _finddata_t c_file;
    long hFile;

    /* Find first .c file in current directory */

    if( (hFile = _findfirst( pszDirOrFileName, &c_file )) == -1L )
        return FALSE;


    return TRUE;
}
