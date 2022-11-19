/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     ska_butl.c
**
** Description: 
**     The core functions that provide EQS functionality.
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 01-01-04   redstar    SortBookName 함수 추가 
** 01-01-04   redstar    (전화번호 전체일치 검색에서 )SearchNumber의 리턴갑을 List Handle에서 BookAddress로 변경
*/

#include "ska.h"

#define MAX_STACK_SIZE     100

#define NUMBER_INFO_ARRAY_SIZE  10

#define  NUMBER_011_PREFIX    "011"
#define  NUMBER_017_PREFIX    "017"


LOCAL   INT16* l_pStack;
LOCAL   INT16  l_nStackIndex;
LOCAL   INT16  l_nStackSize;




BOOL StackInit(INT16 nSize)
{
    l_nStackIndex = 0;
    l_pStack = (INT16*) SKY_Malloc( nSize );
	l_nStackSize = nSize;
	ASSERT( l_pStack != NULL );
	if( l_pStack  )
		return TRUE;
	else
		return FALSE;
}

void StackFree(void)
{
	SKY_Free( l_pStack );
	l_pStack = NULL;
}

BOOL Push(INT16 data)
{
    if(l_nStackIndex >= l_nStackSize )
        return FALSE;
    l_pStack[l_nStackIndex++] = data;

    return TRUE;
}

INT16 Pop(void)
{
    if( l_nStackIndex > 0 )
    {
        return l_pStack[--l_nStackIndex];
    }
    return 0;
}
BOOL IsStackEmpty(void)
{
	if( l_nStackIndex == 0 )
		return TRUE;
	else
		return FALSE;
}


INT16  CountActiveRecord( PB_Type nv_type )
{
    pb_number_info_type    number_info;
    pb_anniv_info_type     anniv_info;
    pb_email_info_type      email_info;

    int count=0;
    int i;

    switch( nv_type )
    {
    case PB_ACTIVE_LIST_I:

        break;
    case PB_NUMBER_INFO_I:
        number_info.address = active_list.number_info;
        
        while( IsValid( PB_NUMBER_INFO_I, number_info.address ) == TRUE )
        {
            ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &number_info );
            count++;
            number_info.address = (number_info.next_index > 0 ) ? number_info.next_index : -(number_info.next_index);
        }

        break;
    case PB_ANNIV_INFO_I:
        for( i=0; i<12; i++)
        {
            anniv_info.address = active_list.anniv_info[i];
        
            while( IsValid( PB_ANNIV_INFO_I, anniv_info.address ) == TRUE )
            {
                ui_get_pb( PB_ANNIV_INFO_I ,(pb_item_type*) &anniv_info );
                count++;
                anniv_info.address = (anniv_info.next_index > 0 ) ? anniv_info.next_index : -(anniv_info.next_index);
            }
        }
        break;


    case PB_EMAIL_INFO_I:

        email_info.address = active_list.email_info;
        
        while( IsValid( PB_EMAIL_INFO_I, email_info.address ) == TRUE )
        {
            ui_get_pb( PB_EMAIL_INFO_I ,(pb_item_type*) &email_info );
            count++;
            email_info.address = (email_info.next_index > 0 ) ? email_info.next_index : -(email_info.next_index);
        }
        break;

    default:
     
        break;
    }

    return count;
}

INT16  CountFreeRecord( PB_Type nv_type )
{

    int count=0;
    int address;

    switch( nv_type )
    {
    case PB_ACTIVE_LIST_I:
        break;

    case PB_NUMBER_INFO_I:
        address = active_list.free_number_info;
        while( address > 0 )
        {
            count++;
            address = pb_get_address( PB_NUMBER_INFO_I , address );
        }

        break;

    case PB_ANNIV_INFO_I:
        address = active_list.free_anniv_info;
        while( address > 0 )
        {
            count++;
            address = pb_get_address( PB_ANNIV_INFO_I , address );
        }
        break;

    case PB_EMAIL_INFO_I:
        address = active_list.free_email_info;
        while( address > 0 )
        {
            count++;
            address = pb_get_address( PB_EMAIL_INFO_I , address );
        }
        break;

    default:
     
        break;
    }

    return count;
}

BOOL   IsValid( PB_Type  nv_type , INT16 address )
{

    switch( nv_type )
    {
    case PB_SDIAL_I:
        if( address > 0 && address <= MAX_SDIAL_NUMBER )
            return TRUE;
        else
            return FALSE;
        break;

    case PB_GROUP_INFO_I:
        if( address > 0 && address <= MAX_GROUP_NUMBER )
            return TRUE;
        else
            return FALSE;

        break;

    case PB_BOOK_INFO_I:
        if( address > 0 && address <= MAX_BOOK_NUMBER )
            return TRUE;
        else
            return FALSE;

        break;
    case PB_NUMBER_INFO_I:
        if( address > 0 && address <= MAX_PHONE_NUMBER )
            return TRUE;
        else
            return FALSE;

        break;
    case PB_ANNIV_INFO_I:
        if( address > 0 && address <= MAX_ANNIV_NUMBER )
            return TRUE;
        else
            return FALSE;

        break;
    case PB_EMAIL_INFO_I:
        if( address > 0 && address <= MAX_EMAIL_NUMBER )
            return TRUE;
        else
            return FALSE;

        break;

    case PB_LIST_HANDLE_I:
        if( address > 0 && address <= MAX_LIST_NUMBER )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
        break;
    default:    
        break;
    }
    return FALSE;
}

void   BookCleanUp(void)
{
    pb_group_info_type   GroupInfo;
	pb_book_info_type    BookInfo;
	 pb_info_type        PersonalInfo;
    INT16 i, k;
	BYTE  Buffer[1024];

#ifndef WIN32    
	nv_dir_number_type     nv_dir_num;  
#endif
	
    PBookInit(PB_NUMBER_INFO_I);
    PBookInit(PB_ANNIV_INFO_I); 
    PBookInit(PB_EMAIL_INFO_I);
    PBookInit(PB_ACTIVE_LIST_I);
    PBookInit(PB_SDIAL_I);
	PBookInit(PB_INFO_I);

#ifndef WIN32
	
	memset(&nv_dir_num, 0, sizeof(nv_dir_num));
    nv_dir_num.nam = SKY_GET_CURRENT_NAM();     // 자기정보 저장 
    SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );
	memset(&PersonalInfo, 0, sizeof(PersonalInfo)); 
	if( nv_dir_num.dir_number[0] != '0' )
	{
		PersonalInfo.MobileTel[0] = '0';
		STRNCPY(PersonalInfo.MobileTel+1, nv_dir_num.dir_number,MAX_TEL_LEN -1);
	}
	else
	{
		STRNCPY(PersonalInfo.MobileTel, nv_dir_num.dir_number,MAX_TEL_LEN );
	}    
    for(i=0; i< MAX_TEL_LEN; i++)
	{
		if( !isdigit(PersonalInfo.MobileTel[i]) )
		{
			PersonalInfo.MobileTel[i] = 0;
			break;
		}
	}
	PersonalInfo.MobileTel[MAX_TEL_LEN] = 0;
	ui_put_pb(PB_INFO_I, &PersonalInfo);
#endif

	memset(Buffer,0,1024);
	InitBookFile(Buffer,1024);
	

    for( k = 0; k< MAX_BOOK_NUMBER; k++)
    {
        g_szPersonName[k][0]    = 0;
        g_PersonGroup[k]        = BOOK_NOT_USE;
        g_PersonMultiGroup[k]   = BOOK_NOT_USE;
        for( i=0; i<PERSONAL_NUMBER_LIMIT; i++)
            g_PersonNumberType[k][i] = BOOK_NOT_USE;
    }
	InitNumberFile(Buffer,1024);


    for( k = 0; k< MAX_GROUP_NUMBER; k++)
    {
        g_szGroupName[k][0]     = BOOK_NOT_USE;
        g_GroupOrder[k]         = BOOK_NOT_USE;
    }
	InitGroupFile(Buffer,1024); 	
    for( k=1; k <= 6; k++)
    {
		GroupInfo.address = k;		
        if( ui_get_pb(PB_GROUP_INFO_I, &GroupInfo) > 0 )
		{
			STRCPY( g_szGroupName[k-1], GroupInfo.group_name);
			g_GroupOrder[k-1] = k;
		}
    }
	g_nGroup = k-1;
	
	InitEmailFile(Buffer,1024);

	InitAnnivFile(Buffer,1024);

	PhoneBookUpdate();

#if 0
    init_pb_node( PB_GROUP_INFO_I, &GroupInfo);
    for( k=1; k < 6; k++)
    {
        STRCPY( g_szGroupName[k-1], SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM1 + k - 1) );
        g_GroupOrder[k-1] = k;     // non secret group
        STRCPY( GroupInfo.group_name, g_szGroupName[k-1]);
        GroupInfo.address    = k;
        GroupInfo.next_index = k+1;
        ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
        active_list.nFreeGroup--;
    }
    STRCPY( g_szGroupName[k-1], SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM1 + k - 1) );
    g_GroupOrder[k-1] = k;         // non secret group
    STRCPY( GroupInfo.group_name, g_szGroupName[k-1]);
    GroupInfo.address    = k;
    GroupInfo.next_index = 0;
    ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
	active_list.nFreeGroup--;
    active_list.group_info = 1;
    g_nGroup =  k;
    PhoneBookUpdate();
#endif

}

