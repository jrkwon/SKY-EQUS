#ifndef PRLI_H
#define PRLI_H
/*===========================================================================

 P R E F F E R E D   R O A M I N G   L I S T   I N T E R N A L   H E A D E R

DESCRIPTION
  This file contains internal Preferred Roaming List declarations.

Copyright (c) 1997 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/prli.h_v   1.0   Aug 11 2000 11:44:30   lpetrucc  $

when       who     what, where, why
--------   ---     -------------------------------------------------------
08/04/98   ych     Merged E911, SSPR-800, and Acq. Improvements.
02/24/98   dna     Added support for cellular CDMA using Channels.
01/20/97   jrs     Created module.

===========================================================================*/


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* The cellular analog system acquisition record
 */
typedef struct
{
  prl_ab_type a_b;                      /* System A/B selection type */
} prli_cellular_analog_type;

/* The cellular CDMA system acquisition record
 */
typedef struct
{
  prl_ab_type a_b;                      /* System A/B selection type */
  prl_pri_sec_type pri_sec;             /* Primary/secondary CDMA channel */
} prli_cellular_cdma_type;              /* selection type */

#ifdef FEATURE_IS683A_PRL
typedef struct
{
  int num_chans;                        /* Number of channels */
  word chan[32];                        /* Channels */
} prli_cellular_cdma_custom_type;
#endif

/* The cellular generic system acquisition record
 */
typedef struct
{
  prl_ab_type a_b;                      /* System A/B selection type */
} prli_cellular_generic_type;

/* The PCS CDMA system acquisition (using blocks) record
 */
typedef struct
{
  int num_blocks;                       /* Number of blocks */
  word block[7];                        /* Blocks */
} prli_pcs_cdma_block_type;

/* The PCS CDMA system acquisition (using channels) record
 */
typedef struct
{
  int num_chans;                        /* Number of channels */
  word chan[32];                        /* Channels */
} prli_pcs_cdma_chan_type;

/* This structure defines the acquisition record type
 */
typedef struct
{
  prl_acq_rec_type acq_type;
  union
  {
    prli_cellular_analog_type  cellular_analog;
    prli_cellular_cdma_type    cellular_cdma;
#ifdef FEATURE_IS683A_PRL
    prli_cellular_cdma_custom_type cellular_cdma_cust;
#endif
    prli_cellular_generic_type cellular_generic;
    prli_pcs_cdma_block_type   pcs_cdma_block;
    prli_pcs_cdma_chan_type    pcs_cdma_chan;
  } type;
} prli_acq_rec_type;

/* This structure defines the system record type
 */
typedef struct
{
  word sid;                             /* System identification */
  prl_nid_incl_type nid_incl;           /* NID Included */
  word nid;                             /* Network identification */
  prl_neg_pref_type neg_pref;           /* Negative/preferred system */
  prl_geo_region_type geo;              /* Geographical region indicator */
  prl_pri_type pri;                     /* Relative priority indicator */
  byte acq_index;                       /* Acquisition record index */
  db_roam_type roam_ind;                /* Roaming indicator */
} prli_sys_rec_type;

/* This structure defines a roaming list entry that contains an acquisition
 * record and a system record
 */
typedef struct
{
  prli_sys_rec_type sys_rec;
  prli_acq_rec_type acq_rec;
} prli_roaming_list_entry_type;

#endif /* PRLI_H */

