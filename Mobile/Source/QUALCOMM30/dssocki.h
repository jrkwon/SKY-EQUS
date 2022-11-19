#ifndef DS_SOCKI_H
#define DS_SOCKI_H

/*===========================================================================

          S O C K E T S  I N T E R N A L  H E A D E R  F I L E
                   
DESCRIPTION

 The Data Services Internal Socket Header File. Contains shared variables and 
 enums, as well as declarations for functions for each protocol.


Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 2000, 2001 by QUALCOMM, Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/dssocki.h_v   1.5   14 Feb 2001 15:19:16   jeffsong  $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/14/00    js     Added definitions to support socket options. 
02/08/01    yll    Added read_dsm_chain in the dss_fcn_ptr_table.
11/27/00    na     Cleanup. Packed acb_type and struct scb_s better to save
                   RAM.
10/30/00    snn    Removed define for SOCKFDBASE( 0x1000)  and APPIDBASE 
                   (0x2000) because global variables will be used in their
                   place.
                   Renamed MAX_SOCKS to DSS_MAX_SOCKS. and MAX_APPS 
                   to DSS_MAX_APPS.
                   Renamed scb_type as scb_s, to follow naming convention.
                   Rearranged elements in some structures to pack the data
                   properly.
10/10/00    snn    Moved MAX_SOCKS and MAX_APPS from here to dssocket.h for
                   cleaner interface, because they are updated by customers.
09/03/00    snn    Made changes to support multiple applications.
08/03/00    mvl/js Added a function pointer to the socket function pointer
                   table definition to support protocol specific bind(). 
07/27/00    mvl    Backed out the MAX_APPS change as there was no support for
                   multiple applications using the library.
05/11/00    mvl    Modified MAX_SOCKS to support multiple sockets and
                   MAX_APPS to support as many applications as there are
                   sockets.
08/11/99    ak/rc  Added abort to fcn ptr table, to take care of closing
                   socket when no network.
04/21/99    hcg    Updated parameter to dssocki_send_to_ps(), to conform to 
                   new kind field sizing in dsm_item_type.
04/05/99    hcg    Incorporated code review changes and comments.  Removed 
                   external declaration of dssocki_convert_ntoh() as its 
                   functionality is now provided through macros.
10/06/98    na/hcg Created module.
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATURE_DS_SOCKETS
#include "customer.h"
#include "dsm.h"
#include "psglobal.h"
#include "tcp.h"
#include "netuser.h"
#include "ip.h"
#include "dssocket.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/*---------------------------------------------------------------------------
  Socket allocation constants.
---------------------------------------------------------------------------*/
#define SOCKAVAIL            0                   /* socket is free for use */
#define SOCKFDMAX       0x1FFF                    /* socket descriptor max */
#define APPAVAIL             0              /* application is free for use */
#define APPIDMAX        0x2FFF                    /* socket descriptor max */
#define PORTBASE        0x8000                      /* ephemeral port base */

/*---------------------------------------------------------------------------
  Socket Cleanup Timer Intervals
---------------------------------------------------------------------------*/
#define DSS_SHORT_CLOSE_INTERVAL   30       /* TCP closed cleanup interval */
#define DSS_LONG_CLOSE_INTERVAL  1000    /* TCP time wait cleanup interval */

/*---------------------------------------------------------------------------
                     Socket Option Data Structures
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  Allow enough space for the largest possible option value type. 
---------------------------------------------------------------------------*/
typedef union
{
  int integer;                      
}dssocki_optval_types_type;

/*---------------------------------------------------------------------------
  Date types for option values. When an entry for a specified option is 
  referenced from the options table, the dss_sockopt_data_type field of the 
  entry reveals the option's data type. 
---------------------------------------------------------------------------*/
typedef enum
{
  DSSOCKI_SOCKOPT_INT                       /* integer type                */
}dssocki_sockopt_data_type;

/*---------------------------------------------------------------------------
  Encapsulation of socket option. Fields such as option name, level and
  option value type are not to be changed once initilzed. 
---------------------------------------------------------------------------*/
typedef struct
{
  dssocki_optval_types_type optval;       /* option value                  */
  dss_sockopt_levels_type level;          /* option level                  */
  uint32 optlen;                          /* legth of the option value     */
  int optname;                            /* option name                   */
  dssocki_sockopt_data_type optvaltype;   /* option type                   */
}dssocki_sockopt_type;