LOCAL BOOL sIsMatchName(BYTE* pszString, BYTE* pszName, SEARCH_MODE_E Mode)
{
    INT8 i,nSize;
    
    if( STRLEN(pszString) == 0 )
        return FALSE;
    if( Mode == BSM_NAME_FULL_MATCH )
    {
		if( STRCMP(pszString, pszName) == 0 )
			return TRUE;
/*
        nSize = STRLEN(pszName);
        for(i=0; i<nSize; i++)
        {
            if( pszString[i] != pszName[i] )
                break;
        }
        if( i == nSize )
            return TRUE;
*/
    }
    else // Mode == BSM_NAME_PART_MATCH
    {			
		return IsPartialMatchString(pszString, pszName);
			
    }
    return FALSE;
}

BOOL  IsPartialMatchString(BYTE* Name, BYTE* ComparedName)
{
    WORD   wCompare, wFined;
    UINT8  nMatchKorCho = 0;
	INT    i,j;

	i = 0;
	j = 0;
	while( Name[i] != 0 && ComparedName[j] != 0 )
	{
		wCompare = ((WORD)ComparedName[j] << 8) + ComparedName[j+1];
		wFined = ((WORD)Name[i] << 8) + Name[i+1];

		if ((wCompare >= 0xa4a1) && (wCompare <= 0xa4be))
		{
            if( wCompare == wFined )
			{
				i += 2;
				j += 2;
			}
			else
			{
				switch (wCompare)
				{
				case 0xa4a1: /* kiyeuk */
					if ((wFined >= 0xb0a1) && (wFined <= 0xb1ed))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}						
					break;

				case 0xa4a2: /* ssangkiyeuk */
					if ((wFined >= 0xb1ee) && (wFined <= 0xb3a9))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4a4: /* neiun */
					if ((wFined >= 0xb3aa) && (wFined <= 0xb4d8))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;
				case 0xa4a7: /* deigeut */
					if ((wFined >= 0xb4d9) && (wFined <= 0xb5fa))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;
				case 0xa4a8: /* ssangdeigeut */
					if ((wFined >= 0xb5fb) && (wFined <= 0xb6f2))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4a9: /* leil */
					if ((wFined >= 0xb6f3) && (wFined <= 0xb8b5))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4b1: /* meum */
					if ((wFined >= 0xb8b6) && (wFined <= 0xb9d8))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4b2: /* beup */
					if ((wFined >= 0xb9d9) && (wFined <= 0xbafb))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;
				
				case 0xa4b3: /* ssangbeup */
					if ((wFined >= 0xbafc) && (wFined <= 0xbbe6))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;
				
				case 0xa4b5: /* siot */
					if ((wFined >= 0xbbe7) && (wFined <= 0xbdcd))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4b6: /* ssangsiot */
					if ((wFined >= 0xbdce) && (wFined <= 0xbec5))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4b7: /* eung */
					if ((wFined >= 0xbec6) && (wFined <= 0xc0d9))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4b8: /* zeut */
					if ((wFined >= 0xc0da) && (wFined <= 0xc2a4))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4b9: /* ssangzeut */
					if ((wFined >= 0xc2a5) && (wFined <= 0xc2f6))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}

					break;  

				case 0xa4ba: /* chiut */
					if ((wFined >= 0xc2f7) && (wFined <= 0xc4aa))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4bb: /* kiuk */
					if ((wFined >= 0xc4ab) && (wFined <= 0xc5b7))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}

					break;

				case 0xa4bc: /* tegeuk */
					if ((wFined >= 0xc5b8) && (wFined <= 0xc6c3))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4bd: /* peup */
					if ((wFined >= 0xc6c4) && (wFined <= 0xc7ce))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				case 0xa4be: /* heut */
					if ((wFined >= 0xc7cf) && (wFined <= 0xc8fe))
					{
						i += 2;
						j += 2;
					}
					else 
					{
						if( Name[i] & 0x80 )
						{
							i += 2;
						}
						else
						{
							i++;
						}
						j  = 0;
					}
					break;

				default:
					break;
				}
			}
		}
		else if( wCompare >= 0xb0a1 && wCompare <= 0xc8fe )
		{
			if( wCompare == wFined )
			{
				i += 2;
				j += 2;
			}
			else 
			{
				if( Name[i] & 0x80 )
				{
					i += 2;
				}
				else
				{
					i++;
				}				
				j  = 0;
			}

		}
		else
		{
			if( wCompare & 0x8000 )  //특수 문자 
			{
				if( Name[i] == ComparedName[j] && Name[i+1] == ComparedName[j+1] )
				{
					i+=2;
					j+=2;
				}
				else
				{
					if( Name[i] & 0x80 )
					{
						i += 2;
					}
					else
					{
						i++;
					}
					j=0;
				}
			}
			else //일반 문자 
			{
				if( toupper(Name[i]) == toupper(ComparedName[j]) )
				{
					i++;
					j++;
				}
				else
				{
					if( Name[i] & 0x80 )
					{
						i += 2;
					}
					else
					{
						i++;
					}
					j = 0;
				}
			}
		}		
	}
	if( j >= STRLEN(ComparedName) )
		return TRUE;
	else
		return FALSE;
}

#define	    KOREAN_KIYEUK       0				
#define     KOREAN_SSANGKIYEUK  KOREAN_KIYEUK
#define     KOREAN_NEIUN        KOREAN_SSANGKIYEUK+1 
#define     KOREAN_DEIGEUT      KOREAN_NEIUN +1		    
#define     KOREAN_SSANGDEIGEUT KOREAN_DEIGEUT
#define     KOREAN_LEIL         KOREAN_SSANGDEIGEUT+1
#define     KOREAN_MEUM         KOREAN_LEIL+1
#define     KOREAN_BEUP			KOREAN_MEUM +1 
#define     KOREAN_SSANGBEUP    KOREAN_BEUP	        
#define     KOREAN_SIOT         KOREAN_SSANGBEUP + 1
#define     KOREAN_SSANGSIOT    KOREAN_SIOT
#define     KOREAN_EUNG         KOREAN_SSANGSIOT+1		    
#define     KOREAN_ZEUT         KOREAN_EUNG+1
#define     KOREAN_SSANGZEUT    KOREAN_ZEUT
#define     KOREAN_CHIUT        KOREAN_SSANGZEUT+1
#define     KOREAN_KIUK         KOREAN_CHIUT+1
#define     KOREAN_TEGEUK       KOREAN_KIUK+1
#define     KOREAN_PEUP         KOREAN_TEGEUK+1
#define     KOREAN_HEUT         KOREAN_PEUP+1

//#define     KOREAN_NONE         0
#define     KOREAN_NONE         KOREAN_HEUT + 1


