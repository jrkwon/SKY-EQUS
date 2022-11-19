/*
** ============================================================================
**
** Project: 
**     BUNNY
**
** File:
**     sky_file.h
**
** Description:
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-11-29 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_FILE_H_
#define _SKY_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define MAX_PHOTO_COUNT        50
/*----------------------------------------------------------------------------
**  Directory
*/
/* SKY ui root directory */
#define SKY_ROOT_DIR                (BYTE*)"SKY"

/* system directroy */
#define SKY_SYS_DIR                 SKY_ROOT_DIR""SKY_DIR_CHAR"SYS"

/* application directroy */
#define SKY_PBK_DIR                 SKY_ROOT_DIR""SKY_DIR_CHAR"PBK"
/* 
#define SKY_XXXX_DIR                SKY_ROOT_DIR""SKY_DIR_CHAR"XXX"
*/

/* melody data directory */
#define SKY_MELODY_DIR              SKY_ROOT_DIR""SKY_DIR_CHAR"MEL"
/* animation, bitmap image data directory */
#define SKY_IMAGE_DIR               SKY_ROOT_DIR""SKY_DIR_CHAR"IMG"

#ifdef FEATURE_CHARACBELL
/* characbell directory */
#define SKY_CHARACBELL_DIR          SKY_ROOT_DIR""SKY_DIR_CHAR"CHA"
/* characbell melody directory */
#define SKY_CHA_MELODY_DIR          SKY_CHARACBELL_DIR""SKY_DIR_CHAR"MEL"
/* characbell image directory */
#define SKY_CHA_IMAGE_DIR           SKY_CHARACBELL_DIR""SKY_DIR_CHAR"IMG"
#endif // FEATURE_CHARACBELL

/* GVM directory */
#define SKY_GVM_DIR                 SKY_ROOT_DIR""SKY_DIR_CHAR"GVM"
#define SKY_MW_DIR                 SKY_ROOT_DIR""SKY_DIR_CHAR"MW"
/* JAR(Java) directory */
//#define SKY_JAR_DIR                 SKY_ROOT_DIR""SKY_DIR_CHAR"JAR"

/* rosa 00-12-18 --> @why, what */
/* Scheduler directroy */
#define SKY_SCHL_DIR                SKY_ROOT_DIR""SKY_DIR_CHAR"SCH"
/* <-- */

/* reo 00-01-017 {-- */
/* SMS MO SEND & MO SENT Message directory */
#define SKY_SMSMO_DIR              SKY_ROOT_DIR""SKY_DIR_CHAR"SMO"
/* reo 00-01-17 --} */

/* VMS directory */
#define SKY_VMS_DIR                 SKY_ROOT_DIR""SKY_DIR_CHAR"VMS"

/* MYBELL directory */
#define SKY_MBL_DIR                 SKY_ROOT_DIR""SKY_DIR_CHAR"MBL"

/* CAMERA PHOTO directory*/
#define SKY_PHOTO_DIR               SKY_ROOT_DIR""SKY_DIR_CHAR"PHT"

#define SKY_CALLSTACK_DIR           SKY_ROOT_DIR""SKY_DIR_CHAR"CST"

/* MAIL directory*/
//FEATURE_WAPMAIL
#define SKY_MAIL_DIR				SKY_ROOT_DIR""SKY_DIR_CHAR"MAL"

#ifdef FEATURE_SKTT_PM
/* Video Mail directroy */
#define SKY_VM_DIR	                SKY_ROOT_DIR""SKY_DIR_CHAR"VM"
#endif

#define SKY_CALLSTACK_DIR           SKY_ROOT_DIR""SKY_DIR_CHAR"CST"

#ifdef	FEATURE_WAVELET	//	yoursun 01/06/08
/* WAVELET directory*/
#define SKY_WAVELET_DIR             SKY_ROOT_DIR""SKY_DIR_CHAR"WAV"//	khekim 01/01/16
/* WAVELET Temporary directory*/
#define SKY_TEMPWAV_DIR             SKY_ROOT_DIR""SKY_DIR_CHAR"TWA"//	yoursun/01/09/25
#endif	//	#ifdef	FEATURE_WAVELET

