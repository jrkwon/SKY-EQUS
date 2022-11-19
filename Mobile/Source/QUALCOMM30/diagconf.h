#ifndef DIAGCONF_H 
#define DIAGCONF_H
/*==========================================================================

  C O N F I G U R A T I O N   O F   P O W E R   A D J U S T M E N T

                             T A B L E S

                        H E A D E R   F I L E


DESCRIPTION
  Data declarations to support the configuration of the power adjustment
  tables.

  Copyright (c) 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
  Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
  Copyright (c) 1996,1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
  Copyright (c) 1999           by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/diagconf.h_v   1.1   10 Sep 1999 17:39:18   lchan  $

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/31/99   ms      Include comdef.h
04/10/92   twp     First release

===========================================================================*/

/*===========================================================================

                       INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*
   The set of parameters that can be downloaded or retreived. 
*/

typedef enum {
  CONF_ID_APC_TAB1_I,                 /* A power control look-up-table    */
  CONF_ID_APC_TAB2_I,
  CONF_ID_APC_TAB3_I,
  CONF_ID_APC_TAB4_I,
  CONF_ID_MAX_I                       /* The last enum entry, not used
                                         except to terminate this list    */
} conf_item_id_type;

/*
   The set of actions that can be performed on the defined parameters.
*/

typedef enum {
 CONF_ACT_WRITEALL_F,              /* Write the entire item to its permanent
                                      storage location  (NV)           */
 CONF_ACT_WRITESOME_F,             /* Write some specified part of the item
                                      to its permanent location (NV)   */
 CONF_ACT_READALL_F,               /* Read and return the entire item  */
 CONF_ACT_READSOME_F,              /* Read and return some specified part
                                      of the item.                     */
 CONF_ACT_USEALL_F,                /* Use the entire item, but do not store
                                      in its permanent location        */
 CONF_ACT_USESOME_F,               /* Use some specified part of the item,
                                      but do not store in its permanent
                                      location                         */
 CONF_ACT_TEST_F,                  /* The first (of how many?) tests to
                                      perform using the loaded data */
 CONF_ACT_MAX_F                    /* The last enum entry, not used except
                                      to terminate this list           */
} conf_item_action_type;



/*
   The parameter download/retrieval header.
*/

typedef PACKED struct
{
 conf_item_action_type  action;    /*  Identifies the operation          */
 conf_item_id_type          id;    /*  Identifies the parameter          */
 word                     size;    /*  Size (in bytes) of the parameter
                                       data                              */
 word                   offset;    /*  For operations involving only some
                                       of the parameter (large lookup
                                       tables), the offset, in bytes, from 
                                       the beginning of the parameter 
                                       item.  */
} conf_hdr_type;


/*===========================================================================

   The definitions of all the parameter items

============================================================================*/

/*
   The analog power control look-up-table types are in flux
*/

#define  APC_TAB1_SIZ 256
#define  APC_TAB2_SIZ 256
#define  APC_TAB3_SIZ  32
#define  APC_TAB4_SIZ   1

typedef byte  conf_apc_tab1_type[  APC_TAB1_SIZ  ] ;
typedef byte  conf_apc_tab2_type[  APC_TAB2_SIZ  ] ;
typedef byte  conf_apc_tab3_type[  APC_TAB3_SIZ  ] ;
typedef byte  conf_apc_tab4_type[  APC_TAB4_SIZ  ] ;




/*===========================================================================

   The parm_data_type is a union of all the defined parameter types

============================================================================*/

typedef PACKED union
{
 conf_apc_tab1_type      apc1;
 conf_apc_tab2_type      apc2;
 conf_apc_tab3_type      apc3;
 conf_apc_tab4_type      apc4;
} conf_data_type;

#endif  /* DIAGCONF_H */
