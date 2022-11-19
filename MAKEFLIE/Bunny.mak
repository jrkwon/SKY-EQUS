#============================================================================
#  Name:
#    Bunny.MAK
#
#  Description:
#    Makefile to build the DMSS software for the SURF (Subscriber Unit
#    Reference Design) using the MSM5000 (ARM) ASIC.
#    This is a Cellular build with IS95B, NV Item Manager and Embedded
#    File System.
#
#   The following nmake targets are available in this makefile:
#
#     all           - make .elf and .hex image files (default)
#     clean         - delete object directory and image files
#     depend        - update makefile dependencies
#     test          - list flags for assembler, compiler and linker
#     tools         - list ARM SDT tool versions
#     filename.mix  - make mixed source / assembly file
#     filename.o    - make object file
#
#   The above targets can be made with the following command:
#
#     nmake /f Bunny.mak [target]
#
# Assumptions:
#
# Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
#----------------------------------------------------------------------------
#============================================================================
#
#                        EDIT HISTORY FOR MODULE
#
# $Header:   L:/src/asw/MSM5000/VCS/surf4ce.mav   1.3.1.0   19 Jan 2000 19:45:26   lchan  $
#
# when       who     what, where, why
# -------    ---     --------------------------------------------------------
# 11/15/00   nxb     Added new FS objs.
# 11/14/00   nxb     Added acplog.o
# 11/13/00   bgc     Updated for split MUX from common archive
# 11/06/00   nxb     Added uasmsu.o
# 11/06/00   ks      Moved tdso.o object to MC_OBJS. 
# 11/06/00   ak      Added pstimer object.
# 11/01/00   lcc     Added mccso object.
# 10/11/00   ts      Added mcnvdf object.     
# 00.12.04   jrkwon  Recreate for EQUS and SKY UI
# 00.07.24   jrkwon  Delete unused items
# 00.07.18   jrkwon  Add MMI_EQUS in MMI_TYPE
# 00.05.18   jrkwon  Add MMI_SKNUI in MMI_TYPE
# 00.05.17   jrkwon  2.0a update
# 00.04.10   jrkwon  QualComm TR1010 update
# 00.03.31   jrkwon  NUI_DIR --> NUI_ENGINE_DIR and NUI_APPLET_DIR
# 00.03.29   jrkwon  OPT - restore the original optimization option
# 00.03.27   jrkown  add NUI_DIR and NUI source code
# 00.03.20   jrkwon  OPT - no compiler optimization
# 00.03.03   chos    Audio 관련 module 추가
# 00.03.02   jnlee   EFS 관련 module 추가
# 00.03.02   chos    LCD, FONT 관련 module 추가
# -------    ---     --------------------------------------------------------
# 01/19/00   lcc     Removed TEMP_HACKS_FOR_TIGER.  Added atmr (commented).
#                    Updated dependency.
# 11/04/99   lcc     Change compiler flag from ASSERT_WARN to ASSERT_FATAL
#                    Added support for the sdevmap.c file.
#                    Applied TIMETEST to all compiler flags.
# 10/04/99   lcc     Initial release with MSM5000 support. Based on GV4015
#                    surf4ce.mak.
#
#============================================================================


#===============================================================================
#                         Target file name and type definitions
#===============================================================================
#-------------------------------------------------------------------------------
# Target file name 
#-------------------------------------------------------------------------------

MODEL_ID	= MODEL_B2#

TARGET		= Bunny#
TARGETDIR	= OBJ\Bunny#
MAKEFILE	= Bunny
CUSTNAME	= B2#
TARGETID	= T_G#
CUSTFILE	= CUST$(CUSTNAME).H#
#MAINBOARD	= -DT_SURF#

#TOOLFILE   = ADS101.MAKINC       # Make include file that specifies tools commands
#OBJ_FILE   = dmss_objs.makinc
#SCLFILE    = msm5000_1_5Mi.scl    # Target scatter load file

#GETDEP_SCRIPT  = $(QUALCOMMDIR)\getdep.pl
#MDEPEND_SCRIPT = $(QUALCOMMDIR)\mdepend.pl
#----># Software tool and environment definitions

# Target scatter load file for ADS linker
SCATTERDIR	= Source\ModelDep#
SCLFILE	= $(SCATTERDIR)\$(TARGET)ADS.scl#

################################################################################
#      Edit this area for Model dependent
#              --------------------------------------------->
################################################################################
#-------------------------------------------------------------------------------
# Select NV type
#-------------------------------------------------------------------------------

NV_TYPE	= EFS#

#-------------------------------------------------------------------------------
# Select EEPROM type
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Select PLL type
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Select MMI type		by jnlee 01.06.28
#-------------------------------------------------------------------------------

EFS_TYPE	= SK_TYPE#

#-------------------------------------------------------------------------------
# Select MMI type
#-------------------------------------------------------------------------------

MMI_TYPE	= SKY_UI#

#-------------------------------------------------------------------------------
# Select SKY model ID
#-------------------------------------------------------------------------------

SKY_MODEL_ID	= SKY_MODEL_B2#

#-------------------------------------------------------------------------------
# Select WAP compiling 
#-------------------------------------------------------------------------------

# WAP4.3 Compile
WAP2 = FEATURE_WAP4.8

!IFDEF WAP2
!ELSE
WAP = FEATURE_WAP
!ENDIF

#-------------------------------------------------------------------------------
# Select SocketApi compiling 
#-------------------------------------------------------------------------------

SOCKAPI	= USE_SOCKET#

#-------------------------------------------------------------------------------
# GVM compiling 
#-------------------------------------------------------------------------------

GVM	= USE_GVM#
EMAIL	= USE_EMAIL#

#-------------------------------------------------------------------------------
# Define symbols for photo mail 
#-------------------------------------------------------------------------------

PHOTOMAIL = -DUSE_PHOTOMAIL#

#-------------------------------------------------------------------------------
# SKVM compiling 
#-------------------------------------------------------------------------------

SKVM = -DUSE_SKVM

#-------------------------------------------------------------------------------
# Define symbols for VAS 
#-------------------------------------------------------------------------------

VAS	= -DUSE_VAS#
VAS_DEF	= -DFEATURE_VAS#

#-------------------------------------------------------------------------------
# If You Want to compile IrDA, 
#-------------------------------------------------------------------------------

IRDA	= USE_IRDA#

#-------------------------------------------------------------------------------
# Select EIF compiling
#-------------------------------------------------------------------------------

EIF	= USE_EIF#

#-------------------------------------------------------------------------------
# Define M-Commerce
#-------------------------------------------------------------------------------

MCOMMERCE = -DFEATURE_MCOMMERCE

#-------------------------------------------------------------------------------
# Define symbols
#-------------------------------------------------------------------------------

EQUS_SYMBOL_DEFINE	= -DCONFIG_EQS_COLOR_64K#
SKY_SYMBOL_DEFINE	= -DSKY_MODEL_ID=$(SKY_MODEL_ID) \
						$(EQUS_SYMBOL_DEFINE)#
MMI	= -DMMI_TYPE=$(MMI_TYPE)#

################################################################################
#      <---------------------------------------------
#              Edit this area for Model dependent
################################################################################


#===============================================================================
#                             TOOL DEFINITIONS   I
#===============================================================================
#-------------------------------------------------------------------------------
# Build process output type
#-------------------------------------------------------------------------------

EXETYPE	= elf#

#-------------------------------------------------------------------------------
# Software tool and environment definitions
#-------------------------------------------------------------------------------
ENV_FILE	= Env.makinc
!INCLUDE	$(ENV_FILE)#

QUALCOMMDIR = Source\QUALCOMM30#
DOWNLOADDIR	= .\DownLoad
#TARGETDIR	= OBJ\Bunny#

#ARMBIN	= $(ADS101_DIR)\BIN#
#위의 경로를 사용하면 perl에서 에러납니다.
#컴파일러 실행경로는 VS의 옵션을 사용하여 지정해 주세요.
#youandi 01.09.07
ARMINC	= $(ADS101_DIR)\INCLUDE#
ARMLIB	= $(ADS101_DIR)\LIB#

CC         = tcc                # ARM SDT Thumb 16-bit inst. set ANSI C compiler
ARMCC      = armcc              # ARM SDT ARM 32-bit inst. set ANSI C compiler
ASM        = armasm             # ARM SDT assembler
LD         = armlink            # ARM SDT linker
HEXTOOL    = fromelf            # ARM SDT utility to create hex file from image

BINTOOL	= $(DOWNLOADDIR)\hexbin2
MAKE_BIN= $(DOWNLOADDIR)\MakeBin

OBJ_CMD    = -o								# Command line option to specify output filename
DIRTCC     = perl $(QUALCOMMDIR)\dirtcc.pl     # Utility to include directory name in mobile.o
ASM_SCRIPT = $(QUALCOMMDIR)\asm.pl             # Perl script to include .h files in assembly
CAT_SCRIPT = $(QUALCOMMDIR)\cat.pl             # Perl script to print output for assembly files

ELF_FILE	= $(DOWNLOADDIR)\$(TARGET).$(EXETYPE)#
HEX_FILE	= $(DOWNLOADDIR)\$(TARGET).hex#
BIN_FILE	= $(DOWNLOADDIR)\$(TARGET).bin#
BOOT_BIN	= $(DOWNLOADDIR)\BootB2.bin#

GETDEP_SCRIPT  = $(QUALCOMMDIR)\getdep.pl
MDEPEND_SCRIPT = $(QUALCOMMDIR)\mdepend_iram.pl

#===============================================================================
#                         COMPILE FLAG DEFINITIONS
#===============================================================================
#-------------------------------------------------------------------------------
# Target compile time symbol definitions
#-------------------------------------------------------------------------------

!IFDEF IRDA                             # temperory ...  
TG      = -DTG=$(TARGETID) -DMAKE_IRDA  # Target identifier definition
!ELSE
TG      = -DTG=$(TARGETID)              # Target identifier definition
!ENDIF
CUSTH   = -DCUST_H=\"$(CUSTFILE)\" -DMODEL_DEP_H=\"Bunny.h\"# Feature include file definition
ARM_ASM = -D_ARM_ASM_                   # ARM assembly language selection
ARM     = -DT_ARM                       # ARM processor target
MSM     = -DT_MSM3 -DT_MSM5000
SURF    = -DT_SURF                      # SURF card target
EXTENSIONS	=

#-------------------------------------------------------------------------------
# Diagnostic support options (default = no diagnostics support)
#-------------------------------------------------------------------------------

TIMETEST     =                          # Disable TIMETEST profiling support
#TIMETEST     = -DTIMETEST              # Enable TIMETEST profiling support
SHOW_STAT    = -DSHOW_STAT              # Force LOCALs to visible global scope

ASSERT_FATAL = -DASSERT=ASSERT_FATAL    # Invoke ERR_FATAL for ASSERT
ASSERT_WARN  = -DASSERT=ASSERT_WARN     # Invoke ERR for ASSERT

#-------------------------------------------------------------------------------
# Diagnostic message options (default = no diagnostics messages)
#-------------------------------------------------------------------------------

MSG_LVL_LOW   = -DMSG_LEVEL=MSG_LVL_LOW         # ALL message priority levels
MSG_LVL_MED   = -DMSG_LEVEL=MSG_LVL_MED         # MED priority level and above
MSG_LVL_HIGH  = -DMSG_LEVEL=MSG_LVL_HIGH        # HIGH priority level and above
MSG_LVL_ERROR = -DMSG_LEVEL=MSG_LVL_ERROR       # ERROR priority level and above
MSG_LVL_NONE  = -DMSG_LEVEL=MSG_LVL_NONE        # NO messages

#-------------------------------------------------------------------------------
# Compiler symbol definitions
#-------------------------------------------------------------------------------

# Common definitions
DMSS_CFLAGS_COMMON = $(ARM) $(MSM) $(SURF) $(TG) $(CUSTH) $(EXTENSIONS) $(SHOW_STAT) $(TIMETEST) $(MMI) $(SKY_SYMBOL_DEFINE)
DMSS_CFLAGS_COMMON4EQUS = $(ARM) $(MSM) $(SURF) $(TG) $(CUSTH) $(EXTENSIONS) $(TIMETEST) $(MMI) $(EQUS_SYMBOL_DEFINE)
DMSS_CFLAGS_COMMON4SKY = $(ARM) $(MSM) $(SURF) $(TG) $(CUSTH) $(EXTENSIONS) $(TIMETEST) $(MMI) $(SKY_SYMBOL_DEFINE)

#-------------------------------------------------------------------------------
# IrDA added definitions
#-------------------------------------------------------------------------------
!IFDEF IRDA
DMSS_CFLAGS_COMMON = $(DMSS_CFLAGS_COMMON) -DUSE_IRDA
DMSS_CFLAGS_COMMON4SKY = $(DMSS_CFLAGS_COMMON4SKY) -DUSE_IRDA
!ENDIF

#-------------------------------------------------------------------------------
# WAVELET added definitions
#-------------------------------------------------------------------------------
!IFDEF WAVELET
SCLFILE      = $(SCATTER_DIR)\$(TARGET_NAME).scl_wav 
DMSS_CFLAGS_COMMON = $(DMSS_CFLAGS_COMMON) -DFEATURE_WAVELET
DMSS_CFLAGS_COMMON4EQUS = $(DMSS_CFLAGS_COMMON4EQUS) -DFEATURE_WAVELET
DMSS_CFLAGS_COMMON4SKY = $(DMSS_CFLAGS_COMMON4SKY) -DFEATURE_WAVELET
!ENDIF

#-------------------------------------------------------------------------------
# VAS added definitions
#-------------------------------------------------------------------------------
!IFDEF VAS
DMSS_CFLAGS_COMMON = $(DMSS_CFLAGS_COMMON) -DFEATURE_VAS
DMSS_CFLAGS_COMMON4SKY = $(DMSS_CFLAGS_COMMON4SKY) -DFEATURE_VAS
!ENDIF

# Default definitions
DMSS_CFLAGS = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_ERROR)
DMSS_CFLAGS_MSG_LVL_MED   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_MED)
DMSS_CFLAGS_MSG_LVL_HIGH  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_HIGH)
DMSS_CFLAGS_MSG_LVL_NONE  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)

