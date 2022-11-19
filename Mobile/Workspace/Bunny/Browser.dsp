# Microsoft Developer Studio Project File - Name="Browser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Browser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Browser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Browser.mak" CFG="Browser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Browser - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Browser - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/B2/Mobile/Workspace/Bunny", ZSRHAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Browser - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Browser - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\SOURCE\SKY\B2\INCLUDE" /I "..\..\SOURCE\SKY\B2\APPLET\INCLUDE" /I "..\..\SOURCE\SKY\B2\PLATFORM\WIN32" /I "..\..\SOURCE\SKY\B2\PLATFORM\REX" /I "..\..\SOURCE\SKY\B2\DEVICE\INCLUDE" /I "..\..\SOURCE\SKY\B2\RESOURCE\STRING" /I "..\..\SOURCE\SKY\B2\RESOURCE\IMAGE\ANI" /I "..\..\SOURCE\SKY\B2\RESOURCE\IMAGE\BITMAP" /I "..\..\SOURCE\SKY\B2\RESOURCE\IMAGE\ICON" /I "..\..\SOURCE\EQUS\INCLUDE" /I "..\..\SOURCE\EQUS\CONTROL\INCLUDE" /I "..\..\SOURCE\EQUS\DISPLAY\FONT" /I "..\..\SOURCE\SMB_B1" /I "..\..\SOURCE\EQUS\RESOURCE\STRING" /I "..\..\SOURCE\EQUS\RESOURCE\IMAGE\BITMAP" /I "..\..\SOURCE\EQUS\PLATFORM\WIN32" /I "..\..\SOURCE\EQUS\PLATFORM\REX" /I "..\..\SOURCE\AUDIO" /I "..\..\SOURCE\DS" /I "..\..\SOURCE\MODELDEP" /I "..\..\SOURCE\qualcomm30" /I "..\..\SOURCE\sounder" /I "..\..\SOURCE\SOCKAPI" /I "..\..\SOURCE\Irda" /I "..\..\SOURCE\Irda\lib\include" /I "..\..\SOURCE\Irda\lib\dcinc" /I "..\..\SOURCE\WAP_B1\CORE\INCLUDE" /I "..\..\SOURCE\WAP_B1\REX\MMI" /I "..\..\SOURCE\WAP_B1\REX\EIF" /I "..\..\SOURCE\WAP_B1\REX\CLNT" /I "..\..\SOURCE\WAP_B1\CORE\source" /I "..\..\SOURCE\WAP_B1\CORE\optional\charset" /I "..\..\SOURCE\WAP_B1\CORE\optional\memory" /I "..\..\SOURCE\WAP_B1" /I "..\..\SOURCE\flash" /I "..\..\include" /D "T_ARM" /D "T_SURF" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WAPPER" /D "WAP_NEO" /D "WAP_USE_PZID2" /D "WAP_USE_SKT_NEW" /D "WAP_USE_SECURE" /D "WAP_USE_DORM" /D "MAKE_IRDA" /D MODEL_ID=MODEL_B2 /D NV_TYPE=EFS /D MMI_TYPE=SKY_UI /D SKY_MODEL_ID=SKY_MODEL_B2 /D MODEL_DEP_H=\"Bunny.h\" /D CUST_H=\"custb2.h\" /D TG=T_G /D "_FOR_BROWSER" /D MSG_FILE=\"MSG\" /D MSG_LEVEL=MSG_LVL_FATAL /D "T_MSM3" /D "CONFIG_EQS_COLOR_256" /D "T_MSM5000" /D "WAP_B2" /D SOCKAPI=USE_SOCKET /D EMAIL=USE_EMAIL /D IRDA=USE_IRDA /D GVM=USE_GVM /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Browser - Win32 Release"
# Name "Browser - Win32 Debug"
# Begin Group "PEGASUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\PEGASUS\temp4build.c
# End Source File
# End Group
# Begin Group "AUDIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Audio\AudioTest.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Audio\MusicDB_B1.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Audio\MusicDB_B1.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Audio\OKI_M7732.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Audio\OKI_M7732.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Audio\YMU759.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Audio\YMU759.h
# End Source File
# End Group
# Begin Group "INCLUDE"

# PROP Default_Filter ""
# End Group
# Begin Group "SKY"

# PROP Default_Filter ""
# Begin Group "DEVICE"

