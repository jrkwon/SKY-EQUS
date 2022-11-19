/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


	  R F     C A L    I N I T I A L    D A T A   for E/S SMT



GENERAL DESCRIPTION
  이 File은 RF 관련 NV Item들의 초기값들을 설정한다.	

EDIT HISTORY FOR MODULE

when       who     what, where, why
--------   ---     -----------------------------------------------------
06/26/00   kmh     현재 D1 Project Cal Default Data값으로 Setting

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* -----------------------------------------------------------------------
** FM NV calibration tables.  Initially contains default values.  If these
** items are not read from NV, the default values are used.
** ----------------------------------------------------------------------- */
rfnv_fm_items_type rfnv_fm_tbl = {
  /* FM mode TX_AGC_ADJ initial setting vs power table
  */
  {  288,288,288,288,266,249,231,215,
       0,  0,  0,  0,  0,  0,  0,  0 },
  /* +24,+24,+24,+24,+20,+16,+12, +8 dBm settings */
  /* Each element = NV data multiplied by RF_FM_AGC_PWR_SCALE during copy */

  /* FM mode TX_AGC_ADJ initial setting vs frequency compensation table
  */
  { 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0  },

  /* FM mode TX_AGC_ADJ initial setting vs temperature compensation table
  */
  { 0, 0, 0, 0, 0, 0, 0, 0 },

  /* FM AGC adjust vs frequency table
  */
  { 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0  },

  /* FM expected HDET vs temperature table
     -> SKTT_CAL : CDMA_LNA_SLP < FM_EXP_HDET_VS_TEMP>
  */
  { 0x3d, 0x2d, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36 },

  /* FM expected HDET vs power level table
  */
  {  114,114,114,114, 53,227,136, 87,
       0,  0,  0,  0,  0,  0,  0,  0 },
  /* +24,+24,+24,+24,+20,+16,+12, +8 dBm settings */

  /* FM error slope vs power level table
  */
  {   20, 20, 20, 20, 28, 12, 20, 24,
       0,  0,  0,  0,  0,  0,  0,  0 },
  /* +24,+24,+24,+24,+20,+16,+12, +8 dBm settings */

  /* FM AGC underflow limit
  */
  0x28,   /* 0.4V */

  /* FM AGC overflow limit
  */
  0x1c,   /* 3.1V */

  /* FM Frequency-Sense-Gain (Tx Gain)
     -> SKTT_CAL : CDMA_LNA_LIN_OFF_0 <FM_FREQ_SENSE_GAIN>
  */
  0x40,

  /* FM RSSI offset
  */
  4500,
  /* Corresponds to -113dBm */

  /* FM RSSI span
  */
  2800
};

