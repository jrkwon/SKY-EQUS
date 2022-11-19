/*===========================================================================

                      A U D I O   T E S T   P R O G R A M

DESCRIPTION
  This file contains audio test program

  Copyright (c) 2000 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.12.15   jwpark  created
===========================================================================*/




#include <customer.h>
#include <comdef.h>
#include <uicommon.h>
#include <uistate.h>
#include <uixscrn.h>



#if (MODEL_ID == MODEL_A10)
#incoude "AudioTest.h"



///////////////////////////////////////////////////////////////////////////////
AudioTestGainType	audioTestGain;
static void			AudioDisplayGain(int inc);



///////////////////////////////////////////////////////////////////////////////
MENU_STATE_T AudioTestMenu(MENU_STATE_T state, UB keyin)
{
  static int	inc		 = 1;
  MENU_STATE_T	newState = state;

  switch(state)
	{
	case ENTRANCE_S:
	  AudioDisplayGain(inc);
	  newState = KEYIN_S;
	  break;
	case KEYIN_S:
	  {
		switch(keyin)
		  {
		  case DIGIT0_K: if (Inc == 1) Inc = 0x10; else Inc = 1; break;
		  case DIGIT1_K: audioTestGain.isAudioGainTest = FALSE; break;
		  case DIGIT3_K: audioTestGain.isAudioGainTest = TRUE;  break;
		  case DIGIT4_K:
			if((int)audioTestGain.vocoderRx - inc <= 0)
			  audioTestGain.vocoderRx  = 0;
			else
			  audioTestGain.vocoderRx -= inc;
			break;
		  case DIGIT6_K:
			if((int)audioTestGain.vocoderRx + inc <= 0)
			  audioTestGain.vocoderRx  = 0xFFFF;
			else
			  audioTestGain.vocoderRx += inc;
			break;
		  case DIGIT7_K:
			if(audioTestGain.codecRx == 0)
			  audioTestGain.vocoderRx = 0;
			else
			  audioTestGain.vocoderRx--;
			break;
		  case DIGIT9_K:
			if(audioTestGain.codecRx >= 0x7)
			  audioTestGain.vocoderRx = 0x7;
			else
			  audioTestGain.vocoderRx++;
			break;
		  case CLR_K:
		  case MEMORY_K: ui_PopUp(); break;
		  case END_K: NewState = MENU_EXIT_S; break;
		  }
		AudioDisplayGain(inc);
	  }
	}
  return NewState;
}



///////////////////////////////////////////////////////////////////////////////
static void AudioDisplayGain(int inc)
{
  static char audioTestText[NUM_LCD_LINES][NUM_LCD_COLUMNS];

  if(audioTestGain.isAudioGainTest)
	sprintf(audioTestText[1], "test \204\301  ON  \204\303 ");
  else
	sprintf(audioTestText[1], "test \204\301  OFF \204\303 ");
  sprintf(audioTestText[2], "voc  \204\304 %04x \204\306 ",
		  audioTestGain.vocoderRx);
  sprintf(audioTestText[3], "codec\204\307 %04x \204\311 ",
		  audioTestGain.codecRx);
  sprintf(audioTestText[4], " \204\300 inc : 0x%02x  ", inc);
  kj_ClearScreen();
  kj_SetTextY(0, (char *)" Audio Gain Test");
  kj_SetTextY(1, &audioTestText[1]);
  kj_SetTextY(2, &audioTestText[2]);
  kj_SetTextY(3, &audioTestText[3]);
  kj_TextOutY(4, &audioTestText[4]);
}



#else
MENU_STATE_T AudioTestMenu(MENU_STATE_T state, UB keyin)
{
  return MENU_EXIT_S;
}
#endif
