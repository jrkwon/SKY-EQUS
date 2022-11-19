/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     ska_bapi.c
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
**
**/ 

#include "ska.h"
#include "string.h"

                          
LOCAL BOOL   sDeleteAnnivLink(pb_anniv_info_type*   pAnnivInfo);
LOCAL BOOL   sDeleteBook(INT16 address);


LOCAL BOOL sDeleteBook(INT16 address)
{ 
	pb_book_info_type book_info;
	pb_number_info_type number_info;
	pb_anniv_info_type anniv_info;
	pb_email_info_type email_info;
    INT16 i, nCount;
    
    book_info.address = address;
    if( ui_get_pb( PB_BOOK_INFO_I, &book_info) <= 0 )
	{
        return FALSE;
	}
    number_info.address = book_info.number_address;
	nCount = 0;
    while( ui_get_pb( PB_NUMBER_INFO_I, &number_info) > 0 && nCount < PERSONAL_NUMBER_LIMIT )
    {
		nCount++;        
        DeleteNumberLink(&number_info);
        free_pb_node( PB_NUMBER_INFO_I , number_info.address );
        number_info.address = number_info.next_index;
    }
    anniv_info.address = book_info.anniv_address;
	nCount=0;
    while( ui_get_pb( PB_ANNIV_INFO_I, &anniv_info) > 0 && nCount < PERSONAL_ANNIV_LIMIT )
    {
        nCount++;
		sDeleteAnnivLink(&anniv_info);
        free_pb_node( PB_ANNIV_INFO_I , anniv_info.address );
        anniv_info.address = anniv_info.next_index;
    }
    email_info.address = book_info.email_address;
	nCount=0;
    while( ui_get_pb( PB_EMAIL_INFO_I, &email_info) > 0 && nCount < PERSONAL_EMAIL_LIMIT)
    {
        nCount++;
        free_pb_node( PB_EMAIL_INFO_I , email_info.address );
        email_info.address = email_info.next_index;
    }
    
    free_pb_node( PB_BOOK_INFO_I, book_info.address );
    g_PersonGroup[book_info.address-1]      = BOOK_NOT_USE;
    g_PersonMultiGroup[book_info.address-1] = BOOK_NOT_USE;
    for(i=0; i< PERSONAL_NUMBER_LIMIT; i++)
	{
        g_PersonNumberType[book_info.address-1][i] = BOOK_NOT_USE;
	}
    
	return TRUE;

}

BOOL DeleteBookInfo(INT16 nBookAddress)
{
    return sDeleteBook(nBookAddress);
}

BOOL DeleteGroupBook(pb_group_info_type* pGroupInfo)
{
    INT16 nIndex;

    if( IsValid(PB_GROUP_INFO_I, pGroupInfo->address) == FALSE )
	{
        return FALSE;
	}
    for(nIndex = 0; nIndex < MAX_BOOK_NUMBER; nIndex++)
    {
        if( g_PersonGroup[nIndex] == pGroupInfo->address )
        {
            sDeleteBook(nIndex+1); 
			SKY_KickDog();
        }
    }
    pGroupInfo->first_book_address = 0;
    return TRUE;
}
// hEditNumber는 전화부 수정시 수정된 전화번호의 주소가 있는 리스트를 나타낸다.
// 사람의 모든 데이터를 삭제할려고 하면 hEditNumber 값으로 0를 준다.
BOOL DeleteBook(pb_group_info_type* pGroupInfo, pb_book_info_type* pBookInfo, BOOL fDeleteNumber)
{
	pb_book_info_type  previous_book;
	pb_number_info_type number_info;
	pb_anniv_info_type anniv_info;
	pb_email_info_type email_info;
    INT16 j, nPos, nCount;
    
    if( fDeleteNumber == TRUE )
    {
        number_info.address = pBookInfo->number_address;
		nCount = 0;
        while( ui_get_pb(PB_NUMBER_INFO_I, &number_info) > 0 && nCount < PERSONAL_NUMBER_LIMIT )
        {       
			nCount++;
            DeleteNumberLink(&number_info);           
            free_pb_node( PB_NUMBER_INFO_I , number_info.address);
            number_info.address = number_info.next_index;            
        }
    }
    
    anniv_info.address = pBookInfo->anniv_address;
	nCount = 0;
    while( ui_get_pb( PB_ANNIV_INFO_I, &anniv_info) > 0 && nCount < PERSONAL_ANNIV_LIMIT )
    {
		nCount++;        
        sDeleteAnnivLink(&anniv_info);
        free_pb_node( PB_ANNIV_INFO_I , anniv_info.address );
        anniv_info.address = anniv_info.next_index;
    }
    email_info.address = pBookInfo->email_address;
	nCount = 0;
    while( ui_get_pb( PB_EMAIL_INFO_I, &email_info) > 0 && nCount < PERSONAL_EMAIL_LIMIT )
    {
		nCount++;
        free_pb_node( PB_EMAIL_INFO_I , email_info.address );
        email_info.address = email_info.next_index;
    }
    
	pGroupInfo->wMember--;    //01.04.10
    free_pb_node( PB_BOOK_INFO_I, pBookInfo->address );
    g_PersonGroup[pBookInfo->address-1]      = BOOK_NOT_USE;
    g_PersonMultiGroup[pBookInfo->address-1] = BOOK_NOT_USE;
    for(j=0; j< PERSONAL_NUMBER_LIMIT; j++)
	{
        g_PersonNumberType[pBookInfo->address-1][j] = BOOK_NOT_USE;
	}
    
	return TRUE;
}