#ifdef FEATURE_SKVM
#define SKY_SKVM_DIR				(BYTE*)"xce"
#endif
/*----------------------------------------------------------------------------
**  Maximum value
**  - maximum values of the file stuffs is defined in fs_parm.h but 
**    to minimize the amount of memory usage, redefine these value
*/
#define SKY_FILE_EXT_NAME_MAX_LENGTH  (17+1)  /* file name + ext name */
#define SKY_FILE_NAME_MAX_LENGTH      (14+1)  /* file name only */
#define SKY_EXT_NAME_MAX_LENGTH       (3+1)   /* ext name only */
#define SKY_PATH_NAME_MAX_LENGTH      (40+1)  /* maximum length of pathname */
/* the maximum number of files each of melody and animation data */
#define SKY_MAX_NUM_FILES             (50)    
/* the allocated byte size for melody and animation data downloaded */
#if SKY_MODEL_ID == SKY_MODEL_B0
#define SKY_MAX_EFS_SIZE              (250*1024)      /* 250 K */
#elif (SKY_MODEL_ID == SKY_MODEL_B1)
/*
** auto answer/voice memo : 300K
** SMS message            :  10K
** SMS voice/my bell ...  :  10K
** NV item                :  40K
** phone book             : 140K
** schedular              :  10K
** recent call list       :   2K
** system                 :   2K
** game                   :  14.4K
** -------------------------------
** sub total              : 514.4K
**
** EFS overhead           : 128K
** reserved for changes   : 256K
** -------------------------------
** grand total              898.4K
**
** used space in EFS data area for B1 
**                          915K
**                          ====
*/
#define SKY_MAX_EFS_SIZE              ((2048-915)*1024)
#elif (SKY_MODEL_ID == SKY_MODEL_B2)
/*
** auto answer/voice memo : 300K
** SMS message            :  16K(rcv: 6K, send: 5K, sent: 5K)
** SMS voice/my bell ...  : 2.5K(voice: 1K, mybell: 1.5K)
** NV item                :  40K
** phone book             : 220K(140K+80K(추가분))
** schedular              :  10K
** recent call list       :   3K(2K+1K(추가분))
** system                 :   2K
** game                   :  14.4K
** characbell             :   1K
** -------------------------------
** sub total              : 608.9K
**
** EFS overhead           : 128K
** reserved for changes   : 256K
** -------------------------------
** grand total              992.9K
**
** used space in EFS data area for B1 
**                          1024K (992.9 + reserved)
**                          =====
*/
#define sMAX_RESERVED_EFS_SIZE        1024
#define sMAX_WAVELET_TEMP_SIZE		   180
#define SKY_MAX_EFS_SIZE              ((2048-sMAX_RESERVED_EFS_SIZE-sMAX_WAVELET_TEMP_SIZE)*1024)
#endif

#if MODEL_ID == MODEL_B0
#define	MM_NUM_FILE_TYPE		4
#else
#define	MM_NUM_FILE_TYPE		0
#endif
/*----------------------------------------------------------------------------
**  Misc.
*/
#define HEADER_RESERVED_SIZE        14

#define SKY_MAX_CAMERA_IMAGE_FILE_LEN   16 //redstar 2001.02.22
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/*----------------------------------------------------------------------------
** 버전 관리를 해 주어야 하는 파일. 
** image나 melody 같은 데이터는 버전 관리할 필요없다 
*/
enum tagSKY_FILE_LIST_E
{
    SFL_EXT,            /* 기타 정보 파일 */
    SFL_SYS,            /* 시스템 정보 */
    SFL_MISC,           /* 부가 기능 */
    SFL_APP,            /* 기본 응용프로그램 */
    SFL_GVM,            /* GVM 정보 파일 */
//	SFL_MWALLET,
//	SFL_MWTEMP1,		/* 전자 지갑 정보 파일 */
//	SFL_MWTEMP2,		/* 전자 지갑 정보 파일 */
//	SFL_MWMYCERT1,		/* 전자 지갑 정보 파일 */
//	SFL_MWMYCERT2,		/* 전자 지갑 정보 파일 */
//	SFL_MWMYCERT3,		/* 전자 지갑 정보 파일 */
//	SFL_MWMYCERT4,		/* 전자 지갑 정보 파일 */
//	SFL_MWMYCERT5,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT1,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT2,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT3,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT4,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT5,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT6,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT7,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT8,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT9,		/* 전자 지갑 정보 파일 */
//	SFL_MWTCACERT10,		/* 전자 지갑 정보 파일 */
//	SFL_MWALLET,		/* 전자 지갑 정보 파일 */
    SFL_PBOOK,          /* 전화부 */
    SFL_MAX
};
typedef enum tagSKY_FILE_LIST_E         SKY_FILE_LIST_E;

