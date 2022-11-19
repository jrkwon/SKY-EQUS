/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_cam.h
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
** 4334-01-11 white     Created.
**
** ============================================================================
*/

#ifndef _SKA_CAM_H_
#define _SKA_CAM_H_

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
#define MY_PHOTO_NOT_EXIST  0

#define IMAGE_HEADER_SIZE               9 + 1    
#define IMAGE_VIEWER_WIDTH              120
#define IMAGE_VIEWER_HEIGHT             120
#define IMAGE_VIEWER_TOTAL_SIZE         (14400+10800)
#define PHOTO_LIST_LINE                 7

#define PHOTO_HEADER_SIZE               IMAGE_HEADER_SIZE   
#define PHOTO_H_SIZE                    120//96
#define PHOTO_V_SIZE                    90//72
//#define PHOTO_TOTAL_SIZE                10800*2//6912    
#define PHOTO_MAX_DATA                  40
//#define COLOR_IDLE_IMAGE_BPP            16  

#define PHOTO_DRAW_X                    4//10
#define PHOTO_DRAW_Y                    18//8
#define CAMERA_BUF_SPACE                3600

#define CAMERA_TURN                     1

#define CAMERA_NAME_EDIT_MAX_LEN        14
#define CAMERA_NAME_NULL_POS            15
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
struct tagPHOTO_LIST_T{
    DWORD PhotoList;
    BYTE  PhotoName[SKY_FILE_EXT_NAME_MAX_LENGTH+1];
};
typedef struct tagPHOTO_LIST_T PHOTO_LIST_T;
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
extern void SKY_API SKA_MainCamera(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainImageViewer(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_PhotoViewer(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_PhotoListNView(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_PhotoList(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainMenuCamera(SKY_EVENT_T *pEvent);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_CAM_H_ */