UINT8 GetKoreanCho(BYTE* Name)
{
    WORD   wCompare;
    UINT8  i,nMatchKorCho;
   
    i = 0;
    nMatchKorCho = KOREAN_NONE;

    while( Name[i] ==' ' )
       i++;
    if( Name[i] == 0 )
    	return nMatchKorCho;
          
    wCompare = ((WORD)Name[i] << 8) + Name[i+1];
      
    if ((wCompare >= 0xa4a1) && (wCompare <= 0xa4be)) 
    {
        switch (wCompare)
	    {
	    case 0xa4a1: /* kiyeuk */
		    nMatchKorCho = KOREAN_KIYEUK;
		    break;

	    case 0xa4a2: /* ssangkiyeuk */
		    nMatchKorCho = KOREAN_SSANGKIYEUK;
		    break;

        case 0xa4a4: /* neiun */
			nMatchKorCho = KOREAN_NEIUN ;
			break;

        case 0xa4a7: /* deigeut */			    
			nMatchKorCho = KOREAN_DEIGEUT ;
			break;

        case 0xa4a8: /* ssangdeigeut */
			nMatchKorCho = KOREAN_SSANGDEIGEUT;
		    break;

        case 0xa4a9: /* leil */
			nMatchKorCho = KOREAN_LEIL;
		    break;

        case 0xa4b1: /* meum */
			nMatchKorCho = KOREAN_MEUM;
		    break;

        case 0xa4b2: /* beup */
			nMatchKorCho = KOREAN_BEUP;
		    break;
	    
        case 0xa4b3: /* ssangbeup */			    
			nMatchKorCho = KOREAN_SSANGBEUP;
		    break;
	    
        case 0xa4b5: /* siot */
			nMatchKorCho = KOREAN_SIOT;
		    break;

	    case 0xa4b6: /* ssangsiot */
			nMatchKorCho = KOREAN_SSANGSIOT;
		    break;

        case 0xa4b7: /* eung */			    
			nMatchKorCho = KOREAN_EUNG;
		    break;

        case 0xa4b8: /* zeut */
			nMatchKorCho = KOREAN_ZEUT;
		    break;

        case 0xa4b9: /* ssangzeut */
			nMatchKorCho = KOREAN_SSANGZEUT;
		    break;  

        case 0xa4ba: /* chiut */
			nMatchKorCho = KOREAN_CHIUT;  
		    break;

        case 0xa4bb: /* kiuk */
			nMatchKorCho = KOREAN_KIUK;
		    break;

        case 0xa4bc: /* tegeuk */
		    nMatchKorCho = KOREAN_TEGEUK;
            break;

        case 0xa4bd: /* peup */
		    nMatchKorCho = KOREAN_PEUP;
		    break;

        case 0xa4be: /* heut */
			nMatchKorCho = KOREAN_HEUT;
		    break;

        default:
		    nMatchKorCho = KOREAN_NONE;
		    break;
        }
    }
	else if( wCompare >= 0xb0a1 && wCompare <= 0xc8fe )
	{
	    /* kiyeuk */
		if ((wCompare >= 0xb0a1) && (wCompare <= 0xb1ed))
		{
		    nMatchKorCho = KOREAN_KIYEUK;
		}
	     /* ssangkiyeuk */
		else if ((wCompare >= 0xb1ee) && (wCompare <= 0xb3a9))
		{
		    nMatchKorCho = KOREAN_SSANGKIYEUK ;
		}
		/* neiun */
		else if ((wCompare >= 0xb3aa) && (wCompare <= 0xb4d8))
		{
			nMatchKorCho = KOREAN_NEIUN;
		}
		/* deigeut */
		else if ((wCompare >= 0xb4d9) && (wCompare <= 0xb5fa))
		{
			nMatchKorCho = KOREAN_DEIGEUT;
		}
		/* ssangdeigeut */
		else if ((wCompare >= 0xb5fb) && (wCompare <= 0xb6f2))
		{
			nMatchKorCho = KOREAN_SSANGDEIGEUT;
		}
		/* leil */
		else if ((wCompare >= 0xb6f3) && (wCompare <= 0xb8b5))
		{
			nMatchKorCho = KOREAN_LEIL;
		}
		 /* meum */
		else if ((wCompare >= 0xb8b6) && (wCompare <= 0xb9d8))
		{
			nMatchKorCho = KOREAN_MEUM ;
		}
		/* beup */
		else if ((wCompare >= 0xb9d9) && (wCompare <= 0xbafb))
		{
			nMatchKorCho = KOREAN_BEUP;
		}
		/* ssangbeup */
		else if ((wCompare >= 0xbafc) && (wCompare <= 0xbbe6))
		{
			nMatchKorCho = KOREAN_SSANGBEUP;
		}
		/* siot */
		else if ((wCompare >= 0xbbe7) && (wCompare <= 0xbdcd))
		{
			nMatchKorCho = KOREAN_SIOT;
		}
		/* ssangsiot */
		else if ((wCompare >= 0xbdce) && (wCompare <= 0xbec5))
		{
			nMatchKorCho = KOREAN_SSANGSIOT;
		}
		/* eung */
		else if ((wCompare >= 0xbec6) && (wCompare <= 0xc0d9))
		{
			nMatchKorCho = KOREAN_EUNG;
		}
		/* zeut */
		else if ((wCompare >= 0xc0da) && (wCompare <= 0xc2a4))
		{
			nMatchKorCho = KOREAN_ZEUT;
		}
		/* ssangzeut */
		else if ((wCompare >= 0xc2a5) && (wCompare <= 0xc2f6))
		{
			nMatchKorCho = KOREAN_SSANGZEUT;
		}
		/* chiut */
		else if ((wCompare >= 0xc2f7) && (wCompare <= 0xc4aa))
		{
			nMatchKorCho = KOREAN_CHIUT;
		}
		/* kiuk */
		else if ((wCompare >= 0xc4ab) && (wCompare <= 0xc5b7))
		{
			nMatchKorCho = KOREAN_KIUK ;
		}
		 /* tegeuk */
		else if ((wCompare >= 0xc5b8) && (wCompare <= 0xc6c3))
		{
			nMatchKorCho = KOREAN_TEGEUK;
		}
		/* peup */
		else if ((wCompare >= 0xc6c4) && (wCompare <= 0xc7ce))
		{
			nMatchKorCho = KOREAN_PEUP;
		}
		 /* heut */		
		else// if ((wCompare >= 0xc7cf) && (wCompare <= 0xc8fe))
		{
			nMatchKorCho = KOREAN_HEUT;
		}
    }
	else
	{
		;
	}
    return nMatchKorCho;
}

