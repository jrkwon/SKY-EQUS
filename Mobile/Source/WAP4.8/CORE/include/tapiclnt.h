/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
#ifndef TAPICLNT_H
#define TAPICLNT_H

#include "tapicmmn.h"


/*
    UINT8   type,
        0 = integer
        1 = float
        2 = string
        3 = boolean
        4 = invalid

    INT32   intVal, (32 bit integer)
        If the type indicates integer, the value is stored here.

    FLOAT32 floatVal, (32 bit floating point following the ANSI/IEEE Standard 754)
        If the type indicates float, the value is stored here.

    INT16   stringIANAcharset,
        If the type is string then this value is the MIBenum IANA code 
        for the character encoding used in the string (stringVal).
        For instance 1000 == UCS-2 (Unicode) and 36 == KSC5610.
        The character set must be supported by the Generic WAP Client
        (see INIT_ACCEPTCHARSET).

    UINT32  stringLengthInBytes,
        If the type is string then this is the length (in number of bytes) 
        of the string stored in stringVal. The length should not 
        count a terminating NULL if there is one.

    CHAR    *stringVal,
        If the type is string:
            then this is the string value encoded in the character encoding indicated by
            stringIANAcharset and with the length of stringLengthInBytes.
            It is not necessary to end the string with a NULL character.
        else
            the stringVal must be a NULL pointer.

    BOOL    boolVal,
        If the type indicates boolean, the value is stored here.
        0 = FALSE
        1 = TRUE
*/
typedef struct WMLSvar
{
    UINT8   type;
    INT32   intVal;
    FLOAT32 floatVal;
    INT16   stringIANAcharset;
    UINT32  stringLengthInBytes;
    CHAR    *stringVal;
    BOOL    boolVal;
} WMLSvar;

/* Error Codes used for provisoning */

#define Warning_IncompleteAttributeRemoved		1
#define Warning_IllegalPlacedAttributeRemoved	2
#define Error_InDecoding						4
#define Error_wrongPINcode						8
#define Error_noMACincluded						16
#define Error_invalidSEC						32
#define Error_wrongListID						64
#define Error_unknown							128


/* Allowed values for parameter SEC */
#define NETWPIN		0
#define USERPIN		1
#define USERNETWPIN	2
#define USERPINMAC	3

/* ProvisioningDoc Attribute start tokens */
#define	ATTRST_version								(unsigned char) '\x45'
#define	ATTRST_version_1_0							(unsigned char) '\x46'

/* Characteristic Attribute start tokens */
#define PRVNG_type_LITERAL				(unsigned char) '\x50'
#define PRVNG_PXLOGICAL					(unsigned char) '\x51'
#define PRVNG_PXPHYSICAL				(unsigned char) '\x52'
#define PRVNG_PORT						(unsigned char) '\x53'
#define PRVNG_VALIDITY					(unsigned char) '\x54'
#define PRVNG_NAPDEF					(unsigned char) '\x55'
#define PRVNG_BOOTSTRAP					(unsigned char) '\x56'
#define PRVNG_VENDOR_CONFIG				(unsigned char) '\x57'
#define PRVNG_CLIENTIDENTITY			(unsigned char) '\x58'
#define PRVNG_PXAUTHINFO				(unsigned char) '\x59'
#define PRVNG_NAPAUTHINFO				(unsigned char) '\x5A'


