/********************************************************************

	IrAPP Apprication convinience Program

*********************************************************************/

typedef struct t_AUTODISCOVERY{
	int				count;						/* ディスカバリ回数	*/
	int				slot;						/* スロット数		*/
	int				interval;					/* ディスカバリ間隔	*/
	int				*pResult;					/* 発見個数			*/
	DISCOVERYLOG	*pLog;						/* ログの場所		*/
	int				(*pConfFunc)(int result);	/* コールバック関数	*/
	void			(*pIndFunc)(int sw);		/* 表示関数			*/
} AUTODISCOVERY;

extern	int	IrAPP_GetAutoDiscoveryStat	(void);
extern	int	IrAPP_AbortAutoDiscovery	(int result);
extern	int	IrAPP_SetAutoDiscovery		(AUTODISCOVERY *p);

typedef struct t_IASGETVALUE{
	dword		Addr;							/* 接続先アドレス	*/
	const char	*pClass;						/* クラス名			*/
	const char	*pAttribute;					/* アトリビュート名	*/
	void		*pData;							/* データの場所		*/
	int			*pDataSize;						/* データサイズ		*/
	IASVALUEBYCLASS	*pList;						/* リストの場所		*/
	int			*pListSize;						/* リストのサイズ	*/
	void		(*pConfFunc)(int result );		/* コールバック関数	*/
} IASGETVALUE;

extern	int	IrAPP_GetValueByClass	(IASGETVALUE *p);

extern	int	IrAPP_AppOpenReq(	dword daddr,int dlsap,						/* デバイスアドレス	ＬＳＡＰ	*/
								int		(*OpenFunc)(dword addr,int dlsap),	/* オープンする関数				*/
								void	(*ConfFunc)(int res)				/* コンファーム関数				*/
	);

extern	void	IrAPP_Initialize(void);