/* rosa 2001-09-24 --> @why, what */
//INT8  KOREAN_STRCMP(BYTE* pszStr1, BYTE* pszStr2 )
INT32  KOREAN_STRCMP(BYTE* pszStr1, BYTE* pszStr2 )
/* <-- */
{ 
	UINT8 nCho1, nCho2;

	nCho1 = GetKoreanCho(pszStr1);
	nCho2 = GetKoreanCho(pszStr2);

	if( nCho1 == nCho2 )
	{
		return strcmp((char*)pszStr1, (char*)pszStr2);
	}
	else if( nCho1 < nCho2 )
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

UINT8 SearchName( BYTE* Name, SEARCH_MODE_E Mode)
{
    INT16 nIndex;
    UINT8 Handle;

    if( Name == NULL )
	{
        return 0;
	}
        
    if( STRLEN(Name) == 0 )
	{
        return 0;
	}

    Handle = CreateList(MAX_BOOK_NUMBER);
    for( nIndex=0; nIndex<MAX_BOOK_NUMBER; nIndex++)
    {
        if( sIsMatchName( g_szPersonName[nIndex], Name, Mode) == TRUE )
        {
			if( IS_SECRET_GROUP(GET_BOOK_GROUP(nIndex+1)) == FALSE )
			{
				AddList(Handle, nIndex+1, EQS_NA);
			}
        }
    }
    if( GetListSize(Handle) == 0 )
    {
        DeleteList(Handle);
        return 0;
    }
    return Handle;
}
INT16    FindBook(BYTE* szGroup, BYTE* szName)
{
    INT8   nGroupAddress;
    INT16  nIndex;

    if( szName == NULL )
        return 0;

    if( STRLEN(szName) == 0 )
        return 0;

    if( szGroup == NULL )
	{
        nGroupAddress = 0;
	}
    else
	{
        nGroupAddress = SearchGroup(szGroup);
	}

    if( nGroupAddress == 0 )
        return 0;

    for( nIndex=0; nIndex<MAX_BOOK_NUMBER; nIndex++)
    {
        if( g_szPersonName[nIndex][0] != BOOK_NOT_USE )
        {
            if( KOREAN_STRCMP( g_szPersonName[nIndex], szName) == 0 )
            {
                if( nGroupAddress == 0 )
                {
                    break;
                }
                else
                {
                    if( g_PersonGroup[nIndex] == nGroupAddress )
                        break;
                }
            }
        }
    }

    if( nIndex >= MAX_BOOK_NUMBER )
	{
        return 0;
	}

    return (nIndex+1);
}



UINT8 SearchGroup(BYTE* name)
{
    UINT8 nIndex;
   
    for( nIndex=0; nIndex< MAX_GROUP_NUMBER; nIndex++)
    {
        if( KOREAN_STRCMP( g_szGroupName[nIndex], name) == 0 )
            break;
    }
    if( nIndex >= MAX_GROUP_NUMBER )
        return 0;
    else
    {
        return nIndex+1;
    }
}

#if 0
void MoveGroup(UINT8 FromGroup, UINT8 ToGroup)
{
    UINT8 FromHandle, ToHandle;
    INT16 nSize, i;
    pb_group_info_type ToGroupInfo, FromGroupInfo;



    FromHandle = GetBookList(FromGroup);
    ToHandle   = GetBookList(ToGroup);
    SortBookName(FromHandle);
    SortBookName(ToHandle);

    ToGroupInfo.address = ToGroup;
    ui_get_pb( PB_GROUP_INFO_I, &ToGroupInfo);

    FromGroupInfo.address = FromGroup;
    ui_get_pb( PB_GROUP_INFO_I, &FromGroupInfo);

    nSize = GetListSize(FromHandle);
    for(i=nSize-1; i>= 0; i--)
    {
        MoveMultiBook(&ToGroupInfo, GetListValue(FromHandle,i), ToHandle);
    }
    ui_put_pb( PB_GROUP_INFO_I, &ToGroupInfo);

    FromGroupInfo.wMember            = 0;
    FromGroupInfo.first_book_address = 0;
    ui_put_pb( PB_GROUP_INFO_I, &FromGroupInfo);
    
    DeleteList(FromHandle);
    DeleteList(ToHandle);
}
#endif

void MoveGroup(UINT8 FromGroup, UINT8 ToGroup)
{
    pb_group_info_type GroupInfo;
	pb_book_info_type  BookInfo;
	INT16  i,j;

	//GroupInfo.address = ToGroup;
	//ui_get_pb( PB_GROUP_INFO_I, &GroupInfo);
	for(i=0; i<MAX_BOOK_NUMBER; i++)
	{
		if( g_PersonGroup[i] == FromGroup )
		{
			g_PersonGroup[i] = ToGroup;
			BookInfo.address = i+1;
			ui_get_pb( PB_BOOK_INFO_I, &BookInfo);
			BookInfo.group_address = ToGroup;
			//GroupInfo.wMember++;
			ui_put_pb( PB_BOOK_INFO_I, &BookInfo);
			SKY_KickDog();
		}
	}
	//ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
}

BYTE* FindNumber(BYTE* szNumber)
{
    INT16 BookAddress;
	UINT8 Handle;
   
    if( szNumber == NULL )
        return NULL;
    if( STRLEN(szNumber) == 0 )
        return NULL;
	
    BookAddress = SearchNumber(szNumber, BSM_NUMBER_FULL_MATCH);
	if( BookAddress > 0 )
	{
		if( IS_SECRET_GROUP(g_PersonGroup[BookAddress-1]) == FALSE )
			return g_szPersonName[BookAddress-1];
	}
	
	return NULL;

#if 0

	Handle = SearchNumber(szNumber,BSM_NUMBER_PART_MATCH);
	if( GetListSize(Handle) > 0 )
	{
		BookAddress = GetListValue(Handle, 0);
		DeleteList(Handle);
		if( BookAddress > 0 )
		{
			if( IS_SECRET_GROUP(g_PersonGroup[BookAddress-1]) == FALSE )
				return g_szPersonName[BookAddress-1];
		}
	}
	return NULL;
#endif
}

INT16 SearchNumber(BYTE* pszNumber, SEARCH_MODE_E Mode)
{
	pb_number_info_type number_info, Number[NUMBER_INFO_ARRAY_SIZE];
	INT8 handle;
    int i, j, nDigit, nNumberLen, count;
    char* ret_v;
    BOOL fSearchTable[MAX_BOOK_NUMBER];
	BYTE   szBuf[63];

	count = 0;
		
    if( pszNumber == NULL )
        return 0;
    if( STRLEN(pszNumber) == 0 )
        return 0;

    if( Mode == BSM_NUMBER_FULL_MATCH )
    {	  
/*		
        number_info.address = active_list.number_search[SumOfNumber(pszNumber)%10];
		nNumberLen = STRLEN( pszNumber );        
	    while( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &number_info ) > 0 )
	    {
			number_info.address = active_list.number_search[SumOfNumber(pszNumber)%10];
        
			while( ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &number_info ) > 0 )
			{
				if( KOREAN_STRCMP(number_info.digits, pszNumber) == 0 )
				{	
					if( IS_VALID_BOOK(number_info.book_address) == TRUE)
						return number_info.book_address; 
				}                       
				number_info.address = number_info.slink;
				ui_get_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &number_info );
			}
			return 0;
		}
*/
		return Search011Number(pszNumber);
    }
    else if( Mode == BSM_NUMBER_PART_MATCH )
    {
        for(i=0; i< MAX_BOOK_NUMBER; i++)
            fSearchTable[i] = FALSE;

        for(i=1; i<=MAX_PHONE_NUMBER; i+=NUMBER_INFO_ARRAY_SIZE)
        {
            if( i > active_list.number_info_limit )  // 더이상 저장된 number info 정보가 없다.
                break;

            Read_number(Number,i,NUMBER_INFO_ARRAY_SIZE);
            for(j=0;j<NUMBER_INFO_ARRAY_SIZE;j++)
            {           
                if( (i+j) > active_list.number_info_limit )  // 더이상 유효한 데이터가 아닌 경우 
                    break;
                if( Number[j].digits[0] != 0 )
				{
					ret_v = strstr((char *)Number[j].digits, pszNumber);
					if( ret_v != NULL )
					{
						if( fSearchTable[Number[j].book_address-1] == FALSE )
						{
							fSearchTable[Number[j].book_address-1] = TRUE;
							count++;
						}
					}
				}
	        }
        }

        if( count == 0 )
            return 0;
        else
        {
            handle = CreateList(MAX_BOOK_NUMBER);
            for(i=0; i< MAX_BOOK_NUMBER; i++)
            {
                if( fSearchTable[i] == TRUE )
                {
                    if( IS_VALID_BOOK(i+1) )  // check valid information
					{
                        if( IS_SECRET_GROUP(GET_BOOK_GROUP(i+1)) == FALSE )
						{
							AddList(handle, i+1 , EQS_NA);
						}
					}
                }
            }
            if( GetListSize(handle) == 0 ) 
            {
                DeleteList(handle);
                handle = 0;
            }
            return handle;
        }
    }
    else
    {
        return 0;
    }
}


void GetNumberTypeSFont(UINT8 nSource, BYTE szTypeFontBuf[])
{
	switch(nSource)
	{
	  case CELL:
		  sprintf((char*)szTypeFontBuf,"%s", (BYTE*)SF_CELLULAR);
		  break;

	  case HOME:
		  sprintf((char*)szTypeFontBuf,"%s", (BYTE*)SF_HOME);
		  break;

	  case WORK:
		  sprintf((char*)szTypeFontBuf,"%s", (BYTE*)SF_OFFICE);
		  break;

	  case PAGER:
		  sprintf((char*)szTypeFontBuf,"%s", (BYTE*)SF_PAGER);
		  break;

	  case FAX:
		  sprintf((char*)szTypeFontBuf,"%s", (BYTE*)SF_FAX);
		  break;

	  default:
		  STRCPY(szTypeFontBuf, " ");
		  break;
	}
}

BOOL FindSMSNameInBook(BYTE**ppNumber, BOOL* pfaMatch, UINT8 DataSize)
{

	pb_number_info_type Number[NUMBER_INFO_ARRAY_SIZE];
	BOOL   fMatchTable[MAX_VMS_MSG];
	BYTE   szBuf[63];
	BYTE*  pszNumber;
    int i, j, k, nIndex, nPhoneNumberSize, nDigit, nMatch;

  
	for( i = 0;  i< MAX_VMS_MSG; i++)
		fMatchTable[i] = FALSE;
	nMatch = 0;

	for( i=0; i< DataSize; i++)
	{
		pfaMatch[i] = FALSE;
	}

    for(i=1; i<=MAX_PHONE_NUMBER; i+=NUMBER_INFO_ARRAY_SIZE)
    {
        if( i > active_list.number_info_limit )  // 더이상 저장된 number info 정보가 없다.
            break;

        Read_number(Number, i, NUMBER_INFO_ARRAY_SIZE);
        for(j=0;j<NUMBER_INFO_ARRAY_SIZE;j++)
        {           
            if( (i+j) > active_list.number_info_limit )  // 더이상 유효한 데이터가 아닌 경우 
                break;
			nPhoneNumberSize = STRLEN(Number[j].digits);
			if( nPhoneNumberSize > 0 )
			{
				for(k=0; k< DataSize; k++)
				{
					if( fMatchTable[k] == FALSE )
					{
						pszNumber = (BYTE*)ppNumber[k];
						if( pszNumber[0] != 0 )
						{
							nDigit = nPhoneNumberSize - STRLEN(pszNumber);
							if( nDigit == 0 )
							{
								if( KOREAN_STRCMP( Number[j].digits, pszNumber) == 0 )
								{						                                                 
									if( IS_SECRET_GROUP(g_PersonGroup[Number[j].book_address-1]) )
									{
										;
									}
									else
									{	                               
										STRCPY(pszNumber, g_szPersonName[Number[j].book_address-1]);
										pfaMatch[k] = TRUE;
									}
									fMatchTable[k] = TRUE;
									nMatch++;									
								}
							}
							else if ( nDigit == 3 )
							{
								if( SKY_GET_SERVICE_PROVIDER() == SP_STI )
								{
									STRCPY(szBuf, NUMBER_017_PREFIX);
								}
								else  // SP_NONE or SP_SKT
								{
									STRCPY(szBuf, NUMBER_011_PREFIX);//"011");
								}
								STRCAT(szBuf, pszNumber);
								if( KOREAN_STRCMP( Number[j].digits, szBuf) == 0 )
								{
									if( IS_SECRET_GROUP(g_PersonGroup[Number[j].book_address-1]) )
									{
										;
									}
									else
									{						
										STRCPY(pszNumber, g_szPersonName[Number[j].book_address-1]);
										pfaMatch[k] = TRUE;
									}						
									fMatchTable[k] = TRUE;
									nMatch++;
								}
								
							}
							else if( nDigit == -3 )
							{
								if( SKY_GET_SERVICE_PROVIDER() == SP_STI )
								{
									STRCPY(szBuf, NUMBER_017_PREFIX);
								}
								else  // SP_NONE or SP_SKT
								{
									STRCPY(szBuf, NUMBER_011_PREFIX);//"011");
								}
								STRCAT(szBuf, Number[j].digits);
								if( KOREAN_STRCMP( szBuf, pszNumber) == 0 )
								{
									if( IS_SECRET_GROUP(g_PersonGroup[Number[j].book_address-1]) )
									{
										;
									}
									else
									{
										STRCPY(pszNumber, g_szPersonName[Number[j].book_address-1]);
										pfaMatch[k] = TRUE;
									}						
									fMatchTable[k] = TRUE;
									nMatch++;
								}				
							}
							else
								;
						}
					}
				}
			}
			if( nMatch == DataSize )
				return TRUE;
		}
	}
	return FALSE;
}

#define MAX_MATCH_TABLE_SIZE  10

BOOL FindCallerNameInBook(UINT8 Handle, BYTE*pName, UINT8 nNameLen, UINT8 nStartIndex, UINT8 nSize)
{

	pb_number_info_type Number[NUMBER_INFO_ARRAY_SIZE];
	BOOL   fMatchTable[MAX_MATCH_TABLE_SIZE];
	BYTE   szBuf[63];
	BYTE   szTmpBuf[63]; /* added by hjcho 01-04-22 */
	BYTE   szType[5];    /* added by hjcho 01-04-22 */
    int i, j, k, nIndex, nPhoneNumberSize, nDigit, nMatch;

	CALLSTACK_T*    pCallStack;       
    
    pCallStack = GetCSKData();    
	for( i = 0;  i< MAX_MATCH_TABLE_SIZE; i++)
		fMatchTable[i] = FALSE;
	nMatch = 0;

    for(i=1; i<=MAX_PHONE_NUMBER; i+=NUMBER_INFO_ARRAY_SIZE)
    {
        if( i > active_list.number_info_limit )  // 더이상 저장된 number info 정보가 없다.
            break;

        Read_number(Number,i,NUMBER_INFO_ARRAY_SIZE);
        for(j=0;j<NUMBER_INFO_ARRAY_SIZE;j++)
        {           
            if( (i+j) > active_list.number_info_limit )  // 더이상 유효한 데이터가 아닌 경우 
                break;
			nPhoneNumberSize = STRLEN(Number[j].digits);
			if( nPhoneNumberSize > 0 )
			{
				for(k=nStartIndex; k< (nStartIndex+nSize); k++)
				{
					if( !fMatchTable[k-nStartIndex] )
					{
						nIndex = GetListValue( Handle, k);
						if( pCallStack->Stack[nIndex].aDigits[0] != 0 )
						{
							nDigit = nPhoneNumberSize - STRLEN(pCallStack->Stack[nIndex].aDigits);
							if( nDigit == 0 )
							{
								if( KOREAN_STRCMP( Number[j].digits, pCallStack->Stack[nIndex].aDigits) == 0 )
								{									
									if( IS_SECRET_GROUP(g_PersonGroup[Number[j].book_address-1]) )
									{
										STRCPY( pName + (k-nStartIndex)*nNameLen,"");
									}
									else
									{
										GetNumberTypeSFont(Number[j].number_type, szType);
										sprintf((char*) szTmpBuf, "%s%s", g_szPersonName[Number[j].book_address-1], szType);
										//STRCPY(pName + (k-nStartIndex)*nNameLen, g_szPersonName[Number[j].book_address-1]);
										STRCPY(pName + (k-nStartIndex)*nNameLen, szTmpBuf);
									}
									fMatchTable[k-nStartIndex] = TRUE;
									nMatch++;									
								}
							}
							else if ( nDigit == 3 )
							{
								if( SKY_GET_SERVICE_PROVIDER() == SP_STI )
								{
									STRCPY(szBuf, NUMBER_017_PREFIX);
								}
								else  // SP_NONE or SP_SKT
								{
									STRCPY(szBuf, NUMBER_011_PREFIX);//"011");
								}								
								STRCAT(szBuf, pCallStack->Stack[nIndex].aDigits);
								if( KOREAN_STRCMP( Number[j].digits, szBuf) == 0 )
								{
									if( IS_SECRET_GROUP(g_PersonGroup[Number[j].book_address-1]) )
									{
										STRCPY( pName + (k-nStartIndex)*nNameLen,"");
									}
									else
									{
										GetNumberTypeSFont(Number[j].number_type, szType);
										sprintf((char*) szTmpBuf, "%s%s", g_szPersonName[Number[j].book_address-1], szType);
										STRCPY(pName + (k-nStartIndex)*nNameLen, szTmpBuf);

										//STRCPY(pName + (k-nStartIndex)*nNameLen, g_szPersonName[Number[j].book_address-1]);
									}						

									fMatchTable[k-nStartIndex] = TRUE;
									nMatch++;
								}
								
							}
							else if( nDigit == -3 )
							{							
								if( SKY_GET_SERVICE_PROVIDER() == SP_STI )
								{
									STRCPY(szBuf, NUMBER_017_PREFIX);
								}
								else  // SP_NONE or SP_SKT
								{
									STRCPY(szBuf, NUMBER_011_PREFIX);//"011");
								}
								STRCAT(szBuf, Number[j].digits);
								if( KOREAN_STRCMP( szBuf, pCallStack->Stack[nIndex].aDigits) == 0 )
								{
									if( IS_SECRET_GROUP(g_PersonGroup[Number[j].book_address-1]) )
									{
										STRCPY( pName+ (k-nStartIndex)*nNameLen,"");
									}
									else
									{
										GetNumberTypeSFont(Number[j].number_type, szType);
										sprintf((char*) szTmpBuf, "%s%s", g_szPersonName[Number[j].book_address-1], szType);
										STRCPY(pName + (k-nStartIndex)*nNameLen, szTmpBuf);
										//STRCPY(pName + (k-nStartIndex)*nNameLen, g_szPersonName[Number[j].book_address-1]);
									}						
									fMatchTable[k-nStartIndex] = TRUE;
									nMatch++;
								}				
							}
							else
								;
						}
					}
				}
			}
			if( nMatch == nSize )
				return TRUE;
		}
	}
	return FALSE;
}


INT16 Search011Number(BYTE* pszNumber)
{
	pb_number_info_type Number[NUMBER_INFO_ARRAY_SIZE];
	BYTE   szBuf[63];	
    int i, j, nDigit, nLen;
    char* ret_v;
		
    if( pszNumber == NULL )
        return 0;
	nLen = STRLEN(pszNumber);
    if( nLen == 0 )
        return 0;

    for(i=1; i<=MAX_PHONE_NUMBER; i+=NUMBER_INFO_ARRAY_SIZE)
    {
        if( i > active_list.number_info_limit )  // 더이상 저장된 number info 정보가 없다.
            break;

        Read_number(Number,i,NUMBER_INFO_ARRAY_SIZE);
        for(j=0;j<NUMBER_INFO_ARRAY_SIZE;j++)
        {           
            if( (i+j) > active_list.number_info_limit )  // 더이상 유효한 데이터가 아닌 경우 
                break;

			nDigit = STRLEN(Number[j].digits);

			if( nDigit > 0 )
			{
				nDigit = nDigit - nLen;

				if( nDigit == 0 )
				{
					if( KOREAN_STRCMP( Number[j].digits, pszNumber) == 0 )
					{										
						if( IS_VALID_BOOK( Number[j].book_address) == TRUE)
						{
							if( IS_SECRET_GROUP(GET_BOOK_GROUP(Number[j].book_address)) == FALSE )
								return Number[j].book_address;
							else
								return 0;
						}
					}
				}
				else if ( nDigit == 3 )
				{
					if( SKY_GET_SERVICE_PROVIDER() == SP_STI )
					{
						STRCPY(szBuf, NUMBER_017_PREFIX);
					}
					else  // SP_NONE or SP_SKT
					{
						STRCPY(szBuf, NUMBER_011_PREFIX);//"011");
					}
					STRCAT(szBuf, pszNumber);
					if( KOREAN_STRCMP( Number[j].digits, szBuf) == 0 )
					{
						if( IS_VALID_BOOK( Number[j].book_address) == TRUE)
						{
							if( IS_SECRET_GROUP(GET_BOOK_GROUP(Number[j].book_address)) == FALSE )
								return Number[j].book_address;
							else
								return 0;
						}
					}				
				}
				else if( nDigit == -3 )
				{
					if( SKY_GET_SERVICE_PROVIDER() == SP_STI )
					{
						STRCPY(szBuf, NUMBER_017_PREFIX);
					}
					else  // SP_NONE or SP_SKT
					{
						STRCPY(szBuf, NUMBER_011_PREFIX);//"011");
					}
					STRCAT(szBuf, Number[j].digits);
					if( KOREAN_STRCMP( szBuf, pszNumber) == 0 )
					{
						if( IS_VALID_BOOK( Number[j].book_address) == TRUE)
						{
							if( IS_SECRET_GROUP(GET_BOOK_GROUP(Number[j].book_address)) == FALSE )
								return Number[j].book_address;
							else
								return 0;
						}
					}				
				}
				else
					;
			}		
		}

    }
	return 0;
}

LOCAL UINT16  sMoT_Partition(UINT8 Handle, INT16 left, INT16 right)
{
	INT16  center;
	if( right - left < 2  )
		return 0;
	center = (left+right)/2;
    if( KOREAN_STRCMP( g_szPersonName[GetListValue(Handle,left)-1],\
                       g_szPersonName[GetListValue(Handle,center)-1] ) > 0 )
	{
		SwapListItem(Handle,left,center);
	}

    if( KOREAN_STRCMP( g_szPersonName[GetListValue(Handle,left)-1],\
                       g_szPersonName[GetListValue(Handle,right)-1] ) > 0 )
	{
		SwapListItem(Handle,left,right);
	}

    if( KOREAN_STRCMP( g_szPersonName[GetListValue(Handle,center)-1],\
                       g_szPersonName[GetListValue(Handle,right)-1] ) > 0 )
	{
		SwapListItem(Handle,center,right);
	}
	return center;
}

LOCAL void booksort(UINT8 Handle, INT16 left, INT16 right)
{
    int v, i,j;

	StackInit((right-left+1)*3);

	for( ; ; )
	{
		while( right > left )
		{

		    v = right - left ;
			if( v == 1 )
			{
				if( KOREAN_STRCMP( g_szPersonName[GetListValue(Handle,left)-1],\
								   g_szPersonName[GetListValue(Handle,right)-1] ) > 0 )
				{
					SwapListItem(Handle,left,right);
				}
				break;
			}
			else if ( v == 2 )
			{
                sMoT_Partition(Handle, left, right);
                break;
			}
			else
			{
	            v = sMoT_Partition(Handle, left, right);
				SwapListItem(Handle,v,right-1);
			    v = right-1;
                i = left;
				j = right-1;
				while( 1 ) // partition
				{
					while( KOREAN_STRCMP( g_szPersonName[GetListValue(Handle,++i)-1],\
										  g_szPersonName[GetListValue(Handle,v)-1] ) < 0 )
					{
									   ;
					}
					while( KOREAN_STRCMP( g_szPersonName[GetListValue(Handle,--j)-1],\
										  g_szPersonName[GetListValue(Handle,v)-1] ) > 0 )
					{
									   ;
					}
					if(  i >= j )
						break;

					SwapListItem(Handle,i,j);
				}
				SwapListItem(Handle,i,right-1);
				if(  i - left > right - i )
				{
					Push(left);
					Push(i-1);
					left  = i+1;
				}
				else
				{
					Push(i+1);
					Push(right);
					right = i - 1;
				}
			}

		}
		if( IsStackEmpty() )
			break;
		right = Pop();
		left  = Pop();
	}
	StackFree();
}

void SortBookName(UINT8 Handle)               // 2001.01.05
{
    INT16  i, j, nSize, nValue, nType;
      
    nSize = GetListSize(Handle);
	booksort( Handle, 0, nSize-1);
}
        
UINT8 GetGroupList(void)               // 2001.01.05
{
    UINT8  Handle, nOrder;
	INT8   nIndex;
    
    Handle = CreateList(MAX_GROUP_NUMBER+1);
    SetListSize(Handle, g_nGroup);   
    for( nIndex=0; nIndex< MAX_GROUP_NUMBER; nIndex++)
    {
        if( g_szGroupName[nIndex][0] != BOOK_NOT_USE )
        {
            nOrder = GET_GROUP_ORDER(nIndex+1);
            SetListValue(Handle,nOrder-1, nIndex+1);
        }
    }
	
	for( nIndex=g_nGroup-1; nIndex>= 0; nIndex--)
	{
		if( GetListValue(Handle,nIndex) == 0 )
		{
			RemoveListItem(Handle,nIndex);
		}
	}
    return Handle;
}

UINT8 SortGroup(void)
{
    UINT8  Handle;	
    UINT8  Index[MAX_GROUP_NUMBER+1];
	UINT8  last_group = 0;
	INT    i, j, temp, size;
    pb_group_info_type GroupInfo;
    
    Handle = CreateList(MAX_GROUP_NUMBER+1);
	size   = 0;
   
    for(i=0; i< MAX_GROUP_NUMBER; i++)
    {
        if( g_szGroupName[i][0] != 0 )
        {       
            if(KOREAN_STRCMP(g_szGroupName[i] , SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6)) != 0 )
                Index[size++] = i;      
            else 
                last_group = i+1;
        }
    }

	if( size == 0 )
	{
		Index[size] = last_group - 1;
	}

    for(i=size-1; i> 0; i--)
    {
        for( j=0; j< i; j++)
        {
            if( KOREAN_STRCMP( g_szGroupName[Index[j]], g_szGroupName[Index[j+1]]) > 0 )
            {
                temp = Index[j];
                Index[j] = Index[j+1];
                Index[j+1] = temp;
            }
        }
    }
    for(i=0; i<size; i++)
    {
        AddList(Handle,Index[i]+1,EQS_NA);
        SET_GROUP_ORDER(Index[i]+1, i+1);
        //g_GroupOrder[Index[i]] = i+1;

        GroupInfo.address = Index[i]+1;
        ui_get_pb(PB_GROUP_INFO_I, &GroupInfo);
		if( GroupInfo.status == SECRET_GROUP )
		{
			SET_SECRET_GROUP(GroupInfo.address, TRUE);
		}
        if( i != size-1 )
            GroupInfo.next_index = Index[i+1]+1;
        else
            GroupInfo.next_index = last_group;
        ui_put_pb(PB_GROUP_INFO_I, &GroupInfo);
    }    
    AddList(Handle, last_group, EQS_NA);   // 지정안됨
    SET_GROUP_ORDER(last_group, g_nGroup);

    GroupInfo.address = last_group;
    ui_get_pb(PB_GROUP_INFO_I, &GroupInfo);
    GroupInfo.next_index = 0;
    ui_put_pb(PB_GROUP_INFO_I, &GroupInfo);

    active_list.group_info = Index[0]+1;
    ////ui_put_pb(PB_ACTIVE_LIST_I, &active_list);
    return Handle;
}


