/* crypto/rsa/rsa_eay.c */
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

#include <stdio.h>
#include "cryptlib.h"
#include "bn_new.h"
#include "rsa_new.h"
#include "rand.h"

int RSA_eay_public_encrypt(int flen, unsigned char *from,
	     unsigned char *to, RSA *rsa, int padding)
	{
	BIGNUM f,ret;
	int i,j,k,num=0,r= -1;
	unsigned char *buf=NULL;
	BN_CTX *ctx=NULL;

	BN_init(&f);
	BN_init(&ret);
	if ((ctx=BN_CTX_new()) == NULL) goto err;
	num=BN_num_bytes(rsa->n);
	if ((buf=(unsigned char *)Malloc(num)) == NULL)
		{
		goto err;
		}

	switch (padding)
		{
	case RSA_PKCS1_PADDING:
		i=RSA_padding_add_PKCS1_type_2(buf,num,from,flen);
		break;
#ifndef NO_SHA
	case RSA_PKCS1_OAEP_PADDING:
	        i=RSA_padding_add_PKCS1_OAEP(buf,num,from,flen,NULL,0);
		break;
#endif
//	case RSA_SSLV23_PADDING:
//		i=RSA_padding_add_SSLv23(buf,num,from,flen);
//		break;
//	case RSA_NO_PADDING:
//		i=RSA_padding_add_none(buf,num,from,flen);
//		break;
	default:
		goto err;
		}
	if (i <= 0) goto err;

	if (BN_bin2bn(buf,num,&f) == NULL) goto err;
	
	if ((rsa->_method_mod_n == NULL) && (rsa->flags & RSA_FLAG_CACHE_PUBLIC))
		{
		if ((rsa->_method_mod_n=BN_MONT_CTX_new()) != NULL)
			if (!BN_MONT_CTX_set(rsa->_method_mod_n,rsa->n,ctx))
			    goto err;
		}

#if 0
	if (!rsa->meth->bn_mod_exp(&ret,&f,rsa->e,rsa->n,ctx,
		rsa->_method_mod_n)) goto err;
#endif
	if (!BN_mod_exp_mont(&ret,&f,rsa->e,rsa->n,ctx,
		rsa->_method_mod_n)) goto err;

	/* put in leading 0 bytes if the number is less than the
	 * length of the modulus */
	j=BN_num_bytes(&ret);
	i=BN_bn2bin(&ret,&(to[num-j]));
	for (k=0; k<(num-i); k++)
		to[k]=0;

	r=num;
err:
	if (ctx != NULL) BN_CTX_free(ctx);
	BN_clear_free(&f);
	BN_clear_free(&ret);
	if (buf != NULL) 
		{
		memset(buf,0,num);
		Free(buf);
		}
	return(r);
	}

int RSA_eay_private_encrypt(int flen, unsigned char *from,
	     unsigned char *to, RSA *rsa, int padding)
	{
	BIGNUM f,ret;
	int i,j,k,num=0,r= -1;
	unsigned char *buf=NULL;
	BN_CTX *ctx=NULL;

	BN_init(&f);
	BN_init(&ret);

	if ((ctx=BN_CTX_new()) == NULL) goto err;
	num=BN_num_bytes(rsa->n);
	if ((buf=(unsigned char *)Malloc(num)) == NULL)
		{
		goto err;
		}
	switch (padding)
		{
	case RSA_PKCS1_OAEP_PADDING:
	    i=RSA_padding_add_PKCS1_OAEP(buf,num,from,flen,NULL,0);
        break;
	case RSA_PKCS1_PADDING:
		i=RSA_padding_add_PKCS1_type_1(buf,num,from,flen);
		break;
//	case RSA_NO_PADDING:
//		i=RSA_padding_add_none(buf,num,from,flen);
//		break;
//	case RSA_SSLV23_PADDING:
	default:
		goto err;
		}
	if (i <= 0) goto err;

	if (BN_bin2bn(buf,num,&f) == NULL) goto err;

//	if ((rsa->flags & RSA_FLAG_BLINDING) && (rsa->blinding == NULL))
//		RSA_blinding_on(rsa,ctx);
//	if (rsa->flags & RSA_FLAG_BLINDING)
//		if (!BN_BLINDING_convert(&f,rsa->blinding,ctx)) goto err;

	if ( (rsa->flags & RSA_FLAG_EXT_PKEY) ||
		((rsa->p != NULL) &&
		(rsa->q != NULL) &&
		(rsa->dmp1 != NULL) &&
		(rsa->dmq1 != NULL) &&
		(rsa->iqmp != NULL)) )
		{ if (!RSA_eay_mod_exp(&ret,&f,rsa)) goto err; }
	else
		{
		if (!BN_mod_exp_mont(&ret,&f,rsa->d,rsa->n,ctx,NULL)) goto err;
		}

//	if (rsa->flags & RSA_FLAG_BLINDING)
//		if (!BN_BLINDING_invert(&ret,rsa->blinding,ctx)) goto err;

	/* put in leading 0 bytes if the number is less than the
	 * length of the modulus */
	j=BN_num_bytes(&ret);
	i=BN_bn2bin(&ret,&(to[num-j]));
	for (k=0; k<(num-i); k++)
		to[k]=0;

	r=num;
err:
	if (ctx != NULL) BN_CTX_free(ctx);
	BN_clear_free(&ret);
	BN_clear_free(&f);
	if (buf != NULL)
		{
		memset(buf,0,num);
		Free(buf);
		}
	return(r);
	}

