/*===========================================================================

                   Y A M A H A   M E L O D Y  D R I V E R

DESCRIPTION
  This file contains functions of the YAMAHA FM synthesizer for the melody
  sound. 

  Copyright (c) 1999 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who      what, where, why
--------   ---      ----------------------------------------------------------
00.08.16   jwpark   YMU757B new API for music on EFS
00.08.14   jwpark   YMU757B new API
00.07.24   jwpark   start porting to cooper from nui-ori
00.07.12   jwpark   start proring to A10 from A7
           happykid A7 yamaha.c

===========================================================================*/




/////////////////////////////////////////////////////////////////////////////////
#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "err.h"
#include "bio.h"
#include "clk.h"
#include "tramp.h"
#include "task.h"
#include "snd.h"
#include "sndi.h"
#include "sndihw.h"


/////////////////////////////////////////////////////////////////////////////////
#ifdef FEATURE_YMU757B_MELODY // jwpark 00.07.24
///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "YMU757B.h"
#include "MusicDb.h"
#include "AudioTest.h"	
#include "mmedia.h"




//////////////////////////////////////////////////////////////////////////////
// variables for sleep check : SND_ALLOW_SLEEPING()
extern boolean snd_tone_awake;         // TRUE If Generating a Tone
extern boolean snd_voc_acquired;       // TRUE If Vocoder Acquired
extern boolean snd_need_voc_acquire;   // TRUE If WANT to Vocoder Acquire
extern boolean Codec_sleep_enable;     // TRUE if CODEC allow sleeping
       boolean MUSIC_sleep_enable;     // TRUE If YMU757B really off
extern boolean snd_IsMusicPlaying;




///////////////////////////////////////////////////////////////////////////////
// for MUSIC_MakeTimbreData() & MUSIC_compressTimbre()
//
typedef struct {
  byte	wave;
  byte	multi;
  byte	tl;
  byte	fl;
  byte	vib;
  byte	egt;
  byte	ar;
  byte	dr;
  byte	sl;
  byte	rr;
  byte	sus;
} basic_Timbre_data;
typedef struct {
  basic_Timbre_data	Mod_T;
  basic_Timbre_data	Car_T;
} Timbre_data_type;




