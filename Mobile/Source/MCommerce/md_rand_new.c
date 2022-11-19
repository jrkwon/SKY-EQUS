/* crypto/rand/md_rand.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */
/* ====================================================================
 * Copyright (c) 1998-2000 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#define ENTROPY_NEEDED 16  /* require 128 bits = 16 bytes of randomness */

#ifndef MD_RAND_DEBUG
# ifndef NDEBUG
#   define NDEBUG
# endif
#endif

#include <assert.h>
#include <stdio.h>

//swjung temporarily blocked
#ifdef BLOCK
	#include <time.h>
#endif //#ifdef BLOCK

#include <string.h>

//swjung
//#include "ska.h"

#include "e_os.h"

#include "crypto.h"
#include "md5_new.h"

#if !defined(USE_MD5_RAND) && !defined(USE_SHA1_RAND) && !defined(USE_MDC2_RAND) && !defined(USE_MD2_RAND)
#if !defined(NO_SHA) && !defined(NO_SHA1)
#define USE_SHA1_RAND
#elif !defined(NO_MD5)
#define USE_MD5_RAND
#else
#error No message digest algorithm available
#endif
#endif

/* Changed how the state buffer used.  I now attempt to 'wrap' such
 * that I don't run over the same locations the next time  go through
 * the 1023 bytes - many thanks to
 * Robert J. LeBlanc <rjl@renaissoft.com> for his comments
 */

#if defined(USE_MD5_RAND)
#include "md5_new.h"
#define MD_DIGEST_LENGTH	MD5_DIGEST_LENGTH
#define MD_CTX			MD5_CTX
#define MD_Init(a)		MD5_Init(a)
#define MD_Update(a,b,c)	MD5_Update(a,b,c)
#define	MD_Final(a,b)		MD5_Final(a,b)
#define	MD(a,b,c)		MD5(a,b,c)
#elif defined(USE_SHA1_RAND)
#include "sha.h"
#define MD_DIGEST_LENGTH	SHA_DIGEST_LENGTH
#define MD_CTX			SHA_CTX
#define MD_Init(a)		SHA1_Init(a)
#define MD_Update(a,b,c)	SHA1_Update(a,b,c)
#define	MD_Final(a,b)		SHA1_Final(a,b)
#define	MD(a,b,c)		SHA1(a,b,c)
#endif

#include "rand.h"

#ifdef BN_DEBUG
# define PREDICT
#endif

/* #define NORAND	1 */
/* #define PREDICT	1 */

#define STATE_SIZE	1023
static int state_num=0,state_index=0;
static unsigned char state[STATE_SIZE+MD_DIGEST_LENGTH];
static unsigned char md[MD_DIGEST_LENGTH];
static long md_count[2]={0,0};
static double entropy=0;
static int initialized=0;

#ifdef PREDICT
int rand_predictable=0;
#endif

const char *RAND_version="RAND";

static void ssleay_rand_cleanup(void);
static void ssleay_rand_seed(const void *buf, int num);
static void ssleay_rand_add(const void *buf, int num, double add_entropy);
static int ssleay_rand_bytes(unsigned char *buf, int num);
static int ssleay_rand_pseudo_bytes(unsigned char *buf, int num);
static int ssleay_rand_status(void);

RAND_METHOD rand_ssleay_meth={
	ssleay_rand_seed,
	ssleay_rand_bytes,
	ssleay_rand_cleanup,
	ssleay_rand_add,
	ssleay_rand_pseudo_bytes,
	ssleay_rand_status
	}; 

RAND_METHOD *RAND_SSLeay(void)
	{
	return(&rand_ssleay_meth);
	}

static void ssleay_rand_cleanup(void)
	{
	memset(state,0,sizeof(state));
	state_num=0;
	state_index=0;
	memset(md,0,MD_DIGEST_LENGTH);
	md_count[0]=0;
	md_count[1]=0;
	entropy=0;
	}

