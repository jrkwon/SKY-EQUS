/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */

/* Program generated by SDT Cmicro, version 1.33 */

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif

#define C_MICRO_1_3
#include "ml_typw.h"
#include "ctypes.h"
#include "buffmgt.h"
#include "glbldecl.h"
#include "wdp_ul.h"
#include "wspcmwtp.h"
#include "wtppckg.h"


/*************************************************************************
**                  #CODE directives, #BODY sections                    **
*************************************************************************/

/*************************************************************************
**                  SECTION Variables and Functions                     **
*************************************************************************/

/*****
* NEWTYPE TransactionType  (BODY section)
* <<PACKAGE WTPpckg>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\packages\wtppckg.sun(1),119(40,55),1)
******/

/*------------------DEFAULT-----------------*/
#ifndef XNOPROTO
extern void yDef_TransactionType( TransactionType *yVar )
#else
extern void yDef_TransactionType( yVar )
  TransactionType *yVar;
#endif
{
  xDef_SDL_Natural(&((*yVar).dir));
  xDef_SDL_Natural(&((*yVar).tid));
  xDef_SDL_Integer(&((*yVar).handle));
  xDef_SDL_PId(&((*yVar).pid));
  yDef_AddressType(&((*yVar).clientAddress));
  yDef_AddressType(&((*yVar).serverAddress));
}

/*-------------------MAKE-------------------*/
#ifndef XNOPROTO
extern TransactionType yMake_TransactionType(
  SDL_Natural ydir,
  SDL_Natural ytid,
  SDL_Integer yhandle,
  SDL_PId ypid,
  AddressType yclientAddress,
  AddressType yserverAddress )
#else
extern TransactionType  yMake_TransactionType( ydir, ytid, yhandle, ypid,
  yclientAddress, yserverAddress )
  SDL_Natural ydir;
  SDL_Natural ytid;
  SDL_Integer yhandle;
  SDL_PId ypid;
  AddressType yclientAddress;
  AddressType yserverAddress;
#endif
{
  TransactionType  yVar;
  memset((void *)(&yVar), 0, sizeof(TransactionType));
  yAssF_SDL_Natural(yVar.dir, ydir, XASSMAKE);
  yAssF_SDL_Natural(yVar.tid, ytid, XASSMAKE);
  yAssF_SDL_Integer(yVar.handle, yhandle, XASSMAKE);
  yAssF_SDL_PId(yVar.pid, ypid, XASSMAKE);
  yAssF_AddressType(yVar.clientAddress, yclientAddress, XASSMAKE);
  yAssF_AddressType(yVar.serverAddress, yserverAddress, XASSMAKE);
  return yVar;
}