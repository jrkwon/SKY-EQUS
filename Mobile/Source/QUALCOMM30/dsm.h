#ifndef DSM_H
#define DSM_H
/*===========================================================================

                D A T A   S E R V I C E   M E M O R Y   P O O L

                       H E A D E R    F I L E

DESCRIPTION
  This file contains types and declarations associated with the DMSS Data
  Service Memory pool and services.

Copyright (c) 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 2000,2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   O:/src/asw/COMMON/vcs/dsm.h_v   1.3   21 Feb 2001 13:43:14   akhare  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/21/01   ak      Increased small item pool to account for 16x R-SCH with
                   IS2000.
01/02/01   na      Minor cleanup - removed FEATURE_NEW_SIO( always on) 
                   & FEATURE_DS_TEST_MEM_POOL( never on).
01/18/01   va      Fixed some compiler errors when T_WINNT is defined.
10/10/00   jd      Merged PPG r1.16 changes from PPG (following 2 entries)
10/04/00   hcg     Added dsm_offset_new_buffer() and dsm_pushdown_tail() 
                   functions.
09/12/00   jd      Added sizing of DSM_DS_SMALL_ITEMs to include room for 
                   rfcomm and l2cap header fields when FEATURE_BT enabled.
09/04/00   ttl     Removed DSM_DNS_LOOKUP due to the change of the DNS lookup 
                   mechanism.
08/25/00   rc      Added Type for identifying a particular DSM memory pool.
                   Added pools for DUP, Bluetooth and DIAG.
                   Added a Type for table definition for managing memory
                   free pools. Added a mask for indicating which zone the
                   pool is in.Added new macro for setting the zone the pool
                   is in. Added macros for getting status of a pool (i.e
                   which zone the pool is in). Added a prototype to register 
                   the memory events for the pools. dsm_pushdown() takes 
                   pool_id as an input parameter and returns a boolean value.
                   dsm_return_small_item_free_cnt() 
                   and dsm_return_large_item_free_cnt() take pool_id as 
                   parameter.dsm_dup_packet() get a buffer from dup pool.
                   Added prototype for dsm_peek_byte().                    
06/10/00   snn     Added a new item ,DSM_DNS_LOOKUP. This is used for performing
                   the DNS Lookup for IP address of a given hostname.
06/07/00   rc      Removed ovld of link ptr and dup ptr in dsm_item_type and
                   added a new variable app_field for user specific needs.
04/14/00   rc      Changed DSM_SMALL_ITEM_CNT from 360 to 260 for Pink 
                   Panther builds and for Tiger builds with 
                   FEATURE_DIAG_NON_STREAMING defined.
03/10/00   snn     Added a new constant DSM_CHAP_PROTOCOL
02/22/00   ak      Added code to support callbacks from RLP3.
12/06/99   hcg     Removed dsm_q_last_get() function prototype, as its 
                   functionality is now provided as part of queue services.
11/30/99   na      Added prototype for dsm_pullup_tail.
                   Increased DSM_SMALL_ITEM_SIZ to 44 for cdma2000 operation.
09/01/99   mvl     Changed the DSM_PPP_ASY_IN tracer field so that it is now
                   DSM_UM_PPP_ASY_IN and added DSM_RM_ASY_IN tracer.
08/25/99   mvl     Added tracer type to help track netmodel memory leak.
08/20/99   na      Added prototype for dsm_check_mem_pool.
06/17/99   mvl     Added the prototype for dsm_seek_extract(), and added new
                   DSM kind field types to support the network model.
06/14/99   na      Fixed a bug where size was not being multiplied by 
                   DSM_ALLOCATED_DIV in the DSM_ITEM_HEAD macro.
04/08/99   na      Reduced dsm_item_type struct header by 4 bytes.
03/04/99   na/hcg  Added new DS-to-PS protocol commands to support sockets
                   interface, under FEATURE_DS_SOCKETS.
01/17/99   ak      New SIO is now under a compile time switch.
01/09/99   jjw     Added parameter to support new SIO interface
12/03/98   na      Added prototype for dsm_q_last_get().
10/31/98   ldg     dsm_pull16 and dsm_pull32 now present only for FEATURE_DS.
                   Changed an "enum" declaration to "typedef enum" (ARM).
10/30/98   na      Increased SMALL_ITEM_DONT_EXCEED.
10/29/98   na      Increased SMALL_ITEM_CNT to 360 and LARGE_ITEM_CNT to 20.
08/10/98   na      Reduced memory pool usage. 
07/24/98   jjn     Made free item processing more efficient by implementing
                   free item stack instead of queue.
04/14/98   na      Added support for buffer based flow control.
01/19/98   na      Increased size of small items to 36 to account for larger
                   rxlev in the serial driver.
12/12/97   ldg     Commented enum definitions of DSM_STATS items.
11/06/97   ldg     Corrected return type of dsm_get_stats() to void.
09/09/97   ldg     Added accessor function for UI to get DSM stats.
06/16/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/05/97   na      Changed large item size to TCP segment size - we can now
                   have a string of large items.
01/16/97   JGR     Added Large item size for Unwired Planet inside FEATURE_DS_UP
11/14/96   jjw     Changes to 'kind' field for DS-PS interface
10/18/96   jjw     Added new 'kind' field definition to enhance TCP Open
10/17/96   jjw     Added UP mode interface definitions
05/24/96   jjw     Gemini & ISS2 Data Services release.
11/09/95   jjw     Completed changes to support fully compliant IS-99
07/13/95   jjw     Changes to support IS-99 protocol integration
01/26/94   was     added features for packet management.
10/10/94   jjw     Created.

===========================================================================*/


