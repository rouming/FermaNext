# Microsoft Developer Studio Project File - Name="Agg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Agg - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Agg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Agg.mak" CFG="Agg - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Agg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Agg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl
MTL=midl
RSC=rc
BSC32=bscmake.exe

!IF  "$(CFG)" == "Agg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\build\release"
# PROP BASE Intermediate_Dir "..\..\build\release\obj\Agg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\build\release"
# PROP Intermediate_Dir "..\..\build\release\obj\Agg"
# PROP Target_Dir ""
# ADD CPP /I"$(QTDIR)/include/QtGui" /I"$(QTDIR)/include/QtCore" /I"$(QTDIR)/include" /I"$(AGGDIR)/include" /I"$(AGGDIR)/svg" /I"..\common" /I"$(QTDIR)/include/ActiveQt" /I"..\..\build\release\mocs\Agg" /I"..\..\build\debug\ui\Agg" /I"$(QTDIR)/mkspecs/win32-msvc" /c /FD -nologo -Zm200 -O1 -MD -O1 -MD -W3 -GR -GX  /D "_LIB"  /D UNICODE /D QT_LARGEFILE_SUPPORT /D WIN32 /D QT_THREAD_SUPPORT /D QT_NO_DEBUG /D QT_CORE_LIB /D QT_GUI_LIB  /D "WIN32"  
# ADD MTL /nologo /mktyplib203 /win32 /D "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
# ADD BSC32 /nologo
LIB32=lib /NOLOGO
# ADD LIB32 /out:"..\..\build\release\Agg.lib" 

!ELSEIF  "$(CFG)" == "Agg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\build\debug"
# PROP BASE Intermediate_Dir "..\..\build\debug\obj\Agg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\build\debug"
# PROP Intermediate_Dir "..\..\build\debug\obj\Agg"
# PROP Target_Dir ""
# ADD CPP /I"$(QTDIR)/include/QtGui" /I"$(QTDIR)/include/QtCore" /I"$(QTDIR)/include" /I"$(AGGDIR)/include" /I"$(AGGDIR)/svg" /I"..\common" /I"$(QTDIR)/include/ActiveQt" /I"..\..\build\debug\mocs\Agg" /I"..\..\build\debug\ui\Agg" /I"$(QTDIR)/mkspecs/win32-msvc" /c /FD -nologo -Zm200 -Zi -MDd -Zi -MDd -W3 -GR -GX  /D "_LIB"  /D UNICODE /D QT_LARGEFILE_SUPPORT /D WIN32 /D QT_THREAD_SUPPORT /D QT_CORE_LIB /D QT_GUI_LIB  /D "WIN32"  
# ADD MTL /nologo /mktyplib203 /win32 /D "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
# ADD BSC32 /nologo
LIB32=lib /NOLOGO
# ADD LIB32 /out:"..\..\build\debug\Agg.lib" 

!ENDIF 

# Begin Target

# Name "Agg - Win32 Release"
# Name "Agg - Win32 Debug"

# Begin Group "Source Files"
# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE=AggButton.cpp
# End Source File

# Begin Source File
SOURCE=AggPaintThread.cpp
# End Source File

# Begin Source File
SOURCE=AggPopupHint.cpp
# End Source File

# Begin Source File
SOURCE=AggQWidget.cpp
# End Source File

# Begin Source File
SOURCE=AggSubsidiary.cpp
# End Source File

# Begin Source File
SOURCE=AggToolBar.cpp
# End Source File

# End Group

# Begin Group "Header Files"
# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File
SOURCE=AggButton.h
!IF"$(CFG)" == "Agg - Win32 Release"

USERDEP_AggButton.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AggButton.h
InputPath=AggButton.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Agg" -I"..\..\build\debug\ui\Agg" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Agg\moc_AggButton.cpp
"..\..\build\release\mocs\Agg\moc_AggButton.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Agg - Win32 Debug"

USERDEP_AggButton.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AggButton.h
InputPath=AggButton.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Agg" -I"..\..\build\debug\ui\Agg" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Agg\moc_AggButton.cpp
"..\..\build\debug\mocs\Agg\moc_AggButton.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=AggPaintThread.h
!IF"$(CFG)" == "Agg - Win32 Release"

USERDEP_AggPaintThread.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AggPaintThread.h
InputPath=AggPaintThread.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Agg" -I"..\..\build\debug\ui\Agg" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Agg\moc_AggPaintThread.cpp
"..\..\build\release\mocs\Agg\moc_AggPaintThread.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Agg - Win32 Debug"

USERDEP_AggPaintThread.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AggPaintThread.h
InputPath=AggPaintThread.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Agg" -I"..\..\build\debug\ui\Agg" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Agg\moc_AggPaintThread.cpp
"..\..\build\debug\mocs\Agg\moc_AggPaintThread.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=AggPopupHint.h
!IF"$(CFG)" == "Agg - Win32 Release"

USERDEP_AggPopupHint.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AggPopupHint.h
InputPath=AggPopupHint.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Agg" -I"..\..\build\debug\ui\Agg" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Agg\moc_AggPopupHint.cpp
"..\..\build\release\mocs\Agg\moc_AggPopupHint.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Agg - Win32 Debug"

USERDEP_AggPopupHint.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AggPopupHint.h
InputPath=AggPopupHint.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Agg" -I"..\..\build\debug\ui\Agg" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Agg\moc_AggPopupHint.cpp
"..\..\build\debug\mocs\Agg\moc_AggPopupHint.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=AggQWidget.h
# End Source File

# Begin Source File
SOURCE=AggSubsidiary.h
# End Source File

# Begin Source File
SOURCE=AggToolBar.h
!IF"$(CFG)" == "Agg - Win32 Release"

USERDEP_AggToolBar.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AggToolBar.h
InputPath=AggToolBar.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Agg" -I"..\..\build\debug\ui\Agg" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Agg\moc_AggToolBar.cpp
"..\..\build\release\mocs\Agg\moc_AggToolBar.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "Agg - Win32 Debug"

USERDEP_AggToolBar.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AggToolBar.h
InputPath=AggToolBar.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_THREAD_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"$(AGGDIR)/include" -I"$(AGGDIR)/svg" -I"..\common" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Agg" -I"..\..\build\debug\ui\Agg" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Agg\moc_AggToolBar.cpp
"..\..\build\debug\mocs\Agg\moc_AggToolBar.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# End Group

# Begin Group "Generated"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\build\debug\mocs\Agg\moc_AggButton.cpp
!IF"$(CFG)" == "Agg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Agg - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\debug\mocs\Agg\moc_AggPaintThread.cpp
!IF"$(CFG)" == "Agg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Agg - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\debug\mocs\Agg\moc_AggPopupHint.cpp
!IF"$(CFG)" == "Agg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Agg - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\debug\mocs\Agg\moc_AggToolBar.cpp
!IF"$(CFG)" == "Agg - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "Agg - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\Agg\moc_AggButton.cpp
!IF"$(CFG)" == "Agg - Win32 Release"


!ELSEIF"$(CFG)" == "Agg - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\Agg\moc_AggPaintThread.cpp
!IF"$(CFG)" == "Agg - Win32 Release"


!ELSEIF"$(CFG)" == "Agg - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\Agg\moc_AggPopupHint.cpp
!IF"$(CFG)" == "Agg - Win32 Release"


!ELSEIF"$(CFG)" == "Agg - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\Agg\moc_AggToolBar.cpp
!IF"$(CFG)" == "Agg - Win32 Release"


!ELSEIF"$(CFG)" == "Agg - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# End Group


# End Target
# End Project
