/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

				Lempel-Ziv 77 변형 LZB 앞축 Algorithm

=============================================================================

                        EDIT HISTORY FOR MODULE

when      who   what, where, why
--------  ---   ------------------------------------------------------------
01-09-06 jwpark add comments
01-09-03 jwpark created
=============================================================================*/



///////////////////////////////////////////////////////////////////////////////
#include "LZB.h"



///////////////////////////////////////////////////////////////////////////////
void LZBDecompress(dword *cdata, // compress data
				   byte  *odata) // original data => decompress data
{
  word				header;		// current header
  byte				single;		// current single data
  dword				offset;		// current offset
  dword				match;		// current match
  dword				opos;		// current decompress data position
  dword				osize;		// original data size
  word				offsetBits;	// offset bits
  word				matchBits;	// match bits
  register word		rbits;		// current valid data bits
  register dword	rdata;		// current data

  // get original data size
  osize      = *cdata;
  cdata++;

  // get compression information : <offset_bits, match_bits>
  offsetBits = (word)(*cdata >> 16);
  matchBits  = (word)(*cdata & 0x0000FFFF);
  cdata++;


  opos  = 0;
  rbits = 32;
  rdata = *cdata;
  while(opos < osize)
	{
	  // header
	  if(rbits == 0)
		{
		  cdata++;
		  rdata = *cdata;
		  rbits = 32;
		}
	  header  = (word)(rdata >> (32 - 1));
	  rdata <<= 1;
	  rbits--;


	  if(header == LZB_SINGLE)
		{
		  // signle
		  if(rbits == 0)
			{
			  cdata++;
			  rdata	  = *cdata;
			  single  = (byte)(rdata >> (32 - 8));
			  rdata <<= 8;
			  rbits   = 32 - 8;
			}
		  else if(rbits < 8)
			{
			  single  = (byte)(rdata >> (32 - 8));
			  cdata++;
			  rdata   = *cdata;
			  single |= (byte)(rdata >> (32 - (8 - rbits)));
			  rdata <<= (8 - rbits);
			  rbits   = 32 - (8 - rbits);
			}
		  else
			{
			  single  = (byte)(rdata >> (32 - 8));
			  rdata <<= 8;
			  rbits  -= 8;
			}
		  odata[opos] = single;
		  opos++;
		}
	  else
		{
		  // offset
		  if(rbits == 0)
			{
			  cdata++;
			  rdata   = *cdata;
			  offset  = rdata >> (32 - offsetBits);
			  rdata <<= offsetBits;
			  rbits   = 32 - offsetBits;
			}
		  else if(rbits < offsetBits)
			{
			  offset  = rdata >> (32 - offsetBits);
			  cdata++;
			  rdata   = *cdata;
			  offset |= rdata >> (32 - (offsetBits - rbits));
			  rdata <<= offsetBits - rbits;
			  rbits   = 32 - (offsetBits - rbits);
			}
		  else
			{
			  offset  = rdata >> (32 - offsetBits);
			  rdata <<= offsetBits;
			  rbits  -= offsetBits;
			}

		  // match
		  if(rbits == 0)
			{
			  cdata++;
			  rdata   = *cdata;
			  match   = rdata >> (32 - matchBits);
			  rdata <<= matchBits;
			  rbits   = 32 - matchBits;
			}
		  else if(rbits < matchBits)
			{
			  match   = rdata >> (32 - matchBits);
			  cdata++;
			  rdata   = *cdata;
			  match  |= rdata >> (32 - (matchBits - rbits));
			  rdata <<= matchBits - rbits;
			  rbits   = 32 - (matchBits - rbits);
			}
		  else
			{
			  match   = rdata >> (32 - matchBits);
			  rdata <<= matchBits;
			  rbits   -= matchBits;
			}
		  while(match > 0)
			{
			  odata[opos] = odata[opos - offset];
			  opos++;
			  match--;
			}
		}
	}
}
