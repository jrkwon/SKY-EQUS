/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        Q U E U E    S E R V I C E S

GENERAL DESCRIPTION

  A queue is a simple data structure used for logically storing and re-
  trieving data blocks, in a first in - first out (FIFO) order without
  physically copying them. Software tasks and interrupt handlers can use
  queues for cleanly transferring data blocks to one another.

                +-------+     +-------+     +-------+
                | DATA  |     | DATA  |     | DATA  |
                | BLOCK |     | BLOCK |     | BLOCK |
  +-------+     +-------+     +-------+     +-------+
  | QUEUE |---->| LINK  |---->| LINK  |---->| LINK  |---->NULL
  +-------+     +-------+     +-------+     +-------+
  |       |     |       |     |       |     |       |
  +-------+     |       |     |       |     |       |
                +-------+     +-------+     +-------+


  The Queue Services provide a small set of declarations and functions for
  defining and initializing a queue, defining and initializing a links with-
  in a data block, placing a data block at the tail of a queue, removing a
  data block from the head of a queue, and removing a data block from any
  position in a queue.

  Aside from requiring each data block to contain a link, the Queue Services
  impose no restrictions on the size of structure of data blocks used with
  with queues. This allows software to pass virtually any type of data on
  queues. Notice that a data block may contain multiple links allowing it to
  be placed simultaneously on multiple queues.


EXTERNALIZED FUNCTIONS

  q_init
    This function initializes a queue. It should be called on behalf of a
    queue prior to using the queue.

  q_link
    This function initializes a link field. It should be called on behalf
    of a link field prior to using the link filed with the other Queue
    Services.


  q_check
    This function returns a pointer to the data block at the head of a queue.
    The data block is not removed from the queue. If the queue is empty, then 
    a NULL pointer is returned.

  q_last_get
    This function removes the data block at the tail of a queue and returns a
    pointer to the data block. If the queue is empty, then a NULL pointer
    is returned.

  q_put
    This function places a data block at the tail of a queue.

  q_cnt
    This function returns the number of items currently on a queue.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Prior to use, a queue must be initialized by calling q_init. Similarly,
  a link must be initialized prior to use by calling q_link.

Copyright (c) 1990-1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1996-1999 by QUALCOMM Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/queue.c_v   1.4   02 Mar 2001 07:22:04   nbouchar  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/22/01    ak     q_check now in IRAM.
07/07/99    sk     Introduced q_last_get().
06/15/99   jct     Changed type of queue count from word to int.
04/09/99    sk     Comments changed.
04/06/99    sk     Introduced FEATURE_Q_NO_SELF_QPTR and FEATURE_Q_SINGLE_LINK
12/16/98   jct     Removed 80186 support
10/08/98   jct     Moved INTLOCK in q_put to beginning of function, added
                   setting link pointer to NULL in q_delete
09/24/98   jct     Update copyright
12/20/96   rdh     Removed from Boot Block, changed to Pascal call convention.
04/08/96   dna     Added comment to each func. saying it is in the Boot Block
02/28/96   dna     Prepended bb_ to all function names and put module in the
                   Boot Block.
04/23/92   ip      Initial porting of file from Brassboard to DMSS.
02/20/92   arh     Added q_check routine

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "queue.h"

#ifdef CUST_H
 #include "customer.h"
#endif

/*==========================================================================

                FEATURE DEFINITIONS

FEATURE_Q_SINGLE_LINK is enabled to Queue all elements as a singly-linked
list. The head of the queue however maintains both forward and backward 
pointer.

FEATURE_Q_NO_SELF_QPTR is enabled to disable the use of self pointer and 
queue pointer in queue elements.

==========================================================================*/

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

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
q_type* q_init(
  q_type  *q_ptr  /* Ptr to queue to be initialized. */
)
{

  q_ptr->link.next_ptr = (q_link_type *)(&q_ptr->link);  /* Points to q link. */

  q_ptr->link.prev_ptr = (q_link_type *)(&q_ptr->link);  /* Points to q link. */

#ifndef FEATURE_Q_NO_SELF_QPTR

  q_ptr->link.self_ptr = NULL;  /* Null for queues. */

  q_ptr->link.q_ptr    = NULL;  /* Null for queues. */

#endif

  q_ptr->cnt = 0;  /* Mark queue as empty. */

  return ( q_ptr );  /* Simply return a pointer to the queue. */

} /* END q_init */


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
q_link_type* q_link(
  void         *item_ptr,  /* Ptr to item or variable containing link. */
  q_link_type  *link_ptr   /* Ptr to link field within variable. */
)
{

  link_ptr->next_ptr = NULL;       /* No next link. */

#ifndef FEATURE_Q_SINGLE_LINK

  link_ptr->prev_ptr = NULL;       /* No previous link. */

#endif

#ifndef FEATURE_Q_NO_SELF_QPTR

  link_ptr->self_ptr = item_ptr;   /* Link points to containing variable. */

  link_ptr->q_ptr    = NULL;       /* No queue yet. */

#endif

  return ( link_ptr );

} /* END q_link */

