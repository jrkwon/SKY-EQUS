# Microsoft Developer Studio Project File - Name="Graphics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Graphics - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Graphics.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Graphics.mak" CFG="Graphics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Graphics - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Graphics - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/NUI-Ori/Mobile/Source/EQUS/DISPLAY/TEST4WIN32", MXTBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Graphics - Win32 Release"

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

!ELSEIF  "$(CFG)" == "Graphics - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Graphics___Win32_Debug"
# PROP BASE Intermediate_Dir "Graphics___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\..\INCLUDE" /I "..\..\CONTROL\INCLUDE" /I "..\..\APPLET\INCLUDE" /I "..\..\I18N" /I "..\..\State" /I "..\..\Control" /I "..\..\RESOURCE\STRING" /I "..\..\RESOURCE\IMAGE\BITMAP" /I "..\..\PLATFORM\WIN32" /I "..\..\DISPLAY\FONT" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CONFIG_EQS_COLOR_4GRAY" /FD /GZ /c
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

# Name "Graphics - Win32 Release"
# Name "Graphics - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ControllerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\Graphics.rc
# End Source File
# Begin Source File

SOURCE=.\GraphicsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\testgrph.c
# End Source File
# Begin Source File

SOURCE=.\testmult.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ControllerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Graphics.h
# End Source File
# Begin Source File

SOURCE=.\GraphicsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\testgrph.h
# End Source File
# Begin Source File

SOURCE=.\testmult.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Graphics.ico
# End Source File
# Begin Source File

SOURCE=.\res\Graphics.rc2
# End Source File
# End Group
# Begin Group "EQUS"

# PROP Default_Filter ""
# Begin Group "Token"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\I18N\eqs_kor.tkn
# End Source File
# Begin Source File

SOURCE=..\..\I18N\eqs_usa.tkn
# End Source File
# End Group
# Begin Source File

SOURCE=..\eqs_dc.c
# End Source File
# Begin Source File

SOURCE=..\eqs_grph.c
# End Source File
# Begin Source File

SOURCE=..\eqs_gui.c
# End Source File
# Begin Source File

SOURCE=..\..\I18N\eqs_i18n.c
# End Source File
# Begin Source File

SOURCE=..\..\UTIL\eqs_list.c
# End Source File
# Begin Source File

SOURCE=..\..\UTIL\eqs_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\I18N\eqs_tokn.c
# End Source File
# Begin Source File

SOURCE=..\..\UTIL\eqs_util.c
# End Source File
# End Group
# Begin Group "EQUS Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\INCLUDE\eqs.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_all.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_dc.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_err.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_evnt.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_feat.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_grph.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_gui.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_i18n.h
# End Source File
# Begin Source File

SOURCE=..\..\CONTROL\eqs_ioc.h
# End Source File
# Begin Source File

SOURCE=..\..\STATE\eqs_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_tokn.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\eqs_type.h
# End Source File
# Begin Source File

SOURCE=..\..\STATE\eqs_util.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