///////////////////////////////////////////////////////////////////////////////
// for MUSIC_MakeTimbreData() & MUSIC_compressTimbre()
//
static const Timbre_data_type GMIDI_timbre[128] = {
  //	voice  1	PIANO
  {{0,	1,	10,	0,	0,	0,	14,	14,	3,	5,	0},
   {0,	1,	0,	0,	1,	0,	14,	5,	3,	5,	0}},
  //	voice  2	BRITE PIANO
  {{0,	3,	24,	0,	0,	0,	14,	14,	2,	5,	0},
   {0,	1,	0,	0,	1,	0,	14,	5,	3,	5,	0}},
  //	voice  3	E GRAND PIANO
  {{0,	3,	19,	6,	1,	0,	14,	14,	2,	5,	0},
   {0,	1,	0,	0,	0,	0,	14,	7,	10,	1,	0}},
  //	voice  4	HONKY-TONK
  {{0,	1,	17,	5,	1,	1,	15,	5,	3,	14,	0},
   {0,	1,	0,	0,	0,	0,	15,	3,	0,	7,	0}},
  //	voice  5	ELE PIANO 1
  {{0,	1,	20,	7,	0,	0,	14,	14,	3,	5,	0},
   {0,	1,	0,	0,	1,	0,	14,	5,	2,	5,	0}},
  //	voice  6	ELE PIANO 2
  {{0,	7,	22,	7,	1,	0,	14,	14,	3,	5,	0},
   {0,	1,	0,	0,	0,	0,	14,	5,	2,	5,	0}},
  //	voice  7	HARPSICHORD
  {{0,	6,	22,	7,	0,	1,	13,	6,	9,	10,	0},
   {1,	2,	0,	0,	1,	1,	13,	7,	15,	15,	0}},
  //	voice  8	CLAVI
  {{0,	1,	18,	5,	1,	1,	15,	5,	10,	11,	0},
   {1,	1,	0,	0,	0,	1,	15,	6,	11,	15,	0}},
  //	voice  9	CELESTA
  {{0,	7,	63,	0,	0,	0,	15,	9,	5,	7,	0},
   {0,	4,	0,	0,	1,	0,	15,	7,	11,	6,	0}},
  //	voice 10	GLOCKENSPIEL
  {{1,	6,	50,	0,	1,	0,	13,	15,	7,	0,	1},
   {0,	4,	0,	0,	0,	0,	15,	6,	10,	3,	1}},
  //	voice 11	MUSIC BOX
  {{1,	6,	10,	4,	0,	0,	15,	13,	15,	0,	0},
   {0,	2,	0,	0,	1,	1,	15,	6,	15,	6,	0}},
  //	voice 12	VIBRAPHONE
  {{1,	5,	10,	0,	1,	0,	15,	15,	7,	0,	1},
   {0,	4,	0,	0,	1,	0,	15,	6,	10,	4,	1}},
  //	voice 13	MARIMBA
  {{0,	5,	16,	7,	0,	0,	14,	13,	10,	9,	0},
   {0,	2,	0,	0,	1,	0,	15,	9,	14,	9,	0}},
  //	voice 14	XYLOPHONE
  {{1,	7,	12,	7,	0,	0,	13,	13,	10,	9,	0},
   {0,	4,	0,	0,	1,	0,	15,	10,	15,	15,	0}},
  //	voice 15	TUBULAR BELLS
  {{0,	3,	25,	4,	1,	1,	15,	7,	3,	0,	1},
   {1,	5,	0,	0,	1,	0,	15,	6,	14,	2,	1}},
  //	voice 16	DULCIMAR
  {{1,	3,	9,	2,	1,	0,	12,	7,	5,	8,	0},
   {0,	1,	0,	0,	0,	0,	15,	0,	0,	6,	0}},


  //	voice 17	DRAW ORGAN
  {{0,	1,	18,	1,	1,	1,	15,	9,	1,	12,	0},
   {0,	1,	0,	0,	0,	1,	15,	4,	0,	12,	0}},
  //	voice 18	PERC ORGAN
  {{0,	4,	20,	1,	1,	1,	15,	10,	5,	11,	0},
   {0,	1,	0,	0,	0,	1,	15,	10,	0,	11,	0}},
  //	voice 19	ROCK ORGAN
  {{0,	1,	9,	2,	1,	1,	14,	13,	1,	11,	0},
   {0,	2,	0,	0,	0,	1,	13,	13,	0,	11,	0}},
  //	voice 20	CHURCH ORGN
  {{1,	6,	19,	2,	0,	1,	14,	10,	2,	6,	0},
   {0,	2,	0,	0,	1,	1,	10,	8,	1,	7,	0}},
  //	voice 21	REED ORGN
  {{1,	2,	19,	2,	0,	1,	14,	10,	0,	7,	0},
   {0,	2,	0,	0,	1,	1,	10,	8,	0,	7,	0}},
  //	voice 22	ACORDION
  {{1,	4,	8,	6,	0,	1,	14,	11,	2,	13,	0},
   {0,	2,	0,	0,	1,	1,	9,	9,	1,	13,	0}},
  //	voice 23	HARMONICA
  {{1,	2,	18,	6,	1,	1,	14,	4,	0,	12,	0},
   {0,	2,	0,	0,	0,	1,	11,	4,	0,	13,	0}},
  //	voice 24	TANGO ACD
  {{1,	5,	29,	0,	1,	1,	13,	4,	0,	9,	0},
   {0,	2,	0,	0,	0,	1,	12,	4,	0,	10,	0}},
  //	voice 25	NYLON GUITAR
  {{1,	2,	23,	6,	0,	0,	15,	6,	9,	7,	0},
   {0,	1,	0,	0,	1,	0,	15,	6,	12,	11,	0}},
  //	voice 26	STEEL GUITAR
  {{1,	3,	20,	6,	0,	0,	15,	6,	9,	13,	1},
   {0,	1,	0,	0,	1,	0,	15,	4,	14,	10,	1}},
  //	voice 27	JAZZ GUITAR
  {{0,	3,	35,	0,	1,	1,	15,	4,	3,	13,	0},
   {0,	1,	0,	0,	0,	1,	15,	4,	15,	11,	0}},
  //	voice 28	CLEAN GUITAR
  {{0,	3,	21,	6,	1,	0,	14,	7,	2,	4,	0},
   {0,	1,	0,	0,	0,	1,	14,	6,	15,	9,	0}},
  //	voice 29	MUTED GUITAR
  {{1,	2,	23,	0,	0,	0,	14,	12,	7,	7,	0},
   {1,	1,	0,	0,	1,	0,	15,	10,	4,	7,	0}},
  //	voice 30	OVERDRIVE
  {{1,	3,	8,	4,	1,	1,	14,	8,	1,	12,	0},
   {0,	1,	0,	0,	0,	1,	12,	7,	1,	12,	0}},
  //	voice 31	DISTOTION GTR
  {{1,	3,	0,	5,	0,	1,	14,	8,	1,	12,	0},
   {0,	1,	0,	0,	1,	1,	14,	7,	1,	12,	0}},
  //	voice 32	GTR HARMO
  {{1,	7,	63,	0,	1,	0,	13,	0,	4,	15,	0},
   {0,	4,	0,	0,	0,	0,	15,	4,	15,	11,	0}},


  //	voice 33	ACOUST BASS
  {{0,	0,	15,	1,	1,	1,	14,	7,	0,	9,	0},
   {0,	1,	0,	0,	0,	1,	14,	6,	10,	9,	0}},
  //	voice 34	FINGER BASS
  {{0,	0,	22,	5,	0,	1,	15,	4,	2,	11,	0},
   {0,	0,	0,	0,	1,	1,	15,	5,	14,	10,	0}},
  //	voice 35	PICK BASS
  {{0,	0,	20,	5,	0,	1,	14,	5,	15,	11,	0},
   {0,	0,	0,	0,	1,	1,	15,	6,	12,	13,	0}},
  //	voice 36	FRET LESS
  {{0,	0,	25,	6,	0,	1,	11,	4,	2,	11,	0},
   {0,	1,	0,	0,	1,	1,	12,	3,	13,	15,	0}},
  //	voice 37	SLAP BASS 1
  {{0,	0,	11,	4,	0,	0,	13,	4,	9,	13,	0},
   {0,	0,	0,	0,	1,	1,	12,	7,	14,	15,	0}},
  //	voice 38	SLAP BASS 2
  {{0,	0,	3,	3,	1,	0,	14,	1,	14,	13,	0},
   {0,	0,	0,	0,	0,	1,	15,	7,	15,	15,	0}},
  //	voice 39	SYNTH BASS 1
  {{0,	0,	11,	5,	0,	1,	15,	7,	14,	11,	0},
   {0,	0,	0,	0,	1,	1,	15,	3,	7,	11,	0}},
  //	voice 40	SYNTH BASS 2
  {{0,	0,	18,	5,	0,	1,	15,	4,	2,	11,	0},
   {0,	0,	0,	0,	1,	1,	15,	4,	1,	11,	0}},
  //	voice 41	VIOLIN
  {{1,	2,	12,	4,	1,	1,	15,	15,	1,	11,	0},
   {0,	2,	0,	0,	0,	1,	10,	9,	1,	11,	0}},
  //	voice 42	VIOLA
  {{1,	1,	17,	4,	0,	1,	15,	15,	1,	12,	0},
   {0,	1,	0,	0,	1,	1,	11,	9,	0,	12,	0}},
  //	voice 43	CELLO
  {{1,	0,	14,	4,	0,	1,	15,	1,	1,	15,	0},
   {0,	0,	0,	0,	1,	1,	10,	4,	0,	15,	0}},
  //	voice 44	CONTRABASS
  {{1,	0,	13,	1,	0,	1,	14,	4,	1,	5,	0},
   {0,	0,	0,	0,	1,	1,	10,	5,	0,	9,	0}},
  //	voice 45	TREM STRINGS
  {{1,	1,	0,	0,	0,	1,	15,	4,	2,	4,	0},
   {0,	1,	0,	0,	1,	1,	10,	15,	1,	9,	0}},
  //	voice 46	PIZZ STRINGS
  {{0,	4,	22,	7,	1,	0,	10,	8,	0,	0,	0},
   {0,	2,	0,	0,	0,	0,	15,	10,	5,	9,	0}},
  //	voice 47	HARP 
  {{1,	2,	29,	0,	0,	0,	15,	8,	7,	8,	0},
   {0,	1,	0,	0,	1,	0,	15,	6,	15,	7,	0}},
  //	voice 48	TIMPANI
  {{0,	0,	7,	3,	0,	0,	14,	11,	0,	5,	0},
   {0,	0,	0,	0,	1,	0,	15,	13,	0,	6,	0}},


  //	voice 49	STRINGS 1
  {{1,	2,	18,	7,	0,	1,	15,	15,	1,	6,	0},
   {0,	4,	0,	0,	1,	1,	10,	9,	2,	9,	0}},
  //	voice 50	STRINGS 2
  {{1,	2,	22,	7,	0,	1,	15,	15,	1,	6,	0},
   {0,	4,	0,	0,	1,	1,	9,	9,	0,	9,	0}},
  //	voice 51	SYN STRINGS1
  {{0,	6,	19,	6,	1,	1,	15,	15,	3,	0,	0},
   {0,	4,	0,	0,	0,	1,	9,	15,	0,	8,	0}},
  //	voice 52	SYN STRINGS2
  {{0,	2,	24,	6,	1,	1,	15,	4,	2,	5,	0},
   {0,	2,	0,	0,	0,	1,	9,	15,	0,	8,	0}},
  //	voice 53	CHOIR Aahs
  {{0,	1,	32,	0,	0,	1,	15,	7,	1,	8,	0},
   {1,	2,	0,	0,	1,	1,	11,	13,	1,	8,	0}},
  //	voice 54	VOICE Oohs
  {{0,	4,	31,	0,	1,	1,	9,	5,	3,	12,	0},
   {1,	1,	0,	0,	0,	1,	13,	4,	4,	12,	0}},
  //	voice 55	SYNYH VOICE
  {{0,	1,	30,	6,	0,	1,	10,	7,	0,	8,	0},
   {1,	2,	0,	0,	1,	1,	11,	3,	0,	8,	0}},
  //	voice 56	ORCHESTRA HIT
  {{0,	1,	0,	4,	0,	0,	10,	7,	6,	6,	0},
   {1,	2,	0,	0,	1,	0,	14,	8,	4,	8,	0}},
  //	voice 57	TRUMPET
  {{0,	2,	22,	6,	0,	1,	13,	8,	1,	12,	0},
   {0,	2,	0,	0,	1,	1,	13,	15,	0,	12,	0}},
  //	voice 58	TROMBONE
  {{0,	1,	20,	6,	1,	1,	12,	8,	1,	12,	0},
   {0,	1,	0,	0,	0,	1,	11,	15,	0,	12,	0}},
  //	voice 59	TUBA
  {{0,	0,	20,	6,	1,	1,	12,	9,	1,	10,	0},
   {0,	0,	0,	0,	0,	1,	13,	5,	3,	10,	0}},
  //	voice 60	MUTED TRP
  {{0,	7,	32,	7,	0,	1,	12,	10,	0,	0,	0},
   {1,	1,	0,	0,	1,	1,	14,	15,	0,	10,	0}},
  //	voice 61	FR HORN
  {{0,	1,	27,	1,	1,	1,	11,	4,	0,	13,	0},
   {0,	1,	0,	0,	0,	1,	12,	15,	0,	13,	0}},
  //	voice 62	BRASS SECTION
  {{0,	1,	7,	3,	1,	1,	12,	0,	0,	15,	0},
   {0,	2,	0,	0,	0,	1,	14,	7,	0,	15,	0}},
  //	voice 63	SYN BRASS 1
  {{1,	1,	6,	0,	1,	1,	12,	5,	5,	8,	0},
   {0,	1,	0,	0,	0,	1,	12,	4,	1,	11,	0}},
  //	voice 64	SYN BRASS 2
  {{0,	1,	13,	4,	1,	1,	10,	7,	3,	15,	0},
   {0,	1,	0,	0,	0,	1,	14,	9,	1,	15,	0}},

  //	voice 65	SOPRANO SAX
  {{1,	2,	16,	0,	1,	1,	15,	7,	2,	14,	0},
   {0,	2,	0,	0,	0,	1,	12,	5,	0,	14,	0}},
  //	voice 66	ALTO SAX
  {{1,	1,	14,	4,	0,	1,	14,	6,	0,	12,	0},
   {0,	1,	0,	0,	1,	1,	12,	5,	0,	12,	0}},
  //	voice 67	TENOR SAX
  {{1,	1,	17,	5,	0,	1,	14,	6,	0,	12,	0},
   {0,	1,	0,	0,	1,	1,	13,	5,	0,	12,	0}},
  //	voice 68	BARITONE SAX
  {{1,	0,	14,	5,	0,	1,	14,	6,	0,	15,	0},
   {0,	0,	0,	0,	1,	1,	12,	5,	0,	15,	0}},
  //	voice 69	OBOE
  {{0,	1,	13,	4,	0,	1,	14,	13,	1,	13,	0},
   {0,	1,	0,	0,	1,	1,	12,	15,	0,	13,	0}},
  //	voice 70	ENGLISH HORN
  {{1,	1,	11,	4,	0,	1,	13,	15,	1,	13,	0},
   {1,	1,	0,	0,	1,	1,	13,	14,	0,	15,	0}},
  //	voice 71	BASSOON
  {{1,	0,	14,	4,	0,	1,	12,	8,	0,	12,	0},
   {0,	0,	0,	0,	1,	1,	11,	4,	1,	12,	0}},
  //	voice 72	CLARINET
  {{0,	2,	16,	2,	0,	1,	14,	14,	2,	12,	0},
   {0,	1,	0,	0,	1,	1,	12,	5,	1,	15,	0}},
  //	voice 73	PICCOLO
  {{0,	4,	17,	0,	0,	1,	13,	8,	5,	15,	0},
   {0,	4,	0,	0,	1,	1,	10,	8,	1,	13,	0}},
  //	voice 74	FLUTE
  {{0,	2,	24,	7,	0,	1,	15,	10,	3,	13,	0},
   {0,	2,	0,	0,	1,	1,	10,	6,	1,	10,	0}},
  //	voice 75	RECORDER
  {{0,	2,	39,	4,	0,	1,	12,	8,	1,	13,	0},
   {0,	2,	0,	0,	1,	1,	12,	8,	0,	13,	0}},
  //	voice 76	PAN FLUTE
  {{1,	2,	25,	3,	0,	1,	13,	7,	3,	11,	0},
   {0,	2,	0,	0,	1,	1,	10,	9,	0,	13,	0}},
  //	voice 77	BOTTLE
  {{0,	1,	11,	3,	1,	1,	13,	7,	15,	11,	0},
   {0,	1,	0,	0,	0,	1,	9,	9,	0,	13,	0}},
  //	voice 78	SHAKUHACHI
  {{0,	1,	0,	7,	0,	0,	15,	9,	15,	9,	0},
   {0,	1,	0,	0,	1,	1,	11,	9,	0,	8,	0}},
  //	voice 79	WHISTLE
  {{0,	2,	44,	0,	0,	1,	7,	8,	5,	15,	0},
   {0,	2,	0,	0,	1,	1,	12,	9,	0,	13,	0}},
  //	voice 80	OCARINA
  {{0,	2,	63,	0,	0,	1,	12,	8,	0,	15,	0},
   {0,	2,	0,	0,	1,	1,	14,	6,	1,	6,	0}},


  //	voice 81	SQUARE LEAD
  {{1,	4,	25,	0,	1,	1,	15,	15,	0,	12,	0},
   {0,	2,	0,	0,	0,	1,	15,	15,	0,	15,	0}},
  //	voice 82	SAW LEAD
  {{1,	3,	33,	0,	1,	1,	15,	15,	0,	15,	0},
   {1,	1,	0,	0,	0,	1,	15,	15,	0,	15,	0}},
  //	voice 83	CALIOP LEAD
  {{0,	1,	27,	7,	1,	1,	13,	7,	5,	8,	0},
   {0,	1,	0,	0,	0,	1,	11,	7,	0,	11,	0}},
  //	voice 84	CHIFF LEAD
  {{0,	1,	5,	0,	1,	1,	11,	9,	5,	5,	0},
   {0,	1,	0,	0,	0,	1,	14,	9,	3,	13,	0}},
  //	voice 85	CHARANG LEAD
  {{1,	2,	16,	5,	1,	1,	10,	9,	3,	12,	0},
   {1,	1,	0,	0,	0,	1,	10,	9,	1,	12,	0}},
  //	voice 86	VOICE LEAD
  {{0,	2,	24,	0,	0,	1,	15,	15,	1,	12,	0},
   {1,	1,	0,	0,	1,	1,	11,	15,	0,	12,	0}},
  //	voice 87	FIFTH LEAD
  {{1,	7,	25,	7,	1,	1,	10,	10,	0,	0,	0},
   {1,	1,	0,	0,	0,	1,	14,	15,	0,	10,	0}},
  //	voice 88	BASS & LEAD
  {{1,	1,	12,	0,	1,	1,	13,	14,	0,	10,	0},
   {0,	1,	0,	0,	0,	1,	14,	5,	0,	10,	0}},
  //	voice 89	NEW AGE PAD
  {{1,	7,	43,	1,	1,	1,	13,	15,	3,	15,	0},
   {0,	1,	0,	0,	0,	1,	15,	5,	0,	8,	0}},
  //	voice 90	WARM PAD
  {{0,	1,	24,	5,	0,	1,	14,	11,	1,	4,	0},
   {0,	1,	0,	0,	1,	1,	8,	4,	0,	5,	0}},
  //	voice 91	POLYSYN PAD
  {{0,	1,	23,	6,	1,	1,	15,	4,	3,	7,	0},
   {0,	1,	0,	0,	0,	1,	15,	8,	1,	9,	0}},
  //	voice 92	CHOIR PAD
  {{0,	1,	25,	0,	0,	1,	10,	7,	0,	6,	0},
   {0,	2,	0,	0,	1,	1,	11,	13,	0,	6,	0}},
  //	voice 93	BOWED PAD
  {{0,	1,	32,	5,	1,	1,	8,	4,	4,	6,	0},
   {0,	2,	0,	0,	0,	1,	12,	5,	3,	8,	0}},
  //	voice 94	METAL PAD
  {{1,	2,	14,	0,	1,	1,	14,	4,	7,	10,	0},
   {1,	1,	0,	0,	0,	1,	9,	4,	4,	10,	0}},
  //	voice 95	HALO PAD
  {{0,	1,	31,	7,	1,	1,	14,	3,	15,	9,	0},
   {1,	1,	0,	0,	0,	1,	11,	3,	0,	9,	0}},
  //	voice 96	SWEEP PAD
  {{1,	1,	9,	0,	0,	1,	6,	0,	0,	1,	0},
   {0,	1,	0,	0,	1,	1,	5,	1,	0,	3,	0}},


  //	voice 97	RAIN
  {{0,	7,	14,	1,	1,	1,	15,	11,	8,	9,	0},
   {0,	1,	0,	0,	0,	0,	13,	11,	0,	7,	0}},
  //	voice 98	SOUND TRACK
  {{1,	7,	17,	3,	1,	1,	11,	6,	13,	3,	0},
   {0,	1,	0,	0,	0,	1,	8,	3,	1,	6,	0}},
  //	voice 99	CRYSTAL
  {{0,	6,	63,	2,	0,	1,	11,	0,	11,	15,	1},
   {1,	4,	0,	0,	1,	0,	15,	6,	9,	6,	1}},
  //	voice100	ATMOSPHERE
  {{1,	2,	19,	0,	0,	0,	13,	8,	7,	8,	0},
   {0,	1,	0,	0,	1,	0,	15,	6,	15,	7,	0}},
  //	voice101	BRIGHT
  {{1,	3,	11,	0,	0,	0,	15,	15,	7,	0,	1},
   {0,	2,	0,	0,	1,	0,	15,	6,	10,	4,	1}},
  //	voice102	GOBLINS
  {{1,	1,	9,	3,	1,	1,	5,	5,	8,	4,	1},
   {0,	3,	0,	0,	0,	1,	5,	4,	1,	4,	1}},
  //	voice103	ECHOES
  {{0,	1,	16,	0,	0,	1,	13,	4,	3,	6,	0},
   {0,	1,	0,	0,	1,	1,	12,	15,	0,	6,	0}},
  //	voice104	Sci-Fi
  {{0,	1,	20,	6,	0,	1,	15,	4,	2,	5,	0},
   {0,	1,	0,	0,	1,	1,	10,	7,	7,	8,	0}},
  //	voice105	SITAR
  {{0,	2,	5,	0,	0,	0,	15,	5,	5,	6,	0},
   {1,	7,	0,	0,	1,	0,	15,	5,	15,	9,	0}},
  //	voice106	BANJO
  {{1,	1,	7,	0,	0,	0,	14,	6,	1,	4,	0},
   {0,	3,	0,	0,	1,	0,	14,	8,	15,	8,	0}},
  //	voice107	SHAMISEN
  {{1,	2,	6,	2,	0,	0,	15,	9,	4,	4,	0},
   {1,	2,	0,	0,	1,	0,	15,	9,	15,	3,	0}},
  //	voice108	KOTO
  {{0,	3,	17,	4,	1,	0,	15,	7,	3,	5,	0},
   {1,	1,	0,	0,	0,	0,	14,	7,	8,	6,	1}},
  //	voice109	KALIMBA
  {{0,	4,	21,	6,	0,	0,	15,	13,	5,	9,	0},
   {0,	1,	0,	0,	1,	0,	15,	5,	0,	8,	0}},
  //	voice110	BAGPIPE
  {{0,	1,	9,	3,	1,	1,	12,	15,	2,	15,	0},
   {1,	2,	0,	0,	0,	1,	11,	15,	0,	15,	0}},
  //	voice111	FIDDLE
  {{1,	1,	5,	5,	1,	1,	15,	15,	3,	7,	0},
   {0,	1,	0,	0,	0,	1,	10,	9,	1,	9,	0}},
  //	voice112	SHANAI
  {{1,	1,	4,	3,	1,	1,	15,	13,	0,	14,	0},
   {0,	2,	0,	0,	0,	1,	13,	15,	0,	14,	0}},


  //	voice113	TINKLE BELL
  {{0,	7,	34,	4,	0,	0,	15,	4,	14,	8,	0},
   {0,	5,	0,	0,	1,	0,	15,	6,	10,	3,	0}},
  //	voice114	AGOGO
  {{0,	7,	21,	5,	0,	0,	15,	15,	1,	9,	0},
   {0,	2,	0,	0,	1,	0,	15,	10,	1,	8,	0}},
  //	voice115	STEEL DRUM
  {{0,	5,	29,	4,	0,	0,	11,	10,	3,	8,	0},
   {0,	1,	0,	0,	1,	0,	15,	15,	0,	8,	0}},
  //	voice116	WOODBLOCK
  {{0,	7,	22,	5,	0,	0,	15,	13,	2,	11,	0},
   {0,	2,	0,	0,	0,	0,	15,	11,	14,	8,	0}},
  //	voice117	TAIKO DRUM
  {{0,	0,	6,	3,	0,	0,	14,	11,	0,	10,	0},
   {0,	0,	0,	0,	0,	0,	15,	13,	0,	6,	0}},
  //	voice118	MELODIC TOM
  {{1,	1,	0,	6,	0,	0,	15,	11,	4,	10,	0},
   {0,	0,	0,	0,	0,	0,	15,	6,	0,	8,	0}},
  //	voice119	SYNTH DRUM
  {{0,	0,	9,	7,	0,	0,	15,	15,	0,	9,	0},
   {0,	0,	0,	0,	0,	0,	15,	8,	15,	4,	0}},
  //	voice120	REV CYMBAL
  {{1,	7,	0,	7,	0,	0,	15,	15,	0,	0,	0},
   {0,	6,	0,	0,	0,	0,	6,	15,	15,	15,	0}},
  //	voice121	GTR FRETNOISE
  {{0,	7,	19,	7,	0,	0,	10,	8,	0,	0,	0},
   {0,	4,	0,	0,	0,	0,	10,	10,	5,	9,	0}},
  //	voice122	BREATH NOISE
  {{0,	1,	12,	7,	1,	0,	15,	4,	15,	9,	0},
   {0,	1,	0,	0,	0,	0,	11,	10,	4,	10,	0}},
  //	voice123	SEASHORE
  {{0,	7,	0,	7,	0,	1,	15,	0,	0,	0,	0},
   {0,	0,	0,	0,	0,	1,	5,	6,	11,	6,	0}},
  //	voice124	BIRD TWEET
  {{0,	4,	26,	0,	1,	0,	9,	14,	15,	9,	0},
   {0,	4,	0,	0,	0,	0,	14,	11,	2,	8,	0}},
  //	voice125	TELEPHONE
  {{1,	7,	0,	0,	0,	0,	15,	15,	7,	0,	1},
   {0,	2,	0,	0,	0,	0,	15,	5,	10,	2,	1}},
  //	voice126	HELICOPTER
  {{0,	0,	0,	7,	0,	0,	15,	7,	0,	0,	0},
   {1,	4,	0,	0,	0,	0,	10,	15,	15,	15,	0}},
  //	voice127	APPLAUSE
  {{0,	7,	9,	7,	0,	1,	15,	0,	0,	0,	0},
   {0,	7,	0,	0,	0,	1,	11,	3,	0,	4,	0}},
  //	voice128	GUNSHOT
  {{0,	3,	10,	7,	0,	1,	15,	0,	15,	0,	1},
   {0,	3,	0,	0,	0,	0,	15,	9,	15,	5,	1}} 
};