#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "customer.h"
#include "queue.h"      /* Queue declaration package            */
#ifndef T_WINNT
#include "psglobal.h"
#endif


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


#ifdef FEATURE_DS_DEBUG_UI
typedef enum
{
   DSM_STATS_SMALL_ITEMS_USED, /* The number of small items not in the free
                                  queue (either in use or lost),  minus one.
                                  (There is always at least one in use.    */

   DSM_STATS_LARGE_ITEMS_USED, /* The number of large items not in the free
                                  queue (either in use or lost).           */


   DSM_STATS_DUP_ITEMS_USED,   /* The number of dup items not in the
                                  free queue (either in use or lost)       */ 
#ifdef FEATURE_DSM_BT_ITEMS
#error code not present
#endif /* FEATURE_DSM_BT_ITEMS */
#ifdef FEATURE_DSM_DIAG_ITEMS
   DSM_STATS_DIAG_ITEMS_USED,  /* The number of diag items not in the free
                                  queue (either in use or lost).           */
#endif /* FEATURE_DSM_DIAG_ITEMS */
   DSM_STATS_COUNT
} dsm_stats_enum;
#endif /* FEATURE_DS_DEBUG_UI */

/*---------------------------------------------------------------------------
  Watermark Type definition. This structure provides the means for 2 software
  entities to convey data and control its flow. Note that the function pointer
  fields must 1st be checked to non-NULL prior to invocation.
---------------------------------------------------------------------------*/
typedef struct
{
  word   lo_watermark;                   /* Low watermark for total bytes  */
  word   hi_watermark;                   /* Hi watermark for total bytes   */
  word   dont_exceed_cnt;                /* Discard count for RX bytes     */
  word   current_cnt;                    /* Current total byte count       */
#ifdef FEATURE_DSM_MEM_CHK
  word   highest_cnt;                    /* largest count since powerupp   */
  dword  total_rcvd_cnt;                 /* Total RX bytes thru watermark  */
#endif
  void   (*lowater_func_ptr)(void);      /* Lo Watermark function pointer,
                                            Invoke function when queue hits
                                            the Lo watermark               */
  void   (*gone_empty_func_ptr)(void);   /* Gone empty function pointer,
                                            invoke when queue goes empty   */
  void   (*non_empty_func_ptr)(void);    /* Non-empty function pointer,
                                            Invoke when queue goes nonempty*/
  void   (*hiwater_func_ptr)(void);      /* Hi Watermark function pointer,
                                            Invoke function when queue hits
                                            the Hi watermark               */
  q_type *q_ptr;                         /* Pointer to associated queue    */
} dsm_watermark_type;

/*---------------------------------------------------------------------------
  Data Service Memory Pool type and kind field definitions.
---------------------------------------------------------------------------*/
typedef struct dsm_item_s
{
  q_link_type        link;         /* field for putting/getting to/from Qs */

  struct dsm_item_s  *pkt_ptr;     /* Pointer, if used, to next buffer item
                                      in packet                            */
  struct dsm_item_s  *dup_ptr;     /* Pointer, if used, to any duplicated
                                      buffer item                          */
  uint32             app_field;    /* user defined field                   */

  byte               references;   /* Reference count (ie duplicate count) */

  /*-------------------------------------------------------------------------
    To keep allocated as a byte, the actual size of the dsm_item divided 
    by 4 is stored in the allocated field.
  -------------------------------------------------------------------------*/
  byte               allocated;    /* bytes in buffer, with head & foot    */
  #define DSM_ALLOCATED_DIV 4
  
  word               size;         /* maximum size of array allocation     */

  word               used;         /* current length of stuff in data array*/

  byte               kind;         /* protocol type or TCP command/response*/

  byte               priority;        /* priority field                    */

#ifdef FEATURE_DSM_MEM_CHK
  /*-------------------------------------------------------------------------
    The 2 fields below provide the means to debug memory problems.
  -------------------------------------------------------------------------*/
  /*-------------------------------------------------------------------------
    The cookie field must also contain the DSM_COOKIE value. The
    'dsm_free_buffer' and 'dsm_new_buffer' will check the cookie field for
    validity and ERR_FATAL if the field is not valid.
  -------------------------------------------------------------------------*/
  word               cookie;

  /*-------------------------------------------------------------------------
    The tracer field holds the value for the last queue/watermark/task/etc
    where a 'dsm_item_type' was placed. This aids in recontructing memory
    leaks. 0-79 are reserved for Data Services. 80-ff can be used by other 
    applications.
  -------------------------------------------------------------------------*/
  word               tracer;

#endif /* FEATURE_DSM_MEM_CHK */

  byte              *data_ptr;    /* working data pointer. */
} dsm_item_type;


/*---------------------------------------------------------------------------
  DS-to-PS protocol commands.
---------------------------------------------------------------------------*/
#define DSM_TCP_CTL_OPEN_ACTIVE    2  /* Open TCP, no data to send yet     */
#define DSM_IP_CTL_OPEN            3  /* Open IP mode for PS task          */
#define DSM_TCP_CTL_OPEN_PASSIVE   4  /* Open TCP, data ready to send      */
#define DSM_PROTOCOL_CLOSE         5  /* Close current PS protocol         */
#define DSM_PROTOCOL_ABORT         6  /* Abort current PS protocol         */

/*---------------------------------------------------------------------------
  PS-to-DS protocol command results.
---------------------------------------------------------------------------*/
#define DSM_PROTOCOL_OPENED        7  /* Protocol Opened                   */
#define DSM_PROTOCOL_CLOSING       8  /* BS initiated TCP close in process */
#define DSM_PROTOCOL_CLOSED        9  /* Protocol now fully closed         */
#define DSM_PROTOCOL_ABORTED       6  /* Protocol Aborted during a close   */

