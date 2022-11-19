#ifndef PRL_H
#define PRL_H

/*===========================================================================

        P R E F F E R E D   R O A M I N G   L I S T   H E A D E R

DESCRIPTION
  This file contains Preferred Roaming List declarations for targets that
  support SD 2.0.

Copyright (c) 1996,1997 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/prl.h_v   1.2   26 Feb 2001 17:43:00   rotemc  $

when       who     what, where, why
--------   ---     -------------------------------------------------------
02/09/01   RC      Created new prl.h from old prl.h and prli.h for SD 2.0.
06/29/00   vn      Added function prototype mcsyspr_multiple_geo() - CR13283
03/16/00   vn      Merged fix for CR7346.
03/09/00   vn      Merged Wildcard SID feature.
02/08/99   ht      Fix Lint warnings.
01/26/99   dna     Change GET_GEO_FOR() to not depend on signed value.  
                   Use bpackw/bunpackw for 9 bit acq_index.
01/26/99   ht      Add #defines for Fast PRL support.
01/25/99   br      Packed Unions for ARM Support.
12/18/98   pms     Moved prl_reformat() prototype from mcsyspr.h to here.
09/01/98   lcc     Merged in ARM support.
08/28/98   ht      Add macros and definitions for Fast PRL Searching
08/06/98   ych     Fixed Lint warnings
08/04/98   ych     Merged E911, SSPR-800, and Acq. Improvements. Also added
                   SSPR CDMA available GAM processing support.
07/22/98   abh     Merged E911, SSPR-800, Acq. Improvements.
02/24/98   dna     New type for pri_sec and new enum for cell CDMA w/chans
11/05/97   abh     Deleted definitions for MAX/MIN Cellular Band CDMA
                   channels
01/20/97   jrs     Added PRL_ prefix to identifiers. Moved internal typedefs
                   to prli.h.
11/13/96   jrs     Updates for cellular analog.
06/24/96   jrs     Created module.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "customer.h" /* Customer configuration file */
#include "comdef.h"   /* Definition for basic types and macros */
#include "db.h"       /* Interface to database services */
#include "nv.h"       /* Interface to NV services */


#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */




/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#ifdef FEATURE_IS683A_PRL

  #define PR_LIST_SIZE_LENGTH    16
  #define PR_LIST_ID_LENGTH      16
  #define PREF_ONLY_LENGTH       1
  #define ROAM_IND_LENGTH        8
  #define ACQ_REC_INDEX_LENGTH   9
  #define SYS_REC_INDEX_LENGTH   14
  #define CRC_LENGTH             16

  #define SYS_REC_SID_LENGTH      15
  #define SYS_REC_NID_INCL_LENGTH 2
  #define SYS_REC_NID_LENGTH      16
  #define SYS_REC_PREF_NEG_LENGTH 1
  #define SYS_REC_GEO_LENGTH      1
  #define SYS_REC_PRI_LENGTH      1

  #define NV_ROAMING_LIST_ITEM    NV_ROAMING_LIST_683_I

  #define MINIMUM_PRL_SIZE  ((sizeof(prl_fixed_roaming_list_type)+CRC_LENGTH)/8)

#else

  #define PREF_ONLY_LENGTH       1
  #define ROAM_IND_LENGTH        3
  #define ACQ_REC_INDEX_LENGTH   6
  #define SYS_REC_INDEX_LENGTH   8

  #define SYS_REC_SID_LENGTH      15
  #define SYS_REC_NID_INCL_LENGTH 1
  #define SYS_REC_NID_LENGTH      16
  #define SYS_REC_PREF_NEG_LENGTH 1
  #define SYS_REC_GEO_LENGTH      1
  #define SYS_REC_PRI_LENGTH      1

  #define NV_ROAMING_LIST_ITEM    NV_ROAMING_LIST_I

#endif


/* Roaming list value for a wildcard SID
 */
#define PRL_WILDCARD_SID 0

/* Roaming list value for a wildcard NID
 */
#define PRL_WILDCARD_NID 65535

/* Roaming List Value for a Public Network NID */
#define PRL_PUBLIC_NETWORK_NID 0


/* This enumerated type lists the types of acquisition records
 */