static void ssleay_rand_add(const void *buf, int num, double add)
	{
	int i,j,k,st_idx;
	long md_c[2];
	unsigned char local_md[MD_DIGEST_LENGTH];
	MD_CTX m;

#ifdef NORAND
	return;
#endif

	/*
	 * (Based on the rand(3) manpage)
	 *
	 * The input is chopped up into units of 20 bytes (or less for
	 * the last block).  Each of these blocks is run through the hash
	 * function as follows:  The data passed to the hash function
	 * is the current 'md', the same number of bytes from the 'state'
	 * (the location determined by in incremented looping index) as
	 * the current 'block', the new key data 'block', and 'count'
	 * (which is incremented after each use).
	 * The result of this is kept in 'md' and also xored into the
	 * 'state' at the same locations that were used as input into the
         * hash function.
	 */

	CRYPTO_w_lock(CRYPTO_LOCK_RAND);
	st_idx=state_index;

	/* use our own copies of the counters so that even
	 * if a concurrent thread seeds with exactly the
	 * same data and uses the same subarray there's _some_
	 * difference */
	md_c[0] = md_count[0];
	md_c[1] = md_count[1];

	memcpy(local_md, md, sizeof md);

	/* state_index <= state_num <= STATE_SIZE */
	state_index += num;
	if (state_index >= STATE_SIZE)
		{
		state_index%=STATE_SIZE;
		state_num=STATE_SIZE;
		}
	else if (state_num < STATE_SIZE)	
		{
		if (state_index > state_num)
			state_num=state_index;
		}
	/* state_index <= state_num <= STATE_SIZE */

	/* state[st_idx], ..., state[(st_idx + num - 1) % STATE_SIZE]
	 * are what we will use now, but other threads may use them
	 * as well */

	md_count[1] += (num / MD_DIGEST_LENGTH) + (num % MD_DIGEST_LENGTH > 0);

	CRYPTO_w_unlock(CRYPTO_LOCK_RAND);

	for (i=0; i<num; i+=MD_DIGEST_LENGTH)
		{
		j=(num-i);
		j=(j > MD_DIGEST_LENGTH)?MD_DIGEST_LENGTH:j;

		MD_Init(&m);
		MD_Update(&m,local_md,MD_DIGEST_LENGTH);
		k=(st_idx+j)-STATE_SIZE;
		if (k > 0)
			{
			MD_Update(&m,&(state[st_idx]),j-k);
			MD_Update(&m,&(state[0]),k);
			}
		else
			MD_Update(&m,&(state[st_idx]),j);
			
		MD_Update(&m,buf,j);
		MD_Update(&m,(unsigned char *)&(md_c[0]),sizeof(md_c));
		MD_Final(local_md,&m);
		md_c[1]++;

		buf=(const char *)buf + j;

		for (k=0; k<j; k++)
			{
			/* Parallel threads may interfere with this,
			 * but always each byte of the new state is
			 * the XOR of some previous value of its
			 * and local_md (itermediate values may be lost).
			 * Alway using locking could hurt performance more
			 * than necessary given that conflicts occur only
			 * when the total seeding is longer than the random
			 * state. */
			state[st_idx++]^=local_md[k];
			if (st_idx >= STATE_SIZE)
				st_idx=0;
			}
		}
	memset((char *)&m,0,sizeof(m));

	CRYPTO_w_lock(CRYPTO_LOCK_RAND);
	/* Don't just copy back local_md into md -- this could mean that
	 * other thread's seeding remains without effect (except for
	 * the incremented counter).  By XORing it we keep at least as
	 * much entropy as fits into md. */
	for (k = 0; k < sizeof md; k++)
		{
		md[k] ^= local_md[k];
		}
	if (entropy < ENTROPY_NEEDED) /* stop counting when we have enough */
	    entropy += add;
	CRYPTO_w_unlock(CRYPTO_LOCK_RAND);
	
#ifndef THREADS	
	assert(md_c[1] == md_count[1]);
#endif
	}