# Module specific definitions
DMSS_CFLAGS_ACP  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_DS   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_MED)
DMSS_CFLAGS_RLP  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_MC   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_MOB  = $(DMSS_CFLAGS_COMMON)
DMSS_CFLAGS_NONE = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_NV   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_RXTX = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SND  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SRCH = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_UI   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_CM   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_VOC  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_MED)
DMSS_CFLAGS_SIO  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_DEC  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_ENC  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_EQUS = $(DMSS_CFLAGS_COMMON4EQUS) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SKY  = $(DMSS_CFLAGS_COMMON4SKY) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_IRDA = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SMB  = $(DMSS_CFLAGS_COMMON) $(TIMETEST) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_WAP  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_MED)
DMSS_CFLAGS_GVM  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SKVM  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_ERROR)
DMSS_CFLAGS_VAS  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_HIGH)
DMSS_CFLAGS_MW  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)

#-------------------------------------------------------------------------------
# WAP added definitions
#-------------------------------------------------------------------------------
!IFDEF WAP
#WAP_DEF     = -DWAPPER -DWAP_NEO -DUSE_PROPRIETARY_WMLS_LIBS -DTRGT_REX -DWAP_B2 -DWAP_USE_DORM -DWAP_USE_SECURE -DWAP_USE_LEAK
#WAP_DEF     = -DWAPPER -DWAP_NEO -DUSE_PROPRIETARY_WMLS_LIBS -DTRGT_REX -DWAP_B2 -DWAP_USE_DORM -DWAP_USE_SECURE -DWAP_USE_LEAK -DFEATURE_SK_VM -DFEATURE_WAP_WAVELET -DQUALCOM30_WAP
WAP_DEF     = -DWAPPER -DWAP_NEO -DUSE_PROPRIETARY_WMLS_LIBS -DTRGT_REX -DWAP_B2 -DWAP_USE_DORM -DWAP_USE_SECURE -DWAP_USE_LEAK -DQUALCOM30_WAP
WAP_NEW     = -DWAP_USE_SKT_NEW
WAP_95C     = -DWAP_USE_PZID2
#WAP_LOG     = -DLOG_ENABLE
#WAP_WARNING = -DWAPCORE_FOR_WARNING
DMSS_CFLAGS_COMMON = $(DMSS_CFLAGS_COMMON) $(WAP_DEF) $(WAP_LOG) $(WAP_INIT) $(WAP_95C) $(WAP_NEW)
DMSS_CFLAGS_SKY = $(DMSS_CFLAGS_SKY) $(WAP_DEF) $(WAP_LOG) $(WAP_INIT) $(WAP_95C) $(WAP_NEW)
!ENDIF

#-------------------------------------------------------------------------------
# WAP2 added definitions
#-------------------------------------------------------------------------------
!IFDEF WAP2
WAP_DEF     = -DFEATURE_WAP -DTRGT_REX -DUSE_WIP_MALLOC -DUSE_PROPRIETARY_WMLS_LIBS
WAP_LOG     = -DLOG_ENABLE
DMSS_CFLAGS_COMMON = $(DMSS_CFLAGS_COMMON) $(WAP_DEF) $(WAP_LOG)
DMSS_CFLAGS_SKY = $(DMSS_CFLAGS_SKY) $(WAP_DEF) $(WAP_LOG)
!ENDIF

#-------------------------------------------------------------------------------
# Assembler symbol definitions
#-------------------------------------------------------------------------------

DMSS_AFLAGS = $(ARM) $(MSM) $(TG) $(CUSTH) $(APCS) $(EXTENSIONS) $(ARM_ASM) $(TIMETEST)


#===============================================================================
#                             TOOL DEFINITIONS	II
#===============================================================================
#						EQUS / SKY / WAP / IrDA directories
#						Include file search path options
#===============================================================================
#-------------------------------------------------------------------------------
# Define EQUS and SKY directories
#-------------------------------------------------------------------------------

EQUS_DIR    = Source\EQUS

EQUS_INC_DIR            = $(EQUS_DIR)\INCLUDE
EQUS_CTRL_DIR           = $(EQUS_DIR)\CONTROL
EQUS_CTRL_INC_DIR       = $(EQUS_CTRL_DIR)\INCLUDE
EQUS_DISPLAY_DIR        = $(EQUS_DIR)\DISPLAY
EQUS_FONT_DIR           = $(EQUS_DISPLAY_DIR)\FONT
EQUS_I18N_DIR           = $(EQUS_DIR)\I18N
EQUS_PLT_DIR            = $(EQUS_DIR)\PLATFORM
EQUS_PLT_REX_DIR        = $(EQUS_PLT_DIR)\REX
EQUS_RES_DIR            = $(EQUS_DIR)\RESOURCE
EQUS_RES_STR_DIR        = $(EQUS_RES_DIR)\STRING
EQUS_RES_IMG_DIR        = $(EQUS_RES_DIR)\IMAGE
EQUS_RES_IMG_BMP_DIR    = $(EQUS_RES_IMG_DIR)\BITMAP
EQUS_STATE_DIR          = $(EQUS_DIR)\STATE
EQUS_UTIL_DIR           = $(EQUS_DIR)\UTIL
EQUS_UTIL_MEM_DIR       = $(EQUS_UTIL_DIR)\MEMORY

SKY_DIR     = Source\SKY\$(CUSTNAME)

SKY_APP_DIR             = $(SKY_DIR)\APPLET
SKY_APP_INC_DIR         = $(SKY_APP_DIR)\INCLUDE
SKY_APP_ALARM_DIR       = $(SKY_APP_DIR)\ALARM
SKY_APP_AUTOANS_DIR     = $(SKY_APP_DIR)\AUTOANS
SKY_APP_BELL_VIB_DIR    = $(SKY_APP_DIR)\BELL_VIB
SKY_APP_CALC_DIR        = $(SKY_APP_DIR)\CALC
SKY_APP_CTRL_DIR        = $(SKY_APP_DIR)\CONTROL
SKY_APP_EMAIL_DIR       = $(SKY_APP_DIR)\EMAIL
SKY_APP_GAMEAPP_DIR     = $(SKY_APP_DIR)\GAME
SKY_APP_IRDA_DIR        = $(SKY_APP_DIR)\IRDA
SKY_APP_MEMO_DIR        = $(SKY_APP_DIR)\MEMO
SKY_APP_MENU_DIR        = $(SKY_APP_DIR)\MENU
SKY_APP_SCHEDULE_DIR    = $(SKY_APP_DIR)\SCHEDULE
SKY_APP_EIF_DIR         = $(SKY_APP_DIR)\EIF
SKY_APP_PBOOK_DIR       = $(SKY_APP_DIR)\PBOOK
SKY_APP_RECPLAY_DIR     = $(SKY_APP_DIR)\RECPLAY
SKY_APP_SMS_DIR         = $(SKY_APP_DIR)\SMS
SKY_APP_SYS_DIR         = $(SKY_APP_DIR)\SYSTEM
SKY_APP_SETTING_DIR     = $(SKY_APP_DIR)\SETTING
SKY_APP_SWATCH_DIR	    = $(SKY_APP_DIR)\SWATCH
SKY_APP_WTIME_DIR       = $(SKY_APP_DIR)\WORLDTIME
SKY_APP_VOCODER_DIR     = $(SKY_APP_DIR)\VOCODER
SKY_APP_TEST_DIR        = $(SKY_APP_DIR)\TEST
SKY_APP_DATACFG_DIR     = $(SKY_APP_DIR)\DATACFG
SKY_APP_PIC_DIR         = $(SKY_APP_DIR)\PICTURE
SKY_APP_CAMERA_DIR      = $(SKY_APP_DIR)\CAMERA
SKY_APP_DATAMGR_DIR     = $(SKY_APP_DIR)\DATAMGR
SKY_APP_CALLLIST_DIR    = $(SKY_APP_DIR)\CALLLIST
!IFDEF WAP
SKY_APP_WAP_DIR         = $(SKY_APP_DIR)\WAP_APP
!ENDIF
!IFDEF WAP2
SKY_APP_WAP_DIR         = $(SKY_APP_DIR)\WAP_APP
!ENDIF
#/* rosa 01-02-02 --> @why, what */
!IFDEF GVM
SKY_APP_GVM_DIR         = $(SKY_APP_DIR)\GVM
!ENDIF

#/* <-- */
!IFDEF MCOMMERCE
SKY_APP_MWALLET_DIR		= $(SKY_APP_DIR)\MWALLET
!ENDIF

!IFDEF SKVM
SKY_APP_SKVM_DIR         = $(SKY_APP_DIR)\SKVM
SKY_APP_SKVM_LIB_DIR         = $(SKY_APP_DIR)\SKVM\XVM_XCE_OBJS
!ENDIF

SKY_APP_GAME_LIB_DIR    = $(SKY_APP_DIR)\SJ_GAMES\GLib
SKY_APP_GAME_DIR        = $(SKY_APP_DIR)\SJ_GAMES\GameSrc

!IFDEF PHOTOMAIL
SKY_APP_PM_DIR          = $(SKY_APP_DIR)\VM
!ENDIF

!IFDEF VAS
SKY_APP_VAS_DIR         = $(SKY_APP_DIR)\VAS
!ENDIF

SKY_DEVICE_DIR          = $(SKY_DIR)\DEVICE
SKY_DEVICE_LCD_DIR      = $(SKY_DEVICE_DIR)\LCD
SKY_DEVICE_INC_DIR      = $(SKY_DEVICE_DIR)\INCLUDE
SKY_I18N_DIR            = $(SKY_DIR)\I18N
SKY_INC_DIR             = $(SKY_DIR)\INCLUDE
SKY_PLT_DIR             = $(SKY_DIR)\PLATFORM
SKY_PLT_REX_DIR         = $(SKY_PLT_DIR)\REX
SKY_RES_DIR             = $(SKY_DIR)\RESOURCE
SKY_RES_IMG_DIR         = $(SKY_RES_DIR)\IMAGE
SKY_RES_IMG_ANI_DIR     = $(SKY_RES_IMG_DIR)\ANI
SKY_RES_IMG_BMP_DIR     = $(SKY_RES_IMG_DIR)\BITMAP
SKY_RES_IMG_ICN_DIR     = $(SKY_RES_IMG_DIR)\ICON
SKY_RES_SND_DIR         = $(SKY_RES_DIR)\SOUND
SKY_RES_STR_DIR         = $(SKY_RES_DIR)\STRING
SKY_RES_MENU_DIR        = $(SKY_RES_DIR)\MENU
SKY_SYS_DIR             = $(SKY_DIR)\SYSTEM

# htg. 2001.1.5
SOCKAPI_DIR   = SOURCE\SOCKAPI
SOCKAPI_INC   = SORUCE\SOCKAPI 
#

#-------------------------------------------------------------------------------
# WAP 
#-------------------------------------------------------------------------------
!IFDEF WAP
WAP_DIR=SOURCE\WAP_B2
WAP_SRC=$(WAP_DIR)\CORE
WAP_INC=	-I $(WAP_DIR) \
			-I $(WAP_SRC)\INCLUDE \
	       	-I $(WAP_SRC)\SOURCE \
			-I $(WAP_SRC)\OPTIONAL\CHARSET \
			-I $(WAP_SRC)\OPTIONAL\MEMORY \
			-I $(WAP_DIR)\REX \
			-I $(WAP_DIR)\REX\CLNT \
			-I $(WAP_DIR)\REX\MMI \
			-I $(WAP_DIR)\REX\EIF \
			-I $(WAP_DIR)\SECURE \
!IFDEF MCOMMERCE
			-I SOURCE\Mcommerce\
!ENDIF #!IFDEF MCOMMERCE
			-I SOURCE\MEMORY \
!ENDIF

#-------------------------------------------------------------------------------
# WAP2 
#-------------------------------------------------------------------------------
!IFDEF WAP2
WAP_DIR=SOURCE\WAP4.8
WAP_SRC=$(WAP_DIR)\CORE
WAP_INC=	-I $(WAP_DIR) \
			-I $(WAP_SRC)\INCLUDE \
	       	-I $(WAP_SRC)\SOURCE \
			-I $(WAP_SRC)\OPTIONAL\CHARSET \
			-I $(WAP_SRC)\OPTIONAL\MEMORY \
			-I $(WAP_DIR)\SKY\
			-I $(WAP_DIR)\SKY\CLNT \
			-I $(WAP_DIR)\SKY\MMI \
!IFDEF MCOMMERCE
			-I SOURCE\Mcommerce\
!ENDIF #!IFDEF MCOMMERCE
			-I $(WAP_DIR)\SKY\EIF \
!ENDIF

!IFDEF IRDA                                     # Enable IRDA profiling support
IRDA_LIB_DIR = SOURCE\IRDA\LIB
IRDA_IRUTY   = $(IRDA_LIB_DIR)\IRUTY
IRDA_IRSIR   = $(IRDA_LIB_DIR)\IRSIR
IRDA_PROTO   = $(IRDA_LIB_DIR)\PROTOCOL
IRDA_DIR     = SOURCE\IRDA
IRDA_INC   = -I $(IRDA_DIR) \
             -I $(IRDA_LIB_DIR)\include \
             -I $(IRDA_LIB_DIR)\dcinc \
             -I $(IRDA_LIB_DIR)\DeepCore\prot
!ENDIF                                          # Enable IRDA profiling support

#-------------------------------------------------------------------------------
# Include file search path options
#-------------------------------------------------------------------------------

EQUS_INC_DIRS = -I$(EQUS_INC_DIR) \
                -I$(EQUS_CTRL_INC_DIR) \
                -I$(EQUS_FONT_DIR) \
                -I$(EQUS_PLT_REX_DIR) \
                -I$(EQUS_RES_STR_DIR) \
                -I$(EQUS_RES_IMG_DIR) \
                -I$(EQUS_RES_IMG_BMP_DIR) \
                -I$(EQUS_UTIL_MEM_DIR)

SKY_INC_DIRS  = -I$(SKY_APP_INC_DIR) \
                -I$(SKY_DEVICE_INC_DIR) \
                -I$(SKY_INC_DIR) \
                -I$(SKY_PLT_REX_DIR) \
                -I$(SKY_RES_IMG_ANI_DIR) \
                -I$(SKY_RES_IMG_BMP_DIR) \
                -I$(SKY_RES_IMG_ICN_DIR) \
                -I$(SKY_RES_SND_DIR) \
                -I$(SKY_RES_STR_DIR) \
                -I$(SKY_RES_MENU_DIR)

