/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
		MDARM_SYS.C
GENERAL DESCRIPTION
  This Software unit contains all memory allocation functions, file system
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
#include "mdarm_def.h"


/******************************************************************/
/*	   define constans , macro and types                          */
/******************************************************************/
#define	FD_HASH(x)	((x) & 0x1f)
#define	FD_FULL		(fdCount >= MAX_OPEN)


/******************************************************************/
/*	   grobal variables					                          */
/******************************************************************/
static  short        lock_SharedBuffer;
static  qword		RAM gap; /* 1970.1.1 -- 1980.1.6 */
short	fdCount;
int		fdHash[MAX_OPEN];

/******************************************************************/
/*	   extern variables					                          */
/******************************************************************/
extern char		Image$$APP_SHARE_RAM$$Base[];


/******************************************************************/
/*	   static variables					                          */
/******************************************************************/
static char *STATIC_HEAP = (char *)Image$$APP_SHARE_RAM$$Base;
static char *SharedBuffer = (char *)Image$$APP_SHARE_RAM$$Base + STATIC_HEAP_SIZE;

/******************************************************************/
/*	   extern functions					                          */
/******************************************************************/
extern void lcderr(const char *s, int len);

/******************************************************************/
/*     function Declare											  */
/******************************************************************/

/******************************************************************/
/*     function Define											  */
/******************************************************************/

static void fd_init(void)
{
    fdCount = 0;
    memset(fdHash, 0xff, MAX_OPEN * sizeof(int));
}

static void fd_finalize(void)
{
    int	i;
    extern int md_close(int);

    for(i = 0; i < MAX_OPEN; i++)
	if (fdHash[i] != -1)
	    md_close(fdHash[i]);
}

static int fd_find(int sfd, int fd)
{
    int tfd;


    if (fdHash[sfd] == fd) {
		return sfd;
    }
	
    tfd = sfd + SKIP_OPEN;
    if (tfd >= MAX_OPEN)
		tfd -= MAX_OPEN;
    
    while(tfd != sfd) {

        if (fdHash[tfd] == fd) {
            return tfd;
        }

	tfd += SKIP_OPEN;
	if (tfd >= MAX_OPEN)
	    tfd -= MAX_OPEN;
    }
    return -1;
}

static int fd_mark(int fd)
{
    int	tfd;


    if (fd < 0) {
		return -1;
    }
    
    tfd = fd_find(FD_HASH(fd), -1);
    if (tfd < 0) {
		return -1;
    }
    fdCount++;
    fdHash[tfd] = fd;
    return tfd;
}

static int fd_unmark(int fd)
{
    int	tfd;


    if (fd < 0) {
		return -1;
    }
    
    tfd = fd_find(FD_HASH(fd), fd);
    if (tfd < 0){
		return -1;
    }
	fdCount--;
    fdHash[tfd] = -1;
    return tfd;
}
 
void *md_malloc(int *size)
{

    *size = STATIC_HEAP_SIZE;
    return (void *) STATIC_HEAP;
}

void md_free(void *addr)
{
}

#define MSGIllegalSharedBufferState     "IllegalSharedBufferState\n"
void *md_getSharedBuffer(int *size)
{
    if (lock_SharedBuffer) {
		lcderr(MSGIllegalSharedBufferState, strlen(MSGIllegalSharedBufferState));
        return NULL;
    }
    
    *size = SHARED_BUFFER_SIZE;
    lock_SharedBuffer = 1;

    return (void *)SharedBuffer;
}

void md_putSharedBuffer(void *buf)
{
    if (!lock_SharedBuffer || (buf != SharedBuffer)) {
        lcderr(MSGIllegalSharedBufferState, strlen(MSGIllegalSharedBufferState));
        return;
    }
    lock_SharedBuffer = 0;
}

