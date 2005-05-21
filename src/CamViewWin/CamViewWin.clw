; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCamViewWinDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CamViewWin.h"

ClassCount=3
Class1=CCamViewWinApp
Class2=CCamViewWinDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CAMVIEWWIN_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_CAMVIEWWIN_DIALOG (English (U.S.))

[CLS:CCamViewWinApp]
Type=0
HeaderFile=CamViewWin.h
ImplementationFile=CamViewWin.cpp
Filter=N

[CLS:CCamViewWinDlg]
Type=0
HeaderFile=CamViewWinDlg.h
ImplementationFile=CamViewWinDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_TEST

[CLS:CAboutDlg]
Type=0
HeaderFile=CamViewWinDlg.h
ImplementationFile=CamViewWinDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_CAMVIEWWIN_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CCamViewWinDlg

[DLG:IDD_CAMVIEWWIN_DIALOG (English (U.S.))]
Type=1
Class=CCamViewWinDlg
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_TEST,button,1342242816
Control4=IDC_CAMERAS,listbox,1352728835
Control5=IDC_VIDEOMODES,listbox,1352728833

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

