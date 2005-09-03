# Microsoft Developer Studio Project File - Name="FermaNext" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

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
!MESSAGE "FermaNext - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "FermaNext - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Release"
# PROP Intermediate_Dir "../../Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "." /I "..\common" /I "..\TrussUnit" /I "..\gui" /I "$(QTDIR)\include" /I "$(QTDIR)\mkspecs\win32-msvc" /I "$(AGGDIR)\include" /I "$(AGGDIR)\font_freetype" /I "$(AGGDIR)\font_win32_tt" /I "$(AGGDIR)\svg" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 LibAGG.lib LibSVG.lib libexpat.lib TrussUnit.lib qt-mt.lib qtmain_rel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../../FermaNext.exe" /libpath:"../../" /libpath:"../../build" /libpath:"$(QTDIR)\lib" /libpath:"$(EXPATDIR)/Libs"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Debug"
# PROP Intermediate_Dir "../../Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /Od /I "." /I "..\common" /I "..\TrussUnit" /I "..\gui" /I "$(QTDIR)\include" /I "$(QTDIR)\mkspecs\win32-msvc" /I "$(AGGDIR)\include" /I "$(AGGDIR)\font_freetype" /I "$(AGGDIR)\font_win32_tt" /I "$(AGGDIR)\svg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /YX /FD /GZ -Zm200 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 LibAGG.lib LibSVG.lib libexpat.lib TrussUnit.lib qt-mt332.lib qtmain.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib winspool.lib /nologo /subsystem:console /debug /machine:I386 /out:"../../FermaNext.exe" /pdbtype:sept /libpath:"../../" /libpath:"../../build" /libpath:"$(QTDIR)\lib" /libpath:"$(EXPATDIR)/Libs"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "FermaNext - Win32 Release"
# Name "FermaNext - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\..\common\AggQWidget.cpp
# End Source File
# Begin Source File

SOURCE=..\common\AggSubsidiary.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\Benchmark.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\CalcDataToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\DynaLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\FermaNext.cpp
# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\FermaNextConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\FermaNextMainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\FermaNextProject.cpp
# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\FermaNextWorkspace.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\ObjectState.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\ObjectStateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\PluginLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\ProjectToolBox.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\StatefulObject.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\SubsidiaryConstants.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\ToolBarManager.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\WindowListBox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\..\common\AbstractObjectAction.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\common
InputPath=.\..\common\AbstractObjectAction.h
InputName=AbstractObjectAction

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\common
InputPath=.\..\common\AbstractObjectAction.h
InputName=AbstractObjectAction

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\common\AggQWidget.h
# End Source File
# Begin Source File

SOURCE=..\common\AggSubsidiary.h
# End Source File
# Begin Source File

SOURCE=.\..\common\Benchmark.h
# End Source File
# Begin Source File

SOURCE=.\..\gui\CalcDataToolBar.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\CalcDataToolBar.h
InputName=CalcDataToolBar

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\CalcDataToolBar.h
InputName=CalcDataToolBar

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\common\DynaLoader.h
# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\FermaNextConfig.h
# End Source File
# Begin Source File

SOURCE=.\..\gui\FermaNextMainFrame.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\FermaNextMainFrame.h
InputName=FermaNextMainFrame

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\FermaNextMainFrame.h
InputName=FermaNextMainFrame

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\FermaNextPlugin.h
# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\FermaNextProject.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\..\FermaNext\FermaNextProject.h
InputName=FermaNextProject

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\..\FermaNext\FermaNextProject.h
InputName=FermaNextProject

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\FermaNextWorkspace.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\..\FermaNext\FermaNextWorkspace.h
InputName=FermaNextWorkspace

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\..\FermaNext\FermaNextWorkspace.h
InputName=FermaNextWorkspace

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\common\ObjectState.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\common
InputPath=.\..\common\ObjectState.h
InputName=ObjectState

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\common
InputPath=.\..\common\ObjectState.h
InputName=ObjectState

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\common\ObjectStateManager.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\common
InputPath=.\..\common\ObjectStateManager.h
InputName=ObjectStateManager

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\common
InputPath=.\..\common\ObjectStateManager.h
InputName=ObjectStateManager

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\common\PluginLoader.h
# End Source File
# Begin Source File

SOURCE=.\..\gui\ProjectToolBox.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\ProjectToolBox.h
InputName=ProjectToolBox

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\ProjectToolBox.h
InputName=ProjectToolBox

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\common\StatefulObject.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\common
InputPath=.\..\common\StatefulObject.h
InputName=StatefulObject

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\common
InputPath=.\..\common\StatefulObject.h
InputName=StatefulObject

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\common\SubsidiaryConstants.h
# End Source File
# Begin Source File

SOURCE=.\..\gui\ToolBarManager.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\ToolBarManager.h
InputName=ToolBarManager

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\ToolBarManager.h
InputName=ToolBarManager

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\..\gui\WindowListBox.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\WindowListBox.h
InputName=WindowListBox

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=\ferma\Source\FermaNext\src\gui
InputPath=.\..\gui\WindowListBox.h
InputName=WindowListBox

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Moc files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\..\common\mocs\moc_AbstractObjectAction.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\mocs\moc_CalcDataToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\mocs\moc_FermaNextMainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\mocs\moc_FermaNextProject.cpp
# End Source File
# Begin Source File

SOURCE=.\..\FermaNext\mocs\moc_FermaNextWorkspace.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\mocs\moc_ObjectState.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\mocs\moc_ObjectStateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\mocs\moc_ProjectToolBox.cpp
# End Source File
# Begin Source File

SOURCE=.\..\common\mocs\moc_StatefulObject.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\mocs\moc_ToolBarManager.cpp
# End Source File
# Begin Source File

SOURCE=.\..\gui\mocs\moc_WindowListBox.cpp
# End Source File
# End Group
# End Target
# End Project
