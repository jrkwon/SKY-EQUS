/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_aapi.h
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
** 2000-12-27   hjcho      Created.
**
** ============================================================================
*/

#ifndef _SKA_AAPI_H_
#define _SKA_AAPI_H_

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
extern boolean bRecordingTraffic;
extern boolean bPlayingTraffic;

  
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
void	REC_voice_start_traffic(byte fn);
void	REC_voice_stop_traffic(void);
boolean PLAY_voice_start_traffic(void);
void	PLAY_voice_stop_traffic(void);
void    PLAY_voice_stop_traffic_with_noevent(void);


#ifdef FEATURE_HOLDON
boolean PLAY_HO_voice_start_traffic(void);
void	PLAY_HO_voice_stop_traffic(void);
#endif

void REC_packet_traffic(byte *voc_bits, byte voc_rate);
cai_data_rate_type PLAY_packet_traffic(byte *voc_bits);

void ui_set_anstxt(char *txt_ptr, byte answertype);
void ui_set_unable_anstxt(char *txt_ptr);

  
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_AAPI_H_ */


