/***************************************************************************

	FS Interface Routine

****************************************************************************/
#include	"
#include	"irda.h"
#include	"filesys.h"
#include	"console.h"

#define	FILESIZE	1000000

static	int	FS_Id;

static	dword	left;
static	dword	prog;

int	*FS_Open(char *name,char *mode)
{
#if defined(USE_MONITOR)
	cprintf	("FS:Open[%s] Mode[%s]\n",name,mode);
#endif
	FS_Id	= 1;
	left	= FILESIZE;
	prog	= 0;
	return	&FS_Id;
}

int	FS_Close(int *fd)
{
#if defined(USE_MONITOR)
	cprintf("FS:Close %d\n",*fd);
#endif
	FS_Id	= 0;
	return	0;
}

int	FS_Read(void *buf,int size,int block,int *fd)
{
dword	len,no;
byte	*dp;
#if defined(USE_MONITOR)
	cprintf("FS:Read (%dx%d)\n",size,block);
#endif
	if(FS_Id){
		if(left == 0){
			FS_Id = 0;
			return	0;
		}
		if(size == 0 || block == 0){
			return	0;
		}
		len	= size * block;
		if(left < len){
			len = left;
		}
		dp = (byte *)buf;
		for(no = 0;no < len;no++){
			*dp++ = (prog + no) / 0x100;
		}
		left	-= len;
		prog	+= len;
		return(len);
	}else{
		return(-1);
	}
}

int	FS_Write(void *buf,int size,int block,int *fd)
{
#if defined(USE_MONITOR)
	cprintf("FS:Write (%dx%d)\n",size,block);
#endif
	return	block;
}

unsigned long FS_GetFileSize(int *fd)
{
	return	FILESIZE;
}
