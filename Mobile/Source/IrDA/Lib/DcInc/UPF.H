/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       upf.h                                        */
/*             Created:    7/25/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/

typedef	struct	t_UPFINF {
	byte	SizeH[2];		/* 2	2	*/	
	byte	SizeV[2];		/* 2	4	*/
	byte	PixcelConf;		/* 1	5	*/
	byte	Rotation;		/* 1	6	*/
	byte	Reserve1;		/* 1	7	*/
	byte	CompRatio;		/* 1	8	*/
	byte	WhiteLevel;		/* 1	9	*/
	byte	InputDevice;	/* 1	10	*/
	byte	Reserve2[3];	/* 3	13	*/
	byte	DummyData;		/* 1	14	*/
	byte	XBegin[2];		/* 2	16	*/
	byte	YBegin[2];		/* 2	18	*/
	byte	XSize[2];		/* 2	20	*/
	byte	YSize[2];		/* 2	22	*/
	byte	NoComp;			/* 1	23	*/
	byte	Reserve3[3];	/* 3	26	*/
} UPFINF;

typedef struct t_UPFENTRY{
	byte	Offset[4];		/* 4	4	*/
	byte	Size[4];		/* 4	8	*/
	byte	DataID;			/* 1	9	*/
	byte	Rsv0;			/* 1	10	*/
	byte	Data[26];		/* 26	36	*/
} UPFENTRY;

typedef struct t_UPFHDR {
	byte	GenDecl[8];		/* 8	8	*/
	byte	FileDecl[8];	/* 8	16	*/
	byte	FileID[2];		/* 2	18	*/
	byte	FileVer[2];		/* 2	20	*/
	byte	AppCreate[8];	/* 8	28	*/
	byte	AppUpdate[8];	/* 8	36	*/
	byte	MakerCreate[4];	/* 4	40	*/
	byte	MakerUpdate[4];	/* 4	44	*/
	byte	Rsv0[16];		/* 16	60	*/
	byte	DataEntry;		/* 1	61	*/
	byte	TableMax;		/* 1	62	*/
	byte	Rsv1;			/* 1	63	*/
	byte	CharCode;		/* 1	64	*/
	byte	Title[128];		/* 128	192	*/
	byte	Rsv2[48];		/* 48	240	*/
	byte	Entry[4][36];	/* 144	384	*/
} UPFHDR;