/* FILE SYSTEM */
char *md_ls(const char *dirname, void *buf, int *len)
{
    fs_rsp_msg_type	rsp_msg;
    int	LEN_FILELIST;//, num;
    char *LS_buf;
MSG_ERROR("md_ls called ",0,0,0);    

    if (*len < 4) return NULL;

    LEN_FILELIST = *len;
    LS_buf = (char *) buf;

    LEN_FILELIST--;
    *LS_buf++ = '\0';
    // -1 : NULL at the end of the file list 
    fs_list_dirs(dirname, LS_buf, LEN_FILELIST-1, NULL, &rsp_msg);
    if (rsp_msg.list_dirs.status != FS_OKAY_S) {
        return NULL;
    }
    *len = rsp_msg.list_dirs.count;

    /*num = SKY_GetDirNameList (dirname, LS_buf);
    *len = num;*/
    /* Place double-NULL at the end of the dir list */
    if (*len < 2) {
	    *len = 2;
	    LEN_FILELIST++;
	    LS_buf--;
    }

    fs_list_files(dirname, LS_buf+*len, LEN_FILELIST-*len, NULL, &rsp_msg);
    if (rsp_msg.list_files.status != FS_OKAY_S) {
        return NULL;
    }

    /*num = SKY_GetFileNameList (dirname, LS_buf+*len); */

    // *len += rsp_msg.list_files.count;
    // remove dirname from files : dir/file -> file 
    {
        int	dirlen, flen;
        char *p, *q;
        dirlen = strlen(dirname);
        p = q = LS_buf+*len;
        while (*p != '\0') {
            p += dirlen+1;
            flen = strlen(p);
            memmove(q, p, flen+1);
            p += flen+1;
            q += flen+1;
        }
        *q++ = '\0';
        *len = q - LS_buf;
    }
    // Place double-NULL at the end of the file list 
    // if there is no files, trifle-null 

	// 여기에서 값이 맞게 전달되는지 찍어보도록 하자.
	return (char*)LS_buf;
}


int md_mkdir(const char *name)
{

    fs_rsp_msg_type	rsp_msg;
    
    fs_mkdir(name,/* Name of new directory */
	    NULL,		/* Callback function ptr */
	    &rsp_msg);		/* Response message ptr */

    if (rsp_msg.mkdir.status == FS_OKAY_S)
	return 0;
    return -1;
/*
    fs_rsp_msg_type	rsp_msg;
  
	char dirname[FS_FILENAME_MAX_LENGTH];
	char* pfind;
	char name_new[FS_FILENAME_MAX_LENGTH];
	int namesize,i,j;
		
	memset((void*)name_new,0x00,sizeof(name_new));
	memcpy(name_new, name, strlen(name));

    for (namesize = 0 ; namesize < FS_FILENAME_MAX_LENGTH ;namesize++) {
        if (name_new[namesize] == NULL)
            break;
    }

	if (name_new[namesize-1] != FS_DIRECTORY_CHAR) {
		name_new[namesize] = FS_DIRECTORY_CHAR;
		namesize++;
		//name_new[++namesize] = NULL;
	}

    j=0;	
    memset((void*)dirname,0x00,sizeof(dirname));
    for (i=0 ; i < namesize ;i++) {
        if (name_new[i] == FS_DIRECTORY_CHAR) {
            // directory create 
// SKY_IsFileOrDirExist(..)에서 FATAL ERROR가 발생되는 경우가 있어서 
// fs_test을 직접 call하기로 함. 
//			if(!SKY_IsFileOrDirExist((CONST BYTE*)dirname)) { 
			fs_test(dirname, NULL, &rsp_msg);
			if (rsp_msg.nametest.status == FS_OKAY_S)
			{
				// FILE NAME으로 존재하지 않을때 
				if (!rsp_msg.nametest.name_found)
				{
					memset((void*)&rsp_msg,NULL,sizeof(fs_rsp_msg_type));
					fs_mkdir(dirname,
							NULL,		
							&rsp_msg);	


					if (rsp_msg.mkdir.status != FS_OKAY_S) {
						return -1;
					}
				}
  
            }
			else
			{
				MSG_ERROR("fs_mkdir=> Fatal error occur!",0,0,0);
				return -1;
			} 
            dirname[j++] = FS_DIRECTORY_CHAR;
        }
        else if (name_new[i] == NULL) {
            break;	
        }
        else {
            dirname[j++] = name_new[i];
        }
    }
    return 0;
*/
}

int md_rmdir(const char *name)
{
    fs_rsp_msg_type	rsp_msg;
    
	fs_rmdir(name,/* Name of new directory */
	    NULL,		/* Callback function ptr */
	    &rsp_msg);		/* Response message ptr */

    if (rsp_msg.rmdir.status == FS_OKAY_S) 
    {
        return 0;
    }
    return -1;
}