SKTT_INC_DIR =	-ISource\DS \
				-ISource\Audio \
				-ISource\LCD \
				-ISource\Flash \
				-ISource\SockApi \
				-ISource\ModelDep

#               -ISource\smb_b1 \

!IFDEF WAP
APP_INC = -IInclude -I$(QUALCOMMDIR) -I"$(ARMINC)" $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR) 
INC = -IInclude -I$(QUALCOMMDIR) -I"$(ARMINC)" $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR) $(IRDA_INC) $(WAP_INC)
!ELSE
!IFDEF WAP2
APP_INC = -IInclude -I$(QUALCOMMDIR) -I"$(ARMINC)" $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR) 
INC = -IInclude -I$(QUALCOMMDIR) -I"$(ARMINC)" $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR) $(IRDA_INC) $(WAP_INC)
!ELSE
APP_INC = -IInclude -I$(QUALCOMMDIR) -I"$(ARMINC)" $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR)
INC = -IInclude -I$(QUALCOMMDIR) -I"$(ARMINC)" $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR) $(IRDA_INC)
!ENDIF
!ENDIF

# NOTE:
# The -I. option allows memory.h to be found in the current directory when
# included using #include <memory.h>



#===============================================================================
#                             TOOL DEFINITIONS  III
#===============================================================================
# 	  				 Processor / APCS / Compiler	Options
#===============================================================================
#-------------------------------------------------------------------------------
# Processor architecture options
#-------------------------------------------------------------------------------

CPU = -cpu ARM7TDMI             # ARM7TDMI target processor

#-------------------------------------------------------------------------------
# ARM Procedure Call Standard (APCS) options
#-------------------------------------------------------------------------------

PCSZ     = 32bit                # Program counter (PC) size
STACKCHK = noswst               # No software stack checking support
FRAMEPTR = nofp                 # No frame pointer
FLOATPNT = softfp               # Use software floating point library
INTRWORK = interwork            # ARM/Thumb interworking support
NARROW   = narrow               # Use caller parameter narrowing
REENTRNT = nonreentrant         # No support for re-entrancy

APCS = -apcs /$(STACKCHK)/$(INTRWORK)

# NOTE:
# If software stack checking support is enabled, FEATURE_STACK_CHECK must be
# #defined in the cust*.h file to enable the required common services support.
#
# The following APCS options are obsolete in the ARM SDT 2.50 tools:
#   PCSZ
#   FRAMEPTR
#   REENTRNT

#-------------------------------------------------------------------------------
# Compiler output options
#-------------------------------------------------------------------------------

OUT = -c                        # Object file output only

#-------------------------------------------------------------------------------
# Compiler/assembler debug options
#-------------------------------------------------------------------------------

DBG = -g -dwarf2                # Enable DWARF2 format debug tables

#-------------------------------------------------------------------------------
# Compiler optimization options
#-------------------------------------------------------------------------------

OPT = -Ospace -O2               # Full compiler optimization for space

#------------------------------------------------------LFLAGS-------------------------
# Compiler code generation options
#-------------------------------------------------------------------------------

END = -littleend                # Compile for little endian memory architecture
ZA  = -za1                      # LDR may only access 32-bit aligned addresses
ZAS = -zas4                     # Min byte alignment for structures
ZAP = -zap0                     # Struct ptrs NOT assumed to be aligned per -zas
ZAT = -zat1                     # Min byte alignment for top-level static objects
ZZT = -zzt0                     # Force uninitialized variables to ZI data area

CODE = $(END)

#/* rosa 01-02-02 --> @why, what */
!IFDEF GVM
GVM_CODE = -littleend#
!ENDIF
#/* <-- */

!IFDEF SKVM
SKVM_CODE = -littleend#
SKVM_DBG = -g-
SKVM_OPT = -O1
!ENDIF

# NOTE:
# The -zap0 option is required due to a known ARM 2.50 compiler bug with
# pointers to struct sub-fields within __packed structs

#-------------------------------------------------------------------------------
# Include file search path options
#-------------------------------------------------------------------------------
#INC = -I. -I"$(ARMINC)"         # Search local dir, then ARM SDT \include dir
# NOTE:
# The -I. option allows memory.h to be found in the current directory when
# included using #include <memory.h>
###################### TOOL DEFINITIONS	II

#-------------------------------------------------------------------------------
# Compiler pragma emulation options
#-------------------------------------------------------------------------------

#PRAGMA = -zpcontinue_after_hash_error

# NOTE:
# The -zpcontinue_after_hash_error allows the compiler to continue when a
# #error statement is encountered.  This option also allows the compiler to
# continue when any other Error condition is encountered at compile time.
#        ** This option should be removed as soon as possible **

#-------------------------------------------------------------------------------
# Compiler error and warning message control options
#-------------------------------------------------------------------------------

MSG = -Wb                       # No warnings for bit fields

#-------------------------------------------------------------------------------
# Additional compile time error checking options
#-------------------------------------------------------------------------------

CHK = -fa                       # Check for data flow anomolies

#-------------------------------------------------------------------------------
# Linker options
#-------------------------------------------------------------------------------

MAP  = -map                             # Memory map of all object file areas
INFO = -info sizes,totals,veneers       # Info on object size and veneers
XREF = -xref                            # List cross-references between files
VERB = -verbose                         # Print verbose output messages
LIST = -list $(TARGET).map              # Direct map and info output to file
SYM  = -symbols -symdefs $(TARGET).sym  # Direct symbol table to file
LINK = -scatter $(SCLFILE)              # Use scatter load description file

#-------------------------------------------------------------------------------
# Linker library options
#-------------------------------------------------------------------------------

CLIB  = "$(ARMLIB)"\armlib\c_t__un.l    # ARM embedded ANSI C library
SISLIB = $(WAP_DIR)\sk_sisdec_4g_ads.a	#\sislib_arm.alf
LIBS = $(CLIB) $(SISLIB)

#-------------------------------------------------------------------------------
# Compiler flag definitions
#-------------------------------------------------------------------------------

CFLAGS0 = $(OUT) $(INC) $(CPU) $(APCS) $(CODE) $(PRAGMA) $(CHK) $(DBG) $(MSG)
CFLAGS  = $(CFLAGS0) $(OPT)

#/* rosa 01-02-02 --> @why, what */
!IFDEF GVM
GVM_CFLAGS0 = $(OUT) $(INC) $(CPU) $(APCS) $(GVM_CODE) $(PRAGMA) $(CHK) $(DBG) $(MSG) 
GVM_CFLAGS  = $(GVM_CFLAGS0) $(OPT)
!ENDIF
#/* <-- */

!IFDEF SKVM
SKVM_CFLAGS0 = $(OUT) $(INC) $(CPU) $(APCS) $(SKVM_CODE) $(PRAGMA) $(CHK) $(DBG) $(MSG) 
SKVM_CFLAGS  = $(SKVM_CFLAGS0) $(SKVM_OPT)
!ENDIF

#-------------------------------------------------------------------------------
# Assembler flag definitions
#-------------------------------------------------------------------------------

AFLAGS = $(CPU) $(APCS) $(DBG)

#-------------------------------------------------------------------------------
# Linker flag definitions
#-------------------------------------------------------------------------------

LFLAGS = $(MAP) $(INFO) $(LIST) $(SYM) $(LINK) -noremove

#-------------------------------------------------------------------------------
# Hex file flag definitions
#-------------------------------------------------------------------------------

HEXCMD = $(HEXTOOL) -i32 -o .\DownLoad\$(TARGET).hex .\DownLoad\$(TARGET).$(EXETYPE)
#HEXCMD = $(HEXTOOL) -nozeropad .\DownLoad\$(TARGET).$(EXETYPE) -i32 .\DownLoad\$(TARGET).hex

#-------------------------------------------------------------------------------
# Tools target definitions (used to echo tool information)
#-------------------------------------------------------------------------------

ARM_VERSION_BANNER = ARM ADS 1.00 TOOLS
VER_FLAG = -vsn


#===============================================================================
#                      TARGET OBJECT FILE LIST
#===============================================================================
# The following is an alphabetically sorted list of all objects required to
# build the target image file and hex file.  The following objects are built 
# according to the module specific rules and default suffix rules defined in
# this makefile.
#===============================================================================

ADC_OBJS =      $(TARGETDIR)\adcg.o

ATMR_OBJS =     $(TARGETDIR)\atmr.o

AVS_SND_OBJS =  $(TARGETDIR)\snd.o \
				$(TARGETDIR)\snditab.o \
                $(TARGETDIR)\sndhwg2.o

VOC_OBJS =		$(TARGETDIR)\vocdown.o \
				$(TARGETDIR)\vocimags.o \
				$(TARGETDIR)\vocm2.o \
				$(TARGETDIR)\vocmux.o \
				$(TARGETDIR)\vocsup.o#

AVS_OBJS =      $(TARGETDIR)\ring.o \
                $(AVS_SND_OBJS) \
                $(VOC_OBJS)

BBSND_OBJS =	$(TARGETDIR)\bbsndtab.o#

SND_OBJS =      $(BBSND_OBJS) \
                $(AVS_SND_OBJS)#

BIO_OBJS =      $(TARGETDIR)\bio.o

BOOT_C_OBJS =   $(TARGETDIR)\boot_trap.o \
                $(TARGETDIR)\bootdata.o \
                $(TARGETDIR)\boothw.o \
                $(TARGETDIR)\bootmem.o

BOOT_A_OBJS =	$(TARGETDIR)\bootapp.o \
				$(TARGETDIR)\bootsys.o

BOOT_OBJS =     $(BOOT_A_OBJS)
#    $(BOOT_C_OBJS) \


CLKMSG_OBJS =   $(TARGETDIR)\clkjul.o \
                $(TARGETDIR)\clkm2p.o \
				$(TARGETDIR)\clkregim.o#

CLK_OBJS =      $(CLKMSG_OBJS) \
                $(TARGETDIR)\clk.o \
                $(TARGETDIR)\ts_.o

CM_OBJS =       $(TARGETDIR)\cm.o \
                $(TARGETDIR)\cmcall.o \
                $(TARGETDIR)\cmclient.o \
                $(TARGETDIR)\cminband.o \
                $(TARGETDIR)\cmnv.o \
                $(TARGETDIR)\cmph.o \
                $(TARGETDIR)\cmsms.o \
                $(TARGETDIR)\cmss.o

CMD_OBJS =      $(TARGETDIR)\cmd_.o

COMMON_OBJS =	$(TARGETDIR)\lifetstg.o \
                $(TARGETDIR)\bit_.o \
                $(TARGETDIR)\crc.o \
                $(TARGETDIR)\misc_.o \
                $(TARGETDIR)\queue_.o \
                $(TARGETDIR)\qw_.o \
                $(TARGETDIR)\ran.o \
                $(TARGETDIR)\task.o
#   $(CMD_OBJS) \

MC_OBJS =       $(TARGETDIR)\caix.o \
                $(TARGETDIR)\mc.o \
                $(TARGETDIR)\mccap.o \
                $(TARGETDIR)\mccccl.o \
                $(TARGETDIR)\mccdma.o \
                $(TARGETDIR)\mccidl.o \
                $(TARGETDIR)\mccini.o \
                $(TARGETDIR)\mccqpch.o \
                $(TARGETDIR)\mccreg.o \
                $(TARGETDIR)\mccrx.o \
                $(TARGETDIR)\mccrxtx.o \
                $(TARGETDIR)\mccsa.o \
                $(TARGETDIR)\mccso.o \
                $(TARGETDIR)\mccsch.o \
                $(TARGETDIR)\mccscm.o \
                $(TARGETDIR)\mccsrch.o \
                $(TARGETDIR)\mccsup.o \
                $(TARGETDIR)\mccsyobj.o \
                $(TARGETDIR)\mcctc.o \
                $(TARGETDIR)\mcctcho.o \
                $(TARGETDIR)\mcctcsup.o \
                $(TARGETDIR)\mcctxns.o \
                $(TARGETDIR)\mcnvdf.o \
                $(TARGETDIR)\mcscript.o \
                $(TARGETDIR)\mcsysci.o \
                $(TARGETDIR)\mcsyspr.o \
                $(TARGETDIR)\mcsyspra.o \
                $(TARGETDIR)\mcsysprd.o \
                $(TARGETDIR)\mcsyssup.o \
                $(TARGETDIR)\mctrans_.o \
                $(TARGETDIR)\otasp.o \
                $(TARGETDIR)\otaspx.o \
                $(TARGETDIR)\parm_.o \
                $(TARGETDIR)\snm.o \
                $(TARGETDIR)\tmsi.o \
                $(TARGETDIR)\tdso.o

CP_RXTX_OBJS =  $(TARGETDIR)\rx.o \
                $(TARGETDIR)\rxtx.o \
                $(TARGETDIR)\tx.o

DCP_OBJS =      $(MC_OBJS) \
                $(CP_RXTX_OBJS) \
                $(TARGETDIR)\bch.o \
                $(TARGETDIR)\db.o \
                $(TARGETDIR)\distreg.o \
                $(TARGETDIR)\loopback.o \
                $(TARGETDIR)\mar_.o \
                $(TARGETDIR)\martable.o \
                $(TARGETDIR)\prl.o \
                $(TARGETDIR)\srv.o #

AUTH_OBJS =#
#     $(TARGETDIR)\apwrx.o \
#     $(TARGETDIR)\auth.o \
#     $(TARGETDIR)\cave.o \
#     $(TARGETDIR)\dh.o

