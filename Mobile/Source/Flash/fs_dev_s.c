/*


	fs_dev.c에서 사용하는 RAM프로그램 


------------------------------------------------
  99.11.6	jnlee		create

*/


#include "customer.h"
#ifdef FEATURE_EFS
#include "err.h"
//#include "msg.h" 
#include "fs_dev.h"
#include "fs_devi.h"
#include "clk.h"
#include "dmod.h"
#include "msg.h"

/* Device base address. */
extern volatile word *fs_dev_base;

/* Which flash component did we detect. */
extern fsi_device *fsi_target;

extern fsi_erase_state_type fsi_erase_state;

/* Most components need an address to check status of, or continue an
   erase. */
extern dword fsi_erase_location;



/* start of insert		by jnlee 99.11.4 */
/* Enable,Disable Interrupt	by jnlee 99.11.4 */
#ifndef _WINDOWS		// browser 파일 만들기 위함 99.10.11
#define  DISABLE_INTERRUPTS(ikey0,ikey1)	\
							__asm{	mrs     r0, CPSR;		\
									orr     r1, r0, 0xc0;	\
									msr     CPSR, r1;		\
									and     r0, r0, 0xc0;	\
									mov		ikey0,r0;		}\
								ikey1=ikey0

#define  ENABLE_INTERRUPTS(ikey0,ikey1) if(!ikey0)	{		\
							__asm{	mrs     r0, CPSR;		\
									and     r1, r0, 0x3f;	\
									msr     CPSR, r1;		\
									and     r0, r0, 0xc0;	\
									}\
								}	ikey0=ikey1			

#else		// _WINDOWS
#define  DISABLE_INTERRUPTS(ikey0,ikey1)	
#define  ENABLE_INTERRUPTS(ikey0,ikey1) 
#endif		// _WINDOWS

extern boolean dog_auto_kicking;		

#define EFS_DOG_KICK()																							\
  if ( dog_auto_kicking==FALSE) {																				\
    /* DMOD_OUTM( DMOD_SLEEP_CTL, DMOD_DOG_M, DMOD_DOG_V ); */													\
	MSM_image_H[(DMOD_SLEEP_CTL-MSM_BASE)/4] = MSM_image_H[(DMOD_SLEEP_CTL-MSM_BASE)/4] | (word)DMOD_DOG_V;		\
	*((volatile byte *) (DMOD_SLEEP_CTL)) = (byte)MSM_image_H[(DMOD_SLEEP_CTL-MSM_BASE)/4];						\
	/* DMOD_OUTM( DMOD_SLEEP_CTL, DMOD_DOG_M, 0 ); */															\
	MSM_image_H[(DMOD_SLEEP_CTL-MSM_BASE)/4] = MSM_image_H[(DMOD_SLEEP_CTL-MSM_BASE)/4] & ~((word)DMOD_DOG_V);	\
	*((volatile byte *) (DMOD_SLEEP_CTL)) = (byte)MSM_image_H[(DMOD_SLEEP_CTL-MSM_BASE)/4];						\
  }

extern word tramp_int_mask_0;    /* Interrupt status mask for int reg #0 */
extern word tramp_int_mask_1;    /* Interrupt status mask for int reg #1 */

#define INTERRUPT_PENDING	 \
	(MSM_INHM( INT_STATUS_0_RH, tramp_int_mask_0 )| MSM_INHM( INT_STATUS_1_RH, tramp_int_mask_1 ))


// inserted by jnlee 2000.1.28
#define ERASE_WHILE_LOOP	2000
word *saved_erase_ptr;			
/* end of insert		by jnlee 99.11.4	*/