int md_close(int fd)
{
	fs_rsp_msg_type rsp_msg;
    int romfile_close(void);

    if (fd == ROMFILEFD) {
        return romfile_close();
    }

    fs_close(fd,		/* File handle */
	    NULL,		/* Callback function ptr */
	    &rsp_msg);		/* Response message ptr */
    
    if (rsp_msg.close.status == FS_OKAY_S) 
    {
	    fd_unmark(fd);
        return 0;
    }
    return -1;
}


int md_ropen(const char *pathname)
{
    fs_rsp_msg_type	rsp_msg;
    int romfile_open(void);

	if (ISROMFILENAME(pathname)) 
    {
        if (romfile_open() == 0) 
        {
			return ROMFILEFD;
        }
        else 
        {
			return -1;
        }
	}
    if (FD_FULL) 
    {
		return -1;
    }
    
    fs_open(pathname,	// Name of file to open 
	    FS_OA_READONLY,	// Open mode 
	    NULL,		// No optional parameters
	    NULL,		// Callback function ptr 
	    &rsp_msg);		// Response message ptr 
    
    if (rsp_msg.open.handle == FS_NULL_HANDLE) 
    {
		return -1;
    }
    fd_mark(rsp_msg.open.handle);
    return ROMSAFE_FD(rsp_msg.open.handle);
}

int md_wopenc(const char *pathname)
{
    fs_rsp_msg_type	rsp_msg;
    fs_open_xparms_type fs_create_xparms;  // Optional open parameters 
    
    if (FD_FULL)
    {
		return -1;
    }

    fs_create_xparms.truncate.position = 0;
    fs_open(pathname,  // Name of file to open 
	    FS_OA_TRUNCATE,            // Open mode 
	    &fs_create_xparms,            // Optional parameters 
	    NULL,                      // Callback function ptr
	    &rsp_msg);                 // Response message ptr 

    if (rsp_msg.open.handle != FS_NULL_HANDLE) 
    {
		fd_mark(rsp_msg.open.handle);
		return ROMSAFE_FD(rsp_msg.open.handle);
    }

    // file doesn't exist, create it
    /*if (md_fsavail() > 0) 
    {*/
        fs_create_xparms.create.cleanup_option = FS_OC_CLOSE;
        fs_create_xparms.create.buffering_option = FS_OB_ALLOW;
        fs_create_xparms.create.attribute_mask = FS_FA_UNRESTRICTED;
        fs_create_xparms.create.base_address = 0;

        fs_open(pathname,  // Name of file to open 
	        FS_OA_CREATE,                 // Open mode 
	        &fs_create_xparms,            // Optional parameters 
	        NULL,                         // Callback function ptr 
	        &rsp_msg);               // Response message ptr 

        if (rsp_msg.open.handle == FS_NULL_HANDLE) 
        {
		    return -1;
        }
        fd_mark(rsp_msg.open.handle);
        return ROMSAFE_FD(rsp_msg.open.handle);     
    /*}
    else // 남은 용량이 없는 경우
        return -1;*/
}


int md_rwopenc(const char *pathname)
{
    fs_rsp_msg_type	rsp_msg;
    fs_open_xparms_type fs_create_xparms;  // Optional open parameters 
    
    if (FD_FULL) 
    {
		return -1;
    }

    fs_open(pathname,  // Name of file to open 
	    FS_OA_READWRITE,           // Open mode 
	    NULL,                      // No optional parameters
	    NULL,                      // Callback function ptr 
	    &rsp_msg);                 // Response message ptr 

    if (rsp_msg.open.handle != FS_NULL_HANDLE) 
    {
		fd_mark(rsp_msg.open.handle);
		return ROMSAFE_FD(rsp_msg.open.handle);
    }

    // file doesn't exist, create it 
    /*if (md_fsavail() > 0) 
    {*/
        fs_create_xparms.create.cleanup_option = FS_OC_CLOSE;
        fs_create_xparms.create.buffering_option = FS_OB_ALLOW;
        fs_create_xparms.create.attribute_mask = FS_FA_UNRESTRICTED;
        fs_create_xparms.create.base_address = 0;

        fs_open(pathname,  // Name of file to open 
	        FS_OA_CREATE,                 // Open mode
	        &fs_create_xparms,            // Optional parameters 
	        NULL,                         // Callback function ptr 
	        &rsp_msg);               // Response message ptr 

        if (rsp_msg.open.handle == FS_NULL_HANDLE) 
        {
		    return -1;
        }
        fd_mark(rsp_msg.open.handle);
	    return ROMSAFE_FD(rsp_msg.open.handle);
    /*}
    else // 남은 용량이 없는 경우
        return -1;*/
}

