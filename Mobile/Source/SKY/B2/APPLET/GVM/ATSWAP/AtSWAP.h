#ifndef	ATSWAP_H	
#define	ATSWAP_H
////////////////////////////////
// Skt_Esms.h
//
#include "customer.h"
#include "comdef.h"

////////////////////////////////
// Serial to KeyJudge Mail ID
//
#ifdef	FEATURE_GVM

#define	SKT_SWAP_CMD			0x8B

// External interface
extern	void	skt_swapInitialize(void);		// Initialize
extern	unsigned char		at_swap_command(unsigned char **command);

// Added by SISI, 2000/08/01
#endif	//	#ifdef	FEATURE_GVM
#endif	//	#ifndef	ATSWAP_H