///////////////////////////////////////////////////////////////////////////////
static rex_timer_type MUSIC_vRef_PDdelay_timer;
static rex_timer_type MUSIC_music_timer;
static byte           MUSIC_power_status;
static boolean        MUSIC_IsRepeatForever;
static word           MUSIC_RemainRepeatNum;
static boolean        MUSIC_MusicStopEnable;
static word          *MUSIC_MusicData;
static word           MUSIC_MusicDataPt;
static word           MUSIC_TotalNoteNum;
static word           MUSIC_Tempo;
static byte           MUSIC_WaterMark;
static int            MUSIC_FilePreLoadDataNum;
static boolean        MUSIC_IsFileNextDataLoadOK;




///////////////////////////////////////////////////////////////////////////////
static void  YMU757B_cmd(byte reg, word mask, word val);
static void  MUSIC_ReadTimbreData(void);
static void  MUSIC_ReadFirstData(void);
static void  MUSIC_ReReadData(void);
static void  MUSIC_LastTimbre(word last_timbre);
static void  MUSIC_FileReadTimbreData(void);
static void  MUSIC_FileReadFirstData(void);
static void  MUSIC_FileReReadData(void);
static void  MUSIC_MakeTimbreData(uint16 voice_type, word* timbre);
static dword MUSIC_CompressTimbre(basic_Timbre_data td);