#define DSM_SERIAL_IO             10  /* Serial I/O item type              */
#define DSM_RLP                   12  /* RLP item type                     */
#define DSM_APP_PAYLOAD           14  /* ATCoP or IP payload data          */
#define DSM_ENABLE_SIO_APP_FLOW   16  /* Enable TCP TX data flow           */
#define DSM_DISABLE_SIO_APP_FLOW  17  /* Disable TCP TX data flow          */
#define DSM_ENABLE_DS_APP_FLOW    18  /* Enable PS-to-DS data flow         */
#define DSM_DISABLE_DS_APP_FLOW   19  /* Disable PS-to-DS data flow        */
#define DSM_RAW_APP_PAYLOAD       22  /* Unescaped bytes for Async data    */
#define DSM_LCP_PROTOCOL          23  /* PPP LCP protocol identifier       */
#define DSM_IPCP_PROTOCOL         24  /* PPP IPCP protocol identifier      */
#define DSM_PAP_PROTOCOL          25  /* PPP PAP protocol identifier       */
#define DSM_IP_COMPRESS           26  /* Compress this IP datagram         */
#define DSM_IP                    33  /* IP datagram packet item type 0x21 */

#define DSM_CHAP_PROTOCOL         27  /* PPP CHAP protocol identifier      */

#ifdef  FEATURE_DS_NET_MODEL
/*-------------------------------------------------------------------------
  PS-to-DS command
-------------------------------------------------------------------------*/
#define DSM_DS_DROP_CALL         100  /* Used for ppp outgoing             */
#endif

#ifdef FEATURE_DS_SOCKETS
#define DSM_SOCKET_NET_DOWN      200  /* Network is down for the socket    */
#define DSM_SOCKET_NET_UP        201  /* Network is up for the socket      */
#define DSM_SOCKET_OPEN_PPP      202  /* Bring up PPP                      */
#define DSM_SOCKET_PAYLOAD       203  /* Socket TCP payload                */
#define DSM_SOCKET_CONNECT       204  /* TCP Connect packet                */
#endif /* FEATURE_DS_SOCKETS */

#ifdef FEATURE_DS_NET_MODEL
#define DSM_PPP_FRAME_ONLY       250  /* Used for ppp outgoing             */
#endif

#define DSM_PS_HOLD_PAYLOAD      252  /* PS task hold payload              */

/*---------------------------------------------------------------------------
  Priority Field Values
---------------------------------------------------------------------------*/

#define DSM_NORMAL      0
#define DSM_FAST        0x40
#define DSM_HIGHEST     0x7f

#define DSM_COOKIE     0xBEAF     /* Cookie must have this value */

#define DSM_FREE            0   /* item in free queue                      */

/* Over-the-air-RX-to-serial-TX direction values                           */

#define DSM_RXC_RLP          1  /* RX task RLP RX processing               */
#define DSM_RLP_POST_RX_Q    3  /* Queued from RLP post-RX to PPP RX queue */
#define DSM_UM_PPP_ASY_IN    5  /* Um 'ppp_asy_in' processing              */
#define DSM_PPP_INCOMING     7  /* PPP packet processing                   */
#define DSM_TCP_RCVQ         9  /* Application data queued to TCP RX queue */
#define DSM_DS_FROM_PS_Q    11  /* Application/command Queued to DS task   */
#define DSM_DS_TO_SIO_WM    13  /* DS gave to SIO for serial TX            */
#define DSM_CREATED_IN_ASY_IN 15 /* dsm item allocated in ppp_asy_in()     */
#define DSM_RM_PPP_ASY_IN   17  /* Um 'ppp_asy_in' processing              */

/*---------------------------------------------------------------------------
  Serial-RX-to-over-the-air-TX direction values                           
 --------------------------------------------------------------------------*/

#define DSM_DS_FROM_SIO_WM   2  /* SIO to DS task watermark                */
#define DSM_DS_TO_PS_WM      4  /* DS task to PS task watermark            */
#define DSM_TCP_SNDQ         6  /* TCP Send Queue                          */
#define DSM_PPP_TX_WM        8  /* PPP TX watermark                        */
#define DSM_PS_TO_RLP_WM    10  /* PS to RLP TX watermark                  */
#define DSM_RLP_TX          12  /* RLP TX processing                       */
#define DSM_SIO_RX          14  /* Inside SIO                              */


/*---------------------------------------------------------------------------
  Item size for DS Small Memory Pool items.
  - 36 for RLP2
  - 44 for RLP3
  - 44 + RFC (bt_rc_frame_hdre_type)/L2C(bt_rc_fcs_type) for Bluetooth
---------------------------------------------------------------------------*/
#define DWORDALIGN(x)                  (((x)+3)/4*4) /* least n, 4*n >= x  */

#ifdef FEATURE_BT
#error code not present
#elif defined (FEATURE_DS_RLP3)
#define DSM_DS_SMALL_ITEM_DATA_OFFSET  (0)
#define DSM_DS_SMALL_ITEM_FOOTER       (0)
#define DSM_DS_SMALL_ITEM_SIZ          44               /* Size of MuxPDU3 */

#else /* !defined (FEATURE_BT) && !defined (FEATURE_DS_RLP3) */
#define DSM_DS_SMALL_ITEM_DATA_OFFSET  (0)
#define DSM_DS_SMALL_ITEM_FOOTER       (0)
#define DSM_DS_SMALL_ITEM_SIZ          36     /* Rate set 2 RLP frame size */

#endif /* !defined (FEATURE_BT) && !defined (FEATURE_DS_RLP3) */

/*---------------------------------------------------------------------------
  Small item memory pool items count.
---------------------------------------------------------------------------*/
#ifdef FEATURE_DS_RLP3
  #define DSM_DS_SMALL_ITEM_CNT   540