BOOL  DeleteNumberLink(pb_number_info_type* pNumberInfo)
{
	UINT8  i;
	pb_sdial_type  sDial;

	for(i=1; i<= MAX_SDIAL_NUMBER; i++)
	{
		if( g_SDialNumber[i-1] == pNumberInfo->address )
		{
			sDial.address = i;
			sDial.dial_address = BOOK_NOT_USE; //0
			g_SDialNumber[i-1] = BOOK_NOT_USE;
			ui_put_pb( PB_SDIAL_I, &sDial);			
		}
	}

	return TRUE;
}

BOOL  DeleteNumberInfo(pb_number_info_type* pNumberInfo)
{ 
    DeleteNumberLink( pNumberInfo );
    free_pb_node( PB_NUMBER_INFO_I, pNumberInfo->address );	
	return TRUE;
}

BOOL  DeleteNumber(pb_book_info_type * pBookInfo, INT16 nNumberAddress )
{
    pb_number_info_type  number_info, delete_info, modify_info;
    INT8  nTypePos = 0;
    INT16 i = 0;

   
    delete_info.address =  nNumberAddress;
    if( ui_get_pb( PB_NUMBER_INFO_I, &delete_info) == 0 )
	{
        return FALSE;
	}

    if( pBookInfo->number_address == nNumberAddress )
    {        
        if( IsValid( PB_NUMBER_INFO_I, delete_info.next_index ) == TRUE )
        {
             pBookInfo->number_address = delete_info.next_index;             
        }
        else
        {
            pBookInfo->number_address = 0;
        }
        ui_put_pb( PB_BOOK_INFO_I, pBookInfo );
    }
    else
    {
        number_info.address = pBookInfo->number_address;
        while(  ui_get_pb( PB_NUMBER_INFO_I, &number_info ) > 0 && nTypePos < PERSONAL_NUMBER_LIMIT )
        {
			nTypePos++;
            if( number_info.next_index == nNumberAddress )
                break;
            number_info.address  = number_info.next_index;            
        }
        if( number_info.next_index == nNumberAddress )
        {
            number_info.next_index = delete_info.next_index;
            ui_put_pb( PB_NUMBER_INFO_I, &number_info);
        }
        else
        {
            return FALSE;
        }
    }

	if( nTypePos == (PERSONAL_NUMBER_LIMIT-1) )
	{
		g_PersonNumberType[pBookInfo->address-1][nTypePos] = BOOK_NOT_USE;
	}
	else
	{
		for( i=nTypePos; i< PERSONAL_NUMBER_LIMIT-1; i++)
		{
			g_PersonNumberType[pBookInfo->address-1][i] = g_PersonNumberType[pBookInfo->address-1][i+1];
		}
		g_PersonNumberType[pBookInfo->address-1][PERSONAL_NUMBER_LIMIT-1] = BOOK_NOT_USE;

	}
	


#if 0
    modify_info.address = delete_info.slink;
    i = SumOfNumber(delete_info.digits)%10;
    if( active_list.number_search[i] == delete_info.address )
    {
        active_list.number_search[i] = delete_info.next_index;
        ////ui_put_pb( PB_ACTIVE_LIST_I, &active_list);
        free_pb_node( PB_NUMBER_INFO_I, delete_info.address );
        return TRUE;
    }
#endif

    DeleteNumberLink(&delete_info);
    free_pb_node( PB_NUMBER_INFO_I, delete_info.address );
    return TRUE;   
}