typedef enum
{
  PRL_ACQ_REC_RESERVED,                 /* Reserved */
  PRL_ACQ_REC_CELLULAR_ANALOG,          /* Cellular analog */
#ifdef FEATURE_IS683A_PRL
  PRL_ACQ_REC_CELLULAR_CDMA_STD,
  PRL_ACQ_REC_CELLULAR_CDMA_CUST,
#else
  PRL_ACQ_REC_CELLULAR_CDMA,            /* Cellular CDMA */
#endif
  PRL_ACQ_REC_CELLULAR_GENERIC,         /* Cellular generic */
  PRL_ACQ_REC_PCS_CDMA_BLOCKS,          /* PCS CDMA using blocks */
  PRL_ACQ_REC_PCS_CDMA_CHANNELS,        /* PCS CDMA using channels */
  PRL_ACQ_REC_NONE
} prl_acq_rec_type;

/* This enumerated type defines the values that the neg_pref field can
 * contain
 */
typedef enum
{
  PRL_NEGATIVE,               /* A Negative system record */
  PRL_PREFERRED               /* A Preferred system record */
} prl_neg_pref_type;

/* This enumerated type defines the values that the geo region field can
 * contain
 */
typedef enum
{
  PRL_NEW_GEO_REGION = 0,     /* Delineates a new geo region */
  PRL_SAME_GEO_REGION         /* This entry is in the same geo region as */
                              /* the previous entry */
} prl_geo_region_type;

/* This enumerated type defines the values that the pri field can contain
 */
typedef enum
{
  PRL_PRI_SAME = 0,           /* This entry is the same priority as the */
                              /* next entry */
  PRL_PRI_HIGHER              /* This entry is  higher priority than the */
                              /* next entry */
} prl_pri_type;


/* This enumerated type defines the values that the a_b field can contain
 */
typedef enum
{
  PRL_AB_SYSTEM_A,            /* System A */
  PRL_AB_SYSTEM_B,            /* System B */
  PRL_AB_RESERVED,            /* Reserved */
  PRL_AB_SYSTEM_A_OR_B        /* System A or system B */
} prl_ab_type;

/* This enumerated type defines the values that the pri_sec field can contain
 */
#ifdef FEATURE_IS683A_PRL

typedef enum
{
  PRL_PRISEC_RESERVED=0,           /* Reserved */
  PRL_PRISEC_PRIMARY=1,            /* Primary Channel Only */
  PRL_PRISEC_SECONDARY=2,          /* Secondary Channel Only */
  PRL_PRISEC_PRI_OR_SEC=3          /* Primary or Secondary Channel */
} prl_pri_sec_type;

#else

typedef enum
{
  PRL_PRISEC_PRIMARY,            /* Primary Channel Only */
  PRL_PRISEC_SECONDARY,          /* Secondary Channel Only */
  PRL_PRISEC_RESERVED,           /* Reserved */
  PRL_PRISEC_PRI_OR_SEC          /* Primary or Secondary Channel */
} prl_pri_sec_type;

#endif


/* This enumerated type defines the values the NID_INCL field
   in the System Record can assume 
 */
typedef enum
{
  PRL_NID_NOT_INCL_WILDCARD_NID=0   /* NID Not included in Sys Record */
  , PRL_NID_INCL=1                  /* NID Included in Sys Record */
#ifdef FEATURE_IS683A_PRL
  , PRL_NID_NOT_INCL_PUBLIC_NID=2   /* NID Not included in Sys Record */
  , PRL_NID_INCL_RESERVED=3         /* Reserved */
#endif
} prl_nid_incl_type;

/* The valid blocks that can be defined in a roaming list
 */
#define PRL_BLOCK_A         0
#define PRL_BLOCK_B         1
#define PRL_BLOCK_C         2
#define PRL_BLOCK_D         3
#define PRL_BLOCK_E         4
#define PRL_BLOCK_F         5
#define PRL_BLOCK_RESERVED  6
#define PRL_BLOCK_WILDCARD  7

/* The lowest and highest CDMA PCS channel numbers
 */
#define PRL_LOWEST_PCS_CHANNEL  25
#define PRL_HIGHEST_PCS_CHANNEL 1175

#ifdef FEATURE_WCARD_SID_MGMT
typedef enum
{
   CELLULAR_ANALOG,
   CELLULAR_CDMA,
   PCS_CDMA,
   MCSYSPR_BAND_MODE_MAX
}mcsyspr_band_mode_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Preferred Roaming List External Declarations
 */

/* Acquisition type header
 */
typedef PACKED struct
{
  byte acq_rec[4];                      /* Acquisition type */
} prl_acq_rec_header_type;

/* The cellular analog system acquisition record
 */
