#ifndef VAS_H
#define VAS_H
/*===========================================================================

              VAS   I N T E R F A C E   H A N D L E R


DESCRIPTION

  This include file contains the prototypes of the functions that are shared
  among VAS Handler files. It also, exports the global variables that are
  shared.
  
  Copyright (c) 1999 through 2000 by QUALCOMM INCORPORATED  All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/06/18   jmpark  VAS state handler

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#if 0
typedef enum {
	WAP  = 1,
	JAVA,
	GVM ,  
	MAX
} service_name_type;

typedef struct  
{
	service_name_type    KindOfService;  //service kind
									 //WAP = 1, JAVA = 2, 
} VASSTATE_T;

extern VASSTATE_T gVasState;


#define GET_SERVICE_STATE               (  gVasState.KindOfService           )
#define SET_SERVICE_STATE( fValue )     (  gVasState.KindOfService = fValue  )

#define HI_BYTE(word)	((unsigned char)(((word)>>8) & (0x00ff)))
#define LO_BYTE(word)	((unsigned char)((word) & (0x00ff)))
#define MAKE_HALF(h, l) ((unsigned short)((((h)&0x00ff)<<8) | ((l)&0x00ff)))
#define MAKE_LONG(uh,ul,lh,ll) ((unsigned long)( ((uh)<<24) | ((ul)<<16) | \
												 ((lh)<< 8) | ((ll)) ) )

#define MUPPER_BYTE(exp)         ((unsigned char)(((exp)>>24) & (0x000000ff)))
#define MLOWER_BYTE(exp)		 ((unsigned char)(((exp)>>16) & (0x000000ff)))
#define LUPPER_BYTE(exp)		 ((unsigned char)(((exp)>> 8) & (0x000000ff)))
#define LLOWER_BYTE(exp)		 ((unsigned char)(((exp)    ) & (0x000000ff)))

#ifdef WIN32
	#define DPRINTF(msg)	printf msg
#else
	#define DPRINTF(msg)	
#endif//win32

extern void SetKindOfService(int Service);
extern int GetKindOfService(void);

#endif//0

#endif//VAS_H