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
#   1. The ARM SDT 2.50 tools are installed in the
#       c:\progra~1\arm250 directory.
#   2. Perl is available either locally or via the environment path.
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


#-------------------------------------------------------------------------------
# Target global definitions
#-------------------------------------------------------------------------------

MODEL_ID     = MODEL_B2
TARGET_NAME  = Bunny              # Target name for output files and object dir
CUSTNAME     = B2


################################################################################
#
#
#      Edit this area for Model dependent
#
#              --------------------------------------------->
#
################################################################################

#-------------------------------------------------------------------------------
# Select NV type
#-------------------------------------------------------------------------------

#NV_TYPE      = EEPROM            # Use EEPROM
NV_TYPE      = EFS                # Use FLASH memory(QUALCOMM EFS)
#NV_TYPE      = FDI               # Use FLASH memory(INTEL FDI)

#
#   !if "$(NV_TYPE)" == "EEPROM"
#   CUSTNAME     = SC
#   NV           = -DUSE_EEPROM
#   OBJ_ROOT     = OBJ_EEP
#   !endif                          # !if "$(NV_TYPE)" == "EEPROM"
#   !if "$(NV_TYPE)" == "EFS"
#   CUSTNAME     = SCE
#   NV           = -DNOT_USE_EEPROM
#   OBJ_ROOT      = OBJ
#   !endif                          # !if "$(NV_TYPE)" == "EFS"
#


#-------------------------------------------------------------------------------
# Select EEPROM type
#-------------------------------------------------------------------------------

#EEPROM_TYPE     = 24LCXX
#EEPROM_TYPE     = X84XXX


#-------------------------------------------------------------------------------
# Select PLL type
#-------------------------------------------------------------------------------

#PLL_TYPE     = LMX233X
#PLL_TYPE     = MB15F02

#-------------------------------------------------------------------------------
# Select MMI type		by jnlee 01.06.28
#-------------------------------------------------------------------------------

#EFS_TYPE     = Q_TYPE		#qualcomm original style
EFS_TYPE     = SK_TYPE		#sktt style for 1 flash

#-------------------------------------------------------------------------------
# Select MMI type
#-------------------------------------------------------------------------------

MMI_TYPE     = SKY_UI

#-------------------------------------------------------------------------------
# Select SKY model ID
#-------------------------------------------------------------------------------

SKY_MODEL_ID   = SKY_MODEL_B2

#-------------------------------------------------------------------------------
# Select WAP compiling 
#-------------------------------------------------------------------------------

# WAP4.3 Compile
WAP2 = FEATURE_WAP4.8

!IFDEF WAP2
!ELSE
WAP = FEATURE_WAP
!ENDIF

#/* rosa 01-02-02 --> @why, what */
#-------------------------------------------------------------------------------
# Select SocketApi compiling 
#-------------------------------------------------------------------------------

SOCKAPI = USE_SOCKET

#-------------------------------------------------------------------------------
# GVM compiling 
#-------------------------------------------------------------------------------

GVM = USE_GVM
EMAIL = USE_EMAIL
#/* <-- */

#/* rosa 01-02-02 --> @why, what */
#-------------------------------------------------------------------------------
# Define symbols for photo mail 
#-------------------------------------------------------------------------------
PHOTOMAIL = -DUSE_PHOTOMAIL

#-------------------------------------------------------------------------------
# SKVM compiling 
#-------------------------------------------------------------------------------
SKVM = -DUSE_SKVM

#-------------------------------------------------------------------------------
# Define symbols for VAS 
#-------------------------------------------------------------------------------
VAS = -DUSE_VAS
VAS_DEF = -DFEATURE_VAS

#-------------------------------------------------------------------------------
# Select EIF compiling
#-------------------------------------------------------------------------------
EIF	= USE_EIF#

#-------------------------------------------------------------------------------
# Define M-Commerce
#-------------------------------------------------------------------------------
MCOMMERCE = -DFEATURE_MCOMMERCE

#-------------------------------------------------------------------------------
# Define CHARACBELL
#-------------------------------------------------------------------------------
CHARACTER_BELL = -DFEATURE_CHARACTER_BELL

#-------------------------------------------------------------------------------
# Define SISDecoder (MLZ)
#-------------------------------------------------------------------------------
MLZ = -DFEATURE_MLZ

#-------------------------------------------------------------------------------
# Define symbols
#-------------------------------------------------------------------------------
#EQUS_SYMBOL_DEFINE = -DCONFIG_EQS_COLOR_256
EQUS_SYMBOL_DEFINE = -DCONFIG_EQS_COLOR_64K
SKY_SYMBOL_DEFINE = -DSKY_MODEL_ID=$(SKY_MODEL_ID) \
                    $(EQUS_SYMBOL_DEFINE)
MMI               = -DMMI_TYPE=$(MMI_TYPE)

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
!IFDEF CHARACTER_BELL
SKY_APP_CHARACBELL_DIR    = $(SKY_APP_DIR)\CHARACBELL
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

#-------------------------------------------------------------------------------
# Select melody IC type
#-------------------------------------------------------------------------------

#MELODY_IC_TYPE = YAMAHA

#-------------------------------------------------------------------------------
# If You Want to compile IrDA, 
#-------------------------------------------------------------------------------
IRDA       = USE_IRDA

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
# If You Want to compile IrDA, 
#-------------------------------------------------------------------------------
FEATURE_WAVELET = USE_FEATURE_WAVELET

################################################################################
#
#      <---------------------------------------------
#
#              Edit this area for Model dependent
#
#
################################################################################


#-------------------------------------------------------------------------------
# Target file name and type definitions
#-------------------------------------------------------------------------------

TARGETID     = T_G                # Target ID defined on compile line
EXETYPE      = elf                # Target image file format
CUSTFILE     = CUST$(CUSTNAME).H  # Target feature definition include file
MAKE_TARGET  = $(TARGET_NAME)     # Target Makefile name
STRUCT_ALIGN = -zat4              # Aligns structures to 32bit boundaries

QUALCOMM_DIR = Source\QUALCOMM30
SRC_DIR      = $(QUALCOMM_DIR)

# Target scatter load file for ARM SDT linker
SCATTER_DIR  = Source\ModelDep
SCLFILE      = $(SCATTER_DIR)\$(TARGET_NAME).scl 

OBJ_ROOT     = OBJ
OBJ_DIR      = $(OBJ_ROOT)\$(TARGET_NAME)

TARGET       = $(OBJ_DIR)
TARGET_DIR   = .\DownLoad
TARGET_EXE   = $(TARGET_DIR)\$(TARGET_NAME).$(EXETYPE)
TARGET_HEX   = $(TARGET_DIR)\$(TARGET_NAME).hex
TARGET_BIN   = $(TARGET_DIR)\$(TARGET_NAME).bin

BOOT_BIN     = $(TARGET_DIR)\BootB2.bin

MAKE_HEX     = $(QUALCOMM_DIR)\fromelf
HEX2BIN      = $(TARGET_DIR)\hexbin2
MAKE_BIN     = $(TARGET_DIR)\MakeBin

#-------------------------------------------------------------------------------
# Target compile time symbol definitions
#-------------------------------------------------------------------------------
!IFDEF IRDA                             # temperory ...  
TG      = -DTG=$(TARGETID) -DMAKE_IRDA  # Target identifier definition
!ELSE
TG      = -DTG=$(TARGETID)              # Target identifier definition
!ENDIF
CUSTH   = -DCUST_H=\"$(CUSTFILE)\"      # Feature include file definition
ARM_ASM = -D_ARM_ASM_                   # ARM assembly language selection
ARM     = -DT_ARM                       # ARM processor target
MSM     = -DT_MSM3 -DT_MSM5000
SURF    = -DT_SURF                      # SURF card target

