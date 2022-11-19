/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        D A T A   S E R V I C E S
            
		 S W A P   C O M M A N D   P R O C E S S O R

GENERAL DESCRIPTION

  This module implements the executing of SWAP commands

  Copyright (c) 2000 by SK Teletech Co., Ltd. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


// ---------- 표준함수 ---------------------------- 
#include	<string.h>		// C470 string functions
#include	<stdio.h>		// C470 string functions
#include	<ctype.h>       // for Character Functions
#include	<stdlib.h>
#include	<math.h>
#include <memory.h>

#include "customer.h"
#include "comdef.h"
// Added by SISI
#ifdef	FEATURE_GVM

#if(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
#else

#include "..\..\uiCommon.h" // for user defined types
#include "uiuint.h"
#include "uiutxt.h"
#include "uixscrn.h"
#include "uixsnd.h"
#include "uistate.h"

#endif

#include "dsrmio.h"
#include "clk.h"

#include "msg.h"
#include "err.h"
#include "snd.h"
#include "mar.h"
#include "mc.h"
#include "dsatcopi.h"
#include "diagi.h"

#include "sio.h"
#include "voc.h"
#include "mobile.h"
#include "bbver.h"
#include "ds.h"
#include "parm.h"

#include "diagpkt.h"			

/* rosa 01-03-06 --> @why, what */
#include "ska.h"
#include "..\SWAP\SWAP.h"
#include "..\SWAP\gvm_config.h"
typedef unsigned char     UB;
/* <-- */

#define	BCD_2_ASCII(n)			(((n) >> 4)*10 + ((n) & 0x0F))

#define UPPER(c) ((c) + 'A' - 'a') // to uppercase macro
#define ALPHA(c) (UPPER((c)) >= 'A' && (UPPER((c)) <= 'Z') // Is it a character?
#define DIGIT(c) ((c) >= '0' && (c) <= '9') // Is it a numeric character?

// External interface
extern	void	at_send_extended_text(char *cmd);
int		swap_GetString(UB **, UB);

// Parsing functions
UB	swap_getid_command(UB *src);
UB  swap_exec_command(UB *src);
UB	swap_event_command(UB *src);
UB	swap_uload_command(UB *src);

int		skt_GetStr(UB **, UB);
UINT8 ExistScript(const WORD cpid, const WORD gid);  // undersea added

// Parsing table
struct	t_sktjob	
{
	char	*ascii_com;					// ASCII Command text
	UB		(*ascii_job)(UB*);			// ASCII Parsing function
};


const	struct	t_sktjob	at_tb_sktable[] = {	// ASCII AT SKT Module Table
	"*GETID",		swap_getid_command,		
	"*EVENT",		swap_event_command,	
    "*EXEC",        swap_exec_command,
	"*ULOAD",		swap_uload_command,		

	NULL,			NULL
};


//static	UB			skt_BeepTurn = 0;
//static	UB			skt_BellFlag = 0;

UB	AtSWAPCommandBuf[256];
extern UB	asciiBuffer[128];

BYTE  *buffer = NULL;

int	swap_GetString(UB **command, UB remove_space_flag)
{
	int	len = 0;
	UB	*src = *command;

	asciiBuffer[len] = 0;
	while (len < 81 
		   && (*src != ',' || remove_space_flag == 0) 
		   && (*src != ds_atcop_s3_val) 
		   && (*src != 0x00)) {

		if (remove_space_flag == 1 && *src == ' ') {
			src++;
			continue;
		}

		asciiBuffer[len++] = *src++;
	}
	asciiBuffer[len] = '\0';
	*command = src;

	if (len > 80)
		len = -1;

	return len;
}

int	at_asc_2_bin(UB **command)
{
	UB	*src;
	int	no;

	src = *command;

	for	(no = 0 ; ; src++)
	{
		if	('0' <= *src && *src <= '9')
			no = (no * 10) + (*src - '0');
		else if (*src==';')
			;
		else	{
			if (('A' > *src || *src > 'Z') && *src!='+' && *src!=ds_atcop_s3_val && *src!=0x00)	
				no = -1;
			break;
		}
	}

	*command = src;

	if (no>65535)
		return (-1);
	else
		return(no);
}