#elif (defined(T_MSM31) || (defined(T_MSM5000) && defined(FEATURE_DIAG_NON_STREAMING))) 
  #define DSM_DS_SMALL_ITEM_CNT   260
#else
  #define DSM_DS_SMALL_ITEM_CNT   360
#endif

/*---------------------------------------------------------------------------
  Sizing for Large Memory Pool items. Header size defines enough space for
  worst case TCP/IP/PPP header.
---------------------------------------------------------------------------*/
#define DSM_HEADER      128  /* room for worstcase TCP/IP/PPP header bytes */
#define DSM_FOOTER        4  /* only 3 needed (for PPP CRC and flag bytes) */
#define DSM_EXTRA       (DSM_HEADER + DSM_FOOTER)

#ifdef FEATURE_DS
#define DSM_DS_LARGE_ITEM_SIZ (DEF_MSS + DSM_EXTRA)
                                     /* large enough to hold PPP packets
                                        using TCP MSS, plus 132            */
#else
#define DSM_DS_LARGE_ITEM_SIZ 668 /* This definition of DSM_DS_LARGE_ITEM_SIZ
                                        is intended to be equal to the above
                                        definition.  However, the above 
                                        defintion introduces dependencies
                                        which necessiate inclusion of many
                                        DS specific files.  Such dependencies
                                        are un-warranted for targets without
                                        DS feature.                        */                                       
#endif

/*---------------------------------------------------------------------------
  Large item memory pool items count.
---------------------------------------------------------------------------*/
#ifdef FEATURE_WAP
#define DSM_DS_LARGE_ITEM_CNT   40 
#else
#define DSM_DS_LARGE_ITEM_CNT   20
#endif//feature_wap

/*---------------------------------------------------------------------------
  Item size for DUP memory pool items.
---------------------------------------------------------------------------*/
#define DSM_DUP_ITEM_SIZ        12 
/*---------------------------------------------------------------------------
  DUP item memory pool item count.
---------------------------------------------------------------------------*/
#define DSM_DUP_ITEM_CNT        120 


#ifdef FEATURE_DSM_BT_ITEMS
#error code not present
#endif /* FEATURE_DSM_BT_ITEMS */

#ifdef FEATURE_DSM_DIAG_ITEMS
  /*-------------------------------------------------------------------------
    Item size for DIAG memory pool items.
  -------------------------------------------------------------------------*/
  #define DSM_DIAG_ITEM_SIZ       100 
  /*-------------------------------------------------------------------------
    DIAG item memory pool item count.
  -------------------------------------------------------------------------*/
  #define DSM_DIAG_ITEM_CNT       50 
   
#endif /* FEATURE_DSM_DIAG_ITEMS */

/*---------------------------------------------------------------------------
  Definitions for few, many and do not exceed counts used in buffer
  based flow control. 
  Dont_Exceed is based on the minimum number of buffers required by the 
  system. All incoming data from Rm or Um is dropped at this point.
  Few Mark is when the system starts flow controlling the Um and Rm interfaces
  to stop new data from coming into the system.
  Many Mark is when the system reallows new data to come into the system from
  the Um or the Rm interface.
  These are empirical numbers based on various test scenarios. 
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  Small and large items have separate flow points.
  The small item and large item numbers are approximately in the same ratio.
---------------------------------------------------------------------------*/

#ifdef FEATURE_DS_RLP3
  #define DSM_DS_SMALL_ITEM_DONT_EXCEED   5  
  #define DSM_DS_SMALL_ITEM_FEW_MARK      50
  #define DSM_DS_SMALL_ITEM_MANY_MARK     75
#else
  #define DSM_DS_SMALL_ITEM_DONT_EXCEED   5  
  #define DSM_DS_SMALL_ITEM_FEW_MARK      100
  #define DSM_DS_SMALL_ITEM_MANY_MARK     140
#endif  


#define DSM_DS_LARGE_ITEM_DONT_EXCEED     1  
#define DSM_DS_LARGE_ITEM_FEW_MARK        7
#define DSM_DS_LARGE_ITEM_MANY_MARK       11

/*---------------------------------------------------------------------------
  Few, many and do not exceed counts for DUP items 
---------------------------------------------------------------------------*/
#define DSM_DUP_ITEM_DONT_EXCEED          5
#define DSM_DUP_ITEM_FEW_MARK             25
#define DSM_DUP_ITEM_MANY_MARK            50

/*---------------------------------------------------------------------------
  Few, many and do not exceed counts for Bluetooth items.
---------------------------------------------------------------------------*/
#ifdef FEATURE_DSM_BT_ITEMS
#error code not present
#endif /* FEATURE_DSM_BT_ITEMS */

/*---------------------------------------------------------------------------
  Few, many and do not exceed counts for DIAG items 
---------------------------------------------------------------------------*/
#ifdef FEATURE_DSM_DIAG_ITEMS
  #define DSM_DIAG_ITEM_DONT_EXCEED       5
  #define DSM_DIAG_ITEM_FEW_MARK          10
  #define DSM_DIAG_ITEM_MANY_MARK         30 
#endif /* FEATURE_DSM_DIAG_ITEMS */

/*---------------------------------------------------------------------------
  Type for identifying a particular DSM memory pool
---------------------------------------------------------------------------*/
typedef enum
{
  DSM_DS_SMALL_ITEM_POOL = 0,       /* Data Services Small item pool */
  DSM_DS_LARGE_ITEM_POOL,           /* Data Services Large item pool */
  DSM_DUP_ITEM_POOL,                /* DUP HEADER items pool */
#ifdef FEATURE_DSM_BT_ITEMS
#error code not present
#endif /* FEATURE_DSM_BT_ITEMS */
#ifdef FEATURE_DSM_DIAG_ITEMS
  DSM_DIAG_ITEM_POOL,               /* DIAG streaming items pool     */
#endif /* FEATURE_DSM_DIAG_ITEMS */
  DSM_ITEM_POOL_MAX                 /* Array sizer, MUST BE LAST     */
} dsm_mempool_id_enum_type;