/* -----------------------------------------------------------------------
** Digital (CDMA/PCS) NV calibration tables.  Initially contains default
** values.  If these items are not read from NV, the default values are
** used.
** ----------------------------------------------------------------------- */
rfnv_digital_items_type rfnv_cdma_tbl = {

#ifdef RF_LIMIT_RX_AGC_OUTPUT
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Values to limit the AGC output
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Were we able to read the limit Rx AGC values out of NV?
  */
  FALSE,    /* The AGC output limit values not available yet */

  /* Maximum output from the AGC_VALUE accumulator
  */
  0,

  /* Minimum output from the AGC_VALUE accumulator
  */
  0,
#endif

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    PDM1 and PDM2
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
  /* PA range step calibrated value
  */
// A10에서는 rfnv_pa_range_step_cal가 LNA Polarity를 결정하는 값으로 사용된다.
#if MODEL_ID == MODEL_A10	// kmh insert 00.10.25
  0x02,
#else	// #if MODEL_ID == MODEL_A10
  0x43,
#endif	// #if MODEL_ID == MODEL_A10
#ifdef RF_HAS_PA_RN				// by jnlee for B1 PA range 00.08.29
    {0, 0, 0, 0},
#endif

#else	// #ifdef FEATURE_SKTT_CAL

#ifdef RF_HAS_PA_RANGE_CAL
  /* PA range step calibrated value
  */
  192,

#else
  #ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
    /* PA offset correction vs. temperature - includes base values
       some meaningful values here would be nice
    */
    {{0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0}
    },

    /* PA offset correction vs. frequency
    */
    {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
  
  #else

#ifdef RF_CAP_PDM1_FOR_TX
  /* PDM1 frequency compensation table
  */
  {37, 37, 37, 37, 37, 37, 37, 37,
   37, 37, 37, 37, 37, 37, 37, 37},
#endif

#ifdef RF_CAP_PDM2_FOR_TX
  /* PDM2 frequency compensation table
  */
  {-52, -52, -52, -52, -52, -52, -52, -52,
   -52, -52, -52, -52, -52, -52, -52, -52},
#endif

  #endif  /* FEATURE_PA_RANGE_TEMP_FREQ_COMP */
#endif /* RF_HAS_PA_RANGE_CAL */  

#endif	// #ifdef FEATURE_SKTT_CAL

  /* Rx linearizer offsets.  There is one extra segment to allow for
  ** extrapolation.
  */
  {  0x4c,
     0x5a,0x65,0x73,0x7e,0x8b,0x95,0x9e,0xa7,
     0xb1,0xbc,0xc5,0xcb,0xce,0xd1,0xd4,0xd7 },

  /* Rx Linearizer slopes.
  */
  {  0x0e,0x0b,0x0e,0x0b,0x0d,0x0a,0x09,0x09,
     0x0a,0x0b,0x09,0x06,0x03,0x03,0x03,0x03 },

  /* Rx Linearizer versus temperature table.
  */
  { 0 },

  /* Rx slope versus temperature table.
  */
  { 0 },

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     CDMA mode RX/TX_GAIN_COMP values vs frequency
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Rx gain compensation versus frequency.
  */
  { 0 },

  /* Tx gain compensation versus frequency
  */
#ifdef FEATURE_4PAGE_TX_LINEARIZER

  {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
  },

#else

  {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
  },

#endif

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     CDMA mode Tx AGC linearization (MSM2P data is derived from these tables)
     Low numbers equate to high TX power.
     pa_r0 and pa_r1 pins are active low, so 0-0 means highest power, 1-1 lowest
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_4PAGE_TX_LINEARIZER
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
/* Low power table.*/
#define TABLE1      {48,76,104,132,160,197,226,251,275,298, \
      320,340,361,383,406,434,462,462,462,462,462,\
      462,462,462,462,462,462,462,462,462,462,462,462,462,462,462}
/* High power table. */      
#define TABLE2      {50,60,70,80,90,100,110,120,130,140, \
      150,160,170,180,190,200,210,220,230,240,250,\
      250,250,250,250,250,250,250,250,250,250,250,250,250,250,250}

  /* CDMA Tx linearizer master offset
  */
  {
     /*TABLE1,TABLE1,TABLE2,TABLE3 Original order. */
//     TABLE1,TABLE2,TABLE2,TABLE2
     TABLE1,TABLE1,TABLE1,TABLE1
  },
#else	// MODEL_ID == MODEL_B1
/* Low power table.*/
#define TABLE1      {511-477,511-477,511-477,511-477,511-477,511-477,511-450,511-433,511-415,511-402,511-388,511-375,511-\
      362,511-350,511-339,511-327,511-316,511-306,511-295,511-285,511-275,511-265,511-256,511-246,511-\
      237,511-227,511-218,511-209,511-200,511-191,511-181,511-173,511-162,511-154,511-144,511-132}
/* Intermediate power table. */      
#define TABLE2      {511-499,511-499,511-499,511-499,511-499,511-499,511-474,511-457,511-442,511-428,511-414,511-401,511-\
      389,511-377,511-365,511-354,511-343,511-332,511-321,511-311,511-302,511-291,511-280,511-272,511-\
      262,511-252,511-243,511-233,511-224,511-214,511-205,511-196,511-187,511-177,511-166,511-155}
