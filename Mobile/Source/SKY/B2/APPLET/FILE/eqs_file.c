/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_file.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-10-10 redstar     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"
#include "eqs_file.h"

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

char cSystemFile[MAX_SYSTEM_FILE][20] =
{    
    "state.sys",
    "callinfo.sys",
    "callstack.sys"
};
DWORD dwSystemFile[MAX_SYSTEM_FILE] =
{
    sizeof(SYSTEM_STATE_T),
    sizeof(SYSTEM_CALLINFO_T),
    sizeof(SYSTEM_CALLSTACK_T)
};
#ifndef WIN32

fs_rsp_msg_type                  SystemFile[MAX_SYSTEM_FILE];

#else
#include <stdio.h>
FILE*                            SystemFile[MAX_SYSTEM_FILE];

#endif

/*
BOOL EQS_OpenData ( SYSTEM_CONFIG_E  Index )
{    

#ifndef WIN32
    fs_open_xparms_type    open_option;


    fs_open((char*)cSystemFile[Index], FS_OA_READWRITE, &open_option, NULL,&SystemFile[Index]);  
    if( SystemFile[Index].open.handle == 0 )
    {
        fs_open((char*)cSystemFile[Index], FS_OA_CREATE, &open_option, NULL, &SystemFile[Index]);
        if( SystemFile[Index].open.handle == 0 )
            return FALSE;
    }
    
#else
    if( (SystemFile[Index] = fopen((char*)cSystemFile[Index], "rb+" )) == NULL )
    {
        if( (SystemFile[Index] = fopen((char*)cSystemFile[Index], "wb+")) == NULL )
            return FALSE;
    }

#endif
    return TRUE;

}

BOOL EQS_CloseData( SYSTEM_CONFIG_E  Index )
{
#ifndef WIN32
    fs_rsp_msg_type        close_rsp_msg; 
    fs_close(SystemFile[Index].open.handle, NULL, &close_rsp_msg);
#else
    fclose(SystemFile[Index]); 
#endif
    return TRUE;

}
*/

BOOL EQS_DataFileSetup(BOOL fReset)
{
    SYSTEM_STATE_T     State;
    SYSTEM_CALLINFO_T  CallInfo;
    SYSTEM_CALLSTACK_T Stack;
    DWORD dwFileSize;

#ifndef WIN32
    fs_rsp_msg_type    rsp_msg;
        
    dwFileSize = fs_space_available();
    fs_file_size(cSystemFile[0], NULL, &rsp_msg );
    dwFileSize = rsp_msg.file_size.size;
#else         
    if( (SystemFile[0] = fopen((char*)cSystemFile[0], "rb+" )) == NULL )
    {
        if( (SystemFile[0] = fopen((char*)cSystemFile[0], "wb+")) == NULL )
            return FALSE;
    }
    fseek(SystemFile[0], 0, SEEK_END );
    dwFileSize = ftell(SystemFile[0]);  
    fclose(SystemFile[0]); 
#endif

   
    if( dwFileSize == 0  || fReset == TRUE)             // initialize permanat variable
    {       
        /*memset(&State,0,sizeof(SYSTEM_STATE_T));
        memset(&CallInfo,0,sizeof(SYSTEM_CALLINFO_T));
        memset(&Stack,0,sizeof(SYSTEM_CALLSTACK_T));          */
        EQS_Memset(&State,0,sizeof(SYSTEM_STATE_T));
        EQS_Memset(&CallInfo,0,sizeof(SYSTEM_CALLINFO_T));
        EQS_Memset(&Stack,0,sizeof(SYSTEM_CALLSTACK_T));          
        EQS_WriteData( SYSTEM_STATE_E, &State );
        EQS_WriteData( SYSTEM_CALLINFO_E, &CallInfo );
        EQS_WriteData( SYSTEM_CALLSTACK_E, &Stack);

        SET_PS_LOCK( FALSE );
        SET_PS_PASSWORD((BYTE*)" ");  
        SET_PS_RESTRICT(0) ;   
        SET_PS_ONETOUCH(FALSE) ;

        SET_NP_CURRENTNAM(1);

        //SET_NP_MIN1(0, pdwValue);
        //SET_NP_MIN2(0, pwValue);

        SET_ED_BLIGHT(100);

        SET_SND_AREABEEP(FALSE);
        SET_SND_FADEBEEP(FALSE);
        SET_SND_MINBEEP(FALSE);
        SET_SND_BURSTDTMF(FALSE);
        SET_SND_ROAMRINGER(FALSE);

        SET_ST_ALERTLVL(5);
        SET_ST_ALERTLVL_SHADOW(5);
        SET_ST_HEADSET_BEEPLVL(5);
        SET_ST_BEEPLVL(5);
        SET_ST_SPKR_BEEPLVL(5);
        SET_ST_EAR_HEADSETLVL(5);
        SET_ST_EARLVL(5);
        SET_ST_EAR_SPKRLVL(5);
        SET_ST_MUSICLVL(5);
        SET_ST_RINGERLVL(5);
        SET_ST_RINGER_SPKRLVL(5);


        SET_CS_AUTOANSWER(FALSE);
        SET_CS_ANSWER(0);

        //SET_CI_LASTDIAL(LastDial);
        SET_CI_RETRY(FALSE);
        SET_CI_RETRYTIME(0);
        SET_CI_HOMECOUNT(0);
        SET_CI_ROAMCOUNT(0);
        SET_CI_TOTALCALLCOUNT(0);
        SET_CI_HOMECALLS(0,0);
        SET_CI_ROAMCALLS(0,0);
        SET_CI_HOMETIMES(0,0);
        SET_CI_ROAMTIMES(0,0);
        SET_CI_TOTALCALLTIME(0);
        SET_CI_TOTALRUNTIME(0);
        SET_CI_RUNTIME(0);


        SET_SMS_UTCTIME(FALSE);

    }
                                                       // initialize non permant variable
    
    SET_PS_PAD( FALSE ) ;        
    SET_PS_IGNITIONTIME(100) ;
    SET_PS_IGNITION(FALSE);
    SET_PS_OPENPHONE(FALSE)   ;
    SET_PS_SYSTIMEVALID(FALSE) ;


    SET_NP_ALLOWMODE(FALSE);
    SET_NP_FORCESO(0);
    SET_NP_CDMA( 0, FALSE);
    SET_NP_ANALOG(0, FALSE);


    SET_ED_HANDYPHONE(FALSE);
    SET_ED_CHARGING(FALSE);
    SET_ED_EXTDEV(FALSE);
    SET_ED_EXTPOWER(FALSE);
    SET_ED_HEADSET(FALSE);
    SET_ED_ONHOOK(FALSE);
    SET_ED_SPEAKER(FALSE);
    SET_ED_BLIGHT1(100);
    SET_ED_BLIGHT2(100);
     

    SET_SND_SHORTBURST(FALSE);
    SET_SND_EDITVOLUME(FALSE);
    SET_SND_MENUVOLUME(FALSE);
    SET_SND_CLEARBEEP(FALSE);


    //SET_ST_KEYDOWNDTMF(KeyDownDTMF)
    //SET_ST_KEYDOWNTONE(KeyDownTone)
    SET_ST_STOPDTMF(0);


    SET_CS_OTASPFAIL(FALSE);
    SET_CS_ALERT(FALSE);
    SET_CS_ROAMCALL(FALSE);
    SET_CS_DIGITAL(FALSE);
    SET_CS_ORIGINATE(FALSE);
    //SET_CS_PHONEINFO(PInfo);
    SET_CS_UPCALL(FALSE);
    SET_CS_TXDTMF(FALSE);
    SET_CS_VOICE(FALSE);
    //SET_CS_CALLERID(CallerID);

    //SET_CI_REDIAL(DialBuf);
    SET_CI_SECRET(FALSE);
    SET_CI_UPDATE(0,FALSE);

    SET_CSK_CHANGE(FALSE);
    //SET_CSK_INDEX(Index);
    //SET_CSK_STACK(nIndex, Dial);

    //SET_SMS_BCCONFIG( BCConfig  );
    //SET_SMS_BCPREF( BCPref  );
    //SET_SMS_BCSVCTABLE( BCTable );
    //SET_SMS_BCMENUINDEX( bIndex );
    //SET_SMS_BCSVCINFO( SVCInfo  );

 
    return TRUE;

}