/*---------------------------------------------------------------------------
  Type for the Table definition for managing the memory free pools.
  free_count: The number of available memory pool items.
  few_count: The count of memory pool items that denotes the "few" condition.
  The "few" condition signifies that the particular pool is running low on
  memory
  many_count: The count of memory pool items that denotes the "many" 
  condition. The "many" condition signifies that the particular pool has
  plenty of available memory.
  dont_exceed_count: The count of memory pool items that denotes that no more
  memory should be allocated.
  flow_control_trigger: This boolean is used decide when to enable or disable
  flow control, i.e it decides when the few or many events are called
---------------------------------------------------------------------------*/

typedef struct
{
  word    free_count;       /* The current number of items in Free Queue   */
  word    item_count;       /* The max number of items in the pool         */
  word    pool_item_size;   /* Size of items for the pool                  */
  word    min_free_count;   /* Minimum count the free pool dropped to      */
  word    many_count;       /* Count when pool reaches "Many" condition    */
  byte    few_count;        /* Count when pool reaches "Few" condition     */
  byte    dont_exceed_count;/* Count when pool reaches "Dont Exceed" 
                                                                condition  */
  boolean flow_control_trigger; /* Indicates when flow control should be   */
                                /* enabled. TRUE indicates flow control
                                   enabled, FALSE indicates flow control
                                   disabled.                               */
  void    (*many_func_ptr)(dsm_mempool_id_enum_type );    
                                /* "Many" event function holder            */
  void    (*few_func_ptr)(dsm_mempool_id_enum_type );     
                                /* "Few" event function holder             */
  void    (*dont_exceed_func_ptr)(dsm_mempool_id_enum_type );  
                                /* "Dont Exceed" event function holder     */
} dsmi_flow_mgmt_table_type;


extern dsmi_flow_mgmt_table_type dsmi_flow_mgmt_table[DSM_ITEM_POOL_MAX];

extern dsm_mempool_id_enum_type pool_id;

extern uint32 dsm_zone_bitmask;

/*--------------------------------------------------------------------------
  This MACRO will return the memory pool ID for the DS memory pool 
  (DS_SMALL or DS_LARGE) that is >= the passed size parameter.
  This MACRO should only be used for the DS Small & DS Large item pools.
--------------------------------------------------------------------------*/
#define DSM_DS_POOL_SIZE(size) \
(dsm_mempool_id_enum_type)\
((dsmi_flow_mgmt_table[DSM_DS_SMALL_ITEM_POOL].pool_item_size >= \
 (word)(size)) ? \
(DSM_DS_SMALL_ITEM_POOL) : \
(DSM_DS_LARGE_ITEM_POOL))

/*--------------------------------------------------------------------------
  Mask indicating which zone the pool is in.
                  -------------------------------
                  |  MORE THAN MANY ZONE        |
                  ------------------------------- -- MANY MARK
                  | LESS THAN THAN MANY ZONE    |
                  ------------------------------- -- FEW MARK
                  |  LESS THAN FEW ZONE         |
                  ------------------------------- -- DONT EXCEED
                  |  DONT EXCEED ZONE           |
                  ------------------------------- -- 0
 Three bits are reserved per pool to indicate which zone the pool is in.
 Starting from the LSB:
 MORE THAN MANY ZONE(bit 0)
 LESS THAN MANY ZONE(bit 1)
 LESS THAN FEW ZONE (bit 2)
 DONT EXCEED ZONE (bit 3)
--------------------------------------------------------------------------*/

#define DSM_MORE_THAN_MANY_MASK 0x00000001
#define DSM_LESS_THAN_MANY_MASK 0x00000002
#define DSM_LESS_THAN_FEW_MASK  0x00000004
#define DSM_DONT_EXCEED_MASK    0x00000008

/*--------------------------------------------------------------------------
  Mask to clear the dsm_zone_bitmask for a particular pool before setting 
  it with a new value. This is done to ensure that the other pools are not 
  affected.
--------------------------------------------------------------------------*/
#define DSM_CLEAR_POOL_MASK     0x0000000f


/*--------------------------------------------------------------------------
  Macro that sets the zone the pool is in
--------------------------------------------------------------------------*/

#define DSM_SET_ZONE_BIT(pool_id)\
 dsm_zone_bitmask &= (~(DSM_CLEAR_POOL_MASK << (pool_id * 4))); \
 ((dsmi_flow_mgmt_table[pool_id].free_count >= \
  dsmi_flow_mgmt_table[pool_id].many_count) ? \
  (dsm_zone_bitmask |= (DSM_MORE_THAN_MANY_MASK << (pool_id * 4))): \
  (dsmi_flow_mgmt_table[pool_id].free_count >= \
  dsmi_flow_mgmt_table[pool_id].few_count) ? \
  (dsm_zone_bitmask |= (DSM_LESS_THAN_MANY_MASK << (pool_id * 4))) : \
  (dsmi_flow_mgmt_table[pool_id].free_count > \
  dsmi_flow_mgmt_table[pool_id].dont_exceed_count) ? \
  (dsm_zone_bitmask |= (DSM_LESS_THAN_FEW_MASK << (pool_id * 4)))  : \
  (dsm_zone_bitmask |= (DSM_DONT_EXCEED_MASK << (pool_id * 4))))