////////////////////////////////////////////////////////////////////////////////
//	Variable initialize
void	swap_EsmsInitialize()
{
	extern	UB	DmMode;

	////////////////////////////////////////////////////////////////
	// Non-sleep for Data Manager Flash handling
	if (DmMode == 1) {
		DmMode = 0;	
	}	
}
 
////////////////////////////////////////////////////////////////////////////////
//	SWAP Command Main
//
UB	at_swap_command(UB **command)
{
	UB		*src = *command;
	int		i, len;
	char	capsBuf[10];

	memset((char*)capsBuf, 0, sizeof((char*)capsBuf)); // initializing

	for (i = 0; i < 10; i++)
	{
		if(ALPHA(src[i]))) // ?
			capsBuf[i] = UPPER(src[i]); // to uppercase
		else 
			capsBuf[i] = src[i];		
	}
		 
	for (i = 0; at_tb_sktable[i].ascii_com != NULL; i++) {
		len = strlen(at_tb_sktable[i].ascii_com);
		if (!strncmp((char*)capsBuf, at_tb_sktable[i].ascii_com, len)) {
			src += len;
			break;
		}
	}

	if (at_tb_sktable[i].ascii_com == NULL)
		return 4;

	return at_tb_sktable[i].ascii_job(src);

}


////////////////////////////////////////////////////////////////////////////////
//	Get a divided word
int	swap_GetStr(UB **command, UB remove_space_flag)
{
	int	len = 0;
	UB	*src = *command;

	asciiBuffer[len] = 0;
	while (len < 81 
		   && (*src != ',' || remove_space_flag == 0) 
		   && (*src != ds_atcop_s3_val) 
		   && (*src != 0x00)) {

		if (remove_space_flag == 1 && *src == ' ') {
			src++;
			continue;
		}

		asciiBuffer[len++] = *src++;
		asciiBuffer[len] = 0;
	}
	*command = src;

	if (len > 80)
		len = -1;

	return len;
}

// GVM2.04에서 규격 추가 - by undersea
UB swap_getid_command(UB *src)
{

	// defined in swap
//	extern UB swRunFlag;
    int cpid, id, len;

    switch (*src)	{

	case	'=':
		src++;
		if ((len = swap_GetString(&src, 1)) <= 0) 
			break;
		cpid = atoi((char*)asciiBuffer);
        src++;

        if ((len = swap_GetString(&src, 1)) <= 0) 
			break;
		id = atoi((char*)asciiBuffer);

        if (ExistScript((WORD)cpid, (WORD)id) != 0xFF) {
            sprintf((char*)asciiBuffer, "*SWAP*GETID:%d,%d", cpid, id);            
        }
        else {
            sprintf((char*)asciiBuffer, "*SWAP*GETID:0,0");            
        }
        at_send_extended_text((char*)asciiBuffer);
        break;

    case    '\0' :
        if (SKY_IS_IN_GVM()) {
            sprintf((char*)asciiBuffer, "*SWAP*GETID: %d, %d",
            (int)SKY_GET_GVM_GAME_CPID(g_nCurSSB), (int)SKY_GET_GVM_GAME_ID(g_nCurSSB) );
        }
        else {
            sprintf((char*)asciiBuffer, "*SWAP*GETID: 0, 0");
	    }
	    at_send_extended_text((char*)asciiBuffer);
        break;
    }

	return 0xFE;
}

UINT8 ExistScript(const WORD cpid, const WORD gid)
{

	UINT8   ssbIndex = 0xFF;
    UINT8   i=0;

    for (i=0; i<SWAP_SSB_SIZE; i++)
    {
		if (SKY_GET_GVM_GAME_CPID(i) == cpid &&
			SKY_GET_GVM_GAME_ID(i) == gid)
        {
            ssbIndex = i;
            break;
        }
    }
    return ssbIndex;
}

