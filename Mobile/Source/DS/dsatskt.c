/* -----------------------------------------------------------------------------------
**
**		Implement the parsing of AT specific Command for SK Telecom ESMS
** 
** -----------------------------------------------------------------------------------
*/

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====

GENERAL DESCRIPTION
  This module implements the parsing of AT modem commands for
  SK Telecom

EXTERNALIZED FUNCTIONS

  AtSktDrCommand	parse at*skt*dr command

when		who			what, where, why
--------	---			---------------------------------------------------------------
00/02/20	khekim		dsAtSktDrDflm[] const 선언 제거
00/02/14	khekim		AtSktDrCommand()구현: at*skt*dr command parsing
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "comdef.h"
#include "customer.h"
#ifdef FEATURE_DS
#ifdef	FEATURE_SKT_DR_DS	//	khekim 00/02/14
#include "target.h"
#include "err.h"
#include "bbver.h"
#include "dsm.h"
#include "dsi.h"
#include "dsmgr.h"
#include "dsatcopi.h"
#include "db.h"
#include "tcp.h"
#include "mobile.h"
#include "sio.h"
#include "dsrmio.h"
#include "ui.h"
#include "ps.h"
#include "memory.h"
#include "assert.h"
#include "diag.h"
#include "nv.h"

#include "uiuint.h"	//	ui_get_nv()
#include "dsskt.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// The following type is defined in dsatcop.c
// PROTOTYPES
	typedef enum
	{                     /*  ds_atcop_atoi result returns:   */
		ATOI_OK,              /*  conversion complete             */
		ATOI_NO_ARG,          /*  nothing to convert              */
		ATOI_OUT_OF_RANGE     /*  character or value out of range */
	} atoi_enum_type;

	typedef enum            /*  specifies the format type for   */
	{                     /*    displaying AT parameters      */

		RESPONSE,             /*  Generates response to command - */
							/*    No name for +Fxxx             */

		RESPONSE_ALL,         /*  Generates response to command - */
							/*    Names all commands            */

		SET                   /*  Generates a SET command - used  */
							/*    to transmit configuaration    */
							/*    to the base station           */
	} fmt_enum_type;


//////////////////////////////////////////////////////////////////////////////////////////////
// EXTERNAL VARIABLES
// ds_atcop_fmt_num_range() is defined in dsmgr.c 
extern	unsigned int ds_atcop_fmt_num_range
(
  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */

  byte *rb_ptr                  /*  point to buffer for result  */
) ;

// ds_atcop_fmt_num_val_ex() is defined in dsmgr.c 
extern	unsigned int ds_atcop_fmt_num_val_ex
(
  const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
  byte *rb_ptr,                 /*  point to buffer for result  */
  fmt_enum_type fmt_sw          /*  selects response or set fmt */
);

// ds_atcop_srch_list() is defined in dsmgr.c 
extern	atoi_enum_type ds_atcop_srch_list
(
	ds_atcop_num_item_type *val_arg_ptr,
	const list_type l,
	byte * arg
);

// ds_atcop_atoi() is defined in dsmgr.c 
extern	atoi_enum_type ds_atcop_atoi
(
  ds_atcop_num_item_type *val,  /*  value returned  */
  const byte *s,                /*  points to string to eval  */
  unsigned int r                /*  radix */
);


