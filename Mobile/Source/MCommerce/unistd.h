#ifndef _UNISTD_H
#define _UNISTD_H

//swjung 
#ifdef BLOCK
#include "sys\types.h"
#endif //#ifdef BLOCK

#include "types.h"

 
pid_t getpid(void);
 
uid_t getuid(void);
 
#endif  // _UNISTD_H

 