#ifndef QUEUE_H
#define QUEUE_H
/*===========================================================================

            Q U E U E    S E R V I C E S    H E A D E R    F I L E

DESCRIPTION
 This file contains types and declarations associated with the Queue
 Services.

Copyright (c) 1990,1991,1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999           by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/queue.h_v   1.2   04 Oct 1999 16:15:50   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/07/99    sk     Introduced q_last_get().
06/15/99   jct     Added extern "C" to allow this file to export the queue
                   routines to a C++ program.  Changed use of word to int to 
                   count the number of elements in a queue.  Upgraded comments
                   to include function headers.  Removed the inclusion of
                   comdef.h since it is no longer needed
04/09/99    sk     Comments changed.
04/06/99    sk     Introduced FEATURE_Q_NO_SELF_QPTR and FEATURE_Q_SINGLE_LINK
12/16/98   jct     Removed 80186 support
09/24/98   jct     Updated copyright
12/20/96   rdh     Changed to Pascal call convention.
02/08/96   jca     Added missing #include.
04/22/92   ip      Initial porting of file from Brassboard to DMSS.
02/20/92   arh     Added new function q_check.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef CUST_H
  #include "customer.h"
#endif


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* -------------------------------------------------------------------------
** Queue Link Structure
**
** The following link structure is really the heart of the Queue Services. 
** It is used as a link field with variables which allows them to be moved
** on and off queues. It is also used in the definition of queues themselves.
**
** Do NOT directly access the fields of a link! Only the Queue Services should
** do this.
** ------------------------------------------------------------------------- */
typedef struct q_link_struct
{
  struct q_link_struct  *next_ptr;
    /* Ptr to next link in list. If NULL, there is no next link. */

#ifndef FEATURE_Q_SINGLE_LINK

  struct q_link_struct  *prev_ptr; 

    /* Ptr to prev link in list. If NULL, there is no prev link. */
#endif

#ifndef FEATURE_Q_NO_SELF_QPTR

  void                  *self_ptr;
    /* Ptr to item which contains this link. */

  struct q_struct       *q_ptr;
    /* Ptr to the queue on which this item is enqueued. NULL if the
       item is not on a queue. While not strictly necessary, this field
       is helpful for debugging.*/
#endif

} q_link_type;

/* ------------------------------------------------------------------------
** Queue Head Link Structure
**
** When queue items are linked in a singly link list, q_head_link_type is
** used instead of q_link_type. This avoids the overhead of traversing 
** the whole of link list when queueing at the end of the link list.
** This structure should be accessed only by Queue services.
** ------------------------------------------------------------------------ */
typedef struct q_head_link_struct
{
  struct q_link_struct  *next_ptr;
    /* Ptr to head of the link list */

  struct q_link_struct  *prev_ptr; 
    /* Ptr to the tail of link list */

#ifndef FEATURE_Q_NO_SELF_QPTR

  void                  *self_ptr;
    /* Ptr to item which contains this link. */

  struct q_struct       *q_ptr;
    /* Ptr to the queue on which this item is enqueued. NULL if the
       item is not on a queue. While not strictly necessary, this field
       is helpful for debugging.*/
#endif
} q_head_link_type;


/* ------------------------------------------------------------------------
** Queue Structure
**
** The following structure format is used by the Queue Services to represent
** a queue.
** 
** Do NOT access the fields of a queue directly. Only the Queue Services should
** do this.
** ------------------------------------------------------------------------ */
typedef struct q_struct
{
#ifdef FEATURE_Q_SINGLE_LINK
  q_head_link_type  link;
#else
  q_link_type  link;
#endif
    /* Used for linking items into queue. */

  int          cnt;
    /* Keeps track of number of items enqueued. Though not necessary, having
       this field is tremendously helpful for debugging. */
} q_type;