typedef PACKED struct
{
  byte a_b[2];                          /* System A/B selection */
} prl_cellular_analog_type;

/* The cellular CDMA system acquisition record
 */
typedef PACKED struct
{
  byte a_b[2];                          /* System A/B selection */
  byte pri_sec[2];                      /* Primary/secondary CDMA channel */
} prl_cellular_cdma_type;               /* selection                      */

#ifdef FEATURE_IS683A_PRL
typedef PACKED struct
{
 byte num_chans[5];
 byte chan[11];    
} prl_cellular_cdma_custom_type;
#endif /* FEATURE_IS683A_PRL */

/* The cellular generic system acquisition record
 */
typedef PACKED struct
{
  byte a_b[2];                          /* System A/B selection */
} prl_cellular_generic_type;

/* The PCS CDMA system acquisition (using blocks) record
 */
typedef PACKED struct
{
  byte num_blocks[3];                   /* Number of frequency blocks */
  byte block[3];                        /* PCS frequency block */
} prl_pcs_cdma_using_blocks_type;

/* The PCS CDMA system acquisition (using channels) record
 */
typedef PACKED struct
{
  byte num_chans[5];                    /* Number of channels */
  byte chan[11];                        /* PCS CDMA channel */
} prl_pcs_cdma_using_chans_type;

/* The acquisition record format
 */
typedef PACKED struct
{
  prl_acq_rec_header_type             acq_rec_header;
  PACKED union
  {
    prl_cellular_analog_type          cellular_analog;
    prl_cellular_cdma_type            cellular_cdma;
    prl_cellular_generic_type         cellular_generic;
    prl_pcs_cdma_using_blocks_type    pcs_cdma_using_blocks;
    prl_pcs_cdma_using_chans_type     pcs_cdma_using_channels;
  } acq_rec;
} prl_acq_record_type;

/* The system record format
 */
typedef PACKED struct
{
  byte sid[SYS_REC_SID_LENGTH];           /* System identification */
  byte nid_incl[SYS_REC_NID_INCL_LENGTH]; /* NID included */
  byte nid[SYS_REC_NID_LENGTH];           /* Network identification */
  byte neg_pref[SYS_REC_PREF_NEG_LENGTH]; /* Negative/preferred system */
  byte geo[SYS_REC_GEO_LENGTH];           /* Geographical region indicator */
  byte pri[SYS_REC_PRI_LENGTH];           /* Relative priority indicator */
  byte acq_index[ACQ_REC_INDEX_LENGTH];   /* Acquisition record index */
  byte roam_ind[ROAM_IND_LENGTH];         /* Roaming indicator */
} prl_sys_record_type;

/* The fixed portion of the preferred roaming list
 */
typedef PACKED struct
{
#ifdef FEATURE_IS683A_PRL
  byte pr_list_size[PR_LIST_SIZE_LENGTH];   /* PR_LIST_SIZE */
  byte pr_list_id[PR_LIST_ID_LENGTH];       /* PR_LIST_ID */
#endif
  byte pref_only[PREF_ONLY_LENGTH];         /* PREF_ONLY */
  byte def_roam_ind[ROAM_IND_LENGTH];       /* DEF_ROAM_IND */
  byte num_acq_recs[ACQ_REC_INDEX_LENGTH];  /* NUM_ACQ_RECS */
  byte num_sys_recs[SYS_REC_INDEX_LENGTH];  /* NUM_SYS_RECS */
} prl_fixed_roaming_list_type;

/*
 * Macro to retrieve info from reformatted PRL
 */
/* To get GEO bit info */
extern word * mcsyspr_geo_sid_startp;

#define MCSYSPR_NEW_GEO_SID      ((word) 0x8000)

#define MCSYSPR_GET_GEO_FOR(rl_entry)                               \
      ( (((*(mcsyspr_geo_sid_startp + (rl_entry)))                   \
        & MCSYSPR_NEW_GEO_SID) == 0) ?                       \
        PRL_SAME_GEO_REGION : PRL_NEW_GEO_REGION)

/* To get SID info */
#define MCSYSPR_GET_SID_FOR(rl_entry)                               \
        ((int) (*(mcsyspr_geo_sid_startp + (rl_entry)) & 0x7fff))

/* To get ACQ_INDEX info */
#define MCSYSPR_GET_ACQ_INDEX_FOR(rl_entry)                         \
    (b_unpackw((byte*)mcsyspr_acq_index_startp,                             \
              (((word) rl_entry) * FSIZ(prl_sys_record_type, acq_index)), \
              FSIZ(prl_sys_record_type, acq_index)))             