CUSTH   = $(CUSTH) -DMODEL_DEP_H=\"Bunny.h\"


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
DMSS_CFLAGS_COMMON = $(ARM) $(MSM) $(SURF) $(TG) $(CUSTH) $(SHOW_STAT) $(TIMETEST) $(MMI) $(SKY_SYMBOL_DEFINE)
DMSS_CFLAGS_COMMON4EQUS = $(ARM) $(MSM) $(SURF) $(TG) $(CUSTH) $(TIMETEST) $(MMI) $(EQUS_SYMBOL_DEFINE)
DMSS_CFLAGS_COMMON4SKY = $(ARM) $(MSM) $(SURF) $(TG) $(CUSTH) $(TIMETEST) $(MMI) $(SKY_SYMBOL_DEFINE)

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

!IFDEF FEATURE_WAVELET
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

# Module specific definitions
DMSS_CFLAGS_ACP  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_DS   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_MED)
DMSS_CFLAGS_RLP  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_MC   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_HIGH)
DMSS_CFLAGS_MOB  = $(DMSS_CFLAGS_COMMON)
DMSS_CFLAGS_NONE = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_NV   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_RXTX = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SND  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SRCH = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_UI   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_CM   = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_VOC  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SIO  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_DEC  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_ENC  = $(DMSS_CFLAGS_COMMON) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_EQUS = $(DMSS_CFLAGS_COMMON4EQUS) $(ASSERT_FATAL) $(MSG_LVL_NONE)
DMSS_CFLAGS_SKY  = $(DMSS_CFLAGS_COMMON4SKY) $(ASSERT_FATAL) $(MSG_LVL_MED)
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
WAP_DEF     = -DFEATURE_WAP -DTRGT_REX -DUSE_WIP_MALLOC -DUSE_PROPRIETARY_WMLS_LIBS -DWAPDORMANT
WAP_LOG     = -DLOG_ENABLE
DMSS_CFLAGS_COMMON = $(DMSS_CFLAGS_COMMON) $(WAP_DEF) $(WAP_LOG)
DMSS_CFLAGS_SKY = $(DMSS_CFLAGS_SKY) $(WAP_DEF) $(WAP_LOG)
!ENDIF

#-------------------------------------------------------------------------------
# Assembler symbol definitions
#-------------------------------------------------------------------------------

DMSS_AFLAGS = $(ARM) $(MSM) $(TG) $(CUSTH) $(ARM_ASM) $(TIMETEST)

#===============================================================================
#                         TARGET OBJECT FILE LIST
#===============================================================================

# The following is an alphabetically sorted list of all objects required to
# build the target image file and hex file.  The following objects are built 
# according to the module specific rules and default suffix rules defined in
# this makefile.

OBJECTS =       \
#               $(TARGET)\acp553.o \
#               $(TARGET)\acpbch.o \
#               $(TARGET)\acpfm.o \
#               $(TARGET)\acplog.o \
#               $(TARGET)\acpmc.o \
#               $(TARGET)\acpmcidl.o \
#               $(TARGET)\acpmcini.o \
#               $(TARGET)\acpmcsa.o \
#               $(TARGET)\acpmcsup.o \
#               $(TARGET)\acpmcvc.o \
#               $(TARGET)\acprx.o \
#               $(TARGET)\acprxcmd.o \
#               $(TARGET)\acprxmsg.o \
#               $(TARGET)\acpsamsg.o \
#               $(TARGET)\acpsasup.o \
#               $(TARGET)\acptx.o \
#               $(TARGET)\acptxcmd.o \
#               $(TARGET)\acpvcmsg.o \
#               $(TARGET)\acpvcsup.o \
                $(TARGET)\acpwbm2.o \
                $(TARGET)\adcg.o \
                $(TARGET)\atmr.o \
#               $(TARGET)\auth.o \
#bwpark         $(TARGET)\bbdiagp.o \
                $(TARGET)\bbsndtab.o \
                $(TARGET)\bch.o \
                $(TARGET)\bio.o \
                $(TARGET)\bit.o \
                $(TARGET)\boot_trap.o \
                $(TARGET)\bootapp.o \
                $(TARGET)\bootdata.o \
                $(TARGET)\boothw.o \
                $(TARGET)\bootmem.o \
                $(TARGET)\bootsys.o \
                $(TARGET)\caix.o \
#               $(TARGET)\cave.o \
                $(TARGET)\clk.o \
                $(TARGET)\clkjul.o \
                $(TARGET)\clkm2p.o \
                $(TARGET)\clkregim.o \
                $(TARGET)\cm.o \
                $(TARGET)\cmcall.o \
                $(TARGET)\cmclient.o \
                $(TARGET)\cmd.o \
                $(TARGET)\cminband.o \
                $(TARGET)\cmnv.o \
                $(TARGET)\cmph.o \
                $(TARGET)\cmsms.o \
                $(TARGET)\cmss.o \
                $(TARGET)\crc.o \
                $(TARGET)\db.o \
                $(TARGET)\dec5000.o \
                $(TARGET)\deint.o \
                $(TARGET)\diag.o \
                $(TARGET)\diagbuf.o\
                $(TARGET)\diagfs.o \
                $(TARGET)\diag_guid.o \
                $(TARGET)\diagp.o \
                $(TARGET)\diag_table.o \
                $(TARGET)\diagx.o \
                $(TARGET)\distreg.o \
                $(TARGET)\dloadarm.o \
                $(TARGET)\dloadpoll.o \
                $(TARGET)\dloaduart.o \
                $(TARGET)\dmdimage.o \
                $(TARGET)\dmddown.o \
                $(TARGET)\dmod.o \
                $(TARGET)\dog.o \
                $(TARGET)\ds_snoop.o \
                $(TARGET)\dsatcop.o \
                $(TARGET)\dsatdat.o \
                $(TARGET)\dsatps.o \
                $(TARGET)\dsatsio.o \
                $(TARGET)\dsctl.o \
                $(TARGET)\dsm.o \
                $(TARGET)\dsmgr.o \
                $(TARGET)\dsnetmdl.o \
# bwpark 잠시 막아둠				$(TARGET)\dsretry.o \
                $(TARGET)\dsrlp.o \
                $(TARGET)\dsrlpi.o \
                $(TARGET)\dsrlpdel.o\
                $(TARGET)\dsrlp3.o \
                $(TARGET)\dsrlpnak.o \
                $(TARGET)\dsrlprsq.o \
                $(TARGET)\dsrlprtx.o \
                $(TARGET)\dsrlptxq.o \
                $(TARGET)\dssdorm.o \
# bwpark            $(TARGET)\dssicmp.o \
# temp               $(TARGET)\dsskt.o \
# temp               $(TARGET)\dsatskt.o \
                $(TARGET)\dssnet.o \
                $(TARGET)\dssocfg.o \
                $(TARGET)\dssocket.o \
                $(TARGET)\dssocki.o \
                $(TARGET)\dssoctl.o \
                $(TARGET)\dsstcp.o \
                $(TARGET)\dssudp.o \
# girin. 2000.8.31
                $(TARGET)\dsscrm.o \
                $(TARGET)\dssdns.o \
# girin. end.
                $(TARGET)\enc.o \
                $(TARGET)\err.o \
                $(TARGET)\event.o\
                $(TARGET)\feature.o \
                $(TARGET)\ffpc.o \
                $(TARGET)\fs.o \
                $(TARGET)\fs_alloc.o \
                $(TARGET)\fs_am.o \
                $(TARGET)\fs_dev.o \
                $(TARGET)\fs_dir.o \
                $(TARGET)\fs_ops.o \
                $(TARGET)\fs_mpoint.o \
                $(TARGET)\fs_udir.o \
!if "$(EFS_TYPE)" == "Q_TYPE"		#EFS of qualcomm original way
                $(TARGET)\fsd_intel.o \
!endif
#                $(TARGET)\hsg.o \
                $(TARGET)\hw.o \
                $(TARGET)\icmp.o \
                $(TARGET)\ip.o \
                $(TARGET)\iphdr.o \
#                $(TARGET)\lcd.o \
!IF "$(PLL_TYPE)" == "LMX233X"
                $(TARGET)\lmx233x.o \
!ELSEIF "$(PLL_TYPE)" == "MB15F02"          # !IF "$(PLL_TYPE)" ==
                $(TARGET)\MB15F02.o \