LOCAL BOOL   sDeleteAnnivLink(pb_anniv_info_type*   pAnnivInfo)
{

    pb_anniv_info_type  PreviousAnniv;
    INT16  nMonth, nCount;
           
    nMonth = pAnnivInfo->month-1;
    
    if( active_list.anniv_info[nMonth] == pAnnivInfo->address )
    {
        active_list.anniv_info[nMonth] =  pAnnivInfo->link;  
        return TRUE;
    }
    PreviousAnniv.address = active_list.anniv_info[nMonth];
	nCount = 0;
    while( ui_get_pb( PB_ANNIV_INFO_I, &PreviousAnniv ) > 0 && nCount < MAX_ANNIV_NUMBER )
    {
		nCount++;
        if( PreviousAnniv.link == pAnnivInfo->address )
        {
            PreviousAnniv.link =  pAnnivInfo->link;
            ui_put_pb( PB_ANNIV_INFO_I, &PreviousAnniv);
            return TRUE;
        }
        PreviousAnniv.address = PreviousAnniv.link;
    }
    return FALSE;
}

BOOL   DeleteAnnivInfo(pb_anniv_info_type* pAnnivInfo)
{
			
    sDeleteAnnivLink(pAnnivInfo); 
    free_pb_node( PB_ANNIV_INFO_I, pAnnivInfo->address );
	return TRUE;
}

BOOL  DeleteAnniv(pb_book_info_type * pBookInfo, INT16 addr )
{
    pb_anniv_info_type   anniv_info, delete_info;
	INT16 nCount;
   
    delete_info.address = addr;
    if( ui_get_pb( PB_ANNIV_INFO_I, &delete_info) <= 0 )
	{
        return FALSE;
	}

    if( pBookInfo->anniv_address == addr )
    {        
        pBookInfo->anniv_address = delete_info.next_index;
        ui_put_pb( PB_BOOK_INFO_I, pBookInfo );       
    }
    else
    {
        anniv_info.address = pBookInfo->anniv_address;
		nCount = 0;
        while( ui_get_pb( PB_ANNIV_INFO_I, &anniv_info ) > 0 && nCount < PERSONAL_ANNIV_LIMIT )
        {
			nCount++;
            if( anniv_info.next_index == addr )
                break;
            anniv_info.address  = anniv_info.next_index;
        }
        if( anniv_info.next_index == addr )
        {
            anniv_info.next_index = delete_info.next_index;
            ui_put_pb( PB_ANNIV_INFO_I, &anniv_info);
        }
        else
        {
            return FALSE;
        }
    }

    sDeleteAnnivLink(&delete_info); 
    free_pb_node( PB_ANNIV_INFO_I, delete_info.address );    
    return TRUE;
}

BOOL  DeleteEmail(pb_book_info_type * pBookInfo, INT16 addr )
{
    pb_email_info_type    email_info, delete_info;
	INT8  nCount;
   
    delete_info.address = addr;
    if( ui_get_pb( PB_EMAIL_INFO_I, &delete_info) == 0 )
	{
        return FALSE;
	}

    if( pBookInfo->email_address == addr )
    {
		pBookInfo->email_address = delete_info.next_index; 
        ui_put_pb( PB_BOOK_INFO_I, pBookInfo );
    }
    else
    {
		nCount = 0;
        email_info.address = pBookInfo->email_address;
        while(  ui_get_pb( PB_EMAIL_INFO_I, &email_info ) > 0  && nCount < PERSONAL_EMAIL_LIMIT )
        {
			nCount++;
            if( email_info.next_index == addr )
                break;
            email_info.address  = email_info.next_index;
        }
        if( email_info.next_index == addr && nCount <= PERSONAL_EMAIL_LIMIT )
        {
            email_info.next_index = delete_info.next_index;
            ui_put_pb( PB_EMAIL_INFO_I, &email_info);
        }
        else
        {
            return FALSE;
        }
    }
    free_pb_node( PB_EMAIL_INFO_I, delete_info.address );
    return TRUE;
}

