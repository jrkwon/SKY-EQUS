/********************************************************************

	IrAPP Apprication convinience Program

*********************************************************************/
#ifndef	_IRDAUTY_H_
#define	_IRDAUTY_H_
extern	void	IrAPP_DisplayDeviceName(int no );
extern	void	IrAPP_SetIASClass(char *class,char *attribute,int lsap );
extern	IrDABOOL	IrAPP_DiscoveryDevice(void);
extern	int		IrAPP_FoundServiceHint(int hintBit);
extern	int		IrAPP_QueryLSAP(int no ,char *class,char *attr ,dword *daddr,int *dlsap);
extern	void	IrAPP_OpenStack(void);
extern	void	IrAPP_CloseStack(void);
extern	void	IrAPP_SetDeviceName( const char *name, const char *lname ) ;
extern	void	IrAPP_Pause(dword tout);
extern	void	IrAPP_SetPortConfig(void);

#endif	/* _IRDAUTY_H_ */
