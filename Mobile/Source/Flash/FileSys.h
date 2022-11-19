/*****************************************************************************
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/31/00   jnlee   Created.
*****************************************************************************/
/*****************************************************************************/


#define FL_CELL_SIZE	256 // 256 bytes bytes per flash cell.
// Visible out to VocCommon.c
/*
extern nv_map_type used_map;
extern nv_map_type free_map;
extern byte free_start;
extern byte free_end;
extern nv_ftab_type FTAB[NV_MAX_FL_FILE_NUM]; // global file descriptor table.
*/

#ifdef VOICE_RECOGNITION_FEATURE
// Visible out to uinstate.c
extern nv_map_type vd_used_map;
extern nv_map_type vd_free_map;
extern byte vd_free_start;
extern byte vd_free_end;
extern nv_ftab_type VD_FTAB[NV_VD_MAX_FL_FILE_NUM]; // global file descriptor table.
#endif // VOICE_RECOGNITION_FEATURE
//---------------------------------------------------------------------------------------


boolean flffdisk(void);
boolean vd_flffdisk(void);
boolean update_superblock(void);
boolean read_superblock(void);
boolean flfopen(byte fn, byte type);
boolean flfclose(byte fn);
dword flfsize(byte fn);
boolean flferase(byte fn);
dword flfread(byte fn, byte *data, dword size);
dword flfwrite(byte fn, byte *data, dword size);
byte getfn(void);
int getfreeindxnum(void);
byte revive_missing_link(void);
byte GatherMissingLinkGetAvailIndxNum(void);

#ifdef VOICE_RECOGNITION_FEATURE
//----------------------------------------------------------------------------------------
// Voice Dialing
boolean vd_update_superblock(void);
boolean vd_read_superblock(void);
boolean vd_flfopen(byte fn, byte type);
boolean vd_flfclose(byte fn);
dword vd_flfsize(byte fn);
boolean vd_flferase(byte fn);
dword vd_flfread(byte fn, byte *data, dword size);
dword vd_flfwrite(byte fn, byte *data, dword size);
byte vd_getfn(void);
int vd_getfreeindxnum(void);
//eStatus UI_CleanUp(void);	// added by chos 99.07.20	//del by jnlee 00.2.10
byte vd_GatherMissingLinkGetAvailIndxNum(void);

#endif // VOICE_RECOGNITION_FEATURE

//-----------------------------------------------------------------------------------------

#define FS_VOICE_WRITE_UNIT 1500
#define MAX_CELL_NUM 5
extern byte cell_buf_onecell[MAX_CELL_NUM][FS_VOICE_WRITE_UNIT];			// FEATURE_EFS

void erase_unused_Fn(void);
void subcmd_play_rec(void);