LOCAL UINT8 sInsertGroup(BYTE* Name, UINT8 nPos, BOOL fSecret, UINT8 GroupHandle)
{
    UINT8 i, nOrder;
    pb_group_info_type group_info, previous_info;

    init_pb_node(PB_GROUP_INFO_I, &group_info);
    group_info.address = (UINT8) alloc_pb_node( PB_GROUP_INFO_I);

	if( group_info.address == 0 )
	{
		return 0;  // group file is  full
	}

    STRCPY(group_info.group_name, Name);   
    if( fSecret == TRUE )
    {
        group_info.status = SECRET_GROUP;
    }
    else
    {
        group_info.status = NON_SECRET_GROUP;
    }
    
    if( nPos > 0 )
    {
        previous_info.address = (UINT8)GetListValue(GroupHandle,nPos-1);
        ui_get_pb(PB_GROUP_INFO_I, &previous_info);        
        group_info.next_index = previous_info.next_index;  
        previous_info.next_index = group_info.address;
        ui_put_pb( PB_GROUP_INFO_I, &previous_info);
    }
    else
    {                                 
        group_info.next_index  = active_list.group_info;
        active_list.group_info = group_info.address;
    }

    ui_put_pb(PB_GROUP_INFO_I, &group_info);               
    STRCPY( g_szGroupName[group_info.address-1], Name);
    for(i=0; i<MAX_GROUP_NUMBER; i++)
    {
        nOrder = GET_GROUP_ORDER(i+1);
        if( nOrder >= nPos+1 )
		{
			if( IS_SECRET_GROUP(i+1) )
			{
				SET_GROUP_ORDER(i+1, -(nOrder+1));
			}
			else
			{
				SET_GROUP_ORDER(i+1, nOrder+1);
			}
		}
    }           
	if( fSecret == TRUE )
	{
		SET_GROUP_ORDER( group_info.address, -(nPos+1) );
	}
	else
	{
		SET_GROUP_ORDER( group_info.address, nPos+1 );
	}
    //g_GroupOrder[group_info.address-1] = nPos+1;
    g_nGroup++;
   
    return group_info.address;

}

UINT8 InsertGroup(BYTE* Name, UINT8 nPos, BOOL fSecret)   // 그룹 정렬을 하지 않고 nPos 위치에 새로 그룹을 삽입한다.
{
    UINT8 Handle;
    UINT8 GroupAddress;

	if( Name == NULL || Name[0] == 0 )
	{
		return 0;
	}

    if( SearchGroup(Name) != 0 )
	{
        return 0;  // 이미 같은 그룹이 존재 
	}
    Handle          = GetGroupList();
    GroupAddress    = sInsertGroup(Name, nPos, fSecret, Handle);
    DeleteList(Handle);

    return GroupAddress;
}

UINT8 AddGroup(BYTE* Name, BOOL fSecret)  //2001.01.08    // 그룹을 정렬해서 알맞은 위치에 삽입
{
    UINT8  Handle, nSize, nIndex;
    INT    cmp;
    UINT8  GroupAddress;
   
	if( Name == NULL || Name[0] == 0 )
	{
		return 0;
	}

    //Handle = SortGroup();
	Handle = GetGroupList();
    nSize  = (UINT8)GetListSize(Handle);

    if( nSize >= MAX_GROUP_NUMBER )
    {
        DeleteList(Handle);
        return 0;      // 메모리가 꽉찼음.
    }

	if( strcmp(g_szGroupName[GetListValue(Handle,nSize-1)-1], (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6)) == 0 )
	{
		nSize--;
	} 	

    for( nIndex=0; nIndex<(nSize-1); nIndex++)
    {
        cmp = KOREAN_STRCMP( g_szGroupName[GetListValue(Handle,nIndex)-1], (BYTE*)Name );
        if( cmp == 0 )
        {
            DeleteList(Handle);
            return 0;   // 이미 그룹이 존재
        }
        else if( cmp > 0 )
            break;
        else ;           

    }
    GroupAddress = sInsertGroup(Name, nIndex, fSecret, Handle);
    DeleteList(Handle);

    return GroupAddress;
}