/* High power table. */      
#define TABLE3      {511-495,511-495,511-495,511-495,511-495,511-495,511-495,511-495,511-495,511-495,511-495,511-495,511-\
      495,511-495,511-495,511-495,511-476,511-458,511-441,511-425,511-408,511-394,511-380,511-367,511-\
      354,511-341,511-329,511-319,511-307,511-296,511-286,511-275,511-264,511-253,511-241,511-221}

  /* CDMA Tx linearizer master offset
  */
  {
     /*TABLE1,TABLE1,TABLE2,TABLE3 Original order. */
     TABLE1,TABLE2,TABLE2,TABLE3
  },
#endif	// MODEL_ID == MODEL_B1

#else

  /* CDMA Tx linearizer master offset
  */
  {   0x083,
      0x09b,0x0b3,0x0cb,0x0df,0x0f1,0x103,0x112,0x121,
      0x12e,0x13a,0x145,0x14f,0x15a,0x164,0x170,0x17c,
      0x17c,0x17c,0x17c,0x17c,0x17c,0x17c,0x17c,0x17c,
	  0x17c,0x17c,0x17c,0x17c,0x17c,0x17c,0x17c,0x17c,
      0x17c,0x17c,0x17c },

#endif /* !FEATURE_4PAGE_TX_LINEARIZER */

#ifdef FEATURE_SKTT_CAL		// kmh insert 00.06.26
  /* CDMA Tx linearizer master slope
  */
  {    0x18, 0x18, 0x18, 0x14, 0x12, 0x12, 0x0f, 0x0f, 
       0x0d, 0x0c, 0x0b, 0x0a, 0x0b, 0x0a, 0x0c, 0x0c,    
	      0,    0,    0,    0,    0,    0,    0,    0,    
		  0,    0,    0,    0,    0,    0,    0,    0,    
		  0,    0,    0,    0 },
#endif	// #ifdef FEATURE_SKTT_CAL

  /* CDMA Tx linearizer versus temperature
  */
#ifdef FEATURE_4PAGE_TX_LINEARIZER

  {{0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0}
  },

#else

  {{0,0,0,0,0,0,0,0}
  },

#endif


  /* CDMA Tx slope versus temperature
  */
#ifdef FEATURE_4PAGE_TX_LINEARIZER

  {{0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0}
  },

#else

  {{0,0,0,0,0,0,0,0}
  },

#endif


#ifdef RF_HAS_PA_RANGE_CAL
  /* PA range versus temperature
  */
  { 0 },

#else
  #ifndef FEATURE_PA_RANGE_TEMP_FREQ_COMP
#ifdef RF_CAP_PDM1_FOR_TX
      /* PDM1 versus temperature
  */
  { 0 },
#endif /* RF_CAP_PDM1_FOR_TX */

#ifdef RF_CAP_PDM2_FOR_TX
      /* PDM2 versus temperature
  */
  { 0 },
#endif /* RF_CAP_PFM2_FOR_TX */

  #endif /* !FEATURE_PA_RANGE_TEMP_FREQ_COMP */

#endif /* RF_HAS_PA_RANGE_CAL */


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     CDMA mode Tx power limit data vs temperature,
     frequency, TX_AGC_ADJ PDM setting, and HDET reading
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* CDMA Tx limit versus temperature
  */
  {  832, 832, 832, 832, 832, 832, 832, 832 },
  /* Each element = NV data plus RF_CDMA_TX_LIM_OFFSET during copy */

  /* CDMA Tx limit versus frequency
  */
