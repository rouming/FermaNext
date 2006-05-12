# Microsoft Developer Studio Project File - Name="old_SimpleCalc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=old_SimpleCalc - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "old_SimpleCalc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "old_SimpleCalc.mak" CFG="old_SimpleCalc - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "old_SimpleCalc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "old_SimpleCalc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "old_SimpleCalc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\plugins"
# PROP BASE Intermediate_Dir "..\..\..\build\release\obj\old_SimpleCalc.plg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\plugins"
# PROP Intermediate_Dir "..\..\..\build\release\obj\old_SimpleCalc.plg"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "$(QTDIR)/include/QtNetwork" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include" /I "..\..\Common" /I "..\..\Common\Plugin" /I "..\..\Common\Loader" /I "..\..\FermaNext\TrussUnit" /I "$(QTDIR)/include/ActiveQt" /I "..\..\..\build\release\mocs\old_SimpleCalc.plg" /I "..\..\..\build\release\ui\old_SimpleCalc.plg" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_USRDLL" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "WIN32" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /D "QT_NO_DEBUG" /D "QT_PLUGIN" /D "QT_CORE_LIB" /D "QT_GUI_LIB" /D "QT_NETWORK_LIB" /D "QT_XML_LIB" /FD -Zm200 /c
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "TrussUnit.lib" "Common.lib" "$(QTDIR)\lib\QtCore4.lib" "$(QTDIR)\lib\QtGui4.lib" "$(QTDIR)\lib\QtNetwork4.lib" /nologo /dll /incremental:yes /machine:IX86 /nodefaultlib:"libc" /out:"..\..\..\plugins\old_SimpleCalc.plg.dll" /libpath:"..\..\..\build\release" /libpath:"$(QTDIR)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy plugin wrappers
PostBuild_Cmds=xcopy /y Simple_f.dll ..\..\..\plugins	xcopy /y win_Simple_f.dll ..\..\..\plugins
# End Special Build Tool

!ELSEIF  "$(CFG)" == "old_SimpleCalc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\plugins"
# PROP BASE Intermediate_Dir "..\..\..\build\debug\obj\old_SimpleCalc.plg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\plugins"
# PROP Intermediate_Dir "..\..\..\build\debug\obj\old_SimpleCalc.plg"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /I "$(QTDIR)/include/QtNetwork" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include" /I "..\..\Common" /I "..\..\Common\Plugin" /I "..\..\Common\Loader" /I "..\..\FermaNext\TrussUnit" /I "$(QTDIR)/include/ActiveQt" /I "..\..\..\build\debug\mocs\old_SimpleCalc.plg" /I "..\..\..\build\debug\ui\old_SimpleCalc.plg" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_USRDLL" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "WIN32" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /D "QT_PLUGIN" /D "QT_CORE_LIB" /D "QT_GUI_LIB" /D "QT_NETWORK_LIB" /D "QT_XML_LIB" /FD -Zm200 /c
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "TrussUnit.lib" "Common.lib" "$(QTDIR)\lib\QtCored4.lib" "$(QTDIR)\lib\QtGuid4.lib" "$(QTDIR)\lib\QtNetworkd4.lib" /nologo /dll /debug /machine:IX86 /nodefaultlib:"libc" /out:"..\..\..\plugins\old_SimpleCalc.plg.dll" /libpath:"..\..\..\build\debug" /libpath:"$(QTDIR)\lib"

!ENDIF 

# Begin Target

# Name "old_SimpleCalc - Win32 Release"
# Name "old_SimpleCalc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=FRMWriter.cpp
# End Source File
# Begin Source File

SOURCE=old_SimpleCalc.cpp
# End Source File
# Begin Source File

SOURCE=VYVReader.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=FRMWriter.h
# End Source File
# Begin Source File

SOURCE=.\old_SimpleCalc.h
# End Source File
# Begin Source File

SOURCE=VYVReader.h
# End Source File
# Begin Source File

SOURCE=.\win_SimpleCalc.h
# End Source File
# End Group
# End Target
# End Project