UINT8 GetBookList(UINT8 GroupAddress)
{
    INT16 i;
    UINT8 Handle;
    
    Handle = CreateList(MAX_BOOK_NUMBER);

    for( i=0; i<MAX_BOOK_NUMBER; i++)
    {
        if( g_szPersonName[i][0] != 0 )
        {
            if( GroupAddress == 0 )
            {
				if( IS_SECRET_GROUP( g_PersonGroup[i] ) == FALSE )
				{
					AddList(Handle, i+1, EQS_NA);
				}
            }
            else
            {
                if( g_PersonGroup[i] == GroupAddress )
                    //g_PersonMultiGroup[i] == GroupAddress     ) // multi 그룹을 고려
                {
                    AddList(Handle, i+1, EQS_NA);
				}                                
            }
        }
    }
    return Handle;
}

INT16 GetNumOfGroupMember(UINT8 GroupAddress)
{
    INT16 i, nCount;

	nCount = 0;
    for( i=0; i<MAX_BOOK_NUMBER; i++)
    {
        if( g_PersonGroup[i] == GroupAddress )
        {
            if( g_szPersonName[i][0] != BOOK_NOT_USE )
                nCount++;
        }                    
    }
    return nCount;
}

void PhoneBookUpdate(void)     // Save Status of Phone Book
{
    ui_put_pb( PB_ACTIVE_LIST_I, &active_list);
}