// by chos 00.03.04
//	RF cal을 안하면 max power가 안나오므로
//	max power limit default 값을 크게 해서 max power 나오게
//  { 0 },
  { 100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100 },

  /* CDMA expected HDET versus AGC
  */
  {   2,  4, 10, 19, 29, 38, 50, 66, 87,107,127,152,176,201,226,251,255 },

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     ADC offset and gain values for calculating indices to the above tables
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* ADC HDET reading offset
  */
  28,

  /* ADC HDET reading span
  */
  98,

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     CDMA mode LNA on/off thresholds

     LNA rise/fall (dBm to MSM register & NV storage)

        [ (dBm Rx + 106) / 85.333 * 256 ] - 128

     LNA rise/fall (MSM register & NV storage to dBm)

        [ (value + 128) / 256 * 85.333 ] - 106
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* LNA range rise value
  */
  127,    /* equivalent to -80dBm input */

  /* LNA range fall value
  */
  127,    /* equivalent to -90dBm input */

  /* LNA range offset
  */
  0,    /* equivalent to 16dB (180 => 15dB) */

  /* LNA range polarity
  */
#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
  CAGC_LNA_GN_POL_V,	
#else	// #ifdef FEATURE_SKTT_CAL
  CAGC_LNA_POL_INV_V,
#endif	// #ifdef FEATURE_SKTT_CAL

  /* LNA range ON value
  */
  CAGC_LNA_LO_V,

  /* LNA range OFF value
  */
  CAGC_LNA_HI_V,

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Miscellaneous other NV digital items
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* CDMA Tx limiter loop gain, scaled by RF_CDMA_TX_LIM_ADJ_SCALE.
  */
  (RF_CDMA_TX_LIM_ADJ_SCALE / 4),

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
  /* PA_R1 rise value
  */
  160,    /* 약 0 dbm             */

  /* PA_R1 fall value
  */
  145,    /* 약 -4 dbm            */

  /* PA_R2 rise value
  */
  255,     /* disabled             */

  /* PA_R2 fall value
  */
  255,     /* disabled             */

  /* PA_R3 rise value
  */
  255,     /* disabled             */

  /* PA_R3 fall value
  */
  255     /* disabled             */
#else
  /* PA_R1 rise value
  */
  0xff,    /* approximately - 4dBm */

  /* PA_R1 fall value
  */
  0xff,    /* approximately - 8dBm */

  /* PA_R2 rise value
  */
  0xaa,    /* disabled             */

  /* PA_R2 fall value
  */
  0xaa,    /* disabled             */

  /* PA_R3 rise value
  */
  0x20,    /* approximately +17dBm */

  /* PA_R3 fall value
  */
  0xe0     /* disabled             */
#endif

// kmh insert start for A10 VCA Control 00.10.04
#ifdef FEATURE_RF_PDM1_VCACNT
  /* VCA min
  */
  ,0x54

  /* VCA max
  */
  ,0x00
#endif	// #ifdef FEATURE_RF_PDM1_VCACNT
// kmh insert end for A10 VCA Control

#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13

  // 00.06.20 kmh change lna using buffer 16 -> 8
  /* LNA lin offset */
  ,{ 0x40, 
     0x7d, 0xaa, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff } 

  /* LNA lin slope */
  ,{ 0x3d, 0x2d, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36 }    

  /* LNA polarity */
  ,0x20

  /* LNA gain min */
  ,0xa0   

  /* LNA gain pwr max */
  ,0x30   

  /* rfnv_patemp_hi */
  ,0x9a		

  /* rfnv_patemp_lo */
  ,0x0		

  /* rfnv_pa_pdmhi */
  ,128	

  /* rfnv_pa_pdmlo */
  ,76

  /* Tx Power limit vs Temperature according to Frequency Group */
  ,{
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 0
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 1
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 2
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 3
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 4
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 5
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 6
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 7
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 8
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 9
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 10
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 11
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 12
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 13
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340,		// Group 14
	  0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340, 0x340		// Group 15
  }
#endif	// #ifdef FEATURE_SKTT_CAL

};

