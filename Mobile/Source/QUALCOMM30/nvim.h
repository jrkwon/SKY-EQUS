#ifndef NVIM_H
#define NVIM_H

/*===========================================================================

   N V   I T E M   M A N A G E R   H E A D E R   F I L E

DESCRIPTION
  All the declarations and definitions necessary to support the table  
  lookup of mapping between an NV item and the Flash File System files
  which simulate storage in an EEPROM.
  
REFERENCES

Copyright (c)  1999,2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/nvim.h_v   1.0   05 Oct 2000 17:12:46   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03-02-00   rp      Added nvim_rebuild_needed() and nvim_update_klt(). 
11-11-99   rp      Add #define NVIM_MAX_NAMS 4 for invariant items.
08-13-99   rp      Initial ASW version.
06-09-99   rp      Cleaned up support for permanent system files.
05-26-99   rp      Added support for permanent system files.
05-20-99   rp      Added nvim_nv_clear(void).
04-30-99   rp      Initial version.
   

===========================================================================*/

/* ======================================================================

                       INCLUDE FILES FOR MODULE

========================================================================*/
#include <stddef.h>
#include "comdef.h"
#include "nv.h"                  /* for nv_item_type */
#include "nvi.h"                 
#include "fs.h"                  /* for fs_ types */

