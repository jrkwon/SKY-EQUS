# Microsoft Developer Studio Project File - Name="SkySimul" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SkySimul - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SkySimul.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SkySimul.mak" CFG="SkySimul - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SkySimul - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SkySimul - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/NEO-Penta/Mobile/Source/SKY/B0/SkySimul", BXNDAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SkySimul - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SkySimul - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\INCLUDE" /I "..\APPLET\INCLUDE" /I "..\PLATFORM\WIN32" /I "..\DEVICE\INCLUDE" /I "..\RESOURCE\STRING" /I "..\RESOURCE\IMAGE\ANI" /I "..\RESOURCE\IMAGE\BITMAP" /I "..\RESOURCE\IMAGE\ICON" /I "..\..\..\EQUS\INCLUDE" /I "..\..\..\EQUS\CONTROL\INCLUDE" /I "..\..\..\EQUS\DISPLAY\FONT" /I "..\..\..\EQUS\RESOURCE\STRING" /I "..\..\..\EQUS\RESOURCE\IMAGE\BITMAP" /I "..\..\..\EQUS\PLATFORM\WIN32" /I "..\..\..\QUALCOMM30" /I "..\..\..\AUDIO" /I "..\..\..\SOCKAPI" /I "..\..\..\DS" /I "..\..\..\IrDA" /I "..\..\..\MODELDEP" /I "..\..\..\MMI\QUALCOMM" /I "..\..\..\..\include" /I "..\..\..\..\..\terminal\akeygen\msmhdr" /I "..\..\..\sounder" /I "..\..\..\WAP_B2\REX\EIF" /I "..\..\..\WAP_B2\CORE\include" /I "..\..\..\flash" /I "..\..\..\mcommerce" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WIN32" /D MODEL_DEP_H=\"Bunny.h\" /D CUST_H=\"custb2.h\" /D TG=T_G /D "_FOR_BROWSER" /D MSG_FILE=\"MSG\" /D MSG_LEVEL=MSG_LVL_FATAL /D "T_MSM5000" /D "CONFIG_EQS_COLOR_64K" /D SKY_MODEL_ID=SKY_MODEL_B2 /D "USE_IRDA" /D "FEATURE_SKT_DEBUG" /D "FEATURE_SKTT_PM" /D "WAP_B2" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "SkySimul - Win32 Release"
# Name "SkySimul - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SkySimul.cpp
# End Source File
# Begin Source File

SOURCE=.\SkySimul.rc
# End Source File
# Begin Source File

SOURCE=.\SkySimulDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SkySimul.h
# End Source File
# Begin Source File

SOURCE=.\SkySimulDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SkySimul.ico
# End Source File
# Begin Source File

SOURCE=.\res\SkySimul.rc2
# End Source File
# End Group
# Begin Group "EQUS"

# PROP Default_Filter ""
# Begin Group "CONTROL"

# PROP Default_Filter ""
# Begin Group "INC_CTRL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_aicn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_all.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_anim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_ckbx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_img.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_menu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_msgb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_nedt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_pbar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_pbtn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_pmnu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_rbtn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_scrl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_spin.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_stxt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_tedt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqc_time.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqs_eng.tbl
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqs_kor.tbl
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\INCLUDE\eqs_sym.tbl
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_aicn.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_anim.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_ckbx.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_img.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_menu.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_msgb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_nedt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_pbar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_pbtn.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_pmnu.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_rbtn.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_scrl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_spin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_stxt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_tedt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\CONTROL\eqc_time.c
# End Source File
# End Group
# Begin Group "DISPLAY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\DISPLAY\eqs_dc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\DISPLAY\eqs_frmt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\DISPLAY\eqs_grph.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\DISPLAY\eqs_gui.c
# End Source File
# End Group
# Begin Group "INC_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_aapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_all.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_data.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_dc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_err.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_evnt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_evnt.tbl
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_feat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_frmt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_grph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_gui.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_i18n.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_sfnt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_swin.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_timr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_tokn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_type.h
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\INCLUDE\eqs_util.h
# End Source File
# End Group
# Begin Group "STATE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\STATE\eqs_aapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\STATE\eqs_swin.c
# End Source File
# End Group
# Begin Group "I18N_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\I18N\eqs_i18n.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\I18N\eqs_tokn.c
# End Source File
# End Group
# Begin Group "UTIL"

# PROP Default_Filter ""
# Begin Group "MEMORY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\UTIL\MEMORY\au_mem.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\EQUS\UTIL\eqs_list.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\UTIL\eqs_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\UTIL\eqs_timr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\UTIL\eqs_util.c
# End Source File
# End Group
# Begin Group "RES_EQUS"

# PROP Default_Filter ""
# Begin Group "STR_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\RESOURCE\STRING\eqs_kor.tkn
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\RESOURCE\STRING\eqs_usa.tkn
# End Source File
# End Group
# Begin Group "IMG_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\RESOURCE\IMAGE\eqs_data.c
# End Source File
# End Group
# End Group
# Begin Group "PLT_EQUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\EQUS\PLATFORM\WIN32\eqswutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\EQUS\PLATFORM\WIN32\eqswutil.h
# End Source File
# End Group
# End Group
# Begin Group "SKY"

