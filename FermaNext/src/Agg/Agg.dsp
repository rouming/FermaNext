# Microsoft Developer Studio Project File - Name="Agg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Agg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Agg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Agg.mak" CFG="Agg - Win32 Debug"
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
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Agg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Release"
# PROP Intermediate_Dir "../../Release"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../common" /I "$(QTDIR)\include" /I "$(QTDIR)\mkspecs\win32-msvc" /I "$(AGGDIR)\include" /I "$(AGGDIR)\font_freetype" /I "$(AGGDIR)\font_win32_tt" /I "$(AGGDIR)\svg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\build\Agg.lib"

!ELSEIF  "$(CFG)" == "Agg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Debug"
# PROP Intermediate_Dir "../../Debug"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /I "$(QTDIR)\include" /I "$(QTDIR)\mkspecs\win32-msvc" /I "$(AGGDIR)\include" /I "$(AGGDIR)\font_freetype" /I "$(AGGDIR)\font_win32_tt" /I "$(AGGDIR)\svg" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /D "BENCHMARK_ON" /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\build\Agg.lib"

!ENDIF 

# Begin Target

# Name "Agg - Win32 Release"
# Name "Agg - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AggButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AggQWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\AggSubsidiary.cpp
# End Source File
# Begin Source File

SOURCE=.\AggToolBar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AggButton.h

!IF  "$(CFG)" == "Agg - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\AggButton.h
InputName=AggButton

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Agg - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\AggButton.h
InputName=AggButton

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AggQWidget.h
# End Source File
# Begin Source File

SOURCE=.\AggSubsidiary.h
# End Source File
# Begin Source File

SOURCE=.\AggToolBar.h

!IF  "$(CFG)" == "Agg - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\AggToolBar.h
InputName=AggToolBar

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Agg - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\AggToolBar.h
InputName=AggToolBar

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Moc Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\mocs\moc_AggButton.cpp
# End Source File
# Begin Source File

SOURCE=.\mocs\moc_AggToolBar.cpp
# End Source File
# End Group
# End Target
# End Project
