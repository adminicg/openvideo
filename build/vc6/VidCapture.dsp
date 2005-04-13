# Microsoft Developer Studio Project File - Name="VidCapture" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VidCapture - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VidCapture.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VidCapture.mak" CFG="VidCapture - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VidCapture - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VidCapture - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "VidCapture - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_VidCapture"
# PROP Intermediate_Dir "Release_VidCapture"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/VidCapture.lib"

!ELSEIF  "$(CFG)" == "VidCapture - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_VidCapture"
# PROP Intermediate_Dir "Debug_VidCapture"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/VidCaptured.lib"

!ENDIF 

# Begin Target

# Name "VidCapture - Win32 Release"
# Name "VidCapture - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\vidcapture\CVDShowUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImageGrey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImageRGB24.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImageRGBFloat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVPlatformWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVTraceWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVVidCapture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVVidCaptureDSWin32.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\vidcapture\CVDShowUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVFile.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImageGrey.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImageRGB24.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImageRGBFloat.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVImageStructs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVPlatform.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVRes.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVResDll.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVResFile.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVResImage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVResVidCap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVTrace.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVVidCapture.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\CVVidCaptureDSWin32.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\VidCapture.h
# End Source File
# Begin Source File

SOURCE=..\..\src\vidcapture\VidCapture_Docs.h
# End Source File
# End Group
# End Target
# End Project
