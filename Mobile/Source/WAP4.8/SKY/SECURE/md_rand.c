#include <string.h>
#include "md5.h"
#include "const.h"
#define MD_DIGEST_LENGTH	MD5_DIGEST_LENGTH
#define MD_CTX			MD5_CTX
#define MD_Init(a)		ID_MD5_Init(a)
#define MD_Update(a,b,c)	ID_MD5_Update(a,b,c)
#define	MD_Final(a,b)		ID_MD5_Final(a,b)

#define STATE_SIZE	1023
static int state_num=0,state_index=0;
static unsigned char state[STATE_SIZE];
static unsigned char md[MD_DIGEST_LENGTH];
static int count=0;

#define PURIFY

void DLX_RAND_Seed(buf,num)
unsigned char *buf;
int num;
	{
	int i,j,k,st_idx,st_num;
	MD_CTX m;

#ifdef NORAND
	return;
#endif

	st_idx=state_index;
	st_num=state_num;

	state_index=(state_index+num);
	if (state_index > STATE_SIZE)
		{
		state_index%=STATE_SIZE;
		state_num=STATE_SIZE;
		}
	else if (state_num < STATE_SIZE)	
		{
		if (state_index > state_num)
			state_num=state_index;
		}

	for (i=0; i<num; i+=MD_DIGEST_LENGTH)
		{
		j=(num-i);
		j=(j > MD_DIGEST_LENGTH)?MD_DIGEST_LENGTH:j;

		MD_Init(&m);
		MD_Update(&m,md,MD_DIGEST_LENGTH);
		k=(st_idx+j)-STATE_SIZE;
		if (k > 0)
			{
			MD_Update(&m,&(state[st_idx]),j-k);
			MD_Update(&m,&(state[0]),k);
			}
		else
			MD_Update(&m,&(state[st_idx]),j);
			
		MD_Update(&m,buf,j);
		MD_Final(md,&m);

		buf+=j;

		for (k=0; k<j; k++)
			{
			state[st_idx++]^=md[k];
			if (st_idx >= STATE_SIZE)
				{
				st_idx=0;
				st_num=STATE_SIZE;
				}
			}
		}
	memset((char *)&m,0,sizeof(m));
	}

extern unsigned long ui_get_local_time(void);
void DLX_RAND_Bytes(buf,num)
unsigned char *buf;
int num;
	{
	int i,j,k,st_num,st_idx;
	MD_CTX m;
	static int init=1;
	unsigned long l;
#ifdef DEVRANDOM
	FILE *fh;
#endif

#ifdef PREDICT
	{
	static unsigned char val=0;

	for (i=0; i<num; i++)
		buf[i]=val++;
	return;
	}
#endif

	if (init)
		{
		init=0;
		DLX_RAND_Seed((unsigned char *)&m, (int)sizeof(m));
		//l = (ul)(ui_get_local_time() + ELAPSED_SECONDS);
		l = 0x00000000;
		DLX_RAND_Seed((unsigned char *)&l, (int)sizeof(l));

#ifdef DEVRANDOM
		if ((fh = fopen(DEVRANDOM, "r")) != NULL)
			{
			unsigned char buf[32];

			fread((unsigned char *)buf,1,32,fh);
			fclose(fh);
			DLX_RAND_Seed(buf,32);
			memset(buf,0,32);
			}
#endif
#ifdef PURIFY
		memset(state,0,STATE_SIZE);
		memset(md,0,MD_DIGEST_LENGTH);
#endif
		}

	st_idx=state_index;
	st_num=state_num;
	state_index+=num;
	if (state_index > state_num)
		state_index=(state_index%state_num);

	while (num > 0)
		{
		j=(num >= MD_DIGEST_LENGTH/2)?MD_DIGEST_LENGTH/2:num;
		num-=j;
		MD_Init(&m);
		MD_Update(&m,&(md[MD_DIGEST_LENGTH/2]),MD_DIGEST_LENGTH/2);
#ifndef PURIFY
		MD_Update(&m,buf,j);
#endif
		k=(st_idx+j)-st_num;
		if (k > 0)
			{
			MD_Update(&m,&(state[st_idx]),j-k);
			MD_Update(&m,&(state[0]),k);
			}
		else
			MD_Update(&m,&(state[st_idx]),j);
		MD_Final(md,&m);

		for (i=0; i<j; i++)
			{
			if (st_idx >= st_num)
				st_idx=0;
			state[st_idx++]^=md[i];
			*(buf++)=md[i+MD_DIGEST_LENGTH/2];
			}
		}

	MD_Init(&m);
	MD_Update(&m,(unsigned char *)&count,sizeof(count)); count++;
	MD_Update(&m,md,MD_DIGEST_LENGTH);
	MD_Final(md,&m);
	memset(&m,0,sizeof(m));
	}
