#ifndef	__XVMSTATE_H__
#define	__XVMSTATE_H__

#ifndef SLEEP_PRI
#define SLEEP_PRI	50
#endif
#ifndef FS_PRI
#define  FS_PRI     78   /* REX priority for the File System Task     */
#endif

//#define XUI_PRI			(SLEEP_PRI + 1)
#define XUI_PRI			(FS_PRI + 1)
#define XVM_TIMERS_SIG	0x8000    /*reuse TASK_START_SIG */


extern int	xvm_state;

#define	XVMSTATE_ERROR			-1
#define	XVMSTATE_DEAD			0x0
#define	XVMSTATE_START			0x1
#define	XVMSTATE_RUNNING		0x2
#define	XVMSTATE_SUSPEND		0x3
#define	XVMSTATE_STOP			0x4
#define	XVMSTATE_STOPPED		0x5
#define	XVMSTATE_FROZEN			0x10

/* (xvm_state != XVMSTATE_DEAD && xvm_state != XVMSTATE_FROZEN) */
#define	xvmProcessing	(xvm_state & 0x0f)
#define	xvmActive	(xvm_state == XVMSTATE_RUNNING || xvm_state == XVMSTATE_SUSPEND || xvm_state == XVMSTATE_FROZEN)

extern short xvm_errnum;

#define XVMERR_NORMAL			0	/* normal exit */
#define XVMERR_ABORT			1	/* xvm abort */

#define XVMERR_CLASSFORMAT		2	/* class format */
#define XVMERR_CLASSSIG			3	/* malformed sig */
#define XVMERR_NOTCONST			4	/* not constant */
#define XVMERR_BYTECODE			5	/* bytecode err */

#define XVMERR_OUTOFMEMORY		6	/* out of memory */
#define XVMERR_MEMCONSIST		7	/* memory consist */
#define XVMERR_HEAPALLOC		8	/* heap alloc err */
#define XVMERR_MEMMARK			9	/* memory mark */
#define XVMERR_GCMARK			10	/* gc mark err */

#define XVMERR_NOSUCHMETHOD		11	/* no such method */
#define XVMERR_NOSUCHFIELD		12	/* no such field */
#define XVMERR_MEMBERACCESS		13	/* member access */

#define XVMERR_THROWEXCEPTION		14	/* throw except */
#define XVMERR_UNCATCHED		15	/* uncatched err */
#define XVMERR_THREADRESUME		16	/* thread resume */
#define XVMERR_THREADSWITCH		17	/* thread switch */
#define XVMERR_MONITOR			18	/* monitor err */

#define XVMERR_JLOGBUF			19	/* jlog buf err */
#define XVMERR_ASSERT			20	/* assert err */
#define XVMERR_FILEMAP			21	/* filemap err */

#endif	/* !__XVMSTATE_H__ */