# PROP Default_Filter ""
# Begin Group "INC_DEV"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\INCLUDE\eqsqlcd_B1.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\INCLUDE\ID_LCD_B1.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\INCLUDE\idlcd_b1.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\INCLUDE\LCD_B1.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\INCLUDE\LCD_IFB1.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\INCLUDE\sky_idlcd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\INCLUDE\sky_lcd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\INCLUDE\sky_slcd.h
# End Source File
# End Group
# Begin Group "LCD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\LCD\sky_lcd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\DEVICE\LCD\sky_slcd.c
# End Source File
# End Group
# End Group
# Begin Group "INC_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_all.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_annu.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_brkr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_cm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_conf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_data.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_def.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_efs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_evnt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_feat.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_file.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_hcbt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_hcmd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_hkey.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_hs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_hsig.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_img.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_irda.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_mtxt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_mus.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_nv.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_rex.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_scrn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_snd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Include\sky_sock.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_tokn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_ui.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\INCLUDE\sky_util.h
# End Source File
# End Group
# Begin Group "APPLET"

# PROP Default_Filter ""
# Begin Group "SYS_APP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SYSTEM\ska_call.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SYSTEM\ska_clid.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SYSTEM\ska_code.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SYSTEM\ska_idle.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SYSTEM\ska_nam.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SYSTEM\ska_off.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SYSTEM\ska_strt.c
# End Source File
# End Group
# Begin Group "INC_APP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_011.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_2get.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_aans.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_aapi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_add.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_alrm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_bapi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_bell.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_bstr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_butl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_calc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_call.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_cam.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_cbell.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_clid.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_clst.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_code.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_crec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_dmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_dscf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_dsvc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_efs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_eif.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_game.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_gvm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_hdon.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_idle.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_info.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_init.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_irda.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_irph.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_ligt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_loca.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_lock.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_mail.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_memo.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_menu.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_mo.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_mwlt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_mybe.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_nam.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_NAMs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_ntop.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_off.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_pbo.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_pic.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_pop3.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_sbel.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_scal.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_schl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_self.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_sfil.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_shld.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_skvm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_slst.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_sms.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_smsl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_smtp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_stop.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_strt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_test.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_thme.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_tnum.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_vcmm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_vcore.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_ver.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\Applet\Include\ska_vm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_vmemo.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_vms.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_volm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_wap.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_WAPAPP.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCE\SKY\B2\APPLET\INCLUDE\ska_wav.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\INCLUDE\ska_wtim.h
# End Source File
# End Group
# Begin Group "PBOOK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\PBOOK\ska_bapi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\PBOOK\ska_bstr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\PBOOK\ska_butl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\PBOOK\ska_pbo.c
# End Source File
# End Group
# Begin Group "MENU"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\MENU\ska_menu.c
# End Source File
# End Group
# Begin Group "AUTOANS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\AUTOANS\ska_aans.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\AUTOANS\ska_aapi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\AUTOANS\ska_crec.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\AUTOANS\ska_hdon.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\AUTOANS\ska_ment.c
# End Source File
# End Group
# Begin Group "CALC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\CALC\eqa_calc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\CALC\eqa_calc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\CALC\ska_calc.c
# End Source File
# End Group
# Begin Group "EMAIL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\EMAIL\ska_mail.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\EMAIL\ska_mbox.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\EMAIL\ska_pop3.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\EMAIL\ska_smtp.c
# End Source File
# End Group
# Begin Group "GAME"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GAME\ska_game.c
# End Source File
# End Group
# Begin Group "IrDA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\IrDA_B2.tbl
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\ska_ftrs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\ska_iani.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\ska_ibok.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\ska_imel.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\ska_ipht.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\ska_irda.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\ska_irpc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\IRDA\ska_irph.c
# End Source File
# End Group
# Begin Group "TEST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\AudioTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\BatteryTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\BlightTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\Cameratest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\CtrlTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\IrDATest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\LcdTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\McTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\MelodyTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\Rftest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\RingTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\RssiTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\ska_efs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\ska_test.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\sky_temp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\testgrph.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\TestMenu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\VolumeTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\TEST\VolumeUpDTest.c
# End Source File
# End Group
# Begin Group "SETTING"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_011.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_2get.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_add.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_info.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_init.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_ligt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_lock.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_NAMs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_ntop.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_self.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_thme.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_tnum.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_ver.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SETTING\ska_volm.c
# End Source File
# End Group
# Begin Group "SCHEDULE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SCHEDULE\ska_sbel.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SCHEDULE\ska_scal.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SCHEDULE\ska_schl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SCHEDULE\ska_sfil.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SCHEDULE\ska_shld.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SCHEDULE\ska_slst.c
# End Source File
# End Group
# Begin Group "STOPWATCH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SWATCH\ska_stop.c
# End Source File
# End Group
# Begin Group "SMS"