/* To get NI_PR (NID_INCLUDED bit 1 and PREF_NEG bit) info */
extern byte * mcsyspr_ni_pr_startp;
#define MCSYSPR_GET_NI_PR_FOR(index)                                \
          ((*((mcsyspr_ni_pr_startp) + ((index) / 4)) >>            \
           ((3-((index) & MCSYSPR_NI_PR_BYTE_MASK))*2))             \
            & MCSYSPR_NI_PR_BYTE_MASK)

/* To get NID_INCLUDED bit 2 info */
extern byte * mcsyspr_ni2bit_startp;
#define MCSYSPR_GET_NID2_FOR(index)                                 \
          ((*((mcsyspr_ni2bit_startp) + ((index) / 8)) >>           \
           (7 - ((index) % 8))) & 0x1)

/* To get PREF_NEG bit info */
#define MCSYSPR_GET_PREF_FOR(index)                                 \
    ((MCSYSPR_GET_NI_PR_FOR(index)) & 0x1)

/* NID_INCLUDED(2 bits) and PREF_NEG grouped definitions */
typedef enum 
{
  NI00_PR0_TYPE = 0,       /* NID not included, NID is 65535, negative */
  NI00_PR1_TYPE,           /* NID not included, NID is 65535, preferred */
  NI01_PR0_TYPE,           /* NID is included, negative */
  NI01_PR1_TYPE,           /* NID is included, preferred */
  NI10_PR0_TYPE,           /* NID not included, NID is 0, negative */
  NI10_PR1_TYPE,           /* NID not included, NID is 0, preferred */
  NI11_PR0_TYPE,           /* reserved */
  NI11_PR1_TYPE,           /* reserved */
  NI_PR_MAX_TYPE
} mcsyspr_ni_pr_type;

#ifdef FEATURE_IS683A_PRL
/*
 * The following definitions are the miscellaneous field length of
 * of each system record based on the NID_INCLUDED(bit 1) and PREF_NEG
 * values
 */
#define NI0_PR0                       \
         (                            \
           0                          \
         )

#define NI0_PR1                       \
         (                            \
           SYS_REC_PRI_LENGTH +       \
           ROAM_IND_LENGTH            \
         )

#define NI1_PR0                       \
         (                            \
           SYS_REC_NID_LENGTH         \
         )

#define NI1_PR1                       \
         (                            \
           SYS_REC_NID_LENGTH +       \
           SYS_REC_PRI_LENGTH +       \
           ROAM_IND_LENGTH            \
         )
#else
#define NI0_PR0                       \
         (                            \
           SYS_REC_PRI_LENGTH         \
         )

#define NI0_PR1                       \
         (                            \
           SYS_REC_PRI_LENGTH +       \
           ROAM_IND_LENGTH            \
         )

#define NI1_PR0                       \
         (                            \
           SYS_REC_NID_LENGTH +       \
           SYS_REC_PRI_LENGTH         \
         )

#define NI1_PR1                       \
         (                            \
           SYS_REC_NID_LENGTH +       \
           SYS_REC_PRI_LENGTH +       \
           ROAM_IND_LENGTH            \
         )
#endif


#define MCSYSPR_INVALID_ROAM     ((1 << ROAM_IND_LENGTH) - 1)

#define MCSYSPR_NI_PR_BYTE_MASK      0x3
#define MCSYSPR_NI_PR_BYTE_SHIFT       2
#define MCSYSPR_NI_PR_PAIRS_IN_BYTE    4
#define MCSYSPR_BITS_IN_BYTE           8
#define MCSYSPR_NI2BIT_TRUE            1
#define MCSYSPR_NI2BIT_IN_BYTE         8

#define MCSYSPR_NEW_GEO_BYTE_MASK    0x7
#define MCSYSPR_NEW_GEO_BYTE_SHIFT     3

#define MCSYSPR_BITS_IN_NI_PR          2


#define MCSYSPR_NID_NOT_INCLUDED       0
#define MCSYSPR_NID_INCLUDED           1

/* NID_INCLUDED bits position */
#define MCSYSPR_NIBIT1               0x1 /* bit determine whether explicit
                                          * NID is included
                                          */
#define MCSYSPR_NIBIT2               0x2 /* bit determines whether NID is
                                          * wildcard or public network
                                          */