!ELSEIF "$(PLL_TYPE)" == "LMX160X"          # !IF "$(PLL_TYPE)" ==
                $(TARGET)\LMX160X.o \
!ENDIF                                      # !IF "$(PLL_TYPE)" ==
                $(TARGET)\lifetstg.o \
                $(TARGET)\log.o \
                $(TARGET)\loopback.o \
                $(TARGET)\mar.o \
                $(TARGET)\martable.o \
                $(TARGET)\mc.o \
                $(TARGET)\mccap.o \
                $(TARGET)\mccccl.o \
                $(TARGET)\mccdma.o \
                $(TARGET)\mccidl.o \
                $(TARGET)\mccini.o \
                $(TARGET)\mccqpch.o \
                $(TARGET)\mccreg.o \
                $(TARGET)\mccrx.o \
                $(TARGET)\mccrxtx.o \
                $(TARGET)\mccsa.o \
                $(TARGET)\mccso.o \
                $(TARGET)\mccsch.o \
                $(TARGET)\mccscm.o \
                $(TARGET)\mccsrch.o \
                $(TARGET)\mccsup.o \
                $(TARGET)\mctrans.o\
                $(TARGET)\mccsyobj.o \
                $(TARGET)\mcctc.o \
                $(TARGET)\mcctcho.o \
                $(TARGET)\mcctcsup.o \
                $(TARGET)\mcctxns.o \
                $(TARGET)\mcnvdf.o \
                $(TARGET)\mcscript.o \
                $(TARGET)\mcsysci.o \
                $(TARGET)\mcsyspr.o \
                $(TARGET)\mcsyspra.o \
                $(TARGET)\mcsysprd.o \
                $(TARGET)\mcsyssup.o \
                $(TARGET)\mdrrlp.o \
                $(TARGET)\misc.o \
                $(TARGET)\mobile.o \
                $(TARGET)\msg.o \
                $(TARGET)\msm5_init.o \
                $(TARGET)\nvim.o \
                $(TARGET)\nvimnv.o \
                $(TARGET)\nvimio.o \
                $(TARGET)\nvimr.o \
                $(TARGET)\nvimw.o \
				$(TARGET)\nvsetup.o \
				$(TARGET)\otasp.o \
                $(TARGET)\otaspx.o \
                $(TARGET)\parm.o \
                $(TARGET)\pnmask.o \
                $(TARGET)\ppp.o \
                $(TARGET)\pppfsm.o \
                $(TARGET)\pppipcp.o \
                $(TARGET)\ppplcp.o \
                $(TARGET)\prl.o \
                $(TARGET)\psctl.o \
                $(TARGET)\psmgr.o \
                $(TARGET)\psmisc.o \
                $(TARGET)\pstimer.o \
                $(TARGET)\queue.o \
                $(TARGET)\qw.o \
                $(TARGET)\ran.o \
                $(TARGET)\rex.o \
                $(TARGET)\rexarm.o \
                $(TARGET)\rfcs.o \
                $(TARGET)\rfmsm.o \
                $(TARGET)\rfnv.o \
                $(TARGET)\ring.o \
                $(TARGET)\rlgc.o \
                $(TARGET)\rlp.o \
                $(TARGET)\rx.o \
  			    $(TARGET)\rxccommon.o \
				$(TARGET)\rxcdemux.o \
				$(TARGET)\rxcpaging.o \
				$(TARGET)\rxcrda.o \
				$(TARGET)\rxcsync.o \
				$(TARGET)\rxctraffic.o \
				$(TARGET)\txcaccess.o \
				$(TARGET)\txccommon.o \
				$(TARGET)\txcmux.o \
				$(TARGET)\txcso.o \
				$(TARGET)\txctraffic.o \
                $(TARGET)\rxtx.o \
                $(TARGET)\sbi.o \
                $(TARGET)\sdevmap.o \
                $(TARGET)\sio.o \
                $(TARGET)\siog2.o \
                $(TARGET)\siors232.o \
                $(TARGET)\sleep.o \
                $(TARGET)\slhc.o \
                $(TARGET)\snd.o \
                $(TARGET)\sndhwg2.o \
                $(TARGET)\snditab.o \
                $(TARGET)\snm.o \
                $(TARGET)\srch.o \
                $(TARGET)\srchaq.o \
                $(TARGET)\srchcd.o \
                $(TARGET)\srchdrv.o \
                $(TARGET)\srchdz.o \
                $(TARGET)\srchint.o \
                $(TARGET)\srchpc.o \
                $(TARGET)\srchqpch.o \
                $(TARGET)\srchsc.o \
                $(TARGET)\srchsl.o \
                $(TARGET)\srchst.o \
                $(TARGET)\srchtc.o \
                $(TARGET)\srchtri.o \
                $(TARGET)\srchun.o \
                $(TARGET)\srchzz.o \
                $(TARGET)\srv.o \
                $(TARGET)\task.o \
                $(TARGET)\tcphdr.o \
                $(TARGET)\tcpin.o \
                $(TARGET)\tcpout.o \
                $(TARGET)\tcpshell.o \
                $(TARGET)\tcpsubr.o \
                $(TARGET)\tcptimer.o \
                $(TARGET)\therm.o \
                $(TARGET)\tmsi.o \
                $(TARGET)\tdso.o \
#bwpark
#                $(TARGET)\trampm3.o \
                $(TARGET)\trampm5.o \
                $(TARGET)\ts.o \
                $(TARGET)\tx.o \
#	happykid
#                $(TARGET)\txc.o \
                $(TARGET)\uasms.o \
                $(TARGET)\uasmsnv.o \
                $(TARGET)\uasmsx.o \
                $(TARGET)\uasmsu.o \
                $(TARGET)\udp.o \
#                $(TARGET)\ui.o \
#                $(TARGET)\uinB5.o\
#                $(TARGET)\uihcbt.o \
#                $(TARGET)\uihcmd.o \
#                $(TARGET)\uihkey.o \
#                $(TARGET)\uihsig.o \
#                $(TARGET)\uisalph.o \
#                $(TARGET)\uiscall.o \
#				$(TARGET)\uiscli.o \
#                $(TARGET)\uiscode.o \
#                $(TARGET)\uishelp.o \
#                $(TARGET)\uisidle.o \
#                $(TARGET)\uisinfo.o \
#                $(TARGET)\uislist.o \
#                $(TARGET)\uislock.o \
#                $(TARGET)\uislpm.o \
#                $(TARGET)\uismenu.o \
#                $(TARGET)\uismsg.o \
#                $(TARGET)\uisnum.o \
#                $(TARGET)\uisoff.o \
#                $(TARGET)\uisrcl.o \
#                $(TARGET)\uisserv.o \
# reo           $(TARGET)\uissms.o \
#                $(TARGET)\uisstrt.o \
#                $(TARGET)\uissto.o \
#                $(TARGET)\uistate.o \
#                $(TARGET)\uisview.o \
#                $(TARGET)\uiumenu.o \
#                $(TARGET)\uiudata.o \
#reo            $(TARGET)\uiusmsd.o \
#reo            $(TARGET)\uiusmsl.o \
#hjcho          $(TARGET)\uiutstmn.o \
#                $(TARGET)\uiutxt.o \
#                $(TARGET)\uixcm.o \
#                $(TARGET)\uixnv.o \
#                $(TARGET)\uixscrn.o \
#                $(TARGET)\uixsnd.o \
                $(TARGET)\uixuasms.o \
                $(TARGET)\ulpn.o \
                $(TARGET)\vbatt.o \
                $(TARGET)\vocdown.o \
                $(TARGET)\vocimags.o \
                $(TARGET)\vocm2.o \
                $(TARGET)\vocmux.o \
                $(TARGET)\vocsup.o \

#===============================================================================
#                             TOOL DEFINITIONS
#===============================================================================
# The following environment variables must be defined prior to using this make
# file: ARMBIN, ARMLIB, and ARMINC. In addition the PATH must be updated for
# the ARM tools.

#-------------------------------------------------------------------------------
# Set Compiler Environment
#-------------------------------------------------------------------------------
include      "arm250env.txt"