typedef struct tagSKY_FILE_VERSION_T    SKY_FILE_VERSION_T;
typedef struct tagSKY_FILE_HEADER_T     SKY_FILE_HEADER_T;
typedef struct tagSKY_MMFILE_HANDLE_T   SKY_MMFILE_HANDLE_T;

/*----------------------------------------------------------------------------
** 버전 관리가 필요없는 데이터 파일
** melody/image/GVM/WAVELET
*/
enum tagSKY_DATA_FILE_E 
{
    SDF_MELODY,         /* 멜로디 */
    SDF_IMAGE,          /* 이미지/애니메이션 */
#ifdef FEATURE_CHARACBELL
    SDF_CHA_MELODY,      /* 캐릭벨 */
    SDF_CHA_IMAGE,      /* 캐릭벨 */
#endif // FEATURE_CHARACBELL
    SDF_GVM,            /* GVM 데이터 */
    SDF_PHOTO,          /* 사진 이미지 */
//FEATURE_WAPMAIL
    SDF_MAIL,           /* MAIL데이터 */
#ifdef FEATURE_SKTT_PM
	SDF_VM,				/* Video Mail데이터 */
#endif
#ifdef	FEATURE_WAVELET	//	khekim 01/01/16
    SDF_WAVELET,
	SDF_TEMPWAV,
#endif	//	#ifdef	FEATURE_WAVELET
    SDF_MAX
};
typedef enum tagSKY_DATA_FILE_E         SKY_DATA_FILE_E;

/*----------------------------------------------------------------------------
** 멜로디/이미지/GVM 데이터 관리를 위한 함수의 리턴 값
*/
enum tagSKY_DATA_FILE_RESULT_E 
{
    SDR_OK              = 1,    /* OK */
    SDR_NO              = 0,    /* general error */
    SDR_OUT_OF_SPACE    = -1,   /* out of reserved EFS area */
    SDR_FILE_EXIST      = -2,   /* the file is already exist */
    SDR_NO_FILE         = -3,   /* the file isn't exist */
    SDR_TOO_MANY_FILES  = -3,   /* too many files */
    SDR_WRITE_ERROR     = -4,   /* write error */
    SDR_READ_ERROR      = -5,   /* read error */
    SDR_OPEN_ERROR      = -6    /* open error */
};
typedef enum tagSKY_DATA_FILE_RESULT_E  SKY_DATA_FILE_RESULT_E;

#define IS_SDR_OK(r)             ((r) > 0)
#define IS_SDR_ERROR(r)          ((r) <= 0)

enum FILENAME_VALID_E {
    FV_OK,          /* valid file name */
    FV_NULL,        /* null file name */
    FV_DIRCHAR,     /* DIR CHAR included */
    FV_2LONG,       /* too long file name */
    FV_MAX
};
typedef enum FILENAME_VALID_E     FILENAME_VALID_E;

enum tagSKY_FILEOPEN_TYPE_E {
    SFT_READ,
    SFT_WRITE
};
typedef enum tagSKY_FILEOPEN_TYPE_E SKY_FILEOPEN_TYPE_E;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
/*----------------------------------------------------------------------------
**  SKY 파일 버전
**  
**  MAJOR : 
**      파일에 저장하는 자료 구조 자체가 달라지거나, 아니면, 자료 구조 중간에
**      새 항목이 추가되는 경우에 변경해 준다.
**      이 값이 변경되면, 해당 파일을 완전히 삭제하고 새로 만들어 준다.
**      해당 파일을 완전히 삭제하고 새로 만들어야 함
**  MINOR : 
**      새 항목이 기존 파일의 가장 끝에 추가되는 경우에는 이 값을 변경해 준다.
**
**  이 버전 정보는 항상 저장된 파일의 정보와 동기시켜주어야 한다.      
*/
struct tagSKY_FILE_VERSION_T
{
    INT8    nMajor;
    INT8    nMinor;
};

#define SKY_GET_FILE_MAJOR_VERSION(s)       (g_aFileVersion[s]).nMajor
#define SKY_GET_FILE_MINOR_VERSION(s)       (g_aFileVersion[s]).nMinor


/*----------------------------------------------------------------------------
**  SKY 파일 헤더
**  
**  SKY_FILE_VERSION_T
**  reserved area - 14 byte    
*/
struct tagSKY_FILE_HEADER_T
{
    SKY_FILE_VERSION_T  Version;
    BYTE                abReserved[HEADER_RESERVED_SIZE];
};

#define SKY_GET_FILE_HEADER(s)          (s).Header