/*---------------------------------------------------------------------------
  Enum for socket state.
---------------------------------------------------------------------------*/
typedef enum
{
  DSSOCKI_NULL =0,                                          /* empty state */
  DSSOCKI_INIT,                             /* TCP socket has been created */
  DSSOCKI_OPENING,                       /* TCP is attempting a connection */
  DSSOCKI_OPEN,     /* TCP connection established, DGRAMS open for writing */
  DSSOCKI_CLOSING,                                   /* TCP issued a close */
  DSSOCKI_WAIT_FOR_CLOSE,                           /* TCP close completed */
  DSSOCKI_CLOSED                          /* connection or network failure */
} dssocki_state_enum_type;

/*---------------------------------------------------------------------------
  Enum for type of caller function
---------------------------------------------------------------------------*/
typedef enum 
{
  DSSOCKI_CONNECT_ROUTINE =0,
  DSSOCKI_CLOSE_ROUTINE,
  DSSOCKI_IO_ROUTINE
} dssocki_caller_enum_type;

/*---------------------------------------------------------------------------
  Application control block structure.
---------------------------------------------------------------------------*/
typedef struct
{
  void (*net_callback_fcn)(void *);          /* network event callback fcn */
  void (*socket_callback_fcn)(void *);        /* socket event callback fcn */
  sint15 app_id;                     /* ACB's corresponding application ID */
  boolean net_event_active;          /* flag used for calling net callback */
  boolean ppp_active;                         /* PPP connection is needed  */
  boolean notified_cb_flag;             /* Socket callback has been called */
} acb_type;

/*---------------------------------------------------------------------------
  protocol control block union type - contains pointers to protocol 
  specific control blocks
---------------------------------------------------------------------------*/
typedef union 
{
  struct tcb *tcb;                                                  /* TCP */
  struct udp_cb *ucb;                                               /* UDP */
  struct raw_ip *rcb;                                            /* Raw IP */
  void *p;                                                       /* dunno? */
} protocol_ctl_block_type;

/*---------------------------------------------------------------------------
  socket control block structure.  
---------------------------------------------------------------------------*/
struct scb_s
{
  acb_type *acb_ptr;          /* Ptr to application ctl block for this scb */
  struct dss_fcn_ptr_table *fcn_ptr_table;  /* protocol function ptr table */
  protocol_ctl_block_type protocol_ctl_blk;      /* protocol control block */
  sint31 requested_event_mask;       /* Events the socket is interested in */
  sint31 event_happened_mask;                 /* Events that have occured  */
  struct socket localname;                                    /* host name */
  sint15 closed_state_reason;      /* reason socket went into CLOSED state */
  struct socket servname;                                   /* server name */  
  sint15 sockfd;                  /* SCB's corresponding socket descriptor */  
  dssocki_state_enum_type socket_state;             /* state of the socket */
  byte family;                                           /* address family */
  byte type;                                                /* socket type */
  boolean data_available;                             /* Something to read */
  byte protocol;                                          /* protocol type */
  dssocki_sockopt_type dss_sock_opts[DSS_SOCKOPT_MAX];    /* sockopt table */
};

/*---------------------------------------------------------------------------
  protocol specific function pointer table - contains function pointers
  to "common" socket functions, that are implemented differently, depending
  on the protocol being used.
---------------------------------------------------------------------------*/
struct dss_fcn_ptr_table 
{
  int type;                                                 /* Socket type */
  sint15 (*socket)(struct scb_s *scb_ptr, sint15 *errno); /* allocate sckt */
  sint15 (*connect_setup)(struct scb_s *scb_ptr);/* set-up connection      */
  sint15 (*connect)(struct scb_s *scb_ptr); /* actually perform connection */
  sint15 (*write)(protocol_ctl_block_type pcb,      
                  dsm_item_type *dsm_item);         /* byte output routine */
  sint15 (*read)( struct scb_s* scb_ptr, byte* buffer,   
                  uint16 nbytes, struct sockaddr_in *fromaddr, 
                  sint15* errno);                    /* byte input routine */
  sint15 (*read_dsm_chain)( struct scb_s* scb_ptr,   /* dsm item input routine */ 
                            dsm_item_type **item_ptr,   
                            struct sockaddr_in *fromaddr, 
                            sint15* errno);
  sint15 (*close)(struct scb_s *scb_ptr, sint15 *errno); /* close/clean con*/
  sint15 (*abort)(struct scb_s *scb_ptr, sint15 *errno); /* abort/clean con*/
  void (*bind)(struct scb_s *scb_ptr, struct sockaddr_in *la); /* bind sock*/ 
};

