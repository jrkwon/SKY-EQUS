#ifndef AUDIOTEST_H
#define AUDIOTEST_H
/*===========================================================================

                      A U D I O   T E S T   H E A D E R

DESCRIPTION
  This file contains audio test information

  Copyright (c) 1999 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.07.28   jwpark  created
===========================================================================*/



#include "..\Qualcomm30\comdef.h"


typedef struct {
	uint16	VocoderRx;
	uint16	EQ;
	uint16	CODECRx;
	uint16	CODECTx;
	uint16	FM;
	uint16	HP;
	uint16	SPK;
	uint16  PCM;
} AudioTestGainType;


extern boolean				IsAudioGainTest;
extern AudioTestGainType	AudioTestGain;


#endif // #ifndef AUDIOTEST_H#i