DS_OBJS =       $(TARGETDIR)\ds_snoop_.o \
                $(TARGETDIR)\dsatcop.o \
                $(TARGETDIR)\dsatdat.o \
                $(TARGETDIR)\dsatps.o \
                $(TARGETDIR)\dsatsio.o \
                $(TARGETDIR)\dsctl.o \
                $(TARGETDIR)\dsm_.o \
                $(TARGETDIR)\dsmgr_.o \
                $(TARGETDIR)\dsnetmdl.o \
                $(TARGETDIR)\dsrlp_.o \
                $(TARGETDIR)\dsrlpdel_.o \
                $(TARGETDIR)\dsrlpi_.o \
                $(TARGETDIR)\dsrlp3_.o \
                $(TARGETDIR)\dsrlpnak_.o \
                $(TARGETDIR)\dsrlprsq_.o \
                $(TARGETDIR)\dsrlptxq_.o \
                $(TARGETDIR)\dsrlprtx.o \
                $(TARGETDIR)\dsscrm.o \
                $(TARGETDIR)\dssdns.o \
                $(TARGETDIR)\dssdorm.o \
                $(TARGETDIR)\dssnet.o \
                $(TARGETDIR)\dssocfg.o \
                $(TARGETDIR)\dssocket.o \
                $(TARGETDIR)\dssocki.o \
                $(TARGETDIR)\dssoctl.o \
                $(TARGETDIR)\dsstcp.o \
                $(TARGETDIR)\dssudp.o \
                $(TARGETDIR)\ip.o \
                $(TARGETDIR)\iphdr.o \
				$(TARGETDIR)\mdrrlp.o \
                $(TARGETDIR)\ppp_.o \
                $(TARGETDIR)\pppfsm.o \
                $(TARGETDIR)\pppipcp.o \
                $(TARGETDIR)\ppplcp.o \
                $(TARGETDIR)\psctl.o \
                $(TARGETDIR)\psmgr.o \
                $(TARGETDIR)\psmisc.o \
                $(TARGETDIR)\pstimer.o \
                $(TARGETDIR)\rlp.o \
                $(TARGETDIR)\slhc.o \
                $(TARGETDIR)\tcphdr.o \
                $(TARGETDIR)\tcpin.o \
                $(TARGETDIR)\tcpout.o \
                $(TARGETDIR)\tcpshell.o \
                $(TARGETDIR)\tcpsubr.o \
                $(TARGETDIR)\tcptimer.o \
                $(TARGETDIR)\udp.o

DATA_OBJS =     $(DS_OBJS) \
                $(TARGETDIR)\icmp.o

DEC_OBJS =      $(TARGETDIR)\dec5000_.o \
                $(TARGETDIR)\deint.o

DEMOD_OBJS =    $(TARGETDIR)\dmdimage.o \
                $(TARGETDIR)\dmddown.o

DFM_OBJS =      $(TARGETDIR)\acpwbm2.o

DIAG_C_OBJS =   $(TARGETDIR)\diag.o \
                $(TARGETDIR)\diagbuf_.o \
                $(TARGETDIR)\diagfs.o \
                $(TARGETDIR)\diagp.o \
                $(TARGETDIR)\diagx.o \
                $(TARGETDIR)\diag_guid.o \
                $(TARGETDIR)\diag_table.o \
                $(TARGETDIR)\event.o \
                $(TARGETDIR)\feature.o \
                $(TARGETDIR)\msg_.o

DIAGMSG_OBJS =  $(TARGETDIR)\err.o \
                $(TARGETDIR)\log_.o

DIAG_OBJS =     $(DIAG_C_OBJS) \
                $(DIAGMSG_OBJS)

DLOAD_C_OBJS =  $(TARGETDIR)\dloadarm.o \
                $(TARGETDIR)\dloaduart.o

DLOAD_A_OBJS =  $(TARGETDIR)\dloadpoll.o

NORAM_OBJS =    $(BOOT_C_OBJS) \
                $(DLOAD_C_OBJS)

DLOAD_OBJS =    $(DLOAD_A_OBJS) \
#    $(DLOAD_C_OBJS) \

DOG_OBJS =      $(TARGETDIR)\dog.o

ENC_OBJS =      $(TARGETDIR)\enc_.o

FS_OBJS =       $(TARGETDIR)\fs.o \
                $(TARGETDIR)\fs_alloc.o \
                $(TARGETDIR)\fs_am.o \
                $(TARGETDIR)\fs_dev.o \
                $(TARGETDIR)\fs_dir.o \
                $(TARGETDIR)\fs_mpoint.o \
                $(TARGETDIR)\fs_ops.o \
                $(TARGETDIR)\fs_udir.o

FS_DEV_OBJS =	$(TARGETDIR)\fs_dev_s.o#

HW_OBJS =       $(TARGETDIR)\hw.o \
                $(TARGETDIR)\msm5_init.o \
                $(TARGETDIR)\memtest.o \
				$(TARGETDIR)\reloc.o#

LCD_OBJS =#$(TARGETDIR)\lcd_v2428.o

FTM_OBJS =#
#      $(TARGETDIR)\test_mc.o \
#      $(TARGETDIR)\ftm_cmd.o

MOBILE_OBJS =   $(TARGETDIR)\mobile.o

MUX_OBJS =		$(TARGETDIR)\ffpc_.o \
                $(TARGETDIR)\rlgc_.o \
				$(TARGETDIR)\rxccommon.o \
                $(TARGETDIR)\rxcdemux_.o \
                $(TARGETDIR)\rxcpaging.o \
                $(TARGETDIR)\rxcrda_.o \
                $(TARGETDIR)\rxcsync.o \
                $(TARGETDIR)\rxctraffic_.o \
                $(TARGETDIR)\txcaccess.o \
                $(TARGETDIR)\txccommon_.o \
                $(TARGETDIR)\txcmux.o \
                $(TARGETDIR)\txcso.o \
                $(TARGETDIR)\txctraffic_.o#

RXTX_OBJS =     $(CP_RXTX_OBJS)
#                $(MUX_OBJS)

NVDR_OBJS =#
#     $(TARGETDIR)\nv.o \
#     $(TARGETDIR)\nv24lcxx.o \
#                $(TARGETDIR)\nva.o \
#                $(TARGETDIR)\nvb.o \
#                $(TARGETDIR)\nvmio.o \
#                $(TARGETDIR)\nvr.o \
#                $(TARGETDIR)\nvw.o

NVIM_OBJS =     $(TARGETDIR)\nvim.o \
                $(TARGETDIR)\nvimnv.o \
                $(TARGETDIR)\nvimio.o \
                $(TARGETDIR)\nvimr.o \
                $(TARGETDIR)\nvimw.o \
				$(TARGETDIR)\nvsetup.o #
#youandi nvsetup은 어디서 나온거지???

TRAMP_OBJS =	$(TARGETDIR)\trampm5.o#

REX_OBJS =      $(TARGETDIR)\rex_.o \
                $(TARGETDIR)\rexarm.o

RF_OBJS =       $(TARGETDIR)\rfcs.o \
                $(TARGETDIR)\rfmsm.o \
                $(TARGETDIR)\rfnv.o#

SBI_OBJS =      $(TARGETDIR)\sbi.o

SIO_OBJS =      $(TARGETDIR)\siors232_.o \
                $(TARGETDIR)\sio_.o \
				$(TARGETDIR)\siog2.o \
                $(TARGETDIR)\sdevmap.o#

SRCH_OBJS =     $(TARGETDIR)\dmod.o \
                $(TARGETDIR)\pnmask.o \
                $(TARGETDIR)\srch.o \
                $(TARGETDIR)\sleep.o \
                $(TARGETDIR)\srchaq.o \
                $(TARGETDIR)\srchcd.o \
                $(TARGETDIR)\srchdrv_.o \
                $(TARGETDIR)\srchdz.o \
                $(TARGETDIR)\srchint.o \
                $(TARGETDIR)\srchpc.o \
                $(TARGETDIR)\srchqpch.o \
                $(TARGETDIR)\srchsc.o \
                $(TARGETDIR)\srchsl.o \
                $(TARGETDIR)\srchst.o \
                $(TARGETDIR)\srchtc.o \
                $(TARGETDIR)\srchtri.o \
                $(TARGETDIR)\srchun.o \
                $(TARGETDIR)\srchzz.o \
                $(TARGETDIR)\ulpn.o

SEARCH_OBJS =   $(DMD_OBJS) \
                $(SRCH_OBJS)

THERM_OBJS =    $(TARGETDIR)\therm.o

UASMS_OBJS =    $(TARGETDIR)\uasms.o \
                $(TARGETDIR)\uasmsnv.o \
                $(TARGETDIR)\uasmsu.o \
                $(TARGETDIR)\uasmsx.o

CMGR_OBJS =     $(CM_OBJS) \
                $(UASMS_OBJS)
#                $(CMD_OBJS) \

UI_OBJS =       $(TARGETDIR)\uixuasms.o

USB_OBJS =#
#      $(TARGETDIR)\siousb.o \
#                $(TARGETDIR)\usbdc.o \
#                $(TARGETDIR)\usbcdc.o \
#                $(TARGETDIR)\usbsdms.o

VBATT_OBJS =    $(TARGETDIR)\vbatt.o#

OBJECTS_MSM5000_IRAM_REV = $(TARGETDIR)\cmd_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\dsrlp_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\dsrlpi_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\dsrlptxq_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\enc_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\mar_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\mctrans_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\rlgc_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\siors232_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\srchdrv_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\ts_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\txccommon_MSM5000_IRAM_REV_.o \
     $(TARGETDIR)\txctraffic_MSM5000_IRAM_REV_.o

OBJECTS_MSM5000_IRAM_FWD = $(TARGETDIR)\bit_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\diagbuf_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\ds_snoop_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dsm_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dsmgr_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dsrlp_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dsrlp3_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dsrlpdel_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dsrlpi_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dsrlpnak_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\dsrlprsq_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\ffpc_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\log_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\misc_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\msg_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\parm_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\ppp_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\queue_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\qw_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\rex_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\rxcdemux_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\rxcrda_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\rxctraffic_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\sio_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\siors232_MSM5000_IRAM_FWD_.o \
     $(TARGETDIR)\txctraffic_MSM5000_IRAM_FWD_.o

########################################
FILESYS_OBJS = $(TARGETDIR)\filesysB1.o#
SMEMORY_OBJS = $(TARGETDIR)\Shared.o
SKTT_RF_OBJS = $(TARGETDIR)\Newcal.o

AUDIO_OBJS = $(TARGETDIR)\OKI_M7732.o \
			 $(TARGETDIR)\YMU759.o   \
			 $(TARGETDIR)\MusicDB_B1.o
########################################

OBJECTS =		$(NORAM_OBJS) \
				$(ADC_OBJS) \
                $(ATMR_OBJS) \
                $(AVS_OBJS) \
                $(BBSND_OBJS) \
                $(BIO_OBJS) \
                $(BOOT_OBJS) \
                $(CLK_OBJS) \
                $(CM_OBJS) \
				$(CMD_OBJS) \
                $(COMMON_OBJS) \
                $(DCP_OBJS) \
                $(DATA_OBJS) \
                $(DEC_OBJS) \
                $(DEMOD_OBJS) \
                $(DFM_OBJS) \
                $(DIAG_OBJS) \
                $(DLOAD_OBJS) \
                $(DOG_OBJS) \
                $(ENC_OBJS) \
                $(FS_OBJS) \
				$(HW_OBJS) \
				$(LCD_OBJS) \
                $(MOBILE_OBJS) \
                $(MUX_OBJS) \
                $(NVIM_OBJS) \
                $(TRAMP_OBJS) \
                $(REX_OBJS) \
                $(RF_OBJS) \
                $(SBI_OBJS) \
                $(SIO_OBJS) \
                $(SRCH_OBJS) \
                $(THERM_OBJS) \
                $(UASMS_OBJS) \
                $(UI_OBJS) \
                $(VBATT_OBJS) \
			    $(OBJECTS_MSM5000_IRAM_FWD) \
				$(OBJECTS_MSM5000_IRAM_REV) \
!IFDEF WAP
				 $(WAP_OBJECTS_ALL) \
!ENDIF
!IFDEF WAP2
				 $(WAP_OBJECTS_ALL) \
!ENDIF
!IFDEF IRDA									# Enable IRDA profiling support
				 $(IRDA_OBJS) \
!ENDIF										# Enable IRDA profiling support
                 $(MELODY_OBJS) \
                 $(AUDIO_OBJS) \
				 $(SKY_DEVICE_IDLCD_OBJS) \
!IFDEF SMB
                 $(SMB_OBJS) \
!ENDIF
                 $(SKY_APP_CAMERA_OBJS) \
!IFDEF FEATURE_WAVELET  #yoursun 01/06/08
				$(SKY_APP_WAVELET_OBJS) \
				$(SKY_APP_WAVDECODE_OBJS) \
!ENDIF #FEATURE_WAVELET
!if "$(MODEL_ID)" == "MODEL_B0"
				 $(SMB_OBJS) \
!endif
                 $(FS_DEV_OBJS) \
                 $(FILESYS_OBJS) \
                 $(SMEMORY_OBJS) \
                 $(SKTT_RF_OBJS) \
!IFDEF  SOCKAPI   #jrkwon       
				 $(SOCKAPI_OBJS) \
!ENDIF
!IFDEF GVM
				 $(GVM_OBJS) \
				 $(GVM_LIB_OBJS) \
				 $(GVM_ATSWAP_OBJS) \
!ENDIF
!IFDEF SKVM
				 $(SKY_APP_SKVM_OBJS) \
				 $(SKY_APP_SKVM_LIB_OBJS) \
!ENDIF
!IFDEF MCOMMERCE
				 $(MCOMMERCE_OBJS) \
!ENDIF
				 $(DSSKT_OBJS) \
				 $(EQUS_UTIL_MEM_OBJS) \
                 $(EQUS_STATE_OBJS) \
                 $(EQUS_UTIL_OBJS) \
                 $(EQUS_DISPLAY_OBJS) \
                 $(EQUS_CTRL_OBJS) \
                 $(EQUS_I18N_OBJS) \
                 $(EQUS_RES_IMG_OBJS) \
                 $(SKY_DEVICE_LCD_OBJS) \
                 $(SKY_I18N_OBJS) \
                 $(SKY_PLT_REX_OBJS) \
                 $(SKY_RES_IMG_OBJS) \
                 $(SKY_RES_MENU_OBJS) \
                 $(SKY_SYS_OBJS) \
#                 $(SKY_TEMP_OBJS) \
                 $(SKY_APP_SYS_OBJS) \
				 $(SKY_APP_EIF_OBJS) \
                 $(SKY_APP_PBOOK_OBJS) \
                 $(SKY_APP_BELL_VIB_OBJS) \
                 $(SKY_APP_PIC_OBJS) \
				 $(SKY_APP_CALC_OBJS) \
				 $(SKY_APP_SWATCH_OBJS)\
				 $(SKY_APP_WTIME_OBJS)\
                 $(SKY_APP_SCHEDULE_OBJS) \
                 $(SKY_APP_ALARM_OBJS) \
                 $(SKY_APP_MEMO_OBJS) \
!IFDEF MCOMMERCE
				 $(SKY_APP_MWALLET_OBJS) \
!ENDIF
!IFDEF PHOTOMAIL
                 $(SKY_APP_PM_OBJS) \
