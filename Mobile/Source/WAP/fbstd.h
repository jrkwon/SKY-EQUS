#ifndef __FBSTD_H
#define __FBSTD_H



#ifndef WIN32
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned char u_char;
typedef unsigned int sizet;

int stricmp (  const char * dst,  const char * src );
int strnicmp ( const char * first, const char * last,  sizet count );
#endif

#endif  /* __FBSTD_H */