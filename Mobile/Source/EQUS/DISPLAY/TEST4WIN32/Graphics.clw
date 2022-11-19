; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CControllerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Graphics.h"

ClassCount=5
Class1=CGraphicsApp
Class2=CGraphicsDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CRemocon
Resource3=IDD_GRAPHICS_DIALOG
Class5=CControllerDlg
Resource4=IDD_CONTROLLER

[CLS:CGraphicsApp]
Type=0
HeaderFile=Graphics.h
ImplementationFile=Graphics.cpp
Filter=N

[CLS:CGraphicsDlg]
Type=0
HeaderFile=GraphicsDlg.h
ImplementationFile=GraphicsDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BTN_OPEN

[CLS:CAboutDlg]
Type=0
HeaderFile=GraphicsDlg.h
ImplementationFile=GraphicsDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_GRAPHICS_DIALOG]
Type=1
Class=CGraphicsDlg
ControlCount=15
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_RDO_COLOR,button,1342373897
Control4=IDC_RDO_COLOR2,button,1342177289
Control5=IDC_BTN_OPEN,button,1342242816
Control6=IDC_BTN_CLOSE,button,1342242816
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,button,1342177287
Control9=IDC_RDO_MAG,button,1342373897
Control10=IDC_RDO_MAG2,button,1342177289
Control11=IDC_RDO_MAG3,button,1342177289
Control12=IDC_RDO_MAG4,button,1342177289
Control13=IDC_BTN_REFRESH,button,1342242816
Control14=IDC_BTN_CLS,button,1342242816
Control15=IDOK,button,1342242817

[CLS:CRemocon]
Type=0
HeaderFile=Remocon.h
ImplementationFile=Remocon.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CRemocon

[DLG:IDD_CONTROLLER]
Type=1
Class=CControllerDlg
ControlCount=6
Control1=IDC_BTN_PRIMITIVE,button,1342242816
Control2=IDC_BTN_TEXTOUT,button,1342242816
Control3=IDC_BTN_4GIMAGE,button,1342242816
Control4=IDC_BTN_256IMAGE,button,1342242816
Control5=IDC_BTN_DC,button,1342242816
Control6=IDC_BTN_GETPUTIMAGE,button,1342242816

[CLS:CControllerDlg]
Type=0
HeaderFile=ControllerDlg.h
ImplementationFile=ControllerDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BTN_GETIMAGE

