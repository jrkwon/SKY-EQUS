#ifndef NV24LCI_H
#define NV24LCI_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      N V   2 4 L C X X  D R I V E R   

GENERAL DESCRIPTION
  All the declarations and definitions necessary for the low level 
  NV device driver to interface with the part.  This file
  is specific to the Microchip 24lc65 and 24lc32 parts, please refer
  to the data sheets when examinig this module.
  Several small and conditionally compiled code sections allow running and
  testing this file under DOS.  

REFERENCES
  Microchip 24lc65 data sheet.
  Microchip 24lc32 data sheet.

Copyright (c) 1994,1995,1996,1997,1998,1999 by QUALCOMM Incorporated.  All Rights 
Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/MSM5000/VCS/nv24lci.h_v   1.2   29 Mar 2000 17:59:14   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/13/99   rp      Fixed compiler warnings by moving #include msg.h to .c file. 
03/11/99   snn     ny did following changes to earlier version. Included them
                   here.
                   Cleaned up compilation to remove warnings/error messages.
                   Applied merge guidelines 3.0 to clean up the code.
--------   ---     ----------------------------------------------------------
03/29/00   pg      Added support for COMET testing.
02/11/99   ls      Added support for 24LC64 EEPROM.
10/28/98   dlb     Added support for ARM.
10/01/98   ls      Added _cdecl keyword to nvd_glitch_recovery_lld.
04/10/98   ls      Optimize NVRAM wait delay times.
11/21/97   ls      Make second device select bits be variable.
06/12/97   ls      Reduced ack_timeout value for better performance.
06/11/97   ls      Increased NVD_MAX_CHKPNT for partial block writes.
06/05/97   ls      Fixed reference to 16K mask in macro for ISS2.
06/03/97   ls      Implemented auto-detection of EEPROM configuration.
05/27/97   ls      Added support for ATMEL EEPROM.
01/03/97   ls      Featurized NV.
05/21/96   ls      Increased NVD_MAX_CHKPNT to avoid checkpoint overflow
                   during deadcell recovery.
12/11/95   ras     Split cache crc into several crcs for performance reasons.
11/27/95   ras     Added NV_IO_TST_OFFSET.
11/22/95   ras     Cache checkpointing/glitch recovery debug and 1st release.
10/18/95   ras     Extended write wait time for out of spec. parts.
09/05/95   ras     Reclaimed memory for checkpointing.
08/23/95   ras     Temporary reduction of data usage in checkpoint of cache.
08/08/95   ras     Lint cleanup.
07/17/95   ras     Unit test support for dead cell recovery.
05/31/95   ras     Write out sda value before output enabling SDA.
05/25/95   ras     All start conditions set SDA and SCL into idle position.
05/22/95   ras     Added back support for unit testing.
05/12/95   ras     Code review modifications (including name change).
05/09/95   ras     Initial next generation nv checkin.
04/11/95   ras     Pre code review cleanup.
10/18/94   ras     Initial version.
                   
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "comdef.h"
#include "nv.h"

#if ((defined FEATURE_NV_UNIT_TEST) || (defined FEATURE_UTF_WIN32))
#error code not present
#else
#include "bio.h"
#endif 

#include "nvi.h"
#include "nvd.h"
#include "dog.h"

#ifdef FEATURE_NV_UNIT_TEST
#error code not present
#endif



/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                         DATA DEFINITIONS                                */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define NVD_BIT_LOW           0  /* data bit is zero */
#define NVD_BIT_HIGH          1  /* data bit is one */

#define NVD_BYTE_SIZE         8  /* 8 bits = 1 byte */
#define NVD_MSB_M             0x80  /* most significant bit mask */
#define NVD_ACK               BIO_EEPROM_DATA_LO_V  /* acknowledge */
#define NVD_NACK              BIO_EEPROM_DATA_HI_V  /* !ack */
#define NVD_CACHE_LINE_SIZE   8  /* bytes */
#define NVD_FUDGE             24 /* desire to reduce width of checkpointing
                                    buffer */
