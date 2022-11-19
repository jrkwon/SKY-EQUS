
#ifndef DIAGX_H
#define DIAGX_H

/*==========================================================================

                    D I A G N O S T I C    T A S K   
    
               E X T E N S I O N S   H E A D E R    F I L E

DESCRIPTION
  Header file for diagx.c
 
Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

 $Header:   L:/src/asw/MSM5000/VCS/diagx.h_v   1.2   22 May 2000 19:49:46   ldefauw  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/22/00   lad     Increased max property size.
02/28/00   lad     Changed functions to use diagbuf allocation services.
01/20/00    gr     Created.

===========================================================================*/

/* This file only used if FEATURE_DIAG_QCT_EXT is on. */
#ifdef FEATURE_DIAG_QCT_EXT


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"


/* <EJECT>^L */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*
** The maximum number of properties and callback functions. These are not
** used to determine the size of any data structure; they are used merely
** to guard against infinite loops caused by corruption of the callback
** and properties tables.
*/
#define DIAG_MAX_NUM_PROPS 20
#define DIAG_MAX_NUM_FUNCS 20

typedef void (*diag_cb_func_type) (
                                   unsigned char  *data_ptr,
                                   unsigned short  data_len,
                                   unsigned char  *rsp_ptr,
                                   unsigned short *rsp_len_ptr
);

typedef struct
{
   char             *name;
   diag_cb_func_type address;
} diag_cb_struct_type;

typedef struct
{
   char *name;
   void *address;
} diag_prop_struct_type;

typedef enum
{
  DIAG_EXTN_INVALID_GUID = 1,
  DIAG_EXTN_INVALID_SIZE,
  DIAG_EXTN_INVALID_ADDRESS,
  DIAG_EXTN_INVALID_NAME,
  DIAG_EXTN_INVALID_DATA
} diag_extn_err_type;

/* <EJECT>^L */
/*===========================================================================

                          PACKET DEFINITONS

===========================================================================*/

/*===========================================================================

PACKET   diag_get_property_req_type

ID       DIAG_GET_PROPERTY_F

PURPOSE  Sent by the DM to retrieve a specified number of bytes from
         memory starting at a specified location.

RESPONSE DMSS retrieves data from memory and sends a
         diag_get_property_rsp_type packet

===========================================================================*/
typedef PACKED unsigned long diag_guid_type[ 4 ];

typedef PACKED struct
{
  byte           cmd_code;                 /* Command code                */
  diag_guid_type guid;                     /* GUID for verification       */
  dword          address;                  /* Starting address in memory  */
  word           size;                     /* Number of bytes to retrieve */
} diag_get_property_req_type;


/*===========================================================================

PACKET   diag_put_property_req_type

ID       DIAG_PUT_PROPERTY_F

PURPOSE  Sent by the DM to set the values of a specified number of bytes
         in memory starting at a specified location.

RESPONSE DMSS sets the specified bytes to the specified values and sends a
         diag_put_property_rsp_type packet

===========================================================================*/
#define DIAG_MAX_PROPERTY_SIZE 800
typedef PACKED struct
{
  byte           cmd_code;               /* Command code               */
  diag_guid_type guid;                   /* GUID for verification      */
  dword          address;                /* Starting address in memory */
  word           size;                   /* Number of bytes to set     */
  byte           data[ DIAG_MAX_PROPERTY_SIZE ]; /* Values             */
} diag_put_property_req_type;


/*===========================================================================

PACKET   diag_get_guid_req_type

ID       DIAG_GET_GUID_F

PURPOSE  Sent by the DM to retrieve the GUID (globally unique identifier)
         for the current build. This is stored during the build process.

RESPONSE DMSS retrieves the GUID and sends a diag_get_guid_rsp_type packet

===========================================================================*/
typedef PACKED struct
{
  byte  cmd_code;                     /* Command code                */
} diag_get_guid_req_type;


/*===========================================================================

PACKET   diag_user_cmd_req_type

ID       DIAG_USER_CMD_F

PURPOSE  Sent by the DM to call a user-specified callback function.

RESPONSE DMSS invokes the callback and sends a diag_user_cmd_rsp_type packet
         if the callback function returns a non-null response. If the
         callback function's response has zero length, then no response
         is sent to the DM.

===========================================================================*/
#define DIAG_MAX_USER_PACKET_SIZE 200
typedef PACKED struct
{
  byte              cmd_code;             /* Command code                  */
  diag_guid_type    guid;                 /* GUID for verification         */
  diag_cb_func_type address;              /* Address of callback function  */ 
  word              length;               /* Length of data that follows   */
  byte              data[ DIAG_MAX_USER_PACKET_SIZE ]; /* Data for callback*/
} diag_user_cmd_req_type;


/*===========================================================================

PACKET   diag_get_perm_property_req_type

ID       DIAG_GET_PERM_PROPERTY_F

PURPOSE  Sent by the DM to retrieve the contents of a structure specified
         by name.

RESPONSE DMSS retrieves data and sends a diag_get_perm_property_rsp_type
         packet

===========================================================================*/
#define DIAG_MAX_PROPERTY_NAME_SIZE 40

typedef PACKED struct
{
  byte           cmd_code;                 /* Command code                */
  diag_guid_type guid;                     /* GUID for verification       */
  byte           name[ DIAG_MAX_PROPERTY_NAME_SIZE ]; /* Structure name   */
  word           size;                     /* Number of bytes to retrieve */
} diag_get_perm_property_req_type;


