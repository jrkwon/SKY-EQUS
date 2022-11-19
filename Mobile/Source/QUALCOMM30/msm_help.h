#ifndef MSM_HELP_H
#define MSM_HELP_H

/*===========================================================================

              M S M   R E G I S T E R   A C C E S S   H E L P E R S

                            H E A D E R    F I L E

DESCRIPTION
  This file contains macros to support read and writes of MSM byte (8-bit),
  and half-word (16-bit) registers, as well as to access bit-fields within
  MSM byte and word registers.
  
Copyright (c) 1999,2000,2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/



/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/msm_help.h_v   1.3   18 Jan 2001 16:32:46   evanmell  $



when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/18/01   ejv     Added some more macros to support new macros in msm_drv.h.
08/31/00   rmd     Changed the shadow register code so that it supports 
                   8 bit and 16 bit registers in one array.
01/13/00   ajn     Added the MSM_UNLOCK_AND_LOCK macro.
11/10/99   ejv     Added MSM_RH_MASKED macro.
08/12/99   ejv     Lint cleanup.
08/06/99   ejv     Merged jcw's changes for initial Tiger2 support.
07/23/99   ajn     First cut of file.

===========================================================================*/

#include "msm.h"





/*===========================================================================

                            LOCK/UNLOCK MACROS

===========================================================================*/


/*===========================================================================

MOTIVATION:

  The MSM_LOCK and MSM_UNLOCK macros are used to provide uninterrupted
  access to sets of MSM registers and the MSM shadow registers.
  
  In the past, DMOD_(F)_IN/OUT(H)(M) macros would provide the appropriate
  interrupt locking around the specific msm register, to assure proper
  operation.  For example, the DMOD_F_OUT macro would lock interrupts,
  select the appropriate finger page, output the quantity to the desired
  address, and finally restore interrupt processing.  However, this leads
  to inefficient code.  Consider:

        DMOD_F_OUT( f, DMOD_SLEW_0, B_PTR(slew)[0] );
        DMOD_F_OUT( f, DMOD_SLEW_1, B_PTR(slew)[1] );
        DMOD_F_OUT( f, DMOD_SLEW_2, B_PTR(slew)[2] );
        
  This code translates into:
        INTLOCK( );
          DMOD_OUT( DMOD_F_PAGE, 1 << f );
          DMOD_OUT( DMOD_SLEW_0, B_PTR(slew)[0] );
        INTFREE( );
        INTLOCK( );
          DMOD_OUT( DMOD_F_PAGE, 1 << f );
          DMOD_OUT( DMOD_SLEW_1, B_PTR(slew)[1] );
        INTFREE( );
        INTLOCK( );
          DMOD_OUT( DMOD_F_PAGE, 1 << f );
          DMOD_OUT( DMOD_SLEW_2, B_PTR(slew)[2] );
        INTFREE( );
        
  Interrupts are being locked and unlocked 3 times, and the finger page
  register is being written to 3 times with the same value.  Because
  INTLOCK( ) / INTFREE( ) translates to an extra stack level with a local
  "sav" variable, and function call to REX, the temporary "1 << f" may have
  to be computed 3 times.
  
  Clearly, it would be more efficient to write the following:
        INTLOCK( );
          DMOD_OUT( DMOD_F_PAGE, 1 << f );
          DMOD_OUT( DMOD_SLEW_0, B_PTR(slew)[0] );
          DMOD_OUT( DMOD_SLEW_1, B_PTR(slew)[1] );
          DMOD_OUT( DMOD_SLEW_2, B_PTR(slew)[2] );
        INTFREE( );
   
  More over, this even avoids a subtle bug in that the DMOD_SLEW_0 and
  DMOD_SLEW_1 registers are not actually finger paged registers, and if an
  interrupt between an INTFREE( )/INTLOCK( ) slews a finger, when the
  interrupt returns the original slew command is corrupted.
  
  To this end, MSM register access macros have been designed to NOT
  internally perform INTLOCK/INTFREE operations, but rather rely on the
  calling procedure to provide MSM_LOCK( ) and MSM_FREE( ) calls surrounding
  the MSM register access macros, to create smaller, faster, more robust
  code.

===========================================================================*/


