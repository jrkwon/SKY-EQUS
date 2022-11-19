/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
		MDARM_SYS.C
GENERAL DESCRIPTION
  This Software unit contains all phone information functions, file system
  and system time for XVM engine.This includes all REX Interfacing functions
  and XVM engine Interfacing functions.
  
EXTERNALIZED FUNCTIONS
	
INITIALIZATION AND SEQUENCING REQUIREMENTS

   Copyright (c) 2001 by SK Teletech Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
 
/******************************************************************/
/*	   include Header Files				                          */
/******************************************************************/

#include "extern.h" 
#include <string.h>
#include "sky.h"


/******************************************************************/
/*	   define constans ,macros and types	                      */
/******************************************************************/
// phone property 
typedef struct _Property {
	char *key;
	char *val;
}Property;

/******************************************************************/
/*	   grobal variables					                          */
/******************************************************************/
Property property_tbl[ ] ={
	{"m.CARRIER","SKT"},  // ���� ����� : SKT
	{"m.TYPE","B"},       // �ܸ��� Ÿ�� phone:1 pda:2 smart phone:3 RFU:4-9
	{"m.VENDER","SK"},     // �������� - SK:0X04
	{"m.MODEL","11"},	  // �ܸ��� �� 
					/* IM 3100 �������� 0X10���� ���� IM 3200 �̻��� ��� �̰��� �����Ұ�.. by tortol*/
	{"m.CONF_PROFILE","4230"},  // SK-VM 1.x �԰� 0x3C  
	{"m.SK_VM", "11"},       // SK-VM version   
    {"m.EXT_SW", "000"},     // Ȯ�� �ΰ� S/W
	{"m.LCD_PIXEL","128144"}, // lcd pixel�� B1: 128*144
	                          // lcd�� pixel�� �� Ŀ���� �����Ұ�.. 
	{"m.LCD_FONT","1809"},    // font�� ũ�� �켱 �⺻ ��Ʈ�� ����  ���ڸ���
	                           // �⺻ ��Ʈ 14*15 -> 128/7 = 18  144/15 = 9.6 ��) 18�� 9�� -> 1809
	{"m.SOUND_POLY","230"},	  /* B1�� ��� H/W solution���� MA2�� 
							  //  230	                          //   --
							  //  2: MA2,   3: EVRC
							  */   
	{"m.COLOR","7"},		  // 65536 color -> B2 // �ӵ� ������ ���� 256 color����� ������ ���� 
	{NULL,NULL}
};
/******************************************************************/
/*	   static variables					                          */
/******************************************************************/

/******************************************************************/
/*	   extern variables					                          */
/******************************************************************/

/******************************************************************/
/*	   extern functions					                          */
/******************************************************************/

/******************************************************************/
/*     function Declare											  */
/******************************************************************/

/******************************************************************/
/*     function Define											  */
/******************************************************************/

/*************************************************************
*NAME
*md_get_property
*DESCRIPTION
*val is charactor buffer with valMaxLen length.
*fill val according to key,return val.
*
*RETURN VALUE
*On success return val,On failure return NULL.
****************************************************************/
char *md_get_property(char *key,char *val,int valMaxLen)
{

	Property *p;
	nv_dir_number_type  nv_dir_num; /* current nam name */

    CHAR szBuf[21];
    db_items_value_type dbi;
	if(!strcmp(key,"m.MIN")){
		// ��ȭ��ȣ ����
		nv_dir_num.nam = SKY_GET_CURRENT_NAM();
		(void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );
        if (nv_dir_num.dir_number[0] == '0') // 10�ڸ� ��ȣ�� ���
		{
			val[0] = '0';
            STRNCPY((BYTE *)val+1, (BYTE *)&(nv_dir_num.dir_number[3]), 7);
		    val[8] = '\0';
		}
		else // 11�ڸ� ��ȣ�� ��� (11.....�̷��� ����)
		{
			STRNCPY((BYTE *)val, (BYTE *)&(nv_dir_num.dir_number[2]), 8);
			val[8] = '\0';
		}
		return val;
	}
	
	////////////////////////////////////////////
	else if (!strcmp(key,"lcdui.tchandler")){
		strncpy(val,"com.sktt.lcdui.TCHandler",valMaxLen);

		val[ valMaxLen-1] ='\0';
		return val;
	}
	else if (!strcmp(key,"m.SID")){
		/*���� ���� �������� �Ѵ�.*/
		db_get(DB_SID,&dbi);
		sprintf(val, "%d", dbi.sid);
		val[ valMaxLen-1] ='\0';
		return val;
	}
	else if (!strcmp(key,"m.NID")){
		/*���� ���� �������� �Ѵ�.*/
		db_get(DB_NID,&dbi);
		sprintf(val, "%d", dbi.nid);
    	val[ valMaxLen-1] ='\0';
		return val;
	}
	else if (!strcmp(key,"m.BASE_ID")){
		/*���� ���� �������� �Ѵ�.*/
		db_get(DB_BASE_ID,&dbi);
		sprintf(val, "%d", dbi.reg_zone);
    	val[ valMaxLen-1] ='\0';
		return val;
	}
	else if (!strcmp(key,"m.REG_ZONE")){
		/*���� ���� �������� �Ѵ�.*/
		db_get(DB_REG_ZONE,&dbi);
		sprintf(val,"%d", dbi.reg_zone);
    	val[ valMaxLen-1] ='\0';
		return val;
	}
	else if (!strcmp(key,"m.PILOT_PN")){
		/*���� ���� �������� �Ѵ�.*/
		db_get( DB_PILOT_PN_OFF, &dbi ); //Show PN Offset
		sprintf(val,"%d", dbi.pilot_pn_off);
		val[ valMaxLen-1] ='\0';
		return val;
	}
	for (p =property_tbl;p->key;p++){
		if (!strcmp(key,p->key)){
			strncpy(val,p->val,valMaxLen);
			val[ valMaxLen-1] ='\0';
			return val;
		}
	}
	return NULL;
}