static void ssleay_rand_seed(const void *buf, int num)
	{
	ssleay_rand_add(buf, num, num);
	}

//swjung
//extern unsigned long ui_get_local_time(void);
extern unsigned long SKY_GetLocalTime(void);

unsigned char input_dummy[]="HereComesANewChallenger!Really?NuclearLaunchDetected~Siren!!";
unsigned char output_dummy[50];

static void ssleay_rand_initialize(void)
	{
	unsigned long l;
	//int dummy = 73;
#ifndef GETPID_IS_MEANINGLESS
	pid_t curr_pid = getpid();
#endif
#ifdef DEVRANDOM
	FILE *fh;
#endif

#ifdef NORAND
	return;
#endif

	CRYPTO_w_unlock(CRYPTO_LOCK_RAND);
	/* put in some default random data, we need more than just this */
#ifndef GETPID_IS_MEANINGLESS
	l=curr_pid;
	RAND_add(&l,sizeof(l),0);
	l=getuid();
	RAND_add(&l,sizeof(l),0);
#endif
	//swjung
	//l=time(NULL);
	l=SKY_GetLocalTime();
	RAND_add(&l,sizeof(l),0);


	//swjung add RAND_seed function
	MD5((const unsigned char *)input_dummy, (unsigned long)strlen((const char *)input_dummy), output_dummy);

	RAND_seed(output_dummy, 16);

#ifdef DEVRANDOM
	/* Use a random entropy pool device. Linux, FreeBSD and OpenBSD
	 * have this. Use /dev/urandom if you can as /dev/random may block
	 * if it runs out of random entries.  */

	if ((fh = fopen(DEVRANDOM, "r")) != NULL)
		{
		unsigned char tmpbuf[ENTROPY_NEEDED];
		int n;
		
		setvbuf(fh, NULL, _IONBF, 0);
		n=fread((unsigned char *)tmpbuf,1,ENTROPY_NEEDED,fh);
		fclose(fh);
		RAND_add(tmpbuf,sizeof tmpbuf,n);
		memset(tmpbuf,0,n);
		}
#endif
#ifdef PURIFY
	memset(state,0,STATE_SIZE);
	memset(md,0,MD_DIGEST_LENGTH);
#endif
	CRYPTO_w_lock(CRYPTO_LOCK_RAND);
	initialized=1;
	}