#define PRL_RECORD_READ  128  /* This number has to be a multiple of 16,
                               * Preferred maximum is 128
                               */

#define PRL_SYS_REC_IN_BUF    PRL_RECORD_READ

#define NI_PR_BUF_SZ          (PRL_SYS_REC_IN_BUF / 4)

#define NI2BIT_BUF_SZ         (PRL_SYS_REC_IN_BUF / 8) 

#define ACQ_BUF_SZ    (ACQ_REC_INDEX_LENGTH * PRL_SYS_REC_IN_BUF / 8)



/*===========================================================================

FUNCTION MCSYSPR_SID_IS_MG

DESCRIPTION
  This function uses the SID value to search the mcsyspr_multi_geo_sid
  data structure to determine if the SID is classified
  as a multi_geo_region SID in the PRL.

  To qualify, the SID must be listed in multiple geographic regions

  A SID is listed only in one geographic region but multiple times
  is not qualified.

DEPENDENCIES
  None.

RETURN VALUE

  MCSYSPR_SID_IS_NOT_MG - The SID is not a multi_geo_region SID
  Otherwise - The index to the mcsyspr_multi_geo_sid.sid array

SIDE EFFECTS
  None.

===========================================================================*/

int mcsyspr_sid_is_mg
(
  word  sid             /* system ID */
);


/*===========================================================================

FUNCTION MCSYSPR_MG_MATCH_RL_ENTRY


DESCRIPTION
  This function attempts to match the acquired system to a roaming list entry.
  The acquired system is compared with each system record in the system table.
  For system records with an explicit SID, a match is declared if the acquired
  SID, NID, mode and band class match the system record and the following
  conditions is met:

  1. During Alt scan, Periodic Re-scan or CDMA availabe gam
     A. If target system and serving system is overlapped in geo region,
        the target system is a match if it is a more preferred system in
        that region(i.e. in higher tiers).  Otherwise, it is no match.
     B. If target system and serving system is not overlapped in any
        geo region, the target system is a match if it is more preferred
        system.
  2. During all other scans, the most preferred system among all possible
     choices is matched

DEPENDENCIES
  None.

RETURN VALUE
  If a match is found, the index into the roaming list is returned. If no
  match is found, MCSYSPR_NO_RL_ENTRY is returned.

SIDE EFFECTS
  None.

===========================================================================*/

int mcsyspr_mg_match_rl_entry
(
  byte mode,                  /* The mode of the acquired system */
  word band_class,            /* The band class of the acquired system */
  nv_cs_union_type cs,        /*lint -esym(715,cs) Supress "No Reference" */
                              /* The acquired CDMA channel or analog system */
  word sid,                   /* The acquired SID */
  word nid,                   /* The acquired NID */
  int  *match_rlep,           /* the index to mcsyspr_multi_geo_sid.sid */
  boolean check_acceptable    /* check the acceptability of system only */
);

#ifdef FEATURE_FASTPRL
/*===========================================================================

FUNCTION PRL_REFORMAT

DESCRIPTION
  This re-formats the System Table of a PRL.  The fixed length
  fields are separated and stored in a separate storage area with
  a pointer to the start of that area.

  1. GEO bit(1 bit) and SID(15 bits) are combined and stored in 
     int format.  GEO bit is the MSB.  Pointer is mcsyspr_geo_sid_startp.

  2. NID_INCLUDED(bit 1 - the bit that determines if NID is explicitly
     included) and PREF_NEG bit are stored in a 2 bit format for each 
     record. There can be four records packed in a char.

     The following is a layout of a char:

          MSB                                          LSB
        -----------------------------------------------------
        | NI1 | PREF | NI1 | PREF | NI1 | PREF | NI1 | PREF |
        -----------------------------------------------------
        \____________/                         \____________/
              |                                      |-> (n+3)th entry
              |                                          in system table
              |
              |-> nth entry in system table

      Pointer is mcsyspr_ni_pr_startp.

   3. The ACQ_INDEX is stored in a bit packed format in an area pointed
      by mcsyspr_acq_index_startp.

#ifdef FEATURE_IS683A_PRL
   4. The NID_INCLUDED(bit 2 - the bit that determines if the implicit
      NID is a wildcard or a public network NID), or NI2 bit is stored
      in byte format.  Each byte has 8 bits.  Each bit stores one NI2 bit
      of a system table entry.  Therefore, each byte can store the NI2 bit
      of 8 system table entries.  The storing sequence is from the MSB to
      LSB.
   
      The following is a byte layout:

         MSB                         LSB
        ---------------------------------
        |   |   |   |   |   |   |   |   |
        ---------------------------------
          |                           |
          |                           |-> (n+7)th entry in system table
          |
          |-> nth entry in system table
     
      Pointer to this area is mcsyspr_ni2bit_startp.
#endif

    The remaining fields are stored in an MISC area at the beginning
    of the system table.

DEPENDENCIES
  None.

RETURN VALUE
  NONE

SIDE EFFECTS
  A prl_reformatted flag is set.

===========================================================================*/
boolean prl_reformat(void);

