#ifndef __DNS_H
#define __DNS_H

#include	"fbstd.h"

#ifdef WIN32
#pragma pack (push)

#pragma pack(1)
struct dnsheader {
	u_short 	id;
	
	char	rd		: 1;
	char	tc		: 1;
	char	aa		: 1;
	char	opcode	: 4;
	char	qr		: 1;
	
	char	rcode	: 4;
	char	z		: 3;
	char	ra		: 1;

	u_short 	qdcount;
	u_short 	ancount;
	u_short 	nscount;
	u_short 	arcount;
};
#pragma pack()

struct hostent* gethostbyname(const char* name);
struct hostent* gethostbyaddr(const char* addr, int len, int type);

#pragma pack(pop)

#else  // above WIN32

//typedef byte	int8;


PACKED struct dnsheader {
	unsigned short 	id;
	
	byte	rd		: 1;
	byte	tc		: 1;
	byte	aa		: 1;
	byte	opcode	: 4;
	byte	qr		: 1;
	
	byte	rcode	: 4;
	byte	z		: 3;
	byte	ra		: 1;

	unsigned short 	qdcount;
	unsigned short 	ancount;
	unsigned short 	nscount;
	unsigned short 	arcount;
};

#endif

#endif