!ENDIF
!IFDEF VAS
                 $(SKY_APP_VAS_OBJS) \
!ENDIF
                 $(SKY_APP_SETTING_OBJS) \
				 $(SKY_APP_DATACFG_OBJS) \
                 $(SKY_APP_DATAMGR_OBJS) \
				 $(SKY_APP_CALLLIST_OBJS) \
                 $(SKY_APP_MENU_OBJS) \
                 $(SKY_APP_RECPLAY_OBJS) \
                 $(SKY_APP_SMS_OBJS) \
                 $(SKY_APP_VOCODER_OBJS) \
                 $(SKY_APP_AUTOANS_OBJS) \
!IFDEF IRDA
                 $(SKY_APP_IRDA_OBJS) \
!ENDIF                
                 $(SKY_APP_GAME_LIB_OBJS) \
                 $(SKY_APP_GAME_OBJS) \
                 $(SKY_APP_GAMEAPP_OBJS) \
                 $(SKY_APP_EMAIL_OBJS) \
                 $(SKY_APP_TEST_OBJS) \
				 $(SKY_APP_WAP_OBJS)#
#                 $(SKY_APP_MEMO_OBJS) \
#                 $(SKY_APP_CTRL_OBJS) \
#                 $(SKY_APP_SMS_OBJS) \


#===============================================================================
#                               TARGET RULES
#===============================================================================

# The following are the target rules supported by this makefile with the
# exception of the 'depend' target which is located at the end of this file.

#-------------------------------------------------------------------------------
# Default target
#-------------------------------------------------------------------------------

# The default target lists the tool versions, creates an object subdirectory
# if required, and builds the target image file and hex file.  The mobile
# object includes the compile date and time and is deleted after each build
# to ensure that it will always be rebuilt to include accurate build date and
# time information.

all : tools $(TARGETDIR)\exist exe hex
         @-del $(TARGETDIR)\mobile.o
         @-del $(TARGETDIR)\diag_guid.o
         @-del $(TARGETDIR)\diag_table.o

#-------------------------------------------------------------------------------
# Object directory target
#-------------------------------------------------------------------------------

# The object subdirectoy is created if it does not already exist.

$(TARGETDIR)\exist :
         @-if not exist $(TARGETDIR)\exist mkdir $(TARGETDIR)
         @echo Building $(TARGETDIR) > $(TARGETDIR)\exist

#-------------------------------------------------------------------------------
# Image file target
#-------------------------------------------------------------------------------

# The target image file is produced by the linker in the selected image format,
# and then a hex file is created from the target image file.

# Image file
exe : $(TARGET).$(EXETYPE)

#$(TARGET).$(EXETYPE) : $(TARGETDIR)\exist sources $(OBJECTS)
#        @echo ---------------------------------------------------------------
#        @echo TARGET $@
#        $(LD) -$(EXETYPE) $(LFLAGS) $(OBJ_CMD) $@ $(LIBS) -VIA <<
#        $(OBJECTS)
#<<

# generate the BIN files
$(BIN_FILE) : $(TARGET).hex


# Hex file
hex : $(TARGET).hex
$(TARGET).hex : $(ELF_FILE)
	@echo
    @echo TARGET $(TARGET).hex
    $(HEXCMD)
	@echo ------------------------------------------------------------------
	@echo Generate the BIN files
	$(BINTOOL) $(HEX_FILE) $(BIN_FILE) I 0000 5FFFF 2
	@echo ------------------------------------------------------------------
	@echo Make bin complete
	@echo ------------------------------------------------------------------
	$(MAKE_BIN) $(BIN_FILE) $(BOOT_BIN) 600000
	@echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Clean target
#-------------------------------------------------------------------------------

# The object subdirectory, target image file, and target hex file are deleted.

clean :
        @echo ---------------------------------------------------------------
        @echo CLEAN
        -if exist $(TARGETDIR) rmdir /s/q $(TARGETDIR)
        -if exist $(TARGET).$(EXETYPE) del /f $(TARGET).$(EXETYPE)
        -if exist $(TARGET).hex del /f $(TARGET).hex
        -if exist *._C del *_.c
        @echo ---------------------------------------------------------------

#-------------------------------------------------------------------------------
# Test target
#-------------------------------------------------------------------------------

# The flags and symbol definitions for the compiler, assembler and linker are
# listed for makefile test purposes.

test :
        @echo ------------------------------------------------------------------
        @echo AFLAGS : $(AFLAGS)
        @echo ------------------------------------------------------------------
        @echo DMSS_AFLAGS : $(DMSS_AFLAGS)
        @echo ------------------------------------------------------------------
        @echo CFLAGS : $(CFLAGS)
        @echo ------------------------------------------------------------------
        @echo DMSS_CFLAGS : $(DMSS_CFLAGS)
        @echo ------------------------------------------------------------------
        @echo LFLAGS : $(LFLAGS)
        @echo ------------------------------------------------------------------
        @echo LIBS : $(LIBS)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Tools target
#-------------------------------------------------------------------------------

# The tool versions are listed.
tools :
	@echo ------------------------------------------------------------------
	@echo ARM TOOL VERSIONS
	@$(ARMCC) -vsn
	@$(CC)    -vsn
	@$(ASM)   -vsn
	@echo ------------------------------------------------------------------



#===============================================================================
#                           DEFAULT SUFFIX RULES
#===============================================================================

# The following are the default suffix rules used to compile all objects that
# are not specifically included in one of the module specific rules defined
# in the next section.

# The following macros are used to specify the output object file, MSG_FILE
# symbol definition and input source file on the compile line in the rules
# defined below.

SRC_FILE = $(SRC_DIR)\$(@F:.o=.c)                     # Input source file specification
OBJ_FILE = $(OBJ_CMD) $(TARGETDIR)\$(@F)   # Output object file specification
MSG_FILE = -DMSG_FILE=\"$(@F:.o=.c)\"      # MSG_FILE symbol definition

SRC_DIR = $(QUALCOMMDIR)
#-------------------------------------------------------------------------------
# C code inference rules
#-------------------------------------------------------------------------------

{$(QUALCOMMDIR)}.c{$(TARGETDIR)}.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $<
        @echo ------------------------------------------------------------------

{$(TARGETDIR)}.c{$(TARGETDIR)}.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $<
        @echo ------------------------------------------------------------------

.c{$(TARGETDIR)}.o: 
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Assembly code inference rules
#-------------------------------------------------------------------------------

.s.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) -E $(DMSS_AFLAGS) < $< | perl $(ASM_SCRIPT) - > $(TARGETDIR)/$*.i
        $(ASM) $(AFLAGS) -list $(TARGETDIR)\$*.lst $*.i $(OBJ_CMD) $(TARGETDIR)\$@
        @echo ------------------------------------------------------------------

.s{$(TARGETDIR)}.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) -E $(DMSS_AFLAGS) < $< | perl $(ASM_SCRIPT) - > $*.i
        $(ASM) $(AFLAGS) -list $*.lst $*.i $(OBJ_CMD) $@
        @echo ------------------------------------------------------------------

{$(QUALCOMMDIR)}.s{$(TARGETDIR)}.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) -E $(DMSS_AFLAGS) $(INC)< $< | perl $(ASM_SCRIPT) - > $*.i
        $(ASM) $(AFLAGS) -list $*.lst $*.i $(OBJ_CMD) $@
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# Mixed source/assembly inference rule
#-------------------------------------------------------------------------------

.c.mix:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) -S -fs $(CFLAGS) $(DMSS_CFLAGS) -DMSG_FILE=\"$<\" $(OBJ_CMD) $@ $<
        @echo ------------------------------------------------------------------

#===============================================================================
#                           MODULE SPECIFIC RULES
#===============================================================================

# The following module specific rules allow sets of objects to be compiled
# with compiler flags other than the defaults used in the suffix rules defined
# above.  The following rules use the module specific compiler flags defined
# at the beginning of this file.  New objects can be added to any of the
# following rules by modifying the corresponding list of objects.

#-------------------------------------------------------------------------------
# Boot code
#-------------------------------------------------------------------------------

#BOOT_OBJS2 = $(TARGETDIR)\boot_trap.o \
#			 $(TARGETDIR)\bootdata.o \
#			 $(TARGETDIR)\boothw.o \
#			 $(TARGETDIR)\bootmem.o \
#			 $(TARGETDIR)\dloadarm.o \
#			 $(TARGETDIR)\dloaduart.o#
#$(BOOT_OBJS2) : $(SRC_FILE)
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $(@F)
#	$(CC) $(CFLAGS0) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(QUALCOMMDIR)\$(SRC_FILE)
#	@echo ------------------------------------------------------------------


$(TARGETDIR)\mobile.o :
	@echo -----------------------------------------------------------------
	@echo OBJECT $@
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_MOB) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo -----------------------------------------------------------------


$(TARGETDIR)\prl.o :
	@echo -----------------------------------------------------------------
	@echo OBJECT $@
	$(CC) $(CFLAGS0) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo -----------------------------------------------------------------

#-------------------------------------------------------------------------------
# No RAM code
#-------------------------------------------------------------------------------
# Compile the boot code with no compiler optimization to avoid insertion of
# stack push and pop instructions in functions that are called before target
# RAM has been initialized.

$(NORAM_OBJS) : 
	@echo ------------------------------------------------------------------
	@echo OBJECT $@
	$(CC) $(CFLAGS0) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------


#$(DIAGMSG_OBJS) :
#	@echo -----------------------------------------------------------------
#	@echo OBJECT $@
#	$(CC) $(CFLAGS) $(DMSS_CFLAGS_NONE) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#	@echo -----------------------------------------------------------------


$(CLKMSG_OBJS) :
	@echo ------------------------------------------------------------------
	@echo OBJECT $@ 
	$(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------


#$(DEC_OBJS) :
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $@
#	$(CC) $(CFLAGS) $(DMSS_CFLAGS_DEC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#	@echo ------------------------------------------------------------------


#$(ENC_OBJS) :
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $@
#	$(CC) $(CFLAGS) $(DMSS_CFLAGS_ENC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#	@echo ------------------------------------------------------------------


#$(MC_OBJS) :
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $@
#	$(CC) $(CFLAGS) $(DMSS_CFLAGS_MC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE) 
#	@echo ------------------------------------------------------------------


$(RXTX_OBJS) :
	@echo ------------------------------------------------------------------
	@echo OBJECT $@
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_RXTX) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------


#$(DS_OBJS) :
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $@
#	$(CC) $(CFLAGS) $(DMSS_CFLAGS_DS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#	@echo ------------------------------------------------------------------


$(NVIM_OBJS) :
	@echo ------------------------------------------------------------------
	@echo OBJECT $@
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_NV) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------


$(FS_OBJS) :
	@echo ------------------------------------------------------------------
	@echo OBJECT $@
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_NV) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------


#$(FS_DEV_OBJS) :
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $@
#	$(ARMCC) $(CFLAGS) $(DMSS_CFLAGS_NVIM) $(OBJ_FILE) $(QUALCOMMDIR)\$(SRC_FILE)
#	@echo ------------------------------------------------------------------


#$(SIO_OBJS) :
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $@
#	$(CC) $(CFLAGS) $(DMSS_CFLAGS_SIO) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#	@echo ------------------------------------------------------------------


$(SND_OBJS) :
	@echo ------------------------------------------------------------------
	@echo OBJECT $@
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_SND) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------


#$(SEARCH_OBJS) :
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $@
#	$(CC) $(CFLAGS) $(DMSS_CFLAGS_SRCH) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#	@echo ------------------------------------------------------------------


#$(UI_OBJS) :
#	@echo ------------------------------------------------------------------
#	@echo OBJECT $@
#	$(CC) $(CFLAGS) $(DMSS_CFLAGS_UI) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#	@echo ------------------------------------------------------------------


$(CMGR_OBJS) :
	@echo ------------------------------------------------------------------
	@echo OBJECT $@ 
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_CM) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE) 
	@echo ------------------------------------------------------------------


$(VOC_OBJS) :
	@echo ------------------------------------------------------------------
	@echo OBJECT $@
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_VOC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------

$(QUALCOMMDIR)\diag_guid.c :
	@echo ------------------------------------------------------------------
	@echo Generating a GUID for the build
	@echo OBJECT $@
	@perl $(QUALCOMMDIR)\diag_guid_gen.pl
	@echo ------------------------------------------------------------------


$(QUALCOMMDIR)\diag_table.c :
	@echo ------------------------------------------------------------------
	@echo Generating DIAG tables
	@perl $(QUALCOMMDIR)\diag_table_gen.pl
	@echo OBJECT $@
	@echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# IRAM test