/*---------------------------------------------------------------------------
  protocol specific function pointer table - array of function tables,
  on a per protocol basis.
---------------------------------------------------------------------------*/
extern struct dss_fcn_ptr_table dssock_fcn_ptr_table[];

extern struct scb_s scb_array[DSS_MAX_SOCKS]; /* socket ctl block array    */

extern acb_type dssocki_acb_array[DSS_MAX_APPS];

/*---------------------------------------------------------------------------
  Ranges of sockfd and sockfdbase:   
  NOTE THAT THE  SOCKFDBASE_MIN_VAL SHOULD NOT EQUAL ZERO.
  Note these should be within range of sockfdbase.
---------------------------------------------------------------------------*/
#define SOCKFDBASE_MIN_VAL   100
#define SOCKFDBASE_MAX_VAL  20000 

/*---------------------------------------------------------------------------
  Application Identifier Base.                            
   NOTE THAT THE  APPIDBASE_MIN_VAL SHOULD NOT EQUAL ZERO.  
   Note, these should be within range of appidbase type.
---------------------------------------------------------------------------*/
#define APPIDBASE_MIN_VAL  100
#define APPIDBASE_MAX_VAL  1000 


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION DSSOCKI_ALLOCSCB

DESCRIPTION
  Allocates socket control block from SCB array.  Calculates socket
  descriptor and sets it in the SCB.

DEPENDENCIES
  None.

RETURN VALUE
  Returns pointer to socket control block structure.  If none are
  available, returns NULL.

SIDE EFFECTS
  None.

===========================================================================*/
extern struct scb_s *dssocki_allocscb
(
  void
);

/*===========================================================================

FUNCTION DSSOCKI_FREESCB

DESCRIPTION
  Frees a socket control block in the SCB array for re-use.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void dssocki_freescb
(
  struct scb_s *scb_ptr           /* ptr to socket control block structure */
);

/*===========================================================================

FUNCTION DSSOCKI_SOCKFDTOSCB

DESCRIPTION
  Given a socket descriptor, returns a socket control block from SCB array.  

DEPENDENCIES
  None.

RETURN VALUE
  Returns pointer to socket control block structure.

SIDE EFFECTS
  None.

===========================================================================*/
extern struct scb_s *dssocki_sockfdtoscb
(
  int sockfd                                          /* socket descriptor */
);

/*===========================================================================

FUNCTION DSSOCKI_ALLOCACB

DESCRIPTION
  Allocates application control block from ACB array.  Assigns application
  ID and sets the network and socket callback functions.

DEPENDENCIES
  None.

RETURN VALUE
  Returns pointer to application control block structure.  If none are
  available, returns NULL.

SIDE EFFECTS
  None.

===========================================================================*/
extern acb_type *dssocki_allocacb
(
  void
);

/*===========================================================================

FUNCTION DSSOCKI_FREEACB

DESCRIPTION
  Frees an application control block in the ACB array for re-use.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void dssocki_freeacb
(
  acb_type *acb_ptr          /* ptr to application control block structure */
);

/*===========================================================================

FUNCTION DSSOCKI_APPIDTOACB

DESCRIPTION
  Given an application ID, returns an application control block from ACB 
  array.  

DEPENDENCIES
  None.

RETURN VALUE
  Returns pointer to application control block structure.

SIDE EFFECTS
  None.

===========================================================================*/
extern acb_type *dssocki_appidtoacb
(
  sint15 app_id                                          /* application ID */
);

/*===========================================================================

FUNCTION DSSOCKI_GETACBCOUNT

DESCRIPTION
    Gets the current number of allocated application control blocks in the 
    system.

DEPENDENCIES
  None.

RETURN VALUE
  Returns number of allocated applications in ACB array.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dssocki_getacbcount
(
  void
);

/*===========================================================================

FUNCTION DSSOCKI_GETEMPHEM_PORT

DESCRIPTION
  Generates a client-side ephemeral port between 0x8000 and 0x8FFFF and
  sets it in the socket control block.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern uint16 dssocki_getephem_port
(
  struct scb_s *scb_ptr           /* ptr to socket control block structure */
);