///////////////////////////////////////////////////////////////////////////////
void MUSIC_hw_init(void)
{
  // 1. timer definition
  // 2. GPIO initialization
  // 3. 757B initialization
  // 4. enable MUSIC_sleep_enable


  // 1. timer definition
  rex_def_timer(&MUSIC_music_timer, &snd_tcb, SND_MUSIC_TIMER_SIG);
  rex_def_timer(&MUSIC_vRef_PDdelay_timer, &snd_tcb, SND_VREF_TIMER_SIG);


  // 2. GPIO initialization
  BIO_TRISTATE(DEC_GPIO_0, BIO_MUSIC_CLK_M, BIO_OUTPUT_ENA_V);
  BIO_TRISTATE(DEC_GPIO_0, BIO_MUSIC_DATA_M, BIO_OUTPUT_ENA_V);
  BIO_TRISTATE(DEC_GPIO_0, BIO_MUSIC_SYNC_M, BIO_OUTPUT_ENA_V);
  BIO_TRISTATE(DMOD_GPIO_INT, BIO_YMU737B_IRQ_M, BIO_OUTPUT_DIS_V);


  // 3. YMU757B initialization
  YMU757B_cmd(MUSIC_CLOCK_I_INDEX, MUSIC_CLOCK_I_SELECT_MASK, CLOCK_I_19_680);
  YMU757B_cmd(MUSIC_FM_CONTROL_INDEX,
              FM_CONTOL_START_STOP_MASK | FM_CONTOL_RESET_MASK,
              FM_CONTOL_STOP | FM_CONTOL_RESET);
  clk_busy_wait(20);
  YMU757B_cmd(MUSIC_FM_CONTROL_INDEX,
              FM_CONTOL_START_STOP_MASK | FM_CONTOL_RESET_MASK,
              FM_CONTOL_STOP);
  YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
              VREF_POWER_REDUCE_MASK,
              VREF_POWER_REDUCE_ON);
  YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
              VOLUME_AMP_REDUCE_MASK,
              VOLUME_AMP_REDUCE_ON);
  YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX, SPKOUT_REDUCE_MASK, SPKOUT_REDUCE_ON);
  YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX, EAR_DAC_REDUCE_MASK,EAR_DAC_REDUCE_ON);


  // 4. Enable MUSIC_sleep_enable
  MUSIC_sleep_enable = TRUE;


  MSG_MED("MUSIC_hw_init() finished", 0, 0, 0);  
} // MUSIC_hw_init()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_power(byte power_status)
{
  // 1. power on
  // 2. power off
  // 3. update music power status
  // 4. sleep forbid


  // 0. check power_status, MUSIC_power_status
  power_status       &= (DP_ON | AP1_ON | AP2_ON | AP3_ON | AP4_ON);
  MUSIC_power_status &= (DP_ON | AP1_ON | AP2_ON | AP3_ON | AP4_ON);
  if(power_status == 0x00 && MUSIC_power_status == 0x00)
  {
      MUSIC_sleep_enable = TRUE;
      if(SND_ALLOW_SLEEPING) SND_SLEEP_ALLOW();
  }
  if(power_status == MUSIC_power_status) return;


  // 1. power on 
  if((power_status & DP_ON) && !(MUSIC_power_status & DP_ON))
  {
      YMU757B_cmd(MUSIC_CLOCK_I_INDEX,
                  MUSIC_CLOCK_I_SELECT_MASK,
                  CLOCK_I_19_680);
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  ENTIRE_POWER_REDUCE_MASK,
                  ENTIRE_POWER_REDUCE_OFF);
      MUSIC_power_status |= DP_ON;
    }
  if((power_status & AP1_ON) && !(MUSIC_power_status & AP1_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  VREF_POWER_REDUCE_MASK,
                  VREF_POWER_REDUCE_OFF);
      clk_busy_wait(150000);
      MUSIC_power_status |= AP1_ON;
  }
  if((power_status & AP2_ON) && !(MUSIC_power_status & AP2_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  VOLUME_AMP_REDUCE_MASK,
                  VOLUME_AMP_REDUCE_OFF);
      clk_busy_wait(20);
      MUSIC_power_status |= AP2_ON;
  }
  if((power_status & AP3_ON) && !(MUSIC_power_status & AP3_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  SPKOUT_REDUCE_MASK,
                  SPKOUT_REDUCE_OFF);
      MUSIC_power_status |= AP3_ON;
  }
  if((power_status & AP4_ON) && !(MUSIC_power_status & AP4_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  EAR_DAC_REDUCE_MASK,
                  EAR_DAC_REDUCE_OFF);
      MUSIC_power_status |= AP4_ON;
  }


  // 2. power off (just conotrol volumes & call MUSIC_power_real_off())
  if(!(power_status & AP2_ON) && (MUSIC_power_status & AP2_ON))
  {
      YMU757B_cmd(MUSIC_FM_VOLUME_INDEX, MUSIC_FM_VOLUME_MASK, 0);
      YMU757B_cmd(MUSIC_SPK_VOLUME_INDEX, MUSIC_SPK_VOLUME_MASK, 0);
  }
  if(!(power_status & AP4_ON) && (MUSIC_power_status & AP4_ON))
  {
      YMU757B_cmd(MUSIC_HPOUT_VOLUME_INDEX, MUSIC_HPOUT_VOLUME_MASK, 0);
  }
  if((!(power_status &  DP_ON) && (MUSIC_power_status &  DP_ON))||
     (!(power_status & AP1_ON) && (MUSIC_power_status & AP1_ON))||
     (!(power_status & AP2_ON) && (MUSIC_power_status & AP2_ON))||
     (!(power_status & AP3_ON) && (MUSIC_power_status & AP3_ON))||
     (!(power_status & AP4_ON) && (MUSIC_power_status & AP4_ON)))
  {
      // 3. update music power status
      MUSIC_power_status  = power_status;

      rex_set_timer(&MUSIC_vRef_PDdelay_timer, 3000);
  
  }
  else
  {
      // 3. update music power status
      MUSIC_power_status  = power_status;
  }


  // 4. sleep forbid
  MUSIC_sleep_enable = FALSE;
  SND_SLEEP_FORBID();
} // MUSIC_power_on()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_real_power_off(void)
{
  // 1. clear power off timer
  // 2. power real off
  // 3. sleep allow


  // 0. check MUSIC_power_status
  MUSIC_power_status &= (DP_ON | AP1_ON | AP2_ON | AP3_ON | AP4_ON);


  // 1. clear real power off timer
  rex_clr_timer(&MUSIC_vRef_PDdelay_timer);


  // 2. power real off
  if(!(MUSIC_power_status & DP_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  ENTIRE_POWER_REDUCE_MASK,
                  ENTIRE_POWER_REDUCE_ON);
  }
  if(!(MUSIC_power_status & AP1_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  VREF_POWER_REDUCE_MASK,
                  VREF_POWER_REDUCE_ON);
  }
  if(!(MUSIC_power_status & AP2_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  VOLUME_AMP_REDUCE_MASK,
                  VOLUME_AMP_REDUCE_ON); 
  }
  if(!(MUSIC_power_status & AP3_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  SPKOUT_REDUCE_MASK,
                  SPKOUT_REDUCE_ON); 
  }
  if(!(MUSIC_power_status & AP4_ON))
  {
      YMU757B_cmd(MUSIC_POWER_MANAGE_INDEX,
                  EAR_DAC_REDUCE_MASK,
                  EAR_DAC_REDUCE_ON);
  }

  
  // 3. sleep allow
  if(MUSIC_power_status == 0x00)
  {
      MUSIC_sleep_enable = TRUE;
      if(SND_ALLOW_SLEEPING) SND_SLEEP_ALLOW();
  }
} // MUSIC_real_power_off()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_music_start(void)
{
  // clear power off timer
  rex_clr_timer(&MUSIC_music_timer);

  // music start command
  YMU757B_cmd(MUSIC_FM_CONTROL_INDEX,
              FM_CONTOL_START_STOP_MASK,
              FM_CONTOL_START); 

  snd_IsMusicPlaying = TRUE;
} // MUSIC_music_start()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_music_stop(void)
{
  snd_IsMusicPlaying = FALSE;

  // clear power off timer
  rex_clr_timer(&MUSIC_music_timer);

  // music stop command
  YMU757B_cmd(MUSIC_FM_CONTROL_INDEX,
              FM_CONTOL_START_STOP_MASK,
              FM_CONTOL_STOP);

  mm_BellFileLoadClose();
} // MUSIC_music_stop()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_music_id(word music_id, word repeat_num)
{
  MUSIC_music_buff(MUSIC_get_music_data((mus_id_type)music_id), repeat_num);
} // MUSIC_music_id()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_music_buff(word *music_data, word repeat_num)
{
  if(music_data == NULL) return;

  MUSIC_IsRepeatForever = (repeat_num == 0) ? TRUE : FALSE;
  MUSIC_RemainRepeatNum = repeat_num;
  MUSIC_MusicStopEnable = FALSE;
  MUSIC_MusicData       = music_data;

  MUSIC_ReadTimbreData();
  MUSIC_ReadFirstData();
  tramp_set_isr(TRAMP_GPIO_INT_0_ISR, MUSIC_ReReadData);
} // MUSIC_music()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_music_file(char *file_name, word repeat_num)
{
  dword dummy;

  if(mm_BellFileLoadOpen(file_name,
                         &dummy,
                         (byte **)&MUSIC_MusicData) != MM_OK) return;
  MUSIC_IsRepeatForever = (repeat_num == 0) ? TRUE : FALSE;
  MUSIC_RemainRepeatNum = repeat_num;
  MUSIC_MusicStopEnable = FALSE;
  
  MUSIC_FileReadTimbreData();
  MUSIC_FileReadFirstData();
  tramp_set_isr(TRAMP_GPIO_INT_0_ISR, MUSIC_FileReReadData);
} // MUSIC_music_file()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_FileLoadData_cb(void)
{
  if(MUSIC_IsFileNextDataLoadOK) return;

  mm_BellFileBlockSeek(2 * MUSIC_MusicDataPt);
  if(mm_BellFileBlockLoad(2 * MUSIC_FilePreLoadDataNum) == MM_OK)
    MUSIC_IsFileNextDataLoadOK = TRUE;
} // MUSIC_LoadFileData()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_volcmd(mus_volume_ctr_type voltype, uint16 vol)
{
  // 1. Is in Audio Test Mode?
  if(IsAudioGainTest)
  {
      if     (voltype == MUS_FM_VOL   ) vol = AudioTestGain.FM;
      else if(voltype == MUS_HPOUT_VOL) vol = AudioTestGain.HP;
      else if(voltype == MUS_SPK_VOL  ) vol = AudioTestGain.SPK;
  }

  // set FM | HP | SPK gain
  if(voltype == MUS_FM_VOL)
    YMU757B_cmd(MUSIC_FM_VOLUME_INDEX, MUSIC_FM_VOLUME_MASK, vol);
  else if(voltype == MUS_HPOUT_VOL)
    YMU757B_cmd(MUSIC_HPOUT_VOLUME_INDEX, MUSIC_HPOUT_VOLUME_MASK, vol);
  else if(voltype == MUS_SPK_VOL)
    YMU757B_cmd(MUSIC_SPK_VOLUME_INDEX, MUSIC_SPK_VOLUME_MASK, vol);
} // MUSIC_volcmd()




