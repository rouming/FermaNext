# Microsoft Developer Studio Project File - Name="Common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Common - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak" CFG="Common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Common - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Common - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\build\debug"
# PROP BASE Intermediate_Dir "..\..\build\debug\obj\Common"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\build\debug"
# PROP Intermediate_Dir "..\..\build\debug\obj\Common"
# PROP Target_Dir ""
MTL=midl
# ADD MTL /nologo /mktyplib203 /win32 /D "_DEBUG"
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include" /I "." /I "Plugin" /I "Loader" /I "$(QTDIR)/include/ActiveQt" /I "..\..\build\debug\mocs\Common" /I "..\..\build\release\ui\Common" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_LIB" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "WIN32" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /D "QT_THREAD_SUPPORT" /FD -DFERMA_VERSION=\"0.2.0-alpha\" -Zm200 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\build\release"
# PROP BASE Intermediate_Dir "..\..\build\release\obj\Common"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\build\release"
# PROP Intermediate_Dir "..\..\build\release\obj\Common"
# PROP Target_Dir ""
MTL=midl
# ADD MTL /nologo /mktyplib203 /win32 /D "NDEBUG"
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "$(QTDIR)/include/QtCore" /I "$(QTDIR)/include/QtGui" /I "$(QTDIR)/include/QtXml" /I "$(QTDIR)/include" /I "." /I "Plugin" /I "Loader" /I "$(QTDIR)/include/ActiveQt" /I "..\..\build\release\mocs\Common" /I "..\..\build\release\ui\Common" /I "$(QTDIR)/mkspecs/win32-msvc" /D "_LIB" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "WIN32" /D "QT_NO_DEBUG" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /D "QT_THREAD_SUPPORT" /FD -DFERMA_VERSION=\"0.2.0-alpha\" -Zm200 /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib

!ENDIF 

# Begin Target

# Name "Common - Win32 Debug"
# Name "Common - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=Benchmark.cpp
# End Source File
# Begin Source File

SOURCE=Config.cpp
# End Source File
# Begin Source File

SOURCE=Loader\DynaLoader.cpp
# End Source File
# Begin Source File

SOURCE=Global.cpp
# End Source File
# Begin Source File

SOURCE=ObjectState.cpp
# End Source File
# Begin Source File

SOURCE=ObjectStateManager.cpp
# End Source File
# Begin Source File

SOURCE=Plugin\Plugin.cpp
# End Source File
# Begin Source File

SOURCE=Loader\PluginLoader.cpp
# End Source File
# Begin Source File

SOURCE=Plugin\PluginManager.cpp
# End Source File
# Begin Source File

SOURCE=StatefulObject.cpp
# End Source File
# Begin Source File

SOURCE=UUIDGen.cpp
# End Source File
# Begin Source File

SOURCE=XMLSerializableObject.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=AbstractObjectAction.h

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AbstractObjectAction.h
InputPath=AbstractObjectAction.h

"..\..\build\debug\mocs\Common\moc_AbstractObjectAction.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Common\moc_AbstractObjectAction.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AbstractObjectAction.h
InputPath=AbstractObjectAction.h

"..\..\build\release\mocs\Common\moc_AbstractObjectAction.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Common\moc_AbstractObjectAction.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Benchmark.h
# End Source File
# Begin Source File

SOURCE=Plugin\CalculationInterface.h
# End Source File
# Begin Source File

SOURCE=Plugin\CalculationJavaPlugin.h
# End Source File
# Begin Source File

SOURCE=Plugin\CalculationNativePlugin.h
# End Source File
# Begin Source File

SOURCE=Config.h

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Config.h
InputPath=Config.h

"..\..\build\debug\mocs\Common\moc_Config.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Common\moc_Config.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Config.h
InputPath=Config.h

"..\..\build\release\mocs\Common\moc_Config.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Common\moc_Config.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Loader\DynaLoader.h
# End Source File
# Begin Source File

SOURCE=Geometry.h
# End Source File
# Begin Source File

SOURCE=Global.h
# End Source File
# Begin Source File

SOURCE=Plugin\JavaPlugin.h
# End Source File
# Begin Source File

SOURCE=Log4CXX.h
# End Source File
# Begin Source File

SOURCE=Plugin\NativePlugin.h
# End Source File
# Begin Source File

SOURCE=Plugin\NativePluginFrontEnd.h
# End Source File
# Begin Source File

SOURCE=ObjectState.h

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ObjectState.h
InputPath=ObjectState.h

"..\..\build\debug\mocs\Common\moc_ObjectState.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Common\moc_ObjectState.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ObjectState.h
InputPath=ObjectState.h

"..\..\build\release\mocs\Common\moc_ObjectState.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Common\moc_ObjectState.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=ObjectStateManager.h

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ObjectStateManager.h
InputPath=ObjectStateManager.h

"..\..\build\debug\mocs\Common\moc_ObjectStateManager.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Common\moc_ObjectStateManager.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ObjectStateManager.h
InputPath=ObjectStateManager.h

"..\..\build\release\mocs\Common\moc_ObjectStateManager.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Common\moc_ObjectStateManager.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Plugin\OptimizationInterface.h
# End Source File
# Begin Source File

SOURCE=Plugin\Plugin.h

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Plugin\Plugin.h
InputPath=Plugin\Plugin.h

"..\..\build\debug\mocs\Common\moc_Plugin.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Common\moc_Plugin.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Plugin\Plugin.h
InputPath=Plugin\Plugin.h

"..\..\build\release\mocs\Common\moc_Plugin.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Common\moc_Plugin.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Loader\PluginLoader.h
# End Source File
# Begin Source File

SOURCE=Plugin\PluginManager.h

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Plugin\PluginManager.h
InputPath=Plugin\PluginManager.h

"..\..\build\debug\mocs\Common\moc_PluginManager.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Common\moc_PluginManager.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on Plugin\PluginManager.h
InputPath=Plugin\PluginManager.h

"..\..\build\release\mocs\Common\moc_PluginManager.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Common\moc_PluginManager.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=StatefulObject.h

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on StatefulObject.h
InputPath=StatefulObject.h

"..\..\build\debug\mocs\Common\moc_StatefulObject.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\Common\moc_StatefulObject.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on StatefulObject.h
InputPath=StatefulObject.h

"..\..\build\release\mocs\Common\moc_StatefulObject.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include" -I"." -I"Plugin" -I"Loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\Common" -I"..\..\build\release\ui\Common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\Common\moc_StatefulObject.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=UUIDGen.h
# End Source File
# Begin Source File

SOURCE=UUIDObject.h
# End Source File
# Begin Source File

SOURCE=XMLSerializableObject.h
# End Source File
# End Group
# Begin Group "Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\build\debug\mocs\Common\moc_AbstractObjectAction.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\Common\moc_AbstractObjectAction.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\Common\moc_Config.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\Common\moc_Config.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\Common\moc_ObjectState.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\Common\moc_ObjectState.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\Common\moc_ObjectStateManager.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\Common\moc_ObjectStateManager.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\Common\moc_Plugin.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\Common\moc_Plugin.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\Common\moc_PluginManager.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\Common\moc_PluginManager.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\debug\mocs\Common\moc_StatefulObject.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\build\release\mocs\Common\moc_StatefulObject.cpp

!IF  "$(CFG)" == "Common - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Release"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