/* Parm Attribute start tokens */
#define PRVNG_Parm_LITERAL							(unsigned char) '\x5'
#define PRVNG_Parm_VALUE							(unsigned char) '\x6'
#define PRVNG_Parm_NAME						(unsigned char) '\x7'
#define PRVNG_Parm_NAP_ADDRESS				(unsigned char) '\x8'
#define PRVNG_Parm_NAP_ADDRTYPE				(unsigned char) '\x9'
#define PRVNG_Parm_CALLTYPE					(unsigned char) '\xA'
#define PRVNG_Parm_VALIDUNTIL					(unsigned char) '\xB'
#define PRVNG_name_AUTHTYPE					(unsigned char) '\xC'
#define PRVNG_Parm_AUTHNAME					(unsigned char) '\xD'
#define PRVNG_Parm_AUTHSECRET					(unsigned char) '\xE'
#define PRVNG_Parm_LINGER						(unsigned char) '\xF'
#define PRVNG_Parm_BEARER						(unsigned char) '\x10'
#define PRVNG_Parm_NAPID						(unsigned char) '\x11'
#define PRVNG_Parm_COUNTRY					(unsigned char) '\x12'
#define PRVNG_Parm_NETWORK					(unsigned char) '\x13'
#define PRVNG_Parm_INTERNET					(unsigned char) '\x14'
#define PRVNG_Parm_PROXY_ID					(unsigned char) '\x15'
#define PRVNG_Parm_PROXY_PROVIDER_ID			(unsigned char) '\x16'
#define PRVNG_Parm_DOMAIN						(unsigned char) '\x17'
#define PRVNG_Parm_PROVURL					(unsigned char) '\x18'
#define PRVNG_Parm_PXAUTH_TYPE				(unsigned char) '\x19'
#define PRVNG_Parm_PXAUTH_ID					(unsigned char) '\x1A'
#define PRVNG_Parm_PXAUTH_PW					(unsigned char) '\x1B'
#define PRVNG_Parm_STARTPAGE					(unsigned char) '\x1C'
#define PRVNG_Parm_BASAUTH_ID					(unsigned char) '\x1D'
#define PRVNG_Parm_BASAUTH_PW					(unsigned char) '\x1E'
#define PRVNG_Parm_PUSHENABLED				(unsigned char) '\x1F'
#define PRVNG_Parm_PXADDR						(unsigned char) '\x20'
#define PRVNG_Parm_PXADDRTYPE					(unsigned char) '\x21'
#define PRVNG_Parm_TO_NAPID					(unsigned char) '\x22'
#define PRVNG_Parm_PORTNBR					(unsigned char) '\x23'
#define PRVNG_Parm_SERVICE					(unsigned char) '\x24'
#define PRVNG_Parm_LINKSPEED					(unsigned char) '\x25'
#define PRVNG_Parm_DNLINKSPEED				(unsigned char) '\x26'
#define PRVNG_Parm_LOCAL_ADDR					(unsigned char) '\x27'
#define PRVNG_Parm_LOCAL_ADDRTYPE				(unsigned char) '\x28'
#define PRVNG_Parm_CONTEXT_ALLOW				(unsigned char) '\x29'
#define PRVNG_Parm_TRUST						(unsigned char) '\x2A'
#define PRVNG_Parm_MASTER						(unsigned char) '\x2B'
#define PRVNG_Parm_SID						(unsigned char) '\x2C'
#define PRVNG_Parm_SOC						(unsigned char) '\x2D'
#define PRVNG_Parm_WSP_VERSION				(unsigned char) '\x2E'
#define PRVNG_Parm_PHYSICAL_PROXY_ID			(unsigned char) '\x2F'
#define PRVNG_Parm_CLIENT_ID					(unsigned char) '\x30'
#define PRVNG_Parm_DELIVERY_ERR_SDU			(unsigned char) '\x31'
#define PRVNG_Parm_DELIVERY_ORDER				(unsigned char) '\x32'
#define PRVNG_Parm_TRAFFIC_CLASS				(unsigned char) '\x33'
#define PRVNG_Parm_MAX_SDU_SIZE				(unsigned char) '\x34'
#define PRVNG_Parm_MAX_BITRATE_UPLINK			(unsigned char) '\x35'
#define PRVNG_Parm_MAX_BITRATE_DNLINK			(unsigned char) '\x36'
#define PRVNG_Parm_RESIDUAL_BER				(unsigned char) '\x37'
#define PRVNG_Parm_SDU_ERROR_RATIO			(unsigned char) '\x38'
#define PRVNG_Parm_TRAFFIC_HANDL_PRIO			(unsigned char) '\x39'
#define PRVNG_Parm_TRANSFER_DELAY				(unsigned char) '\x3A'
#define PRVNG_Parm_GUARANTEED_BITRATE_UPLINK	(unsigned char) '\x3B'
#define PRVNG_Parm_GUARANTEED_BITRATE_DNLINK	(unsigned char) '\x3C'


/* Attribute value tokens.		*/