BOOL GetGroupInfo(pb_group_info_type* pGroupInfo, UINT8 nPos)  // from 0
{
     UINT8 nIndex;
     if( nPos >= g_nGroup )
         return FALSE;
     for( nIndex=0; nIndex<MAX_GROUP_NUMBER; nIndex++)
     {
         if( nPos == (g_GroupOrder[nIndex]-1) || nPos == -(g_GroupOrder[nIndex]+1) )
         {
             pGroupInfo->address = nIndex+1;
             ui_get_pb(PB_GROUP_INFO_I, pGroupInfo);
             return TRUE;
         }
     }
     return FALSE;
}

BYTE* GetGroupName(UINT8 nIndex)  // 0 th 부터 시작 
{
     UINT8 i;
     if( nIndex >= g_nGroup )
         return NULL;
     for( i=0; i<MAX_GROUP_NUMBER; i++)
     {
         if( g_GroupOrder[i] == (nIndex+1) || g_GroupOrder[i] == -(nIndex+1) )
         {
             return g_szGroupName[i];
         }
     }
     return NULL;
}

BOOL SetGroupInfo(pb_group_info_type* pGroupInfo, UINT8 nPos)     // from 0th 
{
     UINT8 nIndex;
     if( nPos >= g_nGroup )
         return FALSE;
     for( nIndex=0; nIndex<MAX_GROUP_NUMBER; nIndex++)
     {         
         if( nPos == (g_GroupOrder[nIndex]-1) || nPos == -(g_GroupOrder[nIndex]+1) )
         {
             pGroupInfo->address = nIndex+1;
             STRCPY(g_szGroupName[nIndex], pGroupInfo->group_name);
             ui_put_pb(PB_GROUP_INFO_I, pGroupInfo);
             return TRUE;
         }
     }
     return FALSE;
}

BOOL ReadBook(pb_book_info_type* pBookInfo)
{    
    if( ui_get_pb( PB_BOOK_INFO_I, pBookInfo) > 0 )
        return TRUE;
    return FALSE;
}

BOOL WriteBook(pb_book_info_type* pBookInfo)
{
    if( ui_put_pb( PB_BOOK_INFO_I, pBookInfo) > 0 )
        return TRUE;
    return FALSE;
}