#ifndef MSM5000_IRAM_FWD
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
void q_put(
  q_type       *q_ptr,    /* Ptr to queue. */
  q_link_type  *link_ptr  /* Ptr to item link to use for queueing. */
)
{
#ifdef FEATURE_UTF_ASP
#error code not present
#endif

  INTLOCK( );  /* Disable interrupts. */

  link_ptr->next_ptr = (q_link_type *)&q_ptr->link;  /* Update item link fields. */

#ifndef FEATURE_Q_NO_SELF_QPTR

  link_ptr->q_ptr    = q_ptr;

#endif

#ifndef FEATURE_Q_SINGLE_LINK

  link_ptr->prev_ptr = q_ptr->link.prev_ptr;  /* Update remaining item
                                                 link field. */
#endif

  q_ptr->link.prev_ptr->next_ptr = link_ptr;  /* Update queue fields. */

  q_ptr->link.prev_ptr = link_ptr;

  q_ptr->cnt++;  /* Tally up a new item on queue. */

  INTFREE( );  /* Restore interrupts. */

} /* END q_put */
#endif /*MSM5000_IRAM_FWD*/

#ifndef MSM5000_IRAM_FWD
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
void* q_get(
  q_type  *q_ptr  /* Ptr to queue. */
)
{
  q_link_type  *link_ptr;  /* For returning value. */

#ifdef FEATURE_Q_NO_SELF_QPTR
  q_link_type  *ret_ptr=NULL;  /* For returning value. */
#endif


  INTLOCK( );  /* Disable interrupts. */

  link_ptr = q_ptr->link.next_ptr;  /* Get ptr to 1st queue item. */

  if ( q_ptr->cnt > 0 )  /* Is queue non-empty? */
    {

      q_ptr->link.next_ptr = link_ptr->next_ptr;  /* Update queue ptr. */

#ifdef FEATURE_Q_SINGLE_LINK

      if (link_ptr->next_ptr == (q_link_type *)q_ptr)
        {
          q_ptr->link.prev_ptr = (q_link_type *)(&q_ptr->link);
        }
#else

      link_ptr->next_ptr->prev_ptr = &q_ptr->link;  /* Update ptr of next item
                                                     in queue */
#endif

      q_ptr->cnt--;  /* Drop count of queued items. */

#ifdef FEATURE_Q_NO_SELF_QPTR

      link_ptr->next_ptr = NULL;  /* Mark item as no longer in a queue. */
      ret_ptr = link_ptr;

#else

      link_ptr->q_ptr = NULL;  /* Mark item as no longer in a queue. */

#endif 

  }

  INTFREE( );  /* Restore interrupts. */

#ifdef FEATURE_Q_NO_SELF_QPTR

  return (void *)ret_ptr;

#else
  return ( link_ptr->self_ptr );  /* Return ptr to dequeued item. */

#endif

} /* END q_get */
#endif /*MSM5000_IRAM_FWD*/

