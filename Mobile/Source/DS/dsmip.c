/* -----------------------------------------------------------------------------------
**
**		Implement the parsing of AT specific Command for SK Telecom
** 
** -----------------------------------------------------------------------------------
*/

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====

GENERAL DESCRIPTION
  This module implements the Mobile IP

EXTERNALIZED FUNCTIONS

when		who			what, where, why
--------	---			---------------------------------------------------------------
00/08/08    khekim      Created
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "comdef.h"
#include "customer.h"
#ifdef	FEATURE_SKT_MIP_DS	//	khekim 00/08/07
#include "target.h"
#include "err.h"
#include "bbver.h"
#include "dsm.h"
#include "dsi.h"
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
#include "msg.h"
#include "err.h"

#include "icmp.h"
#include "ip.h"

#include "dsmip.h"


////////////////////////////////////////////////////////////////////////////////////////
// ICMP Agent Solicitation Message
void SendIcmpAgentSolicitMsg
(
	ip4a	source
)
{
	struct icmp		icmp;
	dsm_item_type	*pItem;
	uint8			pDestIP[4] = {224, 0, 0, 2};	//	Destination Address

	MSG_HIGH("SendIcmpAgentSolicitMsg", 0, 0, 0);

	icmp.type = ICMP_SOLICIT;	// 8bits
	icmp.code = 0;				// 8bits
								// 16bits icmp->checksum
	icmp.args.unused = 0;		// 32bits reserved

	pItem = dsm_new_buffer(DSM_SMALL_ITEM_SIZ);
	new_item_ptr->kind = DSM_APP_PAYLOAD;
	pItem->used = sizeof(struct icmp);
	memcpy((uint8*)pItem->data_ptr, (uint8*)&icmp, pItem->used);

	htonicmp(icmp, &pItem);		// pItem=> address of pointer to ICMP packet data

//	ip_send( source, dest, ICMP_PTCL, tos, ttl, pItem, pItem->used, id, dfflag);
	ip_send( htonl(source), htonl(*(ip4a *)&pDestIP[0]), ICMP_PTCL, 0, 1, pItem, pItem->used, 0, 1);
}

#endif	//	#ifdef	FEATURE_SKT_MIP_DS