BOOL     GetShortDialInfo(UINT8 DialAddress, TelInfo* pTelInfo)
{
    pb_sdial_type           sdial;
    pb_number_info_type     number_info;

    sdial.address = DialAddress;
    if( ui_get_pb(PB_SDIAL_I, &sdial) > 0 )
    {
        number_info.address = sdial.dial_address;
        if( ui_get_pb( PB_NUMBER_INFO_I, &number_info) > 0 )
        {
            pTelInfo->nBookAddress = number_info.book_address;
            pTelInfo->nNumAddress  = number_info.address;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL     SetShortDialInfo(pb_sdial_type* pSDial)
{

    if( ui_put_pb( PB_SDIAL_I, pSDial) > 0 )
	{
		g_SDialNumber[pSDial->address-1] = pSDial->dial_address;
        return TRUE;
	}
    return FALSE;
}


BOOL     GetTelInfo(BYTE* szNumber, TelInfo* pTelInfo)
{

	pb_number_info_type Number[NUMBER_INFO_ARRAY_SIZE];
	INT16 i, j, nLen ;
		
    if( szNumber == NULL )
        return FALSE;
	nLen = STRLEN(szNumber);
    if( nLen == 0 )
        return FALSE;

    for(i=1; i<=MAX_PHONE_NUMBER; i+=NUMBER_INFO_ARRAY_SIZE)
    {
        if( i > active_list.number_info_limit )  // 더이상 저장된 number info 정보가 없다.
            break;

        Read_number(Number,i,NUMBER_INFO_ARRAY_SIZE);
        for(j=0;j<NUMBER_INFO_ARRAY_SIZE;j++)
        {           
            if( (i+j) > active_list.number_info_limit )  // 더이상 유효한 데이터가 아닌 경우 
                break;

			if( KOREAN_STRCMP( Number[j].digits, szNumber) == 0 )
			{										
				if( IS_VALID_BOOK( Number[j].book_address) == TRUE)
				{
					pTelInfo->nBookAddress = Number[j].book_address;
					pTelInfo->nNumAddress  = Number[j].address;
					return TRUE;
				}
			}
		}
	}
	return FALSE;	
}

BOOL     ReadGroup(pb_group_info_type*  pGroupInfo)
{
    if( ui_get_pb( PB_GROUP_INFO_I, pGroupInfo) > 0 )
        return TRUE;
    return FALSE;
}
BOOL     WriteGroup(pb_group_info_type* pGroupInfo)
{
    if( ui_put_pb( PB_GROUP_INFO_I, pGroupInfo) > 0 )
        return TRUE;
    return FALSE;
}

BOOL     ReadSDial(pb_sdial_type*   pSDial)
{
    if( ui_get_pb( PB_SDIAL_I, pSDial) > 0 )
        return TRUE;
    return FALSE;
}

BOOL     WriteSDial(pb_sdial_type*  pSDial)
{
    if( ui_put_pb( PB_SDIAL_I, pSDial) > 0 )
	{
		g_SDialNumber[pSDial->address-1] = pSDial->dial_address;
        return TRUE;
	}
    return FALSE;
}
BOOL     ReadNumber(pb_number_info_type* pNumberInfo)
{
    if( ui_get_pb( PB_NUMBER_INFO_I, pNumberInfo) > 0 )
        return TRUE;
    return FALSE;
}
BOOL     WriteNumber(pb_number_info_type* pNumberInfo)
{
    if( ui_put_pb( PB_NUMBER_INFO_I, pNumberInfo) > 0 )
        return TRUE;
    return FALSE;
}

BOOL     ReadEmail(pb_email_info_type*  pEmailInfo)
{
    if( ui_get_pb(PB_EMAIL_INFO_I, pEmailInfo) > 0 )
        return TRUE;
    return FALSE;
}
BOOL     WriteEmail(pb_email_info_type* pEmailInfo)
{
    if( ui_put_pb(PB_EMAIL_INFO_I, pEmailInfo) > 0 )
        return TRUE;
    return FALSE;
}
BOOL     ReadAnniv(pb_anniv_info_type* pAnnivInfo)
{
    if( ui_get_pb(PB_ANNIV_INFO_I, pAnnivInfo) > 0 )
        return TRUE;
    return FALSE;
}
BOOL     WriteAnniv(pb_anniv_info_type* pAnnivInfo)
{
    if( ui_put_pb(PB_ANNIV_INFO_I, pAnnivInfo) > 0 )
        return TRUE;
    return FALSE;
}

BOOL    GetBookNumberIndex(INT16 nBookAddress, INT16* pnNumber)
{
    UINT8 i, nIndex;
    pb_book_info_type    BookInfo;
    pb_number_info_type  NumberInfo;

    BookInfo.address = nBookAddress;
    if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) <= 0 )
        return FALSE;
    NumberInfo.address = BookInfo.number_address;
    nIndex = 0;
    while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) > 0 && nIndex < PERSONAL_NUMBER_LIMIT )
    {
        pnNumber[nIndex++] = NumberInfo.address ;
        NumberInfo.address = NumberInfo.next_index;
    }
    for(i = nIndex; i< PERSONAL_NUMBER_LIMIT; i++)
        pnNumber[i] = 0;
    return TRUE;
}
            
PBOOK_ERROR_E SaveOldBook(BYTE* szGroup, NV_DIAL_TYPE*  pDial, INT16* nBookAddress)
{
    pb_group_info_type  GroupInfo;
    pb_book_info_type   BookInfo;
    pb_anniv_info_type  AnnivInfo;
    INT8                i,index;
    BYTE                szNumber[A7_NV_MAX_DIAL_DIGITS+1];
    clk_julian_type     julian;

    GroupInfo.address = SearchGroup(szGroup);
    if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
    {
        init_pb_node( PB_BOOK_INFO_I, &BookInfo);

        MakeValidString((BYTE*)pDial->letters, A7_NV_MAX_LTRS );
		if( STRLEN(pDial->letters) )
			STRCPY( BookInfo.name,  pDial->letters );
		else
			STRCPY( BookInfo.name, SKY_GET_TOKEN(TKN_PBOOK_NO_NAME));		
        
        MakeValidString((BYTE*)pDial->memo, A7_NV_MAX_MEMO_SIZ);
		//memo
        STRCPY(BookInfo.szText,(BYTE*)pDial->memo);
                                  
        BookInfo.sync_flag =  pDial->sync_flag;
        BookInfo.mmi_flag  =  pDial->status;
        BookInfo.tstamp    =  pDial->tstamp;
                       
        if( pDial->anniv_contidx > 0 )
        {
            if( active_list.nFreeAnniv == 0 )
                return PBE_FULL_ANNIV;

            init_pb_node(PB_ANNIV_INFO_I, &AnnivInfo);
            AnnivInfo.type = pDial->anniv_contidx;                     
            MakeValidString((BYTE*)pDial->anniv_content,A7_NV_MAX_LTRS);
            STRCPY( AnnivInfo.content, (BYTE*)pDial->anniv_content);
            AnnivInfo.content[A7_NV_MAX_LTRS] = 0;
            AnnivInfo.day    = pDial->anniv_day;
            AnnivInfo.month  = pDial->anniv_month;
            AnnivInfo.fLunar = pDial->anniv_lunar;
			AnnivInfo.year   = 0;
         
        }

        index = 0;
        for( i=3; i>=0; i--)
        {
            if( pDial->multi_num_digits[i] )
            {
                index++;                
            }
        }
        if( active_list.nFreeNumber < index )
            return PBE_FULL_NUMBER;

		// email			
		MakeValidString((BYTE*)pDial->email,A7_NV_MAX_DIAL_DIGITS);			
		if( STRLEN(pDial->email) > 0 )
		{
			if( active_list.nFreeEMail  == 0 )			    
				return PBE_FULL_EMAIL;
		}

        if( AddBook(&GroupInfo, &BookInfo) <= 0 )
		{
            return PBE_FULL_BOOK;        
		}
			
		if( STRLEN(pDial->email) > 0 )
		{
			AddEmail(&BookInfo, (BYTE*) pDial->email);
		}
		
        if( pDial->anniv_contidx > 0 )
		{
            AddAnnivInfo(&BookInfo, &AnnivInfo);
		}

        for( i=3; i>=0; i--)
        {
            if( pDial->multi_num_digits[i] )
            {                
                STRNCPY(szNumber, pDial->multi_digits[i], pDial->multi_num_digits[i]);
                szNumber[pDial->multi_num_digits[i]] = 0;
                //MakeValidString( pDial->multi_digits[i], A7_NV_MAX_DIAL_DIGITS);
                ReverseAddNumber(&BookInfo, szNumber, CELL + i);
            }
        }

        ui_put_pb( PB_BOOK_INFO_I,  &BookInfo);
		PhoneBookUpdate();
        //ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
        *nBookAddress = BookInfo.address;
        return PBE_NO_ERROR;
    }   
    return PBE_INVALID_GROUP;
}