#-------------------------------------------------------------------------------
# Software tool and environment definitions
#-------------------------------------------------------------------------------
CC         = $(ARMBIN)\tcc      # ARM SDT Thumb 16-bit inst. set ANSI C compiler
ARMCC      = $(ARMBIN)\armcc    # ARM SDT ARM 32-bit inst. set ANSI C compiler
ASM        = $(ARMBIN)\armasm   # ARM SDT assembler
LD         = $(ARMBIN)\armlink  # ARM SDT linker
HEXTOOL    = $(ARMBIN)\fromelf  # ARM SDT utility to create hex file from image
PERLTOOL   = $(PERLPATH)\perl

OBJ_CMD    = -o                 # Command line option to specify output filename
DIRTCC     = dirtcc             # Utility to include directory name in mobile.o
ASM_SCRIPT = $(QUALCOMM_DIR)\asm.pl # Perl script to include .h files in assembly
CAT_SCRIPT = $(QUALCOMM_DIR)\cat.pl # Perl script to print output for assembly files
ERRLOG     = errorlog.exe       # Tool extracting only warning, error, and 
                                # serious errors from log file.

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
INTRWORK = nointerwork          # No ARM/Thumb interworking support
NARROW   = narrow               # Use caller parameter narrowing
REENTRNT = nonreentrant         # No support for re-entrancy

APCS = -apcs /$(STACKCHK)/$(INTRWORK)/$(FLOATPNT)/$(NARROW)

#-------------------------------------------------------------------------------
# start   by jnlee (99.11.5) for interwork fs_****.c 
INTRWORK_YES = interwork        # ARM/Thumb interworking support
INTRWORK_APCS = -apcs /$(STACKCHK)/$(INTRWORK_YES)/$(FLOATPNT)/$(NARROW)
# end       by jnlee
#-------------------------------------------------------------------------------

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

DBG = -g+ -dwarf2               # Enable DWARF2 format debug tables

#-------------------------------------------------------------------------------
# Compiler optimization options
#-------------------------------------------------------------------------------

OPT = -Ospace -O2               # Full compiler optimization for space
#OPT = -O0                        # No compiler optimization for space

#-------------------------------------------------------------------------------
# Compiler code generation options
#-------------------------------------------------------------------------------

END = -littleend                # Compile for little endian memory architecture
ZA  = -za1                      # LDR may only access 32-bit aligned addresses
ZAS = -zas4                     # Min byte alignment for structures
ZAP = -zap0                     # Struct ptrs NOT assumed to be aligned per -zas
ZAT = -zat1                     # Min byte alignment for top-level static objects
ZZT = -zzt0                     # Force uninitialized variables to ZI data area

CODE = $(END) $(ZA) $(ZAS) $(ZAP) $(ZAT)

#/* rosa 01-02-02 --> @why, what */
!IFDEF GVM
GVM_CODE = -littleend -za1 -zas4 -zap0 -zat2 -zzt0
!ENDIF
#/* <-- */

!IFDEF SKVM
SKVM_CODE = -littleend -za1 -zas4 -zap0 -zat1 -zc
SKVM_DBG = -g-
SKVM_OPT = -O1
!ENDIF

# NOTE:
# The -zap0 option is required due to a known ARM 2.50 compiler bug with
# pointers to struct sub-fields within __packed structs

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



SKTT_INC_DIR = -ISource\DS \
               -ISource\Audio \
#               -ISource\smb_b1 \
               -ISource\LCD \
			   -ISource\Flash \
               -ISource\SockApi \
               -ISource\ModelDep

!IFDEF WAP2
APP_INC = -IInclude -I$(QUALCOMM_DIR) -I$(ARMINC) $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR) 
INC = -IInclude -I$(QUALCOMM_DIR) -I$(ARMINC) $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR) $(IRDA_INC) $(WAP_INC)
!ELSE
APP_INC = -IInclude -I$(QUALCOMM_DIR) -I$(ARMINC) $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR)
INC = -IInclude -I$(QUALCOMM_DIR) -I$(ARMINC) $(EQUS_INC_DIRS) $(SKY_INC_DIRS) $(SKTT_INC_DIR) $(IRDA_INC)
!ENDIF


# NOTE:
# The -I. option allows memory.h to be found in the current directory when
# included using #include <memory.h>

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

MSG =  -Wb                         # No additional message control

#-------------------------------------------------------------------------------
# Additional compile time error checking options
#-------------------------------------------------------------------------------

CHK = -fa                       # Check for data flow anomolies

#-------------------------------------------------------------------------------
# Linker options
#-------------------------------------------------------------------------------

MAP  = -map                             # Memory map of all object file areas
INFO = -info sizes,totals,interwork     # Info on object size and interworking
XREF = -xref                            # List cross-references between files
VERB = -verbose                         # Print verbose output messages
LIST = -list $(TARGET_NAME).map         # Direct map and info output to file
SYM  = -symbols $(TARGET_NAME).sym      # Direct symbol table to file
LINK = -scatter $(SCLFILE)              # Use scatter load description file

#-------------------------------------------------------------------------------
# Linker library options
#-------------------------------------------------------------------------------

CLIB  = $(ARMLIB)\armlib.16l             # ARM ANSI C library
#CLIB = $(ARMLIB)\embedded\armlib_i.16l  # ARM embedded ANSI C library
!IFDEF MLZ
SISLIB = $(WAP_DIR)\saf_lib_tcc_010920_MLZ.alf	#\sislib_arm.alf
!ELSE
SISLIB = $(WAP_DIR)\saf_lib_tcc_010531.alf	#\sislib_arm.alf
!ENDIF

!IFDEF CHARACTER_BELL
MMFLIB = $(WAP_DIR)\mmf_arm_lib_010820.alf	#\mmflib_arm.alf
LIBS = $(CLIB) $(SISLIB)  $(MMFLIB)
!ELSE
LIBS = $(CLIB) $(SISLIB)
!ENDIF 

#-------------------------------------------------------------------------------
# Compiler flag definitions
#-------------------------------------------------------------------------------

CFLAGS0 = $(OUT) $(INC) $(CPU) $(APCS) $(CODE) $(PRAGMA) $(CHK) $(DBG) $(MSG)
CFLAGS  = $(CFLAGS0) $(OPT)

#-------------------------------------------------------------------------------
# start by jnlee 99.11.5 for 'interwork option '
INTRWORK_CFLAGS0 = $(OUT) $(INC) $(CPU) $(INTRWORK_APCS) $(CODE) $(PRAGMA) $(CHK) $(DBG) $(MSG) 
INTRWORK_CFLAGS  = $(INTRWORK_CFLAGS0) $(OPT)
# end by jnlee 
#-------------------------------------------------------------------------------

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

LFLAGS = $(MAP) $(INFO) $(LIST) $(SYM) $(LINK)

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

#all : $(TARGET)\exist $(TARGET_EXE)
#         @-del $(TARGET)\mobile.o
all : $(TARGET)\exist $(TARGET_BIN)

#-------------------------------------------------------------------------------
# Object directory target
#-------------------------------------------------------------------------------

# The object subdirectoy is created if it does not already exist.

$(TARGET)\exist :
         @-if not exist $(OBJ_ROOT) mkdir $(OBJ_ROOT)
         @-if not exist $(TARGET)\exist mkdir $(TARGET)
         @echo Building $(TARGET) > $(TARGET)\exist

#-------------------------------------------------------------------------------
# Image file target
#-------------------------------------------------------------------------------

# generate the BIN files
$(TARGET_BIN) : $(TARGET_HEX)
        @echo ------------------------------------------------------------------
        @echo Generate the BIN files
        @$(HEX2BIN) $(TARGET_HEX) $(TARGET_BIN) I 0000 5FFFF 2
        @echo ------------------------------------------------------------------
        @echo Make bin complete
        @echo ------------------------------------------------------------------
        @$(MAKE_BIN) $(TARGET_BIN) $(BOOT_BIN) 600000
        @echo ------------------------------------------------------------------

# The target image file is produced by the linker in the selected image format,
# and then a hex file is created from the target image file.

