/*************************************************************************
*   Copyright (c) 1998-1999 Phone.com, Inc. All rights reserved.         *
*                                                                        *
*    THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Phone.com, Inc.      *
*    The copyright notice above does not evidence any                    *
*    actual or intended publication of such source code.                 *
*************************************************************************/


#ifndef	__WDPTYPES_H__
#define	__WDPTYPES_H__

/* This file should be standalone, since it will be shipped with the
   porting kit*/


#ifdef __cplusplus
extern "C"	{
#endif

/********************* Bearer Types  *****************************/

typedef enum	{
		CBearer_IPv4 			= 0x00,
		CBearer_IPv6			= 0x01,
		CBearer_GSM_USSD		= 0x02,
		CBearer_GSM_SMS			= 0x03,
		CBearer_IS_136_R_DATA 	= 0x04,
		CBearer_CDMA_SMS		= 0x05,
		CBearer_PHS_SMS			= 0xFC,
		CBearer_PDC_SMS			= 0xFD,
		CBearer_SMSIP           = 0xFF,
		CBearer_UNKNOWN         = 0x80

} TWDPBearerType;

#define WDP_IPv4_ADDR_LEN       4
#define WDP_SMS_ADDR_LEN        10	/* stored in semi-octet format*/


/* MAX_WDP_ADDR_LEN is the max possible bearer address length. As new
address types are added this should be changed to reflect the actual
maximum*/

/* MAX_WDP_ADDR_LEN can be defined by device vendors to specify the max.
possible address length in the device.*/

#ifndef MAX_WDP_ADDR_LEN
#define MAX_WDP_ADDR_LEN	25
#endif

typedef struct {
        unsigned char    addrLen;
        unsigned char    bearerTypes;
        unsigned char    portNum[2];
        unsigned char    bearerAddress[MAX_WDP_ADDR_LEN];
} WDPAddress;


#ifdef __cplusplus
}
#endif
#endif