/*===========================================================================

FUNCTION DSSOCKI_FCNPTR_SETUP

DESCRIPTION
  Sets up the protocol specific function pointer table.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Function pointer table is assinged.

===========================================================================*/
extern sint15 dssocki_fcnptr_setup
(
  struct scb_s *sock_ptr,                /* socket control block structure */ 
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSSOCKI_SEND_TO_PS

DESCRIPTION
  Sends the application's dsm_item data to the PS task, which will send it 
  out using the protocol for the given socket.
  
  This function is called from the context of the socket application task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void dssocki_send_to_ps
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  dsm_item_type* item_ptr,                    /* Ptr to payload to be sent */
  byte kind                                 /* Type of item to be enqueued */
);

/*===========================================================================

FUNCTION DSSOCKI_COPY_BUF_TO_DSM

DESCRIPTION
  Copies application data buffer vector to data services memory pool.

DEPENDENCIES
  None.

RETURN VALUE
  pointer to dsm_item_type

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dssocki_copy_buf_to_dsm
(
  struct iovec *iov,      /* array of data buffers from which to copy data */
  sint15 iovcount,                                /* number of array items */
  dsm_item_type **buf_ptr_ptr          /* pointer to dsm item to copy into */
);

/*===========================================================================

FUNCTION DSSOCKI_SET_SOCK_STATE

DESCRIPTION
  This function sets the socket state. It can be NULL, INIT, OPENING, OPEN, 
  CLOSING, WAIT_FOR_CLOSE, or CLOSED.
  
  NULL: Is a pseudo-state, that is set prior to initial socket creation.
        A NULL socket cannot be accessed by any API function.

  INIT: Is specific to TCP, and is set when a TCP socket is first created,
        through a call to socket().  Note this is an invalid state for 
        UDP sockets.
  
  OPENING: the socket is not writable and read() and write() would
           return DS_EWOULDBLOCK.
           A connected socket is in the OPENING state after dss_connect() 
           has been called and before the socket is completly connected.
           A connectionless socket is never in this state.
  
  OPEN:    means the socket is writable if there is enough memory available.
           A connected socket is in the OPEN state when TCP is in the 
           ESTABLISHED state.
           A connectionless socket is in OPEN state in the dss_socket() 
           call.
  
  CLOSED:  means the socket is writable and readable  and read() and write() 
           would return DS_ECONNABORTED, DS_ECONNRESET, DS_IPADDRCHANGED or
           DS_ENETDOWN. A connected socket is in the CLOSED state after it 
           lost its connection to the server or the network connectivity was
           lost any time after the dss_connect() was called. 
           A connectionless socket is in this state, only when the network
           subsystem has experienced a failure.
  
  CLOSING: means that a TCP socket has issued a close(), but the TCP close
           is still in progress.  The initial call to close() for TCP 
           sockets would return DS_EWOULDBLOCK.  This corresponds to the 
           active close, and the socket is waiting for TCP to fully 
           negotiate the close.
           This state is invalid for connectionless sockets.

  WAIT_FOR_CLOSE:  means that TCP has fully closed, or the TCP close timer
                   has expired.  In this state, the application may call
                   close() again to clean up the socket and free it for
                   re-use.
                   This state is invalid for connectionless sockets.

  This should be called whenever the state changes and is the only function
  which modifies the socket_state member of the scb_s.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void dssocki_set_sock_state
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  dssocki_state_enum_type new_state                        /* Socket state */
);

/*===========================================================================

FUNCTION DSSOCKI_NET_EVENT_NOTIFY

DESCRIPTION
  This function notifies the application that the network event of interest
  has occurred. If the passed parameter is NULL then it notifies all the 
  opened   applications. If passed parameter is a valid one, it notifies
  only that application.
  
DEPENDENCIES 
    It may be a good practice to check that the curr_acb_ptr being passed 
    is a valid one by the function calling this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void dssocki_net_event_notify
(
    acb_type* curr_acb_ptr /* ACB to be notiifed */
);

/*===========================================================================

FUNCTION DSSOCKI_SOCKET_EVENT_CHECK

DESCRIPTION
  This function performs a real-time check, to determine if any of the
  socket events of interest have occurred.  If so, so returns the
  event mask of the events that have occurred.

DEPENDENCIES
  None.

RETURN VALUE
  sint31 event mask of the socket events that have occurred.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint31 dssocki_socket_event_check
( 
  struct scb_s* scb_ptr           /* ptr to socket control block structure */
);