# PROP Default_Filter ""
# Begin Group "VOCODER"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\VOCODER\ska_vcmm.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SMS\ska_loca.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SMS\ska_mo.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SMS\ska_mybe.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SMS\ska_sms.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SMS\ska_smsl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SMS\ska_vms.c
# End Source File
# End Group
# Begin Group "WORLDTIME"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\WORLDTIME\ska_wtim.c
# End Source File
# End Group
# Begin Group "BELL_VIB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\BELL_VIB\ska_bell.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\BELL_VIB\ska_mbel.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\BELL_VIB\ska_warn.c
# End Source File
# End Group
# Begin Group "DATACFG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\DATACFG\ska_dscf.c
# End Source File
# End Group
# Begin Group "PICTURE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Picture\ska_mani.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\PICTURE\ska_pic.c
# End Source File
# End Group
# Begin Group "ALARM"

# PROP Default_Filter ""
# Begin Group "MEMO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\MEMO\ska_memo.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\ALARM\ska_alrm.c
# End Source File
# End Group
# Begin Group "DATAMGR"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\DATAMGR\ska_dmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\DATAMGR\ska_dsvc.c
# End Source File
# End Group
# Begin Group "EIF"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\EIF\ska_eif.c
# End Source File
# End Group
# Begin Group "GVM"

# PROP Default_Filter ""
# Begin Group "SWAP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\gvm_config.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\Midd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\Mil.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\Mil.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\MILNet.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\SCL.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\SVM.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\SVM.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\SWAP\SWAP.h
# End Source File
# End Group
# Begin Group "ATSWAP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\ATSWAP\AtSWAP.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\ATSWAP\AtSWAP.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\GVM\ska_gvm.c
# End Source File
# End Group
# Begin Group "CAMERA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\CAMERA\ska_cam.c
# End Source File
# End Group
# Begin Group "RECPLAY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\RECPLAY\ska_vmemo.c
# End Source File
# End Group
# Begin Group "WAVELET"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\WAVELET\data.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\WAVELET\ska_wav.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Wavelet\wav_multisock.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Wavelet\wav_multisock.h
# End Source File
# End Group
# Begin Group "SJ_GAME"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Sj_games\GameSrc\Apache5.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SJ_GAMES\GLib\GExt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SJ_GAMES\GLib\GExt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SJ_GAMES\GLib\GLib.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SJ_GAMES\GLib\GLibMain.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SJ_GAMES\GLib\GLibScore.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Sj_games\GameSrc\MarsScouter3.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Sj_games\GameSrc\Puzzle3.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Sj_games\GameSrc\Raccoon3.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Sj_games\GameSrc\samtris3.c
# End Source File
# End Group
# Begin Group "WAP_APP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\WAP_APP\ska_WAPAPP.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Wap_app\ska_wapdev.c
# End Source File
# End Group
# Begin Group "VM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\VM\ska_vm.c
# End Source File
# End Group
# Begin Group "VAS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Vas\Idevnet.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Vas\ska_vas.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Vas\ska_vas.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Vas\uptypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\Applet\Vas\Wdptypes.h
# End Source File
# End Group
# Begin Group "SKVM"

# PROP Default_Filter ""
# Begin Group "XVM_XCE_OBJS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\XVM_XCE_OBJS\externlib.o
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\XVM_XCE_OBJS\mdsock.o
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\XVM_XCE_OBJS\nativelib.o
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\XVM_XCE_OBJS\romlib.o
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\XVM_XCE_OBJS\xvmlib.o
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\extern.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\graphics.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\graphics.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\mdarm_def.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\mdarm_main.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\mdarm_net.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\mdarm_skt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\mdarm_sys.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\mdarm_terminal.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\romfile.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\romfile.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\ska_skvm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\xvm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\SKVM\xvmstate.h
# End Source File
# End Group
# Begin Group "MWALLET"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\MWALLET\ska_mwlt.c
# End Source File
# End Group
# Begin Group "CALLLIST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\CALLLIST\ska_clst.c
# End Source File
# End Group
# Begin Group "CHARACBELL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\APPLET\CHARACBELL\ska_cbel.c
# End Source File
# End Group
# End Group
# Begin Group "I18N_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\I18N\sky_tokn.c
# End Source File
# End Group
# Begin Group "RES_SKY"

# PROP Default_Filter ""
# Begin Group "STR_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\STRING\sky_kor.tkn
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\STRING\sky_usa.tkn
# End Source File
# End Group
# Begin Group "SOUND"

# PROP Default_Filter ""
# End Group
# Begin Group "IMG_SKY"