static int ssleay_rand_bytes(unsigned char *buf, int num)
	{
	int i,j,k,st_num,st_idx;
	int ok;
	long md_c[2];
	unsigned char local_md[MD_DIGEST_LENGTH];
	MD_CTX m;
#ifndef GETPID_IS_MEANINGLESS
	pid_t curr_pid = getpid();
#endif

#ifdef PREDICT
	if (rand_predictable)
		{
		static unsigned char val=0;

		for (i=0; i<num; i++)
			buf[i]=val++;
		return(1);
		}
#endif

	/*
	 * (Based on the rand(3) manpage:)
	 *
	 * For each group of 10 bytes (or less), we do the following:
	 *
	 * Input into the hash function the top 10 bytes from the
	 * local 'md' (which is initialized from the global 'md'
	 * before any bytes are generated), the bytes that are
	 * to be overwritten by the random bytes, and bytes from the
	 * 'state' (incrementing looping index).  From this digest output
	 * (which is kept in 'md'), the top (up to) 10 bytes are
	 * returned to the caller and the bottom (up to) 10 bytes are xored
	 * into the 'state'.
	 * Finally, after we have finished 'num' random bytes for the
	 * caller, 'count' (which is incremented) and the local and global 'md'
	 * are fed into the hash function and the results are kept in the
	 * global 'md'.
	 */

	CRYPTO_w_lock(CRYPTO_LOCK_RAND);

	if (!initialized)
		ssleay_rand_initialize();

	ok = (entropy >= ENTROPY_NEEDED);
	if (!ok)
		{
		/* If the PRNG state is not yet unpredictable, then seeing
		 * the PRNG output may help attackers to determine the new
		 * state; thus we have to decrease the entropy estimate.
		 * Once we've had enough initial seeding we don't bother to
		 * adjust the entropy count, though, because we're not ambitious
		 * to provide *information-theoretic* randomness.
		 */
		entropy -= num;
		if (entropy < 0)
			entropy = 0;
		}

	st_idx=state_index;
	st_num=state_num;
	md_c[0] = md_count[0];
	md_c[1] = md_count[1];
	memcpy(local_md, md, sizeof md);

	state_index+=num;
	if (state_index > state_num)
		state_index %= state_num;

	/* state[st_idx], ..., state[(st_idx + num - 1) % st_num]
	 * are now ours (but other threads may use them too) */

	md_count[0] += 1;
	CRYPTO_w_unlock(CRYPTO_LOCK_RAND);

	while (num > 0)
		{
		j=(num >= MD_DIGEST_LENGTH/2)?MD_DIGEST_LENGTH/2:num;
		num-=j;
		MD_Init(&m);
#ifndef GETPID_IS_MEANINGLESS
		if (curr_pid) /* just in the first iteration to save time */
			{
			MD_Update(&m,(unsigned char*)&curr_pid,sizeof curr_pid);
			curr_pid = 0;
			}
#endif
		MD_Update(&m,&(local_md[MD_DIGEST_LENGTH/2]),MD_DIGEST_LENGTH/2);
		MD_Update(&m,(unsigned char *)&(md_c[0]),sizeof(md_c));
#ifndef PURIFY
		MD_Update(&m,buf,j); /* purify complains */
#endif
		k=(st_idx+j)-st_num;
		if (k > 0)
			{
			MD_Update(&m,&(state[st_idx]),j-k);
			MD_Update(&m,&(state[0]),k);
			}
		else
			MD_Update(&m,&(state[st_idx]),j);
		MD_Final(local_md,&m);

		for (i=0; i<j; i++)
			{
			state[st_idx++]^=local_md[i]; /* may compete with other threads */
			*(buf++)=local_md[i+MD_DIGEST_LENGTH/2];
			if (st_idx >= st_num)
				st_idx=0;
			}
		}

	MD_Init(&m);
	MD_Update(&m,(unsigned char *)&(md_c[0]),sizeof(md_c));
	MD_Update(&m,local_md,MD_DIGEST_LENGTH);
	CRYPTO_w_lock(CRYPTO_LOCK_RAND);
	MD_Update(&m,md,MD_DIGEST_LENGTH);
	MD_Final(md,&m);
	CRYPTO_w_unlock(CRYPTO_LOCK_RAND);

	memset(&m,0,sizeof(m));
	if (ok)
		return(1);
	else
		{
		return(0);
		}
	}

/* pseudo-random bytes that are guaranteed to be unique but not
   unpredictable */
static int ssleay_rand_pseudo_bytes(unsigned char *buf, int num) 
	{
	int ret;

	ret = RAND_bytes(buf, num);
	if (ret == 0)
		{
	#if 0
		err = ERR_peek_error();
		if (ERR_GET_LIB(err) == ERR_LIB_RAND &&
		    ERR_GET_REASON(err) == RAND_R_PRNG_NOT_SEEDED)
			(void)ERR_get_error();
	#endif
		}
	return (ret);
	}

static int ssleay_rand_status(void)
	{
	int ret;

	CRYPTO_w_lock(CRYPTO_LOCK_RAND);

	if (!initialized)
		ssleay_rand_initialize();
	ret = entropy >= ENTROPY_NEEDED;

	CRYPTO_w_unlock(CRYPTO_LOCK_RAND);

	return ret;
	}