# Hex file
$(TARGET_HEX) : $(TARGET_EXE)
        @echo ---------------------------------------------------------------
        @echo TARGET $(TARGET_HEX)
        $(HEXTOOL) -nozeropad $(TARGET_EXE) -i32 $(TARGET_HEX)
        @echo ---------------------------------------------------------------


#-------------------------------------------------------------------------------
# Clean target
#-------------------------------------------------------------------------------

# The object subdirectory, target image file, and target hex file are deleted.

clean :
        @echo ---------------------------------------------------------------
        @echo CLEAN
        -if exist $(TARGET) rmdir /s/q $(TARGET)
        -if exist $(TARGET_EXE) del /f $(TARGET_EXE)
        -if exist $(TARGET_HEX) del /f $(TARGET_HEX)
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

# The ARM SDT 2.50 tool versions are listed.

tools :
        @echo ------------------------------------------------------------------
        @echo ARM SDT 2.50 TOOLS
        @$(ARMCC) -vsn
        @$(CC)    -vsn
        @$(ASM)   -vsn
        @$(LD)    -vsn
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

SRC_FILE = $(SRC_DIR)\$(@F:.o=.c)       # Input source file specification
OBJ_FILE = $(OBJ_CMD) $(TARGET)\$(@F)   # Output object file specification
MSG_FILE = -DMSG_FILE=\"$(@F:.o=.c)\"   # MSG_FILE symbol definition

#-------------------------------------------------------------------------------
# C code inference rules
#-------------------------------------------------------------------------------

.c.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
         
{$(QUALCOMM_DIR)}.c.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
         
.c{$(TARGET)}.o:
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
        $(CC) -E $(APP_INC) $(DMSS_AFLAGS) < $< | $(PERLTOOL) $(ASM_SCRIPT) - > $*.i
        $(ASM) $(AFLAGS) -list $*.lst $*.i $(OBJ_CMD) $@
        @echo ------------------------------------------------------------------

{$(QUALCOMM_DIR)}.s.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) -E $(APP_INC) $(DMSS_AFLAGS) < $< | $(PERLTOOL) $(ASM_SCRIPT) - > $*.i
        $(ASM) $(AFLAGS) -list $*.lst $*.i $(OBJ_CMD) $@
        @echo ------------------------------------------------------------------

.s{$(TARGET)}.o:
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) -E $(APP_INC) $(DMSS_AFLAGS) < $< | $(PERLTOOL) $(ASM_SCRIPT) - > $*.i
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
# Mobile object
#-------------------------------------------------------------------------------

# Invoke dirtcc.exe to include the current working directory in the mobile
# object.  The mobile object also includes the compile date and time and is
# built every time to ensure that the date and time information is accurate.

$(TARGET)\mobile.o :
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        -$(CC) $(CFLAGS) $(DMSS_CFLAGS_MOB) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Feature query
#-------------------------------------------------------------------------------

# Update the feature.c and feature.h files with any new features in target
# cust*.h feature definition header file.

#feature.h : $(CUSTFILE)
#        @echo ------------------------------------------------------------------
#        @echo OBJECT $(@F)
#        -perl $(QUALCOMM_DIR)\fqupdate.pl $(CUSTFILE)
#        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Boot code
#-------------------------------------------------------------------------------

# Compile the boot code with no compiler optimization to avoid insertion of
# stack push and pop instructions in functions that are called before target
# RAM has been initialized.

BOOT_OBJS = $(TARGET)\boot_trap.o \
            $(TARGET)\bootdata.o \
            $(TARGET)\boothw.o \
            $(TARGET)\bootmem.o \
            $(TARGET)\dloadarm.o \
            $(TARGET)\dloaduart.o

$(BOOT_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS0) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# PRL object
#-------------------------------------------------------------------------------

# Compile the PRL object without optimization to avoid an ARM SDT 2.50 optimizer
# problem.

$(TARGET)\prl.o : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS0) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Debug Message Processing Files
#-------------------------------------------------------------------------------

MSG_OBJS = $(TARGET)\err.o \
           $(TARGET)\log.o

$(MSG_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_NONE) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Clock Files requiring no MSG level.
#-------------------------------------------------------------------------------

MSG_OBJS = $(TARGET)\clkjul.o \
           $(TARGET)\clkm2p.o \
           $(TARGET)\clkregim.o

$(MSG_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_NONE) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# DEINTERLEAVER AND DECODER UNIT
#-------------------------------------------------------------------------------

DEC_OBJS = $(TARGET)\dec5000.o \
           $(TARGET)\deint.o