#-------------------------------------------------------------------------------
$(TARGETDIR)\reloc.o :
	@echo ------------------------------------------------------------------
	@echo OBJECT reloc.o
	$(ARMCC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# AUDIO
#-------------------------------------------------------------------------------
SRC_DIR = Source\Audio
$(AUDIO_OBJS) : $(SRC_FILE)
	@echo ------------------------------------------------------------------
	@echo OBJECT $(@F)
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_UI) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# FLASH
#-------------------------------------------------------------------------------
SRC_DIR = Source\Flash
$(FS_DEV_OBJS) : $(SRC_FILE)
	@echo ------------------------------------------------------------------
	@echo OBJECT $(@F)
	$(ARMCC) $(CFLAGS0) $(DMSS_CFLAGS_NONE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# FLASH                             자동응답 관련             2001. 1. 17  hjcho
#-------------------------------------------------------------------------------
SRC_DIR = Source\Flash
$(FILESYS_OBJS) : $(SRC_FILE)
	@echo ------------------------------------------------------------------
	@echo OBJECT $(@F)
	$(CC) $(CFLAGS) $(DMSS_CFLAGS_UI) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------

SRC_DIR = Source\SMEMORY
$(SMEMORY_OBJS) : $(SRC_FILE)
	@echo ------------------------------------------------------------------
	@echo OBJECT $(@F)
	$(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------

SRC_DIR = Source\RF
$(SKTT_RF_OBJS) : $(SRC_FILE)
	@echo ------------------------------------------------------------------
	@echo OBJECT $(@F)
	$(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo ------------------------------------------------------------------

!IFDEF WAP
#-------------------------------------------------------------------------------
# WAP
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# SOURCE / WAP /
#-------------------------------------------------------------------------------

SRC_DIR = Source\WAP_B2
WAP_ROOT = $(TARGETDIR)\wapstate.o \
		   $(TARGETDIR)\waptask.o \
		   $(TARGETDIR)\wapcmd.o \
		   $(TARGETDIR)\wapaddress.o 

$(WAP_ROOT) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) -I $(WAP_DIR)\SECURE $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

!IFDEF WAP_LOG
#-------------------------------------------------------------------------------
# SOURCE / WAP /
#-------------------------------------------------------------------------------

SRC_DIR = Source\WAP_B2
WAP_LOGGING = $(TARGETDIR)\waplog.o

$(WAP_LOGGING) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_LVL_MED) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
!ENDIF

#-------------------------------------------------------------------------------
# SOURCE / WAP / CORE / SOURCE 
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\core\source
WAP_SOURCE = $(TARGETDIR)\BuffMgt.o \
			 $(TARGETDIR)\cache.o \
			 $(TARGETDIR)\capiclnt.o \
			 $(TARGETDIR)\capimem.o \
			 $(TARGETDIR)\capimmi.o \
			 $(TARGETDIR)\capisms.o \
			 $(TARGETDIR)\capiudp.o \
			 $(TARGETDIR)\capiussd.o \
			 $(TARGETDIR)\capiwd.o \
			 $(TARGETDIR)\capiwta.o \
			 $(TARGETDIR)\CmmnRsrc.o \
			 $(TARGETDIR)\config.o \
			 $(TARGETDIR)\ctypes.o \
			 $(TARGETDIR)\env.o \
			 $(TARGETDIR)\GlblDecl.o \
			 $(TARGETDIR)\HdrUtil.o \
			 $(TARGETDIR)\Header.o \
			 $(TARGETDIR)\mk_cpu.o \
			 $(TARGETDIR)\mk_main.o \
			 $(TARGETDIR)\mk_outp.o \
			 $(TARGETDIR)\mk_queu.o \
			 $(TARGETDIR)\mk_sche.o \
			 $(TARGETDIR)\mk_user.o \
			 $(TARGETDIR)\ml_buf.o \
			 $(TARGETDIR)\ml_err.o \
			 $(TARGETDIR)\ml_mem.o \
			 $(TARGETDIR)\msgqueue.o \
			 $(TARGETDIR)\pdubuf.o \
			 $(TARGETDIR)\sctpred.o \
			 $(TARGETDIR)\si_bpi.o \
			 $(TARGETDIR)\si_float.o \
			 $(TARGETDIR)\si_frame.o \
			 $(TARGETDIR)\si_fs.o \
			 $(TARGETDIR)\si_instr.o \
			 $(TARGETDIR)\si_int.o \
			 $(TARGETDIR)\si_libs.o \
			 $(TARGETDIR)\si_list.o \
			 $(TARGETDIR)\si_misc.o \
			 $(TARGETDIR)\si_ops.o \
			 $(TARGETDIR)\si_re.o \
			 $(TARGETDIR)\si_sdl.o \
			 $(TARGETDIR)\si_si.o \
			 $(TARGETDIR)\si_sq.o \
			 $(TARGETDIR)\si_var.o \
			 $(TARGETDIR)\si_varop.o \
			 $(TARGETDIR)\si_wtai.o \
			 $(TARGETDIR)\SPTP_TLS.o \
			 $(TARGETDIR)\StackMgr.o \
			 $(TARGETDIR)\Storage.o \
			 $(TARGETDIR)\trnscode.o \
			 $(TARGETDIR)\UDCP.o \
			 $(TARGETDIR)\UDCPUSSD.o \
			 $(TARGETDIR)\URL.o \
			 $(TARGETDIR)\WAE.o \
			 $(TARGETDIR)\WAEctype.o \
			 $(TARGETDIR)\WAEDef.o \
			 $(TARGETDIR)\WAEPckg.o \
			 $(TARGETDIR)\WAE_Cmmn.o \
			 $(TARGETDIR)\WAE_WSP.o \
			 $(TARGETDIR)\WAP.o \
			 $(TARGETDIR)\WBXMLDec.o \
			 $(TARGETDIR)\WBXMLElm.o \
			 $(TARGETDIR)\WDP.o \
			 $(TARGETDIR)\WDP_SMS.o \
			 $(TARGETDIR)\WDP_UDCP.o \
			 $(TARGETDIR)\WDP_UDP.o \
			 $(TARGETDIR)\WDP_UL.o \
			 $(TARGETDIR)\wiptimer.o \
			 $(TARGETDIR)\WMLElm.o \
			 $(TARGETDIR)\WMLEvent.o \
			 $(TARGETDIR)\WMLIF.o \
			 $(TARGETDIR)\WMLParse.o \
			 $(TARGETDIR)\WMLToken.o \
			 $(TARGETDIR)\WML_MMI.o \
			 $(TARGETDIR)\WML_UAFn.o \
			 $(TARGETDIR)\WSP.o \
			 $(TARGETDIR)\WSPCapa.o \
			 $(TARGETDIR)\WSPCLWDP.o \
			 $(TARGETDIR)\WSPCMWTP.o \
			 $(TARGETDIR)\WSPIF.o \
			 $(TARGETDIR)\WSPproCL.o \
			 $(TARGETDIR)\WSPproCM.o \
			 $(TARGETDIR)\WTPpckg.o \
			 $(TARGETDIR)\WTPPrvdr.o \
			 $(TARGETDIR)\WTPsuppt.o \
			 $(TARGETDIR)\WTP_WDP.o

$(WAP_SOURCE) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
              
#-------------------------------------------------------------------------------
# SOURCE / WAP / CORE / OPTIONAL / CHARSET
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\core\optional\charset
WAP_OPTIONAL_CHARSET = $(TARGETDIR)\charset.o \
				       $(TARGETDIR)\HCodeCnvt.o

$(WAP_OPTIONAL_CHARSET) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / CORE / OPTIONAL / MEMORY
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\core\optional\memory
WAP_OPTIONAL_MEMORY = $(TARGETDIR)\wip_mem.o

$(WAP_OPTIONAL_MEMORY) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / CLNT
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\rex\clnt
WAP_CLNT = $(TARGETDIR)\clntadapter.o \
		   $(TARGETDIR)\clntapp.o \
		   $(TARGETDIR)\clntPIM.o \
		   $(TARGETDIR)\clntwallet.o

$(WAP_CLNT) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / MEM
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\rex\mem
WAP_MEM = $(TARGETDIR)\aapimem.o \
		  $(TARGETDIR)\memadapter.o

$(WAP_MEM) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / MMI
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\rex\mmi
WAP_MMI = $(TARGETDIR)\mmiadapter.o \
		  $(TARGETDIR)\mmicmd.o \
		  $(TARGETDIR)\GenDsp.o \
		  $(TARGETDIR)\mmi_adapter.o \
		  $(TARGETDIR)\mmi_connector.o \
		  $(TARGETDIR)\mmi_wml.o \
		  $(TARGETDIR)\mmi_option.o \
		  $(TARGETDIR)\mmi_keypad.o \
		  $(TARGETDIR)\mmi_wap_menu.o \
		  $(TARGETDIR)\mmi_dialog.o \
		  $(TARGETDIR)\mmi_musicbell.o

$(WAP_MMI) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS0) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

WAP_MMI_SIS = SOURCE\WAP_B2\REX\MMI\sisdecoder.o

#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / EIF
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\REX\eif
WAP_EIF = $(TARGETDIR)\eifwap.o
				
$(WAP_EIF) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / SMS
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\REX\sms
WAP_SMS = $(TARGETDIR)\aapisms.o
				
$(WAP_SMS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / UDP
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\REX\udp
WAP_UDP = $(TARGETDIR)\aapiudp.o
				
$(WAP_UDP) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / USSD
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\REX\ussd
WAP_USSD = $(TARGETDIR)\aapiussd.o
				
$(WAP_USSD) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
 
#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / WTA
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\REX\wta
WAP_WTA = $(TARGETDIR)\aapiwta.o
			   
$(WAP_WTA) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / REX / WD
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\REX\WD
WAP_WD = $(TARGETDIR)\aapiwd.o
			   
$(WAP_WD) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SECURE
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP_B2\SECURE
WAP_SECURE =$(TARGETDIR)\alert.o \
			$(TARGETDIR)\bitt.o \
			$(TARGETDIR)\bn_add.o \
			$(TARGETDIR)\bn_div.o \
			$(TARGETDIR)\bn_exp.o \
			$(TARGETDIR)\bn_gcd.o \
			$(TARGETDIR)\bn_lib.o \
			$(TARGETDIR)\bn_mont.o \
			$(TARGETDIR)\bn_mul.o \
			$(TARGETDIR)\bn_mulw.o \
			$(TARGETDIR)\bn_print.o \
			$(TARGETDIR)\bn_shift.o \
			$(TARGETDIR)\bn_sqr.o \
			$(TARGETDIR)\bn_sub.o \
			$(TARGETDIR)\cipher.o \
			$(TARGETDIR)\cssls.o \
			$(TARGETDIR)\keygen.o \
			$(TARGETDIR)\md_rand.o \
			$(TARGETDIR)\md5.o \
			$(TARGETDIR)\md5_lib.o \
#			$(TARGETDIR)\prtbuf.o \
			$(TARGETDIR)\rand.o \
			$(TARGETDIR)\rec.o \
			$(TARGETDIR)\rsa_lib.o \
			$(TARGETDIR)\Seedx.o	

$(WAP_SECURE) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
			  
WAP_OBJECTS_ALL = \
              $(WAP_ROOT) \
!IFDEF WAP_LOG
			  $(WAP_LOGGING) \
!ENDIF
              $(WAP_OPTIONAL_CHARSET) \
              $(WAP_OPTIONAL_MEMORY) \
			  $(WAP_SOURCE) \
              $(WAP_CLNT) \
			  $(WAP_MEM) \
              $(WAP_MMI) \
#			  $(WAP_MMI_SIS) \
			  $(WAP_EIF) \
              $(WAP_SMS) \
              $(WAP_UDP) \
			  $(WAP_USSD) \
			  $(WAP_WTA) \
			  $(WAP_WD) \
              $(WAP_SECURE) \

			  
#-------------------------------------------------------------------------------
# WAP End
#-------------------------------------------------------------------------------
!ENDIF

!IFDEF WAP2
#-------------------------------------------------------------------------------
# WAP4.8
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# SOURCE / WAP /
#-------------------------------------------------------------------------------

SRC_DIR = Source\WAP4.8
WAP_ROOT = $(TARGETDIR)\waptask.o \
		   $(TARGETDIR)\wapcmd.o \
		   $(TARGETDIR)\wapaddress.o \
		   $(TARGETDIR)\wapcommon.o \

$(WAP_ROOT) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

!IFDEF WAP_LOG
#-------------------------------------------------------------------------------
# SOURCE / WAP /
#-------------------------------------------------------------------------------

SRC_DIR = Source\WAP4.8
WAP_LOGGING = $(TARGETDIR)\waplog.o

$(WAP_LOGGING) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_LVL_MED) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
!ENDIF

#-------------------------------------------------------------------------------
# SOURCE / WAP / CORE / SOURCE 
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\CORE\SOURCE
WAP_SOURCE = $(TARGETDIR)\alert.o \
             $(TARGETDIR)\buffmgt.o \
			 $(TARGETDIR)\cache.o \
			 $(TARGETDIR)\capiclnt.o \
			 $(TARGETDIR)\capicrpt.o \
			 $(TARGETDIR)\capimem.o \
			 $(TARGETDIR)\capimmi.o \
			 $(TARGETDIR)\capipush.o \
			 $(TARGETDIR)\capiudp.o \
			 $(TARGETDIR)\capiwta.o \
			 $(TARGETDIR)\chpars.o \
			 $(TARGETDIR)\cmmnrsrc.o \
			 $(TARGETDIR)\connectn.o \
			 $(TARGETDIR)\connmgr.o \
			 $(TARGETDIR)\convert.o \
			 $(TARGETDIR)\crypto.o \
			 $(TARGETDIR)\ctypes.o \
			 $(TARGETDIR)\dbfunct.o \
#            $(TARGETDIR)\env.o \
             $(TARGETDIR)\glblDecl.o \
			 $(TARGETDIR)\handshk.o \
			 $(TARGETDIR)\hdrutil.o \
			 $(TARGETDIR)\header.o \
			 $(TARGETDIR)\history.o \
			 $(TARGETDIR)\keyexchg.o \
			 $(TARGETDIR)\logging.o \
			 $(TARGETDIR)\mgtent.o \
			 $(TARGETDIR)\mk_cpu.o \
			 $(TARGETDIR)\mk_main.o \
			 $(TARGETDIR)\mk_outp.o \
			 $(TARGETDIR)\mk_queu.o \
			 $(TARGETDIR)\mk_sche.o \
			 $(TARGETDIR)\mk_user.o \
			 $(TARGETDIR)\ml_buf.o \
			 $(TARGETDIR)\ml_err.o \
			 $(TARGETDIR)\ml_mem.o \
