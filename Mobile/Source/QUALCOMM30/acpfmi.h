#ifndef ACPFMI_H
#define ACPFMI_H
/*===========================================================================

                  ACP FM Interface (Internal Definitions)

DESCRIPTION
   This header file contains the definitions utilized internally by the
   FM Interface.

Copyright (c) 1991,1992,1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpfmi.h_v   1.0   Aug 11 2000 11:40:36   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/12/95   jca     Added Gemini target support.
08/09/94   jca     Added missing include.  Deleted unused constant.
07/14/92   ARH     Moved definition of size of free queue from acpfm.c
                   to this file.
12/17/91   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "queue.h"
#include "target.h"
#include "acp553.h"
#include "acpbch.h"

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

#if defined( T_MSM2P )
/* ---------------------------------------------------------------------
** Queue buffer type used for queuing each word of message data as it is
** received on the analog forward control or voice channel.
** --------------------------------------------------------------------- */
typedef struct
{
  q_link_type               link;
  acp553_rxword_type        rxword;
  acpbch_decode_status_type bch_status;
} acpfm_rxword_q_item_type;

#else

/* ----------------------------------------------------------------------
** Define a type to contain the five repeats: FOCC data words are 40 bits
** (5 bytes) as well as FVC data words.
** ---------------------------------------------------------------------- */
#define RX_WORD_MAX_BYTE_SIZE    5 /* 40 bits                          */
#define RX_NUM_REPEATS           5 /* Hold 5 repeats for majority vote */
typedef byte rx_repeat_words_type[RX_WORD_MAX_BYTE_SIZE][RX_NUM_REPEATS];

/* -----------------------------------------------------------------------
** Queue to hold the 5 word repeats of data coming in on forward channel.
** This data stucture is also referred to as the 'raw word' buffer.
** It contains the 5 repeats of a word (A or B) with the busy idle bit
** stripped out (See Forward Control Channel and Forward Voice Channel
** Message Stream format in Section 3.7 of EIA/TIA 553 or the equivalent).
** ----------------------------------------------------------------------- */
typedef struct
{
  q_link_type          link;       /* Link element */
  rx_repeat_words_type data;       /* Data element */
} acpfm_repeat_words_q_type;
#endif

#define ACPFM_RX_WORD_Q_BUF_NUM 5   /* Number of items in free queue */

extern q_type acpfm_word_q;         /* Receive Word Queue      */
extern q_type acpfm_word_free_q;    /* Receive Word Free Queue */

/* --------------------------------------------
** NOTE: 'q_puts' are done by the ACPWB module,
**       'q_gets' are done by the ACPFM module.
** -------------------------------------------- */

#endif /* ACPFMI_H */

