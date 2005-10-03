# Microsoft Developer Studio Project File - Name="common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=common - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "common.mak" CFG="common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "common - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "common - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "common - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(QTDIR)\include" /I "$(QTDIR)\mkspecs\win32-msvc" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\build\common.lib"

!ELSEIF  "$(CFG)" == "common - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /I "$(QTDIR)\mkspecs\win32-msvc" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_ACCESSIBILITY_SUPPORT" /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\build\common.lib"

!ENDIF 

# Begin Target

# Name "common - Win32 Release"
# Name "common - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\Benchmark.cpp
# End Source File
# Begin Source File

SOURCE=.\DynaLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectState.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectStateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StatefulObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SubsidiaryConstants.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\AbstractObjectAction.h

!IF  "$(CFG)" == "common - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\AbstractObjectAction.h
InputName=AbstractObjectAction

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "common - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\AbstractObjectAction.h
InputName=AbstractObjectAction

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Benchmark.h
# End Source File
# Begin Source File

SOURCE=.\DynaLoader.h
# End Source File
# Begin Source File

SOURCE=.\Geometry.h
# End Source File
# Begin Source File

SOURCE=.\ObjectState.h

!IF  "$(CFG)" == "common - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\ObjectState.h
InputName=ObjectState

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "common - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\ObjectState.h
InputName=ObjectState

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ObjectStateManager.h

!IF  "$(CFG)" == "common - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\ObjectStateManager.h
InputName=ObjectStateManager

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "common - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\ObjectStateManager.h
InputName=ObjectStateManager

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Plugin.h
# End Source File
# Begin Source File

SOURCE=.\PluginLoader.h
# End Source File
# Begin Source File

SOURCE=.\PluginManager.h
# End Source File
# Begin Source File

SOURCE=.\StatefulObject.h

!IF  "$(CFG)" == "common - Win32 Release"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\StatefulObject.h
InputName=StatefulObject

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "common - Win32 Debug"

# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.
InputPath=.\StatefulObject.h
InputName=StatefulObject

"$(InputDir)\mocs\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST "$(InputDir)\mocs" mkdir "$(InputDir)\mocs" 
	"%qtdir%\bin\moc.exe" -o "$(InputDir)\mocs\moc_$(InputName).cpp" "$(InputDir)\$(InputName).h" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SubsidiaryConstants.h
# End Source File
# End Group
# Begin Group "Moc Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\mocs\moc_AbstractObjectAction.cpp
# End Source File
# Begin Source File

SOURCE=.\mocs\moc_ObjectState.cpp
# End Source File
# Begin Source File

SOURCE=.\mocs\moc_ObjectStateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\mocs\moc_StatefulObject.cpp
# End Source File
# End Group
# End Target
# End Project
