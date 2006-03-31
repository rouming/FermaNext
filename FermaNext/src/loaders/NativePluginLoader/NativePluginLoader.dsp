# Microsoft Developer Studio Project File - Name="NativePluginLoader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=NativePluginLoader - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NativePluginLoader.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NativePluginLoader.mak" CFG="NativePluginLoader - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NativePluginLoader - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NativePluginLoader - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NativePluginLoader - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\plugins\loaders"
# PROP BASE Intermediate_Dir "..\..\..\build\release\obj\NativePluginLoader.ldr"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\plugins\loaders"
# PROP Intermediate_Dir "..\..\..\build\release\obj\NativePluginLoader.ldr"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include" /I "..\..\common" /I "..\..\common\plugin" /I "..\..\common\plugin\loader" /I "$(QTDIR)/include/ActiveQt" /I "..\..\..\build\release\mocs\NativePluginLoader.ldr" /I "..\..\..\build\debug\ui\NativePluginLoader.ldr" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_USRDLL" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /D "QT_NO_DEBUG" /D "QT_PLUGIN" /D "QT_CORE_LIB" /D "QT_GUI_LIB" /D "WIN32" /FD -Zm200 /c
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "common.lib" "$(QTDIR)\lib\QtCore4.lib" "$(QTDIR)\lib\QtGui4.lib" /nologo /dll /incremental:yes /machine:IX86 /nodefaultlib:"libc" /out:"..\..\..\plugins\loaders\NativePluginLoader.ldr.dll" /libpath:"..\..\..\build\debug" /libpath:"$(QTDIR)\lib"

!ELSEIF  "$(CFG)" == "NativePluginLoader - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\plugins\loaders"
# PROP BASE Intermediate_Dir "..\..\..\build\debug\obj\NativePluginLoader.ldr"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\plugins\loaders"
# PROP Intermediate_Dir "..\..\..\build\debug\obj\NativePluginLoader.ldr"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include" /I "..\..\common" /I "..\..\common\plugin" /I "..\..\common\plugin\loader" /I "$(QTDIR)/include/ActiveQt" /I "..\..\..\build\debug\mocs\NativePluginLoader.ldr" /I "..\..\..\build\debug\ui\NativePluginLoader.ldr" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_USRDLL" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /D "QT_PLUGIN" /D "QT_CORE_LIB" /D "QT_GUI_LIB" /D "WIN32" /FD -Zm200 /c
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "common.lib" "$(QTDIR)\lib\QtCored4.lib" "$(QTDIR)\lib\QtGuid4.lib" /nologo /dll /debug /machine:IX86 /nodefaultlib:"libc" /out:"..\..\..\plugins\loaders\NativePluginLoader.ldr.dll" /libpath:"..\..\..\build\debug" /libpath:"$(QTDIR)\lib"

!ENDIF 

# Begin Target

# Name "NativePluginLoader - Win32 Release"
# Name "NativePluginLoader - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=NativePluginLoader.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=NativePluginLoader.h
# End Source File
# End Group
# End Target
# End Project
