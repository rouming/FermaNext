# Microsoft Developer Studio Project File - Name="common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=common - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "common.mak" CFG="common - Win32 Release"
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
CPP=cl
MTL=midl
RSC=rc
BSC32=bscmake.exe

!IF  "$(CFG)" == "common - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\build\release"
# PROP BASE Intermediate_Dir "..\..\build\release\obj\common"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\build\release"
# PROP Intermediate_Dir "..\..\build\release\obj\common"
# PROP Target_Dir ""
# ADD CPP /I"$(QTDIR)/include/QtXml" /I"$(QTDIR)/include/QtGui" /I"$(QTDIR)/include/QtCore" /I"$(QTDIR)/include" /I"." /I"plugin" /I"plugin\loader" /I"$(QTDIR)/include/ActiveQt" /I"..\..\build\release\mocs\common" /I"..\..\build\debug\ui\common" /I"$(QTDIR)/mkspecs/win32-msvc" /c /FD -nologo -Zm200 -O1 -MD -O1 -MD -W3 -GR -GX  /D "_LIB"  /D UNICODE /D QT_LARGEFILE_SUPPORT /D WIN32 /D QT_NO_DEBUG /D QT_CORE_LIB /D QT_GUI_LIB /D QT_XML_LIB /D QT_THREAD_SUPPORT  /D "WIN32"  
# ADD MTL /nologo /mktyplib203 /win32 /D "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
# ADD BSC32 /nologo
LIB32=lib /NOLOGO
# ADD LIB32 /out:"..\..\build\release\common.lib" 

!ELSEIF  "$(CFG)" == "common - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\build\debug"
# PROP BASE Intermediate_Dir "..\..\build\debug\obj\common"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\build\debug"
# PROP Intermediate_Dir "..\..\build\debug\obj\common"
# PROP Target_Dir ""
# ADD CPP /I"$(QTDIR)/include/QtXml" /I"$(QTDIR)/include/QtGui" /I"$(QTDIR)/include/QtCore" /I"$(QTDIR)/include" /I"." /I"plugin" /I"plugin\loader" /I"$(QTDIR)/include/ActiveQt" /I"..\..\build\debug\mocs\common" /I"..\..\build\debug\ui\common" /I"$(QTDIR)/mkspecs/win32-msvc" /c /FD -nologo -Zm200 -Zi -MDd -Zi -MDd -W3 -GR -GX  /D "_LIB"  /D UNICODE /D QT_LARGEFILE_SUPPORT /D WIN32 /D QT_CORE_LIB /D QT_GUI_LIB /D QT_XML_LIB /D QT_THREAD_SUPPORT  /D "WIN32"  
# ADD MTL /nologo /mktyplib203 /win32 /D "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
# ADD BSC32 /nologo
LIB32=lib /NOLOGO
# ADD LIB32 /out:"..\..\build\debug\common.lib" 

!ENDIF 

# Begin Target

# Name "common - Win32 Release"
# Name "common - Win32 Debug"

# Begin Group "Source Files"
# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE=Benchmark.cpp
# End Source File

# Begin Source File
SOURCE=ObjectState.cpp
# End Source File

# Begin Source File
SOURCE=ObjectStateManager.cpp
# End Source File

# Begin Source File
SOURCE=StatefulObject.cpp
# End Source File

# Begin Source File
SOURCE=SubsidiaryConstants.cpp
# End Source File

# Begin Source File
SOURCE=UUIDGen.cpp
# End Source File

# Begin Source File
SOURCE=XMLSerializableObject.cpp
# End Source File

# Begin Source File
SOURCE=plugin\Plugin.cpp
# End Source File

# Begin Source File
SOURCE=plugin\PluginManager.cpp
# End Source File

# Begin Source File
SOURCE=plugin\loader\DynaLoader.cpp
# End Source File

# Begin Source File
SOURCE=plugin\loader\PluginLoader.cpp
# End Source File

# End Group

# Begin Group "Header Files"
# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File
SOURCE=AbstractObjectAction.h
!IF"$(CFG)" == "common - Win32 Release"

USERDEP_AbstractObjectAction.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AbstractObjectAction.h
InputPath=AbstractObjectAction.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\common\moc_AbstractObjectAction.cpp
"..\..\build\release\mocs\common\moc_AbstractObjectAction.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "common - Win32 Debug"

USERDEP_AbstractObjectAction.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on AbstractObjectAction.h
InputPath=AbstractObjectAction.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\common\moc_AbstractObjectAction.cpp
"..\..\build\debug\mocs\common\moc_AbstractObjectAction.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=Benchmark.h
# End Source File

# Begin Source File
SOURCE=Geometry.h
# End Source File

# Begin Source File
SOURCE=Log4CXX.h
# End Source File

# Begin Source File
SOURCE=ObjectState.h
!IF"$(CFG)" == "common - Win32 Release"

USERDEP_ObjectState.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ObjectState.h
InputPath=ObjectState.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\common\moc_ObjectState.cpp
"..\..\build\release\mocs\common\moc_ObjectState.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "common - Win32 Debug"

USERDEP_ObjectState.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ObjectState.h
InputPath=ObjectState.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\common\moc_ObjectState.cpp
"..\..\build\debug\mocs\common\moc_ObjectState.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=ObjectStateManager.h
!IF"$(CFG)" == "common - Win32 Release"