$(DEC_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_DEC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# ENCODER
#-------------------------------------------------------------------------------

ENC_OBJS = $(TARGET)\enc.o

$(ENC_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_ENC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Analog call processing
#-------------------------------------------------------------------------------

ACP_OBJS = $(TARGET)\acp553.o \
           $(TARGET)\acpbch.o \
           $(TARGET)\acpfm.o \
           $(TARGET)\acpmc.o \
           $(TARGET)\acpmcidl.o \
           $(TARGET)\acpmcini.o \
           $(TARGET)\acpmcsa.o \
           $(TARGET)\acpmcsup.o \
           $(TARGET)\acpmcvc.o \
           $(TARGET)\acprx.o \
           $(TARGET)\acprxcmd.o \
           $(TARGET)\acprxmsg.o \
           $(TARGET)\acpsamsg.o \
           $(TARGET)\acpsasup.o \
           $(TARGET)\acptx.o \
           $(TARGET)\acptxcmd.o \
           $(TARGET)\acpvcmsg.o \
           $(TARGET)\acpvcsup.o

$(ACP_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_ACP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# CDMA call processing
#-------------------------------------------------------------------------------

MC_OBJS = $(TARGET)\caix.o \
          $(TARGET)\mc.o \
          $(TARGET)\mccccl.o \
          $(TARGET)\mccdma.o \
          $(TARGET)\mccidl.o \
          $(TARGET)\mccini.o \
          $(TARGET)\mccreg.o \
          $(TARGET)\mccrx.o \
          $(TARGET)\mccrxtx.o \
          $(TARGET)\mccsa.o \
		  $(TARGET)\mccso.o \
          $(TARGET)\mccscm.o \
          $(TARGET)\mccsrch.o \
          $(TARGET)\mccsup.o \
          $(TARGET)\mccsyobj.o \
          $(TARGET)\mcctc.o \
          $(TARGET)\mcctcho.o \
          $(TARGET)\mcctcsup.o \
		  $(TARGET)\mcnvdf.o \
          $(TARGET)\mcscript.o \
          $(TARGET)\mcsysci.o \
          $(TARGET)\mcsyspr.o \
          $(TARGET)\mcsyspra.o \
          $(TARGET)\mcsysprd.o \
          $(TARGET)\mcsyssup.o \
          $(TARGET)\otasp.o \
          $(TARGET)\otaspx.o \
          $(TARGET)\parm.o \
          $(TARGET)\snm.o \
          $(TARGET)\tmsi.o

$(MC_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_MC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Receive and transmit call processing
#-------------------------------------------------------------------------------

RXTX_OBJS = $(TARGET)\rx.o \
#            $(TARGET)\rxc.o \
            $(TARGET)\rxtx.o \
            $(TARGET)\tx.o \
#            $(TARGET)\txc.o
  			$(TARGET)\rxccommon.o \
			$(TARGET)\rxcdemux.o \
			$(TARGET)\rxcpaging.o \
			$(TARGET)\rxcrda.o \
			$(TARGET)\rxcsync.o \
			$(TARGET)\rxctraffic.o \
			$(TARGET)\txcaccess.o \
			$(TARGET)\txccommon.o \
			$(TARGET)\txcmux.o \
			$(TARGET)\txcso.o \
			$(TARGET)\txctraffic.o \
			$(TARGET)\rfmsm.o


$(RXTX_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_RXTX) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Data services
#-------------------------------------------------------------------------------

DS_OBJS = $(TARGET)\ds_snoop.o \
          $(TARGET)\dsatcop.o \
          $(TARGET)\dsatdat.o \
          $(TARGET)\dsatps.o \
          $(TARGET)\dsatsio.o \
          $(TARGET)\dsctl.o \
          $(TARGET)\dsm.o \
          $(TARGET)\dsmgr.o \
          $(TARGET)\dsnetmdl.o \
          $(TARGET)\dssdorm.o \
          $(TARGET)\dssicmp.o \
          $(TARGET)\dssnet.o \
          $(TARGET)\dssocfg.o \
          $(TARGET)\dssocket.o \
          $(TARGET)\dssocki.o \
          $(TARGET)\dssoctl.o \
          $(TARGET)\dsstcp.o \
          $(TARGET)\dssudp.o \
          $(TARGET)\ip.o \
          $(TARGET)\iphdr.o \
# commented out by khekim 00/04/04
#          $(TARGET)\mdrrlp.o \
          $(TARGET)\ppp.o \
          $(TARGET)\pppfsm.o \
          $(TARGET)\pppipcp.o \
          $(TARGET)\ppplcp.o \
          $(TARGET)\psctl.o \
          $(TARGET)\psmgr.o \
          $(TARGET)\psmisc.o \
		  $(TARGET)\pstimer.o \
          $(TARGET)\rlp.o \
          $(TARGET)\slhc.o \
          $(TARGET)\tcphdr.o \
          $(TARGET)\tcpin.o \
          $(TARGET)\tcpout.o \
          $(TARGET)\tcpshell.o \
          $(TARGET)\tcpsubr.o \
          $(TARGET)\tcptimer.o \
          $(TARGET)\udp.o

$(DS_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_DS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# NV driver
#-------------------------------------------------------------------------------

NV_OBJS = $(TARGET)\nvim.o \
          $(TARGET)\nvimnv.o \
          $(TARGET)\nvimio.o \
          $(TARGET)\nvimr.o \
          $(TARGET)\nvimw.o

$(NV_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_NV) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Embedded File System
#-------------------------------------------------------------------------------

FS_OBJS = $(TARGET)\fs.o \
          $(TARGET)\fs_alloc.o \
          $(TARGET)\fs_am.o \
          $(TARGET)\fs_dev.o \
          $(TARGET)\fs_dir.o \
          $(TARGET)\fs_mpoint.o \
          $(TARGET)\fs_ops.o \
!if "$(EFS_TYPE)" == "Q_TYPE"		#EFS of qualcomm original way
          $(TARGET)\fsd_intel.o \
!endif
          $(TARGET)\fs_udir.o

# start by jnlee 00.03.02 for 'interwork option'
$(FS_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
!if "$(EFS_TYPE)" == "Q_TYPE"		#EFS of qualcomm original way
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_NV) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
!else
        $(CC) $(INTRWORK_CFLAGS) $(DMSS_CFLAGS_NV) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
!endif
        @echo ------------------------------------------------------------------

SRC_DIR = $(QUALCOMM_DIR)
# end by jnlee 

#-------------------------------------------------------------------------------
# SIO
#-------------------------------------------------------------------------------

SIO_OBJS = $(TARGET)\siors232.o \
           $(TARGET)\sio.o \
           $(TARGET)\sdevmap.o

$(SIO_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SIO) $(IRDA_INC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Sound
#-------------------------------------------------------------------------------

SND_OBJS = $(TARGET)\bbsndtab.o \
           $(TARGET)\snd.o \
           $(TARGET)\sndhwg2.o \
           $(TARGET)\snditab.o

$(SND_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SND) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Searcher
#-------------------------------------------------------------------------------

SRC_DIR = $(QUALCOMM_DIR)

SRCH_OBJS = $(TARGET)\dmdimage.o \
            $(TARGET)\dmddown.o \
            $(TARGET)\dmod.o \
            $(TARGET)\pnmask.o \
            $(TARGET)\sleep.o \
            $(TARGET)\srch.o \
            $(TARGET)\srchaq.o \
            $(TARGET)\srchcd.o \
            $(TARGET)\srchdrv.o \
            $(TARGET)\srchdz.o \
            $(TARGET)\srchint.o \
            $(TARGET)\srchpc.o \
            $(TARGET)\srchsc.o \
            $(TARGET)\srchsl.o \
            $(TARGET)\srchst.o \
            $(TARGET)\srchtc.o \
            $(TARGET)\srchtri.o \
            $(TARGET)\srchun.o \
            $(TARGET)\srchzz.o \
            $(TARGET)\ulpn.o

$(SRCH_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SRCH) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# User interface
#-------------------------------------------------------------------------------

#UI_OBJS = $(TARGET)\uixcm.o

#UI_OBJS = $(TARGET)\ui.o \
#          $(TARGET)\uihcbt.o \
#          $(TARGET)\uihcmd.o \
#         $(TARGET)\uihkey.o \
#          $(TARGET)\uihsig.o \
#          $(TARGET)\uisalph.o \
#          $(TARGET)\uiscall.o \
#          $(TARGET)\uiscli.o \
#          $(TARGET)\uiscode.o \
#          $(TARGET)\uishelp.o \
#          $(TARGET)\uisidle.o \
#          $(TARGET)\uisinfo.o \
#          $(TARGET)\uislist.o \
#          $(TARGET)\uislock.o \
#          $(TARGET)\uislpm.o \
#          $(TARGET)\uismenu.o \
#          $(TARGET)\uismsg.o \
#          $(TARGET)\uisnum.o \
#          $(TARGET)\uisoff.o \
#          $(TARGET)\uisrcl.o \
#          $(TARGET)\uisserv.o \
## reo     $(TARGET)\uissms.o \
#          $(TARGET)\uisstrt.o \
#          $(TARGET)\uissto.o \
#          $(TARGET)\uistate.o \
#          $(TARGET)\uisview.o \
#          $(TARGET)\uiudata.o \
## hjcho   $(TARGET)\uiumenu.o \
## hjcho   $(TARGET)\uiusmsd.o \
## hjcho   $(TARGET)\uiusmsl.o \
# hjcho   $(TARGET)\uiutstmn.o \
#          $(TARGET)\uiutxt.o \
#          $(TARGET)\uixcm.o \
#          $(TARGET)\uixnv.o \
#          $(TARGET)\uixscrn.o \
## hjcho    $(TARGET)\uixsnd.o \
#          $(TARGET)\uixuasms.o
#
#$(UI_OBJS) : $(SRC_FILE)
#        @echo ------------------------------------------------------------------
#        @echo OBJECT $(@F)
#        $(CC) $(CFLAGS) $(DMSS_CFLAGS_UI) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Call Manager
#-------------------------------------------------------------------------------
SRC_DIR = $(QUALCOMM_DIR)
CM_OBJS = $(TARGET)\cm.o \
          $(TARGET)\cmcall.o \
          $(TARGET)\cmclient.o \
          $(TARGET)\cmd.o \
          $(TARGET)\cminband.o \
          $(TARGET)\cmnv.o \
          $(TARGET)\cmph.o \
          $(TARGET)\cmsms.o \
          $(TARGET)\cmss.o \
          $(TARGET)\uasms.o \
          $(TARGET)\uasmsx.o \
          $(TARGET)\uasmsu.o

$(CM_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_CM) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Vocoding
#-------------------------------------------------------------------------------

VOC_OBJS = $(TARGET)\vocdown.o \
           $(TARGET)\vocimags.o \
           $(TARGET)\vocm2.o \
           $(TARGET)\vocmux.o \
           $(TARGET)\vocsup.o

$(VOC_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_VOC) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# AUDIO
#-------------------------------------------------------------------------------

SRC_DIR = Source\Audio

AUDIO_OBJS = $(TARGET)\OKI_M7732.o \
			 $(TARGET)\YMU759.o   \
			 $(TARGET)\MusicDB_B1.o

$(AUDIO_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_UI) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# FLASH
#-------------------------------------------------------------------------------

SRC_DIR = Source\Flash

FS_DEV_OBJS = $(TARGET)\fs_dev_s.o

$(FS_DEV_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(ARMCC) $(INTRWORK_CFLAGS0) $(DMSS_CFLAGS_NONE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


#-------------------------------------------------------------------------------
# FLASH                             자동응답 관련             2001. 1. 17  hjcho
#-------------------------------------------------------------------------------

SRC_DIR = Source\Flash

FILESYS_OBJS = $(TARGET)\filesysB1.o

$(FILESYS_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_UI) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------



SRC_DIR = Source\SMEMORY

SMEMORY_OBJS = $(TARGET)\Shared.o

$(SMEMORY_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

SRC_DIR = Source\RF

SKTT_RF_OBJS = $(TARGET)\Newcal.o

$(SKTT_RF_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------


# khekim start 00.01.31
#-------------------------------------------------------------------------------
# SKT-specific Data Service     by khekim
#-------------------------------------------------------------------------------
#
#SRC_DIR = Source\DS
#
#DSSKT_OBJS = $(TARGET)\dsskt.o \
#             $(TARGET)\dsatskt.o
#
#$(DSSKT_OBJS) : $(SRC_FILE)
#        @echo ------------------------------------------------------------------
#        @echo OBJECT $(@F)
#        $(CC) $(CFLAGS) $(DMSS_CFLAGS_DS) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
#        @echo ------------------------------------------------------------------
# khekim end 00.01.31

#khekim 00/02/07 start
#-------------------------------------------------------------------------------
# SKT-specific mdrrlp.c     by khekim
#-------------------------------------------------------------------------------
SRC_DIR = $(QUALCOMM_DIR)
RLP_OBJ = $(TARGET)\mdrrlp.o

$(RLP_OBJ) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_RLP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
#khekim 00/02/07 end

!IFDEF SMB
# tasik 00.07.25 : Smart Battery(Gas Guage)
#-------------------------------------------------------------------------------
# Smart Battery(Gas Guage)
#-------------------------------------------------------------------------------
SRC_DIR = Source\smb_b1
SMB_OBJS= $(TARGET)\smbdev.o \
   		  $(TARGET)\smb.o \
   		  $(TARGET)\smbtest.o

$(SMB_OBJS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_SMB) $(MSG_LVL_MED) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
!ENDIF



!IFDEF WAP2
#-------------------------------------------------------------------------------
# WAP4.8
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# SOURCE / WAP /
#-------------------------------------------------------------------------------

SRC_DIR = Source\WAP4.8
WAP_ROOT = $(TARGET)\wapaddress.o \
		   $(TARGET)\wapcommon.o \

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
WAP_LOGGING = $(TARGET)\waplog.o

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
WAP_SOURCE = $(TARGET)\alert.o \
             $(TARGET)\buffmgt.o \
			 $(TARGET)\cache.o \
			 $(TARGET)\capiclnt.o \
			 $(TARGET)\capicrpt.o \
			 $(TARGET)\capimem.o \
			 $(TARGET)\capimmi.o \
			 $(TARGET)\capipush.o \
			 $(TARGET)\capiudp.o \
			 $(TARGET)\capiwta.o \
			 $(TARGET)\chpars.o \
			 $(TARGET)\cmmnrsrc.o \
			 $(TARGET)\connectn.o \
			 $(TARGET)\connmgr.o \
			 $(TARGET)\convert.o \
			 $(TARGET)\crypto.o \
			 $(TARGET)\ctypes.o \
			 $(TARGET)\dbfunct.o \
#            $(TARGET)\env.o \
             $(TARGET)\glblDecl.o \
			 $(TARGET)\handshk.o \
			 $(TARGET)\hdrutil.o \
			 $(TARGET)\header.o \
			 $(TARGET)\history.o \
			 $(TARGET)\keyexchg.o \
			 $(TARGET)\logging.o \
			 $(TARGET)\mgtent.o \
			 $(TARGET)\mk_cpu.o \
			 $(TARGET)\mk_main.o \
			 $(TARGET)\mk_outp.o \
			 $(TARGET)\mk_queu.o \
			 $(TARGET)\mk_sche.o \
			 $(TARGET)\mk_user.o \
			 $(TARGET)\ml_buf.o \
			 $(TARGET)\ml_err.o \
			 $(TARGET)\ml_mem.o \
#            $(TARGET)\msgqueue.o \
             $(TARGET)\p_codec.o \
			 $(TARGET)\p_contif.o \
			 $(TARGET)\p_cotok.o \
			 $(TARGET)\p_sidec.o \
			 $(TARGET)\p_sitok.o \
			 $(TARGET)\p_sldec.o \
			 $(TARGET)\p_sltok.o \
			 $(TARGET)\pdubuf.o \
			 $(TARGET)\prov_dec.o \
			 $(TARGET)\provif.o \
			 $(TARGET)\push.o \
			 $(TARGET)\pushelm.o \
			 $(TARGET)\pushll.o \
			 $(TARGET)\pushpckg.o \
			 $(TARGET)\record.o \
			 $(TARGET)\reph.o \
			 $(TARGET)\rephdr.o \
			 $(TARGET)\repll.o \
			 $(TARGET)\sctpred.o \
			 $(TARGET)\si_bpi.o \
			 $(TARGET)\si_crpto.o \
			 $(TARGET)\si_float.o \
			 $(TARGET)\si_frame.o \
			 $(TARGET)\si_fs.o \
			 $(TARGET)\si_instr.o \
			 $(TARGET)\si_int.o \
			 $(TARGET)\si_libs.o \
			 $(TARGET)\si_list.o \
			 $(TARGET)\si_misc.o \
			 $(TARGET)\si_ops.o \
			 $(TARGET)\si_re.o \
			 $(TARGET)\si_sdl.o \
			 $(TARGET)\si_si.o \
			 $(TARGET)\si_sq.o \
			 $(TARGET)\si_var.o \
			 $(TARGET)\si_varop.o \
			 $(TARGET)\si_wtaia.o \
			 $(TARGET)\si_wtaic.o \
			 $(TARGET)\si_wtaig.o \
			 $(TARGET)\sptp_tls.o \
			 $(TARGET)\stackmgr.o \
			 $(TARGET)\storage.o \
			 $(TARGET)\trnscode.o \
			 $(TARGET)\url.o \
			 $(TARGET)\wae.o \
			 $(TARGET)\wae_cmmn.o \
			 $(TARGET)\wae_push.o \
			 $(TARGET)\wae_ua.o \
			 $(TARGET)\wae_wsp.o \
			 $(TARGET)\waectype.o \
			 $(TARGET)\waedef.o \
			 $(TARGET)\waepckg.o \
			 $(TARGET)\waewtaif.o \
			 $(TARGET)\wap.o \
			 $(TARGET)\wapcvt.o \
			 $(TARGET)\wbxmldec.o \
			 $(TARGET)\wbxmlelm.o \
			 $(TARGET)\wdp.o \
			 $(TARGET)\wdp_udp.o \
			 $(TARGET)\wdp_ul.o \
			 $(TARGET)\wdppckg.o \
			 $(TARGET)\wdpsuppt.o \
			 $(TARGET)\wipdb.o \
			 $(TARGET)\wiptimer.o \
			 $(TARGET)\wml_mmi.o \
			 $(TARGET)\wml_uafn.o \
			 $(TARGET)\wmlelm.o \
			 $(TARGET)\wmlevent.o \
			 $(TARGET)\wmlif.o \
			 $(TARGET)\wmlparse.o \
			 $(TARGET)\wmltoken.o \
			 $(TARGET)\wsp.o \
#            $(TARGET)\wspcapa.o \
             $(TARGET)\wspclwdp.o \
			 $(TARGET)\wspcmwtp.o \
			 $(TARGET)\wspif.o \
			 $(TARGET)\wspprocl.o \
			 $(TARGET)\wspprocm.o \
			 $(TARGET)\wta_ua.o \
			 $(TARGET)\wtachdec.o \
			 $(TARGET)\wtachelm.o \
			 $(TARGET)\wtachtok.o \
			 $(TARGET)\wtai_if.o \
			 $(TARGET)\wtapckg.o \
			 $(TARGET)\wtls.o \
			 $(TARGET)\wtls_wdp.o \
			 $(TARGET)\wtlserr.o \
			 $(TARGET)\wtlsmain.o \
			 $(TARGET)\wtlspckg.o \
			 $(TARGET)\wtp_wdp.o \
			 $(TARGET)\wtppckg.o \
			 $(TARGET)\wtpprvdr.o \
			 $(TARGET)\wtpsar.o \
			 $(TARGET)\wtpsuppt.o \
			 $(TARGET)\xml_conv.o \

$(WAP_SOURCE) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
              
#-------------------------------------------------------------------------------
# SOURCE / WAP / CORE / OPTIONAL / CHARSET
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\core\optional\charset
WAP_OPTIONAL_CHARSET = $(TARGET)\charset.o \
				       $(TARGET)\HCodeCnvt.o

$(WAP_OPTIONAL_CHARSET) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / CORE / OPTIONAL / MEMORY
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\core\optional\memory
WAP_OPTIONAL_MEMORY = $(TARGET)\wip_mem.o

$(WAP_OPTIONAL_MEMORY) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / CLNT
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\CLNT
WAP_CLNT = $(TARGET)\clntadapter.o \
		   $(TARGET)\clntapp.o \
		   $(TARGET)\clntPIM.o \
		   $(TARGET)\clntCRYPT.o \
		   $(TARGET)\clntwallet.o

$(WAP_CLNT) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / MEM
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\MEM
WAP_MEM = $(TARGET)\aapimem.o 
#		  $(TARGET)\memadapter.o

$(WAP_MEM) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / MMI
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\MMI
WAP_MMI = $(TARGET)\mmiadapter.o \
		  $(TARGET)\mmicmd.o \
		  $(TARGET)\GenDsp.o \
		  $(TARGET)\mmi_adapter.o \
		  $(TARGET)\mmi_connector.o \
		  $(TARGET)\mmi_wml.o \
		  $(TARGET)\mmi_option.o \
		  $(TARGET)\mmi_keypad.o \
		  $(TARGET)\mmi_wap_menu.o \
		  $(TARGET)\mmi_dialog.o \
!IFDEF CHARACTER_BELL
		  $(TARGET)\mmi_musicbell.o \
          $(TARGET)\mmi_characbell.o
!ELSE
		  $(TARGET)\mmi_musicbell.o 
!ENDIF

$(WAP_MMI) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS0) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

WAP_MMI_SIS = SOURCE\WAP\SKY\MMI\sisdecoder.o

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / PUSH
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\PUSH
WAP_PUSH = $(TARGET)\aapipush.o

$(WAP_PUSH) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / SECURE
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\SECURE
WAP_SECURE =$(TARGET)\aapisecure.o \
			$(TARGET)\alertsec.o \
			$(TARGET)\bitt.o \
			$(TARGET)\bn_add.o \
			$(TARGET)\bn_div.o \
			$(TARGET)\bn_exp.o \
			$(TARGET)\bn_gcd.o \
			$(TARGET)\bn_lib.o \
			$(TARGET)\bn_mont.o \
			$(TARGET)\bn_mul.o \
			$(TARGET)\bn_mulw.o \
			$(TARGET)\bn_print.o \
			$(TARGET)\bn_shift.o \
			$(TARGET)\bn_sqr.o \
			$(TARGET)\bn_sub.o \
			$(TARGET)\cipher.o \
			$(TARGET)\cssls.o \
			$(TARGET)\keygen.o \
			$(TARGET)\md_rand.o \
			$(TARGET)\md5.o \
			$(TARGET)\md5_lib.o \
			$(TARGET)\rand.o \
			$(TARGET)\rec.o \
			$(TARGET)\rsa_lib.o \
			$(TARGET)\Seedx.o

$(WAP_SECURE) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / EIF
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\EIF
WAP_EIF = $(TARGET)\eifwap.o
				
$(WAP_EIF) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / SMS
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\SMS
WAP_SMS = $(TARGET)\aapisms.o
				
$(WAP_SMS) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / USSD
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\USSD
WAP_USSD = $(TARGET)\aapiussd.o
				
$(WAP_USSD) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------
 
#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / WTA
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\WTA
WAP_WTA = $(TARGET)\aapiwta.o \
          $(TARGET)\aapiwtai.o \
			   
$(WAP_WTA) : $(SRC_FILE)
        @echo ------------------------------------------------------------------
        @echo OBJECT $(@F)
        $(CC) $(CFLAGS) $(DMSS_CFLAGS_WAP) $(MSG_FILE) $(OBJ_FILE) $(SRC_FILE)
        @echo ------------------------------------------------------------------

#-------------------------------------------------------------------------------
# SOURCE / WAP / SKY / WD
#-------------------------------------------------------------------------------

SRC_DIR = SOURCE\WAP4.8\SKY\WD
WAP_WD = $(TARGET)\aapiwd.o
			   
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

#-------------------------------------------------------------------------------
# include common object for EQUS and SKY
#-------------------------------------------------------------------------------

include "common.inc"

#===============================================================================
#                               DEPENDENCIES
#===============================================================================

# The dependencies listed at the end of this makefile can be automatically
# updated by making the 'depend' target to invoke the following rules.

#MAKEFILE_NAME   = $(TARGET_NAME).mak
#MAKEFILE_BACKUP = $(TARGET_NAME).bak

MAKEFILE_NAME   = $(MAKE_TARGET).mak
MAKEFILE_BACKUP = $(MAKE_TARGET).bak

.SUFFIXES: .s .o .c .dep

GETDEP_SCRIPT  = $(QUALCOMM_DIR)\getdep.pl
MDEPEND_SCRIPT = $(QUALCOMM_DIR)\mdepend.pl

# khekim 00/04/04 start
{Source\DS}.c{$(TARGET)}.dep:
        $(CC) $(CFLAGS) -ISource\DS $(DMSS_CFLAGS) -E < $< | \
          $(PERLTOOL) $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@
# khekim 00/04/04 end

# msrock 00/04/19 start
{Source\Flash}.c{$(TARGET)}.dep:
        $(CC) $(CFLAGS) -ISource\Flash $(DMSS_CFLAGS) -E < $< | \
          $(PERLTOOL) $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@
# msrock 00/04/19 end

!IFDEF IRDA
# IrDA LIB & interface
{$(IRDA_DIR)}.c{$(TARGET)}.dep:
        $(CC) $(CFLAGS) -I$(IRDA_DIR)   $(DMSS_CFLAGS) -E < $< | \
          $(PERLTOOL) $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

{$(IRDA_IRUTY)}.c{$(TARGET)}.dep:
        $(CC) $(CFLAGS) -I$(IRDA_IRUTY) $(DMSS_CFLAGS) -E < $< | \
          $(PERLTOOL) $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

{$(IRDA_IRSIR)}.c{$(TARGET)}.dep:
        $(CC) $(CFLAGS) -I$(IRDA_IRSIR) $(DMSS_CFLAGS) -E < $< | \
          $(PERLTOOL) $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

{$(IRDA_PROTO)}.c{$(TARGET)}.dep:
        $(CC) $(CFLAGS) -I$(IRDA_PROTO) $(DMSS_CFLAGS) -E < $< | \
          $(PERLTOOL) $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@
!ENDIF

{$(QUALCOMM_DIR)}.c{$(TARGET)}.dep:
        $(CC) $(CFLAGS) $(DMSS_CFLAGS) -E < $< | \
          $(PERLTOOL) $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

{$(QUALCOMM_DIR)}.s{$(TARGET)}.dep:
        $(CC) $(CFLAGS) $(DMSS_AFLAGS) $(DMSS_CFLAGS) -E < $< | \
          $(PERLTOOL) $(GETDEP_SCRIPT) $*.o $< > $*.de_
        -del $@
        move $*.de_ $@

depend: $(TARGET)\exist $(OBJECTS:.o=.dep) force.frc
        $(PERLTOOL) $(MDEPEND_SCRIPT) $(MAKEFILE_NAME) $(TARGET) > makefile.tmp
        -del $(MAKEFILE_BACKUP)
        move $(MAKEFILE_NAME) $(MAKEFILE_BACKUP)
        move makefile.tmp $(MAKEFILE_NAME)

force.frc:

# ------------------------------
# DO NOT EDIT BELOW THIS LINE

# End of auto generated dependencies.
