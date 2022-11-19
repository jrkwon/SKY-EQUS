/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     clntPIM.c
**
** Description: 
**     Phonebook library Function 
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-02-19 ytchoi     Created.
**
** ===========================================================================
*/
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sky.h"
#include "ska.h"
#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif
#include "clnt.h"
#include "clntapp.h"

int  Atoi(char* pszStr)
{
    int nValue, i;

    nValue = 0;
    i = 0;
    while( pszStr[i] != 0 )
    {
        nValue = nValue* 10 + (pszStr[i++] - '0');
    }

    return nValue;
}


/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/*EQS_API*/
extern BOOL IsMiddleOfMultiByte(BYTE *pszSrc, UINT16 nPosition);
extern byte get_length( byte *chk_str, byte buff_length);


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void comma2space(char *target);
LOCAL void plus2space(char *target);
LOCAL void dollar2space(char *target);
LOCAL int str_cmp16(byte *src, byte *target);


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#ifdef FEATURE_PHONEBOOK_VER12
#define PIMSYNC_VERSION_ID "1.2" //with selective upload service
#else
#define PIMSYNC_VERSION_ID "1.1" //with upload, download, selective download service	
#endif //#ifdef FEATURE_PHONEBOOK_VER12


NV_DIAL_TYPE wml_dial;
NV_DIAL_TYPE tmp_dial;

#define HAND_IDX   0
#define HOME_IDX   1
#define OFFICE_IDX 2
#define PAGER_IDX  3