/*===========================================================================

PACKET   diag_put_perm_property_req_type

ID       DIAG_PUT_PERM_PROPERTY_F

PURPOSE  Sent by the DM to fill in a structure specified by name.

RESPONSE DMSS fills in the structure using the specified bytes and sends a
         diag_put_perm_property_rsp_type packet

===========================================================================*/
typedef PACKED struct
{
  byte           cmd_code;               /* Command code                  */
  diag_guid_type guid;                   /* GUID for verification         */
  byte           name[ DIAG_MAX_PROPERTY_NAME_SIZE ]; /* Structure name   */
  word           size;                     /* Number of bytes of data     */
  byte           data[ DIAG_MAX_PROPERTY_SIZE ]; /* Values                */
} diag_put_perm_property_req_type;


/*===========================================================================

PACKET   diag_perm_user_cmd_req_type

ID       DIAG_PERM_USER_CMD_F

PURPOSE  Sent by the DM to call a user-specified callback function. The
         function is specified by address.

RESPONSE DMSS invokes the callback and sends a diag_perm_user_cmd_rsp_type
         packet if the callback function returns a non-null response. If the
         callback function's response has zero length, then no response
         is sent to the DM.

===========================================================================*/
#define DIAG_MAX_FUNCTION_NAME_SIZE 40
typedef PACKED struct
{
  byte           cmd_code;               /* Command code                  */
  diag_guid_type guid;                   /* GUID for verification         */
  byte           name[ DIAG_MAX_FUNCTION_NAME_SIZE ]; /* Callback fn name */
  word           length;                 /* Length of data that follows   */
  byte           data[ DIAG_MAX_USER_PACKET_SIZE ]; /* Data for callback  */
} diag_perm_user_cmd_req_type;


/*===========================================================================

PACKET   diag_get_property_rsp_type

ID       DIAG_GET_PROPERTY_F

PURPOSE  Sent by the DM to retrieve a specified number of bytes from
         memory starting at a specified location.

RESPONSE DMSS retrieves data from memory and sends a
         diag_get_property_rsp_type packet

===========================================================================*/
typedef PACKED struct
{
  byte           cmd_code;                /* Command code                */
  diag_guid_type guid;                    /* GUID for verification       */
  dword          address;                 /* Starting address in memory  */
  word           size;                    /* Number of bytes to retrieve */
  byte           data[ DIAG_MAX_PROPERTY_SIZE ]; /* Byte values          */
} diag_get_property_rsp_type;


/*===========================================================================

PACKET   diag_put_property_rsp_type

ID       DIAG_PUT_PROPERTY_F

PURPOSE  Sent by the DM to set the values of a specified number of bytes
         in memory starting at a specified location.

RESPONSE DMSS sets the specified bytes to the specified values and sends a
         diag_put_property_rsp_type packet

===========================================================================*/
typedef diag_put_property_req_type diag_put_property_rsp_type;


/*===========================================================================

PACKET   diag_get_guid_rsp_type

ID       DIAG_GET_GUID_F

PURPOSE  Sent by the DM to retrieve the GUID (globally unique identifier)
         for the current build. This is stored during the build process.

RESPONSE DMSS retrieves the GUID and sends a diag_get_guid_rsp_type packet

===========================================================================*/
typedef PACKED struct
{
  byte           cmd_code;           /* Command code                */
  diag_guid_type guid;               /* Globally unique identifier  */
} diag_get_guid_rsp_type;


/*===========================================================================

PACKET   diag_user_cmd_rsp_type

ID       DIAG_USER_CMD_F

PURPOSE  Sent by the DM to call a user-specified callback function. The
         function is specified using its address.

RESPONSE DMSS invokes the callback and sends a diag_user_cmd_rsp_type packet
         if the callback function returns a non-null response. If the
         callback function's response has zero length, then no response
         is sent to the DM.

===========================================================================*/
typedef diag_user_cmd_req_type diag_user_cmd_rsp_type;


/*===========================================================================

PACKET   diag_get_perm_property_rsp_type

ID       DIAG_GET_PERM_PROPERTY_F

PURPOSE  Sent by the DM to retrieve the contents of a structure specified
         by name.

RESPONSE DMSS retrieves data and sends a diag_get_perm_property_rsp_type
         packet

===========================================================================*/
typedef PACKED struct
{
  byte           cmd_code;                 /* Command code                */
  diag_guid_type guid;                     /* GUID for verification       */
  byte           name[ DIAG_MAX_PROPERTY_NAME_SIZE ]; /* Structure name   */
  word           size;                    /* Number of bytes to retrieve  */
  byte           data[ DIAG_MAX_PROPERTY_SIZE ]; /* Structure Contents    */
} diag_get_perm_property_rsp_type;


/*===========================================================================

PACKET   diag_put_perm_property_rsp_type

ID       DIAG_PUT_PERM_PROPERTY_F

PURPOSE  Sent by the DM to fill in a structure specified by name.

RESPONSE DMSS fills in the structure using the specified bytes and sends a
         diag_put_perm_property_rsp_type packet

===========================================================================*/
typedef diag_put_perm_property_req_type diag_put_perm_property_rsp_type;


/*===========================================================================

PACKET   diag_perm_user_cmd_rsp_type

ID       DIAG_USER_CMD_F

PURPOSE  Sent by the DM to call a user-specified callback function. The
         function must be specified by name.

RESPONSE DMSS invokes the callback and sends a diag_perm_user_cmd_rsp_type
         packet if the callback function returns a non-null response. If the
         callback function's response has zero length, then no response
         is sent to the DM.

===========================================================================*/
typedef diag_perm_user_cmd_req_type diag_perm_user_cmd_rsp_type;


#endif /* FEATURE_DIAG_QCT_EXT */

#endif /* DIAGX_H */

