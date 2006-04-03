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
CPP=cl
MTL=midl
RSC=rc
BSC32=bscmake.exe

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
# ADD CPP /I"$(QTDIR)/include/QtCore" /I"$(QTDIR)/include/QtGui" /I"$(QTDIR)/include/QtXml" /I"$(QTDIR)/include/Qt3Support" /I"$(QTDIR)/include" /I"$(AGGDIR)/include" /I"$(AGGDIR)/svg" /I"." /I"Agg" /I"..\..\Common" /I"..\..\Common\Plugin" /I"..\..\Common\Loader" /I"..\TrussUnit" /I".." /I"$(QTDIR)/include/ActiveQt" /I"..\..\..\build\debug\mocs\Gui" /I"..\..\..\build\release\ui\Gui" /I"$(QTDIR)/mkspecs/win32-msvc" /c /FD -nologo -Zm200 -Zi -MDd -Zi -MDd -W3 -GR -GX  /D "_LIB"  /D UNICODE /D QT_LARGEFILE_SUPPORT /D WIN32 /D QT_THREAD_SUPPORT /D QT_QT3SUPPORT_LIB /D QT3_SUPPORT /D QT_XML_LIB /D QT_GUI_LIB /D QT_CORE_LIB  /D "WIN32"  
# ADD MTL /nologo /mktyplib203 /win32 /D "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
# ADD BSC32 /nologo
LIB32=lib /NOLOGO
# ADD LIB32 /out:"..\..\..\build\debug\Gui.lib" 

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
# ADD CPP /I"$(QTDIR)/include/QtCore" /I"$(QTDIR)/include/QtGui" /I"$(QTDIR)/include/QtXml" /I"$(QTDIR)/include/Qt3Support" /I"$(QTDIR)/include" /I"$(AGGDIR)/include" /I"$(AGGDIR)/svg" /I"." /I"Agg" /I"..\..\Common" /I"..\..\Common\Plugin" /I"..\..\Common\Loader" /I"..\TrussUnit" /I".." /I"$(QTDIR)/include/ActiveQt" /I"..\..\..\build\release\mocs\Gui" /I"..\..\..\build\release\ui\Gui" /I"$(QTDIR)/mkspecs/win32-msvc" /c /FD -nologo -Zm200 -O1 -MD -O1 -MD -W3 -GR -GX  /D "_LIB"  /D UNICODE /D QT_LARGEFILE_SUPPORT /D WIN32 /D QT_THREAD_SUPPORT /D QT_NO_DEBUG /D QT_QT3SUPPORT_LIB /D QT3_SUPPORT /D QT_XML_LIB /D QT_GUI_LIB /D QT_CORE_LIB  /D "WIN32"  
# ADD MTL /nologo /mktyplib203 /win32 /D "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
# ADD BSC32 /nologo
LIB32=lib /NOLOGO
# ADD LIB32 /out:"..\..\..\build\release\Gui.lib" 

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
SOURCE=Agg\AggSubsidiary.cpp
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
SOURCE=ProjectToolBox.cpp
# End Source File

# Begin Source File
SOURCE=Splash.cpp
# End Source File

# Begin Source File
SOURCE=ToolBarManager.cpp
# End Source File

# Begin Source File
SOURCE=TrussDesignerWidget.cpp
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
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_Agg\AggButton.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggButton.h
InputPath=Agg\AggButton.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggButton.cpp
"..\..\..\build\debug\mocs\Gui\moc_AggButton.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_Agg\AggButton.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggButton.h
InputPath=Agg\AggButton.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggButton.cpp
"..\..\..\build\release\mocs\Gui\moc_AggButton.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=Agg\AggPaintThread.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_Agg\AggPaintThread.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggPaintThread.h
InputPath=Agg\AggPaintThread.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggPaintThread.cpp
"..\..\..\build\debug\mocs\Gui\moc_AggPaintThread.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_Agg\AggPaintThread.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggPaintThread.h
InputPath=Agg\AggPaintThread.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggPaintThread.cpp
"..\..\..\build\release\mocs\Gui\moc_AggPaintThread.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=Agg\AggPopupHint.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_Agg\AggPopupHint.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggPopupHint.h
InputPath=Agg\AggPopupHint.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggPopupHint.cpp
"..\..\..\build\debug\mocs\Gui\moc_AggPopupHint.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_Agg\AggPopupHint.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggPopupHint.h
InputPath=Agg\AggPopupHint.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggPopupHint.cpp
"..\..\..\build\release\mocs\Gui\moc_AggPopupHint.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=Agg\AggSubsidiary.h
# End Source File

# Begin Source File
SOURCE=Agg\AggToolBar.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_Agg\AggToolBar.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggToolBar.h
InputPath=Agg\AggToolBar.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggToolBar.cpp
"..\..\..\build\debug\mocs\Gui\moc_AggToolBar.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_Agg\AggToolBar.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggToolBar.h
InputPath=Agg\AggToolBar.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggToolBar.cpp
"..\..\..\build\release\mocs\Gui\moc_AggToolBar.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=Agg\AggTrussToolBar.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_Agg\AggTrussToolBar.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggTrussToolBar.h
InputPath=Agg\AggTrussToolBar.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggTrussToolBar.cpp
"..\..\..\build\debug\mocs\Gui\moc_AggTrussToolBar.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_Agg\AggTrussToolBar.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggTrussToolBar.h
InputPath=Agg\AggTrussToolBar.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggTrussToolBar.cpp
"..\..\..\build\release\mocs\Gui\moc_AggTrussToolBar.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=Agg\AggTrussWindowButton.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_Agg\AggTrussWindowButton.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggTrussWindowButton.h
InputPath=Agg\AggTrussWindowButton.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_AggTrussWindowButton.cpp
"..\..\..\build\debug\mocs\Gui\moc_AggTrussWindowButton.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_Agg\AggTrussWindowButton.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Agg\AggTrussWindowButton.h
InputPath=Agg\AggTrussWindowButton.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_AggTrussWindowButton.cpp
"..\..\..\build\release\mocs\Gui\moc_AggTrussWindowButton.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=Agg\AggWidget.h
# End Source File