BOOL   DeleteSDial(UINT8 nDial)
{
    pb_sdial_type  sdial;

    sdial.address = nDial;
    if( ui_get_pb( PB_SDIAL_I, &sdial) > 0 )
    {
        sdial.dial_address = 0;
        ui_put_pb( PB_SDIAL_I, &sdial);
		g_SDialNumber[sdial.address-1] = 0; //BOOK_NOT_USE
        return TRUE;
    }
    return FALSE;
}


BOOL DeleteGroup(UINT8 nGroupAddress, BOOL fContent)  //2001.01.08
{
    UINT8  nIndex, nSize, nOrder;
    INT16  i;
    pb_group_info_type GroupInfo, PreviousGroup;


    if( STRLEN( g_szGroupName[nGroupAddress-1] ) == 0 )
		return FALSE;

    GroupInfo.address = nGroupAddress;
    ui_get_pb( PB_GROUP_INFO_I, &GroupInfo);

    // delete group member info
    if( fContent == TRUE )
    {                
        DeleteGroupBook(&GroupInfo);
    }
    else
    {
        nIndex = SearchGroup( SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) );
        if( nIndex == 0 )
        {
            //nIndex = AddGroup(SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6), FALSE);
			nIndex = InsertGroup(SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6), g_nGroup, FALSE);
        }
        MoveGroup(nGroupAddress,nIndex);        
    }

	nOrder = GET_GROUP_ORDER(nGroupAddress);  // group order

	// delete group info
    if( nOrder == 1 )
    {
        active_list.group_info = GroupInfo.next_index;
    }
    else
    {
		for( nIndex = 1; nIndex <= MAX_GROUP_NUMBER; nIndex++ )
		{
			if( GET_GROUP_ORDER(nIndex) == (nOrder-1) )
				break;
		}
		if( nIndex <= MAX_GROUP_NUMBER )
		{
			PreviousGroup.address = nIndex;
			ui_get_pb(PB_GROUP_INFO_I, &PreviousGroup);
			PreviousGroup.next_index = GroupInfo.next_index;
			ui_put_pb( PB_GROUP_INFO_I, &PreviousGroup);
		}
		else
		{
			return FALSE;
		}
    }
    g_szGroupName[nGroupAddress-1][0] = BOOK_NOT_USE;
    SET_GROUP_ORDER(nGroupAddress, BOOK_NOT_USE);
    
    for(i=1; i<=MAX_GROUP_NUMBER; i++)
    {
        nIndex = GET_GROUP_ORDER(i);

        if( nIndex > nOrder )
        {
			if( IS_SECRET_GROUP(i) )
			{
				SET_GROUP_ORDER(i, -(nIndex-1) );
			}
			else
			{
				SET_GROUP_ORDER(i, nIndex-1);
			}
		}
    }
    free_pb_node(PB_GROUP_INFO_I, nGroupAddress);
    g_nGroup--;
    return TRUE;
}