/*===========================================================================

MACRO MSM_LOCK_REQUIRED_DEFN
MACRO MSM_LOCK_REQUIRED

  These macros are used to help prevent against accidental use of MSM
  register access macros which require use of a surrounding MSM_LOCK and
  MSM_UNLOCK context, without the appropriate surrounding context.
  
  The MSM_LOCK_REQUIRED_DEFN macro, used in MSM_LOCK( ), declares a local
  scope variable, "MSM_LOCK_required".  The variable will go out of scope at
  the corresponding MSM_UNLOCK( ) macro.
  
  The MSM_LOCK_REQUIRED macro assigns "MSM_LOCK_required" to itself.  This
  requires the compiler to know of the existance of the "MSM_LOCK_required"
  variable, but otherwise does not generate any code.  The MSM_LOCK_REQUIRED
  macro is then included in any MSM register access macro that requires
  interrupts to be disabled for proper operation.  If that macro is called
  without the surrounding MSM_LOCK/MSM_UNLOCK pair, the compiler will
  generate an error message about an undefined variable "MSM_LOCK_required"
  at that point.  The developer would then have to modify the procedure to
  call MSM_LOCK and MSM_UNLOCK around the register access.
  

  If it is determined that the MSM_LOCK_REQUIRED_DEFN / MSM_LOCK_REQUIRED
  macros actually increase the code size for a particular version of the
  compiler or level of optimization, once the code has been deemed correct,
  the macro DISABLE_MSM_LOCK_REQUIRED_CHECKING may be defined, to compile
  out the MSM_LOCK_REQUIRED code.

===========================================================================*/


#undef DISABLE_MSM_LOCK_REQUIRED_CHECKING
  /* Define this macro if it is determined that executable code size has
     increased due to the checking */



#ifdef DISABLE_MSM_LOCK_REQUIRED_CHECKING


#define MSM_LOCK_REQUIRED_DEFN
  /* No code here */

#define MSM_LOCK_REQUIRED
  /* No code here */


#else /* MSM_LOCK_REQUIRED checking is enabled */


#define MSM_LOCK_REQUIRED_DEFN  \
    boolean MSM_LOCK_required = TRUE;

#define MSM_LOCK_REQUIRED \
    MSM_LOCK_required = MSM_LOCK_required;


#endif




/*===========================================================================

MACRO MSM_LOCK

DESCRIPTION     This macro int-locks the microprocessor for MSM register
                input/output operations.

DEPENDENCIES    It must be followed by a MSM_UNLOCK in the same function,
                at the same block-nesting level.

RETURN VALUE    None.
      
SIDE EFFECTS    Interrupts are blocked until MSM_UNLOCK( ).

===========================================================================*/

#define MSM_LOCK( )    \
    { \
      MSM_LOCK_REQUIRED_DEFN \
      word msm_sav; \
      INTLOCK_SAV( msm_sav )

/*===========================================================================

MACRO MSM_UNLOCK

DESCRIPTION     This macro undoes the locking done by the MSM_LOCK( ) macro.

DEPENDENCIES    It must be followed by a MSM_UNLOCK in the same function,
                at the same block-nesting level.

RETURN VALUE    None.
      
SIDE EFFECTS    Interrupts processing is restored to previous state.

===========================================================================*/

#define MSM_UNLOCK( )    \
    MSM_LOCK_REQUIRED \
    INTFREE_SAV( msm_sav ); }

/*===========================================================================

MACRO MSM_UNLOCK_AND_LOCK

DESCRIPTION     This macro undoes the locking done by the MSM_LOCK( ) macro
      and then quickly redoes the lock.

DEPENDENCIES    It must be preceded by an MSM_LOCK and then followed by an
      MSM_UNLOCK in the same function.

RETURN VALUE    None.
      
SIDE EFFECTS    Interrupts processing is restored to previous state.

===========================================================================*/

#define MSM_UNLOCK_AND_LOCK( )    \
    MSM_LOCK_REQUIRED \
    INTFREE_SAV( msm_sav ), \
    INTLOCK_SAV( msm_sav )


/*===========================================================================

                            BYTE WRITE MACROS

===========================================================================*/



