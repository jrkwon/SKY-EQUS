#ifndef YAMAHA_H

#define YAMAHA_H
/*===========================================================================

               Y A M A H A   B I O   H E A D E R    F I L E

DESCRIPTION
  This file contains definitions of the bits of the YAMAHA FM synthesizer for
  the melody sound.  These bits are the control lines which can be read
  or written using the descipted register.

  Copyright (c) 1999 by SKTeletech Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/01/99   jah     Created file

===========================================================================*/

//#define YAMAHA_MELODY_TEST  // for test

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


#define MUSIC_NOTE_REST_DATA_INDEX			0x00

#define MUSIC_1ST_TIMBRE_MODUL_INDEX1		0x10
#define MUSIC_1ST_TIMBRE_MODUL_INDEX2		0x11
#define MUSIC_1ST_TIMBRE_CARRIER_INDEX1		0x12
#define MUSIC_1ST_TIMBRE_CARRIER_INDEX2		0x13
#define MUSIC_2ND_TIMBRE_MODUL_INDEX1		0x14
#define MUSIC_2ND_TIMBRE_MODUL_INDEX2		0x15
#define MUSIC_2ND_TIMBRE_CARRIER_INDEX1		0x16
#define MUSIC_2ND_TIMBRE_CARRIER_INDEX2		0x17
#define MUSIC_3RD_TIMBRE_MODUL_INDEX1		0x18
#define MUSIC_3RD_TIMBRE_MODUL_INDEX2		0x19
#define MUSIC_3RD_TIMBRE_CARRIER_INDEX1		0x1A
#define MUSIC_3RD_TIMBRE_CARRIER_INDEX2		0x1B
#define MUSIC_4TH_TIMBRE_MODUL_INDEX1		0x1C
#define MUSIC_4TH_TIMBRE_MODUL_INDEX2		0x1D
#define MUSIC_4TH_TIMBRE_CARRIER_INDEX1		0x1E
#define MUSIC_4TH_TIMBRE_CARRIER_INDEX2		0x1F
#define MUSIC_5TH_TIMBRE_MODUL_INDEX1		0x20
#define MUSIC_5TH_TIMBRE_MODUL_INDEX2		0x21
#define MUSIC_5TH_TIMBRE_CARRIER_INDEX1		0x22
#define MUSIC_5TH_TIMBRE_CARRIER_INDEX2		0x23
#define MUSIC_6TH_TIMBRE_MODUL_INDEX1		0x24
#define MUSIC_6TH_TIMBRE_MODUL_INDEX2		0x25
#define MUSIC_6TH_TIMBRE_CARRIER_INDEX1		0x26
#define MUSIC_6TH_TIMBRE_CARRIER_INDEX2		0x27
#define MUSIC_7TH_TIMBRE_MODUL_INDEX1		0x28
#define MUSIC_7TH_TIMBRE_MODUL_INDEX2		0x29
#define MUSIC_7TH_TIMBRE_CARRIER_INDEX1		0x2A
#define MUSIC_7TH_TIMBRE_CARRIER_INDEX2		0x2B
#define MUSIC_8TH_TIMBRE_MODUL_INDEX1		0x2C
#define MUSIC_8TH_TIMBRE_MODUL_INDEX2		0x2D
#define MUSIC_8TH_TIMBRE_CARRIER_INDEX1		0x2E
#define MUSIC_8TH_TIMBRE_CARRIER_INDEX2		0x2F

#define MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX	0x30
#define MUSIC_TEMPO_DATA_INDEX				0x31
#define MUSIC_FM_CONTROL_INDEX				0x32
#define MUSIC_CLOCK_I_INDEX					0x33
#define MUSIC_IRQ_CONTROL_INDEX				0x34
#define MUSIC_SPK_VOLUME_INDEX				0x35
#define MUSIC_FM_VOLUME_INDEX				0x36
#define MUSIC_HPOUT_VOLUME_INDEX			0x37
#define MUSIC_POWER_MANAGE_INDEX			0x38


// SYNTH_NOTE_REST_DATA_INDEX	Index : 0x00

