/*

	Console and Modem Port Funcions

*/


extern	void	cbaud(long baud);
extern	int		cgetc(void );
extern	int		cget(void );
extern	void	cputc(int c);
extern	int		cGetLine(void);

extern	int		cgets(char *cp,int len);
extern	void	cputs(const char *cp);
extern	void	cprintf(const char *fmt,...);
extern	void	_DPRINTF(int level,const char *fmt,...);
extern  void	Monitor(void);
