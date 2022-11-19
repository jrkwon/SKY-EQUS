/******************************************************************************

	Convert File System 

********************************************************************************/

#define	fopen	FS_Open
#define	fclose	FS_Close
#define	fread	FS_Read
#define	fwrite	FS_Write
#define	fsize	FS_GetFileSize

#define	FILE	int

#define	FS_READ		0
#define	FS_WRITE	1
#define	FS_ERROR	-1


extern	int				*FS_Open(char *name,char *mode);
extern	int				FS_Close(int *fd);
extern	int				FS_Read(void *buf,int size,int block,int *fd);
extern	int				FS_Write(void *buf,int size,int block,int *fd);
extern	unsigned long	FS_GetFileSize(int *fd);