// 변경된 그룹정보는 저장하지 않는다.
INT16 MoveMultiBook(pb_group_info_type* pGroupInfo, INT16 nBookAddress, UINT8 SortedBookHandle)
{
    INT16  nIndex;
    INT    cmp;
    pb_book_info_type book_info, previous_book;

    for( nIndex=0; nIndex< GetListSize(SortedBookHandle); nIndex++)
    {
        cmp =  KOREAN_STRCMP(g_szPersonName[GetListValue(SortedBookHandle,nIndex)-1],\
			                 g_szPersonName[nBookAddress-1] );
        if( cmp == 0 )
		{
            return -(GetListValue(SortedBookHandle,nIndex));
		}
        else if( cmp > 0 )
		{
            break;
		}
        else 
		{
			;
		}
    }
    book_info.address = nBookAddress;
    ui_get_pb( PB_BOOK_INFO_I, &book_info);
       
    pGroupInfo->wMember++;

    if( nIndex == 0 )
    {
        book_info.next_index = pGroupInfo->first_book_address;
        pGroupInfo->first_book_address = book_info.address;
        ui_put_pb( PB_GROUP_INFO_I, pGroupInfo);
    }
    else
    {
        previous_book.address = GetListValue(SortedBookHandle,nIndex-1);
        ui_get_pb( PB_BOOK_INFO_I, &previous_book);
        book_info.next_index = previous_book.next_index;
        previous_book.next_index = book_info.address;
        ui_put_pb(PB_BOOK_INFO_I, &previous_book);
    }

    g_PersonGroup[nBookAddress-1] = pGroupInfo->address;
    if( g_PersonMultiGroup[nBookAddress-1] == pGroupInfo->address )
    {        
        g_PersonMultiGroup[nBookAddress-1]  = BOOK_NOT_USE;
        book_info.multi_group = BOOK_NOT_USE;
    }
    book_info.group_address = pGroupInfo->address;
    ui_put_pb( PB_BOOK_INFO_I, &book_info);
    InsertList(SortedBookHandle, nIndex, nBookAddress, EQS_NA);

    return nBookAddress;
}


INT16 MoveBook(pb_group_info_type* pGroupInfo, INT16 nBookAddress)  // 변경된 그룹정보는 저장하지 않는다.
{
    UINT8  Handle;    
    INT16  nResultAddress;
    Handle = GetBookList(pGroupInfo->address);
    SortBookName(Handle);
    nResultAddress = MoveMultiBook(pGroupInfo, nBookAddress, Handle);
    DeleteList(Handle);
    return nResultAddress;
}

INT16 AddMultiBook(pb_group_info_type* pGroupInfo, pb_book_info_type* pBookInfo, UINT8 SortedBookHandle)
{
    INT16  nIndex, j;
    INT    cmp;
    pb_book_info_type previous_book;

    for( nIndex=0; nIndex< GetListSize(SortedBookHandle); nIndex++)
    {
        cmp =  KOREAN_STRCMP(g_szPersonName[GetListValue(SortedBookHandle,nIndex)-1], pBookInfo->name );
        if( cmp == 0 )
        {
            if( KOREAN_STRCMP( pBookInfo->name, SKY_GET_TOKEN(TKN_PBOOK_NO_NAME)) )
                return -(GetListValue(SortedBookHandle,nIndex));
        }
        else if( cmp > 0 )
		{
            break;
		}
        else
		{
			;
		}
    }    
   
    pBookInfo->address = alloc_pb_node( PB_BOOK_INFO_I );
    if( IsValid( PB_BOOK_INFO_I, pBookInfo->address ) == FALSE )
	{
        return 0;             //메모리 부족 
	}
    
    pBookInfo->group_address = pGroupInfo->address;
    pGroupInfo->wMember++;

    if( nIndex == 0 )
    {
        pBookInfo->next_index = pGroupInfo->first_book_address;
        pGroupInfo->first_book_address = pBookInfo->address;        
    }
    else
    {
        previous_book.address = GetListValue(SortedBookHandle,nIndex-1);
        ui_get_pb( PB_BOOK_INFO_I, &previous_book);
        pBookInfo->next_index = previous_book.next_index;
        previous_book.next_index = pBookInfo->address;
        ui_put_pb(PB_BOOK_INFO_I, &previous_book);
    }
    ui_put_pb( PB_GROUP_INFO_I, pGroupInfo);

    STRCPY( g_szPersonName[pBookInfo->address-1], pBookInfo->name );
    g_PersonGroup[pBookInfo->address-1]         = pGroupInfo->address;
    g_PersonMultiGroup[pBookInfo->address-1]    = pBookInfo->multi_group;
    for(j=0; j <PERSONAL_NUMBER_LIMIT; j++)
	{
        g_PersonNumberType[pBookInfo->address-1][j] = BOOK_NOT_USE;
	}
    InsertList(SortedBookHandle, nIndex,pBookInfo->address, EQS_NA);

    return pBookInfo->address;
}