/*-------------------------------------------------------------------------
  MACROS to get the status of a pool
-------------------------------------------------------------------------*/

#define DSM_MORE_THAN_MANY_FREE_ITEMS(pool_id) \
(boolean)((dsm_zone_bitmask & (DSM_MORE_THAN_MANY_MASK << (pool_id * 4))) != 0 ? \
  TRUE : FALSE )
 
#define DSM_LESS_THAN_MANY_FREE_ITEMS(pool_id) \
(boolean)((dsm_zone_bitmask & (DSM_LESS_THAN_MANY_MASK << (pool_id * 4))) != 0 ? \
  TRUE : FALSE )

#define DSM_LESS_THAN_FEW_FREE_ITEMS(pool_id) \
(boolean)((dsm_zone_bitmask & (DSM_LESS_THAN_FEW_MASK << (pool_id * 4))) != 0 ? \
  TRUE : FALSE )

#define DSM_DONT_EXCEED_ITEMS(pool_id) \
(boolean)((dsm_zone_bitmask & (DSM_DONT_EXCEED_MASK << (pool_id * 4))) != 0 ? \
  TRUE : FALSE )


/*---------------------------------------------------------------------------
  Macro to set the data_ptr field of a buffer item to the beginning of the
  Packet head portion of a data buffer item.

DEPENDENCY: 1) bp must be a DSM_DS_LARGE_ITEM_SIZ item.
            2) bp must be a 'dsm_item_type'
---------------------------------------------------------------------------*/
#define DSM_ITEM_HEAD( bp)  \
  (bp->data_ptr = (byte *)((byte *)bp + sizeof(dsm_item_type))); \
  bp->size      = (bp->allocated * DSM_ALLOCATED_DIV)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION dsm_reg_mempool_events

DESCRIPTION
  This function will register the passed function pointers to be later invoked
  if the specified events occur for the specified memory pool.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_reg_mempool_events
(
  dsm_mempool_id_enum_type  pool_id,   /* Memory pool ID indicator         */
  void  (*many_func_ptr)(dsm_mempool_id_enum_type pool_id),  
                                       /* "Many" event function ptr        */
  void  (*few_func_ptr)(dsm_mempool_id_enum_type pool_id),  
                                       /* "Few" event function ptr         */
  void  (*dont_exceed_func_ptr)(dsm_mempool_id_enum_type pool_id)
                                       /* "Dont Exceed" event function ptr */
);

/*===========================================================================

FUNCTION DSM_INIT

DESCRIPTION
  This function will initialize the Data Service Memory Pool. It should be
  called once upon system startup. All the memory items are initialized and
  put onto their respective free queues.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_init( void);


/*===========================================================================

FUNCTION dsm_dequeue

DESCRIPTION
  This function will return a pointer to the next item on the shared queue
  associated with the passed Watermark item. This function will also update
  the 'current_cnt' field in the passed Watermark item and check for and
  perform any relevent 'get' events.

DEPENDENCIES
  None

RETURN VALUE
  A pointer to a 'dsm_item_type' or NULL if no items available.

SIDE EFFECTS
  None
===========================================================================*/

extern dsm_item_type *dsm_dequeue
(
  dsm_watermark_type *wm_ptr      /* pointer to Watermark item to get from */
);

/*===========================================================================

FUNCTION dsm_enqueue

DESCRIPTION
  This function will put the passed DSM item to the passed shared queue then
  check for and perform any 'put' events.

DEPENDENCIES
  The 'item_ptr' parameter must not be NULL.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_enqueue
(
  dsm_watermark_type *wm_ptr,     /* pointer to Watermark item to put to   */
  dsm_item_type      **item_pp    /* pointer to item to load onto queue    */
);

/*===========================================================================

FUNCTION dsm_free_buffer

DESCRIPTION
  This function will return to the proper DSM free queue the passed buffer.

DEPENDENCIES
  None

RETURN VALUE
  The next item in the packet chain.

SIDE EFFECTS
  Note that this does not clear pointer, unlike free_packet
===========================================================================*/

extern dsm_item_type *dsm_free_buffer
(
  dsm_item_type *item_ptr       /* pointer to item to return to free queue */
);

/*===========================================================================

FUNCTION dsm_free_packet

DESCRIPTION
  This function will return to the proper DSM free queue
  the entire passed packet buffer chain.

DEPENDENCIES
  None

RETURN VALUE
  The next item in the queue.

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_free_packet
(
  dsm_item_type **item_pp       /* pointer to item to return to free queue */
);

/*===========================================================================

FUNCTION dsm_length_packet

DESCRIPTION
  This function counts the number of bytes in a packet buffer chain.

DEPENDENCIES
  None

RETURN VALUE
  The number of bytes in use.

SIDE EFFECTS
  None
===========================================================================*/

extern word dsm_length_packet
(
  dsm_item_type *item_ptr       /* pointer to item to return to free queue */
);

/*===========================================================================

FUNCTION dsm_new_buffer

DESCRIPTION
  This function will return a pointer to the next free item from the free
  queue represented by the passed parameter.  The best size will be chosen.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to a 'dsm_item_type'

SIDE EFFECTS
  None
===========================================================================*/

extern dsm_item_type *dsm_new_buffer
(
  dsm_mempool_id_enum_type pool_id               /* Memory Pool Identifier */
);

/*===========================================================================

FUNCTION dsm_offset_new_buffer

DESCRIPTION
  This function will return a pointer to the next free item from the free
  pool represented by the passed pool ID parameter.  

  In addition, the data_ptr field of the DSM item will be offset the 
  specified number of bytes into the payload.  A specified offset larger 
  than the size of the item in the pool ID will result in an error and 
  return a NULL DSM item.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to a 'dsm_item_type'.  The data_ptr will be moved by "offset"
  number of bytes.

SIDE EFFECTS
  None
===========================================================================*/