INT EQS_ReadData(  SYSTEM_CONFIG_E  Index, void* pData )
{
#ifndef WIN32
    fs_rsp_msg_type        rsp_msg;
    fs_open_xparms_type    open_option;

    open_option.create.cleanup_option = FS_OC_CLOSE;
    open_option.create.buffering_option = FS_OB_ALLOW;
    open_option.create.attribute_mask = FS_FA_UNRESTRICTED;


    fs_open((char*)cSystemFile[Index], FS_OA_READWRITE, &open_option, NULL,&SystemFile[Index]);  
    if( SystemFile[Index].open.handle == 0 )
    {
        fs_open((char*)cSystemFile[Index], FS_OA_CREATE, &open_option, NULL, &SystemFile[Index]);
        if( SystemFile[Index].open.handle == 0 )
            return 0;
    }
    fs_read( SystemFile[Index].open.handle, pData, dwSystemFile[Index], NULL, &rsp_msg );
    fs_close(SystemFile[Index].open.handle, NULL, &rsp_msg);
    
#else
    if( (SystemFile[Index] = fopen((char*)cSystemFile[Index], "rb+" )) == NULL )
    {
        if( (SystemFile[Index] = fopen((char*)cSystemFile[Index], "wb+")) == NULL )
            return 0;
    }
    fread(pData,dwSystemFile[Index],1, SystemFile[Index]);
    fclose(SystemFile[Index]); 

#endif
    return 1;
}

INT EQS_WriteData( SYSTEM_CONFIG_E  Index, void* pData )
{
#ifndef WIN32

    fs_rsp_msg_type        rsp_msg;
    fs_open_xparms_type    open_option;
    open_option.create.cleanup_option = FS_OC_CLOSE;
    open_option.create.buffering_option = FS_OB_ALLOW;
    open_option.create.attribute_mask = FS_FA_UNRESTRICTED;


    fs_open((char*)cSystemFile[Index], FS_OA_READWRITE, &open_option, NULL,&SystemFile[Index]);  

    if( SystemFile[Index].open.handle == 0 )
    {
        fs_open((char*)cSystemFile[Index], FS_OA_CREATE, &open_option, NULL, &SystemFile[Index]);
        if( SystemFile[Index].open.handle == 0 )
            return 0;
    }
    fs_write(SystemFile[Index].open.handle, pData, dwSystemFile[Index], NULL, &rsp_msg);
    fs_close(SystemFile[Index].open.handle, NULL, &rsp_msg);

#else
    if( (SystemFile[Index] = fopen((char*)cSystemFile[Index], "rb+" )) == NULL )
    {
        if( (SystemFile[Index] = fopen((char*)cSystemFile[Index], "wb+")) == NULL )
            return 0;
    }
    fwrite( pData, dwSystemFile[Index],1, SystemFile[Index] );
    fclose(SystemFile[Index]); 
#endif

    return 1;
}