/*===========================================================================

FUNCTION DSSOCKI_NOTIFY_APP

DESCRIPTION
  This function calls the application callback function to alert the
  application that a socket event has occurred.  Note that the application
  callback will not be called if the application has already been notified.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The application callback function is invoked.

===========================================================================*/
extern void dssocki_notify_app
( 
  acb_type *acb_ptr                    /* ptr to application control block */
);

/*===========================================================================

FUNCTION DSSOCKI_SOCKET_EVENT_OCCURRED

DESCRIPTION
  This function notifies all interested sockets that a socket event 
  has occurred.  If a NULL scb_ptr is specified, the function assumes it
  should provide notification for all sockets.  The function then loops
  through the socket control block array and checks each socket for events
  that may have occurred.  The application is notified upon any events
  having occurred.  If a valid scb_ptr is specified, the function checks
  that particular socket for any events that may have occurred.  Again, 
  the application is notified if any of the events that it is interested
  in, have occurred. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void dssocki_socket_event_occurred
( 
  struct scb_s *scb_ptr                     /* ptr to socket control block */
);

/*===========================================================================

FUNCTION DSSOCKI_SOCK_SET_STATE_CLOSED

DESCRIPTION
  This function sets the socket state to DSSOCKI_CLOSED, however, only if 
  the current socket state is set to DSSOCKI_OPEN or DSSOCKI_OPENING.  If a 
  NULL parameter is passed in, it will change all DSSOCKI_OPEN or 
  DSSOCKI_OPENING sockets to DSSOCKI_CLOSED.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Sets socket state to DSSOCKI_CLOSED.
===========================================================================*/
extern void  dssocki_sock_set_state_closed
(
  struct scb_s *scb_ptr           /* ptr to socket control block structure */
);

/*===========================================================================

FUNCTION DSSOCKI_SET_CLOSED_STATE_REASON

DESCRIPTION
  Sets the closed_state_reason field in the SCB with the appropriate
  reason for the socket transitioning into the CLOSED state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Sets the closed_state_reason field.
===========================================================================*/
extern void  dssocki_set_closed_state_reason
(
  struct scb_s *scb_ptr,          /* ptr to socket control block structure */
  sint15   reason                /* reason for socket tranistion to CLOSED */
);

/*===========================================================================

FUNCTION DSSOCKI_GET_ERR_VAL

DESCRIPTION
  Gets the appropriate error value based on current socket state and 
  type of operation (connect(), close(), read(), write()).  This function
  follows the TCP/UDP socket state machines.  

DEPENDENCIES
  None.

RETURN VALUE
  Error condition.

SIDE EFFECTS
  None.
===========================================================================*/
extern sint15 dssocki_get_err_val
(
  struct scb_s *scb_ptr,               /* ptr to socket control block type */
  dssocki_caller_enum_type type,   /* type of function that is calling this
                                      function.                            */
  sint15 *errno                                   /* error condition value */
);
/*==========================================================================

FUNCTION DSSOCKI_NUM_ACTIVE_APPS

DESCRIPTION
 Returns the number of active applications currently opened.

DEPENDENCIES
  None.

RETURN VALUE
  Returns number of the active applications currently opened. This 
  Should be less than MAX_APPS

SIDE EFFECTS
  None.

===========================================================================*/

extern byte dssocki_num_active_apps (void);

/*===========================================================================

FUNCTION DSSOCKI_PPP_IN_USE

DESCRIPTION
 This function checks all the ppp_active flags in the ACB and returns if 
 any of the applications need PPP connection. This function can be called
 to check if any active applications need PPP.

DEPENDENCIES
  None.

RETURN VALUE
  Returns TRUE if any active application needs PPP Connection.
  Returns FALSE if none of the applications need PPP connection.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean dssocki_ppp_in_use(void);

/*===========================================================================

FUNCTION DSSOCKI_INDEXTOSCB

DESCRIPTION
  Convert the index of scb_array to the address.
  
  This function will take the index of the scb_array as the input. If the 
  sockfd is valid for this index in the scb_array, i.e. Not SOCKAVAIL, then 
  we return the address of that element of array. 
  If the sockfd equals SOCKAVAIL, then we return a NULL.
  
DEPENDENCIES
  None.

RETURN VALUE
  Returns pointer to socket control block structure.

SIDE EFFECTS
  None.

===========================================================================*/
extern struct scb_s *dssocki_indextoscb (int i);

#endif /* FEATURE_DS_SOCKETS */

#endif /* DS_SOCKI_H */