#endif /* FEATURE_FASTPRL */

#ifdef FEATURE_WCARD_SID_MGMT
/*===========================================================================

FUNCTION MCSYSPR_WSID_MORE_DESIRABLE_SID

DESCRIPTION
  This function determines if there is a more desirable SID specific
  system in the same geo-region as the rl_entry is. A more desirable
  system has a higher PRIORITY and neg_pref is PREFERRED.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if there is a more desirable SID specific system.
  FALSE otherwise.
   
SIDE EFFECTS
  None.

===========================================================================*/
boolean mcsyspr_wsid_more_desirable_sid(int rle);


/*===========================================================================

FUNCTION MCSYSPR_INIT_WCARD_SID

DESCRIPTION

  This initialize the mcsyspr_wcard_sid structure based on data 
  from the Preferred Roaming List. It groups all wildcard SID entries
  that have the same band and mode in PRL.
  To save runtime processing, Acquisition type, system table index 
  and celluler systems for each of the wildcard SIDs is also listed in
  mcsyspr_init_wsid.
 
  Any wildcard SID in PRL will be collected in
  the mcsyspr_wcard_sid structure.

DEPENDENCIES
  None.

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
void mcsyspr_init_wcard_sid(void);


/*===========================================================================

FUNCTION MCSYSPR_WSID_MATCH_RL_ENTRY

DESCRIPTION
  This function attempts to match the acquired wildcard SID system to a roaming
  list entry.
  The acquired system is compared with each system record in the system table.
  to look for a match with a wildcard SID entry. Match is declared if the 
  acquired NID, mode, band class, and CDMA channel or analog system match.
  If in ALTERNATE_SCAN or RESCAN, the acquired target system is compared with
  the serving system. If target system is more desirable than serving system
  in all overlapping geo-regions, the  target roaming list index is returned.

DEPENDENCIES
  None.

RETURN VALUE
  If a match is found, the index into the roaming list is returned. If no
  match is found, MCSYSPR_NO_RL_ENTRY is returned.

SIDE EFFECTS
  None.

===========================================================================*/
boolean mcsyspr_wsid_match_rl_entry
(
  byte mode,                  /* The mode of the acquired system */
  word band_class,            /* The band class of the acquired system */
  nv_cs_union_type cs,        /*lint -esym(715,cs) Supress "No Reference" */
                              /* The acquired CDMA channel or analog system */
  word nid,                   /* The acquired NID */
  int  *match_rlep,           /* the band_mode type of matched system that can
                               * be used as an index to mcsyspr_wcard_sid */
  boolean check_acceptable    /* check the acceptability of the system only */
);


/*===========================================================================

FUNCTION MCSYSPR_WSID_MORE_DESIRABLE_SVC

DESCRIPTION
  This function determines if there is more desirable service within all
  geographical regions that a multi/single geo region Wildcard SID belongs.

  For all such geographical regions, it builds the more desirable
  systems into the Alternate Scan List.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if there is more desirable service.
  FALSE if the acquired system is the most desirable within any geo region.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcsyspr_wsid_more_desirable_svc
(
  mcsyspr_band_mode_type band_mode,  /*lint -esym(715,mode) Supress "No Reference" */
                              /* The mode of the acquired system */
                              /* The band class of the acquired system */
  int rl_entry
);

/*===========================================================================

FUNCTION MCSYSPR_MULTIPLE_GEO

DESCRIPTION
  This function checks if there are multiple geo-regions in the PRL.
  
DEPENDENCIES
  None.

RETURN VALUE
  Boolean: TRUE => More than one geo-regions in PRL
           FALSE => Only one geo-region in PRL

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcsyspr_multiple_geo
(
  void     
);

#endif



#endif /* ! FEATURE_SD20 */

#endif /* PRL_H */

