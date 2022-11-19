/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_anim.h
**
** Description:
**      Animation Pictures
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-10-12   redstar       Created.
**
** ===========================================================================
*/

#ifndef _EQC_ANIM_H_
#define _EQC_ANIM_H_

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
/* CONTROL_STYLE : bit flag */
#define CS_ANIMATION_BORDER            0x0001   /* 0000 0000 0000 0001 */
#define CS_ANIMATION_REPEAT            0x0002   /* 0000 0000 0000 0001 */

#define CS_ANIMATION_DEFAULT           EQS_DEFAULT_STYLE


#define MAX_ANIMATION_FRAME            50

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
//typedef strcut tagANIM_CUT  ANIM_CUT;
//typedef ANIM_CUT ANIM_TYPE[];

//typedef struct tagANIMATION_FRAME_T    ANIMATION_FRAME_T;
typedef struct tagANIMATION_HEADER_T   ANIMATION_HEADER_T;
typedef struct tagANIMATION_INFO_T     ANIMATION_INFO_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagANIMATION_HEADER_T {
    UINT8              nFrame;          /* frame count */
    WORD               awDelayTime[MAX_ANIMATION_FRAME];
};

struct tagANIMATION_INFO_T {
    //UINT8              nFrameCount;
    //DWORD              dwDispMilliSecond;   /* display time per frame in ms */
    BITMAP_HEADER      BitmapHeader;
    ANIMATION_HEADER_T AnimationHeader;
    //UINT8              nFrame;          /* frame count */
    //WORD               awDelayTime[MAX_ANIMATION_FRAME];
    CONST BYTE         *pbData;
    //UINT16             nDataSize;
};

#define GET_FRAME_SIZE(xWidth, yHeight, nBPP) \
                                (int)((xWidth)*(yHeight)*((nBPP)/8.0))

#define EQC_GET_FRAME_SIZE(BmpHdr) \
                        GET_FRAME_SIZE((BmpHdr).Size.xWidth, \
                                       (BmpHdr).Size.yHeight, \
                                       (BmpHdr).nBitsPerPixel)

#define EQC_GET_FRAME_DX(BmpHdr) \
                        ((BmpHdr).Size.xWidth)

#define EQC_GET_FRAME_DY(BmpHdr) \
                        ((BmpHdr).Size.yHeight)


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
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
extern HCONTROL EQS_API EQC_Animation(COORD x, COORD y,
                                      ANIMATION_INFO_T CONST *pAnimationInfo,
                                      BYTE* pszPathFileName);
extern void     EQS_API EQC_StartAnimation(HCONTROL hControl, BOOL fRepeat);
extern void     EQS_API EQC_StopAnimation(HCONTROL hControl);
extern void     EQS_API EQC_SetStartYAnimation(HCONTROL hControl, COORD yStart);
extern void     EQS_API EQC_SetAnimationHeight(HCONTROL hControl, COORD yHeight);

#ifdef __cplusplus
}
#endif

#endif /* _EQC_ANIM_H_  */