#define NVD_MAX_CACHE_PAGES   8

/* The 24LC64 device has a 32-byte cache */
#ifdef FEATURE_NV_24LC64
#error code not present
#else
  /* All others have a 64-byte cache     */
  #define NVD_CACHE_SIZE      64
#endif

/* Define as one cache line less than the cache size so that we
 * do not have to deal with cache roll over into the first line, on
 * unaligned buffers.
 */
#ifdef FEATURE_NV_24LC64
#error code not present
#else
  #define NVD_MAX_CACHE_WRITE  (NVD_CACHE_SIZE-(NVD_CACHE_LINE_SIZE+NVD_FUDGE))
#endif  

/* Absolute highest possible NVRAM address (assuming the maximum 
 * number of EEPROM devices are installed).
*/
#define NVD_MAX_POSSIBLE_OFFSET  0x3FFF

/* NVD_CNTL_BYTE, per specification 
 *          | 1 0 1 0  |d d d         |        X|
 *          |slave addr|dev. sel. bits| Read = 1|
*/
#define NVD_CNTL_BYTE    ((byte)0xA0)    /* defined in data sheet */
#define NVD_CNTL_BYTE_M  ((byte)0x01)    /* MASK for R/W bit  */
#define NVD_DEV_SEL_M    ((byte)0x0E)    /* MASK for device select bits */
#define NVD_DEV_SEL_1    ((byte)0x00)    /* selects EEPROM device #1 */
#define NVD_DEV_SEL_2    ((byte)0x08)    /* selects EEPROM device #2 */
#define NVD_DEV_SEL_3    ((byte)0x04)    /* selects EEPROM device #3 */
#define NVD_DEV_SEL_4    ((byte)0x0C)    /* selects EEPROM device #4 */
#define NVD_DEV_SEL_5    ((byte)0x02)    /* selects EEPROM device #5 */

/* To specify the quantity of EEPROMs installed */
#define NVD_EEPROM_QUANTITY_1  1         /* one EEPROM installed    */
#define NVD_EEPROM_QUANTITY_2  2         /* two EEPROMs installed   */
#define NVD_EEPROM_QUANTITY_3  3         /* three EEPROMs installed */
#define NVD_EEPROM_QUANTITY_4  4         /* four EEPROMs installed  */

/* Number of start conditions to issue upon power-up */
#define NVD_NUMBER_OF_INITIAL_STARTS  9  

/* codes require to extract the endurance block location and size 
 * The high endurance block is set to the end of memory, where the
 * tps and msl live
 */
#define NVD_ENDURANCE_CONFIG  0x40       /* read configuration */
#define NVD_ENDURANCE_SET     0x00       /* set configuration */
#define NVD_ENDURANCE_ADDR_L  0xFF       /* high endurance r/w addr */
#define NVD_ENDURANCE_ADDR_H  0xFF       /* high endurance r/w addr */

/* codes require to extract the security block location */
#define NVD_SECURITY_MASK     0x0F       /* for write protect block */
#define NVD_SECURITY_CONFIG   0xC0       /* for write protect block */
#define NVD_SECURITY_ADDR_L   0x00       /* for write protect block */
#define NVD_SECURITY_ADDR_H   0x80       /* for write protect block */

#define NVD_WRITE_S  0                   /* write bit in control block */
#define NVD_READ_S   1                   /* read bit in control block */
#define NV_FACTORY_RSVD       0x7F       /* give 127 bytes to factory test */
#define EEPROM_GPIO_BUF       DEC_GPIO_0_BUF

/* NOTE: Device manuals spec 5ms to 10ms per page worst-case write 
 * cycle time.  The "_US" is used in a busy wait polling loop, and thus
 * only specifies the delay between successive polls.  The "_MS" is used 
 * in a calculation of the wait for the entire set of pages; for best
 * performance, it should be set a small as possible (an efficient
 * "retry-if-fail" algorithm is invoked if the wait is insufficient).
 */