USERDEP_ObjectStateManager.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ObjectStateManager.h
InputPath=ObjectStateManager.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\common\moc_ObjectStateManager.cpp
"..\..\build\release\mocs\common\moc_ObjectStateManager.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "common - Win32 Debug"

USERDEP_ObjectStateManager.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on ObjectStateManager.h
InputPath=ObjectStateManager.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\common\moc_ObjectStateManager.cpp
"..\..\build\debug\mocs\common\moc_ObjectStateManager.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=StatefulObject.h
!IF"$(CFG)" == "common - Win32 Release"

USERDEP_StatefulObject.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on StatefulObject.h
InputPath=StatefulObject.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\common\moc_StatefulObject.cpp
"..\..\build\release\mocs\common\moc_StatefulObject.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "common - Win32 Debug"

USERDEP_StatefulObject.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on StatefulObject.h
InputPath=StatefulObject.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\common\moc_StatefulObject.cpp
"..\..\build\debug\mocs\common\moc_StatefulObject.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=SubsidiaryConstants.h
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

# Begin Source File
SOURCE=plugin\CalculationInterface.h
# End Source File

# Begin Source File
SOURCE=plugin\CalculationJavaPlugin.h
# End Source File

# Begin Source File
SOURCE=plugin\CalculationNativePlugin.h
# End Source File

# Begin Source File
SOURCE=plugin\JavaPlugin.h
# End Source File

# Begin Source File
SOURCE=plugin\NativePlugin.h
# End Source File

# Begin Source File
SOURCE=plugin\NativePluginFrontEnd.h
# End Source File

# Begin Source File
SOURCE=plugin\OptimizationInterface.h
# End Source File

# Begin Source File
SOURCE=plugin\Plugin.h
!IF"$(CFG)" == "common - Win32 Release"

USERDEP_plugin\Plugin.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on plugin\Plugin.h
InputPath=plugin\Plugin.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\common\moc_Plugin.cpp
"..\..\build\release\mocs\common\moc_Plugin.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "common - Win32 Debug"

USERDEP_plugin\Plugin.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on plugin\Plugin.h
InputPath=plugin\Plugin.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\common\moc_Plugin.cpp
"..\..\build\debug\mocs\common\moc_Plugin.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=plugin\PluginManager.h
!IF"$(CFG)" == "common - Win32 Release"

USERDEP_plugin\PluginManager.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on plugin\PluginManager.h
InputPath=plugin\PluginManager.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\release\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\release\mocs\common\moc_PluginManager.cpp
"..\..\build\release\mocs\common\moc_PluginManager.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF"$(CFG)" == "common - Win32 Debug"

USERDEP_plugin\PluginManager.h=
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on plugin\PluginManager.h
InputPath=plugin\PluginManager.h
BuildCmds=  \
	$(QTDIR)\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DWIN32 -DQT_CORE_LIB -DQT_GUI_LIB -DQT_XML_LIB -DQT_THREAD_SUPPORT -I"$(QTDIR)/include/QtXml" -I"$(QTDIR)/include/QtGui" -I"$(QTDIR)/include/QtCore" -I"$(QTDIR)/include" -I"." -I"plugin" -I"plugin\loader" -I"$(QTDIR)/include/ActiveQt" -I"..\..\build\debug\mocs\common" -I"..\..\build\debug\ui\common" -I"$(QTDIR)/mkspecs/win32-msvc" -D_MSC_VER=1200 -DWIN32 $(InputPath) -o ..\..\build\debug\mocs\common\moc_PluginManager.cpp
"..\..\build\debug\mocs\common\moc_PluginManager.cpp": $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF
# End Source File

# Begin Source File
SOURCE=plugin\loader\DynaLoader.h
# End Source File

# Begin Source File
SOURCE=plugin\loader\PluginLoader.h
# End Source File

# End Group

# Begin Group "Generated"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\build\debug\mocs\common\moc_AbstractObjectAction.cpp
!IF"$(CFG)" == "common - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "common - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\debug\mocs\common\moc_ObjectState.cpp
!IF"$(CFG)" == "common - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "common - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\debug\mocs\common\moc_ObjectStateManager.cpp
!IF"$(CFG)" == "common - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "common - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\debug\mocs\common\moc_Plugin.cpp
!IF"$(CFG)" == "common - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "common - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\debug\mocs\common\moc_PluginManager.cpp
!IF"$(CFG)" == "common - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "common - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\debug\mocs\common\moc_StatefulObject.cpp
!IF"$(CFG)" == "common - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF"$(CFG)" == "common - Win32 Debug"


!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\common\moc_AbstractObjectAction.cpp
!IF"$(CFG)" == "common - Win32 Release"


!ELSEIF"$(CFG)" == "common - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\common\moc_ObjectState.cpp
!IF"$(CFG)" == "common - Win32 Release"


!ELSEIF"$(CFG)" == "common - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\common\moc_ObjectStateManager.cpp
!IF"$(CFG)" == "common - Win32 Release"


!ELSEIF"$(CFG)" == "common - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\common\moc_Plugin.cpp
!IF"$(CFG)" == "common - Win32 Release"


!ELSEIF"$(CFG)" == "common - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\common\moc_PluginManager.cpp
!IF"$(CFG)" == "common - Win32 Release"


!ELSEIF"$(CFG)" == "common - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# Begin Source File
SOURCE=..\..\build\release\mocs\common\moc_StatefulObject.cpp
!IF"$(CFG)" == "common - Win32 Release"


!ELSEIF"$(CFG)" == "common - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF
# End Source File

# End Group


# End Target
# End Project
