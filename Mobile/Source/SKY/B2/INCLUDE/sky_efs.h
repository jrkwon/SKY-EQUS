/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_efs.h
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

#ifndef _SKY_EFS_H_
#define _SKY_EFS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
enum tagFS_ACCESS_MODE_E {
    FS_AM_CREATE,
    FS_AM_APPEND,
    FS_AM_READONLY,
    FS_AM_READWRITE,
    FS_AM_TRUNCATE,
    FS_AM_TEST
};
typedef enum tagFS_ACCESS_MODE_E      FS_ACCESS_MODE_E;

enum tagFS_SEEK_ORIGIN_E {
    FS_SO_SET,
    FS_SO_CURRENT,
    FS_SO_END
};
typedef enum tagFS_SEEK_ORIGIN_E      FS_SEEK_ORIGIN_E;

enum tagFS_CLEANUP_OPTION_E {
    FS_CO_CLOSE,            /* normal */
    FS_CO_DISCARD,          /* downloaded file */
    FS_CO_TRUNCATE,
    FS_CO_REVERT
};
typedef enum tagFS_CLEANUP_OPTION_E   FS_CLEANUP_OPTION_E;

#define SKY_INVALID_FILE_HANDLE    0xffff

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




#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_EFS_H_ */
