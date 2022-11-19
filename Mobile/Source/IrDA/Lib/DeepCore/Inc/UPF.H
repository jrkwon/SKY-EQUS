


typedef	struct	t_UPFINF {
	byte	SizeH[2];			
	byte	SizeV[2];		
	byte	PixcelConf;		
	byte	Rotation;		
	byte	Reserve1;		
	byte	CompRatio;		
	byte	WhiteLevel;		
	byte	InputDevice;	
	byte	Reserve2[3];	
	byte	DummyData;		
	byte	XBegin[2];		
	byte	YBegin[2];		
	byte	XSize[2];		
	byte	YSize[2];		
	byte	NoComp;			
	byte	Reserve3[3];	
} UPFINF;


typedef struct t_UPFENTRY{
	byte	Offset[4];		
	byte	Size[4];		
	byte	DataID;			
	byte	Rsv0;			
	byte	Data[26];		
} UPFENTRY;


typedef struct t_UPFHDR {
	byte	GenDecl[8];		
	byte	FileDecl[8];	
	byte	FileID[2];		
	byte	FileVer[2];		
	byte	AppCreate[8];	
	byte	AppUpdate[8];	
	byte	MakerCreate[4];	
	byte	MakerUpdate[4];	
	byte	Rsv0[16];		
	byte	DataEntry;		
	byte	TableMax;		
	byte	Rsv1;			
	byte	CharCode;		
	byte	Title[128];		
	byte	Rsv2[48];		
	byte	Entry[4][36];	
} UPFHDR;
