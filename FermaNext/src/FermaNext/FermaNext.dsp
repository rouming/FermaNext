# Microsoft Developer Studio Project File - Name="FermaNext" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FermaNext - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FermaNext.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FermaNext.mak" CFG="FermaNext - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FermaNext - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "FermaNext - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FermaNext - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\.."
# PROP BASE Intermediate_Dir "..\..\build\debug\obj\FermaNext"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\.."
# PROP Intermediate_Dir "..\..\build\debug\obj\FermaNext"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include/Qt3Support" /I "$(QTDIR)/include" /I "$(AGGDIR)/include" /I "$(AGGDIR)/svg" /I "." /I "Gui" /I "Gui\Agg" /I "TrussUnit" /I "..\Common" /I "..\Common\Plugin" /I "..\Common\Loader" /I "$(QTDIR)/include/ActiveQt" /I "..\..\build\debug\mocs\FermaNext" /I "..\..\build\release\ui\FermaNext" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_WINDOWS" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /D "QT_QT3SUPPORT_LIB" /D "QT3_SUPPORT" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /D "WIN32" /FD -Zm200 /c
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "$(QTDIR)\lib\qtmaind.lib" "Common.lib" "Gui.lib" "TrussUnit.lib" "LibAgg.lib" "LibSVG.lib" "libexpat.lib" "$(QTDIR)\lib\Qt3Supportd4.lib" "$(QTDIR)\lib\QtXmld4.lib" "$(QTDIR)\lib\QtGuid4.lib" "$(QTDIR)\lib\QtCored4.lib" /nologo /subsystem:console /debug /machine:IX86 /nodefaultlib:"libc" /libpath:"..\..\build\debug" /libpath:"..\.." /libpath:"$(EXPATDIR)\Libs" /libpath:"$(QTDIR)\lib"

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\.."
# PROP BASE Intermediate_Dir "..\..\build\release\obj\FermaNext"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\.."
# PROP Intermediate_Dir "..\..\build\release\obj\FermaNext"
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include/Qt3Support" /I "$(QTDIR)/include" /I "$(AGGDIR)/include" /I "$(AGGDIR)/svg" /I "." /I "Gui" /I "Gui\Agg" /I "TrussUnit" /I "..\Common" /I "..\Common\Plugin" /I "..\Common\Loader" /I "$(QTDIR)/include/ActiveQt" /I "..\..\build\release\mocs\FermaNext" /I "..\..\build\release\ui\FermaNext" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_WINDOWS" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /D "QT_NO_DEBUG" /D "QT_QT3SUPPORT_LIB" /D "QT3_SUPPORT" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /D "WIN32" /FD -Zm200 /c
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "$(QTDIR)\lib\qtmain.lib" "Common.lib" "Gui.lib" "TrussUnit.lib" "LibAgg.lib" "LibSVG.lib" "libexpat.lib" "$(QTDIR)\lib\Qt3Support4.lib" "$(QTDIR)\lib\QtXml4.lib" "$(QTDIR)\lib\QtGui4.lib" "$(QTDIR)\lib\QtCore4.lib" /nologo /subsystem:windows /incremental:yes /machine:IX86 /nodefaultlib:"libc" /libpath:"..\..\build\release" /libpath:"..\.." /libpath:"$(EXPATDIR)\Libs" /libpath:"$(QTDIR)\lib"

!ENDIF 

# Begin Target

# Name "FermaNext - Win32 Debug"
# Name "FermaNext - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=FermaNextProject.cpp
# End Source File
# Begin Source File

SOURCE=FermaNextWorkspace.cpp
# End Source File
# Begin Source File

SOURCE=Main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=FermaNextProject.h

!IF  "$(CFG)" == "FermaNext - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on FermaNextProject.h
InputPath=FermaNextProject.h

"..\..\build\debug\mocs\FermaNext\moc_FermaNextProject.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_THREAD_SUPPORT -DQT_DLL -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Gui" -I"Gui\Agg" -I"TrussUnit" -I"..\Common" -I"..\Common\Plugin" -I"..\Common\Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\FermaNext" -I"..\..\build\release\ui\FermaNext" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\FermaNext\moc_FermaNextProject.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on FermaNextProject.h
InputPath=FermaNextProject.h

"..\..\build\release\mocs\FermaNext\moc_FermaNextProject.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_THREAD_SUPPORT -DQT_DLL -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Gui" -I"Gui\Agg" -I"TrussUnit" -I"..\Common" -I"..\Common\Plugin" -I"..\Common\Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\FermaNext" -I"..\..\build\release\ui\FermaNext" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\FermaNext\moc_FermaNextProject.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=FermaNextWorkspace.h

!IF  "$(CFG)" == "FermaNext - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on FermaNextWorkspace.h
InputPath=FermaNextWorkspace.h

"..\..\build\debug\mocs\FermaNext\moc_FermaNextWorkspace.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_THREAD_SUPPORT -DQT_DLL -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Gui" -I"Gui\Agg" -I"TrussUnit" -I"..\Common" -I"..\Common\Plugin" -I"..\Common\Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\FermaNext" -I"..\..\build\release\ui\FermaNext" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\FermaNext\moc_FermaNextWorkspace.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on FermaNextWorkspace.h
InputPath=FermaNextWorkspace.h

"..\..\build\release\mocs\FermaNext\moc_FermaNextWorkspace.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_THREAD_SUPPORT -DQT_DLL -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Gui" -I"Gui\Agg" -I"TrussUnit" -I"..\Common" -I"..\Common\Plugin" -I"..\Common\Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\FermaNext" -I"..\..\build\release\ui\FermaNext" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\FermaNext\moc_FermaNextWorkspace.cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\build\debug\mocs\FermaNext\moc_FermaNextProject.cpp

!IF  "$(CFG)" == "FermaNext - Win32 Debug"

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\FermaNext\moc_FermaNextProject.cpp

!IF  "$(CFG)" == "FermaNext - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\FermaNext\moc_FermaNextWorkspace.cpp

!IF  "$(CFG)" == "FermaNext - Win32 Debug"

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\FermaNext\moc_FermaNextWorkspace.cpp

!IF  "$(CFG)" == "FermaNext - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Release"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