# PROP Default_Filter ""
# Begin Group "ANI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\DataService1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\DataService2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\DataService3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\girl1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\girl2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\girl3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\girl4.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_clock01.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_clock02.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_clock03.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_clock04.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_clock05.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_menu01.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_menu02.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_menu03.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_pbook01.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_pbook02.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_pbook03.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_sms01.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_sms02.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_sms03.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_sms04.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_stack01.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_stack02.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_stack03.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_stack04.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\icon_stack05.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall01.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall02.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall03.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall04.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall05.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall06.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall07.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall08.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall09.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall10.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\outcall11.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky01.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky02.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky03.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky04.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky05.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky06.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky07.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky08.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky09.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky10.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky11.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky12.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky13.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky14.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky15.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky16.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky17.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky18.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky19.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\sky20.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst01.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst02.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst03.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst04.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst05.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst06.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst07.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst08.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst09.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst10.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst11.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst12.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst13.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst14.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ANI\uconst15.ebi
# End Source File
# End Group
# Begin Group "ICON"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\bellvib0.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\bellvib1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\bellvib2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\bellvib3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\bellvib4.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\idleframe.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\pbook0.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\pbook1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\pbook2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\pbook3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\pbook4.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\schedul0.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\schedul1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\schedul2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\schedul3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\ICON\schedul4.ebi
# End Source File
# End Group
# Begin Group "BITMAP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\3209SKY1.EBI
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\alrm09x11x1xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\arrw1616.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\auto11x11x1xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\batman1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\batt.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\batt16x11x11xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\bellvib.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\blvb11x11x3xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\bootlogo.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\call.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\chicken.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\colon.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit0.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit4.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit5.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit6.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit7.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit8.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digit9.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\digitblk.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\etqt07x11x1xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\faces.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\gafield.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\gagamel.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\hair.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\idleframe.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\jrkwon.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\pasmuf.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\pig.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\pooh.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\roam.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\roam14x11x6xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\rock.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\rose.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\rssi.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\rssi14x11x6xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\rssi_s.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\runner0.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\runner1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\runner2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\runner3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\runner4.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\runner5.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\runner6.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\runner7.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\simp.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\sky_cml.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\skycml_c.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\skycml_l.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\skycml_r.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\slcd_logo84x32xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\sms.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\sms_11x11x1xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\smufpet.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\spider1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\spkr07x11x1xBW.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\startup.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\symbol_page1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\symbol_page2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\symbol_page3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\symbol_page4.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\symbol_page5.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\symbol_page6.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\symbol_page7.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\symbol_page8.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\tiger.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\tosmuf.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\totoro1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\tv.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\BITMAP\wp_cal.ebi
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\IMAGE\sky_data.c
# End Source File
# End Group
# Begin Group "MENU_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\RESOURCE\MENU\sky_mtxt.c
# End Source File
# End Group
# End Group
# Begin Group "SYSTEM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_annu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_cm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_conf.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_evnt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_file.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_hcbt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_hcmd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_hkey.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_hs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_hsig.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_irda.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_nv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_scrn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_sms.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_snd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sky\B2\System\sky_sock.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\sky_ui.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\SYSTEM\uixcm.c
# End Source File
# End Group
# Begin Group "PLATFORM"

# PROP Default_Filter ""
# Begin Group "WIN32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\PLATFORM\WIN32\skywefs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\PLATFORM\WIN32\skywefs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\PLATFORM\WIN32\skywrex.c
# End Source File
# End Group
# Begin Group "REX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\SKY\B2\PLATFORM\REX\skyrefs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\PLATFORM\REX\skyrefs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SKY\B2\PLATFORM\REX\skyrrex.c
# End Source File
# End Group
# End Group
# End Group
# Begin Group "EQUS"

# PROP Default_Filter ""
# Begin Group "CONTROL"

# PROP Default_Filter ""
# Begin Group "INC_CTRL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_aicn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_all.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_anim.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_atxt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_ckbx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Equs\Control\Include\eqc_clnd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_img.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_menu.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_msgb.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_nedt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_pbar.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_pbtn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_pmnu.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_rbtn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_scrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_spin.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_stxt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_tedt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_time.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\INCLUDE\eqc_tmpl.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_aicn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_anim.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_atxt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_ckbx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_img.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_menu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_msgb.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_nedt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_pbar.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_pbtn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_pmnu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_rbtn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_scrl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_spin.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_stxt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_tedt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_time.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\CONTROL\eqc_tmpl.c
# End Source File
# End Group
# Begin Group "DISPLAY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\EQUS\DISPLAY\eqs_dc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\DISPLAY\eqs_frmt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\DISPLAY\eqs_grph.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\DISPLAY\eqs_gui.c
# End Source File
# End Group
# Begin Group "INC_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_aapi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_all.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_dc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_err.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_evnt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_evnt.tbl
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_feat.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_file.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_frmt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_grph.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_gui.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_i18n.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_list.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_sfnt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_swin.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_timr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_tokn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_type.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\INCLUDE\eqs_util.h
# End Source File
# End Group
# Begin Group "STATE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\EQUS\STATE\eqs_aapi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\STATE\eqs_swin.c
# End Source File
# End Group
# Begin Group "I18N_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\EQUS\I18N\eqs_i18n.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\I18N\eqs_tokn.c
# End Source File
# End Group
# Begin Group "UTIL"