#define NVD_ACK_TIMEOUT_MS   (5)      /* 5 msec/page write cycle time */
#define NVD_ACK_TIMEOUT_US   (100)    /* 100 microsec "poll rate"  */
/* Set to 200% of worst-case timing for an entire block write */
#define NVD_MAX_DEVICE_POLLS ((1000/NVD_ACK_TIMEOUT_US)*NVD_MAX_CACHE_PAGES*20)
#define NVD_MAX_DEVICE_RETRIES (NVD_MAX_CACHE_PAGES*20)

#define NVD_MIN_OFFSET       0        /* low nv memory == offset 0 */

#define NVD_INIT_DEBUG_VAL   0xf0
#define NVD_UNINIT_DEBUG_VAL 0x55
#define NVD_CELL_MAX_WRITES  150  


#ifdef FEATURE_NV_24LC32           /* NV_24LC32 part in use */

#define NVD_MAX_OFFSET       (0x0FFF - NV_FACTORY_RSVD)  /* 4kb */
#define NVD_INVALID_8K_OFFSET_MASK 0xF000    /* invalid nv array addr. */
#define NVD_INVALID_16K_OFFSET_MASK 0xC000    /* invalid nv (16K) addr. */
#define NVD_HIGHENDUR_LEN    0     /* #bytes in high endurance block */
#define NVD_NUM_CRC_BLOCKS        8  /* must be a power of 2 */
#define NVD_CRC_BLOCKS_SHIFT      9  /* upper bits determine block */
#define NVD_CRC_BLOCK_SIZ    (0x1000/NVD_NUM_CRC_BLOCKS)

#elif defined(FEATURE_NV_24LC65)   /* NV_24LC65 part in use */

#define NVD_MAX_OFFSET       (0x1FFF - NV_FACTORY_RSVD)  /* 8kb */
#define NVD_INVALID_8K_OFFSET_MASK 0xE000    /* invalid nv (8K) addr. */
#define NVD_INVALID_16K_OFFSET_MASK 0xC000    /* invalid nv (16K) addr. */
#define NVD_HIGHENDUR_LEN    4096  /* #bytes in high endurance block */
#define NVD_NUM_CRC_BLOCKS       16  /* must be a power of 2 */
#define NVD_CRC_BLOCKS_SHIFT      9  /* upper bits determine block */
#define NVD_CRC_BLOCK_SIZ    (0x2000/NVD_NUM_CRC_BLOCKS)

#else
#error  Device type not defined 
#endif /* FEATURE_NV_24LC32 */

/* the factory was only 'really' given the last 100 bytes of nvm to use.
 * We steal two bytes from the upper end of the factory data to
 * perform a write/read test to insure that nv is  not wedged. Speculatively
 * it looks like writing to a byte of nv will unlock a part that was
 * reading all 0xff upon boot.
 */
#define NVD_IO_TST_OFFSET_BANK_1  0x1FF8
#define NVD_IO_TST_OFFSET_BANK_2  0x3FF8

#define NVD_EEPROM_SIZ ((NVD_MAX_OFFSET-NVD_MIN_OFFSET) + NV_FACTORY_RSVD)

#define NVD_MAX_CHKPNT       31

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                               Macros                                    */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Busy wait of 0 is used for anything requiring 500ns or less of delay
 * prior to the next toggling of a GPIO line, this is because back to
 * back 'out' or 'in' instructions take a minimum of 518ns to execute
 */

#if defined(T_ARM)
// Watch out for NVD_BUSY_WAIT_0 on arm *BJ*
#define NVD_BUSY_WAIT_0  clk_nano_wait(); clk_nano_wait();   /* 0.0 microseconds of wait */
#define NVD_BUSY_WAIT_1   clk_busy_wait(1) // will 1microsecond do here? *BJ*
#define NVD_BUSY_WAIT_2   clk_busy_wait(2)
#define NVD_BUSY_WAIT_4   clk_busy_wait(4)
#define NVD_BUSY_WAIT_5   clk_busy_wait(5)
#define NVD_BUSY_WAIT_10  clk_busy_wait(10)