# PROP Default_Filter ""
# Begin Group "DEVICE"

# PROP Default_Filter ""
# Begin Group "INC_DEV"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\DEVICE\INCLUDE\sky_lcd.h
# End Source File
# Begin Source File

SOURCE=..\DEVICE\INCLUDE\sky_slcd.h
# End Source File
# End Group
# Begin Group "LCD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\DEVICE\LCD\sky_lcd.c
# End Source File
# Begin Source File

SOURCE=..\DEVICE\LCD\sky_slcd.c
# End Source File
# End Group
# End Group
# Begin Group "APPLET"

# PROP Default_Filter ""
# Begin Group "SYS_APP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\SYSTEM\ska_call.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SYSTEM\ska_clid.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SYSTEM\ska_code.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SYSTEM\ska_idle.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SYSTEM\ska_nam.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SYSTEM\ska_off.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SYSTEM\ska_strt.c
# End Source File
# End Group
# Begin Group "INC_APP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_011.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_2get.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_aans.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_add.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_alrm.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_bapi.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_bell.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_bstr.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_butl.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_calc.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_call.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_cam.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_clid.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_code.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_dmgr.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_dscf.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_dsvc.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_eif.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_game.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_gvm.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_hdon.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_idle.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_info.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_init.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_irda.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_irph.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_ligt.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_lock.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_mail.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_memo.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_mo.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_nams.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_ntop.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_off.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_pbo.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_pic.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_pop3.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_sbel.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_scal.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_schl.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_sfil.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_slst.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_sms.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_smsl.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_smtp.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_stop.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_strt.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_test.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_tnum.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_vcmm.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_ver.h
# End Source File
# Begin Source File

SOURCE=..\Applet\Include\ska_vm.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_vmemo.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_vms.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_volm.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_wap.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_wav.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\ska_wtim.h
# End Source File
# Begin Source File

SOURCE=..\APPLET\INCLUDE\sky_wav.h
# End Source File
# End Group
# Begin Group "PBOOK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\PBOOK\ska_bapi.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\PBOOK\ska_bstr.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\PBOOK\ska_butl.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\PBOOK\ska_pbo.c
# End Source File
# End Group
# Begin Group "MENU"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\MENU\ska_menu.c
# End Source File
# End Group
# Begin Group "AUTOANS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\AUTOANS\ska_aans.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\AUTOANS\ska_aapi.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\AUTOANS\ska_crec.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\AUTOANS\ska_hdon.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\AUTOANS\ska_ment.c
# End Source File
# End Group
# Begin Group "RECPLAY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\RECPLAY\ska_vmemo.c
# End Source File
# End Group
# Begin Group "CALC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\CALC\ska_calc.c
# End Source File
# End Group
# Begin Group "EMAIL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\EMAIL\ska_mail.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\EMAIL\ska_mbox.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\EMAIL\ska_pop3.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\EMAIL\ska_smtp.c
# End Source File
# End Group
# Begin Group "GAME"

# PROP Default_Filter ""
# End Group
# Begin Group "IrDA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\IRDA\IrDA_B0.tbl
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\IrDA_B1.tbl
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\ska_ftrs.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\ska_iani.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\ska_ibok.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\ska_imel.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\ska_ipht.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\ska_irda.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\ska_irpc.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\IRDA\ska_irph.c
# End Source File
# End Group
# Begin Group "TEST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\TEST\CtrlTest.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\TEST\LcdTest.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\TEST\MelodyTest.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\TEST\ska_efs.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\TEST\ska_test.c
# End Source File
# End Group
# Begin Group "SETTING"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_011.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_017.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_2get.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_add.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_info.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_init.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_ligt.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_lock.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_nams.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_ntop.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_thme.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_tnum.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_ver.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SETTING\ska_volm.c
# End Source File
# End Group
# Begin Group "SCHEDULE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Applet\Schedule\ska_sbel.c
# End Source File
# Begin Source File

SOURCE=..\Applet\Schedule\ska_scal.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SCHEDULE\ska_schl.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SCHEDULE\ska_sfil.c
# End Source File
# Begin Source File

SOURCE=..\Applet\Schedule\ska_shld.c
# End Source File
# Begin Source File

SOURCE=..\Applet\Schedule\ska_slst.c
# End Source File
# End Group
# Begin Group "STOPWATCH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\SWATCH\ska_stop.c
# End Source File
# End Group
# Begin Group "VOCODER"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\VOCODER\ska_vcmm.c
# End Source File
# End Group
# Begin Group "SMS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\SMS\ska_mo.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SMS\ska_mybe.c
# End Source File
# Begin Source File

SOURCE=..\Applet\Sms\ska_sms.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SMS\ska_smsl.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\SMS\ska_vms.c
# End Source File
# End Group
# Begin Group "WORLDTIME"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\WORLDTIME\ska_wtim.c
# End Source File
# End Group
# Begin Group "BELL_VIB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\BELL_VIB\Bell_ID.B1
# End Source File
# Begin Source File