/* ADDRTYPE Values */
#define	PRVNG_IPV4									(unsigned char) '\x85'
#define	PRVNG_IPV6									(unsigned char) '\x86'
#define	PRVNG_E164									(unsigned char) '\x87'
#define	PRVNG_ALPHA									(unsigned char) '\x88'
#define	PRVNG_APN									(unsigned char) '\x89'
#define	PRVNG_SCODE									(unsigned char) '\x8A'
#define	PRVNG_TETRA_ITSI							(unsigned char) '\x8B'
#define	PRVNG_MAN									(unsigned char) '\x8C'
/* CALLTYPE Values		*/
#define	PRVNG_ANALOG_MODEM							(unsigned char) '\x90'
#define	PRVNG_V_120									(unsigned char) '\x91'
#define	PRVNG_V_110									(unsigned char) '\x92'
#define	PRVNG_X_31									(unsigned char) '\x93'
#define	PRVNG_BIT_TRANSPARENT						(unsigned char) '\x94'
#define	PRVNG_DIRECT_ASCNCHRONOUS_DATA_SERVICE		(unsigned char) '\x95'
/* AUTHTYPE/PXAUTH-TYPE Values */
#define	PRVNG_PAP									(unsigned char) '\x9A'
#define	PRVNG_CHAP									(unsigned char) '\x9B'
#define	PRVNG_HTTP_BASIC							(unsigned char) '\x9C'
#define	PRVNG_HTTP_DIGEST							(unsigned char) '\x9D'
#define	PRVNG_WTLS_SS								(unsigned char) '\x9E'
/* BEARER Values	*/
#define	PRVNG_GSM_USSD								(unsigned char) '\xA2'
#define	PRVNG_GSM_SMS								(unsigned char) '\xA3'
#define	PRVNG_ANSI_136_GUTS							(unsigned char) '\xA4'
#define	PRVNG_IS_95_CDMA_SMS						(unsigned char) '\xA5'
#define	PRVNG_IS_95_CDMA_CSD						(unsigned char) '\xA6'
#define	PRVNG_IS_95_CDMA_PACKET						(unsigned char) '\xA7'
#define	PRVNG_ANSI_136_CSD							(unsigned char) '\xA8'
#define	PRVNG_ANSI_136_GPRS							(unsigned char) '\xA9'
#define	PRVNG_GSM_CSD								(unsigned char) '\xAA'
#define	PRVNG_GSM_GPRS								(unsigned char) '\xAB'
#define	PRVNG_AMPS_CDPD								(unsigned char) '\xAC'
#define	PRVNG_PDC_CSD								(unsigned char) '\xAD'
#define	PRVNG_PDC_PACKET							(unsigned char) '\xAE'
#define	PRVNG_IDEN_SMS								(unsigned char) '\xAF'
#define	PRVNG_IDEN_CSD								(unsigned char) '\xB0'
#define	PRVNG_IDEN_PACKET							(unsigned char) '\xB1'
#define	PRVNG_FLEX_REFLEX							(unsigned char) '\xB2'
#define	PRVNG_PHS_SMS								(unsigned char) '\xB3'
#define	PRVNG_PHS_CSD								(unsigned char) '\xB4'
#define	PRVNG_TETRA_SDS								(unsigned char) '\xB5'
#define	PRVNG_TETRA_PACKET							(unsigned char) '\xB6'
#define	PRVNG_ANSI_136_GHOST						(unsigned char) '\xB7'
#define	PRVNG_MOBITEX_MPAK							(unsigned char) '\xB8'
/* LINKSPEED Values */
#define	PRVNG_AUTOBAUDING							(unsigned char) '\xC5'
/* SERVICE Values */
#define	PRVNG_CL_WSP								(unsigned char) '\xCA'
#define	PRVNG_CO_WSP								(unsigned char) '\xCB'
#define	PRVNG_CL_SEC_WSP							(unsigned char) '\xCC'
#define	PRVNG_CO_SEC_WSP							(unsigned char) '\xCD'
#define	PRVNG_CL_SEC_WTA							(unsigned char) '\xCE'
#define	PRVNG_CO_SEC_WTA							(unsigned char) '\xCF'


#endif