#else

#define NVD_BUSY_WAIT_0  /* 0.0 microseconds of wait */

#define NVD_BUSY_WAIT_1  /* 1.1 microseconds of wait */       \
{       \
  _asm { _asm nop _asm nop _asm nop _asm nop _asm nop };     \
}
#define NVD_BUSY_WAIT_2  {NVD_BUSY_WAIT_1; NVD_BUSY_WAIT_1}  /* 2.2usec delay */
#define NVD_BUSY_WAIT_4  {NVD_BUSY_WAIT_2; NVD_BUSY_WAIT_2}  /* 4.4usec delay */
#define NVD_BUSY_WAIT_5  {NVD_BUSY_WAIT_4; NVD_BUSY_WAIT_1}  /* 5.5usec delay */
#define NVD_BUSY_WAIT_10  {NVD_BUSY_WAIT_5; NVD_BUSY_WAIT_5}  /* 5.5usec delay */

#endif

/* Rise time of SCL and SDA */
#define NVI_TR_DELAY           NVD_BUSY_WAIT_1  /* 1 usec @2.5v */

/* Fall time of SCL and SDA */
#define NVI_TF_DELAY           NVD_BUSY_WAIT_0  /* 0.3 usec @2.5v */

/* minimum time SCL must remain low for each operation */
#define NVI_TLOW_DELAY         NVD_BUSY_WAIT_5  /* 4.7 usec minimum @2.5v */

/* minimum time SCL must remain low after a stop condition*/
#define NVI_TBUF_DELAY         NVD_BUSY_WAIT_5  /* 4.7 usec minimum @2.5v */

/* minimum time SCL must remain high for each operation */
#define NVI_THIGH_DELAY        NVD_BUSY_WAIT_4  /* 4 usec @2.5v */

/* Start condition setup time */
#define NVI_TSU_STA_DELAY      NVD_BUSY_WAIT_5  /* 4.7 usec minimum @2.5v */

/* Start condition hold time */
#define NVI_THD_STA_DELAY      NVD_BUSY_WAIT_4  /* 4 usec minimum @2.5v */

/* Stop condition setup time */
#define NVI_TSU_STO_DELAY      NVD_BUSY_WAIT_4  /* 4 usec minimum @2.5v */



#ifdef FEATURE_NV_UNIT_TEST
#error code not present
#else /* !FEATURE_NV_UNIT_TEST */


/*===========================================================================

MACRO NVD_WRITE_SDA

DESCRIPTION
  Write the serial data line. Possible values are high or low.
  The effect of writing the SDA is state dependent. Possible effects are:
    Can lead to a start or stop condition if transitioned when clock is high.
    Can lead to data being stored in the part, or an address being selected. 

PARAMETERS
  val - position to set SDA in, BIO_EEPROM_DATA_HI_V or BIO_EEPROM_DATA_LO_V

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  SDA is left in the position it is set to.

===========================================================================*/
#define NVD_WRITE_SDA(val) \
  BIO_OUT( DEC_GPIO_0, BIO_EEPROM_DATA_M, val)


/*===========================================================================

MACRO NVD_WRITE_SCL

DESCRIPTION
  Write the serial clock line. Possible values are high or low.
  The effect of writing the SCL is state dependent. Possible effects are:
    Can lead to a start or stop condition if transitioned when clock is high.
    Can lead to data being stored in the part, or an address being selected. 

PARAMETERS
  val - position to set SCL in, BIO_EEPROM_CLK_HI_V or BIO_EEPROM_CLK_LO_V

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  SCL is left in the position it is set to.

===========================================================================*/
#define NVD_WRITE_SCL(val) \
  BIO_OUT( DEC_GPIO_0, BIO_EEPROM_CLK_M, val)