SOURCE=..\APPLET\BELL_VIB\ska_bell.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\BELL_VIB\ska_mbel.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\BELL_VIB\ska_warn.c
# End Source File
# End Group
# Begin Group "DATACFG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Applet\Datacfg\ska_dscf.c
# End Source File
# End Group
# Begin Group "PICTURE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\PICTURE\ANI.B0
# End Source File
# Begin Source File

SOURCE=..\APPLET\PICTURE\ANI.B1
# End Source File
# Begin Source File

SOURCE=..\APPLET\PICTURE\ska_mani.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\PICTURE\ska_pic.c
# End Source File
# End Group
# Begin Group "ALARM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Applet\Alarm\ska_alrm.c
# End Source File
# End Group
# Begin Group "MEMO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Applet\Memo\ska_memo.c
# End Source File
# End Group
# Begin Group "CAMERA"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=..\APPLET\CAMERA\ska_cam.c
# End Source File
# End Group
# Begin Group "DATAMGR"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=..\APPLET\DATAMGR\ska_dmgr.c
# End Source File
# Begin Source File

SOURCE=..\APPLET\DATAMGR\ska_dsvc.c
# End Source File
# End Group
# Begin Group "EIF"

# PROP Default_Filter ""
# End Group
# Begin Group "GVM"

# PROP Default_Filter ""
# End Group
# Begin Group "VM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\VM\ska_vm.c
# End Source File
# End Group
# Begin Group "CALLLIST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\APPLET\CALLLIST\ska_clst.c
# End Source File
# End Group
# End Group
# Begin Group "INC_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\INCLUDE\sky.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_all.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_annu.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_brkr.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_cm.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_cml.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_conf.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_data.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_def.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_efs.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_evnt.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_feat.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_file.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_hcbt.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_hcmd.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_hkey.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_hs.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_hsig.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_img.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_mem.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_mtxt.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_mus.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_nv.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_rex.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_scrn.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_snd.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_tokn.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\sky_ui.h
# End Source File
# End Group
# Begin Group "I18N_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\I18N\sky_tokn.c
# End Source File
# End Group
# Begin Group "RES_SKY"

# PROP Default_Filter ""
# Begin Group "STR_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\RESOURCE\STRING\sky_kor.tkn
# End Source File
# Begin Source File

SOURCE=..\RESOURCE\STRING\sky_usa.tkn
# End Source File
# End Group
# Begin Group "SOUND"

# PROP Default_Filter ""
# End Group
# Begin Group "IMG_SKY"

# PROP Default_Filter ""
# Begin Group "ANI"

# PROP Default_Filter ""
# End Group
# Begin Group "ICON"

# PROP Default_Filter ""
# End Group
# Begin Group "BITMAP"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=..\RESOURCE\IMAGE\sky_data.c
# End Source File
# End Group
# Begin Group "MENU_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\RESOURCE\MENU\sky_mtxt.c
# End Source File
# End Group
# End Group
# Begin Group "SYSTEM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SYSTEM\sky_annu.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_cm.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_cml.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_conf.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_evnt.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_file.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_hcbt.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_hcmd.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_hkey.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_hs.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_hsig.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_irda.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_mem.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_nv.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_scrn.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_snd.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_sock.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\sky_ui.c
# End Source File
# End Group
# Begin Group "PLT_SKY"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\PLATFORM\WIN32\skywefs.c
# End Source File
# Begin Source File

SOURCE=..\PLATFORM\WIN32\skywefs.h
# End Source File
# Begin Source File

SOURCE=..\PLATFORM\WIN32\skywrex.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\Audio\LZB.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Audio\LZBDecompress.c
# End Source File
# End Group
# Begin Group "QUALCOMM"

# PROP Default_Filter ""
# Begin Group "SRC_QUAL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\QUALCOMM30\bio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\QUALCOMM30\db.c
# End Source File
# Begin Source File

SOURCE=..\..\..\QUALCOMM30\dmod.c
# End Source File
# Begin Source File

SOURCE=..\..\..\QUALCOMM30\queue.c
# End Source File
# Begin Source File

SOURCE=..\..\..\QUALCOMM30\qw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\QUALCOMM30\rex.c
# End Source File
# Begin Source File

SOURCE=..\..\..\QUALCOMM30\task.c
# End Source File
# Begin Source File

SOURCE=..\SYSTEM\uixcm.c
# End Source File
# End Group
# Begin Group "INC_QUAL"

# PROP Default_Filter ""
# End Group
# End Group
# Begin Group "PEGASUS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\PEGASUS\temp4build.c
# End Source File
# End Group
# Begin Group "msm4win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClkSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\EqsSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\EqsSystem.h
# End Source File
# Begin Source File

SOURCE=.\msm4win32.cpp
# End Source File
# Begin Source File

SOURCE=.\msm4win32.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