int RSA_eay_private_decrypt(int flen, unsigned char *from,
	     unsigned char *to, RSA *rsa, int padding)
	{
	BIGNUM f,ret;
	int j,num=0,r= -1;
	unsigned char *p;
	unsigned char *buf=NULL;
	BN_CTX *ctx=NULL;

	BN_init(&f);
	BN_init(&ret);
	ctx=BN_CTX_new();
	if (ctx == NULL) goto err;

	num=BN_num_bytes(rsa->n);

	if ((buf=(unsigned char *)Malloc(num)) == NULL)
		{
		goto err;
		}

	/* This check was for equality but PGP does evil things
	 * and chops off the top '0' bytes */
	if (flen > num)
		{
		goto err;
		}

	/* make data into a big number */
	if (BN_bin2bn(from,(int)flen,&f) == NULL) goto err;

//	if ((rsa->flags & RSA_FLAG_BLINDING) && (rsa->blinding == NULL))
//		RSA_blinding_on(rsa,ctx);
//	if (rsa->flags & RSA_FLAG_BLINDING)
//		if (!BN_BLINDING_convert(&f,rsa->blinding,ctx)) goto err;

	/* do the decrypt */
	if ( (rsa->flags & RSA_FLAG_EXT_PKEY) ||
		((rsa->p != NULL) &&
		(rsa->q != NULL) &&
		(rsa->dmp1 != NULL) &&
		(rsa->dmq1 != NULL) &&
		(rsa->iqmp != NULL)) )
		{ if (!RSA_eay_mod_exp(&ret,&f,rsa)) goto err; }
	else
		{
		if (!BN_mod_exp_mont(&ret,&f,rsa->d,rsa->n,ctx,NULL))
			goto err;
		}

//	if (rsa->flags & RSA_FLAG_BLINDING)
//		if (!BN_BLINDING_invert(&ret,rsa->blinding,ctx)) goto err;

	p=buf;
	j=BN_bn2bin(&ret,p); /* j is only used with no-padding mode */

	switch (padding)
		{
	case RSA_PKCS1_PADDING:
		r=RSA_padding_check_PKCS1_type_2(to,num,buf,j,num);
		break;
#ifndef NO_SHA
        case RSA_PKCS1_OAEP_PADDING:
	        r=RSA_padding_check_PKCS1_OAEP(to,num,buf,j,num,NULL,0);
                break;
#endif
// 	case RSA_SSLV23_PADDING:
//		r=RSA_padding_check_SSLv23(to,num,buf,j,num);
//		break;
//	case RSA_NO_PADDING:
//		r=RSA_padding_check_none(to,num,buf,j,num);
//		break;
	default:
		goto err;
		}
	if (r < 0)

err:
	if (ctx != NULL) BN_CTX_free(ctx);
	BN_clear_free(&f);
	BN_clear_free(&ret);
	if (buf != NULL)
		{
		memset(buf,0,num);
		Free(buf);
		}
	return(r);
	}