/*===========================================================================

MACRO NVD_READ_SDA

DESCRIPTION
  Reaturn the serial data line value.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  NVD_BIT_HIGH if the returned data value is high.
  NVD_BIT_LOW if the returned data value is low.

SIDE EFFECTS
  None.

===========================================================================*/
#define NVD_READ_SDA() \
  ((BIO_INM( DEC_GPIO_0, BIO_EEPROM_DATA_M ) == BIO_EEPROM_DATA_HI_V) \
    ? NVD_BIT_HIGH : NVD_BIT_LOW)



/*===========================================================================

MACRO NVD_CLK_DATA_IO

DESCRIPTION
  Transition the serial clock line from low to high to low with
  the proper timing. When writing this clocks out a data bit.

PARAMETERS
  None.

DEPENDENCIES
  Only for use when writing the SDA value out to the part.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  

===========================================================================*/
#define NVD_CLK_DATA_IO() \
{\
  NVD_WRITE_SCL(BIO_EEPROM_CLK_HI_V); \
  NVI_THIGH_DELAY; \
  NVI_TR_DELAY; \
  NVD_WRITE_SCL(BIO_EEPROM_CLK_LO_V); \
  NVI_TF_DELAY; \
  NVI_TLOW_DELAY;\
}


/*===========================================================================

MACRO NVD_OUTPUT_ENA

DESCRIPTION
  Configure the serial data line for writing.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None. 

===========================================================================*/
#define NVD_SDA_OUTPUT_ENA()  \
  BIO_TRISTATE(DEC_GPIO_0, BIO_EEPROM_DATA_M, BIO_EEPROM_DATA_M)


/*===========================================================================

MACRO NVD_TRISTATE_R

DESCRIPTION
  Configure the serial data line for reading.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None. 

===========================================================================*/
#define NVD_SDA_TRISTATE_R()  \
  BIO_TRISTATE( DEC_GPIO_0, BIO_EEPROM_DATA_M, 0)


/*===========================================================================

MACRO NVD_SCL_INIT

DESCRIPTION
  Initialize the serial clock line to a known state.
  The serial clock gpio is configured as an output and left high.

PARAMETERS
  None.

DEPENDENCIES
  Only to be used after powerup, before any other action is taken on the part.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
#define NVD_SCL_INIT() \
{\
  NVD_WRITE_SCL(BIO_EEPROM_CLK_HI_V); \
  BIO_TRISTATE( DEC_GPIO_0, BIO_EEPROM_CLK_M, BIO_EEPROM_CLK_M); \
}


/*===========================================================================

MACRO NVD_SDA_INIT

DESCRIPTION
  Set the serial data line to a known state.
  The serial data line is configured as an output and left high.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
#define NVD_SDA_INIT() \
{ \
  NVD_WRITE_SDA(BIO_EEPROM_DATA_HI_V);\
  BIO_TRISTATE( DEC_GPIO_0, BIO_EEPROM_DATA_M, BIO_EEPROM_DATA_M); \
}


/*===========================================================================

MACRO NVD_SET_START_COND_INT

DESCRIPTION
  Set the start condition bit, per the data sheet.
  A start condition is a data transition from high to low while the clock is
  high. The '_INT' indicates that this routine does not require the SCL
  line to be left high prior to the macro's execution. Typical usage
  of this macro is by ERR_FATAL() to interrupt an ongoing write.
  
PARAMETERS
  None.

DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.

RETURN VALUE
  None

SIDE EFFECTS
  SCL is left low
  SDA is left high
  If the nv part is hung, this should reset anything except a 
  cached write which is not completed.

===========================================================================*/
#define NVD_SET_START_COND_INT() \
{ \
  /* always enable SDA for output incase ERR_FATAL interrupting a read */ \
  NVD_SDA_OUTPUT_ENA()  \
  /* always raise SDA & SDL for output incase ERR_FATAL interrupting I/O op. */\
  NVD_WRITE_SDA(BIO_EEPROM_DATA_HI_V); \
  NVI_TR_DELAY; \
  NVD_WRITE_SCL(BIO_EEPROM_CLK_HI_V); \
  NVI_TSU_STA_DELAY; \
  /* SDA transition while SCL is high */ \
  NVD_WRITE_SDA(BIO_EEPROM_DATA_LO_V); \
  NVI_THD_STA_DELAY; \
  NVD_WRITE_SCL(BIO_EEPROM_CLK_LO_V); \
  /* Default data line to high postion. \
   */ \
  NVD_WRITE_SDA(BIO_EEPROM_DATA_HI_V); \
} /* nvd_set_start_cond_int */