#            $(TARGETDIR)\msgqueue.o \
             $(TARGETDIR)\p_codec.o \
			 $(TARGETDIR)\p_contif.o \
			 $(TARGETDIR)\p_cotok.o \
			 $(TARGETDIR)\p_sidec.o \
			 $(TARGETDIR)\p_sitok.o \
			 $(TARGETDIR)\p_sldec.o \
			 $(TARGETDIR)\p_sltok.o \
			 $(TARGETDIR)\pdubuf.o \
			 $(TARGETDIR)\prov_dec.o \
			 $(TARGETDIR)\provif.o \
			 $(TARGETDIR)\push.o \
			 $(TARGETDIR)\pushelm.o \
			 $(TARGETDIR)\pushll.o \
			 $(TARGETDIR)\pushpckg.o \
			 $(TARGETDIR)\record.o \
			 $(TARGETDIR)\reph.o \
			 $(TARGETDIR)\rephdr.o \
			 $(TARGETDIR)\repll.o \
			 $(TARGETDIR)\sctpred.o \
			 $(TARGETDIR)\si_bpi.o \
			 $(TARGETDIR)\si_crpto.o \
			 $(TARGETDIR)\si_float.o \
			 $(TARGETDIR)\si_frame.o \
			 $(TARGETDIR)\si_fs.o \
			 $(TARGETDIR)\si_instr.o \
			 $(TARGETDIR)\si_int.o \
			 $(TARGETDIR)\si_libs.o \
			 $(TARGETDIR)\si_list.o \
			 $(TARGETDIR)\si_misc.o \
			 $(TARGETDIR)\si_ops.o \
			 $(TARGETDIR)\si_re.o \
			 $(TARGETDIR)\si_sdl.o \
			 $(TARGETDIR)\si_si.o \
			 $(TARGETDIR)\si_sq.o \
			 $(TARGETDIR)\si_var.o \
			 $(TARGETDIR)\si_varop.o \
			 $(TARGETDIR)\si_wtaia.o \
			 $(TARGETDIR)\si_wtaic.o \
			 $(TARGETDIR)\si_wtaig.o \
			 $(TARGETDIR)\sptp_tls.o \
			 $(TARGETDIR)\stackmgr.o \
			 $(TARGETDIR)\storage.o \
			 $(TARGETDIR)\trnscode.o \
			 $(TARGETDIR)\url.o \
			 $(TARGETDIR)\wae.o \
			 $(TARGETDIR)\wae_cmmn.o \
			 $(TARGETDIR)\wae_push.o \
			 $(TARGETDIR)\wae_ua.o \
			 $(TARGETDIR)\wae_wsp.o \
			 $(TARGETDIR)\waectype.o \
			 $(TARGETDIR)\waedef.o \
			 $(TARGETDIR)\waepckg.o \
			 $(TARGETDIR)\waewtaif.o \
			 $(TARGETDIR)\wap.o \
			 $(TARGETDIR)\wapcvt.o \
			 $(TARGETDIR)\wbxmldec.o \
			 $(TARGETDIR)\wbxmlelm.o \
			 $(TARGETDIR)\wdp.o \
			 $(TARGETDIR)\wdp_udp.o \
			 $(TARGETDIR)\wdp_ul.o \
			 $(TARGETDIR)\wdppckg.o \
			 $(TARGETDIR)\wdpsuppt.o \
			 $(TARGETDIR)\wipdb.o \
			 $(TARGETDIR)\wiptimer.o \
			 $(TARGETDIR)\wml_mmi.o \
			 $(TARGETDIR)\wml_uafn.o \
			 $(TARGETDIR)\wmlelm.o \
			 $(TARGETDIR)\wmlevent.o \
			 $(TARGETDIR)\wmlif.o \
			 $(TARGETDIR)\wmlparse.o \
			 $(TARGETDIR)\wmltoken.o \
			 $(TARGETDIR)\wsp.o \
#            $(TARGETDIR)\wspcapa.o \
             $(TARGETDIR)\wspclwdp.o \
			 $(TARGETDIR)\wspcmwtp.o \
			 $(TARGETDIR)\wspif.o \
			 $(TARGETDIR)\wspprocl.o \
			 $(TARGETDIR)\wspprocm.o \
			 $(TARGETDIR)\wta_ua.o \
			 $(TARGETDIR)\wtachdec.o \
			 $(TARGETDIR)\wtachelm.o \
			 $(TARGETDIR)\wtachtok.o \
			 $(TARGETDIR)\wtai_if.o \
			 $(TARGETDIR)\wtapckg.o \
			 $(TARGETDIR)\wtls.o \
			 $(TARGETDIR)\wtls_wdp.o \
			 $(TARGETDIR)\wtlserr.o \
			 $(TARGETDIR)\wtlsmain.o \
			 $(TARGETDIR)\wtlspckg.o \
			 $(TARGETDIR)\wtp_wdp.o \
			 $(TARGETDIR)\wtppckg.o \
			 $(TARGETDIR)\wtpprvdr.o \
			 $(TARGETDIR)\wtpsar.o \
			 $(TARGETDIR)\wtpsuppt.o \
			 $(TARGETDIR)\xml_conv.o \

$(WAP_SOURCE) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
              
#-------------------------------------------------------------------------------
# SOURCE / WAP / CORE / OPTIONAL / CHARSET
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\core\optional\charset
WAP_OPTIONAL_CHARSET = $(TARGETDIR)\charset.o \
				       $(TARGETDIR)\HCodeCnvt.o

$(WAP_OPTIONAL_CHARSET) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / CORE / OPTIONAL / MEMORY
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\core\optional\memory
WAP_OPTIONAL_MEMORY = $(TARGETDIR)\wip_mem.o

$(WAP_OPTIONAL_MEMORY) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / CLNT
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\CLNT
WAP_CLNT = $(TARGETDIR)\clntadapter.o \
		   $(TARGETDIR)\clntapp.o \
		   $(TARGETDIR)\clntPIM.o \
		   $(TARGETDIR)\clntCRYPT.o \
		   $(TARGETDIR)\clntwallet.o

$(WAP_CLNT) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / MEM
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\MEM
WAP_MEM = $(TARGETDIR)\aapimem.o 
#		  $(TARGETDIR)\memadapter.o

$(WAP_MEM) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / MMI
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\MMI
WAP_MMI = $(TARGETDIR)\mmiadapter.o \
		  $(TARGETDIR)\mmicmd.o \
		  $(TARGETDIR)\GenDsp.o \
		  $(TARGETDIR)\mmi_adapter.o \
		  $(TARGETDIR)\mmi_connector.o \
		  $(TARGETDIR)\mmi_wml.o \
		  $(TARGETDIR)\mmi_option.o \
		  $(TARGETDIR)\mmi_keypad.o \
		  $(TARGETDIR)\mmi_wap_menu.o \
		  $(TARGETDIR)\mmi_dialog.o \
		  $(TARGETDIR)\mmi_musicbell.o

$(WAP_MMI) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS0) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#WAP_MMI_SIS = SOURCE\WAP\SKY\MMI\sisdecoder.o

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / PUSH
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\PUSH
WAP_PUSH = $(TARGETDIR)\aapipush.o

$(WAP_PUSH) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / SECURE
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\SECURE
WAP_SECURE = $(TARGETDIR)\aapisecure.o

$(WAP_SECURE) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / EIF
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\EIF
WAP_EIF = $(TARGETDIR)\eifwap.o
				
$(WAP_EIF) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / SMS
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\SMS
WAP_SMS = $(TARGETDIR)\aapisms.o
				
$(WAP_SMS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / UDP
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\UDP
WAP_UDP = $(TARGETDIR)\aapiudp.o
				
$(WAP_UDP) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / USSD
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\USSD
WAP_USSD = $(TARGETDIR)\aapiussd.o
				
$(WAP_USSD) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
 
#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / WTA
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\WTA
WAP_WTA = $(TARGETDIR)\aapiwta.o \
          $(TARGETDIR)\aapiwtai.o \
			   
$(WAP_WTA) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / WD
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\WD
WAP_WD = $(TARGETDIR)\aapiwd.o
			   
$(WAP_WD) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

WAP_OBJECTS_ALL = \
              $(WAP_ROOT) \
!IFDEF WAP_LOG
			  $(WAP_LOGGING) \
!ENDIF
              $(WAP_OPTIONAL_CHARSET) \
              $(WAP_OPTIONAL_MEMORY) \
			  $(WAP_SOURCE) \
              $(WAP_CLNT) \
			  $(WAP_MEM) \
              $(WAP_MMI) \
			  $(WAP_PUSH) \
			  $(WAP_SECURE) \
#			  $(WAP_MMI_SIS) \
			  $(WAP_EIF) \
              $(WAP_SMS) \
              $(WAP_UDP) \
			  $(WAP_USSD) \
			  $(WAP_WTA) \
			  $(WAP_WD) \
			  
#-------------------------------------------------------------------------------
# WAP End
#-------------------------------------------------------------------------------
!ENDIF

include "common.inc"

#===============================================================================
#                               DEPENDENCIES
#===============================================================================

# The dependencies listed at the end of this makefile can be automatically
# updated by making the 'depend' target to invoke the following rules.

#MAKEFILE_NAME   = $(TARGET_NAME).mak
#MAKEFILE_BACKUP = $(TARGET_NAME).bak

MAKEFILE_NAME   = $(MAKEFILE).mak
MAKEFILE_BACKUP = $(MAKEFILE).bak

.SUFFIXES: .s .o .c .dep

# khekim 00/04/04 start
{Source\DS}.c{$(TARGETDIR)}.dep:
        $(CC) $(CFLAGS) -ISource\DS $(DMSS_CFLAGS) -E < $< | \
          perl $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@
# khekim 00/04/04 end

# msrock 00/04/19 start
{Source\Flash}.c{$(TARGETDIR)}.dep:
        $(CC) $(CFLAGS) -ISource\Flash $(DMSS_CFLAGS) -E < $< | \
          perl $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@
# msrock 00/04/19 end

!IFDEF IRDA
# IrDA LIB & interface
{$(IRDA_DIR)}.c{$(TARGETDIR)}.dep:
        $(CC) $(CFLAGS) -I$(IRDA_DIR)   $(DMSS_CFLAGS) -E < $< | \
          perl $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

{$(IRDA_IRUTY)}.c{$(TARGETDIR)}.dep:
        $(CC) $(CFLAGS) -I$(IRDA_IRUTY) $(DMSS_CFLAGS) -E < $< | \
          perl $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

{$(IRDA_IRSIR)}.c{$(TARGETDIR)}.dep:
        $(CC) $(CFLAGS) -I$(IRDA_IRSIR) $(DMSS_CFLAGS) -E < $< | \
          perl $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

{$(IRDA_PROTO)}.c{$(TARGETDIR)}.dep:
        $(CC) $(CFLAGS) -I$(IRDA_PROTO) $(DMSS_CFLAGS) -E < $< | \
          perl $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@
!ENDIF

{$(QUALCOMMDIR)}.c{$(TARGETDIR)}.dep:
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) -E < $< | \
          perl $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

{$(QUALCOMMDIR)}.s{$(TARGETDIR)}.dep:
        $(CC) $(CFLAGS) $(DMSS_AFLAGS) $(DMSS_CFLAGS) -E < $< | \
          perl $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

depend: $(TARGETDIR)\exist $(OBJECTS:.o=.dep) force.frc
        perl $(MDEPEND_SCRIPT) $(MAKEFILE_NAME) $(TARGETDIR) > makefile.tmp
        -del $(MAKEFILE_BACKUP)
        move $(MAKEFILE_NAME) $(MAKEFILE_BACKUP)
        move makefile.tmp $(MAKEFILE_NAME)
force.frc:


#-------------------------------------------------------------------------------
# Target rules specific to this target or set of objects
#-------------------------------------------------------------------------------
# INSERT RULES HERE

$(TARGETDIR)\bit_.c : $(QUALCOMMDIR)\bit.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) bit.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\cmd_.c : $(QUALCOMMDIR)\cmd.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) cmd.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dec5000_.c : $(QUALCOMMDIR)\dec5000.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dec5000.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\diagbuf_.c : $(QUALCOMMDIR)\diagbuf.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) diagbuf.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\ds_snoop_.c : $(QUALCOMMDIR)\ds_snoop.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) ds_snoop.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsm_.c : $(QUALCOMMDIR)\dsm.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsm.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsmgr_.c : $(QUALCOMMDIR)\dsmgr.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsmgr.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlp_.c : $(QUALCOMMDIR)\dsrlp.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlp.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlp3_.c : $(QUALCOMMDIR)\dsrlp3.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlp3.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlpdel_.c : $(QUALCOMMDIR)\dsrlpdel.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlpdel.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlpi_.c : $(QUALCOMMDIR)\dsrlpi.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlpi.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlpnak_.c : $(QUALCOMMDIR)\dsrlpnak.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlpnak.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlprsq_.c : $(QUALCOMMDIR)\dsrlprsq.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlprsq.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlptxq_.c : $(QUALCOMMDIR)\dsrlptxq.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlptxq.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\enc_.c : $(QUALCOMMDIR)\enc.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) enc.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\ffpc_.c : $(QUALCOMMDIR)\ffpc.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) ffpc.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\log_.c : $(QUALCOMMDIR)\log.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) log.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\mar_.c : $(QUALCOMMDIR)\mar.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) mar.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\mctrans_.c : $(QUALCOMMDIR)\mctrans.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) mctrans.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\misc_.c : $(QUALCOMMDIR)\misc.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) misc.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\msg_.c : $(QUALCOMMDIR)\msg.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) msg.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\parm_.c : $(QUALCOMMDIR)\parm.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) parm.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\ppp_.c : $(QUALCOMMDIR)\ppp.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) ppp.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\queue_.c : $(QUALCOMMDIR)\queue.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) queue.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\qw_.c : $(QUALCOMMDIR)\qw.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) qw.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rex_.c : $(QUALCOMMDIR)\rex.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rex.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rlgc_.c : $(QUALCOMMDIR)\rlgc.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rlgc.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rxcdemux_.c : $(QUALCOMMDIR)\rxcdemux.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rxcdemux.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rxcrda_.c : $(QUALCOMMDIR)\rxcrda.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rxcrda.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rxctraffic_.c : $(QUALCOMMDIR)\rxctraffic.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rxctraffic.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\sio_.c : $(QUALCOMMDIR)\sio.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) sio.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\siors232_.c : $(QUALCOMMDIR)\siors232.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) siors232.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\srchdrv_.c : $(QUALCOMMDIR)\srchdrv.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) srchdrv.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\ts_.c : $(QUALCOMMDIR)\ts.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) ts.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\txccommon_.c : $(QUALCOMMDIR)\txccommon.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) txccommon.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\txctraffic_.c : $(QUALCOMMDIR)\txctraffic.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) txctraffic.c MSM5000_IRAM_FWD MSM5000_IRAM_REV


$(TARGETDIR)\bit_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\bit_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT bit_MSM5000_IRAM_FWD_.o
#     armcc -c -I. -I$(ARMINC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 -DT_ARM -DT_MSM3 -DT_MSM5000 -DT_SURF -DTG=T_G -DCUST_H=\"CUSTB2.H\"  -DSHOW_STAT  -DASSERT=ASSERT_FATAL -DMSG_LEVEL=MSG_LVL_ERROR -DMSG_FILE=\"bit_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\bit_MSM5000_IRAM_FWD_.o $(TARGETDIR)\bit_MSM5000_IRAM_FWD_.c
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"bit_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\bit_MSM5000_IRAM_FWD_.o $(TARGETDIR)\bit_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\bit_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\bit_MSM5000_IRAM_FWD_.c
$(TARGETDIR)\bit_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\bit.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) bit.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dec5000_MSM5000_IRAM_FWD_.o
#     armcc -c -I. -I$(ARMINC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 -DT_ARM -DT_MSM3 -DT_MSM5000 -DT_SURF -DTG=T_G -DCUST_H=\"CUSTB2.H\"  -DSHOW_STAT  -DASSERT=ASSERT_FATAL -DMSG_LEVEL=MSG_LVL_MED -DMSG_FILE=\"dec5000_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.c
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dec5000_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------