// GVM2.04에서 Exec Command 추가 - by undersea
UB swap_exec_command(UB *src)
{
    //char	buf[100 + 1];

	//UB  exec;
    int cpid, id;
	int  len;
	//int i = 0;

	switch (*src)	{

	case	'=':
		src++;
		if ((len = swap_GetString(&src, 1)) <= 0) 
			break;
		cpid = atoi((char*)asciiBuffer);
        src++;

        if ((len = swap_GetString(&src, 1)) <= 0) 
			break;
		id = atoi((char*)asciiBuffer);

        
        // 저장되어 있지 않거나 GVM 동작중에는 0, 0 출력*/
        if (ExistScript((WORD)cpid, (WORD)id) == 0xFF) {
            sprintf((char*)asciiBuffer, "*SWAP*EXEC:0,0");
        }
        else {
            if (SKY_IS_IN_GVM()) {
                sprintf((char*)asciiBuffer, "*SWAP*EXEC:0,0");
            }
            else {                
                sprintf((char*)asciiBuffer, "*SWAP*EXEC:%d,%d", cpid, id);                
                
                // 실행할 것~                
                g_nCurSSB = ExistScript((WORD)cpid, (WORD)id);
                SKY_SET_TO_GVM(TRUE);
                g_nRunningType = GRT_RUN_START;

                EQS_SEND_EVENT(SE_TONETGAME, EQS_NA, EQS_NA);
            }            
        }
        at_send_extended_text((char*)asciiBuffer);
	
		break;
	}

	return 0xFE;
}

UB swap_event_command(UB *src)
{

#define	TO_NUM(n)	(((n) >= 'A') ? 10 + (n) - 'A' : (n) - '0')

	char	buf[100 + 1];

	UB  event;
	int  len;
	int i = 0;

	switch (*src)	{

	case	'=':
		src++;
		if ((len = swap_GetString(&src, 1)) <= 0) 
			break;
		event = (UB)atoi((char*)asciiBuffer);

		if(/*event < 0 ||*/ event > 255)
			return 4;

		//if (swRunFlag == 0)
        if (SKY_IS_IN_GVM() == FALSE) 
			at_send_extended_text("*SWAP*EVENT: 0");
		else 
		{
			src++;
			if ((len = swap_GetString(&src, 1)) <= 0) 
				break;
			len = len/2;
			for( i = 0; i < len; i++)
			{
				buf[i] = ((TO_NUM(asciiBuffer[i*2]) << 4) & 0xf0) | (TO_NUM(asciiBuffer[i*2 + 1]) & 0x0f); 
			}
			buf[i] = '\0';
			if (len <= 0) 
				scl_RsEvent(event, 0, (byte *)NULL);
			else
				scl_RsEvent(event, len, (byte *)buf);

			at_send_extended_text("*SWAP*EVENT: 1");
		}
		break;
	}

	return 0xFE;
}

UB swap_uload_command(UB *src)
{

#define	TO_NUM(n)	(((n) >= 'A') ? 10 + (n) - 'A' : (n) - '0')

	char	buf[100 + 1];

	UB  size;
	int  len;
	int i = 0;

	switch (*src)	{

	case	'=':
		src++;
		if ((len = swap_GetString(&src, 1)) <= 0) 
			break;
		size = (UB)atoi((char*)asciiBuffer);

		if(size > 100)
			return 4;

		//if (swRunFlag == 0)
        if (SKY_IS_IN_GVM() == FALSE) 
			at_send_extended_text("*SWAP*ULOAD: 0");
		else 
		{
			src++;
			if ((len = swap_GetString(&src, 1)) <= 0) 
				break;
			len = len/2;
			for( i = 0; i < len; i++)
			{
				buf[i] = ((TO_NUM(asciiBuffer[i*2]) << 4) & 0xf0 ) | (TO_NUM(asciiBuffer[i*2 + 1]) & 0x0f);
			}
			buf[i] = '\0';
			if (len <= 0) 
				scl_RsUpload(size, (byte *)NULL);
			else
				scl_RsUpload(size, (byte *)buf);

			at_send_extended_text("*SWAP*ULOAD: 1");
		}
		break;
	}

	return 0xFE;
}
#endif	// FEATURE_GVM
