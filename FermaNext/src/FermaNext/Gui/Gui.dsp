# Microsoft Developer Studio Project File - Name="Gui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Gui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Gui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Gui.mak" CFG="Gui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Gui - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Gui - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\..\build\debug"
# PROP BASE Intermediate_Dir "..\..\..\build\debug\obj\Gui"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\build\debug"
# PROP Intermediate_Dir "..\..\..\build\debug\obj\Gui"
# PROP Target_Dir ""
MTL=midl
# ADD MTL /nologo /mktyplib203 /win32 /D "_DEBUG"
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include" /I "$(AGGDIR)/include" /I "$(AGGDIR)/svg" /I "." /I "Agg" /I "..\..\Common" /I "..\..\Common\Plugin" /I "..\..\Common\Loader" /I "..\TrussUnit" /I ".." /I "$(QTDIR)/include/ActiveQt" /I "..\..\..\build\debug\mocs\Gui" /I "..\..\..\build\release\ui\Gui" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_LIB" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "WIN32" /D "QT_THREAD_SUPPORT" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /FD -Zm200 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\..\build\release"
# PROP BASE Intermediate_Dir "..\..\..\build\release\obj\Gui"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\build\release"
# PROP Intermediate_Dir "..\..\..\build\release\obj\Gui"
# PROP Target_Dir ""
MTL=midl
# ADD MTL /nologo /mktyplib203 /win32 /D "NDEBUG"
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include" /I "$(AGGDIR)/include" /I "$(AGGDIR)/svg" /I "." /I "Agg" /I "..\..\Common" /I "..\..\Common\Plugin" /I "..\..\Common\Loader" /I "..\TrussUnit" /I ".." /I "$(QTDIR)/include/ActiveQt" /I "..\..\..\build\release\mocs\Gui" /I "..\..\..\build\release\ui\Gui" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_LIB" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "WIN32" /D "QT_THREAD_SUPPORT" /D "QT_NO_DEBUG" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /FD -Zm200 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "Gui - Win32 Debug"
# Name "Gui - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=Agg\AggButton.cpp
# End Source File
# Begin Source File

SOURCE=Agg\AggPaintThread.cpp
# End Source File
# Begin Source File

SOURCE=Agg\AggPopupHint.cpp
# End Source File
# Begin Source File

SOURCE=.\Agg\AggSubsidiary.cpp
# End Source File
# Begin Source File

SOURCE=Agg\AggToolBar.cpp
# End Source File
# Begin Source File

SOURCE=Agg\AggTrussToolBar.cpp
# End Source File
# Begin Source File

SOURCE=Agg\AggTrussWindowButton.cpp
# End Source File
# Begin Source File

SOURCE=Agg\AggWidget.cpp
# End Source File
# Begin Source File

SOURCE=FermaNextMainWindow.cpp
# End Source File
# Begin Source File

SOURCE=GeometryTabWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiSubsidiary.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesWidget.cpp
# End Source File
# Begin Source File

SOURCE=ProjectToolBox.cpp
# End Source File
# Begin Source File

SOURCE=Splash.cpp
# End Source File
# Begin Source File

SOURCE=TrussDesignerWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\TrussPropertyTabWidget.cpp
# End Source File
# Begin Source File

SOURCE=UndoRedoListBox.cpp
# End Source File
# Begin Source File

SOURCE=WindowListBox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=Agg\AggButton.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggButton.h
InputPath=Agg\AggButton.h

"..\..\..\build\debug\mocs\Gui\moc_AggButton.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggButton.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggButton.h
InputPath=Agg\AggButton.h

"..\..\..\build\release\mocs\Gui\moc_AggButton.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggButton.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Agg\AggPaintThread.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggPaintThread.h
InputPath=Agg\AggPaintThread.h

"..\..\..\build\debug\mocs\Gui\moc_AggPaintThread.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggPaintThread.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggPaintThread.h
InputPath=Agg\AggPaintThread.h

"..\..\..\build\release\mocs\Gui\moc_AggPaintThread.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggPaintThread.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Agg\AggPopupHint.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggPopupHint.h
InputPath=Agg\AggPopupHint.h

"..\..\..\build\debug\mocs\Gui\moc_AggPopupHint.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggPopupHint.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggPopupHint.h
InputPath=Agg\AggPopupHint.h

"..\..\..\build\release\mocs\Gui\moc_AggPopupHint.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggPopupHint.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Agg\AggSubsidiary.h
# End Source File
# Begin Source File

SOURCE=Agg\AggToolBar.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggToolBar.h
InputPath=Agg\AggToolBar.h

"..\..\..\build\debug\mocs\Gui\moc_AggToolBar.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggToolBar.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggToolBar.h
InputPath=Agg\AggToolBar.h

"..\..\..\build\release\mocs\Gui\moc_AggToolBar.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggToolBar.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Agg\AggTrussToolBar.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggTrussToolBar.h
InputPath=Agg\AggTrussToolBar.h

"..\..\..\build\debug\mocs\Gui\moc_AggTrussToolBar.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggTrussToolBar.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggTrussToolBar.h
InputPath=Agg\AggTrussToolBar.h

