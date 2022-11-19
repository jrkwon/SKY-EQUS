#ifndef NVMIO_H
#define NVMIO_H
/*==========================================================================

         N V   I / O  S E R V I C E S    H E A D E R    F I L E

DESCRIPTION

  This header file contains all the definitions necessary to interface
  with the Non Volatile Memory I/O services.

Copyright (c) 1994,1995,1996,1997,1998,1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/nvmio.h_v   1.1   10 Sep 1999 18:12:16   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/14/99   ny      Cleaned up the compilation to remove warnings/error messges.
                   Applied merge guidelines 3.0 to clean up the code
09/17/96   ls      removed unused double-buffered routine headers
01/23/96   ras     pre-code review cleanup
08/08/95   ras     lint cleanup
05/22/95   ras     Combined item support
05/09/95   ras     Initial next generation nv checkin

===========================================================================*/


/*===========================================================================
 
                         DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                       FUNCTION DECLARATIONS

===========================================================================*/



/*===========================================================================

FUNCTION NVMIO_GET_BASE_ADDR

DESCRIPTION
  This function gets the address of a pointer based item.  The function
  first computes the address of the item pointer within the fixed array,
  and then it reads from NVM the contents of the item pointer within that
  array.

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S if it worked.

SIDE EFFECTS
  None

===========================================================================*/
extern nv_stat_enum_type nvmio_get_base_addr
(
  nvi_items_enum_type item,             /* Item enum */
  word                *item_ptr        /* Item address */
);

/*===========================================================================

FUNCTION NVMIO_FIXED

DESCRIPTION
  Read/Write a fixed NVM pool item.

DEPENDENCIES
  nva_init() should have been called first.

RETURN VALUE
  Status of the I/O operation.

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvmio_fixed
(
  void            *value,           /* Data buffer for I/O operation */
  word            value_size,       /* # bytes of data to be transfered */
  nv_io_enum_type read_write,       /* I/O type */
  word            fixed_position    /* index within foxed pool for item */
);


/*===========================================================================

FUNCTION NVMIO_SINGLE

DESCRIPTION
  Read/Write a dynamic NVM pool, single buffered (active), item.

DEPENDENCIES
  nva_init() must have been called.

RETURN VALUE
  Status of the I/O operation.

SIDE EFFECTS
  None.

===========================================================================*/
extern nv_stat_enum_type nvmio_single
(
  nvi_items_enum_type    item,         /* Item to access */
  void                   *value,       /* buffer for I/O transaction */
  word                   value_size,   /* Number of bytes to transfer */
  nv_io_enum_type        read_write,   /* I/O type */
  word                   sub_index     /* If combined item, offset to item */
);

/*===========================================================================

FUNCTION NVMIO_FIXED_ARRAY

DESCRIPTION
  Read/Write a fixed NVM pool array item.
 
DEPENDENCIES
  nva_init() must have been called.

RETURN VALUE
  Status of I/O operation.

SIDE EFFECTS
  None.

===========================================================================*/
extern nv_stat_enum_type nvmio_fixed_array
(
  nvi_items_enum_type  item,          /* Item to access */
  byte                 index_range,   /* Maximum allowed range for index */
  byte                 index,         /* Item index within its array */
  void                 *value,        /* Data buffer for I/O operation */
  word                 value_size,    /* # of bytes to be transfered */
  nv_io_enum_type      read_write,    /* I/O type */
  word                 fixed_position /* offset of array in fixed pool */
);


/*===========================================================================

FUNCTION NVMIO_SINGLE_ARRAY

DESCRIPTION
  Read/Write a dynamic NVM pool, single buffered (active), array item.

DEPENDENCIES
  nva_init() must have been called.

RETURN VALUE
  Status of I/O operation.

SIDE EFFECTS
  None.

===========================================================================*/
extern nv_stat_enum_type nvmio_single_array
(
  nvi_items_enum_type  item,        /* Item to access */
  byte                 index_range, /* Maximum allowed range for index */
  byte                 index,       /* Item index within its array */
  void                 *value,      /* Data buffer for I/O operation */
  word                 value_size,  /* Number of bytes in I/O transaction */
  nv_io_enum_type      read_write   /* I/O type */
);


/*===========================================================================

FUNCTION NVMIO_IO

DESCRIPTION
  Initiate the correct type of I/O operation for the specified item.

DEPENDENCIES
  nva_init() should have been called.

RETURN VALUE
  Status of I/O operation.

SIDE EFFECTS
  May allocate or free NVM, thus modifying several internal data structures.

===========================================================================*/
extern nv_stat_enum_type nvmio_io
(
  nv_cmd_type  *cmd_ptr,          /* Command block */
  nv_io_enum_type io_type         /* read/write/? */
);
#endif /* NVMIO.H */