$(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dec5000_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dec5000.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dec5000.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\diagbuf_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\diagbuf_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT diagbuf_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"diagbuf_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\diagbuf_MSM5000_IRAM_FWD_.o $(TARGETDIR)\diagbuf_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\diagbuf_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\diagbuf_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\diagbuf_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\diagbuf.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) diagbuf.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\ds_snoop_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\ds_snoop_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT ds_snoop_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"ds_snoop_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\ds_snoop_MSM5000_IRAM_FWD_.o $(TARGETDIR)\ds_snoop_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------


$(TARGETDIR)\ds_snoop_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\ds_snoop_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\ds_snoop_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\ds_snoop.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) ds_snoop.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsm_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dsm_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsm_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsm_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dsm_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dsm_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsm_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dsm_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dsm_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dsm.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsm.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsmgr_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dsmgr_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsmgr_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsmgr_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dsmgr_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dsmgr_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsmgr_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dsmgr_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dsmgr_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dsmgr.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsmgr.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlp_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dsrlp_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlp_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlp_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dsrlp_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dsrlp_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlp_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dsrlp_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dsrlp_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dsrlp.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlp.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlp3_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dsrlp3_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlp3_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlp3_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dsrlp3_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dsrlp3_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlp3_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dsrlp3_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dsrlp3_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dsrlp3.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlp3.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlpdel_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dsrlpdel_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlpdel_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlpdel_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dsrlpdel_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dsrlpdel_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlpdel_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dsrlpdel_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dsrlpdel_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dsrlpdel.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlpdel.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlpi_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dsrlpi_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlpi_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlpi_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dsrlpi_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dsrlpi_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlpi_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dsrlpi_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dsrlpi_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dsrlpi.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlpi.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlpnak_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dsrlpnak_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlpnak_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlpnak_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dsrlpnak_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dsrlpnak_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlpnak_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dsrlpnak_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dsrlpnak_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dsrlpnak.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlpnak.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlprsq_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\dsrlprsq_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlprsq_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlprsq_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\dsrlprsq_MSM5000_IRAM_FWD_.o $(TARGETDIR)\dsrlprsq_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlprsq_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\dsrlprsq_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\dsrlprsq_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\dsrlprsq.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlprsq.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\ffpc_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\ffpc_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT ffpc_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_HIGH) -DMSG_FILE=\"ffpc_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\ffpc_MSM5000_IRAM_FWD_.o $(TARGETDIR)\ffpc_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\ffpc_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\ffpc_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\ffpc_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\ffpc.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) ffpc.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\log_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\log_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT log_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_NONE) -DMSG_FILE=\"log_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\log_MSM5000_IRAM_FWD_.o $(TARGETDIR)\log_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\log_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\log_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\log_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\log.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) log.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\misc_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\misc_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT misc_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"misc_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\misc_MSM5000_IRAM_FWD_.o $(TARGETDIR)\misc_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\misc_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\misc_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\misc_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\misc.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) misc.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\msg_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\msg_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT msg_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"msg_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\msg_MSM5000_IRAM_FWD_.o $(TARGETDIR)\msg_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\msg_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\msg_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\msg_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\msg.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) msg.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\parm_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\parm_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT parm_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"parm_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\parm_MSM5000_IRAM_FWD_.o $(TARGETDIR)\parm_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\parm_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\parm_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\parm_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\parm.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) parm.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\ppp_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\ppp_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT ppp_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"ppp_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\ppp_MSM5000_IRAM_FWD_.o $(TARGETDIR)\ppp_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\ppp_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\ppp_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\ppp_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\ppp.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) ppp.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\queue_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\queue_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT queue_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"queue_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\queue_MSM5000_IRAM_FWD_.o $(TARGETDIR)\queue_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\queue_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\queue_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\queue_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\queue.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) queue.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\qw_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\qw_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT qw_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"qw_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\qw_MSM5000_IRAM_FWD_.o $(TARGETDIR)\qw_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\qw_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\qw_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\qw_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\qw.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) qw.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rex_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\rex_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT rex_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"rex_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\rex_MSM5000_IRAM_FWD_.o $(TARGETDIR)\rex_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\rex_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\rex_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\rex_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\rex.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rex.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rxcdemux_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\rxcdemux_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT rxcdemux_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_HIGH) -DMSG_FILE=\"rxcdemux_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\rxcdemux_MSM5000_IRAM_FWD_.o $(TARGETDIR)\rxcdemux_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\rxcdemux_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\rxcdemux_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\rxcdemux_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\rxcdemux.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rxcdemux.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rxcrda_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\rxcrda_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT rxcrda_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_HIGH) -DMSG_FILE=\"rxcrda_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\rxcrda_MSM5000_IRAM_FWD_.o $(TARGETDIR)\rxcrda_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\rxcrda_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\rxcrda_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\rxcrda_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\rxcrda.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rxcrda.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rxctraffic_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\rxctraffic_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT rxctraffic_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_HIGH) -DMSG_FILE=\"rxctraffic_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\rxctraffic_MSM5000_IRAM_FWD_.o $(TARGETDIR)\rxctraffic_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\rxctraffic_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\rxctraffic_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\rxctraffic_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\rxctraffic.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rxctraffic.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\sio_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\sio_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT sio_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"sio_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\sio_MSM5000_IRAM_FWD_.o $(TARGETDIR)\sio_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\sio_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\sio_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\sio_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\sio.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) sio.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\siors232_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\siors232_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT siors232_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"siors232_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\siors232_MSM5000_IRAM_FWD_.o $(TARGETDIR)\siors232_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\siors232_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\siors232_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\siors232_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\siors232.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) siors232.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\txctraffic_MSM5000_IRAM_FWD_.o : $(TARGETDIR)\txctraffic_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT txctraffic_MSM5000_IRAM_FWD_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_HIGH) -DMSG_FILE=\"txctraffic_MSM5000_IRAM_FWD_.c\" -o $(TARGETDIR)\txctraffic_MSM5000_IRAM_FWD_.o $(TARGETDIR)\txctraffic_MSM5000_IRAM_FWD_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\txctraffic_MSM5000_IRAM_FWD_.dep : $(TARGETDIR)\txctraffic_MSM5000_IRAM_FWD_.c

$(TARGETDIR)\txctraffic_MSM5000_IRAM_FWD_.c : $(QUALCOMMDIR)\txctraffic.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) txctraffic.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\cmd_MSM5000_IRAM_REV_.o : $(TARGETDIR)\cmd_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT cmd_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"cmd_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\cmd_MSM5000_IRAM_REV_.o $(TARGETDIR)\cmd_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\cmd_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\cmd_MSM5000_IRAM_REV_.c

$(TARGETDIR)\cmd_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\cmd.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) cmd.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlp_MSM5000_IRAM_REV_.o : $(TARGETDIR)\dsrlp_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlp_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlp_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\dsrlp_MSM5000_IRAM_REV_.o $(TARGETDIR)\dsrlp_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlp_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\dsrlp_MSM5000_IRAM_REV_.c

$(TARGETDIR)\dsrlp_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\dsrlp.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlp.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlpi_MSM5000_IRAM_REV_.o : $(TARGETDIR)\dsrlpi_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlpi_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlpi_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\dsrlpi_MSM5000_IRAM_REV_.o $(TARGETDIR)\dsrlpi_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlpi_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\dsrlpi_MSM5000_IRAM_REV_.c

$(TARGETDIR)\dsrlpi_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\dsrlpi.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlpi.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\dsrlptxq_MSM5000_IRAM_REV_.o : $(TARGETDIR)\dsrlptxq_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT dsrlptxq_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"dsrlptxq_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\dsrlptxq_MSM5000_IRAM_REV_.o $(TARGETDIR)\dsrlptxq_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\dsrlptxq_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\dsrlptxq_MSM5000_IRAM_REV_.c

$(TARGETDIR)\dsrlptxq_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\dsrlptxq.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) dsrlptxq.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\enc_MSM5000_IRAM_REV_.o : $(TARGETDIR)\enc_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT enc_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"enc_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\enc_MSM5000_IRAM_REV_.o $(TARGETDIR)\enc_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\enc_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\enc_MSM5000_IRAM_REV_.c

$(TARGETDIR)\enc_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\enc.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) enc.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\mar_MSM5000_IRAM_REV_.o : $(TARGETDIR)\mar_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT mar_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"mar_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\mar_MSM5000_IRAM_REV_.o $(TARGETDIR)\mar_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\mar_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\mar_MSM5000_IRAM_REV_.c

$(TARGETDIR)\mar_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\mar.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) mar.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\mctrans_MSM5000_IRAM_REV_.o : $(TARGETDIR)\mctrans_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT mctrans_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"mctrans_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\mctrans_MSM5000_IRAM_REV_.o $(TARGETDIR)\mctrans_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\mctrans_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\mctrans_MSM5000_IRAM_REV_.c

$(TARGETDIR)\mctrans_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\mctrans.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) mctrans.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\rlgc_MSM5000_IRAM_REV_.o : $(TARGETDIR)\rlgc_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT rlgc_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_HIGH) -DMSG_FILE=\"rlgc_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\rlgc_MSM5000_IRAM_REV_.o $(TARGETDIR)\rlgc_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\rlgc_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\rlgc_MSM5000_IRAM_REV_.c

$(TARGETDIR)\rlgc_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\rlgc.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) rlgc.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\siors232_MSM5000_IRAM_REV_.o : $(TARGETDIR)\siors232_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT siors232_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"siors232_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\siors232_MSM5000_IRAM_REV_.o $(TARGETDIR)\siors232_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\siors232_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\siors232_MSM5000_IRAM_REV_.c

$(TARGETDIR)\siors232_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\siors232.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) siors232.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\srchdrv_MSM5000_IRAM_REV_.o : $(TARGETDIR)\srchdrv_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT srchdrv_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_MED) -DMSG_FILE=\"srchdrv_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\srchdrv_MSM5000_IRAM_REV_.o $(TARGETDIR)\srchdrv_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\srchdrv_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\srchdrv_MSM5000_IRAM_REV_.c

$(TARGETDIR)\srchdrv_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\srchdrv.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) srchdrv.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\ts_MSM5000_IRAM_REV_.o : $(TARGETDIR)\ts_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT ts_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS) -DMSG_FILE=\"ts_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\ts_MSM5000_IRAM_REV_.o $(TARGETDIR)\ts_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\ts_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\ts_MSM5000_IRAM_REV_.c

$(TARGETDIR)\ts_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\ts.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) ts.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\txccommon_MSM5000_IRAM_REV_.o : $(TARGETDIR)\txccommon_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT txccommon_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_HIGH) -DMSG_FILE=\"txccommon_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\txccommon_MSM5000_IRAM_REV_.o $(TARGETDIR)\txccommon_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\txccommon_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\txccommon_MSM5000_IRAM_REV_.c

$(TARGETDIR)\txccommon_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\txccommon.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) txccommon.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

$(TARGETDIR)\txctraffic_MSM5000_IRAM_REV_.o : $(TARGETDIR)\txctraffic_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------
     @echo OBJECT txctraffic_MSM5000_IRAM_REV_.o
     armcc -c $(INC) -cpu ARM7TDMI -apcs /noswst/interwork -littleend  -fa -g -dwarf2 -Wb -Ospace -O2 $(DMSS_CFLAGS_MSG_LVL_HIGH) -DMSG_FILE=\"txctraffic_MSM5000_IRAM_REV_.c\" -o $(TARGETDIR)\txctraffic_MSM5000_IRAM_REV_.o $(TARGETDIR)\txctraffic_MSM5000_IRAM_REV_.c
     @echo ------------------------------------------------------------------

$(TARGETDIR)\txctraffic_MSM5000_IRAM_REV_.dep : $(TARGETDIR)\txctraffic_MSM5000_IRAM_REV_.c

$(TARGETDIR)\txctraffic_MSM5000_IRAM_REV_.c : $(QUALCOMMDIR)\txctraffic.c
     perl csplit.pl -f $(MAKEFILE_NAME) -t $(TARGETDIR) txctraffic.c MSM5000_IRAM_FWD MSM5000_IRAM_REV

sources : $(TARGETDIR)\bit_.c \
     $(TARGETDIR)\cmd_.c \
     $(TARGETDIR)\dec5000_.c \
     $(TARGETDIR)\diagbuf_.c \
     $(TARGETDIR)\ds_snoop_.c \
     $(TARGETDIR)\dsm_.c \
     $(TARGETDIR)\dsmgr_.c \
     $(TARGETDIR)\dsrlp_.c \
     $(TARGETDIR)\dsrlp3_.c \
     $(TARGETDIR)\dsrlpdel_.c \
     $(TARGETDIR)\dsrlpi_.c \
     $(TARGETDIR)\dsrlpnak_.c \
     $(TARGETDIR)\dsrlprsq_.c \
     $(TARGETDIR)\dsrlptxq_.c \
     $(TARGETDIR)\enc_.c \
     $(TARGETDIR)\ffpc_.c \
     $(TARGETDIR)\log_.c \
     $(TARGETDIR)\mar_.c \
     $(TARGETDIR)\mctrans_.c \
     $(TARGETDIR)\misc_.c \
     $(TARGETDIR)\msg_.c \
     $(TARGETDIR)\parm_.c \
     $(TARGETDIR)\ppp_.c \
     $(TARGETDIR)\queue_.c \
     $(TARGETDIR)\qw_.c \
     $(TARGETDIR)\rex_.c \
     $(TARGETDIR)\rlgc_.c \
     $(TARGETDIR)\rxcdemux_.c \
     $(TARGETDIR)\rxcrda_.c \
     $(TARGETDIR)\rxctraffic_.c \
     $(TARGETDIR)\sio_.c \
     $(TARGETDIR)\siors232_.c \
     $(TARGETDIR)\srchdrv_.c \
     $(TARGETDIR)\ts_.c \
     $(TARGETDIR)\txccommon_.c \
     $(TARGETDIR)\txctraffic_.c 

# ------------------------------
# DO NOT EDIT BELOW THIS LINE

# End of auto generated dependencies.