INT16 AddBook(pb_group_info_type* pGroupInfo, pb_book_info_type* pBookInfo)
{

    INT16  nIndex, j;
    INT    cmp;
         
    pBookInfo->address = alloc_pb_node( PB_BOOK_INFO_I );
	pBookInfo->next_index = 0;
    if( IsValid( PB_BOOK_INFO_I, pBookInfo->address ) == FALSE )
	{
        return 0;             //메모리 부족 
	}
    
    pBookInfo->group_address = pGroupInfo->address;
    pGroupInfo->wMember++;
    // ui_put_pb( PB_GROUP_INFO_I, pGroupInfo); //01.04.10

    STRCPY( g_szPersonName[pBookInfo->address-1], pBookInfo->name );
    g_PersonGroup[pBookInfo->address-1]         = pGroupInfo->address;
    g_PersonMultiGroup[pBookInfo->address-1]    = pBookInfo->multi_group;
    for(j=0; j <PERSONAL_NUMBER_LIMIT; j++)
	{
        g_PersonNumberType[pBookInfo->address-1][j] = BOOK_NOT_USE;
	}

    return pBookInfo->address;     
}

INT16 ReverseAddNumber(pb_book_info_type* pBookInfo, BYTE* number, UINT8 type)
{    
    pb_number_info_type new_number;	
    INT16 Index;


    if( type < NUMBER_TYPE_MIN || type > NUMBER_TYPE_MAX)
	{
        return 0;
	}
        
    new_number.address = alloc_pb_node(PB_NUMBER_INFO_I);
    if( IsValid( PB_NUMBER_INFO_I, new_number.address ) == FALSE )
	{
        return 0;
	}

    init_pb_node(PB_NUMBER_INFO_I , &new_number );
    new_number.book_address = pBookInfo->address;  
    new_number.number_type = type;
	STRCPY(new_number.digits, number);

#if 0
    Index = SumOfNumber(new_number.digits)%10;
    new_number.slink = active_list.number_search[Index];
    active_list.number_search[Index] = new_number.address;
#endif

          
    if( pBookInfo->number_address == 0 )
    {
        pBookInfo->number_address = new_number.address;
        new_number.next_index     = 0;
        active_list.number_info   = new_number.address;

        ui_put_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &new_number );        
    }
    else
    {
        new_number.next_index = pBookInfo->number_address;
        pBookInfo->number_address = new_number.address;

        ui_put_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &new_number );        
    }


   // ui_put_pb( PB_ACTIVE_LIST_I,(pb_item_type*)&active_list );  
	if( g_PersonNumberType[pBookInfo->address-1][0] == BOOK_NOT_USE )
	{
		g_PersonNumberType[pBookInfo->address-1][0] = type;
	}
	else
	{
		for( Index=PERSONAL_NUMBER_LIMIT-2; Index >= 0; Index--)
		{
			g_PersonNumberType[pBookInfo->address-1][Index+1] = g_PersonNumberType[pBookInfo->address-1][Index];
		}
		g_PersonNumberType[pBookInfo->address-1][0] = type;
	}

    return new_number.address;
}

INT16 AddNumber(pb_book_info_type* pBookInfo, BYTE* number, UINT8 type)
{    
    pb_number_info_type new_number, pre_number;	
    INT16 Index;


    if( type < NUMBER_TYPE_MIN || type > NUMBER_TYPE_MAX)
	{
        return 0;
	}
        
    new_number.address = alloc_pb_node(PB_NUMBER_INFO_I);
    if( IsValid( PB_NUMBER_INFO_I, new_number.address ) == FALSE )
	{
        return 0;
	}

    init_pb_node(PB_NUMBER_INFO_I , &new_number );
    new_number.book_address = pBookInfo->address;  
    new_number.number_type = type;
	STRCPY(new_number.digits, number);

#if 0
    Index = SumOfNumber(new_number.digits)%10;
    new_number.slink = active_list.number_search[Index];
    active_list.number_search[Index] = new_number.address;
#endif

    Index = 0;
          
    if( pBookInfo->number_address == 0 )
    {
        pBookInfo->number_address = new_number.address;
        new_number.next_index     = 0;
        active_list.number_info   = new_number.address;
        ui_put_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &new_number );        		
    }
    else
    {
		pre_number.address = pBookInfo->number_address;
		while( ui_get_pb( PB_NUMBER_INFO_I, &pre_number) > 0 && Index < PERSONAL_NUMBER_LIMIT )
		{
			if( IsValid( PB_NUMBER_INFO_I, pre_number.next_index ) )
			{
				pre_number.address = pre_number.next_index;
				Index++;
			}
			else
				break;
		}
		pre_number.next_index = new_number.address;
		new_number.next_index = 0;
        
		ui_put_pb( PB_NUMBER_INFO_I, &pre_number);
        ui_put_pb( PB_NUMBER_INFO_I ,(pb_item_type*) &new_number );
    }
	g_PersonNumberType[pBookInfo->address-1][Index] = type;
    return new_number.address;
}