#define MUSIC_OCT_BLK_SET_M			0xC000	// b15, b14 : Octave Block setting
											// 0(rest), 1,2,3 

#define MUSIC_OCT_BLK_SET_SHIFT			14	// b15, b14 : Octave Block setting

#define MUSIC_PITCH_SET_M			0x3C00	// b13,b12,b11,b10 :Pitch setting
#define NOTE_C_SHARP				0x0400  // 0x01
#define NOTE_D						0x0800  // 0x02
#define NOTE_D_SHARP				0x0C00  // 0x03
#define NOTE_E						0x1400  // 0x05
#define NOTE_F						0x1800  // 0x06
#define NOTE_F_SHARP				0x1C00  // 0x07
#define NOTE_G						0x2400  // 0x09
#define NOTE_G_SHARP				0x2800  // 0x0A
#define NOTE_A						0x2C00  // 0x0B
#define NOTE_A_SHARP				0x3400  // 0x0D
#define NOTE_B						0x3800  // 0x0E
#define NOTE_C						0x3C00  // 0x0F

#define MUSIC_PART_SET_M			0x0300	// b9,b8 : Part setting
#define CHANNEL_0					0x0000	// Channel 0
#define CHANNEL_1					0x0100	// Channel 1
#define CHANNEL_2					0x0200	// Channel 2
#define CHANNEL_3					0x0300	// Channel 3

#define MUSIC_VIBRATO_SET_M			0x0080	// b7 : Vibrato set(6.4Hz, 13.47cent)
#define VIBRATO_ON					0x0080	// Set Vibrato on 
#define VIBRATO_OFF					0x0000	// Set Vibrato off

#define MUSIC_INTERVAL_SET_M		0x0078	// b6, b5, b4 , b3: Interval set
#define NO_INTERV					0x0000  // 0x00
#define A_TWENTYFOURTH_INTERV		0x0008  // 0x01
#define A_SIXTEENTH_INTERV			0x0010  // 0x02
#define A_TWELVETH_INTERV			0x0018  // 0x03
#define A_EIGHTH_INTERV				0x0020  // 0x04
#define A_SIXTH_INTERV				0x0028  // 0x05
#define THREE_SIXTEENTH_INTERV		0x0030  // 0x06
#define A_QUATER_INTERV				0x0038  // 0x07
#define THREE_EIGHTH_INTERV				0x0040  // 0x08
#define A_HALF_INTERV				0x0048  // 0x09
#define WHOLE_INTERV				0x0050  // 0x0A
//#define NO_INTERV					0x0058  // 0x0B
#define A_THIRD_INTERV				0x0060	// 0x0C
//#define A_HALF_INTERV				0x0068  // 0x0D
#define THREE_QUATER_INTERV			0x0070  // 0x0E
//#define WHOLE_INTERV				0x0078  // 0x0F

#define MUSIC_NOTE_LENGTH_M			0x0007	// b2, b1, b0 : sound length designation


#define MUSIC_TIMBER_CE_M			0x0080	// b7 : Timbre change enable
#define MUSIC_TIMBER_NO_M			0x0007	// b2, b1, b0 : changing Timbre number

//#define MUSIC_1ST_TIMBRE_MODUL_INDEX1		0x10

#define MUSIC_TIMBRE_MODUL_MASK		0xFFFF

#define MUSIC_TIMBRE_MULTIPLE_MASK		0xE0000000
#define MUSIC_TIMBRE_VIBRATO_MASK		0x10000000
#define MUSIC_TIMBRE_ENV_TYPE_MASK		0x08000000
#define MUSIC_TIMBRE_SUSTAIN_ONOFF_MASK	0x04000000
#define MUSIC_TIMBRE_RELEASE_L_MASK		0x03C00000
#define MUSIC_TIMBRE_DECAY_L_MASK		0x003C0000
#define MUSIC_TIMBRE_ATTACK_L_MASK		0x0003C000
#define MUSIC_TIMBRE_SUSTAIN_L_MASK		0x00003C00
#define MUSIC_TIMBRE_TOTAL_L_MASK		0x000003F0
#define MUSIC_TIMBRE_WAVEFORM_MASK		0x00000008
#define MUSIC_TIMBRE_FEEDBACK_MASK		0x00000007