/*===========================================================================

                            Function Declarations

===========================================================================*/
#ifdef __cplusplus
   extern "C" {
#endif

/*==========================================================================

FUNCTION Q_INIT

DESCRIPTION
  This function initializes a specified queue. It should be called for each
  queue prior to using the queue with the other Queue Services.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to the initialized queue.

SIDE EFFECTS
  The specified queue is initialized for use with Queue Services.

===========================================================================*/
q_type* q_init ( q_type *q_ptr );


/*===========================================================================

FUNCTION Q_LINK

DESCRIPTION
  This function initializes a specified link. It should be called for each
  link prior to using the link with the other Queue Services.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to the initialized link.

SIDE EFFECTS
  The specified link is initialized for use with the Queue Services.

===========================================================================*/
q_link_type* q_link ( void *item_ptr, q_link_type *link_ptr );


/*===========================================================================

FUNCTION Q_PUT

DESCRIPTION
  This function enqueues an item onto a specified queue using a specified
  link.

DEPENDENCIES
  The specified queue should have been previously initialized via a call
  to q_init. The specified link field of the item should have been prev-
  iously initialized via a call to q_init_link.

RETURN VALUE
  None.

SIDE EFFECTS
  The specified item is placed at the tail of the specified queue.

===========================================================================*/
void  q_put  ( q_type *q_ptr, q_link_type *link_ptr );


/*===========================================================================

FUNCTION Q_GET

DESCRIPTION
  This function removes an item from the head of a specified queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  A pointer to the dequeued item. If the specified queue is empty, then
  NULL is returned.

SIDE EFFECTS
  The head item, if any, is removed from the specified queue.

===========================================================================*/
void* q_get ( q_type *q_ptr );

/*===========================================================================

FUNCTION Q_LAST_GET

DESCRIPTION
  This function removes an item from the tail of a specified queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  A pointer to the dequeued item. If the specified queue is empty, then
  NULL is returned.

SIDE EFFECTS
  The head item, if any, is removed from the specified queue.

===========================================================================*/
void* q_last_get ( q_type *q_ptr );



/*===========================================================================

FUNCTION Q_CNT

DESCRIPTION
  This function returns the number of items currently queued on a specified
  queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  The number of items currently queued on the specified queue.

SIDE EFFECTS
  None.

===========================================================================*/
int q_cnt  ( q_type *q_ptr );


/*===========================================================================

FUNCTION Q_CHECK

DESCRIPTION
  This function returns a pointer to the data block at the head of the queue.
  The data block is not removed from the queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  A pointer to the queue item. If the specified queue is empty, then
  NULL is returned.

SIDE EFFECTS
  None

===========================================================================*/
void* q_check (q_type  *q_ptr);


/*===========================================================================

FUNCTION Q_NEXT

DESCRIPTION
  This function returns a pointer to the next item on the queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  A pointer to the next item on the queue. If the end of the queue is reached, 
  then NULL is returned.

SIDE EFFECTS
  None.

===========================================================================*/
void* q_next  ( q_type *q_ptr, q_link_type *link_ptr );


/*===========================================================================

FUNCTION Q_INSERT

DESCRIPTION
  This function inserts an item before a specified item on a queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Input item is inserted before input item.

===========================================================================*/
#ifdef FEATURE_Q_NO_SELF_QPTR
   void q_insert  ( q_type *q_ptr, q_link_type *q_insert_ptr, q_link_type *q_item_ptr );
#else
   void q_insert  ( q_link_type *q_insert_ptr, q_link_type *q_item_ptr );
#endif


/*===========================================================================

FUNCTION Q_DELETE

DESCRIPTION
  This function removes an item from a specified queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Input item is delete from the queue.

===========================================================================*/
#ifdef FEATURE_Q_NO_SELF_QPTR
   void q_delete  ( q_type *q_ptr, q_link_type *q_delete_ptr );
#else
   void q_delete  ( q_link_type *q_delete_ptr );
#endif


#ifdef __cplusplus
   }
#endif

#endif /* QUEUE_H */