/*===========================================================================

FUNCTION FSI_INTEL_ERASE

DESCRIPTION
  Initiate erase operation for an INTEL part.

DEPENDENCIES
  fs_dev_init must be invoked prior to this call.

RETURN VALUE
  FS_OKAY_S            -  If erase operation was successfully initiated
  FS_GC_IN_PROGRESS_S  -  If an erase operation is already in progress 
  FS_FAIL_S            -  If erase operation could not otherwise be started

SIDE EFFECTS
  None

===========================================================================*/
LOCAL fs_status_type
ROM_fsi_intel_erase (dword offset)
{
	volatile word *wptr;
	word tmp;	
	fs_status_type result = FS_OKAY_S;
	int ikey0,ikey1;

	word lp=0;

	if(offset == 0xff)
	{	// erase suspend
		DISABLE_INTERRUPTS(ikey0,ikey1);
		fsi_erase_state = FSI_ERASING;
		wptr = saved_erase_ptr;			//resume시 사용하기 위해 저장 버퍼 다시 읽기 
		*wptr  = 0xd0;					//erase resume
#if MODEL_ID== MODEL_B2	// by jnlee 01.07.04
		*wptr = 0x70;	// show status for intel W30 series
#endif
	}
	else if (offset == 0xf0)
	{	//sw_reset에서 사용함 
		fsi_erase_state = FSI_ERASING;
		wptr = saved_erase_ptr;
		DISABLE_INTERRUPTS(ikey0,ikey1);
		*wptr = 0xd0;	// erase resume
#if MODEL_ID== MODEL_B2	// by jnlee 01.07.04
		*wptr = 0x70;	// show status for intel W30 series
#endif
		while(!( (tmp=*wptr) & 0x80 ) )
			EFS_DOG_KICK();
		*wptr = 0x60;	//lock
		*wptr = 0x01;	//lock
		while(!(*wptr & 0x80))
			EFS_DOG_KICK();
		*wptr =0xff;	//read state
		if((tmp &0x2a))		//flash error
		{
			*wptr =0x50;	//clear status
			*wptr =0x70;	//clear status
			while( !( *wptr & 0x80) )
				EFS_DOG_KICK();	//wait till clear
			*wptr =0xff;	//read state			
			result = FS_FAIL_S;
		}
		fsi_erase_state = FSI_READ_MODE;
		ENABLE_INTERRUPTS(ikey0,ikey1);
		return(result);
	}
	else
	{	// 처음 erase로 들어온 경우 

		/* Determine if we have to check if an erase has terminated or
		 not. */
		switch (fsi_erase_state)
		{
		case FSI_READ_MODE:
		break;                                   /* Easy. */

		case FSI_ERASING:
		/* Must check to see if this erase is still running. */
		//    ERR_FATAL ("not yet written 123", 0, 0, 0);
		break;

		case FSI_ERASE_SUSPENDED:
//			MSG_MED ("fsi_dev: Attempt to start erase while erase is suspended",
//				 0, 0, 0);
		return FS_FAIL_S;
		}

	  /* If we made it here, then the component is now sitting in read
		 mode.  Our offset is in bytes from the beginning of NV, get a
		 pointer to the real data. */
		fsi_erase_location = fsi_target->nv_offset + offset / 2;
		wptr = &fs_dev_base[fsi_erase_location];

		// suspend에서 다시 resume하기 위해서 저장해 놓음 
		saved_erase_ptr = (word *)wptr;			//inserted by jnlee 99.12.31

		DISABLE_INTERRUPTS(ikey0,ikey1);
		*wptr = 0x60;		//unlock
		*wptr = 0xd0;		//unlock
		*wptr = 0x20;		// erase 
		*wptr = 0xD0;
	}

	fsi_erase_state = FSI_ERASING;
	while(1){
		while(  (((tmp=*wptr)&0x80) ==0) && !INTERRUPT_PENDING && (lp<ERASE_WHILE_LOOP) ){
		 EFS_DOG_KICK();
		 lp++;
		}
		if( (tmp&0x80)==0){			// interrup 발생 
			*wptr = 0xb0;		//suspend
			*wptr = 0x70;		//suspend 
			//wait till read status
			while( !(*wptr&0x80) );
			if( (*wptr&0x40) )
			{	// erase suspend 
				*wptr = 0xff;
				fsi_erase_state = FSI_ERASE_SUSPENDED;
				ENABLE_INTERRUPTS(ikey0,ikey1);
				if(lp >= ERASE_WHILE_LOOP)		
				{// 지정 루프 회수 초과시 그냥 suspend상태로 리턴함 
					return result;
				}
				DISABLE_INTERRUPTS(ikey0,ikey1);
				*wptr = 0xd0;		//resume
#if MODEL_ID == MODEL_B2		// by jnlee 01.07.04
				*wptr = 0x70;		//show status for intel W30 series
#endif
			}
			else
			{	// erase complete
//				*wptr = 0xff;
				break;
			}
		}
		else	//정상적으로 마친 경우 
		  break;
	}

	//정상적으로 끝마친 경우 
	*wptr =0x60;	//lock
	*wptr =0x01;	//lock
//	while( !( *wptr & 0x80) );	// wait till clear
	*wptr =0xff;	//read state
	ENABLE_INTERRUPTS(ikey0,ikey1);
	/* 0x20 = SR.5 - Erase Status.
	   0x08 = SR.3 - Vpp Status.
	   0x02 = SR.1 - Block lock status.
	   if any of these bits are set, this is an error. */
	if((tmp &0x2a))		//flash error
	{
		DISABLE_INTERRUPTS(ikey0,ikey1);
		*wptr =0x50;	//clear status
		*wptr =0x70;	//clear status
		while( !( *wptr & 0x80) );	// wait till clear
		*wptr =0xff;	//read state			
		ENABLE_INTERRUPTS(ikey0,ikey1);
		result = FS_FAIL_S;
	}
	fsi_erase_state = FSI_READ_MODE;
	return result;
}

/* by jnlee 99.11.4
*/
void ROM_fsi_intel_erase_end(void)		
{	/* This func is null func to define end of fsi_intel_write() */ }