///////////////////////////////////////////////////////////////////////////////
YMU757B_gain_type MUSIC_get_YMU757B_gains(uint16 volume_level)
{

  int n;


  const YMU757B_gain_type YMU757B_gain_table[] =
  {
    {0x0000, 0x0000, 0x0000},
    {0x001A, 0x000e, 0x0013},
    {0x001A, 0x0010, 0x0016},
    {0x001A, 0x0012, 0x0019},
    {0x001A, 0x0014, 0x001c},
    {0x001A, 0x0016, 0x001f}
  };


  n = sizeof(YMU757B_gain_table) / sizeof(YMU757B_gain_type);
  if(volume_level < n) return YMU757B_gain_table[volume_level];

  return YMU757B_gain_table[n - 1];
} // MUSIC_get_YMU757B_gains()




///////////////////////////////////////////////////////////////////////////////
void MUSIC_MakeMyMelodyHeader(word *hp, byte timbre)
{
  word temp[4];

  ///Load General MIDI Timbre data
  MUSIC_MakeTimbreData(timbre, temp);
  temp[2] = (temp[2] & 0x1FFF) | 0x2000;
  memcpy(hp, temp, 8);

  temp[2] = (temp[2] & 0x1FFF) | 0x8000;
  memcpy(hp + 28, temp, 8);

  hp[32]  = (ALLOT_CH_3_MASK | ALLOT_CH_2_MASK |
			 ALLOT_CH_1_MASK | ALLOT_CH_0_MASK ) & 0x1070;
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// static functions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void YMU757B_cmd(byte reg, word mask, word val)
{
  dword	bit_mask;
  word	isave;


  // 1. update YMU757B registers
  // 2. start sync
  // 3. send addresss to YMU757B from MSM5000
  // 4. send data     to YMU757B from MSM5000
  // 5. end sync


  // 1. update YMU757B registers
  static word MUSIC_REG_BUF[MUSIC_REG_NUM] = {0,};
  MUSIC_REG_BUF[reg] = (MUSIC_REG_BUF[reg] & ~mask) | (val & mask);


  // 2. start sync
  INTLOCK_SAV(isave);
  BIO_OUT(DEC_GPIO_0, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_LO_V);
  BIO_OUT(DEC_GPIO_0, BIO_MUSIC_CLK_M , BIO_MUSIC_CLK_LO_V);


  // 3. send addresss to YMU757B from MSM5000
  for(bit_mask = MUSIC_ADDR_MSB; bit_mask != 0; bit_mask >>= 1)
  {
      if(reg & bit_mask)
	  {
	      BIO_OUT(DEC_GPIO_0, BIO_MUSIC_DATA_M, BIO_MUSIC_DATA_HI_V);
	  }
      else
	  {
          BIO_OUT(DEC_GPIO_0, BIO_MUSIC_DATA_M, BIO_MUSIC_DATA_LO_V);
	  }
      BIO_OUT(DEC_GPIO_0, BIO_MUSIC_CLK_M, BIO_MUSIC_CLK_HI_V);
      clk_busy_wait(1);
      BIO_OUT(DEC_GPIO_0, BIO_MUSIC_CLK_M, BIO_MUSIC_CLK_LO_V);
  }


  // 4. send data     to YMU757B from MSM5000
  for(bit_mask = MUSIC_DATA_MSB; bit_mask != 0; bit_mask >>= 1) 
  {
      if(MUSIC_REG_BUF[reg] & bit_mask)
	  {
	      BIO_OUT(DEC_GPIO_0, BIO_MUSIC_DATA_M, BIO_MUSIC_DATA_HI_V);
	  }
      else
	  {
	      BIO_OUT(DEC_GPIO_0, BIO_MUSIC_DATA_M, BIO_MUSIC_DATA_LO_V);
	  }
      BIO_OUT(DEC_GPIO_0, BIO_MUSIC_CLK_M, BIO_MUSIC_CLK_HI_V);
      clk_busy_wait(1);
      BIO_OUT(DEC_GPIO_0, BIO_MUSIC_CLK_M, BIO_MUSIC_CLK_LO_V);
  }

  
  // 5. end sync
  BIO_OUT(DEC_GPIO_0, BIO_MUSIC_DATA_M , BIO_MUSIC_DATA_LO_V);
  BIO_OUT(DEC_GPIO_0, BIO_MUSIC_SYNC_M, BIO_MUSIC_SYNC_HI_V);
  clk_busy_wait(1);
  INTFREE_SAV(isave);
} // YMU757B_cmd()




///////////////////////////////////////////////////////////////////////////////
static void MUSIC_ReadTimbreData(void)
{
  int i;


  MUSIC_MusicDataPt  = 42;
  MUSIC_TotalNoteNum = MUSIC_MusicData[0] / 2;
  if(MUSIC_TotalNoteNum <= 42) MUSIC_TotalNoteNum = 42;


  for(i = 0; i < 32; i++)
  {
      YMU757B_cmd(MUSIC_1ST_TIMBRE_MODUL_INDEX1 + i,
                  MUSIC_TIMBRE_MODUL_MASK,
                  MUSIC_MusicData[i + 1]);
  }


  YMU757B_cmd(MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX, 
              ALLOT_CH_3_MASK|ALLOT_CH_2_MASK|ALLOT_CH_1_MASK|ALLOT_CH_0_MASK,
              MUSIC_MusicData[33]);


  MUSIC_Tempo = MUSIC_MusicData[34];
  YMU757B_cmd(MUSIC_TEMPO_DATA_INDEX, MUSIC_TEMPO_DATA_MASK, MUSIC_Tempo);
} // MUSIC_ReadTimbreData()




///////////////////////////////////////////////////////////////////////////////
static void MUSIC_ReadFirstData(void)
{
  int i, n;


  if(MUSIC_RemainRepeatNum > 0) MUSIC_RemainRepeatNum--;


  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO);


  n = MIN(32, MUSIC_TotalNoteNum - MUSIC_MusicDataPt);
  for(i = 0; i < n; i++, MUSIC_MusicDataPt++)
  {
      YMU757B_cmd(MUSIC_NOTE_REST_DATA_INDEX,
                  0xFFFF, 
                  MUSIC_MusicData[MUSIC_MusicDataPt]);
  }


  if(MUSIC_TotalNoteNum - 42 < 32) MUSIC_WaterMark = 1;
  else        MUSIC_WaterMark = MUSIC_REFILL_REMEAINED;
  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, MUSIC_WaterMark);


  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
              MUSIC_IRQ_ENABLE_MASK,
              WAIT_FIFO_EMPTY_INT); 
} // MUSIC_ReadFirstData()