int md_read(int fd, void *buf, int count)
{
    fs_rsp_msg_type rsp_msg;
    int romfile_read(void *buf, int len);

    if (fd == ROMFILEFD) 
    {
		return romfile_read(buf, count);
    }

    fs_read( fd,  // file handle 
	    buf,   // source to store bytes 
	    count,    // # bytes to read
	    NULL,     // callback_function 
	    &rsp_msg); // message ptr

    if (rsp_msg.read.status == FS_OKAY_S)
    {
		return rsp_msg.read.count;
    }
    return -1;
}

int md_write(int fd, const void *buf, int count)
{
    fs_rsp_msg_type rsp_msg;

    fs_write( fd,  // file handle
	    buf,   // source to store bytes
	    count,    // # bytes to write 
	    NULL,     // callback_function 
	    &rsp_msg); // message ptr 

    if(rsp_msg.write.status == FS_OKAY_S) 
    {
        return rsp_msg.write.count;
    }
    return -1;
}

int md_slseek(int fildes, int offset)
{
    fs_rsp_msg_type rsp_msg;
    int romfile_lseek(int n, int whence);

    if (fildes == ROMFILEFD) 
    {
		return romfile_lseek(offset, 0);
    }

    fs_seek( fildes,  // file handle 
	    FS_SEEK_SET, // file origin 
	    offset,// file position 
	    NULL,    // callback_function 
	    &rsp_msg);   // message ptr  

    if(rsp_msg.seek.status != FS_OKAY_S)
    {
		return -1;
    }

    fs_tell( fildes,  // file handle
	    NULL,    // callback_function 
	    &rsp_msg);   // message ptr  

    if(rsp_msg.tell.status == FS_OKAY_S)
    {
		return rsp_msg.tell.position;
    }
    return -1;
}

int md_clseek(int fildes, int offset)
{
    fs_rsp_msg_type rsp_msg;
    int romfile_lseek(int n, int whence);

    if (fildes == ROMFILEFD) 
    {
        return romfile_lseek(offset, 1);
    }

    if (offset != 0) 
    {
	    fs_seek( fildes,  // file handle 
	    	FS_SEEK_CURRENT, // file origin 
	    	offset,// file position 
	    	NULL,    // callback_function 
	    	&rsp_msg);   // message ptr  

        if(rsp_msg.seek.status != FS_OKAY_S) 
        {
            return -1;
        }
    }

    fs_tell( fildes,  // file handle 
	    NULL,    // callback_function 
	    &rsp_msg);   // message ptr 

    if(rsp_msg.tell.status == FS_OKAY_S) 
    {
        return rsp_msg.tell.position;
    }
    return -1;
}

int md_elseek(int fildes, int offset)
{
    fs_rsp_msg_type rsp_msg;
    int romfile_lseek(int n, int whence);

    if (fildes == ROMFILEFD) 
    {
        return romfile_lseek(offset, 2);
    }

    fs_seek( fildes,  // file handle 
	    FS_SEEK_EOF, // file origin 
	    offset,// file position 
	    NULL,    // callback_function 
	    &rsp_msg);   // message ptr 

    if(rsp_msg.seek.status != FS_OKAY_S) 
    {
        return -1;
    }

    fs_tell( fildes,  // file handle 
	    NULL,    // callback_function
	    &rsp_msg);   // message ptr  

    if(rsp_msg.tell.status == FS_OKAY_S) 
    {
        return rsp_msg.tell.position;
    }
    return -1;
}

int md_flush(int fd)
{
    // 이 부분은 구현하지 않는다고 합니다.
    return 0;
}