#define MUSIC_TIMBRE_MULTIPLE_SHIFT			29
#define MUSIC_TIMBRE_VIBRATO_SHIFT			28
#define MUSIC_TIMBRE_ENV_TYPE_SHIFT			27
#define MUSIC_TIMBRE_SUSTAIN_ONOFF_SHIFT	26
#define MUSIC_TIMBRE_RELEASE_L_SHIFT		22
#define MUSIC_TIMBRE_DECAY_L_SHIFT			18
#define MUSIC_TIMBRE_ATTACK_L_SHIFT			14
#define MUSIC_TIMBRE_SUSTAIN_L_SHIFT		10
#define MUSIC_TIMBRE_TOTAL_L_SHIFT			4
#define MUSIC_TIMBRE_WAVEFORM_SHIFT			3
#define MUSIC_TIMBRE_FEEDBACK_SHIFT			0




//#define MUSIC_TIMBRE_ALLOTMENT_DATA_INDEX	0x30

#define ALLOT_CH_0_MASK				0x0007
#define ALLOT_CH_1_MASK				0x0070
#define ALLOT_CH_2_MASK				0x0700
#define ALLOT_CH_3_MASK				0x7000

//#define MUSIC_TEMPO_DATA_INDEX				0x31

#define MUSIC_TEMPO_DATA_MASK		0x00FF

//#define MUSIC_FM_CONTROL_INDEX				0x32

#define FM_CONTOL_START_STOP_MASK	0x0001
#define FM_CONTOL_START				0x0001
#define FM_CONTOL_STOP				0x0000

#define FM_CONTOL_RESET_MASK		0x0002
#define FM_CONTOL_RESET				0x0002


//#define MUSIC_CLOCK_I_INDEX					0x33

#define MUSIC_CLOCK_I_SELECT_MASK	0x0007

#define CLOCK_I_2_688				0x0000
#define CLOCK_I_19_200				0x0001
#define CLOCK_I_19_680				0x0002
#define CLOCK_I_19_800				0x0003
#define CLOCK_I_8_400				0x0004
#define CLOCK_I_14_400				0x0005
#define CLOCK_I_27_821				0x0006
#define CLOCK_I_12_600				0x0007


//#define MUSIC_IRQ_CONTROL_INDEX			0x34

#define MUSIC_IRQ_ENABLE_MASK		0x0020
#define MUSIC_IRQ_POINT_MASK		0x001F

#define WAIT_FIFO_EMPTY_INT			0x0020
#define FILL_DATA_FIFO				0x0000


//#define MUSIC_SPK_VOLUME_INDEX			0x35

#define MUSIC_SPK_VOLUME_MASK		0x001F


//#define MUSIC_FM_VOLUME_INDEX				0x36
#define MUSIC_FM_VOLUME_MASK		0x001F

//#define MUSIC_HPOUT_VOLUME_INDEX			0x37
#define MUSIC_HPOUT_VOLUME_MASK		0x001F


//#define MUSIC_POWER_MANAGE_INDEX			0x38

#define ENTIRE_POWER_REDUCE_MASK	0x0001
#define ENTIRE_POWER_REDUCE_ON		0x0001
#define ENTIRE_POWER_REDUCE_OFF		0x0000

#define VREF_POWER_REDUCE_MASK		0x0002
#define VREF_POWER_REDUCE_ON		0x0002
#define VREF_POWER_REDUCE_OFF		0x0000

#define VOLUME_AMP_REDUCE_MASK		0x0004
#define VOLUME_AMP_REDUCE_ON		0x0004
#define VOLUME_AMP_REDUCE_OFF		0x0000

#define SPKOUT_REDUCE_MASK			0x0008
#define SPKOUT_REDUCE_ON			0x0008
#define SPKOUT_REDUCE_OFF			0x0000