int RSA_eay_public_decrypt(int flen, unsigned char *from,
	     unsigned char *to, RSA *rsa, int padding)
	{
	BIGNUM f,ret;
	int i,num=0,r= -1;
	unsigned char *p;
	unsigned char *buf=NULL;
	BN_CTX *ctx=NULL;

	BN_init(&f);
	BN_init(&ret);
	ctx=BN_CTX_new();
	if (ctx == NULL) goto err;

	num=BN_num_bytes(rsa->n);
	buf=(unsigned char *)Malloc(num);
	if (buf == NULL)
		{
		goto err;
		}

	/* This check was for equality but PGP does evil things
	 * and chops off the top '0' bytes */
	if (flen > num)
		{
		goto err;
		}

	if (BN_bin2bn(from,flen,&f) == NULL) goto err;
	/* do the decrypt */
	if ((rsa->_method_mod_n == NULL) && (rsa->flags & RSA_FLAG_CACHE_PUBLIC))
		{
		if ((rsa->_method_mod_n=BN_MONT_CTX_new()) != NULL)
			if (!BN_MONT_CTX_set(rsa->_method_mod_n,rsa->n,ctx))
			    goto err;
		}

	if (!BN_mod_exp_mont(&ret,&f,rsa->e,rsa->n,ctx,
		rsa->_method_mod_n)) goto err;

	p=buf;
	i=BN_bn2bin(&ret,p);

	switch (padding)
		{
    case RSA_PKCS1_OAEP_PADDING:
	    r=RSA_padding_check_PKCS1_OAEP(to,num,buf,i,num,NULL,0);
        break;
	case RSA_PKCS1_PADDING:
		r=RSA_padding_check_PKCS1_type_1(to,num,buf,i,num);
		break;
//	case RSA_NO_PADDING:
//		r=RSA_padding_check_none(to,num,buf,i,num);
//		break;
	default:
		goto err;
		}
	if (r < 0)

err:
	if (ctx != NULL) BN_CTX_free(ctx);
	BN_clear_free(&f);
	BN_clear_free(&ret);
	if (buf != NULL)
		{
		memset(buf,0,num);
		Free(buf);
		}
	return(r);
	}

int RSA_eay_mod_exp(BIGNUM *r0, BIGNUM *I, RSA *rsa)
	{
	BIGNUM r1,m1;
	int ret=0;
	BN_CTX *ctx;

	if ((ctx=BN_CTX_new()) == NULL) goto err;
	BN_init(&m1);
	BN_init(&r1);

	if (rsa->flags & RSA_FLAG_CACHE_PRIVATE)
		{
		if (rsa->_method_mod_p == NULL)
			{
			if ((rsa->_method_mod_p=BN_MONT_CTX_new()) != NULL)
				if (!BN_MONT_CTX_set(rsa->_method_mod_p,rsa->p,
						     ctx))
					goto err;
			}
		if (rsa->_method_mod_q == NULL)
			{
			if ((rsa->_method_mod_q=BN_MONT_CTX_new()) != NULL)
				if (!BN_MONT_CTX_set(rsa->_method_mod_q,rsa->q,
						     ctx))
					goto err;
			}
		}

	if (!BN_mod(&r1,I,rsa->q,ctx)) goto err;
	if (!BN_mod_exp_mont(&m1,&r1,rsa->dmq1,rsa->q,ctx,
		rsa->_method_mod_q)) goto err;

	if (!BN_mod(&r1,I,rsa->p,ctx)) goto err;
	if (!BN_mod_exp_mont(r0,&r1,rsa->dmp1,rsa->p,ctx,
		rsa->_method_mod_p)) goto err;

	if (!BN_sub(r0,r0,&m1)) goto err;
	/* This will help stop the size of r0 increasing, which does
	 * affect the multiply if it optimised for a power of 2 size */
	if (r0->neg)
		if (!BN_add(r0,r0,rsa->p)) goto err;

	if (!BN_mul(&r1,r0,rsa->iqmp,ctx)) goto err;
	if (!BN_mod(r0,&r1,rsa->p,ctx)) goto err;
	/* If p < q it is occasionally possible for the correction of
         * adding 'p' if r0 is negative above to leave the result still
	 * negative. This can break the private key operations: the following
	 * second correction should *always* correct this rare occurrence.
	 * This will *never* happen with OpenSSL generated keys because
         * they ensure p > q [steve]
         */
	if (r0->neg)
		if (!BN_add(r0,r0,rsa->p)) goto err;
	if (!BN_mul(&r1,r0,rsa->q,ctx)) goto err;
	if (!BN_add(r0,&r1,&m1)) goto err;

	ret=1;
err:
	BN_clear_free(&m1);
	BN_clear_free(&r1);
	BN_CTX_free(ctx);
	return(ret);
	}

int RSA_eay_init(RSA *rsa)
	{
	rsa->flags|=RSA_FLAG_CACHE_PUBLIC|RSA_FLAG_CACHE_PRIVATE;
	return(1);
	}

int RSA_eay_finish(RSA *rsa)
	{
	if (rsa->_method_mod_n != NULL)
		BN_MONT_CTX_free(rsa->_method_mod_n);
	if (rsa->_method_mod_p != NULL)
		BN_MONT_CTX_free(rsa->_method_mod_p);
	if (rsa->_method_mod_q != NULL)
		BN_MONT_CTX_free(rsa->_method_mod_q);
	return(1);
	}