# PROP Default_Filter ""
# Begin Group "MEMORY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\MEMORY\au_cfvr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\MEMORY\au_cmmn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\MEMORY\au_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\MEMORY\au_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\MEMORY\au_mmry.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\MEMORY\au_rsrc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\MEMORY\au_trgt.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\eqs_list.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\eqs_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\eqs_timr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\UTIL\eqs_util.c
# End Source File
# End Group
# Begin Group "RES_EQUS"

# PROP Default_Filter ""
# Begin Group "STR_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\STRING\eqs_kor.tkn
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\STRING\eqs_usa.tkn
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\arrw1616.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\day10015.ebi
# End Source File
# Begin Source File

SOURCE="..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\day10415-1.ebi"
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\day10415.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\defcml.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\headlogo.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\iocmlcen.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\iocmllef.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\iocmlrig.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\symbol_page1.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\symbol_page2.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\symbol_page3.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\symbol_page4.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\symbol_page5.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\symbol_page6.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\symbol_page7.ebi
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\RESOURCE\IMAGE\BITMAP\symbol_page8.ebi
# End Source File
# End Group
# Begin Group "PLT_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\EQUS\PLATFORM\REX\eqsrutil.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\PLATFORM\WIN32\eqswefs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\PLATFORM\WIN32\eqswutil.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\EQUS\PLATFORM\WIN32\eqswutil.h
# End Source File
# End Group
# End Group
# Begin Group "SOCKAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Sockapi\sockapi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sockapi\sockapi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sockapi\sockctl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sockapi\sockctl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sockapi\uisockapi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sockapi\uisockapi.h
# End Source File
# End Group
# Begin Group "DS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\DS\dsatskt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\DS\dsatskt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\DS\dsmip.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\DS\dsmip.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\DS\dsskt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\DS\dsskt.h
# End Source File
# End Group
# Begin Group "QUALCOMM30"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acp553.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acp553.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpbch.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpbch.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpcmdef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpfm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpfm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpfmi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acplog.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acplog.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmci.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcidl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcidl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcini.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcini.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcsa.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcsa.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcsup.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcsup.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcvc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcvc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpmcvci.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acprx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acprx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acprxcmd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acprxcmd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acprxi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acprxmsg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acprxmsg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpsamsg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpsamsg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpsasup.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpsasup.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acptx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acptx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acptxcmd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acptxcmd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acptxi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpvcmsg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpvcmsg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpvcsup.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpvcsup.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpwb.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\acpwbm2.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\adc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\adcg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\addrdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\apwrx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\apwrx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\arm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\armasm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\assert.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\atmr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\atmr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\auth.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\auth.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\authi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bbdiagp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bbsndtab.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bbsndtab.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bbver.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bch.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bio.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bio.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\biog.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bit.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bit.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\boot_trap.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\boot_trap.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bootdata.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bootdata.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\boothw.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\boothw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bootmem.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\bootmem.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cagc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cai.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\caii.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\caix.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\caix.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cave.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cave.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\clk.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\clk.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\clki.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\clkjul.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\clkm2p.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\clkregim.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\clkregim.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmcall.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\CMCALL.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmclient.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmclient.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmdbg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\CMI.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cminband.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cminband.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmmc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\CMNV.C
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\CMNV.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmph.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\CMPH.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmsms.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cmsms.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\CMSS.C
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\CMSS.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\comdef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\crc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\crc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cust4sc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cust4sce.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cust4sp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\cust4spe.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\customer.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\custsurf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\db.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\db.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dec5000.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dec5000.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\deci.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\deint.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\deint.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dfm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dh.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dh.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diag.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diag.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diag_guid.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diag_table.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagbuf.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagbuf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagcmd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagconf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagfs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagfs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagparm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagpkt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\diagx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\distreg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\distreg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dload.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dloadarm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dloadarm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dloaduart.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dloaduart.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dmddown.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dmddown.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dmdimage.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dmdimage.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dmod.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dmod.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dog.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dog.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ds.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ds_snoop.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ds_snoop.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsatcop.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsatcop.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsatcopi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsatdat.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsatps.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsatsio.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsctl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsnetmdl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsnetmdl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsretry.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlp3.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlp3.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlpdel.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlpdel.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlpi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlpi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlpnak.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlpnak.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlprsq.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlprsq.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlprtx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlprtx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlpseq.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlptxq.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrlptxq.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsrmio.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsscrm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssdns.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssdns.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssdorm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssdorm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssicmp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssicmp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssinternal.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssnet.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssnet.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssocfg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssocfg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssocket.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssocket.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssocki.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssocki.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssoctl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssoctl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsstcp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dsstcp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssudp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\dssudp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\enc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\enc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\enci.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\err.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\err.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\event.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\event.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\feature.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\feature.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ffpc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ffpc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS.C
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\fs_alloc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\fs_alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\fs_am.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_AM.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\fs_dev.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_DEV.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\Flash\fs_dev_s.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_DEVI.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\fs_dir.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_DIR.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_MPOINT.C
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_MPOINT.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_OPS.C
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_OPS.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\fs_parms.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_RAM.C
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_UDIR.C
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\FS_UDIR.H
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\fsd_intel.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\fsi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\gpsbias.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\hs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\hsg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\hsig.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\hw.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\hw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\icmp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\icmp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\iface.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\internet.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ip.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ip.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\iphdr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\lcd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\lcd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\lcdp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\lifetstg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\log.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\log.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\log_dmss.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\loopback.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\loopback.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mar.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mar.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\martable.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\martable.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccap.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccap.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccccl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccccl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccdma.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccdma.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccidl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccini.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccqpch.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccqpch.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccreg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccreg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccrx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccrx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccrxtx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccrxtx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccsa.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccsch.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccsch.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccscm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccscm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccso.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccso.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccsrch.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccsrch.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccsup.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccsyobj.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mccsyobj.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcctc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcctcho.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcctcho.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcctci.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcctcsup.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcctcsup.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcctxns.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcctxns.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mci.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcnvdf.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcnvdf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcscript.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcscript.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcsys.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcsysci.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcsyspr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcsyspr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcsyspra.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcsysprd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mcsyssup.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mctrans.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mctrans.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mctrans_cmd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\md5.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\md5c.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mdrrlp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mdrrlp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\memory.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\memtest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\misc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\misc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mobile.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mobile.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mod.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\msg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\msg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\msm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\msm50reg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\msm5_init.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\msm_drv.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\msm_help.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\msm_init.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\mux.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\netuser.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Rf\NewCal.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Rf\NewCal.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nv.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nv24lci.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nv24lcxx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nva.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nva.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvb.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvb.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvi_hand.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvim.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvim.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvimio.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvimio.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvimnv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvimr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvimr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvimw.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvimw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvmio.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvmio.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvruim.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvruimi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvsetup.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvw.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\nvw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\otasp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\otaspe.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\otaspi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\otaspx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\otaspx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\parm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\parm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\pnmask.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\pnmask.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ppp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ppp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\pppfsm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\pppfsm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\pppipcp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\pppipcp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ppplcp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ppplcp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\prl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\prl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\prli.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\processor.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ps.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\psctl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\psglobal.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\psi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\psmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\psmisc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\psmisc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\pstimer.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\pstimer.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\queue.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\queue.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\qw.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\qw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ran.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ran.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\reloc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\reloc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rex.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rex.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rexarm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Rf\RfCalData.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfcs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfcs160x.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rficap.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfifreq.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfigpio.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfmsm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfmsm_d1.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfnv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rfnv.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ring.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ring.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rlgc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rlgc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rlp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rlp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ruim.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ruim.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ruimi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ruimusr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxccommon.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxccommon.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxcdemux.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxcdemux.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxcpaging.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxcpaging.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxcrda.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxcrda.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxcsync.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxcsync.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxctraffic.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxctraffic.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxtx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\rxtx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sbi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sbi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sdbid_b1.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sdevmap.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sdevmap.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sdevmenu.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sio.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sio.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\siog2.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\siors232.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\siors232.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\siorsreg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sleep.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sleep.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\slhc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\slhc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sms.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\smsi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\smsx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\snd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\snd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sndhwg2.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sndi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sndihw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\snditab.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\snditab.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\sndring.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\snm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\snm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srch.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srch.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchaq.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchcd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchdrv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchdz.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchint.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchpc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchqpch.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchsc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchsl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchst.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchtc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchtri.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchun.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srchzz.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\srv.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\target.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\targetg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\task.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\task.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tcp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tcphdr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tcpin.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tcpout.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tcpshell.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tcpsubr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tcptimer.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tdso.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tdso.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\therm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\therm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\timetest.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tmsi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tmsi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tramp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\trampm3.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\trampm5.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ts.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ts.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\tx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txcaccess.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txcaccess.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txccommon.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txccommon.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txcmux.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txcmux.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txcso.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txcso.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txctraffic.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\txctraffic.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uapi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasms.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasms.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasmsi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasmsnv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasmsnv.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasmsu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasmsu.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasmsx.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uasmsx.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\udp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\udp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ui.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ui.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uih.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uihcbt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uihcmd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uihcmd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uihkey.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uihsig.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisalph.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiscall.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiscall.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiscli.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiscode.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uishelp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisidle.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisinfo.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uislist.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uislock.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uislpm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uismenu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uismm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uismm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uismsg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisnum.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisoff.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisrcl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisserv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uissms.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uissms.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uissto.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisstrt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uistate.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uistate.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisview.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisvm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisvm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisvmi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisvr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisvr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uisvri.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiudata.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiudata.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiuint.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiumenu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiumenu.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiusmsd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiusmsd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiusmsl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiusmsl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiutstmn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiutstmn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiutxt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiutxt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uiutxti.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixcm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixnv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixscrn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixscrn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixsnd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixsnd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixuasms.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixuasms.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixvs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixvs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\uixvsi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ulpn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\ulpn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vbatt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vbatt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\voc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\voc_core.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vocdown.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vocdown.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vocimags.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vocm2.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vocm2.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vocmux.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vocmux.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\QUALCOMM30\vocsup.c
# End Source File
# End Group
# Begin Group "SOUNDER"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\Sounder\audTest.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sounder\audTest.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sounder\melodyDb.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sounder\melodyDb.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sounder\mldtab.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sounder\mldtab.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sounder\yamaha.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\Sounder\yamaha.h
# End Source File
# End Group
# Begin Group "MCOMMERCE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\MCommerce\a_bitstr_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\a_bytes_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\a_int_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\a_object_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\a_octet_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\asn1.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\asn1_lib_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\asn1_mac.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\asn1enc_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\asn1param_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\asn1pri_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\asn1pub_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\asn1sig_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\base64.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\base64_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bio.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_add_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_asm_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_ctx_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_div_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_exp_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_gcd_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_lcl_new.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_lib_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_mont_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_mul_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_new.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_prime.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_prime_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_print_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_rand_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_shift_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_sqr_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\bn_word_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\buffer.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\buffer_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\cert.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\cert_manage.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\cert_manage_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\cert_request.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\cert_request_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\content_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\create_Signed_Certificate_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\cryptlib.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\cryptlib_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\crypto.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\crypto_script.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\crypto_script_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\crypto_size.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\crypto_size_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\crypto_types.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\d2i_r_pr_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\d2i_r_pu_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\decrypttext_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\des.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\des_enc_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\des_locl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\dummy_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\e_os.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\e_os2.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Ecc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Ecc_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Ecdh.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Ecdh_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Ecdsa.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Ecdsa_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Ecelgamal.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Ecelgamal_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\encrypttext.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\encrypttext_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\esc2str_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\ex_data_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Field163.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Field163_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\get_ca_certificate_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Group163.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Group163_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\i2d_r_pr_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\i2d_r_pu_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\key_hash.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\key_hash_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\lhash.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\Macro.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\manage_TCA_Cert_Info_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\md32_common.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\md5.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\md5_dgst_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\md5_locl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\md5_locl_new.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\md5_new.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\md5_one_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\md_rand_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\mem_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\mwcore_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\mwdata_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\mwdef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\mwgetdata_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\mwscript.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\mwscript_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\objects.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\opensslconf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\parse_Hashed_Certificate_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\parse_Signed_Certificate_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\parse_wtls_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\parse_x509_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\proc_cert_response_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rand.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rand_key_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rand_lib_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa_eay_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa_gen_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa_lib_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa_new.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa_oaep_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa_pk1_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa_ssl_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\rsa_verify_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\safestack.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\script.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\script_enum.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\seed.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\seed_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\seed_rand_key_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\set_key_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\sha.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\sha1_one_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\sha1dgst_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\sha_locl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\sig_template_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\signtext.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\signtext_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\signtext_rsa_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\spr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\stack.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\stack_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\types.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\unistd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\util.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\verify_cert_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\verify_Hashed_Certificate_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\verify_Signed_Certificate_new.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\wap_mime.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\wcert.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\MCommerce\x509_verify_new.c
# End Source File
# End Group
# Begin Group "WAP4.8"