extern dsm_item_type *dsm_offset_new_buffer
(
  dsm_mempool_id_enum_type pool_id,    /* memory pool Identifier           */
  word  offset                         /* Number of bytes to move data_ptr */
);

/*===========================================================================

FUNCTION dsm_dup_packet

DESCRIPTION
  Duplicate first 'cnt' bytes of packet starting at 'offset'.
  This is done without copying data; only the headers are duplicated,
  but without data segments of their own. The pointers are set up to
  share the data segments of the original copy. The return pointer is
  passed back through the first argument, and the return value is the
  number of bytes actually duplicated.

DEPENDENCIES
  None

RETURN VALUE
  the total size in bytes of the duplicated packet.

SIDE EFFECTS
  None
===========================================================================*/

extern word dsm_dup_packet
(
  dsm_item_type **hp,      /* address of pointer to dup'ed packet          */
  dsm_item_type *bp,       /* pointer to packet to duplicate               */
  word offset,             /* offset count from start of packet            */
  word cnt                 /* count of bytes to duplicate                  */
);

/*===========================================================================

FUNCTION dsm_trim_packet

DESCRIPTION
  Trim specified length of bytes from teh end of a DSM Packet.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_trim_packet
(
  dsm_item_type **bpp,                /* Address of pointer to packet head */
  word length                         /* length to trim packet to          */
);

/*===========================================================================

FUNCTION dsm_append

DESCRIPTION
  Append a DSM buffer to end of DSM buffer chain (packet)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_append
(
  dsm_item_type **bph,             /* Address of Packet chain head pointer */
  dsm_item_type **bpp              /* Address of Packet item pointer       */
);

/*===========================================================================

FUNCTION dsm_pushdown

DESCRIPTION
  Insert specified amount of contiguous new space at the beginning of an
  dsm_item_type chain. If enough space is available in the first 
  dsm_item_type, no new space is allocated. Otherwise a dsm_item_type of the
  appropriate size is allocated and tacked on the front of the chain.  
  The cnt is updated. If the pool id passed in is for a ds pool (large or 
  small), "size" parameter is used to get the appropriate memory pool item. 
  So, if this function is being used for DS, either pass in 
  DSM_DS_LARGE_ITEM_POOL or DSM_DS_SMALL_ITEM_POOL. 
  
  This operation is the logical inverse of pullup(), hence the name.

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if the call succeeded
  FALSE - if dsm_new buffer returned NULL and so could not successfully 
          complete execution of dsm_pushdown 

SIDE EFFECTS
  None
===========================================================================*/

extern boolean dsm_pushdown
(
  dsm_item_type **bph,                  /* address of pointer to packet head,
                                           can be NULL and may be changed
                                           within this function            */
  void          *buf,                   /* Pointer to any bytes to copy    */
  word          size,                   /* size in bytes of pushdown       */
  dsm_mempool_id_enum_type pool_id      /* memory pool identifier          */
);

/*===========================================================================

FUNCTION dsm_pushdown_tail

DESCRIPTION
  Insert specified amount of contiguous new data at the end of an
  dsm_item_type chain. If enough space is available in the last item,
  no new space is allocated.  Otherwise a dsm_item_type of the appropriate 
  size is allocated and appended to the end of the chain.  

  This operation is the logical inverse of pullup_tail(), hence the name.

DEPENDENCIES
  None.

RETURN VALUE
  The number of bytes that were pushed on to the tail end of the last item.

SIDE EFFECTS
  If the pushdown requires an additional data item, the pkt_ptr field of the 
  last item in the passed packet chain may be changed.
===========================================================================*/

extern word dsm_pushdown_tail
(
  dsm_item_type **pkt_head_ptr,    /* Address of ptr to head of packet     */
  void          *buf,              /* Ptr. to bytes to copy                */
  word          size,              /* size in bytes of pushdown            */
  dsm_mempool_id_enum_type pool_id /* Memory Pool identifier               */
);

/*===========================================================================

FUNCTION dsm_pullup

DESCRIPTION
 Copy and delete "cnt" bytes from beginning of packet. Return number of
 bytes actually pulled off

DEPENDENCIES
  The passed data item must not be on a queue.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern word dsm_pullup
(
  dsm_item_type **bph,             /* Address of ptr to head of packet     */
  void *buf,                       /* dest. ptr, can be NULL to avoid copy */
  word cnt                         /* the byte count to pull from pkt head */
);

/*===========================================================================

FUNCTION dsm_pullup_tail

DESCRIPTION
 Copy and remove "cnt" bytes from the end of packet. Return number of
 bytes actually pulled off.  

DEPENDENCIES
  The passed data item must not be on a queue.

RETURN VALUE
  The number of bytes pulled from the passed packet head ptr. If the number 
  of bytes is greater than the  packet chain, the entire packet chain is 
  returned.

SIDE EFFECTS
  When the last byte is pulled from a buffer item the item is returned to
  its free queue.
===========================================================================*/
extern word dsm_pullup_tail
(
  dsm_item_type **pkt_head_ptr,    /* Address of ptr to head of packet     */
  void          *buf,              /* dest. ptr, can be NULL to avoid copy */
  word          cnt                /* the byte count to pull from pkt tail */
);


/*===========================================================================

FUNCTION dsm_pull8

DESCRIPTION
 Pull single character from dsm_item_type

DEPENDENCIES
  None

RETURN VALUE
  returns -1 if nothing

SIDE EFFECTS
  None
===========================================================================*/

extern sint15 dsm_pull8
(
  dsm_item_type **bpp                   /* Address of pointer to data item */
);