int md_exists(const char *filename)
{
    fs_rsp_msg_type rsp_msg;

    fs_nametest(filename,	/* Name of file */
	    FS_TEST_FILE_OR_DIR,
	    NULL,		/* Callback function ptr */
	    &rsp_msg);		/* Response message ptr */

    if(rsp_msg.nametest.status == FS_OKAY_S)
	return rsp_msg.nametest.name_found == TRUE;
    return -1;
/*
   	char name_new[FS_FILENAME_MAX_LENGTH];
	fs_rsp_msg_type rsp_msg;
	int ret;
		
	memset((void*)name_new,0x00,sizeof(name_new));
	memcpy(name_new, filename, strlen(filename));


// SKY_IsFileOrDirExist(..)에서 FATAL ERROR가 발생되는 경우가 있어서 
// fs_test을 직접 call하기로 함. 
//	if(!SKY_IsFileOrDirExist((CONST BYTE*)newname)) { 

    fs_test(name_new, NULL, &rsp_msg);
	if (rsp_msg.nametest.status == FS_OKAY_S)
	{
		if (rsp_msg.nametest.name_found)
			ret = 1;
		else ret = -1;
    }
    else {
		MSG_ERROR("md_exists => Fatal error occur",0,0,0);
        ret = -1;
    }

	return ret;
*/
}

int md_filesize(const char *filename)
{
    fs_rsp_msg_type rsp_msg;

    fs_file_size(filename, // Name of file 
	    NULL,             // Callback function ptr 
	    &rsp_msg);        // Response message ptr 

    if(rsp_msg.file_size.status == FS_OKAY_S) 
    {
        return (int)rsp_msg.file_size.size;
    }
    return -1;
}

int md_fsavail(void)
{   
    return (int) fs_space_available();
}

int md_fsused(void)
{   
    return (int) fs_space_used();
}

int md_unlink(const char *filename)
{
    fs_rsp_msg_type rsp_msg;

    fs_remove(filename, // Name of file
	    NULL,             // Callback function ptr 
	    &rsp_msg);        // Response message ptr

    if(rsp_msg.rmfile.status == FS_OKAY_S) 
    {
        return 0;
    }

    return -1;
}

/* TIMER SYSTEM */

void md_time(unsigned long *hiTime, unsigned long *loTime)
{
    qword	now;

    clk_read_ms(now);		/* since 1980_1_6 */
    qw_add(now, now, gap);	/* since 1970_1_1 */
    *hiTime = qw_hi(now);
    *loTime = qw_lo(now);
}

void md_date2time(int yy, int mm, int dd, int h, int m, int s, int ms,
       	unsigned long *hiTime, unsigned long *loTime)
{
	qword	time;
    clk_julian_type	tf;

    tf.year = yy;
    tf.month = mm + 1;
    tf.day = dd;
    tf.hour = h;
    tf.minute = m;
    tf.second = s;

    qw_set(time, 0, clk_julian_to_secs(&tf));
    qw_mul(time, time, (dword)1000);
    qw_inc(time, (dword)ms);	/* since 1980_1_6 */
    qw_add(time, time, gap);	/* since 1970_1_1 */

    *hiTime = qw_hi(time);
    *loTime = qw_lo(time);
}

void md_time2date(unsigned long hiTime, unsigned long loTime,
	int *yy, int *mm, int *dd, int *w, int *h, int *m, int *s, int *ms)
{
	qword	time, qsec, qmsec;
    clk_julian_type	tf;

    qw_set(time, hiTime, loTime);	/* since 1970_1_1 */
    qw_sub(time, time, gap);		/* since 1980_1_6 */
    qw_div(qsec, time, 1000);

    clk_secs_to_julian(qw_lo(qsec), &tf);

    qw_mul(qsec, qsec, 1000);
    qw_sub(qmsec, time, qsec);

    *yy = tf.year;
    *mm = tf.month - 1;
    *dd = tf.day;
    *w =  (tf.day_of_week == 6 ? 0 : tf.day_of_week + 1);
    *h = tf.hour;
    *m = tf.minute;
    *s = tf.second;
    *ms = qw_lo(qmsec);
}

// mdarm_sys.c 의 static변수를 다른 file에서 사용을 위해  
void  mdarm_fd_init(void)
{
	fd_init();
}
void  mdarm_fd_finalize(void)
{
	fd_finalize();
}

void mdarm_set_lock_SharedBuffer(short i)
{
	lock_SharedBuffer = i;
}

void mdarm_get_gap(qword* g)
{
	g = &gap;
}