///////////////////////////////////////////////////////////////////////////////
static void MUSIC_ReReadData(void)
{
  int i, n;


  if(MUSIC_MusicDataPt < MUSIC_TotalNoteNum)
  {
      // Disable IRQ for writing note data
      YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
                  MUSIC_IRQ_ENABLE_MASK,
                  FILL_DATA_FIFO); 


      n = MIN(32 - MUSIC_WaterMark, MUSIC_TotalNoteNum - MUSIC_MusicDataPt);
      for(i = 0;i < n ; i++, MUSIC_MusicDataPt++)
	  {
          YMU757B_cmd(MUSIC_NOTE_REST_DATA_INDEX,
                      0xFFFF,
                      MUSIC_MusicData[MUSIC_MusicDataPt]);
	  }


      YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
                  MUSIC_IRQ_ENABLE_MASK,
                  WAIT_FIFO_EMPTY_INT); 
  }
  else
  {
      if(MUSIC_IsRepeatForever || MUSIC_RemainRepeatNum > 0)
	  {
		  // repeat the melody
		  if(MUSIC_RemainRepeatNum > 0) MUSIC_RemainRepeatNum--;

		  MUSIC_MusicDataPt = 42;
		  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
					  MUSIC_IRQ_ENABLE_MASK,
					  FILL_DATA_FIFO);

		  n = MIN(32 - MUSIC_WaterMark,
			  MUSIC_TotalNoteNum - MUSIC_MusicDataPt);
		  for(i = 0; i < n; i++, MUSIC_MusicDataPt++)
		  {
			  YMU757B_cmd(MUSIC_NOTE_REST_DATA_INDEX,
                          0xFFFF, 
                          MUSIC_MusicData[MUSIC_MusicDataPt]);
		  }
		  
		  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
                      MUSIC_IRQ_ENABLE_MASK,
                      WAIT_FIFO_EMPTY_INT); 
	  }
      else
	  {
		  // stop the melody
		  if(!MUSIC_MusicStopEnable)
		  {
			  MUSIC_MusicStopEnable = TRUE;

			  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
                          MUSIC_IRQ_ENABLE_MASK,
                          FILL_DATA_FIFO); 
			  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
			  YMU757B_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078); 
			  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
                          MUSIC_IRQ_ENABLE_MASK,
                          WAIT_FIFO_EMPTY_INT); 
			}
		  else
			{
			  MUSIC_MusicStopEnable = FALSE;

			  tramp_set_isr(TRAMP_GPIO_INT_0_ISR, NULL);
			  MUSIC_MusicDataPt--;
			  MUSIC_LastTimbre(MUSIC_MusicData[MUSIC_MusicDataPt]);
			}
	  }
  }
} // MUSIC_ReReadData();