#ifdef FEATURE_DS
/*===========================================================================

FUNCTION dsm_pull16

DESCRIPTION
 Pull a 16-bit integer in host order from buffer in network byte order.
 Return -1 on error

DEPENDENCIES
  None

RETURN VALUE
  returns -1 if nothing

SIDE EFFECTS
  None
===========================================================================*/

extern sint31 dsm_pull16
(
  dsm_item_type **bpp                   /* Address of pointer to data item */
);
/*===========================================================================

FUNCTION dsm_pull32

DESCRIPTION
 Pull a 32-bit integer in host order from buffer in network byte order.
 On error, return 0. Note that this is indistinguishable from a normal
 return.

DEPENDENCIES
  None

RETURN VALUE
  returns  0 if nothing

SIDE EFFECTS
  None
===========================================================================*/

extern dword dsm_pull32
(
  dsm_item_type **bpp                   /* Address of pointer to data item */
);
#endif /* FEATURE_DS */

/*===========================================================================

FUNCTION dsm_extract

DESCRIPTION
 Copy data from within mbuf to user-provided buffer, starting at
 'offset' bytes from start of mbuf and copying no more than 'len'
 bytes. Return actual number of bytes copied

DEPENDENCIES
  None

RETURN VALUE
  returns  0 if nothing

SIDE EFFECTS
  None
===========================================================================*/

extern word dsm_extract
(
  dsm_item_type *packet_ptr, /* pointer to packet head to remove data from */
  word          offset,      /* offset number of bytes from packet head    */
  void          *buf,        /* pointer to destination buffer              */
  word          len          /* length in bytes of extraction              */
);


/*===========================================================================
FUNCTION  DSM_SEEK_EXTRACT()

DESCRIPTION
  This function will seek offset bytes into the item that is passed in.  If
  'buf' is not NULL it will copy 'len' bytes into it. If there are not 'len'
  bytes to copy all the butes from offesetuntil the end of the item chain
  will be copied.
  If the seek or the extract tries go beyond the end of the item chain the
  function will return FALSE, and niether 'item_head_ptr' nor 'offset_ptr
  will' be modified.  Otherwise 'item_head_ptr' and 'offset_ptr' will be set
  to indicate the SEEK end point (which is not the EXTRACT end point, this
  means that the offset will indicate the first byte that was copied).

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  We have successfully seeked to the right byte, and if we were doing
         an extract we copied ALL of the bytes indicated by 'len.'
           pkt_head_ptr: set to the item in which the seek ended
           offset_ptr: set to the offset in the item in which the seek ended
           len: if bytes were extracted this is set to the number that were

  FALSE: We have seeked past the end of the packet without finding the byte
         we are interesed in or we have seeked to the right place but not
         succeeded in copying all of the bytes.  Niether 'pkt_head_ptr' or
         'offset_ptr' are modified.  However:
             len: will be changed to reflect the number of bytes that were
                  copied


                                ***WARNING***
  Successful completion of this call WILL modify the first two arguments, and
  in all cases the last two arguments MAY be modified.
  

SIDE EFFECTS
  None
===========================================================================*/
boolean dsm_seek_extract
(
  dsm_item_type **item_head_ptr, /* Pointer to item head                   */
  word          *offset_ptr,     /* Pointer to offset item                 */
  void          *buf,            /* pointer to destination buffer          */
  word          *len             /* length in bytes of extraction          */
);


/*===========================================================================

FUNCTION DSM_PEEK_BYTE

DESCRIPTION
  This routine peeks at the value at the specified offset in a packet.

  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean dsm_peek_byte
(
  dsm_item_type* item_head_ptr,     /* Pointer to head of packet chain     */
  word           offset,            /* Offset into the packet chain        */
  uint8*          value             /* Pointer for storing value at a 
                                       particular offset                   */
);



#ifdef FEATURE_DS_DEBUG_UI
/*===========================================================================

FUNCTION DSM_GET_STATS

DESCRIPTION
 Copy DSM stats into the given array.

DEPENDENCIES
  The given array must contain at least DSM_STATS_COUNT word elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_get_stats
(
  word *stats_ptr                             /* pointer to array of stats */
);
#endif /* FEATURE_DS_DEBUG_UI */
                                                                             
/*===========================================================================

FUNCTION DSM_EMPTY_QUEUE

DESCRIPTION
  This routine completely empties a queue.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void dsm_empty_queue
( 
  dsm_watermark_type *water_mark         /* pointer to watermark to clean  */
);

/*===========================================================================

FUNCTION DSM_RETURN_SMALL_ITEM_FREE_CNT

DESCRIPTION
  Get number of free small items.
  This function should be used only for the DS SMALL ITEM Memory Pool.
  
DEPENDENCIES
  None
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

int2 dsm_return_small_item_free_cnt( void);


/*===========================================================================

FUNCTION DSM_RETURN_LARGE_ITEM_FREE_CNT

DESCRIPTION
  Get number of free small items.
  This function should be used only for the DS LARGE ITEM Memory Pool.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

int2 dsm_return_large_item_free_cnt( void);


/*===========================================================================

FUNCTION dsm_simple_enqueue_isr

DESCRIPTION
  This function will put the passed DSM item to the passed shared queue then
  check for and perform any 'put' events.  This function does not check
  for priority.  It simply enqueues to the tail of the queue.

DEPENDENCIES
  1) Both parameters must NOT be NULL.
  2) Does not support packet chaining.
  3) Should only be called from ISR or from within critical section in which
     interrupts are disabled.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_simple_enqueue_isr
(
  dsm_watermark_type *wm_ptr,     /* pointer to Watermark item to put to   */
  dsm_item_type      **item_ptr   /* pointer to item to load onto queue    */
);

#endif /* DSM_H */

