# Microsoft Developer Studio Project File - Name="TrussUnit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TrussUnit - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TrussUnit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TrussUnit.mak" CFG="TrussUnit - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TrussUnit - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TrussUnit - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\build\release"
# PROP BASE Intermediate_Dir "..\..\build\release\obj\TrussUnit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\build\release"
# PROP Intermediate_Dir "..\..\build\release\obj\TrussUnit"
# PROP Target_Dir ""
MTL=midl
# ADD MTL /nologo /mktyplib203 /win32 /D "NDEBUG"
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include" /I "$(AGGDIR)/include" /I "$(AGGDIR)/svg" /I "..\common" /I "..\Agg" /I "$(QTDIR)/include/ActiveQt" /I "..\..\build\release\mocs\TrussUnit" /I "..\..\build\debug\ui\TrussUnit" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_LIB" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "WIN32" /D "QT_THREAD_SUPPORT" /D "QT_NO_DEBUG" /D "QT_CORE_LIB" /D "QT_GUI_LIB" /D "QT_XML_LIB" /FD -Zm200 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\build\debug"
# PROP BASE Intermediate_Dir "..\..\build\debug\obj\TrussUnit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\build\debug"
# PROP Intermediate_Dir "..\..\build\debug\obj\TrussUnit"
# PROP Target_Dir ""
MTL=midl
# ADD MTL /nologo /mktyplib203 /win32 /D "_DEBUG"
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include" /I "$(AGGDIR)/include" /I "$(AGGDIR)/svg" /I "..\common" /I "..\Agg" /I "$(QTDIR)/include/ActiveQt" /I "..\..\build\debug\mocs\TrussUnit" /I "..\..\build\debug\ui\TrussUnit" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_LIB" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "WIN32" /D "QT_THREAD_SUPPORT" /D "QT_CORE_LIB" /D "QT_GUI_LIB" /D "QT_XML_LIB" /FD -Zm200 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "TrussUnit - Win32 Release"
# Name "TrussUnit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=Truss.cpp
# End Source File
# Begin Source File

SOURCE=TrussLoad.cpp
# End Source File
# Begin Source File

SOURCE=TrussMaterial.cpp
# End Source File
# Begin Source File

SOURCE=TrussNode.cpp
# End Source File
# Begin Source File

SOURCE=TrussPivot.cpp
# End Source File
# Begin Source File

SOURCE=TrussUnit.cpp
# End Source File
# Begin Source File

SOURCE=TrussUnitActions.cpp
# End Source File
# Begin Source File

SOURCE=TrussUnitDesignerWidget.cpp
# End Source File
# Begin Source File

SOURCE=TrussUnitToolBar.cpp
# End Source File
# Begin Source File

SOURCE=TrussUnitWindow.cpp
# End Source File
# Begin Source File

SOURCE=TrussUnitWindowButton.cpp
# End Source File
# Begin Source File

SOURCE=TrussUnitWindowManager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=Truss.h

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Truss.h
InputPath=Truss.h

"..\..\build\release\mocs\TrussUnit\moc_Truss.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\TrussUnit\moc_Truss.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Truss.h
InputPath=Truss.h

"..\..\build\debug\mocs\TrussUnit\moc_Truss.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\TrussUnit\moc_Truss.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=TrussLoad.h

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussLoad.h
InputPath=TrussLoad.h

"..\..\build\release\mocs\TrussUnit\moc_TrussLoad.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\TrussUnit\moc_TrussLoad.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussLoad.h
InputPath=TrussLoad.h

"..\..\build\debug\mocs\TrussUnit\moc_TrussLoad.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\TrussUnit\moc_TrussLoad.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=TrussMaterial.h
# End Source File
# Begin Source File

SOURCE=TrussUnit.h

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnit.h
InputPath=TrussUnit.h

"..\..\build\release\mocs\TrussUnit\moc_TrussUnit.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\TrussUnit\moc_TrussUnit.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnit.h
InputPath=TrussUnit.h

"..\..\build\debug\mocs\TrussUnit\moc_TrussUnit.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\TrussUnit\moc_TrussUnit.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=TrussUnitActions.h
# End Source File
# Begin Source File

SOURCE=TrussUnitDesignerWidget.h

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitDesignerWidget.h
InputPath=TrussUnitDesignerWidget.h

"..\..\build\release\mocs\TrussUnit\moc_TrussUnitDesignerWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\TrussUnit\moc_TrussUnitDesignerWidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitDesignerWidget.h
InputPath=TrussUnitDesignerWidget.h

"..\..\build\debug\mocs\TrussUnit\moc_TrussUnitDesignerWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\TrussUnit\moc_TrussUnitDesignerWidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=TrussUnitToolBar.h

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitToolBar.h
InputPath=TrussUnitToolBar.h

"..\..\build\release\mocs\TrussUnit\moc_TrussUnitToolBar.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\TrussUnit\moc_TrussUnitToolBar.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitToolBar.h
InputPath=TrussUnitToolBar.h

"..\..\build\debug\mocs\TrussUnit\moc_TrussUnitToolBar.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\TrussUnit\moc_TrussUnitToolBar.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=TrussUnitWindow.h

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitWindow.h
InputPath=TrussUnitWindow.h

"..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindow.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindow.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitWindow.h
InputPath=TrussUnitWindow.h

"..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindow.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindow.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=TrussUnitWindowButton.h

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitWindowButton.h
InputPath=TrussUnitWindowButton.h

"..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindowButton.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindowButton.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitWindowButton.h
InputPath=TrussUnitWindowButton.h

"..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindowButton.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindowButton.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=TrussUnitWindowManager.h

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitWindowManager.h
InputPath=TrussUnitWindowManager.h

"..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindowManager.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindowManager.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussUnitWindowManager.h
InputPath=TrussUnitWindowManager.h

"..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindowManager.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"..\Agg" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\TrussUnit" -I"..\..\build\debug\ui\TrussUnit" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindowManager.cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\build\debug\mocs\TrussUnit\moc_Truss.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\TrussUnit\moc_Truss.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\TrussUnit\moc_TrussLoad.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\TrussUnit\moc_TrussLoad.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\TrussUnit\moc_TrussUnit.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\TrussUnit\moc_TrussUnit.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\TrussUnit\moc_TrussUnitDesignerWidget.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\TrussUnit\moc_TrussUnitDesignerWidget.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\TrussUnit\moc_TrussUnitToolBar.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\TrussUnit\moc_TrussUnitToolBar.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindow.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindow.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindowButton.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindowButton.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\TrussUnit\moc_TrussUnitWindowManager.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\TrussUnit\moc_TrussUnitWindowManager.cpp

!IF  "$(CFG)" == "TrussUnit - Win32 Release"

!ELSEIF  "$(CFG)" == "TrussUnit - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