///////////////////////////////////////////////////////////////////////////////
static void MUSIC_LastTimbre(word last_timbre)
{
  if(last_timbre & MUSIC_OCT_BLK_SET_M)
    //
    // (                 8728    )   /
    // ( 60000[msec] * --------- )  /  4
    // (               Opt_tempo ) /
    //
    rex_set_timer(&MUSIC_music_timer, (dword)MUSIC_Tempo * 30000.0 / 1091.0);
  else
    rex_set_timer(&MUSIC_music_timer, (dword)10);
} // MUSIC_LastTimbre()




///////////////////////////////////////////////////////////////////////////////
static void MUSIC_FileReadTimbreData(void)
{
  int i;
  mm_BellFileBlockLoad(2);
  MUSIC_TotalNoteNum = MUSIC_MusicData[0] / 2;
  MUSIC_MusicDataPt  = 42;
  if(MUSIC_TotalNoteNum <= 42) MUSIC_TotalNoteNum = 42;


  mm_BellFileBlockLoad(64);
  for(i = 0; i < 32; i++)
  {
    YMU757B_cmd(MUSIC_1ST_TIMBRE_MODUL_INDEX1 + i,
                MUSIC_TIMBRE_MODUL_MASK,
                MUSIC_MusicData[i]);
  }


  mm_BellFileBlockLoad(4);
  YMU757B_cmd(MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX, 
              ALLOT_CH_3_MASK|ALLOT_CH_2_MASK|ALLOT_CH_1_MASK|ALLOT_CH_0_MASK,
              MUSIC_MusicData[0]);
  MUSIC_Tempo = MUSIC_MusicData[1];
  YMU757B_cmd(MUSIC_TEMPO_DATA_INDEX, MUSIC_TEMPO_DATA_MASK, MUSIC_Tempo);


  mm_BellFileBlockSeek(84);
} // MUSIC_ReadTimbreFileData()




///////////////////////////////////////////////////////////////////////////////
static void MUSIC_FileReadFirstData(void)
{
  int i, n;


  if(MUSIC_RemainRepeatNum > 0) MUSIC_RemainRepeatNum--;


  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO);


  n = MIN(32, MUSIC_TotalNoteNum - MUSIC_MusicDataPt);
  mm_BellFileBlockLoad(2 * n);
  for(i = 0; i < n; i++, MUSIC_MusicDataPt++)
    YMU757B_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, MUSIC_MusicData[i]);

    
  if(MUSIC_TotalNoteNum - 42 < 32) MUSIC_WaterMark = 1;
  else        MUSIC_WaterMark = MUSIC_REFILL_REMEAINED;
  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, MUSIC_WaterMark);
  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
              MUSIC_IRQ_ENABLE_MASK,
              WAIT_FIFO_EMPTY_INT);


  // Pre-load data from the file
  if(MUSIC_MusicDataPt < MUSIC_TotalNoteNum)
  {
    n = MIN(32 - MUSIC_WaterMark,MUSIC_TotalNoteNum - MUSIC_MusicDataPt);
    MUSIC_FilePreLoadDataNum = n;
    mm_BellFileBlockLoad(2 * n);
    MUSIC_IsFileNextDataLoadOK  = TRUE;
  }
  else
  {
    if(MUSIC_IsRepeatForever || MUSIC_RemainRepeatNum > 0)
	{
	  // repeat the music
	  if(MUSIC_RemainRepeatNum > 0) MUSIC_RemainRepeatNum--;


	  MUSIC_MusicDataPt = 42;
	  n = MIN(32 - MUSIC_WaterMark,MUSIC_TotalNoteNum - MUSIC_MusicDataPt);
	  MUSIC_FilePreLoadDataNum = n;
	  MUSIC_IsFileNextDataLoadOK  = TRUE;
	  if(MUSIC_WaterMark == MUSIC_REFILL_REMEAINED)
	  {
	    mm_BellFileBlockSeek(2 * MUSIC_MusicDataPt);
	    mm_BellFileBlockLoad(2 * MUSIC_FilePreLoadDataNum);
	  }
	}
    else
	{
	  // stop the music
	  if(!MUSIC_MusicStopEnable)
	  {
	    YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
		            MUSIC_IRQ_ENABLE_MASK,
		            FILL_DATA_FIFO); 
	    YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
	    YMU757B_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078); 
	    YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
		            MUSIC_IRQ_ENABLE_MASK,
		            WAIT_FIFO_EMPTY_INT); 
	    MUSIC_MusicStopEnable = TRUE;
	  }
	  else
	  {
	    tramp_set_isr(TRAMP_GPIO_INT_0_ISR, NULL);
	    MUSIC_MusicDataPt--;
	    if(MUSIC_WaterMark == MUSIC_REFILL_REMEAINED)
		{
		  mm_BellFileBlockSeek(2 * MUSIC_MusicDataPt);
		  mm_BellFileBlockLoad(2);
		  MUSIC_LastTimbre(MUSIC_MusicData[0]);
		}
	    else MUSIC_LastTimbre(MUSIC_MusicData[MUSIC_MusicDataPt] - 0x20);
	    MUSIC_MusicStopEnable = FALSE;
	  }
	}
  }
} // MUSIC_FileReadFirstData()




