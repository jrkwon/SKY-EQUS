#include "const.h"
#include "bitt.h"

uc setbit(uc ucCh, uc ucBin)
{
	return (ucCh | ucBin);
}

uc resetbit(uc ucCh, uc ucBin)
{
	return (ucCh & ~ucBin);
}

uc chkbit(uc ucCh, uc ucBin)
{
	return (ucCh & ucBin);
}
