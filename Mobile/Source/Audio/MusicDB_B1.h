#ifndef MUSICDBB1_H
#define MUSICDBB1_H
/*===========================================================================

              M A 1   M E L O D Y   D A T A B A S E   H E A D E R

DESCRIPTION
  This file contains ma1 melody data

  Copyright (c) 1999 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.10.10   youandi delete A10 data and write MMF data for MA-2
00.07.25   jwpark  proting to coopper from nui-ori
00.07.13   jwpark  start porting to A10 from A7

===========================================================================*/

#include "sdbid_b1.h"
struct tagMUS_DATA_TYPE_T {
    const byte *pData;
    sint15      vol;
};

typedef struct tagMUS_DATA_TYPE_T MUS_DATA_TYPE_T;


const byte *MUSIC_get_music_data(MUS_ID_TYPE_E  MuiscID);
sint15 MUSIC_get_music_volume(MUS_ID_TYPE_E  MusicID);
#endif // #ifndef MUSICDB_H