///////////////////////////////////////////////////////////////////////////////
static void MUSIC_FileReReadData(void)
{
  int i, n;

  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_ENABLE_MASK, FILL_DATA_FIFO); 


  for(i = 0;i < MUSIC_FilePreLoadDataNum; i++)
  {
    YMU757B_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, MUSIC_MusicData[i]); 
    MUSIC_MusicDataPt++;
  }


  YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
	      MUSIC_IRQ_ENABLE_MASK,
	      WAIT_FIFO_EMPTY_INT); 


  if(MUSIC_MusicDataPt < MUSIC_TotalNoteNum)
  {
    n = MIN(32 - MUSIC_WaterMark,MUSIC_TotalNoteNum - MUSIC_MusicDataPt);
    MUSIC_FilePreLoadDataNum = n;
    MUSIC_IsFileNextDataLoadOK  = FALSE;
    rex_set_sigs(&snd_tcb, SND_FILE_READ_SIG);
  }
  else
  {
    if(MUSIC_IsRepeatForever || MUSIC_RemainRepeatNum > 0)
	{
	  // repeat the music
	  if(MUSIC_RemainRepeatNum > 0) MUSIC_RemainRepeatNum--;


	  MUSIC_MusicDataPt = 42;
	  n = MIN(32 - MUSIC_WaterMark,MUSIC_TotalNoteNum - MUSIC_MusicDataPt);
	  MUSIC_FilePreLoadDataNum = n;
	  if(MUSIC_WaterMark == MUSIC_REFILL_REMEAINED)
	  {
	    MUSIC_IsFileNextDataLoadOK = FALSE;
	    rex_set_sigs(&snd_tcb, SND_FILE_READ_SIG);
	  }
	}
    else
	{
	  if(!MUSIC_MusicStopEnable)
	  {
	    YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
	                MUSIC_IRQ_ENABLE_MASK,
	                FILL_DATA_FIFO); 
	    YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX, MUSIC_IRQ_POINT_MASK, 0); 
	    YMU757B_cmd(MUSIC_NOTE_REST_DATA_INDEX, 0xFFFF, 0x3078); 
	    YMU757B_cmd(MUSIC_IRQ_CONTROL_INDEX,
                    MUSIC_IRQ_ENABLE_MASK,
                      WAIT_FIFO_EMPTY_INT); 
	    MUSIC_MusicStopEnable = TRUE;
	  }
	  else
	  {
	    tramp_set_isr(TRAMP_GPIO_INT_0_ISR, NULL);
	    MUSIC_MusicDataPt--;
	    MUSIC_LastTimbre(MUSIC_MusicData[MUSIC_FilePreLoadDataNum - 1]);
	    MUSIC_MusicStopEnable = FALSE;
	  }
	}
  }
} // MUSIC_FileReReadData()




///////////////////////////////////////////////////////////////////////////////
static void MUSIC_MakeTimbreData(uint16 voice_type, word* timbre)
{
  const Timbre_data_type *UnCompressedTimbre;
  dword					 CompressedTimbre_data;
	
  UnCompressedTimbre    = (Timbre_data_type *)&GMIDI_timbre[voice_type];

  CompressedTimbre_data = MUSIC_CompressTimbre(UnCompressedTimbre->Mod_T);
  timbre[0] = (word)((CompressedTimbre_data >> 16) & MUSIC_TIMBRE_MODUL_MASK);
  timbre[1] = (word)(CompressedTimbre_data & MUSIC_TIMBRE_MODUL_MASK);

  CompressedTimbre_data = MUSIC_CompressTimbre(UnCompressedTimbre->Car_T);
  timbre[2] = (word)((CompressedTimbre_data >> 16) & MUSIC_TIMBRE_MODUL_MASK);
  timbre[3] = (word)(CompressedTimbre_data & MUSIC_TIMBRE_MODUL_MASK);
}




///////////////////////////////////////////////////////////////////////////////
static dword MUSIC_CompressTimbre(basic_Timbre_data td)
{
  return ((MUSIC_TIMBRE_MULTIPLE_MASK      &
		   ((dword)td.multi << MUSIC_TIMBRE_MULTIPLE_SHIFT))
		  |
		  (MUSIC_TIMBRE_VIBRATO_MASK       &
		   ((dword)td.vib   << MUSIC_TIMBRE_VIBRATO_SHIFT))
		  |
		  (MUSIC_TIMBRE_ENV_TYPE_MASK      &
		   ((dword)td.egt   << MUSIC_TIMBRE_ENV_TYPE_SHIFT))
		  |
		  (MUSIC_TIMBRE_SUSTAIN_ONOFF_MASK &
		   ((dword)td.sus   << MUSIC_TIMBRE_SUSTAIN_ONOFF_SHIFT))
		  |
		  (MUSIC_TIMBRE_RELEASE_L_MASK     &
		   ((dword)td.rr    << MUSIC_TIMBRE_RELEASE_L_SHIFT))
		  |
		  (MUSIC_TIMBRE_DECAY_L_MASK       &
		   ((dword)td.dr    << MUSIC_TIMBRE_DECAY_L_SHIFT))
		  |
		  (MUSIC_TIMBRE_ATTACK_L_MASK      &
		   ((dword)td.ar    << MUSIC_TIMBRE_ATTACK_L_SHIFT))
		  |
		  (MUSIC_TIMBRE_SUSTAIN_L_MASK     &
		   ((dword)td.sl    << MUSIC_TIMBRE_SUSTAIN_L_SHIFT))
		  |
		  (MUSIC_TIMBRE_TOTAL_L_MASK       &
		   ((dword)td.tl    << MUSIC_TIMBRE_TOTAL_L_SHIFT))	
		  |
		  (MUSIC_TIMBRE_WAVEFORM_MASK      &
		   ((dword)td.wave  << MUSIC_TIMBRE_WAVEFORM_SHIFT))	
		  |
		  (MUSIC_TIMBRE_FEEDBACK_MASK      &
		   ((dword)td.fl    << MUSIC_TIMBRE_FEEDBACK_SHIFT)));
}




////////////////////////////////////////////////////////////////////////////
word MUSIC_ConvertRingerToNote(uint16 tone, uint16 dur)
{
	const byte Tbl_RingerToNote_h[] = {
		0x44,	// NT_C3P  
		0x48,	// NT_D3
		0x4c,	// NT_D3P
		0x54,	// NT_E3
		0x58,	// NT_F3
		0x5c,	// NT_F3P
		0x64,	// NT_G3
		0x68,	// NT_G3P
		0x6c,	// NT_A3
		0x74,	// NT_A3P
		0x78,	// NT_B3
		0x7c,	// NT_C4
		0x84,	// NT_C4P
		0x88,	// NT_D4 
		0x8c,	// NT_D4P
		0x94,	// NT_E4 
		0x98,	// NT_F4 
		0x9c,	// NT_F4P
		0xa4,	// NT_G4 
		0xa8,	// NT_G4P
		0xac,	// NT_A4 
		0xb4,	// NT_A4P
		0xb8,	// NT_B4 
		0xbc,	// NT_C5 
		0xc4,	// NT_C5P
		0xc8,	// NT_D5 
		0xcc,	// NT_D5P
		0xd4,	// NT_E5 
		0xd8,	// NT_F5 
		0xdc,	// NT_F5P
		0xe4,	// NT_G5 
		0xe8,	// NT_G5P
		0xec,	// NT_A5 
		0xf4,	// NT_A5P
		0xf8,	// NT_B5 
		0xfc,	// NT_C6 
		0x85,	// NT_C6P      
		0x89,	// NT_D6
		0x8d,	// NT_D6P
		0x95,	// NT_E6
		0x99,	// NT_F6
		0x9d,	// NT_F6P
		0xa5,	// NT_G6
		0xa9,	// NT_G6P
		0xad,	// NT_A6	
		0xb5,	// NT_A6P
		0xb9,	// NT_B6	
		0xbd,	// NT_C7	
		0xc5,	// NT_C7P 
		0xc9,	// NT_D7
		0xcd,	// NT_D7P
		0xd5,	// NT_E7
		0xd9,	// NT_F7
		0xdd,	// NT_F7P
		0xe5,	// NT_G7
		0xe9,	// NT_G7P
		0xed,	// NT_A7	
		0xf5,	// NT_A7P
		0xf9,	// NT_B7	
		0xfd,	// NT_C8	

	};
	const byte Tbl_RingerToNote_l[] = {
		0x0,	//NT_NULL
		0x7f,	//NT_1
		0x7f,	//NT_2D
		0x7e,	//NT_2
		0x74,	//NT_4D
		0x69,	//NT_4
		0x47,	//NT_8D
		0x3e,	//NT_8
		0x35,	//NT_16D
		0x23,	//NT_16
		0x11	//NT_32
	};

	byte Cur_note_height = 0;
	byte Cur_note_length = 0;
	word Cur_note;

	if(tone >= NT_C3P && tone <=  NT_C8)
	{
		Cur_note_height = Tbl_RingerToNote_h[tone - NT_C3P];
		if( dur <= NT_32)
		{
			Cur_note_length = Tbl_RingerToNote_l[dur - NT_NULL];
		}

	}
	else if(tone == NT_SILENCE)
	{
		Cur_note_height = 0x30;
		if(dur <= NT_32)
		{
			Cur_note_length = Tbl_RingerToNote_l[dur - NT_NULL] & 0x78;
		}
	}

	Cur_note = ((((word)Cur_note_height << 8) & 0xFF00) | (Cur_note_length & 0x00FF));

	return Cur_note;
}
#endif // #ifdef FEATURE_YMU757B_MELODY // jwpark 00.07.24 end
