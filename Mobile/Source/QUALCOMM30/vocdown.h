#ifndef VOCDOWN
#define VOCDOWN
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               M S M 2   V O C O D E R   S E R V I C E S
                        H E A D E R  F I L E

DESCRIPTION
  This file contains references for interaction between VOCM2 module and
  VOCDOWN modules.

REFERENCES
  MSM 2.3 Technical Specification
  Fast-Load RAM Interface for the QDSP1+         S.Halter, Sept  4, 1997
  Lynx Changes and Enhancements                            Oct. 20, 1997
  Auxiliary ROM Services SDD                    K.Seltman, Sept 17, 1997

Copyright (c) 1995,1996,1997,1998,1999 by QUALCOMM, Incorporated.  
All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/vocdown.h_v   1.2   15 Aug 2000 15:09:34   rmarkey  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/11/00    sm     AAGC changes:Split 8k/13k image.  Support for "compressed"
                   images.
11/12/98    sm     Included "target.h" before T_ defines are used.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.
08/18/98    ss     Removed the redundant "three booleans" interface from the 
                   function voc_check_download_status().  The routine now uses
                   one enumerated type 'qdsp1p_pcm_clk_init_type' to specify
                   actions to be taken upon download completion.
04/22/98    ss     First version.




===========================================================================*/


/*===========================================================================

                     INCLUDE FILE FOR THIS HEADER FILE

===========================================================================*/
#include "target.h"                    /* Target specific definitions      */

/* DEFINES & MACROS.
 * --------------------------
 */

/* MSM3000'S QDSP1+ Memory structure REQUIRES 8K bytes of data downloaded
**   to successfully activate the vocoder upon reset.  This "known" becomes
**   part of image compression.
*/
#define IMAGE_INS_LENGTH 8192            /* # of bytes in vocoder image */
                        
/* TYPEDEFS, ENUMS & STRUCTS. 
 * --------------------------
 */

typedef struct                 
{
#if defined(T_MSM3)
  const word *ins_data;                /* Ptr. to inst data words in flash. */
#endif /* T_MSM3 */
  word  ins_start_address;             /* 1st Dest. address in IRAM. */
  dword ins_data_length;               /* # of bytes to be written. */   
} qdsp1p_download_data;

typedef enum {                         /* This allows the code that's doing */
  VOC_Q1P_FM_DOWNLOAD = 0,             /* the download to index into our    */  
#ifdef FEATURE_AUDIO_AGC
#error code not present
#else
  VOC_Q1P_CDMA_8K13K_DOWNLOAD,         /* download data just by reference   */    
#endif
  VOC_Q1P_CDMA_EVRC_DOWNLOAD,          /* to a given service option.        */        
#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif
  VOC_Q1P_MAX_DOWNLOADS                  
} qdsp1p_download_type;                  

typedef volatile enum {
  QDSP1P_NOTHING_DOWNLOADED = 0,       /* a) Power-up state only.       */
  QDSP1P_BUSY_DOWNLOADING,             /* b) DMA download active NOW!   */
  QDSP1P_DOWNLOAD_COMPLETED,           /* c) Complete service option is */
  QDSP1P_MAX_STATI                     /*    resident in QDSP1P memory. */
} qdsp1p_download_status_type;         

typedef enum {                         /* Flag feeding into downloader, via */      
  QDSP1P_PCM_CLK_INIT_NOT_NEEDED = 0,  /* call to the function              */    
  QDSP1P_PCM_CLK_INIT_NEEDED,          /* voc_check_download_status().      */
  QDSP1P_MAX_INITS                     /* Indicates whether to schedule PCM */ 
} qdsp1p_pcm_clk_init_type;            /* and Clk initialisation as well,   */
                                       /* if a vocoder acquisition needs to */
                                       /* be postponed because vocoder      */
                                       /* download is in progress.          */

/* GLOBAL VARIABLES. 
 * -----------------
 */
extern qdsp1p_download_status_type     qdsp1p_download_status;
extern qdsp1p_download_type            qdsp1p_resident_image;


/* FUNCTION PROTOTYPES. 
 * --------------------
 */

/*===========================================================================

FUNCTION QDSP1P_DOWNLOAD_IMAGE

DESCRIPTION
  This function downloads a complete service option image onto the 
  QDSP1+ DSP - both coefficient and instruction data.  The download is via
  80186 DMA, as handled by the AUXROM library service.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void qdsp1p_download_image
(
  qdsp1p_download_type service_option,
  boolean              let_clocks_remain
);

/*===========================================================================

FUNCTION VOC_CHECK_DOWNLOAD_STATUS

DESCRIPTION
  This function checks and returns the current download status to a calling 
  routine, which may well want to perform some desired action on the vocoder, 
  such as acquisition.
  
  If the vocoder is downloading, then the "desired action" must be postponed
  by the caller.  However, in this situation, voc_check_download_status() 
  saves the "action flag" parameters (provided by the caller) so that the
  requested carried out upon download completion.
  
DEPENDENCIES
  None.

RETURN VALUE
  qdsp1p_download_status - the current download status of the vocoder.

SIDE EFFECTS
  None.
  
===========================================================================*/
qdsp1p_download_status_type voc_check_download_status 
  ( qdsp1p_pcm_clk_init_type  pcm_clk_init_flag );


/*===========================================================================

FUNCTION VOC_POLL_DOWNLOAD_STATUS

DESCRIPTION
  This function returns the current download status to the calling vocoder
  interrupt service routine.  
  
  In contrast to voc_check_download_status, no interrupts are locked, and no
  actions are scheduled if the vocoder is in fact downloading.
  
  This routine is provided in addition to voc_check_download_status, because
  the interrupt "save & lock" necessary in voc_check_download_status is not
  desirable when the caller is in fact an ISR.
  
DEPENDENCIES
  None.

RETURN VALUE
  qdsp1p_download_status - the current download status of the vocoder.

SIDE EFFECTS
  None.
  
===========================================================================*/
qdsp1p_download_status_type voc_poll_download_status (void);

#endif /* VOCDOWN. */
/* End of File. */