struct tagSKY_MMFILE_HANDLE_T{
	BYTE			FPathName[SKY_PATH_NAME_MAX_LENGTH];
	WORD			FilePos;    /* The current file position */
	WORD			FileSize;   /* The file size */
	WORD			MaxBlockSize;	/* The length of writing or reading data */
	HFILE           Handle;     /* File handle */
	BYTE			*pBlock;    /* The pointer of data to be written or read */
};
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern ROM SKY_FILE_VERSION_T   g_aFileVersion[];

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void  SKY_API SKY_InitDataDir(void);
extern void  SKY_API SKY_RenameDataFile(SKY_DATA_FILE_E SkyDataFile, BYTE *pszOldName, BYTE *pszNewName);
extern SKY_DATA_FILE_RESULT_E  SKY_API SKY_DeleteDataFile(SKY_DATA_FILE_E SkyDataFile, BYTE *pszFileName);
extern void  SKY_API SKY_DeleteDataFiles(SKY_DATA_FILE_E SkyDataFile);
extern void  SKY_API SKY_DeleteAllDataFile(void);
extern UINT8 SKY_API SKY_GetFileExtList(const BYTE *pszDirName, 
                                        //BYTE *pszFileNameList,
                                        BYTE *paszFileExtNameList[]);
extern UINT8 SKY_API SKY_GetFileList(const BYTE *pszDirName, 
                                     //BYTE *pszFileNameList,
                                     BYTE *paszFileNameList[]);
extern DWORD SKY_API SKY_GetUsedSpaceInDataFile(SKY_DATA_FILE_E SkyDataFile);
extern DWORD SKY_API SKY_GetFreeSpaceInDataFile(void);
extern WORD  SKY_API SKY_GetFileNumberInDataFile(SKY_DATA_FILE_E SkyDataFile);

extern SKY_DATA_FILE_RESULT_E SKY_API SKY_WriteDataFile(SKY_DATA_FILE_E SkyDataFile,
                                                        CONST BYTE* pszFileName, 
                                                        DWORD dwBlockSize,
                                                        CONST void* pBlockData);
extern SKY_DATA_FILE_RESULT_E SKY_API SKY_ReadDataFile(SKY_DATA_FILE_E SkyDataFile,
                                                       CONST BYTE* pszFileName,
                                                       DWORD dwBlockSize,      
                                                       void* pBlockDataToRead);
extern SKY_DATA_FILE_RESULT_E SKY_API SKY_CreateDataFile(SKY_DATA_FILE_E SkyDataFile,
                                                         CONST BYTE* pszFileName,
                                                         HFILE *phFile); 
extern SKY_DATA_FILE_RESULT_E SKY_API SKY_OpenDataFile(SKY_DATA_FILE_E SkyDataFile,
                                                       CONST BYTE* pszFileName,
                                                       HFILE *phFile,
                                                       SKY_FILEOPEN_TYPE_E openType) ;
extern DWORD SKY_API SKY_GetDataFileSize(SKY_DATA_FILE_E SkyDataFile,
                                  CONST BYTE* pszFileName);
extern BOOL SKY_API SKY_IsDataFile(SKY_DATA_FILE_E SkyDataFile,
                                  CONST BYTE* pszFileName);
extern void  SKY_API SKY_CloseDataFile(HFILE hFile);
extern DWORD SKY_API SKY_ReadDataBlock(HFILE hFile, void *pBufferToRead, DWORD dwSize);
extern DWORD SKY_API SKY_WriteDataBlock(HFILE hFile, CONST void *pBufferToWrite, DWORD dwSize);

extern void SKY_API mm_BellFileLoadClose(void);
extern UINT8 SKY_API mm_BellFileLoadOpen(BYTE *pFileName, 
					             DWORD *pFileSize,
					             BYTE **pAllocBlock);
extern void mm_BellFileBlockSeek(DWORD Position);
extern UINT8 mm_BellFileBlockLoad(WORD BlockSize);


extern void ConvertCameraImageFileName(BYTE* pszFileName, DWORD dwIndex); //redstar  2001.02.22
extern HFILE SKY_MakeCameraImageFile(DWORD dwIndex);
extern FILENAME_VALID_E SKY_API SKY_IsValidFileName(const BYTE* pszFileName);
extern WORD SKY_API SKY_GetFileNumberInAllDataFile(void);
extern VOID SKY_SetCameraCount(VOID);       //white 2001.05.17
extern BOOL SKY_IsCameraData(CHAR* szName); //white 2001.06.11
extern void SKY_RemoveSKVMAllData(CONST BYTE* pszDirName) ; // tortol 2001.10.10

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_FILE_H_ */
