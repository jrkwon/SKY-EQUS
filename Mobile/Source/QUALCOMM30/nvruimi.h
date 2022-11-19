#ifndef NVRUIMI_H
#define NVRUIMI_H
/*==========================================================================

         N V    R - U I M    I N T E R N A L    H E A D E R    F I L E

DESCRIPTION
  This header file contains all the internal interface definitions for
  the R-UIM subsystem of the NV task.

REFERENCES
  PN-4690, Removable User Identity Module (RUIM) for Spread Spectrum Systems

Copyright (c) 2000 by QUALCOMM 
Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/nvruimi.h_v   1.3   19 Jan 2001 17:37:28   tsummers  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/11/01   ts      Added nvruim_flush_cache to be accessed when the RUIM is 
                   reset.
11/15/00   ck      Added the enum definitions for esn usage indicator.

04/24/00   jah     Initial Revision

===========================================================================*/


/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "nv.h"


/*===========================================================================
 
                           DATA DECLARATIONS

===========================================================================*/

/*=========================================================================*/
/* Status as to whether or not a particular NV item is supported by the    */
/* R-UIM subsystem.                                                        */

typedef enum {
  NV_RUIM_ITEM_NOT_SUPPORTED=0,         /* item is supported by R-UIM      */
  NV_RUIM_SUPPORTS_ITEM=1               /* item is not supported by R-UIM  */
} nv_ruim_support_status;

typedef enum {
  NV_RUIM_USE_ESN = 0,                  /* Use ESN as ESN */
  NV_RUIM_USE_RUIM_ID = 1               /* Use R-UIM ID as ESN */
} nvruim_esn_usage_type;

/*=========================================================================*/
/* IMSI EF structure for IMSI_M and IMSI_T accesses                        */

typedef PACKED struct {
  byte imsi_class;
  byte imsi_s2[2];
  byte imsi_s1[3];
  byte imsi_11_12;
  byte imsi_addr_num;
  byte imsi_mcc[2];
} nvruim_imsi_type;

/* Mask of bit indicating the IMSI was programmed */

#define NVRUIM_IMSI_PROGRAMMED  0x80

/* Mask of bits for the IMSI Addr Num */

#define NVRUIM_IMSI_ADDR_NUM    0x07

/*=========================================================================*/
/* R-UIM data sizes which do not match NV data types                       */

#define NVRUIM_MIN1_SIZE        3       /* MIN1, True IMSI S1              */
#define NVRUIM_MIN2_SIZE        2       /* MIN2, True IMSI S2              */

#define NVRUIM_MSISDN_DN_SIZE   8       /* MSISDN MDN size                 */

#define NVRUIM_SMSS_SIZE        5       /* SMS Status                      */

#define NVRUIM_MSISDN_SIZE      11      /* MSISDN total size               */

#define NVRUIM_MSISDN_ND_INDEX   0      /* Number of digits byte offset from 
                                           beginning  of MSISDN    */

#define NVRUIM_MSISDN_MDN_INDEX  1      /* MDN byte offset from beginning of 
                                           MSISDN    */


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/



/*===========================================================================

FUNCTION NVRUIM_WRITE

DESCRIPTION
  Check to see if the item is supported by the R-UIM.
  If it is supported, write it.

DEPENDENCIES
  None

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status indicates the success/failure of the write.  For items
  which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  None

===========================================================================*/
extern nv_ruim_support_status nvruim_write
(
  nv_cmd_type  *nv_cmd_ptr,       /* command block               */
  nv_stat_enum_type *op_status    /* status of the I/O operation */
);


/*===========================================================================

FUNCTION NVRUIM_READ

DESCRIPTION
  Check to see if the item is supported by the R-UIM.
  If it is supported, read it.

DEPENDENCIES
  None

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status indicates the success/failure of the read.  For items
  which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  None

===========================================================================*/
extern nv_ruim_support_status nvruim_read
(
  nv_cmd_type  *nv_cmd_ptr,       /* command block               */
  nv_stat_enum_type *op_status    /* status of the I/O operation */
);


/*===========================================================================

FUNCTION NVRUIM_FREE

DESCRIPTION
  Check to see if the item is supported by the R-UIM.
  If it is supported, free it.

DEPENDENCIES
  Non-reentrant

RETURN VALUE
  NV_RUIM_ITEM_NOT_SUPPORTED    Item is not supported
  NV_RUIM_SUPPORTS_ITEM         Item is supported, see op_status

  op_status indicates the success/failure of the free.  For items
  which are "not supported", the value of op_status is undefined.

SIDE EFFECTS
  None

===========================================================================*/
extern nv_ruim_support_status nvruim_free
(
  nv_cmd_type  *nv_cmd_ptr,       /* command block               */
  nv_stat_enum_type *op_status    /* status of the I/O operation */
);


/*===========================================================================

FUNCTION NVRUIM_ACCESS                                  INTERNAL FUNCTION

DESCRIPTION
  Access the R-UIM

DEPENDENCIES
  Non-reentrant, only works for NAM=0

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern ruim_rpt_status nvruim_access
(
  auth_cmd_type *cmd_ptr       /* R-UIM command                               */
);


/*===========================================================================

FUNCTION NVRUIM_SET_ESN_USAGE

DESCRIPTION
   This function sets the usage indicator that is used by the NV-RUIM module
   to decide whether ESN or R-UIM ID is to be returned for an ESN read .

DEPENDENCIES
  Non-reentrant

RETURN VALUE
  None.
                                               
SIDE EFFECTS
  Updates the usage indicator variable nvruim_esn_usage.

===========================================================================*/
extern void nvruim_set_esn_usage
(
   nvruim_esn_usage_type esn_usage
);



/*===========================================================================

FUNCTION NVRUIM_FLUSH_CACHE                                 EXTERNAL FUNCTION

DESCRIPTION
  This function clears the cache control so that all items are read from the 
  RUIM the next time they are requested.

DEPENDENCIES
  This function affects the cache control variable and therefore affects the 
  operation of the read of cached NV items from the RUIM.

RETURN VALUE
  None

SIDE EFFECTS
  This function affects the operation of the cache algorithm.

===========================================================================*/
extern void nvruim_flush_cache
(
  void 
);

#endif /* NVRUIMI_H */