# Begin Source File
SOURCE=CalcDataWidget.ui.h
# End Source File

# Begin Source File
SOURCE=FermaNextMainWindow.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_FermaNextMainWindow.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on FermaNextMainWindow.h
InputPath=FermaNextMainWindow.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_FermaNextMainWindow.cpp
"..\..\..\build\debug\mocs\Gui\moc_FermaNextMainWindow.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_FermaNextMainWindow.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on FermaNextMainWindow.h
InputPath=FermaNextMainWindow.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_FermaNextMainWindow.cpp
"..\..\..\build\release\mocs\Gui\moc_FermaNextMainWindow.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=GeometryTabWidget.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_GeometryTabWidget.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on GeometryTabWidget.h
InputPath=GeometryTabWidget.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_GeometryTabWidget.cpp
"..\..\..\build\debug\mocs\Gui\moc_GeometryTabWidget.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_GeometryTabWidget.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on GeometryTabWidget.h
InputPath=GeometryTabWidget.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_GeometryTabWidget.cpp
"..\..\..\build\release\mocs\Gui\moc_GeometryTabWidget.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=ProjectToolBox.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_ProjectToolBox.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ProjectToolBox.h
InputPath=ProjectToolBox.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_ProjectToolBox.cpp
"..\..\..\build\debug\mocs\Gui\moc_ProjectToolBox.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_ProjectToolBox.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ProjectToolBox.h
InputPath=ProjectToolBox.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_ProjectToolBox.cpp
"..\..\..\build\release\mocs\Gui\moc_ProjectToolBox.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=Splash.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_Splash.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Splash.h
InputPath=Splash.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_Splash.cpp
"..\..\..\build\debug\mocs\Gui\moc_Splash.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_Splash.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Splash.h
InputPath=Splash.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_Splash.cpp
"..\..\..\build\release\mocs\Gui\moc_Splash.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=ToolBarManager.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_ToolBarManager.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ToolBarManager.h
InputPath=ToolBarManager.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_ToolBarManager.cpp
"..\..\..\build\debug\mocs\Gui\moc_ToolBarManager.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_ToolBarManager.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ToolBarManager.h
InputPath=ToolBarManager.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_ToolBarManager.cpp
"..\..\..\build\release\mocs\Gui\moc_ToolBarManager.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=TrussDesignerWidget.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_TrussDesignerWidget.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussDesignerWidget.h
InputPath=TrussDesignerWidget.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_TrussDesignerWidget.cpp
"..\..\..\build\debug\mocs\Gui\moc_TrussDesignerWidget.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_TrussDesignerWidget.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on TrussDesignerWidget.h
InputPath=TrussDesignerWidget.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_TrussDesignerWidget.cpp
"..\..\..\build\release\mocs\Gui\moc_TrussDesignerWidget.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=UndoRedoListBox.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_UndoRedoListBox.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on UndoRedoListBox.h
InputPath=UndoRedoListBox.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_UndoRedoListBox.cpp
"..\..\..\build\debug\mocs\Gui\moc_UndoRedoListBox.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_UndoRedoListBox.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on UndoRedoListBox.h
InputPath=UndoRedoListBox.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_UndoRedoListBox.cpp
"..\..\..\build\release\mocs\Gui\moc_UndoRedoListBox.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=WindowListBox.h
!IF"$(CFG)" == "Gui - Win32 Debug"

USERDEP_WindowListBox.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on WindowListBox.h
InputPath=WindowListBox.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\debug\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\debug\mocs\Gui\moc_WindowListBox.cpp
"..\..\..\build\debug\mocs\Gui\moc_WindowListBox.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Gui - Win32 Release"

USERDEP_WindowListBox.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on WindowListBox.h
InputPath=WindowListBox.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/Qt3Support" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"." -I"Agg" -I"..\..\Common" -I"..\..\Common\Plugin" -I"..\..\Common\Loader" -I"..\TrussUnit" -I".." -I"$(QTDIR)/include/ActiveQt" -I"..\..\..\build\release\mocs\Gui" -I"..\..\..\build\release\ui\Gui" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\..\build\release\mocs\Gui\moc_WindowListBox.cpp
"..\..\..\build\release\mocs\Gui\moc_WindowListBox.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# End Group

# Begin Group "Generated"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggButton.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggPaintThread.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggPopupHint.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggToolBar.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggTrussToolBar.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_AggTrussWindowButton.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_FermaNextMainWindow.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_GeometryTabWidget.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_ProjectToolBox.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_Splash.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_ToolBarManager.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_TrussDesignerWidget.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_UndoRedoListBox.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\debug\mocs\Gui\moc_WindowListBox.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"


!ELSEIF"$(CFG)" == "Gui - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_AggButton.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_AggPaintThread.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_AggPopupHint.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_AggToolBar.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_AggTrussToolBar.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_AggTrussWindowButton.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_FermaNextMainWindow.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_GeometryTabWidget.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_ProjectToolBox.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_Splash.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_ToolBarManager.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_TrussDesignerWidget.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_UndoRedoListBox.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\..\build\release\mocs\Gui\moc_WindowListBox.cpp
!IF"$(CFG)" == "Gui - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Gui - Win32 Release"


!ENDIF
# End Source File

# End Group


# End Target
# End Project
