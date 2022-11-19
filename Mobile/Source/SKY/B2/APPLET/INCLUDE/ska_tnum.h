/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_tnum.h 
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
** 4333-12-15 white     Created.
**
** ============================================================================
*/

#ifndef _SKA_TNUM_H_
#define _SKA_TNUM_H_

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

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tag_TWO_NUMBER_CTRL_E {
    TNC_ADDNUM_LABEL = 0,
    TNC_ADDNUM_EDIT,
    TNC_CHECK_BOX,
    TNC_BELL_LABEL,
//    TNC_BELL_SET_SCRL,
    TNC_BELL_SCRL,
    TNC_MAX
};
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
void SKY_API SKA_MainTwoNum(SKY_EVENT_T *pEvent);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_TNUM_H_ */