INT16 AddAnnivInfo(pb_book_info_type* pBookInfo, pb_anniv_info_type* pAnnivInfo)
{
	pb_anniv_info_type anniv_info;

    if( pAnnivInfo->month < 1 || pAnnivInfo->month > 12 )
        return 0;

    pAnnivInfo->book_address = pBookInfo->address;
    pAnnivInfo->address = alloc_pb_node(PB_ANNIV_INFO_I);
    
    if( IsValid( PB_ANNIV_INFO_I, pAnnivInfo->address ) == FALSE )
        return 0;

    switch( pAnnivInfo->type )   /* 0:없음 1:생일 2: 결혼 3: 첫만남 4: 편집  */
    {                 
    case BIRTH_ANNIV:// 1
        STRCPY( pAnnivInfo->content, SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU1));
        break;
    case WEDDING_ANNIV: //2
        STRCPY( pAnnivInfo->content, SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU2));
        break;     
    case MEET_ANNIV:  //3
        STRCPY( pAnnivInfo->content, SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU3));
        break;
    case ETC_ANNIV:
        break;
    default:      
        return 0;       
    }
     
    pAnnivInfo->link = active_list.anniv_info[pAnnivInfo->month-1];
    active_list.anniv_info[pAnnivInfo->month-1] = pAnnivInfo->address;

    pAnnivInfo->next_index = pBookInfo->anniv_address;
    pBookInfo->anniv_address = pAnnivInfo->address;
	ui_put_pb( PB_ANNIV_INFO_I ,(pb_item_type*) pAnnivInfo );  

#if 0
    if( pBookInfo->anniv_address == 0 )
    {
        pBookInfo->anniv_address = pAnnivInfo->address;
        pAnnivInfo->next_index =  0;  //redstar
        ui_put_pb( PB_ANNIV_INFO_I ,(pb_item_type*) pAnnivInfo );        
    }
    else
    {
        pAnnivInfo->next_index = pBookInfo->anniv_address;
        pBookInfo->anniv_address = pAnnivInfo->address;
        
        anniv_info.address = pBookInfo->anniv_address;
        ui_put_pb( PB_ANNIV_INFO_I ,(pb_item_type*) pAnnivInfo );  
    }
#endif
    return pAnnivInfo->address;
}

INT16 AddEmail(pb_book_info_type* pBookInfo, BYTE* szEmailID)	
{
	pb_email_info_type new_email;

    init_pb_node(PB_EMAIL_INFO_I , &new_email );
    new_email.address = alloc_pb_node(PB_EMAIL_INFO_I);

    if( IsValid( PB_EMAIL_INFO_I, new_email.address ) == FALSE )
        return 0;
    
    new_email.book_address = pBookInfo->address;  
    STRCPY(new_email.szMailID, szEmailID);
	
    new_email.next_index = pBookInfo->email_address;
    pBookInfo->email_address = new_email.address;
    ui_put_pb( PB_EMAIL_INFO_I, &new_email );
    return new_email.address;
}


INT16 SumOfNumber(BYTE *number)
{
	char num_temp[32];
	int value = 0;
	int i , size;
	
	size = strlen(number);
	strcpy((char *)num_temp, number);
	
	for(i = 0 ; i <size; i++)
	{	
		value += 0x0f & (int)num_temp[i];
	}
	return value;
}