/*===========================================================================

FUNCTION Q_LAST_GET

DESCRIPTION            
  This function returns the item which was most recently enqueued in a queue.
  
  Note, this is different from q_get() which returns the oldest item in a 
  queue.

DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void * q_last_get
( 
  q_type* q_ptr           /* The queue from which the item will be removed */
)
{
   q_link_type  *link_ptr;                         /* For returning value. */
#if defined(FEATURE_Q_SINGLE_LINK) || defined(FEATURE_Q_NO_SELF_QPTR)
   q_link_type  *ret_ptr=NULL;                     /* For returning value. */
#endif  /* FEATURE_Q_SINGLE_LINK || FEATURE_Q_NO_SELF_QPTR */
 
 /*- - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - -*/
 
   INTLOCK( );  /* Disable interrupts. */
 
#ifdef FEATURE_Q_SINGLE_LINK
   for( link_ptr = (q_link_type *)q_ptr;link_ptr->next_ptr 
        != q_ptr->link.prev_ptr;link_ptr=link_ptr->next_ptr);

    if (q_ptr->cnt > 0)
     {
      ret_ptr = link_ptr->next_ptr;

      q_ptr->link.prev_ptr = link_ptr;
      
      link_ptr->next_ptr = (q_link_type *)(&q_ptr->link);
      q_ptr->cnt--;
#ifdef FEATURE_Q_NO_SELF_QPTR
      ret_ptr->next_ptr = NULL;  /* Mark item as no longer in a queue. */
#else
      link_ptr = ret_ptr;
      link_ptr->qptr = NULL;     /* Mark item as no longer in a queue. */
#endif
     }
#else
   link_ptr = q_ptr->link.prev_ptr;  /* Get ptr to queue item. */
 
   if ( q_ptr->cnt > 0 )  /* Is queue non-empty? */
   {
     q_ptr->link.prev_ptr = link_ptr->prev_ptr;  /* Update queue ptr. */
 
     link_ptr->prev_ptr->next_ptr = &q_ptr->link; /* Update ptr of next item
                                                     in queue. */
 
     q_ptr->cnt--;  /* Drop count of queued items. */
 
#ifdef FEATURE_Q_NO_SELF_QPTR
     link_ptr->next_ptr = NULL;  /* Mark item as no longer in a queue. */
     ret_ptr = link_ptr;
#else
     link_ptr->q_ptr = NULL;  /* Mark item as no longer in a queue. */
#endif
   }
#endif
 
   INTFREE( );  /* Restore interrupts. */
 
#ifdef FEATURE_Q_NO_SELF_QPTR
   return  (void *)ret_ptr;  /* Return ptr to dequeued item. */
#else
   return ( link_ptr->self_ptr );  /* Return ptr to dequeued item. */
#endif
}  /* q_last_get */

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
void* q_next(
  q_type  *q_ptr,            /* Ptr to queue. */
  q_link_type  *q_link_ptr  /* Ptr to current link. */
)
{

  if ( (void *) q_link_ptr->next_ptr != (void *) q_ptr )
    {
      return (q_link_ptr->next_ptr );
    }
  else
    {
      return ( NULL );
    }

} /* END q_next */


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
void q_insert(
#ifdef FEATURE_Q_NO_SELF_QPTR
  q_type       *q_ptr,          /* Ptr to the queue */
#endif
  q_link_type  *q_insert_ptr,   /* Ptr to link of item to insert */
  q_link_type  *q_item_ptr      /* Ptr to link item to insert before */
)
{
#ifdef FEATURE_Q_SINGLE_LINK
  q_link_type  *link_ptr;
#endif
  
  INTLOCK( );  /* Disable interrupts. */

  q_insert_ptr->next_ptr = q_item_ptr;

#ifdef FEATURE_Q_SINGLE_LINK

  for ( link_ptr = (q_link_type *)q_ptr; 
        link_ptr->next_ptr != q_item_ptr;
        link_ptr = link_ptr->next_ptr )

      { }
      

  link_ptr->next_ptr = q_insert_ptr;

#else

  q_insert_ptr->prev_ptr = q_item_ptr->prev_ptr;

  q_item_ptr->prev_ptr->next_ptr = q_insert_ptr;

  q_item_ptr->prev_ptr = q_insert_ptr;

#endif


#ifdef FEATURE_Q_NO_SELF_QPTR

 q_ptr->cnt++;  /* Tally up a new item on queue. */

#else

  q_insert_ptr->q_ptr = q_item_ptr->q_ptr;
  q_item_ptr->q_ptr->cnt++;  /* Tally up a new item on queue. */

#endif  

  INTFREE( );  /* Restore interrupts. */

  return;
} /* END q_insert */


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
void q_delete(
#ifdef FEATURE_Q_NO_SELF_QPTR
  q_type       *q_ptr,          /* Ptr to the Queue */
#endif
  q_link_type  *q_delete_ptr   /* Ptr to link of item to delete */
)
{

#ifdef FEATURE_Q_SINGLE_LINK

  q_link_type *link_ptr;

#endif

  INTLOCK( );  /* Disable interrupts. */

#ifdef FEATURE_Q_SINGLE_LINK

  for ( link_ptr=(q_link_type *)q_ptr;
        link_ptr->next_ptr != q_delete_ptr; 
        link_ptr=link_ptr->next_ptr)
        
        { }

  link_ptr->next_ptr = q_delete_ptr->next_ptr;
  
  if (link_ptr->next_ptr == (q_link_type *)q_ptr)
     {
      q_ptr->link.prev_ptr = link_ptr;
     }

#else

  q_delete_ptr->prev_ptr->next_ptr = q_delete_ptr->next_ptr;
  q_delete_ptr->next_ptr->prev_ptr = q_delete_ptr->prev_ptr;


#endif

#ifdef FEATURE_Q_NO_SELF_QPTR

  q_ptr->cnt--;  /* One less item on queue. */
  q_delete_ptr->next_ptr = NULL;

#else

  q_delete_ptr->q_ptr->cnt--;  /* One less item on queue. */
  q_delete_ptr->q_ptr = NULL;  /* Mark item as no longer in a queue. */

#endif

  INTFREE( );  /* Restore interrupts. */

  return;
} /* END q_delete */


#ifndef MSM5000_IRAM_FWD
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
void* q_check(
  q_type  *q_ptr  
            /* Ptr to queue. */
)
{
  q_link_type  *link_ptr;  /* For returning value. */

#ifdef FEATURE_Q_NO_SELF_QPTR
  q_link_type  *ret_ptr=NULL;  /* For returning value. */
#endif

  INTLOCK( );  /* Disable interrupts. */

  link_ptr = q_ptr->link.next_ptr;  /* Get ptr to 1st queue item. */

#ifdef FEATURE_Q_NO_SELF_QPTR

  if (q_ptr->cnt >0)
     ret_ptr = link_ptr;

#endif

  INTFREE( );  /* Restore interrupts. */

#ifdef FEATURE_Q_NO_SELF_QPTR

  return  (void *)ret_ptr ;  /* Return ptr to head item. */

#else

  return ( link_ptr->self_ptr );  /* Return ptr to head item. */

#endif

} /* END q_check */
#endif /*MSM5000_IRAM_FWD*/


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
int q_cnt(
  q_type  *q_ptr  /* Ptr to queue. */
)
{
  return ( q_ptr->cnt );

} /* END q_cnt */