#ifdef __cplusplus
extern "C" {
#endif
 

/* ======================================================================

                       DEFINITIONS AND DECLARATIONS 

========================================================================*/

/* The file containing the ESN and a_keys has a reserved area of FFs */ 
#define PERM_FILE_RESERVED  85     /* Reserved area, all 0xFF */

/* The file containing the factory data has a reserved area of FFs */ 
#define FACTORY_FILE_RESERVED  78     /* Reserved area, all 0xFF */

/* The factory data block has a fixed size */ 
#define NVIM_FACTORY_DATA_SIZE  128

/* Fixed value of dynamic pool area */
//#define NVIM_DYNAMIC_POOL_CAPACITY 4096
#define NVIM_DYNAMIC_POOL_CAPACITY 6144  // huni 10.22 6144(4096 + 2048) change for sms memory shortage 

/* The following defines are simply held over from NVM for convenience */
#define NVIKLTADDR        ((word) 0)

/* Special value to indicate nvm has been built */
#define NVIKLTMAGIC       (((word)NVI_KLT_REV<<8)+NVI_VERNO_MIN)

#define NV_FACTORY_RSVD       0x7F       /* give 127 bytes to factory test */

/* Start of factory reserved test area */
#define NVD_MAX_OFFSET       (0x1FFF - NV_FACTORY_RSVD)  /* 8kb */

/* Numbers of bytes represented by a bit in the allocation map */
#define NVIBLOCKSIZE       ((word)8)
/* Number of blocks in a long allocation block */
#define NVIBLOCKSPERLONGALLOC ((word)4)
/* Offset of data in a long block */
#define NVIDATAOFFSET    ((word)1)  
            
#define NVILONGBLOCK      ((word)(NVIBLOCKSIZE*NVIBLOCKSPERLONGALLOC))
#define NVILONGDATASIZE  ((word)(NVILONGBLOCK - NVIDATAOFFSET))

/* Maximum number of files containing SMS data */
#define NVIM_SMS_FILE_COUNT 44 // 24->44 huni Max Size 40

/* Maximum allowed NAMS, can not modify this value as it affects
   invariant data */
#define NVIM_MAX_NAMS 4

/* File parameters */
typedef struct {
  char *fname;
  fs_attribute_mask_type file_type;
  fs_handle_type rd_handle;   /* Read handle */
  fs_handle_type wr_handle;   /* Write handle */
  int fsize; 
} nvim_efs_params;

typedef int nvim_item_size_type;

/* Item file mapping info */
typedef struct {
  nvim_efs_params *fparams;
  fs_file_position_type fpos;
  nvim_item_size_type item_size;
} nvim_item_location_type;



/*===========================================================================

FUNCTION nvim_fs_init

DESCRIPTION
  This function performs the necessary internal operations to open 
  or create all Flash File System files which simulate storage in an
  EEPROM. 
  
DEPENDENCIES
  FFS support.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
nv_stat_enum_type nvim_fs_init(void);


/*===========================================================================

FUNCTION nvim_fs_term

DESCRIPTION
  This function performs the necessary internal operations to close 
  all Flash File System files which simulate storage in an EEPROM. 
  
DEPENDENCIES
  FFS support.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void nvim_fs_term(void);


/*===========================================================================

FUNCTION nvim_nv_clear

DESCRIPTION
  This function performs the necessary internal operations to zero 
  fill the fixed item related files excluding the ESN. 
  
DEPENDENCIES
  FFS support.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

extern void nvim_nv_clear(void);


/*===========================================================================

FUNCTION nvim_fs_lookup

DESCRIPTION
  When passed an nv item, this function returns the necessary EFS parameters
  in order to access the item in Flash. The nv item serves as an index into 
  a table off all nv EFS parameters.
  
DEPENDENCIES
  None.

RETURN VALUE
  A corresponding table entry.

SIDE EFFECTS
  None.

===========================================================================*/
extern const
nvim_item_location_type *nvim_fs_lookup( nv_items_enum_type item );


/*===========================================================================

FUNCTION nvim_sms_file

DESCRIPTION
  When passed an index, this function returns the EFS filename
  in order to access the item in Flash.   

DEPENDENCIES
  None.

RETURN VALUE
  A corresponding filename.

SIDE EFFECTS
  None.

===========================================================================*/
extern const char* nvim_sms_file( int sms_index );


/*===========================================================================

FUNCTION nvim_sms_data_stored

DESCRIPTION
  Sums the amount of data stored in all SMS files
  
DEPENDENCIES
  None.

RETURN VALUE
  Size of of all SMS files.

SIDE EFFECTS
  None.

===========================================================================*/

word nvim_sms_data_stored(void);


/*===========================================================================

FUNCTION nvim_prl_fs_params

DESCRIPTION
  When passed an index, this function returns the EFS fileh params
  in order to access the roaming list from an EFS file.   

DEPENDENCIES
  None.

RETURN VALUE
  A corresponding nvim_efs_params structure.

SIDE EFFECTS
  None.

===========================================================================*/
extern nvim_efs_params *nvim_prl_fs_params( int index );


/*===========================================================================

FUNCTION nvim_factory_file

DESCRIPTION
  This function returns the EFS filename for factory peek/poke data.

DEPENDENCIES
  None.

RETURN VALUE
  A corresponding filename.

SIDE EFFECTS
  None.

===========================================================================*/

extern const char* nvim_factory_file(void);


/*===========================================================================

FUNCTION nvim_open_write

DESCRIPTION
  Open the givrn file for write acces and save the write handle.

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S or NV_FAIL_S

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvim_open_write
( 
  const nvim_item_location_type *fs_item 
);


/*===========================================================================

FUNCTION nvim_close_write

DESCRIPTION
  Close the file for write access.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvim_close_write
( 
 const nvim_item_location_type *fs_item 
);


/*===========================================================================

FUNCTION nvim_open_write_all

DESCRIPTION
  Open all fixed item files for write acces and save the write handle.

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S or NV_FAIL_S

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvim_open_write_all(void);


/*===========================================================================

FUNCTION nvim_close_write_all

DESCRIPTION
  Close all fixed item files for write access.

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S or NV_FAIL_S

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvim_close_write_all(void);


/*===========================================================================

FUNCTION nvim_update_file_attrs

DESCRIPTION
  Update the file attributes for the given file.

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S or NV_FAIL_S

SIDE EFFECTS
  None.

===========================================================================*/

nv_stat_enum_type nvim_update_file_attrs
( 
   const char *fname,                  /* File name */
   fs_attribute_mask_type attributes   /* New attributes */
);


/*===========================================================================

FUNCTION nvim_update_fs_item_file_attrs

DESCRIPTION
  Update the file attributes on this item's file.

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S or NV_FAIL_S

SIDE EFFECTS
  None.

===========================================================================*/

nv_stat_enum_type nvim_update_fs_item_file_attrs
( 
   const nvim_item_location_type *fs_item, 
   fs_attribute_mask_type attributes          /* New attributes */
);

/*===========================================================================

FUNCTION NVIM_REBUILD_NEEDED

DESCRIPTION
  This function returns TRUE if an NV rebuild is needed, FALSE otherwise.
  A rebuild is needed if any part of the version designator stored in
  NV does not match the corresponding build version designator.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE   - if a rebuild is needed
  FALSE  - if no rebuild is needed

SIDE EFFECTS
  None.

===========================================================================*/

boolean nvim_rebuild_needed(void);
  
/*===========================================================================

FUNCTION nvim_update_klt

DESCRIPTION
  Update items associated with the old nvi_klt_type

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S or NV_FAIL_S

SIDE EFFECTS
  None.

===========================================================================*/
nv_stat_enum_type nvim_update_klt
( 
   byte minor_edit      /* New NV minor version number */
);

#ifdef __cplusplus
}
#endif

#endif  /* ifndef NVIM_H */