//////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
dflm_type dsAtSktDrDflm [] =
{
	{0, 0, 2},
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION
void	AtSktDrCommand
(
	const ds_atcop_cmd_type *tab_entry, /*  entry in parse table  */
	tokens_struct_type *tok_ptr,  /*  Command tokens from parser  */
	byte *rb_ptr                  /*  point to buffer for result  */
)
{
  ds_atcop_num_item_type scr_val [MAX_ARG];
  byte flag [MAX_ARG];
  atoi_enum_type e;
  word radix;
  unsigned int j;


  *rb_ptr = '\0';

	if (tok_ptr->op == (NA))            /*  Name only - set to default  */
	{
		* ((ds_atcop_num_item_type *)tab_entry->val_ptr) = 
			((dflm_type *) tab_entry->def_lim_ptr)[0].default_v;
	}

	else if (tok_ptr->op == (NA|EQ|QU)) /*  TEST command  */
	{
		(void) ds_atcop_fmt_num_range (tab_entry, rb_ptr);
	}

	else if (tok_ptr->op == (NA|QU))    /*  READ command  */
	{
		(void) ds_atcop_fmt_num_val_ex (tab_entry, rb_ptr, RESPONSE);
	}

	else if (tok_ptr->op == (NA|EQ|AR)) /*  SET command   */
	{
//	(void) ds_atcop_put_num_arg (tab_entry, tok_ptr);
// 아래 문장은 ds_atcop_put_num_arg ()를 변형한 것임
		radix = ( (tab_entry->attrib & HEX) ? 16 : 10);

		if (tok_ptr->args_found > tab_entry->compound)
		{
			ds_atcop_result = DS_ATCOP_ERROR; /*  Too many arguments  */
			DS_ATCOP_ERROR_MSG();
			return;
		}

		for (j = 0; j < tok_ptr->args_found; ++j)
		{
			flag [j] = 0;
			if ((tab_entry->attrib & LIST) == 0)
			{
				e = ds_atcop_atoi (&scr_val [j], tok_ptr->arg[j], radix);
				if (e == ATOI_OK &&
					(scr_val [j] < ((dflm_type *) tab_entry->def_lim_ptr)[j].lower
					|| scr_val [j] > ((dflm_type *) tab_entry->def_lim_ptr)[j].upper))
				{
					e = ATOI_OUT_OF_RANGE;
				}
			}
			else
			{
				e = ds_atcop_srch_list (&scr_val [j], ((def_list_type *) tab_entry->def_lim_ptr)->list_v, *tok_ptr->arg);
			}
#ifdef DUMP_ARG /*  Unit test debugging */
			printf ("arg[%d] = %d, err=%d\n", j, scr_val[j], e);
#endif  /*  DUMP_ARG  */
			if (e == ATOI_OUT_OF_RANGE)
			{
				DS_ATCOP_ERROR_MSG();
				ds_atcop_result = DS_ATCOP_PARAM;
				return;
			}
			else if (e == ATOI_OK)
			{
				flag [j] = 1;
//				displayDataBanner(check_dtr()); 
//				ds_nv_item.data_rev_mode = dsAtSktDrVal;
//				(void) ui_put_nv(NV_DATA_REV_MODE_I, &ds_nv_item);
#ifdef DUMP_ARG /*  Unit test debugging */
		  printf (", flag = %d\n", flag [j]);
#endif  /*  DUMP_ARG  */
			/*else if (e == ATOI_NO_ARG) do nothing: flag[j] remains unset  */
			/*    and the value is not stored                               */
			}
		}
		for (j = 0; j < tok_ptr->args_found; ++j)
		{
			if (flag[j])
			{
				* ((ds_atcop_num_item_type *)tab_entry->val_ptr + j)
				  = scr_val [j];
			}
		}
	}
	else                                /*  Didn't make sense - ERROR */
	{
		ds_atcop_result = DS_ATCOP_ERROR;
		DS_ATCOP_ERROR_MSG();
		return;		// khekim 00/1/21
	}
	MSG_HIGH("AT*SKT*DR()>> dsAtSktDrVal %d", dsAtSktDrVal, 0, 0);

#ifdef	FEATURE_SKT_DS_MMI	//	khekim 00/04/04
//	ui_get_nv(NV_DATA_REV_MODE_I, &ds_nv_item);
	dsi_get_nv_item(NV_DATA_REV_MODE_I, &ds_nv_item);
	if (dsAtSktDrVal != ds_nv_item.data_rev_mode)
	{
		ds_nv_item.data_rev_mode = dsAtSktDrVal;
		(void) dsi_put_nv_item(NV_DATA_REV_MODE_I, &ds_nv_item);
	}
#endif	//	#ifdef	FEATURE_SKT_DS_MMI
}

#endif	//	#ifdef	FEATURE_SKT_DR_DS	//	khekim 00/02/14
#endif	//	#ifdef FEATURE_DS
