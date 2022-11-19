/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     skyrefs.h
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
** 2000-09-27 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKYREFS_H_
#define _SKYREFS_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "fs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SKY_DIR_CHAR                 "/" /* FS_DIRECTORY_CHAR */
//#define SKY_TEMP_FILE_NAME1          "JRHSHNBA.TMP"
#define SKY_TEMP_FILE_NAME             (BYTE*)"JRHSHNBA.TMP"//SKY_MELODY_DIR""SKY_TEMP_FILE_NAME1

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef fs_handle_type          HFILE;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern HFILE SKY_API SKY_OpenFile(CONST BYTE* pszFileName, FS_ACCESS_MODE_E Mode);
extern HFILE SKY_API SKY_OpenFileEx(CONST BYTE* pszFileName, FS_ACCESS_MODE_E Mode,
                                    FS_CLEANUP_OPTION_E CleanUpOption);
extern HFILE SKY_API SKY_OpenTempFile(void);
extern void  SKY_API SKY_CloseFile(HFILE hFile);
extern DWORD SKY_API SKY_ReadFile(HFILE hFile, void* pBuffer, DWORD dwCount);
extern DWORD SKY_API SKY_WriteFile(HFILE hFile, const void* pBuffer, DWORD dwCount);
extern void  SKY_API SKY_SeekFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin, 
                          DWORD dwPosition);
extern DWORD SKY_API SKY_SeekReadFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin,
                              DWORD dwPosition, 
                              void* pBuffer, DWORD dwCount);
extern DWORD SKY_API SKY_SeekWriteFile(HFILE hFile, FS_SEEK_ORIGIN_E SeekOrigin,
                               DWORD dwPosition, 
                               void* pBuffer, DWORD dwCount);
extern void SKY_API SKY_DeleteFile(CONST BYTE* pszFileName);
extern void SKY_API SKY_RenameFile(CONST BYTE* pszOldFileName, CONST BYTE* pszNewFileName);
extern void SKY_API SKY_MakeDir(CONST BYTE* pszDirName);
extern void SKY_API SKY_RemoveDir(CONST BYTE* pszDirName);
extern DWORD SKY_API SKY_GetFileSize(const BYTE* pszFileName);
extern DWORD SKY_API SKY_GetFileSizeByHandle(HFILE hFile);
extern DWORD SKY_API SKY_GetDirSize(const BYTE*  pszDirName);
extern DWORD SKY_API SKY_GetFreeSpace(void);
extern DWORD SKY_API SKY_GetUsedSpace(void);
extern WORD SKY_API SKY_GetNumFile(const BYTE* pszDirName);
extern WORD SKY_API SKY_GetNumDir(const BYTE* pszDirName);
extern WORD SKY_API SKY_GetFileNameList(const BYTE* pszPathName, \
                                 BYTE* pszFileNameList);
extern WORD SKY_API SKY_GetDirNameList(const BYTE* pszPathName, BYTE* pszDirNameList);
extern WORD SKY_API SKY_DeleteAllFile(const BYTE* pszPathName);
extern BOOL SKY_API SKY_IsFileOrDirExist(const BYTE* pszDirOrFileName);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKYREFS_H_ */
