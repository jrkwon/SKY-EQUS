/********************************************************************

	IrAPP Apprication convinience Program

*********************************************************************/

typedef struct t_AUTODISCOVERY{
	int				count;						/* �f�B�X�J�o����	*/
	int				slot;						/* �X���b�g��		*/
	int				interval;					/* �f�B�X�J�o���Ԋu	*/
	int				*pResult;					/* ������			*/
	DISCOVERYLOG	*pLog;						/* ���O�̏ꏊ		*/
	int				(*pConfFunc)(int result);	/* �R�[���o�b�N�֐�	*/
	void			(*pIndFunc)(int sw);		/* �\���֐�			*/
} AUTODISCOVERY;

extern	int	IrAPP_GetAutoDiscoveryStat	(void);
extern	int	IrAPP_AbortAutoDiscovery	(int result);
extern	int	IrAPP_SetAutoDiscovery		(AUTODISCOVERY *p);

typedef struct t_IASGETVALUE{
	dword		Addr;							/* �ڑ���A�h���X	*/
	const char	*pClass;						/* �N���X��			*/
	const char	*pAttribute;					/* �A�g���r���[�g��	*/
	void		*pData;							/* �f�[�^�̏ꏊ		*/
	int			*pDataSize;						/* �f�[�^�T�C�Y		*/
	IASVALUEBYCLASS	*pList;						/* ���X�g�̏ꏊ		*/
	int			*pListSize;						/* ���X�g�̃T�C�Y	*/
	void		(*pConfFunc)(int result );		/* �R�[���o�b�N�֐�	*/
} IASGETVALUE;

extern	int	IrAPP_GetValueByClass	(IASGETVALUE *p);

extern	int	IrAPP_AppOpenReq(	dword daddr,int dlsap,						/* �f�o�C�X�A�h���X	�k�r�`�o	*/
								int		(*OpenFunc)(dword addr,int dlsap),	/* �I�[�v������֐�				*/
								void	(*ConfFunc)(int res)				/* �R���t�@�[���֐�				*/
	);

extern	void	IrAPP_Initialize(void);