/*===========================================================================

MACRO NVD_SET_START_COND

DESCRIPTION
  Set the start condition bit, per the data sheet.
  A start condition is a data transition from high to low while the clock is
  high.
  
PARAMETERS
  None.

DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.
  SCL must be high either from nvd_init() or NVD_SET_START_COND()
  SDA must be high

RETURN VALUE
  None

SIDE EFFECTS
  SCL is left low
  SDA is left low
  If the nv part is hung, this should reset anything except a 
  cached write which is not completed.

===========================================================================*/
#define NVD_SET_START_COND() \
      NVD_SET_START_COND_INT()
/*{*/
/*  SDA transition while SCL is high     */
/*  NVD_WRITE_SDA(BIO_EEPROM_DATA_LO_V); */
/*  NVI_THD_STA_DELAY;                   */
/*  NVD_WRITE_SCL(BIO_EEPROM_CLK_LO_V);  */
/*  NVI_TF_DELAY;                        */
/*}*/ /* nvd_set_start_cond */

/* <EJECT> */
/*===========================================================================

MACRO NVD_SET_STOP_COND

DESCRIPTION
  Set the stop condition bit.
  A stop condition is a data transition from low to high while the clock is
  high.
  
PARAMETERS
  None.

DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.

RETURN VALUE
  None

SIDE EFFECTS
  SCL is left low
  SDA is left high

===========================================================================*/
#define NVD_SET_STOP_COND() \
{ \
  /* Data line starts in an unknown state */ \
  NVD_WRITE_SDA(BIO_EEPROM_DATA_LO_V);  \
  NVI_TF_DELAY;  \
  NVD_WRITE_SCL(BIO_EEPROM_CLK_HI_V);  \
  NVI_TSU_STO_DELAY;  \
  /* SDA transition while SCL is high */  \
  NVD_WRITE_SDA(BIO_EEPROM_DATA_HI_V);    \
  NVI_TBUF_DELAY;  \
} /* nvd_set_stop_cond */

/* <EJECT> */
/*===========================================================================

MACRO NVD_ACK_BYTE

DESCRIPTION
  Write the acknowledge bit to the part to ack a byte that was just read
  
PARAMETERS
  None.

DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.

RETURN VALUE
  None

SIDE EFFECTS
  SCL is left low
  SDA is left low
  SDA is configured for output

===========================================================================*/
#define NVD_ACK_BYTE() \
{ \
    /* Transmit an 'ack' of the byte */ \
    NVD_WRITE_SDA( NVD_ACK ); \
    /* Configure writing */ \
    NVD_SDA_OUTPUT_ENA();  \
    NVI_TR_DELAY; \
    /* Clock the bit out */ \
    NVD_CLK_DATA_IO();  \
}

/* <EJECT> */
/*===========================================================================

MACRO NVD_NACK_BYTE

DESCRIPTION
  Write the no acknowledge bit to the part to ack a byte that was just read
  
PARAMETERS
  None.

DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.

RETURN VALUE
  None

SIDE EFFECTS
  SCL is left low
  SDA is left high
  SDA is configured for output

===========================================================================*/
#define NVD_NACK_BYTE() \
{ \
    /* Transmit an 'ack' of the byte */ \
    NVD_WRITE_SDA( NVD_NACK ); \
    /* Configure writing */ \
    NVD_SDA_OUTPUT_ENA();  \
    NVI_TR_DELAY; \
    /* Clock the bit out */ \
    NVD_CLK_DATA_IO();  \
}


