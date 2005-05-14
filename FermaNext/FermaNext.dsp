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
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

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
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /Od /I "src" /I "src\common" /I "src\TrussUnit" /I "src\gui" /I "$(QTDIR)\include" /I "$(QTDIR)\mkspecs\win32-msvc" /I "$(AGGDIR)\include" /I "$(AGGDIR)\font_freetype" /I "$(AGGDIR)\font_win32_tt" /I "$(AGGDIR)\svg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /YX /FD /GZ -Zm200 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 "LibAGG.lib" "TrussUnit.lib" "qt-mt332.lib" "qtmain.lib" "opengl32.lib" "glu32.lib" "delayimp.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "imm32.lib" "winmm.lib" "wsock32.lib" "winspool.lib" /nologo /subsystem:windows /debug /machine:I386 /out:"FermaNext.exe" /pdbtype:sept /libpath:"$(QTDIR)\lib" /libpath:"build"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "FermaNext - Win32 Release"
# Name "FermaNext - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\src\FermaNext.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\FermaNextMainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\common\FermaNextProject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\common\FermaNextWorkspace.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\ProjectToolBox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\PseudoWindowListBox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\src\gui\FermaNextMainFrame.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src\gui
InputPath=.\src\gui\FermaNextMainFrame.h
InputName=FermaNextMainFrame

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\common\FermaNextProject.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src\common
InputPath=.\src\common\FermaNextProject.h
InputName=FermaNextProject

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\common\FermaNextWorkspace.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src\common
InputPath=.\src\common\FermaNextWorkspace.h
InputName=FermaNextWorkspace

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\gui\ProjectToolBox.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src\gui
InputPath=.\src\gui\ProjectToolBox.h
InputName=ProjectToolBox

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\gui\PseudoWindowListBox.h

!IF  "$(CFG)" == "FermaNext - Win32 Release"

!ELSEIF  "$(CFG)" == "FermaNext - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src\gui
InputPath=.\src\gui\PseudoWindowListBox.h
InputName=PseudoWindowListBox

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

SOURCE=.\src\gui\mocs\moc_FermaNextMainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\common\mocs\moc_FermaNextProject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\common\mocs\moc_FermaNextWorkspace.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\mocs\moc_ProjectToolBox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\mocs\moc_PseudoWindowListBox.cpp
# End Source File
# End Group
# End Target
# End Project
