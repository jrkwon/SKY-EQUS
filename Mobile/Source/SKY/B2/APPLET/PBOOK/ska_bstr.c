/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     ska_bstr.c
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
** 00-09-14   redstar    Create
*/

#include "ska.h"

#define BYTE_BUFFER_SIZE   1024
#define INT16_BUFFER_SIZE  512 

#define ADDRESS_TABLE_BUFFER 200


#define  MAX_BOOK_HANDLE   MAX_GROUP_NUMBER

#ifndef WIN32 

fs_rsp_msg_type           BookSysFile;
fs_rsp_msg_type           GroupFile;
fs_rsp_msg_type           BookFile;
fs_rsp_msg_type           NumberFile;
fs_rsp_msg_type           EmailFile;
fs_rsp_msg_type           AnnivFile;
fs_rsp_msg_type           ScheduleFile;
fs_rsp_msg_type           TimetableFile;
#else
FILE*           BookSysFile;
FILE*           GroupFile;
FILE*           BookFile;
FILE*           NumberFile;
FILE*           EmailFile;
FILE*           AnnivFile;
FILE*           ScheduleFile;
FILE*           TimetableFile;
#endif /* FEATURE_EQUS_A10B1 */

INT16   g_SDialNumber[MAX_SDIAL_NUMBER];
BYTE    g_szGroupName[MAX_GROUP_NUMBER][MAX_GROUP_NAME+1];    // redstar  2001.01.05
INT8    g_GroupOrder[MAX_GROUP_NUMBER];
UINT8   g_nGroup;
BYTE    g_szPersonName[MAX_BOOK_NUMBER][MAX_BOOK_NAME+1];
UINT8   g_PersonGroup[MAX_BOOK_NUMBER];
UINT8   g_PersonMultiGroup[MAX_BOOK_NUMBER];
UINT8   g_PersonNumberType[MAX_BOOK_NUMBER][PERSONAL_NUMBER_LIMIT];


pb_active_list_type  active_list;


static INT16  Read_sdial( BYTE   address, pb_sdial_type* pSdial );
static INT16  Read_active_list( pb_active_list_type*  pactive_list );

static INT16  Read_group_info(BYTE   address, pb_group_info_type* pgroup_info );

static INT16  Read_book_info(INT16 address ,pb_book_info_type* pbook_info);
static INT16  Read_number_info(INT16 address ,pb_number_info_type* pnumber_info);
static INT16  Read_anniv_info(INT16 address ,pb_anniv_info_type* panniv_info);
static INT16  Read_memo_info(INT16 address ,pb_email_info_type* pmemo_info);

static INT16 Write_sdial( BYTE   address, pb_sdial_type* pSdial );
static INT16 Write_active_list( pb_active_list_type*  pactive_list );

static INT16 Write_group_info(BYTE   address ,pb_group_info_type*  pgroup_info);
static INT16 Write_book_info(INT16 address  ,pb_book_info_type*   pbook_info);
static INT16 Write_number_info(INT16 address,pb_number_info_type* pnumber_info);
static INT16 Write_anniv_info(INT16 address,pb_anniv_info_type* panniv_info);
static INT16 Write_memo_info(INT16 address,pb_email_info_type* pmemo_info);


void init_pb_node( PB_Type  NvType , void* data )
{
    pb_group_info_type*     group_info_ptr;
    pb_book_info_type*      book_info_ptr;
    pb_number_info_type*    number_info_ptr;
    pb_anniv_info_type*     anniv_info_ptr;
    pb_email_info_type*     memo_info_ptr;
    pb_sdial_type*          sdial_ptr;
    BYTE  i;

    switch( NvType )
    {
	case PB_INFO_I:
        memset(data, 0, sizeof(pb_info_type));
		break;
    case PB_SDIAL_I:
        sdial_ptr  = (pb_sdial_type*) data;
        sdial_ptr->address      = 0;
        sdial_ptr->dial_address = 0;
        sdial_ptr->status       = 0;

        break;

    case PB_GROUP_INFO_I:
        group_info_ptr = (pb_group_info_type*) data;
        group_info_ptr->next_index          = 0;
        group_info_ptr->first_book_address  = 0;
        //group_info_ptr->group_name[0]       = 0;
		memset(group_info_ptr->group_name, 0, MAX_GROUP_NAME+1);
        group_info_ptr->wMember             = 0;
        group_info_ptr->status              = 0;
        group_info_ptr->Bell.wBellID        = 0;
        group_info_ptr->Bell.szFileName[0]  = 0;
        group_info_ptr->tstamp              = 0;
#ifdef FEATURE_COLOR_ID_LCD
        group_info_ptr->alert_color         =(ID_LCD_COLOR_E) NONE_GROUP_ALERT_LCD;
#endif
        break;


    case PB_BOOK_INFO_I:
        book_info_ptr = (pb_book_info_type*) data;
        book_info_ptr->group_address    = 0;
        book_info_ptr->multi_group      = 0;        
        //book_info_ptr->name[0]          = 0;
		memset(book_info_ptr->name, 0, MAX_BOOK_NAME+1);
        book_info_ptr->szText[0]        = 0;
        book_info_ptr->number_address   = 0;
        book_info_ptr->anniv_address    = 0;        
        book_info_ptr->email_address    = 0;
        book_info_ptr->sync_flag        = 'C';
        book_info_ptr->mmi_flag         = 'C';
        book_info_ptr->tstamp           =  0;
        book_info_ptr->dwImageIndex     =  0;
        for(i=0;i< PB_MAX_LINK; i++)
        {
            book_info_ptr->other_link[i] = 0;
        }
        break;

    case PB_NUMBER_INFO_I:
        number_info_ptr = (pb_number_info_type*) data;
        number_info_ptr->next_index   = 0;
        number_info_ptr->slink        = 0;
        number_info_ptr->book_address = 0;
        //number_info_ptr->digits[0]    = 0;
		memset(number_info_ptr->digits, 0, PB_MAX_DIGIT_LENGTH+1);
        number_info_ptr->call_number  = 0;
        number_info_ptr->number_type  = 0;
        break;

    case PB_ANNIV_INFO_I:
        anniv_info_ptr = (pb_anniv_info_type*) data;
        anniv_info_ptr->next_index      = 0;
        anniv_info_ptr->link            = 0;
        anniv_info_ptr->content[0]      = 0;
        anniv_info_ptr->book_address    = 0;        
        anniv_info_ptr->type            = 0;
        anniv_info_ptr->month           = 1;
        anniv_info_ptr->day             = 1;
        anniv_info_ptr->fLunar          = FALSE;
        anniv_info_ptr->year            = 2001;
        break;
    case PB_EMAIL_INFO_I:
        memo_info_ptr = (pb_email_info_type*) data;
        memo_info_ptr->next_index       = 0;
        memo_info_ptr->book_address     = 0;
        memo_info_ptr->szMailID[0]      = 0;
        break;

    default:
        break;
    }
    return ;
}

//2001,01.22 free list update는 나중에 한꺼번에 active_list와 같이 해준다.
// PhoneBookUpdate 함수를 이용한다.
INT16 alloc_pb_node( PB_Type nv_type )
{
    INT16 i;
    pb_number_info_type number_info;
    pb_anniv_info_type anniv_info;
    pb_email_info_type  email_info;

    switch( nv_type )
    {
    case PB_ACTIVE_LIST_I:
        break;

    case PB_GROUP_INFO_I:
        if( active_list.nFreeGroup == 0 )
            return 0;
        for(i=0; i<MAX_GROUP_NUMBER; i++)
        {
            if( g_szGroupName[i][0] == BOOK_NOT_USE )
                break;
        }
        if( i < MAX_GROUP_NUMBER )
        {
			if( active_list.nFreeGroup > 0 )
			{
				active_list.nFreeGroup--;
			}
            return (i+1);
        }
        else
            return 0;

        break;


    case PB_BOOK_INFO_I:
        if( active_list.nFreeBook == 0 )
            return 0;
        for(i=0; i<MAX_BOOK_NUMBER; i++)
        {
            if( g_szPersonName[i][0] == BOOK_NOT_USE )
                break;
        }
        if( i < MAX_BOOK_NUMBER )
        {
			if( active_list.nFreeBook > 0 )
			{
				active_list.nFreeBook--;
			}
            return (i+1);
        }
        else
            return 0;

        break;

    case PB_NUMBER_INFO_I:
        if( active_list.nFreeNumber == 0 || active_list.free_number_info == 0)
            return 0;

        number_info.address = active_list.free_number_info;
        active_list.free_number_info = pb_get_address( PB_NUMBER_INFO_I , number_info.address );
        if( number_info.address > active_list.number_info_limit )
            active_list.number_info_limit = number_info.address;

		if( active_list.nFreeNumber > 0 )
		{
			active_list.nFreeNumber--;
		}

        return number_info.address;
                
    case PB_ANNIV_INFO_I:
        if( active_list.nFreeAnniv == 0 || active_list.free_anniv_info == 0 )
            return 0;

        anniv_info.address = active_list.free_anniv_info;
        active_list.free_anniv_info = pb_get_address( PB_ANNIV_INFO_I , (INT16)anniv_info.address );
		if( active_list.nFreeAnniv > 0 )
		{
			active_list.nFreeAnniv--;
		}
        return anniv_info.address;

    case PB_EMAIL_INFO_I:
        if( active_list.nFreeEMail == 0 || active_list.free_email_info == 0 )
            return 0;
        
        email_info.address = active_list.free_email_info;
        active_list.free_email_info = pb_get_address( PB_EMAIL_INFO_I , email_info.address );
		if( active_list.nFreeEMail > 0 )
		{
			active_list.nFreeEMail--;
		}
        return email_info.address;
        
    default:
        return 0;       
    } 
	return 0;
}