/*===========================================================================

FUNCTION FSI_INTEL_WRITE

DESCRIPTION
  Write a block of bytes to an INTEL part.

DEPENDENCIES
  The fs_dev_init must have been called.  The device must not be in
  erasing state.

RETURN VALUE
  FS_OKAY_S      -  If write completed normally
  FS_FAIL_S      -  If write operation failed

SIDE EFFECTS
  None

===========================================================================*/
fs_status_type
ROM_fsi_intel_write (byte *buffer,
               dword offset,
               dword count)
{
	volatile byte *part_base, *bptr;
	volatile word *wptr;
	word value;
	word tmp;
	int ikey0,ikey1;

	// MSG_LOW ("fsi_intel_write, %x, %x, %x", buffer, offset, count);

	/* Verify that the requested data is in range.  The user see's the
	 component as starting at zero, so we only need to check the end. */
	if ((offset + count) > (fsi_target->nv_size * 2))
	{
//		MSG_MED ("fs_dev: write request out of range", 0, 0, 0);
//		return FS_FAIL_S;
	}

	/* We cannot write while an erase is happening. */
	if (fsi_erase_state != FSI_READ_MODE
	  && fsi_erase_state != FSI_ERASE_SUSPENDED)
	{
//		MSG_MED ("fs_dev: attempt to write while device is erasing", 0,
//				 0, 0);
		return FS_FAIL_S;
	}

	/* !!! Put check here if we are trying to write the sector that is
	 in erase suspend !!! */

	/* Base address of operation. */
	part_base = ((volatile byte *) &fs_dev_base[fsi_target->nv_offset]
			   + offset);

	/* Determine if the block is aligned or not. */
	if (((dword) buffer & 1) == 0
	  && (offset & 1) == 0
	  && (count & 1) == 0)
	{
		/* Do word operations. */
		wptr = (volatile word *) part_base;

		/* Since we're already aligned, we could compare with 0, but the
		   1 will be needed for future optimizations. */
		while (count > 1)
		{	
			value = *((word *) buffer);

			DISABLE_INTERRUPTS(ikey0,ikey1);
			*wptr = 0x60;		//unlock
			*wptr = 0xd0;		//unlock
			*wptr = 0x40;		//write command
			*wptr = value;

			while(  (((tmp=*wptr)&0x80) ==0) )
				EFS_DOG_KICK();

			*wptr =0x60;	//lock
			*wptr =0x01;	//lock
			while( !( *wptr & 0x80) );	// wait till clear

			if((tmp &0x3a))		//flash error
			{
				DISABLE_INTERRUPTS(ikey0,ikey1);
				*wptr =0x50;	//clear status
				*wptr =0x70;	//clear status
				while( !( *wptr & 0x80) );	// wait till clear
				*wptr =0xff;	//read state			
				ENABLE_INTERRUPTS(ikey0,ikey1);
				return FS_FAIL_S;
			}
//			fsi_erase_state = FSI_READ_MODE;

			*wptr =0xff;	//read state
			ENABLE_INTERRUPTS(ikey0,ikey1);


			buffer += 2;
			wptr   += 1;
			count  -= 2;
		}
	}
	else
	{
		/* Do byte operations. */
		bptr = part_base;

		while (count > 0)
		{
			value = *buffer;

			/* The command accesses need to be word oriented or the data
			 will be placed on the wrong data lines.  Compute a word
			 pointer from the byte pointer that is always at an even
			 address. */
			wptr = ((volatile word *) ((dword) bptr & ~1));

			DISABLE_INTERRUPTS(ikey0,ikey1);

			/* Get old value. */
			*wptr = 0xFF;
			tmp = *wptr;

			*wptr = 0x60;		//unlock
			*wptr = 0xd0;		//unlock
			*wptr = 0x40;
			/* Assumes little endian. */
			if (((dword) bptr & 1) == 0)
			value = tmp & (value | 0xFF00);
			else
			value = tmp & ((value << 8) | 0x00FF);

			*wptr = value;

			//MSG_LOW ("write: %02x to %04x", value, bptr, 0);

			while(  (((tmp=*wptr)&0x80) ==0) )
				EFS_DOG_KICK();

			*wptr =0x60;	//lock
			*wptr =0x01;	//lock
			while( !( *wptr & 0x80) );	// wait till clear

			if((tmp &0x3a))		//flash error
			{
				DISABLE_INTERRUPTS(ikey0,ikey1);
				*wptr =0x50;	//clear status
				*wptr =0x70;	//clear status
				while( !( *wptr & 0x80) );	// wait till clear
				*wptr =0xff;	//read state			
				ENABLE_INTERRUPTS(ikey0,ikey1);
				return FS_FAIL_S;
			}
//			fsi_erase_state = FSI_READ_MODE;

			*wptr =0xff;	//read state
			ENABLE_INTERRUPTS(ikey0,ikey1);


			buffer += 1;
			bptr   += 1;
			count  -= 1;
		}
	}
	/* Exit write mode. */
	wptr = (volatile word *) ((dword) part_base & ~1);
	*wptr = 0xFF;

	return FS_OKAY_S;
}

/* by jnlee 99.11.4
*/
void ROM_fsi_intel_write_end(void)		
{/* This func is null func to define end of fsi_intel_write() */}


#endif /* FEATURE_EFS */
