#ifndef _BIT_H
#define _BIT_H

/*	return (ucCh | ucBin) */
uc setbit(uc ucCh, uc ucBin);

/* 	return (ucCh & ~ucBin) */
uc resetbit(uc ucCh, uc ucBin);

/* 	return (ucCh & ucBin) */
uc chkbit(uc ucCh, uc ucBin);

#endif