//2001,01.22 free list update는 나중에 한꺼번에 active_list와 같이 해준다.
// PhoneBookUpdate 함수를 이용한다.
BOOL  free_pb_node( PB_Type nv_type , INT16 address )  
{
    pb_number_info_type    number_info;
	pb_book_info_type      book_info;
	pb_group_info_type     group_info;
    BOOL result = TRUE;

    switch( nv_type )
    {
    case PB_ACTIVE_LIST_I:
        break;

    case PB_GROUP_INFO_I:
        if( 0 < address  && address <= MAX_GROUP_NUMBER )
        {            
			init_pb_node( PB_GROUP_INFO_I, &group_info);
			group_info.address = address;
			ui_put_pb( PB_GROUP_INFO_I, &group_info);
			g_szGroupName[address-1][0] = BOOK_NOT_USE;
            active_list.nFreeGroup++;
			if( active_list.nFreeGroup > MAX_GROUP_NUMBER )
				active_list.nFreeGroup = MAX_GROUP_NUMBER;
        }
        break;

    case PB_BOOK_INFO_I:
        if( 0 < address  && address <= MAX_BOOK_NUMBER )
        {

			init_pb_node( PB_BOOK_INFO_I, &book_info);
			book_info.address = address;
			ui_put_pb( PB_BOOK_INFO_I, &book_info);

            g_szPersonName[address-1][0] = BOOK_NOT_USE;
            active_list.nFreeBook++;
			if( active_list.nFreeBook > MAX_BOOK_NUMBER )
			{
				active_list.nFreeBook = MAX_BOOK_NUMBER;
			}
        }
        break;

    case PB_NUMBER_INFO_I:
        if( 0 < address  && address <= MAX_PHONE_NUMBER )
        {
            pb_put_address( PB_NUMBER_INFO_I , address , active_list.free_number_info );
            active_list.free_number_info = address;
            active_list.nFreeNumber++;
			if( active_list.nFreeNumber > MAX_PHONE_NUMBER )
			{
				active_list.nFreeNumber = MAX_PHONE_NUMBER;
			}

            init_pb_node( PB_NUMBER_INFO_I, &number_info);
            number_info.address = address;
            ui_put_pb( PB_NUMBER_INFO_I, &number_info);
        }
        break;

    case PB_ANNIV_INFO_I:
        if( 0 < address  && address <= MAX_ANNIV_NUMBER )
        {
			pb_anniv_info_type   anniv_info;

            pb_put_address( PB_ANNIV_INFO_I , address , active_list.free_anniv_info );
            active_list.free_anniv_info = address;
            active_list.nFreeAnniv++;
			if( active_list.nFreeAnniv > MAX_ANNIV_NUMBER )
			{
				active_list.nFreeAnniv = MAX_ANNIV_NUMBER;
			}

            init_pb_node( PB_ANNIV_INFO_I, &anniv_info);
			anniv_info.address = address;
			ui_put_pb( PB_ANNIV_INFO_I, &anniv_info);
        }
        break;

    case PB_EMAIL_INFO_I:
        if( 0 < address  && address <= MAX_EMAIL_NUMBER )
        {
            pb_put_address( PB_EMAIL_INFO_I , address , active_list.free_email_info );
            active_list.free_email_info = address;
            active_list.nFreeEMail++;
			if( active_list.nFreeEMail > MAX_EMAIL_NUMBER )
			{
				active_list.nFreeEMail = MAX_EMAIL_NUMBER;
			}
        }
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
}


INT16   ui_get_pb( PB_Type  nv_type ,  void*  data )
{
    INT16 result = 1;

    switch( nv_type )
    {
    case PB_SDIAL_I:
        if( IsValid( PB_SDIAL_I, ((pb_sdial_type*)data)->address ) == FALSE )
            return 0;

        result =Read_sdial( ((pb_sdial_type*)data)->address,(pb_sdial_type*)data);

        break;
    case PB_INFO_I:
        {
#ifndef WIN32 
            fs_rsp_msg_type        rsp_msg;           
            fs_seek_read( BookSysFile.open.handle, FS_SEEK_SET, PB_INFO_POS,\
                          data,sizeof(pb_info_type),NULL,&rsp_msg);           
#else
       
            fseek( BookSysFile, PB_INFO_POS, SEEK_SET );
            fread(data,sizeof(pb_info_type),1, BookSysFile);          
#endif
        }
        return 1;

        break;
    case PB_ACTIVE_LIST_I:
        result =Read_active_list((pb_active_list_type*) data );
        break;

    case PB_GROUP_INFO_I:
        if( IsValid( PB_GROUP_INFO_I, ((pb_group_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }               
        result =Read_group_info( ((pb_group_info_type*)data)->address,(pb_group_info_type*)data);
        break;


    case PB_BOOK_INFO_I:
        if( IsValid( PB_BOOK_INFO_I, ((pb_book_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }          
        result =Read_book_info( ((pb_book_info_type*)data)->address,(pb_book_info_type*)data);
        break;
    case PB_NUMBER_INFO_I:
        if( IsValid( PB_NUMBER_INFO_I, ((pb_number_info_type*)data)->address ) == FALSE )
        {
           //SKY_ASSERT(FALSE);
            return 0;
        }         
        result =Read_number_info( ((pb_number_info_type*)data)->address,(pb_number_info_type*)data);
        break;
    case PB_ANNIV_INFO_I:
        if( IsValid( PB_ANNIV_INFO_I, ((pb_anniv_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }
        result =Read_anniv_info( ((pb_anniv_info_type*)data)->address,(pb_anniv_info_type*)data);
        break;
    case PB_EMAIL_INFO_I:
        if( IsValid( PB_EMAIL_INFO_I, ((pb_email_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }
        result =Read_memo_info( ((pb_email_info_type*)data)->address,(pb_email_info_type*)data);
        break;

    default:
        result = 0;
        break;
    }
    return result;

}
INT16   ui_put_pb( PB_Type  nv_type ,  void*  data )
{
    INT16 result = 1;
    switch( nv_type )
    {
    case PB_SDIAL_I:
        if( IsValid( PB_SDIAL_I, ((pb_sdial_type*)data)->address ) == FALSE )
            return 0;
        result = Write_sdial( ((pb_sdial_type*)data)->address,(pb_sdial_type*)data);
        break;
    case PB_INFO_I:
        {
#ifndef WIN32 
            fs_rsp_msg_type        rsp_msg;           
            fs_seek_write( BookSysFile.open.handle, FS_SEEK_SET, PB_INFO_POS,\
                          data,sizeof(pb_info_type),NULL,&rsp_msg);           
#else
       
            fseek( BookSysFile, PB_INFO_POS, SEEK_SET );
            fwrite(data,sizeof(pb_info_type),1, BookSysFile);          
#endif
        }        
        break;

    case PB_ACTIVE_LIST_I:
        result =Write_active_list((pb_active_list_type*) data );
        break;

    case PB_GROUP_INFO_I:
        if( IsValid( PB_GROUP_INFO_I, ((pb_group_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }
        result =Write_group_info( ((pb_group_info_type*)data)->address,(pb_group_info_type*)data);
        break;

    case PB_BOOK_INFO_I:
        if( IsValid( PB_BOOK_INFO_I, ((pb_book_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }           
        result =Write_book_info( ((pb_book_info_type*)data)->address,(pb_book_info_type*)data);
        break;
    case PB_NUMBER_INFO_I:
        if( IsValid( PB_NUMBER_INFO_I, ((pb_number_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }         
        result =Write_number_info( ((pb_number_info_type*)data)->address,(pb_number_info_type*)data);
        break;
    case PB_ANNIV_INFO_I:
        if( IsValid( PB_ANNIV_INFO_I, ((pb_anniv_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }
        result =Write_anniv_info( ((pb_anniv_info_type*)data)->address,(pb_anniv_info_type*)data);
        break;
    case PB_EMAIL_INFO_I:
        if( IsValid( PB_EMAIL_INFO_I, ((pb_email_info_type*)data)->address ) == FALSE )
        {
            //SKY_ASSERT(FALSE);
            return 0;
        }
        result =Write_memo_info( ((pb_email_info_type*)data)->address,(pb_email_info_type*)data);
        break;

    default:
        result = 0;
        break;
    }
	if( result )
	{
		SKY_MAKE_NO_VIRGIN();
	}
    return result;
}


INT16    pb_get_address( PB_Type  nv_type ,  INT16 address )
{

    INT16  two_byte;

    DWORD index = 0;
    INT16 result = 1;

    
#ifndef WIN32 
    fs_rsp_msg_type        rsp_msg;
#endif /* FEATURE_EQUS_A10B1 */


    switch( nv_type )
    {

    case PB_NUMBER_INFO_I:
        if( IsValid( PB_NUMBER_INFO_I, address ) == FALSE )
            return 0;
#ifndef WIN32 

        index = NUMBER_INFO_ALLOC_POS + (address-1)*sizeof(INT16);      
        fs_seek_read( BookSysFile.open.handle, FS_SEEK_SET,index, &two_byte, sizeof(INT16), NULL,&rsp_msg);

        result = two_byte;
#else 
    
        index = NUMBER_INFO_ALLOC_POS + (address-1)*sizeof(INT16);  
        fseek( BookSysFile,  index, SEEK_SET );
        fread( &two_byte,sizeof(INT16), 1, BookSysFile);
        result = two_byte;
    
#endif /* FEATURE_EQUS_A10B1 */

        break;
    case PB_ANNIV_INFO_I:
        if( IsValid( PB_ANNIV_INFO_I, address ) == FALSE )
            return 0;
#ifndef WIN32 

        index = ANNIV_INFO_ALLOC_POS + (address-1)*sizeof(INT16);      
        fs_seek_read( BookSysFile.open.handle, FS_SEEK_SET,index,&two_byte, sizeof(INT16),NULL,&rsp_msg);

        result = two_byte;
#else 
    
        index = ANNIV_INFO_ALLOC_POS + (address-1)*sizeof(INT16);         
        fseek( BookSysFile,  index, SEEK_SET );
        fread( &two_byte,sizeof(INT16), 1, BookSysFile);
        result = two_byte;
    
#endif /* FEATURE_EQUS_A10B1 */

        break;
    case PB_EMAIL_INFO_I:
        if( IsValid( PB_EMAIL_INFO_I, address ) == FALSE )
            return 0;
#ifndef WIN32 

        index = EMAIL_INFO_ALLOC_POS + (address-1)*sizeof(INT16);      
        fs_seek_read( BookSysFile.open.handle, FS_SEEK_SET,index,&two_byte,sizeof(INT16),NULL,&rsp_msg);

        result = two_byte;
#else 
    
        index = EMAIL_INFO_ALLOC_POS + (address-1)*sizeof(INT16);  
        fseek( BookSysFile,  index, SEEK_SET );
        fread( &two_byte,sizeof(INT16),1, BookSysFile);
        result = two_byte;
    
#endif /* FEATURE_EQUS_A10B1 */

        break;

    default:
        result = 0;
        break;
    }
    return result;


}

INT16    pb_put_address( PB_Type  nv_type ,  INT16 address , INT16 data )
{
    INT16  two_byte;

    DWORD index = 0;
    INT16 result = 1;

    
#ifndef WIN32 
    fs_rsp_msg_type        rsp_msg;
#endif /* FEATURE_EQUS_A10B1 */


    switch( nv_type )
    {
    case PB_NUMBER_INFO_I:
        if( IsValid( PB_NUMBER_INFO_I, address ) == FALSE )
            return 0;
        two_byte = data;
#ifndef WIN32 

        index = NUMBER_INFO_ALLOC_POS + (address-1)*sizeof(INT16);  
        fs_seek_write( BookSysFile.open.handle, FS_SEEK_SET,index,&two_byte,sizeof(INT16),NULL,&rsp_msg);
        result = two_byte;
#else 
    
        index = NUMBER_INFO_ALLOC_POS + (address-1)*sizeof(INT16);  
        fseek( BookSysFile,  index, SEEK_SET );
        fwrite(&two_byte,sizeof(INT16),1, BookSysFile);
        result = two_byte;
    
#endif /* FEATURE_EQUS_A10B1 */

        break;
    case PB_ANNIV_INFO_I:
        if( IsValid( PB_ANNIV_INFO_I, address ) == FALSE )
            return 0;
        two_byte = data;
#ifndef WIN32 

        index = ANNIV_INFO_ALLOC_POS + (address-1)*sizeof(INT16);      
        fs_seek_write( BookSysFile.open.handle, FS_SEEK_SET,index,&two_byte,sizeof(INT16),NULL,&rsp_msg);
        result = two_byte;
#else 
    
        index = ANNIV_INFO_ALLOC_POS + (address-1)*sizeof(INT16);  
        fseek( BookSysFile,  index, SEEK_SET );
        fwrite(&two_byte,sizeof(INT16),1,BookSysFile);
        result = two_byte;
    
#endif /* FEATURE_EQUS_A10B1 */

        break;
    case PB_EMAIL_INFO_I:
        if( IsValid( PB_EMAIL_INFO_I, address ) == FALSE )
            return 0;
        two_byte = data;
#ifndef WIN32 

        index = EMAIL_INFO_ALLOC_POS + (address-1)*sizeof(INT16);  
        fs_seek_write( BookSysFile.open.handle, FS_SEEK_SET,index,&two_byte,sizeof(INT16),NULL,&rsp_msg);
        result = two_byte;
#else 
    
        index = EMAIL_INFO_ALLOC_POS + (address-1)*sizeof(INT16);  
        fseek( BookSysFile,  index, SEEK_SET );
        fwrite(&two_byte,sizeof(INT16),1,BookSysFile);
        result = two_byte;
    
#endif /* FEATURE_EQUS_A10B1 */
        break;

    default:
        result = 0;
        break;
    }
    return result;
}

#ifndef WIN32
void   pb_get_header( PB_Type  nv_type , SKY_FILE_HEADER_T* pHeader )
{

    fs_rsp_msg_type        rsp_msg;    
    switch( nv_type )
    {
    case PB_SYS_I:
        fs_seek_read( BookSysFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_GROUP_INFO_I:  
        fs_seek_read( GroupFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;

    case PB_BOOK_INFO_I:
        fs_seek_read( BookFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_NUMBER_INFO_I:
        fs_seek_read( NumberFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_EMAIL_INFO_I:
        fs_seek_read( EmailFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_ANNIV_INFO_I:
        fs_seek_read( AnnivFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    default:   
        break;
    }
}

#else 

void   pb_get_header( PB_Type  nv_type , SKY_FILE_HEADER_T* pHeader )
{
    switch( nv_type )
    {
    case PB_SYS_I:
        fseek( BookSysFile,  0, SEEK_SET );
        fread( pHeader,sizeof(SKY_FILE_HEADER_T),1,BookSysFile);
        break;
    case PB_GROUP_INFO_I:
        fseek( GroupFile,  0, SEEK_SET );
        fread( pHeader,sizeof(SKY_FILE_HEADER_T),1,GroupFile);
        break;
    case PB_BOOK_INFO_I:
        fseek( BookFile,  0, SEEK_SET );
        fread( pHeader,sizeof(SKY_FILE_HEADER_T),1,BookFile);
        break;
    case PB_NUMBER_INFO_I:
        fseek( NumberFile,  0, SEEK_SET );
        fread( pHeader,sizeof(SKY_FILE_HEADER_T),1,NumberFile);
        break;
    case PB_EMAIL_INFO_I:
        fseek( EmailFile,  0, SEEK_SET );
        fread( pHeader,sizeof(SKY_FILE_HEADER_T),1,EmailFile);
        break;
    case PB_ANNIV_INFO_I:
        fseek( AnnivFile,  0, SEEK_SET );
        fread( pHeader,sizeof(SKY_FILE_HEADER_T),1,AnnivFile);
        break;
    default:
        break;
    }

}
#endif


#ifndef WIN32
void   pb_put_header( PB_Type  nv_type , SKY_FILE_HEADER_T* pHeader )
{

    fs_rsp_msg_type        rsp_msg;    
    switch( nv_type )
    {
    case PB_SYS_I:
        fs_seek_write( BookSysFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_GROUP_INFO_I:  
        fs_seek_write( GroupFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_BOOK_INFO_I:
        fs_seek_write( BookFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_NUMBER_INFO_I:
        fs_seek_write( NumberFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_EMAIL_INFO_I:
        fs_seek_write( EmailFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    case PB_ANNIV_INFO_I:
        fs_seek_write( AnnivFile.open.handle, FS_SEEK_SET,0,pHeader,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        break;
    default:   
        break;
    }
}

#else 

void   pb_put_header( PB_Type  nv_type , SKY_FILE_HEADER_T* pHeader )
{
    switch( nv_type )
    {
    case PB_SYS_I:
        fseek( BookSysFile,  0, SEEK_SET );
        fwrite( pHeader,sizeof(SKY_FILE_HEADER_T),1,BookSysFile);
        break;
    case PB_GROUP_INFO_I:
        fseek( GroupFile,  0, SEEK_SET );
        fwrite( pHeader,sizeof(SKY_FILE_HEADER_T),1,GroupFile);
        break;
    case PB_BOOK_INFO_I:
        fseek( BookFile,  0, SEEK_SET );
        fwrite( pHeader,sizeof(SKY_FILE_HEADER_T),1,BookFile);
        break;
    case PB_NUMBER_INFO_I:
        fseek( NumberFile,  0, SEEK_SET );
        fwrite( pHeader,sizeof(SKY_FILE_HEADER_T),1,NumberFile);
        break;
    case PB_EMAIL_INFO_I:
        fseek( EmailFile,  0, SEEK_SET );
        fwrite( pHeader,sizeof(SKY_FILE_HEADER_T),1,EmailFile);
        break;
    case PB_ANNIV_INFO_I:
        fseek( AnnivFile,  0, SEEK_SET );
        fwrite( pHeader,sizeof(SKY_FILE_HEADER_T),1,AnnivFile);
        break;

    default:
        break;
    }

}
#endif

INT16  Read_sdial(BYTE   address , pb_sdial_type* pSdial)
{

#ifndef WIN32 

    fs_rsp_msg_type        rsp_msg;

    DWORD index =  SDIAL_POS + (address-1) *sizeof(pb_sdial_type);  
    
    fs_seek_read( BookSysFile.open.handle, FS_SEEK_SET,index,pSdial,sizeof(pb_sdial_type),NULL,&rsp_msg);
  
    return 1;

#else

    
    DWORD  i;

    i = SDIAL_POS + (address-1) *sizeof(pb_sdial_type);
    
    fseek( BookSysFile,  i, SEEK_SET );
    fread( pSdial,sizeof(pb_sdial_type),1,BookSysFile);
   
    return 1;
#endif /* FEATURE_EQUS_A10B1 */
       
}


INT16 Read_active_list( pb_active_list_type*  pactive_list )
{
  
#ifndef WIN32 

    fs_rsp_msg_type        rsp_msg;

    DWORD index = ACTIVE_LIST_POS;          
    fs_seek_read( BookSysFile.open.handle, FS_SEEK_SET,index,pactive_list,sizeof(pb_active_list_type),NULL,&rsp_msg);
  
    return 1;

#else

    DWORD  i;
    i = ACTIVE_LIST_POS;
    fseek( BookSysFile,  i, SEEK_SET );
    fread( pactive_list,sizeof(pb_active_list_type),1,BookSysFile);

    return 1;
    
#endif /* FEATURE_EQUS_A10B1 */



}


INT16  Read_group_info(BYTE   address, pb_group_info_type* pgroup_info )
{

#ifndef WIN32 

    fs_rsp_msg_type        rsp_msg;
    byte i;

    DWORD index = GROUP_INFO_POS + (address-1) *sizeof(pb_group_info_type);          
    i = pgroup_info->address;
    fs_seek_read( GroupFile.open.handle, FS_SEEK_SET,index, pgroup_info,sizeof(pb_group_info_type),NULL,&rsp_msg);
  
    return 1;

#else
    
    DWORD  i;

    i = GROUP_INFO_POS + (address-1) *sizeof(pb_group_info_type);
    fseek( GroupFile,  i, SEEK_SET );
    fread( pgroup_info,sizeof(pb_group_info_type),1, GroupFile);


    return 1;

#endif /* FEATURE_EQUS_A10B1 */

}


INT16  Read_book_info(INT16 address ,pb_book_info_type* pbook_info)
{


#ifndef WIN32 

    fs_rsp_msg_type         rsp_msg;

    DWORD index = BOOK_INFO_POS + (address-1) *sizeof(pb_book_info_type);          
    fs_seek_read( BookFile.open.handle, FS_SEEK_SET,index,pbook_info,sizeof(pb_book_info_type),NULL,&rsp_msg);
  
    return 1;

#else

    DWORD  i;

    i = BOOK_INFO_POS + (address-1) *sizeof(pb_book_info_type);
    fseek( BookFile,  i, SEEK_SET );
    fread( pbook_info,sizeof(pb_book_info_type),1, BookFile);

    return 1;

#endif /* FEATURE_EQUS_A10B1 */

}

INT16  Read_number_info(INT16 address ,pb_number_info_type* pnumber_info)
{
  
#ifndef WIN32 

    fs_rsp_msg_type         rsp_msg;

    DWORD index = NUMBER_INFO_POS +(address-1) *sizeof(pb_number_info_type);
    fs_seek_read( NumberFile.open.handle, FS_SEEK_SET,index,pnumber_info,sizeof(pb_number_info_type),NULL,&rsp_msg);
 
    return 1;
#else

    DWORD  i;

    i = NUMBER_INFO_POS + (address-1) *sizeof(pb_number_info_type);
    fseek( NumberFile,  i, SEEK_SET );
    fread( pnumber_info,sizeof(pb_number_info_type),1, NumberFile);

    return 1;

#endif /* FEATURE_EQUS_A10B1 */

}

INT16  Read_number(void* pBuffer, INT16 StartAddress, UINT8 nNumber)
{
  
#ifndef WIN32 

    fs_rsp_msg_type         rsp_msg;

    DWORD index =  NUMBER_INFO_POS +(StartAddress-1) *sizeof(pb_number_info_type);
    fs_seek_read( NumberFile.open.handle, FS_SEEK_SET,index,pBuffer,sizeof(pb_number_info_type)*nNumber,NULL,&rsp_msg);
 
    return 1;

#else

    DWORD  i;

    i = NUMBER_INFO_POS + (StartAddress-1) *sizeof(pb_number_info_type);
    fseek( NumberFile,  i, SEEK_SET );
    fread( pBuffer, nNumber*sizeof(pb_number_info_type),1, NumberFile);

    return 1;

#endif /* FEATURE_EQUS_A10B1 */

}


INT16  Read_anniv_info(INT16 address ,pb_anniv_info_type* panniv_info)
{
#ifndef WIN32 

    fs_rsp_msg_type         rsp_msg;

    DWORD index = ANNIV_INFO_POS +(address-1) *sizeof(pb_anniv_info_type);          
    fs_seek_read( AnnivFile.open.handle, FS_SEEK_SET,index,panniv_info,sizeof(pb_anniv_info_type),NULL,&rsp_msg);
 

    return 1;

#else

    DWORD  i;

    i = ANNIV_INFO_POS + (address-1) *sizeof(pb_anniv_info_type);    
    fseek( AnnivFile,  i, SEEK_SET );
    fread( panniv_info,sizeof(pb_anniv_info_type),1, AnnivFile);

    return 1;

#endif /* FEATURE_EQUS_A10B1 */

}


INT16  Read_anniv(void* pBuffer, INT16 StartAddress, UINT8 nNumber)
{
  
#ifndef WIN32 

    fs_rsp_msg_type         rsp_msg;

    DWORD index =  ANNIV_INFO_POS +(StartAddress-1) *sizeof(pb_anniv_info_type);
    fs_seek_read( AnnivFile.open.handle, FS_SEEK_SET,index,pBuffer,sizeof(pb_anniv_info_type)*nNumber,NULL,&rsp_msg);
 
    return 1;

#else

    DWORD  i;

    i = ANNIV_INFO_POS + (StartAddress-1) *sizeof(pb_anniv_info_type);
    fseek( AnnivFile,  i, SEEK_SET );
    fread( pBuffer, nNumber*sizeof(pb_anniv_info_type),1, AnnivFile);

    return 1;

#endif /* FEATURE_EQUS_A10B1 */

}

INT16  Read_memo_info(INT16 address ,pb_email_info_type* pmemo_info)
{


#ifndef WIN32 

    fs_rsp_msg_type         rsp_msg;

    DWORD index = EMAIL_INFO_POS +(address-1) *sizeof(pb_email_info_type);         
    fs_seek_read( EmailFile.open.handle, FS_SEEK_SET,index,pmemo_info,sizeof(pb_email_info_type),NULL,&rsp_msg);
    return 1;

#else
    DWORD  i;


    i = EMAIL_INFO_POS + (address-1) *sizeof(pb_email_info_type);
    fseek( EmailFile,  i, SEEK_SET );
    fread( pmemo_info,sizeof(pb_email_info_type),1, EmailFile);


    return 1;

#endif /* FEATURE_EQUS_A10B1 */

}

INT16  Read_email(void* pBuffer, INT16 StartAddress, UINT8 nNumber)
{

#ifndef WIN32 

    fs_rsp_msg_type         rsp_msg;

    DWORD index =  EMAIL_INFO_POS +(StartAddress-1) *sizeof(pb_email_info_type);
    fs_seek_read( EmailFile.open.handle, FS_SEEK_SET,index,pBuffer,sizeof(pb_email_info_type)*nNumber,NULL,&rsp_msg);
 
    return 1;

#else

    DWORD  i;

    i = EMAIL_INFO_POS + (StartAddress-1) *sizeof(pb_email_info_type);
    fseek( EmailFile,  i, SEEK_SET );
    fread( pBuffer, nNumber*sizeof(pb_email_info_type),1, EmailFile);

    return 1;

#endif /* FEATURE_EQUS_A10B1 */

}


INT16 Write_sdial( BYTE   address, pb_sdial_type* pSdial )
{

#ifndef WIN32 

    fs_rsp_msg_type        rsp_msg;

    DWORD index = SDIAL_POS + (address-1) *sizeof(pb_sdial_type);          
    fs_seek_write( BookSysFile.open.handle, FS_SEEK_SET,index,pSdial,sizeof(pb_sdial_type),NULL,&rsp_msg);

    return 1;
#else
    
    DWORD  i;

    i = SDIAL_POS + (address-1) *sizeof(pb_sdial_type);
    fseek( BookSysFile,  i, SEEK_SET );
    fwrite(pSdial,sizeof(pb_sdial_type),1, BookSysFile);

    return 1;
    
#endif /* FEATURE_EQUS_A10B1 */
}


INT16 Write_active_list( pb_active_list_type*  pactive_list )
{

#ifndef WIN32 
    fs_rsp_msg_type        rsp_msg;

    DWORD index = ACTIVE_LIST_POS;          
    fs_seek_write( BookSysFile.open.handle, FS_SEEK_SET,index,pactive_list,sizeof(pb_active_list_type),NULL,&rsp_msg);

    return 1;
#else

    
    DWORD  i;
    i = ACTIVE_LIST_POS;
    fseek( BookSysFile,  i, SEEK_SET );
    fwrite(pactive_list,sizeof(pb_active_list_type),1, BookSysFile);

    return 1;

    
#endif /* FEATURE_EQUS_A10B1 */

}



INT16 Write_group_info(BYTE   address ,pb_group_info_type*  pgroup_info)
{


#ifndef WIN32 

    fs_rsp_msg_type        rsp_msg;

    DWORD index = GROUP_INFO_POS + (address-1) *sizeof(pb_group_info_type);          
    fs_seek_write( GroupFile.open.handle, FS_SEEK_SET,index,pgroup_info,sizeof(pb_group_info_type),NULL,&rsp_msg);

    return 1;
#else
    
    DWORD  i;
 
    i = GROUP_INFO_POS + (address-1) *sizeof(pb_group_info_type);
    fseek( GroupFile,  i, SEEK_SET );
    fwrite(pgroup_info,sizeof(pb_group_info_type),1, GroupFile);

    return 1;
    
#endif /* FEATURE_EQUS_A10B1 */
}


INT16 Write_book_info(INT16 address  ,pb_book_info_type*   pbook_info)
{

#ifndef WIN32 

    fs_rsp_msg_type        rsp_msg;

    DWORD index = BOOK_INFO_POS +(address-1) *sizeof(pb_book_info_type);          
    fs_seek_write( BookFile.open.handle, FS_SEEK_SET,index,pbook_info,sizeof(pb_book_info_type),NULL,&rsp_msg);

    return 1;
#else
    
    DWORD  i;

    i = BOOK_INFO_POS + (address-1) *sizeof(pb_book_info_type);
    fseek( BookFile,  i, SEEK_SET );
    fwrite(pbook_info,sizeof(pb_book_info_type),1, BookFile);
    return 1;
    
#endif /* FEATURE_EQUS_A10B1 */

}

INT16 Write_number_info(INT16 address,pb_number_info_type* pnumber_info)
{


#ifndef WIN32 
    fs_rsp_msg_type        rsp_msg;

    DWORD index =NUMBER_INFO_POS +  (address-1) *sizeof(pb_number_info_type);
    fs_seek_write( NumberFile.open.handle, FS_SEEK_SET,index,pnumber_info,sizeof(pb_number_info_type),NULL,&rsp_msg);

    return 1;
#else
    
    DWORD  i;

    i = NUMBER_INFO_POS + (address-1) *sizeof(pb_number_info_type);
    fseek( NumberFile,  i, SEEK_SET );
    fwrite(pnumber_info,sizeof(pb_number_info_type),1, NumberFile);

    return 1;
    
#endif /* FEATURE_EQUS_A10B1 */
}

INT16 Write_anniv_info(INT16 address,pb_anniv_info_type* panniv_info)
{
  
#ifndef WIN32 

    fs_rsp_msg_type        rsp_msg;

    DWORD index = ANNIV_INFO_POS +(address-1) *sizeof(pb_anniv_info_type);          
    fs_seek_write( AnnivFile.open.handle, FS_SEEK_SET,index,panniv_info,sizeof(pb_anniv_info_type),NULL,&rsp_msg);
    
    return 1;
#else
    
    DWORD  i;

    i = ANNIV_INFO_POS + (address-1) *sizeof(pb_anniv_info_type);
    fseek( AnnivFile,  i, SEEK_SET );
    fwrite(panniv_info,sizeof(pb_anniv_info_type),1, AnnivFile);

    return 1;
    
#endif /* FEATURE_EQUS_A10B1 */
}

INT16 Write_memo_info(INT16 address,pb_email_info_type* pmemo_info)
{
   
#ifndef WIN32 

    fs_rsp_msg_type        rsp_msg;

    DWORD index = EMAIL_INFO_POS + (address-1) *sizeof(pb_email_info_type);         
    fs_seek_write( EmailFile.open.handle, FS_SEEK_SET,index,pmemo_info,sizeof(pb_email_info_type),NULL,&rsp_msg);
    
    return 1;
#else
    
    DWORD  i;

    i = EMAIL_INFO_POS + (address-1) *sizeof(pb_email_info_type);
    fseek( EmailFile,  i, SEEK_SET );
    fwrite(pmemo_info,sizeof(pb_email_info_type),1, EmailFile);

    return 1;
    
#endif /* FEATURE_EQUS_A10B1 */
}


void PBookClose(void)
{

#ifndef WIN32
    fs_rsp_msg_type        close_rsp_msg;


    fs_close(BookSysFile.open.handle, NULL, &close_rsp_msg);
    fs_close(GroupFile.open.handle, NULL, &close_rsp_msg);
    fs_close(BookFile.open.handle, NULL, &close_rsp_msg);
    fs_close(NumberFile.open.handle, NULL, &close_rsp_msg);
    fs_close(EmailFile.open.handle, NULL, &close_rsp_msg);
    fs_close(AnnivFile.open.handle, NULL, &close_rsp_msg);

#else 


    fclose( BookSysFile );
    fclose( GroupFile );
    fclose( BookFile  );
    fclose( NumberFile );
    fclose( EmailFile );
    fclose( AnnivFile);

    
#endif /* FEATURE_EQUS_A10B1 */
}

void InitSysFile(void)
{    
    PBookInit(PB_NUMBER_INFO_I);
    PBookInit(PB_ANNIV_INFO_I); 
    PBookInit(PB_EMAIL_INFO_I);
    PBookInit(PB_ACTIVE_LIST_I);
    PBookInit(PB_SDIAL_I);
    PBookInit(PB_INFO_I);
}

void InitGroupFile(BYTE* sByteBuffer, UINT16 nBufferSize)
{
    INT   nWriteSize, k, size;
    pb_group_info_type GroupInfo;
#ifndef WIN32
    fs_rsp_msg_type        rsp_msg;
#endif

    size = MAX_GROUP_NUMBER * sizeof(pb_group_info_type)+sizeof(SKY_FILE_HEADER_T);
    nWriteSize = 0;
#ifndef WIN32
	fs_seek( GroupFile.open.handle, FS_SEEK_SET,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
    while( TRUE )
    {		
        if( nWriteSize + nBufferSize < size )
            fs_write( GroupFile.open.handle, sByteBuffer,nBufferSize,NULL,&rsp_msg);
        else
        {   
            fs_write( GroupFile.open.handle, sByteBuffer,(size-nWriteSize),NULL,&rsp_msg);
            init_pb_node( PB_GROUP_INFO_I, &GroupInfo);
            for( k=1; k < 6; k++)
            {
                STRCPY( GroupInfo.group_name, SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM1 + k - 1));
                GroupInfo.address    = k;
                GroupInfo.next_index = k+1;
                ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
                active_list.nFreeGroup--;
            }
            STRCPY( GroupInfo.group_name, SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM1 + k - 1));
            GroupInfo.address    = k;
            GroupInfo.next_index = 0;
            ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
            active_list.group_info = 1;
            PhoneBookUpdate();
            break;
        }
        nWriteSize += nBufferSize;
    }  
#else
	fseek( GroupFile,  sizeof(SKY_FILE_HEADER_T), SEEK_SET );
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fwrite(sByteBuffer,nBufferSize,1,GroupFile);
        else
        {   
            fwrite(sByteBuffer,(size-nWriteSize),1,GroupFile);
            init_pb_node( PB_GROUP_INFO_I, &GroupInfo);
            for( k=1; k < 6; k++)
            {
                STRCPY( GroupInfo.group_name, SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM1 + k - 1));                    
                GroupInfo.address    = k;                   
                GroupInfo.next_index = k+1;
                ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
                active_list.nFreeGroup--;
            }
            STRCPY( GroupInfo.group_name, SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM1 + k - 1));
            GroupInfo.address    = k;
            GroupInfo.next_index = 0;			
            ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);   
			active_list.nFreeGroup--;
            active_list.group_info = 1;
            PhoneBookUpdate();
            break;
        }
        nWriteSize += nBufferSize;
    }  
#endif
	SKY_KickDog();
}
void InitBookFile(BYTE* sByteBuffer, UINT16 nBufferSize)
{
    INT   nWriteSize, size;
#ifndef WIN32
    fs_rsp_msg_type        rsp_msg;
#endif

    size =  MAX_BOOK_NUMBER*sizeof(pb_book_info_type)+sizeof(SKY_FILE_HEADER_T);
    nWriteSize = 0;
#ifndef WIN32
	fs_seek( BookFile.open.handle, FS_SEEK_SET,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fs_write( BookFile.open.handle, sByteBuffer,nBufferSize,NULL,&rsp_msg);
        else
        {   
            fs_write( BookFile.open.handle, sByteBuffer,(size-nWriteSize),NULL,&rsp_msg);
            break;
        }
        nWriteSize += nBufferSize;
    }
#else
	fseek( BookFile,  sizeof(SKY_FILE_HEADER_T), SEEK_SET );
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fwrite(sByteBuffer,nBufferSize,1, BookFile);
        else
        {   
            fwrite(sByteBuffer,(size-nWriteSize),1, BookFile);
            break;
        }
        nWriteSize += nBufferSize;
    }

#endif
	SKY_KickDog();
}
void InitNumberFile(BYTE* sByteBuffer, UINT16 nBufferSize)
{
    INT   nWriteSize, size;
#ifndef WIN32
    fs_rsp_msg_type        rsp_msg;
#endif

    size = MAX_PHONE_NUMBER * sizeof(pb_number_info_type)+sizeof(SKY_FILE_HEADER_T);               
    nWriteSize = 0;
#ifndef WIN32
	fs_seek( NumberFile.open.handle, FS_SEEK_SET,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fs_write( NumberFile.open.handle, sByteBuffer,nBufferSize,NULL,&rsp_msg);
        else
        {   
            fs_write( NumberFile.open.handle, sByteBuffer,(size-nWriteSize),NULL,&rsp_msg);
            break;
        }
        nWriteSize += nBufferSize;
    }
#else
	fseek( NumberFile, sizeof(SKY_FILE_HEADER_T), SEEK_SET );
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fwrite( sByteBuffer,nBufferSize,1,NumberFile);
        else
        {   
            fwrite( sByteBuffer,(size-nWriteSize),1, NumberFile);
            break;
        }
        nWriteSize += nBufferSize;
    }

#endif
	SKY_KickDog();
}

void InitEmailFile(BYTE* sByteBuffer, UINT16 nBufferSize)
{   
    INT   nWriteSize, size;
#ifndef WIN32
    fs_rsp_msg_type        rsp_msg;
#endif

    size = MAX_EMAIL_NUMBER * sizeof(pb_email_info_type)+sizeof(SKY_FILE_HEADER_T);               
    nWriteSize = 0;
#ifndef WIN32
	fs_seek( EmailFile.open.handle, FS_SEEK_SET,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fs_write( EmailFile.open.handle, sByteBuffer,nBufferSize,NULL,&rsp_msg);
        else
        {   
            fs_write( EmailFile.open.handle, sByteBuffer,(size-nWriteSize),NULL,&rsp_msg);
            break;
        }
        nWriteSize += nBufferSize;
    }
#else
	fseek( EmailFile, sizeof(SKY_FILE_HEADER_T), SEEK_SET );
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fwrite( sByteBuffer,nBufferSize,1, EmailFile);
        else
        {   
            fwrite( sByteBuffer,(size-nWriteSize),1, EmailFile);
            break;
        }
        nWriteSize += nBufferSize;
    }
#endif
	SKY_KickDog();
}

void InitAnnivFile(BYTE* sByteBuffer, UINT16 nBufferSize)
{
    INT   nWriteSize, size;
#ifndef WIN32
    fs_rsp_msg_type        rsp_msg;
#endif

    size = MAX_ANNIV_NUMBER * sizeof(pb_anniv_info_type)+sizeof(SKY_FILE_HEADER_T);
    nWriteSize = 0;
#ifndef WIN32
	fs_seek( AnnivFile.open.handle, FS_SEEK_SET,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fs_write( AnnivFile.open.handle, sByteBuffer,nBufferSize,NULL,&rsp_msg);
        else
        {   
            fs_write( AnnivFile.open.handle, sByteBuffer,(size-nWriteSize),NULL,&rsp_msg);
            break;
        }
        nWriteSize += nBufferSize;
    }
#else
	fseek( AnnivFile, sizeof(SKY_FILE_HEADER_T), SEEK_SET );
    while( TRUE )
    {
        if( nWriteSize + nBufferSize < size )
            fwrite( sByteBuffer,nBufferSize,1, AnnivFile);
        else
        {   
            fwrite( sByteBuffer,(size-nWriteSize),1, AnnivFile);
            break;
        }
        nWriteSize += nBufferSize;
    }
#endif
	SKY_KickDog();
}


void PBookFilesOpen(void)
{
#ifndef WIN32
    fs_open_xparms_type    open_option;
    fs_rsp_msg_type        rsp_msg;
    DWORD                  size;
    pb_info_type           PersonalInfo;

    open_option.create.cleanup_option = FS_OC_CLOSE;
    //open_option.create.buffering_option = FS_OB_PROHIBIT;
    open_option.create.buffering_option = FS_OB_ALLOW;
    open_option.create.attribute_mask = FS_FA_UNRESTRICTED;

    if(SKY_IsFileOrDirExist(SKY_ROOT_DIR) == FALSE)    //폰북용 directory를 만든다.
    {
		PBookClose();
        PBookOpen();
		return;
    }
    if(SKY_IsFileOrDirExist(SKY_PBK_DIR) == FALSE)
    {
		PBookClose();
        PBookOpen();
		return;
    }

    fs_open(PSYS_FILE, FS_OA_READWRITE, &open_option, NULL, &BookSysFile);
    if( BookSysFile.open.handle == 0 )
    {
		PBookClose();
        PBookOpen();
		return;
    }


    fs_open(PGROUP_FILE, FS_OA_READWRITE, &open_option, NULL, &GroupFile);
    if( GroupFile.open.handle == 0 )
    {
		PBookClose();
        PBookOpen();
		return;
    }


    fs_open(PBOOK_FILE, FS_OA_READWRITE, &open_option, NULL,&BookFile);
    if( BookFile.open.handle == 0 )
    {
		PBookClose();
        PBookOpen();
		return;
    }


    fs_open(PNUMBER_FILE, FS_OA_READWRITE, &open_option, NULL, &NumberFile);
    if( NumberFile.open.handle == 0 )
    {
		PBookClose();
        PBookOpen();
		return;
    }   


    fs_open(PEMAIL_FILE, FS_OA_READWRITE, &open_option, NULL, &EmailFile);
    if( EmailFile.open.handle == 0 )
    {
		PBookClose();
        PBookOpen();
		return;
    }  


    fs_open(PANNIV_FILE, FS_OA_READWRITE, &open_option, NULL, &AnnivFile);
    if( AnnivFile.open.handle == 0 )
    {
		PBookClose();
        PBookOpen();
		return;
    }  
#endif
}
                                  
void PBookOpen(void)
{
    BYTE  sByteBuffer[1024];
	BOOL  *pfaUsed;
    INT   i, j, k, count, number_count, nAddress, nBufferIndex, nFreeAddress;
	
  
    pb_group_info_type  GroupInfo;
    pb_book_info_type   BookInfo;
    pb_number_info_type NumberInfo;
	pb_email_info_type* pEmailInfo;
	pb_anniv_info_type* pAnnivInfo;
	pb_sdial_type  SdialInfo;
    SKY_FILE_HEADER_T   Header;
    SKY_FILE_HEADER_T   BookHeader[6] = 
    { 
        {{2,0},"system"},
        {{2,0},"group"},
        {{2,0},"book"},
        {{2,0},"number"},
        {{2,0},"memo"},
        {{2,0},"anniv"}
    };
        
#ifndef WIN32       
    nv_dir_number_type     nv_dir_num;    
    fs_open_xparms_type    open_option;
    fs_rsp_msg_type        rsp_msg;
    DWORD                  size;
    pb_info_type           PersonalInfo;

    open_option.create.cleanup_option = FS_OC_CLOSE;
    //open_option.create.buffering_option = FS_OB_PROHIBIT;
    open_option.create.buffering_option = FS_OB_ALLOW;
    open_option.create.attribute_mask = FS_FA_UNRESTRICTED;

    size = fs_space_available( );

    InitBookListTable();

	memset(sByteBuffer,0,1024);

    if(SKY_IsFileOrDirExist(SKY_ROOT_DIR) == FALSE)    //폰북용 directory를 만든다.
    {
        SKY_MakeDir(SKY_ROOT_DIR);
    }
    if(SKY_IsFileOrDirExist(SKY_PBK_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_PBK_DIR);
    }

    fs_open(PSYS_FILE, FS_OA_READWRITE, &open_option, NULL, &BookSysFile);
    if( BookSysFile.open.handle == 0 )
    {
        fs_open(PSYS_FILE, FS_OA_CREATE, &open_option, NULL,&BookSysFile);
        pb_put_header(PB_SYS_I, &BookHeader[0]);
        InitSysFile();
    }
    else
    {
        fs_read( BookSysFile.open.handle, &Header,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        if( Header.Version.nMajor != BookHeader[0].Version.nMajor || 
            Header.Version.nMinor != BookHeader[0].Version.nMinor )
        {
           pb_put_header(PB_SYS_I, &BookHeader[0]);
           InitSysFile();
        }
        else
        {
            ui_get_pb( PB_ACTIVE_LIST_I ,(pb_item_type*) &active_list );
        }

    }

    fs_open(PGROUP_FILE, FS_OA_READWRITE, &open_option, NULL, &GroupFile);
    if( GroupFile.open.handle == 0 )
    {
        fs_open(PGROUP_FILE, FS_OA_CREATE, &open_option, NULL,&GroupFile);
        pb_put_header(PB_GROUP_INFO_I, &BookHeader[1]);
        InitGroupFile(sByteBuffer,1024);  
		
    }
    else
    {
        fs_read( GroupFile.open.handle, &Header,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);  
        if( Header.Version.nMajor != BookHeader[1].Version.nMajor || 
            Header.Version.nMinor != BookHeader[1].Version.nMinor )
        {
           pb_put_header(PB_GROUP_INFO_I, &BookHeader[1]);
           InitGroupFile(sByteBuffer,1024);		   
        }
    }

    fs_open(PBOOK_FILE, FS_OA_READWRITE, &open_option, NULL,&BookFile);
    if( BookFile.open.handle == 0 )
    {
        fs_open(PBOOK_FILE, FS_OA_CREATE, &open_option, NULL, &BookFile);
        pb_put_header(PB_BOOK_INFO_I, &BookHeader[2]);
        InitBookFile(sByteBuffer, 1024);	
    }
    else
    {
        fs_read( BookFile.open.handle, &Header,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        if( Header.Version.nMajor != BookHeader[2].Version.nMajor || 
            Header.Version.nMinor != BookHeader[2].Version.nMinor )
        {
           pb_put_header(PB_BOOK_INFO_I, &BookHeader[2]);
           InitBookFile(sByteBuffer,1024);
        }
    }

    fs_open(PNUMBER_FILE, FS_OA_READWRITE, &open_option, NULL, &NumberFile);
    if( NumberFile.open.handle == 0 )
    {
        fs_open(PNUMBER_FILE, FS_OA_CREATE, &open_option, NULL, &NumberFile);
        pb_put_header(PB_NUMBER_INFO_I, &BookHeader[3]);
        InitNumberFile(sByteBuffer,1024);
    }   
    else
    {
        fs_read( NumberFile.open.handle, &Header,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        if( Header.Version.nMajor != BookHeader[3].Version.nMajor || 
            Header.Version.nMinor != BookHeader[3].Version.nMinor )
        {
           pb_put_header(PB_NUMBER_INFO_I, &BookHeader[3]);
           InitNumberFile(sByteBuffer,1024);
        }
    }

    fs_open(PEMAIL_FILE, FS_OA_READWRITE, &open_option, NULL, &EmailFile);
    if( EmailFile.open.handle == 0 )
    {
        fs_open(PEMAIL_FILE, FS_OA_CREATE, &open_option, NULL, &EmailFile);
        pb_put_header(PB_EMAIL_INFO_I, &BookHeader[4]);
        InitEmailFile(sByteBuffer,1024);
    }  
    else
    {
        fs_read( EmailFile.open.handle, &Header,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        if( Header.Version.nMajor != BookHeader[4].Version.nMajor || 
            Header.Version.nMinor != BookHeader[4].Version.nMinor )
        {
            pb_put_header(PB_EMAIL_INFO_I, &BookHeader[4]);           
            InitEmailFile(sByteBuffer,1024);
        }
    }

    fs_open(PANNIV_FILE, FS_OA_READWRITE, &open_option, NULL, &AnnivFile);
    if( AnnivFile.open.handle == 0 )
    {
        fs_open(PANNIV_FILE, FS_OA_CREATE, &open_option, NULL, &AnnivFile);
        pb_put_header(PB_ANNIV_INFO_I, &BookHeader[5]);
        InitAnnivFile(sByteBuffer,1024);
    }  
    else
    {
        fs_read( AnnivFile.open.handle, &Header,sizeof(SKY_FILE_HEADER_T),NULL,&rsp_msg);
        if( Header.Version.nMajor != BookHeader[5].Version.nMajor || 
            Header.Version.nMinor != BookHeader[5].Version.nMinor )
        {
            pb_put_header(PB_ANNIV_INFO_I, &BookHeader[5]);           
            InitAnnivFile(sByteBuffer,1024);
        }
    }
    memset(&nv_dir_num, 0, sizeof(nv_dir_num));
    nv_dir_num.nam = SKY_GET_CURRENT_NAM();     // 자기정보 저장 
    SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );
	//init_pb_node(PB_INFO_I, &PersonalInfo);
	memset(&PersonalInfo, 0, sizeof(PersonalInfo));
    ui_get_pb(PB_INFO_I, &PersonalInfo);
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
    
#else

    InitBookListTable();
	memset(sByteBuffer,0,1024);

    if(SKY_IsFileOrDirExist(SKY_ROOT_DIR) == FALSE)    //폰북용 directory를 만든다.
    {
        SKY_MakeDir(SKY_ROOT_DIR);
    }
    if(SKY_IsFileOrDirExist(SKY_PBK_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_PBK_DIR);
    }

    if( (BookSysFile = fopen(PSYS_FILE, "rb+" )) == NULL )
    {
        BookSysFile = fopen(PSYS_FILE, "wb+");
        pb_put_header(PB_SYS_I, &BookHeader[0]); 
        InitSysFile();
    }
    else
    {
        fread(&Header,sizeof(SKY_FILE_HEADER_T),1,BookSysFile);
        if( Header.Version.nMajor != BookHeader[0].Version.nMajor || 
            Header.Version.nMinor != BookHeader[0].Version.nMinor )
        {
           pb_put_header(PB_SYS_I, &BookHeader[0]);
           InitSysFile();
        }
        else
        {
            ui_get_pb( PB_ACTIVE_LIST_I ,(pb_item_type*) &active_list );    
        }
    }

    if( (GroupFile = fopen(PGROUP_FILE, "rb+" )) == NULL )
    {
        GroupFile = fopen(PGROUP_FILE, "wb+");
        pb_put_header(PB_GROUP_INFO_I, &BookHeader[1]); 
        InitGroupFile(sByteBuffer,1024);       
    }
    else
    {
        fread(&Header,sizeof(SKY_FILE_HEADER_T),1,GroupFile);
        if( Header.Version.nMajor != BookHeader[1].Version.nMajor || 
            Header.Version.nMinor != BookHeader[1].Version.nMinor )
        {
            pb_put_header(PB_GROUP_INFO_I, &BookHeader[1]); 
            InitGroupFile(sByteBuffer,1024);
        }
    }

    if( (BookFile = fopen(PBOOK_FILE, "rb+" )) == NULL )
    {
        BookFile = fopen(PBOOK_FILE, "wb+");
        pb_put_header(PB_BOOK_INFO_I, &BookHeader[2]); 
        InitBookFile(sByteBuffer,1024);
    }
    else
    {
        fread(&Header,sizeof(SKY_FILE_HEADER_T),1,BookFile);
        if( Header.Version.nMajor != BookHeader[2].Version.nMajor || 
            Header.Version.nMinor != BookHeader[2].Version.nMinor )
        {
            pb_put_header(PB_BOOK_INFO_I, &BookHeader[2]); 
            InitBookFile(sByteBuffer,1024);
        }
    }

    if( (NumberFile = fopen(PNUMBER_FILE, "rb+" )) == NULL )
    {
        NumberFile = fopen(PNUMBER_FILE, "wb+");
        pb_put_header(PB_NUMBER_INFO_I, &BookHeader[3]); 
        InitNumberFile(sByteBuffer,1024);
    }
    else
    {
        fread(&Header,sizeof(SKY_FILE_HEADER_T),1,NumberFile);
        if( Header.Version.nMajor != BookHeader[3].Version.nMajor || 
            Header.Version.nMinor != BookHeader[3].Version.nMinor )
        {
            pb_put_header(PB_NUMBER_INFO_I, &BookHeader[3]); 
            InitNumberFile(sByteBuffer,1024);
        }
    }
    if( (EmailFile = fopen(PEMAIL_FILE, "rb+" )) == NULL )
    {
        EmailFile = fopen(PEMAIL_FILE, "wb+");
        pb_put_header(PB_EMAIL_INFO_I, &BookHeader[4]); 
        InitEmailFile(sByteBuffer,1024);
    }
    else
    {
        fread(&Header,sizeof(SKY_FILE_HEADER_T),1,EmailFile);
        if( Header.Version.nMajor != BookHeader[4].Version.nMajor || 
            Header.Version.nMinor != BookHeader[4].Version.nMinor )
        {
            pb_put_header(PB_EMAIL_INFO_I, &BookHeader[4]); 
            InitEmailFile(sByteBuffer,1024);
        }

    }

    if( (AnnivFile = fopen(PANNIV_FILE, "rb+" )) == NULL )
    {
        AnnivFile = fopen(PANNIV_FILE, "wb+");
        pb_put_header(PB_ANNIV_INFO_I, &BookHeader[5]); 
        InitAnnivFile(sByteBuffer,1024);
    }
    else
    {
        fread(&Header,sizeof(SKY_FILE_HEADER_T),1,AnnivFile);
        if( Header.Version.nMajor != BookHeader[5].Version.nMajor || 
            Header.Version.nMinor != BookHeader[5].Version.nMinor )
        {             
            pb_put_header(PB_ANNIV_INFO_I, &BookHeader[5]);            
            InitAnnivFile(sByteBuffer,1024);
        }
    }

#endif /* FEATURE_EQUS_A10B1 */
	for( k = 0; k< MAX_SDIAL_NUMBER; k++ )
	{
		g_SDialNumber[k] = BOOK_NOT_USE;//0
	}
    for( k = 0; k< MAX_GROUP_NUMBER; k++)
    {       
		memset( g_szGroupName[k], 0, MAX_GROUP_NAME + 1);
        g_GroupOrder[k]         = 0;
    }
    for( k = 0; k< MAX_BOOK_NUMBER; k++)
    {	       
		memset( g_szPersonName[k], 0, MAX_BOOK_NAME + 1);
        g_PersonGroup[k]        =  0; //BOOK_NOT_USE
        g_PersonMultiGroup[k]   =  0;
        for( i=0; i<PERSONAL_NUMBER_LIMIT; i++)
            g_PersonNumberType[k][i] = BOOK_NOT_USE;
    }

    g_nGroup = 0;
    GroupInfo.address = active_list.group_info;  
	nAddress = GroupInfo.address;
    k = 0;
    while( ui_get_pb( PB_GROUP_INFO_I, &GroupInfo) > 0 )
    {
		if( GroupInfo.group_name[0] != 0 && GroupInfo.address == nAddress)
		{
			if( g_szGroupName[GroupInfo.address-1][0] == 0 )
			{
				STRCPY(g_szGroupName[GroupInfo.address-1], GroupInfo.group_name);
				g_GroupOrder[GroupInfo.address-1] = ++g_nGroup;         // order of group
				if( GroupInfo.status == SECRET_GROUP )
				{
					g_GroupOrder[GroupInfo.address-1] =  -g_GroupOrder[GroupInfo.address-1] ;  // secret group
				}
				GroupInfo.address = GroupInfo.next_index;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}		
		k++;     
		nAddress = GroupInfo.address;
        if( g_nGroup > MAX_GROUP_NUMBER || k > MAX_GROUP_NUMBER )
            break;   // 최대 그룹 수 보다 많은 그룹이 있는 경우
    }  


	if( g_nGroup == 0 )
	{
		INT16 nNextGroup = 0;
		for( k=MAX_GROUP_NUMBER; k > 0; k-- )
		{
			GroupInfo.address = k;
			ui_get_pb( PB_GROUP_INFO_I, &GroupInfo);
			if( GroupInfo.group_name[0] != 0 )
			{				
				GroupInfo.next_index = nNextGroup;
				ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
				nNextGroup = GroupInfo.address;
				g_nGroup++;
				STRCPY(g_szGroupName[GroupInfo.address-1], GroupInfo.group_name);				
				if( GroupInfo.status == SECRET_GROUP )
				{
					g_GroupOrder[GroupInfo.address-1] = -g_nGroup ;  // secret group
				}
				else
				{
					g_GroupOrder[GroupInfo.address-1] = g_nGroup;
				}
			}
		}

		if( g_nGroup > 0 )
		{
			active_list.group_info = nNextGroup;
            for( k=0; k< MAX_GROUP_NUMBER; k++)
			{
				if( g_szGroupName[k][0] != 0 )
				{
					if( g_GroupOrder[k] < 0 )
					{
						g_GroupOrder[k] = -( g_nGroup+1 + g_GroupOrder[k] );
					}
					else
					{
						g_GroupOrder[k] = ( g_nGroup + 1 - g_GroupOrder[k] );
					}
				}
			}
		}
		else
		{			
			init_pb_node( PB_GROUP_INFO_I, &GroupInfo);
			for( k=1; k < 6; k++)
			{
				STRCPY( g_szGroupName[k-1], SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM1 + k - 1) );
				g_GroupOrder[k-1] = k;     // non secret group
				STRCPY( GroupInfo.group_name, g_szGroupName[k-1]);
				GroupInfo.address    = k;
				GroupInfo.next_index = k+1;
				ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);

			}
			STRCPY( g_szGroupName[k-1], SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) );
			g_GroupOrder[k-1] = k;         // non secret group
			STRCPY( GroupInfo.group_name, g_szGroupName[k-1]);
			GroupInfo.address    = k;
			GroupInfo.next_index = 0;
			ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);		
			active_list.group_info = 1;			
			g_nGroup =  k;
		}
	}

	nAddress = SearchGroup( SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) );  //지정안됨 그룹 
	if( nAddress == 0 )
	{
		pb_group_info_type NotAssignGroup;
		for(k=1; k<=MAX_GROUP_NUMBER; k++)
		{
			if( GET_GROUP_ORDER(k) == g_nGroup )
			{
				break;
			}
		}

		if( k <= MAX_GROUP_NUMBER )
		{
			GroupInfo.address = k;
			ui_get_pb( PB_GROUP_INFO_I, &GroupInfo); //마지막 그룹 
			init_pb_node( PB_GROUP_INFO_I, &NotAssignGroup);
			NotAssignGroup.address = alloc_pb_node(PB_GROUP_INFO_I);
			g_nGroup++;
			STRCPY( g_szGroupName[NotAssignGroup.address-1], SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) );
			g_GroupOrder[NotAssignGroup.address-1] = g_nGroup;
			STRCPY( NotAssignGroup.group_name, SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6));

			GroupInfo.next_index = NotAssignGroup.address;
			ui_put_pb( PB_GROUP_INFO_I, &GroupInfo);
			ui_put_pb( PB_GROUP_INFO_I, &NotAssignGroup);
			nAddress = NotAssignGroup.address; // 지정안됨 그룹 
		}
	}
	//ASSERT(active_list.nFreeGroup == MAX_GROUP_NUMBER - g_nGroup);
	active_list.nFreeGroup = MAX_GROUP_NUMBER - g_nGroup;
	
	pfaUsed = (BOOL*) SKY_Malloc( sizeof(BOOL)* MAX_PHONE_NUMBER);
	memset(pfaUsed,0, MAX_PHONE_NUMBER);
	count        = 0;
	number_count = 0;
	for( k = 1; k<= MAX_BOOK_NUMBER; k++)
	{
		BookInfo.address = k;        
        ui_get_pb( PB_BOOK_INFO_I, &BookInfo);

	    if(	BookInfo.name[0] != 0 && BookInfo.address == k )
		{  
			if( STRLEN(BookInfo.name) > MAX_BOOK_NAME )
				continue;
			if( IsValid( PB_GROUP_INFO_I, BookInfo.group_address) == FALSE ||\
				g_szGroupName[BookInfo.group_address-1][0] == 0 )
			{
				BookInfo.group_address = nAddress ; //SearchGroup( SKY_GET_TOKEN(TKN_PBOOK_GROUP_ITEM6) );
				ui_put_pb( PB_BOOK_INFO_I, &BookInfo);
			}
			count++;			
			STRCPY( g_szPersonName[BookInfo.address-1], BookInfo.name );
			g_PersonGroup[BookInfo.address - 1]         = BookInfo.group_address;
			g_PersonMultiGroup[BookInfo.address - 1]    = BookInfo.multi_group;

			i = 0;	
			j = 0;
			NumberInfo.address = BookInfo.number_address;
			while( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) > 0 )  // read number info
			{
				if( pfaUsed[NumberInfo.address-1] == TRUE )
				{
					break;
				}
				if( NumberInfo.book_address == BookInfo.address )
				{
					pfaUsed[NumberInfo.address-1] = TRUE;
					number_count++;
					g_PersonNumberType[BookInfo.address-1][i++] = NumberInfo.number_type;
					NumberInfo.address = NumberInfo.next_index;
					if( i >=  PERSONAL_NUMBER_LIMIT )
					{
						//SKY_ASSERT(0);
						break;   // 최대로 가질수 있는 number 보다 많은 number를 가진 경우 
					}
				}
			    else
				{	
					j++;
					if( j >= PERSONAL_NUMBER_LIMIT )
						break;
					free_pb_node( PB_NUMBER_INFO_I, NumberInfo.address );
					NumberInfo.address = NumberInfo.next_index;
				}
			}
		}
	}

	for( k=1; k<= MAX_SDIAL_NUMBER; k++)
	{
		SdialInfo.address = k;
	    ui_get_pb( PB_SDIAL_I, &SdialInfo);
		NumberInfo.address = SdialInfo.dial_address;
		if( ui_get_pb( PB_NUMBER_INFO_I, &NumberInfo) > 0 )
		{
			if( NumberInfo.digits[0] != 0 )
			{
				g_SDialNumber[k-1] = NumberInfo.address;
			}
			else
			{
				SdialInfo.dial_address = BOOK_NOT_USE;
				ui_put_pb( PB_SDIAL_I, &SdialInfo);
			}
		}
	}	
	active_list.nFreeBook = MAX_BOOK_NUMBER - count;

	if( active_list.nFreeNumber != MAX_PHONE_NUMBER - number_count )
	{	
#ifndef WIN32
		INT16* pWord;
		INT16  Number_Index;

		active_list.nFreeNumber = MAX_PHONE_NUMBER - number_count;

		pWord = (INT16*) sByteBuffer;

        k = MAX_PHONE_NUMBER;
		nFreeAddress = 0;
		count = (MAX_PHONE_NUMBER)/ADDRESS_TABLE_BUFFER - 1;//5;  //ADDRESS_TABLE_BUFFER = 200
		while( k > 0 && count >= 0 )
		{	
			memset(sByteBuffer,0,ADDRESS_TABLE_BUFFER*sizeof(INT16));
			for(j=ADDRESS_TABLE_BUFFER-1; j >= 0; j--)
			{				
				if( pfaUsed[k-1] == 0 )
				{
					pWord[j] = nFreeAddress;
					nFreeAddress = k;
				}
				k--;
			}		
			Number_Index = NUMBER_INFO_ALLOC_POS  + count*ADDRESS_TABLE_BUFFER*sizeof(INT16);
			fs_seek( BookSysFile.open.handle, FS_SEEK_SET, Number_Index,NULL,&rsp_msg);	
			fs_write( BookSysFile.open.handle,pWord,ADDRESS_TABLE_BUFFER*sizeof(INT16),NULL,&rsp_msg);
			count--;
		}
		active_list.free_number_info = nFreeAddress;
#else
      
		INT16* pWord;
		INT16  Number_Index;

		active_list.nFreeNumber = MAX_PHONE_NUMBER - number_count;

		pWord = (INT16*) sByteBuffer;

        k = MAX_PHONE_NUMBER;
		nFreeAddress = 0;
		count = (MAX_PHONE_NUMBER)/ADDRESS_TABLE_BUFFER - 1;//5;  //ADDRESS_TABLE_BUFFER = 200
		while( k > 0  && count >= 0 )
		{	
			//memset(sByteBuffer,0,500);
			memset(sByteBuffer,0,ADDRESS_TABLE_BUFFER*sizeof(INT16));
			for(j=ADDRESS_TABLE_BUFFER-1; j >= 0; j--)
			{				
				if( pfaUsed[k-1] == 0 )
				{
					pWord[j] = nFreeAddress;
					nFreeAddress = k;
				}
				k--;
			}
			//Number_Index = NUMBER_INFO_ALLOC_POS + count*200*sizeof(INT16);
			Number_Index = NUMBER_INFO_ALLOC_POS  + count*ADDRESS_TABLE_BUFFER*sizeof(INT16);
			fseek( BookSysFile,  Number_Index, SEEK_SET );
			fwrite(pWord,ADDRESS_TABLE_BUFFER*sizeof(INT16),1, BookSysFile);
			count--;
		}
		active_list.free_number_info = nFreeAddress;	
#endif

	}

#if 0
	memset(pfaUsed,0, MAX_EMAIL_NUMBER);
	pEmailInfo = (pb_email_info_type*) SKY_Malloc( sizeof(pb_email_info_type) * 10 );
	nFreeAddress = 0;
	count = 0;
	for( k= MAX_EMAIL_NUMBER-10; k> 0; k-= 10 )
	{
		Read_email( pEmailInfo, k, 10 );
		for(i=10; i>0; i--)
		{
			if( IsValid(PB_BOOK_INFO_I, pEmailInfo[i].book_address ) > 0 && g_szPersonName[pEmailInfo[i].book_address-1][0] != 0 )
			{
				count++;
			}
			else
			{
				pb_put_address( PB_EMAIL_INFO_I, k+i, nFreeAddress);
				nFreeAddress = k+i;
			}
		}
	}
	active_list.free_email_info = nFreeAddress;
	active_list.nFreeEMail = MAX_EMAIL_NUMBER - count;

	memset(pfaUsed,0, MAX_ANNIV_NUMBER);
	pAnnivInfo = (pb_anniv_info_type*) SKY_Malloc( sizeof(pb_anniv_info_type) * 10 );
	nFreeAddress = 0;
	count = 0;
	for( k= MAX_ANNIV_NUMBER-10; k> 0; k-= 10 )
	{
		Read_email( pAnnivInfo, k, 10 );
		for(i=10; i>0; i--)
		{
			if( IsValid( PB_BOOK_INFO_I, pAnnivInfo[i].book_address ) > 0 && g_szPersonName[pAnnivInfo[i].book_address-1][0] != 0 )
			{
				count++;
			}
			else
			{
				pb_put_address( PB_ANNIV_INFO_I, k+i, nFreeAddress);
				nFreeAddress = k+i;
			}
		}
	}
	active_list.free_anniv_info = nFreeAddress;
	active_list.nFreeAnniv = MAX_ANNIV_NUMBER - count;

	SKY_Free( pAnnivInfo );
	SKY_Free( pEmailInfo );
	SKY_Free( pfaUsed );
#endif		
	PhoneBookUpdate();
}

#ifndef WIN32
BOOL PBookInit(PB_Type  booktype)
{  
    INT16 i,k;
    INT16 BookInit_Index = 0;          
    fs_rsp_msg_type        rsp_msg;

    switch( booktype )
    {
    case PB_SDIAL_I:
        {
            pb_sdial_type sdial[MAX_SDIAL_NUMBER];        
            for(i=0; i<MAX_SDIAL_NUMBER; i++)
            {
                init_pb_node( PB_SDIAL_I, &sdial[i]);
                sdial[i].address = i+1;               
            }               
            fs_seek_write( BookSysFile.open.handle, FS_SEEK_SET,SDIAL_POS, \
                           sdial,sizeof(pb_sdial_type)*MAX_SDIAL_NUMBER,NULL,&rsp_msg);       
        }
        break;
    case PB_INFO_I:
        {
            pb_info_type PersonalInfo;
			memset(&PersonalInfo, 0, sizeof(PersonalInfo) );
            PersonalInfo.MobileTel[0] = BOOK_NOT_USE;
            PersonalInfo.HomeTel[0]   = BOOK_NOT_USE;
            PersonalInfo.Name[0]      = BOOK_NOT_USE;
            PersonalInfo.Address[0]   = BOOK_NOT_USE;
			PersonalInfo.Email[0]     = BOOK_NOT_USE;
            //PersonalInfo.nDay         = 1;
            //PersonalInfo.nMonth       = 1;
            //PersonalInfo.wYear        = 2001;
            //PersonalInfo.fLunar       = FALSE;

            ui_put_pb(PB_INFO_I, &PersonalInfo);
        }
        break;

    case PB_ACTIVE_LIST_I:        
        active_list.group_info    = 0;
        active_list.number_info   = 0;
        active_list.email_info    = 0;
        for(i=0;i<12;i++)
        {
            active_list.anniv_info[i] = 0;
        }
        for(i=0;i<10;i++)
        {
            active_list.number_search[i] = 0;
        }
        active_list.number_info_limit = 0;

        active_list.free_number_info   = 1;
        active_list.free_anniv_info    = 1;
        active_list.free_email_info    = 1;
        active_list.nFreeSdial    = MAX_SDIAL_NUMBER;
        active_list.nFreeGroup    = MAX_GROUP_NUMBER;
        active_list.nFreeBook     = MAX_BOOK_NUMBER;
        active_list.nFreeNumber   = MAX_PHONE_NUMBER;
        active_list.nFreeEMail    = MAX_EMAIL_NUMBER;
        active_list.nFreeAnniv    = MAX_ANNIV_NUMBER;
		active_list.nFreeMemo     = MAX_BOOK_NUMBER;
		active_list.nFreePicture  = MAX_BOOK_NUMBER;

        ui_put_pb( PB_ACTIVE_LIST_I ,(pb_item_type*) &active_list );

        break;

    case PB_NUMBER_INFO_I:
        {
            INT16  INT16_address_index[INT16_BUFFER_SIZE];

            k = sizeof(pb_number_info_type);
            k = 0;
            BookInit_Index = NUMBER_INFO_ALLOC_POS; 
            fs_seek( BookSysFile.open.handle, FS_SEEK_SET,BookInit_Index,NULL,&rsp_msg);
            for(i=1;i< MAX_PHONE_NUMBER ;i++)    
            {
                INT16_address_index[(i-1)%INT16_BUFFER_SIZE] = i+1;
                if( i > 1 && i%INT16_BUFFER_SIZE == 0 )
                {
                    k+=INT16_BUFFER_SIZE;
                    fs_write( BookSysFile.open.handle,INT16_address_index,INT16_BUFFER_SIZE*sizeof(INT16),NULL,&rsp_msg);
                }                                           
            }
            INT16_address_index[(i-1)%INT16_BUFFER_SIZE] = 0;
            if( i% INT16_BUFFER_SIZE == 0 )
            {
                fs_write( BookSysFile.open.handle, INT16_address_index,INT16_BUFFER_SIZE*sizeof(INT16),NULL,&rsp_msg);
                k+=INT16_BUFFER_SIZE;
            }
            else
            {
                fs_write( BookSysFile.open.handle,INT16_address_index,(i%INT16_BUFFER_SIZE)*sizeof(INT16),NULL,&rsp_msg);
                k+= i%BYTE_BUFFER_SIZE;
            }
        }
        break;

    case PB_ANNIV_INFO_I:
        {
            INT16  INT16_address_index[INT16_BUFFER_SIZE];
            k = sizeof(pb_anniv_info_type);
            k = 0;
            BookInit_Index = ANNIV_INFO_ALLOC_POS; 
            fs_seek( BookSysFile.open.handle, FS_SEEK_SET,BookInit_Index,NULL,&rsp_msg);
            for(i=1;i< MAX_ANNIV_NUMBER ;i++)    
            {
                INT16_address_index[(i-1)%INT16_BUFFER_SIZE] = i+1;
                if( i > 1 && i%INT16_BUFFER_SIZE == 0 )
                {
                    k+=INT16_BUFFER_SIZE;               
                    fs_write( BookSysFile.open.handle,INT16_address_index,INT16_BUFFER_SIZE*sizeof(INT16),NULL,&rsp_msg);
                }                                           
            }
            INT16_address_index[(i-1)%INT16_BUFFER_SIZE] = 0;
            if( i% INT16_BUFFER_SIZE == 0 )
            {
                fs_write( BookSysFile.open.handle, INT16_address_index,INT16_BUFFER_SIZE*sizeof(INT16),NULL,&rsp_msg);
                k+=INT16_BUFFER_SIZE;
            }
            else
            {
                fs_write( BookSysFile.open.handle,INT16_address_index,(i%INT16_BUFFER_SIZE)*sizeof(INT16),NULL,&rsp_msg);
                k+= i%BYTE_BUFFER_SIZE;
            }
        }
        break;

    case PB_EMAIL_INFO_I:
        {
            INT16  INT16_address_index[INT16_BUFFER_SIZE];
            k = sizeof(pb_email_info_type);
            k = 0;
            BookInit_Index = EMAIL_INFO_ALLOC_POS; 
            fs_seek( BookSysFile.open.handle, FS_SEEK_SET,BookInit_Index,NULL,&rsp_msg);
            for(i=1;i< MAX_EMAIL_NUMBER ;i++)    
            {
                INT16_address_index[(i-1)%INT16_BUFFER_SIZE] = i+1;
                if( i > 1 && i%INT16_BUFFER_SIZE == 0 )
                {
                    k+=INT16_BUFFER_SIZE;               
                    fs_write( BookSysFile.open.handle,INT16_address_index,INT16_BUFFER_SIZE*sizeof(INT16),NULL,&rsp_msg);
                }                                           
            }
            INT16_address_index[(i-1)%INT16_BUFFER_SIZE] = 0;
            if( i% INT16_BUFFER_SIZE == 0 )
            {
                fs_write( BookSysFile.open.handle, INT16_address_index,INT16_BUFFER_SIZE*sizeof(INT16),NULL,&rsp_msg);
                k+=INT16_BUFFER_SIZE;
            }
            else
            {
                fs_write( BookSysFile.open.handle,INT16_address_index,(i%INT16_BUFFER_SIZE)*sizeof(INT16),NULL,&rsp_msg);
                k+= i%BYTE_BUFFER_SIZE;
            }
        }
        break; 
    default:
        FALSE;
        break;
    }
    return TRUE;
}
#else

BOOL PBookInit(PB_Type  booktype)
{

    pb_sdial_type            sdial;
    INT16 i, index;


    switch( booktype )
    {
    case PB_SDIAL_I:
        init_pb_node( PB_SDIAL_I, &sdial);
        for(i=1; i<=MAX_SDIAL_NUMBER; i++)
        {
            sdial.address = (UINT8) i;          
            ui_put_pb( PB_SDIAL_I ,(pb_item_type*) &sdial );
			g_SDialNumber[i-1] = 0; //BOOK_NOT_USE;
        }
        break;
    case PB_INFO_I:
        {
            pb_info_type PersonalInfo;
			memset(&PersonalInfo, 0, sizeof(PersonalInfo) );
            PersonalInfo.MobileTel[0] = BOOK_NOT_USE;
            PersonalInfo.HomeTel[0]   = BOOK_NOT_USE;
            PersonalInfo.Name[0]      = BOOK_NOT_USE;
//            PersonalInfo.PostNum[0]   = BOOK_NOT_USE;
            PersonalInfo.Address[0]   = BOOK_NOT_USE;
			PersonalInfo.Email[0]     = BOOK_NOT_USE;

            //PersonalInfo.nDay         = 1;
            //PersonalInfo.nMonth       = 1;
            //PersonalInfo.wYear        = 2001;
            //PersonalInfo.fLunar       = FALSE;

            ui_put_pb(PB_INFO_I, &PersonalInfo);
        }
        break;
    case PB_ACTIVE_LIST_I:
        active_list.group_info    = 0;
        active_list.number_info   = 0;        
        active_list.email_info     = 0;
        for(i=0;i<12;i++)
        {
            active_list.anniv_info[i] = 0;
        }
        for(i=0;i<10;i++)
        {
            active_list.number_search[i] = 0;
        }
        active_list.number_info_limit = 0;

        active_list.free_number_info   = 1;
        active_list.free_anniv_info    = 1;
        active_list.free_email_info     = 1;
        active_list.nFreeSdial    = MAX_SDIAL_NUMBER;
        active_list.nFreeGroup    = MAX_GROUP_NUMBER;
        active_list.nFreeBook     = MAX_BOOK_NUMBER;
        active_list.nFreeNumber   = MAX_PHONE_NUMBER;
        active_list.nFreeEMail    = MAX_EMAIL_NUMBER;
        active_list.nFreeAnniv    = MAX_ANNIV_NUMBER;;
		active_list.nFreeMemo     = MAX_BOOK_NUMBER;
		active_list.nFreePicture  = MAX_BOOK_NUMBER;

        ui_put_pb( PB_ACTIVE_LIST_I ,(pb_item_type*) &active_list );

        break;

    case PB_NUMBER_INFO_I:
        index = NUMBER_INFO_ALLOC_POS;
        //k=fseek( BookSysFile,  index, SEEK_SET );
        for(i=1;i<= MAX_PHONE_NUMBER-1;i++)
        {
            //NumberTable[i] = i+2;
            pb_put_address( PB_NUMBER_INFO_I ,  i , i+1 );
        }
        //NumberTable[i] = 0;
        //k=fwrite((void*)&NumberTable,MAX_PHONE_NUMBER*sizeof(BYTE),1, BookSysFile);  
        pb_put_address( PB_NUMBER_INFO_I ,  i , 0);

        break;

    case PB_ANNIV_INFO_I:
        index = ANNIV_INFO_ALLOC_POS;
        //k=fseek( BookSysFile,  index, SEEK_SET );
        for(i=1;i<= MAX_ANNIV_NUMBER-1;i++)
        {
            //AnnivTable[i] = i+2;
            pb_put_address( PB_ANNIV_INFO_I ,  i , i+1 );
        }
        //AnnivTable[i] = 0;
        //k=fwrite((void*)&AnnivTable,MAX_ANNIV_NUMBER*sizeof(BYTE),1, BookSysFile);  
        pb_put_address( PB_ANNIV_INFO_I ,  i , 0);

        break;

    case PB_EMAIL_INFO_I:
        index = EMAIL_INFO_ALLOC_POS;
        //k=fseek( BookSysFile,  index, SEEK_SET );
        for(i=1;i<= MAX_EMAIL_NUMBER-1;i++)
        {
            //MemoTable[i] = i+2;
            pb_put_address( PB_EMAIL_INFO_I ,  i , i+1 );
        }
        //MemoTable[i] = 0;
        //k=fwrite((void*)&MemoTable,MAX_EMAIL_NUMBER*sizeof(BYTE),1, BookSysFile);  
        pb_put_address( PB_EMAIL_INFO_I ,  i , 0);

        break;  
              
    default:
        return FALSE;
        break;
    }
    return TRUE;
}
#endif


BOOL     IsValidSDial(UINT8 Address)
{
    pb_sdial_type sdial;

    if( Address == 0 || Address > MAX_SDIAL_NUMBER)
        return FALSE;
    sdial.address = Address;
    ui_get_pb( PB_SDIAL_I, &sdial);
    if( IsValid( PB_NUMBER_INFO_I, sdial.dial_address) == TRUE)
        return TRUE;
    else
        return FALSE;
       
}
BOOL IsFullPBook(void)
{
/*
    if( active_list.nFreeBook == 0 )
		return TRUE;
	if( active_list.nFreeNumber < PERSONAL_NUMBER_LIMIT )
		return TRUE;
	if( active_list.nFreeEMail   < PERSONAL_MEMO_LIMIT   )
		return TRUE;
	if( active_list.nFreeAnniv  < PERSONAL_ANNIV_LIMIT  )
		return TRUE;
*/
	if( active_list.nFreeBook == 0 || active_list.nFreeNumber  == 0  )
		return TRUE;	
	return FALSE;
}

BOOL IsFullGroup(void)
{
	//if( active_list.nFreeGroup == 0 )
	if( g_nGroup >= MAX_GROUP_NUMBER )
		return TRUE;
	else
		return FALSE;
}