"..\..\..\build\release\mocs\Gui\moc_AggTrussToolBar.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggTrussToolBar.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Agg\AggTrussWindowButton.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggTrussWindowButton.h
InputPath=Agg\AggTrussWindowButton.h

"..\..\..\build\debug\mocs\Gui\moc_AggTrussWindowButton.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggTrussWindowButton.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggTrussWindowButton.h
InputPath=Agg\AggTrussWindowButton.h

"..\..\..\build\release\mocs\Gui\moc_AggTrussWindowButton.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggTrussWindowButton.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Agg\AggWidget.h
# End Source File
# Begin Source File

SOURCE=FermaNextMainWindow.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on FermaNextMainWindow.h
InputPath=FermaNextMainWindow.h

"..\..\..\build\debug\mocs\Gui\moc_FermaNextMainWindow.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_FermaNextMainWindow.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on FermaNextMainWindow.h
InputPath=FermaNextMainWindow.h

"..\..\..\build\release\mocs\Gui\moc_FermaNextMainWindow.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_FermaNextMainWindow.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=GeometryTabWidget.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on GeometryTabWidget.h
InputPath=GeometryTabWidget.h

"..\..\..\build\debug\mocs\Gui\moc_GeometryTabWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_GeometryTabWidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on GeometryTabWidget.h
InputPath=GeometryTabWidget.h

"..\..\..\build\release\mocs\Gui\moc_GeometryTabWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_GeometryTabWidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GuiSubsidiary.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesWidget.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# Begin Custom Build - Running MOC on PreferencesWidget.h
InputPath=.\PreferencesWidget.h

"..\..\..\build\debug\mocs\Gui\moc_PreferencesWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\debug\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_PreferencesWidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# Begin Custom Build - Running MOC on PreferencesWidget.h
InputPath=.\PreferencesWidget.h

"..\..\..\build\release\mocs\Gui\moc_PreferencesWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_PreferencesWidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ProjectToolBox.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ProjectToolBox.h
InputPath=ProjectToolBox.h

"..\..\..\build\debug\mocs\Gui\moc_ProjectToolBox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_ProjectToolBox.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ProjectToolBox.h
InputPath=ProjectToolBox.h

"..\..\..\build\release\mocs\Gui\moc_ProjectToolBox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_ProjectToolBox.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Splash.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Splash.h
InputPath=Splash.h

"..\..\..\build\debug\mocs\Gui\moc_Splash.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_Splash.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Splash.h
InputPath=Splash.h

"..\..\..\build\release\mocs\Gui\moc_Splash.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_Splash.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=TrussDesignerWidget.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussDesignerWidget.h
InputPath=TrussDesignerWidget.h

"..\..\..\build\debug\mocs\Gui\moc_TrussDesignerWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_TrussDesignerWidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussDesignerWidget.h
InputPath=TrussDesignerWidget.h

"..\..\..\build\release\mocs\Gui\moc_TrussDesignerWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_TrussDesignerWidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrussPropertyTabWidget.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# Begin Custom Build - Running MOC on TrussPropertyTabWidget.h
InputPath=.\TrussPropertyTabWidget.h

"..\..\..\build\debug\mocs\Gui\moc_TrussPropertyTabWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_TrussPropertyTabWidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# Begin Custom Build - Running MOC on TrussPropertyTabWidget.h
InputPath=.\TrussPropertyTabWidget.h

"..\..\..\build\release\mocs\Gui\moc_TrussPropertyTabWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_TrussPropertyTabWidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=UndoRedoListBox.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on UndoRedoListBox.h
InputPath=UndoRedoListBox.h

"..\..\..\build\debug\mocs\Gui\moc_UndoRedoListBox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_UndoRedoListBox.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on UndoRedoListBox.h
InputPath=UndoRedoListBox.h

"..\..\..\build\release\mocs\Gui\moc_UndoRedoListBox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_UndoRedoListBox.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=WindowListBox.h

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on WindowListBox.h
InputPath=WindowListBox.h

"..\..\..\build\debug\mocs\Gui\moc_WindowListBox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_WindowListBox.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on WindowListBox.h
InputPath=WindowListBox.h

"..\..\..\build\release\mocs\Gui\moc_WindowListBox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_WindowListBox.cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggButton.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_AggButton.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggPaintThread.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_AggPaintThread.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggPopupHint.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_AggPopupHint.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggToolBar.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_AggToolBar.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggTrussToolBar.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_AggTrussToolBar.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggTrussWindowButton.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_AggTrussWindowButton.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_FermaNextMainWindow.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_FermaNextMainWindow.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_GeometryTabWidget.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_GeometryTabWidget.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_PreferencesWidget.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_PreferencesWidget.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_ProjectToolBox.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_ProjectToolBox.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_Splash.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_Splash.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_TrussDesignerWidget.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_TrussDesignerWidget.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_TrussPropertyTabWidget.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_TrussPropertyTabWidget.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_UndoRedoListBox.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_UndoRedoListBox.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\debug\mocs\Gui\moc_WindowListBox.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\build\release\mocs\Gui\moc_WindowListBox.cpp

!IF  "$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Gui - Win32 Release"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
