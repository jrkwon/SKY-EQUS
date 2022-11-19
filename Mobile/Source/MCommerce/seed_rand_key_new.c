
#include "seed.h"
#include "rand.h"


//int seed_rand_key(unsigned char * seedKey)
int seed_rand_key(SEED_CBLOCK * seedKey)
{
	if (RAND_bytes((unsigned char *)seedKey, SeedBlockSize) != 1){
		return 0;
	}
	return 1;
}
