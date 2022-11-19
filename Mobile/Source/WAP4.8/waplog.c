#ifdef LOG_ENABLE

#include "msg.h"
/**********************************************************
***      This function is called by Clnta_log()         ***
***      to dump logging into the DMSS DM.              ***
***      Better not to call directly !                  ***
***      Linked only in the WAPPER                      ***
***                                                     ***
***                                    S.J.Ahn          ***
***                                   1999.8.14         ***
***********************************************************/
msg_store_type zz_store[200];

void debug_msg(  char *str, int a, int b, int c )
{

//  static const msg_store_type xx_store = {MSG_LVL_HIGH, __LINE__, msg_file, junkxxyy }; 
//static const msg_store_type xx_store;
//static msg_store_type xx_store[10];
static int dbg_indx =0;

//  xx_store.file_ptr = msg_file;
//  xx_store.level = MSG_LVL_HIGH;
//  xx_store.line = __LINE__;
//  xx_store.xx_fmt = str;

  //strcpy(zz_store[dbg_indx].file_ptr, msg_file);
  zz_store[dbg_indx].file_ptr = msg_file;
  zz_store[dbg_indx].level = MSG_LVL_ERROR;
  zz_store[dbg_indx].line = __LINE__;
  zz_store[dbg_indx].xx_fmt = str;

//  if ((!(MSG_LVL_HIGH < msg_limit)) && (!(MSG_LVL_HIGH < MSG_THRESHOLD))) { 
//	    msg_put(&xx_store, (dword)a, (dword)b, (dword)c); 
	    msg_put(&zz_store[dbg_indx], (dword)a, (dword)b, (dword)c); 
//  }
//  msg_limit = old_limit;
    dbg_indx = (dbg_indx + 1) % 200;
}
#endif //#ifdef LOG_ENABLE