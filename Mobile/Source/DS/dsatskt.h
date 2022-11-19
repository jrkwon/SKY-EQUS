#ifndef	DSATSKT_H
#define	DSATSKT_H

#ifdef	FEATURE_SKT_DR_DS	//	khekim 00/02/14

#include	"customer.h"
#include	"comdef.h"
#include	"target.h"
#include	"ds.h"
#include	"dsi.h"
#include	"dsatcopi.h"

extern	dflm_type dsAtSktDrDflm [];
extern	ds_atcop_num_item_type dsAtSktDrVal;


extern	void	AtSktDrCommand
(
	const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
	tokens_struct_type *tok_ptr,  /*  Command tokens from parser  */
	byte *rb_ptr                  /*  point to buffer for result  */
);

#endif	//	#ifdef	FEATURE_SKT_DR_DS	//	khekim 00/02/14

#endif	//	#ifndef	DSATSKT_H