/*===========================================================================

MACRO MSM_WB

DESCRIPTION     This macro writes to a "_WB" style 8-bit msm register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    Since this macro only writes to one memory location, and does
                not update any shadow registers, it may be used outside the
                context of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define MSM_WB( base, val ) \
    MSM_OUT( base##_WB, (val) )


/*===========================================================================

MACRO MSM_W2B

DESCRIPTION     This macro writes a 16-bit quantity to a pair of "_WB" style
                8-bit msm registers.  The upper byte is written last.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This macro must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef _lint
#define MSM_W2B( base, low, high, val ) \
      (void) (val)
#else
#define MSM_W2B( base, low, high, val ) \
    MSM_LOCK_REQUIRED \
    MSM_OUT( base##_##low##_WB,  (byte)  (val)       ),  \
    MSM_OUT( base##_##high##_WB, (byte) ((val) >> 8) )
#endif


/*===========================================================================

MACRO MSM_W3B

DESCRIPTION     This macro writes a 24-bit quantity to a trio of "_WB" style
                8-bit msm registers.  The upper byte is written last.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This macro must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef _lint
#define MSM_W3B( base, low, mid, high, val ) \
    (void) (val)
#else
#define MSM_W3B( base, low, mid, high, val ) \
    MSM_LOCK_REQUIRED \
    MSM_OUT( base##_##low##_WB,  (byte)  (val)        ), \
    MSM_OUT( base##_##mid##_WB,  (byte) ((val) >> 8 ) ), \
    MSM_OUT( base##_##high##_WB, (byte) ((val) >> 16) )
#endif


/*===========================================================================

MACRO MSM_W3B

DESCRIPTION     This macro writes a 48-bit quantity to a set of 6 "_WB" style
                8-bit msm registers.  The upper byte is written last.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

                The value parameter must be an l-value, as it's address is
                required.

DEPENDENCIES    This macro must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define MSM_W6B( base, a0, a1, a2, a3, a4, a5, val ) \
    MSM_LOCK_REQUIRED \
    MSM_OUT( base##_##a0##_WB,  B_PTR(val)[0] ),  \
    MSM_OUT( base##_##a1##_WB,  B_PTR(val)[1] ),  \
    MSM_OUT( base##_##a2##_WB,  B_PTR(val)[2] ),  \
    MSM_OUT( base##_##a3##_WB,  B_PTR(val)[3] ),  \
    MSM_OUT( base##_##a4##_WB,  B_PTR(val)[4] ),  \
    MSM_OUT( base##_##a5##_WB,  B_PTR(val)[5] )


/*===========================================================================

                            HALF-WORD WRITE MACROS

===========================================================================*/



/*===========================================================================

MACRO MSM_WH

DESCRIPTION     This macro writes to a "_WH" style 16-bit demod register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    Since this macro only writes to one memory location, and does
                not update any shadow registers, it may be used outside the
                context of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define MSM_WH( base, val ) \
    MSM_OUTH( base##_WH, (val) )


/*===========================================================================

MACRO MSM_W2H

DESCRIPTION     This macro writes a 32-bit quantity to a pair of "_WH" style
                16-bit msm registers.  The upper word is written last.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This macro must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef _lint
#define MSM_W2H( base, low, high, val ) \
    (void) (val)
#else
#define MSM_W2H( base, low, high, val ) \
    MSM_LOCK_REQUIRED \
    MSM_OUTH( base##_##low##_WH, (word) ((val) & 0xffff) ), \
    MSM_OUTH( base##_##high##_WH, (word) ((val) >> 16 ) )
#endif

/*===========================================================================

MACRO MSM_W3H

DESCRIPTION     This macro writes a 48-bit quantity to three "_WH" style
                16-bit msm registers.  The upper word is written last.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This macro must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#ifdef _lint
#define MSM_W3H( base, low, med, high, val ) \
    (void) (val)
#else
#define MSM_W3H( base, low, med, high, val ) \
    MSM_LOCK_REQUIRED \
    MSM_OUTH( base##_##low##_WH, W_PTR(val)[0] ), \
    MSM_OUTH( base##_##med##_WH, W_PTR(val)[1] ), \
    MSM_OUTH( base##_##high##_WH, W_PTR(val)[2] )
#endif


/*===========================================================================

                            BYTE READ MACROS

===========================================================================*/



/*===========================================================================

MACRO MSM_RB

DESCRIPTION     This macro reads from a "_RB" style 8-bit msm register.
                
                "_RB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or output registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    Since this macro only reads from one memory location, and does
                not update any shadow registers, it may be used outside the
                context of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    Register value (byte).
      
SIDE EFFECTS    None.

===========================================================================*/