/*===========================================================================

MACRO NVD_WRITE_CNTL_BYTE

DESCRIPTION
  Writes the control byte out to the nv part, to configure the
  part for either reading or writing nv RAM.

PARAMETERS
  dev_sel_bits - three device select bits (NVD_DEV_SEL_0, etc.)
  rw_bit       - read/write bit, NVD_WRITE_S or NVD_READ_S 

DEPENDENCIES
  None.

RETURN VALUE
  status from nvd_write_byte.

SIDE EFFECTS
  If an operation is in progress, other than a cached write awaiting
  completion, the operation is interrupted and the state machine within the
  part reset (but not the current address).

===========================================================================*/
#define NVD_WRITE_CNTL_BYTE( dev_sel_bits, rw_bit ) \
  nvd_write_byte( (byte)(NVD_CNTL_BYTE | \
                        ((dev_sel_bits) & NVD_DEV_SEL_M) | \
                        ((rw_bit) & NVD_CNTL_BYTE_M)))


#endif /* FEATURE_NV_UNIT_TEST */


/*===========================================================================

MACRO NVD_VALID_8K_OFFSET

DESCRIPTION
  Determine if the offset is a valid offset within 8K capacity NVRAM.

PARAMETERS
  offset - address within the part
  length - number of bytes from offset to be accessed.
DEPENDENCIES
  None.

RETURN VALUE
  TRUE if offset is valid.
  FALSE if offset is invalid.

SIDE EFFECTS
  Points out coding errors.

===========================================================================*/
#define NVD_VALID_8K_OFFSET(offset,length) \
  (!(( ((offset)&NVD_INVALID_8K_OFFSET_MASK) !=0) || \
    ( (((offset)+(length))&NVD_INVALID_8K_OFFSET_MASK) !=0)))


/*===========================================================================

MACRO NVD_VALID_16K_OFFSET

DESCRIPTION
  Determine if the offset is a valid offset within 16K capacity NVRAM.

PARAMETERS
  offset - address within the part
  length - number of bytes from offset to be accessed.
DEPENDENCIES
  None.

RETURN VALUE
  TRUE if offset is valid.
  FALSE if offset is invalid.

SIDE EFFECTS
  Points out coding errors.

===========================================================================*/
#define NVD_VALID_16K_OFFSET(offset,length) \
  (!(( ((offset)&NVD_INVALID_16K_OFFSET_MASK) !=0) || \
    ( (((offset)+(length))&NVD_INVALID_16K_OFFSET_MASK) !=0)))


/*===========================================================================

MACRO NVD_TRANSLATE_TO_PHYSICAL

DESCRIPTION
  Determines the device select bits and physical NV offset 
  corresponding to the specified logical NV offset.

PARAMETERS
  logical_offset - (input)  - the logical address to translate.
  dev_select     - (output) - the device select bits to use.
  phy_offset     - (output) - the physical offset to use.

DEPENDENCIES
  NV must be initialized before this macro can be used.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
#define NVD_TRANSLATE_TO_PHYSICAL( logical_offset, dev_select, phy_offset ) \
  if ((logical_offset) < NV_8K_VALUE) {                  \
    dev_select = NVD_DEV_SEL_1;                          \
    phy_offset = logical_offset;                         \
  }                                                      \
  else {                                                 \
    if (nvi_eeprom_quantity == NVD_EEPROM_QUANTITY_1) {  \
      dev_select = NVD_DEV_SEL_1;                        \
      phy_offset = logical_offset;                       \
    }                                                    \
    else {                                               \
      dev_select = nvi_eeprom_dev_sel_2;                 \
      phy_offset = logical_offset - NV_8K_VALUE;         \
    }                                                    \
  }



/*===========================================================================

MACRO NVD_CRC_BLOCK

DESCRIPTION
  The software eeprom cache is divided into several CRC blocks, for
  performance reasons. This macro determines which CRC block an 
  offset lies in.

PARAMETERS
  offset - address within the part

DEPENDENCIES
  None.

RETURN VALUE
  block number

SIDE EFFECTS
  None

===========================================================================*/
#define NVD_CRC_BLOCK(ADDR)  ((ADDR) >> NVD_CRC_BLOCKS_SHIFT)


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                                                                         */
/*                               Functions                                 */
/*                                                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*===========================================================================

FUNCTION NVD_PUT_BYTES_LLD

DESCRIPTION
  Write data to nv. Writes the data in 'buf' to the
  offset specified by, 'offset', for the number of bytes specified in, 'len'.
  
DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.
  SCL is expected to be low when called.

RETURN VALUE
  NV_BADPARM_S   -  If destination is illegal
  NV_FAIL_S      -  If write failed
  NV_DONE_S      -  If read completed normally

SIDE EFFECTS
  Device's internal address pointer changes
  A big delay is possible.

===========================================================================*/
extern nv_stat_enum_type nvd_put_bytes_lld
(
  byte *buf,     /* data to write to the part */
  word offset,   /* offset within the part to start writing at */
  word len       /* number of bytes to write into he part */
);