# PROP Default_Filter ""
# Begin Group "CORE4.8"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapiclnt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapicrpt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapifile.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapimem.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapimmi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapipush.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapissec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapiudp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\aapiwta.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\alert.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\ansilibs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\buffmgt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\buffmgt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\cache.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\cache.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\capiclnt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\capiclnt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\capicrpt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\capicrpt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\capimem.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\capimem.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\capimmi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\capimmi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\capipush.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\capipush.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\capiudp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\capiudp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\capiwta.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\capiwta.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\optional\charset\charset.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\optional\charset\charset.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\chpars.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\chpars.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\cmmnrsrc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\cmmnrsrc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\confvars.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\connectn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\connmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\contypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\convert.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\crypto.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ctypes.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ctypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\dbfunct.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\dbfunct.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\errcodes.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\glbldecl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\glbldecl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\handshk.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\optional\charset\HCodeCnvt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\optional\charset\HCodeCnvt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\optional\charset\HCodeDef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\optional\charset\HCodeTable.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\hdrutil.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\hdrutil.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\headdef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\header.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\header.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\history.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\history.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\keyexchg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\logcodes.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\logging.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\logging.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mgtent.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_cpu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_main.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_main.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_outp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_outp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_queu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_queu.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_sche.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_sche.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\mk_user.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ml_buf.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ml_buf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ml_err.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ml_err.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ml_mcfw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ml_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ml_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\ml_typw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_codec.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_codec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_contif.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_contif.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_cotok.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_cotok.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_sidec.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_sidec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_sitok.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_sitok.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_sldec.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_sldec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_sltok.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\p_sltok.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pdubuf.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pdubuf.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\prov_dec.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\prov_dec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\provif.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\provif.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\push.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\push.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pushdef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pushelm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pushelm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pushll.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pushll.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pushpckg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\pushpckg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\record.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\reph.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\rephdr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\rephdr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\repll.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\repll.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\sctpred.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\sctpred.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\sctpredg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\sdl_cfgw.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_bpi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_bpi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_crpto.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_crpto.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_float.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_float.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_frame.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_frame.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_fs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_fs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_instr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_instr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_int.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_int.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_libs.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_libs.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_list.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_list.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_misc.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_misc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_ops.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_ops.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_re.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_re.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_sdl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_sdl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_si.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_si.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_sq.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_sq.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_types.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_var.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_var.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_varop.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_wtaia.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_wtaia.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_wtaic.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_wtaic.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_wtaig.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\si_wtaig.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\sptp_tls.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\sptp_tls.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\stackmgr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\stackmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\storage.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\storage.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\tapiclnt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\tapicmmn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\tapimmi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\trnscode.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\trnscode.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\url.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\url.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae_cmmn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae_cmmn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae_push.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae_ua.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae_ua.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae_wsp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wae_wsp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\waectype.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\waectype.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\waedef.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\waedef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\waepckg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\waepckg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\waewtaif.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wap.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wap.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wapcvt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wapcvt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wbxmldec.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wbxmldec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wbxmldef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wbxmlelm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wbxmlelm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdp_udp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdp_udp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdp_ul.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdp_ul.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdppckg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdppckg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdpsuppt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wdpsuppt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\optional\memory\wip_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\optional\memory\wip_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\wipalloc.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wipdb.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wipdb.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\wipprdct.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wiptimer.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wiptimer.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\include\wiptrgt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wipvrsn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wml_mmi.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wml_mmi.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wml_uafn.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wml_uafn.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmldef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmlelm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmlelm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmlevent.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmlevent.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmlif.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmlif.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmlparse.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmlparse.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmltoken.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wmltoken.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wsp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspclwdp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspclwdp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspcmwtp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspcmwtp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspif.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspprocl.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspprocl.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspprocm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wspprocm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wta_ua.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wta_ua.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtachdec.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtachdec.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtachdef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtachelm.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtachelm.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtachtok.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtachtok.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtai_if.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtai_if.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtapckg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtapckg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtls.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtls_wdp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtls_wdp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtlscon.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtlsdef.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtlserr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtlsmain.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtlspckg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtlspckg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtp_wdp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtp_wdp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtppckg.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtppckg.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtpprvdr.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtpsar.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtpsar.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtpsuppt.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\wtpsuppt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\xml_conv.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\CORE\source\xml_conv.h
# End Source File
# End Group
# Begin Group "SKY4.8"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MEM\aapimem.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\PUSH\aapipush.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\UDP\aapiudp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\WTA\aapiwta.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\CLNT\clnt.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\CLNT\clntadapter.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\CLNT\clntapp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\CLNT\clntapp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\CLNT\clntCRYPT.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\CLNT\clntPIM.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\CLNT\clntwallet.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\CLNT\clntwallet.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\EIF\eifwap.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\EIF\eifwap.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\GenDsp.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\GenDsp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MEM\memadapter.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\MMFdecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_adapter.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_characbell.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_connector.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_data.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_dialog.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_func.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_keypad.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_musicbell.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_option.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_wap_menu.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmi_wml.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\mmicmd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\SKY\MMI\SISdecoder.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Source\WAP4.8\dns.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\dssock.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\fbstd.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\wapadapter.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\wapaddress.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\wapapp.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\wapcmd.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\wapcommon.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\wapcommon.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\wapconnector.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\waplog.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\waptask.c
# End Source File
# Begin Source File

SOURCE=..\..\Source\WAP4.8\waptask.h
# End Source File
# End Group
# End Target
# End Project