#define MSM_RB( base ) \
    MSM_IN( base##_RB )


/*===========================================================================

MACRO MSM_R2B

DESCRIPTION     This macro reads a 16-bit quantity from a pair of "_RB" style
                8-bit msm registers.  The upper byte is read last.
                
                "_RB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or output registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

                The "val" parameter must be an l-value.

DEPENDENCIES    This macro must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    "val" is updated with the value of the register.

===========================================================================*/

#define MSM_R2B( base, low, high, val ) \
    MSM_LOCK_REQUIRED \
    B_PTR( val ) [ 0 ] = MSM_IN( base##_##low##_RB ), \
    B_PTR( val ) [ 1 ] = MSM_IN( base##_##high##_RB )


/*===========================================================================

MACRO MSM_R3B

DESCRIPTION     This macro reads a 24-bit quantity from a trio of "_RB" style
                8-bit msm registers.  The upper byte is read last.
                
                "_RB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or output registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

                The "val" parameter must be an l-value.

DEPENDENCIES    This macro must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    "val" is updated with the value of the register.  Only the
                lower 3 bytes of "val" are affected.  The quantity is not
                sign extended into the 4th byte.

===========================================================================*/

#define MSM_R3B( base, low, mid, high, val ) \
    MSM_LOCK_REQUIRED \
    B_PTR( val ) [ 0 ] = MSM_IN( base##_##low##_RB ), \
    B_PTR( val ) [ 1 ] = MSM_IN( base##_##mid##_RB ), \
    B_PTR( val ) [ 2 ] = MSM_IN( base##_##high##_RB )


/*===========================================================================

MACRO MSM_R6B

DESCRIPTION     This macro reads a 48-bit quantity from a set of 6 "_RB" style
                8-bit msm registers.  The upper byte is read last.
                
                "_RB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or output registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

                The "val" parameter must be an l-value.

DEPENDENCIES    This macro must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    "val" is updated with the value of the register.

===========================================================================*/

#define MSM_R6B( base, a0, a1, a2, a3, a4, a5, val ) \
    MSM_LOCK_REQUIRED \
    B_PTR( val ) [ 0 ] = MSM_IN( base##_##a0##_RB ), \
    B_PTR( val ) [ 1 ] = MSM_IN( base##_##a1##_RB ), \
    B_PTR( val ) [ 2 ] = MSM_IN( base##_##a2##_RB ), \
    B_PTR( val ) [ 3 ] = MSM_IN( base##_##a3##_RB ), \
    B_PTR( val ) [ 4 ] = MSM_IN( base##_##a4##_RB ), \
    B_PTR( val ) [ 5 ] = MSM_IN( base##_##a5##_RB )


/*===========================================================================

                            HALF-WORD READ MACROS

===========================================================================*/



/*===========================================================================

MACRO MSM_RH

DESCRIPTION     This macro reads a 16-bit quantity from an "_RH" style 16-bit
                 msm register.
                
                "_RH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or output registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.
                
DEPENDENCIES    Since this macro only reads from one memory location, and does
                not update any shadow registers, it may be used outside the
                context of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    Register value (16-bit).
      
SIDE EFFECTS    None.

===========================================================================*/

#define MSM_RH( base ) \
    MSM_INH( base##_RH )


/*===========================================================================

MACRO MSM_RH_MASKED

DESCRIPTION     This macro reads a 16-bit quantity from an "_RH" style 16-bit
                msm register.  The "field" mask is then used to mask off the
                undesired bits
                
                "_RH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or output registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.
                
DEPENDENCIES    Since this macro only reads from one memory location, and does
                not update any shadow registers, it may be used outside the
                context of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    Register value (16-bit) & MASK.
      
SIDE EFFECTS    None.

===========================================================================*/

#define MSM_RH_MASKED( base, field ) \
    ( MSM_RH( base ) & base##_RH__##field##_MASK )


/*===========================================================================

MACRO MSM_R2H

DESCRIPTION     This macro reads a 32-bit quantity from a pair of "_RH" style
                16-bit msm registers.
                
                "_RH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers, or output registers
                
                The address must be passed in as a literal.  Variables are
                not allowed.

                The "val" parameter must be an l-value.
                
DEPENDENCIES    Since this macro only reads from one memory location, and does
                not update any shadow registers, it may be used outside the
                context of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    "val" is updated with the contents of the register.

===========================================================================*/

#define MSM_R2H( base, low, high, val ) \
    MSM_LOCK_REQUIRED \
    W_PTR(val)[0] = MSM_INH( base##_##low##_RH ), \
    W_PTR(val)[1] = MSM_INH( base##_##high##_RH )

/*===========================================================================

MACRO MSM_R3H

DESCRIPTION     This macro reads a 48-bit quantity from three "_RH" style
                16-bit msm registers.
                
                "_RH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers, or output registers
                
                The address must be passed in as a literal.  Variables are
                not allowed.

                The "val" parameter must be an l-value.
                
DEPENDENCIES    Since this macro only reads from one memory location, and does
                not update any shadow registers, it may be used outside the
                context of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    "val" is updated with the contents of the register.

===========================================================================*/

#define MSM_R3H( base, low, med, high, val ) \
    MSM_LOCK_REQUIRED \
    W_PTR(val)[0] = MSM_INH( base##_##low##_RH ), \
    W_PTR(val)[1] = MSM_INH( base##_##med##_RH ), \
    W_PTR(val)[2] = MSM_INH( base##_##high##_RH )


/*===========================================================================

                            SHADOW REGISTER MACROS

===========================================================================*/

/* Byte MSM Shadow Registers */

#if defined (T_MSM5000) || defined (FEATURE_16BIT_SHADOW_REG)
    #define SHADOW_B(base) \
        MSM_image_H[ MSM_INX(base##_WB) ]
#else
    #define SHADOW_B(base) \
        MSM_image[ MSM_INX(base##_WB) ]
#endif


/* Word MSM Shadow Registers */

#if defined (T_MSM5000) || defined (FEATURE_16BIT_SHADOW_REG)
    #define SHADOW_H(base) \
        MSM_image_H[ MSM_INX(base##_WH) ]
#else
    #define SHADOW_H(base) \
        MSM_image_H[ base##_WH_INX ]
#endif

/* Byte MSM Finger Shadow Registers */

#define SHADOW_FINGER_B(base) \
    MSM_fimage[ _dmod_fing ][ MSM_FINX(base##_WB) ]

#define SHADOW_FINGER_H(base) \
    MSM_fimage[ _dmod_fing ][ MSM_FINX(base##_WH) ]



/*===========================================================================

                            BIT MASK HELPER MACROS

===========================================================================*/


/* Masking operation helper macros */

#define MSM_MASK_VALUE( size, reg, mask, val ) \
    MSM_LOCK_REQUIRED \
    (reg = (reg & (size)(~(mask))) | ((size)((val) & (mask))))

#define MSM_MASK_SET( size, reg, mask ) \
    MSM_LOCK_REQUIRED \
    (reg |= (size)(mask))

#define MSM_MASK_CLEAR( size, reg, mask ) \
    MSM_LOCK_REQUIRED \
    (reg &= (size)(~(mask)))

#define MSM_MASK_STROBE_HIGH( size, reg, mask ) \
    (reg | (size)(mask))

#define MSM_MASK_STROBE_LOW( size, reg, mask ) \
    (reg & (size)(~(mask)))



/*===========================================================================

                       BIT MASK MACROS (8-bit registers)

   In the following macros, the field parameter is appended to the base
   address parameter in order to generate the field mask literal name.
   
   For example, for a base of XXX and a field of YYY, the address parameter
   would expand to "XXX_WB" (8-bit write register) and the mask parameter
   would expand to "XXX_WB__YYY_MASK" (field of the 8-bit write register.)

===========================================================================*/



/*===========================================================================

MACRO MSM_WB_INIT

DESCRIPTION     This macro zeros all bits in a "_WB" style 8-bit msm
                and the corresponding shadow register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_INIT( base ) \
    MSM_LOCK_REQUIRED \
    SHADOW_B( base ) = 0x00, \
    MSM_WB( base, SHADOW_B(base) )


#define MSM_WB_SHADOW( base, value ) \
    MSM_LOCK_REQUIRED \
    SHADOW_B( base ) = (value), \
    MSM_WB( base, SHADOW_B(base) )


/*===========================================================================

MACRO MSM_WB_RESTORE

DESCRIPTION     This macro restores all bits in a "_WB" style 8-bit msm
                to the corresponding shadow register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_RESTORE( base ) \
    MSM_LOCK_REQUIRED \
    MSM_WB( base, SHADOW_B(base) )

/*===========================================================================

MACRO MSM_WB_MASKED

DESCRIPTION     This macro alters a subset of bits in a "_WB" style
                8-bit msm register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.
                
DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_MASKED( base, field, val ) \
    MSM_MASK_VALUE( byte, SHADOW_B(base), base##_WB__##field##_MASK, val ), \
    MSM_WB(base, SHADOW_B(base) )


/*===========================================================================

MACRO MSM_WB_SET

DESCRIPTION     This macro sets a specific bit in a "_WB" style 8-bit msm
                register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.
                
DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_SET( base, field ) \
    MSM_MASK_SET( byte, SHADOW_B(base), base##_WB__##field##_MASK ), \
    MSM_WB(base, SHADOW_B(base) )


/*===========================================================================

MACRO MSM_WB_SET_BIT

DESCRIPTION     This macro sets a specific bit in a "_WB" style 8-bit msm
                register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.
                
DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_SET_BIT( base, bit ) \
    MSM_MASK_SET( byte, SHADOW_B(base), 1 << (bit) ), \
    MSM_WB(base, SHADOW_B(base) )


/*===========================================================================

MACRO MSM_WB_CLEAR

DESCRIPTION     This macro clears a specific bit in a "_WB" style 8-bit msm
                register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_CLEAR( base, field ) \
    MSM_MASK_CLEAR( byte, SHADOW_B(base), base##_WB__##field##_MASK ), \
    MSM_WB(base, SHADOW_B(base) )


/*===========================================================================

MACRO MSM_WB_CLEAR_BIT

DESCRIPTION     This macro clears a specific bit in a "_WB" style 8-bit msm
                register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_CLEAR_BIT( base, bit ) \
    MSM_MASK_CLEAR( byte, SHADOW_B(base), 1 << (bit) ), \
    MSM_WB(base, SHADOW_B(base) )


/*===========================================================================

MACRO MSM_WB_STROBE_HIGH

DESCRIPTION     This macro strobes a specific bit in a "_WB" style 8-bit msm
                register high.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.
                
DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_STROBE_HIGH( base, field ) \
    MSM_LOCK_REQUIRED \
    MSM_WB(base, MSM_MASK_STROBE_HIGH( byte, SHADOW_B(base), \
                                       base##_WB__##field##_MASK ) )


/*===========================================================================

MACRO MSM_WB_STROBE_LOW

DESCRIPTION     This macro strobes a specific bit in a "_WB" style 8-bit msm
                register low.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_STROBE_LOW( base, field ) \
    MSM_LOCK_REQUIRED \
    MSM_WB(base, MSM_MASK_STROBE_LOW( byte, SHADOW_B(base), \
                                       base##_WB__##field##_MASK ) )


/*===========================================================================

MACRO MSM_WB_PULSE

DESCRIPTION     This macro pulses (sets high and then low) a specific bit in
                a "_WB" style 8-bit msm register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image[ ]) is updated.

===========================================================================*/

#define MSM_WB_PULSE( base, field ) \
    MSM_WB_STROBE_HIGH( base, field ), \
    MSM_WB_CLEAR( base, field )


/*===========================================================================

                       BIT MASK MACROS (16-bit registers)

   In the following macros, the field parameter is appended to the base
   address parameter in order to generate the field mask literal name.
   
   For example, for a base of XXX and a field of YYY, the address parameter
   would expand to "XXX_WH" (16-bit write register) and the mask parameter
   would expand to "XXX_WH__YYY_MASK" (field of the 16-bit write register.)

===========================================================================*/



/*===========================================================================

MACRO MSM_WH_INIT

DESCRIPTION     This macro zeros all bits in a "_WH" style 16-bit msm
                and the corresponding shadow register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image_H[ ]) is updated.

===========================================================================*/

#define MSM_WH_INIT( base ) \
    MSM_LOCK_REQUIRED \
    SHADOW_H( base ) = 0x0000, \
    MSM_WH( base, SHADOW_H(base) )


/*===========================================================================

MACRO MSM_WH_MASKED

DESCRIPTION     This macro alters a subset of bits in a "_WH" style
                16-bit msm register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image_H[ ]) is updated.

===========================================================================*/

#define MSM_WH_MASKED( base, field, val ) \
    MSM_MASK_VALUE( word, SHADOW_H(base), base##_WH__##field##_MASK, val ), \
    MSM_WH(base, SHADOW_H(base) )


/*===========================================================================

MACRO MSM_WH_SET

DESCRIPTION     This macro sets a specific bit in a "_WH" style 16-bit msm
                register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image_H[ ]) is updated.

===========================================================================*/

#define MSM_WH_SET( base, field ) \
    MSM_MASK_SET( word, SHADOW_H(base), base##_WH__##field##_MASK ), \
    MSM_WH(base, SHADOW_H(base) )


/*===========================================================================

MACRO MSM_WH_CLEAR

DESCRIPTION     This macro clears a specific bit in a "_WH" style 16-bit msm
                register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image_H[ ]) is updated.

===========================================================================*/

#define MSM_WH_CLEAR( base, field ) \
    MSM_MASK_CLEAR( word, SHADOW_H(base), base##_WH__##field##_MASK ), \
    MSM_WH(base, SHADOW_H(base) )


/*===========================================================================

MACRO MSM_WH_PULSE

DESCRIPTION     This macro pulses (sets high and then low) a specific bit in
                a "_WH" style 16-bit msm register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 8-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_image_H[ ]) is updated.

===========================================================================*/

#define MSM_WH_PULSE( base, field ) \
    MSM_WH_SET( base, field ), \
    MSM_WH_CLEAR( base, field )


/*===========================================================================

                       MSM FINGER SELECTION

===========================================================================*/


/*===========================================================================

MACRO MSM_FINGER_SELECT

DESCRIPTION     This macro selects a demodulator finger for i/o operations

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.
                
                It must be followed by a MSM_FINGER_UNSELECT in the same
                function at the same block-nesting level.
                
RETURN VALUE    None.
      
SIDE EFFECTS    The finger page register is written to.

===========================================================================*/

#define MSM_FINGER_SELECT( finger )    \
    { \
      word _dmod_fing = (finger); \
      MSM_LOCK_REQUIRED \
      MSM_WB( FFE_FINGER_PAGE_REG, 1 << _dmod_fing )

#define MSM_FINGER_SELECT_REQUIRED  \
    _dmod_fing = _dmod_fing;

/*===========================================================================

MACRO MSM_FINGER_UNSELECT

DESCRIPTION     This macro unselects the selected demodulator finger

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_LOCK / MSM_UNLOCK pair.
                
                It must be preceeded by a MSM_FINGER_SELECT in the same
                function at the same block-nesting level.

RETURN VALUE    None.
      
SIDE EFFECTS    None.

===========================================================================*/

#define MSM_FINGER_UNSELECT( ) \
        MSM_FINGER_SELECT_REQUIRED \
    }


/*===========================================================================

                       FINGER BIT MASK MACROS (8-bit registers)

   In the following macros, the field parameter is appended to the base
   address parameter in order to generate the field mask literal name.
   
   For example, for a base of XXX and a field of YYY, the address parameter
   would expand to "XXX_WB" (8-bit write register) and the mask parameter
   would expand to "XXX_WB__YYY_MASK" (field of the 8-bit write register.)

===========================================================================*/


/*===========================================================================

MACRO MSM_FINGER_WB_INIT

DESCRIPTION     This macro zeros all bits in a "_WB" style 8-bit msm
                finger register and the corresponding shadow register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The address must be passed in as a literal.  Variables are
                not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_FINGER_SELECT / MSM_FINGER_DESELECT pair inside
                the content of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_fimage[ ]) is updated.

===========================================================================*/

#define MSM_FINGER_WB_INIT( base ) \
    SHADOW_FINGER_B( base ) = 0x00, \
    MSM_WB( base, SHADOW_FINGER_B(base) )

#define MSM_FINGER_WB_SHADOW( base, value ) \
    SHADOW_FINGER_B( base ) = (value), \
    MSM_WB( base, SHADOW_FINGER_B(base) )


/*===========================================================================

MACRO MSM_FINGER_WB_MASKED

DESCRIPTION     This macro alters a subset of bits in a "_WB" style
                8-bit msm finger register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.
                
DEPENDENCIES    This function must only be called from inside the context
                of a MSM_FINGER_SELECT / MSM_FINGER_DESELECT pair inside
                the content of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_fimage[ ]) is updated.

===========================================================================*/

#define MSM_FINGER_WB_MASKED( base, field, val ) \
    MSM_MASK_VALUE( byte, SHADOW_FINGER_B(base), base##_WB__##field##_MASK, val ), \
    MSM_WB(base, SHADOW_FINGER_B(base) )

/*===========================================================================

MACRO MSM_FINGER_WH_MASKED

DESCRIPTION     This macro alters a subset of bits in a "_WH" style
                16-bit msm finger register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.
                
DEPENDENCIES    This function must only be called from inside the context
                of a MSM_FINGER_SELECT / MSM_FINGER_DESELECT pair inside
                the content of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_fimage[ ]) is updated.

===========================================================================*/

#define MSM_FINGER_WH_MASKED( base, field, val ) \
    MSM_MASK_VALUE( word, SHADOW_FINGER_H(base), base##_WH__##field##_MASK, val ), \
    MSM_WH(base, SHADOW_FINGER_H(base) )


/*===========================================================================

MACRO MSM_FINGER_WB_SET

DESCRIPTION     This macro sets a specific bit in a "_WB" style 8-bit msm
                finger register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.
                
DEPENDENCIES    This function must only be called from inside the context
                of a MSM_FINGER_SELECT / MSM_FINGER_DESELECT pair inside
                the content of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_fimage[ ]) is updated.

===========================================================================*/

#define MSM_FINGER_WB_SET( base, field ) \
    MSM_MASK_SET( byte, SHADOW_FINGER_B(base), base##_WB__##field##_MASK ), \
    MSM_WB(base, SHADOW_FINGER_B(base) )

#define MSM_FINGER_WB_SET_BIT( base, bit ) \
    MSM_MASK_SET( byte, SHADOW_FINGER_B(base), 1 << (bit) ), \
    MSM_WB(base, SHADOW_FINGER_B(base) )

/*===========================================================================

MACRO MSM_FINGER_WH_SET

DESCRIPTION     This macro sets a specific bit in a "_WH" style 16-bit msm
                finger register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.
                
DEPENDENCIES    This function must only be called from inside the context
                of a MSM_FINGER_SELECT / MSM_FINGER_DESELECT pair inside
                the content of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_fimage[ ]) is updated.

===========================================================================*/

#define MSM_FINGER_WH_SET( base, field ) \
    MSM_MASK_SET( word, SHADOW_FINGER_H(base), base##_WH__##field##_MASK ), \
    MSM_WH(base, SHADOW_FINGER_H(base) )

#define MSM_FINGER_WH_SET_BIT( base, bit ) \
    MSM_MASK_SET( word, SHADOW_FINGER_H(base), 1 << (bit) ), \
    MSM_WH(base, SHADOW_FINGER_H(base) )


/*===========================================================================

MACRO MSM_FINGER_WB_CLEAR

DESCRIPTION     This macro clears a specific bit in a "_WB" style 8-bit msm
                finger register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_FINGER_SELECT / MSM_FINGER_DESELECT pair inside
                the content of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_fimage[ ]) is updated.

===========================================================================*/

#define MSM_FINGER_WB_CLEAR( base, field ) \
    MSM_MASK_CLEAR( byte, SHADOW_FINGER_B(base), base##_WB__##field##_MASK ), \
    MSM_WB(base, SHADOW_FINGER_B(base) )

#define MSM_FINGER_WB_CLEAR_BIT( base, bit ) \
    MSM_MASK_CLEAR( byte, SHADOW_FINGER_B(base), 1 << (bit) ), \
    MSM_WB(base, SHADOW_FINGER_B(base) )

/*===========================================================================

MACRO MSM_FINGER_WH_CLEAR

DESCRIPTION     This macro clears a specific bit in a "_WH" style 8-bit msm
                finger register.
                
                "_WH" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_FINGER_SELECT / MSM_FINGER_DESELECT pair inside
                the content of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_fimage[ ]) is updated.

===========================================================================*/

#define MSM_FINGER_WH_CLEAR( base, field ) \
    MSM_MASK_CLEAR( word, SHADOW_FINGER_H(base), base##_WH__##field##_MASK ), \
    MSM_WH(base, SHADOW_FINGER_H(base) )

#define MSM_FINGER_WH_CLEAR_BIT( base, bit ) \
    MSM_MASK_CLEAR( word, SHADOW_FINGER_H(base), 1 << (bit) ), \
    MSM_WH(base, SHADOW_FINGER_H(base) )


/*===========================================================================

MACRO MSM_FINGER_WB_PULSE

DESCRIPTION     This macro pulses (sets high and then low) a specific bit in
                a "_WB" style 8-bit msm finger register.
                
                "_WB" is appended to the address to ensure that the macro
                cannot be used with 16-bit registers or input registers.
                
                The base address and field parameters must be passed in as
                literals.  Variables are not allowed.

DEPENDENCIES    This function must only be called from inside the context
                of a MSM_FINGER_SELECT / MSM_FINGER_DESELECT pair inside
                the content of a MSM_LOCK / MSM_UNLOCK pair.

RETURN VALUE    None.
      
SIDE EFFECTS    The corresponding shadow register (MSM_fimage[ ]) is updated.

===========================================================================*/

#define MSM_FINGER_WB_PULSE( base, field ) \
    MSM_FINGER_WB_SET( base, field ), \
    MSM_FINGER_WB_CLEAR( base, field )


/*===========================================================================

                               End of Interface

===========================================================================*/

#endif /* MSM_HELP_H */

