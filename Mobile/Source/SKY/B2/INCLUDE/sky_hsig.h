/*
** ============================================================================
**
** Project: 
**     BUNNY
**
** File:
**     sky_hsig.h
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
** 2000-10-25 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_HSIG_H_
#define _SKY_HSIG_H_

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

#if (MODEL_ID == MODEL_B1) ||  (MODEL_ID == MODEL_B2)	// audio_en전원 관리 관련 by jnlee 01.04.23
	#define AUDIO_EN_BY_SND		0x01	// yamaha playing
	#define	AUDIO_EN_BY_CAM_CHK	0x02	// jacksense for camera
	#define AUDIO_EN_BY_CAM_WRK	0x04	// power for camera working
	#define AUDIO_EN_BY_VIB		0x08	// power for vibtrator
	#define AUDIO_EN_SUPER_F	0x80	// super flag for preventing power on

	#define	AUDIO_EN_OFF() \
		INTLOCK(); \
		if(fAudioPwrOff==0){ \
			BIO_OUT ( DEC_GPIO_0, BIO_AUDIO_EN_M, BIO_AUDIO_EN_OFF_V);}\
		INTFREE()
	#define AUDIO_EN_ON() \
		INTLOCK(); \
		if(fAudioPwrOff==0){ \
			BIO_OUT ( DEC_GPIO_0, BIO_AUDIO_EN_M, BIO_AUDIO_EN_ON_V);}\
		INTFREE()
#endif //#if MODEL_ID == MODEL_B1


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
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern BOOL g_fPowerDownComplete;
extern BOOL g_fMCInitiatedPowerDown;


#if (MODEL_ID == MODEL_B1) ||  (MODEL_ID == MODEL_B2)// audio_en전원 관리 관련 by jnlee 01.04.23
extern byte fAudioPwrOff;		// sky_hs.c에서 선언 
#endif	//#if MODEL_ID == MODEL_B1


/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API SKY_HandleSignal(rex_sigs_type sigs, q_type *ui_cmd_q_ptr);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_HSIG_H_ */