PBOOK_ERROR_E WriteOldBook(NV_DIAL_TYPE*  pDial)
{
    pb_group_info_type  GroupInfo;
    pb_book_info_type   BookInfo;
    pb_anniv_info_type  AnnivInfo;
    INT8                i,index;
    BYTE                szNumber[A7_NV_MAX_DIAL_DIGITS+1];
    clk_julian_type     julian;

    GroupInfo.address = pDial->group;
    if( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
    {
        init_pb_node( PB_BOOK_INFO_I, &BookInfo);

        MakeValidString((BYTE*)pDial->letters, A7_NV_MAX_LTRS );
		if( STRLEN(pDial->letters) )
			STRCPY( BookInfo.name,  pDial->letters );
		else
			STRCPY( BookInfo.name, SKY_GET_TOKEN(TKN_PBOOK_NO_NAME));

		MakeValidString((BYTE*)pDial->memo, A7_NV_MAX_MEMO_SIZ);
        //memo
        STRCPY(BookInfo.szText,(BYTE*)pDial->memo);
                                  
        BookInfo.sync_flag =  pDial->sync_flag;
        BookInfo.mmi_flag  =  pDial->status;
        BookInfo.tstamp    =  pDial->tstamp;
                       
        if( pDial->anniv_contidx > 0 )
        {
            if( active_list.nFreeAnniv == 0 )
                return PBE_FULL_ANNIV;

            init_pb_node(PB_ANNIV_INFO_I, &AnnivInfo);
            AnnivInfo.type = pDial->anniv_contidx;                     
            MakeValidString((BYTE*)pDial->anniv_content,A7_NV_MAX_LTRS);
            STRCPY( AnnivInfo.content, (BYTE*)pDial->anniv_content);
            AnnivInfo.content[A7_NV_MAX_LTRS] = 0;
            AnnivInfo.day    = pDial->anniv_day;
            AnnivInfo.month  = pDial->anniv_month;
            AnnivInfo.fLunar = pDial->anniv_lunar;
			AnnivInfo.year   = 0;          
        }
        index = 0;
        for( i=3; i>=0; i--)
        {
            if( pDial->multi_num_digits[i] )
            {
                index++;                
            }
        }
        if( active_list.nFreeNumber < index )
            return PBE_FULL_NUMBER;

		// email
		MakeValidString((BYTE*)pDial->email,A7_NV_MAX_DIAL_DIGITS);
				
		if( STRLEN(pDial->email) > 0 )
		{
			if( active_list.nFreeEMail  == 0 )
				  return PBE_FULL_EMAIL;
		}


        if( IsValid(PB_BOOK_INFO_I, pDial->address ) == FALSE)
        {
            if( AddBook(&GroupInfo, &BookInfo) <= 0 )                  
                return PBE_FULL_BOOK;
        }
        else
        {
            if( g_szPersonName[pDial->address-1][0] == BOOK_NOT_USE  )
            {
                if( AddBook(&GroupInfo, &BookInfo) <= 0 )                  
                    return PBE_FULL_BOOK;               
            }          
        }

		if( STRLEN(pDial->email) > 0 )
		{
			AddEmail(&BookInfo, (BYTE*) pDial->email);
		}

        if( pDial->anniv_contidx > 0 )
		{
            AddAnnivInfo(&BookInfo, &AnnivInfo);
		}

        for( i=3; i>=0; i--)
        {
            if( pDial->multi_num_digits[i] )
            {                
                STRNCPY(szNumber, pDial->multi_digits[i], pDial->multi_num_digits[i]);
                szNumber[pDial->multi_num_digits[i]] = 0;
                //MakeValidString( pDial->multi_digits[i], A7_NV_MAX_DIAL_DIGITS);
                ReverseAddNumber(&BookInfo, szNumber, CELL + i);
            }
        }

        ui_put_pb( PB_BOOK_INFO_I,  &BookInfo);
		PhoneBookUpdate();
        //ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
        return PBE_NO_ERROR;
    }   
    return PBE_INVALID_GROUP;
}


BOOL     MakeOldBook(INT16 BookAddress, NV_DIAL_TYPE* pDial)
{
    pb_book_info_type   BookInfo;
    pb_number_info_type NumberInfo;
    pb_email_info_type  EmailInfo;
    pb_anniv_info_type  AnnivInfo;
    UINT8  i, index, nCount;


    BookInfo.address = BookAddress;
    if( ui_get_pb( PB_BOOK_INFO_I, &BookInfo) > 0 )
    {
        pDial->exist     = 1;
        pDial->status    = BookInfo.mmi_flag;
        pDial->sync_flag = BookInfo.sync_flag;
        pDial->tstamp    = BookInfo.tstamp;

        pDial->group = BookInfo.group_address;     
         
        //memo
        STRCPY( (BYTE*)pDial->memo  , BookInfo.szText);
		MakeOldString((BYTE*)pDial->memo, A7_NV_MAX_MEMO_SIZ);
        
		
        STRCPY( (BYTE*)pDial->letters, BookInfo.name);
        MakeOldString((BYTE*)pDial->letters,A7_NV_MAX_LTRS);

		//email
        EmailInfo.address = BookInfo.email_address;
        if( ui_get_pb( PB_EMAIL_INFO_I, &EmailInfo) > 0 )
        {
            STRCPY((BYTE*)pDial->email, EmailInfo.szMailID);
        }
        else
        {
            pDial->email[0] = 0;
        }
		MakeOldString((BYTE*)pDial->email, A7_NV_MAX_DIAL_DIGITS);       

        for( i= 0; i<4; i++)
        {
            pDial->multi_num_digits[i] = 0;
            pDial->multi_digits[i][0]  = 0;
            MakeOldString((BYTE*)pDial->multi_digits[i], A7_NV_MAX_DIAL_DIGITS);
        }
        
        NumberInfo.address = BookInfo.number_address;
		nCount = 0;
        while( ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) > 0 && nCount < PERSONAL_NUMBER_LIMIT)
        {
			nCount++;
            index = NumberInfo.number_type - CELL;   // CELL is 1
            pDial->multi_num_digits[index] =  STRLEN(NumberInfo.digits);
            STRCPY((BYTE*) pDial->multi_digits[index], NumberInfo.digits);
            MakeOldString((BYTE*)pDial->multi_digits[index], A7_NV_MAX_DIAL_DIGITS);
            NumberInfo.address = NumberInfo.next_index;
        }

        AnnivInfo.address = BookInfo.anniv_address;
        if( ui_get_pb( PB_ANNIV_INFO_I, &AnnivInfo) > 0 )
        {
            pDial->anniv_contidx = AnnivInfo.type;
            pDial->anniv_day     = AnnivInfo.day;
            pDial->anniv_month   = AnnivInfo.month;
            pDial->anniv_lunar   = AnnivInfo.fLunar == TRUE ? 1 : 0 ;
            STRCPY( (BYTE*) pDial->anniv_content, AnnivInfo.content);
        }
        else
        {
            pDial->anniv_contidx = 0; // 없음
            pDial->anniv_content[0] = 0;
        }  
        MakeOldString((BYTE*) pDial->anniv_content, A7_NV_MAX_LTRS);
        return TRUE;
    }
    return FALSE;               
}

BOOL  DeltePictureInBook(DWORD dwPictureIndex)
{

    INT16   nIndex;
    pb_book_info_type BookInfo;

    for(nIndex=1; nIndex<=MAX_BOOK_NUMBER; nIndex++)
    {
        if( IS_VALID_BOOK(nIndex) )
        {
            BookInfo.address = nIndex;
            ui_get_pb( PB_BOOK_INFO_I, &BookInfo);
            if( BookInfo.dwImageIndex == dwPictureIndex )
            {
                BookInfo.dwImageIndex = 0;
                ui_put_pb( PB_BOOK_INFO_I, &BookInfo);
            }
        }
    }
    return TRUE;
}


UINT8 GetEmailList(void)
{
    INT16 i;
    UINT8 BookList, GroupList;
    pb_book_info_type  BookInfo;

    BookList  = CreateList(MAX_BOOK_NUMBER);    


    for( i =1; i<= MAX_BOOK_NUMBER; i++)
    {
        if( IS_VALID_BOOK(i) )
        {
            BookInfo.address = i;
            ui_get_pb( PB_BOOK_INFO_I, &BookInfo);
            //if( STRLEN(BookInfo.email) )
			if( IsValid( PB_EMAIL_INFO_I, BookInfo.email_address) )
            {
                AddList(BookList, i, BookInfo.group_address);
            }
        }
    }

    if( GetListSize(BookList) > 0 )
    {
        GroupList = CreateList(MAX_GROUP_NUMBER+1);
        for(i=1; i<= MAX_GROUP_NUMBER; i++)
        {
            if( IS_VALID_GROUP(i) )
            {
                if( FindListType(BookList, i) >= 0)
                {
                    AddList(GroupList, i, BookList);
                }
            }
        }
    }
    else
    {
        DeleteList(BookList);
        return 0;
    }

    return GroupList;      
}


UINT8 GetBookListInGroup(UINT8 nGroup)
{
    INT16 i, nSize;
    UINT8 ListHandle;

    if( IS_VALID_GROUP(nGroup) == FALSE )
        return 0;

    nSize = 0;

    for( i=0; i<MAX_BOOK_NUMBER; i++)
    {
        if( g_szPersonName[i][0] != 0 )
        {
            if( g_PersonGroup[i]  == nGroup )
            {
                nSize++;
            }
        }
    }   

    ListHandle = CreateList(nSize);

    for( i=0; i<MAX_BOOK_NUMBER; i++)
    {
        if( g_szPersonName[i][0] != 0 )
        {
            if( g_PersonGroup[i]  == nGroup )
            {              
                AddList(ListHandle, i+1, EQS_NA);
            }
        }
    }   

    return ListHandle;
}

UINT8  GetBirthStar(UINT8 nMonth, UINT8 nDay)
{
	switch( nMonth )
	{
	case  1:
		if( nDay < 20 )
			return 11; //염소자리 
		else
			return  0; //물병자리 
	case  2:
		if( nDay < 19 )
			return  0; //불병자리
		else
			return 1;  //물고기자리			
	case  3:
		if( nDay < 21 )
			return 1;  //물고기자리
		else
			return 2;  //양자리 
	case  4:
		if( nDay < 21 )
			return 2; //양자리
		else
			return 3; //황소자리
	case  5:
		if( nDay < 21 )
			return 3; //황소자리
	    else
			return 4; //쌍둥이자리 

	case  6:
		if( nDay < 22 )
			return 4; //쌍둥이 자리 
		else
			return 5; //게자리
	case  7:
		if( nDay < 23 )
			return 5; //게자리
		else
			return 6; //사자자리
	case  8:
		if( nDay < 23 )
			return 6; //사자자리 
		else
			return 7; //처녀자리 
	case  9:
		if( nDay < 23 )
			return 7; //처녀자리
		else
			return 8; //천칭자리

	case 10:
		if( nDay < 22 )
			return 8; //천칭자리
		else
			return 9; //전갈자리
	case 11:
		if( nDay < 22 )
			return 9; //전갈자리 
		else
			return 10;//사수자리
	case 12:
		if( nDay < 22 )
			return 10;//사수자리
		else
			return 11;//염소자리
	default:
		return 0;		
	}
}

INT16    FindDuplicatedBook(INT8 nGroupAddress, INT16 nBookAddress, BYTE* szName)
{
    UINT8  hList;
    INT16  nIndex, i;


    if( szName == NULL )
        return 0;
    if( STRLEN(szName) == 0 )
        return 0;

    if( nGroupAddress == 0)
        return 0;
    else
		;

	hList = SearchName(szName, BSM_NAME_FULL_MATCH);
	if( hList == 0 )
		return 0;
	
    if( nBookAddress == 0 )
	{		
        for(i=0; i< GetListSize(hList); i++)
		{
			nIndex = GetListValue(hList,i);
			if( g_PersonGroup[nIndex-1] == nGroupAddress )
			{
				DeleteList(hList);
				return nIndex;
			}
		}
	}
	else
	{	
		for(i=0; i< GetListSize(hList); i++)
		{
			nIndex = GetListValue(hList,i);
			if( g_PersonGroup[nIndex-1] == nGroupAddress &&\
				nIndex != nBookAddress )
			{
				DeleteList(hList);
				return nIndex;
			}
		}
	}
	DeleteList(hList);
	return 0;
}



