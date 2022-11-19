#ifndef _DSSDNS_H
#define _DSSDNS_H

#ifdef FEATURE_DS_SOCKETS
/*===========================================================================

   D A T A   S E R V I C E S   S O C K E T   A P I   H E A D E R   F I L E
                   
DESCRIPTION

 The Data Services Socket Header File. Contains shared variables and enums, 
 as well as declarations for functions.

Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/08/00   ttl     Added cashing table and refined the function structures.
05/22/00   ttl     Created module.

===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
//#include "psglobal.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#define TYPE_A      1                                      /* Host address */
#define TYPE_NS     2                                       /* Name server */
#define TYPE_MD     3                       /* Mail destination (obsolete) */
#define TYPE_MF     4                         /* Mail forwarder (obsolete) */
#define TYPE_CNAME  5                                    /* Canonical name */
#define TYPE_SOA    6                                /* Start of Authority */
#define TYPE_MB     7                       /* Mailbox name (experimental) */
#define TYPE_MG     8                  /* Mail group member (experimental) */
#define TYPE_MR     9                   /* Mail rename name (experimental) */
#define TYPE_NULL   10                              /* Null (experimental) */
#define TYPE_WKS    11                               /* Well-known sockets */
#define TYPE_PTR    12                                   /* Pointer record */
#define TYPE_HINFO  13                                 /* Host information */
#define TYPE_MINFO  14                /* Mailbox information (experimental)*/
#define TYPE_MX     15                                   /* Mail exchanger */
#define TYPE_TXT    16                                     /* Text strings */
#define TYPE_ANY    255                                /* Matches any type */



/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/



/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION  DSS_INIT_DNS_CASH_TBL

DESCRIPTION
  Initialize the DNS cashing table.
  
DEPENDENCIES
  None.

RETURN VALUE
  None. 

SIDE EFFECTS
  None.

===========================================================================*/
extern void dss_init_dns_cash_tbl(void);

/*===========================================================================

FUNCTION  RESOLVER

DESCRIPTION
  Sends domain name loop-up query to DNS server, and waits for the response
  from the DNS server. If the primary DNS server is unavailable, it tries
  to reach to the secondary DNS server for answer.
  
  This is a state machine w/ multiple entries. When this function is called,
  the status of the state determines what to do next. 
  
  STATES
  -----------
  DNS_INIT        Initial state. Creates of UDP socket.
  DNS_STARTING    Prepares domain name query and sends to the DNS server
                  via UDP socket.
  DNS_SENT        Socket has the query packet already. Waits until socket
                  deliver it out.
  DNS_WAIT        Waits for the DNS response. Sets a timer if needs.
  DNS_RCVD        Has the DNS response then go to DNS_CLOSE state. If not,
                  tries the secondary DNS server. If failed either, go to 
                  DNS_CLOSE. 
  DNS_CLOSE       Closes the socket. Calls the application callback function.
  
  Once the process is completed,  the application callback function is 
  called to inform the application that the answer is back. The application
  will call dss_gethostaddrs() again to get the answer.
  If the process is failed, the returned IP address is 0L and the 
  corresponding entry's status in the DNS cashing table will be mask as an
  error occured.

DEPENDENCIES
  None.

RETURN VALUE
  None. 

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 resolver(
  char    *domain_name, 
  void (*apps_dns_cb)(void)
);

#endif  /* FEATURE_DS_SOCKETS */

#endif  /* _DSSDNS_H */
