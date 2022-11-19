#ifndef BBSNDTAB_H
#define BBSNDTAB_H
/*===========================================================================

     B O O T   B L O C K   S O U N D   T A B L E (S)   H E A D E R   F I L E

DESCRIPTION
  This file contains the definitions needed for the sound driver to access
  the tables found in BBSNDTAB.C in the Boot Block.

Copyright (c) 1996, 1997, 1998 by QUALCOMM Incorporated. All Rights Reserved.
===========================================================================*/

#include "comdef.h"
#include "sndi.h"

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/MSM5000/VCS/bbsndtab.h_v   1.2   12 Sep 2000 08:57:00   nbouchar  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/12/00   nxb     Removed SND_ROM.
03/04/99    st     Use snd_compact_type instead of snd_type.
12/04/98    sm     Support selectable ROM keyword for backward compatibility.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.  Updated copyright
02/28/96   dna     Created file.

===========================================================================*/

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Phone Ringing (Alrt)*/
extern const snd_compact_type ring_buf[];

/* Wake-up/Power-up snd*/
extern const snd_compact_type pwrup_buf[];                 

/* Dial tone           */
extern const snd_compact_type dial_tone_buf[];             

/* Rngback, audible rng*/
extern const snd_compact_type call_buf[];                  

/* Intercept           */
extern const snd_compact_type int_buf[];                   

/* Intercept           */
extern const snd_compact_type abrv_int_buf[];              

/* Reorder             */
extern const snd_compact_type reord_buf[];                 

/* Abbreviated Reorder */
extern const snd_compact_type abrv_reord_buf[];            

/* Abbrev. Alert (busy)*/
extern const snd_compact_type busy_buf[];                  

/* Confirmation Tone   */
extern const snd_compact_type confirm_tone_buf[];          

/* Answer tone         */
extern const snd_compact_type answer_buf[];

/* Call Waiting        */
extern const snd_compact_type call_waiting_buf[];          

/* Off-Hook Warning    */
extern const snd_compact_type off_hook_buf[];              

/* Normal Alerting     */
extern const snd_compact_type normal_alert_buf[];          

/* Intergroup Alerting */
extern const snd_compact_type intergroup_alert_buf[];      

/* Spcl/Priority Alerti*/
extern const snd_compact_type special_alert_buf[];         

/* "Ping ring"         */
extern const snd_compact_type ping_ring_buf[];             

/* Long                */
extern const snd_compact_type is54b_long_h_alert_buf[];    

/* Short-short         */
extern const snd_compact_type is54b_ss_h_alert_buf[];      

/* Short-short-long    */
extern const snd_compact_type is54b_ssl_h_alert_buf[];     

/* Short-short-2       */
extern const snd_compact_type is54b_ss2_h_alert_buf[];     

/* Short-long-short    */
extern const snd_compact_type is54b_sls_h_alert_buf[];     

/* Short-short-short-sh*/
extern const snd_compact_type is54b_ssss_h_alert_buf[];    

/* PBX Long            */
extern const snd_compact_type is54b_pbx_long_h_alert_buf[];

/* PBX Short-short     */
extern const snd_compact_type is54b_pbx_ss_h_alert_buf[];  

/* PBX Srt-srt-lng exte*/
extern const snd_compact_type is54b_pbx_ssl_h_alert_buf[]; 

/* PBX Short-long-short*/
extern const snd_compact_type is54b_pbx_sls_h_alert_buf[]; 

/* PBX Srt-srt-srt-srt */
extern const snd_compact_type is54b_pbx_ssss_h_alert_buf[];

/* Pip-Pip-Pip-Pip     */
extern const snd_compact_type is53a_pppp_h_alert_buf[];    

/* Long                */
extern const snd_compact_type is54b_long_m_alert_buf[];    

/* Short-short         */
extern const snd_compact_type is54b_ss_m_alert_buf[];      

/* Short-short-long    */
extern const snd_compact_type is54b_ssl_m_alert_buf[];     

/* Short-short-2       */
extern const snd_compact_type is54b_ss2_m_alert_buf[];     

/* Short-long-short    */
extern const snd_compact_type is54b_sls_m_alert_buf[];     

/* Short-short-short-sh*/
extern const snd_compact_type is54b_ssss_m_alert_buf[];    

/* PBX Long            */
extern const snd_compact_type is54b_pbx_long_m_alert_buf[];

/* PBX Short-short     */
extern const snd_compact_type is54b_pbx_ss_m_alert_buf[];  

/* PBX Short-short-long*/
extern const snd_compact_type is54b_pbx_ssl_m_alert_buf[]; 

/* PBX Short-long-short*/
extern const snd_compact_type is54b_pbx_sls_m_alert_buf[]; 

/* PBX srt-srt-srt-srt */     
extern const snd_compact_type is54b_pbx_ssss_m_alert_buf[];

/* Pip-Pip-Pip-Pip     */
extern const snd_compact_type is53a_pppp_m_alert_buf[];    

/* Long                */
extern const snd_compact_type is54b_long_l_alert_buf[];    

/* Short-short         */
extern const snd_compact_type is54b_ss_l_alert_buf[];      

/* Short-short-long    */
extern const snd_compact_type is54b_ssl_l_alert_buf[];     

/* Short-short-2       */
extern const snd_compact_type is54b_ss2_l_alert_buf[];     

/* Short-long-short    */
extern const snd_compact_type is54b_sls_l_alert_buf[];     

/* Short-short-short-sh*/
extern const snd_compact_type is54b_ssss_l_alert_buf[];    

/* PBX Long            */
extern const snd_compact_type is54b_pbx_long_l_alert_buf[];

/* PBX Short-short     */
extern const snd_compact_type is54b_pbx_ss_l_alert_buf[];  

/* PBX Short-short-long*/
extern const snd_compact_type is54b_pbx_ssl_l_alert_buf[]; 

/* PBX Short-long-short*/
extern const snd_compact_type is54b_pbx_sls_l_alert_buf[]; 

/* PBX Short-short-sh-s*/
extern const snd_compact_type is54b_pbx_ssss_l_alert_buf[];

/* Pip-Pip-Pip-Pip     */
extern const snd_compact_type is53a_pppp_l_alert_buf[];    

/* Fade tone           */
extern const snd_compact_type fade_tone_buf[];             

/* Service change tone */
extern const snd_compact_type svc_change_buf[];            

/* Message Waiting Aler*/
extern const snd_compact_type message_waiting_buf[];       

/* Abbreviated Alert   */
extern const snd_compact_type abrv_alert_buf[];            

/* Pip-Pip-Pip-Pip In C*/
extern const snd_compact_type pip_tone_buf[];              

#if (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)
extern const snd_compact_type audio_app_start_sound[];
extern const snd_compact_type audio_app_end_sound[];
#endif //#if (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#endif /* BBSNDTAB_H */
