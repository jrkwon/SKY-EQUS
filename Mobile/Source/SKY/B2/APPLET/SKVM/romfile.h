#define	USE_ROMFILE_AS_C
#ifndef USE_ROMFILE_AS_C
#define	ROMFILEADDR	((unsigned char *)0x370000)
#else
extern unsigned char const ROM romfile[];
#define	ROMFILEADDR	((unsigned char *)romfile)
#endif
#define ROMFILESIZEBYTES 0x00, 0x00, 0xbf, 0x1a, /* 48922 */
