#ifndef RFIFREQ_H
#define RFIFREQ_H

/* EJECT  */
/*===========================================================================

   R F   F r e q u e n c y   D e f i n i t i o n s   H e a d e r   F i l e

DESCRIPTION
  This header file defines relationship between channels and frequency for
  FM, CDMA, PCS, Korean PCS.  This header file is internal to the RF unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/rfifreq.h_v   1.1   10 Sep 1999 18:12:32   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/06/99   ychan   Modified for targets NGP, SURF, Q-1900.
01/06/99   ychan   Updated Copyright date.
02/10/98   thh     Added channel scheme for Korean PCS.
                   Removed frequency definitions dependency on supported RF
                   mode.
11/17/97   jjn     Partitioned ISS2 RF to conform to the new RF architecture
09/04/97   thh     Initial version.

===========================================================================*/
#include "rficap.h"

/* EJECT  */
/*===========================================================================

                          PUBLIC DEFINITIONS

===========================================================================*/

/* This table shows how FM/CDMA channel #, Tx frequency, Rx frequency,
** UHF synthesizer frequency are related:
**
**  BAND   CHANNEL   CHANNEL     Tx FREQ    Rx FREQ    UHF FREQ
**  NAME   NUMBER    OFFSET       (kHz)      (kHz)      (kHz)
**  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**   ?       990     unused      824,010    869,010    954,390
**  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**   A"      991         0       824,040    869,040    954,420
**   A"     1023 (0)    32       825,000    870,000    955,380
**  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**   A         1        33       825,030    870,030    955,410
**   A       333       365       834,990    879,990    965,370
**  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**   B       334       366       835,020    880,020    965,400
**   B       666       698       844,980    889,980    975,360
**  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**   A'      667       699       845,010    890,010    975,390
**   A'      716       748       846,480    891,480    976,860
**  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
**   B'      717       749       846,510    891,510    976,890
**   B'      799       831       848,970    893,970    979,350
**  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define RF_800MHZ_CHAN_MIN_FREQ   991 /* channel with min frequency     */
#define RF_800MHZ_CHAN_MAX_FREQ   799 /* channel with max frequency     */

#define RF_800MHZ_CHAN_LO_OFFSET  991 /* -offset for low channels       */
#define RF_800MHZ_CHAN_HI_OFFSET   32 /* +offset for high channels      */

#define RF_800MHZ_CHAN_SPN        832 /* channel span (total channels)  */

#define RF_FM_CHAN_MIN_FREQ       991 /* channel with min frequency     */
#define RF_FM_CHAN_MAX_FREQ       799 /* channel with max frequency     */

#define RF_FM_CHAN_LO_OFFSET      991 /* -offset for low channels       */
#define RF_FM_CHAN_HI_OFFSET       32 /* +offset for high channels      */

#define RF_FM_CHAN_SPN            832 /* channel span (total channels)  */

#define RF_CDMA_CHAN_MIN_FREQ     991 /* channel with min frequency     */
#define RF_CDMA_CHAN_MAX_FREQ     799 /* channel with max frequency     */

#define RF_CDMA_CHAN_LO_OFFSET    991 /* -offset for low channels       */
#define RF_CDMA_CHAN_HI_OFFSET     32 /* +offset for high channels      */

#define RF_CDMA_CHAN_SPN          832 /* channel span (total channels)  */

#if defined(RF_HAS_KPCS)
#error code not present
#else

/* This table shows how PCS channel #, Tx frequency, Rx frequency,
** UHF synthesizer frequency are related:
**
**  BAND   CHANNEL   Tx FREQ     Rx FREQ    UHF FREQ
**  NAME   NUMBER     (kHz)       (kHz)       (kHz)
**  - - - - - - - - - - - - - - - - - - - - - - - - -
**   A         0    1,930,000   1,850,000   1,719,620
**   A       299    1,944,950   1,864,950   1,734,570
**  - - - - - - - - - - - - - - - - - - - - - - - - -
**   D       300    1,945,000   1,865,000   1,734,620
**   D       399    1,949,950   1,869,950   1,739,570
**  - - - - - - - - - - - - - - - - - - - - - - - - -
**   B       400    1,950,000   1,870,000   1,739,620
**   B       699    1,964,950   1,884,950   1,754,570
**  - - - - - - - - - - - - - - - - - - - - - - - - -
**  HIGH/LOW BAND BOUNDARY
**  - - - - - - - - - - - - - - - - - - - - - - - - -
**   E       700    1,965,000   1,885,000   1,754,620
**   E       799    1,969,950   1,889,950   1,759,570
**  - - - - - - - - - - - - - - - - - - - - - - - - -
**   F       800    1,970,000   1,890,000   1,759,620
**   F       899    1,974,950   1,894,950   1,764,570
**  - - - - - - - - - - - - - - - - - - - - - - - - -
**   C       900    1,975,000   1,895,000   1,764,620
**   C      1199    1,989,950   1,909,950   1,779,570
**  - - - - - - - - - - - - - - - - - - - - - - - - -
**   ?      1200    1,990,000   1,910,000   1,779,620
**  - - - - - - - - - - - - - - - - - - - - - - - - - */

/* JSTD-008 PCS UHF min/mid/max/span channel definitions
*/
#define RF_PCS_CHAN_MIN_FREQ         0    /* channel == min frequency */
#define RF_PCS_CHAN_MAX_FREQ      1200    /* channel == max frequency */
#define RF_PCS_CHAN_SPN           1201    /* channel span             */

// MERGE CHANGE
#define RF_PCS_CHAN_MID_FREQ         ((word)700) /* channels < are low band  */
#define RF_PCS_CHAN_MID_FREQ_GRASP1  ((word)601) /* channels < are low band  */
//

#endif

#endif  /* RFIFREQ_H */