#define EAR_DAC_REDUCE_MASK			0x0010
#define EAR_DAC_REDUCE_ON			0x0010
#define EAR_DAC_REDUCE_OFF			0x0000



#define MUSIC_REG_NUM				0x3A

#define MUSIC_ADDR_SIZ  8
#define MUSIC_ADDR_MSB  (((dword)0x01) << (MUSIC_ADDR_SIZ - 1) )

#define MUSIC_DATA_SIZ  16
#define MUSIC_DATA_MSB  (((dword)0x01) << (MUSIC_DATA_SIZ - 1) )

#define MUSIC_REFILL_REMEAINED		0x10

#define MUSIC_POWER_MASK(a,b) \
			(a&(1<<b))



/*---------------------------------------------------------------------------
                        MELODY CHIP POWER MANAGEMENT VALUES

The following are the users of the melody.  To use the melody chip, it is
acquired, used, and then later released.
---------------------------------------------------------------------------*/
#define DP_ON	0x10
#define AP4_ON	0x08
#define AP3_ON	0x04
#define AP2_ON	0x02
#define AP1_ON	0x01

#define ANALOG_POWER_MASK  (AP4_ON | AP3_ON | AP2_ON | AP1_ON)

#define MELODY_POWER_MASK  (DP_ON | AP4_ON | AP3_ON | AP2_ON | AP1_ON)

#define EAR_MELODY_POWER_MASK  (DP_ON | AP4_ON | AP1_ON)

#define SPK_POWER_MASK     (DP_ON | AP3_ON | AP2_ON | AP1_ON)


typedef enum {
  MUS_NONE		= 0x00,
  MUS_MELODY		= 0x01,
  MUS_EAR_MELODY	= 0x02,
  MUS_SPK			= 0x04
} mus_acquire_type;


/*---------------------------------------------------------------------------
                        MELODY CHIP VOLUME MANAGEMENT VALUES
---------------------------------------------------------------------------*/

typedef enum {
  MUS_FM_VOL		= 0x01,
  MUS_SPK_VOL		= 0x02,
  MUS_HPOUT_VOL		= 0x04
} mus_volume_ctr_type;

#define MELODY_VOL_MASK \
			(MUS_FM_VOL | MUS_SPK_VOL | MUS_HPOUT_VOL)

#define EAR_MELODY_VOL_MASK \
			MUS_HPOUT_VOL

#define SPK_VOL_MASK \
			MUS_SPK_VOL

/*---------------------------------------------------------------------------
                        FUNCTION DEFINITION
---------------------------------------------------------------------------*/

extern void MUSIC_bio_init(void);
extern void MUSIC_hw_init(void);
extern void MUSIC_power_on(mus_acquire_type,boolean,boolean);
extern void MUSIC_power_off(mus_acquire_type,boolean,boolean);
extern void MUSIC_power_real_off(void);
extern void MUSIC_set_volume(void);
extern void MUSIC_soundstart(void);//int, boolean);
extern void MUSIC_soundstop(void);
extern void MUSIC_FillfirstData(void);
extern void MUSIC_ReadTimbreData( void);
extern void MUSIC_ReFillData( void);

extern void MUSIC_FillfirstFileData(void);
extern void MUSIC_ReadTimbreFileData( void);
extern void MUSIC_ReFill_FileData( void);

extern void MUSIC_music_id(word , word );
extern void MUSIC_music(word*, word );
extern void MUSIC_music_fileopen(char*, word  );

extern void MUSIC_sound_id(word , word );

extern void MUSIC_test(void);
extern void MUSIC_write_data_to_FIFO(void);
extern void MUSIC_volcmd(mus_volume_ctr_type , uint16);
extern void ReadMelodyFromFlash(void);
extern void Make_MyMelody_Header(	
	word *Hp,  /* preplay ram buffer   */
	byte timb    /* Timbre to load     */
);
extern word Convert_RingerToNote(uint16 , uint16 );
extern sint15 get_volume_offset(void);
extern byte MUSIC_power_status ;
extern byte MUSIC_power_real_status ;
extern byte MUSIC_power_sleep_status ;

#endif //  YAMAHA_H