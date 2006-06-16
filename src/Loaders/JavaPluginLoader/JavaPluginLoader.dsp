# Microsoft Developer Studio Project File - Name="JavaPluginLoader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JavaPluginLoader - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JavaPluginLoader.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JavaPluginLoader.mak" CFG="JavaPluginLoader - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JavaPluginLoader - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JavaPluginLoader - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JavaPluginLoader - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\plugins\loaders"
# PROP BASE Intermediate_Dir "..\..\..\build\release\obj\JavaPluginLoader.ldr"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\plugins\loaders"
# PROP Intermediate_Dir "..\..\..\build\release\obj\JavaPluginLoader.ldr"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "stubs-src" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include" /I "..\..\Common" /I "..\..\Common\Plugin" /I "..\..\Common\Loader" /I "$(JAVA_HOME)\include" /I "$(JAVA_HOME)\include\win32" /I "$(QTDIR)/include/ActiveQt" /I "..\..\..\build\release\mocs\JavaPluginLoader.ldr" /I "..\..\..\build\debug\ui\JavaPluginLoader.ldr" /I "$(QTDIR)/mkspecs/win32-msvc" /D "QT_NO_DEBUG" /D "_USRDLL" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /D "QT_PLUGIN" /D "QT_CORE_LIB" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "WIN32" /FD -Zm200 /c
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "Common.lib" "$(QTDIR)\lib\QtCore4.lib" "$(QTDIR)\lib\QtXml4.lib" "$(QTDIR)\lib\QtGui4.lib" /nologo /dll /incremental:yes /machine:IX86 /nodefaultlib:"libc" /out:"..\..\..\plugins\loaders\JavaPluginLoader.ldr.dll" /libpath:"..\..\..\build\release" /libpath:"$(QTDIR)\lib"

!ELSEIF  "$(CFG)" == "JavaPluginLoader - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\plugins\loaders"
# PROP BASE Intermediate_Dir "..\..\..\build\debug\obj\JavaPluginLoader.ldr"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\plugins\loaders"
# PROP Intermediate_Dir "..\..\..\build\debug\obj\JavaPluginLoader.ldr"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /I "stubs-src" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include" /I "..\..\Common" /I "..\..\Common\Plugin" /I "..\..\Common\Loader" /I "$(JAVA_HOME)\include" /I "$(JAVA_HOME)\include\win32" /I "$(QTDIR)/include/ActiveQt" /I "..\..\..\build\debug\mocs\JavaPluginLoader.ldr" /I "..\..\..\build\debug\ui\JavaPluginLoader.ldr" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_USRDLL" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /D "QT_PLUGIN" /D "QT_CORE_LIB" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "WIN32" /FD -Zm200 /c
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "Common.lib" "$(QTDIR)\lib\QtCored4.lib" "$(QTDIR)\lib\QtXmld4.lib" "$(QTDIR)\lib\QtGuid4.lib" /nologo /dll /debug /machine:IX86 /nodefaultlib:"libc" /out:"..\..\..\plugins\loaders\JavaPluginLoader.ldr.dll" /libpath:"..\..\..\build\debug" /libpath:"$(QTDIR)\lib"

!ENDIF 

# Begin Target

# Name "JavaPluginLoader - Win32 Release"
# Name "JavaPluginLoader - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="stubs-src\fermanext_trussunit_TrussUnit.cpp"
# End Source File
# Begin Source File

SOURCE=.\JavaPlugin.cpp
# End Source File
# Begin Source File

SOURCE=JavaPluginLoader.cpp
# End Source File
# Begin Source File

SOURCE=JavaVM\JavaVirtualMachine.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\JavaPlugin.h
# End Source File
# Begin Source File

SOURCE=JavaPluginLoader.h

!IF  "$(CFG)" == "JavaPluginLoader - Win32 Release"

# Begin Custom Build - Building Java sources ...
TargetDir=\ferma\Source\FermaNext\plugins\loaders
InputPath=JavaPluginLoader.h

"$(TargetDir)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(ANT_HOME)/bin/ant

# End Custom Build

!ELSEIF  "$(CFG)" == "JavaPluginLoader - Win32 Debug"

# Begin Custom Build - Building Java sources ...
TargetDir=\ferma\Source\FermaNext\plugins\loaders
InputPath=JavaPluginLoader.h

"$(TargetDir)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(ANT_HOME)/bin/ant

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=JavaVM\JavaVirtualMachine.h
# End Source File
# Begin Source File

SOURCE=JavaVM\JNITypes.h
# End Source File
# End Group
# End Target
# End Project