/* <EJECT> */
/*===========================================================================

FUNCTION NVD_GET_BYTES_LLD

DESCRIPTION
  Read data from nv. Reads the data in the device
  starting at, 'offset' for the number of bytes specified in, 'len', the
  data is placed in, 'buf'.
  
DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.
  SCL is expected to be low when called.

RETURN VALUE
  NV_BADPARM_S   -  If source is illegal
  NV_DONE_S      -  If read completed normally
  NV_FAIL_S      -  Read operation failed

SIDE EFFECTS
  Device's internal address pointer changes

===========================================================================*/
extern nv_stat_enum_type nvd_get_bytes_lld
(
  byte *buf,       /* buffer read data is placed into */
  word offset,     /* offset in the part to start reading bytes from */
  word len         /* number of bytes to read from the part */
);

/* <EJECT> */
/*===========================================================================

FUNCTION NVD_PUT_BYTE_LLD

DESCRIPTION
  Write a byte to nv
  
DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.
  SCL is expected to be low when called.

RETURN VALUE
  NV_BADPARM_S   -  If destination is illegal
  NV_FAIL_S      -  If write failed
  NV_DONE_S      -  If read completed normally

SIDE EFFECTS
  Device's internal address pointer changes

===========================================================================*/
extern nv_stat_enum_type nvd_put_byte_lld
(
  byte value,         /* byte to write into the part */
  word offset         /* offset in part to write the byte to */
);

/* <EJECT> */
/*===========================================================================

FUNCTION NVD_GET_BYTE_LLD

DESCRIPTION
  Read a byte from nv
  
DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.
  SCL is expected to be low when called.

RETURN VALUE
  NV_BADPARM_S   -  If source is illegal
  NV_DONE_S      -  If read completed normally
  NV_FAIL_S      -  Read operation failed

SIDE EFFECTS
  Device's internal address pointer changes

===========================================================================*/
extern nv_stat_enum_type nvd_get_byte_lld
(
  byte *value,      /* buffer to place read byte into */
  word offset       /* offset in part to read the byte from */
);
/* <EJECT> */
/*===========================================================================

FUNCTION NVD_SYNC_LLD

DESCRIPTION
  low level driver routine for insuring nv is ready to write. 
  
DEPENDENCIES
  nvd_init_lld must be invoked prior to this call.
  SCL is expected to be low when called.

RETURN VALUE
  NV_FAIL_S      -  If previous write failed
  NV_DONE_S      -  If previous write or read completed normally

SIDE EFFECTS
  If a cached write is in progress, it will complete before this function
  returns.
  A big delay is possible.

===========================================================================*/
extern nv_stat_enum_type nvd_sync_lld(void);

/* <EJECT> */
/*===========================================================================

FUNCTION NVD_INIT_LLD

DESCRIPTION
  Initialize clock and data lines to a known state.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void nvd_init_lld( void );

/* <EJECT> */
/*===========================================================================

FUNCTION NVD_UNINIT_LLD

DESCRIPTION
  Insure any write operation in progress has finished. For unit testing,
  save nv array to disk.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void nvd_uninit_lld( void );


#endif /* NV24LCI_H */