#define VER2 1
#ifdef VER2
void isModifyAll(int Start, int End, char *result)
{
	int i=0;
	byte first_flag = 0;
	char tmp_buf[6];
	pb_book_info_type BookInfo;

	*result = '\0';
	if( Start > End || Start < 1 || Start > MAX_BOOK_NUMBER ||
		End < 1 || End > MAX_BOOK_NUMBER ) {
		return;	// woodstok 00.4.28  void function
	}

//B1에서 wml_dial --> BookInfo change
	for( i=Start; i<=End ; i++ ) { /* read all the entries */
#ifdef PORTING_B0_WAP
	    if( ui.isdial[i] ) {
		     wml_dial.address = i;            
			(void)wap_process_nv_item( NV_DIAL_I, ( nv_item_type * )&wml_dial, TRUE);
#else //PORTING_B0_WAP
		if( IS_VALID_BOOK(i) ) {
			BookInfo.address = i;
			ui_get_pb(PB_BOOK_INFO_I, &BookInfo);
#endif //PORTING_B0_WAP
			if( BookInfo.sync_flag == 'C' || BookInfo.sync_flag == 'M' ) {
				if( !first_flag ) {
					sprintf(tmp_buf,"%d",i);
					first_flag = 1;
				} else {
					sprintf(tmp_buf,",%d",i);
				}
				strcat(result,tmp_buf);
			} else if ( BookInfo.sync_flag != 'C' && BookInfo.sync_flag != 'M' && BookInfo.sync_flag != 'S') {
				BookInfo.sync_flag = 'C';
#ifdef PORTING_B0_WAP
				(void)wap_process_nv_item( NV_DIAL_I, ( nv_item_type * )&wml_dial, FALSE);
#else //PORTING_B0_WAP
				ui_put_pb( PB_BOOK_INFO_I, &BookInfo );
#endif //PORTING_B0_WAP
				if( !first_flag ) {
					sprintf(tmp_buf,"%d",i);
					first_flag = 1;
				} else {
					sprintf(tmp_buf,",%d",i);
				}
				strcat(result,tmp_buf);
			}
		}
	}
}
#endif //VER2


// nv에서 읽지 않고 전역번수를 이용한다.
void isExist(char *name, char *result)
{
		int  index_list[MAX_BOOK_NUMBER+1];
	    char tmp_buf[10];
		int  i=0,j=0,k=0,l=0;
		byte korcho_match;
		word dial_word;
		word edbuf_word;
	    *result = '\0';

        for( i=1; i<=MAX_BOOK_NUMBER; i++ ) { /* read all the entries */
          if( (i&7) == 0 ) {
            dog_report( DOG_UI_RPT );
          }
#ifdef PORTING_B0_WAP
          if( ui.isdial[i] ) {
            wml_dial.address = i;
			(void)wap_process_nv_item( NV_DIAL_I, ( nv_item_type * )&wml_dial, TRUE);
#else //PORTING_B0_WAP
		  if( IS_VALID_BOOK(i) ) {
#endif //PORTING_B0_WAP
				edbuf_word = ((word)name[0] << 8) + name[1];
				korcho_match = 0;
				if ((edbuf_word >= 0xa4a1) && (edbuf_word <= 0xa4be)) {
					dial_word = ((word)g_szPersonName[i-1][0] << 8) + g_szPersonName[i-1][1];
					switch (edbuf_word)
					{
						case 0xa4a1: /* kiyeuk */
							if ((dial_word >= 0xb0a1) && (dial_word <= 0xb1ed))
								korcho_match = 1;
							break;
						case 0xa4a2: /* ssangkiyeuk */
							if ((dial_word >= 0xb1ee) && (dial_word <= 0xb3a9))
								korcho_match = 1;
							break;
						case 0xa4a4: /* neiun */
							if ((dial_word >= 0xb3aa) && (dial_word <= 0xb4d8))
								korcho_match = 1;
							break;
						case 0xa4a7: /* deigeut */
							if ((dial_word >= 0xb4d9) && (dial_word <= 0xb5fa))
								korcho_match = 1;
							break;
						case 0xa4a8: /* ssangdeigeut */
							if ((dial_word >= 0xb5fb) && (dial_word <= 0xb6f2))
								korcho_match = 1;
							break;
						case 0xa4a9: /* leil */
							if ((dial_word >= 0xb6f3) && (dial_word <= 0xb8b5))
								korcho_match = 1;
							break;
						case 0xa4b1: /* meum */
							if ((dial_word >= 0xb8b6) && (dial_word <= 0xb9d8))
								korcho_match = 1;
							break;
						case 0xa4b2: /* beup */
							if ((dial_word >= 0xb9d9) && (dial_word <= 0xbafb))
								korcho_match = 1;
							break;
						case 0xa4b3: /* ssangbeup */
							if ((dial_word >= 0xbafc) && (dial_word <= 0xbbe6))
								korcho_match = 1;
							break;
						case 0xa4b5: /* siot */
							if ((dial_word >= 0xbbe7) && (dial_word <= 0xbdcd))
								korcho_match = 1;
							break;
						case 0xa4b6: /* ssangsiot */
							if ((dial_word >= 0xbdce) && (dial_word <= 0xbec5))
								korcho_match = 1;
							break;
						case 0xa4b7: /* eung */
							if ((dial_word >= 0xbec6) && (dial_word <= 0xc0d9))
								korcho_match = 1;
							break;
						case 0xa4b8: /* zeut */
							if ((dial_word >= 0xc0da) && (dial_word <= 0xc2a4))
								korcho_match = 1;
							break;
						case 0xa4b9: /* ssangzeut */
							if ((dial_word >= 0xc2a5) && (dial_word <= 0xc2f6))
								korcho_match = 1;
							break;
						case 0xa4ba: /* chiut */
							if ((dial_word >= 0xc2f7) && (dial_word <= 0xc4aa))
								korcho_match = 1;
							break;
						case 0xa4bb: /* kiuk */
							if ((dial_word >= 0xc4ab) && (dial_word <= 0xc5b7))
								korcho_match = 1;
							break;
						case 0xa4bc: /* tegeuk */
							if ((dial_word >= 0xc5b8) && (dial_word <= 0xc6c3))
								korcho_match = 1;
							break;
						case 0xa4bd: /* peup */
							if ((dial_word >= 0xc6c4) && (dial_word <= 0xc7ce))
								korcho_match = 1;
							break;
						case 0xa4be: /* heut */
							if ((dial_word >= 0xc7cf) && (dial_word <= 0xc8fe))
								korcho_match = 1;
							break;
						default:
							korcho_match = 0;
							break;
					}
				}
				if(korcho_match) {
					index_list[l++] = i;   /* save index and count match */
				}else {					 
				for( j=0; j<A7_NV_MAX_LTRS; j++ ) {//A7_NV_MAX_LTRS =16
				  for( k=0; name[k]!='\0'; k++ ) {
					  if( (g_szPersonName[i-1][j+k]) != name[k] ) { /* mismatch */
						k = 99;
						break;
					  }
				  }
				  if( k != 99 ) {     /* a match! */
					index_list[l++] = i;   /* save index and count match */
					break;
				  }
				  if(g_szPersonName[i-1][j] & 0x80) j++;
				}
				}
          }                       /* if ui.isdial */
        }
		
		for(j=0; j < l; j++) {
			if( j == 0 ) {
				sprintf(tmp_buf, "%d",index_list[j]);
			} else {
				sprintf(tmp_buf, ",%d",index_list[j]);
			}
			strcat(result,tmp_buf);		
		}

		
}

void isGroup(char *groupname, char *result)
{
        byte index_list[MAX_BOOK_NUMBER+1];
	    char tmp_buf[10];
		int  i=0,j=0,k=0,l=0;
		byte korcho_match;
		word dial_word;
		word edbuf_word;
	    *result = '\0';

        for( i=0; i<MAX_GROUP_NUMBER; i++ ) { /*NV_MAX_GROUP = 20 /MAX_GROUP_NUMBER =40 all numbers including emergency */
#ifdef PORTING_B0_WAP
          if( ui.isgroup[i] ) {
#else //PORTING_B0_WAP
		  if( IS_VALID_GROUP(i) ) {	  
#endif //PORTING_B0_WAP
				edbuf_word = ((word)groupname[0] << 8) + groupname[1];
				korcho_match = 0;
				if ((edbuf_word >= 0xa4a1) && (edbuf_word <= 0xa4be)) {
					dial_word = ((word)g_szGroupName[i-1][0] << 8) + g_szGroupName[i-1][1];
					switch (edbuf_word)
					{
						case 0xa4a1: /* kiyeuk */
							if ((dial_word >= 0xb0a1) && (dial_word <= 0xb1ed))
								korcho_match = 1;
							break;
						case 0xa4a2: /* ssangkiyeuk */
							if ((dial_word >= 0xb1ee) && (dial_word <= 0xb3a9))
								korcho_match = 1;
							break;
						case 0xa4a4: /* neiun */
							if ((dial_word >= 0xb3aa) && (dial_word <= 0xb4d8))
								korcho_match = 1;
							break;
						case 0xa4a7: /* deigeut */
							if ((dial_word >= 0xb4d9) && (dial_word <= 0xb5fa))
								korcho_match = 1;
							break;
						case 0xa4a8: /* ssangdeigeut */
							if ((dial_word >= 0xb5fb) && (dial_word <= 0xb6f2))
								korcho_match = 1;
							break;
						case 0xa4a9: /* leil */
							if ((dial_word >= 0xb6f3) && (dial_word <= 0xb8b5))
								korcho_match = 1;
							break;
						case 0xa4b1: /* meum */
							if ((dial_word >= 0xb8b6) && (dial_word <= 0xb9d8))
								korcho_match = 1;
							break;
						case 0xa4b2: /* beup */
							if ((dial_word >= 0xb9d9) && (dial_word <= 0xbafb))
								korcho_match = 1;
							break;
						case 0xa4b3: /* ssangbeup */
							if ((dial_word >= 0xbafc) && (dial_word <= 0xbbe6))
								korcho_match = 1;
							break;
						case 0xa4b5: /* siot */
							if ((dial_word >= 0xbbe7) && (dial_word <= 0xbdcd))
								korcho_match = 1;
							break;
						case 0xa4b6: /* ssangsiot */
							if ((dial_word >= 0xbdce) && (dial_word <= 0xbec5))
								korcho_match = 1;
							break;
						case 0xa4b7: /* eung */
							if ((dial_word >= 0xbec6) && (dial_word <= 0xc0d9))
								korcho_match = 1;
							break;
						case 0xa4b8: /* zeut */
							if ((dial_word >= 0xc0da) && (dial_word <= 0xc2a4))
								korcho_match = 1;
							break;
						case 0xa4b9: /* ssangzeut */
							if ((dial_word >= 0xc2a5) && (dial_word <= 0xc2f6))
								korcho_match = 1;
							break;
						case 0xa4ba: /* chiut */
							if ((dial_word >= 0xc2f7) && (dial_word <= 0xc4aa))
								korcho_match = 1;
							break;
						case 0xa4bb: /* kiuk */
							if ((dial_word >= 0xc4ab) && (dial_word <= 0xc5b7))
								korcho_match = 1;
							break;
						case 0xa4bc: /* tegeuk */
							if ((dial_word >= 0xc5b8) && (dial_word <= 0xc6c3))
								korcho_match = 1;
							break;
						case 0xa4bd: /* peup */
							if ((dial_word >= 0xc6c4) && (dial_word <= 0xc7ce))
								korcho_match = 1;
							break;
						case 0xa4be: /* heut */
							if ((dial_word >= 0xc7cf) && (dial_word <= 0xc8fe))
								korcho_match = 1;
							break;
						default:
							korcho_match = 0;
							break;
					}
				}
				if(korcho_match) {
					index_list[l++] = i;   /* save index and count match */
				}else {
				for( j=0; j<A7_NV_MAX_LTRS; j++ ) {
				  for( k=0; groupname[k]!='\0'; k++ ) {
					  if( (g_szGroupName[i-1][j+k]) != groupname[k] ) { /* mismatch */
						k = 99;
						break;
					  }
				  }
				  if( k != 99 ) {     /* a match! */
					index_list[l++] = i;   /* save index and count match */
					break;
				  }
				  if(g_szGroupName[i-1][j]& 0x80) j++;
				}
				}
          }                       /* if ui.isdial */
        }

		for(j=0; j < l; j++) {
			if( j == 0 ) {
				sprintf(tmp_buf, "%d",index_list[j] + 1);
			} else {
				sprintf(tmp_buf, ",%d",index_list[j] + 1);
			}
			strcat(result,tmp_buf);		
		}
}

void getMaxEntry(char *result)
{
	int i;
// 임시로 B1과 같이 200개로 함!!ytchoi 2001/09/25
//#ifdef FEATURE_PIM_INVALIDERROR
	i = 200;
//#else
    //i = MAX_BOOK_NUMBER;
//#endif //FEATURE_PIM_INVALIDERROR
	*result = '\0';
	sprintf(result,"%d",i);
		
}

void getMaxGroup(char *result)
{
	int i;
	i = MAX_GROUP_NUMBER; // NV_MAX_GROUP = 20 --> MAX_GROUP_NUMBER =40
	*result = '\0';
	sprintf(result,"%d",i);
}

void getMaxShortKey(char *result)
{
	*result = '\0';
}

void getPhonebook(char *result)
{
	int i=0;
	int no_book=0;

	for( i=1; i<=MAX_BOOK_NUMBER; i++ ) { /* read all the entries */
#ifdef PORTING_B0_WAP
		if( ui.isdial[i] ) {
#else //PORTING_B0_WAP
		if( IS_VALID_BOOK(i) ) {
#endif //PORTING_B0_WAP
			no_book++;
		}
	}
	*result = '\0';
	sprintf(result,"%d",no_book);
    CLNTa_log(1,0,"getPhonebook num:%d", no_book);
}

#ifdef VER2
void getDeletedEntry(int Start, int End, char *result)
{
	int i=0;
	byte first = 0;
	char tmp_buf[10];
	*result = '\0';

	if( Start > End || Start < 1 || Start > MAX_BOOK_NUMBER ||
		End < 1 || End > MAX_BOOK_NUMBER ) {
		return;	// woodstok 00.4.28  void function
	}
	for( i=Start; i<=End; i++ ) { /* read all the entries */
#ifdef PORTING_B0_WAP
		if( !ui.isdial[i] ) {
#else //PORTING_B0_WAP
		if( !IS_VALID_BOOK(i) ) {
#endif //PORTING_B0_WAP
			if( first == 0 ) {
				first = 1;
				sprintf(tmp_buf, "%d",i);
			} else {
				sprintf(tmp_buf, ",%d",i);
			}
			strcat(result,tmp_buf);			
		}
	}

}
#endif//VER2

void getGroup(char *result)
{
	int i=0;
	byte m=0;
	byte first = 0;
	char group_name[A7_NV_MAX_LTRS +1];
	char tmp_buf[30];
	pb_group_info_type  GroupInfo;
	*result = '\0';
// nv_group_type group대신에 GroupInfo이용 
	
	for( i=0; i<MAX_GROUP_NUMBER; i++ ) { /* all numbers including emergency */
#ifdef PORTING_B0_WAP
          if( ui.isgroup[i] ) {
#else //PORTING_B0_WAP
          if( IS_VALID_GROUP(i) ) {
#endif //PORTING_B0_WAP			
				GroupInfo.address = i;
				//(void)wap_process_nv_item( NV_GROUP_I, ( nv_item_type * )&group, TRUE);
				ui_get_pb( PB_GROUP_INFO_I, &GroupInfo);
					
				m = get_length((byte *)GroupInfo.group_name, A7_NV_MAX_LTRS );
				memcpy(group_name, (void *)GroupInfo.group_name, m);
				group_name[m] = '\0';

				comma2space(group_name);

				if(!first) {
					first = 1;
					if( GroupInfo.tstamp == 0 ) {
						sprintf(tmp_buf, "%d,%s,-2147483648",i+1,group_name);
					} else {
						sprintf(tmp_buf, "%d,%s,%ld",i+1,group_name,GroupInfo.tstamp);
					}
				} else {
					if( GroupInfo.tstamp == 0 ) {
						sprintf(tmp_buf, ",%d,%s,-2147483648",i+1,group_name);
					} else {
						sprintf(tmp_buf, ",%d,%s,%ld",i+1,group_name,GroupInfo.tstamp);
					}
				}
				strcat(result,tmp_buf);	
		  }
	}
}

int setGroup(int GroupID, char *groupname)
{
// nv_group_type group대신에 GroupInfo이용 	
	pb_group_info_type GroupInfo;
	int str_leng=0;
	int i=0;
	char tmpname[20];
	char tmp_buf[30];

	str_leng = strlen(groupname);
	strcpy(tmp_buf, groupname);
	if( str_leng > A7_NV_MAX_LTRS) {
		if(IsMiddleOfMultiByte((byte *)tmp_buf,A7_NV_MAX_LTRS)) { //IsMiddleofHan() --> IsMiddleOfMultiByte()
				memcpy(groupname, tmp_buf, A7_NV_MAX_LTRS-1);
				groupname[A7_NV_MAX_LTRS-1] = NULL;
			} else {
				memcpy(groupname, tmp_buf, A7_NV_MAX_LTRS);
				groupname[A7_NV_MAX_LTRS] = NULL;
			}
	}

	str_leng = strlen(groupname);
	if( GroupID < 1 || GroupID > MAX_GROUP_NUMBER || str_leng <= 0 || str_leng > A7_NV_MAX_LTRS ) {
		return((int)INVALID_PARAMETER); /* Invalid_Parameter */
	}
// SAME group name check
	memset(tmpname,' ', 20);
	memcpy( tmpname, groupname, str_leng);
	for(i=1; i<= MAX_GROUP_NUMBER; i++) {
#ifdef PORTING_B0_WAP
		if(!ui.isgroup[i]) {
#else //PORTING_B0_WAP
		if(!IS_VALID_GROUP(i)) {
#endif //PORTING_B0_WAP
			continue;
		} else {
			if(str_cmp16((byte *)tmpname, g_szGroupName[i-1])) /* same(1) : diff(0) */
			break;
		}
	}
	if( ((i != GroupID)&& i> MAX_GROUP_NUMBER) || str_cmp16((byte *)tmpname, (byte *)"지정안함        ")|| str_cmp16((byte *)tmpname, (byte *)"지정안됨        ")) {
		return(SAME_ENTRY); /* Invalid_Parameter : same name */	
	}	
// SAME group name check end
#ifdef PORTING_B0_WAP
    group.bell.Option = 0;
	group.exist = 1;
	memset((void *)group.letters, ' ', A7_NV_MAX_LTRS);
	group.address = GroupID -1;	
	memcpy((void *)group.letters, groupname, str_leng);
	group.status = NON_SECRET_SD_NUM;
	(void)wap_process_nv_item( NV_GROUP_I, ( nv_item_type * )&group, FALSE);
	DmRefreshGroup(group.address, &group); // later !!1
#else //PORTING_B0_WAP 
	if( g_szGroupName[0] != BOOK_NOT_USE )
	{
		GroupInfo.address = GroupID;
		ui_get_pb(PB_GROUP_INFO_I, &GroupInfo);    
		GroupInfo.Bell.wBellID        = 0;
		GroupInfo.Bell.szFileName[0]  = 0;    
		STRCPY((BYTE*)GroupInfo.group_name,(BYTE*)groupname);
		GroupInfo.status = NON_SECRET_GROUP;
		ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
	}
	else
	{
		GroupInfo.address = GroupID;
		init_pb_node( PB_GROUP_INFO_I, &GroupInfo);
		GroupInfo.next_index = active_list.group_info;
		active_list.group_info = GroupInfo.address;
		GroupInfo.Bell.wBellID        = 0;
		GroupInfo.Bell.szFileName[0]  = 0;    
		STRCPY((BYTE*)GroupInfo.group_name,(BYTE*)groupname);
		GroupInfo.status = NON_SECRET_GROUP;
		ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
	}		
	PhoneBookUpdate();
#endif //PORTING_B0_WAP

/////////////////////
//Don't forget !!!
//DmRefreshGroup(int index, nv_group_type* pgroup) in uinsstrt.c 
//must be called
/////////////////////
	return(SUCCESS);
}

//RedStar 검토해주세요!!!!
void getEntry(int EntryID, char *result)
{
	char tmp_buf[70];
	char tmp_mem[70];

	byte length=0;  
	byte  tmp_value = 0;
	char *anv[4] = {"", "생일", "결혼", "첫만남"};

	*result = '\0';

    if( EntryID < 1 || EntryID > MAX_BOOK_NUMBER ) {
		return; 
	}
	wml_dial.address = EntryID;
#ifdef PORTING_B0_WAP
	if( !ui.isdial[EntryID] ) return ;
	(void)wap_process_nv_item( NV_DIAL_I, ( nv_item_type * )&wml_dial, TRUE);
#else //PORTING_B0_WAP
	if( !IS_VALID_BOOK(EntryID) ) return ;
	MakeOldBook(EntryID, (NV_DIAL_TYPE *)&wml_dial);
#endif //PORTING_B0_WAP
//////////////////////////////////////////////
// Group ID
	if( wml_dial.group == 100 ) {
		tmp_value = 0; 
	} else {
		tmp_value = wml_dial.group + 1;
	}
	sprintf(tmp_buf, "%d,",tmp_value);
	strcat(result, tmp_buf);

// Name
	length = get_length((byte *)wml_dial.letters, A7_NV_MAX_LTRS );
	if( length ) {
		memcpy(tmp_buf, (void *)wml_dial.letters, length);
		tmp_buf[length] = ',';
		tmp_buf[length + 1] = '\0';

		comma2space(tmp_buf);
	    plus2space(tmp_buf);
		dollar2space(tmp_buf);

		strcat(result, tmp_buf);
	} else {
		strcat(result, ",");
	}
// Home : 집 전화번호
	length = wml_dial.multi_num_digits[HOME_IDX];
	if( length ) {
		memcpy(tmp_buf, (void *)wml_dial.multi_digits[HOME_IDX], length);
		tmp_buf[length] = ',';
		tmp_buf[length + 1] = '\0';
		strcat(result, tmp_buf);
	} else {
		strcat(result, ",");
	}

// SK_HOME Short key
	strcat(result, ",");

// ADD_HOME 집 주소 
	strcat(result, ",");	

// Mobile : 휴대폰 전화번호 
	length = wml_dial.multi_num_digits[HAND_IDX];
	if( length ) {
		memcpy(tmp_buf, (void *)wml_dial.multi_digits[HAND_IDX], length);
		tmp_buf[length] = ',';
		tmp_buf[length + 1] = '\0';
		strcat(result, tmp_buf);
	} else {
		strcat(result, ",");
	}   

// SK_MOBILE : Short key
	strcat(result, ",");

// Pager : 호출기 전화번호
	length = wml_dial.multi_num_digits[PAGER_IDX];
	if( length ) {
		memcpy(tmp_buf, (void *)wml_dial.multi_digits[PAGER_IDX], length);
		tmp_buf[length] = ',';
		tmp_buf[length + 1] = '\0';
		strcat(result, tmp_buf);
	} else {
		strcat(result, ",");
	}   

// SK_PAGER : Short key
	strcat(result, ",");

// School : 직장/학교 전화번호
	length = wml_dial.multi_num_digits[OFFICE_IDX];
	if( length ) {
		memcpy(tmp_buf, (void *)wml_dial.multi_digits[OFFICE_IDX], length);
		tmp_buf[length] = ',';
		tmp_buf[length + 1] = '\0';
		strcat(result, tmp_buf);
	} else {
		strcat(result, ",");
	}
	
// SK_SCHOOL : Short key
	strcat(result, ",");

// ADD_SCHOOL : 직장/학교 주소
	strcat(result, ",");

// Fax
	strcat(result, ",");

// E-mail 
	length = get_length((byte *) wml_dial.email, A7_NV_MAX_DIAL_DIGITS); //A7_NV_MAX_DIAL_DIGITS 32
	if( length ) {
		memcpy(tmp_buf, (void *)wml_dial.email, length);
		tmp_buf[length] = ',';
		tmp_buf[length + 1] = '\0';

		comma2space(tmp_buf);
	    plus2space(tmp_buf);
		dollar2space(tmp_buf);

		strcat(result, tmp_buf);
	} else {
		strcat(result, ",");
	}
// Entension field : 메모(1) + 기념일(2)
	strcat(result,"3");

// Memo
	strcat(result,"+1,"); // 메모 파라메터 갯수
	length = get_length((byte *) wml_dial.memo, A7_NV_MAX_MEMO_SIZ); //A7_NV_MAX_MEMO_SIZ 64
	if( length ) {
		if( length > 32 ) {
			if(IsMiddleOfMultiByte((byte *)tmp_buf,32)) {//IsMiddleofHan() --> IsMiddleOfMultiByte()
				memcpy(tmp_buf, (void *)wml_dial.memo, 32 - 1);
				tmp_buf[32 - 1] = '\0';
			} else {
				memcpy(tmp_buf, (void *)wml_dial.memo, 32);
				tmp_buf[32] = '\0';
			}
		} else {
			memcpy(tmp_buf, (void *)wml_dial.memo, length);
			tmp_buf[length] = '\0';
		}

		comma2space(tmp_buf);
	    plus2space(tmp_buf);
		dollar2space(tmp_buf);
		strcat(result, tmp_buf);
	}

// 기념일 
	strcat(result,"+4,"); // 기념일 파라메터 갯수
	if( wml_dial.anniv_contidx == 0) { // 기념일 없음
		strcat(result,",,,");
	} else if ( wml_dial.anniv_contidx == 4) {// 기념일 내용 편집
		tmp_value = (wml_dial.anniv_lunar + 1) % 2;	
		length = get_length( (byte *)wml_dial.anniv_content, A7_NV_MAX_LTRS);
		memcpy(tmp_mem, (void *)wml_dial.anniv_content, length);
		tmp_mem[length] = '\0';

		comma2space(tmp_mem);
	    plus2space(tmp_mem);
		dollar2space(tmp_mem);

		sprintf(tmp_buf,"%d,%d,%d,%s",tmp_value, wml_dial.anniv_month, wml_dial.anniv_day, tmp_mem);
		strcat(result,tmp_buf);
	} else {
		tmp_value = (wml_dial.anniv_lunar + 1) % 2;		
		sprintf(tmp_buf,"%d,%d,%d,%s",tmp_value, wml_dial.anniv_month, wml_dial.anniv_day, anv[wml_dial.anniv_contidx]);
		strcat(result,tmp_buf);
	}

// Time stamp
		if( wml_dial.tstamp == 0 ) {
			strcat( result, ",-2147483648");
		} else {
			sprintf(tmp_buf,",%ld",wml_dial.tstamp);
			strcat( result, tmp_buf);
		}
//////////////////////////////////////	
}

int deleteEntry(int EntryID )
{

	if( EntryID < 1 || EntryID > MAX_BOOK_NUMBER ) {
		return((int)INVALID_PARAMETER); /* Invalid_Phonebook_Parameter */
	}
#ifdef PORTING_B0_WAP
	wml_dial.address = EntryID;
	wml_dial.exist = 0;
	(void)wap_process_nv_item( NV_DIAL_I, ( nv_item_type * )&wml_dial, FALSE);
	ui.isdial[wml_dial.address] = 0;
#else
    DeleteBookInfo(EntryID); //RAM과 ROM처리!! 
#endif //PORTING_B0_WAP
/////////////////////
//Don't forget !!!
//DmRefreshDial(int index, nv_group_type* pgroup) in uinsstrt.c 
//must be called
/////////////////////
	return(SUCCESS);
}


int setEntry( int EntryID, char *data )
{
	char tmp_buf[70];
	byte chk_flag = 0;
	byte i=0;
	int  tmp_length=0;

	char *anv[4] = {"", "생일", "결혼", "첫만남"};
	int  tmp_value;
	int  k=0;

	if( EntryID < 1 || EntryID > MAX_BOOK_NUMBER ) {
		return((int)INVALID_PARAMETER); /* Invalid_Phonebook_Parameter */
	}
	wml_dial.address = EntryID; 
	wml_dial.exist = 1;
    wml_dial.anniv_contidx = 0;
	wml_dial.anniv_month   = 1;
	wml_dial.anniv_day     = 1;
	wml_dial.anniv_lunar   = 0;
	memset((void *) wml_dial.anniv_content,  ' ', A7_NV_MAX_LTRS);
	memset((void *) wml_dial.memo,  ' ', A7_NV_MAX_MEMO_SIZ);
///////////////////////////////////////////////////
// Group ID
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		if( *data < '0' || *data > '9' )  {
			return((int)INVALID_PARAMETER); // INVALID_PHONEBOOK_PARAMETER
		}
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';

	if( chk_flag )
    {
		//2001/12/17 type이 맞지 않아서 홀수 address로 &wml_dial.group
        //이 assign 되고 abort handler가 발생해서 phone keylock되는 현상때문에 수정함 
		//sscanf( tmp_buf, "%d", &wml_dial.group );
        wml_dial.group = (BYTE)Atoi(tmp_buf); 
		if ( wml_dial.group == 0 )
        {
			wml_dial.group = 100;  // ETC_GROUP
		}
        else
        {
			wml_dial.group -= 1;
#ifdef PORTING_B0_WAP
			if(!ui.isgroup[wml_dial.group])
            {
#else
            if(!IS_VALID_GROUP(wml_dial.group))
            {
#endif //PORTING_B0_WAP
				return((int)INVALID_PARAMETER);
			}
		}
	} 
    else
    {
		return((int)INVALID_PARAMETER); // INVALID_PHONEBOOK_PARAMETER
	}	
// Name
	++data;
	chk_flag = 0;
	i = 0;
	memset(tmp_buf, ' ', 40);
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	
// SAME phonebook name check
	for(k=1; k<= MAX_BOOK_NUMBER; k++) {
#ifdef PORTING_B0_WAP
		if(ui.isdial[k]) {
			tmp_dial.address = k;
			tmp_length = strlen(tmp_buf);
				(void)wap_process_nv_item( NV_DIAL_I, ( nv_item_type * )&tmp_dial, TRUE);
#else //PORTING_B0_WAP			
		if(IS_VALID_BOOK(k)) {
				tmp_length = strlen(tmp_buf);
				MakeOldBook(k, (NV_DIAL_TYPE *)&tmp_dial);
#endif //PORTING_B0_WAP
				if(memcmp(tmp_buf, (void *)tmp_dial.letters, tmp_length) == 0) /* same(1) : diff(0) */
					break;
		}
	}
	if( k != EntryID  && k != MAX_BOOK_NUMBER + 1 ) {
		return(SAME_ENTRY); /* Invalid_Parameter : same name */	
	}	

// SAME phonebook name check end
	memset((void *)wml_dial.letters, ' ', A7_NV_MAX_LTRS);
	if( chk_flag ) {
		if (strlen(tmp_buf) > A7_NV_MAX_LTRS ) {
			return((int)INVALID_PARAMETER); // INVALID_PHONEBOOK_PARAMETER
		}
		memcpy((void *)wml_dial.letters, tmp_buf , i);
	}

// Home : 집 전화번호
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	memset((void *)wml_dial.multi_digits[HOME_IDX], ' ', A7_NV_MAX_DIAL_DIGITS);
	if( chk_flag ) {
		if (strlen(tmp_buf) > A7_NV_MAX_DIAL_DIGITS ) {
			return((int)INVALID_PARAMETER); // INVALID_PHONEBOOK_PARAMETER
		}
		wml_dial.multi_num_digits[HOME_IDX] = i;
		memcpy( (void *)wml_dial.multi_digits[HOME_IDX], tmp_buf, i );
	} else {
		wml_dial.multi_num_digits[HOME_IDX] = 0;
	}

// SK_HOME : Short key
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';

// Add_HOME : 집 주소 
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';


// Mobile : 휴대폰 전화번호
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	memset((void *)wml_dial.multi_digits[HAND_IDX], ' ', A7_NV_MAX_DIAL_DIGITS);
	if( chk_flag ) {
		if (strlen(tmp_buf) > A7_NV_MAX_DIAL_DIGITS ) {
			return((int)INVALID_PARAMETER); // INVALID_PHONEBOOK_PARAMETER
		}
		wml_dial.multi_num_digits[HAND_IDX] = i;
		memcpy((void *) wml_dial.multi_digits[HAND_IDX], tmp_buf, i );
	} else {
		wml_dial.multi_num_digits[HAND_IDX] = 0;
	}
// SK_MOBILE : Short key
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';

// Pager : 호출기 전화번호 
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	memset((void *)wml_dial.multi_digits[PAGER_IDX], ' ', A7_NV_MAX_DIAL_DIGITS);
	if( chk_flag ) {
		if (strlen(tmp_buf) > A7_NV_MAX_DIAL_DIGITS ) {
			return((int)INVALID_PARAMETER); // INVALID_PHONEBOOK_PARAMETER
		}
		wml_dial.multi_num_digits[PAGER_IDX] = i;
		memcpy((void *) wml_dial.multi_digits[PAGER_IDX], tmp_buf, i );
	} else {
		wml_dial.multi_num_digits[PAGER_IDX] = 0;
	}
// SK_PAGER : Short key
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';

// School : 직장/학교 전화번호 
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	memset((void *)wml_dial.multi_digits[OFFICE_IDX], ' ', A7_NV_MAX_DIAL_DIGITS);
	if( chk_flag ) {
		if (strlen(tmp_buf) > A7_NV_MAX_DIAL_DIGITS ) {
			return((int)INVALID_PARAMETER); // INVALID_PHONEBOOK_PARAMETER
		}
		wml_dial.multi_num_digits[OFFICE_IDX] = i;
		memcpy((void *) wml_dial.multi_digits[OFFICE_IDX], tmp_buf, i );
	} else {
		wml_dial.multi_num_digits[OFFICE_IDX] = 0;
	}
// SK_SCHOOL : Short key
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';

// Add_school : 직장/학교 주소
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';

// Fax
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';

// E-mail
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	memset((void *)wml_dial.email, ' ', A7_NV_MAX_DIAL_DIGITS);
	if( chk_flag ) {
		if (strlen(tmp_buf) > A7_NV_MAX_DIAL_DIGITS ) {

			if(IsMiddleOfMultiByte((byte *)tmp_buf,A7_NV_MAX_DIAL_DIGITS)) { //IsMiddleofHan() --> IsMiddleOfMultiByte()
				memcpy((void *)wml_dial.email, tmp_buf, A7_NV_MAX_DIAL_DIGITS-1);
			} else {
				memcpy((void *)wml_dial.email, tmp_buf, A7_NV_MAX_DIAL_DIGITS);
			}
		} else {
			memcpy((void *)wml_dial.email, tmp_buf, i);
		}
	}
// Extension flag : "3+1,"
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' && *data != '\0' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';

	if(chk_flag  ) {
		if( i == 1 && tmp_buf[0] != '3' )
		{	
			return((int)INVALID_PARAMETER);
		}
		if( i == 3 && strcmp(tmp_buf,"3+1") ) 
		{	
			return((int)INVALID_PARAMETER);
		}
	} else {
		return((int)INVALID_PARAMETER);
	}

	if( i == 1 && *data != '\0' ) 
	{
		return(NOT_SUPPORTED_FORMAT);
	}

	if(*data == '\0' ) { 
		goto END;
	}

// 메모 : "간단한 메모+"
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != '+' && *data != '\0' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	if(chk_flag  ) {
		memcpy((void *)wml_dial.memo, tmp_buf, i);
	}
	if (*data != '+') 
	{
		return((int)INVALID_PARAMETER);
	}

// 다음 Extension flag "4,"
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' && *data != '\0' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	if( chk_flag ) {
		if ( i != 1 || tmp_buf[0] != '4' ) {
			return((int)INVALID_PARAMETER);
		}
	} else {
		return((int)INVALID_PARAMETER);
	}

// 양/음력
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' && *data != '\0' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	if(chk_flag) {
		//sscanf(tmp_buf,"%d",&tmp_value);
        tmp_value = Atoi(tmp_buf);
		wml_dial.anniv_lunar = (tmp_value + 1) % 2;
	} else {
		goto END;
	}
	if( *data == '\0' ) 
	{
		return((int)INVALID_PARAMETER);
	}

// 월
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' && *data != '\0' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	if(chk_flag) {
		//sscanf(tmp_buf,"%d",&tmp_value);
        tmp_value = Atoi(tmp_buf);
		wml_dial.anniv_month = tmp_value;
	} else {
		goto END;
	}
	if( *data == '\0' )
	{
		return((int)INVALID_PARAMETER);
	}

//일
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' && *data != '\0' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	if(chk_flag) {
		//sscanf(tmp_buf,"%d",&tmp_value);
        tmp_value = Atoi(tmp_buf);
		wml_dial.anniv_day = tmp_value;
	} else {
		goto END;
	}
	if( *data == '\0' )
	{
		return((int)INVALID_PARAMETER);
	}


//기념일 내용
	++data;
	chk_flag = 0;
	i = 0;
	while(*data != ',' && *data != '\0' ) {
		tmp_buf[i++] = *data++;
		chk_flag = 1;
	}
	tmp_buf[i] = '\0';
	wml_dial.anniv_contidx = 4;
	if( i > A7_NV_MAX_LTRS) return(OVERRANGE_PARAMETER);
	if(chk_flag) {
		for(tmp_value = 1; tmp_value < 4; tmp_value++) {
			if(!strcmp(anv[tmp_value], tmp_buf)) {
				wml_dial.anniv_contidx = tmp_value;
				break;
			} 
		}
		if( tmp_value == 4) {
			wml_dial.anniv_contidx = tmp_value;
			memcpy((void *) wml_dial.anniv_content, tmp_buf, i);
		}
	}
	if( *data != '\0' ) 
	{
		return((int)INVALID_PARAMETER);
	}


END:
// Cmodify flag
#ifdef PORTING_B0_WAP
	wml_dial.sync_flag  = 'S';
	(void)wap_process_nv_item( NV_DIAL_I, ( nv_item_type * )&wml_dial, FALSE);
////////////////////////////////////////////////////////////
	DmRefreshDial(wml_dial.address, &wml_dial);
////////////////////////////////////////////////////////////
#else

	wml_dial.sync_flag  = 'S';
    DeleteBookInfo(EntryID);
    if( !IS_VALID_GROUP(wml_dial.group) )
    {        
        SaveOldBook((BYTE*) SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6), &wml_dial, (INT16*)&wml_dial.address);
    }
    else
    {
        WriteOldBook(&wml_dial);
    }
    PhoneBookUpdate();
////////////////////////////////////////////////////////////
	// tmp DmRefreshDial(wml_dial.address, &wml_dial);
////////////////////////////////////////////////////////////

#endif //PORTING_B0_WAP
/////////////////////
//Don't forget !!!
//DmRefreshDial(int index, nv_group_type* pgroup) in uinsstrt.c 
//must be called
/////////////////////
	return(SUCCESS);
}

#ifdef FEATURE_PHONEBOOK_VER12
void getEntryElement(int subList, int startEntryID, int EntryID, int extFlag, char *result)
{
	char tmp_buf[100];
	char tmp_mem[70];
	char length=0;  
	char tmp_value = 0;
	char *anv[4] = {"", "생일", "결혼", "첫만남"};
	int  groupID=0;

	*result = '\0';

    if( EntryID < 1 || EntryID > MAX_BOOK_NUMBER ) {
		return; 
	}
#ifdef PORTING_B0_WAP
	wml_dial.address = EntryID;

	if( !ui.isdial[EntryID] ) 
		return ;
	(void)wap_process_nv_item( NV_DIAL_I, ( nv_item_type * )&wml_dial, TRUE);
#else //PORTING_B0_WAP
	if(!IS_VALID_BOOK(EntryID))
		return ;		
		MakeOldBook(EntryID, (NV_DIAL_TYPE *)&wml_dial);	
#endif //PORTING_B0_WAP
	if(subList & PB_ITEM_ENTRY_ID)
	{	
		char tmp[100];

		memset(tmp, '\0', 100);

		if(startEntryID != EntryID)
		{
			CLNTa_log(1,0,"I am not a StartEntryID\n");
			strcat(result, ",");
		}

		sprintf(tmp, "%d", EntryID);
		strcat(result, tmp);
		//CLNTa_log(1,0,"%s\n", result);

		if(subList & 0xffff << 1)
		{
			strcat(result, ",");
		}
	}

	if(subList & PB_ITEM_NAME)
	{
		char tmp[100];

		memset(tmp, '\0', 100);
		length = get_length((byte *)wml_dial.letters, A7_NV_MAX_LTRS );
		if(length) 
		{
			memcpy(tmp, (void *)wml_dial.letters, length);
			if(subList & 0xffff << 2)
				tmp[length] = ',';
			comma2space(tmp);
			plus2space(tmp);
			dollar2space(tmp);
			strcat(result, tmp);
		} 
		else
		{
			if(subList & 0xffff << 2)
				strcat(result, ",");
		}

		//CLNTa_log(1,0,"%s\n", result);
	}

	if(subList & PB_ITEM_HOME)
	{
		char tmp[100];

		memset(tmp, '\0', 100);
		length = wml_dial.multi_num_digits[HOME_IDX];

		if(length) 
		{
			memcpy(tmp, (void *)wml_dial.multi_digits[HOME_IDX], length);
			if(subList & 0xffff << 3) 
				tmp[length] = ',';
			strcat(result, tmp);
		}
		else
		{
			if(subList & 0xffff << 3) 
				strcat(result, ",");
		}
		
		//CLNTa_log(1,0,"%s\n", result);
	}

	if(subList & PB_ITEM_MOBILE)
	{
		char tmp[100];

		memset(tmp, '\0', 100);
		length = wml_dial.multi_num_digits[HAND_IDX];

		if( length ) 
		{
			memcpy(tmp, (void *)wml_dial.multi_digits[HAND_IDX], length);
			if(subList & 0xffff << 4)
				tmp[length] = ',';
			strcat(result, tmp);
		}
		else
		{
			if(subList & 0xffff << 4)
				strcat(result, ",");
		}

		//CLNTa_log(1,0,"%s\n", result);
	}

	if(subList & PB_ITEM_SCHOOL)
	{
		char tmp[100];

		memset(tmp, '\0', 100);
		length = wml_dial.multi_num_digits[OFFICE_IDX];
		if( length ) {
			memcpy(tmp, (void *)wml_dial.multi_digits[OFFICE_IDX], length);
			if(subList & 0xffff << 5)
				tmp[length] = ',';
			strcat(result, tmp);
		}
		else
		{
			if(subList & 0xffff << 5)
				strcat(result, ",");
		}
		
		//CLNTa_log(1,0,"%s\n", result);
	}

	if(subList & PB_ITEM_FAX)
	{	
		//CLNTa_log(1,0,"%s\n", result);

		if(subList & 0xffff << 6)
			strcat(result, ",");
	}

	if(subList & PB_ITEM_EMAIL)
	{
		char tmp[100];

		memset(tmp, '\0', 100);
		length = get_length((byte *) wml_dial.email, A7_NV_MAX_DIAL_DIGITS);

		if( length ) 
		{
			memcpy(tmp, (void *)wml_dial.email, length);
			if(subList & 0xffff << 7)
				tmp[length] = ',';
			comma2space(tmp);
			plus2space(tmp);
			dollar2space(tmp);
			strcat(result, tmp);
		}
		else
		{
			if(subList & 0xffff << 7)
				strcat(result, ",");
		}

		//CLNTa_log(1,0,"%s\n", result);
	}

	if(subList & PB_ITEM_GROUP_ID)
	{
		char tmp[100];

		memset(tmp, '\0', 100);
		if( wml_dial.group == 100 ) 
		{
			tmp_value = 0; 
		} 
		else 
		{
			tmp_value = wml_dial.group + 1;
		}
		groupID = tmp_value;
		sprintf(tmp, "%d",tmp_value);
		strcat(result, tmp);

		if(subList & 0xffff << 8)
			strcat(result, ",");

		//CLNTa_log(1,0,"%s\n", result);
	}

	if(subList & PB_ITEM_GROUP_NAME)
	{
		byte m=0;
#ifdef PORTING_B0_WAP
		nv_group_type group;
		// nv_group_type group대신에 GroupInfo이용 	
#else //PORTING_B0_WAP

#endif //PORTING_B0_WAP
		char group_name[A7_NV_MAX_LTRS +1];
		char tmp_buf[30];
		
		memset(tmp_buf, '\0', 30);
#ifdef PORTING_B0_WAP
		if( ui.isgroup[groupID] ) {				
			(void)wap_process_nv_item( NV_GROUP_I, ( nv_item_type * )&group, TRUE);
			m = get_length((byte *)group.letters, A7_NV_MAX_LTRS );
			memcpy(group_name, (void *)group.letters, m);
#else //PORTING_B0_WAP
		if( IS_VALID_GROUP(groupID)) {
			m = get_length((byte *)g_szGroupName[groupID-1], A7_NV_MAX_LTRS );
			memcpy(group_name, (void *)g_szGroupName[groupID-1], m);
#endif //PORTING_B0_WAP
			group_name[m] = '\0';
			comma2space(group_name);
			sprintf(tmp_buf, "%s", group_name);
			strcat(result,tmp_buf);
		}
		//CLNTa_log(1,0,"%s\n", result);

		//1. there is extension flag or
		//2. there is additional pb items have their value more than 0x0100

		if(subList & 0xffff << 9)
		{
			CLNTa_log(1,0,"Additional sublist case\n");
			strcat(result, ",");
		}

		if(extFlag & 0x03)
		{
			CLNTa_log(1,0,"Extension case\n");
			strcat(result, ",");
		}
	}

	if(extFlag)
		strcat(result,"3");

	if(extFlag & PB_ITEM_EXT_MEMO)
	{
		strcat(result,"+1,"); // 메모 파라메터 갯수
		length = get_length((byte *) wml_dial.memo, A7_NV_MAX_MEMO_SIZ);
		if( length ) 
		{
			if( length > 32 ) 
			{
				if(IsMiddleOfMultiByte((byte *)tmp_buf,32)) //IsMiddleofHan() --> IsMiddleOfMultiByte()
				{
					memcpy(tmp_buf, (void *)wml_dial.memo, 32 - 1);
					tmp_buf[32 - 1] = '\0';
				} 
				else 
				{
					memcpy(tmp_buf, (void *)wml_dial.memo, 32);
					tmp_buf[32] = '\0';
				}
			} 
			else 
			{
				memcpy(tmp_buf, (void *)wml_dial.memo, length);
				tmp_buf[length] = '\0';
			}

			comma2space(tmp_buf);
			plus2space(tmp_buf);
			dollar2space(tmp_buf);
			strcat(result, tmp_buf);
		}
	}

	if(extFlag & PB_ITEM_EXT_ANNI)
	{	
		strcat(result,"+4,"); // 기념일 파라메터 갯수
		if( wml_dial.anniv_contidx == 0) 
		{ // 기념일 없음
			strcat(result,",,,");
		} 
		else if ( wml_dial.anniv_contidx == 4) 
		{// 기념일 내용 편집
			tmp_value = (wml_dial.anniv_lunar + 1) % 2;	
	
			length = get_length( (byte *)wml_dial.anniv_content, A7_NV_MAX_LTRS);
	
			memcpy(tmp_mem, (void *)wml_dial.anniv_content, length);
			tmp_mem[length] = '\0';

			comma2space(tmp_mem);
			plus2space(tmp_mem);
			dollar2space(tmp_mem);

			sprintf(tmp_buf,"%d,%d,%d,%s",tmp_value, wml_dial.anniv_month, wml_dial.anniv_day, tmp_mem);
			strcat(result,tmp_buf);
		} 
		else 
		{
			tmp_value = (wml_dial.anniv_lunar + 1) % 2;		
			sprintf(tmp_buf,"%d,%d,%d,%s",tmp_value, wml_dial.anniv_month, wml_dial.anniv_day, anv[wml_dial.anniv_contidx]);
			strcat(result,tmp_buf);
		}
		//CLNTa_log(1,0,"%s\n", result);
	}

}
//////////
unsigned int getNextEntry(unsigned int start)
{
	unsigned int i;

	for(i=start; i<MAX_BOOK_NUMBER+1; i++)
	{
        if(IS_VALID_BOOK(i))
		{
			CLNTa_log(1,0,"Existing ID[%d]\n",i);
			return i;
		}
	}

	return MAX_BOOK_NUMBER;//for removing error
}

//////////

//Keep in mind 3rd arg, number is 0 case
int getEntryList(int subList, int startEntryID, int number, int extFlag, char *result, unsigned int sizeOfString)
{	
	int i, EntryID = startEntryID;
	int numberOfEntry = number;
	char subResult[300];
	
	if(result)
		memset(result, '\0', sizeOfString);
	else
		return 0;

	memset(subResult, '\0', 300);

	CLNTa_log(1,0,"subList[%d], startEntryID[%d], number[%d], extFlag[%x]\n",subList, startEntryID, number, extFlag);

	//Get entry info and list it up one by one.

	do {
		i = getNextEntry(EntryID);

		EntryID = i + 1;

		if(EntryID>=MAX_BOOK_NUMBER) break;

		memset(subResult, '\0', 300);

		getEntryElement(subList, startEntryID, i, extFlag, subResult);

		strcat(result, subResult);

		CLNTa_log(1,0,"sub%2d[%s]\n",i, subResult);

		CLNTa_log(1,0,"total[%s]\n",result);

	} while(--numberOfEntry);
}
#endif //#ifdef FEATURE_PHONEBOOK_VER12

///////////////////////////////////////////////////////////////
// PIMSYNC_VERSION_ID should be changed according to 
// the version of WMLScript extra libraries by SKTelecom
///////////////////////////////////////////////////////////////

void getVerID(char *result)
{
	MSG_ERROR("called getverID",0,0,0);//01.04.04 ytchoi to solve warning
	strcpy(result, PIMSYNC_VERSION_ID);
}
// Phonebook library end
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
//uicommon --> B1 이동 
LOCAL void comma2space(char *target)
{
	char *spaceptr = (char *)NULL;
	int  length=0;
	char *traverseptr = target;

	length = strlen(target);

	while(1) {
		spaceptr = strchr(traverseptr, ',');
		if( (spaceptr == (char *)NULL) || (spaceptr == target + length - 1) ) {
			break;
		} else {
			*spaceptr = ' ';
			if( *spaceptr++ == NULL ) {
				break;
			} else {
				traverseptr = spaceptr;
			}
		}
	}
}

LOCAL void plus2space(char *target)
{
	char *spaceptr = (char *)NULL;
	int  length=0;
	char *traverseptr = target;

	length = strlen(target);

	while(1) {
		spaceptr = strchr(traverseptr, '+');
		if( spaceptr == (char *)NULL ) {
			break;
		} else {
			*spaceptr = ' ';
			if( *spaceptr++ == NULL ) {
				break;
			} else {
				traverseptr = spaceptr;
			}
		}
	}
}

LOCAL void dollar2space(char *target)
{
	char *spaceptr = (char *)NULL;
	int  length=0;
	char *traverseptr = target;

	length = strlen(target);

	while(1) {
		spaceptr = strchr(traverseptr, '$');
		if( spaceptr == (char *)NULL ) {
			break;
		} else {
			*spaceptr = ' ';
			if( *spaceptr++ == NULL ) {
				break;
			} else {
				traverseptr = spaceptr;
			}
		}
	}
}

LOCAL int str_cmp16(byte *src, byte *target)
{
	int i;
	for(i=0; i < 16; i++) {
		if( *src++ != *target++ ) {
			return(0);        /* different */
		}
	}
	return(1);                /* same      */

}